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
    if (a.empty())
    {
        CV_Error(cv::Error::StsBadArg, "Matrix operand is an empty matrix.");
    }
}

static void checkOperandsExist(const Mat& a, const Mat& b)
{
    if (a.empty() || b.empty())
    {
        CV_Error(cv::Error::StsBadArg, "One or more matrix operands are empty.");
    }
}


class MatOp_Identity CV_FINAL : public MatOp
{
public:
    MatOp_Identity() {}
    virtual ~MatOp_Identity() {}

    bool elementWise(const Mat_पाहायाग& /*पाहायाग*/) const CV_OVERRIDE { return true; }
    void assign(const Mat_पाहायाग& पाहायाग, Mat& m, int type=-1) const CV_OVERRIDE;

    static void make_पाहायाग(Mat_पाहायाग& res, const Mat& m);
};

static MatOp_Identity g_MatOp_Identity;

class MatOp_AddEx CV_FINAL : public MatOp
{
public:
    MatOp_AddEx() {}
    virtual ~MatOp_AddEx() {}

    bool elementWise(const Mat_पाहायाग& /*पाहायाग*/) const CV_OVERRIDE { return true; }
    void assign(const Mat_पाहायाग& पाहायाग, Mat& m, int type=-1) const CV_OVERRIDE;

    void add(const Mat_पाहायाग& e1, const Scalar& s, Mat_पाहायाग& res) const CV_OVERRIDE;
    void subtract(const Scalar& s, const Mat_पाहायाग& पाहायाग, Mat_पाहायाग& res) const CV_OVERRIDE;
    void multiply(const Mat_पाहायाग& e1, double s, Mat_पाहायाग& res) const CV_OVERRIDE;
    void divide(double s, const Mat_पाहायाग& e, Mat_पाहायाग& res) const CV_OVERRIDE;

    void transpose(const Mat_पाहायाग& e1, Mat_पाहायाग& res) const CV_OVERRIDE;
    void abs(const Mat_पाहायाग& पाहायाग, Mat_पाहायाग& res) const CV_OVERRIDE;

    static void make_पाहायाग(Mat_पाहायाग& res, const Mat& a, const Mat& b, double alpha, double beta, const Scalar& s=Scalar());
};

static MatOp_AddEx g_MatOp_AddEx;

class MatOp_Bin CV_FINAL : public MatOp
{
public:
    MatOp_Bin() {}
    virtual ~MatOp_Bin() {}

    bool elementWise(const Mat_पाहायाग& /*पाहायाग*/) const CV_OVERRIDE { return true; }
    void assign(const Mat_पाहायाग& पाहायाग, Mat& m, int type=-1) const CV_OVERRIDE;

    void multiply(const Mat_पाहायाग& e1, double s, Mat_पाहायाग& res) const CV_OVERRIDE;
    void divide(double s, const Mat_पाहायाग& e, Mat_पाहायाग& res) const CV_OVERRIDE;

    static void make_पाहायाग(Mat_पाहायाग& res, char op, const Mat& a, const Mat& b, double scale=1);
    static void make_पाहायाग(Mat_पाहायाग& res, char op, const Mat& a, const Scalar& s);
};

static MatOp_Bin g_MatOp_Bin;

class MatOp_Cmp CV_FINAL : public MatOp
{
public:
    MatOp_Cmp() {}
    virtual ~MatOp_Cmp() {}

    bool elementWise(const Mat_पाहायाग& /*पाहायाग*/) const CV_OVERRIDE { return true; }
    void assign(const Mat_पाहायाग& पाहायाग, Mat& m, int type=-1) const CV_OVERRIDE;

    static void make_पाहायाग(Mat_पाहायाग& res, int cmpop, const Mat& a, const Mat& b);
    static void make_पाहायाग(Mat_पाहायाग& res, int cmpop, const Mat& a, double alpha);
};

static MatOp_Cmp g_MatOp_Cmp;

class MatOp_GEMM CV_FINAL : public MatOp
{
public:
    MatOp_GEMM() {}
    virtual ~MatOp_GEMM() {}

    bool elementWise(const Mat_पाहायाग& /*पाहायाग*/) const CV_OVERRIDE { return false; }
    void assign(const Mat_पाहायाग& पाहायाग, Mat& m, int type=-1) const CV_OVERRIDE;

    void add(const Mat_पाहायाग& e1, const Mat_पाहायाग& e2, Mat_पाहायाग& res) const CV_OVERRIDE;
    void subtract(const Mat_पाहायाग& e1, const Mat_पाहायाग& e2, Mat_पाहायाग& res) const CV_OVERRIDE;
    void multiply(const Mat_पाहायाग& e, double s, Mat_पाहायाग& res) const CV_OVERRIDE;

    void transpose(const Mat_पाहायाग& पाहायाग, Mat_पाहायाग& res) const CV_OVERRIDE;

    Size size(const Mat_पाहायाग& पाहायाग) const CV_OVERRIDE
    {
        return Size(
            (पाहायाग.flags & GEMM_2_T) ? पाहायाग.b.rows : पाहायाग.b.cols,
            (पाहायाग.flags & GEMM_1_T) ? पाहायाग.a.cols : पाहायाग.a.rows
        );
    }

    static void make_पाहायाग(Mat_पाहायाग& res, int flags, const Mat& a, const Mat& b,
                         double alpha=1, const Mat& c=Mat(), double beta=1);

};

static MatOp_GEMM g_MatOp_GEMM;

class MatOp_Invert CV_FINAL : public MatOp
{
public:
    MatOp_Invert() {}
    virtual ~MatOp_Invert() {}

    bool elementWise(const Mat_पाहायाग& /*पाहायाग*/) const CV_OVERRIDE { return false; }
    void assign(const Mat_पाहायाग& पाहायाग, Mat& m, int type=-1) const CV_OVERRIDE;

    void matmul(const Mat_पाहायाग& expr1, const Mat_पाहायाग& expr2, Mat_पाहायाग& res) const CV_OVERRIDE;

    static void make_पाहायाग(Mat_पाहायाग& res, int method, const Mat& m);
};

static MatOp_Invert g_MatOp_Invert;

class MatOp_T CV_FINAL : public MatOp
{
public:
    MatOp_T() {}
    virtual ~MatOp_T() {}

    bool elementWise(const Mat_पाहायाग& /*पाहायाग*/) const CV_OVERRIDE { return false; }
    void assign(const Mat_पाहायाग& पाहायाग, Mat& m, int type=-1) const CV_OVERRIDE;

    void multiply(const Mat_पाहायाग& e1, double s, Mat_पाहायाग& res) const CV_OVERRIDE;
    void transpose(const Mat_पाहायाग& पाहायाग, Mat_पाहायाग& res) const CV_OVERRIDE;

    static void make_पाहायाग(Mat_पाहायाग& res, const Mat& a, double alpha=1);
};

static MatOp_T g_MatOp_T;

class MatOp_Solve CV_FINAL : public MatOp
{
public:
    MatOp_Solve() {}
    virtual ~MatOp_Solve() {}

    bool elementWise(const Mat_पाहायाग& /*पाहायाग*/) const CV_OVERRIDE { return false; }
    void assign(const Mat_पाहायाग& पाहायाग, Mat& m, int type=-1) const CV_OVERRIDE;

    static void make_पाहायाग(Mat_पाहायाग& res, int method, const Mat& a, const Mat& b);
};

static MatOp_Solve g_MatOp_Solve;

class MatOp_Initializer CV_FINAL : public MatOp
{
public:
    MatOp_Initializer() {}
    virtual ~MatOp_Initializer() {}

