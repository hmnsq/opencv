// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

// Implementation of cv::BroadcastOp (declared in opencv2/core/mat.hpp): an op-agnostic driver for
// broadcasting element-wise traversal. It computes the numpy-broadcast iteration space over a flat
// list of operand Mats, collapses contiguous dims, partitions into tiles for parallel_for_, and hands
// each 2D tile's per-operand slices to a `body` callback (which owns all semantics).

#समावेश "precomp.hpp"
#समावेश <algorithm>
#समावेश <array>
#समावेश <climits>
#समावेश <cmath>

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
क्रमागत ChMode { CH_FOLD, CH_DIM, CH_ELEM };

अटल व्योम matLayout(स्थिर Mat& m, ChMode mode, MatShape& shp, MatStep& step, पूर्णांक& esz1)
{
    स्थिर पूर्णांक nd = m.dims, cn = m.channels();
    अगर (mode == CH_ELEM)
    {
        esz1 = (पूर्णांक)m.elem_आकार();                       // one full (cn-channel) pixel
        shp.resize(nd);
        क्रम (पूर्णांक i = 0; i < nd; i++) { shp[i] = m.size[i]; step[i] = m.step[i] / esz1; }
        वापस;
    }
    esz1 = (पूर्णांक)m.elemSize1();                           // one scalar (channel value)
    अगर (mode == CH_DIM)
    {
        shp.resize(nd + 1);
        क्रम (पूर्णांक i = 0; i < nd; i++) { shp[i] = m.size[i]; step[i] = m.step[i] / esz1; }
        shp[nd] = cn; step[nd] = 1;                      // channels = explicit innermost dim
    }
    अन्यथा // CH_FOLD
    {
        अगर (nd == 0)                                     // 0-dim scalar: channels are the only dim
        {
            shp.assign(1, cn);
            step[0] = 1;
        }
        अन्यथा
        {
            shp.resize(nd);
            क्रम (पूर्णांक i = 0; i < nd; i++) { shp[i] = m.size[i]; step[i] = m.step[i] / esz1; }
            shp[nd - 1] *= cn;                           // fold channels into the innermost dim
            step[nd - 1] = 1;                            // scalars are contiguous there
        }
    }
}

// A single-channel scalar (one value, cn==1, total()==1) broadcasts into everything trivially
// (step 0 on every axis incl. channels), so it must NOT force CH_DIM - it is excluded from the
// channel-mode decision entirely.
अटल bool isSingleChannelScalar(स्थिर Mat& m)
{
    वापस m.channels() == 1 && m.योग() == 1;
}

// Decide (globally, across all operands) how channels are presented for expandChannels=true:
// CH_FOLD when no channel broadcast is needed, CH_DIM when it is. Single-channel scalars are
// excluded first; among the rest, multichannel operands must all share the same cn (an (n,m) mix
// with both > 1 is an error).
अटल ChMode decideChannelMode(स्थिर Mat* स्थिर* arrays, पूर्णांक K)
{
    पूर्णांक N = 1;                                           // the single multichannel count, if any
    क्रम (पूर्णांक k = 0; k < K; k++)
    {
        अगर (isSingleChannelScalar(*arrays[k])) जारी;
        पूर्णांक c = arrays[k]->channels();
        अगर (c > 1) { अगर (N == 1) N = c; अन्यथा CV_Assert(N == c && "ew: (n,m) channel mix unsupported"); }
    }
    अगर (N == 1) वापस CH_FOLD;                          // all single-channel -> fold (a no-op)

    bool allMulti = true, sameBack = true;
    पूर्णांक back = -1;
    क्रम (पूर्णांक k = 0; k < K; k++)
    {
        स्थिर Mat& a = *arrays[k];
        अगर (isSingleChannelScalar(a)) जारी;
        अगर (a.channels() != N) allMulti = false;
        पूर्णांक b = a.dims > 0 ? a.size[a.dims - 1] : 1;   // 0-dim scalar has no spatial back (=1)
        अगर (back < 0) back = b; अन्यथा अगर (b != back) sameBack = false;
    }
    वापस (allMulti && sameBack) ? CH_FOLD : CH_DIM;    // fold only if no channel broadcast
}

