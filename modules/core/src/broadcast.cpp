// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

// Implementation of cv::BroadcastOp (declared in opencv2/core/mat.hpp): an op-agnostic driver for
// broadcasting element-wise traversal. It computes the numpy-broadcast iteration space over a flat
// list of operand Mats, collapses contiguous dims, partitions into tiles for parallel_for_, and hands
// each 2D tile's per-operand slices to a `body` callback (which owns all semantics).

#ಅನ್ತರ್ಭಾವ "precomp.hpp"
#ಅನ್ತರ್ಭಾವ <algorithm>
#ಅನ್ತರ್ಭಾವ <array>
#ಅನ್ತರ್ಭಾವ <climits>
#ಅನ್ತರ್ಭಾವ <cmath>

ನಾಮಾಕಾಶ cv {

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
ಗಣನಾ ChMode { CH_FOLD, CH_DIM, CH_ELEM };

ಸ್ಥಿರ ಶೂನ್ಯ matLayout(ಧ್ರುವ Mat& m, ChMode mode, MatShape& shp, MatStep& step, ಪೂರ್ಣಾಙ್ಕ& esz1)
{
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ nd = m.dims, cn = m.channels();
    ಯದಿ (mode == CH_ELEM)
    {
        esz1 = (ಪೂರ್ಣಾಙ್ಕ)m.elemSize();                       // one full (cn-channel) pixel
        shp.resize(nd);
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < nd; i++) { shp[i] = m.size[i]; step[i] = m.step[i] / esz1; }
        ಪ್ರತ್ಯಾಗಮ;
    }
    esz1 = (ಪೂರ್ಣಾಙ್ಕ)m.elemSize1();                           // one scalar (channel value)
    ಯದಿ (mode == CH_DIM)
    {
        shp.resize(nd + 1);
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < nd; i++) { shp[i] = m.size[i]; step[i] = m.step[i] / esz1; }
        shp[nd] = cn; step[nd] = 1;                      // channels = explicit innermost dim
    }
    ಅನ್ಯಥಾ // CH_FOLD
    {
        ಯದಿ (nd == 0)                                     // 0-dim scalar: channels are the only dim
        {
            shp.assign(1, cn);
            step[0] = 1;
        }
        ಅನ್ಯಥಾ
        {
            shp.resize(nd);
            ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < nd; i++) { shp[i] = m.size[i]; step[i] = m.step[i] / esz1; }
            shp[nd - 1] *= cn;                           // fold channels into the innermost dim
            step[nd - 1] = 1;                            // scalars are contiguous there
        }
    }
}

// A single-channel scalar (one value, cn==1, total()==1) broadcasts into everything trivially
// (step 0 on every axis incl. channels), so it must NOT force CH_DIM - it is excluded from the
// channel-mode decision entirely.
ಸ್ಥಿರ ತಾರ್ಕಿಕ isSingleChannelScalar(ಧ್ರುವ Mat& m)
{
    ಪ್ರತ್ಯಾಗಮ m.channels() == 1 && m.total() == 1;
}

// Decide (globally, across all operands) how channels are presented for expandChannels=true:
// CH_FOLD when no channel broadcast is needed, CH_DIM when it is. Single-channel scalars are
// excluded first; among the rest, multichannel operands must all share the same cn (an (n,m) mix
// with both > 1 is an error).
ಸ್ಥಿರ ChMode decideChannelMode(ಧ್ರುವ Mat* ಧ್ರುವ* arrays, ಪೂರ್ಣಾಙ್ಕ K)
{
    ಪೂರ್ಣಾಙ್ಕ N = 1;                                           // the single multichannel count, if any
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ k = 0; k < K; k++)
    {
        ಯದಿ (isSingleChannelScalar(*arrays[k])) ಅನುವರ್ತ;
        ಪೂರ್ಣಾಙ್ಕ c = arrays[k]->channels();
        ಯದಿ (c > 1) { ಯದಿ (N == 1) N = c; ಅನ್ಯಥಾ CV_Assert(N == c && "ew: (n,m) channel mix unsupported"); }
    }
    ಯದಿ (N == 1) ಪ್ರತ್ಯಾಗಮ CH_FOLD;                          // all single-channel -> fold (a no-op)

    ತಾರ್ಕಿಕ allMulti = ಸತ್ಯ, sameBack = ಸತ್ಯ;
    ಪೂರ್ಣಾಙ್ಕ back = -1;
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ k = 0; k < K; k++)
    {
        ಧ್ರುವ Mat& a = *arrays[k];
        ಯದಿ (isSingleChannelScalar(a)) ಅನುವರ್ತ;
        ಯದಿ (a.channels() != N) allMulti = ಅಸತ್ಯ;
        ಪೂರ್ಣಾಙ್ಕ b = a.dims > 0 ? a.size[a.dims - 1] : 1;   // 0-dim scalar has no spatial back (=1)
        ಯದಿ (back < 0) back = b; ಅನ್ಯಥಾ ಯದಿ (b != back) sameBack = ಅಸತ್ಯ;
    }
    ಪ್ರತ್ಯಾಗಮ (allMulti && sameBack) ? CH_FOLD : CH_DIM;    // fold only if no channel broadcast
}

