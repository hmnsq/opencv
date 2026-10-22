// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


/* ////////////////////////////////////////////////////////////////////
//
//  Mat basic operations: Copy, Set
//
// */

#ਅਨ੍ਤਰ੍ਭਾਵ "precomp.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ <opencv2/core/utils/logger.hpp>

ਨਾਮਾਕਾਸ਼ cv
{

//This and its overload below are used in various MatExpr operator overloads
//implemented to check that Matrix operands exist.
ਸਥਿਰ ਖਾਲੀ checkOperandsExist(ਅਚਲ Mat& a)
{
    ਜੇ (a.empty())
    {
        CV_Error(cv::Error::StsBadArg, "Matrix operand is an empty matrix.");
    }
}

ਸਥਿਰ ਖਾਲੀ checkOperandsExist(ਅਚਲ Mat& a, ਅਚਲ Mat& b)
{
    ਜੇ (a.empty() || b.empty())
    {
        CV_Error(cv::Error::StsBadArg, "One or more matrix operands are empty.");
    }
}


ਵਰਗ Mat_Op_ਪਛਾਣ_ਤੱਤ CV_FINAL : ਜਨਤਕ MatOp
{
ਜਨਤਕ:
    Mat_Op_ਪਛਾਣ_ਤੱਤ() {}
    ਆਭਾਸੀ ~Mat_Op_ਪਛਾਣ_ਤੱਤ() {}

    ਤਾਰਕਿਕ elementWise(ਅਚਲ Mat_ਵਿਅੰਜਕ& /*ਵਿਅੰਜਕ*/) ਅਚਲ CV_OVERRIDE { ਵਾਪਸ ਸੱਚ; }
    ਖਾਲੀ assign(ਅਚਲ Mat_ਵਿਅੰਜਕ& ਵਿਅੰਜਕ, Mat& m, ਪੂਰਨ type=-1) ਅਚਲ CV_OVERRIDE;

    ਸਥਿਰ ਖਾਲੀ make_ਵਿਅੰਜਕ(Mat_ਵਿਅੰਜਕ& res, ਅਚਲ Mat& m);
};

ਸਥਿਰ Mat_Op_ਪਛਾਣ_ਤੱਤ g_Mat_Op_ਪਛਾਣ_ਤੱਤ;

ਵਰਗ Mat_Op_ਜੋੜੋ_Ex CV_FINAL : ਜਨਤਕ MatOp
{
ਜਨਤਕ:
    Mat_Op_ਜੋੜੋ_Ex() {}
    ਆਭਾਸੀ ~Mat_Op_ਜੋੜੋ_Ex() {}

    ਤਾਰਕਿਕ elementWise(ਅਚਲ Mat_ਵਿਅੰਜਕ& /*ਵਿਅੰਜਕ*/) ਅਚਲ CV_OVERRIDE { ਵਾਪਸ ਸੱਚ; }
    ਖਾਲੀ assign(ਅਚਲ Mat_ਵਿਅੰਜਕ& ਵਿਅੰਜਕ, Mat& m, ਪੂਰਨ type=-1) ਅਚਲ CV_OVERRIDE;

    ਖਾਲੀ ਜੋੜੋ(ਅਚਲ Mat_ਵਿਅੰਜਕ& e1, ਅਚਲ Scalar& s, Mat_ਵਿਅੰਜਕ& res) ਅਚਲ CV_OVERRIDE;
    ਖਾਲੀ ਘਟਾਓ(ਅਚਲ Scalar& s, ਅਚਲ Mat_ਵਿਅੰਜਕ& ਵਿਅੰਜਕ, Mat_ਵਿਅੰਜਕ& res) ਅਚਲ CV_OVERRIDE;
    ਖਾਲੀ multiply(ਅਚਲ Mat_ਵਿਅੰਜਕ& e1, ਦੁੱਗਣਾ s, Mat_ਵਿਅੰਜਕ& res) ਅਚਲ CV_OVERRIDE;
    ਖਾਲੀ divide(ਦੁੱਗਣਾ s, ਅਚਲ Mat_ਵਿਅੰਜਕ& e, Mat_ਵਿਅੰਜਕ& res) ਅਚਲ CV_OVERRIDE;

    ਖਾਲੀ transpose(ਅਚਲ Mat_ਵਿਅੰਜਕ& e1, Mat_ਵਿਅੰਜਕ& res) ਅਚਲ CV_OVERRIDE;
    ਖਾਲੀ ਨਿਰਪੇਖ(ਅਚਲ Mat_ਵਿਅੰਜਕ& ਵਿਅੰਜਕ, Mat_ਵਿਅੰਜਕ& res) ਅਚਲ CV_OVERRIDE;

    ਸਥਿਰ ਖਾਲੀ make_ਵਿਅੰਜਕ(Mat_ਵਿਅੰਜਕ& res, ਅਚਲ Mat& a, ਅਚਲ Mat& b, ਦੁੱਗਣਾ alpha, ਦੁੱਗਣਾ beta, ਅਚਲ Scalar& s=Scalar());
};

ਸਥਿਰ Mat_Op_ਜੋੜੋ_Ex g_Mat_Op_ਜੋੜੋ_Ex;

ਵਰਗ MatOp_Bin CV_FINAL : ਜਨਤਕ MatOp
{
ਜਨਤਕ:
    MatOp_Bin() {}
    ਆਭਾਸੀ ~MatOp_Bin() {}

    ਤਾਰਕਿਕ elementWise(ਅਚਲ Mat_ਵਿਅੰਜਕ& /*ਵਿਅੰਜਕ*/) ਅਚਲ CV_OVERRIDE { ਵਾਪਸ ਸੱਚ; }
    ਖਾਲੀ assign(ਅਚਲ Mat_ਵਿਅੰਜਕ& ਵਿਅੰਜਕ, Mat& m, ਪੂਰਨ type=-1) ਅਚਲ CV_OVERRIDE;

    ਖਾਲੀ multiply(ਅਚਲ Mat_ਵਿਅੰਜਕ& e1, ਦੁੱਗਣਾ s, Mat_ਵਿਅੰਜਕ& res) ਅਚਲ CV_OVERRIDE;
    ਖਾਲੀ divide(ਦੁੱਗਣਾ s, ਅਚਲ Mat_ਵਿਅੰਜਕ& e, Mat_ਵਿਅੰਜਕ& res) ਅਚਲ CV_OVERRIDE;

    ਸਥਿਰ ਖਾਲੀ make_ਵਿਅੰਜਕ(Mat_ਵਿਅੰਜਕ& res, ਅੱਖਰ op, ਅਚਲ Mat& a, ਅਚਲ Mat& b, ਦੁੱਗਣਾ scale=1);
    ਸਥਿਰ ਖਾਲੀ make_ਵਿਅੰਜਕ(Mat_ਵਿਅੰਜਕ& res, ਅੱਖਰ op, ਅਚਲ Mat& a, ਅਚਲ Scalar& s);
};

ਸਥਿਰ MatOp_Bin g_MatOp_Bin;

ਵਰਗ MatOp_Cmp CV_FINAL : ਜਨਤਕ MatOp
{
ਜਨਤਕ:
    MatOp_Cmp() {}
    ਆਭਾਸੀ ~MatOp_Cmp() {}

    ਤਾਰਕਿਕ elementWise(ਅਚਲ Mat_ਵਿਅੰਜਕ& /*ਵਿਅੰਜਕ*/) ਅਚਲ CV_OVERRIDE { ਵਾਪਸ ਸੱਚ; }
    ਖਾਲੀ assign(ਅਚਲ Mat_ਵਿਅੰਜਕ& ਵਿਅੰਜਕ, Mat& m, ਪੂਰਨ type=-1) ਅਚਲ CV_OVERRIDE;

    ਸਥਿਰ ਖਾਲੀ make_ਵਿਅੰਜਕ(Mat_ਵਿਅੰਜਕ& res, ਪੂਰਨ cmpop, ਅਚਲ Mat& a, ਅਚਲ Mat& b);
    ਸਥਿਰ ਖਾਲੀ make_ਵਿਅੰਜਕ(Mat_ਵਿਅੰਜਕ& res, ਪੂਰਨ cmpop, ਅਚਲ Mat& a, ਦੁੱਗਣਾ alpha);
};

ਸਥਿਰ MatOp_Cmp g_MatOp_Cmp;

ਵਰਗ MatOp_GEMM CV_FINAL : ਜਨਤਕ MatOp
{
ਜਨਤਕ:
    MatOp_GEMM() {}
    ਆਭਾਸੀ ~MatOp_GEMM() {}

    ਤਾਰਕਿਕ elementWise(ਅਚਲ Mat_ਵਿਅੰਜਕ& /*ਵਿਅੰਜਕ*/) ਅਚਲ CV_OVERRIDE { ਵਾਪਸ ਝੂਠ; }
    ਖਾਲੀ assign(ਅਚਲ Mat_ਵਿਅੰਜਕ& ਵਿਅੰਜਕ, Mat& m, ਪੂਰਨ type=-1) ਅਚਲ CV_OVERRIDE;

    ਖਾਲੀ ਜੋੜੋ(ਅਚਲ Mat_ਵਿਅੰਜਕ& e1, ਅਚਲ Mat_ਵਿਅੰਜਕ& e2, Mat_ਵਿਅੰਜਕ& res) ਅਚਲ CV_OVERRIDE;
    ਖਾਲੀ ਘਟਾਓ(ਅਚਲ Mat_ਵਿਅੰਜਕ& e1, ਅਚਲ Mat_ਵਿਅੰਜਕ& e2, Mat_ਵਿਅੰਜਕ& res) ਅਚਲ CV_OVERRIDE;
    ਖਾਲੀ multiply(ਅਚਲ Mat_ਵਿਅੰਜਕ& e, ਦੁੱਗਣਾ s, Mat_ਵਿਅੰਜਕ& res) ਅਚਲ CV_OVERRIDE;

    ਖਾਲੀ transpose(ਅਚਲ Mat_ਵਿਅੰਜਕ& ਵਿਅੰਜਕ, Mat_ਵਿਅੰਜਕ& res) ਅਚਲ CV_OVERRIDE;

    ਮਾਪ size(ਅਚਲ Mat_ਵਿਅੰਜਕ& ਵਿਅੰਜਕ) ਅਚਲ CV_OVERRIDE
    {
        ਵਾਪਸ ਮਾਪ(
            (ਵਿਅੰਜਕ.flags & GEMM_2_T) ? ਵਿਅੰਜਕ.b.ਪੰਕਤੀਆਂ : ਵਿਅੰਜਕ.b.ਖਾਨੇ_ਗਿਣਤੀ,
            (ਵਿਅੰਜਕ.flags & GEMM_1_T) ? ਵਿਅੰਜਕ.a.ਖਾਨੇ_ਗਿਣਤੀ : ਵਿਅੰਜਕ.a.ਪੰਕਤੀਆਂ
        );
    }

    ਸਥਿਰ ਖਾਲੀ make_ਵਿਅੰਜਕ(Mat_ਵਿਅੰਜਕ& res, ਪੂਰਨ flags, ਅਚਲ Mat& a, ਅਚਲ Mat& b,
                         ਦੁੱਗਣਾ alpha=1, ਅਚਲ Mat& c=Mat(), ਦੁੱਗਣਾ beta=1);

};

ਸਥਿਰ MatOp_GEMM g_MatOp_GEMM;

ਵਰਗ MatOp_Invert CV_FINAL : ਜਨਤਕ MatOp
{
ਜਨਤਕ:
    MatOp_Invert() {}
    ਆਭਾਸੀ ~MatOp_Invert() {}

    ਤਾਰਕਿਕ elementWise(ਅਚਲ Mat_ਵਿਅੰਜਕ& /*ਵਿਅੰਜਕ*/) ਅਚਲ CV_OVERRIDE { ਵਾਪਸ ਝੂਠ; }
    ਖਾਲੀ assign(ਅਚਲ Mat_ਵਿਅੰਜਕ& ਵਿਅੰਜਕ, Mat& m, ਪੂਰਨ type=-1) ਅਚਲ CV_OVERRIDE;

    ਖਾਲੀ matmul(ਅਚਲ Mat_ਵਿਅੰਜਕ& expr1, ਅਚਲ Mat_ਵਿਅੰਜਕ& expr2, Mat_ਵਿਅੰਜਕ& res) ਅਚਲ CV_OVERRIDE;

    ਸਥਿਰ ਖਾਲੀ make_ਵਿਅੰਜਕ(Mat_ਵਿਅੰਜਕ& res, ਪੂਰਨ ਵਿਧੀ, ਅਚਲ Mat& m);
};

ਸਥਿਰ MatOp_Invert g_MatOp_Invert;

ਵਰਗ MatOp_T CV_FINAL : ਜਨਤਕ MatOp
{
ਜਨਤਕ:
    MatOp_T() {}
    ਆਭਾਸੀ ~MatOp_T() {}

    ਤਾਰਕਿਕ elementWise(ਅਚਲ Mat_ਵਿਅੰਜਕ& /*ਵਿਅੰਜਕ*/) ਅਚਲ CV_OVERRIDE { ਵਾਪਸ ਝੂਠ; }
    ਖਾਲੀ assign(ਅਚਲ Mat_ਵਿਅੰਜਕ& ਵਿਅੰਜਕ, Mat& m, ਪੂਰਨ type=-1) ਅਚਲ CV_OVERRIDE;

    ਖਾਲੀ multiply(ਅਚਲ Mat_ਵਿਅੰਜਕ& e1, ਦੁੱਗਣਾ s, Mat_ਵਿਅੰਜਕ& res) ਅਚਲ CV_OVERRIDE;
    ਖਾਲੀ transpose(ਅਚਲ Mat_ਵਿਅੰਜਕ& ਵਿਅੰਜਕ, Mat_ਵਿਅੰਜਕ& res) ਅਚਲ CV_OVERRIDE;

    ਸਥਿਰ ਖਾਲੀ make_ਵਿਅੰਜਕ(Mat_ਵਿਅੰਜਕ& res, ਅਚਲ Mat& a, ਦੁੱਗਣਾ alpha=1);
};

ਸਥਿਰ MatOp_T g_MatOp_T;

ਵਰਗ MatOp_Solve CV_FINAL : ਜਨਤਕ MatOp
{
ਜਨਤਕ:
    MatOp_Solve() {}
    ਆਭਾਸੀ ~MatOp_Solve() {}

    ਤਾਰਕਿਕ elementWise(ਅਚਲ Mat_ਵਿਅੰਜਕ& /*ਵਿਅੰਜਕ*/) ਅਚਲ CV_OVERRIDE { ਵਾਪਸ ਝੂਠ; }
    ਖਾਲੀ assign(ਅਚਲ Mat_ਵਿਅੰਜਕ& ਵਿਅੰਜਕ, Mat& m, ਪੂਰਨ type=-1) ਅਚਲ CV_OVERRIDE;

    ਸਥਿਰ ਖਾਲੀ make_ਵਿਅੰਜਕ(Mat_ਵਿਅੰਜਕ& res, ਪੂਰਨ ਵਿਧੀ, ਅਚਲ Mat& a, ਅਚਲ Mat& b);
};

ਸਥਿਰ MatOp_Solve g_MatOp_Solve;

ਵਰਗ MatOp_Initializer CV_FINAL : ਜਨਤਕ MatOp
{
ਜਨਤਕ:
    MatOp_Initializer() {}
    ਆਭਾਸੀ ~MatOp_Initializer() {}

    ਤਾਰਕਿਕ elementWise(ਅਚਲ Mat_ਵਿਅੰਜਕ& /*ਵਿਅੰਜਕ*/) ਅਚਲ CV_OVERRIDE { ਵਾਪਸ ਝੂਠ; }
    ਖਾਲੀ assign(ਅਚਲ Mat_ਵਿਅੰਜਕ& ਵਿਅੰਜਕ, Mat& m, ਪੂਰਨ type=-1) ਅਚਲ CV_OVERRIDE;

    ਖਾਲੀ multiply(ਅਚਲ Mat_ਵਿਅੰਜਕ& e, ਦੁੱਗਣਾ s, Mat_ਵਿਅੰਜਕ& res) ਅਚਲ CV_OVERRIDE;

    ਸਥਿਰ ਖਾਲੀ make_ਵਿਅੰਜਕ(Mat_ਵਿਅੰਜਕ& res, ਪੂਰਨ ਵਿਧੀ, ਮਾਪ sz, ਪੂਰਨ type, ਦੁੱਗਣਾ alpha=1);
    ਸਥਿਰ ਖਾਲੀ make_ਵਿਅੰਜਕ(Mat_ਵਿਅੰਜਕ& res, ਪੂਰਨ ਵਿਧੀ, ਪੂਰਨ ndims, ਅਚਲ ਪੂਰਨ* sizes, ਪੂਰਨ type, ਦੁੱਗਣਾ alpha=1);
};

ਸਥਿਰ MatOp_Initializer* getGlobalMatOpInitializer()
{
    CV_SINGLETON_LAZY_INIT(MatOp_Initializer, ਨਵਾਂ MatOp_Initializer())
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ is_ਪਛਾਣ_ਤੱਤ(ਅਚਲ Mat_ਵਿਅੰਜਕ& e) { ਵਾਪਸ e.op == &g_Mat_Op_ਪਛਾਣ_ਤੱਤ; }
ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ is_ਜੋੜੋ_Ex(ਅਚਲ Mat_ਵਿਅੰਜਕ& e) { ਵਾਪਸ e.op == &g_Mat_Op_ਜੋੜੋ_Ex; }
ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ isScaled(ਅਚਲ Mat_ਵਿਅੰਜਕ& e) { ਵਾਪਸ is_ਜੋੜੋ_Ex(e) && (!e.b.ਅੰਕੜੇ || e.beta == 0) && e.s == Scalar(); }
ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ isBin(ਅਚਲ Mat_ਵਿਅੰਜਕ& e, ਅੱਖਰ c) { ਵਾਪਸ e.op == &g_MatOp_Bin && e.flags == c; }
ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ isCmp(ਅਚਲ Mat_ਵਿਅੰਜਕ& e) { ਵਾਪਸ e.op == &g_MatOp_Cmp; }
ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ isReciprocal(ਅਚਲ Mat_ਵਿਅੰਜਕ& e) { ਵਾਪਸ isBin(e,'/') && (!e.b.ਅੰਕੜੇ || e.beta == 0); }
ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ isT(ਅਚਲ Mat_ਵਿਅੰਜਕ& e) { ਵਾਪਸ e.op == &g_MatOp_T; }
ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ isInv(ਅਚਲ Mat_ਵਿਅੰਜਕ& e) { ਵਾਪਸ e.op == &g_MatOp_Invert; }
ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ isSolve(ਅਚਲ Mat_ਵਿਅੰਜਕ& e) { ਵਾਪਸ e.op == &g_MatOp_Solve; }
//static inline bool isGEMM(const MatExpr& e) { return e.op == &g_MatOp_GEMM; }
ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ isMatProd(ਅਚਲ Mat_ਵਿਅੰਜਕ& e) { ਵਾਪਸ e.op == &g_MatOp_GEMM && (!e.c.ਅੰਕੜੇ || e.beta == 0); }
ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ isInitializer(ਅਚਲ Mat_ਵਿਅੰਜਕ& e) { ਵਾਪਸ e.op == getGlobalMatOpInitializer(); }

/////////////////////////////////////////////////////////////////////////////////////////////////////

MatOp::MatOp() {}
MatOp::~MatOp() {}


ਤਾਰਕਿਕ MatOp::elementWise(ਅਚਲ Mat_ਵਿਅੰਜਕ& /*ਵਿਅੰਜਕ*/) ਅਚਲ
{
    ਵਾਪਸ ਝੂਠ;
}

ਖਾਲੀ MatOp::roi(ਅਚਲ Mat_ਵਿਅੰਜਕ& ਵਿਅੰਜਕ, ਅਚਲ Range& ਪੰਕਤੀ_Range_2, ਅਚਲ Range& ਖਾਨਾ_Range_2, Mat_ਵਿਅੰਜਕ& e) ਅਚਲ
{
    ਜੇ( elementWise(ਵਿਅੰਜਕ) )
    {
        e = Mat_ਵਿਅੰਜਕ(ਵਿਅੰਜਕ.op, ਵਿਅੰਜਕ.flags, Mat(), Mat(), Mat(),
                    ਵਿਅੰਜਕ.alpha, ਵਿਅੰਜਕ.beta, ਵਿਅੰਜਕ.s);
        ਜੇ(ਵਿਅੰਜਕ.a.ਅੰਕੜੇ)
            e.a = ਵਿਅੰਜਕ.a(ਪੰਕਤੀ_Range_2, ਖਾਨਾ_Range_2);
        ਜੇ(ਵਿਅੰਜਕ.b.ਅੰਕੜੇ)
            e.b = ਵਿਅੰਜਕ.b(ਪੰਕਤੀ_Range_2, ਖਾਨਾ_Range_2);
        ਜੇ(ਵਿਅੰਜਕ.c.ਅੰਕੜੇ)
            e.c = ਵਿਅੰਜਕ.c(ਪੰਕਤੀ_Range_2, ਖਾਨਾ_Range_2);
    }
    ਨਹੀਂਤਾਂ
    {
        Mat m;
        ਵਿਅੰਜਕ.op->assign(ਵਿਅੰਜਕ, m);
        e = Mat_ਵਿਅੰਜਕ(&g_Mat_Op_ਪਛਾਣ_ਤੱਤ, 0, m(ਪੰਕਤੀ_Range_2, ਖਾਨਾ_Range_2), Mat(), Mat());
    }
}

ਖਾਲੀ MatOp::diag(ਅਚਲ Mat_ਵਿਅੰਜਕ& ਵਿਅੰਜਕ, ਪੂਰਨ d, Mat_ਵਿਅੰਜਕ& e) ਅਚਲ
{
    ਜੇ( elementWise(ਵਿਅੰਜਕ) )
    {
        e = Mat_ਵਿਅੰਜਕ(ਵਿਅੰਜਕ.op, ਵਿਅੰਜਕ.flags, Mat(), Mat(), Mat(),
                    ਵਿਅੰਜਕ.alpha, ਵਿਅੰਜਕ.beta, ਵਿਅੰਜਕ.s);
        ਜੇ(ਵਿਅੰਜਕ.a.ਅੰਕੜੇ)
            e.a = ਵਿਅੰਜਕ.a.diag(d);
        ਜੇ(ਵਿਅੰਜਕ.b.ਅੰਕੜੇ)
            e.b = ਵਿਅੰਜਕ.b.diag(d);
        ਜੇ(ਵਿਅੰਜਕ.c.ਅੰਕੜੇ)
            e.c = ਵਿਅੰਜਕ.c.diag(d);
    }
    ਨਹੀਂਤਾਂ
    {
        Mat m;
        ਵਿਅੰਜਕ.op->assign(ਵਿਅੰਜਕ, m);
        e = Mat_ਵਿਅੰਜਕ(&g_Mat_Op_ਪਛਾਣ_ਤੱਤ, 0, m.diag(d), Mat(), Mat());
    }
}


ਖਾਲੀ MatOp::aug_ਸੌਂਪੋ_ਜੋੜੋ(ਅਚਲ Mat_ਵਿਅੰਜਕ& ਵਿਅੰਜਕ, Mat& m) ਅਚਲ
{
    Mat ਆਰਜ਼ੀ_2;
    ਵਿਅੰਜਕ.op->assign(ਵਿਅੰਜਕ, ਆਰਜ਼ੀ_2);
    m += ਆਰਜ਼ੀ_2;
}


ਖਾਲੀ MatOp::aug_ਸੌਂਪੋ_ਘਟਾਓ(ਅਚਲ Mat_ਵਿਅੰਜਕ& ਵਿਅੰਜਕ, Mat& m) ਅਚਲ
{
    Mat ਆਰਜ਼ੀ_2;
    ਵਿਅੰਜਕ.op->assign(ਵਿਅੰਜਕ, ਆਰਜ਼ੀ_2);
    m -= ਆਰਜ਼ੀ_2;
}


ਖਾਲੀ MatOp::aug_ਸੌਂਪੋ_Multiply(ਅਚਲ Mat_ਵਿਅੰਜਕ& ਵਿਅੰਜਕ, Mat& m) ਅਚਲ
{
    Mat ਆਰਜ਼ੀ_2;
    ਵਿਅੰਜਕ.op->assign(ਵਿਅੰਜਕ, ਆਰਜ਼ੀ_2);
    m *= ਆਰਜ਼ੀ_2;
}


ਖਾਲੀ MatOp::aug_ਸੌਂਪੋ_Divide(ਅਚਲ Mat_ਵਿਅੰਜਕ& ਵਿਅੰਜਕ, Mat& m) ਅਚਲ
{
    Mat ਆਰਜ਼ੀ_2;
    ਵਿਅੰਜਕ.op->assign(ਵਿਅੰਜਕ, ਆਰਜ਼ੀ_2);
    m /= ਆਰਜ਼ੀ_2;
}


ਖਾਲੀ MatOp::aug_ਸੌਂਪੋ_And(ਅਚਲ Mat_ਵਿਅੰਜਕ& ਵਿਅੰਜਕ, Mat& m) ਅਚਲ
{
    Mat ਆਰਜ਼ੀ_2;
    ਵਿਅੰਜਕ.op->assign(ਵਿਅੰਜਕ, ਆਰਜ਼ੀ_2);
    m &= ਆਰਜ਼ੀ_2;
}


ਖਾਲੀ MatOp::aug_ਸੌਂਪੋ_Or(ਅਚਲ Mat_ਵਿਅੰਜਕ& ਵਿਅੰਜਕ, Mat& m) ਅਚਲ
{
    Mat ਆਰਜ਼ੀ_2;
    ਵਿਅੰਜਕ.op->assign(ਵਿਅੰਜਕ, ਆਰਜ਼ੀ_2);
    m |= ਆਰਜ਼ੀ_2;
}


ਖਾਲੀ MatOp::aug_ਸੌਂਪੋ_Xor(ਅਚਲ Mat_ਵਿਅੰਜਕ& ਵਿਅੰਜਕ, Mat& m) ਅਚਲ
{
    Mat ਆਰਜ਼ੀ_2;
    ਵਿਅੰਜਕ.op->assign(ਵਿਅੰਜਕ, ਆਰਜ਼ੀ_2);
    m ^= ਆਰਜ਼ੀ_2;
}


ਖਾਲੀ MatOp::ਜੋੜੋ(ਅਚਲ Mat_ਵਿਅੰਜਕ& e1, ਅਚਲ Mat_ਵਿਅੰਜਕ& e2, Mat_ਵਿਅੰਜਕ& res) ਅਚਲ
{
    CV_INSTRUMENT_REGION();

    ਜੇ( ਇਹ == e2.op )
    {
        ਦੁੱਗਣਾ alpha = 1, beta = 1;
        Scalar s;
        Mat m1, m2;
        ਜੇ( is_ਜੋੜੋ_Ex(e1) && (!e1.b.ਅੰਕੜੇ || e1.beta == 0) )
        {
            m1 = e1.a;
            alpha = e1.alpha;
            s = e1.s;
        }
        ਨਹੀਂਤਾਂ
            e1.op->assign(e1, m1);

        ਜੇ( is_ਜੋੜੋ_Ex(e2) && (!e2.b.ਅੰਕੜੇ || e2.beta == 0) )
        {
            m2 = e2.a;
            beta = e2.alpha;
            s += e2.s;
        }
        ਨਹੀਂਤਾਂ
            e2.op->assign(e2, m2);
        Mat_Op_ਜੋੜੋ_Ex::make_ਵਿਅੰਜਕ(res, m1, m2, alpha, beta, s);
    }
    ਨਹੀਂਤਾਂ
        e2.op->ਜੋੜੋ(e1, e2, res);
}


ਖਾਲੀ MatOp::ਜੋੜੋ(ਅਚਲ Mat_ਵਿਅੰਜਕ& expr1, ਅਚਲ Scalar& s, Mat_ਵਿਅੰਜਕ& res) ਅਚਲ
{
    CV_INSTRUMENT_REGION();

    Mat m1;
    expr1.op->assign(expr1, m1);
    Mat_Op_ਜੋੜੋ_Ex::make_ਵਿਅੰਜਕ(res, m1, Mat(), 1, 0, s);
}


ਖਾਲੀ MatOp::ਘਟਾਓ(ਅਚਲ Mat_ਵਿਅੰਜਕ& e1, ਅਚਲ Mat_ਵਿਅੰਜਕ& e2, Mat_ਵਿਅੰਜਕ& res) ਅਚਲ
{
    CV_INSTRUMENT_REGION();

    ਜੇ( ਇਹ == e2.op )
    {
        ਦੁੱਗਣਾ alpha = 1, beta = -1;
        Scalar s;
        Mat m1, m2;
        ਜੇ( is_ਜੋੜੋ_Ex(e1) && (!e1.b.ਅੰਕੜੇ || e1.beta == 0) )
        {
            m1 = e1.a;
            alpha = e1.alpha;
            s = e1.s;
        }
        ਨਹੀਂਤਾਂ
            e1.op->assign(e1, m1);

        ਜੇ( is_ਜੋੜੋ_Ex(e2) && (!e2.b.ਅੰਕੜੇ || e2.beta == 0) )
        {
            m2 = e2.a;
            beta = -e2.alpha;
            s -= e2.s;
        }
        ਨਹੀਂਤਾਂ
            e2.op->assign(e2, m2);
        Mat_Op_ਜੋੜੋ_Ex::make_ਵਿਅੰਜਕ(res, m1, m2, alpha, beta, s);
    }
    ਨਹੀਂਤਾਂ
        e2.op->ਘਟਾਓ(e1, e2, res);
}


ਖਾਲੀ MatOp::ਘਟਾਓ(ਅਚਲ Scalar& s, ਅਚਲ Mat_ਵਿਅੰਜਕ& ਵਿਅੰਜਕ, Mat_ਵਿਅੰਜਕ& res) ਅਚਲ
{
    CV_INSTRUMENT_REGION();

    Mat m;
    ਵਿਅੰਜਕ.op->assign(ਵਿਅੰਜਕ, m);
    Mat_Op_ਜੋੜੋ_Ex::make_ਵਿਅੰਜਕ(res, m, Mat(), -1, 0, s);
}


ਖਾਲੀ MatOp::multiply(ਅਚਲ Mat_ਵਿਅੰਜਕ& e1, ਅਚਲ Mat_ਵਿਅੰਜਕ& e2, Mat_ਵਿਅੰਜਕ& res, ਦੁੱਗਣਾ scale) ਅਚਲ
{
    CV_INSTRUMENT_REGION();

    ਜੇ( ਇਹ == e2.op )
    {
        Mat m1, m2;

        ਜੇ( isReciprocal(e1) )
        {
            ਜੇ( isScaled(e2) )
            {
                scale *= e2.alpha;
                m2 = e2.a;
            }
            ਨਹੀਂਤਾਂ
                e2.op->assign(e2, m2);

            MatOp_Bin::make_ਵਿਅੰਜਕ(res, '/', m2, e1.a, scale/e1.alpha);
        }
        ਨਹੀਂਤਾਂ
        {
            ਅੱਖਰ op = '*';
            ਜੇ( isScaled(e1) )
            {
                m1 = e1.a;
                scale *= e1.alpha;
            }
            ਨਹੀਂਤਾਂ
                e1.op->assign(e1, m1);

            ਜੇ( isScaled(e2) )
            {
                m2 = e2.a;
                scale *= e2.alpha;
            }
            ਨਹੀਂਤਾਂ ਜੇ( isReciprocal(e2) )
            {
                op = '/';
                m2 = e2.a;
                scale *= e2.alpha;
            }
            ਨਹੀਂਤਾਂ
                e2.op->assign(e2, m2);

            MatOp_Bin::make_ਵਿਅੰਜਕ(res, op, m1, m2, scale);
        }
    }
    ਨਹੀਂਤਾਂ
        e2.op->multiply(e1, e2, res, scale);
}


ਖਾਲੀ MatOp::multiply(ਅਚਲ Mat_ਵਿਅੰਜਕ& ਵਿਅੰਜਕ, ਦੁੱਗਣਾ s, Mat_ਵਿਅੰਜਕ& res) ਅਚਲ
{
    CV_INSTRUMENT_REGION();

    Mat m;
    ਵਿਅੰਜਕ.op->assign(ਵਿਅੰਜਕ, m);
    Mat_Op_ਜੋੜੋ_Ex::make_ਵਿਅੰਜਕ(res, m, Mat(), s, 0);
}


ਖਾਲੀ MatOp::divide(ਅਚਲ Mat_ਵਿਅੰਜਕ& e1, ਅਚਲ Mat_ਵਿਅੰਜਕ& e2, Mat_ਵਿਅੰਜਕ& res, ਦੁੱਗਣਾ scale) ਅਚਲ
{
    CV_INSTRUMENT_REGION();

    ਜੇ( ਇਹ == e2.op )
    {
        ਜੇ( isReciprocal(e1) && isReciprocal(e2) )
            MatOp_Bin::make_ਵਿਅੰਜਕ(res, '/', e2.a, e1.a, e1.alpha/e2.alpha);
        ਨਹੀਂਤਾਂ
        {
            Mat m1, m2;
            ਅੱਖਰ op = '/';

            ਜੇ( isScaled(e1) )
            {
                m1 = e1.a;
                scale *= e1.alpha;
            }
            ਨਹੀਂਤਾਂ
                e1.op->assign(e1, m1);

            ਜੇ( isScaled(e2) )
            {
                m2 = e2.a;
                scale /= e2.alpha;
            }
            ਨਹੀਂਤਾਂ ਜੇ( isReciprocal(e2) )
            {
                m2 = e2.a;
                scale /= e2.alpha;
                op = '*';
            }
            ਨਹੀਂਤਾਂ
                e2.op->assign(e2, m2);
            MatOp_Bin::make_ਵਿਅੰਜਕ(res, op, m1, m2, scale);
        }
    }
    ਨਹੀਂਤਾਂ
        e2.op->divide(e1, e2, res, scale);
}


ਖਾਲੀ MatOp::divide(ਦੁੱਗਣਾ s, ਅਚਲ Mat_ਵਿਅੰਜਕ& ਵਿਅੰਜਕ, Mat_ਵਿਅੰਜਕ& res) ਅਚਲ
{
    CV_INSTRUMENT_REGION();

    Mat m;
    ਵਿਅੰਜਕ.op->assign(ਵਿਅੰਜਕ, m);
    MatOp_Bin::make_ਵਿਅੰਜਕ(res, '/', m, Mat(), s);
}


ਖਾਲੀ MatOp::ਨਿਰਪੇਖ(ਅਚਲ Mat_ਵਿਅੰਜਕ& ਵਿਅੰਜਕ, Mat_ਵਿਅੰਜਕ& res) ਅਚਲ
{
    CV_INSTRUMENT_REGION();

    Mat m;
    ਵਿਅੰਜਕ.op->assign(ਵਿਅੰਜਕ, m);
    MatOp_Bin::make_ਵਿਅੰਜਕ(res, 'a', m, Mat());
}


ਖਾਲੀ MatOp::transpose(ਅਚਲ Mat_ਵਿਅੰਜਕ& ਵਿਅੰਜਕ, Mat_ਵਿਅੰਜਕ& res) ਅਚਲ
{
    CV_INSTRUMENT_REGION();

    Mat m;
    ਵਿਅੰਜਕ.op->assign(ਵਿਅੰਜਕ, m);
    MatOp_T::make_ਵਿਅੰਜਕ(res, m, 1);
}


ਖਾਲੀ MatOp::matmul(ਅਚਲ Mat_ਵਿਅੰਜਕ& e1, ਅਚਲ Mat_ਵਿਅੰਜਕ& e2, Mat_ਵਿਅੰਜਕ& res) ਅਚਲ
{
    ਜੇ( ਇਹ == e2.op )
    {
        ਦੁੱਗਣਾ scale = 1;
        ਪੂਰਨ flags = 0;
        Mat m1, m2;

        ਜੇ( isT(e1) )
        {
            flags = cv::GEMM_1_T;
            scale = e1.alpha;
            m1 = e1.a;
        }
        ਨਹੀਂਤਾਂ ਜੇ( isScaled(e1) )
        {
            scale = e1.alpha;
            m1 = e1.a;
        }
        ਨਹੀਂਤਾਂ
            e1.op->assign(e1, m1);

        ਜੇ( isT(e2) )
        {
            flags |= cv::GEMM_2_T;
            scale *= e2.alpha;
            m2 = e2.a;
        }
        ਨਹੀਂਤਾਂ ਜੇ( isScaled(e2) )
        {
            scale *= e2.alpha;
            m2 = e2.a;
        }
        ਨਹੀਂਤਾਂ
            e2.op->assign(e2, m2);

        MatOp_GEMM::make_ਵਿਅੰਜਕ(res, flags, m1, m2, scale);
    }
    ਨਹੀਂਤਾਂ
        e2.op->matmul(e1, e2, res);
}


ਖਾਲੀ MatOp::invert(ਅਚਲ Mat_ਵਿਅੰਜਕ& ਵਿਅੰਜਕ, ਪੂਰਨ ਵਿਧੀ, Mat_ਵਿਅੰਜਕ& res) ਅਚਲ
{
    Mat m;
    ਵਿਅੰਜਕ.op->assign(ਵਿਅੰਜਕ, m);
    MatOp_Invert::make_ਵਿਅੰਜਕ(res, ਵਿਧੀ, m);
}


ਮਾਪ MatOp::size(ਅਚਲ Mat_ਵਿਅੰਜਕ& ਵਿਅੰਜਕ) ਅਚਲ
{
    ਵਾਪਸ !ਵਿਅੰਜਕ.a.empty() ? ਵਿਅੰਜਕ.a.size() : ਵਿਅੰਜਕ.b.empty() ? ਵਿਅੰਜਕ.b.size() : ਵਿਅੰਜਕ.c.size();
}

ਪੂਰਨ MatOp::type(ਅਚਲ Mat_ਵਿਅੰਜਕ& ਵਿਅੰਜਕ) ਅਚਲ
{
    CV_INSTRUMENT_REGION();

    ਵਾਪਸ !ਵਿਅੰਜਕ.a.empty() ? ਵਿਅੰਜਕ.a.type() : ਵਿਅੰਜਕ.b.empty() ? ਵਿਅੰਜਕ.b.type() : ਵਿਅੰਜਕ.c.type();
}

//////////////////////////////////////////////////////////////////////////////////////////////////

Mat_ਵਿਅੰਜਕ::Mat_ਵਿਅੰਜਕ(ਅਚਲ Mat& m) : op(&g_Mat_Op_ਪਛਾਣ_ਤੱਤ), flags(0), a(m), b(Mat()), c(Mat()), alpha(1), beta(0), s(Scalar())
{
}

Mat_ਵਿਅੰਜਕ Mat_ਵਿਅੰਜਕ::ਪੰਕਤੀ(ਪੂਰਨ y) ਅਚਲ
{
    Mat_ਵਿਅੰਜਕ e;
    op->roi(*ਇਹ, Range(y, y+1), Range::all(), e);
    ਵਾਪਸ e;
}

Mat_ਵਿਅੰਜਕ Mat_ਵਿਅੰਜਕ::ਖਾਨਾ(ਪੂਰਨ x) ਅਚਲ
{
    Mat_ਵਿਅੰਜਕ e;
    op->roi(*ਇਹ, Range::all(), Range(x, x+1), e);
    ਵਾਪਸ e;
}

Mat_ਵਿਅੰਜਕ Mat_ਵਿਅੰਜਕ::diag(ਪੂਰਨ d) ਅਚਲ
{
    Mat_ਵਿਅੰਜਕ e;
    op->diag(*ਇਹ, d, e);
    ਵਾਪਸ e;
}

Mat_ਵਿਅੰਜਕ Mat_ਵਿਅੰਜਕ::ਸੰਚਾਲਕ()( ਅਚਲ Range& ਪੰਕਤੀ_Range_2, ਅਚਲ Range& ਖਾਨਾ_Range_2 ) ਅਚਲ
{
    Mat_ਵਿਅੰਜਕ e;
    op->roi(*ਇਹ, ਪੰਕਤੀ_Range_2, ਖਾਨਾ_Range_2, e);
    ਵਾਪਸ e;
}

Mat_ਵਿਅੰਜਕ Mat_ਵਿਅੰਜਕ::ਸੰਚਾਲਕ()( ਅਚਲ Rect& roi ) ਅਚਲ
{
    Mat_ਵਿਅੰਜਕ e;
    op->roi(*ਇਹ, Range(roi.y, roi.y + roi.ਉਚਾਈ), Range(roi.x, roi.x + roi.ਚੌੜਾਈ), e);
    ਵਾਪਸ e;
}

Mat Mat_ਵਿਅੰਜਕ::cross(ਅਚਲ Mat& m) ਅਚਲ
{
    ਵਾਪਸ ((Mat)*ਇਹ).cross(m);
}

ਦੁੱਗਣਾ Mat_ਵਿਅੰਜਕ::dot(ਅਚਲ Mat& m) ਅਚਲ
{
    ਵਾਪਸ ((Mat)*ਇਹ).dot(m);
}

Mat_ਵਿਅੰਜਕ Mat_ਵਿਅੰਜਕ::t() ਅਚਲ
{
    Mat_ਵਿਅੰਜਕ e;
    op->transpose(*ਇਹ, e);
    ਵਾਪਸ e;
}

Mat_ਵਿਅੰਜਕ Mat_ਵਿਅੰਜਕ::inv(ਪੂਰਨ ਵਿਧੀ) ਅਚਲ
{
    Mat_ਵਿਅੰਜਕ e;
    op->invert(*ਇਹ, ਵਿਧੀ, e);
    ਵਾਪਸ e;
}

Mat_ਵਿਅੰਜਕ Mat_ਵਿਅੰਜਕ::mul(ਅਚਲ Mat_ਵਿਅੰਜਕ& e, ਦੁੱਗਣਾ scale) ਅਚਲ
{
    Mat_ਵਿਅੰਜਕ en;
    op->multiply(*ਇਹ, e, en, scale);
    ਵਾਪਸ en;
}

Mat_ਵਿਅੰਜਕ Mat_ਵਿਅੰਜਕ::mul(ਅਚਲ Mat& m, ਦੁੱਗਣਾ scale) ਅਚਲ
{
    Mat_ਵਿਅੰਜਕ e;
    op->multiply(*ਇਹ, Mat_ਵਿਅੰਜਕ(m), e, scale);
    ਵਾਪਸ e;
}

Mat_ਵਿਅੰਜਕ ਸੰਚਾਲਕ + (ਅਚਲ Mat& a, ਅਚਲ Mat& b)
{
    checkOperandsExist(a, b);
    Mat_ਵਿਅੰਜਕ e;
    Mat_Op_ਜੋੜੋ_Ex::make_ਵਿਅੰਜਕ(e, a, b, 1, 1);
    ਵਾਪਸ e;
}

Mat_ਵਿਅੰਜਕ ਸੰਚਾਲਕ + (ਅਚਲ Mat& a, ਅਚਲ Scalar& s)
{
    checkOperandsExist(a);
    Mat_ਵਿਅੰਜਕ e;
    Mat_Op_ਜੋੜੋ_Ex::make_ਵਿਅੰਜਕ(e, a, Mat(), 1, 0, s);
    ਵਾਪਸ e;
}

Mat_ਵਿਅੰਜਕ ਸੰਚਾਲਕ + (ਅਚਲ Scalar& s, ਅਚਲ Mat& a)
{
    checkOperandsExist(a);
    Mat_ਵਿਅੰਜਕ e;
    Mat_Op_ਜੋੜੋ_Ex::make_ਵਿਅੰਜਕ(e, a, Mat(), 1, 0, s);
    ਵਾਪਸ e;
}

Mat_ਵਿਅੰਜਕ ਸੰਚਾਲਕ + (ਅਚਲ Mat_ਵਿਅੰਜਕ& e, ਅਚਲ Mat& m)
{
    Mat_ਵਿਅੰਜਕ en;
    e.op->ਜੋੜੋ(e, Mat_ਵਿਅੰਜਕ(m), en);
    ਵਾਪਸ en;
}

Mat_ਵਿਅੰਜਕ ਸੰਚਾਲਕ + (ਅਚਲ Mat& m, ਅਚਲ Mat_ਵਿਅੰਜਕ& e)
{
    checkOperandsExist(m);
    Mat_ਵਿਅੰਜਕ en;
    e.op->ਜੋੜੋ(e, Mat_ਵਿਅੰਜਕ(m), en);
    ਵਾਪਸ en;
}

Mat_ਵਿਅੰਜਕ ਸੰਚਾਲਕ + (ਅਚਲ Mat_ਵਿਅੰਜਕ& e, ਅਚਲ Scalar& s)
{
    Mat_ਵਿਅੰਜਕ en;
    e.op->ਜੋੜੋ(e, s, en);
    ਵਾਪਸ en;
}

Mat_ਵਿਅੰਜਕ ਸੰਚਾਲਕ + (ਅਚਲ Scalar& s, ਅਚਲ Mat_ਵਿਅੰਜਕ& e)
{
    Mat_ਵਿਅੰਜਕ en;
    e.op->ਜੋੜੋ(e, s, en);
    ਵਾਪਸ en;
}

Mat_ਵਿਅੰਜਕ ਸੰਚਾਲਕ + (ਅਚਲ Mat_ਵਿਅੰਜਕ& e1, ਅਚਲ Mat_ਵਿਅੰਜਕ& e2)
{
    Mat_ਵਿਅੰਜਕ en;
    e1.op->ਜੋੜੋ(e1, e2, en);
    ਵਾਪਸ en;
}

Mat_ਵਿਅੰਜਕ ਸੰਚਾਲਕ - (ਅਚਲ Mat& a, ਅਚਲ Mat& b)
{
    checkOperandsExist(a, b);
    Mat_ਵਿਅੰਜਕ e;
    Mat_Op_ਜੋੜੋ_Ex::make_ਵਿਅੰਜਕ(e, a, b, 1, -1);
    ਵਾਪਸ e;
}

Mat_ਵਿਅੰਜਕ ਸੰਚਾਲਕ - (ਅਚਲ Mat& a, ਅਚਲ Scalar& s)
{
    checkOperandsExist(a);
    Mat_ਵਿਅੰਜਕ e;
    Mat_Op_ਜੋੜੋ_Ex::make_ਵਿਅੰਜਕ(e, a, Mat(), 1, 0, -s);
    ਵਾਪਸ e;
}

Mat_ਵਿਅੰਜਕ ਸੰਚਾਲਕ - (ਅਚਲ Scalar& s, ਅਚਲ Mat& a)
{
    checkOperandsExist(a);
    Mat_ਵਿਅੰਜਕ e;
    Mat_Op_ਜੋੜੋ_Ex::make_ਵਿਅੰਜਕ(e, a, Mat(), -1, 0, s);
    ਵਾਪਸ e;
}

Mat_ਵਿਅੰਜਕ ਸੰਚਾਲਕ - (ਅਚਲ Mat_ਵਿਅੰਜਕ& e, ਅਚਲ Mat& m)
{
    checkOperandsExist(m);
    Mat_ਵਿਅੰਜਕ en;
    e.op->ਘਟਾਓ(e, Mat_ਵਿਅੰਜਕ(m), en);
    ਵਾਪਸ en;
}

Mat_ਵਿਅੰਜਕ ਸੰਚਾਲਕ - (ਅਚਲ Mat& m, ਅਚਲ Mat_ਵਿਅੰਜਕ& e)
{
    checkOperandsExist(m);
    Mat_ਵਿਅੰਜਕ en;
    e.op->ਘਟਾਓ(Mat_ਵਿਅੰਜਕ(m), e, en);
    ਵਾਪਸ en;
}

Mat_ਵਿਅੰਜਕ ਸੰਚਾਲਕ - (ਅਚਲ Mat_ਵਿਅੰਜਕ& e, ਅਚਲ Scalar& s)
{
    Mat_ਵਿਅੰਜਕ en;
    e.op->ਜੋੜੋ(e, -s, en);
    ਵਾਪਸ en;
}

Mat_ਵਿਅੰਜਕ ਸੰਚਾਲਕ - (ਅਚਲ Scalar& s, ਅਚਲ Mat_ਵਿਅੰਜਕ& e)
{
    Mat_ਵਿਅੰਜਕ en;
    e.op->ਘਟਾਓ(s, e, en);
    ਵਾਪਸ en;
}

Mat_ਵਿਅੰਜਕ ਸੰਚਾਲਕ - (ਅਚਲ Mat_ਵਿਅੰਜਕ& e1, ਅਚਲ Mat_ਵਿਅੰਜਕ& e2)
{
    Mat_ਵਿਅੰਜਕ en;
    e1.op->ਘਟਾਓ(e1, e2, en);
    ਵਾਪਸ en;
}

Mat_ਵਿਅੰਜਕ ਸੰਚਾਲਕ - (ਅਚਲ Mat& m)
{
    checkOperandsExist(m);
    Mat_ਵਿਅੰਜਕ e;
    Mat_Op_ਜੋੜੋ_Ex::make_ਵਿਅੰਜਕ(e, m, Mat(), -1, 0);
    ਵਾਪਸ e;
}

Mat_ਵਿਅੰਜਕ ਸੰਚਾਲਕ - (ਅਚਲ Mat_ਵਿਅੰਜਕ& e)
{
    Mat_ਵਿਅੰਜਕ en;
    e.op->ਘਟਾਓ(Scalar(0), e, en);
    ਵਾਪਸ en;
}

Mat_ਵਿਅੰਜਕ ਸੰਚਾਲਕ * (ਅਚਲ Mat& a, ਅਚਲ Mat& b)
{
    checkOperandsExist(a, b);
    Mat_ਵਿਅੰਜਕ e;
    MatOp_GEMM::make_ਵਿਅੰਜਕ(e, 0, a, b);
    ਵਾਪਸ e;
}

Mat_ਵਿਅੰਜਕ ਸੰਚਾਲਕ * (ਅਚਲ Mat& a, ਦੁੱਗਣਾ s)
{
    checkOperandsExist(a);
    Mat_ਵਿਅੰਜਕ e;
    Mat_Op_ਜੋੜੋ_Ex::make_ਵਿਅੰਜਕ(e, a, Mat(), s, 0);
    ਵਾਪਸ e;
}

Mat_ਵਿਅੰਜਕ ਸੰਚਾਲਕ * (ਦੁੱਗਣਾ s, ਅਚਲ Mat& a)
{
    checkOperandsExist(a);
    Mat_ਵਿਅੰਜਕ e;
    Mat_Op_ਜੋੜੋ_Ex::make_ਵਿਅੰਜਕ(e, a, Mat(), s, 0);
    ਵਾਪਸ e;
}

Mat_ਵਿਅੰਜਕ ਸੰਚਾਲਕ * (ਅਚਲ Mat_ਵਿਅੰਜਕ& e, ਅਚਲ Mat& m)
{
    Mat_ਵਿਅੰਜਕ en;
    e.op->matmul(e, Mat_ਵਿਅੰਜਕ(m), en);
    ਵਾਪਸ en;
}

Mat_ਵਿਅੰਜਕ ਸੰਚਾਲਕ * (ਅਚਲ Mat& m, ਅਚਲ Mat_ਵਿਅੰਜਕ& e)
{
    checkOperandsExist(m);
    Mat_ਵਿਅੰਜਕ en;
    e.op->matmul(Mat_ਵਿਅੰਜਕ(m), e, en);
    ਵਾਪਸ en;
}

Mat_ਵਿਅੰਜਕ ਸੰਚਾਲਕ * (ਅਚਲ Mat_ਵਿਅੰਜਕ& e, ਦੁੱਗਣਾ s)
{
    Mat_ਵਿਅੰਜਕ en;
    e.op->multiply(e, s, en);
    ਵਾਪਸ en;
}

Mat_ਵਿਅੰਜਕ ਸੰਚਾਲਕ * (ਦੁੱਗਣਾ s, ਅਚਲ Mat_ਵਿਅੰਜਕ& e)
{
    Mat_ਵਿਅੰਜਕ en;
    e.op->multiply(e, s, en);
    ਵਾਪਸ en;
}

Mat_ਵਿਅੰਜਕ ਸੰਚਾਲਕ * (ਅਚਲ Mat_ਵਿਅੰਜਕ& e1, ਅਚਲ Mat_ਵਿਅੰਜਕ& e2)
{
    Mat_ਵਿਅੰਜਕ en;
    e1.op->matmul(e1, e2, en);
    ਵਾਪਸ en;
}

Mat_ਵਿਅੰਜਕ ਸੰਚਾਲਕ / (ਅਚਲ Mat& a, ਅਚਲ Mat& b)
{
    checkOperandsExist(a, b);
    Mat_ਵਿਅੰਜਕ e;
    MatOp_Bin::make_ਵਿਅੰਜਕ(e, '/', a, b);
    ਵਾਪਸ e;
}

Mat_ਵਿਅੰਜਕ ਸੰਚਾਲਕ / (ਅਚਲ Mat& a, ਦੁੱਗਣਾ s)
{
    checkOperandsExist(a);
    Mat_ਵਿਅੰਜਕ e;
    Mat_Op_ਜੋੜੋ_Ex::make_ਵਿਅੰਜਕ(e, a, Mat(), 1./s, 0);
    ਵਾਪਸ e;
}

Mat_ਵਿਅੰਜਕ ਸੰਚਾਲਕ / (ਦੁੱਗਣਾ s, ਅਚਲ Mat& a)
{
    checkOperandsExist(a);
    Mat_ਵਿਅੰਜਕ e;
    MatOp_Bin::make_ਵਿਅੰਜਕ(e, '/', a, Mat(), s);
    ਵਾਪਸ e;
}

Mat_ਵਿਅੰਜਕ ਸੰਚਾਲਕ / (ਅਚਲ Mat_ਵਿਅੰਜਕ& e, ਅਚਲ Mat& m)
{
    Mat_ਵਿਅੰਜਕ en;
    e.op->divide(e, Mat_ਵਿਅੰਜਕ(m), en);
    ਵਾਪਸ en;
}

Mat_ਵਿਅੰਜਕ ਸੰਚਾਲਕ / (ਅਚਲ Mat& m, ਅਚਲ Mat_ਵਿਅੰਜਕ& e)
{
    checkOperandsExist(m);
    Mat_ਵਿਅੰਜਕ en;
    e.op->divide(Mat_ਵਿਅੰਜਕ(m), e, en);
    ਵਾਪਸ en;
}

Mat_ਵਿਅੰਜਕ ਸੰਚਾਲਕ / (ਅਚਲ Mat_ਵਿਅੰਜਕ& e, ਦੁੱਗਣਾ s)
{
    Mat_ਵਿਅੰਜਕ en;
    e.op->multiply(e, 1./s, en);
    ਵਾਪਸ en;
}

Mat_ਵਿਅੰਜਕ ਸੰਚਾਲਕ / (ਦੁੱਗਣਾ s, ਅਚਲ Mat_ਵਿਅੰਜਕ& e)
{
    Mat_ਵਿਅੰਜਕ en;
    e.op->divide(s, e, en);
    ਵਾਪਸ en;
}

Mat_ਵਿਅੰਜਕ ਸੰਚਾਲਕ / (ਅਚਲ Mat_ਵਿਅੰਜਕ& e1, ਅਚਲ Mat_ਵਿਅੰਜਕ& e2)
{
    Mat_ਵਿਅੰਜਕ en;
    e1.op->divide(e1, e2, en);
    ਵਾਪਸ en;
}

Mat_ਵਿਅੰਜਕ ਸੰਚਾਲਕ < (ਅਚਲ Mat& a, ਅਚਲ Mat& b)
{
    checkOperandsExist(a, b);
    Mat_ਵਿਅੰਜਕ e;
    MatOp_Cmp::make_ਵਿਅੰਜਕ(e, cv::CMP_LT, a, b);
    ਵਾਪਸ e;
}

Mat_ਵਿਅੰਜਕ ਸੰਚਾਲਕ < (ਅਚਲ Mat& a, ਦੁੱਗਣਾ s)
{
    checkOperandsExist(a);
    Mat_ਵਿਅੰਜਕ e;
    MatOp_Cmp::make_ਵਿਅੰਜਕ(e, cv::CMP_LT, a, s);
    ਵਾਪਸ e;
}

Mat_ਵਿਅੰਜਕ ਸੰਚਾਲਕ < (ਦੁੱਗਣਾ s, ਅਚਲ Mat& a)
{
    checkOperandsExist(a);
    Mat_ਵਿਅੰਜਕ e;
    MatOp_Cmp::make_ਵਿਅੰਜਕ(e, cv::CMP_GT, a, s);
    ਵਾਪਸ e;
}

Mat_ਵਿਅੰਜਕ ਸੰਚਾਲਕ <= (ਅਚਲ Mat& a, ਅਚਲ Mat& b)
{
    checkOperandsExist(a, b);
    Mat_ਵਿਅੰਜਕ e;
    MatOp_Cmp::make_ਵਿਅੰਜਕ(e, cv::CMP_LE, a, b);
    ਵਾਪਸ e;
}

Mat_ਵਿਅੰਜਕ ਸੰਚਾਲਕ <= (ਅਚਲ Mat& a, ਦੁੱਗਣਾ s)
{
    checkOperandsExist(a);
    Mat_ਵਿਅੰਜਕ e;
    MatOp_Cmp::make_ਵਿਅੰਜਕ(e, cv::CMP_LE, a, s);
    ਵਾਪਸ e;
}

Mat_ਵਿਅੰਜਕ ਸੰਚਾਲਕ <= (ਦੁੱਗਣਾ s, ਅਚਲ Mat& a)
{
    checkOperandsExist(a);
    Mat_ਵਿਅੰਜਕ e;
    MatOp_Cmp::make_ਵਿਅੰਜਕ(e, cv::CMP_GE, a, s);
    ਵਾਪਸ e;
}

Mat_ਵਿਅੰਜਕ ਸੰਚਾਲਕ == (ਅਚਲ Mat& a, ਅਚਲ Mat& b)
{
    checkOperandsExist(a, b);
    Mat_ਵਿਅੰਜਕ e;
    MatOp_Cmp::make_ਵਿਅੰਜਕ(e, cv::CMP_EQ, a, b);
    ਵਾਪਸ e;
}

Mat_ਵਿਅੰਜਕ ਸੰਚਾਲਕ == (ਅਚਲ Mat& a, ਦੁੱਗਣਾ s)
{
    checkOperandsExist(a);
    Mat_ਵਿਅੰਜਕ e;
    MatOp_Cmp::make_ਵਿਅੰਜਕ(e, cv::CMP_EQ, a, s);
    ਵਾਪਸ e;
}

Mat_ਵਿਅੰਜਕ ਸੰਚਾਲਕ == (ਦੁੱਗਣਾ s, ਅਚਲ Mat& a)
{
    checkOperandsExist(a);
    Mat_ਵਿਅੰਜਕ e;
    MatOp_Cmp::make_ਵਿਅੰਜਕ(e, cv::CMP_EQ, a, s);
    ਵਾਪਸ e;
}

Mat_ਵਿਅੰਜਕ ਸੰਚਾਲਕ != (ਅਚਲ Mat& a, ਅਚਲ Mat& b)
{
    checkOperandsExist(a, b);
    Mat_ਵਿਅੰਜਕ e;
    MatOp_Cmp::make_ਵਿਅੰਜਕ(e, cv::CMP_NE, a, b);
    ਵਾਪਸ e;
}

Mat_ਵਿਅੰਜਕ ਸੰਚਾਲਕ != (ਅਚਲ Mat& a, ਦੁੱਗਣਾ s)
{
    checkOperandsExist(a);
    Mat_ਵਿਅੰਜਕ e;
    MatOp_Cmp::make_ਵਿਅੰਜਕ(e, cv::CMP_NE, a, s);
    ਵਾਪਸ e;
}

Mat_ਵਿਅੰਜਕ ਸੰਚਾਲਕ != (ਦੁੱਗਣਾ s, ਅਚਲ Mat& a)
{
    checkOperandsExist(a);
    Mat_ਵਿਅੰਜਕ e;
    MatOp_Cmp::make_ਵਿਅੰਜਕ(e, cv::CMP_NE, a, s);
    ਵਾਪਸ e;
}

Mat_ਵਿਅੰਜਕ ਸੰਚਾਲਕ >= (ਅਚਲ Mat& a, ਅਚਲ Mat& b)
{
    checkOperandsExist(a, b);
    Mat_ਵਿਅੰਜਕ e;
    MatOp_Cmp::make_ਵਿਅੰਜਕ(e, cv::CMP_GE, a, b);
    ਵਾਪਸ e;
}

Mat_ਵਿਅੰਜਕ ਸੰਚਾਲਕ >= (ਅਚਲ Mat& a, ਦੁੱਗਣਾ s)
{
    checkOperandsExist(a);
    Mat_ਵਿਅੰਜਕ e;
    MatOp_Cmp::make_ਵਿਅੰਜਕ(e, cv::CMP_GE, a, s);
    ਵਾਪਸ e;
}

Mat_ਵਿਅੰਜਕ ਸੰਚਾਲਕ >= (ਦੁੱਗਣਾ s, ਅਚਲ Mat& a)
{
    checkOperandsExist(a);
    Mat_ਵਿਅੰਜਕ e;
    MatOp_Cmp::make_ਵਿਅੰਜਕ(e, cv::CMP_LE, a, s);
    ਵਾਪਸ e;
}

Mat_ਵਿਅੰਜਕ ਸੰਚਾਲਕ > (ਅਚਲ Mat& a, ਅਚਲ Mat& b)
{
    checkOperandsExist(a, b);
    Mat_ਵਿਅੰਜਕ e;
    MatOp_Cmp::make_ਵਿਅੰਜਕ(e, cv::CMP_GT, a, b);
    ਵਾਪਸ e;
}

Mat_ਵਿਅੰਜਕ ਸੰਚਾਲਕ > (ਅਚਲ Mat& a, ਦੁੱਗਣਾ s)
{
    checkOperandsExist(a);
    Mat_ਵਿਅੰਜਕ e;
    MatOp_Cmp::make_ਵਿਅੰਜਕ(e, cv::CMP_GT, a, s);
    ਵਾਪਸ e;
}

Mat_ਵਿਅੰਜਕ ਸੰਚਾਲਕ > (ਦੁੱਗਣਾ s, ਅਚਲ Mat& a)
{
    checkOperandsExist(a);
    Mat_ਵਿਅੰਜਕ e;
    MatOp_Cmp::make_ਵਿਅੰਜਕ(e, cv::CMP_LT, a, s);
    ਵਾਪਸ e;
}

Mat_ਵਿਅੰਜਕ min(ਅਚਲ Mat& a, ਅਚਲ Mat& b)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a, b);
    Mat_ਵਿਅੰਜਕ e;
    MatOp_Bin::make_ਵਿਅੰਜਕ(e, 'm', a, b);
    ਵਾਪਸ e;
}

