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
    אם (a.empty())
    {
        CV_Error(cv::Error::StsBadArg, "Matrix operand is an empty matrix.");
    }
}

ساكن فراغ checkOperandsExist(ثابت Mat& a, ثابت Mat& b)
{
    אם (a.empty() || b.empty())
    {
        CV_Error(cv::Error::StsBadArg, "One or more matrix operands are empty.");
    }
}


מחלקה MatOp_Identity CV_FINAL : عام MatOp
{
عام:
    MatOp_Identity() {}
    virtual ~MatOp_Identity() {}

    منطقي elementWise(ثابت MatExpr& /*expr*/) ثابت CV_OVERRIDE { החזר אמת; }
    فراغ assign(ثابت MatExpr& expr, Mat& m, صحيح type=-1) ثابت CV_OVERRIDE;

    ساكن فراغ makeExpr(MatExpr& res, ثابت Mat& m);
};

ساكن MatOp_Identity g_MatOp_Identity;

מחלקה MatOp_AddEx CV_FINAL : عام MatOp
{
عام:
    MatOp_AddEx() {}
    virtual ~MatOp_AddEx() {}

    منطقي elementWise(ثابت MatExpr& /*expr*/) ثابت CV_OVERRIDE { החזר אמת; }
    فراغ assign(ثابت MatExpr& expr, Mat& m, صحيح type=-1) ثابت CV_OVERRIDE;

    فراغ add(ثابت MatExpr& e1, ثابت Scalar& s, MatExpr& res) ثابت CV_OVERRIDE;
    فراغ subtract(ثابت Scalar& s, ثابت MatExpr& expr, MatExpr& res) ثابت CV_OVERRIDE;
    فراغ multiply(ثابت MatExpr& e1, مزدوج s, MatExpr& res) ثابت CV_OVERRIDE;
    فراغ divide(مزدوج s, ثابت MatExpr& e, MatExpr& res) ثابت CV_OVERRIDE;

    فراغ transpose(ثابت MatExpr& e1, MatExpr& res) ثابت CV_OVERRIDE;
    فراغ abs(ثابت MatExpr& expr, MatExpr& res) ثابت CV_OVERRIDE;

    ساكن فراغ makeExpr(MatExpr& res, ثابت Mat& a, ثابت Mat& b, مزدوج alpha, مزدوج beta, ثابت Scalar& s=Scalar());
};

ساكن MatOp_AddEx g_MatOp_AddEx;

מחלקה MatOp_Bin CV_FINAL : عام MatOp
{
عام:
    MatOp_Bin() {}
    virtual ~MatOp_Bin() {}

    منطقي elementWise(ثابت MatExpr& /*expr*/) ثابت CV_OVERRIDE { החזר אמת; }
    فراغ assign(ثابت MatExpr& expr, Mat& m, صحيح type=-1) ثابت CV_OVERRIDE;

    فراغ multiply(ثابت MatExpr& e1, مزدوج s, MatExpr& res) ثابت CV_OVERRIDE;
    فراغ divide(مزدوج s, ثابت MatExpr& e, MatExpr& res) ثابت CV_OVERRIDE;

    ساكن فراغ makeExpr(MatExpr& res, حرف op, ثابت Mat& a, ثابت Mat& b, مزدوج scale=1);
    ساكن فراغ makeExpr(MatExpr& res, حرف op, ثابت Mat& a, ثابت Scalar& s);
};

ساكن MatOp_Bin g_MatOp_Bin;

מחלקה MatOp_Cmp CV_FINAL : عام MatOp
{
عام:
    MatOp_Cmp() {}
    virtual ~MatOp_Cmp() {}

    منطقي elementWise(ثابت MatExpr& /*expr*/) ثابت CV_OVERRIDE { החזר אמת; }
    فراغ assign(ثابت MatExpr& expr, Mat& m, صحيح type=-1) ثابت CV_OVERRIDE;

    ساكن فراغ makeExpr(MatExpr& res, صحيح cmpop, ثابت Mat& a, ثابت Mat& b);
    ساكن فراغ makeExpr(MatExpr& res, صحيح cmpop, ثابت Mat& a, مزدوج alpha);
};

ساكن MatOp_Cmp g_MatOp_Cmp;

מחלקה MatOp_GEMM CV_FINAL : عام MatOp
{
عام:
    MatOp_GEMM() {}
    virtual ~MatOp_GEMM() {}

    منطقي elementWise(ثابت MatExpr& /*expr*/) ثابت CV_OVERRIDE { החזר שקר; }
    فراغ assign(ثابت MatExpr& expr, Mat& m, صحيح type=-1) ثابت CV_OVERRIDE;

    فراغ add(ثابت MatExpr& e1, ثابت MatExpr& e2, MatExpr& res) ثابت CV_OVERRIDE;
    فراغ subtract(ثابت MatExpr& e1, ثابت MatExpr& e2, MatExpr& res) ثابت CV_OVERRIDE;
    فراغ multiply(ثابت MatExpr& e, مزدوج s, MatExpr& res) ثابت CV_OVERRIDE;

    فراغ transpose(ثابت MatExpr& expr, MatExpr& res) ثابت CV_OVERRIDE;

    Size size(ثابت MatExpr& expr) ثابت CV_OVERRIDE
    {
        החזר Size(
            (expr.flags & GEMM_2_T) ? expr.b.rows : expr.b.cols,
            (expr.flags & GEMM_1_T) ? expr.a.cols : expr.a.rows
        );
    }

    ساكن فراغ makeExpr(MatExpr& res, صحيح flags, ثابت Mat& a, ثابت Mat& b,
                         مزدوج alpha=1, ثابت Mat& c=Mat(), مزدوج beta=1);

};

ساكن MatOp_GEMM g_MatOp_GEMM;

מחלקה MatOp_Invert CV_FINAL : عام MatOp
{
عام:
    MatOp_Invert() {}
    virtual ~MatOp_Invert() {}

    منطقي elementWise(ثابت MatExpr& /*expr*/) ثابت CV_OVERRIDE { החזר שקר; }
    فراغ assign(ثابت MatExpr& expr, Mat& m, صحيح type=-1) ثابت CV_OVERRIDE;

    فراغ matmul(ثابت MatExpr& expr1, ثابت MatExpr& expr2, MatExpr& res) ثابت CV_OVERRIDE;

    ساكن فراغ makeExpr(MatExpr& res, صحيح method, ثابت Mat& m);
};

ساكن MatOp_Invert g_MatOp_Invert;

מחלקה MatOp_T CV_FINAL : عام MatOp
{
عام:
    MatOp_T() {}
    virtual ~MatOp_T() {}

    منطقي elementWise(ثابت MatExpr& /*expr*/) ثابت CV_OVERRIDE { החזר שקר; }
    فراغ assign(ثابت MatExpr& expr, Mat& m, صحيح type=-1) ثابت CV_OVERRIDE;

    فراغ multiply(ثابت MatExpr& e1, مزدوج s, MatExpr& res) ثابت CV_OVERRIDE;
    فراغ transpose(ثابت MatExpr& expr, MatExpr& res) ثابت CV_OVERRIDE;

    ساكن فراغ makeExpr(MatExpr& res, ثابت Mat& a, مزدوج alpha=1);
};

ساكن MatOp_T g_MatOp_T;

מחלקה MatOp_Solve CV_FINAL : عام MatOp
{
عام:
    MatOp_Solve() {}
    virtual ~MatOp_Solve() {}

    منطقي elementWise(ثابت MatExpr& /*expr*/) ثابت CV_OVERRIDE { החזר שקר; }
    فراغ assign(ثابت MatExpr& expr, Mat& m, صحيح type=-1) ثابت CV_OVERRIDE;

    ساكن فراغ makeExpr(MatExpr& res, صحيح method, ثابت Mat& a, ثابت Mat& b);
};