    bool elementWise(const Mat_पाहायाग& /*पाहायाग*/) const CV_OVERRIDE { return false; }
    void assign(const Mat_पाहायाग& पाहायाग, Mat& m, int type=-1) const CV_OVERRIDE;

    void multiply(const Mat_पाहायाग& e, double s, Mat_पाहायाग& res) const CV_OVERRIDE;

    static void make_पाहायाग(Mat_पाहायाग& res, int method, Size sz, int type, double alpha=1);
    static void make_पाहायाग(Mat_पाहायाग& res, int method, int ndims, const int* sizes, int type, double alpha=1);
};

static MatOp_Initializer* getGlobalMatOpInitializer()
{
    CV_SINGLETON_LAZY_INIT(MatOp_Initializer, new MatOp_Initializer())
}

static inline bool isIdentity(const Mat_पाहायाग& e) { return e.op == &g_MatOp_Identity; }
static inline bool isAddEx(const Mat_पाहायाग& e) { return e.op == &g_MatOp_AddEx; }
static inline bool isScaled(const Mat_पाहायाग& e) { return isAddEx(e) && (!e.b.data || e.beta == 0) && e.s == Scalar(); }
static inline bool isBin(const Mat_पाहायाग& e, char c) { return e.op == &g_MatOp_Bin && e.flags == c; }
static inline bool isCmp(const Mat_पाहायाग& e) { return e.op == &g_MatOp_Cmp; }
static inline bool isReciprocal(const Mat_पाहायाग& e) { return isBin(e,'/') && (!e.b.data || e.beta == 0); }
static inline bool isT(const Mat_पाहायाग& e) { return e.op == &g_MatOp_T; }
static inline bool isInv(const Mat_पाहायाग& e) { return e.op == &g_MatOp_Invert; }
static inline bool isSolve(const Mat_पाहायाग& e) { return e.op == &g_MatOp_Solve; }
//static inline bool isGEMM(const MatExpr& e) { return e.op == &g_MatOp_GEMM; }
static inline bool isMatProd(const Mat_पाहायाग& e) { return e.op == &g_MatOp_GEMM && (!e.c.data || e.beta == 0); }
static inline bool isInitializer(const Mat_पाहायाग& e) { return e.op == getGlobalMatOpInitializer(); }

/////////////////////////////////////////////////////////////////////////////////////////////////////

MatOp::MatOp() {}
MatOp::~MatOp() {}


bool MatOp::elementWise(const Mat_पाहायाग& /*पाहायाग*/) const
{
    return false;
}

void MatOp::roi(const Mat_पाहायाग& पाहायाग, const Range& rowRange, const Range& colRange, Mat_पाहायाग& e) const
{
    if( elementWise(पाहायाग) )
    {
        e = Mat_पाहायाग(पाहायाग.op, पाहायाग.flags, Mat(), Mat(), Mat(),
                    पाहायाग.alpha, पाहायाग.beta, पाहायाग.s);
        if(पाहायाग.a.data)
            e.a = पाहायाग.a(rowRange, colRange);
        if(पाहायाग.b.data)
            e.b = पाहायाग.b(rowRange, colRange);
        if(पाहायाग.c.data)
            e.c = पाहायाग.c(rowRange, colRange);
    }
    else
    {
        Mat m;
        पाहायाग.op->assign(पाहायाग, m);
        e = Mat_पाहायाग(&g_MatOp_Identity, 0, m(rowRange, colRange), Mat(), Mat());
    }
}

void MatOp::diag(const Mat_पाहायाग& पाहायाग, int d, Mat_पाहायाग& e) const
{
    if( elementWise(पाहायाग) )
    {
        e = Mat_पाहायाग(पाहायाग.op, पाहायाग.flags, Mat(), Mat(), Mat(),
                    पाहायाग.alpha, पाहायाग.beta, पाहायाग.s);
        if(पाहायाग.a.data)
            e.a = पाहायाग.a.diag(d);
        if(पाहायाग.b.data)
            e.b = पाहायाग.b.diag(d);
        if(पाहायाग.c.data)
            e.c = पाहायाग.c.diag(d);
    }
    else
    {
        Mat m;
        पाहायाग.op->assign(पाहायाग, m);
        e = Mat_पाहायाग(&g_MatOp_Identity, 0, m.diag(d), Mat(), Mat());
    }
}


void MatOp::augAssignAdd(const Mat_पाहायाग& पाहायाग, Mat& m) const
{
    Mat temp;
    पाहायाग.op->assign(पाहायाग, temp);
    m += temp;
}


void MatOp::augAssignSubtract(const Mat_पाहायाग& पाहायाग, Mat& m) const
{
    Mat temp;
    पाहायाग.op->assign(पाहायाग, temp);
    m -= temp;
}


void MatOp::augAssignMultiply(const Mat_पाहायाग& पाहायाग, Mat& m) const
{
    Mat temp;
    पाहायाग.op->assign(पाहायाग, temp);
    m *= temp;
}


void MatOp::augAssignDivide(const Mat_पाहायाग& पाहायाग, Mat& m) const
{
    Mat temp;
    पाहायाग.op->assign(पाहायाग, temp);
    m /= temp;
}


void MatOp::augAssignAnd(const Mat_पाहायाग& पाहायाग, Mat& m) const
{
    Mat temp;
    पाहायाग.op->assign(पाहायाग, temp);
    m &= temp;
}


void MatOp::augAssignOr(const Mat_पाहायाग& पाहायाग, Mat& m) const
{
    Mat temp;
    पाहायाग.op->assign(पाहायाग, temp);
    m |= temp;
}


void MatOp::augAssignXor(const Mat_पाहायाग& पाहायाग, Mat& m) const
{
    Mat temp;
    पाहायाग.op->assign(पाहायाग, temp);
    m ^= temp;
}


void MatOp::add(const Mat_पाहायाग& e1, const Mat_पाहायाग& e2, Mat_पाहायाग& res) const
{
    CV_INSTRUMENT_REGION();

    if( this == e2.op )
    {
        double alpha = 1, beta = 1;
        Scalar s;
        Mat m1, m2;
        if( isAddEx(e1) && (!e1.b.data || e1.beta == 0) )
        {
            m1 = e1.a;
            alpha = e1.alpha;
            s = e1.s;
        }
        else
            e1.op->assign(e1, m1);

        if( isAddEx(e2) && (!e2.b.data || e2.beta == 0) )
        {
            m2 = e2.a;
            beta = e2.alpha;
            s += e2.s;
        }
        else
            e2.op->assign(e2, m2);
        MatOp_AddEx::make_पाहायाग(res, m1, m2, alpha, beta, s);
    }
    else
        e2.op->add(e1, e2, res);
}


void MatOp::add(const Mat_पाहायाग& expr1, const Scalar& s, Mat_पाहायाग& res) const
{
    CV_INSTRUMENT_REGION();

    Mat m1;
    expr1.op->assign(expr1, m1);
    MatOp_AddEx::make_पाहायाग(res, m1, Mat(), 1, 0, s);
}


void MatOp::subtract(const Mat_पाहायाग& e1, const Mat_पाहायाग& e2, Mat_पाहायाग& res) const
{
    CV_INSTRUMENT_REGION();

    if( this == e2.op )
    {
        double alpha = 1, beta = -1;
        Scalar s;
        Mat m1, m2;
        if( isAddEx(e1) && (!e1.b.data || e1.beta == 0) )
        {
            m1 = e1.a;
            alpha = e1.alpha;
            s = e1.s;
        }
        else
            e1.op->assign(e1, m1);

        if( isAddEx(e2) && (!e2.b.data || e2.beta == 0) )
        {
            m2 = e2.a;
            beta = -e2.alpha;
            s -= e2.s;
        }
        else
            e2.op->assign(e2, m2);
        MatOp_AddEx::make_पाहायाग(res, m1, m2, alpha, beta, s);
    }
    else
        e2.op->subtract(e1, e2, res);
}


