// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


/* ////////////////////////////////////////////////////////////////////
//
//  Mat basic operations: Copy, Set
//
// */

#अन्तर्भाव "precomp.hpp"
#अन्तर्भाव <opencv2/core/utils/logger.hpp>

नामाकाश cv
{

//This and its overload below are used in various MatExpr operator overloads
//implemented to check that Matrix operands exist.
स्थिर शून्य checkOperandsExist(स्थिरांक Mat& a)
{
    जर (a.empty())
    {
        CV_Error(cv::Error::StsBadArg, "Matrix operand is an empty matrix.");
    }
}

स्थिर शून्य checkOperandsExist(स्थिरांक Mat& a, स्थिरांक Mat& b)
{
    जर (a.empty() || b.empty())
    {
        CV_Error(cv::Error::StsBadArg, "One or more matrix operands are empty.");
    }
}


वर्ग MatOp_Identity CV_FINAL : सार्वजनिक MatOp
{
सार्वजनिक:
    MatOp_Identity() {}
    आभासी ~MatOp_Identity() {}

    तार्किक elementWise(स्थिरांक Mat_अभिव्यक्ती& /*अभिव्यक्ती*/) स्थिरांक CV_OVERRIDE { परत सत्य; }
    शून्य assign(स्थिरांक Mat_अभिव्यक्ती& अभिव्यक्ती, Mat& m, पूर्णांक type=-1) स्थिरांक CV_OVERRIDE;

    स्थिर शून्य make_अभिव्यक्ती(Mat_अभिव्यक्ती& res, स्थिरांक Mat& m);
};

स्थिर MatOp_Identity g_MatOp_Identity;

वर्ग MatOp_AddEx CV_FINAL : सार्वजनिक MatOp
{
सार्वजनिक:
    MatOp_AddEx() {}
    आभासी ~MatOp_AddEx() {}

    तार्किक elementWise(स्थिरांक Mat_अभिव्यक्ती& /*अभिव्यक्ती*/) स्थिरांक CV_OVERRIDE { परत सत्य; }
    शून्य assign(स्थिरांक Mat_अभिव्यक्ती& अभिव्यक्ती, Mat& m, पूर्णांक type=-1) स्थिरांक CV_OVERRIDE;

    शून्य add(स्थिरांक Mat_अभिव्यक्ती& e1, स्थिरांक Scalar& s, Mat_अभिव्यक्ती& res) स्थिरांक CV_OVERRIDE;
    शून्य subtract(स्थिरांक Scalar& s, स्थिरांक Mat_अभिव्यक्ती& अभिव्यक्ती, Mat_अभिव्यक्ती& res) स्थिरांक CV_OVERRIDE;
    शून्य multiply(स्थिरांक Mat_अभिव्यक्ती& e1, द्विगुण s, Mat_अभिव्यक्ती& res) स्थिरांक CV_OVERRIDE;
    शून्य divide(द्विगुण s, स्थिरांक Mat_अभिव्यक्ती& e, Mat_अभिव्यक्ती& res) स्थिरांक CV_OVERRIDE;

    शून्य transpose(स्थिरांक Mat_अभिव्यक्ती& e1, Mat_अभिव्यक्ती& res) स्थिरांक CV_OVERRIDE;
    शून्य असल(स्थिरांक Mat_अभिव्यक्ती& अभिव्यक्ती, Mat_अभिव्यक्ती& res) स्थिरांक CV_OVERRIDE;

    स्थिर शून्य make_अभिव्यक्ती(Mat_अभिव्यक्ती& res, स्थिरांक Mat& a, स्थिरांक Mat& b, द्विगुण alpha, द्विगुण beta, स्थिरांक Scalar& s=Scalar());
};

स्थिर MatOp_AddEx g_MatOp_AddEx;

वर्ग MatOp_Bin CV_FINAL : सार्वजनिक MatOp
{
सार्वजनिक:
    MatOp_Bin() {}
    आभासी ~MatOp_Bin() {}

    तार्किक elementWise(स्थिरांक Mat_अभिव्यक्ती& /*अभिव्यक्ती*/) स्थिरांक CV_OVERRIDE { परत सत्य; }
    शून्य assign(स्थिरांक Mat_अभिव्यक्ती& अभिव्यक्ती, Mat& m, पूर्णांक type=-1) स्थिरांक CV_OVERRIDE;

    शून्य multiply(स्थिरांक Mat_अभिव्यक्ती& e1, द्विगुण s, Mat_अभिव्यक्ती& res) स्थिरांक CV_OVERRIDE;
    शून्य divide(द्विगुण s, स्थिरांक Mat_अभिव्यक्ती& e, Mat_अभिव्यक्ती& res) स्थिरांक CV_OVERRIDE;

    स्थिर शून्य make_अभिव्यक्ती(Mat_अभिव्यक्ती& res, अक्षर op, स्थिरांक Mat& a, स्थिरांक Mat& b, द्विगुण scale=1);
    स्थिर शून्य make_अभिव्यक्ती(Mat_अभिव्यक्ती& res, अक्षर op, स्थिरांक Mat& a, स्थिरांक Scalar& s);
};

स्थिर MatOp_Bin g_MatOp_Bin;

वर्ग MatOp_Cmp CV_FINAL : सार्वजनिक MatOp
{
सार्वजनिक:
    MatOp_Cmp() {}
    आभासी ~MatOp_Cmp() {}

    तार्किक elementWise(स्थिरांक Mat_अभिव्यक्ती& /*अभिव्यक्ती*/) स्थिरांक CV_OVERRIDE { परत सत्य; }
    शून्य assign(स्थिरांक Mat_अभिव्यक्ती& अभिव्यक्ती, Mat& m, पूर्णांक type=-1) स्थिरांक CV_OVERRIDE;

    स्थिर शून्य make_अभिव्यक्ती(Mat_अभिव्यक्ती& res, पूर्णांक cmpop, स्थिरांक Mat& a, स्थिरांक Mat& b);
    स्थिर शून्य make_अभिव्यक्ती(Mat_अभिव्यक्ती& res, पूर्णांक cmpop, स्थिरांक Mat& a, द्विगुण alpha);
};

स्थिर MatOp_Cmp g_MatOp_Cmp;

वर्ग MatOp_GEMM CV_FINAL : सार्वजनिक MatOp
{
सार्वजनिक:
    MatOp_GEMM() {}
    आभासी ~MatOp_GEMM() {}

    तार्किक elementWise(स्थिरांक Mat_अभिव्यक्ती& /*अभिव्यक्ती*/) स्थिरांक CV_OVERRIDE { परत असत्य; }
    शून्य assign(स्थिरांक Mat_अभिव्यक्ती& अभिव्यक्ती, Mat& m, पूर्णांक type=-1) स्थिरांक CV_OVERRIDE;

    शून्य add(स्थिरांक Mat_अभिव्यक्ती& e1, स्थिरांक Mat_अभिव्यक्ती& e2, Mat_अभिव्यक्ती& res) स्थिरांक CV_OVERRIDE;
    शून्य subtract(स्थिरांक Mat_अभिव्यक्ती& e1, स्थिरांक Mat_अभिव्यक्ती& e2, Mat_अभिव्यक्ती& res) स्थिरांक CV_OVERRIDE;
    शून्य multiply(स्थिरांक Mat_अभिव्यक्ती& e, द्विगुण s, Mat_अभिव्यक्ती& res) स्थिरांक CV_OVERRIDE;

    शून्य transpose(स्थिरांक Mat_अभिव्यक्ती& अभिव्यक्ती, Mat_अभिव्यक्ती& res) स्थिरांक CV_OVERRIDE;

    Size size(स्थिरांक Mat_अभिव्यक्ती& अभिव्यक्ती) स्थिरांक CV_OVERRIDE
    {
        परत Size(
            (अभिव्यक्ती.flags & GEMM_2_T) ? अभिव्यक्ती.b.rows : अभिव्यक्ती.b.cols,
            (अभिव्यक्ती.flags & GEMM_1_T) ? अभिव्यक्ती.a.cols : अभिव्यक्ती.a.rows
        );
    }

    स्थिर शून्य make_अभिव्यक्ती(Mat_अभिव्यक्ती& res, पूर्णांक flags, स्थिरांक Mat& a, स्थिरांक Mat& b,
                         द्विगुण alpha=1, स्थिरांक Mat& c=Mat(), द्विगुण beta=1);

};

स्थिर MatOp_GEMM g_MatOp_GEMM;

वर्ग MatOp_Invert CV_FINAL : सार्वजनिक MatOp
{
सार्वजनिक:
    MatOp_Invert() {}
    आभासी ~MatOp_Invert() {}

    तार्किक elementWise(स्थिरांक Mat_अभिव्यक्ती& /*अभिव्यक्ती*/) स्थिरांक CV_OVERRIDE { परत असत्य; }
    शून्य assign(स्थिरांक Mat_अभिव्यक्ती& अभिव्यक्ती, Mat& m, पूर्णांक type=-1) स्थिरांक CV_OVERRIDE;

    शून्य matmul(स्थिरांक Mat_अभिव्यक्ती& expr1, स्थिरांक Mat_अभिव्यक्ती& expr2, Mat_अभिव्यक्ती& res) स्थिरांक CV_OVERRIDE;

    स्थिर शून्य make_अभिव्यक्ती(Mat_अभिव्यक्ती& res, पूर्णांक method, स्थिरांक Mat& m);
};

स्थिर MatOp_Invert g_MatOp_Invert;

वर्ग MatOp_T CV_FINAL : सार्वजनिक MatOp
{
सार्वजनिक:
    MatOp_T() {}
    आभासी ~MatOp_T() {}

    तार्किक elementWise(स्थिरांक Mat_अभिव्यक्ती& /*अभिव्यक्ती*/) स्थिरांक CV_OVERRIDE { परत असत्य; }
    शून्य assign(स्थिरांक Mat_अभिव्यक्ती& अभिव्यक्ती, Mat& m, पूर्णांक type=-1) स्थिरांक CV_OVERRIDE;

    शून्य multiply(स्थिरांक Mat_अभिव्यक्ती& e1, द्विगुण s, Mat_अभिव्यक्ती& res) स्थिरांक CV_OVERRIDE;
    शून्य transpose(स्थिरांक Mat_अभिव्यक्ती& अभिव्यक्ती, Mat_अभिव्यक्ती& res) स्थिरांक CV_OVERRIDE;

    स्थिर शून्य make_अभिव्यक्ती(Mat_अभिव्यक्ती& res, स्थिरांक Mat& a, द्विगुण alpha=1);
};

स्थिर MatOp_T g_MatOp_T;

वर्ग MatOp_Solve CV_FINAL : सार्वजनिक MatOp
{
सार्वजनिक:
    MatOp_Solve() {}
    आभासी ~MatOp_Solve() {}

    तार्किक elementWise(स्थिरांक Mat_अभिव्यक्ती& /*अभिव्यक्ती*/) स्थिरांक CV_OVERRIDE { परत असत्य; }
    शून्य assign(स्थिरांक Mat_अभिव्यक्ती& अभिव्यक्ती, Mat& m, पूर्णांक type=-1) स्थिरांक CV_OVERRIDE;

    स्थिर शून्य make_अभिव्यक्ती(Mat_अभिव्यक्ती& res, पूर्णांक method, स्थिरांक Mat& a, स्थिरांक Mat& b);
};

स्थिर MatOp_Solve g_MatOp_Solve;

वर्ग MatOp_Initializer CV_FINAL : सार्वजनिक MatOp
{
सार्वजनिक:
    MatOp_Initializer() {}
    आभासी ~MatOp_Initializer() {}

    तार्किक elementWise(स्थिरांक Mat_अभिव्यक्ती& /*अभिव्यक्ती*/) स्थिरांक CV_OVERRIDE { परत असत्य; }
    शून्य assign(स्थिरांक Mat_अभिव्यक्ती& अभिव्यक्ती, Mat& m, पूर्णांक type=-1) स्थिरांक CV_OVERRIDE;

    शून्य multiply(स्थिरांक Mat_अभिव्यक्ती& e, द्विगुण s, Mat_अभिव्यक्ती& res) स्थिरांक CV_OVERRIDE;

    स्थिर शून्य make_अभिव्यक्ती(Mat_अभिव्यक्ती& res, पूर्णांक method, Size sz, पूर्णांक type, द्विगुण alpha=1);
    स्थिर शून्य make_अभिव्यक्ती(Mat_अभिव्यक्ती& res, पूर्णांक method, पूर्णांक ndims, स्थिरांक पूर्णांक* sizes, पूर्णांक type, द्विगुण alpha=1);
};

स्थिर MatOp_Initializer* getGlobalMatOpInitializer()
{
    CV_SINGLETON_LAZY_INIT(MatOp_Initializer, नवीन MatOp_Initializer())
}

स्थिर ओळीत तार्किक isIdentity(स्थिरांक Mat_अभिव्यक्ती& e) { परत e.op == &g_MatOp_Identity; }
स्थिर ओळीत तार्किक isAddEx(स्थिरांक Mat_अभिव्यक्ती& e) { परत e.op == &g_MatOp_AddEx; }
स्थिर ओळीत तार्किक isScaled(स्थिरांक Mat_अभिव्यक्ती& e) { परत isAddEx(e) && (!e.b.data || e.beta == 0) && e.s == Scalar(); }
स्थिर ओळीत तार्किक isBin(स्थिरांक Mat_अभिव्यक्ती& e, अक्षर c) { परत e.op == &g_MatOp_Bin && e.flags == c; }
स्थिर ओळीत तार्किक isCmp(स्थिरांक Mat_अभिव्यक्ती& e) { परत e.op == &g_MatOp_Cmp; }
स्थिर ओळीत तार्किक isReciprocal(स्थिरांक Mat_अभिव्यक्ती& e) { परत isBin(e,'/') && (!e.b.data || e.beta == 0); }
स्थिर ओळीत तार्किक isT(स्थिरांक Mat_अभिव्यक्ती& e) { परत e.op == &g_MatOp_T; }
स्थिर ओळीत तार्किक isInv(स्थिरांक Mat_अभिव्यक्ती& e) { परत e.op == &g_MatOp_Invert; }
स्थिर ओळीत तार्किक isSolve(स्थिरांक Mat_अभिव्यक्ती& e) { परत e.op == &g_MatOp_Solve; }
//static inline bool isGEMM(const MatExpr& e) { return e.op == &g_MatOp_GEMM; }
स्थिर ओळीत तार्किक isMatProd(स्थिरांक Mat_अभिव्यक्ती& e) { परत e.op == &g_MatOp_GEMM && (!e.c.data || e.beta == 0); }
स्थिर ओळीत तार्किक isInitializer(स्थिरांक Mat_अभिव्यक्ती& e) { परत e.op == getGlobalMatOpInitializer(); }

/////////////////////////////////////////////////////////////////////////////////////////////////////

MatOp::MatOp() {}
MatOp::~MatOp() {}


तार्किक MatOp::elementWise(स्थिरांक Mat_अभिव्यक्ती& /*अभिव्यक्ती*/) स्थिरांक
{
    परत असत्य;
}

शून्य MatOp::roi(स्थिरांक Mat_अभिव्यक्ती& अभिव्यक्ती, स्थिरांक Range& rowRange, स्थिरांक Range& colRange, Mat_अभिव्यक्ती& e) स्थिरांक
{
    जर( elementWise(अभिव्यक्ती) )
    {
        e = Mat_अभिव्यक्ती(अभिव्यक्ती.op, अभिव्यक्ती.flags, Mat(), Mat(), Mat(),
                    अभिव्यक्ती.alpha, अभिव्यक्ती.beta, अभिव्यक्ती.s);
        जर(अभिव्यक्ती.a.data)
            e.a = अभिव्यक्ती.a(rowRange, colRange);
        जर(अभिव्यक्ती.b.data)
            e.b = अभिव्यक्ती.b(rowRange, colRange);
        जर(अभिव्यक्ती.c.data)
            e.c = अभिव्यक्ती.c(rowRange, colRange);
    }
    अन्यथा
    {
        Mat m;
        अभिव्यक्ती.op->assign(अभिव्यक्ती, m);
        e = Mat_अभिव्यक्ती(&g_MatOp_Identity, 0, m(rowRange, colRange), Mat(), Mat());
    }
}

शून्य MatOp::diag(स्थिरांक Mat_अभिव्यक्ती& अभिव्यक्ती, पूर्णांक d, Mat_अभिव्यक्ती& e) स्थिरांक
{
    जर( elementWise(अभिव्यक्ती) )
    {
        e = Mat_अभिव्यक्ती(अभिव्यक्ती.op, अभिव्यक्ती.flags, Mat(), Mat(), Mat(),
                    अभिव्यक्ती.alpha, अभिव्यक्ती.beta, अभिव्यक्ती.s);
        जर(अभिव्यक्ती.a.data)
            e.a = अभिव्यक्ती.a.diag(d);
        जर(अभिव्यक्ती.b.data)
            e.b = अभिव्यक्ती.b.diag(d);
        जर(अभिव्यक्ती.c.data)
            e.c = अभिव्यक्ती.c.diag(d);
    }
    अन्यथा
    {
        Mat m;
        अभिव्यक्ती.op->assign(अभिव्यक्ती, m);
        e = Mat_अभिव्यक्ती(&g_MatOp_Identity, 0, m.diag(d), Mat(), Mat());
    }
}


शून्य MatOp::augAssignAdd(स्थिरांक Mat_अभिव्यक्ती& अभिव्यक्ती, Mat& m) स्थिरांक
{
    Mat temp;
    अभिव्यक्ती.op->assign(अभिव्यक्ती, temp);
    m += temp;
}


शून्य MatOp::augAssignSubtract(स्थिरांक Mat_अभिव्यक्ती& अभिव्यक्ती, Mat& m) स्थिरांक
{
    Mat temp;
    अभिव्यक्ती.op->assign(अभिव्यक्ती, temp);
    m -= temp;
}


शून्य MatOp::augAssignMultiply(स्थिरांक Mat_अभिव्यक्ती& अभिव्यक्ती, Mat& m) स्थिरांक
{
    Mat temp;
    अभिव्यक्ती.op->assign(अभिव्यक्ती, temp);
    m *= temp;
}


शून्य MatOp::augAssignDivide(स्थिरांक Mat_अभिव्यक्ती& अभिव्यक्ती, Mat& m) स्थिरांक
{
    Mat temp;
    अभिव्यक्ती.op->assign(अभिव्यक्ती, temp);
    m /= temp;
}


शून्य MatOp::augAssignAnd(स्थिरांक Mat_अभिव्यक्ती& अभिव्यक्ती, Mat& m) स्थिरांक
{
    Mat temp;
    अभिव्यक्ती.op->assign(अभिव्यक्ती, temp);
    m &= temp;
}


शून्य MatOp::augAssignOr(स्थिरांक Mat_अभिव्यक्ती& अभिव्यक्ती, Mat& m) स्थिरांक
{
    Mat temp;
    अभिव्यक्ती.op->assign(अभिव्यक्ती, temp);
    m |= temp;
}


शून्य MatOp::augAssignXor(स्थिरांक Mat_अभिव्यक्ती& अभिव्यक्ती, Mat& m) स्थिरांक
{
    Mat temp;
    अभिव्यक्ती.op->assign(अभिव्यक्ती, temp);
    m ^= temp;
}


शून्य MatOp::add(स्थिरांक Mat_अभिव्यक्ती& e1, स्थिरांक Mat_अभिव्यक्ती& e2, Mat_अभिव्यक्ती& res) स्थिरांक
{
    CV_INSTRUMENT_REGION();

    जर( हे == e2.op )
    {
        द्विगुण alpha = 1, beta = 1;
        Scalar s;
        Mat m1, m2;
        जर( isAddEx(e1) && (!e1.b.data || e1.beta == 0) )
        {
            m1 = e1.a;
            alpha = e1.alpha;
            s = e1.s;
        }
        अन्यथा
            e1.op->assign(e1, m1);

        जर( isAddEx(e2) && (!e2.b.data || e2.beta == 0) )
        {
            m2 = e2.a;
            beta = e2.alpha;
            s += e2.s;
        }
        अन्यथा
            e2.op->assign(e2, m2);
        MatOp_AddEx::make_अभिव्यक्ती(res, m1, m2, alpha, beta, s);
    }
    अन्यथा
        e2.op->add(e1, e2, res);
}


शून्य MatOp::add(स्थिरांक Mat_अभिव्यक्ती& expr1, स्थिरांक Scalar& s, Mat_अभिव्यक्ती& res) स्थिरांक
{
    CV_INSTRUMENT_REGION();

    Mat m1;
    expr1.op->assign(expr1, m1);
    MatOp_AddEx::make_अभिव्यक्ती(res, m1, Mat(), 1, 0, s);
}


शून्य MatOp::subtract(स्थिरांक Mat_अभिव्यक्ती& e1, स्थिरांक Mat_अभिव्यक्ती& e2, Mat_अभिव्यक्ती& res) स्थिरांक
{
    CV_INSTRUMENT_REGION();

    जर( हे == e2.op )
    {
        द्विगुण alpha = 1, beta = -1;
        Scalar s;
        Mat m1, m2;
        जर( isAddEx(e1) && (!e1.b.data || e1.beta == 0) )
        {
            m1 = e1.a;
            alpha = e1.alpha;
            s = e1.s;
        }
        अन्यथा
            e1.op->assign(e1, m1);

        जर( isAddEx(e2) && (!e2.b.data || e2.beta == 0) )
        {
            m2 = e2.a;
            beta = -e2.alpha;
            s -= e2.s;
        }
        अन्यथा
            e2.op->assign(e2, m2);
        MatOp_AddEx::make_अभिव्यक्ती(res, m1, m2, alpha, beta, s);
    }
    अन्यथा
        e2.op->subtract(e1, e2, res);
}


शून्य MatOp::subtract(स्थिरांक Scalar& s, स्थिरांक Mat_अभिव्यक्ती& अभिव्यक्ती, Mat_अभिव्यक्ती& res) स्थिरांक
{
    CV_INSTRUMENT_REGION();

    Mat m;
    अभिव्यक्ती.op->assign(अभिव्यक्ती, m);
    MatOp_AddEx::make_अभिव्यक्ती(res, m, Mat(), -1, 0, s);
}


शून्य MatOp::multiply(स्थिरांक Mat_अभिव्यक्ती& e1, स्थिरांक Mat_अभिव्यक्ती& e2, Mat_अभिव्यक्ती& res, द्विगुण scale) स्थिरांक
{
    CV_INSTRUMENT_REGION();

    जर( हे == e2.op )
    {
        Mat m1, m2;

        जर( isReciprocal(e1) )
        {
            जर( isScaled(e2) )
            {
                scale *= e2.alpha;
                m2 = e2.a;
            }
            अन्यथा
                e2.op->assign(e2, m2);

            MatOp_Bin::make_अभिव्यक्ती(res, '/', m2, e1.a, scale/e1.alpha);
        }
        अन्यथा
        {
            अक्षर op = '*';
            जर( isScaled(e1) )
            {
                m1 = e1.a;
                scale *= e1.alpha;
            }
            अन्यथा
                e1.op->assign(e1, m1);

            जर( isScaled(e2) )
            {
                m2 = e2.a;
                scale *= e2.alpha;
            }
            अन्यथा जर( isReciprocal(e2) )
            {
                op = '/';
                m2 = e2.a;
                scale *= e2.alpha;
            }
            अन्यथा
                e2.op->assign(e2, m2);

            MatOp_Bin::make_अभिव्यक्ती(res, op, m1, m2, scale);
        }
    }
    अन्यथा
        e2.op->multiply(e1, e2, res, scale);
}


शून्य MatOp::multiply(स्थिरांक Mat_अभिव्यक्ती& अभिव्यक्ती, द्विगुण s, Mat_अभिव्यक्ती& res) स्थिरांक
{
    CV_INSTRUMENT_REGION();

    Mat m;
    अभिव्यक्ती.op->assign(अभिव्यक्ती, m);
    MatOp_AddEx::make_अभिव्यक्ती(res, m, Mat(), s, 0);
}


शून्य MatOp::divide(स्थिरांक Mat_अभिव्यक्ती& e1, स्थिरांक Mat_अभिव्यक्ती& e2, Mat_अभिव्यक्ती& res, द्विगुण scale) स्थिरांक
{
    CV_INSTRUMENT_REGION();

    जर( हे == e2.op )
    {
        जर( isReciprocal(e1) && isReciprocal(e2) )
            MatOp_Bin::make_अभिव्यक्ती(res, '/', e2.a, e1.a, e1.alpha/e2.alpha);
        अन्यथा
        {
            Mat m1, m2;
            अक्षर op = '/';

            जर( isScaled(e1) )
            {
                m1 = e1.a;
                scale *= e1.alpha;
            }
            अन्यथा
                e1.op->assign(e1, m1);

            जर( isScaled(e2) )
            {
                m2 = e2.a;
                scale /= e2.alpha;
            }
            अन्यथा जर( isReciprocal(e2) )
            {
                m2 = e2.a;
                scale /= e2.alpha;
                op = '*';
            }
            अन्यथा
                e2.op->assign(e2, m2);
            MatOp_Bin::make_अभिव्यक्ती(res, op, m1, m2, scale);
        }
    }
    अन्यथा
        e2.op->divide(e1, e2, res, scale);
}


शून्य MatOp::divide(द्विगुण s, स्थिरांक Mat_अभिव्यक्ती& अभिव्यक्ती, Mat_अभिव्यक्ती& res) स्थिरांक
{
    CV_INSTRUMENT_REGION();

    Mat m;
    अभिव्यक्ती.op->assign(अभिव्यक्ती, m);
    MatOp_Bin::make_अभिव्यक्ती(res, '/', m, Mat(), s);
}


शून्य MatOp::असल(स्थिरांक Mat_अभिव्यक्ती& अभिव्यक्ती, Mat_अभिव्यक्ती& res) स्थिरांक
{
    CV_INSTRUMENT_REGION();

    Mat m;
    अभिव्यक्ती.op->assign(अभिव्यक्ती, m);
    MatOp_Bin::make_अभिव्यक्ती(res, 'a', m, Mat());
}


शून्य MatOp::transpose(स्थिरांक Mat_अभिव्यक्ती& अभिव्यक्ती, Mat_अभिव्यक्ती& res) स्थिरांक
{
    CV_INSTRUMENT_REGION();

    Mat m;
    अभिव्यक्ती.op->assign(अभिव्यक्ती, m);
    MatOp_T::make_अभिव्यक्ती(res, m, 1);
}


शून्य MatOp::matmul(स्थिरांक Mat_अभिव्यक्ती& e1, स्थिरांक Mat_अभिव्यक्ती& e2, Mat_अभिव्यक्ती& res) स्थिरांक
{
    जर( हे == e2.op )
    {
        द्विगुण scale = 1;
        पूर्णांक flags = 0;
        Mat m1, m2;

        जर( isT(e1) )
        {
            flags = cv::GEMM_1_T;
            scale = e1.alpha;
            m1 = e1.a;
        }
        अन्यथा जर( isScaled(e1) )
        {
            scale = e1.alpha;
            m1 = e1.a;
        }
        अन्यथा
            e1.op->assign(e1, m1);

        जर( isT(e2) )
        {
            flags |= cv::GEMM_2_T;
            scale *= e2.alpha;
            m2 = e2.a;
        }
        अन्यथा जर( isScaled(e2) )
        {
            scale *= e2.alpha;
            m2 = e2.a;
        }
        अन्यथा
            e2.op->assign(e2, m2);

        MatOp_GEMM::make_अभिव्यक्ती(res, flags, m1, m2, scale);
    }
    अन्यथा
        e2.op->matmul(e1, e2, res);
}


शून्य MatOp::invert(स्थिरांक Mat_अभिव्यक्ती& अभिव्यक्ती, पूर्णांक method, Mat_अभिव्यक्ती& res) स्थिरांक
{
    Mat m;
    अभिव्यक्ती.op->assign(अभिव्यक्ती, m);
    MatOp_Invert::make_अभिव्यक्ती(res, method, m);
}


Size MatOp::size(स्थिरांक Mat_अभिव्यक्ती& अभिव्यक्ती) स्थिरांक
{
    परत !अभिव्यक्ती.a.empty() ? अभिव्यक्ती.a.size() : अभिव्यक्ती.b.empty() ? अभिव्यक्ती.b.size() : अभिव्यक्ती.c.size();
}

पूर्णांक MatOp::type(स्थिरांक Mat_अभिव्यक्ती& अभिव्यक्ती) स्थिरांक
{
    CV_INSTRUMENT_REGION();

    परत !अभिव्यक्ती.a.empty() ? अभिव्यक्ती.a.type() : अभिव्यक्ती.b.empty() ? अभिव्यक्ती.b.type() : अभिव्यक्ती.c.type();
}

//////////////////////////////////////////////////////////////////////////////////////////////////

Mat_अभिव्यक्ती::Mat_अभिव्यक्ती(स्थिरांक Mat& m) : op(&g_MatOp_Identity), flags(0), a(m), b(Mat()), c(Mat()), alpha(1), beta(0), s(Scalar())
{
}

Mat_अभिव्यक्ती Mat_अभिव्यक्ती::row(पूर्णांक y) स्थिरांक
{
    Mat_अभिव्यक्ती e;
    op->roi(*हे, Range(y, y+1), Range::all(), e);
    परत e;
}

Mat_अभिव्यक्ती Mat_अभिव्यक्ती::col(पूर्णांक x) स्थिरांक
{
    Mat_अभिव्यक्ती e;
    op->roi(*हे, Range::all(), Range(x, x+1), e);
    परत e;
}

Mat_अभिव्यक्ती Mat_अभिव्यक्ती::diag(पूर्णांक d) स्थिरांक
{
    Mat_अभिव्यक्ती e;
    op->diag(*हे, d, e);
    परत e;
}

Mat_अभिव्यक्ती Mat_अभिव्यक्ती::चालक()( स्थिरांक Range& rowRange, स्थिरांक Range& colRange ) स्थिरांक
{
    Mat_अभिव्यक्ती e;
    op->roi(*हे, rowRange, colRange, e);
    परत e;
}

Mat_अभिव्यक्ती Mat_अभिव्यक्ती::चालक()( स्थिरांक Rect& roi ) स्थिरांक
{
    Mat_अभिव्यक्ती e;
    op->roi(*हे, Range(roi.y, roi.y + roi.height), Range(roi.x, roi.x + roi.width), e);
    परत e;
}

Mat Mat_अभिव्यक्ती::cross(स्थिरांक Mat& m) स्थिरांक
{
    परत ((Mat)*हे).cross(m);
}

द्विगुण Mat_अभिव्यक्ती::dot(स्थिरांक Mat& m) स्थिरांक
{
    परत ((Mat)*हे).dot(m);
}

Mat_अभिव्यक्ती Mat_अभिव्यक्ती::t() स्थिरांक
{
    Mat_अभिव्यक्ती e;
    op->transpose(*हे, e);
    परत e;
}

Mat_अभिव्यक्ती Mat_अभिव्यक्ती::inv(पूर्णांक method) स्थिरांक
{
    Mat_अभिव्यक्ती e;
    op->invert(*हे, method, e);
    परत e;
}

Mat_अभिव्यक्ती Mat_अभिव्यक्ती::mul(स्थिरांक Mat_अभिव्यक्ती& e, द्विगुण scale) स्थिरांक
{
    Mat_अभिव्यक्ती en;
    op->multiply(*हे, e, en, scale);
    परत en;
}

Mat_अभिव्यक्ती Mat_अभिव्यक्ती::mul(स्थिरांक Mat& m, द्विगुण scale) स्थिरांक
{
    Mat_अभिव्यक्ती e;
    op->multiply(*हे, Mat_अभिव्यक्ती(m), e, scale);
    परत e;
}

Mat_अभिव्यक्ती चालक + (स्थिरांक Mat& a, स्थिरांक Mat& b)
{
    checkOperandsExist(a, b);
    Mat_अभिव्यक्ती e;
    MatOp_AddEx::make_अभिव्यक्ती(e, a, b, 1, 1);
    परत e;
}

Mat_अभिव्यक्ती चालक + (स्थिरांक Mat& a, स्थिरांक Scalar& s)
{
    checkOperandsExist(a);
    Mat_अभिव्यक्ती e;
    MatOp_AddEx::make_अभिव्यक्ती(e, a, Mat(), 1, 0, s);
    परत e;
}

Mat_अभिव्यक्ती चालक + (स्थिरांक Scalar& s, स्थिरांक Mat& a)
{
    checkOperandsExist(a);
    Mat_अभिव्यक्ती e;
    MatOp_AddEx::make_अभिव्यक्ती(e, a, Mat(), 1, 0, s);
    परत e;
}

Mat_अभिव्यक्ती चालक + (स्थिरांक Mat_अभिव्यक्ती& e, स्थिरांक Mat& m)
{
    Mat_अभिव्यक्ती en;
    e.op->add(e, Mat_अभिव्यक्ती(m), en);
    परत en;
}

Mat_अभिव्यक्ती चालक + (स्थिरांक Mat& m, स्थिरांक Mat_अभिव्यक्ती& e)
{
    checkOperandsExist(m);
    Mat_अभिव्यक्ती en;
    e.op->add(e, Mat_अभिव्यक्ती(m), en);
    परत en;
}

Mat_अभिव्यक्ती चालक + (स्थिरांक Mat_अभिव्यक्ती& e, स्थिरांक Scalar& s)
{
    Mat_अभिव्यक्ती en;
    e.op->add(e, s, en);
    परत en;
}

Mat_अभिव्यक्ती चालक + (स्थिरांक Scalar& s, स्थिरांक Mat_अभिव्यक्ती& e)
{
    Mat_अभिव्यक्ती en;
    e.op->add(e, s, en);
    परत en;
}

Mat_अभिव्यक्ती चालक + (स्थिरांक Mat_अभिव्यक्ती& e1, स्थिरांक Mat_अभिव्यक्ती& e2)
{
    Mat_अभिव्यक्ती en;
    e1.op->add(e1, e2, en);
    परत en;
}

Mat_अभिव्यक्ती चालक - (स्थिरांक Mat& a, स्थिरांक Mat& b)
{
    checkOperandsExist(a, b);
    Mat_अभिव्यक्ती e;
    MatOp_AddEx::make_अभिव्यक्ती(e, a, b, 1, -1);
    परत e;
}

Mat_अभिव्यक्ती चालक - (स्थिरांक Mat& a, स्थिरांक Scalar& s)
{
    checkOperandsExist(a);
    Mat_अभिव्यक्ती e;
    MatOp_AddEx::make_अभिव्यक्ती(e, a, Mat(), 1, 0, -s);
    परत e;
}

Mat_अभिव्यक्ती चालक - (स्थिरांक Scalar& s, स्थिरांक Mat& a)
{
    checkOperandsExist(a);
    Mat_अभिव्यक्ती e;
    MatOp_AddEx::make_अभिव्यक्ती(e, a, Mat(), -1, 0, s);
    परत e;
}

Mat_अभिव्यक्ती चालक - (स्थिरांक Mat_अभिव्यक्ती& e, स्थिरांक Mat& m)
{
    checkOperandsExist(m);
    Mat_अभिव्यक्ती en;
    e.op->subtract(e, Mat_अभिव्यक्ती(m), en);
    परत en;
}

Mat_अभिव्यक्ती चालक - (स्थिरांक Mat& m, स्थिरांक Mat_अभिव्यक्ती& e)
{
    checkOperandsExist(m);
    Mat_अभिव्यक्ती en;
    e.op->subtract(Mat_अभिव्यक्ती(m), e, en);
    परत en;
}

Mat_अभिव्यक्ती चालक - (स्थिरांक Mat_अभिव्यक्ती& e, स्थिरांक Scalar& s)
{
    Mat_अभिव्यक्ती en;
    e.op->add(e, -s, en);
    परत en;
}

Mat_अभिव्यक्ती चालक - (स्थिरांक Scalar& s, स्थिरांक Mat_अभिव्यक्ती& e)
{
    Mat_अभिव्यक्ती en;
    e.op->subtract(s, e, en);
    परत en;
}

Mat_अभिव्यक्ती चालक - (स्थिरांक Mat_अभिव्यक्ती& e1, स्थिरांक Mat_अभिव्यक्ती& e2)
{
    Mat_अभिव्यक्ती en;
    e1.op->subtract(e1, e2, en);
    परत en;
}

Mat_अभिव्यक्ती चालक - (स्थिरांक Mat& m)
{
    checkOperandsExist(m);
    Mat_अभिव्यक्ती e;
    MatOp_AddEx::make_अभिव्यक्ती(e, m, Mat(), -1, 0);
    परत e;
}

Mat_अभिव्यक्ती चालक - (स्थिरांक Mat_अभिव्यक्ती& e)
{
    Mat_अभिव्यक्ती en;
    e.op->subtract(Scalar(0), e, en);
    परत en;
}

Mat_अभिव्यक्ती चालक * (स्थिरांक Mat& a, स्थिरांक Mat& b)
{
    checkOperandsExist(a, b);
    Mat_अभिव्यक्ती e;
    MatOp_GEMM::make_अभिव्यक्ती(e, 0, a, b);
    परत e;
}

Mat_अभिव्यक्ती चालक * (स्थिरांक Mat& a, द्विगुण s)
{
    checkOperandsExist(a);
    Mat_अभिव्यक्ती e;
    MatOp_AddEx::make_अभिव्यक्ती(e, a, Mat(), s, 0);
    परत e;
}

Mat_अभिव्यक्ती चालक * (द्विगुण s, स्थिरांक Mat& a)
{
    checkOperandsExist(a);
    Mat_अभिव्यक्ती e;
    MatOp_AddEx::make_अभिव्यक्ती(e, a, Mat(), s, 0);
    परत e;
}

Mat_अभिव्यक्ती चालक * (स्थिरांक Mat_अभिव्यक्ती& e, स्थिरांक Mat& m)
{
    Mat_अभिव्यक्ती en;
    e.op->matmul(e, Mat_अभिव्यक्ती(m), en);
    परत en;
}

Mat_अभिव्यक्ती चालक * (स्थिरांक Mat& m, स्थिरांक Mat_अभिव्यक्ती& e)
{
    checkOperandsExist(m);
    Mat_अभिव्यक्ती en;
    e.op->matmul(Mat_अभिव्यक्ती(m), e, en);
    परत en;
}

Mat_अभिव्यक्ती चालक * (स्थिरांक Mat_अभिव्यक्ती& e, द्विगुण s)
{
    Mat_अभिव्यक्ती en;
    e.op->multiply(e, s, en);
    परत en;
}

Mat_अभिव्यक्ती चालक * (द्विगुण s, स्थिरांक Mat_अभिव्यक्ती& e)
{
    Mat_अभिव्यक्ती en;
    e.op->multiply(e, s, en);
    परत en;
}

Mat_अभिव्यक्ती चालक * (स्थिरांक Mat_अभिव्यक्ती& e1, स्थिरांक Mat_अभिव्यक्ती& e2)
{
    Mat_अभिव्यक्ती en;
    e1.op->matmul(e1, e2, en);
    परत en;
}

Mat_अभिव्यक्ती चालक / (स्थिरांक Mat& a, स्थिरांक Mat& b)
{
    checkOperandsExist(a, b);
    Mat_अभिव्यक्ती e;
    MatOp_Bin::make_अभिव्यक्ती(e, '/', a, b);
    परत e;
}

Mat_अभिव्यक्ती चालक / (स्थिरांक Mat& a, द्विगुण s)
{
    checkOperandsExist(a);
    Mat_अभिव्यक्ती e;
    MatOp_AddEx::make_अभिव्यक्ती(e, a, Mat(), 1./s, 0);
    परत e;
}

Mat_अभिव्यक्ती चालक / (द्विगुण s, स्थिरांक Mat& a)
{
    checkOperandsExist(a);
    Mat_अभिव्यक्ती e;
    MatOp_Bin::make_अभिव्यक्ती(e, '/', a, Mat(), s);
    परत e;
}

Mat_अभिव्यक्ती चालक / (स्थिरांक Mat_अभिव्यक्ती& e, स्थिरांक Mat& m)
{
    Mat_अभिव्यक्ती en;
    e.op->divide(e, Mat_अभिव्यक्ती(m), en);
    परत en;
}

Mat_अभिव्यक्ती चालक / (स्थिरांक Mat& m, स्थिरांक Mat_अभिव्यक्ती& e)
{
    checkOperandsExist(m);
    Mat_अभिव्यक्ती en;
    e.op->divide(Mat_अभिव्यक्ती(m), e, en);
    परत en;
}

Mat_अभिव्यक्ती चालक / (स्थिरांक Mat_अभिव्यक्ती& e, द्विगुण s)
{
    Mat_अभिव्यक्ती en;
    e.op->multiply(e, 1./s, en);
    परत en;
}

Mat_अभिव्यक्ती चालक / (द्विगुण s, स्थिरांक Mat_अभिव्यक्ती& e)
{
    Mat_अभिव्यक्ती en;
    e.op->divide(s, e, en);
    परत en;
}

Mat_अभिव्यक्ती चालक / (स्थिरांक Mat_अभिव्यक्ती& e1, स्थिरांक Mat_अभिव्यक्ती& e2)
{
    Mat_अभिव्यक्ती en;
    e1.op->divide(e1, e2, en);
    परत en;
}

Mat_अभिव्यक्ती चालक < (स्थिरांक Mat& a, स्थिरांक Mat& b)
{
    checkOperandsExist(a, b);
    Mat_अभिव्यक्ती e;
    MatOp_Cmp::make_अभिव्यक्ती(e, cv::CMP_LT, a, b);
    परत e;
}

Mat_अभिव्यक्ती चालक < (स्थिरांक Mat& a, द्विगुण s)
{
    checkOperandsExist(a);
    Mat_अभिव्यक्ती e;
    MatOp_Cmp::make_अभिव्यक्ती(e, cv::CMP_LT, a, s);
    परत e;
}

Mat_अभिव्यक्ती चालक < (द्विगुण s, स्थिरांक Mat& a)
{
    checkOperandsExist(a);
    Mat_अभिव्यक्ती e;
    MatOp_Cmp::make_अभिव्यक्ती(e, cv::CMP_GT, a, s);
    परत e;
}

Mat_अभिव्यक्ती चालक <= (स्थिरांक Mat& a, स्थिरांक Mat& b)
{
    checkOperandsExist(a, b);
    Mat_अभिव्यक्ती e;
    MatOp_Cmp::make_अभिव्यक्ती(e, cv::CMP_LE, a, b);
    परत e;
}

Mat_अभिव्यक्ती चालक <= (स्थिरांक Mat& a, द्विगुण s)
{
    checkOperandsExist(a);
    Mat_अभिव्यक्ती e;
    MatOp_Cmp::make_अभिव्यक्ती(e, cv::CMP_LE, a, s);
    परत e;
}

Mat_अभिव्यक्ती चालक <= (द्विगुण s, स्थिरांक Mat& a)
{
    checkOperandsExist(a);
    Mat_अभिव्यक्ती e;
    MatOp_Cmp::make_अभिव्यक्ती(e, cv::CMP_GE, a, s);
    परत e;
}

Mat_अभिव्यक्ती चालक == (स्थिरांक Mat& a, स्थिरांक Mat& b)
{
    checkOperandsExist(a, b);
    Mat_अभिव्यक्ती e;
    MatOp_Cmp::make_अभिव्यक्ती(e, cv::CMP_EQ, a, b);
    परत e;
}

Mat_अभिव्यक्ती चालक == (स्थिरांक Mat& a, द्विगुण s)
{
    checkOperandsExist(a);
    Mat_अभिव्यक्ती e;
    MatOp_Cmp::make_अभिव्यक्ती(e, cv::CMP_EQ, a, s);
    परत e;
}

Mat_अभिव्यक्ती चालक == (द्विगुण s, स्थिरांक Mat& a)
{
    checkOperandsExist(a);
    Mat_अभिव्यक्ती e;
    MatOp_Cmp::make_अभिव्यक्ती(e, cv::CMP_EQ, a, s);
    परत e;
}

Mat_अभिव्यक्ती चालक != (स्थिरांक Mat& a, स्थिरांक Mat& b)
{
    checkOperandsExist(a, b);
    Mat_अभिव्यक्ती e;
    MatOp_Cmp::make_अभिव्यक्ती(e, cv::CMP_NE, a, b);
    परत e;
}

Mat_अभिव्यक्ती चालक != (स्थिरांक Mat& a, द्विगुण s)
{
    checkOperandsExist(a);
    Mat_अभिव्यक्ती e;
    MatOp_Cmp::make_अभिव्यक्ती(e, cv::CMP_NE, a, s);
    परत e;
}

Mat_अभिव्यक्ती चालक != (द्विगुण s, स्थिरांक Mat& a)
{
    checkOperandsExist(a);
    Mat_अभिव्यक्ती e;
    MatOp_Cmp::make_अभिव्यक्ती(e, cv::CMP_NE, a, s);
    परत e;
}

Mat_अभिव्यक्ती चालक >= (स्थिरांक Mat& a, स्थिरांक Mat& b)
{
    checkOperandsExist(a, b);
    Mat_अभिव्यक्ती e;
    MatOp_Cmp::make_अभिव्यक्ती(e, cv::CMP_GE, a, b);
    परत e;
}

Mat_अभिव्यक्ती चालक >= (स्थिरांक Mat& a, द्विगुण s)
{
    checkOperandsExist(a);
    Mat_अभिव्यक्ती e;
    MatOp_Cmp::make_अभिव्यक्ती(e, cv::CMP_GE, a, s);
    परत e;
}

Mat_अभिव्यक्ती चालक >= (द्विगुण s, स्थिरांक Mat& a)
{
    checkOperandsExist(a);
    Mat_अभिव्यक्ती e;
    MatOp_Cmp::make_अभिव्यक्ती(e, cv::CMP_LE, a, s);
    परत e;
}

Mat_अभिव्यक्ती चालक > (स्थिरांक Mat& a, स्थिरांक Mat& b)
{
    checkOperandsExist(a, b);
    Mat_अभिव्यक्ती e;
    MatOp_Cmp::make_अभिव्यक्ती(e, cv::CMP_GT, a, b);
    परत e;
}

Mat_अभिव्यक्ती चालक > (स्थिरांक Mat& a, द्विगुण s)
{
    checkOperandsExist(a);
    Mat_अभिव्यक्ती e;
    MatOp_Cmp::make_अभिव्यक्ती(e, cv::CMP_GT, a, s);
    परत e;
}

Mat_अभिव्यक्ती चालक > (द्विगुण s, स्थिरांक Mat& a)
{
    checkOperandsExist(a);
    Mat_अभिव्यक्ती e;
    MatOp_Cmp::make_अभिव्यक्ती(e, cv::CMP_LT, a, s);
    परत e;
}

Mat_अभिव्यक्ती min(स्थिरांक Mat& a, स्थिरांक Mat& b)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a, b);
    Mat_अभिव्यक्ती e;
    MatOp_Bin::make_अभिव्यक्ती(e, 'm', a, b);
    परत e;
}

