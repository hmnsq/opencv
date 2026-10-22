// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


/* ////////////////////////////////////////////////////////////////////
//
//  Mat basic operations: Copy, Set
//
// */

#অন্তর্ভাব "precomp.hpp"
#অন্তর্ভাব <opencv2/core/utils/logger.hpp>

নামাকাশ cv
{

//This and its overload below are used in various MatExpr operator overloads
//implemented to check that Matrix operands exist.
স্থির শূন্য checkOperandsExist(ধ্রুব Mat& a)
{
    যদি (a.empty())
    {
        CV_Error(cv::Error::StsBadArg, "Matrix operand is an empty matrix.");
    }
}

স্থির শূন্য checkOperandsExist(ধ্রুব Mat& a, ধ্রুব Mat& b)
{
    যদি (a.empty() || b.empty())
    {
        CV_Error(cv::Error::StsBadArg, "One or more matrix operands are empty.");
    }
}


বর্গ MatOp_Identity CV_FINAL : সার্বজনিক MatOp
{
সার্বজনিক:
    MatOp_Identity() {}
    আভাস ~MatOp_Identity() {}

    তার্কিক elementWise(ধ্রুব Mat_রাশিমালা& /*রাশিমালা*/) ধ্রুব CV_OVERRIDE { প্রত্যাগম সত্য; }
    শূন্য assign(ধ্রুব Mat_রাশিমালা& রাশিমালা, Mat& m, পূর্ণাঙ্ক type=-1) ধ্রুব CV_OVERRIDE;

    স্থির শূন্য make_রাশিমালা(Mat_রাশিমালা& res, ধ্রুব Mat& m);
};

স্থির MatOp_Identity g_MatOp_Identity;

বর্গ MatOp_AddEx CV_FINAL : সার্বজনিক MatOp
{
সার্বজনিক:
    MatOp_AddEx() {}
    আভাস ~MatOp_AddEx() {}

    তার্কিক elementWise(ধ্রুব Mat_রাশিমালা& /*রাশিমালা*/) ধ্রুব CV_OVERRIDE { প্রত্যাগম সত্য; }
    শূন্য assign(ধ্রুব Mat_রাশিমালা& রাশিমালা, Mat& m, পূর্ণাঙ্ক type=-1) ধ্রুব CV_OVERRIDE;

    শূন্য add(ধ্রুব Mat_রাশিমালা& e1, ধ্রুব Scalar& s, Mat_রাশিমালা& res) ধ্রুব CV_OVERRIDE;
    শূন্য subtract(ধ্রুব Scalar& s, ধ্রুব Mat_রাশিমালা& রাশিমালা, Mat_রাশিমালা& res) ধ্রুব CV_OVERRIDE;
    শূন্য multiply(ধ্রুব Mat_রাশিমালা& e1, দ্বিগুণ s, Mat_রাশিমালা& res) ধ্রুব CV_OVERRIDE;
    শূন্য divide(দ্বিগুণ s, ধ্রুব Mat_রাশিমালা& e, Mat_রাশিমালা& res) ধ্রুব CV_OVERRIDE;

    শূন্য transpose(ধ্রুব Mat_রাশিমালা& e1, Mat_রাশিমালা& res) ধ্রুব CV_OVERRIDE;
    শূন্য abs(ধ্রুব Mat_রাশিমালা& রাশিমালা, Mat_রাশিমালা& res) ধ্রুব CV_OVERRIDE;

    স্থির শূন্য make_রাশিমালা(Mat_রাশিমালা& res, ধ্রুব Mat& a, ধ্রুব Mat& b, দ্বিগুণ alpha, দ্বিগুণ beta, ধ্রুব Scalar& s=Scalar());
};

স্থির MatOp_AddEx g_MatOp_AddEx;

বর্গ MatOp_Bin CV_FINAL : সার্বজনিক MatOp
{
সার্বজনিক:
    MatOp_Bin() {}
    আভাস ~MatOp_Bin() {}

    তার্কিক elementWise(ধ্রুব Mat_রাশিমালা& /*রাশিমালা*/) ধ্রুব CV_OVERRIDE { প্রত্যাগম সত্য; }
    শূন্য assign(ধ্রুব Mat_রাশিমালা& রাশিমালা, Mat& m, পূর্ণাঙ্ক type=-1) ধ্রুব CV_OVERRIDE;

    শূন্য multiply(ধ্রুব Mat_রাশিমালা& e1, দ্বিগুণ s, Mat_রাশিমালা& res) ধ্রুব CV_OVERRIDE;
    শূন্য divide(দ্বিগুণ s, ধ্রুব Mat_রাশিমালা& e, Mat_রাশিমালা& res) ধ্রুব CV_OVERRIDE;

    স্থির শূন্য make_রাশিমালা(Mat_রাশিমালা& res, বর্ণ op, ধ্রুব Mat& a, ধ্রুব Mat& b, দ্বিগুণ scale=1);
    স্থির শূন্য make_রাশিমালা(Mat_রাশিমালা& res, বর্ণ op, ধ্রুব Mat& a, ধ্রুব Scalar& s);
};

স্থির MatOp_Bin g_MatOp_Bin;

বর্গ MatOp_Cmp CV_FINAL : সার্বজনিক MatOp
{
সার্বজনিক:
    MatOp_Cmp() {}
    আভাস ~MatOp_Cmp() {}

    তার্কিক elementWise(ধ্রুব Mat_রাশিমালা& /*রাশিমালা*/) ধ্রুব CV_OVERRIDE { প্রত্যাগম সত্য; }
    শূন্য assign(ধ্রুব Mat_রাশিমালা& রাশিমালা, Mat& m, পূর্ণাঙ্ক type=-1) ধ্রুব CV_OVERRIDE;

    স্থির শূন্য make_রাশিমালা(Mat_রাশিমালা& res, পূর্ণাঙ্ক cmpop, ধ্রুব Mat& a, ধ্রুব Mat& b);
    স্থির শূন্য make_রাশিমালা(Mat_রাশিমালা& res, পূর্ণাঙ্ক cmpop, ধ্রুব Mat& a, দ্বিগুণ alpha);
};

স্থির MatOp_Cmp g_MatOp_Cmp;

বর্গ MatOp_GEMM CV_FINAL : সার্বজনিক MatOp
{
সার্বজনিক:
    MatOp_GEMM() {}
    আভাস ~MatOp_GEMM() {}

    তার্কিক elementWise(ধ্রুব Mat_রাশিমালা& /*রাশিমালা*/) ধ্রুব CV_OVERRIDE { প্রত্যাগম অসত্য; }
    শূন্য assign(ধ্রুব Mat_রাশিমালা& রাশিমালা, Mat& m, পূর্ণাঙ্ক type=-1) ধ্রুব CV_OVERRIDE;

    শূন্য add(ধ্রুব Mat_রাশিমালা& e1, ধ্রুব Mat_রাশিমালা& e2, Mat_রাশিমালা& res) ধ্রুব CV_OVERRIDE;
    শূন্য subtract(ধ্রুব Mat_রাশিমালা& e1, ধ্রুব Mat_রাশিমালা& e2, Mat_রাশিমালা& res) ধ্রুব CV_OVERRIDE;
    শূন্য multiply(ধ্রুব Mat_রাশিমালা& e, দ্বিগুণ s, Mat_রাশিমালা& res) ধ্রুব CV_OVERRIDE;

    শূন্য transpose(ধ্রুব Mat_রাশিমালা& রাশিমালা, Mat_রাশিমালা& res) ধ্রুব CV_OVERRIDE;

    Size size(ধ্রুব Mat_রাশিমালা& রাশিমালা) ধ্রুব CV_OVERRIDE
    {
        প্রত্যাগম Size(
            (রাশিমালা.flags & GEMM_2_T) ? রাশিমালা.b.rows : রাশিমালা.b.cols,
            (রাশিমালা.flags & GEMM_1_T) ? রাশিমালা.a.cols : রাশিমালা.a.rows
        );
    }

    স্থির শূন্য make_রাশিমালা(Mat_রাশিমালা& res, পূর্ণাঙ্ক flags, ধ্রুব Mat& a, ধ্রুব Mat& b,
                         দ্বিগুণ alpha=1, ধ্রুব Mat& c=Mat(), দ্বিগুণ beta=1);

};

স্থির MatOp_GEMM g_MatOp_GEMM;

বর্গ MatOp_Invert CV_FINAL : সার্বজনিক MatOp
{
সার্বজনিক:
    MatOp_Invert() {}
    আভাস ~MatOp_Invert() {}

    তার্কিক elementWise(ধ্রুব Mat_রাশিমালা& /*রাশিমালা*/) ধ্রুব CV_OVERRIDE { প্রত্যাগম অসত্য; }
    শূন্য assign(ধ্রুব Mat_রাশিমালা& রাশিমালা, Mat& m, পূর্ণাঙ্ক type=-1) ধ্রুব CV_OVERRIDE;

    শূন্য matmul(ধ্রুব Mat_রাশিমালা& expr1, ধ্রুব Mat_রাশিমালা& expr2, Mat_রাশিমালা& res) ধ্রুব CV_OVERRIDE;

    স্থির শূন্য make_রাশিমালা(Mat_রাশিমালা& res, পূর্ণাঙ্ক method, ধ্রুব Mat& m);
};

স্থির MatOp_Invert g_MatOp_Invert;

বর্গ MatOp_T CV_FINAL : সার্বজনিক MatOp
{
সার্বজনিক:
    MatOp_T() {}
    আভাস ~MatOp_T() {}

    তার্কিক elementWise(ধ্রুব Mat_রাশিমালা& /*রাশিমালা*/) ধ্রুব CV_OVERRIDE { প্রত্যাগম অসত্য; }
    শূন্য assign(ধ্রুব Mat_রাশিমালা& রাশিমালা, Mat& m, পূর্ণাঙ্ক type=-1) ধ্রুব CV_OVERRIDE;

    শূন্য multiply(ধ্রুব Mat_রাশিমালা& e1, দ্বিগুণ s, Mat_রাশিমালা& res) ধ্রুব CV_OVERRIDE;
    শূন্য transpose(ধ্রুব Mat_রাশিমালা& রাশিমালা, Mat_রাশিমালা& res) ধ্রুব CV_OVERRIDE;

    স্থির শূন্য make_রাশিমালা(Mat_রাশিমালা& res, ধ্রুব Mat& a, দ্বিগুণ alpha=1);
};

স্থির MatOp_T g_MatOp_T;

বর্গ MatOp_Solve CV_FINAL : সার্বজনিক MatOp
{
সার্বজনিক:
    MatOp_Solve() {}
    আভাস ~MatOp_Solve() {}

    তার্কিক elementWise(ধ্রুব Mat_রাশিমালা& /*রাশিমালা*/) ধ্রুব CV_OVERRIDE { প্রত্যাগম অসত্য; }
    শূন্য assign(ধ্রুব Mat_রাশিমালা& রাশিমালা, Mat& m, পূর্ণাঙ্ক type=-1) ধ্রুব CV_OVERRIDE;

    স্থির শূন্য make_রাশিমালা(Mat_রাশিমালা& res, পূর্ণাঙ্ক method, ধ্রুব Mat& a, ধ্রুব Mat& b);
};

স্থির MatOp_Solve g_MatOp_Solve;

বর্গ MatOp_Initializer CV_FINAL : সার্বজনিক MatOp
{
সার্বজনিক:
    MatOp_Initializer() {}
    আভাস ~MatOp_Initializer() {}

    তার্কিক elementWise(ধ্রুব Mat_রাশিমালা& /*রাশিমালা*/) ধ্রুব CV_OVERRIDE { প্রত্যাগম অসত্য; }
    শূন্য assign(ধ্রুব Mat_রাশিমালা& রাশিমালা, Mat& m, পূর্ণাঙ্ক type=-1) ধ্রুব CV_OVERRIDE;

    শূন্য multiply(ধ্রুব Mat_রাশিমালা& e, দ্বিগুণ s, Mat_রাশিমালা& res) ধ্রুব CV_OVERRIDE;

    স্থির শূন্য make_রাশিমালা(Mat_রাশিমালা& res, পূর্ণাঙ্ক method, Size sz, পূর্ণাঙ্ক type, দ্বিগুণ alpha=1);
    স্থির শূন্য make_রাশিমালা(Mat_রাশিমালা& res, পূর্ণাঙ্ক method, পূর্ণাঙ্ক ndims, ধ্রুব পূর্ণাঙ্ক* sizes, পূর্ণাঙ্ক type, দ্বিগুণ alpha=1);
};

স্থির MatOp_Initializer* getGlobalMatOpInitializer()
{
    CV_SINGLETON_LAZY_INIT(MatOp_Initializer, নূতন MatOp_Initializer())
}

স্থির অন্তঃস্থ তার্কিক isIdentity(ধ্রুব Mat_রাশিমালা& e) { প্রত্যাগম e.op == &g_MatOp_Identity; }
স্থির অন্তঃস্থ তার্কিক isAddEx(ধ্রুব Mat_রাশিমালা& e) { প্রত্যাগম e.op == &g_MatOp_AddEx; }
স্থির অন্তঃস্থ তার্কিক isScaled(ধ্রুব Mat_রাশিমালা& e) { প্রত্যাগম isAddEx(e) && (!e.b.data || e.beta == 0) && e.s == Scalar(); }
স্থির অন্তঃস্থ তার্কিক isBin(ধ্রুব Mat_রাশিমালা& e, বর্ণ c) { প্রত্যাগম e.op == &g_MatOp_Bin && e.flags == c; }
স্থির অন্তঃস্থ তার্কিক isCmp(ধ্রুব Mat_রাশিমালা& e) { প্রত্যাগম e.op == &g_MatOp_Cmp; }
স্থির অন্তঃস্থ তার্কিক isReciprocal(ধ্রুব Mat_রাশিমালা& e) { প্রত্যাগম isBin(e,'/') && (!e.b.data || e.beta == 0); }
স্থির অন্তঃস্থ তার্কিক isT(ধ্রুব Mat_রাশিমালা& e) { প্রত্যাগম e.op == &g_MatOp_T; }
স্থির অন্তঃস্থ তার্কিক isInv(ধ্রুব Mat_রাশিমালা& e) { প্রত্যাগম e.op == &g_MatOp_Invert; }
স্থির অন্তঃস্থ তার্কিক isSolve(ধ্রুব Mat_রাশিমালা& e) { প্রত্যাগম e.op == &g_MatOp_Solve; }
//static inline bool isGEMM(const MatExpr& e) { return e.op == &g_MatOp_GEMM; }
স্থির অন্তঃস্থ তার্কিক isMatProd(ধ্রুব Mat_রাশিমালা& e) { প্রত্যাগম e.op == &g_MatOp_GEMM && (!e.c.data || e.beta == 0); }
স্থির অন্তঃস্থ তার্কিক isInitializer(ধ্রুব Mat_রাশিমালা& e) { প্রত্যাগম e.op == getGlobalMatOpInitializer(); }

/////////////////////////////////////////////////////////////////////////////////////////////////////

MatOp::MatOp() {}
MatOp::~MatOp() {}


তার্কিক MatOp::elementWise(ধ্রুব Mat_রাশিমালা& /*রাশিমালা*/) ধ্রুব
{
    প্রত্যাগম অসত্য;
}

শূন্য MatOp::roi(ধ্রুব Mat_রাশিমালা& রাশিমালা, ধ্রুব Range& rowRange, ধ্রুব Range& colRange, Mat_রাশিমালা& e) ধ্রুব
{
    যদি( elementWise(রাশিমালা) )
    {
        e = Mat_রাশিমালা(রাশিমালা.op, রাশিমালা.flags, Mat(), Mat(), Mat(),
                    রাশিমালা.alpha, রাশিমালা.beta, রাশিমালা.s);
        যদি(রাশিমালা.a.data)
            e.a = রাশিমালা.a(rowRange, colRange);
        যদি(রাশিমালা.b.data)
            e.b = রাশিমালা.b(rowRange, colRange);
        যদি(রাশিমালা.c.data)
            e.c = রাশিমালা.c(rowRange, colRange);
    }
    অন্যথা
    {
        Mat m;
        রাশিমালা.op->assign(রাশিমালা, m);
        e = Mat_রাশিমালা(&g_MatOp_Identity, 0, m(rowRange, colRange), Mat(), Mat());
    }
}

শূন্য MatOp::diag(ধ্রুব Mat_রাশিমালা& রাশিমালা, পূর্ণাঙ্ক d, Mat_রাশিমালা& e) ধ্রুব
{
    যদি( elementWise(রাশিমালা) )
    {
        e = Mat_রাশিমালা(রাশিমালা.op, রাশিমালা.flags, Mat(), Mat(), Mat(),
                    রাশিমালা.alpha, রাশিমালা.beta, রাশিমালা.s);
        যদি(রাশিমালা.a.data)
            e.a = রাশিমালা.a.diag(d);
        যদি(রাশিমালা.b.data)
            e.b = রাশিমালা.b.diag(d);
        যদি(রাশিমালা.c.data)
            e.c = রাশিমালা.c.diag(d);
    }
    অন্যথা
    {
        Mat m;
        রাশিমালা.op->assign(রাশিমালা, m);
        e = Mat_রাশিমালা(&g_MatOp_Identity, 0, m.diag(d), Mat(), Mat());
    }
}


শূন্য MatOp::augAssignAdd(ধ্রুব Mat_রাশিমালা& রাশিমালা, Mat& m) ধ্রুব
{
    Mat temp;
    রাশিমালা.op->assign(রাশিমালা, temp);
    m += temp;
}


শূন্য MatOp::augAssignSubtract(ধ্রুব Mat_রাশিমালা& রাশিমালা, Mat& m) ধ্রুব
{
    Mat temp;
    রাশিমালা.op->assign(রাশিমালা, temp);
    m -= temp;
}


শূন্য MatOp::augAssignMultiply(ধ্রুব Mat_রাশিমালা& রাশিমালা, Mat& m) ধ্রুব
{
    Mat temp;
    রাশিমালা.op->assign(রাশিমালা, temp);
    m *= temp;
}


শূন্য MatOp::augAssignDivide(ধ্রুব Mat_রাশিমালা& রাশিমালা, Mat& m) ধ্রুব
{
    Mat temp;
    রাশিমালা.op->assign(রাশিমালা, temp);
    m /= temp;
}


শূন্য MatOp::augAssignAnd(ধ্রুব Mat_রাশিমালা& রাশিমালা, Mat& m) ধ্রুব
{
    Mat temp;
    রাশিমালা.op->assign(রাশিমালা, temp);
    m &= temp;
}


শূন্য MatOp::augAssignOr(ধ্রুব Mat_রাশিমালা& রাশিমালা, Mat& m) ধ্রুব
{
    Mat temp;
    রাশিমালা.op->assign(রাশিমালা, temp);
    m |= temp;
}


শূন্য MatOp::augAssignXor(ধ্রুব Mat_রাশিমালা& রাশিমালা, Mat& m) ধ্রুব
{
    Mat temp;
    রাশিমালা.op->assign(রাশিমালা, temp);
    m ^= temp;
}


শূন্য MatOp::add(ধ্রুব Mat_রাশিমালা& e1, ধ্রুব Mat_রাশিমালা& e2, Mat_রাশিমালা& res) ধ্রুব
{
    CV_INSTRUMENT_REGION();

    যদি( এতত্ == e2.op )
    {
        দ্বিগুণ alpha = 1, beta = 1;
        Scalar s;
        Mat m1, m2;
        যদি( isAddEx(e1) && (!e1.b.data || e1.beta == 0) )
        {
            m1 = e1.a;
            alpha = e1.alpha;
            s = e1.s;
        }
        অন্যথা
            e1.op->assign(e1, m1);

        যদি( isAddEx(e2) && (!e2.b.data || e2.beta == 0) )
        {
            m2 = e2.a;
            beta = e2.alpha;
            s += e2.s;
        }
        অন্যথা
            e2.op->assign(e2, m2);
        MatOp_AddEx::make_রাশিমালা(res, m1, m2, alpha, beta, s);
    }
    অন্যথা
        e2.op->add(e1, e2, res);
}


শূন্য MatOp::add(ধ্রুব Mat_রাশিমালা& expr1, ধ্রুব Scalar& s, Mat_রাশিমালা& res) ধ্রুব
{
    CV_INSTRUMENT_REGION();

    Mat m1;
    expr1.op->assign(expr1, m1);
    MatOp_AddEx::make_রাশিমালা(res, m1, Mat(), 1, 0, s);
}


শূন্য MatOp::subtract(ধ্রুব Mat_রাশিমালা& e1, ধ্রুব Mat_রাশিমালা& e2, Mat_রাশিমালা& res) ধ্রুব
{
    CV_INSTRUMENT_REGION();

    যদি( এতত্ == e2.op )
    {
        দ্বিগুণ alpha = 1, beta = -1;
        Scalar s;
        Mat m1, m2;
        যদি( isAddEx(e1) && (!e1.b.data || e1.beta == 0) )
        {
            m1 = e1.a;
            alpha = e1.alpha;
            s = e1.s;
        }
        অন্যথা
            e1.op->assign(e1, m1);

        যদি( isAddEx(e2) && (!e2.b.data || e2.beta == 0) )
        {
            m2 = e2.a;
            beta = -e2.alpha;
            s -= e2.s;
        }
        অন্যথা
            e2.op->assign(e2, m2);
        MatOp_AddEx::make_রাশিমালা(res, m1, m2, alpha, beta, s);
    }
    অন্যথা
        e2.op->subtract(e1, e2, res);
}


শূন্য MatOp::subtract(ধ্রুব Scalar& s, ধ্রুব Mat_রাশিমালা& রাশিমালা, Mat_রাশিমালা& res) ধ্রুব
{
    CV_INSTRUMENT_REGION();

    Mat m;
    রাশিমালা.op->assign(রাশিমালা, m);
    MatOp_AddEx::make_রাশিমালা(res, m, Mat(), -1, 0, s);
}


শূন্য MatOp::multiply(ধ্রুব Mat_রাশিমালা& e1, ধ্রুব Mat_রাশিমালা& e2, Mat_রাশিমালা& res, দ্বিগুণ scale) ধ্রুব
{
    CV_INSTRUMENT_REGION();

    যদি( এতত্ == e2.op )
    {
        Mat m1, m2;

        যদি( isReciprocal(e1) )
        {
            যদি( isScaled(e2) )
            {
                scale *= e2.alpha;
                m2 = e2.a;
            }
            অন্যথা
                e2.op->assign(e2, m2);

            MatOp_Bin::make_রাশিমালা(res, '/', m2, e1.a, scale/e1.alpha);
        }
        অন্যথা
        {
            বর্ণ op = '*';
            যদি( isScaled(e1) )
            {
                m1 = e1.a;
                scale *= e1.alpha;
            }
            অন্যথা
                e1.op->assign(e1, m1);

            যদি( isScaled(e2) )
            {
                m2 = e2.a;
                scale *= e2.alpha;
            }
            অন্যথা যদি( isReciprocal(e2) )
            {
                op = '/';
                m2 = e2.a;
                scale *= e2.alpha;
            }
            অন্যথা
                e2.op->assign(e2, m2);

            MatOp_Bin::make_রাশিমালা(res, op, m1, m2, scale);
        }
    }
    অন্যথা
        e2.op->multiply(e1, e2, res, scale);
}


শূন্য MatOp::multiply(ধ্রুব Mat_রাশিমালা& রাশিমালা, দ্বিগুণ s, Mat_রাশিমালা& res) ধ্রুব
{
    CV_INSTRUMENT_REGION();

    Mat m;
    রাশিমালা.op->assign(রাশিমালা, m);
    MatOp_AddEx::make_রাশিমালা(res, m, Mat(), s, 0);
}


শূন্য MatOp::divide(ধ্রুব Mat_রাশিমালা& e1, ধ্রুব Mat_রাশিমালা& e2, Mat_রাশিমালা& res, দ্বিগুণ scale) ধ্রুব
{
    CV_INSTRUMENT_REGION();

    যদি( এতত্ == e2.op )
    {
        যদি( isReciprocal(e1) && isReciprocal(e2) )
            MatOp_Bin::make_রাশিমালা(res, '/', e2.a, e1.a, e1.alpha/e2.alpha);
        অন্যথা
        {
            Mat m1, m2;
            বর্ণ op = '/';

            যদি( isScaled(e1) )
            {
                m1 = e1.a;
                scale *= e1.alpha;
            }
            অন্যথা
                e1.op->assign(e1, m1);

            যদি( isScaled(e2) )
            {
                m2 = e2.a;
                scale /= e2.alpha;
            }
            অন্যথা যদি( isReciprocal(e2) )
            {
                m2 = e2.a;
                scale /= e2.alpha;
                op = '*';
            }
            অন্যথা
                e2.op->assign(e2, m2);
            MatOp_Bin::make_রাশিমালা(res, op, m1, m2, scale);
        }
    }
    অন্যথা
        e2.op->divide(e1, e2, res, scale);
}


শূন্য MatOp::divide(দ্বিগুণ s, ধ্রুব Mat_রাশিমালা& রাশিমালা, Mat_রাশিমালা& res) ধ্রুব
{
    CV_INSTRUMENT_REGION();

    Mat m;
    রাশিমালা.op->assign(রাশিমালা, m);
    MatOp_Bin::make_রাশিমালা(res, '/', m, Mat(), s);
}


শূন্য MatOp::abs(ধ্রুব Mat_রাশিমালা& রাশিমালা, Mat_রাশিমালা& res) ধ্রুব
{
    CV_INSTRUMENT_REGION();

    Mat m;
    রাশিমালা.op->assign(রাশিমালা, m);
    MatOp_Bin::make_রাশিমালা(res, 'a', m, Mat());
}


শূন্য MatOp::transpose(ধ্রুব Mat_রাশিমালা& রাশিমালা, Mat_রাশিমালা& res) ধ্রুব
{
    CV_INSTRUMENT_REGION();

    Mat m;
    রাশিমালা.op->assign(রাশিমালা, m);
    MatOp_T::make_রাশিমালা(res, m, 1);
}


শূন্য MatOp::matmul(ধ্রুব Mat_রাশিমালা& e1, ধ্রুব Mat_রাশিমালা& e2, Mat_রাশিমালা& res) ধ্রুব
{
    যদি( এতত্ == e2.op )
    {
        দ্বিগুণ scale = 1;
        পূর্ণাঙ্ক flags = 0;
        Mat m1, m2;

        যদি( isT(e1) )
        {
            flags = cv::GEMM_1_T;
            scale = e1.alpha;
            m1 = e1.a;
        }
        অন্যথা যদি( isScaled(e1) )
        {
            scale = e1.alpha;
            m1 = e1.a;
        }
        অন্যথা
            e1.op->assign(e1, m1);

        যদি( isT(e2) )
        {
            flags |= cv::GEMM_2_T;
            scale *= e2.alpha;
            m2 = e2.a;
        }
        অন্যথা যদি( isScaled(e2) )
        {
            scale *= e2.alpha;
            m2 = e2.a;
        }
        অন্যথা
            e2.op->assign(e2, m2);

        MatOp_GEMM::make_রাশিমালা(res, flags, m1, m2, scale);
    }
    অন্যথা
        e2.op->matmul(e1, e2, res);
}


শূন্য MatOp::invert(ধ্রুব Mat_রাশিমালা& রাশিমালা, পূর্ণাঙ্ক method, Mat_রাশিমালা& res) ধ্রুব
{
    Mat m;
    রাশিমালা.op->assign(রাশিমালা, m);
    MatOp_Invert::make_রাশিমালা(res, method, m);
}


Size MatOp::size(ধ্রুব Mat_রাশিমালা& রাশিমালা) ধ্রুব
{
    প্রত্যাগম !রাশিমালা.a.empty() ? রাশিমালা.a.size() : রাশিমালা.b.empty() ? রাশিমালা.b.size() : রাশিমালা.c.size();
}

পূর্ণাঙ্ক MatOp::type(ধ্রুব Mat_রাশিমালা& রাশিমালা) ধ্রুব
{
    CV_INSTRUMENT_REGION();

    প্রত্যাগম !রাশিমালা.a.empty() ? রাশিমালা.a.type() : রাশিমালা.b.empty() ? রাশিমালা.b.type() : রাশিমালা.c.type();
}

//////////////////////////////////////////////////////////////////////////////////////////////////

Mat_রাশিমালা::Mat_রাশিমালা(ধ্রুব Mat& m) : op(&g_MatOp_Identity), flags(0), a(m), b(Mat()), c(Mat()), alpha(1), beta(0), s(Scalar())
{
}

Mat_রাশিমালা Mat_রাশিমালা::row(পূর্ণাঙ্ক y) ধ্রুব
{
    Mat_রাশিমালা e;
    op->roi(*এতত্, Range(y, y+1), Range::all(), e);
    প্রত্যাগম e;
}

Mat_রাশিমালা Mat_রাশিমালা::col(পূর্ণাঙ্ক x) ধ্রুব
{
    Mat_রাশিমালা e;
    op->roi(*এতত্, Range::all(), Range(x, x+1), e);
    প্রত্যাগম e;
}

Mat_রাশিমালা Mat_রাশিমালা::diag(পূর্ণাঙ্ক d) ধ্রুব
{
    Mat_রাশিমালা e;
    op->diag(*এতত্, d, e);
    প্রত্যাগম e;
}

Mat_রাশিমালা Mat_রাশিমালা::operator()( ধ্রুব Range& rowRange, ধ্রুব Range& colRange ) ধ্রুব
{
    Mat_রাশিমালা e;
    op->roi(*এতত্, rowRange, colRange, e);
    প্রত্যাগম e;
}

Mat_রাশিমালা Mat_রাশিমালা::operator()( ধ্রুব Rect& roi ) ধ্রুব
{
    Mat_রাশিমালা e;
    op->roi(*এতত্, Range(roi.y, roi.y + roi.height), Range(roi.x, roi.x + roi.width), e);
    প্রত্যাগম e;
}

Mat Mat_রাশিমালা::cross(ধ্রুব Mat& m) ধ্রুব
{
    প্রত্যাগম ((Mat)*এতত্).cross(m);
}

দ্বিগুণ Mat_রাশিমালা::dot(ধ্রুব Mat& m) ধ্রুব
{
    প্রত্যাগম ((Mat)*এতত্).dot(m);
}

Mat_রাশিমালা Mat_রাশিমালা::t() ধ্রুব
{
    Mat_রাশিমালা e;
    op->transpose(*এতত্, e);
    প্রত্যাগম e;
}

Mat_রাশিমালা Mat_রাশিমালা::inv(পূর্ণাঙ্ক method) ধ্রুব
{
    Mat_রাশিমালা e;
    op->invert(*এতত্, method, e);
    প্রত্যাগম e;
}

Mat_রাশিমালা Mat_রাশিমালা::mul(ধ্রুব Mat_রাশিমালা& e, দ্বিগুণ scale) ধ্রুব
{
    Mat_রাশিমালা en;
    op->multiply(*এতত্, e, en, scale);
    প্রত্যাগম en;
}

Mat_রাশিমালা Mat_রাশিমালা::mul(ধ্রুব Mat& m, দ্বিগুণ scale) ধ্রুব
{
    Mat_রাশিমালা e;
    op->multiply(*এতত্, Mat_রাশিমালা(m), e, scale);
    প্রত্যাগম e;
}

Mat_রাশিমালা operator + (ধ্রুব Mat& a, ধ্রুব Mat& b)
{
    checkOperandsExist(a, b);
    Mat_রাশিমালা e;
    MatOp_AddEx::make_রাশিমালা(e, a, b, 1, 1);
    প্রত্যাগম e;
}

Mat_রাশিমালা operator + (ধ্রুব Mat& a, ধ্রুব Scalar& s)
{
    checkOperandsExist(a);
    Mat_রাশিমালা e;
    MatOp_AddEx::make_রাশিমালা(e, a, Mat(), 1, 0, s);
    প্রত্যাগম e;
}

Mat_রাশিমালা operator + (ধ্রুব Scalar& s, ধ্রুব Mat& a)
{
    checkOperandsExist(a);
    Mat_রাশিমালা e;
    MatOp_AddEx::make_রাশিমালা(e, a, Mat(), 1, 0, s);
    প্রত্যাগম e;
}

Mat_রাশিমালা operator + (ধ্রুব Mat_রাশিমালা& e, ধ্রুব Mat& m)
{
    Mat_রাশিমালা en;
    e.op->add(e, Mat_রাশিমালা(m), en);
    প্রত্যাগম en;
}

Mat_রাশিমালা operator + (ধ্রুব Mat& m, ধ্রুব Mat_রাশিমালা& e)
{
    checkOperandsExist(m);
    Mat_রাশিমালা en;
    e.op->add(e, Mat_রাশিমালা(m), en);
    প্রত্যাগম en;
}

Mat_রাশিমালা operator + (ধ্রুব Mat_রাশিমালা& e, ধ্রুব Scalar& s)
{
    Mat_রাশিমালা en;
    e.op->add(e, s, en);
    প্রত্যাগম en;
}

Mat_রাশিমালা operator + (ধ্রুব Scalar& s, ধ্রুব Mat_রাশিমালা& e)
{
    Mat_রাশিমালা en;
    e.op->add(e, s, en);
    প্রত্যাগম en;
}

Mat_রাশিমালা operator + (ধ্রুব Mat_রাশিমালা& e1, ধ্রুব Mat_রাশিমালা& e2)
{
    Mat_রাশিমালা en;
    e1.op->add(e1, e2, en);
    প্রত্যাগম en;
}

Mat_রাশিমালা operator - (ধ্রুব Mat& a, ধ্রুব Mat& b)
{
    checkOperandsExist(a, b);
    Mat_রাশিমালা e;
    MatOp_AddEx::make_রাশিমালা(e, a, b, 1, -1);
    প্রত্যাগম e;
}

Mat_রাশিমালা operator - (ধ্রুব Mat& a, ধ্রুব Scalar& s)
{
    checkOperandsExist(a);
    Mat_রাশিমালা e;
    MatOp_AddEx::make_রাশিমালা(e, a, Mat(), 1, 0, -s);
    প্রত্যাগম e;
}

Mat_রাশিমালা operator - (ধ্রুব Scalar& s, ধ্রুব Mat& a)
{
    checkOperandsExist(a);
    Mat_রাশিমালা e;
    MatOp_AddEx::make_রাশিমালা(e, a, Mat(), -1, 0, s);
    প্রত্যাগম e;
}

Mat_রাশিমালা operator - (ধ্রুব Mat_রাশিমালা& e, ধ্রুব Mat& m)
{
    checkOperandsExist(m);
    Mat_রাশিমালা en;
    e.op->subtract(e, Mat_রাশিমালা(m), en);
    প্রত্যাগম en;
}

Mat_রাশিমালা operator - (ধ্রুব Mat& m, ধ্রুব Mat_রাশিমালা& e)
{
    checkOperandsExist(m);
    Mat_রাশিমালা en;
    e.op->subtract(Mat_রাশিমালা(m), e, en);
    প্রত্যাগম en;
}

Mat_রাশিমালা operator - (ধ্রুব Mat_রাশিমালা& e, ধ্রুব Scalar& s)
{
    Mat_রাশিমালা en;
    e.op->add(e, -s, en);
    প্রত্যাগম en;
}

Mat_রাশিমালা operator - (ধ্রুব Scalar& s, ধ্রুব Mat_রাশিমালা& e)
{
    Mat_রাশিমালা en;
    e.op->subtract(s, e, en);
    প্রত্যাগম en;
}

Mat_রাশিমালা operator - (ধ্রুব Mat_রাশিমালা& e1, ধ্রুব Mat_রাশিমালা& e2)
{
    Mat_রাশিমালা en;
    e1.op->subtract(e1, e2, en);
    প্রত্যাগম en;
}

Mat_রাশিমালা operator - (ধ্রুব Mat& m)
{
    checkOperandsExist(m);
    Mat_রাশিমালা e;
    MatOp_AddEx::make_রাশিমালা(e, m, Mat(), -1, 0);
    প্রত্যাগম e;
}

Mat_রাশিমালা operator - (ধ্রুব Mat_রাশিমালা& e)
{
    Mat_রাশিমালা en;
    e.op->subtract(Scalar(0), e, en);
    প্রত্যাগম en;
}

Mat_রাশিমালা operator * (ধ্রুব Mat& a, ধ্রুব Mat& b)
{
    checkOperandsExist(a, b);
    Mat_রাশিমালা e;
    MatOp_GEMM::make_রাশিমালা(e, 0, a, b);
    প্রত্যাগম e;
}

Mat_রাশিমালা operator * (ধ্রুব Mat& a, দ্বিগুণ s)
{
    checkOperandsExist(a);
    Mat_রাশিমালা e;
    MatOp_AddEx::make_রাশিমালা(e, a, Mat(), s, 0);
    প্রত্যাগম e;
}

Mat_রাশিমালা operator * (দ্বিগুণ s, ধ্রুব Mat& a)
{
    checkOperandsExist(a);
    Mat_রাশিমালা e;
    MatOp_AddEx::make_রাশিমালা(e, a, Mat(), s, 0);
    প্রত্যাগম e;
}

Mat_রাশিমালা operator * (ধ্রুব Mat_রাশিমালা& e, ধ্রুব Mat& m)
{
    Mat_রাশিমালা en;
    e.op->matmul(e, Mat_রাশিমালা(m), en);
    প্রত্যাগম en;
}

Mat_রাশিমালা operator * (ধ্রুব Mat& m, ধ্রুব Mat_রাশিমালা& e)
{
    checkOperandsExist(m);
    Mat_রাশিমালা en;
    e.op->matmul(Mat_রাশিমালা(m), e, en);
    প্রত্যাগম en;
}

Mat_রাশিমালা operator * (ধ্রুব Mat_রাশিমালা& e, দ্বিগুণ s)
{
    Mat_রাশিমালা en;
    e.op->multiply(e, s, en);
    প্রত্যাগম en;
}

Mat_রাশিমালা operator * (দ্বিগুণ s, ধ্রুব Mat_রাশিমালা& e)
{
    Mat_রাশিমালা en;
    e.op->multiply(e, s, en);
    প্রত্যাগম en;
}

Mat_রাশিমালা operator * (ধ্রুব Mat_রাশিমালা& e1, ধ্রুব Mat_রাশিমালা& e2)
{
    Mat_রাশিমালা en;
    e1.op->matmul(e1, e2, en);
    প্রত্যাগম en;
}

Mat_রাশিমালা operator / (ধ্রুব Mat& a, ধ্রুব Mat& b)
{
    checkOperandsExist(a, b);
    Mat_রাশিমালা e;
    MatOp_Bin::make_রাশিমালা(e, '/', a, b);
    প্রত্যাগম e;
}

Mat_রাশিমালা operator / (ধ্রুব Mat& a, দ্বিগুণ s)
{
    checkOperandsExist(a);
    Mat_রাশিমালা e;
    MatOp_AddEx::make_রাশিমালা(e, a, Mat(), 1./s, 0);
    প্রত্যাগম e;
}

Mat_রাশিমালা operator / (দ্বিগুণ s, ধ্রুব Mat& a)
{
    checkOperandsExist(a);
    Mat_রাশিমালা e;
    MatOp_Bin::make_রাশিমালা(e, '/', a, Mat(), s);
    প্রত্যাগম e;
}

Mat_রাশিমালা operator / (ধ্রুব Mat_রাশিমালা& e, ধ্রুব Mat& m)
{
    Mat_রাশিমালা en;
    e.op->divide(e, Mat_রাশিমালা(m), en);
    প্রত্যাগম en;
}

Mat_রাশিমালা operator / (ধ্রুব Mat& m, ধ্রুব Mat_রাশিমালা& e)
{
    checkOperandsExist(m);
    Mat_রাশিমালা en;
    e.op->divide(Mat_রাশিমালা(m), e, en);
    প্রত্যাগম en;
}

Mat_রাশিমালা operator / (ধ্রুব Mat_রাশিমালা& e, দ্বিগুণ s)
{
    Mat_রাশিমালা en;
    e.op->multiply(e, 1./s, en);
    প্রত্যাগম en;
}

Mat_রাশিমালা operator / (দ্বিগুণ s, ধ্রুব Mat_রাশিমালা& e)
{
    Mat_রাশিমালা en;
    e.op->divide(s, e, en);
    প্রত্যাগম en;
}

Mat_রাশিমালা operator / (ধ্রুব Mat_রাশিমালা& e1, ধ্রুব Mat_রাশিমালা& e2)
{
    Mat_রাশিমালা en;
    e1.op->divide(e1, e2, en);
    প্রত্যাগম en;
}

Mat_রাশিমালা operator < (ধ্রুব Mat& a, ধ্রুব Mat& b)
{
    checkOperandsExist(a, b);
    Mat_রাশিমালা e;
    MatOp_Cmp::make_রাশিমালা(e, cv::CMP_LT, a, b);
    প্রত্যাগম e;
}

Mat_রাশিমালা operator < (ধ্রুব Mat& a, দ্বিগুণ s)
{
    checkOperandsExist(a);
    Mat_রাশিমালা e;
    MatOp_Cmp::make_রাশিমালা(e, cv::CMP_LT, a, s);
    প্রত্যাগম e;
}

Mat_রাশিমালা operator < (দ্বিগুণ s, ধ্রুব Mat& a)
{
    checkOperandsExist(a);
    Mat_রাশিমালা e;
    MatOp_Cmp::make_রাশিমালা(e, cv::CMP_GT, a, s);
    প্রত্যাগম e;
}

Mat_রাশিমালা operator <= (ধ্রুব Mat& a, ধ্রুব Mat& b)
{
    checkOperandsExist(a, b);
    Mat_রাশিমালা e;
    MatOp_Cmp::make_রাশিমালা(e, cv::CMP_LE, a, b);
    প্রত্যাগম e;
}

Mat_রাশিমালা operator <= (ধ্রুব Mat& a, দ্বিগুণ s)
{
    checkOperandsExist(a);
    Mat_রাশিমালা e;
    MatOp_Cmp::make_রাশিমালা(e, cv::CMP_LE, a, s);
    প্রত্যাগম e;
}

Mat_রাশিমালা operator <= (দ্বিগুণ s, ধ্রুব Mat& a)
{
    checkOperandsExist(a);
    Mat_রাশিমালা e;
    MatOp_Cmp::make_রাশিমালা(e, cv::CMP_GE, a, s);
    প্রত্যাগম e;
}

Mat_রাশিমালা operator == (ধ্রুব Mat& a, ধ্রুব Mat& b)
{
    checkOperandsExist(a, b);
    Mat_রাশিমালা e;
    MatOp_Cmp::make_রাশিমালা(e, cv::CMP_EQ, a, b);
    প্রত্যাগম e;
}

Mat_রাশিমালা operator == (ধ্রুব Mat& a, দ্বিগুণ s)
{
    checkOperandsExist(a);
    Mat_রাশিমালা e;
    MatOp_Cmp::make_রাশিমালা(e, cv::CMP_EQ, a, s);
    প্রত্যাগম e;
}

Mat_রাশিমালা operator == (দ্বিগুণ s, ধ্রুব Mat& a)
{
    checkOperandsExist(a);
    Mat_রাশিমালা e;
    MatOp_Cmp::make_রাশিমালা(e, cv::CMP_EQ, a, s);
    প্রত্যাগম e;
}

Mat_রাশিমালা operator != (ধ্রুব Mat& a, ধ্রুব Mat& b)
{
    checkOperandsExist(a, b);
    Mat_রাশিমালা e;
    MatOp_Cmp::make_রাশিমালা(e, cv::CMP_NE, a, b);
    প্রত্যাগম e;
}

Mat_রাশিমালা operator != (ধ্রুব Mat& a, দ্বিগুণ s)
{
    checkOperandsExist(a);
    Mat_রাশিমালা e;
    MatOp_Cmp::make_রাশিমালা(e, cv::CMP_NE, a, s);
    প্রত্যাগম e;
}

Mat_রাশিমালা operator != (দ্বিগুণ s, ধ্রুব Mat& a)
{
    checkOperandsExist(a);
    Mat_রাশিমালা e;
    MatOp_Cmp::make_রাশিমালা(e, cv::CMP_NE, a, s);
    প্রত্যাগম e;
}

Mat_রাশিমালা operator >= (ধ্রুব Mat& a, ধ্রুব Mat& b)
{
    checkOperandsExist(a, b);
    Mat_রাশিমালা e;
    MatOp_Cmp::make_রাশিমালা(e, cv::CMP_GE, a, b);
    প্রত্যাগম e;
}

Mat_রাশিমালা operator >= (ধ্রুব Mat& a, দ্বিগুণ s)
{
    checkOperandsExist(a);
    Mat_রাশিমালা e;
    MatOp_Cmp::make_রাশিমালা(e, cv::CMP_GE, a, s);
    প্রত্যাগম e;
}

Mat_রাশিমালা operator >= (দ্বিগুণ s, ধ্রুব Mat& a)
{
    checkOperandsExist(a);
    Mat_রাশিমালা e;
    MatOp_Cmp::make_রাশিমালা(e, cv::CMP_LE, a, s);
    প্রত্যাগম e;
}

Mat_রাশিমালা operator > (ধ্রুব Mat& a, ধ্রুব Mat& b)
{
    checkOperandsExist(a, b);
    Mat_রাশিমালা e;
    MatOp_Cmp::make_রাশিমালা(e, cv::CMP_GT, a, b);
    প্রত্যাগম e;
}

Mat_রাশিমালা operator > (ধ্রুব Mat& a, দ্বিগুণ s)
{
    checkOperandsExist(a);
    Mat_রাশিমালা e;
    MatOp_Cmp::make_রাশিমালা(e, cv::CMP_GT, a, s);
    প্রত্যাগম e;
}

Mat_রাশিমালা operator > (দ্বিগুণ s, ধ্রুব Mat& a)
{
    checkOperandsExist(a);
    Mat_রাশিমালা e;
    MatOp_Cmp::make_রাশিমালা(e, cv::CMP_LT, a, s);
    প্রত্যাগম e;
}

Mat_রাশিমালা min(ধ্রুব Mat& a, ধ্রুব Mat& b)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a, b);
    Mat_রাশিমালা e;
    MatOp_Bin::make_রাশিমালা(e, 'm', a, b);
    প্রত্যাগম e;
}

