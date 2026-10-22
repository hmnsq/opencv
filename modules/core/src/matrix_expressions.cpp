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


वर्ग Mat_Op_तटस्थः CV_FINAL : सार्वजनिक MatOp
{
सार्वजनिक:
    Mat_Op_तटस्थः() {}
    आभास ~Mat_Op_तटस्थः() {}

    तार्किक elementWise(ध्रुव Mat_व्यञ्जकम्& /*व्यञ्जकम्*/) ध्रुव CV_OVERRIDE { प्रत्यागम सत्य; }
    शून्य assign(ध्रुव Mat_व्यञ्जकम्& व्यञ्जकम्, Mat& m, पूर्णाङ्क type=-1) ध्रुव CV_OVERRIDE;

    स्थिर शून्य make_व्यञ्जकम्(Mat_व्यञ्जकम्& res, ध्रुव Mat& m);
};

स्थिर Mat_Op_तटस्थः g_Mat_Op_तटस्थः;

वर्ग Mat_Op_सङ्कलय_Ex CV_FINAL : सार्वजनिक MatOp
{
सार्वजनिक:
    Mat_Op_सङ्कलय_Ex() {}
    आभास ~Mat_Op_सङ्कलय_Ex() {}

    तार्किक elementWise(ध्रुव Mat_व्यञ्जकम्& /*व्यञ्जकम्*/) ध्रुव CV_OVERRIDE { प्रत्यागम सत्य; }
    शून्य assign(ध्रुव Mat_व्यञ्जकम्& व्यञ्जकम्, Mat& m, पूर्णाङ्क type=-1) ध्रुव CV_OVERRIDE;

    शून्य सङ्कलय(ध्रुव Mat_व्यञ्जकम्& e1, ध्रुव Scalar& s, Mat_व्यञ्जकम्& res) ध्रुव CV_OVERRIDE;
    शून्य व्यवकलय(ध्रुव Scalar& s, ध्रुव Mat_व्यञ्जकम्& व्यञ्जकम्, Mat_व्यञ्जकम्& res) ध्रुव CV_OVERRIDE;
    शून्य multiply(ध्रुव Mat_व्यञ्जकम्& e1, द्विगुण s, Mat_व्यञ्जकम्& res) ध्रुव CV_OVERRIDE;
    शून्य divide(द्विगुण s, ध्रुव Mat_व्यञ्जकम्& e, Mat_व्यञ्जकम्& res) ध्रुव CV_OVERRIDE;

    शून्य transpose(ध्रुव Mat_व्यञ्जकम्& e1, Mat_व्यञ्जकम्& res) ध्रुव CV_OVERRIDE;
    शून्य निरपेक्षम्(ध्रुव Mat_व्यञ्जकम्& व्यञ्जकम्, Mat_व्यञ्जकम्& res) ध्रुव CV_OVERRIDE;

    स्थिर शून्य make_व्यञ्जकम्(Mat_व्यञ्जकम्& res, ध्रुव Mat& a, ध्रुव Mat& b, द्विगुण alpha, द्विगुण beta, ध्रुव Scalar& s=Scalar());
};

स्थिर Mat_Op_सङ्कलय_Ex g_Mat_Op_सङ्कलय_Ex;

वर्ग MatOp_Bin CV_FINAL : सार्वजनिक MatOp
{
सार्वजनिक:
    MatOp_Bin() {}
    आभास ~MatOp_Bin() {}

    तार्किक elementWise(ध्रुव Mat_व्यञ्जकम्& /*व्यञ्जकम्*/) ध्रुव CV_OVERRIDE { प्रत्यागम सत्य; }
    शून्य assign(ध्रुव Mat_व्यञ्जकम्& व्यञ्जकम्, Mat& m, पूर्णाङ्क type=-1) ध्रुव CV_OVERRIDE;

    शून्य multiply(ध्रुव Mat_व्यञ्जकम्& e1, द्विगुण s, Mat_व्यञ्जकम्& res) ध्रुव CV_OVERRIDE;
    शून्य divide(द्विगुण s, ध्रुव Mat_व्यञ्जकम्& e, Mat_व्यञ्जकम्& res) ध्रुव CV_OVERRIDE;

    स्थिर शून्य make_व्यञ्जकम्(Mat_व्यञ्जकम्& res, वर्ण op, ध्रुव Mat& a, ध्रुव Mat& b, द्विगुण scale=1);
    स्थिर शून्य make_व्यञ्जकम्(Mat_व्यञ्जकम्& res, वर्ण op, ध्रुव Mat& a, ध्रुव Scalar& s);
};

स्थिर MatOp_Bin g_MatOp_Bin;

वर्ग MatOp_Cmp CV_FINAL : सार्वजनिक MatOp
{
सार्वजनिक:
    MatOp_Cmp() {}
    आभास ~MatOp_Cmp() {}

    तार्किक elementWise(ध्रुव Mat_व्यञ्जकम्& /*व्यञ्जकम्*/) ध्रुव CV_OVERRIDE { प्रत्यागम सत्य; }
    शून्य assign(ध्रुव Mat_व्यञ्जकम्& व्यञ्जकम्, Mat& m, पूर्णाङ्क type=-1) ध्रुव CV_OVERRIDE;

    स्थिर शून्य make_व्यञ्जकम्(Mat_व्यञ्जकम्& res, पूर्णाङ्क cmpop, ध्रुव Mat& a, ध्रुव Mat& b);
    स्थिर शून्य make_व्यञ्जकम्(Mat_व्यञ्जकम्& res, पूर्णाङ्क cmpop, ध्रुव Mat& a, द्विगुण alpha);
};

स्थिर MatOp_Cmp g_MatOp_Cmp;

वर्ग MatOp_GEMM CV_FINAL : सार्वजनिक MatOp
{
सार्वजनिक:
    MatOp_GEMM() {}
    आभास ~MatOp_GEMM() {}

    तार्किक elementWise(ध्रुव Mat_व्यञ्जकम्& /*व्यञ्जकम्*/) ध्रुव CV_OVERRIDE { प्रत्यागम असत्य; }
    शून्य assign(ध्रुव Mat_व्यञ्जकम्& व्यञ्जकम्, Mat& m, पूर्णाङ्क type=-1) ध्रुव CV_OVERRIDE;

    शून्य सङ्कलय(ध्रुव Mat_व्यञ्जकम्& e1, ध्रुव Mat_व्यञ्जकम्& e2, Mat_व्यञ्जकम्& res) ध्रुव CV_OVERRIDE;
    शून्य व्यवकलय(ध्रुव Mat_व्यञ्जकम्& e1, ध्रुव Mat_व्यञ्जकम्& e2, Mat_व्यञ्जकम्& res) ध्रुव CV_OVERRIDE;
    शून्य multiply(ध्रुव Mat_व्यञ्जकम्& e, द्विगुण s, Mat_व्यञ्जकम्& res) ध्रुव CV_OVERRIDE;

    शून्य transpose(ध्रुव Mat_व्यञ्जकम्& व्यञ्जकम्, Mat_व्यञ्जकम्& res) ध्रुव CV_OVERRIDE;

    आकार size(ध्रुव Mat_व्यञ्जकम्& व्यञ्जकम्) ध्रुव CV_OVERRIDE
    {
        प्रत्यागम आकार(
            (व्यञ्जकम्.flags & GEMM_2_T) ? व्यञ्जकम्.b.पङ्क्तिकाः : व्यञ्जकम्.b.स्तम्भसङ्ख्या,
            (व्यञ्जकम्.flags & GEMM_1_T) ? व्यञ्जकम्.a.स्तम्भसङ्ख्या : व्यञ्जकम्.a.पङ्क्तिकाः
        );
    }

    स्थिर शून्य make_व्यञ्जकम्(Mat_व्यञ्जकम्& res, पूर्णाङ्क flags, ध्रुव Mat& a, ध्रुव Mat& b,
                         द्विगुण alpha=1, ध्रुव Mat& c=Mat(), द्विगुण beta=1);

};

स्थिर MatOp_GEMM g_MatOp_GEMM;

वर्ग MatOp_Invert CV_FINAL : सार्वजनिक MatOp
{
सार्वजनिक:
    MatOp_Invert() {}
    आभास ~MatOp_Invert() {}

    तार्किक elementWise(ध्रुव Mat_व्यञ्जकम्& /*व्यञ्जकम्*/) ध्रुव CV_OVERRIDE { प्रत्यागम असत्य; }
    शून्य assign(ध्रुव Mat_व्यञ्जकम्& व्यञ्जकम्, Mat& m, पूर्णाङ्क type=-1) ध्रुव CV_OVERRIDE;

    शून्य matmul(ध्रुव Mat_व्यञ्जकम्& expr1, ध्रुव Mat_व्यञ्जकम्& expr2, Mat_व्यञ्जकम्& res) ध्रुव CV_OVERRIDE;

    स्थिर शून्य make_व्यञ्जकम्(Mat_व्यञ्जकम्& res, पूर्णाङ्क विधानम्, ध्रुव Mat& m);
};

स्थिर MatOp_Invert g_MatOp_Invert;

वर्ग MatOp_T CV_FINAL : सार्वजनिक MatOp
{
सार्वजनिक:
    MatOp_T() {}
    आभास ~MatOp_T() {}

    तार्किक elementWise(ध्रुव Mat_व्यञ्जकम्& /*व्यञ्जकम्*/) ध्रुव CV_OVERRIDE { प्रत्यागम असत्य; }
    शून्य assign(ध्रुव Mat_व्यञ्जकम्& व्यञ्जकम्, Mat& m, पूर्णाङ्क type=-1) ध्रुव CV_OVERRIDE;

    शून्य multiply(ध्रुव Mat_व्यञ्जकम्& e1, द्विगुण s, Mat_व्यञ्जकम्& res) ध्रुव CV_OVERRIDE;
    शून्य transpose(ध्रुव Mat_व्यञ्जकम्& व्यञ्जकम्, Mat_व्यञ्जकम्& res) ध्रुव CV_OVERRIDE;

    स्थिर शून्य make_व्यञ्जकम्(Mat_व्यञ्जकम्& res, ध्रुव Mat& a, द्विगुण alpha=1);
};

