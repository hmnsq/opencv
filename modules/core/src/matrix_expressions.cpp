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


صنف Mat_Op_پچھان_تت CV_FINAL : عام MatOp
{
عام:
    Mat_Op_پچھان_تت() {}
    مجازی ~Mat_Op_پچھان_تت() {}

    منطقي elementWise(ثابت Mat_اظہاریہ& /*اظہاریہ*/) ثابت CV_OVERRIDE { أرجع سچ; }
    فراغ assign(ثابت Mat_اظہاریہ& اظہاریہ, Mat& m, صحيح type=-1) ثابت CV_OVERRIDE;

    ساكن فراغ make_اظہاریہ(Mat_اظہاریہ& res, ثابت Mat& m);
};

ساكن Mat_Op_پچھان_تت g_Mat_Op_پچھان_تت;

صنف Mat_Op_جوڑو_Ex CV_FINAL : عام MatOp
{
عام:
    Mat_Op_جوڑو_Ex() {}
    مجازی ~Mat_Op_جوڑو_Ex() {}

    منطقي elementWise(ثابت Mat_اظہاریہ& /*اظہاریہ*/) ثابت CV_OVERRIDE { أرجع سچ; }
    فراغ assign(ثابت Mat_اظہاریہ& اظہاریہ, Mat& m, صحيح type=-1) ثابت CV_OVERRIDE;

    فراغ جوڑو(ثابت Mat_اظہاریہ& e1, ثابت Scalar& s, Mat_اظہاریہ& res) ثابت CV_OVERRIDE;
    فراغ گھٹاؤ(ثابت Scalar& s, ثابت Mat_اظہاریہ& اظہاریہ, Mat_اظہاریہ& res) ثابت CV_OVERRIDE;
    فراغ multiply(ثابت Mat_اظہاریہ& e1, مزدوج s, Mat_اظہاریہ& res) ثابت CV_OVERRIDE;
    فراغ divide(مزدوج s, ثابت Mat_اظہاریہ& e, Mat_اظہاریہ& res) ثابت CV_OVERRIDE;

    فراغ transpose(ثابت Mat_اظہاریہ& e1, Mat_اظہاریہ& res) ثابت CV_OVERRIDE;
    فراغ مطلق_قدر(ثابت Mat_اظہاریہ& اظہاریہ, Mat_اظہاریہ& res) ثابت CV_OVERRIDE;

    ساكن فراغ make_اظہاریہ(Mat_اظہاریہ& res, ثابت Mat& a, ثابت Mat& b, مزدوج alpha, مزدوج beta, ثابت Scalar& s=Scalar());
};

ساكن Mat_Op_جوڑو_Ex g_Mat_Op_جوڑو_Ex;

صنف MatOp_Bin CV_FINAL : عام MatOp
{
عام:
    MatOp_Bin() {}
    مجازی ~MatOp_Bin() {}

    منطقي elementWise(ثابت Mat_اظہاریہ& /*اظہاریہ*/) ثابت CV_OVERRIDE { أرجع سچ; }
    فراغ assign(ثابت Mat_اظہاریہ& اظہاریہ, Mat& m, صحيح type=-1) ثابت CV_OVERRIDE;

    فراغ multiply(ثابت Mat_اظہاریہ& e1, مزدوج s, Mat_اظہاریہ& res) ثابت CV_OVERRIDE;
    فراغ divide(مزدوج s, ثابت Mat_اظہاریہ& e, Mat_اظہاریہ& res) ثابت CV_OVERRIDE;

    ساكن فراغ make_اظہاریہ(Mat_اظہاریہ& res, حرف op, ثابت Mat& a, ثابت Mat& b, مزدوج scale=1);
    ساكن فراغ make_اظہاریہ(Mat_اظہاریہ& res, حرف op, ثابت Mat& a, ثابت Scalar& s);
};

ساكن MatOp_Bin g_MatOp_Bin;

صنف MatOp_Cmp CV_FINAL : عام MatOp
{
عام:
    MatOp_Cmp() {}
    مجازی ~MatOp_Cmp() {}

    منطقي elementWise(ثابت Mat_اظہاریہ& /*اظہاریہ*/) ثابت CV_OVERRIDE { أرجع سچ; }
    فراغ assign(ثابت Mat_اظہاریہ& اظہاریہ, Mat& m, صحيح type=-1) ثابت CV_OVERRIDE;

    ساكن فراغ make_اظہاریہ(Mat_اظہاریہ& res, صحيح cmpop, ثابت Mat& a, ثابت Mat& b);
    ساكن فراغ make_اظہاریہ(Mat_اظہاریہ& res, صحيح cmpop, ثابت Mat& a, مزدوج alpha);
};

ساكن MatOp_Cmp g_MatOp_Cmp;

صنف MatOp_GEMM CV_FINAL : عام MatOp
{
عام:
    MatOp_GEMM() {}
    مجازی ~MatOp_GEMM() {}

    منطقي elementWise(ثابت Mat_اظہاریہ& /*اظہاریہ*/) ثابت CV_OVERRIDE { أرجع خطأ; }
    فراغ assign(ثابت Mat_اظہاریہ& اظہاریہ, Mat& m, صحيح type=-1) ثابت CV_OVERRIDE;

    فراغ جوڑو(ثابت Mat_اظہاریہ& e1, ثابت Mat_اظہاریہ& e2, Mat_اظہاریہ& res) ثابت CV_OVERRIDE;
    فراغ گھٹاؤ(ثابت Mat_اظہاریہ& e1, ثابت Mat_اظہاریہ& e2, Mat_اظہاریہ& res) ثابت CV_OVERRIDE;
    فراغ multiply(ثابت Mat_اظہاریہ& e, مزدوج s, Mat_اظہاریہ& res) ثابت CV_OVERRIDE;

    فراغ transpose(ثابت Mat_اظہاریہ& اظہاریہ, Mat_اظہاریہ& res) ثابت CV_OVERRIDE;

    ناپ size(ثابت Mat_اظہاریہ& اظہاریہ) ثابت CV_OVERRIDE
    {
        أرجع ناپ(
            (اظہاریہ.flags & GEMM_2_T) ? اظہاریہ.b.صفاں : اظہاریہ.b.خانے_گنتی,
            (اظہاریہ.flags & GEMM_1_T) ? اظہاریہ.a.خانے_گنتی : اظہاریہ.a.صفاں
        );
    }

    ساكن فراغ make_اظہاریہ(Mat_اظہاریہ& res, صحيح flags, ثابت Mat& a, ثابت Mat& b,
                         مزدوج alpha=1, ثابت Mat& c=Mat(), مزدوج beta=1);

};

ساكن MatOp_GEMM g_MatOp_GEMM;

صنف MatOp_Invert CV_FINAL : عام MatOp
{
عام:
    MatOp_Invert() {}
    مجازی ~MatOp_Invert() {}

    منطقي elementWise(ثابت Mat_اظہاریہ& /*اظہاریہ*/) ثابت CV_OVERRIDE { أرجع خطأ; }
    فراغ assign(ثابت Mat_اظہاریہ& اظہاریہ, Mat& m, صحيح type=-1) ثابت CV_OVERRIDE;

    فراغ matmul(ثابت Mat_اظہاریہ& expr1, ثابت Mat_اظہاریہ& expr2, Mat_اظہاریہ& res) ثابت CV_OVERRIDE;

    ساكن فراغ make_اظہاریہ(Mat_اظہاریہ& res, صحيح طریقہ, ثابت Mat& m);
};

ساكن MatOp_Invert g_MatOp_Invert;

صنف MatOp_T CV_FINAL : عام MatOp
{
عام:
    MatOp_T() {}
    مجازی ~MatOp_T() {}

    منطقي elementWise(ثابت Mat_اظہاریہ& /*اظہاریہ*/) ثابت CV_OVERRIDE { أرجع خطأ; }
    فراغ assign(ثابت Mat_اظہاریہ& اظہاریہ, Mat& m, صحيح type=-1) ثابت CV_OVERRIDE;

    فراغ multiply(ثابت Mat_اظہاریہ& e1, مزدوج s, Mat_اظہاریہ& res) ثابت CV_OVERRIDE;
    فراغ transpose(ثابت Mat_اظہاریہ& اظہاریہ, Mat_اظہاریہ& res) ثابت CV_OVERRIDE;

    ساكن فراغ make_اظہاریہ(Mat_اظہاریہ& res, ثابت Mat& a, مزدوج alpha=1);
};

ساكن MatOp_T g_MatOp_T;

صنف MatOp_Solve CV_FINAL : عام MatOp
{
عام:
    MatOp_Solve() {}
    مجازی ~MatOp_Solve() {}

    منطقي elementWise(ثابت Mat_اظہاریہ& /*اظہاریہ*/) ثابت CV_OVERRIDE { أرجع خطأ; }
    فراغ assign(ثابت Mat_اظہاریہ& اظہاریہ, Mat& m, صحيح type=-1) ثابت CV_OVERRIDE;

    ساكن فراغ make_اظہاریہ(Mat_اظہاریہ& res, صحيح طریقہ, ثابت Mat& a, ثابت Mat& b);
};

