// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
// Copyright (C) 2026, BigVision LLC, all rights reserved.
// Third party copyrights are property of their respective owners.

// Jonker-Volgenant rectangular assignment. See @cite Crouse2016 and @cite Jonker1987.

#ଅନ୍ତର୍ଭାଵ "precomp.hpp"

#ଅନ୍ତର୍ଭାଵ <algorithm>
#ଅନ୍ତର୍ଭାଵ <limits>

ନାମାକାଶ cv {

// cvIsNaN/cvIsInf inspect the bit pattern, so unlike std::isfinite they still work under
// -ffast-math.
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ତାର୍କିକ isFiniteVal(ଦ୍ୱିଗୁଣ x)
{
    ଫେରନ୍ତୁ !cvIsNaN(x) && !cvIsInf(x);
}

// Type the duals and the running sums are kept in. double suits all three inputs: it holds every
// int and float value exactly, so the integer path needs no arithmetic of its own.
template <typename T> ସଂରଚନା LapAccumulator { ପ୍ରକାରବ୍ୟାଖ୍ୟା ଦ୍ୱିଗୁଣ type; };

// A cell is usable only if it is a real cost within the threshold. Check finiteness first:
// NaN > costThreshold is false, so a bare comparison would let NaN through as allowed.
template <typename T>
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ତାର୍କିକ cellAllowed(T value, ଦ୍ୱିଗୁଣ costThreshold)
{
    ଅଚଳ ଦ୍ୱିଗୁଣ v = (ଦ୍ୱିଗୁଣ)value;
    ଫେରନ୍ତୁ isFiniteVal(v) && v <= costThreshold;
}

// Assigns every row to its own column at the lowest total. `work` is nrows x ncols, rows <= cols,
// all finite. Each pass adds one row by a Dijkstra search over the reduced costs.
template <typename Acc>
ସ୍ଥିର ଶୂନ୍ୟ solveJV(ଅଚଳ std::vector<Acc>& work, ପୂର୍ଣ୍ଣସଂଖ୍ୟା nrows, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ncols, std::vector<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>& colOfRow)
{
    CV_DbgAssert(nrows <= ncols);

    std::vector<Acc> u((size_t)nrows, Acc(0));
    std::vector<Acc> v((size_t)ncols, Acc(0));
    std::vector<Acc> dist((size_t)ncols);
    std::vector<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> rowOfCol((size_t)ncols, -1);
    std::vector<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> prevRow((size_t)ncols);
    std::vector<uchar> labelled((size_t)ncols);
    std::vector<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> remaining((size_t)ncols);   // columns not yet labelled, in no order

    colOfRow.assign((size_t)nrows, -1);

    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା freeRow = 0; freeRow < nrows; freeRow++)
    {
        std::fill(dist.begin(), dist.end(), std::numeric_limits<Acc>::max());
        std::fill(labelled.begin(), labelled.end(), (uchar)0);
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0; j < ncols; j++)
            remaining[j] = j;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା nremaining = ncols;

        ପୂର୍ଣ୍ଣସଂଖ୍ୟା row = freeRow;      // row currently being expanded
        Acc delta = Acc(0);     // length of the shortest path found so far
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା sink = -1;          // free column that ends the augmenting path

        ଯେତେବେଳେ (sink < 0)
        {
            ଅଚଳ Acc* rowPtr = &work[(size_t)row * ncols];
            ଅଚଳ Acc urow = u[row];
            // One pass over the unlabelled columns relaxes them and picks the nearest. A settled
            // column is swapped off the tail so it is never read again.
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା best = 0;
            Acc bestDist = std::numeric_limits<Acc>::max();
            ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0; k < nremaining; k++)
            {
                ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = remaining[k];
                ଅଚଳ Acc cand = delta + rowPtr[j] - urow - v[j];
                ଯଦି (cand < dist[j])
                {
                    dist[j] = cand;
                    prevRow[j] = row;
                }
                ଯଦି (dist[j] < bestDist)
                {
                    bestDist = dist[j];
                    best = k;
                }
            }
            // ncols >= nrows leaves at least one column free, and every cost is finite, so the
            // search can always reach it.
            CV_Assert(bestDist < std::numeric_limits<Acc>::max());

            ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା next = remaining[best];
            remaining[best] = remaining[--nremaining];
            labelled[next] = 1;    // still needed by the dual update below
            delta = bestDist;

            ଯଦି (rowOfCol[next] < 0)
                sink = next;
            ଅନ୍ୟଥା
                row = rowOfCol[next];   // occupied, so keep going through its row
        }

        // Shift the duals so the path edges land at zero without disturbing the matched pairs.
        // Must run before the matching below, which it reads.
        u[freeRow] += delta;
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0; j < ncols; j++)
        {
            ଯଦି (labelled[j] && j != sink)
            {
                ଅଚଳ Acc shift = delta - dist[j];
                v[j] -= shift;
                u[rowOfCol[j]] += shift;
            }
        }

        // Walk the path back from the free column, flipping each edge.
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = sink;;)
        {
            ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = prevRow[j];
            rowOfCol[j] = i;
            ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା jPrev = colOfRow[i];
            colOfRow[i] = j;
            ଯଦି (i == freeRow)
                ଭାଙ୍ଗ;
            j = jPrev;
        }
    }
}

