// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


/* ////////////////////////////////////////////////////////////////////
//
//  Mat basic operations: Copy, Set
//
// */

#అన్తర్భావ "precomp.hpp"
#అన్తర్భావ <opencv2/core/utils/logger.hpp>

నామాకాశ cv
{

//This and its overload below are used in various MatExpr operator overloads
//implemented to check that Matrix operands exist.
స్థిర శూన్య checkOperandsExist(స్థిరరాశి Mat& a)
{
    ఒకవేళ (a.empty())
    {
        CV_Error(cv::Error::StsBadArg, "Matrix operand is an empty matrix.");
    }
}

స్థిర శూన్య checkOperandsExist(స్థిరరాశి Mat& a, స్థిరరాశి Mat& b)
{
    ఒకవేళ (a.empty() || b.empty())
    {
        CV_Error(cv::Error::StsBadArg, "One or more matrix operands are empty.");
    }
}


తరగతి Mat_Op_తటస్థం CV_FINAL : బహిరంగ MatOp
{
బహిరంగ:
    Mat_Op_తటస్థం() {}
    మాయా ~Mat_Op_తటస్థం() {}

    తార్కిక elementWise(స్థిరరాశి Mat_సమాసం& /*సమాసం*/) స్థిరరాశి CV_OVERRIDE { తిరిగి నిజం; }
    శూన్య assign(స్థిరరాశి Mat_సమాసం& సమాసం, Mat& m, పూర్ణాంకం type=-1) స్థిరరాశి CV_OVERRIDE;

    స్థిర శూన్య make_సమాసం(Mat_సమాసం& res, స్థిరరాశి Mat& m);
};

స్థిర Mat_Op_తటస్థం g_Mat_Op_తటస్థం;

తరగతి Mat_Op_కూడు_Ex CV_FINAL : బహిరంగ MatOp
{
బహిరంగ:
    Mat_Op_కూడు_Ex() {}
    మాయా ~Mat_Op_కూడు_Ex() {}

    తార్కిక elementWise(స్థిరరాశి Mat_సమాసం& /*సమాసం*/) స్థిరరాశి CV_OVERRIDE { తిరిగి నిజం; }
    శూన్య assign(స్థిరరాశి Mat_సమాసం& సమాసం, Mat& m, పూర్ణాంకం type=-1) స్థిరరాశి CV_OVERRIDE;

    శూన్య కూడు(స్థిరరాశి Mat_సమాసం& e1, స్థిరరాశి Scalar& s, Mat_సమాసం& res) స్థిరరాశి CV_OVERRIDE;
    శూన్య వ్యవకలనం(స్థిరరాశి Scalar& s, స్థిరరాశి Mat_సమాసం& సమాసం, Mat_సమాసం& res) స్థిరరాశి CV_OVERRIDE;
    శూన్య multiply(స్థిరరాశి Mat_సమాసం& e1, ద్విగుణ s, Mat_సమాసం& res) స్థిరరాశి CV_OVERRIDE;
    శూన్య divide(ద్విగుణ s, స్థిరరాశి Mat_సమాసం& e, Mat_సమాసం& res) స్థిరరాశి CV_OVERRIDE;

    శూన్య transpose(స్థిరరాశి Mat_సమాసం& e1, Mat_సమాసం& res) స్థిరరాశి CV_OVERRIDE;
    శూన్య నిరపేక్ష(స్థిరరాశి Mat_సమాసం& సమాసం, Mat_సమాసం& res) స్థిరరాశి CV_OVERRIDE;

    స్థిర శూన్య make_సమాసం(Mat_సమాసం& res, స్థిరరాశి Mat& a, స్థిరరాశి Mat& b, ద్విగుణ alpha, ద్విగుణ beta, స్థిరరాశి Scalar& s=Scalar());
};

స్థిర Mat_Op_కూడు_Ex g_Mat_Op_కూడు_Ex;

తరగతి MatOp_Bin CV_FINAL : బహిరంగ MatOp
{
బహిరంగ:
    MatOp_Bin() {}
    మాయా ~MatOp_Bin() {}

    తార్కిక elementWise(స్థిరరాశి Mat_సమాసం& /*సమాసం*/) స్థిరరాశి CV_OVERRIDE { తిరిగి నిజం; }
    శూన్య assign(స్థిరరాశి Mat_సమాసం& సమాసం, Mat& m, పూర్ణాంకం type=-1) స్థిరరాశి CV_OVERRIDE;

    శూన్య multiply(స్థిరరాశి Mat_సమాసం& e1, ద్విగుణ s, Mat_సమాసం& res) స్థిరరాశి CV_OVERRIDE;
    శూన్య divide(ద్విగుణ s, స్థిరరాశి Mat_సమాసం& e, Mat_సమాసం& res) స్థిరరాశి CV_OVERRIDE;

    స్థిర శూన్య make_సమాసం(Mat_సమాసం& res, అక్షరం op, స్థిరరాశి Mat& a, స్థిరరాశి Mat& b, ద్విగుణ scale=1);
    స్థిర శూన్య make_సమాసం(Mat_సమాసం& res, అక్షరం op, స్థిరరాశి Mat& a, స్థిరరాశి Scalar& s);
};

స్థిర MatOp_Bin g_MatOp_Bin;

తరగతి MatOp_Cmp CV_FINAL : బహిరంగ MatOp
{
బహిరంగ:
    MatOp_Cmp() {}
    మాయా ~MatOp_Cmp() {}

    తార్కిక elementWise(స్థిరరాశి Mat_సమాసం& /*సమాసం*/) స్థిరరాశి CV_OVERRIDE { తిరిగి నిజం; }
    శూన్య assign(స్థిరరాశి Mat_సమాసం& సమాసం, Mat& m, పూర్ణాంకం type=-1) స్థిరరాశి CV_OVERRIDE;

    స్థిర శూన్య make_సమాసం(Mat_సమాసం& res, పూర్ణాంకం cmpop, స్థిరరాశి Mat& a, స్థిరరాశి Mat& b);
    స్థిర శూన్య make_సమాసం(Mat_సమాసం& res, పూర్ణాంకం cmpop, స్థిరరాశి Mat& a, ద్విగుణ alpha);
};

స్థిర MatOp_Cmp g_MatOp_Cmp;

తరగతి MatOp_GEMM CV_FINAL : బహిరంగ MatOp
{
బహిరంగ:
    MatOp_GEMM() {}
    మాయా ~MatOp_GEMM() {}

    తార్కిక elementWise(స్థిరరాశి Mat_సమాసం& /*సమాసం*/) స్థిరరాశి CV_OVERRIDE { తిరిగి అబద్ధం; }
    శూన్య assign(స్థిరరాశి Mat_సమాసం& సమాసం, Mat& m, పూర్ణాంకం type=-1) స్థిరరాశి CV_OVERRIDE;

    శూన్య కూడు(స్థిరరాశి Mat_సమాసం& e1, స్థిరరాశి Mat_సమాసం& e2, Mat_సమాసం& res) స్థిరరాశి CV_OVERRIDE;
    శూన్య వ్యవకలనం(స్థిరరాశి Mat_సమాసం& e1, స్థిరరాశి Mat_సమాసం& e2, Mat_సమాసం& res) స్థిరరాశి CV_OVERRIDE;
    శూన్య multiply(స్థిరరాశి Mat_సమాసం& e, ద్విగుణ s, Mat_సమాసం& res) స్థిరరాశి CV_OVERRIDE;

    శూన్య transpose(స్థిరరాశి Mat_సమాసం& సమాసం, Mat_సమాసం& res) స్థిరరాశి CV_OVERRIDE;

    కొలత size(స్థిరరాశి Mat_సమాసం& సమాసం) స్థిరరాశి CV_OVERRIDE
    {
        తిరిగి కొలత(
            (సమాసం.flags & GEMM_2_T) ? సమాసం.b.అడ్డువరుసలు : సమాసం.b.నిలువుల_సంఖ్య,
            (సమాసం.flags & GEMM_1_T) ? సమాసం.a.నిలువుల_సంఖ్య : సమాసం.a.అడ్డువరుసలు
        );
    }

    స్థిర శూన్య make_సమాసం(Mat_సమాసం& res, పూర్ణాంకం flags, స్థిరరాశి Mat& a, స్థిరరాశి Mat& b,
                         ద్విగుణ alpha=1, స్థిరరాశి Mat& c=Mat(), ద్విగుణ beta=1);

};

స్థిర MatOp_GEMM g_MatOp_GEMM;

తరగతి MatOp_Invert CV_FINAL : బహిరంగ MatOp
{
బహిరంగ:
    MatOp_Invert() {}
    మాయా ~MatOp_Invert() {}

    తార్కిక elementWise(స్థిరరాశి Mat_సమాసం& /*సమాసం*/) స్థిరరాశి CV_OVERRIDE { తిరిగి అబద్ధం; }
    శూన్య assign(స్థిరరాశి Mat_సమాసం& సమాసం, Mat& m, పూర్ణాంకం type=-1) స్థిరరాశి CV_OVERRIDE;

    శూన్య matmul(స్థిరరాశి Mat_సమాసం& expr1, స్థిరరాశి Mat_సమాసం& expr2, Mat_సమాసం& res) స్థిరరాశి CV_OVERRIDE;

    స్థిర శూన్య make_సమాసం(Mat_సమాసం& res, పూర్ణాంకం పద్ధతి, స్థిరరాశి Mat& m);
};

స్థిర MatOp_Invert g_MatOp_Invert;

తరగతి MatOp_T CV_FINAL : బహిరంగ MatOp
{
బహిరంగ:
    MatOp_T() {}
    మాయా ~MatOp_T() {}

    తార్కిక elementWise(స్థిరరాశి Mat_సమాసం& /*సమాసం*/) స్థిరరాశి CV_OVERRIDE { తిరిగి అబద్ధం; }
    శూన్య assign(స్థిరరాశి Mat_సమాసం& సమాసం, Mat& m, పూర్ణాంకం type=-1) స్థిరరాశి CV_OVERRIDE;

    శూన్య multiply(స్థిరరాశి Mat_సమాసం& e1, ద్విగుణ s, Mat_సమాసం& res) స్థిరరాశి CV_OVERRIDE;
    శూన్య transpose(స్థిరరాశి Mat_సమాసం& సమాసం, Mat_సమాసం& res) స్థిరరాశి CV_OVERRIDE;

    స్థిర శూన్య make_సమాసం(Mat_సమాసం& res, స్థిరరాశి Mat& a, ద్విగుణ alpha=1);
};

స్థిర MatOp_T g_MatOp_T;

తరగతి MatOp_Solve CV_FINAL : బహిరంగ MatOp
{
బహిరంగ:
    MatOp_Solve() {}
    మాయా ~MatOp_Solve() {}

    తార్కిక elementWise(స్థిరరాశి Mat_సమాసం& /*సమాసం*/) స్థిరరాశి CV_OVERRIDE { తిరిగి అబద్ధం; }
    శూన్య assign(స్థిరరాశి Mat_సమాసం& సమాసం, Mat& m, పూర్ణాంకం type=-1) స్థిరరాశి CV_OVERRIDE;

    స్థిర శూన్య make_సమాసం(Mat_సమాసం& res, పూర్ణాంకం పద్ధతి, స్థిరరాశి Mat& a, స్థిరరాశి Mat& b);
};

స్థిర MatOp_Solve g_MatOp_Solve;

తరగతి MatOp_Initializer CV_FINAL : బహిరంగ MatOp
{
బహిరంగ:
    MatOp_Initializer() {}
    మాయా ~MatOp_Initializer() {}

    తార్కిక elementWise(స్థిరరాశి Mat_సమాసం& /*సమాసం*/) స్థిరరాశి CV_OVERRIDE { తిరిగి అబద్ధం; }
    శూన్య assign(స్థిరరాశి Mat_సమాసం& సమాసం, Mat& m, పూర్ణాంకం type=-1) స్థిరరాశి CV_OVERRIDE;

    శూన్య multiply(స్థిరరాశి Mat_సమాసం& e, ద్విగుణ s, Mat_సమాసం& res) స్థిరరాశి CV_OVERRIDE;

    స్థిర శూన్య make_సమాసం(Mat_సమాసం& res, పూర్ణాంకం పద్ధతి, కొలత sz, పూర్ణాంకం type, ద్విగుణ alpha=1);
    స్థిర శూన్య make_సమాసం(Mat_సమాసం& res, పూర్ణాంకం పద్ధతి, పూర్ణాంకం ndims, స్థిరరాశి పూర్ణాంకం* sizes, పూర్ణాంకం type, ద్విగుణ alpha=1);
};

స్థిర MatOp_Initializer* getGlobalMatOpInitializer()
{
    CV_SINGLETON_LAZY_INIT(MatOp_Initializer, కొత్త MatOp_Initializer())
}

స్థిర అంతఃవరుస తార్కిక is_తటస్థం(స్థిరరాశి Mat_సమాసం& e) { తిరిగి e.op == &g_Mat_Op_తటస్థం; }
స్థిర అంతఃవరుస తార్కిక is_కూడు_Ex(స్థిరరాశి Mat_సమాసం& e) { తిరిగి e.op == &g_Mat_Op_కూడు_Ex; }
స్థిర అంతఃవరుస తార్కిక isScaled(స్థిరరాశి Mat_సమాసం& e) { తిరిగి is_కూడు_Ex(e) && (!e.b.దత్తాంశం || e.beta == 0) && e.s == Scalar(); }
స్థిర అంతఃవరుస తార్కిక isBin(స్థిరరాశి Mat_సమాసం& e, అక్షరం c) { తిరిగి e.op == &g_MatOp_Bin && e.flags == c; }
స్థిర అంతఃవరుస తార్కిక isCmp(స్థిరరాశి Mat_సమాసం& e) { తిరిగి e.op == &g_MatOp_Cmp; }
స్థిర అంతఃవరుస తార్కిక isReciprocal(స్థిరరాశి Mat_సమాసం& e) { తిరిగి isBin(e,'/') && (!e.b.దత్తాంశం || e.beta == 0); }
స్థిర అంతఃవరుస తార్కిక isT(స్థిరరాశి Mat_సమాసం& e) { తిరిగి e.op == &g_MatOp_T; }
స్థిర అంతఃవరుస తార్కిక isInv(స్థిరరాశి Mat_సమాసం& e) { తిరిగి e.op == &g_MatOp_Invert; }
స్థిర అంతఃవరుస తార్కిక isSolve(స్థిరరాశి Mat_సమాసం& e) { తిరిగి e.op == &g_MatOp_Solve; }
//static inline bool isGEMM(const MatExpr& e) { return e.op == &g_MatOp_GEMM; }
స్థిర అంతఃవరుస తార్కిక isMatProd(స్థిరరాశి Mat_సమాసం& e) { తిరిగి e.op == &g_MatOp_GEMM && (!e.c.దత్తాంశం || e.beta == 0); }
స్థిర అంతఃవరుస తార్కిక isInitializer(స్థిరరాశి Mat_సమాసం& e) { తిరిగి e.op == getGlobalMatOpInitializer(); }

/////////////////////////////////////////////////////////////////////////////////////////////////////

MatOp::MatOp() {}
MatOp::~MatOp() {}


తార్కిక MatOp::elementWise(స్థిరరాశి Mat_సమాసం& /*సమాసం*/) స్థిరరాశి
{
    తిరిగి అబద్ధం;
}

శూన్య MatOp::roi(స్థిరరాశి Mat_సమాసం& సమాసం, స్థిరరాశి Range& అడ్డువరుస_Range_2, స్థిరరాశి Range& నిలువు_Range_2, Mat_సమాసం& e) స్థిరరాశి
{
    ఒకవేళ( elementWise(సమాసం) )
    {
        e = Mat_సమాసం(సమాసం.op, సమాసం.flags, Mat(), Mat(), Mat(),
                    సమాసం.alpha, సమాసం.beta, సమాసం.s);
        ఒకవేళ(సమాసం.a.దత్తాంశం)
            e.a = సమాసం.a(అడ్డువరుస_Range_2, నిలువు_Range_2);
        ఒకవేళ(సమాసం.b.దత్తాంశం)
            e.b = సమాసం.b(అడ్డువరుస_Range_2, నిలువు_Range_2);
        ఒకవేళ(సమాసం.c.దత్తాంశం)
            e.c = సమాసం.c(అడ్డువరుస_Range_2, నిలువు_Range_2);
    }
    లేకపోతే
    {
        Mat m;
        సమాసం.op->assign(సమాసం, m);
        e = Mat_సమాసం(&g_Mat_Op_తటస్థం, 0, m(అడ్డువరుస_Range_2, నిలువు_Range_2), Mat(), Mat());
    }
}

శూన్య MatOp::diag(స్థిరరాశి Mat_సమాసం& సమాసం, పూర్ణాంకం d, Mat_సమాసం& e) స్థిరరాశి
{
    ఒకవేళ( elementWise(సమాసం) )
    {
        e = Mat_సమాసం(సమాసం.op, సమాసం.flags, Mat(), Mat(), Mat(),
                    సమాసం.alpha, సమాసం.beta, సమాసం.s);
        ఒకవేళ(సమాసం.a.దత్తాంశం)
            e.a = సమాసం.a.diag(d);
        ఒకవేళ(సమాసం.b.దత్తాంశం)
            e.b = సమాసం.b.diag(d);
        ఒకవేళ(సమాసం.c.దత్తాంశం)
            e.c = సమాసం.c.diag(d);
    }
    లేకపోతే
    {
        Mat m;
        సమాసం.op->assign(సమాసం, m);
        e = Mat_సమాసం(&g_Mat_Op_తటస్థం, 0, m.diag(d), Mat(), Mat());
    }
}


శూన్య MatOp::aug_కేటాయించు_కూడు(స్థిరరాశి Mat_సమాసం& సమాసం, Mat& m) స్థిరరాశి
{
    Mat తాత్కాలిక_2;
    సమాసం.op->assign(సమాసం, తాత్కాలిక_2);
    m += తాత్కాలిక_2;
}


శూన్య MatOp::aug_కేటాయించు_వ్యవకలనం(స్థిరరాశి Mat_సమాసం& సమాసం, Mat& m) స్థిరరాశి
{
    Mat తాత్కాలిక_2;
    సమాసం.op->assign(సమాసం, తాత్కాలిక_2);
    m -= తాత్కాలిక_2;
}


శూన్య MatOp::aug_కేటాయించు_Multiply(స్థిరరాశి Mat_సమాసం& సమాసం, Mat& m) స్థిరరాశి
{
    Mat తాత్కాలిక_2;
    సమాసం.op->assign(సమాసం, తాత్కాలిక_2);
    m *= తాత్కాలిక_2;
}


శూన్య MatOp::aug_కేటాయించు_Divide(స్థిరరాశి Mat_సమాసం& సమాసం, Mat& m) స్థిరరాశి
{
    Mat తాత్కాలిక_2;
    సమాసం.op->assign(సమాసం, తాత్కాలిక_2);
    m /= తాత్కాలిక_2;
}


శూన్య MatOp::aug_కేటాయించు_And(స్థిరరాశి Mat_సమాసం& సమాసం, Mat& m) స్థిరరాశి
{
    Mat తాత్కాలిక_2;
    సమాసం.op->assign(సమాసం, తాత్కాలిక_2);
    m &= తాత్కాలిక_2;
}


శూన్య MatOp::aug_కేటాయించు_Or(స్థిరరాశి Mat_సమాసం& సమాసం, Mat& m) స్థిరరాశి
{
    Mat తాత్కాలిక_2;
    సమాసం.op->assign(సమాసం, తాత్కాలిక_2);
    m |= తాత్కాలిక_2;
}


శూన్య MatOp::aug_కేటాయించు_Xor(స్థిరరాశి Mat_సమాసం& సమాసం, Mat& m) స్థిరరాశి
{
    Mat తాత్కాలిక_2;
    సమాసం.op->assign(సమాసం, తాత్కాలిక_2);
    m ^= తాత్కాలిక_2;
}


శూన్య MatOp::కూడు(స్థిరరాశి Mat_సమాసం& e1, స్థిరరాశి Mat_సమాసం& e2, Mat_సమాసం& res) స్థిరరాశి
{
    CV_INSTRUMENT_REGION();

    ఒకవేళ( ఇది == e2.op )
    {
        ద్విగుణ alpha = 1, beta = 1;
        Scalar s;
        Mat m1, m2;
        ఒకవేళ( is_కూడు_Ex(e1) && (!e1.b.దత్తాంశం || e1.beta == 0) )
        {
            m1 = e1.a;
            alpha = e1.alpha;
            s = e1.s;
        }
        లేకపోతే
            e1.op->assign(e1, m1);

        ఒకవేళ( is_కూడు_Ex(e2) && (!e2.b.దత్తాంశం || e2.beta == 0) )
        {
            m2 = e2.a;
            beta = e2.alpha;
            s += e2.s;
        }
        లేకపోతే
            e2.op->assign(e2, m2);
        Mat_Op_కూడు_Ex::make_సమాసం(res, m1, m2, alpha, beta, s);
    }
    లేకపోతే
        e2.op->కూడు(e1, e2, res);
}


శూన్య MatOp::కూడు(స్థిరరాశి Mat_సమాసం& expr1, స్థిరరాశి Scalar& s, Mat_సమాసం& res) స్థిరరాశి
{
    CV_INSTRUMENT_REGION();

    Mat m1;
    expr1.op->assign(expr1, m1);
    Mat_Op_కూడు_Ex::make_సమాసం(res, m1, Mat(), 1, 0, s);
}


శూన్య MatOp::వ్యవకలనం(స్థిరరాశి Mat_సమాసం& e1, స్థిరరాశి Mat_సమాసం& e2, Mat_సమాసం& res) స్థిరరాశి
{
    CV_INSTRUMENT_REGION();

    ఒకవేళ( ఇది == e2.op )
    {
        ద్విగుణ alpha = 1, beta = -1;
        Scalar s;
        Mat m1, m2;
        ఒకవేళ( is_కూడు_Ex(e1) && (!e1.b.దత్తాంశం || e1.beta == 0) )
        {
            m1 = e1.a;
            alpha = e1.alpha;
            s = e1.s;
        }
        లేకపోతే
            e1.op->assign(e1, m1);

        ఒకవేళ( is_కూడు_Ex(e2) && (!e2.b.దత్తాంశం || e2.beta == 0) )
        {
            m2 = e2.a;
            beta = -e2.alpha;
            s -= e2.s;
        }
        లేకపోతే
            e2.op->assign(e2, m2);
        Mat_Op_కూడు_Ex::make_సమాసం(res, m1, m2, alpha, beta, s);
    }
    లేకపోతే
        e2.op->వ్యవకలనం(e1, e2, res);
}


శూన్య MatOp::వ్యవకలనం(స్థిరరాశి Scalar& s, స్థిరరాశి Mat_సమాసం& సమాసం, Mat_సమాసం& res) స్థిరరాశి
{
    CV_INSTRUMENT_REGION();

    Mat m;
    సమాసం.op->assign(సమాసం, m);
    Mat_Op_కూడు_Ex::make_సమాసం(res, m, Mat(), -1, 0, s);
}


శూన్య MatOp::multiply(స్థిరరాశి Mat_సమాసం& e1, స్థిరరాశి Mat_సమాసం& e2, Mat_సమాసం& res, ద్విగుణ scale) స్థిరరాశి
{
    CV_INSTRUMENT_REGION();

    ఒకవేళ( ఇది == e2.op )
    {
        Mat m1, m2;

        ఒకవేళ( isReciprocal(e1) )
        {
            ఒకవేళ( isScaled(e2) )
            {
                scale *= e2.alpha;
                m2 = e2.a;
            }
            లేకపోతే
                e2.op->assign(e2, m2);

            MatOp_Bin::make_సమాసం(res, '/', m2, e1.a, scale/e1.alpha);
        }
        లేకపోతే
        {
            అక్షరం op = '*';
            ఒకవేళ( isScaled(e1) )
            {
                m1 = e1.a;
                scale *= e1.alpha;
            }
            లేకపోతే
                e1.op->assign(e1, m1);

            ఒకవేళ( isScaled(e2) )
            {
                m2 = e2.a;
                scale *= e2.alpha;
            }
            లేకపోతే ఒకవేళ( isReciprocal(e2) )
            {
                op = '/';
                m2 = e2.a;
                scale *= e2.alpha;
            }
            లేకపోతే
                e2.op->assign(e2, m2);

            MatOp_Bin::make_సమాసం(res, op, m1, m2, scale);
        }
    }
    లేకపోతే
        e2.op->multiply(e1, e2, res, scale);
}


శూన్య MatOp::multiply(స్థిరరాశి Mat_సమాసం& సమాసం, ద్విగుణ s, Mat_సమాసం& res) స్థిరరాశి
{
    CV_INSTRUMENT_REGION();

    Mat m;
    సమాసం.op->assign(సమాసం, m);
    Mat_Op_కూడు_Ex::make_సమాసం(res, m, Mat(), s, 0);
}


శూన్య MatOp::divide(స్థిరరాశి Mat_సమాసం& e1, స్థిరరాశి Mat_సమాసం& e2, Mat_సమాసం& res, ద్విగుణ scale) స్థిరరాశి
{
    CV_INSTRUMENT_REGION();

    ఒకవేళ( ఇది == e2.op )
    {
        ఒకవేళ( isReciprocal(e1) && isReciprocal(e2) )
            MatOp_Bin::make_సమాసం(res, '/', e2.a, e1.a, e1.alpha/e2.alpha);
        లేకపోతే
        {
            Mat m1, m2;
            అక్షరం op = '/';

            ఒకవేళ( isScaled(e1) )
            {
                m1 = e1.a;
                scale *= e1.alpha;
            }
            లేకపోతే
                e1.op->assign(e1, m1);

            ఒకవేళ( isScaled(e2) )
            {
                m2 = e2.a;
                scale /= e2.alpha;
            }
            లేకపోతే ఒకవేళ( isReciprocal(e2) )
            {
                m2 = e2.a;
                scale /= e2.alpha;
                op = '*';
            }
            లేకపోతే
                e2.op->assign(e2, m2);
            MatOp_Bin::make_సమాసం(res, op, m1, m2, scale);
        }
    }
    లేకపోతే
        e2.op->divide(e1, e2, res, scale);
}


శూన్య MatOp::divide(ద్విగుణ s, స్థిరరాశి Mat_సమాసం& సమాసం, Mat_సమాసం& res) స్థిరరాశి
{
    CV_INSTRUMENT_REGION();

    Mat m;
    సమాసం.op->assign(సమాసం, m);
    MatOp_Bin::make_సమాసం(res, '/', m, Mat(), s);
}


శూన్య MatOp::నిరపేక్ష(స్థిరరాశి Mat_సమాసం& సమాసం, Mat_సమాసం& res) స్థిరరాశి
{
    CV_INSTRUMENT_REGION();

    Mat m;
    సమాసం.op->assign(సమాసం, m);
    MatOp_Bin::make_సమాసం(res, 'a', m, Mat());
}


శూన్య MatOp::transpose(స్థిరరాశి Mat_సమాసం& సమాసం, Mat_సమాసం& res) స్థిరరాశి
{
    CV_INSTRUMENT_REGION();

    Mat m;
    సమాసం.op->assign(సమాసం, m);
    MatOp_T::make_సమాసం(res, m, 1);
}


శూన్య MatOp::matmul(స్థిరరాశి Mat_సమాసం& e1, స్థిరరాశి Mat_సమాసం& e2, Mat_సమాసం& res) స్థిరరాశి
{
    ఒకవేళ( ఇది == e2.op )
    {
        ద్విగుణ scale = 1;
        పూర్ణాంకం flags = 0;
        Mat m1, m2;

        ఒకవేళ( isT(e1) )
        {
            flags = cv::GEMM_1_T;
            scale = e1.alpha;
            m1 = e1.a;
        }
        లేకపోతే ఒకవేళ( isScaled(e1) )
        {
            scale = e1.alpha;
            m1 = e1.a;
        }
        లేకపోతే
            e1.op->assign(e1, m1);

        ఒకవేళ( isT(e2) )
        {
            flags |= cv::GEMM_2_T;
            scale *= e2.alpha;
            m2 = e2.a;
        }
        లేకపోతే ఒకవేళ( isScaled(e2) )
        {
            scale *= e2.alpha;
            m2 = e2.a;
        }
        లేకపోతే
            e2.op->assign(e2, m2);

        MatOp_GEMM::make_సమాసం(res, flags, m1, m2, scale);
    }
    లేకపోతే
        e2.op->matmul(e1, e2, res);
}


శూన్య MatOp::invert(స్థిరరాశి Mat_సమాసం& సమాసం, పూర్ణాంకం పద్ధతి, Mat_సమాసం& res) స్థిరరాశి
{
    Mat m;
    సమాసం.op->assign(సమాసం, m);
    MatOp_Invert::make_సమాసం(res, పద్ధతి, m);
}


కొలత MatOp::size(స్థిరరాశి Mat_సమాసం& సమాసం) స్థిరరాశి
{
    తిరిగి !సమాసం.a.empty() ? సమాసం.a.size() : సమాసం.b.empty() ? సమాసం.b.size() : సమాసం.c.size();
}

పూర్ణాంకం MatOp::type(స్థిరరాశి Mat_సమాసం& సమాసం) స్థిరరాశి
{
    CV_INSTRUMENT_REGION();

    తిరిగి !సమాసం.a.empty() ? సమాసం.a.type() : సమాసం.b.empty() ? సమాసం.b.type() : సమాసం.c.type();
}

//////////////////////////////////////////////////////////////////////////////////////////////////

Mat_సమాసం::Mat_సమాసం(స్థిరరాశి Mat& m) : op(&g_Mat_Op_తటస్థం), flags(0), a(m), b(Mat()), c(Mat()), alpha(1), beta(0), s(Scalar())
{
}

Mat_సమాసం Mat_సమాసం::అడ్డువరుస(పూర్ణాంకం y) స్థిరరాశి
{
    Mat_సమాసం e;
    op->roi(*ఇది, Range(y, y+1), Range::all(), e);
    తిరిగి e;
}

Mat_సమాసం Mat_సమాసం::నిలువు(పూర్ణాంకం x) స్థిరరాశి
{
    Mat_సమాసం e;
    op->roi(*ఇది, Range::all(), Range(x, x+1), e);
    తిరిగి e;
}

Mat_సమాసం Mat_సమాసం::diag(పూర్ణాంకం d) స్థిరరాశి
{
    Mat_సమాసం e;
    op->diag(*ఇది, d, e);
    తిరిగి e;
}

Mat_సమాసం Mat_సమాసం::క్రియాకారకం()( స్థిరరాశి Range& అడ్డువరుస_Range_2, స్థిరరాశి Range& నిలువు_Range_2 ) స్థిరరాశి
{
    Mat_సమాసం e;
    op->roi(*ఇది, అడ్డువరుస_Range_2, నిలువు_Range_2, e);
    తిరిగి e;
}

Mat_సమాసం Mat_సమాసం::క్రియాకారకం()( స్థిరరాశి Rect& roi ) స్థిరరాశి
{
    Mat_సమాసం e;
    op->roi(*ఇది, Range(roi.y, roi.y + roi.ఎత్తు), Range(roi.x, roi.x + roi.వెడల్పు), e);
    తిరిగి e;
}

Mat Mat_సమాసం::cross(స్థిరరాశి Mat& m) స్థిరరాశి
{
    తిరిగి ((Mat)*ఇది).cross(m);
}

ద్విగుణ Mat_సమాసం::dot(స్థిరరాశి Mat& m) స్థిరరాశి
{
    తిరిగి ((Mat)*ఇది).dot(m);
}

Mat_సమాసం Mat_సమాసం::t() స్థిరరాశి
{
    Mat_సమాసం e;
    op->transpose(*ఇది, e);
    తిరిగి e;
}

Mat_సమాసం Mat_సమాసం::inv(పూర్ణాంకం పద్ధతి) స్థిరరాశి
{
    Mat_సమాసం e;
    op->invert(*ఇది, పద్ధతి, e);
    తిరిగి e;
}

Mat_సమాసం Mat_సమాసం::mul(స్థిరరాశి Mat_సమాసం& e, ద్విగుణ scale) స్థిరరాశి
{
    Mat_సమాసం en;
    op->multiply(*ఇది, e, en, scale);
    తిరిగి en;
}

Mat_సమాసం Mat_సమాసం::mul(స్థిరరాశి Mat& m, ద్విగుణ scale) స్థిరరాశి
{
    Mat_సమాసం e;
    op->multiply(*ఇది, Mat_సమాసం(m), e, scale);
    తిరిగి e;
}

Mat_సమాసం క్రియాకారకం + (స్థిరరాశి Mat& a, స్థిరరాశి Mat& b)
{
    checkOperandsExist(a, b);
    Mat_సమాసం e;
    Mat_Op_కూడు_Ex::make_సమాసం(e, a, b, 1, 1);
    తిరిగి e;
}

Mat_సమాసం క్రియాకారకం + (స్థిరరాశి Mat& a, స్థిరరాశి Scalar& s)
{
    checkOperandsExist(a);
    Mat_సమాసం e;
    Mat_Op_కూడు_Ex::make_సమాసం(e, a, Mat(), 1, 0, s);
    తిరిగి e;
}

Mat_సమాసం క్రియాకారకం + (స్థిరరాశి Scalar& s, స్థిరరాశి Mat& a)
{
    checkOperandsExist(a);
    Mat_సమాసం e;
    Mat_Op_కూడు_Ex::make_సమాసం(e, a, Mat(), 1, 0, s);
    తిరిగి e;
}

Mat_సమాసం క్రియాకారకం + (స్థిరరాశి Mat_సమాసం& e, స్థిరరాశి Mat& m)
{
    Mat_సమాసం en;
    e.op->కూడు(e, Mat_సమాసం(m), en);
    తిరిగి en;
}

Mat_సమాసం క్రియాకారకం + (స్థిరరాశి Mat& m, స్థిరరాశి Mat_సమాసం& e)
{
    checkOperandsExist(m);
    Mat_సమాసం en;
    e.op->కూడు(e, Mat_సమాసం(m), en);
    తిరిగి en;
}

Mat_సమాసం క్రియాకారకం + (స్థిరరాశి Mat_సమాసం& e, స్థిరరాశి Scalar& s)
{
    Mat_సమాసం en;
    e.op->కూడు(e, s, en);
    తిరిగి en;
}

Mat_సమాసం క్రియాకారకం + (స్థిరరాశి Scalar& s, స్థిరరాశి Mat_సమాసం& e)
{
    Mat_సమాసం en;
    e.op->కూడు(e, s, en);
    తిరిగి en;
}

Mat_సమాసం క్రియాకారకం + (స్థిరరాశి Mat_సమాసం& e1, స్థిరరాశి Mat_సమాసం& e2)
{
    Mat_సమాసం en;
    e1.op->కూడు(e1, e2, en);
    తిరిగి en;
}

Mat_సమాసం క్రియాకారకం - (స్థిరరాశి Mat& a, స్థిరరాశి Mat& b)
{
    checkOperandsExist(a, b);
    Mat_సమాసం e;
    Mat_Op_కూడు_Ex::make_సమాసం(e, a, b, 1, -1);
    తిరిగి e;
}

Mat_సమాసం క్రియాకారకం - (స్థిరరాశి Mat& a, స్థిరరాశి Scalar& s)
{
    checkOperandsExist(a);
    Mat_సమాసం e;
    Mat_Op_కూడు_Ex::make_సమాసం(e, a, Mat(), 1, 0, -s);
    తిరిగి e;
}

Mat_సమాసం క్రియాకారకం - (స్థిరరాశి Scalar& s, స్థిరరాశి Mat& a)
{
    checkOperandsExist(a);
    Mat_సమాసం e;
    Mat_Op_కూడు_Ex::make_సమాసం(e, a, Mat(), -1, 0, s);
    తిరిగి e;
}

Mat_సమాసం క్రియాకారకం - (స్థిరరాశి Mat_సమాసం& e, స్థిరరాశి Mat& m)
{
    checkOperandsExist(m);
    Mat_సమాసం en;
    e.op->వ్యవకలనం(e, Mat_సమాసం(m), en);
    తిరిగి en;
}

Mat_సమాసం క్రియాకారకం - (స్థిరరాశి Mat& m, స్థిరరాశి Mat_సమాసం& e)
{
    checkOperandsExist(m);
    Mat_సమాసం en;
    e.op->వ్యవకలనం(Mat_సమాసం(m), e, en);
    తిరిగి en;
}

Mat_సమాసం క్రియాకారకం - (స్థిరరాశి Mat_సమాసం& e, స్థిరరాశి Scalar& s)
{
    Mat_సమాసం en;
    e.op->కూడు(e, -s, en);
    తిరిగి en;
}

Mat_సమాసం క్రియాకారకం - (స్థిరరాశి Scalar& s, స్థిరరాశి Mat_సమాసం& e)
{
    Mat_సమాసం en;
    e.op->వ్యవకలనం(s, e, en);
    తిరిగి en;
}

Mat_సమాసం క్రియాకారకం - (స్థిరరాశి Mat_సమాసం& e1, స్థిరరాశి Mat_సమాసం& e2)
{
    Mat_సమాసం en;
    e1.op->వ్యవకలనం(e1, e2, en);
    తిరిగి en;
}

Mat_సమాసం క్రియాకారకం - (స్థిరరాశి Mat& m)
{
    checkOperandsExist(m);
    Mat_సమాసం e;
    Mat_Op_కూడు_Ex::make_సమాసం(e, m, Mat(), -1, 0);
    తిరిగి e;
}

Mat_సమాసం క్రియాకారకం - (స్థిరరాశి Mat_సమాసం& e)
{
    Mat_సమాసం en;
    e.op->వ్యవకలనం(Scalar(0), e, en);
    తిరిగి en;
}

Mat_సమాసం క్రియాకారకం * (స్థిరరాశి Mat& a, స్థిరరాశి Mat& b)
{
    checkOperandsExist(a, b);
    Mat_సమాసం e;
    MatOp_GEMM::make_సమాసం(e, 0, a, b);
    తిరిగి e;
}

Mat_సమాసం క్రియాకారకం * (స్థిరరాశి Mat& a, ద్విగుణ s)
{
    checkOperandsExist(a);
    Mat_సమాసం e;
    Mat_Op_కూడు_Ex::make_సమాసం(e, a, Mat(), s, 0);
    తిరిగి e;
}

Mat_సమాసం క్రియాకారకం * (ద్విగుణ s, స్థిరరాశి Mat& a)
{
    checkOperandsExist(a);
    Mat_సమాసం e;
    Mat_Op_కూడు_Ex::make_సమాసం(e, a, Mat(), s, 0);
    తిరిగి e;
}

Mat_సమాసం క్రియాకారకం * (స్థిరరాశి Mat_సమాసం& e, స్థిరరాశి Mat& m)
{
    Mat_సమాసం en;
    e.op->matmul(e, Mat_సమాసం(m), en);
    తిరిగి en;
}

Mat_సమాసం క్రియాకారకం * (స్థిరరాశి Mat& m, స్థిరరాశి Mat_సమాసం& e)
{
    checkOperandsExist(m);
    Mat_సమాసం en;
    e.op->matmul(Mat_సమాసం(m), e, en);
    తిరిగి en;
}

Mat_సమాసం క్రియాకారకం * (స్థిరరాశి Mat_సమాసం& e, ద్విగుణ s)
{
    Mat_సమాసం en;
    e.op->multiply(e, s, en);
    తిరిగి en;
}

Mat_సమాసం క్రియాకారకం * (ద్విగుణ s, స్థిరరాశి Mat_సమాసం& e)
{
    Mat_సమాసం en;
    e.op->multiply(e, s, en);
    తిరిగి en;
}

Mat_సమాసం క్రియాకారకం * (స్థిరరాశి Mat_సమాసం& e1, స్థిరరాశి Mat_సమాసం& e2)
{
    Mat_సమాసం en;
    e1.op->matmul(e1, e2, en);
    తిరిగి en;
}

Mat_సమాసం క్రియాకారకం / (స్థిరరాశి Mat& a, స్థిరరాశి Mat& b)
{
    checkOperandsExist(a, b);
    Mat_సమాసం e;
    MatOp_Bin::make_సమాసం(e, '/', a, b);
    తిరిగి e;
}

Mat_సమాసం క్రియాకారకం / (స్థిరరాశి Mat& a, ద్విగుణ s)
{
    checkOperandsExist(a);
    Mat_సమాసం e;
    Mat_Op_కూడు_Ex::make_సమాసం(e, a, Mat(), 1./s, 0);
    తిరిగి e;
}

Mat_సమాసం క్రియాకారకం / (ద్విగుణ s, స్థిరరాశి Mat& a)
{
    checkOperandsExist(a);
    Mat_సమాసం e;
    MatOp_Bin::make_సమాసం(e, '/', a, Mat(), s);
    తిరిగి e;
}

Mat_సమాసం క్రియాకారకం / (స్థిరరాశి Mat_సమాసం& e, స్థిరరాశి Mat& m)
{
    Mat_సమాసం en;
    e.op->divide(e, Mat_సమాసం(m), en);
    తిరిగి en;
}

Mat_సమాసం క్రియాకారకం / (స్థిరరాశి Mat& m, స్థిరరాశి Mat_సమాసం& e)
{
    checkOperandsExist(m);
    Mat_సమాసం en;
    e.op->divide(Mat_సమాసం(m), e, en);
    తిరిగి en;
}

Mat_సమాసం క్రియాకారకం / (స్థిరరాశి Mat_సమాసం& e, ద్విగుణ s)
{
    Mat_సమాసం en;
    e.op->multiply(e, 1./s, en);
    తిరిగి en;
}

Mat_సమాసం క్రియాకారకం / (ద్విగుణ s, స్థిరరాశి Mat_సమాసం& e)
{
    Mat_సమాసం en;
    e.op->divide(s, e, en);
    తిరిగి en;
}

Mat_సమాసం క్రియాకారకం / (స్థిరరాశి Mat_సమాసం& e1, స్థిరరాశి Mat_సమాసం& e2)
{
    Mat_సమాసం en;
    e1.op->divide(e1, e2, en);
    తిరిగి en;
}

Mat_సమాసం క్రియాకారకం < (స్థిరరాశి Mat& a, స్థిరరాశి Mat& b)
{
    checkOperandsExist(a, b);
    Mat_సమాసం e;
    MatOp_Cmp::make_సమాసం(e, cv::CMP_LT, a, b);
    తిరిగి e;
}

Mat_సమాసం క్రియాకారకం < (స్థిరరాశి Mat& a, ద్విగుణ s)
{
    checkOperandsExist(a);
    Mat_సమాసం e;
    MatOp_Cmp::make_సమాసం(e, cv::CMP_LT, a, s);
    తిరిగి e;
}

Mat_సమాసం క్రియాకారకం < (ద్విగుణ s, స్థిరరాశి Mat& a)
{
    checkOperandsExist(a);
    Mat_సమాసం e;
    MatOp_Cmp::make_సమాసం(e, cv::CMP_GT, a, s);
    తిరిగి e;
}

Mat_సమాసం క్రియాకారకం <= (స్థిరరాశి Mat& a, స్థిరరాశి Mat& b)
{
    checkOperandsExist(a, b);
    Mat_సమాసం e;
    MatOp_Cmp::make_సమాసం(e, cv::CMP_LE, a, b);
    తిరిగి e;
}

Mat_సమాసం క్రియాకారకం <= (స్థిరరాశి Mat& a, ద్విగుణ s)
{
    checkOperandsExist(a);
    Mat_సమాసం e;
    MatOp_Cmp::make_సమాసం(e, cv::CMP_LE, a, s);
    తిరిగి e;
}

Mat_సమాసం క్రియాకారకం <= (ద్విగుణ s, స్థిరరాశి Mat& a)
{
    checkOperandsExist(a);
    Mat_సమాసం e;
    MatOp_Cmp::make_సమాసం(e, cv::CMP_GE, a, s);
    తిరిగి e;
}

Mat_సమాసం క్రియాకారకం == (స్థిరరాశి Mat& a, స్థిరరాశి Mat& b)
{
    checkOperandsExist(a, b);
    Mat_సమాసం e;
    MatOp_Cmp::make_సమాసం(e, cv::CMP_EQ, a, b);
    తిరిగి e;
}

Mat_సమాసం క్రియాకారకం == (స్థిరరాశి Mat& a, ద్విగుణ s)
{
    checkOperandsExist(a);
    Mat_సమాసం e;
    MatOp_Cmp::make_సమాసం(e, cv::CMP_EQ, a, s);
    తిరిగి e;
}

Mat_సమాసం క్రియాకారకం == (ద్విగుణ s, స్థిరరాశి Mat& a)
{
    checkOperandsExist(a);
    Mat_సమాసం e;
    MatOp_Cmp::make_సమాసం(e, cv::CMP_EQ, a, s);
    తిరిగి e;
}

Mat_సమాసం క్రియాకారకం != (స్థిరరాశి Mat& a, స్థిరరాశి Mat& b)
{
    checkOperandsExist(a, b);
    Mat_సమాసం e;
    MatOp_Cmp::make_సమాసం(e, cv::CMP_NE, a, b);
    తిరిగి e;
}

Mat_సమాసం క్రియాకారకం != (స్థిరరాశి Mat& a, ద్విగుణ s)
{
    checkOperandsExist(a);
    Mat_సమాసం e;
    MatOp_Cmp::make_సమాసం(e, cv::CMP_NE, a, s);
    తిరిగి e;
}

Mat_సమాసం క్రియాకారకం != (ద్విగుణ s, స్థిరరాశి Mat& a)
{
    checkOperandsExist(a);
    Mat_సమాసం e;
    MatOp_Cmp::make_సమాసం(e, cv::CMP_NE, a, s);
    తిరిగి e;
}

Mat_సమాసం క్రియాకారకం >= (స్థిరరాశి Mat& a, స్థిరరాశి Mat& b)
{
    checkOperandsExist(a, b);
    Mat_సమాసం e;
    MatOp_Cmp::make_సమాసం(e, cv::CMP_GE, a, b);
    తిరిగి e;
}

Mat_సమాసం క్రియాకారకం >= (స్థిరరాశి Mat& a, ద్విగుణ s)
{
    checkOperandsExist(a);
    Mat_సమాసం e;
    MatOp_Cmp::make_సమాసం(e, cv::CMP_GE, a, s);
    తిరిగి e;
}

Mat_సమాసం క్రియాకారకం >= (ద్విగుణ s, స్థిరరాశి Mat& a)
{
    checkOperandsExist(a);
    Mat_సమాసం e;
    MatOp_Cmp::make_సమాసం(e, cv::CMP_LE, a, s);
    తిరిగి e;
}

Mat_సమాసం క్రియాకారకం > (స్థిరరాశి Mat& a, స్థిరరాశి Mat& b)
{
    checkOperandsExist(a, b);
    Mat_సమాసం e;
    MatOp_Cmp::make_సమాసం(e, cv::CMP_GT, a, b);
    తిరిగి e;
}

Mat_సమాసం క్రియాకారకం > (స్థిరరాశి Mat& a, ద్విగుణ s)
{
    checkOperandsExist(a);
    Mat_సమాసం e;
    MatOp_Cmp::make_సమాసం(e, cv::CMP_GT, a, s);
    తిరిగి e;
}

Mat_సమాసం క్రియాకారకం > (ద్విగుణ s, స్థిరరాశి Mat& a)
{
    checkOperandsExist(a);
    Mat_సమాసం e;
    MatOp_Cmp::make_సమాసం(e, cv::CMP_LT, a, s);
    తిరిగి e;
}

Mat_సమాసం min(స్థిరరాశి Mat& a, స్థిరరాశి Mat& b)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a, b);
    Mat_సమాసం e;
    MatOp_Bin::make_సమాసం(e, 'm', a, b);
    తిరిగి e;
}

