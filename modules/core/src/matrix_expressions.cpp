// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


/* ////////////////////////////////////////////////////////////////////
//
//  Mat basic operations: Copy, Set
//
// */

#شامل "precomp.hpp"
#شامل <opencv2/core/utils/logger.hpp>

نامخلا cv
{

//This and its overload below are used in various MatExpr operator overloads
//implemented to check that Matrix operands exist.
ساکن خالی checkOperandsExist(ثابت Mat& a)
{
    اگر (a.empty())
    {
        CV_Error(cv::Error::StsBadArg, "Matrix operand is an empty matrix.");
    }
}

ساکن خالی checkOperandsExist(ثابت Mat& a, ثابت Mat& b)
{
    اگر (a.empty() || b.empty())
    {
        CV_Error(cv::Error::StsBadArg, "One or more matrix operands are empty.");
    }
}


جماعت Mat_Op_شناختی_عنصر CV_FINAL : عوامی MatOp
{
عوامی:
    Mat_Op_شناختی_عنصر() {}
    مجازی ~Mat_Op_شناختی_عنصر() {}

    منطقی elementWise(ثابت Mat_اظہاریہ& /*اظہاریہ*/) ثابت CV_OVERRIDE { واپس سچ; }
    خالی assign(ثابت Mat_اظہاریہ& اظہاریہ, Mat& m, صحیح type=-1) ثابت CV_OVERRIDE;

    ساکن خالی make_اظہاریہ(Mat_اظہاریہ& res, ثابت Mat& m);
};

ساکن Mat_Op_شناختی_عنصر g_Mat_Op_شناختی_عنصر;

جماعت Mat_Op_جوڑو_Ex CV_FINAL : عوامی MatOp
{
عوامی:
    Mat_Op_جوڑو_Ex() {}
    مجازی ~Mat_Op_جوڑو_Ex() {}

    منطقی elementWise(ثابت Mat_اظہاریہ& /*اظہاریہ*/) ثابت CV_OVERRIDE { واپس سچ; }
    خالی assign(ثابت Mat_اظہاریہ& اظہاریہ, Mat& m, صحیح type=-1) ثابت CV_OVERRIDE;

    خالی جوڑو(ثابت Mat_اظہاریہ& e1, ثابت Scalar& s, Mat_اظہاریہ& res) ثابت CV_OVERRIDE;
    خالی تفریق(ثابت Scalar& s, ثابت Mat_اظہاریہ& اظہاریہ, Mat_اظہاریہ& res) ثابت CV_OVERRIDE;
    خالی multiply(ثابت Mat_اظہاریہ& e1, دوہرا s, Mat_اظہاریہ& res) ثابت CV_OVERRIDE;
    خالی divide(دوہرا s, ثابت Mat_اظہاریہ& e, Mat_اظہاریہ& res) ثابت CV_OVERRIDE;

    خالی transpose(ثابت Mat_اظہاریہ& e1, Mat_اظہاریہ& res) ثابت CV_OVERRIDE;
    خالی مطلق_قدر(ثابت Mat_اظہاریہ& اظہاریہ, Mat_اظہاریہ& res) ثابت CV_OVERRIDE;

    ساکن خالی make_اظہاریہ(Mat_اظہاریہ& res, ثابت Mat& a, ثابت Mat& b, دوہرا alpha, دوہرا beta, ثابت Scalar& s=Scalar());
};

ساکن Mat_Op_جوڑو_Ex g_Mat_Op_جوڑو_Ex;

جماعت MatOp_Bin CV_FINAL : عوامی MatOp
{
عوامی:
    MatOp_Bin() {}
    مجازی ~MatOp_Bin() {}

    منطقی elementWise(ثابت Mat_اظہاریہ& /*اظہاریہ*/) ثابت CV_OVERRIDE { واپس سچ; }
    خالی assign(ثابت Mat_اظہاریہ& اظہاریہ, Mat& m, صحیح type=-1) ثابت CV_OVERRIDE;

    خالی multiply(ثابت Mat_اظہاریہ& e1, دوہرا s, Mat_اظہاریہ& res) ثابت CV_OVERRIDE;
    خالی divide(دوہرا s, ثابت Mat_اظہاریہ& e, Mat_اظہاریہ& res) ثابت CV_OVERRIDE;

    ساکن خالی make_اظہاریہ(Mat_اظہاریہ& res, حرف op, ثابت Mat& a, ثابت Mat& b, دوہرا scale=1);
    ساکن خالی make_اظہاریہ(Mat_اظہاریہ& res, حرف op, ثابت Mat& a, ثابت Scalar& s);
};

ساکن MatOp_Bin g_MatOp_Bin;

جماعت MatOp_Cmp CV_FINAL : عوامی MatOp
{
عوامی:
    MatOp_Cmp() {}
    مجازی ~MatOp_Cmp() {}

    منطقی elementWise(ثابت Mat_اظہاریہ& /*اظہاریہ*/) ثابت CV_OVERRIDE { واپس سچ; }
    خالی assign(ثابت Mat_اظہاریہ& اظہاریہ, Mat& m, صحیح type=-1) ثابت CV_OVERRIDE;

    ساکن خالی make_اظہاریہ(Mat_اظہاریہ& res, صحیح cmpop, ثابت Mat& a, ثابت Mat& b);
    ساکن خالی make_اظہاریہ(Mat_اظہاریہ& res, صحیح cmpop, ثابت Mat& a, دوہرا alpha);
};

ساکن MatOp_Cmp g_MatOp_Cmp;

جماعت MatOp_GEMM CV_FINAL : عوامی MatOp
{
عوامی:
    MatOp_GEMM() {}
    مجازی ~MatOp_GEMM() {}

    منطقی elementWise(ثابت Mat_اظہاریہ& /*اظہاریہ*/) ثابت CV_OVERRIDE { واپس جھوٹ; }
    خالی assign(ثابت Mat_اظہاریہ& اظہاریہ, Mat& m, صحیح type=-1) ثابت CV_OVERRIDE;

    خالی جوڑو(ثابت Mat_اظہاریہ& e1, ثابت Mat_اظہاریہ& e2, Mat_اظہاریہ& res) ثابت CV_OVERRIDE;
    خالی تفریق(ثابت Mat_اظہاریہ& e1, ثابت Mat_اظہاریہ& e2, Mat_اظہاریہ& res) ثابت CV_OVERRIDE;
    خالی multiply(ثابت Mat_اظہاریہ& e, دوہرا s, Mat_اظہاریہ& res) ثابت CV_OVERRIDE;

    خالی transpose(ثابت Mat_اظہاریہ& اظہاریہ, Mat_اظہاریہ& res) ثابت CV_OVERRIDE;

    سائز size(ثابت Mat_اظہاریہ& اظہاریہ) ثابت CV_OVERRIDE
    {
        واپس سائز(
            (اظہاریہ.flags & GEMM_2_T) ? اظہاریہ.b.صفیں : اظہاریہ.b.ستون_تعداد,
            (اظہاریہ.flags & GEMM_1_T) ? اظہاریہ.a.ستون_تعداد : اظہاریہ.a.صفیں
        );
    }

    ساکن خالی make_اظہاریہ(Mat_اظہاریہ& res, صحیح flags, ثابت Mat& a, ثابت Mat& b,
                         دوہرا alpha=1, ثابت Mat& c=Mat(), دوہرا beta=1);

};

ساکن MatOp_GEMM g_MatOp_GEMM;

جماعت MatOp_Invert CV_FINAL : عوامی MatOp
{
عوامی:
    MatOp_Invert() {}
    مجازی ~MatOp_Invert() {}

    منطقی elementWise(ثابت Mat_اظہاریہ& /*اظہاریہ*/) ثابت CV_OVERRIDE { واپس جھوٹ; }
    خالی assign(ثابت Mat_اظہاریہ& اظہاریہ, Mat& m, صحیح type=-1) ثابت CV_OVERRIDE;

    خالی matmul(ثابت Mat_اظہاریہ& expr1, ثابت Mat_اظہاریہ& expr2, Mat_اظہاریہ& res) ثابت CV_OVERRIDE;

    ساکن خالی make_اظہاریہ(Mat_اظہاریہ& res, صحیح طریقہ, ثابت Mat& m);
};

ساکن MatOp_Invert g_MatOp_Invert;

جماعت MatOp_T CV_FINAL : عوامی MatOp
{
عوامی:
    MatOp_T() {}
    مجازی ~MatOp_T() {}

    منطقی elementWise(ثابت Mat_اظہاریہ& /*اظہاریہ*/) ثابت CV_OVERRIDE { واپس جھوٹ; }
    خالی assign(ثابت Mat_اظہاریہ& اظہاریہ, Mat& m, صحیح type=-1) ثابت CV_OVERRIDE;

    خالی multiply(ثابت Mat_اظہاریہ& e1, دوہرا s, Mat_اظہاریہ& res) ثابت CV_OVERRIDE;
    خالی transpose(ثابت Mat_اظہاریہ& اظہاریہ, Mat_اظہاریہ& res) ثابت CV_OVERRIDE;

    ساکن خالی make_اظہاریہ(Mat_اظہاریہ& res, ثابت Mat& a, دوہرا alpha=1);
};

ساکن MatOp_T g_MatOp_T;

جماعت MatOp_Solve CV_FINAL : عوامی MatOp
{
عوامی:
    MatOp_Solve() {}
    مجازی ~MatOp_Solve() {}

    منطقی elementWise(ثابت Mat_اظہاریہ& /*اظہاریہ*/) ثابت CV_OVERRIDE { واپس جھوٹ; }
    خالی assign(ثابت Mat_اظہاریہ& اظہاریہ, Mat& m, صحیح type=-1) ثابت CV_OVERRIDE;

    ساکن خالی make_اظہاریہ(Mat_اظہاریہ& res, صحیح طریقہ, ثابت Mat& a, ثابت Mat& b);
};

ساکن MatOp_Solve g_MatOp_Solve;

جماعت MatOp_Initializer CV_FINAL : عوامی MatOp
{
عوامی:
    MatOp_Initializer() {}
    مجازی ~MatOp_Initializer() {}

    منطقی elementWise(ثابت Mat_اظہاریہ& /*اظہاریہ*/) ثابت CV_OVERRIDE { واپس جھوٹ; }
    خالی assign(ثابت Mat_اظہاریہ& اظہاریہ, Mat& m, صحیح type=-1) ثابت CV_OVERRIDE;

    خالی multiply(ثابت Mat_اظہاریہ& e, دوہرا s, Mat_اظہاریہ& res) ثابت CV_OVERRIDE;

    ساکن خالی make_اظہاریہ(Mat_اظہاریہ& res, صحیح طریقہ, سائز sz, صحیح type, دوہرا alpha=1);
    ساکن خالی make_اظہاریہ(Mat_اظہاریہ& res, صحیح طریقہ, صحیح ndims, ثابت صحیح* sizes, صحیح type, دوہرا alpha=1);
};

ساکن MatOp_Initializer* getGlobalMatOpInitializer()
{
    CV_SINGLETON_LAZY_INIT(MatOp_Initializer, نیا MatOp_Initializer())
}

ساکن اندرونی منطقی is_شناختی_عنصر(ثابت Mat_اظہاریہ& e) { واپس e.op == &g_Mat_Op_شناختی_عنصر; }
ساکن اندرونی منطقی is_جوڑو_Ex(ثابت Mat_اظہاریہ& e) { واپس e.op == &g_Mat_Op_جوڑو_Ex; }
ساکن اندرونی منطقی isScaled(ثابت Mat_اظہاریہ& e) { واپس is_جوڑو_Ex(e) && (!e.b.ڈیٹا || e.beta == 0) && e.s == Scalar(); }
ساکن اندرونی منطقی isBin(ثابت Mat_اظہاریہ& e, حرف c) { واپس e.op == &g_MatOp_Bin && e.flags == c; }
ساکن اندرونی منطقی isCmp(ثابت Mat_اظہاریہ& e) { واپس e.op == &g_MatOp_Cmp; }
ساکن اندرونی منطقی isReciprocal(ثابت Mat_اظہاریہ& e) { واپس isBin(e,'/') && (!e.b.ڈیٹا || e.beta == 0); }
ساکن اندرونی منطقی isT(ثابت Mat_اظہاریہ& e) { واپس e.op == &g_MatOp_T; }
ساکن اندرونی منطقی isInv(ثابت Mat_اظہاریہ& e) { واپس e.op == &g_MatOp_Invert; }
ساکن اندرونی منطقی isSolve(ثابت Mat_اظہاریہ& e) { واپس e.op == &g_MatOp_Solve; }
//static inline bool isGEMM(const MatExpr& e) { return e.op == &g_MatOp_GEMM; }
ساکن اندرونی منطقی isMatProd(ثابت Mat_اظہاریہ& e) { واپس e.op == &g_MatOp_GEMM && (!e.c.ڈیٹا || e.beta == 0); }
ساکن اندرونی منطقی isInitializer(ثابت Mat_اظہاریہ& e) { واپس e.op == getGlobalMatOpInitializer(); }

/////////////////////////////////////////////////////////////////////////////////////////////////////

MatOp::MatOp() {}
MatOp::~MatOp() {}


منطقی MatOp::elementWise(ثابت Mat_اظہاریہ& /*اظہاریہ*/) ثابت
{
    واپس جھوٹ;
}

خالی MatOp::roi(ثابت Mat_اظہاریہ& اظہاریہ, ثابت Range& صف_Range_2, ثابت Range& ستون_Range_2, Mat_اظہاریہ& e) ثابت
{
    اگر( elementWise(اظہاریہ) )
    {
        e = Mat_اظہاریہ(اظہاریہ.op, اظہاریہ.flags, Mat(), Mat(), Mat(),
                    اظہاریہ.alpha, اظہاریہ.beta, اظہاریہ.s);
        اگر(اظہاریہ.a.ڈیٹا)
            e.a = اظہاریہ.a(صف_Range_2, ستون_Range_2);
        اگر(اظہاریہ.b.ڈیٹا)
            e.b = اظہاریہ.b(صف_Range_2, ستون_Range_2);
        اگر(اظہاریہ.c.ڈیٹا)
            e.c = اظہاریہ.c(صف_Range_2, ستون_Range_2);
    }
    ورنہ
    {
        Mat m;
        اظہاریہ.op->assign(اظہاریہ, m);
        e = Mat_اظہاریہ(&g_Mat_Op_شناختی_عنصر, 0, m(صف_Range_2, ستون_Range_2), Mat(), Mat());
    }
}

خالی MatOp::diag(ثابت Mat_اظہاریہ& اظہاریہ, صحیح d, Mat_اظہاریہ& e) ثابت
{
    اگر( elementWise(اظہاریہ) )
    {
        e = Mat_اظہاریہ(اظہاریہ.op, اظہاریہ.flags, Mat(), Mat(), Mat(),
                    اظہاریہ.alpha, اظہاریہ.beta, اظہاریہ.s);
        اگر(اظہاریہ.a.ڈیٹا)
            e.a = اظہاریہ.a.diag(d);
        اگر(اظہاریہ.b.ڈیٹا)
            e.b = اظہاریہ.b.diag(d);
        اگر(اظہاریہ.c.ڈیٹا)
            e.c = اظہاریہ.c.diag(d);
    }
    ورنہ
    {
        Mat m;
        اظہاریہ.op->assign(اظہاریہ, m);
        e = Mat_اظہاریہ(&g_Mat_Op_شناختی_عنصر, 0, m.diag(d), Mat(), Mat());
    }
}


خالی MatOp::aug_تفویض_جوڑو(ثابت Mat_اظہاریہ& اظہاریہ, Mat& m) ثابت
{
    Mat عارضی_2;
    اظہاریہ.op->assign(اظہاریہ, عارضی_2);
    m += عارضی_2;
}


خالی MatOp::aug_تفویض_تفریق(ثابت Mat_اظہاریہ& اظہاریہ, Mat& m) ثابت
{
    Mat عارضی_2;
    اظہاریہ.op->assign(اظہاریہ, عارضی_2);
    m -= عارضی_2;
}


خالی MatOp::aug_تفویض_Multiply(ثابت Mat_اظہاریہ& اظہاریہ, Mat& m) ثابت
{
    Mat عارضی_2;
    اظہاریہ.op->assign(اظہاریہ, عارضی_2);
    m *= عارضی_2;
}


خالی MatOp::aug_تفویض_Divide(ثابت Mat_اظہاریہ& اظہاریہ, Mat& m) ثابت
{
    Mat عارضی_2;
    اظہاریہ.op->assign(اظہاریہ, عارضی_2);
    m /= عارضی_2;
}


خالی MatOp::aug_تفویض_And(ثابت Mat_اظہاریہ& اظہاریہ, Mat& m) ثابت
{
    Mat عارضی_2;
    اظہاریہ.op->assign(اظہاریہ, عارضی_2);
    m &= عارضی_2;
}


خالی MatOp::aug_تفویض_Or(ثابت Mat_اظہاریہ& اظہاریہ, Mat& m) ثابت
{
    Mat عارضی_2;
    اظہاریہ.op->assign(اظہاریہ, عارضی_2);
    m |= عارضی_2;
}


خالی MatOp::aug_تفویض_Xor(ثابت Mat_اظہاریہ& اظہاریہ, Mat& m) ثابت
{
    Mat عارضی_2;
    اظہاریہ.op->assign(اظہاریہ, عارضی_2);
    m ^= عارضی_2;
}


خالی MatOp::جوڑو(ثابت Mat_اظہاریہ& e1, ثابت Mat_اظہاریہ& e2, Mat_اظہاریہ& res) ثابت
{
    CV_INSTRUMENT_REGION();

    اگر( یہ == e2.op )
    {
        دوہرا alpha = 1, beta = 1;
        Scalar s;
        Mat m1, m2;
        اگر( is_جوڑو_Ex(e1) && (!e1.b.ڈیٹا || e1.beta == 0) )
        {
            m1 = e1.a;
            alpha = e1.alpha;
            s = e1.s;
        }
        ورنہ
            e1.op->assign(e1, m1);

        اگر( is_جوڑو_Ex(e2) && (!e2.b.ڈیٹا || e2.beta == 0) )
        {
            m2 = e2.a;
            beta = e2.alpha;
            s += e2.s;
        }
        ورنہ
            e2.op->assign(e2, m2);
        Mat_Op_جوڑو_Ex::make_اظہاریہ(res, m1, m2, alpha, beta, s);
    }
    ورنہ
        e2.op->جوڑو(e1, e2, res);
}


خالی MatOp::جوڑو(ثابت Mat_اظہاریہ& expr1, ثابت Scalar& s, Mat_اظہاریہ& res) ثابت
{
    CV_INSTRUMENT_REGION();

    Mat m1;
    expr1.op->assign(expr1, m1);
    Mat_Op_جوڑو_Ex::make_اظہاریہ(res, m1, Mat(), 1, 0, s);
}


خالی MatOp::تفریق(ثابت Mat_اظہاریہ& e1, ثابت Mat_اظہاریہ& e2, Mat_اظہاریہ& res) ثابت
{
    CV_INSTRUMENT_REGION();

    اگر( یہ == e2.op )
    {
        دوہرا alpha = 1, beta = -1;
        Scalar s;
        Mat m1, m2;
        اگر( is_جوڑو_Ex(e1) && (!e1.b.ڈیٹا || e1.beta == 0) )
        {
            m1 = e1.a;
            alpha = e1.alpha;
            s = e1.s;
        }
        ورنہ
            e1.op->assign(e1, m1);

        اگر( is_جوڑو_Ex(e2) && (!e2.b.ڈیٹا || e2.beta == 0) )
        {
            m2 = e2.a;
            beta = -e2.alpha;
            s -= e2.s;
        }
        ورنہ
            e2.op->assign(e2, m2);
        Mat_Op_جوڑو_Ex::make_اظہاریہ(res, m1, m2, alpha, beta, s);
    }
    ورنہ
        e2.op->تفریق(e1, e2, res);
}


خالی MatOp::تفریق(ثابت Scalar& s, ثابت Mat_اظہاریہ& اظہاریہ, Mat_اظہاریہ& res) ثابت
{
    CV_INSTRUMENT_REGION();

    Mat m;
    اظہاریہ.op->assign(اظہاریہ, m);
    Mat_Op_جوڑو_Ex::make_اظہاریہ(res, m, Mat(), -1, 0, s);
}


خالی MatOp::multiply(ثابت Mat_اظہاریہ& e1, ثابت Mat_اظہاریہ& e2, Mat_اظہاریہ& res, دوہرا scale) ثابت
{
    CV_INSTRUMENT_REGION();

    اگر( یہ == e2.op )
    {
        Mat m1, m2;

        اگر( isReciprocal(e1) )
        {
            اگر( isScaled(e2) )
            {
                scale *= e2.alpha;
                m2 = e2.a;
            }
            ورنہ
                e2.op->assign(e2, m2);

            MatOp_Bin::make_اظہاریہ(res, '/', m2, e1.a, scale/e1.alpha);
        }
        ورنہ
        {
            حرف op = '*';
            اگر( isScaled(e1) )
            {
                m1 = e1.a;
                scale *= e1.alpha;
            }
            ورنہ
                e1.op->assign(e1, m1);

            اگر( isScaled(e2) )
            {
                m2 = e2.a;
                scale *= e2.alpha;
            }
            ورنہ اگر( isReciprocal(e2) )
            {
                op = '/';
                m2 = e2.a;
                scale *= e2.alpha;
            }
            ورنہ
                e2.op->assign(e2, m2);

            MatOp_Bin::make_اظہاریہ(res, op, m1, m2, scale);
        }
    }
    ورنہ
        e2.op->multiply(e1, e2, res, scale);
}


خالی MatOp::multiply(ثابت Mat_اظہاریہ& اظہاریہ, دوہرا s, Mat_اظہاریہ& res) ثابت
{
    CV_INSTRUMENT_REGION();

    Mat m;
    اظہاریہ.op->assign(اظہاریہ, m);
    Mat_Op_جوڑو_Ex::make_اظہاریہ(res, m, Mat(), s, 0);
}


خالی MatOp::divide(ثابت Mat_اظہاریہ& e1, ثابت Mat_اظہاریہ& e2, Mat_اظہاریہ& res, دوہرا scale) ثابت
{
    CV_INSTRUMENT_REGION();

    اگر( یہ == e2.op )
    {
        اگر( isReciprocal(e1) && isReciprocal(e2) )
            MatOp_Bin::make_اظہاریہ(res, '/', e2.a, e1.a, e1.alpha/e2.alpha);
        ورنہ
        {
            Mat m1, m2;
            حرف op = '/';

            اگر( isScaled(e1) )
            {
                m1 = e1.a;
                scale *= e1.alpha;
            }
            ورنہ
                e1.op->assign(e1, m1);

            اگر( isScaled(e2) )
            {
                m2 = e2.a;
                scale /= e2.alpha;
            }
            ورنہ اگر( isReciprocal(e2) )
            {
                m2 = e2.a;
                scale /= e2.alpha;
                op = '*';
            }
            ورنہ
                e2.op->assign(e2, m2);
            MatOp_Bin::make_اظہاریہ(res, op, m1, m2, scale);
        }
    }
    ورنہ
        e2.op->divide(e1, e2, res, scale);
}


خالی MatOp::divide(دوہرا s, ثابت Mat_اظہاریہ& اظہاریہ, Mat_اظہاریہ& res) ثابت
{
    CV_INSTRUMENT_REGION();

    Mat m;
    اظہاریہ.op->assign(اظہاریہ, m);
    MatOp_Bin::make_اظہاریہ(res, '/', m, Mat(), s);
}


خالی MatOp::مطلق_قدر(ثابت Mat_اظہاریہ& اظہاریہ, Mat_اظہاریہ& res) ثابت
{
    CV_INSTRUMENT_REGION();

    Mat m;
    اظہاریہ.op->assign(اظہاریہ, m);
    MatOp_Bin::make_اظہاریہ(res, 'a', m, Mat());
}


خالی MatOp::transpose(ثابت Mat_اظہاریہ& اظہاریہ, Mat_اظہاریہ& res) ثابت
{
    CV_INSTRUMENT_REGION();

    Mat m;
    اظہاریہ.op->assign(اظہاریہ, m);
    MatOp_T::make_اظہاریہ(res, m, 1);
}


خالی MatOp::matmul(ثابت Mat_اظہاریہ& e1, ثابت Mat_اظہاریہ& e2, Mat_اظہاریہ& res) ثابت
{
    اگر( یہ == e2.op )
    {
        دوہرا scale = 1;
        صحیح flags = 0;
        Mat m1, m2;

        اگر( isT(e1) )
        {
            flags = cv::GEMM_1_T;
            scale = e1.alpha;
            m1 = e1.a;
        }
        ورنہ اگر( isScaled(e1) )
        {
            scale = e1.alpha;
            m1 = e1.a;
        }
        ورنہ
            e1.op->assign(e1, m1);

        اگر( isT(e2) )
        {
            flags |= cv::GEMM_2_T;
            scale *= e2.alpha;
            m2 = e2.a;
        }
        ورنہ اگر( isScaled(e2) )
        {
            scale *= e2.alpha;
            m2 = e2.a;
        }
        ورنہ
            e2.op->assign(e2, m2);

        MatOp_GEMM::make_اظہاریہ(res, flags, m1, m2, scale);
    }
    ورنہ
        e2.op->matmul(e1, e2, res);
}


خالی MatOp::invert(ثابت Mat_اظہاریہ& اظہاریہ, صحیح طریقہ, Mat_اظہاریہ& res) ثابت
{
    Mat m;
    اظہاریہ.op->assign(اظہاریہ, m);
    MatOp_Invert::make_اظہاریہ(res, طریقہ, m);
}


سائز MatOp::size(ثابت Mat_اظہاریہ& اظہاریہ) ثابت
{
    واپس !اظہاریہ.a.empty() ? اظہاریہ.a.size() : اظہاریہ.b.empty() ? اظہاریہ.b.size() : اظہاریہ.c.size();
}

صحیح MatOp::type(ثابت Mat_اظہاریہ& اظہاریہ) ثابت
{
    CV_INSTRUMENT_REGION();

    واپس !اظہاریہ.a.empty() ? اظہاریہ.a.type() : اظہاریہ.b.empty() ? اظہاریہ.b.type() : اظہاریہ.c.type();
}

//////////////////////////////////////////////////////////////////////////////////////////////////

Mat_اظہاریہ::Mat_اظہاریہ(ثابت Mat& m) : op(&g_Mat_Op_شناختی_عنصر), flags(0), a(m), b(Mat()), c(Mat()), alpha(1), beta(0), s(Scalar())
{
}

Mat_اظہاریہ Mat_اظہاریہ::صف(صحیح y) ثابت
{
    Mat_اظہاریہ e;
    op->roi(*یہ, Range(y, y+1), Range::all(), e);
    واپس e;
}

Mat_اظہاریہ Mat_اظہاریہ::ستون(صحیح x) ثابت
{
    Mat_اظہاریہ e;
    op->roi(*یہ, Range::all(), Range(x, x+1), e);
    واپس e;
}

Mat_اظہاریہ Mat_اظہاریہ::diag(صحیح d) ثابت
{
    Mat_اظہاریہ e;
    op->diag(*یہ, d, e);
    واپس e;
}

Mat_اظہاریہ Mat_اظہاریہ::عامل()( ثابت Range& صف_Range_2, ثابت Range& ستون_Range_2 ) ثابت
{
    Mat_اظہاریہ e;
    op->roi(*یہ, صف_Range_2, ستون_Range_2, e);
    واپس e;
}

Mat_اظہاریہ Mat_اظہاریہ::عامل()( ثابت Rect& roi ) ثابت
{
    Mat_اظہاریہ e;
    op->roi(*یہ, Range(roi.y, roi.y + roi.اونچائی), Range(roi.x, roi.x + roi.چوڑائی), e);
    واپس e;
}

Mat Mat_اظہاریہ::cross(ثابت Mat& m) ثابت
{
    واپس ((Mat)*یہ).cross(m);
}

دوہرا Mat_اظہاریہ::dot(ثابت Mat& m) ثابت
{
    واپس ((Mat)*یہ).dot(m);
}

Mat_اظہاریہ Mat_اظہاریہ::t() ثابت
{
    Mat_اظہاریہ e;
    op->transpose(*یہ, e);
    واپس e;
}

Mat_اظہاریہ Mat_اظہاریہ::inv(صحیح طریقہ) ثابت
{
    Mat_اظہاریہ e;
    op->invert(*یہ, طریقہ, e);
    واپس e;
}

Mat_اظہاریہ Mat_اظہاریہ::mul(ثابت Mat_اظہاریہ& e, دوہرا scale) ثابت
{
    Mat_اظہاریہ en;
    op->multiply(*یہ, e, en, scale);
    واپس en;
}

Mat_اظہاریہ Mat_اظہاریہ::mul(ثابت Mat& m, دوہرا scale) ثابت
{
    Mat_اظہاریہ e;
    op->multiply(*یہ, Mat_اظہاریہ(m), e, scale);
    واپس e;
}

Mat_اظہاریہ عامل + (ثابت Mat& a, ثابت Mat& b)
{
    checkOperandsExist(a, b);
    Mat_اظہاریہ e;
    Mat_Op_جوڑو_Ex::make_اظہاریہ(e, a, b, 1, 1);
    واپس e;
}

Mat_اظہاریہ عامل + (ثابت Mat& a, ثابت Scalar& s)
{
    checkOperandsExist(a);
    Mat_اظہاریہ e;
    Mat_Op_جوڑو_Ex::make_اظہاریہ(e, a, Mat(), 1, 0, s);
    واپس e;
}

Mat_اظہاریہ عامل + (ثابت Scalar& s, ثابت Mat& a)
{
    checkOperandsExist(a);
    Mat_اظہاریہ e;
    Mat_Op_جوڑو_Ex::make_اظہاریہ(e, a, Mat(), 1, 0, s);
    واپس e;
}

Mat_اظہاریہ عامل + (ثابت Mat_اظہاریہ& e, ثابت Mat& m)
{
    Mat_اظہاریہ en;
    e.op->جوڑو(e, Mat_اظہاریہ(m), en);
    واپس en;
}

Mat_اظہاریہ عامل + (ثابت Mat& m, ثابت Mat_اظہاریہ& e)
{
    checkOperandsExist(m);
    Mat_اظہاریہ en;
    e.op->جوڑو(e, Mat_اظہاریہ(m), en);
    واپس en;
}

Mat_اظہاریہ عامل + (ثابت Mat_اظہاریہ& e, ثابت Scalar& s)
{
    Mat_اظہاریہ en;
    e.op->جوڑو(e, s, en);
    واپس en;
}

Mat_اظہاریہ عامل + (ثابت Scalar& s, ثابت Mat_اظہاریہ& e)
{
    Mat_اظہاریہ en;
    e.op->جوڑو(e, s, en);
    واپس en;
}

Mat_اظہاریہ عامل + (ثابت Mat_اظہاریہ& e1, ثابت Mat_اظہاریہ& e2)
{
    Mat_اظہاریہ en;
    e1.op->جوڑو(e1, e2, en);
    واپس en;
}

Mat_اظہاریہ عامل - (ثابت Mat& a, ثابت Mat& b)
{
    checkOperandsExist(a, b);
    Mat_اظہاریہ e;
    Mat_Op_جوڑو_Ex::make_اظہاریہ(e, a, b, 1, -1);
    واپس e;
}

Mat_اظہاریہ عامل - (ثابت Mat& a, ثابت Scalar& s)
{
    checkOperandsExist(a);
    Mat_اظہاریہ e;
    Mat_Op_جوڑو_Ex::make_اظہاریہ(e, a, Mat(), 1, 0, -s);
    واپس e;
}

Mat_اظہاریہ عامل - (ثابت Scalar& s, ثابت Mat& a)
{
    checkOperandsExist(a);
    Mat_اظہاریہ e;
    Mat_Op_جوڑو_Ex::make_اظہاریہ(e, a, Mat(), -1, 0, s);
    واپس e;
}

Mat_اظہاریہ عامل - (ثابت Mat_اظہاریہ& e, ثابت Mat& m)
{
    checkOperandsExist(m);
    Mat_اظہاریہ en;
    e.op->تفریق(e, Mat_اظہاریہ(m), en);
    واپس en;
}

Mat_اظہاریہ عامل - (ثابت Mat& m, ثابت Mat_اظہاریہ& e)
{
    checkOperandsExist(m);
    Mat_اظہاریہ en;
    e.op->تفریق(Mat_اظہاریہ(m), e, en);
    واپس en;
}

Mat_اظہاریہ عامل - (ثابت Mat_اظہاریہ& e, ثابت Scalar& s)
{
    Mat_اظہاریہ en;
    e.op->جوڑو(e, -s, en);
    واپس en;
}

Mat_اظہاریہ عامل - (ثابت Scalar& s, ثابت Mat_اظہاریہ& e)
{
    Mat_اظہاریہ en;
    e.op->تفریق(s, e, en);
    واپس en;
}

Mat_اظہاریہ عامل - (ثابت Mat_اظہاریہ& e1, ثابت Mat_اظہاریہ& e2)
{
    Mat_اظہاریہ en;
    e1.op->تفریق(e1, e2, en);
    واپس en;
}

Mat_اظہاریہ عامل - (ثابت Mat& m)
{
    checkOperandsExist(m);
    Mat_اظہاریہ e;
    Mat_Op_جوڑو_Ex::make_اظہاریہ(e, m, Mat(), -1, 0);
    واپس e;
}

Mat_اظہاریہ عامل - (ثابت Mat_اظہاریہ& e)
{
    Mat_اظہاریہ en;
    e.op->تفریق(Scalar(0), e, en);
    واپس en;
}

Mat_اظہاریہ عامل * (ثابت Mat& a, ثابت Mat& b)
{
    checkOperandsExist(a, b);
    Mat_اظہاریہ e;
    MatOp_GEMM::make_اظہاریہ(e, 0, a, b);
    واپس e;
}

Mat_اظہاریہ عامل * (ثابت Mat& a, دوہرا s)
{
    checkOperandsExist(a);
    Mat_اظہاریہ e;
    Mat_Op_جوڑو_Ex::make_اظہاریہ(e, a, Mat(), s, 0);
    واپس e;
}

Mat_اظہاریہ عامل * (دوہرا s, ثابت Mat& a)
{
    checkOperandsExist(a);
    Mat_اظہاریہ e;
    Mat_Op_جوڑو_Ex::make_اظہاریہ(e, a, Mat(), s, 0);
    واپس e;
}

Mat_اظہاریہ عامل * (ثابت Mat_اظہاریہ& e, ثابت Mat& m)
{
    Mat_اظہاریہ en;
    e.op->matmul(e, Mat_اظہاریہ(m), en);
    واپس en;
}

Mat_اظہاریہ عامل * (ثابت Mat& m, ثابت Mat_اظہاریہ& e)
{
    checkOperandsExist(m);
    Mat_اظہاریہ en;
    e.op->matmul(Mat_اظہاریہ(m), e, en);
    واپس en;
}

Mat_اظہاریہ عامل * (ثابت Mat_اظہاریہ& e, دوہرا s)
{
    Mat_اظہاریہ en;
    e.op->multiply(e, s, en);
    واپس en;
}

Mat_اظہاریہ عامل * (دوہرا s, ثابت Mat_اظہاریہ& e)
{
    Mat_اظہاریہ en;
    e.op->multiply(e, s, en);
    واپس en;
}

Mat_اظہاریہ عامل * (ثابت Mat_اظہاریہ& e1, ثابت Mat_اظہاریہ& e2)
{
    Mat_اظہاریہ en;
    e1.op->matmul(e1, e2, en);
    واپس en;
}

Mat_اظہاریہ عامل / (ثابت Mat& a, ثابت Mat& b)
{
    checkOperandsExist(a, b);
    Mat_اظہاریہ e;
    MatOp_Bin::make_اظہاریہ(e, '/', a, b);
    واپس e;
}

Mat_اظہاریہ عامل / (ثابت Mat& a, دوہرا s)
{
    checkOperandsExist(a);
    Mat_اظہاریہ e;
    Mat_Op_جوڑو_Ex::make_اظہاریہ(e, a, Mat(), 1./s, 0);
    واپس e;
}

Mat_اظہاریہ عامل / (دوہرا s, ثابت Mat& a)
{
    checkOperandsExist(a);
    Mat_اظہاریہ e;
    MatOp_Bin::make_اظہاریہ(e, '/', a, Mat(), s);
    واپس e;
}

Mat_اظہاریہ عامل / (ثابت Mat_اظہاریہ& e, ثابت Mat& m)
{
    Mat_اظہاریہ en;
    e.op->divide(e, Mat_اظہاریہ(m), en);
    واپس en;
}

Mat_اظہاریہ عامل / (ثابت Mat& m, ثابت Mat_اظہاریہ& e)
{
    checkOperandsExist(m);
    Mat_اظہاریہ en;
    e.op->divide(Mat_اظہاریہ(m), e, en);
    واپس en;
}

Mat_اظہاریہ عامل / (ثابت Mat_اظہاریہ& e, دوہرا s)
{
    Mat_اظہاریہ en;
    e.op->multiply(e, 1./s, en);
    واپس en;
}

Mat_اظہاریہ عامل / (دوہرا s, ثابت Mat_اظہاریہ& e)
{
    Mat_اظہاریہ en;
    e.op->divide(s, e, en);
    واپس en;
}

Mat_اظہاریہ عامل / (ثابت Mat_اظہاریہ& e1, ثابت Mat_اظہاریہ& e2)
{
    Mat_اظہاریہ en;
    e1.op->divide(e1, e2, en);
    واپس en;
}

Mat_اظہاریہ عامل < (ثابت Mat& a, ثابت Mat& b)
{
    checkOperandsExist(a, b);
    Mat_اظہاریہ e;
    MatOp_Cmp::make_اظہاریہ(e, cv::CMP_LT, a, b);
    واپس e;
}

Mat_اظہاریہ عامل < (ثابت Mat& a, دوہرا s)
{
    checkOperandsExist(a);
    Mat_اظہاریہ e;
    MatOp_Cmp::make_اظہاریہ(e, cv::CMP_LT, a, s);
    واپس e;
}

Mat_اظہاریہ عامل < (دوہرا s, ثابت Mat& a)
{
    checkOperandsExist(a);
    Mat_اظہاریہ e;
    MatOp_Cmp::make_اظہاریہ(e, cv::CMP_GT, a, s);
    واپس e;
}

Mat_اظہاریہ عامل <= (ثابت Mat& a, ثابت Mat& b)
{
    checkOperandsExist(a, b);
    Mat_اظہاریہ e;
    MatOp_Cmp::make_اظہاریہ(e, cv::CMP_LE, a, b);
    واپس e;
}

Mat_اظہاریہ عامل <= (ثابت Mat& a, دوہرا s)
{
    checkOperandsExist(a);
    Mat_اظہاریہ e;
    MatOp_Cmp::make_اظہاریہ(e, cv::CMP_LE, a, s);
    واپس e;
}

Mat_اظہاریہ عامل <= (دوہرا s, ثابت Mat& a)
{
    checkOperandsExist(a);
    Mat_اظہاریہ e;
    MatOp_Cmp::make_اظہاریہ(e, cv::CMP_GE, a, s);
    واپس e;
}

Mat_اظہاریہ عامل == (ثابت Mat& a, ثابت Mat& b)
{
    checkOperandsExist(a, b);
    Mat_اظہاریہ e;
    MatOp_Cmp::make_اظہاریہ(e, cv::CMP_EQ, a, b);
    واپس e;
}

Mat_اظہاریہ عامل == (ثابت Mat& a, دوہرا s)
{
    checkOperandsExist(a);
    Mat_اظہاریہ e;
    MatOp_Cmp::make_اظہاریہ(e, cv::CMP_EQ, a, s);
    واپس e;
}

Mat_اظہاریہ عامل == (دوہرا s, ثابت Mat& a)
{
    checkOperandsExist(a);
    Mat_اظہاریہ e;
    MatOp_Cmp::make_اظہاریہ(e, cv::CMP_EQ, a, s);
    واپس e;
}

Mat_اظہاریہ عامل != (ثابت Mat& a, ثابت Mat& b)
{
    checkOperandsExist(a, b);
    Mat_اظہاریہ e;
    MatOp_Cmp::make_اظہاریہ(e, cv::CMP_NE, a, b);
    واپس e;
}

Mat_اظہاریہ عامل != (ثابت Mat& a, دوہرا s)
{
    checkOperandsExist(a);
    Mat_اظہاریہ e;
    MatOp_Cmp::make_اظہاریہ(e, cv::CMP_NE, a, s);
    واپس e;
}

Mat_اظہاریہ عامل != (دوہرا s, ثابت Mat& a)
{
    checkOperandsExist(a);
    Mat_اظہاریہ e;
    MatOp_Cmp::make_اظہاریہ(e, cv::CMP_NE, a, s);
    واپس e;
}

Mat_اظہاریہ عامل >= (ثابت Mat& a, ثابت Mat& b)
{
    checkOperandsExist(a, b);
    Mat_اظہاریہ e;
    MatOp_Cmp::make_اظہاریہ(e, cv::CMP_GE, a, b);
    واپس e;
}

Mat_اظہاریہ عامل >= (ثابت Mat& a, دوہرا s)
{
    checkOperandsExist(a);
    Mat_اظہاریہ e;
    MatOp_Cmp::make_اظہاریہ(e, cv::CMP_GE, a, s);
    واپس e;
}

Mat_اظہاریہ عامل >= (دوہرا s, ثابت Mat& a)
{
    checkOperandsExist(a);
    Mat_اظہاریہ e;
    MatOp_Cmp::make_اظہاریہ(e, cv::CMP_LE, a, s);
    واپس e;
}

Mat_اظہاریہ عامل > (ثابت Mat& a, ثابت Mat& b)
{
    checkOperandsExist(a, b);
    Mat_اظہاریہ e;
    MatOp_Cmp::make_اظہاریہ(e, cv::CMP_GT, a, b);
    واپس e;
}

Mat_اظہاریہ عامل > (ثابت Mat& a, دوہرا s)
{
    checkOperandsExist(a);
    Mat_اظہاریہ e;
    MatOp_Cmp::make_اظہاریہ(e, cv::CMP_GT, a, s);
    واپس e;
}

Mat_اظہاریہ عامل > (دوہرا s, ثابت Mat& a)
{
    checkOperandsExist(a);
    Mat_اظہاریہ e;
    MatOp_Cmp::make_اظہاریہ(e, cv::CMP_LT, a, s);
    واپس e;
}

Mat_اظہاریہ min(ثابت Mat& a, ثابت Mat& b)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a, b);
    Mat_اظہاریہ e;
    MatOp_Bin::make_اظہاریہ(e, 'm', a, b);
    واپس e;
}

