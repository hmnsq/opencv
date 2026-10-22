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


صنف Mat_Op_عنصر_خنثی CV_FINAL : عام MatOp
{
عام:
    Mat_Op_عنصر_خنثی() {}
    مجازی ~Mat_Op_عنصر_خنثی() {}

    منطقي elementWise(ثابت Mat_عبارت& /*عبارت*/) ثابت CV_OVERRIDE { أرجع درست; }
    فراغ assign(ثابت Mat_عبارت& عبارت, Mat& m, صحيح type=-1) ثابت CV_OVERRIDE;

    ساكن فراغ make_عبارت(Mat_عبارت& res, ثابت Mat& m);
};

ساكن Mat_Op_عنصر_خنثی g_Mat_Op_عنصر_خنثی;

صنف Mat_Op_جمع_کن_Ex CV_FINAL : عام MatOp
{
عام:
    Mat_Op_جمع_کن_Ex() {}
    مجازی ~Mat_Op_جمع_کن_Ex() {}

    منطقي elementWise(ثابت Mat_عبارت& /*عبارت*/) ثابت CV_OVERRIDE { أرجع درست; }
    فراغ assign(ثابت Mat_عبارت& عبارت, Mat& m, صحيح type=-1) ثابت CV_OVERRIDE;

    فراغ جمع_کن(ثابت Mat_عبارت& e1, ثابت Scalar& s, Mat_عبارت& res) ثابت CV_OVERRIDE;
    فراغ تفریق(ثابت Scalar& s, ثابت Mat_عبارت& عبارت, Mat_عبارت& res) ثابت CV_OVERRIDE;
    فراغ multiply(ثابت Mat_عبارت& e1, مزدوج s, Mat_عبارت& res) ثابت CV_OVERRIDE;
    فراغ divide(مزدوج s, ثابت Mat_عبارت& e, Mat_عبارت& res) ثابت CV_OVERRIDE;

    فراغ transpose(ثابت Mat_عبارت& e1, Mat_عبارت& res) ثابت CV_OVERRIDE;
    فراغ قدر_مطلق(ثابت Mat_عبارت& عبارت, Mat_عبارت& res) ثابت CV_OVERRIDE;

    ساكن فراغ make_عبارت(Mat_عبارت& res, ثابت Mat& a, ثابت Mat& b, مزدوج alpha, مزدوج beta, ثابت Scalar& s=Scalar());
};

ساكن Mat_Op_جمع_کن_Ex g_Mat_Op_جمع_کن_Ex;

صنف MatOp_Bin CV_FINAL : عام MatOp
{
عام:
    MatOp_Bin() {}
    مجازی ~MatOp_Bin() {}

    منطقي elementWise(ثابت Mat_عبارت& /*عبارت*/) ثابت CV_OVERRIDE { أرجع درست; }
    فراغ assign(ثابت Mat_عبارت& عبارت, Mat& m, صحيح type=-1) ثابت CV_OVERRIDE;

    فراغ multiply(ثابت Mat_عبارت& e1, مزدوج s, Mat_عبارت& res) ثابت CV_OVERRIDE;
    فراغ divide(مزدوج s, ثابت Mat_عبارت& e, Mat_عبارت& res) ثابت CV_OVERRIDE;

    ساكن فراغ make_عبارت(Mat_عبارت& res, حرف op, ثابت Mat& a, ثابت Mat& b, مزدوج scale=1);
    ساكن فراغ make_عبارت(Mat_عبارت& res, حرف op, ثابت Mat& a, ثابت Scalar& s);
};

ساكن MatOp_Bin g_MatOp_Bin;

صنف MatOp_Cmp CV_FINAL : عام MatOp
{
عام:
    MatOp_Cmp() {}
    مجازی ~MatOp_Cmp() {}

    منطقي elementWise(ثابت Mat_عبارت& /*عبارت*/) ثابت CV_OVERRIDE { أرجع درست; }
    فراغ assign(ثابت Mat_عبارت& عبارت, Mat& m, صحيح type=-1) ثابت CV_OVERRIDE;

    ساكن فراغ make_عبارت(Mat_عبارت& res, صحيح cmpop, ثابت Mat& a, ثابت Mat& b);
    ساكن فراغ make_عبارت(Mat_عبارت& res, صحيح cmpop, ثابت Mat& a, مزدوج alpha);
};

ساكن MatOp_Cmp g_MatOp_Cmp;

صنف MatOp_GEMM CV_FINAL : عام MatOp
{
عام:
    MatOp_GEMM() {}
    مجازی ~MatOp_GEMM() {}

    منطقي elementWise(ثابت Mat_عبارت& /*عبارت*/) ثابت CV_OVERRIDE { أرجع خطأ; }
    فراغ assign(ثابت Mat_عبارت& عبارت, Mat& m, صحيح type=-1) ثابت CV_OVERRIDE;

    فراغ جمع_کن(ثابت Mat_عبارت& e1, ثابت Mat_عبارت& e2, Mat_عبارت& res) ثابت CV_OVERRIDE;
    فراغ تفریق(ثابت Mat_عبارت& e1, ثابت Mat_عبارت& e2, Mat_عبارت& res) ثابت CV_OVERRIDE;
    فراغ multiply(ثابت Mat_عبارت& e, مزدوج s, Mat_عبارت& res) ثابت CV_OVERRIDE;

    فراغ transpose(ثابت Mat_عبارت& عبارت, Mat_عبارت& res) ثابت CV_OVERRIDE;

    اندازه size(ثابت Mat_عبارت& عبارت) ثابت CV_OVERRIDE
    {
        أرجع اندازه(
            (عبارت.flags & GEMM_2_T) ? عبارت.b.ردیفها : عبارت.b.تعداد_ستون,
            (عبارت.flags & GEMM_1_T) ? عبارت.a.تعداد_ستون : عبارت.a.ردیفها
        );
    }

    ساكن فراغ make_عبارت(Mat_عبارت& res, صحيح flags, ثابت Mat& a, ثابت Mat& b,
                         مزدوج alpha=1, ثابت Mat& c=Mat(), مزدوج beta=1);

};

ساكن MatOp_GEMM g_MatOp_GEMM;

صنف MatOp_Invert CV_FINAL : عام MatOp
{
عام:
    MatOp_Invert() {}
    مجازی ~MatOp_Invert() {}

    منطقي elementWise(ثابت Mat_عبارت& /*عبارت*/) ثابت CV_OVERRIDE { أرجع خطأ; }
    فراغ assign(ثابت Mat_عبارت& عبارت, Mat& m, صحيح type=-1) ثابت CV_OVERRIDE;

    فراغ matmul(ثابت Mat_عبارت& expr1, ثابت Mat_عبارت& expr2, Mat_عبارت& res) ثابت CV_OVERRIDE;

    ساكن فراغ make_عبارت(Mat_عبارت& res, صحيح روش, ثابت Mat& m);
};

ساكن MatOp_Invert g_MatOp_Invert;

صنف MatOp_T CV_FINAL : عام MatOp
{
عام:
    MatOp_T() {}
    مجازی ~MatOp_T() {}

    منطقي elementWise(ثابت Mat_عبارت& /*عبارت*/) ثابت CV_OVERRIDE { أرجع خطأ; }
    فراغ assign(ثابت Mat_عبارت& عبارت, Mat& m, صحيح type=-1) ثابت CV_OVERRIDE;

    فراغ multiply(ثابت Mat_عبارت& e1, مزدوج s, Mat_عبارت& res) ثابت CV_OVERRIDE;
    فراغ transpose(ثابت Mat_عبارت& عبارت, Mat_عبارت& res) ثابت CV_OVERRIDE;

    ساكن فراغ make_عبارت(Mat_عبارت& res, ثابت Mat& a, مزدوج alpha=1);
};

ساكن MatOp_T g_MatOp_T;

صنف MatOp_Solve CV_FINAL : عام MatOp
{
عام:
    MatOp_Solve() {}
    مجازی ~MatOp_Solve() {}

    منطقي elementWise(ثابت Mat_عبارت& /*عبارت*/) ثابت CV_OVERRIDE { أرجع خطأ; }
    فراغ assign(ثابت Mat_عبارت& عبارت, Mat& m, صحيح type=-1) ثابت CV_OVERRIDE;

    ساكن فراغ make_عبارت(Mat_عبارت& res, صحيح روش, ثابت Mat& a, ثابت Mat& b);
};