Mat_సమాసం min(స్థిరరాశి Mat& a, ద్విగుణ s)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    Mat_సమాసం e;
    MatOp_Bin::make_సమాసం(e, 'n', a, s);
    తిరిగి e;
}

Mat_సమాసం min(ద్విగుణ s, స్థిరరాశి Mat& a)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    Mat_సమాసం e;
    MatOp_Bin::make_సమాసం(e, 'n', a, s);
    తిరిగి e;
}

Mat_సమాసం max(స్థిరరాశి Mat& a, స్థిరరాశి Mat& b)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a, b);
    Mat_సమాసం e;
    MatOp_Bin::make_సమాసం(e, 'M', a, b);
    తిరిగి e;
}

Mat_సమాసం max(స్థిరరాశి Mat& a, ద్విగుణ s)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    Mat_సమాసం e;
    MatOp_Bin::make_సమాసం(e, 'N', a, s);
    తిరిగి e;
}

Mat_సమాసం max(ద్విగుణ s, స్థిరరాశి Mat& a)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    Mat_సమాసం e;
    MatOp_Bin::make_సమాసం(e, 'N', a, s);
    తిరిగి e;
}

Mat_సమాసం క్రియాకారకం & (స్థిరరాశి Mat& a, స్థిరరాశి Mat& b)
{
    checkOperandsExist(a, b);
    Mat_సమాసం e;
    MatOp_Bin::make_సమాసం(e, '&', a, b);
    తిరిగి e;
}