ساكن MatOp_Solve g_MatOp_Solve;

صنف MatOp_Initializer CV_FINAL : عام MatOp
{
عام:
    MatOp_Initializer() {}
    مجازی ~MatOp_Initializer() {}

    منطقي elementWise(ثابت Mat_اظہاریہ& /*اظہاریہ*/) ثابت CV_OVERRIDE { أرجع خطأ; }
    فراغ assign(ثابت Mat_اظہاریہ& اظہاریہ, Mat& m, صحيح type=-1) ثابت CV_OVERRIDE;

    فراغ multiply(ثابت Mat_اظہاریہ& e, مزدوج s, Mat_اظہاریہ& res) ثابت CV_OVERRIDE;

    ساكن فراغ make_اظہاریہ(Mat_اظہاریہ& res, صحيح طریقہ, ناپ sz, صحيح type, مزدوج alpha=1);
    ساكن فراغ make_اظہاریہ(Mat_اظہاریہ& res, صحيح طریقہ, صحيح ndims, ثابت صحيح* sizes, صحيح type, مزدوج alpha=1);
};

ساكن MatOp_Initializer* getGlobalMatOpInitializer()
{
    CV_SINGLETON_LAZY_INIT(MatOp_Initializer, جديد MatOp_Initializer())
}

ساكن مضمن منطقي is_پچھان_تت(ثابت Mat_اظہاریہ& e) { أرجع e.op == &g_Mat_Op_پچھان_تت; }
ساكن مضمن منطقي is_جوڑو_Ex(ثابت Mat_اظہاریہ& e) { أرجع e.op == &g_Mat_Op_جوڑو_Ex; }
ساكن مضمن منطقي isScaled(ثابت Mat_اظہاریہ& e) { أرجع is_جوڑو_Ex(e) && (!e.b.انکڑے || e.beta == 0) && e.s == Scalar(); }
ساكن مضمن منطقي isBin(ثابت Mat_اظہاریہ& e, حرف c) { أرجع e.op == &g_MatOp_Bin && e.flags == c; }
ساكن مضمن منطقي isCmp(ثابت Mat_اظہاریہ& e) { أرجع e.op == &g_MatOp_Cmp; }
ساكن مضمن منطقي isReciprocal(ثابت Mat_اظہاریہ& e) { أرجع isBin(e,'/') && (!e.b.انکڑے || e.beta == 0); }
ساكن مضمن منطقي isT(ثابت Mat_اظہاریہ& e) { أرجع e.op == &g_MatOp_T; }
ساكن مضمن منطقي isInv(ثابت Mat_اظہاریہ& e) { أرجع e.op == &g_MatOp_Invert; }
ساكن مضمن منطقي isSolve(ثابت Mat_اظہاریہ& e) { أرجع e.op == &g_MatOp_Solve; }
//static inline bool isGEMM(const MatExpr& e) { return e.op == &g_MatOp_GEMM; }
ساكن مضمن منطقي isMatProd(ثابت Mat_اظہاریہ& e) { أرجع e.op == &g_MatOp_GEMM && (!e.c.انکڑے || e.beta == 0); }
ساكن مضمن منطقي isInitializer(ثابت Mat_اظہاریہ& e) { أرجع e.op == getGlobalMatOpInitializer(); }

/////////////////////////////////////////////////////////////////////////////////////////////////////

MatOp::MatOp() {}
MatOp::~MatOp() {}


منطقي MatOp::elementWise(ثابت Mat_اظہاریہ& /*اظہاریہ*/) ثابت
{
    أرجع خطأ;
}

فراغ MatOp::roi(ثابت Mat_اظہاریہ& اظہاریہ, ثابت Range& صف_Range_2, ثابت Range& خانہ_Range_2, Mat_اظہاریہ& e) ثابت
{
    إذا( elementWise(اظہاریہ) )
    {
        e = Mat_اظہاریہ(اظہاریہ.op, اظہاریہ.flags, Mat(), Mat(), Mat(),
                    اظہاریہ.alpha, اظہاریہ.beta, اظہاریہ.s);
        إذا(اظہاریہ.a.انکڑے)
            e.a = اظہاریہ.a(صف_Range_2, خانہ_Range_2);
        إذا(اظہاریہ.b.انکڑے)
            e.b = اظہاریہ.b(صف_Range_2, خانہ_Range_2);
        إذا(اظہاریہ.c.انکڑے)
            e.c = اظہاریہ.c(صف_Range_2, خانہ_Range_2);
    }
    وإلا
    {
        Mat m;
        اظہاریہ.op->assign(اظہاریہ, m);
        e = Mat_اظہاریہ(&g_Mat_Op_پچھان_تت, 0, m(صف_Range_2, خانہ_Range_2), Mat(), Mat());
    }
}

فراغ MatOp::diag(ثابت Mat_اظہاریہ& اظہاریہ, صحيح d, Mat_اظہاریہ& e) ثابت
{
    إذا( elementWise(اظہاریہ) )
    {
        e = Mat_اظہاریہ(اظہاریہ.op, اظہاریہ.flags, Mat(), Mat(), Mat(),
                    اظہاریہ.alpha, اظہاریہ.beta, اظہاریہ.s);
        إذا(اظہاریہ.a.انکڑے)
            e.a = اظہاریہ.a.diag(d);
        إذا(اظہاریہ.b.انکڑے)
            e.b = اظہاریہ.b.diag(d);
        إذا(اظہاریہ.c.انکڑے)
            e.c = اظہاریہ.c.diag(d);
    }
    وإلا
    {
        Mat m;
        اظہاریہ.op->assign(اظہاریہ, m);
        e = Mat_اظہاریہ(&g_Mat_Op_پچھان_تت, 0, m.diag(d), Mat(), Mat());
    }
}


فراغ MatOp::aug_سونپو_جوڑو(ثابت Mat_اظہاریہ& اظہاریہ, Mat& m) ثابت
{
    Mat عارضی_2;
    اظہاریہ.op->assign(اظہاریہ, عارضی_2);
    m += عارضی_2;
}


فراغ MatOp::aug_سونپو_گھٹاؤ(ثابت Mat_اظہاریہ& اظہاریہ, Mat& m) ثابت
{
    Mat عارضی_2;
    اظہاریہ.op->assign(اظہاریہ, عارضی_2);
    m -= عارضی_2;
}


فراغ MatOp::aug_سونپو_Multiply(ثابت Mat_اظہاریہ& اظہاریہ, Mat& m) ثابت
{
    Mat عارضی_2;
    اظہاریہ.op->assign(اظہاریہ, عارضی_2);
    m *= عارضی_2;
}


فراغ MatOp::aug_سونپو_Divide(ثابت Mat_اظہاریہ& اظہاریہ, Mat& m) ثابت
{
    Mat عارضی_2;
    اظہاریہ.op->assign(اظہاریہ, عارضی_2);
    m /= عارضی_2;
}


فراغ MatOp::aug_سونپو_And(ثابت Mat_اظہاریہ& اظہاریہ, Mat& m) ثابت
{
    Mat عارضی_2;
    اظہاریہ.op->assign(اظہاریہ, عارضی_2);
    m &= عارضی_2;
}


فراغ MatOp::aug_سونپو_Or(ثابت Mat_اظہاریہ& اظہاریہ, Mat& m) ثابت
{
    Mat عارضی_2;
    اظہاریہ.op->assign(اظہاریہ, عارضی_2);
    m |= عارضی_2;
}


فراغ MatOp::aug_سونپو_Xor(ثابت Mat_اظہاریہ& اظہاریہ, Mat& m) ثابت
{
    Mat عارضی_2;
    اظہاریہ.op->assign(اظہاریہ, عارضی_2);
    m ^= عارضی_2;
}


فراغ MatOp::جوڑو(ثابت Mat_اظہاریہ& e1, ثابت Mat_اظہاریہ& e2, Mat_اظہاریہ& res) ثابت
{
    CV_INSTRUMENT_REGION();

    إذا( هذا == e2.op )
    {
        مزدوج alpha = 1, beta = 1;
        Scalar s;
        Mat m1, m2;
        إذا( is_جوڑو_Ex(e1) && (!e1.b.انکڑے || e1.beta == 0) )
        {
            m1 = e1.a;
            alpha = e1.alpha;
            s = e1.s;
        }
        وإلا
            e1.op->assign(e1, m1);

        إذا( is_جوڑو_Ex(e2) && (!e2.b.انکڑے || e2.beta == 0) )
        {
            m2 = e2.a;
            beta = e2.alpha;
            s += e2.s;
        }
        وإلا
            e2.op->assign(e2, m2);
        Mat_Op_جوڑو_Ex::make_اظہاریہ(res, m1, m2, alpha, beta, s);
    }
    وإلا
        e2.op->جوڑو(e1, e2, res);
}


