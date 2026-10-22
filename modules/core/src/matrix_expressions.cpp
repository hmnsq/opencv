// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


/* ////////////////////////////////////////////////////////////////////
//
//  Mat basic operations: Copy, Set
//
// */

#समावेश "precomp.hpp"
#समावेश <opencv2/core/utils/logger.hpp>

namespace cv
{

//This and its overload below are used in various MatExpr operator overloads
//implemented to check that Matrix operands exist.
अटल व्योम checkOperandsExist(स्थिर Mat& a)
{
    अगर (a.empty())
    {
        CV_Error(cv::Error::StsBadArg, "Matrix operand is an empty matrix.");
    }
}

अटल व्योम checkOperandsExist(स्थिर Mat& a, स्थिर Mat& b)
{
    अगर (a.empty() || b.empty())
    {
        CV_Error(cv::Error::StsBadArg, "One or more matrix operands are empty.");
    }
}


श्रेणी MatOp_Identity CV_FINAL : खुला MatOp
{
खुला:
    MatOp_Identity() {}
    भव ~MatOp_Identity() {}

    bool elementWise(स्थिर Mat_व्यंजक& /*व्यंजक*/) स्थिर CV_OVERRIDE { वापस true; }
    व्योम assign(स्थिर Mat_व्यंजक& व्यंजक, Mat& m, पूर्णांक type=-1) स्थिर CV_OVERRIDE;

    अटल व्योम make_व्यंजक(Mat_व्यंजक& res, स्थिर Mat& m);
};

अटल MatOp_Identity g_MatOp_Identity;

श्रेणी MatOp_AddEx CV_FINAL : खुला MatOp
{
खुला:
    MatOp_AddEx() {}
    भव ~MatOp_AddEx() {}

    bool elementWise(स्थिर Mat_व्यंजक& /*व्यंजक*/) स्थिर CV_OVERRIDE { वापस true; }
    व्योम assign(स्थिर Mat_व्यंजक& व्यंजक, Mat& m, पूर्णांक type=-1) स्थिर CV_OVERRIDE;

    व्योम add(स्थिर Mat_व्यंजक& e1, स्थिर Scalar& s, Mat_व्यंजक& res) स्थिर CV_OVERRIDE;
    व्योम subtract(स्थिर Scalar& s, स्थिर Mat_व्यंजक& व्यंजक, Mat_व्यंजक& res) स्थिर CV_OVERRIDE;
    व्योम multiply(स्थिर Mat_व्यंजक& e1, द्विगुन s, Mat_व्यंजक& res) स्थिर CV_OVERRIDE;
    व्योम divide(द्विगुन s, स्थिर Mat_व्यंजक& e, Mat_व्यंजक& res) स्थिर CV_OVERRIDE;

    व्योम transpose(स्थिर Mat_व्यंजक& e1, Mat_व्यंजक& res) स्थिर CV_OVERRIDE;
    व्योम असल(स्थिर Mat_व्यंजक& व्यंजक, Mat_व्यंजक& res) स्थिर CV_OVERRIDE;

    अटल व्योम make_व्यंजक(Mat_व्यंजक& res, स्थिर Mat& a, स्थिर Mat& b, द्विगुन alpha, द्विगुन beta, स्थिर Scalar& s=Scalar());
};

अटल MatOp_AddEx g_MatOp_AddEx;

श्रेणी MatOp_Bin CV_FINAL : खुला MatOp
{
खुला:
    MatOp_Bin() {}
    भव ~MatOp_Bin() {}

    bool elementWise(स्थिर Mat_व्यंजक& /*व्यंजक*/) स्थिर CV_OVERRIDE { वापस true; }
    व्योम assign(स्थिर Mat_व्यंजक& व्यंजक, Mat& m, पूर्णांक type=-1) स्थिर CV_OVERRIDE;

    व्योम multiply(स्थिर Mat_व्यंजक& e1, द्विगुन s, Mat_व्यंजक& res) स्थिर CV_OVERRIDE;
    व्योम divide(द्विगुन s, स्थिर Mat_व्यंजक& e, Mat_व्यंजक& res) स्थिर CV_OVERRIDE;

    अटल व्योम make_व्यंजक(Mat_व्यंजक& res, अक्षर op, स्थिर Mat& a, स्थिर Mat& b, द्विगुन scale=1);
    अटल व्योम make_व्यंजक(Mat_व्यंजक& res, अक्षर op, स्थिर Mat& a, स्थिर Scalar& s);
};

अटल MatOp_Bin g_MatOp_Bin;

श्रेणी MatOp_Cmp CV_FINAL : खुला MatOp
{
खुला:
    MatOp_Cmp() {}
    भव ~MatOp_Cmp() {}

    bool elementWise(स्थिर Mat_व्यंजक& /*व्यंजक*/) स्थिर CV_OVERRIDE { वापस true; }
    व्योम assign(स्थिर Mat_व्यंजक& व्यंजक, Mat& m, पूर्णांक type=-1) स्थिर CV_OVERRIDE;

    अटल व्योम make_व्यंजक(Mat_व्यंजक& res, पूर्णांक cmpop, स्थिर Mat& a, स्थिर Mat& b);
    अटल व्योम make_व्यंजक(Mat_व्यंजक& res, पूर्णांक cmpop, स्थिर Mat& a, द्विगुन alpha);
};

अटल MatOp_Cmp g_MatOp_Cmp;

श्रेणी MatOp_GEMM CV_FINAL : खुला MatOp
{
खुला:
    MatOp_GEMM() {}
    भव ~MatOp_GEMM() {}

    bool elementWise(स्थिर Mat_व्यंजक& /*व्यंजक*/) स्थिर CV_OVERRIDE { वापस false; }
    व्योम assign(स्थिर Mat_व्यंजक& व्यंजक, Mat& m, पूर्णांक type=-1) स्थिर CV_OVERRIDE;

    व्योम add(स्थिर Mat_व्यंजक& e1, स्थिर Mat_व्यंजक& e2, Mat_व्यंजक& res) स्थिर CV_OVERRIDE;
    व्योम subtract(स्थिर Mat_व्यंजक& e1, स्थिर Mat_व्यंजक& e2, Mat_व्यंजक& res) स्थिर CV_OVERRIDE;
    व्योम multiply(स्थिर Mat_व्यंजक& e, द्विगुन s, Mat_व्यंजक& res) स्थिर CV_OVERRIDE;

    व्योम transpose(स्थिर Mat_व्यंजक& व्यंजक, Mat_व्यंजक& res) स्थिर CV_OVERRIDE;

    आकार size(स्थिर Mat_व्यंजक& व्यंजक) स्थिर CV_OVERRIDE
    {
        वापस आकार(
            (व्यंजक.flags & GEMM_2_T) ? व्यंजक.b.rows : व्यंजक.b.cols,
            (व्यंजक.flags & GEMM_1_T) ? व्यंजक.a.cols : व्यंजक.a.rows
        );
    }

    अटल व्योम make_व्यंजक(Mat_व्यंजक& res, पूर्णांक flags, स्थिर Mat& a, स्थिर Mat& b,
                         द्विगुन alpha=1, स्थिर Mat& c=Mat(), द्विगुन beta=1);

};

अटल MatOp_GEMM g_MatOp_GEMM;

श्रेणी MatOp_Invert CV_FINAL : खुला MatOp
{
खुला:
    MatOp_Invert() {}
    भव ~MatOp_Invert() {}

    bool elementWise(स्थिर Mat_व्यंजक& /*व्यंजक*/) स्थिर CV_OVERRIDE { वापस false; }
    व्योम assign(स्थिर Mat_व्यंजक& व्यंजक, Mat& m, पूर्णांक type=-1) स्थिर CV_OVERRIDE;

    व्योम matmul(स्थिर Mat_व्यंजक& expr1, स्थिर Mat_व्यंजक& expr2, Mat_व्यंजक& res) स्थिर CV_OVERRIDE;

    अटल व्योम make_व्यंजक(Mat_व्यंजक& res, पूर्णांक method, स्थिर Mat& m);
};

अटल MatOp_Invert g_MatOp_Invert;

श्रेणी MatOp_T CV_FINAL : खुला MatOp
{
खुला:
    MatOp_T() {}
    भव ~MatOp_T() {}

    bool elementWise(स्थिर Mat_व्यंजक& /*व्यंजक*/) स्थिर CV_OVERRIDE { वापस false; }
    व्योम assign(स्थिर Mat_व्यंजक& व्यंजक, Mat& m, पूर्णांक type=-1) स्थिर CV_OVERRIDE;

    व्योम multiply(स्थिर Mat_व्यंजक& e1, द्विगुन s, Mat_व्यंजक& res) स्थिर CV_OVERRIDE;
    व्योम transpose(स्थिर Mat_व्यंजक& व्यंजक, Mat_व्यंजक& res) स्थिर CV_OVERRIDE;

    अटल व्योम make_व्यंजक(Mat_व्यंजक& res, स्थिर Mat& a, द्विगुन alpha=1);
};

अटल MatOp_T g_MatOp_T;

श्रेणी MatOp_Solve CV_FINAL : खुला MatOp
{
खुला:
    MatOp_Solve() {}
    भव ~MatOp_Solve() {}

    bool elementWise(स्थिर Mat_व्यंजक& /*व्यंजक*/) स्थिर CV_OVERRIDE { वापस false; }
    व्योम assign(स्थिर Mat_व्यंजक& व्यंजक, Mat& m, पूर्णांक type=-1) स्थिर CV_OVERRIDE;

    अटल व्योम make_व्यंजक(Mat_व्यंजक& res, पूर्णांक method, स्थिर Mat& a, स्थिर Mat& b);
};

अटल MatOp_Solve g_MatOp_Solve;

श्रेणी MatOp_Initializer CV_FINAL : खुला MatOp
{
खुला:
    MatOp_Initializer() {}
    भव ~MatOp_Initializer() {}

    bool elementWise(स्थिर Mat_व्यंजक& /*व्यंजक*/) स्थिर CV_OVERRIDE { वापस false; }
    व्योम assign(स्थिर Mat_व्यंजक& व्यंजक, Mat& m, पूर्णांक type=-1) स्थिर CV_OVERRIDE;

    व्योम multiply(स्थिर Mat_व्यंजक& e, द्विगुन s, Mat_व्यंजक& res) स्थिर CV_OVERRIDE;

    अटल व्योम make_व्यंजक(Mat_व्यंजक& res, पूर्णांक method, आकार sz, पूर्णांक type, द्विगुन alpha=1);
    अटल व्योम make_व्यंजक(Mat_व्यंजक& res, पूर्णांक method, पूर्णांक ndims, स्थिर पूर्णांक* sizes, पूर्णांक type, द्विगुन alpha=1);
};

अटल MatOp_Initializer* getGlobalMatOpInitializer()
{
    CV_SINGLETON_LAZY_INIT(MatOp_Initializer, नया MatOp_Initializer())
}

अटल अंतरभूत bool isIdentity(स्थिर Mat_व्यंजक& e) { वापस e.op == &g_MatOp_Identity; }
अटल अंतरभूत bool isAddEx(स्थिर Mat_व्यंजक& e) { वापस e.op == &g_MatOp_AddEx; }
अटल अंतरभूत bool isScaled(स्थिर Mat_व्यंजक& e) { वापस isAddEx(e) && (!e.b.तथ्य || e.beta == 0) && e.s == Scalar(); }
अटल अंतरभूत bool isBin(स्थिर Mat_व्यंजक& e, अक्षर c) { वापस e.op == &g_MatOp_Bin && e.flags == c; }
अटल अंतरभूत bool isCmp(स्थिर Mat_व्यंजक& e) { वापस e.op == &g_MatOp_Cmp; }
अटल अंतरभूत bool isReciprocal(स्थिर Mat_व्यंजक& e) { वापस isBin(e,'/') && (!e.b.तथ्य || e.beta == 0); }
अटल अंतरभूत bool isT(स्थिर Mat_व्यंजक& e) { वापस e.op == &g_MatOp_T; }
अटल अंतरभूत bool isInv(स्थिर Mat_व्यंजक& e) { वापस e.op == &g_MatOp_Invert; }
अटल अंतरभूत bool isSolve(स्थिर Mat_व्यंजक& e) { वापस e.op == &g_MatOp_Solve; }
//static inline bool isGEMM(const MatExpr& e) { return e.op == &g_MatOp_GEMM; }
अटल अंतरभूत bool isMatProd(स्थिर Mat_व्यंजक& e) { वापस e.op == &g_MatOp_GEMM && (!e.c.तथ्य || e.beta == 0); }
अटल अंतरभूत bool isInitializer(स्थिर Mat_व्यंजक& e) { वापस e.op == getGlobalMatOpInitializer(); }

/////////////////////////////////////////////////////////////////////////////////////////////////////

MatOp::MatOp() {}
MatOp::~MatOp() {}


bool MatOp::elementWise(स्थिर Mat_व्यंजक& /*व्यंजक*/) स्थिर
{
    वापस false;
}

व्योम MatOp::roi(स्थिर Mat_व्यंजक& व्यंजक, स्थिर Range& rowRange, स्थिर Range& colRange, Mat_व्यंजक& e) स्थिर
{
    अगर( elementWise(व्यंजक) )
    {
        e = Mat_व्यंजक(व्यंजक.op, व्यंजक.flags, Mat(), Mat(), Mat(),
                    व्यंजक.alpha, व्यंजक.beta, व्यंजक.s);
        अगर(व्यंजक.a.तथ्य)
            e.a = व्यंजक.a(rowRange, colRange);
        अगर(व्यंजक.b.तथ्य)
            e.b = व्यंजक.b(rowRange, colRange);
        अगर(व्यंजक.c.तथ्य)
            e.c = व्यंजक.c(rowRange, colRange);
    }
    अन्यथा
    {
        Mat m;
        व्यंजक.op->assign(व्यंजक, m);
        e = Mat_व्यंजक(&g_MatOp_Identity, 0, m(rowRange, colRange), Mat(), Mat());
    }
}

व्योम MatOp::diag(स्थिर Mat_व्यंजक& व्यंजक, पूर्णांक d, Mat_व्यंजक& e) स्थिर
{
    अगर( elementWise(व्यंजक) )
    {
        e = Mat_व्यंजक(व्यंजक.op, व्यंजक.flags, Mat(), Mat(), Mat(),
                    व्यंजक.alpha, व्यंजक.beta, व्यंजक.s);
        अगर(व्यंजक.a.तथ्य)
            e.a = व्यंजक.a.diag(d);
        अगर(व्यंजक.b.तथ्य)
            e.b = व्यंजक.b.diag(d);
        अगर(व्यंजक.c.तथ्य)
            e.c = व्यंजक.c.diag(d);
    }
    अन्यथा
    {
        Mat m;
        व्यंजक.op->assign(व्यंजक, m);
        e = Mat_व्यंजक(&g_MatOp_Identity, 0, m.diag(d), Mat(), Mat());
    }
}


व्योम MatOp::augAssignAdd(स्थिर Mat_व्यंजक& व्यंजक, Mat& m) स्थिर
{
    Mat अस्थायी_2;
    व्यंजक.op->assign(व्यंजक, अस्थायी_2);
    m += अस्थायी_2;
}


व्योम MatOp::augAssignSubtract(स्थिर Mat_व्यंजक& व्यंजक, Mat& m) स्थिर
{
    Mat अस्थायी_2;
    व्यंजक.op->assign(व्यंजक, अस्थायी_2);
    m -= अस्थायी_2;
}


व्योम MatOp::augAssignMultiply(स्थिर Mat_व्यंजक& व्यंजक, Mat& m) स्थिर
{
    Mat अस्थायी_2;
    व्यंजक.op->assign(व्यंजक, अस्थायी_2);
    m *= अस्थायी_2;
}


व्योम MatOp::augAssignDivide(स्थिर Mat_व्यंजक& व्यंजक, Mat& m) स्थिर
{
    Mat अस्थायी_2;
    व्यंजक.op->assign(व्यंजक, अस्थायी_2);
    m /= अस्थायी_2;
}


व्योम MatOp::augAssignAnd(स्थिर Mat_व्यंजक& व्यंजक, Mat& m) स्थिर
{
    Mat अस्थायी_2;
    व्यंजक.op->assign(व्यंजक, अस्थायी_2);
    m &= अस्थायी_2;
}


व्योम MatOp::augAssignOr(स्थिर Mat_व्यंजक& व्यंजक, Mat& m) स्थिर
{
    Mat अस्थायी_2;
    व्यंजक.op->assign(व्यंजक, अस्थायी_2);
    m |= अस्थायी_2;
}


व्योम MatOp::augAssignXor(स्थिर Mat_व्यंजक& व्यंजक, Mat& m) स्थिर
{
    Mat अस्थायी_2;
    व्यंजक.op->assign(व्यंजक, अस्थायी_2);
    m ^= अस्थायी_2;
}


व्योम MatOp::add(स्थिर Mat_व्यंजक& e1, स्थिर Mat_व्यंजक& e2, Mat_व्यंजक& res) स्थिर
{
    CV_INSTRUMENT_REGION();

    अगर( यह == e2.op )
    {
        द्विगुन alpha = 1, beta = 1;
        Scalar s;
        Mat m1, m2;
        अगर( isAddEx(e1) && (!e1.b.तथ्य || e1.beta == 0) )
        {
            m1 = e1.a;
            alpha = e1.alpha;
            s = e1.s;
        }
        अन्यथा
            e1.op->assign(e1, m1);

        अगर( isAddEx(e2) && (!e2.b.तथ्य || e2.beta == 0) )
        {
            m2 = e2.a;
            beta = e2.alpha;
            s += e2.s;
        }
        अन्यथा
            e2.op->assign(e2, m2);
        MatOp_AddEx::make_व्यंजक(res, m1, m2, alpha, beta, s);
    }
    अन्यथा
        e2.op->add(e1, e2, res);
}


व्योम MatOp::add(स्थिर Mat_व्यंजक& expr1, स्थिर Scalar& s, Mat_व्यंजक& res) स्थिर
{
    CV_INSTRUMENT_REGION();

    Mat m1;
    expr1.op->assign(expr1, m1);
    MatOp_AddEx::make_व्यंजक(res, m1, Mat(), 1, 0, s);
}


व्योम MatOp::subtract(स्थिर Mat_व्यंजक& e1, स्थिर Mat_व्यंजक& e2, Mat_व्यंजक& res) स्थिर
{
    CV_INSTRUMENT_REGION();

    अगर( यह == e2.op )
    {
        द्विगुन alpha = 1, beta = -1;
        Scalar s;
        Mat m1, m2;
        अगर( isAddEx(e1) && (!e1.b.तथ्य || e1.beta == 0) )
        {
            m1 = e1.a;
            alpha = e1.alpha;
            s = e1.s;
        }
        अन्यथा
            e1.op->assign(e1, m1);

        अगर( isAddEx(e2) && (!e2.b.तथ्य || e2.beta == 0) )
        {
            m2 = e2.a;
            beta = -e2.alpha;
            s -= e2.s;
        }
        अन्यथा
            e2.op->assign(e2, m2);
        MatOp_AddEx::make_व्यंजक(res, m1, m2, alpha, beta, s);
    }
    अन्यथा
        e2.op->subtract(e1, e2, res);
}


व्योम MatOp::subtract(स्थिर Scalar& s, स्थिर Mat_व्यंजक& व्यंजक, Mat_व्यंजक& res) स्थिर
{
    CV_INSTRUMENT_REGION();

    Mat m;
    व्यंजक.op->assign(व्यंजक, m);
    MatOp_AddEx::make_व्यंजक(res, m, Mat(), -1, 0, s);
}


व्योम MatOp::multiply(स्थिर Mat_व्यंजक& e1, स्थिर Mat_व्यंजक& e2, Mat_व्यंजक& res, द्विगुन scale) स्थिर
{
    CV_INSTRUMENT_REGION();

    अगर( यह == e2.op )
    {
        Mat m1, m2;

        अगर( isReciprocal(e1) )
        {
            अगर( isScaled(e2) )
            {
                scale *= e2.alpha;
                m2 = e2.a;
            }
            अन्यथा
                e2.op->assign(e2, m2);

            MatOp_Bin::make_व्यंजक(res, '/', m2, e1.a, scale/e1.alpha);
        }
        अन्यथा
        {
            अक्षर op = '*';
            अगर( isScaled(e1) )
            {
                m1 = e1.a;
                scale *= e1.alpha;
            }
            अन्यथा
                e1.op->assign(e1, m1);

            अगर( isScaled(e2) )
            {
                m2 = e2.a;
                scale *= e2.alpha;
            }
            अन्यथा अगर( isReciprocal(e2) )
            {
                op = '/';
                m2 = e2.a;
                scale *= e2.alpha;
            }
            अन्यथा
                e2.op->assign(e2, m2);

            MatOp_Bin::make_व्यंजक(res, op, m1, m2, scale);
        }
    }
    अन्यथा
        e2.op->multiply(e1, e2, res, scale);
}


व्योम MatOp::multiply(स्थिर Mat_व्यंजक& व्यंजक, द्विगुन s, Mat_व्यंजक& res) स्थिर
{
    CV_INSTRUMENT_REGION();

    Mat m;
    व्यंजक.op->assign(व्यंजक, m);
    MatOp_AddEx::make_व्यंजक(res, m, Mat(), s, 0);
}


व्योम MatOp::divide(स्थिर Mat_व्यंजक& e1, स्थिर Mat_व्यंजक& e2, Mat_व्यंजक& res, द्विगुन scale) स्थिर
{
    CV_INSTRUMENT_REGION();

    अगर( यह == e2.op )
    {
        अगर( isReciprocal(e1) && isReciprocal(e2) )
            MatOp_Bin::make_व्यंजक(res, '/', e2.a, e1.a, e1.alpha/e2.alpha);
        अन्यथा
        {
            Mat m1, m2;
            अक्षर op = '/';

            अगर( isScaled(e1) )
            {
                m1 = e1.a;
                scale *= e1.alpha;
            }
            अन्यथा
                e1.op->assign(e1, m1);

            अगर( isScaled(e2) )
            {
                m2 = e2.a;
                scale /= e2.alpha;
            }
            अन्यथा अगर( isReciprocal(e2) )
            {
                m2 = e2.a;
                scale /= e2.alpha;
                op = '*';
            }
            अन्यथा
                e2.op->assign(e2, m2);
            MatOp_Bin::make_व्यंजक(res, op, m1, m2, scale);
        }
    }
    अन्यथा
        e2.op->divide(e1, e2, res, scale);
}


व्योम MatOp::divide(द्विगुन s, स्थिर Mat_व्यंजक& व्यंजक, Mat_व्यंजक& res) स्थिर
{
    CV_INSTRUMENT_REGION();

    Mat m;
    व्यंजक.op->assign(व्यंजक, m);
    MatOp_Bin::make_व्यंजक(res, '/', m, Mat(), s);
}


व्योम MatOp::असल(स्थिर Mat_व्यंजक& व्यंजक, Mat_व्यंजक& res) स्थिर
{
    CV_INSTRUMENT_REGION();

    Mat m;
    व्यंजक.op->assign(व्यंजक, m);
    MatOp_Bin::make_व्यंजक(res, 'a', m, Mat());
}


व्योम MatOp::transpose(स्थिर Mat_व्यंजक& व्यंजक, Mat_व्यंजक& res) स्थिर
{
    CV_INSTRUMENT_REGION();

    Mat m;
    व्यंजक.op->assign(व्यंजक, m);
    MatOp_T::make_व्यंजक(res, m, 1);
}


व्योम MatOp::matmul(स्थिर Mat_व्यंजक& e1, स्थिर Mat_व्यंजक& e2, Mat_व्यंजक& res) स्थिर
{
    अगर( यह == e2.op )
    {
        द्विगुन scale = 1;
        पूर्णांक flags = 0;
        Mat m1, m2;

        अगर( isT(e1) )
        {
            flags = cv::GEMM_1_T;
            scale = e1.alpha;
            m1 = e1.a;
        }
        अन्यथा अगर( isScaled(e1) )
        {
            scale = e1.alpha;
            m1 = e1.a;
        }
        अन्यथा
            e1.op->assign(e1, m1);

        अगर( isT(e2) )
        {
            flags |= cv::GEMM_2_T;
            scale *= e2.alpha;
            m2 = e2.a;
        }
        अन्यथा अगर( isScaled(e2) )
        {
            scale *= e2.alpha;
            m2 = e2.a;
        }
        अन्यथा
            e2.op->assign(e2, m2);

        MatOp_GEMM::make_व्यंजक(res, flags, m1, m2, scale);
    }
    अन्यथा
        e2.op->matmul(e1, e2, res);
}


व्योम MatOp::invert(स्थिर Mat_व्यंजक& व्यंजक, पूर्णांक method, Mat_व्यंजक& res) स्थिर
{
    Mat m;
    व्यंजक.op->assign(व्यंजक, m);
    MatOp_Invert::make_व्यंजक(res, method, m);
}


आकार MatOp::size(स्थिर Mat_व्यंजक& व्यंजक) स्थिर
{
    वापस !व्यंजक.a.empty() ? व्यंजक.a.size() : व्यंजक.b.empty() ? व्यंजक.b.size() : व्यंजक.c.size();
}

पूर्णांक MatOp::type(स्थिर Mat_व्यंजक& व्यंजक) स्थिर
{
    CV_INSTRUMENT_REGION();

    वापस !व्यंजक.a.empty() ? व्यंजक.a.type() : व्यंजक.b.empty() ? व्यंजक.b.type() : व्यंजक.c.type();
}

//////////////////////////////////////////////////////////////////////////////////////////////////

Mat_व्यंजक::Mat_व्यंजक(स्थिर Mat& m) : op(&g_MatOp_Identity), flags(0), a(m), b(Mat()), c(Mat()), alpha(1), beta(0), s(Scalar())
{
}

Mat_व्यंजक Mat_व्यंजक::row(पूर्णांक y) स्थिर
{
    Mat_व्यंजक e;
    op->roi(*यह, Range(y, y+1), Range::all(), e);
    वापस e;
}

Mat_व्यंजक Mat_व्यंजक::col(पूर्णांक x) स्थिर
{
    Mat_व्यंजक e;
    op->roi(*यह, Range::all(), Range(x, x+1), e);
    वापस e;
}

Mat_व्यंजक Mat_व्यंजक::diag(पूर्णांक d) स्थिर
{
    Mat_व्यंजक e;
    op->diag(*यह, d, e);
    वापस e;
}

Mat_व्यंजक Mat_व्यंजक::चालक()( स्थिर Range& rowRange, स्थिर Range& colRange ) स्थिर
{
    Mat_व्यंजक e;
    op->roi(*यह, rowRange, colRange, e);
    वापस e;
}

Mat_व्यंजक Mat_व्यंजक::चालक()( स्थिर Rect& roi ) स्थिर
{
    Mat_व्यंजक e;
    op->roi(*यह, Range(roi.y, roi.y + roi.height), Range(roi.x, roi.x + roi.width), e);
    वापस e;
}

Mat Mat_व्यंजक::cross(स्थिर Mat& m) स्थिर
{
    वापस ((Mat)*यह).cross(m);
}

द्विगुन Mat_व्यंजक::dot(स्थिर Mat& m) स्थिर
{
    वापस ((Mat)*यह).dot(m);
}

Mat_व्यंजक Mat_व्यंजक::t() स्थिर
{
    Mat_व्यंजक e;
    op->transpose(*यह, e);
    वापस e;
}

Mat_व्यंजक Mat_व्यंजक::inv(पूर्णांक method) स्थिर
{
    Mat_व्यंजक e;
    op->invert(*यह, method, e);
    वापस e;
}

Mat_व्यंजक Mat_व्यंजक::mul(स्थिर Mat_व्यंजक& e, द्विगुन scale) स्थिर
{
    Mat_व्यंजक en;
    op->multiply(*यह, e, en, scale);
    वापस en;
}

Mat_व्यंजक Mat_व्यंजक::mul(स्थिर Mat& m, द्विगुन scale) स्थिर
{
    Mat_व्यंजक e;
    op->multiply(*यह, Mat_व्यंजक(m), e, scale);
    वापस e;
}

Mat_व्यंजक चालक + (स्थिर Mat& a, स्थिर Mat& b)
{
    checkOperandsExist(a, b);
    Mat_व्यंजक e;
    MatOp_AddEx::make_व्यंजक(e, a, b, 1, 1);
    वापस e;
}

Mat_व्यंजक चालक + (स्थिर Mat& a, स्थिर Scalar& s)
{
    checkOperandsExist(a);
    Mat_व्यंजक e;
    MatOp_AddEx::make_व्यंजक(e, a, Mat(), 1, 0, s);
    वापस e;
}

Mat_व्यंजक चालक + (स्थिर Scalar& s, स्थिर Mat& a)
{
    checkOperandsExist(a);
    Mat_व्यंजक e;
    MatOp_AddEx::make_व्यंजक(e, a, Mat(), 1, 0, s);
    वापस e;
}

Mat_व्यंजक चालक + (स्थिर Mat_व्यंजक& e, स्थिर Mat& m)
{
    Mat_व्यंजक en;
    e.op->add(e, Mat_व्यंजक(m), en);
    वापस en;
}

Mat_व्यंजक चालक + (स्थिर Mat& m, स्थिर Mat_व्यंजक& e)
{
    checkOperandsExist(m);
    Mat_व्यंजक en;
    e.op->add(e, Mat_व्यंजक(m), en);
    वापस en;
}

Mat_व्यंजक चालक + (स्थिर Mat_व्यंजक& e, स्थिर Scalar& s)
{
    Mat_व्यंजक en;
    e.op->add(e, s, en);
    वापस en;
}

Mat_व्यंजक चालक + (स्थिर Scalar& s, स्थिर Mat_व्यंजक& e)
{
    Mat_व्यंजक en;
    e.op->add(e, s, en);
    वापस en;
}

Mat_व्यंजक चालक + (स्थिर Mat_व्यंजक& e1, स्थिर Mat_व्यंजक& e2)
{
    Mat_व्यंजक en;
    e1.op->add(e1, e2, en);
    वापस en;
}

Mat_व्यंजक चालक - (स्थिर Mat& a, स्थिर Mat& b)
{
    checkOperandsExist(a, b);
    Mat_व्यंजक e;
    MatOp_AddEx::make_व्यंजक(e, a, b, 1, -1);
    वापस e;
}

Mat_व्यंजक चालक - (स्थिर Mat& a, स्थिर Scalar& s)
{
    checkOperandsExist(a);
    Mat_व्यंजक e;
    MatOp_AddEx::make_व्यंजक(e, a, Mat(), 1, 0, -s);
    वापस e;
}

Mat_व्यंजक चालक - (स्थिर Scalar& s, स्थिर Mat& a)
{
    checkOperandsExist(a);
    Mat_व्यंजक e;
    MatOp_AddEx::make_व्यंजक(e, a, Mat(), -1, 0, s);
    वापस e;
}

Mat_व्यंजक चालक - (स्थिर Mat_व्यंजक& e, स्थिर Mat& m)
{
    checkOperandsExist(m);
    Mat_व्यंजक en;
    e.op->subtract(e, Mat_व्यंजक(m), en);
    वापस en;
}

Mat_व्यंजक चालक - (स्थिर Mat& m, स्थिर Mat_व्यंजक& e)
{
    checkOperandsExist(m);
    Mat_व्यंजक en;
    e.op->subtract(Mat_व्यंजक(m), e, en);
    वापस en;
}

Mat_व्यंजक चालक - (स्थिर Mat_व्यंजक& e, स्थिर Scalar& s)
{
    Mat_व्यंजक en;
    e.op->add(e, -s, en);
    वापस en;
}

Mat_व्यंजक चालक - (स्थिर Scalar& s, स्थिर Mat_व्यंजक& e)
{
    Mat_व्यंजक en;
    e.op->subtract(s, e, en);
    वापस en;
}

Mat_व्यंजक चालक - (स्थिर Mat_व्यंजक& e1, स्थिर Mat_व्यंजक& e2)
{
    Mat_व्यंजक en;
    e1.op->subtract(e1, e2, en);
    वापस en;
}

Mat_व्यंजक चालक - (स्थिर Mat& m)
{
    checkOperandsExist(m);
    Mat_व्यंजक e;
    MatOp_AddEx::make_व्यंजक(e, m, Mat(), -1, 0);
    वापस e;
}

Mat_व्यंजक चालक - (स्थिर Mat_व्यंजक& e)
{
    Mat_व्यंजक en;
    e.op->subtract(Scalar(0), e, en);
    वापस en;
}

Mat_व्यंजक चालक * (स्थिर Mat& a, स्थिर Mat& b)
{
    checkOperandsExist(a, b);
    Mat_व्यंजक e;
    MatOp_GEMM::make_व्यंजक(e, 0, a, b);
    वापस e;
}

Mat_व्यंजक चालक * (स्थिर Mat& a, द्विगुन s)
{
    checkOperandsExist(a);
    Mat_व्यंजक e;
    MatOp_AddEx::make_व्यंजक(e, a, Mat(), s, 0);
    वापस e;
}

Mat_व्यंजक चालक * (द्विगुन s, स्थिर Mat& a)
{
    checkOperandsExist(a);
    Mat_व्यंजक e;
    MatOp_AddEx::make_व्यंजक(e, a, Mat(), s, 0);
    वापस e;
}

Mat_व्यंजक चालक * (स्थिर Mat_व्यंजक& e, स्थिर Mat& m)
{
    Mat_व्यंजक en;
    e.op->matmul(e, Mat_व्यंजक(m), en);
    वापस en;
}

Mat_व्यंजक चालक * (स्थिर Mat& m, स्थिर Mat_व्यंजक& e)
{
    checkOperandsExist(m);
    Mat_व्यंजक en;
    e.op->matmul(Mat_व्यंजक(m), e, en);
    वापस en;
}

Mat_व्यंजक चालक * (स्थिर Mat_व्यंजक& e, द्विगुन s)
{
    Mat_व्यंजक en;
    e.op->multiply(e, s, en);
    वापस en;
}

Mat_व्यंजक चालक * (द्विगुन s, स्थिर Mat_व्यंजक& e)
{
    Mat_व्यंजक en;
    e.op->multiply(e, s, en);
    वापस en;
}

Mat_व्यंजक चालक * (स्थिर Mat_व्यंजक& e1, स्थिर Mat_व्यंजक& e2)
{
    Mat_व्यंजक en;
    e1.op->matmul(e1, e2, en);
    वापस en;
}

Mat_व्यंजक चालक / (स्थिर Mat& a, स्थिर Mat& b)
{
    checkOperandsExist(a, b);
    Mat_व्यंजक e;
    MatOp_Bin::make_व्यंजक(e, '/', a, b);
    वापस e;
}

Mat_व्यंजक चालक / (स्थिर Mat& a, द्विगुन s)
{
    checkOperandsExist(a);
    Mat_व्यंजक e;
    MatOp_AddEx::make_व्यंजक(e, a, Mat(), 1./s, 0);
    वापस e;
}

Mat_व्यंजक चालक / (द्विगुन s, स्थिर Mat& a)
{
    checkOperandsExist(a);
    Mat_व्यंजक e;
    MatOp_Bin::make_व्यंजक(e, '/', a, Mat(), s);
    वापस e;
}

Mat_व्यंजक चालक / (स्थिर Mat_व्यंजक& e, स्थिर Mat& m)
{
    Mat_व्यंजक en;
    e.op->divide(e, Mat_व्यंजक(m), en);
    वापस en;
}

Mat_व्यंजक चालक / (स्थिर Mat& m, स्थिर Mat_व्यंजक& e)
{
    checkOperandsExist(m);
    Mat_व्यंजक en;
    e.op->divide(Mat_व्यंजक(m), e, en);
    वापस en;
}

Mat_व्यंजक चालक / (स्थिर Mat_व्यंजक& e, द्विगुन s)
{
    Mat_व्यंजक en;
    e.op->multiply(e, 1./s, en);
    वापस en;
}

Mat_व्यंजक चालक / (द्विगुन s, स्थिर Mat_व्यंजक& e)
{
    Mat_व्यंजक en;
    e.op->divide(s, e, en);
    वापस en;
}

Mat_व्यंजक चालक / (स्थिर Mat_व्यंजक& e1, स्थिर Mat_व्यंजक& e2)
{
    Mat_व्यंजक en;
    e1.op->divide(e1, e2, en);
    वापस en;
}

Mat_व्यंजक चालक < (स्थिर Mat& a, स्थिर Mat& b)
{
    checkOperandsExist(a, b);
    Mat_व्यंजक e;
    MatOp_Cmp::make_व्यंजक(e, cv::CMP_LT, a, b);
    वापस e;
}

Mat_व्यंजक चालक < (स्थिर Mat& a, द्विगुन s)
{
    checkOperandsExist(a);
    Mat_व्यंजक e;
    MatOp_Cmp::make_व्यंजक(e, cv::CMP_LT, a, s);
    वापस e;
}

Mat_व्यंजक चालक < (द्विगुन s, स्थिर Mat& a)
{
    checkOperandsExist(a);
    Mat_व्यंजक e;
    MatOp_Cmp::make_व्यंजक(e, cv::CMP_GT, a, s);
    वापस e;
}

Mat_व्यंजक चालक <= (स्थिर Mat& a, स्थिर Mat& b)
{
    checkOperandsExist(a, b);
    Mat_व्यंजक e;
    MatOp_Cmp::make_व्यंजक(e, cv::CMP_LE, a, b);
    वापस e;
}

Mat_व्यंजक चालक <= (स्थिर Mat& a, द्विगुन s)
{
    checkOperandsExist(a);
    Mat_व्यंजक e;
    MatOp_Cmp::make_व्यंजक(e, cv::CMP_LE, a, s);
    वापस e;
}

Mat_व्यंजक चालक <= (द्विगुन s, स्थिर Mat& a)
{
    checkOperandsExist(a);
    Mat_व्यंजक e;
    MatOp_Cmp::make_व्यंजक(e, cv::CMP_GE, a, s);
    वापस e;
}

Mat_व्यंजक चालक == (स्थिर Mat& a, स्थिर Mat& b)
{
    checkOperandsExist(a, b);
    Mat_व्यंजक e;
    MatOp_Cmp::make_व्यंजक(e, cv::CMP_EQ, a, b);
    वापस e;
}

Mat_व्यंजक चालक == (स्थिर Mat& a, द्विगुन s)
{
    checkOperandsExist(a);
    Mat_व्यंजक e;
    MatOp_Cmp::make_व्यंजक(e, cv::CMP_EQ, a, s);
    वापस e;
}

Mat_व्यंजक चालक == (द्विगुन s, स्थिर Mat& a)
{
    checkOperandsExist(a);
    Mat_व्यंजक e;
    MatOp_Cmp::make_व्यंजक(e, cv::CMP_EQ, a, s);
    वापस e;
}

Mat_व्यंजक चालक != (स्थिर Mat& a, स्थिर Mat& b)
{
    checkOperandsExist(a, b);
    Mat_व्यंजक e;
    MatOp_Cmp::make_व्यंजक(e, cv::CMP_NE, a, b);
    वापस e;
}

Mat_व्यंजक चालक != (स्थिर Mat& a, द्विगुन s)
{
    checkOperandsExist(a);
    Mat_व्यंजक e;
    MatOp_Cmp::make_व्यंजक(e, cv::CMP_NE, a, s);
    वापस e;
}

Mat_व्यंजक चालक != (द्विगुन s, स्थिर Mat& a)
{
    checkOperandsExist(a);
    Mat_व्यंजक e;
    MatOp_Cmp::make_व्यंजक(e, cv::CMP_NE, a, s);
    वापस e;
}

Mat_व्यंजक चालक >= (स्थिर Mat& a, स्थिर Mat& b)
{
    checkOperandsExist(a, b);
    Mat_व्यंजक e;
    MatOp_Cmp::make_व्यंजक(e, cv::CMP_GE, a, b);
    वापस e;
}

Mat_व्यंजक चालक >= (स्थिर Mat& a, द्विगुन s)
{
    checkOperandsExist(a);
    Mat_व्यंजक e;
    MatOp_Cmp::make_व्यंजक(e, cv::CMP_GE, a, s);
    वापस e;
}

Mat_व्यंजक चालक >= (द्विगुन s, स्थिर Mat& a)
{
    checkOperandsExist(a);
    Mat_व्यंजक e;
    MatOp_Cmp::make_व्यंजक(e, cv::CMP_LE, a, s);
    वापस e;
}

Mat_व्यंजक चालक > (स्थिर Mat& a, स्थिर Mat& b)
{
    checkOperandsExist(a, b);
    Mat_व्यंजक e;
    MatOp_Cmp::make_व्यंजक(e, cv::CMP_GT, a, b);
    वापस e;
}

Mat_व्यंजक चालक > (स्थिर Mat& a, द्विगुन s)
{
    checkOperandsExist(a);
    Mat_व्यंजक e;
    MatOp_Cmp::make_व्यंजक(e, cv::CMP_GT, a, s);
    वापस e;
}

Mat_व्यंजक चालक > (द्विगुन s, स्थिर Mat& a)
{
    checkOperandsExist(a);
    Mat_व्यंजक e;
    MatOp_Cmp::make_व्यंजक(e, cv::CMP_LT, a, s);
    वापस e;
}

Mat_व्यंजक min(स्थिर Mat& a, स्थिर Mat& b)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a, b);
    Mat_व्यंजक e;
    MatOp_Bin::make_व्यंजक(e, 'm', a, b);
    वापस e;
}

