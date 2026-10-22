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


صنف Mat_Op_سڃاڻپ_عنصر CV_FINAL : عام MatOp
{
عام:
    Mat_Op_سڃاڻپ_عنصر() {}
    مجازي ~Mat_Op_سڃاڻپ_عنصر() {}

    منطقي elementWise(ثابت Mat_اظهار& /*اظهار*/) ثابت CV_OVERRIDE { أرجع سچ; }
    فراغ assign(ثابت Mat_اظهار& اظهار, Mat& m, صحيح type=-1) ثابت CV_OVERRIDE;

    ساكن فراغ make_اظهار(Mat_اظهار& res, ثابت Mat& m);
};

ساكن Mat_Op_سڃاڻپ_عنصر g_Mat_Op_سڃاڻپ_عنصر;

صنف Mat_Op_جوڙيو_Ex CV_FINAL : عام MatOp
{
عام:
    Mat_Op_جوڙيو_Ex() {}
    مجازي ~Mat_Op_جوڙيو_Ex() {}

    منطقي elementWise(ثابت Mat_اظهار& /*اظهار*/) ثابت CV_OVERRIDE { أرجع سچ; }
    فراغ assign(ثابت Mat_اظهار& اظهار, Mat& m, صحيح type=-1) ثابت CV_OVERRIDE;

    فراغ جوڙيو(ثابت Mat_اظهار& e1, ثابت Scalar& s, Mat_اظهار& res) ثابت CV_OVERRIDE;
    فراغ ڪٽيو(ثابت Scalar& s, ثابت Mat_اظهار& اظهار, Mat_اظهار& res) ثابت CV_OVERRIDE;
    فراغ multiply(ثابت Mat_اظهار& e1, مزدوج s, Mat_اظهار& res) ثابت CV_OVERRIDE;
    فراغ divide(مزدوج s, ثابت Mat_اظهار& e, Mat_اظهار& res) ثابت CV_OVERRIDE;

    فراغ transpose(ثابت Mat_اظهار& e1, Mat_اظهار& res) ثابت CV_OVERRIDE;
    فراغ مطلق_قدر(ثابت Mat_اظهار& اظهار, Mat_اظهار& res) ثابت CV_OVERRIDE;

    ساكن فراغ make_اظهار(Mat_اظهار& res, ثابت Mat& a, ثابت Mat& b, مزدوج alpha, مزدوج beta, ثابت Scalar& s=Scalar());
};

ساكن Mat_Op_جوڙيو_Ex g_Mat_Op_جوڙيو_Ex;

صنف MatOp_Bin CV_FINAL : عام MatOp
{
عام:
    MatOp_Bin() {}
    مجازي ~MatOp_Bin() {}

    منطقي elementWise(ثابت Mat_اظهار& /*اظهار*/) ثابت CV_OVERRIDE { أرجع سچ; }
    فراغ assign(ثابت Mat_اظهار& اظهار, Mat& m, صحيح type=-1) ثابت CV_OVERRIDE;

    فراغ multiply(ثابت Mat_اظهار& e1, مزدوج s, Mat_اظهار& res) ثابت CV_OVERRIDE;
    فراغ divide(مزدوج s, ثابت Mat_اظهار& e, Mat_اظهار& res) ثابت CV_OVERRIDE;

    ساكن فراغ make_اظهار(Mat_اظهار& res, حرف op, ثابت Mat& a, ثابت Mat& b, مزدوج scale=1);
    ساكن فراغ make_اظهار(Mat_اظهار& res, حرف op, ثابت Mat& a, ثابت Scalar& s);
};

ساكن MatOp_Bin g_MatOp_Bin;

صنف MatOp_Cmp CV_FINAL : عام MatOp
{
عام:
    MatOp_Cmp() {}
    مجازي ~MatOp_Cmp() {}

    منطقي elementWise(ثابت Mat_اظهار& /*اظهار*/) ثابت CV_OVERRIDE { أرجع سچ; }
    فراغ assign(ثابت Mat_اظهار& اظهار, Mat& m, صحيح type=-1) ثابت CV_OVERRIDE;

    ساكن فراغ make_اظهار(Mat_اظهار& res, صحيح cmpop, ثابت Mat& a, ثابت Mat& b);
    ساكن فراغ make_اظهار(Mat_اظهار& res, صحيح cmpop, ثابت Mat& a, مزدوج alpha);
};

ساكن MatOp_Cmp g_MatOp_Cmp;

صنف MatOp_GEMM CV_FINAL : عام MatOp
{
عام:
    MatOp_GEMM() {}
    مجازي ~MatOp_GEMM() {}

    منطقي elementWise(ثابت Mat_اظهار& /*اظهار*/) ثابت CV_OVERRIDE { أرجع خطأ; }
    فراغ assign(ثابت Mat_اظهار& اظهار, Mat& m, صحيح type=-1) ثابت CV_OVERRIDE;

    فراغ جوڙيو(ثابت Mat_اظهار& e1, ثابت Mat_اظهار& e2, Mat_اظهار& res) ثابت CV_OVERRIDE;
    فراغ ڪٽيو(ثابت Mat_اظهار& e1, ثابت Mat_اظهار& e2, Mat_اظهار& res) ثابت CV_OVERRIDE;
    فراغ multiply(ثابت Mat_اظهار& e, مزدوج s, Mat_اظهار& res) ثابت CV_OVERRIDE;

    فراغ transpose(ثابت Mat_اظهار& اظهار, Mat_اظهار& res) ثابت CV_OVERRIDE;

    سائيز size(ثابت Mat_اظهار& اظهار) ثابت CV_OVERRIDE
    {
        أرجع سائيز(
            (اظهار.flags & GEMM_2_T) ? اظهار.b.صفون : اظهار.b.کانن_ڳڻپ,
            (اظهار.flags & GEMM_1_T) ? اظهار.a.کانن_ڳڻپ : اظهار.a.صفون
        );
    }

    ساكن فراغ make_اظهار(Mat_اظهار& res, صحيح flags, ثابت Mat& a, ثابت Mat& b,
                         مزدوج alpha=1, ثابت Mat& c=Mat(), مزدوج beta=1);

};

ساكن MatOp_GEMM g_MatOp_GEMM;

صنف MatOp_Invert CV_FINAL : عام MatOp
{
عام:
    MatOp_Invert() {}
    مجازي ~MatOp_Invert() {}

    منطقي elementWise(ثابت Mat_اظهار& /*اظهار*/) ثابت CV_OVERRIDE { أرجع خطأ; }
    فراغ assign(ثابت Mat_اظهار& اظهار, Mat& m, صحيح type=-1) ثابت CV_OVERRIDE;

    فراغ matmul(ثابت Mat_اظهار& expr1, ثابت Mat_اظهار& expr2, Mat_اظهار& res) ثابت CV_OVERRIDE;

    ساكن فراغ make_اظهار(Mat_اظهار& res, صحيح طريقو, ثابت Mat& m);
};

ساكن MatOp_Invert g_MatOp_Invert;

صنف MatOp_T CV_FINAL : عام MatOp
{
عام:
    MatOp_T() {}
    مجازي ~MatOp_T() {}

    منطقي elementWise(ثابت Mat_اظهار& /*اظهار*/) ثابت CV_OVERRIDE { أرجع خطأ; }
    فراغ assign(ثابت Mat_اظهار& اظهار, Mat& m, صحيح type=-1) ثابت CV_OVERRIDE;

    فراغ multiply(ثابت Mat_اظهار& e1, مزدوج s, Mat_اظهار& res) ثابت CV_OVERRIDE;
    فراغ transpose(ثابت Mat_اظهار& اظهار, Mat_اظهار& res) ثابت CV_OVERRIDE;

    ساكن فراغ make_اظهار(Mat_اظهار& res, ثابت Mat& a, مزدوج alpha=1);
};

ساكن MatOp_T g_MatOp_T;

صنف MatOp_Solve CV_FINAL : عام MatOp
{
عام:
    MatOp_Solve() {}
    مجازي ~MatOp_Solve() {}

    منطقي elementWise(ثابت Mat_اظهار& /*اظهار*/) ثابت CV_OVERRIDE { أرجع خطأ; }
    فراغ assign(ثابت Mat_اظهار& اظهار, Mat& m, صحيح type=-1) ثابت CV_OVERRIDE;

    ساكن فراغ make_اظهار(Mat_اظهار& res, صحيح طريقو, ثابت Mat& a, ثابت Mat& b);
};

