// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


/* ////////////////////////////////////////////////////////////////////
//
//  Mat basic operations: Copy, Set
//
// */

#അന്തര്ഭാവ "precomp.hpp"
#അന്തര്ഭാവ <opencv2/core/utils/logger.hpp>

നാമാകാശ cv
{

//This and its overload below are used in various MatExpr operator overloads
//implemented to check that Matrix operands exist.
സ്ഥിതം ശൂന്യം checkOperandsExist(സ്ഥിരം Mat& a)
{
    എങ്കിൽ (a.empty())
    {
        CV_Error(cv::Error::StsBadArg, "Matrix operand is an empty matrix.");
    }
}

സ്ഥിതം ശൂന്യം checkOperandsExist(സ്ഥിരം Mat& a, സ്ഥിരം Mat& b)
{
    എങ്കിൽ (a.empty() || b.empty())
    {
        CV_Error(cv::Error::StsBadArg, "One or more matrix operands are empty.");
    }
}


വർഗ്ഗം MatOp_Identity CV_FINAL : പൊതു MatOp
{
പൊതു:
    MatOp_Identity() {}
    മായാ ~MatOp_Identity() {}

    തർക്കം elementWise(സ്ഥിരം Mat_വ്യഞ്ജകം& /*വ്യഞ്ജകം*/) സ്ഥിരം CV_OVERRIDE { തിരികെ സത്യം; }
    ശൂന്യം assign(സ്ഥിരം Mat_വ്യഞ്ജകം& വ്യഞ്ജകം, Mat& m, പൂർണ്ണസംഖ്യ type=-1) സ്ഥിരം CV_OVERRIDE;

    സ്ഥിതം ശൂന്യം make_വ്യഞ്ജകം(Mat_വ്യഞ്ജകം& res, സ്ഥിരം Mat& m);
};

സ്ഥിതം MatOp_Identity g_MatOp_Identity;

വർഗ്ഗം MatOp_AddEx CV_FINAL : പൊതു MatOp
{
പൊതു:
    MatOp_AddEx() {}
    മായാ ~MatOp_AddEx() {}

    തർക്കം elementWise(സ്ഥിരം Mat_വ്യഞ്ജകം& /*വ്യഞ്ജകം*/) സ്ഥിരം CV_OVERRIDE { തിരികെ സത്യം; }
    ശൂന്യം assign(സ്ഥിരം Mat_വ്യഞ്ജകം& വ്യഞ്ജകം, Mat& m, പൂർണ്ണസംഖ്യ type=-1) സ്ഥിരം CV_OVERRIDE;

    ശൂന്യം add(സ്ഥിരം Mat_വ്യഞ്ജകം& e1, സ്ഥിരം Scalar& s, Mat_വ്യഞ്ജകം& res) സ്ഥിരം CV_OVERRIDE;
    ശൂന്യം subtract(സ്ഥിരം Scalar& s, സ്ഥിരം Mat_വ്യഞ്ജകം& വ്യഞ്ജകം, Mat_വ്യഞ്ജകം& res) സ്ഥിരം CV_OVERRIDE;
    ശൂന്യം multiply(സ്ഥിരം Mat_വ്യഞ്ജകം& e1, ഇരട്ട s, Mat_വ്യഞ്ജകം& res) സ്ഥിരം CV_OVERRIDE;
    ശൂന്യം divide(ഇരട്ട s, സ്ഥിരം Mat_വ്യഞ്ജകം& e, Mat_വ്യഞ്ജകം& res) സ്ഥിരം CV_OVERRIDE;

    ശൂന്യം transpose(സ്ഥിരം Mat_വ്യഞ്ജകം& e1, Mat_വ്യഞ്ജകം& res) സ്ഥിരം CV_OVERRIDE;
    ശൂന്യം abs(സ്ഥിരം Mat_വ്യഞ്ജകം& വ്യഞ്ജകം, Mat_വ്യഞ്ജകം& res) സ്ഥിരം CV_OVERRIDE;

    സ്ഥിതം ശൂന്യം make_വ്യഞ്ജകം(Mat_വ്യഞ്ജകം& res, സ്ഥിരം Mat& a, സ്ഥിരം Mat& b, ഇരട്ട alpha, ഇരട്ട beta, സ്ഥിരം Scalar& s=Scalar());
};

സ്ഥിതം MatOp_AddEx g_MatOp_AddEx;

വർഗ്ഗം MatOp_Bin CV_FINAL : പൊതു MatOp
{
പൊതു:
    MatOp_Bin() {}
    മായാ ~MatOp_Bin() {}

    തർക്കം elementWise(സ്ഥിരം Mat_വ്യഞ്ജകം& /*വ്യഞ്ജകം*/) സ്ഥിരം CV_OVERRIDE { തിരികെ സത്യം; }
    ശൂന്യം assign(സ്ഥിരം Mat_വ്യഞ്ജകം& വ്യഞ്ജകം, Mat& m, പൂർണ്ണസംഖ്യ type=-1) സ്ഥിരം CV_OVERRIDE;

    ശൂന്യം multiply(സ്ഥിരം Mat_വ്യഞ്ജകം& e1, ഇരട്ട s, Mat_വ്യഞ്ജകം& res) സ്ഥിരം CV_OVERRIDE;
    ശൂന്യം divide(ഇരട്ട s, സ്ഥിരം Mat_വ്യഞ്ജകം& e, Mat_വ്യഞ്ജകം& res) സ്ഥിരം CV_OVERRIDE;

    സ്ഥിതം ശൂന്യം make_വ്യഞ്ജകം(Mat_വ്യഞ്ജകം& res, അക്ഷരം op, സ്ഥിരം Mat& a, സ്ഥിരം Mat& b, ഇരട്ട scale=1);
    സ്ഥിതം ശൂന്യം make_വ്യഞ്ജകം(Mat_വ്യഞ്ജകം& res, അക്ഷരം op, സ്ഥിരം Mat& a, സ്ഥിരം Scalar& s);
};

സ്ഥിതം MatOp_Bin g_MatOp_Bin;

വർഗ്ഗം MatOp_Cmp CV_FINAL : പൊതു MatOp
{
പൊതു:
    MatOp_Cmp() {}
    മായാ ~MatOp_Cmp() {}

    തർക്കം elementWise(സ്ഥിരം Mat_വ്യഞ്ജകം& /*വ്യഞ്ജകം*/) സ്ഥിരം CV_OVERRIDE { തിരികെ സത്യം; }
    ശൂന്യം assign(സ്ഥിരം Mat_വ്യഞ്ജകം& വ്യഞ്ജകം, Mat& m, പൂർണ്ണസംഖ്യ type=-1) സ്ഥിരം CV_OVERRIDE;

    സ്ഥിതം ശൂന്യം make_വ്യഞ്ജകം(Mat_വ്യഞ്ജകം& res, പൂർണ്ണസംഖ്യ cmpop, സ്ഥിരം Mat& a, സ്ഥിരം Mat& b);
    സ്ഥിതം ശൂന്യം make_വ്യഞ്ജകം(Mat_വ്യഞ്ജകം& res, പൂർണ്ണസംഖ്യ cmpop, സ്ഥിരം Mat& a, ഇരട്ട alpha);
};

സ്ഥിതം MatOp_Cmp g_MatOp_Cmp;

വർഗ്ഗം MatOp_GEMM CV_FINAL : പൊതു MatOp
{
പൊതു:
    MatOp_GEMM() {}
    മായാ ~MatOp_GEMM() {}

    തർക്കം elementWise(സ്ഥിരം Mat_വ്യഞ്ജകം& /*വ്യഞ്ജകം*/) സ്ഥിരം CV_OVERRIDE { തിരികെ അസത്യം; }
    ശൂന്യം assign(സ്ഥിരം Mat_വ്യഞ്ജകം& വ്യഞ്ജകം, Mat& m, പൂർണ്ണസംഖ്യ type=-1) സ്ഥിരം CV_OVERRIDE;

    ശൂന്യം add(സ്ഥിരം Mat_വ്യഞ്ജകം& e1, സ്ഥിരം Mat_വ്യഞ്ജകം& e2, Mat_വ്യഞ്ജകം& res) സ്ഥിരം CV_OVERRIDE;
    ശൂന്യം subtract(സ്ഥിരം Mat_വ്യഞ്ജകം& e1, സ്ഥിരം Mat_വ്യഞ്ജകം& e2, Mat_വ്യഞ്ജകം& res) സ്ഥിരം CV_OVERRIDE;
    ശൂന്യം multiply(സ്ഥിരം Mat_വ്യഞ്ജകം& e, ഇരട്ട s, Mat_വ്യഞ്ജകം& res) സ്ഥിരം CV_OVERRIDE;

    ശൂന്യം transpose(സ്ഥിരം Mat_വ്യഞ്ജകം& വ്യഞ്ജകം, Mat_വ്യഞ്ജകം& res) സ്ഥിരം CV_OVERRIDE;

    Size size(സ്ഥിരം Mat_വ്യഞ്ജകം& വ്യഞ്ജകം) സ്ഥിരം CV_OVERRIDE
    {
        തിരികെ Size(
            (വ്യഞ്ജകം.flags & GEMM_2_T) ? വ്യഞ്ജകം.b.rows : വ്യഞ്ജകം.b.cols,
            (വ്യഞ്ജകം.flags & GEMM_1_T) ? വ്യഞ്ജകം.a.cols : വ്യഞ്ജകം.a.rows
        );
    }

    സ്ഥിതം ശൂന്യം make_വ്യഞ്ജകം(Mat_വ്യഞ്ജകം& res, പൂർണ്ണസംഖ്യ flags, സ്ഥിരം Mat& a, സ്ഥിരം Mat& b,
                         ഇരട്ട alpha=1, സ്ഥിരം Mat& c=Mat(), ഇരട്ട beta=1);

};

സ്ഥിതം MatOp_GEMM g_MatOp_GEMM;

വർഗ്ഗം MatOp_Invert CV_FINAL : പൊതു MatOp
{
പൊതു:
    MatOp_Invert() {}
    മായാ ~MatOp_Invert() {}

    തർക്കം elementWise(സ്ഥിരം Mat_വ്യഞ്ജകം& /*വ്യഞ്ജകം*/) സ്ഥിരം CV_OVERRIDE { തിരികെ അസത്യം; }
    ശൂന്യം assign(സ്ഥിരം Mat_വ്യഞ്ജകം& വ്യഞ്ജകം, Mat& m, പൂർണ്ണസംഖ്യ type=-1) സ്ഥിരം CV_OVERRIDE;

    ശൂന്യം matmul(സ്ഥിരം Mat_വ്യഞ്ജകം& expr1, സ്ഥിരം Mat_വ്യഞ്ജകം& expr2, Mat_വ്യഞ്ജകം& res) സ്ഥിരം CV_OVERRIDE;

    സ്ഥിതം ശൂന്യം make_വ്യഞ്ജകം(Mat_വ്യഞ്ജകം& res, പൂർണ്ണസംഖ്യ method, സ്ഥിരം Mat& m);
};

സ്ഥിതം MatOp_Invert g_MatOp_Invert;

വർഗ്ഗം MatOp_T CV_FINAL : പൊതു MatOp
{
പൊതു:
    MatOp_T() {}
    മായാ ~MatOp_T() {}

    തർക്കം elementWise(സ്ഥിരം Mat_വ്യഞ്ജകം& /*വ്യഞ്ജകം*/) സ്ഥിരം CV_OVERRIDE { തിരികെ അസത്യം; }
    ശൂന്യം assign(സ്ഥിരം Mat_വ്യഞ്ജകം& വ്യഞ്ജകം, Mat& m, പൂർണ്ണസംഖ്യ type=-1) സ്ഥിരം CV_OVERRIDE;

    ശൂന്യം multiply(സ്ഥിരം Mat_വ്യഞ്ജകം& e1, ഇരട്ട s, Mat_വ്യഞ്ജകം& res) സ്ഥിരം CV_OVERRIDE;
    ശൂന്യം transpose(സ്ഥിരം Mat_വ്യഞ്ജകം& വ്യഞ്ജകം, Mat_വ്യഞ്ജകം& res) സ്ഥിരം CV_OVERRIDE;

    സ്ഥിതം ശൂന്യം make_വ്യഞ്ജകം(Mat_വ്യഞ്ജകം& res, സ്ഥിരം Mat& a, ഇരട്ട alpha=1);
};

സ്ഥിതം MatOp_T g_MatOp_T;

വർഗ്ഗം MatOp_Solve CV_FINAL : പൊതു MatOp
{
പൊതു:
    MatOp_Solve() {}
    മായാ ~MatOp_Solve() {}

    തർക്കം elementWise(സ്ഥിരം Mat_വ്യഞ്ജകം& /*വ്യഞ്ജകം*/) സ്ഥിരം CV_OVERRIDE { തിരികെ അസത്യം; }
    ശൂന്യം assign(സ്ഥിരം Mat_വ്യഞ്ജകം& വ്യഞ്ജകം, Mat& m, പൂർണ്ണസംഖ്യ type=-1) സ്ഥിരം CV_OVERRIDE;

    സ്ഥിതം ശൂന്യം make_വ്യഞ്ജകം(Mat_വ്യഞ്ജകം& res, പൂർണ്ണസംഖ്യ method, സ്ഥിരം Mat& a, സ്ഥിരം Mat& b);
};

സ്ഥിതം MatOp_Solve g_MatOp_Solve;

വർഗ്ഗം MatOp_Initializer CV_FINAL : പൊതു MatOp
{
പൊതു:
    MatOp_Initializer() {}
    മായാ ~MatOp_Initializer() {}

    തർക്കം elementWise(സ്ഥിരം Mat_വ്യഞ്ജകം& /*വ്യഞ്ജകം*/) സ്ഥിരം CV_OVERRIDE { തിരികെ അസത്യം; }
    ശൂന്യം assign(സ്ഥിരം Mat_വ്യഞ്ജകം& വ്യഞ്ജകം, Mat& m, പൂർണ്ണസംഖ്യ type=-1) സ്ഥിരം CV_OVERRIDE;

    ശൂന്യം multiply(സ്ഥിരം Mat_വ്യഞ്ജകം& e, ഇരട്ട s, Mat_വ്യഞ്ജകം& res) സ്ഥിരം CV_OVERRIDE;

    സ്ഥിതം ശൂന്യം make_വ്യഞ്ജകം(Mat_വ്യഞ്ജകം& res, പൂർണ്ണസംഖ്യ method, Size sz, പൂർണ്ണസംഖ്യ type, ഇരട്ട alpha=1);
    സ്ഥിതം ശൂന്യം make_വ്യഞ്ജകം(Mat_വ്യഞ്ജകം& res, പൂർണ്ണസംഖ്യ method, പൂർണ്ണസംഖ്യ ndims, സ്ഥിരം പൂർണ്ണസംഖ്യ* sizes, പൂർണ്ണസംഖ്യ type, ഇരട്ട alpha=1);
};

സ്ഥിതം MatOp_Initializer* getGlobalMatOpInitializer()
{
    CV_SINGLETON_LAZY_INIT(MatOp_Initializer, പുതിയ MatOp_Initializer())
}

സ്ഥിതം വരിയിൽ തർക്കം isIdentity(സ്ഥിരം Mat_വ്യഞ്ജകം& e) { തിരികെ e.op == &g_MatOp_Identity; }
സ്ഥിതം വരിയിൽ തർക്കം isAddEx(സ്ഥിരം Mat_വ്യഞ്ജകം& e) { തിരികെ e.op == &g_MatOp_AddEx; }
സ്ഥിതം വരിയിൽ തർക്കം isScaled(സ്ഥിരം Mat_വ്യഞ്ജകം& e) { തിരികെ isAddEx(e) && (!e.b.data || e.beta == 0) && e.s == Scalar(); }
സ്ഥിതം വരിയിൽ തർക്കം isBin(സ്ഥിരം Mat_വ്യഞ്ജകം& e, അക്ഷരം c) { തിരികെ e.op == &g_MatOp_Bin && e.flags == c; }
സ്ഥിതം വരിയിൽ തർക്കം isCmp(സ്ഥിരം Mat_വ്യഞ്ജകം& e) { തിരികെ e.op == &g_MatOp_Cmp; }
സ്ഥിതം വരിയിൽ തർക്കം isReciprocal(സ്ഥിരം Mat_വ്യഞ്ജകം& e) { തിരികെ isBin(e,'/') && (!e.b.data || e.beta == 0); }
സ്ഥിതം വരിയിൽ തർക്കം isT(സ്ഥിരം Mat_വ്യഞ്ജകം& e) { തിരികെ e.op == &g_MatOp_T; }
സ്ഥിതം വരിയിൽ തർക്കം isInv(സ്ഥിരം Mat_വ്യഞ്ജകം& e) { തിരികെ e.op == &g_MatOp_Invert; }
സ്ഥിതം വരിയിൽ തർക്കം isSolve(സ്ഥിരം Mat_വ്യഞ്ജകം& e) { തിരികെ e.op == &g_MatOp_Solve; }
//static inline bool isGEMM(const MatExpr& e) { return e.op == &g_MatOp_GEMM; }
സ്ഥിതം വരിയിൽ തർക്കം isMatProd(സ്ഥിരം Mat_വ്യഞ്ജകം& e) { തിരികെ e.op == &g_MatOp_GEMM && (!e.c.data || e.beta == 0); }
സ്ഥിതം വരിയിൽ തർക്കം isInitializer(സ്ഥിരം Mat_വ്യഞ്ജകം& e) { തിരികെ e.op == getGlobalMatOpInitializer(); }

/////////////////////////////////////////////////////////////////////////////////////////////////////

MatOp::MatOp() {}
MatOp::~MatOp() {}


തർക്കം MatOp::elementWise(സ്ഥിരം Mat_വ്യഞ്ജകം& /*വ്യഞ്ജകം*/) സ്ഥിരം
{
    തിരികെ അസത്യം;
}

ശൂന്യം MatOp::roi(സ്ഥിരം Mat_വ്യഞ്ജകം& വ്യഞ്ജകം, സ്ഥിരം Range& rowRange, സ്ഥിരം Range& colRange, Mat_വ്യഞ്ജകം& e) സ്ഥിരം
{
    എങ്കിൽ( elementWise(വ്യഞ്ജകം) )
    {
        e = Mat_വ്യഞ്ജകം(വ്യഞ്ജകം.op, വ്യഞ്ജകം.flags, Mat(), Mat(), Mat(),
                    വ്യഞ്ജകം.alpha, വ്യഞ്ജകം.beta, വ്യഞ്ജകം.s);
        എങ്കിൽ(വ്യഞ്ജകം.a.data)
            e.a = വ്യഞ്ജകം.a(rowRange, colRange);
        എങ്കിൽ(വ്യഞ്ജകം.b.data)
            e.b = വ്യഞ്ജകം.b(rowRange, colRange);
        എങ്കിൽ(വ്യഞ്ജകം.c.data)
            e.c = വ്യഞ്ജകം.c(rowRange, colRange);
    }
    അല്ലെങ്കിൽ
    {
        Mat m;
        വ്യഞ്ജകം.op->assign(വ്യഞ്ജകം, m);
        e = Mat_വ്യഞ്ജകം(&g_MatOp_Identity, 0, m(rowRange, colRange), Mat(), Mat());
    }
}

ശൂന്യം MatOp::diag(സ്ഥിരം Mat_വ്യഞ്ജകം& വ്യഞ്ജകം, പൂർണ്ണസംഖ്യ d, Mat_വ്യഞ്ജകം& e) സ്ഥിരം
{
    എങ്കിൽ( elementWise(വ്യഞ്ജകം) )
    {
        e = Mat_വ്യഞ്ജകം(വ്യഞ്ജകം.op, വ്യഞ്ജകം.flags, Mat(), Mat(), Mat(),
                    വ്യഞ്ജകം.alpha, വ്യഞ്ജകം.beta, വ്യഞ്ജകം.s);
        എങ്കിൽ(വ്യഞ്ജകം.a.data)
            e.a = വ്യഞ്ജകം.a.diag(d);
        എങ്കിൽ(വ്യഞ്ജകം.b.data)
            e.b = വ്യഞ്ജകം.b.diag(d);
        എങ്കിൽ(വ്യഞ്ജകം.c.data)
            e.c = വ്യഞ്ജകം.c.diag(d);
    }
    അല്ലെങ്കിൽ
    {
        Mat m;
        വ്യഞ്ജകം.op->assign(വ്യഞ്ജകം, m);
        e = Mat_വ്യഞ്ജകം(&g_MatOp_Identity, 0, m.diag(d), Mat(), Mat());
    }
}


ശൂന്യം MatOp::augAssignAdd(സ്ഥിരം Mat_വ്യഞ്ജകം& വ്യഞ്ജകം, Mat& m) സ്ഥിരം
{
    Mat temp;
    വ്യഞ്ജകം.op->assign(വ്യഞ്ജകം, temp);
    m += temp;
}


ശൂന്യം MatOp::augAssignSubtract(സ്ഥിരം Mat_വ്യഞ്ജകം& വ്യഞ്ജകം, Mat& m) സ്ഥിരം
{
    Mat temp;
    വ്യഞ്ജകം.op->assign(വ്യഞ്ജകം, temp);
    m -= temp;
}


ശൂന്യം MatOp::augAssignMultiply(സ്ഥിരം Mat_വ്യഞ്ജകം& വ്യഞ്ജകം, Mat& m) സ്ഥിരം
{
    Mat temp;
    വ്യഞ്ജകം.op->assign(വ്യഞ്ജകം, temp);
    m *= temp;
}


ശൂന്യം MatOp::augAssignDivide(സ്ഥിരം Mat_വ്യഞ്ജകം& വ്യഞ്ജകം, Mat& m) സ്ഥിരം
{
    Mat temp;
    വ്യഞ്ജകം.op->assign(വ്യഞ്ജകം, temp);
    m /= temp;
}


ശൂന്യം MatOp::augAssignAnd(സ്ഥിരം Mat_വ്യഞ്ജകം& വ്യഞ്ജകം, Mat& m) സ്ഥിരം
{
    Mat temp;
    വ്യഞ്ജകം.op->assign(വ്യഞ്ജകം, temp);
    m &= temp;
}


ശൂന്യം MatOp::augAssignOr(സ്ഥിരം Mat_വ്യഞ്ജകം& വ്യഞ്ജകം, Mat& m) സ്ഥിരം
{
    Mat temp;
    വ്യഞ്ജകം.op->assign(വ്യഞ്ജകം, temp);
    m |= temp;
}


ശൂന്യം MatOp::augAssignXor(സ്ഥിരം Mat_വ്യഞ്ജകം& വ്യഞ്ജകം, Mat& m) സ്ഥിരം
{
    Mat temp;
    വ്യഞ്ജകം.op->assign(വ്യഞ്ജകം, temp);
    m ^= temp;
}


ശൂന്യം MatOp::add(സ്ഥിരം Mat_വ്യഞ്ജകം& e1, സ്ഥിരം Mat_വ്യഞ്ജകം& e2, Mat_വ്യഞ്ജകം& res) സ്ഥിരം
{
    CV_INSTRUMENT_REGION();

    എങ്കിൽ( ഇത് == e2.op )
    {
        ഇരട്ട alpha = 1, beta = 1;
        Scalar s;
        Mat m1, m2;
        എങ്കിൽ( isAddEx(e1) && (!e1.b.data || e1.beta == 0) )
        {
            m1 = e1.a;
            alpha = e1.alpha;
            s = e1.s;
        }
        അല്ലെങ്കിൽ
            e1.op->assign(e1, m1);

        എങ്കിൽ( isAddEx(e2) && (!e2.b.data || e2.beta == 0) )
        {
            m2 = e2.a;
            beta = e2.alpha;
            s += e2.s;
        }
        അല്ലെങ്കിൽ
            e2.op->assign(e2, m2);
        MatOp_AddEx::make_വ്യഞ്ജകം(res, m1, m2, alpha, beta, s);
    }
    അല്ലെങ്കിൽ
        e2.op->add(e1, e2, res);
}


ശൂന്യം MatOp::add(സ്ഥിരം Mat_വ്യഞ്ജകം& expr1, സ്ഥിരം Scalar& s, Mat_വ്യഞ്ജകം& res) സ്ഥിരം
{
    CV_INSTRUMENT_REGION();

    Mat m1;
    expr1.op->assign(expr1, m1);
    MatOp_AddEx::make_വ്യഞ്ജകം(res, m1, Mat(), 1, 0, s);
}


ശൂന്യം MatOp::subtract(സ്ഥിരം Mat_വ്യഞ്ജകം& e1, സ്ഥിരം Mat_വ്യഞ്ജകം& e2, Mat_വ്യഞ്ജകം& res) സ്ഥിരം
{
    CV_INSTRUMENT_REGION();

    എങ്കിൽ( ഇത് == e2.op )
    {
        ഇരട്ട alpha = 1, beta = -1;
        Scalar s;
        Mat m1, m2;
        എങ്കിൽ( isAddEx(e1) && (!e1.b.data || e1.beta == 0) )
        {
            m1 = e1.a;
            alpha = e1.alpha;
            s = e1.s;
        }
        അല്ലെങ്കിൽ
            e1.op->assign(e1, m1);

        എങ്കിൽ( isAddEx(e2) && (!e2.b.data || e2.beta == 0) )
        {
            m2 = e2.a;
            beta = -e2.alpha;
            s -= e2.s;
        }
        അല്ലെങ്കിൽ
            e2.op->assign(e2, m2);
        MatOp_AddEx::make_വ്യഞ്ജകം(res, m1, m2, alpha, beta, s);
    }
    അല്ലെങ്കിൽ
        e2.op->subtract(e1, e2, res);
}


ശൂന്യം MatOp::subtract(സ്ഥിരം Scalar& s, സ്ഥിരം Mat_വ്യഞ്ജകം& വ്യഞ്ജകം, Mat_വ്യഞ്ജകം& res) സ്ഥിരം
{
    CV_INSTRUMENT_REGION();

    Mat m;
    വ്യഞ്ജകം.op->assign(വ്യഞ്ജകം, m);
    MatOp_AddEx::make_വ്യഞ്ജകം(res, m, Mat(), -1, 0, s);
}


ശൂന്യം MatOp::multiply(സ്ഥിരം Mat_വ്യഞ്ജകം& e1, സ്ഥിരം Mat_വ്യഞ്ജകം& e2, Mat_വ്യഞ്ജകം& res, ഇരട്ട scale) സ്ഥിരം
{
    CV_INSTRUMENT_REGION();

    എങ്കിൽ( ഇത് == e2.op )
    {
        Mat m1, m2;

        എങ്കിൽ( isReciprocal(e1) )
        {
            എങ്കിൽ( isScaled(e2) )
            {
                scale *= e2.alpha;
                m2 = e2.a;
            }
            അല്ലെങ്കിൽ
                e2.op->assign(e2, m2);

            MatOp_Bin::make_വ്യഞ്ജകം(res, '/', m2, e1.a, scale/e1.alpha);
        }
        അല്ലെങ്കിൽ
        {
            അക്ഷരം op = '*';
            എങ്കിൽ( isScaled(e1) )
            {
                m1 = e1.a;
                scale *= e1.alpha;
            }
            അല്ലെങ്കിൽ
                e1.op->assign(e1, m1);

            എങ്കിൽ( isScaled(e2) )
            {
                m2 = e2.a;
                scale *= e2.alpha;
            }
            അല്ലെങ്കിൽ എങ്കിൽ( isReciprocal(e2) )
            {
                op = '/';
                m2 = e2.a;
                scale *= e2.alpha;
            }
            അല്ലെങ്കിൽ
                e2.op->assign(e2, m2);

            MatOp_Bin::make_വ്യഞ്ജകം(res, op, m1, m2, scale);
        }
    }
    അല്ലെങ്കിൽ
        e2.op->multiply(e1, e2, res, scale);
}


ശൂന്യം MatOp::multiply(സ്ഥിരം Mat_വ്യഞ്ജകം& വ്യഞ്ജകം, ഇരട്ട s, Mat_വ്യഞ്ജകം& res) സ്ഥിരം
{
    CV_INSTRUMENT_REGION();

    Mat m;
    വ്യഞ്ജകം.op->assign(വ്യഞ്ജകം, m);
    MatOp_AddEx::make_വ്യഞ്ജകം(res, m, Mat(), s, 0);
}


ശൂന്യം MatOp::divide(സ്ഥിരം Mat_വ്യഞ്ജകം& e1, സ്ഥിരം Mat_വ്യഞ്ജകം& e2, Mat_വ്യഞ്ജകം& res, ഇരട്ട scale) സ്ഥിരം
{
    CV_INSTRUMENT_REGION();

    എങ്കിൽ( ഇത് == e2.op )
    {
        എങ്കിൽ( isReciprocal(e1) && isReciprocal(e2) )
            MatOp_Bin::make_വ്യഞ്ജകം(res, '/', e2.a, e1.a, e1.alpha/e2.alpha);
        അല്ലെങ്കിൽ
        {
            Mat m1, m2;
            അക്ഷരം op = '/';

            എങ്കിൽ( isScaled(e1) )
            {
                m1 = e1.a;
                scale *= e1.alpha;
            }
            അല്ലെങ്കിൽ
                e1.op->assign(e1, m1);

            എങ്കിൽ( isScaled(e2) )
            {
                m2 = e2.a;
                scale /= e2.alpha;
            }
            അല്ലെങ്കിൽ എങ്കിൽ( isReciprocal(e2) )
            {
                m2 = e2.a;
                scale /= e2.alpha;
                op = '*';
            }
            അല്ലെങ്കിൽ
                e2.op->assign(e2, m2);
            MatOp_Bin::make_വ്യഞ്ജകം(res, op, m1, m2, scale);
        }
    }
    അല്ലെങ്കിൽ
        e2.op->divide(e1, e2, res, scale);
}


ശൂന്യം MatOp::divide(ഇരട്ട s, സ്ഥിരം Mat_വ്യഞ്ജകം& വ്യഞ്ജകം, Mat_വ്യഞ്ജകം& res) സ്ഥിരം
{
    CV_INSTRUMENT_REGION();

    Mat m;
    വ്യഞ്ജകം.op->assign(വ്യഞ്ജകം, m);
    MatOp_Bin::make_വ്യഞ്ജകം(res, '/', m, Mat(), s);
}


ശൂന്യം MatOp::abs(സ്ഥിരം Mat_വ്യഞ്ജകം& വ്യഞ്ജകം, Mat_വ്യഞ്ജകം& res) സ്ഥിരം
{
    CV_INSTRUMENT_REGION();

    Mat m;
    വ്യഞ്ജകം.op->assign(വ്യഞ്ജകം, m);
    MatOp_Bin::make_വ്യഞ്ജകം(res, 'a', m, Mat());
}


ശൂന്യം MatOp::transpose(സ്ഥിരം Mat_വ്യഞ്ജകം& വ്യഞ്ജകം, Mat_വ്യഞ്ജകം& res) സ്ഥിരം
{
    CV_INSTRUMENT_REGION();

    Mat m;
    വ്യഞ്ജകം.op->assign(വ്യഞ്ജകം, m);
    MatOp_T::make_വ്യഞ്ജകം(res, m, 1);
}


ശൂന്യം MatOp::matmul(സ്ഥിരം Mat_വ്യഞ്ജകം& e1, സ്ഥിരം Mat_വ്യഞ്ജകം& e2, Mat_വ്യഞ്ജകം& res) സ്ഥിരം
{
    എങ്കിൽ( ഇത് == e2.op )
    {
        ഇരട്ട scale = 1;
        പൂർണ്ണസംഖ്യ flags = 0;
        Mat m1, m2;

        എങ്കിൽ( isT(e1) )
        {
            flags = cv::GEMM_1_T;
            scale = e1.alpha;
            m1 = e1.a;
        }
        അല്ലെങ്കിൽ എങ്കിൽ( isScaled(e1) )
        {
            scale = e1.alpha;
            m1 = e1.a;
        }
        അല്ലെങ്കിൽ
            e1.op->assign(e1, m1);

        എങ്കിൽ( isT(e2) )
        {
            flags |= cv::GEMM_2_T;
            scale *= e2.alpha;
            m2 = e2.a;
        }
        അല്ലെങ്കിൽ എങ്കിൽ( isScaled(e2) )
        {
            scale *= e2.alpha;
            m2 = e2.a;
        }
        അല്ലെങ്കിൽ
            e2.op->assign(e2, m2);

        MatOp_GEMM::make_വ്യഞ്ജകം(res, flags, m1, m2, scale);
    }
    അല്ലെങ്കിൽ
        e2.op->matmul(e1, e2, res);
}


ശൂന്യം MatOp::invert(സ്ഥിരം Mat_വ്യഞ്ജകം& വ്യഞ്ജകം, പൂർണ്ണസംഖ്യ method, Mat_വ്യഞ്ജകം& res) സ്ഥിരം
{
    Mat m;
    വ്യഞ്ജകം.op->assign(വ്യഞ്ജകം, m);
    MatOp_Invert::make_വ്യഞ്ജകം(res, method, m);
}


Size MatOp::size(സ്ഥിരം Mat_വ്യഞ്ജകം& വ്യഞ്ജകം) സ്ഥിരം
{
    തിരികെ !വ്യഞ്ജകം.a.empty() ? വ്യഞ്ജകം.a.size() : വ്യഞ്ജകം.b.empty() ? വ്യഞ്ജകം.b.size() : വ്യഞ്ജകം.c.size();
}

പൂർണ്ണസംഖ്യ MatOp::type(സ്ഥിരം Mat_വ്യഞ്ജകം& വ്യഞ്ജകം) സ്ഥിരം
{
    CV_INSTRUMENT_REGION();

    തിരികെ !വ്യഞ്ജകം.a.empty() ? വ്യഞ്ജകം.a.type() : വ്യഞ്ജകം.b.empty() ? വ്യഞ്ജകം.b.type() : വ്യഞ്ജകം.c.type();
}

//////////////////////////////////////////////////////////////////////////////////////////////////

Mat_വ്യഞ്ജകം::Mat_വ്യഞ്ജകം(സ്ഥിരം Mat& m) : op(&g_MatOp_Identity), flags(0), a(m), b(Mat()), c(Mat()), alpha(1), beta(0), s(Scalar())
{
}

Mat_വ്യഞ്ജകം Mat_വ്യഞ്ജകം::row(പൂർണ്ണസംഖ്യ y) സ്ഥിരം
{
    Mat_വ്യഞ്ജകം e;
    op->roi(*ഇത്, Range(y, y+1), Range::all(), e);
    തിരികെ e;
}

Mat_വ്യഞ്ജകം Mat_വ്യഞ്ജകം::col(പൂർണ്ണസംഖ്യ x) സ്ഥിരം
{
    Mat_വ്യഞ്ജകം e;
    op->roi(*ഇത്, Range::all(), Range(x, x+1), e);
    തിരികെ e;
}

Mat_വ്യഞ്ജകം Mat_വ്യഞ്ജകം::diag(പൂർണ്ണസംഖ്യ d) സ്ഥിരം
{
    Mat_വ്യഞ്ജകം e;
    op->diag(*ഇത്, d, e);
    തിരികെ e;
}

Mat_വ്യഞ്ജകം Mat_വ്യഞ്ജകം::operator()( സ്ഥിരം Range& rowRange, സ്ഥിരം Range& colRange ) സ്ഥിരം
{
    Mat_വ്യഞ്ജകം e;
    op->roi(*ഇത്, rowRange, colRange, e);
    തിരികെ e;
}

Mat_വ്യഞ്ജകം Mat_വ്യഞ്ജകം::operator()( സ്ഥിരം Rect& roi ) സ്ഥിരം
{
    Mat_വ്യഞ്ജകം e;
    op->roi(*ഇത്, Range(roi.y, roi.y + roi.height), Range(roi.x, roi.x + roi.width), e);
    തിരികെ e;
}

Mat Mat_വ്യഞ്ജകം::cross(സ്ഥിരം Mat& m) സ്ഥിരം
{
    തിരികെ ((Mat)*ഇത്).cross(m);
}

ഇരട്ട Mat_വ്യഞ്ജകം::dot(സ്ഥിരം Mat& m) സ്ഥിരം
{
    തിരികെ ((Mat)*ഇത്).dot(m);
}

Mat_വ്യഞ്ജകം Mat_വ്യഞ്ജകം::t() സ്ഥിരം
{
    Mat_വ്യഞ്ജകം e;
    op->transpose(*ഇത്, e);
    തിരികെ e;
}

Mat_വ്യഞ്ജകം Mat_വ്യഞ്ജകം::inv(പൂർണ്ണസംഖ്യ method) സ്ഥിരം
{
    Mat_വ്യഞ്ജകം e;
    op->invert(*ഇത്, method, e);
    തിരികെ e;
}

Mat_വ്യഞ്ജകം Mat_വ്യഞ്ജകം::mul(സ്ഥിരം Mat_വ്യഞ്ജകം& e, ഇരട്ട scale) സ്ഥിരം
{
    Mat_വ്യഞ്ജകം en;
    op->multiply(*ഇത്, e, en, scale);
    തിരികെ en;
}

Mat_വ്യഞ്ജകം Mat_വ്യഞ്ജകം::mul(സ്ഥിരം Mat& m, ഇരട്ട scale) സ്ഥിരം
{
    Mat_വ്യഞ്ജകം e;
    op->multiply(*ഇത്, Mat_വ്യഞ്ജകം(m), e, scale);
    തിരികെ e;
}

Mat_വ്യഞ്ജകം operator + (സ്ഥിരം Mat& a, സ്ഥിരം Mat& b)
{
    checkOperandsExist(a, b);
    Mat_വ്യഞ്ജകം e;
    MatOp_AddEx::make_വ്യഞ്ജകം(e, a, b, 1, 1);
    തിരികെ e;
}

Mat_വ്യഞ്ജകം operator + (സ്ഥിരം Mat& a, സ്ഥിരം Scalar& s)
{
    checkOperandsExist(a);
    Mat_വ്യഞ്ജകം e;
    MatOp_AddEx::make_വ്യഞ്ജകം(e, a, Mat(), 1, 0, s);
    തിരികെ e;
}

Mat_വ്യഞ്ജകം operator + (സ്ഥിരം Scalar& s, സ്ഥിരം Mat& a)
{
    checkOperandsExist(a);
    Mat_വ്യഞ്ജകം e;
    MatOp_AddEx::make_വ്യഞ്ജകം(e, a, Mat(), 1, 0, s);
    തിരികെ e;
}

Mat_വ്യഞ്ജകം operator + (സ്ഥിരം Mat_വ്യഞ്ജകം& e, സ്ഥിരം Mat& m)
{
    Mat_വ്യഞ്ജകം en;
    e.op->add(e, Mat_വ്യഞ്ജകം(m), en);
    തിരികെ en;
}

Mat_വ്യഞ്ജകം operator + (സ്ഥിരം Mat& m, സ്ഥിരം Mat_വ്യഞ്ജകം& e)
{
    checkOperandsExist(m);
    Mat_വ്യഞ്ജകം en;
    e.op->add(e, Mat_വ്യഞ്ജകം(m), en);
    തിരികെ en;
}

Mat_വ്യഞ്ജകം operator + (സ്ഥിരം Mat_വ്യഞ്ജകം& e, സ്ഥിരം Scalar& s)
{
    Mat_വ്യഞ്ജകം en;
    e.op->add(e, s, en);
    തിരികെ en;
}

Mat_വ്യഞ്ജകം operator + (സ്ഥിരം Scalar& s, സ്ഥിരം Mat_വ്യഞ്ജകം& e)
{
    Mat_വ്യഞ്ജകം en;
    e.op->add(e, s, en);
    തിരികെ en;
}

Mat_വ്യഞ്ജകം operator + (സ്ഥിരം Mat_വ്യഞ്ജകം& e1, സ്ഥിരം Mat_വ്യഞ്ജകം& e2)
{
    Mat_വ്യഞ്ജകം en;
    e1.op->add(e1, e2, en);
    തിരികെ en;
}

Mat_വ്യഞ്ജകം operator - (സ്ഥിരം Mat& a, സ്ഥിരം Mat& b)
{
    checkOperandsExist(a, b);
    Mat_വ്യഞ്ജകം e;
    MatOp_AddEx::make_വ്യഞ്ജകം(e, a, b, 1, -1);
    തിരികെ e;
}

Mat_വ്യഞ്ജകം operator - (സ്ഥിരം Mat& a, സ്ഥിരം Scalar& s)
{
    checkOperandsExist(a);
    Mat_വ്യഞ്ജകം e;
    MatOp_AddEx::make_വ്യഞ്ജകം(e, a, Mat(), 1, 0, -s);
    തിരികെ e;
}

Mat_വ്യഞ്ജകം operator - (സ്ഥിരം Scalar& s, സ്ഥിരം Mat& a)
{
    checkOperandsExist(a);
    Mat_വ്യഞ്ജകം e;
    MatOp_AddEx::make_വ്യഞ്ജകം(e, a, Mat(), -1, 0, s);
    തിരികെ e;
}

Mat_വ്യഞ്ജകം operator - (സ്ഥിരം Mat_വ്യഞ്ജകം& e, സ്ഥിരം Mat& m)
{
    checkOperandsExist(m);
    Mat_വ്യഞ്ജകം en;
    e.op->subtract(e, Mat_വ്യഞ്ജകം(m), en);
    തിരികെ en;
}

Mat_വ്യഞ്ജകം operator - (സ്ഥിരം Mat& m, സ്ഥിരം Mat_വ്യഞ്ജകം& e)
{
    checkOperandsExist(m);
    Mat_വ്യഞ്ജകം en;
    e.op->subtract(Mat_വ്യഞ്ജകം(m), e, en);
    തിരികെ en;
}

Mat_വ്യഞ്ജകം operator - (സ്ഥിരം Mat_വ്യഞ്ജകം& e, സ്ഥിരം Scalar& s)
{
    Mat_വ്യഞ്ജകം en;
    e.op->add(e, -s, en);
    തിരികെ en;
}

Mat_വ്യഞ്ജകം operator - (സ്ഥിരം Scalar& s, സ്ഥിരം Mat_വ്യഞ്ജകം& e)
{
    Mat_വ്യഞ്ജകം en;
    e.op->subtract(s, e, en);
    തിരികെ en;
}

Mat_വ്യഞ്ജകം operator - (സ്ഥിരം Mat_വ്യഞ്ജകം& e1, സ്ഥിരം Mat_വ്യഞ്ജകം& e2)
{
    Mat_വ്യഞ്ജകം en;
    e1.op->subtract(e1, e2, en);
    തിരികെ en;
}

Mat_വ്യഞ്ജകം operator - (സ്ഥിരം Mat& m)
{
    checkOperandsExist(m);
    Mat_വ്യഞ്ജകം e;
    MatOp_AddEx::make_വ്യഞ്ജകം(e, m, Mat(), -1, 0);
    തിരികെ e;
}

Mat_വ്യഞ്ജകം operator - (സ്ഥിരം Mat_വ്യഞ്ജകം& e)
{
    Mat_വ്യഞ്ജകം en;
    e.op->subtract(Scalar(0), e, en);
    തിരികെ en;
}

Mat_വ്യഞ്ജകം operator * (സ്ഥിരം Mat& a, സ്ഥിരം Mat& b)
{
    checkOperandsExist(a, b);
    Mat_വ്യഞ്ജകം e;
    MatOp_GEMM::make_വ്യഞ്ജകം(e, 0, a, b);
    തിരികെ e;
}

Mat_വ്യഞ്ജകം operator * (സ്ഥിരം Mat& a, ഇരട്ട s)
{
    checkOperandsExist(a);
    Mat_വ്യഞ്ജകം e;
    MatOp_AddEx::make_വ്യഞ്ജകം(e, a, Mat(), s, 0);
    തിരികെ e;
}

Mat_വ്യഞ്ജകം operator * (ഇരട്ട s, സ്ഥിരം Mat& a)
{
    checkOperandsExist(a);
    Mat_വ്യഞ്ജകം e;
    MatOp_AddEx::make_വ്യഞ്ജകം(e, a, Mat(), s, 0);
    തിരികെ e;
}

Mat_വ്യഞ്ജകം operator * (സ്ഥിരം Mat_വ്യഞ്ജകം& e, സ്ഥിരം Mat& m)
{
    Mat_വ്യഞ്ജകം en;
    e.op->matmul(e, Mat_വ്യഞ്ജകം(m), en);
    തിരികെ en;
}

Mat_വ്യഞ്ജകം operator * (സ്ഥിരം Mat& m, സ്ഥിരം Mat_വ്യഞ്ജകം& e)
{
    checkOperandsExist(m);
    Mat_വ്യഞ്ജകം en;
    e.op->matmul(Mat_വ്യഞ്ജകം(m), e, en);
    തിരികെ en;
}

Mat_വ്യഞ്ജകം operator * (സ്ഥിരം Mat_വ്യഞ്ജകം& e, ഇരട്ട s)
{
    Mat_വ്യഞ്ജകം en;
    e.op->multiply(e, s, en);
    തിരികെ en;
}

Mat_വ്യഞ്ജകം operator * (ഇരട്ട s, സ്ഥിരം Mat_വ്യഞ്ജകം& e)
{
    Mat_വ്യഞ്ജകം en;
    e.op->multiply(e, s, en);
    തിരികെ en;
}

Mat_വ്യഞ്ജകം operator * (സ്ഥിരം Mat_വ്യഞ്ജകം& e1, സ്ഥിരം Mat_വ്യഞ്ജകം& e2)
{
    Mat_വ്യഞ്ജകം en;
    e1.op->matmul(e1, e2, en);
    തിരികെ en;
}

Mat_വ്യഞ്ജകം operator / (സ്ഥിരം Mat& a, സ്ഥിരം Mat& b)
{
    checkOperandsExist(a, b);
    Mat_വ്യഞ്ജകം e;
    MatOp_Bin::make_വ്യഞ്ജകം(e, '/', a, b);
    തിരികെ e;
}

Mat_വ്യഞ്ജകം operator / (സ്ഥിരം Mat& a, ഇരട്ട s)
{
    checkOperandsExist(a);
    Mat_വ്യഞ്ജകം e;
    MatOp_AddEx::make_വ്യഞ്ജകം(e, a, Mat(), 1./s, 0);
    തിരികെ e;
}

Mat_വ്യഞ്ജകം operator / (ഇരട്ട s, സ്ഥിരം Mat& a)
{
    checkOperandsExist(a);
    Mat_വ്യഞ്ജകം e;
    MatOp_Bin::make_വ്യഞ്ജകം(e, '/', a, Mat(), s);
    തിരികെ e;
}

Mat_വ്യഞ്ജകം operator / (സ്ഥിരം Mat_വ്യഞ്ജകം& e, സ്ഥിരം Mat& m)
{
    Mat_വ്യഞ്ജകം en;
    e.op->divide(e, Mat_വ്യഞ്ജകം(m), en);
    തിരികെ en;
}

Mat_വ്യഞ്ജകം operator / (സ്ഥിരം Mat& m, സ്ഥിരം Mat_വ്യഞ്ജകം& e)
{
    checkOperandsExist(m);
    Mat_വ്യഞ്ജകം en;
    e.op->divide(Mat_വ്യഞ്ജകം(m), e, en);
    തിരികെ en;
}

Mat_വ്യഞ്ജകം operator / (സ്ഥിരം Mat_വ്യഞ്ജകം& e, ഇരട്ട s)
{
    Mat_വ്യഞ്ജകം en;
    e.op->multiply(e, 1./s, en);
    തിരികെ en;
}

Mat_വ്യഞ്ജകം operator / (ഇരട്ട s, സ്ഥിരം Mat_വ്യഞ്ജകം& e)
{
    Mat_വ്യഞ്ജകം en;
    e.op->divide(s, e, en);
    തിരികെ en;
}

Mat_വ്യഞ്ജകം operator / (സ്ഥിരം Mat_വ്യഞ്ജകം& e1, സ്ഥിരം Mat_വ്യഞ്ജകം& e2)
{
    Mat_വ്യഞ്ജകം en;
    e1.op->divide(e1, e2, en);
    തിരികെ en;
}

Mat_വ്യഞ്ജകം operator < (സ്ഥിരം Mat& a, സ്ഥിരം Mat& b)
{
    checkOperandsExist(a, b);
    Mat_വ്യഞ്ജകം e;
    MatOp_Cmp::make_വ്യഞ്ജകം(e, cv::CMP_LT, a, b);
    തിരികെ e;
}

Mat_വ്യഞ്ജകം operator < (സ്ഥിരം Mat& a, ഇരട്ട s)
{
    checkOperandsExist(a);
    Mat_വ്യഞ്ജകം e;
    MatOp_Cmp::make_വ്യഞ്ജകം(e, cv::CMP_LT, a, s);
    തിരികെ e;
}

Mat_വ്യഞ്ജകം operator < (ഇരട്ട s, സ്ഥിരം Mat& a)
{
    checkOperandsExist(a);
    Mat_വ്യഞ്ജകം e;
    MatOp_Cmp::make_വ്യഞ്ജകം(e, cv::CMP_GT, a, s);
    തിരികെ e;
}

Mat_വ്യഞ്ജകം operator <= (സ്ഥിരം Mat& a, സ്ഥിരം Mat& b)
{
    checkOperandsExist(a, b);
    Mat_വ്യഞ്ജകം e;
    MatOp_Cmp::make_വ്യഞ്ജകം(e, cv::CMP_LE, a, b);
    തിരികെ e;
}

Mat_വ്യഞ്ജകം operator <= (സ്ഥിരം Mat& a, ഇരട്ട s)
{
    checkOperandsExist(a);
    Mat_വ്യഞ്ജകം e;
    MatOp_Cmp::make_വ്യഞ്ജകം(e, cv::CMP_LE, a, s);
    തിരികെ e;
}

Mat_വ്യഞ്ജകം operator <= (ഇരട്ട s, സ്ഥിരം Mat& a)
{
    checkOperandsExist(a);
    Mat_വ്യഞ്ജകം e;
    MatOp_Cmp::make_വ്യഞ്ജകം(e, cv::CMP_GE, a, s);
    തിരികെ e;
}

Mat_വ്യഞ്ജകം operator == (സ്ഥിരം Mat& a, സ്ഥിരം Mat& b)
{
    checkOperandsExist(a, b);
    Mat_വ്യഞ്ജകം e;
    MatOp_Cmp::make_വ്യഞ്ജകം(e, cv::CMP_EQ, a, b);
    തിരികെ e;
}

Mat_വ്യഞ്ജകം operator == (സ്ഥിരം Mat& a, ഇരട്ട s)
{
    checkOperandsExist(a);
    Mat_വ്യഞ്ജകം e;
    MatOp_Cmp::make_വ്യഞ്ജകം(e, cv::CMP_EQ, a, s);
    തിരികെ e;
}

Mat_വ്യഞ്ജകം operator == (ഇരട്ട s, സ്ഥിരം Mat& a)
{
    checkOperandsExist(a);
    Mat_വ്യഞ്ജകം e;
    MatOp_Cmp::make_വ്യഞ്ജകം(e, cv::CMP_EQ, a, s);
    തിരികെ e;
}

Mat_വ്യഞ്ജകം operator != (സ്ഥിരം Mat& a, സ്ഥിരം Mat& b)
{
    checkOperandsExist(a, b);
    Mat_വ്യഞ്ജകം e;
    MatOp_Cmp::make_വ്യഞ്ജകം(e, cv::CMP_NE, a, b);
    തിരികെ e;
}

Mat_വ്യഞ്ജകം operator != (സ്ഥിരം Mat& a, ഇരട്ട s)
{
    checkOperandsExist(a);
    Mat_വ്യഞ്ജകം e;
    MatOp_Cmp::make_വ്യഞ്ജകം(e, cv::CMP_NE, a, s);
    തിരികെ e;
}

Mat_വ്യഞ്ജകം operator != (ഇരട്ട s, സ്ഥിരം Mat& a)
{
    checkOperandsExist(a);
    Mat_വ്യഞ്ജകം e;
    MatOp_Cmp::make_വ്യഞ്ജകം(e, cv::CMP_NE, a, s);
    തിരികെ e;
}

Mat_വ്യഞ്ജകം operator >= (സ്ഥിരം Mat& a, സ്ഥിരം Mat& b)
{
    checkOperandsExist(a, b);
    Mat_വ്യഞ്ജകം e;
    MatOp_Cmp::make_വ്യഞ്ജകം(e, cv::CMP_GE, a, b);
    തിരികെ e;
}

Mat_വ്യഞ്ജകം operator >= (സ്ഥിരം Mat& a, ഇരട്ട s)
{
    checkOperandsExist(a);
    Mat_വ്യഞ്ജകം e;
    MatOp_Cmp::make_വ്യഞ്ജകം(e, cv::CMP_GE, a, s);
    തിരികെ e;
}

Mat_വ്യഞ്ജകം operator >= (ഇരട്ട s, സ്ഥിരം Mat& a)
{
    checkOperandsExist(a);
    Mat_വ്യഞ്ജകം e;
    MatOp_Cmp::make_വ്യഞ്ജകം(e, cv::CMP_LE, a, s);
    തിരികെ e;
}

Mat_വ്യഞ്ജകം operator > (സ്ഥിരം Mat& a, സ്ഥിരം Mat& b)
{
    checkOperandsExist(a, b);
    Mat_വ്യഞ്ജകം e;
    MatOp_Cmp::make_വ്യഞ്ജകം(e, cv::CMP_GT, a, b);
    തിരികെ e;
}

Mat_വ്യഞ്ജകം operator > (സ്ഥിരം Mat& a, ഇരട്ട s)
{
    checkOperandsExist(a);
    Mat_വ്യഞ്ജകം e;
    MatOp_Cmp::make_വ്യഞ്ജകം(e, cv::CMP_GT, a, s);
    തിരികെ e;
}

Mat_വ്യഞ്ജകം operator > (ഇരട്ട s, സ്ഥിരം Mat& a)
{
    checkOperandsExist(a);
    Mat_വ്യഞ്ജകം e;
    MatOp_Cmp::make_വ്യഞ്ജകം(e, cv::CMP_LT, a, s);
    തിരികെ e;
}

Mat_വ്യഞ്ജകം min(സ്ഥിരം Mat& a, സ്ഥിരം Mat& b)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a, b);
    Mat_വ്യഞ്ജകം e;
    MatOp_Bin::make_വ്യഞ്ജകം(e, 'm', a, b);
    തിരികെ e;
}

