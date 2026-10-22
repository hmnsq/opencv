// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

// Implementation of cv::BroadcastOp (declared in opencv2/core/mat.hpp): an op-agnostic driver for
// broadcasting element-wise traversal. It computes the numpy-broadcast iteration space over a flat
// list of operand Mats, collapses contiguous dims, partitions into tiles for parallel_for_, and hands
// each 2D tile's per-operand slices to a `body` callback (which owns all semantics).

#ଅନ୍ତର୍ଭାଵ "precomp.hpp"
#ଅନ୍ତର୍ଭାଵ <algorithm>
#ଅନ୍ତର୍ଭାଵ <array>
#ଅନ୍ତର୍ଭାଵ <climits>
#ଅନ୍ତର୍ଭାଵ <cmath>

ନାମାକାଶ cv {

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
ଗଣନା ChMode { CH_FOLD, CH_DIM, CH_ELEM };

ସ୍ଥିର ଶୂନ୍ୟ matLayout(ଅଚଳ Mat& m, ChMode mode, MatShape& shp, MatStep& step, ପୂର୍ଣ୍ଣସଂଖ୍ୟା& esz1)
{
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା nd = m.dims, cn = m.channels();
    ଯଦି (mode == CH_ELEM)
    {
        esz1 = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)m.elemSize();                       // one full (cn-channel) pixel
        shp.resize(nd);
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < nd; i++) { shp[i] = m.size[i]; step[i] = m.step[i] / esz1; }
        ଫେରନ୍ତୁ;
    }
    esz1 = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)m.elemSize1();                           // one scalar (channel value)
    ଯଦି (mode == CH_DIM)
    {
        shp.resize(nd + 1);
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < nd; i++) { shp[i] = m.size[i]; step[i] = m.step[i] / esz1; }
        shp[nd] = cn; step[nd] = 1;                      // channels = explicit innermost dim
    }
    ଅନ୍ୟଥା // CH_FOLD
    {
        ଯଦି (nd == 0)                                     // 0-dim scalar: channels are the only dim
        {
            shp.assign(1, cn);
            step[0] = 1;
        }
        ଅନ୍ୟଥା
        {
            shp.resize(nd);
            ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < nd; i++) { shp[i] = m.size[i]; step[i] = m.step[i] / esz1; }
            shp[nd - 1] *= cn;                           // fold channels into the innermost dim
            step[nd - 1] = 1;                            // scalars are contiguous there
        }
    }
}

// A single-channel scalar (one value, cn==1, total()==1) broadcasts into everything trivially
// (step 0 on every axis incl. channels), so it must NOT force CH_DIM - it is excluded from the
// channel-mode decision entirely.
ସ୍ଥିର ତାର୍କିକ isSingleChannelScalar(ଅଚଳ Mat& m)
{
    ଫେରନ୍ତୁ m.channels() == 1 && m.total() == 1;
}

// Decide (globally, across all operands) how channels are presented for expandChannels=true:
// CH_FOLD when no channel broadcast is needed, CH_DIM when it is. Single-channel scalars are
// excluded first; among the rest, multichannel operands must all share the same cn (an (n,m) mix
// with both > 1 is an error).
ସ୍ଥିର ChMode decideChannelMode(ଅଚଳ Mat* ଅଚଳ* arrays, ପୂର୍ଣ୍ଣସଂଖ୍ୟା K)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା N = 1;                                           // the single multichannel count, if any
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0; k < K; k++)
    {
        ଯଦି (isSingleChannelScalar(*arrays[k])) ଜାରି;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା c = arrays[k]->channels();
        ଯଦି (c > 1) { ଯଦି (N == 1) N = c; ଅନ୍ୟଥା CV_Assert(N == c && "ew: (n,m) channel mix unsupported"); }
    }
    ଯଦି (N == 1) ଫେରନ୍ତୁ CH_FOLD;                          // all single-channel -> fold (a no-op)

    ତାର୍କିକ allMulti = ସତ୍ୟ, sameBack = ସତ୍ୟ;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା back = -1;
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0; k < K; k++)
    {
        ଅଚଳ Mat& a = *arrays[k];
        ଯଦି (isSingleChannelScalar(a)) ଜାରି;
        ଯଦି (a.channels() != N) allMulti = ମିଥ୍ୟା;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା b = a.dims > 0 ? a.size[a.dims - 1] : 1;   // 0-dim scalar has no spatial back (=1)
        ଯଦି (back < 0) back = b; ଅନ୍ୟଥା ଯଦି (b != back) sameBack = ମିଥ୍ୟା;
    }
    ଫେରନ୍ତୁ (allMulti && sameBack) ? CH_FOLD : CH_DIM;    // fold only if no channel broadcast
}

