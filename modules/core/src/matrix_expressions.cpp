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
স্থির শূন্য checkOperandsExist(ধ্রুবক Mat& a)
{
    যদি (a.empty())
    {
        CV_Error(cv::Error::StsBadArg, "Matrix operand is an empty matrix.");
    }
}

স্থির শূন্য checkOperandsExist(ধ্রুবক Mat& a, ধ্রুবক Mat& b)
{
    যদি (a.empty() || b.empty())
    {
        CV_Error(cv::Error::StsBadArg, "One or more matrix operands are empty.");
    }
}


শ্রেণী MatOp_Identity CV_FINAL : সর্বজনীন MatOp
{
সর্বজনীন:
    MatOp_Identity() {}
    আভাসী ~MatOp_Identity() {}

    যৌক্তিক elementWise(ধ্রুবক Mat_রাশিমালা& /*রাশিমালা*/) ধ্রুবক CV_OVERRIDE { ফেরত সত্য; }
    শূন্য assign(ধ্রুবক Mat_রাশিমালা& রাশিমালা, Mat& m, পূর্ণসংখ্যা type=-1) ধ্রুবক CV_OVERRIDE;

    স্থির শূন্য make_রাশিমালা(Mat_রাশিমালা& res, ধ্রুবক Mat& m);
};

স্থির MatOp_Identity g_MatOp_Identity;

শ্রেণী MatOp_AddEx CV_FINAL : সর্বজনীন MatOp
{
সর্বজনীন:
    MatOp_AddEx() {}
    আভাসী ~MatOp_AddEx() {}

    যৌক্তিক elementWise(ধ্রুবক Mat_রাশিমালা& /*রাশিমালা*/) ধ্রুবক CV_OVERRIDE { ফেরত সত্য; }
    শূন্য assign(ধ্রুবক Mat_রাশিমালা& রাশিমালা, Mat& m, পূর্ণসংখ্যা type=-1) ধ্রুবক CV_OVERRIDE;

    শূন্য add(ধ্রুবক Mat_রাশিমালা& e1, ধ্রুবক Scalar& s, Mat_রাশিমালা& res) ধ্রুবক CV_OVERRIDE;
    শূন্য subtract(ধ্রুবক Scalar& s, ধ্রুবক Mat_রাশিমালা& রাশিমালা, Mat_রাশিমালা& res) ধ্রুবক CV_OVERRIDE;
    শূন্য multiply(ধ্রুবক Mat_রাশিমালা& e1, দ্বিগুণ s, Mat_রাশিমালা& res) ধ্রুবক CV_OVERRIDE;
    শূন্য divide(দ্বিগুণ s, ধ্রুবক Mat_রাশিমালা& e, Mat_রাশিমালা& res) ধ্রুবক CV_OVERRIDE;

    শূন্য transpose(ধ্রুবক Mat_রাশিমালা& e1, Mat_রাশিমালা& res) ধ্রুবক CV_OVERRIDE;
    শূন্য abs(ধ্রুবক Mat_রাশিমালা& রাশিমালা, Mat_রাশিমালা& res) ধ্রুবক CV_OVERRIDE;

    স্থির শূন্য make_রাশিমালা(Mat_রাশিমালা& res, ধ্রুবক Mat& a, ধ্রুবক Mat& b, দ্বিগুণ alpha, দ্বিগুণ beta, ধ্রুবক Scalar& s=Scalar());
};

স্থির MatOp_AddEx g_MatOp_AddEx;

শ্রেণী MatOp_Bin CV_FINAL : সর্বজনীন MatOp
{
সর্বজনীন:
    MatOp_Bin() {}
    আভাসী ~MatOp_Bin() {}

    যৌক্তিক elementWise(ধ্রুবক Mat_রাশিমালা& /*রাশিমালা*/) ধ্রুবক CV_OVERRIDE { ফেরত সত্য; }
    শূন্য assign(ধ্রুবক Mat_রাশিমালা& রাশিমালা, Mat& m, পূর্ণসংখ্যা type=-1) ধ্রুবক CV_OVERRIDE;

    শূন্য multiply(ধ্রুবক Mat_রাশিমালা& e1, দ্বিগুণ s, Mat_রাশিমালা& res) ধ্রুবক CV_OVERRIDE;
    শূন্য divide(দ্বিগুণ s, ধ্রুবক Mat_রাশিমালা& e, Mat_রাশিমালা& res) ধ্রুবক CV_OVERRIDE;

    স্থির শূন্য make_রাশিমালা(Mat_রাশিমালা& res, অক্ষর op, ধ্রুবক Mat& a, ধ্রুবক Mat& b, দ্বিগুণ scale=1);
    স্থির শূন্য make_রাশিমালা(Mat_রাশিমালা& res, অক্ষর op, ধ্রুবক Mat& a, ধ্রুবক Scalar& s);
};

স্থির MatOp_Bin g_MatOp_Bin;

শ্রেণী MatOp_Cmp CV_FINAL : সর্বজনীন MatOp
{
সর্বজনীন:
    MatOp_Cmp() {}
    আভাসী ~MatOp_Cmp() {}

    যৌক্তিক elementWise(ধ্রুবক Mat_রাশিমালা& /*রাশিমালা*/) ধ্রুবক CV_OVERRIDE { ফেরত সত্য; }
    শূন্য assign(ধ্রুবক Mat_রাশিমালা& রাশিমালা, Mat& m, পূর্ণসংখ্যা type=-1) ধ্রুবক CV_OVERRIDE;

    স্থির শূন্য make_রাশিমালা(Mat_রাশিমালা& res, পূর্ণসংখ্যা cmpop, ধ্রুবক Mat& a, ধ্রুবক Mat& b);
    স্থির শূন্য make_রাশিমালা(Mat_রাশিমালা& res, পূর্ণসংখ্যা cmpop, ধ্রুবক Mat& a, দ্বিগুণ alpha);
};

স্থির MatOp_Cmp g_MatOp_Cmp;

শ্রেণী MatOp_GEMM CV_FINAL : সর্বজনীন MatOp
{
সর্বজনীন:
    MatOp_GEMM() {}
    আভাসী ~MatOp_GEMM() {}

    যৌক্তিক elementWise(ধ্রুবক Mat_রাশিমালা& /*রাশিমালা*/) ধ্রুবক CV_OVERRIDE { ফেরত মিথ্যা; }
    শূন্য assign(ধ্রুবক Mat_রাশিমালা& রাশিমালা, Mat& m, পূর্ণসংখ্যা type=-1) ধ্রুবক CV_OVERRIDE;

    শূন্য add(ধ্রুবক Mat_রাশিমালা& e1, ধ্রুবক Mat_রাশিমালা& e2, Mat_রাশিমালা& res) ধ্রুবক CV_OVERRIDE;
    শূন্য subtract(ধ্রুবক Mat_রাশিমালা& e1, ধ্রুবক Mat_রাশিমালা& e2, Mat_রাশিমালা& res) ধ্রুবক CV_OVERRIDE;
    শূন্য multiply(ধ্রুবক Mat_রাশিমালা& e, দ্বিগুণ s, Mat_রাশিমালা& res) ধ্রুবক CV_OVERRIDE;

    শূন্য transpose(ধ্রুবক Mat_রাশিমালা& রাশিমালা, Mat_রাশিমালা& res) ধ্রুবক CV_OVERRIDE;

    আকার_2 size(ধ্রুবক Mat_রাশিমালা& রাশিমালা) ধ্রুবক CV_OVERRIDE
    {
        ফেরত আকার_2(
            (রাশিমালা.flags & GEMM_2_T) ? রাশিমালা.b.rows : রাশিমালা.b.cols,
            (রাশিমালা.flags & GEMM_1_T) ? রাশিমালা.a.cols : রাশিমালা.a.rows
        );
    }

    স্থির শূন্য make_রাশিমালা(Mat_রাশিমালা& res, পূর্ণসংখ্যা flags, ধ্রুবক Mat& a, ধ্রুবক Mat& b,
                         দ্বিগুণ alpha=1, ধ্রুবক Mat& c=Mat(), দ্বিগুণ beta=1);

};

স্থির MatOp_GEMM g_MatOp_GEMM;

শ্রেণী MatOp_Invert CV_FINAL : সর্বজনীন MatOp
{
সর্বজনীন:
    MatOp_Invert() {}
    আভাসী ~MatOp_Invert() {}

    যৌক্তিক elementWise(ধ্রুবক Mat_রাশিমালা& /*রাশিমালা*/) ধ্রুবক CV_OVERRIDE { ফেরত মিথ্যা; }
    শূন্য assign(ধ্রুবক Mat_রাশিমালা& রাশিমালা, Mat& m, পূর্ণসংখ্যা type=-1) ধ্রুবক CV_OVERRIDE;

    শূন্য matmul(ধ্রুবক Mat_রাশিমালা& expr1, ধ্রুবক Mat_রাশিমালা& expr2, Mat_রাশিমালা& res) ধ্রুবক CV_OVERRIDE;

    স্থির শূন্য make_রাশিমালা(Mat_রাশিমালা& res, পূর্ণসংখ্যা method, ধ্রুবক Mat& m);
};

স্থির MatOp_Invert g_MatOp_Invert;

