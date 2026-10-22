// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


/* ////////////////////////////////////////////////////////////////////
//
//  Mat basic operations: Copy, Set
//
// */

#ضمن "precomp.hpp"
#ضمن <opencv2/core/utils/logger.hpp>

نطاق cv
{

//This and its overload below are used in various MatExpr operator overloads
//implemented to check that Matrix operands exist.
ساكن فراغ checkOperandsExist(ثابت Mat& a)
{
    إذا (a.empty())
    {
        CV_Error(cv::Error::StsBadArg, "Matrix operand is an empty matrix.");
    }
}

ساكن فراغ checkOperandsExist(ثابت Mat& a, ثابت Mat& b)
{
    إذا (a.empty() || b.empty())
    {
        CV_Error(cv::Error::StsBadArg, "One or more matrix operands are empty.");
    }
}


صنف MatOp_Identity CV_FINAL : عام MatOp
{
عام:
    MatOp_Identity() {}
    virtual ~MatOp_Identity() {}

    منطقي elementWise(ثابت Mat_ئىپادە& /*ئىپادە*/) ثابت CV_OVERRIDE { أرجع true; }
    فراغ assign(ثابت Mat_ئىپادە& ئىپادە, Mat& m, صحيح type=-1) ثابت CV_OVERRIDE;

    ساكن فراغ make_ئىپادە(Mat_ئىپادە& res, ثابت Mat& m);
};

ساكن MatOp_Identity g_MatOp_Identity;

صنف MatOp_AddEx CV_FINAL : عام MatOp
{
عام:
    MatOp_AddEx() {}
    virtual ~MatOp_AddEx() {}

    منطقي elementWise(ثابت Mat_ئىپادە& /*ئىپادە*/) ثابت CV_OVERRIDE { أرجع true; }
    فراغ assign(ثابت Mat_ئىپادە& ئىپادە, Mat& m, صحيح type=-1) ثابت CV_OVERRIDE;

    فراغ add(ثابت Mat_ئىپادە& e1, ثابت Scalar& s, Mat_ئىپادە& res) ثابت CV_OVERRIDE;
    فراغ subtract(ثابت Scalar& s, ثابت Mat_ئىپادە& ئىپادە, Mat_ئىپادە& res) ثابت CV_OVERRIDE;
    فراغ multiply(ثابت Mat_ئىپادە& e1, مزدوج s, Mat_ئىپادە& res) ثابت CV_OVERRIDE;
    فراغ divide(مزدوج s, ثابت Mat_ئىپادە& e, Mat_ئىپادە& res) ثابت CV_OVERRIDE;

    فراغ transpose(ثابت Mat_ئىپادە& e1, Mat_ئىپادە& res) ثابت CV_OVERRIDE;
    فراغ abs(ثابت Mat_ئىپادە& ئىپادە, Mat_ئىپادە& res) ثابت CV_OVERRIDE;

    ساكن فراغ make_ئىپادە(Mat_ئىپادە& res, ثابت Mat& a, ثابت Mat& b, مزدوج alpha, مزدوج beta, ثابت Scalar& s=Scalar());
};

ساكن MatOp_AddEx g_MatOp_AddEx;

صنف MatOp_Bin CV_FINAL : عام MatOp
{
عام:
    MatOp_Bin() {}
    virtual ~MatOp_Bin() {}

    منطقي elementWise(ثابت Mat_ئىپادە& /*ئىپادە*/) ثابت CV_OVERRIDE { أرجع true; }
    فراغ assign(ثابت Mat_ئىپادە& ئىپادە, Mat& m, صحيح type=-1) ثابت CV_OVERRIDE;

    فراغ multiply(ثابت Mat_ئىپادە& e1, مزدوج s, Mat_ئىپادە& res) ثابت CV_OVERRIDE;
    فراغ divide(مزدوج s, ثابت Mat_ئىپادە& e, Mat_ئىپادە& res) ثابت CV_OVERRIDE;

    ساكن فراغ make_ئىپادە(Mat_ئىپادە& res, حرف op, ثابت Mat& a, ثابت Mat& b, مزدوج scale=1);
    ساكن فراغ make_ئىپادە(Mat_ئىپادە& res, حرف op, ثابت Mat& a, ثابت Scalar& s);
};

ساكن MatOp_Bin g_MatOp_Bin;

صنف MatOp_Cmp CV_FINAL : عام MatOp
{
عام:
    MatOp_Cmp() {}
    virtual ~MatOp_Cmp() {}

    منطقي elementWise(ثابت Mat_ئىپادە& /*ئىپادە*/) ثابت CV_OVERRIDE { أرجع true; }
    فراغ assign(ثابت Mat_ئىپادە& ئىپادە, Mat& m, صحيح type=-1) ثابت CV_OVERRIDE;

    ساكن فراغ make_ئىپادە(Mat_ئىپادە& res, صحيح cmpop, ثابت Mat& a, ثابت Mat& b);
    ساكن فراغ make_ئىپادە(Mat_ئىپادە& res, صحيح cmpop, ثابت Mat& a, مزدوج alpha);
};

ساكن MatOp_Cmp g_MatOp_Cmp;

صنف MatOp_GEMM CV_FINAL : عام MatOp
{
عام:
    MatOp_GEMM() {}
    virtual ~MatOp_GEMM() {}

    منطقي elementWise(ثابت Mat_ئىپادە& /*ئىپادە*/) ثابت CV_OVERRIDE { أرجع خطأ; }
    فراغ assign(ثابت Mat_ئىپادە& ئىپادە, Mat& m, صحيح type=-1) ثابت CV_OVERRIDE;

    فراغ add(ثابت Mat_ئىپادە& e1, ثابت Mat_ئىپادە& e2, Mat_ئىپادە& res) ثابت CV_OVERRIDE;
    فراغ subtract(ثابت Mat_ئىپادە& e1, ثابت Mat_ئىپادە& e2, Mat_ئىپادە& res) ثابت CV_OVERRIDE;
    فراغ multiply(ثابت Mat_ئىپادە& e, مزدوج s, Mat_ئىپادە& res) ثابت CV_OVERRIDE;

    فراغ transpose(ثابت Mat_ئىپادە& ئىپادە, Mat_ئىپادە& res) ثابت CV_OVERRIDE;

    Size size(ثابت Mat_ئىپادە& ئىپادە) ثابت CV_OVERRIDE
    {
        أرجع Size(
            (ئىپادە.flags & GEMM_2_T) ? ئىپادە.b.rows : ئىپادە.b.cols,
            (ئىپادە.flags & GEMM_1_T) ? ئىپادە.a.cols : ئىپادە.a.rows
        );
    }

    ساكن فراغ make_ئىپادە(Mat_ئىپادە& res, صحيح flags, ثابت Mat& a, ثابت Mat& b,
                         مزدوج alpha=1, ثابت Mat& c=Mat(), مزدوج beta=1);

};

ساكن MatOp_GEMM g_MatOp_GEMM;

صنف MatOp_Invert CV_FINAL : عام MatOp
{
عام:
    MatOp_Invert() {}
    virtual ~MatOp_Invert() {}

    منطقي elementWise(ثابت Mat_ئىپادە& /*ئىپادە*/) ثابت CV_OVERRIDE { أرجع خطأ; }
    فراغ assign(ثابت Mat_ئىپادە& ئىپادە, Mat& m, صحيح type=-1) ثابت CV_OVERRIDE;

    فراغ matmul(ثابت Mat_ئىپادە& expr1, ثابت Mat_ئىپادە& expr2, Mat_ئىپادە& res) ثابت CV_OVERRIDE;

    ساكن فراغ make_ئىپادە(Mat_ئىپادە& res, صحيح method, ثابت Mat& m);
};

ساكن MatOp_Invert g_MatOp_Invert;

صنف MatOp_T CV_FINAL : عام MatOp
{
عام:
    MatOp_T() {}
    virtual ~MatOp_T() {}

    منطقي elementWise(ثابت Mat_ئىپادە& /*ئىپادە*/) ثابت CV_OVERRIDE { أرجع خطأ; }
    فراغ assign(ثابت Mat_ئىپادە& ئىپادە, Mat& m, صحيح type=-1) ثابت CV_OVERRIDE;

    فراغ multiply(ثابت Mat_ئىپادە& e1, مزدوج s, Mat_ئىپادە& res) ثابت CV_OVERRIDE;
    فراغ transpose(ثابت Mat_ئىپادە& ئىپادە, Mat_ئىپادە& res) ثابت CV_OVERRIDE;

    ساكن فراغ make_ئىپادە(Mat_ئىپادە& res, ثابت Mat& a, مزدوج alpha=1);
};

ساكن MatOp_T g_MatOp_T;

صنف MatOp_Solve CV_FINAL : عام MatOp
{
عام:
    MatOp_Solve() {}
    virtual ~MatOp_Solve() {}

    منطقي elementWise(ثابت Mat_ئىپادە& /*ئىپادە*/) ثابت CV_OVERRIDE { أرجع خطأ; }
    فراغ assign(ثابت Mat_ئىپادە& ئىپادە, Mat& m, صحيح type=-1) ثابت CV_OVERRIDE;

    ساكن فراغ make_ئىپادە(Mat_ئىپادە& res, صحيح method, ثابت Mat& a, ثابت Mat& b);
};

ساكن MatOp_Solve g_MatOp_Solve;

صنف MatOp_Initializer CV_FINAL : عام MatOp
{
عام:
    MatOp_Initializer() {}
    virtual ~MatOp_Initializer() {}

    منطقي elementWise(ثابت Mat_ئىپادە& /*ئىپادە*/) ثابت CV_OVERRIDE { أرجع خطأ; }
    فراغ assign(ثابت Mat_ئىپادە& ئىپادە, Mat& m, صحيح type=-1) ثابت CV_OVERRIDE;

    فراغ multiply(ثابت Mat_ئىپادە& e, مزدوج s, Mat_ئىپادە& res) ثابت CV_OVERRIDE;

    ساكن فراغ make_ئىپادە(Mat_ئىپادە& res, صحيح method, Size sz, صحيح type, مزدوج alpha=1);
    ساكن فراغ make_ئىپادە(Mat_ئىپادە& res, صحيح method, صحيح ndims, ثابت صحيح* sizes, صحيح type, مزدوج alpha=1);
};

ساكن MatOp_Initializer* getGlobalMatOpInitializer()
{
    CV_SINGLETON_LAZY_INIT(MatOp_Initializer, جديد MatOp_Initializer())
}

ساكن مضمن منطقي isIdentity(ثابت Mat_ئىپادە& e) { أرجع e.op == &g_MatOp_Identity; }
ساكن مضمن منطقي isAddEx(ثابت Mat_ئىپادە& e) { أرجع e.op == &g_MatOp_AddEx; }
ساكن مضمن منطقي isScaled(ثابت Mat_ئىپادە& e) { أرجع isAddEx(e) && (!e.b.data || e.beta == 0) && e.s == Scalar(); }
ساكن مضمن منطقي isBin(ثابت Mat_ئىپادە& e, حرف c) { أرجع e.op == &g_MatOp_Bin && e.flags == c; }
ساكن مضمن منطقي isCmp(ثابت Mat_ئىپادە& e) { أرجع e.op == &g_MatOp_Cmp; }
ساكن مضمن منطقي isReciprocal(ثابت Mat_ئىپادە& e) { أرجع isBin(e,'/') && (!e.b.data || e.beta == 0); }
ساكن مضمن منطقي isT(ثابت Mat_ئىپادە& e) { أرجع e.op == &g_MatOp_T; }
ساكن مضمن منطقي isInv(ثابت Mat_ئىپادە& e) { أرجع e.op == &g_MatOp_Invert; }
ساكن مضمن منطقي isSolve(ثابت Mat_ئىپادە& e) { أرجع e.op == &g_MatOp_Solve; }
//static inline bool isGEMM(const MatExpr& e) { return e.op == &g_MatOp_GEMM; }
ساكن مضمن منطقي isMatProd(ثابت Mat_ئىپادە& e) { أرجع e.op == &g_MatOp_GEMM && (!e.c.data || e.beta == 0); }
ساكن مضمن منطقي isInitializer(ثابت Mat_ئىپادە& e) { أرجع e.op == getGlobalMatOpInitializer(); }

/////////////////////////////////////////////////////////////////////////////////////////////////////

MatOp::MatOp() {}
MatOp::~MatOp() {}


منطقي MatOp::elementWise(ثابت Mat_ئىپادە& /*ئىپادە*/) ثابت
{
    أرجع خطأ;
}

فراغ MatOp::roi(ثابت Mat_ئىپادە& ئىپادە, ثابت Range& rowRange, ثابت Range& colRange, Mat_ئىپادە& e) ثابت
{
    إذا( elementWise(ئىپادە) )
    {
        e = Mat_ئىپادە(ئىپادە.op, ئىپادە.flags, Mat(), Mat(), Mat(),
                    ئىپادە.alpha, ئىپادە.beta, ئىپادە.s);
        إذا(ئىپادە.a.data)
            e.a = ئىپادە.a(rowRange, colRange);
        إذا(ئىپادە.b.data)
            e.b = ئىپادە.b(rowRange, colRange);
        إذا(ئىپادە.c.data)
            e.c = ئىپادە.c(rowRange, colRange);
    }
    وإلا
    {
        Mat m;
        ئىپادە.op->assign(ئىپادە, m);
        e = Mat_ئىپادە(&g_MatOp_Identity, 0, m(rowRange, colRange), Mat(), Mat());
    }
}

فراغ MatOp::diag(ثابت Mat_ئىپادە& ئىپادە, صحيح d, Mat_ئىپادە& e) ثابت
{
    إذا( elementWise(ئىپادە) )
    {
        e = Mat_ئىپادە(ئىپادە.op, ئىپادە.flags, Mat(), Mat(), Mat(),
                    ئىپادە.alpha, ئىپادە.beta, ئىپادە.s);
        إذا(ئىپادە.a.data)
            e.a = ئىپادە.a.diag(d);
        إذا(ئىپادە.b.data)
            e.b = ئىپادە.b.diag(d);
        إذا(ئىپادە.c.data)
            e.c = ئىپادە.c.diag(d);
    }
    وإلا
    {
        Mat m;
        ئىپادە.op->assign(ئىپادە, m);
        e = Mat_ئىپادە(&g_MatOp_Identity, 0, m.diag(d), Mat(), Mat());
    }
}


فراغ MatOp::augAssignAdd(ثابت Mat_ئىپادە& ئىپادە, Mat& m) ثابت
{
    Mat temp;
    ئىپادە.op->assign(ئىپادە, temp);
    m += temp;
}


فراغ MatOp::augAssignSubtract(ثابت Mat_ئىپادە& ئىپادە, Mat& m) ثابت
{
    Mat temp;
    ئىپادە.op->assign(ئىپادە, temp);
    m -= temp;
}


فراغ MatOp::augAssignMultiply(ثابت Mat_ئىپادە& ئىپادە, Mat& m) ثابت
{
    Mat temp;
    ئىپادە.op->assign(ئىپادە, temp);
    m *= temp;
}


فراغ MatOp::augAssignDivide(ثابت Mat_ئىپادە& ئىپادە, Mat& m) ثابت
{
    Mat temp;
    ئىپادە.op->assign(ئىپادە, temp);
    m /= temp;
}


فراغ MatOp::augAssignAnd(ثابت Mat_ئىپادە& ئىپادە, Mat& m) ثابت
{
    Mat temp;
    ئىپادە.op->assign(ئىپادە, temp);
    m &= temp;
}


فراغ MatOp::augAssignOr(ثابت Mat_ئىپادە& ئىپادە, Mat& m) ثابت
{
    Mat temp;
    ئىپادە.op->assign(ئىپادە, temp);
    m |= temp;
}


فراغ MatOp::augAssignXor(ثابت Mat_ئىپادە& ئىپادە, Mat& m) ثابت
{
    Mat temp;
    ئىپادە.op->assign(ئىپادە, temp);
    m ^= temp;
}


فراغ MatOp::add(ثابت Mat_ئىپادە& e1, ثابت Mat_ئىپادە& e2, Mat_ئىپادە& res) ثابت
{
    CV_INSTRUMENT_REGION();

    إذا( هذا == e2.op )
    {
        مزدوج alpha = 1, beta = 1;
        Scalar s;
        Mat m1, m2;
        إذا( isAddEx(e1) && (!e1.b.data || e1.beta == 0) )
        {
            m1 = e1.a;
            alpha = e1.alpha;
            s = e1.s;
        }
        وإلا
            e1.op->assign(e1, m1);

        إذا( isAddEx(e2) && (!e2.b.data || e2.beta == 0) )
        {
            m2 = e2.a;
            beta = e2.alpha;
            s += e2.s;
        }
        وإلا
            e2.op->assign(e2, m2);
        MatOp_AddEx::make_ئىپادە(res, m1, m2, alpha, beta, s);
    }
    وإلا
        e2.op->add(e1, e2, res);
}


فراغ MatOp::add(ثابت Mat_ئىپادە& expr1, ثابت Scalar& s, Mat_ئىپادە& res) ثابت
{
    CV_INSTRUMENT_REGION();

    Mat m1;
    expr1.op->assign(expr1, m1);
    MatOp_AddEx::make_ئىپادە(res, m1, Mat(), 1, 0, s);
}


فراغ MatOp::subtract(ثابت Mat_ئىپادە& e1, ثابت Mat_ئىپادە& e2, Mat_ئىپادە& res) ثابت
{
    CV_INSTRUMENT_REGION();

    إذا( هذا == e2.op )
    {
        مزدوج alpha = 1, beta = -1;
        Scalar s;
        Mat m1, m2;
        إذا( isAddEx(e1) && (!e1.b.data || e1.beta == 0) )
        {
            m1 = e1.a;
            alpha = e1.alpha;
            s = e1.s;
        }
        وإلا
            e1.op->assign(e1, m1);

        إذا( isAddEx(e2) && (!e2.b.data || e2.beta == 0) )
        {
            m2 = e2.a;
            beta = -e2.alpha;
            s -= e2.s;
        }
        وإلا
            e2.op->assign(e2, m2);
        MatOp_AddEx::make_ئىپادە(res, m1, m2, alpha, beta, s);
    }
    وإلا
        e2.op->subtract(e1, e2, res);
}


فراغ MatOp::subtract(ثابت Scalar& s, ثابت Mat_ئىپادە& ئىپادە, Mat_ئىپادە& res) ثابت
{
    CV_INSTRUMENT_REGION();

    Mat m;
    ئىپادە.op->assign(ئىپادە, m);
    MatOp_AddEx::make_ئىپادە(res, m, Mat(), -1, 0, s);
}


فراغ MatOp::multiply(ثابت Mat_ئىپادە& e1, ثابت Mat_ئىپادە& e2, Mat_ئىپادە& res, مزدوج scale) ثابت
{
    CV_INSTRUMENT_REGION();

    إذا( هذا == e2.op )
    {
        Mat m1, m2;

        إذا( isReciprocal(e1) )
        {
            إذا( isScaled(e2) )
            {
                scale *= e2.alpha;
                m2 = e2.a;
            }
            وإلا
                e2.op->assign(e2, m2);

            MatOp_Bin::make_ئىپادە(res, '/', m2, e1.a, scale/e1.alpha);
        }
        وإلا
        {
            حرف op = '*';
            إذا( isScaled(e1) )
            {
                m1 = e1.a;
                scale *= e1.alpha;
            }
            وإلا
                e1.op->assign(e1, m1);

            إذا( isScaled(e2) )
            {
                m2 = e2.a;
                scale *= e2.alpha;
            }
            وإلا إذا( isReciprocal(e2) )
            {
                op = '/';
                m2 = e2.a;
                scale *= e2.alpha;
            }
            وإلا
                e2.op->assign(e2, m2);

            MatOp_Bin::make_ئىپادە(res, op, m1, m2, scale);
        }
    }
    وإلا
        e2.op->multiply(e1, e2, res, scale);
}


فراغ MatOp::multiply(ثابت Mat_ئىپادە& ئىپادە, مزدوج s, Mat_ئىپادە& res) ثابت
{
    CV_INSTRUMENT_REGION();

    Mat m;
    ئىپادە.op->assign(ئىپادە, m);
    MatOp_AddEx::make_ئىپادە(res, m, Mat(), s, 0);
}


فراغ MatOp::divide(ثابت Mat_ئىپادە& e1, ثابت Mat_ئىپادە& e2, Mat_ئىپادە& res, مزدوج scale) ثابت
{
    CV_INSTRUMENT_REGION();

    إذا( هذا == e2.op )
    {
        إذا( isReciprocal(e1) && isReciprocal(e2) )
            MatOp_Bin::make_ئىپادە(res, '/', e2.a, e1.a, e1.alpha/e2.alpha);
        وإلا
        {
            Mat m1, m2;
            حرف op = '/';

            إذا( isScaled(e1) )
            {
                m1 = e1.a;
                scale *= e1.alpha;
            }
            وإلا
                e1.op->assign(e1, m1);

            إذا( isScaled(e2) )
            {
                m2 = e2.a;
                scale /= e2.alpha;
            }
            وإلا إذا( isReciprocal(e2) )
            {
                m2 = e2.a;
                scale /= e2.alpha;
                op = '*';
            }
            وإلا
                e2.op->assign(e2, m2);
            MatOp_Bin::make_ئىپادە(res, op, m1, m2, scale);
        }
    }
    وإلا
        e2.op->divide(e1, e2, res, scale);
}


فراغ MatOp::divide(مزدوج s, ثابت Mat_ئىپادە& ئىپادە, Mat_ئىپادە& res) ثابت
{
    CV_INSTRUMENT_REGION();

    Mat m;
    ئىپادە.op->assign(ئىپادە, m);
    MatOp_Bin::make_ئىپادە(res, '/', m, Mat(), s);
}


فراغ MatOp::abs(ثابت Mat_ئىپادە& ئىپادە, Mat_ئىپادە& res) ثابت
{
    CV_INSTRUMENT_REGION();

    Mat m;
    ئىپادە.op->assign(ئىپادە, m);
    MatOp_Bin::make_ئىپادە(res, 'a', m, Mat());
}


فراغ MatOp::transpose(ثابت Mat_ئىپادە& ئىپادە, Mat_ئىپادە& res) ثابت
{
    CV_INSTRUMENT_REGION();

    Mat m;
    ئىپادە.op->assign(ئىپادە, m);
    MatOp_T::make_ئىپادە(res, m, 1);
}


فراغ MatOp::matmul(ثابت Mat_ئىپادە& e1, ثابت Mat_ئىپادە& e2, Mat_ئىپادە& res) ثابت
{
    إذا( هذا == e2.op )
    {
        مزدوج scale = 1;
        صحيح flags = 0;
        Mat m1, m2;

        إذا( isT(e1) )
        {
            flags = cv::GEMM_1_T;
            scale = e1.alpha;
            m1 = e1.a;
        }
        وإلا إذا( isScaled(e1) )
        {
            scale = e1.alpha;
            m1 = e1.a;
        }
        وإلا
            e1.op->assign(e1, m1);

        إذا( isT(e2) )
        {
            flags |= cv::GEMM_2_T;
            scale *= e2.alpha;
            m2 = e2.a;
        }
        وإلا إذا( isScaled(e2) )
        {
            scale *= e2.alpha;
            m2 = e2.a;
        }
        وإلا
            e2.op->assign(e2, m2);

        MatOp_GEMM::make_ئىپادە(res, flags, m1, m2, scale);
    }
    وإلا
        e2.op->matmul(e1, e2, res);
}


فراغ MatOp::invert(ثابت Mat_ئىپادە& ئىپادە, صحيح method, Mat_ئىپادە& res) ثابت
{
    Mat m;
    ئىپادە.op->assign(ئىپادە, m);
    MatOp_Invert::make_ئىپادە(res, method, m);
}


Size MatOp::size(ثابت Mat_ئىپادە& ئىپادە) ثابت
{
    أرجع !ئىپادە.a.empty() ? ئىپادە.a.size() : ئىپادە.b.empty() ? ئىپادە.b.size() : ئىپادە.c.size();
}

صحيح MatOp::type(ثابت Mat_ئىپادە& ئىپادە) ثابت
{
    CV_INSTRUMENT_REGION();

    أرجع !ئىپادە.a.empty() ? ئىپادە.a.type() : ئىپادە.b.empty() ? ئىپادە.b.type() : ئىپادە.c.type();
}

//////////////////////////////////////////////////////////////////////////////////////////////////

Mat_ئىپادە::Mat_ئىپادە(ثابت Mat& m) : op(&g_MatOp_Identity), flags(0), a(m), b(Mat()), c(Mat()), alpha(1), beta(0), s(Scalar())
{
}

Mat_ئىپادە Mat_ئىپادە::row(صحيح y) ثابت
{
    Mat_ئىپادە e;
    op->roi(*هذا, Range(y, y+1), Range::all(), e);
    أرجع e;
}

Mat_ئىپادە Mat_ئىپادە::col(صحيح x) ثابت
{
    Mat_ئىپادە e;
    op->roi(*هذا, Range::all(), Range(x, x+1), e);
    أرجع e;
}

Mat_ئىپادە Mat_ئىپادە::diag(صحيح d) ثابت
{
    Mat_ئىپادە e;
    op->diag(*هذا, d, e);
    أرجع e;
}

Mat_ئىپادە Mat_ئىپادە::operator()( ثابت Range& rowRange, ثابت Range& colRange ) ثابت
{
    Mat_ئىپادە e;
    op->roi(*هذا, rowRange, colRange, e);
    أرجع e;
}

Mat_ئىپادە Mat_ئىپادە::operator()( ثابت Rect& roi ) ثابت
{
    Mat_ئىپادە e;
    op->roi(*هذا, Range(roi.y, roi.y + roi.height), Range(roi.x, roi.x + roi.width), e);
    أرجع e;
}

Mat Mat_ئىپادە::cross(ثابت Mat& m) ثابت
{
    أرجع ((Mat)*هذا).cross(m);
}

مزدوج Mat_ئىپادە::dot(ثابت Mat& m) ثابت
{
    أرجع ((Mat)*هذا).dot(m);
}

Mat_ئىپادە Mat_ئىپادە::t() ثابت
{
    Mat_ئىپادە e;
    op->transpose(*هذا, e);
    أرجع e;
}

Mat_ئىپادە Mat_ئىپادە::inv(صحيح method) ثابت
{
    Mat_ئىپادە e;
    op->invert(*هذا, method, e);
    أرجع e;
}

Mat_ئىپادە Mat_ئىپادە::mul(ثابت Mat_ئىپادە& e, مزدوج scale) ثابت
{
    Mat_ئىپادە en;
    op->multiply(*هذا, e, en, scale);
    أرجع en;
}

Mat_ئىپادە Mat_ئىپادە::mul(ثابت Mat& m, مزدوج scale) ثابت
{
    Mat_ئىپادە e;
    op->multiply(*هذا, Mat_ئىپادە(m), e, scale);
    أرجع e;
}

Mat_ئىپادە operator + (ثابت Mat& a, ثابت Mat& b)
{
    checkOperandsExist(a, b);
    Mat_ئىپادە e;
    MatOp_AddEx::make_ئىپادە(e, a, b, 1, 1);
    أرجع e;
}

Mat_ئىپادە operator + (ثابت Mat& a, ثابت Scalar& s)
{
    checkOperandsExist(a);
    Mat_ئىپادە e;
    MatOp_AddEx::make_ئىپادە(e, a, Mat(), 1, 0, s);
    أرجع e;
}

Mat_ئىپادە operator + (ثابت Scalar& s, ثابت Mat& a)
{
    checkOperandsExist(a);
    Mat_ئىپادە e;
    MatOp_AddEx::make_ئىپادە(e, a, Mat(), 1, 0, s);
    أرجع e;
}

Mat_ئىپادە operator + (ثابت Mat_ئىپادە& e, ثابت Mat& m)
{
    Mat_ئىپادە en;
    e.op->add(e, Mat_ئىپادە(m), en);
    أرجع en;
}

Mat_ئىپادە operator + (ثابت Mat& m, ثابت Mat_ئىپادە& e)
{
    checkOperandsExist(m);
    Mat_ئىپادە en;
    e.op->add(e, Mat_ئىپادە(m), en);
    أرجع en;
}

Mat_ئىپادە operator + (ثابت Mat_ئىپادە& e, ثابت Scalar& s)
{
    Mat_ئىپادە en;
    e.op->add(e, s, en);
    أرجع en;
}

Mat_ئىپادە operator + (ثابت Scalar& s, ثابت Mat_ئىپادە& e)
{
    Mat_ئىپادە en;
    e.op->add(e, s, en);
    أرجع en;
}

Mat_ئىپادە operator + (ثابت Mat_ئىپادە& e1, ثابت Mat_ئىپادە& e2)
{
    Mat_ئىپادە en;
    e1.op->add(e1, e2, en);
    أرجع en;
}

Mat_ئىپادە operator - (ثابت Mat& a, ثابت Mat& b)
{
    checkOperandsExist(a, b);
    Mat_ئىپادە e;
    MatOp_AddEx::make_ئىپادە(e, a, b, 1, -1);
    أرجع e;
}

Mat_ئىپادە operator - (ثابت Mat& a, ثابت Scalar& s)
{
    checkOperandsExist(a);
    Mat_ئىپادە e;
    MatOp_AddEx::make_ئىپادە(e, a, Mat(), 1, 0, -s);
    أرجع e;
}

Mat_ئىپادە operator - (ثابت Scalar& s, ثابت Mat& a)
{
    checkOperandsExist(a);
    Mat_ئىپادە e;
    MatOp_AddEx::make_ئىپادە(e, a, Mat(), -1, 0, s);
    أرجع e;
}

Mat_ئىپادە operator - (ثابت Mat_ئىپادە& e, ثابت Mat& m)
{
    checkOperandsExist(m);
    Mat_ئىپادە en;
    e.op->subtract(e, Mat_ئىپادە(m), en);
    أرجع en;
}

Mat_ئىپادە operator - (ثابت Mat& m, ثابت Mat_ئىپادە& e)
{
    checkOperandsExist(m);
    Mat_ئىپادە en;
    e.op->subtract(Mat_ئىپادە(m), e, en);
    أرجع en;
}

Mat_ئىپادە operator - (ثابت Mat_ئىپادە& e, ثابت Scalar& s)
{
    Mat_ئىپادە en;
    e.op->add(e, -s, en);
    أرجع en;
}

Mat_ئىپادە operator - (ثابت Scalar& s, ثابت Mat_ئىپادە& e)
{
    Mat_ئىپادە en;
    e.op->subtract(s, e, en);
    أرجع en;
}

Mat_ئىپادە operator - (ثابت Mat_ئىپادە& e1, ثابت Mat_ئىپادە& e2)
{
    Mat_ئىپادە en;
    e1.op->subtract(e1, e2, en);
    أرجع en;
}

Mat_ئىپادە operator - (ثابت Mat& m)
{
    checkOperandsExist(m);
    Mat_ئىپادە e;
    MatOp_AddEx::make_ئىپادە(e, m, Mat(), -1, 0);
    أرجع e;
}

Mat_ئىپادە operator - (ثابت Mat_ئىپادە& e)
{
    Mat_ئىپادە en;
    e.op->subtract(Scalar(0), e, en);
    أرجع en;
}

Mat_ئىپادە operator * (ثابت Mat& a, ثابت Mat& b)
{
    checkOperandsExist(a, b);
    Mat_ئىپادە e;
    MatOp_GEMM::make_ئىپادە(e, 0, a, b);
    أرجع e;
}

Mat_ئىپادە operator * (ثابت Mat& a, مزدوج s)
{
    checkOperandsExist(a);
    Mat_ئىپادە e;
    MatOp_AddEx::make_ئىپادە(e, a, Mat(), s, 0);
    أرجع e;
}

Mat_ئىپادە operator * (مزدوج s, ثابت Mat& a)
{
    checkOperandsExist(a);
    Mat_ئىپادە e;
    MatOp_AddEx::make_ئىپادە(e, a, Mat(), s, 0);
    أرجع e;
}

Mat_ئىپادە operator * (ثابت Mat_ئىپادە& e, ثابت Mat& m)
{
    Mat_ئىپادە en;
    e.op->matmul(e, Mat_ئىپادە(m), en);
    أرجع en;
}

Mat_ئىپادە operator * (ثابت Mat& m, ثابت Mat_ئىپادە& e)
{
    checkOperandsExist(m);
    Mat_ئىپادە en;
    e.op->matmul(Mat_ئىپادە(m), e, en);
    أرجع en;
}

Mat_ئىپادە operator * (ثابت Mat_ئىپادە& e, مزدوج s)
{
    Mat_ئىپادە en;
    e.op->multiply(e, s, en);
    أرجع en;
}

Mat_ئىپادە operator * (مزدوج s, ثابت Mat_ئىپادە& e)
{
    Mat_ئىپادە en;
    e.op->multiply(e, s, en);
    أرجع en;
}

Mat_ئىپادە operator * (ثابت Mat_ئىپادە& e1, ثابت Mat_ئىپادە& e2)
{
    Mat_ئىپادە en;
    e1.op->matmul(e1, e2, en);
    أرجع en;
}

Mat_ئىپادە operator / (ثابت Mat& a, ثابت Mat& b)
{
    checkOperandsExist(a, b);
    Mat_ئىپادە e;
    MatOp_Bin::make_ئىپادە(e, '/', a, b);
    أرجع e;
}

Mat_ئىپادە operator / (ثابت Mat& a, مزدوج s)
{
    checkOperandsExist(a);
    Mat_ئىپادە e;
    MatOp_AddEx::make_ئىپادە(e, a, Mat(), 1./s, 0);
    أرجع e;
}

Mat_ئىپادە operator / (مزدوج s, ثابت Mat& a)
{
    checkOperandsExist(a);
    Mat_ئىپادە e;
    MatOp_Bin::make_ئىپادە(e, '/', a, Mat(), s);
    أرجع e;
}

Mat_ئىپادە operator / (ثابت Mat_ئىپادە& e, ثابت Mat& m)
{
    Mat_ئىپادە en;
    e.op->divide(e, Mat_ئىپادە(m), en);
    أرجع en;
}

Mat_ئىپادە operator / (ثابت Mat& m, ثابت Mat_ئىپادە& e)
{
    checkOperandsExist(m);
    Mat_ئىپادە en;
    e.op->divide(Mat_ئىپادە(m), e, en);
    أرجع en;
}

Mat_ئىپادە operator / (ثابت Mat_ئىپادە& e, مزدوج s)
{
    Mat_ئىپادە en;
    e.op->multiply(e, 1./s, en);
    أرجع en;
}

Mat_ئىپادە operator / (مزدوج s, ثابت Mat_ئىپادە& e)
{
    Mat_ئىپادە en;
    e.op->divide(s, e, en);
    أرجع en;
}

Mat_ئىپادە operator / (ثابت Mat_ئىپادە& e1, ثابت Mat_ئىپادە& e2)
{
    Mat_ئىپادە en;
    e1.op->divide(e1, e2, en);
    أرجع en;
}

Mat_ئىپادە operator < (ثابت Mat& a, ثابت Mat& b)
{
    checkOperandsExist(a, b);
    Mat_ئىپادە e;
    MatOp_Cmp::make_ئىپادە(e, cv::CMP_LT, a, b);
    أرجع e;
}

Mat_ئىپادە operator < (ثابت Mat& a, مزدوج s)
{
    checkOperandsExist(a);
    Mat_ئىپادە e;
    MatOp_Cmp::make_ئىپادە(e, cv::CMP_LT, a, s);
    أرجع e;
}

Mat_ئىپادە operator < (مزدوج s, ثابت Mat& a)
{
    checkOperandsExist(a);
    Mat_ئىپادە e;
    MatOp_Cmp::make_ئىپادە(e, cv::CMP_GT, a, s);
    أرجع e;
}

Mat_ئىپادە operator <= (ثابت Mat& a, ثابت Mat& b)
{
    checkOperandsExist(a, b);
    Mat_ئىپادە e;
    MatOp_Cmp::make_ئىپادە(e, cv::CMP_LE, a, b);
    أرجع e;
}

Mat_ئىپادە operator <= (ثابت Mat& a, مزدوج s)
{
    checkOperandsExist(a);
    Mat_ئىپادە e;
    MatOp_Cmp::make_ئىپادە(e, cv::CMP_LE, a, s);
    أرجع e;
}

Mat_ئىپادە operator <= (مزدوج s, ثابت Mat& a)
{
    checkOperandsExist(a);
    Mat_ئىپادە e;
    MatOp_Cmp::make_ئىپادە(e, cv::CMP_GE, a, s);
    أرجع e;
}

Mat_ئىپادە operator == (ثابت Mat& a, ثابت Mat& b)
{
    checkOperandsExist(a, b);
    Mat_ئىپادە e;
    MatOp_Cmp::make_ئىپادە(e, cv::CMP_EQ, a, b);
    أرجع e;
}

Mat_ئىپادە operator == (ثابت Mat& a, مزدوج s)
{
    checkOperandsExist(a);
    Mat_ئىپادە e;
    MatOp_Cmp::make_ئىپادە(e, cv::CMP_EQ, a, s);
    أرجع e;
}

Mat_ئىپادە operator == (مزدوج s, ثابت Mat& a)
{
    checkOperandsExist(a);
    Mat_ئىپادە e;
    MatOp_Cmp::make_ئىپادە(e, cv::CMP_EQ, a, s);
    أرجع e;
}

Mat_ئىپادە operator != (ثابت Mat& a, ثابت Mat& b)
{
    checkOperandsExist(a, b);
    Mat_ئىپادە e;
    MatOp_Cmp::make_ئىپادە(e, cv::CMP_NE, a, b);
    أرجع e;
}

Mat_ئىپادە operator != (ثابت Mat& a, مزدوج s)
{
    checkOperandsExist(a);
    Mat_ئىپادە e;
    MatOp_Cmp::make_ئىپادە(e, cv::CMP_NE, a, s);
    أرجع e;
}

Mat_ئىپادە operator != (مزدوج s, ثابت Mat& a)
{
    checkOperandsExist(a);
    Mat_ئىپادە e;
    MatOp_Cmp::make_ئىپادە(e, cv::CMP_NE, a, s);
    أرجع e;
}

Mat_ئىپادە operator >= (ثابت Mat& a, ثابت Mat& b)
{
    checkOperandsExist(a, b);
    Mat_ئىپادە e;
    MatOp_Cmp::make_ئىپادە(e, cv::CMP_GE, a, b);
    أرجع e;
}

Mat_ئىپادە operator >= (ثابت Mat& a, مزدوج s)
{
    checkOperandsExist(a);
    Mat_ئىپادە e;
    MatOp_Cmp::make_ئىپادە(e, cv::CMP_GE, a, s);
    أرجع e;
}

Mat_ئىپادە operator >= (مزدوج s, ثابت Mat& a)
{
    checkOperandsExist(a);
    Mat_ئىپادە e;
    MatOp_Cmp::make_ئىپادە(e, cv::CMP_LE, a, s);
    أرجع e;
}

Mat_ئىپادە operator > (ثابت Mat& a, ثابت Mat& b)
{
    checkOperandsExist(a, b);
    Mat_ئىپادە e;
    MatOp_Cmp::make_ئىپادە(e, cv::CMP_GT, a, b);
    أرجع e;
}

Mat_ئىپادە operator > (ثابت Mat& a, مزدوج s)
{
    checkOperandsExist(a);
    Mat_ئىپادە e;
    MatOp_Cmp::make_ئىپادە(e, cv::CMP_GT, a, s);
    أرجع e;
}

Mat_ئىپادە operator > (مزدوج s, ثابت Mat& a)
{
    checkOperandsExist(a);
    Mat_ئىپادە e;
    MatOp_Cmp::make_ئىپادە(e, cv::CMP_LT, a, s);
    أرجع e;
}

Mat_ئىپادە min(ثابت Mat& a, ثابت Mat& b)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a, b);
    Mat_ئىپادە e;
    MatOp_Bin::make_ئىپادە(e, 'm', a, b);
    أرجع e;
}