// numpy-style broadcast of several right-aligned shapes.
अटल bool broadcastShape(स्थिर MatShape* shps, पूर्णांक K, MatShape& out)
{
    माप_प्रकार nd = 0;
    क्रम (पूर्णांक k = 0; k < K; k++) nd = std::max(nd, shps[k].size());
    out.assign(nd, 1);
    क्रम (पूर्णांक k = 0; k < K; k++)
    {
        स्थिर MatShape& s = shps[k];
        माप_प्रकार off = nd - s.size();
        क्रम (माप_प्रकार i = 0; i < s.size(); i++)
        {
            पूर्णांक d = s[i], &o = out[off + i];
            अगर (o == 1) o = d;
            अन्यथा अगर (d != 1 && d != o) वापस false;
        }
    }
    वापस true;
}

// Right-align an arg's own (shp,step) to nd dims; broadcast dims get step 0.
अटल व्योम alignArg(स्थिर MatShape& shp, स्थिर MatStep& step, पूर्णांक nd,
                     MatStep& as, MatShape& ash)
{
    as.सफा();
    ash.assign(nd, 1);
    पूर्णांक off = nd - (पूर्णांक)shp.size();
    क्रम (पूर्णांक i = 0; i < (पूर्णांक)shp.size(); i++)
    {
        पूर्णांक d = shp[i];
        ash[off + i] = d;
        as[off + i] = (d == 1) ? 0 : step[i];
    }
}

// Collapse adjacent dims that are contiguous (and broadcast-consistent) across all args.
अटल पूर्णांक collapseDims(MatStep* S, MatShape* H, पूर्णांक K, MatShape& D)
{
    पूर्णांक nd = (पूर्णांक)D.size();
    अगर (nd <= 1) वापस nd;

    पूर्णांक j = nd - 1;
    क्रम (पूर्णांक i = j - 1; i >= 0; i--)
    {
        bool contig = true, scalar = true, consist = true;
        क्रम (पूर्णांक k = 0; k < K; k++)
        {
            माप_प्रकार st = S[k][j] * (माप_प्रकार)H[k][j];
            bool पूर्व_Scalar = H[k][j] == 1;
            bool curScalar = H[k][i] == 1;
            contig  = contig  && (st == S[k][i]);
            scalar  = scalar  && curScalar;
            consist = consist && (curScalar == पूर्व_Scalar);
        }
        अगर (contig && (consist || scalar))
        {
            क्रम (पूर्णांक k = 0; k < K; k++) H[k][j] *= H[k][i];
            D[j] *= D[i];
        }
        अन्यथा
        {
            j--;
            अगर (i < j)
            {
                क्रम (पूर्णांक k = 0; k < K; k++) { H[k][j] = H[k][i]; S[k][j] = S[k][i]; }
                D[j] = D[i];
            }
        }
    }

    पूर्णांक m = nd - j;
    क्रम (पूर्णांक d = 0; d < m; d++)
    {
        D[d] = D[j + d];
        क्रम (पूर्णांक k = 0; k < K; k++) { S[k][d] = S[k][j + d]; H[k][d] = H[k][j + d]; }
    }
    D.resize(m);
    क्रम (पूर्णांक k = 0; k < K; k++) H[k].resize(m);

    // Zero out steps of broadcast (size-1) dims (numpy step==0 trick).
    क्रम (पूर्णांक d = 0; d < m; d++)
        क्रम (पूर्णांक k = 0; k < K; k++)
            अगर (H[k][d] == 1) S[k][d] = 0;

    वापस m;
}