ساكن MatOp_Solve g_MatOp_Solve;

صنف MatOp_Initializer CV_FINAL : عام MatOp
{
عام:
    MatOp_Initializer() {}
    مجازی ~MatOp_Initializer() {}

    منطقي elementWise(ثابت Mat_عبارت& /*عبارت*/) ثابت CV_OVERRIDE { أرجع خطأ; }
    فراغ assign(ثابت Mat_عبارت& عبارت, Mat& m, صحيح type=-1) ثابت CV_OVERRIDE;

    فراغ multiply(ثابت Mat_عبارت& e, مزدوج s, Mat_عبارت& res) ثابت CV_OVERRIDE;

    ساكن فراغ make_عبارت(Mat_عبارت& res, صحيح روش, اندازه sz, صحيح type, مزدوج alpha=1);
    ساكن فراغ make_عبارت(Mat_عبارت& res, صحيح روش, صحيح ndims, ثابت صحيح* sizes, صحيح type, مزدوج alpha=1);
};

ساكن MatOp_Initializer* getGlobalMatOpInitializer()
{
    CV_SINGLETON_LAZY_INIT(MatOp_Initializer, جديد MatOp_Initializer())
}

ساكن مضمن منطقي is_عنصر_خنثی(ثابت Mat_عبارت& e) { أرجع e.op == &g_Mat_Op_عنصر_خنثی; }
ساكن مضمن منطقي is_جمع_کن_Ex(ثابت Mat_عبارت& e) { أرجع e.op == &g_Mat_Op_جمع_کن_Ex; }
ساكن مضمن منطقي isScaled(ثابت Mat_عبارت& e) { أرجع is_جمع_کن_Ex(e) && (!e.b.داده || e.beta == 0) && e.s == Scalar(); }
ساكن مضمن منطقي isBin(ثابت Mat_عبارت& e, حرف c) { أرجع e.op == &g_MatOp_Bin && e.flags == c; }
ساكن مضمن منطقي isCmp(ثابت Mat_عبارت& e) { أرجع e.op == &g_MatOp_Cmp; }
ساكن مضمن منطقي isReciprocal(ثابت Mat_عبارت& e) { أرجع isBin(e,'/') && (!e.b.داده || e.beta == 0); }
ساكن مضمن منطقي isT(ثابت Mat_عبارت& e) { أرجع e.op == &g_MatOp_T; }
ساكن مضمن منطقي isInv(ثابت Mat_عبارت& e) { أرجع e.op == &g_MatOp_Invert; }
ساكن مضمن منطقي isSolve(ثابت Mat_عبارت& e) { أرجع e.op == &g_MatOp_Solve; }
//static inline bool isGEMM(const MatExpr& e) { return e.op == &g_MatOp_GEMM; }
ساكن مضمن منطقي isMatProd(ثابت Mat_عبارت& e) { أرجع e.op == &g_MatOp_GEMM && (!e.c.داده || e.beta == 0); }
ساكن مضمن منطقي isInitializer(ثابت Mat_عبارت& e) { أرجع e.op == getGlobalMatOpInitializer(); }

/////////////////////////////////////////////////////////////////////////////////////////////////////

MatOp::MatOp() {}
MatOp::~MatOp() {}


منطقي MatOp::elementWise(ثابت Mat_عبارت& /*عبارت*/) ثابت
{
    أرجع خطأ;
}

فراغ MatOp::roi(ثابت Mat_عبارت& عبارت, ثابت Range& ردیف_Range_2, ثابت Range& ستون_Range_2, Mat_عبارت& e) ثابت
{
    إذا( elementWise(عبارت) )
    {
        e = Mat_عبارت(عبارت.op, عبارت.flags, Mat(), Mat(), Mat(),
                    عبارت.alpha, عبارت.beta, عبارت.s);
        إذا(عبارت.a.داده)
            e.a = عبارت.a(ردیف_Range_2, ستون_Range_2);
        إذا(عبارت.b.داده)
            e.b = عبارت.b(ردیف_Range_2, ستون_Range_2);
        إذا(عبارت.c.داده)
            e.c = عبارت.c(ردیف_Range_2, ستون_Range_2);
    }
    وإلا
    {
        Mat m;
        عبارت.op->assign(عبارت, m);
        e = Mat_عبارت(&g_Mat_Op_عنصر_خنثی, 0, m(ردیف_Range_2, ستون_Range_2), Mat(), Mat());
    }
}

فراغ MatOp::diag(ثابت Mat_عبارت& عبارت, صحيح d, Mat_عبارت& e) ثابت
{
    إذا( elementWise(عبارت) )
    {
        e = Mat_عبارت(عبارت.op, عبارت.flags, Mat(), Mat(), Mat(),
                    عبارت.alpha, عبارت.beta, عبارت.s);
        إذا(عبارت.a.داده)
            e.a = عبارت.a.diag(d);
        إذا(عبارت.b.داده)
            e.b = عبارت.b.diag(d);
        إذا(عبارت.c.داده)
            e.c = عبارت.c.diag(d);
    }
    وإلا
    {
        Mat m;
        عبارت.op->assign(عبارت, m);
        e = Mat_عبارت(&g_Mat_Op_عنصر_خنثی, 0, m.diag(d), Mat(), Mat());
    }
}


فراغ MatOp::aug_تخصیص_بده_جمع_کن(ثابت Mat_عبارت& عبارت, Mat& m) ثابت
{
    Mat موقت_2;
    عبارت.op->assign(عبارت, موقت_2);
    m += موقت_2;
}


فراغ MatOp::aug_تخصیص_بده_تفریق(ثابت Mat_عبارت& عبارت, Mat& m) ثابت
{
    Mat موقت_2;
    عبارت.op->assign(عبارت, موقت_2);
    m -= موقت_2;
}


فراغ MatOp::aug_تخصیص_بده_Multiply(ثابت Mat_عبارت& عبارت, Mat& m) ثابت
{
    Mat موقت_2;
    عبارت.op->assign(عبارت, موقت_2);
    m *= موقت_2;
}


فراغ MatOp::aug_تخصیص_بده_Divide(ثابت Mat_عبارت& عبارت, Mat& m) ثابت
{
    Mat موقت_2;
    عبارت.op->assign(عبارت, موقت_2);
    m /= موقت_2;
}


فراغ MatOp::aug_تخصیص_بده_And(ثابت Mat_عبارت& عبارت, Mat& m) ثابت
{
    Mat موقت_2;
    عبارت.op->assign(عبارت, موقت_2);
    m &= موقت_2;
}


فراغ MatOp::aug_تخصیص_بده_Or(ثابت Mat_عبارت& عبارت, Mat& m) ثابت
{
    Mat موقت_2;
    عبارت.op->assign(عبارت, موقت_2);
    m |= موقت_2;
}


فراغ MatOp::aug_تخصیص_بده_Xor(ثابت Mat_عبارت& عبارت, Mat& m) ثابت
{
    Mat موقت_2;
    عبارت.op->assign(عبارت, موقت_2);
    m ^= موقت_2;
}


فراغ MatOp::جمع_کن(ثابت Mat_عبارت& e1, ثابت Mat_عبارت& e2, Mat_عبارت& res) ثابت
{
    CV_INSTRUMENT_REGION();

    إذا( هذا == e2.op )
    {
        مزدوج alpha = 1, beta = 1;
        Scalar s;
        Mat m1, m2;
        إذا( is_جمع_کن_Ex(e1) && (!e1.b.داده || e1.beta == 0) )
        {
            m1 = e1.a;
            alpha = e1.alpha;
            s = e1.s;
        }
        وإلا
            e1.op->assign(e1, m1);

        إذا( is_جمع_کن_Ex(e2) && (!e2.b.داده || e2.beta == 0) )
        {
            m2 = e2.a;
            beta = e2.alpha;
            s += e2.s;
        }
        وإلا
            e2.op->assign(e2, m2);
        Mat_Op_جمع_کن_Ex::make_عبارت(res, m1, m2, alpha, beta, s);
    }
    وإلا
        e2.op->جمع_کن(e1, e2, res);
}