ساكن MatOp_Solve g_MatOp_Solve;

صنف MatOp_Initializer CV_FINAL : عام MatOp
{
عام:
    MatOp_Initializer() {}
    مجازي ~MatOp_Initializer() {}

    منطقي elementWise(ثابت Mat_اظهار& /*اظهار*/) ثابت CV_OVERRIDE { أرجع خطأ; }
    فراغ assign(ثابت Mat_اظهار& اظهار, Mat& m, صحيح type=-1) ثابت CV_OVERRIDE;

    فراغ multiply(ثابت Mat_اظهار& e, مزدوج s, Mat_اظهار& res) ثابت CV_OVERRIDE;

    ساكن فراغ make_اظهار(Mat_اظهار& res, صحيح طريقو, سائيز sz, صحيح type, مزدوج alpha=1);
    ساكن فراغ make_اظهار(Mat_اظهار& res, صحيح طريقو, صحيح ndims, ثابت صحيح* sizes, صحيح type, مزدوج alpha=1);
};

ساكن MatOp_Initializer* getGlobalMatOpInitializer()
{
    CV_SINGLETON_LAZY_INIT(MatOp_Initializer, جديد MatOp_Initializer())
}

ساكن مضمن منطقي is_سڃاڻپ_عنصر(ثابت Mat_اظهار& e) { أرجع e.op == &g_Mat_Op_سڃاڻپ_عنصر; }
ساكن مضمن منطقي is_جوڙيو_Ex(ثابت Mat_اظهار& e) { أرجع e.op == &g_Mat_Op_جوڙيو_Ex; }
ساكن مضمن منطقي isScaled(ثابت Mat_اظهار& e) { أرجع is_جوڙيو_Ex(e) && (!e.b.معلومات || e.beta == 0) && e.s == Scalar(); }
ساكن مضمن منطقي isBin(ثابت Mat_اظهار& e, حرف c) { أرجع e.op == &g_MatOp_Bin && e.flags == c; }
ساكن مضمن منطقي isCmp(ثابت Mat_اظهار& e) { أرجع e.op == &g_MatOp_Cmp; }
ساكن مضمن منطقي isReciprocal(ثابت Mat_اظهار& e) { أرجع isBin(e,'/') && (!e.b.معلومات || e.beta == 0); }
ساكن مضمن منطقي isT(ثابت Mat_اظهار& e) { أرجع e.op == &g_MatOp_T; }
ساكن مضمن منطقي isInv(ثابت Mat_اظهار& e) { أرجع e.op == &g_MatOp_Invert; }
ساكن مضمن منطقي isSolve(ثابت Mat_اظهار& e) { أرجع e.op == &g_MatOp_Solve; }
//static inline bool isGEMM(const MatExpr& e) { return e.op == &g_MatOp_GEMM; }
ساكن مضمن منطقي isMatProd(ثابت Mat_اظهار& e) { أرجع e.op == &g_MatOp_GEMM && (!e.c.معلومات || e.beta == 0); }
ساكن مضمن منطقي isInitializer(ثابت Mat_اظهار& e) { أرجع e.op == getGlobalMatOpInitializer(); }

/////////////////////////////////////////////////////////////////////////////////////////////////////

MatOp::MatOp() {}
MatOp::~MatOp() {}


منطقي MatOp::elementWise(ثابت Mat_اظهار& /*اظهار*/) ثابت
{
    أرجع خطأ;
}

فراغ MatOp::roi(ثابت Mat_اظهار& اظهار, ثابت Range& صف_Range_2, ثابت Range& کانو_Range_2, Mat_اظهار& e) ثابت
{
    إذا( elementWise(اظهار) )
    {
        e = Mat_اظهار(اظهار.op, اظهار.flags, Mat(), Mat(), Mat(),
                    اظهار.alpha, اظهار.beta, اظهار.s);
        إذا(اظهار.a.معلومات)
            e.a = اظهار.a(صف_Range_2, کانو_Range_2);
        إذا(اظهار.b.معلومات)
            e.b = اظهار.b(صف_Range_2, کانو_Range_2);
        إذا(اظهار.c.معلومات)
            e.c = اظهار.c(صف_Range_2, کانو_Range_2);
    }
    وإلا
    {
        Mat m;
        اظهار.op->assign(اظهار, m);
        e = Mat_اظهار(&g_Mat_Op_سڃاڻپ_عنصر, 0, m(صف_Range_2, کانو_Range_2), Mat(), Mat());
    }
}

فراغ MatOp::diag(ثابت Mat_اظهار& اظهار, صحيح d, Mat_اظهار& e) ثابت
{
    إذا( elementWise(اظهار) )
    {
        e = Mat_اظهار(اظهار.op, اظهار.flags, Mat(), Mat(), Mat(),
                    اظهار.alpha, اظهار.beta, اظهار.s);
        إذا(اظهار.a.معلومات)
            e.a = اظهار.a.diag(d);
        إذا(اظهار.b.معلومات)
            e.b = اظهار.b.diag(d);
        إذا(اظهار.c.معلومات)
            e.c = اظهار.c.diag(d);
    }
    وإلا
    {
        Mat m;
        اظهار.op->assign(اظهار, m);
        e = Mat_اظهار(&g_Mat_Op_سڃاڻپ_عنصر, 0, m.diag(d), Mat(), Mat());
    }
}


فراغ MatOp::aug_سونپيو_جوڙيو(ثابت Mat_اظهار& اظهار, Mat& m) ثابت
{
    Mat عارضي_2;
    اظهار.op->assign(اظهار, عارضي_2);
    m += عارضي_2;
}


فراغ MatOp::aug_سونپيو_ڪٽيو(ثابت Mat_اظهار& اظهار, Mat& m) ثابت
{
    Mat عارضي_2;
    اظهار.op->assign(اظهار, عارضي_2);
    m -= عارضي_2;
}


فراغ MatOp::aug_سونپيو_Multiply(ثابت Mat_اظهار& اظهار, Mat& m) ثابت
{
    Mat عارضي_2;
    اظهار.op->assign(اظهار, عارضي_2);
    m *= عارضي_2;
}


فراغ MatOp::aug_سونپيو_Divide(ثابت Mat_اظهار& اظهار, Mat& m) ثابت
{
    Mat عارضي_2;
    اظهار.op->assign(اظهار, عارضي_2);
    m /= عارضي_2;
}


فراغ MatOp::aug_سونپيو_And(ثابت Mat_اظهار& اظهار, Mat& m) ثابت
{
    Mat عارضي_2;
    اظهار.op->assign(اظهار, عارضي_2);
    m &= عارضي_2;
}


فراغ MatOp::aug_سونپيو_Or(ثابت Mat_اظهار& اظهار, Mat& m) ثابت
{
    Mat عارضي_2;
    اظهار.op->assign(اظهار, عارضي_2);
    m |= عارضي_2;
}


فراغ MatOp::aug_سونپيو_Xor(ثابت Mat_اظهار& اظهار, Mat& m) ثابت
{
    Mat عارضي_2;
    اظهار.op->assign(اظهار, عارضي_2);
    m ^= عارضي_2;
}


فراغ MatOp::جوڙيو(ثابت Mat_اظهار& e1, ثابت Mat_اظهار& e2, Mat_اظهار& res) ثابت
{
    CV_INSTRUMENT_REGION();

    إذا( هذا == e2.op )
    {
        مزدوج alpha = 1, beta = 1;
        Scalar s;
        Mat m1, m2;
        إذا( is_جوڙيو_Ex(e1) && (!e1.b.معلومات || e1.beta == 0) )
        {
            m1 = e1.a;
            alpha = e1.alpha;
            s = e1.s;
        }
        وإلا
            e1.op->assign(e1, m1);

        إذا( is_جوڙيو_Ex(e2) && (!e2.b.معلومات || e2.beta == 0) )
        {
            m2 = e2.a;
            beta = e2.alpha;
            s += e2.s;
        }
        وإلا
            e2.op->assign(e2, m2);
        Mat_Op_جوڙيو_Ex::make_اظهار(res, m1, m2, alpha, beta, s);
    }
    وإلا
        e2.op->جوڙيو(e1, e2, res);
}


