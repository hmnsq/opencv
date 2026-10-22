// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
// Copyright (C) 2026, BigVision LLC, all rights reserved.
// Third party copyrights are property of their respective owners.

// Jonker-Volgenant rectangular assignment. See @cite Crouse2016 and @cite Jonker1987.

#অন্তর্ভাৱ "precomp.hpp"

#অন্তর্ভাৱ <algorithm>
#অন্তর্ভাৱ <limits>

নামাকাশ cv {

// cvIsNaN/cvIsInf inspect the bit pattern, so unlike std::isfinite they still work under
// -ffast-math.
স্থির অন্তঃস্থ তার্কিক isFiniteVal(দ্ৱিগুণ x)
{
    ঘূৰাই !cvIsNaN(x) && !cvIsInf(x);
}

// Type the duals and the running sums are kept in. double suits all three inputs: it holds every
// int and float value exactly, so the integer path needs no arithmetic of its own.
template <typename T> সংরচনা LapAccumulator { প্রকারনির্দেশ দ্ৱিগুণ type; };

// A cell is usable only if it is a real cost within the threshold. Check finiteness first:
// NaN > costThreshold is false, so a bare comparison would let NaN through as allowed.
template <typename T>
স্থির অন্তঃস্থ তার্কিক cellAllowed(T value, দ্ৱিগুণ costThreshold)
{
    ধ্রুৱ দ্ৱিগুণ v = (দ্ৱিগুণ)value;
    ঘূৰাই isFiniteVal(v) && v <= costThreshold;
}

// Assigns every row to its own column at the lowest total. `work` is nrows x ncols, rows <= cols,
// all finite. Each pass adds one row by a Dijkstra search over the reduced costs.
template <typename Acc>
স্থির শূন্য solveJV(ধ্রুৱ std::vector<Acc>& work, পূর্ণাঙ্ক nrows, পূর্ণাঙ্ক ncols, std::vector<পূর্ণাঙ্ক>& colOfRow)
{
    CV_DbgAssert(nrows <= ncols);

    std::vector<Acc> u((size_t)nrows, Acc(0));
    std::vector<Acc> v((size_t)ncols, Acc(0));
    std::vector<Acc> dist((size_t)ncols);
    std::vector<পূর্ণাঙ্ক> rowOfCol((size_t)ncols, -1);
    std::vector<পূর্ণাঙ্ক> prevRow((size_t)ncols);
    std::vector<uchar> labelled((size_t)ncols);
    std::vector<পূর্ণাঙ্ক> remaining((size_t)ncols);   // columns not yet labelled, in no order

    colOfRow.assign((size_t)nrows, -1);

    বাবে (পূর্ণাঙ্ক freeRow = 0; freeRow < nrows; freeRow++)
    {
        std::fill(dist.begin(), dist.end(), std::numeric_limits<Acc>::max());
        std::fill(labelled.begin(), labelled.end(), (uchar)0);
        বাবে (পূর্ণাঙ্ক j = 0; j < ncols; j++)
            remaining[j] = j;
        পূর্ণাঙ্ক nremaining = ncols;

        পূর্ণাঙ্ক row = freeRow;      // row currently being expanded
        Acc delta = Acc(0);     // length of the shortest path found so far
        পূর্ণাঙ্ক sink = -1;          // free column that ends the augmenting path

        যেতিয়ালৈ (sink < 0)
        {
            ধ্রুৱ Acc* rowPtr = &work[(size_t)row * ncols];
            ধ্রুৱ Acc urow = u[row];
            // One pass over the unlabelled columns relaxes them and picks the nearest. A settled
            // column is swapped off the tail so it is never read again.
            পূর্ণাঙ্ক best = 0;
            Acc bestDist = std::numeric_limits<Acc>::max();
            বাবে (পূর্ণাঙ্ক k = 0; k < nremaining; k++)
            {
                ধ্রুৱ পূর্ণাঙ্ক j = remaining[k];
                ধ্রুৱ Acc cand = delta + rowPtr[j] - urow - v[j];
                যদি (cand < dist[j])
                {
                    dist[j] = cand;
                    prevRow[j] = row;
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

            ধ্রুৱ পূর্ণাঙ্ক next = remaining[best];
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
        বাবে (পূর্ণাঙ্ক j = 0; j < ncols; j++)
        {
            যদি (labelled[j] && j != sink)
            {
                ধ্রুৱ Acc shift = delta - dist[j];
                v[j] -= shift;
                u[rowOfCol[j]] += shift;
            }
        }

        // Walk the path back from the free column, flipping each edge.
        বাবে (পূর্ণাঙ্ক j = sink;;)
        {
            ধ্রুৱ পূর্ণাঙ্ক i = prevRow[j];
            rowOfCol[j] = i;
            ধ্রুৱ পূর্ণাঙ্ক jPrev = colOfRow[i];
            colOfRow[i] = j;
            যদি (i == freeRow)
                ভঙ্গ;
            j = jPrev;
        }
    }
}

// Solves for one input type. `src` is read in its own type, so the padded working matrix is the
// only copy made and no promoted duplicate of the input is allocated.
template <typename T>
স্থির দ্ৱিগুণ solveTyped(ধ্রুৱ Mat& src, তার্কিক transposed, std::vector<পূর্ণাঙ্ক>& assignment,
                         দ্ৱিগুণ costThreshold)
{
    প্রকারনির্দেশ typename LapAccumulator<T>::type Acc;

    ধ্রুৱ পূর্ণাঙ্ক nrows = src.rows;
    ধ্রুৱ পূর্ণাঙ্ক nreal = src.cols;

    Mat allowed(nrows, nreal, CV_8U);
    Acc absSum = Acc(0);
    দ্ৱিগুণ maxCost = -std::numeric_limits<দ্ৱিগুণ>::max();
    তার্কিক anyForbidden = মিছা;
    বাবে (পূর্ণাঙ্ক i = 0; i < nrows; i++)
    {
        ধ্রুৱ T* s = src.ptr<T>(i);
        uchar* a = allowed.ptr<uchar>(i);
        বাবে (পূর্ণাঙ্ক j = 0; j < nreal; j++)
        {
            ধ্রুৱ তার্কিক ok = cellAllowed<T>(s[j], costThreshold);
            a[j] = ok ? 1 : 0;
            যদি (ok)
            {
                absSum += (Acc)std::abs((দ্ৱিগুণ)s[j]);
                maxCost = std::max(maxCost, (দ্ৱিগুণ)s[j]);
            }
            অন্যথা
                anyForbidden = সঁচা;
        }
    }

    // Price of leaving a row unmatched, clamped so the DBL_MAX default stays finite.
    // The 2 is required: k -> k+1 augments a path, so the delta is 2*absSum, not one edge.
    Acc dummy = Acc(2) * absSum + Acc(1);
    যদি (costThreshold < (দ্ৱিগুণ)dummy)
        dummy = (Acc)costThreshold;
    CV_Assert(isFiniteVal((দ্ৱিগুণ)dummy));

    // A forbidden cell only has to lose to a dummy, and every row has one. The margin scales
    // because adding 1 to a very large dummy changes nothing.
    ধ্রুৱ Acc forbidden = dummy + (Acc)std::max(1.0, std::abs((দ্ৱিগুণ)dummy) * 1e-12);
    CV_Assert(isFiniteVal((দ্ৱিগুণ)forbidden) && forbidden > dummy);

    // Drop the dummy columns when no row can go unmatched: nothing forbidden, every cost below the
    // no-pair price. Halves the columns on square input; a cost equal to that price is a tie.
    ধ্রুৱ পূর্ণাঙ্ক ndummy = (anyForbidden || !(maxCost < (দ্ৱিগুণ)dummy)) ? nrows : 0;
    ধ্রুৱ পূর্ণাঙ্ক ncols = nreal + ndummy;

    std::vector<Acc> work((size_t)nrows * ncols);
    বাবে (পূর্ণাঙ্ক i = 0; i < nrows; i++)
    {
        ধ্রুৱ T* s = src.ptr<T>(i);
        ধ্রুৱ uchar* a = allowed.ptr<uchar>(i);
        Acc* w = &work[(size_t)i * ncols];
        বাবে (পূর্ণাঙ্ক j = 0; j < nreal; j++)
            w[j] = a[j] ? (Acc)s[j] : forbidden;
        বাবে (পূর্ণাঙ্ক j = nreal; j < ncols; j++)
            w[j] = dummy;
    }

    std::vector<পূর্ণাঙ্ক> colOfRow;
    solveJV<Acc>(work, nrows, ncols, colOfRow);

    দ্ৱিগুণ total = 0.0;
    বাবে (পূর্ণাঙ্ক i = 0; i < nrows; i++)
    {
        ধ্রুৱ পূর্ণাঙ্ক j = colOfRow[i];
        CV_DbgAssert(j >= 0 && j < ncols);      // solveJV gives every row a column
        যদি (j >= nreal || !allowed.at<uchar>(i, j))
            অনুৱর্ত;   // landed on a dummy column or a forbidden cell, so it stays unmatched

        total += (দ্ৱিগুণ)src.ptr<T>(i)[j];
        ধ্রুৱ পূর্ণাঙ্ক row = transposed ? j : i;
        CV_DbgAssert(row >= 0 && (size_t)row < assignment.size());
        assignment[(size_t)row] = transposed ? i : j;
    }
    ঘূৰাই total;
}

দ্ৱিগুণ linearAssignment(প্ৰবেশ_Array _cost, std::vector<পূর্ণাঙ্ক>& assignment, দ্ৱিগুণ costThreshold)
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
    ধ্রুৱ তার্কিক transposed = cost.rows > cost.cols;
    Mat src;
    যদি (transposed)
        cv::transpose(cost, src);
    অন্যথা
        src = cost;

    ৱিকল্প (cost.depth())
    {
    পক্ষ CV_32F: ঘূৰাই solveTyped<ভিন্ন>(src, transposed, assignment, costThreshold);
    পক্ষ CV_64F: ঘূৰাই solveTyped<দ্ৱিগুণ>(src, transposed, assignment, costThreshold);
    পক্ষ CV_32S: ঘূৰাই solveTyped<পূর্ণাঙ্ক>(src, transposed, assignment, costThreshold);
    স্ৱভাৱ:     CV_Error(Error::StsUnsupportedFormat, "unsupported cost matrix type");
    }
}

} // namespace cv