Mat_রাশিমালা min(ধ্রুব Mat& a, দ্বিগুণ s)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    Mat_রাশিমালা e;
    MatOp_Bin::make_রাশিমালা(e, 'n', a, s);
    প্রত্যাগম e;
}

Mat_রাশিমালা min(দ্বিগুণ s, ধ্রুব Mat& a)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    Mat_রাশিমালা e;
    MatOp_Bin::make_রাশিমালা(e, 'n', a, s);
    প্রত্যাগম e;
}

Mat_রাশিমালা max(ধ্রুব Mat& a, ধ্রুব Mat& b)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a, b);
    Mat_রাশিমালা e;
    MatOp_Bin::make_রাশিমালা(e, 'M', a, b);
    প্রত্যাগম e;
}

Mat_রাশিমালা max(ধ্রুব Mat& a, দ্বিগুণ s)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    Mat_রাশিমালা e;
    MatOp_Bin::make_রাশিমালা(e, 'N', a, s);
    প্রত্যাগম e;
}

Mat_রাশিমালা max(দ্বিগুণ s, ধ্রুব Mat& a)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    Mat_রাশিমালা e;
    MatOp_Bin::make_রাশিমালা(e, 'N', a, s);
    প্রত্যাগম e;
}

Mat_রাশিমালা operator & (ধ্রুব Mat& a, ধ্রুব Mat& b)
{
    checkOperandsExist(a, b);
    Mat_রাশিমালা e;
    MatOp_Bin::make_রাশিমালা(e, '&', a, b);
    প্রত্যাগম e;
}