void MatOp::subtract(const Scalar& s, const Mat_पाहायाग& पाहायाग, Mat_पाहायाग& res) const
{
    CV_INSTRUMENT_REGION();

    Mat m;
    पाहायाग.op->assign(पाहायाग, m);
    MatOp_AddEx::make_पाहायाग(res, m, Mat(), -1, 0, s);
}


void MatOp::multiply(const Mat_पाहायाग& e1, const Mat_पाहायाग& e2, Mat_पाहायाग& res, double scale) const
{
    CV_INSTRUMENT_REGION();

    if( this == e2.op )
    {
        Mat m1, m2;

        if( isReciprocal(e1) )
        {
            if( isScaled(e2) )
            {
                scale *= e2.alpha;
                m2 = e2.a;
            }
            else
                e2.op->assign(e2, m2);

            MatOp_Bin::make_पाहायाग(res, '/', m2, e1.a, scale/e1.alpha);
        }
        else
        {
            char op = '*';
            if( isScaled(e1) )
            {
                m1 = e1.a;
                scale *= e1.alpha;
            }
            else
                e1.op->assign(e1, m1);

            if( isScaled(e2) )
            {
                m2 = e2.a;
                scale *= e2.alpha;
            }
            else if( isReciprocal(e2) )
            {
                op = '/';
                m2 = e2.a;
                scale *= e2.alpha;
            }
            else
                e2.op->assign(e2, m2);

            MatOp_Bin::make_पाहायाग(res, op, m1, m2, scale);
        }
    }
    else
        e2.op->multiply(e1, e2, res, scale);
}


void MatOp::multiply(const Mat_पाहायाग& पाहायाग, double s, Mat_पाहायाग& res) const
{
    CV_INSTRUMENT_REGION();

    Mat m;
    पाहायाग.op->assign(पाहायाग, m);
    MatOp_AddEx::make_पाहायाग(res, m, Mat(), s, 0);
}


void MatOp::divide(const Mat_पाहायाग& e1, const Mat_पाहायाग& e2, Mat_पाहायाग& res, double scale) const
{
    CV_INSTRUMENT_REGION();

    if( this == e2.op )
    {
        if( isReciprocal(e1) && isReciprocal(e2) )
            MatOp_Bin::make_पाहायाग(res, '/', e2.a, e1.a, e1.alpha/e2.alpha);
        else
        {
            Mat m1, m2;
            char op = '/';

            if( isScaled(e1) )
            {
                m1 = e1.a;
                scale *= e1.alpha;
            }
            else
                e1.op->assign(e1, m1);

            if( isScaled(e2) )
            {
                m2 = e2.a;
                scale /= e2.alpha;
            }
            else if( isReciprocal(e2) )
            {
                m2 = e2.a;
                scale /= e2.alpha;
                op = '*';
            }
            else
                e2.op->assign(e2, m2);
            MatOp_Bin::make_पाहायाग(res, op, m1, m2, scale);
        }
    }
    else
        e2.op->divide(e1, e2, res, scale);
}


void MatOp::divide(double s, const Mat_पाहायाग& पाहायाग, Mat_पाहायाग& res) const
{
    CV_INSTRUMENT_REGION();

    Mat m;
    पाहायाग.op->assign(पाहायाग, m);
    MatOp_Bin::make_पाहायाग(res, '/', m, Mat(), s);
}


void MatOp::abs(const Mat_पाहायाग& पाहायाग, Mat_पाहायाग& res) const
{
    CV_INSTRUMENT_REGION();

    Mat m;
    पाहायाग.op->assign(पाहायाग, m);
    MatOp_Bin::make_पाहायाग(res, 'a', m, Mat());
}


void MatOp::transpose(const Mat_पाहायाग& पाहायाग, Mat_पाहायाग& res) const
{
    CV_INSTRUMENT_REGION();

    Mat m;
    पाहायाग.op->assign(पाहायाग, m);
    MatOp_T::make_पाहायाग(res, m, 1);
}


void MatOp::matmul(const Mat_पाहायाग& e1, const Mat_पाहायाग& e2, Mat_पाहायाग& res) const
{
    if( this == e2.op )
    {
        double scale = 1;
        int flags = 0;
        Mat m1, m2;

        if( isT(e1) )
        {
            flags = cv::GEMM_1_T;
            scale = e1.alpha;
            m1 = e1.a;
        }
        else if( isScaled(e1) )
        {
            scale = e1.alpha;
            m1 = e1.a;
        }
        else
            e1.op->assign(e1, m1);

        if( isT(e2) )
        {
            flags |= cv::GEMM_2_T;
            scale *= e2.alpha;
            m2 = e2.a;
        }
        else if( isScaled(e2) )
        {
            scale *= e2.alpha;
            m2 = e2.a;
        }
        else
            e2.op->assign(e2, m2);

        MatOp_GEMM::make_पाहायाग(res, flags, m1, m2, scale);
    }
    else
        e2.op->matmul(e1, e2, res);
}


void MatOp::invert(const Mat_पाहायाग& पाहायाग, int method, Mat_पाहायाग& res) const
{
    Mat m;
    पाहायाग.op->assign(पाहायाग, m);
    MatOp_Invert::make_पाहायाग(res, method, m);
}


Size MatOp::size(const Mat_पाहायाग& पाहायाग) const
{
    return !पाहायाग.a.empty() ? पाहायाग.a.size() : पाहायाग.b.empty() ? पाहायाग.b.size() : पाहायाग.c.size();
}

int MatOp::type(const Mat_पाहायाग& पाहायाग) const
{
    CV_INSTRUMENT_REGION();

    return !पाहायाग.a.empty() ? पाहायाग.a.type() : पाहायाग.b.empty() ? पाहायाग.b.type() : पाहायाग.c.type();
}

//////////////////////////////////////////////////////////////////////////////////////////////////

Mat_पाहायाग::Mat_पाहायाग(const Mat& m) : op(&g_MatOp_Identity), flags(0), a(m), b(Mat()), c(Mat()), alpha(1), beta(0), s(Scalar())
{
}

Mat_पाहायाग Mat_पाहायाग::row(int y) const
{
    Mat_पाहायाग e;
    op->roi(*this, Range(y, y+1), Range::all(), e);
    return e;
}

Mat_पाहायाग Mat_पाहायाग::col(int x) const
{
    Mat_पाहायाग e;
    op->roi(*this, Range::all(), Range(x, x+1), e);
    return e;
}

Mat_पाहायाग Mat_पाहायाग::diag(int d) const
{
    Mat_पाहायाग e;
    op->diag(*this, d, e);
    return e;
}

Mat_पाहायाग Mat_पाहायाग::operator()( const Range& rowRange, const Range& colRange ) const
{
    Mat_पाहायाग e;
    op->roi(*this, rowRange, colRange, e);
    return e;
}

Mat_पाहायाग Mat_पाहायाग::operator()( const Rect& roi ) const
{
    Mat_पाहायाग e;
    op->roi(*this, Range(roi.y, roi.y + roi.height), Range(roi.x, roi.x + roi.width), e);
    return e;
}

Mat Mat_पाहायाग::cross(const Mat& m) const
{
    return ((Mat)*this).cross(m);
}

double Mat_पाहायाग::dot(const Mat& m) const
{
    return ((Mat)*this).dot(m);
}

Mat_पाहायाग Mat_पाहायाग::t() const
{
    Mat_पाहायाग e;
    op->transpose(*this, e);
    return e;
}

