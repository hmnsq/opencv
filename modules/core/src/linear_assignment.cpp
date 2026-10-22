// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
// Copyright (C) 2026, BigVision LLC, all rights reserved.
// Third party copyrights are property of their respective owners.

// Jonker-Volgenant rectangular assignment. See @cite Crouse2016 and @cite Jonker1987.

#ಅನ್ತರ್ಭಾವ "precomp.hpp"

#ಅನ್ತರ್ಭಾವ <algorithm>
#ಅನ್ತರ್ಭಾವ <limits>

ನಾಮಾಕಾಶ cv {

// cvIsNaN/cvIsInf inspect the bit pattern, so unlike std::isfinite they still work under
// -ffast-math.
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ತಾರ್ಕಿಕ isFiniteVal(ದ್ವಿಗುಣ x)
{
    ಹಿಂತಿರುಗಿ !cvIsNaN(x) && !cvIsInf(x);
}

// Type the duals and the running sums are kept in. double suits all three inputs: it holds every
// int and float value exactly, so the integer path needs no arithmetic of its own.
ಮಾದರಿ <ಬಗೆ_ಹೆಸರು T> ರಚನೆ Lap_ಸಂಚಯಕ { ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ದ್ವಿಗುಣ type; };

// A cell is usable only if it is a real cost within the threshold. Check finiteness first:
// NaN > costThreshold is false, so a bare comparison would let NaN through as allowed.
ಮಾದರಿ <ಬಗೆ_ಹೆಸರು T>
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ತಾರ್ಕಿಕ cellAllowed(T value, ದ್ವಿಗುಣ ವೆಚ್ಚ_ಮಿತಿ)
{
    ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ v = (ದ್ವಿಗುಣ)value;
    ಹಿಂತಿರುಗಿ isFiniteVal(v) && v <= ವೆಚ್ಚ_ಮಿತಿ;
}

// Assigns every row to its own column at the lowest total. `work` is nrows x ncols, rows <= cols,
// all finite. Each pass adds one row by a Dijkstra search over the reduced costs.
ಮಾದರಿ <ಬಗೆ_ಹೆಸರು Acc>
ಸ್ಥಿರ ಶೂನ್ಯ solveJV(ಸ್ಥಿರಾಂಕ std::vector<Acc>& work, ಪೂರ್ಣಾಂಕ nrows, ಪೂರ್ಣಾಂಕ ncols, std::vector<ಪೂರ್ಣಾಂಕ>& ಲಂಬಸಾಲು_ದ_ಅಡ್ಡಸಾಲು)
{
    CV_DbgAssert(nrows <= ncols);

    std::vector<Acc> u((size_t)nrows, Acc(0));
    std::vector<Acc> v((size_t)ncols, Acc(0));
    std::vector<Acc> dist((size_t)ncols);
    std::vector<ಪೂರ್ಣಾಂಕ> ಅಡ್ಡಸಾಲು_ದ_ಲಂಬಸಾಲು((size_t)ncols, -1);
    std::vector<ಪೂರ್ಣಾಂಕ> prev_ಅಡ್ಡಸಾಲು((size_t)ncols);
    std::vector<uchar> labelled((size_t)ncols);
    std::vector<ಪೂರ್ಣಾಂಕ> ಉಳಿದ((size_t)ncols);   // columns not yet labelled, in no order

    ಲಂಬಸಾಲು_ದ_ಅಡ್ಡಸಾಲು.assign((size_t)nrows, -1);

    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ free_ಅಡ್ಡಸಾಲು = 0; free_ಅಡ್ಡಸಾಲು < nrows; free_ಅಡ್ಡಸಾಲು++)
    {
        std::fill(dist.begin(), dist.end(), std::numeric_limits<Acc>::max());
        std::fill(labelled.begin(), labelled.end(), (uchar)0);
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = 0; j < ncols; j++)
            ಉಳಿದ[j] = j;
        ಪೂರ್ಣಾಂಕ nremaining = ncols;

        ಪೂರ್ಣಾಂಕ ಅಡ್ಡಸಾಲು = free_ಅಡ್ಡಸಾಲು;      // row currently being expanded
        Acc delta = Acc(0);     // length of the shortest path found so far
        ಪೂರ್ಣಾಂಕ sink = -1;          // free column that ends the augmenting path

        ಆಗ (sink < 0)
        {
            ಸ್ಥಿರಾಂಕ Acc* ಅಡ್ಡಸಾಲು_Ptr = &work[(size_t)ಅಡ್ಡಸಾಲು * ncols];
            ಸ್ಥಿರಾಂಕ Acc urow = u[ಅಡ್ಡಸಾಲು];
            // One pass over the unlabelled columns relaxes them and picks the nearest. A settled
            // column is swapped off the tail so it is never read again.
            ಪೂರ್ಣಾಂಕ ಉತ್ತಮ = 0;
            Acc ಉತ್ತಮ_Dist = std::numeric_limits<Acc>::max();
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ k = 0; k < nremaining; k++)
            {
                ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ j = ಉಳಿದ[k];
                ಸ್ಥಿರಾಂಕ Acc cand = delta + ಅಡ್ಡಸಾಲು_Ptr[j] - urow - v[j];
                ಆದರೆ (cand < dist[j])
                {
                    dist[j] = cand;
                    prev_ಅಡ್ಡಸಾಲು[j] = ಅಡ್ಡಸಾಲು;
                }
                ಆದರೆ (dist[j] < ಉತ್ತಮ_Dist)
                {
                    ಉತ್ತಮ_Dist = dist[j];
                    ಉತ್ತಮ = k;
                }
            }
            // ncols >= nrows leaves at least one column free, and every cost is finite, so the
            // search can always reach it.
            CV_Assert(ಉತ್ತಮ_Dist < std::numeric_limits<Acc>::max());

            ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ next = ಉಳಿದ[ಉತ್ತಮ];
            ಉಳಿದ[ಉತ್ತಮ] = ಉಳಿದ[--nremaining];
            labelled[next] = 1;    // still needed by the dual update below
            delta = ಉತ್ತಮ_Dist;

            ಆದರೆ (ಅಡ್ಡಸಾಲು_ದ_ಲಂಬಸಾಲು[next] < 0)
                sink = next;
            ಇಲ್ಲದಿದ್ದರೆ
                ಅಡ್ಡಸಾಲು = ಅಡ್ಡಸಾಲು_ದ_ಲಂಬಸಾಲು[next];   // occupied, so keep going through its row
        }

        // Shift the duals so the path edges land at zero without disturbing the matched pairs.
        // Must run before the matching below, which it reads.
        u[free_ಅಡ್ಡಸಾಲು] += delta;
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = 0; j < ncols; j++)
        {
            ಆದರೆ (labelled[j] && j != sink)
            {
                ಸ್ಥಿರಾಂಕ Acc ಸರಿಸು = delta - dist[j];
                v[j] -= ಸರಿಸು;
                u[ಅಡ್ಡಸಾಲು_ದ_ಲಂಬಸಾಲು[j]] += ಸರಿಸು;
            }
        }

        // Walk the path back from the free column, flipping each edge.
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = sink;;)
        {
            ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ i = prev_ಅಡ್ಡಸಾಲು[j];
            ಅಡ್ಡಸಾಲು_ದ_ಲಂಬಸಾಲು[j] = i;
            ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ jPrev = ಲಂಬಸಾಲು_ದ_ಅಡ್ಡಸಾಲು[i];
            ಲಂಬಸಾಲು_ದ_ಅಡ್ಡಸಾಲು[i] = j;
            ಆದರೆ (i == free_ಅಡ್ಡಸಾಲು)
                ನಿಲ್ಲಿಸು;
            j = jPrev;
        }
    }
}