فراغ MatOp::جمع_کن(ثابت Mat_عبارت& expr1, ثابت Scalar& s, Mat_عبارت& res) ثابت
{
    CV_INSTRUMENT_REGION();

    Mat m1;
    expr1.op->assign(expr1, m1);
    Mat_Op_جمع_کن_Ex::make_عبارت(res, m1, Mat(), 1, 0, s);
}


فراغ MatOp::تفریق(ثابت Mat_عبارت& e1, ثابت Mat_عبارت& e2, Mat_عبارت& res) ثابت
{
    CV_INSTRUMENT_REGION();

    إذا( هذا == e2.op )
    {
        مزدوج alpha = 1, beta = -1;
        Scalar s;
        Mat m1, m2;
        إذا( is_جمع_کن_Ex(e1) && (!e1.b.داده || e1.beta == 0) )
        {
            m1 = e1.a;
            alpha = e1.alpha;
            s = e1.s;
        }
        وإلا
            e1.op->assign(e1, m1);

        إذا( is_جمع_کن_Ex(e2) && (!e2.b.داده || e2.beta == 0) )
        {
            m2 = e2.a;
            beta = -e2.alpha;
            s -= e2.s;
        }
        وإلا
            e2.op->assign(e2, m2);
        Mat_Op_جمع_کن_Ex::make_عبارت(res, m1, m2, alpha, beta, s);
    }
    وإلا
        e2.op->تفریق(e1, e2, res);
}


فراغ MatOp::تفریق(ثابت Scalar& s, ثابت Mat_عبارت& عبارت, Mat_عبارت& res) ثابت
{
    CV_INSTRUMENT_REGION();

    Mat m;
    عبارت.op->assign(عبارت, m);
    Mat_Op_جمع_کن_Ex::make_عبارت(res, m, Mat(), -1, 0, s);
}


فراغ MatOp::multiply(ثابت Mat_عبارت& e1, ثابت Mat_عبارت& e2, Mat_عبارت& res, مزدوج scale) ثابت
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

            MatOp_Bin::make_عبارت(res, '/', m2, e1.a, scale/e1.alpha);
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

            MatOp_Bin::make_عبارت(res, op, m1, m2, scale);
        }
    }
    وإلا
        e2.op->multiply(e1, e2, res, scale);
}


فراغ MatOp::multiply(ثابت Mat_عبارت& عبارت, مزدوج s, Mat_عبارت& res) ثابت
{
    CV_INSTRUMENT_REGION();

    Mat m;
    عبارت.op->assign(عبارت, m);
    Mat_Op_جمع_کن_Ex::make_عبارت(res, m, Mat(), s, 0);
}


فراغ MatOp::divide(ثابت Mat_عبارت& e1, ثابت Mat_عبارت& e2, Mat_عبارت& res, مزدوج scale) ثابت
{
    CV_INSTRUMENT_REGION();

    إذا( هذا == e2.op )
    {
        إذا( isReciprocal(e1) && isReciprocal(e2) )
            MatOp_Bin::make_عبارت(res, '/', e2.a, e1.a, e1.alpha/e2.alpha);
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
            MatOp_Bin::make_عبارت(res, op, m1, m2, scale);
        }
    }
    وإلا
        e2.op->divide(e1, e2, res, scale);
}


فراغ MatOp::divide(مزدوج s, ثابت Mat_عبارت& عبارت, Mat_عبارت& res) ثابت
{
    CV_INSTRUMENT_REGION();

    Mat m;
    عبارت.op->assign(عبارت, m);
    MatOp_Bin::make_عبارت(res, '/', m, Mat(), s);
}


فراغ MatOp::قدر_مطلق(ثابت Mat_عبارت& عبارت, Mat_عبارت& res) ثابت
{
    CV_INSTRUMENT_REGION();

    Mat m;
    عبارت.op->assign(عبارت, m);
    MatOp_Bin::make_عبارت(res, 'a', m, Mat());
}


فراغ MatOp::transpose(ثابت Mat_عبارت& عبارت, Mat_عبارت& res) ثابت
{
    CV_INSTRUMENT_REGION();

    Mat m;
    عبارت.op->assign(عبارت, m);
    MatOp_T::make_عبارت(res, m, 1);
}


فراغ MatOp::matmul(ثابت Mat_عبارت& e1, ثابت Mat_عبارت& e2, Mat_عبارت& res) ثابت
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

        MatOp_GEMM::make_عبارت(res, flags, m1, m2, scale);
    }
    وإلا
        e2.op->matmul(e1, e2, res);
}


فراغ MatOp::invert(ثابت Mat_عبارت& عبارت, صحيح روش, Mat_عبارت& res) ثابت
{
    Mat m;
    عبارت.op->assign(عبارت, m);
    MatOp_Invert::make_عبارت(res, روش, m);
}


اندازه MatOp::size(ثابت Mat_عبارت& عبارت) ثابت
{
    أرجع !عبارت.a.empty() ? عبارت.a.size() : عبارت.b.empty() ? عبارت.b.size() : عبارت.c.size();
}

صحيح MatOp::type(ثابت Mat_عبارت& عبارت) ثابت
{
    CV_INSTRUMENT_REGION();

    أرجع !عبارت.a.empty() ? عبارت.a.type() : عبارت.b.empty() ? عبارت.b.type() : عبارت.c.type();
}

//////////////////////////////////////////////////////////////////////////////////////////////////

Mat_عبارت::Mat_عبارت(ثابت Mat& m) : op(&g_Mat_Op_عنصر_خنثی), flags(0), a(m), b(Mat()), c(Mat()), alpha(1), beta(0), s(Scalar())
{
}

Mat_عبارت Mat_عبارت::ردیف(صحيح y) ثابت
{
    Mat_عبارت e;
    op->roi(*هذا, Range(y, y+1), Range::all(), e);
    أرجع e;
}

Mat_عبارت Mat_عبارت::ستون(صحيح x) ثابت
{
    Mat_عبارت e;
    op->roi(*هذا, Range::all(), Range(x, x+1), e);
    أرجع e;
}

Mat_عبارت Mat_عبارت::diag(صحيح d) ثابت
{
    Mat_عبارت e;
    op->diag(*هذا, d, e);
    أرجع e;
}

Mat_عبارت Mat_عبارت::عملگر()( ثابت Range& ردیف_Range_2, ثابت Range& ستون_Range_2 ) ثابت
{
    Mat_عبارت e;
    op->roi(*هذا, ردیف_Range_2, ستون_Range_2, e);
    أرجع e;
}

Mat_عبارت Mat_عبارت::عملگر()( ثابت Rect& roi ) ثابت
{
    Mat_عبارت e;
    op->roi(*هذا, Range(roi.y, roi.y + roi.ارتفاع), Range(roi.x, roi.x + roi.عرض), e);
    أرجع e;
}

Mat Mat_عبارت::cross(ثابت Mat& m) ثابت
{
    أرجع ((Mat)*هذا).cross(m);
}

مزدوج Mat_عبارت::dot(ثابت Mat& m) ثابت
{
    أرجع ((Mat)*هذا).dot(m);
}

Mat_عبارت Mat_عبارت::t() ثابت
{
    Mat_عبارت e;
    op->transpose(*هذا, e);
    أرجع e;
}

Mat_عبارت Mat_عبارت::inv(صحيح روش) ثابت
{
    Mat_عبارت e;
    op->invert(*هذا, روش, e);
    أرجع e;
}

Mat_عبارت Mat_عبارت::mul(ثابت Mat_عبارت& e, مزدوج scale) ثابت
{
    Mat_عبارت en;
    op->multiply(*هذا, e, en, scale);
    أرجع en;
}

Mat_عبارت Mat_عبارت::mul(ثابت Mat& m, مزدوج scale) ثابت
{
    Mat_عبارت e;
    op->multiply(*هذا, Mat_عبارت(m), e, scale);
    أرجع e;
}

Mat_عبارت عملگر + (ثابت Mat& a, ثابت Mat& b)
{
    checkOperandsExist(a, b);
    Mat_عبارت e;
    Mat_Op_جمع_کن_Ex::make_عبارت(e, a, b, 1, 1);
    أرجع e;
}