Mat_വ്യഞ്ജകം min(സ്ഥിരം Mat& a, ഇരട്ട s)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    Mat_വ്യഞ്ജകം e;
    MatOp_Bin::make_വ്യഞ്ജകം(e, 'n', a, s);
    തിരികെ e;
}

Mat_വ്യഞ്ജകം min(ഇരട്ട s, സ്ഥിരം Mat& a)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    Mat_വ്യഞ്ജകം e;
    MatOp_Bin::make_വ്യഞ്ജകം(e, 'n', a, s);
    തിരികെ e;
}

Mat_വ്യഞ്ജകം max(സ്ഥിരം Mat& a, സ്ഥിരം Mat& b)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a, b);
    Mat_വ്യഞ്ജകം e;
    MatOp_Bin::make_വ്യഞ്ജകം(e, 'M', a, b);
    തിരികെ e;
}

Mat_വ്യഞ്ജകം max(സ്ഥിരം Mat& a, ഇരട്ട s)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    Mat_വ്യഞ്ജകം e;
    MatOp_Bin::make_വ്യഞ്ജകം(e, 'N', a, s);
    തിരികെ e;
}

Mat_വ്യഞ്ജകം max(ഇരട്ട s, സ്ഥിരം Mat& a)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    Mat_വ്യഞ്ജകം e;
    MatOp_Bin::make_വ്യഞ്ജകം(e, 'N', a, s);
    തിരികെ e;
}