// Fast geometry for the dominant case: every operand is either (a) an array sharing ONE common
// shape - same dims, sizes and channel count - and contiguous, or (b) a single-channel scalar
// (cn==1, total()==1). Then the whole traversal is a single contiguous 1D run of `total` scalars
// (channels folded in): arrays get stepx 1, scalars stepx 0. This skips decideChannelMode /
// broadcastShape / alignArg / collapseDims and all their per-operand buffers entirely. Returns
// false (leaving outputs untouched) when the operands don't fit, so the caller runs general
// geometry. expandChannels=false (CH_ELEM) keeps channels in the element and is left to general.
अटल bool fastSameShape(स्थिर Mat* स्थिर* arrays, पूर्णांक K, bool expandChannels,
                          uchar** base, पूर्णांक* esz1, MatStep* S, MatShape& D, पूर्णांक& m)
{
    अगर (!expandChannels) वापस false;
    पूर्णांक ref = -1;
    क्रम (पूर्णांक k = 0; k < K; k++)
        अगर (!isSingleChannelScalar(*arrays[k])) { ref = k; अवरोध; }
    अगर (ref < 0) वापस false;                       // all single-channel scalars: let general handle
    स्थिर Mat& R = *arrays[ref];
    स्थिर पूर्णांक rdims = R.dims, rcn = R.channels();
    क्रम (पूर्णांक k = 0; k < K; k++)
    {
        स्थिर Mat& a = *arrays[k];
        अगर (isSingleChannelScalar(a)) जारी;
        अगर (a.channels() != rcn || a.dims != rdims || !a.isContinuous()) वापस false;
        क्रम (पूर्णांक i = 0; i < rdims; i++) अगर (a.size[i] != R.size[i]) वापस false;
    }
    स्थिर दीर्घ दीर्घ योग = (दीर्घ दीर्घ)R.योग() * rcn;   // channels folded into the 1D run
    CV_Assert(योग <= (दीर्घ दीर्घ)पूर्णांक_उच्च);
    क्रम (पूर्णांक k = 0; k < K; k++)
    {
        स्थिर Mat& a = *arrays[k];
        base[k] = (uchar*)a.तथ्य;
        esz1[k] = (पूर्णांक)a.elemSize1();
        S[k][0] = isSingleChannelScalar(a) ? 0 : 1;
    }
    D.assign(1, (पूर्णांक)योग);
    m = 1;
    वापस true;
}

// ---------------------------------------------------------------------------
// BroadcastOp::run
// ---------------------------------------------------------------------------
// At namespace scope, NOT inside run(): MSVC 2019 loses the constexpr-ness of function-local
// constants used as template arguments inside a lambda (C2975).
अटल constexpr पूर्णांक MAX_DIMS = MatShape::MAX_DIMS;
अटल constexpr पूर्णांक LOCAL_OPS = 8;

