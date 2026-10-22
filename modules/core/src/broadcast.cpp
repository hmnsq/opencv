// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

// Implementation of cv::BroadcastOp (declared in opencv2/core/mat.hpp): an op-agnostic driver for
// broadcasting element-wise traversal. It computes the numpy-broadcast iteration space over a flat
// list of operand Mats, collapses contiguous dims, partitions into tiles for parallel_for_, and hands
// each 2D tile's per-operand slices to a `body` callback (which owns all semantics).

#അന്തര്ഭാവ "precomp.hpp"
#അന്തര്ഭാവ <algorithm>
#അന്തര്ഭാവ <array>
#അന്തര്ഭാവ <climits>
#അന്തര്ഭാവ <cmath>

നാമാകാശ cv {

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
എണ്ണൽ ChMode { CH_FOLD, CH_DIM, CH_ELEM };

സ്ഥിതം ശൂന്യം matLayout(സ്ഥിരം Mat& m, ChMode mode, MatShape& shp, MatStep& step, പൂർണ്ണസംഖ്യ& esz1)
{
    സ്ഥിരം പൂർണ്ണസംഖ്യ nd = m.dims, cn = m.channels();
    എങ്കിൽ (mode == CH_ELEM)
    {
        esz1 = (പൂർണ്ണസംഖ്യ)m.elemSize();                       // one full (cn-channel) pixel
        shp.resize(nd);
        വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < nd; i++) { shp[i] = m.size[i]; step[i] = m.step[i] / esz1; }
        തിരികെ;
    }
    esz1 = (പൂർണ്ണസംഖ്യ)m.elemSize1();                           // one scalar (channel value)
    എങ്കിൽ (mode == CH_DIM)
    {
        shp.resize(nd + 1);
        വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < nd; i++) { shp[i] = m.size[i]; step[i] = m.step[i] / esz1; }
        shp[nd] = cn; step[nd] = 1;                      // channels = explicit innermost dim
    }
    അല്ലെങ്കിൽ // CH_FOLD
    {
        എങ്കിൽ (nd == 0)                                     // 0-dim scalar: channels are the only dim
        {
            shp.assign(1, cn);
            step[0] = 1;
        }
        അല്ലെങ്കിൽ
        {
            shp.resize(nd);
            വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < nd; i++) { shp[i] = m.size[i]; step[i] = m.step[i] / esz1; }
            shp[nd - 1] *= cn;                           // fold channels into the innermost dim
            step[nd - 1] = 1;                            // scalars are contiguous there
        }
    }
}

// A single-channel scalar (one value, cn==1, total()==1) broadcasts into everything trivially
// (step 0 on every axis incl. channels), so it must NOT force CH_DIM - it is excluded from the
// channel-mode decision entirely.
സ്ഥിതം തർക്കം isSingleChannelScalar(സ്ഥിരം Mat& m)
{
    തിരികെ m.channels() == 1 && m.total() == 1;
}

// Decide (globally, across all operands) how channels are presented for expandChannels=true:
// CH_FOLD when no channel broadcast is needed, CH_DIM when it is. Single-channel scalars are
// excluded first; among the rest, multichannel operands must all share the same cn (an (n,m) mix
// with both > 1 is an error).
സ്ഥിതം ChMode decideChannelMode(സ്ഥിരം Mat* സ്ഥിരം* arrays, പൂർണ്ണസംഖ്യ K)
{
    പൂർണ്ണസംഖ്യ N = 1;                                           // the single multichannel count, if any
    വേണ്ടി (പൂർണ്ണസംഖ്യ k = 0; k < K; k++)
    {
        എങ്കിൽ (isSingleChannelScalar(*arrays[k])) തുടരുക;
        പൂർണ്ണസംഖ്യ c = arrays[k]->channels();
        എങ്കിൽ (c > 1) { എങ്കിൽ (N == 1) N = c; അല്ലെങ്കിൽ CV_Assert(N == c && "ew: (n,m) channel mix unsupported"); }
    }
    എങ്കിൽ (N == 1) തിരികെ CH_FOLD;                          // all single-channel -> fold (a no-op)

    തർക്കം allMulti = സത്യം, sameBack = സത്യം;
    പൂർണ്ണസംഖ്യ back = -1;
    വേണ്ടി (പൂർണ്ണസംഖ്യ k = 0; k < K; k++)
    {
        സ്ഥിരം Mat& a = *arrays[k];
        എങ്കിൽ (isSingleChannelScalar(a)) തുടരുക;
        എങ്കിൽ (a.channels() != N) allMulti = അസത്യം;
        പൂർണ്ണസംഖ്യ b = a.dims > 0 ? a.size[a.dims - 1] : 1;   // 0-dim scalar has no spatial back (=1)
        എങ്കിൽ (back < 0) back = b; അല്ലെങ്കിൽ എങ്കിൽ (b != back) sameBack = അസത്യം;
    }
    തിരികെ (allMulti && sameBack) ? CH_FOLD : CH_DIM;    // fold only if no channel broadcast
}