Mat_اظہاریہ min(ثابت Mat& a, دوہرا s)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    Mat_اظہاریہ e;
    MatOp_Bin::make_اظہاریہ(e, 'n', a, s);
    واپس e;
}

Mat_اظہاریہ min(دوہرا s, ثابت Mat& a)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    Mat_اظہاریہ e;
    MatOp_Bin::make_اظہاریہ(e, 'n', a, s);
    واپس e;
}

Mat_اظہاریہ max(ثابت Mat& a, ثابت Mat& b)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a, b);
    Mat_اظہاریہ e;
    MatOp_Bin::make_اظہاریہ(e, 'M', a, b);
    واپس e;
}

Mat_اظہاریہ max(ثابت Mat& a, دوہرا s)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    Mat_اظہاریہ e;
    MatOp_Bin::make_اظہاریہ(e, 'N', a, s);
    واپس e;
}

Mat_اظہاریہ max(دوہرا s, ثابت Mat& a)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    Mat_اظہاریہ e;
    MatOp_Bin::make_اظہاریہ(e, 'N', a, s);
    واپس e;
}

Mat_اظہاریہ عامل & (ثابت Mat& a, ثابت Mat& b)
{
    checkOperandsExist(a, b);
    Mat_اظہاریہ e;
    MatOp_Bin::make_اظہاریہ(e, '&', a, b);
    واپس e;
}

