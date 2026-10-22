// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

// Implementation of cv::BroadcastOp (declared in opencv2/core/mat.hpp): an op-agnostic driver for
// broadcasting element-wise traversal. It computes the numpy-broadcast iteration space over a flat
// list of operand Mats, collapses contiguous dims, partitions into tiles for parallel_for_, and hands
// each 2D tile's per-operand slices to a `body` callback (which owns all semantics).

#include "precomp.hpp"
#include <algorithm>
#include <array>
#include <climits>
#include <cmath>

namespace cv {

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
enum ChMode { CH_FOLD, CH_DIM, CH_ELEM };

static void matLayout(const Mat& m, ChMode mode, MatShape& shp, MatStep& step, 整数& esz1)
{
    const 整数 nd = m.dims, cn = m.channels();
    如果 (mode == CH_ELEM)
    {
        esz1 = (整数)m.elemSize();                       // one full (cn-channel) pixel
        shp.resize(nd);
        为 (整数 i = 0; i < nd; i++) { shp[i] = m.size[i]; step[i] = m.step[i] / esz1; }
        返回;
    }
    esz1 = (整数)m.elemSize1();                           // one scalar (channel value)
    如果 (mode == CH_DIM)
    {
        shp.resize(nd + 1);
        为 (整数 i = 0; i < nd; i++) { shp[i] = m.size[i]; step[i] = m.step[i] / esz1; }
        shp[nd] = cn; step[nd] = 1;                      // channels = explicit innermost dim
    }
    否则 // CH_FOLD
    {
        如果 (nd == 0)                                     // 0-dim scalar: channels are the only dim
        {
            shp.assign(1, cn);
            step[0] = 1;
        }
        否则
        {
            shp.resize(nd);
            为 (整数 i = 0; i < nd; i++) { shp[i] = m.size[i]; step[i] = m.step[i] / esz1; }
            shp[nd - 1] *= cn;                           // fold channels into the innermost dim
            step[nd - 1] = 1;                            // scalars are contiguous there
        }
    }
}

// A single-channel scalar (one value, cn==1, total()==1) broadcasts into everything trivially
// (step 0 on every axis incl. channels), so it must NOT force CH_DIM - it is excluded from the
// channel-mode decision entirely.
static bool isSingleChannelScalar(const Mat& m)
{
    返回 m.channels() == 1 && m.total() == 1;
}

// Decide (globally, across all operands) how channels are presented for expandChannels=true:
// CH_FOLD when no channel broadcast is needed, CH_DIM when it is. Single-channel scalars are
// excluded first; among the rest, multichannel operands must all share the same cn (an (n,m) mix
// with both > 1 is an error).
static ChMode decideChannelMode(const Mat* const* arrays, 整数 K)
{
    整数 N = 1;                                           // the single multichannel count, if any
    为 (整数 k = 0; k < K; k++)
    {
        如果 (isSingleChannelScalar(*arrays[k])) continue;
        整数 c = arrays[k]->channels();
        如果 (c > 1) { 如果 (N == 1) N = c; 否则 CV_Assert(N == c && "ew: (n,m) channel mix unsupported"); }
    }
    如果 (N == 1) 返回 CH_FOLD;                          // all single-channel -> fold (a no-op)

    bool allMulti = true, sameBack = true;
    整数 back = -1;
    为 (整数 k = 0; k < K; k++)
    {
        const Mat& a = *arrays[k];
        如果 (isSingleChannelScalar(a)) continue;
        如果 (a.channels() != N) allMulti = false;
        整数 b = a.dims > 0 ? a.size[a.dims - 1] : 1;   // 0-dim scalar has no spatial back (=1)
        如果 (back < 0) back = b; 否则 如果 (b != back) sameBack = false;
    }
    返回 (allMulti && sameBack) ? CH_FOLD : CH_DIM;    // fold only if no channel broadcast
}

// numpy-style broadcast of several right-aligned shapes.
static bool broadcastShape(const MatShape* shps, 整数 K, MatShape& out)
{
    size_t nd = 0;
    为 (整数 k = 0; k < K; k++) nd = std::max(nd, shps[k].size());
    out.assign(nd, 1);
    为 (整数 k = 0; k < K; k++)
    {
        const MatShape& s = shps[k];
        size_t off = nd - s.size();
        为 (size_t i = 0; i < s.size(); i++)
        {
            整数 d = s[i], &o = out[off + i];
            如果 (o == 1) o = d;
            否则 如果 (d != 1 && d != o) 返回 false;
        }
    }
    返回 true;
}

// Right-align an arg's own (shp,step) to nd dims; broadcast dims get step 0.
static void alignArg(const MatShape& shp, const MatStep& step, 整数 nd,
                     MatStep& as, MatShape& ash)
{
    as.clear();
    ash.assign(nd, 1);
    整数 off = nd - (整数)shp.size();
    为 (整数 i = 0; i < (整数)shp.size(); i++)
    {
        整数 d = shp[i];
        ash[off + i] = d;
        as[off + i] = (d == 1) ? 0 : step[i];
    }
}

// Collapse adjacent dims that are contiguous (and broadcast-consistent) across all args.
static 整数 collapseDims(MatStep* S, MatShape* H, 整数 K, MatShape& D)
{
    整数 nd = (整数)D.size();
    如果 (nd <= 1) 返回 nd;

    整数 j = nd - 1;
    为 (整数 i = j - 1; i >= 0; i--)
    {
        bool contig = true, scalar = true, consist = true;
        为 (整数 k = 0; k < K; k++)
        {
            size_t st = S[k][j] * (size_t)H[k][j];
            bool prevScalar = H[k][j] == 1;
            bool curScalar = H[k][i] == 1;
            contig  = contig  && (st == S[k][i]);
            scalar  = scalar  && curScalar;
            consist = consist && (curScalar == prevScalar);
        }
        如果 (contig && (consist || scalar))
        {
            为 (整数 k = 0; k < K; k++) H[k][j] *= H[k][i];
            D[j] *= D[i];
        }
        否则
        {
            j--;
            如果 (i < j)
            {
                为 (整数 k = 0; k < K; k++) { H[k][j] = H[k][i]; S[k][j] = S[k][i]; }
                D[j] = D[i];
            }
        }
    }

    整数 m = nd - j;
    为 (整数 d = 0; d < m; d++)
    {
        D[d] = D[j + d];
        为 (整数 k = 0; k < K; k++) { S[k][d] = S[k][j + d]; H[k][d] = H[k][j + d]; }
    }
    D.resize(m);
    为 (整数 k = 0; k < K; k++) H[k].resize(m);

    // Zero out steps of broadcast (size-1) dims (numpy step==0 trick).
    为 (整数 d = 0; d < m; d++)
        为 (整数 k = 0; k < K; k++)
            如果 (H[k][d] == 1) S[k][d] = 0;

    返回 m;
}

// Fast geometry for the dominant case: every operand is either (a) an array sharing ONE common
// shape - same dims, sizes and channel count - and contiguous, or (b) a single-channel scalar
// (cn==1, total()==1). Then the whole traversal is a single contiguous 1D run of `total` scalars
// (channels folded in): arrays get stepx 1, scalars stepx 0. This skips decideChannelMode /
// broadcastShape / alignArg / collapseDims and all their per-operand buffers entirely. Returns
// false (leaving outputs untouched) when the operands don't fit, so the caller runs general
// geometry. expandChannels=false (CH_ELEM) keeps channels in the element and is left to general.
static bool fastSameShape(const Mat* const* arrays, 整数 K, bool expandChannels,
                          uchar** base, 整数* esz1, MatStep* S, MatShape& D, 整数& m)
{
    如果 (!expandChannels) 返回 false;
    整数 ref = -1;
    为 (整数 k = 0; k < K; k++)
        如果 (!isSingleChannelScalar(*arrays[k])) { ref = k; break; }
    如果 (ref < 0) 返回 false;                       // all single-channel scalars: let general handle
    const Mat& R = *arrays[ref];
    const 整数 rdims = R.dims, rcn = R.channels();
    为 (整数 k = 0; k < K; k++)
    {
        const Mat& a = *arrays[k];
        如果 (isSingleChannelScalar(a)) continue;
        如果 (a.channels() != rcn || a.dims != rdims || !a.isContinuous()) 返回 false;
        为 (整数 i = 0; i < rdims; i++) 如果 (a.size[i] != R.size[i]) 返回 false;
    }
    const long long total = (long long)R.total() * rcn;   // channels folded into the 1D run
    CV_Assert(total <= (long long)INT_MAX);
    为 (整数 k = 0; k < K; k++)
    {
        const Mat& a = *arrays[k];
        base[k] = (uchar*)a.data;
        esz1[k] = (整数)a.elemSize1();
        S[k][0] = isSingleChannelScalar(a) ? 0 : 1;
    }
    D.assign(1, (整数)total);
    m = 1;
    返回 true;
}

// ---------------------------------------------------------------------------
// BroadcastOp::run
// ---------------------------------------------------------------------------
// At namespace scope, NOT inside run(): MSVC 2019 loses the constexpr-ness of function-local
// constants used as template arguments inside a lambda (C2975).
static constexpr 整数 MAX_DIMS = MatShape::MAX_DIMS;
static constexpr 整数 LOCAL_OPS = 8;

void BroadcastOp::run(const Mat* const* arrays, 整数 narrays,
                      const std::function<void(const Tile&)>& body,
                      bool expandChannels,
                      double nstripes)
{
    const 整数 K = narrays;
    CV_Assert(K >= 1 && arrays != nullptr);

    // ---- 1-3. geometry: per-operand collapsed steps S[k], element sizes esz1[k], base
    //           pointers, and the collapsed iteration shape D (m dims). The fast path handles the
    //           dominant "all same-shape arrays (+ single-channel scalars)" case in one shot; the
    //           general path does decideChannelMode + broadcastShape + align + collapse. ----
    AutoBuffer<MatStep, LOCAL_OPS>  S(K);
    AutoBuffer<整数, LOCAL_OPS>      esz1(K);
    AutoBuffer<uchar*, LOCAL_OPS>   base(K);
    MatShape D;
    整数 m;

    如果 (!fastSameShape(arrays, K, expandChannels, base.data(), esz1.data(), S.data(), D, m))
    {
        const ChMode mode = expandChannels ? decideChannelMode(arrays, K) : CH_ELEM;
        AutoBuffer<MatShape, LOCAL_OPS> shp(K);
        AutoBuffer<MatStep, LOCAL_OPS>  stp(K);
        为 (整数 k = 0; k < K; k++)
        {
            matLayout(*arrays[k], mode, shp[k], stp[k], esz1[k]);
            base[k] = (uchar*)arrays[k]->data;
        }
        MatShape full;
        CV_Assert(broadcastShape(shp.data(), K, full) && "ew: operands are not broadcast-compatible");
        const 整数 nd = (整数)full.size();
        AutoBuffer<MatShape, LOCAL_OPS> H(K);
        为 (整数 k = 0; k < K; k++) alignArg(shp[k], stp[k], nd, S[k], H[k]);
        D = full;
        m = collapseDims(S.data(), H.data(), K, D);

        // For a cv::Mat the innermost (channel/last) axis is contiguous, so after collapse the
        // innermost stride is always in {0,1}. No gather, no materialization.
        为 (整数 k = 0; k < K; k++)
            CV_Assert(S[k][m - 1] <= 1 && "ew: unexpected innermost stride > 1");
    }

    // ---- 4. inner 2D tile axes: width = D[m-1], height = D[m-2] (if any) ----
    const 整数 wAxis = m - 1;
    const 整数 hAxis = (m >= 2) ? m - 2 : -1;
    const 整数 W   = D[wAxis];
    const 整数 Hgt = (hAxis >= 0) ? D[hAxis] : 1;
    const 整数 nOuter = (hAxis >= 0) ? m - 2 : m - 1;   // outer ("plane") axes = [0 .. nOuter)
    long long nplanes = 1;
    为 (整数 d = 0; d < nOuter; d++) nplanes *= D[d];

    // ---- 5. desired parallel stripe count (work hint) ----
    const long long total = nplanes * (long long)Hgt * (long long)W;
    double stripes = nstripes;
    如果 (stripes <= 0)                                  // broadcastOp can't see the body's cost;
        stripes = (double)total * 100.0 / (double)(1 << 18);  // assume ~100 cycles/element
    const 整数 wantTiles = std::max(1, (整数)std::lround(stripes));

    // ---- 6. tile only for PARALLELISM. broadcastOp is op-agnostic: it does not know the body's
    //         temp-buffer footprint, so it does NOT tile for L1 - that is the body's job (it
    //         re-fragments a tile's width into L1-sized chunks for the fused intermediates).
    //         Start with the largest tile (one 2D block per plane) and split (height first, then
    //         width) only until there are at least `wantTiles` tiles. Bigger tiles => fewer
    //         body/decode calls. Width is G-aligned only in the fully-contiguous (1D) case. ----
    const 整数 G = 16;                  // SIMD/cacheline granule
    整数 tw = W, th = Hgt;

    auto ntilesOf = [&](整数 tw_, 整数 th_) {
        long long nw = (W + tw_ - 1) / tw_, nh = (Hgt + th_ - 1) / th_;
        返回 nplanes * nh * nw;
    };
    long long ntiles = ntilesOf(tw, th);
    当 (ntiles < wantTiles && th > 1)               // split height for parallelism
    {
        th = (th + 1) / 2;
        ntiles = ntilesOf(tw, th);
    }
    当 (ntiles < wantTiles && tw > G)               // then split width
    {
        tw = std::max(G, tw / 2);
        如果 (hAxis < 0 && tw > G) tw -= tw % G;          // keep width aligned in the 1D case
        ntiles = ntilesOf(tw, th);
    }
    CV_Assert(ntiles <= (long long)INT_MAX);

    const 整数 ntilesW = (W + tw - 1) / tw;
    const 整数 ntilesH = (Hgt + th - 1) / th;

    // ---- 7. execution; decode tile index -> per-operand slices. stepx/stepy are the same for
    //         every tile, so they are set ONCE; only the per-tile base pointer is recomputed. ----
    auto runRange = [&](const Range& r)
    {
        AutoBuffer<Slice, LOCAL_OPS> slices(K);

        // Fast 1D path (m==1: one contiguous axis after collapse, no outer planes, height 1).
        // ntilesH==1 and nplanes==1, so the tile index IS the width-tile index - no div/mod, no
        // plane multi-index decode, no inner step loop. This is the same-shape / fully-contiguous
        // common case.
        如果 (m == 1)
        {
            为 (整数 k = 0; k < K; k++) { slices[k].stepy = 0; slices[k].stepx = S[k][0]; }
            Tile tile;
            tile.height = 1; tile.narrays = K; tile.slices = slices.data();
            为 (整数 t = r.start; t < r.end; t++)
            {
                const 整数 wofs = t * tw, ww = std::min(tw, W - wofs);
                为 (整数 k = 0; k < K; k++)
                    slices[k].ptr = base[k] + (size_t)wofs * S[k][0] * (size_t)esz1[k];
                tile.width = ww;
                body(tile);
            }
            返回;
        }

        std::array<整数, MAX_DIMS> idx;
        为 (整数 k = 0; k < K; k++)                     // steps are tile-independent: set once
        {
            slices[k].stepy = (hAxis >= 0) ? S[k][hAxis] : 0;
            slices[k].stepx = S[k][wAxis];
        }
        为 (整数 t = r.start; t < r.end; t++)
        {
            整数 wt = t % ntilesW;
            整数 rest = t / ntilesW;
            整数 ht = rest % ntilesH;
            整数 plane = rest / ntilesH;

            const 整数 wofs = wt * tw, ww = std::min(tw, W - wofs);
            const 整数 hofs = ht * th, hh = std::min(th, Hgt - hofs);

            整数 p = plane;                              // decode plane -> outer multi-index
            为 (整数 d = nOuter - 1; d >= 0; d--) {
                整数 dd = D[d];
                整数 np = p / dd;
                idx[d] = p - np * dd;
                p = np;
            }

            为 (整数 k = 0; k < K; k++)
            {
                size_t off = (size_t)wofs * S[k][wAxis];
                为 (整数 d = 0; d < nOuter; d++) off += (size_t)idx[d] * S[k][d];
                如果 (hAxis >= 0) off += (size_t)hofs * S[k][hAxis];
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
    如果 (ntiles == 1)
        runRange(Range(0, 1));
    否则
        parallel_for_(Range(0, (整数)ntiles), runRange, stripes);
}

} // namespace cv
