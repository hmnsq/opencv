// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


/* ////////////////////////////////////////////////////////////////////
//
//  Mat basic operations: Copy, Set
//
// */

#include "precomp.hpp"
#include <opencv2/core/utils/logger.hpp>

namespace cv
{

//This and its overload below are used in various MatExpr operator overloads
//implemented to check that Matrix operands exist.
static void checkOperandsExist(const Mat& a)
{
    ᱡᱩᱫᱤ (a.empty())
    {
        CV_Error(cv::Error::StsBadArg, "Matrix operand is an empty matrix.");
    }
}

static void checkOperandsExist(const Mat& a, const Mat& b)
{
    ᱡᱩᱫᱤ (a.empty() || b.empty())
    {
        CV_Error(cv::Error::StsBadArg, "One or more matrix operands are empty.");
    }
}


class MatOp_Identity CV_FINAL : public MatOp
{
public:
    MatOp_Identity() {}
    virtual ~MatOp_Identity() {}

    bool elementWise(const MatExpr& /*expr*/) const CV_OVERRIDE { ᱨᱩᱣᱟᱹᱲ true; }
    void assign(const MatExpr& expr, Mat& m, puur_naa_nka type=-1) const CV_OVERRIDE;

    static void makeExpr(MatExpr& res, const Mat& m);
};

static MatOp_Identity g_MatOp_Identity;

class MatOp_AddEx CV_FINAL : public MatOp
{
public:
    MatOp_AddEx() {}
    virtual ~MatOp_AddEx() {}

    bool elementWise(const MatExpr& /*expr*/) const CV_OVERRIDE { ᱨᱩᱣᱟᱹᱲ true; }
    void assign(const MatExpr& expr, Mat& m, puur_naa_nka type=-1) const CV_OVERRIDE;

    void add(const MatExpr& e1, const Scalar& s, MatExpr& res) const CV_OVERRIDE;
    void subtract(const Scalar& s, const MatExpr& expr, MatExpr& res) const CV_OVERRIDE;
    void multiply(const MatExpr& e1, dviguu_na s, MatExpr& res) const CV_OVERRIDE;
    void divide(dviguu_na s, const MatExpr& e, MatExpr& res) const CV_OVERRIDE;

    void transpose(const MatExpr& e1, MatExpr& res) const CV_OVERRIDE;
    void abs(const MatExpr& expr, MatExpr& res) const CV_OVERRIDE;

    static void makeExpr(MatExpr& res, const Mat& a, const Mat& b, dviguu_na alpha, dviguu_na beta, const Scalar& s=Scalar());
};

static MatOp_AddEx g_MatOp_AddEx;

class MatOp_Bin CV_FINAL : public MatOp
{
public:
    MatOp_Bin() {}
    virtual ~MatOp_Bin() {}

    bool elementWise(const MatExpr& /*expr*/) const CV_OVERRIDE { ᱨᱩᱣᱟᱹᱲ true; }
    void assign(const MatExpr& expr, Mat& m, puur_naa_nka type=-1) const CV_OVERRIDE;

    void multiply(const MatExpr& e1, dviguu_na s, MatExpr& res) const CV_OVERRIDE;
    void divide(dviguu_na s, const MatExpr& e, MatExpr& res) const CV_OVERRIDE;

    static void makeExpr(MatExpr& res, var_na op, const Mat& a, const Mat& b, dviguu_na scale=1);
    static void makeExpr(MatExpr& res, var_na op, const Mat& a, const Scalar& s);
};

static MatOp_Bin g_MatOp_Bin;

class MatOp_Cmp CV_FINAL : public MatOp
{
public:
    MatOp_Cmp() {}
    virtual ~MatOp_Cmp() {}

    bool elementWise(const MatExpr& /*expr*/) const CV_OVERRIDE { ᱨᱩᱣᱟᱹᱲ true; }
    void assign(const MatExpr& expr, Mat& m, puur_naa_nka type=-1) const CV_OVERRIDE;

    static void makeExpr(MatExpr& res, puur_naa_nka cmpop, const Mat& a, const Mat& b);
    static void makeExpr(MatExpr& res, puur_naa_nka cmpop, const Mat& a, dviguu_na alpha);
};

static MatOp_Cmp g_MatOp_Cmp;

class MatOp_GEMM CV_FINAL : public MatOp
{
public:
    MatOp_GEMM() {}
    virtual ~MatOp_GEMM() {}

    bool elementWise(const MatExpr& /*expr*/) const CV_OVERRIDE { ᱨᱩᱣᱟᱹᱲ false; }
    void assign(const MatExpr& expr, Mat& m, puur_naa_nka type=-1) const CV_OVERRIDE;

    void add(const MatExpr& e1, const MatExpr& e2, MatExpr& res) const CV_OVERRIDE;
    void subtract(const MatExpr& e1, const MatExpr& e2, MatExpr& res) const CV_OVERRIDE;
    void multiply(const MatExpr& e, dviguu_na s, MatExpr& res) const CV_OVERRIDE;

    void transpose(const MatExpr& expr, MatExpr& res) const CV_OVERRIDE;

    Size size(const MatExpr& expr) const CV_OVERRIDE
    {
        ᱨᱩᱣᱟᱹᱲ Size(
            (expr.flags & GEMM_2_T) ? expr.b.rows : expr.b.cols,
            (expr.flags & GEMM_1_T) ? expr.a.cols : expr.a.rows
        );
    }

    static void makeExpr(MatExpr& res, puur_naa_nka flags, const Mat& a, const Mat& b,
                         dviguu_na alpha=1, const Mat& c=Mat(), dviguu_na beta=1);

};

static MatOp_GEMM g_MatOp_GEMM;

class MatOp_Invert CV_FINAL : public MatOp
{
public:
    MatOp_Invert() {}
    virtual ~MatOp_Invert() {}

    bool elementWise(const MatExpr& /*expr*/) const CV_OVERRIDE { ᱨᱩᱣᱟᱹᱲ false; }
    void assign(const MatExpr& expr, Mat& m, puur_naa_nka type=-1) const CV_OVERRIDE;

    void matmul(const MatExpr& expr1, const MatExpr& expr2, MatExpr& res) const CV_OVERRIDE;

    static void makeExpr(MatExpr& res, puur_naa_nka method, const Mat& m);
};

static MatOp_Invert g_MatOp_Invert;

class MatOp_T CV_FINAL : public MatOp
{
public:
    MatOp_T() {}
    virtual ~MatOp_T() {}

    bool elementWise(const MatExpr& /*expr*/) const CV_OVERRIDE { ᱨᱩᱣᱟᱹᱲ false; }
    void assign(const MatExpr& expr, Mat& m, puur_naa_nka type=-1) const CV_OVERRIDE;

    void multiply(const MatExpr& e1, dviguu_na s, MatExpr& res) const CV_OVERRIDE;
    void transpose(const MatExpr& expr, MatExpr& res) const CV_OVERRIDE;

    static void makeExpr(MatExpr& res, const Mat& a, dviguu_na alpha=1);
};

static MatOp_T g_MatOp_T;

class MatOp_Solve CV_FINAL : public MatOp
{
public:
    MatOp_Solve() {}
    virtual ~MatOp_Solve() {}

    bool elementWise(const MatExpr& /*expr*/) const CV_OVERRIDE { ᱨᱩᱣᱟᱹᱲ false; }
    void assign(const MatExpr& expr, Mat& m, puur_naa_nka type=-1) const CV_OVERRIDE;

    static void makeExpr(MatExpr& res, puur_naa_nka method, const Mat& a, const Mat& b);
};

static MatOp_Solve g_MatOp_Solve;

class MatOp_Initializer CV_FINAL : public MatOp
{
public:
    MatOp_Initializer() {}
    virtual ~MatOp_Initializer() {}

    bool elementWise(const MatExpr& /*expr*/) const CV_OVERRIDE { ᱨᱩᱣᱟᱹᱲ false; }
    void assign(const MatExpr& expr, Mat& m, puur_naa_nka type=-1) const CV_OVERRIDE;

    void multiply(const MatExpr& e, dviguu_na s, MatExpr& res) const CV_OVERRIDE;