শ্রেণী MatOp_T CV_FINAL : সর্বজনীন MatOp
{
সর্বজনীন:
    MatOp_T() {}
    আভাসী ~MatOp_T() {}

    যৌক্তিক elementWise(ধ্রুবক Mat_রাশিমালা& /*রাশিমালা*/) ধ্রুবক CV_OVERRIDE { ফেরত মিথ্যা; }
    শূন্য assign(ধ্রুবক Mat_রাশিমালা& রাশিমালা, Mat& m, পূর্ণসংখ্যা type=-1) ধ্রুবক CV_OVERRIDE;

    শূন্য multiply(ধ্রুবক Mat_রাশিমালা& e1, দ্বিগুণ s, Mat_রাশিমালা& res) ধ্রুবক CV_OVERRIDE;
    শূন্য transpose(ধ্রুবক Mat_রাশিমালা& রাশিমালা, Mat_রাশিমালা& res) ধ্রুবক CV_OVERRIDE;

    স্থির শূন্য make_রাশিমালা(Mat_রাশিমালা& res, ধ্রুবক Mat& a, দ্বিগুণ alpha=1);
};

স্থির MatOp_T g_MatOp_T;

শ্রেণী MatOp_Solve CV_FINAL : সর্বজনীন MatOp
{
সর্বজনীন:
    MatOp_Solve() {}
    আভাসী ~MatOp_Solve() {}

    যৌক্তিক elementWise(ধ্রুবক Mat_রাশিমালা& /*রাশিমালা*/) ধ্রুবক CV_OVERRIDE { ফেরত মিথ্যা; }
    শূন্য assign(ধ্রুবক Mat_রাশিমালা& রাশিমালা, Mat& m, পূর্ণসংখ্যা type=-1) ধ্রুবক CV_OVERRIDE;

    স্থির শূন্য make_রাশিমালা(Mat_রাশিমালা& res, পূর্ণসংখ্যা method, ধ্রুবক Mat& a, ধ্রুবক Mat& b);
};

স্থির MatOp_Solve g_MatOp_Solve;

শ্রেণী MatOp_Initializer CV_FINAL : সর্বজনীন MatOp
{
সর্বজনীন:
    MatOp_Initializer() {}
    আভাসী ~MatOp_Initializer() {}

    যৌক্তিক elementWise(ধ্রুবক Mat_রাশিমালা& /*রাশিমালা*/) ধ্রুবক CV_OVERRIDE { ফেরত মিথ্যা; }
    শূন্য assign(ধ্রুবক Mat_রাশিমালা& রাশিমালা, Mat& m, পূর্ণসংখ্যা type=-1) ধ্রুবক CV_OVERRIDE;

    শূন্য multiply(ধ্রুবক Mat_রাশিমালা& e, দ্বিগুণ s, Mat_রাশিমালা& res) ধ্রুবক CV_OVERRIDE;

    স্থির শূন্য make_রাশিমালা(Mat_রাশিমালা& res, পূর্ণসংখ্যা method, আকার_2 sz, পূর্ণসংখ্যা type, দ্বিগুণ alpha=1);
    স্থির শূন্য make_রাশিমালা(Mat_রাশিমালা& res, পূর্ণসংখ্যা method, পূর্ণসংখ্যা ndims, ধ্রুবক পূর্ণসংখ্যা* sizes, পূর্ণসংখ্যা type, দ্বিগুণ alpha=1);
};

স্থির MatOp_Initializer* getGlobalMatOpInitializer()
{
    CV_SINGLETON_LAZY_INIT(MatOp_Initializer, নতুন MatOp_Initializer())
}

স্থির অন্তঃরেখ যৌক্তিক isIdentity(ধ্রুবক Mat_রাশিমালা& e) { ফেরত e.op == &g_MatOp_Identity; }
স্থির অন্তঃরেখ যৌক্তিক isAddEx(ধ্রুবক Mat_রাশিমালা& e) { ফেরত e.op == &g_MatOp_AddEx; }
স্থির অন্তঃরেখ যৌক্তিক isScaled(ধ্রুবক Mat_রাশিমালা& e) { ফেরত isAddEx(e) && (!e.b.তথ্য || e.beta == 0) && e.s == Scalar(); }
স্থির অন্তঃরেখ যৌক্তিক isBin(ধ্রুবক Mat_রাশিমালা& e, অক্ষর c) { ফেরত e.op == &g_MatOp_Bin && e.flags == c; }
স্থির অন্তঃরেখ যৌক্তিক isCmp(ধ্রুবক Mat_রাশিমালা& e) { ফেরত e.op == &g_MatOp_Cmp; }
স্থির অন্তঃরেখ যৌক্তিক isReciprocal(ধ্রুবক Mat_রাশিমালা& e) { ফেরত isBin(e,'/') && (!e.b.তথ্য || e.beta == 0); }
স্থির অন্তঃরেখ যৌক্তিক isT(ধ্রুবক Mat_রাশিমালা& e) { ফেরত e.op == &g_MatOp_T; }
স্থির অন্তঃরেখ যৌক্তিক isInv(ধ্রুবক Mat_রাশিমালা& e) { ফেরত e.op == &g_MatOp_Invert; }
স্থির অন্তঃরেখ যৌক্তিক isSolve(ধ্রুবক Mat_রাশিমালা& e) { ফেরত e.op == &g_MatOp_Solve; }
//static inline bool isGEMM(const MatExpr& e) { return e.op == &g_MatOp_GEMM; }
স্থির অন্তঃরেখ যৌক্তিক isMatProd(ধ্রুবক Mat_রাশিমালা& e) { ফেরত e.op == &g_MatOp_GEMM && (!e.c.তথ্য || e.beta == 0); }
স্থির অন্তঃরেখ যৌক্তিক isInitializer(ধ্রুবক Mat_রাশিমালা& e) { ফেরত e.op == getGlobalMatOpInitializer(); }

/////////////////////////////////////////////////////////////////////////////////////////////////////

MatOp::MatOp() {}
MatOp::~MatOp() {}


যৌক্তিক MatOp::elementWise(ধ্রুবক Mat_রাশিমালা& /*রাশিমালা*/) ধ্রুবক
{
    ফেরত মিথ্যা;
}

শূন্য MatOp::roi(ধ্রুবক Mat_রাশিমালা& রাশিমালা, ধ্রুবক Range& rowRange, ধ্রুবক Range& colRange, Mat_রাশিমালা& e) ধ্রুবক
{
    যদি( elementWise(রাশিমালা) )
    {
        e = Mat_রাশিমালা(রাশিমালা.op, রাশিমালা.flags, Mat(), Mat(), Mat(),
                    রাশিমালা.alpha, রাশিমালা.beta, রাশিমালা.s);
        যদি(রাশিমালা.a.তথ্য)
            e.a = রাশিমালা.a(rowRange, colRange);
        যদি(রাশিমালা.b.তথ্য)
            e.b = রাশিমালা.b(rowRange, colRange);
        যদি(রাশিমালা.c.তথ্য)
            e.c = রাশিমালা.c(rowRange, colRange);
    }
    অন্যথা
    {
        Mat m;
        রাশিমালা.op->assign(রাশিমালা, m);
        e = Mat_রাশিমালা(&g_MatOp_Identity, 0, m(rowRange, colRange), Mat(), Mat());
    }
}

শূন্য MatOp::diag(ধ্রুবক Mat_রাশিমালা& রাশিমালা, পূর্ণসংখ্যা d, Mat_রাশিমালা& e) ধ্রুবক
{
    যদি( elementWise(রাশিমালা) )
    {
        e = Mat_রাশিমালা(রাশিমালা.op, রাশিমালা.flags, Mat(), Mat(), Mat(),
                    রাশিমালা.alpha, রাশিমালা.beta, রাশিমালা.s);
        যদি(রাশিমালা.a.তথ্য)
            e.a = রাশিমালা.a.diag(d);
        যদি(রাশিমালা.b.তথ্য)
            e.b = রাশিমালা.b.diag(d);
        যদি(রাশিমালা.c.তথ্য)
            e.c = রাশিমালা.c.diag(d);
    }
    অন্যথা
    {
        Mat m;
        রাশিমালা.op->assign(রাশিমালা, m);
        e = Mat_রাশিমালা(&g_MatOp_Identity, 0, m.diag(d), Mat(), Mat());
    }
}


শূন্য MatOp::augAssignAdd(ধ্রুবক Mat_রাশিমালা& রাশিমালা, Mat& m) ধ্রুবক
{
    Mat অস্থায়ী_2;
    রাশিমালা.op->assign(রাশিমালা, অস্থায়ী_2);
    m += অস্থায়ী_2;
}


শূন্য MatOp::augAssignSubtract(ধ্রুবক Mat_রাশিমালা& রাশিমালা, Mat& m) ধ্রুবক
{
    Mat অস্থায়ী_2;
    রাশিমালা.op->assign(রাশিমালা, অস্থায়ী_2);
    m -= অস্থায়ী_2;
}


শূন্য MatOp::augAssignMultiply(ধ্রুবক Mat_রাশিমালা& রাশিমালা, Mat& m) ধ্রুবক
{
    Mat অস্থায়ী_2;
    রাশিমালা.op->assign(রাশিমালা, অস্থায়ী_2);
    m *= অস্থায়ী_2;
}