Mat_اظہاریہ عامل & (ثابت Mat& a, ثابت Scalar& s)
{
    checkOperandsExist(a);
    Mat_اظہاریہ e;
    MatOp_Bin::make_اظہاریہ(e, '&', a, s);
    واپس e;
}

Mat_اظہاریہ عامل & (ثابت Scalar& s, ثابت Mat& a)
{
    checkOperandsExist(a);
    Mat_اظہاریہ e;
    MatOp_Bin::make_اظہاریہ(e, '&', a, s);
    واپس e;
}

Mat_اظہاریہ عامل | (ثابت Mat& a, ثابت Mat& b)
{
    checkOperandsExist(a, b);
    Mat_اظہاریہ e;
    MatOp_Bin::make_اظہاریہ(e, '|', a, b);
    واپس e;
}

Mat_اظہاریہ عامل | (ثابت Mat& a, ثابت Scalar& s)
{
    checkOperandsExist(a);
    Mat_اظہاریہ e;
    MatOp_Bin::make_اظہاریہ(e, '|', a, s);
    واپس e;
}

Mat_اظہاریہ عامل | (ثابت Scalar& s, ثابت Mat& a)
{
    checkOperandsExist(a);
    Mat_اظہاریہ e;
    MatOp_Bin::make_اظہاریہ(e, '|', a, s);
    واپس e;
}