// Solves for one input type. `src` is read in its own type, so the padded working matrix is the
// only copy made and no promoted duplicate of the input is allocated.
template <typename T>
ସ୍ଥିର ଦ୍ୱିଗୁଣ solveTyped(ଅଚଳ Mat& src, ତାର୍କିକ transposed, std::vector<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>& assignment,
                         ଦ୍ୱିଗୁଣ costThreshold)
{
    ପ୍ରକାରବ୍ୟାଖ୍ୟା typename LapAccumulator<T>::type Acc;

    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା nrows = src.rows;
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା nreal = src.cols;

    Mat allowed(nrows, nreal, CV_8U);
    Acc absSum = Acc(0);
    ଦ୍ୱିଗୁଣ maxCost = -std::numeric_limits<ଦ୍ୱିଗୁଣ>::max();
    ତାର୍କିକ anyForbidden = ମିଥ୍ୟା;
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < nrows; i++)
    {
        ଅଚଳ T* s = src.ptr<T>(i);
        uchar* a = allowed.ptr<uchar>(i);
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0; j < nreal; j++)
        {
            ଅଚଳ ତାର୍କିକ ok = cellAllowed<T>(s[j], costThreshold);
            a[j] = ok ? 1 : 0;
            ଯଦି (ok)
            {
                absSum += (Acc)std::abs((ଦ୍ୱିଗୁଣ)s[j]);
                maxCost = std::max(maxCost, (ଦ୍ୱିଗୁଣ)s[j]);
            }
            ଅନ୍ୟଥା
                anyForbidden = ସତ୍ୟ;
        }
    }

    // Price of leaving a row unmatched, clamped so the DBL_MAX default stays finite.
    // The 2 is required: k -> k+1 augments a path, so the delta is 2*absSum, not one edge.
    Acc dummy = Acc(2) * absSum + Acc(1);
    ଯଦି (costThreshold < (ଦ୍ୱିଗୁଣ)dummy)
        dummy = (Acc)costThreshold;
    CV_Assert(isFiniteVal((ଦ୍ୱିଗୁଣ)dummy));

    // A forbidden cell only has to lose to a dummy, and every row has one. The margin scales
    // because adding 1 to a very large dummy changes nothing.
    ଅଚଳ Acc forbidden = dummy + (Acc)std::max(1.0, std::abs((ଦ୍ୱିଗୁଣ)dummy) * 1e-12);
    CV_Assert(isFiniteVal((ଦ୍ୱିଗୁଣ)forbidden) && forbidden > dummy);

    // Drop the dummy columns when no row can go unmatched: nothing forbidden, every cost below the
    // no-pair price. Halves the columns on square input; a cost equal to that price is a tie.
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା ndummy = (anyForbidden || !(maxCost < (ଦ୍ୱିଗୁଣ)dummy)) ? nrows : 0;
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା ncols = nreal + ndummy;

    std::vector<Acc> work((size_t)nrows * ncols);
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < nrows; i++)
    {
        ଅଚଳ T* s = src.ptr<T>(i);
        ଅଚଳ uchar* a = allowed.ptr<uchar>(i);
        Acc* w = &work[(size_t)i * ncols];
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0; j < nreal; j++)
            w[j] = a[j] ? (Acc)s[j] : forbidden;
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = nreal; j < ncols; j++)
            w[j] = dummy;
    }

    std::vector<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> colOfRow;
    solveJV<Acc>(work, nrows, ncols, colOfRow);

    ଦ୍ୱିଗୁଣ total = 0.0;
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < nrows; i++)
    {
        ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = colOfRow[i];
        CV_DbgAssert(j >= 0 && j < ncols);      // solveJV gives every row a column
        ଯଦି (j >= nreal || !allowed.at<uchar>(i, j))
            ଜାରି;   // landed on a dummy column or a forbidden cell, so it stays unmatched

        total += (ଦ୍ୱିଗୁଣ)src.ptr<T>(i)[j];
        ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା row = transposed ? j : i;
        CV_DbgAssert(row >= 0 && (size_t)row < assignment.size());
        assignment[(size_t)row] = transposed ? i : j;
    }
    ଫେରନ୍ତୁ total;
}

ଦ୍ୱିଗୁଣ linearAssignment(ନିବେଶ_Array _cost, std::vector<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>& assignment, ଦ୍ୱିଗୁଣ costThreshold)
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
    ଅଚଳ ତାର୍କିକ transposed = cost.rows > cost.cols;
    Mat src;
    ଯଦି (transposed)
        cv::transpose(cost, src);
    ଅନ୍ୟଥା
        src = cost;

    ବାଛ (cost.depth())
    {
    ସ୍ଥିତି CV_32F: ଫେରନ୍ତୁ solveTyped<ଦଶମିକ>(src, transposed, assignment, costThreshold);
    ସ୍ଥିତି CV_64F: ଫେରନ୍ତୁ solveTyped<ଦ୍ୱିଗୁଣ>(src, transposed, assignment, costThreshold);
    ସ୍ଥିତି CV_32S: ଫେରନ୍ତୁ solveTyped<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(src, transposed, assignment, costThreshold);
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:     CV_Error(Error::StsUnsupportedFormat, "unsupported cost matrix type");
    }
}

} // namespace cv
