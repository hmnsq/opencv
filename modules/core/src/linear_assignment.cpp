// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
// Copyright (C) 2026, BigVision LLC, all rights reserved.
// Third party copyrights are property of their respective owners.

// Jonker-Volgenant rectangular assignment. See @cite Crouse2016 and @cite Jonker1987.

#شامل "precomp.hpp"

#شامل <algorithm>
#شامل <limits>

فضاینام cv {

// cvIsNaN/cvIsInf inspect the bit pattern, so unlike std::isfinite they still work under
// -ffast-math.
ایستا درونخطی منطقی isFiniteVal(دوگانه x)
{
    بازگشت !cvIsNaN(x) && !cvIsInf(x);
}

// Type the duals and the running sums are kept in. double suits all three inputs: it holds every
// int and float value exactly, so the integer path needs no arithmetic of its own.
template <typename T> ساختار LapAccumulator { تعریفنوع دوگانه type; };

// A cell is usable only if it is a real cost within the threshold. Check finiteness first:
// NaN > costThreshold is false, so a bare comparison would let NaN through as allowed.
template <typename T>
ایستا درونخطی منطقی cellAllowed(T value, دوگانه costThreshold)
{
    ثابت دوگانه v = (دوگانه)value;
    بازگشت isFiniteVal(v) && v <= costThreshold;
}

// Assigns every row to its own column at the lowest total. `work` is nrows x ncols, rows <= cols,
// all finite. Each pass adds one row by a Dijkstra search over the reduced costs.
template <typename Acc>
ایستا تهی solveJV(ثابت std::vector<Acc>& work, صحیح nrows, صحیح ncols, std::vector<صحیح>& colOfRow)
{
    CV_DbgAssert(nrows <= ncols);

    std::vector<Acc> u((size_t)nrows, Acc(0));
    std::vector<Acc> v((size_t)ncols, Acc(0));
    std::vector<Acc> dist((size_t)ncols);
    std::vector<صحیح> rowOfCol((size_t)ncols, -1);
    std::vector<صحیح> prevRow((size_t)ncols);
    std::vector<uchar> labelled((size_t)ncols);
    std::vector<صحیح> remaining((size_t)ncols);   // columns not yet labelled, in no order

    colOfRow.assign((size_t)nrows, -1);

    برای (صحیح freeRow = 0; freeRow < nrows; freeRow++)
    {
        std::fill(dist.begin(), dist.end(), std::numeric_limits<Acc>::max());
        std::fill(labelled.begin(), labelled.end(), (uchar)0);
        برای (صحیح j = 0; j < ncols; j++)
            remaining[j] = j;
        صحیح nremaining = ncols;

        صحیح row = freeRow;      // row currently being expanded
        Acc delta = Acc(0);     // length of the shortest path found so far
        صحیح sink = -1;          // free column that ends the augmenting path

        وقتی (sink < 0)
        {
            ثابت Acc* rowPtr = &work[(size_t)row * ncols];
            ثابت Acc urow = u[row];
            // One pass over the unlabelled columns relaxes them and picks the nearest. A settled
            // column is swapped off the tail so it is never read again.
            صحیح best = 0;
            Acc bestDist = std::numeric_limits<Acc>::max();
            برای (صحیح k = 0; k < nremaining; k++)
            {
                ثابت صحیح j = remaining[k];
                ثابت Acc cand = delta + rowPtr[j] - urow - v[j];
                اگر (cand < dist[j])
                {
                    dist[j] = cand;
                    prevRow[j] = row;
                }
                اگر (dist[j] < bestDist)
                {
                    bestDist = dist[j];
                    best = k;
                }
            }
            // ncols >= nrows leaves at least one column free, and every cost is finite, so the
            // search can always reach it.
            CV_Assert(bestDist < std::numeric_limits<Acc>::max());

            ثابت صحیح next = remaining[best];
            remaining[best] = remaining[--nremaining];
            labelled[next] = 1;    // still needed by the dual update below
            delta = bestDist;

            اگر (rowOfCol[next] < 0)
                sink = next;
            وگرنه
                row = rowOfCol[next];   // occupied, so keep going through its row
        }

        // Shift the duals so the path edges land at zero without disturbing the matched pairs.
        // Must run before the matching below, which it reads.
        u[freeRow] += delta;
        برای (صحیح j = 0; j < ncols; j++)
        {
            اگر (labelled[j] && j != sink)
            {
                ثابت Acc shift = delta - dist[j];
                v[j] -= shift;
                u[rowOfCol[j]] += shift;
            }
        }

        // Walk the path back from the free column, flipping each edge.
        برای (صحیح j = sink;;)
        {
            ثابت صحیح i = prevRow[j];
            rowOfCol[j] = i;
            ثابت صحیح jPrev = colOfRow[i];
            colOfRow[i] = j;
            اگر (i == freeRow)
                بشکن;
            j = jPrev;
        }
    }
}

// Solves for one input type. `src` is read in its own type, so the padded working matrix is the
// only copy made and no promoted duplicate of the input is allocated.
template <typename T>
ایستا دوگانه solveTyped(ثابت Mat& src, منطقی transposed, std::vector<صحیح>& assignment,
                         دوگانه costThreshold)
{
    تعریفنوع typename LapAccumulator<T>::type Acc;

    ثابت صحیح nrows = src.rows;
    ثابت صحیح nreal = src.cols;

    Mat allowed(nrows, nreal, CV_8U);
    Acc absSum = Acc(0);
    دوگانه maxCost = -std::numeric_limits<دوگانه>::max();
    منطقی anyForbidden = نادرست;
    برای (صحیح i = 0; i < nrows; i++)
    {
        ثابت T* s = src.ptr<T>(i);
        uchar* a = allowed.ptr<uchar>(i);
        برای (صحیح j = 0; j < nreal; j++)
        {
            ثابت منطقی ok = cellAllowed<T>(s[j], costThreshold);
            a[j] = ok ? 1 : 0;
            اگر (ok)
            {
                absSum += (Acc)std::abs((دوگانه)s[j]);
                maxCost = std::max(maxCost, (دوگانه)s[j]);
            }
            وگرنه
                anyForbidden = درست;
        }
    }

    // Price of leaving a row unmatched, clamped so the DBL_MAX default stays finite.
    // The 2 is required: k -> k+1 augments a path, so the delta is 2*absSum, not one edge.
    Acc dummy = Acc(2) * absSum + Acc(1);
    اگر (costThreshold < (دوگانه)dummy)
        dummy = (Acc)costThreshold;
    CV_Assert(isFiniteVal((دوگانه)dummy));

    // A forbidden cell only has to lose to a dummy, and every row has one. The margin scales
    // because adding 1 to a very large dummy changes nothing.
    ثابت Acc forbidden = dummy + (Acc)std::max(1.0, std::abs((دوگانه)dummy) * 1e-12);
    CV_Assert(isFiniteVal((دوگانه)forbidden) && forbidden > dummy);

    // Drop the dummy columns when no row can go unmatched: nothing forbidden, every cost below the
    // no-pair price. Halves the columns on square input; a cost equal to that price is a tie.
    ثابت صحیح ndummy = (anyForbidden || !(maxCost < (دوگانه)dummy)) ? nrows : 0;
    ثابت صحیح ncols = nreal + ndummy;

    std::vector<Acc> work((size_t)nrows * ncols);
    برای (صحیح i = 0; i < nrows; i++)
    {
        ثابت T* s = src.ptr<T>(i);
        ثابت uchar* a = allowed.ptr<uchar>(i);
        Acc* w = &work[(size_t)i * ncols];
        برای (صحیح j = 0; j < nreal; j++)
            w[j] = a[j] ? (Acc)s[j] : forbidden;
        برای (صحیح j = nreal; j < ncols; j++)
            w[j] = dummy;
    }

    std::vector<صحیح> colOfRow;
    solveJV<Acc>(work, nrows, ncols, colOfRow);

    دوگانه total = 0.0;
    برای (صحیح i = 0; i < nrows; i++)
    {
        ثابت صحیح j = colOfRow[i];
        CV_DbgAssert(j >= 0 && j < ncols);      // solveJV gives every row a column
        اگر (j >= nreal || !allowed.at<uchar>(i, j))
            ادامه;   // landed on a dummy column or a forbidden cell, so it stays unmatched

        total += (دوگانه)src.ptr<T>(i)[j];
        ثابت صحیح row = transposed ? j : i;
        CV_DbgAssert(row >= 0 && (size_t)row < assignment.size());
        assignment[(size_t)row] = transposed ? i : j;
    }
    بازگشت total;
}

دوگانه linearAssignment(درون_داد_Array _cost, std::vector<صحیح>& assignment, دوگانه costThreshold)
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
    ثابت منطقی transposed = cost.rows > cost.cols;
    Mat src;
    اگر (transposed)
        cv::transpose(cost, src);
    وگرنه
        src = cost;

    انتخاب (cost.depth())
    {
    حالت CV_32F: بازگشت solveTyped<اعشاری>(src, transposed, assignment, costThreshold);
    حالت CV_64F: بازگشت solveTyped<دوگانه>(src, transposed, assignment, costThreshold);
    حالت CV_32S: بازگشت solveTyped<صحیح>(src, transposed, assignment, costThreshold);
    پیشفرض:     CV_Error(Error::StsUnsupportedFormat, "unsupported cost matrix type");
    }
}

} // namespace cv
