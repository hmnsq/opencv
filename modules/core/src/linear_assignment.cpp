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
کينډۍ <ډول_نوم T> بنية Lap_راټولونکی { تعريفنوع مزدوج type; };

// A cell is usable only if it is a real cost within the threshold. Check finiteness first:
// NaN > costThreshold is false, so a bare comparison would let NaN through as allowed.
کينډۍ <ډول_نوم T>
ساكن مضمن منطقي cellAllowed(T value, مزدوج لګښت_پوله)
{
    ثابت مزدوج v = (مزدوج)value;
    أرجع isFiniteVal(v) && v <= لګښت_پوله;
}

// Assigns every row to its own column at the lowest total. `work` is nrows x ncols, rows <= cols,
// all finite. Each pass adds one row by a Dijkstra search over the reduced costs.
کينډۍ <ډول_نوم Acc>
ساكن فراغ solveJV(ثابت std::vector<Acc>& work, صحيح nrows, صحيح ncols, std::vector<صحيح>& ستون_د_لیکه)
{
    CV_DbgAssert(nrows <= ncols);

    std::vector<Acc> u((size_t)nrows, Acc(0));
    std::vector<Acc> v((size_t)ncols, Acc(0));
    std::vector<Acc> dist((size_t)ncols);
    std::vector<صحيح> لیکه_د_ستون((size_t)ncols, -1);
    std::vector<صحيح> prev_لیکه((size_t)ncols);
    std::vector<uchar> labelled((size_t)ncols);
    std::vector<صحيح> پاتې_شوی((size_t)ncols);   // columns not yet labelled, in no order

    ستون_د_لیکه.assign((size_t)nrows, -1);

    لكل (صحيح free_لیکه = 0; free_لیکه < nrows; free_لیکه++)
    {
        std::fill(dist.begin(), dist.end(), std::numeric_limits<Acc>::max());
        std::fill(labelled.begin(), labelled.end(), (uchar)0);
        لكل (صحيح j = 0; j < ncols; j++)
            پاتې_شوی[j] = j;
        صحيح nremaining = ncols;

        صحيح لیکه = free_لیکه;      // row currently being expanded
        Acc delta = Acc(0);     // length of the shortest path found so far
        صحيح sink = -1;          // free column that ends the augmenting path

        طالما (sink < 0)
        {
            ثابت Acc* لیکه_Ptr = &work[(size_t)لیکه * ncols];
            ثابت Acc urow = u[لیکه];
            // One pass over the unlabelled columns relaxes them and picks the nearest. A settled
            // column is swapped off the tail so it is never read again.
            صحيح غوره = 0;
            Acc غوره_Dist = std::numeric_limits<Acc>::max();
            لكل (صحيح k = 0; k < nremaining; k++)
            {
                ثابت صحيح j = پاتې_شوی[k];
                ثابت Acc cand = delta + لیکه_Ptr[j] - urow - v[j];
                إذا (cand < dist[j])
                {
                    dist[j] = cand;
                    prev_لیکه[j] = لیکه;
                }
                إذا (dist[j] < غوره_Dist)
                {
                    غوره_Dist = dist[j];
                    غوره = k;
                }
            }
            // ncols >= nrows leaves at least one column free, and every cost is finite, so the
            // search can always reach it.
            CV_Assert(غوره_Dist < std::numeric_limits<Acc>::max());

            ثابت صحيح next = پاتې_شوی[غوره];
            پاتې_شوی[غوره] = پاتې_شوی[--nremaining];
            labelled[next] = 1;    // still needed by the dual update below
            delta = غوره_Dist;

            إذا (لیکه_د_ستون[next] < 0)
                sink = next;
            وإلا
                لیکه = لیکه_د_ستون[next];   // occupied, so keep going through its row
        }

        // Shift the duals so the path edges land at zero without disturbing the matched pairs.
        // Must run before the matching below, which it reads.
        u[free_لیکه] += delta;
        لكل (صحيح j = 0; j < ncols; j++)
        {
            إذا (labelled[j] && j != sink)
            {
                ثابت Acc ښويدنه = delta - dist[j];
                v[j] -= ښويدنه;
                u[لیکه_د_ستون[j]] += ښويدنه;
            }
        }

        // Walk the path back from the free column, flipping each edge.
        لكل (صحيح j = sink;;)
        {
            ثابت صحيح i = prev_لیکه[j];
            لیکه_د_ستون[j] = i;
            ثابت صحيح jPrev = ستون_د_لیکه[i];
            ستون_د_لیکه[i] = j;
            إذا (i == free_لیکه)
                اكسر;
            j = jPrev;
        }
    }
}