// Solves for one input type. `src` is read in its own type, so the padded working matrix is the
// only copy made and no promoted duplicate of the input is allocated.
ಮಾದರಿ <ಬಗೆ_ಹೆಸರು T>
ಸ್ಥಿರ ದ್ವಿಗುಣ solveTyped(ಸ್ಥಿರಾಂಕ Mat& src, ತಾರ್ಕಿಕ transposed, std::vector<ಪೂರ್ಣಾಂಕ>& assignment,
                         ದ್ವಿಗುಣ ವೆಚ್ಚ_ಮಿತಿ)
{
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಬಗೆ_ಹೆಸರು Lap_ಸಂಚಯಕ<T>::type Acc;

    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ nrows = src.ಅಡ್ಡಸಾಲುಗಳು;
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ nreal = src.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ;

    Mat allowed(nrows, nreal, CV_8U);
    Acc abs_ಮೊತ್ತ = Acc(0);
    ದ್ವಿಗುಣ max_ವೆಚ್ಚ = -std::numeric_limits<ದ್ವಿಗುಣ>::max();
    ತಾರ್ಕಿಕ anyForbidden = ಸುಳ್ಳು;
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < nrows; i++)
    {
        ಸ್ಥಿರಾಂಕ T* s = src.ptr<T>(i);
        uchar* a = allowed.ptr<uchar>(i);
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = 0; j < nreal; j++)
        {
            ಸ್ಥಿರಾಂಕ ತಾರ್ಕಿಕ ok = cellAllowed<T>(s[j], ವೆಚ್ಚ_ಮಿತಿ);
            a[j] = ok ? 1 : 0;
            ಆದರೆ (ok)
            {
                abs_ಮೊತ್ತ += (Acc)std::ನಿರಪೇಕ್ಷ((ದ್ವಿಗುಣ)s[j]);
                max_ವೆಚ್ಚ = std::max(max_ವೆಚ್ಚ, (ದ್ವಿಗುಣ)s[j]);
            }
            ಇಲ್ಲದಿದ್ದರೆ
                anyForbidden = ನಿಜ;
        }
    }

    // Price of leaving a row unmatched, clamped so the DBL_MAX default stays finite.
    // The 2 is required: k -> k+1 augments a path, so the delta is 2*absSum, not one edge.
    Acc dummy = Acc(2) * abs_ಮೊತ್ತ + Acc(1);
    ಆದರೆ (ವೆಚ್ಚ_ಮಿತಿ < (ದ್ವಿಗುಣ)dummy)
        dummy = (Acc)ವೆಚ್ಚ_ಮಿತಿ;
    CV_Assert(isFiniteVal((ದ್ವಿಗುಣ)dummy));

    // A forbidden cell only has to lose to a dummy, and every row has one. The margin scales
    // because adding 1 to a very large dummy changes nothing.
    ಸ್ಥಿರಾಂಕ Acc forbidden = dummy + (Acc)std::max(1.0, std::ನಿರಪೇಕ್ಷ((ದ್ವಿಗುಣ)dummy) * 1e-12);
    CV_Assert(isFiniteVal((ದ್ವಿಗುಣ)forbidden) && forbidden > dummy);

    // Drop the dummy columns when no row can go unmatched: nothing forbidden, every cost below the
    // no-pair price. Halves the columns on square input; a cost equal to that price is a tie.
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ndummy = (anyForbidden || !(max_ವೆಚ್ಚ < (ದ್ವಿಗುಣ)dummy)) ? nrows : 0;
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ncols = nreal + ndummy;

    std::vector<Acc> work((size_t)nrows * ncols);
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < nrows; i++)
    {
        ಸ್ಥಿರಾಂಕ T* s = src.ptr<T>(i);
        ಸ್ಥಿರಾಂಕ uchar* a = allowed.ptr<uchar>(i);
        Acc* w = &work[(size_t)i * ncols];
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = 0; j < nreal; j++)
            w[j] = a[j] ? (Acc)s[j] : forbidden;
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = nreal; j < ncols; j++)
            w[j] = dummy;
    }

    std::vector<ಪೂರ್ಣಾಂಕ> ಲಂಬಸಾಲು_ದ_ಅಡ್ಡಸಾಲು;
    solveJV<Acc>(work, nrows, ncols, ಲಂಬಸಾಲು_ದ_ಅಡ್ಡಸಾಲು);

    ದ್ವಿಗುಣ ಒಟ್ಟು = 0.0;
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < nrows; i++)
    {
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ j = ಲಂಬಸಾಲು_ದ_ಅಡ್ಡಸಾಲು[i];
        CV_DbgAssert(j >= 0 && j < ncols);      // solveJV gives every row a column
        ಆದರೆ (j >= nreal || !allowed.at<uchar>(i, j))
            ಮುಂದುವರಿ;   // landed on a dummy column or a forbidden cell, so it stays unmatched

        ಒಟ್ಟು += (ದ್ವಿಗುಣ)src.ptr<T>(i)[j];
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ಅಡ್ಡಸಾಲು = transposed ? j : i;
        CV_DbgAssert(ಅಡ್ಡಸಾಲು >= 0 && (size_t)ಅಡ್ಡಸಾಲು < assignment.size());
        assignment[(size_t)ಅಡ್ಡಸಾಲು] = transposed ? i : j;
    }
    ಹಿಂತಿರುಗಿ ಒಟ್ಟು;
}