Mat_पाहायाग Mat_पाहायाग::inv(int method) const
{
    Mat_पाहायाग e;
    op->invert(*this, method, e);
    return e;
}

Mat_पाहायाग Mat_पाहायाग::mul(const Mat_पाहायाग& e, double scale) const
{
    Mat_पाहायाग en;
    op->multiply(*this, e, en, scale);
    return en;
}

Mat_पाहायाग Mat_पाहायाग::mul(const Mat& m, double scale) const
{
    Mat_पाहायाग e;
    op->multiply(*this, Mat_पाहायाग(m), e, scale);
    return e;
}

Mat_पाहायाग operator + (const Mat& a, const Mat& b)
{
    checkOperandsExist(a, b);
    Mat_पाहायाग e;
    MatOp_AddEx::make_पाहायाग(e, a, b, 1, 1);
    return e;
}

Mat_पाहायाग operator + (const Mat& a, const Scalar& s)
{
    checkOperandsExist(a);
    Mat_पाहायाग e;
    MatOp_AddEx::make_पाहायाग(e, a, Mat(), 1, 0, s);
    return e;
}

Mat_पाहायाग operator + (const Scalar& s, const Mat& a)
{
    checkOperandsExist(a);
    Mat_पाहायाग e;
    MatOp_AddEx::make_पाहायाग(e, a, Mat(), 1, 0, s);
    return e;
}

Mat_पाहायाग operator + (const Mat_पाहायाग& e, const Mat& m)
{
    Mat_पाहायाग en;
    e.op->add(e, Mat_पाहायाग(m), en);
    return en;
}

Mat_पाहायाग operator + (const Mat& m, const Mat_पाहायाग& e)
{
    checkOperandsExist(m);
    Mat_पाहायाग en;
    e.op->add(e, Mat_पाहायाग(m), en);
    return en;
}

Mat_पाहायाग operator + (const Mat_पाहायाग& e, const Scalar& s)
{
    Mat_पाहायाग en;
    e.op->add(e, s, en);
    return en;
}

Mat_पाहायाग operator + (const Scalar& s, const Mat_पाहायाग& e)
{
    Mat_पाहायाग en;
    e.op->add(e, s, en);
    return en;
}

Mat_पाहायाग operator + (const Mat_पाहायाग& e1, const Mat_पाहायाग& e2)
{
    Mat_पाहायाग en;
    e1.op->add(e1, e2, en);
    return en;
}

Mat_पाहायाग operator - (const Mat& a, const Mat& b)
{
    checkOperandsExist(a, b);
    Mat_पाहायाग e;
    MatOp_AddEx::make_पाहायाग(e, a, b, 1, -1);
    return e;
}

Mat_पाहायाग operator - (const Mat& a, const Scalar& s)
{
    checkOperandsExist(a);
    Mat_पाहायाग e;
    MatOp_AddEx::make_पाहायाग(e, a, Mat(), 1, 0, -s);
    return e;
}

Mat_पाहायाग operator - (const Scalar& s, const Mat& a)
{
    checkOperandsExist(a);
    Mat_पाहायाग e;
    MatOp_AddEx::make_पाहायाग(e, a, Mat(), -1, 0, s);
    return e;
}

Mat_पाहायाग operator - (const Mat_पाहायाग& e, const Mat& m)
{
    checkOperandsExist(m);
    Mat_पाहायाग en;
    e.op->subtract(e, Mat_पाहायाग(m), en);
    return en;
}

Mat_पाहायाग operator - (const Mat& m, const Mat_पाहायाग& e)
{
    checkOperandsExist(m);
    Mat_पाहायाग en;
    e.op->subtract(Mat_पाहायाग(m), e, en);
    return en;
}

Mat_पाहायाग operator - (const Mat_पाहायाग& e, const Scalar& s)
{
    Mat_पाहायाग en;
    e.op->add(e, -s, en);
    return en;
}

Mat_पाहायाग operator - (const Scalar& s, const Mat_पाहायाग& e)
{
    Mat_पाहायाग en;
    e.op->subtract(s, e, en);
    return en;
}

Mat_पाहायाग operator - (const Mat_पाहायाग& e1, const Mat_पाहायाग& e2)
{
    Mat_पाहायाग en;
    e1.op->subtract(e1, e2, en);
    return en;
}

Mat_पाहायाग operator - (const Mat& m)
{
    checkOperandsExist(m);
    Mat_पाहायाग e;
    MatOp_AddEx::make_पाहायाग(e, m, Mat(), -1, 0);
    return e;
}

Mat_पाहायाग operator - (const Mat_पाहायाग& e)
{
    Mat_पाहायाग en;
    e.op->subtract(Scalar(0), e, en);
    return en;
}

Mat_पाहायाग operator * (const Mat& a, const Mat& b)
{
    checkOperandsExist(a, b);
    Mat_पाहायाग e;
    MatOp_GEMM::make_पाहायाग(e, 0, a, b);
    return e;
}

Mat_पाहायाग operator * (const Mat& a, double s)
{
    checkOperandsExist(a);
    Mat_पाहायाग e;
    MatOp_AddEx::make_पाहायाग(e, a, Mat(), s, 0);
    return e;
}

Mat_पाहायाग operator * (double s, const Mat& a)
{
    checkOperandsExist(a);
    Mat_पाहायाग e;
    MatOp_AddEx::make_पाहायाग(e, a, Mat(), s, 0);
    return e;
}

Mat_पाहायाग operator * (const Mat_पाहायाग& e, const Mat& m)
{
    Mat_पाहायाग en;
    e.op->matmul(e, Mat_पाहायाग(m), en);
    return en;
}

Mat_पाहायाग operator * (const Mat& m, const Mat_पाहायाग& e)
{
    checkOperandsExist(m);
    Mat_पाहायाग en;
    e.op->matmul(Mat_पाहायाग(m), e, en);
    return en;
}

Mat_पाहायाग operator * (const Mat_पाहायाग& e, double s)
{
    Mat_पाहायाग en;
    e.op->multiply(e, s, en);
    return en;
}

Mat_पाहायाग operator * (double s, const Mat_पाहायाग& e)
{
    Mat_पाहायाग en;
    e.op->multiply(e, s, en);
    return en;
}

Mat_पाहायाग operator * (const Mat_पाहायाग& e1, const Mat_पाहायाग& e2)
{
    Mat_पाहायाग en;
    e1.op->matmul(e1, e2, en);
    return en;
}

Mat_पाहायाग operator / (const Mat& a, const Mat& b)
{
    checkOperandsExist(a, b);
    Mat_पाहायाग e;
    MatOp_Bin::make_पाहायाग(e, '/', a, b);
    return e;
}

Mat_पाहायाग operator / (const Mat& a, double s)
{
    checkOperandsExist(a);
    Mat_पाहायाग e;
    MatOp_AddEx::make_पाहायाग(e, a, Mat(), 1./s, 0);
    return e;
}

Mat_पाहायाग operator / (double s, const Mat& a)
{
    checkOperandsExist(a);
    Mat_पाहायाग e;
    MatOp_Bin::make_पाहायाग(e, '/', a, Mat(), s);
    return e;
}

Mat_पाहायाग operator / (const Mat_पाहायाग& e, const Mat& m)
{
    Mat_पाहायाग en;
    e.op->divide(e, Mat_पाहायाग(m), en);
    return en;
}

Mat_पाहायाग operator / (const Mat& m, const Mat_पाहायाग& e)
{
    checkOperandsExist(m);
    Mat_पाहायाग en;
    e.op->divide(Mat_पाहायाग(m), e, en);
    return en;
}