ساكن MatOp_Solve g_MatOp_Solve;

מחלקה MatOp_Initializer CV_FINAL : عام MatOp
{
عام:
    MatOp_Initializer() {}
    virtual ~MatOp_Initializer() {}

    منطقي elementWise(ثابت MatExpr& /*expr*/) ثابت CV_OVERRIDE { החזר שקר; }
    فراغ assign(ثابت MatExpr& expr, Mat& m, صحيح type=-1) ثابت CV_OVERRIDE;

    فراغ multiply(ثابت MatExpr& e, مزدوج s, MatExpr& res) ثابت CV_OVERRIDE;

    ساكن فراغ makeExpr(MatExpr& res, صحيح method, Size sz, صحيح type, مزدوج alpha=1);
    ساكن فراغ makeExpr(MatExpr& res, صحيح method, صحيح ndims, ثابت صحيح* sizes, صحيح type, مزدوج alpha=1);
};

ساكن MatOp_Initializer* getGlobalMatOpInitializer()
{
    CV_SINGLETON_LAZY_INIT(MatOp_Initializer, جديد MatOp_Initializer())
}

ساكن مضمن منطقي isIdentity(ثابت MatExpr& e) { החזר e.op == &g_MatOp_Identity; }
ساكن مضمن منطقي isAddEx(ثابت MatExpr& e) { החזר e.op == &g_MatOp_AddEx; }
ساكن مضمن منطقي isScaled(ثابت MatExpr& e) { החזר isAddEx(e) && (!e.b.data || e.beta == 0) && e.s == Scalar(); }
ساكن مضمن منطقي isBin(ثابت MatExpr& e, حرف c) { החזר e.op == &g_MatOp_Bin && e.flags == c; }
ساكن مضمن منطقي isCmp(ثابت MatExpr& e) { החזר e.op == &g_MatOp_Cmp; }
ساكن مضمن منطقي isReciprocal(ثابت MatExpr& e) { החזר isBin(e,'/') && (!e.b.data || e.beta == 0); }
ساكن مضمن منطقي isT(ثابت MatExpr& e) { החזר e.op == &g_MatOp_T; }
ساكن مضمن منطقي isInv(ثابت MatExpr& e) { החזר e.op == &g_MatOp_Invert; }
ساكن مضمن منطقي isSolve(ثابت MatExpr& e) { החזר e.op == &g_MatOp_Solve; }
//static inline bool isGEMM(const MatExpr& e) { return e.op == &g_MatOp_GEMM; }
ساكن مضمن منطقي isMatProd(ثابت MatExpr& e) { החזר e.op == &g_MatOp_GEMM && (!e.c.data || e.beta == 0); }
ساكن مضمن منطقي isInitializer(ثابت MatExpr& e) { החזר e.op == getGlobalMatOpInitializer(); }

/////////////////////////////////////////////////////////////////////////////////////////////////////

MatOp::MatOp() {}
MatOp::~MatOp() {}


منطقي MatOp::elementWise(ثابت MatExpr& /*expr*/) ثابت
{
    החזר שקר;
}

فراغ MatOp::roi(ثابت MatExpr& expr, ثابت Range& rowRange, ثابت Range& colRange, MatExpr& e) ثابت
{
    אם( elementWise(expr) )
    {
        e = MatExpr(expr.op, expr.flags, Mat(), Mat(), Mat(),
                    expr.alpha, expr.beta, expr.s);
        אם(expr.a.data)
            e.a = expr.a(rowRange, colRange);
        אם(expr.b.data)
            e.b = expr.b(rowRange, colRange);
        אם(expr.c.data)
            e.c = expr.c(rowRange, colRange);
    }
    אחרת
    {
        Mat m;
        expr.op->assign(expr, m);
        e = MatExpr(&g_MatOp_Identity, 0, m(rowRange, colRange), Mat(), Mat());
    }
}

فراغ MatOp::diag(ثابت MatExpr& expr, صحيح d, MatExpr& e) ثابت
{
    אם( elementWise(expr) )
    {
        e = MatExpr(expr.op, expr.flags, Mat(), Mat(), Mat(),
                    expr.alpha, expr.beta, expr.s);
        אם(expr.a.data)
            e.a = expr.a.diag(d);
        אם(expr.b.data)
            e.b = expr.b.diag(d);
        אם(expr.c.data)
            e.c = expr.c.diag(d);
    }
    אחרת
    {
        Mat m;
        expr.op->assign(expr, m);
        e = MatExpr(&g_MatOp_Identity, 0, m.diag(d), Mat(), Mat());
    }
}


فراغ MatOp::augAssignAdd(ثابت MatExpr& expr, Mat& m) ثابت
{
    Mat temp;
    expr.op->assign(expr, temp);
    m += temp;
}


فراغ MatOp::augAssignSubtract(ثابت MatExpr& expr, Mat& m) ثابت
{
    Mat temp;
    expr.op->assign(expr, temp);
    m -= temp;
}


فراغ MatOp::augAssignMultiply(ثابت MatExpr& expr, Mat& m) ثابت
{
    Mat temp;
    expr.op->assign(expr, temp);
    m *= temp;
}


فراغ MatOp::augAssignDivide(ثابت MatExpr& expr, Mat& m) ثابت
{
    Mat temp;
    expr.op->assign(expr, temp);
    m /= temp;
}


فراغ MatOp::augAssignAnd(ثابت MatExpr& expr, Mat& m) ثابت
{
    Mat temp;
    expr.op->assign(expr, temp);
    m &= temp;
}


فراغ MatOp::augAssignOr(ثابت MatExpr& expr, Mat& m) ثابت
{
    Mat temp;
    expr.op->assign(expr, temp);
    m |= temp;
}


فراغ MatOp::augAssignXor(ثابت MatExpr& expr, Mat& m) ثابت
{
    Mat temp;
    expr.op->assign(expr, temp);
    m ^= temp;
}


فراغ MatOp::add(ثابت MatExpr& e1, ثابت MatExpr& e2, MatExpr& res) ثابت
{
    CV_INSTRUMENT_REGION();

    אם( هذا == e2.op )
    {
        مزدوج alpha = 1, beta = 1;
        Scalar s;
        Mat m1, m2;
        אם( isAddEx(e1) && (!e1.b.data || e1.beta == 0) )
        {
            m1 = e1.a;
            alpha = e1.alpha;
            s = e1.s;
        }
        אחרת
            e1.op->assign(e1, m1);

        אם( isAddEx(e2) && (!e2.b.data || e2.beta == 0) )
        {
            m2 = e2.a;
            beta = e2.alpha;
            s += e2.s;
        }
        אחרת
            e2.op->assign(e2, m2);
        MatOp_AddEx::makeExpr(res, m1, m2, alpha, beta, s);
    }
    אחרת
        e2.op->add(e1, e2, res);
}


فراغ MatOp::add(ثابت MatExpr& expr1, ثابت Scalar& s, MatExpr& res) ثابت
{
    CV_INSTRUMENT_REGION();

    Mat m1;
    expr1.op->assign(expr1, m1);
    MatOp_AddEx::makeExpr(res, m1, Mat(), 1, 0, s);
}


