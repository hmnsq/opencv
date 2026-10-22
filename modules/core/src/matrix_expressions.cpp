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
स्थिर शून्य checkOperandsExist(ध्रुव Mat& a)
{
    सचे (a.empty())
    {
        CV_Error(cv::Error::StsBadArg, "Matrix operand is an empty matrix.");
    }
}

स्थिर शून्य checkOperandsExist(ध्रुव Mat& a, ध्रुव Mat& b)
{
    सचे (a.empty() || b.empty())
    {
        CV_Error(cv::Error::StsBadArg, "One or more matrix operands are empty.");
    }
}


वग्ग MatOp_Identity CV_FINAL : सार्वजनिक MatOp
{
सार्वजनिक:
    MatOp_Identity() {}
    आभास ~MatOp_Identity() {}

    तार्किक elementWise(ध्रुव Mat_व्यञ्जकम्& /*व्यञ्जकम्*/) ध्रुव CV_OVERRIDE { पटिनिवत्त सच्च; }
    शून्य assign(ध्रुव Mat_व्यञ्जकम्& व्यञ्जकम्, Mat& m, पूर्णाङ्क type=-1) ध्रुव CV_OVERRIDE;

    स्थिर शून्य make_व्यञ्जकम्(Mat_व्यञ्जकम्& res, ध्रुव Mat& m);
};

स्थिर MatOp_Identity g_MatOp_Identity;

वग्ग MatOp_AddEx CV_FINAL : सार्वजनिक MatOp
{
सार्वजनिक:
    MatOp_AddEx() {}
    आभास ~MatOp_AddEx() {}

    तार्किक elementWise(ध्रुव Mat_व्यञ्जकम्& /*व्यञ्जकम्*/) ध्रुव CV_OVERRIDE { पटिनिवत्त सच्च; }
    शून्य assign(ध्रुव Mat_व्यञ्जकम्& व्यञ्जकम्, Mat& m, पूर्णाङ्क type=-1) ध्रुव CV_OVERRIDE;

    शून्य add(ध्रुव Mat_व्यञ्जकम्& e1, ध्रुव Scalar& s, Mat_व्यञ्जकम्& res) ध्रुव CV_OVERRIDE;
    शून्य subtract(ध्रुव Scalar& s, ध्रुव Mat_व्यञ्जकम्& व्यञ्जकम्, Mat_व्यञ्जकम्& res) ध्रुव CV_OVERRIDE;
    शून्य multiply(ध्रुव Mat_व्यञ्जकम्& e1, द्विगुण s, Mat_व्यञ्जकम्& res) ध्रुव CV_OVERRIDE;
    शून्य divide(द्विगुण s, ध्रुव Mat_व्यञ्जकम्& e, Mat_व्यञ्जकम्& res) ध्रुव CV_OVERRIDE;

    शून्य transpose(ध्रुव Mat_व्यञ्जकम्& e1, Mat_व्यञ्जकम्& res) ध्रुव CV_OVERRIDE;
    शून्य abs(ध्रुव Mat_व्यञ्जकम्& व्यञ्जकम्, Mat_व्यञ्जकम्& res) ध्रुव CV_OVERRIDE;

    स्थिर शून्य make_व्यञ्जकम्(Mat_व्यञ्जकम्& res, ध्रुव Mat& a, ध्रुव Mat& b, द्विगुण alpha, द्विगुण beta, ध्रुव Scalar& s=Scalar());
};

स्थिर MatOp_AddEx g_MatOp_AddEx;

वग्ग MatOp_Bin CV_FINAL : सार्वजनिक MatOp
{
सार्वजनिक:
    MatOp_Bin() {}
    आभास ~MatOp_Bin() {}

    तार्किक elementWise(ध्रुव Mat_व्यञ्जकम्& /*व्यञ्जकम्*/) ध्रुव CV_OVERRIDE { पटिनिवत्त सच्च; }
    शून्य assign(ध्रुव Mat_व्यञ्जकम्& व्यञ्जकम्, Mat& m, पूर्णाङ्क type=-1) ध्रुव CV_OVERRIDE;

    शून्य multiply(ध्रुव Mat_व्यञ्जकम्& e1, द्विगुण s, Mat_व्यञ्जकम्& res) ध्रुव CV_OVERRIDE;
    शून्य divide(द्विगुण s, ध्रुव Mat_व्यञ्जकम्& e, Mat_व्यञ्जकम्& res) ध्रुव CV_OVERRIDE;

    स्थिर शून्य make_व्यञ्जकम्(Mat_व्यञ्जकम्& res, वर्ण op, ध्रुव Mat& a, ध्रुव Mat& b, द्विगुण scale=1);
    स्थिर शून्य make_व्यञ्जकम्(Mat_व्यञ्जकम्& res, वर्ण op, ध्रुव Mat& a, ध्रुव Scalar& s);
};

स्थिर MatOp_Bin g_MatOp_Bin;

वग्ग MatOp_Cmp CV_FINAL : सार्वजनिक MatOp
{
सार्वजनिक:
    MatOp_Cmp() {}
    आभास ~MatOp_Cmp() {}

    तार्किक elementWise(ध्रुव Mat_व्यञ्जकम्& /*व्यञ्जकम्*/) ध्रुव CV_OVERRIDE { पटिनिवत्त सच्च; }
    शून्य assign(ध्रुव Mat_व्यञ्जकम्& व्यञ्जकम्, Mat& m, पूर्णाङ्क type=-1) ध्रुव CV_OVERRIDE;

    स्थिर शून्य make_व्यञ्जकम्(Mat_व्यञ्जकम्& res, पूर्णाङ्क cmpop, ध्रुव Mat& a, ध्रुव Mat& b);
    स्थिर शून्य make_व्यञ्जकम्(Mat_व्यञ्जकम्& res, पूर्णाङ्क cmpop, ध्रुव Mat& a, द्विगुण alpha);
};

स्थिर MatOp_Cmp g_MatOp_Cmp;

वग्ग MatOp_GEMM CV_FINAL : सार्वजनिक MatOp
{
सार्वजनिक:
    MatOp_GEMM() {}
    आभास ~MatOp_GEMM() {}

    तार्किक elementWise(ध्रुव Mat_व्यञ्जकम्& /*व्यञ्जकम्*/) ध्रुव CV_OVERRIDE { पटिनिवत्त मिच्छा; }
    शून्य assign(ध्रुव Mat_व्यञ्जकम्& व्यञ्जकम्, Mat& m, पूर्णाङ्क type=-1) ध्रुव CV_OVERRIDE;

    शून्य add(ध्रुव Mat_व्यञ्जकम्& e1, ध्रुव Mat_व्यञ्जकम्& e2, Mat_व्यञ्जकम्& res) ध्रुव CV_OVERRIDE;
    शून्य subtract(ध्रुव Mat_व्यञ्जकम्& e1, ध्रुव Mat_व्यञ्जकम्& e2, Mat_व्यञ्जकम्& res) ध्रुव CV_OVERRIDE;
    शून्य multiply(ध्रुव Mat_व्यञ्जकम्& e, द्विगुण s, Mat_व्यञ्जकम्& res) ध्रुव CV_OVERRIDE;

    शून्य transpose(ध्रुव Mat_व्यञ्जकम्& व्यञ्जकम्, Mat_व्यञ्जकम्& res) ध्रुव CV_OVERRIDE;

    Size size(ध्रुव Mat_व्यञ्जकम्& व्यञ्जकम्) ध्रुव CV_OVERRIDE
    {
        पटिनिवत्त Size(
            (व्यञ्जकम्.flags & GEMM_2_T) ? व्यञ्जकम्.b.rows : व्यञ्जकम्.b.cols,
            (व्यञ्जकम्.flags & GEMM_1_T) ? व्यञ्जकम्.a.cols : व्यञ्जकम्.a.rows
        );
    }

    स्थिर शून्य make_व्यञ्जकम्(Mat_व्यञ्जकम्& res, पूर्णाङ्क flags, ध्रुव Mat& a, ध्रुव Mat& b,
                         द्विगुण alpha=1, ध्रुव Mat& c=Mat(), द्विगुण beta=1);

};

स्थिर MatOp_GEMM g_MatOp_GEMM;

वग्ग MatOp_Invert CV_FINAL : सार्वजनिक MatOp
{
सार्वजनिक:
    MatOp_Invert() {}
    आभास ~MatOp_Invert() {}

    तार्किक elementWise(ध्रुव Mat_व्यञ्जकम्& /*व्यञ्जकम्*/) ध्रुव CV_OVERRIDE { पटिनिवत्त मिच्छा; }
    शून्य assign(ध्रुव Mat_व्यञ्जकम्& व्यञ्जकम्, Mat& m, पूर्णाङ्क type=-1) ध्रुव CV_OVERRIDE;

    शून्य matmul(ध्रुव Mat_व्यञ्जकम्& expr1, ध्रुव Mat_व्यञ्जकम्& expr2, Mat_व्यञ्जकम्& res) ध्रुव CV_OVERRIDE;

    स्थिर शून्य make_व्यञ्जकम्(Mat_व्यञ्जकम्& res, पूर्णाङ्क method, ध्रुव Mat& m);
};

स्थिर MatOp_Invert g_MatOp_Invert;

वग्ग MatOp_T CV_FINAL : सार्वजनिक MatOp
{
सार्वजनिक:
    MatOp_T() {}
    आभास ~MatOp_T() {}

    तार्किक elementWise(ध्रुव Mat_व्यञ्जकम्& /*व्यञ्जकम्*/) ध्रुव CV_OVERRIDE { पटिनिवत्त मिच्छा; }
    शून्य assign(ध्रुव Mat_व्यञ्जकम्& व्यञ्जकम्, Mat& m, पूर्णाङ्क type=-1) ध्रुव CV_OVERRIDE;

    शून्य multiply(ध्रुव Mat_व्यञ्जकम्& e1, द्विगुण s, Mat_व्यञ्जकम्& res) ध्रुव CV_OVERRIDE;
    शून्य transpose(ध्रुव Mat_व्यञ्जकम्& व्यञ्जकम्, Mat_व्यञ्जकम्& res) ध्रुव CV_OVERRIDE;

    स्थिर शून्य make_व्यञ्जकम्(Mat_व्यञ्जकम्& res, ध्रुव Mat& a, द्विगुण alpha=1);
};