শূন্য MatOp::augAssignDivide(ধ্রুবক Mat_রাশিমালা& রাশিমালা, Mat& m) ধ্রুবক
{
    Mat অস্থায়ী_2;
    রাশিমালা.op->assign(রাশিমালা, অস্থায়ী_2);
    m /= অস্থায়ী_2;
}


শূন্য MatOp::augAssignAnd(ধ্রুবক Mat_রাশিমালা& রাশিমালা, Mat& m) ধ্রুবক
{
    Mat অস্থায়ী_2;
    রাশিমালা.op->assign(রাশিমালা, অস্থায়ী_2);
    m &= অস্থায়ী_2;
}


শূন্য MatOp::augAssignOr(ধ্রুবক Mat_রাশিমালা& রাশিমালা, Mat& m) ধ্রুবক
{
    Mat অস্থায়ী_2;
    রাশিমালা.op->assign(রাশিমালা, অস্থায়ী_2);
    m |= অস্থায়ী_2;
}


শূন্য MatOp::augAssignXor(ধ্রুবক Mat_রাশিমালা& রাশিমালা, Mat& m) ধ্রুবক
{
    Mat অস্থায়ী_2;
    রাশিমালা.op->assign(রাশিমালা, অস্থায়ী_2);
    m ^= অস্থায়ী_2;
}