Mat_വ്യഞ്ജകം operator & (സ്ഥിരം Mat& a, സ്ഥിരം Mat& b)
{
    checkOperandsExist(a, b);
    Mat_വ്യഞ്ജകം e;
    MatOp_Bin::make_വ്യഞ്ജകം(e, '&', a, b);
    തിരികെ e;
}

Mat_വ്യഞ്ജകം operator & (സ്ഥിരം Mat& a, സ്ഥിരം Scalar& s)
{
    checkOperandsExist(a);
    Mat_വ്യഞ്ജകം e;
    MatOp_Bin::make_വ്യഞ്ജകം(e, '&', a, s);
    തിരികെ e;
}

Mat_വ്യഞ്ജകം operator & (സ്ഥിരം Scalar& s, സ്ഥിരം Mat& a)
{
    checkOperandsExist(a);
    Mat_വ്യഞ്ജകം e;
    MatOp_Bin::make_വ്യഞ്ജകം(e, '&', a, s);
    തിരികെ e;
}

Mat_വ്യഞ്ജകം operator | (സ്ഥിരം Mat& a, സ്ഥിരം Mat& b)
{
    checkOperandsExist(a, b);
    Mat_വ്യഞ്ജകം e;
    MatOp_Bin::make_വ്യഞ്ജകം(e, '|', a, b);
    തിരികെ e;
}