स्थिर MatOp_T g_MatOp_T;

वग्ग MatOp_Solve CV_FINAL : सार्वजनिक MatOp
{
सार्वजनिक:
    MatOp_Solve() {}
    आभास ~MatOp_Solve() {}

    तार्किक elementWise(ध्रुव Mat_व्यञ्जकम्& /*व्यञ्जकम्*/) ध्रुव CV_OVERRIDE { पटिनिवत्त मिच्छा; }
    शून्य assign(ध्रुव Mat_व्यञ्जकम्& व्यञ्जकम्, Mat& m, पूर्णाङ्क type=-1) ध्रुव CV_OVERRIDE;

    स्थिर शून्य make_व्यञ्जकम्(Mat_व्यञ्जकम्& res, पूर्णाङ्क method, ध्रुव Mat& a, ध्रुव Mat& b);
};

स्थिर MatOp_Solve g_MatOp_Solve;

वग्ग MatOp_Initializer CV_FINAL : सार्वजनिक MatOp
{
सार्वजनिक:
    MatOp_Initializer() {}
    आभास ~MatOp_Initializer() {}

    तार्किक elementWise(ध्रुव Mat_व्यञ्जकम्& /*व्यञ्जकम्*/) ध्रुव CV_OVERRIDE { पटिनिवत्त मिच्छा; }
    शून्य assign(ध्रुव Mat_व्यञ्जकम्& व्यञ्जकम्, Mat& m, पूर्णाङ्क type=-1) ध्रुव CV_OVERRIDE;

    शून्य multiply(ध्रुव Mat_व्यञ्जकम्& e, द्विगुण s, Mat_व्यञ्जकम्& res) ध्रुव CV_OVERRIDE;

    स्थिर शून्य make_व्यञ्जकम्(Mat_व्यञ्जकम्& res, पूर्णाङ्क method, Size sz, पूर्णाङ्क type, द्विगुण alpha=1);
    स्थिर शून्य make_व्यञ्जकम्(Mat_व्यञ्जकम्& res, पूर्णाङ्क method, पूर्णाङ्क ndims, ध्रुव पूर्णाङ्क* sizes, पूर्णाङ्क type, द्विगुण alpha=1);
};

स्थिर MatOp_Initializer* getGlobalMatOpInitializer()
{
    CV_SINGLETON_LAZY_INIT(MatOp_Initializer, नूतन MatOp_Initializer())
}

स्थिर अन्तःस्थ तार्किक isIdentity(ध्रुव Mat_व्यञ्जकम्& e) { पटिनिवत्त e.op == &g_MatOp_Identity; }
स्थिर अन्तःस्थ तार्किक isAddEx(ध्रुव Mat_व्यञ्जकम्& e) { पटिनिवत्त e.op == &g_MatOp_AddEx; }
स्थिर अन्तःस्थ तार्किक isScaled(ध्रुव Mat_व्यञ्जकम्& e) { पटिनिवत्त isAddEx(e) && (!e.b.data || e.beta == 0) && e.s == Scalar(); }
स्थिर अन्तःस्थ तार्किक isBin(ध्रुव Mat_व्यञ्जकम्& e, वर्ण c) { पटिनिवत्त e.op == &g_MatOp_Bin && e.flags == c; }
स्थिर अन्तःस्थ तार्किक isCmp(ध्रुव Mat_व्यञ्जकम्& e) { पटिनिवत्त e.op == &g_MatOp_Cmp; }
स्थिर अन्तःस्थ तार्किक isReciprocal(ध्रुव Mat_व्यञ्जकम्& e) { पटिनिवत्त isBin(e,'/') && (!e.b.data || e.beta == 0); }
स्थिर अन्तःस्थ तार्किक isT(ध्रुव Mat_व्यञ्जकम्& e) { पटिनिवत्त e.op == &g_MatOp_T; }
स्थिर अन्तःस्थ तार्किक isInv(ध्रुव Mat_व्यञ्जकम्& e) { पटिनिवत्त e.op == &g_MatOp_Invert; }
स्थिर अन्तःस्थ तार्किक isSolve(ध्रुव Mat_व्यञ्जकम्& e) { पटिनिवत्त e.op == &g_MatOp_Solve; }
//static inline bool isGEMM(const MatExpr& e) { return e.op == &g_MatOp_GEMM; }
स्थिर अन्तःस्थ तार्किक isMatProd(ध्रुव Mat_व्यञ्जकम्& e) { पटिनिवत्त e.op == &g_MatOp_GEMM && (!e.c.data || e.beta == 0); }
स्थिर अन्तःस्थ तार्किक isInitializer(ध्रुव Mat_व्यञ्जकम्& e) { पटिनिवत्त e.op == getGlobalMatOpInitializer(); }

/////////////////////////////////////////////////////////////////////////////////////////////////////

MatOp::MatOp() {}
MatOp::~MatOp() {}


तार्किक MatOp::elementWise(ध्रुव Mat_व्यञ्जकम्& /*व्यञ्जकम्*/) ध्रुव
{
    पटिनिवत्त मिच्छा;
}

शून्य MatOp::roi(ध्रुव Mat_व्यञ्जकम्& व्यञ्जकम्, ध्रुव Range& rowRange, ध्रुव Range& colRange, Mat_व्यञ्जकम्& e) ध्रुव
{
    सचे( elementWise(व्यञ्जकम्) )
    {
        e = Mat_व्यञ्जकम्(व्यञ्जकम्.op, व्यञ्जकम्.flags, Mat(), Mat(), Mat(),
                    व्यञ्जकम्.alpha, व्यञ्जकम्.beta, व्यञ्जकम्.s);
        सचे(व्यञ्जकम्.a.data)
            e.a = व्यञ्जकम्.a(rowRange, colRange);
        सचे(व्यञ्जकम्.b.data)
            e.b = व्यञ्जकम्.b(rowRange, colRange);
        सचे(व्यञ्जकम्.c.data)
            e.c = व्यञ्जकम्.c(rowRange, colRange);
    }
    अञ्ञथा
    {
        Mat m;
        व्यञ्जकम्.op->assign(व्यञ्जकम्, m);
        e = Mat_व्यञ्जकम्(&g_MatOp_Identity, 0, m(rowRange, colRange), Mat(), Mat());
    }
}

शून्य MatOp::diag(ध्रुव Mat_व्यञ्जकम्& व्यञ्जकम्, पूर्णाङ्क d, Mat_व्यञ्जकम्& e) ध्रुव
{
    सचे( elementWise(व्यञ्जकम्) )
    {
        e = Mat_व्यञ्जकम्(व्यञ्जकम्.op, व्यञ्जकम्.flags, Mat(), Mat(), Mat(),
                    व्यञ्जकम्.alpha, व्यञ्जकम्.beta, व्यञ्जकम्.s);
        सचे(व्यञ्जकम्.a.data)
            e.a = व्यञ्जकम्.a.diag(d);
        सचे(व्यञ्जकम्.b.data)
            e.b = व्यञ्जकम्.b.diag(d);
        सचे(व्यञ्जकम्.c.data)
            e.c = व्यञ्जकम्.c.diag(d);
    }
    अञ्ञथा
    {
        Mat m;
        व्यञ्जकम्.op->assign(व्यञ्जकम्, m);
        e = Mat_व्यञ्जकम्(&g_MatOp_Identity, 0, m.diag(d), Mat(), Mat());
    }
}


शून्य MatOp::augAssignAdd(ध्रुव Mat_व्यञ्जकम्& व्यञ्जकम्, Mat& m) ध्रुव
{
    Mat temp;
    व्यञ्जकम्.op->assign(व्यञ्जकम्, temp);
    m += temp;
}


शून्य MatOp::augAssignSubtract(ध्रुव Mat_व्यञ्जकम्& व्यञ्जकम्, Mat& m) ध्रुव
{
    Mat temp;
    व्यञ्जकम्.op->assign(व्यञ्जकम्, temp);
    m -= temp;
}


शून्य MatOp::augAssignMultiply(ध्रुव Mat_व्यञ्जकम्& व्यञ्जकम्, Mat& m) ध्रुव
{
    Mat temp;
    व्यञ्जकम्.op->assign(व्यञ्जकम्, temp);
    m *= temp;
}


शून्य MatOp::augAssignDivide(ध्रुव Mat_व्यञ्जकम्& व्यञ्जकम्, Mat& m) ध्रुव
{
    Mat temp;
    व्यञ्जकम्.op->assign(व्यञ्जकम्, temp);
    m /= temp;
}


शून्य MatOp::augAssignAnd(ध्रुव Mat_व्यञ्जकम्& व्यञ्जकम्, Mat& m) ध्रुव
{
    Mat temp;
    व्यञ्जकम्.op->assign(व्यञ्जकम्, temp);
    m &= temp;
}


शून्य MatOp::augAssignOr(ध्रुव Mat_व्यञ्जकम्& व्यञ्जकम्, Mat& m) ध्रुव
{
    Mat temp;
    व्यञ्जकम्.op->assign(व्यञ्जकम्, temp);
    m |= temp;
}


शून्य MatOp::augAssignXor(ध्रुव Mat_व्यञ्जकम्& व्यञ्जकम्, Mat& m) ध्रुव
{
    Mat temp;
    व्यञ्जकम्.op->assign(व्यञ्जकम्, temp);
    m ^= temp;
}


शून्य MatOp::add(ध्रुव Mat_व्यञ्जकम्& e1, ध्रुव Mat_व्यञ्जकम्& e2, Mat_व्यञ्जकम्& res) ध्रुव
{
    CV_INSTRUMENT_REGION();

    सचे( एतत् == e2.op )
    {
        द्विगुण alpha = 1, beta = 1;
        Scalar s;
        Mat m1, m2;
        सचे( isAddEx(e1) && (!e1.b.data || e1.beta == 0) )
        {
            m1 = e1.a;
            alpha = e1.alpha;
            s = e1.s;
        }
        अञ्ञथा
            e1.op->assign(e1, m1);

        सचे( isAddEx(e2) && (!e2.b.data || e2.beta == 0) )
        {
            m2 = e2.a;
            beta = e2.alpha;
            s += e2.s;
        }
        अञ्ञथा
            e2.op->assign(e2, m2);
        MatOp_AddEx::make_व्यञ्जकम्(res, m1, m2, alpha, beta, s);
    }
    अञ्ञथा
        e2.op->add(e1, e2, res);
}


