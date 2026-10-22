// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


/* ////////////////////////////////////////////////////////////////////
//
//  Mat basic operations: Copy, Set
//
// */

#অন্তর্ভাৱ "precomp.hpp"
#অন্তর্ভাৱ <opencv2/core/utils/logger.hpp>

নামাকাশ cv
{

//This and its overload below are used in various MatExpr operator overloads
//implemented to check that Matrix operands exist.
স্থির শূন্য checkOperandsExist(ধ্রুৱ Mat& a)
{
    যদি (a.empty())
    {
        CV_Error(cv::Error::StsBadArg, "Matrix operand is an empty matrix.");
    }
}

স্থির শূন্য checkOperandsExist(ধ্রুৱ Mat& a, ধ্রুৱ Mat& b)
{
    যদি (a.empty() || b.empty())
    {
        CV_Error(cv::Error::StsBadArg, "One or more matrix operands are empty.");
    }
}


শ্ৰেণী MatOp_Identity CV_FINAL : সার্ৱজনিক MatOp
{
সার্ৱজনিক:
    MatOp_Identity() {}
    আভাস ~MatOp_Identity() {}

    তার্কিক elementWise(ধ্রুৱ Mat_ৰাশিমালা& /*ৰাশিমালা*/) ধ্রুৱ CV_OVERRIDE { ঘূৰাই সঁচা; }
    শূন্য assign(ধ্রুৱ Mat_ৰাশিমালা& ৰাশিমালা, Mat& m, পূর্ণাঙ্ক type=-1) ধ্রুৱ CV_OVERRIDE;

    স্থির শূন্য make_ৰাশিমালা(Mat_ৰাশিমালা& res, ধ্রুৱ Mat& m);
};

স্থির MatOp_Identity g_MatOp_Identity;

শ্ৰেণী MatOp_AddEx CV_FINAL : সার্ৱজনিক MatOp
{
সার্ৱজনিক:
    MatOp_AddEx() {}
    আভাস ~MatOp_AddEx() {}

    তার্কিক elementWise(ধ্রুৱ Mat_ৰাশিমালা& /*ৰাশিমালা*/) ধ্রুৱ CV_OVERRIDE { ঘূৰাই সঁচা; }
    শূন্য assign(ধ্রুৱ Mat_ৰাশিমালা& ৰাশিমালা, Mat& m, পূর্ণাঙ্ক type=-1) ধ্রুৱ CV_OVERRIDE;

    শূন্য add(ধ্রুৱ Mat_ৰাশিমালা& e1, ধ্রুৱ Scalar& s, Mat_ৰাশিমালা& res) ধ্রুৱ CV_OVERRIDE;
    শূন্য subtract(ধ্রুৱ Scalar& s, ধ্রুৱ Mat_ৰাশিমালা& ৰাশিমালা, Mat_ৰাশিমালা& res) ধ্রুৱ CV_OVERRIDE;
    শূন্য multiply(ধ্রুৱ Mat_ৰাশিমালা& e1, দ্ৱিগুণ s, Mat_ৰাশিমালা& res) ধ্রুৱ CV_OVERRIDE;
    শূন্য divide(দ্ৱিগুণ s, ধ্রুৱ Mat_ৰাশিমালা& e, Mat_ৰাশিমালা& res) ধ্রুৱ CV_OVERRIDE;

    শূন্য transpose(ধ্রুৱ Mat_ৰাশিমালা& e1, Mat_ৰাশিমালা& res) ধ্রুৱ CV_OVERRIDE;
    শূন্য abs(ধ্রুৱ Mat_ৰাশিমালা& ৰাশিমালা, Mat_ৰাশিমালা& res) ধ্রুৱ CV_OVERRIDE;

    স্থির শূন্য make_ৰাশিমালা(Mat_ৰাশিমালা& res, ধ্রুৱ Mat& a, ধ্রুৱ Mat& b, দ্ৱিগুণ alpha, দ্ৱিগুণ beta, ধ্রুৱ Scalar& s=Scalar());
};

স্থির MatOp_AddEx g_MatOp_AddEx;

শ্ৰেণী MatOp_Bin CV_FINAL : সার্ৱজনিক MatOp
{
সার্ৱজনিক:
    MatOp_Bin() {}
    আভাস ~MatOp_Bin() {}

    তার্কিক elementWise(ধ্রুৱ Mat_ৰাশিমালা& /*ৰাশিমালা*/) ধ্রুৱ CV_OVERRIDE { ঘূৰাই সঁচা; }
    শূন্য assign(ধ্রুৱ Mat_ৰাশিমালা& ৰাশিমালা, Mat& m, পূর্ণাঙ্ক type=-1) ধ্রুৱ CV_OVERRIDE;

    শূন্য multiply(ধ্রুৱ Mat_ৰাশিমালা& e1, দ্ৱিগুণ s, Mat_ৰাশিমালা& res) ধ্রুৱ CV_OVERRIDE;
    শূন্য divide(দ্ৱিগুণ s, ধ্রুৱ Mat_ৰাশিমালা& e, Mat_ৰাশিমালা& res) ধ্রুৱ CV_OVERRIDE;

    স্থির শূন্য make_ৰাশিমালা(Mat_ৰাশিমালা& res, ৱর্ণ op, ধ্রুৱ Mat& a, ধ্রুৱ Mat& b, দ্ৱিগুণ scale=1);
    স্থির শূন্য make_ৰাশিমালা(Mat_ৰাশিমালা& res, ৱর্ণ op, ধ্রুৱ Mat& a, ধ্রুৱ Scalar& s);
};

স্থির MatOp_Bin g_MatOp_Bin;

শ্ৰেণী MatOp_Cmp CV_FINAL : সার্ৱজনিক MatOp
{
সার্ৱজনিক:
    MatOp_Cmp() {}
    আভাস ~MatOp_Cmp() {}

    তার্কিক elementWise(ধ্রুৱ Mat_ৰাশিমালা& /*ৰাশিমালা*/) ধ্রুৱ CV_OVERRIDE { ঘূৰাই সঁচা; }
    শূন্য assign(ধ্রুৱ Mat_ৰাশিমালা& ৰাশিমালা, Mat& m, পূর্ণাঙ্ক type=-1) ধ্রুৱ CV_OVERRIDE;

    স্থির শূন্য make_ৰাশিমালা(Mat_ৰাশিমালা& res, পূর্ণাঙ্ক cmpop, ধ্রুৱ Mat& a, ধ্রুৱ Mat& b);
    স্থির শূন্য make_ৰাশিমালা(Mat_ৰাশিমালা& res, পূর্ণাঙ্ক cmpop, ধ্রুৱ Mat& a, দ্ৱিগুণ alpha);
};

স্থির MatOp_Cmp g_MatOp_Cmp;

শ্ৰেণী MatOp_GEMM CV_FINAL : সার্ৱজনিক MatOp
{
সার্ৱজনিক:
    MatOp_GEMM() {}
    আভাস ~MatOp_GEMM() {}

    তার্কিক elementWise(ধ্রুৱ Mat_ৰাশিমালা& /*ৰাশিমালা*/) ধ্রুৱ CV_OVERRIDE { ঘূৰাই মিছা; }
    শূন্য assign(ধ্রুৱ Mat_ৰাশিমালা& ৰাশিমালা, Mat& m, পূর্ণাঙ্ক type=-1) ধ্রুৱ CV_OVERRIDE;

    শূন্য add(ধ্রুৱ Mat_ৰাশিমালা& e1, ধ্রুৱ Mat_ৰাশিমালা& e2, Mat_ৰাশিমালা& res) ধ্রুৱ CV_OVERRIDE;
    শূন্য subtract(ধ্রুৱ Mat_ৰাশিমালা& e1, ধ্রুৱ Mat_ৰাশিমালা& e2, Mat_ৰাশিমালা& res) ধ্রুৱ CV_OVERRIDE;
    শূন্য multiply(ধ্রুৱ Mat_ৰাশিমালা& e, দ্ৱিগুণ s, Mat_ৰাশিমালা& res) ধ্রুৱ CV_OVERRIDE;

    শূন্য transpose(ধ্রুৱ Mat_ৰাশিমালা& ৰাশিমালা, Mat_ৰাশিমালা& res) ধ্রুৱ CV_OVERRIDE;

    Size size(ধ্রুৱ Mat_ৰাশিমালা& ৰাশিমালা) ধ্রুৱ CV_OVERRIDE
    {
        ঘূৰাই Size(
            (ৰাশিমালা.flags & GEMM_2_T) ? ৰাশিমালা.b.rows : ৰাশিমালা.b.cols,
            (ৰাশিমালা.flags & GEMM_1_T) ? ৰাশিমালা.a.cols : ৰাশিমালা.a.rows
        );
    }

    স্থির শূন্য make_ৰাশিমালা(Mat_ৰাশিমালা& res, পূর্ণাঙ্ক flags, ধ্রুৱ Mat& a, ধ্রুৱ Mat& b,
                         দ্ৱিগুণ alpha=1, ধ্রুৱ Mat& c=Mat(), দ্ৱিগুণ beta=1);

};

স্থির MatOp_GEMM g_MatOp_GEMM;

শ্ৰেণী MatOp_Invert CV_FINAL : সার্ৱজনিক MatOp
{
সার্ৱজনিক:
    MatOp_Invert() {}
    আভাস ~MatOp_Invert() {}

    তার্কিক elementWise(ধ্রুৱ Mat_ৰাশিমালা& /*ৰাশিমালা*/) ধ্রুৱ CV_OVERRIDE { ঘূৰাই মিছা; }
    শূন্য assign(ধ্রুৱ Mat_ৰাশিমালা& ৰাশিমালা, Mat& m, পূর্ণাঙ্ক type=-1) ধ্রুৱ CV_OVERRIDE;

    শূন্য matmul(ধ্রুৱ Mat_ৰাশিমালা& expr1, ধ্রুৱ Mat_ৰাশিমালা& expr2, Mat_ৰাশিমালা& res) ধ্রুৱ CV_OVERRIDE;

    স্থির শূন্য make_ৰাশিমালা(Mat_ৰাশিমালা& res, পূর্ণাঙ্ক method, ধ্রুৱ Mat& m);
};

স্থির MatOp_Invert g_MatOp_Invert;

শ্ৰেণী MatOp_T CV_FINAL : সার্ৱজনিক MatOp
{
সার্ৱজনিক:
    MatOp_T() {}
    আভাস ~MatOp_T() {}

    তার্কিক elementWise(ধ্রুৱ Mat_ৰাশিমালা& /*ৰাশিমালা*/) ধ্রুৱ CV_OVERRIDE { ঘূৰাই মিছা; }
    শূন্য assign(ধ্রুৱ Mat_ৰাশিমালা& ৰাশিমালা, Mat& m, পূর্ণাঙ্ক type=-1) ধ্রুৱ CV_OVERRIDE;

    শূন্য multiply(ধ্রুৱ Mat_ৰাশিমালা& e1, দ্ৱিগুণ s, Mat_ৰাশিমালা& res) ধ্রুৱ CV_OVERRIDE;
    শূন্য transpose(ধ্রুৱ Mat_ৰাশিমালা& ৰাশিমালা, Mat_ৰাশিমালা& res) ধ্রুৱ CV_OVERRIDE;

    স্থির শূন্য make_ৰাশিমালা(Mat_ৰাশিমালা& res, ধ্রুৱ Mat& a, দ্ৱিগুণ alpha=1);
};

স্থির MatOp_T g_MatOp_T;

শ্ৰেণী MatOp_Solve CV_FINAL : সার্ৱজনিক MatOp
{
সার্ৱজনিক:
    MatOp_Solve() {}
    আভাস ~MatOp_Solve() {}

    তার্কিক elementWise(ধ্রুৱ Mat_ৰাশিমালা& /*ৰাশিমালা*/) ধ্রুৱ CV_OVERRIDE { ঘূৰাই মিছা; }
    শূন্য assign(ধ্রুৱ Mat_ৰাশিমালা& ৰাশিমালা, Mat& m, পূর্ণাঙ্ক type=-1) ধ্রুৱ CV_OVERRIDE;

    স্থির শূন্য make_ৰাশিমালা(Mat_ৰাশিমালা& res, পূর্ণাঙ্ক method, ধ্রুৱ Mat& a, ধ্রুৱ Mat& b);
};

স্থির MatOp_Solve g_MatOp_Solve;

শ্ৰেণী MatOp_Initializer CV_FINAL : সার্ৱজনিক MatOp
{
সার্ৱজনিক:
    MatOp_Initializer() {}
    আভাস ~MatOp_Initializer() {}

    তার্কিক elementWise(ধ্রুৱ Mat_ৰাশিমালা& /*ৰাশিমালা*/) ধ্রুৱ CV_OVERRIDE { ঘূৰাই মিছা; }
    শূন্য assign(ধ্রুৱ Mat_ৰাশিমালা& ৰাশিমালা, Mat& m, পূর্ণাঙ্ক type=-1) ধ্রুৱ CV_OVERRIDE;

    শূন্য multiply(ধ্রুৱ Mat_ৰাশিমালা& e, দ্ৱিগুণ s, Mat_ৰাশিমালা& res) ধ্রুৱ CV_OVERRIDE;

    স্থির শূন্য make_ৰাশিমালা(Mat_ৰাশিমালা& res, পূর্ণাঙ্ক method, Size sz, পূর্ণাঙ্ক type, দ্ৱিগুণ alpha=1);
    স্থির শূন্য make_ৰাশিমালা(Mat_ৰাশিমালা& res, পূর্ণাঙ্ক method, পূর্ণাঙ্ক ndims, ধ্রুৱ পূর্ণাঙ্ক* sizes, পূর্ণাঙ্ক type, দ্ৱিগুণ alpha=1);
};

স্থির MatOp_Initializer* getGlobalMatOpInitializer()
{
    CV_SINGLETON_LAZY_INIT(MatOp_Initializer, নূতন MatOp_Initializer())
}

স্থির অন্তঃস্থ তার্কিক isIdentity(ধ্রুৱ Mat_ৰাশিমালা& e) { ঘূৰাই e.op == &g_MatOp_Identity; }
স্থির অন্তঃস্থ তার্কিক isAddEx(ধ্রুৱ Mat_ৰাশিমালা& e) { ঘূৰাই e.op == &g_MatOp_AddEx; }
স্থির অন্তঃস্থ তার্কিক isScaled(ধ্রুৱ Mat_ৰাশিমালা& e) { ঘূৰাই isAddEx(e) && (!e.b.data || e.beta == 0) && e.s == Scalar(); }
স্থির অন্তঃস্থ তার্কিক isBin(ধ্রুৱ Mat_ৰাশিমালা& e, ৱর্ণ c) { ঘূৰাই e.op == &g_MatOp_Bin && e.flags == c; }
স্থির অন্তঃস্থ তার্কিক isCmp(ধ্রুৱ Mat_ৰাশিমালা& e) { ঘূৰাই e.op == &g_MatOp_Cmp; }
স্থির অন্তঃস্থ তার্কিক isReciprocal(ধ্রুৱ Mat_ৰাশিমালা& e) { ঘূৰাই isBin(e,'/') && (!e.b.data || e.beta == 0); }
স্থির অন্তঃস্থ তার্কিক isT(ধ্রুৱ Mat_ৰাশিমালা& e) { ঘূৰাই e.op == &g_MatOp_T; }
স্থির অন্তঃস্থ তার্কিক isInv(ধ্রুৱ Mat_ৰাশিমালা& e) { ঘূৰাই e.op == &g_MatOp_Invert; }
স্থির অন্তঃস্থ তার্কিক isSolve(ধ্রুৱ Mat_ৰাশিমালা& e) { ঘূৰাই e.op == &g_MatOp_Solve; }
//static inline bool isGEMM(const MatExpr& e) { return e.op == &g_MatOp_GEMM; }
স্থির অন্তঃস্থ তার্কিক isMatProd(ধ্রুৱ Mat_ৰাশিমালা& e) { ঘূৰাই e.op == &g_MatOp_GEMM && (!e.c.data || e.beta == 0); }
স্থির অন্তঃস্থ তার্কিক isInitializer(ধ্রুৱ Mat_ৰাশিমালা& e) { ঘূৰাই e.op == getGlobalMatOpInitializer(); }

/////////////////////////////////////////////////////////////////////////////////////////////////////

MatOp::MatOp() {}
MatOp::~MatOp() {}


তার্কিক MatOp::elementWise(ধ্রুৱ Mat_ৰাশিমালা& /*ৰাশিমালা*/) ধ্রুৱ
{
    ঘূৰাই মিছা;
}

শূন্য MatOp::roi(ধ্রুৱ Mat_ৰাশিমালা& ৰাশিমালা, ধ্রুৱ Range& rowRange, ধ্রুৱ Range& colRange, Mat_ৰাশিমালা& e) ধ্রুৱ
{
    যদি( elementWise(ৰাশিমালা) )
    {
        e = Mat_ৰাশিমালা(ৰাশিমালা.op, ৰাশিমালা.flags, Mat(), Mat(), Mat(),
                    ৰাশিমালা.alpha, ৰাশিমালা.beta, ৰাশিমালা.s);
        যদি(ৰাশিমালা.a.data)
            e.a = ৰাশিমালা.a(rowRange, colRange);
        যদি(ৰাশিমালা.b.data)
            e.b = ৰাশিমালা.b(rowRange, colRange);
        যদি(ৰাশিমালা.c.data)
            e.c = ৰাশিমালা.c(rowRange, colRange);
    }
    অন্যথা
    {
        Mat m;
        ৰাশিমালা.op->assign(ৰাশিমালা, m);
        e = Mat_ৰাশিমালা(&g_MatOp_Identity, 0, m(rowRange, colRange), Mat(), Mat());
    }
}

শূন্য MatOp::diag(ধ্রুৱ Mat_ৰাশিমালা& ৰাশিমালা, পূর্ণাঙ্ক d, Mat_ৰাশিমালা& e) ধ্রুৱ
{
    যদি( elementWise(ৰাশিমালা) )
    {
        e = Mat_ৰাশিমালা(ৰাশিমালা.op, ৰাশিমালা.flags, Mat(), Mat(), Mat(),
                    ৰাশিমালা.alpha, ৰাশিমালা.beta, ৰাশিমালা.s);
        যদি(ৰাশিমালা.a.data)
            e.a = ৰাশিমালা.a.diag(d);
        যদি(ৰাশিমালা.b.data)
            e.b = ৰাশিমালা.b.diag(d);
        যদি(ৰাশিমালা.c.data)
            e.c = ৰাশিমালা.c.diag(d);
    }
    অন্যথা
    {
        Mat m;
        ৰাশিমালা.op->assign(ৰাশিমালা, m);
        e = Mat_ৰাশিমালা(&g_MatOp_Identity, 0, m.diag(d), Mat(), Mat());
    }
}


শূন্য MatOp::augAssignAdd(ধ্রুৱ Mat_ৰাশিমালা& ৰাশিমালা, Mat& m) ধ্রুৱ
{
    Mat temp;
    ৰাশিমালা.op->assign(ৰাশিমালা, temp);
    m += temp;
}


শূন্য MatOp::augAssignSubtract(ধ্রুৱ Mat_ৰাশিমালা& ৰাশিমালা, Mat& m) ধ্রুৱ
{
    Mat temp;
    ৰাশিমালা.op->assign(ৰাশিমালা, temp);
    m -= temp;
}


শূন্য MatOp::augAssignMultiply(ধ্রুৱ Mat_ৰাশিমালা& ৰাশিমালা, Mat& m) ধ্রুৱ
{
    Mat temp;
    ৰাশিমালা.op->assign(ৰাশিমালা, temp);
    m *= temp;
}


শূন্য MatOp::augAssignDivide(ধ্রুৱ Mat_ৰাশিমালা& ৰাশিমালা, Mat& m) ধ্রুৱ
{
    Mat temp;
    ৰাশিমালা.op->assign(ৰাশিমালা, temp);
    m /= temp;
}


শূন্য MatOp::augAssignAnd(ধ্রুৱ Mat_ৰাশিমালা& ৰাশিমালা, Mat& m) ধ্রুৱ
{
    Mat temp;
    ৰাশিমালা.op->assign(ৰাশিমালা, temp);
    m &= temp;
}


শূন্য MatOp::augAssignOr(ধ্রুৱ Mat_ৰাশিমালা& ৰাশিমালা, Mat& m) ধ্রুৱ
{
    Mat temp;
    ৰাশিমালা.op->assign(ৰাশিমালা, temp);
    m |= temp;
}


শূন্য MatOp::augAssignXor(ধ্রুৱ Mat_ৰাশিমালা& ৰাশিমালা, Mat& m) ধ্রুৱ
{
    Mat temp;
    ৰাশিমালা.op->assign(ৰাশিমালা, temp);
    m ^= temp;
}


শূন্য MatOp::add(ধ্রুৱ Mat_ৰাশিমালা& e1, ধ্রুৱ Mat_ৰাশিমালা& e2, Mat_ৰাশিমালা& res) ধ্রুৱ
{
    CV_INSTRUMENT_REGION();

    যদি( এতত্ == e2.op )
    {
        দ্ৱিগুণ alpha = 1, beta = 1;
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
        MatOp_AddEx::make_ৰাশিমালা(res, m1, m2, alpha, beta, s);
    }
    অন্যথা
        e2.op->add(e1, e2, res);
}


শূন্য MatOp::add(ধ্রুৱ Mat_ৰাশিমালা& expr1, ধ্রুৱ Scalar& s, Mat_ৰাশিমালা& res) ধ্রুৱ
{
    CV_INSTRUMENT_REGION();

    Mat m1;
    expr1.op->assign(expr1, m1);
    MatOp_AddEx::make_ৰাশিমালা(res, m1, Mat(), 1, 0, s);
}


শূন্য MatOp::subtract(ধ্রুৱ Mat_ৰাশিমালা& e1, ধ্রুৱ Mat_ৰাশিমালা& e2, Mat_ৰাশিমালা& res) ধ্রুৱ
{
    CV_INSTRUMENT_REGION();

    যদি( এতত্ == e2.op )
    {
        দ্ৱিগুণ alpha = 1, beta = -1;
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
        MatOp_AddEx::make_ৰাশিমালা(res, m1, m2, alpha, beta, s);
    }
    অন্যথা
        e2.op->subtract(e1, e2, res);
}


শূন্য MatOp::subtract(ধ্রুৱ Scalar& s, ধ্রুৱ Mat_ৰাশিমালা& ৰাশিমালা, Mat_ৰাশিমালা& res) ধ্রুৱ
{
    CV_INSTRUMENT_REGION();

    Mat m;
    ৰাশিমালা.op->assign(ৰাশিমালা, m);
    MatOp_AddEx::make_ৰাশিমালা(res, m, Mat(), -1, 0, s);
}


শূন্য MatOp::multiply(ধ্রুৱ Mat_ৰাশিমালা& e1, ধ্রুৱ Mat_ৰাশিমালা& e2, Mat_ৰাশিমালা& res, দ্ৱিগুণ scale) ধ্রুৱ
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

            MatOp_Bin::make_ৰাশিমালা(res, '/', m2, e1.a, scale/e1.alpha);
        }
        অন্যথা
        {
            ৱর্ণ op = '*';
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

            MatOp_Bin::make_ৰাশিমালা(res, op, m1, m2, scale);
        }
    }
    অন্যথা
        e2.op->multiply(e1, e2, res, scale);
}