// numpy-style broadcast of several right-aligned shapes.
ಸ್ಥಿರ ತಾರ್ಕಿಕ broadcastShape(ಧ್ರುವ MatShape* shps, ಪೂರ್ಣಾಙ್ಕ K, MatShape& out)
{
    size_t nd = 0;
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ k = 0; k < K; k++) nd = std::max(nd, shps[k].size());
    out.assign(nd, 1);
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ k = 0; k < K; k++)
    {
        ಧ್ರುವ MatShape& s = shps[k];
        size_t off = nd - s.size();
        ಪ್ರತಿ (size_t i = 0; i < s.size(); i++)
        {
            ಪೂರ್ಣಾಙ್ಕ d = s[i], &o = out[off + i];
            ಯದಿ (o == 1) o = d;
            ಅನ್ಯಥಾ ಯದಿ (d != 1 && d != o) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
        }
    }
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
}

// Right-align an arg's own (shp,step) to nd dims; broadcast dims get step 0.
ಸ್ಥಿರ ಶೂನ್ಯ alignArg(ಧ್ರುವ MatShape& shp, ಧ್ರುವ MatStep& step, ಪೂರ್ಣಾಙ್ಕ nd,
                     MatStep& as, MatShape& ash)
{
    as.clear();
    ash.assign(nd, 1);
    ಪೂರ್ಣಾಙ್ಕ off = nd - (ಪೂರ್ಣಾಙ್ಕ)shp.size();
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < (ಪೂರ್ಣಾಙ್ಕ)shp.size(); i++)
    {
        ಪೂರ್ಣಾಙ್ಕ d = shp[i];
        ash[off + i] = d;
        as[off + i] = (d == 1) ? 0 : step[i];
    }
}

// Collapse adjacent dims that are contiguous (and broadcast-consistent) across all args.
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ collapseDims(MatStep* S, MatShape* H, ಪೂರ್ಣಾಙ್ಕ K, MatShape& D)
{
    ಪೂರ್ಣಾಙ್ಕ nd = (ಪೂರ್ಣಾಙ್ಕ)D.size();
    ಯದಿ (nd <= 1) ಪ್ರತ್ಯಾಗಮ nd;

    ಪೂರ್ಣಾಙ್ಕ j = nd - 1;
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = j - 1; i >= 0; i--)
    {
        ತಾರ್ಕಿಕ contig = ಸತ್ಯ, scalar = ಸತ್ಯ, consist = ಸತ್ಯ;
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ k = 0; k < K; k++)
        {
            size_t st = S[k][j] * (size_t)H[k][j];
            ತಾರ್ಕಿಕ prevScalar = H[k][j] == 1;
            ತಾರ್ಕಿಕ curScalar = H[k][i] == 1;
            contig  = contig  && (st == S[k][i]);
            scalar  = scalar  && curScalar;
            consist = consist && (curScalar == prevScalar);
        }
        ಯದಿ (contig && (consist || scalar))
        {
            ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ k = 0; k < K; k++) H[k][j] *= H[k][i];
            D[j] *= D[i];
        }
        ಅನ್ಯಥಾ
        {
            j--;
            ಯದಿ (i < j)
            {
                ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ k = 0; k < K; k++) { H[k][j] = H[k][i]; S[k][j] = S[k][i]; }
                D[j] = D[i];
            }
        }
    }

    ಪೂರ್ಣಾಙ್ಕ m = nd - j;
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ d = 0; d < m; d++)
    {
        D[d] = D[j + d];
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ k = 0; k < K; k++) { S[k][d] = S[k][j + d]; H[k][d] = H[k][j + d]; }
    }
    D.resize(m);
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ k = 0; k < K; k++) H[k].resize(m);

    // Zero out steps of broadcast (size-1) dims (numpy step==0 trick).
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ d = 0; d < m; d++)
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ k = 0; k < K; k++)
            ಯದಿ (H[k][d] == 1) S[k][d] = 0;

    ಪ್ರತ್ಯಾಗಮ m;
}

