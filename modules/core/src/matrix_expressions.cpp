// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


/* ////////////////////////////////////////////////////////////////////
//
//  Mat basic operations: Copy, Set
//
// */

#ଅନ୍ତର୍ଭାଵ "precomp.hpp"
#ଅନ୍ତର୍ଭାଵ <opencv2/core/utils/logger.hpp>

ନାମାକାଶ cv
{

//This and its overload below are used in various MatExpr operator overloads
//implemented to check that Matrix operands exist.
ସ୍ଥିର ଶୂନ୍ୟ checkOperandsExist(ଅଚଳ Mat& a)
{
    ଯଦି (a.empty())
    {
        CV_Error(cv::Error::StsBadArg, "Matrix operand is an empty matrix.");
    }
}

ସ୍ଥିର ଶୂନ୍ୟ checkOperandsExist(ଅଚଳ Mat& a, ଅଚଳ Mat& b)
{
    ଯଦି (a.empty() || b.empty())
    {
        CV_Error(cv::Error::StsBadArg, "One or more matrix operands are empty.");
    }
}


ଶ୍ରେଣୀ MatOp_Identity CV_FINAL : ସର୍ବସାଧାରଣ MatOp
{
ସର୍ବସାଧାରଣ:
    MatOp_Identity() {}
    ଆଭାସୀ ~MatOp_Identity() {}

    ତାର୍କିକ elementWise(ଅଚଳ Mat_ପଦସମଷ୍ଟି& /*ପଦସମଷ୍ଟି*/) ଅଚଳ CV_OVERRIDE { ଫେରନ୍ତୁ ସତ୍ୟ; }
    ଶୂନ୍ୟ assign(ଅଚଳ Mat_ପଦସମଷ୍ଟି& ପଦସମଷ୍ଟି, Mat& m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା type=-1) ଅଚଳ CV_OVERRIDE;

    ସ୍ଥିର ଶୂନ୍ୟ make_ପଦସମଷ୍ଟି(Mat_ପଦସମଷ୍ଟି& res, ଅଚଳ Mat& m);
};

ସ୍ଥିର MatOp_Identity g_MatOp_Identity;

ଶ୍ରେଣୀ MatOp_AddEx CV_FINAL : ସର୍ବସାଧାରଣ MatOp
{
ସର୍ବସାଧାରଣ:
    MatOp_AddEx() {}
    ଆଭାସୀ ~MatOp_AddEx() {}

    ତାର୍କିକ elementWise(ଅଚଳ Mat_ପଦସମଷ୍ଟି& /*ପଦସମଷ୍ଟି*/) ଅଚଳ CV_OVERRIDE { ଫେରନ୍ତୁ ସତ୍ୟ; }
    ଶୂନ୍ୟ assign(ଅଚଳ Mat_ପଦସମଷ୍ଟି& ପଦସମଷ୍ଟି, Mat& m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା type=-1) ଅଚଳ CV_OVERRIDE;

    ଶୂନ୍ୟ add(ଅଚଳ Mat_ପଦସମଷ୍ଟି& e1, ଅଚଳ Scalar& s, Mat_ପଦସମଷ୍ଟି& res) ଅଚଳ CV_OVERRIDE;
    ଶୂନ୍ୟ subtract(ଅଚଳ Scalar& s, ଅଚଳ Mat_ପଦସମଷ୍ଟି& ପଦସମଷ୍ଟି, Mat_ପଦସମଷ୍ଟି& res) ଅଚଳ CV_OVERRIDE;
    ଶୂନ୍ୟ multiply(ଅଚଳ Mat_ପଦସମଷ୍ଟି& e1, ଦ୍ୱିଗୁଣ s, Mat_ପଦସମଷ୍ଟି& res) ଅଚଳ CV_OVERRIDE;
    ଶୂନ୍ୟ divide(ଦ୍ୱିଗୁଣ s, ଅଚଳ Mat_ପଦସମଷ୍ଟି& e, Mat_ପଦସମଷ୍ଟି& res) ଅଚଳ CV_OVERRIDE;

    ଶୂନ୍ୟ transpose(ଅଚଳ Mat_ପଦସମଷ୍ଟି& e1, Mat_ପଦସମଷ୍ଟି& res) ଅଚଳ CV_OVERRIDE;
    ଶୂନ୍ୟ abs(ଅଚଳ Mat_ପଦସମଷ୍ଟି& ପଦସମଷ୍ଟି, Mat_ପଦସମଷ୍ଟି& res) ଅଚଳ CV_OVERRIDE;

    ସ୍ଥିର ଶୂନ୍ୟ make_ପଦସମଷ୍ଟି(Mat_ପଦସମଷ୍ଟି& res, ଅଚଳ Mat& a, ଅଚଳ Mat& b, ଦ୍ୱିଗୁଣ alpha, ଦ୍ୱିଗୁଣ beta, ଅଚଳ Scalar& s=Scalar());
};

ସ୍ଥିର MatOp_AddEx g_MatOp_AddEx;

ଶ୍ରେଣୀ MatOp_Bin CV_FINAL : ସର୍ବସାଧାରଣ MatOp
{
ସର୍ବସାଧାରଣ:
    MatOp_Bin() {}
    ଆଭାସୀ ~MatOp_Bin() {}

    ତାର୍କିକ elementWise(ଅଚଳ Mat_ପଦସମଷ୍ଟି& /*ପଦସମଷ୍ଟି*/) ଅଚଳ CV_OVERRIDE { ଫେରନ୍ତୁ ସତ୍ୟ; }
    ଶୂନ୍ୟ assign(ଅଚଳ Mat_ପଦସମଷ୍ଟି& ପଦସମଷ୍ଟି, Mat& m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା type=-1) ଅଚଳ CV_OVERRIDE;

    ଶୂନ୍ୟ multiply(ଅଚଳ Mat_ପଦସମଷ୍ଟି& e1, ଦ୍ୱିଗୁଣ s, Mat_ପଦସମଷ୍ଟି& res) ଅଚଳ CV_OVERRIDE;
    ଶୂନ୍ୟ divide(ଦ୍ୱିଗୁଣ s, ଅଚଳ Mat_ପଦସମଷ୍ଟି& e, Mat_ପଦସମଷ୍ଟି& res) ଅଚଳ CV_OVERRIDE;

    ସ୍ଥିର ଶୂନ୍ୟ make_ପଦସମଷ୍ଟି(Mat_ପଦସମଷ୍ଟି& res, ଅକ୍ଷର op, ଅଚଳ Mat& a, ଅଚଳ Mat& b, ଦ୍ୱିଗୁଣ scale=1);
    ସ୍ଥିର ଶୂନ୍ୟ make_ପଦସମଷ୍ଟି(Mat_ପଦସମଷ୍ଟି& res, ଅକ୍ଷର op, ଅଚଳ Mat& a, ଅଚଳ Scalar& s);
};

ସ୍ଥିର MatOp_Bin g_MatOp_Bin;

ଶ୍ରେଣୀ MatOp_Cmp CV_FINAL : ସର୍ବସାଧାରଣ MatOp
{
ସର୍ବସାଧାରଣ:
    MatOp_Cmp() {}
    ଆଭାସୀ ~MatOp_Cmp() {}

    ତାର୍କିକ elementWise(ଅଚଳ Mat_ପଦସମଷ୍ଟି& /*ପଦସମଷ୍ଟି*/) ଅଚଳ CV_OVERRIDE { ଫେରନ୍ତୁ ସତ୍ୟ; }
    ଶୂନ୍ୟ assign(ଅଚଳ Mat_ପଦସମଷ୍ଟି& ପଦସମଷ୍ଟି, Mat& m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା type=-1) ଅଚଳ CV_OVERRIDE;

    ସ୍ଥିର ଶୂନ୍ୟ make_ପଦସମଷ୍ଟି(Mat_ପଦସମଷ୍ଟି& res, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmpop, ଅଚଳ Mat& a, ଅଚଳ Mat& b);
    ସ୍ଥିର ଶୂନ୍ୟ make_ପଦସମଷ୍ଟି(Mat_ପଦସମଷ୍ଟି& res, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmpop, ଅଚଳ Mat& a, ଦ୍ୱିଗୁଣ alpha);
};

ସ୍ଥିର MatOp_Cmp g_MatOp_Cmp;

ଶ୍ରେଣୀ MatOp_GEMM CV_FINAL : ସର୍ବସାଧାରଣ MatOp
{
ସର୍ବସାଧାରଣ:
    MatOp_GEMM() {}
    ଆଭାସୀ ~MatOp_GEMM() {}

    ତାର୍କିକ elementWise(ଅଚଳ Mat_ପଦସମଷ୍ଟି& /*ପଦସମଷ୍ଟି*/) ଅଚଳ CV_OVERRIDE { ଫେରନ୍ତୁ ମିଥ୍ୟା; }
    ଶୂନ୍ୟ assign(ଅଚଳ Mat_ପଦସମଷ୍ଟି& ପଦସମଷ୍ଟି, Mat& m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା type=-1) ଅଚଳ CV_OVERRIDE;

    ଶୂନ୍ୟ add(ଅଚଳ Mat_ପଦସମଷ୍ଟି& e1, ଅଚଳ Mat_ପଦସମଷ୍ଟି& e2, Mat_ପଦସମଷ୍ଟି& res) ଅଚଳ CV_OVERRIDE;
    ଶୂନ୍ୟ subtract(ଅଚଳ Mat_ପଦସମଷ୍ଟି& e1, ଅଚଳ Mat_ପଦସମଷ୍ଟି& e2, Mat_ପଦସମଷ୍ଟି& res) ଅଚଳ CV_OVERRIDE;
    ଶୂନ୍ୟ multiply(ଅଚଳ Mat_ପଦସମଷ୍ଟି& e, ଦ୍ୱିଗୁଣ s, Mat_ପଦସମଷ୍ଟି& res) ଅଚଳ CV_OVERRIDE;

    ଶୂନ୍ୟ transpose(ଅଚଳ Mat_ପଦସମଷ୍ଟି& ପଦସମଷ୍ଟି, Mat_ପଦସମଷ୍ଟି& res) ଅଚଳ CV_OVERRIDE;

    Size size(ଅଚଳ Mat_ପଦସମଷ୍ଟି& ପଦସମଷ୍ଟି) ଅଚଳ CV_OVERRIDE
    {
        ଫେରନ୍ତୁ Size(
            (ପଦସମଷ୍ଟି.flags & GEMM_2_T) ? ପଦସମଷ୍ଟି.b.rows : ପଦସମଷ୍ଟି.b.cols,
            (ପଦସମଷ୍ଟି.flags & GEMM_1_T) ? ପଦସମଷ୍ଟି.a.cols : ପଦସମଷ୍ଟି.a.rows
        );
    }

    ସ୍ଥିର ଶୂନ୍ୟ make_ପଦସମଷ୍ଟି(Mat_ପଦସମଷ୍ଟି& res, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags, ଅଚଳ Mat& a, ଅଚଳ Mat& b,
                         ଦ୍ୱିଗୁଣ alpha=1, ଅଚଳ Mat& c=Mat(), ଦ୍ୱିଗୁଣ beta=1);

};

ସ୍ଥିର MatOp_GEMM g_MatOp_GEMM;

ଶ୍ରେଣୀ MatOp_Invert CV_FINAL : ସର୍ବସାଧାରଣ MatOp
{
ସର୍ବସାଧାରଣ:
    MatOp_Invert() {}
    ଆଭାସୀ ~MatOp_Invert() {}

    ତାର୍କିକ elementWise(ଅଚଳ Mat_ପଦସମଷ୍ଟି& /*ପଦସମଷ୍ଟି*/) ଅଚଳ CV_OVERRIDE { ଫେରନ୍ତୁ ମିଥ୍ୟା; }
    ଶୂନ୍ୟ assign(ଅଚଳ Mat_ପଦସମଷ୍ଟି& ପଦସମଷ୍ଟି, Mat& m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା type=-1) ଅଚଳ CV_OVERRIDE;

    ଶୂନ୍ୟ matmul(ଅଚଳ Mat_ପଦସମଷ୍ଟି& expr1, ଅଚଳ Mat_ପଦସମଷ୍ଟି& expr2, Mat_ପଦସମଷ୍ଟି& res) ଅଚଳ CV_OVERRIDE;

    ସ୍ଥିର ଶୂନ୍ୟ make_ପଦସମଷ୍ଟି(Mat_ପଦସମଷ୍ଟି& res, ପୂର୍ଣ୍ଣସଂଖ୍ୟା method, ଅଚଳ Mat& m);
};

ସ୍ଥିର MatOp_Invert g_MatOp_Invert;

ଶ୍ରେଣୀ MatOp_T CV_FINAL : ସର୍ବସାଧାରଣ MatOp
{
ସର୍ବସାଧାରଣ:
    MatOp_T() {}
    ଆଭାସୀ ~MatOp_T() {}

    ତାର୍କିକ elementWise(ଅଚଳ Mat_ପଦସମଷ୍ଟି& /*ପଦସମଷ୍ଟି*/) ଅଚଳ CV_OVERRIDE { ଫେରନ୍ତୁ ମିଥ୍ୟା; }
    ଶୂନ୍ୟ assign(ଅଚଳ Mat_ପଦସମଷ୍ଟି& ପଦସମଷ୍ଟି, Mat& m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା type=-1) ଅଚଳ CV_OVERRIDE;

    ଶୂନ୍ୟ multiply(ଅଚଳ Mat_ପଦସମଷ୍ଟି& e1, ଦ୍ୱିଗୁଣ s, Mat_ପଦସମଷ୍ଟି& res) ଅଚଳ CV_OVERRIDE;
    ଶୂନ୍ୟ transpose(ଅଚଳ Mat_ପଦସମଷ୍ଟି& ପଦସମଷ୍ଟି, Mat_ପଦସମଷ୍ଟି& res) ଅଚଳ CV_OVERRIDE;

    ସ୍ଥିର ଶୂନ୍ୟ make_ପଦସମଷ୍ଟି(Mat_ପଦସମଷ୍ଟି& res, ଅଚଳ Mat& a, ଦ୍ୱିଗୁଣ alpha=1);
};

ସ୍ଥିର MatOp_T g_MatOp_T;

ଶ୍ରେଣୀ MatOp_Solve CV_FINAL : ସର୍ବସାଧାରଣ MatOp
{
ସର୍ବସାଧାରଣ:
    MatOp_Solve() {}
    ଆଭାସୀ ~MatOp_Solve() {}

    ତାର୍କିକ elementWise(ଅଚଳ Mat_ପଦସମଷ୍ଟି& /*ପଦସମଷ୍ଟି*/) ଅଚଳ CV_OVERRIDE { ଫେରନ୍ତୁ ମିଥ୍ୟା; }
    ଶୂନ୍ୟ assign(ଅଚଳ Mat_ପଦସମଷ୍ଟି& ପଦସମଷ୍ଟି, Mat& m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା type=-1) ଅଚଳ CV_OVERRIDE;

    ସ୍ଥିର ଶୂନ୍ୟ make_ପଦସମଷ୍ଟି(Mat_ପଦସମଷ୍ଟି& res, ପୂର୍ଣ୍ଣସଂଖ୍ୟା method, ଅଚଳ Mat& a, ଅଚଳ Mat& b);
};

ସ୍ଥିର MatOp_Solve g_MatOp_Solve;

ଶ୍ରେଣୀ MatOp_Initializer CV_FINAL : ସର୍ବସାଧାରଣ MatOp
{
ସର୍ବସାଧାରଣ:
    MatOp_Initializer() {}
    ଆଭାସୀ ~MatOp_Initializer() {}

    ତାର୍କିକ elementWise(ଅଚଳ Mat_ପଦସମଷ୍ଟି& /*ପଦସମଷ୍ଟି*/) ଅଚଳ CV_OVERRIDE { ଫେରନ୍ତୁ ମିଥ୍ୟା; }
    ଶୂନ୍ୟ assign(ଅଚଳ Mat_ପଦସମଷ୍ଟି& ପଦସମଷ୍ଟି, Mat& m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା type=-1) ଅଚଳ CV_OVERRIDE;

    ଶୂନ୍ୟ multiply(ଅଚଳ Mat_ପଦସମଷ୍ଟି& e, ଦ୍ୱିଗୁଣ s, Mat_ପଦସମଷ୍ଟି& res) ଅଚଳ CV_OVERRIDE;

    ସ୍ଥିର ଶୂନ୍ୟ make_ପଦସମଷ୍ଟି(Mat_ପଦସମଷ୍ଟି& res, ପୂର୍ଣ୍ଣସଂଖ୍ୟା method, Size sz, ପୂର୍ଣ୍ଣସଂଖ୍ୟା type, ଦ୍ୱିଗୁଣ alpha=1);
    ସ୍ଥିର ଶୂନ୍ୟ make_ପଦସମଷ୍ଟି(Mat_ପଦସମଷ୍ଟି& res, ପୂର୍ଣ୍ଣସଂଖ୍ୟା method, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ndims, ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା* sizes, ପୂର୍ଣ୍ଣସଂଖ୍ୟା type, ଦ୍ୱିଗୁଣ alpha=1);
};

ସ୍ଥିର MatOp_Initializer* getGlobalMatOpInitializer()
{
    CV_SINGLETON_LAZY_INIT(MatOp_Initializer, ନୂତନ MatOp_Initializer())
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ତାର୍କିକ isIdentity(ଅଚଳ Mat_ପଦସମଷ୍ଟି& e) { ଫେରନ୍ତୁ e.op == &g_MatOp_Identity; }
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ତାର୍କିକ isAddEx(ଅଚଳ Mat_ପଦସମଷ୍ଟି& e) { ଫେରନ୍ତୁ e.op == &g_MatOp_AddEx; }
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ତାର୍କିକ isScaled(ଅଚଳ Mat_ପଦସମଷ୍ଟି& e) { ଫେରନ୍ତୁ isAddEx(e) && (!e.b.data || e.beta == 0) && e.s == Scalar(); }
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ତାର୍କିକ isBin(ଅଚଳ Mat_ପଦସମଷ୍ଟି& e, ଅକ୍ଷର c) { ଫେରନ୍ତୁ e.op == &g_MatOp_Bin && e.flags == c; }
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ତାର୍କିକ isCmp(ଅଚଳ Mat_ପଦସମଷ୍ଟି& e) { ଫେରନ୍ତୁ e.op == &g_MatOp_Cmp; }
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ତାର୍କିକ isReciprocal(ଅଚଳ Mat_ପଦସମଷ୍ଟି& e) { ଫେରନ୍ତୁ isBin(e,'/') && (!e.b.data || e.beta == 0); }
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ତାର୍କିକ isT(ଅଚଳ Mat_ପଦସମଷ୍ଟି& e) { ଫେରନ୍ତୁ e.op == &g_MatOp_T; }
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ତାର୍କିକ isInv(ଅଚଳ Mat_ପଦସମଷ୍ଟି& e) { ଫେରନ୍ତୁ e.op == &g_MatOp_Invert; }
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ତାର୍କିକ isSolve(ଅଚଳ Mat_ପଦସମଷ୍ଟି& e) { ଫେରନ୍ତୁ e.op == &g_MatOp_Solve; }
//static inline bool isGEMM(const MatExpr& e) { return e.op == &g_MatOp_GEMM; }
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ତାର୍କିକ isMatProd(ଅଚଳ Mat_ପଦସମଷ୍ଟି& e) { ଫେରନ୍ତୁ e.op == &g_MatOp_GEMM && (!e.c.data || e.beta == 0); }
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ତାର୍କିକ isInitializer(ଅଚଳ Mat_ପଦସମଷ୍ଟି& e) { ଫେରନ୍ତୁ e.op == getGlobalMatOpInitializer(); }

/////////////////////////////////////////////////////////////////////////////////////////////////////

MatOp::MatOp() {}
MatOp::~MatOp() {}


ତାର୍କିକ MatOp::elementWise(ଅଚଳ Mat_ପଦସମଷ୍ଟି& /*ପଦସମଷ୍ଟି*/) ଅଚଳ
{
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
}

ଶୂନ୍ୟ MatOp::roi(ଅଚଳ Mat_ପଦସମଷ୍ଟି& ପଦସମଷ୍ଟି, ଅଚଳ Range& rowRange, ଅଚଳ Range& colRange, Mat_ପଦସମଷ୍ଟି& e) ଅଚଳ
{
    ଯଦି( elementWise(ପଦସମଷ୍ଟି) )
    {
        e = Mat_ପଦସମଷ୍ଟି(ପଦସମଷ୍ଟି.op, ପଦସମଷ୍ଟି.flags, Mat(), Mat(), Mat(),
                    ପଦସମଷ୍ଟି.alpha, ପଦସମଷ୍ଟି.beta, ପଦସମଷ୍ଟି.s);
        ଯଦି(ପଦସମଷ୍ଟି.a.data)
            e.a = ପଦସମଷ୍ଟି.a(rowRange, colRange);
        ଯଦି(ପଦସମଷ୍ଟି.b.data)
            e.b = ପଦସମଷ୍ଟି.b(rowRange, colRange);
        ଯଦି(ପଦସମଷ୍ଟି.c.data)
            e.c = ପଦସମଷ୍ଟି.c(rowRange, colRange);
    }
    ଅନ୍ୟଥା
    {
        Mat m;
        ପଦସମଷ୍ଟି.op->assign(ପଦସମଷ୍ଟି, m);
        e = Mat_ପଦସମଷ୍ଟି(&g_MatOp_Identity, 0, m(rowRange, colRange), Mat(), Mat());
    }
}

ଶୂନ୍ୟ MatOp::diag(ଅଚଳ Mat_ପଦସମଷ୍ଟି& ପଦସମଷ୍ଟି, ପୂର୍ଣ୍ଣସଂଖ୍ୟା d, Mat_ପଦସମଷ୍ଟି& e) ଅଚଳ
{
    ଯଦି( elementWise(ପଦସମଷ୍ଟି) )
    {
        e = Mat_ପଦସମଷ୍ଟି(ପଦସମଷ୍ଟି.op, ପଦସମଷ୍ଟି.flags, Mat(), Mat(), Mat(),
                    ପଦସମଷ୍ଟି.alpha, ପଦସମଷ୍ଟି.beta, ପଦସମଷ୍ଟି.s);
        ଯଦି(ପଦସମଷ୍ଟି.a.data)
            e.a = ପଦସମଷ୍ଟି.a.diag(d);
        ଯଦି(ପଦସମଷ୍ଟି.b.data)
            e.b = ପଦସମଷ୍ଟି.b.diag(d);
        ଯଦି(ପଦସମଷ୍ଟି.c.data)
            e.c = ପଦସମଷ୍ଟି.c.diag(d);
    }
    ଅନ୍ୟଥା
    {
        Mat m;
        ପଦସମଷ୍ଟି.op->assign(ପଦସମଷ୍ଟି, m);
        e = Mat_ପଦସମଷ୍ଟି(&g_MatOp_Identity, 0, m.diag(d), Mat(), Mat());
    }
}


ଶୂନ୍ୟ MatOp::augAssignAdd(ଅଚଳ Mat_ପଦସମଷ୍ଟି& ପଦସମଷ୍ଟି, Mat& m) ଅଚଳ
{
    Mat temp;
    ପଦସମଷ୍ଟି.op->assign(ପଦସମଷ୍ଟି, temp);
    m += temp;
}


ଶୂନ୍ୟ MatOp::augAssignSubtract(ଅଚଳ Mat_ପଦସମଷ୍ଟି& ପଦସମଷ୍ଟି, Mat& m) ଅଚଳ
{
    Mat temp;
    ପଦସମଷ୍ଟି.op->assign(ପଦସମଷ୍ଟି, temp);
    m -= temp;
}


ଶୂନ୍ୟ MatOp::augAssignMultiply(ଅଚଳ Mat_ପଦସମଷ୍ଟି& ପଦସମଷ୍ଟି, Mat& m) ଅଚଳ
{
    Mat temp;
    ପଦସମଷ୍ଟି.op->assign(ପଦସମଷ୍ଟି, temp);
    m *= temp;
}


ଶୂନ୍ୟ MatOp::augAssignDivide(ଅଚଳ Mat_ପଦସମଷ୍ଟି& ପଦସମଷ୍ଟି, Mat& m) ଅଚଳ
{
    Mat temp;
    ପଦସମଷ୍ଟି.op->assign(ପଦସମଷ୍ଟି, temp);
    m /= temp;
}


ଶୂନ୍ୟ MatOp::augAssignAnd(ଅଚଳ Mat_ପଦସମଷ୍ଟି& ପଦସମଷ୍ଟି, Mat& m) ଅଚଳ
{
    Mat temp;
    ପଦସମଷ୍ଟି.op->assign(ପଦସମଷ୍ଟି, temp);
    m &= temp;
}


ଶୂନ୍ୟ MatOp::augAssignOr(ଅଚଳ Mat_ପଦସମଷ୍ଟି& ପଦସମଷ୍ଟି, Mat& m) ଅଚଳ
{
    Mat temp;
    ପଦସମଷ୍ଟି.op->assign(ପଦସମଷ୍ଟି, temp);
    m |= temp;
}


ଶୂନ୍ୟ MatOp::augAssignXor(ଅଚଳ Mat_ପଦସମଷ୍ଟି& ପଦସମଷ୍ଟି, Mat& m) ଅଚଳ
{
    Mat temp;
    ପଦସମଷ୍ଟି.op->assign(ପଦସମଷ୍ଟି, temp);
    m ^= temp;
}


ଶୂନ୍ୟ MatOp::add(ଅଚଳ Mat_ପଦସମଷ୍ଟି& e1, ଅଚଳ Mat_ପଦସମଷ୍ଟି& e2, Mat_ପଦସମଷ୍ଟି& res) ଅଚଳ
{
    CV_INSTRUMENT_REGION();

    ଯଦି( ଏହି == e2.op )
    {
        ଦ୍ୱିଗୁଣ alpha = 1, beta = 1;
        Scalar s;
        Mat m1, m2;
        ଯଦି( isAddEx(e1) && (!e1.b.data || e1.beta == 0) )
        {
            m1 = e1.a;
            alpha = e1.alpha;
            s = e1.s;
        }
        ଅନ୍ୟଥା
            e1.op->assign(e1, m1);

        ଯଦି( isAddEx(e2) && (!e2.b.data || e2.beta == 0) )
        {
            m2 = e2.a;
            beta = e2.alpha;
            s += e2.s;
        }
        ଅନ୍ୟଥା
            e2.op->assign(e2, m2);
        MatOp_AddEx::make_ପଦସମଷ୍ଟି(res, m1, m2, alpha, beta, s);
    }
    ଅନ୍ୟଥା
        e2.op->add(e1, e2, res);
}


ଶୂନ୍ୟ MatOp::add(ଅଚଳ Mat_ପଦସମଷ୍ଟି& expr1, ଅଚଳ Scalar& s, Mat_ପଦସମଷ୍ଟି& res) ଅଚଳ
{
    CV_INSTRUMENT_REGION();

    Mat m1;
    expr1.op->assign(expr1, m1);
    MatOp_AddEx::make_ପଦସମଷ୍ଟି(res, m1, Mat(), 1, 0, s);
}


ଶୂନ୍ୟ MatOp::subtract(ଅଚଳ Mat_ପଦସମଷ୍ଟି& e1, ଅଚଳ Mat_ପଦସମଷ୍ଟି& e2, Mat_ପଦସମଷ୍ଟି& res) ଅଚଳ
{
    CV_INSTRUMENT_REGION();

    ଯଦି( ଏହି == e2.op )
    {
        ଦ୍ୱିଗୁଣ alpha = 1, beta = -1;
        Scalar s;
        Mat m1, m2;
        ଯଦି( isAddEx(e1) && (!e1.b.data || e1.beta == 0) )
        {
            m1 = e1.a;
            alpha = e1.alpha;
            s = e1.s;
        }
        ଅନ୍ୟଥା
            e1.op->assign(e1, m1);

        ଯଦି( isAddEx(e2) && (!e2.b.data || e2.beta == 0) )
        {
            m2 = e2.a;
            beta = -e2.alpha;
            s -= e2.s;
        }
        ଅନ୍ୟଥା
            e2.op->assign(e2, m2);
        MatOp_AddEx::make_ପଦସମଷ୍ଟି(res, m1, m2, alpha, beta, s);
    }
    ଅନ୍ୟଥା
        e2.op->subtract(e1, e2, res);
}


ଶୂନ୍ୟ MatOp::subtract(ଅଚଳ Scalar& s, ଅଚଳ Mat_ପଦସମଷ୍ଟି& ପଦସମଷ୍ଟି, Mat_ପଦସମଷ୍ଟି& res) ଅଚଳ
{
    CV_INSTRUMENT_REGION();

    Mat m;
    ପଦସମଷ୍ଟି.op->assign(ପଦସମଷ୍ଟି, m);
    MatOp_AddEx::make_ପଦସମଷ୍ଟି(res, m, Mat(), -1, 0, s);
}


ଶୂନ୍ୟ MatOp::multiply(ଅଚଳ Mat_ପଦସମଷ୍ଟି& e1, ଅଚଳ Mat_ପଦସମଷ୍ଟି& e2, Mat_ପଦସମଷ୍ଟି& res, ଦ୍ୱିଗୁଣ scale) ଅଚଳ
{
    CV_INSTRUMENT_REGION();

    ଯଦି( ଏହି == e2.op )
    {
        Mat m1, m2;

        ଯଦି( isReciprocal(e1) )
        {
            ଯଦି( isScaled(e2) )
            {
                scale *= e2.alpha;
                m2 = e2.a;
            }
            ଅନ୍ୟଥା
                e2.op->assign(e2, m2);

            MatOp_Bin::make_ପଦସମଷ୍ଟି(res, '/', m2, e1.a, scale/e1.alpha);
        }
        ଅନ୍ୟଥା
        {
            ଅକ୍ଷର op = '*';
            ଯଦି( isScaled(e1) )
            {
                m1 = e1.a;
                scale *= e1.alpha;
            }
            ଅନ୍ୟଥା
                e1.op->assign(e1, m1);

            ଯଦି( isScaled(e2) )
            {
                m2 = e2.a;
                scale *= e2.alpha;
            }
            ଅନ୍ୟଥା ଯଦି( isReciprocal(e2) )
            {
                op = '/';
                m2 = e2.a;
                scale *= e2.alpha;
            }
            ଅନ୍ୟଥା
                e2.op->assign(e2, m2);

            MatOp_Bin::make_ପଦସମଷ୍ଟି(res, op, m1, m2, scale);
        }
    }
    ଅନ୍ୟଥା
        e2.op->multiply(e1, e2, res, scale);
}


ଶୂନ୍ୟ MatOp::multiply(ଅଚଳ Mat_ପଦସମଷ୍ଟି& ପଦସମଷ୍ଟି, ଦ୍ୱିଗୁଣ s, Mat_ପଦସମଷ୍ଟି& res) ଅଚଳ
{
    CV_INSTRUMENT_REGION();

    Mat m;
    ପଦସମଷ୍ଟି.op->assign(ପଦସମଷ୍ଟି, m);
    MatOp_AddEx::make_ପଦସମଷ୍ଟି(res, m, Mat(), s, 0);
}


ଶୂନ୍ୟ MatOp::divide(ଅଚଳ Mat_ପଦସମଷ୍ଟି& e1, ଅଚଳ Mat_ପଦସମଷ୍ଟି& e2, Mat_ପଦସମଷ୍ଟି& res, ଦ୍ୱିଗୁଣ scale) ଅଚଳ
{
    CV_INSTRUMENT_REGION();

    ଯଦି( ଏହି == e2.op )
    {
        ଯଦି( isReciprocal(e1) && isReciprocal(e2) )
            MatOp_Bin::make_ପଦସମଷ୍ଟି(res, '/', e2.a, e1.a, e1.alpha/e2.alpha);
        ଅନ୍ୟଥା
        {
            Mat m1, m2;
            ଅକ୍ଷର op = '/';

            ଯଦି( isScaled(e1) )
            {
                m1 = e1.a;
                scale *= e1.alpha;
            }
            ଅନ୍ୟଥା
                e1.op->assign(e1, m1);

            ଯଦି( isScaled(e2) )
            {
                m2 = e2.a;
                scale /= e2.alpha;
            }
            ଅନ୍ୟଥା ଯଦି( isReciprocal(e2) )
            {
                m2 = e2.a;
                scale /= e2.alpha;
                op = '*';
            }
            ଅନ୍ୟଥା
                e2.op->assign(e2, m2);
            MatOp_Bin::make_ପଦସମଷ୍ଟି(res, op, m1, m2, scale);
        }
    }
    ଅନ୍ୟଥା
        e2.op->divide(e1, e2, res, scale);
}


ଶୂନ୍ୟ MatOp::divide(ଦ୍ୱିଗୁଣ s, ଅଚଳ Mat_ପଦସମଷ୍ଟି& ପଦସମଷ୍ଟି, Mat_ପଦସମଷ୍ଟି& res) ଅଚଳ
{
    CV_INSTRUMENT_REGION();

    Mat m;
    ପଦସମଷ୍ଟି.op->assign(ପଦସମଷ୍ଟି, m);
    MatOp_Bin::make_ପଦସମଷ୍ଟି(res, '/', m, Mat(), s);
}


ଶୂନ୍ୟ MatOp::abs(ଅଚଳ Mat_ପଦସମଷ୍ଟି& ପଦସମଷ୍ଟି, Mat_ପଦସମଷ୍ଟି& res) ଅଚଳ
{
    CV_INSTRUMENT_REGION();

    Mat m;
    ପଦସମଷ୍ଟି.op->assign(ପଦସମଷ୍ଟି, m);
    MatOp_Bin::make_ପଦସମଷ୍ଟି(res, 'a', m, Mat());
}


ଶୂନ୍ୟ MatOp::transpose(ଅଚଳ Mat_ପଦସମଷ୍ଟି& ପଦସମଷ୍ଟି, Mat_ପଦସମଷ୍ଟି& res) ଅଚଳ
{
    CV_INSTRUMENT_REGION();

    Mat m;
    ପଦସମଷ୍ଟି.op->assign(ପଦସମଷ୍ଟି, m);
    MatOp_T::make_ପଦସମଷ୍ଟି(res, m, 1);
}


ଶୂନ୍ୟ MatOp::matmul(ଅଚଳ Mat_ପଦସମଷ୍ଟି& e1, ଅଚଳ Mat_ପଦସମଷ୍ଟି& e2, Mat_ପଦସମଷ୍ଟି& res) ଅଚଳ
{
    ଯଦି( ଏହି == e2.op )
    {
        ଦ୍ୱିଗୁଣ scale = 1;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags = 0;
        Mat m1, m2;

        ଯଦି( isT(e1) )
        {
            flags = cv::GEMM_1_T;
            scale = e1.alpha;
            m1 = e1.a;
        }
        ଅନ୍ୟଥା ଯଦି( isScaled(e1) )
        {
            scale = e1.alpha;
            m1 = e1.a;
        }
        ଅନ୍ୟଥା
            e1.op->assign(e1, m1);

        ଯଦି( isT(e2) )
        {
            flags |= cv::GEMM_2_T;
            scale *= e2.alpha;
            m2 = e2.a;
        }
        ଅନ୍ୟଥା ଯଦି( isScaled(e2) )
        {
            scale *= e2.alpha;
            m2 = e2.a;
        }
        ଅନ୍ୟଥା
            e2.op->assign(e2, m2);

        MatOp_GEMM::make_ପଦସମଷ୍ଟି(res, flags, m1, m2, scale);
    }
    ଅନ୍ୟଥା
        e2.op->matmul(e1, e2, res);
}


ଶୂନ୍ୟ MatOp::invert(ଅଚଳ Mat_ପଦସମଷ୍ଟି& ପଦସମଷ୍ଟି, ପୂର୍ଣ୍ଣସଂଖ୍ୟା method, Mat_ପଦସମଷ୍ଟି& res) ଅଚଳ
{
    Mat m;
    ପଦସମଷ୍ଟି.op->assign(ପଦସମଷ୍ଟି, m);
    MatOp_Invert::make_ପଦସମଷ୍ଟି(res, method, m);
}


Size MatOp::size(ଅଚଳ Mat_ପଦସମଷ୍ଟି& ପଦସମଷ୍ଟି) ଅଚଳ
{
    ଫେରନ୍ତୁ !ପଦସମଷ୍ଟି.a.empty() ? ପଦସମଷ୍ଟି.a.size() : ପଦସମଷ୍ଟି.b.empty() ? ପଦସମଷ୍ଟି.b.size() : ପଦସମଷ୍ଟି.c.size();
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା MatOp::type(ଅଚଳ Mat_ପଦସମଷ୍ଟି& ପଦସମଷ୍ଟି) ଅଚଳ
{
    CV_INSTRUMENT_REGION();

    ଫେରନ୍ତୁ !ପଦସମଷ୍ଟି.a.empty() ? ପଦସମଷ୍ଟି.a.type() : ପଦସମଷ୍ଟି.b.empty() ? ପଦସମଷ୍ଟି.b.type() : ପଦସମଷ୍ଟି.c.type();
}

//////////////////////////////////////////////////////////////////////////////////////////////////

Mat_ପଦସମଷ୍ଟି::Mat_ପଦସମଷ୍ଟି(ଅଚଳ Mat& m) : op(&g_MatOp_Identity), flags(0), a(m), b(Mat()), c(Mat()), alpha(1), beta(0), s(Scalar())
{
}

Mat_ପଦସମଷ୍ଟି Mat_ପଦସମଷ୍ଟି::row(ପୂର୍ଣ୍ଣସଂଖ୍ୟା y) ଅଚଳ
{
    Mat_ପଦସମଷ୍ଟି e;
    op->roi(*ଏହି, Range(y, y+1), Range::all(), e);
    ଫେରନ୍ତୁ e;
}

Mat_ପଦସମଷ୍ଟି Mat_ପଦସମଷ୍ଟି::col(ପୂର୍ଣ୍ଣସଂଖ୍ୟା x) ଅଚଳ
{
    Mat_ପଦସମଷ୍ଟି e;
    op->roi(*ଏହି, Range::all(), Range(x, x+1), e);
    ଫେରନ୍ତୁ e;
}

Mat_ପଦସମଷ୍ଟି Mat_ପଦସମଷ୍ଟି::diag(ପୂର୍ଣ୍ଣସଂଖ୍ୟା d) ଅଚଳ
{
    Mat_ପଦସମଷ୍ଟି e;
    op->diag(*ଏହି, d, e);
    ଫେରନ୍ତୁ e;
}

Mat_ପଦସମଷ୍ଟି Mat_ପଦସମଷ୍ଟି::operator()( ଅଚଳ Range& rowRange, ଅଚଳ Range& colRange ) ଅଚଳ
{
    Mat_ପଦସମଷ୍ଟି e;
    op->roi(*ଏହି, rowRange, colRange, e);
    ଫେରନ୍ତୁ e;
}

Mat_ପଦସମଷ୍ଟି Mat_ପଦସମଷ୍ଟି::operator()( ଅଚଳ Rect& roi ) ଅଚଳ
{
    Mat_ପଦସମଷ୍ଟି e;
    op->roi(*ଏହି, Range(roi.y, roi.y + roi.height), Range(roi.x, roi.x + roi.width), e);
    ଫେରନ୍ତୁ e;
}

Mat Mat_ପଦସମଷ୍ଟି::cross(ଅଚଳ Mat& m) ଅଚଳ
{
    ଫେରନ୍ତୁ ((Mat)*ଏହି).cross(m);
}

ଦ୍ୱିଗୁଣ Mat_ପଦସମଷ୍ଟି::dot(ଅଚଳ Mat& m) ଅଚଳ
{
    ଫେରନ୍ତୁ ((Mat)*ଏହି).dot(m);
}

Mat_ପଦସମଷ୍ଟି Mat_ପଦସମଷ୍ଟି::t() ଅଚଳ
{
    Mat_ପଦସମଷ୍ଟି e;
    op->transpose(*ଏହି, e);
    ଫେରନ୍ତୁ e;
}

Mat_ପଦସମଷ୍ଟି Mat_ପଦସମଷ୍ଟି::inv(ପୂର୍ଣ୍ଣସଂଖ୍ୟା method) ଅଚଳ
{
    Mat_ପଦସମଷ୍ଟି e;
    op->invert(*ଏହି, method, e);
    ଫେରନ୍ତୁ e;
}

Mat_ପଦସମଷ୍ଟି Mat_ପଦସମଷ୍ଟି::mul(ଅଚଳ Mat_ପଦସମଷ୍ଟି& e, ଦ୍ୱିଗୁଣ scale) ଅଚଳ
{
    Mat_ପଦସମଷ୍ଟି en;
    op->multiply(*ଏହି, e, en, scale);
    ଫେରନ୍ତୁ en;
}

Mat_ପଦସମଷ୍ଟି Mat_ପଦସମଷ୍ଟି::mul(ଅଚଳ Mat& m, ଦ୍ୱିଗୁଣ scale) ଅଚଳ
{
    Mat_ପଦସମଷ୍ଟି e;
    op->multiply(*ଏହି, Mat_ପଦସମଷ୍ଟି(m), e, scale);
    ଫେରନ୍ତୁ e;
}

Mat_ପଦସମଷ୍ଟି operator + (ଅଚଳ Mat& a, ଅଚଳ Mat& b)
{
    checkOperandsExist(a, b);
    Mat_ପଦସମଷ୍ଟି e;
    MatOp_AddEx::make_ପଦସମଷ୍ଟି(e, a, b, 1, 1);
    ଫେରନ୍ତୁ e;
}

Mat_ପଦସମଷ୍ଟି operator + (ଅଚଳ Mat& a, ଅଚଳ Scalar& s)
{
    checkOperandsExist(a);
    Mat_ପଦସମଷ୍ଟି e;
    MatOp_AddEx::make_ପଦସମଷ୍ଟି(e, a, Mat(), 1, 0, s);
    ଫେରନ୍ତୁ e;
}

Mat_ପଦସମଷ୍ଟି operator + (ଅଚଳ Scalar& s, ଅଚଳ Mat& a)
{
    checkOperandsExist(a);
    Mat_ପଦସମଷ୍ଟି e;
    MatOp_AddEx::make_ପଦସମଷ୍ଟି(e, a, Mat(), 1, 0, s);
    ଫେରନ୍ତୁ e;
}

Mat_ପଦସମଷ୍ଟି operator + (ଅଚଳ Mat_ପଦସମଷ୍ଟି& e, ଅଚଳ Mat& m)
{
    Mat_ପଦସମଷ୍ଟି en;
    e.op->add(e, Mat_ପଦସମଷ୍ଟି(m), en);
    ଫେରନ୍ତୁ en;
}

Mat_ପଦସମଷ୍ଟି operator + (ଅଚଳ Mat& m, ଅଚଳ Mat_ପଦସମଷ୍ଟି& e)
{
    checkOperandsExist(m);
    Mat_ପଦସମଷ୍ଟି en;
    e.op->add(e, Mat_ପଦସମଷ୍ଟି(m), en);
    ଫେରନ୍ତୁ en;
}

Mat_ପଦସମଷ୍ଟି operator + (ଅଚଳ Mat_ପଦସମଷ୍ଟି& e, ଅଚଳ Scalar& s)
{
    Mat_ପଦସମଷ୍ଟି en;
    e.op->add(e, s, en);
    ଫେରନ୍ତୁ en;
}

Mat_ପଦସମଷ୍ଟି operator + (ଅଚଳ Scalar& s, ଅଚଳ Mat_ପଦସମଷ୍ଟି& e)
{
    Mat_ପଦସମଷ୍ଟି en;
    e.op->add(e, s, en);
    ଫେରନ୍ତୁ en;
}

Mat_ପଦସମଷ୍ଟି operator + (ଅଚଳ Mat_ପଦସମଷ୍ଟି& e1, ଅଚଳ Mat_ପଦସମଷ୍ଟି& e2)
{
    Mat_ପଦସମଷ୍ଟି en;
    e1.op->add(e1, e2, en);
    ଫେରନ୍ତୁ en;
}

Mat_ପଦସମଷ୍ଟି operator - (ଅଚଳ Mat& a, ଅଚଳ Mat& b)
{
    checkOperandsExist(a, b);
    Mat_ପଦସମଷ୍ଟି e;
    MatOp_AddEx::make_ପଦସମଷ୍ଟି(e, a, b, 1, -1);
    ଫେରନ୍ତୁ e;
}

Mat_ପଦସମଷ୍ଟି operator - (ଅଚଳ Mat& a, ଅଚଳ Scalar& s)
{
    checkOperandsExist(a);
    Mat_ପଦସମଷ୍ଟି e;
    MatOp_AddEx::make_ପଦସମଷ୍ଟି(e, a, Mat(), 1, 0, -s);
    ଫେରନ୍ତୁ e;
}

Mat_ପଦସମଷ୍ଟି operator - (ଅଚଳ Scalar& s, ଅଚଳ Mat& a)
{
    checkOperandsExist(a);
    Mat_ପଦସମଷ୍ଟି e;
    MatOp_AddEx::make_ପଦସମଷ୍ଟି(e, a, Mat(), -1, 0, s);
    ଫେରନ୍ତୁ e;
}

Mat_ପଦସମଷ୍ଟି operator - (ଅଚଳ Mat_ପଦସମଷ୍ଟି& e, ଅଚଳ Mat& m)
{
    checkOperandsExist(m);
    Mat_ପଦସମଷ୍ଟି en;
    e.op->subtract(e, Mat_ପଦସମଷ୍ଟି(m), en);
    ଫେରନ୍ତୁ en;
}

Mat_ପଦସମଷ୍ଟି operator - (ଅଚଳ Mat& m, ଅଚଳ Mat_ପଦସମଷ୍ଟି& e)
{
    checkOperandsExist(m);
    Mat_ପଦସମଷ୍ଟି en;
    e.op->subtract(Mat_ପଦସମଷ୍ଟି(m), e, en);
    ଫେରନ୍ତୁ en;
}

Mat_ପଦସମଷ୍ଟି operator - (ଅଚଳ Mat_ପଦସମଷ୍ଟି& e, ଅଚଳ Scalar& s)
{
    Mat_ପଦସମଷ୍ଟି en;
    e.op->add(e, -s, en);
    ଫେରନ୍ତୁ en;
}

Mat_ପଦସମଷ୍ଟି operator - (ଅଚଳ Scalar& s, ଅଚଳ Mat_ପଦସମଷ୍ଟି& e)
{
    Mat_ପଦସମଷ୍ଟି en;
    e.op->subtract(s, e, en);
    ଫେରନ୍ତୁ en;
}

Mat_ପଦସମଷ୍ଟି operator - (ଅଚଳ Mat_ପଦସମଷ୍ଟି& e1, ଅଚଳ Mat_ପଦସମଷ୍ଟି& e2)
{
    Mat_ପଦସମଷ୍ଟି en;
    e1.op->subtract(e1, e2, en);
    ଫେରନ୍ତୁ en;
}

Mat_ପଦସମଷ୍ଟି operator - (ଅଚଳ Mat& m)
{
    checkOperandsExist(m);
    Mat_ପଦସମଷ୍ଟି e;
    MatOp_AddEx::make_ପଦସମଷ୍ଟି(e, m, Mat(), -1, 0);
    ଫେରନ୍ତୁ e;
}

Mat_ପଦସମଷ୍ଟି operator - (ଅଚଳ Mat_ପଦସମଷ୍ଟି& e)
{
    Mat_ପଦସମଷ୍ଟି en;
    e.op->subtract(Scalar(0), e, en);
    ଫେରନ୍ତୁ en;
}

Mat_ପଦସମଷ୍ଟି operator * (ଅଚଳ Mat& a, ଅଚଳ Mat& b)
{
    checkOperandsExist(a, b);
    Mat_ପଦସମଷ୍ଟି e;
    MatOp_GEMM::make_ପଦସମଷ୍ଟି(e, 0, a, b);
    ଫେରନ୍ତୁ e;
}

Mat_ପଦସମଷ୍ଟି operator * (ଅଚଳ Mat& a, ଦ୍ୱିଗୁଣ s)
{
    checkOperandsExist(a);
    Mat_ପଦସମଷ୍ଟି e;
    MatOp_AddEx::make_ପଦସମଷ୍ଟି(e, a, Mat(), s, 0);
    ଫେରନ୍ତୁ e;
}

Mat_ପଦସମଷ୍ଟି operator * (ଦ୍ୱିଗୁଣ s, ଅଚଳ Mat& a)
{
    checkOperandsExist(a);
    Mat_ପଦସମଷ୍ଟି e;
    MatOp_AddEx::make_ପଦସମଷ୍ଟି(e, a, Mat(), s, 0);
    ଫେରନ୍ତୁ e;
}

Mat_ପଦସମଷ୍ଟି operator * (ଅଚଳ Mat_ପଦସମଷ୍ଟି& e, ଅଚଳ Mat& m)
{
    Mat_ପଦସମଷ୍ଟି en;
    e.op->matmul(e, Mat_ପଦସମଷ୍ଟି(m), en);
    ଫେରନ୍ତୁ en;
}

Mat_ପଦସମଷ୍ଟି operator * (ଅଚଳ Mat& m, ଅଚଳ Mat_ପଦସମଷ୍ଟି& e)
{
    checkOperandsExist(m);
    Mat_ପଦସମଷ୍ଟି en;
    e.op->matmul(Mat_ପଦସମଷ୍ଟି(m), e, en);
    ଫେରନ୍ତୁ en;
}

Mat_ପଦସମଷ୍ଟି operator * (ଅଚଳ Mat_ପଦସମଷ୍ଟି& e, ଦ୍ୱିଗୁଣ s)
{
    Mat_ପଦସମଷ୍ଟି en;
    e.op->multiply(e, s, en);
    ଫେରନ୍ତୁ en;
}

Mat_ପଦସମଷ୍ଟି operator * (ଦ୍ୱିଗୁଣ s, ଅଚଳ Mat_ପଦସମଷ୍ଟି& e)
{
    Mat_ପଦସମଷ୍ଟି en;
    e.op->multiply(e, s, en);
    ଫେରନ୍ତୁ en;
}

Mat_ପଦସମଷ୍ଟି operator * (ଅଚଳ Mat_ପଦସମଷ୍ଟି& e1, ଅଚଳ Mat_ପଦସମଷ୍ଟି& e2)
{
    Mat_ପଦସମଷ୍ଟି en;
    e1.op->matmul(e1, e2, en);
    ଫେରନ୍ତୁ en;
}

Mat_ପଦସମଷ୍ଟି operator / (ଅଚଳ Mat& a, ଅଚଳ Mat& b)
{
    checkOperandsExist(a, b);
    Mat_ପଦସମଷ୍ଟି e;
    MatOp_Bin::make_ପଦସମଷ୍ଟି(e, '/', a, b);
    ଫେରନ୍ତୁ e;
}

Mat_ପଦସମଷ୍ଟି operator / (ଅଚଳ Mat& a, ଦ୍ୱିଗୁଣ s)
{
    checkOperandsExist(a);
    Mat_ପଦସମଷ୍ଟି e;
    MatOp_AddEx::make_ପଦସମଷ୍ଟି(e, a, Mat(), 1./s, 0);
    ଫେରନ୍ତୁ e;
}

Mat_ପଦସମଷ୍ଟି operator / (ଦ୍ୱିଗୁଣ s, ଅଚଳ Mat& a)
{
    checkOperandsExist(a);
    Mat_ପଦସମଷ୍ଟି e;
    MatOp_Bin::make_ପଦସମଷ୍ଟି(e, '/', a, Mat(), s);
    ଫେରନ୍ତୁ e;
}

Mat_ପଦସମଷ୍ଟି operator / (ଅଚଳ Mat_ପଦସମଷ୍ଟି& e, ଅଚଳ Mat& m)
{
    Mat_ପଦସମଷ୍ଟି en;
    e.op->divide(e, Mat_ପଦସମଷ୍ଟି(m), en);
    ଫେରନ୍ତୁ en;
}

Mat_ପଦସମଷ୍ଟି operator / (ଅଚଳ Mat& m, ଅଚଳ Mat_ପଦସମଷ୍ଟି& e)
{
    checkOperandsExist(m);
    Mat_ପଦସମଷ୍ଟି en;
    e.op->divide(Mat_ପଦସମଷ୍ଟି(m), e, en);
    ଫେରନ୍ତୁ en;
}

Mat_ପଦସମଷ୍ଟି operator / (ଅଚଳ Mat_ପଦସମଷ୍ଟି& e, ଦ୍ୱିଗୁଣ s)
{
    Mat_ପଦସମଷ୍ଟି en;
    e.op->multiply(e, 1./s, en);
    ଫେରନ୍ତୁ en;
}

Mat_ପଦସମଷ୍ଟି operator / (ଦ୍ୱିଗୁଣ s, ଅଚଳ Mat_ପଦସମଷ୍ଟି& e)
{
    Mat_ପଦସମଷ୍ଟି en;
    e.op->divide(s, e, en);
    ଫେରନ୍ତୁ en;
}

Mat_ପଦସମଷ୍ଟି operator / (ଅଚଳ Mat_ପଦସମଷ୍ଟି& e1, ଅଚଳ Mat_ପଦସମଷ୍ଟି& e2)
{
    Mat_ପଦସମଷ୍ଟି en;
    e1.op->divide(e1, e2, en);
    ଫେରନ୍ତୁ en;
}

Mat_ପଦସମଷ୍ଟି operator < (ଅଚଳ Mat& a, ଅଚଳ Mat& b)
{
    checkOperandsExist(a, b);
    Mat_ପଦସମଷ୍ଟି e;
    MatOp_Cmp::make_ପଦସମଷ୍ଟି(e, cv::CMP_LT, a, b);
    ଫେରନ୍ତୁ e;
}

Mat_ପଦସମଷ୍ଟି operator < (ଅଚଳ Mat& a, ଦ୍ୱିଗୁଣ s)
{
    checkOperandsExist(a);
    Mat_ପଦସମଷ୍ଟି e;
    MatOp_Cmp::make_ପଦସମଷ୍ଟି(e, cv::CMP_LT, a, s);
    ଫେରନ୍ତୁ e;
}

Mat_ପଦସମଷ୍ଟି operator < (ଦ୍ୱିଗୁଣ s, ଅଚଳ Mat& a)
{
    checkOperandsExist(a);
    Mat_ପଦସମଷ୍ଟି e;
    MatOp_Cmp::make_ପଦସମଷ୍ଟି(e, cv::CMP_GT, a, s);
    ଫେରନ୍ତୁ e;
}

Mat_ପଦସମଷ୍ଟି operator <= (ଅଚଳ Mat& a, ଅଚଳ Mat& b)
{
    checkOperandsExist(a, b);
    Mat_ପଦସମଷ୍ଟି e;
    MatOp_Cmp::make_ପଦସମଷ୍ଟି(e, cv::CMP_LE, a, b);
    ଫେରନ୍ତୁ e;
}

Mat_ପଦସମଷ୍ଟି operator <= (ଅଚଳ Mat& a, ଦ୍ୱିଗୁଣ s)
{
    checkOperandsExist(a);
    Mat_ପଦସମଷ୍ଟି e;
    MatOp_Cmp::make_ପଦସମଷ୍ଟି(e, cv::CMP_LE, a, s);
    ଫେରନ୍ତୁ e;
}

Mat_ପଦସମଷ୍ଟି operator <= (ଦ୍ୱିଗୁଣ s, ଅଚଳ Mat& a)
{
    checkOperandsExist(a);
    Mat_ପଦସମଷ୍ଟି e;
    MatOp_Cmp::make_ପଦସମଷ୍ଟି(e, cv::CMP_GE, a, s);
    ଫେରନ୍ତୁ e;
}

Mat_ପଦସମଷ୍ଟି operator == (ଅଚଳ Mat& a, ଅଚଳ Mat& b)
{
    checkOperandsExist(a, b);
    Mat_ପଦସମଷ୍ଟି e;
    MatOp_Cmp::make_ପଦସମଷ୍ଟି(e, cv::CMP_EQ, a, b);
    ଫେରନ୍ତୁ e;
}

Mat_ପଦସମଷ୍ଟି operator == (ଅଚଳ Mat& a, ଦ୍ୱିଗୁଣ s)
{
    checkOperandsExist(a);
    Mat_ପଦସମଷ୍ଟି e;
    MatOp_Cmp::make_ପଦସମଷ୍ଟି(e, cv::CMP_EQ, a, s);
    ଫେରନ୍ତୁ e;
}

Mat_ପଦସମଷ୍ଟି operator == (ଦ୍ୱିଗୁଣ s, ଅଚଳ Mat& a)
{
    checkOperandsExist(a);
    Mat_ପଦସମଷ୍ଟି e;
    MatOp_Cmp::make_ପଦସମଷ୍ଟି(e, cv::CMP_EQ, a, s);
    ଫେରନ୍ତୁ e;
}

Mat_ପଦସମଷ୍ଟି operator != (ଅଚଳ Mat& a, ଅଚଳ Mat& b)
{
    checkOperandsExist(a, b);
    Mat_ପଦସମଷ୍ଟି e;
    MatOp_Cmp::make_ପଦସମଷ୍ଟି(e, cv::CMP_NE, a, b);
    ଫେରନ୍ତୁ e;
}

Mat_ପଦସମଷ୍ଟି operator != (ଅଚଳ Mat& a, ଦ୍ୱିଗୁଣ s)
{
    checkOperandsExist(a);
    Mat_ପଦସମଷ୍ଟି e;
    MatOp_Cmp::make_ପଦସମଷ୍ଟି(e, cv::CMP_NE, a, s);
    ଫେରନ୍ତୁ e;
}

Mat_ପଦସମଷ୍ଟି operator != (ଦ୍ୱିଗୁଣ s, ଅଚଳ Mat& a)
{
    checkOperandsExist(a);
    Mat_ପଦସମଷ୍ଟି e;
    MatOp_Cmp::make_ପଦସମଷ୍ଟି(e, cv::CMP_NE, a, s);
    ଫେରନ୍ତୁ e;
}

Mat_ପଦସମଷ୍ଟି operator >= (ଅଚଳ Mat& a, ଅଚଳ Mat& b)
{
    checkOperandsExist(a, b);
    Mat_ପଦସମଷ୍ଟି e;
    MatOp_Cmp::make_ପଦସମଷ୍ଟି(e, cv::CMP_GE, a, b);
    ଫେରନ୍ତୁ e;
}

Mat_ପଦସମଷ୍ଟି operator >= (ଅଚଳ Mat& a, ଦ୍ୱିଗୁଣ s)
{
    checkOperandsExist(a);
    Mat_ପଦସମଷ୍ଟି e;
    MatOp_Cmp::make_ପଦସମଷ୍ଟି(e, cv::CMP_GE, a, s);
    ଫେରନ୍ତୁ e;
}

Mat_ପଦସମଷ୍ଟି operator >= (ଦ୍ୱିଗୁଣ s, ଅଚଳ Mat& a)
{
    checkOperandsExist(a);
    Mat_ପଦସମଷ୍ଟି e;
    MatOp_Cmp::make_ପଦସମଷ୍ଟି(e, cv::CMP_LE, a, s);
    ଫେରନ୍ତୁ e;
}

Mat_ପଦସମଷ୍ଟି operator > (ଅଚଳ Mat& a, ଅଚଳ Mat& b)
{
    checkOperandsExist(a, b);
    Mat_ପଦସମଷ୍ଟି e;
    MatOp_Cmp::make_ପଦସମଷ୍ଟି(e, cv::CMP_GT, a, b);
    ଫେରନ୍ତୁ e;
}

Mat_ପଦସମଷ୍ଟି operator > (ଅଚଳ Mat& a, ଦ୍ୱିଗୁଣ s)
{
    checkOperandsExist(a);
    Mat_ପଦସମଷ୍ଟି e;
    MatOp_Cmp::make_ପଦସମଷ୍ଟି(e, cv::CMP_GT, a, s);
    ଫେରନ୍ତୁ e;
}

Mat_ପଦସମଷ୍ଟି operator > (ଦ୍ୱିଗୁଣ s, ଅଚଳ Mat& a)
{
    checkOperandsExist(a);
    Mat_ପଦସମଷ୍ଟି e;
    MatOp_Cmp::make_ପଦସମଷ୍ଟି(e, cv::CMP_LT, a, s);
    ଫେରନ୍ତୁ e;
}

Mat_ପଦସମଷ୍ଟି min(ଅଚଳ Mat& a, ଅଚଳ Mat& b)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a, b);
    Mat_ପଦସମଷ୍ଟି e;
    MatOp_Bin::make_ପଦସମଷ୍ଟି(e, 'm', a, b);
    ଫେରନ୍ତୁ e;
}

Mat_ପଦସମଷ୍ଟି min(ଅଚଳ Mat& a, ଦ୍ୱିଗୁଣ s)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    Mat_ପଦସମଷ୍ଟି e;
    MatOp_Bin::make_ପଦସମଷ୍ଟି(e, 'n', a, s);
    ଫେରନ୍ତୁ e;
}

