// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

// Implementation of cv::BroadcastOp (declared in opencv2/core/mat.hpp): an op-agnostic driver for
// broadcasting element-wise traversal. It computes the numpy-broadcast iteration space over a flat
// list of operand Mats, collapses contiguous dims, partitions into tiles for parallel_for_, and hands
// each 2D tile's per-operand slices to a `body` callback (which owns all semantics).

#অন্তর্ভাৱ "precomp.hpp"
#অন্তর্ভাৱ <algorithm>
#অন্তর্ভাৱ <array>
#অন্তর্ভাৱ <climits>
#অন্তর্ভাৱ <cmath>

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

স্থির শূন্য matLayout(ধ্রুৱ Mat& m, ChMode mode, MatShape& shp, MatStep& step, পূর্ণাঙ্ক& esz1)
{
    ধ্রুৱ পূর্ণাঙ্ক nd = m.dims, cn = m.channels();
    যদি (mode == CH_ELEM)
    {
        esz1 = (পূর্ণাঙ্ক)m.elemSize();                       // one full (cn-channel) pixel
        shp.resize(nd);
        বাবে (পূর্ণাঙ্ক i = 0; i < nd; i++) { shp[i] = m.size[i]; step[i] = m.step[i] / esz1; }
        ঘূৰাই;
    }
    esz1 = (পূর্ণাঙ্ক)m.elemSize1();                           // one scalar (channel value)
    যদি (mode == CH_DIM)
    {
        shp.resize(nd + 1);
        বাবে (পূর্ণাঙ্ক i = 0; i < nd; i++) { shp[i] = m.size[i]; step[i] = m.step[i] / esz1; }
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
            বাবে (পূর্ণাঙ্ক i = 0; i < nd; i++) { shp[i] = m.size[i]; step[i] = m.step[i] / esz1; }
            shp[nd - 1] *= cn;                           // fold channels into the innermost dim
            step[nd - 1] = 1;                            // scalars are contiguous there
        }
    }
}

// A single-channel scalar (one value, cn==1, total()==1) broadcasts into everything trivially
// (step 0 on every axis incl. channels), so it must NOT force CH_DIM - it is excluded from the
// channel-mode decision entirely.
স্থির তার্কিক isSingleChannelScalar(ধ্রুৱ Mat& m)
{
    ঘূৰাই m.channels() == 1 && m.total() == 1;
}

// Decide (globally, across all operands) how channels are presented for expandChannels=true:
// CH_FOLD when no channel broadcast is needed, CH_DIM when it is. Single-channel scalars are
// excluded first; among the rest, multichannel operands must all share the same cn (an (n,m) mix
// with both > 1 is an error).
স্থির ChMode decideChannelMode(ধ্রুৱ Mat* ধ্রুৱ* arrays, পূর্ণাঙ্ক K)
{
    পূর্ণাঙ্ক N = 1;                                           // the single multichannel count, if any
    বাবে (পূর্ণাঙ্ক k = 0; k < K; k++)
    {
        যদি (isSingleChannelScalar(*arrays[k])) অনুৱর্ত;
        পূর্ণাঙ্ক c = arrays[k]->channels();
        যদি (c > 1) { যদি (N == 1) N = c; অন্যথা CV_Assert(N == c && "ew: (n,m) channel mix unsupported"); }
    }
    যদি (N == 1) ঘূৰাই CH_FOLD;                          // all single-channel -> fold (a no-op)

    তার্কিক allMulti = সঁচা, sameBack = সঁচা;
    পূর্ণাঙ্ক back = -1;
    বাবে (পূর্ণাঙ্ক k = 0; k < K; k++)
    {
        ধ্রুৱ Mat& a = *arrays[k];
        যদি (isSingleChannelScalar(a)) অনুৱর্ত;
        যদি (a.channels() != N) allMulti = মিছা;
        পূর্ণাঙ্ক b = a.dims > 0 ? a.size[a.dims - 1] : 1;   // 0-dim scalar has no spatial back (=1)
        যদি (back < 0) back = b; অন্যথা যদি (b != back) sameBack = মিছা;
    }
    ঘূৰাই (allMulti && sameBack) ? CH_FOLD : CH_DIM;    // fold only if no channel broadcast
}