शून्य MatOp::add(ध्रुव Mat_व्यञ्जकम्& expr1, ध्रुव Scalar& s, Mat_व्यञ्जकम्& res) ध्रुव
{
    CV_INSTRUMENT_REGION();

    Mat m1;
    expr1.op->assign(expr1, m1);
    MatOp_AddEx::make_व्यञ्जकम्(res, m1, Mat(), 1, 0, s);
}


शून्य MatOp::subtract(ध्रुव Mat_व्यञ्जकम्& e1, ध्रुव Mat_व्यञ्जकम्& e2, Mat_व्यञ्जकम्& res) ध्रुव
{
    CV_INSTRUMENT_REGION();

    सचे( एतत् == e2.op )
    {
        द्विगुण alpha = 1, beta = -1;
        Scalar s;
        Mat m1, m2;
        सचे( isAddEx(e1) && (!e1.b.data || e1.beta == 0) )
        {
            m1 = e1.a;
            alpha = e1.alpha;
            s = e1.s;
        }
        अञ्ञथा
            e1.op->assign(e1, m1);

        सचे( isAddEx(e2) && (!e2.b.data || e2.beta == 0) )
        {
            m2 = e2.a;
            beta = -e2.alpha;
            s -= e2.s;
        }
        अञ्ञथा
            e2.op->assign(e2, m2);
        MatOp_AddEx::make_व्यञ्जकम्(res, m1, m2, alpha, beta, s);
    }
    अञ्ञथा
        e2.op->subtract(e1, e2, res);
}


शून्य MatOp::subtract(ध्रुव Scalar& s, ध्रुव Mat_व्यञ्जकम्& व्यञ्जकम्, Mat_व्यञ्जकम्& res) ध्रुव
{
    CV_INSTRUMENT_REGION();

    Mat m;
    व्यञ्जकम्.op->assign(व्यञ्जकम्, m);
    MatOp_AddEx::make_व्यञ्जकम्(res, m, Mat(), -1, 0, s);
}


शून्य MatOp::multiply(ध्रुव Mat_व्यञ्जकम्& e1, ध्रुव Mat_व्यञ्जकम्& e2, Mat_व्यञ्जकम्& res, द्विगुण scale) ध्रुव
{
    CV_INSTRUMENT_REGION();

    सचे( एतत् == e2.op )
    {
        Mat m1, m2;

        सचे( isReciprocal(e1) )
        {
            सचे( isScaled(e2) )
            {
                scale *= e2.alpha;
                m2 = e2.a;
            }
            अञ्ञथा
                e2.op->assign(e2, m2);

            MatOp_Bin::make_व्यञ्जकम्(res, '/', m2, e1.a, scale/e1.alpha);
        }
        अञ्ञथा
        {
            वर्ण op = '*';
            सचे( isScaled(e1) )
            {
                m1 = e1.a;
                scale *= e1.alpha;
            }
            अञ्ञथा
                e1.op->assign(e1, m1);

            सचे( isScaled(e2) )
            {
                m2 = e2.a;
                scale *= e2.alpha;
            }
            अञ्ञथा सचे( isReciprocal(e2) )
            {
                op = '/';
                m2 = e2.a;
                scale *= e2.alpha;
            }
            अञ्ञथा
                e2.op->assign(e2, m2);

            MatOp_Bin::make_व्यञ्जकम्(res, op, m1, m2, scale);
        }
    }
    अञ्ञथा
        e2.op->multiply(e1, e2, res, scale);
}


शून्य MatOp::multiply(ध्रुव Mat_व्यञ्जकम्& व्यञ्जकम्, द्विगुण s, Mat_व्यञ्जकम्& res) ध्रुव
{
    CV_INSTRUMENT_REGION();

    Mat m;
    व्यञ्जकम्.op->assign(व्यञ्जकम्, m);
    MatOp_AddEx::make_व्यञ्जकम्(res, m, Mat(), s, 0);
}


शून्य MatOp::divide(ध्रुव Mat_व्यञ्जकम्& e1, ध्रुव Mat_व्यञ्जकम्& e2, Mat_व्यञ्जकम्& res, द्विगुण scale) ध्रुव
{
    CV_INSTRUMENT_REGION();

    सचे( एतत् == e2.op )
    {
        सचे( isReciprocal(e1) && isReciprocal(e2) )
            MatOp_Bin::make_व्यञ्जकम्(res, '/', e2.a, e1.a, e1.alpha/e2.alpha);
        अञ्ञथा
        {
            Mat m1, m2;
            वर्ण op = '/';

            सचे( isScaled(e1) )
            {
                m1 = e1.a;
                scale *= e1.alpha;
            }
            अञ्ञथा
                e1.op->assign(e1, m1);

            सचे( isScaled(e2) )
            {
                m2 = e2.a;
                scale /= e2.alpha;
            }
            अञ्ञथा सचे( isReciprocal(e2) )
            {
                m2 = e2.a;
                scale /= e2.alpha;
                op = '*';
            }
            अञ्ञथा
                e2.op->assign(e2, m2);
            MatOp_Bin::make_व्यञ्जकम्(res, op, m1, m2, scale);
        }
    }
    अञ्ञथा
        e2.op->divide(e1, e2, res, scale);
}


शून्य MatOp::divide(द्विगुण s, ध्रुव Mat_व्यञ्जकम्& व्यञ्जकम्, Mat_व्यञ्जकम्& res) ध्रुव
{
    CV_INSTRUMENT_REGION();

    Mat m;
    व्यञ्जकम्.op->assign(व्यञ्जकम्, m);
    MatOp_Bin::make_व्यञ्जकम्(res, '/', m, Mat(), s);
}


शून्य MatOp::abs(ध्रुव Mat_व्यञ्जकम्& व्यञ्जकम्, Mat_व्यञ्जकम्& res) ध्रुव
{
    CV_INSTRUMENT_REGION();

    Mat m;
    व्यञ्जकम्.op->assign(व्यञ्जकम्, m);
    MatOp_Bin::make_व्यञ्जकम्(res, 'a', m, Mat());
}


शून्य MatOp::transpose(ध्रुव Mat_व्यञ्जकम्& व्यञ्जकम्, Mat_व्यञ्जकम्& res) ध्रुव
{
    CV_INSTRUMENT_REGION();

    Mat m;
    व्यञ्जकम्.op->assign(व्यञ्जकम्, m);
    MatOp_T::make_व्यञ्जकम्(res, m, 1);
}


शून्य MatOp::matmul(ध्रुव Mat_व्यञ्जकम्& e1, ध्रुव Mat_व्यञ्जकम्& e2, Mat_व्यञ्जकम्& res) ध्रुव
{
    सचे( एतत् == e2.op )
    {
        द्विगुण scale = 1;
        पूर्णाङ्क flags = 0;
        Mat m1, m2;

        सचे( isT(e1) )
        {
            flags = cv::GEMM_1_T;
            scale = e1.alpha;
            m1 = e1.a;
        }
        अञ्ञथा सचे( isScaled(e1) )
        {
            scale = e1.alpha;
            m1 = e1.a;
        }
        अञ्ञथा
            e1.op->assign(e1, m1);

        सचे( isT(e2) )
        {
            flags |= cv::GEMM_2_T;
            scale *= e2.alpha;
            m2 = e2.a;
        }
        अञ्ञथा सचे( isScaled(e2) )
        {
            scale *= e2.alpha;
            m2 = e2.a;
        }
        अञ्ञथा
            e2.op->assign(e2, m2);

        MatOp_GEMM::make_व्यञ्जकम्(res, flags, m1, m2, scale);
    }
    अञ्ञथा
        e2.op->matmul(e1, e2, res);
}


शून्य MatOp::invert(ध्रुव Mat_व्यञ्जकम्& व्यञ्जकम्, पूर्णाङ्क method, Mat_व्यञ्जकम्& res) ध्रुव
{
    Mat m;
    व्यञ्जकम्.op->assign(व्यञ्जकम्, m);
    MatOp_Invert::make_व्यञ्जकम्(res, method, m);
}


Size MatOp::size(ध्रुव Mat_व्यञ्जकम्& व्यञ्जकम्) ध्रुव
{
    पटिनिवत्त !व्यञ्जकम्.a.empty() ? व्यञ्जकम्.a.size() : व्यञ्जकम्.b.empty() ? व्यञ्जकम्.b.size() : व्यञ्जकम्.c.size();
}

पूर्णाङ्क MatOp::type(ध्रुव Mat_व्यञ्जकम्& व्यञ्जकम्) ध्रुव
{
    CV_INSTRUMENT_REGION();

    पटिनिवत्त !व्यञ्जकम्.a.empty() ? व्यञ्जकम्.a.type() : व्यञ्जकम्.b.empty() ? व्यञ्जकम्.b.type() : व्यञ्जकम्.c.type();
}

//////////////////////////////////////////////////////////////////////////////////////////////////

Mat_व्यञ्जकम्::Mat_व्यञ्जकम्(ध्रुव Mat& m) : op(&g_MatOp_Identity), flags(0), a(m), b(Mat()), c(Mat()), alpha(1), beta(0), s(Scalar())
{
}

Mat_व्यञ्जकम् Mat_व्यञ्जकम्::row(पूर्णाङ्क y) ध्रुव
{
    Mat_व्यञ्जकम् e;
    op->roi(*एतत्, Range(y, y+1), Range::all(), e);
    पटिनिवत्त e;
}

Mat_व्यञ्जकम् Mat_व्यञ्जकम्::col(पूर्णाङ्क x) ध्रुव
{
    Mat_व्यञ्जकम् e;
    op->roi(*एतत्, Range::all(), Range(x, x+1), e);
    पटिनिवत्त e;
}