Mat_রাশিমালা operator & (ধ্রুব Mat& a, ধ্রুব Scalar& s)
{
    checkOperandsExist(a);
    Mat_রাশিমালা e;
    MatOp_Bin::make_রাশিমালা(e, '&', a, s);
    প্রত্যাগম e;
}

Mat_রাশিমালা operator & (ধ্রুব Scalar& s, ধ্রুব Mat& a)
{
    checkOperandsExist(a);
    Mat_রাশিমালা e;
    MatOp_Bin::make_রাশিমালা(e, '&', a, s);
    প্রত্যাগম e;
}

Mat_রাশিমালা operator | (ধ্রুব Mat& a, ধ্রুব Mat& b)
{
    checkOperandsExist(a, b);
    Mat_রাশিমালা e;
    MatOp_Bin::make_রাশিমালা(e, '|', a, b);
    প্রত্যাগম e;
}

Mat_রাশিমালা operator | (ধ্রুব Mat& a, ধ্রুব Scalar& s)
{
    checkOperandsExist(a);
    Mat_রাশিমালা e;
    MatOp_Bin::make_রাশিমালা(e, '|', a, s);
    প্রত্যাগম e;
}

Mat_রাশিমালা operator | (ধ্রুব Scalar& s, ধ্রুব Mat& a)
{
    checkOperandsExist(a);
    Mat_রাশিমালা e;
    MatOp_Bin::make_রাশিমালা(e, '|', a, s);
    প্রত্যাগম e;
}