ದ್ವಿಗುಣ ರೇಖೀಯ_Assignment(ಒಳಹರಿವು_Array ವೆಚ್ಚ, std::vector<ಪೂರ್ಣಾಂಕ>& assignment, ದ್ವಿಗುಣ ವೆಚ್ಚ_ಮಿತಿ)
{
    CV_INSTRUMENT_REGION();

    Mat ವೆಚ್ಚ_2 = ವೆಚ್ಚ.getMat();
    CV_Assert(ವೆಚ್ಚ_2.dims <= 2);
    CV_Assert(!ವೆಚ್ಚ_2.empty());

    assignment.assign((size_t)ವೆಚ್ಚ_2.ಅಡ್ಡಸಾಲುಗಳು, -1);

    CV_CheckType(ವೆಚ್ಚ_2.type(),
                 ವೆಚ್ಚ_2.type() == CV_32FC1 || ವೆಚ್ಚ_2.type() == CV_64FC1 || ವೆಚ್ಚ_2.type() == CV_32SC1,
                 "cost must be a single-channel CV_32F, CV_64F or CV_32S matrix");
    CV_Assert(!cvIsNaN(ವೆಚ್ಚ_ಮಿತಿ));

    // The solver assigns every row, so it needs rows <= cols. Transposing also makes A and its
    // transpose give the same answer, since min(M,N) is the same either way.
    ಸ್ಥಿರಾಂಕ ತಾರ್ಕಿಕ transposed = ವೆಚ್ಚ_2.ಅಡ್ಡಸಾಲುಗಳು > ವೆಚ್ಚ_2.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ;
    Mat src;
    ಆದರೆ (transposed)
        cv::transpose(ವೆಚ್ಚ_2, src);
    ಇಲ್ಲದಿದ್ದರೆ
        src = ವೆಚ್ಚ_2;

    ಆಯ್ಕೆ (ವೆಚ್ಚ_2.ಆಳ())
    {
    ಸ್ಥಿತಿ CV_32F: ಹಿಂತಿರುಗಿ solveTyped<ದಶಮಾಂಶ>(src, transposed, assignment, ವೆಚ್ಚ_ಮಿತಿ);
    ಸ್ಥಿತಿ CV_64F: ಹಿಂತಿರುಗಿ solveTyped<ದ್ವಿಗುಣ>(src, transposed, assignment, ವೆಚ್ಚ_ಮಿತಿ);
    ಸ್ಥಿತಿ CV_32S: ಹಿಂತಿರುಗಿ solveTyped<ಪೂರ್ಣಾಂಕ>(src, transposed, assignment, ವೆಚ್ಚ_ಮಿತಿ);
    ಪೂರ್ವನಿಯೋಜಿತ:     CV_Error(Error::StsUnsupportedFormat, "unsupported cost matrix type");
    }
}

} // namespace cv
