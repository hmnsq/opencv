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
स्थिर ओळीत तार्किक isFiniteVal(द्विगुण x)
{
    परत !cvIsNaN(x) && !cvIsInf(x);
}

// Type the duals and the running sums are kept in. double suits all three inputs: it holds every
// int and float value exactly, so the integer path needs no arithmetic of its own.
ढाँचा <typename T> संरचना LapAccumulator { प्रकारव्याख्या द्विगुण type; };

// A cell is usable only if it is a real cost within the threshold. Check finiteness first:
// NaN > costThreshold is false, so a bare comparison would let NaN through as allowed.
ढाँचा <typename T>
स्थिर ओळीत तार्किक cellAllowed(T value, द्विगुण costThreshold)
{
    स्थिरांक द्विगुण v = (द्विगुण)value;
    परत isFiniteVal(v) && v <= costThreshold;
}

// Assigns every row to its own column at the lowest total. `work` is nrows x ncols, rows <= cols,
// all finite. Each pass adds one row by a Dijkstra search over the reduced costs.
ढाँचा <typename Acc>
स्थिर शून्य solveJV(स्थिरांक std::vector<Acc>& work, पूर्णांक nrows, पूर्णांक ncols, std::vector<पूर्णांक>& colOfRow)
{
    CV_DbgAssert(nrows <= ncols);

    std::vector<Acc> u((माप_प्रकार)nrows, Acc(0));
    std::vector<Acc> v((माप_प्रकार)ncols, Acc(0));
    std::vector<Acc> dist((माप_प्रकार)ncols);
    std::vector<पूर्णांक> rowOfCol((माप_प्रकार)ncols, -1);
    std::vector<पूर्णांक> prevRow((माप_प्रकार)ncols);
    std::vector<uchar> labelled((माप_प्रकार)ncols);
    std::vector<पूर्णांक> remaining((माप_प्रकार)ncols);   // columns not yet labelled, in no order

    colOfRow.assign((माप_प्रकार)nrows, -1);

    साठी (पूर्णांक freeRow = 0; freeRow < nrows; freeRow++)
    {
        std::fill(dist.begin(), dist.end(), std::numeric_limits<Acc>::max());
        std::fill(labelled.begin(), labelled.end(), (uchar)0);
        साठी (पूर्णांक j = 0; j < ncols; j++)
            remaining[j] = j;
        पूर्णांक nremaining = ncols;

        पूर्णांक row = freeRow;      // row currently being expanded
        Acc delta = Acc(0);     // length of the shortest path found so far
        पूर्णांक sink = -1;          // free column that ends the augmenting path

        पर्यंत (sink < 0)
        {
            स्थिरांक Acc* rowPtr = &work[(माप_प्रकार)row * ncols];
            स्थिरांक Acc urow = u[row];
            // One pass over the unlabelled columns relaxes them and picks the nearest. A settled
            // column is swapped off the tail so it is never read again.
            पूर्णांक best = 0;
            Acc bestDist = std::numeric_limits<Acc>::max();
            साठी (पूर्णांक k = 0; k < nremaining; k++)
            {
                स्थिरांक पूर्णांक j = remaining[k];
                स्थिरांक Acc cand = delta + rowPtr[j] - urow - v[j];
                जर (cand < dist[j])
                {
                    dist[j] = cand;
                    prevRow[j] = row;
                }
                जर (dist[j] < bestDist)
                {
                    bestDist = dist[j];
                    best = k;
                }
            }
            // ncols >= nrows leaves at least one column free, and every cost is finite, so the
            // search can always reach it.
            CV_Assert(bestDist < std::numeric_limits<Acc>::max());

            स्थिरांक पूर्णांक next = remaining[best];
            remaining[best] = remaining[--nremaining];
            labelled[next] = 1;    // still needed by the dual update below
            delta = bestDist;

            जर (rowOfCol[next] < 0)
                sink = next;
            अन्यथा
                row = rowOfCol[next];   // occupied, so keep going through its row
        }

        // Shift the duals so the path edges land at zero without disturbing the matched pairs.
        // Must run before the matching below, which it reads.
        u[freeRow] += delta;
        साठी (पूर्णांक j = 0; j < ncols; j++)
        {
            जर (labelled[j] && j != sink)
            {
                स्थिरांक Acc shift = delta - dist[j];
                v[j] -= shift;
                u[rowOfCol[j]] += shift;
            }
        }

        // Walk the path back from the free column, flipping each edge.
        साठी (पूर्णांक j = sink;;)
        {
            स्थिरांक पूर्णांक i = prevRow[j];
            rowOfCol[j] = i;
            स्थिरांक पूर्णांक jPrev = colOfRow[i];
            colOfRow[i] = j;
            जर (i == freeRow)
                थांबा;
            j = jPrev;
        }
    }
}