Mat_ਵਿਅੰਜਕ min(ਅਚਲ Mat& a, ਦੁੱਗਣਾ s)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    Mat_ਵਿਅੰਜਕ e;
    MatOp_Bin::make_ਵਿਅੰਜਕ(e, 'n', a, s);
    ਵਾਪਸ e;
}

Mat_ਵਿਅੰਜਕ min(ਦੁੱਗਣਾ s, ਅਚਲ Mat& a)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    Mat_ਵਿਅੰਜਕ e;
    MatOp_Bin::make_ਵਿਅੰਜਕ(e, 'n', a, s);
    ਵਾਪਸ e;
}

Mat_ਵਿਅੰਜਕ max(ਅਚਲ Mat& a, ਅਚਲ Mat& b)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a, b);
    Mat_ਵਿਅੰਜਕ e;
    MatOp_Bin::make_ਵਿਅੰਜਕ(e, 'M', a, b);
    ਵਾਪਸ e;
}

Mat_ਵਿਅੰਜਕ max(ਅਚਲ Mat& a, ਦੁੱਗਣਾ s)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    Mat_ਵਿਅੰਜਕ e;
    MatOp_Bin::make_ਵਿਅੰਜਕ(e, 'N', a, s);
    ਵਾਪਸ e;
}

Mat_ਵਿਅੰਜਕ max(ਦੁੱਗਣਾ s, ਅਚਲ Mat& a)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    Mat_ਵਿਅੰਜਕ e;
    MatOp_Bin::make_ਵਿਅੰਜਕ(e, 'N', a, s);
    ਵਾਪਸ e;
}