// numpy-style broadcast of several right-aligned shapes.
স্থির তার্কিক broadcastShape(ধ্রুৱ MatShape* shps, পূর্ণাঙ্ক K, MatShape& out)
{
    size_t nd = 0;
    বাবে (পূর্ণাঙ্ক k = 0; k < K; k++) nd = std::max(nd, shps[k].size());
    out.assign(nd, 1);
    বাবে (পূর্ণাঙ্ক k = 0; k < K; k++)
    {
        ধ্রুৱ MatShape& s = shps[k];
        size_t off = nd - s.size();
        বাবে (size_t i = 0; i < s.size(); i++)
        {
            পূর্ণাঙ্ক d = s[i], &o = out[off + i];
            যদি (o == 1) o = d;
            অন্যথা যদি (d != 1 && d != o) ঘূৰাই মিছা;
        }
    }
    ঘূৰাই সঁচা;
}

// Right-align an arg's own (shp,step) to nd dims; broadcast dims get step 0.
স্থির শূন্য alignArg(ধ্রুৱ MatShape& shp, ধ্রুৱ MatStep& step, পূর্ণাঙ্ক nd,
                     MatStep& as, MatShape& ash)
{
    as.clear();
    ash.assign(nd, 1);
    পূর্ণাঙ্ক off = nd - (পূর্ণাঙ্ক)shp.size();
    বাবে (পূর্ণাঙ্ক i = 0; i < (পূর্ণাঙ্ক)shp.size(); i++)
    {
        পূর্ণাঙ্ক d = shp[i];
        ash[off + i] = d;
        as[off + i] = (d == 1) ? 0 : step[i];
    }
}

// Collapse adjacent dims that are contiguous (and broadcast-consistent) across all args.
স্থির পূর্ণাঙ্ক collapseDims(MatStep* S, MatShape* H, পূর্ণাঙ্ক K, MatShape& D)
{
    পূর্ণাঙ্ক nd = (পূর্ণাঙ্ক)D.size();
    যদি (nd <= 1) ঘূৰাই nd;

    পূর্ণাঙ্ক j = nd - 1;
    বাবে (পূর্ণাঙ্ক i = j - 1; i >= 0; i--)
    {
        তার্কিক contig = সঁচা, scalar = সঁচা, consist = সঁচা;
        বাবে (পূর্ণাঙ্ক k = 0; k < K; k++)
        {
            size_t st = S[k][j] * (size_t)H[k][j];
            তার্কিক prevScalar = H[k][j] == 1;
            তার্কিক curScalar = H[k][i] == 1;
            contig  = contig  && (st == S[k][i]);
            scalar  = scalar  && curScalar;
            consist = consist && (curScalar == prevScalar);
        }
        যদি (contig && (consist || scalar))
        {
            বাবে (পূর্ণাঙ্ক k = 0; k < K; k++) H[k][j] *= H[k][i];
            D[j] *= D[i];
        }
        অন্যথা
        {
            j--;
            যদি (i < j)
            {
                বাবে (পূর্ণাঙ্ক k = 0; k < K; k++) { H[k][j] = H[k][i]; S[k][j] = S[k][i]; }
                D[j] = D[i];
            }
        }
    }

    পূর্ণাঙ্ক m = nd - j;
    বাবে (পূর্ণাঙ্ক d = 0; d < m; d++)
    {
        D[d] = D[j + d];
        বাবে (পূর্ণাঙ্ক k = 0; k < K; k++) { S[k][d] = S[k][j + d]; H[k][d] = H[k][j + d]; }
    }
    D.resize(m);
    বাবে (পূর্ণাঙ্ক k = 0; k < K; k++) H[k].resize(m);

    // Zero out steps of broadcast (size-1) dims (numpy step==0 trick).
    বাবে (পূর্ণাঙ্ক d = 0; d < m; d++)
        বাবে (পূর্ণাঙ্ক k = 0; k < K; k++)
            যদি (H[k][d] == 1) S[k][d] = 0;

    ঘূৰাই m;
}