فراغ MatOp::subtract(ثابت MatExpr& e1, ثابت MatExpr& e2, MatExpr& res) ثابت
{
    CV_INSTRUMENT_REGION();

    אם( هذا == e2.op )
    {
        مزدوج alpha = 1, beta = -1;
        Scalar s;
        Mat m1, m2;
        אם( isAddEx(e1) && (!e1.b.data || e1.beta == 0) )
        {
            m1 = e1.a;
            alpha = e1.alpha;
            s = e1.s;
        }
        אחרת
            e1.op->assign(e1, m1);

        אם( isAddEx(e2) && (!e2.b.data || e2.beta == 0) )
        {
            m2 = e2.a;
            beta = -e2.alpha;
            s -= e2.s;
        }
        אחרת
            e2.op->assign(e2, m2);
        MatOp_AddEx::makeExpr(res, m1, m2, alpha, beta, s);
    }
    אחרת
        e2.op->subtract(e1, e2, res);
}


فراغ MatOp::subtract(ثابت Scalar& s, ثابت MatExpr& expr, MatExpr& res) ثابت
{
    CV_INSTRUMENT_REGION();

    Mat m;
    expr.op->assign(expr, m);
    MatOp_AddEx::makeExpr(res, m, Mat(), -1, 0, s);
}


فراغ MatOp::multiply(ثابت MatExpr& e1, ثابت MatExpr& e2, MatExpr& res, مزدوج scale) ثابت
{
    CV_INSTRUMENT_REGION();

    אם( هذا == e2.op )
    {
        Mat m1, m2;

        אם( isReciprocal(e1) )
        {
            אם( isScaled(e2) )
            {
                scale *= e2.alpha;
                m2 = e2.a;
            }
            אחרת
                e2.op->assign(e2, m2);

            MatOp_Bin::makeExpr(res, '/', m2, e1.a, scale/e1.alpha);
        }
        אחרת
        {
            حرف op = '*';
            אם( isScaled(e1) )
            {
                m1 = e1.a;
                scale *= e1.alpha;
            }
            אחרת
                e1.op->assign(e1, m1);

            אם( isScaled(e2) )
            {
                m2 = e2.a;
                scale *= e2.alpha;
            }
            אחרת אם( isReciprocal(e2) )
            {
                op = '/';
                m2 = e2.a;
                scale *= e2.alpha;
            }
            אחרת
                e2.op->assign(e2, m2);

            MatOp_Bin::makeExpr(res, op, m1, m2, scale);
        }
    }
    אחרת
        e2.op->multiply(e1, e2, res, scale);
}


فراغ MatOp::multiply(ثابت MatExpr& expr, مزدوج s, MatExpr& res) ثابت
{
    CV_INSTRUMENT_REGION();

    Mat m;
    expr.op->assign(expr, m);
    MatOp_AddEx::makeExpr(res, m, Mat(), s, 0);
}


فراغ MatOp::divide(ثابت MatExpr& e1, ثابت MatExpr& e2, MatExpr& res, مزدوج scale) ثابت
{
    CV_INSTRUMENT_REGION();

    אם( هذا == e2.op )
    {
        אם( isReciprocal(e1) && isReciprocal(e2) )
            MatOp_Bin::makeExpr(res, '/', e2.a, e1.a, e1.alpha/e2.alpha);
        אחרת
        {
            Mat m1, m2;
            حرف op = '/';

            אם( isScaled(e1) )
            {
                m1 = e1.a;
                scale *= e1.alpha;
            }
            אחרת
                e1.op->assign(e1, m1);

            אם( isScaled(e2) )
            {
                m2 = e2.a;
                scale /= e2.alpha;
            }
            אחרת אם( isReciprocal(e2) )
            {
                m2 = e2.a;
                scale /= e2.alpha;
                op = '*';
            }
            אחרת
                e2.op->assign(e2, m2);
            MatOp_Bin::makeExpr(res, op, m1, m2, scale);
        }
    }
    אחרת
        e2.op->divide(e1, e2, res, scale);
}


فراغ MatOp::divide(مزدوج s, ثابت MatExpr& expr, MatExpr& res) ثابت
{
    CV_INSTRUMENT_REGION();

    Mat m;
    expr.op->assign(expr, m);
    MatOp_Bin::makeExpr(res, '/', m, Mat(), s);
}


فراغ MatOp::abs(ثابت MatExpr& expr, MatExpr& res) ثابت
{
    CV_INSTRUMENT_REGION();

    Mat m;
    expr.op->assign(expr, m);
    MatOp_Bin::makeExpr(res, 'a', m, Mat());
}


فراغ MatOp::transpose(ثابت MatExpr& expr, MatExpr& res) ثابت
{
    CV_INSTRUMENT_REGION();

    Mat m;
    expr.op->assign(expr, m);
    MatOp_T::makeExpr(res, m, 1);
}


فراغ MatOp::matmul(ثابت MatExpr& e1, ثابت MatExpr& e2, MatExpr& res) ثابت
{
    אם( هذا == e2.op )
    {
        مزدوج scale = 1;
        صحيح flags = 0;
        Mat m1, m2;

        אם( isT(e1) )
        {
            flags = cv::GEMM_1_T;
            scale = e1.alpha;
            m1 = e1.a;
        }
        אחרת אם( isScaled(e1) )
        {
            scale = e1.alpha;
            m1 = e1.a;
        }
        אחרת
            e1.op->assign(e1, m1);

        אם( isT(e2) )
        {
            flags |= cv::GEMM_2_T;
            scale *= e2.alpha;
            m2 = e2.a;
        }
        אחרת אם( isScaled(e2) )
        {
            scale *= e2.alpha;
            m2 = e2.a;
        }
        אחרת
            e2.op->assign(e2, m2);

        MatOp_GEMM::makeExpr(res, flags, m1, m2, scale);
    }
    אחרת
        e2.op->matmul(e1, e2, res);
}


فراغ MatOp::invert(ثابت MatExpr& expr, صحيح method, MatExpr& res) ثابت
{
    Mat m;
    expr.op->assign(expr, m);
    MatOp_Invert::makeExpr(res, method, m);
}


Size MatOp::size(ثابت MatExpr& expr) ثابت
{
    החזר !expr.a.empty() ? expr.a.size() : expr.b.empty() ? expr.b.size() : expr.c.size();
}

صحيح MatOp::type(ثابت MatExpr& expr) ثابت
{
    CV_INSTRUMENT_REGION();

    החזר !expr.a.empty() ? expr.a.type() : expr.b.empty() ? expr.b.type() : expr.c.type();
}

//////////////////////////////////////////////////////////////////////////////////////////////////

MatExpr::MatExpr(ثابت Mat& m) : op(&g_MatOp_Identity), flags(0), a(m), b(Mat()), c(Mat()), alpha(1), beta(0), s(Scalar())
{
}

MatExpr MatExpr::row(صحيح y) ثابت
{
    MatExpr e;
    op->roi(*هذا, Range(y, y+1), Range::all(), e);
    החזר e;
}

MatExpr MatExpr::col(صحيح x) ثابت
{
    MatExpr e;
    op->roi(*هذا, Range::all(), Range(x, x+1), e);
    החזר e;
}

MatExpr MatExpr::diag(صحيح d) ثابت
{
    MatExpr e;
    op->diag(*هذا, d, e);
    החזר e;
}

MatExpr MatExpr::operator()( ثابت Range& rowRange, ثابت Range& colRange ) ثابت
{
    MatExpr e;
    op->roi(*هذا, rowRange, colRange, e);
    החזר e;
}

MatExpr MatExpr::operator()( ثابت Rect& roi ) ثابت
{
    MatExpr e;
    op->roi(*هذا, Range(roi.y, roi.y + roi.height), Range(roi.x, roi.x + roi.width), e);
    החזר e;
}