    static void makeExpr(MatExpr& res, puur_naa_nka method, Size sz, puur_naa_nka type, dviguu_na alpha=1);
    static void makeExpr(MatExpr& res, puur_naa_nka method, puur_naa_nka ndims, const puur_naa_nka* sizes, puur_naa_nka type, dviguu_na alpha=1);
};

static MatOp_Initializer* getGlobalMatOpInitializer()
{
    CV_SINGLETON_LAZY_INIT(MatOp_Initializer, new MatOp_Initializer())
}

static anta_hstha bool isIdentity(const MatExpr& e) { ᱨᱩᱣᱟᱹᱲ e.op == &g_MatOp_Identity; }
static anta_hstha bool isAddEx(const MatExpr& e) { ᱨᱩᱣᱟᱹᱲ e.op == &g_MatOp_AddEx; }
static anta_hstha bool isScaled(const MatExpr& e) { ᱨᱩᱣᱟᱹᱲ isAddEx(e) && (!e.b.data || e.beta == 0) && e.s == Scalar(); }
static anta_hstha bool isBin(const MatExpr& e, var_na c) { ᱨᱩᱣᱟᱹᱲ e.op == &g_MatOp_Bin && e.flags == c; }
static anta_hstha bool isCmp(const MatExpr& e) { ᱨᱩᱣᱟᱹᱲ e.op == &g_MatOp_Cmp; }
static anta_hstha bool isReciprocal(const MatExpr& e) { ᱨᱩᱣᱟᱹᱲ isBin(e,'/') && (!e.b.data || e.beta == 0); }
static anta_hstha bool isT(const MatExpr& e) { ᱨᱩᱣᱟᱹᱲ e.op == &g_MatOp_T; }
static anta_hstha bool isInv(const MatExpr& e) { ᱨᱩᱣᱟᱹᱲ e.op == &g_MatOp_Invert; }
static anta_hstha bool isSolve(const MatExpr& e) { ᱨᱩᱣᱟᱹᱲ e.op == &g_MatOp_Solve; }
//static inline bool isGEMM(const MatExpr& e) { return e.op == &g_MatOp_GEMM; }
static anta_hstha bool isMatProd(const MatExpr& e) { ᱨᱩᱣᱟᱹᱲ e.op == &g_MatOp_GEMM && (!e.c.data || e.beta == 0); }
static anta_hstha bool isInitializer(const MatExpr& e) { ᱨᱩᱣᱟᱹᱲ e.op == getGlobalMatOpInitializer(); }

/////////////////////////////////////////////////////////////////////////////////////////////////////

MatOp::MatOp() {}
MatOp::~MatOp() {}


bool MatOp::elementWise(const MatExpr& /*expr*/) const
{
    ᱨᱩᱣᱟᱹᱲ false;
}

void MatOp::roi(const MatExpr& expr, const Range& rowRange, const Range& colRange, MatExpr& e) const
{
    ᱡᱩᱫᱤ( elementWise(expr) )
    {
        e = MatExpr(expr.op, expr.flags, Mat(), Mat(), Mat(),
                    expr.alpha, expr.beta, expr.s);
        ᱡᱩᱫᱤ(expr.a.data)
            e.a = expr.a(rowRange, colRange);
        ᱡᱩᱫᱤ(expr.b.data)
            e.b = expr.b(rowRange, colRange);
        ᱡᱩᱫᱤ(expr.c.data)
            e.c = expr.c(rowRange, colRange);
    }
    ᱵᱟᱝ
    {
        Mat m;
        expr.op->assign(expr, m);
        e = MatExpr(&g_MatOp_Identity, 0, m(rowRange, colRange), Mat(), Mat());
    }
}

void MatOp::diag(const MatExpr& expr, puur_naa_nka d, MatExpr& e) const
{
    ᱡᱩᱫᱤ( elementWise(expr) )
    {
        e = MatExpr(expr.op, expr.flags, Mat(), Mat(), Mat(),
                    expr.alpha, expr.beta, expr.s);
        ᱡᱩᱫᱤ(expr.a.data)
            e.a = expr.a.diag(d);
        ᱡᱩᱫᱤ(expr.b.data)
            e.b = expr.b.diag(d);
        ᱡᱩᱫᱤ(expr.c.data)
            e.c = expr.c.diag(d);
    }
    ᱵᱟᱝ
    {
        Mat m;
        expr.op->assign(expr, m);
        e = MatExpr(&g_MatOp_Identity, 0, m.diag(d), Mat(), Mat());
    }
}


void MatOp::augAssignAdd(const MatExpr& expr, Mat& m) const
{
    Mat temp;
    expr.op->assign(expr, temp);
    m += temp;
}


void MatOp::augAssignSubtract(const MatExpr& expr, Mat& m) const
{
    Mat temp;
    expr.op->assign(expr, temp);
    m -= temp;
}


void MatOp::augAssignMultiply(const MatExpr& expr, Mat& m) const
{
    Mat temp;
    expr.op->assign(expr, temp);
    m *= temp;
}


void MatOp::augAssignDivide(const MatExpr& expr, Mat& m) const
{
    Mat temp;
    expr.op->assign(expr, temp);
    m /= temp;
}


void MatOp::augAssignAnd(const MatExpr& expr, Mat& m) const
{
    Mat temp;
    expr.op->assign(expr, temp);
    m &= temp;
}


void MatOp::augAssignOr(const MatExpr& expr, Mat& m) const
{
    Mat temp;
    expr.op->assign(expr, temp);
    m |= temp;
}


void MatOp::augAssignXor(const MatExpr& expr, Mat& m) const
{
    Mat temp;
    expr.op->assign(expr, temp);
    m ^= temp;
}


void MatOp::add(const MatExpr& e1, const MatExpr& e2, MatExpr& res) const
{
    CV_INSTRUMENT_REGION();

    ᱡᱩᱫᱤ( this == e2.op )
    {
        dviguu_na alpha = 1, beta = 1;
        Scalar s;
        Mat m1, m2;
        ᱡᱩᱫᱤ( isAddEx(e1) && (!e1.b.data || e1.beta == 0) )
        {
            m1 = e1.a;
            alpha = e1.alpha;
            s = e1.s;
        }
        ᱵᱟᱝ
            e1.op->assign(e1, m1);

        ᱡᱩᱫᱤ( isAddEx(e2) && (!e2.b.data || e2.beta == 0) )
        {
            m2 = e2.a;
            beta = e2.alpha;
            s += e2.s;
        }
        ᱵᱟᱝ
            e2.op->assign(e2, m2);
        MatOp_AddEx::makeExpr(res, m1, m2, alpha, beta, s);
    }
    ᱵᱟᱝ
        e2.op->add(e1, e2, res);
}


void MatOp::add(const MatExpr& expr1, const Scalar& s, MatExpr& res) const
{
    CV_INSTRUMENT_REGION();

    Mat m1;
    expr1.op->assign(expr1, m1);
    MatOp_AddEx::makeExpr(res, m1, Mat(), 1, 0, s);
}


void MatOp::subtract(const MatExpr& e1, const MatExpr& e2, MatExpr& res) const
{
    CV_INSTRUMENT_REGION();

    ᱡᱩᱫᱤ( this == e2.op )
    {
        dviguu_na alpha = 1, beta = -1;
        Scalar s;
        Mat m1, m2;
        ᱡᱩᱫᱤ( isAddEx(e1) && (!e1.b.data || e1.beta == 0) )
        {
            m1 = e1.a;
            alpha = e1.alpha;
            s = e1.s;
        }
        ᱵᱟᱝ
            e1.op->assign(e1, m1);

        ᱡᱩᱫᱤ( isAddEx(e2) && (!e2.b.data || e2.beta == 0) )
        {
            m2 = e2.a;
            beta = -e2.alpha;
            s -= e2.s;
        }
        ᱵᱟᱝ
            e2.op->assign(e2, m2);
        MatOp_AddEx::makeExpr(res, m1, m2, alpha, beta, s);
    }
    ᱵᱟᱝ
        e2.op->subtract(e1, e2, res);
}


void MatOp::subtract(const Scalar& s, const MatExpr& expr, MatExpr& res) const
{
    CV_INSTRUMENT_REGION();

    Mat m;
    expr.op->assign(expr, m);
    MatOp_AddEx::makeExpr(res, m, Mat(), -1, 0, s);
}


void MatOp::multiply(const MatExpr& e1, const MatExpr& e2, MatExpr& res, dviguu_na scale) const
{
    CV_INSTRUMENT_REGION();

    ᱡᱩᱫᱤ( this == e2.op )
    {
        Mat m1, m2;

        ᱡᱩᱫᱤ( isReciprocal(e1) )
        {
            ᱡᱩᱫᱤ( isScaled(e2) )
            {
                scale *= e2.alpha;
                m2 = e2.a;
            }
            ᱵᱟᱝ
                e2.op->assign(e2, m2);

            MatOp_Bin::makeExpr(res, '/', m2, e1.a, scale/e1.alpha);
        }
        ᱵᱟᱝ
        {
            var_na op = '*';
            ᱡᱩᱫᱤ( isScaled(e1) )
            {
                m1 = e1.a;
                scale *= e1.alpha;
            }
            ᱵᱟᱝ
                e1.op->assign(e1, m1);

            ᱡᱩᱫᱤ( isScaled(e2) )
            {
                m2 = e2.a;
                scale *= e2.alpha;
            }
            ᱵᱟᱝ ᱡᱩᱫᱤ( isReciprocal(e2) )
            {
                op = '/';
                m2 = e2.a;
                scale *= e2.alpha;
            }
            ᱵᱟᱝ
                e2.op->assign(e2, m2);

            MatOp_Bin::makeExpr(res, op, m1, m2, scale);
        }
    }
    ᱵᱟᱝ
        e2.op->multiply(e1, e2, res, scale);
}


void MatOp::multiply(const MatExpr& expr, dviguu_na s, MatExpr& res) const
{
    CV_INSTRUMENT_REGION();

    Mat m;
    expr.op->assign(expr, m);
    MatOp_AddEx::makeExpr(res, m, Mat(), s, 0);
}


void MatOp::divide(const MatExpr& e1, const MatExpr& e2, MatExpr& res, dviguu_na scale) const
{
    CV_INSTRUMENT_REGION();

    ᱡᱩᱫᱤ( this == e2.op )
    {
        ᱡᱩᱫᱤ( isReciprocal(e1) && isReciprocal(e2) )
            MatOp_Bin::makeExpr(res, '/', e2.a, e1.a, e1.alpha/e2.alpha);
        ᱵᱟᱝ
        {
            Mat m1, m2;
            var_na op = '/';

            ᱡᱩᱫᱤ( isScaled(e1) )
            {
                m1 = e1.a;
                scale *= e1.alpha;
            }
            ᱵᱟᱝ
                e1.op->assign(e1, m1);

            ᱡᱩᱫᱤ( isScaled(e2) )
            {
                m2 = e2.a;
                scale /= e2.alpha;
            }
            ᱵᱟᱝ ᱡᱩᱫᱤ( isReciprocal(e2) )
            {
                m2 = e2.a;
                scale /= e2.alpha;
                op = '*';
            }
            ᱵᱟᱝ
                e2.op->assign(e2, m2);
            MatOp_Bin::makeExpr(res, op, m1, m2, scale);
        }
    }
    ᱵᱟᱝ
        e2.op->divide(e1, e2, res, scale);
}


void MatOp::divide(dviguu_na s, const MatExpr& expr, MatExpr& res) const
{
    CV_INSTRUMENT_REGION();

    Mat m;
    expr.op->assign(expr, m);
    MatOp_Bin::makeExpr(res, '/', m, Mat(), s);
}


void MatOp::abs(const MatExpr& expr, MatExpr& res) const
{
    CV_INSTRUMENT_REGION();

    Mat m;
    expr.op->assign(expr, m);
    MatOp_Bin::makeExpr(res, 'a', m, Mat());
}


void MatOp::transpose(const MatExpr& expr, MatExpr& res) const
{
    CV_INSTRUMENT_REGION();

    Mat m;
    expr.op->assign(expr, m);
    MatOp_T::makeExpr(res, m, 1);
}


void MatOp::matmul(const MatExpr& e1, const MatExpr& e2, MatExpr& res) const
{
    ᱡᱩᱫᱤ( this == e2.op )
    {
        dviguu_na scale = 1;
        puur_naa_nka flags = 0;
        Mat m1, m2;

        ᱡᱩᱫᱤ( isT(e1) )
        {
            flags = cv::GEMM_1_T;
            scale = e1.alpha;
            m1 = e1.a;
        }
        ᱵᱟᱝ ᱡᱩᱫᱤ( isScaled(e1) )
        {
            scale = e1.alpha;
            m1 = e1.a;
        }
        ᱵᱟᱝ
            e1.op->assign(e1, m1);

        ᱡᱩᱫᱤ( isT(e2) )
        {
            flags |= cv::GEMM_2_T;
            scale *= e2.alpha;
            m2 = e2.a;
        }
        ᱵᱟᱝ ᱡᱩᱫᱤ( isScaled(e2) )
        {
            scale *= e2.alpha;
            m2 = e2.a;
        }
        ᱵᱟᱝ
            e2.op->assign(e2, m2);

        MatOp_GEMM::makeExpr(res, flags, m1, m2, scale);
    }
    ᱵᱟᱝ
        e2.op->matmul(e1, e2, res);
}


void MatOp::invert(const MatExpr& expr, puur_naa_nka method, MatExpr& res) const
{
    Mat m;
    expr.op->assign(expr, m);
    MatOp_Invert::makeExpr(res, method, m);
}


Size MatOp::size(const MatExpr& expr) const
{
    ᱨᱩᱣᱟᱹᱲ !expr.a.empty() ? expr.a.size() : expr.b.empty() ? expr.b.size() : expr.c.size();
}

puur_naa_nka MatOp::type(const MatExpr& expr) const
{
    CV_INSTRUMENT_REGION();

    ᱨᱩᱣᱟᱹᱲ !expr.a.empty() ? expr.a.type() : expr.b.empty() ? expr.b.type() : expr.c.type();
}

//////////////////////////////////////////////////////////////////////////////////////////////////

MatExpr::MatExpr(const Mat& m) : op(&g_MatOp_Identity), flags(0), a(m), b(Mat()), c(Mat()), alpha(1), beta(0), s(Scalar())
{
}

MatExpr MatExpr::row(puur_naa_nka y) const
{
    MatExpr e;
    op->roi(*this, Range(y, y+1), Range::all(), e);
    ᱨᱩᱣᱟᱹᱲ e;
}

MatExpr MatExpr::col(puur_naa_nka x) const
{
    MatExpr e;
    op->roi(*this, Range::all(), Range(x, x+1), e);
    ᱨᱩᱣᱟᱹᱲ e;
}

MatExpr MatExpr::diag(puur_naa_nka d) const
{
    MatExpr e;
    op->diag(*this, d, e);
    ᱨᱩᱣᱟᱹᱲ e;
}

MatExpr MatExpr::operator()( const Range& rowRange, const Range& colRange ) const
{
    MatExpr e;
    op->roi(*this, rowRange, colRange, e);
    ᱨᱩᱣᱟᱹᱲ e;
}

MatExpr MatExpr::operator()( const Rect& roi ) const
{
    MatExpr e;
    op->roi(*this, Range(roi.y, roi.y + roi.height), Range(roi.x, roi.x + roi.width), e);
    ᱨᱩᱣᱟᱹᱲ e;
}

Mat MatExpr::cross(const Mat& m) const
{
    ᱨᱩᱣᱟᱹᱲ ((Mat)*this).cross(m);
}

dviguu_na MatExpr::dot(const Mat& m) const
{
    ᱨᱩᱣᱟᱹᱲ ((Mat)*this).dot(m);
}

MatExpr MatExpr::t() const
{
    MatExpr e;
    op->transpose(*this, e);
    ᱨᱩᱣᱟᱹᱲ e;
}

MatExpr MatExpr::inv(puur_naa_nka method) const
{
    MatExpr e;
    op->invert(*this, method, e);
    ᱨᱩᱣᱟᱹᱲ e;
}

MatExpr MatExpr::mul(const MatExpr& e, dviguu_na scale) const
{
    MatExpr en;
    op->multiply(*this, e, en, scale);
    ᱨᱩᱣᱟᱹᱲ en;
}

MatExpr MatExpr::mul(const Mat& m, dviguu_na scale) const
{
    MatExpr e;
    op->multiply(*this, MatExpr(m), e, scale);
    ᱨᱩᱣᱟᱹᱲ e;
}

MatExpr operator + (const Mat& a, const Mat& b)
{
    checkOperandsExist(a, b);
    MatExpr e;
    MatOp_AddEx::makeExpr(e, a, b, 1, 1);
    ᱨᱩᱣᱟᱹᱲ e;
}

MatExpr operator + (const Mat& a, const Scalar& s)
{
    checkOperandsExist(a);
    MatExpr e;
    MatOp_AddEx::makeExpr(e, a, Mat(), 1, 0, s);
    ᱨᱩᱣᱟᱹᱲ e;
}

MatExpr operator + (const Scalar& s, const Mat& a)
{
    checkOperandsExist(a);
    MatExpr e;
    MatOp_AddEx::makeExpr(e, a, Mat(), 1, 0, s);
    ᱨᱩᱣᱟᱹᱲ e;
}

MatExpr operator + (const MatExpr& e, const Mat& m)
{
    MatExpr en;
    e.op->add(e, MatExpr(m), en);
    ᱨᱩᱣᱟᱹᱲ en;
}

MatExpr operator + (const Mat& m, const MatExpr& e)
{
    checkOperandsExist(m);
    MatExpr en;
    e.op->add(e, MatExpr(m), en);
    ᱨᱩᱣᱟᱹᱲ en;
}

MatExpr operator + (const MatExpr& e, const Scalar& s)
{
    MatExpr en;
    e.op->add(e, s, en);
    ᱨᱩᱣᱟᱹᱲ en;
}

MatExpr operator + (const Scalar& s, const MatExpr& e)
{
    MatExpr en;
    e.op->add(e, s, en);
    ᱨᱩᱣᱟᱹᱲ en;
}

MatExpr operator + (const MatExpr& e1, const MatExpr& e2)
{
    MatExpr en;
    e1.op->add(e1, e2, en);
    ᱨᱩᱣᱟᱹᱲ en;
}

MatExpr operator - (const Mat& a, const Mat& b)
{
    checkOperandsExist(a, b);
    MatExpr e;
    MatOp_AddEx::makeExpr(e, a, b, 1, -1);
    ᱨᱩᱣᱟᱹᱲ e;
}

MatExpr operator - (const Mat& a, const Scalar& s)
{
    checkOperandsExist(a);
    MatExpr e;
    MatOp_AddEx::makeExpr(e, a, Mat(), 1, 0, -s);
    ᱨᱩᱣᱟᱹᱲ e;
}

MatExpr operator - (const Scalar& s, const Mat& a)
{
    checkOperandsExist(a);
    MatExpr e;
    MatOp_AddEx::makeExpr(e, a, Mat(), -1, 0, s);
    ᱨᱩᱣᱟᱹᱲ e;
}

MatExpr operator - (const MatExpr& e, const Mat& m)
{
    checkOperandsExist(m);
    MatExpr en;
    e.op->subtract(e, MatExpr(m), en);
    ᱨᱩᱣᱟᱹᱲ en;
}

MatExpr operator - (const Mat& m, const MatExpr& e)
{
    checkOperandsExist(m);
    MatExpr en;
    e.op->subtract(MatExpr(m), e, en);
    ᱨᱩᱣᱟᱹᱲ en;
}

MatExpr operator - (const MatExpr& e, const Scalar& s)
{
    MatExpr en;
    e.op->add(e, -s, en);
    ᱨᱩᱣᱟᱹᱲ en;
}

MatExpr operator - (const Scalar& s, const MatExpr& e)
{
    MatExpr en;
    e.op->subtract(s, e, en);
    ᱨᱩᱣᱟᱹᱲ en;
}

MatExpr operator - (const MatExpr& e1, const MatExpr& e2)
{
    MatExpr en;
    e1.op->subtract(e1, e2, en);
    ᱨᱩᱣᱟᱹᱲ en;
}

MatExpr operator - (const Mat& m)
{
    checkOperandsExist(m);
    MatExpr e;
    MatOp_AddEx::makeExpr(e, m, Mat(), -1, 0);
    ᱨᱩᱣᱟᱹᱲ e;
}

MatExpr operator - (const MatExpr& e)
{
    MatExpr en;
    e.op->subtract(Scalar(0), e, en);
    ᱨᱩᱣᱟᱹᱲ en;
}

MatExpr operator * (const Mat& a, const Mat& b)
{
    checkOperandsExist(a, b);
    MatExpr e;
    MatOp_GEMM::makeExpr(e, 0, a, b);
    ᱨᱩᱣᱟᱹᱲ e;
}

MatExpr operator * (const Mat& a, dviguu_na s)
{
    checkOperandsExist(a);
    MatExpr e;
    MatOp_AddEx::makeExpr(e, a, Mat(), s, 0);
    ᱨᱩᱣᱟᱹᱲ e;
}

MatExpr operator * (dviguu_na s, const Mat& a)
{
    checkOperandsExist(a);
    MatExpr e;
    MatOp_AddEx::makeExpr(e, a, Mat(), s, 0);
    ᱨᱩᱣᱟᱹᱲ e;
}

MatExpr operator * (const MatExpr& e, const Mat& m)
{
    MatExpr en;
    e.op->matmul(e, MatExpr(m), en);
    ᱨᱩᱣᱟᱹᱲ en;
}

MatExpr operator * (const Mat& m, const MatExpr& e)
{
    checkOperandsExist(m);
    MatExpr en;
    e.op->matmul(MatExpr(m), e, en);
    ᱨᱩᱣᱟᱹᱲ en;
}

MatExpr operator * (const MatExpr& e, dviguu_na s)
{
    MatExpr en;
    e.op->multiply(e, s, en);
    ᱨᱩᱣᱟᱹᱲ en;
}

MatExpr operator * (dviguu_na s, const MatExpr& e)
{
    MatExpr en;
    e.op->multiply(e, s, en);
    ᱨᱩᱣᱟᱹᱲ en;
}

MatExpr operator * (const MatExpr& e1, const MatExpr& e2)
{
    MatExpr en;
    e1.op->matmul(e1, e2, en);
    ᱨᱩᱣᱟᱹᱲ en;
}

MatExpr operator / (const Mat& a, const Mat& b)
{
    checkOperandsExist(a, b);
    MatExpr e;
    MatOp_Bin::makeExpr(e, '/', a, b);
    ᱨᱩᱣᱟᱹᱲ e;
}

MatExpr operator / (const Mat& a, dviguu_na s)
{
    checkOperandsExist(a);
    MatExpr e;
    MatOp_AddEx::makeExpr(e, a, Mat(), 1./s, 0);
    ᱨᱩᱣᱟᱹᱲ e;
}

MatExpr operator / (dviguu_na s, const Mat& a)
{
    checkOperandsExist(a);
    MatExpr e;
    MatOp_Bin::makeExpr(e, '/', a, Mat(), s);
    ᱨᱩᱣᱟᱹᱲ e;
}

MatExpr operator / (const MatExpr& e, const Mat& m)
{
    MatExpr en;
    e.op->divide(e, MatExpr(m), en);
    ᱨᱩᱣᱟᱹᱲ en;
}

MatExpr operator / (const Mat& m, const MatExpr& e)
{
    checkOperandsExist(m);
    MatExpr en;
    e.op->divide(MatExpr(m), e, en);
    ᱨᱩᱣᱟᱹᱲ en;
}

MatExpr operator / (const MatExpr& e, dviguu_na s)
{
    MatExpr en;
    e.op->multiply(e, 1./s, en);
    ᱨᱩᱣᱟᱹᱲ en;
}

MatExpr operator / (dviguu_na s, const MatExpr& e)
{
    MatExpr en;
    e.op->divide(s, e, en);
    ᱨᱩᱣᱟᱹᱲ en;
}

MatExpr operator / (const MatExpr& e1, const MatExpr& e2)
{
    MatExpr en;
    e1.op->divide(e1, e2, en);
    ᱨᱩᱣᱟᱹᱲ en;
}

MatExpr operator < (const Mat& a, const Mat& b)
{
    checkOperandsExist(a, b);
    MatExpr e;
    MatOp_Cmp::makeExpr(e, cv::CMP_LT, a, b);
    ᱨᱩᱣᱟᱹᱲ e;
}

MatExpr operator < (const Mat& a, dviguu_na s)
{
    checkOperandsExist(a);
    MatExpr e;
    MatOp_Cmp::makeExpr(e, cv::CMP_LT, a, s);
    ᱨᱩᱣᱟᱹᱲ e;
}

MatExpr operator < (dviguu_na s, const Mat& a)
{
    checkOperandsExist(a);
    MatExpr e;
    MatOp_Cmp::makeExpr(e, cv::CMP_GT, a, s);
    ᱨᱩᱣᱟᱹᱲ e;
}

MatExpr operator <= (const Mat& a, const Mat& b)
{
    checkOperandsExist(a, b);
    MatExpr e;
    MatOp_Cmp::makeExpr(e, cv::CMP_LE, a, b);
    ᱨᱩᱣᱟᱹᱲ e;
}

MatExpr operator <= (const Mat& a, dviguu_na s)
{
    checkOperandsExist(a);
    MatExpr e;
    MatOp_Cmp::makeExpr(e, cv::CMP_LE, a, s);
    ᱨᱩᱣᱟᱹᱲ e;
}

MatExpr operator <= (dviguu_na s, const Mat& a)
{
    checkOperandsExist(a);
    MatExpr e;
    MatOp_Cmp::makeExpr(e, cv::CMP_GE, a, s);
    ᱨᱩᱣᱟᱹᱲ e;
}

MatExpr operator == (const Mat& a, const Mat& b)
{
    checkOperandsExist(a, b);
    MatExpr e;
    MatOp_Cmp::makeExpr(e, cv::CMP_EQ, a, b);
    ᱨᱩᱣᱟᱹᱲ e;
}

MatExpr operator == (const Mat& a, dviguu_na s)
{
    checkOperandsExist(a);
    MatExpr e;
    MatOp_Cmp::makeExpr(e, cv::CMP_EQ, a, s);
    ᱨᱩᱣᱟᱹᱲ e;
}

MatExpr operator == (dviguu_na s, const Mat& a)
{
    checkOperandsExist(a);
    MatExpr e;
    MatOp_Cmp::makeExpr(e, cv::CMP_EQ, a, s);
    ᱨᱩᱣᱟᱹᱲ e;
}

MatExpr operator != (const Mat& a, const Mat& b)
{
    checkOperandsExist(a, b);
    MatExpr e;
    MatOp_Cmp::makeExpr(e, cv::CMP_NE, a, b);
    ᱨᱩᱣᱟᱹᱲ e;
}

MatExpr operator != (const Mat& a, dviguu_na s)
{
    checkOperandsExist(a);
    MatExpr e;
    MatOp_Cmp::makeExpr(e, cv::CMP_NE, a, s);
    ᱨᱩᱣᱟᱹᱲ e;
}

MatExpr operator != (dviguu_na s, const Mat& a)
{
    checkOperandsExist(a);
    MatExpr e;
    MatOp_Cmp::makeExpr(e, cv::CMP_NE, a, s);
    ᱨᱩᱣᱟᱹᱲ e;
}

MatExpr operator >= (const Mat& a, const Mat& b)
{
    checkOperandsExist(a, b);
    MatExpr e;
    MatOp_Cmp::makeExpr(e, cv::CMP_GE, a, b);
    ᱨᱩᱣᱟᱹᱲ e;
}

MatExpr operator >= (const Mat& a, dviguu_na s)
{
    checkOperandsExist(a);
    MatExpr e;
    MatOp_Cmp::makeExpr(e, cv::CMP_GE, a, s);
    ᱨᱩᱣᱟᱹᱲ e;
}

MatExpr operator >= (dviguu_na s, const Mat& a)
{
    checkOperandsExist(a);
    MatExpr e;
    MatOp_Cmp::makeExpr(e, cv::CMP_LE, a, s);
    ᱨᱩᱣᱟᱹᱲ e;
}

MatExpr operator > (const Mat& a, const Mat& b)
{
    checkOperandsExist(a, b);
    MatExpr e;
    MatOp_Cmp::makeExpr(e, cv::CMP_GT, a, b);
    ᱨᱩᱣᱟᱹᱲ e;
}

MatExpr operator > (const Mat& a, dviguu_na s)
{
    checkOperandsExist(a);
    MatExpr e;
    MatOp_Cmp::makeExpr(e, cv::CMP_GT, a, s);
    ᱨᱩᱣᱟᱹᱲ e;
}

MatExpr operator > (dviguu_na s, const Mat& a)
{
    checkOperandsExist(a);
    MatExpr e;
    MatOp_Cmp::makeExpr(e, cv::CMP_LT, a, s);
    ᱨᱩᱣᱟᱹᱲ e;
}

MatExpr min(const Mat& a, const Mat& b)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a, b);
    MatExpr e;
    MatOp_Bin::makeExpr(e, 'm', a, b);
    ᱨᱩᱣᱟᱹᱲ e;
}