Mat_पाहायाग operator / (const Mat_पाहायाग& e, double s)
{
    Mat_पाहायाग en;
    e.op->multiply(e, 1./s, en);
    return en;
}

Mat_पाहायाग operator / (double s, const Mat_पाहायाग& e)
{
    Mat_पाहायाग en;
    e.op->divide(s, e, en);
    return en;
}

Mat_पाहायाग operator / (const Mat_पाहायाग& e1, const Mat_पाहायाग& e2)
{
    Mat_पाहायाग en;
    e1.op->divide(e1, e2, en);
    return en;
}

Mat_पाहायाग operator < (const Mat& a, const Mat& b)
{
    checkOperandsExist(a, b);
    Mat_पाहायाग e;
    MatOp_Cmp::make_पाहायाग(e, cv::CMP_LT, a, b);
    return e;
}

Mat_पाहायाग operator < (const Mat& a, double s)
{
    checkOperandsExist(a);
    Mat_पाहायाग e;
    MatOp_Cmp::make_पाहायाग(e, cv::CMP_LT, a, s);
    return e;
}

Mat_पाहायाग operator < (double s, const Mat& a)
{
    checkOperandsExist(a);
    Mat_पाहायाग e;
    MatOp_Cmp::make_पाहायाग(e, cv::CMP_GT, a, s);
    return e;
}

Mat_पाहायाग operator <= (const Mat& a, const Mat& b)
{
    checkOperandsExist(a, b);
    Mat_पाहायाग e;
    MatOp_Cmp::make_पाहायाग(e, cv::CMP_LE, a, b);
    return e;
}

Mat_पाहायाग operator <= (const Mat& a, double s)
{
    checkOperandsExist(a);
    Mat_पाहायाग e;
    MatOp_Cmp::make_पाहायाग(e, cv::CMP_LE, a, s);
    return e;
}

Mat_पाहायाग operator <= (double s, const Mat& a)
{
    checkOperandsExist(a);
    Mat_पाहायाग e;
    MatOp_Cmp::make_पाहायाग(e, cv::CMP_GE, a, s);
    return e;
}

Mat_पाहायाग operator == (const Mat& a, const Mat& b)
{
    checkOperandsExist(a, b);
    Mat_पाहायाग e;
    MatOp_Cmp::make_पाहायाग(e, cv::CMP_EQ, a, b);
    return e;
}

Mat_पाहायाग operator == (const Mat& a, double s)
{
    checkOperandsExist(a);
    Mat_पाहायाग e;
    MatOp_Cmp::make_पाहायाग(e, cv::CMP_EQ, a, s);
    return e;
}

Mat_पाहायाग operator == (double s, const Mat& a)
{
    checkOperandsExist(a);
    Mat_पाहायाग e;
    MatOp_Cmp::make_पाहायाग(e, cv::CMP_EQ, a, s);
    return e;
}

Mat_पाहायाग operator != (const Mat& a, const Mat& b)
{
    checkOperandsExist(a, b);
    Mat_पाहायाग e;
    MatOp_Cmp::make_पाहायाग(e, cv::CMP_NE, a, b);
    return e;
}

Mat_पाहायाग operator != (const Mat& a, double s)
{
    checkOperandsExist(a);
    Mat_पाहायाग e;
    MatOp_Cmp::make_पाहायाग(e, cv::CMP_NE, a, s);
    return e;
}

Mat_पाहायाग operator != (double s, const Mat& a)
{
    checkOperandsExist(a);
    Mat_पाहायाग e;
    MatOp_Cmp::make_पाहायाग(e, cv::CMP_NE, a, s);
    return e;
}

Mat_पाहायाग operator >= (const Mat& a, const Mat& b)
{
    checkOperandsExist(a, b);
    Mat_पाहायाग e;
    MatOp_Cmp::make_पाहायाग(e, cv::CMP_GE, a, b);
    return e;
}

Mat_पाहायाग operator >= (const Mat& a, double s)
{
    checkOperandsExist(a);
    Mat_पाहायाग e;
    MatOp_Cmp::make_पाहायाग(e, cv::CMP_GE, a, s);
    return e;
}

Mat_पाहायाग operator >= (double s, const Mat& a)
{
    checkOperandsExist(a);
    Mat_पाहायाग e;
    MatOp_Cmp::make_पाहायाग(e, cv::CMP_LE, a, s);
    return e;
}

Mat_पाहायाग operator > (const Mat& a, const Mat& b)
{
    checkOperandsExist(a, b);
    Mat_पाहायाग e;
    MatOp_Cmp::make_पाहायाग(e, cv::CMP_GT, a, b);
    return e;
}

Mat_पाहायाग operator > (const Mat& a, double s)
{
    checkOperandsExist(a);
    Mat_पाहायाग e;
    MatOp_Cmp::make_पाहायाग(e, cv::CMP_GT, a, s);
    return e;
}

Mat_पाहायाग operator > (double s, const Mat& a)
{
    checkOperandsExist(a);
    Mat_पाहायाग e;
    MatOp_Cmp::make_पाहायाग(e, cv::CMP_LT, a, s);
    return e;
}

Mat_पाहायाग min(const Mat& a, const Mat& b)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a, b);
    Mat_पाहायाग e;
    MatOp_Bin::make_पाहायाग(e, 'm', a, b);
    return e;
}

Mat_पाहायाग min(const Mat& a, double s)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    Mat_पाहायाग e;
    MatOp_Bin::make_पाहायाग(e, 'n', a, s);
    return e;
}

Mat_पाहायाग min(double s, const Mat& a)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    Mat_पाहायाग e;
    MatOp_Bin::make_पाहायाग(e, 'n', a, s);
    return e;
}

Mat_पाहायाग max(const Mat& a, const Mat& b)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a, b);
    Mat_पाहायाग e;
    MatOp_Bin::make_पाहायाग(e, 'M', a, b);
    return e;
}

Mat_पाहायाग max(const Mat& a, double s)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    Mat_पाहायाग e;
    MatOp_Bin::make_पाहायाग(e, 'N', a, s);
    return e;
}

Mat_पाहायाग max(double s, const Mat& a)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    Mat_पाहायाग e;
    MatOp_Bin::make_पाहायाग(e, 'N', a, s);
    return e;
}

Mat_पाहायाग operator & (const Mat& a, const Mat& b)
{
    checkOperandsExist(a, b);
    Mat_पाहायाग e;
    MatOp_Bin::make_पाहायाग(e, '&', a, b);
    return e;
}

Mat_पाहायाग operator & (const Mat& a, const Scalar& s)
{
    checkOperandsExist(a);
    Mat_पाहायाग e;
    MatOp_Bin::make_पाहायाग(e, '&', a, s);
    return e;
}

Mat_पाहायाग operator & (const Scalar& s, const Mat& a)
{
    checkOperandsExist(a);
    Mat_पाहायाग e;
    MatOp_Bin::make_पाहायाग(e, '&', a, s);
    return e;
}

Mat_पाहायाग operator | (const Mat& a, const Mat& b)
{
    checkOperandsExist(a, b);
    Mat_पाहायाग e;
    MatOp_Bin::make_पाहायाग(e, '|', a, b);
    return e;
}

Mat_पाहायाग operator | (const Mat& a, const Scalar& s)
{
    checkOperandsExist(a);
    Mat_पाहायाग e;
    MatOp_Bin::make_पाहायाग(e, '|', a, s);
    return e;
}

Mat_पाहायाग operator | (const Scalar& s, const Mat& a)
{
    checkOperandsExist(a);
    Mat_पाहायाग e;
    MatOp_Bin::make_पाहायाग(e, '|', a, s);
    return e;
}

