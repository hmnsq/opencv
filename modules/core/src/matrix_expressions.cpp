// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


/* ////////////////////////////////////////////////////////////////////
//
//  Mat basic operations: Copy, Set
//
// */

#شامل "precomp.hpp"
#شامل <opencv2/core/utils/logger.hpp>

فضاینام cv
{

//This and its overload below are used in various MatExpr operator overloads
//implemented to check that Matrix operands exist.
ایستا تهی checkOperandsExist(ثابت Mat& a)
{
    اگر (a.empty())
    {
        CV_Error(cv::Error::StsBadArg, "Matrix operand is an empty matrix.");
    }
}

ایستا تهی checkOperandsExist(ثابت Mat& a, ثابت Mat& b)
{
    اگر (a.empty() || b.empty())
    {
        CV_Error(cv::Error::StsBadArg, "One or more matrix operands are empty.");
    }
}


رده MatOp_Identity CV_FINAL : عمومی MatOp
{
عمومی:
    MatOp_Identity() {}
    مجازی ~MatOp_Identity() {}

    منطقی elementWise(ثابت Mat_عبارت& /*عبارت*/) ثابت CV_OVERRIDE { بازگشت درست; }
    تهی assign(ثابت Mat_عبارت& عبارت, Mat& m, صحیح type=-1) ثابت CV_OVERRIDE;

    ایستا تهی make_عبارت(Mat_عبارت& res, ثابت Mat& m);
};

ایستا MatOp_Identity g_MatOp_Identity;

رده MatOp_AddEx CV_FINAL : عمومی MatOp
{
عمومی:
    MatOp_AddEx() {}
    مجازی ~MatOp_AddEx() {}

    منطقی elementWise(ثابت Mat_عبارت& /*عبارت*/) ثابت CV_OVERRIDE { بازگشت درست; }
    تهی assign(ثابت Mat_عبارت& عبارت, Mat& m, صحیح type=-1) ثابت CV_OVERRIDE;

    تهی add(ثابت Mat_عبارت& e1, ثابت Scalar& s, Mat_عبارت& res) ثابت CV_OVERRIDE;
    تهی subtract(ثابت Scalar& s, ثابت Mat_عبارت& عبارت, Mat_عبارت& res) ثابت CV_OVERRIDE;
    تهی multiply(ثابت Mat_عبارت& e1, دوگانه s, Mat_عبارت& res) ثابت CV_OVERRIDE;
    تهی divide(دوگانه s, ثابت Mat_عبارت& e, Mat_عبارت& res) ثابت CV_OVERRIDE;

    تهی transpose(ثابت Mat_عبارت& e1, Mat_عبارت& res) ثابت CV_OVERRIDE;
    تهی abs(ثابت Mat_عبارت& عبارت, Mat_عبارت& res) ثابت CV_OVERRIDE;

    ایستا تهی make_عبارت(Mat_عبارت& res, ثابت Mat& a, ثابت Mat& b, دوگانه alpha, دوگانه beta, ثابت Scalar& s=Scalar());
};

ایستا MatOp_AddEx g_MatOp_AddEx;

رده MatOp_Bin CV_FINAL : عمومی MatOp
{
عمومی:
    MatOp_Bin() {}
    مجازی ~MatOp_Bin() {}

    منطقی elementWise(ثابت Mat_عبارت& /*عبارت*/) ثابت CV_OVERRIDE { بازگشت درست; }
    تهی assign(ثابت Mat_عبارت& عبارت, Mat& m, صحیح type=-1) ثابت CV_OVERRIDE;

    تهی multiply(ثابت Mat_عبارت& e1, دوگانه s, Mat_عبارت& res) ثابت CV_OVERRIDE;
    تهی divide(دوگانه s, ثابت Mat_عبارت& e, Mat_عبارت& res) ثابت CV_OVERRIDE;

    ایستا تهی make_عبارت(Mat_عبارت& res, نویسه op, ثابت Mat& a, ثابت Mat& b, دوگانه scale=1);
    ایستا تهی make_عبارت(Mat_عبارت& res, نویسه op, ثابت Mat& a, ثابت Scalar& s);
};

ایستا MatOp_Bin g_MatOp_Bin;

رده MatOp_Cmp CV_FINAL : عمومی MatOp
{
عمومی:
    MatOp_Cmp() {}
    مجازی ~MatOp_Cmp() {}

    منطقی elementWise(ثابت Mat_عبارت& /*عبارت*/) ثابت CV_OVERRIDE { بازگشت درست; }
    تهی assign(ثابت Mat_عبارت& عبارت, Mat& m, صحیح type=-1) ثابت CV_OVERRIDE;

    ایستا تهی make_عبارت(Mat_عبارت& res, صحیح cmpop, ثابت Mat& a, ثابت Mat& b);
    ایستا تهی make_عبارت(Mat_عبارت& res, صحیح cmpop, ثابت Mat& a, دوگانه alpha);
};

ایستا MatOp_Cmp g_MatOp_Cmp;

رده MatOp_GEMM CV_FINAL : عمومی MatOp
{
عمومی:
    MatOp_GEMM() {}
    مجازی ~MatOp_GEMM() {}

    منطقی elementWise(ثابت Mat_عبارت& /*عبارت*/) ثابت CV_OVERRIDE { بازگشت نادرست; }
    تهی assign(ثابت Mat_عبارت& عبارت, Mat& m, صحیح type=-1) ثابت CV_OVERRIDE;

    تهی add(ثابت Mat_عبارت& e1, ثابت Mat_عبارت& e2, Mat_عبارت& res) ثابت CV_OVERRIDE;
    تهی subtract(ثابت Mat_عبارت& e1, ثابت Mat_عبارت& e2, Mat_عبارت& res) ثابت CV_OVERRIDE;
    تهی multiply(ثابت Mat_عبارت& e, دوگانه s, Mat_عبارت& res) ثابت CV_OVERRIDE;

    تهی transpose(ثابت Mat_عبارت& عبارت, Mat_عبارت& res) ثابت CV_OVERRIDE;

    Size size(ثابت Mat_عبارت& عبارت) ثابت CV_OVERRIDE
    {
        بازگشت Size(
            (عبارت.flags & GEMM_2_T) ? عبارت.b.rows : عبارت.b.cols,
            (عبارت.flags & GEMM_1_T) ? عبارت.a.cols : عبارت.a.rows
        );
    }

    ایستا تهی make_عبارت(Mat_عبارت& res, صحیح flags, ثابت Mat& a, ثابت Mat& b,
                         دوگانه alpha=1, ثابت Mat& c=Mat(), دوگانه beta=1);

};

ایستا MatOp_GEMM g_MatOp_GEMM;

رده MatOp_Invert CV_FINAL : عمومی MatOp
{
عمومی:
    MatOp_Invert() {}
    مجازی ~MatOp_Invert() {}

    منطقی elementWise(ثابت Mat_عبارت& /*عبارت*/) ثابت CV_OVERRIDE { بازگشت نادرست; }
    تهی assign(ثابت Mat_عبارت& عبارت, Mat& m, صحیح type=-1) ثابت CV_OVERRIDE;

    تهی matmul(ثابت Mat_عبارت& expr1, ثابت Mat_عبارت& expr2, Mat_عبارت& res) ثابت CV_OVERRIDE;

    ایستا تهی make_عبارت(Mat_عبارت& res, صحیح method, ثابت Mat& m);
};

ایستا MatOp_Invert g_MatOp_Invert;

رده MatOp_T CV_FINAL : عمومی MatOp
{
عمومی:
    MatOp_T() {}
    مجازی ~MatOp_T() {}

    منطقی elementWise(ثابت Mat_عبارت& /*عبارت*/) ثابت CV_OVERRIDE { بازگشت نادرست; }
    تهی assign(ثابت Mat_عبارت& عبارت, Mat& m, صحیح type=-1) ثابت CV_OVERRIDE;

    تهی multiply(ثابت Mat_عبارت& e1, دوگانه s, Mat_عبارت& res) ثابت CV_OVERRIDE;
    تهی transpose(ثابت Mat_عبارت& عبارت, Mat_عبارت& res) ثابت CV_OVERRIDE;

    ایستا تهی make_عبارت(Mat_عبارت& res, ثابت Mat& a, دوگانه alpha=1);
};

ایستا MatOp_T g_MatOp_T;

رده MatOp_Solve CV_FINAL : عمومی MatOp
{
عمومی:
    MatOp_Solve() {}
    مجازی ~MatOp_Solve() {}

    منطقی elementWise(ثابت Mat_عبارت& /*عبارت*/) ثابت CV_OVERRIDE { بازگشت نادرست; }
    تهی assign(ثابت Mat_عبارت& عبارت, Mat& m, صحیح type=-1) ثابت CV_OVERRIDE;

    ایستا تهی make_عبارت(Mat_عبارت& res, صحیح method, ثابت Mat& a, ثابت Mat& b);
};

ایستا MatOp_Solve g_MatOp_Solve;

رده MatOp_Initializer CV_FINAL : عمومی MatOp
{
عمومی:
    MatOp_Initializer() {}
    مجازی ~MatOp_Initializer() {}

    منطقی elementWise(ثابت Mat_عبارت& /*عبارت*/) ثابت CV_OVERRIDE { بازگشت نادرست; }
    تهی assign(ثابت Mat_عبارت& عبارت, Mat& m, صحیح type=-1) ثابت CV_OVERRIDE;

    تهی multiply(ثابت Mat_عبارت& e, دوگانه s, Mat_عبارت& res) ثابت CV_OVERRIDE;

    ایستا تهی make_عبارت(Mat_عبارت& res, صحیح method, Size sz, صحیح type, دوگانه alpha=1);
    ایستا تهی make_عبارت(Mat_عبارت& res, صحیح method, صحیح ndims, ثابت صحیح* sizes, صحیح type, دوگانه alpha=1);
};

ایستا MatOp_Initializer* getGlobalMatOpInitializer()
{
    CV_SINGLETON_LAZY_INIT(MatOp_Initializer, جدید MatOp_Initializer())
}

ایستا درونخطی منطقی isIdentity(ثابت Mat_عبارت& e) { بازگشت e.op == &g_MatOp_Identity; }
ایستا درونخطی منطقی isAddEx(ثابت Mat_عبارت& e) { بازگشت e.op == &g_MatOp_AddEx; }
ایستا درونخطی منطقی isScaled(ثابت Mat_عبارت& e) { بازگشت isAddEx(e) && (!e.b.data || e.beta == 0) && e.s == Scalar(); }
ایستا درونخطی منطقی isBin(ثابت Mat_عبارت& e, نویسه c) { بازگشت e.op == &g_MatOp_Bin && e.flags == c; }
ایستا درونخطی منطقی isCmp(ثابت Mat_عبارت& e) { بازگشت e.op == &g_MatOp_Cmp; }
ایستا درونخطی منطقی isReciprocal(ثابت Mat_عبارت& e) { بازگشت isBin(e,'/') && (!e.b.data || e.beta == 0); }
ایستا درونخطی منطقی isT(ثابت Mat_عبارت& e) { بازگشت e.op == &g_MatOp_T; }
ایستا درونخطی منطقی isInv(ثابت Mat_عبارت& e) { بازگشت e.op == &g_MatOp_Invert; }
ایستا درونخطی منطقی isSolve(ثابت Mat_عبارت& e) { بازگشت e.op == &g_MatOp_Solve; }
//static inline bool isGEMM(const MatExpr& e) { return e.op == &g_MatOp_GEMM; }
ایستا درونخطی منطقی isMatProd(ثابت Mat_عبارت& e) { بازگشت e.op == &g_MatOp_GEMM && (!e.c.data || e.beta == 0); }
ایستا درونخطی منطقی isInitializer(ثابت Mat_عبارت& e) { بازگشت e.op == getGlobalMatOpInitializer(); }

/////////////////////////////////////////////////////////////////////////////////////////////////////

MatOp::MatOp() {}
MatOp::~MatOp() {}


منطقی MatOp::elementWise(ثابت Mat_عبارت& /*عبارت*/) ثابت
{
    بازگشت نادرست;
}

تهی MatOp::roi(ثابت Mat_عبارت& عبارت, ثابت Range& rowRange, ثابت Range& colRange, Mat_عبارت& e) ثابت
{
    اگر( elementWise(عبارت) )
    {
        e = Mat_عبارت(عبارت.op, عبارت.flags, Mat(), Mat(), Mat(),
                    عبارت.alpha, عبارت.beta, عبارت.s);
        اگر(عبارت.a.data)
            e.a = عبارت.a(rowRange, colRange);
        اگر(عبارت.b.data)
            e.b = عبارت.b(rowRange, colRange);
        اگر(عبارت.c.data)
            e.c = عبارت.c(rowRange, colRange);
    }
    وگرنه
    {
        Mat m;
        عبارت.op->assign(عبارت, m);
        e = Mat_عبارت(&g_MatOp_Identity, 0, m(rowRange, colRange), Mat(), Mat());
    }
}

تهی MatOp::diag(ثابت Mat_عبارت& عبارت, صحیح d, Mat_عبارت& e) ثابت
{
    اگر( elementWise(عبارت) )
    {
        e = Mat_عبارت(عبارت.op, عبارت.flags, Mat(), Mat(), Mat(),
                    عبارت.alpha, عبارت.beta, عبارت.s);
        اگر(عبارت.a.data)
            e.a = عبارت.a.diag(d);
        اگر(عبارت.b.data)
            e.b = عبارت.b.diag(d);
        اگر(عبارت.c.data)
            e.c = عبارت.c.diag(d);
    }
    وگرنه
    {
        Mat m;
        عبارت.op->assign(عبارت, m);
        e = Mat_عبارت(&g_MatOp_Identity, 0, m.diag(d), Mat(), Mat());
    }
}


تهی MatOp::augAssignAdd(ثابت Mat_عبارت& عبارت, Mat& m) ثابت
{
    Mat temp;
    عبارت.op->assign(عبارت, temp);
    m += temp;
}


تهی MatOp::augAssignSubtract(ثابت Mat_عبارت& عبارت, Mat& m) ثابت
{
    Mat temp;
    عبارت.op->assign(عبارت, temp);
    m -= temp;
}


تهی MatOp::augAssignMultiply(ثابت Mat_عبارت& عبارت, Mat& m) ثابت
{
    Mat temp;
    عبارت.op->assign(عبارت, temp);
    m *= temp;
}


تهی MatOp::augAssignDivide(ثابت Mat_عبارت& عبارت, Mat& m) ثابت
{
    Mat temp;
    عبارت.op->assign(عبارت, temp);
    m /= temp;
}


تهی MatOp::augAssignAnd(ثابت Mat_عبارت& عبارت, Mat& m) ثابت
{
    Mat temp;
    عبارت.op->assign(عبارت, temp);
    m &= temp;
}


تهی MatOp::augAssignOr(ثابت Mat_عبارت& عبارت, Mat& m) ثابت
{
    Mat temp;
    عبارت.op->assign(عبارت, temp);
    m |= temp;
}


تهی MatOp::augAssignXor(ثابت Mat_عبارت& عبارت, Mat& m) ثابت
{
    Mat temp;
    عبارت.op->assign(عبارت, temp);
    m ^= temp;
}


تهی MatOp::add(ثابت Mat_عبارت& e1, ثابت Mat_عبارت& e2, Mat_عبارت& res) ثابت
{
    CV_INSTRUMENT_REGION();

    اگر( این == e2.op )
    {
        دوگانه alpha = 1, beta = 1;
        Scalar s;
        Mat m1, m2;
        اگر( isAddEx(e1) && (!e1.b.data || e1.beta == 0) )
        {
            m1 = e1.a;
            alpha = e1.alpha;
            s = e1.s;
        }
        وگرنه
            e1.op->assign(e1, m1);

        اگر( isAddEx(e2) && (!e2.b.data || e2.beta == 0) )
        {
            m2 = e2.a;
            beta = e2.alpha;
            s += e2.s;
        }
        وگرنه
            e2.op->assign(e2, m2);
        MatOp_AddEx::make_عبارت(res, m1, m2, alpha, beta, s);
    }
    وگرنه
        e2.op->add(e1, e2, res);
}


تهی MatOp::add(ثابت Mat_عبارت& expr1, ثابت Scalar& s, Mat_عبارت& res) ثابت
{
    CV_INSTRUMENT_REGION();

    Mat m1;
    expr1.op->assign(expr1, m1);
    MatOp_AddEx::make_عبارت(res, m1, Mat(), 1, 0, s);
}


تهی MatOp::subtract(ثابت Mat_عبارت& e1, ثابت Mat_عبارت& e2, Mat_عبارت& res) ثابت
{
    CV_INSTRUMENT_REGION();

    اگر( این == e2.op )
    {
        دوگانه alpha = 1, beta = -1;
        Scalar s;
        Mat m1, m2;
        اگر( isAddEx(e1) && (!e1.b.data || e1.beta == 0) )
        {
            m1 = e1.a;
            alpha = e1.alpha;
            s = e1.s;
        }
        وگرنه
            e1.op->assign(e1, m1);

        اگر( isAddEx(e2) && (!e2.b.data || e2.beta == 0) )
        {
            m2 = e2.a;
            beta = -e2.alpha;
            s -= e2.s;
        }
        وگرنه
            e2.op->assign(e2, m2);
        MatOp_AddEx::make_عبارت(res, m1, m2, alpha, beta, s);
    }
    وگرنه
        e2.op->subtract(e1, e2, res);
}


تهی MatOp::subtract(ثابت Scalar& s, ثابت Mat_عبارت& عبارت, Mat_عبارت& res) ثابت
{
    CV_INSTRUMENT_REGION();

    Mat m;
    عبارت.op->assign(عبارت, m);
    MatOp_AddEx::make_عبارت(res, m, Mat(), -1, 0, s);
}


تهی MatOp::multiply(ثابت Mat_عبارت& e1, ثابت Mat_عبارت& e2, Mat_عبارت& res, دوگانه scale) ثابت
{
    CV_INSTRUMENT_REGION();

    اگر( این == e2.op )
    {
        Mat m1, m2;

        اگر( isReciprocal(e1) )
        {
            اگر( isScaled(e2) )
            {
                scale *= e2.alpha;
                m2 = e2.a;
            }
            وگرنه
                e2.op->assign(e2, m2);

            MatOp_Bin::make_عبارت(res, '/', m2, e1.a, scale/e1.alpha);
        }
        وگرنه
        {
            نویسه op = '*';
            اگر( isScaled(e1) )
            {
                m1 = e1.a;
                scale *= e1.alpha;
            }
            وگرنه
                e1.op->assign(e1, m1);

            اگر( isScaled(e2) )
            {
                m2 = e2.a;
                scale *= e2.alpha;
            }
            وگرنه اگر( isReciprocal(e2) )
            {
                op = '/';
                m2 = e2.a;
                scale *= e2.alpha;
            }
            وگرنه
                e2.op->assign(e2, m2);

            MatOp_Bin::make_عبارت(res, op, m1, m2, scale);
        }
    }
    وگرنه
        e2.op->multiply(e1, e2, res, scale);
}


تهی MatOp::multiply(ثابت Mat_عبارت& عبارت, دوگانه s, Mat_عبارت& res) ثابت
{
    CV_INSTRUMENT_REGION();

    Mat m;
    عبارت.op->assign(عبارت, m);
    MatOp_AddEx::make_عبارت(res, m, Mat(), s, 0);
}


تهی MatOp::divide(ثابت Mat_عبارت& e1, ثابت Mat_عبارت& e2, Mat_عبارت& res, دوگانه scale) ثابت
{
    CV_INSTRUMENT_REGION();

    اگر( این == e2.op )
    {
        اگر( isReciprocal(e1) && isReciprocal(e2) )
            MatOp_Bin::make_عبارت(res, '/', e2.a, e1.a, e1.alpha/e2.alpha);
        وگرنه
        {
            Mat m1, m2;
            نویسه op = '/';

            اگر( isScaled(e1) )
            {
                m1 = e1.a;
                scale *= e1.alpha;
            }
            وگرنه
                e1.op->assign(e1, m1);

            اگر( isScaled(e2) )
            {
                m2 = e2.a;
                scale /= e2.alpha;
            }
            وگرنه اگر( isReciprocal(e2) )
            {
                m2 = e2.a;
                scale /= e2.alpha;
                op = '*';
            }
            وگرنه
                e2.op->assign(e2, m2);
            MatOp_Bin::make_عبارت(res, op, m1, m2, scale);
        }
    }
    وگرنه
        e2.op->divide(e1, e2, res, scale);
}


تهی MatOp::divide(دوگانه s, ثابت Mat_عبارت& عبارت, Mat_عبارت& res) ثابت
{
    CV_INSTRUMENT_REGION();

    Mat m;
    عبارت.op->assign(عبارت, m);
    MatOp_Bin::make_عبارت(res, '/', m, Mat(), s);
}


تهی MatOp::abs(ثابت Mat_عبارت& عبارت, Mat_عبارت& res) ثابت
{
    CV_INSTRUMENT_REGION();

    Mat m;
    عبارت.op->assign(عبارت, m);
    MatOp_Bin::make_عبارت(res, 'a', m, Mat());
}


تهی MatOp::transpose(ثابت Mat_عبارت& عبارت, Mat_عبارت& res) ثابت
{
    CV_INSTRUMENT_REGION();

    Mat m;
    عبارت.op->assign(عبارت, m);
    MatOp_T::make_عبارت(res, m, 1);
}


تهی MatOp::matmul(ثابت Mat_عبارت& e1, ثابت Mat_عبارت& e2, Mat_عبارت& res) ثابت
{
    اگر( این == e2.op )
    {
        دوگانه scale = 1;
        صحیح flags = 0;
        Mat m1, m2;

        اگر( isT(e1) )
        {
            flags = cv::GEMM_1_T;
            scale = e1.alpha;
            m1 = e1.a;
        }
        وگرنه اگر( isScaled(e1) )
        {
            scale = e1.alpha;
            m1 = e1.a;
        }
        وگرنه
            e1.op->assign(e1, m1);

        اگر( isT(e2) )
        {
            flags |= cv::GEMM_2_T;
            scale *= e2.alpha;
            m2 = e2.a;
        }
        وگرنه اگر( isScaled(e2) )
        {
            scale *= e2.alpha;
            m2 = e2.a;
        }
        وگرنه
            e2.op->assign(e2, m2);

        MatOp_GEMM::make_عبارت(res, flags, m1, m2, scale);
    }
    وگرنه
        e2.op->matmul(e1, e2, res);
}


تهی MatOp::invert(ثابت Mat_عبارت& عبارت, صحیح method, Mat_عبارت& res) ثابت
{
    Mat m;
    عبارت.op->assign(عبارت, m);
    MatOp_Invert::make_عبارت(res, method, m);
}


Size MatOp::size(ثابت Mat_عبارت& عبارت) ثابت
{
    بازگشت !عبارت.a.empty() ? عبارت.a.size() : عبارت.b.empty() ? عبارت.b.size() : عبارت.c.size();
}

صحیح MatOp::type(ثابت Mat_عبارت& عبارت) ثابت
{
    CV_INSTRUMENT_REGION();

    بازگشت !عبارت.a.empty() ? عبارت.a.type() : عبارت.b.empty() ? عبارت.b.type() : عبارت.c.type();
}

//////////////////////////////////////////////////////////////////////////////////////////////////

Mat_عبارت::Mat_عبارت(ثابت Mat& m) : op(&g_MatOp_Identity), flags(0), a(m), b(Mat()), c(Mat()), alpha(1), beta(0), s(Scalar())
{
}

Mat_عبارت Mat_عبارت::row(صحیح y) ثابت
{
    Mat_عبارت e;
    op->roi(*این, Range(y, y+1), Range::all(), e);
    بازگشت e;
}

Mat_عبارت Mat_عبارت::col(صحیح x) ثابت
{
    Mat_عبارت e;
    op->roi(*این, Range::all(), Range(x, x+1), e);
    بازگشت e;
}

Mat_عبارت Mat_عبارت::diag(صحیح d) ثابت
{
    Mat_عبارت e;
    op->diag(*این, d, e);
    بازگشت e;
}

Mat_عبارت Mat_عبارت::operator()( ثابت Range& rowRange, ثابت Range& colRange ) ثابت
{
    Mat_عبارت e;
    op->roi(*این, rowRange, colRange, e);
    بازگشت e;
}

Mat_عبارت Mat_عبارت::operator()( ثابت Rect& roi ) ثابت
{
    Mat_عبارت e;
    op->roi(*این, Range(roi.y, roi.y + roi.height), Range(roi.x, roi.x + roi.width), e);
    بازگشت e;
}

Mat Mat_عبارت::cross(ثابت Mat& m) ثابت
{
    بازگشت ((Mat)*این).cross(m);
}

دوگانه Mat_عبارت::dot(ثابت Mat& m) ثابت
{
    بازگشت ((Mat)*این).dot(m);
}

Mat_عبارت Mat_عبارت::t() ثابت
{
    Mat_عبارت e;
    op->transpose(*این, e);
    بازگشت e;
}

Mat_عبارت Mat_عبارت::inv(صحیح method) ثابت
{
    Mat_عبارت e;
    op->invert(*این, method, e);
    بازگشت e;
}

Mat_عبارت Mat_عبارت::mul(ثابت Mat_عبارت& e, دوگانه scale) ثابت
{
    Mat_عبارت en;
    op->multiply(*این, e, en, scale);
    بازگشت en;
}

Mat_عبارت Mat_عبارت::mul(ثابت Mat& m, دوگانه scale) ثابت
{
    Mat_عبارت e;
    op->multiply(*این, Mat_عبارت(m), e, scale);
    بازگشت e;
}

Mat_عبارت operator + (ثابت Mat& a, ثابت Mat& b)
{
    checkOperandsExist(a, b);
    Mat_عبارت e;
    MatOp_AddEx::make_عبارت(e, a, b, 1, 1);
    بازگشت e;
}

Mat_عبارت operator + (ثابت Mat& a, ثابت Scalar& s)
{
    checkOperandsExist(a);
    Mat_عبارت e;
    MatOp_AddEx::make_عبارت(e, a, Mat(), 1, 0, s);
    بازگشت e;
}

Mat_عبارت operator + (ثابت Scalar& s, ثابت Mat& a)
{
    checkOperandsExist(a);
    Mat_عبارت e;
    MatOp_AddEx::make_عبارت(e, a, Mat(), 1, 0, s);
    بازگشت e;
}

Mat_عبارت operator + (ثابت Mat_عبارت& e, ثابت Mat& m)
{
    Mat_عبارت en;
    e.op->add(e, Mat_عبارت(m), en);
    بازگشت en;
}

Mat_عبارت operator + (ثابت Mat& m, ثابت Mat_عبارت& e)
{
    checkOperandsExist(m);
    Mat_عبارت en;
    e.op->add(e, Mat_عبارت(m), en);
    بازگشت en;
}

Mat_عبارت operator + (ثابت Mat_عبارت& e, ثابت Scalar& s)
{
    Mat_عبارت en;
    e.op->add(e, s, en);
    بازگشت en;
}

Mat_عبارت operator + (ثابت Scalar& s, ثابت Mat_عبارت& e)
{
    Mat_عبارت en;
    e.op->add(e, s, en);
    بازگشت en;
}

Mat_عبارت operator + (ثابت Mat_عبارت& e1, ثابت Mat_عبارت& e2)
{
    Mat_عبارت en;
    e1.op->add(e1, e2, en);
    بازگشت en;
}

Mat_عبارت operator - (ثابت Mat& a, ثابت Mat& b)
{
    checkOperandsExist(a, b);
    Mat_عبارت e;
    MatOp_AddEx::make_عبارت(e, a, b, 1, -1);
    بازگشت e;
}

Mat_عبارت operator - (ثابت Mat& a, ثابت Scalar& s)
{
    checkOperandsExist(a);
    Mat_عبارت e;
    MatOp_AddEx::make_عبارت(e, a, Mat(), 1, 0, -s);
    بازگشت e;
}

Mat_عبارت operator - (ثابت Scalar& s, ثابت Mat& a)
{
    checkOperandsExist(a);
    Mat_عبارت e;
    MatOp_AddEx::make_عبارت(e, a, Mat(), -1, 0, s);
    بازگشت e;
}

Mat_عبارت operator - (ثابت Mat_عبارت& e, ثابت Mat& m)
{
    checkOperandsExist(m);
    Mat_عبارت en;
    e.op->subtract(e, Mat_عبارت(m), en);
    بازگشت en;
}

Mat_عبارت operator - (ثابت Mat& m, ثابت Mat_عبارت& e)
{
    checkOperandsExist(m);
    Mat_عبارت en;
    e.op->subtract(Mat_عبارت(m), e, en);
    بازگشت en;
}

Mat_عبارت operator - (ثابت Mat_عبارت& e, ثابت Scalar& s)
{
    Mat_عبارت en;
    e.op->add(e, -s, en);
    بازگشت en;
}

Mat_عبارت operator - (ثابت Scalar& s, ثابت Mat_عبارت& e)
{
    Mat_عبارت en;
    e.op->subtract(s, e, en);
    بازگشت en;
}

Mat_عبارت operator - (ثابت Mat_عبارت& e1, ثابت Mat_عبارت& e2)
{
    Mat_عبارت en;
    e1.op->subtract(e1, e2, en);
    بازگشت en;
}

Mat_عبارت operator - (ثابت Mat& m)
{
    checkOperandsExist(m);
    Mat_عبارت e;
    MatOp_AddEx::make_عبارت(e, m, Mat(), -1, 0);
    بازگشت e;
}

Mat_عبارت operator - (ثابت Mat_عبارت& e)
{
    Mat_عبارت en;
    e.op->subtract(Scalar(0), e, en);
    بازگشت en;
}

Mat_عبارت operator * (ثابت Mat& a, ثابت Mat& b)
{
    checkOperandsExist(a, b);
    Mat_عبارت e;
    MatOp_GEMM::make_عبارت(e, 0, a, b);
    بازگشت e;
}

Mat_عبارت operator * (ثابت Mat& a, دوگانه s)
{
    checkOperandsExist(a);
    Mat_عبارت e;
    MatOp_AddEx::make_عبارت(e, a, Mat(), s, 0);
    بازگشت e;
}

Mat_عبارت operator * (دوگانه s, ثابت Mat& a)
{
    checkOperandsExist(a);
    Mat_عبارت e;
    MatOp_AddEx::make_عبارت(e, a, Mat(), s, 0);
    بازگشت e;
}

Mat_عبارت operator * (ثابت Mat_عبارت& e, ثابت Mat& m)
{
    Mat_عبارت en;
    e.op->matmul(e, Mat_عبارت(m), en);
    بازگشت en;
}

Mat_عبارت operator * (ثابت Mat& m, ثابت Mat_عبارت& e)
{
    checkOperandsExist(m);
    Mat_عبارت en;
    e.op->matmul(Mat_عبارت(m), e, en);
    بازگشت en;
}

Mat_عبارت operator * (ثابت Mat_عبارت& e, دوگانه s)
{
    Mat_عبارت en;
    e.op->multiply(e, s, en);
    بازگشت en;
}

Mat_عبارت operator * (دوگانه s, ثابت Mat_عبارت& e)
{
    Mat_عبارت en;
    e.op->multiply(e, s, en);
    بازگشت en;
}

Mat_عبارت operator * (ثابت Mat_عبارت& e1, ثابت Mat_عبارت& e2)
{
    Mat_عبارت en;
    e1.op->matmul(e1, e2, en);
    بازگشت en;
}

Mat_عبارت operator / (ثابت Mat& a, ثابت Mat& b)
{
    checkOperandsExist(a, b);
    Mat_عبارت e;
    MatOp_Bin::make_عبارت(e, '/', a, b);
    بازگشت e;
}

Mat_عبارت operator / (ثابت Mat& a, دوگانه s)
{
    checkOperandsExist(a);
    Mat_عبارت e;
    MatOp_AddEx::make_عبارت(e, a, Mat(), 1./s, 0);
    بازگشت e;
}

Mat_عبارت operator / (دوگانه s, ثابت Mat& a)
{
    checkOperandsExist(a);
    Mat_عبارت e;
    MatOp_Bin::make_عبارت(e, '/', a, Mat(), s);
    بازگشت e;
}

Mat_عبارت operator / (ثابت Mat_عبارت& e, ثابت Mat& m)
{
    Mat_عبارت en;
    e.op->divide(e, Mat_عبارت(m), en);
    بازگشت en;
}

Mat_عبارت operator / (ثابت Mat& m, ثابت Mat_عبارت& e)
{
    checkOperandsExist(m);
    Mat_عبارت en;
    e.op->divide(Mat_عبارت(m), e, en);
    بازگشت en;
}

Mat_عبارت operator / (ثابت Mat_عبارت& e, دوگانه s)
{
    Mat_عبارت en;
    e.op->multiply(e, 1./s, en);
    بازگشت en;
}

Mat_عبارت operator / (دوگانه s, ثابت Mat_عبارت& e)
{
    Mat_عبارت en;
    e.op->divide(s, e, en);
    بازگشت en;
}

Mat_عبارت operator / (ثابت Mat_عبارت& e1, ثابت Mat_عبارت& e2)
{
    Mat_عبارت en;
    e1.op->divide(e1, e2, en);
    بازگشت en;
}

Mat_عبارت operator < (ثابت Mat& a, ثابت Mat& b)
{
    checkOperandsExist(a, b);
    Mat_عبارت e;
    MatOp_Cmp::make_عبارت(e, cv::CMP_LT, a, b);
    بازگشت e;
}

Mat_عبارت operator < (ثابت Mat& a, دوگانه s)
{
    checkOperandsExist(a);
    Mat_عبارت e;
    MatOp_Cmp::make_عبارت(e, cv::CMP_LT, a, s);
    بازگشت e;
}

Mat_عبارت operator < (دوگانه s, ثابت Mat& a)
{
    checkOperandsExist(a);
    Mat_عبارت e;
    MatOp_Cmp::make_عبارت(e, cv::CMP_GT, a, s);
    بازگشت e;
}

Mat_عبارت operator <= (ثابت Mat& a, ثابت Mat& b)
{
    checkOperandsExist(a, b);
    Mat_عبارت e;
    MatOp_Cmp::make_عبارت(e, cv::CMP_LE, a, b);
    بازگشت e;
}

Mat_عبارت operator <= (ثابت Mat& a, دوگانه s)
{
    checkOperandsExist(a);
    Mat_عبارت e;
    MatOp_Cmp::make_عبارت(e, cv::CMP_LE, a, s);
    بازگشت e;
}

Mat_عبارت operator <= (دوگانه s, ثابت Mat& a)
{
    checkOperandsExist(a);
    Mat_عبارت e;
    MatOp_Cmp::make_عبارت(e, cv::CMP_GE, a, s);
    بازگشت e;
}

Mat_عبارت operator == (ثابت Mat& a, ثابت Mat& b)
{
    checkOperandsExist(a, b);
    Mat_عبارت e;
    MatOp_Cmp::make_عبارت(e, cv::CMP_EQ, a, b);
    بازگشت e;
}

Mat_عبارت operator == (ثابت Mat& a, دوگانه s)
{
    checkOperandsExist(a);
    Mat_عبارت e;
    MatOp_Cmp::make_عبارت(e, cv::CMP_EQ, a, s);
    بازگشت e;
}

Mat_عبارت operator == (دوگانه s, ثابت Mat& a)
{
    checkOperandsExist(a);
    Mat_عبارت e;
    MatOp_Cmp::make_عبارت(e, cv::CMP_EQ, a, s);
    بازگشت e;
}

Mat_عبارت operator != (ثابت Mat& a, ثابت Mat& b)
{
    checkOperandsExist(a, b);
    Mat_عبارت e;
    MatOp_Cmp::make_عبارت(e, cv::CMP_NE, a, b);
    بازگشت e;
}

Mat_عبارت operator != (ثابت Mat& a, دوگانه s)
{
    checkOperandsExist(a);
    Mat_عبارت e;
    MatOp_Cmp::make_عبارت(e, cv::CMP_NE, a, s);
    بازگشت e;
}

Mat_عبارت operator != (دوگانه s, ثابت Mat& a)
{
    checkOperandsExist(a);
    Mat_عبارت e;
    MatOp_Cmp::make_عبارت(e, cv::CMP_NE, a, s);
    بازگشت e;
}

Mat_عبارت operator >= (ثابت Mat& a, ثابت Mat& b)
{
    checkOperandsExist(a, b);
    Mat_عبارت e;
    MatOp_Cmp::make_عبارت(e, cv::CMP_GE, a, b);
    بازگشت e;
}

Mat_عبارت operator >= (ثابت Mat& a, دوگانه s)
{
    checkOperandsExist(a);
    Mat_عبارت e;
    MatOp_Cmp::make_عبارت(e, cv::CMP_GE, a, s);
    بازگشت e;
}

Mat_عبارت operator >= (دوگانه s, ثابت Mat& a)
{
    checkOperandsExist(a);
    Mat_عبارت e;
    MatOp_Cmp::make_عبارت(e, cv::CMP_LE, a, s);
    بازگشت e;
}

Mat_عبارت operator > (ثابت Mat& a, ثابت Mat& b)
{
    checkOperandsExist(a, b);
    Mat_عبارت e;
    MatOp_Cmp::make_عبارت(e, cv::CMP_GT, a, b);
    بازگشت e;
}

Mat_عبارت operator > (ثابت Mat& a, دوگانه s)
{
    checkOperandsExist(a);
    Mat_عبارت e;
    MatOp_Cmp::make_عبارت(e, cv::CMP_GT, a, s);
    بازگشت e;
}

Mat_عبارت operator > (دوگانه s, ثابت Mat& a)
{
    checkOperandsExist(a);
    Mat_عبارت e;
    MatOp_Cmp::make_عبارت(e, cv::CMP_LT, a, s);
    بازگشت e;
}

Mat_عبارت min(ثابت Mat& a, ثابت Mat& b)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a, b);
    Mat_عبارت e;
    MatOp_Bin::make_عبارت(e, 'm', a, b);
    بازگشت e;
}