// Fast geometry for the dominant case: every operand is either (a) an array sharing ONE common
// shape - same dims, sizes and channel count - and contiguous, or (b) a single-channel scalar
// (cn==1, total()==1). Then the whole traversal is a single contiguous 1D run of `total` scalars
// (channels folded in): arrays get stepx 1, scalars stepx 0. This skips decideChannelMode /
// broadcastShape / alignArg / collapseDims and all their per-operand buffers entirely. Returns
// false (leaving outputs untouched) when the operands don't fit, so the caller runs general
// geometry. expandChannels=false (CH_ELEM) keeps channels in the element and is left to general.
স্থির তার্কিক fastSameShape(ধ্রুৱ Mat* ধ্রুৱ* arrays, পূর্ণাঙ্ক K, তার্কিক expandChannels,
                          uchar** base, পূর্ণাঙ্ক* esz1, MatStep* S, MatShape& D, পূর্ণাঙ্ক& m)
{
    যদি (!expandChannels) ঘূৰাই মিছা;
    পূর্ণাঙ্ক ref = -1;
    বাবে (পূর্ণাঙ্ক k = 0; k < K; k++)
        যদি (!isSingleChannelScalar(*arrays[k])) { ref = k; ভঙ্গ; }
    যদি (ref < 0) ঘূৰাই মিছা;                       // all single-channel scalars: let general handle
    ধ্রুৱ Mat& R = *arrays[ref];
    ধ্রুৱ পূর্ণাঙ্ক rdims = R.dims, rcn = R.channels();
    বাবে (পূর্ণাঙ্ক k = 0; k < K; k++)
    {
        ধ্রুৱ Mat& a = *arrays[k];
        যদি (isSingleChannelScalar(a)) অনুৱর্ত;
        যদি (a.channels() != rcn || a.dims != rdims || !a.isContinuous()) ঘূৰাই মিছা;
        বাবে (পূর্ণাঙ্ক i = 0; i < rdims; i++) যদি (a.size[i] != R.size[i]) ঘূৰাই মিছা;
    }
    ধ্রুৱ long long total = (long long)R.total() * rcn;   // channels folded into the 1D run
    CV_Assert(total <= (long long)INT_MAX);
    বাবে (পূর্ণাঙ্ক k = 0; k < K; k++)
    {
        ধ্রুৱ Mat& a = *arrays[k];
        base[k] = (uchar*)a.data;
        esz1[k] = (পূর্ণাঙ্ক)a.elemSize1();
        S[k][0] = isSingleChannelScalar(a) ? 0 : 1;
    }
    D.assign(1, (পূর্ণাঙ্ক)total);
    m = 1;
    ঘূৰাই সঁচা;
}

// ---------------------------------------------------------------------------
// BroadcastOp::run
// ---------------------------------------------------------------------------
// At namespace scope, NOT inside run(): MSVC 2019 loses the constexpr-ness of function-local
// constants used as template arguments inside a lambda (C2975).
স্থির constexpr পূর্ণাঙ্ক MAX_DIMS = MatShape::MAX_DIMS;
স্থির constexpr পূর্ণাঙ্ক LOCAL_OPS = 8;