فراغ MatOp::جوڑو(ثابت Mat_اظہاریہ& expr1, ثابت Scalar& s, Mat_اظہاریہ& res) ثابت
{
    CV_INSTRUMENT_REGION();

    Mat m1;
    expr1.op->assign(expr1, m1);
    Mat_Op_جوڑو_Ex::make_اظہاریہ(res, m1, Mat(), 1, 0, s);
}


فراغ MatOp::گھٹاؤ(ثابت Mat_اظہاریہ& e1, ثابت Mat_اظہاریہ& e2, Mat_اظہاریہ& res) ثابت
{
    CV_INSTRUMENT_REGION();

    إذا( هذا == e2.op )
    {
        مزدوج alpha = 1, beta = -1;
        Scalar s;
        Mat m1, m2;
        إذا( is_جوڑو_Ex(e1) && (!e1.b.انکڑے || e1.beta == 0) )
        {
            m1 = e1.a;
            alpha = e1.alpha;
            s = e1.s;
        }
        وإلا
            e1.op->assign(e1, m1);

        إذا( is_جوڑو_Ex(e2) && (!e2.b.انکڑے || e2.beta == 0) )
        {
            m2 = e2.a;
            beta = -e2.alpha;
            s -= e2.s;
        }
        وإلا
            e2.op->assign(e2, m2);
        Mat_Op_جوڑو_Ex::make_اظہاریہ(res, m1, m2, alpha, beta, s);
    }
    وإلا
        e2.op->گھٹاؤ(e1, e2, res);
}


فراغ MatOp::گھٹاؤ(ثابت Scalar& s, ثابت Mat_اظہاریہ& اظہاریہ, Mat_اظہاریہ& res) ثابت
{
    CV_INSTRUMENT_REGION();

    Mat m;
    اظہاریہ.op->assign(اظہاریہ, m);
    Mat_Op_جوڑو_Ex::make_اظہاریہ(res, m, Mat(), -1, 0, s);
}


فراغ MatOp::multiply(ثابت Mat_اظہاریہ& e1, ثابت Mat_اظہاریہ& e2, Mat_اظہاریہ& res, مزدوج scale) ثابت
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

            MatOp_Bin::make_اظہاریہ(res, '/', m2, e1.a, scale/e1.alpha);
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

            MatOp_Bin::make_اظہاریہ(res, op, m1, m2, scale);
        }
    }
    وإلا
        e2.op->multiply(e1, e2, res, scale);
}


فراغ MatOp::multiply(ثابت Mat_اظہاریہ& اظہاریہ, مزدوج s, Mat_اظہاریہ& res) ثابت
{
    CV_INSTRUMENT_REGION();

    Mat m;
    اظہاریہ.op->assign(اظہاریہ, m);
    Mat_Op_جوڑو_Ex::make_اظہاریہ(res, m, Mat(), s, 0);
}


فراغ MatOp::divide(ثابت Mat_اظہاریہ& e1, ثابت Mat_اظہاریہ& e2, Mat_اظہاریہ& res, مزدوج scale) ثابت
{
    CV_INSTRUMENT_REGION();

    إذا( هذا == e2.op )
    {
        إذا( isReciprocal(e1) && isReciprocal(e2) )
            MatOp_Bin::make_اظہاریہ(res, '/', e2.a, e1.a, e1.alpha/e2.alpha);
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
            MatOp_Bin::make_اظہاریہ(res, op, m1, m2, scale);
        }
    }
    وإلا
        e2.op->divide(e1, e2, res, scale);
}


فراغ MatOp::divide(مزدوج s, ثابت Mat_اظہاریہ& اظہاریہ, Mat_اظہاریہ& res) ثابت
{
    CV_INSTRUMENT_REGION();

    Mat m;
    اظہاریہ.op->assign(اظہاریہ, m);
    MatOp_Bin::make_اظہاریہ(res, '/', m, Mat(), s);
}


فراغ MatOp::مطلق_قدر(ثابت Mat_اظہاریہ& اظہاریہ, Mat_اظہاریہ& res) ثابت
{
    CV_INSTRUMENT_REGION();

    Mat m;
    اظہاریہ.op->assign(اظہاریہ, m);
    MatOp_Bin::make_اظہاریہ(res, 'a', m, Mat());
}


فراغ MatOp::transpose(ثابت Mat_اظہاریہ& اظہاریہ, Mat_اظہاریہ& res) ثابت
{
    CV_INSTRUMENT_REGION();

    Mat m;
    اظہاریہ.op->assign(اظہاریہ, m);
    MatOp_T::make_اظہاریہ(res, m, 1);
}


فراغ MatOp::matmul(ثابت Mat_اظہاریہ& e1, ثابت Mat_اظہاریہ& e2, Mat_اظہاریہ& res) ثابت
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

        MatOp_GEMM::make_اظہاریہ(res, flags, m1, m2, scale);
    }
    وإلا
        e2.op->matmul(e1, e2, res);
}


فراغ MatOp::invert(ثابت Mat_اظہاریہ& اظہاریہ, صحيح طریقہ, Mat_اظہاریہ& res) ثابت
{
    Mat m;
    اظہاریہ.op->assign(اظہاریہ, m);
    MatOp_Invert::make_اظہاریہ(res, طریقہ, m);
}


ناپ MatOp::size(ثابت Mat_اظہاریہ& اظہاریہ) ثابت
{
    أرجع !اظہاریہ.a.empty() ? اظہاریہ.a.size() : اظہاریہ.b.empty() ? اظہاریہ.b.size() : اظہاریہ.c.size();
}

صحيح MatOp::type(ثابت Mat_اظہاریہ& اظہاریہ) ثابت
{
    CV_INSTRUMENT_REGION();

    أرجع !اظہاریہ.a.empty() ? اظہاریہ.a.type() : اظہاریہ.b.empty() ? اظہاریہ.b.type() : اظہاریہ.c.type();
}

//////////////////////////////////////////////////////////////////////////////////////////////////

Mat_اظہاریہ::Mat_اظہاریہ(ثابت Mat& m) : op(&g_Mat_Op_پچھان_تت), flags(0), a(m), b(Mat()), c(Mat()), alpha(1), beta(0), s(Scalar())
{
}

Mat_اظہاریہ Mat_اظہاریہ::صف(صحيح y) ثابت
{
    Mat_اظہاریہ e;
    op->roi(*هذا, Range(y, y+1), Range::all(), e);
    أرجع e;
}

Mat_اظہاریہ Mat_اظہاریہ::خانہ(صحيح x) ثابت
{
    Mat_اظہاریہ e;
    op->roi(*هذا, Range::all(), Range(x, x+1), e);
    أرجع e;
}

Mat_اظہاریہ Mat_اظہاریہ::diag(صحيح d) ثابت
{
    Mat_اظہاریہ e;
    op->diag(*هذا, d, e);
    أرجع e;
}

Mat_اظہاریہ Mat_اظہاریہ::عامل()( ثابت Range& صف_Range_2, ثابت Range& خانہ_Range_2 ) ثابت
{
    Mat_اظہاریہ e;
    op->roi(*هذا, صف_Range_2, خانہ_Range_2, e);
    أرجع e;
}

Mat_اظہاریہ Mat_اظہاریہ::عامل()( ثابت Rect& roi ) ثابت
{
    Mat_اظہاریہ e;
    op->roi(*هذا, Range(roi.y, roi.y + roi.اچائی), Range(roi.x, roi.x + roi.چوڑائی), e);
    أرجع e;
}

Mat Mat_اظہاریہ::cross(ثابت Mat& m) ثابت
{
    أرجع ((Mat)*هذا).cross(m);
}

مزدوج Mat_اظہاریہ::dot(ثابت Mat& m) ثابت
{
    أرجع ((Mat)*هذا).dot(m);
}

Mat_اظہاریہ Mat_اظہاریہ::t() ثابت
{
    Mat_اظہاریہ e;
    op->transpose(*هذا, e);
    أرجع e;
}

Mat_اظہاریہ Mat_اظہاریہ::inv(صحيح طریقہ) ثابت
{
    Mat_اظہاریہ e;
    op->invert(*هذا, طریقہ, e);
    أرجع e;
}

Mat_اظہاریہ Mat_اظہاریہ::mul(ثابت Mat_اظہاریہ& e, مزدوج scale) ثابت
{
    Mat_اظہاریہ en;
    op->multiply(*هذا, e, en, scale);
    أرجع en;
}

Mat_اظہاریہ Mat_اظہاریہ::mul(ثابت Mat& m, مزدوج scale) ثابت
{
    Mat_اظہاریہ e;
    op->multiply(*هذا, Mat_اظہاریہ(m), e, scale);
    أرجع e;
}