स्थिर MatOp_T g_MatOp_T;

वर्ग MatOp_Solve CV_FINAL : सार्वजनिक MatOp
{
सार्वजनिक:
    MatOp_Solve() {}
    आभास ~MatOp_Solve() {}

    तार्किक elementWise(ध्रुव Mat_व्यञ्जकम्& /*व्यञ्जकम्*/) ध्रुव CV_OVERRIDE { प्रत्यागम असत्य; }
    शून्य assign(ध्रुव Mat_व्यञ्जकम्& व्यञ्जकम्, Mat& m, पूर्णाङ्क type=-1) ध्रुव CV_OVERRIDE;

    स्थिर शून्य make_व्यञ्जकम्(Mat_व्यञ्जकम्& res, पूर्णाङ्क विधानम्, ध्रुव Mat& a, ध्रुव Mat& b);
};

स्थिर MatOp_Solve g_MatOp_Solve;

वर्ग MatOp_Initializer CV_FINAL : सार्वजनिक MatOp
{
सार्वजनिक:
    MatOp_Initializer() {}
    आभास ~MatOp_Initializer() {}

    तार्किक elementWise(ध्रुव Mat_व्यञ्जकम्& /*व्यञ्जकम्*/) ध्रुव CV_OVERRIDE { प्रत्यागम असत्य; }
    शून्य assign(ध्रुव Mat_व्यञ्जकम्& व्यञ्जकम्, Mat& m, पूर्णाङ्क type=-1) ध्रुव CV_OVERRIDE;

    शून्य multiply(ध्रुव Mat_व्यञ्जकम्& e, द्विगुण s, Mat_व्यञ्जकम्& res) ध्रुव CV_OVERRIDE;

    स्थिर शून्य make_व्यञ्जकम्(Mat_व्यञ्जकम्& res, पूर्णाङ्क विधानम्, आकार sz, पूर्णाङ्क type, द्विगुण alpha=1);
    स्थिर शून्य make_व्यञ्जकम्(Mat_व्यञ्जकम्& res, पूर्णाङ्क विधानम्, पूर्णाङ्क ndims, ध्रुव पूर्णाङ्क* sizes, पूर्णाङ्क type, द्विगुण alpha=1);
};

स्थिर MatOp_Initializer* getGlobalMatOpInitializer()
{
    CV_SINGLETON_LAZY_INIT(MatOp_Initializer, नूतन MatOp_Initializer())
}

स्थिर अन्तःस्थ तार्किक is_तटस्थः(ध्रुव Mat_व्यञ्जकम्& e) { प्रत्यागम e.op == &g_Mat_Op_तटस्थः; }
स्थिर अन्तःस्थ तार्किक is_सङ्कलय_Ex(ध्रुव Mat_व्यञ्जकम्& e) { प्रत्यागम e.op == &g_Mat_Op_सङ्कलय_Ex; }
स्थिर अन्तःस्थ तार्किक isScaled(ध्रुव Mat_व्यञ्जकम्& e) { प्रत्यागम is_सङ्कलय_Ex(e) && (!e.b.दत्त || e.beta == 0) && e.s == Scalar(); }
स्थिर अन्तःस्थ तार्किक isBin(ध्रुव Mat_व्यञ्जकम्& e, वर्ण c) { प्रत्यागम e.op == &g_MatOp_Bin && e.flags == c; }
स्थिर अन्तःस्थ तार्किक isCmp(ध्रुव Mat_व्यञ्जकम्& e) { प्रत्यागम e.op == &g_MatOp_Cmp; }
स्थिर अन्तःस्थ तार्किक isReciprocal(ध्रुव Mat_व्यञ्जकम्& e) { प्रत्यागम isBin(e,'/') && (!e.b.दत्त || e.beta == 0); }
स्थिर अन्तःस्थ तार्किक isT(ध्रुव Mat_व्यञ्जकम्& e) { प्रत्यागम e.op == &g_MatOp_T; }
स्थिर अन्तःस्थ तार्किक isInv(ध्रुव Mat_व्यञ्जकम्& e) { प्रत्यागम e.op == &g_MatOp_Invert; }
स्थिर अन्तःस्थ तार्किक isSolve(ध्रुव Mat_व्यञ्जकम्& e) { प्रत्यागम e.op == &g_MatOp_Solve; }
//static inline bool isGEMM(const MatExpr& e) { return e.op == &g_MatOp_GEMM; }
स्थिर अन्तःस्थ तार्किक isMatProd(ध्रुव Mat_व्यञ्जकम्& e) { प्रत्यागम e.op == &g_MatOp_GEMM && (!e.c.दत्त || e.beta == 0); }
स्थिर अन्तःस्थ तार्किक isInitializer(ध्रुव Mat_व्यञ्जकम्& e) { प्रत्यागम e.op == getGlobalMatOpInitializer(); }

/////////////////////////////////////////////////////////////////////////////////////////////////////

MatOp::MatOp() {}
MatOp::~MatOp() {}


तार्किक MatOp::elementWise(ध्रुव Mat_व्यञ्जकम्& /*व्यञ्जकम्*/) ध्रुव
{
    प्रत्यागम असत्य;
}

शून्य MatOp::roi(ध्रुव Mat_व्यञ्जकम्& व्यञ्जकम्, ध्रुव Range& rowRange, ध्रुव Range& colRange, Mat_व्यञ्जकम्& e) ध्रुव
{
    यदि( elementWise(व्यञ्जकम्) )
    {
        e = Mat_व्यञ्जकम्(व्यञ्जकम्.op, व्यञ्जकम्.flags, Mat(), Mat(), Mat(),
                    व्यञ्जकम्.alpha, व्यञ्जकम्.beta, व्यञ्जकम्.s);
        यदि(व्यञ्जकम्.a.दत्त)
            e.a = व्यञ्जकम्.a(rowRange, colRange);
        यदि(व्यञ्जकम्.b.दत्त)
            e.b = व्यञ्जकम्.b(rowRange, colRange);
        यदि(व्यञ्जकम्.c.दत्त)
            e.c = व्यञ्जकम्.c(rowRange, colRange);
    }
    अन्यथा
    {
        Mat m;
        व्यञ्जकम्.op->assign(व्यञ्जकम्, m);
        e = Mat_व्यञ्जकम्(&g_Mat_Op_तटस्थः, 0, m(rowRange, colRange), Mat(), Mat());
    }
}

शून्य MatOp::diag(ध्रुव Mat_व्यञ्जकम्& व्यञ्जकम्, पूर्णाङ्क d, Mat_व्यञ्जकम्& e) ध्रुव
{
    यदि( elementWise(व्यञ्जकम्) )
    {
        e = Mat_व्यञ्जकम्(व्यञ्जकम्.op, व्यञ्जकम्.flags, Mat(), Mat(), Mat(),
                    व्यञ्जकम्.alpha, व्यञ्जकम्.beta, व्यञ्जकम्.s);
        यदि(व्यञ्जकम्.a.दत्त)
            e.a = व्यञ्जकम्.a.diag(d);
        यदि(व्यञ्जकम्.b.दत्त)
            e.b = व्यञ्जकम्.b.diag(d);
        यदि(व्यञ्जकम्.c.दत्त)
            e.c = व्यञ्जकम्.c.diag(d);
    }
    अन्यथा
    {
        Mat m;
        व्यञ्जकम्.op->assign(व्यञ्जकम्, m);
        e = Mat_व्यञ्जकम्(&g_Mat_Op_तटस्थः, 0, m.diag(d), Mat(), Mat());
    }
}


शून्य MatOp::aug_नियोजय_सङ्कलय(ध्रुव Mat_व्यञ्जकम्& व्यञ्जकम्, Mat& m) ध्रुव
{
    Mat अस्थायी_2;
    व्यञ्जकम्.op->assign(व्यञ्जकम्, अस्थायी_2);
    m += अस्थायी_2;
}


शून्य MatOp::aug_नियोजय_व्यवकलय(ध्रुव Mat_व्यञ्जकम्& व्यञ्जकम्, Mat& m) ध्रुव
{
    Mat अस्थायी_2;
    व्यञ्जकम्.op->assign(व्यञ्जकम्, अस्थायी_2);
    m -= अस्थायी_2;
}


शून्य MatOp::aug_नियोजय_Multiply(ध्रुव Mat_व्यञ्जकम्& व्यञ्जकम्, Mat& m) ध्रुव
{
    Mat अस्थायी_2;
    व्यञ्जकम्.op->assign(व्यञ्जकम्, अस्थायी_2);
    m *= अस्थायी_2;
}


शून्य MatOp::aug_नियोजय_Divide(ध्रुव Mat_व्यञ्जकम्& व्यञ्जकम्, Mat& m) ध्रुव
{
    Mat अस्थायी_2;
    व्यञ्जकम्.op->assign(व्यञ्जकम्, अस्थायी_2);
    m /= अस्थायी_2;
}


शून्य MatOp::aug_नियोजय_And(ध्रुव Mat_व्यञ्जकम्& व्यञ्जकम्, Mat& m) ध्रुव
{
    Mat अस्थायी_2;
    व्यञ्जकम्.op->assign(व्यञ्जकम्, अस्थायी_2);
    m &= अस्थायी_2;
}


शून्य MatOp::aug_नियोजय_Or(ध्रुव Mat_व्यञ्जकम्& व्यञ्जकम्, Mat& m) ध्रुव
{
    Mat अस्थायी_2;
    व्यञ्जकम्.op->assign(व्यञ्जकम्, अस्थायी_2);
    m |= अस्थायी_2;
}