Mat_ପଦସମଷ୍ଟି min(ଦ୍ୱିଗୁଣ s, ଅଚଳ Mat& a)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    Mat_ପଦସମଷ୍ଟି e;
    MatOp_Bin::make_ପଦସମଷ୍ଟି(e, 'n', a, s);
    ଫେରନ୍ତୁ e;
}

Mat_ପଦସମଷ୍ଟି max(ଅଚଳ Mat& a, ଅଚଳ Mat& b)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a, b);
    Mat_ପଦସମଷ୍ଟି e;
    MatOp_Bin::make_ପଦସମଷ୍ଟି(e, 'M', a, b);
    ଫେରନ୍ତୁ e;
}

Mat_ପଦସମଷ୍ଟି max(ଅଚଳ Mat& a, ଦ୍ୱିଗୁଣ s)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    Mat_ପଦସମଷ୍ଟି e;
    MatOp_Bin::make_ପଦସମଷ୍ଟି(e, 'N', a, s);
    ଫେରନ୍ତୁ e;
}

Mat_ପଦସମଷ୍ଟି max(ଦ୍ୱିଗୁଣ s, ଅଚଳ Mat& a)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    Mat_ପଦସମଷ୍ଟି e;
    MatOp_Bin::make_ପଦସମଷ୍ଟି(e, 'N', a, s);
    ଫେରନ୍ତୁ e;
}