Mat_عبارت عملگر + (ثابت Mat& a, ثابت Scalar& s)
{
    checkOperandsExist(a);
    Mat_عبارت e;
    Mat_Op_جمع_کن_Ex::make_عبارت(e, a, Mat(), 1, 0, s);
    أرجع e;
}

Mat_عبارت عملگر + (ثابت Scalar& s, ثابت Mat& a)
{
    checkOperandsExist(a);
    Mat_عبارت e;
    Mat_Op_جمع_کن_Ex::make_عبارت(e, a, Mat(), 1, 0, s);
    أرجع e;
}

Mat_عبارت عملگر + (ثابت Mat_عبارت& e, ثابت Mat& m)
{
    Mat_عبارت en;
    e.op->جمع_کن(e, Mat_عبارت(m), en);
    أرجع en;
}

Mat_عبارت عملگر + (ثابت Mat& m, ثابت Mat_عبارت& e)
{
    checkOperandsExist(m);
    Mat_عبارت en;
    e.op->جمع_کن(e, Mat_عبارت(m), en);
    أرجع en;
}

Mat_عبارت عملگر + (ثابت Mat_عبارت& e, ثابت Scalar& s)
{
    Mat_عبارت en;
    e.op->جمع_کن(e, s, en);
    أرجع en;
}

Mat_عبارت عملگر + (ثابت Scalar& s, ثابت Mat_عبارت& e)
{
    Mat_عبارت en;
    e.op->جمع_کن(e, s, en);
    أرجع en;
}

Mat_عبارت عملگر + (ثابت Mat_عبارت& e1, ثابت Mat_عبارت& e2)
{
    Mat_عبارت en;
    e1.op->جمع_کن(e1, e2, en);
    أرجع en;
}

Mat_عبارت عملگر - (ثابت Mat& a, ثابت Mat& b)
{
    checkOperandsExist(a, b);
    Mat_عبارت e;
    Mat_Op_جمع_کن_Ex::make_عبارت(e, a, b, 1, -1);
    أرجع e;
}

Mat_عبارت عملگر - (ثابت Mat& a, ثابت Scalar& s)
{
    checkOperandsExist(a);
    Mat_عبارت e;
    Mat_Op_جمع_کن_Ex::make_عبارت(e, a, Mat(), 1, 0, -s);
    أرجع e;
}

Mat_عبارت عملگر - (ثابت Scalar& s, ثابت Mat& a)
{
    checkOperandsExist(a);
    Mat_عبارت e;
    Mat_Op_جمع_کن_Ex::make_عبارت(e, a, Mat(), -1, 0, s);
    أرجع e;
}

Mat_عبارت عملگر - (ثابت Mat_عبارت& e, ثابت Mat& m)
{
    checkOperandsExist(m);
    Mat_عبارت en;
    e.op->تفریق(e, Mat_عبارت(m), en);
    أرجع en;
}

Mat_عبارت عملگر - (ثابت Mat& m, ثابت Mat_عبارت& e)
{
    checkOperandsExist(m);
    Mat_عبارت en;
    e.op->تفریق(Mat_عبارت(m), e, en);
    أرجع en;
}

Mat_عبارت عملگر - (ثابت Mat_عبارت& e, ثابت Scalar& s)
{
    Mat_عبارت en;
    e.op->جمع_کن(e, -s, en);
    أرجع en;
}

Mat_عبارت عملگر - (ثابت Scalar& s, ثابت Mat_عبارت& e)
{
    Mat_عبارت en;
    e.op->تفریق(s, e, en);
    أرجع en;
}

Mat_عبارت عملگر - (ثابت Mat_عبارت& e1, ثابت Mat_عبارت& e2)
{
    Mat_عبارت en;
    e1.op->تفریق(e1, e2, en);
    أرجع en;
}

Mat_عبارت عملگر - (ثابت Mat& m)
{
    checkOperandsExist(m);
    Mat_عبارت e;
    Mat_Op_جمع_کن_Ex::make_عبارت(e, m, Mat(), -1, 0);
    أرجع e;
}

Mat_عبارت عملگر - (ثابت Mat_عبارت& e)
{
    Mat_عبارت en;
    e.op->تفریق(Scalar(0), e, en);
    أرجع en;
}

Mat_عبارت عملگر * (ثابت Mat& a, ثابت Mat& b)
{
    checkOperandsExist(a, b);
    Mat_عبارت e;
    MatOp_GEMM::make_عبارت(e, 0, a, b);
    أرجع e;
}

Mat_عبارت عملگر * (ثابت Mat& a, مزدوج s)
{
    checkOperandsExist(a);
    Mat_عبارت e;
    Mat_Op_جمع_کن_Ex::make_عبارت(e, a, Mat(), s, 0);
    أرجع e;
}

Mat_عبارت عملگر * (مزدوج s, ثابت Mat& a)
{
    checkOperandsExist(a);
    Mat_عبارت e;
    Mat_Op_جمع_کن_Ex::make_عبارت(e, a, Mat(), s, 0);
    أرجع e;
}

Mat_عبارت عملگر * (ثابت Mat_عبارت& e, ثابت Mat& m)
{
    Mat_عبارت en;
    e.op->matmul(e, Mat_عبارت(m), en);
    أرجع en;
}

Mat_عبارت عملگر * (ثابت Mat& m, ثابت Mat_عبارت& e)
{
    checkOperandsExist(m);
    Mat_عبارت en;
    e.op->matmul(Mat_عبارت(m), e, en);
    أرجع en;
}

Mat_عبارت عملگر * (ثابت Mat_عبارت& e, مزدوج s)
{
    Mat_عبارت en;
    e.op->multiply(e, s, en);
    أرجع en;
}

Mat_عبارت عملگر * (مزدوج s, ثابت Mat_عبارت& e)
{
    Mat_عبارت en;
    e.op->multiply(e, s, en);
    أرجع en;
}

Mat_عبارت عملگر * (ثابت Mat_عبارت& e1, ثابت Mat_عبارت& e2)
{
    Mat_عبارت en;
    e1.op->matmul(e1, e2, en);
    أرجع en;
}

Mat_عبارت عملگر / (ثابت Mat& a, ثابت Mat& b)
{
    checkOperandsExist(a, b);
    Mat_عبارت e;
    MatOp_Bin::make_عبارت(e, '/', a, b);
    أرجع e;
}

Mat_عبارت عملگر / (ثابت Mat& a, مزدوج s)
{
    checkOperandsExist(a);
    Mat_عبارت e;
    Mat_Op_جمع_کن_Ex::make_عبارت(e, a, Mat(), 1./s, 0);
    أرجع e;
}

Mat_عبارت عملگر / (مزدوج s, ثابت Mat& a)
{
    checkOperandsExist(a);
    Mat_عبارت e;
    MatOp_Bin::make_عبارت(e, '/', a, Mat(), s);
    أرجع e;
}

Mat_عبارت عملگر / (ثابت Mat_عبارت& e, ثابت Mat& m)
{
    Mat_عبارت en;
    e.op->divide(e, Mat_عبارت(m), en);
    أرجع en;
}

Mat_عبارت عملگر / (ثابت Mat& m, ثابت Mat_عبارت& e)
{
    checkOperandsExist(m);
    Mat_عبارت en;
    e.op->divide(Mat_عبارت(m), e, en);
    أرجع en;
}

Mat_عبارت عملگر / (ثابت Mat_عبارت& e, مزدوج s)
{
    Mat_عبارت en;
    e.op->multiply(e, 1./s, en);
    أرجع en;
}

Mat_عبارت عملگر / (مزدوج s, ثابت Mat_عبارت& e)
{
    Mat_عبارت en;
    e.op->divide(s, e, en);
    أرجع en;
}

Mat_عبارت عملگر / (ثابت Mat_عبارت& e1, ثابت Mat_عبارت& e2)
{
    Mat_عبارت en;
    e1.op->divide(e1, e2, en);
    أرجع en;
}

Mat_عبارت عملگر < (ثابت Mat& a, ثابت Mat& b)
{
    checkOperandsExist(a, b);
    Mat_عبارت e;
    MatOp_Cmp::make_عبارت(e, cv::CMP_LT, a, b);
    أرجع e;
}

Mat_عبارت عملگر < (ثابت Mat& a, مزدوج s)
{
    checkOperandsExist(a);
    Mat_عبارت e;
    MatOp_Cmp::make_عبارت(e, cv::CMP_LT, a, s);
    أرجع e;
}

