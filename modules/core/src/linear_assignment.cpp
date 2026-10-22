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
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ isFiniteVal(ದ್ವಿಗುಣ x)
{
    ಪ್ರತ್ಯಾಗಮ !cvIsNaN(x) && !cvIsInf(x);
}

// Type the duals and the running sums are kept in. double suits all three inputs: it holds every
// int and float value exactly, so the integer path needs no arithmetic of its own.
template <typename T> ಸಂರಚನಾ LapAccumulator { ಪ್ರಕಾರನಿರ್ದೇಶ ದ್ವಿಗುಣ type; };

// A cell is usable only if it is a real cost within the threshold. Check finiteness first:
// NaN > costThreshold is false, so a bare comparison would let NaN through as allowed.
template <typename T>
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ cellAllowed(T value, ದ್ವಿಗುಣ costThreshold)
{
    ಧ್ರುವ ದ್ವಿಗುಣ v = (ದ್ವಿಗುಣ)value;
    ಪ್ರತ್ಯಾಗಮ isFiniteVal(v) && v <= costThreshold;
}

// Assigns every row to its own column at the lowest total. `work` is nrows x ncols, rows <= cols,
// all finite. Each pass adds one row by a Dijkstra search over the reduced costs.
template <typename Acc>
ಸ್ಥಿರ ಶೂನ್ಯ solveJV(ಧ್ರುವ std::vector<Acc>& work, ಪೂರ್ಣಾಙ್ಕ nrows, ಪೂರ್ಣಾಙ್ಕ ncols, std::vector<ಪೂರ್ಣಾಙ್ಕ>& colOfRow)
{
    CV_DbgAssert(nrows <= ncols);

    std::vector<Acc> u((size_t)nrows, Acc(0));
    std::vector<Acc> v((size_t)ncols, Acc(0));
    std::vector<Acc> dist((size_t)ncols);
    std::vector<ಪೂರ್ಣಾಙ್ಕ> rowOfCol((size_t)ncols, -1);
    std::vector<ಪೂರ್ಣಾಙ್ಕ> prevRow((size_t)ncols);
    std::vector<uchar> labelled((size_t)ncols);
    std::vector<ಪೂರ್ಣಾಙ್ಕ> remaining((size_t)ncols);   // columns not yet labelled, in no order

    colOfRow.assign((size_t)nrows, -1);

    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ freeRow = 0; freeRow < nrows; freeRow++)
    {
        std::fill(dist.begin(), dist.end(), std::numeric_limits<Acc>::max());
        std::fill(labelled.begin(), labelled.end(), (uchar)0);
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ j = 0; j < ncols; j++)
            remaining[j] = j;
        ಪೂರ್ಣಾಙ್ಕ nremaining = ncols;

        ಪೂರ್ಣಾಙ್ಕ row = freeRow;      // row currently being expanded
        Acc delta = Acc(0);     // length of the shortest path found so far
        ಪೂರ್ಣಾಙ್ಕ sink = -1;          // free column that ends the augmenting path

        ಯಾವತ್ (sink < 0)
        {
            ಧ್ರುವ Acc* rowPtr = &work[(size_t)row * ncols];
            ಧ್ರುವ Acc urow = u[row];
            // One pass over the unlabelled columns relaxes them and picks the nearest. A settled
            // column is swapped off the tail so it is never read again.
            ಪೂರ್ಣಾಙ್ಕ best = 0;
            Acc bestDist = std::numeric_limits<Acc>::max();
            ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ k = 0; k < nremaining; k++)
            {
                ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ j = remaining[k];
                ಧ್ರುವ Acc cand = delta + rowPtr[j] - urow - v[j];
                ಯದಿ (cand < dist[j])
                {
                    dist[j] = cand;
                    prevRow[j] = row;
                }
                ಯದಿ (dist[j] < bestDist)
                {
                    bestDist = dist[j];
                    best = k;
                }
            }
            // ncols >= nrows leaves at least one column free, and every cost is finite, so the
            // search can always reach it.
            CV_Assert(bestDist < std::numeric_limits<Acc>::max());

            ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ next = remaining[best];
            remaining[best] = remaining[--nremaining];
            labelled[next] = 1;    // still needed by the dual update below
            delta = bestDist;

            ಯದಿ (rowOfCol[next] < 0)
                sink = next;
            ಅನ್ಯಥಾ
                row = rowOfCol[next];   // occupied, so keep going through its row
        }

        // Shift the duals so the path edges land at zero without disturbing the matched pairs.
        // Must run before the matching below, which it reads.
        u[freeRow] += delta;
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ j = 0; j < ncols; j++)
        {
            ಯದಿ (labelled[j] && j != sink)
            {
                ಧ್ರುವ Acc shift = delta - dist[j];
                v[j] -= shift;
                u[rowOfCol[j]] += shift;
            }
        }

        // Walk the path back from the free column, flipping each edge.
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ j = sink;;)
        {
            ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ i = prevRow[j];
            rowOfCol[j] = i;
            ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ jPrev = colOfRow[i];
            colOfRow[i] = j;
            ಯದಿ (i == freeRow)
                ಭಙ್ಗ;
            j = jPrev;
        }
    }
}

