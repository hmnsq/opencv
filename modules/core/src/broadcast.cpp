// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

// Implementation of cv::BroadcastOp (declared in opencv2/core/mat.hpp): an op-agnostic driver for
// broadcasting element-wise traversal. It computes the numpy-broadcast iteration space over a flat
// list of operand Mats, collapses contiguous dims, partitions into tiles for parallel_for_, and hands
// each 2D tile's per-operand slices to a `body` callback (which owns all semantics).

#অন্তর্ভাব "precomp.hpp"
#অন্তর্ভাব <algorithm>
#অন্তর্ভাব <array>
#অন্তর্ভাব <climits>
#অন্তর্ভাব <cmath>

নামাকাশ cv {

// Per-operand steps along the (collapsed) iteration axes are kept in a MatStep (a value-type holding
// MAX_DIMS size_t entries). Here the entries are steps in ELEMENTS (one scalar / channel value), not
// the byte steps a Mat stores - the container is reused for its fixed-size storage and [] access.

// ---------------------------------------------------------------------------
// Geometry helpers.
// ---------------------------------------------------------------------------

// How a Mat's channels are mapped into the logical (shape, step, esz1) handed to the geometry:
//   CH_FOLD : channels stay scalar-wise, folded into the innermost dim (back() *= cn, step 1).
//             Used (expandChannels=true) when no channel broadcast is needed (all single-channel,
//             or all same-cn with equal back()) - the body then sees single-channel data.
//   CH_DIM  : channels become an explicit innermost iteration dim (cn, step 1); single-channel
//             operands get a size-1 channel that broadcasts 1->N. Used when channel broadcast is
//             needed (mixed channel counts, or multichannel with differing back()).
//   CH_ELEM : channels stay inside the element (esz = full elemSize, no channel dim). Used with
//             expandChannels=false; the body handles channels itself (deinterleave fast path).
গণনা ChMode { CH_FOLD, CH_DIM, CH_ELEM };

স্থির শূন্য matLayout(ধ্রুবক Mat& m, ChMode mode, MatShape& shp, MatStep& step, পূর্ণসংখ্যা& esz1)
{
    ধ্রুবক পূর্ণসংখ্যা nd = m.dims, cn = m.channels();
    যদি (mode == CH_ELEM)
    {
        esz1 = (পূর্ণসংখ্যা)m.elem_আকার();                       // one full (cn-channel) pixel
        shp.resize(nd);
        জন্য (পূর্ণসংখ্যা i = 0; i < nd; i++) { shp[i] = m.size[i]; step[i] = m.step[i] / esz1; }
        ফেরত;
    }
    esz1 = (পূর্ণসংখ্যা)m.elemSize1();                           // one scalar (channel value)
    যদি (mode == CH_DIM)
    {
        shp.resize(nd + 1);
        জন্য (পূর্ণসংখ্যা i = 0; i < nd; i++) { shp[i] = m.size[i]; step[i] = m.step[i] / esz1; }
        shp[nd] = cn; step[nd] = 1;                      // channels = explicit innermost dim
    }
    অন্যথা // CH_FOLD
    {
        যদি (nd == 0)                                     // 0-dim scalar: channels are the only dim
        {
            shp.assign(1, cn);
            step[0] = 1;
        }
        অন্যথা
        {
            shp.resize(nd);
            জন্য (পূর্ণসংখ্যা i = 0; i < nd; i++) { shp[i] = m.size[i]; step[i] = m.step[i] / esz1; }
            shp[nd - 1] *= cn;                           // fold channels into the innermost dim
            step[nd - 1] = 1;                            // scalars are contiguous there
        }
    }
}

// A single-channel scalar (one value, cn==1, total()==1) broadcasts into everything trivially
// (step 0 on every axis incl. channels), so it must NOT force CH_DIM - it is excluded from the
// channel-mode decision entirely.
স্থির যৌক্তিক isSingleChannelScalar(ধ্রুবক Mat& m)
{
    ফেরত m.channels() == 1 && m.যোগ() == 1;
}

// Decide (globally, across all operands) how channels are presented for expandChannels=true:
// CH_FOLD when no channel broadcast is needed, CH_DIM when it is. Single-channel scalars are
// excluded first; among the rest, multichannel operands must all share the same cn (an (n,m) mix
// with both > 1 is an error).
স্থির ChMode decideChannelMode(ধ্রুবক Mat* ধ্রুবক* arrays, পূর্ণসংখ্যা K)
{
    পূর্ণসংখ্যা N = 1;                                           // the single multichannel count, if any
    জন্য (পূর্ণসংখ্যা k = 0; k < K; k++)
    {
        যদি (isSingleChannelScalar(*arrays[k])) চালু;
        পূর্ণসংখ্যা c = arrays[k]->channels();
        যদি (c > 1) { যদি (N == 1) N = c; অন্যথা CV_Assert(N == c && "ew: (n,m) channel mix unsupported"); }
    }
    যদি (N == 1) ফেরত CH_FOLD;                          // all single-channel -> fold (a no-op)

    যৌক্তিক allMulti = সত্য, sameBack = সত্য;
    পূর্ণসংখ্যা back = -1;
    জন্য (পূর্ণসংখ্যা k = 0; k < K; k++)
    {
        ধ্রুবক Mat& a = *arrays[k];
        যদি (isSingleChannelScalar(a)) চালু;
        যদি (a.channels() != N) allMulti = মিথ্যা;
        পূর্ণসংখ্যা b = a.dims > 0 ? a.size[a.dims - 1] : 1;   // 0-dim scalar has no spatial back (=1)
        যদি (back < 0) back = b; অন্যথা যদি (b != back) sameBack = মিথ্যা;
    }
    ফেরত (allMulti && sameBack) ? CH_FOLD : CH_DIM;    // fold only if no channel broadcast
}

// numpy-style broadcast of several right-aligned shapes.
স্থির যৌক্তিক broadcastShape(ধ্রুবক MatShape* shps, পূর্ণসংখ্যা K, MatShape& out)
{
    size_t nd = 0;
    জন্য (পূর্ণসংখ্যা k = 0; k < K; k++) nd = std::max(nd, shps[k].size());
    out.assign(nd, 1);
    জন্য (পূর্ণসংখ্যা k = 0; k < K; k++)
    {
        ধ্রুবক MatShape& s = shps[k];
        size_t off = nd - s.size();
        জন্য (size_t i = 0; i < s.size(); i++)
        {
            পূর্ণসংখ্যা d = s[i], &o = out[off + i];
            যদি (o == 1) o = d;
            অন্যথা যদি (d != 1 && d != o) ফেরত মিথ্যা;
        }
    }
    ফেরত সত্য;
}

// Right-align an arg's own (shp,step) to nd dims; broadcast dims get step 0.
স্থির শূন্য alignArg(ধ্রুবক MatShape& shp, ধ্রুবক MatStep& step, পূর্ণসংখ্যা nd,
                     MatStep& as, MatShape& ash)
{
    as.clear();
    ash.assign(nd, 1);
    পূর্ণসংখ্যা off = nd - (পূর্ণসংখ্যা)shp.size();
    জন্য (পূর্ণসংখ্যা i = 0; i < (পূর্ণসংখ্যা)shp.size(); i++)
    {
        পূর্ণসংখ্যা d = shp[i];
        ash[off + i] = d;
        as[off + i] = (d == 1) ? 0 : step[i];
    }
}

// Collapse adjacent dims that are contiguous (and broadcast-consistent) across all args.
স্থির পূর্ণসংখ্যা collapseDims(MatStep* S, MatShape* H, পূর্ণসংখ্যা K, MatShape& D)
{
    পূর্ণসংখ্যা nd = (পূর্ণসংখ্যা)D.size();
    যদি (nd <= 1) ফেরত nd;

    পূর্ণসংখ্যা j = nd - 1;
    জন্য (পূর্ণসংখ্যা i = j - 1; i >= 0; i--)
    {
        যৌক্তিক contig = সত্য, scalar = সত্য, consist = সত্য;
        জন্য (পূর্ণসংখ্যা k = 0; k < K; k++)
        {
            size_t st = S[k][j] * (size_t)H[k][j];
            যৌক্তিক পূর্ব_Scalar = H[k][j] == 1;
            যৌক্তিক curScalar = H[k][i] == 1;
            contig  = contig  && (st == S[k][i]);
            scalar  = scalar  && curScalar;
            consist = consist && (curScalar == পূর্ব_Scalar);
        }
        যদি (contig && (consist || scalar))
        {
            জন্য (পূর্ণসংখ্যা k = 0; k < K; k++) H[k][j] *= H[k][i];
            D[j] *= D[i];
        }
        অন্যথা
        {
            j--;
            যদি (i < j)
            {
                জন্য (পূর্ণসংখ্যা k = 0; k < K; k++) { H[k][j] = H[k][i]; S[k][j] = S[k][i]; }
                D[j] = D[i];
            }
        }
    }

    পূর্ণসংখ্যা m = nd - j;
    জন্য (পূর্ণসংখ্যা d = 0; d < m; d++)
    {
        D[d] = D[j + d];
        জন্য (পূর্ণসংখ্যা k = 0; k < K; k++) { S[k][d] = S[k][j + d]; H[k][d] = H[k][j + d]; }
    }
    D.resize(m);
    জন্য (পূর্ণসংখ্যা k = 0; k < K; k++) H[k].resize(m);

    // Zero out steps of broadcast (size-1) dims (numpy step==0 trick).
    জন্য (পূর্ণসংখ্যা d = 0; d < m; d++)
        জন্য (পূর্ণসংখ্যা k = 0; k < K; k++)
            যদি (H[k][d] == 1) S[k][d] = 0;

    ফেরত m;
}

// Fast geometry for the dominant case: every operand is either (a) an array sharing ONE common
// shape - same dims, sizes and channel count - and contiguous, or (b) a single-channel scalar
// (cn==1, total()==1). Then the whole traversal is a single contiguous 1D run of `total` scalars
// (channels folded in): arrays get stepx 1, scalars stepx 0. This skips decideChannelMode /
// broadcastShape / alignArg / collapseDims and all their per-operand buffers entirely. Returns
// false (leaving outputs untouched) when the operands don't fit, so the caller runs general
// geometry. expandChannels=false (CH_ELEM) keeps channels in the element and is left to general.
স্থির যৌক্তিক fastSameShape(ধ্রুবক Mat* ধ্রুবক* arrays, পূর্ণসংখ্যা K, যৌক্তিক expandChannels,
                          uchar** base, পূর্ণসংখ্যা* esz1, MatStep* S, MatShape& D, পূর্ণসংখ্যা& m)
{
    যদি (!expandChannels) ফেরত মিথ্যা;
    পূর্ণসংখ্যা ref = -1;
    জন্য (পূর্ণসংখ্যা k = 0; k < K; k++)
        যদি (!isSingleChannelScalar(*arrays[k])) { ref = k; বিরতি; }
    যদি (ref < 0) ফেরত মিথ্যা;                       // all single-channel scalars: let general handle
    ধ্রুবক Mat& R = *arrays[ref];
    ধ্রুবক পূর্ণসংখ্যা rdims = R.dims, rcn = R.channels();
    জন্য (পূর্ণসংখ্যা k = 0; k < K; k++)
    {
        ধ্রুবক Mat& a = *arrays[k];
        যদি (isSingleChannelScalar(a)) চালু;
        যদি (a.channels() != rcn || a.dims != rdims || !a.isContinuous()) ফেরত মিথ্যা;
        জন্য (পূর্ণসংখ্যা i = 0; i < rdims; i++) যদি (a.size[i] != R.size[i]) ফেরত মিথ্যা;
    }
    ধ্রুবক long long যোগ = (long long)R.যোগ() * rcn;   // channels folded into the 1D run
    CV_Assert(যোগ <= (long long)INT_MAX);
    জন্য (পূর্ণসংখ্যা k = 0; k < K; k++)
    {
        ধ্রুবক Mat& a = *arrays[k];
        base[k] = (uchar*)a.তথ্য;
        esz1[k] = (পূর্ণসংখ্যা)a.elemSize1();
        S[k][0] = isSingleChannelScalar(a) ? 0 : 1;
    }
    D.assign(1, (পূর্ণসংখ্যা)যোগ);
    m = 1;
    ফেরত সত্য;
}

// ---------------------------------------------------------------------------
// BroadcastOp::run
// ---------------------------------------------------------------------------
// At namespace scope, NOT inside run(): MSVC 2019 loses the constexpr-ness of function-local
// constants used as template arguments inside a lambda (C2975).
স্থির constexpr পূর্ণসংখ্যা MAX_DIMS = MatShape::MAX_DIMS;
স্থির constexpr পূর্ণসংখ্যা LOCAL_OPS = 8;

শূন্য BroadcastOp::run(ধ্রুবক Mat* ধ্রুবক* arrays, পূর্ণসংখ্যা narrays,
                      ধ্রুবক std::function<শূন্য(ধ্রুবক Tile&)>& body,
                      যৌক্তিক expandChannels,
                      দ্বিগুণ nstripes)
{
    ধ্রুবক পূর্ণসংখ্যা K = narrays;
    CV_Assert(K >= 1 && arrays != nullptr);

    // ---- 1-3. geometry: per-operand collapsed steps S[k], element sizes esz1[k], base
    //           pointers, and the collapsed iteration shape D (m dims). The fast path handles the
    //           dominant "all same-shape arrays (+ single-channel scalars)" case in one shot; the
    //           general path does decideChannelMode + broadcastShape + align + collapse. ----
    Auto_বাফার<MatStep, LOCAL_OPS>  S(K);
    Auto_বাফার<পূর্ণসংখ্যা, LOCAL_OPS>      esz1(K);
    Auto_বাফার<uchar*, LOCAL_OPS>   base(K);
    MatShape D;
    পূর্ণসংখ্যা m;

    যদি (!fastSameShape(arrays, K, expandChannels, base.তথ্য(), esz1.তথ্য(), S.তথ্য(), D, m))
    {
        ধ্রুবক ChMode mode = expandChannels ? decideChannelMode(arrays, K) : CH_ELEM;
        Auto_বাফার<MatShape, LOCAL_OPS> shp(K);
        Auto_বাফার<MatStep, LOCAL_OPS>  stp(K);
        জন্য (পূর্ণসংখ্যা k = 0; k < K; k++)
        {
            matLayout(*arrays[k], mode, shp[k], stp[k], esz1[k]);
            base[k] = (uchar*)arrays[k]->তথ্য;
        }
        MatShape full;
        CV_Assert(broadcastShape(shp.তথ্য(), K, full) && "ew: operands are not broadcast-compatible");
        ধ্রুবক পূর্ণসংখ্যা nd = (পূর্ণসংখ্যা)full.size();
        Auto_বাফার<MatShape, LOCAL_OPS> H(K);
        জন্য (পূর্ণসংখ্যা k = 0; k < K; k++) alignArg(shp[k], stp[k], nd, S[k], H[k]);
        D = full;
        m = collapseDims(S.তথ্য(), H.তথ্য(), K, D);

        // For a cv::Mat the innermost (channel/last) axis is contiguous, so after collapse the
        // innermost stride is always in {0,1}. No gather, no materialization.
        জন্য (পূর্ণসংখ্যা k = 0; k < K; k++)
            CV_Assert(S[k][m - 1] <= 1 && "ew: unexpected innermost stride > 1");
    }

    // ---- 4. inner 2D tile axes: width = D[m-1], height = D[m-2] (if any) ----
    ধ্রুবক পূর্ণসংখ্যা wAxis = m - 1;
    ধ্রুবক পূর্ণসংখ্যা hAxis = (m >= 2) ? m - 2 : -1;
    ধ্রুবক পূর্ণসংখ্যা W   = D[wAxis];
    ধ্রুবক পূর্ণসংখ্যা Hgt = (hAxis >= 0) ? D[hAxis] : 1;
    ধ্রুবক পূর্ণসংখ্যা nOuter = (hAxis >= 0) ? m - 2 : m - 1;   // outer ("plane") axes = [0 .. nOuter)
    long long nplanes = 1;
    জন্য (পূর্ণসংখ্যা d = 0; d < nOuter; d++) nplanes *= D[d];

    // ---- 5. desired parallel stripe count (work hint) ----
    ধ্রুবক long long যোগ = nplanes * (long long)Hgt * (long long)W;
    দ্বিগুণ stripes = nstripes;
    যদি (stripes <= 0)                                  // broadcastOp can't see the body's cost;
        stripes = (দ্বিগুণ)যোগ * 100.0 / (দ্বিগুণ)(1 << 18);  // assume ~100 cycles/element
    ধ্রুবক পূর্ণসংখ্যা wantTiles = std::max(1, (পূর্ণসংখ্যা)std::lround(stripes));

    // ---- 6. tile only for PARALLELISM. broadcastOp is op-agnostic: it does not know the body's
    //         temp-buffer footprint, so it does NOT tile for L1 - that is the body's job (it
    //         re-fragments a tile's width into L1-sized chunks for the fused intermediates).
    //         Start with the largest tile (one 2D block per plane) and split (height first, then
    //         width) only until there are at least `wantTiles` tiles. Bigger tiles => fewer
    //         body/decode calls. Width is G-aligned only in the fully-contiguous (1D) case. ----
    ধ্রুবক পূর্ণসংখ্যা G = 16;                  // SIMD/cacheline granule
    পূর্ণসংখ্যা tw = W, th = Hgt;

    auto ntilesOf = [&](পূর্ণসংখ্যা tw_, পূর্ণসংখ্যা th_) {
        long long nw = (W + tw_ - 1) / tw_, nh = (Hgt + th_ - 1) / th_;
        ফেরত nplanes * nh * nw;
    };
    long long ntiles = ntilesOf(tw, th);
    যখন (ntiles < wantTiles && th > 1)               // split height for parallelism
    {
        th = (th + 1) / 2;
        ntiles = ntilesOf(tw, th);
    }
    যখন (ntiles < wantTiles && tw > G)               // then split width
    {
        tw = std::max(G, tw / 2);
        যদি (hAxis < 0 && tw > G) tw -= tw % G;          // keep width aligned in the 1D case
        ntiles = ntilesOf(tw, th);
    }
    CV_Assert(ntiles <= (long long)INT_MAX);

    ধ্রুবক পূর্ণসংখ্যা ntilesW = (W + tw - 1) / tw;
    ধ্রুবক পূর্ণসংখ্যা ntilesH = (Hgt + th - 1) / th;

    // ---- 7. execution; decode tile index -> per-operand slices. stepx/stepy are the same for
    //         every tile, so they are set ONCE; only the per-tile base pointer is recomputed. ----
    auto runRange = [&](ধ্রুবক Range& r)
    {
        Auto_বাফার<Slice, LOCAL_OPS> slices(K);

        // Fast 1D path (m==1: one contiguous axis after collapse, no outer planes, height 1).
        // ntilesH==1 and nplanes==1, so the tile index IS the width-tile index - no div/mod, no
        // plane multi-index decode, no inner step loop. This is the same-shape / fully-contiguous
        // common case.
        যদি (m == 1)
        {
            জন্য (পূর্ণসংখ্যা k = 0; k < K; k++) { slices[k].stepy = 0; slices[k].stepx = S[k][0]; }
            Tile tile;
            tile.height = 1; tile.narrays = K; tile.slices = slices.তথ্য();
            জন্য (পূর্ণসংখ্যা t = r.start; t < r.end; t++)
            {
                ধ্রুবক পূর্ণসংখ্যা wofs = t * tw, ww = std::min(tw, W - wofs);
                জন্য (পূর্ণসংখ্যা k = 0; k < K; k++)
                    slices[k].ptr = base[k] + (size_t)wofs * S[k][0] * (size_t)esz1[k];
                tile.width = ww;
                body(tile);
            }
            ফেরত;
        }

        std::array<পূর্ণসংখ্যা, MAX_DIMS> idx;
        জন্য (পূর্ণসংখ্যা k = 0; k < K; k++)                     // steps are tile-independent: set once
        {
            slices[k].stepy = (hAxis >= 0) ? S[k][hAxis] : 0;
            slices[k].stepx = S[k][wAxis];
        }
        জন্য (পূর্ণসংখ্যা t = r.start; t < r.end; t++)
        {
            পূর্ণসংখ্যা wt = t % ntilesW;
            পূর্ণসংখ্যা rest = t / ntilesW;
            পূর্ণসংখ্যা ht = rest % ntilesH;
            পূর্ণসংখ্যা plane = rest / ntilesH;

            ধ্রুবক পূর্ণসংখ্যা wofs = wt * tw, ww = std::min(tw, W - wofs);
            ধ্রুবক পূর্ণসংখ্যা hofs = ht * th, hh = std::min(th, Hgt - hofs);

            পূর্ণসংখ্যা p = plane;                              // decode plane -> outer multi-index
            জন্য (পূর্ণসংখ্যা d = nOuter - 1; d >= 0; d--) {
                পূর্ণসংখ্যা dd = D[d];
                পূর্ণসংখ্যা np = p / dd;
                idx[d] = p - np * dd;
                p = np;
            }

            জন্য (পূর্ণসংখ্যা k = 0; k < K; k++)
            {
                size_t off = (size_t)wofs * S[k][wAxis];
                জন্য (পূর্ণসংখ্যা d = 0; d < nOuter; d++) off += (size_t)idx[d] * S[k][d];
                যদি (hAxis >= 0) off += (size_t)hofs * S[k][hAxis];
                slices[k].ptr = base[k] + off * (size_t)esz1[k];
            }

            Tile tile;
            tile.width = ww; tile.height = hh; tile.narrays = K; tile.slices = slices.তথ্য();
            body(tile);
        }
    };

    // Single tile (small work, wantTiles==1) => run inline and skip the parallel framework
    // entirely: its dispatch (std::function wrap + Range machinery + backend hop) is pure
    // overhead when there is nothing to parallelize, and dominates small-array latency.
    যদি (ntiles == 1)
        runRange(Range(0, 1));
    অন্যথা
        parallel_for_(Range(0, (পূর্ণসংখ্যা)ntiles), runRange, stripes);
}

} // namespace cv