Mat_اظہاریہ عامل ^ (ثابت Mat& a, ثابت Mat& b)
{
    checkOperandsExist(a, b);
    Mat_اظہاریہ e;
    MatOp_Bin::make_اظہاریہ(e, '^', a, b);
    واپس e;
}

Mat_اظہاریہ عامل ^ (ثابت Mat& a, ثابت Scalar& s)
{
    checkOperandsExist(a);
    Mat_اظہاریہ e;
    MatOp_Bin::make_اظہاریہ(e, '^', a, s);
    واپس e;
}

Mat_اظہاریہ عامل ^ (ثابت Scalar& s, ثابت Mat& a)
{
    checkOperandsExist(a);
    Mat_اظہاریہ e;
    MatOp_Bin::make_اظہاریہ(e, '^', a, s);
    واپس e;
}

Mat_اظہاریہ عامل ~(ثابت Mat& a)
{
    checkOperandsExist(a);
    Mat_اظہاریہ e;
    MatOp_Bin::make_اظہاریہ(e, '~', a, Scalar());
    واپس e;
}

Mat_اظہاریہ مطلق_قدر(ثابت Mat& a)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    Mat_اظہاریہ e;
    MatOp_Bin::make_اظہاریہ(e, 'a', a, Scalar());
    واپس e;
}

