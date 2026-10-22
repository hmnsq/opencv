// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

// Implementation of cv::BroadcastOp (declared in opencv2/core/mat.hpp): an op-agnostic driver for
// broadcasting element-wise traversal. It computes the numpy-broadcast iteration space over a flat
// list of operand Mats, collapses contiguous dims, partitions into tiles for parallel_for_, and hands
// each 2D tile's per-operand slices to a `body` callback (which owns all semantics).

#ਅਨ੍ਤਰ੍ਭਾਵ "precomp.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ <algorithm>
#ਅਨ੍ਤਰ੍ਭਾਵ <array>
#ਅਨ੍ਤਰ੍ਭਾਵ <climits>
#ਅਨ੍ਤਰ੍ਭਾਵ <cmath>

ਨਾਮਾਕਾਸ਼ cv {

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
ਗਿਣਤੀ Ch_ਬਹੁਲਕ { CH_FOLD, CH_DIM, CH_ELEM };

ਸਥਿਰ ਖਾਲੀ matLayout(ਅਚਲ Mat& m, Ch_ਬਹੁਲਕ ਬਹੁਲਕ, MatShape& shp, Mat_ਕਦਮ& ਕਦਮ, ਪੂਰਨ& esz1)
{
    ਅਚਲ ਪੂਰਨ nd = m.dims, cn = m.channels();
    ਜੇ (ਬਹੁਲਕ == CH_ELEM)
    {
        esz1 = (ਪੂਰਨ)m.elem_ਮਾਪ();                       // one full (cn-channel) pixel
        shp.resize(nd);
        ਲਈ (ਪੂਰਨ i = 0; i < nd; i++) { shp[i] = m.size[i]; ਕਦਮ[i] = m.ਕਦਮ[i] / esz1; }
        ਵਾਪਸ;
    }
    esz1 = (ਪੂਰਨ)m.elemSize1();                           // one scalar (channel value)
    ਜੇ (ਬਹੁਲਕ == CH_DIM)
    {
        shp.resize(nd + 1);
        ਲਈ (ਪੂਰਨ i = 0; i < nd; i++) { shp[i] = m.size[i]; ਕਦਮ[i] = m.ਕਦਮ[i] / esz1; }
        shp[nd] = cn; ਕਦਮ[nd] = 1;                      // channels = explicit innermost dim
    }
    ਨਹੀਂਤਾਂ // CH_FOLD
    {
        ਜੇ (nd == 0)                                     // 0-dim scalar: channels are the only dim
        {
            shp.assign(1, cn);
            ਕਦਮ[0] = 1;
        }
        ਨਹੀਂਤਾਂ
        {
            shp.resize(nd);
            ਲਈ (ਪੂਰਨ i = 0; i < nd; i++) { shp[i] = m.size[i]; ਕਦਮ[i] = m.ਕਦਮ[i] / esz1; }
            shp[nd - 1] *= cn;                           // fold channels into the innermost dim
            ਕਦਮ[nd - 1] = 1;                            // scalars are contiguous there
        }
    }
}

// A single-channel scalar (one value, cn==1, total()==1) broadcasts into everything trivially
// (step 0 on every axis incl. channels), so it must NOT force CH_DIM - it is excluded from the
// channel-mode decision entirely.
ਸਥਿਰ ਤਾਰਕਿਕ isSingleChannelScalar(ਅਚਲ Mat& m)
{
    ਵਾਪਸ m.channels() == 1 && m.ਕੁੱਲ() == 1;
}

// Decide (globally, across all operands) how channels are presented for expandChannels=true:
// CH_FOLD when no channel broadcast is needed, CH_DIM when it is. Single-channel scalars are
// excluded first; among the rest, multichannel operands must all share the same cn (an (n,m) mix
// with both > 1 is an error).
ਸਥਿਰ Ch_ਬਹੁਲਕ decide_Channel_ਬਹੁਲਕ(ਅਚਲ Mat* ਅਚਲ* arrays, ਪੂਰਨ K)
{
    ਪੂਰਨ N = 1;                                           // the single multichannel count, if any
    ਲਈ (ਪੂਰਨ k = 0; k < K; k++)
    {
        ਜੇ (isSingleChannelScalar(*arrays[k])) ਜਾਰੀ;
        ਪੂਰਨ c = arrays[k]->channels();
        ਜੇ (c > 1) { ਜੇ (N == 1) N = c; ਨਹੀਂਤਾਂ CV_Assert(N == c && "ew: (n,m) channel mix unsupported"); }
    }
    ਜੇ (N == 1) ਵਾਪਸ CH_FOLD;                          // all single-channel -> fold (a no-op)

    ਤਾਰਕਿਕ allMulti = ਸੱਚ, same_ਪਿੱਠ = ਸੱਚ;
    ਪੂਰਨ back = -1;
    ਲਈ (ਪੂਰਨ k = 0; k < K; k++)
    {
        ਅਚਲ Mat& a = *arrays[k];
        ਜੇ (isSingleChannelScalar(a)) ਜਾਰੀ;
        ਜੇ (a.channels() != N) allMulti = ਝੂਠ;
        ਪੂਰਨ b = a.dims > 0 ? a.size[a.dims - 1] : 1;   // 0-dim scalar has no spatial back (=1)
        ਜੇ (back < 0) back = b; ਨਹੀਂਤਾਂ ਜੇ (b != back) same_ਪਿੱਠ = ਝੂਠ;
    }
    ਵਾਪਸ (allMulti && same_ਪਿੱਠ) ? CH_FOLD : CH_DIM;    // fold only if no channel broadcast
}

// numpy-style broadcast of several right-aligned shapes.
ਸਥਿਰ ਤਾਰਕਿਕ ਪ੍ਰਸਾਰਣ_Shape(ਅਚਲ MatShape* shps, ਪੂਰਨ K, MatShape& out)
{
    size_t nd = 0;
    ਲਈ (ਪੂਰਨ k = 0; k < K; k++) nd = std::max(nd, shps[k].size());
    out.assign(nd, 1);
    ਲਈ (ਪੂਰਨ k = 0; k < K; k++)
    {
        ਅਚਲ MatShape& s = shps[k];
        size_t off = nd - s.size();
        ਲਈ (size_t i = 0; i < s.size(); i++)
        {
            ਪੂਰਨ d = s[i], &o = out[off + i];
            ਜੇ (o == 1) o = d;
            ਨਹੀਂਤਾਂ ਜੇ (d != 1 && d != o) ਵਾਪਸ ਝੂਠ;
        }
    }
    ਵਾਪਸ ਸੱਚ;
}

// Right-align an arg's own (shp,step) to nd dims; broadcast dims get step 0.
ਸਥਿਰ ਖਾਲੀ alignArg(ਅਚਲ MatShape& shp, ਅਚਲ Mat_ਕਦਮ& ਕਦਮ, ਪੂਰਨ nd,
                     Mat_ਕਦਮ& as, MatShape& ash)
{
    as.clear();
    ash.assign(nd, 1);
    ਪੂਰਨ off = nd - (ਪੂਰਨ)shp.size();
    ਲਈ (ਪੂਰਨ i = 0; i < (ਪੂਰਨ)shp.size(); i++)
    {
        ਪੂਰਨ d = shp[i];
        ash[off + i] = d;
        as[off + i] = (d == 1) ? 0 : ਕਦਮ[i];
    }
}

// Collapse adjacent dims that are contiguous (and broadcast-consistent) across all args.
ਸਥਿਰ ਪੂਰਨ collapseDims(Mat_ਕਦਮ* S, MatShape* H, ਪੂਰਨ K, MatShape& D)
{
    ਪੂਰਨ nd = (ਪੂਰਨ)D.size();
    ਜੇ (nd <= 1) ਵਾਪਸ nd;

    ਪੂਰਨ j = nd - 1;
    ਲਈ (ਪੂਰਨ i = j - 1; i >= 0; i--)
    {
        ਤਾਰਕਿਕ contig = ਸੱਚ, scalar = ਸੱਚ, consist = ਸੱਚ;
        ਲਈ (ਪੂਰਨ k = 0; k < K; k++)
        {
            size_t st = S[k][j] * (size_t)H[k][j];
            ਤਾਰਕਿਕ prevScalar = H[k][j] == 1;
            ਤਾਰਕਿਕ curScalar = H[k][i] == 1;
            contig  = contig  && (st == S[k][i]);
            scalar  = scalar  && curScalar;
            consist = consist && (curScalar == prevScalar);
        }
        ਜੇ (contig && (consist || scalar))
        {
            ਲਈ (ਪੂਰਨ k = 0; k < K; k++) H[k][j] *= H[k][i];
            D[j] *= D[i];
        }
        ਨਹੀਂਤਾਂ
        {
            j--;
            ਜੇ (i < j)
            {
                ਲਈ (ਪੂਰਨ k = 0; k < K; k++) { H[k][j] = H[k][i]; S[k][j] = S[k][i]; }
                D[j] = D[i];
            }
        }
    }

    ਪੂਰਨ m = nd - j;
    ਲਈ (ਪੂਰਨ d = 0; d < m; d++)
    {
        D[d] = D[j + d];
        ਲਈ (ਪੂਰਨ k = 0; k < K; k++) { S[k][d] = S[k][j + d]; H[k][d] = H[k][j + d]; }
    }
    D.resize(m);
    ਲਈ (ਪੂਰਨ k = 0; k < K; k++) H[k].resize(m);

    // Zero out steps of broadcast (size-1) dims (numpy step==0 trick).
    ਲਈ (ਪੂਰਨ d = 0; d < m; d++)
        ਲਈ (ਪੂਰਨ k = 0; k < K; k++)
            ਜੇ (H[k][d] == 1) S[k][d] = 0;

    ਵਾਪਸ m;
}

// Fast geometry for the dominant case: every operand is either (a) an array sharing ONE common
// shape - same dims, sizes and channel count - and contiguous, or (b) a single-channel scalar
// (cn==1, total()==1). Then the whole traversal is a single contiguous 1D run of `total` scalars
// (channels folded in): arrays get stepx 1, scalars stepx 0. This skips decideChannelMode /
// broadcastShape / alignArg / collapseDims and all their per-operand buffers entirely. Returns
// false (leaving outputs untouched) when the operands don't fit, so the caller runs general
// geometry. expandChannels=false (CH_ELEM) keeps channels in the element and is left to general.
ਸਥਿਰ ਤਾਰਕਿਕ fastSameShape(ਅਚਲ Mat* ਅਚਲ* arrays, ਪੂਰਨ K, ਤਾਰਕਿਕ expandChannels,
                          uchar** ਅਧਾਰ, ਪੂਰਨ* esz1, Mat_ਕਦਮ* S, MatShape& D, ਪੂਰਨ& m)
{
    ਜੇ (!expandChannels) ਵਾਪਸ ਝੂਠ;
    ਪੂਰਨ ref = -1;
    ਲਈ (ਪੂਰਨ k = 0; k < K; k++)
        ਜੇ (!isSingleChannelScalar(*arrays[k])) { ref = k; ਰੋਕੋ; }
    ਜੇ (ref < 0) ਵਾਪਸ ਝੂਠ;                       // all single-channel scalars: let general handle
    ਅਚਲ Mat& R = *arrays[ref];
    ਅਚਲ ਪੂਰਨ rdims = R.dims, rcn = R.channels();
    ਲਈ (ਪੂਰਨ k = 0; k < K; k++)
    {
        ਅਚਲ Mat& a = *arrays[k];
        ਜੇ (isSingleChannelScalar(a)) ਜਾਰੀ;
        ਜੇ (a.channels() != rcn || a.dims != rdims || !a.isContinuous()) ਵਾਪਸ ਝੂਠ;
        ਲਈ (ਪੂਰਨ i = 0; i < rdims; i++) ਜੇ (a.size[i] != R.size[i]) ਵਾਪਸ ਝੂਠ;
    }
    ਅਚਲ ਲੰਮਾ ਲੰਮਾ ਕੁੱਲ = (ਲੰਮਾ ਲੰਮਾ)R.ਕੁੱਲ() * rcn;   // channels folded into the 1D run
    CV_Assert(ਕੁੱਲ <= (ਲੰਮਾ ਲੰਮਾ)ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ);
    ਲਈ (ਪੂਰਨ k = 0; k < K; k++)
    {
        ਅਚਲ Mat& a = *arrays[k];
        ਅਧਾਰ[k] = (uchar*)a.ਅੰਕੜੇ;
        esz1[k] = (ਪੂਰਨ)a.elemSize1();
        S[k][0] = isSingleChannelScalar(a) ? 0 : 1;
    }
    D.assign(1, (ਪੂਰਨ)ਕੁੱਲ);
    m = 1;
    ਵਾਪਸ ਸੱਚ;
}

// ---------------------------------------------------------------------------
// BroadcastOp::run
// ---------------------------------------------------------------------------
// At namespace scope, NOT inside run(): MSVC 2019 loses the constexpr-ness of function-local
// constants used as template arguments inside a lambda (C2975).
ਸਥਿਰ constexpr ਪੂਰਨ MAX_DIMS = MatShape::MAX_DIMS;
ਸਥਿਰ constexpr ਪੂਰਨ LOCAL_OPS = 8;

ਖਾਲੀ ਪ੍ਰਸਾਰਣ_Op::ਲੜੀਵਾਰ(ਅਚਲ Mat* ਅਚਲ* arrays, ਪੂਰਨ narrays,
                      ਅਚਲ std::function<ਖਾਲੀ(ਅਚਲ Tile&)>& body,
                      ਤਾਰਕਿਕ expandChannels,
                      ਦੁੱਗਣਾ nstripes)
{
    ਅਚਲ ਪੂਰਨ K = narrays;
    CV_Assert(K >= 1 && arrays != ਖਾਲੀ_ਸੂਚਕ);

    // ---- 1-3. geometry: per-operand collapsed steps S[k], element sizes esz1[k], base
    //           pointers, and the collapsed iteration shape D (m dims). The fast path handles the
    //           dominant "all same-shape arrays (+ single-channel scalars)" case in one shot; the
    //           general path does decideChannelMode + broadcastShape + align + collapse. ----
    Auto_ਬਫਰ<Mat_ਕਦਮ, LOCAL_OPS>  S(K);
    Auto_ਬਫਰ<ਪੂਰਨ, LOCAL_OPS>      esz1(K);
    Auto_ਬਫਰ<uchar*, LOCAL_OPS>   ਅਧਾਰ(K);
    MatShape D;
    ਪੂਰਨ m;

    ਜੇ (!fastSameShape(arrays, K, expandChannels, ਅਧਾਰ.ਅੰਕੜੇ(), esz1.ਅੰਕੜੇ(), S.ਅੰਕੜੇ(), D, m))
    {
        ਅਚਲ Ch_ਬਹੁਲਕ ਬਹੁਲਕ = expandChannels ? decide_Channel_ਬਹੁਲਕ(arrays, K) : CH_ELEM;
        Auto_ਬਫਰ<MatShape, LOCAL_OPS> shp(K);
        Auto_ਬਫਰ<Mat_ਕਦਮ, LOCAL_OPS>  stp(K);
        ਲਈ (ਪੂਰਨ k = 0; k < K; k++)
        {
            matLayout(*arrays[k], ਬਹੁਲਕ, shp[k], stp[k], esz1[k]);
            ਅਧਾਰ[k] = (uchar*)arrays[k]->ਅੰਕੜੇ;
        }
        MatShape ਭਰਿਆ;
        CV_Assert(ਪ੍ਰਸਾਰਣ_Shape(shp.ਅੰਕੜੇ(), K, ਭਰਿਆ) && "ew: operands are not broadcast-compatible");
        ਅਚਲ ਪੂਰਨ nd = (ਪੂਰਨ)ਭਰਿਆ.size();
        Auto_ਬਫਰ<MatShape, LOCAL_OPS> H(K);
        ਲਈ (ਪੂਰਨ k = 0; k < K; k++) alignArg(shp[k], stp[k], nd, S[k], H[k]);
        D = ਭਰਿਆ;
        m = collapseDims(S.ਅੰਕੜੇ(), H.ਅੰਕੜੇ(), K, D);

        // For a cv::Mat the innermost (channel/last) axis is contiguous, so after collapse the
        // innermost stride is always in {0,1}. No gather, no materialization.
        ਲਈ (ਪੂਰਨ k = 0; k < K; k++)
            CV_Assert(S[k][m - 1] <= 1 && "ew: unexpected innermost stride > 1");
    }

    // ---- 4. inner 2D tile axes: width = D[m-1], height = D[m-2] (if any) ----
    ਅਚਲ ਪੂਰਨ wAxis = m - 1;
    ਅਚਲ ਪੂਰਨ hAxis = (m >= 2) ? m - 2 : -1;
    ਅਚਲ ਪੂਰਨ W   = D[wAxis];
    ਅਚਲ ਪੂਰਨ Hgt = (hAxis >= 0) ? D[hAxis] : 1;
    ਅਚਲ ਪੂਰਨ nOuter = (hAxis >= 0) ? m - 2 : m - 1;   // outer ("plane") axes = [0 .. nOuter)
    ਲੰਮਾ ਲੰਮਾ nplanes = 1;
    ਲਈ (ਪੂਰਨ d = 0; d < nOuter; d++) nplanes *= D[d];

    // ---- 5. desired parallel stripe count (work hint) ----
    ਅਚਲ ਲੰਮਾ ਲੰਮਾ ਕੁੱਲ = nplanes * (ਲੰਮਾ ਲੰਮਾ)Hgt * (ਲੰਮਾ ਲੰਮਾ)W;
    ਦੁੱਗਣਾ stripes = nstripes;
    ਜੇ (stripes <= 0)                                  // broadcastOp can't see the body's cost;
        stripes = (ਦੁੱਗਣਾ)ਕੁੱਲ * 100.0 / (ਦੁੱਗਣਾ)(1 << 18);  // assume ~100 cycles/element
    ਅਚਲ ਪੂਰਨ wantTiles = std::max(1, (ਪੂਰਨ)std::lround(stripes));

    // ---- 6. tile only for PARALLELISM. broadcastOp is op-agnostic: it does not know the body's
    //         temp-buffer footprint, so it does NOT tile for L1 - that is the body's job (it
    //         re-fragments a tile's width into L1-sized chunks for the fused intermediates).
    //         Start with the largest tile (one 2D block per plane) and split (height first, then
    //         width) only until there are at least `wantTiles` tiles. Bigger tiles => fewer
    //         body/decode calls. Width is G-aligned only in the fully-contiguous (1D) case. ----
    ਅਚਲ ਪੂਰਨ G = 16;                  // SIMD/cacheline granule
    ਪੂਰਨ tw = W, th = Hgt;

    ਸਵੈਚਾਲਿਤ ntiles_ਦਾ = [&](ਪੂਰਨ tw_, ਪੂਰਨ th_) {
        ਲੰਮਾ ਲੰਮਾ nw = (W + tw_ - 1) / tw_, nh = (Hgt + th_ - 1) / th_;
        ਵਾਪਸ nplanes * nh * nw;
    };
    ਲੰਮਾ ਲੰਮਾ ntiles = ntiles_ਦਾ(tw, th);
    ਜਦੋਂ (ntiles < wantTiles && th > 1)               // split height for parallelism
    {
        th = (th + 1) / 2;
        ntiles = ntiles_ਦਾ(tw, th);
    }
    ਜਦੋਂ (ntiles < wantTiles && tw > G)               // then split width
    {
        tw = std::max(G, tw / 2);
        ਜੇ (hAxis < 0 && tw > G) tw -= tw % G;          // keep width aligned in the 1D case
        ntiles = ntiles_ਦਾ(tw, th);
    }
    CV_Assert(ntiles <= (ਲੰਮਾ ਲੰਮਾ)ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ);

    ਅਚਲ ਪੂਰਨ ntilesW = (W + tw - 1) / tw;
    ਅਚਲ ਪੂਰਨ ntilesH = (Hgt + th - 1) / th;

    // ---- 7. execution; decode tile index -> per-operand slices. stepx/stepy are the same for
    //         every tile, so they are set ONCE; only the per-tile base pointer is recomputed. ----
    ਸਵੈਚਾਲਿਤ ਲੜੀਵਾਰ_Range = [&](ਅਚਲ Range& r)
    {
        Auto_ਬਫਰ<ਟੁਕੜਾ, LOCAL_OPS> slices(K);

        // Fast 1D path (m==1: one contiguous axis after collapse, no outer planes, height 1).
        // ntilesH==1 and nplanes==1, so the tile index IS the width-tile index - no div/mod, no
        // plane multi-index decode, no inner step loop. This is the same-shape / fully-contiguous
        // common case.
        ਜੇ (m == 1)
        {
            ਲਈ (ਪੂਰਨ k = 0; k < K; k++) { slices[k].stepy = 0; slices[k].stepx = S[k][0]; }
            Tile tile;
            tile.ਉਚਾਈ = 1; tile.narrays = K; tile.slices = slices.ਅੰਕੜੇ();
            ਲਈ (ਪੂਰਨ t = r.ਸ਼ੁਰੂ; t < r.end; t++)
            {
                ਅਚਲ ਪੂਰਨ wofs = t * tw, ww = std::min(tw, W - wofs);
                ਲਈ (ਪੂਰਨ k = 0; k < K; k++)
                    slices[k].ptr = ਅਧਾਰ[k] + (size_t)wofs * S[k][0] * (size_t)esz1[k];
                tile.ਚੌੜਾਈ = ww;
                body(tile);
            }
            ਵਾਪਸ;
        }

        std::array<ਪੂਰਨ, MAX_DIMS> idx;
        ਲਈ (ਪੂਰਨ k = 0; k < K; k++)                     // steps are tile-independent: set once
        {
            slices[k].stepy = (hAxis >= 0) ? S[k][hAxis] : 0;
            slices[k].stepx = S[k][wAxis];
        }
        ਲਈ (ਪੂਰਨ t = r.ਸ਼ੁਰੂ; t < r.end; t++)
        {
            ਪੂਰਨ wt = t % ntilesW;
            ਪੂਰਨ rest = t / ntilesW;
            ਪੂਰਨ ht = rest % ntilesH;
            ਪੂਰਨ plane = rest / ntilesH;

            ਅਚਲ ਪੂਰਨ wofs = wt * tw, ww = std::min(tw, W - wofs);
            ਅਚਲ ਪੂਰਨ hofs = ht * th, hh = std::min(th, Hgt - hofs);

            ਪੂਰਨ p = plane;                              // decode plane -> outer multi-index
            ਲਈ (ਪੂਰਨ d = nOuter - 1; d >= 0; d--) {
                ਪੂਰਨ dd = D[d];
                ਪੂਰਨ np = p / dd;
                idx[d] = p - np * dd;
                p = np;
            }

            ਲਈ (ਪੂਰਨ k = 0; k < K; k++)
            {
                size_t off = (size_t)wofs * S[k][wAxis];
                ਲਈ (ਪੂਰਨ d = 0; d < nOuter; d++) off += (size_t)idx[d] * S[k][d];
                ਜੇ (hAxis >= 0) off += (size_t)hofs * S[k][hAxis];
                slices[k].ptr = ਅਧਾਰ[k] + off * (size_t)esz1[k];
            }

            Tile tile;
            tile.ਚੌੜਾਈ = ww; tile.ਉਚਾਈ = hh; tile.narrays = K; tile.slices = slices.ਅੰਕੜੇ();
            body(tile);
        }
    };

    // Single tile (small work, wantTiles==1) => run inline and skip the parallel framework
    // entirely: its dispatch (std::function wrap + Range machinery + backend hop) is pure
    // overhead when there is nothing to parallelize, and dominates small-array latency.
    ਜੇ (ntiles == 1)
        ਲੜੀਵਾਰ_Range(Range(0, 1));
    ਨਹੀਂਤਾਂ
        parallel_for_(Range(0, (ਪੂਰਨ)ntiles), ਲੜੀਵਾਰ_Range, stripes);
}

} // namespace cv
