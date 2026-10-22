// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

// Implementation of cv::BroadcastOp (declared in opencv2/core/mat.hpp): an op-agnostic driver for
// broadcasting element-wise traversal. It computes the numpy-broadcast iteration space over a flat
// list of operand Mats, collapses contiguous dims, partitions into tiles for parallel_for_, and hands
// each 2D tile's per-operand slices to a `body` callback (which owns all semantics).

#شامل "precomp.hpp"
#شامل <algorithm>
#شامل <array>
#شامل <climits>
#شامل <cmath>

فضاینام cv {

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
شمارش ChMode { CH_FOLD, CH_DIM, CH_ELEM };

ایستا تهی matLayout(ثابت Mat& m, ChMode mode, MatShape& shp, MatStep& step, صحیح& esz1)
{
    ثابت صحیح nd = m.dims, cn = m.channels();
    اگر (mode == CH_ELEM)
    {
        esz1 = (صحیح)m.elemSize();                       // one full (cn-channel) pixel
        shp.resize(nd);
        برای (صحیح i = 0; i < nd; i++) { shp[i] = m.size[i]; step[i] = m.step[i] / esz1; }
        بازگشت;
    }
    esz1 = (صحیح)m.elemSize1();                           // one scalar (channel value)
    اگر (mode == CH_DIM)
    {
        shp.resize(nd + 1);
        برای (صحیح i = 0; i < nd; i++) { shp[i] = m.size[i]; step[i] = m.step[i] / esz1; }
        shp[nd] = cn; step[nd] = 1;                      // channels = explicit innermost dim
    }
    وگرنه // CH_FOLD
    {
        اگر (nd == 0)                                     // 0-dim scalar: channels are the only dim
        {
            shp.assign(1, cn);
            step[0] = 1;
        }
        وگرنه
        {
            shp.resize(nd);
            برای (صحیح i = 0; i < nd; i++) { shp[i] = m.size[i]; step[i] = m.step[i] / esz1; }
            shp[nd - 1] *= cn;                           // fold channels into the innermost dim
            step[nd - 1] = 1;                            // scalars are contiguous there
        }
    }
}

// A single-channel scalar (one value, cn==1, total()==1) broadcasts into everything trivially
// (step 0 on every axis incl. channels), so it must NOT force CH_DIM - it is excluded from the
// channel-mode decision entirely.
ایستا منطقی isSingleChannelScalar(ثابت Mat& m)
{
    بازگشت m.channels() == 1 && m.total() == 1;
}

// Decide (globally, across all operands) how channels are presented for expandChannels=true:
// CH_FOLD when no channel broadcast is needed, CH_DIM when it is. Single-channel scalars are
// excluded first; among the rest, multichannel operands must all share the same cn (an (n,m) mix
// with both > 1 is an error).
ایستا ChMode decideChannelMode(ثابت Mat* ثابت* arrays, صحیح K)
{
    صحیح N = 1;                                           // the single multichannel count, if any
    برای (صحیح k = 0; k < K; k++)
    {
        اگر (isSingleChannelScalar(*arrays[k])) ادامه;
        صحیح c = arrays[k]->channels();
        اگر (c > 1) { اگر (N == 1) N = c; وگرنه CV_Assert(N == c && "ew: (n,m) channel mix unsupported"); }
    }
    اگر (N == 1) بازگشت CH_FOLD;                          // all single-channel -> fold (a no-op)

    منطقی allMulti = درست, sameBack = درست;
    صحیح back = -1;
    برای (صحیح k = 0; k < K; k++)
    {
        ثابت Mat& a = *arrays[k];
        اگر (isSingleChannelScalar(a)) ادامه;
        اگر (a.channels() != N) allMulti = نادرست;
        صحیح b = a.dims > 0 ? a.size[a.dims - 1] : 1;   // 0-dim scalar has no spatial back (=1)
        اگر (back < 0) back = b; وگرنه اگر (b != back) sameBack = نادرست;
    }
    بازگشت (allMulti && sameBack) ? CH_FOLD : CH_DIM;    // fold only if no channel broadcast
}

// numpy-style broadcast of several right-aligned shapes.
ایستا منطقی broadcastShape(ثابت MatShape* shps, صحیح K, MatShape& out)
{
    size_t nd = 0;
    برای (صحیح k = 0; k < K; k++) nd = std::max(nd, shps[k].size());
    out.assign(nd, 1);
    برای (صحیح k = 0; k < K; k++)
    {
        ثابت MatShape& s = shps[k];
        size_t off = nd - s.size();
        برای (size_t i = 0; i < s.size(); i++)
        {
            صحیح d = s[i], &o = out[off + i];
            اگر (o == 1) o = d;
            وگرنه اگر (d != 1 && d != o) بازگشت نادرست;
        }
    }
    بازگشت درست;
}

// Right-align an arg's own (shp,step) to nd dims; broadcast dims get step 0.
ایستا تهی alignArg(ثابت MatShape& shp, ثابت MatStep& step, صحیح nd,
                     MatStep& as, MatShape& ash)
{
    as.clear();
    ash.assign(nd, 1);
    صحیح off = nd - (صحیح)shp.size();
    برای (صحیح i = 0; i < (صحیح)shp.size(); i++)
    {
        صحیح d = shp[i];
        ash[off + i] = d;
        as[off + i] = (d == 1) ? 0 : step[i];
    }
}

// Collapse adjacent dims that are contiguous (and broadcast-consistent) across all args.
ایستا صحیح collapseDims(MatStep* S, MatShape* H, صحیح K, MatShape& D)
{
    صحیح nd = (صحیح)D.size();
    اگر (nd <= 1) بازگشت nd;

    صحیح j = nd - 1;
    برای (صحیح i = j - 1; i >= 0; i--)
    {
        منطقی contig = درست, scalar = درست, consist = درست;
        برای (صحیح k = 0; k < K; k++)
        {
            size_t st = S[k][j] * (size_t)H[k][j];
            منطقی prevScalar = H[k][j] == 1;
            منطقی curScalar = H[k][i] == 1;
            contig  = contig  && (st == S[k][i]);
            scalar  = scalar  && curScalar;
            consist = consist && (curScalar == prevScalar);
        }
        اگر (contig && (consist || scalar))
        {
            برای (صحیح k = 0; k < K; k++) H[k][j] *= H[k][i];
            D[j] *= D[i];
        }
        وگرنه
        {
            j--;
            اگر (i < j)
            {
                برای (صحیح k = 0; k < K; k++) { H[k][j] = H[k][i]; S[k][j] = S[k][i]; }
                D[j] = D[i];
            }
        }
    }

    صحیح m = nd - j;
    برای (صحیح d = 0; d < m; d++)
    {
        D[d] = D[j + d];
        برای (صحیح k = 0; k < K; k++) { S[k][d] = S[k][j + d]; H[k][d] = H[k][j + d]; }
    }
    D.resize(m);
    برای (صحیح k = 0; k < K; k++) H[k].resize(m);

    // Zero out steps of broadcast (size-1) dims (numpy step==0 trick).
    برای (صحیح d = 0; d < m; d++)
        برای (صحیح k = 0; k < K; k++)
            اگر (H[k][d] == 1) S[k][d] = 0;

    بازگشت m;
}

// Fast geometry for the dominant case: every operand is either (a) an array sharing ONE common
// shape - same dims, sizes and channel count - and contiguous, or (b) a single-channel scalar
// (cn==1, total()==1). Then the whole traversal is a single contiguous 1D run of `total` scalars
// (channels folded in): arrays get stepx 1, scalars stepx 0. This skips decideChannelMode /
// broadcastShape / alignArg / collapseDims and all their per-operand buffers entirely. Returns
// false (leaving outputs untouched) when the operands don't fit, so the caller runs general
// geometry. expandChannels=false (CH_ELEM) keeps channels in the element and is left to general.
ایستا منطقی fastSameShape(ثابت Mat* ثابت* arrays, صحیح K, منطقی expandChannels,
                          uchar** base, صحیح* esz1, MatStep* S, MatShape& D, صحیح& m)
{
    اگر (!expandChannels) بازگشت نادرست;
    صحیح ref = -1;
    برای (صحیح k = 0; k < K; k++)
        اگر (!isSingleChannelScalar(*arrays[k])) { ref = k; بشکن; }
    اگر (ref < 0) بازگشت نادرست;                       // all single-channel scalars: let general handle
    ثابت Mat& R = *arrays[ref];
    ثابت صحیح rdims = R.dims, rcn = R.channels();
    برای (صحیح k = 0; k < K; k++)
    {
        ثابت Mat& a = *arrays[k];
        اگر (isSingleChannelScalar(a)) ادامه;
        اگر (a.channels() != rcn || a.dims != rdims || !a.isContinuous()) بازگشت نادرست;
        برای (صحیح i = 0; i < rdims; i++) اگر (a.size[i] != R.size[i]) بازگشت نادرست;
    }
    ثابت long long total = (long long)R.total() * rcn;   // channels folded into the 1D run
    CV_Assert(total <= (long long)INT_MAX);
    برای (صحیح k = 0; k < K; k++)
    {
        ثابت Mat& a = *arrays[k];
        base[k] = (uchar*)a.data;
        esz1[k] = (صحیح)a.elemSize1();
        S[k][0] = isSingleChannelScalar(a) ? 0 : 1;
    }
    D.assign(1, (صحیح)total);
    m = 1;
    بازگشت درست;
}

// ---------------------------------------------------------------------------
// BroadcastOp::run
// ---------------------------------------------------------------------------
// At namespace scope, NOT inside run(): MSVC 2019 loses the constexpr-ness of function-local
// constants used as template arguments inside a lambda (C2975).
ایستا constexpr صحیح MAX_DIMS = MatShape::MAX_DIMS;
ایستا constexpr صحیح LOCAL_OPS = 8;

تهی BroadcastOp::run(ثابت Mat* ثابت* arrays, صحیح narrays,
                      ثابت std::function<تهی(ثابت Tile&)>& body,
                      منطقی expandChannels,
                      دوگانه nstripes)
{
    ثابت صحیح K = narrays;
    CV_Assert(K >= 1 && arrays != nullptr);

    // ---- 1-3. geometry: per-operand collapsed steps S[k], element sizes esz1[k], base
    //           pointers, and the collapsed iteration shape D (m dims). The fast path handles the
    //           dominant "all same-shape arrays (+ single-channel scalars)" case in one shot; the
    //           general path does decideChannelMode + broadcastShape + align + collapse. ----
    AutoBuffer<MatStep, LOCAL_OPS>  S(K);
    AutoBuffer<صحیح, LOCAL_OPS>      esz1(K);
    AutoBuffer<uchar*, LOCAL_OPS>   base(K);
    MatShape D;
    صحیح m;

    اگر (!fastSameShape(arrays, K, expandChannels, base.data(), esz1.data(), S.data(), D, m))
    {
        ثابت ChMode mode = expandChannels ? decideChannelMode(arrays, K) : CH_ELEM;
        AutoBuffer<MatShape, LOCAL_OPS> shp(K);
        AutoBuffer<MatStep, LOCAL_OPS>  stp(K);
        برای (صحیح k = 0; k < K; k++)
        {
            matLayout(*arrays[k], mode, shp[k], stp[k], esz1[k]);
            base[k] = (uchar*)arrays[k]->data;
        }
        MatShape full;
        CV_Assert(broadcastShape(shp.data(), K, full) && "ew: operands are not broadcast-compatible");
        ثابت صحیح nd = (صحیح)full.size();
        AutoBuffer<MatShape, LOCAL_OPS> H(K);
        برای (صحیح k = 0; k < K; k++) alignArg(shp[k], stp[k], nd, S[k], H[k]);
        D = full;
        m = collapseDims(S.data(), H.data(), K, D);

        // For a cv::Mat the innermost (channel/last) axis is contiguous, so after collapse the
        // innermost stride is always in {0,1}. No gather, no materialization.
        برای (صحیح k = 0; k < K; k++)
            CV_Assert(S[k][m - 1] <= 1 && "ew: unexpected innermost stride > 1");
    }

    // ---- 4. inner 2D tile axes: width = D[m-1], height = D[m-2] (if any) ----
    ثابت صحیح wAxis = m - 1;
    ثابت صحیح hAxis = (m >= 2) ? m - 2 : -1;
    ثابت صحیح W   = D[wAxis];
    ثابت صحیح Hgt = (hAxis >= 0) ? D[hAxis] : 1;
    ثابت صحیح nOuter = (hAxis >= 0) ? m - 2 : m - 1;   // outer ("plane") axes = [0 .. nOuter)
    long long nplanes = 1;
    برای (صحیح d = 0; d < nOuter; d++) nplanes *= D[d];

    // ---- 5. desired parallel stripe count (work hint) ----
    ثابت long long total = nplanes * (long long)Hgt * (long long)W;
    دوگانه stripes = nstripes;
    اگر (stripes <= 0)                                  // broadcastOp can't see the body's cost;
        stripes = (دوگانه)total * 100.0 / (دوگانه)(1 << 18);  // assume ~100 cycles/element
    ثابت صحیح wantTiles = std::max(1, (صحیح)std::lround(stripes));

    // ---- 6. tile only for PARALLELISM. broadcastOp is op-agnostic: it does not know the body's
    //         temp-buffer footprint, so it does NOT tile for L1 - that is the body's job (it
    //         re-fragments a tile's width into L1-sized chunks for the fused intermediates).
    //         Start with the largest tile (one 2D block per plane) and split (height first, then
    //         width) only until there are at least `wantTiles` tiles. Bigger tiles => fewer
    //         body/decode calls. Width is G-aligned only in the fully-contiguous (1D) case. ----
    ثابت صحیح G = 16;                  // SIMD/cacheline granule
    صحیح tw = W, th = Hgt;

    auto ntilesOf = [&](صحیح tw_, صحیح th_) {
        long long nw = (W + tw_ - 1) / tw_, nh = (Hgt + th_ - 1) / th_;
        بازگشت nplanes * nh * nw;
    };
    long long ntiles = ntilesOf(tw, th);
    وقتی (ntiles < wantTiles && th > 1)               // split height for parallelism
    {
        th = (th + 1) / 2;
        ntiles = ntilesOf(tw, th);
    }
    وقتی (ntiles < wantTiles && tw > G)               // then split width
    {
        tw = std::max(G, tw / 2);
        اگر (hAxis < 0 && tw > G) tw -= tw % G;          // keep width aligned in the 1D case
        ntiles = ntilesOf(tw, th);
    }
    CV_Assert(ntiles <= (long long)INT_MAX);

    ثابت صحیح ntilesW = (W + tw - 1) / tw;
    ثابت صحیح ntilesH = (Hgt + th - 1) / th;

    // ---- 7. execution; decode tile index -> per-operand slices. stepx/stepy are the same for
    //         every tile, so they are set ONCE; only the per-tile base pointer is recomputed. ----
    auto runRange = [&](ثابت Range& r)
    {
        AutoBuffer<Slice, LOCAL_OPS> slices(K);

        // Fast 1D path (m==1: one contiguous axis after collapse, no outer planes, height 1).
        // ntilesH==1 and nplanes==1, so the tile index IS the width-tile index - no div/mod, no
        // plane multi-index decode, no inner step loop. This is the same-shape / fully-contiguous
        // common case.
        اگر (m == 1)
        {
            برای (صحیح k = 0; k < K; k++) { slices[k].stepy = 0; slices[k].stepx = S[k][0]; }
            Tile tile;
            tile.height = 1; tile.narrays = K; tile.slices = slices.data();
            برای (صحیح t = r.start; t < r.end; t++)
            {
                ثابت صحیح wofs = t * tw, ww = std::min(tw, W - wofs);
                برای (صحیح k = 0; k < K; k++)
                    slices[k].ptr = base[k] + (size_t)wofs * S[k][0] * (size_t)esz1[k];
                tile.width = ww;
                body(tile);
            }
            بازگشت;
        }

        std::array<صحیح, MAX_DIMS> idx;
        برای (صحیح k = 0; k < K; k++)                     // steps are tile-independent: set once
        {
            slices[k].stepy = (hAxis >= 0) ? S[k][hAxis] : 0;
            slices[k].stepx = S[k][wAxis];
        }
        برای (صحیح t = r.start; t < r.end; t++)
        {
            صحیح wt = t % ntilesW;
            صحیح rest = t / ntilesW;
            صحیح ht = rest % ntilesH;
            صحیح plane = rest / ntilesH;

            ثابت صحیح wofs = wt * tw, ww = std::min(tw, W - wofs);
            ثابت صحیح hofs = ht * th, hh = std::min(th, Hgt - hofs);

            صحیح p = plane;                              // decode plane -> outer multi-index
            برای (صحیح d = nOuter - 1; d >= 0; d--) {
                صحیح dd = D[d];
                صحیح np = p / dd;
                idx[d] = p - np * dd;
                p = np;
            }

            برای (صحیح k = 0; k < K; k++)
            {
                size_t off = (size_t)wofs * S[k][wAxis];
                برای (صحیح d = 0; d < nOuter; d++) off += (size_t)idx[d] * S[k][d];
                اگر (hAxis >= 0) off += (size_t)hofs * S[k][hAxis];
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
    اگر (ntiles == 1)
        runRange(Range(0, 1));
    وگرنه
        parallel_for_(Range(0, (صحیح)ntiles), runRange, stripes);
}

} // namespace cv