Mat_اظہاریہ مطلق_قدر(ثابت Mat_اظہاریہ& e)
{
    CV_INSTRUMENT_REGION();

    Mat_اظہاریہ en;
    e.op->مطلق_قدر(e, en);
    واپس en;
}


سائز Mat_اظہاریہ::size() ثابت
{
    اگر( isT(*یہ) || isInv(*یہ) )
        واپس سائز(a.صفیں, a.ستون_تعداد);
    اگر( isSolve(*یہ) )
        واپس سائز(b.ستون_تعداد, a.ستون_تعداد);
    اگر( isInitializer(*یہ) )
        واپس a.size();
    واپس op ? op->size(*یہ) : سائز();
}


صحیح Mat_اظہاریہ::type() ثابت
{
    CV_INSTRUMENT_REGION();

    اگر( isInitializer(*یہ) )
        واپس a.type();
    اگر( isCmp(*یہ) )
        واپس CV_MAKETYPE(CV_8U, a.channels());
    واپس op ? op->type(*یہ) : -1;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////

خالی Mat_Op_شناختی_عنصر::assign(ثابت Mat_اظہاریہ& e, Mat& m, صحیح _type) ثابت
{
    اگر( _type == -1 || _type == e.a.type() )
        m = e.a;
    ورنہ
    {
        CV_Assert( CV_MAT_CN(_type) == e.a.channels() );
        e.a.convert_مقصد(m, _type);
    }
}

اندرونی خالی Mat_Op_شناختی_عنصر::make_اظہاریہ(Mat_اظہاریہ& res, ثابت Mat& m)
{
    res = Mat_اظہاریہ(&g_Mat_Op_شناختی_عنصر, 0, m, Mat(), Mat(), 1, 0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

خالی Mat_Op_جوڑو_Ex::assign(ثابت Mat_اظہاریہ& e, Mat& m, صحیح _type) ثابت
{
    Mat عارضی_2, &dst = _type == -1 || e.a.type() == _type ? m : عارضی_2;
    اگر( e.b.ڈیٹا )
    {
        اگر( e.s == Scalar() || !e.s.isReal() )
        {
            اگر( e.alpha == 1 )
            {
                اگر( e.beta == 1 )
                    cv::جوڑو(e.a, e.b, dst);
                ورنہ اگر( e.beta == -1 )
                    cv::تفریق(e.a, e.b, dst);
                ورنہ
                    cv::scale_جوڑو(e.b, e.beta, e.a, dst);
            }
            ورنہ اگر( e.beta == 1 )
            {
                اگر( e.alpha == -1 )
                    cv::تفریق(e.b, e.a, dst);
                ورنہ
                    cv::scale_جوڑو(e.a, e.alpha, e.b, dst);
            }
            ورنہ
                cv::جوڑو_Weighted(e.a, e.alpha, e.b, e.beta, 0, dst);

            اگر( !e.s.isReal() )
                cv::جوڑو(dst, e.s, dst);
        }
        ورنہ
        {
            اگر (e.a.channels() > 1)
                CV_LOG_ONCE_WARNING(تہی, "OpenCV/MatExpr: processing of multi-channel arrays might be changed in the future: "
                                          "https://github.com/opencv/opencv/issues/16739");
            cv::جوڑو_Weighted(e.a, e.alpha, e.b, e.beta, e.s[0], dst);
        }
    }
    ورنہ اگر( e.s.isReal() && (dst.ڈیٹا != m.ڈیٹا || fabs(e.alpha) != 1))
    {
        اگر (e.a.channels() > 1 && e.s[0] != 0.0)
            CV_LOG_ONCE_WARNING(تہی, "OpenCV/MatExpr: processing of multi-channel arrays might be changed in the future: "
                                      "https://github.com/opencv/opencv/issues/16739");
        e.a.convert_مقصد(m, _type, e.alpha, e.s[0]);
        واپس;
    }
    ورنہ اگر( e.alpha == 1 )
        cv::جوڑو(e.a, e.s, dst);
    ورنہ اگر( e.alpha == -1 )
        cv::تفریق(e.s, e.a, dst);
    ورنہ
    {
        e.a.convert_مقصد(dst, e.a.type(), e.alpha);
        cv::جوڑو(dst, e.s, dst);
    }

    اگر( dst.ڈیٹا != m.ڈیٹا )
        dst.convert_مقصد(m, m.type());
}


خالی Mat_Op_جوڑو_Ex::جوڑو(ثابت Mat_اظہاریہ& e, ثابت Scalar& s, Mat_اظہاریہ& res) ثابت
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.s += s;
}


خالی Mat_Op_جوڑو_Ex::تفریق(ثابت Scalar& s, ثابت Mat_اظہاریہ& e, Mat_اظہاریہ& res) ثابت
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha = -res.alpha;
    res.beta = -res.beta;
    res.s = s - res.s;
}

خالی Mat_Op_جوڑو_Ex::multiply(ثابت Mat_اظہاریہ& e, دوہرا s, Mat_اظہاریہ& res) ثابت
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha *= s;
    res.beta *= s;
    res.s *= s;
}