Mat_ئىپادە min(ثابت Mat& a, مزدوج s)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    Mat_ئىپادە e;
    MatOp_Bin::make_ئىپادە(e, 'n', a, s);
    أرجع e;
}

Mat_ئىپادە min(مزدوج s, ثابت Mat& a)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    Mat_ئىپادە e;
    MatOp_Bin::make_ئىپادە(e, 'n', a, s);
    أرجع e;
}

Mat_ئىپادە max(ثابت Mat& a, ثابت Mat& b)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a, b);
    Mat_ئىپادە e;
    MatOp_Bin::make_ئىپادە(e, 'M', a, b);
    أرجع e;
}

Mat_ئىپادە max(ثابت Mat& a, مزدوج s)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    Mat_ئىپادە e;
    MatOp_Bin::make_ئىپادە(e, 'N', a, s);
    أرجع e;
}

Mat_ئىپادە max(مزدوج s, ثابت Mat& a)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    Mat_ئىپادە e;
    MatOp_Bin::make_ئىپادە(e, 'N', a, s);
    أرجع e;
}

Mat_ئىپادە operator & (ثابت Mat& a, ثابت Mat& b)
{
    checkOperandsExist(a, b);
    Mat_ئىپادە e;
    MatOp_Bin::make_ئىپادە(e, '&', a, b);
    أرجع e;
}