Mat_വ്യഞ്ജകം operator | (സ്ഥിരം Mat& a, സ്ഥിരം Scalar& s)
{
    checkOperandsExist(a);
    Mat_വ്യഞ്ജകം e;
    MatOp_Bin::make_വ്യഞ്ജകം(e, '|', a, s);
    തിരികെ e;
}

Mat_വ്യഞ്ജകം operator | (സ്ഥിരം Scalar& s, സ്ഥിരം Mat& a)
{
    checkOperandsExist(a);
    Mat_വ്യഞ്ജകം e;
    MatOp_Bin::make_വ്യഞ്ജകം(e, '|', a, s);
    തിരികെ e;
}

Mat_വ്യഞ്ജകം operator ^ (സ്ഥിരം Mat& a, സ്ഥിരം Mat& b)
{
    checkOperandsExist(a, b);
    Mat_വ്യഞ്ജകം e;
    MatOp_Bin::make_വ്യഞ്ജകം(e, '^', a, b);
    തിരികെ e;
}

Mat_വ്യഞ്ജകം operator ^ (സ്ഥിരം Mat& a, സ്ഥിരം Scalar& s)
{
    checkOperandsExist(a);
    Mat_വ്യഞ്ജകം e;
    MatOp_Bin::make_വ്യഞ്ജകം(e, '^', a, s);
    തിരികെ e;
}