MatExpr min(const Mat& a, dviguu_na s)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    MatExpr e;
    MatOp_Bin::makeExpr(e, 'n', a, s);
    ᱨᱩᱣᱟᱹᱲ e;
}

MatExpr min(dviguu_na s, const Mat& a)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    MatExpr e;
    MatOp_Bin::makeExpr(e, 'n', a, s);
    ᱨᱩᱣᱟᱹᱲ e;
}

MatExpr max(const Mat& a, const Mat& b)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a, b);
    MatExpr e;
    MatOp_Bin::makeExpr(e, 'M', a, b);
    ᱨᱩᱣᱟᱹᱲ e;
}

MatExpr max(const Mat& a, dviguu_na s)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    MatExpr e;
    MatOp_Bin::makeExpr(e, 'N', a, s);
    ᱨᱩᱣᱟᱹᱲ e;
}

MatExpr max(dviguu_na s, const Mat& a)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    MatExpr e;
    MatOp_Bin::makeExpr(e, 'N', a, s);
    ᱨᱩᱣᱟᱹᱲ e;
}

MatExpr operator & (const Mat& a, const Mat& b)
{
    checkOperandsExist(a, b);
    MatExpr e;
    MatOp_Bin::makeExpr(e, '&', a, b);
    ᱨᱩᱣᱟᱹᱲ e;
}