Mat_ئىپادە operator & (ثابت Mat& a, ثابت Scalar& s)
{
    checkOperandsExist(a);
    Mat_ئىپادە e;
    MatOp_Bin::make_ئىپادە(e, '&', a, s);
    أرجع e;
}

Mat_ئىپادە operator & (ثابت Scalar& s, ثابت Mat& a)
{
    checkOperandsExist(a);
    Mat_ئىپادە e;
    MatOp_Bin::make_ئىپادە(e, '&', a, s);
    أرجع e;
}

Mat_ئىپادە operator | (ثابت Mat& a, ثابت Mat& b)
{
    checkOperandsExist(a, b);
    Mat_ئىپادە e;
    MatOp_Bin::make_ئىپادە(e, '|', a, b);
    أرجع e;
}

Mat_ئىپادە operator | (ثابت Mat& a, ثابت Scalar& s)
{
    checkOperandsExist(a);
    Mat_ئىپادە e;
    MatOp_Bin::make_ئىپادە(e, '|', a, s);
    أرجع e;
}

Mat_ئىپادە operator | (ثابت Scalar& s, ثابت Mat& a)
{
    checkOperandsExist(a);
    Mat_ئىپادە e;
    MatOp_Bin::make_ئىپادە(e, '|', a, s);
    أرجع e;
}

