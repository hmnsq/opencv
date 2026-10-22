// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
// Copyright (C) 2026, BigVision LLC, all rights reserved.
// Third party copyrights are property of their respective owners.

// Jonker-Volgenant rectangular assignment. See @cite Crouse2016 and @cite Jonker1987.

#அந்தர்பாவ "precomp.hpp"

#அந்தர்பாவ <algorithm>
#அந்தர்பாவ <limits>

நாமாகாஶ cv {

// cvIsNaN/cvIsInf inspect the bit pattern, so unlike std::isfinite they still work under
// -ffast-math.
நிலையான உள்வரி தருக்க isFiniteVal(இரட்டை x)
{
    திரும்பு !cvIsNaN(x) && !cvIsInf(x);
}

// Type the duals and the running sums are kept in. double suits all three inputs: it holds every
// int and float value exactly, so the integer path needs no arithmetic of its own.
template <typename T> கட்டமைப்பு LapAccumulator { வகைவரை இரட்டை type; };

// A cell is usable only if it is a real cost within the threshold. Check finiteness first:
// NaN > costThreshold is false, so a bare comparison would let NaN through as allowed.
template <typename T>
நிலையான உள்வரி தருக்க cellAllowed(T value, இரட்டை costThreshold)
{
    மாறிலி இரட்டை v = (இரட்டை)value;
    திரும்பு isFiniteVal(v) && v <= costThreshold;
}

// Assigns every row to its own column at the lowest total. `work` is nrows x ncols, rows <= cols,
// all finite. Each pass adds one row by a Dijkstra search over the reduced costs.
template <typename Acc>
நிலையான வெறுமை solveJV(மாறிலி std::vector<Acc>& work, முழு nrows, முழு ncols, std::vector<முழு>& colOfRow)
{
    CV_DbgAssert(nrows <= ncols);

    std::vector<Acc> u((size_t)nrows, Acc(0));
    std::vector<Acc> v((size_t)ncols, Acc(0));
    std::vector<Acc> dist((size_t)ncols);
    std::vector<முழு> rowOfCol((size_t)ncols, -1);
    std::vector<முழு> prevRow((size_t)ncols);
    std::vector<uchar> labelled((size_t)ncols);
    std::vector<முழு> remaining((size_t)ncols);   // columns not yet labelled, in no order

    colOfRow.assign((size_t)nrows, -1);

    க்கு (முழு freeRow = 0; freeRow < nrows; freeRow++)
    {
        std::fill(dist.begin(), dist.end(), std::numeric_limits<Acc>::max());
        std::fill(labelled.begin(), labelled.end(), (uchar)0);
        க்கு (முழு j = 0; j < ncols; j++)
            remaining[j] = j;
        முழு nremaining = ncols;

        முழு row = freeRow;      // row currently being expanded
        Acc delta = Acc(0);     // length of the shortest path found so far
        முழு sink = -1;          // free column that ends the augmenting path

        போது (sink < 0)
        {
            மாறிலி Acc* rowPtr = &work[(size_t)row * ncols];
            மாறிலி Acc urow = u[row];
            // One pass over the unlabelled columns relaxes them and picks the nearest. A settled
            // column is swapped off the tail so it is never read again.
            முழு best = 0;
            Acc bestDist = std::numeric_limits<Acc>::max();
            க்கு (முழு k = 0; k < nremaining; k++)
            {
                மாறிலி முழு j = remaining[k];
                மாறிலி Acc cand = delta + rowPtr[j] - urow - v[j];
                எனில் (cand < dist[j])
                {
                    dist[j] = cand;
                    prevRow[j] = row;
                }
                எனில் (dist[j] < bestDist)
                {
                    bestDist = dist[j];
                    best = k;
                }
            }
            // ncols >= nrows leaves at least one column free, and every cost is finite, so the
            // search can always reach it.
            CV_Assert(bestDist < std::numeric_limits<Acc>::max());

            மாறிலி முழு next = remaining[best];
            remaining[best] = remaining[--nremaining];
            labelled[next] = 1;    // still needed by the dual update below
            delta = bestDist;

            எனில் (rowOfCol[next] < 0)
                sink = next;
            இல்லையெனில்
                row = rowOfCol[next];   // occupied, so keep going through its row
        }

        // Shift the duals so the path edges land at zero without disturbing the matched pairs.
        // Must run before the matching below, which it reads.
        u[freeRow] += delta;
        க்கு (முழு j = 0; j < ncols; j++)
        {
            எனில் (labelled[j] && j != sink)
            {
                மாறிலி Acc shift = delta - dist[j];
                v[j] -= shift;
                u[rowOfCol[j]] += shift;
            }
        }

        // Walk the path back from the free column, flipping each edge.
        க்கு (முழு j = sink;;)
        {
            மாறிலி முழு i = prevRow[j];
            rowOfCol[j] = i;
            மாறிலி முழு jPrev = colOfRow[i];
            colOfRow[i] = j;
            எனில் (i == freeRow)
                நிறுத்து;
            j = jPrev;
        }
    }
}

// Solves for one input type. `src` is read in its own type, so the padded working matrix is the
// only copy made and no promoted duplicate of the input is allocated.
template <typename T>
நிலையான இரட்டை solveTyped(மாறிலி Mat& src, தருக்க transposed, std::vector<முழு>& assignment,
                         இரட்டை costThreshold)
{
    வகைவரை typename LapAccumulator<T>::type Acc;

    மாறிலி முழு nrows = src.rows;
    மாறிலி முழு nreal = src.cols;

    Mat allowed(nrows, nreal, CV_8U);
    Acc absSum = Acc(0);
    இரட்டை maxCost = -std::numeric_limits<இரட்டை>::max();
    தருக்க anyForbidden = பொய்;
    க்கு (முழு i = 0; i < nrows; i++)
    {
        மாறிலி T* s = src.ptr<T>(i);
        uchar* a = allowed.ptr<uchar>(i);
        க்கு (முழு j = 0; j < nreal; j++)
        {
            மாறிலி தருக்க ok = cellAllowed<T>(s[j], costThreshold);
            a[j] = ok ? 1 : 0;
            எனில் (ok)
            {
                absSum += (Acc)std::abs((இரட்டை)s[j]);
                maxCost = std::max(maxCost, (இரட்டை)s[j]);
            }
            இல்லையெனில்
                anyForbidden = உண்மை;
        }
    }

    // Price of leaving a row unmatched, clamped so the DBL_MAX default stays finite.
    // The 2 is required: k -> k+1 augments a path, so the delta is 2*absSum, not one edge.
    Acc dummy = Acc(2) * absSum + Acc(1);
    எனில் (costThreshold < (இரட்டை)dummy)
        dummy = (Acc)costThreshold;
    CV_Assert(isFiniteVal((இரட்டை)dummy));

    // A forbidden cell only has to lose to a dummy, and every row has one. The margin scales
    // because adding 1 to a very large dummy changes nothing.
    மாறிலி Acc forbidden = dummy + (Acc)std::max(1.0, std::abs((இரட்டை)dummy) * 1e-12);
    CV_Assert(isFiniteVal((இரட்டை)forbidden) && forbidden > dummy);

    // Drop the dummy columns when no row can go unmatched: nothing forbidden, every cost below the
    // no-pair price. Halves the columns on square input; a cost equal to that price is a tie.
    மாறிலி முழு ndummy = (anyForbidden || !(maxCost < (இரட்டை)dummy)) ? nrows : 0;
    மாறிலி முழு ncols = nreal + ndummy;

    std::vector<Acc> work((size_t)nrows * ncols);
    க்கு (முழு i = 0; i < nrows; i++)
    {
        மாறிலி T* s = src.ptr<T>(i);
        மாறிலி uchar* a = allowed.ptr<uchar>(i);
        Acc* w = &work[(size_t)i * ncols];
        க்கு (முழு j = 0; j < nreal; j++)
            w[j] = a[j] ? (Acc)s[j] : forbidden;
        க்கு (முழு j = nreal; j < ncols; j++)
            w[j] = dummy;
    }

    std::vector<முழு> colOfRow;
    solveJV<Acc>(work, nrows, ncols, colOfRow);

    இரட்டை total = 0.0;
    க்கு (முழு i = 0; i < nrows; i++)
    {
        மாறிலி முழு j = colOfRow[i];
        CV_DbgAssert(j >= 0 && j < ncols);      // solveJV gives every row a column
        எனில் (j >= nreal || !allowed.at<uchar>(i, j))
            தொடர்;   // landed on a dummy column or a forbidden cell, so it stays unmatched

        total += (இரட்டை)src.ptr<T>(i)[j];
        மாறிலி முழு row = transposed ? j : i;
        CV_DbgAssert(row >= 0 && (size_t)row < assignment.size());
        assignment[(size_t)row] = transposed ? i : j;
    }
    திரும்பு total;
}

இரட்டை linearAssignment(உள்ளீடு_Array _cost, std::vector<முழு>& assignment, இரட்டை costThreshold)
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
    மாறிலி தருக்க transposed = cost.rows > cost.cols;
    Mat src;
    எனில் (transposed)
        cv::transpose(cost, src);
    இல்லையெனில்
        src = cost;

    தேர்வு (cost.depth())
    {
    நிலை CV_32F: திரும்பு solveTyped<தசம>(src, transposed, assignment, costThreshold);
    நிலை CV_64F: திரும்பு solveTyped<இரட்டை>(src, transposed, assignment, costThreshold);
    நிலை CV_32S: திரும்பு solveTyped<முழு>(src, transposed, assignment, costThreshold);
    இயல்பு:     CV_Error(Error::StsUnsupportedFormat, "unsupported cost matrix type");
    }
}

} // namespace cv