শূন্য MatOp::multiply(ধ্রুৱ Mat_ৰাশিমালা& ৰাশিমালা, দ্ৱিগুণ s, Mat_ৰাশিমালা& res) ধ্রুৱ
{
    CV_INSTRUMENT_REGION();

    Mat m;
    ৰাশিমালা.op->assign(ৰাশিমালা, m);
    MatOp_AddEx::make_ৰাশিমালা(res, m, Mat(), s, 0);
}


শূন্য MatOp::divide(ধ্রুৱ Mat_ৰাশিমালা& e1, ধ্রুৱ Mat_ৰাশিমালা& e2, Mat_ৰাশিমালা& res, দ্ৱিগুণ scale) ধ্রুৱ
{
    CV_INSTRUMENT_REGION();

    যদি( এতত্ == e2.op )
    {
        যদি( isReciprocal(e1) && isReciprocal(e2) )
            MatOp_Bin::make_ৰাশিমালা(res, '/', e2.a, e1.a, e1.alpha/e2.alpha);
        অন্যথা
        {
            Mat m1, m2;
            ৱর্ণ op = '/';

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
            MatOp_Bin::make_ৰাশিমালা(res, op, m1, m2, scale);
        }
    }
    অন্যথা
        e2.op->divide(e1, e2, res, scale);
}


