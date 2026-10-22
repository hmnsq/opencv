// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
// Copyright (C) 2026, BigVision LLC, all rights reserved.
// Third party copyrights are property of their respective owners.

// Jonker-Volgenant rectangular assignment. See @cite Crouse2016 and @cite Jonker1987.

#अन्तर्भाव "precomp.hpp"

#अन्तर्भाव <algorithm>
#अन्तर्भाव <limits>

नामाकाश cv {

// cvIsNaN/cvIsInf inspect the bit pattern, so unlike std::isfinite they still work under
// -ffast-math.
स्थिर अन्तःस्थ तार्किक isFiniteVal(द्विगुण x)
{
    प्रत्यागम !cvIsNaN(x) && !cvIsInf(x);
}

// Type the duals and the running sums are kept in. double suits all three inputs: it holds every
// int and float value exactly, so the integer path needs no arithmetic of its own.
template <typename T> संरचना LapAccumulator { प्रकारनिर्देश द्विगुण type; };

// A cell is usable only if it is a real cost within the threshold. Check finiteness first:
// NaN > costThreshold is false, so a bare comparison would let NaN through as allowed.
template <typename T>
स्थिर अन्तःस्थ तार्किक cellAllowed(T value, द्विगुण costThreshold)
{
    ध्रुव द्विगुण v = (द्विगुण)value;
    प्रत्यागम isFiniteVal(v) && v <= costThreshold;
}

// Assigns every row to its own column at the lowest total. `work` is nrows x ncols, rows <= cols,
// all finite. Each pass adds one row by a Dijkstra search over the reduced costs.
template <typename Acc>
स्थिर शून्य solveJV(ध्रुव std::vector<Acc>& work, पूर्णाङ्क nrows, पूर्णाङ्क ncols, std::vector<पूर्णाङ्क>& colOfRow)
{
    CV_DbgAssert(nrows <= ncols);

    std::vector<Acc> u((size_t)nrows, Acc(0));
    std::vector<Acc> v((size_t)ncols, Acc(0));
    std::vector<Acc> dist((size_t)ncols);
    std::vector<पूर्णाङ्क> rowOfCol((size_t)ncols, -1);
    std::vector<पूर्णाङ्क> prevRow((size_t)ncols);
    std::vector<uchar> labelled((size_t)ncols);
    std::vector<पूर्णाङ्क> remaining((size_t)ncols);   // columns not yet labelled, in no order

    colOfRow.assign((size_t)nrows, -1);

    प्रति (पूर्णाङ्क freeRow = 0; freeRow < nrows; freeRow++)
    {
        std::fill(dist.begin(), dist.end(), std::numeric_limits<Acc>::max());
        std::fill(labelled.begin(), labelled.end(), (uchar)0);
        प्रति (पूर्णाङ्क j = 0; j < ncols; j++)
            remaining[j] = j;
        पूर्णाङ्क nremaining = ncols;

        पूर्णाङ्क row = freeRow;      // row currently being expanded
        Acc delta = Acc(0);     // length of the shortest path found so far
        पूर्णाङ्क sink = -1;          // free column that ends the augmenting path

        जाव (sink < 0)
        {
            ध्रुव Acc* rowPtr = &work[(size_t)row * ncols];
            ध्रुव Acc urow = u[row];
            // One pass over the unlabelled columns relaxes them and picks the nearest. A settled
            // column is swapped off the tail so it is never read again.
            पूर्णाङ्क best = 0;
            Acc bestDist = std::numeric_limits<Acc>::max();
            प्रति (पूर्णाङ्क k = 0; k < nremaining; k++)
            {
                ध्रुव पूर्णाङ्क j = remaining[k];
                ध्रुव Acc cand = delta + rowPtr[j] - urow - v[j];
                जइ (cand < dist[j])
                {
                    dist[j] = cand;
                    prevRow[j] = row;
                }
                जइ (dist[j] < bestDist)
                {
                    bestDist = dist[j];
                    best = k;
                }
            }
            // ncols >= nrows leaves at least one column free, and every cost is finite, so the
            // search can always reach it.
            CV_Assert(bestDist < std::numeric_limits<Acc>::max());

            ध्रुव पूर्णाङ्क next = remaining[best];
            remaining[best] = remaining[--nremaining];
            labelled[next] = 1;    // still needed by the dual update below
            delta = bestDist;

            जइ (rowOfCol[next] < 0)
                sink = next;
            अण्णहा
                row = rowOfCol[next];   // occupied, so keep going through its row
        }

        // Shift the duals so the path edges land at zero without disturbing the matched pairs.
        // Must run before the matching below, which it reads.
        u[freeRow] += delta;
        प्रति (पूर्णाङ्क j = 0; j < ncols; j++)
        {
            जइ (labelled[j] && j != sink)
            {
                ध्रुव Acc shift = delta - dist[j];
                v[j] -= shift;
                u[rowOfCol[j]] += shift;
            }
        }

        // Walk the path back from the free column, flipping each edge.
        प्रति (पूर्णाङ्क j = sink;;)
        {
            ध्रुव पूर्णाङ्क i = prevRow[j];
            rowOfCol[j] = i;
            ध्रुव पूर्णाङ्क jPrev = colOfRow[i];
            colOfRow[i] = j;
            जइ (i == freeRow)
                भङ्ग;
            j = jPrev;
        }
    }
}

// Solves for one input type. `src` is read in its own type, so the padded working matrix is the
// only copy made and no promoted duplicate of the input is allocated.
template <typename T>
स्थिर द्विगुण solveTyped(ध्रुव Mat& src, तार्किक transposed, std::vector<पूर्णाङ्क>& assignment,
                         द्विगुण costThreshold)
{
    प्रकारनिर्देश typename LapAccumulator<T>::type Acc;

    ध्रुव पूर्णाङ्क nrows = src.rows;
    ध्रुव पूर्णाङ्क nreal = src.cols;

    Mat allowed(nrows, nreal, CV_8U);
    Acc absSum = Acc(0);
    द्विगुण maxCost = -std::numeric_limits<द्विगुण>::max();
    तार्किक anyForbidden = मिच्छ;
    प्रति (पूर्णाङ्क i = 0; i < nrows; i++)
    {
        ध्रुव T* s = src.ptr<T>(i);
        uchar* a = allowed.ptr<uchar>(i);
        प्रति (पूर्णाङ्क j = 0; j < nreal; j++)
        {
            ध्रुव तार्किक ok = cellAllowed<T>(s[j], costThreshold);
            a[j] = ok ? 1 : 0;
            जइ (ok)
            {
                absSum += (Acc)std::abs((द्विगुण)s[j]);
                maxCost = std::max(maxCost, (द्विगुण)s[j]);
            }
            अण्णहा
                anyForbidden = सच्च;
        }
    }

    // Price of leaving a row unmatched, clamped so the DBL_MAX default stays finite.
    // The 2 is required: k -> k+1 augments a path, so the delta is 2*absSum, not one edge.
    Acc dummy = Acc(2) * absSum + Acc(1);
    जइ (costThreshold < (द्विगुण)dummy)
        dummy = (Acc)costThreshold;
    CV_Assert(isFiniteVal((द्विगुण)dummy));

    // A forbidden cell only has to lose to a dummy, and every row has one. The margin scales
    // because adding 1 to a very large dummy changes nothing.
    ध्रुव Acc forbidden = dummy + (Acc)std::max(1.0, std::abs((द्विगुण)dummy) * 1e-12);
    CV_Assert(isFiniteVal((द्विगुण)forbidden) && forbidden > dummy);

    // Drop the dummy columns when no row can go unmatched: nothing forbidden, every cost below the
    // no-pair price. Halves the columns on square input; a cost equal to that price is a tie.
    ध्रुव पूर्णाङ्क ndummy = (anyForbidden || !(maxCost < (द्विगुण)dummy)) ? nrows : 0;
    ध्रुव पूर्णाङ्क ncols = nreal + ndummy;

    std::vector<Acc> work((size_t)nrows * ncols);
    प्रति (पूर्णाङ्क i = 0; i < nrows; i++)
    {
        ध्रुव T* s = src.ptr<T>(i);
        ध्रुव uchar* a = allowed.ptr<uchar>(i);
        Acc* w = &work[(size_t)i * ncols];
        प्रति (पूर्णाङ्क j = 0; j < nreal; j++)
            w[j] = a[j] ? (Acc)s[j] : forbidden;
        प्रति (पूर्णाङ्क j = nreal; j < ncols; j++)
            w[j] = dummy;
    }

    std::vector<पूर्णाङ्क> colOfRow;
    solveJV<Acc>(work, nrows, ncols, colOfRow);

    द्विगुण total = 0.0;
    प्रति (पूर्णाङ्क i = 0; i < nrows; i++)
    {
        ध्रुव पूर्णाङ्क j = colOfRow[i];
        CV_DbgAssert(j >= 0 && j < ncols);      // solveJV gives every row a column
        जइ (j >= nreal || !allowed.at<uchar>(i, j))
            अनुवर्त;   // landed on a dummy column or a forbidden cell, so it stays unmatched

        total += (द्विगुण)src.ptr<T>(i)[j];
        ध्रुव पूर्णाङ्क row = transposed ? j : i;
        CV_DbgAssert(row >= 0 && (size_t)row < assignment.size());
        assignment[(size_t)row] = transposed ? i : j;
    }
    प्रत्यागम total;
}

द्विगुण linearAssignment(निवेशः_Array _cost, std::vector<पूर्णाङ्क>& assignment, द्विगुण costThreshold)
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
    ध्रुव तार्किक transposed = cost.rows > cost.cols;
    Mat src;
    जइ (transposed)
        cv::transpose(cost, src);
    अण्णहा
        src = cost;

    विकल्प (cost.depth())
    {
    पक्ष CV_32F: प्रत्यागम solveTyped<भिन्न>(src, transposed, assignment, costThreshold);
    पक्ष CV_64F: प्रत्यागम solveTyped<द्विगुण>(src, transposed, assignment, costThreshold);
    पक्ष CV_32S: प्रत्यागम solveTyped<पूर्णाङ्क>(src, transposed, assignment, costThreshold);
    स्वभाव:     CV_Error(Error::StsUnsupportedFormat, "unsupported cost matrix type");
    }
}

} // namespace cv
