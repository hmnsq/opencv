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
    यदि (a.empty())
    {
        CV_Error(cv::Error::StsBadArg, "Matrix operand is an empty matrix.");
    }
}

स्थिर शून्य checkOperandsExist(ध्रुव Mat& a, ध्रुव Mat& b)
{
    यदि (a.empty() || b.empty())
    {
        CV_Error(cv::Error::StsBadArg, "One or more matrix operands are empty.");
    }
}


वर्ग MatOp_Identity CV_FINAL : सार्वजनिक MatOp
{
सार्वजनिक:
    MatOp_Identity() {}
    आभास ~MatOp_Identity() {}

    तार्किक elementWise(ध्रुव Mat_व्यंजक& /*व्यंजक*/) ध्रुव CV_OVERRIDE { प्रत्यागम सत्य; }
    शून्य assign(ध्रुव Mat_व्यंजक& व्यंजक, Mat& m, पूर्णाङ्क type=-1) ध्रुव CV_OVERRIDE;

    स्थिर शून्य make_व्यंजक(Mat_व्यंजक& res, ध्रुव Mat& m);
};

स्थिर MatOp_Identity g_MatOp_Identity;

वर्ग MatOp_AddEx CV_FINAL : सार्वजनिक MatOp
{
सार्वजनिक:
    MatOp_AddEx() {}
    आभास ~MatOp_AddEx() {}

    तार्किक elementWise(ध्रुव Mat_व्यंजक& /*व्यंजक*/) ध्रुव CV_OVERRIDE { प्रत्यागम सत्य; }
    शून्य assign(ध्रुव Mat_व्यंजक& व्यंजक, Mat& m, पूर्णाङ्क type=-1) ध्रुव CV_OVERRIDE;

    शून्य add(ध्रुव Mat_व्यंजक& e1, ध्रुव Scalar& s, Mat_व्यंजक& res) ध्रुव CV_OVERRIDE;
    शून्य subtract(ध्रुव Scalar& s, ध्रुव Mat_व्यंजक& व्यंजक, Mat_व्यंजक& res) ध्रुव CV_OVERRIDE;
    शून्य multiply(ध्रुव Mat_व्यंजक& e1, द्विगुण s, Mat_व्यंजक& res) ध्रुव CV_OVERRIDE;
    शून्य divide(द्विगुण s, ध्रुव Mat_व्यंजक& e, Mat_व्यंजक& res) ध्रुव CV_OVERRIDE;

    शून्य transpose(ध्रुव Mat_व्यंजक& e1, Mat_व्यंजक& res) ध्रुव CV_OVERRIDE;
    शून्य abs(ध्रुव Mat_व्यंजक& व्यंजक, Mat_व्यंजक& res) ध्रुव CV_OVERRIDE;

    स्थिर शून्य make_व्यंजक(Mat_व्यंजक& res, ध्रुव Mat& a, ध्रुव Mat& b, द्विगुण alpha, द्विगुण beta, ध्रुव Scalar& s=Scalar());
};

स्थिर MatOp_AddEx g_MatOp_AddEx;

वर्ग MatOp_Bin CV_FINAL : सार्वजनिक MatOp
{
सार्वजनिक:
    MatOp_Bin() {}
    आभास ~MatOp_Bin() {}

    तार्किक elementWise(ध्रुव Mat_व्यंजक& /*व्यंजक*/) ध्रुव CV_OVERRIDE { प्रत्यागम सत्य; }
    शून्य assign(ध्रुव Mat_व्यंजक& व्यंजक, Mat& m, पूर्णाङ्क type=-1) ध्रुव CV_OVERRIDE;

    शून्य multiply(ध्रुव Mat_व्यंजक& e1, द्विगुण s, Mat_व्यंजक& res) ध्रुव CV_OVERRIDE;
    शून्य divide(द्विगुण s, ध्रुव Mat_व्यंजक& e, Mat_व्यंजक& res) ध्रुव CV_OVERRIDE;

    स्थिर शून्य make_व्यंजक(Mat_व्यंजक& res, वर्ण op, ध्रुव Mat& a, ध्रुव Mat& b, द्विगुण scale=1);
    स्थिर शून्य make_व्यंजक(Mat_व्यंजक& res, वर्ण op, ध्रुव Mat& a, ध्रुव Scalar& s);
};

स्थिर MatOp_Bin g_MatOp_Bin;

वर्ग MatOp_Cmp CV_FINAL : सार्वजनिक MatOp
{
सार्वजनिक:
    MatOp_Cmp() {}
    आभास ~MatOp_Cmp() {}

    तार्किक elementWise(ध्रुव Mat_व्यंजक& /*व्यंजक*/) ध्रुव CV_OVERRIDE { प्रत्यागम सत्य; }
    शून्य assign(ध्रुव Mat_व्यंजक& व्यंजक, Mat& m, पूर्णाङ्क type=-1) ध्रुव CV_OVERRIDE;

    स्थिर शून्य make_व्यंजक(Mat_व्यंजक& res, पूर्णाङ्क cmpop, ध्रुव Mat& a, ध्रुव Mat& b);
    स्थिर शून्य make_व्यंजक(Mat_व्यंजक& res, पूर्णाङ्क cmpop, ध्रुव Mat& a, द्विगुण alpha);
};

स्थिर MatOp_Cmp g_MatOp_Cmp;

वर्ग MatOp_GEMM CV_FINAL : सार्वजनिक MatOp
{
सार्वजनिक:
    MatOp_GEMM() {}
    आभास ~MatOp_GEMM() {}

    तार्किक elementWise(ध्रुव Mat_व्यंजक& /*व्यंजक*/) ध्रुव CV_OVERRIDE { प्रत्यागम असत्य; }
    शून्य assign(ध्रुव Mat_व्यंजक& व्यंजक, Mat& m, पूर्णाङ्क type=-1) ध्रुव CV_OVERRIDE;

    शून्य add(ध्रुव Mat_व्यंजक& e1, ध्रुव Mat_व्यंजक& e2, Mat_व्यंजक& res) ध्रुव CV_OVERRIDE;
    शून्य subtract(ध्रुव Mat_व्यंजक& e1, ध्रुव Mat_व्यंजक& e2, Mat_व्यंजक& res) ध्रुव CV_OVERRIDE;
    शून्य multiply(ध्रुव Mat_व्यंजक& e, द्विगुण s, Mat_व्यंजक& res) ध्रुव CV_OVERRIDE;

    शून्य transpose(ध्रुव Mat_व्यंजक& व्यंजक, Mat_व्यंजक& res) ध्रुव CV_OVERRIDE;

    Size size(ध्रुव Mat_व्यंजक& व्यंजक) ध्रुव CV_OVERRIDE
    {
        प्रत्यागम Size(
            (व्यंजक.flags & GEMM_2_T) ? व्यंजक.b.rows : व्यंजक.b.cols,
            (व्यंजक.flags & GEMM_1_T) ? व्यंजक.a.cols : व्यंजक.a.rows
        );
    }

    स्थिर शून्य make_व्यंजक(Mat_व्यंजक& res, पूर्णाङ्क flags, ध्रुव Mat& a, ध्रुव Mat& b,
                         द्विगुण alpha=1, ध्रुव Mat& c=Mat(), द्विगुण beta=1);

};

स्थिर MatOp_GEMM g_MatOp_GEMM;

वर्ग MatOp_Invert CV_FINAL : सार्वजनिक MatOp
{
सार्वजनिक:
    MatOp_Invert() {}
    आभास ~MatOp_Invert() {}

    तार्किक elementWise(ध्रुव Mat_व्यंजक& /*व्यंजक*/) ध्रुव CV_OVERRIDE { प्रत्यागम असत्य; }
    शून्य assign(ध्रुव Mat_व्यंजक& व्यंजक, Mat& m, पूर्णाङ्क type=-1) ध्रुव CV_OVERRIDE;

    शून्य matmul(ध्रुव Mat_व्यंजक& expr1, ध्रुव Mat_व्यंजक& expr2, Mat_व्यंजक& res) ध्रुव CV_OVERRIDE;

    स्थिर शून्य make_व्यंजक(Mat_व्यंजक& res, पूर्णाङ्क method, ध्रुव Mat& m);
};

स्थिर MatOp_Invert g_MatOp_Invert;

वर्ग MatOp_T CV_FINAL : सार्वजनिक MatOp
{
सार्वजनिक:
    MatOp_T() {}
    आभास ~MatOp_T() {}

    तार्किक elementWise(ध्रुव Mat_व्यंजक& /*व्यंजक*/) ध्रुव CV_OVERRIDE { प्रत्यागम असत्य; }
    शून्य assign(ध्रुव Mat_व्यंजक& व्यंजक, Mat& m, पूर्णाङ्क type=-1) ध्रुव CV_OVERRIDE;

    शून्य multiply(ध्रुव Mat_व्यंजक& e1, द्विगुण s, Mat_व्यंजक& res) ध्रुव CV_OVERRIDE;
    शून्य transpose(ध्रुव Mat_व्यंजक& व्यंजक, Mat_व्यंजक& res) ध्रुव CV_OVERRIDE;

    स्थिर शून्य make_व्यंजक(Mat_व्यंजक& res, ध्रुव Mat& a, द्विगुण alpha=1);
};

