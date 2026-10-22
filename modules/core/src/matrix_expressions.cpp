// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


/* ////////////////////////////////////////////////////////////////////
//
//  Mat basic operations: Copy, Set
//
// */

#અન્તર્ભાવ "precomp.hpp"
#અન્તર્ભાવ <opencv2/core/utils/logger.hpp>

નામાકાશ cv
{

//This and its overload below are used in various MatExpr operator overloads
//implemented to check that Matrix operands exist.
સ્થિર શૂન્ય checkOperandsExist(અચલ Mat& a)
{
    જો (a.empty())
    {
        CV_Error(cv::Error::StsBadArg, "Matrix operand is an empty matrix.");
    }
}

સ્થિર શૂન્ય checkOperandsExist(અચલ Mat& a, અચલ Mat& b)
{
    જો (a.empty() || b.empty())
    {
        CV_Error(cv::Error::StsBadArg, "One or more matrix operands are empty.");
    }
}


વર્ગ MatOp_Identity CV_FINAL : જાહેર MatOp
{
જાહેર:
    MatOp_Identity() {}
    આભાસી ~MatOp_Identity() {}

    તાર્કિક elementWise(અચલ Mat_પદાવલિ& /*પદાવલિ*/) અચલ CV_OVERRIDE { પાછા સાચું; }
    શૂન્ય assign(અચલ Mat_પદાવલિ& પદાવલિ, Mat& m, પૂર્ણાંક type=-1) અચલ CV_OVERRIDE;

    સ્થિર શૂન્ય make_પદાવલિ(Mat_પદાવલિ& res, અચલ Mat& m);
};

સ્થિર MatOp_Identity g_MatOp_Identity;

વર્ગ MatOp_AddEx CV_FINAL : જાહેર MatOp
{
જાહેર:
    MatOp_AddEx() {}
    આભાસી ~MatOp_AddEx() {}

    તાર્કિક elementWise(અચલ Mat_પદાવલિ& /*પદાવલિ*/) અચલ CV_OVERRIDE { પાછા સાચું; }
    શૂન્ય assign(અચલ Mat_પદાવલિ& પદાવલિ, Mat& m, પૂર્ણાંક type=-1) અચલ CV_OVERRIDE;

    શૂન્ય add(અચલ Mat_પદાવલિ& e1, અચલ Scalar& s, Mat_પદાવલિ& res) અચલ CV_OVERRIDE;
    શૂન્ય subtract(અચલ Scalar& s, અચલ Mat_પદાવલિ& પદાવલિ, Mat_પદાવલિ& res) અચલ CV_OVERRIDE;
    શૂન્ય multiply(અચલ Mat_પદાવલિ& e1, દ્વિગુણ s, Mat_પદાવલિ& res) અચલ CV_OVERRIDE;
    શૂન્ય divide(દ્વિગુણ s, અચલ Mat_પદાવલિ& e, Mat_પદાવલિ& res) અચલ CV_OVERRIDE;

    શૂન્ય transpose(અચલ Mat_પદાવલિ& e1, Mat_પદાવલિ& res) અચલ CV_OVERRIDE;
    શૂન્ય abs(અચલ Mat_પદાવલિ& પદાવલિ, Mat_પદાવલિ& res) અચલ CV_OVERRIDE;

    સ્થિર શૂન્ય make_પદાવલિ(Mat_પદાવલિ& res, અચલ Mat& a, અચલ Mat& b, દ્વિગુણ alpha, દ્વિગુણ beta, અચલ Scalar& s=Scalar());
};

સ્થિર MatOp_AddEx g_MatOp_AddEx;

વર્ગ MatOp_Bin CV_FINAL : જાહેર MatOp
{
જાહેર:
    MatOp_Bin() {}
    આભાસી ~MatOp_Bin() {}

    તાર્કિક elementWise(અચલ Mat_પદાવલિ& /*પદાવલિ*/) અચલ CV_OVERRIDE { પાછા સાચું; }
    શૂન્ય assign(અચલ Mat_પદાવલિ& પદાવલિ, Mat& m, પૂર્ણાંક type=-1) અચલ CV_OVERRIDE;

    શૂન્ય multiply(અચલ Mat_પદાવલિ& e1, દ્વિગુણ s, Mat_પદાવલિ& res) અચલ CV_OVERRIDE;
    શૂન્ય divide(દ્વિગુણ s, અચલ Mat_પદાવલિ& e, Mat_પદાવલિ& res) અચલ CV_OVERRIDE;

    સ્થિર શૂન્ય make_પદાવલિ(Mat_પદાવલિ& res, અક્ષર op, અચલ Mat& a, અચલ Mat& b, દ્વિગુણ scale=1);
    સ્થિર શૂન્ય make_પદાવલિ(Mat_પદાવલિ& res, અક્ષર op, અચલ Mat& a, અચલ Scalar& s);
};

સ્થિર MatOp_Bin g_MatOp_Bin;

વર્ગ MatOp_Cmp CV_FINAL : જાહેર MatOp
{
જાહેર:
    MatOp_Cmp() {}
    આભાસી ~MatOp_Cmp() {}

    તાર્કિક elementWise(અચલ Mat_પદાવલિ& /*પદાવલિ*/) અચલ CV_OVERRIDE { પાછા સાચું; }
    શૂન્ય assign(અચલ Mat_પદાવલિ& પદાવલિ, Mat& m, પૂર્ણાંક type=-1) અચલ CV_OVERRIDE;

    સ્થિર શૂન્ય make_પદાવલિ(Mat_પદાવલિ& res, પૂર્ણાંક cmpop, અચલ Mat& a, અચલ Mat& b);
    સ્થિર શૂન્ય make_પદાવલિ(Mat_પદાવલિ& res, પૂર્ણાંક cmpop, અચલ Mat& a, દ્વિગુણ alpha);
};

સ્થિર MatOp_Cmp g_MatOp_Cmp;

વર્ગ MatOp_GEMM CV_FINAL : જાહેર MatOp
{
જાહેર:
    MatOp_GEMM() {}
    આભાસી ~MatOp_GEMM() {}

    તાર્કિક elementWise(અચલ Mat_પદાવલિ& /*પદાવલિ*/) અચલ CV_OVERRIDE { પાછા ખોટું; }
    શૂન્ય assign(અચલ Mat_પદાવલિ& પદાવલિ, Mat& m, પૂર્ણાંક type=-1) અચલ CV_OVERRIDE;

    શૂન્ય add(અચલ Mat_પદાવલિ& e1, અચલ Mat_પદાવલિ& e2, Mat_પદાવલિ& res) અચલ CV_OVERRIDE;
    શૂન્ય subtract(અચલ Mat_પદાવલિ& e1, અચલ Mat_પદાવલિ& e2, Mat_પદાવલિ& res) અચલ CV_OVERRIDE;
    શૂન્ય multiply(અચલ Mat_પદાવલિ& e, દ્વિગુણ s, Mat_પદાવલિ& res) અચલ CV_OVERRIDE;

    શૂન્ય transpose(અચલ Mat_પદાવલિ& પદાવલિ, Mat_પદાવલિ& res) અચલ CV_OVERRIDE;

    Size size(અચલ Mat_પદાવલિ& પદાવલિ) અચલ CV_OVERRIDE
    {
        પાછા Size(
            (પદાવલિ.flags & GEMM_2_T) ? પદાવલિ.b.rows : પદાવલિ.b.cols,
            (પદાવલિ.flags & GEMM_1_T) ? પદાવલિ.a.cols : પદાવલિ.a.rows
        );
    }

    સ્થિર શૂન્ય make_પદાવલિ(Mat_પદાવલિ& res, પૂર્ણાંક flags, અચલ Mat& a, અચલ Mat& b,
                         દ્વિગુણ alpha=1, અચલ Mat& c=Mat(), દ્વિગુણ beta=1);

};

સ્થિર MatOp_GEMM g_MatOp_GEMM;

વર્ગ MatOp_Invert CV_FINAL : જાહેર MatOp
{
જાહેર:
    MatOp_Invert() {}
    આભાસી ~MatOp_Invert() {}

    તાર્કિક elementWise(અચલ Mat_પદાવલિ& /*પદાવલિ*/) અચલ CV_OVERRIDE { પાછા ખોટું; }
    શૂન્ય assign(અચલ Mat_પદાવલિ& પદાવલિ, Mat& m, પૂર્ણાંક type=-1) અચલ CV_OVERRIDE;

    શૂન્ય matmul(અચલ Mat_પદાવલિ& expr1, અચલ Mat_પદાવલિ& expr2, Mat_પદાવલિ& res) અચલ CV_OVERRIDE;

    સ્થિર શૂન્ય make_પદાવલિ(Mat_પદાવલિ& res, પૂર્ણાંક method, અચલ Mat& m);
};

સ્થિર MatOp_Invert g_MatOp_Invert;

વર્ગ MatOp_T CV_FINAL : જાહેર MatOp
{
જાહેર:
    MatOp_T() {}
    આભાસી ~MatOp_T() {}

    તાર્કિક elementWise(અચલ Mat_પદાવલિ& /*પદાવલિ*/) અચલ CV_OVERRIDE { પાછા ખોટું; }
    શૂન્ય assign(અચલ Mat_પદાવલિ& પદાવલિ, Mat& m, પૂર્ણાંક type=-1) અચલ CV_OVERRIDE;

    શૂન્ય multiply(અચલ Mat_પદાવલિ& e1, દ્વિગુણ s, Mat_પદાવલિ& res) અચલ CV_OVERRIDE;
    શૂન્ય transpose(અચલ Mat_પદાવલિ& પદાવલિ, Mat_પદાવલિ& res) અચલ CV_OVERRIDE;

    સ્થિર શૂન્ય make_પદાવલિ(Mat_પદાવલિ& res, અચલ Mat& a, દ્વિગુણ alpha=1);
};

સ્થિર MatOp_T g_MatOp_T;

વર્ગ MatOp_Solve CV_FINAL : જાહેર MatOp
{
જાહેર:
    MatOp_Solve() {}
    આભાસી ~MatOp_Solve() {}

    તાર્કિક elementWise(અચલ Mat_પદાવલિ& /*પદાવલિ*/) અચલ CV_OVERRIDE { પાછા ખોટું; }
    શૂન્ય assign(અચલ Mat_પદાવલિ& પદાવલિ, Mat& m, પૂર્ણાંક type=-1) અચલ CV_OVERRIDE;

    સ્થિર શૂન્ય make_પદાવલિ(Mat_પદાવલિ& res, પૂર્ણાંક method, અચલ Mat& a, અચલ Mat& b);
};

સ્થિર MatOp_Solve g_MatOp_Solve;

વર્ગ MatOp_Initializer CV_FINAL : જાહેર MatOp
{
જાહેર:
    MatOp_Initializer() {}
    આભાસી ~MatOp_Initializer() {}

    તાર્કિક elementWise(અચલ Mat_પદાવલિ& /*પદાવલિ*/) અચલ CV_OVERRIDE { પાછા ખોટું; }
    શૂન્ય assign(અચલ Mat_પદાવલિ& પદાવલિ, Mat& m, પૂર્ણાંક type=-1) અચલ CV_OVERRIDE;

    શૂન્ય multiply(અચલ Mat_પદાવલિ& e, દ્વિગુણ s, Mat_પદાવલિ& res) અચલ CV_OVERRIDE;

    સ્થિર શૂન્ય make_પદાવલિ(Mat_પદાવલિ& res, પૂર્ણાંક method, Size sz, પૂર્ણાંક type, દ્વિગુણ alpha=1);
    સ્થિર શૂન્ય make_પદાવલિ(Mat_પદાવલિ& res, પૂર્ણાંક method, પૂર્ણાંક ndims, અચલ પૂર્ણાંક* sizes, પૂર્ણાંક type, દ્વિગુણ alpha=1);
};

સ્થિર MatOp_Initializer* getGlobalMatOpInitializer()
{
    CV_SINGLETON_LAZY_INIT(MatOp_Initializer, નવું MatOp_Initializer())
}

સ્થિર અંતઃરેખ તાર્કિક isIdentity(અચલ Mat_પદાવલિ& e) { પાછા e.op == &g_MatOp_Identity; }
સ્થિર અંતઃરેખ તાર્કિક isAddEx(અચલ Mat_પદાવલિ& e) { પાછા e.op == &g_MatOp_AddEx; }
સ્થિર અંતઃરેખ તાર્કિક isScaled(અચલ Mat_પદાવલિ& e) { પાછા isAddEx(e) && (!e.b.data || e.beta == 0) && e.s == Scalar(); }
સ્થિર અંતઃરેખ તાર્કિક isBin(અચલ Mat_પદાવલિ& e, અક્ષર c) { પાછા e.op == &g_MatOp_Bin && e.flags == c; }
સ્થિર અંતઃરેખ તાર્કિક isCmp(અચલ Mat_પદાવલિ& e) { પાછા e.op == &g_MatOp_Cmp; }
સ્થિર અંતઃરેખ તાર્કિક isReciprocal(અચલ Mat_પદાવલિ& e) { પાછા isBin(e,'/') && (!e.b.data || e.beta == 0); }
સ્થિર અંતઃરેખ તાર્કિક isT(અચલ Mat_પદાવલિ& e) { પાછા e.op == &g_MatOp_T; }
સ્થિર અંતઃરેખ તાર્કિક isInv(અચલ Mat_પદાવલિ& e) { પાછા e.op == &g_MatOp_Invert; }
સ્થિર અંતઃરેખ તાર્કિક isSolve(અચલ Mat_પદાવલિ& e) { પાછા e.op == &g_MatOp_Solve; }
//static inline bool isGEMM(const MatExpr& e) { return e.op == &g_MatOp_GEMM; }
સ્થિર અંતઃરેખ તાર્કિક isMatProd(અચલ Mat_પદાવલિ& e) { પાછા e.op == &g_MatOp_GEMM && (!e.c.data || e.beta == 0); }
સ્થિર અંતઃરેખ તાર્કિક isInitializer(અચલ Mat_પદાવલિ& e) { પાછા e.op == getGlobalMatOpInitializer(); }

/////////////////////////////////////////////////////////////////////////////////////////////////////

MatOp::MatOp() {}
MatOp::~MatOp() {}


તાર્કિક MatOp::elementWise(અચલ Mat_પદાવલિ& /*પદાવલિ*/) અચલ
{
    પાછા ખોટું;
}

શૂન્ય MatOp::roi(અચલ Mat_પદાવલિ& પદાવલિ, અચલ Range& rowRange, અચલ Range& colRange, Mat_પદાવલિ& e) અચલ
{
    જો( elementWise(પદાવલિ) )
    {
        e = Mat_પદાવલિ(પદાવલિ.op, પદાવલિ.flags, Mat(), Mat(), Mat(),
                    પદાવલિ.alpha, પદાવલિ.beta, પદાવલિ.s);
        જો(પદાવલિ.a.data)
            e.a = પદાવલિ.a(rowRange, colRange);
        જો(પદાવલિ.b.data)
            e.b = પદાવલિ.b(rowRange, colRange);
        જો(પદાવલિ.c.data)
            e.c = પદાવલિ.c(rowRange, colRange);
    }
    નહીંતર
    {
        Mat m;
        પદાવલિ.op->assign(પદાવલિ, m);
        e = Mat_પદાવલિ(&g_MatOp_Identity, 0, m(rowRange, colRange), Mat(), Mat());
    }
}

શૂન્ય MatOp::diag(અચલ Mat_પદાવલિ& પદાવલિ, પૂર્ણાંક d, Mat_પદાવલિ& e) અચલ
{
    જો( elementWise(પદાવલિ) )
    {
        e = Mat_પદાવલિ(પદાવલિ.op, પદાવલિ.flags, Mat(), Mat(), Mat(),
                    પદાવલિ.alpha, પદાવલિ.beta, પદાવલિ.s);
        જો(પદાવલિ.a.data)
            e.a = પદાવલિ.a.diag(d);
        જો(પદાવલિ.b.data)
            e.b = પદાવલિ.b.diag(d);
        જો(પદાવલિ.c.data)
            e.c = પદાવલિ.c.diag(d);
    }
    નહીંતર
    {
        Mat m;
        પદાવલિ.op->assign(પદાવલિ, m);
        e = Mat_પદાવલિ(&g_MatOp_Identity, 0, m.diag(d), Mat(), Mat());
    }
}


શૂન્ય MatOp::augAssignAdd(અચલ Mat_પદાવલિ& પદાવલિ, Mat& m) અચલ
{
    Mat temp;
    પદાવલિ.op->assign(પદાવલિ, temp);
    m += temp;
}


શૂન્ય MatOp::augAssignSubtract(અચલ Mat_પદાવલિ& પદાવલિ, Mat& m) અચલ
{
    Mat temp;
    પદાવલિ.op->assign(પદાવલિ, temp);
    m -= temp;
}


શૂન્ય MatOp::augAssignMultiply(અચલ Mat_પદાવલિ& પદાવલિ, Mat& m) અચલ
{
    Mat temp;
    પદાવલિ.op->assign(પદાવલિ, temp);
    m *= temp;
}


શૂન્ય MatOp::augAssignDivide(અચલ Mat_પદાવલિ& પદાવલિ, Mat& m) અચલ
{
    Mat temp;
    પદાવલિ.op->assign(પદાવલિ, temp);
    m /= temp;
}


શૂન્ય MatOp::augAssignAnd(અચલ Mat_પદાવલિ& પદાવલિ, Mat& m) અચલ
{
    Mat temp;
    પદાવલિ.op->assign(પદાવલિ, temp);
    m &= temp;
}


શૂન્ય MatOp::augAssignOr(અચલ Mat_પદાવલિ& પદાવલિ, Mat& m) અચલ
{
    Mat temp;
    પદાવલિ.op->assign(પદાવલિ, temp);
    m |= temp;
}


શૂન્ય MatOp::augAssignXor(અચલ Mat_પદાવલિ& પદાવલિ, Mat& m) અચલ
{
    Mat temp;
    પદાવલિ.op->assign(પદાવલિ, temp);
    m ^= temp;
}


શૂન્ય MatOp::add(અચલ Mat_પદાવલિ& e1, અચલ Mat_પદાવલિ& e2, Mat_પદાવલિ& res) અચલ
{
    CV_INSTRUMENT_REGION();

    જો( આ == e2.op )
    {
        દ્વિગુણ alpha = 1, beta = 1;
        Scalar s;
        Mat m1, m2;
        જો( isAddEx(e1) && (!e1.b.data || e1.beta == 0) )
        {
            m1 = e1.a;
            alpha = e1.alpha;
            s = e1.s;
        }
        નહીંતર
            e1.op->assign(e1, m1);

        જો( isAddEx(e2) && (!e2.b.data || e2.beta == 0) )
        {
            m2 = e2.a;
            beta = e2.alpha;
            s += e2.s;
        }
        નહીંતર
            e2.op->assign(e2, m2);
        MatOp_AddEx::make_પદાવલિ(res, m1, m2, alpha, beta, s);
    }
    નહીંતર
        e2.op->add(e1, e2, res);
}


શૂન્ય MatOp::add(અચલ Mat_પદાવલિ& expr1, અચલ Scalar& s, Mat_પદાવલિ& res) અચલ
{
    CV_INSTRUMENT_REGION();

    Mat m1;
    expr1.op->assign(expr1, m1);
    MatOp_AddEx::make_પદાવલિ(res, m1, Mat(), 1, 0, s);
}


શૂન્ય MatOp::subtract(અચલ Mat_પદાવલિ& e1, અચલ Mat_પદાવલિ& e2, Mat_પદાવલિ& res) અચલ
{
    CV_INSTRUMENT_REGION();

    જો( આ == e2.op )
    {
        દ્વિગુણ alpha = 1, beta = -1;
        Scalar s;
        Mat m1, m2;
        જો( isAddEx(e1) && (!e1.b.data || e1.beta == 0) )
        {
            m1 = e1.a;
            alpha = e1.alpha;
            s = e1.s;
        }
        નહીંતર
            e1.op->assign(e1, m1);

        જો( isAddEx(e2) && (!e2.b.data || e2.beta == 0) )
        {
            m2 = e2.a;
            beta = -e2.alpha;
            s -= e2.s;
        }
        નહીંતર
            e2.op->assign(e2, m2);
        MatOp_AddEx::make_પદાવલિ(res, m1, m2, alpha, beta, s);
    }
    નહીંતર
        e2.op->subtract(e1, e2, res);
}


શૂન્ય MatOp::subtract(અચલ Scalar& s, અચલ Mat_પદાવલિ& પદાવલિ, Mat_પદાવલિ& res) અચલ
{
    CV_INSTRUMENT_REGION();

    Mat m;
    પદાવલિ.op->assign(પદાવલિ, m);
    MatOp_AddEx::make_પદાવલિ(res, m, Mat(), -1, 0, s);
}


શૂન્ય MatOp::multiply(અચલ Mat_પદાવલિ& e1, અચલ Mat_પદાવલિ& e2, Mat_પદાવલિ& res, દ્વિગુણ scale) અચલ
{
    CV_INSTRUMENT_REGION();

    જો( આ == e2.op )
    {
        Mat m1, m2;

        જો( isReciprocal(e1) )
        {
            જો( isScaled(e2) )
            {
                scale *= e2.alpha;
                m2 = e2.a;
            }
            નહીંતર
                e2.op->assign(e2, m2);

            MatOp_Bin::make_પદાવલિ(res, '/', m2, e1.a, scale/e1.alpha);
        }
        નહીંતર
        {
            અક્ષર op = '*';
            જો( isScaled(e1) )
            {
                m1 = e1.a;
                scale *= e1.alpha;
            }
            નહીંતર
                e1.op->assign(e1, m1);

            જો( isScaled(e2) )
            {
                m2 = e2.a;
                scale *= e2.alpha;
            }
            નહીંતર જો( isReciprocal(e2) )
            {
                op = '/';
                m2 = e2.a;
                scale *= e2.alpha;
            }
            નહીંતર
                e2.op->assign(e2, m2);

            MatOp_Bin::make_પદાવલિ(res, op, m1, m2, scale);
        }
    }
    નહીંતર
        e2.op->multiply(e1, e2, res, scale);
}


શૂન્ય MatOp::multiply(અચલ Mat_પદાવલિ& પદાવલિ, દ્વિગુણ s, Mat_પદાવલિ& res) અચલ
{
    CV_INSTRUMENT_REGION();

    Mat m;
    પદાવલિ.op->assign(પદાવલિ, m);
    MatOp_AddEx::make_પદાવલિ(res, m, Mat(), s, 0);
}


શૂન્ય MatOp::divide(અચલ Mat_પદાવલિ& e1, અચલ Mat_પદાવલિ& e2, Mat_પદાવલિ& res, દ્વિગુણ scale) અચલ
{
    CV_INSTRUMENT_REGION();

    જો( આ == e2.op )
    {
        જો( isReciprocal(e1) && isReciprocal(e2) )
            MatOp_Bin::make_પદાવલિ(res, '/', e2.a, e1.a, e1.alpha/e2.alpha);
        નહીંતર
        {
            Mat m1, m2;
            અક્ષર op = '/';

            જો( isScaled(e1) )
            {
                m1 = e1.a;
                scale *= e1.alpha;
            }
            નહીંતર
                e1.op->assign(e1, m1);

            જો( isScaled(e2) )
            {
                m2 = e2.a;
                scale /= e2.alpha;
            }
            નહીંતર જો( isReciprocal(e2) )
            {
                m2 = e2.a;
                scale /= e2.alpha;
                op = '*';
            }
            નહીંતર
                e2.op->assign(e2, m2);
            MatOp_Bin::make_પદાવલિ(res, op, m1, m2, scale);
        }
    }
    નહીંતર
        e2.op->divide(e1, e2, res, scale);
}


શૂન્ય MatOp::divide(દ્વિગુણ s, અચલ Mat_પદાવલિ& પદાવલિ, Mat_પદાવલિ& res) અચલ
{
    CV_INSTRUMENT_REGION();

    Mat m;
    પદાવલિ.op->assign(પદાવલિ, m);
    MatOp_Bin::make_પદાવલિ(res, '/', m, Mat(), s);
}


શૂન્ય MatOp::abs(અચલ Mat_પદાવલિ& પદાવલિ, Mat_પદાવલિ& res) અચલ
{
    CV_INSTRUMENT_REGION();

    Mat m;
    પદાવલિ.op->assign(પદાવલિ, m);
    MatOp_Bin::make_પદાવલિ(res, 'a', m, Mat());
}


શૂન્ય MatOp::transpose(અચલ Mat_પદાવલિ& પદાવલિ, Mat_પદાવલિ& res) અચલ
{
    CV_INSTRUMENT_REGION();

    Mat m;
    પદાવલિ.op->assign(પદાવલિ, m);
    MatOp_T::make_પદાવલિ(res, m, 1);
}


શૂન્ય MatOp::matmul(અચલ Mat_પદાવલિ& e1, અચલ Mat_પદાવલિ& e2, Mat_પદાવલિ& res) અચલ
{
    જો( આ == e2.op )
    {
        દ્વિગુણ scale = 1;
        પૂર્ણાંક flags = 0;
        Mat m1, m2;

        જો( isT(e1) )
        {
            flags = cv::GEMM_1_T;
            scale = e1.alpha;
            m1 = e1.a;
        }
        નહીંતર જો( isScaled(e1) )
        {
            scale = e1.alpha;
            m1 = e1.a;
        }
        નહીંતર
            e1.op->assign(e1, m1);

        જો( isT(e2) )
        {
            flags |= cv::GEMM_2_T;
            scale *= e2.alpha;
            m2 = e2.a;
        }
        નહીંતર જો( isScaled(e2) )
        {
            scale *= e2.alpha;
            m2 = e2.a;
        }
        નહીંતર
            e2.op->assign(e2, m2);

        MatOp_GEMM::make_પદાવલિ(res, flags, m1, m2, scale);
    }
    નહીંતર
        e2.op->matmul(e1, e2, res);
}


શૂન્ય MatOp::invert(અચલ Mat_પદાવલિ& પદાવલિ, પૂર્ણાંક method, Mat_પદાવલિ& res) અચલ
{
    Mat m;
    પદાવલિ.op->assign(પદાવલિ, m);
    MatOp_Invert::make_પદાવલિ(res, method, m);
}


Size MatOp::size(અચલ Mat_પદાવલિ& પદાવલિ) અચલ
{
    પાછા !પદાવલિ.a.empty() ? પદાવલિ.a.size() : પદાવલિ.b.empty() ? પદાવલિ.b.size() : પદાવલિ.c.size();
}

પૂર્ણાંક MatOp::type(અચલ Mat_પદાવલિ& પદાવલિ) અચલ
{
    CV_INSTRUMENT_REGION();

    પાછા !પદાવલિ.a.empty() ? પદાવલિ.a.type() : પદાવલિ.b.empty() ? પદાવલિ.b.type() : પદાવલિ.c.type();
}

//////////////////////////////////////////////////////////////////////////////////////////////////

Mat_પદાવલિ::Mat_પદાવલિ(અચલ Mat& m) : op(&g_MatOp_Identity), flags(0), a(m), b(Mat()), c(Mat()), alpha(1), beta(0), s(Scalar())
{
}

Mat_પદાવલિ Mat_પદાવલિ::row(પૂર્ણાંક y) અચલ
{
    Mat_પદાવલિ e;
    op->roi(*આ, Range(y, y+1), Range::all(), e);
    પાછા e;
}

Mat_પદાવલિ Mat_પદાવલિ::col(પૂર્ણાંક x) અચલ
{
    Mat_પદાવલિ e;
    op->roi(*આ, Range::all(), Range(x, x+1), e);
    પાછા e;
}

Mat_પદાવલિ Mat_પદાવલિ::diag(પૂર્ણાંક d) અચલ
{
    Mat_પદાવલિ e;
    op->diag(*આ, d, e);
    પાછા e;
}

Mat_પદાવલિ Mat_પદાવલિ::operator()( અચલ Range& rowRange, અચલ Range& colRange ) અચલ
{
    Mat_પદાવલિ e;
    op->roi(*આ, rowRange, colRange, e);
    પાછા e;
}

Mat_પદાવલિ Mat_પદાવલિ::operator()( અચલ Rect& roi ) અચલ
{
    Mat_પદાવલિ e;
    op->roi(*આ, Range(roi.y, roi.y + roi.height), Range(roi.x, roi.x + roi.width), e);
    પાછા e;
}

Mat Mat_પદાવલિ::cross(અચલ Mat& m) અચલ
{
    પાછા ((Mat)*આ).cross(m);
}

દ્વિગુણ Mat_પદાવલિ::dot(અચલ Mat& m) અચલ
{
    પાછા ((Mat)*આ).dot(m);
}

Mat_પદાવલિ Mat_પદાવલિ::t() અચલ
{
    Mat_પદાવલિ e;
    op->transpose(*આ, e);
    પાછા e;
}

Mat_પદાવલિ Mat_પદાવલિ::inv(પૂર્ણાંક method) અચલ
{
    Mat_પદાવલિ e;
    op->invert(*આ, method, e);
    પાછા e;
}

Mat_પદાવલિ Mat_પદાવલિ::mul(અચલ Mat_પદાવલિ& e, દ્વિગુણ scale) અચલ
{
    Mat_પદાવલિ en;
    op->multiply(*આ, e, en, scale);
    પાછા en;
}

Mat_પદાવલિ Mat_પદાવલિ::mul(અચલ Mat& m, દ્વિગુણ scale) અચલ
{
    Mat_પદાવલિ e;
    op->multiply(*આ, Mat_પદાવલિ(m), e, scale);
    પાછા e;
}

Mat_પદાવલિ operator + (અચલ Mat& a, અચલ Mat& b)
{
    checkOperandsExist(a, b);
    Mat_પદાવલિ e;
    MatOp_AddEx::make_પદાવલિ(e, a, b, 1, 1);
    પાછા e;
}

Mat_પદાવલિ operator + (અચલ Mat& a, અચલ Scalar& s)
{
    checkOperandsExist(a);
    Mat_પદાવલિ e;
    MatOp_AddEx::make_પદાવલિ(e, a, Mat(), 1, 0, s);
    પાછા e;
}

Mat_પદાવલિ operator + (અચલ Scalar& s, અચલ Mat& a)
{
    checkOperandsExist(a);
    Mat_પદાવલિ e;
    MatOp_AddEx::make_પદાવલિ(e, a, Mat(), 1, 0, s);
    પાછા e;
}

Mat_પદાવલિ operator + (અચલ Mat_પદાવલિ& e, અચલ Mat& m)
{
    Mat_પદાવલિ en;
    e.op->add(e, Mat_પદાવલિ(m), en);
    પાછા en;
}

Mat_પદાવલિ operator + (અચલ Mat& m, અચલ Mat_પદાવલિ& e)
{
    checkOperandsExist(m);
    Mat_પદાવલિ en;
    e.op->add(e, Mat_પદાવલિ(m), en);
    પાછા en;
}

Mat_પદાવલિ operator + (અચલ Mat_પદાવલિ& e, અચલ Scalar& s)
{
    Mat_પદાવલિ en;
    e.op->add(e, s, en);
    પાછા en;
}

Mat_પદાવલિ operator + (અચલ Scalar& s, અચલ Mat_પદાવલિ& e)
{
    Mat_પદાવલિ en;
    e.op->add(e, s, en);
    પાછા en;
}

Mat_પદાવલિ operator + (અચલ Mat_પદાવલિ& e1, અચલ Mat_પદાવલિ& e2)
{
    Mat_પદાવલિ en;
    e1.op->add(e1, e2, en);
    પાછા en;
}

Mat_પદાવલિ operator - (અચલ Mat& a, અચલ Mat& b)
{
    checkOperandsExist(a, b);
    Mat_પદાવલિ e;
    MatOp_AddEx::make_પદાવલિ(e, a, b, 1, -1);
    પાછા e;
}

Mat_પદાવલિ operator - (અચલ Mat& a, અચલ Scalar& s)
{
    checkOperandsExist(a);
    Mat_પદાવલિ e;
    MatOp_AddEx::make_પદાવલિ(e, a, Mat(), 1, 0, -s);
    પાછા e;
}

Mat_પદાવલિ operator - (અચલ Scalar& s, અચલ Mat& a)
{
    checkOperandsExist(a);
    Mat_પદાવલિ e;
    MatOp_AddEx::make_પદાવલિ(e, a, Mat(), -1, 0, s);
    પાછા e;
}

Mat_પદાવલિ operator - (અચલ Mat_પદાવલિ& e, અચલ Mat& m)
{
    checkOperandsExist(m);
    Mat_પદાવલિ en;
    e.op->subtract(e, Mat_પદાવલિ(m), en);
    પાછા en;
}

Mat_પદાવલિ operator - (અચલ Mat& m, અચલ Mat_પદાવલિ& e)
{
    checkOperandsExist(m);
    Mat_પદાવલિ en;
    e.op->subtract(Mat_પદાવલિ(m), e, en);
    પાછા en;
}

Mat_પદાવલિ operator - (અચલ Mat_પદાવલિ& e, અચલ Scalar& s)
{
    Mat_પદાવલિ en;
    e.op->add(e, -s, en);
    પાછા en;
}

Mat_પદાવલિ operator - (અચલ Scalar& s, અચલ Mat_પદાવલિ& e)
{
    Mat_પદાવલિ en;
    e.op->subtract(s, e, en);
    પાછા en;
}

Mat_પદાવલિ operator - (અચલ Mat_પદાવલિ& e1, અચલ Mat_પદાવલિ& e2)
{
    Mat_પદાવલિ en;
    e1.op->subtract(e1, e2, en);
    પાછા en;
}

Mat_પદાવલિ operator - (અચલ Mat& m)
{
    checkOperandsExist(m);
    Mat_પદાવલિ e;
    MatOp_AddEx::make_પદાવલિ(e, m, Mat(), -1, 0);
    પાછા e;
}

Mat_પદાવલિ operator - (અચલ Mat_પદાવલિ& e)
{
    Mat_પદાવલિ en;
    e.op->subtract(Scalar(0), e, en);
    પાછા en;
}

Mat_પદાવલિ operator * (અચલ Mat& a, અચલ Mat& b)
{
    checkOperandsExist(a, b);
    Mat_પદાવલિ e;
    MatOp_GEMM::make_પદાવલિ(e, 0, a, b);
    પાછા e;
}

Mat_પદાવલિ operator * (અચલ Mat& a, દ્વિગુણ s)
{
    checkOperandsExist(a);
    Mat_પદાવલિ e;
    MatOp_AddEx::make_પદાવલિ(e, a, Mat(), s, 0);
    પાછા e;
}

Mat_પદાવલિ operator * (દ્વિગુણ s, અચલ Mat& a)
{
    checkOperandsExist(a);
    Mat_પદાવલિ e;
    MatOp_AddEx::make_પદાવલિ(e, a, Mat(), s, 0);
    પાછા e;
}

Mat_પદાવલિ operator * (અચલ Mat_પદાવલિ& e, અચલ Mat& m)
{
    Mat_પદાવલિ en;
    e.op->matmul(e, Mat_પદાવલિ(m), en);
    પાછા en;
}

Mat_પદાવલિ operator * (અચલ Mat& m, અચલ Mat_પદાવલિ& e)
{
    checkOperandsExist(m);
    Mat_પદાવલિ en;
    e.op->matmul(Mat_પદાવલિ(m), e, en);
    પાછા en;
}

Mat_પદાવલિ operator * (અચલ Mat_પદાવલિ& e, દ્વિગુણ s)
{
    Mat_પદાવલિ en;
    e.op->multiply(e, s, en);
    પાછા en;
}

Mat_પદાવલિ operator * (દ્વિગુણ s, અચલ Mat_પદાવલિ& e)
{
    Mat_પદાવલિ en;
    e.op->multiply(e, s, en);
    પાછા en;
}

Mat_પદાવલિ operator * (અચલ Mat_પદાવલિ& e1, અચલ Mat_પદાવલિ& e2)
{
    Mat_પદાવલિ en;
    e1.op->matmul(e1, e2, en);
    પાછા en;
}

Mat_પદાવલિ operator / (અચલ Mat& a, અચલ Mat& b)
{
    checkOperandsExist(a, b);
    Mat_પદાવલિ e;
    MatOp_Bin::make_પદાવલિ(e, '/', a, b);
    પાછા e;
}

Mat_પદાવલિ operator / (અચલ Mat& a, દ્વિગુણ s)
{
    checkOperandsExist(a);
    Mat_પદાવલિ e;
    MatOp_AddEx::make_પદાવલિ(e, a, Mat(), 1./s, 0);
    પાછા e;
}

Mat_પદાવલિ operator / (દ્વિગુણ s, અચલ Mat& a)
{
    checkOperandsExist(a);
    Mat_પદાવલિ e;
    MatOp_Bin::make_પદાવલિ(e, '/', a, Mat(), s);
    પાછા e;
}

Mat_પદાવલિ operator / (અચલ Mat_પદાવલિ& e, અચલ Mat& m)
{
    Mat_પદાવલિ en;
    e.op->divide(e, Mat_પદાવલિ(m), en);
    પાછા en;
}

Mat_પદાવલિ operator / (અચલ Mat& m, અચલ Mat_પદાવલિ& e)
{
    checkOperandsExist(m);
    Mat_પદાવલિ en;
    e.op->divide(Mat_પદાવલિ(m), e, en);
    પાછા en;
}

Mat_પદાવલિ operator / (અચલ Mat_પદાવલિ& e, દ્વિગુણ s)
{
    Mat_પદાવલિ en;
    e.op->multiply(e, 1./s, en);
    પાછા en;
}

Mat_પદાવલિ operator / (દ્વિગુણ s, અચલ Mat_પદાવલિ& e)
{
    Mat_પદાવલિ en;
    e.op->divide(s, e, en);
    પાછા en;
}

Mat_પદાવલિ operator / (અચલ Mat_પદાવલિ& e1, અચલ Mat_પદાવલિ& e2)
{
    Mat_પદાવલિ en;
    e1.op->divide(e1, e2, en);
    પાછા en;
}

Mat_પદાવલિ operator < (અચલ Mat& a, અચલ Mat& b)
{
    checkOperandsExist(a, b);
    Mat_પદાવલિ e;
    MatOp_Cmp::make_પદાવલિ(e, cv::CMP_LT, a, b);
    પાછા e;
}

Mat_પદાવલિ operator < (અચલ Mat& a, દ્વિગુણ s)
{
    checkOperandsExist(a);
    Mat_પદાવલિ e;
    MatOp_Cmp::make_પદાવલિ(e, cv::CMP_LT, a, s);
    પાછા e;
}

Mat_પદાવલિ operator < (દ્વિગુણ s, અચલ Mat& a)
{
    checkOperandsExist(a);
    Mat_પદાવલિ e;
    MatOp_Cmp::make_પદાવલિ(e, cv::CMP_GT, a, s);
    પાછા e;
}

Mat_પદાવલિ operator <= (અચલ Mat& a, અચલ Mat& b)
{
    checkOperandsExist(a, b);
    Mat_પદાવલિ e;
    MatOp_Cmp::make_પદાવલિ(e, cv::CMP_LE, a, b);
    પાછા e;
}

Mat_પદાવલિ operator <= (અચલ Mat& a, દ્વિગુણ s)
{
    checkOperandsExist(a);
    Mat_પદાવલિ e;
    MatOp_Cmp::make_પદાવલિ(e, cv::CMP_LE, a, s);
    પાછા e;
}

Mat_પદાવલિ operator <= (દ્વિગુણ s, અચલ Mat& a)
{
    checkOperandsExist(a);
    Mat_પદાવલિ e;
    MatOp_Cmp::make_પદાવલિ(e, cv::CMP_GE, a, s);
    પાછા e;
}

Mat_પદાવલિ operator == (અચલ Mat& a, અચલ Mat& b)
{
    checkOperandsExist(a, b);
    Mat_પદાવલિ e;
    MatOp_Cmp::make_પદાવલિ(e, cv::CMP_EQ, a, b);
    પાછા e;
}

Mat_પદાવલિ operator == (અચલ Mat& a, દ્વિગુણ s)
{
    checkOperandsExist(a);
    Mat_પદાવલિ e;
    MatOp_Cmp::make_પદાવલિ(e, cv::CMP_EQ, a, s);
    પાછા e;
}

Mat_પદાવલિ operator == (દ્વિગુણ s, અચલ Mat& a)
{
    checkOperandsExist(a);
    Mat_પદાવલિ e;
    MatOp_Cmp::make_પદાવલિ(e, cv::CMP_EQ, a, s);
    પાછા e;
}

Mat_પદાવલિ operator != (અચલ Mat& a, અચલ Mat& b)
{
    checkOperandsExist(a, b);
    Mat_પદાવલિ e;
    MatOp_Cmp::make_પદાવલિ(e, cv::CMP_NE, a, b);
    પાછા e;
}

Mat_પદાવલિ operator != (અચલ Mat& a, દ્વિગુણ s)
{
    checkOperandsExist(a);
    Mat_પદાવલિ e;
    MatOp_Cmp::make_પદાવલિ(e, cv::CMP_NE, a, s);
    પાછા e;
}

Mat_પદાવલિ operator != (દ્વિગુણ s, અચલ Mat& a)
{
    checkOperandsExist(a);
    Mat_પદાવલિ e;
    MatOp_Cmp::make_પદાવલિ(e, cv::CMP_NE, a, s);
    પાછા e;
}

Mat_પદાવલિ operator >= (અચલ Mat& a, અચલ Mat& b)
{
    checkOperandsExist(a, b);
    Mat_પદાવલિ e;
    MatOp_Cmp::make_પદાવલિ(e, cv::CMP_GE, a, b);
    પાછા e;
}

Mat_પદાવલિ operator >= (અચલ Mat& a, દ્વિગુણ s)
{
    checkOperandsExist(a);
    Mat_પદાવલિ e;
    MatOp_Cmp::make_પદાવલિ(e, cv::CMP_GE, a, s);
    પાછા e;
}

Mat_પદાવલિ operator >= (દ્વિગુણ s, અચલ Mat& a)
{
    checkOperandsExist(a);
    Mat_પદાવલિ e;
    MatOp_Cmp::make_પદાવલિ(e, cv::CMP_LE, a, s);
    પાછા e;
}

Mat_પદાવલિ operator > (અચલ Mat& a, અચલ Mat& b)
{
    checkOperandsExist(a, b);
    Mat_પદાવલિ e;
    MatOp_Cmp::make_પદાવલિ(e, cv::CMP_GT, a, b);
    પાછા e;
}

Mat_પદાવલિ operator > (અચલ Mat& a, દ્વિગુણ s)
{
    checkOperandsExist(a);
    Mat_પદાવલિ e;
    MatOp_Cmp::make_પદાવલિ(e, cv::CMP_GT, a, s);
    પાછા e;
}

Mat_પદાવલિ operator > (દ્વિગુણ s, અચલ Mat& a)
{
    checkOperandsExist(a);
    Mat_પદાવલિ e;
    MatOp_Cmp::make_પદાવલિ(e, cv::CMP_LT, a, s);
    પાછા e;
}

Mat_પદાવલિ min(અચલ Mat& a, અચલ Mat& b)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a, b);
    Mat_પદાવલિ e;
    MatOp_Bin::make_પદાવલિ(e, 'm', a, b);
    પાછા e;
}