Mat_রাশিমালা operator ^ (ধ্রুব Mat& a, ধ্রুব Mat& b)
{
    checkOperandsExist(a, b);
    Mat_রাশিমালা e;
    MatOp_Bin::make_রাশিমালা(e, '^', a, b);
    প্রত্যাগম e;
}

Mat_রাশিমালা operator ^ (ধ্রুব Mat& a, ধ্রুব Scalar& s)
{
    checkOperandsExist(a);
    Mat_রাশিমালা e;
    MatOp_Bin::make_রাশিমালা(e, '^', a, s);
    প্রত্যাগম e;
}

Mat_রাশিমালা operator ^ (ধ্রুব Scalar& s, ধ্রুব Mat& a)
{
    checkOperandsExist(a);
    Mat_রাশিমালা e;
    MatOp_Bin::make_রাশিমালা(e, '^', a, s);
    প্রত্যাগম e;
}

Mat_রাশিমালা operator ~(ধ্রুব Mat& a)
{
    checkOperandsExist(a);
    Mat_রাশিমালা e;
    MatOp_Bin::make_রাশিমালা(e, '~', a, Scalar());
    প্রত্যাগম e;
}

Mat_রাশিমালা abs(ধ্রুব Mat& a)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    Mat_রাশিমালা e;
    MatOp_Bin::make_রাশিমালা(e, 'a', a, Scalar());
    প্রত্যাগম e;
}

