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

static void matLayout(const Mat& m, ChMode mode, MatShape& shp, MatStep& step, ចំនួនគត់& esz1)
{
    const ចំនួនគត់ nd = m.dims, cn = m.channels();
    ប្រសិនបើ (mode == CH_ELEM)
    {
        esz1 = (ចំនួនគត់)m.elemSize();                       // one full (cn-channel) pixel
        shp.resize(nd);
        សម្រាប់ (ចំនួនគត់ i = 0; i < nd; i++) { shp[i] = m.size[i]; step[i] = m.step[i] / esz1; }
        ត្រឡប់;
    }
    esz1 = (ចំនួនគត់)m.elemSize1();                           // one scalar (channel value)
    ប្រសិនបើ (mode == CH_DIM)
    {
        shp.resize(nd + 1);
        សម្រាប់ (ចំនួនគត់ i = 0; i < nd; i++) { shp[i] = m.size[i]; step[i] = m.step[i] / esz1; }
        shp[nd] = cn; step[nd] = 1;                      // channels = explicit innermost dim
    }
    បើមិនដូច្នេះ // CH_FOLD
    {
        ប្រសិនបើ (nd == 0)                                     // 0-dim scalar: channels are the only dim
        {
            shp.assign(1, cn);
            step[0] = 1;
        }
        បើមិនដូច្នេះ
        {
            shp.resize(nd);
            សម្រាប់ (ចំនួនគត់ i = 0; i < nd; i++) { shp[i] = m.size[i]; step[i] = m.step[i] / esz1; }
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
    ត្រឡប់ m.channels() == 1 && m.total() == 1;
}

// Decide (globally, across all operands) how channels are presented for expandChannels=true:
// CH_FOLD when no channel broadcast is needed, CH_DIM when it is. Single-channel scalars are
// excluded first; among the rest, multichannel operands must all share the same cn (an (n,m) mix
// with both > 1 is an error).
static ChMode decideChannelMode(const Mat* const* arrays, ចំនួនគត់ K)
{
    ចំនួនគត់ N = 1;                                           // the single multichannel count, if any
    សម្រាប់ (ចំនួនគត់ k = 0; k < K; k++)
    {
        ប្រសិនបើ (isSingleChannelScalar(*arrays[k])) continue;
        ចំនួនគត់ c = arrays[k]->channels();
        ប្រសិនបើ (c > 1) { ប្រសិនបើ (N == 1) N = c; បើមិនដូច្នេះ CV_Assert(N == c && "ew: (n,m) channel mix unsupported"); }
    }
    ប្រសិនបើ (N == 1) ត្រឡប់ CH_FOLD;                          // all single-channel -> fold (a no-op)

    bool allMulti = true, sameBack = true;
    ចំនួនគត់ back = -1;
    សម្រាប់ (ចំនួនគត់ k = 0; k < K; k++)
    {
        const Mat& a = *arrays[k];
        ប្រសិនបើ (isSingleChannelScalar(a)) continue;
        ប្រសិនបើ (a.channels() != N) allMulti = false;
        ចំនួនគត់ b = a.dims > 0 ? a.size[a.dims - 1] : 1;   // 0-dim scalar has no spatial back (=1)
        ប្រសិនបើ (back < 0) back = b; បើមិនដូច្នេះ ប្រសិនបើ (b != back) sameBack = false;
    }
    ត្រឡប់ (allMulti && sameBack) ? CH_FOLD : CH_DIM;    // fold only if no channel broadcast
}

// numpy-style broadcast of several right-aligned shapes.
static bool broadcastShape(const MatShape* shps, ចំនួនគត់ K, MatShape& out)
{
    size_t nd = 0;
    សម្រាប់ (ចំនួនគត់ k = 0; k < K; k++) nd = std::max(nd, shps[k].size());
    out.assign(nd, 1);
    សម្រាប់ (ចំនួនគត់ k = 0; k < K; k++)
    {
        const MatShape& s = shps[k];
        size_t off = nd - s.size();
        សម្រាប់ (size_t i = 0; i < s.size(); i++)
        {
            ចំនួនគត់ d = s[i], &o = out[off + i];
            ប្រសិនបើ (o == 1) o = d;
            បើមិនដូច្នេះ ប្រសិនបើ (d != 1 && d != o) ត្រឡប់ false;
        }
    }
    ត្រឡប់ true;
}

// Right-align an arg's own (shp,step) to nd dims; broadcast dims get step 0.
static void alignArg(const MatShape& shp, const MatStep& step, ចំនួនគត់ nd,
                     MatStep& as, MatShape& ash)
{
    as.clear();
    ash.assign(nd, 1);
    ចំនួនគត់ off = nd - (ចំនួនគត់)shp.size();
    សម្រាប់ (ចំនួនគត់ i = 0; i < (ចំនួនគត់)shp.size(); i++)
    {
        ចំនួនគត់ d = shp[i];
        ash[off + i] = d;
        as[off + i] = (d == 1) ? 0 : step[i];
    }
}

// Collapse adjacent dims that are contiguous (and broadcast-consistent) across all args.
static ចំនួនគត់ collapseDims(MatStep* S, MatShape* H, ចំនួនគត់ K, MatShape& D)
{
    ចំនួនគត់ nd = (ចំនួនគត់)D.size();
    ប្រសិនបើ (nd <= 1) ត្រឡប់ nd;

    ចំនួនគត់ j = nd - 1;
    សម្រាប់ (ចំនួនគត់ i = j - 1; i >= 0; i--)
    {
        bool contig = true, scalar = true, consist = true;
        សម្រាប់ (ចំនួនគត់ k = 0; k < K; k++)
        {
            size_t st = S[k][j] * (size_t)H[k][j];
            bool prevScalar = H[k][j] == 1;
            bool curScalar = H[k][i] == 1;
            contig  = contig  && (st == S[k][i]);
            scalar  = scalar  && curScalar;
            consist = consist && (curScalar == prevScalar);
        }
        ប្រសិនបើ (contig && (consist || scalar))
        {
            សម្រាប់ (ចំនួនគត់ k = 0; k < K; k++) H[k][j] *= H[k][i];
            D[j] *= D[i];
        }
        បើមិនដូច្នេះ
        {
            j--;
            ប្រសិនបើ (i < j)
            {
                សម្រាប់ (ចំនួនគត់ k = 0; k < K; k++) { H[k][j] = H[k][i]; S[k][j] = S[k][i]; }
                D[j] = D[i];
            }
        }
    }

    ចំនួនគត់ m = nd - j;
    សម្រាប់ (ចំនួនគត់ d = 0; d < m; d++)
    {
        D[d] = D[j + d];
        សម្រាប់ (ចំនួនគត់ k = 0; k < K; k++) { S[k][d] = S[k][j + d]; H[k][d] = H[k][j + d]; }
    }
    D.resize(m);
    សម្រាប់ (ចំនួនគត់ k = 0; k < K; k++) H[k].resize(m);

    // Zero out steps of broadcast (size-1) dims (numpy step==0 trick).
    សម្រាប់ (ចំនួនគត់ d = 0; d < m; d++)
        សម្រាប់ (ចំនួនគត់ k = 0; k < K; k++)
            ប្រសិនបើ (H[k][d] == 1) S[k][d] = 0;

    ត្រឡប់ m;
}

// Fast geometry for the dominant case: every operand is either (a) an array sharing ONE common
// shape - same dims, sizes and channel count - and contiguous, or (b) a single-channel scalar
// (cn==1, total()==1). Then the whole traversal is a single contiguous 1D run of `total` scalars
// (channels folded in): arrays get stepx 1, scalars stepx 0. This skips decideChannelMode /
// broadcastShape / alignArg / collapseDims and all their per-operand buffers entirely. Returns
// false (leaving outputs untouched) when the operands don't fit, so the caller runs general
// geometry. expandChannels=false (CH_ELEM) keeps channels in the element and is left to general.
static bool fastSameShape(const Mat* const* arrays, ចំនួនគត់ K, bool expandChannels,
                          uchar** base, ចំនួនគត់* esz1, MatStep* S, MatShape& D, ចំនួនគត់& m)
{
    ប្រសិនបើ (!expandChannels) ត្រឡប់ false;
    ចំនួនគត់ ref = -1;
    សម្រាប់ (ចំនួនគត់ k = 0; k < K; k++)
        ប្រសិនបើ (!isSingleChannelScalar(*arrays[k])) { ref = k; bha_nga; }
    ប្រសិនបើ (ref < 0) ត្រឡប់ false;                       // all single-channel scalars: let general handle
    const Mat& R = *arrays[ref];
    const ចំនួនគត់ rdims = R.dims, rcn = R.channels();
    សម្រាប់ (ចំនួនគត់ k = 0; k < K; k++)
    {
        const Mat& a = *arrays[k];
        ប្រសិនបើ (isSingleChannelScalar(a)) continue;
        ប្រសិនបើ (a.channels() != rcn || a.dims != rdims || !a.isContinuous()) ត្រឡប់ false;
        សម្រាប់ (ចំនួនគត់ i = 0; i < rdims; i++) ប្រសិនបើ (a.size[i] != R.size[i]) ត្រឡប់ false;
    }
    const long long total = (long long)R.total() * rcn;   // channels folded into the 1D run
    CV_Assert(total <= (long long)INT_MAX);
    សម្រាប់ (ចំនួនគត់ k = 0; k < K; k++)
    {
        const Mat& a = *arrays[k];
        base[k] = (uchar*)a.data;
        esz1[k] = (ចំនួនគត់)a.elemSize1();
        S[k][0] = isSingleChannelScalar(a) ? 0 : 1;
    }
    D.assign(1, (ចំនួនគត់)total);
    m = 1;
    ត្រឡប់ true;
}

// ---------------------------------------------------------------------------
// BroadcastOp::run
// ---------------------------------------------------------------------------
// At namespace scope, NOT inside run(): MSVC 2019 loses the constexpr-ness of function-local
// constants used as template arguments inside a lambda (C2975).
static constexpr ចំនួនគត់ MAX_DIMS = MatShape::MAX_DIMS;
static constexpr ចំនួនគត់ LOCAL_OPS = 8;

void BroadcastOp::run(const Mat* const* arrays, ចំនួនគត់ narrays,
                      const std::function<void(const Tile&)>& body,
                      bool expandChannels,
                      dviguu_na nstripes)
{
    const ចំនួនគត់ K = narrays;
    CV_Assert(K >= 1 && arrays != nullptr);

    // ---- 1-3. geometry: per-operand collapsed steps S[k], element sizes esz1[k], base
    //           pointers, and the collapsed iteration shape D (m dims). The fast path handles the
    //           dominant "all same-shape arrays (+ single-channel scalars)" case in one shot; the
    //           general path does decideChannelMode + broadcastShape + align + collapse. ----
    AutoBuffer<MatStep, LOCAL_OPS>  S(K);
    AutoBuffer<ចំនួនគត់, LOCAL_OPS>      esz1(K);
    AutoBuffer<uchar*, LOCAL_OPS>   base(K);
    MatShape D;
    ចំនួនគត់ m;

    ប្រសិនបើ (!fastSameShape(arrays, K, expandChannels, base.data(), esz1.data(), S.data(), D, m))
    {
        const ChMode mode = expandChannels ? decideChannelMode(arrays, K) : CH_ELEM;
        AutoBuffer<MatShape, LOCAL_OPS> shp(K);
        AutoBuffer<MatStep, LOCAL_OPS>  stp(K);
        សម្រាប់ (ចំនួនគត់ k = 0; k < K; k++)
        {
            matLayout(*arrays[k], mode, shp[k], stp[k], esz1[k]);
            base[k] = (uchar*)arrays[k]->data;
        }
        MatShape full;
        CV_Assert(broadcastShape(shp.data(), K, full) && "ew: operands are not broadcast-compatible");
        const ចំនួនគត់ nd = (ចំនួនគត់)full.size();
        AutoBuffer<MatShape, LOCAL_OPS> H(K);
        សម្រាប់ (ចំនួនគត់ k = 0; k < K; k++) alignArg(shp[k], stp[k], nd, S[k], H[k]);
        D = full;
        m = collapseDims(S.data(), H.data(), K, D);

        // For a cv::Mat the innermost (channel/last) axis is contiguous, so after collapse the
        // innermost stride is always in {0,1}. No gather, no materialization.
        សម្រាប់ (ចំនួនគត់ k = 0; k < K; k++)
            CV_Assert(S[k][m - 1] <= 1 && "ew: unexpected innermost stride > 1");
    }

    // ---- 4. inner 2D tile axes: width = D[m-1], height = D[m-2] (if any) ----
    const ចំនួនគត់ wAxis = m - 1;
    const ចំនួនគត់ hAxis = (m >= 2) ? m - 2 : -1;
    const ចំនួនគត់ W   = D[wAxis];
    const ចំនួនគត់ Hgt = (hAxis >= 0) ? D[hAxis] : 1;
    const ចំនួនគត់ nOuter = (hAxis >= 0) ? m - 2 : m - 1;   // outer ("plane") axes = [0 .. nOuter)
    long long nplanes = 1;
    សម្រាប់ (ចំនួនគត់ d = 0; d < nOuter; d++) nplanes *= D[d];

    // ---- 5. desired parallel stripe count (work hint) ----
    const long long total = nplanes * (long long)Hgt * (long long)W;
    dviguu_na stripes = nstripes;
    ប្រសិនបើ (stripes <= 0)                                  // broadcastOp can't see the body's cost;
        stripes = (dviguu_na)total * 100.0 / (dviguu_na)(1 << 18);  // assume ~100 cycles/element
    const ចំនួនគត់ wantTiles = std::max(1, (ចំនួនគត់)std::lround(stripes));

    // ---- 6. tile only for PARALLELISM. broadcastOp is op-agnostic: it does not know the body's
    //         temp-buffer footprint, so it does NOT tile for L1 - that is the body's job (it
    //         re-fragments a tile's width into L1-sized chunks for the fused intermediates).
    //         Start with the largest tile (one 2D block per plane) and split (height first, then
    //         width) only until there are at least `wantTiles` tiles. Bigger tiles => fewer
    //         body/decode calls. Width is G-aligned only in the fully-contiguous (1D) case. ----
    const ចំនួនគត់ G = 16;                  // SIMD/cacheline granule
    ចំនួនគត់ tw = W, th = Hgt;

    auto ntilesOf = [&](ចំនួនគត់ tw_, ចំនួនគត់ th_) {
        long long nw = (W + tw_ - 1) / tw_, nh = (Hgt + th_ - 1) / th_;
        ត្រឡប់ nplanes * nh * nw;
    };
    long long ntiles = ntilesOf(tw, th);
    ខណៈពេល (ntiles < wantTiles && th > 1)               // split height for parallelism
    {
        th = (th + 1) / 2;
        ntiles = ntilesOf(tw, th);
    }
    ខណៈពេល (ntiles < wantTiles && tw > G)               // then split width
    {
        tw = std::max(G, tw / 2);
        ប្រសិនបើ (hAxis < 0 && tw > G) tw -= tw % G;          // keep width aligned in the 1D case
        ntiles = ntilesOf(tw, th);
    }
    CV_Assert(ntiles <= (long long)INT_MAX);

    const ចំនួនគត់ ntilesW = (W + tw - 1) / tw;
    const ចំនួនគត់ ntilesH = (Hgt + th - 1) / th;

    // ---- 7. execution; decode tile index -> per-operand slices. stepx/stepy are the same for
    //         every tile, so they are set ONCE; only the per-tile base pointer is recomputed. ----
    auto runRange = [&](const Range& r)
    {
        AutoBuffer<Slice, LOCAL_OPS> slices(K);

        // Fast 1D path (m==1: one contiguous axis after collapse, no outer planes, height 1).
        // ntilesH==1 and nplanes==1, so the tile index IS the width-tile index - no div/mod, no
        // plane multi-index decode, no inner step loop. This is the same-shape / fully-contiguous
        // common case.
        ប្រសិនបើ (m == 1)
        {
            សម្រាប់ (ចំនួនគត់ k = 0; k < K; k++) { slices[k].stepy = 0; slices[k].stepx = S[k][0]; }
            Tile tile;
            tile.height = 1; tile.narrays = K; tile.slices = slices.data();
            សម្រាប់ (ចំនួនគត់ t = r.start; t < r.end; t++)
            {
                const ចំនួនគត់ wofs = t * tw, ww = std::min(tw, W - wofs);
                សម្រាប់ (ចំនួនគត់ k = 0; k < K; k++)
                    slices[k].ptr = base[k] + (size_t)wofs * S[k][0] * (size_t)esz1[k];
                tile.width = ww;
                body(tile);
            }
            ត្រឡប់;
        }

        std::array<ចំនួនគត់, MAX_DIMS> idx;
        សម្រាប់ (ចំនួនគត់ k = 0; k < K; k++)                     // steps are tile-independent: set once
        {
            slices[k].stepy = (hAxis >= 0) ? S[k][hAxis] : 0;
            slices[k].stepx = S[k][wAxis];
        }
        សម្រាប់ (ចំនួនគត់ t = r.start; t < r.end; t++)
        {
            ចំនួនគត់ wt = t % ntilesW;
            ចំនួនគត់ rest = t / ntilesW;
            ចំនួនគត់ ht = rest % ntilesH;
            ចំនួនគត់ plane = rest / ntilesH;

            const ចំនួនគត់ wofs = wt * tw, ww = std::min(tw, W - wofs);
            const ចំនួនគត់ hofs = ht * th, hh = std::min(th, Hgt - hofs);

            ចំនួនគត់ p = plane;                              // decode plane -> outer multi-index
            សម្រាប់ (ចំនួនគត់ d = nOuter - 1; d >= 0; d--) {
                ចំនួនគត់ dd = D[d];
                ចំនួនគត់ np = p / dd;
                idx[d] = p - np * dd;
                p = np;
            }

            សម្រាប់ (ចំនួនគត់ k = 0; k < K; k++)
            {
                size_t off = (size_t)wofs * S[k][wAxis];
                សម្រាប់ (ចំនួនគត់ d = 0; d < nOuter; d++) off += (size_t)idx[d] * S[k][d];
                ប្រសិនបើ (hAxis >= 0) off += (size_t)hofs * S[k][hAxis];
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
    ប្រសិនបើ (ntiles == 1)
        runRange(Range(0, 1));
    បើមិនដូច្នេះ
        parallel_for_(Range(0, (ចំនួនគត់)ntiles), runRange, stripes);
}

} // namespace cv