Mat_व्यंजक min(स्थिर Mat& a, द्विगुन s)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    Mat_व्यंजक e;
    MatOp_Bin::make_व्यंजक(e, 'n', a, s);
    वापस e;
}

Mat_व्यंजक min(द्विगुन s, स्थिर Mat& a)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    Mat_व्यंजक e;
    MatOp_Bin::make_व्यंजक(e, 'n', a, s);
    वापस e;
}

Mat_व्यंजक max(स्थिर Mat& a, स्थिर Mat& b)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a, b);
    Mat_व्यंजक e;
    MatOp_Bin::make_व्यंजक(e, 'M', a, b);
    वापस e;
}

Mat_व्यंजक max(स्थिर Mat& a, द्विगुन s)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    Mat_व्यंजक e;
    MatOp_Bin::make_व्यंजक(e, 'N', a, s);
    वापस e;
}

Mat_व्यंजक max(द्विगुन s, स्थिर Mat& a)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    Mat_व्यंजक e;
    MatOp_Bin::make_व्यंजक(e, 'N', a, s);
    वापस e;
}

Mat_व्यंजक चालक & (स्थिर Mat& a, स्थिर Mat& b)
{
    checkOperandsExist(a, b);
    Mat_व्यंजक e;
    MatOp_Bin::make_व्यंजक(e, '&', a, b);
    वापस e;
}