// Solves for one input type. `src` is read in its own type, so the padded working matrix is the
// only copy made and no promoted duplicate of the input is allocated.
کينډۍ <ډول_نوم T>
ساكن مزدوج solveTyped(ثابت Mat& src, منطقي transposed, std::vector<صحيح>& assignment,
                         مزدوج لګښت_پوله)
{
    تعريفنوع ډول_نوم Lap_راټولونکی<T>::type Acc;

    ثابت صحيح nrows = src.لیکې;
    ثابت صحيح nreal = src.ستون_شمېر;

    Mat allowed(nrows, nreal, CV_8U);
    Acc abs_مجموعه = Acc(0);
    مزدوج max_لګښت = -std::numeric_limits<مزدوج>::max();
    منطقي anyForbidden = خطأ;
    لكل (صحيح i = 0; i < nrows; i++)
    {
        ثابت T* s = src.ptr<T>(i);
        uchar* a = allowed.ptr<uchar>(i);
        لكل (صحيح j = 0; j < nreal; j++)
        {
            ثابت منطقي ok = cellAllowed<T>(s[j], لګښت_پوله);
            a[j] = ok ? 1 : 0;
            إذا (ok)
            {
                abs_مجموعه += (Acc)std::مطلق_قیمت((مزدوج)s[j]);
                max_لګښت = std::max(max_لګښت, (مزدوج)s[j]);
            }
            وإلا
                anyForbidden = ریښتیا;
        }
    }

    // Price of leaving a row unmatched, clamped so the DBL_MAX default stays finite.
    // The 2 is required: k -> k+1 augments a path, so the delta is 2*absSum, not one edge.
    Acc dummy = Acc(2) * abs_مجموعه + Acc(1);
    إذا (لګښت_پوله < (مزدوج)dummy)
        dummy = (Acc)لګښت_پوله;
    CV_Assert(isFiniteVal((مزدوج)dummy));

    // A forbidden cell only has to lose to a dummy, and every row has one. The margin scales
    // because adding 1 to a very large dummy changes nothing.
    ثابت Acc forbidden = dummy + (Acc)std::max(1.0, std::مطلق_قیمت((مزدوج)dummy) * 1e-12);
    CV_Assert(isFiniteVal((مزدوج)forbidden) && forbidden > dummy);

    // Drop the dummy columns when no row can go unmatched: nothing forbidden, every cost below the
    // no-pair price. Halves the columns on square input; a cost equal to that price is a tie.
    ثابت صحيح ndummy = (anyForbidden || !(max_لګښت < (مزدوج)dummy)) ? nrows : 0;
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

    std::vector<صحيح> ستون_د_لیکه;
    solveJV<Acc>(work, nrows, ncols, ستون_د_لیکه);

    مزدوج ټول = 0.0;
    لكل (صحيح i = 0; i < nrows; i++)
    {
        ثابت صحيح j = ستون_د_لیکه[i];
        CV_DbgAssert(j >= 0 && j < ncols);      // solveJV gives every row a column
        إذا (j >= nreal || !allowed.at<uchar>(i, j))
            استمر;   // landed on a dummy column or a forbidden cell, so it stays unmatched

        ټول += (مزدوج)src.ptr<T>(i)[j];
        ثابت صحيح لیکه = transposed ? j : i;
        CV_DbgAssert(لیکه >= 0 && (size_t)لیکه < assignment.size());
        assignment[(size_t)لیکه] = transposed ? i : j;
    }
    أرجع ټول;
}

مزدوج خطي_Assignment(ننوت_Array لګښت, std::vector<صحيح>& assignment, مزدوج لګښت_پوله)
{
    CV_INSTRUMENT_REGION();

    Mat لګښت_2 = لګښت.getMat();
    CV_Assert(لګښت_2.dims <= 2);
    CV_Assert(!لګښت_2.empty());

    assignment.assign((size_t)لګښت_2.لیکې, -1);

    CV_CheckType(لګښت_2.type(),
                 لګښت_2.type() == CV_32FC1 || لګښت_2.type() == CV_64FC1 || لګښت_2.type() == CV_32SC1,
                 "cost must be a single-channel CV_32F, CV_64F or CV_32S matrix");
    CV_Assert(!cvIsNaN(لګښت_پوله));

    // The solver assigns every row, so it needs rows <= cols. Transposing also makes A and its
    // transpose give the same answer, since min(M,N) is the same either way.
    ثابت منطقي transposed = لګښت_2.لیکې > لګښت_2.ستون_شمېر;
    Mat src;
    إذا (transposed)
        cv::transpose(لګښت_2, src);
    وإلا
        src = لګښت_2;

    بدل (لګښت_2.ژوروالی())
    {
    حالة CV_32F: أرجع solveTyped<عائم>(src, transposed, assignment, لګښت_پوله);
    حالة CV_64F: أرجع solveTyped<مزدوج>(src, transposed, assignment, لګښت_پوله);
    حالة CV_32S: أرجع solveTyped<صحيح>(src, transposed, assignment, لګښت_پوله);
    افتراضي:     CV_Error(Error::StsUnsupportedFormat, "unsupported cost matrix type");
    }
}

} // namespace cv