فراغ MatOp::جوڙيو(ثابت Mat_اظهار& expr1, ثابت Scalar& s, Mat_اظهار& res) ثابت
{
    CV_INSTRUMENT_REGION();

    Mat m1;
    expr1.op->assign(expr1, m1);
    Mat_Op_جوڙيو_Ex::make_اظهار(res, m1, Mat(), 1, 0, s);
}


فراغ MatOp::ڪٽيو(ثابت Mat_اظهار& e1, ثابت Mat_اظهار& e2, Mat_اظهار& res) ثابت
{
    CV_INSTRUMENT_REGION();

    إذا( هذا == e2.op )
    {
        مزدوج alpha = 1, beta = -1;
        Scalar s;
        Mat m1, m2;
        إذا( is_جوڙيو_Ex(e1) && (!e1.b.معلومات || e1.beta == 0) )
        {
            m1 = e1.a;
            alpha = e1.alpha;
            s = e1.s;
        }
        وإلا
            e1.op->assign(e1, m1);

        إذا( is_جوڙيو_Ex(e2) && (!e2.b.معلومات || e2.beta == 0) )
        {
            m2 = e2.a;
            beta = -e2.alpha;
            s -= e2.s;
        }
        وإلا
            e2.op->assign(e2, m2);
        Mat_Op_جوڙيو_Ex::make_اظهار(res, m1, m2, alpha, beta, s);
    }
    وإلا
        e2.op->ڪٽيو(e1, e2, res);
}


فراغ MatOp::ڪٽيو(ثابت Scalar& s, ثابت Mat_اظهار& اظهار, Mat_اظهار& res) ثابت
{
    CV_INSTRUMENT_REGION();

    Mat m;
    اظهار.op->assign(اظهار, m);
    Mat_Op_جوڙيو_Ex::make_اظهار(res, m, Mat(), -1, 0, s);
}


فراغ MatOp::multiply(ثابت Mat_اظهار& e1, ثابت Mat_اظهار& e2, Mat_اظهار& res, مزدوج scale) ثابت
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

            MatOp_Bin::make_اظهار(res, '/', m2, e1.a, scale/e1.alpha);
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

            MatOp_Bin::make_اظهار(res, op, m1, m2, scale);
        }
    }
    وإلا
        e2.op->multiply(e1, e2, res, scale);
}


فراغ MatOp::multiply(ثابت Mat_اظهار& اظهار, مزدوج s, Mat_اظهار& res) ثابت
{
    CV_INSTRUMENT_REGION();

    Mat m;
    اظهار.op->assign(اظهار, m);
    Mat_Op_جوڙيو_Ex::make_اظهار(res, m, Mat(), s, 0);
}


فراغ MatOp::divide(ثابت Mat_اظهار& e1, ثابت Mat_اظهار& e2, Mat_اظهار& res, مزدوج scale) ثابت
{
    CV_INSTRUMENT_REGION();

    إذا( هذا == e2.op )
    {
        إذا( isReciprocal(e1) && isReciprocal(e2) )
            MatOp_Bin::make_اظهار(res, '/', e2.a, e1.a, e1.alpha/e2.alpha);
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
            MatOp_Bin::make_اظهار(res, op, m1, m2, scale);
        }
    }
    وإلا
        e2.op->divide(e1, e2, res, scale);
}


فراغ MatOp::divide(مزدوج s, ثابت Mat_اظهار& اظهار, Mat_اظهار& res) ثابت
{
    CV_INSTRUMENT_REGION();

    Mat m;
    اظهار.op->assign(اظهار, m);
    MatOp_Bin::make_اظهار(res, '/', m, Mat(), s);
}


فراغ MatOp::مطلق_قدر(ثابت Mat_اظهار& اظهار, Mat_اظهار& res) ثابت
{
    CV_INSTRUMENT_REGION();

    Mat m;
    اظهار.op->assign(اظهار, m);
    MatOp_Bin::make_اظهار(res, 'a', m, Mat());
}


فراغ MatOp::transpose(ثابت Mat_اظهار& اظهار, Mat_اظهار& res) ثابت
{
    CV_INSTRUMENT_REGION();

    Mat m;
    اظهار.op->assign(اظهار, m);
    MatOp_T::make_اظهار(res, m, 1);
}


فراغ MatOp::matmul(ثابت Mat_اظهار& e1, ثابت Mat_اظهار& e2, Mat_اظهار& res) ثابت
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

        MatOp_GEMM::make_اظهار(res, flags, m1, m2, scale);
    }
    وإلا
        e2.op->matmul(e1, e2, res);
}


فراغ MatOp::invert(ثابت Mat_اظهار& اظهار, صحيح طريقو, Mat_اظهار& res) ثابت
{
    Mat m;
    اظهار.op->assign(اظهار, m);
    MatOp_Invert::make_اظهار(res, طريقو, m);
}


سائيز MatOp::size(ثابت Mat_اظهار& اظهار) ثابت
{
    أرجع !اظهار.a.empty() ? اظهار.a.size() : اظهار.b.empty() ? اظهار.b.size() : اظهار.c.size();
}

صحيح MatOp::type(ثابت Mat_اظهار& اظهار) ثابت
{
    CV_INSTRUMENT_REGION();

    أرجع !اظهار.a.empty() ? اظهار.a.type() : اظهار.b.empty() ? اظهار.b.type() : اظهار.c.type();
}

//////////////////////////////////////////////////////////////////////////////////////////////////

Mat_اظهار::Mat_اظهار(ثابت Mat& m) : op(&g_Mat_Op_سڃاڻپ_عنصر), flags(0), a(m), b(Mat()), c(Mat()), alpha(1), beta(0), s(Scalar())
{
}

Mat_اظهار Mat_اظهار::صف(صحيح y) ثابت
{
    Mat_اظهار e;
    op->roi(*هذا, Range(y, y+1), Range::all(), e);
    أرجع e;
}

Mat_اظهار Mat_اظهار::کانو(صحيح x) ثابت
{
    Mat_اظهار e;
    op->roi(*هذا, Range::all(), Range(x, x+1), e);
    أرجع e;
}

Mat_اظهار Mat_اظهار::diag(صحيح d) ثابت
{
    Mat_اظهار e;
    op->diag(*هذا, d, e);
    أرجع e;
}

Mat_اظهار Mat_اظهار::عامل()( ثابت Range& صف_Range_2, ثابت Range& کانو_Range_2 ) ثابت
{
    Mat_اظهار e;
    op->roi(*هذا, صف_Range_2, کانو_Range_2, e);
    أرجع e;
}

Mat_اظهار Mat_اظهار::عامل()( ثابت Rect& roi ) ثابت
{
    Mat_اظهار e;
    op->roi(*هذا, Range(roi.y, roi.y + roi.اوچائي), Range(roi.x, roi.x + roi.ويڪر), e);
    أرجع e;
}

Mat Mat_اظهار::cross(ثابت Mat& m) ثابت
{
    أرجع ((Mat)*هذا).cross(m);
}

مزدوج Mat_اظهار::dot(ثابت Mat& m) ثابت
{
    أرجع ((Mat)*هذا).dot(m);
}

Mat_اظهار Mat_اظهار::t() ثابت
{
    Mat_اظهار e;
    op->transpose(*هذا, e);
    أرجع e;
}

Mat_اظهار Mat_اظهار::inv(صحيح طريقو) ثابت
{
    Mat_اظهار e;
    op->invert(*هذا, طريقو, e);
    أرجع e;
}

Mat_اظهار Mat_اظهار::mul(ثابت Mat_اظهار& e, مزدوج scale) ثابت
{
    Mat_اظهار en;
    op->multiply(*هذا, e, en, scale);
    أرجع en;
}

Mat_اظهار Mat_اظهار::mul(ثابت Mat& m, مزدوج scale) ثابت
{
    Mat_اظهار e;
    op->multiply(*هذا, Mat_اظهار(m), e, scale);
    أرجع e;
}

Mat_اظهار عامل + (ثابت Mat& a, ثابت Mat& b)
{
    checkOperandsExist(a, b);
    Mat_اظهار e;
    Mat_Op_جوڙيو_Ex::make_اظهار(e, a, b, 1, 1);
    أرجع e;
}