Mat_व्यंजक चालक & (स्थिर Mat& a, स्थिर Scalar& s)
{
    checkOperandsExist(a);
    Mat_व्यंजक e;
    MatOp_Bin::make_व्यंजक(e, '&', a, s);
    वापस e;
}

Mat_व्यंजक चालक & (स्थिर Scalar& s, स्थिर Mat& a)
{
    checkOperandsExist(a);
    Mat_व्यंजक e;
    MatOp_Bin::make_व्यंजक(e, '&', a, s);
    वापस e;
}

Mat_व्यंजक चालक | (स्थिर Mat& a, स्थिर Mat& b)
{
    checkOperandsExist(a, b);
    Mat_व्यंजक e;
    MatOp_Bin::make_व्यंजक(e, '|', a, b);
    वापस e;
}

Mat_व्यंजक चालक | (स्थिर Mat& a, स्थिर Scalar& s)
{
    checkOperandsExist(a);
    Mat_व्यंजक e;
    MatOp_Bin::make_व्यंजक(e, '|', a, s);
    वापस e;
}

Mat_व्यंजक चालक | (स्थिर Scalar& s, स्थिर Mat& a)
{
    checkOperandsExist(a);
    Mat_व्यंजक e;
    MatOp_Bin::make_व्यंजक(e, '|', a, s);
    वापस e;
}