Mat_عبارت min(ثابت Mat& a, دوگانه s)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    Mat_عبارت e;
    MatOp_Bin::make_عبارت(e, 'n', a, s);
    بازگشت e;
}

Mat_عبارت min(دوگانه s, ثابت Mat& a)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    Mat_عبارت e;
    MatOp_Bin::make_عبارت(e, 'n', a, s);
    بازگشت e;
}

Mat_عبارت max(ثابت Mat& a, ثابت Mat& b)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a, b);
    Mat_عبارت e;
    MatOp_Bin::make_عبارت(e, 'M', a, b);
    بازگشت e;
}

Mat_عبارت max(ثابت Mat& a, دوگانه s)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    Mat_عبارت e;
    MatOp_Bin::make_عبارت(e, 'N', a, s);
    بازگشت e;
}

Mat_عبارت max(دوگانه s, ثابت Mat& a)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    Mat_عبارت e;
    MatOp_Bin::make_عبارت(e, 'N', a, s);
    بازگشت e;
}

Mat_عبارت operator & (ثابت Mat& a, ثابت Mat& b)
{
    checkOperandsExist(a, b);
    Mat_عبارت e;
    MatOp_Bin::make_عبارت(e, '&', a, b);
    بازگشت e;
}

Mat_عبارت operator & (ثابت Mat& a, ثابت Scalar& s)
{
    checkOperandsExist(a);
    Mat_عبارت e;
    MatOp_Bin::make_عبارت(e, '&', a, s);
    بازگشت e;
}