শূন্য MatOp::add(ধ্রুবক Mat_রাশিমালা& e1, ধ্রুবক Mat_রাশিমালা& e2, Mat_রাশিমালা& res) ধ্রুবক
{
    CV_INSTRUMENT_REGION();

    যদি( এই == e2.op )
    {
        দ্বিগুণ alpha = 1, beta = 1;
        Scalar s;
        Mat m1, m2;
        যদি( isAddEx(e1) && (!e1.b.তথ্য || e1.beta == 0) )
        {
            m1 = e1.a;
            alpha = e1.alpha;
            s = e1.s;
        }
        অন্যথা
            e1.op->assign(e1, m1);

        যদি( isAddEx(e2) && (!e2.b.তথ্য || e2.beta == 0) )
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


শূন্য MatOp::add(ধ্রুবক Mat_রাশিমালা& expr1, ধ্রুবক Scalar& s, Mat_রাশিমালা& res) ধ্রুবক
{
    CV_INSTRUMENT_REGION();

    Mat m1;
    expr1.op->assign(expr1, m1);
    MatOp_AddEx::make_রাশিমালা(res, m1, Mat(), 1, 0, s);
}


শূন্য MatOp::subtract(ধ্রুবক Mat_রাশিমালা& e1, ধ্রুবক Mat_রাশিমালা& e2, Mat_রাশিমালা& res) ধ্রুবক
{
    CV_INSTRUMENT_REGION();

    যদি( এই == e2.op )
    {
        দ্বিগুণ alpha = 1, beta = -1;
        Scalar s;
        Mat m1, m2;
        যদি( isAddEx(e1) && (!e1.b.তথ্য || e1.beta == 0) )
        {
            m1 = e1.a;
            alpha = e1.alpha;
            s = e1.s;
        }
        অন্যথা
            e1.op->assign(e1, m1);

        যদি( isAddEx(e2) && (!e2.b.তথ্য || e2.beta == 0) )
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


শূন্য MatOp::subtract(ধ্রুবক Scalar& s, ধ্রুবক Mat_রাশিমালা& রাশিমালা, Mat_রাশিমালা& res) ধ্রুবক
{
    CV_INSTRUMENT_REGION();

    Mat m;
    রাশিমালা.op->assign(রাশিমালা, m);
    MatOp_AddEx::make_রাশিমালা(res, m, Mat(), -1, 0, s);
}


শূন্য MatOp::multiply(ধ্রুবক Mat_রাশিমালা& e1, ধ্রুবক Mat_রাশিমালা& e2, Mat_রাশিমালা& res, দ্বিগুণ scale) ধ্রুবক
{
    CV_INSTRUMENT_REGION();

    যদি( এই == e2.op )
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
            অক্ষর op = '*';
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


শূন্য MatOp::multiply(ধ্রুবক Mat_রাশিমালা& রাশিমালা, দ্বিগুণ s, Mat_রাশিমালা& res) ধ্রুবক
{
    CV_INSTRUMENT_REGION();

    Mat m;
    রাশিমালা.op->assign(রাশিমালা, m);
    MatOp_AddEx::make_রাশিমালা(res, m, Mat(), s, 0);
}


শূন্য MatOp::divide(ধ্রুবক Mat_রাশিমালা& e1, ধ্রুবক Mat_রাশিমালা& e2, Mat_রাশিমালা& res, দ্বিগুণ scale) ধ্রুবক
{
    CV_INSTRUMENT_REGION();

    যদি( এই == e2.op )
    {
        যদি( isReciprocal(e1) && isReciprocal(e2) )
            MatOp_Bin::make_রাশিমালা(res, '/', e2.a, e1.a, e1.alpha/e2.alpha);
        অন্যথা
        {
            Mat m1, m2;
            অক্ষর op = '/';

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


শূন্য MatOp::divide(দ্বিগুণ s, ধ্রুবক Mat_রাশিমালা& রাশিমালা, Mat_রাশিমালা& res) ধ্রুবক
{
    CV_INSTRUMENT_REGION();

    Mat m;
    রাশিমালা.op->assign(রাশিমালা, m);
    MatOp_Bin::make_রাশিমালা(res, '/', m, Mat(), s);
}


শূন্য MatOp::abs(ধ্রুবক Mat_রাশিমালা& রাশিমালা, Mat_রাশিমালা& res) ধ্রুবক
{
    CV_INSTRUMENT_REGION();

    Mat m;
    রাশিমালা.op->assign(রাশিমালা, m);
    MatOp_Bin::make_রাশিমালা(res, 'a', m, Mat());
}


শূন্য MatOp::transpose(ধ্রুবক Mat_রাশিমালা& রাশিমালা, Mat_রাশিমালা& res) ধ্রুবক
{
    CV_INSTRUMENT_REGION();

    Mat m;
    রাশিমালা.op->assign(রাশিমালা, m);
    MatOp_T::make_রাশিমালা(res, m, 1);
}


শূন্য MatOp::matmul(ধ্রুবক Mat_রাশিমালা& e1, ধ্রুবক Mat_রাশিমালা& e2, Mat_রাশিমালা& res) ধ্রুবক
{
    যদি( এই == e2.op )
    {
        দ্বিগুণ scale = 1;
        পূর্ণসংখ্যা flags = 0;
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


শূন্য MatOp::invert(ধ্রুবক Mat_রাশিমালা& রাশিমালা, পূর্ণসংখ্যা method, Mat_রাশিমালা& res) ধ্রুবক
{
    Mat m;
    রাশিমালা.op->assign(রাশিমালা, m);
    MatOp_Invert::make_রাশিমালা(res, method, m);
}


আকার_2 MatOp::size(ধ্রুবক Mat_রাশিমালা& রাশিমালা) ধ্রুবক
{
    ফেরত !রাশিমালা.a.empty() ? রাশিমালা.a.size() : রাশিমালা.b.empty() ? রাশিমালা.b.size() : রাশিমালা.c.size();
}

পূর্ণসংখ্যা MatOp::type(ধ্রুবক Mat_রাশিমালা& রাশিমালা) ধ্রুবক
{
    CV_INSTRUMENT_REGION();

    ফেরত !রাশিমালা.a.empty() ? রাশিমালা.a.type() : রাশিমালা.b.empty() ? রাশিমালা.b.type() : রাশিমালা.c.type();
}

//////////////////////////////////////////////////////////////////////////////////////////////////

Mat_রাশিমালা::Mat_রাশিমালা(ধ্রুবক Mat& m) : op(&g_MatOp_Identity), flags(0), a(m), b(Mat()), c(Mat()), alpha(1), beta(0), s(Scalar())
{
}

Mat_রাশিমালা Mat_রাশিমালা::row(পূর্ণসংখ্যা y) ধ্রুবক
{
    Mat_রাশিমালা e;
    op->roi(*এই, Range(y, y+1), Range::all(), e);
    ফেরত e;
}

Mat_রাশিমালা Mat_রাশিমালা::col(পূর্ণসংখ্যা x) ধ্রুবক
{
    Mat_রাশিমালা e;
    op->roi(*এই, Range::all(), Range(x, x+1), e);
    ফেরত e;
}

Mat_রাশিমালা Mat_রাশিমালা::diag(পূর্ণসংখ্যা d) ধ্রুবক
{
    Mat_রাশিমালা e;
    op->diag(*এই, d, e);
    ফেরত e;
}

Mat_রাশিমালা Mat_রাশিমালা::operator()( ধ্রুবক Range& rowRange, ধ্রুবক Range& colRange ) ধ্রুবক
{
    Mat_রাশিমালা e;
    op->roi(*এই, rowRange, colRange, e);
    ফেরত e;
}

Mat_রাশিমালা Mat_রাশিমালা::operator()( ধ্রুবক Rect& roi ) ধ্রুবক
{
    Mat_রাশিমালা e;
    op->roi(*এই, Range(roi.y, roi.y + roi.height), Range(roi.x, roi.x + roi.width), e);
    ফেরত e;
}

Mat Mat_রাশিমালা::cross(ধ্রুবক Mat& m) ধ্রুবক
{
    ফেরত ((Mat)*এই).cross(m);
}

দ্বিগুণ Mat_রাশিমালা::dot(ধ্রুবক Mat& m) ধ্রুবক
{
    ফেরত ((Mat)*এই).dot(m);
}

Mat_রাশিমালা Mat_রাশিমালা::t() ধ্রুবক
{
    Mat_রাশিমালা e;
    op->transpose(*এই, e);
    ফেরত e;
}

Mat_রাশিমালা Mat_রাশিমালা::inv(পূর্ণসংখ্যা method) ধ্রুবক
{
    Mat_রাশিমালা e;
    op->invert(*এই, method, e);
    ফেরত e;
}

Mat_রাশিমালা Mat_রাশিমালা::mul(ধ্রুবক Mat_রাশিমালা& e, দ্বিগুণ scale) ধ্রুবক
{
    Mat_রাশিমালা en;
    op->multiply(*এই, e, en, scale);
    ফেরত en;
}

Mat_রাশিমালা Mat_রাশিমালা::mul(ধ্রুবক Mat& m, দ্বিগুণ scale) ধ্রুবক
{
    Mat_রাশিমালা e;
    op->multiply(*এই, Mat_রাশিমালা(m), e, scale);
    ফেরত e;
}

Mat_রাশিমালা operator + (ধ্রুবক Mat& a, ধ্রুবক Mat& b)
{
    checkOperandsExist(a, b);
    Mat_রাশিমালা e;
    MatOp_AddEx::make_রাশিমালা(e, a, b, 1, 1);
    ফেরত e;
}

Mat_রাশিমালা operator + (ধ্রুবক Mat& a, ধ্রুবক Scalar& s)
{
    checkOperandsExist(a);
    Mat_রাশিমালা e;
    MatOp_AddEx::make_রাশিমালা(e, a, Mat(), 1, 0, s);
    ফেরত e;
}

Mat_রাশিমালা operator + (ধ্রুবক Scalar& s, ধ্রুবক Mat& a)
{
    checkOperandsExist(a);
    Mat_রাশিমালা e;
    MatOp_AddEx::make_রাশিমালা(e, a, Mat(), 1, 0, s);
    ফেরত e;
}

Mat_রাশিমালা operator + (ধ্রুবক Mat_রাশিমালা& e, ধ্রুবক Mat& m)
{
    Mat_রাশিমালা en;
    e.op->add(e, Mat_রাশিমালা(m), en);
    ফেরত en;
}

Mat_রাশিমালা operator + (ধ্রুবক Mat& m, ধ্রুবক Mat_রাশিমালা& e)
{
    checkOperandsExist(m);
    Mat_রাশিমালা en;
    e.op->add(e, Mat_রাশিমালা(m), en);
    ফেরত en;
}

Mat_রাশিমালা operator + (ধ্রুবক Mat_রাশিমালা& e, ধ্রুবক Scalar& s)
{
    Mat_রাশিমালা en;
    e.op->add(e, s, en);
    ফেরত en;
}

Mat_রাশিমালা operator + (ধ্রুবক Scalar& s, ধ্রুবক Mat_রাশিমালা& e)
{
    Mat_রাশিমালা en;
    e.op->add(e, s, en);
    ফেরত en;
}

Mat_রাশিমালা operator + (ধ্রুবক Mat_রাশিমালা& e1, ধ্রুবক Mat_রাশিমালা& e2)
{
    Mat_রাশিমালা en;
    e1.op->add(e1, e2, en);
    ফেরত en;
}

Mat_রাশিমালা operator - (ধ্রুবক Mat& a, ধ্রুবক Mat& b)
{
    checkOperandsExist(a, b);
    Mat_রাশিমালা e;
    MatOp_AddEx::make_রাশিমালা(e, a, b, 1, -1);
    ফেরত e;
}

Mat_রাশিমালা operator - (ধ্রুবক Mat& a, ধ্রুবক Scalar& s)
{
    checkOperandsExist(a);
    Mat_রাশিমালা e;
    MatOp_AddEx::make_রাশিমালা(e, a, Mat(), 1, 0, -s);
    ফেরত e;
}

Mat_রাশিমালা operator - (ধ্রুবক Scalar& s, ধ্রুবক Mat& a)
{
    checkOperandsExist(a);
    Mat_রাশিমালা e;
    MatOp_AddEx::make_রাশিমালা(e, a, Mat(), -1, 0, s);
    ফেরত e;
}

Mat_রাশিমালা operator - (ধ্রুবক Mat_রাশিমালা& e, ধ্রুবক Mat& m)
{
    checkOperandsExist(m);
    Mat_রাশিমালা en;
    e.op->subtract(e, Mat_রাশিমালা(m), en);
    ফেরত en;
}

Mat_রাশিমালা operator - (ধ্রুবক Mat& m, ধ্রুবক Mat_রাশিমালা& e)
{
    checkOperandsExist(m);
    Mat_রাশিমালা en;
    e.op->subtract(Mat_রাশিমালা(m), e, en);
    ফেরত en;
}

Mat_রাশিমালা operator - (ধ্রুবক Mat_রাশিমালা& e, ধ্রুবক Scalar& s)
{
    Mat_রাশিমালা en;
    e.op->add(e, -s, en);
    ফেরত en;
}

Mat_রাশিমালা operator - (ধ্রুবক Scalar& s, ধ্রুবক Mat_রাশিমালা& e)
{
    Mat_রাশিমালা en;
    e.op->subtract(s, e, en);
    ফেরত en;
}

Mat_রাশিমালা operator - (ধ্রুবক Mat_রাশিমালা& e1, ধ্রুবক Mat_রাশিমালা& e2)
{
    Mat_রাশিমালা en;
    e1.op->subtract(e1, e2, en);
    ফেরত en;
}

Mat_রাশিমালা operator - (ধ্রুবক Mat& m)
{
    checkOperandsExist(m);
    Mat_রাশিমালা e;
    MatOp_AddEx::make_রাশিমালা(e, m, Mat(), -1, 0);
    ফেরত e;
}

Mat_রাশিমালা operator - (ধ্রুবক Mat_রাশিমালা& e)
{
    Mat_রাশিমালা en;
    e.op->subtract(Scalar(0), e, en);
    ফেরত en;
}

Mat_রাশিমালা operator * (ধ্রুবক Mat& a, ধ্রুবক Mat& b)
{
    checkOperandsExist(a, b);
    Mat_রাশিমালা e;
    MatOp_GEMM::make_রাশিমালা(e, 0, a, b);
    ফেরত e;
}

Mat_রাশিমালা operator * (ধ্রুবক Mat& a, দ্বিগুণ s)
{
    checkOperandsExist(a);
    Mat_রাশিমালা e;
    MatOp_AddEx::make_রাশিমালা(e, a, Mat(), s, 0);
    ফেরত e;
}

Mat_রাশিমালা operator * (দ্বিগুণ s, ধ্রুবক Mat& a)
{
    checkOperandsExist(a);
    Mat_রাশিমালা e;
    MatOp_AddEx::make_রাশিমালা(e, a, Mat(), s, 0);
    ফেরত e;
}

Mat_রাশিমালা operator * (ধ্রুবক Mat_রাশিমালা& e, ধ্রুবক Mat& m)
{
    Mat_রাশিমালা en;
    e.op->matmul(e, Mat_রাশিমালা(m), en);
    ফেরত en;
}

Mat_রাশিমালা operator * (ধ্রুবক Mat& m, ধ্রুবক Mat_রাশিমালা& e)
{
    checkOperandsExist(m);
    Mat_রাশিমালা en;
    e.op->matmul(Mat_রাশিমালা(m), e, en);
    ফেরত en;
}

Mat_রাশিমালা operator * (ধ্রুবক Mat_রাশিমালা& e, দ্বিগুণ s)
{
    Mat_রাশিমালা en;
    e.op->multiply(e, s, en);
    ফেরত en;
}

Mat_রাশিমালা operator * (দ্বিগুণ s, ধ্রুবক Mat_রাশিমালা& e)
{
    Mat_রাশিমালা en;
    e.op->multiply(e, s, en);
    ফেরত en;
}

Mat_রাশিমালা operator * (ধ্রুবক Mat_রাশিমালা& e1, ধ্রুবক Mat_রাশিমালা& e2)
{
    Mat_রাশিমালা en;
    e1.op->matmul(e1, e2, en);
    ফেরত en;
}

Mat_রাশিমালা operator / (ধ্রুবক Mat& a, ধ্রুবক Mat& b)
{
    checkOperandsExist(a, b);
    Mat_রাশিমালা e;
    MatOp_Bin::make_রাশিমালা(e, '/', a, b);
    ফেরত e;
}

Mat_রাশিমালা operator / (ধ্রুবক Mat& a, দ্বিগুণ s)
{
    checkOperandsExist(a);
    Mat_রাশিমালা e;
    MatOp_AddEx::make_রাশিমালা(e, a, Mat(), 1./s, 0);
    ফেরত e;
}

Mat_রাশিমালা operator / (দ্বিগুণ s, ধ্রুবক Mat& a)
{
    checkOperandsExist(a);
    Mat_রাশিমালা e;
    MatOp_Bin::make_রাশিমালা(e, '/', a, Mat(), s);
    ফেরত e;
}

Mat_রাশিমালা operator / (ধ্রুবক Mat_রাশিমালা& e, ধ্রুবক Mat& m)
{
    Mat_রাশিমালা en;
    e.op->divide(e, Mat_রাশিমালা(m), en);
    ফেরত en;
}

Mat_রাশিমালা operator / (ধ্রুবক Mat& m, ধ্রুবক Mat_রাশিমালা& e)
{
    checkOperandsExist(m);
    Mat_রাশিমালা en;
    e.op->divide(Mat_রাশিমালা(m), e, en);
    ফেরত en;
}

Mat_রাশিমালা operator / (ধ্রুবক Mat_রাশিমালা& e, দ্বিগুণ s)
{
    Mat_রাশিমালা en;
    e.op->multiply(e, 1./s, en);
    ফেরত en;
}

Mat_রাশিমালা operator / (দ্বিগুণ s, ধ্রুবক Mat_রাশিমালা& e)
{
    Mat_রাশিমালা en;
    e.op->divide(s, e, en);
    ফেরত en;
}

Mat_রাশিমালা operator / (ধ্রুবক Mat_রাশিমালা& e1, ধ্রুবক Mat_রাশিমালা& e2)
{
    Mat_রাশিমালা en;
    e1.op->divide(e1, e2, en);
    ফেরত en;
}

Mat_রাশিমালা operator < (ধ্রুবক Mat& a, ধ্রুবক Mat& b)
{
    checkOperandsExist(a, b);
    Mat_রাশিমালা e;
    MatOp_Cmp::make_রাশিমালা(e, cv::CMP_LT, a, b);
    ফেরত e;
}

Mat_রাশিমালা operator < (ধ্রুবক Mat& a, দ্বিগুণ s)
{
    checkOperandsExist(a);
    Mat_রাশিমালা e;
    MatOp_Cmp::make_রাশিমালা(e, cv::CMP_LT, a, s);
    ফেরত e;
}

Mat_রাশিমালা operator < (দ্বিগুণ s, ধ্রুবক Mat& a)
{
    checkOperandsExist(a);
    Mat_রাশিমালা e;
    MatOp_Cmp::make_রাশিমালা(e, cv::CMP_GT, a, s);
    ফেরত e;
}

Mat_রাশিমালা operator <= (ধ্রুবক Mat& a, ধ্রুবক Mat& b)
{
    checkOperandsExist(a, b);
    Mat_রাশিমালা e;
    MatOp_Cmp::make_রাশিমালা(e, cv::CMP_LE, a, b);
    ফেরত e;
}

Mat_রাশিমালা operator <= (ধ্রুবক Mat& a, দ্বিগুণ s)
{
    checkOperandsExist(a);
    Mat_রাশিমালা e;
    MatOp_Cmp::make_রাশিমালা(e, cv::CMP_LE, a, s);
    ফেরত e;
}

Mat_রাশিমালা operator <= (দ্বিগুণ s, ধ্রুবক Mat& a)
{
    checkOperandsExist(a);
    Mat_রাশিমালা e;
    MatOp_Cmp::make_রাশিমালা(e, cv::CMP_GE, a, s);
    ফেরত e;
}

Mat_রাশিমালা operator == (ধ্রুবক Mat& a, ধ্রুবক Mat& b)
{
    checkOperandsExist(a, b);
    Mat_রাশিমালা e;
    MatOp_Cmp::make_রাশিমালা(e, cv::CMP_EQ, a, b);
    ফেরত e;
}

Mat_রাশিমালা operator == (ধ্রুবক Mat& a, দ্বিগুণ s)
{
    checkOperandsExist(a);
    Mat_রাশিমালা e;
    MatOp_Cmp::make_রাশিমালা(e, cv::CMP_EQ, a, s);
    ফেরত e;
}

Mat_রাশিমালা operator == (দ্বিগুণ s, ধ্রুবক Mat& a)
{
    checkOperandsExist(a);
    Mat_রাশিমালা e;
    MatOp_Cmp::make_রাশিমালা(e, cv::CMP_EQ, a, s);
    ফেরত e;
}

Mat_রাশিমালা operator != (ধ্রুবক Mat& a, ধ্রুবক Mat& b)
{
    checkOperandsExist(a, b);
    Mat_রাশিমালা e;
    MatOp_Cmp::make_রাশিমালা(e, cv::CMP_NE, a, b);
    ফেরত e;
}

Mat_রাশিমালা operator != (ধ্রুবক Mat& a, দ্বিগুণ s)
{
    checkOperandsExist(a);
    Mat_রাশিমালা e;
    MatOp_Cmp::make_রাশিমালা(e, cv::CMP_NE, a, s);
    ফেরত e;
}

Mat_রাশিমালা operator != (দ্বিগুণ s, ধ্রুবক Mat& a)
{
    checkOperandsExist(a);
    Mat_রাশিমালা e;
    MatOp_Cmp::make_রাশিমালা(e, cv::CMP_NE, a, s);
    ফেরত e;
}

Mat_রাশিমালা operator >= (ধ্রুবক Mat& a, ধ্রুবক Mat& b)
{
    checkOperandsExist(a, b);
    Mat_রাশিমালা e;
    MatOp_Cmp::make_রাশিমালা(e, cv::CMP_GE, a, b);
    ফেরত e;
}

Mat_রাশিমালা operator >= (ধ্রুবক Mat& a, দ্বিগুণ s)
{
    checkOperandsExist(a);
    Mat_রাশিমালা e;
    MatOp_Cmp::make_রাশিমালা(e, cv::CMP_GE, a, s);
    ফেরত e;
}

Mat_রাশিমালা operator >= (দ্বিগুণ s, ধ্রুবক Mat& a)
{
    checkOperandsExist(a);
    Mat_রাশিমালা e;
    MatOp_Cmp::make_রাশিমালা(e, cv::CMP_LE, a, s);
    ফেরত e;
}

Mat_রাশিমালা operator > (ধ্রুবক Mat& a, ধ্রুবক Mat& b)
{
    checkOperandsExist(a, b);
    Mat_রাশিমালা e;
    MatOp_Cmp::make_রাশিমালা(e, cv::CMP_GT, a, b);
    ফেরত e;
}

Mat_রাশিমালা operator > (ধ্রুবক Mat& a, দ্বিগুণ s)
{
    checkOperandsExist(a);
    Mat_রাশিমালা e;
    MatOp_Cmp::make_রাশিমালা(e, cv::CMP_GT, a, s);
    ফেরত e;
}

Mat_রাশিমালা operator > (দ্বিগুণ s, ধ্রুবক Mat& a)
{
    checkOperandsExist(a);
    Mat_রাশিমালা e;
    MatOp_Cmp::make_রাশিমালা(e, cv::CMP_LT, a, s);
    ফেরত e;
}

Mat_রাশিমালা min(ধ্রুবক Mat& a, ধ্রুবক Mat& b)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a, b);
    Mat_রাশিমালা e;
    MatOp_Bin::make_রাশিমালা(e, 'm', a, b);
    ফেরত e;
}

Mat_রাশিমালা min(ধ্রুবক Mat& a, দ্বিগুণ s)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    Mat_রাশিমালা e;
    MatOp_Bin::make_রাশিমালা(e, 'n', a, s);
    ফেরত e;
}

Mat_রাশিমালা min(দ্বিগুণ s, ধ্রুবক Mat& a)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    Mat_রাশিমালা e;
    MatOp_Bin::make_রাশিমালা(e, 'n', a, s);
    ফেরত e;
}

Mat_রাশিমালা max(ধ্রুবক Mat& a, ধ্রুবক Mat& b)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a, b);
    Mat_রাশিমালা e;
    MatOp_Bin::make_রাশিমালা(e, 'M', a, b);
    ফেরত e;
}

Mat_রাশিমালা max(ধ্রুবক Mat& a, দ্বিগুণ s)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    Mat_রাশিমালা e;
    MatOp_Bin::make_রাশিমালা(e, 'N', a, s);
    ফেরত e;
}

Mat_রাশিমালা max(দ্বিগুণ s, ধ্রুবক Mat& a)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    Mat_রাশিমালা e;
    MatOp_Bin::make_রাশিমালা(e, 'N', a, s);
    ফেরত e;
}

Mat_রাশিমালা operator & (ধ্রুবক Mat& a, ধ্রুবক Mat& b)
{
    checkOperandsExist(a, b);
    Mat_রাশিমালা e;
    MatOp_Bin::make_রাশিমালা(e, '&', a, b);
    ফেরত e;
}

Mat_রাশিমালা operator & (ধ্রুবক Mat& a, ধ্রুবক Scalar& s)
{
    checkOperandsExist(a);
    Mat_রাশিমালা e;
    MatOp_Bin::make_রাশিমালা(e, '&', a, s);
    ফেরত e;
}

Mat_রাশিমালা operator & (ধ্রুবক Scalar& s, ধ্রুবক Mat& a)
{
    checkOperandsExist(a);
    Mat_রাশিমালা e;
    MatOp_Bin::make_রাশিমালা(e, '&', a, s);
    ফেরত e;
}

Mat_রাশিমালা operator | (ধ্রুবক Mat& a, ধ্রুবক Mat& b)
{
    checkOperandsExist(a, b);
    Mat_রাশিমালা e;
    MatOp_Bin::make_রাশিমালা(e, '|', a, b);
    ফেরত e;
}

Mat_রাশিমালা operator | (ধ্রুবক Mat& a, ধ্রুবক Scalar& s)
{
    checkOperandsExist(a);
    Mat_রাশিমালা e;
    MatOp_Bin::make_রাশিমালা(e, '|', a, s);
    ফেরত e;
}

Mat_রাশিমালা operator | (ধ্রুবক Scalar& s, ধ্রুবক Mat& a)
{
    checkOperandsExist(a);
    Mat_রাশিমালা e;
    MatOp_Bin::make_রাশিমালা(e, '|', a, s);
    ফেরত e;
}

Mat_রাশিমালা operator ^ (ধ্রুবক Mat& a, ধ্রুবক Mat& b)
{
    checkOperandsExist(a, b);
    Mat_রাশিমালা e;
    MatOp_Bin::make_রাশিমালা(e, '^', a, b);
    ফেরত e;
}

Mat_রাশিমালা operator ^ (ধ্রুবক Mat& a, ধ্রুবক Scalar& s)
{
    checkOperandsExist(a);
    Mat_রাশিমালা e;
    MatOp_Bin::make_রাশিমালা(e, '^', a, s);
    ফেরত e;
}

Mat_রাশিমালা operator ^ (ধ্রুবক Scalar& s, ধ্রুবক Mat& a)
{
    checkOperandsExist(a);
    Mat_রাশিমালা e;
    MatOp_Bin::make_রাশিমালা(e, '^', a, s);
    ফেরত e;
}

Mat_রাশিমালা operator ~(ধ্রুবক Mat& a)
{
    checkOperandsExist(a);
    Mat_রাশিমালা e;
    MatOp_Bin::make_রাশিমালা(e, '~', a, Scalar());
    ফেরত e;
}

Mat_রাশিমালা abs(ধ্রুবক Mat& a)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    Mat_রাশিমালা e;
    MatOp_Bin::make_রাশিমালা(e, 'a', a, Scalar());
    ফেরত e;
}

Mat_রাশিমালা abs(ধ্রুবক Mat_রাশিমালা& e)
{
    CV_INSTRUMENT_REGION();

    Mat_রাশিমালা en;
    e.op->abs(e, en);
    ফেরত en;
}


আকার_2 Mat_রাশিমালা::size() ধ্রুবক
{
    যদি( isT(*এই) || isInv(*এই) )
        ফেরত আকার_2(a.rows, a.cols);
    যদি( isSolve(*এই) )
        ফেরত আকার_2(b.cols, a.cols);
    যদি( isInitializer(*এই) )
        ফেরত a.size();
    ফেরত op ? op->size(*এই) : আকার_2();
}


পূর্ণসংখ্যা Mat_রাশিমালা::type() ধ্রুবক
{
    CV_INSTRUMENT_REGION();

    যদি( isInitializer(*এই) )
        ফেরত a.type();
    যদি( isCmp(*এই) )
        ফেরত CV_MAKETYPE(CV_8U, a.channels());
    ফেরত op ? op->type(*এই) : -1;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////

শূন্য MatOp_Identity::assign(ধ্রুবক Mat_রাশিমালা& e, Mat& m, পূর্ণসংখ্যা _type) ধ্রুবক
{
    যদি( _type == -1 || _type == e.a.type() )
        m = e.a;
    অন্যথা
    {
        CV_Assert( CV_MAT_CN(_type) == e.a.channels() );
        e.a.convertTo(m, _type);
    }
}

অন্তঃরেখ শূন্য MatOp_Identity::make_রাশিমালা(Mat_রাশিমালা& res, ধ্রুবক Mat& m)
{
    res = Mat_রাশিমালা(&g_MatOp_Identity, 0, m, Mat(), Mat(), 1, 0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

শূন্য MatOp_AddEx::assign(ধ্রুবক Mat_রাশিমালা& e, Mat& m, পূর্ণসংখ্যা _type) ধ্রুবক
{
    Mat অস্থায়ী_2, &dst = _type == -1 || e.a.type() == _type ? m : অস্থায়ী_2;
    যদি( e.b.তথ্য )
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
                CV_LOG_ONCE_WARNING(ফাঁকা, "OpenCV/MatExpr: processing of multi-channel arrays might be changed in the future: "
                                          "https://github.com/opencv/opencv/issues/16739");
            cv::addWeighted(e.a, e.alpha, e.b, e.beta, e.s[0], dst);
        }
    }
    অন্যথা যদি( e.s.isReal() && (dst.তথ্য != m.তথ্য || fabs(e.alpha) != 1))
    {
        যদি (e.a.channels() > 1 && e.s[0] != 0.0)
            CV_LOG_ONCE_WARNING(ফাঁকা, "OpenCV/MatExpr: processing of multi-channel arrays might be changed in the future: "
                                      "https://github.com/opencv/opencv/issues/16739");
        e.a.convertTo(m, _type, e.alpha, e.s[0]);
        ফেরত;
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

    যদি( dst.তথ্য != m.তথ্য )
        dst.convertTo(m, m.type());
}


শূন্য MatOp_AddEx::add(ধ্রুবক Mat_রাশিমালা& e, ধ্রুবক Scalar& s, Mat_রাশিমালা& res) ধ্রুবক
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.s += s;
}


শূন্য MatOp_AddEx::subtract(ধ্রুবক Scalar& s, ধ্রুবক Mat_রাশিমালা& e, Mat_রাশিমালা& res) ধ্রুবক
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha = -res.alpha;
    res.beta = -res.beta;
    res.s = s - res.s;
}

শূন্য MatOp_AddEx::multiply(ধ্রুবক Mat_রাশিমালা& e, দ্বিগুণ s, Mat_রাশিমালা& res) ধ্রুবক
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha *= s;
    res.beta *= s;
    res.s *= s;
}