Mat_अभिव्यक्ती min(स्थिरांक Mat& a, द्विगुण s)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    Mat_अभिव्यक्ती e;
    MatOp_Bin::make_अभिव्यक्ती(e, 'n', a, s);
    परत e;
}

Mat_अभिव्यक्ती min(द्विगुण s, स्थिरांक Mat& a)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    Mat_अभिव्यक्ती e;
    MatOp_Bin::make_अभिव्यक्ती(e, 'n', a, s);
    परत e;
}

Mat_अभिव्यक्ती max(स्थिरांक Mat& a, स्थिरांक Mat& b)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a, b);
    Mat_अभिव्यक्ती e;
    MatOp_Bin::make_अभिव्यक्ती(e, 'M', a, b);
    परत e;
}

Mat_अभिव्यक्ती max(स्थिरांक Mat& a, द्विगुण s)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    Mat_अभिव्यक्ती e;
    MatOp_Bin::make_अभिव्यक्ती(e, 'N', a, s);
    परत e;
}

Mat_अभिव्यक्ती max(द्विगुण s, स्थिरांक Mat& a)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    Mat_अभिव्यक्ती e;
    MatOp_Bin::make_अभिव्यक्ती(e, 'N', a, s);
    परत e;
}

Mat_अभिव्यक्ती चालक & (स्थिरांक Mat& a, स्थिरांक Mat& b)
{
    checkOperandsExist(a, b);
    Mat_अभिव्यक्ती e;
    MatOp_Bin::make_अभिव्यक्ती(e, '&', a, b);
    परत e;
}

