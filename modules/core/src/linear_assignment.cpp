// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
// Copyright (C) 2026, BigVision LLC, all rights reserved.
// Third party copyrights are property of their respective owners.

// Jonker-Volgenant rectangular assignment. See @cite Crouse2016 and @cite Jonker1987.

#അന്തര്ഭാവ "precomp.hpp"

#അന്തര്ഭാവ <algorithm>
#അന്തര്ഭാവ <limits>

നാമാകാശ cv {

// cvIsNaN/cvIsInf inspect the bit pattern, so unlike std::isfinite they still work under
// -ffast-math.
സ്ഥിതം വരിയിൽ തർക്കം isFiniteVal(ഇരട്ട x)
{
    തിരികെ !cvIsNaN(x) && !cvIsInf(x);
}

// Type the duals and the running sums are kept in. double suits all three inputs: it holds every
// int and float value exactly, so the integer path needs no arithmetic of its own.
template <typename T> ഘടന LapAccumulator { തരനിർവചനം ഇരട്ട type; };

// A cell is usable only if it is a real cost within the threshold. Check finiteness first:
// NaN > costThreshold is false, so a bare comparison would let NaN through as allowed.
template <typename T>
സ്ഥിതം വരിയിൽ തർക്കം cellAllowed(T value, ഇരട്ട costThreshold)
{
    സ്ഥിരം ഇരട്ട v = (ഇരട്ട)value;
    തിരികെ isFiniteVal(v) && v <= costThreshold;
}

// Assigns every row to its own column at the lowest total. `work` is nrows x ncols, rows <= cols,
// all finite. Each pass adds one row by a Dijkstra search over the reduced costs.
template <typename Acc>
സ്ഥിതം ശൂന്യം solveJV(സ്ഥിരം std::vector<Acc>& work, പൂർണ്ണസംഖ്യ nrows, പൂർണ്ണസംഖ്യ ncols, std::vector<പൂർണ്ണസംഖ്യ>& colOfRow)
{
    CV_DbgAssert(nrows <= ncols);

    std::vector<Acc> u((size_t)nrows, Acc(0));
    std::vector<Acc> v((size_t)ncols, Acc(0));
    std::vector<Acc> dist((size_t)ncols);
    std::vector<പൂർണ്ണസംഖ്യ> rowOfCol((size_t)ncols, -1);
    std::vector<പൂർണ്ണസംഖ്യ> prevRow((size_t)ncols);
    std::vector<uchar> labelled((size_t)ncols);
    std::vector<പൂർണ്ണസംഖ്യ> remaining((size_t)ncols);   // columns not yet labelled, in no order

    colOfRow.assign((size_t)nrows, -1);

    വേണ്ടി (പൂർണ്ണസംഖ്യ freeRow = 0; freeRow < nrows; freeRow++)
    {
        std::fill(dist.begin(), dist.end(), std::numeric_limits<Acc>::max());
        std::fill(labelled.begin(), labelled.end(), (uchar)0);
        വേണ്ടി (പൂർണ്ണസംഖ്യ j = 0; j < ncols; j++)
            remaining[j] = j;
        പൂർണ്ണസംഖ്യ nremaining = ncols;

        പൂർണ്ണസംഖ്യ row = freeRow;      // row currently being expanded
        Acc delta = Acc(0);     // length of the shortest path found so far
        പൂർണ്ണസംഖ്യ sink = -1;          // free column that ends the augmenting path

        ആകുമ്പോൾ (sink < 0)
        {
            സ്ഥിരം Acc* rowPtr = &work[(size_t)row * ncols];
            സ്ഥിരം Acc urow = u[row];
            // One pass over the unlabelled columns relaxes them and picks the nearest. A settled
            // column is swapped off the tail so it is never read again.
            പൂർണ്ണസംഖ്യ best = 0;
            Acc bestDist = std::numeric_limits<Acc>::max();
            വേണ്ടി (പൂർണ്ണസംഖ്യ k = 0; k < nremaining; k++)
            {
                സ്ഥിരം പൂർണ്ണസംഖ്യ j = remaining[k];
                സ്ഥിരം Acc cand = delta + rowPtr[j] - urow - v[j];
                എങ്കിൽ (cand < dist[j])
                {
                    dist[j] = cand;
                    prevRow[j] = row;
                }
                എങ്കിൽ (dist[j] < bestDist)
                {
                    bestDist = dist[j];
                    best = k;
                }
            }
            // ncols >= nrows leaves at least one column free, and every cost is finite, so the
            // search can always reach it.
            CV_Assert(bestDist < std::numeric_limits<Acc>::max());

            സ്ഥിരം പൂർണ്ണസംഖ്യ next = remaining[best];
            remaining[best] = remaining[--nremaining];
            labelled[next] = 1;    // still needed by the dual update below
            delta = bestDist;

            എങ്കിൽ (rowOfCol[next] < 0)
                sink = next;
            അല്ലെങ്കിൽ
                row = rowOfCol[next];   // occupied, so keep going through its row
        }

        // Shift the duals so the path edges land at zero without disturbing the matched pairs.
        // Must run before the matching below, which it reads.
        u[freeRow] += delta;
        വേണ്ടി (പൂർണ്ണസംഖ്യ j = 0; j < ncols; j++)
        {
            എങ്കിൽ (labelled[j] && j != sink)
            {
                സ്ഥിരം Acc shift = delta - dist[j];
                v[j] -= shift;
                u[rowOfCol[j]] += shift;
            }
        }

        // Walk the path back from the free column, flipping each edge.
        വേണ്ടി (പൂർണ്ണസംഖ്യ j = sink;;)
        {
            സ്ഥിരം പൂർണ്ണസംഖ്യ i = prevRow[j];
            rowOfCol[j] = i;
            സ്ഥിരം പൂർണ്ണസംഖ്യ jPrev = colOfRow[i];
            colOfRow[i] = j;
            എങ്കിൽ (i == freeRow)
                നിർത്തുക;
            j = jPrev;
        }
    }
}

// Solves for one input type. `src` is read in its own type, so the padded working matrix is the
// only copy made and no promoted duplicate of the input is allocated.
template <typename T>
സ്ഥിതം ഇരട്ട solveTyped(സ്ഥിരം Mat& src, തർക്കം transposed, std::vector<പൂർണ്ണസംഖ്യ>& assignment,
                         ഇരട്ട costThreshold)
{
    തരനിർവചനം typename LapAccumulator<T>::type Acc;

    സ്ഥിരം പൂർണ്ണസംഖ്യ nrows = src.rows;
    സ്ഥിരം പൂർണ്ണസംഖ്യ nreal = src.cols;

    Mat allowed(nrows, nreal, CV_8U);
    Acc absSum = Acc(0);
    ഇരട്ട maxCost = -std::numeric_limits<ഇരട്ട>::max();
    തർക്കം anyForbidden = അസത്യം;
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < nrows; i++)
    {
        സ്ഥിരം T* s = src.ptr<T>(i);
        uchar* a = allowed.ptr<uchar>(i);
        വേണ്ടി (പൂർണ്ണസംഖ്യ j = 0; j < nreal; j++)
        {
            സ്ഥിരം തർക്കം ok = cellAllowed<T>(s[j], costThreshold);
            a[j] = ok ? 1 : 0;
            എങ്കിൽ (ok)
            {
                absSum += (Acc)std::abs((ഇരട്ട)s[j]);
                maxCost = std::max(maxCost, (ഇരട്ട)s[j]);
            }
            അല്ലെങ്കിൽ
                anyForbidden = സത്യം;
        }
    }

    // Price of leaving a row unmatched, clamped so the DBL_MAX default stays finite.
    // The 2 is required: k -> k+1 augments a path, so the delta is 2*absSum, not one edge.
    Acc dummy = Acc(2) * absSum + Acc(1);
    എങ്കിൽ (costThreshold < (ഇരട്ട)dummy)
        dummy = (Acc)costThreshold;
    CV_Assert(isFiniteVal((ഇരട്ട)dummy));

    // A forbidden cell only has to lose to a dummy, and every row has one. The margin scales
    // because adding 1 to a very large dummy changes nothing.
    സ്ഥിരം Acc forbidden = dummy + (Acc)std::max(1.0, std::abs((ഇരട്ട)dummy) * 1e-12);
    CV_Assert(isFiniteVal((ഇരട്ട)forbidden) && forbidden > dummy);

    // Drop the dummy columns when no row can go unmatched: nothing forbidden, every cost below the
    // no-pair price. Halves the columns on square input; a cost equal to that price is a tie.
    സ്ഥിരം പൂർണ്ണസംഖ്യ ndummy = (anyForbidden || !(maxCost < (ഇരട്ട)dummy)) ? nrows : 0;
    സ്ഥിരം പൂർണ്ണസംഖ്യ ncols = nreal + ndummy;

    std::vector<Acc> work((size_t)nrows * ncols);
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < nrows; i++)
    {
        സ്ഥിരം T* s = src.ptr<T>(i);
        സ്ഥിരം uchar* a = allowed.ptr<uchar>(i);
        Acc* w = &work[(size_t)i * ncols];
        വേണ്ടി (പൂർണ്ണസംഖ്യ j = 0; j < nreal; j++)
            w[j] = a[j] ? (Acc)s[j] : forbidden;
        വേണ്ടി (പൂർണ്ണസംഖ്യ j = nreal; j < ncols; j++)
            w[j] = dummy;
    }

    std::vector<പൂർണ്ണസംഖ്യ> colOfRow;
    solveJV<Acc>(work, nrows, ncols, colOfRow);

    ഇരട്ട total = 0.0;
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < nrows; i++)
    {
        സ്ഥിരം പൂർണ്ണസംഖ്യ j = colOfRow[i];
        CV_DbgAssert(j >= 0 && j < ncols);      // solveJV gives every row a column
        എങ്കിൽ (j >= nreal || !allowed.at<uchar>(i, j))
            തുടരുക;   // landed on a dummy column or a forbidden cell, so it stays unmatched

        total += (ഇരട്ട)src.ptr<T>(i)[j];
        സ്ഥിരം പൂർണ്ണസംഖ്യ row = transposed ? j : i;
        CV_DbgAssert(row >= 0 && (size_t)row < assignment.size());
        assignment[(size_t)row] = transposed ? i : j;
    }
    തിരികെ total;
}

ഇരട്ട linearAssignment(നിവേശം_Array _cost, std::vector<പൂർണ്ണസംഖ്യ>& assignment, ഇരട്ട costThreshold)
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
    സ്ഥിരം തർക്കം transposed = cost.rows > cost.cols;
    Mat src;
    എങ്കിൽ (transposed)
        cv::transpose(cost, src);
    അല്ലെങ്കിൽ
        src = cost;

    തിരഞ്ഞെടുപ്പ് (cost.depth())
    {
    അവസ്ഥ CV_32F: തിരികെ solveTyped<ദശാംശം>(src, transposed, assignment, costThreshold);
    അവസ്ഥ CV_64F: തിരികെ solveTyped<ഇരട്ട>(src, transposed, assignment, costThreshold);
    അവസ്ഥ CV_32S: തിരികെ solveTyped<പൂർണ്ണസംഖ്യ>(src, transposed, assignment, costThreshold);
    സ്ഥിരസ്ഥിതി:     CV_Error(Error::StsUnsupportedFormat, "unsupported cost matrix type");
    }
}

} // namespace cv