Mat_ئىپادە operator ^ (ثابت Mat& a, ثابت Mat& b)
{
    checkOperandsExist(a, b);
    Mat_ئىپادە e;
    MatOp_Bin::make_ئىپادە(e, '^', a, b);
    أرجع e;
}

Mat_ئىپادە operator ^ (ثابت Mat& a, ثابت Scalar& s)
{
    checkOperandsExist(a);
    Mat_ئىپادە e;
    MatOp_Bin::make_ئىپادە(e, '^', a, s);
    أرجع e;
}

Mat_ئىپادە operator ^ (ثابت Scalar& s, ثابت Mat& a)
{
    checkOperandsExist(a);
    Mat_ئىپادە e;
    MatOp_Bin::make_ئىپادە(e, '^', a, s);
    أرجع e;
}

Mat_ئىپادە operator ~(ثابت Mat& a)
{
    checkOperandsExist(a);
    Mat_ئىپادە e;
    MatOp_Bin::make_ئىپادە(e, '~', a, Scalar());
    أرجع e;
}

Mat_ئىپادە abs(ثابت Mat& a)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    Mat_ئىپادە e;
    MatOp_Bin::make_ئىپادە(e, 'a', a, Scalar());
    أرجع e;
}

Mat_ئىپادە abs(ثابت Mat_ئىپادە& e)
{
    CV_INSTRUMENT_REGION();

    Mat_ئىپادە en;
    e.op->abs(e, en);
    أرجع en;
}