// numpy-style broadcast of several right-aligned shapes.
ସ୍ଥିର ତାର୍କିକ broadcastShape(ଅଚଳ MatShape* shps, ପୂର୍ଣ୍ଣସଂଖ୍ୟା K, MatShape& out)
{
    size_t nd = 0;
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0; k < K; k++) nd = std::max(nd, shps[k].size());
    out.assign(nd, 1);
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0; k < K; k++)
    {
        ଅଚଳ MatShape& s = shps[k];
        size_t off = nd - s.size();
        ପାଇଁ (size_t i = 0; i < s.size(); i++)
        {
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା d = s[i], &o = out[off + i];
            ଯଦି (o == 1) o = d;
            ଅନ୍ୟଥା ଯଦି (d != 1 && d != o) ଫେରନ୍ତୁ ମିଥ୍ୟା;
        }
    }
    ଫେରନ୍ତୁ ସତ୍ୟ;
}

// Right-align an arg's own (shp,step) to nd dims; broadcast dims get step 0.
ସ୍ଥିର ଶୂନ୍ୟ alignArg(ଅଚଳ MatShape& shp, ଅଚଳ MatStep& step, ପୂର୍ଣ୍ଣସଂଖ୍ୟା nd,
                     MatStep& as, MatShape& ash)
{
    as.clear();
    ash.assign(nd, 1);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା off = nd - (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)shp.size();
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)shp.size(); i++)
    {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା d = shp[i];
        ash[off + i] = d;
        as[off + i] = (d == 1) ? 0 : step[i];
    }
}

// Collapse adjacent dims that are contiguous (and broadcast-consistent) across all args.
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା collapseDims(MatStep* S, MatShape* H, ପୂର୍ଣ୍ଣସଂଖ୍ୟା K, MatShape& D)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା nd = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)D.size();
    ଯଦି (nd <= 1) ଫେରନ୍ତୁ nd;

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = nd - 1;
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = j - 1; i >= 0; i--)
    {
        ତାର୍କିକ contig = ସତ୍ୟ, scalar = ସତ୍ୟ, consist = ସତ୍ୟ;
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0; k < K; k++)
        {
            size_t st = S[k][j] * (size_t)H[k][j];
            ତାର୍କିକ prevScalar = H[k][j] == 1;
            ତାର୍କିକ curScalar = H[k][i] == 1;
            contig  = contig  && (st == S[k][i]);
            scalar  = scalar  && curScalar;
            consist = consist && (curScalar == prevScalar);
        }
        ଯଦି (contig && (consist || scalar))
        {
            ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0; k < K; k++) H[k][j] *= H[k][i];
            D[j] *= D[i];
        }
        ଅନ୍ୟଥା
        {
            j--;
            ଯଦି (i < j)
            {
                ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0; k < K; k++) { H[k][j] = H[k][i]; S[k][j] = S[k][i]; }
                D[j] = D[i];
            }
        }
    }

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା m = nd - j;
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା d = 0; d < m; d++)
    {
        D[d] = D[j + d];
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0; k < K; k++) { S[k][d] = S[k][j + d]; H[k][d] = H[k][j + d]; }
    }
    D.resize(m);
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0; k < K; k++) H[k].resize(m);

    // Zero out steps of broadcast (size-1) dims (numpy step==0 trick).
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା d = 0; d < m; d++)
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0; k < K; k++)
            ଯଦି (H[k][d] == 1) S[k][d] = 0;

    ଫେରନ୍ତୁ m;
}