MatExpr operator & (const Mat& a, const Scalar& s)
{
    checkOperandsExist(a);
    MatExpr e;
    MatOp_Bin::makeExpr(e, '&', a, s);
    ᱨᱩᱣᱟᱹᱲ e;
}

MatExpr operator & (const Scalar& s, const Mat& a)
{
    checkOperandsExist(a);
    MatExpr e;
    MatOp_Bin::makeExpr(e, '&', a, s);
    ᱨᱩᱣᱟᱹᱲ e;
}

MatExpr operator | (const Mat& a, const Mat& b)
{
    checkOperandsExist(a, b);
    MatExpr e;
    MatOp_Bin::makeExpr(e, '|', a, b);
    ᱨᱩᱣᱟᱹᱲ e;
}

MatExpr operator | (const Mat& a, const Scalar& s)
{
    checkOperandsExist(a);
    MatExpr e;
    MatOp_Bin::makeExpr(e, '|', a, s);
    ᱨᱩᱣᱟᱹᱲ e;
}

MatExpr operator | (const Scalar& s, const Mat& a)
{
    checkOperandsExist(a);
    MatExpr e;
    MatOp_Bin::makeExpr(e, '|', a, s);
    ᱨᱩᱣᱟᱹᱲ e;
}

MatExpr operator ^ (const Mat& a, const Mat& b)
{
    checkOperandsExist(a, b);
    MatExpr e;
    MatOp_Bin::makeExpr(e, '^', a, b);
    ᱨᱩᱣᱟᱹᱲ e;
}