Mat_సమాసం క్రియాకారకం & (స్థిరరాశి Mat& a, స్థిరరాశి Scalar& s)
{
    checkOperandsExist(a);
    Mat_సమాసం e;
    MatOp_Bin::make_సమాసం(e, '&', a, s);
    తిరిగి e;
}

Mat_సమాసం క్రియాకారకం & (స్థిరరాశి Scalar& s, స్థిరరాశి Mat& a)
{
    checkOperandsExist(a);
    Mat_సమాసం e;
    MatOp_Bin::make_సమాసం(e, '&', a, s);
    తిరిగి e;
}

Mat_సమాసం క్రియాకారకం | (స్థిరరాశి Mat& a, స్థిరరాశి Mat& b)
{
    checkOperandsExist(a, b);
    Mat_సమాసం e;
    MatOp_Bin::make_సమాసం(e, '|', a, b);
    తిరిగి e;
}

Mat_సమాసం క్రియాకారకం | (స్థిరరాశి Mat& a, స్థిరరాశి Scalar& s)
{
    checkOperandsExist(a);
    Mat_సమాసం e;
    MatOp_Bin::make_సమాసం(e, '|', a, s);
    తిరిగి e;
}

Mat_సమాసం క్రియాకారకం | (స్థిరరాశి Scalar& s, స్థిరరాశి Mat& a)
{
    checkOperandsExist(a);
    Mat_సమాసం e;
    MatOp_Bin::make_సమాసం(e, '|', a, s);
    తిరిగి e;
}