Mat_अभिव्यक्ती चालक & (स्थिरांक Mat& a, स्थिरांक Scalar& s)
{
    checkOperandsExist(a);
    Mat_अभिव्यक्ती e;
    MatOp_Bin::make_अभिव्यक्ती(e, '&', a, s);
    परत e;
}

Mat_अभिव्यक्ती चालक & (स्थिरांक Scalar& s, स्थिरांक Mat& a)
{
    checkOperandsExist(a);
    Mat_अभिव्यक्ती e;
    MatOp_Bin::make_अभिव्यक्ती(e, '&', a, s);
    परत e;
}

Mat_अभिव्यक्ती चालक | (स्थिरांक Mat& a, स्थिरांक Mat& b)
{
    checkOperandsExist(a, b);
    Mat_अभिव्यक्ती e;
    MatOp_Bin::make_अभिव्यक्ती(e, '|', a, b);
    परत e;
}

Mat_अभिव्यक्ती चालक | (स्थिरांक Mat& a, स्थिरांक Scalar& s)
{
    checkOperandsExist(a);
    Mat_अभिव्यक्ती e;
    MatOp_Bin::make_अभिव्यक्ती(e, '|', a, s);
    परत e;
}

Mat_अभिव्यक्ती चालक | (स्थिरांक Scalar& s, स्थिरांक Mat& a)
{
    checkOperandsExist(a);
    Mat_अभिव्यक्ती e;
    MatOp_Bin::make_अभिव्यक्ती(e, '|', a, s);
    परत e;
}