Mat_വ്യഞ്ജകം operator ^ (സ്ഥിരം Scalar& s, സ്ഥിരം Mat& a)
{
    checkOperandsExist(a);
    Mat_വ്യഞ്ജകം e;
    MatOp_Bin::make_വ്യഞ്ജകം(e, '^', a, s);
    തിരികെ e;
}

Mat_വ്യഞ്ജകം operator ~(സ്ഥിരം Mat& a)
{
    checkOperandsExist(a);
    Mat_വ്യഞ്ജകം e;
    MatOp_Bin::make_വ്യഞ്ജകം(e, '~', a, Scalar());
    തിരികെ e;
}

Mat_വ്യഞ്ജകം abs(സ്ഥിരം Mat& a)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    Mat_വ്യഞ്ജകം e;
    MatOp_Bin::make_വ്യഞ്ജകം(e, 'a', a, Scalar());
    തിരികെ e;
}

Mat_വ്യഞ്ജകം abs(സ്ഥിരം Mat_വ്യഞ്ജകം& e)
{
    CV_INSTRUMENT_REGION();

    Mat_വ്യഞ്ജകം en;
    e.op->abs(e, en);
    തിരികെ en;
}


Size Mat_വ്യഞ്ജകം::size() സ്ഥിരം
{
    എങ്കിൽ( isT(*ഇത്) || isInv(*ഇത്) )
        തിരികെ Size(a.rows, a.cols);
    എങ്കിൽ( isSolve(*ഇത്) )
        തിരികെ Size(b.cols, a.cols);
    എങ്കിൽ( isInitializer(*ഇത്) )
        തിരികെ a.size();
    തിരികെ op ? op->size(*ഇത്) : Size();
}