Mat_व्यंजक चालक ^ (स्थिर Mat& a, स्थिर Mat& b)
{
    checkOperandsExist(a, b);
    Mat_व्यंजक e;
    MatOp_Bin::make_व्यंजक(e, '^', a, b);
    वापस e;
}

Mat_व्यंजक चालक ^ (स्थिर Mat& a, स्थिर Scalar& s)
{
    checkOperandsExist(a);
    Mat_व्यंजक e;
    MatOp_Bin::make_व्यंजक(e, '^', a, s);
    वापस e;
}

Mat_व्यंजक चालक ^ (स्थिर Scalar& s, स्थिर Mat& a)
{
    checkOperandsExist(a);
    Mat_व्यंजक e;
    MatOp_Bin::make_व्यंजक(e, '^', a, s);
    वापस e;
}

Mat_व्यंजक चालक ~(स्थिर Mat& a)
{
    checkOperandsExist(a);
    Mat_व्यंजक e;
    MatOp_Bin::make_व्यंजक(e, '~', a, Scalar());
    वापस e;
}

Mat_व्यंजक असल(स्थिर Mat& a)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    Mat_व्यंजक e;
    MatOp_Bin::make_व्यंजक(e, 'a', a, Scalar());
    वापस e;
}

Mat_व्यंजक असल(स्थिर Mat_व्यंजक& e)
{
    CV_INSTRUMENT_REGION();

    Mat_व्यंजक en;
    e.op->असल(e, en);
    वापस en;
}