Size Mat_ئىپادە::size() ثابت
{
    إذا( isT(*هذا) || isInv(*هذا) )
        أرجع Size(a.rows, a.cols);
    إذا( isSolve(*هذا) )
        أرجع Size(b.cols, a.cols);
    إذا( isInitializer(*هذا) )
        أرجع a.size();
    أرجع op ? op->size(*هذا) : Size();
}


صحيح Mat_ئىپادە::type() ثابت
{
    CV_INSTRUMENT_REGION();

    إذا( isInitializer(*هذا) )
        أرجع a.type();
    إذا( isCmp(*هذا) )
        أرجع CV_MAKETYPE(CV_8U, a.channels());
    أرجع op ? op->type(*هذا) : -1;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////

فراغ MatOp_Identity::assign(ثابت Mat_ئىپادە& e, Mat& m, صحيح _type) ثابت
{
    إذا( _type == -1 || _type == e.a.type() )
        m = e.a;
    وإلا
    {
        CV_Assert( CV_MAT_CN(_type) == e.a.channels() );
        e.a.convertTo(m, _type);
    }
}

مضمن فراغ MatOp_Identity::make_ئىپادە(Mat_ئىپادە& res, ثابت Mat& m)
{
    res = Mat_ئىپادە(&g_MatOp_Identity, 0, m, Mat(), Mat(), 1, 0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

فراغ MatOp_AddEx::assign(ثابت Mat_ئىپادە& e, Mat& m, صحيح _type) ثابت
{
    Mat temp, &dst = _type == -1 || e.a.type() == _type ? m : temp;
    إذا( e.b.data )
    {
        إذا( e.s == Scalar() || !e.s.isReal() )
        {
            إذا( e.alpha == 1 )
            {
                إذا( e.beta == 1 )
                    cv::add(e.a, e.b, dst);
                وإلا إذا( e.beta == -1 )
                    cv::subtract(e.a, e.b, dst);
                وإلا
                    cv::scaleAdd(e.b, e.beta, e.a, dst);
            }
            وإلا إذا( e.beta == 1 )
            {
                إذا( e.alpha == -1 )
                    cv::subtract(e.b, e.a, dst);
                وإلا
                    cv::scaleAdd(e.a, e.alpha, e.b, dst);
            }
            وإلا
                cv::addWeighted(e.a, e.alpha, e.b, e.beta, 0, dst);

            إذا( !e.s.isReal() )
                cv::add(dst, e.s, dst);
        }
        وإلا
        {
            إذا (e.a.channels() > 1)
                CV_LOG_ONCE_WARNING(عدم, "OpenCV/MatExpr: processing of multi-channel arrays might be changed in the future: "
                                          "https://github.com/opencv/opencv/issues/16739");
            cv::addWeighted(e.a, e.alpha, e.b, e.beta, e.s[0], dst);
        }
    }
    وإلا إذا( e.s.isReal() && (dst.data != m.data || fabs(e.alpha) != 1))
    {
        إذا (e.a.channels() > 1 && e.s[0] != 0.0)
            CV_LOG_ONCE_WARNING(عدم, "OpenCV/MatExpr: processing of multi-channel arrays might be changed in the future: "
                                      "https://github.com/opencv/opencv/issues/16739");
        e.a.convertTo(m, _type, e.alpha, e.s[0]);
        أرجع;
    }
    وإلا إذا( e.alpha == 1 )
        cv::add(e.a, e.s, dst);
    وإلا إذا( e.alpha == -1 )
        cv::subtract(e.s, e.a, dst);
    وإلا
    {
        e.a.convertTo(dst, e.a.type(), e.alpha);
        cv::add(dst, e.s, dst);
    }

    إذا( dst.data != m.data )
        dst.convertTo(m, m.type());
}


فراغ MatOp_AddEx::add(ثابت Mat_ئىپادە& e, ثابت Scalar& s, Mat_ئىپادە& res) ثابت
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.s += s;
}


فراغ MatOp_AddEx::subtract(ثابت Scalar& s, ثابت Mat_ئىپادە& e, Mat_ئىپادە& res) ثابت
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha = -res.alpha;
    res.beta = -res.beta;
    res.s = s - res.s;
}

فراغ MatOp_AddEx::multiply(ثابت Mat_ئىپادە& e, مزدوج s, Mat_ئىپادە& res) ثابت
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha *= s;
    res.beta *= s;
    res.s *= s;
}