Mat_व्यञ्जकम् Mat_व्यञ्जकम्::diag(पूर्णाङ्क d) ध्रुव
{
    Mat_व्यञ्जकम् e;
    op->diag(*एतत्, d, e);
    पटिनिवत्त e;
}

Mat_व्यञ्जकम् Mat_व्यञ्जकम्::operator()( ध्रुव Range& rowRange, ध्रुव Range& colRange ) ध्रुव
{
    Mat_व्यञ्जकम् e;
    op->roi(*एतत्, rowRange, colRange, e);
    पटिनिवत्त e;
}

Mat_व्यञ्जकम् Mat_व्यञ्जकम्::operator()( ध्रुव Rect& roi ) ध्रुव
{
    Mat_व्यञ्जकम् e;
    op->roi(*एतत्, Range(roi.y, roi.y + roi.height), Range(roi.x, roi.x + roi.width), e);
    पटिनिवत्त e;
}

Mat Mat_व्यञ्जकम्::cross(ध्रुव Mat& m) ध्रुव
{
    पटिनिवत्त ((Mat)*एतत्).cross(m);
}

द्विगुण Mat_व्यञ्जकम्::dot(ध्रुव Mat& m) ध्रुव
{
    पटिनिवत्त ((Mat)*एतत्).dot(m);
}

Mat_व्यञ्जकम् Mat_व्यञ्जकम्::t() ध्रुव
{
    Mat_व्यञ्जकम् e;
    op->transpose(*एतत्, e);
    पटिनिवत्त e;
}

Mat_व्यञ्जकम् Mat_व्यञ्जकम्::inv(पूर्णाङ्क method) ध्रुव
{
    Mat_व्यञ्जकम् e;
    op->invert(*एतत्, method, e);
    पटिनिवत्त e;
}

Mat_व्यञ्जकम् Mat_व्यञ्जकम्::mul(ध्रुव Mat_व्यञ्जकम्& e, द्विगुण scale) ध्रुव
{
    Mat_व्यञ्जकम् en;
    op->multiply(*एतत्, e, en, scale);
    पटिनिवत्त en;
}

Mat_व्यञ्जकम् Mat_व्यञ्जकम्::mul(ध्रुव Mat& m, द्विगुण scale) ध्रुव
{
    Mat_व्यञ्जकम् e;
    op->multiply(*एतत्, Mat_व्यञ्जकम्(m), e, scale);
    पटिनिवत्त e;
}

Mat_व्यञ्जकम् operator + (ध्रुव Mat& a, ध्रुव Mat& b)
{
    checkOperandsExist(a, b);
    Mat_व्यञ्जकम् e;
    MatOp_AddEx::make_व्यञ्जकम्(e, a, b, 1, 1);
    पटिनिवत्त e;
}

Mat_व्यञ्जकम् operator + (ध्रुव Mat& a, ध्रुव Scalar& s)
{
    checkOperandsExist(a);
    Mat_व्यञ्जकम् e;
    MatOp_AddEx::make_व्यञ्जकम्(e, a, Mat(), 1, 0, s);
    पटिनिवत्त e;
}

Mat_व्यञ्जकम् operator + (ध्रुव Scalar& s, ध्रुव Mat& a)
{
    checkOperandsExist(a);
    Mat_व्यञ्जकम् e;
    MatOp_AddEx::make_व्यञ्जकम्(e, a, Mat(), 1, 0, s);
    पटिनिवत्त e;
}

Mat_व्यञ्जकम् operator + (ध्रुव Mat_व्यञ्जकम्& e, ध्रुव Mat& m)
{
    Mat_व्यञ्जकम् en;
    e.op->add(e, Mat_व्यञ्जकम्(m), en);
    पटिनिवत्त en;
}

Mat_व्यञ्जकम् operator + (ध्रुव Mat& m, ध्रुव Mat_व्यञ्जकम्& e)
{
    checkOperandsExist(m);
    Mat_व्यञ्जकम् en;
    e.op->add(e, Mat_व्यञ्जकम्(m), en);
    पटिनिवत्त en;
}

Mat_व्यञ्जकम् operator + (ध्रुव Mat_व्यञ्जकम्& e, ध्रुव Scalar& s)
{
    Mat_व्यञ्जकम् en;
    e.op->add(e, s, en);
    पटिनिवत्त en;
}

Mat_व्यञ्जकम् operator + (ध्रुव Scalar& s, ध्रुव Mat_व्यञ्जकम्& e)
{
    Mat_व्यञ्जकम् en;
    e.op->add(e, s, en);
    पटिनिवत्त en;
}

Mat_व्यञ्जकम् operator + (ध्रुव Mat_व्यञ्जकम्& e1, ध्रुव Mat_व्यञ्जकम्& e2)
{
    Mat_व्यञ्जकम् en;
    e1.op->add(e1, e2, en);
    पटिनिवत्त en;
}

Mat_व्यञ्जकम् operator - (ध्रुव Mat& a, ध्रुव Mat& b)
{
    checkOperandsExist(a, b);
    Mat_व्यञ्जकम् e;
    MatOp_AddEx::make_व्यञ्जकम्(e, a, b, 1, -1);
    पटिनिवत्त e;
}

Mat_व्यञ्जकम् operator - (ध्रुव Mat& a, ध्रुव Scalar& s)
{
    checkOperandsExist(a);
    Mat_व्यञ्जकम् e;
    MatOp_AddEx::make_व्यञ्जकम्(e, a, Mat(), 1, 0, -s);
    पटिनिवत्त e;
}

Mat_व्यञ्जकम् operator - (ध्रुव Scalar& s, ध्रुव Mat& a)
{
    checkOperandsExist(a);
    Mat_व्यञ्जकम् e;
    MatOp_AddEx::make_व्यञ्जकम्(e, a, Mat(), -1, 0, s);
    पटिनिवत्त e;
}

Mat_व्यञ्जकम् operator - (ध्रुव Mat_व्यञ्जकम्& e, ध्रुव Mat& m)
{
    checkOperandsExist(m);
    Mat_व्यञ्जकम् en;
    e.op->subtract(e, Mat_व्यञ्जकम्(m), en);
    पटिनिवत्त en;
}

Mat_व्यञ्जकम् operator - (ध्रुव Mat& m, ध्रुव Mat_व्यञ्जकम्& e)
{
    checkOperandsExist(m);
    Mat_व्यञ्जकम् en;
    e.op->subtract(Mat_व्यञ्जकम्(m), e, en);
    पटिनिवत्त en;
}

Mat_व्यञ्जकम् operator - (ध्रुव Mat_व्यञ्जकम्& e, ध्रुव Scalar& s)
{
    Mat_व्यञ्जकम् en;
    e.op->add(e, -s, en);
    पटिनिवत्त en;
}

Mat_व्यञ्जकम् operator - (ध्रुव Scalar& s, ध्रुव Mat_व्यञ्जकम्& e)
{
    Mat_व्यञ्जकम् en;
    e.op->subtract(s, e, en);
    पटिनिवत्त en;
}

Mat_व्यञ्जकम् operator - (ध्रुव Mat_व्यञ्जकम्& e1, ध्रुव Mat_व्यञ्जकम्& e2)
{
    Mat_व्यञ्जकम् en;
    e1.op->subtract(e1, e2, en);
    पटिनिवत्त en;
}

Mat_व्यञ्जकम् operator - (ध्रुव Mat& m)
{
    checkOperandsExist(m);
    Mat_व्यञ्जकम् e;
    MatOp_AddEx::make_व्यञ्जकम्(e, m, Mat(), -1, 0);
    पटिनिवत्त e;
}

Mat_व्यञ्जकम् operator - (ध्रुव Mat_व्यञ्जकम्& e)
{
    Mat_व्यञ्जकम् en;
    e.op->subtract(Scalar(0), e, en);
    पटिनिवत्त en;
}

Mat_व्यञ्जकम् operator * (ध्रुव Mat& a, ध्रुव Mat& b)
{
    checkOperandsExist(a, b);
    Mat_व्यञ्जकम् e;
    MatOp_GEMM::make_व्यञ्जकम्(e, 0, a, b);
    पटिनिवत्त e;
}

Mat_व्यञ्जकम् operator * (ध्रुव Mat& a, द्विगुण s)
{
    checkOperandsExist(a);
    Mat_व्यञ्जकम् e;
    MatOp_AddEx::make_व्यञ्जकम्(e, a, Mat(), s, 0);
    पटिनिवत्त e;
}

Mat_व्यञ्जकम् operator * (द्विगुण s, ध्रुव Mat& a)
{
    checkOperandsExist(a);
    Mat_व्यञ्जकम् e;
    MatOp_AddEx::make_व्यञ्जकम्(e, a, Mat(), s, 0);
    पटिनिवत्त e;
}

Mat_व्यञ्जकम् operator * (ध्रुव Mat_व्यञ्जकम्& e, ध्रुव Mat& m)
{
    Mat_व्यञ्जकम् en;
    e.op->matmul(e, Mat_व्यञ्जकम्(m), en);
    पटिनिवत्त en;
}

Mat_व्यञ्जकम् operator * (ध्रुव Mat& m, ध्रुव Mat_व्यञ्जकम्& e)
{
    checkOperandsExist(m);
    Mat_व्यञ्जकम् en;
    e.op->matmul(Mat_व्यञ्जकम्(m), e, en);
    पटिनिवत्त en;
}

Mat_व्यञ्जकम् operator * (ध्रुव Mat_व्यञ्जकम्& e, द्विगुण s)
{
    Mat_व्यञ्जकम् en;
    e.op->multiply(e, s, en);
    पटिनिवत्त en;
}

Mat_व्यञ्जकम् operator * (द्विगुण s, ध्रुव Mat_व्यञ्जकम्& e)
{
    Mat_व्यञ्जकम् en;
    e.op->multiply(e, s, en);
    पटिनिवत्त en;
}

Mat_व्यञ्जकम् operator * (ध्रुव Mat_व्यञ्जकम्& e1, ध्रुव Mat_व्यञ्जकम्& e2)
{
    Mat_व्यञ्जकम् en;
    e1.op->matmul(e1, e2, en);
    पटिनिवत्त en;
}