Mat_اظہاریہ عامل + (ثابت Mat& a, ثابت Mat& b)
{
    checkOperandsExist(a, b);
    Mat_اظہاریہ e;
    Mat_Op_جوڑو_Ex::make_اظہاریہ(e, a, b, 1, 1);
    أرجع e;
}

Mat_اظہاریہ عامل + (ثابت Mat& a, ثابت Scalar& s)
{
    checkOperandsExist(a);
    Mat_اظہاریہ e;
    Mat_Op_جوڑو_Ex::make_اظہاریہ(e, a, Mat(), 1, 0, s);
    أرجع e;
}

Mat_اظہاریہ عامل + (ثابت Scalar& s, ثابت Mat& a)
{
    checkOperandsExist(a);
    Mat_اظہاریہ e;
    Mat_Op_جوڑو_Ex::make_اظہاریہ(e, a, Mat(), 1, 0, s);
    أرجع e;
}

Mat_اظہاریہ عامل + (ثابت Mat_اظہاریہ& e, ثابت Mat& m)
{
    Mat_اظہاریہ en;
    e.op->جوڑو(e, Mat_اظہاریہ(m), en);
    أرجع en;
}

Mat_اظہاریہ عامل + (ثابت Mat& m, ثابت Mat_اظہاریہ& e)
{
    checkOperandsExist(m);
    Mat_اظہاریہ en;
    e.op->جوڑو(e, Mat_اظہاریہ(m), en);
    أرجع en;
}

Mat_اظہاریہ عامل + (ثابت Mat_اظہاریہ& e, ثابت Scalar& s)
{
    Mat_اظہاریہ en;
    e.op->جوڑو(e, s, en);
    أرجع en;
}

Mat_اظہاریہ عامل + (ثابت Scalar& s, ثابت Mat_اظہاریہ& e)
{
    Mat_اظہاریہ en;
    e.op->جوڑو(e, s, en);
    أرجع en;
}

Mat_اظہاریہ عامل + (ثابت Mat_اظہاریہ& e1, ثابت Mat_اظہاریہ& e2)
{
    Mat_اظہاریہ en;
    e1.op->جوڑو(e1, e2, en);
    أرجع en;
}

Mat_اظہاریہ عامل - (ثابت Mat& a, ثابت Mat& b)
{
    checkOperandsExist(a, b);
    Mat_اظہاریہ e;
    Mat_Op_جوڑو_Ex::make_اظہاریہ(e, a, b, 1, -1);
    أرجع e;
}

Mat_اظہاریہ عامل - (ثابت Mat& a, ثابت Scalar& s)
{
    checkOperandsExist(a);
    Mat_اظہاریہ e;
    Mat_Op_جوڑو_Ex::make_اظہاریہ(e, a, Mat(), 1, 0, -s);
    أرجع e;
}

Mat_اظہاریہ عامل - (ثابت Scalar& s, ثابت Mat& a)
{
    checkOperandsExist(a);
    Mat_اظہاریہ e;
    Mat_Op_جوڑو_Ex::make_اظہاریہ(e, a, Mat(), -1, 0, s);
    أرجع e;
}

Mat_اظہاریہ عامل - (ثابت Mat_اظہاریہ& e, ثابت Mat& m)
{
    checkOperandsExist(m);
    Mat_اظہاریہ en;
    e.op->گھٹاؤ(e, Mat_اظہاریہ(m), en);
    أرجع en;
}

Mat_اظہاریہ عامل - (ثابت Mat& m, ثابت Mat_اظہاریہ& e)
{
    checkOperandsExist(m);
    Mat_اظہاریہ en;
    e.op->گھٹاؤ(Mat_اظہاریہ(m), e, en);
    أرجع en;
}

Mat_اظہاریہ عامل - (ثابت Mat_اظہاریہ& e, ثابت Scalar& s)
{
    Mat_اظہاریہ en;
    e.op->جوڑو(e, -s, en);
    أرجع en;
}

Mat_اظہاریہ عامل - (ثابت Scalar& s, ثابت Mat_اظہاریہ& e)
{
    Mat_اظہاریہ en;
    e.op->گھٹاؤ(s, e, en);
    أرجع en;
}

Mat_اظہاریہ عامل - (ثابت Mat_اظہاریہ& e1, ثابت Mat_اظہاریہ& e2)
{
    Mat_اظہاریہ en;
    e1.op->گھٹاؤ(e1, e2, en);
    أرجع en;
}

Mat_اظہاریہ عامل - (ثابت Mat& m)
{
    checkOperandsExist(m);
    Mat_اظہاریہ e;
    Mat_Op_جوڑو_Ex::make_اظہاریہ(e, m, Mat(), -1, 0);
    أرجع e;
}

Mat_اظہاریہ عامل - (ثابت Mat_اظہاریہ& e)
{
    Mat_اظہاریہ en;
    e.op->گھٹاؤ(Scalar(0), e, en);
    أرجع en;
}

Mat_اظہاریہ عامل * (ثابت Mat& a, ثابت Mat& b)
{
    checkOperandsExist(a, b);
    Mat_اظہاریہ e;
    MatOp_GEMM::make_اظہاریہ(e, 0, a, b);
    أرجع e;
}

Mat_اظہاریہ عامل * (ثابت Mat& a, مزدوج s)
{
    checkOperandsExist(a);
    Mat_اظہاریہ e;
    Mat_Op_جوڑو_Ex::make_اظہاریہ(e, a, Mat(), s, 0);
    أرجع e;
}

Mat_اظہاریہ عامل * (مزدوج s, ثابت Mat& a)
{
    checkOperandsExist(a);
    Mat_اظہاریہ e;
    Mat_Op_جوڑو_Ex::make_اظہاریہ(e, a, Mat(), s, 0);
    أرجع e;
}

Mat_اظہاریہ عامل * (ثابت Mat_اظہاریہ& e, ثابت Mat& m)
{
    Mat_اظہاریہ en;
    e.op->matmul(e, Mat_اظہاریہ(m), en);
    أرجع en;
}

Mat_اظہاریہ عامل * (ثابت Mat& m, ثابت Mat_اظہاریہ& e)
{
    checkOperandsExist(m);
    Mat_اظہاریہ en;
    e.op->matmul(Mat_اظہاریہ(m), e, en);
    أرجع en;
}

Mat_اظہاریہ عامل * (ثابت Mat_اظہاریہ& e, مزدوج s)
{
    Mat_اظہاریہ en;
    e.op->multiply(e, s, en);
    أرجع en;
}

Mat_اظہاریہ عامل * (مزدوج s, ثابت Mat_اظہاریہ& e)
{
    Mat_اظہاریہ en;
    e.op->multiply(e, s, en);
    أرجع en;
}

Mat_اظہاریہ عامل * (ثابت Mat_اظہاریہ& e1, ثابت Mat_اظہاریہ& e2)
{
    Mat_اظہاریہ en;
    e1.op->matmul(e1, e2, en);
    أرجع en;
}

Mat_اظہاریہ عامل / (ثابت Mat& a, ثابت Mat& b)
{
    checkOperandsExist(a, b);
    Mat_اظہاریہ e;
    MatOp_Bin::make_اظہاریہ(e, '/', a, b);
    أرجع e;
}

Mat_اظہاریہ عامل / (ثابت Mat& a, مزدوج s)
{
    checkOperandsExist(a);
    Mat_اظہاریہ e;
    Mat_Op_جوڑو_Ex::make_اظہاریہ(e, a, Mat(), 1./s, 0);
    أرجع e;
}

Mat_اظہاریہ عامل / (مزدوج s, ثابت Mat& a)
{
    checkOperandsExist(a);
    Mat_اظہاریہ e;
    MatOp_Bin::make_اظہاریہ(e, '/', a, Mat(), s);
    أرجع e;
}

Mat_اظہاریہ عامل / (ثابت Mat_اظہاریہ& e, ثابت Mat& m)
{
    Mat_اظہاریہ en;
    e.op->divide(e, Mat_اظہاریہ(m), en);
    أرجع en;
}

Mat_اظہاریہ عامل / (ثابت Mat& m, ثابت Mat_اظہاریہ& e)
{
    checkOperandsExist(m);
    Mat_اظہاریہ en;
    e.op->divide(Mat_اظہاریہ(m), e, en);
    أرجع en;
}

Mat_اظہاریہ عامل / (ثابت Mat_اظہاریہ& e, مزدوج s)
{
    Mat_اظہاریہ en;
    e.op->multiply(e, 1./s, en);
    أرجع en;
}

Mat_اظہاریہ عامل / (مزدوج s, ثابت Mat_اظہاریہ& e)
{
    Mat_اظہاریہ en;
    e.op->divide(s, e, en);
    أرجع en;
}

Mat_اظہاریہ عامل / (ثابت Mat_اظہاریہ& e1, ثابت Mat_اظہاریہ& e2)
{
    Mat_اظہاریہ en;
    e1.op->divide(e1, e2, en);
    أرجع en;
}