Mat_अभिव्यक्ती चालक ^ (स्थिरांक Mat& a, स्थिरांक Mat& b)
{
    checkOperandsExist(a, b);
    Mat_अभिव्यक्ती e;
    MatOp_Bin::make_अभिव्यक्ती(e, '^', a, b);
    परत e;
}

Mat_अभिव्यक्ती चालक ^ (स्थिरांक Mat& a, स्थिरांक Scalar& s)
{
    checkOperandsExist(a);
    Mat_अभिव्यक्ती e;
    MatOp_Bin::make_अभिव्यक्ती(e, '^', a, s);
    परत e;
}

Mat_अभिव्यक्ती चालक ^ (स्थिरांक Scalar& s, स्थिरांक Mat& a)
{
    checkOperandsExist(a);
    Mat_अभिव्यक्ती e;
    MatOp_Bin::make_अभिव्यक्ती(e, '^', a, s);
    परत e;
}

Mat_अभिव्यक्ती चालक ~(स्थिरांक Mat& a)
{
    checkOperandsExist(a);
    Mat_अभिव्यक्ती e;
    MatOp_Bin::make_अभिव्यक्ती(e, '~', a, Scalar());
    परत e;
}

Mat_अभिव्यक्ती असल(स्थिरांक Mat& a)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    Mat_अभिव्यक्ती e;
    MatOp_Bin::make_अभिव्यक्ती(e, 'a', a, Scalar());
    परत e;
}