Mat_సమాసం క్రియాకారకం ^ (స్థిరరాశి Mat& a, స్థిరరాశి Mat& b)
{
    checkOperandsExist(a, b);
    Mat_సమాసం e;
    MatOp_Bin::make_సమాసం(e, '^', a, b);
    తిరిగి e;
}

Mat_సమాసం క్రియాకారకం ^ (స్థిరరాశి Mat& a, స్థిరరాశి Scalar& s)
{
    checkOperandsExist(a);
    Mat_సమాసం e;
    MatOp_Bin::make_సమాసం(e, '^', a, s);
    తిరిగి e;
}

Mat_సమాసం క్రియాకారకం ^ (స్థిరరాశి Scalar& s, స్థిరరాశి Mat& a)
{
    checkOperandsExist(a);
    Mat_సమాసం e;
    MatOp_Bin::make_సమాసం(e, '^', a, s);
    తిరిగి e;
}

Mat_సమాసం క్రియాకారకం ~(స్థిరరాశి Mat& a)
{
    checkOperandsExist(a);
    Mat_సమాసం e;
    MatOp_Bin::make_సమాసం(e, '~', a, Scalar());
    తిరిగి e;
}

Mat_సమాసం నిరపేక్ష(స్థిరరాశి Mat& a)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    Mat_సమాసం e;
    MatOp_Bin::make_సమాసం(e, 'a', a, Scalar());
    తిరిగి e;
}