শূন্য MatOp::divide(দ্ৱিগুণ s, ধ্রুৱ Mat_ৰাশিমালা& ৰাশিমালা, Mat_ৰাশিমালা& res) ধ্রুৱ
{
    CV_INSTRUMENT_REGION();

    Mat m;
    ৰাশিমালা.op->assign(ৰাশিমালা, m);
    MatOp_Bin::make_ৰাশিমালা(res, '/', m, Mat(), s);
}


শূন্য MatOp::abs(ধ্রুৱ Mat_ৰাশিমালা& ৰাশিমালা, Mat_ৰাশিমালা& res) ধ্রুৱ
{
    CV_INSTRUMENT_REGION();

    Mat m;
    ৰাশিমালা.op->assign(ৰাশিমালা, m);
    MatOp_Bin::make_ৰাশিমালা(res, 'a', m, Mat());
}


শূন্য MatOp::transpose(ধ্রুৱ Mat_ৰাশিমালা& ৰাশিমালা, Mat_ৰাশিমালা& res) ধ্রুৱ
{
    CV_INSTRUMENT_REGION();

    Mat m;
    ৰাশিমালা.op->assign(ৰাশিমালা, m);
    MatOp_T::make_ৰাশিমালা(res, m, 1);
}


শূন্য MatOp::matmul(ধ্রুৱ Mat_ৰাশিমালা& e1, ধ্রুৱ Mat_ৰাশিমালা& e2, Mat_ৰাশিমালা& res) ধ্রুৱ
{
    যদি( এতত্ == e2.op )
    {
        দ্ৱিগুণ scale = 1;
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

        MatOp_GEMM::make_ৰাশিমালা(res, flags, m1, m2, scale);
    }
    অন্যথা
        e2.op->matmul(e1, e2, res);
}


শূন্য MatOp::invert(ধ্রুৱ Mat_ৰাশিমালা& ৰাশিমালা, পূর্ণাঙ্ক method, Mat_ৰাশিমালা& res) ধ্রুৱ
{
    Mat m;
    ৰাশিমালা.op->assign(ৰাশিমালা, m);
    MatOp_Invert::make_ৰাশিমালা(res, method, m);
}


Size MatOp::size(ধ্রুৱ Mat_ৰাশিমালা& ৰাশিমালা) ধ্রুৱ
{
    ঘূৰাই !ৰাশিমালা.a.empty() ? ৰাশিমালা.a.size() : ৰাশিমালা.b.empty() ? ৰাশিমালা.b.size() : ৰাশিমালা.c.size();
}

পূর্ণাঙ্ক MatOp::type(ধ্রুৱ Mat_ৰাশিমালা& ৰাশিমালা) ধ্রুৱ
{
    CV_INSTRUMENT_REGION();

    ঘূৰাই !ৰাশিমালা.a.empty() ? ৰাশিমালা.a.type() : ৰাশিমালা.b.empty() ? ৰাশিমালা.b.type() : ৰাশিমালা.c.type();
}

//////////////////////////////////////////////////////////////////////////////////////////////////

Mat_ৰাশিমালা::Mat_ৰাশিমালা(ধ্রুৱ Mat& m) : op(&g_MatOp_Identity), flags(0), a(m), b(Mat()), c(Mat()), alpha(1), beta(0), s(Scalar())
{
}

Mat_ৰাশিমালা Mat_ৰাশিমালা::row(পূর্ণাঙ্ক y) ধ্রুৱ
{
    Mat_ৰাশিমালা e;
    op->roi(*এতত্, Range(y, y+1), Range::all(), e);
    ঘূৰাই e;
}