// Fast geometry for the dominant case: every operand is either (a) an array sharing ONE common
// shape - same dims, sizes and channel count - and contiguous, or (b) a single-channel scalar
// (cn==1, total()==1). Then the whole traversal is a single contiguous 1D run of `total` scalars
// (channels folded in): arrays get stepx 1, scalars stepx 0. This skips decideChannelMode /
// broadcastShape / alignArg / collapseDims and all their per-operand buffers entirely. Returns
// false (leaving outputs untouched) when the operands don't fit, so the caller runs general
// geometry. expandChannels=false (CH_ELEM) keeps channels in the element and is left to general.
ಸ್ಥಿರ ತಾರ್ಕಿಕ fastSameShape(ಧ್ರುವ Mat* ಧ್ರುವ* arrays, ಪೂರ್ಣಾಙ್ಕ K, ತಾರ್ಕಿಕ expandChannels,
                          uchar** base, ಪೂರ್ಣಾಙ್ಕ* esz1, MatStep* S, MatShape& D, ಪೂರ್ಣಾಙ್ಕ& m)
{
    ಯದಿ (!expandChannels) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    ಪೂರ್ಣಾಙ್ಕ ref = -1;
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ k = 0; k < K; k++)
        ಯದಿ (!isSingleChannelScalar(*arrays[k])) { ref = k; ಭಙ್ಗ; }
    ಯದಿ (ref < 0) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;                       // all single-channel scalars: let general handle
    ಧ್ರುವ Mat& R = *arrays[ref];
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ rdims = R.dims, rcn = R.channels();
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ k = 0; k < K; k++)
    {
        ಧ್ರುವ Mat& a = *arrays[k];
        ಯದಿ (isSingleChannelScalar(a)) ಅನುವರ್ತ;
        ಯದಿ (a.channels() != rcn || a.dims != rdims || !a.isContinuous()) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < rdims; i++) ಯದಿ (a.size[i] != R.size[i]) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }
    ಧ್ರುವ long long total = (long long)R.total() * rcn;   // channels folded into the 1D run
    CV_Assert(total <= (long long)INT_MAX);
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ k = 0; k < K; k++)
    {
        ಧ್ರುವ Mat& a = *arrays[k];
        base[k] = (uchar*)a.data;
        esz1[k] = (ಪೂರ್ಣಾಙ್ಕ)a.elemSize1();
        S[k][0] = isSingleChannelScalar(a) ? 0 : 1;
    }
    D.assign(1, (ಪೂರ್ಣಾಙ್ಕ)total);
    m = 1;
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
}

// ---------------------------------------------------------------------------
// BroadcastOp::run
// ---------------------------------------------------------------------------
// At namespace scope, NOT inside run(): MSVC 2019 loses the constexpr-ness of function-local
// constants used as template arguments inside a lambda (C2975).
ಸ್ಥಿರ constexpr ಪೂರ್ಣಾಙ್ಕ MAX_DIMS = MatShape::MAX_DIMS;
ಸ್ಥಿರ constexpr ಪೂರ್ಣಾಙ್ಕ LOCAL_OPS = 8;