Mat_اظہاریہ عامل < (ثابت Mat& a, ثابت Mat& b)
{
    checkOperandsExist(a, b);
    Mat_اظہاریہ e;
    MatOp_Cmp::make_اظہاریہ(e, cv::CMP_LT, a, b);
    أرجع e;
}

Mat_اظہاریہ عامل < (ثابت Mat& a, مزدوج s)
{
    checkOperandsExist(a);
    Mat_اظہاریہ e;
    MatOp_Cmp::make_اظہاریہ(e, cv::CMP_LT, a, s);
    أرجع e;
}

Mat_اظہاریہ عامل < (مزدوج s, ثابت Mat& a)
{
    checkOperandsExist(a);
    Mat_اظہاریہ e;
    MatOp_Cmp::make_اظہاریہ(e, cv::CMP_GT, a, s);
    أرجع e;
}

Mat_اظہاریہ عامل <= (ثابت Mat& a, ثابت Mat& b)
{
    checkOperandsExist(a, b);
    Mat_اظہاریہ e;
    MatOp_Cmp::make_اظہاریہ(e, cv::CMP_LE, a, b);
    أرجع e;
}

Mat_اظہاریہ عامل <= (ثابت Mat& a, مزدوج s)
{
    checkOperandsExist(a);
    Mat_اظہاریہ e;
    MatOp_Cmp::make_اظہاریہ(e, cv::CMP_LE, a, s);
    أرجع e;
}

Mat_اظہاریہ عامل <= (مزدوج s, ثابت Mat& a)
{
    checkOperandsExist(a);
    Mat_اظہاریہ e;
    MatOp_Cmp::make_اظہاریہ(e, cv::CMP_GE, a, s);
    أرجع e;
}

Mat_اظہاریہ عامل == (ثابت Mat& a, ثابت Mat& b)
{
    checkOperandsExist(a, b);
    Mat_اظہاریہ e;
    MatOp_Cmp::make_اظہاریہ(e, cv::CMP_EQ, a, b);
    أرجع e;
}

Mat_اظہاریہ عامل == (ثابت Mat& a, مزدوج s)
{
    checkOperandsExist(a);
    Mat_اظہاریہ e;
    MatOp_Cmp::make_اظہاریہ(e, cv::CMP_EQ, a, s);
    أرجع e;
}

Mat_اظہاریہ عامل == (مزدوج s, ثابت Mat& a)
{
    checkOperandsExist(a);
    Mat_اظہاریہ e;
    MatOp_Cmp::make_اظہاریہ(e, cv::CMP_EQ, a, s);
    أرجع e;
}

Mat_اظہاریہ عامل != (ثابت Mat& a, ثابت Mat& b)
{
    checkOperandsExist(a, b);
    Mat_اظہاریہ e;
    MatOp_Cmp::make_اظہاریہ(e, cv::CMP_NE, a, b);
    أرجع e;
}

Mat_اظہاریہ عامل != (ثابت Mat& a, مزدوج s)
{
    checkOperandsExist(a);
    Mat_اظہاریہ e;
    MatOp_Cmp::make_اظہاریہ(e, cv::CMP_NE, a, s);
    أرجع e;
}

Mat_اظہاریہ عامل != (مزدوج s, ثابت Mat& a)
{
    checkOperandsExist(a);
    Mat_اظہاریہ e;
    MatOp_Cmp::make_اظہاریہ(e, cv::CMP_NE, a, s);
    أرجع e;
}

Mat_اظہاریہ عامل >= (ثابت Mat& a, ثابت Mat& b)
{
    checkOperandsExist(a, b);
    Mat_اظہاریہ e;
    MatOp_Cmp::make_اظہاریہ(e, cv::CMP_GE, a, b);
    أرجع e;
}

Mat_اظہاریہ عامل >= (ثابت Mat& a, مزدوج s)
{
    checkOperandsExist(a);
    Mat_اظہاریہ e;
    MatOp_Cmp::make_اظہاریہ(e, cv::CMP_GE, a, s);
    أرجع e;
}

Mat_اظہاریہ عامل >= (مزدوج s, ثابت Mat& a)
{
    checkOperandsExist(a);
    Mat_اظہاریہ e;
    MatOp_Cmp::make_اظہاریہ(e, cv::CMP_LE, a, s);
    أرجع e;
}

Mat_اظہاریہ عامل > (ثابت Mat& a, ثابت Mat& b)
{
    checkOperandsExist(a, b);
    Mat_اظہاریہ e;
    MatOp_Cmp::make_اظہاریہ(e, cv::CMP_GT, a, b);
    أرجع e;
}

Mat_اظہاریہ عامل > (ثابت Mat& a, مزدوج s)
{
    checkOperandsExist(a);
    Mat_اظہاریہ e;
    MatOp_Cmp::make_اظہاریہ(e, cv::CMP_GT, a, s);
    أرجع e;
}

Mat_اظہاریہ عامل > (مزدوج s, ثابت Mat& a)
{
    checkOperandsExist(a);
    Mat_اظہاریہ e;
    MatOp_Cmp::make_اظہاریہ(e, cv::CMP_LT, a, s);
    أرجع e;
}

Mat_اظہاریہ min(ثابت Mat& a, ثابت Mat& b)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a, b);
    Mat_اظہاریہ e;
    MatOp_Bin::make_اظہاریہ(e, 'm', a, b);
    أرجع e;
}

Mat_اظہاریہ min(ثابت Mat& a, مزدوج s)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    Mat_اظہاریہ e;
    MatOp_Bin::make_اظہاریہ(e, 'n', a, s);
    أرجع e;
}

Mat_اظہاریہ min(مزدوج s, ثابت Mat& a)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    Mat_اظہاریہ e;
    MatOp_Bin::make_اظہاریہ(e, 'n', a, s);
    أرجع e;
}

Mat_اظہاریہ max(ثابت Mat& a, ثابت Mat& b)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a, b);
    Mat_اظہاریہ e;
    MatOp_Bin::make_اظہاریہ(e, 'M', a, b);
    أرجع e;
}

Mat_اظہاریہ max(ثابت Mat& a, مزدوج s)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    Mat_اظہاریہ e;
    MatOp_Bin::make_اظہاریہ(e, 'N', a, s);
    أرجع e;
}

Mat_اظہاریہ max(مزدوج s, ثابت Mat& a)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    Mat_اظہاریہ e;
    MatOp_Bin::make_اظہاریہ(e, 'N', a, s);
    أرجع e;
}

Mat_اظہاریہ عامل & (ثابت Mat& a, ثابت Mat& b)
{
    checkOperandsExist(a, b);
    Mat_اظہاریہ e;
    MatOp_Bin::make_اظہاریہ(e, '&', a, b);
    أرجع e;
}

Mat_اظہاریہ عامل & (ثابت Mat& a, ثابت Scalar& s)
{
    checkOperandsExist(a);
    Mat_اظہاریہ e;
    MatOp_Bin::make_اظہاریہ(e, '&', a, s);
    أرجع e;
}

Mat_اظہاریہ عامل & (ثابت Scalar& s, ثابت Mat& a)
{
    checkOperandsExist(a);
    Mat_اظہاریہ e;
    MatOp_Bin::make_اظہاریہ(e, '&', a, s);
    أرجع e;
}

Mat_اظہاریہ عامل | (ثابت Mat& a, ثابت Mat& b)
{
    checkOperandsExist(a, b);
    Mat_اظہاریہ e;
    MatOp_Bin::make_اظہاریہ(e, '|', a, b);
    أرجع e;
}

Mat_اظہاریہ عامل | (ثابت Mat& a, ثابت Scalar& s)
{
    checkOperandsExist(a);
    Mat_اظہاریہ e;
    MatOp_Bin::make_اظہاریہ(e, '|', a, s);
    أرجع e;
}

Mat_اظہاریہ عامل | (ثابت Scalar& s, ثابت Mat& a)
{
    checkOperandsExist(a);
    Mat_اظہاریہ e;
    MatOp_Bin::make_اظہاریہ(e, '|', a, s);
    أرجع e;
}

Mat_اظہاریہ عامل ^ (ثابت Mat& a, ثابت Mat& b)
{
    checkOperandsExist(a, b);
    Mat_اظہاریہ e;
    MatOp_Bin::make_اظہاریہ(e, '^', a, b);
    أرجع e;
}

Mat_اظہاریہ عامل ^ (ثابت Mat& a, ثابت Scalar& s)
{
    checkOperandsExist(a);
    Mat_اظہاریہ e;
    MatOp_Bin::make_اظہاریہ(e, '^', a, s);
    أرجع e;
}

Mat_اظہاریہ عامل ^ (ثابت Scalar& s, ثابت Mat& a)
{
    checkOperandsExist(a);
    Mat_اظہاریہ e;
    MatOp_Bin::make_اظہاریہ(e, '^', a, s);
    أرجع e;
}