শূন্য BroadcastOp::run(ধ্রুৱ Mat* ধ্রুৱ* arrays, পূর্ণাঙ্ক narrays,
                      ধ্রুৱ std::function<শূন্য(ধ্রুৱ Tile&)>& body,
                      তার্কিক expandChannels,
                      দ্ৱিগুণ nstripes)
{
    ধ্রুৱ পূর্ণাঙ্ক K = narrays;
    CV_Assert(K >= 1 && arrays != nullptr);

    // ---- 1-3. geometry: per-operand collapsed steps S[k], element sizes esz1[k], base
    //           pointers, and the collapsed iteration shape D (m dims). The fast path handles the
    //           dominant "all same-shape arrays (+ single-channel scalars)" case in one shot; the
    //           general path does decideChannelMode + broadcastShape + align + collapse. ----
    AutoBuffer<MatStep, LOCAL_OPS>  S(K);
    AutoBuffer<পূর্ণাঙ্ক, LOCAL_OPS>      esz1(K);
    AutoBuffer<uchar*, LOCAL_OPS>   base(K);
    MatShape D;
    পূর্ণাঙ্ক m;

    যদি (!fastSameShape(arrays, K, expandChannels, base.data(), esz1.data(), S.data(), D, m))
    {
        ধ্রুৱ ChMode mode = expandChannels ? decideChannelMode(arrays, K) : CH_ELEM;
        AutoBuffer<MatShape, LOCAL_OPS> shp(K);
        AutoBuffer<MatStep, LOCAL_OPS>  stp(K);
        বাবে (পূর্ণাঙ্ক k = 0; k < K; k++)
        {
            matLayout(*arrays[k], mode, shp[k], stp[k], esz1[k]);
            base[k] = (uchar*)arrays[k]->data;
        }
        MatShape full;
        CV_Assert(broadcastShape(shp.data(), K, full) && "ew: operands are not broadcast-compatible");
        ধ্রুৱ পূর্ণাঙ্ক nd = (পূর্ণাঙ্ক)full.size();
        AutoBuffer<MatShape, LOCAL_OPS> H(K);
        বাবে (পূর্ণাঙ্ক k = 0; k < K; k++) alignArg(shp[k], stp[k], nd, S[k], H[k]);
        D = full;
        m = collapseDims(S.data(), H.data(), K, D);

        // For a cv::Mat the innermost (channel/last) axis is contiguous, so after collapse the
        // innermost stride is always in {0,1}. No gather, no materialization.
        বাবে (পূর্ণাঙ্ক k = 0; k < K; k++)
            CV_Assert(S[k][m - 1] <= 1 && "ew: unexpected innermost stride > 1");
    }

    // ---- 4. inner 2D tile axes: width = D[m-1], height = D[m-2] (if any) ----
    ধ্রুৱ পূর্ণাঙ্ক wAxis = m - 1;
    ধ্রুৱ পূর্ণাঙ্ক hAxis = (m >= 2) ? m - 2 : -1;
    ধ্রুৱ পূর্ণাঙ্ক W   = D[wAxis];
    ধ্রুৱ পূর্ণাঙ্ক Hgt = (hAxis >= 0) ? D[hAxis] : 1;
    ধ্রুৱ পূর্ণাঙ্ক nOuter = (hAxis >= 0) ? m - 2 : m - 1;   // outer ("plane") axes = [0 .. nOuter)
    long long nplanes = 1;
    বাবে (পূর্ণাঙ্ক d = 0; d < nOuter; d++) nplanes *= D[d];

    // ---- 5. desired parallel stripe count (work hint) ----
    ধ্রুৱ long long total = nplanes * (long long)Hgt * (long long)W;
    দ্ৱিগুণ stripes = nstripes;
    যদি (stripes <= 0)                                  // broadcastOp can't see the body's cost;
        stripes = (দ্ৱিগুণ)total * 100.0 / (দ্ৱিগুণ)(1 << 18);  // assume ~100 cycles/element
    ধ্রুৱ পূর্ণাঙ্ক wantTiles = std::max(1, (পূর্ণাঙ্ক)std::lround(stripes));

    // ---- 6. tile only for PARALLELISM. broadcastOp is op-agnostic: it does not know the body's
    //         temp-buffer footprint, so it does NOT tile for L1 - that is the body's job (it
    //         re-fragments a tile's width into L1-sized chunks for the fused intermediates).
    //         Start with the largest tile (one 2D block per plane) and split (height first, then
    //         width) only until there are at least `wantTiles` tiles. Bigger tiles => fewer
    //         body/decode calls. Width is G-aligned only in the fully-contiguous (1D) case. ----
    ধ্রুৱ পূর্ণাঙ্ক G = 16;                  // SIMD/cacheline granule
    পূর্ণাঙ্ক tw = W, th = Hgt;

    auto ntilesOf = [&](পূর্ণাঙ্ক tw_, পূর্ণাঙ্ক th_) {
        long long nw = (W + tw_ - 1) / tw_, nh = (Hgt + th_ - 1) / th_;
        ঘূৰাই nplanes * nh * nw;
    };
    long long ntiles = ntilesOf(tw, th);
    যেতিয়ালৈ (ntiles < wantTiles && th > 1)               // split height for parallelism
    {
        th = (th + 1) / 2;
        ntiles = ntilesOf(tw, th);
    }
    যেতিয়ালৈ (ntiles < wantTiles && tw > G)               // then split width
    {
        tw = std::max(G, tw / 2);
        যদি (hAxis < 0 && tw > G) tw -= tw % G;          // keep width aligned in the 1D case
        ntiles = ntilesOf(tw, th);
    }
    CV_Assert(ntiles <= (long long)INT_MAX);

    ধ্রুৱ পূর্ণাঙ্ক ntilesW = (W + tw - 1) / tw;
    ধ্রুৱ পূর্ণাঙ্ক ntilesH = (Hgt + th - 1) / th;

    // ---- 7. execution; decode tile index -> per-operand slices. stepx/stepy are the same for
    //         every tile, so they are set ONCE; only the per-tile base pointer is recomputed. ----
    auto runRange = [&](ধ্রুৱ Range& r)
    {
        AutoBuffer<Slice, LOCAL_OPS> slices(K);

        // Fast 1D path (m==1: one contiguous axis after collapse, no outer planes, height 1).
        // ntilesH==1 and nplanes==1, so the tile index IS the width-tile index - no div/mod, no
        // plane multi-index decode, no inner step loop. This is the same-shape / fully-contiguous
        // common case.
        যদি (m == 1)
        {
            বাবে (পূর্ণাঙ্ক k = 0; k < K; k++) { slices[k].stepy = 0; slices[k].stepx = S[k][0]; }
            Tile tile;
            tile.height = 1; tile.narrays = K; tile.slices = slices.data();
            বাবে (পূর্ণাঙ্ক t = r.start; t < r.end; t++)
            {
                ধ্রুৱ পূর্ণাঙ্ক wofs = t * tw, ww = std::min(tw, W - wofs);
                বাবে (পূর্ণাঙ্ক k = 0; k < K; k++)
                    slices[k].ptr = base[k] + (size_t)wofs * S[k][0] * (size_t)esz1[k];
                tile.width = ww;
                body(tile);
            }
            ঘূৰাই;
        }

        std::array<পূর্ণাঙ্ক, MAX_DIMS> idx;
        বাবে (পূর্ণাঙ্ক k = 0; k < K; k++)                     // steps are tile-independent: set once
        {
            slices[k].stepy = (hAxis >= 0) ? S[k][hAxis] : 0;
            slices[k].stepx = S[k][wAxis];
        }
        বাবে (পূর্ণাঙ্ক t = r.start; t < r.end; t++)
        {
            পূর্ণাঙ্ক wt = t % ntilesW;
            পূর্ণাঙ্ক rest = t / ntilesW;
            পূর্ণাঙ্ক ht = rest % ntilesH;
            পূর্ণাঙ্ক plane = rest / ntilesH;

            ধ্রুৱ পূর্ণাঙ্ক wofs = wt * tw, ww = std::min(tw, W - wofs);
            ধ্রুৱ পূর্ণাঙ্ক hofs = ht * th, hh = std::min(th, Hgt - hofs);

            পূর্ণাঙ্ক p = plane;                              // decode plane -> outer multi-index
            বাবে (পূর্ণাঙ্ক d = nOuter - 1; d >= 0; d--) {
                পূর্ণাঙ্ক dd = D[d];
                পূর্ণাঙ্ক np = p / dd;
                idx[d] = p - np * dd;
                p = np;
            }

            বাবে (পূর্ণাঙ্ক k = 0; k < K; k++)
            {
                size_t off = (size_t)wofs * S[k][wAxis];
                বাবে (পূর্ণাঙ্ক d = 0; d < nOuter; d++) off += (size_t)idx[d] * S[k][d];
                যদি (hAxis >= 0) off += (size_t)hofs * S[k][hAxis];
                slices[k].ptr = base[k] + off * (size_t)esz1[k];
            }

            Tile tile;
            tile.width = ww; tile.height = hh; tile.narrays = K; tile.slices = slices.data();
            body(tile);
        }
    };

    // Single tile (small work, wantTiles==1) => run inline and skip the parallel framework
    // entirely: its dispatch (std::function wrap + Range machinery + backend hop) is pure
    // overhead when there is nothing to parallelize, and dominates small-array latency.
    যদি (ntiles == 1)
        runRange(Range(0, 1));
    অন্যথা
        parallel_for_(Range(0, (পূর্ণাঙ্ক)ntiles), runRange, stripes);
}

} // namespace cv