Mat_ਵਿਅੰਜਕ ਸੰਚਾਲਕ & (ਅਚਲ Mat& a, ਅਚਲ Mat& b)
{
    checkOperandsExist(a, b);
    Mat_ਵਿਅੰਜਕ e;
    MatOp_Bin::make_ਵਿਅੰਜਕ(e, '&', a, b);
    ਵਾਪਸ e;
}

Mat_ਵਿਅੰਜਕ ਸੰਚਾਲਕ & (ਅਚਲ Mat& a, ਅਚਲ Scalar& s)
{
    checkOperandsExist(a);
    Mat_ਵਿਅੰਜਕ e;
    MatOp_Bin::make_ਵਿਅੰਜਕ(e, '&', a, s);
    ਵਾਪਸ e;
}

Mat_ਵਿਅੰਜਕ ਸੰਚਾਲਕ & (ਅਚਲ Scalar& s, ਅਚਲ Mat& a)
{
    checkOperandsExist(a);
    Mat_ਵਿਅੰਜਕ e;
    MatOp_Bin::make_ਵਿਅੰਜਕ(e, '&', a, s);
    ਵਾਪਸ e;
}

Mat_ਵਿਅੰਜਕ ਸੰਚਾਲਕ | (ਅਚਲ Mat& a, ਅਚਲ Mat& b)
{
    checkOperandsExist(a, b);
    Mat_ਵਿਅੰਜਕ e;
    MatOp_Bin::make_ਵਿਅੰਜਕ(e, '|', a, b);
    ਵਾਪਸ e;
}