Mat MatExpr::cross(ثابت Mat& m) ثابت
{
    החזר ((Mat)*هذا).cross(m);
}

مزدوج MatExpr::dot(ثابت Mat& m) ثابت
{
    החזר ((Mat)*هذا).dot(m);
}

MatExpr MatExpr::t() ثابت
{
    MatExpr e;
    op->transpose(*هذا, e);
    החזר e;
}

MatExpr MatExpr::inv(صحيح method) ثابت
{
    MatExpr e;
    op->invert(*هذا, method, e);
    החזר e;
}

MatExpr MatExpr::mul(ثابت MatExpr& e, مزدوج scale) ثابت
{
    MatExpr en;
    op->multiply(*هذا, e, en, scale);
    החזר en;
}

MatExpr MatExpr::mul(ثابت Mat& m, مزدوج scale) ثابت
{
    MatExpr e;
    op->multiply(*هذا, MatExpr(m), e, scale);
    החזר e;
}

MatExpr operator + (ثابت Mat& a, ثابت Mat& b)
{
    checkOperandsExist(a, b);
    MatExpr e;
    MatOp_AddEx::makeExpr(e, a, b, 1, 1);
    החזר e;
}

MatExpr operator + (ثابت Mat& a, ثابت Scalar& s)
{
    checkOperandsExist(a);
    MatExpr e;
    MatOp_AddEx::makeExpr(e, a, Mat(), 1, 0, s);
    החזר e;
}

MatExpr operator + (ثابت Scalar& s, ثابت Mat& a)
{
    checkOperandsExist(a);
    MatExpr e;
    MatOp_AddEx::makeExpr(e, a, Mat(), 1, 0, s);
    החזר e;
}

MatExpr operator + (ثابت MatExpr& e, ثابت Mat& m)
{
    MatExpr en;
    e.op->add(e, MatExpr(m), en);
    החזר en;
}

MatExpr operator + (ثابت Mat& m, ثابت MatExpr& e)
{
    checkOperandsExist(m);
    MatExpr en;
    e.op->add(e, MatExpr(m), en);
    החזר en;
}

MatExpr operator + (ثابت MatExpr& e, ثابت Scalar& s)
{
    MatExpr en;
    e.op->add(e, s, en);
    החזר en;
}

MatExpr operator + (ثابت Scalar& s, ثابت MatExpr& e)
{
    MatExpr en;
    e.op->add(e, s, en);
    החזר en;
}

MatExpr operator + (ثابت MatExpr& e1, ثابت MatExpr& e2)
{
    MatExpr en;
    e1.op->add(e1, e2, en);
    החזר en;
}

MatExpr operator - (ثابت Mat& a, ثابت Mat& b)
{
    checkOperandsExist(a, b);
    MatExpr e;
    MatOp_AddEx::makeExpr(e, a, b, 1, -1);
    החזר e;
}

MatExpr operator - (ثابت Mat& a, ثابت Scalar& s)
{
    checkOperandsExist(a);
    MatExpr e;
    MatOp_AddEx::makeExpr(e, a, Mat(), 1, 0, -s);
    החזר e;
}

MatExpr operator - (ثابت Scalar& s, ثابت Mat& a)
{
    checkOperandsExist(a);
    MatExpr e;
    MatOp_AddEx::makeExpr(e, a, Mat(), -1, 0, s);
    החזר e;
}

MatExpr operator - (ثابت MatExpr& e, ثابت Mat& m)
{
    checkOperandsExist(m);
    MatExpr en;
    e.op->subtract(e, MatExpr(m), en);
    החזר en;
}

MatExpr operator - (ثابت Mat& m, ثابت MatExpr& e)
{
    checkOperandsExist(m);
    MatExpr en;
    e.op->subtract(MatExpr(m), e, en);
    החזר en;
}

MatExpr operator - (ثابت MatExpr& e, ثابت Scalar& s)
{
    MatExpr en;
    e.op->add(e, -s, en);
    החזר en;
}

MatExpr operator - (ثابت Scalar& s, ثابت MatExpr& e)
{
    MatExpr en;
    e.op->subtract(s, e, en);
    החזר en;
}

MatExpr operator - (ثابت MatExpr& e1, ثابت MatExpr& e2)
{
    MatExpr en;
    e1.op->subtract(e1, e2, en);
    החזר en;
}

MatExpr operator - (ثابت Mat& m)
{
    checkOperandsExist(m);
    MatExpr e;
    MatOp_AddEx::makeExpr(e, m, Mat(), -1, 0);
    החזר e;
}

MatExpr operator - (ثابت MatExpr& e)
{
    MatExpr en;
    e.op->subtract(Scalar(0), e, en);
    החזר en;
}

MatExpr operator * (ثابت Mat& a, ثابت Mat& b)
{
    checkOperandsExist(a, b);
    MatExpr e;
    MatOp_GEMM::makeExpr(e, 0, a, b);
    החזר e;
}

MatExpr operator * (ثابت Mat& a, مزدوج s)
{
    checkOperandsExist(a);
    MatExpr e;
    MatOp_AddEx::makeExpr(e, a, Mat(), s, 0);
    החזר e;
}

MatExpr operator * (مزدوج s, ثابت Mat& a)
{
    checkOperandsExist(a);
    MatExpr e;
    MatOp_AddEx::makeExpr(e, a, Mat(), s, 0);
    החזר e;
}

MatExpr operator * (ثابت MatExpr& e, ثابت Mat& m)
{
    MatExpr en;
    e.op->matmul(e, MatExpr(m), en);
    החזר en;
}

MatExpr operator * (ثابت Mat& m, ثابت MatExpr& e)
{
    checkOperandsExist(m);
    MatExpr en;
    e.op->matmul(MatExpr(m), e, en);
    החזר en;
}

MatExpr operator * (ثابت MatExpr& e, مزدوج s)
{
    MatExpr en;
    e.op->multiply(e, s, en);
    החזר en;
}

MatExpr operator * (مزدوج s, ثابت MatExpr& e)
{
    MatExpr en;
    e.op->multiply(e, s, en);
    החזר en;
}

MatExpr operator * (ثابت MatExpr& e1, ثابت MatExpr& e2)
{
    MatExpr en;
    e1.op->matmul(e1, e2, en);
    החזר en;
}

MatExpr operator / (ثابت Mat& a, ثابت Mat& b)
{
    checkOperandsExist(a, b);
    MatExpr e;
    MatOp_Bin::makeExpr(e, '/', a, b);
    החזר e;
}

MatExpr operator / (ثابت Mat& a, مزدوج s)
{
    checkOperandsExist(a);
    MatExpr e;
    MatOp_AddEx::makeExpr(e, a, Mat(), 1./s, 0);
    החזר e;
}

MatExpr operator / (مزدوج s, ثابت Mat& a)
{
    checkOperandsExist(a);
    MatExpr e;
    MatOp_Bin::makeExpr(e, '/', a, Mat(), s);
    החזר e;
}

MatExpr operator / (ثابت MatExpr& e, ثابت Mat& m)
{
    MatExpr en;
    e.op->divide(e, MatExpr(m), en);
    החזר en;
}

MatExpr operator / (ثابت Mat& m, ثابت MatExpr& e)
{
    checkOperandsExist(m);
    MatExpr en;
    e.op->divide(MatExpr(m), e, en);
    החזר en;
}

MatExpr operator / (ثابت MatExpr& e, مزدوج s)
{
    MatExpr en;
    e.op->multiply(e, 1./s, en);
    החזר en;
}

MatExpr operator / (مزدوج s, ثابت MatExpr& e)
{
    MatExpr en;
    e.op->divide(s, e, en);
    החזר en;
}

