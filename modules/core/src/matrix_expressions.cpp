// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


/* ////////////////////////////////////////////////////////////////////
//
//  Mat basic operations: Copy, Set
//
// */

#ಅನ್ತರ್ಭಾವ "precomp.hpp"
#ಅನ್ತರ್ಭಾವ <opencv2/core/utils/logger.hpp>

ನಾಮಾಕಾಶ cv
{

//This and its overload below are used in various MatExpr operator overloads
//implemented to check that Matrix operands exist.
ಸ್ಥಿರ ಶೂನ್ಯ checkOperandsExist(ಧ್ರುವ Mat& a)
{
    ಯದಿ (a.empty())
    {
        CV_Error(cv::Error::StsBadArg, "Matrix operand is an empty matrix.");
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ checkOperandsExist(ಧ್ರುವ Mat& a, ಧ್ರುವ Mat& b)
{
    ಯದಿ (a.empty() || b.empty())
    {
        CV_Error(cv::Error::StsBadArg, "One or more matrix operands are empty.");
    }
}


ವರ್ಗ MatOp_Identity CV_FINAL : ಸಾರ್ವಜನಿಕ MatOp
{
ಸಾರ್ವಜನಿಕ:
    MatOp_Identity() {}
    ಆಭಾಸ ~MatOp_Identity() {}

    ತಾರ್ಕಿಕ elementWise(ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& /*ಅಭಿವ್ಯಕ್ತಿ*/) ಧ್ರುವ CV_OVERRIDE { ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ; }
    ಶೂನ್ಯ assign(ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& ಅಭಿವ್ಯಕ್ತಿ, Mat& m, ಪೂರ್ಣಾಙ್ಕ type=-1) ಧ್ರುವ CV_OVERRIDE;

    ಸ್ಥಿರ ಶೂನ್ಯ make_ಅಭಿವ್ಯಕ್ತಿ(Mat_ಅಭಿವ್ಯಕ್ತಿ& res, ಧ್ರುವ Mat& m);
};

ಸ್ಥಿರ MatOp_Identity g_MatOp_Identity;

ವರ್ಗ MatOp_AddEx CV_FINAL : ಸಾರ್ವಜನಿಕ MatOp
{
ಸಾರ್ವಜನಿಕ:
    MatOp_AddEx() {}
    ಆಭಾಸ ~MatOp_AddEx() {}

    ತಾರ್ಕಿಕ elementWise(ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& /*ಅಭಿವ್ಯಕ್ತಿ*/) ಧ್ರುವ CV_OVERRIDE { ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ; }
    ಶೂನ್ಯ assign(ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& ಅಭಿವ್ಯಕ್ತಿ, Mat& m, ಪೂರ್ಣಾಙ್ಕ type=-1) ಧ್ರುವ CV_OVERRIDE;

    ಶೂನ್ಯ add(ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& e1, ಧ್ರುವ Scalar& s, Mat_ಅಭಿವ್ಯಕ್ತಿ& res) ಧ್ರುವ CV_OVERRIDE;
    ಶೂನ್ಯ subtract(ಧ್ರುವ Scalar& s, ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& ಅಭಿವ್ಯಕ್ತಿ, Mat_ಅಭಿವ್ಯಕ್ತಿ& res) ಧ್ರುವ CV_OVERRIDE;
    ಶೂನ್ಯ multiply(ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& e1, ದ್ವಿಗುಣ s, Mat_ಅಭಿವ್ಯಕ್ತಿ& res) ಧ್ರುವ CV_OVERRIDE;
    ಶೂನ್ಯ divide(ದ್ವಿಗುಣ s, ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& e, Mat_ಅಭಿವ್ಯಕ್ತಿ& res) ಧ್ರುವ CV_OVERRIDE;

    ಶೂನ್ಯ transpose(ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& e1, Mat_ಅಭಿವ್ಯಕ್ತಿ& res) ಧ್ರುವ CV_OVERRIDE;
    ಶೂನ್ಯ abs(ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& ಅಭಿವ್ಯಕ್ತಿ, Mat_ಅಭಿವ್ಯಕ್ತಿ& res) ಧ್ರುವ CV_OVERRIDE;

    ಸ್ಥಿರ ಶೂನ್ಯ make_ಅಭಿವ್ಯಕ್ತಿ(Mat_ಅಭಿವ್ಯಕ್ತಿ& res, ಧ್ರುವ Mat& a, ಧ್ರುವ Mat& b, ದ್ವಿಗುಣ alpha, ದ್ವಿಗುಣ beta, ಧ್ರುವ Scalar& s=Scalar());
};

ಸ್ಥಿರ MatOp_AddEx g_MatOp_AddEx;

ವರ್ಗ MatOp_Bin CV_FINAL : ಸಾರ್ವಜನಿಕ MatOp
{
ಸಾರ್ವಜನಿಕ:
    MatOp_Bin() {}
    ಆಭಾಸ ~MatOp_Bin() {}

    ತಾರ್ಕಿಕ elementWise(ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& /*ಅಭಿವ್ಯಕ್ತಿ*/) ಧ್ರುವ CV_OVERRIDE { ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ; }
    ಶೂನ್ಯ assign(ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& ಅಭಿವ್ಯಕ್ತಿ, Mat& m, ಪೂರ್ಣಾಙ್ಕ type=-1) ಧ್ರುವ CV_OVERRIDE;

    ಶೂನ್ಯ multiply(ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& e1, ದ್ವಿಗುಣ s, Mat_ಅಭಿವ್ಯಕ್ತಿ& res) ಧ್ರುವ CV_OVERRIDE;
    ಶೂನ್ಯ divide(ದ್ವಿಗುಣ s, ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& e, Mat_ಅಭಿವ್ಯಕ್ತಿ& res) ಧ್ರುವ CV_OVERRIDE;

    ಸ್ಥಿರ ಶೂನ್ಯ make_ಅಭಿವ್ಯಕ್ತಿ(Mat_ಅಭಿವ್ಯಕ್ತಿ& res, ವರ್ಣ op, ಧ್ರುವ Mat& a, ಧ್ರುವ Mat& b, ದ್ವಿಗುಣ scale=1);
    ಸ್ಥಿರ ಶೂನ್ಯ make_ಅಭಿವ್ಯಕ್ತಿ(Mat_ಅಭಿವ್ಯಕ್ತಿ& res, ವರ್ಣ op, ಧ್ರುವ Mat& a, ಧ್ರುವ Scalar& s);
};

ಸ್ಥಿರ MatOp_Bin g_MatOp_Bin;

ವರ್ಗ MatOp_Cmp CV_FINAL : ಸಾರ್ವಜನಿಕ MatOp
{
ಸಾರ್ವಜನಿಕ:
    MatOp_Cmp() {}
    ಆಭಾಸ ~MatOp_Cmp() {}

    ತಾರ್ಕಿಕ elementWise(ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& /*ಅಭಿವ್ಯಕ್ತಿ*/) ಧ್ರುವ CV_OVERRIDE { ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ; }
    ಶೂನ್ಯ assign(ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& ಅಭಿವ್ಯಕ್ತಿ, Mat& m, ಪೂರ್ಣಾಙ್ಕ type=-1) ಧ್ರುವ CV_OVERRIDE;

    ಸ್ಥಿರ ಶೂನ್ಯ make_ಅಭಿವ್ಯಕ್ತಿ(Mat_ಅಭಿವ್ಯಕ್ತಿ& res, ಪೂರ್ಣಾಙ್ಕ cmpop, ಧ್ರುವ Mat& a, ಧ್ರುವ Mat& b);
    ಸ್ಥಿರ ಶೂನ್ಯ make_ಅಭಿವ್ಯಕ್ತಿ(Mat_ಅಭಿವ್ಯಕ್ತಿ& res, ಪೂರ್ಣಾಙ್ಕ cmpop, ಧ್ರುವ Mat& a, ದ್ವಿಗುಣ alpha);
};

ಸ್ಥಿರ MatOp_Cmp g_MatOp_Cmp;

ವರ್ಗ MatOp_GEMM CV_FINAL : ಸಾರ್ವಜನಿಕ MatOp
{
ಸಾರ್ವಜನಿಕ:
    MatOp_GEMM() {}
    ಆಭಾಸ ~MatOp_GEMM() {}

    ತಾರ್ಕಿಕ elementWise(ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& /*ಅಭಿವ್ಯಕ್ತಿ*/) ಧ್ರುವ CV_OVERRIDE { ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ; }
    ಶೂನ್ಯ assign(ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& ಅಭಿವ್ಯಕ್ತಿ, Mat& m, ಪೂರ್ಣಾಙ್ಕ type=-1) ಧ್ರುವ CV_OVERRIDE;

    ಶೂನ್ಯ add(ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& e1, ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& e2, Mat_ಅಭಿವ್ಯಕ್ತಿ& res) ಧ್ರುವ CV_OVERRIDE;
    ಶೂನ್ಯ subtract(ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& e1, ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& e2, Mat_ಅಭಿವ್ಯಕ್ತಿ& res) ಧ್ರುವ CV_OVERRIDE;
    ಶೂನ್ಯ multiply(ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& e, ದ್ವಿಗುಣ s, Mat_ಅಭಿವ್ಯಕ್ತಿ& res) ಧ್ರುವ CV_OVERRIDE;

    ಶೂನ್ಯ transpose(ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& ಅಭಿವ್ಯಕ್ತಿ, Mat_ಅಭಿವ್ಯಕ್ತಿ& res) ಧ್ರುವ CV_OVERRIDE;

    Size size(ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& ಅಭಿವ್ಯಕ್ತಿ) ಧ್ರುವ CV_OVERRIDE
    {
        ಪ್ರತ್ಯಾಗಮ Size(
            (ಅಭಿವ್ಯಕ್ತಿ.flags & GEMM_2_T) ? ಅಭಿವ್ಯಕ್ತಿ.b.rows : ಅಭಿವ್ಯಕ್ತಿ.b.cols,
            (ಅಭಿವ್ಯಕ್ತಿ.flags & GEMM_1_T) ? ಅಭಿವ್ಯಕ್ತಿ.a.cols : ಅಭಿವ್ಯಕ್ತಿ.a.rows
        );
    }

    ಸ್ಥಿರ ಶೂನ್ಯ make_ಅಭಿವ್ಯಕ್ತಿ(Mat_ಅಭಿವ್ಯಕ್ತಿ& res, ಪೂರ್ಣಾಙ್ಕ flags, ಧ್ರುವ Mat& a, ಧ್ರುವ Mat& b,
                         ದ್ವಿಗುಣ alpha=1, ಧ್ರುವ Mat& c=Mat(), ದ್ವಿಗುಣ beta=1);

};

ಸ್ಥಿರ MatOp_GEMM g_MatOp_GEMM;

ವರ್ಗ MatOp_Invert CV_FINAL : ಸಾರ್ವಜನಿಕ MatOp
{
ಸಾರ್ವಜನಿಕ:
    MatOp_Invert() {}
    ಆಭಾಸ ~MatOp_Invert() {}

    ತಾರ್ಕಿಕ elementWise(ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& /*ಅಭಿವ್ಯಕ್ತಿ*/) ಧ್ರುವ CV_OVERRIDE { ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ; }
    ಶೂನ್ಯ assign(ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& ಅಭಿವ್ಯಕ್ತಿ, Mat& m, ಪೂರ್ಣಾಙ್ಕ type=-1) ಧ್ರುವ CV_OVERRIDE;

    ಶೂನ್ಯ matmul(ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& expr1, ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& expr2, Mat_ಅಭಿವ್ಯಕ್ತಿ& res) ಧ್ರುವ CV_OVERRIDE;

    ಸ್ಥಿರ ಶೂನ್ಯ make_ಅಭಿವ್ಯಕ್ತಿ(Mat_ಅಭಿವ್ಯಕ್ತಿ& res, ಪೂರ್ಣಾಙ್ಕ method, ಧ್ರುವ Mat& m);
};

ಸ್ಥಿರ MatOp_Invert g_MatOp_Invert;

ವರ್ಗ MatOp_T CV_FINAL : ಸಾರ್ವಜನಿಕ MatOp
{
ಸಾರ್ವಜನಿಕ:
    MatOp_T() {}
    ಆಭಾಸ ~MatOp_T() {}

    ತಾರ್ಕಿಕ elementWise(ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& /*ಅಭಿವ್ಯಕ್ತಿ*/) ಧ್ರುವ CV_OVERRIDE { ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ; }
    ಶೂನ್ಯ assign(ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& ಅಭಿವ್ಯಕ್ತಿ, Mat& m, ಪೂರ್ಣಾಙ್ಕ type=-1) ಧ್ರುವ CV_OVERRIDE;

    ಶೂನ್ಯ multiply(ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& e1, ದ್ವಿಗುಣ s, Mat_ಅಭಿವ್ಯಕ್ತಿ& res) ಧ್ರುವ CV_OVERRIDE;
    ಶೂನ್ಯ transpose(ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& ಅಭಿವ್ಯಕ್ತಿ, Mat_ಅಭಿವ್ಯಕ್ತಿ& res) ಧ್ರುವ CV_OVERRIDE;

    ಸ್ಥಿರ ಶೂನ್ಯ make_ಅಭಿವ್ಯಕ್ತಿ(Mat_ಅಭಿವ್ಯಕ್ತಿ& res, ಧ್ರುವ Mat& a, ದ್ವಿಗುಣ alpha=1);
};

ಸ್ಥಿರ MatOp_T g_MatOp_T;

ವರ್ಗ MatOp_Solve CV_FINAL : ಸಾರ್ವಜನಿಕ MatOp
{
ಸಾರ್ವಜನಿಕ:
    MatOp_Solve() {}
    ಆಭಾಸ ~MatOp_Solve() {}

    ತಾರ್ಕಿಕ elementWise(ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& /*ಅಭಿವ್ಯಕ್ತಿ*/) ಧ್ರುವ CV_OVERRIDE { ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ; }
    ಶೂನ್ಯ assign(ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& ಅಭಿವ್ಯಕ್ತಿ, Mat& m, ಪೂರ್ಣಾಙ್ಕ type=-1) ಧ್ರುವ CV_OVERRIDE;

    ಸ್ಥಿರ ಶೂನ್ಯ make_ಅಭಿವ್ಯಕ್ತಿ(Mat_ಅಭಿವ್ಯಕ್ತಿ& res, ಪೂರ್ಣಾಙ್ಕ method, ಧ್ರುವ Mat& a, ಧ್ರುವ Mat& b);
};

ಸ್ಥಿರ MatOp_Solve g_MatOp_Solve;

ವರ್ಗ MatOp_Initializer CV_FINAL : ಸಾರ್ವಜನಿಕ MatOp
{
ಸಾರ್ವಜನಿಕ:
    MatOp_Initializer() {}
    ಆಭಾಸ ~MatOp_Initializer() {}

    ತಾರ್ಕಿಕ elementWise(ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& /*ಅಭಿವ್ಯಕ್ತಿ*/) ಧ್ರುವ CV_OVERRIDE { ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ; }
    ಶೂನ್ಯ assign(ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& ಅಭಿವ್ಯಕ್ತಿ, Mat& m, ಪೂರ್ಣಾಙ್ಕ type=-1) ಧ್ರುವ CV_OVERRIDE;

    ಶೂನ್ಯ multiply(ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& e, ದ್ವಿಗುಣ s, Mat_ಅಭಿವ್ಯಕ್ತಿ& res) ಧ್ರುವ CV_OVERRIDE;

    ಸ್ಥಿರ ಶೂನ್ಯ make_ಅಭಿವ್ಯಕ್ತಿ(Mat_ಅಭಿವ್ಯಕ್ತಿ& res, ಪೂರ್ಣಾಙ್ಕ method, Size sz, ಪೂರ್ಣಾಙ್ಕ type, ದ್ವಿಗುಣ alpha=1);
    ಸ್ಥಿರ ಶೂನ್ಯ make_ಅಭಿವ್ಯಕ್ತಿ(Mat_ಅಭಿವ್ಯಕ್ತಿ& res, ಪೂರ್ಣಾಙ್ಕ method, ಪೂರ್ಣಾಙ್ಕ ndims, ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ* sizes, ಪೂರ್ಣಾಙ್ಕ type, ದ್ವಿಗುಣ alpha=1);
};

ಸ್ಥಿರ MatOp_Initializer* getGlobalMatOpInitializer()
{
    CV_SINGLETON_LAZY_INIT(MatOp_Initializer, ನೂತನ MatOp_Initializer())
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ isIdentity(ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& e) { ಪ್ರತ್ಯಾಗಮ e.op == &g_MatOp_Identity; }
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ isAddEx(ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& e) { ಪ್ರತ್ಯಾಗಮ e.op == &g_MatOp_AddEx; }
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ isScaled(ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& e) { ಪ್ರತ್ಯಾಗಮ isAddEx(e) && (!e.b.data || e.beta == 0) && e.s == Scalar(); }
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ isBin(ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& e, ವರ್ಣ c) { ಪ್ರತ್ಯಾಗಮ e.op == &g_MatOp_Bin && e.flags == c; }
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ isCmp(ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& e) { ಪ್ರತ್ಯಾಗಮ e.op == &g_MatOp_Cmp; }
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ isReciprocal(ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& e) { ಪ್ರತ್ಯಾಗಮ isBin(e,'/') && (!e.b.data || e.beta == 0); }
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ isT(ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& e) { ಪ್ರತ್ಯಾಗಮ e.op == &g_MatOp_T; }
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ isInv(ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& e) { ಪ್ರತ್ಯಾಗಮ e.op == &g_MatOp_Invert; }
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ isSolve(ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& e) { ಪ್ರತ್ಯಾಗಮ e.op == &g_MatOp_Solve; }
//static inline bool isGEMM(const MatExpr& e) { return e.op == &g_MatOp_GEMM; }
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ isMatProd(ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& e) { ಪ್ರತ್ಯಾಗಮ e.op == &g_MatOp_GEMM && (!e.c.data || e.beta == 0); }
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ isInitializer(ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& e) { ಪ್ರತ್ಯಾಗಮ e.op == getGlobalMatOpInitializer(); }

/////////////////////////////////////////////////////////////////////////////////////////////////////

MatOp::MatOp() {}
MatOp::~MatOp() {}


ತಾರ್ಕಿಕ MatOp::elementWise(ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& /*ಅಭಿವ್ಯಕ್ತಿ*/) ಧ್ರುವ
{
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
}

ಶೂನ್ಯ MatOp::roi(ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& ಅಭಿವ್ಯಕ್ತಿ, ಧ್ರುವ Range& rowRange, ಧ್ರುವ Range& colRange, Mat_ಅಭಿವ್ಯಕ್ತಿ& e) ಧ್ರುವ
{
    ಯದಿ( elementWise(ಅಭಿವ್ಯಕ್ತಿ) )
    {
        e = Mat_ಅಭಿವ್ಯಕ್ತಿ(ಅಭಿವ್ಯಕ್ತಿ.op, ಅಭಿವ್ಯಕ್ತಿ.flags, Mat(), Mat(), Mat(),
                    ಅಭಿವ್ಯಕ್ತಿ.alpha, ಅಭಿವ್ಯಕ್ತಿ.beta, ಅಭಿವ್ಯಕ್ತಿ.s);
        ಯದಿ(ಅಭಿವ್ಯಕ್ತಿ.a.data)
            e.a = ಅಭಿವ್ಯಕ್ತಿ.a(rowRange, colRange);
        ಯದಿ(ಅಭಿವ್ಯಕ್ತಿ.b.data)
            e.b = ಅಭಿವ್ಯಕ್ತಿ.b(rowRange, colRange);
        ಯದಿ(ಅಭಿವ್ಯಕ್ತಿ.c.data)
            e.c = ಅಭಿವ್ಯಕ್ತಿ.c(rowRange, colRange);
    }
    ಅನ್ಯಥಾ
    {
        Mat m;
        ಅಭಿವ್ಯಕ್ತಿ.op->assign(ಅಭಿವ್ಯಕ್ತಿ, m);
        e = Mat_ಅಭಿವ್ಯಕ್ತಿ(&g_MatOp_Identity, 0, m(rowRange, colRange), Mat(), Mat());
    }
}

ಶೂನ್ಯ MatOp::diag(ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& ಅಭಿವ್ಯಕ್ತಿ, ಪೂರ್ಣಾಙ್ಕ d, Mat_ಅಭಿವ್ಯಕ್ತಿ& e) ಧ್ರುವ
{
    ಯದಿ( elementWise(ಅಭಿವ್ಯಕ್ತಿ) )
    {
        e = Mat_ಅಭಿವ್ಯಕ್ತಿ(ಅಭಿವ್ಯಕ್ತಿ.op, ಅಭಿವ್ಯಕ್ತಿ.flags, Mat(), Mat(), Mat(),
                    ಅಭಿವ್ಯಕ್ತಿ.alpha, ಅಭಿವ್ಯಕ್ತಿ.beta, ಅಭಿವ್ಯಕ್ತಿ.s);
        ಯದಿ(ಅಭಿವ್ಯಕ್ತಿ.a.data)
            e.a = ಅಭಿವ್ಯಕ್ತಿ.a.diag(d);
        ಯದಿ(ಅಭಿವ್ಯಕ್ತಿ.b.data)
            e.b = ಅಭಿವ್ಯಕ್ತಿ.b.diag(d);
        ಯದಿ(ಅಭಿವ್ಯಕ್ತಿ.c.data)
            e.c = ಅಭಿವ್ಯಕ್ತಿ.c.diag(d);
    }
    ಅನ್ಯಥಾ
    {
        Mat m;
        ಅಭಿವ್ಯಕ್ತಿ.op->assign(ಅಭಿವ್ಯಕ್ತಿ, m);
        e = Mat_ಅಭಿವ್ಯಕ್ತಿ(&g_MatOp_Identity, 0, m.diag(d), Mat(), Mat());
    }
}


ಶೂನ್ಯ MatOp::augAssignAdd(ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& ಅಭಿವ್ಯಕ್ತಿ, Mat& m) ಧ್ರುವ
{
    Mat temp;
    ಅಭಿವ್ಯಕ್ತಿ.op->assign(ಅಭಿವ್ಯಕ್ತಿ, temp);
    m += temp;
}


ಶೂನ್ಯ MatOp::augAssignSubtract(ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& ಅಭಿವ್ಯಕ್ತಿ, Mat& m) ಧ್ರುವ
{
    Mat temp;
    ಅಭಿವ್ಯಕ್ತಿ.op->assign(ಅಭಿವ್ಯಕ್ತಿ, temp);
    m -= temp;
}


ಶೂನ್ಯ MatOp::augAssignMultiply(ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& ಅಭಿವ್ಯಕ್ತಿ, Mat& m) ಧ್ರುವ
{
    Mat temp;
    ಅಭಿವ್ಯಕ್ತಿ.op->assign(ಅಭಿವ್ಯಕ್ತಿ, temp);
    m *= temp;
}


ಶೂನ್ಯ MatOp::augAssignDivide(ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& ಅಭಿವ್ಯಕ್ತಿ, Mat& m) ಧ್ರುವ
{
    Mat temp;
    ಅಭಿವ್ಯಕ್ತಿ.op->assign(ಅಭಿವ್ಯಕ್ತಿ, temp);
    m /= temp;
}


ಶೂನ್ಯ MatOp::augAssignAnd(ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& ಅಭಿವ್ಯಕ್ತಿ, Mat& m) ಧ್ರುವ
{
    Mat temp;
    ಅಭಿವ್ಯಕ್ತಿ.op->assign(ಅಭಿವ್ಯಕ್ತಿ, temp);
    m &= temp;
}


ಶೂನ್ಯ MatOp::augAssignOr(ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& ಅಭಿವ್ಯಕ್ತಿ, Mat& m) ಧ್ರುವ
{
    Mat temp;
    ಅಭಿವ್ಯಕ್ತಿ.op->assign(ಅಭಿವ್ಯಕ್ತಿ, temp);
    m |= temp;
}


ಶೂನ್ಯ MatOp::augAssignXor(ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& ಅಭಿವ್ಯಕ್ತಿ, Mat& m) ಧ್ರುವ
{
    Mat temp;
    ಅಭಿವ್ಯಕ್ತಿ.op->assign(ಅಭಿವ್ಯಕ್ತಿ, temp);
    m ^= temp;
}


ಶೂನ್ಯ MatOp::add(ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& e1, ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& e2, Mat_ಅಭಿವ್ಯಕ್ತಿ& res) ಧ್ರುವ
{
    CV_INSTRUMENT_REGION();

    ಯದಿ( ಏತತ್ == e2.op )
    {
        ದ್ವಿಗುಣ alpha = 1, beta = 1;
        Scalar s;
        Mat m1, m2;
        ಯದಿ( isAddEx(e1) && (!e1.b.data || e1.beta == 0) )
        {
            m1 = e1.a;
            alpha = e1.alpha;
            s = e1.s;
        }
        ಅನ್ಯಥಾ
            e1.op->assign(e1, m1);

        ಯದಿ( isAddEx(e2) && (!e2.b.data || e2.beta == 0) )
        {
            m2 = e2.a;
            beta = e2.alpha;
            s += e2.s;
        }
        ಅನ್ಯಥಾ
            e2.op->assign(e2, m2);
        MatOp_AddEx::make_ಅಭಿವ್ಯಕ್ತಿ(res, m1, m2, alpha, beta, s);
    }
    ಅನ್ಯಥಾ
        e2.op->add(e1, e2, res);
}


ಶೂನ್ಯ MatOp::add(ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& expr1, ಧ್ರುವ Scalar& s, Mat_ಅಭಿವ್ಯಕ್ತಿ& res) ಧ್ರುವ
{
    CV_INSTRUMENT_REGION();

    Mat m1;
    expr1.op->assign(expr1, m1);
    MatOp_AddEx::make_ಅಭಿವ್ಯಕ್ತಿ(res, m1, Mat(), 1, 0, s);
}


ಶೂನ್ಯ MatOp::subtract(ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& e1, ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& e2, Mat_ಅಭಿವ್ಯಕ್ತಿ& res) ಧ್ರುವ
{
    CV_INSTRUMENT_REGION();

    ಯದಿ( ಏತತ್ == e2.op )
    {
        ದ್ವಿಗುಣ alpha = 1, beta = -1;
        Scalar s;
        Mat m1, m2;
        ಯದಿ( isAddEx(e1) && (!e1.b.data || e1.beta == 0) )
        {
            m1 = e1.a;
            alpha = e1.alpha;
            s = e1.s;
        }
        ಅನ್ಯಥಾ
            e1.op->assign(e1, m1);

        ಯದಿ( isAddEx(e2) && (!e2.b.data || e2.beta == 0) )
        {
            m2 = e2.a;
            beta = -e2.alpha;
            s -= e2.s;
        }
        ಅನ್ಯಥಾ
            e2.op->assign(e2, m2);
        MatOp_AddEx::make_ಅಭಿವ್ಯಕ್ತಿ(res, m1, m2, alpha, beta, s);
    }
    ಅನ್ಯಥಾ
        e2.op->subtract(e1, e2, res);
}


ಶೂನ್ಯ MatOp::subtract(ಧ್ರುವ Scalar& s, ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& ಅಭಿವ್ಯಕ್ತಿ, Mat_ಅಭಿವ್ಯಕ್ತಿ& res) ಧ್ರುವ
{
    CV_INSTRUMENT_REGION();

    Mat m;
    ಅಭಿವ್ಯಕ್ತಿ.op->assign(ಅಭಿವ್ಯಕ್ತಿ, m);
    MatOp_AddEx::make_ಅಭಿವ್ಯಕ್ತಿ(res, m, Mat(), -1, 0, s);
}


ಶೂನ್ಯ MatOp::multiply(ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& e1, ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& e2, Mat_ಅಭಿವ್ಯಕ್ತಿ& res, ದ್ವಿಗುಣ scale) ಧ್ರುವ
{
    CV_INSTRUMENT_REGION();

    ಯದಿ( ಏತತ್ == e2.op )
    {
        Mat m1, m2;

        ಯದಿ( isReciprocal(e1) )
        {
            ಯದಿ( isScaled(e2) )
            {
                scale *= e2.alpha;
                m2 = e2.a;
            }
            ಅನ್ಯಥಾ
                e2.op->assign(e2, m2);

            MatOp_Bin::make_ಅಭಿವ್ಯಕ್ತಿ(res, '/', m2, e1.a, scale/e1.alpha);
        }
        ಅನ್ಯಥಾ
        {
            ವರ್ಣ op = '*';
            ಯದಿ( isScaled(e1) )
            {
                m1 = e1.a;
                scale *= e1.alpha;
            }
            ಅನ್ಯಥಾ
                e1.op->assign(e1, m1);

            ಯದಿ( isScaled(e2) )
            {
                m2 = e2.a;
                scale *= e2.alpha;
            }
            ಅನ್ಯಥಾ ಯದಿ( isReciprocal(e2) )
            {
                op = '/';
                m2 = e2.a;
                scale *= e2.alpha;
            }
            ಅನ್ಯಥಾ
                e2.op->assign(e2, m2);

            MatOp_Bin::make_ಅಭಿವ್ಯಕ್ತಿ(res, op, m1, m2, scale);
        }
    }
    ಅನ್ಯಥಾ
        e2.op->multiply(e1, e2, res, scale);
}


ಶೂನ್ಯ MatOp::multiply(ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& ಅಭಿವ್ಯಕ್ತಿ, ದ್ವಿಗುಣ s, Mat_ಅಭಿವ್ಯಕ್ತಿ& res) ಧ್ರುವ
{
    CV_INSTRUMENT_REGION();

    Mat m;
    ಅಭಿವ್ಯಕ್ತಿ.op->assign(ಅಭಿವ್ಯಕ್ತಿ, m);
    MatOp_AddEx::make_ಅಭಿವ್ಯಕ್ತಿ(res, m, Mat(), s, 0);
}


ಶೂನ್ಯ MatOp::divide(ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& e1, ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& e2, Mat_ಅಭಿವ್ಯಕ್ತಿ& res, ದ್ವಿಗುಣ scale) ಧ್ರುವ
{
    CV_INSTRUMENT_REGION();

    ಯದಿ( ಏತತ್ == e2.op )
    {
        ಯದಿ( isReciprocal(e1) && isReciprocal(e2) )
            MatOp_Bin::make_ಅಭಿವ್ಯಕ್ತಿ(res, '/', e2.a, e1.a, e1.alpha/e2.alpha);
        ಅನ್ಯಥಾ
        {
            Mat m1, m2;
            ವರ್ಣ op = '/';

            ಯದಿ( isScaled(e1) )
            {
                m1 = e1.a;
                scale *= e1.alpha;
            }
            ಅನ್ಯಥಾ
                e1.op->assign(e1, m1);

            ಯದಿ( isScaled(e2) )
            {
                m2 = e2.a;
                scale /= e2.alpha;
            }
            ಅನ್ಯಥಾ ಯದಿ( isReciprocal(e2) )
            {
                m2 = e2.a;
                scale /= e2.alpha;
                op = '*';
            }
            ಅನ್ಯಥಾ
                e2.op->assign(e2, m2);
            MatOp_Bin::make_ಅಭಿವ್ಯಕ್ತಿ(res, op, m1, m2, scale);
        }
    }
    ಅನ್ಯಥಾ
        e2.op->divide(e1, e2, res, scale);
}


ಶೂನ್ಯ MatOp::divide(ದ್ವಿಗುಣ s, ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& ಅಭಿವ್ಯಕ್ತಿ, Mat_ಅಭಿವ್ಯಕ್ತಿ& res) ಧ್ರುವ
{
    CV_INSTRUMENT_REGION();

    Mat m;
    ಅಭಿವ್ಯಕ್ತಿ.op->assign(ಅಭಿವ್ಯಕ್ತಿ, m);
    MatOp_Bin::make_ಅಭಿವ್ಯಕ್ತಿ(res, '/', m, Mat(), s);
}


ಶೂನ್ಯ MatOp::abs(ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& ಅಭಿವ್ಯಕ್ತಿ, Mat_ಅಭಿವ್ಯಕ್ತಿ& res) ಧ್ರುವ
{
    CV_INSTRUMENT_REGION();

    Mat m;
    ಅಭಿವ್ಯಕ್ತಿ.op->assign(ಅಭಿವ್ಯಕ್ತಿ, m);
    MatOp_Bin::make_ಅಭಿವ್ಯಕ್ತಿ(res, 'a', m, Mat());
}


ಶೂನ್ಯ MatOp::transpose(ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& ಅಭಿವ್ಯಕ್ತಿ, Mat_ಅಭಿವ್ಯಕ್ತಿ& res) ಧ್ರುವ
{
    CV_INSTRUMENT_REGION();

    Mat m;
    ಅಭಿವ್ಯಕ್ತಿ.op->assign(ಅಭಿವ್ಯಕ್ತಿ, m);
    MatOp_T::make_ಅಭಿವ್ಯಕ್ತಿ(res, m, 1);
}


ಶೂನ್ಯ MatOp::matmul(ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& e1, ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& e2, Mat_ಅಭಿವ್ಯಕ್ತಿ& res) ಧ್ರುವ
{
    ಯದಿ( ಏತತ್ == e2.op )
    {
        ದ್ವಿಗುಣ scale = 1;
        ಪೂರ್ಣಾಙ್ಕ flags = 0;
        Mat m1, m2;

        ಯದಿ( isT(e1) )
        {
            flags = cv::GEMM_1_T;
            scale = e1.alpha;
            m1 = e1.a;
        }
        ಅನ್ಯಥಾ ಯದಿ( isScaled(e1) )
        {
            scale = e1.alpha;
            m1 = e1.a;
        }
        ಅನ್ಯಥಾ
            e1.op->assign(e1, m1);

        ಯದಿ( isT(e2) )
        {
            flags |= cv::GEMM_2_T;
            scale *= e2.alpha;
            m2 = e2.a;
        }
        ಅನ್ಯಥಾ ಯದಿ( isScaled(e2) )
        {
            scale *= e2.alpha;
            m2 = e2.a;
        }
        ಅನ್ಯಥಾ
            e2.op->assign(e2, m2);

        MatOp_GEMM::make_ಅಭಿವ್ಯಕ್ತಿ(res, flags, m1, m2, scale);
    }
    ಅನ್ಯಥಾ
        e2.op->matmul(e1, e2, res);
}


ಶೂನ್ಯ MatOp::invert(ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& ಅಭಿವ್ಯಕ್ತಿ, ಪೂರ್ಣಾಙ್ಕ method, Mat_ಅಭಿವ್ಯಕ್ತಿ& res) ಧ್ರುವ
{
    Mat m;
    ಅಭಿವ್ಯಕ್ತಿ.op->assign(ಅಭಿವ್ಯಕ್ತಿ, m);
    MatOp_Invert::make_ಅಭಿವ್ಯಕ್ತಿ(res, method, m);
}


Size MatOp::size(ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& ಅಭಿವ್ಯಕ್ತಿ) ಧ್ರುವ
{
    ಪ್ರತ್ಯಾಗಮ !ಅಭಿವ್ಯಕ್ತಿ.a.empty() ? ಅಭಿವ್ಯಕ್ತಿ.a.size() : ಅಭಿವ್ಯಕ್ತಿ.b.empty() ? ಅಭಿವ್ಯಕ್ತಿ.b.size() : ಅಭಿವ್ಯಕ್ತಿ.c.size();
}

ಪೂರ್ಣಾಙ್ಕ MatOp::type(ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& ಅಭಿವ್ಯಕ್ತಿ) ಧ್ರುವ
{
    CV_INSTRUMENT_REGION();

    ಪ್ರತ್ಯಾಗಮ !ಅಭಿವ್ಯಕ್ತಿ.a.empty() ? ಅಭಿವ್ಯಕ್ತಿ.a.type() : ಅಭಿವ್ಯಕ್ತಿ.b.empty() ? ಅಭಿವ್ಯಕ್ತಿ.b.type() : ಅಭಿವ್ಯಕ್ತಿ.c.type();
}

//////////////////////////////////////////////////////////////////////////////////////////////////

Mat_ಅಭಿವ್ಯಕ್ತಿ::Mat_ಅಭಿವ್ಯಕ್ತಿ(ಧ್ರುವ Mat& m) : op(&g_MatOp_Identity), flags(0), a(m), b(Mat()), c(Mat()), alpha(1), beta(0), s(Scalar())
{
}

Mat_ಅಭಿವ್ಯಕ್ತಿ Mat_ಅಭಿವ್ಯಕ್ತಿ::row(ಪೂರ್ಣಾಙ್ಕ y) ಧ್ರುವ
{
    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    op->roi(*ಏತತ್, Range(y, y+1), Range::all(), e);
    ಪ್ರತ್ಯಾಗಮ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ Mat_ಅಭಿವ್ಯಕ್ತಿ::col(ಪೂರ್ಣಾಙ್ಕ x) ಧ್ರುವ
{
    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    op->roi(*ಏತತ್, Range::all(), Range(x, x+1), e);
    ಪ್ರತ್ಯಾಗಮ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ Mat_ಅಭಿವ್ಯಕ್ತಿ::diag(ಪೂರ್ಣಾಙ್ಕ d) ಧ್ರುವ
{
    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    op->diag(*ಏತತ್, d, e);
    ಪ್ರತ್ಯಾಗಮ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ Mat_ಅಭಿವ್ಯಕ್ತಿ::operator()( ಧ್ರುವ Range& rowRange, ಧ್ರುವ Range& colRange ) ಧ್ರುವ
{
    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    op->roi(*ಏತತ್, rowRange, colRange, e);
    ಪ್ರತ್ಯಾಗಮ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ Mat_ಅಭಿವ್ಯಕ್ತಿ::operator()( ಧ್ರುವ Rect& roi ) ಧ್ರುವ
{
    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    op->roi(*ಏತತ್, Range(roi.y, roi.y + roi.height), Range(roi.x, roi.x + roi.width), e);
    ಪ್ರತ್ಯಾಗಮ e;
}

Mat Mat_ಅಭಿವ್ಯಕ್ತಿ::cross(ಧ್ರುವ Mat& m) ಧ್ರುವ
{
    ಪ್ರತ್ಯಾಗಮ ((Mat)*ಏತತ್).cross(m);
}

ದ್ವಿಗುಣ Mat_ಅಭಿವ್ಯಕ್ತಿ::dot(ಧ್ರುವ Mat& m) ಧ್ರುವ
{
    ಪ್ರತ್ಯಾಗಮ ((Mat)*ಏತತ್).dot(m);
}

Mat_ಅಭಿವ್ಯಕ್ತಿ Mat_ಅಭಿವ್ಯಕ್ತಿ::t() ಧ್ರುವ
{
    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    op->transpose(*ಏತತ್, e);
    ಪ್ರತ್ಯಾಗಮ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ Mat_ಅಭಿವ್ಯಕ್ತಿ::inv(ಪೂರ್ಣಾಙ್ಕ method) ಧ್ರುವ
{
    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    op->invert(*ಏತತ್, method, e);
    ಪ್ರತ್ಯಾಗಮ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ Mat_ಅಭಿವ್ಯಕ್ತಿ::mul(ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& e, ದ್ವಿಗುಣ scale) ಧ್ರುವ
{
    Mat_ಅಭಿವ್ಯಕ್ತಿ en;
    op->multiply(*ಏತತ್, e, en, scale);
    ಪ್ರತ್ಯಾಗಮ en;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ Mat_ಅಭಿವ್ಯಕ್ತಿ::mul(ಧ್ರುವ Mat& m, ದ್ವಿಗುಣ scale) ಧ್ರುವ
{
    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    op->multiply(*ಏತತ್, Mat_ಅಭಿವ್ಯಕ್ತಿ(m), e, scale);
    ಪ್ರತ್ಯಾಗಮ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ operator + (ಧ್ರುವ Mat& a, ಧ್ರುವ Mat& b)
{
    checkOperandsExist(a, b);
    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    MatOp_AddEx::make_ಅಭಿವ್ಯಕ್ತಿ(e, a, b, 1, 1);
    ಪ್ರತ್ಯಾಗಮ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ operator + (ಧ್ರುವ Mat& a, ಧ್ರುವ Scalar& s)
{
    checkOperandsExist(a);
    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    MatOp_AddEx::make_ಅಭಿವ್ಯಕ್ತಿ(e, a, Mat(), 1, 0, s);
    ಪ್ರತ್ಯಾಗಮ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ operator + (ಧ್ರುವ Scalar& s, ಧ್ರುವ Mat& a)
{
    checkOperandsExist(a);
    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    MatOp_AddEx::make_ಅಭಿವ್ಯಕ್ತಿ(e, a, Mat(), 1, 0, s);
    ಪ್ರತ್ಯಾಗಮ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ operator + (ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& e, ಧ್ರುವ Mat& m)
{
    Mat_ಅಭಿವ್ಯಕ್ತಿ en;
    e.op->add(e, Mat_ಅಭಿವ್ಯಕ್ತಿ(m), en);
    ಪ್ರತ್ಯಾಗಮ en;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ operator + (ಧ್ರುವ Mat& m, ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& e)
{
    checkOperandsExist(m);
    Mat_ಅಭಿವ್ಯಕ್ತಿ en;
    e.op->add(e, Mat_ಅಭಿವ್ಯಕ್ತಿ(m), en);
    ಪ್ರತ್ಯಾಗಮ en;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ operator + (ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& e, ಧ್ರುವ Scalar& s)
{
    Mat_ಅಭಿವ್ಯಕ್ತಿ en;
    e.op->add(e, s, en);
    ಪ್ರತ್ಯಾಗಮ en;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ operator + (ಧ್ರುವ Scalar& s, ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& e)
{
    Mat_ಅಭಿವ್ಯಕ್ತಿ en;
    e.op->add(e, s, en);
    ಪ್ರತ್ಯಾಗಮ en;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ operator + (ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& e1, ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& e2)
{
    Mat_ಅಭಿವ್ಯಕ್ತಿ en;
    e1.op->add(e1, e2, en);
    ಪ್ರತ್ಯಾಗಮ en;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ operator - (ಧ್ರುವ Mat& a, ಧ್ರುವ Mat& b)
{
    checkOperandsExist(a, b);
    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    MatOp_AddEx::make_ಅಭಿವ್ಯಕ್ತಿ(e, a, b, 1, -1);
    ಪ್ರತ್ಯಾಗಮ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ operator - (ಧ್ರುವ Mat& a, ಧ್ರುವ Scalar& s)
{
    checkOperandsExist(a);
    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    MatOp_AddEx::make_ಅಭಿವ್ಯಕ್ತಿ(e, a, Mat(), 1, 0, -s);
    ಪ್ರತ್ಯಾಗಮ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ operator - (ಧ್ರುವ Scalar& s, ಧ್ರುವ Mat& a)
{
    checkOperandsExist(a);
    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    MatOp_AddEx::make_ಅಭಿವ್ಯಕ್ತಿ(e, a, Mat(), -1, 0, s);
    ಪ್ರತ್ಯಾಗಮ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ operator - (ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& e, ಧ್ರುವ Mat& m)
{
    checkOperandsExist(m);
    Mat_ಅಭಿವ್ಯಕ್ತಿ en;
    e.op->subtract(e, Mat_ಅಭಿವ್ಯಕ್ತಿ(m), en);
    ಪ್ರತ್ಯಾಗಮ en;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ operator - (ಧ್ರುವ Mat& m, ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& e)
{
    checkOperandsExist(m);
    Mat_ಅಭಿವ್ಯಕ್ತಿ en;
    e.op->subtract(Mat_ಅಭಿವ್ಯಕ್ತಿ(m), e, en);
    ಪ್ರತ್ಯಾಗಮ en;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ operator - (ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& e, ಧ್ರುವ Scalar& s)
{
    Mat_ಅಭಿವ್ಯಕ್ತಿ en;
    e.op->add(e, -s, en);
    ಪ್ರತ್ಯಾಗಮ en;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ operator - (ಧ್ರುವ Scalar& s, ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& e)
{
    Mat_ಅಭಿವ್ಯಕ್ತಿ en;
    e.op->subtract(s, e, en);
    ಪ್ರತ್ಯಾಗಮ en;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ operator - (ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& e1, ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& e2)
{
    Mat_ಅಭಿವ್ಯಕ್ತಿ en;
    e1.op->subtract(e1, e2, en);
    ಪ್ರತ್ಯಾಗಮ en;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ operator - (ಧ್ರುವ Mat& m)
{
    checkOperandsExist(m);
    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    MatOp_AddEx::make_ಅಭಿವ್ಯಕ್ತಿ(e, m, Mat(), -1, 0);
    ಪ್ರತ್ಯಾಗಮ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ operator - (ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& e)
{
    Mat_ಅಭಿವ್ಯಕ್ತಿ en;
    e.op->subtract(Scalar(0), e, en);
    ಪ್ರತ್ಯಾಗಮ en;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ operator * (ಧ್ರುವ Mat& a, ಧ್ರುವ Mat& b)
{
    checkOperandsExist(a, b);
    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    MatOp_GEMM::make_ಅಭಿವ್ಯಕ್ತಿ(e, 0, a, b);
    ಪ್ರತ್ಯಾಗಮ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ operator * (ಧ್ರುವ Mat& a, ದ್ವಿಗುಣ s)
{
    checkOperandsExist(a);
    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    MatOp_AddEx::make_ಅಭಿವ್ಯಕ್ತಿ(e, a, Mat(), s, 0);
    ಪ್ರತ್ಯಾಗಮ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ operator * (ದ್ವಿಗುಣ s, ಧ್ರುವ Mat& a)
{
    checkOperandsExist(a);
    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    MatOp_AddEx::make_ಅಭಿವ್ಯಕ್ತಿ(e, a, Mat(), s, 0);
    ಪ್ರತ್ಯಾಗಮ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ operator * (ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& e, ಧ್ರುವ Mat& m)
{
    Mat_ಅಭಿವ್ಯಕ್ತಿ en;
    e.op->matmul(e, Mat_ಅಭಿವ್ಯಕ್ತಿ(m), en);
    ಪ್ರತ್ಯಾಗಮ en;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ operator * (ಧ್ರುವ Mat& m, ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& e)
{
    checkOperandsExist(m);
    Mat_ಅಭಿವ್ಯಕ್ತಿ en;
    e.op->matmul(Mat_ಅಭಿವ್ಯಕ್ತಿ(m), e, en);
    ಪ್ರತ್ಯಾಗಮ en;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ operator * (ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& e, ದ್ವಿಗುಣ s)
{
    Mat_ಅಭಿವ್ಯಕ್ತಿ en;
    e.op->multiply(e, s, en);
    ಪ್ರತ್ಯಾಗಮ en;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ operator * (ದ್ವಿಗುಣ s, ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& e)
{
    Mat_ಅಭಿವ್ಯಕ್ತಿ en;
    e.op->multiply(e, s, en);
    ಪ್ರತ್ಯಾಗಮ en;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ operator * (ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& e1, ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& e2)
{
    Mat_ಅಭಿವ್ಯಕ್ತಿ en;
    e1.op->matmul(e1, e2, en);
    ಪ್ರತ್ಯಾಗಮ en;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ operator / (ಧ್ರುವ Mat& a, ಧ್ರುವ Mat& b)
{
    checkOperandsExist(a, b);
    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    MatOp_Bin::make_ಅಭಿವ್ಯಕ್ತಿ(e, '/', a, b);
    ಪ್ರತ್ಯಾಗಮ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ operator / (ಧ್ರುವ Mat& a, ದ್ವಿಗುಣ s)
{
    checkOperandsExist(a);
    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    MatOp_AddEx::make_ಅಭಿವ್ಯಕ್ತಿ(e, a, Mat(), 1./s, 0);
    ಪ್ರತ್ಯಾಗಮ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ operator / (ದ್ವಿಗುಣ s, ಧ್ರುವ Mat& a)
{
    checkOperandsExist(a);
    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    MatOp_Bin::make_ಅಭಿವ್ಯಕ್ತಿ(e, '/', a, Mat(), s);
    ಪ್ರತ್ಯಾಗಮ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ operator / (ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& e, ಧ್ರುವ Mat& m)
{
    Mat_ಅಭಿವ್ಯಕ್ತಿ en;
    e.op->divide(e, Mat_ಅಭಿವ್ಯಕ್ತಿ(m), en);
    ಪ್ರತ್ಯಾಗಮ en;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ operator / (ಧ್ರುವ Mat& m, ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& e)
{
    checkOperandsExist(m);
    Mat_ಅಭಿವ್ಯಕ್ತಿ en;
    e.op->divide(Mat_ಅಭಿವ್ಯಕ್ತಿ(m), e, en);
    ಪ್ರತ್ಯಾಗಮ en;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ operator / (ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& e, ದ್ವಿಗುಣ s)
{
    Mat_ಅಭಿವ್ಯಕ್ತಿ en;
    e.op->multiply(e, 1./s, en);
    ಪ್ರತ್ಯಾಗಮ en;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ operator / (ದ್ವಿಗುಣ s, ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& e)
{
    Mat_ಅಭಿವ್ಯಕ್ತಿ en;
    e.op->divide(s, e, en);
    ಪ್ರತ್ಯಾಗಮ en;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ operator / (ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& e1, ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& e2)
{
    Mat_ಅಭಿವ್ಯಕ್ತಿ en;
    e1.op->divide(e1, e2, en);
    ಪ್ರತ್ಯಾಗಮ en;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ operator < (ಧ್ರುವ Mat& a, ಧ್ರುವ Mat& b)
{
    checkOperandsExist(a, b);
    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    MatOp_Cmp::make_ಅಭಿವ್ಯಕ್ತಿ(e, cv::CMP_LT, a, b);
    ಪ್ರತ್ಯಾಗಮ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ operator < (ಧ್ರುವ Mat& a, ದ್ವಿಗುಣ s)
{
    checkOperandsExist(a);
    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    MatOp_Cmp::make_ಅಭಿವ್ಯಕ್ತಿ(e, cv::CMP_LT, a, s);
    ಪ್ರತ್ಯಾಗಮ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ operator < (ದ್ವಿಗುಣ s, ಧ್ರುವ Mat& a)
{
    checkOperandsExist(a);
    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    MatOp_Cmp::make_ಅಭಿವ್ಯಕ್ತಿ(e, cv::CMP_GT, a, s);
    ಪ್ರತ್ಯಾಗಮ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ operator <= (ಧ್ರುವ Mat& a, ಧ್ರುವ Mat& b)
{
    checkOperandsExist(a, b);
    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    MatOp_Cmp::make_ಅಭಿವ್ಯಕ್ತಿ(e, cv::CMP_LE, a, b);
    ಪ್ರತ್ಯಾಗಮ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ operator <= (ಧ್ರುವ Mat& a, ದ್ವಿಗುಣ s)
{
    checkOperandsExist(a);
    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    MatOp_Cmp::make_ಅಭಿವ್ಯಕ್ತಿ(e, cv::CMP_LE, a, s);
    ಪ್ರತ್ಯಾಗಮ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ operator <= (ದ್ವಿಗುಣ s, ಧ್ರುವ Mat& a)
{
    checkOperandsExist(a);
    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    MatOp_Cmp::make_ಅಭಿವ್ಯಕ್ತಿ(e, cv::CMP_GE, a, s);
    ಪ್ರತ್ಯಾಗಮ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ operator == (ಧ್ರುವ Mat& a, ಧ್ರುವ Mat& b)
{
    checkOperandsExist(a, b);
    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    MatOp_Cmp::make_ಅಭಿವ್ಯಕ್ತಿ(e, cv::CMP_EQ, a, b);
    ಪ್ರತ್ಯಾಗಮ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ operator == (ಧ್ರುವ Mat& a, ದ್ವಿಗುಣ s)
{
    checkOperandsExist(a);
    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    MatOp_Cmp::make_ಅಭಿವ್ಯಕ್ತಿ(e, cv::CMP_EQ, a, s);
    ಪ್ರತ್ಯಾಗಮ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ operator == (ದ್ವಿಗುಣ s, ಧ್ರುವ Mat& a)
{
    checkOperandsExist(a);
    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    MatOp_Cmp::make_ಅಭಿವ್ಯಕ್ತಿ(e, cv::CMP_EQ, a, s);
    ಪ್ರತ್ಯಾಗಮ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ operator != (ಧ್ರುವ Mat& a, ಧ್ರುವ Mat& b)
{
    checkOperandsExist(a, b);
    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    MatOp_Cmp::make_ಅಭಿವ್ಯಕ್ತಿ(e, cv::CMP_NE, a, b);
    ಪ್ರತ್ಯಾಗಮ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ operator != (ಧ್ರುವ Mat& a, ದ್ವಿಗುಣ s)
{
    checkOperandsExist(a);
    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    MatOp_Cmp::make_ಅಭಿವ್ಯಕ್ತಿ(e, cv::CMP_NE, a, s);
    ಪ್ರತ್ಯಾಗಮ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ operator != (ದ್ವಿಗುಣ s, ಧ್ರುವ Mat& a)
{
    checkOperandsExist(a);
    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    MatOp_Cmp::make_ಅಭಿವ್ಯಕ್ತಿ(e, cv::CMP_NE, a, s);
    ಪ್ರತ್ಯಾಗಮ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ operator >= (ಧ್ರುವ Mat& a, ಧ್ರುವ Mat& b)
{
    checkOperandsExist(a, b);
    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    MatOp_Cmp::make_ಅಭಿವ್ಯಕ್ತಿ(e, cv::CMP_GE, a, b);
    ಪ್ರತ್ಯಾಗಮ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ operator >= (ಧ್ರುವ Mat& a, ದ್ವಿಗುಣ s)
{
    checkOperandsExist(a);
    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    MatOp_Cmp::make_ಅಭಿವ್ಯಕ್ತಿ(e, cv::CMP_GE, a, s);
    ಪ್ರತ್ಯಾಗಮ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ operator >= (ದ್ವಿಗುಣ s, ಧ್ರುವ Mat& a)
{
    checkOperandsExist(a);
    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    MatOp_Cmp::make_ಅಭಿವ್ಯಕ್ತಿ(e, cv::CMP_LE, a, s);
    ಪ್ರತ್ಯಾಗಮ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ operator > (ಧ್ರುವ Mat& a, ಧ್ರುವ Mat& b)
{
    checkOperandsExist(a, b);
    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    MatOp_Cmp::make_ಅಭಿವ್ಯಕ್ತಿ(e, cv::CMP_GT, a, b);
    ಪ್ರತ್ಯಾಗಮ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ operator > (ಧ್ರುವ Mat& a, ದ್ವಿಗುಣ s)
{
    checkOperandsExist(a);
    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    MatOp_Cmp::make_ಅಭಿವ್ಯಕ್ತಿ(e, cv::CMP_GT, a, s);
    ಪ್ರತ್ಯಾಗಮ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ operator > (ದ್ವಿಗುಣ s, ಧ್ರುವ Mat& a)
{
    checkOperandsExist(a);
    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    MatOp_Cmp::make_ಅಭಿವ್ಯಕ್ತಿ(e, cv::CMP_LT, a, s);
    ಪ್ರತ್ಯಾಗಮ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ min(ಧ್ರುವ Mat& a, ಧ್ರುವ Mat& b)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a, b);
    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    MatOp_Bin::make_ಅಭಿವ್ಯಕ್ತಿ(e, 'm', a, b);
    ಪ್ರತ್ಯಾಗಮ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ min(ಧ್ರುವ Mat& a, ದ್ವಿಗುಣ s)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    MatOp_Bin::make_ಅಭಿವ್ಯಕ್ತಿ(e, 'n', a, s);
    ಪ್ರತ್ಯಾಗಮ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ min(ದ್ವಿಗುಣ s, ಧ್ರುವ Mat& a)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    MatOp_Bin::make_ಅಭಿವ್ಯಕ್ತಿ(e, 'n', a, s);
    ಪ್ರತ್ಯಾಗಮ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ max(ಧ್ರುವ Mat& a, ಧ್ರುವ Mat& b)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a, b);
    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    MatOp_Bin::make_ಅಭಿವ್ಯಕ್ತಿ(e, 'M', a, b);
    ಪ್ರತ್ಯಾಗಮ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ max(ಧ್ರುವ Mat& a, ದ್ವಿಗುಣ s)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    MatOp_Bin::make_ಅಭಿವ್ಯಕ್ತಿ(e, 'N', a, s);
    ಪ್ರತ್ಯಾಗಮ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ max(ದ್ವಿಗುಣ s, ಧ್ರುವ Mat& a)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    MatOp_Bin::make_ಅಭಿವ್ಯಕ್ತಿ(e, 'N', a, s);
    ಪ್ರತ್ಯಾಗಮ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ operator & (ಧ್ರುವ Mat& a, ಧ್ರುವ Mat& b)
{
    checkOperandsExist(a, b);
    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    MatOp_Bin::make_ಅಭಿವ್ಯಕ್ತಿ(e, '&', a, b);
    ಪ್ರತ್ಯಾಗಮ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ operator & (ಧ್ರುವ Mat& a, ಧ್ರುವ Scalar& s)
{
    checkOperandsExist(a);
    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    MatOp_Bin::make_ಅಭಿವ್ಯಕ್ತಿ(e, '&', a, s);
    ಪ್ರತ್ಯಾಗಮ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ operator & (ಧ್ರುವ Scalar& s, ಧ್ರುವ Mat& a)
{
    checkOperandsExist(a);
    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    MatOp_Bin::make_ಅಭಿವ್ಯಕ್ತಿ(e, '&', a, s);
    ಪ್ರತ್ಯಾಗಮ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ operator | (ಧ್ರುವ Mat& a, ಧ್ರುವ Mat& b)
{
    checkOperandsExist(a, b);
    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    MatOp_Bin::make_ಅಭಿವ್ಯಕ್ತಿ(e, '|', a, b);
    ಪ್ರತ್ಯಾಗಮ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ operator | (ಧ್ರುವ Mat& a, ಧ್ರುವ Scalar& s)
{
    checkOperandsExist(a);
    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    MatOp_Bin::make_ಅಭಿವ್ಯಕ್ತಿ(e, '|', a, s);
    ಪ್ರತ್ಯಾಗಮ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ operator | (ಧ್ರುವ Scalar& s, ಧ್ರುವ Mat& a)
{
    checkOperandsExist(a);
    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    MatOp_Bin::make_ಅಭಿವ್ಯಕ್ತಿ(e, '|', a, s);
    ಪ್ರತ್ಯಾಗಮ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ operator ^ (ಧ್ರುವ Mat& a, ಧ್ರುವ Mat& b)
{
    checkOperandsExist(a, b);
    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    MatOp_Bin::make_ಅಭಿವ್ಯಕ್ತಿ(e, '^', a, b);
    ಪ್ರತ್ಯಾಗಮ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ operator ^ (ಧ್ರುವ Mat& a, ಧ್ರುವ Scalar& s)
{
    checkOperandsExist(a);
    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    MatOp_Bin::make_ಅಭಿವ್ಯಕ್ತಿ(e, '^', a, s);
    ಪ್ರತ್ಯಾಗಮ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ operator ^ (ಧ್ರುವ Scalar& s, ಧ್ರುವ Mat& a)
{
    checkOperandsExist(a);
    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    MatOp_Bin::make_ಅಭಿವ್ಯಕ್ತಿ(e, '^', a, s);
    ಪ್ರತ್ಯಾಗಮ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ operator ~(ಧ್ರುವ Mat& a)
{
    checkOperandsExist(a);
    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    MatOp_Bin::make_ಅಭಿವ್ಯಕ್ತಿ(e, '~', a, Scalar());
    ಪ್ರತ್ಯಾಗಮ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ abs(ಧ್ರುವ Mat& a)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    MatOp_Bin::make_ಅಭಿವ್ಯಕ್ತಿ(e, 'a', a, Scalar());
    ಪ್ರತ್ಯಾಗಮ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ abs(ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& e)
{
    CV_INSTRUMENT_REGION();

    Mat_ಅಭಿವ್ಯಕ್ತಿ en;
    e.op->abs(e, en);
    ಪ್ರತ್ಯಾಗಮ en;
}


Size Mat_ಅಭಿವ್ಯಕ್ತಿ::size() ಧ್ರುವ
{
    ಯದಿ( isT(*ಏತತ್) || isInv(*ಏತತ್) )
        ಪ್ರತ್ಯಾಗಮ Size(a.rows, a.cols);
    ಯದಿ( isSolve(*ಏತತ್) )
        ಪ್ರತ್ಯಾಗಮ Size(b.cols, a.cols);
    ಯದಿ( isInitializer(*ಏತತ್) )
        ಪ್ರತ್ಯಾಗಮ a.size();
    ಪ್ರತ್ಯಾಗಮ op ? op->size(*ಏತತ್) : Size();
}


ಪೂರ್ಣಾಙ್ಕ Mat_ಅಭಿವ್ಯಕ್ತಿ::type() ಧ್ರುವ
{
    CV_INSTRUMENT_REGION();

    ಯದಿ( isInitializer(*ಏತತ್) )
        ಪ್ರತ್ಯಾಗಮ a.type();
    ಯದಿ( isCmp(*ಏತತ್) )
        ಪ್ರತ್ಯಾಗಮ CV_MAKETYPE(CV_8U, a.channels());
    ಪ್ರತ್ಯಾಗಮ op ? op->type(*ಏತತ್) : -1;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////

ಶೂನ್ಯ MatOp_Identity::assign(ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& e, Mat& m, ಪೂರ್ಣಾಙ್ಕ _type) ಧ್ರುವ
{
    ಯದಿ( _type == -1 || _type == e.a.type() )
        m = e.a;
    ಅನ್ಯಥಾ
    {
        CV_Assert( CV_MAT_CN(_type) == e.a.channels() );
        e.a.convertTo(m, _type);
    }
}

ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ MatOp_Identity::make_ಅಭಿವ್ಯಕ್ತಿ(Mat_ಅಭಿವ್ಯಕ್ತಿ& res, ಧ್ರುವ Mat& m)
{
    res = Mat_ಅಭಿವ್ಯಕ್ತಿ(&g_MatOp_Identity, 0, m, Mat(), Mat(), 1, 0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

ಶೂನ್ಯ MatOp_AddEx::assign(ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& e, Mat& m, ಪೂರ್ಣಾಙ್ಕ _type) ಧ್ರುವ
{
    Mat temp, &dst = _type == -1 || e.a.type() == _type ? m : temp;
    ಯದಿ( e.b.data )
    {
        ಯದಿ( e.s == Scalar() || !e.s.isReal() )
        {
            ಯದಿ( e.alpha == 1 )
            {
                ಯದಿ( e.beta == 1 )
                    cv::add(e.a, e.b, dst);
                ಅನ್ಯಥಾ ಯದಿ( e.beta == -1 )
                    cv::subtract(e.a, e.b, dst);
                ಅನ್ಯಥಾ
                    cv::scaleAdd(e.b, e.beta, e.a, dst);
            }
            ಅನ್ಯಥಾ ಯದಿ( e.beta == 1 )
            {
                ಯದಿ( e.alpha == -1 )
                    cv::subtract(e.b, e.a, dst);
                ಅನ್ಯಥಾ
                    cv::scaleAdd(e.a, e.alpha, e.b, dst);
            }
            ಅನ್ಯಥಾ
                cv::addWeighted(e.a, e.alpha, e.b, e.beta, 0, dst);

            ಯದಿ( !e.s.isReal() )
                cv::add(dst, e.s, dst);
        }
        ಅನ್ಯಥಾ
        {
            ಯದಿ (e.a.channels() > 1)
                CV_LOG_ONCE_WARNING(ರಿಕ್ತ, "OpenCV/MatExpr: processing of multi-channel arrays might be changed in the future: "
                                          "https://github.com/opencv/opencv/issues/16739");
            cv::addWeighted(e.a, e.alpha, e.b, e.beta, e.s[0], dst);
        }
    }
    ಅನ್ಯಥಾ ಯದಿ( e.s.isReal() && (dst.data != m.data || fabs(e.alpha) != 1))
    {
        ಯದಿ (e.a.channels() > 1 && e.s[0] != 0.0)
            CV_LOG_ONCE_WARNING(ರಿಕ್ತ, "OpenCV/MatExpr: processing of multi-channel arrays might be changed in the future: "
                                      "https://github.com/opencv/opencv/issues/16739");
        e.a.convertTo(m, _type, e.alpha, e.s[0]);
        ಪ್ರತ್ಯಾಗಮ;
    }
    ಅನ್ಯಥಾ ಯದಿ( e.alpha == 1 )
        cv::add(e.a, e.s, dst);
    ಅನ್ಯಥಾ ಯದಿ( e.alpha == -1 )
        cv::subtract(e.s, e.a, dst);
    ಅನ್ಯಥಾ
    {
        e.a.convertTo(dst, e.a.type(), e.alpha);
        cv::add(dst, e.s, dst);
    }

    ಯದಿ( dst.data != m.data )
        dst.convertTo(m, m.type());
}


ಶೂನ್ಯ MatOp_AddEx::add(ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& e, ಧ್ರುವ Scalar& s, Mat_ಅಭಿವ್ಯಕ್ತಿ& res) ಧ್ರುವ
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.s += s;
}


ಶೂನ್ಯ MatOp_AddEx::subtract(ಧ್ರುವ Scalar& s, ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& e, Mat_ಅಭಿವ್ಯಕ್ತಿ& res) ಧ್ರುವ
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha = -res.alpha;
    res.beta = -res.beta;
    res.s = s - res.s;
}

ಶೂನ್ಯ MatOp_AddEx::multiply(ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& e, ದ್ವಿಗುಣ s, Mat_ಅಭಿವ್ಯಕ್ತಿ& res) ಧ್ರುವ
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha *= s;
    res.beta *= s;
    res.s *= s;
}

ಶೂನ್ಯ MatOp_AddEx::divide(ದ್ವಿಗುಣ s, ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& e, Mat_ಅಭಿವ್ಯಕ್ತಿ& res) ಧ್ರುವ
{
    CV_INSTRUMENT_REGION();

    ಯದಿ( isScaled(e) )
        MatOp_Bin::make_ಅಭಿವ್ಯಕ್ತಿ(res, '/', e.a, Mat(), s/e.alpha);
    ಅನ್ಯಥಾ
        MatOp::divide(s, e, res);
}


ಶೂನ್ಯ MatOp_AddEx::transpose(ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& e, Mat_ಅಭಿವ್ಯಕ್ತಿ& res) ಧ್ರುವ
{
    CV_INSTRUMENT_REGION();

    ಯದಿ( isScaled(e) )
        MatOp_T::make_ಅಭಿವ್ಯಕ್ತಿ(res, e.a, e.alpha);
    ಅನ್ಯಥಾ
        MatOp::transpose(e, res);
}

ಶೂನ್ಯ MatOp_AddEx::abs(ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& e, Mat_ಅಭಿವ್ಯಕ್ತಿ& res) ಧ್ರುವ
{
    CV_INSTRUMENT_REGION();

    ಯದಿ( (!e.b.data || e.beta == 0) && fabs(e.alpha) == 1 )
        MatOp_Bin::make_ಅಭಿವ್ಯಕ್ತಿ(res, 'a', e.a, -e.s*e.alpha);
    ಅನ್ಯಥಾ ಯದಿ( e.b.data && e.alpha + e.beta == 0 && e.alpha*e.beta == -1 )
        MatOp_Bin::make_ಅಭಿವ್ಯಕ್ತಿ(res, 'a', e.a, e.b);
    ಅನ್ಯಥಾ
        MatOp::abs(e, res);
}

ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ MatOp_AddEx::make_ಅಭಿವ್ಯಕ್ತಿ(Mat_ಅಭಿವ್ಯಕ್ತಿ& res, ಧ್ರುವ Mat& a, ಧ್ರುವ Mat& b, ದ್ವಿಗುಣ alpha, ದ್ವಿಗುಣ beta, ಧ್ರುವ Scalar& s)
{
    res = Mat_ಅಭಿವ್ಯಕ್ತಿ(&g_MatOp_AddEx, 0, a, b, Mat(), alpha, beta, s);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ಶೂನ್ಯ MatOp_Bin::assign(ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& e, Mat& m, ಪೂರ್ಣಾಙ್ಕ _type) ಧ್ರುವ
{
    Mat temp, &dst = _type == -1 || e.a.type() == _type ? m : temp;

    ಯದಿ( e.flags == '*' )
        cv::multiply(e.a, e.b, dst, e.alpha);
    ಅನ್ಯಥಾ ಯದಿ( e.flags == '/' && e.b.data )
        cv::divide(e.a, e.b, dst, e.alpha);
    ಅನ್ಯಥಾ ಯದಿ( e.flags == '/' && !e.b.data )
        cv::divide(e.alpha, e.a, dst );
    ಅನ್ಯಥಾ ಯದಿ( e.flags == '&' && e.b.data )
        bitwise_and(e.a, e.b, dst);
    ಅನ್ಯಥಾ ಯದಿ( e.flags == '&' && !e.b.data )
        bitwise_and(e.a, e.s, dst);
    ಅನ್ಯಥಾ ಯದಿ( e.flags == '|' && e.b.data )
        bitwise_or(e.a, e.b, dst);
    ಅನ್ಯಥಾ ಯದಿ( e.flags == '|' && !e.b.data )
        bitwise_or(e.a, e.s, dst);
    ಅನ್ಯಥಾ ಯದಿ( e.flags == '^' && e.b.data )
        bitwise_xor(e.a, e.b, dst);
    ಅನ್ಯಥಾ ಯದಿ( e.flags == '^' && !e.b.data )
        bitwise_xor(e.a, e.s, dst);
    ಅನ್ಯಥಾ ಯದಿ( e.flags == '~' && !e.b.data )
        bitwise_not(e.a, dst);
    ಅನ್ಯಥಾ ಯದಿ( e.flags == 'm' )
        cv::min(e.a, e.b, dst);
    ಅನ್ಯಥಾ ಯದಿ( e.flags == 'n' )
        cv::min(e.a, e.s[0], dst);
    ಅನ್ಯಥಾ ಯದಿ( e.flags == 'M' )
        cv::max(e.a, e.b, dst);
    ಅನ್ಯಥಾ ಯದಿ( e.flags == 'N' )
        cv::max(e.a, e.s[0], dst);
    ಅನ್ಯಥಾ ಯದಿ( e.flags == 'a' && e.b.data )
        cv::absdiff(e.a, e.b, dst);
    ಅನ್ಯಥಾ ಯದಿ( e.flags == 'a' && !e.b.data )
        cv::absdiff(e.a, e.s, dst);
    ಅನ್ಯಥಾ
        CV_Error(cv::Error::StsError, "Unknown operation");

    ಯದಿ( dst.data != m.data )
        dst.convertTo(m, _type);
}

ಶೂನ್ಯ MatOp_Bin::multiply(ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& e, ದ್ವಿಗುಣ s, Mat_ಅಭಿವ್ಯಕ್ತಿ& res) ಧ್ರುವ
{
    CV_INSTRUMENT_REGION();

    ಯದಿ( e.flags == '*' || e.flags == '/' )
    {
        res = e;
        res.alpha *= s;
    }
    ಅನ್ಯಥಾ
        MatOp::multiply(e, s, res);
}

ಶೂನ್ಯ MatOp_Bin::divide(ದ್ವಿಗುಣ s, ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& e, Mat_ಅಭಿವ್ಯಕ್ತಿ& res) ಧ್ರುವ
{
    CV_INSTRUMENT_REGION();

    ಯದಿ( e.flags == '/' && (!e.b.data || e.beta == 0) )
        MatOp_AddEx::make_ಅಭಿವ್ಯಕ್ತಿ(res, e.a, Mat(), s/e.alpha, 0);
    ಅನ್ಯಥಾ
        MatOp::divide(s, e, res);
}

ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ MatOp_Bin::make_ಅಭಿವ್ಯಕ್ತಿ(Mat_ಅಭಿವ್ಯಕ್ತಿ& res, ವರ್ಣ op, ಧ್ರುವ Mat& a, ಧ್ರುವ Mat& b, ದ್ವಿಗುಣ scale)
{
    res = Mat_ಅಭಿವ್ಯಕ್ತಿ(&g_MatOp_Bin, op, a, b, Mat(), scale, b.data ? 1 : 0);
}

ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ MatOp_Bin::make_ಅಭಿವ್ಯಕ್ತಿ(Mat_ಅಭಿವ್ಯಕ್ತಿ& res, ವರ್ಣ op, ಧ್ರುವ Mat& a, ಧ್ರುವ Scalar& s)
{
    res = Mat_ಅಭಿವ್ಯಕ್ತಿ(&g_MatOp_Bin, op, a, Mat(), Mat(), 1, 0, s);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

ಶೂನ್ಯ MatOp_Cmp::assign(ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& e, Mat& m, ಪೂರ್ಣಾಙ್ಕ _type) ಧ್ರುವ
{
    Mat temp, &dst = _type == -1 || _type == CV_8U ? m : temp;

    ಯದಿ( e.b.data )
        cv::compare(e.a, e.b, dst, e.flags);
    ಅನ್ಯಥಾ
        cv::compare(e.a, e.alpha, dst, e.flags);

    ಯದಿ( dst.data != m.data )
        dst.convertTo(m, _type);
}

ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ MatOp_Cmp::make_ಅಭಿವ್ಯಕ್ತಿ(Mat_ಅಭಿವ್ಯಕ್ತಿ& res, ಪೂರ್ಣಾಙ್ಕ cmpop, ಧ್ರುವ Mat& a, ಧ್ರುವ Mat& b)
{
    res = Mat_ಅಭಿವ್ಯಕ್ತಿ(&g_MatOp_Cmp, cmpop, a, b, Mat(), 1, 1);
}

ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ MatOp_Cmp::make_ಅಭಿವ್ಯಕ್ತಿ(Mat_ಅಭಿವ್ಯಕ್ತಿ& res, ಪೂರ್ಣಾಙ್ಕ cmpop, ಧ್ರುವ Mat& a, ದ್ವಿಗುಣ alpha)
{
    res = Mat_ಅಭಿವ್ಯಕ್ತಿ(&g_MatOp_Cmp, cmpop, a, Mat(), Mat(), alpha, 1);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

ಶೂನ್ಯ MatOp_T::assign(ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& e, Mat& m, ಪೂರ್ಣಾಙ್ಕ _type) ಧ್ರುವ
{
    Mat temp, &dst = _type == -1 || _type == e.a.type() ? m : temp;

    cv::transpose(e.a, dst);

    ಯದಿ( dst.data != m.data || e.alpha != 1 )
        dst.convertTo(m, _type, e.alpha);
}

ಶೂನ್ಯ MatOp_T::multiply(ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& e, ದ್ವಿಗುಣ s, Mat_ಅಭಿವ್ಯಕ್ತಿ& res) ಧ್ರುವ
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha *= s;
}

ಶೂನ್ಯ MatOp_T::transpose(ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& e, Mat_ಅಭಿವ್ಯಕ್ತಿ& res) ಧ್ರುವ
{
    CV_INSTRUMENT_REGION();

    ಯದಿ( e.alpha == 1 )
        MatOp_Identity::make_ಅಭಿವ್ಯಕ್ತಿ(res, e.a);
    ಅನ್ಯಥಾ
        MatOp_AddEx::make_ಅಭಿವ್ಯಕ್ತಿ(res, e.a, Mat(), e.alpha, 0);
}

ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ MatOp_T::make_ಅಭಿವ್ಯಕ್ತಿ(Mat_ಅಭಿವ್ಯಕ್ತಿ& res, ಧ್ರುವ Mat& a, ದ್ವಿಗುಣ alpha)
{
    res = Mat_ಅಭಿವ್ಯಕ್ತಿ(&g_MatOp_T, 0, a, Mat(), Mat(), alpha, 0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

ಶೂನ್ಯ MatOp_GEMM::assign(ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& e, Mat& m, ಪೂರ್ಣಾಙ್ಕ _type) ಧ್ರುವ
{
    Mat temp, &dst = _type == -1 || _type == e.a.type() ? m : temp;

    cv::gemm(e.a, e.b, e.alpha, e.c, e.beta, dst, e.flags);
    ಯದಿ( dst.data != m.data )
        dst.convertTo(m, _type);
}

ಶೂನ್ಯ MatOp_GEMM::add(ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& e1, ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& e2, Mat_ಅಭಿವ್ಯಕ್ತಿ& res) ಧ್ರುವ
{
    CV_INSTRUMENT_REGION();

    ತಾರ್ಕಿಕ i1 = isIdentity(e1), i2 = isIdentity(e2);
    ದ್ವಿಗುಣ alpha1 = i1 ? 1 : e1.alpha, alpha2 = i2 ? 1 : e2.alpha;

    ಯದಿ( isMatProd(e1) && (i2 || isScaled(e2) || isT(e2)) )
        MatOp_GEMM::make_ಅಭಿವ್ಯಕ್ತಿ(res, (e1.flags & ~cv::GEMM_3_T)|(isT(e2) ? cv::GEMM_3_T : 0),
                             e1.a, e1.b, alpha1, e2.a, alpha2);
    ಅನ್ಯಥಾ ಯದಿ( isMatProd(e2) && (i1 || isScaled(e1) || isT(e1)) )
        MatOp_GEMM::make_ಅಭಿವ್ಯಕ್ತಿ(res, (e2.flags & ~cv::GEMM_3_T)|(isT(e1) ? cv::GEMM_3_T : 0),
                             e2.a, e2.b, alpha2, e1.a, alpha1);
    ಅನ್ಯಥಾ ಯದಿ( ಏತತ್ == e2.op )
        MatOp::add(e1, e2, res);
    ಅನ್ಯಥಾ
        e2.op->add(e1, e2, res);
}

ಶೂನ್ಯ MatOp_GEMM::subtract(ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& e1, ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& e2, Mat_ಅಭಿವ್ಯಕ್ತಿ& res) ಧ್ರುವ
{
    CV_INSTRUMENT_REGION();

    ತಾರ್ಕಿಕ i1 = isIdentity(e1), i2 = isIdentity(e2);
    ದ್ವಿಗುಣ alpha1 = i1 ? 1 : e1.alpha, alpha2 = i2 ? 1 : e2.alpha;

    ಯದಿ( isMatProd(e1) && (i2 || isScaled(e2) || isT(e2)) )
        MatOp_GEMM::make_ಅಭಿವ್ಯಕ್ತಿ(res, (e1.flags & ~cv::GEMM_3_T)|(isT(e2) ? cv::GEMM_3_T : 0),
                             e1.a, e1.b, alpha1, e2.a, -alpha2);
    ಅನ್ಯಥಾ ಯದಿ( isMatProd(e2) && (i1 || isScaled(e1) || isT(e1)) )
        MatOp_GEMM::make_ಅಭಿವ್ಯಕ್ತಿ(res, (e2.flags & ~cv::GEMM_3_T)|(isT(e1) ? cv::GEMM_3_T : 0),
                            e2.a, e2.b, -alpha2, e1.a, alpha1);
    ಅನ್ಯಥಾ ಯದಿ( ಏತತ್ == e2.op )
        MatOp::subtract(e1, e2, res);
    ಅನ್ಯಥಾ
        e2.op->subtract(e1, e2, res);
}

ಶೂನ್ಯ MatOp_GEMM::multiply(ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& e, ದ್ವಿಗುಣ s, Mat_ಅಭಿವ್ಯಕ್ತಿ& res) ಧ್ರುವ
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha *= s;
    res.beta *= s;
}

ಶೂನ್ಯ MatOp_GEMM::transpose(ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& e, Mat_ಅಭಿವ್ಯಕ್ತಿ& res) ಧ್ರುವ
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.flags = (!(e.flags & cv::GEMM_1_T) ? cv::GEMM_2_T : 0) |
                (!(e.flags & cv::GEMM_2_T) ? cv::GEMM_1_T : 0) |
                (!(e.flags & cv::GEMM_3_T) ? cv::GEMM_3_T : 0);
    swap(res.a, res.b);
}

ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ MatOp_GEMM::make_ಅಭಿವ್ಯಕ್ತಿ(Mat_ಅಭಿವ್ಯಕ್ತಿ& res, ಪೂರ್ಣಾಙ್ಕ flags, ಧ್ರುವ Mat& a, ಧ್ರುವ Mat& b,
                                 ದ್ವಿಗುಣ alpha, ಧ್ರುವ Mat& c, ದ್ವಿಗುಣ beta)
{
    res = Mat_ಅಭಿವ್ಯಕ್ತಿ(&g_MatOp_GEMM, flags, a, b, c, alpha, beta);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

ಶೂನ್ಯ MatOp_Invert::assign(ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& e, Mat& m, ಪೂರ್ಣಾಙ್ಕ _type) ಧ್ರುವ
{
    Mat temp, &dst = _type == -1 || _type == e.a.type() ? m : temp;

    cv::invert(e.a, dst, e.flags);
    ಯದಿ( dst.data != m.data )
        dst.convertTo(m, _type);
}

ಶೂನ್ಯ MatOp_Invert::matmul(ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& e1, ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& e2, Mat_ಅಭಿವ್ಯಕ್ತಿ& res) ಧ್ರುವ
{
    ಯದಿ( isInv(e1) && isIdentity(e2) )
        MatOp_Solve::make_ಅಭಿವ್ಯಕ್ತಿ(res, e1.flags, e1.a, e2.a);
    ಅನ್ಯಥಾ ಯದಿ( ಏತತ್ == e2.op )
        MatOp::matmul(e1, e2, res);
    ಅನ್ಯಥಾ
        e2.op->matmul(e1, e2, res);
}

ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ MatOp_Invert::make_ಅಭಿವ್ಯಕ್ತಿ(Mat_ಅಭಿವ್ಯಕ್ತಿ& res, ಪೂರ್ಣಾಙ್ಕ method, ಧ್ರುವ Mat& m)
{
    res = Mat_ಅಭಿವ್ಯಕ್ತಿ(&g_MatOp_Invert, method, m, Mat(), Mat(), 1, 0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

ಶೂನ್ಯ MatOp_Solve::assign(ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& e, Mat& m, ಪೂರ್ಣಾಙ್ಕ _type) ಧ್ರುವ
{
    Mat temp, &dst = _type == -1 || _type == e.a.type() ? m : temp;

    cv::solve(e.a, e.b, dst, e.flags);
    ಯದಿ( dst.data != m.data )
        dst.convertTo(m, _type);
}

ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ MatOp_Solve::make_ಅಭಿವ್ಯಕ್ತಿ(Mat_ಅಭಿವ್ಯಕ್ತಿ& res, ಪೂರ್ಣಾಙ್ಕ method, ಧ್ರುವ Mat& a, ಧ್ರುವ Mat& b)
{
    res = Mat_ಅಭಿವ್ಯಕ್ತಿ(&g_MatOp_Solve, method, a, b, Mat(), 1, 1);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

ಶೂನ್ಯ MatOp_Initializer::assign(ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& e, Mat& m, ಪೂರ್ಣಾಙ್ಕ _type) ಧ್ರುವ
{
    ಯದಿ( _type == -1 )
        _type = e.a.type();

    m.create(e.a.size, _type);

    ಯದಿ( e.flags == 'I' && e.a.dims <= 2 )
        setIdentity(m, Scalar(e.alpha));
    ಅನ್ಯಥಾ ಯದಿ( e.flags == '0' )
        m = Scalar();
    ಅನ್ಯಥಾ ಯದಿ( e.flags == '1' )
        m = Scalar(e.alpha);
    ಅನ್ಯಥಾ
        CV_Error(cv::Error::StsError, "Invalid matrix initializer type");
}

ಶೂನ್ಯ MatOp_Initializer::multiply(ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& e, ದ್ವಿಗುಣ s, Mat_ಅಭಿವ್ಯಕ್ತಿ& res) ಧ್ರುವ
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha *= s;
}

ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ MatOp_Initializer::make_ಅಭಿವ್ಯಕ್ತಿ(Mat_ಅಭಿವ್ಯಕ್ತಿ& res, ಪೂರ್ಣಾಙ್ಕ method, Size sz, ಪೂರ್ಣಾಙ್ಕ type, ದ್ವಿಗುಣ alpha)
{
    res = Mat_ಅಭಿವ್ಯಕ್ತಿ(getGlobalMatOpInitializer(), method, Mat(sz, type, (ಶೂನ್ಯ*)(size_t)0xEEEEEEEE), Mat(), Mat(), alpha, 0);
}

ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ MatOp_Initializer::make_ಅಭಿವ್ಯಕ್ತಿ(Mat_ಅಭಿವ್ಯಕ್ತಿ& res, ಪೂರ್ಣಾಙ್ಕ method, ಪೂರ್ಣಾಙ್ಕ ndims, ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ* sizes, ಪೂರ್ಣಾಙ್ಕ type, ದ್ವಿಗುಣ alpha)
{
    res = Mat_ಅಭಿವ್ಯಕ್ತಿ(getGlobalMatOpInitializer(), method, Mat(ndims, sizes, type, (ಶೂನ್ಯ*)(size_t)0xEEEEEEEE), Mat(), Mat(), alpha, 0);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

Mat_ಅಭಿವ್ಯಕ್ತಿ Mat::t() ಧ್ರುವ
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(*ಏತತ್);
    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    MatOp_T::make_ಅಭಿವ್ಯಕ್ತಿ(e, *ಏತತ್);
    ಪ್ರತ್ಯಾಗಮ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ Mat::inv(ಪೂರ್ಣಾಙ್ಕ method) ಧ್ರುವ
{
    CV_INSTRUMENT_REGION();

    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    MatOp_Invert::make_ಅಭಿವ್ಯಕ್ತಿ(e, method, *ಏತತ್);
    ಪ್ರತ್ಯಾಗಮ e;
}


Mat_ಅಭಿವ್ಯಕ್ತಿ Mat::mul(ಒಳಹರಿವು_Array m, ದ್ವಿಗುಣ scale) ಧ್ರುವ
{
    CV_INSTRUMENT_REGION();

    Mat b = m.getMat();
    // Unless the argument is a refcounted Mat/UMat, the header returned by getMat() may be
    // a non-owning view of caller memory (e.g. a scalar bound to _InputArray(const double&),
    // a Matx or a Vec) that the returned MatExpr can outlive, so snapshot it.
    // See https://github.com/opencv/opencv/issues/23577
    ಯದಿ( !m.isMat() && !m.isUMat() )
        b = b.clone();

    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    MatOp_Bin::make_ಅಭಿವ್ಯಕ್ತಿ(e, '*', *ಏತತ್, b, scale);
    ಪ್ರತ್ಯಾಗಮ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ Mat::zeros(ಪೂರ್ಣಾಙ್ಕ rows, ಪೂರ್ಣಾಙ್ಕ cols, ಪೂರ್ಣಾಙ್ಕ type)
{
    CV_INSTRUMENT_REGION();

    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    MatOp_Initializer::make_ಅಭಿವ್ಯಕ್ತಿ(e, '0', Size(cols, rows), type);
    ಪ್ರತ್ಯಾಗಮ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ Mat::zeros(Size size, ಪೂರ್ಣಾಙ್ಕ type)
{
    CV_INSTRUMENT_REGION();

    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    MatOp_Initializer::make_ಅಭಿವ್ಯಕ್ತಿ(e, '0', size, type);
    ಪ್ರತ್ಯಾಗಮ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ Mat::zeros(ಪೂರ್ಣಾಙ್ಕ ndims, ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ* sizes, ಪೂರ್ಣಾಙ್ಕ type)
{
    CV_INSTRUMENT_REGION();

    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    MatOp_Initializer::make_ಅಭಿವ್ಯಕ್ತಿ(e, '0', ndims, sizes, type);
    ಪ್ರತ್ಯಾಗಮ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ Mat::zeros(ಧ್ರುವ MatShape& shape, ಪೂರ್ಣಾಙ್ಕ type)
{
    CV_INSTRUMENT_REGION();

    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    MatOp_Initializer::make_ಅಭಿವ್ಯಕ್ತಿ(e, '0', shape.dims, shape.p, type);
    ಪ್ರತ್ಯಾಗಮ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ Mat::ones(ಪೂರ್ಣಾಙ್ಕ rows, ಪೂರ್ಣಾಙ್ಕ cols, ಪೂರ್ಣಾಙ್ಕ type)
{
    CV_INSTRUMENT_REGION();

    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    MatOp_Initializer::make_ಅಭಿವ್ಯಕ್ತಿ(e, '1', Size(cols, rows), type);
    ಪ್ರತ್ಯಾಗಮ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ Mat::ones(Size size, ಪೂರ್ಣಾಙ್ಕ type)
{
    CV_INSTRUMENT_REGION();

    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    MatOp_Initializer::make_ಅಭಿವ್ಯಕ್ತಿ(e, '1', size, type);
    ಪ್ರತ್ಯಾಗಮ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ Mat::ones(ಪೂರ್ಣಾಙ್ಕ ndims, ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ* sizes, ಪೂರ್ಣಾಙ್ಕ type)
{
    CV_INSTRUMENT_REGION();

    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    MatOp_Initializer::make_ಅಭಿವ್ಯಕ್ತಿ(e, '1', ndims, sizes, type);
    ಪ್ರತ್ಯಾಗಮ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ Mat::ones(ಧ್ರುವ MatShape& shape, ಪೂರ್ಣಾಙ್ಕ type)
{
    CV_INSTRUMENT_REGION();

    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    MatOp_Initializer::make_ಅಭಿವ್ಯಕ್ತಿ(e, '1', shape.dims, shape.p, type);
    ಪ್ರತ್ಯಾಗಮ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ Mat::eye(ಪೂರ್ಣಾಙ್ಕ rows, ಪೂರ್ಣಾಙ್ಕ cols, ಪೂರ್ಣಾಙ್ಕ type)
{
    CV_INSTRUMENT_REGION();

    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    MatOp_Initializer::make_ಅಭಿವ್ಯಕ್ತಿ(e, 'I', Size(cols, rows), type);
    ಪ್ರತ್ಯಾಗಮ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ Mat::eye(Size size, ಪೂರ್ಣಾಙ್ಕ type)
{
    CV_INSTRUMENT_REGION();

    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    MatOp_Initializer::make_ಅಭಿವ್ಯಕ್ತಿ(e, 'I', size, type);
    ಪ್ರತ್ಯಾಗಮ e;
}

ಶೂನ್ಯ Mat_ಅಭಿವ್ಯಕ್ತಿ::swap(Mat_ಅಭಿವ್ಯಕ್ತಿ& other)
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

ಒಳಹರಿವು_Array_2::ಒಳಹರಿವು_Array_2(ಧ್ರುವ Mat_ಅಭಿವ್ಯಕ್ತಿ& ಅಭಿವ್ಯಕ್ತಿ)
{
    ಯದಿ (!isIdentity(ಅಭಿವ್ಯಕ್ತಿ))
    {
        Mat result = ಅಭಿವ್ಯಕ್ತಿ;  // TODO improve through refcount == 1 of expr.a (inplace operation is possible - except gemm?)
        Mat_ಅಭಿವ್ಯಕ್ತಿ result_ಅಭಿವ್ಯಕ್ತಿ(result);
        swap(const_cast<Mat_ಅಭಿವ್ಯಕ್ತಿ&>(ಅಭಿವ್ಯಕ್ತಿ), result_ಅಭಿವ್ಯಕ್ತಿ);
    }
    CV_Assert(isIdentity(ಅಭಿವ್ಯಕ್ತಿ));
    init(FIXED_TYPE + FIXED_SIZE + MAT + ACCESS_READ, &ಅಭಿವ್ಯಕ್ತಿ.a);
}

} // cv::