शून्य MatOp::aug_नियोजय_Xor(ध्रुव Mat_व्यञ्जकम्& व्यञ्जकम्, Mat& m) ध्रुव
{
    Mat अस्थायी_2;
    व्यञ्जकम्.op->assign(व्यञ्जकम्, अस्थायी_2);
    m ^= अस्थायी_2;
}


शून्य MatOp::सङ्कलय(ध्रुव Mat_व्यञ्जकम्& e1, ध्रुव Mat_व्यञ्जकम्& e2, Mat_व्यञ्जकम्& res) ध्रुव
{
    CV_INSTRUMENT_REGION();

    यदि( एतत् == e2.op )
    {
        द्विगुण alpha = 1, beta = 1;
        Scalar s;
        Mat m1, m2;
        यदि( is_सङ्कलय_Ex(e1) && (!e1.b.दत्त || e1.beta == 0) )
        {
            m1 = e1.a;
            alpha = e1.alpha;
            s = e1.s;
        }
        अन्यथा
            e1.op->assign(e1, m1);

        यदि( is_सङ्कलय_Ex(e2) && (!e2.b.दत्त || e2.beta == 0) )
        {
            m2 = e2.a;
            beta = e2.alpha;
            s += e2.s;
        }
        अन्यथा
            e2.op->assign(e2, m2);
        Mat_Op_सङ्कलय_Ex::make_व्यञ्जकम्(res, m1, m2, alpha, beta, s);
    }
    अन्यथा
        e2.op->सङ्कलय(e1, e2, res);
}


शून्य MatOp::सङ्कलय(ध्रुव Mat_व्यञ्जकम्& expr1, ध्रुव Scalar& s, Mat_व्यञ्जकम्& res) ध्रुव
{
    CV_INSTRUMENT_REGION();

    Mat m1;
    expr1.op->assign(expr1, m1);
    Mat_Op_सङ्कलय_Ex::make_व्यञ्जकम्(res, m1, Mat(), 1, 0, s);
}


शून्य MatOp::व्यवकलय(ध्रुव Mat_व्यञ्जकम्& e1, ध्रुव Mat_व्यञ्जकम्& e2, Mat_व्यञ्जकम्& res) ध्रुव
{
    CV_INSTRUMENT_REGION();

    यदि( एतत् == e2.op )
    {
        द्विगुण alpha = 1, beta = -1;
        Scalar s;
        Mat m1, m2;
        यदि( is_सङ्कलय_Ex(e1) && (!e1.b.दत्त || e1.beta == 0) )
        {
            m1 = e1.a;
            alpha = e1.alpha;
            s = e1.s;
        }
        अन्यथा
            e1.op->assign(e1, m1);

        यदि( is_सङ्कलय_Ex(e2) && (!e2.b.दत्त || e2.beta == 0) )
        {
            m2 = e2.a;
            beta = -e2.alpha;
            s -= e2.s;
        }
        अन्यथा
            e2.op->assign(e2, m2);
        Mat_Op_सङ्कलय_Ex::make_व्यञ्जकम्(res, m1, m2, alpha, beta, s);
    }
    अन्यथा
        e2.op->व्यवकलय(e1, e2, res);
}


शून्य MatOp::व्यवकलय(ध्रुव Scalar& s, ध्रुव Mat_व्यञ्जकम्& व्यञ्जकम्, Mat_व्यञ्जकम्& res) ध्रुव
{
    CV_INSTRUMENT_REGION();

    Mat m;
    व्यञ्जकम्.op->assign(व्यञ्जकम्, m);
    Mat_Op_सङ्कलय_Ex::make_व्यञ्जकम्(res, m, Mat(), -1, 0, s);
}


शून्य MatOp::multiply(ध्रुव Mat_व्यञ्जकम्& e1, ध्रुव Mat_व्यञ्जकम्& e2, Mat_व्यञ्जकम्& res, द्विगुण scale) ध्रुव
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

            MatOp_Bin::make_व्यञ्जकम्(res, '/', m2, e1.a, scale/e1.alpha);
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

            MatOp_Bin::make_व्यञ्जकम्(res, op, m1, m2, scale);
        }
    }
    अन्यथा
        e2.op->multiply(e1, e2, res, scale);
}


शून्य MatOp::multiply(ध्रुव Mat_व्यञ्जकम्& व्यञ्जकम्, द्विगुण s, Mat_व्यञ्जकम्& res) ध्रुव
{
    CV_INSTRUMENT_REGION();

    Mat m;
    व्यञ्जकम्.op->assign(व्यञ्जकम्, m);
    Mat_Op_सङ्कलय_Ex::make_व्यञ्जकम्(res, m, Mat(), s, 0);
}


शून्य MatOp::divide(ध्रुव Mat_व्यञ्जकम्& e1, ध्रुव Mat_व्यञ्जकम्& e2, Mat_व्यञ्जकम्& res, द्विगुण scale) ध्रुव
{
    CV_INSTRUMENT_REGION();

    यदि( एतत् == e2.op )
    {
        यदि( isReciprocal(e1) && isReciprocal(e2) )
            MatOp_Bin::make_व्यञ्जकम्(res, '/', e2.a, e1.a, e1.alpha/e2.alpha);
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
            MatOp_Bin::make_व्यञ्जकम्(res, op, m1, m2, scale);
        }
    }
    अन्यथा
        e2.op->divide(e1, e2, res, scale);
}


शून्य MatOp::divide(द्विगुण s, ध्रुव Mat_व्यञ्जकम्& व्यञ्जकम्, Mat_व्यञ्जकम्& res) ध्रुव
{
    CV_INSTRUMENT_REGION();

    Mat m;
    व्यञ्जकम्.op->assign(व्यञ्जकम्, m);
    MatOp_Bin::make_व्यञ्जकम्(res, '/', m, Mat(), s);
}


शून्य MatOp::निरपेक्षम्(ध्रुव Mat_व्यञ्जकम्& व्यञ्जकम्, Mat_व्यञ्जकम्& res) ध्रुव
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

        MatOp_GEMM::make_व्यञ्जकम्(res, flags, m1, m2, scale);
    }
    अन्यथा
        e2.op->matmul(e1, e2, res);
}


शून्य MatOp::invert(ध्रुव Mat_व्यञ्जकम्& व्यञ्जकम्, पूर्णाङ्क विधानम्, Mat_व्यञ्जकम्& res) ध्रुव
{
    Mat m;
    व्यञ्जकम्.op->assign(व्यञ्जकम्, m);
    MatOp_Invert::make_व्यञ्जकम्(res, विधानम्, m);
}


आकार MatOp::size(ध्रुव Mat_व्यञ्जकम्& व्यञ्जकम्) ध्रुव
{
    प्रत्यागम !व्यञ्जकम्.a.empty() ? व्यञ्जकम्.a.size() : व्यञ्जकम्.b.empty() ? व्यञ्जकम्.b.size() : व्यञ्जकम्.c.size();
}

पूर्णाङ्क MatOp::type(ध्रुव Mat_व्यञ्जकम्& व्यञ्जकम्) ध्रुव
{
    CV_INSTRUMENT_REGION();

    प्रत्यागम !व्यञ्जकम्.a.empty() ? व्यञ्जकम्.a.type() : व्यञ्जकम्.b.empty() ? व्यञ्जकम्.b.type() : व्यञ्जकम्.c.type();
}

//////////////////////////////////////////////////////////////////////////////////////////////////

Mat_व्यञ्जकम्::Mat_व्यञ्जकम्(ध्रुव Mat& m) : op(&g_Mat_Op_तटस्थः), flags(0), a(m), b(Mat()), c(Mat()), alpha(1), beta(0), s(Scalar())
{
}

Mat_व्यञ्जकम् Mat_व्यञ्जकम्::row(पूर्णाङ्क y) ध्रुव
{
    Mat_व्यञ्जकम् e;
    op->roi(*एतत्, Range(y, y+1), Range::all(), e);
    प्रत्यागम e;
}

Mat_व्यञ्जकम् Mat_व्यञ्जकम्::col(पूर्णाङ्क x) ध्रुव
{
    Mat_व्यञ्जकम् e;
    op->roi(*एतत्, Range::all(), Range(x, x+1), e);
    प्रत्यागम e;
}

Mat_व्यञ्जकम् Mat_व्यञ्जकम्::diag(पूर्णाङ्क d) ध्रुव
{
    Mat_व्यञ्जकम् e;
    op->diag(*एतत्, d, e);
    प्रत्यागम e;
}

Mat_व्यञ्जकम् Mat_व्यञ्जकम्::संक्रियकः()( ध्रुव Range& rowRange, ध्रुव Range& colRange ) ध्रुव
{
    Mat_व्यञ्जकम् e;
    op->roi(*एतत्, rowRange, colRange, e);
    प्रत्यागम e;
}

Mat_व्यञ्जकम् Mat_व्यञ्जकम्::संक्रियकः()( ध्रुव Rect& roi ) ध्रुव
{
    Mat_व्यञ्जकम् e;
    op->roi(*एतत्, Range(roi.y, roi.y + roi.height), Range(roi.x, roi.x + roi.विस्तारः), e);
    प्रत्यागम e;
}

Mat Mat_व्यञ्जकम्::cross(ध्रुव Mat& m) ध्रुव
{
    प्रत्यागम ((Mat)*एतत्).cross(m);
}

द्विगुण Mat_व्यञ्जकम्::dot(ध्रुव Mat& m) ध्रुव
{
    प्रत्यागम ((Mat)*एतत्).dot(m);
}

Mat_व्यञ्जकम् Mat_व्यञ्जकम्::t() ध्रुव
{
    Mat_व्यञ्जकम् e;
    op->transpose(*एतत्, e);
    प्रत्यागम e;
}