Mat_ପଦସମଷ୍ଟି operator & (ଅଚଳ Mat& a, ଅଚଳ Mat& b)
{
    checkOperandsExist(a, b);
    Mat_ପଦସମଷ୍ଟି e;
    MatOp_Bin::make_ପଦସମଷ୍ଟି(e, '&', a, b);
    ଫେରନ୍ତୁ e;
}

Mat_ପଦସମଷ୍ଟି operator & (ଅଚଳ Mat& a, ଅଚଳ Scalar& s)
{
    checkOperandsExist(a);
    Mat_ପଦସମଷ୍ଟି e;
    MatOp_Bin::make_ପଦସମଷ୍ଟି(e, '&', a, s);
    ଫେରନ୍ତୁ e;
}

Mat_ପଦସମଷ୍ଟି operator & (ଅଚଳ Scalar& s, ଅଚଳ Mat& a)
{
    checkOperandsExist(a);
    Mat_ପଦସମଷ୍ଟି e;
    MatOp_Bin::make_ପଦସମଷ୍ଟି(e, '&', a, s);
    ଫେରନ୍ତୁ e;
}

Mat_ପଦସମଷ୍ଟି operator | (ଅଚଳ Mat& a, ଅଚଳ Mat& b)
{
    checkOperandsExist(a, b);
    Mat_ପଦସମଷ୍ଟି e;
    MatOp_Bin::make_ପଦସମଷ୍ଟି(e, '|', a, b);
    ଫେରନ୍ତୁ e;
}