Mat_ৰাশিমালা Mat_ৰাশিমালা::col(পূর্ণাঙ্ক x) ধ্রুৱ
{
    Mat_ৰাশিমালা e;
    op->roi(*এতত্, Range::all(), Range(x, x+1), e);
    ঘূৰাই e;
}

Mat_ৰাশিমালা Mat_ৰাশিমালা::diag(পূর্ণাঙ্ক d) ধ্রুৱ
{
    Mat_ৰাশিমালা e;
    op->diag(*এতত্, d, e);
    ঘূৰাই e;
}

Mat_ৰাশিমালা Mat_ৰাশিমালা::operator()( ধ্রুৱ Range& rowRange, ধ্রুৱ Range& colRange ) ধ্রুৱ
{
    Mat_ৰাশিমালা e;
    op->roi(*এতত্, rowRange, colRange, e);
    ঘূৰাই e;
}

Mat_ৰাশিমালা Mat_ৰাশিমালা::operator()( ধ্রুৱ Rect& roi ) ধ্রুৱ
{
    Mat_ৰাশিমালা e;
    op->roi(*এতত্, Range(roi.y, roi.y + roi.height), Range(roi.x, roi.x + roi.width), e);
    ঘূৰাই e;
}

Mat Mat_ৰাশিমালা::cross(ধ্রুৱ Mat& m) ধ্রুৱ
{
    ঘূৰাই ((Mat)*এতত্).cross(m);
}

দ্ৱিগুণ Mat_ৰাশিমালা::dot(ধ্রুৱ Mat& m) ধ্রুৱ
{
    ঘূৰাই ((Mat)*এতত্).dot(m);
}

Mat_ৰাশিমালা Mat_ৰাশিমালা::t() ধ্রুৱ
{
    Mat_ৰাশিমালা e;
    op->transpose(*এতত্, e);
    ঘূৰাই e;
}

Mat_ৰাশিমালা Mat_ৰাশিমালা::inv(পূর্ণাঙ্ক method) ধ্রুৱ
{
    Mat_ৰাশিমালা e;
    op->invert(*এতত্, method, e);
    ঘূৰাই e;
}

Mat_ৰাশিমালা Mat_ৰাশিমালা::mul(ধ্রুৱ Mat_ৰাশিমালা& e, দ্ৱিগুণ scale) ধ্রুৱ
{
    Mat_ৰাশিমালা en;
    op->multiply(*এতত্, e, en, scale);
    ঘূৰাই en;
}

Mat_ৰাশিমালা Mat_ৰাশিমালা::mul(ধ্রুৱ Mat& m, দ্ৱিগুণ scale) ধ্রুৱ
{
    Mat_ৰাশিমালা e;
    op->multiply(*এতত্, Mat_ৰাশিমালা(m), e, scale);
    ঘূৰাই e;
}

Mat_ৰাশিমালা operator + (ধ্রুৱ Mat& a, ধ্রুৱ Mat& b)
{
    checkOperandsExist(a, b);
    Mat_ৰাশিমালা e;
    MatOp_AddEx::make_ৰাশিমালা(e, a, b, 1, 1);
    ঘূৰাই e;
}

Mat_ৰাশিমালা operator + (ধ্রুৱ Mat& a, ধ্রুৱ Scalar& s)
{
    checkOperandsExist(a);
    Mat_ৰাশিমালা e;
    MatOp_AddEx::make_ৰাশিমালা(e, a, Mat(), 1, 0, s);
    ঘূৰাই e;
}

Mat_ৰাশিমালা operator + (ধ্রুৱ Scalar& s, ধ্রুৱ Mat& a)
{
    checkOperandsExist(a);
    Mat_ৰাশিমালা e;
    MatOp_AddEx::make_ৰাশিমালা(e, a, Mat(), 1, 0, s);
    ঘূৰাই e;
}

Mat_ৰাশিমালা operator + (ধ্রুৱ Mat_ৰাশিমালা& e, ধ্রুৱ Mat& m)
{
    Mat_ৰাশিমালা en;
    e.op->add(e, Mat_ৰাশিমালা(m), en);
    ঘূৰাই en;
}

Mat_ৰাশিমালা operator + (ধ্রুৱ Mat& m, ধ্রুৱ Mat_ৰাশিমালা& e)
{
    checkOperandsExist(m);
    Mat_ৰাশিমালা en;
    e.op->add(e, Mat_ৰাশিমালা(m), en);
    ঘূৰাই en;
}

Mat_ৰাশিমালা operator + (ধ্রুৱ Mat_ৰাশিমালা& e, ধ্রুৱ Scalar& s)
{
    Mat_ৰাশিমালা en;
    e.op->add(e, s, en);
    ঘূৰাই en;
}

Mat_ৰাশিমালা operator + (ধ্রুৱ Scalar& s, ধ্রুৱ Mat_ৰাশিমালা& e)
{
    Mat_ৰাশিমালা en;
    e.op->add(e, s, en);
    ঘূৰাই en;
}

Mat_ৰাশিমালা operator + (ধ্রুৱ Mat_ৰাশিমালা& e1, ধ্রুৱ Mat_ৰাশিমালা& e2)
{
    Mat_ৰাশিমালা en;
    e1.op->add(e1, e2, en);
    ঘূৰাই en;
}

Mat_ৰাশিমালা operator - (ধ্রুৱ Mat& a, ধ্রুৱ Mat& b)
{
    checkOperandsExist(a, b);
    Mat_ৰাশিমালা e;
    MatOp_AddEx::make_ৰাশিমালা(e, a, b, 1, -1);
    ঘূৰাই e;
}

Mat_ৰাশিমালা operator - (ধ্রুৱ Mat& a, ধ্রুৱ Scalar& s)
{
    checkOperandsExist(a);
    Mat_ৰাশিমালা e;
    MatOp_AddEx::make_ৰাশিমালা(e, a, Mat(), 1, 0, -s);
    ঘূৰাই e;
}

Mat_ৰাশিমালা operator - (ধ্রুৱ Scalar& s, ধ্রুৱ Mat& a)
{
    checkOperandsExist(a);
    Mat_ৰাশিমালা e;
    MatOp_AddEx::make_ৰাশিমালা(e, a, Mat(), -1, 0, s);
    ঘূৰাই e;
}

Mat_ৰাশিমালা operator - (ধ্রুৱ Mat_ৰাশিমালা& e, ধ্রুৱ Mat& m)
{
    checkOperandsExist(m);
    Mat_ৰাশিমালা en;
    e.op->subtract(e, Mat_ৰাশিমালা(m), en);
    ঘূৰাই en;
}

Mat_ৰাশিমালা operator - (ধ্রুৱ Mat& m, ধ্রুৱ Mat_ৰাশিমালা& e)
{
    checkOperandsExist(m);
    Mat_ৰাশিমালা en;
    e.op->subtract(Mat_ৰাশিমালা(m), e, en);
    ঘূৰাই en;
}

Mat_ৰাশিমালা operator - (ধ্রুৱ Mat_ৰাশিমালা& e, ধ্রুৱ Scalar& s)
{
    Mat_ৰাশিমালা en;
    e.op->add(e, -s, en);
    ঘূৰাই en;
}

Mat_ৰাশিমালা operator - (ধ্রুৱ Scalar& s, ধ্রুৱ Mat_ৰাশিমালা& e)
{
    Mat_ৰাশিমালা en;
    e.op->subtract(s, e, en);
    ঘূৰাই en;
}

Mat_ৰাশিমালা operator - (ধ্রুৱ Mat_ৰাশিমালা& e1, ধ্রুৱ Mat_ৰাশিমালা& e2)
{
    Mat_ৰাশিমালা en;
    e1.op->subtract(e1, e2, en);
    ঘূৰাই en;
}

Mat_ৰাশিমালা operator - (ধ্রুৱ Mat& m)
{
    checkOperandsExist(m);
    Mat_ৰাশিমালা e;
    MatOp_AddEx::make_ৰাশিমালা(e, m, Mat(), -1, 0);
    ঘূৰাই e;
}

Mat_ৰাশিমালা operator - (ধ্রুৱ Mat_ৰাশিমালা& e)
{
    Mat_ৰাশিমালা en;
    e.op->subtract(Scalar(0), e, en);
    ঘূৰাই en;
}

Mat_ৰাশিমালা operator * (ধ্রুৱ Mat& a, ধ্রুৱ Mat& b)
{
    checkOperandsExist(a, b);
    Mat_ৰাশিমালা e;
    MatOp_GEMM::make_ৰাশিমালা(e, 0, a, b);
    ঘূৰাই e;
}

Mat_ৰাশিমালা operator * (ধ্রুৱ Mat& a, দ্ৱিগুণ s)
{
    checkOperandsExist(a);
    Mat_ৰাশিমালা e;
    MatOp_AddEx::make_ৰাশিমালা(e, a, Mat(), s, 0);
    ঘূৰাই e;
}

Mat_ৰাশিমালা operator * (দ্ৱিগুণ s, ধ্রুৱ Mat& a)
{
    checkOperandsExist(a);
    Mat_ৰাশিমালা e;
    MatOp_AddEx::make_ৰাশিমালা(e, a, Mat(), s, 0);
    ঘূৰাই e;
}

Mat_ৰাশিমালা operator * (ধ্রুৱ Mat_ৰাশিমালা& e, ধ্রুৱ Mat& m)
{
    Mat_ৰাশিমালা en;
    e.op->matmul(e, Mat_ৰাশিমালা(m), en);
    ঘূৰাই en;
}

