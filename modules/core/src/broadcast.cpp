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
ಎಣಿಕೆ Ch_ಬಹುಲಕ { CH_FOLD, CH_DIM, CH_ELEM };

ಸ್ಥಿರ ಶೂನ್ಯ matLayout(ಸ್ಥಿರಾಂಕ Mat& m, Ch_ಬಹುಲಕ ಬಹುಲಕ, MatShape& shp, Mat_ಹೆಜ್ಜೆ& ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ& esz1)
{
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ nd = m.dims, cn = m.channels();
    ಆದರೆ (ಬಹುಲಕ == CH_ELEM)
    {
        esz1 = (ಪೂರ್ಣಾಂಕ)m.elem_ಗಾತ್ರ();                       // one full (cn-channel) pixel
        shp.resize(nd);
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < nd; i++) { shp[i] = m.size[i]; ಹೆಜ್ಜೆ[i] = m.ಹೆಜ್ಜೆ[i] / esz1; }
        ಹಿಂತಿರುಗಿ;
    }
    esz1 = (ಪೂರ್ಣಾಂಕ)m.elemSize1();                           // one scalar (channel value)
    ಆದರೆ (ಬಹುಲಕ == CH_DIM)
    {
        shp.resize(nd + 1);
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < nd; i++) { shp[i] = m.size[i]; ಹೆಜ್ಜೆ[i] = m.ಹೆಜ್ಜೆ[i] / esz1; }
        shp[nd] = cn; ಹೆಜ್ಜೆ[nd] = 1;                      // channels = explicit innermost dim
    }
    ಇಲ್ಲದಿದ್ದರೆ // CH_FOLD
    {
        ಆದರೆ (nd == 0)                                     // 0-dim scalar: channels are the only dim
        {
            shp.assign(1, cn);
            ಹೆಜ್ಜೆ[0] = 1;
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            shp.resize(nd);
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < nd; i++) { shp[i] = m.size[i]; ಹೆಜ್ಜೆ[i] = m.ಹೆಜ್ಜೆ[i] / esz1; }
            shp[nd - 1] *= cn;                           // fold channels into the innermost dim
            ಹೆಜ್ಜೆ[nd - 1] = 1;                            // scalars are contiguous there
        }
    }
}

// A single-channel scalar (one value, cn==1, total()==1) broadcasts into everything trivially
// (step 0 on every axis incl. channels), so it must NOT force CH_DIM - it is excluded from the
// channel-mode decision entirely.
ಸ್ಥಿರ ತಾರ್ಕಿಕ isSingleChannelScalar(ಸ್ಥಿರಾಂಕ Mat& m)
{
    ಹಿಂತಿರುಗಿ m.channels() == 1 && m.ಒಟ್ಟು() == 1;
}

// Decide (globally, across all operands) how channels are presented for expandChannels=true:
// CH_FOLD when no channel broadcast is needed, CH_DIM when it is. Single-channel scalars are
// excluded first; among the rest, multichannel operands must all share the same cn (an (n,m) mix
// with both > 1 is an error).
ಸ್ಥಿರ Ch_ಬಹುಲಕ decide_Channel_ಬಹುಲಕ(ಸ್ಥಿರಾಂಕ Mat* ಸ್ಥಿರಾಂಕ* arrays, ಪೂರ್ಣಾಂಕ K)
{
    ಪೂರ್ಣಾಂಕ N = 1;                                           // the single multichannel count, if any
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ k = 0; k < K; k++)
    {
        ಆದರೆ (isSingleChannelScalar(*arrays[k])) ಮುಂದುವರಿ;
        ಪೂರ್ಣಾಂಕ c = arrays[k]->channels();
        ಆದರೆ (c > 1) { ಆದರೆ (N == 1) N = c; ಇಲ್ಲದಿದ್ದರೆ CV_Assert(N == c && "ew: (n,m) channel mix unsupported"); }
    }
    ಆದರೆ (N == 1) ಹಿಂತಿರುಗಿ CH_FOLD;                          // all single-channel -> fold (a no-op)

    ತಾರ್ಕಿಕ allMulti = ನಿಜ, same_ಹಿಂಭಾಗ = ನಿಜ;
    ಪೂರ್ಣಾಂಕ back = -1;
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ k = 0; k < K; k++)
    {
        ಸ್ಥಿರಾಂಕ Mat& a = *arrays[k];
        ಆದರೆ (isSingleChannelScalar(a)) ಮುಂದುವರಿ;
        ಆದರೆ (a.channels() != N) allMulti = ಸುಳ್ಳು;
        ಪೂರ್ಣಾಂಕ b = a.dims > 0 ? a.size[a.dims - 1] : 1;   // 0-dim scalar has no spatial back (=1)
        ಆದರೆ (back < 0) back = b; ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (b != back) same_ಹಿಂಭಾಗ = ಸುಳ್ಳು;
    }
    ಹಿಂತಿರುಗಿ (allMulti && same_ಹಿಂಭಾಗ) ? CH_FOLD : CH_DIM;    // fold only if no channel broadcast
}