فراغ MatOp_AddEx::divide(مزدوج s, ثابت Mat_ئىپادە& e, Mat_ئىپادە& res) ثابت
{
    CV_INSTRUMENT_REGION();

    إذا( isScaled(e) )
        MatOp_Bin::make_ئىپادە(res, '/', e.a, Mat(), s/e.alpha);
    وإلا
        MatOp::divide(s, e, res);
}


فراغ MatOp_AddEx::transpose(ثابت Mat_ئىپادە& e, Mat_ئىپادە& res) ثابت
{
    CV_INSTRUMENT_REGION();

    إذا( isScaled(e) )
        MatOp_T::make_ئىپادە(res, e.a, e.alpha);
    وإلا
        MatOp::transpose(e, res);
}

فراغ MatOp_AddEx::abs(ثابت Mat_ئىپادە& e, Mat_ئىپادە& res) ثابت
{
    CV_INSTRUMENT_REGION();

    إذا( (!e.b.data || e.beta == 0) && fabs(e.alpha) == 1 )
        MatOp_Bin::make_ئىپادە(res, 'a', e.a, -e.s*e.alpha);
    وإلا إذا( e.b.data && e.alpha + e.beta == 0 && e.alpha*e.beta == -1 )
        MatOp_Bin::make_ئىپادە(res, 'a', e.a, e.b);
    وإلا
        MatOp::abs(e, res);
}