Mat_ৰাশিমালা operator * (ধ্রুৱ Mat& m, ধ্রুৱ Mat_ৰাশিমালা& e)
{
    checkOperandsExist(m);
    Mat_ৰাশিমালা en;
    e.op->matmul(Mat_ৰাশিমালা(m), e, en);
    ঘূৰাই en;
}

Mat_ৰাশিমালা operator * (ধ্রুৱ Mat_ৰাশিমালা& e, দ্ৱিগুণ s)
{
    Mat_ৰাশিমালা en;
    e.op->multiply(e, s, en);
    ঘূৰাই en;
}

Mat_ৰাশিমালা operator * (দ্ৱিগুণ s, ধ্রুৱ Mat_ৰাশিমালা& e)
{
    Mat_ৰাশিমালা en;
    e.op->multiply(e, s, en);
    ঘূৰাই en;
}

Mat_ৰাশিমালা operator * (ধ্রুৱ Mat_ৰাশিমালা& e1, ধ্রুৱ Mat_ৰাশিমালা& e2)
{
    Mat_ৰাশিমালা en;
    e1.op->matmul(e1, e2, en);
    ঘূৰাই en;
}

Mat_ৰাশিমালা operator / (ধ্রুৱ Mat& a, ধ্রুৱ Mat& b)
{
    checkOperandsExist(a, b);
    Mat_ৰাশিমালা e;
    MatOp_Bin::make_ৰাশিমালা(e, '/', a, b);
    ঘূৰাই e;
}

Mat_ৰাশিমালা operator / (ধ্রুৱ Mat& a, দ্ৱিগুণ s)
{
    checkOperandsExist(a);
    Mat_ৰাশিমালা e;
    MatOp_AddEx::make_ৰাশিমালা(e, a, Mat(), 1./s, 0);
    ঘূৰাই e;
}

Mat_ৰাশিমালা operator / (দ্ৱিগুণ s, ধ্রুৱ Mat& a)
{
    checkOperandsExist(a);
    Mat_ৰাশিমালা e;
    MatOp_Bin::make_ৰাশিমালা(e, '/', a, Mat(), s);
    ঘূৰাই e;
}

Mat_ৰাশিমালা operator / (ধ্রুৱ Mat_ৰাশিমালা& e, ধ্রুৱ Mat& m)
{
    Mat_ৰাশিমালা en;
    e.op->divide(e, Mat_ৰাশিমালা(m), en);
    ঘূৰাই en;
}

Mat_ৰাশিমালা operator / (ধ্রুৱ Mat& m, ধ্রুৱ Mat_ৰাশিমালা& e)
{
    checkOperandsExist(m);
    Mat_ৰাশিমালা en;
    e.op->divide(Mat_ৰাশিমালা(m), e, en);
    ঘূৰাই en;
}

Mat_ৰাশিমালা operator / (ধ্রুৱ Mat_ৰাশিমালা& e, দ্ৱিগুণ s)
{
    Mat_ৰাশিমালা en;
    e.op->multiply(e, 1./s, en);
    ঘূৰাই en;
}

Mat_ৰাশিমালা operator / (দ্ৱিগুণ s, ধ্রুৱ Mat_ৰাশিমালা& e)
{
    Mat_ৰাশিমালা en;
    e.op->divide(s, e, en);
    ঘূৰাই en;
}

Mat_ৰাশিমালা operator / (ধ্রুৱ Mat_ৰাশিমালা& e1, ধ্রুৱ Mat_ৰাশিমালা& e2)
{
    Mat_ৰাশিমালা en;
    e1.op->divide(e1, e2, en);
    ঘূৰাই en;
}

Mat_ৰাশিমালা operator < (ধ্রুৱ Mat& a, ধ্রুৱ Mat& b)
{
    checkOperandsExist(a, b);
    Mat_ৰাশিমালা e;
    MatOp_Cmp::make_ৰাশিমালা(e, cv::CMP_LT, a, b);
    ঘূৰাই e;
}

Mat_ৰাশিমালা operator < (ধ্রুৱ Mat& a, দ্ৱিগুণ s)
{
    checkOperandsExist(a);
    Mat_ৰাশিমালা e;
    MatOp_Cmp::make_ৰাশিমালা(e, cv::CMP_LT, a, s);
    ঘূৰাই e;
}

Mat_ৰাশিমালা operator < (দ্ৱিগুণ s, ধ্রুৱ Mat& a)
{
    checkOperandsExist(a);
    Mat_ৰাশিমালা e;
    MatOp_Cmp::make_ৰাশিমালা(e, cv::CMP_GT, a, s);
    ঘূৰাই e;
}

Mat_ৰাশিমালা operator <= (ধ্রুৱ Mat& a, ধ্রুৱ Mat& b)
{
    checkOperandsExist(a, b);
    Mat_ৰাশিমালা e;
    MatOp_Cmp::make_ৰাশিমালা(e, cv::CMP_LE, a, b);
    ঘূৰাই e;
}

Mat_ৰাশিমালা operator <= (ধ্রুৱ Mat& a, দ্ৱিগুণ s)
{
    checkOperandsExist(a);
    Mat_ৰাশিমালা e;
    MatOp_Cmp::make_ৰাশিমালা(e, cv::CMP_LE, a, s);
    ঘূৰাই e;
}

Mat_ৰাশিমালা operator <= (দ্ৱিগুণ s, ধ্রুৱ Mat& a)
{
    checkOperandsExist(a);
    Mat_ৰাশিমালা e;
    MatOp_Cmp::make_ৰাশিমালা(e, cv::CMP_GE, a, s);
    ঘূৰাই e;
}

Mat_ৰাশিমালা operator == (ধ্রুৱ Mat& a, ধ্রুৱ Mat& b)
{
    checkOperandsExist(a, b);
    Mat_ৰাশিমালা e;
    MatOp_Cmp::make_ৰাশিমালা(e, cv::CMP_EQ, a, b);
    ঘূৰাই e;
}

Mat_ৰাশিমালা operator == (ধ্রুৱ Mat& a, দ্ৱিগুণ s)
{
    checkOperandsExist(a);
    Mat_ৰাশিমালা e;
    MatOp_Cmp::make_ৰাশিমালা(e, cv::CMP_EQ, a, s);
    ঘূৰাই e;
}

Mat_ৰাশিমালা operator == (দ্ৱিগুণ s, ধ্রুৱ Mat& a)
{
    checkOperandsExist(a);
    Mat_ৰাশিমালা e;
    MatOp_Cmp::make_ৰাশিমালা(e, cv::CMP_EQ, a, s);
    ঘূৰাই e;
}

Mat_ৰাশিমালা operator != (ধ্রুৱ Mat& a, ধ্রুৱ Mat& b)
{
    checkOperandsExist(a, b);
    Mat_ৰাশিমালা e;
    MatOp_Cmp::make_ৰাশিমালা(e, cv::CMP_NE, a, b);
    ঘূৰাই e;
}

Mat_ৰাশিমালা operator != (ধ্রুৱ Mat& a, দ্ৱিগুণ s)
{
    checkOperandsExist(a);
    Mat_ৰাশিমালা e;
    MatOp_Cmp::make_ৰাশিমালা(e, cv::CMP_NE, a, s);
    ঘূৰাই e;
}

Mat_ৰাশিমালা operator != (দ্ৱিগুণ s, ধ্রুৱ Mat& a)
{
    checkOperandsExist(a);
    Mat_ৰাশিমালা e;
    MatOp_Cmp::make_ৰাশিমালা(e, cv::CMP_NE, a, s);
    ঘূৰাই e;
}

Mat_ৰাশিমালা operator >= (ধ্রুৱ Mat& a, ধ্রুৱ Mat& b)
{
    checkOperandsExist(a, b);
    Mat_ৰাশিমালা e;
    MatOp_Cmp::make_ৰাশিমালা(e, cv::CMP_GE, a, b);
    ঘূৰাই e;
}

Mat_ৰাশিমালা operator >= (ধ্রুৱ Mat& a, দ্ৱিগুণ s)
{
    checkOperandsExist(a);
    Mat_ৰাশিমালা e;
    MatOp_Cmp::make_ৰাশিমালা(e, cv::CMP_GE, a, s);
    ঘূৰাই e;
}

Mat_ৰাশিমালা operator >= (দ্ৱিগুণ s, ধ্রুৱ Mat& a)
{
    checkOperandsExist(a);
    Mat_ৰাশিমালা e;
    MatOp_Cmp::make_ৰাশিমালা(e, cv::CMP_LE, a, s);
    ঘূৰাই e;
}

Mat_ৰাশিমালা operator > (ধ্রুৱ Mat& a, ধ্রুৱ Mat& b)
{
    checkOperandsExist(a, b);
    Mat_ৰাশিমালা e;
    MatOp_Cmp::make_ৰাশিমালা(e, cv::CMP_GT, a, b);
    ঘূৰাই e;
}

Mat_ৰাশিমালা operator > (ধ্রুৱ Mat& a, দ্ৱিগুণ s)
{
    checkOperandsExist(a);
    Mat_ৰাশিমালা e;
    MatOp_Cmp::make_ৰাশিমালা(e, cv::CMP_GT, a, s);
    ঘূৰাই e;
}

Mat_ৰাশিমালা operator > (দ্ৱিগুণ s, ধ্রুৱ Mat& a)
{
    checkOperandsExist(a);
    Mat_ৰাশিমালা e;
    MatOp_Cmp::make_ৰাশিমালা(e, cv::CMP_LT, a, s);
    ঘূৰাই e;
}