Mat_عبارت عملگر < (مزدوج s, ثابت Mat& a)
{
    checkOperandsExist(a);
    Mat_عبارت e;
    MatOp_Cmp::make_عبارت(e, cv::CMP_GT, a, s);
    أرجع e;
}

Mat_عبارت عملگر <= (ثابت Mat& a, ثابت Mat& b)
{
    checkOperandsExist(a, b);
    Mat_عبارت e;
    MatOp_Cmp::make_عبارت(e, cv::CMP_LE, a, b);
    أرجع e;
}

Mat_عبارت عملگر <= (ثابت Mat& a, مزدوج s)
{
    checkOperandsExist(a);
    Mat_عبارت e;
    MatOp_Cmp::make_عبارت(e, cv::CMP_LE, a, s);
    أرجع e;
}

Mat_عبارت عملگر <= (مزدوج s, ثابت Mat& a)
{
    checkOperandsExist(a);
    Mat_عبارت e;
    MatOp_Cmp::make_عبارت(e, cv::CMP_GE, a, s);
    أرجع e;
}

Mat_عبارت عملگر == (ثابت Mat& a, ثابت Mat& b)
{
    checkOperandsExist(a, b);
    Mat_عبارت e;
    MatOp_Cmp::make_عبارت(e, cv::CMP_EQ, a, b);
    أرجع e;
}

Mat_عبارت عملگر == (ثابت Mat& a, مزدوج s)
{
    checkOperandsExist(a);
    Mat_عبارت e;
    MatOp_Cmp::make_عبارت(e, cv::CMP_EQ, a, s);
    أرجع e;
}

Mat_عبارت عملگر == (مزدوج s, ثابت Mat& a)
{
    checkOperandsExist(a);
    Mat_عبارت e;
    MatOp_Cmp::make_عبارت(e, cv::CMP_EQ, a, s);
    أرجع e;
}

Mat_عبارت عملگر != (ثابت Mat& a, ثابت Mat& b)
{
    checkOperandsExist(a, b);
    Mat_عبارت e;
    MatOp_Cmp::make_عبارت(e, cv::CMP_NE, a, b);
    أرجع e;
}

Mat_عبارت عملگر != (ثابت Mat& a, مزدوج s)
{
    checkOperandsExist(a);
    Mat_عبارت e;
    MatOp_Cmp::make_عبارت(e, cv::CMP_NE, a, s);
    أرجع e;
}

Mat_عبارت عملگر != (مزدوج s, ثابت Mat& a)
{
    checkOperandsExist(a);
    Mat_عبارت e;
    MatOp_Cmp::make_عبارت(e, cv::CMP_NE, a, s);
    أرجع e;
}

Mat_عبارت عملگر >= (ثابت Mat& a, ثابت Mat& b)
{
    checkOperandsExist(a, b);
    Mat_عبارت e;
    MatOp_Cmp::make_عبارت(e, cv::CMP_GE, a, b);
    أرجع e;
}

Mat_عبارت عملگر >= (ثابت Mat& a, مزدوج s)
{
    checkOperandsExist(a);
    Mat_عبارت e;
    MatOp_Cmp::make_عبارت(e, cv::CMP_GE, a, s);
    أرجع e;
}

Mat_عبارت عملگر >= (مزدوج s, ثابت Mat& a)
{
    checkOperandsExist(a);
    Mat_عبارت e;
    MatOp_Cmp::make_عبارت(e, cv::CMP_LE, a, s);
    أرجع e;
}

Mat_عبارت عملگر > (ثابت Mat& a, ثابت Mat& b)
{
    checkOperandsExist(a, b);
    Mat_عبارت e;
    MatOp_Cmp::make_عبارت(e, cv::CMP_GT, a, b);
    أرجع e;
}

Mat_عبارت عملگر > (ثابت Mat& a, مزدوج s)
{
    checkOperandsExist(a);
    Mat_عبارت e;
    MatOp_Cmp::make_عبارت(e, cv::CMP_GT, a, s);
    أرجع e;
}

Mat_عبارت عملگر > (مزدوج s, ثابت Mat& a)
{
    checkOperandsExist(a);
    Mat_عبارت e;
    MatOp_Cmp::make_عبارت(e, cv::CMP_LT, a, s);
    أرجع e;
}

Mat_عبارت min(ثابت Mat& a, ثابت Mat& b)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a, b);
    Mat_عبارت e;
    MatOp_Bin::make_عبارت(e, 'm', a, b);
    أرجع e;
}

Mat_عبارت min(ثابت Mat& a, مزدوج s)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    Mat_عبارت e;
    MatOp_Bin::make_عبارت(e, 'n', a, s);
    أرجع e;
}

Mat_عبارت min(مزدوج s, ثابت Mat& a)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    Mat_عبارت e;
    MatOp_Bin::make_عبارت(e, 'n', a, s);
    أرجع e;
}

Mat_عبارت max(ثابت Mat& a, ثابت Mat& b)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a, b);
    Mat_عبارت e;
    MatOp_Bin::make_عبارت(e, 'M', a, b);
    أرجع e;
}

Mat_عبارت max(ثابت Mat& a, مزدوج s)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    Mat_عبارت e;
    MatOp_Bin::make_عبارت(e, 'N', a, s);
    أرجع e;
}

Mat_عبارت max(مزدوج s, ثابت Mat& a)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    Mat_عبارت e;
    MatOp_Bin::make_عبارت(e, 'N', a, s);
    أرجع e;
}

Mat_عبارت عملگر & (ثابت Mat& a, ثابت Mat& b)
{
    checkOperandsExist(a, b);
    Mat_عبارت e;
    MatOp_Bin::make_عبارت(e, '&', a, b);
    أرجع e;
}

Mat_عبارت عملگر & (ثابت Mat& a, ثابت Scalar& s)
{
    checkOperandsExist(a);
    Mat_عبارت e;
    MatOp_Bin::make_عبارت(e, '&', a, s);
    أرجع e;
}

Mat_عبارت عملگر & (ثابت Scalar& s, ثابت Mat& a)
{
    checkOperandsExist(a);
    Mat_عبارت e;
    MatOp_Bin::make_عبارت(e, '&', a, s);
    أرجع e;
}

Mat_عبارت عملگر | (ثابت Mat& a, ثابت Mat& b)
{
    checkOperandsExist(a, b);
    Mat_عبارت e;
    MatOp_Bin::make_عبارت(e, '|', a, b);
    أرجع e;
}

Mat_عبارت عملگر | (ثابت Mat& a, ثابت Scalar& s)
{
    checkOperandsExist(a);
    Mat_عبارت e;
    MatOp_Bin::make_عبارت(e, '|', a, s);
    أرجع e;
}

Mat_عبارت عملگر | (ثابت Scalar& s, ثابت Mat& a)
{
    checkOperandsExist(a);
    Mat_عبارت e;
    MatOp_Bin::make_عبارت(e, '|', a, s);
    أرجع e;
}

Mat_عبارت عملگر ^ (ثابت Mat& a, ثابت Mat& b)
{
    checkOperandsExist(a, b);
    Mat_عبارت e;
    MatOp_Bin::make_عبارت(e, '^', a, b);
    أرجع e;
}

Mat_عبارت عملگر ^ (ثابت Mat& a, ثابت Scalar& s)
{
    checkOperandsExist(a);
    Mat_عبارت e;
    MatOp_Bin::make_عبارت(e, '^', a, s);
    أرجع e;
}

Mat_عبارت عملگر ^ (ثابت Scalar& s, ثابت Mat& a)
{
    checkOperandsExist(a);
    Mat_عبارت e;
    MatOp_Bin::make_عبارت(e, '^', a, s);
    أرجع e;
}

Mat_عبارت عملگر ~(ثابت Mat& a)
{
    checkOperandsExist(a);
    Mat_عبارت e;
    MatOp_Bin::make_عبارت(e, '~', a, Scalar());
    أرجع e;
}

Mat_عبارت قدر_مطلق(ثابت Mat& a)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    Mat_عبارت e;
    MatOp_Bin::make_عبارت(e, 'a', a, Scalar());
    أرجع e;
}