Mat_व्यञ्जकम् Mat_व्यञ्जकम्::inv(पूर्णाङ्क विधानम्) ध्रुव
{
    Mat_व्यञ्जकम् e;
    op->invert(*एतत्, विधानम्, e);
    प्रत्यागम e;
}

Mat_व्यञ्जकम् Mat_व्यञ्जकम्::mul(ध्रुव Mat_व्यञ्जकम्& e, द्विगुण scale) ध्रुव
{
    Mat_व्यञ्जकम् en;
    op->multiply(*एतत्, e, en, scale);
    प्रत्यागम en;
}

Mat_व्यञ्जकम् Mat_व्यञ्जकम्::mul(ध्रुव Mat& m, द्विगुण scale) ध्रुव
{
    Mat_व्यञ्जकम् e;
    op->multiply(*एतत्, Mat_व्यञ्जकम्(m), e, scale);
    प्रत्यागम e;
}

Mat_व्यञ्जकम् संक्रियकः + (ध्रुव Mat& a, ध्रुव Mat& b)
{
    checkOperandsExist(a, b);
    Mat_व्यञ्जकम् e;
    Mat_Op_सङ्कलय_Ex::make_व्यञ्जकम्(e, a, b, 1, 1);
    प्रत्यागम e;
}

Mat_व्यञ्जकम् संक्रियकः + (ध्रुव Mat& a, ध्रुव Scalar& s)
{
    checkOperandsExist(a);
    Mat_व्यञ्जकम् e;
    Mat_Op_सङ्कलय_Ex::make_व्यञ्जकम्(e, a, Mat(), 1, 0, s);
    प्रत्यागम e;
}

Mat_व्यञ्जकम् संक्रियकः + (ध्रुव Scalar& s, ध्रुव Mat& a)
{
    checkOperandsExist(a);
    Mat_व्यञ्जकम् e;
    Mat_Op_सङ्कलय_Ex::make_व्यञ्जकम्(e, a, Mat(), 1, 0, s);
    प्रत्यागम e;
}

Mat_व्यञ्जकम् संक्रियकः + (ध्रुव Mat_व्यञ्जकम्& e, ध्रुव Mat& m)
{
    Mat_व्यञ्जकम् en;
    e.op->सङ्कलय(e, Mat_व्यञ्जकम्(m), en);
    प्रत्यागम en;
}

Mat_व्यञ्जकम् संक्रियकः + (ध्रुव Mat& m, ध्रुव Mat_व्यञ्जकम्& e)
{
    checkOperandsExist(m);
    Mat_व्यञ्जकम् en;
    e.op->सङ्कलय(e, Mat_व्यञ्जकम्(m), en);
    प्रत्यागम en;
}

Mat_व्यञ्जकम् संक्रियकः + (ध्रुव Mat_व्यञ्जकम्& e, ध्रुव Scalar& s)
{
    Mat_व्यञ्जकम् en;
    e.op->सङ्कलय(e, s, en);
    प्रत्यागम en;
}

Mat_व्यञ्जकम् संक्रियकः + (ध्रुव Scalar& s, ध्रुव Mat_व्यञ्जकम्& e)
{
    Mat_व्यञ्जकम् en;
    e.op->सङ्कलय(e, s, en);
    प्रत्यागम en;
}

Mat_व्यञ्जकम् संक्रियकः + (ध्रुव Mat_व्यञ्जकम्& e1, ध्रुव Mat_व्यञ्जकम्& e2)
{
    Mat_व्यञ्जकम् en;
    e1.op->सङ्कलय(e1, e2, en);
    प्रत्यागम en;
}

Mat_व्यञ्जकम् संक्रियकः - (ध्रुव Mat& a, ध्रुव Mat& b)
{
    checkOperandsExist(a, b);
    Mat_व्यञ्जकम् e;
    Mat_Op_सङ्कलय_Ex::make_व्यञ्जकम्(e, a, b, 1, -1);
    प्रत्यागम e;
}

Mat_व्यञ्जकम् संक्रियकः - (ध्रुव Mat& a, ध्रुव Scalar& s)
{
    checkOperandsExist(a);
    Mat_व्यञ्जकम् e;
    Mat_Op_सङ्कलय_Ex::make_व्यञ्जकम्(e, a, Mat(), 1, 0, -s);
    प्रत्यागम e;
}

Mat_व्यञ्जकम् संक्रियकः - (ध्रुव Scalar& s, ध्रुव Mat& a)
{
    checkOperandsExist(a);
    Mat_व्यञ्जकम् e;
    Mat_Op_सङ्कलय_Ex::make_व्यञ्जकम्(e, a, Mat(), -1, 0, s);
    प्रत्यागम e;
}

Mat_व्यञ्जकम् संक्रियकः - (ध्रुव Mat_व्यञ्जकम्& e, ध्रुव Mat& m)
{
    checkOperandsExist(m);
    Mat_व्यञ्जकम् en;
    e.op->व्यवकलय(e, Mat_व्यञ्जकम्(m), en);
    प्रत्यागम en;
}

Mat_व्यञ्जकम् संक्रियकः - (ध्रुव Mat& m, ध्रुव Mat_व्यञ्जकम्& e)
{
    checkOperandsExist(m);
    Mat_व्यञ्जकम् en;
    e.op->व्यवकलय(Mat_व्यञ्जकम्(m), e, en);
    प्रत्यागम en;
}

Mat_व्यञ्जकम् संक्रियकः - (ध्रुव Mat_व्यञ्जकम्& e, ध्रुव Scalar& s)
{
    Mat_व्यञ्जकम् en;
    e.op->सङ्कलय(e, -s, en);
    प्रत्यागम en;
}

Mat_व्यञ्जकम् संक्रियकः - (ध्रुव Scalar& s, ध्रुव Mat_व्यञ्जकम्& e)
{
    Mat_व्यञ्जकम् en;
    e.op->व्यवकलय(s, e, en);
    प्रत्यागम en;
}

Mat_व्यञ्जकम् संक्रियकः - (ध्रुव Mat_व्यञ्जकम्& e1, ध्रुव Mat_व्यञ्जकम्& e2)
{
    Mat_व्यञ्जकम् en;
    e1.op->व्यवकलय(e1, e2, en);
    प्रत्यागम en;
}

Mat_व्यञ्जकम् संक्रियकः - (ध्रुव Mat& m)
{
    checkOperandsExist(m);
    Mat_व्यञ्जकम् e;
    Mat_Op_सङ्कलय_Ex::make_व्यञ्जकम्(e, m, Mat(), -1, 0);
    प्रत्यागम e;
}

Mat_व्यञ्जकम् संक्रियकः - (ध्रुव Mat_व्यञ्जकम्& e)
{
    Mat_व्यञ्जकम् en;
    e.op->व्यवकलय(Scalar(0), e, en);
    प्रत्यागम en;
}

Mat_व्यञ्जकम् संक्रियकः * (ध्रुव Mat& a, ध्रुव Mat& b)
{
    checkOperandsExist(a, b);
    Mat_व्यञ्जकम् e;
    MatOp_GEMM::make_व्यञ्जकम्(e, 0, a, b);
    प्रत्यागम e;
}

Mat_व्यञ्जकम् संक्रियकः * (ध्रुव Mat& a, द्विगुण s)
{
    checkOperandsExist(a);
    Mat_व्यञ्जकम् e;
    Mat_Op_सङ्कलय_Ex::make_व्यञ्जकम्(e, a, Mat(), s, 0);
    प्रत्यागम e;
}

Mat_व्यञ्जकम् संक्रियकः * (द्विगुण s, ध्रुव Mat& a)
{
    checkOperandsExist(a);
    Mat_व्यञ्जकम् e;
    Mat_Op_सङ्कलय_Ex::make_व्यञ्जकम्(e, a, Mat(), s, 0);
    प्रत्यागम e;
}

Mat_व्यञ्जकम् संक्रियकः * (ध्रुव Mat_व्यञ्जकम्& e, ध्रुव Mat& m)
{
    Mat_व्यञ्जकम् en;
    e.op->matmul(e, Mat_व्यञ्जकम्(m), en);
    प्रत्यागम en;
}

Mat_व्यञ्जकम् संक्रियकः * (ध्रुव Mat& m, ध्रुव Mat_व्यञ्जकम्& e)
{
    checkOperandsExist(m);
    Mat_व्यञ्जकम् en;
    e.op->matmul(Mat_व्यञ्जकम्(m), e, en);
    प्रत्यागम en;
}

Mat_व्यञ्जकम् संक्रियकः * (ध्रुव Mat_व्यञ्जकम्& e, द्विगुण s)
{
    Mat_व्यञ्जकम् en;
    e.op->multiply(e, s, en);
    प्रत्यागम en;
}

Mat_व्यञ्जकम् संक्रियकः * (द्विगुण s, ध्रुव Mat_व्यञ्जकम्& e)
{
    Mat_व्यञ्जकम् en;
    e.op->multiply(e, s, en);
    प्रत्यागम en;
}

Mat_व्यञ्जकम् संक्रियकः * (ध्रुव Mat_व्यञ्जकम्& e1, ध्रुव Mat_व्यञ्जकम्& e2)
{
    Mat_व्यञ्जकम् en;
    e1.op->matmul(e1, e2, en);
    प्रत्यागम en;
}

Mat_व्यञ्जकम् संक्रियकः / (ध्रुव Mat& a, ध्रुव Mat& b)
{
    checkOperandsExist(a, b);
    Mat_व्यञ्जकम् e;
    MatOp_Bin::make_व्यञ्जकम्(e, '/', a, b);
    प्रत्यागम e;
}

Mat_व्यञ्जकम् संक्रियकः / (ध्रुव Mat& a, द्विगुण s)
{
    checkOperandsExist(a);
    Mat_व्यञ्जकम् e;
    Mat_Op_सङ्कलय_Ex::make_व्यञ्जकम्(e, a, Mat(), 1./s, 0);
    प्रत्यागम e;
}