Mat_ਵਿਅੰਜਕ ਸੰਚਾਲਕ | (ਅਚਲ Mat& a, ਅਚਲ Scalar& s)
{
    checkOperandsExist(a);
    Mat_ਵਿਅੰਜਕ e;
    MatOp_Bin::make_ਵਿਅੰਜਕ(e, '|', a, s);
    ਵਾਪਸ e;
}

Mat_ਵਿਅੰਜਕ ਸੰਚਾਲਕ | (ਅਚਲ Scalar& s, ਅਚਲ Mat& a)
{
    checkOperandsExist(a);
    Mat_ਵਿਅੰਜਕ e;
    MatOp_Bin::make_ਵਿਅੰਜਕ(e, '|', a, s);
    ਵਾਪਸ e;
}

Mat_ਵਿਅੰਜਕ ਸੰਚਾਲਕ ^ (ਅਚਲ Mat& a, ਅਚਲ Mat& b)
{
    checkOperandsExist(a, b);
    Mat_ਵਿਅੰਜਕ e;
    MatOp_Bin::make_ਵਿਅੰਜਕ(e, '^', a, b);
    ਵਾਪਸ e;
}

Mat_ਵਿਅੰਜਕ ਸੰਚਾਲਕ ^ (ਅਚਲ Mat& a, ਅਚਲ Scalar& s)
{
    checkOperandsExist(a);
    Mat_ਵਿਅੰਜਕ e;
    MatOp_Bin::make_ਵਿਅੰਜਕ(e, '^', a, s);
    ਵਾਪਸ e;
}