പൂർണ്ണസംഖ്യ Mat_വ്യഞ്ജകം::type() സ്ഥിരം
{
    CV_INSTRUMENT_REGION();

    എങ്കിൽ( isInitializer(*ഇത്) )
        തിരികെ a.type();
    എങ്കിൽ( isCmp(*ഇത്) )
        തിരികെ CV_MAKETYPE(CV_8U, a.channels());
    തിരികെ op ? op->type(*ഇത്) : -1;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////

ശൂന്യം MatOp_Identity::assign(സ്ഥിരം Mat_വ്യഞ്ജകം& e, Mat& m, പൂർണ്ണസംഖ്യ _type) സ്ഥിരം
{
    എങ്കിൽ( _type == -1 || _type == e.a.type() )
        m = e.a;
    അല്ലെങ്കിൽ
    {
        CV_Assert( CV_MAT_CN(_type) == e.a.channels() );
        e.a.convertTo(m, _type);
    }
}

വരിയിൽ ശൂന്യം MatOp_Identity::make_വ്യഞ്ജകം(Mat_വ്യഞ്ജകം& res, സ്ഥിരം Mat& m)
{
    res = Mat_വ്യഞ്ജകം(&g_MatOp_Identity, 0, m, Mat(), Mat(), 1, 0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

ശൂന്യം MatOp_AddEx::assign(സ്ഥിരം Mat_വ്യഞ്ജകം& e, Mat& m, പൂർണ്ണസംഖ്യ _type) സ്ഥിരം
{
    Mat temp, &dst = _type == -1 || e.a.type() == _type ? m : temp;
    എങ്കിൽ( e.b.data )
    {
        എങ്കിൽ( e.s == Scalar() || !e.s.isReal() )
        {
            എങ്കിൽ( e.alpha == 1 )
            {
                എങ്കിൽ( e.beta == 1 )
                    cv::add(e.a, e.b, dst);
                അല്ലെങ്കിൽ എങ്കിൽ( e.beta == -1 )
                    cv::subtract(e.a, e.b, dst);
                അല്ലെങ്കിൽ
                    cv::scaleAdd(e.b, e.beta, e.a, dst);
            }
            അല്ലെങ്കിൽ എങ്കിൽ( e.beta == 1 )
            {
                എങ്കിൽ( e.alpha == -1 )
                    cv::subtract(e.b, e.a, dst);
                അല്ലെങ്കിൽ
                    cv::scaleAdd(e.a, e.alpha, e.b, dst);
            }
            അല്ലെങ്കിൽ
                cv::addWeighted(e.a, e.alpha, e.b, e.beta, 0, dst);

            എങ്കിൽ( !e.s.isReal() )
                cv::add(dst, e.s, dst);
        }
        അല്ലെങ്കിൽ
        {
            എങ്കിൽ (e.a.channels() > 1)
                CV_LOG_ONCE_WARNING(ശൂന്യ, "OpenCV/MatExpr: processing of multi-channel arrays might be changed in the future: "
                                          "https://github.com/opencv/opencv/issues/16739");
            cv::addWeighted(e.a, e.alpha, e.b, e.beta, e.s[0], dst);
        }
    }
    അല്ലെങ്കിൽ എങ്കിൽ( e.s.isReal() && (dst.data != m.data || fabs(e.alpha) != 1))
    {
        എങ്കിൽ (e.a.channels() > 1 && e.s[0] != 0.0)
            CV_LOG_ONCE_WARNING(ശൂന്യ, "OpenCV/MatExpr: processing of multi-channel arrays might be changed in the future: "
                                      "https://github.com/opencv/opencv/issues/16739");
        e.a.convertTo(m, _type, e.alpha, e.s[0]);
        തിരികെ;
    }
    അല്ലെങ്കിൽ എങ്കിൽ( e.alpha == 1 )
        cv::add(e.a, e.s, dst);
    അല്ലെങ്കിൽ എങ്കിൽ( e.alpha == -1 )
        cv::subtract(e.s, e.a, dst);
    അല്ലെങ്കിൽ
    {
        e.a.convertTo(dst, e.a.type(), e.alpha);
        cv::add(dst, e.s, dst);
    }

    എങ്കിൽ( dst.data != m.data )
        dst.convertTo(m, m.type());
}


ശൂന്യം MatOp_AddEx::add(സ്ഥിരം Mat_വ്യഞ്ജകം& e, സ്ഥിരം Scalar& s, Mat_വ്യഞ്ജകം& res) സ്ഥിരം
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.s += s;
}


ശൂന്യം MatOp_AddEx::subtract(സ്ഥിരം Scalar& s, സ്ഥിരം Mat_വ്യഞ്ജകം& e, Mat_വ്യഞ്ജകം& res) സ്ഥിരം
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha = -res.alpha;
    res.beta = -res.beta;
    res.s = s - res.s;
}

ശൂന്യം MatOp_AddEx::multiply(സ്ഥിരം Mat_വ്യഞ്ജകം& e, ഇരട്ട s, Mat_വ്യഞ്ജകം& res) സ്ഥിരം
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha *= s;
    res.beta *= s;
    res.s *= s;
}

ശൂന്യം MatOp_AddEx::divide(ഇരട്ട s, സ്ഥിരം Mat_വ്യഞ്ജകം& e, Mat_വ്യഞ്ജകം& res) സ്ഥിരം
{
    CV_INSTRUMENT_REGION();

    എങ്കിൽ( isScaled(e) )
        MatOp_Bin::make_വ്യഞ്ജകം(res, '/', e.a, Mat(), s/e.alpha);
    അല്ലെങ്കിൽ
        MatOp::divide(s, e, res);
}


ശൂന്യം MatOp_AddEx::transpose(സ്ഥിരം Mat_വ്യഞ്ജകം& e, Mat_വ്യഞ്ജകം& res) സ്ഥിരം
{
    CV_INSTRUMENT_REGION();

    എങ്കിൽ( isScaled(e) )
        MatOp_T::make_വ്യഞ്ജകം(res, e.a, e.alpha);
    അല്ലെങ്കിൽ
        MatOp::transpose(e, res);
}

ശൂന്യം MatOp_AddEx::abs(സ്ഥിരം Mat_വ്യഞ്ജകം& e, Mat_വ്യഞ്ജകം& res) സ്ഥിരം
{
    CV_INSTRUMENT_REGION();

    എങ്കിൽ( (!e.b.data || e.beta == 0) && fabs(e.alpha) == 1 )
        MatOp_Bin::make_വ്യഞ്ജകം(res, 'a', e.a, -e.s*e.alpha);
    അല്ലെങ്കിൽ എങ്കിൽ( e.b.data && e.alpha + e.beta == 0 && e.alpha*e.beta == -1 )
        MatOp_Bin::make_വ്യഞ്ജകം(res, 'a', e.a, e.b);
    അല്ലെങ്കിൽ
        MatOp::abs(e, res);
}