व्योम BroadcastOp::run(स्थिर Mat* स्थिर* arrays, पूर्णांक narrays,
                      स्थिर std::function<व्योम(स्थिर Tile&)>& body,
                      bool expandChannels,
                      द्विगुन nstripes)
{
    स्थिर पूर्णांक K = narrays;
    CV_Assert(K >= 1 && arrays != nullptr);

    // ---- 1-3. geometry: per-operand collapsed steps S[k], element sizes esz1[k], base
    //           pointers, and the collapsed iteration shape D (m dims). The fast path handles the
    //           dominant "all same-shape arrays (+ single-channel scalars)" case in one shot; the
    //           general path does decideChannelMode + broadcastShape + align + collapse. ----
    Auto_बफर<MatStep, LOCAL_OPS>  S(K);
    Auto_बफर<पूर्णांक, LOCAL_OPS>      esz1(K);
    Auto_बफर<uchar*, LOCAL_OPS>   base(K);
    MatShape D;
    पूर्णांक m;

    अगर (!fastSameShape(arrays, K, expandChannels, base.तथ्य(), esz1.तथ्य(), S.तथ्य(), D, m))
    {
        स्थिर ChMode mode = expandChannels ? decideChannelMode(arrays, K) : CH_ELEM;
        Auto_बफर<MatShape, LOCAL_OPS> shp(K);
        Auto_बफर<MatStep, LOCAL_OPS>  stp(K);
        क्रम (पूर्णांक k = 0; k < K; k++)
        {
            matLayout(*arrays[k], mode, shp[k], stp[k], esz1[k]);
            base[k] = (uchar*)arrays[k]->तथ्य;
        }
        MatShape full;
        CV_Assert(broadcastShape(shp.तथ्य(), K, full) && "ew: operands are not broadcast-compatible");
        स्थिर पूर्णांक nd = (पूर्णांक)full.size();
        Auto_बफर<MatShape, LOCAL_OPS> H(K);
        क्रम (पूर्णांक k = 0; k < K; k++) alignArg(shp[k], stp[k], nd, S[k], H[k]);
        D = full;
        m = collapseDims(S.तथ्य(), H.तथ्य(), K, D);

        // For a cv::Mat the innermost (channel/last) axis is contiguous, so after collapse the
        // innermost stride is always in {0,1}. No gather, no materialization.
        क्रम (पूर्णांक k = 0; k < K; k++)
            CV_Assert(S[k][m - 1] <= 1 && "ew: unexpected innermost stride > 1");
    }

    // ---- 4. inner 2D tile axes: width = D[m-1], height = D[m-2] (if any) ----
    स्थिर पूर्णांक wAxis = m - 1;
    स्थिर पूर्णांक hAxis = (m >= 2) ? m - 2 : -1;
    स्थिर पूर्णांक W   = D[wAxis];
    स्थिर पूर्णांक Hgt = (hAxis >= 0) ? D[hAxis] : 1;
    स्थिर पूर्णांक nOuter = (hAxis >= 0) ? m - 2 : m - 1;   // outer ("plane") axes = [0 .. nOuter)
    दीर्घ दीर्घ nplanes = 1;
    क्रम (पूर्णांक d = 0; d < nOuter; d++) nplanes *= D[d];

    // ---- 5. desired parallel stripe count (work hint) ----
    स्थिर दीर्घ दीर्घ योग = nplanes * (दीर्घ दीर्घ)Hgt * (दीर्घ दीर्घ)W;
    द्विगुन stripes = nstripes;
    अगर (stripes <= 0)                                  // broadcastOp can't see the body's cost;
        stripes = (द्विगुन)योग * 100.0 / (द्विगुन)(1 << 18);  // assume ~100 cycles/element
    स्थिर पूर्णांक wantTiles = std::max(1, (पूर्णांक)std::lround(stripes));

    // ---- 6. tile only for PARALLELISM. broadcastOp is op-agnostic: it does not know the body's
    //         temp-buffer footprint, so it does NOT tile for L1 - that is the body's job (it
    //         re-fragments a tile's width into L1-sized chunks for the fused intermediates).
    //         Start with the largest tile (one 2D block per plane) and split (height first, then
    //         width) only until there are at least `wantTiles` tiles. Bigger tiles => fewer
    //         body/decode calls. Width is G-aligned only in the fully-contiguous (1D) case. ----
    स्थिर पूर्णांक G = 16;                  // SIMD/cacheline granule
    पूर्णांक tw = W, th = Hgt;

    स्वतः ntilesOf = [&](पूर्णांक tw_, पूर्णांक th_) {
        दीर्घ दीर्घ nw = (W + tw_ - 1) / tw_, nh = (Hgt + th_ - 1) / th_;
        वापस nplanes * nh * nw;
    };
    दीर्घ दीर्घ ntiles = ntilesOf(tw, th);
    जबतक (ntiles < wantTiles && th > 1)               // split height for parallelism
    {
        th = (th + 1) / 2;
        ntiles = ntilesOf(tw, th);
    }
    जबतक (ntiles < wantTiles && tw > G)               // then split width
    {
        tw = std::max(G, tw / 2);
        अगर (hAxis < 0 && tw > G) tw -= tw % G;          // keep width aligned in the 1D case
        ntiles = ntilesOf(tw, th);
    }
    CV_Assert(ntiles <= (दीर्घ दीर्घ)पूर्णांक_उच्च);

    स्थिर पूर्णांक ntilesW = (W + tw - 1) / tw;
    स्थिर पूर्णांक ntilesH = (Hgt + th - 1) / th;

    // ---- 7. execution; decode tile index -> per-operand slices. stepx/stepy are the same for
    //         every tile, so they are set ONCE; only the per-tile base pointer is recomputed. ----
    स्वतः runRange = [&](स्थिर Range& r)
    {
        Auto_बफर<Slice, LOCAL_OPS> slices(K);

        // Fast 1D path (m==1: one contiguous axis after collapse, no outer planes, height 1).
        // ntilesH==1 and nplanes==1, so the tile index IS the width-tile index - no div/mod, no
        // plane multi-index decode, no inner step loop. This is the same-shape / fully-contiguous
        // common case.
        अगर (m == 1)
        {
            क्रम (पूर्णांक k = 0; k < K; k++) { slices[k].stepy = 0; slices[k].stepx = S[k][0]; }
            Tile tile;
            tile.height = 1; tile.narrays = K; tile.slices = slices.तथ्य();
            क्रम (पूर्णांक t = r.start; t < r.end; t++)
            {
                स्थिर पूर्णांक wofs = t * tw, ww = std::min(tw, W - wofs);
                क्रम (पूर्णांक k = 0; k < K; k++)
                    slices[k].ptr = base[k] + (माप_प्रकार)wofs * S[k][0] * (माप_प्रकार)esz1[k];
                tile.width = ww;
                body(tile);
            }
            वापस;
        }

        std::array<पूर्णांक, MAX_DIMS> idx;
        क्रम (पूर्णांक k = 0; k < K; k++)                     // steps are tile-independent: set once
        {
            slices[k].stepy = (hAxis >= 0) ? S[k][hAxis] : 0;
            slices[k].stepx = S[k][wAxis];
        }
        क्रम (पूर्णांक t = r.start; t < r.end; t++)
        {
            पूर्णांक wt = t % ntilesW;
            पूर्णांक rest = t / ntilesW;
            पूर्णांक ht = rest % ntilesH;
            पूर्णांक plane = rest / ntilesH;

            स्थिर पूर्णांक wofs = wt * tw, ww = std::min(tw, W - wofs);
            स्थिर पूर्णांक hofs = ht * th, hh = std::min(th, Hgt - hofs);

            पूर्णांक p = plane;                              // decode plane -> outer multi-index
            क्रम (पूर्णांक d = nOuter - 1; d >= 0; d--) {
                पूर्णांक dd = D[d];
                पूर्णांक np = p / dd;
                idx[d] = p - np * dd;
                p = np;
            }

            क्रम (पूर्णांक k = 0; k < K; k++)
            {
                माप_प्रकार off = (माप_प्रकार)wofs * S[k][wAxis];
                क्रम (पूर्णांक d = 0; d < nOuter; d++) off += (माप_प्रकार)idx[d] * S[k][d];
                अगर (hAxis >= 0) off += (माप_प्रकार)hofs * S[k][hAxis];
                slices[k].ptr = base[k] + off * (माप_प्रकार)esz1[k];
            }

            Tile tile;
            tile.width = ww; tile.height = hh; tile.narrays = K; tile.slices = slices.तथ्य();
            body(tile);
        }
    };

    // Single tile (small work, wantTiles==1) => run inline and skip the parallel framework
    // entirely: its dispatch (std::function wrap + Range machinery + backend hop) is pure
    // overhead when there is nothing to parallelize, and dominates small-array latency.
    अगर (ntiles == 1)
        runRange(Range(0, 1));
    अन्यथा
        parallel_for_(Range(0, (पूर्णांक)ntiles), runRange, stripes);
}

} // namespace cv