خالی Mat_Op_جوڑو_Ex::divide(دوہرا s, ثابت Mat_اظہاریہ& e, Mat_اظہاریہ& res) ثابت
{
    CV_INSTRUMENT_REGION();

    اگر( isScaled(e) )
        MatOp_Bin::make_اظہاریہ(res, '/', e.a, Mat(), s/e.alpha);
    ورنہ
        MatOp::divide(s, e, res);
}


خالی Mat_Op_جوڑو_Ex::transpose(ثابت Mat_اظہاریہ& e, Mat_اظہاریہ& res) ثابت
{
    CV_INSTRUMENT_REGION();

    اگر( isScaled(e) )
        MatOp_T::make_اظہاریہ(res, e.a, e.alpha);
    ورنہ
        MatOp::transpose(e, res);
}

خالی Mat_Op_جوڑو_Ex::مطلق_قدر(ثابت Mat_اظہاریہ& e, Mat_اظہاریہ& res) ثابت
{
    CV_INSTRUMENT_REGION();

    اگر( (!e.b.ڈیٹا || e.beta == 0) && fabs(e.alpha) == 1 )
        MatOp_Bin::make_اظہاریہ(res, 'a', e.a, -e.s*e.alpha);
    ورنہ اگر( e.b.ڈیٹا && e.alpha + e.beta == 0 && e.alpha*e.beta == -1 )
        MatOp_Bin::make_اظہاریہ(res, 'a', e.a, e.b);
    ورنہ
        MatOp::مطلق_قدر(e, res);
}