Mat_عبارت operator & (ثابت Scalar& s, ثابت Mat& a)
{
    checkOperandsExist(a);
    Mat_عبارت e;
    MatOp_Bin::make_عبارت(e, '&', a, s);
    بازگشت e;
}

Mat_عبارت operator | (ثابت Mat& a, ثابت Mat& b)
{
    checkOperandsExist(a, b);
    Mat_عبارت e;
    MatOp_Bin::make_عبارت(e, '|', a, b);
    بازگشت e;
}

Mat_عبارت operator | (ثابت Mat& a, ثابت Scalar& s)
{
    checkOperandsExist(a);
    Mat_عبارت e;
    MatOp_Bin::make_عبارت(e, '|', a, s);
    بازگشت e;
}

Mat_عبارت operator | (ثابت Scalar& s, ثابت Mat& a)
{
    checkOperandsExist(a);
    Mat_عبارت e;
    MatOp_Bin::make_عبارت(e, '|', a, s);
    بازگشت e;
}

Mat_عبارت operator ^ (ثابت Mat& a, ثابت Mat& b)
{
    checkOperandsExist(a, b);
    Mat_عبارت e;
    MatOp_Bin::make_عبارت(e, '^', a, b);
    بازگشت e;
}

Mat_عبارت operator ^ (ثابت Mat& a, ثابت Scalar& s)
{
    checkOperandsExist(a);
    Mat_عبارت e;
    MatOp_Bin::make_عبارت(e, '^', a, s);
    بازگشت e;
}