Mat_अभिव्यक्ती असल(स्थिरांक Mat_अभिव्यक्ती& e)
{
    CV_INSTRUMENT_REGION();

    Mat_अभिव्यक्ती en;
    e.op->असल(e, en);
    परत en;
}


Size Mat_अभिव्यक्ती::size() स्थिरांक
{
    जर( isT(*हे) || isInv(*हे) )
        परत Size(a.rows, a.cols);
    जर( isSolve(*हे) )
        परत Size(b.cols, a.cols);
    जर( isInitializer(*हे) )
        परत a.size();
    परत op ? op->size(*हे) : Size();
}


पूर्णांक Mat_अभिव्यक्ती::type() स्थिरांक
{
    CV_INSTRUMENT_REGION();

    जर( isInitializer(*हे) )
        परत a.type();
    जर( isCmp(*हे) )
        परत CV_MAKETYPE(CV_8U, a.channels());
    परत op ? op->type(*हे) : -1;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////

शून्य MatOp_Identity::assign(स्थिरांक Mat_अभिव्यक्ती& e, Mat& m, पूर्णांक _type) स्थिरांक
{
    जर( _type == -1 || _type == e.a.type() )
        m = e.a;
    अन्यथा
    {
        CV_Assert( CV_MAT_CN(_type) == e.a.channels() );
        e.a.convertTo(m, _type);
    }
}

ओळीत शून्य MatOp_Identity::make_अभिव्यक्ती(Mat_अभिव्यक्ती& res, स्थिरांक Mat& m)
{
    res = Mat_अभिव्यक्ती(&g_MatOp_Identity, 0, m, Mat(), Mat(), 1, 0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

शून्य MatOp_AddEx::assign(स्थिरांक Mat_अभिव्यक्ती& e, Mat& m, पूर्णांक _type) स्थिरांक
{
    Mat temp, &dst = _type == -1 || e.a.type() == _type ? m : temp;
    जर( e.b.data )
    {
        जर( e.s == Scalar() || !e.s.isReal() )
        {
            जर( e.alpha == 1 )
            {
                जर( e.beta == 1 )
                    cv::add(e.a, e.b, dst);
                अन्यथा जर( e.beta == -1 )
                    cv::subtract(e.a, e.b, dst);
                अन्यथा
                    cv::scaleAdd(e.b, e.beta, e.a, dst);
            }
            अन्यथा जर( e.beta == 1 )
            {
                जर( e.alpha == -1 )
                    cv::subtract(e.b, e.a, dst);
                अन्यथा
                    cv::scaleAdd(e.a, e.alpha, e.b, dst);
            }
            अन्यथा
                cv::addWeighted(e.a, e.alpha, e.b, e.beta, 0, dst);

            जर( !e.s.isReal() )
                cv::add(dst, e.s, dst);
        }
        अन्यथा
        {
            जर (e.a.channels() > 1)
                CV_LOG_ONCE_WARNING(रिक्त, "OpenCV/MatExpr: processing of multi-channel arrays might be changed in the future: "
                                          "https://github.com/opencv/opencv/issues/16739");
            cv::addWeighted(e.a, e.alpha, e.b, e.beta, e.s[0], dst);
        }
    }
    अन्यथा जर( e.s.isReal() && (dst.data != m.data || भ_असल(e.alpha) != 1))
    {
        जर (e.a.channels() > 1 && e.s[0] != 0.0)
            CV_LOG_ONCE_WARNING(रिक्त, "OpenCV/MatExpr: processing of multi-channel arrays might be changed in the future: "
                                      "https://github.com/opencv/opencv/issues/16739");
        e.a.convertTo(m, _type, e.alpha, e.s[0]);
        परत;
    }
    अन्यथा जर( e.alpha == 1 )
        cv::add(e.a, e.s, dst);
    अन्यथा जर( e.alpha == -1 )
        cv::subtract(e.s, e.a, dst);
    अन्यथा
    {
        e.a.convertTo(dst, e.a.type(), e.alpha);
        cv::add(dst, e.s, dst);
    }

    जर( dst.data != m.data )
        dst.convertTo(m, m.type());
}


शून्य MatOp_AddEx::add(स्थिरांक Mat_अभिव्यक्ती& e, स्थिरांक Scalar& s, Mat_अभिव्यक्ती& res) स्थिरांक
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.s += s;
}


शून्य MatOp_AddEx::subtract(स्थिरांक Scalar& s, स्थिरांक Mat_अभिव्यक्ती& e, Mat_अभिव्यक्ती& res) स्थिरांक
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha = -res.alpha;
    res.beta = -res.beta;
    res.s = s - res.s;
}

शून्य MatOp_AddEx::multiply(स्थिरांक Mat_अभिव्यक्ती& e, द्विगुण s, Mat_अभिव्यक्ती& res) स्थिरांक
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha *= s;
    res.beta *= s;
    res.s *= s;
}