Mat_व्यञ्जकम् संक्रियकः / (द्विगुण s, ध्रुव Mat& a)
{
    checkOperandsExist(a);
    Mat_व्यञ्जकम् e;
    MatOp_Bin::make_व्यञ्जकम्(e, '/', a, Mat(), s);
    प्रत्यागम e;
}

Mat_व्यञ्जकम् संक्रियकः / (ध्रुव Mat_व्यञ्जकम्& e, ध्रुव Mat& m)
{
    Mat_व्यञ्जकम् en;
    e.op->divide(e, Mat_व्यञ्जकम्(m), en);
    प्रत्यागम en;
}

Mat_व्यञ्जकम् संक्रियकः / (ध्रुव Mat& m, ध्रुव Mat_व्यञ्जकम्& e)
{
    checkOperandsExist(m);
    Mat_व्यञ्जकम् en;
    e.op->divide(Mat_व्यञ्जकम्(m), e, en);
    प्रत्यागम en;
}

Mat_व्यञ्जकम् संक्रियकः / (ध्रुव Mat_व्यञ्जकम्& e, द्विगुण s)
{
    Mat_व्यञ्जकम् en;
    e.op->multiply(e, 1./s, en);
    प्रत्यागम en;
}

Mat_व्यञ्जकम् संक्रियकः / (द्विगुण s, ध्रुव Mat_व्यञ्जकम्& e)
{
    Mat_व्यञ्जकम् en;
    e.op->divide(s, e, en);
    प्रत्यागम en;
}

Mat_व्यञ्जकम् संक्रियकः / (ध्रुव Mat_व्यञ्जकम्& e1, ध्रुव Mat_व्यञ्जकम्& e2)
{
    Mat_व्यञ्जकम् en;
    e1.op->divide(e1, e2, en);
    प्रत्यागम en;
}

Mat_व्यञ्जकम् संक्रियकः < (ध्रुव Mat& a, ध्रुव Mat& b)
{
    checkOperandsExist(a, b);
    Mat_व्यञ्जकम् e;
    MatOp_Cmp::make_व्यञ्जकम्(e, cv::CMP_LT, a, b);
    प्रत्यागम e;
}

Mat_व्यञ्जकम् संक्रियकः < (ध्रुव Mat& a, द्विगुण s)
{
    checkOperandsExist(a);
    Mat_व्यञ्जकम् e;
    MatOp_Cmp::make_व्यञ्जकम्(e, cv::CMP_LT, a, s);
    प्रत्यागम e;
}

Mat_व्यञ्जकम् संक्रियकः < (द्विगुण s, ध्रुव Mat& a)
{
    checkOperandsExist(a);
    Mat_व्यञ्जकम् e;
    MatOp_Cmp::make_व्यञ्जकम्(e, cv::CMP_GT, a, s);
    प्रत्यागम e;
}

Mat_व्यञ्जकम् संक्रियकः <= (ध्रुव Mat& a, ध्रुव Mat& b)
{
    checkOperandsExist(a, b);
    Mat_व्यञ्जकम् e;
    MatOp_Cmp::make_व्यञ्जकम्(e, cv::CMP_LE, a, b);
    प्रत्यागम e;
}

Mat_व्यञ्जकम् संक्रियकः <= (ध्रुव Mat& a, द्विगुण s)
{
    checkOperandsExist(a);
    Mat_व्यञ्जकम् e;
    MatOp_Cmp::make_व्यञ्जकम्(e, cv::CMP_LE, a, s);
    प्रत्यागम e;
}

Mat_व्यञ्जकम् संक्रियकः <= (द्विगुण s, ध्रुव Mat& a)
{
    checkOperandsExist(a);
    Mat_व्यञ्जकम् e;
    MatOp_Cmp::make_व्यञ्जकम्(e, cv::CMP_GE, a, s);
    प्रत्यागम e;
}

Mat_व्यञ्जकम् संक्रियकः == (ध्रुव Mat& a, ध्रुव Mat& b)
{
    checkOperandsExist(a, b);
    Mat_व्यञ्जकम् e;
    MatOp_Cmp::make_व्यञ्जकम्(e, cv::CMP_EQ, a, b);
    प्रत्यागम e;
}

Mat_व्यञ्जकम् संक्रियकः == (ध्रुव Mat& a, द्विगुण s)
{
    checkOperandsExist(a);
    Mat_व्यञ्जकम् e;
    MatOp_Cmp::make_व्यञ्जकम्(e, cv::CMP_EQ, a, s);
    प्रत्यागम e;
}

Mat_व्यञ्जकम् संक्रियकः == (द्विगुण s, ध्रुव Mat& a)
{
    checkOperandsExist(a);
    Mat_व्यञ्जकम् e;
    MatOp_Cmp::make_व्यञ्जकम्(e, cv::CMP_EQ, a, s);
    प्रत्यागम e;
}

Mat_व्यञ्जकम् संक्रियकः != (ध्रुव Mat& a, ध्रुव Mat& b)
{
    checkOperandsExist(a, b);
    Mat_व्यञ्जकम् e;
    MatOp_Cmp::make_व्यञ्जकम्(e, cv::CMP_NE, a, b);
    प्रत्यागम e;
}

Mat_व्यञ्जकम् संक्रियकः != (ध्रुव Mat& a, द्विगुण s)
{
    checkOperandsExist(a);
    Mat_व्यञ्जकम् e;
    MatOp_Cmp::make_व्यञ्जकम्(e, cv::CMP_NE, a, s);
    प्रत्यागम e;
}

Mat_व्यञ्जकम् संक्रियकः != (द्विगुण s, ध्रुव Mat& a)
{
    checkOperandsExist(a);
    Mat_व्यञ्जकम् e;
    MatOp_Cmp::make_व्यञ्जकम्(e, cv::CMP_NE, a, s);
    प्रत्यागम e;
}

Mat_व्यञ्जकम् संक्रियकः >= (ध्रुव Mat& a, ध्रुव Mat& b)
{
    checkOperandsExist(a, b);
    Mat_व्यञ्जकम् e;
    MatOp_Cmp::make_व्यञ्जकम्(e, cv::CMP_GE, a, b);
    प्रत्यागम e;
}

Mat_व्यञ्जकम् संक्रियकः >= (ध्रुव Mat& a, द्विगुण s)
{
    checkOperandsExist(a);
    Mat_व्यञ्जकम् e;
    MatOp_Cmp::make_व्यञ्जकम्(e, cv::CMP_GE, a, s);
    प्रत्यागम e;
}

Mat_व्यञ्जकम् संक्रियकः >= (द्विगुण s, ध्रुव Mat& a)
{
    checkOperandsExist(a);
    Mat_व्यञ्जकम् e;
    MatOp_Cmp::make_व्यञ्जकम्(e, cv::CMP_LE, a, s);
    प्रत्यागम e;
}

Mat_व्यञ्जकम् संक्रियकः > (ध्रुव Mat& a, ध्रुव Mat& b)
{
    checkOperandsExist(a, b);
    Mat_व्यञ्जकम् e;
    MatOp_Cmp::make_व्यञ्जकम्(e, cv::CMP_GT, a, b);
    प्रत्यागम e;
}

Mat_व्यञ्जकम् संक्रियकः > (ध्रुव Mat& a, द्विगुण s)
{
    checkOperandsExist(a);
    Mat_व्यञ्जकम् e;
    MatOp_Cmp::make_व्यञ्जकम्(e, cv::CMP_GT, a, s);
    प्रत्यागम e;
}

Mat_व्यञ्जकम् संक्रियकः > (द्विगुण s, ध्रुव Mat& a)
{
    checkOperandsExist(a);
    Mat_व्यञ्जकम् e;
    MatOp_Cmp::make_व्यञ्जकम्(e, cv::CMP_LT, a, s);
    प्रत्यागम e;
}

Mat_व्यञ्जकम् min(ध्रुव Mat& a, ध्रुव Mat& b)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a, b);
    Mat_व्यञ्जकम् e;
    MatOp_Bin::make_व्यञ्जकम्(e, 'm', a, b);
    प्रत्यागम e;
}

Mat_व्यञ्जकम् min(ध्रुव Mat& a, द्विगुण s)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    Mat_व्यञ्जकम् e;
    MatOp_Bin::make_व्यञ्जकम्(e, 'n', a, s);
    प्रत्यागम e;
}

Mat_व्यञ्जकम् min(द्विगुण s, ध्रुव Mat& a)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    Mat_व्यञ्जकम् e;
    MatOp_Bin::make_व्यञ्जकम्(e, 'n', a, s);
    प्रत्यागम e;
}

Mat_व्यञ्जकम् max(ध्रुव Mat& a, ध्रुव Mat& b)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a, b);
    Mat_व्यञ्जकम् e;
    MatOp_Bin::make_व्यञ्जकम्(e, 'M', a, b);
    प्रत्यागम e;
}

Mat_व्यञ्जकम् max(ध्रुव Mat& a, द्विगुण s)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    Mat_व्यञ्जकम् e;
    MatOp_Bin::make_व्यञ्जकम्(e, 'N', a, s);
    प्रत्यागम e;
}

Mat_व्यञ्जकम् max(द्विगुण s, ध्रुव Mat& a)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    Mat_व्यञ्जकम् e;
    MatOp_Bin::make_व्यञ्जकम्(e, 'N', a, s);
    प्रत्यागम e;
}

Mat_व्यञ्जकम् संक्रियकः & (ध्रुव Mat& a, ध्रुव Mat& b)
{
    checkOperandsExist(a, b);
    Mat_व्यञ्जकम् e;
    MatOp_Bin::make_व्यञ्जकम्(e, '&', a, b);
    प्रत्यागम e;
}