শূন্য MatOp_AddEx::divide(দ্বিগুণ s, ধ্রুবক Mat_রাশিমালা& e, Mat_রাশিমালা& res) ধ্রুবক
{
    CV_INSTRUMENT_REGION();

    যদি( isScaled(e) )
        MatOp_Bin::make_রাশিমালা(res, '/', e.a, Mat(), s/e.alpha);
    অন্যথা
        MatOp::divide(s, e, res);
}


শূন্য MatOp_AddEx::transpose(ধ্রুবক Mat_রাশিমালা& e, Mat_রাশিমালা& res) ধ্রুবক
{
    CV_INSTRUMENT_REGION();

    যদি( isScaled(e) )
        MatOp_T::make_রাশিমালা(res, e.a, e.alpha);
    অন্যথা
        MatOp::transpose(e, res);
}

শূন্য MatOp_AddEx::abs(ধ্রুবক Mat_রাশিমালা& e, Mat_রাশিমালা& res) ধ্রুবক
{
    CV_INSTRUMENT_REGION();

    যদি( (!e.b.তথ্য || e.beta == 0) && fabs(e.alpha) == 1 )
        MatOp_Bin::make_রাশিমালা(res, 'a', e.a, -e.s*e.alpha);
    অন্যথা যদি( e.b.তথ্য && e.alpha + e.beta == 0 && e.alpha*e.beta == -1 )
        MatOp_Bin::make_রাশিমালা(res, 'a', e.a, e.b);
    অন্যথা
        MatOp::abs(e, res);
}