Mat_عبارت operator ^ (ثابت Scalar& s, ثابت Mat& a)
{
    checkOperandsExist(a);
    Mat_عبارت e;
    MatOp_Bin::make_عبارت(e, '^', a, s);
    بازگشت e;
}

Mat_عبارت operator ~(ثابت Mat& a)
{
    checkOperandsExist(a);
    Mat_عبارت e;
    MatOp_Bin::make_عبارت(e, '~', a, Scalar());
    بازگشت e;
}

Mat_عبارت abs(ثابت Mat& a)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    Mat_عبارت e;
    MatOp_Bin::make_عبارت(e, 'a', a, Scalar());
    بازگشت e;
}

Mat_عبارت abs(ثابت Mat_عبارت& e)
{
    CV_INSTRUMENT_REGION();

    Mat_عبارت en;
    e.op->abs(e, en);
    بازگشت en;
}


Size Mat_عبارت::size() ثابت
{
    اگر( isT(*این) || isInv(*این) )
        بازگشت Size(a.rows, a.cols);
    اگر( isSolve(*این) )
        بازگشت Size(b.cols, a.cols);
    اگر( isInitializer(*این) )
        بازگشت a.size();
    بازگشت op ? op->size(*این) : Size();
}


صحیح Mat_عبارت::type() ثابت
{
    CV_INSTRUMENT_REGION();

    اگر( isInitializer(*این) )
        بازگشت a.type();
    اگر( isCmp(*این) )
        بازگشت CV_MAKETYPE(CV_8U, a.channels());
    بازگشت op ? op->type(*این) : -1;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////

تهی MatOp_Identity::assign(ثابت Mat_عبارت& e, Mat& m, صحیح _type) ثابت
{
    اگر( _type == -1 || _type == e.a.type() )
        m = e.a;
    وگرنه
    {
        CV_Assert( CV_MAT_CN(_type) == e.a.channels() );
        e.a.convertTo(m, _type);
    }
}

درونخطی تهی MatOp_Identity::make_عبارت(Mat_عبارت& res, ثابت Mat& m)
{
    res = Mat_عبارت(&g_MatOp_Identity, 0, m, Mat(), Mat(), 1, 0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

تهی MatOp_AddEx::assign(ثابت Mat_عبارت& e, Mat& m, صحیح _type) ثابت
{
    Mat temp, &dst = _type == -1 || e.a.type() == _type ? m : temp;
    اگر( e.b.data )
    {
        اگر( e.s == Scalar() || !e.s.isReal() )
        {
            اگر( e.alpha == 1 )
            {
                اگر( e.beta == 1 )
                    cv::add(e.a, e.b, dst);
                وگرنه اگر( e.beta == -1 )
                    cv::subtract(e.a, e.b, dst);
                وگرنه
                    cv::scaleAdd(e.b, e.beta, e.a, dst);
            }
            وگرنه اگر( e.beta == 1 )
            {
                اگر( e.alpha == -1 )
                    cv::subtract(e.b, e.a, dst);
                وگرنه
                    cv::scaleAdd(e.a, e.alpha, e.b, dst);
            }
            وگرنه
                cv::addWeighted(e.a, e.alpha, e.b, e.beta, 0, dst);

            اگر( !e.s.isReal() )
                cv::add(dst, e.s, dst);
        }
        وگرنه
        {
            اگر (e.a.channels() > 1)
                CV_LOG_ONCE_WARNING(پوچ, "OpenCV/MatExpr: processing of multi-channel arrays might be changed in the future: "
                                          "https://github.com/opencv/opencv/issues/16739");
            cv::addWeighted(e.a, e.alpha, e.b, e.beta, e.s[0], dst);
        }
    }
    وگرنه اگر( e.s.isReal() && (dst.data != m.data || fabs(e.alpha) != 1))
    {
        اگر (e.a.channels() > 1 && e.s[0] != 0.0)
            CV_LOG_ONCE_WARNING(پوچ, "OpenCV/MatExpr: processing of multi-channel arrays might be changed in the future: "
                                      "https://github.com/opencv/opencv/issues/16739");
        e.a.convertTo(m, _type, e.alpha, e.s[0]);
        بازگشت;
    }
    وگرنه اگر( e.alpha == 1 )
        cv::add(e.a, e.s, dst);
    وگرنه اگر( e.alpha == -1 )
        cv::subtract(e.s, e.a, dst);
    وگرنه
    {
        e.a.convertTo(dst, e.a.type(), e.alpha);
        cv::add(dst, e.s, dst);
    }

    اگر( dst.data != m.data )
        dst.convertTo(m, m.type());
}


تهی MatOp_AddEx::add(ثابت Mat_عبارت& e, ثابت Scalar& s, Mat_عبارت& res) ثابت
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.s += s;
}


تهی MatOp_AddEx::subtract(ثابت Scalar& s, ثابت Mat_عبارت& e, Mat_عبارت& res) ثابت
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha = -res.alpha;
    res.beta = -res.beta;
    res.s = s - res.s;
}

تهی MatOp_AddEx::multiply(ثابت Mat_عبارت& e, دوگانه s, Mat_عبارت& res) ثابت
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha *= s;
    res.beta *= s;
    res.s *= s;
}