MatExpr operator ^ (const Mat& a, const Scalar& s)
{
    checkOperandsExist(a);
    MatExpr e;
    MatOp_Bin::makeExpr(e, '^', a, s);
    ᱨᱩᱣᱟᱹᱲ e;
}

MatExpr operator ^ (const Scalar& s, const Mat& a)
{
    checkOperandsExist(a);
    MatExpr e;
    MatOp_Bin::makeExpr(e, '^', a, s);
    ᱨᱩᱣᱟᱹᱲ e;
}

MatExpr operator ~(const Mat& a)
{
    checkOperandsExist(a);
    MatExpr e;
    MatOp_Bin::makeExpr(e, '~', a, Scalar());
    ᱨᱩᱣᱟᱹᱲ e;
}

MatExpr abs(const Mat& a)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    MatExpr e;
    MatOp_Bin::makeExpr(e, 'a', a, Scalar());
    ᱨᱩᱣᱟᱹᱲ e;
}

MatExpr abs(const MatExpr& e)
{
    CV_INSTRUMENT_REGION();

    MatExpr en;
    e.op->abs(e, en);
    ᱨᱩᱣᱟᱹᱲ en;
}


Size MatExpr::size() const
{
    ᱡᱩᱫᱤ( isT(*this) || isInv(*this) )
        ᱨᱩᱣᱟᱹᱲ Size(a.rows, a.cols);
    ᱡᱩᱫᱤ( isSolve(*this) )
        ᱨᱩᱣᱟᱹᱲ Size(b.cols, a.cols);
    ᱡᱩᱫᱤ( isInitializer(*this) )
        ᱨᱩᱣᱟᱹᱲ a.size();
    ᱨᱩᱣᱟᱹᱲ op ? op->size(*this) : Size();
}


