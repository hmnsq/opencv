// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

// Implementation of cv::BroadcastOp (declared in opencv2/core/mat.hpp): an op-agnostic driver for
// broadcasting element-wise traversal. It computes the numpy-broadcast iteration space over a flat
// list of operand Mats, collapses contiguous dims, partitions into tiles for parallel_for_, and hands
// each 2D tile's per-operand slices to a `body` callback (which owns all semantics).

#అన్తర్భావ "precomp.hpp"
#అన్తర్భావ <algorithm>
#అన్తర్భావ <array>
#అన్తర్భావ <climits>
#అన్తర్భావ <cmath>

నామాకాశ cv {

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
గణన Ch_బహుళకం { CH_FOLD, CH_DIM, CH_ELEM };

స్థిర శూన్య matLayout(స్థిరరాశి Mat& m, Ch_బహుళకం బహుళకం, MatShape& shp, Mat_అడుగు& అడుగు, పూర్ణాంకం& esz1)
{
    స్థిరరాశి పూర్ణాంకం nd = m.dims, cn = m.channels();
    ఒకవేళ (బహుళకం == CH_ELEM)
    {
        esz1 = (పూర్ణాంకం)m.elem_కొలత();                       // one full (cn-channel) pixel
        shp.resize(nd);
        కొరకు (పూర్ణాంకం i = 0; i < nd; i++) { shp[i] = m.size[i]; అడుగు[i] = m.అడుగు[i] / esz1; }
        తిరిగి;
    }
    esz1 = (పూర్ణాంకం)m.elemSize1();                           // one scalar (channel value)
    ఒకవేళ (బహుళకం == CH_DIM)
    {
        shp.resize(nd + 1);
        కొరకు (పూర్ణాంకం i = 0; i < nd; i++) { shp[i] = m.size[i]; అడుగు[i] = m.అడుగు[i] / esz1; }
        shp[nd] = cn; అడుగు[nd] = 1;                      // channels = explicit innermost dim
    }
    లేకపోతే // CH_FOLD
    {
        ఒకవేళ (nd == 0)                                     // 0-dim scalar: channels are the only dim
        {
            shp.assign(1, cn);
            అడుగు[0] = 1;
        }
        లేకపోతే
        {
            shp.resize(nd);
            కొరకు (పూర్ణాంకం i = 0; i < nd; i++) { shp[i] = m.size[i]; అడుగు[i] = m.అడుగు[i] / esz1; }
            shp[nd - 1] *= cn;                           // fold channels into the innermost dim
            అడుగు[nd - 1] = 1;                            // scalars are contiguous there
        }
    }
}

// A single-channel scalar (one value, cn==1, total()==1) broadcasts into everything trivially
// (step 0 on every axis incl. channels), so it must NOT force CH_DIM - it is excluded from the
// channel-mode decision entirely.
స్థిర తార్కిక isSingleChannelScalar(స్థిరరాశి Mat& m)
{
    తిరిగి m.channels() == 1 && m.మొత్తం() == 1;
}

// Decide (globally, across all operands) how channels are presented for expandChannels=true:
// CH_FOLD when no channel broadcast is needed, CH_DIM when it is. Single-channel scalars are
// excluded first; among the rest, multichannel operands must all share the same cn (an (n,m) mix
// with both > 1 is an error).
స్థిర Ch_బహుళకం decide_Channel_బహుళకం(స్థిరరాశి Mat* స్థిరరాశి* arrays, పూర్ణాంకం K)
{
    పూర్ణాంకం N = 1;                                           // the single multichannel count, if any
    కొరకు (పూర్ణాంకం k = 0; k < K; k++)
    {
        ఒకవేళ (isSingleChannelScalar(*arrays[k])) కొనసాగు;
        పూర్ణాంకం c = arrays[k]->channels();
        ఒకవేళ (c > 1) { ఒకవేళ (N == 1) N = c; లేకపోతే CV_Assert(N == c && "ew: (n,m) channel mix unsupported"); }
    }
    ఒకవేళ (N == 1) తిరిగి CH_FOLD;                          // all single-channel -> fold (a no-op)

    తార్కిక allMulti = నిజం, same_వెనుక = నిజం;
    పూర్ణాంకం back = -1;
    కొరకు (పూర్ణాంకం k = 0; k < K; k++)
    {
        స్థిరరాశి Mat& a = *arrays[k];
        ఒకవేళ (isSingleChannelScalar(a)) కొనసాగు;
        ఒకవేళ (a.channels() != N) allMulti = అబద్ధం;
        పూర్ణాంకం b = a.dims > 0 ? a.size[a.dims - 1] : 1;   // 0-dim scalar has no spatial back (=1)
        ఒకవేళ (back < 0) back = b; లేకపోతే ఒకవేళ (b != back) same_వెనుక = అబద్ధం;
    }
    తిరిగి (allMulti && same_వెనుక) ? CH_FOLD : CH_DIM;    // fold only if no channel broadcast
}

// numpy-style broadcast of several right-aligned shapes.
స్థిర తార్కిక ప్రసారం_Shape(స్థిరరాశి MatShape* shps, పూర్ణాంకం K, MatShape& out)
{
    size_t nd = 0;
    కొరకు (పూర్ణాంకం k = 0; k < K; k++) nd = std::max(nd, shps[k].size());
    out.assign(nd, 1);
    కొరకు (పూర్ణాంకం k = 0; k < K; k++)
    {
        స్థిరరాశి MatShape& s = shps[k];
        size_t off = nd - s.size();
        కొరకు (size_t i = 0; i < s.size(); i++)
        {
            పూర్ణాంకం d = s[i], &o = out[off + i];
            ఒకవేళ (o == 1) o = d;
            లేకపోతే ఒకవేళ (d != 1 && d != o) తిరిగి అబద్ధం;
        }
    }
    తిరిగి నిజం;
}

// Right-align an arg's own (shp,step) to nd dims; broadcast dims get step 0.
స్థిర శూన్య alignArg(స్థిరరాశి MatShape& shp, స్థిరరాశి Mat_అడుగు& అడుగు, పూర్ణాంకం nd,
                     Mat_అడుగు& as, MatShape& ash)
{
    as.clear();
    ash.assign(nd, 1);
    పూర్ణాంకం off = nd - (పూర్ణాంకం)shp.size();
    కొరకు (పూర్ణాంకం i = 0; i < (పూర్ణాంకం)shp.size(); i++)
    {
        పూర్ణాంకం d = shp[i];
        ash[off + i] = d;
        as[off + i] = (d == 1) ? 0 : అడుగు[i];
    }
}

// Collapse adjacent dims that are contiguous (and broadcast-consistent) across all args.
స్థిర పూర్ణాంకం collapseDims(Mat_అడుగు* S, MatShape* H, పూర్ణాంకం K, MatShape& D)
{
    పూర్ణాంకం nd = (పూర్ణాంకం)D.size();
    ఒకవేళ (nd <= 1) తిరిగి nd;

    పూర్ణాంకం j = nd - 1;
    కొరకు (పూర్ణాంకం i = j - 1; i >= 0; i--)
    {
        తార్కిక contig = నిజం, scalar = నిజం, consist = నిజం;
        కొరకు (పూర్ణాంకం k = 0; k < K; k++)
        {
            size_t st = S[k][j] * (size_t)H[k][j];
            తార్కిక prevScalar = H[k][j] == 1;
            తార్కిక curScalar = H[k][i] == 1;
            contig  = contig  && (st == S[k][i]);
            scalar  = scalar  && curScalar;
            consist = consist && (curScalar == prevScalar);
        }
        ఒకవేళ (contig && (consist || scalar))
        {
            కొరకు (పూర్ణాంకం k = 0; k < K; k++) H[k][j] *= H[k][i];
            D[j] *= D[i];
        }
        లేకపోతే
        {
            j--;
            ఒకవేళ (i < j)
            {
                కొరకు (పూర్ణాంకం k = 0; k < K; k++) { H[k][j] = H[k][i]; S[k][j] = S[k][i]; }
                D[j] = D[i];
            }
        }
    }

    పూర్ణాంకం m = nd - j;
    కొరకు (పూర్ణాంకం d = 0; d < m; d++)
    {
        D[d] = D[j + d];
        కొరకు (పూర్ణాంకం k = 0; k < K; k++) { S[k][d] = S[k][j + d]; H[k][d] = H[k][j + d]; }
    }
    D.resize(m);
    కొరకు (పూర్ణాంకం k = 0; k < K; k++) H[k].resize(m);

    // Zero out steps of broadcast (size-1) dims (numpy step==0 trick).
    కొరకు (పూర్ణాంకం d = 0; d < m; d++)
        కొరకు (పూర్ణాంకం k = 0; k < K; k++)
            ఒకవేళ (H[k][d] == 1) S[k][d] = 0;

    తిరిగి m;
}

// Fast geometry for the dominant case: every operand is either (a) an array sharing ONE common
// shape - same dims, sizes and channel count - and contiguous, or (b) a single-channel scalar
// (cn==1, total()==1). Then the whole traversal is a single contiguous 1D run of `total` scalars
// (channels folded in): arrays get stepx 1, scalars stepx 0. This skips decideChannelMode /
// broadcastShape / alignArg / collapseDims and all their per-operand buffers entirely. Returns
// false (leaving outputs untouched) when the operands don't fit, so the caller runs general
// geometry. expandChannels=false (CH_ELEM) keeps channels in the element and is left to general.
స్థిర తార్కిక fastSameShape(స్థిరరాశి Mat* స్థిరరాశి* arrays, పూర్ణాంకం K, తార్కిక expandChannels,
                          uchar** పునాది, పూర్ణాంకం* esz1, Mat_అడుగు* S, MatShape& D, పూర్ణాంకం& m)
{
    ఒకవేళ (!expandChannels) తిరిగి అబద్ధం;
    పూర్ణాంకం ref = -1;
    కొరకు (పూర్ణాంకం k = 0; k < K; k++)
        ఒకవేళ (!isSingleChannelScalar(*arrays[k])) { ref = k; ఆపు; }
    ఒకవేళ (ref < 0) తిరిగి అబద్ధం;                       // all single-channel scalars: let general handle
    స్థిరరాశి Mat& R = *arrays[ref];
    స్థిరరాశి పూర్ణాంకం rdims = R.dims, rcn = R.channels();
    కొరకు (పూర్ణాంకం k = 0; k < K; k++)
    {
        స్థిరరాశి Mat& a = *arrays[k];
        ఒకవేళ (isSingleChannelScalar(a)) కొనసాగు;
        ఒకవేళ (a.channels() != rcn || a.dims != rdims || !a.isContinuous()) తిరిగి అబద్ధం;
        కొరకు (పూర్ణాంకం i = 0; i < rdims; i++) ఒకవేళ (a.size[i] != R.size[i]) తిరిగి అబద్ధం;
    }
    స్థిరరాశి దీర్ఘ దీర్ఘ మొత్తం = (దీర్ఘ దీర్ఘ)R.మొత్తం() * rcn;   // channels folded into the 1D run
    CV_Assert(మొత్తం <= (దీర్ఘ దీర్ఘ)గరిష్ఠ_పూర్ణాంకం);
    కొరకు (పూర్ణాంకం k = 0; k < K; k++)
    {
        స్థిరరాశి Mat& a = *arrays[k];
        పునాది[k] = (uchar*)a.దత్తాంశం;
        esz1[k] = (పూర్ణాంకం)a.elemSize1();
        S[k][0] = isSingleChannelScalar(a) ? 0 : 1;
    }
    D.assign(1, (పూర్ణాంకం)మొత్తం);
    m = 1;
    తిరిగి నిజం;
}

// ---------------------------------------------------------------------------
// BroadcastOp::run
// ---------------------------------------------------------------------------
// At namespace scope, NOT inside run(): MSVC 2019 loses the constexpr-ness of function-local
// constants used as template arguments inside a lambda (C2975).
స్థిర constexpr పూర్ణాంకం MAX_DIMS = MatShape::MAX_DIMS;
స్థిర constexpr పూర్ణాంకం LOCAL_OPS = 8;

శూన్య ప్రసారం_Op::నిరంతరం(స్థిరరాశి Mat* స్థిరరాశి* arrays, పూర్ణాంకం narrays,
                      స్థిరరాశి std::function<శూన్య(స్థిరరాశి Tile&)>& body,
                      తార్కిక expandChannels,
                      ద్విగుణ nstripes)
{
    స్థిరరాశి పూర్ణాంకం K = narrays;
    CV_Assert(K >= 1 && arrays != శూన్యసూచిక);

    // ---- 1-3. geometry: per-operand collapsed steps S[k], element sizes esz1[k], base
    //           pointers, and the collapsed iteration shape D (m dims). The fast path handles the
    //           dominant "all same-shape arrays (+ single-channel scalars)" case in one shot; the
    //           general path does decideChannelMode + broadcastShape + align + collapse. ----
    Auto_బఫరు<Mat_అడుగు, LOCAL_OPS>  S(K);
    Auto_బఫరు<పూర్ణాంకం, LOCAL_OPS>      esz1(K);
    Auto_బఫరు<uchar*, LOCAL_OPS>   పునాది(K);
    MatShape D;
    పూర్ణాంకం m;

    ఒకవేళ (!fastSameShape(arrays, K, expandChannels, పునాది.దత్తాంశం(), esz1.దత్తాంశం(), S.దత్తాంశం(), D, m))
    {
        స్థిరరాశి Ch_బహుళకం బహుళకం = expandChannels ? decide_Channel_బహుళకం(arrays, K) : CH_ELEM;
        Auto_బఫరు<MatShape, LOCAL_OPS> shp(K);
        Auto_బఫరు<Mat_అడుగు, LOCAL_OPS>  stp(K);
        కొరకు (పూర్ణాంకం k = 0; k < K; k++)
        {
            matLayout(*arrays[k], బహుళకం, shp[k], stp[k], esz1[k]);
            పునాది[k] = (uchar*)arrays[k]->దత్తాంశం;
        }
        MatShape నిండిన;
        CV_Assert(ప్రసారం_Shape(shp.దత్తాంశం(), K, నిండిన) && "ew: operands are not broadcast-compatible");
        స్థిరరాశి పూర్ణాంకం nd = (పూర్ణాంకం)నిండిన.size();
        Auto_బఫరు<MatShape, LOCAL_OPS> H(K);
        కొరకు (పూర్ణాంకం k = 0; k < K; k++) alignArg(shp[k], stp[k], nd, S[k], H[k]);
        D = నిండిన;
        m = collapseDims(S.దత్తాంశం(), H.దత్తాంశం(), K, D);

        // For a cv::Mat the innermost (channel/last) axis is contiguous, so after collapse the
        // innermost stride is always in {0,1}. No gather, no materialization.
        కొరకు (పూర్ణాంకం k = 0; k < K; k++)
            CV_Assert(S[k][m - 1] <= 1 && "ew: unexpected innermost stride > 1");
    }

    // ---- 4. inner 2D tile axes: width = D[m-1], height = D[m-2] (if any) ----
    స్థిరరాశి పూర్ణాంకం wAxis = m - 1;
    స్థిరరాశి పూర్ణాంకం hAxis = (m >= 2) ? m - 2 : -1;
    స్థిరరాశి పూర్ణాంకం W   = D[wAxis];
    స్థిరరాశి పూర్ణాంకం Hgt = (hAxis >= 0) ? D[hAxis] : 1;
    స్థిరరాశి పూర్ణాంకం nOuter = (hAxis >= 0) ? m - 2 : m - 1;   // outer ("plane") axes = [0 .. nOuter)
    దీర్ఘ దీర్ఘ nplanes = 1;
    కొరకు (పూర్ణాంకం d = 0; d < nOuter; d++) nplanes *= D[d];

    // ---- 5. desired parallel stripe count (work hint) ----
    స్థిరరాశి దీర్ఘ దీర్ఘ మొత్తం = nplanes * (దీర్ఘ దీర్ఘ)Hgt * (దీర్ఘ దీర్ఘ)W;
    ద్విగుణ stripes = nstripes;
    ఒకవేళ (stripes <= 0)                                  // broadcastOp can't see the body's cost;
        stripes = (ద్విగుణ)మొత్తం * 100.0 / (ద్విగుణ)(1 << 18);  // assume ~100 cycles/element
    స్థిరరాశి పూర్ణాంకం wantTiles = std::max(1, (పూర్ణాంకం)std::lround(stripes));

    // ---- 6. tile only for PARALLELISM. broadcastOp is op-agnostic: it does not know the body's
    //         temp-buffer footprint, so it does NOT tile for L1 - that is the body's job (it
    //         re-fragments a tile's width into L1-sized chunks for the fused intermediates).
    //         Start with the largest tile (one 2D block per plane) and split (height first, then
    //         width) only until there are at least `wantTiles` tiles. Bigger tiles => fewer
    //         body/decode calls. Width is G-aligned only in the fully-contiguous (1D) case. ----
    స్థిరరాశి పూర్ణాంకం G = 16;                  // SIMD/cacheline granule
    పూర్ణాంకం tw = W, th = Hgt;

    స్వయంచాలక ntiles_యొక్క = [&](పూర్ణాంకం tw_, పూర్ణాంకం th_) {
        దీర్ఘ దీర్ఘ nw = (W + tw_ - 1) / tw_, nh = (Hgt + th_ - 1) / th_;
        తిరిగి nplanes * nh * nw;
    };
    దీర్ఘ దీర్ఘ ntiles = ntiles_యొక్క(tw, th);
    అయితే (ntiles < wantTiles && th > 1)               // split height for parallelism
    {
        th = (th + 1) / 2;
        ntiles = ntiles_యొక్క(tw, th);
    }
    అయితే (ntiles < wantTiles && tw > G)               // then split width
    {
        tw = std::max(G, tw / 2);
        ఒకవేళ (hAxis < 0 && tw > G) tw -= tw % G;          // keep width aligned in the 1D case
        ntiles = ntiles_యొక్క(tw, th);
    }
    CV_Assert(ntiles <= (దీర్ఘ దీర్ఘ)గరిష్ఠ_పూర్ణాంకం);

    స్థిరరాశి పూర్ణాంకం ntilesW = (W + tw - 1) / tw;
    స్థిరరాశి పూర్ణాంకం ntilesH = (Hgt + th - 1) / th;

    // ---- 7. execution; decode tile index -> per-operand slices. stepx/stepy are the same for
    //         every tile, so they are set ONCE; only the per-tile base pointer is recomputed. ----
    స్వయంచాలక నిరంతరం_Range = [&](స్థిరరాశి Range& r)
    {
        Auto_బఫరు<ముక్క, LOCAL_OPS> slices(K);

        // Fast 1D path (m==1: one contiguous axis after collapse, no outer planes, height 1).
        // ntilesH==1 and nplanes==1, so the tile index IS the width-tile index - no div/mod, no
        // plane multi-index decode, no inner step loop. This is the same-shape / fully-contiguous
        // common case.
        ఒకవేళ (m == 1)
        {
            కొరకు (పూర్ణాంకం k = 0; k < K; k++) { slices[k].stepy = 0; slices[k].stepx = S[k][0]; }
            Tile tile;
            tile.ఎత్తు = 1; tile.narrays = K; tile.slices = slices.దత్తాంశం();
            కొరకు (పూర్ణాంకం t = r.ప్రారంభం; t < r.end; t++)
            {
                స్థిరరాశి పూర్ణాంకం wofs = t * tw, ww = std::min(tw, W - wofs);
                కొరకు (పూర్ణాంకం k = 0; k < K; k++)
                    slices[k].ptr = పునాది[k] + (size_t)wofs * S[k][0] * (size_t)esz1[k];
                tile.వెడల్పు = ww;
                body(tile);
            }
            తిరిగి;
        }

        std::array<పూర్ణాంకం, MAX_DIMS> idx;
        కొరకు (పూర్ణాంకం k = 0; k < K; k++)                     // steps are tile-independent: set once
        {
            slices[k].stepy = (hAxis >= 0) ? S[k][hAxis] : 0;
            slices[k].stepx = S[k][wAxis];
        }
        కొరకు (పూర్ణాంకం t = r.ప్రారంభం; t < r.end; t++)
        {
            పూర్ణాంకం wt = t % ntilesW;
            పూర్ణాంకం rest = t / ntilesW;
            పూర్ణాంకం ht = rest % ntilesH;
            పూర్ణాంకం plane = rest / ntilesH;

            స్థిరరాశి పూర్ణాంకం wofs = wt * tw, ww = std::min(tw, W - wofs);
            స్థిరరాశి పూర్ణాంకం hofs = ht * th, hh = std::min(th, Hgt - hofs);

            పూర్ణాంకం p = plane;                              // decode plane -> outer multi-index
            కొరకు (పూర్ణాంకం d = nOuter - 1; d >= 0; d--) {
                పూర్ణాంకం dd = D[d];
                పూర్ణాంకం np = p / dd;
                idx[d] = p - np * dd;
                p = np;
            }

            కొరకు (పూర్ణాంకం k = 0; k < K; k++)
            {
                size_t off = (size_t)wofs * S[k][wAxis];
                కొరకు (పూర్ణాంకం d = 0; d < nOuter; d++) off += (size_t)idx[d] * S[k][d];
                ఒకవేళ (hAxis >= 0) off += (size_t)hofs * S[k][hAxis];
                slices[k].ptr = పునాది[k] + off * (size_t)esz1[k];
            }

            Tile tile;
            tile.వెడల్పు = ww; tile.ఎత్తు = hh; tile.narrays = K; tile.slices = slices.దత్తాంశం();
            body(tile);
        }
    };

    // Single tile (small work, wantTiles==1) => run inline and skip the parallel framework
    // entirely: its dispatch (std::function wrap + Range machinery + backend hop) is pure
    // overhead when there is nothing to parallelize, and dominates small-array latency.
    ఒకవేళ (ntiles == 1)
        నిరంతరం_Range(Range(0, 1));
    లేకపోతే
        parallel_for_(Range(0, (పూర్ణాంకం)ntiles), నిరంతరం_Range, stripes);
}

} // namespace cv