Mat_ପଦସମଷ୍ଟି operator | (ଅଚଳ Mat& a, ଅଚଳ Scalar& s)
{
    checkOperandsExist(a);
    Mat_ପଦସମଷ୍ଟି e;
    MatOp_Bin::make_ପଦସମଷ୍ଟି(e, '|', a, s);
    ଫେରନ୍ତୁ e;
}

Mat_ପଦସମଷ୍ଟି operator | (ଅଚଳ Scalar& s, ଅଚଳ Mat& a)
{
    checkOperandsExist(a);
    Mat_ପଦସମଷ୍ଟି e;
    MatOp_Bin::make_ପଦସମଷ୍ଟି(e, '|', a, s);
    ଫେରନ୍ତୁ e;
}

Mat_ପଦସମଷ୍ଟି operator ^ (ଅଚଳ Mat& a, ଅଚଳ Mat& b)
{
    checkOperandsExist(a, b);
    Mat_ପଦସମଷ୍ଟି e;
    MatOp_Bin::make_ପଦସମଷ୍ଟି(e, '^', a, b);
    ଫେରନ୍ତୁ e;
}

Mat_ପଦସମଷ୍ଟି operator ^ (ଅଚଳ Mat& a, ଅଚଳ Scalar& s)
{
    checkOperandsExist(a);
    Mat_ପଦସମଷ୍ଟି e;
    MatOp_Bin::make_ପଦସମଷ୍ଟି(e, '^', a, s);
    ଫେରନ୍ତୁ e;
}