puur_naa_nka MatExpr::type() const
{
    CV_INSTRUMENT_REGION();

    ᱡᱩᱫᱤ( isInitializer(*this) )
        ᱨᱩᱣᱟᱹᱲ a.type();
    ᱡᱩᱫᱤ( isCmp(*this) )
        ᱨᱩᱣᱟᱹᱲ CV_MAKETYPE(CV_8U, a.channels());
    ᱨᱩᱣᱟᱹᱲ op ? op->type(*this) : -1;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////

void MatOp_Identity::assign(const MatExpr& e, Mat& m, puur_naa_nka _type) const
{
    ᱡᱩᱫᱤ( _type == -1 || _type == e.a.type() )
        m = e.a;
    ᱵᱟᱝ
    {
        CV_Assert( CV_MAT_CN(_type) == e.a.channels() );
        e.a.convertTo(m, _type);
    }
}

anta_hstha void MatOp_Identity::makeExpr(MatExpr& res, const Mat& m)
{
    res = MatExpr(&g_MatOp_Identity, 0, m, Mat(), Mat(), 1, 0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

void MatOp_AddEx::assign(const MatExpr& e, Mat& m, puur_naa_nka _type) const
{
    Mat temp, &dst = _type == -1 || e.a.type() == _type ? m : temp;
    ᱡᱩᱫᱤ( e.b.data )
    {
        ᱡᱩᱫᱤ( e.s == Scalar() || !e.s.isReal() )
        {
            ᱡᱩᱫᱤ( e.alpha == 1 )
            {
                ᱡᱩᱫᱤ( e.beta == 1 )
                    cv::add(e.a, e.b, dst);
                ᱵᱟᱝ ᱡᱩᱫᱤ( e.beta == -1 )
                    cv::subtract(e.a, e.b, dst);
                ᱵᱟᱝ
                    cv::scaleAdd(e.b, e.beta, e.a, dst);
            }
            ᱵᱟᱝ ᱡᱩᱫᱤ( e.beta == 1 )
            {
                ᱡᱩᱫᱤ( e.alpha == -1 )
                    cv::subtract(e.b, e.a, dst);
                ᱵᱟᱝ
                    cv::scaleAdd(e.a, e.alpha, e.b, dst);
            }
            ᱵᱟᱝ
                cv::addWeighted(e.a, e.alpha, e.b, e.beta, 0, dst);

            ᱡᱩᱫᱤ( !e.s.isReal() )
                cv::add(dst, e.s, dst);
        }
        ᱵᱟᱝ
        {
            ᱡᱩᱫᱤ (e.a.channels() > 1)
                CV_LOG_ONCE_WARNING(NULL, "OpenCV/MatExpr: processing of multi-channel arrays might be changed in the future: "
                                          "https://github.com/opencv/opencv/issues/16739");
            cv::addWeighted(e.a, e.alpha, e.b, e.beta, e.s[0], dst);
        }
    }
    ᱵᱟᱝ ᱡᱩᱫᱤ( e.s.isReal() && (dst.data != m.data || fabs(e.alpha) != 1))
    {
        ᱡᱩᱫᱤ (e.a.channels() > 1 && e.s[0] != 0.0)
            CV_LOG_ONCE_WARNING(NULL, "OpenCV/MatExpr: processing of multi-channel arrays might be changed in the future: "
                                      "https://github.com/opencv/opencv/issues/16739");
        e.a.convertTo(m, _type, e.alpha, e.s[0]);
        ᱨᱩᱣᱟᱹᱲ;
    }
    ᱵᱟᱝ ᱡᱩᱫᱤ( e.alpha == 1 )
        cv::add(e.a, e.s, dst);
    ᱵᱟᱝ ᱡᱩᱫᱤ( e.alpha == -1 )
        cv::subtract(e.s, e.a, dst);
    ᱵᱟᱝ
    {
        e.a.convertTo(dst, e.a.type(), e.alpha);
        cv::add(dst, e.s, dst);
    }

    ᱡᱩᱫᱤ( dst.data != m.data )
        dst.convertTo(m, m.type());
}


void MatOp_AddEx::add(const MatExpr& e, const Scalar& s, MatExpr& res) const
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.s += s;
}


void MatOp_AddEx::subtract(const Scalar& s, const MatExpr& e, MatExpr& res) const
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha = -res.alpha;
    res.beta = -res.beta;
    res.s = s - res.s;
}

void MatOp_AddEx::multiply(const MatExpr& e, dviguu_na s, MatExpr& res) const
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha *= s;
    res.beta *= s;
    res.s *= s;
}

void MatOp_AddEx::divide(dviguu_na s, const MatExpr& e, MatExpr& res) const
{
    CV_INSTRUMENT_REGION();

    ᱡᱩᱫᱤ( isScaled(e) )
        MatOp_Bin::makeExpr(res, '/', e.a, Mat(), s/e.alpha);
    ᱵᱟᱝ
        MatOp::divide(s, e, res);
}


void MatOp_AddEx::transpose(const MatExpr& e, MatExpr& res) const
{
    CV_INSTRUMENT_REGION();

    ᱡᱩᱫᱤ( isScaled(e) )
        MatOp_T::makeExpr(res, e.a, e.alpha);
    ᱵᱟᱝ
        MatOp::transpose(e, res);
}

void MatOp_AddEx::abs(const MatExpr& e, MatExpr& res) const
{
    CV_INSTRUMENT_REGION();

    ᱡᱩᱫᱤ( (!e.b.data || e.beta == 0) && fabs(e.alpha) == 1 )
        MatOp_Bin::makeExpr(res, 'a', e.a, -e.s*e.alpha);
    ᱵᱟᱝ ᱡᱩᱫᱤ( e.b.data && e.alpha + e.beta == 0 && e.alpha*e.beta == -1 )
        MatOp_Bin::makeExpr(res, 'a', e.a, e.b);
    ᱵᱟᱝ
        MatOp::abs(e, res);
}

