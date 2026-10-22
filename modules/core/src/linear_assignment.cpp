// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
// Copyright (C) 2026, BigVision LLC, all rights reserved.
// Third party copyrights are property of their respective owners.

// Jonker-Volgenant rectangular assignment. See @cite Crouse2016 and @cite Jonker1987.

#شامل "precomp.hpp"

#شامل <algorithm>
#شامل <limits>

نامخلا cv {

// cvIsNaN/cvIsInf inspect the bit pattern, so unlike std::isfinite they still work under
// -ffast-math.
ساکن اندرونی منطقی isFiniteVal(دوہرا x)
{
    واپس !cvIsNaN(x) && !cvIsInf(x);
}

// Type the duals and the running sums are kept in. double suits all three inputs: it holds every
// int and float value exactly, so the integer path needs no arithmetic of its own.
سانچہ <قسم_نام T> ساخت Lap_مجمع { قسمتعریف دوہرا type; };

// A cell is usable only if it is a real cost within the threshold. Check finiteness first:
// NaN > costThreshold is false, so a bare comparison would let NaN through as allowed.
سانچہ <قسم_نام T>
ساکن اندرونی منطقی cellAllowed(T value, دوہرا لاگت_حد)
{
    ثابت دوہرا v = (دوہرا)value;
    واپس isFiniteVal(v) && v <= لاگت_حد;
}

// Assigns every row to its own column at the lowest total. `work` is nrows x ncols, rows <= cols,
// all finite. Each pass adds one row by a Dijkstra search over the reduced costs.
سانچہ <قسم_نام Acc>
ساکن خالی solveJV(ثابت std::vector<Acc>& work, صحیح nrows, صحیح ncols, std::vector<صحیح>& ستون_کا_صف)
{
    CV_DbgAssert(nrows <= ncols);

    std::vector<Acc> u((size_t)nrows, Acc(0));
    std::vector<Acc> v((size_t)ncols, Acc(0));
    std::vector<Acc> dist((size_t)ncols);
    std::vector<صحیح> صف_کا_ستون((size_t)ncols, -1);
    std::vector<صحیح> پچھلا_صف((size_t)ncols);
    std::vector<uchar> labelled((size_t)ncols);
    std::vector<صحیح> بقایا((size_t)ncols);   // columns not yet labelled, in no order

    ستون_کا_صف.assign((size_t)nrows, -1);

    برائے (صحیح free_صف = 0; free_صف < nrows; free_صف++)
    {
        std::fill(dist.begin(), dist.end(), std::numeric_limits<Acc>::max());
        std::fill(labelled.begin(), labelled.end(), (uchar)0);
        برائے (صحیح j = 0; j < ncols; j++)
            بقایا[j] = j;
        صحیح nremaining = ncols;

        صحیح صف = free_صف;      // row currently being expanded
        Acc delta = Acc(0);     // length of the shortest path found so far
        صحیح sink = -1;          // free column that ends the augmenting path

        جب (sink < 0)
        {
            ثابت Acc* صف_Ptr = &work[(size_t)صف * ncols];
            ثابت Acc urow = u[صف];
            // One pass over the unlabelled columns relaxes them and picks the nearest. A settled
            // column is swapped off the tail so it is never read again.
            صحیح بہترین = 0;
            Acc بہترین_Dist = std::numeric_limits<Acc>::max();
            برائے (صحیح k = 0; k < nremaining; k++)
            {
                ثابت صحیح j = بقایا[k];
                ثابت Acc cand = delta + صف_Ptr[j] - urow - v[j];
                اگر (cand < dist[j])
                {
                    dist[j] = cand;
                    پچھلا_صف[j] = صف;
                }
                اگر (dist[j] < بہترین_Dist)
                {
                    بہترین_Dist = dist[j];
                    بہترین = k;
                }
            }
            // ncols >= nrows leaves at least one column free, and every cost is finite, so the
            // search can always reach it.
            CV_Assert(بہترین_Dist < std::numeric_limits<Acc>::max());

            ثابت صحیح next = بقایا[بہترین];
            بقایا[بہترین] = بقایا[--nremaining];
            labelled[next] = 1;    // still needed by the dual update below
            delta = بہترین_Dist;

            اگر (صف_کا_ستون[next] < 0)
                sink = next;
            ورنہ
                صف = صف_کا_ستون[next];   // occupied, so keep going through its row
        }

        // Shift the duals so the path edges land at zero without disturbing the matched pairs.
        // Must run before the matching below, which it reads.
        u[free_صف] += delta;
        برائے (صحیح j = 0; j < ncols; j++)
        {
            اگر (labelled[j] && j != sink)
            {
                ثابت Acc سرکاؤ = delta - dist[j];
                v[j] -= سرکاؤ;
                u[صف_کا_ستون[j]] += سرکاؤ;
            }
        }

        // Walk the path back from the free column, flipping each edge.
        برائے (صحیح j = sink;;)
        {
            ثابت صحیح i = پچھلا_صف[j];
            صف_کا_ستون[j] = i;
            ثابت صحیح j_پچھلا = ستون_کا_صف[i];
            ستون_کا_صف[i] = j;
            اگر (i == free_صف)
                توڑو;
            j = j_پچھلا;
        }
    }
}

// Solves for one input type. `src` is read in its own type, so the padded working matrix is the
// only copy made and no promoted duplicate of the input is allocated.
سانچہ <قسم_نام T>
ساکن دوہرا solveTyped(ثابت Mat& src, منطقی transposed, std::vector<صحیح>& assignment,
                         دوہرا لاگت_حد)
{
    قسمتعریف قسم_نام Lap_مجمع<T>::type Acc;

    ثابت صحیح nrows = src.صفیں;
    ثابت صحیح nreal = src.ستون_تعداد;

    Mat allowed(nrows, nreal, CV_8U);
    Acc abs_مجموع = Acc(0);
    دوہرا max_لاگت = -std::numeric_limits<دوہرا>::max();
    منطقی anyForbidden = جھوٹ;
    برائے (صحیح i = 0; i < nrows; i++)
    {
        ثابت T* s = src.ptr<T>(i);
        uchar* a = allowed.ptr<uchar>(i);
        برائے (صحیح j = 0; j < nreal; j++)
        {
            ثابت منطقی ok = cellAllowed<T>(s[j], لاگت_حد);
            a[j] = ok ? 1 : 0;
            اگر (ok)
            {
                abs_مجموع += (Acc)std::مطلق_قدر((دوہرا)s[j]);
                max_لاگت = std::max(max_لاگت, (دوہرا)s[j]);
            }
            ورنہ
                anyForbidden = سچ;
        }
    }

    // Price of leaving a row unmatched, clamped so the DBL_MAX default stays finite.
    // The 2 is required: k -> k+1 augments a path, so the delta is 2*absSum, not one edge.
    Acc dummy = Acc(2) * abs_مجموع + Acc(1);
    اگر (لاگت_حد < (دوہرا)dummy)
        dummy = (Acc)لاگت_حد;
    CV_Assert(isFiniteVal((دوہرا)dummy));

    // A forbidden cell only has to lose to a dummy, and every row has one. The margin scales
    // because adding 1 to a very large dummy changes nothing.
    ثابت Acc forbidden = dummy + (Acc)std::max(1.0, std::مطلق_قدر((دوہرا)dummy) * 1e-12);
    CV_Assert(isFiniteVal((دوہرا)forbidden) && forbidden > dummy);

    // Drop the dummy columns when no row can go unmatched: nothing forbidden, every cost below the
    // no-pair price. Halves the columns on square input; a cost equal to that price is a tie.
    ثابت صحیح ndummy = (anyForbidden || !(max_لاگت < (دوہرا)dummy)) ? nrows : 0;
    ثابت صحیح ncols = nreal + ndummy;

    std::vector<Acc> work((size_t)nrows * ncols);
    برائے (صحیح i = 0; i < nrows; i++)
    {
        ثابت T* s = src.ptr<T>(i);
        ثابت uchar* a = allowed.ptr<uchar>(i);
        Acc* w = &work[(size_t)i * ncols];
        برائے (صحیح j = 0; j < nreal; j++)
            w[j] = a[j] ? (Acc)s[j] : forbidden;
        برائے (صحیح j = nreal; j < ncols; j++)
            w[j] = dummy;
    }

    std::vector<صحیح> ستون_کا_صف;
    solveJV<Acc>(work, nrows, ncols, ستون_کا_صف);

    دوہرا کل = 0.0;
    برائے (صحیح i = 0; i < nrows; i++)
    {
        ثابت صحیح j = ستون_کا_صف[i];
        CV_DbgAssert(j >= 0 && j < ncols);      // solveJV gives every row a column
        اگر (j >= nreal || !allowed.at<uchar>(i, j))
            جاری;   // landed on a dummy column or a forbidden cell, so it stays unmatched

        کل += (دوہرا)src.ptr<T>(i)[j];
        ثابت صحیح صف = transposed ? j : i;
        CV_DbgAssert(صف >= 0 && (size_t)صف < assignment.size());
        assignment[(size_t)صف] = transposed ? i : j;
    }
    واپس کل;
}

دوہرا خطی_Assignment(داخلہ_Array لاگت, std::vector<صحیح>& assignment, دوہرا لاگت_حد)
{
    CV_INSTRUMENT_REGION();

    Mat لاگت_2 = لاگت.getMat();
    CV_Assert(لاگت_2.dims <= 2);
    CV_Assert(!لاگت_2.empty());

    assignment.assign((size_t)لاگت_2.صفیں, -1);

    CV_CheckType(لاگت_2.type(),
                 لاگت_2.type() == CV_32FC1 || لاگت_2.type() == CV_64FC1 || لاگت_2.type() == CV_32SC1,
                 "cost must be a single-channel CV_32F, CV_64F or CV_32S matrix");
    CV_Assert(!cvIsNaN(لاگت_حد));

    // The solver assigns every row, so it needs rows <= cols. Transposing also makes A and its
    // transpose give the same answer, since min(M,N) is the same either way.
    ثابت منطقی transposed = لاگت_2.صفیں > لاگت_2.ستون_تعداد;
    Mat src;
    اگر (transposed)
        cv::transpose(لاگت_2, src);
    ورنہ
        src = لاگت_2;

    انتخاب (لاگت_2.گہرائی())
    {
    حالت CV_32F: واپس solveTyped<اعشاری>(src, transposed, assignment, لاگت_حد);
    حالت CV_64F: واپس solveTyped<دوہرا>(src, transposed, assignment, لاگت_حد);
    حالت CV_32S: واپس solveTyped<صحیح>(src, transposed, assignment, لاگت_حد);
    طےشدہ:     CV_Error(Error::StsUnsupportedFormat, "unsupported cost matrix type");
    }
}

} // namespace cv
