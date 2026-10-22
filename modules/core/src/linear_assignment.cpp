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
سانچا <قسم_ناں T> بنية Lap_سنچائک { تعريفنوع مزدوج type; };

// A cell is usable only if it is a real cost within the threshold. Check finiteness first:
// NaN > costThreshold is false, so a bare comparison would let NaN through as allowed.
سانچا <قسم_ناں T>
ساكن مضمن منطقي cellAllowed(T value, مزدوج لاگت_حد)
{
    ثابت مزدوج v = (مزدوج)value;
    أرجع isFiniteVal(v) && v <= لاگت_حد;
}

// Assigns every row to its own column at the lowest total. `work` is nrows x ncols, rows <= cols,
// all finite. Each pass adds one row by a Dijkstra search over the reduced costs.
سانچا <قسم_ناں Acc>
ساكن فراغ solveJV(ثابت std::vector<Acc>& work, صحيح nrows, صحيح ncols, std::vector<صحيح>& خانہ_دا_صف)
{
    CV_DbgAssert(nrows <= ncols);

    std::vector<Acc> u((size_t)nrows, Acc(0));
    std::vector<Acc> v((size_t)ncols, Acc(0));
    std::vector<Acc> dist((size_t)ncols);
    std::vector<صحيح> صف_دا_خانہ((size_t)ncols, -1);
    std::vector<صحيح> prev_صف((size_t)ncols);
    std::vector<uchar> labelled((size_t)ncols);
    std::vector<صحيح> بچیا((size_t)ncols);   // columns not yet labelled, in no order

    خانہ_دا_صف.assign((size_t)nrows, -1);

    لكل (صحيح free_صف = 0; free_صف < nrows; free_صف++)
    {
        std::fill(dist.begin(), dist.end(), std::numeric_limits<Acc>::max());
        std::fill(labelled.begin(), labelled.end(), (uchar)0);
        لكل (صحيح j = 0; j < ncols; j++)
            بچیا[j] = j;
        صحيح nremaining = ncols;

        صحيح صف = free_صف;      // row currently being expanded
        Acc delta = Acc(0);     // length of the shortest path found so far
        صحيح sink = -1;          // free column that ends the augmenting path

        طالما (sink < 0)
        {
            ثابت Acc* صف_Ptr = &work[(size_t)صف * ncols];
            ثابت Acc urow = u[صف];
            // One pass over the unlabelled columns relaxes them and picks the nearest. A settled
            // column is swapped off the tail so it is never read again.
            صحيح بہترین = 0;
            Acc بہترین_Dist = std::numeric_limits<Acc>::max();
            لكل (صحيح k = 0; k < nremaining; k++)
            {
                ثابت صحيح j = بچیا[k];
                ثابت Acc cand = delta + صف_Ptr[j] - urow - v[j];
                إذا (cand < dist[j])
                {
                    dist[j] = cand;
                    prev_صف[j] = صف;
                }
                إذا (dist[j] < بہترین_Dist)
                {
                    بہترین_Dist = dist[j];
                    بہترین = k;
                }
            }
            // ncols >= nrows leaves at least one column free, and every cost is finite, so the
            // search can always reach it.
            CV_Assert(بہترین_Dist < std::numeric_limits<Acc>::max());

            ثابت صحيح next = بچیا[بہترین];
            بچیا[بہترین] = بچیا[--nremaining];
            labelled[next] = 1;    // still needed by the dual update below
            delta = بہترین_Dist;

            إذا (صف_دا_خانہ[next] < 0)
                sink = next;
            وإلا
                صف = صف_دا_خانہ[next];   // occupied, so keep going through its row
        }

        // Shift the duals so the path edges land at zero without disturbing the matched pairs.
        // Must run before the matching below, which it reads.
        u[free_صف] += delta;
        لكل (صحيح j = 0; j < ncols; j++)
        {
            إذا (labelled[j] && j != sink)
            {
                ثابت Acc کھسکاؤ = delta - dist[j];
                v[j] -= کھسکاؤ;
                u[صف_دا_خانہ[j]] += کھسکاؤ;
            }
        }

        // Walk the path back from the free column, flipping each edge.
        لكل (صحيح j = sink;;)
        {
            ثابت صحيح i = prev_صف[j];
            صف_دا_خانہ[j] = i;
            ثابت صحيح jPrev = خانہ_دا_صف[i];
            خانہ_دا_صف[i] = j;
            إذا (i == free_صف)
                اكسر;
            j = jPrev;
        }
    }
}