Mat_પદાવલિ min(અચલ Mat& a, દ્વિગુણ s)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    Mat_પદાવલિ e;
    MatOp_Bin::make_પદાવલિ(e, 'n', a, s);
    પાછા e;
}

Mat_પદાવલિ min(દ્વિગુણ s, અચલ Mat& a)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    Mat_પદાવલિ e;
    MatOp_Bin::make_પદાવલિ(e, 'n', a, s);
    પાછા e;
}

Mat_પદાવલિ max(અચલ Mat& a, અચલ Mat& b)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a, b);
    Mat_પદાવલિ e;
    MatOp_Bin::make_પદાવલિ(e, 'M', a, b);
    પાછા e;
}

Mat_પદાવલિ max(અચલ Mat& a, દ્વિગુણ s)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    Mat_પદાવલિ e;
    MatOp_Bin::make_પદાવલિ(e, 'N', a, s);
    પાછા e;
}

Mat_પદાવલિ max(દ્વિગુણ s, અચલ Mat& a)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    Mat_પદાવલિ e;
    MatOp_Bin::make_પદાવલિ(e, 'N', a, s);
    પાછા e;
}

Mat_પદાવલિ operator & (અચલ Mat& a, અચલ Mat& b)
{
    checkOperandsExist(a, b);
    Mat_પદાવલિ e;
    MatOp_Bin::make_પદાવલિ(e, '&', a, b);
    પાછા e;
}