Mat_ପଦସମଷ୍ଟି operator ^ (ଅଚଳ Scalar& s, ଅଚଳ Mat& a)
{
    checkOperandsExist(a);
    Mat_ପଦସମଷ୍ଟି e;
    MatOp_Bin::make_ପଦସମଷ୍ଟି(e, '^', a, s);
    ଫେରନ୍ତୁ e;
}

Mat_ପଦସମଷ୍ଟି operator ~(ଅଚଳ Mat& a)
{
    checkOperandsExist(a);
    Mat_ପଦସମଷ୍ଟି e;
    MatOp_Bin::make_ପଦସମଷ୍ଟି(e, '~', a, Scalar());
    ଫେରନ୍ତୁ e;
}

Mat_ପଦସମଷ୍ଟି abs(ଅଚଳ Mat& a)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    Mat_ପଦସମଷ୍ଟି e;
    MatOp_Bin::make_ପଦସମଷ୍ଟି(e, 'a', a, Scalar());
    ଫେରନ୍ତୁ e;
}

Mat_ପଦସମଷ୍ଟି abs(ଅଚଳ Mat_ପଦସମଷ୍ଟି& e)
{
    CV_INSTRUMENT_REGION();

    Mat_ପଦସମଷ୍ଟି en;
    e.op->abs(e, en);
    ଫେରନ୍ତୁ en;
}