Mat_पाहायाग operator ^ (const Mat& a, const Mat& b)
{
    checkOperandsExist(a, b);
    Mat_पाहायाग e;
    MatOp_Bin::make_पाहायाग(e, '^', a, b);
    return e;
}

Mat_पाहायाग operator ^ (const Mat& a, const Scalar& s)
{
    checkOperandsExist(a);
    Mat_पाहायाग e;
    MatOp_Bin::make_पाहायाग(e, '^', a, s);
    return e;
}

Mat_पाहायाग operator ^ (const Scalar& s, const Mat& a)
{
    checkOperandsExist(a);
    Mat_पाहायाग e;
    MatOp_Bin::make_पाहायाग(e, '^', a, s);
    return e;
}

Mat_पाहायाग operator ~(const Mat& a)
{
    checkOperandsExist(a);
    Mat_पाहायाग e;
    MatOp_Bin::make_पाहायाग(e, '~', a, Scalar());
    return e;
}

Mat_पाहायाग abs(const Mat& a)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    Mat_पाहायाग e;
    MatOp_Bin::make_पाहायाग(e, 'a', a, Scalar());
    return e;
}

Mat_पाहायाग abs(const Mat_पाहायाग& e)
{
    CV_INSTRUMENT_REGION();

    Mat_पाहायाग en;
    e.op->abs(e, en);
    return en;
}


Size Mat_पाहायाग::size() const
{
    if( isT(*this) || isInv(*this) )
        return Size(a.rows, a.cols);
    if( isSolve(*this) )
        return Size(b.cols, a.cols);
    if( isInitializer(*this) )
        return a.size();
    return op ? op->size(*this) : Size();
}


int Mat_पाहायाग::type() const
{
    CV_INSTRUMENT_REGION();

    if( isInitializer(*this) )
        return a.type();
    if( isCmp(*this) )
        return CV_MAKETYPE(CV_8U, a.channels());
    return op ? op->type(*this) : -1;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////

void MatOp_Identity::assign(const Mat_पाहायाग& e, Mat& m, int _type) const
{
    if( _type == -1 || _type == e.a.type() )
        m = e.a;
    else
    {
        CV_Assert( CV_MAT_CN(_type) == e.a.channels() );
        e.a.convertTo(m, _type);
    }
}

inline void MatOp_Identity::make_पाहायाग(Mat_पाहायाग& res, const Mat& m)
{
    res = Mat_पाहायाग(&g_MatOp_Identity, 0, m, Mat(), Mat(), 1, 0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

void MatOp_AddEx::assign(const Mat_पाहायाग& e, Mat& m, int _type) const
{
    Mat temp, &dst = _type == -1 || e.a.type() == _type ? m : temp;
    if( e.b.data )
    {
        if( e.s == Scalar() || !e.s.isReal() )
        {
            if( e.alpha == 1 )
            {
                if( e.beta == 1 )
                    cv::add(e.a, e.b, dst);
                else if( e.beta == -1 )
                    cv::subtract(e.a, e.b, dst);
                else
                    cv::scaleAdd(e.b, e.beta, e.a, dst);
            }
            else if( e.beta == 1 )
            {
                if( e.alpha == -1 )
                    cv::subtract(e.b, e.a, dst);
                else
                    cv::scaleAdd(e.a, e.alpha, e.b, dst);
            }
            else
                cv::addWeighted(e.a, e.alpha, e.b, e.beta, 0, dst);

            if( !e.s.isReal() )
                cv::add(dst, e.s, dst);
        }
        else
        {
            if (e.a.channels() > 1)
                CV_LOG_ONCE_WARNING(NULL, "OpenCV/MatExpr: processing of multi-channel arrays might be changed in the future: "
                                          "https://github.com/opencv/opencv/issues/16739");
            cv::addWeighted(e.a, e.alpha, e.b, e.beta, e.s[0], dst);
        }
    }
    else if( e.s.isReal() && (dst.data != m.data || fabs(e.alpha) != 1))
    {
        if (e.a.channels() > 1 && e.s[0] != 0.0)
            CV_LOG_ONCE_WARNING(NULL, "OpenCV/MatExpr: processing of multi-channel arrays might be changed in the future: "
                                      "https://github.com/opencv/opencv/issues/16739");
        e.a.convertTo(m, _type, e.alpha, e.s[0]);
        return;
    }
    else if( e.alpha == 1 )
        cv::add(e.a, e.s, dst);
    else if( e.alpha == -1 )
        cv::subtract(e.s, e.a, dst);
    else
    {
        e.a.convertTo(dst, e.a.type(), e.alpha);
        cv::add(dst, e.s, dst);
    }

    if( dst.data != m.data )
        dst.convertTo(m, m.type());
}


void MatOp_AddEx::add(const Mat_पाहायाग& e, const Scalar& s, Mat_पाहायाग& res) const
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.s += s;
}


void MatOp_AddEx::subtract(const Scalar& s, const Mat_पाहायाग& e, Mat_पाहायाग& res) const
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha = -res.alpha;
    res.beta = -res.beta;
    res.s = s - res.s;
}

void MatOp_AddEx::multiply(const Mat_पाहायाग& e, double s, Mat_पाहायाग& res) const
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha *= s;
    res.beta *= s;
    res.s *= s;
}

void MatOp_AddEx::divide(double s, const Mat_पाहायाग& e, Mat_पाहायाग& res) const
{
    CV_INSTRUMENT_REGION();

    if( isScaled(e) )
        MatOp_Bin::make_पाहायाग(res, '/', e.a, Mat(), s/e.alpha);
    else
        MatOp::divide(s, e, res);
}


void MatOp_AddEx::transpose(const Mat_पाहायाग& e, Mat_पाहायाग& res) const
{
    CV_INSTRUMENT_REGION();

    if( isScaled(e) )
        MatOp_T::make_पाहायाग(res, e.a, e.alpha);
    else
        MatOp::transpose(e, res);
}

void MatOp_AddEx::abs(const Mat_पाहायाग& e, Mat_पाहायाग& res) const
{
    CV_INSTRUMENT_REGION();

    if( (!e.b.data || e.beta == 0) && fabs(e.alpha) == 1 )
        MatOp_Bin::make_पाहायाग(res, 'a', e.a, -e.s*e.alpha);
    else if( e.b.data && e.alpha + e.beta == 0 && e.alpha*e.beta == -1 )
        MatOp_Bin::make_पाहायाग(res, 'a', e.a, e.b);
    else
        MatOp::abs(e, res);
}