অন্তঃরেখ শূন্য MatOp_AddEx::make_রাশিমালা(Mat_রাশিমালা& res, ধ্রুবক Mat& a, ধ্রুবক Mat& b, দ্বিগুণ alpha, দ্বিগুণ beta, ধ্রুবক Scalar& s)
{
    res = Mat_রাশিমালা(&g_MatOp_AddEx, 0, a, b, Mat(), alpha, beta, s);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

শূন্য MatOp_Bin::assign(ধ্রুবক Mat_রাশিমালা& e, Mat& m, পূর্ণসংখ্যা _type) ধ্রুবক
{
    Mat অস্থায়ী_2, &dst = _type == -1 || e.a.type() == _type ? m : অস্থায়ী_2;

    যদি( e.flags == '*' )
        cv::multiply(e.a, e.b, dst, e.alpha);
    অন্যথা যদি( e.flags == '/' && e.b.তথ্য )
        cv::divide(e.a, e.b, dst, e.alpha);
    অন্যথা যদি( e.flags == '/' && !e.b.তথ্য )
        cv::divide(e.alpha, e.a, dst );
    অন্যথা যদি( e.flags == '&' && e.b.তথ্য )
        bitwise_and(e.a, e.b, dst);
    অন্যথা যদি( e.flags == '&' && !e.b.তথ্য )
        bitwise_and(e.a, e.s, dst);
    অন্যথা যদি( e.flags == '|' && e.b.তথ্য )
        bitwise_or(e.a, e.b, dst);
    অন্যথা যদি( e.flags == '|' && !e.b.তথ্য )
        bitwise_or(e.a, e.s, dst);
    অন্যথা যদি( e.flags == '^' && e.b.তথ্য )
        bitwise_xor(e.a, e.b, dst);
    অন্যথা যদি( e.flags == '^' && !e.b.তথ্য )
        bitwise_xor(e.a, e.s, dst);
    অন্যথা যদি( e.flags == '~' && !e.b.তথ্য )
        bitwise_not(e.a, dst);
    অন্যথা যদি( e.flags == 'm' )
        cv::min(e.a, e.b, dst);
    অন্যথা যদি( e.flags == 'n' )
        cv::min(e.a, e.s[0], dst);
    অন্যথা যদি( e.flags == 'M' )
        cv::max(e.a, e.b, dst);
    অন্যথা যদি( e.flags == 'N' )
        cv::max(e.a, e.s[0], dst);
    অন্যথা যদি( e.flags == 'a' && e.b.তথ্য )
        cv::absdiff(e.a, e.b, dst);
    অন্যথা যদি( e.flags == 'a' && !e.b.তথ্য )
        cv::absdiff(e.a, e.s, dst);
    অন্যথা
        CV_Error(cv::Error::StsError, "Unknown operation");

    যদি( dst.তথ্য != m.তথ্য )
        dst.convertTo(m, _type);
}

শূন্য MatOp_Bin::multiply(ধ্রুবক Mat_রাশিমালা& e, দ্বিগুণ s, Mat_রাশিমালা& res) ধ্রুবক
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

শূন্য MatOp_Bin::divide(দ্বিগুণ s, ধ্রুবক Mat_রাশিমালা& e, Mat_রাশিমালা& res) ধ্রুবক
{
    CV_INSTRUMENT_REGION();

    যদি( e.flags == '/' && (!e.b.তথ্য || e.beta == 0) )
        MatOp_AddEx::make_রাশিমালা(res, e.a, Mat(), s/e.alpha, 0);
    অন্যথা
        MatOp::divide(s, e, res);
}

অন্তঃরেখ শূন্য MatOp_Bin::make_রাশিমালা(Mat_রাশিমালা& res, অক্ষর op, ধ্রুবক Mat& a, ধ্রুবক Mat& b, দ্বিগুণ scale)
{
    res = Mat_রাশিমালা(&g_MatOp_Bin, op, a, b, Mat(), scale, b.তথ্য ? 1 : 0);
}

অন্তঃরেখ শূন্য MatOp_Bin::make_রাশিমালা(Mat_রাশিমালা& res, অক্ষর op, ধ্রুবক Mat& a, ধ্রুবক Scalar& s)
{
    res = Mat_রাশিমালা(&g_MatOp_Bin, op, a, Mat(), Mat(), 1, 0, s);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

শূন্য MatOp_Cmp::assign(ধ্রুবক Mat_রাশিমালা& e, Mat& m, পূর্ণসংখ্যা _type) ধ্রুবক
{
    Mat অস্থায়ী_2, &dst = _type == -1 || _type == CV_8U ? m : অস্থায়ী_2;

    যদি( e.b.তথ্য )
        cv::compare(e.a, e.b, dst, e.flags);
    অন্যথা
        cv::compare(e.a, e.alpha, dst, e.flags);

    যদি( dst.তথ্য != m.তথ্য )
        dst.convertTo(m, _type);
}

অন্তঃরেখ শূন্য MatOp_Cmp::make_রাশিমালা(Mat_রাশিমালা& res, পূর্ণসংখ্যা cmpop, ধ্রুবক Mat& a, ধ্রুবক Mat& b)
{
    res = Mat_রাশিমালা(&g_MatOp_Cmp, cmpop, a, b, Mat(), 1, 1);
}

অন্তঃরেখ শূন্য MatOp_Cmp::make_রাশিমালা(Mat_রাশিমালা& res, পূর্ণসংখ্যা cmpop, ধ্রুবক Mat& a, দ্বিগুণ alpha)
{
    res = Mat_রাশিমালা(&g_MatOp_Cmp, cmpop, a, Mat(), Mat(), alpha, 1);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

শূন্য MatOp_T::assign(ধ্রুবক Mat_রাশিমালা& e, Mat& m, পূর্ণসংখ্যা _type) ধ্রুবক
{
    Mat অস্থায়ী_2, &dst = _type == -1 || _type == e.a.type() ? m : অস্থায়ী_2;

    cv::transpose(e.a, dst);

    যদি( dst.তথ্য != m.তথ্য || e.alpha != 1 )
        dst.convertTo(m, _type, e.alpha);
}

শূন্য MatOp_T::multiply(ধ্রুবক Mat_রাশিমালা& e, দ্বিগুণ s, Mat_রাশিমালা& res) ধ্রুবক
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha *= s;
}

শূন্য MatOp_T::transpose(ধ্রুবক Mat_রাশিমালা& e, Mat_রাশিমালা& res) ধ্রুবক
{
    CV_INSTRUMENT_REGION();

    যদি( e.alpha == 1 )
        MatOp_Identity::make_রাশিমালা(res, e.a);
    অন্যথা
        MatOp_AddEx::make_রাশিমালা(res, e.a, Mat(), e.alpha, 0);
}

অন্তঃরেখ শূন্য MatOp_T::make_রাশিমালা(Mat_রাশিমালা& res, ধ্রুবক Mat& a, দ্বিগুণ alpha)
{
    res = Mat_রাশিমালা(&g_MatOp_T, 0, a, Mat(), Mat(), alpha, 0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

শূন্য MatOp_GEMM::assign(ধ্রুবক Mat_রাশিমালা& e, Mat& m, পূর্ণসংখ্যা _type) ধ্রুবক
{
    Mat অস্থায়ী_2, &dst = _type == -1 || _type == e.a.type() ? m : অস্থায়ী_2;

    cv::gemm(e.a, e.b, e.alpha, e.c, e.beta, dst, e.flags);
    যদি( dst.তথ্য != m.তথ্য )
        dst.convertTo(m, _type);
}

শূন্য MatOp_GEMM::add(ধ্রুবক Mat_রাশিমালা& e1, ধ্রুবক Mat_রাশিমালা& e2, Mat_রাশিমালা& res) ধ্রুবক
{
    CV_INSTRUMENT_REGION();

    যৌক্তিক i1 = isIdentity(e1), i2 = isIdentity(e2);
    দ্বিগুণ alpha1 = i1 ? 1 : e1.alpha, alpha2 = i2 ? 1 : e2.alpha;

    যদি( isMatProd(e1) && (i2 || isScaled(e2) || isT(e2)) )
        MatOp_GEMM::make_রাশিমালা(res, (e1.flags & ~cv::GEMM_3_T)|(isT(e2) ? cv::GEMM_3_T : 0),
                             e1.a, e1.b, alpha1, e2.a, alpha2);
    অন্যথা যদি( isMatProd(e2) && (i1 || isScaled(e1) || isT(e1)) )
        MatOp_GEMM::make_রাশিমালা(res, (e2.flags & ~cv::GEMM_3_T)|(isT(e1) ? cv::GEMM_3_T : 0),
                             e2.a, e2.b, alpha2, e1.a, alpha1);
    অন্যথা যদি( এই == e2.op )
        MatOp::add(e1, e2, res);
    অন্যথা
        e2.op->add(e1, e2, res);
}

শূন্য MatOp_GEMM::subtract(ধ্রুবক Mat_রাশিমালা& e1, ধ্রুবক Mat_রাশিমালা& e2, Mat_রাশিমালা& res) ধ্রুবক
{
    CV_INSTRUMENT_REGION();

    যৌক্তিক i1 = isIdentity(e1), i2 = isIdentity(e2);
    দ্বিগুণ alpha1 = i1 ? 1 : e1.alpha, alpha2 = i2 ? 1 : e2.alpha;

    যদি( isMatProd(e1) && (i2 || isScaled(e2) || isT(e2)) )
        MatOp_GEMM::make_রাশিমালা(res, (e1.flags & ~cv::GEMM_3_T)|(isT(e2) ? cv::GEMM_3_T : 0),
                             e1.a, e1.b, alpha1, e2.a, -alpha2);
    অন্যথা যদি( isMatProd(e2) && (i1 || isScaled(e1) || isT(e1)) )
        MatOp_GEMM::make_রাশিমালা(res, (e2.flags & ~cv::GEMM_3_T)|(isT(e1) ? cv::GEMM_3_T : 0),
                            e2.a, e2.b, -alpha2, e1.a, alpha1);
    অন্যথা যদি( এই == e2.op )
        MatOp::subtract(e1, e2, res);
    অন্যথা
        e2.op->subtract(e1, e2, res);
}

শূন্য MatOp_GEMM::multiply(ধ্রুবক Mat_রাশিমালা& e, দ্বিগুণ s, Mat_রাশিমালা& res) ধ্রুবক
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha *= s;
    res.beta *= s;
}

শূন্য MatOp_GEMM::transpose(ধ্রুবক Mat_রাশিমালা& e, Mat_রাশিমালা& res) ধ্রুবক
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.flags = (!(e.flags & cv::GEMM_1_T) ? cv::GEMM_2_T : 0) |
                (!(e.flags & cv::GEMM_2_T) ? cv::GEMM_1_T : 0) |
                (!(e.flags & cv::GEMM_3_T) ? cv::GEMM_3_T : 0);
    swap(res.a, res.b);
}

অন্তঃরেখ শূন্য MatOp_GEMM::make_রাশিমালা(Mat_রাশিমালা& res, পূর্ণসংখ্যা flags, ধ্রুবক Mat& a, ধ্রুবক Mat& b,
                                 দ্বিগুণ alpha, ধ্রুবক Mat& c, দ্বিগুণ beta)
{
    res = Mat_রাশিমালা(&g_MatOp_GEMM, flags, a, b, c, alpha, beta);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

শূন্য MatOp_Invert::assign(ধ্রুবক Mat_রাশিমালা& e, Mat& m, পূর্ণসংখ্যা _type) ধ্রুবক
{
    Mat অস্থায়ী_2, &dst = _type == -1 || _type == e.a.type() ? m : অস্থায়ী_2;

    cv::invert(e.a, dst, e.flags);
    যদি( dst.তথ্য != m.তথ্য )
        dst.convertTo(m, _type);
}

শূন্য MatOp_Invert::matmul(ধ্রুবক Mat_রাশিমালা& e1, ধ্রুবক Mat_রাশিমালা& e2, Mat_রাশিমালা& res) ধ্রুবক
{
    যদি( isInv(e1) && isIdentity(e2) )
        MatOp_Solve::make_রাশিমালা(res, e1.flags, e1.a, e2.a);
    অন্যথা যদি( এই == e2.op )
        MatOp::matmul(e1, e2, res);
    অন্যথা
        e2.op->matmul(e1, e2, res);
}

অন্তঃরেখ শূন্য MatOp_Invert::make_রাশিমালা(Mat_রাশিমালা& res, পূর্ণসংখ্যা method, ধ্রুবক Mat& m)
{
    res = Mat_রাশিমালা(&g_MatOp_Invert, method, m, Mat(), Mat(), 1, 0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

শূন্য MatOp_Solve::assign(ধ্রুবক Mat_রাশিমালা& e, Mat& m, পূর্ণসংখ্যা _type) ধ্রুবক
{
    Mat অস্থায়ী_2, &dst = _type == -1 || _type == e.a.type() ? m : অস্থায়ী_2;

    cv::solve(e.a, e.b, dst, e.flags);
    যদি( dst.তথ্য != m.তথ্য )
        dst.convertTo(m, _type);
}

অন্তঃরেখ শূন্য MatOp_Solve::make_রাশিমালা(Mat_রাশিমালা& res, পূর্ণসংখ্যা method, ধ্রুবক Mat& a, ধ্রুবক Mat& b)
{
    res = Mat_রাশিমালা(&g_MatOp_Solve, method, a, b, Mat(), 1, 1);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

শূন্য MatOp_Initializer::assign(ধ্রুবক Mat_রাশিমালা& e, Mat& m, পূর্ণসংখ্যা _type) ধ্রুবক
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

শূন্য MatOp_Initializer::multiply(ধ্রুবক Mat_রাশিমালা& e, দ্বিগুণ s, Mat_রাশিমালা& res) ধ্রুবক
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha *= s;
}

অন্তঃরেখ শূন্য MatOp_Initializer::make_রাশিমালা(Mat_রাশিমালা& res, পূর্ণসংখ্যা method, আকার_2 sz, পূর্ণসংখ্যা type, দ্বিগুণ alpha)
{
    res = Mat_রাশিমালা(getGlobalMatOpInitializer(), method, Mat(sz, type, (শূন্য*)(size_t)0xEEEEEEEE), Mat(), Mat(), alpha, 0);
}

অন্তঃরেখ শূন্য MatOp_Initializer::make_রাশিমালা(Mat_রাশিমালা& res, পূর্ণসংখ্যা method, পূর্ণসংখ্যা ndims, ধ্রুবক পূর্ণসংখ্যা* sizes, পূর্ণসংখ্যা type, দ্বিগুণ alpha)
{
    res = Mat_রাশিমালা(getGlobalMatOpInitializer(), method, Mat(ndims, sizes, type, (শূন্য*)(size_t)0xEEEEEEEE), Mat(), Mat(), alpha, 0);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

Mat_রাশিমালা Mat::t() ধ্রুবক
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(*এই);
    Mat_রাশিমালা e;
    MatOp_T::make_রাশিমালা(e, *এই);
    ফেরত e;
}

Mat_রাশিমালা Mat::inv(পূর্ণসংখ্যা method) ধ্রুবক
{
    CV_INSTRUMENT_REGION();

    Mat_রাশিমালা e;
    MatOp_Invert::make_রাশিমালা(e, method, *এই);
    ফেরত e;
}


Mat_রাশিমালা Mat::mul(প্রবেশ_Array m, দ্বিগুণ scale) ধ্রুবক
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
    MatOp_Bin::make_রাশিমালা(e, '*', *এই, b, scale);
    ফেরত e;
}

Mat_রাশিমালা Mat::zeros(পূর্ণসংখ্যা rows, পূর্ণসংখ্যা cols, পূর্ণসংখ্যা type)
{
    CV_INSTRUMENT_REGION();

    Mat_রাশিমালা e;
    MatOp_Initializer::make_রাশিমালা(e, '0', আকার_2(cols, rows), type);
    ফেরত e;
}

Mat_রাশিমালা Mat::zeros(আকার_2 size, পূর্ণসংখ্যা type)
{
    CV_INSTRUMENT_REGION();

    Mat_রাশিমালা e;
    MatOp_Initializer::make_রাশিমালা(e, '0', size, type);
    ফেরত e;
}

Mat_রাশিমালা Mat::zeros(পূর্ণসংখ্যা ndims, ধ্রুবক পূর্ণসংখ্যা* sizes, পূর্ণসংখ্যা type)
{
    CV_INSTRUMENT_REGION();

    Mat_রাশিমালা e;
    MatOp_Initializer::make_রাশিমালা(e, '0', ndims, sizes, type);
    ফেরত e;
}

Mat_রাশিমালা Mat::zeros(ধ্রুবক MatShape& shape, পূর্ণসংখ্যা type)
{
    CV_INSTRUMENT_REGION();

    Mat_রাশিমালা e;
    MatOp_Initializer::make_রাশিমালা(e, '0', shape.dims, shape.p, type);
    ফেরত e;
}

Mat_রাশিমালা Mat::ones(পূর্ণসংখ্যা rows, পূর্ণসংখ্যা cols, পূর্ণসংখ্যা type)
{
    CV_INSTRUMENT_REGION();

    Mat_রাশিমালা e;
    MatOp_Initializer::make_রাশিমালা(e, '1', আকার_2(cols, rows), type);
    ফেরত e;
}

Mat_রাশিমালা Mat::ones(আকার_2 size, পূর্ণসংখ্যা type)
{
    CV_INSTRUMENT_REGION();

    Mat_রাশিমালা e;
    MatOp_Initializer::make_রাশিমালা(e, '1', size, type);
    ফেরত e;
}

Mat_রাশিমালা Mat::ones(পূর্ণসংখ্যা ndims, ধ্রুবক পূর্ণসংখ্যা* sizes, পূর্ণসংখ্যা type)
{
    CV_INSTRUMENT_REGION();

    Mat_রাশিমালা e;
    MatOp_Initializer::make_রাশিমালা(e, '1', ndims, sizes, type);
    ফেরত e;
}

Mat_রাশিমালা Mat::ones(ধ্রুবক MatShape& shape, পূর্ণসংখ্যা type)
{
    CV_INSTRUMENT_REGION();

    Mat_রাশিমালা e;
    MatOp_Initializer::make_রাশিমালা(e, '1', shape.dims, shape.p, type);
    ফেরত e;
}

Mat_রাশিমালা Mat::eye(পূর্ণসংখ্যা rows, পূর্ণসংখ্যা cols, পূর্ণসংখ্যা type)
{
    CV_INSTRUMENT_REGION();

    Mat_রাশিমালা e;
    MatOp_Initializer::make_রাশিমালা(e, 'I', আকার_2(cols, rows), type);
    ফেরত e;
}

Mat_রাশিমালা Mat::eye(আকার_2 size, পূর্ণসংখ্যা type)
{
    CV_INSTRUMENT_REGION();

    Mat_রাশিমালা e;
    MatOp_Initializer::make_রাশিমালা(e, 'I', size, type);
    ফেরত e;
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

প্রবেশ_Array_2::প্রবেশ_Array_2(ধ্রুবক Mat_রাশিমালা& রাশিমালা)
{
    যদি (!isIdentity(রাশিমালা))
    {
        Mat ফলাফল = রাশিমালা;  // TODO improve through refcount == 1 of expr.a (inplace operation is possible - except gemm?)
        Mat_রাশিমালা ফলাফল_রাশিমালা(ফলাফল);
        swap(const_cast<Mat_রাশিমালা&>(রাশিমালা), ফলাফল_রাশিমালা);
    }
    CV_Assert(isIdentity(রাশিমালা));
    init(FIXED_TYPE + FIXED_আকার + MAT + ACCESS_READ, &রাশিমালা.a);
}

} // cv::
