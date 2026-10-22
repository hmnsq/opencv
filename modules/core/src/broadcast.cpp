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

static void matLayout(const Mat& m, ChMode mode, MatShape& shp, MatStep& step, целое& esz1)
{
    const целое nd = m.dims, cn = m.channels();
    если (mode == CH_ELEM)
    {
        esz1 = (целое)m.elemSize();                       // one full (cn-channel) pixel
        shp.resize(nd);
        для (целое i = 0; i < nd; i++) { shp[i] = m.size[i]; step[i] = m.step[i] / esz1; }
        вернуть;
    }
    esz1 = (целое)m.elemSize1();                           // one scalar (channel value)
    если (mode == CH_DIM)
    {
        shp.resize(nd + 1);
        для (целое i = 0; i < nd; i++) { shp[i] = m.size[i]; step[i] = m.step[i] / esz1; }
        shp[nd] = cn; step[nd] = 1;                      // channels = explicit innermost dim
    }
    иначе // CH_FOLD
    {
        если (nd == 0)                                     // 0-dim scalar: channels are the only dim
        {
            shp.assign(1, cn);
            step[0] = 1;
        }
        иначе
        {
            shp.resize(nd);
            для (целое i = 0; i < nd; i++) { shp[i] = m.size[i]; step[i] = m.step[i] / esz1; }
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
    вернуть m.channels() == 1 && m.total() == 1;
}

// Decide (globally, across all operands) how channels are presented for expandChannels=true:
// CH_FOLD when no channel broadcast is needed, CH_DIM when it is. Single-channel scalars are
// excluded first; among the rest, multichannel operands must all share the same cn (an (n,m) mix
// with both > 1 is an error).
static ChMode decideChannelMode(const Mat* const* arrays, целое K)
{
    целое N = 1;                                           // the single multichannel count, if any
    для (целое k = 0; k < K; k++)
    {
        если (isSingleChannelScalar(*arrays[k])) continue;
        целое c = arrays[k]->channels();
        если (c > 1) { если (N == 1) N = c; иначе CV_Assert(N == c && "ew: (n,m) channel mix unsupported"); }
    }
    если (N == 1) вернуть CH_FOLD;                          // all single-channel -> fold (a no-op)

    bool allMulti = true, sameBack = true;
    целое back = -1;
    для (целое k = 0; k < K; k++)
    {
        const Mat& a = *arrays[k];
        если (isSingleChannelScalar(a)) continue;
        если (a.channels() != N) allMulti = false;
        целое b = a.dims > 0 ? a.size[a.dims - 1] : 1;   // 0-dim scalar has no spatial back (=1)
        если (back < 0) back = b; иначе если (b != back) sameBack = false;
    }
    вернуть (allMulti && sameBack) ? CH_FOLD : CH_DIM;    // fold only if no channel broadcast
}

// numpy-style broadcast of several right-aligned shapes.
static bool broadcastShape(const MatShape* shps, целое K, MatShape& out)
{
    size_t nd = 0;
    для (целое k = 0; k < K; k++) nd = std::max(nd, shps[k].size());
    out.assign(nd, 1);
    для (целое k = 0; k < K; k++)
    {
        const MatShape& s = shps[k];
        size_t off = nd - s.size();
        для (size_t i = 0; i < s.size(); i++)
        {
            целое d = s[i], &o = out[off + i];
            если (o == 1) o = d;
            иначе если (d != 1 && d != o) вернуть false;
        }
    }
    вернуть true;
}

// Right-align an arg's own (shp,step) to nd dims; broadcast dims get step 0.
static void alignArg(const MatShape& shp, const MatStep& step, целое nd,
                     MatStep& as, MatShape& ash)
{
    as.clear();
    ash.assign(nd, 1);
    целое off = nd - (целое)shp.size();
    для (целое i = 0; i < (целое)shp.size(); i++)
    {
        целое d = shp[i];
        ash[off + i] = d;
        as[off + i] = (d == 1) ? 0 : step[i];
    }
}

// Collapse adjacent dims that are contiguous (and broadcast-consistent) across all args.
static целое collapseDims(MatStep* S, MatShape* H, целое K, MatShape& D)
{
    целое nd = (целое)D.size();
    если (nd <= 1) вернуть nd;

    целое j = nd - 1;
    для (целое i = j - 1; i >= 0; i--)
    {
        bool contig = true, scalar = true, consist = true;
        для (целое k = 0; k < K; k++)
        {
            size_t st = S[k][j] * (size_t)H[k][j];
            bool prevScalar = H[k][j] == 1;
            bool curScalar = H[k][i] == 1;
            contig  = contig  && (st == S[k][i]);
            scalar  = scalar  && curScalar;
            consist = consist && (curScalar == prevScalar);
        }
        если (contig && (consist || scalar))
        {
            для (целое k = 0; k < K; k++) H[k][j] *= H[k][i];
            D[j] *= D[i];
        }
        иначе
        {
            j--;
            если (i < j)
            {
                для (целое k = 0; k < K; k++) { H[k][j] = H[k][i]; S[k][j] = S[k][i]; }
                D[j] = D[i];
            }
        }
    }

    целое m = nd - j;
    для (целое d = 0; d < m; d++)
    {
        D[d] = D[j + d];
        для (целое k = 0; k < K; k++) { S[k][d] = S[k][j + d]; H[k][d] = H[k][j + d]; }
    }
    D.resize(m);
    для (целое k = 0; k < K; k++) H[k].resize(m);

    // Zero out steps of broadcast (size-1) dims (numpy step==0 trick).
    для (целое d = 0; d < m; d++)
        для (целое k = 0; k < K; k++)
            если (H[k][d] == 1) S[k][d] = 0;

    вернуть m;
}

// Fast geometry for the dominant case: every operand is either (a) an array sharing ONE common
// shape - same dims, sizes and channel count - and contiguous, or (b) a single-channel scalar
// (cn==1, total()==1). Then the whole traversal is a single contiguous 1D run of `total` scalars
// (channels folded in): arrays get stepx 1, scalars stepx 0. This skips decideChannelMode /
// broadcastShape / alignArg / collapseDims and all their per-operand buffers entirely. Returns
// false (leaving outputs untouched) when the operands don't fit, so the caller runs general
// geometry. expandChannels=false (CH_ELEM) keeps channels in the element and is left to general.
static bool fastSameShape(const Mat* const* arrays, целое K, bool expandChannels,
                          uchar** base, целое* esz1, MatStep* S, MatShape& D, целое& m)
{
    если (!expandChannels) вернуть false;
    целое ref = -1;
    для (целое k = 0; k < K; k++)
        если (!isSingleChannelScalar(*arrays[k])) { ref = k; break; }
    если (ref < 0) вернуть false;                       // all single-channel scalars: let general handle
    const Mat& R = *arrays[ref];
    const целое rdims = R.dims, rcn = R.channels();
    для (целое k = 0; k < K; k++)
    {
        const Mat& a = *arrays[k];
        если (isSingleChannelScalar(a)) continue;
        если (a.channels() != rcn || a.dims != rdims || !a.isContinuous()) вернуть false;
        для (целое i = 0; i < rdims; i++) если (a.size[i] != R.size[i]) вернуть false;
    }
    const long long total = (long long)R.total() * rcn;   // channels folded into the 1D run
    CV_Assert(total <= (long long)INT_MAX);
    для (целое k = 0; k < K; k++)
    {
        const Mat& a = *arrays[k];
        base[k] = (uchar*)a.data;
        esz1[k] = (целое)a.elemSize1();
        S[k][0] = isSingleChannelScalar(a) ? 0 : 1;
    }
    D.assign(1, (целое)total);
    m = 1;
    вернуть true;
}

// ---------------------------------------------------------------------------
// BroadcastOp::run
// ---------------------------------------------------------------------------
// At namespace scope, NOT inside run(): MSVC 2019 loses the constexpr-ness of function-local
// constants used as template arguments inside a lambda (C2975).
static constexpr целое MAX_DIMS = MatShape::MAX_DIMS;
static constexpr целое LOCAL_OPS = 8;

void BroadcastOp::run(const Mat* const* arrays, целое narrays,
                      const std::function<void(const Tile&)>& body,
                      bool expandChannels,
                      double nstripes)
{
    const целое K = narrays;
    CV_Assert(K >= 1 && arrays != nullptr);

    // ---- 1-3. geometry: per-operand collapsed steps S[k], element sizes esz1[k], base
    //           pointers, and the collapsed iteration shape D (m dims). The fast path handles the
    //           dominant "all same-shape arrays (+ single-channel scalars)" case in one shot; the
    //           general path does decideChannelMode + broadcastShape + align + collapse. ----
    AutoBuffer<MatStep, LOCAL_OPS>  S(K);
    AutoBuffer<целое, LOCAL_OPS>      esz1(K);
    AutoBuffer<uchar*, LOCAL_OPS>   base(K);
    MatShape D;
    целое m;

    если (!fastSameShape(arrays, K, expandChannels, base.data(), esz1.data(), S.data(), D, m))
    {
        const ChMode mode = expandChannels ? decideChannelMode(arrays, K) : CH_ELEM;
        AutoBuffer<MatShape, LOCAL_OPS> shp(K);
        AutoBuffer<MatStep, LOCAL_OPS>  stp(K);
        для (целое k = 0; k < K; k++)
        {
            matLayout(*arrays[k], mode, shp[k], stp[k], esz1[k]);
            base[k] = (uchar*)arrays[k]->data;
        }
        MatShape full;
        CV_Assert(broadcastShape(shp.data(), K, full) && "ew: operands are not broadcast-compatible");
        const целое nd = (целое)full.size();
        AutoBuffer<MatShape, LOCAL_OPS> H(K);
        для (целое k = 0; k < K; k++) alignArg(shp[k], stp[k], nd, S[k], H[k]);
        D = full;
        m = collapseDims(S.data(), H.data(), K, D);

        // For a cv::Mat the innermost (channel/last) axis is contiguous, so after collapse the
        // innermost stride is always in {0,1}. No gather, no materialization.
        для (целое k = 0; k < K; k++)
            CV_Assert(S[k][m - 1] <= 1 && "ew: unexpected innermost stride > 1");
    }

    // ---- 4. inner 2D tile axes: width = D[m-1], height = D[m-2] (if any) ----
    const целое wAxis = m - 1;
    const целое hAxis = (m >= 2) ? m - 2 : -1;
    const целое W   = D[wAxis];
    const целое Hgt = (hAxis >= 0) ? D[hAxis] : 1;
    const целое nOuter = (hAxis >= 0) ? m - 2 : m - 1;   // outer ("plane") axes = [0 .. nOuter)
    long long nplanes = 1;
    для (целое d = 0; d < nOuter; d++) nplanes *= D[d];

    // ---- 5. desired parallel stripe count (work hint) ----
    const long long total = nplanes * (long long)Hgt * (long long)W;
    double stripes = nstripes;
    если (stripes <= 0)                                  // broadcastOp can't see the body's cost;
        stripes = (double)total * 100.0 / (double)(1 << 18);  // assume ~100 cycles/element
    const целое wantTiles = std::max(1, (целое)std::lround(stripes));

    // ---- 6. tile only for PARALLELISM. broadcastOp is op-agnostic: it does not know the body's
    //         temp-buffer footprint, so it does NOT tile for L1 - that is the body's job (it
    //         re-fragments a tile's width into L1-sized chunks for the fused intermediates).
    //         Start with the largest tile (one 2D block per plane) and split (height first, then
    //         width) only until there are at least `wantTiles` tiles. Bigger tiles => fewer
    //         body/decode calls. Width is G-aligned only in the fully-contiguous (1D) case. ----
    const целое G = 16;                  // SIMD/cacheline granule
    целое tw = W, th = Hgt;

    auto ntilesOf = [&](целое tw_, целое th_) {
        long long nw = (W + tw_ - 1) / tw_, nh = (Hgt + th_ - 1) / th_;
        вернуть nplanes * nh * nw;
    };
    long long ntiles = ntilesOf(tw, th);
    пока (ntiles < wantTiles && th > 1)               // split height for parallelism
    {
        th = (th + 1) / 2;
        ntiles = ntilesOf(tw, th);
    }
    пока (ntiles < wantTiles && tw > G)               // then split width
    {
        tw = std::max(G, tw / 2);
        если (hAxis < 0 && tw > G) tw -= tw % G;          // keep width aligned in the 1D case
        ntiles = ntilesOf(tw, th);
    }
    CV_Assert(ntiles <= (long long)INT_MAX);

    const целое ntilesW = (W + tw - 1) / tw;
    const целое ntilesH = (Hgt + th - 1) / th;

    // ---- 7. execution; decode tile index -> per-operand slices. stepx/stepy are the same for
    //         every tile, so they are set ONCE; only the per-tile base pointer is recomputed. ----
    auto runRange = [&](const Range& r)
    {
        AutoBuffer<Slice, LOCAL_OPS> slices(K);

        // Fast 1D path (m==1: one contiguous axis after collapse, no outer planes, height 1).
        // ntilesH==1 and nplanes==1, so the tile index IS the width-tile index - no div/mod, no
        // plane multi-index decode, no inner step loop. This is the same-shape / fully-contiguous
        // common case.
        если (m == 1)
        {
            для (целое k = 0; k < K; k++) { slices[k].stepy = 0; slices[k].stepx = S[k][0]; }
            Tile tile;
            tile.height = 1; tile.narrays = K; tile.slices = slices.data();
            для (целое t = r.start; t < r.end; t++)
            {
                const целое wofs = t * tw, ww = std::min(tw, W - wofs);
                для (целое k = 0; k < K; k++)
                    slices[k].ptr = base[k] + (size_t)wofs * S[k][0] * (size_t)esz1[k];
                tile.width = ww;
                body(tile);
            }
            вернуть;
        }

        std::array<целое, MAX_DIMS> idx;
        для (целое k = 0; k < K; k++)                     // steps are tile-independent: set once
        {
            slices[k].stepy = (hAxis >= 0) ? S[k][hAxis] : 0;
            slices[k].stepx = S[k][wAxis];
        }
        для (целое t = r.start; t < r.end; t++)
        {
            целое wt = t % ntilesW;
            целое rest = t / ntilesW;
            целое ht = rest % ntilesH;
            целое plane = rest / ntilesH;

            const целое wofs = wt * tw, ww = std::min(tw, W - wofs);
            const целое hofs = ht * th, hh = std::min(th, Hgt - hofs);

            целое p = plane;                              // decode plane -> outer multi-index
            для (целое d = nOuter - 1; d >= 0; d--) {
                целое dd = D[d];
                целое np = p / dd;
                idx[d] = p - np * dd;
                p = np;
            }

            для (целое k = 0; k < K; k++)
            {
                size_t off = (size_t)wofs * S[k][wAxis];
                для (целое d = 0; d < nOuter; d++) off += (size_t)idx[d] * S[k][d];
                если (hAxis >= 0) off += (size_t)hofs * S[k][hAxis];
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
    если (ntiles == 1)
        runRange(Range(0, 1));
    иначе
        parallel_for_(Range(0, (целое)ntiles), runRange, stripes);
}

} // namespace cv