Mat_اظهار عامل + (ثابت Mat& a, ثابت Scalar& s)
{
    checkOperandsExist(a);
    Mat_اظهار e;
    Mat_Op_جوڙيو_Ex::make_اظهار(e, a, Mat(), 1, 0, s);
    أرجع e;
}

Mat_اظهار عامل + (ثابت Scalar& s, ثابت Mat& a)
{
    checkOperandsExist(a);
    Mat_اظهار e;
    Mat_Op_جوڙيو_Ex::make_اظهار(e, a, Mat(), 1, 0, s);
    أرجع e;
}

Mat_اظهار عامل + (ثابت Mat_اظهار& e, ثابت Mat& m)
{
    Mat_اظهار en;
    e.op->جوڙيو(e, Mat_اظهار(m), en);
    أرجع en;
}

Mat_اظهار عامل + (ثابت Mat& m, ثابت Mat_اظهار& e)
{
    checkOperandsExist(m);
    Mat_اظهار en;
    e.op->جوڙيو(e, Mat_اظهار(m), en);
    أرجع en;
}

Mat_اظهار عامل + (ثابت Mat_اظهار& e, ثابت Scalar& s)
{
    Mat_اظهار en;
    e.op->جوڙيو(e, s, en);
    أرجع en;
}

Mat_اظهار عامل + (ثابت Scalar& s, ثابت Mat_اظهار& e)
{
    Mat_اظهار en;
    e.op->جوڙيو(e, s, en);
    أرجع en;
}

Mat_اظهار عامل + (ثابت Mat_اظهار& e1, ثابت Mat_اظهار& e2)
{
    Mat_اظهار en;
    e1.op->جوڙيو(e1, e2, en);
    أرجع en;
}

Mat_اظهار عامل - (ثابت Mat& a, ثابت Mat& b)
{
    checkOperandsExist(a, b);
    Mat_اظهار e;
    Mat_Op_جوڙيو_Ex::make_اظهار(e, a, b, 1, -1);
    أرجع e;
}

Mat_اظهار عامل - (ثابت Mat& a, ثابت Scalar& s)
{
    checkOperandsExist(a);
    Mat_اظهار e;
    Mat_Op_جوڙيو_Ex::make_اظهار(e, a, Mat(), 1, 0, -s);
    أرجع e;
}

Mat_اظهار عامل - (ثابت Scalar& s, ثابت Mat& a)
{
    checkOperandsExist(a);
    Mat_اظهار e;
    Mat_Op_جوڙيو_Ex::make_اظهار(e, a, Mat(), -1, 0, s);
    أرجع e;
}

Mat_اظهار عامل - (ثابت Mat_اظهار& e, ثابت Mat& m)
{
    checkOperandsExist(m);
    Mat_اظهار en;
    e.op->ڪٽيو(e, Mat_اظهار(m), en);
    أرجع en;
}

Mat_اظهار عامل - (ثابت Mat& m, ثابت Mat_اظهار& e)
{
    checkOperandsExist(m);
    Mat_اظهار en;
    e.op->ڪٽيو(Mat_اظهار(m), e, en);
    أرجع en;
}

Mat_اظهار عامل - (ثابت Mat_اظهار& e, ثابت Scalar& s)
{
    Mat_اظهار en;
    e.op->جوڙيو(e, -s, en);
    أرجع en;
}

Mat_اظهار عامل - (ثابت Scalar& s, ثابت Mat_اظهار& e)
{
    Mat_اظهار en;
    e.op->ڪٽيو(s, e, en);
    أرجع en;
}

Mat_اظهار عامل - (ثابت Mat_اظهار& e1, ثابت Mat_اظهار& e2)
{
    Mat_اظهار en;
    e1.op->ڪٽيو(e1, e2, en);
    أرجع en;
}

Mat_اظهار عامل - (ثابت Mat& m)
{
    checkOperandsExist(m);
    Mat_اظهار e;
    Mat_Op_جوڙيو_Ex::make_اظهار(e, m, Mat(), -1, 0);
    أرجع e;
}

Mat_اظهار عامل - (ثابت Mat_اظهار& e)
{
    Mat_اظهار en;
    e.op->ڪٽيو(Scalar(0), e, en);
    أرجع en;
}

Mat_اظهار عامل * (ثابت Mat& a, ثابت Mat& b)
{
    checkOperandsExist(a, b);
    Mat_اظهار e;
    MatOp_GEMM::make_اظهار(e, 0, a, b);
    أرجع e;
}

Mat_اظهار عامل * (ثابت Mat& a, مزدوج s)
{
    checkOperandsExist(a);
    Mat_اظهار e;
    Mat_Op_جوڙيو_Ex::make_اظهار(e, a, Mat(), s, 0);
    أرجع e;
}

Mat_اظهار عامل * (مزدوج s, ثابت Mat& a)
{
    checkOperandsExist(a);
    Mat_اظهار e;
    Mat_Op_جوڙيو_Ex::make_اظهار(e, a, Mat(), s, 0);
    أرجع e;
}

Mat_اظهار عامل * (ثابت Mat_اظهار& e, ثابت Mat& m)
{
    Mat_اظهار en;
    e.op->matmul(e, Mat_اظهار(m), en);
    أرجع en;
}

Mat_اظهار عامل * (ثابت Mat& m, ثابت Mat_اظهار& e)
{
    checkOperandsExist(m);
    Mat_اظهار en;
    e.op->matmul(Mat_اظهار(m), e, en);
    أرجع en;
}

Mat_اظهار عامل * (ثابت Mat_اظهار& e, مزدوج s)
{
    Mat_اظهار en;
    e.op->multiply(e, s, en);
    أرجع en;
}

Mat_اظهار عامل * (مزدوج s, ثابت Mat_اظهار& e)
{
    Mat_اظهار en;
    e.op->multiply(e, s, en);
    أرجع en;
}

Mat_اظهار عامل * (ثابت Mat_اظهار& e1, ثابت Mat_اظهار& e2)
{
    Mat_اظهار en;
    e1.op->matmul(e1, e2, en);
    أرجع en;
}

Mat_اظهار عامل / (ثابت Mat& a, ثابت Mat& b)
{
    checkOperandsExist(a, b);
    Mat_اظهار e;
    MatOp_Bin::make_اظهار(e, '/', a, b);
    أرجع e;
}

Mat_اظهار عامل / (ثابت Mat& a, مزدوج s)
{
    checkOperandsExist(a);
    Mat_اظهار e;
    Mat_Op_جوڙيو_Ex::make_اظهار(e, a, Mat(), 1./s, 0);
    أرجع e;
}

Mat_اظهار عامل / (مزدوج s, ثابت Mat& a)
{
    checkOperandsExist(a);
    Mat_اظهار e;
    MatOp_Bin::make_اظهار(e, '/', a, Mat(), s);
    أرجع e;
}

Mat_اظهار عامل / (ثابت Mat_اظهار& e, ثابت Mat& m)
{
    Mat_اظهار en;
    e.op->divide(e, Mat_اظهار(m), en);
    أرجع en;
}

Mat_اظهار عامل / (ثابت Mat& m, ثابت Mat_اظهار& e)
{
    checkOperandsExist(m);
    Mat_اظهار en;
    e.op->divide(Mat_اظهار(m), e, en);
    أرجع en;
}

Mat_اظهار عامل / (ثابت Mat_اظهار& e, مزدوج s)
{
    Mat_اظهار en;
    e.op->multiply(e, 1./s, en);
    أرجع en;
}

Mat_اظهار عامل / (مزدوج s, ثابت Mat_اظهار& e)
{
    Mat_اظهار en;
    e.op->divide(s, e, en);
    أرجع en;
}

Mat_اظهار عامل / (ثابت Mat_اظهار& e1, ثابت Mat_اظهار& e2)
{
    Mat_اظهار en;
    e1.op->divide(e1, e2, en);
    أرجع en;
}

Mat_اظهار عامل < (ثابت Mat& a, ثابت Mat& b)
{
    checkOperandsExist(a, b);
    Mat_اظهار e;
    MatOp_Cmp::make_اظهار(e, cv::CMP_LT, a, b);
    أرجع e;
}

Mat_اظهار عامل < (ثابت Mat& a, مزدوج s)
{
    checkOperandsExist(a);
    Mat_اظهار e;
    MatOp_Cmp::make_اظهار(e, cv::CMP_LT, a, s);
    أرجع e;
}