// Solves for one input type. `src` is read in its own type, so the padded working matrix is the
// only copy made and no promoted duplicate of the input is allocated.
template <typename T>
ಸ್ಥಿರ ದ್ವಿಗುಣ solveTyped(ಧ್ರುವ Mat& src, ತಾರ್ಕಿಕ transposed, std::vector<ಪೂರ್ಣಾಙ್ಕ>& assignment,
                         ದ್ವಿಗುಣ costThreshold)
{
    ಪ್ರಕಾರನಿರ್ದೇಶ typename LapAccumulator<T>::type Acc;

    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ nrows = src.rows;
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ nreal = src.cols;

    Mat allowed(nrows, nreal, CV_8U);
    Acc absSum = Acc(0);
    ದ್ವಿಗುಣ maxCost = -std::numeric_limits<ದ್ವಿಗುಣ>::max();
    ತಾರ್ಕಿಕ anyForbidden = ಅಸತ್ಯ;
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < nrows; i++)
    {
        ಧ್ರುವ T* s = src.ptr<T>(i);
        uchar* a = allowed.ptr<uchar>(i);
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ j = 0; j < nreal; j++)
        {
            ಧ್ರುವ ತಾರ್ಕಿಕ ok = cellAllowed<T>(s[j], costThreshold);
            a[j] = ok ? 1 : 0;
            ಯದಿ (ok)
            {
                absSum += (Acc)std::abs((ದ್ವಿಗುಣ)s[j]);
                maxCost = std::max(maxCost, (ದ್ವಿಗುಣ)s[j]);
            }
            ಅನ್ಯಥಾ
                anyForbidden = ಸತ್ಯ;
        }
    }

    // Price of leaving a row unmatched, clamped so the DBL_MAX default stays finite.
    // The 2 is required: k -> k+1 augments a path, so the delta is 2*absSum, not one edge.
    Acc dummy = Acc(2) * absSum + Acc(1);
    ಯದಿ (costThreshold < (ದ್ವಿಗುಣ)dummy)
        dummy = (Acc)costThreshold;
    CV_Assert(isFiniteVal((ದ್ವಿಗುಣ)dummy));

    // A forbidden cell only has to lose to a dummy, and every row has one. The margin scales
    // because adding 1 to a very large dummy changes nothing.
    ಧ್ರುವ Acc forbidden = dummy + (Acc)std::max(1.0, std::abs((ದ್ವಿಗುಣ)dummy) * 1e-12);
    CV_Assert(isFiniteVal((ದ್ವಿಗುಣ)forbidden) && forbidden > dummy);

    // Drop the dummy columns when no row can go unmatched: nothing forbidden, every cost below the
    // no-pair price. Halves the columns on square input; a cost equal to that price is a tie.
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ ndummy = (anyForbidden || !(maxCost < (ದ್ವಿಗುಣ)dummy)) ? nrows : 0;
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ ncols = nreal + ndummy;

    std::vector<Acc> work((size_t)nrows * ncols);
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < nrows; i++)
    {
        ಧ್ರುವ T* s = src.ptr<T>(i);
        ಧ್ರುವ uchar* a = allowed.ptr<uchar>(i);
        Acc* w = &work[(size_t)i * ncols];
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ j = 0; j < nreal; j++)
            w[j] = a[j] ? (Acc)s[j] : forbidden;
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ j = nreal; j < ncols; j++)
            w[j] = dummy;
    }

    std::vector<ಪೂರ್ಣಾಙ್ಕ> colOfRow;
    solveJV<Acc>(work, nrows, ncols, colOfRow);

    ದ್ವಿಗುಣ total = 0.0;
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < nrows; i++)
    {
        ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ j = colOfRow[i];
        CV_DbgAssert(j >= 0 && j < ncols);      // solveJV gives every row a column
        ಯದಿ (j >= nreal || !allowed.at<uchar>(i, j))
            ಅನುವರ್ತ;   // landed on a dummy column or a forbidden cell, so it stays unmatched

        total += (ದ್ವಿಗುಣ)src.ptr<T>(i)[j];
        ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ row = transposed ? j : i;
        CV_DbgAssert(row >= 0 && (size_t)row < assignment.size());
        assignment[(size_t)row] = transposed ? i : j;
    }
    ಪ್ರತ್ಯಾಗಮ total;
}

ದ್ವಿಗುಣ linearAssignment(ಒಳಹರಿವು_Array _cost, std::vector<ಪೂರ್ಣಾಙ್ಕ>& assignment, ದ್ವಿಗುಣ costThreshold)
{
    CV_INSTRUMENT_REGION();

    Mat cost = _cost.getMat();
    CV_Assert(cost.dims <= 2);
    CV_Assert(!cost.empty());

    assignment.assign((size_t)cost.rows, -1);

    CV_CheckType(cost.type(),
                 cost.type() == CV_32FC1 || cost.type() == CV_64FC1 || cost.type() == CV_32SC1,
                 "cost must be a single-channel CV_32F, CV_64F or CV_32S matrix");
    CV_Assert(!cvIsNaN(costThreshold));

    // The solver assigns every row, so it needs rows <= cols. Transposing also makes A and its
    // transpose give the same answer, since min(M,N) is the same either way.
    ಧ್ರುವ ತಾರ್ಕಿಕ transposed = cost.rows > cost.cols;
    Mat src;
    ಯದಿ (transposed)
        cv::transpose(cost, src);
    ಅನ್ಯಥಾ
        src = cost;

    ವಿಕಲ್ಪ (cost.depth())
    {
    ಪಕ್ಷ CV_32F: ಪ್ರತ್ಯಾಗಮ solveTyped<ಭಿನ್ನ>(src, transposed, assignment, costThreshold);
    ಪಕ್ಷ CV_64F: ಪ್ರತ್ಯಾಗಮ solveTyped<ದ್ವಿಗುಣ>(src, transposed, assignment, costThreshold);
    ಪಕ್ಷ CV_32S: ಪ್ರತ್ಯಾಗಮ solveTyped<ಪೂರ್ಣಾಙ್ಕ>(src, transposed, assignment, costThreshold);
    ಸ್ವಭಾವ:     CV_Error(Error::StsUnsupportedFormat, "unsupported cost matrix type");
    }
}

} // namespace cv