// Solves for one input type. `src` is read in its own type, so the padded working matrix is the
// only copy made and no promoted duplicate of the input is allocated.
ढाँचा <typename T>
स्थिर द्विगुण solveTyped(स्थिरांक Mat& src, तार्किक transposed, std::vector<पूर्णांक>& assignment,
                         द्विगुण costThreshold)
{
    प्रकारव्याख्या typename LapAccumulator<T>::type Acc;

    स्थिरांक पूर्णांक nrows = src.rows;
    स्थिरांक पूर्णांक nreal = src.cols;

    Mat allowed(nrows, nreal, CV_8U);
    Acc absSum = Acc(0);
    द्विगुण maxCost = -std::numeric_limits<द्विगुण>::max();
    तार्किक anyForbidden = असत्य;
    साठी (पूर्णांक i = 0; i < nrows; i++)
    {
        स्थिरांक T* s = src.ptr<T>(i);
        uchar* a = allowed.ptr<uchar>(i);
        साठी (पूर्णांक j = 0; j < nreal; j++)
        {
            स्थिरांक तार्किक ok = cellAllowed<T>(s[j], costThreshold);
            a[j] = ok ? 1 : 0;
            जर (ok)
            {
                absSum += (Acc)std::असल((द्विगुण)s[j]);
                maxCost = std::max(maxCost, (द्विगुण)s[j]);
            }
            अन्यथा
                anyForbidden = सत्य;
        }
    }

    // Price of leaving a row unmatched, clamped so the DBL_MAX default stays finite.
    // The 2 is required: k -> k+1 augments a path, so the delta is 2*absSum, not one edge.
    Acc dummy = Acc(2) * absSum + Acc(1);
    जर (costThreshold < (द्विगुण)dummy)
        dummy = (Acc)costThreshold;
    CV_Assert(isFiniteVal((द्विगुण)dummy));

    // A forbidden cell only has to lose to a dummy, and every row has one. The margin scales
    // because adding 1 to a very large dummy changes nothing.
    स्थिरांक Acc forbidden = dummy + (Acc)std::max(1.0, std::असल((द्विगुण)dummy) * 1e-12);
    CV_Assert(isFiniteVal((द्विगुण)forbidden) && forbidden > dummy);

    // Drop the dummy columns when no row can go unmatched: nothing forbidden, every cost below the
    // no-pair price. Halves the columns on square input; a cost equal to that price is a tie.
    स्थिरांक पूर्णांक ndummy = (anyForbidden || !(maxCost < (द्विगुण)dummy)) ? nrows : 0;
    स्थिरांक पूर्णांक ncols = nreal + ndummy;

    std::vector<Acc> work((माप_प्रकार)nrows * ncols);
    साठी (पूर्णांक i = 0; i < nrows; i++)
    {
        स्थिरांक T* s = src.ptr<T>(i);
        स्थिरांक uchar* a = allowed.ptr<uchar>(i);
        Acc* w = &work[(माप_प्रकार)i * ncols];
        साठी (पूर्णांक j = 0; j < nreal; j++)
            w[j] = a[j] ? (Acc)s[j] : forbidden;
        साठी (पूर्णांक j = nreal; j < ncols; j++)
            w[j] = dummy;
    }

    std::vector<पूर्णांक> colOfRow;
    solveJV<Acc>(work, nrows, ncols, colOfRow);

    द्विगुण total = 0.0;
    साठी (पूर्णांक i = 0; i < nrows; i++)
    {
        स्थिरांक पूर्णांक j = colOfRow[i];
        CV_DbgAssert(j >= 0 && j < ncols);      // solveJV gives every row a column
        जर (j >= nreal || !allowed.at<uchar>(i, j))
            सुरूठेवा;   // landed on a dummy column or a forbidden cell, so it stays unmatched

        total += (द्विगुण)src.ptr<T>(i)[j];
        स्थिरांक पूर्णांक row = transposed ? j : i;
        CV_DbgAssert(row >= 0 && (माप_प्रकार)row < assignment.size());
        assignment[(माप_प्रकार)row] = transposed ? i : j;
    }
    परत total;
}

द्विगुण linearAssignment(आदान_Array _cost, std::vector<पूर्णांक>& assignment, द्विगुण costThreshold)
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
    स्थिरांक तार्किक transposed = cost.rows > cost.cols;
    Mat src;
    जर (transposed)
        cv::transpose(cost, src);
    अन्यथा
        src = cost;

    निवड (cost.depth())
    {
    स्थिती CV_32F: परत solveTyped<दशांश>(src, transposed, assignment, costThreshold);
    स्थिती CV_64F: परत solveTyped<द्विगुण>(src, transposed, assignment, costThreshold);
    स्थिती CV_32S: परत solveTyped<पूर्णांक>(src, transposed, assignment, costThreshold);
    पूर्वनिर्धारित:     CV_Error(Error::StsUnsupportedFormat, "unsupported cost matrix type");
    }
}

} // namespace cv
