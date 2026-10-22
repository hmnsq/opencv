// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

// Implementation of cv::BroadcastOp (declared in opencv2/core/mat.hpp): an op-agnostic driver for
// broadcasting element-wise traversal. It computes the numpy-broadcast iteration space over a flat
// list of operand Mats, collapses contiguous dims, partitions into tiles for parallel_for_, and hands
// each 2D tile's per-operand slices to a `body` callback (which owns all semantics).

#अन्तर्भाव "precomp.hpp"
#अन्तर्भाव <algorithm>
#अन्तर्भाव <array>
#अन्तर्भाव <climits>
#अन्तर्भाव <cmath>

नामाकाश cv {

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
गणना ChMode { CH_FOLD, CH_DIM, CH_ELEM };

स्थिर शून्य matLayout(स्थिरांक Mat& m, ChMode mode, MatShape& shp, MatStep& step, पूर्णांक& esz1)
{
    स्थिरांक पूर्णांक nd = m.dims, cn = m.channels();
    जर (mode == CH_ELEM)
    {
        esz1 = (पूर्णांक)m.elemSize();                       // one full (cn-channel) pixel
        shp.resize(nd);
        साठी (पूर्णांक i = 0; i < nd; i++) { shp[i] = m.size[i]; step[i] = m.step[i] / esz1; }
        परत;
    }
    esz1 = (पूर्णांक)m.elemSize1();                           // one scalar (channel value)
    जर (mode == CH_DIM)
    {
        shp.resize(nd + 1);
        साठी (पूर्णांक i = 0; i < nd; i++) { shp[i] = m.size[i]; step[i] = m.step[i] / esz1; }
        shp[nd] = cn; step[nd] = 1;                      // channels = explicit innermost dim
    }
    अन्यथा // CH_FOLD
    {
        जर (nd == 0)                                     // 0-dim scalar: channels are the only dim
        {
            shp.assign(1, cn);
            step[0] = 1;
        }
        अन्यथा
        {
            shp.resize(nd);
            साठी (पूर्णांक i = 0; i < nd; i++) { shp[i] = m.size[i]; step[i] = m.step[i] / esz1; }
            shp[nd - 1] *= cn;                           // fold channels into the innermost dim
            step[nd - 1] = 1;                            // scalars are contiguous there
        }
    }
}

// A single-channel scalar (one value, cn==1, total()==1) broadcasts into everything trivially
// (step 0 on every axis incl. channels), so it must NOT force CH_DIM - it is excluded from the
// channel-mode decision entirely.
स्थिर तार्किक isSingleChannelScalar(स्थिरांक Mat& m)
{
    परत m.channels() == 1 && m.total() == 1;
}

// Decide (globally, across all operands) how channels are presented for expandChannels=true:
// CH_FOLD when no channel broadcast is needed, CH_DIM when it is. Single-channel scalars are
// excluded first; among the rest, multichannel operands must all share the same cn (an (n,m) mix
// with both > 1 is an error).
स्थिर ChMode decideChannelMode(स्थिरांक Mat* स्थिरांक* arrays, पूर्णांक K)
{
    पूर्णांक N = 1;                                           // the single multichannel count, if any
    साठी (पूर्णांक k = 0; k < K; k++)
    {
        जर (isSingleChannelScalar(*arrays[k])) सुरूठेवा;
        पूर्णांक c = arrays[k]->channels();
        जर (c > 1) { जर (N == 1) N = c; अन्यथा CV_Assert(N == c && "ew: (n,m) channel mix unsupported"); }
    }
    जर (N == 1) परत CH_FOLD;                          // all single-channel -> fold (a no-op)

    तार्किक allMulti = सत्य, sameBack = सत्य;
    पूर्णांक back = -1;
    साठी (पूर्णांक k = 0; k < K; k++)
    {
        स्थिरांक Mat& a = *arrays[k];
        जर (isSingleChannelScalar(a)) सुरूठेवा;
        जर (a.channels() != N) allMulti = असत्य;
        पूर्णांक b = a.dims > 0 ? a.size[a.dims - 1] : 1;   // 0-dim scalar has no spatial back (=1)
        जर (back < 0) back = b; अन्यथा जर (b != back) sameBack = असत्य;
    }
    परत (allMulti && sameBack) ? CH_FOLD : CH_DIM;    // fold only if no channel broadcast
}

// numpy-style broadcast of several right-aligned shapes.
स्थिर तार्किक broadcastShape(स्थिरांक MatShape* shps, पूर्णांक K, MatShape& out)
{
    माप_प्रकार nd = 0;
    साठी (पूर्णांक k = 0; k < K; k++) nd = std::max(nd, shps[k].size());
    out.assign(nd, 1);
    साठी (पूर्णांक k = 0; k < K; k++)
    {
        स्थिरांक MatShape& s = shps[k];
        माप_प्रकार off = nd - s.size();
        साठी (माप_प्रकार i = 0; i < s.size(); i++)
        {
            पूर्णांक d = s[i], &o = out[off + i];
            जर (o == 1) o = d;
            अन्यथा जर (d != 1 && d != o) परत असत्य;
        }
    }
    परत सत्य;
}

// Right-align an arg's own (shp,step) to nd dims; broadcast dims get step 0.
स्थिर शून्य alignArg(स्थिरांक MatShape& shp, स्थिरांक MatStep& step, पूर्णांक nd,
                     MatStep& as, MatShape& ash)
{
    as.clear();
    ash.assign(nd, 1);
    पूर्णांक off = nd - (पूर्णांक)shp.size();
    साठी (पूर्णांक i = 0; i < (पूर्णांक)shp.size(); i++)
    {
        पूर्णांक d = shp[i];
        ash[off + i] = d;
        as[off + i] = (d == 1) ? 0 : step[i];
    }
}

// Collapse adjacent dims that are contiguous (and broadcast-consistent) across all args.
स्थिर पूर्णांक collapseDims(MatStep* S, MatShape* H, पूर्णांक K, MatShape& D)
{
    पूर्णांक nd = (पूर्णांक)D.size();
    जर (nd <= 1) परत nd;

    पूर्णांक j = nd - 1;
    साठी (पूर्णांक i = j - 1; i >= 0; i--)
    {
        तार्किक contig = सत्य, scalar = सत्य, consist = सत्य;
        साठी (पूर्णांक k = 0; k < K; k++)
        {
            माप_प्रकार st = S[k][j] * (माप_प्रकार)H[k][j];
            तार्किक prevScalar = H[k][j] == 1;
            तार्किक curScalar = H[k][i] == 1;
            contig  = contig  && (st == S[k][i]);
            scalar  = scalar  && curScalar;
            consist = consist && (curScalar == prevScalar);
        }
        जर (contig && (consist || scalar))
        {
            साठी (पूर्णांक k = 0; k < K; k++) H[k][j] *= H[k][i];
            D[j] *= D[i];
        }
        अन्यथा
        {
            j--;
            जर (i < j)
            {
                साठी (पूर्णांक k = 0; k < K; k++) { H[k][j] = H[k][i]; S[k][j] = S[k][i]; }
                D[j] = D[i];
            }
        }
    }

    पूर्णांक m = nd - j;
    साठी (पूर्णांक d = 0; d < m; d++)
    {
        D[d] = D[j + d];
        साठी (पूर्णांक k = 0; k < K; k++) { S[k][d] = S[k][j + d]; H[k][d] = H[k][j + d]; }
    }
    D.resize(m);
    साठी (पूर्णांक k = 0; k < K; k++) H[k].resize(m);

    // Zero out steps of broadcast (size-1) dims (numpy step==0 trick).
    साठी (पूर्णांक d = 0; d < m; d++)
        साठी (पूर्णांक k = 0; k < K; k++)
            जर (H[k][d] == 1) S[k][d] = 0;

    परत m;
}

// Fast geometry for the dominant case: every operand is either (a) an array sharing ONE common
// shape - same dims, sizes and channel count - and contiguous, or (b) a single-channel scalar
// (cn==1, total()==1). Then the whole traversal is a single contiguous 1D run of `total` scalars
// (channels folded in): arrays get stepx 1, scalars stepx 0. This skips decideChannelMode /
// broadcastShape / alignArg / collapseDims and all their per-operand buffers entirely. Returns
// false (leaving outputs untouched) when the operands don't fit, so the caller runs general
// geometry. expandChannels=false (CH_ELEM) keeps channels in the element and is left to general.
स्थिर तार्किक fastSameShape(स्थिरांक Mat* स्थिरांक* arrays, पूर्णांक K, तार्किक expandChannels,
                          uchar** base, पूर्णांक* esz1, MatStep* S, MatShape& D, पूर्णांक& m)
{
    जर (!expandChannels) परत असत्य;
    पूर्णांक ref = -1;
    साठी (पूर्णांक k = 0; k < K; k++)
        जर (!isSingleChannelScalar(*arrays[k])) { ref = k; थांबा; }
    जर (ref < 0) परत असत्य;                       // all single-channel scalars: let general handle
    स्थिरांक Mat& R = *arrays[ref];
    स्थिरांक पूर्णांक rdims = R.dims, rcn = R.channels();
    साठी (पूर्णांक k = 0; k < K; k++)
    {
        स्थिरांक Mat& a = *arrays[k];
        जर (isSingleChannelScalar(a)) सुरूठेवा;
        जर (a.channels() != rcn || a.dims != rdims || !a.isContinuous()) परत असत्य;
        साठी (पूर्णांक i = 0; i < rdims; i++) जर (a.size[i] != R.size[i]) परत असत्य;
    }
    स्थिरांक दीर्घ दीर्घ total = (दीर्घ दीर्घ)R.total() * rcn;   // channels folded into the 1D run
    CV_Assert(total <= (दीर्घ दीर्घ)पूर्णांक_उच्च);
    साठी (पूर्णांक k = 0; k < K; k++)
    {
        स्थिरांक Mat& a = *arrays[k];
        base[k] = (uchar*)a.data;
        esz1[k] = (पूर्णांक)a.elemSize1();
        S[k][0] = isSingleChannelScalar(a) ? 0 : 1;
    }
    D.assign(1, (पूर्णांक)total);
    m = 1;
    परत सत्य;
}

// ---------------------------------------------------------------------------
// BroadcastOp::run
// ---------------------------------------------------------------------------
// At namespace scope, NOT inside run(): MSVC 2019 loses the constexpr-ness of function-local
// constants used as template arguments inside a lambda (C2975).
स्थिर constexpr पूर्णांक MAX_DIMS = MatShape::MAX_DIMS;
स्थिर constexpr पूर्णांक LOCAL_OPS = 8;

शून्य BroadcastOp::run(स्थिरांक Mat* स्थिरांक* arrays, पूर्णांक narrays,
                      स्थिरांक std::function<शून्य(स्थिरांक Tile&)>& body,
                      तार्किक expandChannels,
                      द्विगुण nstripes)
{
    स्थिरांक पूर्णांक K = narrays;
    CV_Assert(K >= 1 && arrays != nullptr);

    // ---- 1-3. geometry: per-operand collapsed steps S[k], element sizes esz1[k], base
    //           pointers, and the collapsed iteration shape D (m dims). The fast path handles the
    //           dominant "all same-shape arrays (+ single-channel scalars)" case in one shot; the
    //           general path does decideChannelMode + broadcastShape + align + collapse. ----
    AutoBuffer<MatStep, LOCAL_OPS>  S(K);
    AutoBuffer<पूर्णांक, LOCAL_OPS>      esz1(K);
    AutoBuffer<uchar*, LOCAL_OPS>   base(K);
    MatShape D;
    पूर्णांक m;

    जर (!fastSameShape(arrays, K, expandChannels, base.data(), esz1.data(), S.data(), D, m))
    {
        स्थिरांक ChMode mode = expandChannels ? decideChannelMode(arrays, K) : CH_ELEM;
        AutoBuffer<MatShape, LOCAL_OPS> shp(K);
        AutoBuffer<MatStep, LOCAL_OPS>  stp(K);
        साठी (पूर्णांक k = 0; k < K; k++)
        {
            matLayout(*arrays[k], mode, shp[k], stp[k], esz1[k]);
            base[k] = (uchar*)arrays[k]->data;
        }
        MatShape full;
        CV_Assert(broadcastShape(shp.data(), K, full) && "ew: operands are not broadcast-compatible");
        स्थिरांक पूर्णांक nd = (पूर्णांक)full.size();
        AutoBuffer<MatShape, LOCAL_OPS> H(K);
        साठी (पूर्णांक k = 0; k < K; k++) alignArg(shp[k], stp[k], nd, S[k], H[k]);
        D = full;
        m = collapseDims(S.data(), H.data(), K, D);

        // For a cv::Mat the innermost (channel/last) axis is contiguous, so after collapse the
        // innermost stride is always in {0,1}. No gather, no materialization.
        साठी (पूर्णांक k = 0; k < K; k++)
            CV_Assert(S[k][m - 1] <= 1 && "ew: unexpected innermost stride > 1");
    }

    // ---- 4. inner 2D tile axes: width = D[m-1], height = D[m-2] (if any) ----
    स्थिरांक पूर्णांक wAxis = m - 1;
    स्थिरांक पूर्णांक hAxis = (m >= 2) ? m - 2 : -1;
    स्थिरांक पूर्णांक W   = D[wAxis];
    स्थिरांक पूर्णांक Hgt = (hAxis >= 0) ? D[hAxis] : 1;
    स्थिरांक पूर्णांक nOuter = (hAxis >= 0) ? m - 2 : m - 1;   // outer ("plane") axes = [0 .. nOuter)
    दीर्घ दीर्घ nplanes = 1;
    साठी (पूर्णांक d = 0; d < nOuter; d++) nplanes *= D[d];

    // ---- 5. desired parallel stripe count (work hint) ----
    स्थिरांक दीर्घ दीर्घ total = nplanes * (दीर्घ दीर्घ)Hgt * (दीर्घ दीर्घ)W;
    द्विगुण stripes = nstripes;
    जर (stripes <= 0)                                  // broadcastOp can't see the body's cost;
        stripes = (द्विगुण)total * 100.0 / (द्विगुण)(1 << 18);  // assume ~100 cycles/element
    स्थिरांक पूर्णांक wantTiles = std::max(1, (पूर्णांक)std::lround(stripes));

    // ---- 6. tile only for PARALLELISM. broadcastOp is op-agnostic: it does not know the body's
    //         temp-buffer footprint, so it does NOT tile for L1 - that is the body's job (it
    //         re-fragments a tile's width into L1-sized chunks for the fused intermediates).
    //         Start with the largest tile (one 2D block per plane) and split (height first, then
    //         width) only until there are at least `wantTiles` tiles. Bigger tiles => fewer
    //         body/decode calls. Width is G-aligned only in the fully-contiguous (1D) case. ----
    स्थिरांक पूर्णांक G = 16;                  // SIMD/cacheline granule
    पूर्णांक tw = W, th = Hgt;

    स्वतः ntilesOf = [&](पूर्णांक tw_, पूर्णांक th_) {
        दीर्घ दीर्घ nw = (W + tw_ - 1) / tw_, nh = (Hgt + th_ - 1) / th_;
        परत nplanes * nh * nw;
    };
    दीर्घ दीर्घ ntiles = ntilesOf(tw, th);
    पर्यंत (ntiles < wantTiles && th > 1)               // split height for parallelism
    {
        th = (th + 1) / 2;
        ntiles = ntilesOf(tw, th);
    }
    पर्यंत (ntiles < wantTiles && tw > G)               // then split width
    {
        tw = std::max(G, tw / 2);
        जर (hAxis < 0 && tw > G) tw -= tw % G;          // keep width aligned in the 1D case
        ntiles = ntilesOf(tw, th);
    }
    CV_Assert(ntiles <= (दीर्घ दीर्घ)पूर्णांक_उच्च);

    स्थिरांक पूर्णांक ntilesW = (W + tw - 1) / tw;
    स्थिरांक पूर्णांक ntilesH = (Hgt + th - 1) / th;

    // ---- 7. execution; decode tile index -> per-operand slices. stepx/stepy are the same for
    //         every tile, so they are set ONCE; only the per-tile base pointer is recomputed. ----
    स्वतः runRange = [&](स्थिरांक Range& r)
    {
        AutoBuffer<Slice, LOCAL_OPS> slices(K);

        // Fast 1D path (m==1: one contiguous axis after collapse, no outer planes, height 1).
        // ntilesH==1 and nplanes==1, so the tile index IS the width-tile index - no div/mod, no
        // plane multi-index decode, no inner step loop. This is the same-shape / fully-contiguous
        // common case.
        जर (m == 1)
        {
            साठी (पूर्णांक k = 0; k < K; k++) { slices[k].stepy = 0; slices[k].stepx = S[k][0]; }
            Tile tile;
            tile.height = 1; tile.narrays = K; tile.slices = slices.data();
            साठी (पूर्णांक t = r.start; t < r.end; t++)
            {
                स्थिरांक पूर्णांक wofs = t * tw, ww = std::min(tw, W - wofs);
                साठी (पूर्णांक k = 0; k < K; k++)
                    slices[k].ptr = base[k] + (माप_प्रकार)wofs * S[k][0] * (माप_प्रकार)esz1[k];
                tile.width = ww;
                body(tile);
            }
            परत;
        }

        std::array<पूर्णांक, MAX_DIMS> idx;
        साठी (पूर्णांक k = 0; k < K; k++)                     // steps are tile-independent: set once
        {
            slices[k].stepy = (hAxis >= 0) ? S[k][hAxis] : 0;
            slices[k].stepx = S[k][wAxis];
        }
        साठी (पूर्णांक t = r.start; t < r.end; t++)
        {
            पूर्णांक wt = t % ntilesW;
            पूर्णांक rest = t / ntilesW;
            पूर्णांक ht = rest % ntilesH;
            पूर्णांक plane = rest / ntilesH;

            स्थिरांक पूर्णांक wofs = wt * tw, ww = std::min(tw, W - wofs);
            स्थिरांक पूर्णांक hofs = ht * th, hh = std::min(th, Hgt - hofs);

            पूर्णांक p = plane;                              // decode plane -> outer multi-index
            साठी (पूर्णांक d = nOuter - 1; d >= 0; d--) {
                पूर्णांक dd = D[d];
                पूर्णांक np = p / dd;
                idx[d] = p - np * dd;
                p = np;
            }

            साठी (पूर्णांक k = 0; k < K; k++)
            {
                माप_प्रकार off = (माप_प्रकार)wofs * S[k][wAxis];
                साठी (पूर्णांक d = 0; d < nOuter; d++) off += (माप_प्रकार)idx[d] * S[k][d];
                जर (hAxis >= 0) off += (माप_प्रकार)hofs * S[k][hAxis];
                slices[k].ptr = base[k] + off * (माप_प्रकार)esz1[k];
            }

            Tile tile;
            tile.width = ww; tile.height = hh; tile.narrays = K; tile.slices = slices.data();
            body(tile);
        }
    };

    // Single tile (small work, wantTiles==1) => run inline and skip the parallel framework
    // entirely: its dispatch (std::function wrap + Range machinery + backend hop) is pure
    // overhead when there is nothing to parallelize, and dominates small-array latency.
    जर (ntiles == 1)
        runRange(Range(0, 1));
    अन्यथा
        parallel_for_(Range(0, (पूर्णांक)ntiles), runRange, stripes);
}

} // namespace cv