MatExpr operator / (ثابت MatExpr& e1, ثابت MatExpr& e2)
{
    MatExpr en;
    e1.op->divide(e1, e2, en);
    החזר en;
}

MatExpr operator < (ثابت Mat& a, ثابت Mat& b)
{
    checkOperandsExist(a, b);
    MatExpr e;
    MatOp_Cmp::makeExpr(e, cv::CMP_LT, a, b);
    החזר e;
}

MatExpr operator < (ثابت Mat& a, مزدوج s)
{
    checkOperandsExist(a);
    MatExpr e;
    MatOp_Cmp::makeExpr(e, cv::CMP_LT, a, s);
    החזר e;
}

MatExpr operator < (مزدوج s, ثابت Mat& a)
{
    checkOperandsExist(a);
    MatExpr e;
    MatOp_Cmp::makeExpr(e, cv::CMP_GT, a, s);
    החזר e;
}

MatExpr operator <= (ثابت Mat& a, ثابت Mat& b)
{
    checkOperandsExist(a, b);
    MatExpr e;
    MatOp_Cmp::makeExpr(e, cv::CMP_LE, a, b);
    החזר e;
}

MatExpr operator <= (ثابت Mat& a, مزدوج s)
{
    checkOperandsExist(a);
    MatExpr e;
    MatOp_Cmp::makeExpr(e, cv::CMP_LE, a, s);
    החזר e;
}

MatExpr operator <= (مزدوج s, ثابت Mat& a)
{
    checkOperandsExist(a);
    MatExpr e;
    MatOp_Cmp::makeExpr(e, cv::CMP_GE, a, s);
    החזר e;
}

MatExpr operator == (ثابت Mat& a, ثابت Mat& b)
{
    checkOperandsExist(a, b);
    MatExpr e;
    MatOp_Cmp::makeExpr(e, cv::CMP_EQ, a, b);
    החזר e;
}

MatExpr operator == (ثابت Mat& a, مزدوج s)
{
    checkOperandsExist(a);
    MatExpr e;
    MatOp_Cmp::makeExpr(e, cv::CMP_EQ, a, s);
    החזר e;
}

MatExpr operator == (مزدوج s, ثابت Mat& a)
{
    checkOperandsExist(a);
    MatExpr e;
    MatOp_Cmp::makeExpr(e, cv::CMP_EQ, a, s);
    החזר e;
}

MatExpr operator != (ثابت Mat& a, ثابت Mat& b)
{
    checkOperandsExist(a, b);
    MatExpr e;
    MatOp_Cmp::makeExpr(e, cv::CMP_NE, a, b);
    החזר e;
}

MatExpr operator != (ثابت Mat& a, مزدوج s)
{
    checkOperandsExist(a);
    MatExpr e;
    MatOp_Cmp::makeExpr(e, cv::CMP_NE, a, s);
    החזר e;
}

MatExpr operator != (مزدوج s, ثابت Mat& a)
{
    checkOperandsExist(a);
    MatExpr e;
    MatOp_Cmp::makeExpr(e, cv::CMP_NE, a, s);
    החזר e;
}

MatExpr operator >= (ثابت Mat& a, ثابت Mat& b)
{
    checkOperandsExist(a, b);
    MatExpr e;
    MatOp_Cmp::makeExpr(e, cv::CMP_GE, a, b);
    החזר e;
}

MatExpr operator >= (ثابت Mat& a, مزدوج s)
{
    checkOperandsExist(a);
    MatExpr e;
    MatOp_Cmp::makeExpr(e, cv::CMP_GE, a, s);
    החזר e;
}

MatExpr operator >= (مزدوج s, ثابت Mat& a)
{
    checkOperandsExist(a);
    MatExpr e;
    MatOp_Cmp::makeExpr(e, cv::CMP_LE, a, s);
    החזר e;
}

MatExpr operator > (ثابت Mat& a, ثابت Mat& b)
{
    checkOperandsExist(a, b);
    MatExpr e;
    MatOp_Cmp::makeExpr(e, cv::CMP_GT, a, b);
    החזר e;
}

MatExpr operator > (ثابت Mat& a, مزدوج s)
{
    checkOperandsExist(a);
    MatExpr e;
    MatOp_Cmp::makeExpr(e, cv::CMP_GT, a, s);
    החזר e;
}

MatExpr operator > (مزدوج s, ثابت Mat& a)
{
    checkOperandsExist(a);
    MatExpr e;
    MatOp_Cmp::makeExpr(e, cv::CMP_LT, a, s);
    החזר e;
}

MatExpr min(ثابت Mat& a, ثابت Mat& b)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a, b);
    MatExpr e;
    MatOp_Bin::makeExpr(e, 'm', a, b);
    החזר e;
}

MatExpr min(ثابت Mat& a, مزدوج s)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    MatExpr e;
    MatOp_Bin::makeExpr(e, 'n', a, s);
    החזר e;
}

MatExpr min(مزدوج s, ثابت Mat& a)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    MatExpr e;
    MatOp_Bin::makeExpr(e, 'n', a, s);
    החזר e;
}

MatExpr max(ثابت Mat& a, ثابت Mat& b)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a, b);
    MatExpr e;
    MatOp_Bin::makeExpr(e, 'M', a, b);
    החזר e;
}

MatExpr max(ثابت Mat& a, مزدوج s)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    MatExpr e;
    MatOp_Bin::makeExpr(e, 'N', a, s);
    החזר e;
}

MatExpr max(مزدوج s, ثابت Mat& a)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    MatExpr e;
    MatOp_Bin::makeExpr(e, 'N', a, s);
    החזר e;
}

MatExpr operator & (ثابت Mat& a, ثابت Mat& b)
{
    checkOperandsExist(a, b);
    MatExpr e;
    MatOp_Bin::makeExpr(e, '&', a, b);
    החזר e;
}

MatExpr operator & (ثابت Mat& a, ثابت Scalar& s)
{
    checkOperandsExist(a);
    MatExpr e;
    MatOp_Bin::makeExpr(e, '&', a, s);
    החזר e;
}

MatExpr operator & (ثابت Scalar& s, ثابت Mat& a)
{
    checkOperandsExist(a);
    MatExpr e;
    MatOp_Bin::makeExpr(e, '&', a, s);
    החזר e;
}

MatExpr operator | (ثابت Mat& a, ثابت Mat& b)
{
    checkOperandsExist(a, b);
    MatExpr e;
    MatOp_Bin::makeExpr(e, '|', a, b);
    החזר e;
}

MatExpr operator | (ثابت Mat& a, ثابت Scalar& s)
{
    checkOperandsExist(a);
    MatExpr e;
    MatOp_Bin::makeExpr(e, '|', a, s);
    החזר e;
}

MatExpr operator | (ثابت Scalar& s, ثابت Mat& a)
{
    checkOperandsExist(a);
    MatExpr e;
    MatOp_Bin::makeExpr(e, '|', a, s);
    החזר e;
}

MatExpr operator ^ (ثابت Mat& a, ثابت Mat& b)
{
    checkOperandsExist(a, b);
    MatExpr e;
    MatOp_Bin::makeExpr(e, '^', a, b);
    החזר e;
}

MatExpr operator ^ (ثابت Mat& a, ثابت Scalar& s)
{
    checkOperandsExist(a);
    MatExpr e;
    MatOp_Bin::makeExpr(e, '^', a, s);
    החזר e;
}

MatExpr operator ^ (ثابت Scalar& s, ثابت Mat& a)
{
    checkOperandsExist(a);
    MatExpr e;
    MatOp_Bin::makeExpr(e, '^', a, s);
    החזר e;
}