// Fast geometry for the dominant case: every operand is either (a) an array sharing ONE common
// shape - same dims, sizes and channel count - and contiguous, or (b) a single-channel scalar
// (cn==1, total()==1). Then the whole traversal is a single contiguous 1D run of `total` scalars
// (channels folded in): arrays get stepx 1, scalars stepx 0. This skips decideChannelMode /
// broadcastShape / alignArg / collapseDims and all their per-operand buffers entirely. Returns
// false (leaving outputs untouched) when the operands don't fit, so the caller runs general
// geometry. expandChannels=false (CH_ELEM) keeps channels in the element and is left to general.
ସ୍ଥିର ତାର୍କିକ fastSameShape(ଅଚଳ Mat* ଅଚଳ* arrays, ପୂର୍ଣ୍ଣସଂଖ୍ୟା K, ତାର୍କିକ expandChannels,
                          uchar** base, ପୂର୍ଣ୍ଣସଂଖ୍ୟା* esz1, MatStep* S, MatShape& D, ପୂର୍ଣ୍ଣସଂଖ୍ୟା& m)
{
    ଯଦି (!expandChannels) ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ref = -1;
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0; k < K; k++)
        ଯଦି (!isSingleChannelScalar(*arrays[k])) { ref = k; ଭାଙ୍ଗ; }
    ଯଦି (ref < 0) ଫେରନ୍ତୁ ମିଥ୍ୟା;                       // all single-channel scalars: let general handle
    ଅଚଳ Mat& R = *arrays[ref];
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା rdims = R.dims, rcn = R.channels();
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0; k < K; k++)
    {
        ଅଚଳ Mat& a = *arrays[k];
        ଯଦି (isSingleChannelScalar(a)) ଜାରି;
        ଯଦି (a.channels() != rcn || a.dims != rdims || !a.isContinuous()) ଫେରନ୍ତୁ ମିଥ୍ୟା;
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < rdims; i++) ଯଦି (a.size[i] != R.size[i]) ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }
    ଅଚଳ long long total = (long long)R.total() * rcn;   // channels folded into the 1D run
    CV_Assert(total <= (long long)INT_MAX);
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0; k < K; k++)
    {
        ଅଚଳ Mat& a = *arrays[k];
        base[k] = (uchar*)a.data;
        esz1[k] = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)a.elemSize1();
        S[k][0] = isSingleChannelScalar(a) ? 0 : 1;
    }
    D.assign(1, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)total);
    m = 1;
    ଫେରନ୍ତୁ ସତ୍ୟ;
}

// ---------------------------------------------------------------------------
// BroadcastOp::run
// ---------------------------------------------------------------------------
// At namespace scope, NOT inside run(): MSVC 2019 loses the constexpr-ness of function-local
// constants used as template arguments inside a lambda (C2975).
ସ୍ଥିର constexpr ପୂର୍ଣ୍ଣସଂଖ୍ୟା MAX_DIMS = MatShape::MAX_DIMS;
ସ୍ଥିର constexpr ପୂର୍ଣ୍ଣସଂଖ୍ୟା LOCAL_OPS = 8;