Mat_ৰাশিমালা min(ধ্রুৱ Mat& a, ধ্রুৱ Mat& b)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a, b);
    Mat_ৰাশিমালা e;
    MatOp_Bin::make_ৰাশিমালা(e, 'm', a, b);
    ঘূৰাই e;
}

Mat_ৰাশিমালা min(ধ্রুৱ Mat& a, দ্ৱিগুণ s)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    Mat_ৰাশিমালা e;
    MatOp_Bin::make_ৰাশিমালা(e, 'n', a, s);
    ঘূৰাই e;
}

Mat_ৰাশিমালা min(দ্ৱিগুণ s, ধ্রুৱ Mat& a)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    Mat_ৰাশিমালা e;
    MatOp_Bin::make_ৰাশিমালা(e, 'n', a, s);
    ঘূৰাই e;
}

Mat_ৰাশিমালা max(ধ্রুৱ Mat& a, ধ্রুৱ Mat& b)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a, b);
    Mat_ৰাশিমালা e;
    MatOp_Bin::make_ৰাশিমালা(e, 'M', a, b);
    ঘূৰাই e;
}

Mat_ৰাশিমালা max(ধ্রুৱ Mat& a, দ্ৱিগুণ s)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    Mat_ৰাশিমালা e;
    MatOp_Bin::make_ৰাশিমালা(e, 'N', a, s);
    ঘূৰাই e;
}

Mat_ৰাশিমালা max(দ্ৱিগুণ s, ধ্রুৱ Mat& a)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    Mat_ৰাশিমালা e;
    MatOp_Bin::make_ৰাশিমালা(e, 'N', a, s);
    ঘূৰাই e;
}

Mat_ৰাশিমালা operator & (ধ্রুৱ Mat& a, ধ্রুৱ Mat& b)
{
    checkOperandsExist(a, b);
    Mat_ৰাশিমালা e;
    MatOp_Bin::make_ৰাশিমালা(e, '&', a, b);
    ঘূৰাই e;
}

Mat_ৰাশিমালা operator & (ধ্রুৱ Mat& a, ধ্রুৱ Scalar& s)
{
    checkOperandsExist(a);
    Mat_ৰাশিমালা e;
    MatOp_Bin::make_ৰাশিমালা(e, '&', a, s);
    ঘূৰাই e;
}

Mat_ৰাশিমালা operator & (ধ্রুৱ Scalar& s, ধ্রুৱ Mat& a)
{
    checkOperandsExist(a);
    Mat_ৰাশিমালা e;
    MatOp_Bin::make_ৰাশিমালা(e, '&', a, s);
    ঘূৰাই e;
}

Mat_ৰাশিমালা operator | (ধ্রুৱ Mat& a, ধ্রুৱ Mat& b)
{
    checkOperandsExist(a, b);
    Mat_ৰাশিমালা e;
    MatOp_Bin::make_ৰাশিমালা(e, '|', a, b);
    ঘূৰাই e;
}

Mat_ৰাশিমালা operator | (ধ্রুৱ Mat& a, ধ্রুৱ Scalar& s)
{
    checkOperandsExist(a);
    Mat_ৰাশিমালা e;
    MatOp_Bin::make_ৰাশিমালা(e, '|', a, s);
    ঘূৰাই e;
}

Mat_ৰাশিমালা operator | (ধ্রুৱ Scalar& s, ধ্রুৱ Mat& a)
{
    checkOperandsExist(a);
    Mat_ৰাশিমালা e;
    MatOp_Bin::make_ৰাশিমালা(e, '|', a, s);
    ঘূৰাই e;
}

Mat_ৰাশিমালা operator ^ (ধ্রুৱ Mat& a, ধ্রুৱ Mat& b)
{
    checkOperandsExist(a, b);
    Mat_ৰাশিমালা e;
    MatOp_Bin::make_ৰাশিমালা(e, '^', a, b);
    ঘূৰাই e;
}

Mat_ৰাশিমালা operator ^ (ধ্রুৱ Mat& a, ধ্রুৱ Scalar& s)
{
    checkOperandsExist(a);
    Mat_ৰাশিমালা e;
    MatOp_Bin::make_ৰাশিমালা(e, '^', a, s);
    ঘূৰাই e;
}

Mat_ৰাশিমালা operator ^ (ধ্রুৱ Scalar& s, ধ্রুৱ Mat& a)
{
    checkOperandsExist(a);
    Mat_ৰাশিমালা e;
    MatOp_Bin::make_ৰাশিমালা(e, '^', a, s);
    ঘূৰাই e;
}

Mat_ৰাশিমালা operator ~(ধ্রুৱ Mat& a)
{
    checkOperandsExist(a);
    Mat_ৰাশিমালা e;
    MatOp_Bin::make_ৰাশিমালা(e, '~', a, Scalar());
    ঘূৰাই e;
}

Mat_ৰাশিমালা abs(ধ্রুৱ Mat& a)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    Mat_ৰাশিমালা e;
    MatOp_Bin::make_ৰাশিমালা(e, 'a', a, Scalar());
    ঘূৰাই e;
}

Mat_ৰাশিমালা abs(ধ্রুৱ Mat_ৰাশিমালা& e)
{
    CV_INSTRUMENT_REGION();

    Mat_ৰাশিমালা en;
    e.op->abs(e, en);
    ঘূৰাই en;
}


Size Mat_ৰাশিমালা::size() ধ্রুৱ
{
    যদি( isT(*এতত্) || isInv(*এতত্) )
        ঘূৰাই Size(a.rows, a.cols);
    যদি( isSolve(*এতত্) )
        ঘূৰাই Size(b.cols, a.cols);
    যদি( isInitializer(*এতত্) )
        ঘূৰাই a.size();
    ঘূৰাই op ? op->size(*এতত্) : Size();
}


পূর্ণাঙ্ক Mat_ৰাশিমালা::type() ধ্রুৱ
{
    CV_INSTRUMENT_REGION();

    যদি( isInitializer(*এতত্) )
        ঘূৰাই a.type();
    যদি( isCmp(*এতত্) )
        ঘূৰাই CV_MAKETYPE(CV_8U, a.channels());
    ঘূৰাই op ? op->type(*এতত্) : -1;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////

শূন্য MatOp_Identity::assign(ধ্রুৱ Mat_ৰাশিমালা& e, Mat& m, পূর্ণাঙ্ক _type) ধ্রুৱ
{
    যদি( _type == -1 || _type == e.a.type() )
        m = e.a;
    অন্যথা
    {
        CV_Assert( CV_MAT_CN(_type) == e.a.channels() );
        e.a.convertTo(m, _type);
    }
}

অন্তঃস্থ শূন্য MatOp_Identity::make_ৰাশিমালা(Mat_ৰাশিমালা& res, ধ্রুৱ Mat& m)
{
    res = Mat_ৰাশিমালা(&g_MatOp_Identity, 0, m, Mat(), Mat(), 1, 0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

শূন্য MatOp_AddEx::assign(ধ্রুৱ Mat_ৰাশিমালা& e, Mat& m, পূর্ণাঙ্ক _type) ধ্রুৱ
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
        ঘূৰাই;
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


শূন্য MatOp_AddEx::add(ধ্রুৱ Mat_ৰাশিমালা& e, ধ্রুৱ Scalar& s, Mat_ৰাশিমালা& res) ধ্রুৱ
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.s += s;
}


শূন্য MatOp_AddEx::subtract(ধ্রুৱ Scalar& s, ধ্রুৱ Mat_ৰাশিমালা& e, Mat_ৰাশিমালা& res) ধ্রুৱ
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha = -res.alpha;
    res.beta = -res.beta;
    res.s = s - res.s;
}

শূন্য MatOp_AddEx::multiply(ধ্রুৱ Mat_ৰাশিমালা& e, দ্ৱিগুণ s, Mat_ৰাশিমালা& res) ধ্রুৱ
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha *= s;
    res.beta *= s;
    res.s *= s;
}

শূন্য MatOp_AddEx::divide(দ্ৱিগুণ s, ধ্রুৱ Mat_ৰাশিমালা& e, Mat_ৰাশিমালা& res) ধ্রুৱ
{
    CV_INSTRUMENT_REGION();

    যদি( isScaled(e) )
        MatOp_Bin::make_ৰাশিমালা(res, '/', e.a, Mat(), s/e.alpha);
    অন্যথা
        MatOp::divide(s, e, res);
}


শূন্য MatOp_AddEx::transpose(ধ্রুৱ Mat_ৰাশিমালা& e, Mat_ৰাশিমালা& res) ধ্রুৱ
{
    CV_INSTRUMENT_REGION();

    যদি( isScaled(e) )
        MatOp_T::make_ৰাশিমালা(res, e.a, e.alpha);
    অন্যথা
        MatOp::transpose(e, res);
}

শূন্য MatOp_AddEx::abs(ধ্রুৱ Mat_ৰাশিমালা& e, Mat_ৰাশিমালা& res) ধ্রুৱ
{
    CV_INSTRUMENT_REGION();

    যদি( (!e.b.data || e.beta == 0) && fabs(e.alpha) == 1 )
        MatOp_Bin::make_ৰাশিমালা(res, 'a', e.a, -e.s*e.alpha);
    অন্যথা যদি( e.b.data && e.alpha + e.beta == 0 && e.alpha*e.beta == -1 )
        MatOp_Bin::make_ৰাশিমালা(res, 'a', e.a, e.b);
    অন্যথা
        MatOp::abs(e, res);
}