Mat_اظہاریہ عامل ~(ثابت Mat& a)
{
    checkOperandsExist(a);
    Mat_اظہاریہ e;
    MatOp_Bin::make_اظہاریہ(e, '~', a, Scalar());
    أرجع e;
}

Mat_اظہاریہ مطلق_قدر(ثابت Mat& a)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    Mat_اظہاریہ e;
    MatOp_Bin::make_اظہاریہ(e, 'a', a, Scalar());
    أرجع e;
}

Mat_اظہاریہ مطلق_قدر(ثابت Mat_اظہاریہ& e)
{
    CV_INSTRUMENT_REGION();

    Mat_اظہاریہ en;
    e.op->مطلق_قدر(e, en);
    أرجع en;
}


ناپ Mat_اظہاریہ::size() ثابت
{
    إذا( isT(*هذا) || isInv(*هذا) )
        أرجع ناپ(a.صفاں, a.خانے_گنتی);
    إذا( isSolve(*هذا) )
        أرجع ناپ(b.خانے_گنتی, a.خانے_گنتی);
    إذا( isInitializer(*هذا) )
        أرجع a.size();
    أرجع op ? op->size(*هذا) : ناپ();
}


صحيح Mat_اظہاریہ::type() ثابت
{
    CV_INSTRUMENT_REGION();

    إذا( isInitializer(*هذا) )
        أرجع a.type();
    إذا( isCmp(*هذا) )
        أرجع CV_MAKETYPE(CV_8U, a.channels());
    أرجع op ? op->type(*هذا) : -1;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////

فراغ Mat_Op_پچھان_تت::assign(ثابت Mat_اظہاریہ& e, Mat& m, صحيح _type) ثابت
{
    إذا( _type == -1 || _type == e.a.type() )
        m = e.a;
    وإلا
    {
        CV_Assert( CV_MAT_CN(_type) == e.a.channels() );
        e.a.convert_منزل(m, _type);
    }
}

مضمن فراغ Mat_Op_پچھان_تت::make_اظہاریہ(Mat_اظہاریہ& res, ثابت Mat& m)
{
    res = Mat_اظہاریہ(&g_Mat_Op_پچھان_تت, 0, m, Mat(), Mat(), 1, 0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

فراغ Mat_Op_جوڑو_Ex::assign(ثابت Mat_اظہاریہ& e, Mat& m, صحيح _type) ثابت
{
    Mat عارضی_2, &dst = _type == -1 || e.a.type() == _type ? m : عارضی_2;
    إذا( e.b.انکڑے )
    {
        إذا( e.s == Scalar() || !e.s.isReal() )
        {
            إذا( e.alpha == 1 )
            {
                إذا( e.beta == 1 )
                    cv::جوڑو(e.a, e.b, dst);
                وإلا إذا( e.beta == -1 )
                    cv::گھٹاؤ(e.a, e.b, dst);
                وإلا
                    cv::scale_جوڑو(e.b, e.beta, e.a, dst);
            }
            وإلا إذا( e.beta == 1 )
            {
                إذا( e.alpha == -1 )
                    cv::گھٹاؤ(e.b, e.a, dst);
                وإلا
                    cv::scale_جوڑو(e.a, e.alpha, e.b, dst);
            }
            وإلا
                cv::جوڑو_Weighted(e.a, e.alpha, e.b, e.beta, 0, dst);

            إذا( !e.s.isReal() )
                cv::جوڑو(dst, e.s, dst);
        }
        وإلا
        {
            إذا (e.a.channels() > 1)
                CV_LOG_ONCE_WARNING(عدم, "OpenCV/MatExpr: processing of multi-channel arrays might be changed in the future: "
                                          "https://github.com/opencv/opencv/issues/16739");
            cv::جوڑو_Weighted(e.a, e.alpha, e.b, e.beta, e.s[0], dst);
        }
    }
    وإلا إذا( e.s.isReal() && (dst.انکڑے != m.انکڑے || fabs(e.alpha) != 1))
    {
        إذا (e.a.channels() > 1 && e.s[0] != 0.0)
            CV_LOG_ONCE_WARNING(عدم, "OpenCV/MatExpr: processing of multi-channel arrays might be changed in the future: "
                                      "https://github.com/opencv/opencv/issues/16739");
        e.a.convert_منزل(m, _type, e.alpha, e.s[0]);
        أرجع;
    }
    وإلا إذا( e.alpha == 1 )
        cv::جوڑو(e.a, e.s, dst);
    وإلا إذا( e.alpha == -1 )
        cv::گھٹاؤ(e.s, e.a, dst);
    وإلا
    {
        e.a.convert_منزل(dst, e.a.type(), e.alpha);
        cv::جوڑو(dst, e.s, dst);
    }

    إذا( dst.انکڑے != m.انکڑے )
        dst.convert_منزل(m, m.type());
}


فراغ Mat_Op_جوڑو_Ex::جوڑو(ثابت Mat_اظہاریہ& e, ثابت Scalar& s, Mat_اظہاریہ& res) ثابت
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.s += s;
}


فراغ Mat_Op_جوڑو_Ex::گھٹاؤ(ثابت Scalar& s, ثابت Mat_اظہاریہ& e, Mat_اظہاریہ& res) ثابت
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha = -res.alpha;
    res.beta = -res.beta;
    res.s = s - res.s;
}

فراغ Mat_Op_جوڑو_Ex::multiply(ثابت Mat_اظہاریہ& e, مزدوج s, Mat_اظہاریہ& res) ثابت
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha *= s;
    res.beta *= s;
    res.s *= s;
}

فراغ Mat_Op_جوڑو_Ex::divide(مزدوج s, ثابت Mat_اظہاریہ& e, Mat_اظہاریہ& res) ثابت
{
    CV_INSTRUMENT_REGION();

    إذا( isScaled(e) )
        MatOp_Bin::make_اظہاریہ(res, '/', e.a, Mat(), s/e.alpha);
    وإلا
        MatOp::divide(s, e, res);
}


فراغ Mat_Op_جوڑو_Ex::transpose(ثابت Mat_اظہاریہ& e, Mat_اظہاریہ& res) ثابت
{
    CV_INSTRUMENT_REGION();

    إذا( isScaled(e) )
        MatOp_T::make_اظہاریہ(res, e.a, e.alpha);
    وإلا
        MatOp::transpose(e, res);
}

فراغ Mat_Op_جوڑو_Ex::مطلق_قدر(ثابت Mat_اظہاریہ& e, Mat_اظہاریہ& res) ثابت
{
    CV_INSTRUMENT_REGION();

    إذا( (!e.b.انکڑے || e.beta == 0) && fabs(e.alpha) == 1 )
        MatOp_Bin::make_اظہاریہ(res, 'a', e.a, -e.s*e.alpha);
    وإلا إذا( e.b.انکڑے && e.alpha + e.beta == 0 && e.alpha*e.beta == -1 )
        MatOp_Bin::make_اظہاریہ(res, 'a', e.a, e.b);
    وإلا
        MatOp::مطلق_قدر(e, res);
}

