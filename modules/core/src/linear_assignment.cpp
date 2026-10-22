// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
// Copyright (C) 2026, BigVision LLC, all rights reserved.
// Third party copyrights are property of their respective owners.

// Jonker-Volgenant rectangular assignment. See @cite Crouse2016 and @cite Jonker1987.

#समावेश "precomp.hpp"

#समावेश <algorithm>
#समावेश <limits>

namespace cv {

// cvIsNaN/cvIsInf inspect the bit pattern, so unlike std::isfinite they still work under
// -ffast-math.
अटल अंतरभूत bool isFiniteVal(द्विगुन x)
{
    वापस !cvIsNaN(x) && !cvIsInf(x);
}

// Type the duals and the running sums are kept in. double suits all three inputs: it holds every
// int and float value exactly, so the integer path needs no arithmetic of its own.
ढाँचा <typename T> काष्ठा LapAccumulator { प्रकार द्विगुन type; };

// A cell is usable only if it is a real cost within the threshold. Check finiteness first:
// NaN > costThreshold is false, so a bare comparison would let NaN through as allowed.
ढाँचा <typename T>
अटल अंतरभूत bool cellAllowed(T value, द्विगुन costThreshold)
{
    स्थिर द्विगुन v = (द्विगुन)value;
    वापस isFiniteVal(v) && v <= costThreshold;
}

// Assigns every row to its own column at the lowest total. `work` is nrows x ncols, rows <= cols,
// all finite. Each pass adds one row by a Dijkstra search over the reduced costs.
ढाँचा <typename Acc>
अटल व्योम solveJV(स्थिर std::vector<Acc>& work, पूर्णांक nrows, पूर्णांक ncols, std::vector<पूर्णांक>& colOfRow)
{
    CV_DbgAssert(nrows <= ncols);

    std::vector<Acc> u((माप_प्रकार)nrows, Acc(0));
    std::vector<Acc> v((माप_प्रकार)ncols, Acc(0));
    std::vector<Acc> dist((माप_प्रकार)ncols);
    std::vector<पूर्णांक> rowOfCol((माप_प्रकार)ncols, -1);
    std::vector<पूर्णांक> पूर्व_Row((माप_प्रकार)ncols);
    std::vector<uchar> labelled((माप_प्रकार)ncols);
    std::vector<पूर्णांक> remaining((माप_प्रकार)ncols);   // columns not yet labelled, in no order

    colOfRow.assign((माप_प्रकार)nrows, -1);

    क्रम (पूर्णांक freeRow = 0; freeRow < nrows; freeRow++)
    {
        std::fill(dist.begin(), dist.end(), std::numeric_limits<Acc>::max());
        std::fill(labelled.begin(), labelled.end(), (uchar)0);
        क्रम (पूर्णांक j = 0; j < ncols; j++)
            remaining[j] = j;
        पूर्णांक nremaining = ncols;

        पूर्णांक row = freeRow;      // row currently being expanded
        Acc delta = Acc(0);     // length of the shortest path found so far
        पूर्णांक sink = -1;          // free column that ends the augmenting path

        जबतक (sink < 0)
        {
            स्थिर Acc* rowPtr = &work[(माप_प्रकार)row * ncols];
            स्थिर Acc urow = u[row];
            // One pass over the unlabelled columns relaxes them and picks the nearest. A settled
            // column is swapped off the tail so it is never read again.
            पूर्णांक best = 0;
            Acc bestDist = std::numeric_limits<Acc>::max();
            क्रम (पूर्णांक k = 0; k < nremaining; k++)
            {
                स्थिर पूर्णांक j = remaining[k];
                स्थिर Acc cand = delta + rowPtr[j] - urow - v[j];
                अगर (cand < dist[j])
                {
                    dist[j] = cand;
                    पूर्व_Row[j] = row;
                }
                अगर (dist[j] < bestDist)
                {
                    bestDist = dist[j];
                    best = k;
                }
            }
            // ncols >= nrows leaves at least one column free, and every cost is finite, so the
            // search can always reach it.
            CV_Assert(bestDist < std::numeric_limits<Acc>::max());

            स्थिर पूर्णांक next = remaining[best];
            remaining[best] = remaining[--nremaining];
            labelled[next] = 1;    // still needed by the dual update below
            delta = bestDist;

            अगर (rowOfCol[next] < 0)
                sink = next;
            अन्यथा
                row = rowOfCol[next];   // occupied, so keep going through its row
        }

        // Shift the duals so the path edges land at zero without disturbing the matched pairs.
        // Must run before the matching below, which it reads.
        u[freeRow] += delta;
        क्रम (पूर्णांक j = 0; j < ncols; j++)
        {
            अगर (labelled[j] && j != sink)
            {
                स्थिर Acc shift = delta - dist[j];
                v[j] -= shift;
                u[rowOfCol[j]] += shift;
            }
        }

        // Walk the path back from the free column, flipping each edge.
        क्रम (पूर्णांक j = sink;;)
        {
            स्थिर पूर्णांक i = पूर्व_Row[j];
            rowOfCol[j] = i;
            स्थिर पूर्णांक j_पूर्व = colOfRow[i];
            colOfRow[i] = j;
            अगर (i == freeRow)
                अवरोध;
            j = j_पूर्व;
        }
    }
}

// Solves for one input type. `src` is read in its own type, so the padded working matrix is the
// only copy made and no promoted duplicate of the input is allocated.
ढाँचा <typename T>
अटल द्विगुन solveTyped(स्थिर Mat& src, bool transposed, std::vector<पूर्णांक>& assignment,
                         द्विगुन costThreshold)
{
    प्रकार typename LapAccumulator<T>::type Acc;

    स्थिर पूर्णांक nrows = src.rows;
    स्थिर पूर्णांक nreal = src.cols;

    Mat allowed(nrows, nreal, CV_8U);
    Acc abs_योग = Acc(0);
    द्विगुन maxCost = -std::numeric_limits<द्विगुन>::max();
    bool anyForbidden = false;
    क्रम (पूर्णांक i = 0; i < nrows; i++)
    {
        स्थिर T* s = src.ptr<T>(i);
        uchar* a = allowed.ptr<uchar>(i);
        क्रम (पूर्णांक j = 0; j < nreal; j++)
        {
            स्थिर bool ok = cellAllowed<T>(s[j], costThreshold);
            a[j] = ok ? 1 : 0;
            अगर (ok)
            {
                abs_योग += (Acc)std::असल((द्विगुन)s[j]);
                maxCost = std::max(maxCost, (द्विगुन)s[j]);
            }
            अन्यथा
                anyForbidden = true;
        }
    }

    // Price of leaving a row unmatched, clamped so the DBL_MAX default stays finite.
    // The 2 is required: k -> k+1 augments a path, so the delta is 2*absSum, not one edge.
    Acc dummy = Acc(2) * abs_योग + Acc(1);
    अगर (costThreshold < (द्विगुन)dummy)
        dummy = (Acc)costThreshold;
    CV_Assert(isFiniteVal((द्विगुन)dummy));

    // A forbidden cell only has to lose to a dummy, and every row has one. The margin scales
    // because adding 1 to a very large dummy changes nothing.
    स्थिर Acc forbidden = dummy + (Acc)std::max(1.0, std::असल((द्विगुन)dummy) * 1e-12);
    CV_Assert(isFiniteVal((द्विगुन)forbidden) && forbidden > dummy);

    // Drop the dummy columns when no row can go unmatched: nothing forbidden, every cost below the
    // no-pair price. Halves the columns on square input; a cost equal to that price is a tie.
    स्थिर पूर्णांक ndummy = (anyForbidden || !(maxCost < (द्विगुन)dummy)) ? nrows : 0;
    स्थिर पूर्णांक ncols = nreal + ndummy;

    std::vector<Acc> work((माप_प्रकार)nrows * ncols);
    क्रम (पूर्णांक i = 0; i < nrows; i++)
    {
        स्थिर T* s = src.ptr<T>(i);
        स्थिर uchar* a = allowed.ptr<uchar>(i);
        Acc* w = &work[(माप_प्रकार)i * ncols];
        क्रम (पूर्णांक j = 0; j < nreal; j++)
            w[j] = a[j] ? (Acc)s[j] : forbidden;
        क्रम (पूर्णांक j = nreal; j < ncols; j++)
            w[j] = dummy;
    }

    std::vector<पूर्णांक> colOfRow;
    solveJV<Acc>(work, nrows, ncols, colOfRow);

    द्विगुन योग = 0.0;
    क्रम (पूर्णांक i = 0; i < nrows; i++)
    {
        स्थिर पूर्णांक j = colOfRow[i];
        CV_DbgAssert(j >= 0 && j < ncols);      // solveJV gives every row a column
        अगर (j >= nreal || !allowed.at<uchar>(i, j))
            जारी;   // landed on a dummy column or a forbidden cell, so it stays unmatched

        योग += (द्विगुन)src.ptr<T>(i)[j];
        स्थिर पूर्णांक row = transposed ? j : i;
        CV_DbgAssert(row >= 0 && (माप_प्रकार)row < assignment.size());
        assignment[(माप_प्रकार)row] = transposed ? i : j;
    }
    वापस योग;
}

द्विगुन linearAssignment(निवेश_Array _cost, std::vector<पूर्णांक>& assignment, द्विगुन costThreshold)
{
    CV_INSTRUMENT_REGION();

    Mat cost = _cost.getMat();
    CV_Assert(cost.dims <= 2);
    CV_Assert(!cost.empty());

    assignment.assign((माप_प्रकार)cost.rows, -1);

    CV_CheckType(cost.type(),
                 cost.type() == CV_32FC1 || cost.type() == CV_64FC1 || cost.type() == CV_32SC1,
                 "cost must be a single-channel CV_32F, CV_64F or CV_32S matrix");
    CV_Assert(!cvIsNaN(costThreshold));

    // The solver assigns every row, so it needs rows <= cols. Transposing also makes A and its
    // transpose give the same answer, since min(M,N) is the same either way.
    स्थिर bool transposed = cost.rows > cost.cols;
    Mat src;
    अगर (transposed)
        cv::transpose(cost, src);
    अन्यथा
        src = cost;

    चयन (cost.depth())
    {
    हाल CV_32F: वापस solveTyped<भग्न>(src, transposed, assignment, costThreshold);
    हाल CV_64F: वापस solveTyped<द्विगुन>(src, transposed, assignment, costThreshold);
    हाल CV_32S: वापस solveTyped<पूर्णांक>(src, transposed, assignment, costThreshold);
    शेष:     CV_Error(Error::StsUnsupportedFormat, "unsupported cost matrix type");
    }
}

} // namespace cv
