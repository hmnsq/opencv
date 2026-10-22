// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
// Copyright (C) 2026, BigVision LLC, all rights reserved.
// Third party copyrights are property of their respective owners.

// Jonker-Volgenant rectangular assignment. See @cite Crouse2016 and @cite Jonker1987.

#অন্তর্ভাব "precomp.hpp"

#অন্তর্ভাব <algorithm>
#অন্তর্ভাব <limits>

নামাকাশ cv {

// cvIsNaN/cvIsInf inspect the bit pattern, so unlike std::isfinite they still work under
// -ffast-math.
স্থির অন্তঃরেখ যৌক্তিক isFiniteVal(দ্বিগুণ x)
{
    ফেরত !cvIsNaN(x) && !cvIsInf(x);
}

// Type the duals and the running sums are kept in. double suits all three inputs: it holds every
// int and float value exactly, so the integer path needs no arithmetic of its own.
template <typename T> কাঠামো LapAccumulator { প্রকারলেখ দ্বিগুণ type; };

// A cell is usable only if it is a real cost within the threshold. Check finiteness first:
// NaN > costThreshold is false, so a bare comparison would let NaN through as allowed.
template <typename T>
স্থির অন্তঃরেখ যৌক্তিক cellAllowed(T value, দ্বিগুণ costThreshold)
{
    ধ্রুবক দ্বিগুণ v = (দ্বিগুণ)value;
    ফেরত isFiniteVal(v) && v <= costThreshold;
}

// Assigns every row to its own column at the lowest total. `work` is nrows x ncols, rows <= cols,
// all finite. Each pass adds one row by a Dijkstra search over the reduced costs.
template <typename Acc>
স্থির শূন্য solveJV(ধ্রুবক std::vector<Acc>& work, পূর্ণসংখ্যা nrows, পূর্ণসংখ্যা ncols, std::vector<পূর্ণসংখ্যা>& colOfRow)
{
    CV_DbgAssert(nrows <= ncols);

    std::vector<Acc> u((size_t)nrows, Acc(0));
    std::vector<Acc> v((size_t)ncols, Acc(0));
    std::vector<Acc> dist((size_t)ncols);
    std::vector<পূর্ণসংখ্যা> rowOfCol((size_t)ncols, -1);
    std::vector<পূর্ণসংখ্যা> পূর্ব_Row((size_t)ncols);
    std::vector<uchar> labelled((size_t)ncols);
    std::vector<পূর্ণসংখ্যা> remaining((size_t)ncols);   // columns not yet labelled, in no order

    colOfRow.assign((size_t)nrows, -1);

    জন্য (পূর্ণসংখ্যা freeRow = 0; freeRow < nrows; freeRow++)
    {
        std::fill(dist.begin(), dist.end(), std::numeric_limits<Acc>::max());
        std::fill(labelled.begin(), labelled.end(), (uchar)0);
        জন্য (পূর্ণসংখ্যা j = 0; j < ncols; j++)
            remaining[j] = j;
        পূর্ণসংখ্যা nremaining = ncols;

        পূর্ণসংখ্যা row = freeRow;      // row currently being expanded
        Acc delta = Acc(0);     // length of the shortest path found so far
        পূর্ণসংখ্যা sink = -1;          // free column that ends the augmenting path

        যখন (sink < 0)
        {
            ধ্রুবক Acc* rowPtr = &work[(size_t)row * ncols];
            ধ্রুবক Acc urow = u[row];
            // One pass over the unlabelled columns relaxes them and picks the nearest. A settled
            // column is swapped off the tail so it is never read again.
            পূর্ণসংখ্যা best = 0;
            Acc bestDist = std::numeric_limits<Acc>::max();
            জন্য (পূর্ণসংখ্যা k = 0; k < nremaining; k++)
            {
                ধ্রুবক পূর্ণসংখ্যা j = remaining[k];
                ধ্রুবক Acc cand = delta + rowPtr[j] - urow - v[j];
                যদি (cand < dist[j])
                {
                    dist[j] = cand;
                    পূর্ব_Row[j] = row;
                }
                যদি (dist[j] < bestDist)
                {
                    bestDist = dist[j];
                    best = k;
                }
            }
            // ncols >= nrows leaves at least one column free, and every cost is finite, so the
            // search can always reach it.
            CV_Assert(bestDist < std::numeric_limits<Acc>::max());

            ধ্রুবক পূর্ণসংখ্যা next = remaining[best];
            remaining[best] = remaining[--nremaining];
            labelled[next] = 1;    // still needed by the dual update below
            delta = bestDist;

            যদি (rowOfCol[next] < 0)
                sink = next;
            অন্যথা
                row = rowOfCol[next];   // occupied, so keep going through its row
        }

        // Shift the duals so the path edges land at zero without disturbing the matched pairs.
        // Must run before the matching below, which it reads.
        u[freeRow] += delta;
        জন্য (পূর্ণসংখ্যা j = 0; j < ncols; j++)
        {
            যদি (labelled[j] && j != sink)
            {
                ধ্রুবক Acc shift = delta - dist[j];
                v[j] -= shift;
                u[rowOfCol[j]] += shift;
            }
        }

        // Walk the path back from the free column, flipping each edge.
        জন্য (পূর্ণসংখ্যা j = sink;;)
        {
            ধ্রুবক পূর্ণসংখ্যা i = পূর্ব_Row[j];
            rowOfCol[j] = i;
            ধ্রুবক পূর্ণসংখ্যা j_পূর্ব = colOfRow[i];
            colOfRow[i] = j;
            যদি (i == freeRow)
                বিরতি;
            j = j_পূর্ব;
        }
    }
}

// Solves for one input type. `src` is read in its own type, so the padded working matrix is the
// only copy made and no promoted duplicate of the input is allocated.
template <typename T>
স্থির দ্বিগুণ solveTyped(ধ্রুবক Mat& src, যৌক্তিক transposed, std::vector<পূর্ণসংখ্যা>& assignment,
                         দ্বিগুণ costThreshold)
{
    প্রকারলেখ typename LapAccumulator<T>::type Acc;

    ধ্রুবক পূর্ণসংখ্যা nrows = src.rows;
    ধ্রুবক পূর্ণসংখ্যা nreal = src.cols;

    Mat allowed(nrows, nreal, CV_8U);
    Acc abs_যোগ = Acc(0);
    দ্বিগুণ maxCost = -std::numeric_limits<দ্বিগুণ>::max();
    যৌক্তিক anyForbidden = মিথ্যা;
    জন্য (পূর্ণসংখ্যা i = 0; i < nrows; i++)
    {
        ধ্রুবক T* s = src.ptr<T>(i);
        uchar* a = allowed.ptr<uchar>(i);
        জন্য (পূর্ণসংখ্যা j = 0; j < nreal; j++)
        {
            ধ্রুবক যৌক্তিক ok = cellAllowed<T>(s[j], costThreshold);
            a[j] = ok ? 1 : 0;
            যদি (ok)
            {
                abs_যোগ += (Acc)std::abs((দ্বিগুণ)s[j]);
                maxCost = std::max(maxCost, (দ্বিগুণ)s[j]);
            }
            অন্যথা
                anyForbidden = সত্য;
        }
    }

    // Price of leaving a row unmatched, clamped so the DBL_MAX default stays finite.
    // The 2 is required: k -> k+1 augments a path, so the delta is 2*absSum, not one edge.
    Acc dummy = Acc(2) * abs_যোগ + Acc(1);
    যদি (costThreshold < (দ্বিগুণ)dummy)
        dummy = (Acc)costThreshold;
    CV_Assert(isFiniteVal((দ্বিগুণ)dummy));

    // A forbidden cell only has to lose to a dummy, and every row has one. The margin scales
    // because adding 1 to a very large dummy changes nothing.
    ধ্রুবক Acc forbidden = dummy + (Acc)std::max(1.0, std::abs((দ্বিগুণ)dummy) * 1e-12);
    CV_Assert(isFiniteVal((দ্বিগুণ)forbidden) && forbidden > dummy);

    // Drop the dummy columns when no row can go unmatched: nothing forbidden, every cost below the
    // no-pair price. Halves the columns on square input; a cost equal to that price is a tie.
    ধ্রুবক পূর্ণসংখ্যা ndummy = (anyForbidden || !(maxCost < (দ্বিগুণ)dummy)) ? nrows : 0;
    ধ্রুবক পূর্ণসংখ্যা ncols = nreal + ndummy;

    std::vector<Acc> work((size_t)nrows * ncols);
    জন্য (পূর্ণসংখ্যা i = 0; i < nrows; i++)
    {
        ধ্রুবক T* s = src.ptr<T>(i);
        ধ্রুবক uchar* a = allowed.ptr<uchar>(i);
        Acc* w = &work[(size_t)i * ncols];
        জন্য (পূর্ণসংখ্যা j = 0; j < nreal; j++)
            w[j] = a[j] ? (Acc)s[j] : forbidden;
        জন্য (পূর্ণসংখ্যা j = nreal; j < ncols; j++)
            w[j] = dummy;
    }

    std::vector<পূর্ণসংখ্যা> colOfRow;
    solveJV<Acc>(work, nrows, ncols, colOfRow);

    দ্বিগুণ যোগ = 0.0;
    জন্য (পূর্ণসংখ্যা i = 0; i < nrows; i++)
    {
        ধ্রুবক পূর্ণসংখ্যা j = colOfRow[i];
        CV_DbgAssert(j >= 0 && j < ncols);      // solveJV gives every row a column
        যদি (j >= nreal || !allowed.at<uchar>(i, j))
            চালু;   // landed on a dummy column or a forbidden cell, so it stays unmatched

        যোগ += (দ্বিগুণ)src.ptr<T>(i)[j];
        ধ্রুবক পূর্ণসংখ্যা row = transposed ? j : i;
        CV_DbgAssert(row >= 0 && (size_t)row < assignment.size());
        assignment[(size_t)row] = transposed ? i : j;
    }
    ফেরত যোগ;
}

দ্বিগুণ linearAssignment(প্রবেশ_Array _cost, std::vector<পূর্ণসংখ্যা>& assignment, দ্বিগুণ costThreshold)
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
    ধ্রুবক যৌক্তিক transposed = cost.rows > cost.cols;
    Mat src;
    যদি (transposed)
        cv::transpose(cost, src);
    অন্যথা
        src = cost;

    নির্বাচন (cost.depth())
    {
    ক্ষেত্র CV_32F: ফেরত solveTyped<দশমিক>(src, transposed, assignment, costThreshold);
    ক্ষেত্র CV_64F: ফেরত solveTyped<দ্বিগুণ>(src, transposed, assignment, costThreshold);
    ক্ষেত্র CV_32S: ফেরত solveTyped<পূর্ণসংখ্যা>(src, transposed, assignment, costThreshold);
    পূর্বনির্ধারিত:     CV_Error(Error::StsUnsupportedFormat, "unsupported cost matrix type");
    }
}

} // namespace cv