Mat_సమాసం నిరపేక్ష(స్థిరరాశి Mat_సమాసం& e)
{
    CV_INSTRUMENT_REGION();

    Mat_సమాసం en;
    e.op->నిరపేక్ష(e, en);
    తిరిగి en;
}


కొలత Mat_సమాసం::size() స్థిరరాశి
{
    ఒకవేళ( isT(*ఇది) || isInv(*ఇది) )
        తిరిగి కొలత(a.అడ్డువరుసలు, a.నిలువుల_సంఖ్య);
    ఒకవేళ( isSolve(*ఇది) )
        తిరిగి కొలత(b.నిలువుల_సంఖ్య, a.నిలువుల_సంఖ్య);
    ఒకవేళ( isInitializer(*ఇది) )
        తిరిగి a.size();
    తిరిగి op ? op->size(*ఇది) : కొలత();
}


పూర్ణాంకం Mat_సమాసం::type() స్థిరరాశి
{
    CV_INSTRUMENT_REGION();

    ఒకవేళ( isInitializer(*ఇది) )
        తిరిగి a.type();
    ఒకవేళ( isCmp(*ఇది) )
        తిరిగి CV_MAKETYPE(CV_8U, a.channels());
    తిరిగి op ? op->type(*ఇది) : -1;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////

శూన్య Mat_Op_తటస్థం::assign(స్థిరరాశి Mat_సమాసం& e, Mat& m, పూర్ణాంకం _type) స్థిరరాశి
{
    ఒకవేళ( _type == -1 || _type == e.a.type() )
        m = e.a;
    లేకపోతే
    {
        CV_Assert( CV_MAT_CN(_type) == e.a.channels() );
        e.a.convert_గమ్యం(m, _type);
    }
}

అంతఃవరుస శూన్య Mat_Op_తటస్థం::make_సమాసం(Mat_సమాసం& res, స్థిరరాశి Mat& m)
{
    res = Mat_సమాసం(&g_Mat_Op_తటస్థం, 0, m, Mat(), Mat(), 1, 0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

శూన్య Mat_Op_కూడు_Ex::assign(స్థిరరాశి Mat_సమాసం& e, Mat& m, పూర్ణాంకం _type) స్థిరరాశి
{
    Mat తాత్కాలిక_2, &dst = _type == -1 || e.a.type() == _type ? m : తాత్కాలిక_2;
    ఒకవేళ( e.b.దత్తాంశం )
    {
        ఒకవేళ( e.s == Scalar() || !e.s.isReal() )
        {
            ఒకవేళ( e.alpha == 1 )
            {
                ఒకవేళ( e.beta == 1 )
                    cv::కూడు(e.a, e.b, dst);
                లేకపోతే ఒకవేళ( e.beta == -1 )
                    cv::వ్యవకలనం(e.a, e.b, dst);
                లేకపోతే
                    cv::scale_కూడు(e.b, e.beta, e.a, dst);
            }
            లేకపోతే ఒకవేళ( e.beta == 1 )
            {
                ఒకవేళ( e.alpha == -1 )
                    cv::వ్యవకలనం(e.b, e.a, dst);
                లేకపోతే
                    cv::scale_కూడు(e.a, e.alpha, e.b, dst);
            }
            లేకపోతే
                cv::కూడు_Weighted(e.a, e.alpha, e.b, e.beta, 0, dst);

            ఒకవేళ( !e.s.isReal() )
                cv::కూడు(dst, e.s, dst);
        }
        లేకపోతే
        {
            ఒకవేళ (e.a.channels() > 1)
                CV_LOG_ONCE_WARNING(ఖాళీ, "OpenCV/MatExpr: processing of multi-channel arrays might be changed in the future: "
                                          "https://github.com/opencv/opencv/issues/16739");
            cv::కూడు_Weighted(e.a, e.alpha, e.b, e.beta, e.s[0], dst);
        }
    }
    లేకపోతే ఒకవేళ( e.s.isReal() && (dst.దత్తాంశం != m.దత్తాంశం || fabs(e.alpha) != 1))
    {
        ఒకవేళ (e.a.channels() > 1 && e.s[0] != 0.0)
            CV_LOG_ONCE_WARNING(ఖాళీ, "OpenCV/MatExpr: processing of multi-channel arrays might be changed in the future: "
                                      "https://github.com/opencv/opencv/issues/16739");
        e.a.convert_గమ్యం(m, _type, e.alpha, e.s[0]);
        తిరిగి;
    }
    లేకపోతే ఒకవేళ( e.alpha == 1 )
        cv::కూడు(e.a, e.s, dst);
    లేకపోతే ఒకవేళ( e.alpha == -1 )
        cv::వ్యవకలనం(e.s, e.a, dst);
    లేకపోతే
    {
        e.a.convert_గమ్యం(dst, e.a.type(), e.alpha);
        cv::కూడు(dst, e.s, dst);
    }

    ఒకవేళ( dst.దత్తాంశం != m.దత్తాంశం )
        dst.convert_గమ్యం(m, m.type());
}


శూన్య Mat_Op_కూడు_Ex::కూడు(స్థిరరాశి Mat_సమాసం& e, స్థిరరాశి Scalar& s, Mat_సమాసం& res) స్థిరరాశి
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.s += s;
}


శూన్య Mat_Op_కూడు_Ex::వ్యవకలనం(స్థిరరాశి Scalar& s, స్థిరరాశి Mat_సమాసం& e, Mat_సమాసం& res) స్థిరరాశి
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha = -res.alpha;
    res.beta = -res.beta;
    res.s = s - res.s;
}

శూన్య Mat_Op_కూడు_Ex::multiply(స్థిరరాశి Mat_సమాసం& e, ద్విగుణ s, Mat_సమాసం& res) స్థిరరాశి
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha *= s;
    res.beta *= s;
    res.s *= s;
}