MatExpr operator ~(ثابت Mat& a)
{
    checkOperandsExist(a);
    MatExpr e;
    MatOp_Bin::makeExpr(e, '~', a, Scalar());
    החזר e;
}

MatExpr abs(ثابت Mat& a)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    MatExpr e;
    MatOp_Bin::makeExpr(e, 'a', a, Scalar());
    החזר e;
}

MatExpr abs(ثابت MatExpr& e)
{
    CV_INSTRUMENT_REGION();

    MatExpr en;
    e.op->abs(e, en);
    החזר en;
}


Size MatExpr::size() ثابت
{
    אם( isT(*هذا) || isInv(*هذا) )
        החזר Size(a.rows, a.cols);
    אם( isSolve(*هذا) )
        החזר Size(b.cols, a.cols);
    אם( isInitializer(*هذا) )
        החזר a.size();
    החזר op ? op->size(*هذا) : Size();
}


صحيح MatExpr::type() ثابت
{
    CV_INSTRUMENT_REGION();

    אם( isInitializer(*هذا) )
        החזר a.type();
    אם( isCmp(*هذا) )
        החזר CV_MAKETYPE(CV_8U, a.channels());
    החזר op ? op->type(*هذا) : -1;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////

فراغ MatOp_Identity::assign(ثابت MatExpr& e, Mat& m, صحيح _type) ثابت
{
    אם( _type == -1 || _type == e.a.type() )
        m = e.a;
    אחרת
    {
        CV_Assert( CV_MAT_CN(_type) == e.a.channels() );
        e.a.convertTo(m, _type);
    }
}

مضمن فراغ MatOp_Identity::makeExpr(MatExpr& res, ثابت Mat& m)
{
    res = MatExpr(&g_MatOp_Identity, 0, m, Mat(), Mat(), 1, 0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

فراغ MatOp_AddEx::assign(ثابت MatExpr& e, Mat& m, صحيح _type) ثابت
{
    Mat temp, &dst = _type == -1 || e.a.type() == _type ? m : temp;
    אם( e.b.data )
    {
        אם( e.s == Scalar() || !e.s.isReal() )
        {
            אם( e.alpha == 1 )
            {
                אם( e.beta == 1 )
                    cv::add(e.a, e.b, dst);
                אחרת אם( e.beta == -1 )
                    cv::subtract(e.a, e.b, dst);
                אחרת
                    cv::scaleAdd(e.b, e.beta, e.a, dst);
            }
            אחרת אם( e.beta == 1 )
            {
                אם( e.alpha == -1 )
                    cv::subtract(e.b, e.a, dst);
                אחרת
                    cv::scaleAdd(e.a, e.alpha, e.b, dst);
            }
            אחרת
                cv::addWeighted(e.a, e.alpha, e.b, e.beta, 0, dst);

            אם( !e.s.isReal() )
                cv::add(dst, e.s, dst);
        }
        אחרת
        {
            אם (e.a.channels() > 1)
                CV_LOG_ONCE_WARNING(عدم, "OpenCV/MatExpr: processing of multi-channel arrays might be changed in the future: "
                                          "https://github.com/opencv/opencv/issues/16739");
            cv::addWeighted(e.a, e.alpha, e.b, e.beta, e.s[0], dst);
        }
    }
    אחרת אם( e.s.isReal() && (dst.data != m.data || fabs(e.alpha) != 1))
    {
        אם (e.a.channels() > 1 && e.s[0] != 0.0)
            CV_LOG_ONCE_WARNING(عدم, "OpenCV/MatExpr: processing of multi-channel arrays might be changed in the future: "
                                      "https://github.com/opencv/opencv/issues/16739");
        e.a.convertTo(m, _type, e.alpha, e.s[0]);
        החזר;
    }
    אחרת אם( e.alpha == 1 )
        cv::add(e.a, e.s, dst);
    אחרת אם( e.alpha == -1 )
        cv::subtract(e.s, e.a, dst);
    אחרת
    {
        e.a.convertTo(dst, e.a.type(), e.alpha);
        cv::add(dst, e.s, dst);
    }

    אם( dst.data != m.data )
        dst.convertTo(m, m.type());
}


فراغ MatOp_AddEx::add(ثابت MatExpr& e, ثابت Scalar& s, MatExpr& res) ثابت
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.s += s;
}


فراغ MatOp_AddEx::subtract(ثابت Scalar& s, ثابت MatExpr& e, MatExpr& res) ثابت
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha = -res.alpha;
    res.beta = -res.beta;
    res.s = s - res.s;
}

فراغ MatOp_AddEx::multiply(ثابت MatExpr& e, مزدوج s, MatExpr& res) ثابت
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha *= s;
    res.beta *= s;
    res.s *= s;
}

فراغ MatOp_AddEx::divide(مزدوج s, ثابت MatExpr& e, MatExpr& res) ثابت
{
    CV_INSTRUMENT_REGION();

    אם( isScaled(e) )
        MatOp_Bin::makeExpr(res, '/', e.a, Mat(), s/e.alpha);
    אחרת
        MatOp::divide(s, e, res);
}


فراغ MatOp_AddEx::transpose(ثابت MatExpr& e, MatExpr& res) ثابت
{
    CV_INSTRUMENT_REGION();

    אם( isScaled(e) )
        MatOp_T::makeExpr(res, e.a, e.alpha);
    אחרת
        MatOp::transpose(e, res);
}

فراغ MatOp_AddEx::abs(ثابت MatExpr& e, MatExpr& res) ثابت
{
    CV_INSTRUMENT_REGION();

    אם( (!e.b.data || e.beta == 0) && fabs(e.alpha) == 1 )
        MatOp_Bin::makeExpr(res, 'a', e.a, -e.s*e.alpha);
    אחרת אם( e.b.data && e.alpha + e.beta == 0 && e.alpha*e.beta == -1 )
        MatOp_Bin::makeExpr(res, 'a', e.a, e.b);
    אחרת
        MatOp::abs(e, res);
}

