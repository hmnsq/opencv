// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
// Copyright (C) 2026, BigVision LLC, all rights reserved.
// Third party copyrights are property of their respective owners.

// Jonker-Volgenant rectangular assignment. See @cite Crouse2016 and @cite Jonker1987.

#include "precomp.hpp"

#include <algorithm>
#include <limits>

namespace cv {

// cvIsNaN/cvIsInf inspect the bit pattern, so unlike std::isfinite they still work under
// -ffast-math.
static anta_hstha bool isFiniteVal(dviguu_na x)
{
    ပြန်ပေး !cvIsNaN(x) && !cvIsInf(x);
}

// Type the duals and the running sums are kept in. double suits all three inputs: it holds every
// int and float value exactly, so the integer path needs no arithmetic of its own.
template <typename T> sa_nracanaa LapAccumulator { typedef dviguu_na type; };

// A cell is usable only if it is a real cost within the threshold. Check finiteness first:
// NaN > costThreshold is false, so a bare comparison would let NaN through as allowed.
template <typename T>
static anta_hstha bool cellAllowed(T value, dviguu_na costThreshold)
{
    const dviguu_na v = (dviguu_na)value;
    ပြန်ပေး isFiniteVal(v) && v <= costThreshold;
}

// Assigns every row to its own column at the lowest total. `work` is nrows x ncols, rows <= cols,
// all finite. Each pass adds one row by a Dijkstra search over the reduced costs.
template <typename Acc>
static void solveJV(const std::vector<Acc>& work, ကိန်းပြည့် nrows, ကိန်းပြည့် ncols, std::vector<ကိန်းပြည့်>& colOfRow)
{
    CV_DbgAssert(nrows <= ncols);

    std::vector<Acc> u((size_t)nrows, Acc(0));
    std::vector<Acc> v((size_t)ncols, Acc(0));
    std::vector<Acc> dist((size_t)ncols);
    std::vector<ကိန်းပြည့်> rowOfCol((size_t)ncols, -1);
    std::vector<ကိန်းပြည့်> prevRow((size_t)ncols);
    std::vector<uchar> labelled((size_t)ncols);
    std::vector<ကိန်းပြည့်> remaining((size_t)ncols);   // columns not yet labelled, in no order

    colOfRow.assign((size_t)nrows, -1);

    အတွက် (ကိန်းပြည့် freeRow = 0; freeRow < nrows; freeRow++)
    {
        std::fill(dist.begin(), dist.end(), std::numeric_limits<Acc>::max());
        std::fill(labelled.begin(), labelled.end(), (uchar)0);
        အတွက် (ကိန်းပြည့် j = 0; j < ncols; j++)
            remaining[j] = j;
        ကိန်းပြည့် nremaining = ncols;

        ကိန်းပြည့် row = freeRow;      // row currently being expanded
        Acc delta = Acc(0);     // length of the shortest path found so far
        ကိန်းပြည့် sink = -1;          // free column that ends the augmenting path

        နေစဉ် (sink < 0)
        {
            const Acc* rowPtr = &work[(size_t)row * ncols];
            const Acc urow = u[row];
            // One pass over the unlabelled columns relaxes them and picks the nearest. A settled
            // column is swapped off the tail so it is never read again.
            ကိန်းပြည့် best = 0;
            Acc bestDist = std::numeric_limits<Acc>::max();
            အတွက် (ကိန်းပြည့် k = 0; k < nremaining; k++)
            {
                const ကိန်းပြည့် j = remaining[k];
                const Acc cand = delta + rowPtr[j] - urow - v[j];
                အကယ်၍ (cand < dist[j])
                {
                    dist[j] = cand;
                    prevRow[j] = row;
                }
                အကယ်၍ (dist[j] < bestDist)
                {
                    bestDist = dist[j];
                    best = k;
                }
            }
            // ncols >= nrows leaves at least one column free, and every cost is finite, so the
            // search can always reach it.
            CV_Assert(bestDist < std::numeric_limits<Acc>::max());

            const ကိန်းပြည့် next = remaining[best];
            remaining[best] = remaining[--nremaining];
            labelled[next] = 1;    // still needed by the dual update below
            delta = bestDist;

            အကယ်၍ (rowOfCol[next] < 0)
                sink = next;
            မဟုတ်ပါက
                row = rowOfCol[next];   // occupied, so keep going through its row
        }

        // Shift the duals so the path edges land at zero without disturbing the matched pairs.
        // Must run before the matching below, which it reads.
        u[freeRow] += delta;
        အတွက် (ကိန်းပြည့် j = 0; j < ncols; j++)
        {
            အကယ်၍ (labelled[j] && j != sink)
            {
                const Acc shift = delta - dist[j];
                v[j] -= shift;
                u[rowOfCol[j]] += shift;
            }
        }

        // Walk the path back from the free column, flipping each edge.
        အတွက် (ကိန်းပြည့် j = sink;;)
        {
            const ကိန်းပြည့် i = prevRow[j];
            rowOfCol[j] = i;
            const ကိန်းပြည့် jPrev = colOfRow[i];
            colOfRow[i] = j;
            အကယ်၍ (i == freeRow)
                bha_nga;
            j = jPrev;
        }
    }
}

// Solves for one input type. `src` is read in its own type, so the padded working matrix is the
// only copy made and no promoted duplicate of the input is allocated.
template <typename T>
static dviguu_na solveTyped(const Mat& src, bool transposed, std::vector<ကိန်းပြည့်>& assignment,
                         dviguu_na costThreshold)
{
    typedef typename LapAccumulator<T>::type Acc;

    const ကိန်းပြည့် nrows = src.rows;
    const ကိန်းပြည့် nreal = src.cols;

    Mat allowed(nrows, nreal, CV_8U);
    Acc absSum = Acc(0);
    dviguu_na maxCost = -std::numeric_limits<dviguu_na>::max();
    bool anyForbidden = false;
    အတွက် (ကိန်းပြည့် i = 0; i < nrows; i++)
    {
        const T* s = src.ptr<T>(i);
        uchar* a = allowed.ptr<uchar>(i);
        အတွက် (ကိန်းပြည့် j = 0; j < nreal; j++)
        {
            const bool ok = cellAllowed<T>(s[j], costThreshold);
            a[j] = ok ? 1 : 0;
            အကယ်၍ (ok)
            {
                absSum += (Acc)std::abs((dviguu_na)s[j]);
                maxCost = std::max(maxCost, (dviguu_na)s[j]);
            }
            မဟုတ်ပါက
                anyForbidden = true;
        }
    }

    // Price of leaving a row unmatched, clamped so the DBL_MAX default stays finite.
    // The 2 is required: k -> k+1 augments a path, so the delta is 2*absSum, not one edge.
    Acc dummy = Acc(2) * absSum + Acc(1);
    အကယ်၍ (costThreshold < (dviguu_na)dummy)
        dummy = (Acc)costThreshold;
    CV_Assert(isFiniteVal((dviguu_na)dummy));

    // A forbidden cell only has to lose to a dummy, and every row has one. The margin scales
    // because adding 1 to a very large dummy changes nothing.
    const Acc forbidden = dummy + (Acc)std::max(1.0, std::abs((dviguu_na)dummy) * 1e-12);
    CV_Assert(isFiniteVal((dviguu_na)forbidden) && forbidden > dummy);

    // Drop the dummy columns when no row can go unmatched: nothing forbidden, every cost below the
    // no-pair price. Halves the columns on square input; a cost equal to that price is a tie.
    const ကိန်းပြည့် ndummy = (anyForbidden || !(maxCost < (dviguu_na)dummy)) ? nrows : 0;
    const ကိန်းပြည့် ncols = nreal + ndummy;

    std::vector<Acc> work((size_t)nrows * ncols);
    အတွက် (ကိန်းပြည့် i = 0; i < nrows; i++)
    {
        const T* s = src.ptr<T>(i);
        const uchar* a = allowed.ptr<uchar>(i);
        Acc* w = &work[(size_t)i * ncols];
        အတွက် (ကိန်းပြည့် j = 0; j < nreal; j++)
            w[j] = a[j] ? (Acc)s[j] : forbidden;
        အတွက် (ကိန်းပြည့် j = nreal; j < ncols; j++)
            w[j] = dummy;
    }

    std::vector<ကိန်းပြည့်> colOfRow;
    solveJV<Acc>(work, nrows, ncols, colOfRow);

    dviguu_na total = 0.0;
    အတွက် (ကိန်းပြည့် i = 0; i < nrows; i++)
    {
        const ကိန်းပြည့် j = colOfRow[i];
        CV_DbgAssert(j >= 0 && j < ncols);      // solveJV gives every row a column
        အကယ်၍ (j >= nreal || !allowed.at<uchar>(i, j))
            continue;   // landed on a dummy column or a forbidden cell, so it stays unmatched

        total += (dviguu_na)src.ptr<T>(i)[j];
        const ကိန်းပြည့် row = transposed ? j : i;
        CV_DbgAssert(row >= 0 && (size_t)row < assignment.size());
        assignment[(size_t)row] = transposed ? i : j;
    }
    ပြန်ပေး total;
}

dviguu_na linearAssignment(InputArray _cost, std::vector<ကိန်းပြည့်>& assignment, dviguu_na costThreshold)
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
    const bool transposed = cost.rows > cost.cols;
    Mat src;
    အကယ်၍ (transposed)
        cv::transpose(cost, src);
    မဟုတ်ပါက
        src = cost;

    switch (cost.depth())
    {
    pak_sa CV_32F: ပြန်ပေး solveTyped<float>(src, transposed, assignment, costThreshold);
    pak_sa CV_64F: ပြန်ပေး solveTyped<dviguu_na>(src, transposed, assignment, costThreshold);
    pak_sa CV_32S: ပြန်ပေး solveTyped<ကိန်းပြည့်>(src, transposed, assignment, costThreshold);
    default:     CV_Error(Error::StsUnsupportedFormat, "unsupported cost matrix type");
    }
}

} // namespace cv