Size Mat_ପଦସମଷ୍ଟି::size() ଅଚଳ
{
    ଯଦି( isT(*ଏହି) || isInv(*ଏହି) )
        ଫେରନ୍ତୁ Size(a.rows, a.cols);
    ଯଦି( isSolve(*ଏହି) )
        ଫେରନ୍ତୁ Size(b.cols, a.cols);
    ଯଦି( isInitializer(*ଏହି) )
        ଫେରନ୍ତୁ a.size();
    ଫେରନ୍ତୁ op ? op->size(*ଏହି) : Size();
}


ପୂର୍ଣ୍ଣସଂଖ୍ୟା Mat_ପଦସମଷ୍ଟି::type() ଅଚଳ
{
    CV_INSTRUMENT_REGION();

    ଯଦି( isInitializer(*ଏହି) )
        ଫେରନ୍ତୁ a.type();
    ଯଦି( isCmp(*ଏହି) )
        ଫେରନ୍ତୁ CV_MAKETYPE(CV_8U, a.channels());
    ଫେରନ୍ତୁ op ? op->type(*ଏହି) : -1;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////

ଶୂନ୍ୟ MatOp_Identity::assign(ଅଚଳ Mat_ପଦସମଷ୍ଟି& e, Mat& m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା _type) ଅଚଳ
{
    ଯଦି( _type == -1 || _type == e.a.type() )
        m = e.a;
    ଅନ୍ୟଥା
    {
        CV_Assert( CV_MAT_CN(_type) == e.a.channels() );
        e.a.convertTo(m, _type);
    }
}

ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ MatOp_Identity::make_ପଦସମଷ୍ଟି(Mat_ପଦସମଷ୍ଟି& res, ଅଚଳ Mat& m)
{
    res = Mat_ପଦସମଷ୍ଟି(&g_MatOp_Identity, 0, m, Mat(), Mat(), 1, 0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

ଶୂନ୍ୟ MatOp_AddEx::assign(ଅଚଳ Mat_ପଦସମଷ୍ଟି& e, Mat& m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା _type) ଅଚଳ
{
    Mat temp, &dst = _type == -1 || e.a.type() == _type ? m : temp;
    ଯଦି( e.b.data )
    {
        ଯଦି( e.s == Scalar() || !e.s.isReal() )
        {
            ଯଦି( e.alpha == 1 )
            {
                ଯଦି( e.beta == 1 )
                    cv::add(e.a, e.b, dst);
                ଅନ୍ୟଥା ଯଦି( e.beta == -1 )
                    cv::subtract(e.a, e.b, dst);
                ଅନ୍ୟଥା
                    cv::scaleAdd(e.b, e.beta, e.a, dst);
            }
            ଅନ୍ୟଥା ଯଦି( e.beta == 1 )
            {
                ଯଦି( e.alpha == -1 )
                    cv::subtract(e.b, e.a, dst);
                ଅନ୍ୟଥା
                    cv::scaleAdd(e.a, e.alpha, e.b, dst);
            }
            ଅନ୍ୟଥା
                cv::addWeighted(e.a, e.alpha, e.b, e.beta, 0, dst);

            ଯଦି( !e.s.isReal() )
                cv::add(dst, e.s, dst);
        }
        ଅନ୍ୟଥା
        {
            ଯଦି (e.a.channels() > 1)
                CV_LOG_ONCE_WARNING(ଖାଲି, "OpenCV/MatExpr: processing of multi-channel arrays might be changed in the future: "
                                          "https://github.com/opencv/opencv/issues/16739");
            cv::addWeighted(e.a, e.alpha, e.b, e.beta, e.s[0], dst);
        }
    }
    ଅନ୍ୟଥା ଯଦି( e.s.isReal() && (dst.data != m.data || fabs(e.alpha) != 1))
    {
        ଯଦି (e.a.channels() > 1 && e.s[0] != 0.0)
            CV_LOG_ONCE_WARNING(ଖାଲି, "OpenCV/MatExpr: processing of multi-channel arrays might be changed in the future: "
                                      "https://github.com/opencv/opencv/issues/16739");
        e.a.convertTo(m, _type, e.alpha, e.s[0]);
        ଫେରନ୍ତୁ;
    }
    ଅନ୍ୟଥା ଯଦି( e.alpha == 1 )
        cv::add(e.a, e.s, dst);
    ଅନ୍ୟଥା ଯଦି( e.alpha == -1 )
        cv::subtract(e.s, e.a, dst);
    ଅନ୍ୟଥା
    {
        e.a.convertTo(dst, e.a.type(), e.alpha);
        cv::add(dst, e.s, dst);
    }

    ଯଦି( dst.data != m.data )
        dst.convertTo(m, m.type());
}


ଶୂନ୍ୟ MatOp_AddEx::add(ଅଚଳ Mat_ପଦସମଷ୍ଟି& e, ଅଚଳ Scalar& s, Mat_ପଦସମଷ୍ଟି& res) ଅଚଳ
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.s += s;
}


ଶୂନ୍ୟ MatOp_AddEx::subtract(ଅଚଳ Scalar& s, ଅଚଳ Mat_ପଦସମଷ୍ଟି& e, Mat_ପଦସମଷ୍ଟି& res) ଅଚଳ
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha = -res.alpha;
    res.beta = -res.beta;
    res.s = s - res.s;
}

ଶୂନ୍ୟ MatOp_AddEx::multiply(ଅଚଳ Mat_ପଦସମଷ୍ଟି& e, ଦ୍ୱିଗୁଣ s, Mat_ପଦସମଷ୍ଟି& res) ଅଚଳ
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha *= s;
    res.beta *= s;
    res.s *= s;
}

ଶୂନ୍ୟ MatOp_AddEx::divide(ଦ୍ୱିଗୁଣ s, ଅଚଳ Mat_ପଦସମଷ୍ଟି& e, Mat_ପଦସମଷ୍ଟି& res) ଅଚଳ
{
    CV_INSTRUMENT_REGION();

    ଯଦି( isScaled(e) )
        MatOp_Bin::make_ପଦସମଷ୍ଟି(res, '/', e.a, Mat(), s/e.alpha);
    ଅନ୍ୟଥା
        MatOp::divide(s, e, res);
}


ଶୂନ୍ୟ MatOp_AddEx::transpose(ଅଚଳ Mat_ପଦସମଷ୍ଟି& e, Mat_ପଦସମଷ୍ଟି& res) ଅଚଳ
{
    CV_INSTRUMENT_REGION();

    ଯଦି( isScaled(e) )
        MatOp_T::make_ପଦସମଷ୍ଟି(res, e.a, e.alpha);
    ଅନ୍ୟଥା
        MatOp::transpose(e, res);
}