// numpy-style broadcast of several right-aligned shapes.
ಸ್ಥಿರ ತಾರ್ಕಿಕ ಪ್ರಸಾರ_Shape(ಸ್ಥಿರಾಂಕ MatShape* shps, ಪೂರ್ಣಾಂಕ K, MatShape& out)
{
    size_t nd = 0;
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ k = 0; k < K; k++) nd = std::max(nd, shps[k].size());
    out.assign(nd, 1);
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ k = 0; k < K; k++)
    {
        ಸ್ಥಿರಾಂಕ MatShape& s = shps[k];
        size_t off = nd - s.size();
        ಗಾಗಿ (size_t i = 0; i < s.size(); i++)
        {
            ಪೂರ್ಣಾಂಕ d = s[i], &o = out[off + i];
            ಆದರೆ (o == 1) o = d;
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (d != 1 && d != o) ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
        }
    }
    ಹಿಂತಿರುಗಿ ನಿಜ;
}

// Right-align an arg's own (shp,step) to nd dims; broadcast dims get step 0.
ಸ್ಥಿರ ಶೂನ್ಯ alignArg(ಸ್ಥಿರಾಂಕ MatShape& shp, ಸ್ಥಿರಾಂಕ Mat_ಹೆಜ್ಜೆ& ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ nd,
                     Mat_ಹೆಜ್ಜೆ& as, MatShape& ash)
{
    as.clear();
    ash.assign(nd, 1);
    ಪೂರ್ಣಾಂಕ off = nd - (ಪೂರ್ಣಾಂಕ)shp.size();
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < (ಪೂರ್ಣಾಂಕ)shp.size(); i++)
    {
        ಪೂರ್ಣಾಂಕ d = shp[i];
        ash[off + i] = d;
        as[off + i] = (d == 1) ? 0 : ಹೆಜ್ಜೆ[i];
    }
}