स्थिर MatOp_T g_MatOp_T;

वर्ग MatOp_Solve CV_FINAL : सार्वजनिक MatOp
{
सार्वजनिक:
    MatOp_Solve() {}
    आभास ~MatOp_Solve() {}

    तार्किक elementWise(ध्रुव Mat_व्यंजक& /*व्यंजक*/) ध्रुव CV_OVERRIDE { प्रत्यागम असत्य; }
    शून्य assign(ध्रुव Mat_व्यंजक& व्यंजक, Mat& m, पूर्णाङ्क type=-1) ध्रुव CV_OVERRIDE;

    स्थिर शून्य make_व्यंजक(Mat_व्यंजक& res, पूर्णाङ्क method, ध्रुव Mat& a, ध्रुव Mat& b);
};

स्थिर MatOp_Solve g_MatOp_Solve;

वर्ग MatOp_Initializer CV_FINAL : सार्वजनिक MatOp
{
सार्वजनिक:
    MatOp_Initializer() {}
    आभास ~MatOp_Initializer() {}

    तार्किक elementWise(ध्रुव Mat_व्यंजक& /*व्यंजक*/) ध्रुव CV_OVERRIDE { प्रत्यागम असत्य; }
    शून्य assign(ध्रुव Mat_व्यंजक& व्यंजक, Mat& m, पूर्णाङ्क type=-1) ध्रुव CV_OVERRIDE;

    शून्य multiply(ध्रुव Mat_व्यंजक& e, द्विगुण s, Mat_व्यंजक& res) ध्रुव CV_OVERRIDE;

    स्थिर शून्य make_व्यंजक(Mat_व्यंजक& res, पूर्णाङ्क method, Size sz, पूर्णाङ्क type, द्विगुण alpha=1);
    स्थिर शून्य make_व्यंजक(Mat_व्यंजक& res, पूर्णाङ्क method, पूर्णाङ्क ndims, ध्रुव पूर्णाङ्क* sizes, पूर्णाङ्क type, द्विगुण alpha=1);
};

स्थिर MatOp_Initializer* getGlobalMatOpInitializer()
{
    CV_SINGLETON_LAZY_INIT(MatOp_Initializer, नूतन MatOp_Initializer())
}

स्थिर अन्तःस्थ तार्किक isIdentity(ध्रुव Mat_व्यंजक& e) { प्रत्यागम e.op == &g_MatOp_Identity; }
स्थिर अन्तःस्थ तार्किक isAddEx(ध्रुव Mat_व्यंजक& e) { प्रत्यागम e.op == &g_MatOp_AddEx; }
स्थिर अन्तःस्थ तार्किक isScaled(ध्रुव Mat_व्यंजक& e) { प्रत्यागम isAddEx(e) && (!e.b.data || e.beta == 0) && e.s == Scalar(); }
स्थिर अन्तःस्थ तार्किक isBin(ध्रुव Mat_व्यंजक& e, वर्ण c) { प्रत्यागम e.op == &g_MatOp_Bin && e.flags == c; }
स्थिर अन्तःस्थ तार्किक isCmp(ध्रुव Mat_व्यंजक& e) { प्रत्यागम e.op == &g_MatOp_Cmp; }
स्थिर अन्तःस्थ तार्किक isReciprocal(ध्रुव Mat_व्यंजक& e) { प्रत्यागम isBin(e,'/') && (!e.b.data || e.beta == 0); }
स्थिर अन्तःस्थ तार्किक isT(ध्रुव Mat_व्यंजक& e) { प्रत्यागम e.op == &g_MatOp_T; }
स्थिर अन्तःस्थ तार्किक isInv(ध्रुव Mat_व्यंजक& e) { प्रत्यागम e.op == &g_MatOp_Invert; }
स्थिर अन्तःस्थ तार्किक isSolve(ध्रुव Mat_व्यंजक& e) { प्रत्यागम e.op == &g_MatOp_Solve; }
//static inline bool isGEMM(const MatExpr& e) { return e.op == &g_MatOp_GEMM; }
स्थिर अन्तःस्थ तार्किक isMatProd(ध्रुव Mat_व्यंजक& e) { प्रत्यागम e.op == &g_MatOp_GEMM && (!e.c.data || e.beta == 0); }
स्थिर अन्तःस्थ तार्किक isInitializer(ध्रुव Mat_व्यंजक& e) { प्रत्यागम e.op == getGlobalMatOpInitializer(); }

/////////////////////////////////////////////////////////////////////////////////////////////////////

MatOp::MatOp() {}
MatOp::~MatOp() {}


तार्किक MatOp::elementWise(ध्रुव Mat_व्यंजक& /*व्यंजक*/) ध्रुव
{
    प्रत्यागम असत्य;
}

शून्य MatOp::roi(ध्रुव Mat_व्यंजक& व्यंजक, ध्रुव Range& rowRange, ध्रुव Range& colRange, Mat_व्यंजक& e) ध्रुव
{
    यदि( elementWise(व्यंजक) )
    {
        e = Mat_व्यंजक(व्यंजक.op, व्यंजक.flags, Mat(), Mat(), Mat(),
                    व्यंजक.alpha, व्यंजक.beta, व्यंजक.s);
        यदि(व्यंजक.a.data)
            e.a = व्यंजक.a(rowRange, colRange);
        यदि(व्यंजक.b.data)
            e.b = व्यंजक.b(rowRange, colRange);
        यदि(व्यंजक.c.data)
            e.c = व्यंजक.c(rowRange, colRange);
    }
    अन्यथा
    {
        Mat m;
        व्यंजक.op->assign(व्यंजक, m);
        e = Mat_व्यंजक(&g_MatOp_Identity, 0, m(rowRange, colRange), Mat(), Mat());
    }
}

शून्य MatOp::diag(ध्रुव Mat_व्यंजक& व्यंजक, पूर्णाङ्क d, Mat_व्यंजक& e) ध्रुव
{
    यदि( elementWise(व्यंजक) )
    {
        e = Mat_व्यंजक(व्यंजक.op, व्यंजक.flags, Mat(), Mat(), Mat(),
                    व्यंजक.alpha, व्यंजक.beta, व्यंजक.s);
        यदि(व्यंजक.a.data)
            e.a = व्यंजक.a.diag(d);
        यदि(व्यंजक.b.data)
            e.b = व्यंजक.b.diag(d);
        यदि(व्यंजक.c.data)
            e.c = व्यंजक.c.diag(d);
    }
    अन्यथा
    {
        Mat m;
        व्यंजक.op->assign(व्यंजक, m);
        e = Mat_व्यंजक(&g_MatOp_Identity, 0, m.diag(d), Mat(), Mat());
    }
}


शून्य MatOp::augAssignAdd(ध्रुव Mat_व्यंजक& व्यंजक, Mat& m) ध्रुव
{
    Mat temp;
    व्यंजक.op->assign(व्यंजक, temp);
    m += temp;
}


शून्य MatOp::augAssignSubtract(ध्रुव Mat_व्यंजक& व्यंजक, Mat& m) ध्रुव
{
    Mat temp;
    व्यंजक.op->assign(व्यंजक, temp);
    m -= temp;
}


शून्य MatOp::augAssignMultiply(ध्रुव Mat_व्यंजक& व्यंजक, Mat& m) ध्रुव
{
    Mat temp;
    व्यंजक.op->assign(व्यंजक, temp);
    m *= temp;
}


शून्य MatOp::augAssignDivide(ध्रुव Mat_व्यंजक& व्यंजक, Mat& m) ध्रुव
{
    Mat temp;
    व्यंजक.op->assign(व्यंजक, temp);
    m /= temp;
}


शून्य MatOp::augAssignAnd(ध्रुव Mat_व्यंजक& व्यंजक, Mat& m) ध्रुव
{
    Mat temp;
    व्यंजक.op->assign(व्यंजक, temp);
    m &= temp;
}


शून्य MatOp::augAssignOr(ध्रुव Mat_व्यंजक& व्यंजक, Mat& m) ध्रुव
{
    Mat temp;
    व्यंजक.op->assign(व्यंजक, temp);
    m |= temp;
}


शून्य MatOp::augAssignXor(ध्रुव Mat_व्यंजक& व्यंजक, Mat& m) ध्रुव
{
    Mat temp;
    व्यंजक.op->assign(व्यंजक, temp);
    m ^= temp;
}