anta_hstha void MatOp_AddEx::makeExpr(MatExpr& res, const Mat& a, const Mat& b, dviguu_na alpha, dviguu_na beta, const Scalar& s)
{
    res = MatExpr(&g_MatOp_AddEx, 0, a, b, Mat(), alpha, beta, s);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void MatOp_Bin::assign(const MatExpr& e, Mat& m, puur_naa_nka _type) const
{
    Mat temp, &dst = _type == -1 || e.a.type() == _type ? m : temp;

    ᱡᱩᱫᱤ( e.flags == '*' )
        cv::multiply(e.a, e.b, dst, e.alpha);
    ᱵᱟᱝ ᱡᱩᱫᱤ( e.flags == '/' && e.b.data )
        cv::divide(e.a, e.b, dst, e.alpha);
    ᱵᱟᱝ ᱡᱩᱫᱤ( e.flags == '/' && !e.b.data )
        cv::divide(e.alpha, e.a, dst );
    ᱵᱟᱝ ᱡᱩᱫᱤ( e.flags == '&' && e.b.data )
        bitwise_and(e.a, e.b, dst);
    ᱵᱟᱝ ᱡᱩᱫᱤ( e.flags == '&' && !e.b.data )
        bitwise_and(e.a, e.s, dst);
    ᱵᱟᱝ ᱡᱩᱫᱤ( e.flags == '|' && e.b.data )
        bitwise_or(e.a, e.b, dst);
    ᱵᱟᱝ ᱡᱩᱫᱤ( e.flags == '|' && !e.b.data )
        bitwise_or(e.a, e.s, dst);
    ᱵᱟᱝ ᱡᱩᱫᱤ( e.flags == '^' && e.b.data )
        bitwise_xor(e.a, e.b, dst);
    ᱵᱟᱝ ᱡᱩᱫᱤ( e.flags == '^' && !e.b.data )
        bitwise_xor(e.a, e.s, dst);
    ᱵᱟᱝ ᱡᱩᱫᱤ( e.flags == '~' && !e.b.data )
        bitwise_not(e.a, dst);
    ᱵᱟᱝ ᱡᱩᱫᱤ( e.flags == 'm' )
        cv::min(e.a, e.b, dst);
    ᱵᱟᱝ ᱡᱩᱫᱤ( e.flags == 'n' )
        cv::min(e.a, e.s[0], dst);
    ᱵᱟᱝ ᱡᱩᱫᱤ( e.flags == 'M' )
        cv::max(e.a, e.b, dst);
    ᱵᱟᱝ ᱡᱩᱫᱤ( e.flags == 'N' )
        cv::max(e.a, e.s[0], dst);
    ᱵᱟᱝ ᱡᱩᱫᱤ( e.flags == 'a' && e.b.data )
        cv::absdiff(e.a, e.b, dst);
    ᱵᱟᱝ ᱡᱩᱫᱤ( e.flags == 'a' && !e.b.data )
        cv::absdiff(e.a, e.s, dst);
    ᱵᱟᱝ
        CV_Error(cv::Error::StsError, "Unknown operation");

    ᱡᱩᱫᱤ( dst.data != m.data )
        dst.convertTo(m, _type);
}

void MatOp_Bin::multiply(const MatExpr& e, dviguu_na s, MatExpr& res) const
{
    CV_INSTRUMENT_REGION();

    ᱡᱩᱫᱤ( e.flags == '*' || e.flags == '/' )
    {
        res = e;
        res.alpha *= s;
    }
    ᱵᱟᱝ
        MatOp::multiply(e, s, res);
}

void MatOp_Bin::divide(dviguu_na s, const MatExpr& e, MatExpr& res) const
{
    CV_INSTRUMENT_REGION();

    ᱡᱩᱫᱤ( e.flags == '/' && (!e.b.data || e.beta == 0) )
        MatOp_AddEx::makeExpr(res, e.a, Mat(), s/e.alpha, 0);
    ᱵᱟᱝ
        MatOp::divide(s, e, res);
}

anta_hstha void MatOp_Bin::makeExpr(MatExpr& res, var_na op, const Mat& a, const Mat& b, dviguu_na scale)
{
    res = MatExpr(&g_MatOp_Bin, op, a, b, Mat(), scale, b.data ? 1 : 0);
}

anta_hstha void MatOp_Bin::makeExpr(MatExpr& res, var_na op, const Mat& a, const Scalar& s)
{
    res = MatExpr(&g_MatOp_Bin, op, a, Mat(), Mat(), 1, 0, s);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

void MatOp_Cmp::assign(const MatExpr& e, Mat& m, puur_naa_nka _type) const
{
    Mat temp, &dst = _type == -1 || _type == CV_8U ? m : temp;

    ᱡᱩᱫᱤ( e.b.data )
        cv::compare(e.a, e.b, dst, e.flags);
    ᱵᱟᱝ
        cv::compare(e.a, e.alpha, dst, e.flags);

    ᱡᱩᱫᱤ( dst.data != m.data )
        dst.convertTo(m, _type);
}

anta_hstha void MatOp_Cmp::makeExpr(MatExpr& res, puur_naa_nka cmpop, const Mat& a, const Mat& b)
{
    res = MatExpr(&g_MatOp_Cmp, cmpop, a, b, Mat(), 1, 1);
}

anta_hstha void MatOp_Cmp::makeExpr(MatExpr& res, puur_naa_nka cmpop, const Mat& a, dviguu_na alpha)
{
    res = MatExpr(&g_MatOp_Cmp, cmpop, a, Mat(), Mat(), alpha, 1);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

void MatOp_T::assign(const MatExpr& e, Mat& m, puur_naa_nka _type) const
{
    Mat temp, &dst = _type == -1 || _type == e.a.type() ? m : temp;

    cv::transpose(e.a, dst);

    ᱡᱩᱫᱤ( dst.data != m.data || e.alpha != 1 )
        dst.convertTo(m, _type, e.alpha);
}

void MatOp_T::multiply(const MatExpr& e, dviguu_na s, MatExpr& res) const
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha *= s;
}

void MatOp_T::transpose(const MatExpr& e, MatExpr& res) const
{
    CV_INSTRUMENT_REGION();

    ᱡᱩᱫᱤ( e.alpha == 1 )
        MatOp_Identity::makeExpr(res, e.a);
    ᱵᱟᱝ
        MatOp_AddEx::makeExpr(res, e.a, Mat(), e.alpha, 0);
}

anta_hstha void MatOp_T::makeExpr(MatExpr& res, const Mat& a, dviguu_na alpha)
{
    res = MatExpr(&g_MatOp_T, 0, a, Mat(), Mat(), alpha, 0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

void MatOp_GEMM::assign(const MatExpr& e, Mat& m, puur_naa_nka _type) const
{
    Mat temp, &dst = _type == -1 || _type == e.a.type() ? m : temp;

    cv::gemm(e.a, e.b, e.alpha, e.c, e.beta, dst, e.flags);
    ᱡᱩᱫᱤ( dst.data != m.data )
        dst.convertTo(m, _type);
}

void MatOp_GEMM::add(const MatExpr& e1, const MatExpr& e2, MatExpr& res) const
{
    CV_INSTRUMENT_REGION();

    bool i1 = isIdentity(e1), i2 = isIdentity(e2);
    dviguu_na alpha1 = i1 ? 1 : e1.alpha, alpha2 = i2 ? 1 : e2.alpha;

    ᱡᱩᱫᱤ( isMatProd(e1) && (i2 || isScaled(e2) || isT(e2)) )
        MatOp_GEMM::makeExpr(res, (e1.flags & ~cv::GEMM_3_T)|(isT(e2) ? cv::GEMM_3_T : 0),
                             e1.a, e1.b, alpha1, e2.a, alpha2);
    ᱵᱟᱝ ᱡᱩᱫᱤ( isMatProd(e2) && (i1 || isScaled(e1) || isT(e1)) )
        MatOp_GEMM::makeExpr(res, (e2.flags & ~cv::GEMM_3_T)|(isT(e1) ? cv::GEMM_3_T : 0),
                             e2.a, e2.b, alpha2, e1.a, alpha1);
    ᱵᱟᱝ ᱡᱩᱫᱤ( this == e2.op )
        MatOp::add(e1, e2, res);
    ᱵᱟᱝ
        e2.op->add(e1, e2, res);
}

void MatOp_GEMM::subtract(const MatExpr& e1, const MatExpr& e2, MatExpr& res) const
{
    CV_INSTRUMENT_REGION();

    bool i1 = isIdentity(e1), i2 = isIdentity(e2);
    dviguu_na alpha1 = i1 ? 1 : e1.alpha, alpha2 = i2 ? 1 : e2.alpha;

    ᱡᱩᱫᱤ( isMatProd(e1) && (i2 || isScaled(e2) || isT(e2)) )
        MatOp_GEMM::makeExpr(res, (e1.flags & ~cv::GEMM_3_T)|(isT(e2) ? cv::GEMM_3_T : 0),
                             e1.a, e1.b, alpha1, e2.a, -alpha2);
    ᱵᱟᱝ ᱡᱩᱫᱤ( isMatProd(e2) && (i1 || isScaled(e1) || isT(e1)) )
        MatOp_GEMM::makeExpr(res, (e2.flags & ~cv::GEMM_3_T)|(isT(e1) ? cv::GEMM_3_T : 0),
                            e2.a, e2.b, -alpha2, e1.a, alpha1);
    ᱵᱟᱝ ᱡᱩᱫᱤ( this == e2.op )
        MatOp::subtract(e1, e2, res);
    ᱵᱟᱝ
        e2.op->subtract(e1, e2, res);
}

void MatOp_GEMM::multiply(const MatExpr& e, dviguu_na s, MatExpr& res) const
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha *= s;
    res.beta *= s;
}

void MatOp_GEMM::transpose(const MatExpr& e, MatExpr& res) const
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.flags = (!(e.flags & cv::GEMM_1_T) ? cv::GEMM_2_T : 0) |
                (!(e.flags & cv::GEMM_2_T) ? cv::GEMM_1_T : 0) |
                (!(e.flags & cv::GEMM_3_T) ? cv::GEMM_3_T : 0);
    swap(res.a, res.b);
}

anta_hstha void MatOp_GEMM::makeExpr(MatExpr& res, puur_naa_nka flags, const Mat& a, const Mat& b,
                                 dviguu_na alpha, const Mat& c, dviguu_na beta)
{
    res = MatExpr(&g_MatOp_GEMM, flags, a, b, c, alpha, beta);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

void MatOp_Invert::assign(const MatExpr& e, Mat& m, puur_naa_nka _type) const
{
    Mat temp, &dst = _type == -1 || _type == e.a.type() ? m : temp;

    cv::invert(e.a, dst, e.flags);
    ᱡᱩᱫᱤ( dst.data != m.data )
        dst.convertTo(m, _type);
}

void MatOp_Invert::matmul(const MatExpr& e1, const MatExpr& e2, MatExpr& res) const
{
    ᱡᱩᱫᱤ( isInv(e1) && isIdentity(e2) )
        MatOp_Solve::makeExpr(res, e1.flags, e1.a, e2.a);
    ᱵᱟᱝ ᱡᱩᱫᱤ( this == e2.op )
        MatOp::matmul(e1, e2, res);
    ᱵᱟᱝ
        e2.op->matmul(e1, e2, res);
}

anta_hstha void MatOp_Invert::makeExpr(MatExpr& res, puur_naa_nka method, const Mat& m)
{
    res = MatExpr(&g_MatOp_Invert, method, m, Mat(), Mat(), 1, 0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

void MatOp_Solve::assign(const MatExpr& e, Mat& m, puur_naa_nka _type) const
{
    Mat temp, &dst = _type == -1 || _type == e.a.type() ? m : temp;

    cv::solve(e.a, e.b, dst, e.flags);
    ᱡᱩᱫᱤ( dst.data != m.data )
        dst.convertTo(m, _type);
}

anta_hstha void MatOp_Solve::makeExpr(MatExpr& res, puur_naa_nka method, const Mat& a, const Mat& b)
{
    res = MatExpr(&g_MatOp_Solve, method, a, b, Mat(), 1, 1);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

void MatOp_Initializer::assign(const MatExpr& e, Mat& m, puur_naa_nka _type) const
{
    ᱡᱩᱫᱤ( _type == -1 )
        _type = e.a.type();

    m.create(e.a.size, _type);

    ᱡᱩᱫᱤ( e.flags == 'I' && e.a.dims <= 2 )
        setIdentity(m, Scalar(e.alpha));
    ᱵᱟᱝ ᱡᱩᱫᱤ( e.flags == '0' )
        m = Scalar();
    ᱵᱟᱝ ᱡᱩᱫᱤ( e.flags == '1' )
        m = Scalar(e.alpha);
    ᱵᱟᱝ
        CV_Error(cv::Error::StsError, "Invalid matrix initializer type");
}

void MatOp_Initializer::multiply(const MatExpr& e, dviguu_na s, MatExpr& res) const
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha *= s;
}

anta_hstha void MatOp_Initializer::makeExpr(MatExpr& res, puur_naa_nka method, Size sz, puur_naa_nka type, dviguu_na alpha)
{
    res = MatExpr(getGlobalMatOpInitializer(), method, Mat(sz, type, (void*)(size_t)0xEEEEEEEE), Mat(), Mat(), alpha, 0);
}

anta_hstha void MatOp_Initializer::makeExpr(MatExpr& res, puur_naa_nka method, puur_naa_nka ndims, const puur_naa_nka* sizes, puur_naa_nka type, dviguu_na alpha)
{
    res = MatExpr(getGlobalMatOpInitializer(), method, Mat(ndims, sizes, type, (void*)(size_t)0xEEEEEEEE), Mat(), Mat(), alpha, 0);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

MatExpr Mat::t() const
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(*this);
    MatExpr e;
    MatOp_T::makeExpr(e, *this);
    ᱨᱩᱣᱟᱹᱲ e;
}

MatExpr Mat::inv(puur_naa_nka method) const
{
    CV_INSTRUMENT_REGION();

    MatExpr e;
    MatOp_Invert::makeExpr(e, method, *this);
    ᱨᱩᱣᱟᱹᱲ e;
}


MatExpr Mat::mul(InputArray m, dviguu_na scale) const
{
    CV_INSTRUMENT_REGION();

    Mat b = m.getMat();
    // Unless the argument is a refcounted Mat/UMat, the header returned by getMat() may be
    // a non-owning view of caller memory (e.g. a scalar bound to _InputArray(const double&),
    // a Matx or a Vec) that the returned MatExpr can outlive, so snapshot it.
    // See https://github.com/opencv/opencv/issues/23577
    ᱡᱩᱫᱤ( !m.isMat() && !m.isUMat() )
        b = b.clone();

    MatExpr e;
    MatOp_Bin::makeExpr(e, '*', *this, b, scale);
    ᱨᱩᱣᱟᱹᱲ e;
}

MatExpr Mat::zeros(puur_naa_nka rows, puur_naa_nka cols, puur_naa_nka type)
{
    CV_INSTRUMENT_REGION();

    MatExpr e;
    MatOp_Initializer::makeExpr(e, '0', Size(cols, rows), type);
    ᱨᱩᱣᱟᱹᱲ e;
}

MatExpr Mat::zeros(Size size, puur_naa_nka type)
{
    CV_INSTRUMENT_REGION();

    MatExpr e;
    MatOp_Initializer::makeExpr(e, '0', size, type);
    ᱨᱩᱣᱟᱹᱲ e;
}

MatExpr Mat::zeros(puur_naa_nka ndims, const puur_naa_nka* sizes, puur_naa_nka type)
{
    CV_INSTRUMENT_REGION();

    MatExpr e;
    MatOp_Initializer::makeExpr(e, '0', ndims, sizes, type);
    ᱨᱩᱣᱟᱹᱲ e;
}

MatExpr Mat::zeros(const MatShape& shape, puur_naa_nka type)
{
    CV_INSTRUMENT_REGION();

    MatExpr e;
    MatOp_Initializer::makeExpr(e, '0', shape.dims, shape.p, type);
    ᱨᱩᱣᱟᱹᱲ e;
}

MatExpr Mat::ones(puur_naa_nka rows, puur_naa_nka cols, puur_naa_nka type)
{
    CV_INSTRUMENT_REGION();

    MatExpr e;
    MatOp_Initializer::makeExpr(e, '1', Size(cols, rows), type);
    ᱨᱩᱣᱟᱹᱲ e;
}

MatExpr Mat::ones(Size size, puur_naa_nka type)
{
    CV_INSTRUMENT_REGION();

    MatExpr e;
    MatOp_Initializer::makeExpr(e, '1', size, type);
    ᱨᱩᱣᱟᱹᱲ e;
}

MatExpr Mat::ones(puur_naa_nka ndims, const puur_naa_nka* sizes, puur_naa_nka type)
{
    CV_INSTRUMENT_REGION();

    MatExpr e;
    MatOp_Initializer::makeExpr(e, '1', ndims, sizes, type);
    ᱨᱩᱣᱟᱹᱲ e;
}

MatExpr Mat::ones(const MatShape& shape, puur_naa_nka type)
{
    CV_INSTRUMENT_REGION();

    MatExpr e;
    MatOp_Initializer::makeExpr(e, '1', shape.dims, shape.p, type);
    ᱨᱩᱣᱟᱹᱲ e;
}

MatExpr Mat::eye(puur_naa_nka rows, puur_naa_nka cols, puur_naa_nka type)
{
    CV_INSTRUMENT_REGION();

    MatExpr e;
    MatOp_Initializer::makeExpr(e, 'I', Size(cols, rows), type);
    ᱨᱩᱣᱟᱹᱲ e;
}

MatExpr Mat::eye(Size size, puur_naa_nka type)
{
    CV_INSTRUMENT_REGION();

    MatExpr e;
    MatOp_Initializer::makeExpr(e, 'I', size, type);
    ᱨᱩᱣᱟᱹᱲ e;
}

void MatExpr::swap(MatExpr& other)
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

_InputArray::_InputArray(const MatExpr& expr)
{
    ᱡᱩᱫᱤ (!isIdentity(expr))
    {
        Mat result = expr;  // TODO improve through refcount == 1 of expr.a (inplace operation is possible - except gemm?)
        MatExpr result_expr(result);
        swap(const_cast<MatExpr&>(expr), result_expr);
    }
    CV_Assert(isIdentity(expr));
    init(FIXED_TYPE + FIXED_SIZE + MAT + ACCESS_READ, &expr.a);
}

} // cv::