Mat_اظهار عامل < (مزدوج s, ثابت Mat& a)
{
    checkOperandsExist(a);
    Mat_اظهار e;
    MatOp_Cmp::make_اظهار(e, cv::CMP_GT, a, s);
    أرجع e;
}

Mat_اظهار عامل <= (ثابت Mat& a, ثابت Mat& b)
{
    checkOperandsExist(a, b);
    Mat_اظهار e;
    MatOp_Cmp::make_اظهار(e, cv::CMP_LE, a, b);
    أرجع e;
}

Mat_اظهار عامل <= (ثابت Mat& a, مزدوج s)
{
    checkOperandsExist(a);
    Mat_اظهار e;
    MatOp_Cmp::make_اظهار(e, cv::CMP_LE, a, s);
    أرجع e;
}

Mat_اظهار عامل <= (مزدوج s, ثابت Mat& a)
{
    checkOperandsExist(a);
    Mat_اظهار e;
    MatOp_Cmp::make_اظهار(e, cv::CMP_GE, a, s);
    أرجع e;
}

Mat_اظهار عامل == (ثابت Mat& a, ثابت Mat& b)
{
    checkOperandsExist(a, b);
    Mat_اظهار e;
    MatOp_Cmp::make_اظهار(e, cv::CMP_EQ, a, b);
    أرجع e;
}

Mat_اظهار عامل == (ثابت Mat& a, مزدوج s)
{
    checkOperandsExist(a);
    Mat_اظهار e;
    MatOp_Cmp::make_اظهار(e, cv::CMP_EQ, a, s);
    أرجع e;
}

Mat_اظهار عامل == (مزدوج s, ثابت Mat& a)
{
    checkOperandsExist(a);
    Mat_اظهار e;
    MatOp_Cmp::make_اظهار(e, cv::CMP_EQ, a, s);
    أرجع e;
}

Mat_اظهار عامل != (ثابت Mat& a, ثابت Mat& b)
{
    checkOperandsExist(a, b);
    Mat_اظهار e;
    MatOp_Cmp::make_اظهار(e, cv::CMP_NE, a, b);
    أرجع e;
}

Mat_اظهار عامل != (ثابت Mat& a, مزدوج s)
{
    checkOperandsExist(a);
    Mat_اظهار e;
    MatOp_Cmp::make_اظهار(e, cv::CMP_NE, a, s);
    أرجع e;
}

Mat_اظهار عامل != (مزدوج s, ثابت Mat& a)
{
    checkOperandsExist(a);
    Mat_اظهار e;
    MatOp_Cmp::make_اظهار(e, cv::CMP_NE, a, s);
    أرجع e;
}

Mat_اظهار عامل >= (ثابت Mat& a, ثابت Mat& b)
{
    checkOperandsExist(a, b);
    Mat_اظهار e;
    MatOp_Cmp::make_اظهار(e, cv::CMP_GE, a, b);
    أرجع e;
}

Mat_اظهار عامل >= (ثابت Mat& a, مزدوج s)
{
    checkOperandsExist(a);
    Mat_اظهار e;
    MatOp_Cmp::make_اظهار(e, cv::CMP_GE, a, s);
    أرجع e;
}

Mat_اظهار عامل >= (مزدوج s, ثابت Mat& a)
{
    checkOperandsExist(a);
    Mat_اظهار e;
    MatOp_Cmp::make_اظهار(e, cv::CMP_LE, a, s);
    أرجع e;
}

Mat_اظهار عامل > (ثابت Mat& a, ثابت Mat& b)
{
    checkOperandsExist(a, b);
    Mat_اظهار e;
    MatOp_Cmp::make_اظهار(e, cv::CMP_GT, a, b);
    أرجع e;
}

Mat_اظهار عامل > (ثابت Mat& a, مزدوج s)
{
    checkOperandsExist(a);
    Mat_اظهار e;
    MatOp_Cmp::make_اظهار(e, cv::CMP_GT, a, s);
    أرجع e;
}

Mat_اظهار عامل > (مزدوج s, ثابت Mat& a)
{
    checkOperandsExist(a);
    Mat_اظهار e;
    MatOp_Cmp::make_اظهار(e, cv::CMP_LT, a, s);
    أرجع e;
}

Mat_اظهار min(ثابت Mat& a, ثابت Mat& b)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a, b);
    Mat_اظهار e;
    MatOp_Bin::make_اظهار(e, 'm', a, b);
    أرجع e;
}

Mat_اظهار min(ثابت Mat& a, مزدوج s)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    Mat_اظهار e;
    MatOp_Bin::make_اظهار(e, 'n', a, s);
    أرجع e;
}

Mat_اظهار min(مزدوج s, ثابت Mat& a)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    Mat_اظهار e;
    MatOp_Bin::make_اظهار(e, 'n', a, s);
    أرجع e;
}

Mat_اظهار max(ثابت Mat& a, ثابت Mat& b)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a, b);
    Mat_اظهار e;
    MatOp_Bin::make_اظهار(e, 'M', a, b);
    أرجع e;
}

Mat_اظهار max(ثابت Mat& a, مزدوج s)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    Mat_اظهار e;
    MatOp_Bin::make_اظهار(e, 'N', a, s);
    أرجع e;
}

Mat_اظهار max(مزدوج s, ثابت Mat& a)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    Mat_اظهار e;
    MatOp_Bin::make_اظهار(e, 'N', a, s);
    أرجع e;
}

Mat_اظهار عامل & (ثابت Mat& a, ثابت Mat& b)
{
    checkOperandsExist(a, b);
    Mat_اظهار e;
    MatOp_Bin::make_اظهار(e, '&', a, b);
    أرجع e;
}

Mat_اظهار عامل & (ثابت Mat& a, ثابت Scalar& s)
{
    checkOperandsExist(a);
    Mat_اظهار e;
    MatOp_Bin::make_اظهار(e, '&', a, s);
    أرجع e;
}

Mat_اظهار عامل & (ثابت Scalar& s, ثابت Mat& a)
{
    checkOperandsExist(a);
    Mat_اظهار e;
    MatOp_Bin::make_اظهار(e, '&', a, s);
    أرجع e;
}

Mat_اظهار عامل | (ثابت Mat& a, ثابت Mat& b)
{
    checkOperandsExist(a, b);
    Mat_اظهار e;
    MatOp_Bin::make_اظهار(e, '|', a, b);
    أرجع e;
}

Mat_اظهار عامل | (ثابت Mat& a, ثابت Scalar& s)
{
    checkOperandsExist(a);
    Mat_اظهار e;
    MatOp_Bin::make_اظهار(e, '|', a, s);
    أرجع e;
}

Mat_اظهار عامل | (ثابت Scalar& s, ثابت Mat& a)
{
    checkOperandsExist(a);
    Mat_اظهار e;
    MatOp_Bin::make_اظهار(e, '|', a, s);
    أرجع e;
}

Mat_اظهار عامل ^ (ثابت Mat& a, ثابت Mat& b)
{
    checkOperandsExist(a, b);
    Mat_اظهار e;
    MatOp_Bin::make_اظهار(e, '^', a, b);
    أرجع e;
}

Mat_اظهار عامل ^ (ثابت Mat& a, ثابت Scalar& s)
{
    checkOperandsExist(a);
    Mat_اظهار e;
    MatOp_Bin::make_اظهار(e, '^', a, s);
    أرجع e;
}

Mat_اظهار عامل ^ (ثابت Scalar& s, ثابت Mat& a)
{
    checkOperandsExist(a);
    Mat_اظهار e;
    MatOp_Bin::make_اظهار(e, '^', a, s);
    أرجع e;
}

Mat_اظهار عامل ~(ثابت Mat& a)
{
    checkOperandsExist(a);
    Mat_اظهار e;
    MatOp_Bin::make_اظهار(e, '~', a, Scalar());
    أرجع e;
}

Mat_اظهار مطلق_قدر(ثابت Mat& a)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    Mat_اظهار e;
    MatOp_Bin::make_اظهار(e, 'a', a, Scalar());
    أرجع e;
}

Mat_اظهار مطلق_قدر(ثابت Mat_اظهار& e)
{
    CV_INSTRUMENT_REGION();

    Mat_اظهار en;
    e.op->مطلق_قدر(e, en);
    أرجع en;
}


