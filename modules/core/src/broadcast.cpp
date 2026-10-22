// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

// Implementation of cv::BroadcastOp (declared in opencv2/core/mat.hpp): an op-agnostic driver for
// broadcasting element-wise traversal. It computes the numpy-broadcast iteration space over a flat
// list of operand Mats, collapses contiguous dims, partitions into tiles for parallel_for_, and hands
// each 2D tile's per-operand slices to a `body` callback (which owns all semantics).

#અન્તર્ભાવ "precomp.hpp"
#અન્તર્ભાવ <algorithm>
#અન્તર્ભાવ <array>
#અન્તર્ભાવ <climits>
#અન્તર્ભાવ <cmath>

નામાકાશ cv {

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
ગણના ChMode { CH_FOLD, CH_DIM, CH_ELEM };

સ્થિર શૂન્ય matLayout(અચલ Mat& m, ChMode mode, MatShape& shp, MatStep& step, પૂર્ણાંક& esz1)
{
    અચલ પૂર્ણાંક nd = m.dims, cn = m.channels();
    જો (mode == CH_ELEM)
    {
        esz1 = (પૂર્ણાંક)m.elemSize();                       // one full (cn-channel) pixel
        shp.resize(nd);
        માટે (પૂર્ણાંક i = 0; i < nd; i++) { shp[i] = m.size[i]; step[i] = m.step[i] / esz1; }
        પાછા;
    }
    esz1 = (પૂર્ણાંક)m.elemSize1();                           // one scalar (channel value)
    જો (mode == CH_DIM)
    {
        shp.resize(nd + 1);
        માટે (પૂર્ણાંક i = 0; i < nd; i++) { shp[i] = m.size[i]; step[i] = m.step[i] / esz1; }
        shp[nd] = cn; step[nd] = 1;                      // channels = explicit innermost dim
    }
    નહીંતર // CH_FOLD
    {
        જો (nd == 0)                                     // 0-dim scalar: channels are the only dim
        {
            shp.assign(1, cn);
            step[0] = 1;
        }
        નહીંતર
        {
            shp.resize(nd);
            માટે (પૂર્ણાંક i = 0; i < nd; i++) { shp[i] = m.size[i]; step[i] = m.step[i] / esz1; }
            shp[nd - 1] *= cn;                           // fold channels into the innermost dim
            step[nd - 1] = 1;                            // scalars are contiguous there
        }
    }
}

// A single-channel scalar (one value, cn==1, total()==1) broadcasts into everything trivially
// (step 0 on every axis incl. channels), so it must NOT force CH_DIM - it is excluded from the
// channel-mode decision entirely.
સ્થિર તાર્કિક isSingleChannelScalar(અચલ Mat& m)
{
    પાછા m.channels() == 1 && m.total() == 1;
}

// Decide (globally, across all operands) how channels are presented for expandChannels=true:
// CH_FOLD when no channel broadcast is needed, CH_DIM when it is. Single-channel scalars are
// excluded first; among the rest, multichannel operands must all share the same cn (an (n,m) mix
// with both > 1 is an error).
સ્થિર ChMode decideChannelMode(અચલ Mat* અચલ* arrays, પૂર્ણાંક K)
{
    પૂર્ણાંક N = 1;                                           // the single multichannel count, if any
    માટે (પૂર્ણાંક k = 0; k < K; k++)
    {
        જો (isSingleChannelScalar(*arrays[k])) ચાલુ;
        પૂર્ણાંક c = arrays[k]->channels();
        જો (c > 1) { જો (N == 1) N = c; નહીંતર CV_Assert(N == c && "ew: (n,m) channel mix unsupported"); }
    }
    જો (N == 1) પાછા CH_FOLD;                          // all single-channel -> fold (a no-op)

    તાર્કિક allMulti = સાચું, sameBack = સાચું;
    પૂર્ણાંક back = -1;
    માટે (પૂર્ણાંક k = 0; k < K; k++)
    {
        અચલ Mat& a = *arrays[k];
        જો (isSingleChannelScalar(a)) ચાલુ;
        જો (a.channels() != N) allMulti = ખોટું;
        પૂર્ણાંક b = a.dims > 0 ? a.size[a.dims - 1] : 1;   // 0-dim scalar has no spatial back (=1)
        જો (back < 0) back = b; નહીંતર જો (b != back) sameBack = ખોટું;
    }
    પાછા (allMulti && sameBack) ? CH_FOLD : CH_DIM;    // fold only if no channel broadcast
}

// numpy-style broadcast of several right-aligned shapes.
સ્થિર તાર્કિક broadcastShape(અચલ MatShape* shps, પૂર્ણાંક K, MatShape& out)
{
    size_t nd = 0;
    માટે (પૂર્ણાંક k = 0; k < K; k++) nd = std::max(nd, shps[k].size());
    out.assign(nd, 1);
    માટે (પૂર્ણાંક k = 0; k < K; k++)
    {
        અચલ MatShape& s = shps[k];
        size_t off = nd - s.size();
        માટે (size_t i = 0; i < s.size(); i++)
        {
            પૂર્ણાંક d = s[i], &o = out[off + i];
            જો (o == 1) o = d;
            નહીંતર જો (d != 1 && d != o) પાછા ખોટું;
        }
    }
    પાછા સાચું;
}

// Right-align an arg's own (shp,step) to nd dims; broadcast dims get step 0.
સ્થિર શૂન્ય alignArg(અચલ MatShape& shp, અચલ MatStep& step, પૂર્ણાંક nd,
                     MatStep& as, MatShape& ash)
{
    as.clear();
    ash.assign(nd, 1);
    પૂર્ણાંક off = nd - (પૂર્ણાંક)shp.size();
    માટે (પૂર્ણાંક i = 0; i < (પૂર્ણાંક)shp.size(); i++)
    {
        પૂર્ણાંક d = shp[i];
        ash[off + i] = d;
        as[off + i] = (d == 1) ? 0 : step[i];
    }
}

// Collapse adjacent dims that are contiguous (and broadcast-consistent) across all args.
સ્થિર પૂર્ણાંક collapseDims(MatStep* S, MatShape* H, પૂર્ણાંક K, MatShape& D)
{
    પૂર્ણાંક nd = (પૂર્ણાંક)D.size();
    જો (nd <= 1) પાછા nd;

    પૂર્ણાંક j = nd - 1;
    માટે (પૂર્ણાંક i = j - 1; i >= 0; i--)
    {
        તાર્કિક contig = સાચું, scalar = સાચું, consist = સાચું;
        માટે (પૂર્ણાંક k = 0; k < K; k++)
        {
            size_t st = S[k][j] * (size_t)H[k][j];
            તાર્કિક prevScalar = H[k][j] == 1;
            તાર્કિક curScalar = H[k][i] == 1;
            contig  = contig  && (st == S[k][i]);
            scalar  = scalar  && curScalar;
            consist = consist && (curScalar == prevScalar);
        }
        જો (contig && (consist || scalar))
        {
            માટે (પૂર્ણાંક k = 0; k < K; k++) H[k][j] *= H[k][i];
            D[j] *= D[i];
        }
        નહીંતર
        {
            j--;
            જો (i < j)
            {
                માટે (પૂર્ણાંક k = 0; k < K; k++) { H[k][j] = H[k][i]; S[k][j] = S[k][i]; }
                D[j] = D[i];
            }
        }
    }

    પૂર્ણાંક m = nd - j;
    માટે (પૂર્ણાંક d = 0; d < m; d++)
    {
        D[d] = D[j + d];
        માટે (પૂર્ણાંક k = 0; k < K; k++) { S[k][d] = S[k][j + d]; H[k][d] = H[k][j + d]; }
    }
    D.resize(m);
    માટે (પૂર્ણાંક k = 0; k < K; k++) H[k].resize(m);

    // Zero out steps of broadcast (size-1) dims (numpy step==0 trick).
    માટે (પૂર્ણાંક d = 0; d < m; d++)
        માટે (પૂર્ણાંક k = 0; k < K; k++)
            જો (H[k][d] == 1) S[k][d] = 0;

    પાછા m;
}

// Fast geometry for the dominant case: every operand is either (a) an array sharing ONE common
// shape - same dims, sizes and channel count - and contiguous, or (b) a single-channel scalar
// (cn==1, total()==1). Then the whole traversal is a single contiguous 1D run of `total` scalars
// (channels folded in): arrays get stepx 1, scalars stepx 0. This skips decideChannelMode /
// broadcastShape / alignArg / collapseDims and all their per-operand buffers entirely. Returns
// false (leaving outputs untouched) when the operands don't fit, so the caller runs general
// geometry. expandChannels=false (CH_ELEM) keeps channels in the element and is left to general.
સ્થિર તાર્કિક fastSameShape(અચલ Mat* અચલ* arrays, પૂર્ણાંક K, તાર્કિક expandChannels,
                          uchar** base, પૂર્ણાંક* esz1, MatStep* S, MatShape& D, પૂર્ણાંક& m)
{
    જો (!expandChannels) પાછા ખોટું;
    પૂર્ણાંક ref = -1;
    માટે (પૂર્ણાંક k = 0; k < K; k++)
        જો (!isSingleChannelScalar(*arrays[k])) { ref = k; તોડો; }
    જો (ref < 0) પાછા ખોટું;                       // all single-channel scalars: let general handle
    અચલ Mat& R = *arrays[ref];
    અચલ પૂર્ણાંક rdims = R.dims, rcn = R.channels();
    માટે (પૂર્ણાંક k = 0; k < K; k++)
    {
        અચલ Mat& a = *arrays[k];
        જો (isSingleChannelScalar(a)) ચાલુ;
        જો (a.channels() != rcn || a.dims != rdims || !a.isContinuous()) પાછા ખોટું;
        માટે (પૂર્ણાંક i = 0; i < rdims; i++) જો (a.size[i] != R.size[i]) પાછા ખોટું;
    }
    અચલ long long total = (long long)R.total() * rcn;   // channels folded into the 1D run
    CV_Assert(total <= (long long)INT_MAX);
    માટે (પૂર્ણાંક k = 0; k < K; k++)
    {
        અચલ Mat& a = *arrays[k];
        base[k] = (uchar*)a.data;
        esz1[k] = (પૂર્ણાંક)a.elemSize1();
        S[k][0] = isSingleChannelScalar(a) ? 0 : 1;
    }
    D.assign(1, (પૂર્ણાંક)total);
    m = 1;
    પાછા સાચું;
}

// ---------------------------------------------------------------------------
// BroadcastOp::run
// ---------------------------------------------------------------------------
// At namespace scope, NOT inside run(): MSVC 2019 loses the constexpr-ness of function-local
// constants used as template arguments inside a lambda (C2975).
સ્થિર constexpr પૂર્ણાંક MAX_DIMS = MatShape::MAX_DIMS;
સ્થિર constexpr પૂર્ણાંક LOCAL_OPS = 8;

શૂન્ય BroadcastOp::run(અચલ Mat* અચલ* arrays, પૂર્ણાંક narrays,
                      અચલ std::function<શૂન્ય(અચલ Tile&)>& body,
                      તાર્કિક expandChannels,
                      દ્વિગુણ nstripes)
{
    અચલ પૂર્ણાંક K = narrays;
    CV_Assert(K >= 1 && arrays != nullptr);

    // ---- 1-3. geometry: per-operand collapsed steps S[k], element sizes esz1[k], base
    //           pointers, and the collapsed iteration shape D (m dims). The fast path handles the
    //           dominant "all same-shape arrays (+ single-channel scalars)" case in one shot; the
    //           general path does decideChannelMode + broadcastShape + align + collapse. ----
    AutoBuffer<MatStep, LOCAL_OPS>  S(K);
    AutoBuffer<પૂર્ણાંક, LOCAL_OPS>      esz1(K);
    AutoBuffer<uchar*, LOCAL_OPS>   base(K);
    MatShape D;
    પૂર્ણાંક m;

    જો (!fastSameShape(arrays, K, expandChannels, base.data(), esz1.data(), S.data(), D, m))
    {
        અચલ ChMode mode = expandChannels ? decideChannelMode(arrays, K) : CH_ELEM;
        AutoBuffer<MatShape, LOCAL_OPS> shp(K);
        AutoBuffer<MatStep, LOCAL_OPS>  stp(K);
        માટે (પૂર્ણાંક k = 0; k < K; k++)
        {
            matLayout(*arrays[k], mode, shp[k], stp[k], esz1[k]);
            base[k] = (uchar*)arrays[k]->data;
        }
        MatShape full;
        CV_Assert(broadcastShape(shp.data(), K, full) && "ew: operands are not broadcast-compatible");
        અચલ પૂર્ણાંક nd = (પૂર્ણાંક)full.size();
        AutoBuffer<MatShape, LOCAL_OPS> H(K);
        માટે (પૂર્ણાંક k = 0; k < K; k++) alignArg(shp[k], stp[k], nd, S[k], H[k]);
        D = full;
        m = collapseDims(S.data(), H.data(), K, D);

        // For a cv::Mat the innermost (channel/last) axis is contiguous, so after collapse the
        // innermost stride is always in {0,1}. No gather, no materialization.
        માટે (પૂર્ણાંક k = 0; k < K; k++)
            CV_Assert(S[k][m - 1] <= 1 && "ew: unexpected innermost stride > 1");
    }

    // ---- 4. inner 2D tile axes: width = D[m-1], height = D[m-2] (if any) ----
    અચલ પૂર્ણાંક wAxis = m - 1;
    અચલ પૂર્ણાંક hAxis = (m >= 2) ? m - 2 : -1;
    અચલ પૂર્ણાંક W   = D[wAxis];
    અચલ પૂર્ણાંક Hgt = (hAxis >= 0) ? D[hAxis] : 1;
    અચલ પૂર્ણાંક nOuter = (hAxis >= 0) ? m - 2 : m - 1;   // outer ("plane") axes = [0 .. nOuter)
    long long nplanes = 1;
    માટે (પૂર્ણાંક d = 0; d < nOuter; d++) nplanes *= D[d];

    // ---- 5. desired parallel stripe count (work hint) ----
    અચલ long long total = nplanes * (long long)Hgt * (long long)W;
    દ્વિગુણ stripes = nstripes;
    જો (stripes <= 0)                                  // broadcastOp can't see the body's cost;
        stripes = (દ્વિગુણ)total * 100.0 / (દ્વિગુણ)(1 << 18);  // assume ~100 cycles/element
    અચલ પૂર્ણાંક wantTiles = std::max(1, (પૂર્ણાંક)std::lround(stripes));

    // ---- 6. tile only for PARALLELISM. broadcastOp is op-agnostic: it does not know the body's
    //         temp-buffer footprint, so it does NOT tile for L1 - that is the body's job (it
    //         re-fragments a tile's width into L1-sized chunks for the fused intermediates).
    //         Start with the largest tile (one 2D block per plane) and split (height first, then
    //         width) only until there are at least `wantTiles` tiles. Bigger tiles => fewer
    //         body/decode calls. Width is G-aligned only in the fully-contiguous (1D) case. ----
    અચલ પૂર્ણાંક G = 16;                  // SIMD/cacheline granule
    પૂર્ણાંક tw = W, th = Hgt;

    auto ntilesOf = [&](પૂર્ણાંક tw_, પૂર્ણાંક th_) {
        long long nw = (W + tw_ - 1) / tw_, nh = (Hgt + th_ - 1) / th_;
        પાછા nplanes * nh * nw;
    };
    long long ntiles = ntilesOf(tw, th);
    જ્યારે (ntiles < wantTiles && th > 1)               // split height for parallelism
    {
        th = (th + 1) / 2;
        ntiles = ntilesOf(tw, th);
    }
    જ્યારે (ntiles < wantTiles && tw > G)               // then split width
    {
        tw = std::max(G, tw / 2);
        જો (hAxis < 0 && tw > G) tw -= tw % G;          // keep width aligned in the 1D case
        ntiles = ntilesOf(tw, th);
    }
    CV_Assert(ntiles <= (long long)INT_MAX);

    અચલ પૂર્ણાંક ntilesW = (W + tw - 1) / tw;
    અચલ પૂર્ણાંક ntilesH = (Hgt + th - 1) / th;

    // ---- 7. execution; decode tile index -> per-operand slices. stepx/stepy are the same for
    //         every tile, so they are set ONCE; only the per-tile base pointer is recomputed. ----
    auto runRange = [&](અચલ Range& r)
    {
        AutoBuffer<Slice, LOCAL_OPS> slices(K);

        // Fast 1D path (m==1: one contiguous axis after collapse, no outer planes, height 1).
        // ntilesH==1 and nplanes==1, so the tile index IS the width-tile index - no div/mod, no
        // plane multi-index decode, no inner step loop. This is the same-shape / fully-contiguous
        // common case.
        જો (m == 1)
        {
            માટે (પૂર્ણાંક k = 0; k < K; k++) { slices[k].stepy = 0; slices[k].stepx = S[k][0]; }
            Tile tile;
            tile.height = 1; tile.narrays = K; tile.slices = slices.data();
            માટે (પૂર્ણાંક t = r.start; t < r.end; t++)
            {
                અચલ પૂર્ણાંક wofs = t * tw, ww = std::min(tw, W - wofs);
                માટે (પૂર્ણાંક k = 0; k < K; k++)
                    slices[k].ptr = base[k] + (size_t)wofs * S[k][0] * (size_t)esz1[k];
                tile.width = ww;
                body(tile);
            }
            પાછા;
        }

        std::array<પૂર્ણાંક, MAX_DIMS> idx;
        માટે (પૂર્ણાંક k = 0; k < K; k++)                     // steps are tile-independent: set once
        {
            slices[k].stepy = (hAxis >= 0) ? S[k][hAxis] : 0;
            slices[k].stepx = S[k][wAxis];
        }
        માટે (પૂર્ણાંક t = r.start; t < r.end; t++)
        {
            પૂર્ણાંક wt = t % ntilesW;
            પૂર્ણાંક rest = t / ntilesW;
            પૂર્ણાંક ht = rest % ntilesH;
            પૂર્ણાંક plane = rest / ntilesH;

            અચલ પૂર્ણાંક wofs = wt * tw, ww = std::min(tw, W - wofs);
            અચલ પૂર્ણાંક hofs = ht * th, hh = std::min(th, Hgt - hofs);

            પૂર્ણાંક p = plane;                              // decode plane -> outer multi-index
            માટે (પૂર્ણાંક d = nOuter - 1; d >= 0; d--) {
                પૂર્ણાંક dd = D[d];
                પૂર્ણાંક np = p / dd;
                idx[d] = p - np * dd;
                p = np;
            }

            માટે (પૂર્ણાંક k = 0; k < K; k++)
            {
                size_t off = (size_t)wofs * S[k][wAxis];
                માટે (પૂર્ણાંક d = 0; d < nOuter; d++) off += (size_t)idx[d] * S[k][d];
                જો (hAxis >= 0) off += (size_t)hofs * S[k][hAxis];
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
    જો (ntiles == 1)
        runRange(Range(0, 1));
    નહીંતર
        parallel_for_(Range(0, (પૂર્ણાંક)ntiles), runRange, stripes);
}

} // namespace cv