مضمن فراغ MatOp_AddEx::make_ئىپادە(Mat_ئىپادە& res, ثابت Mat& a, ثابت Mat& b, مزدوج alpha, مزدوج beta, ثابت Scalar& s)
{
    res = Mat_ئىپادە(&g_MatOp_AddEx, 0, a, b, Mat(), alpha, beta, s);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

فراغ MatOp_Bin::assign(ثابت Mat_ئىپادە& e, Mat& m, صحيح _type) ثابت
{
    Mat temp, &dst = _type == -1 || e.a.type() == _type ? m : temp;

    إذا( e.flags == '*' )
        cv::multiply(e.a, e.b, dst, e.alpha);
    وإلا إذا( e.flags == '/' && e.b.data )
        cv::divide(e.a, e.b, dst, e.alpha);
    وإلا إذا( e.flags == '/' && !e.b.data )
        cv::divide(e.alpha, e.a, dst );
    وإلا إذا( e.flags == '&' && e.b.data )
        bitwise_and(e.a, e.b, dst);
    وإلا إذا( e.flags == '&' && !e.b.data )
        bitwise_and(e.a, e.s, dst);
    وإلا إذا( e.flags == '|' && e.b.data )
        bitwise_or(e.a, e.b, dst);
    وإلا إذا( e.flags == '|' && !e.b.data )
        bitwise_or(e.a, e.s, dst);
    وإلا إذا( e.flags == '^' && e.b.data )
        bitwise_xor(e.a, e.b, dst);
    وإلا إذا( e.flags == '^' && !e.b.data )
        bitwise_xor(e.a, e.s, dst);
    وإلا إذا( e.flags == '~' && !e.b.data )
        bitwise_not(e.a, dst);
    وإلا إذا( e.flags == 'm' )
        cv::min(e.a, e.b, dst);
    وإلا إذا( e.flags == 'n' )
        cv::min(e.a, e.s[0], dst);
    وإلا إذا( e.flags == 'M' )
        cv::max(e.a, e.b, dst);
    وإلا إذا( e.flags == 'N' )
        cv::max(e.a, e.s[0], dst);
    وإلا إذا( e.flags == 'a' && e.b.data )
        cv::absdiff(e.a, e.b, dst);
    وإلا إذا( e.flags == 'a' && !e.b.data )
        cv::absdiff(e.a, e.s, dst);
    وإلا
        CV_Error(cv::Error::StsError, "Unknown operation");

    إذا( dst.data != m.data )
        dst.convertTo(m, _type);
}

فراغ MatOp_Bin::multiply(ثابت Mat_ئىپادە& e, مزدوج s, Mat_ئىپادە& res) ثابت
{
    CV_INSTRUMENT_REGION();

    إذا( e.flags == '*' || e.flags == '/' )
    {
        res = e;
        res.alpha *= s;
    }
    وإلا
        MatOp::multiply(e, s, res);
}

فراغ MatOp_Bin::divide(مزدوج s, ثابت Mat_ئىپادە& e, Mat_ئىپادە& res) ثابت
{
    CV_INSTRUMENT_REGION();

    إذا( e.flags == '/' && (!e.b.data || e.beta == 0) )
        MatOp_AddEx::make_ئىپادە(res, e.a, Mat(), s/e.alpha, 0);
    وإلا
        MatOp::divide(s, e, res);
}

مضمن فراغ MatOp_Bin::make_ئىپادە(Mat_ئىپادە& res, حرف op, ثابت Mat& a, ثابت Mat& b, مزدوج scale)
{
    res = Mat_ئىپادە(&g_MatOp_Bin, op, a, b, Mat(), scale, b.data ? 1 : 0);
}

مضمن فراغ MatOp_Bin::make_ئىپادە(Mat_ئىپادە& res, حرف op, ثابت Mat& a, ثابت Scalar& s)
{
    res = Mat_ئىپادە(&g_MatOp_Bin, op, a, Mat(), Mat(), 1, 0, s);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

فراغ MatOp_Cmp::assign(ثابت Mat_ئىپادە& e, Mat& m, صحيح _type) ثابت
{
    Mat temp, &dst = _type == -1 || _type == CV_8U ? m : temp;

    إذا( e.b.data )
        cv::compare(e.a, e.b, dst, e.flags);
    وإلا
        cv::compare(e.a, e.alpha, dst, e.flags);

    إذا( dst.data != m.data )
        dst.convertTo(m, _type);
}

مضمن فراغ MatOp_Cmp::make_ئىپادە(Mat_ئىپادە& res, صحيح cmpop, ثابت Mat& a, ثابت Mat& b)
{
    res = Mat_ئىپادە(&g_MatOp_Cmp, cmpop, a, b, Mat(), 1, 1);
}

مضمن فراغ MatOp_Cmp::make_ئىپادە(Mat_ئىپادە& res, صحيح cmpop, ثابت Mat& a, مزدوج alpha)
{
    res = Mat_ئىپادە(&g_MatOp_Cmp, cmpop, a, Mat(), Mat(), alpha, 1);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

فراغ MatOp_T::assign(ثابت Mat_ئىپادە& e, Mat& m, صحيح _type) ثابت
{
    Mat temp, &dst = _type == -1 || _type == e.a.type() ? m : temp;

    cv::transpose(e.a, dst);

    إذا( dst.data != m.data || e.alpha != 1 )
        dst.convertTo(m, _type, e.alpha);
}

فراغ MatOp_T::multiply(ثابت Mat_ئىپادە& e, مزدوج s, Mat_ئىپادە& res) ثابت
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha *= s;
}

فراغ MatOp_T::transpose(ثابت Mat_ئىپادە& e, Mat_ئىپادە& res) ثابت
{
    CV_INSTRUMENT_REGION();

    إذا( e.alpha == 1 )
        MatOp_Identity::make_ئىپادە(res, e.a);
    وإلا
        MatOp_AddEx::make_ئىپادە(res, e.a, Mat(), e.alpha, 0);
}

مضمن فراغ MatOp_T::make_ئىپادە(Mat_ئىپادە& res, ثابت Mat& a, مزدوج alpha)
{
    res = Mat_ئىپادە(&g_MatOp_T, 0, a, Mat(), Mat(), alpha, 0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

فراغ MatOp_GEMM::assign(ثابت Mat_ئىپادە& e, Mat& m, صحيح _type) ثابت
{
    Mat temp, &dst = _type == -1 || _type == e.a.type() ? m : temp;

    cv::gemm(e.a, e.b, e.alpha, e.c, e.beta, dst, e.flags);
    إذا( dst.data != m.data )
        dst.convertTo(m, _type);
}

فراغ MatOp_GEMM::add(ثابت Mat_ئىپادە& e1, ثابت Mat_ئىپادە& e2, Mat_ئىپادە& res) ثابت
{
    CV_INSTRUMENT_REGION();

    منطقي i1 = isIdentity(e1), i2 = isIdentity(e2);
    مزدوج alpha1 = i1 ? 1 : e1.alpha, alpha2 = i2 ? 1 : e2.alpha;

    إذا( isMatProd(e1) && (i2 || isScaled(e2) || isT(e2)) )
        MatOp_GEMM::make_ئىپادە(res, (e1.flags & ~cv::GEMM_3_T)|(isT(e2) ? cv::GEMM_3_T : 0),
                             e1.a, e1.b, alpha1, e2.a, alpha2);
    وإلا إذا( isMatProd(e2) && (i1 || isScaled(e1) || isT(e1)) )
        MatOp_GEMM::make_ئىپادە(res, (e2.flags & ~cv::GEMM_3_T)|(isT(e1) ? cv::GEMM_3_T : 0),
                             e2.a, e2.b, alpha2, e1.a, alpha1);
    وإلا إذا( هذا == e2.op )
        MatOp::add(e1, e2, res);
    وإلا
        e2.op->add(e1, e2, res);
}

فراغ MatOp_GEMM::subtract(ثابت Mat_ئىپادە& e1, ثابت Mat_ئىپادە& e2, Mat_ئىپادە& res) ثابت
{
    CV_INSTRUMENT_REGION();

    منطقي i1 = isIdentity(e1), i2 = isIdentity(e2);
    مزدوج alpha1 = i1 ? 1 : e1.alpha, alpha2 = i2 ? 1 : e2.alpha;

    إذا( isMatProd(e1) && (i2 || isScaled(e2) || isT(e2)) )
        MatOp_GEMM::make_ئىپادە(res, (e1.flags & ~cv::GEMM_3_T)|(isT(e2) ? cv::GEMM_3_T : 0),
                             e1.a, e1.b, alpha1, e2.a, -alpha2);
    وإلا إذا( isMatProd(e2) && (i1 || isScaled(e1) || isT(e1)) )
        MatOp_GEMM::make_ئىپادە(res, (e2.flags & ~cv::GEMM_3_T)|(isT(e1) ? cv::GEMM_3_T : 0),
                            e2.a, e2.b, -alpha2, e1.a, alpha1);
    وإلا إذا( هذا == e2.op )
        MatOp::subtract(e1, e2, res);
    وإلا
        e2.op->subtract(e1, e2, res);
}

فراغ MatOp_GEMM::multiply(ثابت Mat_ئىپادە& e, مزدوج s, Mat_ئىپادە& res) ثابت
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha *= s;
    res.beta *= s;
}

فراغ MatOp_GEMM::transpose(ثابت Mat_ئىپادە& e, Mat_ئىپادە& res) ثابت
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.flags = (!(e.flags & cv::GEMM_1_T) ? cv::GEMM_2_T : 0) |
                (!(e.flags & cv::GEMM_2_T) ? cv::GEMM_1_T : 0) |
                (!(e.flags & cv::GEMM_3_T) ? cv::GEMM_3_T : 0);
    swap(res.a, res.b);
}

مضمن فراغ MatOp_GEMM::make_ئىپادە(Mat_ئىپادە& res, صحيح flags, ثابت Mat& a, ثابت Mat& b,
                                 مزدوج alpha, ثابت Mat& c, مزدوج beta)
{
    res = Mat_ئىپادە(&g_MatOp_GEMM, flags, a, b, c, alpha, beta);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

فراغ MatOp_Invert::assign(ثابت Mat_ئىپادە& e, Mat& m, صحيح _type) ثابت
{
    Mat temp, &dst = _type == -1 || _type == e.a.type() ? m : temp;

    cv::invert(e.a, dst, e.flags);
    إذا( dst.data != m.data )
        dst.convertTo(m, _type);
}

فراغ MatOp_Invert::matmul(ثابت Mat_ئىپادە& e1, ثابت Mat_ئىپادە& e2, Mat_ئىپادە& res) ثابت
{
    إذا( isInv(e1) && isIdentity(e2) )
        MatOp_Solve::make_ئىپادە(res, e1.flags, e1.a, e2.a);
    وإلا إذا( هذا == e2.op )
        MatOp::matmul(e1, e2, res);
    وإلا
        e2.op->matmul(e1, e2, res);
}

مضمن فراغ MatOp_Invert::make_ئىپادە(Mat_ئىپادە& res, صحيح method, ثابت Mat& m)
{
    res = Mat_ئىپادە(&g_MatOp_Invert, method, m, Mat(), Mat(), 1, 0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

فراغ MatOp_Solve::assign(ثابت Mat_ئىپادە& e, Mat& m, صحيح _type) ثابت
{
    Mat temp, &dst = _type == -1 || _type == e.a.type() ? m : temp;

    cv::solve(e.a, e.b, dst, e.flags);
    إذا( dst.data != m.data )
        dst.convertTo(m, _type);
}

مضمن فراغ MatOp_Solve::make_ئىپادە(Mat_ئىپادە& res, صحيح method, ثابت Mat& a, ثابت Mat& b)
{
    res = Mat_ئىپادە(&g_MatOp_Solve, method, a, b, Mat(), 1, 1);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

فراغ MatOp_Initializer::assign(ثابت Mat_ئىپادە& e, Mat& m, صحيح _type) ثابت
{
    إذا( _type == -1 )
        _type = e.a.type();

    m.create(e.a.size, _type);

    إذا( e.flags == 'I' && e.a.dims <= 2 )
        setIdentity(m, Scalar(e.alpha));
    وإلا إذا( e.flags == '0' )
        m = Scalar();
    وإلا إذا( e.flags == '1' )
        m = Scalar(e.alpha);
    وإلا
        CV_Error(cv::Error::StsError, "Invalid matrix initializer type");
}

فراغ MatOp_Initializer::multiply(ثابت Mat_ئىپادە& e, مزدوج s, Mat_ئىپادە& res) ثابت
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha *= s;
}

مضمن فراغ MatOp_Initializer::make_ئىپادە(Mat_ئىپادە& res, صحيح method, Size sz, صحيح type, مزدوج alpha)
{
    res = Mat_ئىپادە(getGlobalMatOpInitializer(), method, Mat(sz, type, (فراغ*)(size_t)0xEEEEEEEE), Mat(), Mat(), alpha, 0);
}

مضمن فراغ MatOp_Initializer::make_ئىپادە(Mat_ئىپادە& res, صحيح method, صحيح ndims, ثابت صحيح* sizes, صحيح type, مزدوج alpha)
{
    res = Mat_ئىپادە(getGlobalMatOpInitializer(), method, Mat(ndims, sizes, type, (فراغ*)(size_t)0xEEEEEEEE), Mat(), Mat(), alpha, 0);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

Mat_ئىپادە Mat::t() ثابت
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(*هذا);
    Mat_ئىپادە e;
    MatOp_T::make_ئىپادە(e, *هذا);
    أرجع e;
}

Mat_ئىپادە Mat::inv(صحيح method) ثابت
{
    CV_INSTRUMENT_REGION();

    Mat_ئىپادە e;
    MatOp_Invert::make_ئىپادە(e, method, *هذا);
    أرجع e;
}


Mat_ئىپادە Mat::mul(كىرگۈزۈش_Array m, مزدوج scale) ثابت
{
    CV_INSTRUMENT_REGION();

    Mat b = m.getMat();
    // Unless the argument is a refcounted Mat/UMat, the header returned by getMat() may be
    // a non-owning view of caller memory (e.g. a scalar bound to _InputArray(const double&),
    // a Matx or a Vec) that the returned MatExpr can outlive, so snapshot it.
    // See https://github.com/opencv/opencv/issues/23577
    إذا( !m.isMat() && !m.isUMat() )
        b = b.clone();

    Mat_ئىپادە e;
    MatOp_Bin::make_ئىپادە(e, '*', *هذا, b, scale);
    أرجع e;
}

Mat_ئىپادە Mat::zeros(صحيح rows, صحيح cols, صحيح type)
{
    CV_INSTRUMENT_REGION();

    Mat_ئىپادە e;
    MatOp_Initializer::make_ئىپادە(e, '0', Size(cols, rows), type);
    أرجع e;
}

Mat_ئىپادە Mat::zeros(Size size, صحيح type)
{
    CV_INSTRUMENT_REGION();

    Mat_ئىپادە e;
    MatOp_Initializer::make_ئىپادە(e, '0', size, type);
    أرجع e;
}

Mat_ئىپادە Mat::zeros(صحيح ndims, ثابت صحيح* sizes, صحيح type)
{
    CV_INSTRUMENT_REGION();

    Mat_ئىپادە e;
    MatOp_Initializer::make_ئىپادە(e, '0', ndims, sizes, type);
    أرجع e;
}

Mat_ئىپادە Mat::zeros(ثابت MatShape& shape, صحيح type)
{
    CV_INSTRUMENT_REGION();

    Mat_ئىپادە e;
    MatOp_Initializer::make_ئىپادە(e, '0', shape.dims, shape.p, type);
    أرجع e;
}

Mat_ئىپادە Mat::ones(صحيح rows, صحيح cols, صحيح type)
{
    CV_INSTRUMENT_REGION();

    Mat_ئىپادە e;
    MatOp_Initializer::make_ئىپادە(e, '1', Size(cols, rows), type);
    أرجع e;
}

Mat_ئىپادە Mat::ones(Size size, صحيح type)
{
    CV_INSTRUMENT_REGION();

    Mat_ئىپادە e;
    MatOp_Initializer::make_ئىپادە(e, '1', size, type);
    أرجع e;
}

Mat_ئىپادە Mat::ones(صحيح ndims, ثابت صحيح* sizes, صحيح type)
{
    CV_INSTRUMENT_REGION();

    Mat_ئىپادە e;
    MatOp_Initializer::make_ئىپادە(e, '1', ndims, sizes, type);
    أرجع e;
}

Mat_ئىپادە Mat::ones(ثابت MatShape& shape, صحيح type)
{
    CV_INSTRUMENT_REGION();

    Mat_ئىپادە e;
    MatOp_Initializer::make_ئىپادە(e, '1', shape.dims, shape.p, type);
    أرجع e;
}

Mat_ئىپادە Mat::eye(صحيح rows, صحيح cols, صحيح type)
{
    CV_INSTRUMENT_REGION();

    Mat_ئىپادە e;
    MatOp_Initializer::make_ئىپادە(e, 'I', Size(cols, rows), type);
    أرجع e;
}

Mat_ئىپادە Mat::eye(Size size, صحيح type)
{
    CV_INSTRUMENT_REGION();

    Mat_ئىپادە e;
    MatOp_Initializer::make_ئىپادە(e, 'I', size, type);
    أرجع e;
}

فراغ Mat_ئىپادە::swap(Mat_ئىپادە& other)
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

كىرگۈزۈش_Array_2::كىرگۈزۈش_Array_2(ثابت Mat_ئىپادە& ئىپادە)
{
    إذا (!isIdentity(ئىپادە))
    {
        Mat result = ئىپادە;  // TODO improve through refcount == 1 of expr.a (inplace operation is possible - except gemm?)
        Mat_ئىپادە result_ئىپادە(result);
        swap(const_cast<Mat_ئىپادە&>(ئىپادە), result_ئىپادە);
    }
    CV_Assert(isIdentity(ئىپادە));
    init(FIXED_TYPE + FIXED_SIZE + MAT + ACCESS_READ, &ئىپادە.a);
}

} // cv::