سائيز Mat_اظهار::size() ثابت
{
    إذا( isT(*هذا) || isInv(*هذا) )
        أرجع سائيز(a.صفون, a.کانن_ڳڻپ);
    إذا( isSolve(*هذا) )
        أرجع سائيز(b.کانن_ڳڻپ, a.کانن_ڳڻپ);
    إذا( isInitializer(*هذا) )
        أرجع a.size();
    أرجع op ? op->size(*هذا) : سائيز();
}


صحيح Mat_اظهار::type() ثابت
{
    CV_INSTRUMENT_REGION();

    إذا( isInitializer(*هذا) )
        أرجع a.type();
    إذا( isCmp(*هذا) )
        أرجع CV_MAKETYPE(CV_8U, a.channels());
    أرجع op ? op->type(*هذا) : -1;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////

فراغ Mat_Op_سڃاڻپ_عنصر::assign(ثابت Mat_اظهار& e, Mat& m, صحيح _type) ثابت
{
    إذا( _type == -1 || _type == e.a.type() )
        m = e.a;
    وإلا
    {
        CV_Assert( CV_MAT_CN(_type) == e.a.channels() );
        e.a.convert_منزل(m, _type);
    }
}

مضمن فراغ Mat_Op_سڃاڻپ_عنصر::make_اظهار(Mat_اظهار& res, ثابت Mat& m)
{
    res = Mat_اظهار(&g_Mat_Op_سڃاڻپ_عنصر, 0, m, Mat(), Mat(), 1, 0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

فراغ Mat_Op_جوڙيو_Ex::assign(ثابت Mat_اظهار& e, Mat& m, صحيح _type) ثابت
{
    Mat عارضي_2, &dst = _type == -1 || e.a.type() == _type ? m : عارضي_2;
    إذا( e.b.معلومات )
    {
        إذا( e.s == Scalar() || !e.s.isReal() )
        {
            إذا( e.alpha == 1 )
            {
                إذا( e.beta == 1 )
                    cv::جوڙيو(e.a, e.b, dst);
                وإلا إذا( e.beta == -1 )
                    cv::ڪٽيو(e.a, e.b, dst);
                وإلا
                    cv::scale_جوڙيو(e.b, e.beta, e.a, dst);
            }
            وإلا إذا( e.beta == 1 )
            {
                إذا( e.alpha == -1 )
                    cv::ڪٽيو(e.b, e.a, dst);
                وإلا
                    cv::scale_جوڙيو(e.a, e.alpha, e.b, dst);
            }
            وإلا
                cv::جوڙيو_Weighted(e.a, e.alpha, e.b, e.beta, 0, dst);

            إذا( !e.s.isReal() )
                cv::جوڙيو(dst, e.s, dst);
        }
        وإلا
        {
            إذا (e.a.channels() > 1)
                CV_LOG_ONCE_WARNING(عدم, "OpenCV/MatExpr: processing of multi-channel arrays might be changed in the future: "
                                          "https://github.com/opencv/opencv/issues/16739");
            cv::جوڙيو_Weighted(e.a, e.alpha, e.b, e.beta, e.s[0], dst);
        }
    }
    وإلا إذا( e.s.isReal() && (dst.معلومات != m.معلومات || fabs(e.alpha) != 1))
    {
        إذا (e.a.channels() > 1 && e.s[0] != 0.0)
            CV_LOG_ONCE_WARNING(عدم, "OpenCV/MatExpr: processing of multi-channel arrays might be changed in the future: "
                                      "https://github.com/opencv/opencv/issues/16739");
        e.a.convert_منزل(m, _type, e.alpha, e.s[0]);
        أرجع;
    }
    وإلا إذا( e.alpha == 1 )
        cv::جوڙيو(e.a, e.s, dst);
    وإلا إذا( e.alpha == -1 )
        cv::ڪٽيو(e.s, e.a, dst);
    وإلا
    {
        e.a.convert_منزل(dst, e.a.type(), e.alpha);
        cv::جوڙيو(dst, e.s, dst);
    }

    إذا( dst.معلومات != m.معلومات )
        dst.convert_منزل(m, m.type());
}


فراغ Mat_Op_جوڙيو_Ex::جوڙيو(ثابت Mat_اظهار& e, ثابت Scalar& s, Mat_اظهار& res) ثابت
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.s += s;
}


فراغ Mat_Op_جوڙيو_Ex::ڪٽيو(ثابت Scalar& s, ثابت Mat_اظهار& e, Mat_اظهار& res) ثابت
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha = -res.alpha;
    res.beta = -res.beta;
    res.s = s - res.s;
}

فراغ Mat_Op_جوڙيو_Ex::multiply(ثابت Mat_اظهار& e, مزدوج s, Mat_اظهار& res) ثابت
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha *= s;
    res.beta *= s;
    res.s *= s;
}

فراغ Mat_Op_جوڙيو_Ex::divide(مزدوج s, ثابت Mat_اظهار& e, Mat_اظهار& res) ثابت
{
    CV_INSTRUMENT_REGION();

    إذا( isScaled(e) )
        MatOp_Bin::make_اظهار(res, '/', e.a, Mat(), s/e.alpha);
    وإلا
        MatOp::divide(s, e, res);
}


فراغ Mat_Op_جوڙيو_Ex::transpose(ثابت Mat_اظهار& e, Mat_اظهار& res) ثابت
{
    CV_INSTRUMENT_REGION();

    إذا( isScaled(e) )
        MatOp_T::make_اظهار(res, e.a, e.alpha);
    وإلا
        MatOp::transpose(e, res);
}

فراغ Mat_Op_جوڙيو_Ex::مطلق_قدر(ثابت Mat_اظهار& e, Mat_اظهار& res) ثابت
{
    CV_INSTRUMENT_REGION();

    إذا( (!e.b.معلومات || e.beta == 0) && fabs(e.alpha) == 1 )
        MatOp_Bin::make_اظهار(res, 'a', e.a, -e.s*e.alpha);
    وإلا إذا( e.b.معلومات && e.alpha + e.beta == 0 && e.alpha*e.beta == -1 )
        MatOp_Bin::make_اظهار(res, 'a', e.a, e.b);
    وإلا
        MatOp::مطلق_قدر(e, res);
}