शून्य MatOp_AddEx::divide(द्विगुण s, स्थिरांक Mat_अभिव्यक्ती& e, Mat_अभिव्यक्ती& res) स्थिरांक
{
    CV_INSTRUMENT_REGION();

    जर( isScaled(e) )
        MatOp_Bin::make_अभिव्यक्ती(res, '/', e.a, Mat(), s/e.alpha);
    अन्यथा
        MatOp::divide(s, e, res);
}


शून्य MatOp_AddEx::transpose(स्थिरांक Mat_अभिव्यक्ती& e, Mat_अभिव्यक्ती& res) स्थिरांक
{
    CV_INSTRUMENT_REGION();

    जर( isScaled(e) )
        MatOp_T::make_अभिव्यक्ती(res, e.a, e.alpha);
    अन्यथा
        MatOp::transpose(e, res);
}

शून्य MatOp_AddEx::असल(स्थिरांक Mat_अभिव्यक्ती& e, Mat_अभिव्यक्ती& res) स्थिरांक
{
    CV_INSTRUMENT_REGION();

    जर( (!e.b.data || e.beta == 0) && भ_असल(e.alpha) == 1 )
        MatOp_Bin::make_अभिव्यक्ती(res, 'a', e.a, -e.s*e.alpha);
    अन्यथा जर( e.b.data && e.alpha + e.beta == 0 && e.alpha*e.beta == -1 )
        MatOp_Bin::make_अभिव्यक्ती(res, 'a', e.a, e.b);
    अन्यथा
        MatOp::असल(e, res);
}