Mat_রাশিমালা abs(ধ্রুব Mat_রাশিমালা& e)
{
    CV_INSTRUMENT_REGION();

    Mat_রাশিমালা en;
    e.op->abs(e, en);
    প্রত্যাগম en;
}


Size Mat_রাশিমালা::size() ধ্রুব
{
    যদি( isT(*এতত্) || isInv(*এতত্) )
        প্রত্যাগম Size(a.rows, a.cols);
    যদি( isSolve(*এতত্) )
        প্রত্যাগম Size(b.cols, a.cols);
    যদি( isInitializer(*এতত্) )
        প্রত্যাগম a.size();
    প্রত্যাগম op ? op->size(*এতত্) : Size();
}


পূর্ণাঙ্ক Mat_রাশিমালা::type() ধ্রুব
{
    CV_INSTRUMENT_REGION();

    যদি( isInitializer(*এতত্) )
        প্রত্যাগম a.type();
    যদি( isCmp(*এতত্) )
        প্রত্যাগম CV_MAKETYPE(CV_8U, a.channels());
    প্রত্যাগম op ? op->type(*এতত্) : -1;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////

শূন্য MatOp_Identity::assign(ধ্রুব Mat_রাশিমালা& e, Mat& m, পূর্ণাঙ্ক _type) ধ্রুব
{
    যদি( _type == -1 || _type == e.a.type() )
        m = e.a;
    অন্যথা
    {
        CV_Assert( CV_MAT_CN(_type) == e.a.channels() );
        e.a.convertTo(m, _type);
    }
}

অন্তঃস্থ শূন্য MatOp_Identity::make_রাশিমালা(Mat_রাশিমালা& res, ধ্রুব Mat& m)
{
    res = Mat_রাশিমালা(&g_MatOp_Identity, 0, m, Mat(), Mat(), 1, 0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

শূন্য MatOp_AddEx::assign(ধ্রুব Mat_রাশিমালা& e, Mat& m, পূর্ণাঙ্ক _type) ধ্রুব
{
    Mat temp, &dst = _type == -1 || e.a.type() == _type ? m : temp;
    যদি( e.b.data )
    {
        যদি( e.s == Scalar() || !e.s.isReal() )
        {
            যদি( e.alpha == 1 )
            {
                যদি( e.beta == 1 )
                    cv::add(e.a, e.b, dst);
                অন্যথা যদি( e.beta == -1 )
                    cv::subtract(e.a, e.b, dst);
                অন্যথা
                    cv::scaleAdd(e.b, e.beta, e.a, dst);
            }
            অন্যথা যদি( e.beta == 1 )
            {
                যদি( e.alpha == -1 )
                    cv::subtract(e.b, e.a, dst);
                অন্যথা
                    cv::scaleAdd(e.a, e.alpha, e.b, dst);
            }
            অন্যথা
                cv::addWeighted(e.a, e.alpha, e.b, e.beta, 0, dst);

            যদি( !e.s.isReal() )
                cv::add(dst, e.s, dst);
        }
        অন্যথা
        {
            যদি (e.a.channels() > 1)
                CV_LOG_ONCE_WARNING(রিক্ত, "OpenCV/MatExpr: processing of multi-channel arrays might be changed in the future: "
                                          "https://github.com/opencv/opencv/issues/16739");
            cv::addWeighted(e.a, e.alpha, e.b, e.beta, e.s[0], dst);
        }
    }
    অন্যথা যদি( e.s.isReal() && (dst.data != m.data || fabs(e.alpha) != 1))
    {
        যদি (e.a.channels() > 1 && e.s[0] != 0.0)
            CV_LOG_ONCE_WARNING(রিক্ত, "OpenCV/MatExpr: processing of multi-channel arrays might be changed in the future: "
                                      "https://github.com/opencv/opencv/issues/16739");
        e.a.convertTo(m, _type, e.alpha, e.s[0]);
        প্রত্যাগম;
    }
    অন্যথা যদি( e.alpha == 1 )
        cv::add(e.a, e.s, dst);
    অন্যথা যদি( e.alpha == -1 )
        cv::subtract(e.s, e.a, dst);
    অন্যথা
    {
        e.a.convertTo(dst, e.a.type(), e.alpha);
        cv::add(dst, e.s, dst);
    }

    যদি( dst.data != m.data )
        dst.convertTo(m, m.type());
}


শূন্য MatOp_AddEx::add(ধ্রুব Mat_রাশিমালা& e, ধ্রুব Scalar& s, Mat_রাশিমালা& res) ধ্রুব
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.s += s;
}


শূন্য MatOp_AddEx::subtract(ধ্রুব Scalar& s, ধ্রুব Mat_রাশিমালা& e, Mat_রাশিমালা& res) ধ্রুব
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha = -res.alpha;
    res.beta = -res.beta;
    res.s = s - res.s;
}

শূন্য MatOp_AddEx::multiply(ধ্রুব Mat_রাশিমালা& e, দ্বিগুণ s, Mat_রাশিমালা& res) ধ্রুব
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha *= s;
    res.beta *= s;
    res.s *= s;
}