శూన్య Mat_Op_కూడు_Ex::divide(ద్విగుణ s, స్థిరరాశి Mat_సమాసం& e, Mat_సమాసం& res) స్థిరరాశి
{
    CV_INSTRUMENT_REGION();

    ఒకవేళ( isScaled(e) )
        MatOp_Bin::make_సమాసం(res, '/', e.a, Mat(), s/e.alpha);
    లేకపోతే
        MatOp::divide(s, e, res);
}


శూన్య Mat_Op_కూడు_Ex::transpose(స్థిరరాశి Mat_సమాసం& e, Mat_సమాసం& res) స్థిరరాశి
{
    CV_INSTRUMENT_REGION();

    ఒకవేళ( isScaled(e) )
        MatOp_T::make_సమాసం(res, e.a, e.alpha);
    లేకపోతే
        MatOp::transpose(e, res);
}

శూన్య Mat_Op_కూడు_Ex::నిరపేక్ష(స్థిరరాశి Mat_సమాసం& e, Mat_సమాసం& res) స్థిరరాశి
{
    CV_INSTRUMENT_REGION();

    ఒకవేళ( (!e.b.దత్తాంశం || e.beta == 0) && fabs(e.alpha) == 1 )
        MatOp_Bin::make_సమాసం(res, 'a', e.a, -e.s*e.alpha);
    లేకపోతే ఒకవేళ( e.b.దత్తాంశం && e.alpha + e.beta == 0 && e.alpha*e.beta == -1 )
        MatOp_Bin::make_సమాసం(res, 'a', e.a, e.b);
    లేకపోతే
        MatOp::నిరపేక్ష(e, res);
}

