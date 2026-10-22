// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

// Implementation of cv::BroadcastOp (declared in opencv2/core/mat.hpp): an op-agnostic driver for
// broadcasting element-wise traversal. It computes the numpy-broadcast iteration space over a flat
// list of operand Mats, collapses contiguous dims, partitions into tiles for parallel_for_, and hands
// each 2D tile's per-operand slices to a `body` callback (which owns all semantics).

#ضمن "precomp.hpp"
#ضمن <algorithm>
#ضمن <array>
#ضمن <climits>
#ضمن <cmath>

نطاق cv {

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
تعداد Ch_منوال { CH_FOLD, CH_DIM, CH_ELEM };

ساكن فراغ matLayout(ثابت Mat& m, Ch_منوال منوال, MatShape& shp, Mat_قدم& قدم, صحيح& esz1)
{
    ثابت صحيح nd = m.dims, cn = m.channels();
    إذا (منوال == CH_ELEM)
    {
        esz1 = (صحيح)m.elem_ناپ();                       // one full (cn-channel) pixel
        shp.resize(nd);
        لكل (صحيح i = 0; i < nd; i++) { shp[i] = m.size[i]; قدم[i] = m.قدم[i] / esz1; }
        أرجع;
    }
    esz1 = (صحيح)m.elemSize1();                           // one scalar (channel value)
    إذا (منوال == CH_DIM)
    {
        shp.resize(nd + 1);
        لكل (صحيح i = 0; i < nd; i++) { shp[i] = m.size[i]; قدم[i] = m.قدم[i] / esz1; }
        shp[nd] = cn; قدم[nd] = 1;                      // channels = explicit innermost dim
    }
    وإلا // CH_FOLD
    {
        إذا (nd == 0)                                     // 0-dim scalar: channels are the only dim
        {
            shp.assign(1, cn);
            قدم[0] = 1;
        }
        وإلا
        {
            shp.resize(nd);
            لكل (صحيح i = 0; i < nd; i++) { shp[i] = m.size[i]; قدم[i] = m.قدم[i] / esz1; }
            shp[nd - 1] *= cn;                           // fold channels into the innermost dim
            قدم[nd - 1] = 1;                            // scalars are contiguous there
        }
    }
}

// A single-channel scalar (one value, cn==1, total()==1) broadcasts into everything trivially
// (step 0 on every axis incl. channels), so it must NOT force CH_DIM - it is excluded from the
// channel-mode decision entirely.
ساكن منطقي isSingleChannelScalar(ثابت Mat& m)
{
    أرجع m.channels() == 1 && m.کل() == 1;
}

// Decide (globally, across all operands) how channels are presented for expandChannels=true:
// CH_FOLD when no channel broadcast is needed, CH_DIM when it is. Single-channel scalars are
// excluded first; among the rest, multichannel operands must all share the same cn (an (n,m) mix
// with both > 1 is an error).
ساكن Ch_منوال decide_Channel_منوال(ثابت Mat* ثابت* arrays, صحيح K)
{
    صحيح N = 1;                                           // the single multichannel count, if any
    لكل (صحيح k = 0; k < K; k++)
    {
        إذا (isSingleChannelScalar(*arrays[k])) استمر;
        صحيح c = arrays[k]->channels();
        إذا (c > 1) { إذا (N == 1) N = c; وإلا CV_Assert(N == c && "ew: (n,m) channel mix unsupported"); }
    }
    إذا (N == 1) أرجع CH_FOLD;                          // all single-channel -> fold (a no-op)

    منطقي allMulti = سچ, same_پٹھ = سچ;
    صحيح back = -1;
    لكل (صحيح k = 0; k < K; k++)
    {
        ثابت Mat& a = *arrays[k];
        إذا (isSingleChannelScalar(a)) استمر;
        إذا (a.channels() != N) allMulti = خطأ;
        صحيح b = a.dims > 0 ? a.size[a.dims - 1] : 1;   // 0-dim scalar has no spatial back (=1)
        إذا (back < 0) back = b; وإلا إذا (b != back) same_پٹھ = خطأ;
    }
    أرجع (allMulti && same_پٹھ) ? CH_FOLD : CH_DIM;    // fold only if no channel broadcast
}

// numpy-style broadcast of several right-aligned shapes.
ساكن منطقي نشر_Shape(ثابت MatShape* shps, صحيح K, MatShape& out)
{
    size_t nd = 0;
    لكل (صحيح k = 0; k < K; k++) nd = std::max(nd, shps[k].size());
    out.assign(nd, 1);
    لكل (صحيح k = 0; k < K; k++)
    {
        ثابت MatShape& s = shps[k];
        size_t off = nd - s.size();
        لكل (size_t i = 0; i < s.size(); i++)
        {
            صحيح d = s[i], &o = out[off + i];
            إذا (o == 1) o = d;
            وإلا إذا (d != 1 && d != o) أرجع خطأ;
        }
    }
    أرجع سچ;
}

// Right-align an arg's own (shp,step) to nd dims; broadcast dims get step 0.
ساكن فراغ alignArg(ثابت MatShape& shp, ثابت Mat_قدم& قدم, صحيح nd,
                     Mat_قدم& as, MatShape& ash)
{
    as.clear();
    ash.assign(nd, 1);
    صحيح off = nd - (صحيح)shp.size();
    لكل (صحيح i = 0; i < (صحيح)shp.size(); i++)
    {
        صحيح d = shp[i];
        ash[off + i] = d;
        as[off + i] = (d == 1) ? 0 : قدم[i];
    }
}

// Collapse adjacent dims that are contiguous (and broadcast-consistent) across all args.
ساكن صحيح collapseDims(Mat_قدم* S, MatShape* H, صحيح K, MatShape& D)
{
    صحيح nd = (صحيح)D.size();
    إذا (nd <= 1) أرجع nd;

    صحيح j = nd - 1;
    لكل (صحيح i = j - 1; i >= 0; i--)
    {
        منطقي contig = سچ, scalar = سچ, consist = سچ;
        لكل (صحيح k = 0; k < K; k++)
        {
            size_t st = S[k][j] * (size_t)H[k][j];
            منطقي prevScalar = H[k][j] == 1;
            منطقي curScalar = H[k][i] == 1;
            contig  = contig  && (st == S[k][i]);
            scalar  = scalar  && curScalar;
            consist = consist && (curScalar == prevScalar);
        }
        إذا (contig && (consist || scalar))
        {
            لكل (صحيح k = 0; k < K; k++) H[k][j] *= H[k][i];
            D[j] *= D[i];
        }
        وإلا
        {
            j--;
            إذا (i < j)
            {
                لكل (صحيح k = 0; k < K; k++) { H[k][j] = H[k][i]; S[k][j] = S[k][i]; }
                D[j] = D[i];
            }
        }
    }

    صحيح m = nd - j;
    لكل (صحيح d = 0; d < m; d++)
    {
        D[d] = D[j + d];
        لكل (صحيح k = 0; k < K; k++) { S[k][d] = S[k][j + d]; H[k][d] = H[k][j + d]; }
    }
    D.resize(m);
    لكل (صحيح k = 0; k < K; k++) H[k].resize(m);

    // Zero out steps of broadcast (size-1) dims (numpy step==0 trick).
    لكل (صحيح d = 0; d < m; d++)
        لكل (صحيح k = 0; k < K; k++)
            إذا (H[k][d] == 1) S[k][d] = 0;

    أرجع m;
}

// Fast geometry for the dominant case: every operand is either (a) an array sharing ONE common
// shape - same dims, sizes and channel count - and contiguous, or (b) a single-channel scalar
// (cn==1, total()==1). Then the whole traversal is a single contiguous 1D run of `total` scalars
// (channels folded in): arrays get stepx 1, scalars stepx 0. This skips decideChannelMode /
// broadcastShape / alignArg / collapseDims and all their per-operand buffers entirely. Returns
// false (leaving outputs untouched) when the operands don't fit, so the caller runs general
// geometry. expandChannels=false (CH_ELEM) keeps channels in the element and is left to general.
ساكن منطقي fastSameShape(ثابت Mat* ثابت* arrays, صحيح K, منطقي expandChannels,
                          uchar** ادھار, صحيح* esz1, Mat_قدم* S, MatShape& D, صحيح& m)
{
    إذا (!expandChannels) أرجع خطأ;
    صحيح ref = -1;
    لكل (صحيح k = 0; k < K; k++)
        إذا (!isSingleChannelScalar(*arrays[k])) { ref = k; اكسر; }
    إذا (ref < 0) أرجع خطأ;                       // all single-channel scalars: let general handle
    ثابت Mat& R = *arrays[ref];
    ثابت صحيح rdims = R.dims, rcn = R.channels();
    لكل (صحيح k = 0; k < K; k++)
    {
        ثابت Mat& a = *arrays[k];
        إذا (isSingleChannelScalar(a)) استمر;
        إذا (a.channels() != rcn || a.dims != rdims || !a.isContinuous()) أرجع خطأ;
        لكل (صحيح i = 0; i < rdims; i++) إذا (a.size[i] != R.size[i]) أرجع خطأ;
    }
    ثابت لما لما کل = (لما لما)R.کل() * rcn;   // channels folded into the 1D run
    CV_Assert(کل <= (لما لما)ودھ_توں_ودھ_عدد);
    لكل (صحيح k = 0; k < K; k++)
    {
        ثابت Mat& a = *arrays[k];
        ادھار[k] = (uchar*)a.انکڑے;
        esz1[k] = (صحيح)a.elemSize1();
        S[k][0] = isSingleChannelScalar(a) ? 0 : 1;
    }
    D.assign(1, (صحيح)کل);
    m = 1;
    أرجع سچ;
}

// ---------------------------------------------------------------------------
// BroadcastOp::run
// ---------------------------------------------------------------------------
// At namespace scope, NOT inside run(): MSVC 2019 loses the constexpr-ness of function-local
// constants used as template arguments inside a lambda (C2975).
ساكن constexpr صحيح MAX_DIMS = MatShape::MAX_DIMS;
ساكن constexpr صحيح LOCAL_OPS = 8;

فراغ نشر_Op::لگاتار(ثابت Mat* ثابت* arrays, صحيح narrays,
                      ثابت std::function<فراغ(ثابت Tile&)>& body,
                      منطقي expandChannels,
                      مزدوج nstripes)
{
    ثابت صحيح K = narrays;
    CV_Assert(K >= 1 && arrays != خالی_سوچک);

    // ---- 1-3. geometry: per-operand collapsed steps S[k], element sizes esz1[k], base
    //           pointers, and the collapsed iteration shape D (m dims). The fast path handles the
    //           dominant "all same-shape arrays (+ single-channel scalars)" case in one shot; the
    //           general path does decideChannelMode + broadcastShape + align + collapse. ----
    Auto_بفر<Mat_قدم, LOCAL_OPS>  S(K);
    Auto_بفر<صحيح, LOCAL_OPS>      esz1(K);
    Auto_بفر<uchar*, LOCAL_OPS>   ادھار(K);
    MatShape D;
    صحيح m;

    إذا (!fastSameShape(arrays, K, expandChannels, ادھار.انکڑے(), esz1.انکڑے(), S.انکڑے(), D, m))
    {
        ثابت Ch_منوال منوال = expandChannels ? decide_Channel_منوال(arrays, K) : CH_ELEM;
        Auto_بفر<MatShape, LOCAL_OPS> shp(K);
        Auto_بفر<Mat_قدم, LOCAL_OPS>  stp(K);
        لكل (صحيح k = 0; k < K; k++)
        {
            matLayout(*arrays[k], منوال, shp[k], stp[k], esz1[k]);
            ادھار[k] = (uchar*)arrays[k]->انکڑے;
        }
        MatShape بھریا;
        CV_Assert(نشر_Shape(shp.انکڑے(), K, بھریا) && "ew: operands are not broadcast-compatible");
        ثابت صحيح nd = (صحيح)بھریا.size();
        Auto_بفر<MatShape, LOCAL_OPS> H(K);
        لكل (صحيح k = 0; k < K; k++) alignArg(shp[k], stp[k], nd, S[k], H[k]);
        D = بھریا;
        m = collapseDims(S.انکڑے(), H.انکڑے(), K, D);

        // For a cv::Mat the innermost (channel/last) axis is contiguous, so after collapse the
        // innermost stride is always in {0,1}. No gather, no materialization.
        لكل (صحيح k = 0; k < K; k++)
            CV_Assert(S[k][m - 1] <= 1 && "ew: unexpected innermost stride > 1");
    }

    // ---- 4. inner 2D tile axes: width = D[m-1], height = D[m-2] (if any) ----
    ثابت صحيح wAxis = m - 1;
    ثابت صحيح hAxis = (m >= 2) ? m - 2 : -1;
    ثابت صحيح W   = D[wAxis];
    ثابت صحيح Hgt = (hAxis >= 0) ? D[hAxis] : 1;
    ثابت صحيح nOuter = (hAxis >= 0) ? m - 2 : m - 1;   // outer ("plane") axes = [0 .. nOuter)
    لما لما nplanes = 1;
    لكل (صحيح d = 0; d < nOuter; d++) nplanes *= D[d];

    // ---- 5. desired parallel stripe count (work hint) ----
    ثابت لما لما کل = nplanes * (لما لما)Hgt * (لما لما)W;
    مزدوج stripes = nstripes;
    إذا (stripes <= 0)                                  // broadcastOp can't see the body's cost;
        stripes = (مزدوج)کل * 100.0 / (مزدوج)(1 << 18);  // assume ~100 cycles/element
    ثابت صحيح wantTiles = std::max(1, (صحيح)std::lround(stripes));

    // ---- 6. tile only for PARALLELISM. broadcastOp is op-agnostic: it does not know the body's
    //         temp-buffer footprint, so it does NOT tile for L1 - that is the body's job (it
    //         re-fragments a tile's width into L1-sized chunks for the fused intermediates).
    //         Start with the largest tile (one 2D block per plane) and split (height first, then
    //         width) only until there are at least `wantTiles` tiles. Bigger tiles => fewer
    //         body/decode calls. Width is G-aligned only in the fully-contiguous (1D) case. ----
    ثابت صحيح G = 16;                  // SIMD/cacheline granule
    صحيح tw = W, th = Hgt;

    خودکار ntiles_دا = [&](صحيح tw_, صحيح th_) {
        لما لما nw = (W + tw_ - 1) / tw_, nh = (Hgt + th_ - 1) / th_;
        أرجع nplanes * nh * nw;
    };
    لما لما ntiles = ntiles_دا(tw, th);
    طالما (ntiles < wantTiles && th > 1)               // split height for parallelism
    {
        th = (th + 1) / 2;
        ntiles = ntiles_دا(tw, th);
    }
    طالما (ntiles < wantTiles && tw > G)               // then split width
    {
        tw = std::max(G, tw / 2);
        إذا (hAxis < 0 && tw > G) tw -= tw % G;          // keep width aligned in the 1D case
        ntiles = ntiles_دا(tw, th);
    }
    CV_Assert(ntiles <= (لما لما)ودھ_توں_ودھ_عدد);

    ثابت صحيح ntilesW = (W + tw - 1) / tw;
    ثابت صحيح ntilesH = (Hgt + th - 1) / th;

    // ---- 7. execution; decode tile index -> per-operand slices. stepx/stepy are the same for
    //         every tile, so they are set ONCE; only the per-tile base pointer is recomputed. ----
    خودکار لگاتار_Range = [&](ثابت Range& r)
    {
        Auto_بفر<ٹکڑا, LOCAL_OPS> slices(K);

        // Fast 1D path (m==1: one contiguous axis after collapse, no outer planes, height 1).
        // ntilesH==1 and nplanes==1, so the tile index IS the width-tile index - no div/mod, no
        // plane multi-index decode, no inner step loop. This is the same-shape / fully-contiguous
        // common case.
        إذا (m == 1)
        {
            لكل (صحيح k = 0; k < K; k++) { slices[k].stepy = 0; slices[k].stepx = S[k][0]; }
            Tile tile;
            tile.اچائی = 1; tile.narrays = K; tile.slices = slices.انکڑے();
            لكل (صحيح t = r.شروع; t < r.end; t++)
            {
                ثابت صحيح wofs = t * tw, ww = std::min(tw, W - wofs);
                لكل (صحيح k = 0; k < K; k++)
                    slices[k].ptr = ادھار[k] + (size_t)wofs * S[k][0] * (size_t)esz1[k];
                tile.چوڑائی = ww;
                body(tile);
            }
            أرجع;
        }

        std::array<صحيح, MAX_DIMS> idx;
        لكل (صحيح k = 0; k < K; k++)                     // steps are tile-independent: set once
        {
            slices[k].stepy = (hAxis >= 0) ? S[k][hAxis] : 0;
            slices[k].stepx = S[k][wAxis];
        }
        لكل (صحيح t = r.شروع; t < r.end; t++)
        {
            صحيح wt = t % ntilesW;
            صحيح rest = t / ntilesW;
            صحيح ht = rest % ntilesH;
            صحيح plane = rest / ntilesH;

            ثابت صحيح wofs = wt * tw, ww = std::min(tw, W - wofs);
            ثابت صحيح hofs = ht * th, hh = std::min(th, Hgt - hofs);

            صحيح p = plane;                              // decode plane -> outer multi-index
            لكل (صحيح d = nOuter - 1; d >= 0; d--) {
                صحيح dd = D[d];
                صحيح np = p / dd;
                idx[d] = p - np * dd;
                p = np;
            }

            لكل (صحيح k = 0; k < K; k++)
            {
                size_t off = (size_t)wofs * S[k][wAxis];
                لكل (صحيح d = 0; d < nOuter; d++) off += (size_t)idx[d] * S[k][d];
                إذا (hAxis >= 0) off += (size_t)hofs * S[k][hAxis];
                slices[k].ptr = ادھار[k] + off * (size_t)esz1[k];
            }

            Tile tile;
            tile.چوڑائی = ww; tile.اچائی = hh; tile.narrays = K; tile.slices = slices.انکڑے();
            body(tile);
        }
    };

    // Single tile (small work, wantTiles==1) => run inline and skip the parallel framework
    // entirely: its dispatch (std::function wrap + Range machinery + backend hop) is pure
    // overhead when there is nothing to parallelize, and dominates small-array latency.
    إذا (ntiles == 1)
        لگاتار_Range(Range(0, 1));
    وإلا
        parallel_for_(Range(0, (صحيح)ntiles), لگاتار_Range, stripes);
}

} // namespace cv