// numpy-style broadcast of several right-aligned shapes.
സ്ഥിതം തർക്കം broadcastShape(സ്ഥിരം MatShape* shps, പൂർണ്ണസംഖ്യ K, MatShape& out)
{
    size_t nd = 0;
    വേണ്ടി (പൂർണ്ണസംഖ്യ k = 0; k < K; k++) nd = std::max(nd, shps[k].size());
    out.assign(nd, 1);
    വേണ്ടി (പൂർണ്ണസംഖ്യ k = 0; k < K; k++)
    {
        സ്ഥിരം MatShape& s = shps[k];
        size_t off = nd - s.size();
        വേണ്ടി (size_t i = 0; i < s.size(); i++)
        {
            പൂർണ്ണസംഖ്യ d = s[i], &o = out[off + i];
            എങ്കിൽ (o == 1) o = d;
            അല്ലെങ്കിൽ എങ്കിൽ (d != 1 && d != o) തിരികെ അസത്യം;
        }
    }
    തിരികെ സത്യം;
}

// Right-align an arg's own (shp,step) to nd dims; broadcast dims get step 0.
സ്ഥിതം ശൂന്യം alignArg(സ്ഥിരം MatShape& shp, സ്ഥിരം MatStep& step, പൂർണ്ണസംഖ്യ nd,
                     MatStep& as, MatShape& ash)
{
    as.clear();
    ash.assign(nd, 1);
    പൂർണ്ണസംഖ്യ off = nd - (പൂർണ്ണസംഖ്യ)shp.size();
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < (പൂർണ്ണസംഖ്യ)shp.size(); i++)
    {
        പൂർണ്ണസംഖ്യ d = shp[i];
        ash[off + i] = d;
        as[off + i] = (d == 1) ? 0 : step[i];
    }
}

// Collapse adjacent dims that are contiguous (and broadcast-consistent) across all args.
സ്ഥിതം പൂർണ്ണസംഖ്യ collapseDims(MatStep* S, MatShape* H, പൂർണ്ണസംഖ്യ K, MatShape& D)
{
    പൂർണ്ണസംഖ്യ nd = (പൂർണ്ണസംഖ്യ)D.size();
    എങ്കിൽ (nd <= 1) തിരികെ nd;

    പൂർണ്ണസംഖ്യ j = nd - 1;
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = j - 1; i >= 0; i--)
    {
        തർക്കം contig = സത്യം, scalar = സത്യം, consist = സത്യം;
        വേണ്ടി (പൂർണ്ണസംഖ്യ k = 0; k < K; k++)
        {
            size_t st = S[k][j] * (size_t)H[k][j];
            തർക്കം prevScalar = H[k][j] == 1;
            തർക്കം curScalar = H[k][i] == 1;
            contig  = contig  && (st == S[k][i]);
            scalar  = scalar  && curScalar;
            consist = consist && (curScalar == prevScalar);
        }
        എങ്കിൽ (contig && (consist || scalar))
        {
            വേണ്ടി (പൂർണ്ണസംഖ്യ k = 0; k < K; k++) H[k][j] *= H[k][i];
            D[j] *= D[i];
        }
        അല്ലെങ്കിൽ
        {
            j--;
            എങ്കിൽ (i < j)
            {
                വേണ്ടി (പൂർണ്ണസംഖ്യ k = 0; k < K; k++) { H[k][j] = H[k][i]; S[k][j] = S[k][i]; }
                D[j] = D[i];
            }
        }
    }

    പൂർണ്ണസംഖ്യ m = nd - j;
    വേണ്ടി (പൂർണ്ണസംഖ്യ d = 0; d < m; d++)
    {
        D[d] = D[j + d];
        വേണ്ടി (പൂർണ്ണസംഖ്യ k = 0; k < K; k++) { S[k][d] = S[k][j + d]; H[k][d] = H[k][j + d]; }
    }
    D.resize(m);
    വേണ്ടി (പൂർണ്ണസംഖ്യ k = 0; k < K; k++) H[k].resize(m);

    // Zero out steps of broadcast (size-1) dims (numpy step==0 trick).
    വേണ്ടി (പൂർണ്ണസംഖ്യ d = 0; d < m; d++)
        വേണ്ടി (പൂർണ്ണസംഖ്യ k = 0; k < K; k++)
            എങ്കിൽ (H[k][d] == 1) S[k][d] = 0;

    തിരികെ m;
}