تهی MatOp_AddEx::divide(دوگانه s, ثابت Mat_عبارت& e, Mat_عبارت& res) ثابت
{
    CV_INSTRUMENT_REGION();

    اگر( isScaled(e) )
        MatOp_Bin::make_عبارت(res, '/', e.a, Mat(), s/e.alpha);
    وگرنه
        MatOp::divide(s, e, res);
}


تهی MatOp_AddEx::transpose(ثابت Mat_عبارت& e, Mat_عبارت& res) ثابت
{
    CV_INSTRUMENT_REGION();

    اگر( isScaled(e) )
        MatOp_T::make_عبارت(res, e.a, e.alpha);
    وگرنه
        MatOp::transpose(e, res);
}

تهی MatOp_AddEx::abs(ثابت Mat_عبارت& e, Mat_عبارت& res) ثابت
{
    CV_INSTRUMENT_REGION();

    اگر( (!e.b.data || e.beta == 0) && fabs(e.alpha) == 1 )
        MatOp_Bin::make_عبارت(res, 'a', e.a, -e.s*e.alpha);
    وگرنه اگر( e.b.data && e.alpha + e.beta == 0 && e.alpha*e.beta == -1 )
        MatOp_Bin::make_عبارت(res, 'a', e.a, e.b);
    وگرنه
        MatOp::abs(e, res);
}

