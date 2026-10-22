// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
// Copyright (C) 2026, BigVision LLC, all rights reserved.
// Third party copyrights are property of their respective owners.

// Jonker-Volgenant rectangular assignment. See @cite Crouse2016 and @cite Jonker1987.

#અન્તર્ભાવ "precomp.hpp"

#અન્તર્ભાવ <algorithm>
#અન્તર્ભાવ <limits>

નામાકાશ cv {

// cvIsNaN/cvIsInf inspect the bit pattern, so unlike std::isfinite they still work under
// -ffast-math.
સ્થિર અંતઃરેખ તાર્કિક isFiniteVal(દ્વિગુણ x)
{
    પાછા !cvIsNaN(x) && !cvIsInf(x);
}

// Type the duals and the running sums are kept in. double suits all three inputs: it holds every
// int and float value exactly, so the integer path needs no arithmetic of its own.
template <typename T> સંરચના LapAccumulator { પ્રકારવ્યાખ્યા દ્વિગુણ type; };

// A cell is usable only if it is a real cost within the threshold. Check finiteness first:
// NaN > costThreshold is false, so a bare comparison would let NaN through as allowed.
template <typename T>
સ્થિર અંતઃરેખ તાર્કિક cellAllowed(T value, દ્વિગુણ costThreshold)
{
    અચલ દ્વિગુણ v = (દ્વિગુણ)value;
    પાછા isFiniteVal(v) && v <= costThreshold;
}

// Assigns every row to its own column at the lowest total. `work` is nrows x ncols, rows <= cols,
// all finite. Each pass adds one row by a Dijkstra search over the reduced costs.
template <typename Acc>
સ્થિર શૂન્ય solveJV(અચલ std::vector<Acc>& work, પૂર્ણાંક nrows, પૂર્ણાંક ncols, std::vector<પૂર્ણાંક>& colOfRow)
{
    CV_DbgAssert(nrows <= ncols);

    std::vector<Acc> u((size_t)nrows, Acc(0));
    std::vector<Acc> v((size_t)ncols, Acc(0));
    std::vector<Acc> dist((size_t)ncols);
    std::vector<પૂર્ણાંક> rowOfCol((size_t)ncols, -1);
    std::vector<પૂર્ણાંક> prevRow((size_t)ncols);
    std::vector<uchar> labelled((size_t)ncols);
    std::vector<પૂર્ણાંક> remaining((size_t)ncols);   // columns not yet labelled, in no order

    colOfRow.assign((size_t)nrows, -1);

    માટે (પૂર્ણાંક freeRow = 0; freeRow < nrows; freeRow++)
    {
        std::fill(dist.begin(), dist.end(), std::numeric_limits<Acc>::max());
        std::fill(labelled.begin(), labelled.end(), (uchar)0);
        માટે (પૂર્ણાંક j = 0; j < ncols; j++)
            remaining[j] = j;
        પૂર્ણાંક nremaining = ncols;

        પૂર્ણાંક row = freeRow;      // row currently being expanded
        Acc delta = Acc(0);     // length of the shortest path found so far
        પૂર્ણાંક sink = -1;          // free column that ends the augmenting path

        જ્યારે (sink < 0)
        {
            અચલ Acc* rowPtr = &work[(size_t)row * ncols];
            અચલ Acc urow = u[row];
            // One pass over the unlabelled columns relaxes them and picks the nearest. A settled
            // column is swapped off the tail so it is never read again.
            પૂર્ણાંક best = 0;
            Acc bestDist = std::numeric_limits<Acc>::max();
            માટે (પૂર્ણાંક k = 0; k < nremaining; k++)
            {
                અચલ પૂર્ણાંક j = remaining[k];
                અચલ Acc cand = delta + rowPtr[j] - urow - v[j];
                જો (cand < dist[j])
                {
                    dist[j] = cand;
                    prevRow[j] = row;
                }
                જો (dist[j] < bestDist)
                {
                    bestDist = dist[j];
                    best = k;
                }
            }
            // ncols >= nrows leaves at least one column free, and every cost is finite, so the
            // search can always reach it.
            CV_Assert(bestDist < std::numeric_limits<Acc>::max());

            અચલ પૂર્ણાંક next = remaining[best];
            remaining[best] = remaining[--nremaining];
            labelled[next] = 1;    // still needed by the dual update below
            delta = bestDist;

            જો (rowOfCol[next] < 0)
                sink = next;
            નહીંતર
                row = rowOfCol[next];   // occupied, so keep going through its row
        }

        // Shift the duals so the path edges land at zero without disturbing the matched pairs.
        // Must run before the matching below, which it reads.
        u[freeRow] += delta;
        માટે (પૂર્ણાંક j = 0; j < ncols; j++)
        {
            જો (labelled[j] && j != sink)
            {
                અચલ Acc shift = delta - dist[j];
                v[j] -= shift;
                u[rowOfCol[j]] += shift;
            }
        }

        // Walk the path back from the free column, flipping each edge.
        માટે (પૂર્ણાંક j = sink;;)
        {
            અચલ પૂર્ણાંક i = prevRow[j];
            rowOfCol[j] = i;
            અચલ પૂર્ણાંક jPrev = colOfRow[i];
            colOfRow[i] = j;
            જો (i == freeRow)
                તોડો;
            j = jPrev;
        }
    }
}

// Solves for one input type. `src` is read in its own type, so the padded working matrix is the
// only copy made and no promoted duplicate of the input is allocated.
template <typename T>
સ્થિર દ્વિગુણ solveTyped(અચલ Mat& src, તાર્કિક transposed, std::vector<પૂર્ણાંક>& assignment,
                         દ્વિગુણ costThreshold)
{
    પ્રકારવ્યાખ્યા typename LapAccumulator<T>::type Acc;

    અચલ પૂર્ણાંક nrows = src.rows;
    અચલ પૂર્ણાંક nreal = src.cols;

    Mat allowed(nrows, nreal, CV_8U);
    Acc absSum = Acc(0);
    દ્વિગુણ maxCost = -std::numeric_limits<દ્વિગુણ>::max();
    તાર્કિક anyForbidden = ખોટું;
    માટે (પૂર્ણાંક i = 0; i < nrows; i++)
    {
        અચલ T* s = src.ptr<T>(i);
        uchar* a = allowed.ptr<uchar>(i);
        માટે (પૂર્ણાંક j = 0; j < nreal; j++)
        {
            અચલ તાર્કિક ok = cellAllowed<T>(s[j], costThreshold);
            a[j] = ok ? 1 : 0;
            જો (ok)
            {
                absSum += (Acc)std::abs((દ્વિગુણ)s[j]);
                maxCost = std::max(maxCost, (દ્વિગુણ)s[j]);
            }
            નહીંતર
                anyForbidden = સાચું;
        }
    }

    // Price of leaving a row unmatched, clamped so the DBL_MAX default stays finite.
    // The 2 is required: k -> k+1 augments a path, so the delta is 2*absSum, not one edge.
    Acc dummy = Acc(2) * absSum + Acc(1);
    જો (costThreshold < (દ્વિગુણ)dummy)
        dummy = (Acc)costThreshold;
    CV_Assert(isFiniteVal((દ્વિગુણ)dummy));

    // A forbidden cell only has to lose to a dummy, and every row has one. The margin scales
    // because adding 1 to a very large dummy changes nothing.
    અચલ Acc forbidden = dummy + (Acc)std::max(1.0, std::abs((દ્વિગુણ)dummy) * 1e-12);
    CV_Assert(isFiniteVal((દ્વિગુણ)forbidden) && forbidden > dummy);

    // Drop the dummy columns when no row can go unmatched: nothing forbidden, every cost below the
    // no-pair price. Halves the columns on square input; a cost equal to that price is a tie.
    અચલ પૂર્ણાંક ndummy = (anyForbidden || !(maxCost < (દ્વિગુણ)dummy)) ? nrows : 0;
    અચલ પૂર્ણાંક ncols = nreal + ndummy;

    std::vector<Acc> work((size_t)nrows * ncols);
    માટે (પૂર્ણાંક i = 0; i < nrows; i++)
    {
        અચલ T* s = src.ptr<T>(i);
        અચલ uchar* a = allowed.ptr<uchar>(i);
        Acc* w = &work[(size_t)i * ncols];
        માટે (પૂર્ણાંક j = 0; j < nreal; j++)
            w[j] = a[j] ? (Acc)s[j] : forbidden;
        માટે (પૂર્ણાંક j = nreal; j < ncols; j++)
            w[j] = dummy;
    }

    std::vector<પૂર્ણાંક> colOfRow;
    solveJV<Acc>(work, nrows, ncols, colOfRow);

    દ્વિગુણ total = 0.0;
    માટે (પૂર્ણાંક i = 0; i < nrows; i++)
    {
        અચલ પૂર્ણાંક j = colOfRow[i];
        CV_DbgAssert(j >= 0 && j < ncols);      // solveJV gives every row a column
        જો (j >= nreal || !allowed.at<uchar>(i, j))
            ચાલુ;   // landed on a dummy column or a forbidden cell, so it stays unmatched

        total += (દ્વિગુણ)src.ptr<T>(i)[j];
        અચલ પૂર્ણાંક row = transposed ? j : i;
        CV_DbgAssert(row >= 0 && (size_t)row < assignment.size());
        assignment[(size_t)row] = transposed ? i : j;
    }
    પાછા total;
}

દ્વિગુણ linearAssignment(આગત_Array _cost, std::vector<પૂર્ણાંક>& assignment, દ્વિગુણ costThreshold)
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
    અચલ તાર્કિક transposed = cost.rows > cost.cols;
    Mat src;
    જો (transposed)
        cv::transpose(cost, src);
    નહીંતર
        src = cost;

    પસંદગી (cost.depth())
    {
    સ્થિતિ CV_32F: પાછા solveTyped<દશાંશ>(src, transposed, assignment, costThreshold);
    સ્થિતિ CV_64F: પાછા solveTyped<દ્વિગુણ>(src, transposed, assignment, costThreshold);
    સ્થિતિ CV_32S: પાછા solveTyped<પૂર્ણાંક>(src, transposed, assignment, costThreshold);
    મૂળભૂત:     CV_Error(Error::StsUnsupportedFormat, "unsupported cost matrix type");
    }
}

} // namespace cv