Mat_ਵਿਅੰਜਕ ਸੰਚਾਲਕ ^ (ਅਚਲ Scalar& s, ਅਚਲ Mat& a)
{
    checkOperandsExist(a);
    Mat_ਵਿਅੰਜਕ e;
    MatOp_Bin::make_ਵਿਅੰਜਕ(e, '^', a, s);
    ਵਾਪਸ e;
}

Mat_ਵਿਅੰਜਕ ਸੰਚਾਲਕ ~(ਅਚਲ Mat& a)
{
    checkOperandsExist(a);
    Mat_ਵਿਅੰਜਕ e;
    MatOp_Bin::make_ਵਿਅੰਜਕ(e, '~', a, Scalar());
    ਵਾਪਸ e;
}

Mat_ਵਿਅੰਜਕ ਨਿਰਪੇਖ(ਅਚਲ Mat& a)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    Mat_ਵਿਅੰਜਕ e;
    MatOp_Bin::make_ਵਿਅੰਜਕ(e, 'a', a, Scalar());
    ਵਾਪਸ e;
}

Mat_ਵਿਅੰਜਕ ਨਿਰਪੇਖ(ਅਚਲ Mat_ਵਿਅੰਜਕ& e)
{
    CV_INSTRUMENT_REGION();

    Mat_ਵਿਅੰਜਕ en;
    e.op->ਨਿਰਪੇਖ(e, en);
    ਵਾਪਸ en;
}