// Fast geometry for the dominant case: every operand is either (a) an array sharing ONE common
// shape - same dims, sizes and channel count - and contiguous, or (b) a single-channel scalar
// (cn==1, total()==1). Then the whole traversal is a single contiguous 1D run of `total` scalars
// (channels folded in): arrays get stepx 1, scalars stepx 0. This skips decideChannelMode /
// broadcastShape / alignArg / collapseDims and all their per-operand buffers entirely. Returns
// false (leaving outputs untouched) when the operands don't fit, so the caller runs general
// geometry. expandChannels=false (CH_ELEM) keeps channels in the element and is left to general.
സ്ഥിതം തർക്കം fastSameShape(സ്ഥിരം Mat* സ്ഥിരം* arrays, പൂർണ്ണസംഖ്യ K, തർക്കം expandChannels,
                          uchar** base, പൂർണ്ണസംഖ്യ* esz1, MatStep* S, MatShape& D, പൂർണ്ണസംഖ്യ& m)
{
    എങ്കിൽ (!expandChannels) തിരികെ അസത്യം;
    പൂർണ്ണസംഖ്യ ref = -1;
    വേണ്ടി (പൂർണ്ണസംഖ്യ k = 0; k < K; k++)
        എങ്കിൽ (!isSingleChannelScalar(*arrays[k])) { ref = k; നിർത്തുക; }
    എങ്കിൽ (ref < 0) തിരികെ അസത്യം;                       // all single-channel scalars: let general handle
    സ്ഥിരം Mat& R = *arrays[ref];
    സ്ഥിരം പൂർണ്ണസംഖ്യ rdims = R.dims, rcn = R.channels();
    വേണ്ടി (പൂർണ്ണസംഖ്യ k = 0; k < K; k++)
    {
        സ്ഥിരം Mat& a = *arrays[k];
        എങ്കിൽ (isSingleChannelScalar(a)) തുടരുക;
        എങ്കിൽ (a.channels() != rcn || a.dims != rdims || !a.isContinuous()) തിരികെ അസത്യം;
        വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < rdims; i++) എങ്കിൽ (a.size[i] != R.size[i]) തിരികെ അസത്യം;
    }
    സ്ഥിരം long long total = (long long)R.total() * rcn;   // channels folded into the 1D run
    CV_Assert(total <= (long long)INT_MAX);
    വേണ്ടി (പൂർണ്ണസംഖ്യ k = 0; k < K; k++)
    {
        സ്ഥിരം Mat& a = *arrays[k];
        base[k] = (uchar*)a.data;
        esz1[k] = (പൂർണ്ണസംഖ്യ)a.elemSize1();
        S[k][0] = isSingleChannelScalar(a) ? 0 : 1;
    }
    D.assign(1, (പൂർണ്ണസംഖ്യ)total);
    m = 1;
    തിരികെ സത്യം;
}

// ---------------------------------------------------------------------------
// BroadcastOp::run
// ---------------------------------------------------------------------------
// At namespace scope, NOT inside run(): MSVC 2019 loses the constexpr-ness of function-local
// constants used as template arguments inside a lambda (C2975).
സ്ഥിതം constexpr പൂർണ്ണസംഖ്യ MAX_DIMS = MatShape::MAX_DIMS;
സ്ഥിതം constexpr പൂർണ്ണസംഖ്യ LOCAL_OPS = 8;