শূন্য MatOp_AddEx::divide(দ্বিগুণ s, ধ্রুব Mat_রাশিমালা& e, Mat_রাশিমালা& res) ধ্রুব
{
    CV_INSTRUMENT_REGION();

    যদি( isScaled(e) )
        MatOp_Bin::make_রাশিমালা(res, '/', e.a, Mat(), s/e.alpha);
    অন্যথা
        MatOp::divide(s, e, res);
}


শূন্য MatOp_AddEx::transpose(ধ্রুব Mat_রাশিমালা& e, Mat_রাশিমালা& res) ধ্রুব
{
    CV_INSTRUMENT_REGION();

    যদি( isScaled(e) )
        MatOp_T::make_রাশিমালা(res, e.a, e.alpha);
    অন্যথা
        MatOp::transpose(e, res);
}

শূন্য MatOp_AddEx::abs(ধ্রুব Mat_রাশিমালা& e, Mat_রাশিমালা& res) ধ্রুব
{
    CV_INSTRUMENT_REGION();

    যদি( (!e.b.data || e.beta == 0) && fabs(e.alpha) == 1 )
        MatOp_Bin::make_রাশিমালা(res, 'a', e.a, -e.s*e.alpha);
    অন্যথা যদি( e.b.data && e.alpha + e.beta == 0 && e.alpha*e.beta == -1 )
        MatOp_Bin::make_রাশিমালা(res, 'a', e.a, e.b);
    অন্যথা
        MatOp::abs(e, res);
}