ਮਾਪ Mat_ਵਿਅੰਜਕ::size() ਅਚਲ
{
    ਜੇ( isT(*ਇਹ) || isInv(*ਇਹ) )
        ਵਾਪਸ ਮਾਪ(a.ਪੰਕਤੀਆਂ, a.ਖਾਨੇ_ਗਿਣਤੀ);
    ਜੇ( isSolve(*ਇਹ) )
        ਵਾਪਸ ਮਾਪ(b.ਖਾਨੇ_ਗਿਣਤੀ, a.ਖਾਨੇ_ਗਿਣਤੀ);
    ਜੇ( isInitializer(*ਇਹ) )
        ਵਾਪਸ a.size();
    ਵਾਪਸ op ? op->size(*ਇਹ) : ਮਾਪ();
}


ਪੂਰਨ Mat_ਵਿਅੰਜਕ::type() ਅਚਲ
{
    CV_INSTRUMENT_REGION();

    ਜੇ( isInitializer(*ਇਹ) )
        ਵਾਪਸ a.type();
    ਜੇ( isCmp(*ਇਹ) )
        ਵਾਪਸ CV_MAKETYPE(CV_8U, a.channels());
    ਵਾਪਸ op ? op->type(*ਇਹ) : -1;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////

ਖਾਲੀ Mat_Op_ਪਛਾਣ_ਤੱਤ::assign(ਅਚਲ Mat_ਵਿਅੰਜਕ& e, Mat& m, ਪੂਰਨ _type) ਅਚਲ
{
    ਜੇ( _type == -1 || _type == e.a.type() )
        m = e.a;
    ਨਹੀਂਤਾਂ
    {
        CV_Assert( CV_MAT_CN(_type) == e.a.channels() );
        e.a.convert_ਮੰਜ਼ਿਲ(m, _type);
    }
}

ਸਤਰਅੰਦਰ ਖਾਲੀ Mat_Op_ਪਛਾਣ_ਤੱਤ::make_ਵਿਅੰਜਕ(Mat_ਵਿਅੰਜਕ& res, ਅਚਲ Mat& m)
{
    res = Mat_ਵਿਅੰਜਕ(&g_Mat_Op_ਪਛਾਣ_ਤੱਤ, 0, m, Mat(), Mat(), 1, 0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

ਖਾਲੀ Mat_Op_ਜੋੜੋ_Ex::assign(ਅਚਲ Mat_ਵਿਅੰਜਕ& e, Mat& m, ਪੂਰਨ _type) ਅਚਲ
{
    Mat ਆਰਜ਼ੀ_2, &dst = _type == -1 || e.a.type() == _type ? m : ਆਰਜ਼ੀ_2;
    ਜੇ( e.b.ਅੰਕੜੇ )
    {
        ਜੇ( e.s == Scalar() || !e.s.isReal() )
        {
            ਜੇ( e.alpha == 1 )
            {
                ਜੇ( e.beta == 1 )
                    cv::ਜੋੜੋ(e.a, e.b, dst);
                ਨਹੀਂਤਾਂ ਜੇ( e.beta == -1 )
                    cv::ਘਟਾਓ(e.a, e.b, dst);
                ਨਹੀਂਤਾਂ
                    cv::scale_ਜੋੜੋ(e.b, e.beta, e.a, dst);
            }
            ਨਹੀਂਤਾਂ ਜੇ( e.beta == 1 )
            {
                ਜੇ( e.alpha == -1 )
                    cv::ਘਟਾਓ(e.b, e.a, dst);
                ਨਹੀਂਤਾਂ
                    cv::scale_ਜੋੜੋ(e.a, e.alpha, e.b, dst);
            }
            ਨਹੀਂਤਾਂ
                cv::ਜੋੜੋ_Weighted(e.a, e.alpha, e.b, e.beta, 0, dst);

            ਜੇ( !e.s.isReal() )
                cv::ਜੋੜੋ(dst, e.s, dst);
        }
        ਨਹੀਂਤਾਂ
        {
            ਜੇ (e.a.channels() > 1)
                CV_LOG_ONCE_WARNING(ਨਿਰਾ, "OpenCV/MatExpr: processing of multi-channel arrays might be changed in the future: "
                                          "https://github.com/opencv/opencv/issues/16739");
            cv::ਜੋੜੋ_Weighted(e.a, e.alpha, e.b, e.beta, e.s[0], dst);
        }
    }
    ਨਹੀਂਤਾਂ ਜੇ( e.s.isReal() && (dst.ਅੰਕੜੇ != m.ਅੰਕੜੇ || fabs(e.alpha) != 1))
    {
        ਜੇ (e.a.channels() > 1 && e.s[0] != 0.0)
            CV_LOG_ONCE_WARNING(ਨਿਰਾ, "OpenCV/MatExpr: processing of multi-channel arrays might be changed in the future: "
                                      "https://github.com/opencv/opencv/issues/16739");
        e.a.convert_ਮੰਜ਼ਿਲ(m, _type, e.alpha, e.s[0]);
        ਵਾਪਸ;
    }
    ਨਹੀਂਤਾਂ ਜੇ( e.alpha == 1 )
        cv::ਜੋੜੋ(e.a, e.s, dst);
    ਨਹੀਂਤਾਂ ਜੇ( e.alpha == -1 )
        cv::ਘਟਾਓ(e.s, e.a, dst);
    ਨਹੀਂਤਾਂ
    {
        e.a.convert_ਮੰਜ਼ਿਲ(dst, e.a.type(), e.alpha);
        cv::ਜੋੜੋ(dst, e.s, dst);
    }

    ਜੇ( dst.ਅੰਕੜੇ != m.ਅੰਕੜੇ )
        dst.convert_ਮੰਜ਼ਿਲ(m, m.type());
}


ਖਾਲੀ Mat_Op_ਜੋੜੋ_Ex::ਜੋੜੋ(ਅਚਲ Mat_ਵਿਅੰਜਕ& e, ਅਚਲ Scalar& s, Mat_ਵਿਅੰਜਕ& res) ਅਚਲ
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.s += s;
}


ਖਾਲੀ Mat_Op_ਜੋੜੋ_Ex::ਘਟਾਓ(ਅਚਲ Scalar& s, ਅਚਲ Mat_ਵਿਅੰਜਕ& e, Mat_ਵਿਅੰਜਕ& res) ਅਚਲ
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha = -res.alpha;
    res.beta = -res.beta;
    res.s = s - res.s;
}

ਖਾਲੀ Mat_Op_ਜੋੜੋ_Ex::multiply(ਅਚਲ Mat_ਵਿਅੰਜਕ& e, ਦੁੱਗਣਾ s, Mat_ਵਿਅੰਜਕ& res) ਅਚਲ
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha *= s;
    res.beta *= s;
    res.s *= s;
}

ਖਾਲੀ Mat_Op_ਜੋੜੋ_Ex::divide(ਦੁੱਗਣਾ s, ਅਚਲ Mat_ਵਿਅੰਜਕ& e, Mat_ਵਿਅੰਜਕ& res) ਅਚਲ
{
    CV_INSTRUMENT_REGION();

    ਜੇ( isScaled(e) )
        MatOp_Bin::make_ਵਿਅੰਜਕ(res, '/', e.a, Mat(), s/e.alpha);
    ਨਹੀਂਤਾਂ
        MatOp::divide(s, e, res);
}


ਖਾਲੀ Mat_Op_ਜੋੜੋ_Ex::transpose(ਅਚਲ Mat_ਵਿਅੰਜਕ& e, Mat_ਵਿਅੰਜਕ& res) ਅਚਲ
{
    CV_INSTRUMENT_REGION();

    ਜੇ( isScaled(e) )
        MatOp_T::make_ਵਿਅੰਜਕ(res, e.a, e.alpha);
    ਨਹੀਂਤਾਂ
        MatOp::transpose(e, res);
}

ਖਾਲੀ Mat_Op_ਜੋੜੋ_Ex::ਨਿਰਪੇਖ(ਅਚਲ Mat_ਵਿਅੰਜਕ& e, Mat_ਵਿਅੰਜਕ& res) ਅਚਲ
{
    CV_INSTRUMENT_REGION();

    ਜੇ( (!e.b.ਅੰਕੜੇ || e.beta == 0) && fabs(e.alpha) == 1 )
        MatOp_Bin::make_ਵਿਅੰਜਕ(res, 'a', e.a, -e.s*e.alpha);
    ਨਹੀਂਤਾਂ ਜੇ( e.b.ਅੰਕੜੇ && e.alpha + e.beta == 0 && e.alpha*e.beta == -1 )
        MatOp_Bin::make_ਵਿਅੰਜਕ(res, 'a', e.a, e.b);
    ਨਹੀਂਤਾਂ
        MatOp::ਨਿਰਪੇਖ(e, res);
}