Mat_व्यञ्जकम् संक्रियकः & (ध्रुव Mat& a, ध्रुव Scalar& s)
{
    checkOperandsExist(a);
    Mat_व्यञ्जकम् e;
    MatOp_Bin::make_व्यञ्जकम्(e, '&', a, s);
    प्रत्यागम e;
}

Mat_व्यञ्जकम् संक्रियकः & (ध्रुव Scalar& s, ध्रुव Mat& a)
{
    checkOperandsExist(a);
    Mat_व्यञ्जकम् e;
    MatOp_Bin::make_व्यञ्जकम्(e, '&', a, s);
    प्रत्यागम e;
}

Mat_व्यञ्जकम् संक्रियकः | (ध्रुव Mat& a, ध्रुव Mat& b)
{
    checkOperandsExist(a, b);
    Mat_व्यञ्जकम् e;
    MatOp_Bin::make_व्यञ्जकम्(e, '|', a, b);
    प्रत्यागम e;
}

Mat_व्यञ्जकम् संक्रियकः | (ध्रुव Mat& a, ध्रुव Scalar& s)
{
    checkOperandsExist(a);
    Mat_व्यञ्जकम् e;
    MatOp_Bin::make_व्यञ्जकम्(e, '|', a, s);
    प्रत्यागम e;
}

Mat_व्यञ्जकम् संक्रियकः | (ध्रुव Scalar& s, ध्रुव Mat& a)
{
    checkOperandsExist(a);
    Mat_व्यञ्जकम् e;
    MatOp_Bin::make_व्यञ्जकम्(e, '|', a, s);
    प्रत्यागम e;
}

Mat_व्यञ्जकम् संक्रियकः ^ (ध्रुव Mat& a, ध्रुव Mat& b)
{
    checkOperandsExist(a, b);
    Mat_व्यञ्जकम् e;
    MatOp_Bin::make_व्यञ्जकम्(e, '^', a, b);
    प्रत्यागम e;
}

Mat_व्यञ्जकम् संक्रियकः ^ (ध्रुव Mat& a, ध्रुव Scalar& s)
{
    checkOperandsExist(a);
    Mat_व्यञ्जकम् e;
    MatOp_Bin::make_व्यञ्जकम्(e, '^', a, s);
    प्रत्यागम e;
}

Mat_व्यञ्जकम् संक्रियकः ^ (ध्रुव Scalar& s, ध्रुव Mat& a)
{
    checkOperandsExist(a);
    Mat_व्यञ्जकम् e;
    MatOp_Bin::make_व्यञ्जकम्(e, '^', a, s);
    प्रत्यागम e;
}

Mat_व्यञ्जकम् संक्रियकः ~(ध्रुव Mat& a)
{
    checkOperandsExist(a);
    Mat_व्यञ्जकम् e;
    MatOp_Bin::make_व्यञ्जकम्(e, '~', a, Scalar());
    प्रत्यागम e;
}

Mat_व्यञ्जकम् निरपेक्षम्(ध्रुव Mat& a)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    Mat_व्यञ्जकम् e;
    MatOp_Bin::make_व्यञ्जकम्(e, 'a', a, Scalar());
    प्रत्यागम e;
}

Mat_व्यञ्जकम् निरपेक्षम्(ध्रुव Mat_व्यञ्जकम्& e)
{
    CV_INSTRUMENT_REGION();

    Mat_व्यञ्जकम् en;
    e.op->निरपेक्षम्(e, en);
    प्रत्यागम en;
}


आकार Mat_व्यञ्जकम्::size() ध्रुव
{
    यदि( isT(*एतत्) || isInv(*एतत्) )
        प्रत्यागम आकार(a.पङ्क्तिकाः, a.स्तम्भसङ्ख्या);
    यदि( isSolve(*एतत्) )
        प्रत्यागम आकार(b.स्तम्भसङ्ख्या, a.स्तम्भसङ्ख्या);
    यदि( isInitializer(*एतत्) )
        प्रत्यागम a.size();
    प्रत्यागम op ? op->size(*एतत्) : आकार();
}


पूर्णाङ्क Mat_व्यञ्जकम्::type() ध्रुव
{
    CV_INSTRUMENT_REGION();

    यदि( isInitializer(*एतत्) )
        प्रत्यागम a.type();
    यदि( isCmp(*एतत्) )
        प्रत्यागम CV_MAKETYPE(CV_8U, a.channels());
    प्रत्यागम op ? op->type(*एतत्) : -1;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////

शून्य Mat_Op_तटस्थः::assign(ध्रुव Mat_व्यञ्जकम्& e, Mat& m, पूर्णाङ्क _type) ध्रुव
{
    यदि( _type == -1 || _type == e.a.type() )
        m = e.a;
    अन्यथा
    {
        CV_Assert( CV_MAT_CN(_type) == e.a.channels() );
        e.a.convert_गन्तव्यम्(m, _type);
    }
}

अन्तःस्थ शून्य Mat_Op_तटस्थः::make_व्यञ्जकम्(Mat_व्यञ्जकम्& res, ध्रुव Mat& m)
{
    res = Mat_व्यञ्जकम्(&g_Mat_Op_तटस्थः, 0, m, Mat(), Mat(), 1, 0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

शून्य Mat_Op_सङ्कलय_Ex::assign(ध्रुव Mat_व्यञ्जकम्& e, Mat& m, पूर्णाङ्क _type) ध्रुव
{
    Mat अस्थायी_2, &dst = _type == -1 || e.a.type() == _type ? m : अस्थायी_2;
    यदि( e.b.दत्त )
    {
        यदि( e.s == Scalar() || !e.s.isReal() )
        {
            यदि( e.alpha == 1 )
            {
                यदि( e.beta == 1 )
                    cv::सङ्कलय(e.a, e.b, dst);
                अन्यथा यदि( e.beta == -1 )
                    cv::व्यवकलय(e.a, e.b, dst);
                अन्यथा
                    cv::scale_सङ्कलय(e.b, e.beta, e.a, dst);
            }
            अन्यथा यदि( e.beta == 1 )
            {
                यदि( e.alpha == -1 )
                    cv::व्यवकलय(e.b, e.a, dst);
                अन्यथा
                    cv::scale_सङ्कलय(e.a, e.alpha, e.b, dst);
            }
            अन्यथा
                cv::सङ्कलय_Weighted(e.a, e.alpha, e.b, e.beta, 0, dst);

            यदि( !e.s.isReal() )
                cv::सङ्कलय(dst, e.s, dst);
        }
        अन्यथा
        {
            यदि (e.a.channels() > 1)
                CV_LOG_ONCE_WARNING(रिक्त, "OpenCV/MatExpr: processing of multi-channel arrays might be changed in the future: "
                                          "https://github.com/opencv/opencv/issues/16739");
            cv::सङ्कलय_Weighted(e.a, e.alpha, e.b, e.beta, e.s[0], dst);
        }
    }
    अन्यथा यदि( e.s.isReal() && (dst.दत्त != m.दत्त || fabs(e.alpha) != 1))
    {
        यदि (e.a.channels() > 1 && e.s[0] != 0.0)
            CV_LOG_ONCE_WARNING(रिक्त, "OpenCV/MatExpr: processing of multi-channel arrays might be changed in the future: "
                                      "https://github.com/opencv/opencv/issues/16739");
        e.a.convert_गन्तव्यम्(m, _type, e.alpha, e.s[0]);
        प्रत्यागम;
    }
    अन्यथा यदि( e.alpha == 1 )
        cv::सङ्कलय(e.a, e.s, dst);
    अन्यथा यदि( e.alpha == -1 )
        cv::व्यवकलय(e.s, e.a, dst);
    अन्यथा
    {
        e.a.convert_गन्तव्यम्(dst, e.a.type(), e.alpha);
        cv::सङ्कलय(dst, e.s, dst);
    }

    यदि( dst.दत्त != m.दत्त )
        dst.convert_गन्तव्यम्(m, m.type());
}


शून्य Mat_Op_सङ्कलय_Ex::सङ्कलय(ध्रुव Mat_व्यञ्जकम्& e, ध्रुव Scalar& s, Mat_व्यञ्जकम्& res) ध्रुव
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.s += s;
}


शून्य Mat_Op_सङ्कलय_Ex::व्यवकलय(ध्रुव Scalar& s, ध्रुव Mat_व्यञ्जकम्& e, Mat_व्यञ्जकम्& res) ध्रुव
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha = -res.alpha;
    res.beta = -res.beta;
    res.s = s - res.s;
}

शून्य Mat_Op_सङ्कलय_Ex::multiply(ध्रुव Mat_व्यञ्जकम्& e, द्विगुण s, Mat_व्यञ्जकम्& res) ध्रुव
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha *= s;
    res.beta *= s;
    res.s *= s;
}

शून्य Mat_Op_सङ्कलय_Ex::divide(द्विगुण s, ध्रुव Mat_व्यञ्जकम्& e, Mat_व्यञ्जकम्& res) ध्रुव
{
    CV_INSTRUMENT_REGION();

    यदि( isScaled(e) )
        MatOp_Bin::make_व्यञ्जकम्(res, '/', e.a, Mat(), s/e.alpha);
    अन्यथा
        MatOp::divide(s, e, res);
}


शून्य Mat_Op_सङ्कलय_Ex::transpose(ध्रुव Mat_व्यञ्जकम्& e, Mat_व्यञ्जकम्& res) ध्रुव
{
    CV_INSTRUMENT_REGION();

    यदि( isScaled(e) )
        MatOp_T::make_व्यञ्जकम्(res, e.a, e.alpha);
    अन्यथा
        MatOp::transpose(e, res);
}