ଶୂନ୍ୟ MatOp_AddEx::abs(ଅଚଳ Mat_ପଦସମଷ୍ଟି& e, Mat_ପଦସମଷ୍ଟି& res) ଅଚଳ
{
    CV_INSTRUMENT_REGION();

    ଯଦି( (!e.b.data || e.beta == 0) && fabs(e.alpha) == 1 )
        MatOp_Bin::make_ପଦସମଷ୍ଟି(res, 'a', e.a, -e.s*e.alpha);
    ଅନ୍ୟଥା ଯଦି( e.b.data && e.alpha + e.beta == 0 && e.alpha*e.beta == -1 )
        MatOp_Bin::make_ପଦସମଷ୍ଟି(res, 'a', e.a, e.b);
    ଅନ୍ୟଥା
        MatOp::abs(e, res);
}

ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ MatOp_AddEx::make_ପଦସମଷ୍ଟି(Mat_ପଦସମଷ୍ଟି& res, ଅଚଳ Mat& a, ଅଚଳ Mat& b, ଦ୍ୱିଗୁଣ alpha, ଦ୍ୱିଗୁଣ beta, ଅଚଳ Scalar& s)
{
    res = Mat_ପଦସମଷ୍ଟି(&g_MatOp_AddEx, 0, a, b, Mat(), alpha, beta, s);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ଶୂନ୍ୟ MatOp_Bin::assign(ଅଚଳ Mat_ପଦସମଷ୍ଟି& e, Mat& m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା _type) ଅଚଳ
{
    Mat temp, &dst = _type == -1 || e.a.type() == _type ? m : temp;

    ଯଦି( e.flags == '*' )
        cv::multiply(e.a, e.b, dst, e.alpha);
    ଅନ୍ୟଥା ଯଦି( e.flags == '/' && e.b.data )
        cv::divide(e.a, e.b, dst, e.alpha);
    ଅନ୍ୟଥା ଯଦି( e.flags == '/' && !e.b.data )
        cv::divide(e.alpha, e.a, dst );
    ଅନ୍ୟଥା ଯଦି( e.flags == '&' && e.b.data )
        bitwise_and(e.a, e.b, dst);
    ଅନ୍ୟଥା ଯଦି( e.flags == '&' && !e.b.data )
        bitwise_and(e.a, e.s, dst);
    ଅନ୍ୟଥା ଯଦି( e.flags == '|' && e.b.data )
        bitwise_or(e.a, e.b, dst);
    ଅନ୍ୟଥା ଯଦି( e.flags == '|' && !e.b.data )
        bitwise_or(e.a, e.s, dst);
    ଅନ୍ୟଥା ଯଦି( e.flags == '^' && e.b.data )
        bitwise_xor(e.a, e.b, dst);
    ଅନ୍ୟଥା ଯଦି( e.flags == '^' && !e.b.data )
        bitwise_xor(e.a, e.s, dst);
    ଅନ୍ୟଥା ଯଦି( e.flags == '~' && !e.b.data )
        bitwise_not(e.a, dst);
    ଅନ୍ୟଥା ଯଦି( e.flags == 'm' )
        cv::min(e.a, e.b, dst);
    ଅନ୍ୟଥା ଯଦି( e.flags == 'n' )
        cv::min(e.a, e.s[0], dst);
    ଅନ୍ୟଥା ଯଦି( e.flags == 'M' )
        cv::max(e.a, e.b, dst);
    ଅନ୍ୟଥା ଯଦି( e.flags == 'N' )
        cv::max(e.a, e.s[0], dst);
    ଅନ୍ୟଥା ଯଦି( e.flags == 'a' && e.b.data )
        cv::absdiff(e.a, e.b, dst);
    ଅନ୍ୟଥା ଯଦି( e.flags == 'a' && !e.b.data )
        cv::absdiff(e.a, e.s, dst);
    ଅନ୍ୟଥା
        CV_Error(cv::Error::StsError, "Unknown operation");

    ଯଦି( dst.data != m.data )
        dst.convertTo(m, _type);
}

ଶୂନ୍ୟ MatOp_Bin::multiply(ଅଚଳ Mat_ପଦସମଷ୍ଟି& e, ଦ୍ୱିଗୁଣ s, Mat_ପଦସମଷ୍ଟି& res) ଅଚଳ
{
    CV_INSTRUMENT_REGION();

    ଯଦି( e.flags == '*' || e.flags == '/' )
    {
        res = e;
        res.alpha *= s;
    }
    ଅନ୍ୟଥା
        MatOp::multiply(e, s, res);
}

ଶୂନ୍ୟ MatOp_Bin::divide(ଦ୍ୱିଗୁଣ s, ଅଚଳ Mat_ପଦସମଷ୍ଟି& e, Mat_ପଦସମଷ୍ଟି& res) ଅଚଳ
{
    CV_INSTRUMENT_REGION();

    ଯଦି( e.flags == '/' && (!e.b.data || e.beta == 0) )
        MatOp_AddEx::make_ପଦସମଷ୍ଟି(res, e.a, Mat(), s/e.alpha, 0);
    ଅନ୍ୟଥା
        MatOp::divide(s, e, res);
}

ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ MatOp_Bin::make_ପଦସମଷ୍ଟି(Mat_ପଦସମଷ୍ଟି& res, ଅକ୍ଷର op, ଅଚଳ Mat& a, ଅଚଳ Mat& b, ଦ୍ୱିଗୁଣ scale)
{
    res = Mat_ପଦସମଷ୍ଟି(&g_MatOp_Bin, op, a, b, Mat(), scale, b.data ? 1 : 0);
}

ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ MatOp_Bin::make_ପଦସମଷ୍ଟି(Mat_ପଦସମଷ୍ଟି& res, ଅକ୍ଷର op, ଅଚଳ Mat& a, ଅଚଳ Scalar& s)
{
    res = Mat_ପଦସମଷ୍ଟି(&g_MatOp_Bin, op, a, Mat(), Mat(), 1, 0, s);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

ଶୂନ୍ୟ MatOp_Cmp::assign(ଅଚଳ Mat_ପଦସମଷ୍ଟି& e, Mat& m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା _type) ଅଚଳ
{
    Mat temp, &dst = _type == -1 || _type == CV_8U ? m : temp;

    ଯଦି( e.b.data )
        cv::compare(e.a, e.b, dst, e.flags);
    ଅନ୍ୟଥା
        cv::compare(e.a, e.alpha, dst, e.flags);

    ଯଦି( dst.data != m.data )
        dst.convertTo(m, _type);
}

ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ MatOp_Cmp::make_ପଦସମଷ୍ଟି(Mat_ପଦସମଷ୍ଟି& res, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmpop, ଅଚଳ Mat& a, ଅଚଳ Mat& b)
{
    res = Mat_ପଦସମଷ୍ଟି(&g_MatOp_Cmp, cmpop, a, b, Mat(), 1, 1);
}

ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ MatOp_Cmp::make_ପଦସମଷ୍ଟି(Mat_ପଦସମଷ୍ଟି& res, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmpop, ଅଚଳ Mat& a, ଦ୍ୱିଗୁଣ alpha)
{
    res = Mat_ପଦସମଷ୍ଟି(&g_MatOp_Cmp, cmpop, a, Mat(), Mat(), alpha, 1);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

ଶୂନ୍ୟ MatOp_T::assign(ଅଚଳ Mat_ପଦସମଷ୍ଟି& e, Mat& m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା _type) ଅଚଳ
{
    Mat temp, &dst = _type == -1 || _type == e.a.type() ? m : temp;

    cv::transpose(e.a, dst);

    ଯଦି( dst.data != m.data || e.alpha != 1 )
        dst.convertTo(m, _type, e.alpha);
}

ଶୂନ୍ୟ MatOp_T::multiply(ଅଚଳ Mat_ପଦସମଷ୍ଟି& e, ଦ୍ୱିଗୁଣ s, Mat_ପଦସମଷ୍ଟି& res) ଅଚଳ
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha *= s;
}

ଶୂନ୍ୟ MatOp_T::transpose(ଅଚଳ Mat_ପଦସମଷ୍ଟି& e, Mat_ପଦସମଷ୍ଟି& res) ଅଚଳ
{
    CV_INSTRUMENT_REGION();

    ଯଦି( e.alpha == 1 )
        MatOp_Identity::make_ପଦସମଷ୍ଟି(res, e.a);
    ଅନ୍ୟଥା
        MatOp_AddEx::make_ପଦସମଷ୍ଟି(res, e.a, Mat(), e.alpha, 0);
}

ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ MatOp_T::make_ପଦସମଷ୍ଟି(Mat_ପଦସମଷ୍ଟି& res, ଅଚଳ Mat& a, ଦ୍ୱିଗୁଣ alpha)
{
    res = Mat_ପଦସମଷ୍ଟି(&g_MatOp_T, 0, a, Mat(), Mat(), alpha, 0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

ଶୂନ୍ୟ MatOp_GEMM::assign(ଅଚଳ Mat_ପଦସମଷ୍ଟି& e, Mat& m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା _type) ଅଚଳ
{
    Mat temp, &dst = _type == -1 || _type == e.a.type() ? m : temp;

    cv::gemm(e.a, e.b, e.alpha, e.c, e.beta, dst, e.flags);
    ଯଦି( dst.data != m.data )
        dst.convertTo(m, _type);
}

ଶୂନ୍ୟ MatOp_GEMM::add(ଅଚଳ Mat_ପଦସମଷ୍ଟି& e1, ଅଚଳ Mat_ପଦସମଷ୍ଟି& e2, Mat_ପଦସମଷ୍ଟି& res) ଅଚଳ
{
    CV_INSTRUMENT_REGION();

    ତାର୍କିକ i1 = isIdentity(e1), i2 = isIdentity(e2);
    ଦ୍ୱିଗୁଣ alpha1 = i1 ? 1 : e1.alpha, alpha2 = i2 ? 1 : e2.alpha;

    ଯଦି( isMatProd(e1) && (i2 || isScaled(e2) || isT(e2)) )
        MatOp_GEMM::make_ପଦସମଷ୍ଟି(res, (e1.flags & ~cv::GEMM_3_T)|(isT(e2) ? cv::GEMM_3_T : 0),
                             e1.a, e1.b, alpha1, e2.a, alpha2);
    ଅନ୍ୟଥା ଯଦି( isMatProd(e2) && (i1 || isScaled(e1) || isT(e1)) )
        MatOp_GEMM::make_ପଦସମଷ୍ଟି(res, (e2.flags & ~cv::GEMM_3_T)|(isT(e1) ? cv::GEMM_3_T : 0),
                             e2.a, e2.b, alpha2, e1.a, alpha1);
    ଅନ୍ୟଥା ଯଦି( ଏହି == e2.op )
        MatOp::add(e1, e2, res);
    ଅନ୍ୟଥା
        e2.op->add(e1, e2, res);
}

ଶୂନ୍ୟ MatOp_GEMM::subtract(ଅଚଳ Mat_ପଦସମଷ୍ଟି& e1, ଅଚଳ Mat_ପଦସମଷ୍ଟି& e2, Mat_ପଦସମଷ୍ଟି& res) ଅଚଳ
{
    CV_INSTRUMENT_REGION();

    ତାର୍କିକ i1 = isIdentity(e1), i2 = isIdentity(e2);
    ଦ୍ୱିଗୁଣ alpha1 = i1 ? 1 : e1.alpha, alpha2 = i2 ? 1 : e2.alpha;

    ଯଦି( isMatProd(e1) && (i2 || isScaled(e2) || isT(e2)) )
        MatOp_GEMM::make_ପଦସମଷ୍ଟି(res, (e1.flags & ~cv::GEMM_3_T)|(isT(e2) ? cv::GEMM_3_T : 0),
                             e1.a, e1.b, alpha1, e2.a, -alpha2);
    ଅନ୍ୟଥା ଯଦି( isMatProd(e2) && (i1 || isScaled(e1) || isT(e1)) )
        MatOp_GEMM::make_ପଦସମଷ୍ଟି(res, (e2.flags & ~cv::GEMM_3_T)|(isT(e1) ? cv::GEMM_3_T : 0),
                            e2.a, e2.b, -alpha2, e1.a, alpha1);
    ଅନ୍ୟଥା ଯଦି( ଏହି == e2.op )
        MatOp::subtract(e1, e2, res);
    ଅନ୍ୟଥା
        e2.op->subtract(e1, e2, res);
}

ଶୂନ୍ୟ MatOp_GEMM::multiply(ଅଚଳ Mat_ପଦସମଷ୍ଟି& e, ଦ୍ୱିଗୁଣ s, Mat_ପଦସମଷ୍ଟି& res) ଅଚଳ
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha *= s;
    res.beta *= s;
}

ଶୂନ୍ୟ MatOp_GEMM::transpose(ଅଚଳ Mat_ପଦସମଷ୍ଟି& e, Mat_ପଦସମଷ୍ଟି& res) ଅଚଳ
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.flags = (!(e.flags & cv::GEMM_1_T) ? cv::GEMM_2_T : 0) |
                (!(e.flags & cv::GEMM_2_T) ? cv::GEMM_1_T : 0) |
                (!(e.flags & cv::GEMM_3_T) ? cv::GEMM_3_T : 0);
    swap(res.a, res.b);
}

ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ MatOp_GEMM::make_ପଦସମଷ୍ଟି(Mat_ପଦସମଷ୍ଟି& res, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags, ଅଚଳ Mat& a, ଅଚଳ Mat& b,
                                 ଦ୍ୱିଗୁଣ alpha, ଅଚଳ Mat& c, ଦ୍ୱିଗୁଣ beta)
{
    res = Mat_ପଦସମଷ୍ଟି(&g_MatOp_GEMM, flags, a, b, c, alpha, beta);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

ଶୂନ୍ୟ MatOp_Invert::assign(ଅଚଳ Mat_ପଦସମଷ୍ଟି& e, Mat& m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା _type) ଅଚଳ
{
    Mat temp, &dst = _type == -1 || _type == e.a.type() ? m : temp;

    cv::invert(e.a, dst, e.flags);
    ଯଦି( dst.data != m.data )
        dst.convertTo(m, _type);
}

ଶୂନ୍ୟ MatOp_Invert::matmul(ଅଚଳ Mat_ପଦସମଷ୍ଟି& e1, ଅଚଳ Mat_ପଦସମଷ୍ଟି& e2, Mat_ପଦସମଷ୍ଟି& res) ଅଚଳ
{
    ଯଦି( isInv(e1) && isIdentity(e2) )
        MatOp_Solve::make_ପଦସମଷ୍ଟି(res, e1.flags, e1.a, e2.a);
    ଅନ୍ୟଥା ଯଦି( ଏହି == e2.op )
        MatOp::matmul(e1, e2, res);
    ଅନ୍ୟଥା
        e2.op->matmul(e1, e2, res);
}

ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ MatOp_Invert::make_ପଦସମଷ୍ଟି(Mat_ପଦସମଷ୍ଟି& res, ପୂର୍ଣ୍ଣସଂଖ୍ୟା method, ଅଚଳ Mat& m)
{
    res = Mat_ପଦସମଷ୍ଟି(&g_MatOp_Invert, method, m, Mat(), Mat(), 1, 0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

ଶୂନ୍ୟ MatOp_Solve::assign(ଅଚଳ Mat_ପଦସମଷ୍ଟି& e, Mat& m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା _type) ଅଚଳ
{
    Mat temp, &dst = _type == -1 || _type == e.a.type() ? m : temp;

    cv::solve(e.a, e.b, dst, e.flags);
    ଯଦି( dst.data != m.data )
        dst.convertTo(m, _type);
}

ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ MatOp_Solve::make_ପଦସମଷ୍ଟି(Mat_ପଦସମଷ୍ଟି& res, ପୂର୍ଣ୍ଣସଂଖ୍ୟା method, ଅଚଳ Mat& a, ଅଚଳ Mat& b)
{
    res = Mat_ପଦସମଷ୍ଟି(&g_MatOp_Solve, method, a, b, Mat(), 1, 1);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

ଶୂନ୍ୟ MatOp_Initializer::assign(ଅଚଳ Mat_ପଦସମଷ୍ଟି& e, Mat& m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା _type) ଅଚଳ
{
    ଯଦି( _type == -1 )
        _type = e.a.type();

    m.create(e.a.size, _type);

    ଯଦି( e.flags == 'I' && e.a.dims <= 2 )
        setIdentity(m, Scalar(e.alpha));
    ଅନ୍ୟଥା ଯଦି( e.flags == '0' )
        m = Scalar();
    ଅନ୍ୟଥା ଯଦି( e.flags == '1' )
        m = Scalar(e.alpha);
    ଅନ୍ୟଥା
        CV_Error(cv::Error::StsError, "Invalid matrix initializer type");
}

ଶୂନ୍ୟ MatOp_Initializer::multiply(ଅଚଳ Mat_ପଦସମଷ୍ଟି& e, ଦ୍ୱିଗୁଣ s, Mat_ପଦସମଷ୍ଟି& res) ଅଚଳ
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha *= s;
}

ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ MatOp_Initializer::make_ପଦସମଷ୍ଟି(Mat_ପଦସମଷ୍ଟି& res, ପୂର୍ଣ୍ଣସଂଖ୍ୟା method, Size sz, ପୂର୍ଣ୍ଣସଂଖ୍ୟା type, ଦ୍ୱିଗୁଣ alpha)
{
    res = Mat_ପଦସମଷ୍ଟି(getGlobalMatOpInitializer(), method, Mat(sz, type, (ଶୂନ୍ୟ*)(size_t)0xEEEEEEEE), Mat(), Mat(), alpha, 0);
}

ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ MatOp_Initializer::make_ପଦସମଷ୍ଟି(Mat_ପଦସମଷ୍ଟି& res, ପୂର୍ଣ୍ଣସଂଖ୍ୟା method, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ndims, ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା* sizes, ପୂର୍ଣ୍ଣସଂଖ୍ୟା type, ଦ୍ୱିଗୁଣ alpha)
{
    res = Mat_ପଦସମଷ୍ଟି(getGlobalMatOpInitializer(), method, Mat(ndims, sizes, type, (ଶୂନ୍ୟ*)(size_t)0xEEEEEEEE), Mat(), Mat(), alpha, 0);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

Mat_ପଦସମଷ୍ଟି Mat::t() ଅଚଳ
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(*ଏହି);
    Mat_ପଦସମଷ୍ଟି e;
    MatOp_T::make_ପଦସମଷ୍ଟି(e, *ଏହି);
    ଫେରନ୍ତୁ e;
}

Mat_ପଦସମଷ୍ଟି Mat::inv(ପୂର୍ଣ୍ଣସଂଖ୍ୟା method) ଅଚଳ
{
    CV_INSTRUMENT_REGION();

    Mat_ପଦସମଷ୍ଟି e;
    MatOp_Invert::make_ପଦସମଷ୍ଟି(e, method, *ଏହି);
    ଫେରନ୍ତୁ e;
}


Mat_ପଦସମଷ୍ଟି Mat::mul(ନିବେଶ_Array m, ଦ୍ୱିଗୁଣ scale) ଅଚଳ
{
    CV_INSTRUMENT_REGION();

    Mat b = m.getMat();
    // Unless the argument is a refcounted Mat/UMat, the header returned by getMat() may be
    // a non-owning view of caller memory (e.g. a scalar bound to _InputArray(const double&),
    // a Matx or a Vec) that the returned MatExpr can outlive, so snapshot it.
    // See https://github.com/opencv/opencv/issues/23577
    ଯଦି( !m.isMat() && !m.isUMat() )
        b = b.clone();

    Mat_ପଦସମଷ୍ଟି e;
    MatOp_Bin::make_ପଦସମଷ୍ଟି(e, '*', *ଏହି, b, scale);
    ଫେରନ୍ତୁ e;
}

Mat_ପଦସମଷ୍ଟି Mat::zeros(ପୂର୍ଣ୍ଣସଂଖ୍ୟା rows, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cols, ପୂର୍ଣ୍ଣସଂଖ୍ୟା type)
{
    CV_INSTRUMENT_REGION();

    Mat_ପଦସମଷ୍ଟି e;
    MatOp_Initializer::make_ପଦସମଷ୍ଟି(e, '0', Size(cols, rows), type);
    ଫେରନ୍ତୁ e;
}

Mat_ପଦସମଷ୍ଟି Mat::zeros(Size size, ପୂର୍ଣ୍ଣସଂଖ୍ୟା type)
{
    CV_INSTRUMENT_REGION();

    Mat_ପଦସମଷ୍ଟି e;
    MatOp_Initializer::make_ପଦସମଷ୍ଟି(e, '0', size, type);
    ଫେରନ୍ତୁ e;
}

Mat_ପଦସମଷ୍ଟି Mat::zeros(ପୂର୍ଣ୍ଣସଂଖ୍ୟା ndims, ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା* sizes, ପୂର୍ଣ୍ଣସଂଖ୍ୟା type)
{
    CV_INSTRUMENT_REGION();

    Mat_ପଦସମଷ୍ଟି e;
    MatOp_Initializer::make_ପଦସମଷ୍ଟି(e, '0', ndims, sizes, type);
    ଫେରନ୍ତୁ e;
}

Mat_ପଦସମଷ୍ଟି Mat::zeros(ଅଚଳ MatShape& shape, ପୂର୍ଣ୍ଣସଂଖ୍ୟା type)
{
    CV_INSTRUMENT_REGION();

    Mat_ପଦସମଷ୍ଟି e;
    MatOp_Initializer::make_ପଦସମଷ୍ଟି(e, '0', shape.dims, shape.p, type);
    ଫେରନ୍ତୁ e;
}

Mat_ପଦସମଷ୍ଟି Mat::ones(ପୂର୍ଣ୍ଣସଂଖ୍ୟା rows, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cols, ପୂର୍ଣ୍ଣସଂଖ୍ୟା type)
{
    CV_INSTRUMENT_REGION();

    Mat_ପଦସମଷ୍ଟି e;
    MatOp_Initializer::make_ପଦସମଷ୍ଟି(e, '1', Size(cols, rows), type);
    ଫେରନ୍ତୁ e;
}

Mat_ପଦସମଷ୍ଟି Mat::ones(Size size, ପୂର୍ଣ୍ଣସଂଖ୍ୟା type)
{
    CV_INSTRUMENT_REGION();

    Mat_ପଦସମଷ୍ଟି e;
    MatOp_Initializer::make_ପଦସମଷ୍ଟି(e, '1', size, type);
    ଫେରନ୍ତୁ e;
}

Mat_ପଦସମଷ୍ଟି Mat::ones(ପୂର୍ଣ୍ଣସଂଖ୍ୟା ndims, ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା* sizes, ପୂର୍ଣ୍ଣସଂଖ୍ୟା type)
{
    CV_INSTRUMENT_REGION();

    Mat_ପଦସମଷ୍ଟି e;
    MatOp_Initializer::make_ପଦସମଷ୍ଟି(e, '1', ndims, sizes, type);
    ଫେରନ୍ତୁ e;
}

Mat_ପଦସମଷ୍ଟି Mat::ones(ଅଚଳ MatShape& shape, ପୂର୍ଣ୍ଣସଂଖ୍ୟା type)
{
    CV_INSTRUMENT_REGION();

    Mat_ପଦସମଷ୍ଟି e;
    MatOp_Initializer::make_ପଦସମଷ୍ଟି(e, '1', shape.dims, shape.p, type);
    ଫେରନ୍ତୁ e;
}

Mat_ପଦସମଷ୍ଟି Mat::eye(ପୂର୍ଣ୍ଣସଂଖ୍ୟା rows, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cols, ପୂର୍ଣ୍ଣସଂଖ୍ୟା type)
{
    CV_INSTRUMENT_REGION();

    Mat_ପଦସମଷ୍ଟି e;
    MatOp_Initializer::make_ପଦସମଷ୍ଟି(e, 'I', Size(cols, rows), type);
    ଫେରନ୍ତୁ e;
}

Mat_ପଦସମଷ୍ଟି Mat::eye(Size size, ପୂର୍ଣ୍ଣସଂଖ୍ୟା type)
{
    CV_INSTRUMENT_REGION();

    Mat_ପଦସମଷ୍ଟି e;
    MatOp_Initializer::make_ପଦସମଷ୍ଟି(e, 'I', size, type);
    ଫେରନ୍ତୁ e;
}

ଶୂନ୍ୟ Mat_ପଦସମଷ୍ଟି::swap(Mat_ପଦସମଷ୍ଟି& other)
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

ନିବେଶ_Array_2::ନିବେଶ_Array_2(ଅଚଳ Mat_ପଦସମଷ୍ଟି& ପଦସମଷ୍ଟି)
{
    ଯଦି (!isIdentity(ପଦସମଷ୍ଟି))
    {
        Mat result = ପଦସମଷ୍ଟି;  // TODO improve through refcount == 1 of expr.a (inplace operation is possible - except gemm?)
        Mat_ପଦସମଷ୍ଟି result_ପଦସମଷ୍ଟି(result);
        swap(const_cast<Mat_ପଦସମଷ୍ଟି&>(ପଦସମଷ୍ଟି), result_ପଦସମଷ୍ଟି);
    }
    CV_Assert(isIdentity(ପଦସମଷ୍ଟି));
    init(FIXED_TYPE + FIXED_SIZE + MAT + ACCESS_READ, &ପଦସମଷ୍ଟି.a);
}

} // cv::