// Collapse adjacent dims that are contiguous (and broadcast-consistent) across all args.
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ collapseDims(Mat_ಹೆಜ್ಜೆ* S, MatShape* H, ಪೂರ್ಣಾಂಕ K, MatShape& D)
{
    ಪೂರ್ಣಾಂಕ nd = (ಪೂರ್ಣಾಂಕ)D.size();
    ಆದರೆ (nd <= 1) ಹಿಂತಿರುಗಿ nd;

    ಪೂರ್ಣಾಂಕ j = nd - 1;
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = j - 1; i >= 0; i--)
    {
        ತಾರ್ಕಿಕ contig = ನಿಜ, scalar = ನಿಜ, consist = ನಿಜ;
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ k = 0; k < K; k++)
        {
            size_t st = S[k][j] * (size_t)H[k][j];
            ತಾರ್ಕಿಕ prevScalar = H[k][j] == 1;
            ತಾರ್ಕಿಕ curScalar = H[k][i] == 1;
            contig  = contig  && (st == S[k][i]);
            scalar  = scalar  && curScalar;
            consist = consist && (curScalar == prevScalar);
        }
        ಆದರೆ (contig && (consist || scalar))
        {
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ k = 0; k < K; k++) H[k][j] *= H[k][i];
            D[j] *= D[i];
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            j--;
            ಆದರೆ (i < j)
            {
                ಗಾಗಿ (ಪೂರ್ಣಾಂಕ k = 0; k < K; k++) { H[k][j] = H[k][i]; S[k][j] = S[k][i]; }
                D[j] = D[i];
            }
        }
    }

    ಪೂರ್ಣಾಂಕ m = nd - j;
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ d = 0; d < m; d++)
    {
        D[d] = D[j + d];
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ k = 0; k < K; k++) { S[k][d] = S[k][j + d]; H[k][d] = H[k][j + d]; }
    }
    D.resize(m);
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ k = 0; k < K; k++) H[k].resize(m);

    // Zero out steps of broadcast (size-1) dims (numpy step==0 trick).
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ d = 0; d < m; d++)
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ k = 0; k < K; k++)
            ಆದರೆ (H[k][d] == 1) S[k][d] = 0;

    ಹಿಂತಿರುಗಿ m;
}

// Fast geometry for the dominant case: every operand is either (a) an array sharing ONE common
// shape - same dims, sizes and channel count - and contiguous, or (b) a single-channel scalar
// (cn==1, total()==1). Then the whole traversal is a single contiguous 1D run of `total` scalars
// (channels folded in): arrays get stepx 1, scalars stepx 0. This skips decideChannelMode /
// broadcastShape / alignArg / collapseDims and all their per-operand buffers entirely. Returns
// false (leaving outputs untouched) when the operands don't fit, so the caller runs general
// geometry. expandChannels=false (CH_ELEM) keeps channels in the element and is left to general.
ಸ್ಥಿರ ತಾರ್ಕಿಕ fastSameShape(ಸ್ಥಿರಾಂಕ Mat* ಸ್ಥಿರಾಂಕ* arrays, ಪೂರ್ಣಾಂಕ K, ತಾರ್ಕಿಕ expandChannels,
                          uchar** ತಳ, ಪೂರ್ಣಾಂಕ* esz1, Mat_ಹೆಜ್ಜೆ* S, MatShape& D, ಪೂರ್ಣಾಂಕ& m)
{
    ಆದರೆ (!expandChannels) ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
    ಪೂರ್ಣಾಂಕ ref = -1;
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ k = 0; k < K; k++)
        ಆದರೆ (!isSingleChannelScalar(*arrays[k])) { ref = k; ನಿಲ್ಲಿಸು; }
    ಆದರೆ (ref < 0) ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;                       // all single-channel scalars: let general handle
    ಸ್ಥಿರಾಂಕ Mat& R = *arrays[ref];
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ rdims = R.dims, rcn = R.channels();
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ k = 0; k < K; k++)
    {
        ಸ್ಥಿರಾಂಕ Mat& a = *arrays[k];
        ಆದರೆ (isSingleChannelScalar(a)) ಮುಂದುವರಿ;
        ಆದರೆ (a.channels() != rcn || a.dims != rdims || !a.isContinuous()) ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < rdims; i++) ಆದರೆ (a.size[i] != R.size[i]) ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
    }
    ಸ್ಥಿರಾಂಕ ದೀರ್ಘ ದೀರ್ಘ ಒಟ್ಟು = (ದೀರ್ಘ ದೀರ್ಘ)R.ಒಟ್ಟು() * rcn;   // channels folded into the 1D run
    CV_Assert(ಒಟ್ಟು <= (ದೀರ್ಘ ದೀರ್ಘ)ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ);
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ k = 0; k < K; k++)
    {
        ಸ್ಥಿರಾಂಕ Mat& a = *arrays[k];
        ತಳ[k] = (uchar*)a.ದತ್ತಾಂಶ;
        esz1[k] = (ಪೂರ್ಣಾಂಕ)a.elemSize1();
        S[k][0] = isSingleChannelScalar(a) ? 0 : 1;
    }
    D.assign(1, (ಪೂರ್ಣಾಂಕ)ಒಟ್ಟು);
    m = 1;
    ಹಿಂತಿರುಗಿ ನಿಜ;
}

