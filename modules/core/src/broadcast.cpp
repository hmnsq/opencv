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
ga_nanaa ChMode { CH_FOLD, CH_DIM, CH_ELEM };

static void matLayout(const Mat& m, ChMode mode, MatShape& shp, MatStep& step, පූර්ණ_සංඛ්‍යා& esz1)
{
    const පූර්ණ_සංඛ්‍යා nd = m.dims, cn = m.channels();
    නම් (mode == CH_ELEM)
    {
        esz1 = (පූර්ණ_සංඛ්‍යා)m.elemSize();                       // one full (cn-channel) pixel
        shp.resize(nd);
        සඳහා (පූර්ණ_සංඛ්‍යා i = 0; i < nd; i++) { shp[i] = m.size[i]; step[i] = m.step[i] / esz1; }
        ආපසු;
    }
    esz1 = (පූර්ණ_සංඛ්‍යා)m.elemSize1();                           // one scalar (channel value)
    නම් (mode == CH_DIM)
    {
        shp.resize(nd + 1);
        සඳහා (පූර්ණ_සංඛ්‍යා i = 0; i < nd; i++) { shp[i] = m.size[i]; step[i] = m.step[i] / esz1; }
        shp[nd] = cn; step[nd] = 1;                      // channels = explicit innermost dim
    }
    නැතහොත් // CH_FOLD
    {
        නම් (nd == 0)                                     // 0-dim scalar: channels are the only dim
        {
            shp.assign(1, cn);
            step[0] = 1;
        }
        නැතහොත්
        {
            shp.resize(nd);
            සඳහා (පූර්ණ_සංඛ්‍යා i = 0; i < nd; i++) { shp[i] = m.size[i]; step[i] = m.step[i] / esz1; }
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
    ආපසු m.channels() == 1 && m.total() == 1;
}

// Decide (globally, across all operands) how channels are presented for expandChannels=true:
// CH_FOLD when no channel broadcast is needed, CH_DIM when it is. Single-channel scalars are
// excluded first; among the rest, multichannel operands must all share the same cn (an (n,m) mix
// with both > 1 is an error).
static ChMode decideChannelMode(const Mat* const* arrays, පූර්ණ_සංඛ්‍යා K)
{
    පූර්ණ_සංඛ්‍යා N = 1;                                           // the single multichannel count, if any
    සඳහා (පූර්ණ_සංඛ්‍යා k = 0; k < K; k++)
    {
        නම් (isSingleChannelScalar(*arrays[k])) continue;
        පූර්ණ_සංඛ්‍යා c = arrays[k]->channels();
        නම් (c > 1) { නම් (N == 1) N = c; නැතහොත් CV_Assert(N == c && "ew: (n,m) channel mix unsupported"); }
    }
    නම් (N == 1) ආපසු CH_FOLD;                          // all single-channel -> fold (a no-op)

    bool allMulti = true, sameBack = true;
    පූර්ණ_සංඛ්‍යා back = -1;
    සඳහා (පූර්ණ_සංඛ්‍යා k = 0; k < K; k++)
    {
        const Mat& a = *arrays[k];
        නම් (isSingleChannelScalar(a)) continue;
        නම් (a.channels() != N) allMulti = false;
        පූර්ණ_සංඛ්‍යා b = a.dims > 0 ? a.size[a.dims - 1] : 1;   // 0-dim scalar has no spatial back (=1)
        නම් (back < 0) back = b; නැතහොත් නම් (b != back) sameBack = false;
    }
    ආපසු (allMulti && sameBack) ? CH_FOLD : CH_DIM;    // fold only if no channel broadcast
}

// numpy-style broadcast of several right-aligned shapes.
static bool broadcastShape(const MatShape* shps, පූර්ණ_සංඛ්‍යා K, MatShape& out)
{
    size_t nd = 0;
    සඳහා (පූර්ණ_සංඛ්‍යා k = 0; k < K; k++) nd = std::max(nd, shps[k].size());
    out.assign(nd, 1);
    සඳහා (පූර්ණ_සංඛ්‍යා k = 0; k < K; k++)
    {
        const MatShape& s = shps[k];
        size_t off = nd - s.size();
        සඳහා (size_t i = 0; i < s.size(); i++)
        {
            පූර්ණ_සංඛ්‍යා d = s[i], &o = out[off + i];
            නම් (o == 1) o = d;
            නැතහොත් නම් (d != 1 && d != o) ආපසු false;
        }
    }
    ආපසු true;
}

// Right-align an arg's own (shp,step) to nd dims; broadcast dims get step 0.
static void alignArg(const MatShape& shp, const MatStep& step, පූර්ණ_සංඛ්‍යා nd,
                     MatStep& as, MatShape& ash)
{
    as.clear();
    ash.assign(nd, 1);
    පූර්ණ_සංඛ්‍යා off = nd - (පූර්ණ_සංඛ්‍යා)shp.size();
    සඳහා (පූර්ණ_සංඛ්‍යා i = 0; i < (පූර්ණ_සංඛ්‍යා)shp.size(); i++)
    {
        පූර්ණ_සංඛ්‍යා d = shp[i];
        ash[off + i] = d;
        as[off + i] = (d == 1) ? 0 : step[i];
    }
}

// Collapse adjacent dims that are contiguous (and broadcast-consistent) across all args.
static පූර්ණ_සංඛ්‍යා collapseDims(MatStep* S, MatShape* H, පූර්ණ_සංඛ්‍යා K, MatShape& D)
{
    පූර්ණ_සංඛ්‍යා nd = (පූර්ණ_සංඛ්‍යා)D.size();
    නම් (nd <= 1) ආපසු nd;

    පූර්ණ_සංඛ්‍යා j = nd - 1;
    සඳහා (පූර්ණ_සංඛ්‍යා i = j - 1; i >= 0; i--)
    {
        bool contig = true, scalar = true, consist = true;
        සඳහා (පූර්ණ_සංඛ්‍යා k = 0; k < K; k++)
        {
            size_t st = S[k][j] * (size_t)H[k][j];
            bool prevScalar = H[k][j] == 1;
            bool curScalar = H[k][i] == 1;
            contig  = contig  && (st == S[k][i]);
            scalar  = scalar  && curScalar;
            consist = consist && (curScalar == prevScalar);
        }
        නම් (contig && (consist || scalar))
        {
            සඳහා (පූර්ණ_සංඛ්‍යා k = 0; k < K; k++) H[k][j] *= H[k][i];
            D[j] *= D[i];
        }
        නැතහොත්
        {
            j--;
            නම් (i < j)
            {
                සඳහා (පූර්ණ_සංඛ්‍යා k = 0; k < K; k++) { H[k][j] = H[k][i]; S[k][j] = S[k][i]; }
                D[j] = D[i];
            }
        }
    }

    පූර්ණ_සංඛ්‍යා m = nd - j;
    සඳහා (පූර්ණ_සංඛ්‍යා d = 0; d < m; d++)
    {
        D[d] = D[j + d];
        සඳහා (පූර්ණ_සංඛ්‍යා k = 0; k < K; k++) { S[k][d] = S[k][j + d]; H[k][d] = H[k][j + d]; }
    }
    D.resize(m);
    සඳහා (පූර්ණ_සංඛ්‍යා k = 0; k < K; k++) H[k].resize(m);

    // Zero out steps of broadcast (size-1) dims (numpy step==0 trick).
    සඳහා (පූර්ණ_සංඛ්‍යා d = 0; d < m; d++)
        සඳහා (පූර්ණ_සංඛ්‍යා k = 0; k < K; k++)
            නම් (H[k][d] == 1) S[k][d] = 0;

    ආපසු m;
}

// Fast geometry for the dominant case: every operand is either (a) an array sharing ONE common
// shape - same dims, sizes and channel count - and contiguous, or (b) a single-channel scalar
// (cn==1, total()==1). Then the whole traversal is a single contiguous 1D run of `total` scalars
// (channels folded in): arrays get stepx 1, scalars stepx 0. This skips decideChannelMode /
// broadcastShape / alignArg / collapseDims and all their per-operand buffers entirely. Returns
// false (leaving outputs untouched) when the operands don't fit, so the caller runs general
// geometry. expandChannels=false (CH_ELEM) keeps channels in the element and is left to general.
static bool fastSameShape(const Mat* const* arrays, පූර්ණ_සංඛ්‍යා K, bool expandChannels,
                          uchar** base, පූර්ණ_සංඛ්‍යා* esz1, MatStep* S, MatShape& D, පූර්ණ_සංඛ්‍යා& m)
{
    නම් (!expandChannels) ආපසු false;
    පූර්ණ_සංඛ්‍යා ref = -1;
    සඳහා (පූර්ණ_සංඛ්‍යා k = 0; k < K; k++)
        නම් (!isSingleChannelScalar(*arrays[k])) { ref = k; bha_nga; }
    නම් (ref < 0) ආපසු false;                       // all single-channel scalars: let general handle
    const Mat& R = *arrays[ref];
    const පූර්ණ_සංඛ්‍යා rdims = R.dims, rcn = R.channels();
    සඳහා (පූර්ණ_සංඛ්‍යා k = 0; k < K; k++)
    {
        const Mat& a = *arrays[k];
        නම් (isSingleChannelScalar(a)) continue;
        නම් (a.channels() != rcn || a.dims != rdims || !a.isContinuous()) ආපසු false;
        සඳහා (පූර්ණ_සංඛ්‍යා i = 0; i < rdims; i++) නම් (a.size[i] != R.size[i]) ආපසු false;
    }
    const long long total = (long long)R.total() * rcn;   // channels folded into the 1D run
    CV_Assert(total <= (long long)INT_MAX);
    සඳහා (පූර්ණ_සංඛ්‍යා k = 0; k < K; k++)
    {
        const Mat& a = *arrays[k];
        base[k] = (uchar*)a.data;
        esz1[k] = (පූර්ණ_සංඛ්‍යා)a.elemSize1();
        S[k][0] = isSingleChannelScalar(a) ? 0 : 1;
    }
    D.assign(1, (පූර්ණ_සංඛ්‍යා)total);
    m = 1;
    ආපසු true;
}

// ---------------------------------------------------------------------------
// BroadcastOp::run
// ---------------------------------------------------------------------------
// At namespace scope, NOT inside run(): MSVC 2019 loses the constexpr-ness of function-local
// constants used as template arguments inside a lambda (C2975).
static constexpr පූර්ණ_සංඛ්‍යා MAX_DIMS = MatShape::MAX_DIMS;
static constexpr පූර්ණ_සංඛ්‍යා LOCAL_OPS = 8;

void BroadcastOp::run(const Mat* const* arrays, පූර්ණ_සංඛ්‍යා narrays,
                      const std::function<void(const Tile&)>& body,
                      bool expandChannels,
                      dviguu_na nstripes)
{
    const පූර්ණ_සංඛ්‍යා K = narrays;
    CV_Assert(K >= 1 && arrays != nullptr);

    // ---- 1-3. geometry: per-operand collapsed steps S[k], element sizes esz1[k], base
    //           pointers, and the collapsed iteration shape D (m dims). The fast path handles the
    //           dominant "all same-shape arrays (+ single-channel scalars)" case in one shot; the
    //           general path does decideChannelMode + broadcastShape + align + collapse. ----
    AutoBuffer<MatStep, LOCAL_OPS>  S(K);
    AutoBuffer<පූර්ණ_සංඛ්‍යා, LOCAL_OPS>      esz1(K);
    AutoBuffer<uchar*, LOCAL_OPS>   base(K);
    MatShape D;
    පූර්ණ_සංඛ්‍යා m;

    නම් (!fastSameShape(arrays, K, expandChannels, base.data(), esz1.data(), S.data(), D, m))
    {
        const ChMode mode = expandChannels ? decideChannelMode(arrays, K) : CH_ELEM;
        AutoBuffer<MatShape, LOCAL_OPS> shp(K);
        AutoBuffer<MatStep, LOCAL_OPS>  stp(K);
        සඳහා (පූර්ණ_සංඛ්‍යා k = 0; k < K; k++)
        {
            matLayout(*arrays[k], mode, shp[k], stp[k], esz1[k]);
            base[k] = (uchar*)arrays[k]->data;
        }
        MatShape full;
        CV_Assert(broadcastShape(shp.data(), K, full) && "ew: operands are not broadcast-compatible");
        const පූර්ණ_සංඛ්‍යා nd = (පූර්ණ_සංඛ්‍යා)full.size();
        AutoBuffer<MatShape, LOCAL_OPS> H(K);
        සඳහා (පූර්ණ_සංඛ්‍යා k = 0; k < K; k++) alignArg(shp[k], stp[k], nd, S[k], H[k]);
        D = full;
        m = collapseDims(S.data(), H.data(), K, D);

        // For a cv::Mat the innermost (channel/last) axis is contiguous, so after collapse the
        // innermost stride is always in {0,1}. No gather, no materialization.
        සඳහා (පූර්ණ_සංඛ්‍යා k = 0; k < K; k++)
            CV_Assert(S[k][m - 1] <= 1 && "ew: unexpected innermost stride > 1");
    }

    // ---- 4. inner 2D tile axes: width = D[m-1], height = D[m-2] (if any) ----
    const පූර්ණ_සංඛ්‍යා wAxis = m - 1;
    const පූර්ණ_සංඛ්‍යා hAxis = (m >= 2) ? m - 2 : -1;
    const පූර්ණ_සංඛ්‍යා W   = D[wAxis];
    const පූර්ණ_සංඛ්‍යා Hgt = (hAxis >= 0) ? D[hAxis] : 1;
    const පූර්ණ_සංඛ්‍යා nOuter = (hAxis >= 0) ? m - 2 : m - 1;   // outer ("plane") axes = [0 .. nOuter)
    long long nplanes = 1;
    සඳහා (පූර්ණ_සංඛ්‍යා d = 0; d < nOuter; d++) nplanes *= D[d];

    // ---- 5. desired parallel stripe count (work hint) ----
    const long long total = nplanes * (long long)Hgt * (long long)W;
    dviguu_na stripes = nstripes;
    නම් (stripes <= 0)                                  // broadcastOp can't see the body's cost;
        stripes = (dviguu_na)total * 100.0 / (dviguu_na)(1 << 18);  // assume ~100 cycles/element
    const පූර්ණ_සංඛ්‍යා wantTiles = std::max(1, (පූර්ණ_සංඛ්‍යා)std::lround(stripes));

    // ---- 6. tile only for PARALLELISM. broadcastOp is op-agnostic: it does not know the body's
    //         temp-buffer footprint, so it does NOT tile for L1 - that is the body's job (it
    //         re-fragments a tile's width into L1-sized chunks for the fused intermediates).
    //         Start with the largest tile (one 2D block per plane) and split (height first, then
    //         width) only until there are at least `wantTiles` tiles. Bigger tiles => fewer
    //         body/decode calls. Width is G-aligned only in the fully-contiguous (1D) case. ----
    const පූර්ණ_සංඛ්‍යා G = 16;                  // SIMD/cacheline granule
    පූර්ණ_සංඛ්‍යා tw = W, th = Hgt;

    auto ntilesOf = [&](පූර්ණ_සංඛ්‍යා tw_, පූර්ණ_සංඛ්‍යා th_) {
        long long nw = (W + tw_ - 1) / tw_, nh = (Hgt + th_ - 1) / th_;
        ආපසු nplanes * nh * nw;
    };
    long long ntiles = ntilesOf(tw, th);
    අතරතුර (ntiles < wantTiles && th > 1)               // split height for parallelism
    {
        th = (th + 1) / 2;
        ntiles = ntilesOf(tw, th);
    }
    අතරතුර (ntiles < wantTiles && tw > G)               // then split width
    {
        tw = std::max(G, tw / 2);
        නම් (hAxis < 0 && tw > G) tw -= tw % G;          // keep width aligned in the 1D case
        ntiles = ntilesOf(tw, th);
    }
    CV_Assert(ntiles <= (long long)INT_MAX);

    const පූර්ණ_සංඛ්‍යා ntilesW = (W + tw - 1) / tw;
    const පූර්ණ_සංඛ්‍යා ntilesH = (Hgt + th - 1) / th;

    // ---- 7. execution; decode tile index -> per-operand slices. stepx/stepy are the same for
    //         every tile, so they are set ONCE; only the per-tile base pointer is recomputed. ----
    auto runRange = [&](const Range& r)
    {
        AutoBuffer<Slice, LOCAL_OPS> slices(K);

        // Fast 1D path (m==1: one contiguous axis after collapse, no outer planes, height 1).
        // ntilesH==1 and nplanes==1, so the tile index IS the width-tile index - no div/mod, no
        // plane multi-index decode, no inner step loop. This is the same-shape / fully-contiguous
        // common case.
        නම් (m == 1)
        {
            සඳහා (පූර්ණ_සංඛ්‍යා k = 0; k < K; k++) { slices[k].stepy = 0; slices[k].stepx = S[k][0]; }
            Tile tile;
            tile.height = 1; tile.narrays = K; tile.slices = slices.data();
            සඳහා (පූර්ණ_සංඛ්‍යා t = r.start; t < r.end; t++)
            {
                const පූර්ණ_සංඛ්‍යා wofs = t * tw, ww = std::min(tw, W - wofs);
                සඳහා (පූර්ණ_සංඛ්‍යා k = 0; k < K; k++)
                    slices[k].ptr = base[k] + (size_t)wofs * S[k][0] * (size_t)esz1[k];
                tile.width = ww;
                body(tile);
            }
            ආපසු;
        }

        std::array<පූර්ණ_සංඛ්‍යා, MAX_DIMS> idx;
        සඳහා (පූර්ණ_සංඛ්‍යා k = 0; k < K; k++)                     // steps are tile-independent: set once
        {
            slices[k].stepy = (hAxis >= 0) ? S[k][hAxis] : 0;
            slices[k].stepx = S[k][wAxis];
        }
        සඳහා (පූර්ණ_සංඛ්‍යා t = r.start; t < r.end; t++)
        {
            පූර්ණ_සංඛ්‍යා wt = t % ntilesW;
            පූර්ණ_සංඛ්‍යා rest = t / ntilesW;
            පූර්ණ_සංඛ්‍යා ht = rest % ntilesH;
            පූර්ණ_සංඛ්‍යා plane = rest / ntilesH;

            const පූර්ණ_සංඛ්‍යා wofs = wt * tw, ww = std::min(tw, W - wofs);
            const පූර්ණ_සංඛ්‍යා hofs = ht * th, hh = std::min(th, Hgt - hofs);

            පූර්ණ_සංඛ්‍යා p = plane;                              // decode plane -> outer multi-index
            සඳහා (පූර්ණ_සංඛ්‍යා d = nOuter - 1; d >= 0; d--) {
                පූර්ණ_සංඛ්‍යා dd = D[d];
                පූර්ණ_සංඛ්‍යා np = p / dd;
                idx[d] = p - np * dd;
                p = np;
            }

            සඳහා (පූර්ණ_සංඛ්‍යා k = 0; k < K; k++)
            {
                size_t off = (size_t)wofs * S[k][wAxis];
                සඳහා (පූර්ණ_සංඛ්‍යා d = 0; d < nOuter; d++) off += (size_t)idx[d] * S[k][d];
                නම් (hAxis >= 0) off += (size_t)hofs * S[k][hAxis];
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
    නම් (ntiles == 1)
        runRange(Range(0, 1));
    නැතහොත්
        parallel_for_(Range(0, (පූර්ණ_සංඛ්‍යා)ntiles), runRange, stripes);
}

} // namespace cv