आकार Mat_व्यंजक::size() स्थिर
{
    अगर( isT(*यह) || isInv(*यह) )
        वापस आकार(a.rows, a.cols);
    अगर( isSolve(*यह) )
        वापस आकार(b.cols, a.cols);
    अगर( isInitializer(*यह) )
        वापस a.size();
    वापस op ? op->size(*यह) : आकार();
}


पूर्णांक Mat_व्यंजक::type() स्थिर
{
    CV_INSTRUMENT_REGION();

    अगर( isInitializer(*यह) )
        वापस a.type();
    अगर( isCmp(*यह) )
        वापस CV_MAKETYPE(CV_8U, a.channels());
    वापस op ? op->type(*यह) : -1;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////

व्योम MatOp_Identity::assign(स्थिर Mat_व्यंजक& e, Mat& m, पूर्णांक _type) स्थिर
{
    अगर( _type == -1 || _type == e.a.type() )
        m = e.a;
    अन्यथा
    {
        CV_Assert( CV_MAT_CN(_type) == e.a.channels() );
        e.a.convertTo(m, _type);
    }
}

अंतरभूत व्योम MatOp_Identity::make_व्यंजक(Mat_व्यंजक& res, स्थिर Mat& m)
{
    res = Mat_व्यंजक(&g_MatOp_Identity, 0, m, Mat(), Mat(), 1, 0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

व्योम MatOp_AddEx::assign(स्थिर Mat_व्यंजक& e, Mat& m, पूर्णांक _type) स्थिर
{
    Mat अस्थायी_2, &dst = _type == -1 || e.a.type() == _type ? m : अस्थायी_2;
    अगर( e.b.तथ्य )
    {
        अगर( e.s == Scalar() || !e.s.isReal() )
        {
            अगर( e.alpha == 1 )
            {
                अगर( e.beta == 1 )
                    cv::add(e.a, e.b, dst);
                अन्यथा अगर( e.beta == -1 )
                    cv::subtract(e.a, e.b, dst);
                अन्यथा
                    cv::scaleAdd(e.b, e.beta, e.a, dst);
            }
            अन्यथा अगर( e.beta == 1 )
            {
                अगर( e.alpha == -1 )
                    cv::subtract(e.b, e.a, dst);
                अन्यथा
                    cv::scaleAdd(e.a, e.alpha, e.b, dst);
            }
            अन्यथा
                cv::addWeighted(e.a, e.alpha, e.b, e.beta, 0, dst);

            अगर( !e.s.isReal() )
                cv::add(dst, e.s, dst);
        }
        अन्यथा
        {
            अगर (e.a.channels() > 1)
                CV_LOG_ONCE_WARNING(शून्य, "OpenCV/MatExpr: processing of multi-channel arrays might be changed in the future: "
                                          "https://github.com/opencv/opencv/issues/16739");
            cv::addWeighted(e.a, e.alpha, e.b, e.beta, e.s[0], dst);
        }
    }
    अन्यथा अगर( e.s.isReal() && (dst.तथ्य != m.तथ्य || भ_असल(e.alpha) != 1))
    {
        अगर (e.a.channels() > 1 && e.s[0] != 0.0)
            CV_LOG_ONCE_WARNING(शून्य, "OpenCV/MatExpr: processing of multi-channel arrays might be changed in the future: "
                                      "https://github.com/opencv/opencv/issues/16739");
        e.a.convertTo(m, _type, e.alpha, e.s[0]);
        वापस;
    }
    अन्यथा अगर( e.alpha == 1 )
        cv::add(e.a, e.s, dst);
    अन्यथा अगर( e.alpha == -1 )
        cv::subtract(e.s, e.a, dst);
    अन्यथा
    {
        e.a.convertTo(dst, e.a.type(), e.alpha);
        cv::add(dst, e.s, dst);
    }

    अगर( dst.तथ्य != m.तथ्य )
        dst.convertTo(m, m.type());
}


व्योम MatOp_AddEx::add(स्थिर Mat_व्यंजक& e, स्थिर Scalar& s, Mat_व्यंजक& res) स्थिर
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.s += s;
}


व्योम MatOp_AddEx::subtract(स्थिर Scalar& s, स्थिर Mat_व्यंजक& e, Mat_व्यंजक& res) स्थिर
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha = -res.alpha;
    res.beta = -res.beta;
    res.s = s - res.s;
}

व्योम MatOp_AddEx::multiply(स्थिर Mat_व्यंजक& e, द्विगुन s, Mat_व्यंजक& res) स्थिर
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha *= s;
    res.beta *= s;
    res.s *= s;
}

