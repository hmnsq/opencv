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

نامخلا cv {

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
شمار Ch_منوال { CH_FOLD, CH_DIM, CH_ELEM };

ساکن خالی matLayout(ثابت Mat& m, Ch_منوال منوال, MatShape& shp, Mat_قدم& قدم, صحیح& esz1)
{
    ثابت صحیح nd = m.dims, cn = m.channels();
    اگر (منوال == CH_ELEM)
    {
        esz1 = (صحیح)m.elem_سائز();                       // one full (cn-channel) pixel
        shp.resize(nd);
        برائے (صحیح i = 0; i < nd; i++) { shp[i] = m.size[i]; قدم[i] = m.قدم[i] / esz1; }
        واپس;
    }
    esz1 = (صحیح)m.elemSize1();                           // one scalar (channel value)
    اگر (منوال == CH_DIM)
    {
        shp.resize(nd + 1);
        برائے (صحیح i = 0; i < nd; i++) { shp[i] = m.size[i]; قدم[i] = m.قدم[i] / esz1; }
        shp[nd] = cn; قدم[nd] = 1;                      // channels = explicit innermost dim
    }
    ورنہ // CH_FOLD
    {
        اگر (nd == 0)                                     // 0-dim scalar: channels are the only dim
        {
            shp.assign(1, cn);
            قدم[0] = 1;
        }
        ورنہ
        {
            shp.resize(nd);
            برائے (صحیح i = 0; i < nd; i++) { shp[i] = m.size[i]; قدم[i] = m.قدم[i] / esz1; }
            shp[nd - 1] *= cn;                           // fold channels into the innermost dim
            قدم[nd - 1] = 1;                            // scalars are contiguous there
        }
    }
}

// A single-channel scalar (one value, cn==1, total()==1) broadcasts into everything trivially
// (step 0 on every axis incl. channels), so it must NOT force CH_DIM - it is excluded from the
// channel-mode decision entirely.
ساکن منطقی isSingleChannelScalar(ثابت Mat& m)
{
    واپس m.channels() == 1 && m.کل() == 1;
}

// Decide (globally, across all operands) how channels are presented for expandChannels=true:
// CH_FOLD when no channel broadcast is needed, CH_DIM when it is. Single-channel scalars are
// excluded first; among the rest, multichannel operands must all share the same cn (an (n,m) mix
// with both > 1 is an error).
ساکن Ch_منوال decide_Channel_منوال(ثابت Mat* ثابت* arrays, صحیح K)
{
    صحیح N = 1;                                           // the single multichannel count, if any
    برائے (صحیح k = 0; k < K; k++)
    {
        اگر (isSingleChannelScalar(*arrays[k])) جاری;
        صحیح c = arrays[k]->channels();
        اگر (c > 1) { اگر (N == 1) N = c; ورنہ CV_Assert(N == c && "ew: (n,m) channel mix unsupported"); }
    }
    اگر (N == 1) واپس CH_FOLD;                          // all single-channel -> fold (a no-op)

    منطقی allMulti = سچ, same_عقب = سچ;
    صحیح back = -1;
    برائے (صحیح k = 0; k < K; k++)
    {
        ثابت Mat& a = *arrays[k];
        اگر (isSingleChannelScalar(a)) جاری;
        اگر (a.channels() != N) allMulti = جھوٹ;
        صحیح b = a.dims > 0 ? a.size[a.dims - 1] : 1;   // 0-dim scalar has no spatial back (=1)
        اگر (back < 0) back = b; ورنہ اگر (b != back) same_عقب = جھوٹ;
    }
    واپس (allMulti && same_عقب) ? CH_FOLD : CH_DIM;    // fold only if no channel broadcast
}

// numpy-style broadcast of several right-aligned shapes.
ساکن منطقی نشر_Shape(ثابت MatShape* shps, صحیح K, MatShape& out)
{
    size_t nd = 0;
    برائے (صحیح k = 0; k < K; k++) nd = std::max(nd, shps[k].size());
    out.assign(nd, 1);
    برائے (صحیح k = 0; k < K; k++)
    {
        ثابت MatShape& s = shps[k];
        size_t off = nd - s.size();
        برائے (size_t i = 0; i < s.size(); i++)
        {
            صحیح d = s[i], &o = out[off + i];
            اگر (o == 1) o = d;
            ورنہ اگر (d != 1 && d != o) واپس جھوٹ;
        }
    }
    واپس سچ;
}

// Right-align an arg's own (shp,step) to nd dims; broadcast dims get step 0.
ساکن خالی alignArg(ثابت MatShape& shp, ثابت Mat_قدم& قدم, صحیح nd,
                     Mat_قدم& as, MatShape& ash)
{
    as.clear();
    ash.assign(nd, 1);
    صحیح off = nd - (صحیح)shp.size();
    برائے (صحیح i = 0; i < (صحیح)shp.size(); i++)
    {
        صحیح d = shp[i];
        ash[off + i] = d;
        as[off + i] = (d == 1) ? 0 : قدم[i];
    }
}

// Collapse adjacent dims that are contiguous (and broadcast-consistent) across all args.
ساکن صحیح collapseDims(Mat_قدم* S, MatShape* H, صحیح K, MatShape& D)
{
    صحیح nd = (صحیح)D.size();
    اگر (nd <= 1) واپس nd;

    صحیح j = nd - 1;
    برائے (صحیح i = j - 1; i >= 0; i--)
    {
        منطقی contig = سچ, scalar = سچ, consist = سچ;
        برائے (صحیح k = 0; k < K; k++)
        {
            size_t st = S[k][j] * (size_t)H[k][j];
            منطقی پچھلا_Scalar = H[k][j] == 1;
            منطقی curScalar = H[k][i] == 1;
            contig  = contig  && (st == S[k][i]);
            scalar  = scalar  && curScalar;
            consist = consist && (curScalar == پچھلا_Scalar);
        }
        اگر (contig && (consist || scalar))
        {
            برائے (صحیح k = 0; k < K; k++) H[k][j] *= H[k][i];
            D[j] *= D[i];
        }
        ورنہ
        {
            j--;
            اگر (i < j)
            {
                برائے (صحیح k = 0; k < K; k++) { H[k][j] = H[k][i]; S[k][j] = S[k][i]; }
                D[j] = D[i];
            }
        }
    }

    صحیح m = nd - j;
    برائے (صحیح d = 0; d < m; d++)
    {
        D[d] = D[j + d];
        برائے (صحیح k = 0; k < K; k++) { S[k][d] = S[k][j + d]; H[k][d] = H[k][j + d]; }
    }
    D.resize(m);
    برائے (صحیح k = 0; k < K; k++) H[k].resize(m);

    // Zero out steps of broadcast (size-1) dims (numpy step==0 trick).
    برائے (صحیح d = 0; d < m; d++)
        برائے (صحیح k = 0; k < K; k++)
            اگر (H[k][d] == 1) S[k][d] = 0;

    واپس m;
}

// Fast geometry for the dominant case: every operand is either (a) an array sharing ONE common
// shape - same dims, sizes and channel count - and contiguous, or (b) a single-channel scalar
// (cn==1, total()==1). Then the whole traversal is a single contiguous 1D run of `total` scalars
// (channels folded in): arrays get stepx 1, scalars stepx 0. This skips decideChannelMode /
// broadcastShape / alignArg / collapseDims and all their per-operand buffers entirely. Returns
// false (leaving outputs untouched) when the operands don't fit, so the caller runs general
// geometry. expandChannels=false (CH_ELEM) keeps channels in the element and is left to general.
ساکن منطقی fastSameShape(ثابت Mat* ثابت* arrays, صحیح K, منطقی expandChannels,
                          uchar** بنیاد, صحیح* esz1, Mat_قدم* S, MatShape& D, صحیح& m)
{
    اگر (!expandChannels) واپس جھوٹ;
    صحیح ref = -1;
    برائے (صحیح k = 0; k < K; k++)
        اگر (!isSingleChannelScalar(*arrays[k])) { ref = k; توڑو; }
    اگر (ref < 0) واپس جھوٹ;                       // all single-channel scalars: let general handle
    ثابت Mat& R = *arrays[ref];
    ثابت صحیح rdims = R.dims, rcn = R.channels();
    برائے (صحیح k = 0; k < K; k++)
    {
        ثابت Mat& a = *arrays[k];
        اگر (isSingleChannelScalar(a)) جاری;
        اگر (a.channels() != rcn || a.dims != rdims || !a.isContinuous()) واپس جھوٹ;
        برائے (صحیح i = 0; i < rdims; i++) اگر (a.size[i] != R.size[i]) واپس جھوٹ;
    }
    ثابت طویل طویل کل = (طویل طویل)R.کل() * rcn;   // channels folded into the 1D run
    CV_Assert(کل <= (طویل طویل)زیادہ_سے_زیادہ_عدد);
    برائے (صحیح k = 0; k < K; k++)
    {
        ثابت Mat& a = *arrays[k];
        بنیاد[k] = (uchar*)a.ڈیٹا;
        esz1[k] = (صحیح)a.elemSize1();
        S[k][0] = isSingleChannelScalar(a) ? 0 : 1;
    }
    D.assign(1, (صحیح)کل);
    m = 1;
    واپس سچ;
}

// ---------------------------------------------------------------------------
// BroadcastOp::run
// ---------------------------------------------------------------------------
// At namespace scope, NOT inside run(): MSVC 2019 loses the constexpr-ness of function-local
// constants used as template arguments inside a lambda (C2975).
ساکن constexpr صحیح MAX_DIMS = MatShape::MAX_DIMS;
ساکن constexpr صحیح LOCAL_OPS = 8;

خالی نشر_Op::تسلسل(ثابت Mat* ثابت* arrays, صحیح narrays,
                      ثابت std::function<خالی(ثابت Tile&)>& body,
                      منطقی expandChannels,
                      دوہرا nstripes)
{
    ثابت صحیح K = narrays;
    CV_Assert(K >= 1 && arrays != خالی_اشاریہ);

    // ---- 1-3. geometry: per-operand collapsed steps S[k], element sizes esz1[k], base
    //           pointers, and the collapsed iteration shape D (m dims). The fast path handles the
    //           dominant "all same-shape arrays (+ single-channel scalars)" case in one shot; the
    //           general path does decideChannelMode + broadcastShape + align + collapse. ----
    Auto_بفر<Mat_قدم, LOCAL_OPS>  S(K);
    Auto_بفر<صحیح, LOCAL_OPS>      esz1(K);
    Auto_بفر<uchar*, LOCAL_OPS>   بنیاد(K);
    MatShape D;
    صحیح m;

    اگر (!fastSameShape(arrays, K, expandChannels, بنیاد.ڈیٹا(), esz1.ڈیٹا(), S.ڈیٹا(), D, m))
    {
        ثابت Ch_منوال منوال = expandChannels ? decide_Channel_منوال(arrays, K) : CH_ELEM;
        Auto_بفر<MatShape, LOCAL_OPS> shp(K);
        Auto_بفر<Mat_قدم, LOCAL_OPS>  stp(K);
        برائے (صحیح k = 0; k < K; k++)
        {
            matLayout(*arrays[k], منوال, shp[k], stp[k], esz1[k]);
            بنیاد[k] = (uchar*)arrays[k]->ڈیٹا;
        }
        MatShape بھرا;
        CV_Assert(نشر_Shape(shp.ڈیٹا(), K, بھرا) && "ew: operands are not broadcast-compatible");
        ثابت صحیح nd = (صحیح)بھرا.size();
        Auto_بفر<MatShape, LOCAL_OPS> H(K);
        برائے (صحیح k = 0; k < K; k++) alignArg(shp[k], stp[k], nd, S[k], H[k]);
        D = بھرا;
        m = collapseDims(S.ڈیٹا(), H.ڈیٹا(), K, D);

        // For a cv::Mat the innermost (channel/last) axis is contiguous, so after collapse the
        // innermost stride is always in {0,1}. No gather, no materialization.
        برائے (صحیح k = 0; k < K; k++)
            CV_Assert(S[k][m - 1] <= 1 && "ew: unexpected innermost stride > 1");
    }

    // ---- 4. inner 2D tile axes: width = D[m-1], height = D[m-2] (if any) ----
    ثابت صحیح wAxis = m - 1;
    ثابت صحیح hAxis = (m >= 2) ? m - 2 : -1;
    ثابت صحیح W   = D[wAxis];
    ثابت صحیح Hgt = (hAxis >= 0) ? D[hAxis] : 1;
    ثابت صحیح nOuter = (hAxis >= 0) ? m - 2 : m - 1;   // outer ("plane") axes = [0 .. nOuter)
    طویل طویل nplanes = 1;
    برائے (صحیح d = 0; d < nOuter; d++) nplanes *= D[d];

    // ---- 5. desired parallel stripe count (work hint) ----
    ثابت طویل طویل کل = nplanes * (طویل طویل)Hgt * (طویل طویل)W;
    دوہرا stripes = nstripes;
    اگر (stripes <= 0)                                  // broadcastOp can't see the body's cost;
        stripes = (دوہرا)کل * 100.0 / (دوہرا)(1 << 18);  // assume ~100 cycles/element
    ثابت صحیح wantTiles = std::max(1, (صحیح)std::lround(stripes));

    // ---- 6. tile only for PARALLELISM. broadcastOp is op-agnostic: it does not know the body's
    //         temp-buffer footprint, so it does NOT tile for L1 - that is the body's job (it
    //         re-fragments a tile's width into L1-sized chunks for the fused intermediates).
    //         Start with the largest tile (one 2D block per plane) and split (height first, then
    //         width) only until there are at least `wantTiles` tiles. Bigger tiles => fewer
    //         body/decode calls. Width is G-aligned only in the fully-contiguous (1D) case. ----
    ثابت صحیح G = 16;                  // SIMD/cacheline granule
    صحیح tw = W, th = Hgt;

    خودکار ntiles_کا = [&](صحیح tw_, صحیح th_) {
        طویل طویل nw = (W + tw_ - 1) / tw_, nh = (Hgt + th_ - 1) / th_;
        واپس nplanes * nh * nw;
    };
    طویل طویل ntiles = ntiles_کا(tw, th);
    جب (ntiles < wantTiles && th > 1)               // split height for parallelism
    {
        th = (th + 1) / 2;
        ntiles = ntiles_کا(tw, th);
    }
    جب (ntiles < wantTiles && tw > G)               // then split width
    {
        tw = std::max(G, tw / 2);
        اگر (hAxis < 0 && tw > G) tw -= tw % G;          // keep width aligned in the 1D case
        ntiles = ntiles_کا(tw, th);
    }
    CV_Assert(ntiles <= (طویل طویل)زیادہ_سے_زیادہ_عدد);

    ثابت صحیح ntilesW = (W + tw - 1) / tw;
    ثابت صحیح ntilesH = (Hgt + th - 1) / th;

    // ---- 7. execution; decode tile index -> per-operand slices. stepx/stepy are the same for
    //         every tile, so they are set ONCE; only the per-tile base pointer is recomputed. ----
    خودکار تسلسل_Range = [&](ثابت Range& r)
    {
        Auto_بفر<قاش, LOCAL_OPS> slices(K);

        // Fast 1D path (m==1: one contiguous axis after collapse, no outer planes, height 1).
        // ntilesH==1 and nplanes==1, so the tile index IS the width-tile index - no div/mod, no
        // plane multi-index decode, no inner step loop. This is the same-shape / fully-contiguous
        // common case.
        اگر (m == 1)
        {
            برائے (صحیح k = 0; k < K; k++) { slices[k].stepy = 0; slices[k].stepx = S[k][0]; }
            Tile tile;
            tile.اونچائی = 1; tile.narrays = K; tile.slices = slices.ڈیٹا();
            برائے (صحیح t = r.آغاز; t < r.end; t++)
            {
                ثابت صحیح wofs = t * tw, ww = std::min(tw, W - wofs);
                برائے (صحیح k = 0; k < K; k++)
                    slices[k].ptr = بنیاد[k] + (size_t)wofs * S[k][0] * (size_t)esz1[k];
                tile.چوڑائی = ww;
                body(tile);
            }
            واپس;
        }

        std::array<صحیح, MAX_DIMS> idx;
        برائے (صحیح k = 0; k < K; k++)                     // steps are tile-independent: set once
        {
            slices[k].stepy = (hAxis >= 0) ? S[k][hAxis] : 0;
            slices[k].stepx = S[k][wAxis];
        }
        برائے (صحیح t = r.آغاز; t < r.end; t++)
        {
            صحیح wt = t % ntilesW;
            صحیح rest = t / ntilesW;
            صحیح ht = rest % ntilesH;
            صحیح plane = rest / ntilesH;

            ثابت صحیح wofs = wt * tw, ww = std::min(tw, W - wofs);
            ثابت صحیح hofs = ht * th, hh = std::min(th, Hgt - hofs);

            صحیح p = plane;                              // decode plane -> outer multi-index
            برائے (صحیح d = nOuter - 1; d >= 0; d--) {
                صحیح dd = D[d];
                صحیح np = p / dd;
                idx[d] = p - np * dd;
                p = np;
            }

            برائے (صحیح k = 0; k < K; k++)
            {
                size_t off = (size_t)wofs * S[k][wAxis];
                برائے (صحیح d = 0; d < nOuter; d++) off += (size_t)idx[d] * S[k][d];
                اگر (hAxis >= 0) off += (size_t)hofs * S[k][hAxis];
                slices[k].ptr = بنیاد[k] + off * (size_t)esz1[k];
            }

            Tile tile;
            tile.چوڑائی = ww; tile.اونچائی = hh; tile.narrays = K; tile.slices = slices.ڈیٹا();
            body(tile);
        }
    };

    // Single tile (small work, wantTiles==1) => run inline and skip the parallel framework
    // entirely: its dispatch (std::function wrap + Range machinery + backend hop) is pure
    // overhead when there is nothing to parallelize, and dominates small-array latency.
    اگر (ntiles == 1)
        تسلسل_Range(Range(0, 1));
    ورنہ
        parallel_for_(Range(0, (صحیح)ntiles), تسلسل_Range, stripes);
}

} // namespace cv