Mat_عبارت قدر_مطلق(ثابت Mat_عبارت& e)
{
    CV_INSTRUMENT_REGION();

    Mat_عبارت en;
    e.op->قدر_مطلق(e, en);
    أرجع en;
}


اندازه Mat_عبارت::size() ثابت
{
    إذا( isT(*هذا) || isInv(*هذا) )
        أرجع اندازه(a.ردیفها, a.تعداد_ستون);
    إذا( isSolve(*هذا) )
        أرجع اندازه(b.تعداد_ستون, a.تعداد_ستون);
    إذا( isInitializer(*هذا) )
        أرجع a.size();
    أرجع op ? op->size(*هذا) : اندازه();
}


صحيح Mat_عبارت::type() ثابت
{
    CV_INSTRUMENT_REGION();

    إذا( isInitializer(*هذا) )
        أرجع a.type();
    إذا( isCmp(*هذا) )
        أرجع CV_MAKETYPE(CV_8U, a.channels());
    أرجع op ? op->type(*هذا) : -1;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////

فراغ Mat_Op_عنصر_خنثی::assign(ثابت Mat_عبارت& e, Mat& m, صحيح _type) ثابت
{
    إذا( _type == -1 || _type == e.a.type() )
        m = e.a;
    وإلا
    {
        CV_Assert( CV_MAT_CN(_type) == e.a.channels() );
        e.a.convert_مقصد(m, _type);
    }
}

مضمن فراغ Mat_Op_عنصر_خنثی::make_عبارت(Mat_عبارت& res, ثابت Mat& m)
{
    res = Mat_عبارت(&g_Mat_Op_عنصر_خنثی, 0, m, Mat(), Mat(), 1, 0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

فراغ Mat_Op_جمع_کن_Ex::assign(ثابت Mat_عبارت& e, Mat& m, صحيح _type) ثابت
{
    Mat موقت_2, &dst = _type == -1 || e.a.type() == _type ? m : موقت_2;
    إذا( e.b.داده )
    {
        إذا( e.s == Scalar() || !e.s.isReal() )
        {
            إذا( e.alpha == 1 )
            {
                إذا( e.beta == 1 )
                    cv::جمع_کن(e.a, e.b, dst);
                وإلا إذا( e.beta == -1 )
                    cv::تفریق(e.a, e.b, dst);
                وإلا
                    cv::scale_جمع_کن(e.b, e.beta, e.a, dst);
            }
            وإلا إذا( e.beta == 1 )
            {
                إذا( e.alpha == -1 )
                    cv::تفریق(e.b, e.a, dst);
                وإلا
                    cv::scale_جمع_کن(e.a, e.alpha, e.b, dst);
            }
            وإلا
                cv::جمع_کن_Weighted(e.a, e.alpha, e.b, e.beta, 0, dst);

            إذا( !e.s.isReal() )
                cv::جمع_کن(dst, e.s, dst);
        }
        وإلا
        {
            إذا (e.a.channels() > 1)
                CV_LOG_ONCE_WARNING(عدم, "OpenCV/MatExpr: processing of multi-channel arrays might be changed in the future: "
                                          "https://github.com/opencv/opencv/issues/16739");
            cv::جمع_کن_Weighted(e.a, e.alpha, e.b, e.beta, e.s[0], dst);
        }
    }
    وإلا إذا( e.s.isReal() && (dst.داده != m.داده || fabs(e.alpha) != 1))
    {
        إذا (e.a.channels() > 1 && e.s[0] != 0.0)
            CV_LOG_ONCE_WARNING(عدم, "OpenCV/MatExpr: processing of multi-channel arrays might be changed in the future: "
                                      "https://github.com/opencv/opencv/issues/16739");
        e.a.convert_مقصد(m, _type, e.alpha, e.s[0]);
        أرجع;
    }
    وإلا إذا( e.alpha == 1 )
        cv::جمع_کن(e.a, e.s, dst);
    وإلا إذا( e.alpha == -1 )
        cv::تفریق(e.s, e.a, dst);
    وإلا
    {
        e.a.convert_مقصد(dst, e.a.type(), e.alpha);
        cv::جمع_کن(dst, e.s, dst);
    }

    إذا( dst.داده != m.داده )
        dst.convert_مقصد(m, m.type());
}


فراغ Mat_Op_جمع_کن_Ex::جمع_کن(ثابت Mat_عبارت& e, ثابت Scalar& s, Mat_عبارت& res) ثابت
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.s += s;
}


فراغ Mat_Op_جمع_کن_Ex::تفریق(ثابت Scalar& s, ثابت Mat_عبارت& e, Mat_عبارت& res) ثابت
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha = -res.alpha;
    res.beta = -res.beta;
    res.s = s - res.s;
}

فراغ Mat_Op_جمع_کن_Ex::multiply(ثابت Mat_عبارت& e, مزدوج s, Mat_عبارت& res) ثابت
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha *= s;
    res.beta *= s;
    res.s *= s;
}

فراغ Mat_Op_جمع_کن_Ex::divide(مزدوج s, ثابت Mat_عبارت& e, Mat_عبارت& res) ثابت
{
    CV_INSTRUMENT_REGION();

    إذا( isScaled(e) )
        MatOp_Bin::make_عبارت(res, '/', e.a, Mat(), s/e.alpha);
    وإلا
        MatOp::divide(s, e, res);
}


فراغ Mat_Op_جمع_کن_Ex::transpose(ثابت Mat_عبارت& e, Mat_عبارت& res) ثابت
{
    CV_INSTRUMENT_REGION();

    إذا( isScaled(e) )
        MatOp_T::make_عبارت(res, e.a, e.alpha);
    وإلا
        MatOp::transpose(e, res);
}

فراغ Mat_Op_جمع_کن_Ex::قدر_مطلق(ثابت Mat_عبارت& e, Mat_عبارت& res) ثابت
{
    CV_INSTRUMENT_REGION();

    إذا( (!e.b.داده || e.beta == 0) && fabs(e.alpha) == 1 )
        MatOp_Bin::make_عبارت(res, 'a', e.a, -e.s*e.alpha);
    وإلا إذا( e.b.داده && e.alpha + e.beta == 0 && e.alpha*e.beta == -1 )
        MatOp_Bin::make_عبارت(res, 'a', e.a, e.b);
    وإلا
        MatOp::قدر_مطلق(e, res);
}