व्योम MatOp_AddEx::divide(द्विगुन s, स्थिर Mat_व्यंजक& e, Mat_व्यंजक& res) स्थिर
{
    CV_INSTRUMENT_REGION();

    अगर( isScaled(e) )
        MatOp_Bin::make_व्यंजक(res, '/', e.a, Mat(), s/e.alpha);
    अन्यथा
        MatOp::divide(s, e, res);
}


व्योम MatOp_AddEx::transpose(स्थिर Mat_व्यंजक& e, Mat_व्यंजक& res) स्थिर
{
    CV_INSTRUMENT_REGION();

    अगर( isScaled(e) )
        MatOp_T::make_व्यंजक(res, e.a, e.alpha);
    अन्यथा
        MatOp::transpose(e, res);
}

व्योम MatOp_AddEx::असल(स्थिर Mat_व्यंजक& e, Mat_व्यंजक& res) स्थिर
{
    CV_INSTRUMENT_REGION();

    अगर( (!e.b.तथ्य || e.beta == 0) && भ_असल(e.alpha) == 1 )
        MatOp_Bin::make_व्यंजक(res, 'a', e.a, -e.s*e.alpha);
    अन्यथा अगर( e.b.तथ्य && e.alpha + e.beta == 0 && e.alpha*e.beta == -1 )
        MatOp_Bin::make_व्यंजक(res, 'a', e.a, e.b);
    अन्यथा
        MatOp::असल(e, res);
}