Mat_व्यञ्जकम् operator / (ध्रुव Mat& a, ध्रुव Mat& b)
{
    checkOperandsExist(a, b);
    Mat_व्यञ्जकम् e;
    MatOp_Bin::make_व्यञ्जकम्(e, '/', a, b);
    पटिनिवत्त e;
}

Mat_व्यञ्जकम् operator / (ध्रुव Mat& a, द्विगुण s)
{
    checkOperandsExist(a);
    Mat_व्यञ्जकम् e;
    MatOp_AddEx::make_व्यञ्जकम्(e, a, Mat(), 1./s, 0);
    पटिनिवत्त e;
}

Mat_व्यञ्जकम् operator / (द्विगुण s, ध्रुव Mat& a)
{
    checkOperandsExist(a);
    Mat_व्यञ्जकम् e;
    MatOp_Bin::make_व्यञ्जकम्(e, '/', a, Mat(), s);
    पटिनिवत्त e;
}

Mat_व्यञ्जकम् operator / (ध्रुव Mat_व्यञ्जकम्& e, ध्रुव Mat& m)
{
    Mat_व्यञ्जकम् en;
    e.op->divide(e, Mat_व्यञ्जकम्(m), en);
    पटिनिवत्त en;
}

Mat_व्यञ्जकम् operator / (ध्रुव Mat& m, ध्रुव Mat_व्यञ्जकम्& e)
{
    checkOperandsExist(m);
    Mat_व्यञ्जकम् en;
    e.op->divide(Mat_व्यञ्जकम्(m), e, en);
    पटिनिवत्त en;
}

Mat_व्यञ्जकम् operator / (ध्रुव Mat_व्यञ्जकम्& e, द्विगुण s)
{
    Mat_व्यञ्जकम् en;
    e.op->multiply(e, 1./s, en);
    पटिनिवत्त en;
}

Mat_व्यञ्जकम् operator / (द्विगुण s, ध्रुव Mat_व्यञ्जकम्& e)
{
    Mat_व्यञ्जकम् en;
    e.op->divide(s, e, en);
    पटिनिवत्त en;
}

Mat_व्यञ्जकम् operator / (ध्रुव Mat_व्यञ्जकम्& e1, ध्रुव Mat_व्यञ्जकम्& e2)
{
    Mat_व्यञ्जकम् en;
    e1.op->divide(e1, e2, en);
    पटिनिवत्त en;
}

Mat_व्यञ्जकम् operator < (ध्रुव Mat& a, ध्रुव Mat& b)
{
    checkOperandsExist(a, b);
    Mat_व्यञ्जकम् e;
    MatOp_Cmp::make_व्यञ्जकम्(e, cv::CMP_LT, a, b);
    पटिनिवत्त e;
}

Mat_व्यञ्जकम् operator < (ध्रुव Mat& a, द्विगुण s)
{
    checkOperandsExist(a);
    Mat_व्यञ्जकम् e;
    MatOp_Cmp::make_व्यञ्जकम्(e, cv::CMP_LT, a, s);
    पटिनिवत्त e;
}

Mat_व्यञ्जकम् operator < (द्विगुण s, ध्रुव Mat& a)
{
    checkOperandsExist(a);
    Mat_व्यञ्जकम् e;
    MatOp_Cmp::make_व्यञ्जकम्(e, cv::CMP_GT, a, s);
    पटिनिवत्त e;
}

Mat_व्यञ्जकम् operator <= (ध्रुव Mat& a, ध्रुव Mat& b)
{
    checkOperandsExist(a, b);
    Mat_व्यञ्जकम् e;
    MatOp_Cmp::make_व्यञ्जकम्(e, cv::CMP_LE, a, b);
    पटिनिवत्त e;
}

Mat_व्यञ्जकम् operator <= (ध्रुव Mat& a, द्विगुण s)
{
    checkOperandsExist(a);
    Mat_व्यञ्जकम् e;
    MatOp_Cmp::make_व्यञ्जकम्(e, cv::CMP_LE, a, s);
    पटिनिवत्त e;
}

Mat_व्यञ्जकम् operator <= (द्विगुण s, ध्रुव Mat& a)
{
    checkOperandsExist(a);
    Mat_व्यञ्जकम् e;
    MatOp_Cmp::make_व्यञ्जकम्(e, cv::CMP_GE, a, s);
    पटिनिवत्त e;
}

Mat_व्यञ्जकम् operator == (ध्रुव Mat& a, ध्रुव Mat& b)
{
    checkOperandsExist(a, b);
    Mat_व्यञ्जकम् e;
    MatOp_Cmp::make_व्यञ्जकम्(e, cv::CMP_EQ, a, b);
    पटिनिवत्त e;
}

Mat_व्यञ्जकम् operator == (ध्रुव Mat& a, द्विगुण s)
{
    checkOperandsExist(a);
    Mat_व्यञ्जकम् e;
    MatOp_Cmp::make_व्यञ्जकम्(e, cv::CMP_EQ, a, s);
    पटिनिवत्त e;
}

Mat_व्यञ्जकम् operator == (द्विगुण s, ध्रुव Mat& a)
{
    checkOperandsExist(a);
    Mat_व्यञ्जकम् e;
    MatOp_Cmp::make_व्यञ्जकम्(e, cv::CMP_EQ, a, s);
    पटिनिवत्त e;
}

Mat_व्यञ्जकम् operator != (ध्रुव Mat& a, ध्रुव Mat& b)
{
    checkOperandsExist(a, b);
    Mat_व्यञ्जकम् e;
    MatOp_Cmp::make_व्यञ्जकम्(e, cv::CMP_NE, a, b);
    पटिनिवत्त e;
}

Mat_व्यञ्जकम् operator != (ध्रुव Mat& a, द्विगुण s)
{
    checkOperandsExist(a);
    Mat_व्यञ्जकम् e;
    MatOp_Cmp::make_व्यञ्जकम्(e, cv::CMP_NE, a, s);
    पटिनिवत्त e;
}

Mat_व्यञ्जकम् operator != (द्विगुण s, ध्रुव Mat& a)
{
    checkOperandsExist(a);
    Mat_व्यञ्जकम् e;
    MatOp_Cmp::make_व्यञ्जकम्(e, cv::CMP_NE, a, s);
    पटिनिवत्त e;
}

Mat_व्यञ्जकम् operator >= (ध्रुव Mat& a, ध्रुव Mat& b)
{
    checkOperandsExist(a, b);
    Mat_व्यञ्जकम् e;
    MatOp_Cmp::make_व्यञ्जकम्(e, cv::CMP_GE, a, b);
    पटिनिवत्त e;
}

Mat_व्यञ्जकम् operator >= (ध्रुव Mat& a, द्विगुण s)
{
    checkOperandsExist(a);
    Mat_व्यञ्जकम् e;
    MatOp_Cmp::make_व्यञ्जकम्(e, cv::CMP_GE, a, s);
    पटिनिवत्त e;
}

Mat_व्यञ्जकम् operator >= (द्विगुण s, ध्रुव Mat& a)
{
    checkOperandsExist(a);
    Mat_व्यञ्जकम् e;
    MatOp_Cmp::make_व्यञ्जकम्(e, cv::CMP_LE, a, s);
    पटिनिवत्त e;
}

Mat_व्यञ्जकम् operator > (ध्रुव Mat& a, ध्रुव Mat& b)
{
    checkOperandsExist(a, b);
    Mat_व्यञ्जकम् e;
    MatOp_Cmp::make_व्यञ्जकम्(e, cv::CMP_GT, a, b);
    पटिनिवत्त e;
}

Mat_व्यञ्जकम् operator > (ध्रुव Mat& a, द्विगुण s)
{
    checkOperandsExist(a);
    Mat_व्यञ्जकम् e;
    MatOp_Cmp::make_व्यञ्जकम्(e, cv::CMP_GT, a, s);
    पटिनिवत्त e;
}

Mat_व्यञ्जकम् operator > (द्विगुण s, ध्रुव Mat& a)
{
    checkOperandsExist(a);
    Mat_व्यञ्जकम् e;
    MatOp_Cmp::make_व्यञ्जकम्(e, cv::CMP_LT, a, s);
    पटिनिवत्त e;
}

Mat_व्यञ्जकम् min(ध्रुव Mat& a, ध्रुव Mat& b)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a, b);
    Mat_व्यञ्जकम् e;
    MatOp_Bin::make_व्यञ्जकम्(e, 'm', a, b);
    पटिनिवत्त e;
}

Mat_व्यञ्जकम् min(ध्रुव Mat& a, द्विगुण s)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    Mat_व्यञ्जकम् e;
    MatOp_Bin::make_व्यञ्जकम्(e, 'n', a, s);
    पटिनिवत्त e;
}

Mat_व्यञ्जकम् min(द्विगुण s, ध्रुव Mat& a)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    Mat_व्यञ्जकम् e;
    MatOp_Bin::make_व्यञ्जकम्(e, 'n', a, s);
    पटिनिवत्त e;
}

Mat_व्यञ्जकम् max(ध्रुव Mat& a, ध्रुव Mat& b)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a, b);
    Mat_व्यञ्जकम् e;
    MatOp_Bin::make_व्यञ्जकम्(e, 'M', a, b);
    पटिनिवत्त e;
}

Mat_व्यञ्जकम् max(ध्रुव Mat& a, द्विगुण s)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    Mat_व्यञ्जकम् e;
    MatOp_Bin::make_व्यञ्जकम्(e, 'N', a, s);
    पटिनिवत्त e;
}

Mat_व्यञ्जकम् max(द्विगुण s, ध्रुव Mat& a)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    Mat_व्यञ्जकम् e;
    MatOp_Bin::make_व्यञ्जकम्(e, 'N', a, s);
    पटिनिवत्त e;
}

Mat_व्यञ्जकम् operator & (ध्रुव Mat& a, ध्रुव Mat& b)
{
    checkOperandsExist(a, b);
    Mat_व्यञ्जकम् e;
    MatOp_Bin::make_व्यञ्जकम्(e, '&', a, b);
    पटिनिवत्त e;
}

Mat_व्यञ्जकम् operator & (ध्रुव Mat& a, ध्रुव Scalar& s)
{
    checkOperandsExist(a);
    Mat_व्यञ्जकम् e;
    MatOp_Bin::make_व्यञ्जकम्(e, '&', a, s);
    पटिनिवत्त e;
}