مضمن فراغ MatOp_AddEx::makeExpr(MatExpr& res, ثابت Mat& a, ثابت Mat& b, مزدوج alpha, مزدوج beta, ثابت Scalar& s)
{
    res = MatExpr(&g_MatOp_AddEx, 0, a, b, Mat(), alpha, beta, s);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

فراغ MatOp_Bin::assign(ثابت MatExpr& e, Mat& m, صحيح _type) ثابت
{
    Mat temp, &dst = _type == -1 || e.a.type() == _type ? m : temp;

    אם( e.flags == '*' )
        cv::multiply(e.a, e.b, dst, e.alpha);
    אחרת אם( e.flags == '/' && e.b.data )
        cv::divide(e.a, e.b, dst, e.alpha);
    אחרת אם( e.flags == '/' && !e.b.data )
        cv::divide(e.alpha, e.a, dst );
    אחרת אם( e.flags == '&' && e.b.data )
        bitwise_and(e.a, e.b, dst);
    אחרת אם( e.flags == '&' && !e.b.data )
        bitwise_and(e.a, e.s, dst);
    אחרת אם( e.flags == '|' && e.b.data )
        bitwise_or(e.a, e.b, dst);
    אחרת אם( e.flags == '|' && !e.b.data )
        bitwise_or(e.a, e.s, dst);
    אחרת אם( e.flags == '^' && e.b.data )
        bitwise_xor(e.a, e.b, dst);
    אחרת אם( e.flags == '^' && !e.b.data )
        bitwise_xor(e.a, e.s, dst);
    אחרת אם( e.flags == '~' && !e.b.data )
        bitwise_not(e.a, dst);
    אחרת אם( e.flags == 'm' )
        cv::min(e.a, e.b, dst);
    אחרת אם( e.flags == 'n' )
        cv::min(e.a, e.s[0], dst);
    אחרת אם( e.flags == 'M' )
        cv::max(e.a, e.b, dst);
    אחרת אם( e.flags == 'N' )
        cv::max(e.a, e.s[0], dst);
    אחרת אם( e.flags == 'a' && e.b.data )
        cv::absdiff(e.a, e.b, dst);
    אחרת אם( e.flags == 'a' && !e.b.data )
        cv::absdiff(e.a, e.s, dst);
    אחרת
        CV_Error(cv::Error::StsError, "Unknown operation");

    אם( dst.data != m.data )
        dst.convertTo(m, _type);
}

فراغ MatOp_Bin::multiply(ثابت MatExpr& e, مزدوج s, MatExpr& res) ثابت
{
    CV_INSTRUMENT_REGION();

    אם( e.flags == '*' || e.flags == '/' )
    {
        res = e;
        res.alpha *= s;
    }
    אחרת
        MatOp::multiply(e, s, res);
}

فراغ MatOp_Bin::divide(مزدوج s, ثابت MatExpr& e, MatExpr& res) ثابت
{
    CV_INSTRUMENT_REGION();

    אם( e.flags == '/' && (!e.b.data || e.beta == 0) )
        MatOp_AddEx::makeExpr(res, e.a, Mat(), s/e.alpha, 0);
    אחרת
        MatOp::divide(s, e, res);
}

مضمن فراغ MatOp_Bin::makeExpr(MatExpr& res, حرف op, ثابت Mat& a, ثابت Mat& b, مزدوج scale)
{
    res = MatExpr(&g_MatOp_Bin, op, a, b, Mat(), scale, b.data ? 1 : 0);
}

مضمن فراغ MatOp_Bin::makeExpr(MatExpr& res, حرف op, ثابت Mat& a, ثابت Scalar& s)
{
    res = MatExpr(&g_MatOp_Bin, op, a, Mat(), Mat(), 1, 0, s);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

فراغ MatOp_Cmp::assign(ثابت MatExpr& e, Mat& m, صحيح _type) ثابت
{
    Mat temp, &dst = _type == -1 || _type == CV_8U ? m : temp;

    אם( e.b.data )
        cv::compare(e.a, e.b, dst, e.flags);
    אחרת
        cv::compare(e.a, e.alpha, dst, e.flags);

    אם( dst.data != m.data )
        dst.convertTo(m, _type);
}

مضمن فراغ MatOp_Cmp::makeExpr(MatExpr& res, صحيح cmpop, ثابت Mat& a, ثابت Mat& b)
{
    res = MatExpr(&g_MatOp_Cmp, cmpop, a, b, Mat(), 1, 1);
}

مضمن فراغ MatOp_Cmp::makeExpr(MatExpr& res, صحيح cmpop, ثابت Mat& a, مزدوج alpha)
{
    res = MatExpr(&g_MatOp_Cmp, cmpop, a, Mat(), Mat(), alpha, 1);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

فراغ MatOp_T::assign(ثابت MatExpr& e, Mat& m, صحيح _type) ثابت
{
    Mat temp, &dst = _type == -1 || _type == e.a.type() ? m : temp;

    cv::transpose(e.a, dst);

    אם( dst.data != m.data || e.alpha != 1 )
        dst.convertTo(m, _type, e.alpha);
}

فراغ MatOp_T::multiply(ثابت MatExpr& e, مزدوج s, MatExpr& res) ثابت
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha *= s;
}

فراغ MatOp_T::transpose(ثابت MatExpr& e, MatExpr& res) ثابت
{
    CV_INSTRUMENT_REGION();

    אם( e.alpha == 1 )
        MatOp_Identity::makeExpr(res, e.a);
    אחרת
        MatOp_AddEx::makeExpr(res, e.a, Mat(), e.alpha, 0);
}

مضمن فراغ MatOp_T::makeExpr(MatExpr& res, ثابت Mat& a, مزدوج alpha)
{
    res = MatExpr(&g_MatOp_T, 0, a, Mat(), Mat(), alpha, 0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

فراغ MatOp_GEMM::assign(ثابت MatExpr& e, Mat& m, صحيح _type) ثابت
{
    Mat temp, &dst = _type == -1 || _type == e.a.type() ? m : temp;

    cv::gemm(e.a, e.b, e.alpha, e.c, e.beta, dst, e.flags);
    אם( dst.data != m.data )
        dst.convertTo(m, _type);
}

فراغ MatOp_GEMM::add(ثابت MatExpr& e1, ثابت MatExpr& e2, MatExpr& res) ثابت
{
    CV_INSTRUMENT_REGION();

    منطقي i1 = isIdentity(e1), i2 = isIdentity(e2);
    مزدوج alpha1 = i1 ? 1 : e1.alpha, alpha2 = i2 ? 1 : e2.alpha;

    אם( isMatProd(e1) && (i2 || isScaled(e2) || isT(e2)) )
        MatOp_GEMM::makeExpr(res, (e1.flags & ~cv::GEMM_3_T)|(isT(e2) ? cv::GEMM_3_T : 0),
                             e1.a, e1.b, alpha1, e2.a, alpha2);
    אחרת אם( isMatProd(e2) && (i1 || isScaled(e1) || isT(e1)) )
        MatOp_GEMM::makeExpr(res, (e2.flags & ~cv::GEMM_3_T)|(isT(e1) ? cv::GEMM_3_T : 0),
                             e2.a, e2.b, alpha2, e1.a, alpha1);
    אחרת אם( هذا == e2.op )
        MatOp::add(e1, e2, res);
    אחרת
        e2.op->add(e1, e2, res);
}

فراغ MatOp_GEMM::subtract(ثابت MatExpr& e1, ثابت MatExpr& e2, MatExpr& res) ثابت
{
    CV_INSTRUMENT_REGION();

    منطقي i1 = isIdentity(e1), i2 = isIdentity(e2);
    مزدوج alpha1 = i1 ? 1 : e1.alpha, alpha2 = i2 ? 1 : e2.alpha;

    אם( isMatProd(e1) && (i2 || isScaled(e2) || isT(e2)) )
        MatOp_GEMM::makeExpr(res, (e1.flags & ~cv::GEMM_3_T)|(isT(e2) ? cv::GEMM_3_T : 0),
                             e1.a, e1.b, alpha1, e2.a, -alpha2);
    אחרת אם( isMatProd(e2) && (i1 || isScaled(e1) || isT(e1)) )
        MatOp_GEMM::makeExpr(res, (e2.flags & ~cv::GEMM_3_T)|(isT(e1) ? cv::GEMM_3_T : 0),
                            e2.a, e2.b, -alpha2, e1.a, alpha1);
    אחרת אם( هذا == e2.op )
        MatOp::subtract(e1, e2, res);
    אחרת
        e2.op->subtract(e1, e2, res);
}

فراغ MatOp_GEMM::multiply(ثابت MatExpr& e, مزدوج s, MatExpr& res) ثابت
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha *= s;
    res.beta *= s;
}

فراغ MatOp_GEMM::transpose(ثابت MatExpr& e, MatExpr& res) ثابت
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.flags = (!(e.flags & cv::GEMM_1_T) ? cv::GEMM_2_T : 0) |
                (!(e.flags & cv::GEMM_2_T) ? cv::GEMM_1_T : 0) |
                (!(e.flags & cv::GEMM_3_T) ? cv::GEMM_3_T : 0);
    swap(res.a, res.b);
}

مضمن فراغ MatOp_GEMM::makeExpr(MatExpr& res, صحيح flags, ثابت Mat& a, ثابت Mat& b,
                                 مزدوج alpha, ثابت Mat& c, مزدوج beta)
{
    res = MatExpr(&g_MatOp_GEMM, flags, a, b, c, alpha, beta);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

فراغ MatOp_Invert::assign(ثابت MatExpr& e, Mat& m, صحيح _type) ثابت
{
    Mat temp, &dst = _type == -1 || _type == e.a.type() ? m : temp;

    cv::invert(e.a, dst, e.flags);
    אם( dst.data != m.data )
        dst.convertTo(m, _type);
}

فراغ MatOp_Invert::matmul(ثابت MatExpr& e1, ثابت MatExpr& e2, MatExpr& res) ثابت
{
    אם( isInv(e1) && isIdentity(e2) )
        MatOp_Solve::makeExpr(res, e1.flags, e1.a, e2.a);
    אחרת אם( هذا == e2.op )
        MatOp::matmul(e1, e2, res);
    אחרת
        e2.op->matmul(e1, e2, res);
}

مضمن فراغ MatOp_Invert::makeExpr(MatExpr& res, صحيح method, ثابت Mat& m)
{
    res = MatExpr(&g_MatOp_Invert, method, m, Mat(), Mat(), 1, 0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

فراغ MatOp_Solve::assign(ثابت MatExpr& e, Mat& m, صحيح _type) ثابت
{
    Mat temp, &dst = _type == -1 || _type == e.a.type() ? m : temp;

    cv::solve(e.a, e.b, dst, e.flags);
    אם( dst.data != m.data )
        dst.convertTo(m, _type);
}

مضمن فراغ MatOp_Solve::makeExpr(MatExpr& res, صحيح method, ثابت Mat& a, ثابت Mat& b)
{
    res = MatExpr(&g_MatOp_Solve, method, a, b, Mat(), 1, 1);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

فراغ MatOp_Initializer::assign(ثابت MatExpr& e, Mat& m, صحيح _type) ثابت
{
    אם( _type == -1 )
        _type = e.a.type();

    m.create(e.a.size, _type);

    אם( e.flags == 'I' && e.a.dims <= 2 )
        setIdentity(m, Scalar(e.alpha));
    אחרת אם( e.flags == '0' )
        m = Scalar();
    אחרת אם( e.flags == '1' )
        m = Scalar(e.alpha);
    אחרת
        CV_Error(cv::Error::StsError, "Invalid matrix initializer type");
}

فراغ MatOp_Initializer::multiply(ثابت MatExpr& e, مزدوج s, MatExpr& res) ثابت
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha *= s;
}

مضمن فراغ MatOp_Initializer::makeExpr(MatExpr& res, صحيح method, Size sz, صحيح type, مزدوج alpha)
{
    res = MatExpr(getGlobalMatOpInitializer(), method, Mat(sz, type, (فراغ*)(size_t)0xEEEEEEEE), Mat(), Mat(), alpha, 0);
}

مضمن فراغ MatOp_Initializer::makeExpr(MatExpr& res, صحيح method, صحيح ndims, ثابت صحيح* sizes, صحيح type, مزدوج alpha)
{
    res = MatExpr(getGlobalMatOpInitializer(), method, Mat(ndims, sizes, type, (فراغ*)(size_t)0xEEEEEEEE), Mat(), Mat(), alpha, 0);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

MatExpr Mat::t() ثابت
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(*هذا);
    MatExpr e;
    MatOp_T::makeExpr(e, *هذا);
    החזר e;
}

MatExpr Mat::inv(صحيح method) ثابت
{
    CV_INSTRUMENT_REGION();

    MatExpr e;
    MatOp_Invert::makeExpr(e, method, *هذا);
    החזר e;
}


MatExpr Mat::mul(InputArray m, مزدوج scale) ثابت
{
    CV_INSTRUMENT_REGION();

    Mat b = m.getMat();
    // Unless the argument is a refcounted Mat/UMat, the header returned by getMat() may be
    // a non-owning view of caller memory (e.g. a scalar bound to _InputArray(const double&),
    // a Matx or a Vec) that the returned MatExpr can outlive, so snapshot it.
    // See https://github.com/opencv/opencv/issues/23577
    אם( !m.isMat() && !m.isUMat() )
        b = b.clone();

    MatExpr e;
    MatOp_Bin::makeExpr(e, '*', *هذا, b, scale);
    החזר e;
}

MatExpr Mat::zeros(صحيح rows, صحيح cols, صحيح type)
{
    CV_INSTRUMENT_REGION();

    MatExpr e;
    MatOp_Initializer::makeExpr(e, '0', Size(cols, rows), type);
    החזר e;
}

MatExpr Mat::zeros(Size size, صحيح type)
{
    CV_INSTRUMENT_REGION();

    MatExpr e;
    MatOp_Initializer::makeExpr(e, '0', size, type);
    החזר e;
}

MatExpr Mat::zeros(صحيح ndims, ثابت صحيح* sizes, صحيح type)
{
    CV_INSTRUMENT_REGION();

    MatExpr e;
    MatOp_Initializer::makeExpr(e, '0', ndims, sizes, type);
    החזר e;
}

MatExpr Mat::zeros(ثابت MatShape& shape, صحيح type)
{
    CV_INSTRUMENT_REGION();

    MatExpr e;
    MatOp_Initializer::makeExpr(e, '0', shape.dims, shape.p, type);
    החזר e;
}

MatExpr Mat::ones(صحيح rows, صحيح cols, صحيح type)
{
    CV_INSTRUMENT_REGION();

    MatExpr e;
    MatOp_Initializer::makeExpr(e, '1', Size(cols, rows), type);
    החזר e;
}

MatExpr Mat::ones(Size size, صحيح type)
{
    CV_INSTRUMENT_REGION();

    MatExpr e;
    MatOp_Initializer::makeExpr(e, '1', size, type);
    החזר e;
}

MatExpr Mat::ones(صحيح ndims, ثابت صحيح* sizes, صحيح type)
{
    CV_INSTRUMENT_REGION();

    MatExpr e;
    MatOp_Initializer::makeExpr(e, '1', ndims, sizes, type);
    החזר e;
}

MatExpr Mat::ones(ثابت MatShape& shape, صحيح type)
{
    CV_INSTRUMENT_REGION();

    MatExpr e;
    MatOp_Initializer::makeExpr(e, '1', shape.dims, shape.p, type);
    החזר e;
}

MatExpr Mat::eye(صحيح rows, صحيح cols, صحيح type)
{
    CV_INSTRUMENT_REGION();

    MatExpr e;
    MatOp_Initializer::makeExpr(e, 'I', Size(cols, rows), type);
    החזר e;
}

MatExpr Mat::eye(Size size, صحيح type)
{
    CV_INSTRUMENT_REGION();

    MatExpr e;
    MatOp_Initializer::makeExpr(e, 'I', size, type);
    החזר e;
}

فراغ MatExpr::swap(MatExpr& other)
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

_InputArray::_InputArray(ثابت MatExpr& expr)
{
    אם (!isIdentity(expr))
    {
        Mat result = expr;  // TODO improve through refcount == 1 of expr.a (inplace operation is possible - except gemm?)
        MatExpr result_expr(result);
        swap(const_cast<MatExpr&>(expr), result_expr);
    }
    CV_Assert(isIdentity(expr));
    init(FIXED_TYPE + FIXED_SIZE + MAT + ACCESS_READ, &expr.a);
}

} // cv::