అంతఃవరుస శూన్య Mat_Op_కూడు_Ex::make_సమాసం(Mat_సమాసం& res, స్థిరరాశి Mat& a, స్థిరరాశి Mat& b, ద్విగుణ alpha, ద్విగుణ beta, స్థిరరాశి Scalar& s)
{
    res = Mat_సమాసం(&g_Mat_Op_కూడు_Ex, 0, a, b, Mat(), alpha, beta, s);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

శూన్య MatOp_Bin::assign(స్థిరరాశి Mat_సమాసం& e, Mat& m, పూర్ణాంకం _type) స్థిరరాశి
{
    Mat తాత్కాలిక_2, &dst = _type == -1 || e.a.type() == _type ? m : తాత్కాలిక_2;

    ఒకవేళ( e.flags == '*' )
        cv::multiply(e.a, e.b, dst, e.alpha);
    లేకపోతే ఒకవేళ( e.flags == '/' && e.b.దత్తాంశం )
        cv::divide(e.a, e.b, dst, e.alpha);
    లేకపోతే ఒకవేళ( e.flags == '/' && !e.b.దత్తాంశం )
        cv::divide(e.alpha, e.a, dst );
    లేకపోతే ఒకవేళ( e.flags == '&' && e.b.దత్తాంశం )
        bitwise_and(e.a, e.b, dst);
    లేకపోతే ఒకవేళ( e.flags == '&' && !e.b.దత్తాంశం )
        bitwise_and(e.a, e.s, dst);
    లేకపోతే ఒకవేళ( e.flags == '|' && e.b.దత్తాంశం )
        bitwise_or(e.a, e.b, dst);
    లేకపోతే ఒకవేళ( e.flags == '|' && !e.b.దత్తాంశం )
        bitwise_or(e.a, e.s, dst);
    లేకపోతే ఒకవేళ( e.flags == '^' && e.b.దత్తాంశం )
        bitwise_xor(e.a, e.b, dst);
    లేకపోతే ఒకవేళ( e.flags == '^' && !e.b.దత్తాంశం )
        bitwise_xor(e.a, e.s, dst);
    లేకపోతే ఒకవేళ( e.flags == '~' && !e.b.దత్తాంశం )
        bitwise_not(e.a, dst);
    లేకపోతే ఒకవేళ( e.flags == 'm' )
        cv::min(e.a, e.b, dst);
    లేకపోతే ఒకవేళ( e.flags == 'n' )
        cv::min(e.a, e.s[0], dst);
    లేకపోతే ఒకవేళ( e.flags == 'M' )
        cv::max(e.a, e.b, dst);
    లేకపోతే ఒకవేళ( e.flags == 'N' )
        cv::max(e.a, e.s[0], dst);
    లేకపోతే ఒకవేళ( e.flags == 'a' && e.b.దత్తాంశం )
        cv::absdiff(e.a, e.b, dst);
    లేకపోతే ఒకవేళ( e.flags == 'a' && !e.b.దత్తాంశం )
        cv::absdiff(e.a, e.s, dst);
    లేకపోతే
        CV_Error(cv::Error::StsError, "Unknown operation");

    ఒకవేళ( dst.దత్తాంశం != m.దత్తాంశం )
        dst.convert_గమ్యం(m, _type);
}

శూన్య MatOp_Bin::multiply(స్థిరరాశి Mat_సమాసం& e, ద్విగుణ s, Mat_సమాసం& res) స్థిరరాశి
{
    CV_INSTRUMENT_REGION();

    ఒకవేళ( e.flags == '*' || e.flags == '/' )
    {
        res = e;
        res.alpha *= s;
    }
    లేకపోతే
        MatOp::multiply(e, s, res);
}

శూన్య MatOp_Bin::divide(ద్విగుణ s, స్థిరరాశి Mat_సమాసం& e, Mat_సమాసం& res) స్థిరరాశి
{
    CV_INSTRUMENT_REGION();

    ఒకవేళ( e.flags == '/' && (!e.b.దత్తాంశం || e.beta == 0) )
        Mat_Op_కూడు_Ex::make_సమాసం(res, e.a, Mat(), s/e.alpha, 0);
    లేకపోతే
        MatOp::divide(s, e, res);
}

అంతఃవరుస శూన్య MatOp_Bin::make_సమాసం(Mat_సమాసం& res, అక్షరం op, స్థిరరాశి Mat& a, స్థిరరాశి Mat& b, ద్విగుణ scale)
{
    res = Mat_సమాసం(&g_MatOp_Bin, op, a, b, Mat(), scale, b.దత్తాంశం ? 1 : 0);
}

అంతఃవరుస శూన్య MatOp_Bin::make_సమాసం(Mat_సమాసం& res, అక్షరం op, స్థిరరాశి Mat& a, స్థిరరాశి Scalar& s)
{
    res = Mat_సమాసం(&g_MatOp_Bin, op, a, Mat(), Mat(), 1, 0, s);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

శూన్య MatOp_Cmp::assign(స్థిరరాశి Mat_సమాసం& e, Mat& m, పూర్ణాంకం _type) స్థిరరాశి
{
    Mat తాత్కాలిక_2, &dst = _type == -1 || _type == CV_8U ? m : తాత్కాలిక_2;

    ఒకవేళ( e.b.దత్తాంశం )
        cv::compare(e.a, e.b, dst, e.flags);
    లేకపోతే
        cv::compare(e.a, e.alpha, dst, e.flags);

    ఒకవేళ( dst.దత్తాంశం != m.దత్తాంశం )
        dst.convert_గమ్యం(m, _type);
}

అంతఃవరుస శూన్య MatOp_Cmp::make_సమాసం(Mat_సమాసం& res, పూర్ణాంకం cmpop, స్థిరరాశి Mat& a, స్థిరరాశి Mat& b)
{
    res = Mat_సమాసం(&g_MatOp_Cmp, cmpop, a, b, Mat(), 1, 1);
}

అంతఃవరుస శూన్య MatOp_Cmp::make_సమాసం(Mat_సమాసం& res, పూర్ణాంకం cmpop, స్థిరరాశి Mat& a, ద్విగుణ alpha)
{
    res = Mat_సమాసం(&g_MatOp_Cmp, cmpop, a, Mat(), Mat(), alpha, 1);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

శూన్య MatOp_T::assign(స్థిరరాశి Mat_సమాసం& e, Mat& m, పూర్ణాంకం _type) స్థిరరాశి
{
    Mat తాత్కాలిక_2, &dst = _type == -1 || _type == e.a.type() ? m : తాత్కాలిక_2;

    cv::transpose(e.a, dst);

    ఒకవేళ( dst.దత్తాంశం != m.దత్తాంశం || e.alpha != 1 )
        dst.convert_గమ్యం(m, _type, e.alpha);
}

శూన్య MatOp_T::multiply(స్థిరరాశి Mat_సమాసం& e, ద్విగుణ s, Mat_సమాసం& res) స్థిరరాశి
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha *= s;
}

శూన్య MatOp_T::transpose(స్థిరరాశి Mat_సమాసం& e, Mat_సమాసం& res) స్థిరరాశి
{
    CV_INSTRUMENT_REGION();

    ఒకవేళ( e.alpha == 1 )
        Mat_Op_తటస్థం::make_సమాసం(res, e.a);
    లేకపోతే
        Mat_Op_కూడు_Ex::make_సమాసం(res, e.a, Mat(), e.alpha, 0);
}

అంతఃవరుస శూన్య MatOp_T::make_సమాసం(Mat_సమాసం& res, స్థిరరాశి Mat& a, ద్విగుణ alpha)
{
    res = Mat_సమాసం(&g_MatOp_T, 0, a, Mat(), Mat(), alpha, 0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

శూన్య MatOp_GEMM::assign(స్థిరరాశి Mat_సమాసం& e, Mat& m, పూర్ణాంకం _type) స్థిరరాశి
{
    Mat తాత్కాలిక_2, &dst = _type == -1 || _type == e.a.type() ? m : తాత్కాలిక_2;

    cv::gemm(e.a, e.b, e.alpha, e.c, e.beta, dst, e.flags);
    ఒకవేళ( dst.దత్తాంశం != m.దత్తాంశం )
        dst.convert_గమ్యం(m, _type);
}

శూన్య MatOp_GEMM::కూడు(స్థిరరాశి Mat_సమాసం& e1, స్థిరరాశి Mat_సమాసం& e2, Mat_సమాసం& res) స్థిరరాశి
{
    CV_INSTRUMENT_REGION();

    తార్కిక i1 = is_తటస్థం(e1), i2 = is_తటస్థం(e2);
    ద్విగుణ alpha1 = i1 ? 1 : e1.alpha, alpha2 = i2 ? 1 : e2.alpha;

    ఒకవేళ( isMatProd(e1) && (i2 || isScaled(e2) || isT(e2)) )
        MatOp_GEMM::make_సమాసం(res, (e1.flags & ~cv::GEMM_3_T)|(isT(e2) ? cv::GEMM_3_T : 0),
                             e1.a, e1.b, alpha1, e2.a, alpha2);
    లేకపోతే ఒకవేళ( isMatProd(e2) && (i1 || isScaled(e1) || isT(e1)) )
        MatOp_GEMM::make_సమాసం(res, (e2.flags & ~cv::GEMM_3_T)|(isT(e1) ? cv::GEMM_3_T : 0),
                             e2.a, e2.b, alpha2, e1.a, alpha1);
    లేకపోతే ఒకవేళ( ఇది == e2.op )
        MatOp::కూడు(e1, e2, res);
    లేకపోతే
        e2.op->కూడు(e1, e2, res);
}

శూన్య MatOp_GEMM::వ్యవకలనం(స్థిరరాశి Mat_సమాసం& e1, స్థిరరాశి Mat_సమాసం& e2, Mat_సమాసం& res) స్థిరరాశి
{
    CV_INSTRUMENT_REGION();

    తార్కిక i1 = is_తటస్థం(e1), i2 = is_తటస్థం(e2);
    ద్విగుణ alpha1 = i1 ? 1 : e1.alpha, alpha2 = i2 ? 1 : e2.alpha;

    ఒకవేళ( isMatProd(e1) && (i2 || isScaled(e2) || isT(e2)) )
        MatOp_GEMM::make_సమాసం(res, (e1.flags & ~cv::GEMM_3_T)|(isT(e2) ? cv::GEMM_3_T : 0),
                             e1.a, e1.b, alpha1, e2.a, -alpha2);
    లేకపోతే ఒకవేళ( isMatProd(e2) && (i1 || isScaled(e1) || isT(e1)) )
        MatOp_GEMM::make_సమాసం(res, (e2.flags & ~cv::GEMM_3_T)|(isT(e1) ? cv::GEMM_3_T : 0),
                            e2.a, e2.b, -alpha2, e1.a, alpha1);
    లేకపోతే ఒకవేళ( ఇది == e2.op )
        MatOp::వ్యవకలనం(e1, e2, res);
    లేకపోతే
        e2.op->వ్యవకలనం(e1, e2, res);
}

శూన్య MatOp_GEMM::multiply(స్థిరరాశి Mat_సమాసం& e, ద్విగుణ s, Mat_సమాసం& res) స్థిరరాశి
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha *= s;
    res.beta *= s;
}

శూన్య MatOp_GEMM::transpose(స్థిరరాశి Mat_సమాసం& e, Mat_సమాసం& res) స్థిరరాశి
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.flags = (!(e.flags & cv::GEMM_1_T) ? cv::GEMM_2_T : 0) |
                (!(e.flags & cv::GEMM_2_T) ? cv::GEMM_1_T : 0) |
                (!(e.flags & cv::GEMM_3_T) ? cv::GEMM_3_T : 0);
    swap(res.a, res.b);
}

అంతఃవరుస శూన్య MatOp_GEMM::make_సమాసం(Mat_సమాసం& res, పూర్ణాంకం flags, స్థిరరాశి Mat& a, స్థిరరాశి Mat& b,
                                 ద్విగుణ alpha, స్థిరరాశి Mat& c, ద్విగుణ beta)
{
    res = Mat_సమాసం(&g_MatOp_GEMM, flags, a, b, c, alpha, beta);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

శూన్య MatOp_Invert::assign(స్థిరరాశి Mat_సమాసం& e, Mat& m, పూర్ణాంకం _type) స్థిరరాశి
{
    Mat తాత్కాలిక_2, &dst = _type == -1 || _type == e.a.type() ? m : తాత్కాలిక_2;

    cv::invert(e.a, dst, e.flags);
    ఒకవేళ( dst.దత్తాంశం != m.దత్తాంశం )
        dst.convert_గమ్యం(m, _type);
}

శూన్య MatOp_Invert::matmul(స్థిరరాశి Mat_సమాసం& e1, స్థిరరాశి Mat_సమాసం& e2, Mat_సమాసం& res) స్థిరరాశి
{
    ఒకవేళ( isInv(e1) && is_తటస్థం(e2) )
        MatOp_Solve::make_సమాసం(res, e1.flags, e1.a, e2.a);
    లేకపోతే ఒకవేళ( ఇది == e2.op )
        MatOp::matmul(e1, e2, res);
    లేకపోతే
        e2.op->matmul(e1, e2, res);
}

అంతఃవరుస శూన్య MatOp_Invert::make_సమాసం(Mat_సమాసం& res, పూర్ణాంకం పద్ధతి, స్థిరరాశి Mat& m)
{
    res = Mat_సమాసం(&g_MatOp_Invert, పద్ధతి, m, Mat(), Mat(), 1, 0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

శూన్య MatOp_Solve::assign(స్థిరరాశి Mat_సమాసం& e, Mat& m, పూర్ణాంకం _type) స్థిరరాశి
{
    Mat తాత్కాలిక_2, &dst = _type == -1 || _type == e.a.type() ? m : తాత్కాలిక_2;

    cv::solve(e.a, e.b, dst, e.flags);
    ఒకవేళ( dst.దత్తాంశం != m.దత్తాంశం )
        dst.convert_గమ్యం(m, _type);
}

అంతఃవరుస శూన్య MatOp_Solve::make_సమాసం(Mat_సమాసం& res, పూర్ణాంకం పద్ధతి, స్థిరరాశి Mat& a, స్థిరరాశి Mat& b)
{
    res = Mat_సమాసం(&g_MatOp_Solve, పద్ధతి, a, b, Mat(), 1, 1);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

శూన్య MatOp_Initializer::assign(స్థిరరాశి Mat_సమాసం& e, Mat& m, పూర్ణాంకం _type) స్థిరరాశి
{
    ఒకవేళ( _type == -1 )
        _type = e.a.type();

    m.create(e.a.size, _type);

    ఒకవేళ( e.flags == 'I' && e.a.dims <= 2 )
        సమితి_తటస్థం(m, Scalar(e.alpha));
    లేకపోతే ఒకవేళ( e.flags == '0' )
        m = Scalar();
    లేకపోతే ఒకవేళ( e.flags == '1' )
        m = Scalar(e.alpha);
    లేకపోతే
        CV_Error(cv::Error::StsError, "Invalid matrix initializer type");
}

శూన్య MatOp_Initializer::multiply(స్థిరరాశి Mat_సమాసం& e, ద్విగుణ s, Mat_సమాసం& res) స్థిరరాశి
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha *= s;
}

అంతఃవరుస శూన్య MatOp_Initializer::make_సమాసం(Mat_సమాసం& res, పూర్ణాంకం పద్ధతి, కొలత sz, పూర్ణాంకం type, ద్విగుణ alpha)
{
    res = Mat_సమాసం(getGlobalMatOpInitializer(), పద్ధతి, Mat(sz, type, (శూన్య*)(size_t)0xEEEEEEEE), Mat(), Mat(), alpha, 0);
}

అంతఃవరుస శూన్య MatOp_Initializer::make_సమాసం(Mat_సమాసం& res, పూర్ణాంకం పద్ధతి, పూర్ణాంకం ndims, స్థిరరాశి పూర్ణాంకం* sizes, పూర్ణాంకం type, ద్విగుణ alpha)
{
    res = Mat_సమాసం(getGlobalMatOpInitializer(), పద్ధతి, Mat(ndims, sizes, type, (శూన్య*)(size_t)0xEEEEEEEE), Mat(), Mat(), alpha, 0);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

Mat_సమాసం Mat::t() స్థిరరాశి
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(*ఇది);
    Mat_సమాసం e;
    MatOp_T::make_సమాసం(e, *ఇది);
    తిరిగి e;
}

Mat_సమాసం Mat::inv(పూర్ణాంకం పద్ధతి) స్థిరరాశి
{
    CV_INSTRUMENT_REGION();

    Mat_సమాసం e;
    MatOp_Invert::make_సమాసం(e, పద్ధతి, *ఇది);
    తిరిగి e;
}


Mat_సమాసం Mat::mul(ఆదానం_Array m, ద్విగుణ scale) స్థిరరాశి
{
    CV_INSTRUMENT_REGION();

    Mat b = m.getMat();
    // Unless the argument is a refcounted Mat/UMat, the header returned by getMat() may be
    // a non-owning view of caller memory (e.g. a scalar bound to _InputArray(const double&),
    // a Matx or a Vec) that the returned MatExpr can outlive, so snapshot it.
    // See https://github.com/opencv/opencv/issues/23577
    ఒకవేళ( !m.isMat() && !m.isUMat() )
        b = b.clone();

    Mat_సమాసం e;
    MatOp_Bin::make_సమాసం(e, '*', *ఇది, b, scale);
    తిరిగి e;
}

Mat_సమాసం Mat::zeros(పూర్ణాంకం అడ్డువరుసలు, పూర్ణాంకం నిలువుల_సంఖ్య, పూర్ణాంకం type)
{
    CV_INSTRUMENT_REGION();

    Mat_సమాసం e;
    MatOp_Initializer::make_సమాసం(e, '0', కొలత(నిలువుల_సంఖ్య, అడ్డువరుసలు), type);
    తిరిగి e;
}

Mat_సమాసం Mat::zeros(కొలత size, పూర్ణాంకం type)
{
    CV_INSTRUMENT_REGION();

    Mat_సమాసం e;
    MatOp_Initializer::make_సమాసం(e, '0', size, type);
    తిరిగి e;
}

Mat_సమాసం Mat::zeros(పూర్ణాంకం ndims, స్థిరరాశి పూర్ణాంకం* sizes, పూర్ణాంకం type)
{
    CV_INSTRUMENT_REGION();

    Mat_సమాసం e;
    MatOp_Initializer::make_సమాసం(e, '0', ndims, sizes, type);
    తిరిగి e;
}

Mat_సమాసం Mat::zeros(స్థిరరాశి MatShape& shape, పూర్ణాంకం type)
{
    CV_INSTRUMENT_REGION();

    Mat_సమాసం e;
    MatOp_Initializer::make_సమాసం(e, '0', shape.dims, shape.p, type);
    తిరిగి e;
}

Mat_సమాసం Mat::ones(పూర్ణాంకం అడ్డువరుసలు, పూర్ణాంకం నిలువుల_సంఖ్య, పూర్ణాంకం type)
{
    CV_INSTRUMENT_REGION();

    Mat_సమాసం e;
    MatOp_Initializer::make_సమాసం(e, '1', కొలత(నిలువుల_సంఖ్య, అడ్డువరుసలు), type);
    తిరిగి e;
}

Mat_సమాసం Mat::ones(కొలత size, పూర్ణాంకం type)
{
    CV_INSTRUMENT_REGION();

    Mat_సమాసం e;
    MatOp_Initializer::make_సమాసం(e, '1', size, type);
    తిరిగి e;
}

Mat_సమాసం Mat::ones(పూర్ణాంకం ndims, స్థిరరాశి పూర్ణాంకం* sizes, పూర్ణాంకం type)
{
    CV_INSTRUMENT_REGION();

    Mat_సమాసం e;
    MatOp_Initializer::make_సమాసం(e, '1', ndims, sizes, type);
    తిరిగి e;
}

Mat_సమాసం Mat::ones(స్థిరరాశి MatShape& shape, పూర్ణాంకం type)
{
    CV_INSTRUMENT_REGION();

    Mat_సమాసం e;
    MatOp_Initializer::make_సమాసం(e, '1', shape.dims, shape.p, type);
    తిరిగి e;
}

Mat_సమాసం Mat::eye(పూర్ణాంకం అడ్డువరుసలు, పూర్ణాంకం నిలువుల_సంఖ్య, పూర్ణాంకం type)
{
    CV_INSTRUMENT_REGION();

    Mat_సమాసం e;
    MatOp_Initializer::make_సమాసం(e, 'I', కొలత(నిలువుల_సంఖ్య, అడ్డువరుసలు), type);
    తిరిగి e;
}

Mat_సమాసం Mat::eye(కొలత size, పూర్ణాంకం type)
{
    CV_INSTRUMENT_REGION();

    Mat_సమాసం e;
    MatOp_Initializer::make_సమాసం(e, 'I', size, type);
    తిరిగి e;
}

శూన్య Mat_సమాసం::swap(Mat_సమాసం& other)
{
    ఉపయోగించి std::swap;

    swap(op, other.op);
    swap(flags, other.flags);

    swap(a, other.a);
    swap(b, other.b);
    swap(c, other.c);

    swap(alpha, other.alpha);
    swap(beta, other.beta);

    swap(s, other.s);
}

ఆదానం_Array_2::ఆదానం_Array_2(స్థిరరాశి Mat_సమాసం& సమాసం)
{
    ఒకవేళ (!is_తటస్థం(సమాసం))
    {
        Mat ఫలితం = సమాసం;  // TODO improve through refcount == 1 of expr.a (inplace operation is possible - except gemm?)
        Mat_సమాసం ఫలితం_సమాసం(ఫలితం);
        swap(const_cast<Mat_సమాసం&>(సమాసం), ఫలితం_సమాసం);
    }
    CV_Assert(is_తటస్థం(సమాసం));
    init(FIXED_TYPE + FIXED_కొలత + MAT + ACCESS_READ, &సమాసం.a);
}

} // cv::
