// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
// Copyright (C) 2026, BigVision LLC, all rights reserved.
// Third party copyrights are property of their respective owners.

// Jonker-Volgenant rectangular assignment. See @cite Crouse2016 and @cite Jonker1987.

#ਅਨ੍ਤਰ੍ਭਾਵ "precomp.hpp"

#ਅਨ੍ਤਰ੍ਭਾਵ <algorithm>
#ਅਨ੍ਤਰ੍ਭਾਵ <limits>

ਨਾਮਾਕਾਸ਼ cv {

// cvIsNaN/cvIsInf inspect the bit pattern, so unlike std::isfinite they still work under
// -ffast-math.
ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ isFiniteVal(ਦੁੱਗਣਾ x)
{
    ਵਾਪਸ !cvIsNaN(x) && !cvIsInf(x);
}

// Type the duals and the running sums are kept in. double suits all three inputs: it holds every
// int and float value exactly, so the integer path needs no arithmetic of its own.
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T> ਬਣਤਰ Lap_ਸੰਚਾਇਕ { ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਦੁੱਗਣਾ type; };

// A cell is usable only if it is a real cost within the threshold. Check finiteness first:
// NaN > costThreshold is false, so a bare comparison would let NaN through as allowed.
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ cellAllowed(T value, ਦੁੱਗਣਾ ਲਾਗਤ_ਹੱਦ)
{
    ਅਚਲ ਦੁੱਗਣਾ v = (ਦੁੱਗਣਾ)value;
    ਵਾਪਸ isFiniteVal(v) && v <= ਲਾਗਤ_ਹੱਦ;
}

// Assigns every row to its own column at the lowest total. `work` is nrows x ncols, rows <= cols,
// all finite. Each pass adds one row by a Dijkstra search over the reduced costs.
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Acc>
ਸਥਿਰ ਖਾਲੀ solveJV(ਅਚਲ std::vector<Acc>& work, ਪੂਰਨ nrows, ਪੂਰਨ ncols, std::vector<ਪੂਰਨ>& ਖਾਨਾ_ਦਾ_ਪੰਕਤੀ)
{
    CV_DbgAssert(nrows <= ncols);

    std::vector<Acc> u((size_t)nrows, Acc(0));
    std::vector<Acc> v((size_t)ncols, Acc(0));
    std::vector<Acc> dist((size_t)ncols);
    std::vector<ਪੂਰਨ> ਪੰਕਤੀ_ਦਾ_ਖਾਨਾ((size_t)ncols, -1);
    std::vector<ਪੂਰਨ> prev_ਪੰਕਤੀ((size_t)ncols);
    std::vector<uchar> labelled((size_t)ncols);
    std::vector<ਪੂਰਨ> ਬਚਿਆ((size_t)ncols);   // columns not yet labelled, in no order

    ਖਾਨਾ_ਦਾ_ਪੰਕਤੀ.assign((size_t)nrows, -1);

    ਲਈ (ਪੂਰਨ free_ਪੰਕਤੀ = 0; free_ਪੰਕਤੀ < nrows; free_ਪੰਕਤੀ++)
    {
        std::fill(dist.begin(), dist.end(), std::numeric_limits<Acc>::max());
        std::fill(labelled.begin(), labelled.end(), (uchar)0);
        ਲਈ (ਪੂਰਨ j = 0; j < ncols; j++)
            ਬਚਿਆ[j] = j;
        ਪੂਰਨ nremaining = ncols;

        ਪੂਰਨ ਪੰਕਤੀ = free_ਪੰਕਤੀ;      // row currently being expanded
        Acc delta = Acc(0);     // length of the shortest path found so far
        ਪੂਰਨ sink = -1;          // free column that ends the augmenting path

        ਜਦੋਂ (sink < 0)
        {
            ਅਚਲ Acc* ਪੰਕਤੀ_Ptr = &work[(size_t)ਪੰਕਤੀ * ncols];
            ਅਚਲ Acc urow = u[ਪੰਕਤੀ];
            // One pass over the unlabelled columns relaxes them and picks the nearest. A settled
            // column is swapped off the tail so it is never read again.
            ਪੂਰਨ ਉੱਤਮ = 0;
            Acc ਉੱਤਮ_Dist = std::numeric_limits<Acc>::max();
            ਲਈ (ਪੂਰਨ k = 0; k < nremaining; k++)
            {
                ਅਚਲ ਪੂਰਨ j = ਬਚਿਆ[k];
                ਅਚਲ Acc cand = delta + ਪੰਕਤੀ_Ptr[j] - urow - v[j];
                ਜੇ (cand < dist[j])
                {
                    dist[j] = cand;
                    prev_ਪੰਕਤੀ[j] = ਪੰਕਤੀ;
                }
                ਜੇ (dist[j] < ਉੱਤਮ_Dist)
                {
                    ਉੱਤਮ_Dist = dist[j];
                    ਉੱਤਮ = k;
                }
            }
            // ncols >= nrows leaves at least one column free, and every cost is finite, so the
            // search can always reach it.
            CV_Assert(ਉੱਤਮ_Dist < std::numeric_limits<Acc>::max());

            ਅਚਲ ਪੂਰਨ next = ਬਚਿਆ[ਉੱਤਮ];
            ਬਚਿਆ[ਉੱਤਮ] = ਬਚਿਆ[--nremaining];
            labelled[next] = 1;    // still needed by the dual update below
            delta = ਉੱਤਮ_Dist;

            ਜੇ (ਪੰਕਤੀ_ਦਾ_ਖਾਨਾ[next] < 0)
                sink = next;
            ਨਹੀਂਤਾਂ
                ਪੰਕਤੀ = ਪੰਕਤੀ_ਦਾ_ਖਾਨਾ[next];   // occupied, so keep going through its row
        }

        // Shift the duals so the path edges land at zero without disturbing the matched pairs.
        // Must run before the matching below, which it reads.
        u[free_ਪੰਕਤੀ] += delta;
        ਲਈ (ਪੂਰਨ j = 0; j < ncols; j++)
        {
            ਜੇ (labelled[j] && j != sink)
            {
                ਅਚਲ Acc ਖਿਸਕਾਓ = delta - dist[j];
                v[j] -= ਖਿਸਕਾਓ;
                u[ਪੰਕਤੀ_ਦਾ_ਖਾਨਾ[j]] += ਖਿਸਕਾਓ;
            }
        }

        // Walk the path back from the free column, flipping each edge.
        ਲਈ (ਪੂਰਨ j = sink;;)
        {
            ਅਚਲ ਪੂਰਨ i = prev_ਪੰਕਤੀ[j];
            ਪੰਕਤੀ_ਦਾ_ਖਾਨਾ[j] = i;
            ਅਚਲ ਪੂਰਨ jPrev = ਖਾਨਾ_ਦਾ_ਪੰਕਤੀ[i];
            ਖਾਨਾ_ਦਾ_ਪੰਕਤੀ[i] = j;
            ਜੇ (i == free_ਪੰਕਤੀ)
                ਰੋਕੋ;
            j = jPrev;
        }
    }
}

// Solves for one input type. `src` is read in its own type, so the padded working matrix is the
// only copy made and no promoted duplicate of the input is allocated.
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
ਸਥਿਰ ਦੁੱਗਣਾ solveTyped(ਅਚਲ Mat& src, ਤਾਰਕਿਕ transposed, std::vector<ਪੂਰਨ>& assignment,
                         ਦੁੱਗਣਾ ਲਾਗਤ_ਹੱਦ)
{
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਕਿਸਮ_ਨਾਂ Lap_ਸੰਚਾਇਕ<T>::type Acc;

    ਅਚਲ ਪੂਰਨ nrows = src.ਪੰਕਤੀਆਂ;
    ਅਚਲ ਪੂਰਨ nreal = src.ਖਾਨੇ_ਗਿਣਤੀ;

    Mat allowed(nrows, nreal, CV_8U);
    Acc abs_ਜੋੜ = Acc(0);
    ਦੁੱਗਣਾ max_ਲਾਗਤ = -std::numeric_limits<ਦੁੱਗਣਾ>::max();
    ਤਾਰਕਿਕ anyForbidden = ਝੂਠ;
    ਲਈ (ਪੂਰਨ i = 0; i < nrows; i++)
    {
        ਅਚਲ T* s = src.ptr<T>(i);
        uchar* a = allowed.ptr<uchar>(i);
        ਲਈ (ਪੂਰਨ j = 0; j < nreal; j++)
        {
            ਅਚਲ ਤਾਰਕਿਕ ok = cellAllowed<T>(s[j], ਲਾਗਤ_ਹੱਦ);
            a[j] = ok ? 1 : 0;
            ਜੇ (ok)
            {
                abs_ਜੋੜ += (Acc)std::ਨਿਰਪੇਖ((ਦੁੱਗਣਾ)s[j]);
                max_ਲਾਗਤ = std::max(max_ਲਾਗਤ, (ਦੁੱਗਣਾ)s[j]);
            }
            ਨਹੀਂਤਾਂ
                anyForbidden = ਸੱਚ;
        }
    }

    // Price of leaving a row unmatched, clamped so the DBL_MAX default stays finite.
    // The 2 is required: k -> k+1 augments a path, so the delta is 2*absSum, not one edge.
    Acc dummy = Acc(2) * abs_ਜੋੜ + Acc(1);
    ਜੇ (ਲਾਗਤ_ਹੱਦ < (ਦੁੱਗਣਾ)dummy)
        dummy = (Acc)ਲਾਗਤ_ਹੱਦ;
    CV_Assert(isFiniteVal((ਦੁੱਗਣਾ)dummy));

    // A forbidden cell only has to lose to a dummy, and every row has one. The margin scales
    // because adding 1 to a very large dummy changes nothing.
    ਅਚਲ Acc forbidden = dummy + (Acc)std::max(1.0, std::ਨਿਰਪੇਖ((ਦੁੱਗਣਾ)dummy) * 1e-12);
    CV_Assert(isFiniteVal((ਦੁੱਗਣਾ)forbidden) && forbidden > dummy);

    // Drop the dummy columns when no row can go unmatched: nothing forbidden, every cost below the
    // no-pair price. Halves the columns on square input; a cost equal to that price is a tie.
    ਅਚਲ ਪੂਰਨ ndummy = (anyForbidden || !(max_ਲਾਗਤ < (ਦੁੱਗਣਾ)dummy)) ? nrows : 0;
    ਅਚਲ ਪੂਰਨ ncols = nreal + ndummy;

    std::vector<Acc> work((size_t)nrows * ncols);
    ਲਈ (ਪੂਰਨ i = 0; i < nrows; i++)
    {
        ਅਚਲ T* s = src.ptr<T>(i);
        ਅਚਲ uchar* a = allowed.ptr<uchar>(i);
        Acc* w = &work[(size_t)i * ncols];
        ਲਈ (ਪੂਰਨ j = 0; j < nreal; j++)
            w[j] = a[j] ? (Acc)s[j] : forbidden;
        ਲਈ (ਪੂਰਨ j = nreal; j < ncols; j++)
            w[j] = dummy;
    }

    std::vector<ਪੂਰਨ> ਖਾਨਾ_ਦਾ_ਪੰਕਤੀ;
    solveJV<Acc>(work, nrows, ncols, ਖਾਨਾ_ਦਾ_ਪੰਕਤੀ);

    ਦੁੱਗਣਾ ਕੁੱਲ = 0.0;
    ਲਈ (ਪੂਰਨ i = 0; i < nrows; i++)
    {
        ਅਚਲ ਪੂਰਨ j = ਖਾਨਾ_ਦਾ_ਪੰਕਤੀ[i];
        CV_DbgAssert(j >= 0 && j < ncols);      // solveJV gives every row a column
        ਜੇ (j >= nreal || !allowed.at<uchar>(i, j))
            ਜਾਰੀ;   // landed on a dummy column or a forbidden cell, so it stays unmatched

        ਕੁੱਲ += (ਦੁੱਗਣਾ)src.ptr<T>(i)[j];
        ਅਚਲ ਪੂਰਨ ਪੰਕਤੀ = transposed ? j : i;
        CV_DbgAssert(ਪੰਕਤੀ >= 0 && (size_t)ਪੰਕਤੀ < assignment.size());
        assignment[(size_t)ਪੰਕਤੀ] = transposed ? i : j;
    }
    ਵਾਪਸ ਕੁੱਲ;
}

ਦੁੱਗਣਾ ਰੇਖਿਕ_Assignment(ਨਿਵੇਸ਼_Array ਲਾਗਤ, std::vector<ਪੂਰਨ>& assignment, ਦੁੱਗਣਾ ਲਾਗਤ_ਹੱਦ)
{
    CV_INSTRUMENT_REGION();

    Mat ਲਾਗਤ_2 = ਲਾਗਤ.getMat();
    CV_Assert(ਲਾਗਤ_2.dims <= 2);
    CV_Assert(!ਲਾਗਤ_2.empty());

    assignment.assign((size_t)ਲਾਗਤ_2.ਪੰਕਤੀਆਂ, -1);

    CV_CheckType(ਲਾਗਤ_2.type(),
                 ਲਾਗਤ_2.type() == CV_32FC1 || ਲਾਗਤ_2.type() == CV_64FC1 || ਲਾਗਤ_2.type() == CV_32SC1,
                 "cost must be a single-channel CV_32F, CV_64F or CV_32S matrix");
    CV_Assert(!cvIsNaN(ਲਾਗਤ_ਹੱਦ));

    // The solver assigns every row, so it needs rows <= cols. Transposing also makes A and its
    // transpose give the same answer, since min(M,N) is the same either way.
    ਅਚਲ ਤਾਰਕਿਕ transposed = ਲਾਗਤ_2.ਪੰਕਤੀਆਂ > ਲਾਗਤ_2.ਖਾਨੇ_ਗਿਣਤੀ;
    Mat src;
    ਜੇ (transposed)
        cv::transpose(ਲਾਗਤ_2, src);
    ਨਹੀਂਤਾਂ
        src = ਲਾਗਤ_2;

    ਚੋਣ (ਲਾਗਤ_2.ਡੂੰਘਾਈ())
    {
    ਹਾਲਤ CV_32F: ਵਾਪਸ solveTyped<ਦਸ਼ਮਲਵ>(src, transposed, assignment, ਲਾਗਤ_ਹੱਦ);
    ਹਾਲਤ CV_64F: ਵਾਪਸ solveTyped<ਦੁੱਗਣਾ>(src, transposed, assignment, ਲਾਗਤ_ਹੱਦ);
    ਹਾਲਤ CV_32S: ਵਾਪਸ solveTyped<ਪੂਰਨ>(src, transposed, assignment, ਲਾਗਤ_ਹੱਦ);
    ਮੂਲ:     CV_Error(Error::StsUnsupportedFormat, "unsupported cost matrix type");
    }
}

} // namespace cv