درونخطی تهی MatOp_AddEx::make_عبارت(Mat_عبارت& res, ثابت Mat& a, ثابت Mat& b, دوگانه alpha, دوگانه beta, ثابت Scalar& s)
{
    res = Mat_عبارت(&g_MatOp_AddEx, 0, a, b, Mat(), alpha, beta, s);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

تهی MatOp_Bin::assign(ثابت Mat_عبارت& e, Mat& m, صحیح _type) ثابت
{
    Mat temp, &dst = _type == -1 || e.a.type() == _type ? m : temp;

    اگر( e.flags == '*' )
        cv::multiply(e.a, e.b, dst, e.alpha);
    وگرنه اگر( e.flags == '/' && e.b.data )
        cv::divide(e.a, e.b, dst, e.alpha);
    وگرنه اگر( e.flags == '/' && !e.b.data )
        cv::divide(e.alpha, e.a, dst );
    وگرنه اگر( e.flags == '&' && e.b.data )
        bitwise_and(e.a, e.b, dst);
    وگرنه اگر( e.flags == '&' && !e.b.data )
        bitwise_and(e.a, e.s, dst);
    وگرنه اگر( e.flags == '|' && e.b.data )
        bitwise_or(e.a, e.b, dst);
    وگرنه اگر( e.flags == '|' && !e.b.data )
        bitwise_or(e.a, e.s, dst);
    وگرنه اگر( e.flags == '^' && e.b.data )
        bitwise_xor(e.a, e.b, dst);
    وگرنه اگر( e.flags == '^' && !e.b.data )
        bitwise_xor(e.a, e.s, dst);
    وگرنه اگر( e.flags == '~' && !e.b.data )
        bitwise_not(e.a, dst);
    وگرنه اگر( e.flags == 'm' )
        cv::min(e.a, e.b, dst);
    وگرنه اگر( e.flags == 'n' )
        cv::min(e.a, e.s[0], dst);
    وگرنه اگر( e.flags == 'M' )
        cv::max(e.a, e.b, dst);
    وگرنه اگر( e.flags == 'N' )
        cv::max(e.a, e.s[0], dst);
    وگرنه اگر( e.flags == 'a' && e.b.data )
        cv::absdiff(e.a, e.b, dst);
    وگرنه اگر( e.flags == 'a' && !e.b.data )
        cv::absdiff(e.a, e.s, dst);
    وگرنه
        CV_Error(cv::Error::StsError, "Unknown operation");

    اگر( dst.data != m.data )
        dst.convertTo(m, _type);
}

تهی MatOp_Bin::multiply(ثابت Mat_عبارت& e, دوگانه s, Mat_عبارت& res) ثابت
{
    CV_INSTRUMENT_REGION();

    اگر( e.flags == '*' || e.flags == '/' )
    {
        res = e;
        res.alpha *= s;
    }
    وگرنه
        MatOp::multiply(e, s, res);
}

تهی MatOp_Bin::divide(دوگانه s, ثابت Mat_عبارت& e, Mat_عبارت& res) ثابت
{
    CV_INSTRUMENT_REGION();

    اگر( e.flags == '/' && (!e.b.data || e.beta == 0) )
        MatOp_AddEx::make_عبارت(res, e.a, Mat(), s/e.alpha, 0);
    وگرنه
        MatOp::divide(s, e, res);
}

درونخطی تهی MatOp_Bin::make_عبارت(Mat_عبارت& res, نویسه op, ثابت Mat& a, ثابت Mat& b, دوگانه scale)
{
    res = Mat_عبارت(&g_MatOp_Bin, op, a, b, Mat(), scale, b.data ? 1 : 0);
}

درونخطی تهی MatOp_Bin::make_عبارت(Mat_عبارت& res, نویسه op, ثابت Mat& a, ثابت Scalar& s)
{
    res = Mat_عبارت(&g_MatOp_Bin, op, a, Mat(), Mat(), 1, 0, s);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

تهی MatOp_Cmp::assign(ثابت Mat_عبارت& e, Mat& m, صحیح _type) ثابت
{
    Mat temp, &dst = _type == -1 || _type == CV_8U ? m : temp;

    اگر( e.b.data )
        cv::compare(e.a, e.b, dst, e.flags);
    وگرنه
        cv::compare(e.a, e.alpha, dst, e.flags);

    اگر( dst.data != m.data )
        dst.convertTo(m, _type);
}

درونخطی تهی MatOp_Cmp::make_عبارت(Mat_عبارت& res, صحیح cmpop, ثابت Mat& a, ثابت Mat& b)
{
    res = Mat_عبارت(&g_MatOp_Cmp, cmpop, a, b, Mat(), 1, 1);
}

درونخطی تهی MatOp_Cmp::make_عبارت(Mat_عبارت& res, صحیح cmpop, ثابت Mat& a, دوگانه alpha)
{
    res = Mat_عبارت(&g_MatOp_Cmp, cmpop, a, Mat(), Mat(), alpha, 1);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

تهی MatOp_T::assign(ثابت Mat_عبارت& e, Mat& m, صحیح _type) ثابت
{
    Mat temp, &dst = _type == -1 || _type == e.a.type() ? m : temp;

    cv::transpose(e.a, dst);

    اگر( dst.data != m.data || e.alpha != 1 )
        dst.convertTo(m, _type, e.alpha);
}

تهی MatOp_T::multiply(ثابت Mat_عبارت& e, دوگانه s, Mat_عبارت& res) ثابت
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha *= s;
}

تهی MatOp_T::transpose(ثابت Mat_عبارت& e, Mat_عبارت& res) ثابت
{
    CV_INSTRUMENT_REGION();

    اگر( e.alpha == 1 )
        MatOp_Identity::make_عبارت(res, e.a);
    وگرنه
        MatOp_AddEx::make_عبارت(res, e.a, Mat(), e.alpha, 0);
}

درونخطی تهی MatOp_T::make_عبارت(Mat_عبارت& res, ثابت Mat& a, دوگانه alpha)
{
    res = Mat_عبارت(&g_MatOp_T, 0, a, Mat(), Mat(), alpha, 0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

تهی MatOp_GEMM::assign(ثابت Mat_عبارت& e, Mat& m, صحیح _type) ثابت
{
    Mat temp, &dst = _type == -1 || _type == e.a.type() ? m : temp;

    cv::gemm(e.a, e.b, e.alpha, e.c, e.beta, dst, e.flags);
    اگر( dst.data != m.data )
        dst.convertTo(m, _type);
}

تهی MatOp_GEMM::add(ثابت Mat_عبارت& e1, ثابت Mat_عبارت& e2, Mat_عبارت& res) ثابت
{
    CV_INSTRUMENT_REGION();

    منطقی i1 = isIdentity(e1), i2 = isIdentity(e2);
    دوگانه alpha1 = i1 ? 1 : e1.alpha, alpha2 = i2 ? 1 : e2.alpha;

    اگر( isMatProd(e1) && (i2 || isScaled(e2) || isT(e2)) )
        MatOp_GEMM::make_عبارت(res, (e1.flags & ~cv::GEMM_3_T)|(isT(e2) ? cv::GEMM_3_T : 0),
                             e1.a, e1.b, alpha1, e2.a, alpha2);
    وگرنه اگر( isMatProd(e2) && (i1 || isScaled(e1) || isT(e1)) )
        MatOp_GEMM::make_عبارت(res, (e2.flags & ~cv::GEMM_3_T)|(isT(e1) ? cv::GEMM_3_T : 0),
                             e2.a, e2.b, alpha2, e1.a, alpha1);
    وگرنه اگر( این == e2.op )
        MatOp::add(e1, e2, res);
    وگرنه
        e2.op->add(e1, e2, res);
}

تهی MatOp_GEMM::subtract(ثابت Mat_عبارت& e1, ثابت Mat_عبارت& e2, Mat_عبارت& res) ثابت
{
    CV_INSTRUMENT_REGION();

    منطقی i1 = isIdentity(e1), i2 = isIdentity(e2);
    دوگانه alpha1 = i1 ? 1 : e1.alpha, alpha2 = i2 ? 1 : e2.alpha;

    اگر( isMatProd(e1) && (i2 || isScaled(e2) || isT(e2)) )
        MatOp_GEMM::make_عبارت(res, (e1.flags & ~cv::GEMM_3_T)|(isT(e2) ? cv::GEMM_3_T : 0),
                             e1.a, e1.b, alpha1, e2.a, -alpha2);
    وگرنه اگر( isMatProd(e2) && (i1 || isScaled(e1) || isT(e1)) )
        MatOp_GEMM::make_عبارت(res, (e2.flags & ~cv::GEMM_3_T)|(isT(e1) ? cv::GEMM_3_T : 0),
                            e2.a, e2.b, -alpha2, e1.a, alpha1);
    وگرنه اگر( این == e2.op )
        MatOp::subtract(e1, e2, res);
    وگرنه
        e2.op->subtract(e1, e2, res);
}

تهی MatOp_GEMM::multiply(ثابت Mat_عبارت& e, دوگانه s, Mat_عبارت& res) ثابت
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha *= s;
    res.beta *= s;
}

تهی MatOp_GEMM::transpose(ثابت Mat_عبارت& e, Mat_عبارت& res) ثابت
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.flags = (!(e.flags & cv::GEMM_1_T) ? cv::GEMM_2_T : 0) |
                (!(e.flags & cv::GEMM_2_T) ? cv::GEMM_1_T : 0) |
                (!(e.flags & cv::GEMM_3_T) ? cv::GEMM_3_T : 0);
    swap(res.a, res.b);
}

درونخطی تهی MatOp_GEMM::make_عبارت(Mat_عبارت& res, صحیح flags, ثابت Mat& a, ثابت Mat& b,
                                 دوگانه alpha, ثابت Mat& c, دوگانه beta)
{
    res = Mat_عبارت(&g_MatOp_GEMM, flags, a, b, c, alpha, beta);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

تهی MatOp_Invert::assign(ثابت Mat_عبارت& e, Mat& m, صحیح _type) ثابت
{
    Mat temp, &dst = _type == -1 || _type == e.a.type() ? m : temp;

    cv::invert(e.a, dst, e.flags);
    اگر( dst.data != m.data )
        dst.convertTo(m, _type);
}

تهی MatOp_Invert::matmul(ثابت Mat_عبارت& e1, ثابت Mat_عبارت& e2, Mat_عبارت& res) ثابت
{
    اگر( isInv(e1) && isIdentity(e2) )
        MatOp_Solve::make_عبارت(res, e1.flags, e1.a, e2.a);
    وگرنه اگر( این == e2.op )
        MatOp::matmul(e1, e2, res);
    وگرنه
        e2.op->matmul(e1, e2, res);
}

درونخطی تهی MatOp_Invert::make_عبارت(Mat_عبارت& res, صحیح method, ثابت Mat& m)
{
    res = Mat_عبارت(&g_MatOp_Invert, method, m, Mat(), Mat(), 1, 0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

تهی MatOp_Solve::assign(ثابت Mat_عبارت& e, Mat& m, صحیح _type) ثابت
{
    Mat temp, &dst = _type == -1 || _type == e.a.type() ? m : temp;

    cv::solve(e.a, e.b, dst, e.flags);
    اگر( dst.data != m.data )
        dst.convertTo(m, _type);
}

درونخطی تهی MatOp_Solve::make_عبارت(Mat_عبارت& res, صحیح method, ثابت Mat& a, ثابت Mat& b)
{
    res = Mat_عبارت(&g_MatOp_Solve, method, a, b, Mat(), 1, 1);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

تهی MatOp_Initializer::assign(ثابت Mat_عبارت& e, Mat& m, صحیح _type) ثابت
{
    اگر( _type == -1 )
        _type = e.a.type();

    m.create(e.a.size, _type);

    اگر( e.flags == 'I' && e.a.dims <= 2 )
        setIdentity(m, Scalar(e.alpha));
    وگرنه اگر( e.flags == '0' )
        m = Scalar();
    وگرنه اگر( e.flags == '1' )
        m = Scalar(e.alpha);
    وگرنه
        CV_Error(cv::Error::StsError, "Invalid matrix initializer type");
}

تهی MatOp_Initializer::multiply(ثابت Mat_عبارت& e, دوگانه s, Mat_عبارت& res) ثابت
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha *= s;
}

درونخطی تهی MatOp_Initializer::make_عبارت(Mat_عبارت& res, صحیح method, Size sz, صحیح type, دوگانه alpha)
{
    res = Mat_عبارت(getGlobalMatOpInitializer(), method, Mat(sz, type, (تهی*)(size_t)0xEEEEEEEE), Mat(), Mat(), alpha, 0);
}

درونخطی تهی MatOp_Initializer::make_عبارت(Mat_عبارت& res, صحیح method, صحیح ndims, ثابت صحیح* sizes, صحیح type, دوگانه alpha)
{
    res = Mat_عبارت(getGlobalMatOpInitializer(), method, Mat(ndims, sizes, type, (تهی*)(size_t)0xEEEEEEEE), Mat(), Mat(), alpha, 0);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

Mat_عبارت Mat::t() ثابت
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(*این);
    Mat_عبارت e;
    MatOp_T::make_عبارت(e, *این);
    بازگشت e;
}

Mat_عبارت Mat::inv(صحیح method) ثابت
{
    CV_INSTRUMENT_REGION();

    Mat_عبارت e;
    MatOp_Invert::make_عبارت(e, method, *این);
    بازگشت e;
}


Mat_عبارت Mat::mul(درون_داد_Array m, دوگانه scale) ثابت
{
    CV_INSTRUMENT_REGION();

    Mat b = m.getMat();
    // Unless the argument is a refcounted Mat/UMat, the header returned by getMat() may be
    // a non-owning view of caller memory (e.g. a scalar bound to _InputArray(const double&),
    // a Matx or a Vec) that the returned MatExpr can outlive, so snapshot it.
    // See https://github.com/opencv/opencv/issues/23577
    اگر( !m.isMat() && !m.isUMat() )
        b = b.clone();

    Mat_عبارت e;
    MatOp_Bin::make_عبارت(e, '*', *این, b, scale);
    بازگشت e;
}

Mat_عبارت Mat::zeros(صحیح rows, صحیح cols, صحیح type)
{
    CV_INSTRUMENT_REGION();

    Mat_عبارت e;
    MatOp_Initializer::make_عبارت(e, '0', Size(cols, rows), type);
    بازگشت e;
}

Mat_عبارت Mat::zeros(Size size, صحیح type)
{
    CV_INSTRUMENT_REGION();

    Mat_عبارت e;
    MatOp_Initializer::make_عبارت(e, '0', size, type);
    بازگشت e;
}

Mat_عبارت Mat::zeros(صحیح ndims, ثابت صحیح* sizes, صحیح type)
{
    CV_INSTRUMENT_REGION();

    Mat_عبارت e;
    MatOp_Initializer::make_عبارت(e, '0', ndims, sizes, type);
    بازگشت e;
}

Mat_عبارت Mat::zeros(ثابت MatShape& shape, صحیح type)
{
    CV_INSTRUMENT_REGION();

    Mat_عبارت e;
    MatOp_Initializer::make_عبارت(e, '0', shape.dims, shape.p, type);
    بازگشت e;
}

Mat_عبارت Mat::ones(صحیح rows, صحیح cols, صحیح type)
{
    CV_INSTRUMENT_REGION();

    Mat_عبارت e;
    MatOp_Initializer::make_عبارت(e, '1', Size(cols, rows), type);
    بازگشت e;
}

Mat_عبارت Mat::ones(Size size, صحیح type)
{
    CV_INSTRUMENT_REGION();

    Mat_عبارت e;
    MatOp_Initializer::make_عبارت(e, '1', size, type);
    بازگشت e;
}

Mat_عبارت Mat::ones(صحیح ndims, ثابت صحیح* sizes, صحیح type)
{
    CV_INSTRUMENT_REGION();

    Mat_عبارت e;
    MatOp_Initializer::make_عبارت(e, '1', ndims, sizes, type);
    بازگشت e;
}

Mat_عبارت Mat::ones(ثابت MatShape& shape, صحیح type)
{
    CV_INSTRUMENT_REGION();

    Mat_عبارت e;
    MatOp_Initializer::make_عبارت(e, '1', shape.dims, shape.p, type);
    بازگشت e;
}

Mat_عبارت Mat::eye(صحیح rows, صحیح cols, صحیح type)
{
    CV_INSTRUMENT_REGION();

    Mat_عبارت e;
    MatOp_Initializer::make_عبارت(e, 'I', Size(cols, rows), type);
    بازگشت e;
}

Mat_عبارت Mat::eye(Size size, صحیح type)
{
    CV_INSTRUMENT_REGION();

    Mat_عبارت e;
    MatOp_Initializer::make_عبارت(e, 'I', size, type);
    بازگشت e;
}

تهی Mat_عبارت::swap(Mat_عبارت& other)
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

درون_داد_Array_2::درون_داد_Array_2(ثابت Mat_عبارت& عبارت)
{
    اگر (!isIdentity(عبارت))
    {
        Mat result = عبارت;  // TODO improve through refcount == 1 of expr.a (inplace operation is possible - except gemm?)
        Mat_عبارت result_عبارت(result);
        swap(const_cast<Mat_عبارت&>(عبارت), result_عبارت);
    }
    CV_Assert(isIdentity(عبارت));
    init(FIXED_TYPE + FIXED_SIZE + MAT + ACCESS_READ, &عبارت.a);
}

} // cv::
