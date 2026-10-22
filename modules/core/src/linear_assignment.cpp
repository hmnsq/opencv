// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
// Copyright (C) 2026, BigVision LLC, all rights reserved.
// Third party copyrights are property of their respective owners.

// Jonker-Volgenant rectangular assignment. See @cite Crouse2016 and @cite Jonker1987.

#ضمن "precomp.hpp"

#ضمن <algorithm>
#ضمن <limits>

نطاق cv {

// cvIsNaN/cvIsInf inspect the bit pattern, so unlike std::isfinite they still work under
// -ffast-math.
ساكن مضمن منطقي isFiniteVal(مزدوج x)
{
    أرجع !cvIsNaN(x) && !cvIsInf(x);
}

// Type the duals and the running sums are kept in. double suits all three inputs: it holds every
// int and float value exactly, so the integer path needs no arithmetic of its own.
قالب <نام_نوع T> بنية Lap_انباشتگر { تعريفنوع مزدوج type; };

// A cell is usable only if it is a real cost within the threshold. Check finiteness first:
// NaN > costThreshold is false, so a bare comparison would let NaN through as allowed.
قالب <نام_نوع T>
ساكن مضمن منطقي cellAllowed(T value, مزدوج هزینه_آستانه)
{
    ثابت مزدوج v = (مزدوج)value;
    أرجع isFiniteVal(v) && v <= هزینه_آستانه;
}

// Assigns every row to its own column at the lowest total. `work` is nrows x ncols, rows <= cols,
// all finite. Each pass adds one row by a Dijkstra search over the reduced costs.
قالب <نام_نوع Acc>
ساكن فراغ solveJV(ثابت std::vector<Acc>& work, صحيح nrows, صحيح ncols, std::vector<صحيح>& ستون_از_ردیف)
{
    CV_DbgAssert(nrows <= ncols);

    std::vector<Acc> u((size_t)nrows, Acc(0));
    std::vector<Acc> v((size_t)ncols, Acc(0));
    std::vector<Acc> dist((size_t)ncols);
    std::vector<صحيح> ردیف_از_ستون((size_t)ncols, -1);
    std::vector<صحيح> prev_ردیف((size_t)ncols);
    std::vector<uchar> labelled((size_t)ncols);
    std::vector<صحيح> مانده((size_t)ncols);   // columns not yet labelled, in no order

    ستون_از_ردیف.assign((size_t)nrows, -1);

    لكل (صحيح free_ردیف = 0; free_ردیف < nrows; free_ردیف++)
    {
        std::fill(dist.begin(), dist.end(), std::numeric_limits<Acc>::max());
        std::fill(labelled.begin(), labelled.end(), (uchar)0);
        لكل (صحيح j = 0; j < ncols; j++)
            مانده[j] = j;
        صحيح nremaining = ncols;

        صحيح ردیف = free_ردیف;      // row currently being expanded
        Acc delta = Acc(0);     // length of the shortest path found so far
        صحيح sink = -1;          // free column that ends the augmenting path

        طالما (sink < 0)
        {
            ثابت Acc* ردیف_Ptr = &work[(size_t)ردیف * ncols];
            ثابت Acc urow = u[ردیف];
            // One pass over the unlabelled columns relaxes them and picks the nearest. A settled
            // column is swapped off the tail so it is never read again.
            صحيح بهترین = 0;
            Acc بهترین_Dist = std::numeric_limits<Acc>::max();
            لكل (صحيح k = 0; k < nremaining; k++)
            {
                ثابت صحيح j = مانده[k];
                ثابت Acc cand = delta + ردیف_Ptr[j] - urow - v[j];
                إذا (cand < dist[j])
                {
                    dist[j] = cand;
                    prev_ردیف[j] = ردیف;
                }
                إذا (dist[j] < بهترین_Dist)
                {
                    بهترین_Dist = dist[j];
                    بهترین = k;
                }
            }
            // ncols >= nrows leaves at least one column free, and every cost is finite, so the
            // search can always reach it.
            CV_Assert(بهترین_Dist < std::numeric_limits<Acc>::max());

            ثابت صحيح next = مانده[بهترین];
            مانده[بهترین] = مانده[--nremaining];
            labelled[next] = 1;    // still needed by the dual update below
            delta = بهترین_Dist;

            إذا (ردیف_از_ستون[next] < 0)
                sink = next;
            وإلا
                ردیف = ردیف_از_ستون[next];   // occupied, so keep going through its row
        }

        // Shift the duals so the path edges land at zero without disturbing the matched pairs.
        // Must run before the matching below, which it reads.
        u[free_ردیف] += delta;
        لكل (صحيح j = 0; j < ncols; j++)
        {
            إذا (labelled[j] && j != sink)
            {
                ثابت Acc لغزش = delta - dist[j];
                v[j] -= لغزش;
                u[ردیف_از_ستون[j]] += لغزش;
            }
        }

        // Walk the path back from the free column, flipping each edge.
        لكل (صحيح j = sink;;)
        {
            ثابت صحيح i = prev_ردیف[j];
            ردیف_از_ستون[j] = i;
            ثابت صحيح jPrev = ستون_از_ردیف[i];
            ستون_از_ردیف[i] = j;
            إذا (i == free_ردیف)
                اكسر;
            j = jPrev;
        }
    }
}

// Solves for one input type. `src` is read in its own type, so the padded working matrix is the
// only copy made and no promoted duplicate of the input is allocated.
قالب <نام_نوع T>
ساكن مزدوج solveTyped(ثابت Mat& src, منطقي transposed, std::vector<صحيح>& assignment,
                         مزدوج هزینه_آستانه)
{
    تعريفنوع نام_نوع Lap_انباشتگر<T>::type Acc;

    ثابت صحيح nrows = src.ردیفها;
    ثابت صحيح nreal = src.تعداد_ستون;

    Mat allowed(nrows, nreal, CV_8U);
    Acc abs_جمع = Acc(0);
    مزدوج max_هزینه = -std::numeric_limits<مزدوج>::max();
    منطقي anyForbidden = خطأ;
    لكل (صحيح i = 0; i < nrows; i++)
    {
        ثابت T* s = src.ptr<T>(i);
        uchar* a = allowed.ptr<uchar>(i);
        لكل (صحيح j = 0; j < nreal; j++)
        {
            ثابت منطقي ok = cellAllowed<T>(s[j], هزینه_آستانه);
            a[j] = ok ? 1 : 0;
            إذا (ok)
            {
                abs_جمع += (Acc)std::قدر_مطلق((مزدوج)s[j]);
                max_هزینه = std::max(max_هزینه, (مزدوج)s[j]);
            }
            وإلا
                anyForbidden = درست;
        }
    }

    // Price of leaving a row unmatched, clamped so the DBL_MAX default stays finite.
    // The 2 is required: k -> k+1 augments a path, so the delta is 2*absSum, not one edge.
    Acc dummy = Acc(2) * abs_جمع + Acc(1);
    إذا (هزینه_آستانه < (مزدوج)dummy)
        dummy = (Acc)هزینه_آستانه;
    CV_Assert(isFiniteVal((مزدوج)dummy));

    // A forbidden cell only has to lose to a dummy, and every row has one. The margin scales
    // because adding 1 to a very large dummy changes nothing.
    ثابت Acc forbidden = dummy + (Acc)std::max(1.0, std::قدر_مطلق((مزدوج)dummy) * 1e-12);
    CV_Assert(isFiniteVal((مزدوج)forbidden) && forbidden > dummy);

    // Drop the dummy columns when no row can go unmatched: nothing forbidden, every cost below the
    // no-pair price. Halves the columns on square input; a cost equal to that price is a tie.
    ثابت صحيح ndummy = (anyForbidden || !(max_هزینه < (مزدوج)dummy)) ? nrows : 0;
    ثابت صحيح ncols = nreal + ndummy;

    std::vector<Acc> work((size_t)nrows * ncols);
    لكل (صحيح i = 0; i < nrows; i++)
    {
        ثابت T* s = src.ptr<T>(i);
        ثابت uchar* a = allowed.ptr<uchar>(i);
        Acc* w = &work[(size_t)i * ncols];
        لكل (صحيح j = 0; j < nreal; j++)
            w[j] = a[j] ? (Acc)s[j] : forbidden;
        لكل (صحيح j = nreal; j < ncols; j++)
            w[j] = dummy;
    }

    std::vector<صحيح> ستون_از_ردیف;
    solveJV<Acc>(work, nrows, ncols, ستون_از_ردیف);

    مزدوج مجموع = 0.0;
    لكل (صحيح i = 0; i < nrows; i++)
    {
        ثابت صحيح j = ستون_از_ردیف[i];
        CV_DbgAssert(j >= 0 && j < ncols);      // solveJV gives every row a column
        إذا (j >= nreal || !allowed.at<uchar>(i, j))
            استمر;   // landed on a dummy column or a forbidden cell, so it stays unmatched

        مجموع += (مزدوج)src.ptr<T>(i)[j];
        ثابت صحيح ردیف = transposed ? j : i;
        CV_DbgAssert(ردیف >= 0 && (size_t)ردیف < assignment.size());
        assignment[(size_t)ردیف] = transposed ? i : j;
    }
    أرجع مجموع;
}

مزدوج خطی_Assignment(درون_داد_Array هزینه, std::vector<صحيح>& assignment, مزدوج هزینه_آستانه)
{
    CV_INSTRUMENT_REGION();

    Mat هزینه_2 = هزینه.getMat();
    CV_Assert(هزینه_2.dims <= 2);
    CV_Assert(!هزینه_2.empty());

    assignment.assign((size_t)هزینه_2.ردیفها, -1);

    CV_CheckType(هزینه_2.type(),
                 هزینه_2.type() == CV_32FC1 || هزینه_2.type() == CV_64FC1 || هزینه_2.type() == CV_32SC1,
                 "cost must be a single-channel CV_32F, CV_64F or CV_32S matrix");
    CV_Assert(!cvIsNaN(هزینه_آستانه));

    // The solver assigns every row, so it needs rows <= cols. Transposing also makes A and its
    // transpose give the same answer, since min(M,N) is the same either way.
    ثابت منطقي transposed = هزینه_2.ردیفها > هزینه_2.تعداد_ستون;
    Mat src;
    إذا (transposed)
        cv::transpose(هزینه_2, src);
    وإلا
        src = هزینه_2;

    بدل (هزینه_2.عمق())
    {
    حالة CV_32F: أرجع solveTyped<عائم>(src, transposed, assignment, هزینه_آستانه);
    حالة CV_64F: أرجع solveTyped<مزدوج>(src, transposed, assignment, هزینه_آستانه);
    حالة CV_32S: أرجع solveTyped<صحيح>(src, transposed, assignment, هزینه_آستانه);
    افتراضي:     CV_Error(Error::StsUnsupportedFormat, "unsupported cost matrix type");
    }
}

} // namespace cv