Mat_व्यञ्जकम् operator & (ध्रुव Scalar& s, ध्रुव Mat& a)
{
    checkOperandsExist(a);
    Mat_व्यञ्जकम् e;
    MatOp_Bin::make_व्यञ्जकम्(e, '&', a, s);
    पटिनिवत्त e;
}

Mat_व्यञ्जकम् operator | (ध्रुव Mat& a, ध्रुव Mat& b)
{
    checkOperandsExist(a, b);
    Mat_व्यञ्जकम् e;
    MatOp_Bin::make_व्यञ्जकम्(e, '|', a, b);
    पटिनिवत्त e;
}

Mat_व्यञ्जकम् operator | (ध्रुव Mat& a, ध्रुव Scalar& s)
{
    checkOperandsExist(a);
    Mat_व्यञ्जकम् e;
    MatOp_Bin::make_व्यञ्जकम्(e, '|', a, s);
    पटिनिवत्त e;
}

Mat_व्यञ्जकम् operator | (ध्रुव Scalar& s, ध्रुव Mat& a)
{
    checkOperandsExist(a);
    Mat_व्यञ्जकम् e;
    MatOp_Bin::make_व्यञ्जकम्(e, '|', a, s);
    पटिनिवत्त e;
}

Mat_व्यञ्जकम् operator ^ (ध्रुव Mat& a, ध्रुव Mat& b)
{
    checkOperandsExist(a, b);
    Mat_व्यञ्जकम् e;
    MatOp_Bin::make_व्यञ्जकम्(e, '^', a, b);
    पटिनिवत्त e;
}

Mat_व्यञ्जकम् operator ^ (ध्रुव Mat& a, ध्रुव Scalar& s)
{
    checkOperandsExist(a);
    Mat_व्यञ्जकम् e;
    MatOp_Bin::make_व्यञ्जकम्(e, '^', a, s);
    पटिनिवत्त e;
}

Mat_व्यञ्जकम् operator ^ (ध्रुव Scalar& s, ध्रुव Mat& a)
{
    checkOperandsExist(a);
    Mat_व्यञ्जकम् e;
    MatOp_Bin::make_व्यञ्जकम्(e, '^', a, s);
    पटिनिवत्त e;
}

Mat_व्यञ्जकम् operator ~(ध्रुव Mat& a)
{
    checkOperandsExist(a);
    Mat_व्यञ्जकम् e;
    MatOp_Bin::make_व्यञ्जकम्(e, '~', a, Scalar());
    पटिनिवत्त e;
}

Mat_व्यञ्जकम् abs(ध्रुव Mat& a)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    Mat_व्यञ्जकम् e;
    MatOp_Bin::make_व्यञ्जकम्(e, 'a', a, Scalar());
    पटिनिवत्त e;
}

Mat_व्यञ्जकम् abs(ध्रुव Mat_व्यञ्जकम्& e)
{
    CV_INSTRUMENT_REGION();

    Mat_व्यञ्जकम् en;
    e.op->abs(e, en);
    पटिनिवत्त en;
}


Size Mat_व्यञ्जकम्::size() ध्रुव
{
    सचे( isT(*एतत्) || isInv(*एतत्) )
        पटिनिवत्त Size(a.rows, a.cols);
    सचे( isSolve(*एतत्) )
        पटिनिवत्त Size(b.cols, a.cols);
    सचे( isInitializer(*एतत्) )
        पटिनिवत्त a.size();
    पटिनिवत्त op ? op->size(*एतत्) : Size();
}


पूर्णाङ्क Mat_व्यञ्जकम्::type() ध्रुव
{
    CV_INSTRUMENT_REGION();

    सचे( isInitializer(*एतत्) )
        पटिनिवत्त a.type();
    सचे( isCmp(*एतत्) )
        पटिनिवत्त CV_MAKETYPE(CV_8U, a.channels());
    पटिनिवत्त op ? op->type(*एतत्) : -1;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////

शून्य MatOp_Identity::assign(ध्रुव Mat_व्यञ्जकम्& e, Mat& m, पूर्णाङ्क _type) ध्रुव
{
    सचे( _type == -1 || _type == e.a.type() )
        m = e.a;
    अञ्ञथा
    {
        CV_Assert( CV_MAT_CN(_type) == e.a.channels() );
        e.a.convertTo(m, _type);
    }
}

अन्तःस्थ शून्य MatOp_Identity::make_व्यञ्जकम्(Mat_व्यञ्जकम्& res, ध्रुव Mat& m)
{
    res = Mat_व्यञ्जकम्(&g_MatOp_Identity, 0, m, Mat(), Mat(), 1, 0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

शून्य MatOp_AddEx::assign(ध्रुव Mat_व्यञ्जकम्& e, Mat& m, पूर्णाङ्क _type) ध्रुव
{
    Mat temp, &dst = _type == -1 || e.a.type() == _type ? m : temp;
    सचे( e.b.data )
    {
        सचे( e.s == Scalar() || !e.s.isReal() )
        {
            सचे( e.alpha == 1 )
            {
                सचे( e.beta == 1 )
                    cv::add(e.a, e.b, dst);
                अञ्ञथा सचे( e.beta == -1 )
                    cv::subtract(e.a, e.b, dst);
                अञ्ञथा
                    cv::scaleAdd(e.b, e.beta, e.a, dst);
            }
            अञ्ञथा सचे( e.beta == 1 )
            {
                सचे( e.alpha == -1 )
                    cv::subtract(e.b, e.a, dst);
                अञ्ञथा
                    cv::scaleAdd(e.a, e.alpha, e.b, dst);
            }
            अञ्ञथा
                cv::addWeighted(e.a, e.alpha, e.b, e.beta, 0, dst);

            सचे( !e.s.isReal() )
                cv::add(dst, e.s, dst);
        }
        अञ्ञथा
        {
            सचे (e.a.channels() > 1)
                CV_LOG_ONCE_WARNING(रिक्त, "OpenCV/MatExpr: processing of multi-channel arrays might be changed in the future: "
                                          "https://github.com/opencv/opencv/issues/16739");
            cv::addWeighted(e.a, e.alpha, e.b, e.beta, e.s[0], dst);
        }
    }
    अञ्ञथा सचे( e.s.isReal() && (dst.data != m.data || fabs(e.alpha) != 1))
    {
        सचे (e.a.channels() > 1 && e.s[0] != 0.0)
            CV_LOG_ONCE_WARNING(रिक्त, "OpenCV/MatExpr: processing of multi-channel arrays might be changed in the future: "
                                      "https://github.com/opencv/opencv/issues/16739");
        e.a.convertTo(m, _type, e.alpha, e.s[0]);
        पटिनिवत्त;
    }
    अञ्ञथा सचे( e.alpha == 1 )
        cv::add(e.a, e.s, dst);
    अञ्ञथा सचे( e.alpha == -1 )
        cv::subtract(e.s, e.a, dst);
    अञ्ञथा
    {
        e.a.convertTo(dst, e.a.type(), e.alpha);
        cv::add(dst, e.s, dst);
    }

    सचे( dst.data != m.data )
        dst.convertTo(m, m.type());
}


शून्य MatOp_AddEx::add(ध्रुव Mat_व्यञ्जकम्& e, ध्रुव Scalar& s, Mat_व्यञ्जकम्& res) ध्रुव
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.s += s;
}


शून्य MatOp_AddEx::subtract(ध्रुव Scalar& s, ध्रुव Mat_व्यञ्जकम्& e, Mat_व्यञ्जकम्& res) ध्रुव
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha = -res.alpha;
    res.beta = -res.beta;
    res.s = s - res.s;
}

शून्य MatOp_AddEx::multiply(ध्रुव Mat_व्यञ्जकम्& e, द्विगुण s, Mat_व्यञ्जकम्& res) ध्रुव
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha *= s;
    res.beta *= s;
    res.s *= s;
}

शून्य MatOp_AddEx::divide(द्विगुण s, ध्रुव Mat_व्यञ्जकम्& e, Mat_व्यञ्जकम्& res) ध्रुव
{
    CV_INSTRUMENT_REGION();

    सचे( isScaled(e) )
        MatOp_Bin::make_व्यञ्जकम्(res, '/', e.a, Mat(), s/e.alpha);
    अञ्ञथा
        MatOp::divide(s, e, res);
}


शून्य MatOp_AddEx::transpose(ध्रुव Mat_व्यञ्जकम्& e, Mat_व्यञ्जकम्& res) ध्रुव
{
    CV_INSTRUMENT_REGION();

    सचे( isScaled(e) )
        MatOp_T::make_व्यञ्जकम्(res, e.a, e.alpha);
    अञ्ञथा
        MatOp::transpose(e, res);
}

शून्य MatOp_AddEx::abs(ध्रुव Mat_व्यञ्जकम्& e, Mat_व्यञ्जकम्& res) ध्रुव
{
    CV_INSTRUMENT_REGION();

    सचे( (!e.b.data || e.beta == 0) && fabs(e.alpha) == 1 )
        MatOp_Bin::make_व्यञ्जकम्(res, 'a', e.a, -e.s*e.alpha);
    अञ्ञथा सचे( e.b.data && e.alpha + e.beta == 0 && e.alpha*e.beta == -1 )
        MatOp_Bin::make_व्यञ्जकम्(res, 'a', e.a, e.b);
    अञ्ञथा
        MatOp::abs(e, res);
}