Mat_પદાવલિ operator & (અચલ Mat& a, અચલ Scalar& s)
{
    checkOperandsExist(a);
    Mat_પદાવલિ e;
    MatOp_Bin::make_પદાવલિ(e, '&', a, s);
    પાછા e;
}

Mat_પદાવલિ operator & (અચલ Scalar& s, અચલ Mat& a)
{
    checkOperandsExist(a);
    Mat_પદાવલિ e;
    MatOp_Bin::make_પદાવલિ(e, '&', a, s);
    પાછા e;
}

Mat_પદાવલિ operator | (અચલ Mat& a, અચલ Mat& b)
{
    checkOperandsExist(a, b);
    Mat_પદાવલિ e;
    MatOp_Bin::make_પદાવલિ(e, '|', a, b);
    પાછા e;
}

Mat_પદાવલિ operator | (અચલ Mat& a, અચલ Scalar& s)
{
    checkOperandsExist(a);
    Mat_પદાવલિ e;
    MatOp_Bin::make_પદાવલિ(e, '|', a, s);
    પાછા e;
}

Mat_પદાવલિ operator | (અચલ Scalar& s, અચલ Mat& a)
{
    checkOperandsExist(a);
    Mat_પદાવલિ e;
    MatOp_Bin::make_પદાવલિ(e, '|', a, s);
    પાછા e;
}

