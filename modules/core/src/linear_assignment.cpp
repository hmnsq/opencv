// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
// Copyright (C) 2026, BigVision LLC, all rights reserved.
// Third party copyrights are property of their respective owners.

// Jonker-Volgenant rectangular assignment. See @cite Crouse2016 and @cite Jonker1987.

#ضمن "precomp.hpp"

#ضمن <algorithm>
#ضمن <limits>

نطاق cv {

// cvIsNaN/cvIsInf inspect the bit pattern, so unlike std::isfinite they still work under
// -ffast-math.
ساكن مضمن منطقي isFiniteVal(مزدوج x)
{
    ܦܢܐ !cvIsNaN(x) && !cvIsInf(x);
}

// Type the duals and the running sums are kept in. double suits all three inputs: it holds every
// int and float value exactly, so the integer path needs no arithmetic of its own.
template <typename T> بنية LapAccumulator { تعريفنوع مزدوج type; };

// A cell is usable only if it is a real cost within the threshold. Check finiteness first:
// NaN > costThreshold is false, so a bare comparison would let NaN through as allowed.
template <typename T>
ساكن مضمن منطقي cellAllowed(T value, مزدوج costThreshold)
{
    ثابت مزدوج v = (مزدوج)value;
    ܦܢܐ isFiniteVal(v) && v <= costThreshold;
}

// Assigns every row to its own column at the lowest total. `work` is nrows x ncols, rows <= cols,
// all finite. Each pass adds one row by a Dijkstra search over the reduced costs.
template <typename Acc>
ساكن فراغ solveJV(ثابت std::vector<Acc>& work, صحيح nrows, صحيح ncols, std::vector<صحيح>& colOfRow)
{
    CV_DbgAssert(nrows <= ncols);

    std::vector<Acc> u((size_t)nrows, Acc(0));
    std::vector<Acc> v((size_t)ncols, Acc(0));
    std::vector<Acc> dist((size_t)ncols);
    std::vector<صحيح> rowOfCol((size_t)ncols, -1);
    std::vector<صحيح> prevRow((size_t)ncols);
    std::vector<uchar> labelled((size_t)ncols);
    std::vector<صحيح> remaining((size_t)ncols);   // columns not yet labelled, in no order

    colOfRow.assign((size_t)nrows, -1);

    لكل (صحيح freeRow = 0; freeRow < nrows; freeRow++)
    {
        std::fill(dist.begin(), dist.end(), std::numeric_limits<Acc>::max());
        std::fill(labelled.begin(), labelled.end(), (uchar)0);
        لكل (صحيح j = 0; j < ncols; j++)
            remaining[j] = j;
        صحيح nremaining = ncols;

        صحيح row = freeRow;      // row currently being expanded
        Acc delta = Acc(0);     // length of the shortest path found so far
        صحيح sink = -1;          // free column that ends the augmenting path

        ܟܡܐ (sink < 0)
        {
            ثابت Acc* rowPtr = &work[(size_t)row * ncols];
            ثابت Acc urow = u[row];
            // One pass over the unlabelled columns relaxes them and picks the nearest. A settled
            // column is swapped off the tail so it is never read again.
            صحيح best = 0;
            Acc bestDist = std::numeric_limits<Acc>::max();
            لكل (صحيح k = 0; k < nremaining; k++)
            {
                ثابت صحيح j = remaining[k];
                ثابت Acc cand = delta + rowPtr[j] - urow - v[j];
                ܐܢ (cand < dist[j])
                {
                    dist[j] = cand;
                    prevRow[j] = row;
                }
                ܐܢ (dist[j] < bestDist)
                {
                    bestDist = dist[j];
                    best = k;
                }
            }
            // ncols >= nrows leaves at least one column free, and every cost is finite, so the
            // search can always reach it.
            CV_Assert(bestDist < std::numeric_limits<Acc>::max());

            ثابت صحيح next = remaining[best];
            remaining[best] = remaining[--nremaining];
            labelled[next] = 1;    // still needed by the dual update below
            delta = bestDist;

            ܐܢ (rowOfCol[next] < 0)
                sink = next;
            ܐܚܪܢܐ
                row = rowOfCol[next];   // occupied, so keep going through its row
        }

        // Shift the duals so the path edges land at zero without disturbing the matched pairs.
        // Must run before the matching below, which it reads.
        u[freeRow] += delta;
        لكل (صحيح j = 0; j < ncols; j++)
        {
            ܐܢ (labelled[j] && j != sink)
            {
                ثابت Acc shift = delta - dist[j];
                v[j] -= shift;
                u[rowOfCol[j]] += shift;
            }
        }

        // Walk the path back from the free column, flipping each edge.
        لكل (صحيح j = sink;;)
        {
            ثابت صحيح i = prevRow[j];
            rowOfCol[j] = i;
            ثابت صحيح jPrev = colOfRow[i];
            colOfRow[i] = j;
            ܐܢ (i == freeRow)
                اكسر;
            j = jPrev;
        }
    }
}

// Solves for one input type. `src` is read in its own type, so the padded working matrix is the
// only copy made and no promoted duplicate of the input is allocated.
template <typename T>
ساكن مزدوج solveTyped(ثابت Mat& src, منطقي transposed, std::vector<صحيح>& assignment,
                         مزدوج costThreshold)
{
    تعريفنوع typename LapAccumulator<T>::type Acc;

    ثابت صحيح nrows = src.rows;
    ثابت صحيح nreal = src.cols;

    Mat allowed(nrows, nreal, CV_8U);
    Acc absSum = Acc(0);
    مزدوج maxCost = -std::numeric_limits<مزدوج>::max();
    منطقي anyForbidden = ܕܓܠܐ;
    لكل (صحيح i = 0; i < nrows; i++)
    {
        ثابت T* s = src.ptr<T>(i);
        uchar* a = allowed.ptr<uchar>(i);
        لكل (صحيح j = 0; j < nreal; j++)
        {
            ثابت منطقي ok = cellAllowed<T>(s[j], costThreshold);
            a[j] = ok ? 1 : 0;
            ܐܢ (ok)
            {
                absSum += (Acc)std::abs((مزدوج)s[j]);
                maxCost = std::max(maxCost, (مزدوج)s[j]);
            }
            ܐܚܪܢܐ
                anyForbidden = ܫܪܪܐ;
        }
    }

    // Price of leaving a row unmatched, clamped so the DBL_MAX default stays finite.
    // The 2 is required: k -> k+1 augments a path, so the delta is 2*absSum, not one edge.
    Acc dummy = Acc(2) * absSum + Acc(1);
    ܐܢ (costThreshold < (مزدوج)dummy)
        dummy = (Acc)costThreshold;
    CV_Assert(isFiniteVal((مزدوج)dummy));

    // A forbidden cell only has to lose to a dummy, and every row has one. The margin scales
    // because adding 1 to a very large dummy changes nothing.
    ثابت Acc forbidden = dummy + (Acc)std::max(1.0, std::abs((مزدوج)dummy) * 1e-12);
    CV_Assert(isFiniteVal((مزدوج)forbidden) && forbidden > dummy);

    // Drop the dummy columns when no row can go unmatched: nothing forbidden, every cost below the
    // no-pair price. Halves the columns on square input; a cost equal to that price is a tie.
    ثابت صحيح ndummy = (anyForbidden || !(maxCost < (مزدوج)dummy)) ? nrows : 0;
    ثابت صحيح ncols = nreal + ndummy;

    std::vector<Acc> work((size_t)nrows * ncols);
    لكل (صحيح i = 0; i < nrows; i++)
    {
        ثابت T* s = src.ptr<T>(i);
        ثابت uchar* a = allowed.ptr<uchar>(i);
        Acc* w = &work[(size_t)i * ncols];
        لكل (صحيح j = 0; j < nreal; j++)
            w[j] = a[j] ? (Acc)s[j] : forbidden;
        لكل (صحيح j = nreal; j < ncols; j++)
            w[j] = dummy;
    }

    std::vector<صحيح> colOfRow;
    solveJV<Acc>(work, nrows, ncols, colOfRow);

    مزدوج total = 0.0;
    لكل (صحيح i = 0; i < nrows; i++)
    {
        ثابت صحيح j = colOfRow[i];
        CV_DbgAssert(j >= 0 && j < ncols);      // solveJV gives every row a column
        ܐܢ (j >= nreal || !allowed.at<uchar>(i, j))
            استمر;   // landed on a dummy column or a forbidden cell, so it stays unmatched

        total += (مزدوج)src.ptr<T>(i)[j];
        ثابت صحيح row = transposed ? j : i;
        CV_DbgAssert(row >= 0 && (size_t)row < assignment.size());
        assignment[(size_t)row] = transposed ? i : j;
    }
    ܦܢܐ total;
}

مزدوج linearAssignment(InputArray _cost, std::vector<صحيح>& assignment, مزدوج costThreshold)
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
    ثابت منطقي transposed = cost.rows > cost.cols;
    Mat src;
    ܐܢ (transposed)
        cv::transpose(cost, src);
    ܐܚܪܢܐ
        src = cost;

    بدل (cost.depth())
    {
    حالة CV_32F: ܦܢܐ solveTyped<عائم>(src, transposed, assignment, costThreshold);
    حالة CV_64F: ܦܢܐ solveTyped<مزدوج>(src, transposed, assignment, costThreshold);
    حالة CV_32S: ܦܢܐ solveTyped<صحيح>(src, transposed, assignment, costThreshold);
    افتراضي:     CV_Error(Error::StsUnsupportedFormat, "unsupported cost matrix type");
    }
}

} // namespace cv