ଶୂନ୍ୟ BroadcastOp::run(ଅଚଳ Mat* ଅଚଳ* arrays, ପୂର୍ଣ୍ଣସଂଖ୍ୟା narrays,
                      ଅଚଳ std::function<ଶୂନ୍ୟ(ଅଚଳ Tile&)>& body,
                      ତାର୍କିକ expandChannels,
                      ଦ୍ୱିଗୁଣ nstripes)
{
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା K = narrays;
    CV_Assert(K >= 1 && arrays != nullptr);

    // ---- 1-3. geometry: per-operand collapsed steps S[k], element sizes esz1[k], base
    //           pointers, and the collapsed iteration shape D (m dims). The fast path handles the
    //           dominant "all same-shape arrays (+ single-channel scalars)" case in one shot; the
    //           general path does decideChannelMode + broadcastShape + align + collapse. ----
    AutoBuffer<MatStep, LOCAL_OPS>  S(K);
    AutoBuffer<ପୂର୍ଣ୍ଣସଂଖ୍ୟା, LOCAL_OPS>      esz1(K);
    AutoBuffer<uchar*, LOCAL_OPS>   base(K);
    MatShape D;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା m;

    ଯଦି (!fastSameShape(arrays, K, expandChannels, base.data(), esz1.data(), S.data(), D, m))
    {
        ଅଚଳ ChMode mode = expandChannels ? decideChannelMode(arrays, K) : CH_ELEM;
        AutoBuffer<MatShape, LOCAL_OPS> shp(K);
        AutoBuffer<MatStep, LOCAL_OPS>  stp(K);
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0; k < K; k++)
        {
            matLayout(*arrays[k], mode, shp[k], stp[k], esz1[k]);
            base[k] = (uchar*)arrays[k]->data;
        }
        MatShape full;
        CV_Assert(broadcastShape(shp.data(), K, full) && "ew: operands are not broadcast-compatible");
        ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା nd = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)full.size();
        AutoBuffer<MatShape, LOCAL_OPS> H(K);
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0; k < K; k++) alignArg(shp[k], stp[k], nd, S[k], H[k]);
        D = full;
        m = collapseDims(S.data(), H.data(), K, D);

        // For a cv::Mat the innermost (channel/last) axis is contiguous, so after collapse the
        // innermost stride is always in {0,1}. No gather, no materialization.
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0; k < K; k++)
            CV_Assert(S[k][m - 1] <= 1 && "ew: unexpected innermost stride > 1");
    }

    // ---- 4. inner 2D tile axes: width = D[m-1], height = D[m-2] (if any) ----
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା wAxis = m - 1;
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା hAxis = (m >= 2) ? m - 2 : -1;
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା W   = D[wAxis];
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା Hgt = (hAxis >= 0) ? D[hAxis] : 1;
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା nOuter = (hAxis >= 0) ? m - 2 : m - 1;   // outer ("plane") axes = [0 .. nOuter)
    long long nplanes = 1;
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା d = 0; d < nOuter; d++) nplanes *= D[d];

    // ---- 5. desired parallel stripe count (work hint) ----
    ଅଚଳ long long total = nplanes * (long long)Hgt * (long long)W;
    ଦ୍ୱିଗୁଣ stripes = nstripes;
    ଯଦି (stripes <= 0)                                  // broadcastOp can't see the body's cost;
        stripes = (ଦ୍ୱିଗୁଣ)total * 100.0 / (ଦ୍ୱିଗୁଣ)(1 << 18);  // assume ~100 cycles/element
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା wantTiles = std::max(1, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)std::lround(stripes));

    // ---- 6. tile only for PARALLELISM. broadcastOp is op-agnostic: it does not know the body's
    //         temp-buffer footprint, so it does NOT tile for L1 - that is the body's job (it
    //         re-fragments a tile's width into L1-sized chunks for the fused intermediates).
    //         Start with the largest tile (one 2D block per plane) and split (height first, then
    //         width) only until there are at least `wantTiles` tiles. Bigger tiles => fewer
    //         body/decode calls. Width is G-aligned only in the fully-contiguous (1D) case. ----
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା G = 16;                  // SIMD/cacheline granule
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା tw = W, th = Hgt;

    auto ntilesOf = [&](ପୂର୍ଣ୍ଣସଂଖ୍ୟା tw_, ପୂର୍ଣ୍ଣସଂଖ୍ୟା th_) {
        long long nw = (W + tw_ - 1) / tw_, nh = (Hgt + th_ - 1) / th_;
        ଫେରନ୍ତୁ nplanes * nh * nw;
    };
    long long ntiles = ntilesOf(tw, th);
    ଯେତେବେଳେ (ntiles < wantTiles && th > 1)               // split height for parallelism
    {
        th = (th + 1) / 2;
        ntiles = ntilesOf(tw, th);
    }
    ଯେତେବେଳେ (ntiles < wantTiles && tw > G)               // then split width
    {
        tw = std::max(G, tw / 2);
        ଯଦି (hAxis < 0 && tw > G) tw -= tw % G;          // keep width aligned in the 1D case
        ntiles = ntilesOf(tw, th);
    }
    CV_Assert(ntiles <= (long long)INT_MAX);

    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା ntilesW = (W + tw - 1) / tw;
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା ntilesH = (Hgt + th - 1) / th;

    // ---- 7. execution; decode tile index -> per-operand slices. stepx/stepy are the same for
    //         every tile, so they are set ONCE; only the per-tile base pointer is recomputed. ----
    auto runRange = [&](ଅଚଳ Range& r)
    {
        AutoBuffer<Slice, LOCAL_OPS> slices(K);

        // Fast 1D path (m==1: one contiguous axis after collapse, no outer planes, height 1).
        // ntilesH==1 and nplanes==1, so the tile index IS the width-tile index - no div/mod, no
        // plane multi-index decode, no inner step loop. This is the same-shape / fully-contiguous
        // common case.
        ଯଦି (m == 1)
        {
            ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0; k < K; k++) { slices[k].stepy = 0; slices[k].stepx = S[k][0]; }
            Tile tile;
            tile.height = 1; tile.narrays = K; tile.slices = slices.data();
            ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା t = r.start; t < r.end; t++)
            {
                ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା wofs = t * tw, ww = std::min(tw, W - wofs);
                ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0; k < K; k++)
                    slices[k].ptr = base[k] + (size_t)wofs * S[k][0] * (size_t)esz1[k];
                tile.width = ww;
                body(tile);
            }
            ଫେରନ୍ତୁ;
        }

        std::array<ପୂର୍ଣ୍ଣସଂଖ୍ୟା, MAX_DIMS> idx;
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0; k < K; k++)                     // steps are tile-independent: set once
        {
            slices[k].stepy = (hAxis >= 0) ? S[k][hAxis] : 0;
            slices[k].stepx = S[k][wAxis];
        }
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା t = r.start; t < r.end; t++)
        {
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା wt = t % ntilesW;
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା rest = t / ntilesW;
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା ht = rest % ntilesH;
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା plane = rest / ntilesH;

            ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା wofs = wt * tw, ww = std::min(tw, W - wofs);
            ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା hofs = ht * th, hh = std::min(th, Hgt - hofs);

            ପୂର୍ଣ୍ଣସଂଖ୍ୟା p = plane;                              // decode plane -> outer multi-index
            ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା d = nOuter - 1; d >= 0; d--) {
                ପୂର୍ଣ୍ଣସଂଖ୍ୟା dd = D[d];
                ପୂର୍ଣ୍ଣସଂଖ୍ୟା np = p / dd;
                idx[d] = p - np * dd;
                p = np;
            }

            ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0; k < K; k++)
            {
                size_t off = (size_t)wofs * S[k][wAxis];
                ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା d = 0; d < nOuter; d++) off += (size_t)idx[d] * S[k][d];
                ଯଦି (hAxis >= 0) off += (size_t)hofs * S[k][hAxis];
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
    ଯଦି (ntiles == 1)
        runRange(Range(0, 1));
    ଅନ୍ୟଥା
        parallel_for_(Range(0, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)ntiles), runRange, stripes);
}

} // namespace cv