Mat_પદાવલિ operator ^ (અચલ Mat& a, અચલ Mat& b)
{
    checkOperandsExist(a, b);
    Mat_પદાવલિ e;
    MatOp_Bin::make_પદાવલિ(e, '^', a, b);
    પાછા e;
}

Mat_પદાવલિ operator ^ (અચલ Mat& a, અચલ Scalar& s)
{
    checkOperandsExist(a);
    Mat_પદાવલિ e;
    MatOp_Bin::make_પદાવલિ(e, '^', a, s);
    પાછા e;
}

Mat_પદાવલિ operator ^ (અચલ Scalar& s, અચલ Mat& a)
{
    checkOperandsExist(a);
    Mat_પદાવલિ e;
    MatOp_Bin::make_પદાવલિ(e, '^', a, s);
    પાછા e;
}

Mat_પદાવલિ operator ~(અચલ Mat& a)
{
    checkOperandsExist(a);
    Mat_પદાવલિ e;
    MatOp_Bin::make_પદાવલિ(e, '~', a, Scalar());
    પાછા e;
}

Mat_પદાવલિ abs(અચલ Mat& a)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    Mat_પદાવલિ e;
    MatOp_Bin::make_પદાવલિ(e, 'a', a, Scalar());
    પાછા e;
}