ശൂന്യം BroadcastOp::run(സ്ഥിരം Mat* സ്ഥിരം* arrays, പൂർണ്ണസംഖ്യ narrays,
                      സ്ഥിരം std::function<ശൂന്യം(സ്ഥിരം Tile&)>& body,
                      തർക്കം expandChannels,
                      ഇരട്ട nstripes)
{
    സ്ഥിരം പൂർണ്ണസംഖ്യ K = narrays;
    CV_Assert(K >= 1 && arrays != nullptr);

    // ---- 1-3. geometry: per-operand collapsed steps S[k], element sizes esz1[k], base
    //           pointers, and the collapsed iteration shape D (m dims). The fast path handles the
    //           dominant "all same-shape arrays (+ single-channel scalars)" case in one shot; the
    //           general path does decideChannelMode + broadcastShape + align + collapse. ----
    AutoBuffer<MatStep, LOCAL_OPS>  S(K);
    AutoBuffer<പൂർണ്ണസംഖ്യ, LOCAL_OPS>      esz1(K);
    AutoBuffer<uchar*, LOCAL_OPS>   base(K);
    MatShape D;
    പൂർണ്ണസംഖ്യ m;

    എങ്കിൽ (!fastSameShape(arrays, K, expandChannels, base.data(), esz1.data(), S.data(), D, m))
    {
        സ്ഥിരം ChMode mode = expandChannels ? decideChannelMode(arrays, K) : CH_ELEM;
        AutoBuffer<MatShape, LOCAL_OPS> shp(K);
        AutoBuffer<MatStep, LOCAL_OPS>  stp(K);
        വേണ്ടി (പൂർണ്ണസംഖ്യ k = 0; k < K; k++)
        {
            matLayout(*arrays[k], mode, shp[k], stp[k], esz1[k]);
            base[k] = (uchar*)arrays[k]->data;
        }
        MatShape full;
        CV_Assert(broadcastShape(shp.data(), K, full) && "ew: operands are not broadcast-compatible");
        സ്ഥിരം പൂർണ്ണസംഖ്യ nd = (പൂർണ്ണസംഖ്യ)full.size();
        AutoBuffer<MatShape, LOCAL_OPS> H(K);
        വേണ്ടി (പൂർണ്ണസംഖ്യ k = 0; k < K; k++) alignArg(shp[k], stp[k], nd, S[k], H[k]);
        D = full;
        m = collapseDims(S.data(), H.data(), K, D);

        // For a cv::Mat the innermost (channel/last) axis is contiguous, so after collapse the
        // innermost stride is always in {0,1}. No gather, no materialization.
        വേണ്ടി (പൂർണ്ണസംഖ്യ k = 0; k < K; k++)
            CV_Assert(S[k][m - 1] <= 1 && "ew: unexpected innermost stride > 1");
    }

    // ---- 4. inner 2D tile axes: width = D[m-1], height = D[m-2] (if any) ----
    സ്ഥിരം പൂർണ്ണസംഖ്യ wAxis = m - 1;
    സ്ഥിരം പൂർണ്ണസംഖ്യ hAxis = (m >= 2) ? m - 2 : -1;
    സ്ഥിരം പൂർണ്ണസംഖ്യ W   = D[wAxis];
    സ്ഥിരം പൂർണ്ണസംഖ്യ Hgt = (hAxis >= 0) ? D[hAxis] : 1;
    സ്ഥിരം പൂർണ്ണസംഖ്യ nOuter = (hAxis >= 0) ? m - 2 : m - 1;   // outer ("plane") axes = [0 .. nOuter)
    long long nplanes = 1;
    വേണ്ടി (പൂർണ്ണസംഖ്യ d = 0; d < nOuter; d++) nplanes *= D[d];

    // ---- 5. desired parallel stripe count (work hint) ----
    സ്ഥിരം long long total = nplanes * (long long)Hgt * (long long)W;
    ഇരട്ട stripes = nstripes;
    എങ്കിൽ (stripes <= 0)                                  // broadcastOp can't see the body's cost;
        stripes = (ഇരട്ട)total * 100.0 / (ഇരട്ട)(1 << 18);  // assume ~100 cycles/element
    സ്ഥിരം പൂർണ്ണസംഖ്യ wantTiles = std::max(1, (പൂർണ്ണസംഖ്യ)std::lround(stripes));

    // ---- 6. tile only for PARALLELISM. broadcastOp is op-agnostic: it does not know the body's
    //         temp-buffer footprint, so it does NOT tile for L1 - that is the body's job (it
    //         re-fragments a tile's width into L1-sized chunks for the fused intermediates).
    //         Start with the largest tile (one 2D block per plane) and split (height first, then
    //         width) only until there are at least `wantTiles` tiles. Bigger tiles => fewer
    //         body/decode calls. Width is G-aligned only in the fully-contiguous (1D) case. ----
    സ്ഥിരം പൂർണ്ണസംഖ്യ G = 16;                  // SIMD/cacheline granule
    പൂർണ്ണസംഖ്യ tw = W, th = Hgt;

    auto ntilesOf = [&](പൂർണ്ണസംഖ്യ tw_, പൂർണ്ണസംഖ്യ th_) {
        long long nw = (W + tw_ - 1) / tw_, nh = (Hgt + th_ - 1) / th_;
        തിരികെ nplanes * nh * nw;
    };
    long long ntiles = ntilesOf(tw, th);
    ആകുമ്പോൾ (ntiles < wantTiles && th > 1)               // split height for parallelism
    {
        th = (th + 1) / 2;
        ntiles = ntilesOf(tw, th);
    }
    ആകുമ്പോൾ (ntiles < wantTiles && tw > G)               // then split width
    {
        tw = std::max(G, tw / 2);
        എങ്കിൽ (hAxis < 0 && tw > G) tw -= tw % G;          // keep width aligned in the 1D case
        ntiles = ntilesOf(tw, th);
    }
    CV_Assert(ntiles <= (long long)INT_MAX);

    സ്ഥിരം പൂർണ്ണസംഖ്യ ntilesW = (W + tw - 1) / tw;
    സ്ഥിരം പൂർണ്ണസംഖ്യ ntilesH = (Hgt + th - 1) / th;

    // ---- 7. execution; decode tile index -> per-operand slices. stepx/stepy are the same for
    //         every tile, so they are set ONCE; only the per-tile base pointer is recomputed. ----
    auto runRange = [&](സ്ഥിരം Range& r)
    {
        AutoBuffer<Slice, LOCAL_OPS> slices(K);

        // Fast 1D path (m==1: one contiguous axis after collapse, no outer planes, height 1).
        // ntilesH==1 and nplanes==1, so the tile index IS the width-tile index - no div/mod, no
        // plane multi-index decode, no inner step loop. This is the same-shape / fully-contiguous
        // common case.
        എങ്കിൽ (m == 1)
        {
            വേണ്ടി (പൂർണ്ണസംഖ്യ k = 0; k < K; k++) { slices[k].stepy = 0; slices[k].stepx = S[k][0]; }
            Tile tile;
            tile.height = 1; tile.narrays = K; tile.slices = slices.data();
            വേണ്ടി (പൂർണ്ണസംഖ്യ t = r.start; t < r.end; t++)
            {
                സ്ഥിരം പൂർണ്ണസംഖ്യ wofs = t * tw, ww = std::min(tw, W - wofs);
                വേണ്ടി (പൂർണ്ണസംഖ്യ k = 0; k < K; k++)
                    slices[k].ptr = base[k] + (size_t)wofs * S[k][0] * (size_t)esz1[k];
                tile.width = ww;
                body(tile);
            }
            തിരികെ;
        }

        std::array<പൂർണ്ണസംഖ്യ, MAX_DIMS> idx;
        വേണ്ടി (പൂർണ്ണസംഖ്യ k = 0; k < K; k++)                     // steps are tile-independent: set once
        {
            slices[k].stepy = (hAxis >= 0) ? S[k][hAxis] : 0;
            slices[k].stepx = S[k][wAxis];
        }
        വേണ്ടി (പൂർണ്ണസംഖ്യ t = r.start; t < r.end; t++)
        {
            പൂർണ്ണസംഖ്യ wt = t % ntilesW;
            പൂർണ്ണസംഖ്യ rest = t / ntilesW;
            പൂർണ്ണസംഖ്യ ht = rest % ntilesH;
            പൂർണ്ണസംഖ്യ plane = rest / ntilesH;

            സ്ഥിരം പൂർണ്ണസംഖ്യ wofs = wt * tw, ww = std::min(tw, W - wofs);
            സ്ഥിരം പൂർണ്ണസംഖ്യ hofs = ht * th, hh = std::min(th, Hgt - hofs);

            പൂർണ്ണസംഖ്യ p = plane;                              // decode plane -> outer multi-index
            വേണ്ടി (പൂർണ്ണസംഖ്യ d = nOuter - 1; d >= 0; d--) {
                പൂർണ്ണസംഖ്യ dd = D[d];
                പൂർണ്ണസംഖ്യ np = p / dd;
                idx[d] = p - np * dd;
                p = np;
            }

            വേണ്ടി (പൂർണ്ണസംഖ്യ k = 0; k < K; k++)
            {
                size_t off = (size_t)wofs * S[k][wAxis];
                വേണ്ടി (പൂർണ്ണസംഖ്യ d = 0; d < nOuter; d++) off += (size_t)idx[d] * S[k][d];
                എങ്കിൽ (hAxis >= 0) off += (size_t)hofs * S[k][hAxis];
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
    എങ്കിൽ (ntiles == 1)
        runRange(Range(0, 1));
    അല്ലെങ്കിൽ
        parallel_for_(Range(0, (പൂർണ്ണസംഖ്യ)ntiles), runRange, stripes);
}

} // namespace cv