शून्य Mat_Op_सङ्कलय_Ex::निरपेक्षम्(ध्रुव Mat_व्यञ्जकम्& e, Mat_व्यञ्जकम्& res) ध्रुव
{
    CV_INSTRUMENT_REGION();

    यदि( (!e.b.दत्त || e.beta == 0) && fabs(e.alpha) == 1 )
        MatOp_Bin::make_व्यञ्जकम्(res, 'a', e.a, -e.s*e.alpha);
    अन्यथा यदि( e.b.दत्त && e.alpha + e.beta == 0 && e.alpha*e.beta == -1 )
        MatOp_Bin::make_व्यञ्जकम्(res, 'a', e.a, e.b);
    अन्यथा
        MatOp::निरपेक्षम्(e, res);
}

अन्तःस्थ शून्य Mat_Op_सङ्कलय_Ex::make_व्यञ्जकम्(Mat_व्यञ्जकम्& res, ध्रुव Mat& a, ध्रुव Mat& b, द्विगुण alpha, द्विगुण beta, ध्रुव Scalar& s)
{
    res = Mat_व्यञ्जकम्(&g_Mat_Op_सङ्कलय_Ex, 0, a, b, Mat(), alpha, beta, s);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

शून्य MatOp_Bin::assign(ध्रुव Mat_व्यञ्जकम्& e, Mat& m, पूर्णाङ्क _type) ध्रुव
{
    Mat अस्थायी_2, &dst = _type == -1 || e.a.type() == _type ? m : अस्थायी_2;

    यदि( e.flags == '*' )
        cv::multiply(e.a, e.b, dst, e.alpha);
    अन्यथा यदि( e.flags == '/' && e.b.दत्त )
        cv::divide(e.a, e.b, dst, e.alpha);
    अन्यथा यदि( e.flags == '/' && !e.b.दत्त )
        cv::divide(e.alpha, e.a, dst );
    अन्यथा यदि( e.flags == '&' && e.b.दत्त )
        bitwise_and(e.a, e.b, dst);
    अन्यथा यदि( e.flags == '&' && !e.b.दत्त )
        bitwise_and(e.a, e.s, dst);
    अन्यथा यदि( e.flags == '|' && e.b.दत्त )
        bitwise_or(e.a, e.b, dst);
    अन्यथा यदि( e.flags == '|' && !e.b.दत्त )
        bitwise_or(e.a, e.s, dst);
    अन्यथा यदि( e.flags == '^' && e.b.दत्त )
        bitwise_xor(e.a, e.b, dst);
    अन्यथा यदि( e.flags == '^' && !e.b.दत्त )
        bitwise_xor(e.a, e.s, dst);
    अन्यथा यदि( e.flags == '~' && !e.b.दत्त )
        bitwise_not(e.a, dst);
    अन्यथा यदि( e.flags == 'm' )
        cv::min(e.a, e.b, dst);
    अन्यथा यदि( e.flags == 'n' )
        cv::min(e.a, e.s[0], dst);
    अन्यथा यदि( e.flags == 'M' )
        cv::max(e.a, e.b, dst);
    अन्यथा यदि( e.flags == 'N' )
        cv::max(e.a, e.s[0], dst);
    अन्यथा यदि( e.flags == 'a' && e.b.दत्त )
        cv::absdiff(e.a, e.b, dst);
    अन्यथा यदि( e.flags == 'a' && !e.b.दत्त )
        cv::absdiff(e.a, e.s, dst);
    अन्यथा
        CV_Error(cv::Error::StsError, "Unknown operation");

    यदि( dst.दत्त != m.दत्त )
        dst.convert_गन्तव्यम्(m, _type);
}

शून्य MatOp_Bin::multiply(ध्रुव Mat_व्यञ्जकम्& e, द्विगुण s, Mat_व्यञ्जकम्& res) ध्रुव
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

शून्य MatOp_Bin::divide(द्विगुण s, ध्रुव Mat_व्यञ्जकम्& e, Mat_व्यञ्जकम्& res) ध्रुव
{
    CV_INSTRUMENT_REGION();

    यदि( e.flags == '/' && (!e.b.दत्त || e.beta == 0) )
        Mat_Op_सङ्कलय_Ex::make_व्यञ्जकम्(res, e.a, Mat(), s/e.alpha, 0);
    अन्यथा
        MatOp::divide(s, e, res);
}

अन्तःस्थ शून्य MatOp_Bin::make_व्यञ्जकम्(Mat_व्यञ्जकम्& res, वर्ण op, ध्रुव Mat& a, ध्रुव Mat& b, द्विगुण scale)
{
    res = Mat_व्यञ्जकम्(&g_MatOp_Bin, op, a, b, Mat(), scale, b.दत्त ? 1 : 0);
}

अन्तःस्थ शून्य MatOp_Bin::make_व्यञ्जकम्(Mat_व्यञ्जकम्& res, वर्ण op, ध्रुव Mat& a, ध्रुव Scalar& s)
{
    res = Mat_व्यञ्जकम्(&g_MatOp_Bin, op, a, Mat(), Mat(), 1, 0, s);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

शून्य MatOp_Cmp::assign(ध्रुव Mat_व्यञ्जकम्& e, Mat& m, पूर्णाङ्क _type) ध्रुव
{
    Mat अस्थायी_2, &dst = _type == -1 || _type == CV_8U ? m : अस्थायी_2;

    यदि( e.b.दत्त )
        cv::compare(e.a, e.b, dst, e.flags);
    अन्यथा
        cv::compare(e.a, e.alpha, dst, e.flags);

    यदि( dst.दत्त != m.दत्त )
        dst.convert_गन्तव्यम्(m, _type);
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
    Mat अस्थायी_2, &dst = _type == -1 || _type == e.a.type() ? m : अस्थायी_2;

    cv::transpose(e.a, dst);

    यदि( dst.दत्त != m.दत्त || e.alpha != 1 )
        dst.convert_गन्तव्यम्(m, _type, e.alpha);
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

    यदि( e.alpha == 1 )
        Mat_Op_तटस्थः::make_व्यञ्जकम्(res, e.a);
    अन्यथा
        Mat_Op_सङ्कलय_Ex::make_व्यञ्जकम्(res, e.a, Mat(), e.alpha, 0);
}

अन्तःस्थ शून्य MatOp_T::make_व्यञ्जकम्(Mat_व्यञ्जकम्& res, ध्रुव Mat& a, द्विगुण alpha)
{
    res = Mat_व्यञ्जकम्(&g_MatOp_T, 0, a, Mat(), Mat(), alpha, 0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

शून्य MatOp_GEMM::assign(ध्रुव Mat_व्यञ्जकम्& e, Mat& m, पूर्णाङ्क _type) ध्रुव
{
    Mat अस्थायी_2, &dst = _type == -1 || _type == e.a.type() ? m : अस्थायी_2;

    cv::gemm(e.a, e.b, e.alpha, e.c, e.beta, dst, e.flags);
    यदि( dst.दत्त != m.दत्त )
        dst.convert_गन्तव्यम्(m, _type);
}

शून्य MatOp_GEMM::सङ्कलय(ध्रुव Mat_व्यञ्जकम्& e1, ध्रुव Mat_व्यञ्जकम्& e2, Mat_व्यञ्जकम्& res) ध्रुव
{
    CV_INSTRUMENT_REGION();

    तार्किक i1 = is_तटस्थः(e1), i2 = is_तटस्थः(e2);
    द्विगुण alpha1 = i1 ? 1 : e1.alpha, alpha2 = i2 ? 1 : e2.alpha;

    यदि( isMatProd(e1) && (i2 || isScaled(e2) || isT(e2)) )
        MatOp_GEMM::make_व्यञ्जकम्(res, (e1.flags & ~cv::GEMM_3_T)|(isT(e2) ? cv::GEMM_3_T : 0),
                             e1.a, e1.b, alpha1, e2.a, alpha2);
    अन्यथा यदि( isMatProd(e2) && (i1 || isScaled(e1) || isT(e1)) )
        MatOp_GEMM::make_व्यञ्जकम्(res, (e2.flags & ~cv::GEMM_3_T)|(isT(e1) ? cv::GEMM_3_T : 0),
                             e2.a, e2.b, alpha2, e1.a, alpha1);
    अन्यथा यदि( एतत् == e2.op )
        MatOp::सङ्कलय(e1, e2, res);
    अन्यथा
        e2.op->सङ्कलय(e1, e2, res);
}

शून्य MatOp_GEMM::व्यवकलय(ध्रुव Mat_व्यञ्जकम्& e1, ध्रुव Mat_व्यञ्जकम्& e2, Mat_व्यञ्जकम्& res) ध्रुव
{
    CV_INSTRUMENT_REGION();

    तार्किक i1 = is_तटस्थः(e1), i2 = is_तटस्थः(e2);
    द्विगुण alpha1 = i1 ? 1 : e1.alpha, alpha2 = i2 ? 1 : e2.alpha;

    यदि( isMatProd(e1) && (i2 || isScaled(e2) || isT(e2)) )
        MatOp_GEMM::make_व्यञ्जकम्(res, (e1.flags & ~cv::GEMM_3_T)|(isT(e2) ? cv::GEMM_3_T : 0),
                             e1.a, e1.b, alpha1, e2.a, -alpha2);
    अन्यथा यदि( isMatProd(e2) && (i1 || isScaled(e1) || isT(e1)) )
        MatOp_GEMM::make_व्यञ्जकम्(res, (e2.flags & ~cv::GEMM_3_T)|(isT(e1) ? cv::GEMM_3_T : 0),
                            e2.a, e2.b, -alpha2, e1.a, alpha1);
    अन्यथा यदि( एतत् == e2.op )
        MatOp::व्यवकलय(e1, e2, res);
    अन्यथा
        e2.op->व्यवकलय(e1, e2, res);
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
    Mat अस्थायी_2, &dst = _type == -1 || _type == e.a.type() ? m : अस्थायी_2;

    cv::invert(e.a, dst, e.flags);
    यदि( dst.दत्त != m.दत्त )
        dst.convert_गन्तव्यम्(m, _type);
}

शून्य MatOp_Invert::matmul(ध्रुव Mat_व्यञ्जकम्& e1, ध्रुव Mat_व्यञ्जकम्& e2, Mat_व्यञ्जकम्& res) ध्रुव
{
    यदि( isInv(e1) && is_तटस्थः(e2) )
        MatOp_Solve::make_व्यञ्जकम्(res, e1.flags, e1.a, e2.a);
    अन्यथा यदि( एतत् == e2.op )
        MatOp::matmul(e1, e2, res);
    अन्यथा
        e2.op->matmul(e1, e2, res);
}

अन्तःस्थ शून्य MatOp_Invert::make_व्यञ्जकम्(Mat_व्यञ्जकम्& res, पूर्णाङ्क विधानम्, ध्रुव Mat& m)
{
    res = Mat_व्यञ्जकम्(&g_MatOp_Invert, विधानम्, m, Mat(), Mat(), 1, 0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

शून्य MatOp_Solve::assign(ध्रुव Mat_व्यञ्जकम्& e, Mat& m, पूर्णाङ्क _type) ध्रुव
{
    Mat अस्थायी_2, &dst = _type == -1 || _type == e.a.type() ? m : अस्थायी_2;

    cv::solve(e.a, e.b, dst, e.flags);
    यदि( dst.दत्त != m.दत्त )
        dst.convert_गन्तव्यम्(m, _type);
}

अन्तःस्थ शून्य MatOp_Solve::make_व्यञ्जकम्(Mat_व्यञ्जकम्& res, पूर्णाङ्क विधानम्, ध्रुव Mat& a, ध्रुव Mat& b)
{
    res = Mat_व्यञ्जकम्(&g_MatOp_Solve, विधानम्, a, b, Mat(), 1, 1);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

शून्य MatOp_Initializer::assign(ध्रुव Mat_व्यञ्जकम्& e, Mat& m, पूर्णाङ्क _type) ध्रुव
{
    यदि( _type == -1 )
        _type = e.a.type();

    m.create(e.a.size, _type);

    यदि( e.flags == 'I' && e.a.dims <= 2 )
        समुच्चयः_तटस्थः(m, Scalar(e.alpha));
    अन्यथा यदि( e.flags == '0' )
        m = Scalar();
    अन्यथा यदि( e.flags == '1' )
        m = Scalar(e.alpha);
    अन्यथा
        CV_Error(cv::Error::StsError, "Invalid matrix initializer type");
}

शून्य MatOp_Initializer::multiply(ध्रुव Mat_व्यञ्जकम्& e, द्विगुण s, Mat_व्यञ्जकम्& res) ध्रुव
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha *= s;
}

अन्तःस्थ शून्य MatOp_Initializer::make_व्यञ्जकम्(Mat_व्यञ्जकम्& res, पूर्णाङ्क विधानम्, आकार sz, पूर्णाङ्क type, द्विगुण alpha)
{
    res = Mat_व्यञ्जकम्(getGlobalMatOpInitializer(), विधानम्, Mat(sz, type, (शून्य*)(size_t)0xEEEEEEEE), Mat(), Mat(), alpha, 0);
}

अन्तःस्थ शून्य MatOp_Initializer::make_व्यञ्जकम्(Mat_व्यञ्जकम्& res, पूर्णाङ्क विधानम्, पूर्णाङ्क ndims, ध्रुव पूर्णाङ्क* sizes, पूर्णाङ्क type, द्विगुण alpha)
{
    res = Mat_व्यञ्जकम्(getGlobalMatOpInitializer(), विधानम्, Mat(ndims, sizes, type, (शून्य*)(size_t)0xEEEEEEEE), Mat(), Mat(), alpha, 0);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

Mat_व्यञ्जकम् Mat::t() ध्रुव
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(*एतत्);
    Mat_व्यञ्जकम् e;
    MatOp_T::make_व्यञ्जकम्(e, *एतत्);
    प्रत्यागम e;
}

Mat_व्यञ्जकम् Mat::inv(पूर्णाङ्क विधानम्) ध्रुव
{
    CV_INSTRUMENT_REGION();

    Mat_व्यञ्जकम् e;
    MatOp_Invert::make_व्यञ्जकम्(e, विधानम्, *एतत्);
    प्रत्यागम e;
}


Mat_व्यञ्जकम् Mat::mul(निवेशः_Array m, द्विगुण scale) ध्रुव
{
    CV_INSTRUMENT_REGION();

    Mat b = m.getMat();
    // Unless the argument is a refcounted Mat/UMat, the header returned by getMat() may be
    // a non-owning view of caller memory (e.g. a scalar bound to _InputArray(const double&),
    // a Matx or a Vec) that the returned MatExpr can outlive, so snapshot it.
    // See https://github.com/opencv/opencv/issues/23577
    यदि( !m.isMat() && !m.isUMat() )
        b = b.clone();

    Mat_व्यञ्जकम् e;
    MatOp_Bin::make_व्यञ्जकम्(e, '*', *एतत्, b, scale);
    प्रत्यागम e;
}

Mat_व्यञ्जकम् Mat::zeros(पूर्णाङ्क पङ्क्तिकाः, पूर्णाङ्क स्तम्भसङ्ख्या, पूर्णाङ्क type)
{
    CV_INSTRUMENT_REGION();

    Mat_व्यञ्जकम् e;
    MatOp_Initializer::make_व्यञ्जकम्(e, '0', आकार(स्तम्भसङ्ख्या, पङ्क्तिकाः), type);
    प्रत्यागम e;
}

Mat_व्यञ्जकम् Mat::zeros(आकार size, पूर्णाङ्क type)
{
    CV_INSTRUMENT_REGION();

    Mat_व्यञ्जकम् e;
    MatOp_Initializer::make_व्यञ्जकम्(e, '0', size, type);
    प्रत्यागम e;
}

Mat_व्यञ्जकम् Mat::zeros(पूर्णाङ्क ndims, ध्रुव पूर्णाङ्क* sizes, पूर्णाङ्क type)
{
    CV_INSTRUMENT_REGION();

    Mat_व्यञ्जकम् e;
    MatOp_Initializer::make_व्यञ्जकम्(e, '0', ndims, sizes, type);
    प्रत्यागम e;
}

Mat_व्यञ्जकम् Mat::zeros(ध्रुव MatShape& shape, पूर्णाङ्क type)
{
    CV_INSTRUMENT_REGION();

    Mat_व्यञ्जकम् e;
    MatOp_Initializer::make_व्यञ्जकम्(e, '0', shape.dims, shape.p, type);
    प्रत्यागम e;
}

Mat_व्यञ्जकम् Mat::ones(पूर्णाङ्क पङ्क्तिकाः, पूर्णाङ्क स्तम्भसङ्ख्या, पूर्णाङ्क type)
{
    CV_INSTRUMENT_REGION();

    Mat_व्यञ्जकम् e;
    MatOp_Initializer::make_व्यञ्जकम्(e, '1', आकार(स्तम्भसङ्ख्या, पङ्क्तिकाः), type);
    प्रत्यागम e;
}

Mat_व्यञ्जकम् Mat::ones(आकार size, पूर्णाङ्क type)
{
    CV_INSTRUMENT_REGION();

    Mat_व्यञ्जकम् e;
    MatOp_Initializer::make_व्यञ्जकम्(e, '1', size, type);
    प्रत्यागम e;
}

Mat_व्यञ्जकम् Mat::ones(पूर्णाङ्क ndims, ध्रुव पूर्णाङ्क* sizes, पूर्णाङ्क type)
{
    CV_INSTRUMENT_REGION();

    Mat_व्यञ्जकम् e;
    MatOp_Initializer::make_व्यञ्जकम्(e, '1', ndims, sizes, type);
    प्रत्यागम e;
}

Mat_व्यञ्जकम् Mat::ones(ध्रुव MatShape& shape, पूर्णाङ्क type)
{
    CV_INSTRUMENT_REGION();

    Mat_व्यञ्जकम् e;
    MatOp_Initializer::make_व्यञ्जकम्(e, '1', shape.dims, shape.p, type);
    प्रत्यागम e;
}

Mat_व्यञ्जकम् Mat::eye(पूर्णाङ्क पङ्क्तिकाः, पूर्णाङ्क स्तम्भसङ्ख्या, पूर्णाङ्क type)
{
    CV_INSTRUMENT_REGION();

    Mat_व्यञ्जकम् e;
    MatOp_Initializer::make_व्यञ्जकम्(e, 'I', आकार(स्तम्भसङ्ख्या, पङ्क्तिकाः), type);
    प्रत्यागम e;
}

Mat_व्यञ्जकम् Mat::eye(आकार size, पूर्णाङ्क type)
{
    CV_INSTRUMENT_REGION();

    Mat_व्यञ्जकम् e;
    MatOp_Initializer::make_व्यञ्जकम्(e, 'I', size, type);
    प्रत्यागम e;
}

शून्य Mat_व्यञ्जकम्::swap(Mat_व्यञ्जकम्& other)
{
    उपयुज्य std::swap;

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
    यदि (!is_तटस्थः(व्यञ्जकम्))
    {
        Mat परिणाम = व्यञ्जकम्;  // TODO improve through refcount == 1 of expr.a (inplace operation is possible - except gemm?)
        Mat_व्यञ्जकम् परिणाम_व्यञ्जकम्(परिणाम);
        swap(const_cast<Mat_व्यञ्जकम्&>(व्यञ्जकम्), परिणाम_व्यञ्जकम्);
    }
    CV_Assert(is_तटस्थः(व्यञ्जकम्));
    init(FIXED_TYPE + FIXED_आकार + MAT + ACCESS_READ, &व्यञ्जकम्.a);
}

} // cv::