مضمن فراغ Mat_Op_جمع_کن_Ex::make_عبارت(Mat_عبارت& res, ثابت Mat& a, ثابت Mat& b, مزدوج alpha, مزدوج beta, ثابت Scalar& s)
{
    res = Mat_عبارت(&g_Mat_Op_جمع_کن_Ex, 0, a, b, Mat(), alpha, beta, s);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

فراغ MatOp_Bin::assign(ثابت Mat_عبارت& e, Mat& m, صحيح _type) ثابت
{
    Mat موقت_2, &dst = _type == -1 || e.a.type() == _type ? m : موقت_2;

    إذا( e.flags == '*' )
        cv::multiply(e.a, e.b, dst, e.alpha);
    وإلا إذا( e.flags == '/' && e.b.داده )
        cv::divide(e.a, e.b, dst, e.alpha);
    وإلا إذا( e.flags == '/' && !e.b.داده )
        cv::divide(e.alpha, e.a, dst );
    وإلا إذا( e.flags == '&' && e.b.داده )
        bitwise_and(e.a, e.b, dst);
    وإلا إذا( e.flags == '&' && !e.b.داده )
        bitwise_and(e.a, e.s, dst);
    وإلا إذا( e.flags == '|' && e.b.داده )
        bitwise_or(e.a, e.b, dst);
    وإلا إذا( e.flags == '|' && !e.b.داده )
        bitwise_or(e.a, e.s, dst);
    وإلا إذا( e.flags == '^' && e.b.داده )
        bitwise_xor(e.a, e.b, dst);
    وإلا إذا( e.flags == '^' && !e.b.داده )
        bitwise_xor(e.a, e.s, dst);
    وإلا إذا( e.flags == '~' && !e.b.داده )
        bitwise_not(e.a, dst);
    وإلا إذا( e.flags == 'm' )
        cv::min(e.a, e.b, dst);
    وإلا إذا( e.flags == 'n' )
        cv::min(e.a, e.s[0], dst);
    وإلا إذا( e.flags == 'M' )
        cv::max(e.a, e.b, dst);
    وإلا إذا( e.flags == 'N' )
        cv::max(e.a, e.s[0], dst);
    وإلا إذا( e.flags == 'a' && e.b.داده )
        cv::absdiff(e.a, e.b, dst);
    وإلا إذا( e.flags == 'a' && !e.b.داده )
        cv::absdiff(e.a, e.s, dst);
    وإلا
        CV_Error(cv::Error::StsError, "Unknown operation");

    إذا( dst.داده != m.داده )
        dst.convert_مقصد(m, _type);
}

فراغ MatOp_Bin::multiply(ثابت Mat_عبارت& e, مزدوج s, Mat_عبارت& res) ثابت
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

فراغ MatOp_Bin::divide(مزدوج s, ثابت Mat_عبارت& e, Mat_عبارت& res) ثابت
{
    CV_INSTRUMENT_REGION();

    إذا( e.flags == '/' && (!e.b.داده || e.beta == 0) )
        Mat_Op_جمع_کن_Ex::make_عبارت(res, e.a, Mat(), s/e.alpha, 0);
    وإلا
        MatOp::divide(s, e, res);
}

مضمن فراغ MatOp_Bin::make_عبارت(Mat_عبارت& res, حرف op, ثابت Mat& a, ثابت Mat& b, مزدوج scale)
{
    res = Mat_عبارت(&g_MatOp_Bin, op, a, b, Mat(), scale, b.داده ? 1 : 0);
}

مضمن فراغ MatOp_Bin::make_عبارت(Mat_عبارت& res, حرف op, ثابت Mat& a, ثابت Scalar& s)
{
    res = Mat_عبارت(&g_MatOp_Bin, op, a, Mat(), Mat(), 1, 0, s);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

فراغ MatOp_Cmp::assign(ثابت Mat_عبارت& e, Mat& m, صحيح _type) ثابت
{
    Mat موقت_2, &dst = _type == -1 || _type == CV_8U ? m : موقت_2;

    إذا( e.b.داده )
        cv::compare(e.a, e.b, dst, e.flags);
    وإلا
        cv::compare(e.a, e.alpha, dst, e.flags);

    إذا( dst.داده != m.داده )
        dst.convert_مقصد(m, _type);
}

مضمن فراغ MatOp_Cmp::make_عبارت(Mat_عبارت& res, صحيح cmpop, ثابت Mat& a, ثابت Mat& b)
{
    res = Mat_عبارت(&g_MatOp_Cmp, cmpop, a, b, Mat(), 1, 1);
}

مضمن فراغ MatOp_Cmp::make_عبارت(Mat_عبارت& res, صحيح cmpop, ثابت Mat& a, مزدوج alpha)
{
    res = Mat_عبارت(&g_MatOp_Cmp, cmpop, a, Mat(), Mat(), alpha, 1);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

فراغ MatOp_T::assign(ثابت Mat_عبارت& e, Mat& m, صحيح _type) ثابت
{
    Mat موقت_2, &dst = _type == -1 || _type == e.a.type() ? m : موقت_2;

    cv::transpose(e.a, dst);

    إذا( dst.داده != m.داده || e.alpha != 1 )
        dst.convert_مقصد(m, _type, e.alpha);
}

فراغ MatOp_T::multiply(ثابت Mat_عبارت& e, مزدوج s, Mat_عبارت& res) ثابت
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha *= s;
}

فراغ MatOp_T::transpose(ثابت Mat_عبارت& e, Mat_عبارت& res) ثابت
{
    CV_INSTRUMENT_REGION();

    إذا( e.alpha == 1 )
        Mat_Op_عنصر_خنثی::make_عبارت(res, e.a);
    وإلا
        Mat_Op_جمع_کن_Ex::make_عبارت(res, e.a, Mat(), e.alpha, 0);
}

مضمن فراغ MatOp_T::make_عبارت(Mat_عبارت& res, ثابت Mat& a, مزدوج alpha)
{
    res = Mat_عبارت(&g_MatOp_T, 0, a, Mat(), Mat(), alpha, 0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

فراغ MatOp_GEMM::assign(ثابت Mat_عبارت& e, Mat& m, صحيح _type) ثابت
{
    Mat موقت_2, &dst = _type == -1 || _type == e.a.type() ? m : موقت_2;

    cv::gemm(e.a, e.b, e.alpha, e.c, e.beta, dst, e.flags);
    إذا( dst.داده != m.داده )
        dst.convert_مقصد(m, _type);
}

فراغ MatOp_GEMM::جمع_کن(ثابت Mat_عبارت& e1, ثابت Mat_عبارت& e2, Mat_عبارت& res) ثابت
{
    CV_INSTRUMENT_REGION();

    منطقي i1 = is_عنصر_خنثی(e1), i2 = is_عنصر_خنثی(e2);
    مزدوج alpha1 = i1 ? 1 : e1.alpha, alpha2 = i2 ? 1 : e2.alpha;

    إذا( isMatProd(e1) && (i2 || isScaled(e2) || isT(e2)) )
        MatOp_GEMM::make_عبارت(res, (e1.flags & ~cv::GEMM_3_T)|(isT(e2) ? cv::GEMM_3_T : 0),
                             e1.a, e1.b, alpha1, e2.a, alpha2);
    وإلا إذا( isMatProd(e2) && (i1 || isScaled(e1) || isT(e1)) )
        MatOp_GEMM::make_عبارت(res, (e2.flags & ~cv::GEMM_3_T)|(isT(e1) ? cv::GEMM_3_T : 0),
                             e2.a, e2.b, alpha2, e1.a, alpha1);
    وإلا إذا( هذا == e2.op )
        MatOp::جمع_کن(e1, e2, res);
    وإلا
        e2.op->جمع_کن(e1, e2, res);
}

فراغ MatOp_GEMM::تفریق(ثابت Mat_عبارت& e1, ثابت Mat_عبارت& e2, Mat_عبارت& res) ثابت
{
    CV_INSTRUMENT_REGION();

    منطقي i1 = is_عنصر_خنثی(e1), i2 = is_عنصر_خنثی(e2);
    مزدوج alpha1 = i1 ? 1 : e1.alpha, alpha2 = i2 ? 1 : e2.alpha;

    إذا( isMatProd(e1) && (i2 || isScaled(e2) || isT(e2)) )
        MatOp_GEMM::make_عبارت(res, (e1.flags & ~cv::GEMM_3_T)|(isT(e2) ? cv::GEMM_3_T : 0),
                             e1.a, e1.b, alpha1, e2.a, -alpha2);
    وإلا إذا( isMatProd(e2) && (i1 || isScaled(e1) || isT(e1)) )
        MatOp_GEMM::make_عبارت(res, (e2.flags & ~cv::GEMM_3_T)|(isT(e1) ? cv::GEMM_3_T : 0),
                            e2.a, e2.b, -alpha2, e1.a, alpha1);
    وإلا إذا( هذا == e2.op )
        MatOp::تفریق(e1, e2, res);
    وإلا
        e2.op->تفریق(e1, e2, res);
}

فراغ MatOp_GEMM::multiply(ثابت Mat_عبارت& e, مزدوج s, Mat_عبارت& res) ثابت
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha *= s;
    res.beta *= s;
}

فراغ MatOp_GEMM::transpose(ثابت Mat_عبارت& e, Mat_عبارت& res) ثابت
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.flags = (!(e.flags & cv::GEMM_1_T) ? cv::GEMM_2_T : 0) |
                (!(e.flags & cv::GEMM_2_T) ? cv::GEMM_1_T : 0) |
                (!(e.flags & cv::GEMM_3_T) ? cv::GEMM_3_T : 0);
    swap(res.a, res.b);
}

مضمن فراغ MatOp_GEMM::make_عبارت(Mat_عبارت& res, صحيح flags, ثابت Mat& a, ثابت Mat& b,
                                 مزدوج alpha, ثابت Mat& c, مزدوج beta)
{
    res = Mat_عبارت(&g_MatOp_GEMM, flags, a, b, c, alpha, beta);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

فراغ MatOp_Invert::assign(ثابت Mat_عبارت& e, Mat& m, صحيح _type) ثابت
{
    Mat موقت_2, &dst = _type == -1 || _type == e.a.type() ? m : موقت_2;

    cv::invert(e.a, dst, e.flags);
    إذا( dst.داده != m.داده )
        dst.convert_مقصد(m, _type);
}

فراغ MatOp_Invert::matmul(ثابت Mat_عبارت& e1, ثابت Mat_عبارت& e2, Mat_عبارت& res) ثابت
{
    إذا( isInv(e1) && is_عنصر_خنثی(e2) )
        MatOp_Solve::make_عبارت(res, e1.flags, e1.a, e2.a);
    وإلا إذا( هذا == e2.op )
        MatOp::matmul(e1, e2, res);
    وإلا
        e2.op->matmul(e1, e2, res);
}

مضمن فراغ MatOp_Invert::make_عبارت(Mat_عبارت& res, صحيح روش, ثابت Mat& m)
{
    res = Mat_عبارت(&g_MatOp_Invert, روش, m, Mat(), Mat(), 1, 0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

فراغ MatOp_Solve::assign(ثابت Mat_عبارت& e, Mat& m, صحيح _type) ثابت
{
    Mat موقت_2, &dst = _type == -1 || _type == e.a.type() ? m : موقت_2;

    cv::solve(e.a, e.b, dst, e.flags);
    إذا( dst.داده != m.داده )
        dst.convert_مقصد(m, _type);
}

مضمن فراغ MatOp_Solve::make_عبارت(Mat_عبارت& res, صحيح روش, ثابت Mat& a, ثابت Mat& b)
{
    res = Mat_عبارت(&g_MatOp_Solve, روش, a, b, Mat(), 1, 1);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

فراغ MatOp_Initializer::assign(ثابت Mat_عبارت& e, Mat& m, صحيح _type) ثابت
{
    إذا( _type == -1 )
        _type = e.a.type();

    m.create(e.a.size, _type);

    إذا( e.flags == 'I' && e.a.dims <= 2 )
        مجموعه_عنصر_خنثی(m, Scalar(e.alpha));
    وإلا إذا( e.flags == '0' )
        m = Scalar();
    وإلا إذا( e.flags == '1' )
        m = Scalar(e.alpha);
    وإلا
        CV_Error(cv::Error::StsError, "Invalid matrix initializer type");
}

فراغ MatOp_Initializer::multiply(ثابت Mat_عبارت& e, مزدوج s, Mat_عبارت& res) ثابت
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha *= s;
}

مضمن فراغ MatOp_Initializer::make_عبارت(Mat_عبارت& res, صحيح روش, اندازه sz, صحيح type, مزدوج alpha)
{
    res = Mat_عبارت(getGlobalMatOpInitializer(), روش, Mat(sz, type, (فراغ*)(size_t)0xEEEEEEEE), Mat(), Mat(), alpha, 0);
}

مضمن فراغ MatOp_Initializer::make_عبارت(Mat_عبارت& res, صحيح روش, صحيح ndims, ثابت صحيح* sizes, صحيح type, مزدوج alpha)
{
    res = Mat_عبارت(getGlobalMatOpInitializer(), روش, Mat(ndims, sizes, type, (فراغ*)(size_t)0xEEEEEEEE), Mat(), Mat(), alpha, 0);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

Mat_عبارت Mat::t() ثابت
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(*هذا);
    Mat_عبارت e;
    MatOp_T::make_عبارت(e, *هذا);
    أرجع e;
}

Mat_عبارت Mat::inv(صحيح روش) ثابت
{
    CV_INSTRUMENT_REGION();

    Mat_عبارت e;
    MatOp_Invert::make_عبارت(e, روش, *هذا);
    أرجع e;
}


Mat_عبارت Mat::mul(درون_داد_Array m, مزدوج scale) ثابت
{
    CV_INSTRUMENT_REGION();

    Mat b = m.getMat();
    // Unless the argument is a refcounted Mat/UMat, the header returned by getMat() may be
    // a non-owning view of caller memory (e.g. a scalar bound to _InputArray(const double&),
    // a Matx or a Vec) that the returned MatExpr can outlive, so snapshot it.
    // See https://github.com/opencv/opencv/issues/23577
    إذا( !m.isMat() && !m.isUMat() )
        b = b.clone();

    Mat_عبارت e;
    MatOp_Bin::make_عبارت(e, '*', *هذا, b, scale);
    أرجع e;
}

Mat_عبارت Mat::zeros(صحيح ردیفها, صحيح تعداد_ستون, صحيح type)
{
    CV_INSTRUMENT_REGION();

    Mat_عبارت e;
    MatOp_Initializer::make_عبارت(e, '0', اندازه(تعداد_ستون, ردیفها), type);
    أرجع e;
}

Mat_عبارت Mat::zeros(اندازه size, صحيح type)
{
    CV_INSTRUMENT_REGION();

    Mat_عبارت e;
    MatOp_Initializer::make_عبارت(e, '0', size, type);
    أرجع e;
}

Mat_عبارت Mat::zeros(صحيح ndims, ثابت صحيح* sizes, صحيح type)
{
    CV_INSTRUMENT_REGION();

    Mat_عبارت e;
    MatOp_Initializer::make_عبارت(e, '0', ndims, sizes, type);
    أرجع e;
}

Mat_عبارت Mat::zeros(ثابت MatShape& shape, صحيح type)
{
    CV_INSTRUMENT_REGION();

    Mat_عبارت e;
    MatOp_Initializer::make_عبارت(e, '0', shape.dims, shape.p, type);
    أرجع e;
}

Mat_عبارت Mat::ones(صحيح ردیفها, صحيح تعداد_ستون, صحيح type)
{
    CV_INSTRUMENT_REGION();

    Mat_عبارت e;
    MatOp_Initializer::make_عبارت(e, '1', اندازه(تعداد_ستون, ردیفها), type);
    أرجع e;
}

Mat_عبارت Mat::ones(اندازه size, صحيح type)
{
    CV_INSTRUMENT_REGION();

    Mat_عبارت e;
    MatOp_Initializer::make_عبارت(e, '1', size, type);
    أرجع e;
}

Mat_عبارت Mat::ones(صحيح ndims, ثابت صحيح* sizes, صحيح type)
{
    CV_INSTRUMENT_REGION();

    Mat_عبارت e;
    MatOp_Initializer::make_عبارت(e, '1', ndims, sizes, type);
    أرجع e;
}

Mat_عبارت Mat::ones(ثابت MatShape& shape, صحيح type)
{
    CV_INSTRUMENT_REGION();

    Mat_عبارت e;
    MatOp_Initializer::make_عبارت(e, '1', shape.dims, shape.p, type);
    أرجع e;
}

Mat_عبارت Mat::eye(صحيح ردیفها, صحيح تعداد_ستون, صحيح type)
{
    CV_INSTRUMENT_REGION();

    Mat_عبارت e;
    MatOp_Initializer::make_عبارت(e, 'I', اندازه(تعداد_ستون, ردیفها), type);
    أرجع e;
}

Mat_عبارت Mat::eye(اندازه size, صحيح type)
{
    CV_INSTRUMENT_REGION();

    Mat_عبارت e;
    MatOp_Initializer::make_عبارت(e, 'I', size, type);
    أرجع e;
}

فراغ Mat_عبارت::swap(Mat_عبارت& other)
{
    استفاده std::swap;

    swap(op, other.op);
    swap(flags, other.flags);

    swap(a, other.a);
    swap(b, other.b);
    swap(c, other.c);

    swap(alpha, other.alpha);
    swap(beta, other.beta);

    swap(s, other.s);
}

درون_داد_Array_2::درون_داد_Array_2(ثابت Mat_عبارت& عبارت)
{
    إذا (!is_عنصر_خنثی(عبارت))
    {
        Mat نتیجه = عبارت;  // TODO improve through refcount == 1 of expr.a (inplace operation is possible - except gemm?)
        Mat_عبارت نتیجه_عبارت(نتیجه);
        swap(const_cast<Mat_عبارت&>(عبارت), نتیجه_عبارت);
    }
    CV_Assert(is_عنصر_خنثی(عبارت));
    init(FIXED_TYPE + FIXED_اندازه + MAT + ACCESS_READ, &عبارت.a);
}

} // cv::