مضمن فراغ Mat_Op_جوڑو_Ex::make_اظہاریہ(Mat_اظہاریہ& res, ثابت Mat& a, ثابت Mat& b, مزدوج alpha, مزدوج beta, ثابت Scalar& s)
{
    res = Mat_اظہاریہ(&g_Mat_Op_جوڑو_Ex, 0, a, b, Mat(), alpha, beta, s);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

فراغ MatOp_Bin::assign(ثابت Mat_اظہاریہ& e, Mat& m, صحيح _type) ثابت
{
    Mat عارضی_2, &dst = _type == -1 || e.a.type() == _type ? m : عارضی_2;

    إذا( e.flags == '*' )
        cv::multiply(e.a, e.b, dst, e.alpha);
    وإلا إذا( e.flags == '/' && e.b.انکڑے )
        cv::divide(e.a, e.b, dst, e.alpha);
    وإلا إذا( e.flags == '/' && !e.b.انکڑے )
        cv::divide(e.alpha, e.a, dst );
    وإلا إذا( e.flags == '&' && e.b.انکڑے )
        bitwise_and(e.a, e.b, dst);
    وإلا إذا( e.flags == '&' && !e.b.انکڑے )
        bitwise_and(e.a, e.s, dst);
    وإلا إذا( e.flags == '|' && e.b.انکڑے )
        bitwise_or(e.a, e.b, dst);
    وإلا إذا( e.flags == '|' && !e.b.انکڑے )
        bitwise_or(e.a, e.s, dst);
    وإلا إذا( e.flags == '^' && e.b.انکڑے )
        bitwise_xor(e.a, e.b, dst);
    وإلا إذا( e.flags == '^' && !e.b.انکڑے )
        bitwise_xor(e.a, e.s, dst);
    وإلا إذا( e.flags == '~' && !e.b.انکڑے )
        bitwise_not(e.a, dst);
    وإلا إذا( e.flags == 'm' )
        cv::min(e.a, e.b, dst);
    وإلا إذا( e.flags == 'n' )
        cv::min(e.a, e.s[0], dst);
    وإلا إذا( e.flags == 'M' )
        cv::max(e.a, e.b, dst);
    وإلا إذا( e.flags == 'N' )
        cv::max(e.a, e.s[0], dst);
    وإلا إذا( e.flags == 'a' && e.b.انکڑے )
        cv::absdiff(e.a, e.b, dst);
    وإلا إذا( e.flags == 'a' && !e.b.انکڑے )
        cv::absdiff(e.a, e.s, dst);
    وإلا
        CV_Error(cv::Error::StsError, "Unknown operation");

    إذا( dst.انکڑے != m.انکڑے )
        dst.convert_منزل(m, _type);
}

فراغ MatOp_Bin::multiply(ثابت Mat_اظہاریہ& e, مزدوج s, Mat_اظہاریہ& res) ثابت
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

فراغ MatOp_Bin::divide(مزدوج s, ثابت Mat_اظہاریہ& e, Mat_اظہاریہ& res) ثابت
{
    CV_INSTRUMENT_REGION();

    إذا( e.flags == '/' && (!e.b.انکڑے || e.beta == 0) )
        Mat_Op_جوڑو_Ex::make_اظہاریہ(res, e.a, Mat(), s/e.alpha, 0);
    وإلا
        MatOp::divide(s, e, res);
}

مضمن فراغ MatOp_Bin::make_اظہاریہ(Mat_اظہاریہ& res, حرف op, ثابت Mat& a, ثابت Mat& b, مزدوج scale)
{
    res = Mat_اظہاریہ(&g_MatOp_Bin, op, a, b, Mat(), scale, b.انکڑے ? 1 : 0);
}

مضمن فراغ MatOp_Bin::make_اظہاریہ(Mat_اظہاریہ& res, حرف op, ثابت Mat& a, ثابت Scalar& s)
{
    res = Mat_اظہاریہ(&g_MatOp_Bin, op, a, Mat(), Mat(), 1, 0, s);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

فراغ MatOp_Cmp::assign(ثابت Mat_اظہاریہ& e, Mat& m, صحيح _type) ثابت
{
    Mat عارضی_2, &dst = _type == -1 || _type == CV_8U ? m : عارضی_2;

    إذا( e.b.انکڑے )
        cv::compare(e.a, e.b, dst, e.flags);
    وإلا
        cv::compare(e.a, e.alpha, dst, e.flags);

    إذا( dst.انکڑے != m.انکڑے )
        dst.convert_منزل(m, _type);
}

مضمن فراغ MatOp_Cmp::make_اظہاریہ(Mat_اظہاریہ& res, صحيح cmpop, ثابت Mat& a, ثابت Mat& b)
{
    res = Mat_اظہاریہ(&g_MatOp_Cmp, cmpop, a, b, Mat(), 1, 1);
}

مضمن فراغ MatOp_Cmp::make_اظہاریہ(Mat_اظہاریہ& res, صحيح cmpop, ثابت Mat& a, مزدوج alpha)
{
    res = Mat_اظہاریہ(&g_MatOp_Cmp, cmpop, a, Mat(), Mat(), alpha, 1);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

فراغ MatOp_T::assign(ثابت Mat_اظہاریہ& e, Mat& m, صحيح _type) ثابت
{
    Mat عارضی_2, &dst = _type == -1 || _type == e.a.type() ? m : عارضی_2;

    cv::transpose(e.a, dst);

    إذا( dst.انکڑے != m.انکڑے || e.alpha != 1 )
        dst.convert_منزل(m, _type, e.alpha);
}

فراغ MatOp_T::multiply(ثابت Mat_اظہاریہ& e, مزدوج s, Mat_اظہاریہ& res) ثابت
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha *= s;
}

فراغ MatOp_T::transpose(ثابت Mat_اظہاریہ& e, Mat_اظہاریہ& res) ثابت
{
    CV_INSTRUMENT_REGION();

    إذا( e.alpha == 1 )
        Mat_Op_پچھان_تت::make_اظہاریہ(res, e.a);
    وإلا
        Mat_Op_جوڑو_Ex::make_اظہاریہ(res, e.a, Mat(), e.alpha, 0);
}

مضمن فراغ MatOp_T::make_اظہاریہ(Mat_اظہاریہ& res, ثابت Mat& a, مزدوج alpha)
{
    res = Mat_اظہاریہ(&g_MatOp_T, 0, a, Mat(), Mat(), alpha, 0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

فراغ MatOp_GEMM::assign(ثابت Mat_اظہاریہ& e, Mat& m, صحيح _type) ثابت
{
    Mat عارضی_2, &dst = _type == -1 || _type == e.a.type() ? m : عارضی_2;

    cv::gemm(e.a, e.b, e.alpha, e.c, e.beta, dst, e.flags);
    إذا( dst.انکڑے != m.انکڑے )
        dst.convert_منزل(m, _type);
}

فراغ MatOp_GEMM::جوڑو(ثابت Mat_اظہاریہ& e1, ثابت Mat_اظہاریہ& e2, Mat_اظہاریہ& res) ثابت
{
    CV_INSTRUMENT_REGION();

    منطقي i1 = is_پچھان_تت(e1), i2 = is_پچھان_تت(e2);
    مزدوج alpha1 = i1 ? 1 : e1.alpha, alpha2 = i2 ? 1 : e2.alpha;

    إذا( isMatProd(e1) && (i2 || isScaled(e2) || isT(e2)) )
        MatOp_GEMM::make_اظہاریہ(res, (e1.flags & ~cv::GEMM_3_T)|(isT(e2) ? cv::GEMM_3_T : 0),
                             e1.a, e1.b, alpha1, e2.a, alpha2);
    وإلا إذا( isMatProd(e2) && (i1 || isScaled(e1) || isT(e1)) )
        MatOp_GEMM::make_اظہاریہ(res, (e2.flags & ~cv::GEMM_3_T)|(isT(e1) ? cv::GEMM_3_T : 0),
                             e2.a, e2.b, alpha2, e1.a, alpha1);
    وإلا إذا( هذا == e2.op )
        MatOp::جوڑو(e1, e2, res);
    وإلا
        e2.op->جوڑو(e1, e2, res);
}

فراغ MatOp_GEMM::گھٹاؤ(ثابت Mat_اظہاریہ& e1, ثابت Mat_اظہاریہ& e2, Mat_اظہاریہ& res) ثابت
{
    CV_INSTRUMENT_REGION();

    منطقي i1 = is_پچھان_تت(e1), i2 = is_پچھان_تت(e2);
    مزدوج alpha1 = i1 ? 1 : e1.alpha, alpha2 = i2 ? 1 : e2.alpha;

    إذا( isMatProd(e1) && (i2 || isScaled(e2) || isT(e2)) )
        MatOp_GEMM::make_اظہاریہ(res, (e1.flags & ~cv::GEMM_3_T)|(isT(e2) ? cv::GEMM_3_T : 0),
                             e1.a, e1.b, alpha1, e2.a, -alpha2);
    وإلا إذا( isMatProd(e2) && (i1 || isScaled(e1) || isT(e1)) )
        MatOp_GEMM::make_اظہاریہ(res, (e2.flags & ~cv::GEMM_3_T)|(isT(e1) ? cv::GEMM_3_T : 0),
                            e2.a, e2.b, -alpha2, e1.a, alpha1);
    وإلا إذا( هذا == e2.op )
        MatOp::گھٹاؤ(e1, e2, res);
    وإلا
        e2.op->گھٹاؤ(e1, e2, res);
}

فراغ MatOp_GEMM::multiply(ثابت Mat_اظہاریہ& e, مزدوج s, Mat_اظہاریہ& res) ثابت
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha *= s;
    res.beta *= s;
}

فراغ MatOp_GEMM::transpose(ثابت Mat_اظہاریہ& e, Mat_اظہاریہ& res) ثابت
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.flags = (!(e.flags & cv::GEMM_1_T) ? cv::GEMM_2_T : 0) |
                (!(e.flags & cv::GEMM_2_T) ? cv::GEMM_1_T : 0) |
                (!(e.flags & cv::GEMM_3_T) ? cv::GEMM_3_T : 0);
    swap(res.a, res.b);
}

مضمن فراغ MatOp_GEMM::make_اظہاریہ(Mat_اظہاریہ& res, صحيح flags, ثابت Mat& a, ثابت Mat& b,
                                 مزدوج alpha, ثابت Mat& c, مزدوج beta)
{
    res = Mat_اظہاریہ(&g_MatOp_GEMM, flags, a, b, c, alpha, beta);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

فراغ MatOp_Invert::assign(ثابت Mat_اظہاریہ& e, Mat& m, صحيح _type) ثابت
{
    Mat عارضی_2, &dst = _type == -1 || _type == e.a.type() ? m : عارضی_2;

    cv::invert(e.a, dst, e.flags);
    إذا( dst.انکڑے != m.انکڑے )
        dst.convert_منزل(m, _type);
}

فراغ MatOp_Invert::matmul(ثابت Mat_اظہاریہ& e1, ثابت Mat_اظہاریہ& e2, Mat_اظہاریہ& res) ثابت
{
    إذا( isInv(e1) && is_پچھان_تت(e2) )
        MatOp_Solve::make_اظہاریہ(res, e1.flags, e1.a, e2.a);
    وإلا إذا( هذا == e2.op )
        MatOp::matmul(e1, e2, res);
    وإلا
        e2.op->matmul(e1, e2, res);
}

مضمن فراغ MatOp_Invert::make_اظہاریہ(Mat_اظہاریہ& res, صحيح طریقہ, ثابت Mat& m)
{
    res = Mat_اظہاریہ(&g_MatOp_Invert, طریقہ, m, Mat(), Mat(), 1, 0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

فراغ MatOp_Solve::assign(ثابت Mat_اظہاریہ& e, Mat& m, صحيح _type) ثابت
{
    Mat عارضی_2, &dst = _type == -1 || _type == e.a.type() ? m : عارضی_2;

    cv::solve(e.a, e.b, dst, e.flags);
    إذا( dst.انکڑے != m.انکڑے )
        dst.convert_منزل(m, _type);
}

مضمن فراغ MatOp_Solve::make_اظہاریہ(Mat_اظہاریہ& res, صحيح طریقہ, ثابت Mat& a, ثابت Mat& b)
{
    res = Mat_اظہاریہ(&g_MatOp_Solve, طریقہ, a, b, Mat(), 1, 1);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

فراغ MatOp_Initializer::assign(ثابت Mat_اظہاریہ& e, Mat& m, صحيح _type) ثابت
{
    إذا( _type == -1 )
        _type = e.a.type();

    m.create(e.a.size, _type);

    إذا( e.flags == 'I' && e.a.dims <= 2 )
        سیٹ_پچھان_تت(m, Scalar(e.alpha));
    وإلا إذا( e.flags == '0' )
        m = Scalar();
    وإلا إذا( e.flags == '1' )
        m = Scalar(e.alpha);
    وإلا
        CV_Error(cv::Error::StsError, "Invalid matrix initializer type");
}

فراغ MatOp_Initializer::multiply(ثابت Mat_اظہاریہ& e, مزدوج s, Mat_اظہاریہ& res) ثابت
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha *= s;
}

مضمن فراغ MatOp_Initializer::make_اظہاریہ(Mat_اظہاریہ& res, صحيح طریقہ, ناپ sz, صحيح type, مزدوج alpha)
{
    res = Mat_اظہاریہ(getGlobalMatOpInitializer(), طریقہ, Mat(sz, type, (فراغ*)(size_t)0xEEEEEEEE), Mat(), Mat(), alpha, 0);
}

مضمن فراغ MatOp_Initializer::make_اظہاریہ(Mat_اظہاریہ& res, صحيح طریقہ, صحيح ndims, ثابت صحيح* sizes, صحيح type, مزدوج alpha)
{
    res = Mat_اظہاریہ(getGlobalMatOpInitializer(), طریقہ, Mat(ndims, sizes, type, (فراغ*)(size_t)0xEEEEEEEE), Mat(), Mat(), alpha, 0);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

Mat_اظہاریہ Mat::t() ثابت
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(*هذا);
    Mat_اظہاریہ e;
    MatOp_T::make_اظہاریہ(e, *هذا);
    أرجع e;
}

Mat_اظہاریہ Mat::inv(صحيح طریقہ) ثابت
{
    CV_INSTRUMENT_REGION();

    Mat_اظہاریہ e;
    MatOp_Invert::make_اظہاریہ(e, طریقہ, *هذا);
    أرجع e;
}


Mat_اظہاریہ Mat::mul(داخلہ_Array m, مزدوج scale) ثابت
{
    CV_INSTRUMENT_REGION();

    Mat b = m.getMat();
    // Unless the argument is a refcounted Mat/UMat, the header returned by getMat() may be
    // a non-owning view of caller memory (e.g. a scalar bound to _InputArray(const double&),
    // a Matx or a Vec) that the returned MatExpr can outlive, so snapshot it.
    // See https://github.com/opencv/opencv/issues/23577
    إذا( !m.isMat() && !m.isUMat() )
        b = b.clone();

    Mat_اظہاریہ e;
    MatOp_Bin::make_اظہاریہ(e, '*', *هذا, b, scale);
    أرجع e;
}

Mat_اظہاریہ Mat::zeros(صحيح صفاں, صحيح خانے_گنتی, صحيح type)
{
    CV_INSTRUMENT_REGION();

    Mat_اظہاریہ e;
    MatOp_Initializer::make_اظہاریہ(e, '0', ناپ(خانے_گنتی, صفاں), type);
    أرجع e;
}

Mat_اظہاریہ Mat::zeros(ناپ size, صحيح type)
{
    CV_INSTRUMENT_REGION();

    Mat_اظہاریہ e;
    MatOp_Initializer::make_اظہاریہ(e, '0', size, type);
    أرجع e;
}

Mat_اظہاریہ Mat::zeros(صحيح ndims, ثابت صحيح* sizes, صحيح type)
{
    CV_INSTRUMENT_REGION();

    Mat_اظہاریہ e;
    MatOp_Initializer::make_اظہاریہ(e, '0', ndims, sizes, type);
    أرجع e;
}

Mat_اظہاریہ Mat::zeros(ثابت MatShape& shape, صحيح type)
{
    CV_INSTRUMENT_REGION();

    Mat_اظہاریہ e;
    MatOp_Initializer::make_اظہاریہ(e, '0', shape.dims, shape.p, type);
    أرجع e;
}

Mat_اظہاریہ Mat::ones(صحيح صفاں, صحيح خانے_گنتی, صحيح type)
{
    CV_INSTRUMENT_REGION();

    Mat_اظہاریہ e;
    MatOp_Initializer::make_اظہاریہ(e, '1', ناپ(خانے_گنتی, صفاں), type);
    أرجع e;
}

Mat_اظہاریہ Mat::ones(ناپ size, صحيح type)
{
    CV_INSTRUMENT_REGION();

    Mat_اظہاریہ e;
    MatOp_Initializer::make_اظہاریہ(e, '1', size, type);
    أرجع e;
}

Mat_اظہاریہ Mat::ones(صحيح ndims, ثابت صحيح* sizes, صحيح type)
{
    CV_INSTRUMENT_REGION();

    Mat_اظہاریہ e;
    MatOp_Initializer::make_اظہاریہ(e, '1', ndims, sizes, type);
    أرجع e;
}

Mat_اظہاریہ Mat::ones(ثابت MatShape& shape, صحيح type)
{
    CV_INSTRUMENT_REGION();

    Mat_اظہاریہ e;
    MatOp_Initializer::make_اظہاریہ(e, '1', shape.dims, shape.p, type);
    أرجع e;
}

Mat_اظہاریہ Mat::eye(صحيح صفاں, صحيح خانے_گنتی, صحيح type)
{
    CV_INSTRUMENT_REGION();

    Mat_اظہاریہ e;
    MatOp_Initializer::make_اظہاریہ(e, 'I', ناپ(خانے_گنتی, صفاں), type);
    أرجع e;
}

Mat_اظہاریہ Mat::eye(ناپ size, صحيح type)
{
    CV_INSTRUMENT_REGION();

    Mat_اظہاریہ e;
    MatOp_Initializer::make_اظہاریہ(e, 'I', size, type);
    أرجع e;
}

فراغ Mat_اظہاریہ::swap(Mat_اظہاریہ& other)
{
    ورتوں std::swap;

    swap(op, other.op);
    swap(flags, other.flags);

    swap(a, other.a);
    swap(b, other.b);
    swap(c, other.c);

    swap(alpha, other.alpha);
    swap(beta, other.beta);

    swap(s, other.s);
}

داخلہ_Array_2::داخلہ_Array_2(ثابت Mat_اظہاریہ& اظہاریہ)
{
    إذا (!is_پچھان_تت(اظہاریہ))
    {
        Mat نتیجہ = اظہاریہ;  // TODO improve through refcount == 1 of expr.a (inplace operation is possible - except gemm?)
        Mat_اظہاریہ نتیجہ_اظہاریہ(نتیجہ);
        swap(const_cast<Mat_اظہاریہ&>(اظہاریہ), نتیجہ_اظہاریہ);
    }
    CV_Assert(is_پچھان_تت(اظہاریہ));
    init(FIXED_TYPE + FIXED_ناپ + MAT + ACCESS_READ, &اظہاریہ.a);
}

} // cv::