अंतरभूत व्योम MatOp_AddEx::make_व्यंजक(Mat_व्यंजक& res, स्थिर Mat& a, स्थिर Mat& b, द्विगुन alpha, द्विगुन beta, स्थिर Scalar& s)
{
    res = Mat_व्यंजक(&g_MatOp_AddEx, 0, a, b, Mat(), alpha, beta, s);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

व्योम MatOp_Bin::assign(स्थिर Mat_व्यंजक& e, Mat& m, पूर्णांक _type) स्थिर
{
    Mat अस्थायी_2, &dst = _type == -1 || e.a.type() == _type ? m : अस्थायी_2;

    अगर( e.flags == '*' )
        cv::multiply(e.a, e.b, dst, e.alpha);
    अन्यथा अगर( e.flags == '/' && e.b.तथ्य )
        cv::divide(e.a, e.b, dst, e.alpha);
    अन्यथा अगर( e.flags == '/' && !e.b.तथ्य )
        cv::divide(e.alpha, e.a, dst );
    अन्यथा अगर( e.flags == '&' && e.b.तथ्य )
        bitwise_and(e.a, e.b, dst);
    अन्यथा अगर( e.flags == '&' && !e.b.तथ्य )
        bitwise_and(e.a, e.s, dst);
    अन्यथा अगर( e.flags == '|' && e.b.तथ्य )
        bitwise_or(e.a, e.b, dst);
    अन्यथा अगर( e.flags == '|' && !e.b.तथ्य )
        bitwise_or(e.a, e.s, dst);
    अन्यथा अगर( e.flags == '^' && e.b.तथ्य )
        bitwise_xor(e.a, e.b, dst);
    अन्यथा अगर( e.flags == '^' && !e.b.तथ्य )
        bitwise_xor(e.a, e.s, dst);
    अन्यथा अगर( e.flags == '~' && !e.b.तथ्य )
        bitwise_not(e.a, dst);
    अन्यथा अगर( e.flags == 'm' )
        cv::min(e.a, e.b, dst);
    अन्यथा अगर( e.flags == 'n' )
        cv::min(e.a, e.s[0], dst);
    अन्यथा अगर( e.flags == 'M' )
        cv::max(e.a, e.b, dst);
    अन्यथा अगर( e.flags == 'N' )
        cv::max(e.a, e.s[0], dst);
    अन्यथा अगर( e.flags == 'a' && e.b.तथ्य )
        cv::absdiff(e.a, e.b, dst);
    अन्यथा अगर( e.flags == 'a' && !e.b.तथ्य )
        cv::absdiff(e.a, e.s, dst);
    अन्यथा
        CV_Error(cv::Error::StsError, "Unknown operation");

    अगर( dst.तथ्य != m.तथ्य )
        dst.convertTo(m, _type);
}

व्योम MatOp_Bin::multiply(स्थिर Mat_व्यंजक& e, द्विगुन s, Mat_व्यंजक& res) स्थिर
{
    CV_INSTRUMENT_REGION();

    अगर( e.flags == '*' || e.flags == '/' )
    {
        res = e;
        res.alpha *= s;
    }
    अन्यथा
        MatOp::multiply(e, s, res);
}

व्योम MatOp_Bin::divide(द्विगुन s, स्थिर Mat_व्यंजक& e, Mat_व्यंजक& res) स्थिर
{
    CV_INSTRUMENT_REGION();

    अगर( e.flags == '/' && (!e.b.तथ्य || e.beta == 0) )
        MatOp_AddEx::make_व्यंजक(res, e.a, Mat(), s/e.alpha, 0);
    अन्यथा
        MatOp::divide(s, e, res);
}

अंतरभूत व्योम MatOp_Bin::make_व्यंजक(Mat_व्यंजक& res, अक्षर op, स्थिर Mat& a, स्थिर Mat& b, द्विगुन scale)
{
    res = Mat_व्यंजक(&g_MatOp_Bin, op, a, b, Mat(), scale, b.तथ्य ? 1 : 0);
}

अंतरभूत व्योम MatOp_Bin::make_व्यंजक(Mat_व्यंजक& res, अक्षर op, स्थिर Mat& a, स्थिर Scalar& s)
{
    res = Mat_व्यंजक(&g_MatOp_Bin, op, a, Mat(), Mat(), 1, 0, s);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

व्योम MatOp_Cmp::assign(स्थिर Mat_व्यंजक& e, Mat& m, पूर्णांक _type) स्थिर
{
    Mat अस्थायी_2, &dst = _type == -1 || _type == CV_8U ? m : अस्थायी_2;

    अगर( e.b.तथ्य )
        cv::compare(e.a, e.b, dst, e.flags);
    अन्यथा
        cv::compare(e.a, e.alpha, dst, e.flags);

    अगर( dst.तथ्य != m.तथ्य )
        dst.convertTo(m, _type);
}

अंतरभूत व्योम MatOp_Cmp::make_व्यंजक(Mat_व्यंजक& res, पूर्णांक cmpop, स्थिर Mat& a, स्थिर Mat& b)
{
    res = Mat_व्यंजक(&g_MatOp_Cmp, cmpop, a, b, Mat(), 1, 1);
}

अंतरभूत व्योम MatOp_Cmp::make_व्यंजक(Mat_व्यंजक& res, पूर्णांक cmpop, स्थिर Mat& a, द्विगुन alpha)
{
    res = Mat_व्यंजक(&g_MatOp_Cmp, cmpop, a, Mat(), Mat(), alpha, 1);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

व्योम MatOp_T::assign(स्थिर Mat_व्यंजक& e, Mat& m, पूर्णांक _type) स्थिर
{
    Mat अस्थायी_2, &dst = _type == -1 || _type == e.a.type() ? m : अस्थायी_2;

    cv::transpose(e.a, dst);

    अगर( dst.तथ्य != m.तथ्य || e.alpha != 1 )
        dst.convertTo(m, _type, e.alpha);
}

व्योम MatOp_T::multiply(स्थिर Mat_व्यंजक& e, द्विगुन s, Mat_व्यंजक& res) स्थिर
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha *= s;
}

व्योम MatOp_T::transpose(स्थिर Mat_व्यंजक& e, Mat_व्यंजक& res) स्थिर
{
    CV_INSTRUMENT_REGION();

    अगर( e.alpha == 1 )
        MatOp_Identity::make_व्यंजक(res, e.a);
    अन्यथा
        MatOp_AddEx::make_व्यंजक(res, e.a, Mat(), e.alpha, 0);
}

अंतरभूत व्योम MatOp_T::make_व्यंजक(Mat_व्यंजक& res, स्थिर Mat& a, द्विगुन alpha)
{
    res = Mat_व्यंजक(&g_MatOp_T, 0, a, Mat(), Mat(), alpha, 0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

व्योम MatOp_GEMM::assign(स्थिर Mat_व्यंजक& e, Mat& m, पूर्णांक _type) स्थिर
{
    Mat अस्थायी_2, &dst = _type == -1 || _type == e.a.type() ? m : अस्थायी_2;

    cv::gemm(e.a, e.b, e.alpha, e.c, e.beta, dst, e.flags);
    अगर( dst.तथ्य != m.तथ्य )
        dst.convertTo(m, _type);
}

व्योम MatOp_GEMM::add(स्थिर Mat_व्यंजक& e1, स्थिर Mat_व्यंजक& e2, Mat_व्यंजक& res) स्थिर
{
    CV_INSTRUMENT_REGION();

    bool i1 = isIdentity(e1), i2 = isIdentity(e2);
    द्विगुन alpha1 = i1 ? 1 : e1.alpha, alpha2 = i2 ? 1 : e2.alpha;

    अगर( isMatProd(e1) && (i2 || isScaled(e2) || isT(e2)) )
        MatOp_GEMM::make_व्यंजक(res, (e1.flags & ~cv::GEMM_3_T)|(isT(e2) ? cv::GEMM_3_T : 0),
                             e1.a, e1.b, alpha1, e2.a, alpha2);
    अन्यथा अगर( isMatProd(e2) && (i1 || isScaled(e1) || isT(e1)) )
        MatOp_GEMM::make_व्यंजक(res, (e2.flags & ~cv::GEMM_3_T)|(isT(e1) ? cv::GEMM_3_T : 0),
                             e2.a, e2.b, alpha2, e1.a, alpha1);
    अन्यथा अगर( यह == e2.op )
        MatOp::add(e1, e2, res);
    अन्यथा
        e2.op->add(e1, e2, res);
}

व्योम MatOp_GEMM::subtract(स्थिर Mat_व्यंजक& e1, स्थिर Mat_व्यंजक& e2, Mat_व्यंजक& res) स्थिर
{
    CV_INSTRUMENT_REGION();

    bool i1 = isIdentity(e1), i2 = isIdentity(e2);
    द्विगुन alpha1 = i1 ? 1 : e1.alpha, alpha2 = i2 ? 1 : e2.alpha;

    अगर( isMatProd(e1) && (i2 || isScaled(e2) || isT(e2)) )
        MatOp_GEMM::make_व्यंजक(res, (e1.flags & ~cv::GEMM_3_T)|(isT(e2) ? cv::GEMM_3_T : 0),
                             e1.a, e1.b, alpha1, e2.a, -alpha2);
    अन्यथा अगर( isMatProd(e2) && (i1 || isScaled(e1) || isT(e1)) )
        MatOp_GEMM::make_व्यंजक(res, (e2.flags & ~cv::GEMM_3_T)|(isT(e1) ? cv::GEMM_3_T : 0),
                            e2.a, e2.b, -alpha2, e1.a, alpha1);
    अन्यथा अगर( यह == e2.op )
        MatOp::subtract(e1, e2, res);
    अन्यथा
        e2.op->subtract(e1, e2, res);
}

व्योम MatOp_GEMM::multiply(स्थिर Mat_व्यंजक& e, द्विगुन s, Mat_व्यंजक& res) स्थिर
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha *= s;
    res.beta *= s;
}

व्योम MatOp_GEMM::transpose(स्थिर Mat_व्यंजक& e, Mat_व्यंजक& res) स्थिर
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.flags = (!(e.flags & cv::GEMM_1_T) ? cv::GEMM_2_T : 0) |
                (!(e.flags & cv::GEMM_2_T) ? cv::GEMM_1_T : 0) |
                (!(e.flags & cv::GEMM_3_T) ? cv::GEMM_3_T : 0);
    swap(res.a, res.b);
}

अंतरभूत व्योम MatOp_GEMM::make_व्यंजक(Mat_व्यंजक& res, पूर्णांक flags, स्थिर Mat& a, स्थिर Mat& b,
                                 द्विगुन alpha, स्थिर Mat& c, द्विगुन beta)
{
    res = Mat_व्यंजक(&g_MatOp_GEMM, flags, a, b, c, alpha, beta);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

व्योम MatOp_Invert::assign(स्थिर Mat_व्यंजक& e, Mat& m, पूर्णांक _type) स्थिर
{
    Mat अस्थायी_2, &dst = _type == -1 || _type == e.a.type() ? m : अस्थायी_2;

    cv::invert(e.a, dst, e.flags);
    अगर( dst.तथ्य != m.तथ्य )
        dst.convertTo(m, _type);
}

व्योम MatOp_Invert::matmul(स्थिर Mat_व्यंजक& e1, स्थिर Mat_व्यंजक& e2, Mat_व्यंजक& res) स्थिर
{
    अगर( isInv(e1) && isIdentity(e2) )
        MatOp_Solve::make_व्यंजक(res, e1.flags, e1.a, e2.a);
    अन्यथा अगर( यह == e2.op )
        MatOp::matmul(e1, e2, res);
    अन्यथा
        e2.op->matmul(e1, e2, res);
}

अंतरभूत व्योम MatOp_Invert::make_व्यंजक(Mat_व्यंजक& res, पूर्णांक method, स्थिर Mat& m)
{
    res = Mat_व्यंजक(&g_MatOp_Invert, method, m, Mat(), Mat(), 1, 0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

व्योम MatOp_Solve::assign(स्थिर Mat_व्यंजक& e, Mat& m, पूर्णांक _type) स्थिर
{
    Mat अस्थायी_2, &dst = _type == -1 || _type == e.a.type() ? m : अस्थायी_2;

    cv::solve(e.a, e.b, dst, e.flags);
    अगर( dst.तथ्य != m.तथ्य )
        dst.convertTo(m, _type);
}

अंतरभूत व्योम MatOp_Solve::make_व्यंजक(Mat_व्यंजक& res, पूर्णांक method, स्थिर Mat& a, स्थिर Mat& b)
{
    res = Mat_व्यंजक(&g_MatOp_Solve, method, a, b, Mat(), 1, 1);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

व्योम MatOp_Initializer::assign(स्थिर Mat_व्यंजक& e, Mat& m, पूर्णांक _type) स्थिर
{
    अगर( _type == -1 )
        _type = e.a.type();

    m.create(e.a.size, _type);

    अगर( e.flags == 'I' && e.a.dims <= 2 )
        setIdentity(m, Scalar(e.alpha));
    अन्यथा अगर( e.flags == '0' )
        m = Scalar();
    अन्यथा अगर( e.flags == '1' )
        m = Scalar(e.alpha);
    अन्यथा
        CV_Error(cv::Error::StsError, "Invalid matrix initializer type");
}

व्योम MatOp_Initializer::multiply(स्थिर Mat_व्यंजक& e, द्विगुन s, Mat_व्यंजक& res) स्थिर
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha *= s;
}

अंतरभूत व्योम MatOp_Initializer::make_व्यंजक(Mat_व्यंजक& res, पूर्णांक method, आकार sz, पूर्णांक type, द्विगुन alpha)
{
    res = Mat_व्यंजक(getGlobalMatOpInitializer(), method, Mat(sz, type, (व्योम*)(माप_प्रकार)0xEEEEEEEE), Mat(), Mat(), alpha, 0);
}

अंतरभूत व्योम MatOp_Initializer::make_व्यंजक(Mat_व्यंजक& res, पूर्णांक method, पूर्णांक ndims, स्थिर पूर्णांक* sizes, पूर्णांक type, द्विगुन alpha)
{
    res = Mat_व्यंजक(getGlobalMatOpInitializer(), method, Mat(ndims, sizes, type, (व्योम*)(माप_प्रकार)0xEEEEEEEE), Mat(), Mat(), alpha, 0);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

Mat_व्यंजक Mat::t() स्थिर
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(*यह);
    Mat_व्यंजक e;
    MatOp_T::make_व्यंजक(e, *यह);
    वापस e;
}

Mat_व्यंजक Mat::inv(पूर्णांक method) स्थिर
{
    CV_INSTRUMENT_REGION();

    Mat_व्यंजक e;
    MatOp_Invert::make_व्यंजक(e, method, *यह);
    वापस e;
}


Mat_व्यंजक Mat::mul(निवेश_Array m, द्विगुन scale) स्थिर
{
    CV_INSTRUMENT_REGION();

    Mat b = m.getMat();
    // Unless the argument is a refcounted Mat/UMat, the header returned by getMat() may be
    // a non-owning view of caller memory (e.g. a scalar bound to _InputArray(const double&),
    // a Matx or a Vec) that the returned MatExpr can outlive, so snapshot it.
    // See https://github.com/opencv/opencv/issues/23577
    अगर( !m.isMat() && !m.isUMat() )
        b = b.clone();

    Mat_व्यंजक e;
    MatOp_Bin::make_व्यंजक(e, '*', *यह, b, scale);
    वापस e;
}

Mat_व्यंजक Mat::zeros(पूर्णांक rows, पूर्णांक cols, पूर्णांक type)
{
    CV_INSTRUMENT_REGION();

    Mat_व्यंजक e;
    MatOp_Initializer::make_व्यंजक(e, '0', आकार(cols, rows), type);
    वापस e;
}

Mat_व्यंजक Mat::zeros(आकार size, पूर्णांक type)
{
    CV_INSTRUMENT_REGION();

    Mat_व्यंजक e;
    MatOp_Initializer::make_व्यंजक(e, '0', size, type);
    वापस e;
}

Mat_व्यंजक Mat::zeros(पूर्णांक ndims, स्थिर पूर्णांक* sizes, पूर्णांक type)
{
    CV_INSTRUMENT_REGION();

    Mat_व्यंजक e;
    MatOp_Initializer::make_व्यंजक(e, '0', ndims, sizes, type);
    वापस e;
}

Mat_व्यंजक Mat::zeros(स्थिर MatShape& shape, पूर्णांक type)
{
    CV_INSTRUMENT_REGION();

    Mat_व्यंजक e;
    MatOp_Initializer::make_व्यंजक(e, '0', shape.dims, shape.p, type);
    वापस e;
}

Mat_व्यंजक Mat::ones(पूर्णांक rows, पूर्णांक cols, पूर्णांक type)
{
    CV_INSTRUMENT_REGION();

    Mat_व्यंजक e;
    MatOp_Initializer::make_व्यंजक(e, '1', आकार(cols, rows), type);
    वापस e;
}

Mat_व्यंजक Mat::ones(आकार size, पूर्णांक type)
{
    CV_INSTRUMENT_REGION();

    Mat_व्यंजक e;
    MatOp_Initializer::make_व्यंजक(e, '1', size, type);
    वापस e;
}

Mat_व्यंजक Mat::ones(पूर्णांक ndims, स्थिर पूर्णांक* sizes, पूर्णांक type)
{
    CV_INSTRUMENT_REGION();

    Mat_व्यंजक e;
    MatOp_Initializer::make_व्यंजक(e, '1', ndims, sizes, type);
    वापस e;
}

Mat_व्यंजक Mat::ones(स्थिर MatShape& shape, पूर्णांक type)
{
    CV_INSTRUMENT_REGION();

    Mat_व्यंजक e;
    MatOp_Initializer::make_व्यंजक(e, '1', shape.dims, shape.p, type);
    वापस e;
}

Mat_व्यंजक Mat::eye(पूर्णांक rows, पूर्णांक cols, पूर्णांक type)
{
    CV_INSTRUMENT_REGION();

    Mat_व्यंजक e;
    MatOp_Initializer::make_व्यंजक(e, 'I', आकार(cols, rows), type);
    वापस e;
}

Mat_व्यंजक Mat::eye(आकार size, पूर्णांक type)
{
    CV_INSTRUMENT_REGION();

    Mat_व्यंजक e;
    MatOp_Initializer::make_व्यंजक(e, 'I', size, type);
    वापस e;
}

व्योम Mat_व्यंजक::swap(Mat_व्यंजक& other)
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

निवेश_Array_2::निवेश_Array_2(स्थिर Mat_व्यंजक& व्यंजक)
{
    अगर (!isIdentity(व्यंजक))
    {
        Mat परिणाम = व्यंजक;  // TODO improve through refcount == 1 of expr.a (inplace operation is possible - except gemm?)
        Mat_व्यंजक परिणाम_व्यंजक(परिणाम);
        swap(const_cast<Mat_व्यंजक&>(व्यंजक), परिणाम_व्यंजक);
    }
    CV_Assert(isIdentity(व्यंजक));
    init(FIXED_TYPE + FIXED_आकार + MAT + ACCESS_READ, &व्यंजक.a);
}

} // cv::