مضمن فراغ Mat_Op_جوڙيو_Ex::make_اظهار(Mat_اظهار& res, ثابت Mat& a, ثابت Mat& b, مزدوج alpha, مزدوج beta, ثابت Scalar& s)
{
    res = Mat_اظهار(&g_Mat_Op_جوڙيو_Ex, 0, a, b, Mat(), alpha, beta, s);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

فراغ MatOp_Bin::assign(ثابت Mat_اظهار& e, Mat& m, صحيح _type) ثابت
{
    Mat عارضي_2, &dst = _type == -1 || e.a.type() == _type ? m : عارضي_2;

    إذا( e.flags == '*' )
        cv::multiply(e.a, e.b, dst, e.alpha);
    وإلا إذا( e.flags == '/' && e.b.معلومات )
        cv::divide(e.a, e.b, dst, e.alpha);
    وإلا إذا( e.flags == '/' && !e.b.معلومات )
        cv::divide(e.alpha, e.a, dst );
    وإلا إذا( e.flags == '&' && e.b.معلومات )
        bitwise_and(e.a, e.b, dst);
    وإلا إذا( e.flags == '&' && !e.b.معلومات )
        bitwise_and(e.a, e.s, dst);
    وإلا إذا( e.flags == '|' && e.b.معلومات )
        bitwise_or(e.a, e.b, dst);
    وإلا إذا( e.flags == '|' && !e.b.معلومات )
        bitwise_or(e.a, e.s, dst);
    وإلا إذا( e.flags == '^' && e.b.معلومات )
        bitwise_xor(e.a, e.b, dst);
    وإلا إذا( e.flags == '^' && !e.b.معلومات )
        bitwise_xor(e.a, e.s, dst);
    وإلا إذا( e.flags == '~' && !e.b.معلومات )
        bitwise_not(e.a, dst);
    وإلا إذا( e.flags == 'm' )
        cv::min(e.a, e.b, dst);
    وإلا إذا( e.flags == 'n' )
        cv::min(e.a, e.s[0], dst);
    وإلا إذا( e.flags == 'M' )
        cv::max(e.a, e.b, dst);
    وإلا إذا( e.flags == 'N' )
        cv::max(e.a, e.s[0], dst);
    وإلا إذا( e.flags == 'a' && e.b.معلومات )
        cv::absdiff(e.a, e.b, dst);
    وإلا إذا( e.flags == 'a' && !e.b.معلومات )
        cv::absdiff(e.a, e.s, dst);
    وإلا
        CV_Error(cv::Error::StsError, "Unknown operation");

    إذا( dst.معلومات != m.معلومات )
        dst.convert_منزل(m, _type);
}

فراغ MatOp_Bin::multiply(ثابت Mat_اظهار& e, مزدوج s, Mat_اظهار& res) ثابت
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

فراغ MatOp_Bin::divide(مزدوج s, ثابت Mat_اظهار& e, Mat_اظهار& res) ثابت
{
    CV_INSTRUMENT_REGION();

    إذا( e.flags == '/' && (!e.b.معلومات || e.beta == 0) )
        Mat_Op_جوڙيو_Ex::make_اظهار(res, e.a, Mat(), s/e.alpha, 0);
    وإلا
        MatOp::divide(s, e, res);
}

مضمن فراغ MatOp_Bin::make_اظهار(Mat_اظهار& res, حرف op, ثابت Mat& a, ثابت Mat& b, مزدوج scale)
{
    res = Mat_اظهار(&g_MatOp_Bin, op, a, b, Mat(), scale, b.معلومات ? 1 : 0);
}

مضمن فراغ MatOp_Bin::make_اظهار(Mat_اظهار& res, حرف op, ثابت Mat& a, ثابت Scalar& s)
{
    res = Mat_اظهار(&g_MatOp_Bin, op, a, Mat(), Mat(), 1, 0, s);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

فراغ MatOp_Cmp::assign(ثابت Mat_اظهار& e, Mat& m, صحيح _type) ثابت
{
    Mat عارضي_2, &dst = _type == -1 || _type == CV_8U ? m : عارضي_2;

    إذا( e.b.معلومات )
        cv::compare(e.a, e.b, dst, e.flags);
    وإلا
        cv::compare(e.a, e.alpha, dst, e.flags);

    إذا( dst.معلومات != m.معلومات )
        dst.convert_منزل(m, _type);
}

مضمن فراغ MatOp_Cmp::make_اظهار(Mat_اظهار& res, صحيح cmpop, ثابت Mat& a, ثابت Mat& b)
{
    res = Mat_اظهار(&g_MatOp_Cmp, cmpop, a, b, Mat(), 1, 1);
}

مضمن فراغ MatOp_Cmp::make_اظهار(Mat_اظهار& res, صحيح cmpop, ثابت Mat& a, مزدوج alpha)
{
    res = Mat_اظهار(&g_MatOp_Cmp, cmpop, a, Mat(), Mat(), alpha, 1);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

فراغ MatOp_T::assign(ثابت Mat_اظهار& e, Mat& m, صحيح _type) ثابت
{
    Mat عارضي_2, &dst = _type == -1 || _type == e.a.type() ? m : عارضي_2;

    cv::transpose(e.a, dst);

    إذا( dst.معلومات != m.معلومات || e.alpha != 1 )
        dst.convert_منزل(m, _type, e.alpha);
}

فراغ MatOp_T::multiply(ثابت Mat_اظهار& e, مزدوج s, Mat_اظهار& res) ثابت
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha *= s;
}

فراغ MatOp_T::transpose(ثابت Mat_اظهار& e, Mat_اظهار& res) ثابت
{
    CV_INSTRUMENT_REGION();

    إذا( e.alpha == 1 )
        Mat_Op_سڃاڻپ_عنصر::make_اظهار(res, e.a);
    وإلا
        Mat_Op_جوڙيو_Ex::make_اظهار(res, e.a, Mat(), e.alpha, 0);
}

مضمن فراغ MatOp_T::make_اظهار(Mat_اظهار& res, ثابت Mat& a, مزدوج alpha)
{
    res = Mat_اظهار(&g_MatOp_T, 0, a, Mat(), Mat(), alpha, 0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

فراغ MatOp_GEMM::assign(ثابت Mat_اظهار& e, Mat& m, صحيح _type) ثابت
{
    Mat عارضي_2, &dst = _type == -1 || _type == e.a.type() ? m : عارضي_2;

    cv::gemm(e.a, e.b, e.alpha, e.c, e.beta, dst, e.flags);
    إذا( dst.معلومات != m.معلومات )
        dst.convert_منزل(m, _type);
}

فراغ MatOp_GEMM::جوڙيو(ثابت Mat_اظهار& e1, ثابت Mat_اظهار& e2, Mat_اظهار& res) ثابت
{
    CV_INSTRUMENT_REGION();

    منطقي i1 = is_سڃاڻپ_عنصر(e1), i2 = is_سڃاڻپ_عنصر(e2);
    مزدوج alpha1 = i1 ? 1 : e1.alpha, alpha2 = i2 ? 1 : e2.alpha;

    إذا( isMatProd(e1) && (i2 || isScaled(e2) || isT(e2)) )
        MatOp_GEMM::make_اظهار(res, (e1.flags & ~cv::GEMM_3_T)|(isT(e2) ? cv::GEMM_3_T : 0),
                             e1.a, e1.b, alpha1, e2.a, alpha2);
    وإلا إذا( isMatProd(e2) && (i1 || isScaled(e1) || isT(e1)) )
        MatOp_GEMM::make_اظهار(res, (e2.flags & ~cv::GEMM_3_T)|(isT(e1) ? cv::GEMM_3_T : 0),
                             e2.a, e2.b, alpha2, e1.a, alpha1);
    وإلا إذا( هذا == e2.op )
        MatOp::جوڙيو(e1, e2, res);
    وإلا
        e2.op->جوڙيو(e1, e2, res);
}

فراغ MatOp_GEMM::ڪٽيو(ثابت Mat_اظهار& e1, ثابت Mat_اظهار& e2, Mat_اظهار& res) ثابت
{
    CV_INSTRUMENT_REGION();

    منطقي i1 = is_سڃاڻپ_عنصر(e1), i2 = is_سڃاڻپ_عنصر(e2);
    مزدوج alpha1 = i1 ? 1 : e1.alpha, alpha2 = i2 ? 1 : e2.alpha;

    إذا( isMatProd(e1) && (i2 || isScaled(e2) || isT(e2)) )
        MatOp_GEMM::make_اظهار(res, (e1.flags & ~cv::GEMM_3_T)|(isT(e2) ? cv::GEMM_3_T : 0),
                             e1.a, e1.b, alpha1, e2.a, -alpha2);
    وإلا إذا( isMatProd(e2) && (i1 || isScaled(e1) || isT(e1)) )
        MatOp_GEMM::make_اظهار(res, (e2.flags & ~cv::GEMM_3_T)|(isT(e1) ? cv::GEMM_3_T : 0),
                            e2.a, e2.b, -alpha2, e1.a, alpha1);
    وإلا إذا( هذا == e2.op )
        MatOp::ڪٽيو(e1, e2, res);
    وإلا
        e2.op->ڪٽيو(e1, e2, res);
}

فراغ MatOp_GEMM::multiply(ثابت Mat_اظهار& e, مزدوج s, Mat_اظهار& res) ثابت
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha *= s;
    res.beta *= s;
}

فراغ MatOp_GEMM::transpose(ثابت Mat_اظهار& e, Mat_اظهار& res) ثابت
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.flags = (!(e.flags & cv::GEMM_1_T) ? cv::GEMM_2_T : 0) |
                (!(e.flags & cv::GEMM_2_T) ? cv::GEMM_1_T : 0) |
                (!(e.flags & cv::GEMM_3_T) ? cv::GEMM_3_T : 0);
    swap(res.a, res.b);
}

مضمن فراغ MatOp_GEMM::make_اظهار(Mat_اظهار& res, صحيح flags, ثابت Mat& a, ثابت Mat& b,
                                 مزدوج alpha, ثابت Mat& c, مزدوج beta)
{
    res = Mat_اظهار(&g_MatOp_GEMM, flags, a, b, c, alpha, beta);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

فراغ MatOp_Invert::assign(ثابت Mat_اظهار& e, Mat& m, صحيح _type) ثابت
{
    Mat عارضي_2, &dst = _type == -1 || _type == e.a.type() ? m : عارضي_2;

    cv::invert(e.a, dst, e.flags);
    إذا( dst.معلومات != m.معلومات )
        dst.convert_منزل(m, _type);
}

فراغ MatOp_Invert::matmul(ثابت Mat_اظهار& e1, ثابت Mat_اظهار& e2, Mat_اظهار& res) ثابت
{
    إذا( isInv(e1) && is_سڃاڻپ_عنصر(e2) )
        MatOp_Solve::make_اظهار(res, e1.flags, e1.a, e2.a);
    وإلا إذا( هذا == e2.op )
        MatOp::matmul(e1, e2, res);
    وإلا
        e2.op->matmul(e1, e2, res);
}

مضمن فراغ MatOp_Invert::make_اظهار(Mat_اظهار& res, صحيح طريقو, ثابت Mat& m)
{
    res = Mat_اظهار(&g_MatOp_Invert, طريقو, m, Mat(), Mat(), 1, 0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

فراغ MatOp_Solve::assign(ثابت Mat_اظهار& e, Mat& m, صحيح _type) ثابت
{
    Mat عارضي_2, &dst = _type == -1 || _type == e.a.type() ? m : عارضي_2;

    cv::solve(e.a, e.b, dst, e.flags);
    إذا( dst.معلومات != m.معلومات )
        dst.convert_منزل(m, _type);
}

مضمن فراغ MatOp_Solve::make_اظهار(Mat_اظهار& res, صحيح طريقو, ثابت Mat& a, ثابت Mat& b)
{
    res = Mat_اظهار(&g_MatOp_Solve, طريقو, a, b, Mat(), 1, 1);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

فراغ MatOp_Initializer::assign(ثابت Mat_اظهار& e, Mat& m, صحيح _type) ثابت
{
    إذا( _type == -1 )
        _type = e.a.type();

    m.create(e.a.size, _type);

    إذا( e.flags == 'I' && e.a.dims <= 2 )
        سيٽ_سڃاڻپ_عنصر(m, Scalar(e.alpha));
    وإلا إذا( e.flags == '0' )
        m = Scalar();
    وإلا إذا( e.flags == '1' )
        m = Scalar(e.alpha);
    وإلا
        CV_Error(cv::Error::StsError, "Invalid matrix initializer type");
}

فراغ MatOp_Initializer::multiply(ثابت Mat_اظهار& e, مزدوج s, Mat_اظهار& res) ثابت
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha *= s;
}

مضمن فراغ MatOp_Initializer::make_اظهار(Mat_اظهار& res, صحيح طريقو, سائيز sz, صحيح type, مزدوج alpha)
{
    res = Mat_اظهار(getGlobalMatOpInitializer(), طريقو, Mat(sz, type, (فراغ*)(size_t)0xEEEEEEEE), Mat(), Mat(), alpha, 0);
}

مضمن فراغ MatOp_Initializer::make_اظهار(Mat_اظهار& res, صحيح طريقو, صحيح ndims, ثابت صحيح* sizes, صحيح type, مزدوج alpha)
{
    res = Mat_اظهار(getGlobalMatOpInitializer(), طريقو, Mat(ndims, sizes, type, (فراغ*)(size_t)0xEEEEEEEE), Mat(), Mat(), alpha, 0);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

Mat_اظهار Mat::t() ثابت
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(*هذا);
    Mat_اظهار e;
    MatOp_T::make_اظهار(e, *هذا);
    أرجع e;
}

Mat_اظهار Mat::inv(صحيح طريقو) ثابت
{
    CV_INSTRUMENT_REGION();

    Mat_اظهار e;
    MatOp_Invert::make_اظهار(e, طريقو, *هذا);
    أرجع e;
}


Mat_اظهار Mat::mul(داخلا_Array m, مزدوج scale) ثابت
{
    CV_INSTRUMENT_REGION();

    Mat b = m.getMat();
    // Unless the argument is a refcounted Mat/UMat, the header returned by getMat() may be
    // a non-owning view of caller memory (e.g. a scalar bound to _InputArray(const double&),
    // a Matx or a Vec) that the returned MatExpr can outlive, so snapshot it.
    // See https://github.com/opencv/opencv/issues/23577
    إذا( !m.isMat() && !m.isUMat() )
        b = b.clone();

    Mat_اظهار e;
    MatOp_Bin::make_اظهار(e, '*', *هذا, b, scale);
    أرجع e;
}

Mat_اظهار Mat::zeros(صحيح صفون, صحيح کانن_ڳڻپ, صحيح type)
{
    CV_INSTRUMENT_REGION();

    Mat_اظهار e;
    MatOp_Initializer::make_اظهار(e, '0', سائيز(کانن_ڳڻپ, صفون), type);
    أرجع e;
}

Mat_اظهار Mat::zeros(سائيز size, صحيح type)
{
    CV_INSTRUMENT_REGION();

    Mat_اظهار e;
    MatOp_Initializer::make_اظهار(e, '0', size, type);
    أرجع e;
}

Mat_اظهار Mat::zeros(صحيح ndims, ثابت صحيح* sizes, صحيح type)
{
    CV_INSTRUMENT_REGION();

    Mat_اظهار e;
    MatOp_Initializer::make_اظهار(e, '0', ndims, sizes, type);
    أرجع e;
}

Mat_اظهار Mat::zeros(ثابت MatShape& shape, صحيح type)
{
    CV_INSTRUMENT_REGION();

    Mat_اظهار e;
    MatOp_Initializer::make_اظهار(e, '0', shape.dims, shape.p, type);
    أرجع e;
}

Mat_اظهار Mat::ones(صحيح صفون, صحيح کانن_ڳڻپ, صحيح type)
{
    CV_INSTRUMENT_REGION();

    Mat_اظهار e;
    MatOp_Initializer::make_اظهار(e, '1', سائيز(کانن_ڳڻپ, صفون), type);
    أرجع e;
}

Mat_اظهار Mat::ones(سائيز size, صحيح type)
{
    CV_INSTRUMENT_REGION();

    Mat_اظهار e;
    MatOp_Initializer::make_اظهار(e, '1', size, type);
    أرجع e;
}

Mat_اظهار Mat::ones(صحيح ndims, ثابت صحيح* sizes, صحيح type)
{
    CV_INSTRUMENT_REGION();

    Mat_اظهار e;
    MatOp_Initializer::make_اظهار(e, '1', ndims, sizes, type);
    أرجع e;
}

Mat_اظهار Mat::ones(ثابت MatShape& shape, صحيح type)
{
    CV_INSTRUMENT_REGION();

    Mat_اظهار e;
    MatOp_Initializer::make_اظهار(e, '1', shape.dims, shape.p, type);
    أرجع e;
}

Mat_اظهار Mat::eye(صحيح صفون, صحيح کانن_ڳڻپ, صحيح type)
{
    CV_INSTRUMENT_REGION();

    Mat_اظهار e;
    MatOp_Initializer::make_اظهار(e, 'I', سائيز(کانن_ڳڻپ, صفون), type);
    أرجع e;
}

Mat_اظهار Mat::eye(سائيز size, صحيح type)
{
    CV_INSTRUMENT_REGION();

    Mat_اظهار e;
    MatOp_Initializer::make_اظهار(e, 'I', size, type);
    أرجع e;
}

فراغ Mat_اظهار::swap(Mat_اظهار& other)
{
    استعمال std::swap;

    swap(op, other.op);
    swap(flags, other.flags);

    swap(a, other.a);
    swap(b, other.b);
    swap(c, other.c);

    swap(alpha, other.alpha);
    swap(beta, other.beta);

    swap(s, other.s);
}

داخلا_Array_2::داخلا_Array_2(ثابت Mat_اظهار& اظهار)
{
    إذا (!is_سڃاڻپ_عنصر(اظهار))
    {
        Mat نتيجو = اظهار;  // TODO improve through refcount == 1 of expr.a (inplace operation is possible - except gemm?)
        Mat_اظهار نتيجو_اظهار(نتيجو);
        swap(const_cast<Mat_اظهار&>(اظهار), نتيجو_اظهار);
    }
    CV_Assert(is_سڃاڻپ_عنصر(اظهار));
    init(FIXED_TYPE + FIXED_سائيز + MAT + ACCESS_READ, &اظهار.a);
}

} // cv::