Mat_પદાવલિ abs(અચલ Mat_પદાવલિ& e)
{
    CV_INSTRUMENT_REGION();

    Mat_પદાવલિ en;
    e.op->abs(e, en);
    પાછા en;
}


Size Mat_પદાવલિ::size() અચલ
{
    જો( isT(*આ) || isInv(*આ) )
        પાછા Size(a.rows, a.cols);
    જો( isSolve(*આ) )
        પાછા Size(b.cols, a.cols);
    જો( isInitializer(*આ) )
        પાછા a.size();
    પાછા op ? op->size(*આ) : Size();
}


પૂર્ણાંક Mat_પદાવલિ::type() અચલ
{
    CV_INSTRUMENT_REGION();

    જો( isInitializer(*આ) )
        પાછા a.type();
    જો( isCmp(*આ) )
        પાછા CV_MAKETYPE(CV_8U, a.channels());
    પાછા op ? op->type(*આ) : -1;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////

શૂન્ય MatOp_Identity::assign(અચલ Mat_પદાવલિ& e, Mat& m, પૂર્ણાંક _type) અચલ
{
    જો( _type == -1 || _type == e.a.type() )
        m = e.a;
    નહીંતર
    {
        CV_Assert( CV_MAT_CN(_type) == e.a.channels() );
        e.a.convertTo(m, _type);
    }
}

અંતઃરેખ શૂન્ય MatOp_Identity::make_પદાવલિ(Mat_પદાવલિ& res, અચલ Mat& m)
{
    res = Mat_પદાવલિ(&g_MatOp_Identity, 0, m, Mat(), Mat(), 1, 0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

શૂન્ય MatOp_AddEx::assign(અચલ Mat_પદાવલિ& e, Mat& m, પૂર્ણાંક _type) અચલ
{
    Mat temp, &dst = _type == -1 || e.a.type() == _type ? m : temp;
    જો( e.b.data )
    {
        જો( e.s == Scalar() || !e.s.isReal() )
        {
            જો( e.alpha == 1 )
            {
                જો( e.beta == 1 )
                    cv::add(e.a, e.b, dst);
                નહીંતર જો( e.beta == -1 )
                    cv::subtract(e.a, e.b, dst);
                નહીંતર
                    cv::scaleAdd(e.b, e.beta, e.a, dst);
            }
            નહીંતર જો( e.beta == 1 )
            {
                જો( e.alpha == -1 )
                    cv::subtract(e.b, e.a, dst);
                નહીંતર
                    cv::scaleAdd(e.a, e.alpha, e.b, dst);
            }
            નહીંતર
                cv::addWeighted(e.a, e.alpha, e.b, e.beta, 0, dst);

            જો( !e.s.isReal() )
                cv::add(dst, e.s, dst);
        }
        નહીંતર
        {
            જો (e.a.channels() > 1)
                CV_LOG_ONCE_WARNING(ખાલી, "OpenCV/MatExpr: processing of multi-channel arrays might be changed in the future: "
                                          "https://github.com/opencv/opencv/issues/16739");
            cv::addWeighted(e.a, e.alpha, e.b, e.beta, e.s[0], dst);
        }
    }
    નહીંતર જો( e.s.isReal() && (dst.data != m.data || fabs(e.alpha) != 1))
    {
        જો (e.a.channels() > 1 && e.s[0] != 0.0)
            CV_LOG_ONCE_WARNING(ખાલી, "OpenCV/MatExpr: processing of multi-channel arrays might be changed in the future: "
                                      "https://github.com/opencv/opencv/issues/16739");
        e.a.convertTo(m, _type, e.alpha, e.s[0]);
        પાછા;
    }
    નહીંતર જો( e.alpha == 1 )
        cv::add(e.a, e.s, dst);
    નહીંતર જો( e.alpha == -1 )
        cv::subtract(e.s, e.a, dst);
    નહીંતર
    {
        e.a.convertTo(dst, e.a.type(), e.alpha);
        cv::add(dst, e.s, dst);
    }

    જો( dst.data != m.data )
        dst.convertTo(m, m.type());
}


શૂન્ય MatOp_AddEx::add(અચલ Mat_પદાવલિ& e, અચલ Scalar& s, Mat_પદાવલિ& res) અચલ
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.s += s;
}


શૂન્ય MatOp_AddEx::subtract(અચલ Scalar& s, અચલ Mat_પદાવલિ& e, Mat_પદાવલિ& res) અચલ
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha = -res.alpha;
    res.beta = -res.beta;
    res.s = s - res.s;
}

શૂન્ય MatOp_AddEx::multiply(અચલ Mat_પદાવલિ& e, દ્વિગુણ s, Mat_પદાવલિ& res) અચલ
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha *= s;
    res.beta *= s;
    res.s *= s;
}