// ---------------------------------------------------------------------------
// BroadcastOp::run
// ---------------------------------------------------------------------------
// At namespace scope, NOT inside run(): MSVC 2019 loses the constexpr-ness of function-local
// constants used as template arguments inside a lambda (C2975).
ಸ್ಥಿರ constexpr ಪೂರ್ಣಾಂಕ MAX_DIMS = MatShape::MAX_DIMS;
ಸ್ಥಿರ constexpr ಪೂರ್ಣಾಂಕ LOCAL_OPS = 8;

ಶೂನ್ಯ ಪ್ರಸಾರ_Op::ಸತತ(ಸ್ಥಿರಾಂಕ Mat* ಸ್ಥಿರಾಂಕ* arrays, ಪೂರ್ಣಾಂಕ narrays,
                      ಸ್ಥಿರಾಂಕ std::function<ಶೂನ್ಯ(ಸ್ಥಿರಾಂಕ Tile&)>& body,
                      ತಾರ್ಕಿಕ expandChannels,
                      ದ್ವಿಗುಣ nstripes)
{
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ K = narrays;
    CV_Assert(K >= 1 && arrays != ಶೂನ್ಯಸೂಚಕ);

    // ---- 1-3. geometry: per-operand collapsed steps S[k], element sizes esz1[k], base
    //           pointers, and the collapsed iteration shape D (m dims). The fast path handles the
    //           dominant "all same-shape arrays (+ single-channel scalars)" case in one shot; the
    //           general path does decideChannelMode + broadcastShape + align + collapse. ----
    Auto_ಬಫರ್<Mat_ಹೆಜ್ಜೆ, LOCAL_OPS>  S(K);
    Auto_ಬಫರ್<ಪೂರ್ಣಾಂಕ, LOCAL_OPS>      esz1(K);
    Auto_ಬಫರ್<uchar*, LOCAL_OPS>   ತಳ(K);
    MatShape D;
    ಪೂರ್ಣಾಂಕ m;

    ಆದರೆ (!fastSameShape(arrays, K, expandChannels, ತಳ.ದತ್ತಾಂಶ(), esz1.ದತ್ತಾಂಶ(), S.ದತ್ತಾಂಶ(), D, m))
    {
        ಸ್ಥಿರಾಂಕ Ch_ಬಹುಲಕ ಬಹುಲಕ = expandChannels ? decide_Channel_ಬಹುಲಕ(arrays, K) : CH_ELEM;
        Auto_ಬಫರ್<MatShape, LOCAL_OPS> shp(K);
        Auto_ಬಫರ್<Mat_ಹೆಜ್ಜೆ, LOCAL_OPS>  stp(K);
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ k = 0; k < K; k++)
        {
            matLayout(*arrays[k], ಬಹುಲಕ, shp[k], stp[k], esz1[k]);
            ತಳ[k] = (uchar*)arrays[k]->ದತ್ತಾಂಶ;
        }
        MatShape ತುಂಬಿದ;
        CV_Assert(ಪ್ರಸಾರ_Shape(shp.ದತ್ತಾಂಶ(), K, ತುಂಬಿದ) && "ew: operands are not broadcast-compatible");
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ nd = (ಪೂರ್ಣಾಂಕ)ತುಂಬಿದ.size();
        Auto_ಬಫರ್<MatShape, LOCAL_OPS> H(K);
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ k = 0; k < K; k++) alignArg(shp[k], stp[k], nd, S[k], H[k]);
        D = ತುಂಬಿದ;
        m = collapseDims(S.ದತ್ತಾಂಶ(), H.ದತ್ತಾಂಶ(), K, D);

        // For a cv::Mat the innermost (channel/last) axis is contiguous, so after collapse the
        // innermost stride is always in {0,1}. No gather, no materialization.
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ k = 0; k < K; k++)
            CV_Assert(S[k][m - 1] <= 1 && "ew: unexpected innermost stride > 1");
    }

    // ---- 4. inner 2D tile axes: width = D[m-1], height = D[m-2] (if any) ----
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ wAxis = m - 1;
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ hAxis = (m >= 2) ? m - 2 : -1;
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ W   = D[wAxis];
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ Hgt = (hAxis >= 0) ? D[hAxis] : 1;
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ nOuter = (hAxis >= 0) ? m - 2 : m - 1;   // outer ("plane") axes = [0 .. nOuter)
    ದೀರ್ಘ ದೀರ್ಘ nplanes = 1;
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ d = 0; d < nOuter; d++) nplanes *= D[d];

    // ---- 5. desired parallel stripe count (work hint) ----
    ಸ್ಥಿರಾಂಕ ದೀರ್ಘ ದೀರ್ಘ ಒಟ್ಟು = nplanes * (ದೀರ್ಘ ದೀರ್ಘ)Hgt * (ದೀರ್ಘ ದೀರ್ಘ)W;
    ದ್ವಿಗುಣ stripes = nstripes;
    ಆದರೆ (stripes <= 0)                                  // broadcastOp can't see the body's cost;
        stripes = (ದ್ವಿಗುಣ)ಒಟ್ಟು * 100.0 / (ದ್ವಿಗುಣ)(1 << 18);  // assume ~100 cycles/element
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ wantTiles = std::max(1, (ಪೂರ್ಣಾಂಕ)std::lround(stripes));

    // ---- 6. tile only for PARALLELISM. broadcastOp is op-agnostic: it does not know the body's
    //         temp-buffer footprint, so it does NOT tile for L1 - that is the body's job (it
    //         re-fragments a tile's width into L1-sized chunks for the fused intermediates).
    //         Start with the largest tile (one 2D block per plane) and split (height first, then
    //         width) only until there are at least `wantTiles` tiles. Bigger tiles => fewer
    //         body/decode calls. Width is G-aligned only in the fully-contiguous (1D) case. ----
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ G = 16;                  // SIMD/cacheline granule
    ಪೂರ್ಣಾಂಕ tw = W, th = Hgt;

    ಸ್ವಯಂಚಾಲಿತ ntiles_ದ = [&](ಪೂರ್ಣಾಂಕ tw_, ಪೂರ್ಣಾಂಕ th_) {
        ದೀರ್ಘ ದೀರ್ಘ nw = (W + tw_ - 1) / tw_, nh = (Hgt + th_ - 1) / th_;
        ಹಿಂತಿರುಗಿ nplanes * nh * nw;
    };
    ದೀರ್ಘ ದೀರ್ಘ ntiles = ntiles_ದ(tw, th);
    ಆಗ (ntiles < wantTiles && th > 1)               // split height for parallelism
    {
        th = (th + 1) / 2;
        ntiles = ntiles_ದ(tw, th);
    }
    ಆಗ (ntiles < wantTiles && tw > G)               // then split width
    {
        tw = std::max(G, tw / 2);
        ಆದರೆ (hAxis < 0 && tw > G) tw -= tw % G;          // keep width aligned in the 1D case
        ntiles = ntiles_ದ(tw, th);
    }
    CV_Assert(ntiles <= (ದೀರ್ಘ ದೀರ್ಘ)ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ);

    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ntilesW = (W + tw - 1) / tw;
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ntilesH = (Hgt + th - 1) / th;

    // ---- 7. execution; decode tile index -> per-operand slices. stepx/stepy are the same for
    //         every tile, so they are set ONCE; only the per-tile base pointer is recomputed. ----
    ಸ್ವಯಂಚಾಲಿತ ಸತತ_Range = [&](ಸ್ಥಿರಾಂಕ Range& r)
    {
        Auto_ಬಫರ್<ತುಂಡು, LOCAL_OPS> slices(K);

        // Fast 1D path (m==1: one contiguous axis after collapse, no outer planes, height 1).
        // ntilesH==1 and nplanes==1, so the tile index IS the width-tile index - no div/mod, no
        // plane multi-index decode, no inner step loop. This is the same-shape / fully-contiguous
        // common case.
        ಆದರೆ (m == 1)
        {
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ k = 0; k < K; k++) { slices[k].stepy = 0; slices[k].stepx = S[k][0]; }
            Tile tile;
            tile.ಎತ್ತರ = 1; tile.narrays = K; tile.slices = slices.ದತ್ತಾಂಶ();
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ t = r.ಆರಂಭ; t < r.end; t++)
            {
                ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ wofs = t * tw, ww = std::min(tw, W - wofs);
                ಗಾಗಿ (ಪೂರ್ಣಾಂಕ k = 0; k < K; k++)
                    slices[k].ptr = ತಳ[k] + (size_t)wofs * S[k][0] * (size_t)esz1[k];
                tile.ಅಗಲ = ww;
                body(tile);
            }
            ಹಿಂತಿರುಗಿ;
        }

        std::array<ಪೂರ್ಣಾಂಕ, MAX_DIMS> idx;
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ k = 0; k < K; k++)                     // steps are tile-independent: set once
        {
            slices[k].stepy = (hAxis >= 0) ? S[k][hAxis] : 0;
            slices[k].stepx = S[k][wAxis];
        }
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ t = r.ಆರಂಭ; t < r.end; t++)
        {
            ಪೂರ್ಣಾಂಕ wt = t % ntilesW;
            ಪೂರ್ಣಾಂಕ rest = t / ntilesW;
            ಪೂರ್ಣಾಂಕ ht = rest % ntilesH;
            ಪೂರ್ಣಾಂಕ plane = rest / ntilesH;

            ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ wofs = wt * tw, ww = std::min(tw, W - wofs);
            ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ hofs = ht * th, hh = std::min(th, Hgt - hofs);

            ಪೂರ್ಣಾಂಕ p = plane;                              // decode plane -> outer multi-index
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ d = nOuter - 1; d >= 0; d--) {
                ಪೂರ್ಣಾಂಕ dd = D[d];
                ಪೂರ್ಣಾಂಕ np = p / dd;
                idx[d] = p - np * dd;
                p = np;
            }

            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ k = 0; k < K; k++)
            {
                size_t off = (size_t)wofs * S[k][wAxis];
                ಗಾಗಿ (ಪೂರ್ಣಾಂಕ d = 0; d < nOuter; d++) off += (size_t)idx[d] * S[k][d];
                ಆದರೆ (hAxis >= 0) off += (size_t)hofs * S[k][hAxis];
                slices[k].ptr = ತಳ[k] + off * (size_t)esz1[k];
            }

            Tile tile;
            tile.ಅಗಲ = ww; tile.ಎತ್ತರ = hh; tile.narrays = K; tile.slices = slices.ದತ್ತಾಂಶ();
            body(tile);
        }
    };

    // Single tile (small work, wantTiles==1) => run inline and skip the parallel framework
    // entirely: its dispatch (std::function wrap + Range machinery + backend hop) is pure
    // overhead when there is nothing to parallelize, and dominates small-array latency.
    ಆದರೆ (ntiles == 1)
        ಸತತ_Range(Range(0, 1));
    ಇಲ್ಲದಿದ್ದರೆ
        parallel_for_(Range(0, (ಪೂರ್ಣಾಂಕ)ntiles), ಸತತ_Range, stripes);
}

} // namespace cv