// Solves for one input type. `src` is read in its own type, so the padded working matrix is the
// only copy made and no promoted duplicate of the input is allocated.
سانچا <قسم_ناں T>
ساكن مزدوج solveTyped(ثابت Mat& src, منطقي transposed, std::vector<صحيح>& assignment,
                         مزدوج لاگت_حد)
{
    تعريفنوع قسم_ناں Lap_سنچائک<T>::type Acc;

    ثابت صحيح nrows = src.صفاں;
    ثابت صحيح nreal = src.خانے_گنتی;

    Mat allowed(nrows, nreal, CV_8U);
    Acc abs_جوڑ = Acc(0);
    مزدوج max_لاگت = -std::numeric_limits<مزدوج>::max();
    منطقي anyForbidden = خطأ;
    لكل (صحيح i = 0; i < nrows; i++)
    {
        ثابت T* s = src.ptr<T>(i);
        uchar* a = allowed.ptr<uchar>(i);
        لكل (صحيح j = 0; j < nreal; j++)
        {
            ثابت منطقي ok = cellAllowed<T>(s[j], لاگت_حد);
            a[j] = ok ? 1 : 0;
            إذا (ok)
            {
                abs_جوڑ += (Acc)std::مطلق_قدر((مزدوج)s[j]);
                max_لاگت = std::max(max_لاگت, (مزدوج)s[j]);
            }
            وإلا
                anyForbidden = سچ;
        }
    }

    // Price of leaving a row unmatched, clamped so the DBL_MAX default stays finite.
    // The 2 is required: k -> k+1 augments a path, so the delta is 2*absSum, not one edge.
    Acc dummy = Acc(2) * abs_جوڑ + Acc(1);
    إذا (لاگت_حد < (مزدوج)dummy)
        dummy = (Acc)لاگت_حد;
    CV_Assert(isFiniteVal((مزدوج)dummy));

    // A forbidden cell only has to lose to a dummy, and every row has one. The margin scales
    // because adding 1 to a very large dummy changes nothing.
    ثابت Acc forbidden = dummy + (Acc)std::max(1.0, std::مطلق_قدر((مزدوج)dummy) * 1e-12);
    CV_Assert(isFiniteVal((مزدوج)forbidden) && forbidden > dummy);

    // Drop the dummy columns when no row can go unmatched: nothing forbidden, every cost below the
    // no-pair price. Halves the columns on square input; a cost equal to that price is a tie.
    ثابت صحيح ndummy = (anyForbidden || !(max_لاگت < (مزدوج)dummy)) ? nrows : 0;
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

    std::vector<صحيح> خانہ_دا_صف;
    solveJV<Acc>(work, nrows, ncols, خانہ_دا_صف);

    مزدوج کل = 0.0;
    لكل (صحيح i = 0; i < nrows; i++)
    {
        ثابت صحيح j = خانہ_دا_صف[i];
        CV_DbgAssert(j >= 0 && j < ncols);      // solveJV gives every row a column
        إذا (j >= nreal || !allowed.at<uchar>(i, j))
            استمر;   // landed on a dummy column or a forbidden cell, so it stays unmatched

        کل += (مزدوج)src.ptr<T>(i)[j];
        ثابت صحيح صف = transposed ? j : i;
        CV_DbgAssert(صف >= 0 && (size_t)صف < assignment.size());
        assignment[(size_t)صف] = transposed ? i : j;
    }
    أرجع کل;
}

مزدوج خطی_Assignment(داخلہ_Array لاگت, std::vector<صحيح>& assignment, مزدوج لاگت_حد)
{
    CV_INSTRUMENT_REGION();

    Mat لاگت_2 = لاگت.getMat();
    CV_Assert(لاگت_2.dims <= 2);
    CV_Assert(!لاگت_2.empty());

    assignment.assign((size_t)لاگت_2.صفاں, -1);

    CV_CheckType(لاگت_2.type(),
                 لاگت_2.type() == CV_32FC1 || لاگت_2.type() == CV_64FC1 || لاگت_2.type() == CV_32SC1,
                 "cost must be a single-channel CV_32F, CV_64F or CV_32S matrix");
    CV_Assert(!cvIsNaN(لاگت_حد));

    // The solver assigns every row, so it needs rows <= cols. Transposing also makes A and its
    // transpose give the same answer, since min(M,N) is the same either way.
    ثابت منطقي transposed = لاگت_2.صفاں > لاگت_2.خانے_گنتی;
    Mat src;
    إذا (transposed)
        cv::transpose(لاگت_2, src);
    وإلا
        src = لاگت_2;

    بدل (لاگت_2.ڈونگھائی())
    {
    حالة CV_32F: أرجع solveTyped<عائم>(src, transposed, assignment, لاگت_حد);
    حالة CV_64F: أرجع solveTyped<مزدوج>(src, transposed, assignment, لاگت_حد);
    حالة CV_32S: أرجع solveTyped<صحيح>(src, transposed, assignment, لاگت_حد);
    افتراضي:     CV_Error(Error::StsUnsupportedFormat, "unsupported cost matrix type");
    }
}

} // namespace cv