অন্তঃস্থ শূন্য MatOp_AddEx::make_রাশিমালা(Mat_রাশিমালা& res, ধ্রুব Mat& a, ধ্রুব Mat& b, দ্বিগুণ alpha, দ্বিগুণ beta, ধ্রুব Scalar& s)
{
    res = Mat_রাশিমালা(&g_MatOp_AddEx, 0, a, b, Mat(), alpha, beta, s);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

শূন্য MatOp_Bin::assign(ধ্রুব Mat_রাশিমালা& e, Mat& m, পূর্ণাঙ্ক _type) ধ্রুব
{
    Mat temp, &dst = _type == -1 || e.a.type() == _type ? m : temp;

    যদি( e.flags == '*' )
        cv::multiply(e.a, e.b, dst, e.alpha);
    অন্যথা যদি( e.flags == '/' && e.b.data )
        cv::divide(e.a, e.b, dst, e.alpha);
    অন্যথা যদি( e.flags == '/' && !e.b.data )
        cv::divide(e.alpha, e.a, dst );
    অন্যথা যদি( e.flags == '&' && e.b.data )
        bitwise_and(e.a, e.b, dst);
    অন্যথা যদি( e.flags == '&' && !e.b.data )
        bitwise_and(e.a, e.s, dst);
    অন্যথা যদি( e.flags == '|' && e.b.data )
        bitwise_or(e.a, e.b, dst);
    অন্যথা যদি( e.flags == '|' && !e.b.data )
        bitwise_or(e.a, e.s, dst);
    অন্যথা যদি( e.flags == '^' && e.b.data )
        bitwise_xor(e.a, e.b, dst);
    অন্যথা যদি( e.flags == '^' && !e.b.data )
        bitwise_xor(e.a, e.s, dst);
    অন্যথা যদি( e.flags == '~' && !e.b.data )
        bitwise_not(e.a, dst);
    অন্যথা যদি( e.flags == 'm' )
        cv::min(e.a, e.b, dst);
    অন্যথা যদি( e.flags == 'n' )
        cv::min(e.a, e.s[0], dst);
    অন্যথা যদি( e.flags == 'M' )
        cv::max(e.a, e.b, dst);
    অন্যথা যদি( e.flags == 'N' )
        cv::max(e.a, e.s[0], dst);
    অন্যথা যদি( e.flags == 'a' && e.b.data )
        cv::absdiff(e.a, e.b, dst);
    অন্যথা যদি( e.flags == 'a' && !e.b.data )
        cv::absdiff(e.a, e.s, dst);
    অন্যথা
        CV_Error(cv::Error::StsError, "Unknown operation");

    যদি( dst.data != m.data )
        dst.convertTo(m, _type);
}

শূন্য MatOp_Bin::multiply(ধ্রুব Mat_রাশিমালা& e, দ্বিগুণ s, Mat_রাশিমালা& res) ধ্রুব
{
    CV_INSTRUMENT_REGION();

    যদি( e.flags == '*' || e.flags == '/' )
    {
        res = e;
        res.alpha *= s;
    }
    অন্যথা
        MatOp::multiply(e, s, res);
}

শূন্য MatOp_Bin::divide(দ্বিগুণ s, ধ্রুব Mat_রাশিমালা& e, Mat_রাশিমালা& res) ধ্রুব
{
    CV_INSTRUMENT_REGION();

    যদি( e.flags == '/' && (!e.b.data || e.beta == 0) )
        MatOp_AddEx::make_রাশিমালা(res, e.a, Mat(), s/e.alpha, 0);
    অন্যথা
        MatOp::divide(s, e, res);
}

অন্তঃস্থ শূন্য MatOp_Bin::make_রাশিমালা(Mat_রাশিমালা& res, বর্ণ op, ধ্রুব Mat& a, ধ্রুব Mat& b, দ্বিগুণ scale)
{
    res = Mat_রাশিমালা(&g_MatOp_Bin, op, a, b, Mat(), scale, b.data ? 1 : 0);
}

অন্তঃস্থ শূন্য MatOp_Bin::make_রাশিমালা(Mat_রাশিমালা& res, বর্ণ op, ধ্রুব Mat& a, ধ্রুব Scalar& s)
{
    res = Mat_রাশিমালা(&g_MatOp_Bin, op, a, Mat(), Mat(), 1, 0, s);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

শূন্য MatOp_Cmp::assign(ধ্রুব Mat_রাশিমালা& e, Mat& m, পূর্ণাঙ্ক _type) ধ্রুব
{
    Mat temp, &dst = _type == -1 || _type == CV_8U ? m : temp;

    যদি( e.b.data )
        cv::compare(e.a, e.b, dst, e.flags);
    অন্যথা
        cv::compare(e.a, e.alpha, dst, e.flags);

    যদি( dst.data != m.data )
        dst.convertTo(m, _type);
}

অন্তঃস্থ শূন্য MatOp_Cmp::make_রাশিমালা(Mat_রাশিমালা& res, পূর্ণাঙ্ক cmpop, ধ্রুব Mat& a, ধ্রুব Mat& b)
{
    res = Mat_রাশিমালা(&g_MatOp_Cmp, cmpop, a, b, Mat(), 1, 1);
}

অন্তঃস্থ শূন্য MatOp_Cmp::make_রাশিমালা(Mat_রাশিমালা& res, পূর্ণাঙ্ক cmpop, ধ্রুব Mat& a, দ্বিগুণ alpha)
{
    res = Mat_রাশিমালা(&g_MatOp_Cmp, cmpop, a, Mat(), Mat(), alpha, 1);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

শূন্য MatOp_T::assign(ধ্রুব Mat_রাশিমালা& e, Mat& m, পূর্ণাঙ্ক _type) ধ্রুব
{
    Mat temp, &dst = _type == -1 || _type == e.a.type() ? m : temp;

    cv::transpose(e.a, dst);

    যদি( dst.data != m.data || e.alpha != 1 )
        dst.convertTo(m, _type, e.alpha);
}

শূন্য MatOp_T::multiply(ধ্রুব Mat_রাশিমালা& e, দ্বিগুণ s, Mat_রাশিমালা& res) ধ্রুব
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha *= s;
}

শূন্য MatOp_T::transpose(ধ্রুব Mat_রাশিমালা& e, Mat_রাশিমালা& res) ধ্রুব
{
    CV_INSTRUMENT_REGION();

    যদি( e.alpha == 1 )
        MatOp_Identity::make_রাশিমালা(res, e.a);
    অন্যথা
        MatOp_AddEx::make_রাশিমালা(res, e.a, Mat(), e.alpha, 0);
}

অন্তঃস্থ শূন্য MatOp_T::make_রাশিমালা(Mat_রাশিমালা& res, ধ্রুব Mat& a, দ্বিগুণ alpha)
{
    res = Mat_রাশিমালা(&g_MatOp_T, 0, a, Mat(), Mat(), alpha, 0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

শূন্য MatOp_GEMM::assign(ধ্রুব Mat_রাশিমালা& e, Mat& m, পূর্ণাঙ্ক _type) ধ্রুব
{
    Mat temp, &dst = _type == -1 || _type == e.a.type() ? m : temp;

    cv::gemm(e.a, e.b, e.alpha, e.c, e.beta, dst, e.flags);
    যদি( dst.data != m.data )
        dst.convertTo(m, _type);
}

শূন্য MatOp_GEMM::add(ধ্রুব Mat_রাশিমালা& e1, ধ্রুব Mat_রাশিমালা& e2, Mat_রাশিমালা& res) ধ্রুব
{
    CV_INSTRUMENT_REGION();

    তার্কিক i1 = isIdentity(e1), i2 = isIdentity(e2);
    দ্বিগুণ alpha1 = i1 ? 1 : e1.alpha, alpha2 = i2 ? 1 : e2.alpha;

    যদি( isMatProd(e1) && (i2 || isScaled(e2) || isT(e2)) )
        MatOp_GEMM::make_রাশিমালা(res, (e1.flags & ~cv::GEMM_3_T)|(isT(e2) ? cv::GEMM_3_T : 0),
                             e1.a, e1.b, alpha1, e2.a, alpha2);
    অন্যথা যদি( isMatProd(e2) && (i1 || isScaled(e1) || isT(e1)) )
        MatOp_GEMM::make_রাশিমালা(res, (e2.flags & ~cv::GEMM_3_T)|(isT(e1) ? cv::GEMM_3_T : 0),
                             e2.a, e2.b, alpha2, e1.a, alpha1);
    অন্যথা যদি( এতত্ == e2.op )
        MatOp::add(e1, e2, res);
    অন্যথা
        e2.op->add(e1, e2, res);
}

শূন্য MatOp_GEMM::subtract(ধ্রুব Mat_রাশিমালা& e1, ধ্রুব Mat_রাশিমালা& e2, Mat_রাশিমালা& res) ধ্রুব
{
    CV_INSTRUMENT_REGION();

    তার্কিক i1 = isIdentity(e1), i2 = isIdentity(e2);
    দ্বিগুণ alpha1 = i1 ? 1 : e1.alpha, alpha2 = i2 ? 1 : e2.alpha;

    যদি( isMatProd(e1) && (i2 || isScaled(e2) || isT(e2)) )
        MatOp_GEMM::make_রাশিমালা(res, (e1.flags & ~cv::GEMM_3_T)|(isT(e2) ? cv::GEMM_3_T : 0),
                             e1.a, e1.b, alpha1, e2.a, -alpha2);
    অন্যথা যদি( isMatProd(e2) && (i1 || isScaled(e1) || isT(e1)) )
        MatOp_GEMM::make_রাশিমালা(res, (e2.flags & ~cv::GEMM_3_T)|(isT(e1) ? cv::GEMM_3_T : 0),
                            e2.a, e2.b, -alpha2, e1.a, alpha1);
    অন্যথা যদি( এতত্ == e2.op )
        MatOp::subtract(e1, e2, res);
    অন্যথা
        e2.op->subtract(e1, e2, res);
}

শূন্য MatOp_GEMM::multiply(ধ্রুব Mat_রাশিমালা& e, দ্বিগুণ s, Mat_রাশিমালা& res) ধ্রুব
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha *= s;
    res.beta *= s;
}

শূন্য MatOp_GEMM::transpose(ধ্রুব Mat_রাশিমালা& e, Mat_রাশিমালা& res) ধ্রুব
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.flags = (!(e.flags & cv::GEMM_1_T) ? cv::GEMM_2_T : 0) |
                (!(e.flags & cv::GEMM_2_T) ? cv::GEMM_1_T : 0) |
                (!(e.flags & cv::GEMM_3_T) ? cv::GEMM_3_T : 0);
    swap(res.a, res.b);
}

অন্তঃস্থ শূন্য MatOp_GEMM::make_রাশিমালা(Mat_রাশিমালা& res, পূর্ণাঙ্ক flags, ধ্রুব Mat& a, ধ্রুব Mat& b,
                                 দ্বিগুণ alpha, ধ্রুব Mat& c, দ্বিগুণ beta)
{
    res = Mat_রাশিমালা(&g_MatOp_GEMM, flags, a, b, c, alpha, beta);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

শূন্য MatOp_Invert::assign(ধ্রুব Mat_রাশিমালা& e, Mat& m, পূর্ণাঙ্ক _type) ধ্রুব
{
    Mat temp, &dst = _type == -1 || _type == e.a.type() ? m : temp;

    cv::invert(e.a, dst, e.flags);
    যদি( dst.data != m.data )
        dst.convertTo(m, _type);
}

শূন্য MatOp_Invert::matmul(ধ্রুব Mat_রাশিমালা& e1, ধ্রুব Mat_রাশিমালা& e2, Mat_রাশিমালা& res) ধ্রুব
{
    যদি( isInv(e1) && isIdentity(e2) )
        MatOp_Solve::make_রাশিমালা(res, e1.flags, e1.a, e2.a);
    অন্যথা যদি( এতত্ == e2.op )
        MatOp::matmul(e1, e2, res);
    অন্যথা
        e2.op->matmul(e1, e2, res);
}

অন্তঃস্থ শূন্য MatOp_Invert::make_রাশিমালা(Mat_রাশিমালা& res, পূর্ণাঙ্ক method, ধ্রুব Mat& m)
{
    res = Mat_রাশিমালা(&g_MatOp_Invert, method, m, Mat(), Mat(), 1, 0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

শূন্য MatOp_Solve::assign(ধ্রুব Mat_রাশিমালা& e, Mat& m, পূর্ণাঙ্ক _type) ধ্রুব
{
    Mat temp, &dst = _type == -1 || _type == e.a.type() ? m : temp;

    cv::solve(e.a, e.b, dst, e.flags);
    যদি( dst.data != m.data )
        dst.convertTo(m, _type);
}

অন্তঃস্থ শূন্য MatOp_Solve::make_রাশিমালা(Mat_রাশিমালা& res, পূর্ণাঙ্ক method, ধ্রুব Mat& a, ধ্রুব Mat& b)
{
    res = Mat_রাশিমালা(&g_MatOp_Solve, method, a, b, Mat(), 1, 1);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

শূন্য MatOp_Initializer::assign(ধ্রুব Mat_রাশিমালা& e, Mat& m, পূর্ণাঙ্ক _type) ধ্রুব
{
    যদি( _type == -1 )
        _type = e.a.type();

    m.create(e.a.size, _type);

    যদি( e.flags == 'I' && e.a.dims <= 2 )
        setIdentity(m, Scalar(e.alpha));
    অন্যথা যদি( e.flags == '0' )
        m = Scalar();
    অন্যথা যদি( e.flags == '1' )
        m = Scalar(e.alpha);
    অন্যথা
        CV_Error(cv::Error::StsError, "Invalid matrix initializer type");
}

শূন্য MatOp_Initializer::multiply(ধ্রুব Mat_রাশিমালা& e, দ্বিগুণ s, Mat_রাশিমালা& res) ধ্রুব
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha *= s;
}

অন্তঃস্থ শূন্য MatOp_Initializer::make_রাশিমালা(Mat_রাশিমালা& res, পূর্ণাঙ্ক method, Size sz, পূর্ণাঙ্ক type, দ্বিগুণ alpha)
{
    res = Mat_রাশিমালা(getGlobalMatOpInitializer(), method, Mat(sz, type, (শূন্য*)(size_t)0xEEEEEEEE), Mat(), Mat(), alpha, 0);
}

অন্তঃস্থ শূন্য MatOp_Initializer::make_রাশিমালা(Mat_রাশিমালা& res, পূর্ণাঙ্ক method, পূর্ণাঙ্ক ndims, ধ্রুব পূর্ণাঙ্ক* sizes, পূর্ণাঙ্ক type, দ্বিগুণ alpha)
{
    res = Mat_রাশিমালা(getGlobalMatOpInitializer(), method, Mat(ndims, sizes, type, (শূন্য*)(size_t)0xEEEEEEEE), Mat(), Mat(), alpha, 0);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

Mat_রাশিমালা Mat::t() ধ্রুব
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(*এতত্);
    Mat_রাশিমালা e;
    MatOp_T::make_রাশিমালা(e, *এতত্);
    প্রত্যাগম e;
}

Mat_রাশিমালা Mat::inv(পূর্ণাঙ্ক method) ধ্রুব
{
    CV_INSTRUMENT_REGION();

    Mat_রাশিমালা e;
    MatOp_Invert::make_রাশিমালা(e, method, *এতত্);
    প্রত্যাগম e;
}


Mat_রাশিমালা Mat::mul(প্রবেশ_Array m, দ্বিগুণ scale) ধ্রুব
{
    CV_INSTRUMENT_REGION();

    Mat b = m.getMat();
    // Unless the argument is a refcounted Mat/UMat, the header returned by getMat() may be
    // a non-owning view of caller memory (e.g. a scalar bound to _InputArray(const double&),
    // a Matx or a Vec) that the returned MatExpr can outlive, so snapshot it.
    // See https://github.com/opencv/opencv/issues/23577
    যদি( !m.isMat() && !m.isUMat() )
        b = b.clone();

    Mat_রাশিমালা e;
    MatOp_Bin::make_রাশিমালা(e, '*', *এতত্, b, scale);
    প্রত্যাগম e;
}

Mat_রাশিমালা Mat::zeros(পূর্ণাঙ্ক rows, পূর্ণাঙ্ক cols, পূর্ণাঙ্ক type)
{
    CV_INSTRUMENT_REGION();

    Mat_রাশিমালা e;
    MatOp_Initializer::make_রাশিমালা(e, '0', Size(cols, rows), type);
    প্রত্যাগম e;
}

Mat_রাশিমালা Mat::zeros(Size size, পূর্ণাঙ্ক type)
{
    CV_INSTRUMENT_REGION();

    Mat_রাশিমালা e;
    MatOp_Initializer::make_রাশিমালা(e, '0', size, type);
    প্রত্যাগম e;
}

Mat_রাশিমালা Mat::zeros(পূর্ণাঙ্ক ndims, ধ্রুব পূর্ণাঙ্ক* sizes, পূর্ণাঙ্ক type)
{
    CV_INSTRUMENT_REGION();

    Mat_রাশিমালা e;
    MatOp_Initializer::make_রাশিমালা(e, '0', ndims, sizes, type);
    প্রত্যাগম e;
}

Mat_রাশিমালা Mat::zeros(ধ্রুব MatShape& shape, পূর্ণাঙ্ক type)
{
    CV_INSTRUMENT_REGION();

    Mat_রাশিমালা e;
    MatOp_Initializer::make_রাশিমালা(e, '0', shape.dims, shape.p, type);
    প্রত্যাগম e;
}

Mat_রাশিমালা Mat::ones(পূর্ণাঙ্ক rows, পূর্ণাঙ্ক cols, পূর্ণাঙ্ক type)
{
    CV_INSTRUMENT_REGION();

    Mat_রাশিমালা e;
    MatOp_Initializer::make_রাশিমালা(e, '1', Size(cols, rows), type);
    প্রত্যাগম e;
}

Mat_রাশিমালা Mat::ones(Size size, পূর্ণাঙ্ক type)
{
    CV_INSTRUMENT_REGION();

    Mat_রাশিমালা e;
    MatOp_Initializer::make_রাশিমালা(e, '1', size, type);
    প্রত্যাগম e;
}

Mat_রাশিমালা Mat::ones(পূর্ণাঙ্ক ndims, ধ্রুব পূর্ণাঙ্ক* sizes, পূর্ণাঙ্ক type)
{
    CV_INSTRUMENT_REGION();

    Mat_রাশিমালা e;
    MatOp_Initializer::make_রাশিমালা(e, '1', ndims, sizes, type);
    প্রত্যাগম e;
}

Mat_রাশিমালা Mat::ones(ধ্রুব MatShape& shape, পূর্ণাঙ্ক type)
{
    CV_INSTRUMENT_REGION();

    Mat_রাশিমালা e;
    MatOp_Initializer::make_রাশিমালা(e, '1', shape.dims, shape.p, type);
    প্রত্যাগম e;
}

Mat_রাশিমালা Mat::eye(পূর্ণাঙ্ক rows, পূর্ণাঙ্ক cols, পূর্ণাঙ্ক type)
{
    CV_INSTRUMENT_REGION();

    Mat_রাশিমালা e;
    MatOp_Initializer::make_রাশিমালা(e, 'I', Size(cols, rows), type);
    প্রত্যাগম e;
}

Mat_রাশিমালা Mat::eye(Size size, পূর্ণাঙ্ক type)
{
    CV_INSTRUMENT_REGION();

    Mat_রাশিমালা e;
    MatOp_Initializer::make_রাশিমালা(e, 'I', size, type);
    প্রত্যাগম e;
}

শূন্য Mat_রাশিমালা::swap(Mat_রাশিমালা& other)
{
    using std::swap;

    swap(op, other.op);
    swap(flags, other.flags);

    swap(a, other.a);
    swap(b, other.b);
    swap(c, other.c);

    swap(alpha, other.alpha);
    swap(beta, other.beta);

    swap(s, other.s);
}

প্রবেশ_Array_2::প্রবেশ_Array_2(ধ্রুব Mat_রাশিমালা& রাশিমালা)
{
    যদি (!isIdentity(রাশিমালা))
    {
        Mat result = রাশিমালা;  // TODO improve through refcount == 1 of expr.a (inplace operation is possible - except gemm?)
        Mat_রাশিমালা result_রাশিমালা(result);
        swap(const_cast<Mat_রাশিমালা&>(রাশিমালা), result_রাশিমালা);
    }
    CV_Assert(isIdentity(রাশিমালা));
    init(FIXED_TYPE + FIXED_SIZE + MAT + ACCESS_READ, &রাশিমালা.a);
}

} // cv::
