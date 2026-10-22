// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
// Copyright (C) 2026, BigVision LLC, all rights reserved.
// Third party copyrights are property of their respective owners.

// Jonker-Volgenant rectangular assignment. See @cite Crouse2016 and @cite Jonker1987.

#అన్తర్భావ "precomp.hpp"

#అన్తర్భావ <algorithm>
#అన్తర్భావ <limits>

నామాకాశ cv {

// cvIsNaN/cvIsInf inspect the bit pattern, so unlike std::isfinite they still work under
// -ffast-math.
స్థిర అంతఃవరుస తార్కిక isFiniteVal(ద్విగుణ x)
{
    తిరిగి !cvIsNaN(x) && !cvIsInf(x);
}

// Type the duals and the running sums are kept in. double suits all three inputs: it holds every
// int and float value exactly, so the integer path needs no arithmetic of its own.
మూస <రకం_పేరు T> నిర్మాణం Lap_సంచాయకం { రకనిర్వచనం ద్విగుణ type; };

// A cell is usable only if it is a real cost within the threshold. Check finiteness first:
// NaN > costThreshold is false, so a bare comparison would let NaN through as allowed.
మూస <రకం_పేరు T>
స్థిర అంతఃవరుస తార్కిక cellAllowed(T value, ద్విగుణ ఖర్చు_పరిమితి)
{
    స్థిరరాశి ద్విగుణ v = (ద్విగుణ)value;
    తిరిగి isFiniteVal(v) && v <= ఖర్చు_పరిమితి;
}

// Assigns every row to its own column at the lowest total. `work` is nrows x ncols, rows <= cols,
// all finite. Each pass adds one row by a Dijkstra search over the reduced costs.
మూస <రకం_పేరు Acc>
స్థిర శూన్య solveJV(స్థిరరాశి std::vector<Acc>& work, పూర్ణాంకం nrows, పూర్ణాంకం ncols, std::vector<పూర్ణాంకం>& నిలువు_యొక్క_అడ్డువరుస)
{
    CV_DbgAssert(nrows <= ncols);

    std::vector<Acc> u((size_t)nrows, Acc(0));
    std::vector<Acc> v((size_t)ncols, Acc(0));
    std::vector<Acc> dist((size_t)ncols);
    std::vector<పూర్ణాంకం> అడ్డువరుస_యొక్క_నిలువు((size_t)ncols, -1);
    std::vector<పూర్ణాంకం> prev_అడ్డువరుస((size_t)ncols);
    std::vector<uchar> labelled((size_t)ncols);
    std::vector<పూర్ణాంకం> మిగిలిన((size_t)ncols);   // columns not yet labelled, in no order

    నిలువు_యొక్క_అడ్డువరుస.assign((size_t)nrows, -1);

    కొరకు (పూర్ణాంకం free_అడ్డువరుస = 0; free_అడ్డువరుస < nrows; free_అడ్డువరుస++)
    {
        std::fill(dist.begin(), dist.end(), std::numeric_limits<Acc>::max());
        std::fill(labelled.begin(), labelled.end(), (uchar)0);
        కొరకు (పూర్ణాంకం j = 0; j < ncols; j++)
            మిగిలిన[j] = j;
        పూర్ణాంకం nremaining = ncols;

        పూర్ణాంకం అడ్డువరుస = free_అడ్డువరుస;      // row currently being expanded
        Acc delta = Acc(0);     // length of the shortest path found so far
        పూర్ణాంకం sink = -1;          // free column that ends the augmenting path

        అయితే (sink < 0)
        {
            స్థిరరాశి Acc* అడ్డువరుస_Ptr = &work[(size_t)అడ్డువరుస * ncols];
            స్థిరరాశి Acc urow = u[అడ్డువరుస];
            // One pass over the unlabelled columns relaxes them and picks the nearest. A settled
            // column is swapped off the tail so it is never read again.
            పూర్ణాంకం ఉత్తమ = 0;
            Acc ఉత్తమ_Dist = std::numeric_limits<Acc>::max();
            కొరకు (పూర్ణాంకం k = 0; k < nremaining; k++)
            {
                స్థిరరాశి పూర్ణాంకం j = మిగిలిన[k];
                స్థిరరాశి Acc cand = delta + అడ్డువరుస_Ptr[j] - urow - v[j];
                ఒకవేళ (cand < dist[j])
                {
                    dist[j] = cand;
                    prev_అడ్డువరుస[j] = అడ్డువరుస;
                }
                ఒకవేళ (dist[j] < ఉత్తమ_Dist)
                {
                    ఉత్తమ_Dist = dist[j];
                    ఉత్తమ = k;
                }
            }
            // ncols >= nrows leaves at least one column free, and every cost is finite, so the
            // search can always reach it.
            CV_Assert(ఉత్తమ_Dist < std::numeric_limits<Acc>::max());

            స్థిరరాశి పూర్ణాంకం next = మిగిలిన[ఉత్తమ];
            మిగిలిన[ఉత్తమ] = మిగిలిన[--nremaining];
            labelled[next] = 1;    // still needed by the dual update below
            delta = ఉత్తమ_Dist;

            ఒకవేళ (అడ్డువరుస_యొక్క_నిలువు[next] < 0)
                sink = next;
            లేకపోతే
                అడ్డువరుస = అడ్డువరుస_యొక్క_నిలువు[next];   // occupied, so keep going through its row
        }

        // Shift the duals so the path edges land at zero without disturbing the matched pairs.
        // Must run before the matching below, which it reads.
        u[free_అడ్డువరుస] += delta;
        కొరకు (పూర్ణాంకం j = 0; j < ncols; j++)
        {
            ఒకవేళ (labelled[j] && j != sink)
            {
                స్థిరరాశి Acc జరుపు = delta - dist[j];
                v[j] -= జరుపు;
                u[అడ్డువరుస_యొక్క_నిలువు[j]] += జరుపు;
            }
        }

        // Walk the path back from the free column, flipping each edge.
        కొరకు (పూర్ణాంకం j = sink;;)
        {
            స్థిరరాశి పూర్ణాంకం i = prev_అడ్డువరుస[j];
            అడ్డువరుస_యొక్క_నిలువు[j] = i;
            స్థిరరాశి పూర్ణాంకం jPrev = నిలువు_యొక్క_అడ్డువరుస[i];
            నిలువు_యొక్క_అడ్డువరుస[i] = j;
            ఒకవేళ (i == free_అడ్డువరుస)
                ఆపు;
            j = jPrev;
        }
    }
}

// Solves for one input type. `src` is read in its own type, so the padded working matrix is the
// only copy made and no promoted duplicate of the input is allocated.
మూస <రకం_పేరు T>
స్థిర ద్విగుణ solveTyped(స్థిరరాశి Mat& src, తార్కిక transposed, std::vector<పూర్ణాంకం>& assignment,
                         ద్విగుణ ఖర్చు_పరిమితి)
{
    రకనిర్వచనం రకం_పేరు Lap_సంచాయకం<T>::type Acc;

    స్థిరరాశి పూర్ణాంకం nrows = src.అడ్డువరుసలు;
    స్థిరరాశి పూర్ణాంకం nreal = src.నిలువుల_సంఖ్య;

    Mat allowed(nrows, nreal, CV_8U);
    Acc abs_కూడిక = Acc(0);
    ద్విగుణ max_ఖర్చు = -std::numeric_limits<ద్విగుణ>::max();
    తార్కిక anyForbidden = అబద్ధం;
    కొరకు (పూర్ణాంకం i = 0; i < nrows; i++)
    {
        స్థిరరాశి T* s = src.ptr<T>(i);
        uchar* a = allowed.ptr<uchar>(i);
        కొరకు (పూర్ణాంకం j = 0; j < nreal; j++)
        {
            స్థిరరాశి తార్కిక ok = cellAllowed<T>(s[j], ఖర్చు_పరిమితి);
            a[j] = ok ? 1 : 0;
            ఒకవేళ (ok)
            {
                abs_కూడిక += (Acc)std::నిరపేక్ష((ద్విగుణ)s[j]);
                max_ఖర్చు = std::max(max_ఖర్చు, (ద్విగుణ)s[j]);
            }
            లేకపోతే
                anyForbidden = నిజం;
        }
    }

    // Price of leaving a row unmatched, clamped so the DBL_MAX default stays finite.
    // The 2 is required: k -> k+1 augments a path, so the delta is 2*absSum, not one edge.
    Acc dummy = Acc(2) * abs_కూడిక + Acc(1);
    ఒకవేళ (ఖర్చు_పరిమితి < (ద్విగుణ)dummy)
        dummy = (Acc)ఖర్చు_పరిమితి;
    CV_Assert(isFiniteVal((ద్విగుణ)dummy));

    // A forbidden cell only has to lose to a dummy, and every row has one. The margin scales
    // because adding 1 to a very large dummy changes nothing.
    స్థిరరాశి Acc forbidden = dummy + (Acc)std::max(1.0, std::నిరపేక్ష((ద్విగుణ)dummy) * 1e-12);
    CV_Assert(isFiniteVal((ద్విగుణ)forbidden) && forbidden > dummy);

    // Drop the dummy columns when no row can go unmatched: nothing forbidden, every cost below the
    // no-pair price. Halves the columns on square input; a cost equal to that price is a tie.
    స్థిరరాశి పూర్ణాంకం ndummy = (anyForbidden || !(max_ఖర్చు < (ద్విగుణ)dummy)) ? nrows : 0;
    స్థిరరాశి పూర్ణాంకం ncols = nreal + ndummy;

    std::vector<Acc> work((size_t)nrows * ncols);
    కొరకు (పూర్ణాంకం i = 0; i < nrows; i++)
    {
        స్థిరరాశి T* s = src.ptr<T>(i);
        స్థిరరాశి uchar* a = allowed.ptr<uchar>(i);
        Acc* w = &work[(size_t)i * ncols];
        కొరకు (పూర్ణాంకం j = 0; j < nreal; j++)
            w[j] = a[j] ? (Acc)s[j] : forbidden;
        కొరకు (పూర్ణాంకం j = nreal; j < ncols; j++)
            w[j] = dummy;
    }

    std::vector<పూర్ణాంకం> నిలువు_యొక్క_అడ్డువరుస;
    solveJV<Acc>(work, nrows, ncols, నిలువు_యొక్క_అడ్డువరుస);

    ద్విగుణ మొత్తం = 0.0;
    కొరకు (పూర్ణాంకం i = 0; i < nrows; i++)
    {
        స్థిరరాశి పూర్ణాంకం j = నిలువు_యొక్క_అడ్డువరుస[i];
        CV_DbgAssert(j >= 0 && j < ncols);      // solveJV gives every row a column
        ఒకవేళ (j >= nreal || !allowed.at<uchar>(i, j))
            కొనసాగు;   // landed on a dummy column or a forbidden cell, so it stays unmatched

        మొత్తం += (ద్విగుణ)src.ptr<T>(i)[j];
        స్థిరరాశి పూర్ణాంకం అడ్డువరుస = transposed ? j : i;
        CV_DbgAssert(అడ్డువరుస >= 0 && (size_t)అడ్డువరుస < assignment.size());
        assignment[(size_t)అడ్డువరుస] = transposed ? i : j;
    }
    తిరిగి మొత్తం;
}

ద్విగుణ రేఖీయ_Assignment(ఆదానం_Array ఖర్చు, std::vector<పూర్ణాంకం>& assignment, ద్విగుణ ఖర్చు_పరిమితి)
{
    CV_INSTRUMENT_REGION();

    Mat ఖర్చు_2 = ఖర్చు.getMat();
    CV_Assert(ఖర్చు_2.dims <= 2);
    CV_Assert(!ఖర్చు_2.empty());

    assignment.assign((size_t)ఖర్చు_2.అడ్డువరుసలు, -1);

    CV_CheckType(ఖర్చు_2.type(),
                 ఖర్చు_2.type() == CV_32FC1 || ఖర్చు_2.type() == CV_64FC1 || ఖర్చు_2.type() == CV_32SC1,
                 "cost must be a single-channel CV_32F, CV_64F or CV_32S matrix");
    CV_Assert(!cvIsNaN(ఖర్చు_పరిమితి));

    // The solver assigns every row, so it needs rows <= cols. Transposing also makes A and its
    // transpose give the same answer, since min(M,N) is the same either way.
    స్థిరరాశి తార్కిక transposed = ఖర్చు_2.అడ్డువరుసలు > ఖర్చు_2.నిలువుల_సంఖ్య;
    Mat src;
    ఒకవేళ (transposed)
        cv::transpose(ఖర్చు_2, src);
    లేకపోతే
        src = ఖర్చు_2;

    ఎంపిక (ఖర్చు_2.లోతు())
    {
    స్థితి CV_32F: తిరిగి solveTyped<దశాంశం>(src, transposed, assignment, ఖర్చు_పరిమితి);
    స్థితి CV_64F: తిరిగి solveTyped<ద్విగుణ>(src, transposed, assignment, ఖర్చు_పరిమితి);
    స్థితి CV_32S: తిరిగి solveTyped<పూర్ణాంకం>(src, transposed, assignment, ఖర్చు_పరిమితి);
    అప్రమేయ:     CV_Error(Error::StsUnsupportedFormat, "unsupported cost matrix type");
    }
}

} // namespace cv