অন্তঃস্থ শূন্য MatOp_AddEx::make_ৰাশিমালা(Mat_ৰাশিমালা& res, ধ্রুৱ Mat& a, ধ্রুৱ Mat& b, দ্ৱিগুণ alpha, দ্ৱিগুণ beta, ধ্রুৱ Scalar& s)
{
    res = Mat_ৰাশিমালা(&g_MatOp_AddEx, 0, a, b, Mat(), alpha, beta, s);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

শূন্য MatOp_Bin::assign(ধ্রুৱ Mat_ৰাশিমালা& e, Mat& m, পূর্ণাঙ্ক _type) ধ্রুৱ
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

শূন্য MatOp_Bin::multiply(ধ্রুৱ Mat_ৰাশিমালা& e, দ্ৱিগুণ s, Mat_ৰাশিমালা& res) ধ্রুৱ
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

শূন্য MatOp_Bin::divide(দ্ৱিগুণ s, ধ্রুৱ Mat_ৰাশিমালা& e, Mat_ৰাশিমালা& res) ধ্রুৱ
{
    CV_INSTRUMENT_REGION();

    যদি( e.flags == '/' && (!e.b.data || e.beta == 0) )
        MatOp_AddEx::make_ৰাশিমালা(res, e.a, Mat(), s/e.alpha, 0);
    অন্যথা
        MatOp::divide(s, e, res);
}

অন্তঃস্থ শূন্য MatOp_Bin::make_ৰাশিমালা(Mat_ৰাশিমালা& res, ৱর্ণ op, ধ্রুৱ Mat& a, ধ্রুৱ Mat& b, দ্ৱিগুণ scale)
{
    res = Mat_ৰাশিমালা(&g_MatOp_Bin, op, a, b, Mat(), scale, b.data ? 1 : 0);
}

অন্তঃস্থ শূন্য MatOp_Bin::make_ৰাশিমালা(Mat_ৰাশিমালা& res, ৱর্ণ op, ধ্রুৱ Mat& a, ধ্রুৱ Scalar& s)
{
    res = Mat_ৰাশিমালা(&g_MatOp_Bin, op, a, Mat(), Mat(), 1, 0, s);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

শূন্য MatOp_Cmp::assign(ধ্রুৱ Mat_ৰাশিমালা& e, Mat& m, পূর্ণাঙ্ক _type) ধ্রুৱ
{
    Mat temp, &dst = _type == -1 || _type == CV_8U ? m : temp;

    যদি( e.b.data )
        cv::compare(e.a, e.b, dst, e.flags);
    অন্যথা
        cv::compare(e.a, e.alpha, dst, e.flags);

    যদি( dst.data != m.data )
        dst.convertTo(m, _type);
}

অন্তঃস্থ শূন্য MatOp_Cmp::make_ৰাশিমালা(Mat_ৰাশিমালা& res, পূর্ণাঙ্ক cmpop, ধ্রুৱ Mat& a, ধ্রুৱ Mat& b)
{
    res = Mat_ৰাশিমালা(&g_MatOp_Cmp, cmpop, a, b, Mat(), 1, 1);
}

অন্তঃস্থ শূন্য MatOp_Cmp::make_ৰাশিমালা(Mat_ৰাশিমালা& res, পূর্ণাঙ্ক cmpop, ধ্রুৱ Mat& a, দ্ৱিগুণ alpha)
{
    res = Mat_ৰাশিমালা(&g_MatOp_Cmp, cmpop, a, Mat(), Mat(), alpha, 1);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

শূন্য MatOp_T::assign(ধ্রুৱ Mat_ৰাশিমালা& e, Mat& m, পূর্ণাঙ্ক _type) ধ্রুৱ
{
    Mat temp, &dst = _type == -1 || _type == e.a.type() ? m : temp;

    cv::transpose(e.a, dst);

    যদি( dst.data != m.data || e.alpha != 1 )
        dst.convertTo(m, _type, e.alpha);
}

শূন্য MatOp_T::multiply(ধ্রুৱ Mat_ৰাশিমালা& e, দ্ৱিগুণ s, Mat_ৰাশিমালা& res) ধ্রুৱ
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha *= s;
}

শূন্য MatOp_T::transpose(ধ্রুৱ Mat_ৰাশিমালা& e, Mat_ৰাশিমালা& res) ধ্রুৱ
{
    CV_INSTRUMENT_REGION();

    যদি( e.alpha == 1 )
        MatOp_Identity::make_ৰাশিমালা(res, e.a);
    অন্যথা
        MatOp_AddEx::make_ৰাশিমালা(res, e.a, Mat(), e.alpha, 0);
}

অন্তঃস্থ শূন্য MatOp_T::make_ৰাশিমালা(Mat_ৰাশিমালা& res, ধ্রুৱ Mat& a, দ্ৱিগুণ alpha)
{
    res = Mat_ৰাশিমালা(&g_MatOp_T, 0, a, Mat(), Mat(), alpha, 0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

শূন্য MatOp_GEMM::assign(ধ্রুৱ Mat_ৰাশিমালা& e, Mat& m, পূর্ণাঙ্ক _type) ধ্রুৱ
{
    Mat temp, &dst = _type == -1 || _type == e.a.type() ? m : temp;

    cv::gemm(e.a, e.b, e.alpha, e.c, e.beta, dst, e.flags);
    যদি( dst.data != m.data )
        dst.convertTo(m, _type);
}

শূন্য MatOp_GEMM::add(ধ্রুৱ Mat_ৰাশিমালা& e1, ধ্রুৱ Mat_ৰাশিমালা& e2, Mat_ৰাশিমালা& res) ধ্রুৱ
{
    CV_INSTRUMENT_REGION();

    তার্কিক i1 = isIdentity(e1), i2 = isIdentity(e2);
    দ্ৱিগুণ alpha1 = i1 ? 1 : e1.alpha, alpha2 = i2 ? 1 : e2.alpha;

    যদি( isMatProd(e1) && (i2 || isScaled(e2) || isT(e2)) )
        MatOp_GEMM::make_ৰাশিমালা(res, (e1.flags & ~cv::GEMM_3_T)|(isT(e2) ? cv::GEMM_3_T : 0),
                             e1.a, e1.b, alpha1, e2.a, alpha2);
    অন্যথা যদি( isMatProd(e2) && (i1 || isScaled(e1) || isT(e1)) )
        MatOp_GEMM::make_ৰাশিমালা(res, (e2.flags & ~cv::GEMM_3_T)|(isT(e1) ? cv::GEMM_3_T : 0),
                             e2.a, e2.b, alpha2, e1.a, alpha1);
    অন্যথা যদি( এতত্ == e2.op )
        MatOp::add(e1, e2, res);
    অন্যথা
        e2.op->add(e1, e2, res);
}

শূন্য MatOp_GEMM::subtract(ধ্রুৱ Mat_ৰাশিমালা& e1, ধ্রুৱ Mat_ৰাশিমালা& e2, Mat_ৰাশিমালা& res) ধ্রুৱ
{
    CV_INSTRUMENT_REGION();

    তার্কিক i1 = isIdentity(e1), i2 = isIdentity(e2);
    দ্ৱিগুণ alpha1 = i1 ? 1 : e1.alpha, alpha2 = i2 ? 1 : e2.alpha;

    যদি( isMatProd(e1) && (i2 || isScaled(e2) || isT(e2)) )
        MatOp_GEMM::make_ৰাশিমালা(res, (e1.flags & ~cv::GEMM_3_T)|(isT(e2) ? cv::GEMM_3_T : 0),
                             e1.a, e1.b, alpha1, e2.a, -alpha2);
    অন্যথা যদি( isMatProd(e2) && (i1 || isScaled(e1) || isT(e1)) )
        MatOp_GEMM::make_ৰাশিমালা(res, (e2.flags & ~cv::GEMM_3_T)|(isT(e1) ? cv::GEMM_3_T : 0),
                            e2.a, e2.b, -alpha2, e1.a, alpha1);
    অন্যথা যদি( এতত্ == e2.op )
        MatOp::subtract(e1, e2, res);
    অন্যথা
        e2.op->subtract(e1, e2, res);
}

শূন্য MatOp_GEMM::multiply(ধ্রুৱ Mat_ৰাশিমালা& e, দ্ৱিগুণ s, Mat_ৰাশিমালা& res) ধ্রুৱ
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha *= s;
    res.beta *= s;
}

শূন্য MatOp_GEMM::transpose(ধ্রুৱ Mat_ৰাশিমালা& e, Mat_ৰাশিমালা& res) ধ্রুৱ
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.flags = (!(e.flags & cv::GEMM_1_T) ? cv::GEMM_2_T : 0) |
                (!(e.flags & cv::GEMM_2_T) ? cv::GEMM_1_T : 0) |
                (!(e.flags & cv::GEMM_3_T) ? cv::GEMM_3_T : 0);
    swap(res.a, res.b);
}

অন্তঃস্থ শূন্য MatOp_GEMM::make_ৰাশিমালা(Mat_ৰাশিমালা& res, পূর্ণাঙ্ক flags, ধ্রুৱ Mat& a, ধ্রুৱ Mat& b,
                                 দ্ৱিগুণ alpha, ধ্রুৱ Mat& c, দ্ৱিগুণ beta)
{
    res = Mat_ৰাশিমালা(&g_MatOp_GEMM, flags, a, b, c, alpha, beta);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

শূন্য MatOp_Invert::assign(ধ্রুৱ Mat_ৰাশিমালা& e, Mat& m, পূর্ণাঙ্ক _type) ধ্রুৱ
{
    Mat temp, &dst = _type == -1 || _type == e.a.type() ? m : temp;

    cv::invert(e.a, dst, e.flags);
    যদি( dst.data != m.data )
        dst.convertTo(m, _type);
}

শূন্য MatOp_Invert::matmul(ধ্রুৱ Mat_ৰাশিমালা& e1, ধ্রুৱ Mat_ৰাশিমালা& e2, Mat_ৰাশিমালা& res) ধ্রুৱ
{
    যদি( isInv(e1) && isIdentity(e2) )
        MatOp_Solve::make_ৰাশিমালা(res, e1.flags, e1.a, e2.a);
    অন্যথা যদি( এতত্ == e2.op )
        MatOp::matmul(e1, e2, res);
    অন্যথা
        e2.op->matmul(e1, e2, res);
}

অন্তঃস্থ শূন্য MatOp_Invert::make_ৰাশিমালা(Mat_ৰাশিমালা& res, পূর্ণাঙ্ক method, ধ্রুৱ Mat& m)
{
    res = Mat_ৰাশিমালা(&g_MatOp_Invert, method, m, Mat(), Mat(), 1, 0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

শূন্য MatOp_Solve::assign(ধ্রুৱ Mat_ৰাশিমালা& e, Mat& m, পূর্ণাঙ্ক _type) ধ্রুৱ
{
    Mat temp, &dst = _type == -1 || _type == e.a.type() ? m : temp;

    cv::solve(e.a, e.b, dst, e.flags);
    যদি( dst.data != m.data )
        dst.convertTo(m, _type);
}

অন্তঃস্থ শূন্য MatOp_Solve::make_ৰাশিমালা(Mat_ৰাশিমালা& res, পূর্ণাঙ্ক method, ধ্রুৱ Mat& a, ধ্রুৱ Mat& b)
{
    res = Mat_ৰাশিমালা(&g_MatOp_Solve, method, a, b, Mat(), 1, 1);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

শূন্য MatOp_Initializer::assign(ধ্রুৱ Mat_ৰাশিমালা& e, Mat& m, পূর্ণাঙ্ক _type) ধ্রুৱ
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

শূন্য MatOp_Initializer::multiply(ধ্রুৱ Mat_ৰাশিমালা& e, দ্ৱিগুণ s, Mat_ৰাশিমালা& res) ধ্রুৱ
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha *= s;
}

অন্তঃস্থ শূন্য MatOp_Initializer::make_ৰাশিমালা(Mat_ৰাশিমালা& res, পূর্ণাঙ্ক method, Size sz, পূর্ণাঙ্ক type, দ্ৱিগুণ alpha)
{
    res = Mat_ৰাশিমালা(getGlobalMatOpInitializer(), method, Mat(sz, type, (শূন্য*)(size_t)0xEEEEEEEE), Mat(), Mat(), alpha, 0);
}

অন্তঃস্থ শূন্য MatOp_Initializer::make_ৰাশিমালা(Mat_ৰাশিমালা& res, পূর্ণাঙ্ক method, পূর্ণাঙ্ক ndims, ধ্রুৱ পূর্ণাঙ্ক* sizes, পূর্ণাঙ্ক type, দ্ৱিগুণ alpha)
{
    res = Mat_ৰাশিমালা(getGlobalMatOpInitializer(), method, Mat(ndims, sizes, type, (শূন্য*)(size_t)0xEEEEEEEE), Mat(), Mat(), alpha, 0);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

Mat_ৰাশিমালা Mat::t() ধ্রুৱ
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(*এতত্);
    Mat_ৰাশিমালা e;
    MatOp_T::make_ৰাশিমালা(e, *এতত্);
    ঘূৰাই e;
}

Mat_ৰাশিমালা Mat::inv(পূর্ণাঙ্ক method) ধ্রুৱ
{
    CV_INSTRUMENT_REGION();

    Mat_ৰাশিমালা e;
    MatOp_Invert::make_ৰাশিমালা(e, method, *এতত্);
    ঘূৰাই e;
}


Mat_ৰাশিমালা Mat::mul(প্ৰবেশ_Array m, দ্ৱিগুণ scale) ধ্রুৱ
{
    CV_INSTRUMENT_REGION();

    Mat b = m.getMat();
    // Unless the argument is a refcounted Mat/UMat, the header returned by getMat() may be
    // a non-owning view of caller memory (e.g. a scalar bound to _InputArray(const double&),
    // a Matx or a Vec) that the returned MatExpr can outlive, so snapshot it.
    // See https://github.com/opencv/opencv/issues/23577
    যদি( !m.isMat() && !m.isUMat() )
        b = b.clone();

    Mat_ৰাশিমালা e;
    MatOp_Bin::make_ৰাশিমালা(e, '*', *এতত্, b, scale);
    ঘূৰাই e;
}

Mat_ৰাশিমালা Mat::zeros(পূর্ণাঙ্ক rows, পূর্ণাঙ্ক cols, পূর্ণাঙ্ক type)
{
    CV_INSTRUMENT_REGION();

    Mat_ৰাশিমালা e;
    MatOp_Initializer::make_ৰাশিমালা(e, '0', Size(cols, rows), type);
    ঘূৰাই e;
}

Mat_ৰাশিমালা Mat::zeros(Size size, পূর্ণাঙ্ক type)
{
    CV_INSTRUMENT_REGION();

    Mat_ৰাশিমালা e;
    MatOp_Initializer::make_ৰাশিমালা(e, '0', size, type);
    ঘূৰাই e;
}

Mat_ৰাশিমালা Mat::zeros(পূর্ণাঙ্ক ndims, ধ্রুৱ পূর্ণাঙ্ক* sizes, পূর্ণাঙ্ক type)
{
    CV_INSTRUMENT_REGION();

    Mat_ৰাশিমালা e;
    MatOp_Initializer::make_ৰাশিমালা(e, '0', ndims, sizes, type);
    ঘূৰাই e;
}

Mat_ৰাশিমালা Mat::zeros(ধ্রুৱ MatShape& shape, পূর্ণাঙ্ক type)
{
    CV_INSTRUMENT_REGION();

    Mat_ৰাশিমালা e;
    MatOp_Initializer::make_ৰাশিমালা(e, '0', shape.dims, shape.p, type);
    ঘূৰাই e;
}

Mat_ৰাশিমালা Mat::ones(পূর্ণাঙ্ক rows, পূর্ণাঙ্ক cols, পূর্ণাঙ্ক type)
{
    CV_INSTRUMENT_REGION();

    Mat_ৰাশিমালা e;
    MatOp_Initializer::make_ৰাশিমালা(e, '1', Size(cols, rows), type);
    ঘূৰাই e;
}

Mat_ৰাশিমালা Mat::ones(Size size, পূর্ণাঙ্ক type)
{
    CV_INSTRUMENT_REGION();

    Mat_ৰাশিমালা e;
    MatOp_Initializer::make_ৰাশিমালা(e, '1', size, type);
    ঘূৰাই e;
}

Mat_ৰাশিমালা Mat::ones(পূর্ণাঙ্ক ndims, ধ্রুৱ পূর্ণাঙ্ক* sizes, পূর্ণাঙ্ক type)
{
    CV_INSTRUMENT_REGION();

    Mat_ৰাশিমালা e;
    MatOp_Initializer::make_ৰাশিমালা(e, '1', ndims, sizes, type);
    ঘূৰাই e;
}

Mat_ৰাশিমালা Mat::ones(ধ্রুৱ MatShape& shape, পূর্ণাঙ্ক type)
{
    CV_INSTRUMENT_REGION();

    Mat_ৰাশিমালা e;
    MatOp_Initializer::make_ৰাশিমালা(e, '1', shape.dims, shape.p, type);
    ঘূৰাই e;
}

Mat_ৰাশিমালা Mat::eye(পূর্ণাঙ্ক rows, পূর্ণাঙ্ক cols, পূর্ণাঙ্ক type)
{
    CV_INSTRUMENT_REGION();

    Mat_ৰাশিমালা e;
    MatOp_Initializer::make_ৰাশিমালা(e, 'I', Size(cols, rows), type);
    ঘূৰাই e;
}

Mat_ৰাশিমালা Mat::eye(Size size, পূর্ণাঙ্ক type)
{
    CV_INSTRUMENT_REGION();

    Mat_ৰাশিমালা e;
    MatOp_Initializer::make_ৰাশিমালা(e, 'I', size, type);
    ঘূৰাই e;
}

শূন্য Mat_ৰাশিমালা::swap(Mat_ৰাশিমালা& other)
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

প্ৰবেশ_Array_2::প্ৰবেশ_Array_2(ধ্রুৱ Mat_ৰাশিমালা& ৰাশিমালা)
{
    যদি (!isIdentity(ৰাশিমালা))
    {
        Mat result = ৰাশিমালা;  // TODO improve through refcount == 1 of expr.a (inplace operation is possible - except gemm?)
        Mat_ৰাশিমালা result_ৰাশিমালা(result);
        swap(const_cast<Mat_ৰাশিমালা&>(ৰাশিমালা), result_ৰাশিমালা);
    }
    CV_Assert(isIdentity(ৰাশিমালা));
    init(FIXED_TYPE + FIXED_SIZE + MAT + ACCESS_READ, &ৰাশিমালা.a);
}

} // cv::