अन्तःस्थ शून्य MatOp_AddEx::make_व्यञ्जकम्(Mat_व्यञ्जकम्& res, ध्रुव Mat& a, ध्रुव Mat& b, द्विगुण alpha, द्विगुण beta, ध्रुव Scalar& s)
{
    res = Mat_व्यञ्जकम्(&g_MatOp_AddEx, 0, a, b, Mat(), alpha, beta, s);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

शून्य MatOp_Bin::assign(ध्रुव Mat_व्यञ्जकम्& e, Mat& m, पूर्णाङ्क _type) ध्रुव
{
    Mat temp, &dst = _type == -1 || e.a.type() == _type ? m : temp;

    सचे( e.flags == '*' )
        cv::multiply(e.a, e.b, dst, e.alpha);
    अञ्ञथा सचे( e.flags == '/' && e.b.data )
        cv::divide(e.a, e.b, dst, e.alpha);
    अञ्ञथा सचे( e.flags == '/' && !e.b.data )
        cv::divide(e.alpha, e.a, dst );
    अञ्ञथा सचे( e.flags == '&' && e.b.data )
        bitwise_and(e.a, e.b, dst);
    अञ्ञथा सचे( e.flags == '&' && !e.b.data )
        bitwise_and(e.a, e.s, dst);
    अञ्ञथा सचे( e.flags == '|' && e.b.data )
        bitwise_or(e.a, e.b, dst);
    अञ्ञथा सचे( e.flags == '|' && !e.b.data )
        bitwise_or(e.a, e.s, dst);
    अञ्ञथा सचे( e.flags == '^' && e.b.data )
        bitwise_xor(e.a, e.b, dst);
    अञ्ञथा सचे( e.flags == '^' && !e.b.data )
        bitwise_xor(e.a, e.s, dst);
    अञ्ञथा सचे( e.flags == '~' && !e.b.data )
        bitwise_not(e.a, dst);
    अञ्ञथा सचे( e.flags == 'm' )
        cv::min(e.a, e.b, dst);
    अञ्ञथा सचे( e.flags == 'n' )
        cv::min(e.a, e.s[0], dst);
    अञ्ञथा सचे( e.flags == 'M' )
        cv::max(e.a, e.b, dst);
    अञ्ञथा सचे( e.flags == 'N' )
        cv::max(e.a, e.s[0], dst);
    अञ्ञथा सचे( e.flags == 'a' && e.b.data )
        cv::absdiff(e.a, e.b, dst);
    अञ्ञथा सचे( e.flags == 'a' && !e.b.data )
        cv::absdiff(e.a, e.s, dst);
    अञ्ञथा
        CV_Error(cv::Error::StsError, "Unknown operation");

    सचे( dst.data != m.data )
        dst.convertTo(m, _type);
}

शून्य MatOp_Bin::multiply(ध्रुव Mat_व्यञ्जकम्& e, द्विगुण s, Mat_व्यञ्जकम्& res) ध्रुव
{
    CV_INSTRUMENT_REGION();

    सचे( e.flags == '*' || e.flags == '/' )
    {
        res = e;
        res.alpha *= s;
    }
    अञ्ञथा
        MatOp::multiply(e, s, res);
}

शून्य MatOp_Bin::divide(द्विगुण s, ध्रुव Mat_व्यञ्जकम्& e, Mat_व्यञ्जकम्& res) ध्रुव
{
    CV_INSTRUMENT_REGION();

    सचे( e.flags == '/' && (!e.b.data || e.beta == 0) )
        MatOp_AddEx::make_व्यञ्जकम्(res, e.a, Mat(), s/e.alpha, 0);
    अञ्ञथा
        MatOp::divide(s, e, res);
}

अन्तःस्थ शून्य MatOp_Bin::make_व्यञ्जकम्(Mat_व्यञ्जकम्& res, वर्ण op, ध्रुव Mat& a, ध्रुव Mat& b, द्विगुण scale)
{
    res = Mat_व्यञ्जकम्(&g_MatOp_Bin, op, a, b, Mat(), scale, b.data ? 1 : 0);
}

अन्तःस्थ शून्य MatOp_Bin::make_व्यञ्जकम्(Mat_व्यञ्जकम्& res, वर्ण op, ध्रुव Mat& a, ध्रुव Scalar& s)
{
    res = Mat_व्यञ्जकम्(&g_MatOp_Bin, op, a, Mat(), Mat(), 1, 0, s);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

शून्य MatOp_Cmp::assign(ध्रुव Mat_व्यञ्जकम्& e, Mat& m, पूर्णाङ्क _type) ध्रुव
{
    Mat temp, &dst = _type == -1 || _type == CV_8U ? m : temp;

    सचे( e.b.data )
        cv::compare(e.a, e.b, dst, e.flags);
    अञ्ञथा
        cv::compare(e.a, e.alpha, dst, e.flags);

    सचे( dst.data != m.data )
        dst.convertTo(m, _type);
}

अन्तःस्थ शून्य MatOp_Cmp::make_व्यञ्जकम्(Mat_व्यञ्जकम्& res, पूर्णाङ्क cmpop, ध्रुव Mat& a, ध्रुव Mat& b)
{
    res = Mat_व्यञ्जकम्(&g_MatOp_Cmp, cmpop, a, b, Mat(), 1, 1);
}

अन्तःस्थ शून्य MatOp_Cmp::make_व्यञ्जकम्(Mat_व्यञ्जकम्& res, पूर्णाङ्क cmpop, ध्रुव Mat& a, द्विगुण alpha)
{
    res = Mat_व्यञ्जकम्(&g_MatOp_Cmp, cmpop, a, Mat(), Mat(), alpha, 1);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

शून्य MatOp_T::assign(ध्रुव Mat_व्यञ्जकम्& e, Mat& m, पूर्णाङ्क _type) ध्रुव
{
    Mat temp, &dst = _type == -1 || _type == e.a.type() ? m : temp;

    cv::transpose(e.a, dst);

    सचे( dst.data != m.data || e.alpha != 1 )
        dst.convertTo(m, _type, e.alpha);
}

शून्य MatOp_T::multiply(ध्रुव Mat_व्यञ्जकम्& e, द्विगुण s, Mat_व्यञ्जकम्& res) ध्रुव
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha *= s;
}

शून्य MatOp_T::transpose(ध्रुव Mat_व्यञ्जकम्& e, Mat_व्यञ्जकम्& res) ध्रुव
{
    CV_INSTRUMENT_REGION();

    सचे( e.alpha == 1 )
        MatOp_Identity::make_व्यञ्जकम्(res, e.a);
    अञ्ञथा
        MatOp_AddEx::make_व्यञ्जकम्(res, e.a, Mat(), e.alpha, 0);
}

अन्तःस्थ शून्य MatOp_T::make_व्यञ्जकम्(Mat_व्यञ्जकम्& res, ध्रुव Mat& a, द्विगुण alpha)
{
    res = Mat_व्यञ्जकम्(&g_MatOp_T, 0, a, Mat(), Mat(), alpha, 0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

शून्य MatOp_GEMM::assign(ध्रुव Mat_व्यञ्जकम्& e, Mat& m, पूर्णाङ्क _type) ध्रुव
{
    Mat temp, &dst = _type == -1 || _type == e.a.type() ? m : temp;

    cv::gemm(e.a, e.b, e.alpha, e.c, e.beta, dst, e.flags);
    सचे( dst.data != m.data )
        dst.convertTo(m, _type);
}

शून्य MatOp_GEMM::add(ध्रुव Mat_व्यञ्जकम्& e1, ध्रुव Mat_व्यञ्जकम्& e2, Mat_व्यञ्जकम्& res) ध्रुव
{
    CV_INSTRUMENT_REGION();

    तार्किक i1 = isIdentity(e1), i2 = isIdentity(e2);
    द्विगुण alpha1 = i1 ? 1 : e1.alpha, alpha2 = i2 ? 1 : e2.alpha;

    सचे( isMatProd(e1) && (i2 || isScaled(e2) || isT(e2)) )
        MatOp_GEMM::make_व्यञ्जकम्(res, (e1.flags & ~cv::GEMM_3_T)|(isT(e2) ? cv::GEMM_3_T : 0),
                             e1.a, e1.b, alpha1, e2.a, alpha2);
    अञ्ञथा सचे( isMatProd(e2) && (i1 || isScaled(e1) || isT(e1)) )
        MatOp_GEMM::make_व्यञ्जकम्(res, (e2.flags & ~cv::GEMM_3_T)|(isT(e1) ? cv::GEMM_3_T : 0),
                             e2.a, e2.b, alpha2, e1.a, alpha1);
    अञ्ञथा सचे( एतत् == e2.op )
        MatOp::add(e1, e2, res);
    अञ्ञथा
        e2.op->add(e1, e2, res);
}

शून्य MatOp_GEMM::subtract(ध्रुव Mat_व्यञ्जकम्& e1, ध्रुव Mat_व्यञ्जकम्& e2, Mat_व्यञ्जकम्& res) ध्रुव
{
    CV_INSTRUMENT_REGION();

    तार्किक i1 = isIdentity(e1), i2 = isIdentity(e2);
    द्विगुण alpha1 = i1 ? 1 : e1.alpha, alpha2 = i2 ? 1 : e2.alpha;

    सचे( isMatProd(e1) && (i2 || isScaled(e2) || isT(e2)) )
        MatOp_GEMM::make_व्यञ्जकम्(res, (e1.flags & ~cv::GEMM_3_T)|(isT(e2) ? cv::GEMM_3_T : 0),
                             e1.a, e1.b, alpha1, e2.a, -alpha2);
    अञ्ञथा सचे( isMatProd(e2) && (i1 || isScaled(e1) || isT(e1)) )
        MatOp_GEMM::make_व्यञ्जकम्(res, (e2.flags & ~cv::GEMM_3_T)|(isT(e1) ? cv::GEMM_3_T : 0),
                            e2.a, e2.b, -alpha2, e1.a, alpha1);
    अञ्ञथा सचे( एतत् == e2.op )
        MatOp::subtract(e1, e2, res);
    अञ्ञथा
        e2.op->subtract(e1, e2, res);
}

शून्य MatOp_GEMM::multiply(ध्रुव Mat_व्यञ्जकम्& e, द्विगुण s, Mat_व्यञ्जकम्& res) ध्रुव
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha *= s;
    res.beta *= s;
}

शून्य MatOp_GEMM::transpose(ध्रुव Mat_व्यञ्जकम्& e, Mat_व्यञ्जकम्& res) ध्रुव
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.flags = (!(e.flags & cv::GEMM_1_T) ? cv::GEMM_2_T : 0) |
                (!(e.flags & cv::GEMM_2_T) ? cv::GEMM_1_T : 0) |
                (!(e.flags & cv::GEMM_3_T) ? cv::GEMM_3_T : 0);
    swap(res.a, res.b);
}

अन्तःस्थ शून्य MatOp_GEMM::make_व्यञ्जकम्(Mat_व्यञ्जकम्& res, पूर्णाङ्क flags, ध्रुव Mat& a, ध्रुव Mat& b,
                                 द्विगुण alpha, ध्रुव Mat& c, द्विगुण beta)
{
    res = Mat_व्यञ्जकम्(&g_MatOp_GEMM, flags, a, b, c, alpha, beta);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

शून्य MatOp_Invert::assign(ध्रुव Mat_व्यञ्जकम्& e, Mat& m, पूर्णाङ्क _type) ध्रुव
{
    Mat temp, &dst = _type == -1 || _type == e.a.type() ? m : temp;

    cv::invert(e.a, dst, e.flags);
    सचे( dst.data != m.data )
        dst.convertTo(m, _type);
}

शून्य MatOp_Invert::matmul(ध्रुव Mat_व्यञ्जकम्& e1, ध्रुव Mat_व्यञ्जकम्& e2, Mat_व्यञ्जकम्& res) ध्रुव
{
    सचे( isInv(e1) && isIdentity(e2) )
        MatOp_Solve::make_व्यञ्जकम्(res, e1.flags, e1.a, e2.a);
    अञ्ञथा सचे( एतत् == e2.op )
        MatOp::matmul(e1, e2, res);
    अञ्ञथा
        e2.op->matmul(e1, e2, res);
}

अन्तःस्थ शून्य MatOp_Invert::make_व्यञ्जकम्(Mat_व्यञ्जकम्& res, पूर्णाङ्क method, ध्रुव Mat& m)
{
    res = Mat_व्यञ्जकम्(&g_MatOp_Invert, method, m, Mat(), Mat(), 1, 0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

शून्य MatOp_Solve::assign(ध्रुव Mat_व्यञ्जकम्& e, Mat& m, पूर्णाङ्क _type) ध्रुव
{
    Mat temp, &dst = _type == -1 || _type == e.a.type() ? m : temp;

    cv::solve(e.a, e.b, dst, e.flags);
    सचे( dst.data != m.data )
        dst.convertTo(m, _type);
}

अन्तःस्थ शून्य MatOp_Solve::make_व्यञ्जकम्(Mat_व्यञ्जकम्& res, पूर्णाङ्क method, ध्रुव Mat& a, ध्रुव Mat& b)
{
    res = Mat_व्यञ्जकम्(&g_MatOp_Solve, method, a, b, Mat(), 1, 1);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

शून्य MatOp_Initializer::assign(ध्रुव Mat_व्यञ्जकम्& e, Mat& m, पूर्णाङ्क _type) ध्रुव
{
    सचे( _type == -1 )
        _type = e.a.type();

    m.create(e.a.size, _type);

    सचे( e.flags == 'I' && e.a.dims <= 2 )
        setIdentity(m, Scalar(e.alpha));
    अञ्ञथा सचे( e.flags == '0' )
        m = Scalar();
    अञ्ञथा सचे( e.flags == '1' )
        m = Scalar(e.alpha);
    अञ्ञथा
        CV_Error(cv::Error::StsError, "Invalid matrix initializer type");
}

शून्य MatOp_Initializer::multiply(ध्रुव Mat_व्यञ्जकम्& e, द्विगुण s, Mat_व्यञ्जकम्& res) ध्रुव
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha *= s;
}

अन्तःस्थ शून्य MatOp_Initializer::make_व्यञ्जकम्(Mat_व्यञ्जकम्& res, पूर्णाङ्क method, Size sz, पूर्णाङ्क type, द्विगुण alpha)
{
    res = Mat_व्यञ्जकम्(getGlobalMatOpInitializer(), method, Mat(sz, type, (शून्य*)(size_t)0xEEEEEEEE), Mat(), Mat(), alpha, 0);
}

अन्तःस्थ शून्य MatOp_Initializer::make_व्यञ्जकम्(Mat_व्यञ्जकम्& res, पूर्णाङ्क method, पूर्णाङ्क ndims, ध्रुव पूर्णाङ्क* sizes, पूर्णाङ्क type, द्विगुण alpha)
{
    res = Mat_व्यञ्जकम्(getGlobalMatOpInitializer(), method, Mat(ndims, sizes, type, (शून्य*)(size_t)0xEEEEEEEE), Mat(), Mat(), alpha, 0);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

Mat_व्यञ्जकम् Mat::t() ध्रुव
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(*एतत्);
    Mat_व्यञ्जकम् e;
    MatOp_T::make_व्यञ्जकम्(e, *एतत्);
    पटिनिवत्त e;
}

Mat_व्यञ्जकम् Mat::inv(पूर्णाङ्क method) ध्रुव
{
    CV_INSTRUMENT_REGION();

    Mat_व्यञ्जकम् e;
    MatOp_Invert::make_व्यञ्जकम्(e, method, *एतत्);
    पटिनिवत्त e;
}


Mat_व्यञ्जकम् Mat::mul(निवेशः_Array m, द्विगुण scale) ध्रुव
{
    CV_INSTRUMENT_REGION();

    Mat b = m.getMat();
    // Unless the argument is a refcounted Mat/UMat, the header returned by getMat() may be
    // a non-owning view of caller memory (e.g. a scalar bound to _InputArray(const double&),
    // a Matx or a Vec) that the returned MatExpr can outlive, so snapshot it.
    // See https://github.com/opencv/opencv/issues/23577
    सचे( !m.isMat() && !m.isUMat() )
        b = b.clone();

    Mat_व्यञ्जकम् e;
    MatOp_Bin::make_व्यञ्जकम्(e, '*', *एतत्, b, scale);
    पटिनिवत्त e;
}

Mat_व्यञ्जकम् Mat::zeros(पूर्णाङ्क rows, पूर्णाङ्क cols, पूर्णाङ्क type)
{
    CV_INSTRUMENT_REGION();

    Mat_व्यञ्जकम् e;
    MatOp_Initializer::make_व्यञ्जकम्(e, '0', Size(cols, rows), type);
    पटिनिवत्त e;
}

Mat_व्यञ्जकम् Mat::zeros(Size size, पूर्णाङ्क type)
{
    CV_INSTRUMENT_REGION();

    Mat_व्यञ्जकम् e;
    MatOp_Initializer::make_व्यञ्जकम्(e, '0', size, type);
    पटिनिवत्त e;
}

Mat_व्यञ्जकम् Mat::zeros(पूर्णाङ्क ndims, ध्रुव पूर्णाङ्क* sizes, पूर्णाङ्क type)
{
    CV_INSTRUMENT_REGION();

    Mat_व्यञ्जकम् e;
    MatOp_Initializer::make_व्यञ्जकम्(e, '0', ndims, sizes, type);
    पटिनिवत्त e;
}

Mat_व्यञ्जकम् Mat::zeros(ध्रुव MatShape& shape, पूर्णाङ्क type)
{
    CV_INSTRUMENT_REGION();

    Mat_व्यञ्जकम् e;
    MatOp_Initializer::make_व्यञ्जकम्(e, '0', shape.dims, shape.p, type);
    पटिनिवत्त e;
}

Mat_व्यञ्जकम् Mat::ones(पूर्णाङ्क rows, पूर्णाङ्क cols, पूर्णाङ्क type)
{
    CV_INSTRUMENT_REGION();

    Mat_व्यञ्जकम् e;
    MatOp_Initializer::make_व्यञ्जकम्(e, '1', Size(cols, rows), type);
    पटिनिवत्त e;
}

Mat_व्यञ्जकम् Mat::ones(Size size, पूर्णाङ्क type)
{
    CV_INSTRUMENT_REGION();

    Mat_व्यञ्जकम् e;
    MatOp_Initializer::make_व्यञ्जकम्(e, '1', size, type);
    पटिनिवत्त e;
}

Mat_व्यञ्जकम् Mat::ones(पूर्णाङ्क ndims, ध्रुव पूर्णाङ्क* sizes, पूर्णाङ्क type)
{
    CV_INSTRUMENT_REGION();

    Mat_व्यञ्जकम् e;
    MatOp_Initializer::make_व्यञ्जकम्(e, '1', ndims, sizes, type);
    पटिनिवत्त e;
}

Mat_व्यञ्जकम् Mat::ones(ध्रुव MatShape& shape, पूर्णाङ्क type)
{
    CV_INSTRUMENT_REGION();

    Mat_व्यञ्जकम् e;
    MatOp_Initializer::make_व्यञ्जकम्(e, '1', shape.dims, shape.p, type);
    पटिनिवत्त e;
}

Mat_व्यञ्जकम् Mat::eye(पूर्णाङ्क rows, पूर्णाङ्क cols, पूर्णाङ्क type)
{
    CV_INSTRUMENT_REGION();

    Mat_व्यञ्जकम् e;
    MatOp_Initializer::make_व्यञ्जकम्(e, 'I', Size(cols, rows), type);
    पटिनिवत्त e;
}

Mat_व्यञ्जकम् Mat::eye(Size size, पूर्णाङ्क type)
{
    CV_INSTRUMENT_REGION();

    Mat_व्यञ्जकम् e;
    MatOp_Initializer::make_व्यञ्जकम्(e, 'I', size, type);
    पटिनिवत्त e;
}

शून्य Mat_व्यञ्जकम्::swap(Mat_व्यञ्जकम्& other)
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

निवेशः_Array_2::निवेशः_Array_2(ध्रुव Mat_व्यञ्जकम्& व्यञ्जकम्)
{
    सचे (!isIdentity(व्यञ्जकम्))
    {
        Mat result = व्यञ्जकम्;  // TODO improve through refcount == 1 of expr.a (inplace operation is possible - except gemm?)
        Mat_व्यञ्जकम् result_व्यञ्जकम्(result);
        swap(const_cast<Mat_व्यञ्जकम्&>(व्यञ्जकम्), result_व्यञ्जकम्);
    }
    CV_Assert(isIdentity(व्यञ्जकम्));
    init(FIXED_TYPE + FIXED_SIZE + MAT + ACCESS_READ, &व्यञ्जकम्.a);
}

} // cv::