inline void MatOp_AddEx::make_पाहायाग(Mat_पाहायाग& res, const Mat& a, const Mat& b, double alpha, double beta, const Scalar& s)
{
    res = Mat_पाहायाग(&g_MatOp_AddEx, 0, a, b, Mat(), alpha, beta, s);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void MatOp_Bin::assign(const Mat_पाहायाग& e, Mat& m, int _type) const
{
    Mat temp, &dst = _type == -1 || e.a.type() == _type ? m : temp;

    if( e.flags == '*' )
        cv::multiply(e.a, e.b, dst, e.alpha);
    else if( e.flags == '/' && e.b.data )
        cv::divide(e.a, e.b, dst, e.alpha);
    else if( e.flags == '/' && !e.b.data )
        cv::divide(e.alpha, e.a, dst );
    else if( e.flags == '&' && e.b.data )
        bitwise_and(e.a, e.b, dst);
    else if( e.flags == '&' && !e.b.data )
        bitwise_and(e.a, e.s, dst);
    else if( e.flags == '|' && e.b.data )
        bitwise_or(e.a, e.b, dst);
    else if( e.flags == '|' && !e.b.data )
        bitwise_or(e.a, e.s, dst);
    else if( e.flags == '^' && e.b.data )
        bitwise_xor(e.a, e.b, dst);
    else if( e.flags == '^' && !e.b.data )
        bitwise_xor(e.a, e.s, dst);
    else if( e.flags == '~' && !e.b.data )
        bitwise_not(e.a, dst);
    else if( e.flags == 'm' )
        cv::min(e.a, e.b, dst);
    else if( e.flags == 'n' )
        cv::min(e.a, e.s[0], dst);
    else if( e.flags == 'M' )
        cv::max(e.a, e.b, dst);
    else if( e.flags == 'N' )
        cv::max(e.a, e.s[0], dst);
    else if( e.flags == 'a' && e.b.data )
        cv::absdiff(e.a, e.b, dst);
    else if( e.flags == 'a' && !e.b.data )
        cv::absdiff(e.a, e.s, dst);
    else
        CV_Error(cv::Error::StsError, "Unknown operation");

    if( dst.data != m.data )
        dst.convertTo(m, _type);
}

void MatOp_Bin::multiply(const Mat_पाहायाग& e, double s, Mat_पाहायाग& res) const
{
    CV_INSTRUMENT_REGION();

    if( e.flags == '*' || e.flags == '/' )
    {
        res = e;
        res.alpha *= s;
    }
    else
        MatOp::multiply(e, s, res);
}

void MatOp_Bin::divide(double s, const Mat_पाहायाग& e, Mat_पाहायाग& res) const
{
    CV_INSTRUMENT_REGION();

    if( e.flags == '/' && (!e.b.data || e.beta == 0) )
        MatOp_AddEx::make_पाहायाग(res, e.a, Mat(), s/e.alpha, 0);
    else
        MatOp::divide(s, e, res);
}

inline void MatOp_Bin::make_पाहायाग(Mat_पाहायाग& res, char op, const Mat& a, const Mat& b, double scale)
{
    res = Mat_पाहायाग(&g_MatOp_Bin, op, a, b, Mat(), scale, b.data ? 1 : 0);
}

inline void MatOp_Bin::make_पाहायाग(Mat_पाहायाग& res, char op, const Mat& a, const Scalar& s)
{
    res = Mat_पाहायाग(&g_MatOp_Bin, op, a, Mat(), Mat(), 1, 0, s);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

void MatOp_Cmp::assign(const Mat_पाहायाग& e, Mat& m, int _type) const
{
    Mat temp, &dst = _type == -1 || _type == CV_8U ? m : temp;

    if( e.b.data )
        cv::compare(e.a, e.b, dst, e.flags);
    else
        cv::compare(e.a, e.alpha, dst, e.flags);

    if( dst.data != m.data )
        dst.convertTo(m, _type);
}

inline void MatOp_Cmp::make_पाहायाग(Mat_पाहायाग& res, int cmpop, const Mat& a, const Mat& b)
{
    res = Mat_पाहायाग(&g_MatOp_Cmp, cmpop, a, b, Mat(), 1, 1);
}

inline void MatOp_Cmp::make_पाहायाग(Mat_पाहायाग& res, int cmpop, const Mat& a, double alpha)
{
    res = Mat_पाहायाग(&g_MatOp_Cmp, cmpop, a, Mat(), Mat(), alpha, 1);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

void MatOp_T::assign(const Mat_पाहायाग& e, Mat& m, int _type) const
{
    Mat temp, &dst = _type == -1 || _type == e.a.type() ? m : temp;

    cv::transpose(e.a, dst);

    if( dst.data != m.data || e.alpha != 1 )
        dst.convertTo(m, _type, e.alpha);
}

void MatOp_T::multiply(const Mat_पाहायाग& e, double s, Mat_पाहायाग& res) const
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha *= s;
}

void MatOp_T::transpose(const Mat_पाहायाग& e, Mat_पाहायाग& res) const
{
    CV_INSTRUMENT_REGION();

    if( e.alpha == 1 )
        MatOp_Identity::make_पाहायाग(res, e.a);
    else
        MatOp_AddEx::make_पाहायाग(res, e.a, Mat(), e.alpha, 0);
}

inline void MatOp_T::make_पाहायाग(Mat_पाहायाग& res, const Mat& a, double alpha)
{
    res = Mat_पाहायाग(&g_MatOp_T, 0, a, Mat(), Mat(), alpha, 0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

void MatOp_GEMM::assign(const Mat_पाहायाग& e, Mat& m, int _type) const
{
    Mat temp, &dst = _type == -1 || _type == e.a.type() ? m : temp;

    cv::gemm(e.a, e.b, e.alpha, e.c, e.beta, dst, e.flags);
    if( dst.data != m.data )
        dst.convertTo(m, _type);
}

void MatOp_GEMM::add(const Mat_पाहायाग& e1, const Mat_पाहायाग& e2, Mat_पाहायाग& res) const
{
    CV_INSTRUMENT_REGION();

    bool i1 = isIdentity(e1), i2 = isIdentity(e2);
    double alpha1 = i1 ? 1 : e1.alpha, alpha2 = i2 ? 1 : e2.alpha;

    if( isMatProd(e1) && (i2 || isScaled(e2) || isT(e2)) )
        MatOp_GEMM::make_पाहायाग(res, (e1.flags & ~cv::GEMM_3_T)|(isT(e2) ? cv::GEMM_3_T : 0),
                             e1.a, e1.b, alpha1, e2.a, alpha2);
    else if( isMatProd(e2) && (i1 || isScaled(e1) || isT(e1)) )
        MatOp_GEMM::make_पाहायाग(res, (e2.flags & ~cv::GEMM_3_T)|(isT(e1) ? cv::GEMM_3_T : 0),
                             e2.a, e2.b, alpha2, e1.a, alpha1);
    else if( this == e2.op )
        MatOp::add(e1, e2, res);
    else
        e2.op->add(e1, e2, res);
}

void MatOp_GEMM::subtract(const Mat_पाहायाग& e1, const Mat_पाहायाग& e2, Mat_पाहायाग& res) const
{
    CV_INSTRUMENT_REGION();

    bool i1 = isIdentity(e1), i2 = isIdentity(e2);
    double alpha1 = i1 ? 1 : e1.alpha, alpha2 = i2 ? 1 : e2.alpha;

    if( isMatProd(e1) && (i2 || isScaled(e2) || isT(e2)) )
        MatOp_GEMM::make_पाहायाग(res, (e1.flags & ~cv::GEMM_3_T)|(isT(e2) ? cv::GEMM_3_T : 0),
                             e1.a, e1.b, alpha1, e2.a, -alpha2);
    else if( isMatProd(e2) && (i1 || isScaled(e1) || isT(e1)) )
        MatOp_GEMM::make_पाहायाग(res, (e2.flags & ~cv::GEMM_3_T)|(isT(e1) ? cv::GEMM_3_T : 0),
                            e2.a, e2.b, -alpha2, e1.a, alpha1);
    else if( this == e2.op )
        MatOp::subtract(e1, e2, res);
    else
        e2.op->subtract(e1, e2, res);
}

void MatOp_GEMM::multiply(const Mat_पाहायाग& e, double s, Mat_पाहायाग& res) const
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha *= s;
    res.beta *= s;
}

void MatOp_GEMM::transpose(const Mat_पाहायाग& e, Mat_पाहायाग& res) const
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.flags = (!(e.flags & cv::GEMM_1_T) ? cv::GEMM_2_T : 0) |
                (!(e.flags & cv::GEMM_2_T) ? cv::GEMM_1_T : 0) |
                (!(e.flags & cv::GEMM_3_T) ? cv::GEMM_3_T : 0);
    swap(res.a, res.b);
}

inline void MatOp_GEMM::make_पाहायाग(Mat_पाहायाग& res, int flags, const Mat& a, const Mat& b,
                                 double alpha, const Mat& c, double beta)
{
    res = Mat_पाहायाग(&g_MatOp_GEMM, flags, a, b, c, alpha, beta);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

void MatOp_Invert::assign(const Mat_पाहायाग& e, Mat& m, int _type) const
{
    Mat temp, &dst = _type == -1 || _type == e.a.type() ? m : temp;

    cv::invert(e.a, dst, e.flags);
    if( dst.data != m.data )
        dst.convertTo(m, _type);
}

void MatOp_Invert::matmul(const Mat_पाहायाग& e1, const Mat_पाहायाग& e2, Mat_पाहायाग& res) const
{
    if( isInv(e1) && isIdentity(e2) )
        MatOp_Solve::make_पाहायाग(res, e1.flags, e1.a, e2.a);
    else if( this == e2.op )
        MatOp::matmul(e1, e2, res);
    else
        e2.op->matmul(e1, e2, res);
}

inline void MatOp_Invert::make_पाहायाग(Mat_पाहायाग& res, int method, const Mat& m)
{
    res = Mat_पाहायाग(&g_MatOp_Invert, method, m, Mat(), Mat(), 1, 0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

void MatOp_Solve::assign(const Mat_पाहायाग& e, Mat& m, int _type) const
{
    Mat temp, &dst = _type == -1 || _type == e.a.type() ? m : temp;

    cv::solve(e.a, e.b, dst, e.flags);
    if( dst.data != m.data )
        dst.convertTo(m, _type);
}

inline void MatOp_Solve::make_पाहायाग(Mat_पाहायाग& res, int method, const Mat& a, const Mat& b)
{
    res = Mat_पाहायाग(&g_MatOp_Solve, method, a, b, Mat(), 1, 1);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

void MatOp_Initializer::assign(const Mat_पाहायाग& e, Mat& m, int _type) const
{
    if( _type == -1 )
        _type = e.a.type();

    m.create(e.a.size, _type);

    if( e.flags == 'I' && e.a.dims <= 2 )
        setIdentity(m, Scalar(e.alpha));
    else if( e.flags == '0' )
        m = Scalar();
    else if( e.flags == '1' )
        m = Scalar(e.alpha);
    else
        CV_Error(cv::Error::StsError, "Invalid matrix initializer type");
}

void MatOp_Initializer::multiply(const Mat_पाहायाग& e, double s, Mat_पाहायाग& res) const
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha *= s;
}

inline void MatOp_Initializer::make_पाहायाग(Mat_पाहायाग& res, int method, Size sz, int type, double alpha)
{
    res = Mat_पाहायाग(getGlobalMatOpInitializer(), method, Mat(sz, type, (void*)(size_t)0xEEEEEEEE), Mat(), Mat(), alpha, 0);
}

inline void MatOp_Initializer::make_पाहायाग(Mat_पाहायाग& res, int method, int ndims, const int* sizes, int type, double alpha)
{
    res = Mat_पाहायाग(getGlobalMatOpInitializer(), method, Mat(ndims, sizes, type, (void*)(size_t)0xEEEEEEEE), Mat(), Mat(), alpha, 0);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

Mat_पाहायाग Mat::t() const
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(*this);
    Mat_पाहायाग e;
    MatOp_T::make_पाहायाग(e, *this);
    return e;
}

Mat_पाहायाग Mat::inv(int method) const
{
    CV_INSTRUMENT_REGION();

    Mat_पाहायाग e;
    MatOp_Invert::make_पाहायाग(e, method, *this);
    return e;
}


Mat_पाहायाग Mat::mul(पासोक_Array m, double scale) const
{
    CV_INSTRUMENT_REGION();

    Mat b = m.getMat();
    // Unless the argument is a refcounted Mat/UMat, the header returned by getMat() may be
    // a non-owning view of caller memory (e.g. a scalar bound to _InputArray(const double&),
    // a Matx or a Vec) that the returned MatExpr can outlive, so snapshot it.
    // See https://github.com/opencv/opencv/issues/23577
    if( !m.isMat() && !m.isUMat() )
        b = b.clone();

    Mat_पाहायाग e;
    MatOp_Bin::make_पाहायाग(e, '*', *this, b, scale);
    return e;
}

Mat_पाहायाग Mat::zeros(int rows, int cols, int type)
{
    CV_INSTRUMENT_REGION();

    Mat_पाहायाग e;
    MatOp_Initializer::make_पाहायाग(e, '0', Size(cols, rows), type);
    return e;
}

Mat_पाहायाग Mat::zeros(Size size, int type)
{
    CV_INSTRUMENT_REGION();

    Mat_पाहायाग e;
    MatOp_Initializer::make_पाहायाग(e, '0', size, type);
    return e;
}

Mat_पाहायाग Mat::zeros(int ndims, const int* sizes, int type)
{
    CV_INSTRUMENT_REGION();

    Mat_पाहायाग e;
    MatOp_Initializer::make_पाहायाग(e, '0', ndims, sizes, type);
    return e;
}

Mat_पाहायाग Mat::zeros(const MatShape& shape, int type)
{
    CV_INSTRUMENT_REGION();

    Mat_पाहायाग e;
    MatOp_Initializer::make_पाहायाग(e, '0', shape.dims, shape.p, type);
    return e;
}

Mat_पाहायाग Mat::ones(int rows, int cols, int type)
{
    CV_INSTRUMENT_REGION();

    Mat_पाहायाग e;
    MatOp_Initializer::make_पाहायाग(e, '1', Size(cols, rows), type);
    return e;
}

Mat_पाहायाग Mat::ones(Size size, int type)
{
    CV_INSTRUMENT_REGION();

    Mat_पाहायाग e;
    MatOp_Initializer::make_पाहायाग(e, '1', size, type);
    return e;
}

Mat_पाहायाग Mat::ones(int ndims, const int* sizes, int type)
{
    CV_INSTRUMENT_REGION();

    Mat_पाहायाग e;
    MatOp_Initializer::make_पाहायाग(e, '1', ndims, sizes, type);
    return e;
}

Mat_पाहायाग Mat::ones(const MatShape& shape, int type)
{
    CV_INSTRUMENT_REGION();

    Mat_पाहायाग e;
    MatOp_Initializer::make_पाहायाग(e, '1', shape.dims, shape.p, type);
    return e;
}

Mat_पाहायाग Mat::eye(int rows, int cols, int type)
{
    CV_INSTRUMENT_REGION();

    Mat_पाहायाग e;
    MatOp_Initializer::make_पाहायाग(e, 'I', Size(cols, rows), type);
    return e;
}

Mat_पाहायाग Mat::eye(Size size, int type)
{
    CV_INSTRUMENT_REGION();

    Mat_पाहायाग e;
    MatOp_Initializer::make_पाहायाग(e, 'I', size, type);
    return e;
}

void Mat_पाहायाग::swap(Mat_पाहायाग& other)
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

पासोक_Array_2::पासोक_Array_2(const Mat_पाहायाग& पाहायाग)
{
    if (!isIdentity(पाहायाग))
    {
        Mat result = पाहायाग;  // TODO improve through refcount == 1 of expr.a (inplace operation is possible - except gemm?)
        Mat_पाहायाग result_पाहायाग(result);
        swap(const_cast<Mat_पाहायाग&>(पाहायाग), result_पाहायाग);
    }
    CV_Assert(isIdentity(पाहायाग));
    init(FIXED_TYPE + FIXED_SIZE + MAT + ACCESS_READ, &पाहायाग.a);
}

} // cv::