ಶೂನ್ಯ BroadcastOp::run(ಧ್ರುವ Mat* ಧ್ರುವ* arrays, ಪೂರ್ಣಾಙ್ಕ narrays,
                      ಧ್ರುವ std::function<ಶೂನ್ಯ(ಧ್ರುವ Tile&)>& body,
                      ತಾರ್ಕಿಕ expandChannels,
                      ದ್ವಿಗುಣ nstripes)
{
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ K = narrays;
    CV_Assert(K >= 1 && arrays != nullptr);

    // ---- 1-3. geometry: per-operand collapsed steps S[k], element sizes esz1[k], base
    //           pointers, and the collapsed iteration shape D (m dims). The fast path handles the
    //           dominant "all same-shape arrays (+ single-channel scalars)" case in one shot; the
    //           general path does decideChannelMode + broadcastShape + align + collapse. ----
    AutoBuffer<MatStep, LOCAL_OPS>  S(K);
    AutoBuffer<ಪೂರ್ಣಾಙ್ಕ, LOCAL_OPS>      esz1(K);
    AutoBuffer<uchar*, LOCAL_OPS>   base(K);
    MatShape D;
    ಪೂರ್ಣಾಙ್ಕ m;

    ಯದಿ (!fastSameShape(arrays, K, expandChannels, base.data(), esz1.data(), S.data(), D, m))
    {
        ಧ್ರುವ ChMode mode = expandChannels ? decideChannelMode(arrays, K) : CH_ELEM;
        AutoBuffer<MatShape, LOCAL_OPS> shp(K);
        AutoBuffer<MatStep, LOCAL_OPS>  stp(K);
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ k = 0; k < K; k++)
        {
            matLayout(*arrays[k], mode, shp[k], stp[k], esz1[k]);
            base[k] = (uchar*)arrays[k]->data;
        }
        MatShape full;
        CV_Assert(broadcastShape(shp.data(), K, full) && "ew: operands are not broadcast-compatible");
        ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ nd = (ಪೂರ್ಣಾಙ್ಕ)full.size();
        AutoBuffer<MatShape, LOCAL_OPS> H(K);
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ k = 0; k < K; k++) alignArg(shp[k], stp[k], nd, S[k], H[k]);
        D = full;
        m = collapseDims(S.data(), H.data(), K, D);

        // For a cv::Mat the innermost (channel/last) axis is contiguous, so after collapse the
        // innermost stride is always in {0,1}. No gather, no materialization.
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ k = 0; k < K; k++)
            CV_Assert(S[k][m - 1] <= 1 && "ew: unexpected innermost stride > 1");
    }

    // ---- 4. inner 2D tile axes: width = D[m-1], height = D[m-2] (if any) ----
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ wAxis = m - 1;
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ hAxis = (m >= 2) ? m - 2 : -1;
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ W   = D[wAxis];
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ Hgt = (hAxis >= 0) ? D[hAxis] : 1;
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ nOuter = (hAxis >= 0) ? m - 2 : m - 1;   // outer ("plane") axes = [0 .. nOuter)
    long long nplanes = 1;
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ d = 0; d < nOuter; d++) nplanes *= D[d];

    // ---- 5. desired parallel stripe count (work hint) ----
    ಧ್ರುವ long long total = nplanes * (long long)Hgt * (long long)W;
    ದ್ವಿಗುಣ stripes = nstripes;
    ಯದಿ (stripes <= 0)                                  // broadcastOp can't see the body's cost;
        stripes = (ದ್ವಿಗುಣ)total * 100.0 / (ದ್ವಿಗುಣ)(1 << 18);  // assume ~100 cycles/element
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ wantTiles = std::max(1, (ಪೂರ್ಣಾಙ್ಕ)std::lround(stripes));

    // ---- 6. tile only for PARALLELISM. broadcastOp is op-agnostic: it does not know the body's
    //         temp-buffer footprint, so it does NOT tile for L1 - that is the body's job (it
    //         re-fragments a tile's width into L1-sized chunks for the fused intermediates).
    //         Start with the largest tile (one 2D block per plane) and split (height first, then
    //         width) only until there are at least `wantTiles` tiles. Bigger tiles => fewer
    //         body/decode calls. Width is G-aligned only in the fully-contiguous (1D) case. ----
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ G = 16;                  // SIMD/cacheline granule
    ಪೂರ್ಣಾಙ್ಕ tw = W, th = Hgt;

    auto ntilesOf = [&](ಪೂರ್ಣಾಙ್ಕ tw_, ಪೂರ್ಣಾಙ್ಕ th_) {
        long long nw = (W + tw_ - 1) / tw_, nh = (Hgt + th_ - 1) / th_;
        ಪ್ರತ್ಯಾಗಮ nplanes * nh * nw;
    };
    long long ntiles = ntilesOf(tw, th);
    ಯಾವತ್ (ntiles < wantTiles && th > 1)               // split height for parallelism
    {
        th = (th + 1) / 2;
        ntiles = ntilesOf(tw, th);
    }
    ಯಾವತ್ (ntiles < wantTiles && tw > G)               // then split width
    {
        tw = std::max(G, tw / 2);
        ಯದಿ (hAxis < 0 && tw > G) tw -= tw % G;          // keep width aligned in the 1D case
        ntiles = ntilesOf(tw, th);
    }
    CV_Assert(ntiles <= (long long)INT_MAX);

    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ ntilesW = (W + tw - 1) / tw;
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ ntilesH = (Hgt + th - 1) / th;

    // ---- 7. execution; decode tile index -> per-operand slices. stepx/stepy are the same for
    //         every tile, so they are set ONCE; only the per-tile base pointer is recomputed. ----
    auto runRange = [&](ಧ್ರುವ Range& r)
    {
        AutoBuffer<Slice, LOCAL_OPS> slices(K);

        // Fast 1D path (m==1: one contiguous axis after collapse, no outer planes, height 1).
        // ntilesH==1 and nplanes==1, so the tile index IS the width-tile index - no div/mod, no
        // plane multi-index decode, no inner step loop. This is the same-shape / fully-contiguous
        // common case.
        ಯದಿ (m == 1)
        {
            ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ k = 0; k < K; k++) { slices[k].stepy = 0; slices[k].stepx = S[k][0]; }
            Tile tile;
            tile.height = 1; tile.narrays = K; tile.slices = slices.data();
            ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ t = r.start; t < r.end; t++)
            {
                ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ wofs = t * tw, ww = std::min(tw, W - wofs);
                ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ k = 0; k < K; k++)
                    slices[k].ptr = base[k] + (size_t)wofs * S[k][0] * (size_t)esz1[k];
                tile.width = ww;
                body(tile);
            }
            ಪ್ರತ್ಯಾಗಮ;
        }

        std::array<ಪೂರ್ಣಾಙ್ಕ, MAX_DIMS> idx;
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ k = 0; k < K; k++)                     // steps are tile-independent: set once
        {
            slices[k].stepy = (hAxis >= 0) ? S[k][hAxis] : 0;
            slices[k].stepx = S[k][wAxis];
        }
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ t = r.start; t < r.end; t++)
        {
            ಪೂರ್ಣಾಙ್ಕ wt = t % ntilesW;
            ಪೂರ್ಣಾಙ್ಕ rest = t / ntilesW;
            ಪೂರ್ಣಾಙ್ಕ ht = rest % ntilesH;
            ಪೂರ್ಣಾಙ್ಕ plane = rest / ntilesH;

            ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ wofs = wt * tw, ww = std::min(tw, W - wofs);
            ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ hofs = ht * th, hh = std::min(th, Hgt - hofs);

            ಪೂರ್ಣಾಙ್ಕ p = plane;                              // decode plane -> outer multi-index
            ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ d = nOuter - 1; d >= 0; d--) {
                ಪೂರ್ಣಾಙ್ಕ dd = D[d];
                ಪೂರ್ಣಾಙ್ಕ np = p / dd;
                idx[d] = p - np * dd;
                p = np;
            }

            ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ k = 0; k < K; k++)
            {
                size_t off = (size_t)wofs * S[k][wAxis];
                ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ d = 0; d < nOuter; d++) off += (size_t)idx[d] * S[k][d];
                ಯದಿ (hAxis >= 0) off += (size_t)hofs * S[k][hAxis];
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
    ಯದಿ (ntiles == 1)
        runRange(Range(0, 1));
    ಅನ್ಯಥಾ
        parallel_for_(Range(0, (ಪೂರ್ಣಾಙ್ಕ)ntiles), runRange, stripes);
}

} // namespace cv