വരിയിൽ ശൂന്യം MatOp_AddEx::make_വ്യഞ്ജകം(Mat_വ്യഞ്ജകം& res, സ്ഥിരം Mat& a, സ്ഥിരം Mat& b, ഇരട്ട alpha, ഇരട്ട beta, സ്ഥിരം Scalar& s)
{
    res = Mat_വ്യഞ്ജകം(&g_MatOp_AddEx, 0, a, b, Mat(), alpha, beta, s);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ശൂന്യം MatOp_Bin::assign(സ്ഥിരം Mat_വ്യഞ്ജകം& e, Mat& m, പൂർണ്ണസംഖ്യ _type) സ്ഥിരം
{
    Mat temp, &dst = _type == -1 || e.a.type() == _type ? m : temp;

    എങ്കിൽ( e.flags == '*' )
        cv::multiply(e.a, e.b, dst, e.alpha);
    അല്ലെങ്കിൽ എങ്കിൽ( e.flags == '/' && e.b.data )
        cv::divide(e.a, e.b, dst, e.alpha);
    അല്ലെങ്കിൽ എങ്കിൽ( e.flags == '/' && !e.b.data )
        cv::divide(e.alpha, e.a, dst );
    അല്ലെങ്കിൽ എങ്കിൽ( e.flags == '&' && e.b.data )
        bitwise_and(e.a, e.b, dst);
    അല്ലെങ്കിൽ എങ്കിൽ( e.flags == '&' && !e.b.data )
        bitwise_and(e.a, e.s, dst);
    അല്ലെങ്കിൽ എങ്കിൽ( e.flags == '|' && e.b.data )
        bitwise_or(e.a, e.b, dst);
    അല്ലെങ്കിൽ എങ്കിൽ( e.flags == '|' && !e.b.data )
        bitwise_or(e.a, e.s, dst);
    അല്ലെങ്കിൽ എങ്കിൽ( e.flags == '^' && e.b.data )
        bitwise_xor(e.a, e.b, dst);
    അല്ലെങ്കിൽ എങ്കിൽ( e.flags == '^' && !e.b.data )
        bitwise_xor(e.a, e.s, dst);
    അല്ലെങ്കിൽ എങ്കിൽ( e.flags == '~' && !e.b.data )
        bitwise_not(e.a, dst);
    അല്ലെങ്കിൽ എങ്കിൽ( e.flags == 'm' )
        cv::min(e.a, e.b, dst);
    അല്ലെങ്കിൽ എങ്കിൽ( e.flags == 'n' )
        cv::min(e.a, e.s[0], dst);
    അല്ലെങ്കിൽ എങ്കിൽ( e.flags == 'M' )
        cv::max(e.a, e.b, dst);
    അല്ലെങ്കിൽ എങ്കിൽ( e.flags == 'N' )
        cv::max(e.a, e.s[0], dst);
    അല്ലെങ്കിൽ എങ്കിൽ( e.flags == 'a' && e.b.data )
        cv::absdiff(e.a, e.b, dst);
    അല്ലെങ്കിൽ എങ്കിൽ( e.flags == 'a' && !e.b.data )
        cv::absdiff(e.a, e.s, dst);
    അല്ലെങ്കിൽ
        CV_Error(cv::Error::StsError, "Unknown operation");

    എങ്കിൽ( dst.data != m.data )
        dst.convertTo(m, _type);
}

ശൂന്യം MatOp_Bin::multiply(സ്ഥിരം Mat_വ്യഞ്ജകം& e, ഇരട്ട s, Mat_വ്യഞ്ജകം& res) സ്ഥിരം
{
    CV_INSTRUMENT_REGION();

    എങ്കിൽ( e.flags == '*' || e.flags == '/' )
    {
        res = e;
        res.alpha *= s;
    }
    അല്ലെങ്കിൽ
        MatOp::multiply(e, s, res);
}

ശൂന്യം MatOp_Bin::divide(ഇരട്ട s, സ്ഥിരം Mat_വ്യഞ്ജകം& e, Mat_വ്യഞ്ജകം& res) സ്ഥിരം
{
    CV_INSTRUMENT_REGION();

    എങ്കിൽ( e.flags == '/' && (!e.b.data || e.beta == 0) )
        MatOp_AddEx::make_വ്യഞ്ജകം(res, e.a, Mat(), s/e.alpha, 0);
    അല്ലെങ്കിൽ
        MatOp::divide(s, e, res);
}

വരിയിൽ ശൂന്യം MatOp_Bin::make_വ്യഞ്ജകം(Mat_വ്യഞ്ജകം& res, അക്ഷരം op, സ്ഥിരം Mat& a, സ്ഥിരം Mat& b, ഇരട്ട scale)
{
    res = Mat_വ്യഞ്ജകം(&g_MatOp_Bin, op, a, b, Mat(), scale, b.data ? 1 : 0);
}

വരിയിൽ ശൂന്യം MatOp_Bin::make_വ്യഞ്ജകം(Mat_വ്യഞ്ജകം& res, അക്ഷരം op, സ്ഥിരം Mat& a, സ്ഥിരം Scalar& s)
{
    res = Mat_വ്യഞ്ജകം(&g_MatOp_Bin, op, a, Mat(), Mat(), 1, 0, s);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

ശൂന്യം MatOp_Cmp::assign(സ്ഥിരം Mat_വ്യഞ്ജകം& e, Mat& m, പൂർണ്ണസംഖ്യ _type) സ്ഥിരം
{
    Mat temp, &dst = _type == -1 || _type == CV_8U ? m : temp;

    എങ്കിൽ( e.b.data )
        cv::compare(e.a, e.b, dst, e.flags);
    അല്ലെങ്കിൽ
        cv::compare(e.a, e.alpha, dst, e.flags);

    എങ്കിൽ( dst.data != m.data )
        dst.convertTo(m, _type);
}

വരിയിൽ ശൂന്യം MatOp_Cmp::make_വ്യഞ്ജകം(Mat_വ്യഞ്ജകം& res, പൂർണ്ണസംഖ്യ cmpop, സ്ഥിരം Mat& a, സ്ഥിരം Mat& b)
{
    res = Mat_വ്യഞ്ജകം(&g_MatOp_Cmp, cmpop, a, b, Mat(), 1, 1);
}

വരിയിൽ ശൂന്യം MatOp_Cmp::make_വ്യഞ്ജകം(Mat_വ്യഞ്ജകം& res, പൂർണ്ണസംഖ്യ cmpop, സ്ഥിരം Mat& a, ഇരട്ട alpha)
{
    res = Mat_വ്യഞ്ജകം(&g_MatOp_Cmp, cmpop, a, Mat(), Mat(), alpha, 1);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

ശൂന്യം MatOp_T::assign(സ്ഥിരം Mat_വ്യഞ്ജകം& e, Mat& m, പൂർണ്ണസംഖ്യ _type) സ്ഥിരം
{
    Mat temp, &dst = _type == -1 || _type == e.a.type() ? m : temp;

    cv::transpose(e.a, dst);

    എങ്കിൽ( dst.data != m.data || e.alpha != 1 )
        dst.convertTo(m, _type, e.alpha);
}

ശൂന്യം MatOp_T::multiply(സ്ഥിരം Mat_വ്യഞ്ജകം& e, ഇരട്ട s, Mat_വ്യഞ്ജകം& res) സ്ഥിരം
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha *= s;
}

ശൂന്യം MatOp_T::transpose(സ്ഥിരം Mat_വ്യഞ്ജകം& e, Mat_വ്യഞ്ജകം& res) സ്ഥിരം
{
    CV_INSTRUMENT_REGION();

    എങ്കിൽ( e.alpha == 1 )
        MatOp_Identity::make_വ്യഞ്ജകം(res, e.a);
    അല്ലെങ്കിൽ
        MatOp_AddEx::make_വ്യഞ്ജകം(res, e.a, Mat(), e.alpha, 0);
}

വരിയിൽ ശൂന്യം MatOp_T::make_വ്യഞ്ജകം(Mat_വ്യഞ്ജകം& res, സ്ഥിരം Mat& a, ഇരട്ട alpha)
{
    res = Mat_വ്യഞ്ജകം(&g_MatOp_T, 0, a, Mat(), Mat(), alpha, 0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

ശൂന്യം MatOp_GEMM::assign(സ്ഥിരം Mat_വ്യഞ്ജകം& e, Mat& m, പൂർണ്ണസംഖ്യ _type) സ്ഥിരം
{
    Mat temp, &dst = _type == -1 || _type == e.a.type() ? m : temp;

    cv::gemm(e.a, e.b, e.alpha, e.c, e.beta, dst, e.flags);
    എങ്കിൽ( dst.data != m.data )
        dst.convertTo(m, _type);
}

ശൂന്യം MatOp_GEMM::add(സ്ഥിരം Mat_വ്യഞ്ജകം& e1, സ്ഥിരം Mat_വ്യഞ്ജകം& e2, Mat_വ്യഞ്ജകം& res) സ്ഥിരം
{
    CV_INSTRUMENT_REGION();

    തർക്കം i1 = isIdentity(e1), i2 = isIdentity(e2);
    ഇരട്ട alpha1 = i1 ? 1 : e1.alpha, alpha2 = i2 ? 1 : e2.alpha;

    എങ്കിൽ( isMatProd(e1) && (i2 || isScaled(e2) || isT(e2)) )
        MatOp_GEMM::make_വ്യഞ്ജകം(res, (e1.flags & ~cv::GEMM_3_T)|(isT(e2) ? cv::GEMM_3_T : 0),
                             e1.a, e1.b, alpha1, e2.a, alpha2);
    അല്ലെങ്കിൽ എങ്കിൽ( isMatProd(e2) && (i1 || isScaled(e1) || isT(e1)) )
        MatOp_GEMM::make_വ്യഞ്ജകം(res, (e2.flags & ~cv::GEMM_3_T)|(isT(e1) ? cv::GEMM_3_T : 0),
                             e2.a, e2.b, alpha2, e1.a, alpha1);
    അല്ലെങ്കിൽ എങ്കിൽ( ഇത് == e2.op )
        MatOp::add(e1, e2, res);
    അല്ലെങ്കിൽ
        e2.op->add(e1, e2, res);
}

ശൂന്യം MatOp_GEMM::subtract(സ്ഥിരം Mat_വ്യഞ്ജകം& e1, സ്ഥിരം Mat_വ്യഞ്ജകം& e2, Mat_വ്യഞ്ജകം& res) സ്ഥിരം
{
    CV_INSTRUMENT_REGION();

    തർക്കം i1 = isIdentity(e1), i2 = isIdentity(e2);
    ഇരട്ട alpha1 = i1 ? 1 : e1.alpha, alpha2 = i2 ? 1 : e2.alpha;

    എങ്കിൽ( isMatProd(e1) && (i2 || isScaled(e2) || isT(e2)) )
        MatOp_GEMM::make_വ്യഞ്ജകം(res, (e1.flags & ~cv::GEMM_3_T)|(isT(e2) ? cv::GEMM_3_T : 0),
                             e1.a, e1.b, alpha1, e2.a, -alpha2);
    അല്ലെങ്കിൽ എങ്കിൽ( isMatProd(e2) && (i1 || isScaled(e1) || isT(e1)) )
        MatOp_GEMM::make_വ്യഞ്ജകം(res, (e2.flags & ~cv::GEMM_3_T)|(isT(e1) ? cv::GEMM_3_T : 0),
                            e2.a, e2.b, -alpha2, e1.a, alpha1);
    അല്ലെങ്കിൽ എങ്കിൽ( ഇത് == e2.op )
        MatOp::subtract(e1, e2, res);
    അല്ലെങ്കിൽ
        e2.op->subtract(e1, e2, res);
}

ശൂന്യം MatOp_GEMM::multiply(സ്ഥിരം Mat_വ്യഞ്ജകം& e, ഇരട്ട s, Mat_വ്യഞ്ജകം& res) സ്ഥിരം
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha *= s;
    res.beta *= s;
}

ശൂന്യം MatOp_GEMM::transpose(സ്ഥിരം Mat_വ്യഞ്ജകം& e, Mat_വ്യഞ്ജകം& res) സ്ഥിരം
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.flags = (!(e.flags & cv::GEMM_1_T) ? cv::GEMM_2_T : 0) |
                (!(e.flags & cv::GEMM_2_T) ? cv::GEMM_1_T : 0) |
                (!(e.flags & cv::GEMM_3_T) ? cv::GEMM_3_T : 0);
    swap(res.a, res.b);
}

വരിയിൽ ശൂന്യം MatOp_GEMM::make_വ്യഞ്ജകം(Mat_വ്യഞ്ജകം& res, പൂർണ്ണസംഖ്യ flags, സ്ഥിരം Mat& a, സ്ഥിരം Mat& b,
                                 ഇരട്ട alpha, സ്ഥിരം Mat& c, ഇരട്ട beta)
{
    res = Mat_വ്യഞ്ജകം(&g_MatOp_GEMM, flags, a, b, c, alpha, beta);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

ശൂന്യം MatOp_Invert::assign(സ്ഥിരം Mat_വ്യഞ്ജകം& e, Mat& m, പൂർണ്ണസംഖ്യ _type) സ്ഥിരം
{
    Mat temp, &dst = _type == -1 || _type == e.a.type() ? m : temp;

    cv::invert(e.a, dst, e.flags);
    എങ്കിൽ( dst.data != m.data )
        dst.convertTo(m, _type);
}

ശൂന്യം MatOp_Invert::matmul(സ്ഥിരം Mat_വ്യഞ്ജകം& e1, സ്ഥിരം Mat_വ്യഞ്ജകം& e2, Mat_വ്യഞ്ജകം& res) സ്ഥിരം
{
    എങ്കിൽ( isInv(e1) && isIdentity(e2) )
        MatOp_Solve::make_വ്യഞ്ജകം(res, e1.flags, e1.a, e2.a);
    അല്ലെങ്കിൽ എങ്കിൽ( ഇത് == e2.op )
        MatOp::matmul(e1, e2, res);
    അല്ലെങ്കിൽ
        e2.op->matmul(e1, e2, res);
}

വരിയിൽ ശൂന്യം MatOp_Invert::make_വ്യഞ്ജകം(Mat_വ്യഞ്ജകം& res, പൂർണ്ണസംഖ്യ method, സ്ഥിരം Mat& m)
{
    res = Mat_വ്യഞ്ജകം(&g_MatOp_Invert, method, m, Mat(), Mat(), 1, 0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

ശൂന്യം MatOp_Solve::assign(സ്ഥിരം Mat_വ്യഞ്ജകം& e, Mat& m, പൂർണ്ണസംഖ്യ _type) സ്ഥിരം
{
    Mat temp, &dst = _type == -1 || _type == e.a.type() ? m : temp;

    cv::solve(e.a, e.b, dst, e.flags);
    എങ്കിൽ( dst.data != m.data )
        dst.convertTo(m, _type);
}

വരിയിൽ ശൂന്യം MatOp_Solve::make_വ്യഞ്ജകം(Mat_വ്യഞ്ജകം& res, പൂർണ്ണസംഖ്യ method, സ്ഥിരം Mat& a, സ്ഥിരം Mat& b)
{
    res = Mat_വ്യഞ്ജകം(&g_MatOp_Solve, method, a, b, Mat(), 1, 1);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

ശൂന്യം MatOp_Initializer::assign(സ്ഥിരം Mat_വ്യഞ്ജകം& e, Mat& m, പൂർണ്ണസംഖ്യ _type) സ്ഥിരം
{
    എങ്കിൽ( _type == -1 )
        _type = e.a.type();

    m.create(e.a.size, _type);

    എങ്കിൽ( e.flags == 'I' && e.a.dims <= 2 )
        setIdentity(m, Scalar(e.alpha));
    അല്ലെങ്കിൽ എങ്കിൽ( e.flags == '0' )
        m = Scalar();
    അല്ലെങ്കിൽ എങ്കിൽ( e.flags == '1' )
        m = Scalar(e.alpha);
    അല്ലെങ്കിൽ
        CV_Error(cv::Error::StsError, "Invalid matrix initializer type");
}

ശൂന്യം MatOp_Initializer::multiply(സ്ഥിരം Mat_വ്യഞ്ജകം& e, ഇരട്ട s, Mat_വ്യഞ്ജകം& res) സ്ഥിരം
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha *= s;
}

വരിയിൽ ശൂന്യം MatOp_Initializer::make_വ്യഞ്ജകം(Mat_വ്യഞ്ജകം& res, പൂർണ്ണസംഖ്യ method, Size sz, പൂർണ്ണസംഖ്യ type, ഇരട്ട alpha)
{
    res = Mat_വ്യഞ്ജകം(getGlobalMatOpInitializer(), method, Mat(sz, type, (ശൂന്യം*)(size_t)0xEEEEEEEE), Mat(), Mat(), alpha, 0);
}

വരിയിൽ ശൂന്യം MatOp_Initializer::make_വ്യഞ്ജകം(Mat_വ്യഞ്ജകം& res, പൂർണ്ണസംഖ്യ method, പൂർണ്ണസംഖ്യ ndims, സ്ഥിരം പൂർണ്ണസംഖ്യ* sizes, പൂർണ്ണസംഖ്യ type, ഇരട്ട alpha)
{
    res = Mat_വ്യഞ്ജകം(getGlobalMatOpInitializer(), method, Mat(ndims, sizes, type, (ശൂന്യം*)(size_t)0xEEEEEEEE), Mat(), Mat(), alpha, 0);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

Mat_വ്യഞ്ജകം Mat::t() സ്ഥിരം
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(*ഇത്);
    Mat_വ്യഞ്ജകം e;
    MatOp_T::make_വ്യഞ്ജകം(e, *ഇത്);
    തിരികെ e;
}

Mat_വ്യഞ്ജകം Mat::inv(പൂർണ്ണസംഖ്യ method) സ്ഥിരം
{
    CV_INSTRUMENT_REGION();

    Mat_വ്യഞ്ജകം e;
    MatOp_Invert::make_വ്യഞ്ജകം(e, method, *ഇത്);
    തിരികെ e;
}


Mat_വ്യഞ്ജകം Mat::mul(നിവേശം_Array m, ഇരട്ട scale) സ്ഥിരം
{
    CV_INSTRUMENT_REGION();

    Mat b = m.getMat();
    // Unless the argument is a refcounted Mat/UMat, the header returned by getMat() may be
    // a non-owning view of caller memory (e.g. a scalar bound to _InputArray(const double&),
    // a Matx or a Vec) that the returned MatExpr can outlive, so snapshot it.
    // See https://github.com/opencv/opencv/issues/23577
    എങ്കിൽ( !m.isMat() && !m.isUMat() )
        b = b.clone();

    Mat_വ്യഞ്ജകം e;
    MatOp_Bin::make_വ്യഞ്ജകം(e, '*', *ഇത്, b, scale);
    തിരികെ e;
}

Mat_വ്യഞ്ജകം Mat::zeros(പൂർണ്ണസംഖ്യ rows, പൂർണ്ണസംഖ്യ cols, പൂർണ്ണസംഖ്യ type)
{
    CV_INSTRUMENT_REGION();

    Mat_വ്യഞ്ജകം e;
    MatOp_Initializer::make_വ്യഞ്ജകം(e, '0', Size(cols, rows), type);
    തിരികെ e;
}

Mat_വ്യഞ്ജകം Mat::zeros(Size size, പൂർണ്ണസംഖ്യ type)
{
    CV_INSTRUMENT_REGION();

    Mat_വ്യഞ്ജകം e;
    MatOp_Initializer::make_വ്യഞ്ജകം(e, '0', size, type);
    തിരികെ e;
}

Mat_വ്യഞ്ജകം Mat::zeros(പൂർണ്ണസംഖ്യ ndims, സ്ഥിരം പൂർണ്ണസംഖ്യ* sizes, പൂർണ്ണസംഖ്യ type)
{
    CV_INSTRUMENT_REGION();

    Mat_വ്യഞ്ജകം e;
    MatOp_Initializer::make_വ്യഞ്ജകം(e, '0', ndims, sizes, type);
    തിരികെ e;
}

Mat_വ്യഞ്ജകം Mat::zeros(സ്ഥിരം MatShape& shape, പൂർണ്ണസംഖ്യ type)
{
    CV_INSTRUMENT_REGION();

    Mat_വ്യഞ്ജകം e;
    MatOp_Initializer::make_വ്യഞ്ജകം(e, '0', shape.dims, shape.p, type);
    തിരികെ e;
}

Mat_വ്യഞ്ജകം Mat::ones(പൂർണ്ണസംഖ്യ rows, പൂർണ്ണസംഖ്യ cols, പൂർണ്ണസംഖ്യ type)
{
    CV_INSTRUMENT_REGION();

    Mat_വ്യഞ്ജകം e;
    MatOp_Initializer::make_വ്യഞ്ജകം(e, '1', Size(cols, rows), type);
    തിരികെ e;
}

Mat_വ്യഞ്ജകം Mat::ones(Size size, പൂർണ്ണസംഖ്യ type)
{
    CV_INSTRUMENT_REGION();

    Mat_വ്യഞ്ജകം e;
    MatOp_Initializer::make_വ്യഞ്ജകം(e, '1', size, type);
    തിരികെ e;
}

Mat_വ്യഞ്ജകം Mat::ones(പൂർണ്ണസംഖ്യ ndims, സ്ഥിരം പൂർണ്ണസംഖ്യ* sizes, പൂർണ്ണസംഖ്യ type)
{
    CV_INSTRUMENT_REGION();

    Mat_വ്യഞ്ജകം e;
    MatOp_Initializer::make_വ്യഞ്ജകം(e, '1', ndims, sizes, type);
    തിരികെ e;
}

Mat_വ്യഞ്ജകം Mat::ones(സ്ഥിരം MatShape& shape, പൂർണ്ണസംഖ്യ type)
{
    CV_INSTRUMENT_REGION();

    Mat_വ്യഞ്ജകം e;
    MatOp_Initializer::make_വ്യഞ്ജകം(e, '1', shape.dims, shape.p, type);
    തിരികെ e;
}

Mat_വ്യഞ്ജകം Mat::eye(പൂർണ്ണസംഖ്യ rows, പൂർണ്ണസംഖ്യ cols, പൂർണ്ണസംഖ്യ type)
{
    CV_INSTRUMENT_REGION();

    Mat_വ്യഞ്ജകം e;
    MatOp_Initializer::make_വ്യഞ്ജകം(e, 'I', Size(cols, rows), type);
    തിരികെ e;
}

Mat_വ്യഞ്ജകം Mat::eye(Size size, പൂർണ്ണസംഖ്യ type)
{
    CV_INSTRUMENT_REGION();

    Mat_വ്യഞ്ജകം e;
    MatOp_Initializer::make_വ്യഞ്ജകം(e, 'I', size, type);
    തിരികെ e;
}

ശൂന്യം Mat_വ്യഞ്ജകം::swap(Mat_വ്യഞ്ജകം& other)
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

നിവേശം_Array_2::നിവേശം_Array_2(സ്ഥിരം Mat_വ്യഞ്ജകം& വ്യഞ്ജകം)
{
    എങ്കിൽ (!isIdentity(വ്യഞ്ജകം))
    {
        Mat result = വ്യഞ്ജകം;  // TODO improve through refcount == 1 of expr.a (inplace operation is possible - except gemm?)
        Mat_വ്യഞ്ജകം result_വ്യഞ്ജകം(result);
        swap(const_cast<Mat_വ്യഞ്ജകം&>(വ്യഞ്ജകം), result_വ്യഞ്ജകം);
    }
    CV_Assert(isIdentity(വ്യഞ്ജകം));
    init(FIXED_TYPE + FIXED_SIZE + MAT + ACCESS_READ, &വ്യഞ്ജകം.a);
}

} // cv::