શૂન્ય MatOp_AddEx::divide(દ્વિગુણ s, અચલ Mat_પદાવલિ& e, Mat_પદાવલિ& res) અચલ
{
    CV_INSTRUMENT_REGION();

    જો( isScaled(e) )
        MatOp_Bin::make_પદાવલિ(res, '/', e.a, Mat(), s/e.alpha);
    નહીંતર
        MatOp::divide(s, e, res);
}


શૂન્ય MatOp_AddEx::transpose(અચલ Mat_પદાવલિ& e, Mat_પદાવલિ& res) અચલ
{
    CV_INSTRUMENT_REGION();

    જો( isScaled(e) )
        MatOp_T::make_પદાવલિ(res, e.a, e.alpha);
    નહીંતર
        MatOp::transpose(e, res);
}

શૂન્ય MatOp_AddEx::abs(અચલ Mat_પદાવલિ& e, Mat_પદાવલિ& res) અચલ
{
    CV_INSTRUMENT_REGION();

    જો( (!e.b.data || e.beta == 0) && fabs(e.alpha) == 1 )
        MatOp_Bin::make_પદાવલિ(res, 'a', e.a, -e.s*e.alpha);
    નહીંતર જો( e.b.data && e.alpha + e.beta == 0 && e.alpha*e.beta == -1 )
        MatOp_Bin::make_પદાવલિ(res, 'a', e.a, e.b);
    નહીંતર
        MatOp::abs(e, res);
}