शून्य MatOp::add(ध्रुव Mat_व्यंजक& e1, ध्रुव Mat_व्यंजक& e2, Mat_व्यंजक& res) ध्रुव
{
    CV_INSTRUMENT_REGION();

    यदि( एतत् == e2.op )
    {
        द्विगुण alpha = 1, beta = 1;
        Scalar s;
        Mat m1, m2;
        यदि( isAddEx(e1) && (!e1.b.data || e1.beta == 0) )
        {
            m1 = e1.a;
            alpha = e1.alpha;
            s = e1.s;
        }
        अन्यथा
            e1.op->assign(e1, m1);

        यदि( isAddEx(e2) && (!e2.b.data || e2.beta == 0) )
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


शून्य MatOp::add(ध्रुव Mat_व्यंजक& expr1, ध्रुव Scalar& s, Mat_व्यंजक& res) ध्रुव
{
    CV_INSTRUMENT_REGION();

    Mat m1;
    expr1.op->assign(expr1, m1);
    MatOp_AddEx::make_व्यंजक(res, m1, Mat(), 1, 0, s);
}


शून्य MatOp::subtract(ध्रुव Mat_व्यंजक& e1, ध्रुव Mat_व्यंजक& e2, Mat_व्यंजक& res) ध्रुव
{
    CV_INSTRUMENT_REGION();

    यदि( एतत् == e2.op )
    {
        द्विगुण alpha = 1, beta = -1;
        Scalar s;
        Mat m1, m2;
        यदि( isAddEx(e1) && (!e1.b.data || e1.beta == 0) )
        {
            m1 = e1.a;
            alpha = e1.alpha;
            s = e1.s;
        }
        अन्यथा
            e1.op->assign(e1, m1);

        यदि( isAddEx(e2) && (!e2.b.data || e2.beta == 0) )
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


शून्य MatOp::subtract(ध्रुव Scalar& s, ध्रुव Mat_व्यंजक& व्यंजक, Mat_व्यंजक& res) ध्रुव
{
    CV_INSTRUMENT_REGION();

    Mat m;
    व्यंजक.op->assign(व्यंजक, m);
    MatOp_AddEx::make_व्यंजक(res, m, Mat(), -1, 0, s);
}


शून्य MatOp::multiply(ध्रुव Mat_व्यंजक& e1, ध्रुव Mat_व्यंजक& e2, Mat_व्यंजक& res, द्विगुण scale) ध्रुव
{
    CV_INSTRUMENT_REGION();

    यदि( एतत् == e2.op )
    {
        Mat m1, m2;

        यदि( isReciprocal(e1) )
        {
            यदि( isScaled(e2) )
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
            वर्ण op = '*';
            यदि( isScaled(e1) )
            {
                m1 = e1.a;
                scale *= e1.alpha;
            }
            अन्यथा
                e1.op->assign(e1, m1);

            यदि( isScaled(e2) )
            {
                m2 = e2.a;
                scale *= e2.alpha;
            }
            अन्यथा यदि( isReciprocal(e2) )
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


शून्य MatOp::multiply(ध्रुव Mat_व्यंजक& व्यंजक, द्विगुण s, Mat_व्यंजक& res) ध्रुव
{
    CV_INSTRUMENT_REGION();

    Mat m;
    व्यंजक.op->assign(व्यंजक, m);
    MatOp_AddEx::make_व्यंजक(res, m, Mat(), s, 0);
}


शून्य MatOp::divide(ध्रुव Mat_व्यंजक& e1, ध्रुव Mat_व्यंजक& e2, Mat_व्यंजक& res, द्विगुण scale) ध्रुव
{
    CV_INSTRUMENT_REGION();

    यदि( एतत् == e2.op )
    {
        यदि( isReciprocal(e1) && isReciprocal(e2) )
            MatOp_Bin::make_व्यंजक(res, '/', e2.a, e1.a, e1.alpha/e2.alpha);
        अन्यथा
        {
            Mat m1, m2;
            वर्ण op = '/';

            यदि( isScaled(e1) )
            {
                m1 = e1.a;
                scale *= e1.alpha;
            }
            अन्यथा
                e1.op->assign(e1, m1);

            यदि( isScaled(e2) )
            {
                m2 = e2.a;
                scale /= e2.alpha;
            }
            अन्यथा यदि( isReciprocal(e2) )
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


शून्य MatOp::divide(द्विगुण s, ध्रुव Mat_व्यंजक& व्यंजक, Mat_व्यंजक& res) ध्रुव
{
    CV_INSTRUMENT_REGION();

    Mat m;
    व्यंजक.op->assign(व्यंजक, m);
    MatOp_Bin::make_व्यंजक(res, '/', m, Mat(), s);
}


शून्य MatOp::abs(ध्रुव Mat_व्यंजक& व्यंजक, Mat_व्यंजक& res) ध्रुव
{
    CV_INSTRUMENT_REGION();

    Mat m;
    व्यंजक.op->assign(व्यंजक, m);
    MatOp_Bin::make_व्यंजक(res, 'a', m, Mat());
}


शून्य MatOp::transpose(ध्रुव Mat_व्यंजक& व्यंजक, Mat_व्यंजक& res) ध्रुव
{
    CV_INSTRUMENT_REGION();

    Mat m;
    व्यंजक.op->assign(व्यंजक, m);
    MatOp_T::make_व्यंजक(res, m, 1);
}


शून्य MatOp::matmul(ध्रुव Mat_व्यंजक& e1, ध्रुव Mat_व्यंजक& e2, Mat_व्यंजक& res) ध्रुव
{
    यदि( एतत् == e2.op )
    {
        द्विगुण scale = 1;
        पूर्णाङ्क flags = 0;
        Mat m1, m2;

        यदि( isT(e1) )
        {
            flags = cv::GEMM_1_T;
            scale = e1.alpha;
            m1 = e1.a;
        }
        अन्यथा यदि( isScaled(e1) )
        {
            scale = e1.alpha;
            m1 = e1.a;
        }
        अन्यथा
            e1.op->assign(e1, m1);

        यदि( isT(e2) )
        {
            flags |= cv::GEMM_2_T;
            scale *= e2.alpha;
            m2 = e2.a;
        }
        अन्यथा यदि( isScaled(e2) )
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


शून्य MatOp::invert(ध्रुव Mat_व्यंजक& व्यंजक, पूर्णाङ्क method, Mat_व्यंजक& res) ध्रुव
{
    Mat m;
    व्यंजक.op->assign(व्यंजक, m);
    MatOp_Invert::make_व्यंजक(res, method, m);
}


Size MatOp::size(ध्रुव Mat_व्यंजक& व्यंजक) ध्रुव
{
    प्रत्यागम !व्यंजक.a.empty() ? व्यंजक.a.size() : व्यंजक.b.empty() ? व्यंजक.b.size() : व्यंजक.c.size();
}

पूर्णाङ्क MatOp::type(ध्रुव Mat_व्यंजक& व्यंजक) ध्रुव
{
    CV_INSTRUMENT_REGION();

    प्रत्यागम !व्यंजक.a.empty() ? व्यंजक.a.type() : व्यंजक.b.empty() ? व्यंजक.b.type() : व्यंजक.c.type();
}

//////////////////////////////////////////////////////////////////////////////////////////////////

Mat_व्यंजक::Mat_व्यंजक(ध्रुव Mat& m) : op(&g_MatOp_Identity), flags(0), a(m), b(Mat()), c(Mat()), alpha(1), beta(0), s(Scalar())
{
}

Mat_व्यंजक Mat_व्यंजक::row(पूर्णाङ्क y) ध्रुव
{
    Mat_व्यंजक e;
    op->roi(*एतत्, Range(y, y+1), Range::all(), e);
    प्रत्यागम e;
}

Mat_व्यंजक Mat_व्यंजक::col(पूर्णाङ्क x) ध्रुव
{
    Mat_व्यंजक e;
    op->roi(*एतत्, Range::all(), Range(x, x+1), e);
    प्रत्यागम e;
}

Mat_व्यंजक Mat_व्यंजक::diag(पूर्णाङ्क d) ध्रुव
{
    Mat_व्यंजक e;
    op->diag(*एतत्, d, e);
    प्रत्यागम e;
}

Mat_व्यंजक Mat_व्यंजक::operator()( ध्रुव Range& rowRange, ध्रुव Range& colRange ) ध्रुव
{
    Mat_व्यंजक e;
    op->roi(*एतत्, rowRange, colRange, e);
    प्रत्यागम e;
}

Mat_व्यंजक Mat_व्यंजक::operator()( ध्रुव Rect& roi ) ध्रुव
{
    Mat_व्यंजक e;
    op->roi(*एतत्, Range(roi.y, roi.y + roi.height), Range(roi.x, roi.x + roi.width), e);
    प्रत्यागम e;
}

Mat Mat_व्यंजक::cross(ध्रुव Mat& m) ध्रुव
{
    प्रत्यागम ((Mat)*एतत्).cross(m);
}

द्विगुण Mat_व्यंजक::dot(ध्रुव Mat& m) ध्रुव
{
    प्रत्यागम ((Mat)*एतत्).dot(m);
}

Mat_व्यंजक Mat_व्यंजक::t() ध्रुव
{
    Mat_व्यंजक e;
    op->transpose(*एतत्, e);
    प्रत्यागम e;
}

Mat_व्यंजक Mat_व्यंजक::inv(पूर्णाङ्क method) ध्रुव
{
    Mat_व्यंजक e;
    op->invert(*एतत्, method, e);
    प्रत्यागम e;
}

Mat_व्यंजक Mat_व्यंजक::mul(ध्रुव Mat_व्यंजक& e, द्विगुण scale) ध्रुव
{
    Mat_व्यंजक en;
    op->multiply(*एतत्, e, en, scale);
    प्रत्यागम en;
}

Mat_व्यंजक Mat_व्यंजक::mul(ध्रुव Mat& m, द्विगुण scale) ध्रुव
{
    Mat_व्यंजक e;
    op->multiply(*एतत्, Mat_व्यंजक(m), e, scale);
    प्रत्यागम e;
}

Mat_व्यंजक operator + (ध्रुव Mat& a, ध्रुव Mat& b)
{
    checkOperandsExist(a, b);
    Mat_व्यंजक e;
    MatOp_AddEx::make_व्यंजक(e, a, b, 1, 1);
    प्रत्यागम e;
}

Mat_व्यंजक operator + (ध्रुव Mat& a, ध्रुव Scalar& s)
{
    checkOperandsExist(a);
    Mat_व्यंजक e;
    MatOp_AddEx::make_व्यंजक(e, a, Mat(), 1, 0, s);
    प्रत्यागम e;
}

Mat_व्यंजक operator + (ध्रुव Scalar& s, ध्रुव Mat& a)
{
    checkOperandsExist(a);
    Mat_व्यंजक e;
    MatOp_AddEx::make_व्यंजक(e, a, Mat(), 1, 0, s);
    प्रत्यागम e;
}

Mat_व्यंजक operator + (ध्रुव Mat_व्यंजक& e, ध्रुव Mat& m)
{
    Mat_व्यंजक en;
    e.op->add(e, Mat_व्यंजक(m), en);
    प्रत्यागम en;
}

Mat_व्यंजक operator + (ध्रुव Mat& m, ध्रुव Mat_व्यंजक& e)
{
    checkOperandsExist(m);
    Mat_व्यंजक en;
    e.op->add(e, Mat_व्यंजक(m), en);
    प्रत्यागम en;
}

Mat_व्यंजक operator + (ध्रुव Mat_व्यंजक& e, ध्रुव Scalar& s)
{
    Mat_व्यंजक en;
    e.op->add(e, s, en);
    प्रत्यागम en;
}

Mat_व्यंजक operator + (ध्रुव Scalar& s, ध्रुव Mat_व्यंजक& e)
{
    Mat_व्यंजक en;
    e.op->add(e, s, en);
    प्रत्यागम en;
}

Mat_व्यंजक operator + (ध्रुव Mat_व्यंजक& e1, ध्रुव Mat_व्यंजक& e2)
{
    Mat_व्यंजक en;
    e1.op->add(e1, e2, en);
    प्रत्यागम en;
}

Mat_व्यंजक operator - (ध्रुव Mat& a, ध्रुव Mat& b)
{
    checkOperandsExist(a, b);
    Mat_व्यंजक e;
    MatOp_AddEx::make_व्यंजक(e, a, b, 1, -1);
    प्रत्यागम e;
}

Mat_व्यंजक operator - (ध्रुव Mat& a, ध्रुव Scalar& s)
{
    checkOperandsExist(a);
    Mat_व्यंजक e;
    MatOp_AddEx::make_व्यंजक(e, a, Mat(), 1, 0, -s);
    प्रत्यागम e;
}

Mat_व्यंजक operator - (ध्रुव Scalar& s, ध्रुव Mat& a)
{
    checkOperandsExist(a);
    Mat_व्यंजक e;
    MatOp_AddEx::make_व्यंजक(e, a, Mat(), -1, 0, s);
    प्रत्यागम e;
}

Mat_व्यंजक operator - (ध्रुव Mat_व्यंजक& e, ध्रुव Mat& m)
{
    checkOperandsExist(m);
    Mat_व्यंजक en;
    e.op->subtract(e, Mat_व्यंजक(m), en);
    प्रत्यागम en;
}

Mat_व्यंजक operator - (ध्रुव Mat& m, ध्रुव Mat_व्यंजक& e)
{
    checkOperandsExist(m);
    Mat_व्यंजक en;
    e.op->subtract(Mat_व्यंजक(m), e, en);
    प्रत्यागम en;
}

Mat_व्यंजक operator - (ध्रुव Mat_व्यंजक& e, ध्रुव Scalar& s)
{
    Mat_व्यंजक en;
    e.op->add(e, -s, en);
    प्रत्यागम en;
}

Mat_व्यंजक operator - (ध्रुव Scalar& s, ध्रुव Mat_व्यंजक& e)
{
    Mat_व्यंजक en;
    e.op->subtract(s, e, en);
    प्रत्यागम en;
}

Mat_व्यंजक operator - (ध्रुव Mat_व्यंजक& e1, ध्रुव Mat_व्यंजक& e2)
{
    Mat_व्यंजक en;
    e1.op->subtract(e1, e2, en);
    प्रत्यागम en;
}

Mat_व्यंजक operator - (ध्रुव Mat& m)
{
    checkOperandsExist(m);
    Mat_व्यंजक e;
    MatOp_AddEx::make_व्यंजक(e, m, Mat(), -1, 0);
    प्रत्यागम e;
}

Mat_व्यंजक operator - (ध्रुव Mat_व्यंजक& e)
{
    Mat_व्यंजक en;
    e.op->subtract(Scalar(0), e, en);
    प्रत्यागम en;
}

Mat_व्यंजक operator * (ध्रुव Mat& a, ध्रुव Mat& b)
{
    checkOperandsExist(a, b);
    Mat_व्यंजक e;
    MatOp_GEMM::make_व्यंजक(e, 0, a, b);
    प्रत्यागम e;
}

Mat_व्यंजक operator * (ध्रुव Mat& a, द्विगुण s)
{
    checkOperandsExist(a);
    Mat_व्यंजक e;
    MatOp_AddEx::make_व्यंजक(e, a, Mat(), s, 0);
    प्रत्यागम e;
}

Mat_व्यंजक operator * (द्विगुण s, ध्रुव Mat& a)
{
    checkOperandsExist(a);
    Mat_व्यंजक e;
    MatOp_AddEx::make_व्यंजक(e, a, Mat(), s, 0);
    प्रत्यागम e;
}

Mat_व्यंजक operator * (ध्रुव Mat_व्यंजक& e, ध्रुव Mat& m)
{
    Mat_व्यंजक en;
    e.op->matmul(e, Mat_व्यंजक(m), en);
    प्रत्यागम en;
}

Mat_व्यंजक operator * (ध्रुव Mat& m, ध्रुव Mat_व्यंजक& e)
{
    checkOperandsExist(m);
    Mat_व्यंजक en;
    e.op->matmul(Mat_व्यंजक(m), e, en);
    प्रत्यागम en;
}

Mat_व्यंजक operator * (ध्रुव Mat_व्यंजक& e, द्विगुण s)
{
    Mat_व्यंजक en;
    e.op->multiply(e, s, en);
    प्रत्यागम en;
}

Mat_व्यंजक operator * (द्विगुण s, ध्रुव Mat_व्यंजक& e)
{
    Mat_व्यंजक en;
    e.op->multiply(e, s, en);
    प्रत्यागम en;
}

Mat_व्यंजक operator * (ध्रुव Mat_व्यंजक& e1, ध्रुव Mat_व्यंजक& e2)
{
    Mat_व्यंजक en;
    e1.op->matmul(e1, e2, en);
    प्रत्यागम en;
}

Mat_व्यंजक operator / (ध्रुव Mat& a, ध्रुव Mat& b)
{
    checkOperandsExist(a, b);
    Mat_व्यंजक e;
    MatOp_Bin::make_व्यंजक(e, '/', a, b);
    प्रत्यागम e;
}

Mat_व्यंजक operator / (ध्रुव Mat& a, द्विगुण s)
{
    checkOperandsExist(a);
    Mat_व्यंजक e;
    MatOp_AddEx::make_व्यंजक(e, a, Mat(), 1./s, 0);
    प्रत्यागम e;
}

Mat_व्यंजक operator / (द्विगुण s, ध्रुव Mat& a)
{
    checkOperandsExist(a);
    Mat_व्यंजक e;
    MatOp_Bin::make_व्यंजक(e, '/', a, Mat(), s);
    प्रत्यागम e;
}

Mat_व्यंजक operator / (ध्रुव Mat_व्यंजक& e, ध्रुव Mat& m)
{
    Mat_व्यंजक en;
    e.op->divide(e, Mat_व्यंजक(m), en);
    प्रत्यागम en;
}

Mat_व्यंजक operator / (ध्रुव Mat& m, ध्रुव Mat_व्यंजक& e)
{
    checkOperandsExist(m);
    Mat_व्यंजक en;
    e.op->divide(Mat_व्यंजक(m), e, en);
    प्रत्यागम en;
}

Mat_व्यंजक operator / (ध्रुव Mat_व्यंजक& e, द्विगुण s)
{
    Mat_व्यंजक en;
    e.op->multiply(e, 1./s, en);
    प्रत्यागम en;
}

Mat_व्यंजक operator / (द्विगुण s, ध्रुव Mat_व्यंजक& e)
{
    Mat_व्यंजक en;
    e.op->divide(s, e, en);
    प्रत्यागम en;
}

Mat_व्यंजक operator / (ध्रुव Mat_व्यंजक& e1, ध्रुव Mat_व्यंजक& e2)
{
    Mat_व्यंजक en;
    e1.op->divide(e1, e2, en);
    प्रत्यागम en;
}

Mat_व्यंजक operator < (ध्रुव Mat& a, ध्रुव Mat& b)
{
    checkOperandsExist(a, b);
    Mat_व्यंजक e;
    MatOp_Cmp::make_व्यंजक(e, cv::CMP_LT, a, b);
    प्रत्यागम e;
}

Mat_व्यंजक operator < (ध्रुव Mat& a, द्विगुण s)
{
    checkOperandsExist(a);
    Mat_व्यंजक e;
    MatOp_Cmp::make_व्यंजक(e, cv::CMP_LT, a, s);
    प्रत्यागम e;
}

Mat_व्यंजक operator < (द्विगुण s, ध्रुव Mat& a)
{
    checkOperandsExist(a);
    Mat_व्यंजक e;
    MatOp_Cmp::make_व्यंजक(e, cv::CMP_GT, a, s);
    प्रत्यागम e;
}

Mat_व्यंजक operator <= (ध्रुव Mat& a, ध्रुव Mat& b)
{
    checkOperandsExist(a, b);
    Mat_व्यंजक e;
    MatOp_Cmp::make_व्यंजक(e, cv::CMP_LE, a, b);
    प्रत्यागम e;
}

Mat_व्यंजक operator <= (ध्रुव Mat& a, द्विगुण s)
{
    checkOperandsExist(a);
    Mat_व्यंजक e;
    MatOp_Cmp::make_व्यंजक(e, cv::CMP_LE, a, s);
    प्रत्यागम e;
}

Mat_व्यंजक operator <= (द्विगुण s, ध्रुव Mat& a)
{
    checkOperandsExist(a);
    Mat_व्यंजक e;
    MatOp_Cmp::make_व्यंजक(e, cv::CMP_GE, a, s);
    प्रत्यागम e;
}

Mat_व्यंजक operator == (ध्रुव Mat& a, ध्रुव Mat& b)
{
    checkOperandsExist(a, b);
    Mat_व्यंजक e;
    MatOp_Cmp::make_व्यंजक(e, cv::CMP_EQ, a, b);
    प्रत्यागम e;
}

Mat_व्यंजक operator == (ध्रुव Mat& a, द्विगुण s)
{
    checkOperandsExist(a);
    Mat_व्यंजक e;
    MatOp_Cmp::make_व्यंजक(e, cv::CMP_EQ, a, s);
    प्रत्यागम e;
}

Mat_व्यंजक operator == (द्विगुण s, ध्रुव Mat& a)
{
    checkOperandsExist(a);
    Mat_व्यंजक e;
    MatOp_Cmp::make_व्यंजक(e, cv::CMP_EQ, a, s);
    प्रत्यागम e;
}

Mat_व्यंजक operator != (ध्रुव Mat& a, ध्रुव Mat& b)
{
    checkOperandsExist(a, b);
    Mat_व्यंजक e;
    MatOp_Cmp::make_व्यंजक(e, cv::CMP_NE, a, b);
    प्रत्यागम e;
}

Mat_व्यंजक operator != (ध्रुव Mat& a, द्विगुण s)
{
    checkOperandsExist(a);
    Mat_व्यंजक e;
    MatOp_Cmp::make_व्यंजक(e, cv::CMP_NE, a, s);
    प्रत्यागम e;
}

Mat_व्यंजक operator != (द्विगुण s, ध्रुव Mat& a)
{
    checkOperandsExist(a);
    Mat_व्यंजक e;
    MatOp_Cmp::make_व्यंजक(e, cv::CMP_NE, a, s);
    प्रत्यागम e;
}

Mat_व्यंजक operator >= (ध्रुव Mat& a, ध्रुव Mat& b)
{
    checkOperandsExist(a, b);
    Mat_व्यंजक e;
    MatOp_Cmp::make_व्यंजक(e, cv::CMP_GE, a, b);
    प्रत्यागम e;
}

Mat_व्यंजक operator >= (ध्रुव Mat& a, द्विगुण s)
{
    checkOperandsExist(a);
    Mat_व्यंजक e;
    MatOp_Cmp::make_व्यंजक(e, cv::CMP_GE, a, s);
    प्रत्यागम e;
}

Mat_व्यंजक operator >= (द्विगुण s, ध्रुव Mat& a)
{
    checkOperandsExist(a);
    Mat_व्यंजक e;
    MatOp_Cmp::make_व्यंजक(e, cv::CMP_LE, a, s);
    प्रत्यागम e;
}

Mat_व्यंजक operator > (ध्रुव Mat& a, ध्रुव Mat& b)
{
    checkOperandsExist(a, b);
    Mat_व्यंजक e;
    MatOp_Cmp::make_व्यंजक(e, cv::CMP_GT, a, b);
    प्रत्यागम e;
}

Mat_व्यंजक operator > (ध्रुव Mat& a, द्विगुण s)
{
    checkOperandsExist(a);
    Mat_व्यंजक e;
    MatOp_Cmp::make_व्यंजक(e, cv::CMP_GT, a, s);
    प्रत्यागम e;
}

Mat_व्यंजक operator > (द्विगुण s, ध्रुव Mat& a)
{
    checkOperandsExist(a);
    Mat_व्यंजक e;
    MatOp_Cmp::make_व्यंजक(e, cv::CMP_LT, a, s);
    प्रत्यागम e;
}

Mat_व्यंजक min(ध्रुव Mat& a, ध्रुव Mat& b)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a, b);
    Mat_व्यंजक e;
    MatOp_Bin::make_व्यंजक(e, 'm', a, b);
    प्रत्यागम e;
}

Mat_व्यंजक min(ध्रुव Mat& a, द्विगुण s)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    Mat_व्यंजक e;
    MatOp_Bin::make_व्यंजक(e, 'n', a, s);
    प्रत्यागम e;
}

Mat_व्यंजक min(द्विगुण s, ध्रुव Mat& a)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    Mat_व्यंजक e;
    MatOp_Bin::make_व्यंजक(e, 'n', a, s);
    प्रत्यागम e;
}

Mat_व्यंजक max(ध्रुव Mat& a, ध्रुव Mat& b)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a, b);
    Mat_व्यंजक e;
    MatOp_Bin::make_व्यंजक(e, 'M', a, b);
    प्रत्यागम e;
}

Mat_व्यंजक max(ध्रुव Mat& a, द्विगुण s)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    Mat_व्यंजक e;
    MatOp_Bin::make_व्यंजक(e, 'N', a, s);
    प्रत्यागम e;
}

Mat_व्यंजक max(द्विगुण s, ध्रुव Mat& a)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    Mat_व्यंजक e;
    MatOp_Bin::make_व्यंजक(e, 'N', a, s);
    प्रत्यागम e;
}

Mat_व्यंजक operator & (ध्रुव Mat& a, ध्रुव Mat& b)
{
    checkOperandsExist(a, b);
    Mat_व्यंजक e;
    MatOp_Bin::make_व्यंजक(e, '&', a, b);
    प्रत्यागम e;
}

Mat_व्यंजक operator & (ध्रुव Mat& a, ध्रुव Scalar& s)
{
    checkOperandsExist(a);
    Mat_व्यंजक e;
    MatOp_Bin::make_व्यंजक(e, '&', a, s);
    प्रत्यागम e;
}

Mat_व्यंजक operator & (ध्रुव Scalar& s, ध्रुव Mat& a)
{
    checkOperandsExist(a);
    Mat_व्यंजक e;
    MatOp_Bin::make_व्यंजक(e, '&', a, s);
    प्रत्यागम e;
}

Mat_व्यंजक operator | (ध्रुव Mat& a, ध्रुव Mat& b)
{
    checkOperandsExist(a, b);
    Mat_व्यंजक e;
    MatOp_Bin::make_व्यंजक(e, '|', a, b);
    प्रत्यागम e;
}

Mat_व्यंजक operator | (ध्रुव Mat& a, ध्रुव Scalar& s)
{
    checkOperandsExist(a);
    Mat_व्यंजक e;
    MatOp_Bin::make_व्यंजक(e, '|', a, s);
    प्रत्यागम e;
}

Mat_व्यंजक operator | (ध्रुव Scalar& s, ध्रुव Mat& a)
{
    checkOperandsExist(a);
    Mat_व्यंजक e;
    MatOp_Bin::make_व्यंजक(e, '|', a, s);
    प्रत्यागम e;
}

Mat_व्यंजक operator ^ (ध्रुव Mat& a, ध्रुव Mat& b)
{
    checkOperandsExist(a, b);
    Mat_व्यंजक e;
    MatOp_Bin::make_व्यंजक(e, '^', a, b);
    प्रत्यागम e;
}

Mat_व्यंजक operator ^ (ध्रुव Mat& a, ध्रुव Scalar& s)
{
    checkOperandsExist(a);
    Mat_व्यंजक e;
    MatOp_Bin::make_व्यंजक(e, '^', a, s);
    प्रत्यागम e;
}

Mat_व्यंजक operator ^ (ध्रुव Scalar& s, ध्रुव Mat& a)
{
    checkOperandsExist(a);
    Mat_व्यंजक e;
    MatOp_Bin::make_व्यंजक(e, '^', a, s);
    प्रत्यागम e;
}

Mat_व्यंजक operator ~(ध्रुव Mat& a)
{
    checkOperandsExist(a);
    Mat_व्यंजक e;
    MatOp_Bin::make_व्यंजक(e, '~', a, Scalar());
    प्रत्यागम e;
}

Mat_व्यंजक abs(ध्रुव Mat& a)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    Mat_व्यंजक e;
    MatOp_Bin::make_व्यंजक(e, 'a', a, Scalar());
    प्रत्यागम e;
}

Mat_व्यंजक abs(ध्रुव Mat_व्यंजक& e)
{
    CV_INSTRUMENT_REGION();

    Mat_व्यंजक en;
    e.op->abs(e, en);
    प्रत्यागम en;
}


Size Mat_व्यंजक::size() ध्रुव
{
    यदि( isT(*एतत्) || isInv(*एतत्) )
        प्रत्यागम Size(a.rows, a.cols);
    यदि( isSolve(*एतत्) )
        प्रत्यागम Size(b.cols, a.cols);
    यदि( isInitializer(*एतत्) )
        प्रत्यागम a.size();
    प्रत्यागम op ? op->size(*एतत्) : Size();
}


पूर्णाङ्क Mat_व्यंजक::type() ध्रुव
{
    CV_INSTRUMENT_REGION();

    यदि( isInitializer(*एतत्) )
        प्रत्यागम a.type();
    यदि( isCmp(*एतत्) )
        प्रत्यागम CV_MAKETYPE(CV_8U, a.channels());
    प्रत्यागम op ? op->type(*एतत्) : -1;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////

शून्य MatOp_Identity::assign(ध्रुव Mat_व्यंजक& e, Mat& m, पूर्णाङ्क _type) ध्रुव
{
    यदि( _type == -1 || _type == e.a.type() )
        m = e.a;
    अन्यथा
    {
        CV_Assert( CV_MAT_CN(_type) == e.a.channels() );
        e.a.convertTo(m, _type);
    }
}

अन्तःस्थ शून्य MatOp_Identity::make_व्यंजक(Mat_व्यंजक& res, ध्रुव Mat& m)
{
    res = Mat_व्यंजक(&g_MatOp_Identity, 0, m, Mat(), Mat(), 1, 0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

शून्य MatOp_AddEx::assign(ध्रुव Mat_व्यंजक& e, Mat& m, पूर्णाङ्क _type) ध्रुव
{
    Mat temp, &dst = _type == -1 || e.a.type() == _type ? m : temp;
    यदि( e.b.data )
    {
        यदि( e.s == Scalar() || !e.s.isReal() )
        {
            यदि( e.alpha == 1 )
            {
                यदि( e.beta == 1 )
                    cv::add(e.a, e.b, dst);
                अन्यथा यदि( e.beta == -1 )
                    cv::subtract(e.a, e.b, dst);
                अन्यथा
                    cv::scaleAdd(e.b, e.beta, e.a, dst);
            }
            अन्यथा यदि( e.beta == 1 )
            {
                यदि( e.alpha == -1 )
                    cv::subtract(e.b, e.a, dst);
                अन्यथा
                    cv::scaleAdd(e.a, e.alpha, e.b, dst);
            }
            अन्यथा
                cv::addWeighted(e.a, e.alpha, e.b, e.beta, 0, dst);

            यदि( !e.s.isReal() )
                cv::add(dst, e.s, dst);
        }
        अन्यथा
        {
            यदि (e.a.channels() > 1)
                CV_LOG_ONCE_WARNING(रिक्त, "OpenCV/MatExpr: processing of multi-channel arrays might be changed in the future: "
                                          "https://github.com/opencv/opencv/issues/16739");
            cv::addWeighted(e.a, e.alpha, e.b, e.beta, e.s[0], dst);
        }
    }
    अन्यथा यदि( e.s.isReal() && (dst.data != m.data || fabs(e.alpha) != 1))
    {
        यदि (e.a.channels() > 1 && e.s[0] != 0.0)
            CV_LOG_ONCE_WARNING(रिक्त, "OpenCV/MatExpr: processing of multi-channel arrays might be changed in the future: "
                                      "https://github.com/opencv/opencv/issues/16739");
        e.a.convertTo(m, _type, e.alpha, e.s[0]);
        प्रत्यागम;
    }
    अन्यथा यदि( e.alpha == 1 )
        cv::add(e.a, e.s, dst);
    अन्यथा यदि( e.alpha == -1 )
        cv::subtract(e.s, e.a, dst);
    अन्यथा
    {
        e.a.convertTo(dst, e.a.type(), e.alpha);
        cv::add(dst, e.s, dst);
    }

    यदि( dst.data != m.data )
        dst.convertTo(m, m.type());
}


शून्य MatOp_AddEx::add(ध्रुव Mat_व्यंजक& e, ध्रुव Scalar& s, Mat_व्यंजक& res) ध्रुव
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.s += s;
}


शून्य MatOp_AddEx::subtract(ध्रुव Scalar& s, ध्रुव Mat_व्यंजक& e, Mat_व्यंजक& res) ध्रुव
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha = -res.alpha;
    res.beta = -res.beta;
    res.s = s - res.s;
}

शून्य MatOp_AddEx::multiply(ध्रुव Mat_व्यंजक& e, द्विगुण s, Mat_व्यंजक& res) ध्रुव
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha *= s;
    res.beta *= s;
    res.s *= s;
}

शून्य MatOp_AddEx::divide(द्विगुण s, ध्रुव Mat_व्यंजक& e, Mat_व्यंजक& res) ध्रुव
{
    CV_INSTRUMENT_REGION();

    यदि( isScaled(e) )
        MatOp_Bin::make_व्यंजक(res, '/', e.a, Mat(), s/e.alpha);
    अन्यथा
        MatOp::divide(s, e, res);
}


शून्य MatOp_AddEx::transpose(ध्रुव Mat_व्यंजक& e, Mat_व्यंजक& res) ध्रुव
{
    CV_INSTRUMENT_REGION();

    यदि( isScaled(e) )
        MatOp_T::make_व्यंजक(res, e.a, e.alpha);
    अन्यथा
        MatOp::transpose(e, res);
}

शून्य MatOp_AddEx::abs(ध्रुव Mat_व्यंजक& e, Mat_व्यंजक& res) ध्रुव
{
    CV_INSTRUMENT_REGION();

    यदि( (!e.b.data || e.beta == 0) && fabs(e.alpha) == 1 )
        MatOp_Bin::make_व्यंजक(res, 'a', e.a, -e.s*e.alpha);
    अन्यथा यदि( e.b.data && e.alpha + e.beta == 0 && e.alpha*e.beta == -1 )
        MatOp_Bin::make_व्यंजक(res, 'a', e.a, e.b);
    अन्यथा
        MatOp::abs(e, res);
}

अन्तःस्थ शून्य MatOp_AddEx::make_व्यंजक(Mat_व्यंजक& res, ध्रुव Mat& a, ध्रुव Mat& b, द्विगुण alpha, द्विगुण beta, ध्रुव Scalar& s)
{
    res = Mat_व्यंजक(&g_MatOp_AddEx, 0, a, b, Mat(), alpha, beta, s);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

शून्य MatOp_Bin::assign(ध्रुव Mat_व्यंजक& e, Mat& m, पूर्णाङ्क _type) ध्रुव
{
    Mat temp, &dst = _type == -1 || e.a.type() == _type ? m : temp;

    यदि( e.flags == '*' )
        cv::multiply(e.a, e.b, dst, e.alpha);
    अन्यथा यदि( e.flags == '/' && e.b.data )
        cv::divide(e.a, e.b, dst, e.alpha);
    अन्यथा यदि( e.flags == '/' && !e.b.data )
        cv::divide(e.alpha, e.a, dst );
    अन्यथा यदि( e.flags == '&' && e.b.data )
        bitwise_and(e.a, e.b, dst);
    अन्यथा यदि( e.flags == '&' && !e.b.data )
        bitwise_and(e.a, e.s, dst);
    अन्यथा यदि( e.flags == '|' && e.b.data )
        bitwise_or(e.a, e.b, dst);
    अन्यथा यदि( e.flags == '|' && !e.b.data )
        bitwise_or(e.a, e.s, dst);
    अन्यथा यदि( e.flags == '^' && e.b.data )
        bitwise_xor(e.a, e.b, dst);
    अन्यथा यदि( e.flags == '^' && !e.b.data )
        bitwise_xor(e.a, e.s, dst);
    अन्यथा यदि( e.flags == '~' && !e.b.data )
        bitwise_not(e.a, dst);
    अन्यथा यदि( e.flags == 'm' )
        cv::min(e.a, e.b, dst);
    अन्यथा यदि( e.flags == 'n' )
        cv::min(e.a, e.s[0], dst);
    अन्यथा यदि( e.flags == 'M' )
        cv::max(e.a, e.b, dst);
    अन्यथा यदि( e.flags == 'N' )
        cv::max(e.a, e.s[0], dst);
    अन्यथा यदि( e.flags == 'a' && e.b.data )
        cv::absdiff(e.a, e.b, dst);
    अन्यथा यदि( e.flags == 'a' && !e.b.data )
        cv::absdiff(e.a, e.s, dst);
    अन्यथा
        CV_Error(cv::Error::StsError, "Unknown operation");

    यदि( dst.data != m.data )
        dst.convertTo(m, _type);
}

शून्य MatOp_Bin::multiply(ध्रुव Mat_व्यंजक& e, द्विगुण s, Mat_व्यंजक& res) ध्रुव
{
    CV_INSTRUMENT_REGION();

    यदि( e.flags == '*' || e.flags == '/' )
    {
        res = e;
        res.alpha *= s;
    }
    अन्यथा
        MatOp::multiply(e, s, res);
}

शून्य MatOp_Bin::divide(द्विगुण s, ध्रुव Mat_व्यंजक& e, Mat_व्यंजक& res) ध्रुव
{
    CV_INSTRUMENT_REGION();

    यदि( e.flags == '/' && (!e.b.data || e.beta == 0) )
        MatOp_AddEx::make_व्यंजक(res, e.a, Mat(), s/e.alpha, 0);
    अन्यथा
        MatOp::divide(s, e, res);
}

अन्तःस्थ शून्य MatOp_Bin::make_व्यंजक(Mat_व्यंजक& res, वर्ण op, ध्रुव Mat& a, ध्रुव Mat& b, द्विगुण scale)
{
    res = Mat_व्यंजक(&g_MatOp_Bin, op, a, b, Mat(), scale, b.data ? 1 : 0);
}

अन्तःस्थ शून्य MatOp_Bin::make_व्यंजक(Mat_व्यंजक& res, वर्ण op, ध्रुव Mat& a, ध्रुव Scalar& s)
{
    res = Mat_व्यंजक(&g_MatOp_Bin, op, a, Mat(), Mat(), 1, 0, s);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

शून्य MatOp_Cmp::assign(ध्रुव Mat_व्यंजक& e, Mat& m, पूर्णाङ्क _type) ध्रुव
{
    Mat temp, &dst = _type == -1 || _type == CV_8U ? m : temp;

    यदि( e.b.data )
        cv::compare(e.a, e.b, dst, e.flags);
    अन्यथा
        cv::compare(e.a, e.alpha, dst, e.flags);

    यदि( dst.data != m.data )
        dst.convertTo(m, _type);
}

अन्तःस्थ शून्य MatOp_Cmp::make_व्यंजक(Mat_व्यंजक& res, पूर्णाङ्क cmpop, ध्रुव Mat& a, ध्रुव Mat& b)
{
    res = Mat_व्यंजक(&g_MatOp_Cmp, cmpop, a, b, Mat(), 1, 1);
}

अन्तःस्थ शून्य MatOp_Cmp::make_व्यंजक(Mat_व्यंजक& res, पूर्णाङ्क cmpop, ध्रुव Mat& a, द्विगुण alpha)
{
    res = Mat_व्यंजक(&g_MatOp_Cmp, cmpop, a, Mat(), Mat(), alpha, 1);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

शून्य MatOp_T::assign(ध्रुव Mat_व्यंजक& e, Mat& m, पूर्णाङ्क _type) ध्रुव
{
    Mat temp, &dst = _type == -1 || _type == e.a.type() ? m : temp;

    cv::transpose(e.a, dst);

    यदि( dst.data != m.data || e.alpha != 1 )
        dst.convertTo(m, _type, e.alpha);
}

शून्य MatOp_T::multiply(ध्रुव Mat_व्यंजक& e, द्विगुण s, Mat_व्यंजक& res) ध्रुव
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha *= s;
}

शून्य MatOp_T::transpose(ध्रुव Mat_व्यंजक& e, Mat_व्यंजक& res) ध्रुव
{
    CV_INSTRUMENT_REGION();

    यदि( e.alpha == 1 )
        MatOp_Identity::make_व्यंजक(res, e.a);
    अन्यथा
        MatOp_AddEx::make_व्यंजक(res, e.a, Mat(), e.alpha, 0);
}

अन्तःस्थ शून्य MatOp_T::make_व्यंजक(Mat_व्यंजक& res, ध्रुव Mat& a, द्विगुण alpha)
{
    res = Mat_व्यंजक(&g_MatOp_T, 0, a, Mat(), Mat(), alpha, 0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

शून्य MatOp_GEMM::assign(ध्रुव Mat_व्यंजक& e, Mat& m, पूर्णाङ्क _type) ध्रुव
{
    Mat temp, &dst = _type == -1 || _type == e.a.type() ? m : temp;

    cv::gemm(e.a, e.b, e.alpha, e.c, e.beta, dst, e.flags);
    यदि( dst.data != m.data )
        dst.convertTo(m, _type);
}

शून्य MatOp_GEMM::add(ध्रुव Mat_व्यंजक& e1, ध्रुव Mat_व्यंजक& e2, Mat_व्यंजक& res) ध्रुव
{
    CV_INSTRUMENT_REGION();

    तार्किक i1 = isIdentity(e1), i2 = isIdentity(e2);
    द्विगुण alpha1 = i1 ? 1 : e1.alpha, alpha2 = i2 ? 1 : e2.alpha;

    यदि( isMatProd(e1) && (i2 || isScaled(e2) || isT(e2)) )
        MatOp_GEMM::make_व्यंजक(res, (e1.flags & ~cv::GEMM_3_T)|(isT(e2) ? cv::GEMM_3_T : 0),
                             e1.a, e1.b, alpha1, e2.a, alpha2);
    अन्यथा यदि( isMatProd(e2) && (i1 || isScaled(e1) || isT(e1)) )
        MatOp_GEMM::make_व्यंजक(res, (e2.flags & ~cv::GEMM_3_T)|(isT(e1) ? cv::GEMM_3_T : 0),
                             e2.a, e2.b, alpha2, e1.a, alpha1);
    अन्यथा यदि( एतत् == e2.op )
        MatOp::add(e1, e2, res);
    अन्यथा
        e2.op->add(e1, e2, res);
}

शून्य MatOp_GEMM::subtract(ध्रुव Mat_व्यंजक& e1, ध्रुव Mat_व्यंजक& e2, Mat_व्यंजक& res) ध्रुव
{
    CV_INSTRUMENT_REGION();

    तार्किक i1 = isIdentity(e1), i2 = isIdentity(e2);
    द्विगुण alpha1 = i1 ? 1 : e1.alpha, alpha2 = i2 ? 1 : e2.alpha;

    यदि( isMatProd(e1) && (i2 || isScaled(e2) || isT(e2)) )
        MatOp_GEMM::make_व्यंजक(res, (e1.flags & ~cv::GEMM_3_T)|(isT(e2) ? cv::GEMM_3_T : 0),
                             e1.a, e1.b, alpha1, e2.a, -alpha2);
    अन्यथा यदि( isMatProd(e2) && (i1 || isScaled(e1) || isT(e1)) )
        MatOp_GEMM::make_व्यंजक(res, (e2.flags & ~cv::GEMM_3_T)|(isT(e1) ? cv::GEMM_3_T : 0),
                            e2.a, e2.b, -alpha2, e1.a, alpha1);
    अन्यथा यदि( एतत् == e2.op )
        MatOp::subtract(e1, e2, res);
    अन्यथा
        e2.op->subtract(e1, e2, res);
}

शून्य MatOp_GEMM::multiply(ध्रुव Mat_व्यंजक& e, द्विगुण s, Mat_व्यंजक& res) ध्रुव
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha *= s;
    res.beta *= s;
}

शून्य MatOp_GEMM::transpose(ध्रुव Mat_व्यंजक& e, Mat_व्यंजक& res) ध्रुव
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.flags = (!(e.flags & cv::GEMM_1_T) ? cv::GEMM_2_T : 0) |
                (!(e.flags & cv::GEMM_2_T) ? cv::GEMM_1_T : 0) |
                (!(e.flags & cv::GEMM_3_T) ? cv::GEMM_3_T : 0);
    swap(res.a, res.b);
}

अन्तःस्थ शून्य MatOp_GEMM::make_व्यंजक(Mat_व्यंजक& res, पूर्णाङ्क flags, ध्रुव Mat& a, ध्रुव Mat& b,
                                 द्विगुण alpha, ध्रुव Mat& c, द्विगुण beta)
{
    res = Mat_व्यंजक(&g_MatOp_GEMM, flags, a, b, c, alpha, beta);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

शून्य MatOp_Invert::assign(ध्रुव Mat_व्यंजक& e, Mat& m, पूर्णाङ्क _type) ध्रुव
{
    Mat temp, &dst = _type == -1 || _type == e.a.type() ? m : temp;

    cv::invert(e.a, dst, e.flags);
    यदि( dst.data != m.data )
        dst.convertTo(m, _type);
}

शून्य MatOp_Invert::matmul(ध्रुव Mat_व्यंजक& e1, ध्रुव Mat_व्यंजक& e2, Mat_व्यंजक& res) ध्रुव
{
    यदि( isInv(e1) && isIdentity(e2) )
        MatOp_Solve::make_व्यंजक(res, e1.flags, e1.a, e2.a);
    अन्यथा यदि( एतत् == e2.op )
        MatOp::matmul(e1, e2, res);
    अन्यथा
        e2.op->matmul(e1, e2, res);
}

अन्तःस्थ शून्य MatOp_Invert::make_व्यंजक(Mat_व्यंजक& res, पूर्णाङ्क method, ध्रुव Mat& m)
{
    res = Mat_व्यंजक(&g_MatOp_Invert, method, m, Mat(), Mat(), 1, 0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

शून्य MatOp_Solve::assign(ध्रुव Mat_व्यंजक& e, Mat& m, पूर्णाङ्क _type) ध्रुव
{
    Mat temp, &dst = _type == -1 || _type == e.a.type() ? m : temp;

    cv::solve(e.a, e.b, dst, e.flags);
    यदि( dst.data != m.data )
        dst.convertTo(m, _type);
}

अन्तःस्थ शून्य MatOp_Solve::make_व्यंजक(Mat_व्यंजक& res, पूर्णाङ्क method, ध्रुव Mat& a, ध्रुव Mat& b)
{
    res = Mat_व्यंजक(&g_MatOp_Solve, method, a, b, Mat(), 1, 1);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

शून्य MatOp_Initializer::assign(ध्रुव Mat_व्यंजक& e, Mat& m, पूर्णाङ्क _type) ध्रुव
{
    यदि( _type == -1 )
        _type = e.a.type();

    m.create(e.a.size, _type);

    यदि( e.flags == 'I' && e.a.dims <= 2 )
        setIdentity(m, Scalar(e.alpha));
    अन्यथा यदि( e.flags == '0' )
        m = Scalar();
    अन्यथा यदि( e.flags == '1' )
        m = Scalar(e.alpha);
    अन्यथा
        CV_Error(cv::Error::StsError, "Invalid matrix initializer type");
}

शून्य MatOp_Initializer::multiply(ध्रुव Mat_व्यंजक& e, द्विगुण s, Mat_व्यंजक& res) ध्रुव
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha *= s;
}

अन्तःस्थ शून्य MatOp_Initializer::make_व्यंजक(Mat_व्यंजक& res, पूर्णाङ्क method, Size sz, पूर्णाङ्क type, द्विगुण alpha)
{
    res = Mat_व्यंजक(getGlobalMatOpInitializer(), method, Mat(sz, type, (शून्य*)(size_t)0xEEEEEEEE), Mat(), Mat(), alpha, 0);
}

अन्तःस्थ शून्य MatOp_Initializer::make_व्यंजक(Mat_व्यंजक& res, पूर्णाङ्क method, पूर्णाङ्क ndims, ध्रुव पूर्णाङ्क* sizes, पूर्णाङ्क type, द्विगुण alpha)
{
    res = Mat_व्यंजक(getGlobalMatOpInitializer(), method, Mat(ndims, sizes, type, (शून्य*)(size_t)0xEEEEEEEE), Mat(), Mat(), alpha, 0);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

Mat_व्यंजक Mat::t() ध्रुव
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(*एतत्);
    Mat_व्यंजक e;
    MatOp_T::make_व्यंजक(e, *एतत्);
    प्रत्यागम e;
}

Mat_व्यंजक Mat::inv(पूर्णाङ्क method) ध्रुव
{
    CV_INSTRUMENT_REGION();

    Mat_व्यंजक e;
    MatOp_Invert::make_व्यंजक(e, method, *एतत्);
    प्रत्यागम e;
}


Mat_व्यंजक Mat::mul(निवेश_Array m, द्विगुण scale) ध्रुव
{
    CV_INSTRUMENT_REGION();

    Mat b = m.getMat();
    // Unless the argument is a refcounted Mat/UMat, the header returned by getMat() may be
    // a non-owning view of caller memory (e.g. a scalar bound to _InputArray(const double&),
    // a Matx or a Vec) that the returned MatExpr can outlive, so snapshot it.
    // See https://github.com/opencv/opencv/issues/23577
    यदि( !m.isMat() && !m.isUMat() )
        b = b.clone();

    Mat_व्यंजक e;
    MatOp_Bin::make_व्यंजक(e, '*', *एतत्, b, scale);
    प्रत्यागम e;
}

Mat_व्यंजक Mat::zeros(पूर्णाङ्क rows, पूर्णाङ्क cols, पूर्णाङ्क type)
{
    CV_INSTRUMENT_REGION();

    Mat_व्यंजक e;
    MatOp_Initializer::make_व्यंजक(e, '0', Size(cols, rows), type);
    प्रत्यागम e;
}

Mat_व्यंजक Mat::zeros(Size size, पूर्णाङ्क type)
{
    CV_INSTRUMENT_REGION();

    Mat_व्यंजक e;
    MatOp_Initializer::make_व्यंजक(e, '0', size, type);
    प्रत्यागम e;
}

Mat_व्यंजक Mat::zeros(पूर्णाङ्क ndims, ध्रुव पूर्णाङ्क* sizes, पूर्णाङ्क type)
{
    CV_INSTRUMENT_REGION();

    Mat_व्यंजक e;
    MatOp_Initializer::make_व्यंजक(e, '0', ndims, sizes, type);
    प्रत्यागम e;
}

Mat_व्यंजक Mat::zeros(ध्रुव MatShape& shape, पूर्णाङ्क type)
{
    CV_INSTRUMENT_REGION();

    Mat_व्यंजक e;
    MatOp_Initializer::make_व्यंजक(e, '0', shape.dims, shape.p, type);
    प्रत्यागम e;
}

Mat_व्यंजक Mat::ones(पूर्णाङ्क rows, पूर्णाङ्क cols, पूर्णाङ्क type)
{
    CV_INSTRUMENT_REGION();

    Mat_व्यंजक e;
    MatOp_Initializer::make_व्यंजक(e, '1', Size(cols, rows), type);
    प्रत्यागम e;
}

Mat_व्यंजक Mat::ones(Size size, पूर्णाङ्क type)
{
    CV_INSTRUMENT_REGION();

    Mat_व्यंजक e;
    MatOp_Initializer::make_व्यंजक(e, '1', size, type);
    प्रत्यागम e;
}

Mat_व्यंजक Mat::ones(पूर्णाङ्क ndims, ध्रुव पूर्णाङ्क* sizes, पूर्णाङ्क type)
{
    CV_INSTRUMENT_REGION();

    Mat_व्यंजक e;
    MatOp_Initializer::make_व्यंजक(e, '1', ndims, sizes, type);
    प्रत्यागम e;
}

Mat_व्यंजक Mat::ones(ध्रुव MatShape& shape, पूर्णाङ्क type)
{
    CV_INSTRUMENT_REGION();

    Mat_व्यंजक e;
    MatOp_Initializer::make_व्यंजक(e, '1', shape.dims, shape.p, type);
    प्रत्यागम e;
}

Mat_व्यंजक Mat::eye(पूर्णाङ्क rows, पूर्णाङ्क cols, पूर्णाङ्क type)
{
    CV_INSTRUMENT_REGION();

    Mat_व्यंजक e;
    MatOp_Initializer::make_व्यंजक(e, 'I', Size(cols, rows), type);
    प्रत्यागम e;
}

Mat_व्यंजक Mat::eye(Size size, पूर्णाङ्क type)
{
    CV_INSTRUMENT_REGION();

    Mat_व्यंजक e;
    MatOp_Initializer::make_व्यंजक(e, 'I', size, type);
    प्रत्यागम e;
}

शून्य Mat_व्यंजक::swap(Mat_व्यंजक& other)
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

निवेश_Array_2::निवेश_Array_2(ध्रुव Mat_व्यंजक& व्यंजक)
{
    यदि (!isIdentity(व्यंजक))
    {
        Mat result = व्यंजक;  // TODO improve through refcount == 1 of expr.a (inplace operation is possible - except gemm?)
        Mat_व्यंजक result_व्यंजक(result);
        swap(const_cast<Mat_व्यंजक&>(व्यंजक), result_व्यंजक);
    }
    CV_Assert(isIdentity(व्यंजक));
    init(FIXED_TYPE + FIXED_SIZE + MAT + ACCESS_READ, &व्यंजक.a);
}

} // cv::