اندرونی خالی Mat_Op_جوڑو_Ex::make_اظہاریہ(Mat_اظہاریہ& res, ثابت Mat& a, ثابت Mat& b, دوہرا alpha, دوہرا beta, ثابت Scalar& s)
{
    res = Mat_اظہاریہ(&g_Mat_Op_جوڑو_Ex, 0, a, b, Mat(), alpha, beta, s);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

خالی MatOp_Bin::assign(ثابت Mat_اظہاریہ& e, Mat& m, صحیح _type) ثابت
{
    Mat عارضی_2, &dst = _type == -1 || e.a.type() == _type ? m : عارضی_2;

    اگر( e.flags == '*' )
        cv::multiply(e.a, e.b, dst, e.alpha);
    ورنہ اگر( e.flags == '/' && e.b.ڈیٹا )
        cv::divide(e.a, e.b, dst, e.alpha);
    ورنہ اگر( e.flags == '/' && !e.b.ڈیٹا )
        cv::divide(e.alpha, e.a, dst );
    ورنہ اگر( e.flags == '&' && e.b.ڈیٹا )
        bitwise_and(e.a, e.b, dst);
    ورنہ اگر( e.flags == '&' && !e.b.ڈیٹا )
        bitwise_and(e.a, e.s, dst);
    ورنہ اگر( e.flags == '|' && e.b.ڈیٹا )
        bitwise_or(e.a, e.b, dst);
    ورنہ اگر( e.flags == '|' && !e.b.ڈیٹا )
        bitwise_or(e.a, e.s, dst);
    ورنہ اگر( e.flags == '^' && e.b.ڈیٹا )
        bitwise_xor(e.a, e.b, dst);
    ورنہ اگر( e.flags == '^' && !e.b.ڈیٹا )
        bitwise_xor(e.a, e.s, dst);
    ورنہ اگر( e.flags == '~' && !e.b.ڈیٹا )
        bitwise_not(e.a, dst);
    ورنہ اگر( e.flags == 'm' )
        cv::min(e.a, e.b, dst);
    ورنہ اگر( e.flags == 'n' )
        cv::min(e.a, e.s[0], dst);
    ورنہ اگر( e.flags == 'M' )
        cv::max(e.a, e.b, dst);
    ورنہ اگر( e.flags == 'N' )
        cv::max(e.a, e.s[0], dst);
    ورنہ اگر( e.flags == 'a' && e.b.ڈیٹا )
        cv::absdiff(e.a, e.b, dst);
    ورنہ اگر( e.flags == 'a' && !e.b.ڈیٹا )
        cv::absdiff(e.a, e.s, dst);
    ورنہ
        CV_Error(cv::Error::StsError, "Unknown operation");

    اگر( dst.ڈیٹا != m.ڈیٹا )
        dst.convert_مقصد(m, _type);
}

خالی MatOp_Bin::multiply(ثابت Mat_اظہاریہ& e, دوہرا s, Mat_اظہاریہ& res) ثابت
{
    CV_INSTRUMENT_REGION();

    اگر( e.flags == '*' || e.flags == '/' )
    {
        res = e;
        res.alpha *= s;
    }
    ورنہ
        MatOp::multiply(e, s, res);
}

خالی MatOp_Bin::divide(دوہرا s, ثابت Mat_اظہاریہ& e, Mat_اظہاریہ& res) ثابت
{
    CV_INSTRUMENT_REGION();

    اگر( e.flags == '/' && (!e.b.ڈیٹا || e.beta == 0) )
        Mat_Op_جوڑو_Ex::make_اظہاریہ(res, e.a, Mat(), s/e.alpha, 0);
    ورنہ
        MatOp::divide(s, e, res);
}

اندرونی خالی MatOp_Bin::make_اظہاریہ(Mat_اظہاریہ& res, حرف op, ثابت Mat& a, ثابت Mat& b, دوہرا scale)
{
    res = Mat_اظہاریہ(&g_MatOp_Bin, op, a, b, Mat(), scale, b.ڈیٹا ? 1 : 0);
}

اندرونی خالی MatOp_Bin::make_اظہاریہ(Mat_اظہاریہ& res, حرف op, ثابت Mat& a, ثابت Scalar& s)
{
    res = Mat_اظہاریہ(&g_MatOp_Bin, op, a, Mat(), Mat(), 1, 0, s);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

خالی MatOp_Cmp::assign(ثابت Mat_اظہاریہ& e, Mat& m, صحیح _type) ثابت
{
    Mat عارضی_2, &dst = _type == -1 || _type == CV_8U ? m : عارضی_2;

    اگر( e.b.ڈیٹا )
        cv::compare(e.a, e.b, dst, e.flags);
    ورنہ
        cv::compare(e.a, e.alpha, dst, e.flags);

    اگر( dst.ڈیٹا != m.ڈیٹا )
        dst.convert_مقصد(m, _type);
}

اندرونی خالی MatOp_Cmp::make_اظہاریہ(Mat_اظہاریہ& res, صحیح cmpop, ثابت Mat& a, ثابت Mat& b)
{
    res = Mat_اظہاریہ(&g_MatOp_Cmp, cmpop, a, b, Mat(), 1, 1);
}

اندرونی خالی MatOp_Cmp::make_اظہاریہ(Mat_اظہاریہ& res, صحیح cmpop, ثابت Mat& a, دوہرا alpha)
{
    res = Mat_اظہاریہ(&g_MatOp_Cmp, cmpop, a, Mat(), Mat(), alpha, 1);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

خالی MatOp_T::assign(ثابت Mat_اظہاریہ& e, Mat& m, صحیح _type) ثابت
{
    Mat عارضی_2, &dst = _type == -1 || _type == e.a.type() ? m : عارضی_2;

    cv::transpose(e.a, dst);

    اگر( dst.ڈیٹا != m.ڈیٹا || e.alpha != 1 )
        dst.convert_مقصد(m, _type, e.alpha);
}

خالی MatOp_T::multiply(ثابت Mat_اظہاریہ& e, دوہرا s, Mat_اظہاریہ& res) ثابت
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha *= s;
}

خالی MatOp_T::transpose(ثابت Mat_اظہاریہ& e, Mat_اظہاریہ& res) ثابت
{
    CV_INSTRUMENT_REGION();

    اگر( e.alpha == 1 )
        Mat_Op_شناختی_عنصر::make_اظہاریہ(res, e.a);
    ورنہ
        Mat_Op_جوڑو_Ex::make_اظہاریہ(res, e.a, Mat(), e.alpha, 0);
}

اندرونی خالی MatOp_T::make_اظہاریہ(Mat_اظہاریہ& res, ثابت Mat& a, دوہرا alpha)
{
    res = Mat_اظہاریہ(&g_MatOp_T, 0, a, Mat(), Mat(), alpha, 0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

خالی MatOp_GEMM::assign(ثابت Mat_اظہاریہ& e, Mat& m, صحیح _type) ثابت
{
    Mat عارضی_2, &dst = _type == -1 || _type == e.a.type() ? m : عارضی_2;

    cv::gemm(e.a, e.b, e.alpha, e.c, e.beta, dst, e.flags);
    اگر( dst.ڈیٹا != m.ڈیٹا )
        dst.convert_مقصد(m, _type);
}

خالی MatOp_GEMM::جوڑو(ثابت Mat_اظہاریہ& e1, ثابت Mat_اظہاریہ& e2, Mat_اظہاریہ& res) ثابت
{
    CV_INSTRUMENT_REGION();

    منطقی i1 = is_شناختی_عنصر(e1), i2 = is_شناختی_عنصر(e2);
    دوہرا alpha1 = i1 ? 1 : e1.alpha, alpha2 = i2 ? 1 : e2.alpha;

    اگر( isMatProd(e1) && (i2 || isScaled(e2) || isT(e2)) )
        MatOp_GEMM::make_اظہاریہ(res, (e1.flags & ~cv::GEMM_3_T)|(isT(e2) ? cv::GEMM_3_T : 0),
                             e1.a, e1.b, alpha1, e2.a, alpha2);
    ورنہ اگر( isMatProd(e2) && (i1 || isScaled(e1) || isT(e1)) )
        MatOp_GEMM::make_اظہاریہ(res, (e2.flags & ~cv::GEMM_3_T)|(isT(e1) ? cv::GEMM_3_T : 0),
                             e2.a, e2.b, alpha2, e1.a, alpha1);
    ورنہ اگر( یہ == e2.op )
        MatOp::جوڑو(e1, e2, res);
    ورنہ
        e2.op->جوڑو(e1, e2, res);
}

خالی MatOp_GEMM::تفریق(ثابت Mat_اظہاریہ& e1, ثابت Mat_اظہاریہ& e2, Mat_اظہاریہ& res) ثابت
{
    CV_INSTRUMENT_REGION();

    منطقی i1 = is_شناختی_عنصر(e1), i2 = is_شناختی_عنصر(e2);
    دوہرا alpha1 = i1 ? 1 : e1.alpha, alpha2 = i2 ? 1 : e2.alpha;

    اگر( isMatProd(e1) && (i2 || isScaled(e2) || isT(e2)) )
        MatOp_GEMM::make_اظہاریہ(res, (e1.flags & ~cv::GEMM_3_T)|(isT(e2) ? cv::GEMM_3_T : 0),
                             e1.a, e1.b, alpha1, e2.a, -alpha2);
    ورنہ اگر( isMatProd(e2) && (i1 || isScaled(e1) || isT(e1)) )
        MatOp_GEMM::make_اظہاریہ(res, (e2.flags & ~cv::GEMM_3_T)|(isT(e1) ? cv::GEMM_3_T : 0),
                            e2.a, e2.b, -alpha2, e1.a, alpha1);
    ورنہ اگر( یہ == e2.op )
        MatOp::تفریق(e1, e2, res);
    ورنہ
        e2.op->تفریق(e1, e2, res);
}

خالی MatOp_GEMM::multiply(ثابت Mat_اظہاریہ& e, دوہرا s, Mat_اظہاریہ& res) ثابت
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha *= s;
    res.beta *= s;
}

خالی MatOp_GEMM::transpose(ثابت Mat_اظہاریہ& e, Mat_اظہاریہ& res) ثابت
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.flags = (!(e.flags & cv::GEMM_1_T) ? cv::GEMM_2_T : 0) |
                (!(e.flags & cv::GEMM_2_T) ? cv::GEMM_1_T : 0) |
                (!(e.flags & cv::GEMM_3_T) ? cv::GEMM_3_T : 0);
    swap(res.a, res.b);
}

اندرونی خالی MatOp_GEMM::make_اظہاریہ(Mat_اظہاریہ& res, صحیح flags, ثابت Mat& a, ثابت Mat& b,
                                 دوہرا alpha, ثابت Mat& c, دوہرا beta)
{
    res = Mat_اظہاریہ(&g_MatOp_GEMM, flags, a, b, c, alpha, beta);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

خالی MatOp_Invert::assign(ثابت Mat_اظہاریہ& e, Mat& m, صحیح _type) ثابت
{
    Mat عارضی_2, &dst = _type == -1 || _type == e.a.type() ? m : عارضی_2;

    cv::invert(e.a, dst, e.flags);
    اگر( dst.ڈیٹا != m.ڈیٹا )
        dst.convert_مقصد(m, _type);
}

خالی MatOp_Invert::matmul(ثابت Mat_اظہاریہ& e1, ثابت Mat_اظہاریہ& e2, Mat_اظہاریہ& res) ثابت
{
    اگر( isInv(e1) && is_شناختی_عنصر(e2) )
        MatOp_Solve::make_اظہاریہ(res, e1.flags, e1.a, e2.a);
    ورنہ اگر( یہ == e2.op )
        MatOp::matmul(e1, e2, res);
    ورنہ
        e2.op->matmul(e1, e2, res);
}

اندرونی خالی MatOp_Invert::make_اظہاریہ(Mat_اظہاریہ& res, صحیح طریقہ, ثابت Mat& m)
{
    res = Mat_اظہاریہ(&g_MatOp_Invert, طریقہ, m, Mat(), Mat(), 1, 0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

خالی MatOp_Solve::assign(ثابت Mat_اظہاریہ& e, Mat& m, صحیح _type) ثابت
{
    Mat عارضی_2, &dst = _type == -1 || _type == e.a.type() ? m : عارضی_2;

    cv::solve(e.a, e.b, dst, e.flags);
    اگر( dst.ڈیٹا != m.ڈیٹا )
        dst.convert_مقصد(m, _type);
}

اندرونی خالی MatOp_Solve::make_اظہاریہ(Mat_اظہاریہ& res, صحیح طریقہ, ثابت Mat& a, ثابت Mat& b)
{
    res = Mat_اظہاریہ(&g_MatOp_Solve, طریقہ, a, b, Mat(), 1, 1);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

خالی MatOp_Initializer::assign(ثابت Mat_اظہاریہ& e, Mat& m, صحیح _type) ثابت
{
    اگر( _type == -1 )
        _type = e.a.type();

    m.create(e.a.size, _type);

    اگر( e.flags == 'I' && e.a.dims <= 2 )
        سیٹ_شناختی_عنصر(m, Scalar(e.alpha));
    ورنہ اگر( e.flags == '0' )
        m = Scalar();
    ورنہ اگر( e.flags == '1' )
        m = Scalar(e.alpha);
    ورنہ
        CV_Error(cv::Error::StsError, "Invalid matrix initializer type");
}

خالی MatOp_Initializer::multiply(ثابت Mat_اظہاریہ& e, دوہرا s, Mat_اظہاریہ& res) ثابت
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha *= s;
}

اندرونی خالی MatOp_Initializer::make_اظہاریہ(Mat_اظہاریہ& res, صحیح طریقہ, سائز sz, صحیح type, دوہرا alpha)
{
    res = Mat_اظہاریہ(getGlobalMatOpInitializer(), طریقہ, Mat(sz, type, (خالی*)(size_t)0xEEEEEEEE), Mat(), Mat(), alpha, 0);
}

اندرونی خالی MatOp_Initializer::make_اظہاریہ(Mat_اظہاریہ& res, صحیح طریقہ, صحیح ndims, ثابت صحیح* sizes, صحیح type, دوہرا alpha)
{
    res = Mat_اظہاریہ(getGlobalMatOpInitializer(), طریقہ, Mat(ndims, sizes, type, (خالی*)(size_t)0xEEEEEEEE), Mat(), Mat(), alpha, 0);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

Mat_اظہاریہ Mat::t() ثابت
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(*یہ);
    Mat_اظہاریہ e;
    MatOp_T::make_اظہاریہ(e, *یہ);
    واپس e;
}

Mat_اظہاریہ Mat::inv(صحیح طریقہ) ثابت
{
    CV_INSTRUMENT_REGION();

    Mat_اظہاریہ e;
    MatOp_Invert::make_اظہاریہ(e, طریقہ, *یہ);
    واپس e;
}


Mat_اظہاریہ Mat::mul(داخلہ_Array m, دوہرا scale) ثابت
{
    CV_INSTRUMENT_REGION();

    Mat b = m.getMat();
    // Unless the argument is a refcounted Mat/UMat, the header returned by getMat() may be
    // a non-owning view of caller memory (e.g. a scalar bound to _InputArray(const double&),
    // a Matx or a Vec) that the returned MatExpr can outlive, so snapshot it.
    // See https://github.com/opencv/opencv/issues/23577
    اگر( !m.isMat() && !m.isUMat() )
        b = b.clone();

    Mat_اظہاریہ e;
    MatOp_Bin::make_اظہاریہ(e, '*', *یہ, b, scale);
    واپس e;
}

Mat_اظہاریہ Mat::zeros(صحیح صفیں, صحیح ستون_تعداد, صحیح type)
{
    CV_INSTRUMENT_REGION();

    Mat_اظہاریہ e;
    MatOp_Initializer::make_اظہاریہ(e, '0', سائز(ستون_تعداد, صفیں), type);
    واپس e;
}

Mat_اظہاریہ Mat::zeros(سائز size, صحیح type)
{
    CV_INSTRUMENT_REGION();

    Mat_اظہاریہ e;
    MatOp_Initializer::make_اظہاریہ(e, '0', size, type);
    واپس e;
}

Mat_اظہاریہ Mat::zeros(صحیح ndims, ثابت صحیح* sizes, صحیح type)
{
    CV_INSTRUMENT_REGION();

    Mat_اظہاریہ e;
    MatOp_Initializer::make_اظہاریہ(e, '0', ndims, sizes, type);
    واپس e;
}

Mat_اظہاریہ Mat::zeros(ثابت MatShape& shape, صحیح type)
{
    CV_INSTRUMENT_REGION();

    Mat_اظہاریہ e;
    MatOp_Initializer::make_اظہاریہ(e, '0', shape.dims, shape.p, type);
    واپس e;
}

Mat_اظہاریہ Mat::ones(صحیح صفیں, صحیح ستون_تعداد, صحیح type)
{
    CV_INSTRUMENT_REGION();

    Mat_اظہاریہ e;
    MatOp_Initializer::make_اظہاریہ(e, '1', سائز(ستون_تعداد, صفیں), type);
    واپس e;
}

Mat_اظہاریہ Mat::ones(سائز size, صحیح type)
{
    CV_INSTRUMENT_REGION();

    Mat_اظہاریہ e;
    MatOp_Initializer::make_اظہاریہ(e, '1', size, type);
    واپس e;
}

Mat_اظہاریہ Mat::ones(صحیح ndims, ثابت صحیح* sizes, صحیح type)
{
    CV_INSTRUMENT_REGION();

    Mat_اظہاریہ e;
    MatOp_Initializer::make_اظہاریہ(e, '1', ndims, sizes, type);
    واپس e;
}

Mat_اظہاریہ Mat::ones(ثابت MatShape& shape, صحیح type)
{
    CV_INSTRUMENT_REGION();

    Mat_اظہاریہ e;
    MatOp_Initializer::make_اظہاریہ(e, '1', shape.dims, shape.p, type);
    واپس e;
}

Mat_اظہاریہ Mat::eye(صحیح صفیں, صحیح ستون_تعداد, صحیح type)
{
    CV_INSTRUMENT_REGION();

    Mat_اظہاریہ e;
    MatOp_Initializer::make_اظہاریہ(e, 'I', سائز(ستون_تعداد, صفیں), type);
    واپس e;
}

Mat_اظہاریہ Mat::eye(سائز size, صحیح type)
{
    CV_INSTRUMENT_REGION();

    Mat_اظہاریہ e;
    MatOp_Initializer::make_اظہاریہ(e, 'I', size, type);
    واپس e;
}

خالی Mat_اظہاریہ::swap(Mat_اظہاریہ& other)
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

داخلہ_Array_2::داخلہ_Array_2(ثابت Mat_اظہاریہ& اظہاریہ)
{
    اگر (!is_شناختی_عنصر(اظہاریہ))
    {
        Mat نتیجہ = اظہاریہ;  // TODO improve through refcount == 1 of expr.a (inplace operation is possible - except gemm?)
        Mat_اظہاریہ نتیجہ_اظہاریہ(نتیجہ);
        swap(const_cast<Mat_اظہاریہ&>(اظہاریہ), نتیجہ_اظہاریہ);
    }
    CV_Assert(is_شناختی_عنصر(اظہاریہ));
    init(FIXED_TYPE + FIXED_سائز + MAT + ACCESS_READ, &اظہاریہ.a);
}

} // cv::