અંતઃરેખ શૂન્ય MatOp_AddEx::make_પદાવલિ(Mat_પદાવલિ& res, અચલ Mat& a, અચલ Mat& b, દ્વિગુણ alpha, દ્વિગુણ beta, અચલ Scalar& s)
{
    res = Mat_પદાવલિ(&g_MatOp_AddEx, 0, a, b, Mat(), alpha, beta, s);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

શૂન્ય MatOp_Bin::assign(અચલ Mat_પદાવલિ& e, Mat& m, પૂર્ણાંક _type) અચલ
{
    Mat temp, &dst = _type == -1 || e.a.type() == _type ? m : temp;

    જો( e.flags == '*' )
        cv::multiply(e.a, e.b, dst, e.alpha);
    નહીંતર જો( e.flags == '/' && e.b.data )
        cv::divide(e.a, e.b, dst, e.alpha);
    નહીંતર જો( e.flags == '/' && !e.b.data )
        cv::divide(e.alpha, e.a, dst );
    નહીંતર જો( e.flags == '&' && e.b.data )
        bitwise_and(e.a, e.b, dst);
    નહીંતર જો( e.flags == '&' && !e.b.data )
        bitwise_and(e.a, e.s, dst);
    નહીંતર જો( e.flags == '|' && e.b.data )
        bitwise_or(e.a, e.b, dst);
    નહીંતર જો( e.flags == '|' && !e.b.data )
        bitwise_or(e.a, e.s, dst);
    નહીંતર જો( e.flags == '^' && e.b.data )
        bitwise_xor(e.a, e.b, dst);
    નહીંતર જો( e.flags == '^' && !e.b.data )
        bitwise_xor(e.a, e.s, dst);
    નહીંતર જો( e.flags == '~' && !e.b.data )
        bitwise_not(e.a, dst);
    નહીંતર જો( e.flags == 'm' )
        cv::min(e.a, e.b, dst);
    નહીંતર જો( e.flags == 'n' )
        cv::min(e.a, e.s[0], dst);
    નહીંતર જો( e.flags == 'M' )
        cv::max(e.a, e.b, dst);
    નહીંતર જો( e.flags == 'N' )
        cv::max(e.a, e.s[0], dst);
    નહીંતર જો( e.flags == 'a' && e.b.data )
        cv::absdiff(e.a, e.b, dst);
    નહીંતર જો( e.flags == 'a' && !e.b.data )
        cv::absdiff(e.a, e.s, dst);
    નહીંતર
        CV_Error(cv::Error::StsError, "Unknown operation");

    જો( dst.data != m.data )
        dst.convertTo(m, _type);
}

શૂન્ય MatOp_Bin::multiply(અચલ Mat_પદાવલિ& e, દ્વિગુણ s, Mat_પદાવલિ& res) અચલ
{
    CV_INSTRUMENT_REGION();

    જો( e.flags == '*' || e.flags == '/' )
    {
        res = e;
        res.alpha *= s;
    }
    નહીંતર
        MatOp::multiply(e, s, res);
}

શૂન્ય MatOp_Bin::divide(દ્વિગુણ s, અચલ Mat_પદાવલિ& e, Mat_પદાવલિ& res) અચલ
{
    CV_INSTRUMENT_REGION();

    જો( e.flags == '/' && (!e.b.data || e.beta == 0) )
        MatOp_AddEx::make_પદાવલિ(res, e.a, Mat(), s/e.alpha, 0);
    નહીંતર
        MatOp::divide(s, e, res);
}

અંતઃરેખ શૂન્ય MatOp_Bin::make_પદાવલિ(Mat_પદાવલિ& res, અક્ષર op, અચલ Mat& a, અચલ Mat& b, દ્વિગુણ scale)
{
    res = Mat_પદાવલિ(&g_MatOp_Bin, op, a, b, Mat(), scale, b.data ? 1 : 0);
}

અંતઃરેખ શૂન્ય MatOp_Bin::make_પદાવલિ(Mat_પદાવલિ& res, અક્ષર op, અચલ Mat& a, અચલ Scalar& s)
{
    res = Mat_પદાવલિ(&g_MatOp_Bin, op, a, Mat(), Mat(), 1, 0, s);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

શૂન્ય MatOp_Cmp::assign(અચલ Mat_પદાવલિ& e, Mat& m, પૂર્ણાંક _type) અચલ
{
    Mat temp, &dst = _type == -1 || _type == CV_8U ? m : temp;

    જો( e.b.data )
        cv::compare(e.a, e.b, dst, e.flags);
    નહીંતર
        cv::compare(e.a, e.alpha, dst, e.flags);

    જો( dst.data != m.data )
        dst.convertTo(m, _type);
}

અંતઃરેખ શૂન્ય MatOp_Cmp::make_પદાવલિ(Mat_પદાવલિ& res, પૂર્ણાંક cmpop, અચલ Mat& a, અચલ Mat& b)
{
    res = Mat_પદાવલિ(&g_MatOp_Cmp, cmpop, a, b, Mat(), 1, 1);
}

અંતઃરેખ શૂન્ય MatOp_Cmp::make_પદાવલિ(Mat_પદાવલિ& res, પૂર્ણાંક cmpop, અચલ Mat& a, દ્વિગુણ alpha)
{
    res = Mat_પદાવલિ(&g_MatOp_Cmp, cmpop, a, Mat(), Mat(), alpha, 1);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

શૂન્ય MatOp_T::assign(અચલ Mat_પદાવલિ& e, Mat& m, પૂર્ણાંક _type) અચલ
{
    Mat temp, &dst = _type == -1 || _type == e.a.type() ? m : temp;

    cv::transpose(e.a, dst);

    જો( dst.data != m.data || e.alpha != 1 )
        dst.convertTo(m, _type, e.alpha);
}

શૂન્ય MatOp_T::multiply(અચલ Mat_પદાવલિ& e, દ્વિગુણ s, Mat_પદાવલિ& res) અચલ
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha *= s;
}

શૂન્ય MatOp_T::transpose(અચલ Mat_પદાવલિ& e, Mat_પદાવલિ& res) અચલ
{
    CV_INSTRUMENT_REGION();

    જો( e.alpha == 1 )
        MatOp_Identity::make_પદાવલિ(res, e.a);
    નહીંતર
        MatOp_AddEx::make_પદાવલિ(res, e.a, Mat(), e.alpha, 0);
}

અંતઃરેખ શૂન્ય MatOp_T::make_પદાવલિ(Mat_પદાવલિ& res, અચલ Mat& a, દ્વિગુણ alpha)
{
    res = Mat_પદાવલિ(&g_MatOp_T, 0, a, Mat(), Mat(), alpha, 0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

શૂન્ય MatOp_GEMM::assign(અચલ Mat_પદાવલિ& e, Mat& m, પૂર્ણાંક _type) અચલ
{
    Mat temp, &dst = _type == -1 || _type == e.a.type() ? m : temp;

    cv::gemm(e.a, e.b, e.alpha, e.c, e.beta, dst, e.flags);
    જો( dst.data != m.data )
        dst.convertTo(m, _type);
}

શૂન્ય MatOp_GEMM::add(અચલ Mat_પદાવલિ& e1, અચલ Mat_પદાવલિ& e2, Mat_પદાવલિ& res) અચલ
{
    CV_INSTRUMENT_REGION();

    તાર્કિક i1 = isIdentity(e1), i2 = isIdentity(e2);
    દ્વિગુણ alpha1 = i1 ? 1 : e1.alpha, alpha2 = i2 ? 1 : e2.alpha;

    જો( isMatProd(e1) && (i2 || isScaled(e2) || isT(e2)) )
        MatOp_GEMM::make_પદાવલિ(res, (e1.flags & ~cv::GEMM_3_T)|(isT(e2) ? cv::GEMM_3_T : 0),
                             e1.a, e1.b, alpha1, e2.a, alpha2);
    નહીંતર જો( isMatProd(e2) && (i1 || isScaled(e1) || isT(e1)) )
        MatOp_GEMM::make_પદાવલિ(res, (e2.flags & ~cv::GEMM_3_T)|(isT(e1) ? cv::GEMM_3_T : 0),
                             e2.a, e2.b, alpha2, e1.a, alpha1);
    નહીંતર જો( આ == e2.op )
        MatOp::add(e1, e2, res);
    નહીંતર
        e2.op->add(e1, e2, res);
}

શૂન્ય MatOp_GEMM::subtract(અચલ Mat_પદાવલિ& e1, અચલ Mat_પદાવલિ& e2, Mat_પદાવલિ& res) અચલ
{
    CV_INSTRUMENT_REGION();

    તાર્કિક i1 = isIdentity(e1), i2 = isIdentity(e2);
    દ્વિગુણ alpha1 = i1 ? 1 : e1.alpha, alpha2 = i2 ? 1 : e2.alpha;

    જો( isMatProd(e1) && (i2 || isScaled(e2) || isT(e2)) )
        MatOp_GEMM::make_પદાવલિ(res, (e1.flags & ~cv::GEMM_3_T)|(isT(e2) ? cv::GEMM_3_T : 0),
                             e1.a, e1.b, alpha1, e2.a, -alpha2);
    નહીંતર જો( isMatProd(e2) && (i1 || isScaled(e1) || isT(e1)) )
        MatOp_GEMM::make_પદાવલિ(res, (e2.flags & ~cv::GEMM_3_T)|(isT(e1) ? cv::GEMM_3_T : 0),
                            e2.a, e2.b, -alpha2, e1.a, alpha1);
    નહીંતર જો( આ == e2.op )
        MatOp::subtract(e1, e2, res);
    નહીંતર
        e2.op->subtract(e1, e2, res);
}

શૂન્ય MatOp_GEMM::multiply(અચલ Mat_પદાવલિ& e, દ્વિગુણ s, Mat_પદાવલિ& res) અચલ
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha *= s;
    res.beta *= s;
}

શૂન્ય MatOp_GEMM::transpose(અચલ Mat_પદાવલિ& e, Mat_પદાવલિ& res) અચલ
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.flags = (!(e.flags & cv::GEMM_1_T) ? cv::GEMM_2_T : 0) |
                (!(e.flags & cv::GEMM_2_T) ? cv::GEMM_1_T : 0) |
                (!(e.flags & cv::GEMM_3_T) ? cv::GEMM_3_T : 0);
    swap(res.a, res.b);
}

અંતઃરેખ શૂન્ય MatOp_GEMM::make_પદાવલિ(Mat_પદાવલિ& res, પૂર્ણાંક flags, અચલ Mat& a, અચલ Mat& b,
                                 દ્વિગુણ alpha, અચલ Mat& c, દ્વિગુણ beta)
{
    res = Mat_પદાવલિ(&g_MatOp_GEMM, flags, a, b, c, alpha, beta);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

શૂન્ય MatOp_Invert::assign(અચલ Mat_પદાવલિ& e, Mat& m, પૂર્ણાંક _type) અચલ
{
    Mat temp, &dst = _type == -1 || _type == e.a.type() ? m : temp;

    cv::invert(e.a, dst, e.flags);
    જો( dst.data != m.data )
        dst.convertTo(m, _type);
}

શૂન્ય MatOp_Invert::matmul(અચલ Mat_પદાવલિ& e1, અચલ Mat_પદાવલિ& e2, Mat_પદાવલિ& res) અચલ
{
    જો( isInv(e1) && isIdentity(e2) )
        MatOp_Solve::make_પદાવલિ(res, e1.flags, e1.a, e2.a);
    નહીંતર જો( આ == e2.op )
        MatOp::matmul(e1, e2, res);
    નહીંતર
        e2.op->matmul(e1, e2, res);
}

અંતઃરેખ શૂન્ય MatOp_Invert::make_પદાવલિ(Mat_પદાવલિ& res, પૂર્ણાંક method, અચલ Mat& m)
{
    res = Mat_પદાવલિ(&g_MatOp_Invert, method, m, Mat(), Mat(), 1, 0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

શૂન્ય MatOp_Solve::assign(અચલ Mat_પદાવલિ& e, Mat& m, પૂર્ણાંક _type) અચલ
{
    Mat temp, &dst = _type == -1 || _type == e.a.type() ? m : temp;

    cv::solve(e.a, e.b, dst, e.flags);
    જો( dst.data != m.data )
        dst.convertTo(m, _type);
}

અંતઃરેખ શૂન્ય MatOp_Solve::make_પદાવલિ(Mat_પદાવલિ& res, પૂર્ણાંક method, અચલ Mat& a, અચલ Mat& b)
{
    res = Mat_પદાવલિ(&g_MatOp_Solve, method, a, b, Mat(), 1, 1);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

શૂન્ય MatOp_Initializer::assign(અચલ Mat_પદાવલિ& e, Mat& m, પૂર્ણાંક _type) અચલ
{
    જો( _type == -1 )
        _type = e.a.type();

    m.create(e.a.size, _type);

    જો( e.flags == 'I' && e.a.dims <= 2 )
        setIdentity(m, Scalar(e.alpha));
    નહીંતર જો( e.flags == '0' )
        m = Scalar();
    નહીંતર જો( e.flags == '1' )
        m = Scalar(e.alpha);
    નહીંતર
        CV_Error(cv::Error::StsError, "Invalid matrix initializer type");
}

શૂન્ય MatOp_Initializer::multiply(અચલ Mat_પદાવલિ& e, દ્વિગુણ s, Mat_પદાવલિ& res) અચલ
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha *= s;
}

અંતઃરેખ શૂન્ય MatOp_Initializer::make_પદાવલિ(Mat_પદાવલિ& res, પૂર્ણાંક method, Size sz, પૂર્ણાંક type, દ્વિગુણ alpha)
{
    res = Mat_પદાવલિ(getGlobalMatOpInitializer(), method, Mat(sz, type, (શૂન્ય*)(size_t)0xEEEEEEEE), Mat(), Mat(), alpha, 0);
}

અંતઃરેખ શૂન્ય MatOp_Initializer::make_પદાવલિ(Mat_પદાવલિ& res, પૂર્ણાંક method, પૂર્ણાંક ndims, અચલ પૂર્ણાંક* sizes, પૂર્ણાંક type, દ્વિગુણ alpha)
{
    res = Mat_પદાવલિ(getGlobalMatOpInitializer(), method, Mat(ndims, sizes, type, (શૂન્ય*)(size_t)0xEEEEEEEE), Mat(), Mat(), alpha, 0);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

Mat_પદાવલિ Mat::t() અચલ
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(*આ);
    Mat_પદાવલિ e;
    MatOp_T::make_પદાવલિ(e, *આ);
    પાછા e;
}

Mat_પદાવલિ Mat::inv(પૂર્ણાંક method) અચલ
{
    CV_INSTRUMENT_REGION();

    Mat_પદાવલિ e;
    MatOp_Invert::make_પદાવલિ(e, method, *આ);
    પાછા e;
}


Mat_પદાવલિ Mat::mul(આગત_Array m, દ્વિગુણ scale) અચલ
{
    CV_INSTRUMENT_REGION();

    Mat b = m.getMat();
    // Unless the argument is a refcounted Mat/UMat, the header returned by getMat() may be
    // a non-owning view of caller memory (e.g. a scalar bound to _InputArray(const double&),
    // a Matx or a Vec) that the returned MatExpr can outlive, so snapshot it.
    // See https://github.com/opencv/opencv/issues/23577
    જો( !m.isMat() && !m.isUMat() )
        b = b.clone();

    Mat_પદાવલિ e;
    MatOp_Bin::make_પદાવલિ(e, '*', *આ, b, scale);
    પાછા e;
}

Mat_પદાવલિ Mat::zeros(પૂર્ણાંક rows, પૂર્ણાંક cols, પૂર્ણાંક type)
{
    CV_INSTRUMENT_REGION();

    Mat_પદાવલિ e;
    MatOp_Initializer::make_પદાવલિ(e, '0', Size(cols, rows), type);
    પાછા e;
}

Mat_પદાવલિ Mat::zeros(Size size, પૂર્ણાંક type)
{
    CV_INSTRUMENT_REGION();

    Mat_પદાવલિ e;
    MatOp_Initializer::make_પદાવલિ(e, '0', size, type);
    પાછા e;
}

Mat_પદાવલિ Mat::zeros(પૂર્ણાંક ndims, અચલ પૂર્ણાંક* sizes, પૂર્ણાંક type)
{
    CV_INSTRUMENT_REGION();

    Mat_પદાવલિ e;
    MatOp_Initializer::make_પદાવલિ(e, '0', ndims, sizes, type);
    પાછા e;
}

Mat_પદાવલિ Mat::zeros(અચલ MatShape& shape, પૂર્ણાંક type)
{
    CV_INSTRUMENT_REGION();

    Mat_પદાવલિ e;
    MatOp_Initializer::make_પદાવલિ(e, '0', shape.dims, shape.p, type);
    પાછા e;
}

Mat_પદાવલિ Mat::ones(પૂર્ણાંક rows, પૂર્ણાંક cols, પૂર્ણાંક type)
{
    CV_INSTRUMENT_REGION();

    Mat_પદાવલિ e;
    MatOp_Initializer::make_પદાવલિ(e, '1', Size(cols, rows), type);
    પાછા e;
}

Mat_પદાવલિ Mat::ones(Size size, પૂર્ણાંક type)
{
    CV_INSTRUMENT_REGION();

    Mat_પદાવલિ e;
    MatOp_Initializer::make_પદાવલિ(e, '1', size, type);
    પાછા e;
}

Mat_પદાવલિ Mat::ones(પૂર્ણાંક ndims, અચલ પૂર્ણાંક* sizes, પૂર્ણાંક type)
{
    CV_INSTRUMENT_REGION();

    Mat_પદાવલિ e;
    MatOp_Initializer::make_પદાવલિ(e, '1', ndims, sizes, type);
    પાછા e;
}

Mat_પદાવલિ Mat::ones(અચલ MatShape& shape, પૂર્ણાંક type)
{
    CV_INSTRUMENT_REGION();

    Mat_પદાવલિ e;
    MatOp_Initializer::make_પદાવલિ(e, '1', shape.dims, shape.p, type);
    પાછા e;
}

Mat_પદાવલિ Mat::eye(પૂર્ણાંક rows, પૂર્ણાંક cols, પૂર્ણાંક type)
{
    CV_INSTRUMENT_REGION();

    Mat_પદાવલિ e;
    MatOp_Initializer::make_પદાવલિ(e, 'I', Size(cols, rows), type);
    પાછા e;
}

Mat_પદાવલિ Mat::eye(Size size, પૂર્ણાંક type)
{
    CV_INSTRUMENT_REGION();

    Mat_પદાવલિ e;
    MatOp_Initializer::make_પદાવલિ(e, 'I', size, type);
    પાછા e;
}

શૂન્ય Mat_પદાવલિ::swap(Mat_પદાવલિ& other)
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

આગત_Array_2::આગત_Array_2(અચલ Mat_પદાવલિ& પદાવલિ)
{
    જો (!isIdentity(પદાવલિ))
    {
        Mat result = પદાવલિ;  // TODO improve through refcount == 1 of expr.a (inplace operation is possible - except gemm?)
        Mat_પદાવલિ result_પદાવલિ(result);
        swap(const_cast<Mat_પદાવલિ&>(પદાવલિ), result_પદાવલિ);
    }
    CV_Assert(isIdentity(પદાવલિ));
    init(FIXED_TYPE + FIXED_SIZE + MAT + ACCESS_READ, &પદાવલિ.a);
}

} // cv::