ਸਤਰਅੰਦਰ ਖਾਲੀ Mat_Op_ਜੋੜੋ_Ex::make_ਵਿਅੰਜਕ(Mat_ਵਿਅੰਜਕ& res, ਅਚਲ Mat& a, ਅਚਲ Mat& b, ਦੁੱਗਣਾ alpha, ਦੁੱਗਣਾ beta, ਅਚਲ Scalar& s)
{
    res = Mat_ਵਿਅੰਜਕ(&g_Mat_Op_ਜੋੜੋ_Ex, 0, a, b, Mat(), alpha, beta, s);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ਖਾਲੀ MatOp_Bin::assign(ਅਚਲ Mat_ਵਿਅੰਜਕ& e, Mat& m, ਪੂਰਨ _type) ਅਚਲ
{
    Mat ਆਰਜ਼ੀ_2, &dst = _type == -1 || e.a.type() == _type ? m : ਆਰਜ਼ੀ_2;

    ਜੇ( e.flags == '*' )
        cv::multiply(e.a, e.b, dst, e.alpha);
    ਨਹੀਂਤਾਂ ਜੇ( e.flags == '/' && e.b.ਅੰਕੜੇ )
        cv::divide(e.a, e.b, dst, e.alpha);
    ਨਹੀਂਤਾਂ ਜੇ( e.flags == '/' && !e.b.ਅੰਕੜੇ )
        cv::divide(e.alpha, e.a, dst );
    ਨਹੀਂਤਾਂ ਜੇ( e.flags == '&' && e.b.ਅੰਕੜੇ )
        bitwise_and(e.a, e.b, dst);
    ਨਹੀਂਤਾਂ ਜੇ( e.flags == '&' && !e.b.ਅੰਕੜੇ )
        bitwise_and(e.a, e.s, dst);
    ਨਹੀਂਤਾਂ ਜੇ( e.flags == '|' && e.b.ਅੰਕੜੇ )
        bitwise_or(e.a, e.b, dst);
    ਨਹੀਂਤਾਂ ਜੇ( e.flags == '|' && !e.b.ਅੰਕੜੇ )
        bitwise_or(e.a, e.s, dst);
    ਨਹੀਂਤਾਂ ਜੇ( e.flags == '^' && e.b.ਅੰਕੜੇ )
        bitwise_xor(e.a, e.b, dst);
    ਨਹੀਂਤਾਂ ਜੇ( e.flags == '^' && !e.b.ਅੰਕੜੇ )
        bitwise_xor(e.a, e.s, dst);
    ਨਹੀਂਤਾਂ ਜੇ( e.flags == '~' && !e.b.ਅੰਕੜੇ )
        bitwise_not(e.a, dst);
    ਨਹੀਂਤਾਂ ਜੇ( e.flags == 'm' )
        cv::min(e.a, e.b, dst);
    ਨਹੀਂਤਾਂ ਜੇ( e.flags == 'n' )
        cv::min(e.a, e.s[0], dst);
    ਨਹੀਂਤਾਂ ਜੇ( e.flags == 'M' )
        cv::max(e.a, e.b, dst);
    ਨਹੀਂਤਾਂ ਜੇ( e.flags == 'N' )
        cv::max(e.a, e.s[0], dst);
    ਨਹੀਂਤਾਂ ਜੇ( e.flags == 'a' && e.b.ਅੰਕੜੇ )
        cv::absdiff(e.a, e.b, dst);
    ਨਹੀਂਤਾਂ ਜੇ( e.flags == 'a' && !e.b.ਅੰਕੜੇ )
        cv::absdiff(e.a, e.s, dst);
    ਨਹੀਂਤਾਂ
        CV_Error(cv::Error::StsError, "Unknown operation");

    ਜੇ( dst.ਅੰਕੜੇ != m.ਅੰਕੜੇ )
        dst.convert_ਮੰਜ਼ਿਲ(m, _type);
}

ਖਾਲੀ MatOp_Bin::multiply(ਅਚਲ Mat_ਵਿਅੰਜਕ& e, ਦੁੱਗਣਾ s, Mat_ਵਿਅੰਜਕ& res) ਅਚਲ
{
    CV_INSTRUMENT_REGION();

    ਜੇ( e.flags == '*' || e.flags == '/' )
    {
        res = e;
        res.alpha *= s;
    }
    ਨਹੀਂਤਾਂ
        MatOp::multiply(e, s, res);
}

ਖਾਲੀ MatOp_Bin::divide(ਦੁੱਗਣਾ s, ਅਚਲ Mat_ਵਿਅੰਜਕ& e, Mat_ਵਿਅੰਜਕ& res) ਅਚਲ
{
    CV_INSTRUMENT_REGION();

    ਜੇ( e.flags == '/' && (!e.b.ਅੰਕੜੇ || e.beta == 0) )
        Mat_Op_ਜੋੜੋ_Ex::make_ਵਿਅੰਜਕ(res, e.a, Mat(), s/e.alpha, 0);
    ਨਹੀਂਤਾਂ
        MatOp::divide(s, e, res);
}

ਸਤਰਅੰਦਰ ਖਾਲੀ MatOp_Bin::make_ਵਿਅੰਜਕ(Mat_ਵਿਅੰਜਕ& res, ਅੱਖਰ op, ਅਚਲ Mat& a, ਅਚਲ Mat& b, ਦੁੱਗਣਾ scale)
{
    res = Mat_ਵਿਅੰਜਕ(&g_MatOp_Bin, op, a, b, Mat(), scale, b.ਅੰਕੜੇ ? 1 : 0);
}

ਸਤਰਅੰਦਰ ਖਾਲੀ MatOp_Bin::make_ਵਿਅੰਜਕ(Mat_ਵਿਅੰਜਕ& res, ਅੱਖਰ op, ਅਚਲ Mat& a, ਅਚਲ Scalar& s)
{
    res = Mat_ਵਿਅੰਜਕ(&g_MatOp_Bin, op, a, Mat(), Mat(), 1, 0, s);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

ਖਾਲੀ MatOp_Cmp::assign(ਅਚਲ Mat_ਵਿਅੰਜਕ& e, Mat& m, ਪੂਰਨ _type) ਅਚਲ
{
    Mat ਆਰਜ਼ੀ_2, &dst = _type == -1 || _type == CV_8U ? m : ਆਰਜ਼ੀ_2;

    ਜੇ( e.b.ਅੰਕੜੇ )
        cv::compare(e.a, e.b, dst, e.flags);
    ਨਹੀਂਤਾਂ
        cv::compare(e.a, e.alpha, dst, e.flags);

    ਜੇ( dst.ਅੰਕੜੇ != m.ਅੰਕੜੇ )
        dst.convert_ਮੰਜ਼ਿਲ(m, _type);
}

ਸਤਰਅੰਦਰ ਖਾਲੀ MatOp_Cmp::make_ਵਿਅੰਜਕ(Mat_ਵਿਅੰਜਕ& res, ਪੂਰਨ cmpop, ਅਚਲ Mat& a, ਅਚਲ Mat& b)
{
    res = Mat_ਵਿਅੰਜਕ(&g_MatOp_Cmp, cmpop, a, b, Mat(), 1, 1);
}

ਸਤਰਅੰਦਰ ਖਾਲੀ MatOp_Cmp::make_ਵਿਅੰਜਕ(Mat_ਵਿਅੰਜਕ& res, ਪੂਰਨ cmpop, ਅਚਲ Mat& a, ਦੁੱਗਣਾ alpha)
{
    res = Mat_ਵਿਅੰਜਕ(&g_MatOp_Cmp, cmpop, a, Mat(), Mat(), alpha, 1);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

ਖਾਲੀ MatOp_T::assign(ਅਚਲ Mat_ਵਿਅੰਜਕ& e, Mat& m, ਪੂਰਨ _type) ਅਚਲ
{
    Mat ਆਰਜ਼ੀ_2, &dst = _type == -1 || _type == e.a.type() ? m : ਆਰਜ਼ੀ_2;

    cv::transpose(e.a, dst);

    ਜੇ( dst.ਅੰਕੜੇ != m.ਅੰਕੜੇ || e.alpha != 1 )
        dst.convert_ਮੰਜ਼ਿਲ(m, _type, e.alpha);
}

ਖਾਲੀ MatOp_T::multiply(ਅਚਲ Mat_ਵਿਅੰਜਕ& e, ਦੁੱਗਣਾ s, Mat_ਵਿਅੰਜਕ& res) ਅਚਲ
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha *= s;
}

ਖਾਲੀ MatOp_T::transpose(ਅਚਲ Mat_ਵਿਅੰਜਕ& e, Mat_ਵਿਅੰਜਕ& res) ਅਚਲ
{
    CV_INSTRUMENT_REGION();

    ਜੇ( e.alpha == 1 )
        Mat_Op_ਪਛਾਣ_ਤੱਤ::make_ਵਿਅੰਜਕ(res, e.a);
    ਨਹੀਂਤਾਂ
        Mat_Op_ਜੋੜੋ_Ex::make_ਵਿਅੰਜਕ(res, e.a, Mat(), e.alpha, 0);
}

ਸਤਰਅੰਦਰ ਖਾਲੀ MatOp_T::make_ਵਿਅੰਜਕ(Mat_ਵਿਅੰਜਕ& res, ਅਚਲ Mat& a, ਦੁੱਗਣਾ alpha)
{
    res = Mat_ਵਿਅੰਜਕ(&g_MatOp_T, 0, a, Mat(), Mat(), alpha, 0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

ਖਾਲੀ MatOp_GEMM::assign(ਅਚਲ Mat_ਵਿਅੰਜਕ& e, Mat& m, ਪੂਰਨ _type) ਅਚਲ
{
    Mat ਆਰਜ਼ੀ_2, &dst = _type == -1 || _type == e.a.type() ? m : ਆਰਜ਼ੀ_2;

    cv::gemm(e.a, e.b, e.alpha, e.c, e.beta, dst, e.flags);
    ਜੇ( dst.ਅੰਕੜੇ != m.ਅੰਕੜੇ )
        dst.convert_ਮੰਜ਼ਿਲ(m, _type);
}

ਖਾਲੀ MatOp_GEMM::ਜੋੜੋ(ਅਚਲ Mat_ਵਿਅੰਜਕ& e1, ਅਚਲ Mat_ਵਿਅੰਜਕ& e2, Mat_ਵਿਅੰਜਕ& res) ਅਚਲ
{
    CV_INSTRUMENT_REGION();

    ਤਾਰਕਿਕ i1 = is_ਪਛਾਣ_ਤੱਤ(e1), i2 = is_ਪਛਾਣ_ਤੱਤ(e2);
    ਦੁੱਗਣਾ alpha1 = i1 ? 1 : e1.alpha, alpha2 = i2 ? 1 : e2.alpha;

    ਜੇ( isMatProd(e1) && (i2 || isScaled(e2) || isT(e2)) )
        MatOp_GEMM::make_ਵਿਅੰਜਕ(res, (e1.flags & ~cv::GEMM_3_T)|(isT(e2) ? cv::GEMM_3_T : 0),
                             e1.a, e1.b, alpha1, e2.a, alpha2);
    ਨਹੀਂਤਾਂ ਜੇ( isMatProd(e2) && (i1 || isScaled(e1) || isT(e1)) )
        MatOp_GEMM::make_ਵਿਅੰਜਕ(res, (e2.flags & ~cv::GEMM_3_T)|(isT(e1) ? cv::GEMM_3_T : 0),
                             e2.a, e2.b, alpha2, e1.a, alpha1);
    ਨਹੀਂਤਾਂ ਜੇ( ਇਹ == e2.op )
        MatOp::ਜੋੜੋ(e1, e2, res);
    ਨਹੀਂਤਾਂ
        e2.op->ਜੋੜੋ(e1, e2, res);
}

ਖਾਲੀ MatOp_GEMM::ਘਟਾਓ(ਅਚਲ Mat_ਵਿਅੰਜਕ& e1, ਅਚਲ Mat_ਵਿਅੰਜਕ& e2, Mat_ਵਿਅੰਜਕ& res) ਅਚਲ
{
    CV_INSTRUMENT_REGION();

    ਤਾਰਕਿਕ i1 = is_ਪਛਾਣ_ਤੱਤ(e1), i2 = is_ਪਛਾਣ_ਤੱਤ(e2);
    ਦੁੱਗਣਾ alpha1 = i1 ? 1 : e1.alpha, alpha2 = i2 ? 1 : e2.alpha;

    ਜੇ( isMatProd(e1) && (i2 || isScaled(e2) || isT(e2)) )
        MatOp_GEMM::make_ਵਿਅੰਜਕ(res, (e1.flags & ~cv::GEMM_3_T)|(isT(e2) ? cv::GEMM_3_T : 0),
                             e1.a, e1.b, alpha1, e2.a, -alpha2);
    ਨਹੀਂਤਾਂ ਜੇ( isMatProd(e2) && (i1 || isScaled(e1) || isT(e1)) )
        MatOp_GEMM::make_ਵਿਅੰਜਕ(res, (e2.flags & ~cv::GEMM_3_T)|(isT(e1) ? cv::GEMM_3_T : 0),
                            e2.a, e2.b, -alpha2, e1.a, alpha1);
    ਨਹੀਂਤਾਂ ਜੇ( ਇਹ == e2.op )
        MatOp::ਘਟਾਓ(e1, e2, res);
    ਨਹੀਂਤਾਂ
        e2.op->ਘਟਾਓ(e1, e2, res);
}

ਖਾਲੀ MatOp_GEMM::multiply(ਅਚਲ Mat_ਵਿਅੰਜਕ& e, ਦੁੱਗਣਾ s, Mat_ਵਿਅੰਜਕ& res) ਅਚਲ
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha *= s;
    res.beta *= s;
}

ਖਾਲੀ MatOp_GEMM::transpose(ਅਚਲ Mat_ਵਿਅੰਜਕ& e, Mat_ਵਿਅੰਜਕ& res) ਅਚਲ
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.flags = (!(e.flags & cv::GEMM_1_T) ? cv::GEMM_2_T : 0) |
                (!(e.flags & cv::GEMM_2_T) ? cv::GEMM_1_T : 0) |
                (!(e.flags & cv::GEMM_3_T) ? cv::GEMM_3_T : 0);
    swap(res.a, res.b);
}

ਸਤਰਅੰਦਰ ਖਾਲੀ MatOp_GEMM::make_ਵਿਅੰਜਕ(Mat_ਵਿਅੰਜਕ& res, ਪੂਰਨ flags, ਅਚਲ Mat& a, ਅਚਲ Mat& b,
                                 ਦੁੱਗਣਾ alpha, ਅਚਲ Mat& c, ਦੁੱਗਣਾ beta)
{
    res = Mat_ਵਿਅੰਜਕ(&g_MatOp_GEMM, flags, a, b, c, alpha, beta);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

ਖਾਲੀ MatOp_Invert::assign(ਅਚਲ Mat_ਵਿਅੰਜਕ& e, Mat& m, ਪੂਰਨ _type) ਅਚਲ
{
    Mat ਆਰਜ਼ੀ_2, &dst = _type == -1 || _type == e.a.type() ? m : ਆਰਜ਼ੀ_2;

    cv::invert(e.a, dst, e.flags);
    ਜੇ( dst.ਅੰਕੜੇ != m.ਅੰਕੜੇ )
        dst.convert_ਮੰਜ਼ਿਲ(m, _type);
}

ਖਾਲੀ MatOp_Invert::matmul(ਅਚਲ Mat_ਵਿਅੰਜਕ& e1, ਅਚਲ Mat_ਵਿਅੰਜਕ& e2, Mat_ਵਿਅੰਜਕ& res) ਅਚਲ
{
    ਜੇ( isInv(e1) && is_ਪਛਾਣ_ਤੱਤ(e2) )
        MatOp_Solve::make_ਵਿਅੰਜਕ(res, e1.flags, e1.a, e2.a);
    ਨਹੀਂਤਾਂ ਜੇ( ਇਹ == e2.op )
        MatOp::matmul(e1, e2, res);
    ਨਹੀਂਤਾਂ
        e2.op->matmul(e1, e2, res);
}

ਸਤਰਅੰਦਰ ਖਾਲੀ MatOp_Invert::make_ਵਿਅੰਜਕ(Mat_ਵਿਅੰਜਕ& res, ਪੂਰਨ ਵਿਧੀ, ਅਚਲ Mat& m)
{
    res = Mat_ਵਿਅੰਜਕ(&g_MatOp_Invert, ਵਿਧੀ, m, Mat(), Mat(), 1, 0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

ਖਾਲੀ MatOp_Solve::assign(ਅਚਲ Mat_ਵਿਅੰਜਕ& e, Mat& m, ਪੂਰਨ _type) ਅਚਲ
{
    Mat ਆਰਜ਼ੀ_2, &dst = _type == -1 || _type == e.a.type() ? m : ਆਰਜ਼ੀ_2;

    cv::solve(e.a, e.b, dst, e.flags);
    ਜੇ( dst.ਅੰਕੜੇ != m.ਅੰਕੜੇ )
        dst.convert_ਮੰਜ਼ਿਲ(m, _type);
}

ਸਤਰਅੰਦਰ ਖਾਲੀ MatOp_Solve::make_ਵਿਅੰਜਕ(Mat_ਵਿਅੰਜਕ& res, ਪੂਰਨ ਵਿਧੀ, ਅਚਲ Mat& a, ਅਚਲ Mat& b)
{
    res = Mat_ਵਿਅੰਜਕ(&g_MatOp_Solve, ਵਿਧੀ, a, b, Mat(), 1, 1);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

ਖਾਲੀ MatOp_Initializer::assign(ਅਚਲ Mat_ਵਿਅੰਜਕ& e, Mat& m, ਪੂਰਨ _type) ਅਚਲ
{
    ਜੇ( _type == -1 )
        _type = e.a.type();

    m.create(e.a.size, _type);

    ਜੇ( e.flags == 'I' && e.a.dims <= 2 )
        ਸੈੱਟ_ਪਛਾਣ_ਤੱਤ(m, Scalar(e.alpha));
    ਨਹੀਂਤਾਂ ਜੇ( e.flags == '0' )
        m = Scalar();
    ਨਹੀਂਤਾਂ ਜੇ( e.flags == '1' )
        m = Scalar(e.alpha);
    ਨਹੀਂਤਾਂ
        CV_Error(cv::Error::StsError, "Invalid matrix initializer type");
}

ਖਾਲੀ MatOp_Initializer::multiply(ਅਚਲ Mat_ਵਿਅੰਜਕ& e, ਦੁੱਗਣਾ s, Mat_ਵਿਅੰਜਕ& res) ਅਚਲ
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha *= s;
}

ਸਤਰਅੰਦਰ ਖਾਲੀ MatOp_Initializer::make_ਵਿਅੰਜਕ(Mat_ਵਿਅੰਜਕ& res, ਪੂਰਨ ਵਿਧੀ, ਮਾਪ sz, ਪੂਰਨ type, ਦੁੱਗਣਾ alpha)
{
    res = Mat_ਵਿਅੰਜਕ(getGlobalMatOpInitializer(), ਵਿਧੀ, Mat(sz, type, (ਖਾਲੀ*)(size_t)0xEEEEEEEE), Mat(), Mat(), alpha, 0);
}

ਸਤਰਅੰਦਰ ਖਾਲੀ MatOp_Initializer::make_ਵਿਅੰਜਕ(Mat_ਵਿਅੰਜਕ& res, ਪੂਰਨ ਵਿਧੀ, ਪੂਰਨ ndims, ਅਚਲ ਪੂਰਨ* sizes, ਪੂਰਨ type, ਦੁੱਗਣਾ alpha)
{
    res = Mat_ਵਿਅੰਜਕ(getGlobalMatOpInitializer(), ਵਿਧੀ, Mat(ndims, sizes, type, (ਖਾਲੀ*)(size_t)0xEEEEEEEE), Mat(), Mat(), alpha, 0);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

Mat_ਵਿਅੰਜਕ Mat::t() ਅਚਲ
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(*ਇਹ);
    Mat_ਵਿਅੰਜਕ e;
    MatOp_T::make_ਵਿਅੰਜਕ(e, *ਇਹ);
    ਵਾਪਸ e;
}

Mat_ਵਿਅੰਜਕ Mat::inv(ਪੂਰਨ ਵਿਧੀ) ਅਚਲ
{
    CV_INSTRUMENT_REGION();

    Mat_ਵਿਅੰਜਕ e;
    MatOp_Invert::make_ਵਿਅੰਜਕ(e, ਵਿਧੀ, *ਇਹ);
    ਵਾਪਸ e;
}


Mat_ਵਿਅੰਜਕ Mat::mul(ਨਿਵੇਸ਼_Array m, ਦੁੱਗਣਾ scale) ਅਚਲ
{
    CV_INSTRUMENT_REGION();

    Mat b = m.getMat();
    // Unless the argument is a refcounted Mat/UMat, the header returned by getMat() may be
    // a non-owning view of caller memory (e.g. a scalar bound to _InputArray(const double&),
    // a Matx or a Vec) that the returned MatExpr can outlive, so snapshot it.
    // See https://github.com/opencv/opencv/issues/23577
    ਜੇ( !m.isMat() && !m.isUMat() )
        b = b.clone();

    Mat_ਵਿਅੰਜਕ e;
    MatOp_Bin::make_ਵਿਅੰਜਕ(e, '*', *ਇਹ, b, scale);
    ਵਾਪਸ e;
}

Mat_ਵਿਅੰਜਕ Mat::zeros(ਪੂਰਨ ਪੰਕਤੀਆਂ, ਪੂਰਨ ਖਾਨੇ_ਗਿਣਤੀ, ਪੂਰਨ type)
{
    CV_INSTRUMENT_REGION();

    Mat_ਵਿਅੰਜਕ e;
    MatOp_Initializer::make_ਵਿਅੰਜਕ(e, '0', ਮਾਪ(ਖਾਨੇ_ਗਿਣਤੀ, ਪੰਕਤੀਆਂ), type);
    ਵਾਪਸ e;
}

Mat_ਵਿਅੰਜਕ Mat::zeros(ਮਾਪ size, ਪੂਰਨ type)
{
    CV_INSTRUMENT_REGION();

    Mat_ਵਿਅੰਜਕ e;
    MatOp_Initializer::make_ਵਿਅੰਜਕ(e, '0', size, type);
    ਵਾਪਸ e;
}

Mat_ਵਿਅੰਜਕ Mat::zeros(ਪੂਰਨ ndims, ਅਚਲ ਪੂਰਨ* sizes, ਪੂਰਨ type)
{
    CV_INSTRUMENT_REGION();

    Mat_ਵਿਅੰਜਕ e;
    MatOp_Initializer::make_ਵਿਅੰਜਕ(e, '0', ndims, sizes, type);
    ਵਾਪਸ e;
}

Mat_ਵਿਅੰਜਕ Mat::zeros(ਅਚਲ MatShape& shape, ਪੂਰਨ type)
{
    CV_INSTRUMENT_REGION();

    Mat_ਵਿਅੰਜਕ e;
    MatOp_Initializer::make_ਵਿਅੰਜਕ(e, '0', shape.dims, shape.p, type);
    ਵਾਪਸ e;
}

Mat_ਵਿਅੰਜਕ Mat::ones(ਪੂਰਨ ਪੰਕਤੀਆਂ, ਪੂਰਨ ਖਾਨੇ_ਗਿਣਤੀ, ਪੂਰਨ type)
{
    CV_INSTRUMENT_REGION();

    Mat_ਵਿਅੰਜਕ e;
    MatOp_Initializer::make_ਵਿਅੰਜਕ(e, '1', ਮਾਪ(ਖਾਨੇ_ਗਿਣਤੀ, ਪੰਕਤੀਆਂ), type);
    ਵਾਪਸ e;
}

Mat_ਵਿਅੰਜਕ Mat::ones(ਮਾਪ size, ਪੂਰਨ type)
{
    CV_INSTRUMENT_REGION();

    Mat_ਵਿਅੰਜਕ e;
    MatOp_Initializer::make_ਵਿਅੰਜਕ(e, '1', size, type);
    ਵਾਪਸ e;
}

Mat_ਵਿਅੰਜਕ Mat::ones(ਪੂਰਨ ndims, ਅਚਲ ਪੂਰਨ* sizes, ਪੂਰਨ type)
{
    CV_INSTRUMENT_REGION();

    Mat_ਵਿਅੰਜਕ e;
    MatOp_Initializer::make_ਵਿਅੰਜਕ(e, '1', ndims, sizes, type);
    ਵਾਪਸ e;
}

Mat_ਵਿਅੰਜਕ Mat::ones(ਅਚਲ MatShape& shape, ਪੂਰਨ type)
{
    CV_INSTRUMENT_REGION();

    Mat_ਵਿਅੰਜਕ e;
    MatOp_Initializer::make_ਵਿਅੰਜਕ(e, '1', shape.dims, shape.p, type);
    ਵਾਪਸ e;
}

Mat_ਵਿਅੰਜਕ Mat::eye(ਪੂਰਨ ਪੰਕਤੀਆਂ, ਪੂਰਨ ਖਾਨੇ_ਗਿਣਤੀ, ਪੂਰਨ type)
{
    CV_INSTRUMENT_REGION();

    Mat_ਵਿਅੰਜਕ e;
    MatOp_Initializer::make_ਵਿਅੰਜਕ(e, 'I', ਮਾਪ(ਖਾਨੇ_ਗਿਣਤੀ, ਪੰਕਤੀਆਂ), type);
    ਵਾਪਸ e;
}

Mat_ਵਿਅੰਜਕ Mat::eye(ਮਾਪ size, ਪੂਰਨ type)
{
    CV_INSTRUMENT_REGION();

    Mat_ਵਿਅੰਜਕ e;
    MatOp_Initializer::make_ਵਿਅੰਜਕ(e, 'I', size, type);
    ਵਾਪਸ e;
}

ਖਾਲੀ Mat_ਵਿਅੰਜਕ::swap(Mat_ਵਿਅੰਜਕ& other)
{
    ਵਰਤੋਂ std::swap;

    swap(op, other.op);
    swap(flags, other.flags);

    swap(a, other.a);
    swap(b, other.b);
    swap(c, other.c);

    swap(alpha, other.alpha);
    swap(beta, other.beta);

    swap(s, other.s);
}

ਨਿਵੇਸ਼_Array_2::ਨਿਵੇਸ਼_Array_2(ਅਚਲ Mat_ਵਿਅੰਜਕ& ਵਿਅੰਜਕ)
{
    ਜੇ (!is_ਪਛਾਣ_ਤੱਤ(ਵਿਅੰਜਕ))
    {
        Mat ਨਤੀਜਾ = ਵਿਅੰਜਕ;  // TODO improve through refcount == 1 of expr.a (inplace operation is possible - except gemm?)
        Mat_ਵਿਅੰਜਕ ਨਤੀਜਾ_ਵਿਅੰਜਕ(ਨਤੀਜਾ);
        swap(const_cast<Mat_ਵਿਅੰਜਕ&>(ਵਿਅੰਜਕ), ਨਤੀਜਾ_ਵਿਅੰਜਕ);
    }
    CV_Assert(is_ਪਛਾਣ_ਤੱਤ(ਵਿਅੰਜਕ));
    init(FIXED_TYPE + FIXED_ਮਾਪ + MAT + ACCESS_READ, &ਵਿਅੰਜਕ.a);
}

} // cv::
