// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

// Implementation of cv::BroadcastOp (declared in opencv2/core/mat.hpp): an op-agnostic driver for
// broadcasting element-wise traversal. It computes the numpy-broadcast iteration space over a flat
// list of operand Mats, collapses contiguous dims, partitions into tiles for parallel_for_, and hands
// each 2D tile's per-operand slices to a `body` callback (which owns all semantics).

#அந்தர்பாவ "precomp.hpp"
#அந்தர்பாவ <algorithm>
#அந்தர்பாவ <array>
#அந்தர்பாவ <climits>
#அந்தர்பாவ <cmath>

நாமாகாஶ cv {

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
எண்ணிக்கை ChMode { CH_FOLD, CH_DIM, CH_ELEM };

நிலையான வெறுமை matLayout(மாறிலி Mat& m, ChMode mode, MatShape& shp, MatStep& step, முழு& esz1)
{
    மாறிலி முழு nd = m.dims, cn = m.channels();
    எனில் (mode == CH_ELEM)
    {
        esz1 = (முழு)m.elemSize();                       // one full (cn-channel) pixel
        shp.resize(nd);
        க்கு (முழு i = 0; i < nd; i++) { shp[i] = m.size[i]; step[i] = m.step[i] / esz1; }
        திரும்பு;
    }
    esz1 = (முழு)m.elemSize1();                           // one scalar (channel value)
    எனில் (mode == CH_DIM)
    {
        shp.resize(nd + 1);
        க்கு (முழு i = 0; i < nd; i++) { shp[i] = m.size[i]; step[i] = m.step[i] / esz1; }
        shp[nd] = cn; step[nd] = 1;                      // channels = explicit innermost dim
    }
    இல்லையெனில் // CH_FOLD
    {
        எனில் (nd == 0)                                     // 0-dim scalar: channels are the only dim
        {
            shp.assign(1, cn);
            step[0] = 1;
        }
        இல்லையெனில்
        {
            shp.resize(nd);
            க்கு (முழு i = 0; i < nd; i++) { shp[i] = m.size[i]; step[i] = m.step[i] / esz1; }
            shp[nd - 1] *= cn;                           // fold channels into the innermost dim
            step[nd - 1] = 1;                            // scalars are contiguous there
        }
    }
}

// A single-channel scalar (one value, cn==1, total()==1) broadcasts into everything trivially
// (step 0 on every axis incl. channels), so it must NOT force CH_DIM - it is excluded from the
// channel-mode decision entirely.
நிலையான தருக்க isSingleChannelScalar(மாறிலி Mat& m)
{
    திரும்பு m.channels() == 1 && m.total() == 1;
}

// Decide (globally, across all operands) how channels are presented for expandChannels=true:
// CH_FOLD when no channel broadcast is needed, CH_DIM when it is. Single-channel scalars are
// excluded first; among the rest, multichannel operands must all share the same cn (an (n,m) mix
// with both > 1 is an error).
நிலையான ChMode decideChannelMode(மாறிலி Mat* மாறிலி* arrays, முழு K)
{
    முழு N = 1;                                           // the single multichannel count, if any
    க்கு (முழு k = 0; k < K; k++)
    {
        எனில் (isSingleChannelScalar(*arrays[k])) தொடர்;
        முழு c = arrays[k]->channels();
        எனில் (c > 1) { எனில் (N == 1) N = c; இல்லையெனில் CV_Assert(N == c && "ew: (n,m) channel mix unsupported"); }
    }
    எனில் (N == 1) திரும்பு CH_FOLD;                          // all single-channel -> fold (a no-op)

    தருக்க allMulti = உண்மை, sameBack = உண்மை;
    முழு back = -1;
    க்கு (முழு k = 0; k < K; k++)
    {
        மாறிலி Mat& a = *arrays[k];
        எனில் (isSingleChannelScalar(a)) தொடர்;
        எனில் (a.channels() != N) allMulti = பொய்;
        முழு b = a.dims > 0 ? a.size[a.dims - 1] : 1;   // 0-dim scalar has no spatial back (=1)
        எனில் (back < 0) back = b; இல்லையெனில் எனில் (b != back) sameBack = பொய்;
    }
    திரும்பு (allMulti && sameBack) ? CH_FOLD : CH_DIM;    // fold only if no channel broadcast
}

// numpy-style broadcast of several right-aligned shapes.
நிலையான தருக்க broadcastShape(மாறிலி MatShape* shps, முழு K, MatShape& out)
{
    size_t nd = 0;
    க்கு (முழு k = 0; k < K; k++) nd = std::max(nd, shps[k].size());
    out.assign(nd, 1);
    க்கு (முழு k = 0; k < K; k++)
    {
        மாறிலி MatShape& s = shps[k];
        size_t off = nd - s.size();
        க்கு (size_t i = 0; i < s.size(); i++)
        {
            முழு d = s[i], &o = out[off + i];
            எனில் (o == 1) o = d;
            இல்லையெனில் எனில் (d != 1 && d != o) திரும்பு பொய்;
        }
    }
    திரும்பு உண்மை;
}

// Right-align an arg's own (shp,step) to nd dims; broadcast dims get step 0.
நிலையான வெறுமை alignArg(மாறிலி MatShape& shp, மாறிலி MatStep& step, முழு nd,
                     MatStep& as, MatShape& ash)
{
    as.clear();
    ash.assign(nd, 1);
    முழு off = nd - (முழு)shp.size();
    க்கு (முழு i = 0; i < (முழு)shp.size(); i++)
    {
        முழு d = shp[i];
        ash[off + i] = d;
        as[off + i] = (d == 1) ? 0 : step[i];
    }
}

// Collapse adjacent dims that are contiguous (and broadcast-consistent) across all args.
நிலையான முழு collapseDims(MatStep* S, MatShape* H, முழு K, MatShape& D)
{
    முழு nd = (முழு)D.size();
    எனில் (nd <= 1) திரும்பு nd;

    முழு j = nd - 1;
    க்கு (முழு i = j - 1; i >= 0; i--)
    {
        தருக்க contig = உண்மை, scalar = உண்மை, consist = உண்மை;
        க்கு (முழு k = 0; k < K; k++)
        {
            size_t st = S[k][j] * (size_t)H[k][j];
            தருக்க prevScalar = H[k][j] == 1;
            தருக்க curScalar = H[k][i] == 1;
            contig  = contig  && (st == S[k][i]);
            scalar  = scalar  && curScalar;
            consist = consist && (curScalar == prevScalar);
        }
        எனில் (contig && (consist || scalar))
        {
            க்கு (முழு k = 0; k < K; k++) H[k][j] *= H[k][i];
            D[j] *= D[i];
        }
        இல்லையெனில்
        {
            j--;
            எனில் (i < j)
            {
                க்கு (முழு k = 0; k < K; k++) { H[k][j] = H[k][i]; S[k][j] = S[k][i]; }
                D[j] = D[i];
            }
        }
    }

    முழு m = nd - j;
    க்கு (முழு d = 0; d < m; d++)
    {
        D[d] = D[j + d];
        க்கு (முழு k = 0; k < K; k++) { S[k][d] = S[k][j + d]; H[k][d] = H[k][j + d]; }
    }
    D.resize(m);
    க்கு (முழு k = 0; k < K; k++) H[k].resize(m);

    // Zero out steps of broadcast (size-1) dims (numpy step==0 trick).
    க்கு (முழு d = 0; d < m; d++)
        க்கு (முழு k = 0; k < K; k++)
            எனில் (H[k][d] == 1) S[k][d] = 0;

    திரும்பு m;
}

// Fast geometry for the dominant case: every operand is either (a) an array sharing ONE common
// shape - same dims, sizes and channel count - and contiguous, or (b) a single-channel scalar
// (cn==1, total()==1). Then the whole traversal is a single contiguous 1D run of `total` scalars
// (channels folded in): arrays get stepx 1, scalars stepx 0. This skips decideChannelMode /
// broadcastShape / alignArg / collapseDims and all their per-operand buffers entirely. Returns
// false (leaving outputs untouched) when the operands don't fit, so the caller runs general
// geometry. expandChannels=false (CH_ELEM) keeps channels in the element and is left to general.
நிலையான தருக்க fastSameShape(மாறிலி Mat* மாறிலி* arrays, முழு K, தருக்க expandChannels,
                          uchar** base, முழு* esz1, MatStep* S, MatShape& D, முழு& m)
{
    எனில் (!expandChannels) திரும்பு பொய்;
    முழு ref = -1;
    க்கு (முழு k = 0; k < K; k++)
        எனில் (!isSingleChannelScalar(*arrays[k])) { ref = k; நிறுத்து; }
    எனில் (ref < 0) திரும்பு பொய்;                       // all single-channel scalars: let general handle
    மாறிலி Mat& R = *arrays[ref];
    மாறிலி முழு rdims = R.dims, rcn = R.channels();
    க்கு (முழு k = 0; k < K; k++)
    {
        மாறிலி Mat& a = *arrays[k];
        எனில் (isSingleChannelScalar(a)) தொடர்;
        எனில் (a.channels() != rcn || a.dims != rdims || !a.isContinuous()) திரும்பு பொய்;
        க்கு (முழு i = 0; i < rdims; i++) எனில் (a.size[i] != R.size[i]) திரும்பு பொய்;
    }
    மாறிலி long long total = (long long)R.total() * rcn;   // channels folded into the 1D run
    CV_Assert(total <= (long long)INT_MAX);
    க்கு (முழு k = 0; k < K; k++)
    {
        மாறிலி Mat& a = *arrays[k];
        base[k] = (uchar*)a.data;
        esz1[k] = (முழு)a.elemSize1();
        S[k][0] = isSingleChannelScalar(a) ? 0 : 1;
    }
    D.assign(1, (முழு)total);
    m = 1;
    திரும்பு உண்மை;
}

// ---------------------------------------------------------------------------
// BroadcastOp::run
// ---------------------------------------------------------------------------
// At namespace scope, NOT inside run(): MSVC 2019 loses the constexpr-ness of function-local
// constants used as template arguments inside a lambda (C2975).
நிலையான constexpr முழு MAX_DIMS = MatShape::MAX_DIMS;
நிலையான constexpr முழு LOCAL_OPS = 8;

வெறுமை BroadcastOp::run(மாறிலி Mat* மாறிலி* arrays, முழு narrays,
                      மாறிலி std::function<வெறுமை(மாறிலி Tile&)>& body,
                      தருக்க expandChannels,
                      இரட்டை nstripes)
{
    மாறிலி முழு K = narrays;
    CV_Assert(K >= 1 && arrays != nullptr);

    // ---- 1-3. geometry: per-operand collapsed steps S[k], element sizes esz1[k], base
    //           pointers, and the collapsed iteration shape D (m dims). The fast path handles the
    //           dominant "all same-shape arrays (+ single-channel scalars)" case in one shot; the
    //           general path does decideChannelMode + broadcastShape + align + collapse. ----
    AutoBuffer<MatStep, LOCAL_OPS>  S(K);
    AutoBuffer<முழு, LOCAL_OPS>      esz1(K);
    AutoBuffer<uchar*, LOCAL_OPS>   base(K);
    MatShape D;
    முழு m;

    எனில் (!fastSameShape(arrays, K, expandChannels, base.data(), esz1.data(), S.data(), D, m))
    {
        மாறிலி ChMode mode = expandChannels ? decideChannelMode(arrays, K) : CH_ELEM;
        AutoBuffer<MatShape, LOCAL_OPS> shp(K);
        AutoBuffer<MatStep, LOCAL_OPS>  stp(K);
        க்கு (முழு k = 0; k < K; k++)
        {
            matLayout(*arrays[k], mode, shp[k], stp[k], esz1[k]);
            base[k] = (uchar*)arrays[k]->data;
        }
        MatShape full;
        CV_Assert(broadcastShape(shp.data(), K, full) && "ew: operands are not broadcast-compatible");
        மாறிலி முழு nd = (முழு)full.size();
        AutoBuffer<MatShape, LOCAL_OPS> H(K);
        க்கு (முழு k = 0; k < K; k++) alignArg(shp[k], stp[k], nd, S[k], H[k]);
        D = full;
        m = collapseDims(S.data(), H.data(), K, D);

        // For a cv::Mat the innermost (channel/last) axis is contiguous, so after collapse the
        // innermost stride is always in {0,1}. No gather, no materialization.
        க்கு (முழு k = 0; k < K; k++)
            CV_Assert(S[k][m - 1] <= 1 && "ew: unexpected innermost stride > 1");
    }

    // ---- 4. inner 2D tile axes: width = D[m-1], height = D[m-2] (if any) ----
    மாறிலி முழு wAxis = m - 1;
    மாறிலி முழு hAxis = (m >= 2) ? m - 2 : -1;
    மாறிலி முழு W   = D[wAxis];
    மாறிலி முழு Hgt = (hAxis >= 0) ? D[hAxis] : 1;
    மாறிலி முழு nOuter = (hAxis >= 0) ? m - 2 : m - 1;   // outer ("plane") axes = [0 .. nOuter)
    long long nplanes = 1;
    க்கு (முழு d = 0; d < nOuter; d++) nplanes *= D[d];

    // ---- 5. desired parallel stripe count (work hint) ----
    மாறிலி long long total = nplanes * (long long)Hgt * (long long)W;
    இரட்டை stripes = nstripes;
    எனில் (stripes <= 0)                                  // broadcastOp can't see the body's cost;
        stripes = (இரட்டை)total * 100.0 / (இரட்டை)(1 << 18);  // assume ~100 cycles/element
    மாறிலி முழு wantTiles = std::max(1, (முழு)std::lround(stripes));

    // ---- 6. tile only for PARALLELISM. broadcastOp is op-agnostic: it does not know the body's
    //         temp-buffer footprint, so it does NOT tile for L1 - that is the body's job (it
    //         re-fragments a tile's width into L1-sized chunks for the fused intermediates).
    //         Start with the largest tile (one 2D block per plane) and split (height first, then
    //         width) only until there are at least `wantTiles` tiles. Bigger tiles => fewer
    //         body/decode calls. Width is G-aligned only in the fully-contiguous (1D) case. ----
    மாறிலி முழு G = 16;                  // SIMD/cacheline granule
    முழு tw = W, th = Hgt;

    auto ntilesOf = [&](முழு tw_, முழு th_) {
        long long nw = (W + tw_ - 1) / tw_, nh = (Hgt + th_ - 1) / th_;
        திரும்பு nplanes * nh * nw;
    };
    long long ntiles = ntilesOf(tw, th);
    போது (ntiles < wantTiles && th > 1)               // split height for parallelism
    {
        th = (th + 1) / 2;
        ntiles = ntilesOf(tw, th);
    }
    போது (ntiles < wantTiles && tw > G)               // then split width
    {
        tw = std::max(G, tw / 2);
        எனில் (hAxis < 0 && tw > G) tw -= tw % G;          // keep width aligned in the 1D case
        ntiles = ntilesOf(tw, th);
    }
    CV_Assert(ntiles <= (long long)INT_MAX);

    மாறிலி முழு ntilesW = (W + tw - 1) / tw;
    மாறிலி முழு ntilesH = (Hgt + th - 1) / th;

    // ---- 7. execution; decode tile index -> per-operand slices. stepx/stepy are the same for
    //         every tile, so they are set ONCE; only the per-tile base pointer is recomputed. ----
    auto runRange = [&](மாறிலி Range& r)
    {
        AutoBuffer<Slice, LOCAL_OPS> slices(K);

        // Fast 1D path (m==1: one contiguous axis after collapse, no outer planes, height 1).
        // ntilesH==1 and nplanes==1, so the tile index IS the width-tile index - no div/mod, no
        // plane multi-index decode, no inner step loop. This is the same-shape / fully-contiguous
        // common case.
        எனில் (m == 1)
        {
            க்கு (முழு k = 0; k < K; k++) { slices[k].stepy = 0; slices[k].stepx = S[k][0]; }
            Tile tile;
            tile.height = 1; tile.narrays = K; tile.slices = slices.data();
            க்கு (முழு t = r.start; t < r.end; t++)
            {
                மாறிலி முழு wofs = t * tw, ww = std::min(tw, W - wofs);
                க்கு (முழு k = 0; k < K; k++)
                    slices[k].ptr = base[k] + (size_t)wofs * S[k][0] * (size_t)esz1[k];
                tile.width = ww;
                body(tile);
            }
            திரும்பு;
        }

        std::array<முழு, MAX_DIMS> idx;
        க்கு (முழு k = 0; k < K; k++)                     // steps are tile-independent: set once
        {
            slices[k].stepy = (hAxis >= 0) ? S[k][hAxis] : 0;
            slices[k].stepx = S[k][wAxis];
        }
        க்கு (முழு t = r.start; t < r.end; t++)
        {
            முழு wt = t % ntilesW;
            முழு rest = t / ntilesW;
            முழு ht = rest % ntilesH;
            முழு plane = rest / ntilesH;

            மாறிலி முழு wofs = wt * tw, ww = std::min(tw, W - wofs);
            மாறிலி முழு hofs = ht * th, hh = std::min(th, Hgt - hofs);

            முழு p = plane;                              // decode plane -> outer multi-index
            க்கு (முழு d = nOuter - 1; d >= 0; d--) {
                முழு dd = D[d];
                முழு np = p / dd;
                idx[d] = p - np * dd;
                p = np;
            }

            க்கு (முழு k = 0; k < K; k++)
            {
                size_t off = (size_t)wofs * S[k][wAxis];
                க்கு (முழு d = 0; d < nOuter; d++) off += (size_t)idx[d] * S[k][d];
                எனில் (hAxis >= 0) off += (size_t)hofs * S[k][hAxis];
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
    எனில் (ntiles == 1)
        runRange(Range(0, 1));
    இல்லையெனில்
        parallel_for_(Range(0, (முழு)ntiles), runRange, stripes);
}

} // namespace cv