ओळीत शून्य MatOp_AddEx::make_अभिव्यक्ती(Mat_अभिव्यक्ती& res, स्थिरांक Mat& a, स्थिरांक Mat& b, द्विगुण alpha, द्विगुण beta, स्थिरांक Scalar& s)
{
    res = Mat_अभिव्यक्ती(&g_MatOp_AddEx, 0, a, b, Mat(), alpha, beta, s);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

शून्य MatOp_Bin::assign(स्थिरांक Mat_अभिव्यक्ती& e, Mat& m, पूर्णांक _type) स्थिरांक
{
    Mat temp, &dst = _type == -1 || e.a.type() == _type ? m : temp;

    जर( e.flags == '*' )
        cv::multiply(e.a, e.b, dst, e.alpha);
    अन्यथा जर( e.flags == '/' && e.b.data )
        cv::divide(e.a, e.b, dst, e.alpha);
    अन्यथा जर( e.flags == '/' && !e.b.data )
        cv::divide(e.alpha, e.a, dst );
    अन्यथा जर( e.flags == '&' && e.b.data )
        bitwise_and(e.a, e.b, dst);
    अन्यथा जर( e.flags == '&' && !e.b.data )
        bitwise_and(e.a, e.s, dst);
    अन्यथा जर( e.flags == '|' && e.b.data )
        bitwise_or(e.a, e.b, dst);
    अन्यथा जर( e.flags == '|' && !e.b.data )
        bitwise_or(e.a, e.s, dst);
    अन्यथा जर( e.flags == '^' && e.b.data )
        bitwise_xor(e.a, e.b, dst);
    अन्यथा जर( e.flags == '^' && !e.b.data )
        bitwise_xor(e.a, e.s, dst);
    अन्यथा जर( e.flags == '~' && !e.b.data )
        bitwise_not(e.a, dst);
    अन्यथा जर( e.flags == 'm' )
        cv::min(e.a, e.b, dst);
    अन्यथा जर( e.flags == 'n' )
        cv::min(e.a, e.s[0], dst);
    अन्यथा जर( e.flags == 'M' )
        cv::max(e.a, e.b, dst);
    अन्यथा जर( e.flags == 'N' )
        cv::max(e.a, e.s[0], dst);
    अन्यथा जर( e.flags == 'a' && e.b.data )
        cv::absdiff(e.a, e.b, dst);
    अन्यथा जर( e.flags == 'a' && !e.b.data )
        cv::absdiff(e.a, e.s, dst);
    अन्यथा
        CV_Error(cv::Error::StsError, "Unknown operation");

    जर( dst.data != m.data )
        dst.convertTo(m, _type);
}

शून्य MatOp_Bin::multiply(स्थिरांक Mat_अभिव्यक्ती& e, द्विगुण s, Mat_अभिव्यक्ती& res) स्थिरांक
{
    CV_INSTRUMENT_REGION();

    जर( e.flags == '*' || e.flags == '/' )
    {
        res = e;
        res.alpha *= s;
    }
    अन्यथा
        MatOp::multiply(e, s, res);
}

शून्य MatOp_Bin::divide(द्विगुण s, स्थिरांक Mat_अभिव्यक्ती& e, Mat_अभिव्यक्ती& res) स्थिरांक
{
    CV_INSTRUMENT_REGION();

    जर( e.flags == '/' && (!e.b.data || e.beta == 0) )
        MatOp_AddEx::make_अभिव्यक्ती(res, e.a, Mat(), s/e.alpha, 0);
    अन्यथा
        MatOp::divide(s, e, res);
}

ओळीत शून्य MatOp_Bin::make_अभिव्यक्ती(Mat_अभिव्यक्ती& res, अक्षर op, स्थिरांक Mat& a, स्थिरांक Mat& b, द्विगुण scale)
{
    res = Mat_अभिव्यक्ती(&g_MatOp_Bin, op, a, b, Mat(), scale, b.data ? 1 : 0);
}

ओळीत शून्य MatOp_Bin::make_अभिव्यक्ती(Mat_अभिव्यक्ती& res, अक्षर op, स्थिरांक Mat& a, स्थिरांक Scalar& s)
{
    res = Mat_अभिव्यक्ती(&g_MatOp_Bin, op, a, Mat(), Mat(), 1, 0, s);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

शून्य MatOp_Cmp::assign(स्थिरांक Mat_अभिव्यक्ती& e, Mat& m, पूर्णांक _type) स्थिरांक
{
    Mat temp, &dst = _type == -1 || _type == CV_8U ? m : temp;

    जर( e.b.data )
        cv::compare(e.a, e.b, dst, e.flags);
    अन्यथा
        cv::compare(e.a, e.alpha, dst, e.flags);

    जर( dst.data != m.data )
        dst.convertTo(m, _type);
}

ओळीत शून्य MatOp_Cmp::make_अभिव्यक्ती(Mat_अभिव्यक्ती& res, पूर्णांक cmpop, स्थिरांक Mat& a, स्थिरांक Mat& b)
{
    res = Mat_अभिव्यक्ती(&g_MatOp_Cmp, cmpop, a, b, Mat(), 1, 1);
}

ओळीत शून्य MatOp_Cmp::make_अभिव्यक्ती(Mat_अभिव्यक्ती& res, पूर्णांक cmpop, स्थिरांक Mat& a, द्विगुण alpha)
{
    res = Mat_अभिव्यक्ती(&g_MatOp_Cmp, cmpop, a, Mat(), Mat(), alpha, 1);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

शून्य MatOp_T::assign(स्थिरांक Mat_अभिव्यक्ती& e, Mat& m, पूर्णांक _type) स्थिरांक
{
    Mat temp, &dst = _type == -1 || _type == e.a.type() ? m : temp;

    cv::transpose(e.a, dst);

    जर( dst.data != m.data || e.alpha != 1 )
        dst.convertTo(m, _type, e.alpha);
}

शून्य MatOp_T::multiply(स्थिरांक Mat_अभिव्यक्ती& e, द्विगुण s, Mat_अभिव्यक्ती& res) स्थिरांक
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha *= s;
}

शून्य MatOp_T::transpose(स्थिरांक Mat_अभिव्यक्ती& e, Mat_अभिव्यक्ती& res) स्थिरांक
{
    CV_INSTRUMENT_REGION();

    जर( e.alpha == 1 )
        MatOp_Identity::make_अभिव्यक्ती(res, e.a);
    अन्यथा
        MatOp_AddEx::make_अभिव्यक्ती(res, e.a, Mat(), e.alpha, 0);
}

ओळीत शून्य MatOp_T::make_अभिव्यक्ती(Mat_अभिव्यक्ती& res, स्थिरांक Mat& a, द्विगुण alpha)
{
    res = Mat_अभिव्यक्ती(&g_MatOp_T, 0, a, Mat(), Mat(), alpha, 0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

शून्य MatOp_GEMM::assign(स्थिरांक Mat_अभिव्यक्ती& e, Mat& m, पूर्णांक _type) स्थिरांक
{
    Mat temp, &dst = _type == -1 || _type == e.a.type() ? m : temp;

    cv::gemm(e.a, e.b, e.alpha, e.c, e.beta, dst, e.flags);
    जर( dst.data != m.data )
        dst.convertTo(m, _type);
}

शून्य MatOp_GEMM::add(स्थिरांक Mat_अभिव्यक्ती& e1, स्थिरांक Mat_अभिव्यक्ती& e2, Mat_अभिव्यक्ती& res) स्थिरांक
{
    CV_INSTRUMENT_REGION();

    तार्किक i1 = isIdentity(e1), i2 = isIdentity(e2);
    द्विगुण alpha1 = i1 ? 1 : e1.alpha, alpha2 = i2 ? 1 : e2.alpha;

    जर( isMatProd(e1) && (i2 || isScaled(e2) || isT(e2)) )
        MatOp_GEMM::make_अभिव्यक्ती(res, (e1.flags & ~cv::GEMM_3_T)|(isT(e2) ? cv::GEMM_3_T : 0),
                             e1.a, e1.b, alpha1, e2.a, alpha2);
    अन्यथा जर( isMatProd(e2) && (i1 || isScaled(e1) || isT(e1)) )
        MatOp_GEMM::make_अभिव्यक्ती(res, (e2.flags & ~cv::GEMM_3_T)|(isT(e1) ? cv::GEMM_3_T : 0),
                             e2.a, e2.b, alpha2, e1.a, alpha1);
    अन्यथा जर( हे == e2.op )
        MatOp::add(e1, e2, res);
    अन्यथा
        e2.op->add(e1, e2, res);
}

शून्य MatOp_GEMM::subtract(स्थिरांक Mat_अभिव्यक्ती& e1, स्थिरांक Mat_अभिव्यक्ती& e2, Mat_अभिव्यक्ती& res) स्थिरांक
{
    CV_INSTRUMENT_REGION();

    तार्किक i1 = isIdentity(e1), i2 = isIdentity(e2);
    द्विगुण alpha1 = i1 ? 1 : e1.alpha, alpha2 = i2 ? 1 : e2.alpha;

    जर( isMatProd(e1) && (i2 || isScaled(e2) || isT(e2)) )
        MatOp_GEMM::make_अभिव्यक्ती(res, (e1.flags & ~cv::GEMM_3_T)|(isT(e2) ? cv::GEMM_3_T : 0),
                             e1.a, e1.b, alpha1, e2.a, -alpha2);
    अन्यथा जर( isMatProd(e2) && (i1 || isScaled(e1) || isT(e1)) )
        MatOp_GEMM::make_अभिव्यक्ती(res, (e2.flags & ~cv::GEMM_3_T)|(isT(e1) ? cv::GEMM_3_T : 0),
                            e2.a, e2.b, -alpha2, e1.a, alpha1);
    अन्यथा जर( हे == e2.op )
        MatOp::subtract(e1, e2, res);
    अन्यथा
        e2.op->subtract(e1, e2, res);
}

शून्य MatOp_GEMM::multiply(स्थिरांक Mat_अभिव्यक्ती& e, द्विगुण s, Mat_अभिव्यक्ती& res) स्थिरांक
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha *= s;
    res.beta *= s;
}

शून्य MatOp_GEMM::transpose(स्थिरांक Mat_अभिव्यक्ती& e, Mat_अभिव्यक्ती& res) स्थिरांक
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.flags = (!(e.flags & cv::GEMM_1_T) ? cv::GEMM_2_T : 0) |
                (!(e.flags & cv::GEMM_2_T) ? cv::GEMM_1_T : 0) |
                (!(e.flags & cv::GEMM_3_T) ? cv::GEMM_3_T : 0);
    swap(res.a, res.b);
}

ओळीत शून्य MatOp_GEMM::make_अभिव्यक्ती(Mat_अभिव्यक्ती& res, पूर्णांक flags, स्थिरांक Mat& a, स्थिरांक Mat& b,
                                 द्विगुण alpha, स्थिरांक Mat& c, द्विगुण beta)
{
    res = Mat_अभिव्यक्ती(&g_MatOp_GEMM, flags, a, b, c, alpha, beta);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

शून्य MatOp_Invert::assign(स्थिरांक Mat_अभिव्यक्ती& e, Mat& m, पूर्णांक _type) स्थिरांक
{
    Mat temp, &dst = _type == -1 || _type == e.a.type() ? m : temp;

    cv::invert(e.a, dst, e.flags);
    जर( dst.data != m.data )
        dst.convertTo(m, _type);
}

शून्य MatOp_Invert::matmul(स्थिरांक Mat_अभिव्यक्ती& e1, स्थिरांक Mat_अभिव्यक्ती& e2, Mat_अभिव्यक्ती& res) स्थिरांक
{
    जर( isInv(e1) && isIdentity(e2) )
        MatOp_Solve::make_अभिव्यक्ती(res, e1.flags, e1.a, e2.a);
    अन्यथा जर( हे == e2.op )
        MatOp::matmul(e1, e2, res);
    अन्यथा
        e2.op->matmul(e1, e2, res);
}

ओळीत शून्य MatOp_Invert::make_अभिव्यक्ती(Mat_अभिव्यक्ती& res, पूर्णांक method, स्थिरांक Mat& m)
{
    res = Mat_अभिव्यक्ती(&g_MatOp_Invert, method, m, Mat(), Mat(), 1, 0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

शून्य MatOp_Solve::assign(स्थिरांक Mat_अभिव्यक्ती& e, Mat& m, पूर्णांक _type) स्थिरांक
{
    Mat temp, &dst = _type == -1 || _type == e.a.type() ? m : temp;

    cv::solve(e.a, e.b, dst, e.flags);
    जर( dst.data != m.data )
        dst.convertTo(m, _type);
}

ओळीत शून्य MatOp_Solve::make_अभिव्यक्ती(Mat_अभिव्यक्ती& res, पूर्णांक method, स्थिरांक Mat& a, स्थिरांक Mat& b)
{
    res = Mat_अभिव्यक्ती(&g_MatOp_Solve, method, a, b, Mat(), 1, 1);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

शून्य MatOp_Initializer::assign(स्थिरांक Mat_अभिव्यक्ती& e, Mat& m, पूर्णांक _type) स्थिरांक
{
    जर( _type == -1 )
        _type = e.a.type();

    m.create(e.a.size, _type);

    जर( e.flags == 'I' && e.a.dims <= 2 )
        setIdentity(m, Scalar(e.alpha));
    अन्यथा जर( e.flags == '0' )
        m = Scalar();
    अन्यथा जर( e.flags == '1' )
        m = Scalar(e.alpha);
    अन्यथा
        CV_Error(cv::Error::StsError, "Invalid matrix initializer type");
}

शून्य MatOp_Initializer::multiply(स्थिरांक Mat_अभिव्यक्ती& e, द्विगुण s, Mat_अभिव्यक्ती& res) स्थिरांक
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha *= s;
}

ओळीत शून्य MatOp_Initializer::make_अभिव्यक्ती(Mat_अभिव्यक्ती& res, पूर्णांक method, Size sz, पूर्णांक type, द्विगुण alpha)
{
    res = Mat_अभिव्यक्ती(getGlobalMatOpInitializer(), method, Mat(sz, type, (शून्य*)(माप_प्रकार)0xEEEEEEEE), Mat(), Mat(), alpha, 0);
}

ओळीत शून्य MatOp_Initializer::make_अभिव्यक्ती(Mat_अभिव्यक्ती& res, पूर्णांक method, पूर्णांक ndims, स्थिरांक पूर्णांक* sizes, पूर्णांक type, द्विगुण alpha)
{
    res = Mat_अभिव्यक्ती(getGlobalMatOpInitializer(), method, Mat(ndims, sizes, type, (शून्य*)(माप_प्रकार)0xEEEEEEEE), Mat(), Mat(), alpha, 0);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

Mat_अभिव्यक्ती Mat::t() स्थिरांक
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(*हे);
    Mat_अभिव्यक्ती e;
    MatOp_T::make_अभिव्यक्ती(e, *हे);
    परत e;
}

Mat_अभिव्यक्ती Mat::inv(पूर्णांक method) स्थिरांक
{
    CV_INSTRUMENT_REGION();

    Mat_अभिव्यक्ती e;
    MatOp_Invert::make_अभिव्यक्ती(e, method, *हे);
    परत e;
}


Mat_अभिव्यक्ती Mat::mul(आदान_Array m, द्विगुण scale) स्थिरांक
{
    CV_INSTRUMENT_REGION();

    Mat b = m.getMat();
    // Unless the argument is a refcounted Mat/UMat, the header returned by getMat() may be
    // a non-owning view of caller memory (e.g. a scalar bound to _InputArray(const double&),
    // a Matx or a Vec) that the returned MatExpr can outlive, so snapshot it.
    // See https://github.com/opencv/opencv/issues/23577
    जर( !m.isMat() && !m.isUMat() )
        b = b.clone();

    Mat_अभिव्यक्ती e;
    MatOp_Bin::make_अभिव्यक्ती(e, '*', *हे, b, scale);
    परत e;
}

Mat_अभिव्यक्ती Mat::zeros(पूर्णांक rows, पूर्णांक cols, पूर्णांक type)
{
    CV_INSTRUMENT_REGION();

    Mat_अभिव्यक्ती e;
    MatOp_Initializer::make_अभिव्यक्ती(e, '0', Size(cols, rows), type);
    परत e;
}

Mat_अभिव्यक्ती Mat::zeros(Size size, पूर्णांक type)
{
    CV_INSTRUMENT_REGION();

    Mat_अभिव्यक्ती e;
    MatOp_Initializer::make_अभिव्यक्ती(e, '0', size, type);
    परत e;
}

Mat_अभिव्यक्ती Mat::zeros(पूर्णांक ndims, स्थिरांक पूर्णांक* sizes, पूर्णांक type)
{
    CV_INSTRUMENT_REGION();

    Mat_अभिव्यक्ती e;
    MatOp_Initializer::make_अभिव्यक्ती(e, '0', ndims, sizes, type);
    परत e;
}

Mat_अभिव्यक्ती Mat::zeros(स्थिरांक MatShape& shape, पूर्णांक type)
{
    CV_INSTRUMENT_REGION();

    Mat_अभिव्यक्ती e;
    MatOp_Initializer::make_अभिव्यक्ती(e, '0', shape.dims, shape.p, type);
    परत e;
}

Mat_अभिव्यक्ती Mat::ones(पूर्णांक rows, पूर्णांक cols, पूर्णांक type)
{
    CV_INSTRUMENT_REGION();

    Mat_अभिव्यक्ती e;
    MatOp_Initializer::make_अभिव्यक्ती(e, '1', Size(cols, rows), type);
    परत e;
}

Mat_अभिव्यक्ती Mat::ones(Size size, पूर्णांक type)
{
    CV_INSTRUMENT_REGION();

    Mat_अभिव्यक्ती e;
    MatOp_Initializer::make_अभिव्यक्ती(e, '1', size, type);
    परत e;
}

Mat_अभिव्यक्ती Mat::ones(पूर्णांक ndims, स्थिरांक पूर्णांक* sizes, पूर्णांक type)
{
    CV_INSTRUMENT_REGION();

    Mat_अभिव्यक्ती e;
    MatOp_Initializer::make_अभिव्यक्ती(e, '1', ndims, sizes, type);
    परत e;
}

Mat_अभिव्यक्ती Mat::ones(स्थिरांक MatShape& shape, पूर्णांक type)
{
    CV_INSTRUMENT_REGION();

    Mat_अभिव्यक्ती e;
    MatOp_Initializer::make_अभिव्यक्ती(e, '1', shape.dims, shape.p, type);
    परत e;
}

Mat_अभिव्यक्ती Mat::eye(पूर्णांक rows, पूर्णांक cols, पूर्णांक type)
{
    CV_INSTRUMENT_REGION();

    Mat_अभिव्यक्ती e;
    MatOp_Initializer::make_अभिव्यक्ती(e, 'I', Size(cols, rows), type);
    परत e;
}

Mat_अभिव्यक्ती Mat::eye(Size size, पूर्णांक type)
{
    CV_INSTRUMENT_REGION();

    Mat_अभिव्यक्ती e;
    MatOp_Initializer::make_अभिव्यक्ती(e, 'I', size, type);
    परत e;
}

शून्य Mat_अभिव्यक्ती::swap(Mat_अभिव्यक्ती& other)
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

आदान_Array_2::आदान_Array_2(स्थिरांक Mat_अभिव्यक्ती& अभिव्यक्ती)
{
    जर (!isIdentity(अभिव्यक्ती))
    {
        Mat result = अभिव्यक्ती;  // TODO improve through refcount == 1 of expr.a (inplace operation is possible - except gemm?)
        Mat_अभिव्यक्ती result_अभिव्यक्ती(result);
        swap(const_cast<Mat_अभिव्यक्ती&>(अभिव्यक्ती), result_अभिव्यक्ती);
    }
    CV_Assert(isIdentity(अभिव्यक्ती));
    init(FIXED_TYPE + FIXED_SIZE + MAT + ACCESS_READ, &अभिव्यक्ती.a);
}

} // cv::
