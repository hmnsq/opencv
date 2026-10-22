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
ಸ್ಥಿರ ಶೂನ್ಯ checkOperandsExist(ಸ್ಥಿರಾಂಕ Mat& a)
{
    ಆದರೆ (a.empty())
    {
        CV_Error(cv::Error::StsBadArg, "Matrix operand is an empty matrix.");
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ checkOperandsExist(ಸ್ಥಿರಾಂಕ Mat& a, ಸ್ಥಿರಾಂಕ Mat& b)
{
    ಆದರೆ (a.empty() || b.empty())
    {
        CV_Error(cv::Error::StsBadArg, "One or more matrix operands are empty.");
    }
}


ವರ್ಗ Mat_Op_ತಟಸ್ಥ CV_FINAL : ಸಾರ್ವಜನಿಕ MatOp
{
ಸಾರ್ವಜನಿಕ:
    Mat_Op_ತಟಸ್ಥ() {}
    ವಾಸ್ತವಿಕ ~Mat_Op_ತಟಸ್ಥ() {}

    ತಾರ್ಕಿಕ elementWise(ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& /*ಅಭಿವ್ಯಕ್ತಿ*/) ಸ್ಥಿರಾಂಕ CV_OVERRIDE { ಹಿಂತಿರುಗಿ ನಿಜ; }
    ಶೂನ್ಯ assign(ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& ಅಭಿವ್ಯಕ್ತಿ, Mat& m, ಪೂರ್ಣಾಂಕ type=-1) ಸ್ಥಿರಾಂಕ CV_OVERRIDE;

    ಸ್ಥಿರ ಶೂನ್ಯ make_ಅಭಿವ್ಯಕ್ತಿ(Mat_ಅಭಿವ್ಯಕ್ತಿ& res, ಸ್ಥಿರಾಂಕ Mat& m);
};

ಸ್ಥಿರ Mat_Op_ತಟಸ್ಥ g_Mat_Op_ತಟಸ್ಥ;

ವರ್ಗ Mat_Op_ಕೂಡಿಸು_Ex CV_FINAL : ಸಾರ್ವಜನಿಕ MatOp
{
ಸಾರ್ವಜನಿಕ:
    Mat_Op_ಕೂಡಿಸು_Ex() {}
    ವಾಸ್ತವಿಕ ~Mat_Op_ಕೂಡಿಸು_Ex() {}

    ತಾರ್ಕಿಕ elementWise(ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& /*ಅಭಿವ್ಯಕ್ತಿ*/) ಸ್ಥಿರಾಂಕ CV_OVERRIDE { ಹಿಂತಿರುಗಿ ನಿಜ; }
    ಶೂನ್ಯ assign(ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& ಅಭಿವ್ಯಕ್ತಿ, Mat& m, ಪೂರ್ಣಾಂಕ type=-1) ಸ್ಥಿರಾಂಕ CV_OVERRIDE;

    ಶೂನ್ಯ ಕೂಡಿಸು(ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& e1, ಸ್ಥಿರಾಂಕ Scalar& s, Mat_ಅಭಿವ್ಯಕ್ತಿ& res) ಸ್ಥಿರಾಂಕ CV_OVERRIDE;
    ಶೂನ್ಯ ಕಳೆ(ಸ್ಥಿರಾಂಕ Scalar& s, ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& ಅಭಿವ್ಯಕ್ತಿ, Mat_ಅಭಿವ್ಯಕ್ತಿ& res) ಸ್ಥಿರಾಂಕ CV_OVERRIDE;
    ಶೂನ್ಯ multiply(ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& e1, ದ್ವಿಗುಣ s, Mat_ಅಭಿವ್ಯಕ್ತಿ& res) ಸ್ಥಿರಾಂಕ CV_OVERRIDE;
    ಶೂನ್ಯ divide(ದ್ವಿಗುಣ s, ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& e, Mat_ಅಭಿವ್ಯಕ್ತಿ& res) ಸ್ಥಿರಾಂಕ CV_OVERRIDE;

    ಶೂನ್ಯ transpose(ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& e1, Mat_ಅಭಿವ್ಯಕ್ತಿ& res) ಸ್ಥಿರಾಂಕ CV_OVERRIDE;
    ಶೂನ್ಯ ನಿರಪೇಕ್ಷ(ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& ಅಭಿವ್ಯಕ್ತಿ, Mat_ಅಭಿವ್ಯಕ್ತಿ& res) ಸ್ಥಿರಾಂಕ CV_OVERRIDE;

    ಸ್ಥಿರ ಶೂನ್ಯ make_ಅಭಿವ್ಯಕ್ತಿ(Mat_ಅಭಿವ್ಯಕ್ತಿ& res, ಸ್ಥಿರಾಂಕ Mat& a, ಸ್ಥಿರಾಂಕ Mat& b, ದ್ವಿಗುಣ alpha, ದ್ವಿಗುಣ beta, ಸ್ಥಿರಾಂಕ Scalar& s=Scalar());
};

ಸ್ಥಿರ Mat_Op_ಕೂಡಿಸು_Ex g_Mat_Op_ಕೂಡಿಸು_Ex;

ವರ್ಗ MatOp_Bin CV_FINAL : ಸಾರ್ವಜನಿಕ MatOp
{
ಸಾರ್ವಜನಿಕ:
    MatOp_Bin() {}
    ವಾಸ್ತವಿಕ ~MatOp_Bin() {}

    ತಾರ್ಕಿಕ elementWise(ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& /*ಅಭಿವ್ಯಕ್ತಿ*/) ಸ್ಥಿರಾಂಕ CV_OVERRIDE { ಹಿಂತಿರುಗಿ ನಿಜ; }
    ಶೂನ್ಯ assign(ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& ಅಭಿವ್ಯಕ್ತಿ, Mat& m, ಪೂರ್ಣಾಂಕ type=-1) ಸ್ಥಿರಾಂಕ CV_OVERRIDE;

    ಶೂನ್ಯ multiply(ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& e1, ದ್ವಿಗುಣ s, Mat_ಅಭಿವ್ಯಕ್ತಿ& res) ಸ್ಥಿರಾಂಕ CV_OVERRIDE;
    ಶೂನ್ಯ divide(ದ್ವಿಗುಣ s, ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& e, Mat_ಅಭಿವ್ಯಕ್ತಿ& res) ಸ್ಥಿರಾಂಕ CV_OVERRIDE;

    ಸ್ಥಿರ ಶೂನ್ಯ make_ಅಭಿವ್ಯಕ್ತಿ(Mat_ಅಭಿವ್ಯಕ್ತಿ& res, ಅಕ್ಷರ op, ಸ್ಥಿರಾಂಕ Mat& a, ಸ್ಥಿರಾಂಕ Mat& b, ದ್ವಿಗುಣ scale=1);
    ಸ್ಥಿರ ಶೂನ್ಯ make_ಅಭಿವ್ಯಕ್ತಿ(Mat_ಅಭಿವ್ಯಕ್ತಿ& res, ಅಕ್ಷರ op, ಸ್ಥಿರಾಂಕ Mat& a, ಸ್ಥಿರಾಂಕ Scalar& s);
};

ಸ್ಥಿರ MatOp_Bin g_MatOp_Bin;

ವರ್ಗ MatOp_Cmp CV_FINAL : ಸಾರ್ವಜನಿಕ MatOp
{
ಸಾರ್ವಜನಿಕ:
    MatOp_Cmp() {}
    ವಾಸ್ತವಿಕ ~MatOp_Cmp() {}

    ತಾರ್ಕಿಕ elementWise(ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& /*ಅಭಿವ್ಯಕ್ತಿ*/) ಸ್ಥಿರಾಂಕ CV_OVERRIDE { ಹಿಂತಿರುಗಿ ನಿಜ; }
    ಶೂನ್ಯ assign(ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& ಅಭಿವ್ಯಕ್ತಿ, Mat& m, ಪೂರ್ಣಾಂಕ type=-1) ಸ್ಥಿರಾಂಕ CV_OVERRIDE;

    ಸ್ಥಿರ ಶೂನ್ಯ make_ಅಭಿವ್ಯಕ್ತಿ(Mat_ಅಭಿವ್ಯಕ್ತಿ& res, ಪೂರ್ಣಾಂಕ cmpop, ಸ್ಥಿರಾಂಕ Mat& a, ಸ್ಥಿರಾಂಕ Mat& b);
    ಸ್ಥಿರ ಶೂನ್ಯ make_ಅಭಿವ್ಯಕ್ತಿ(Mat_ಅಭಿವ್ಯಕ್ತಿ& res, ಪೂರ್ಣಾಂಕ cmpop, ಸ್ಥಿರಾಂಕ Mat& a, ದ್ವಿಗುಣ alpha);
};

ಸ್ಥಿರ MatOp_Cmp g_MatOp_Cmp;

ವರ್ಗ MatOp_GEMM CV_FINAL : ಸಾರ್ವಜನಿಕ MatOp
{
ಸಾರ್ವಜನಿಕ:
    MatOp_GEMM() {}
    ವಾಸ್ತವಿಕ ~MatOp_GEMM() {}

    ತಾರ್ಕಿಕ elementWise(ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& /*ಅಭಿವ್ಯಕ್ತಿ*/) ಸ್ಥಿರಾಂಕ CV_OVERRIDE { ಹಿಂತಿರುಗಿ ಸುಳ್ಳು; }
    ಶೂನ್ಯ assign(ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& ಅಭಿವ್ಯಕ್ತಿ, Mat& m, ಪೂರ್ಣಾಂಕ type=-1) ಸ್ಥಿರಾಂಕ CV_OVERRIDE;

    ಶೂನ್ಯ ಕೂಡಿಸು(ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& e1, ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& e2, Mat_ಅಭಿವ್ಯಕ್ತಿ& res) ಸ್ಥಿರಾಂಕ CV_OVERRIDE;
    ಶೂನ್ಯ ಕಳೆ(ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& e1, ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& e2, Mat_ಅಭಿವ್ಯಕ್ತಿ& res) ಸ್ಥಿರಾಂಕ CV_OVERRIDE;
    ಶೂನ್ಯ multiply(ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& e, ದ್ವಿಗುಣ s, Mat_ಅಭಿವ್ಯಕ್ತಿ& res) ಸ್ಥಿರಾಂಕ CV_OVERRIDE;

    ಶೂನ್ಯ transpose(ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& ಅಭಿವ್ಯಕ್ತಿ, Mat_ಅಭಿವ್ಯಕ್ತಿ& res) ಸ್ಥಿರಾಂಕ CV_OVERRIDE;

    ಗಾತ್ರ_2 size(ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& ಅಭಿವ್ಯಕ್ತಿ) ಸ್ಥಿರಾಂಕ CV_OVERRIDE
    {
        ಹಿಂತಿರುಗಿ ಗಾತ್ರ_2(
            (ಅಭಿವ್ಯಕ್ತಿ.flags & GEMM_2_T) ? ಅಭಿವ್ಯಕ್ತಿ.b.ಅಡ್ಡಸಾಲುಗಳು : ಅಭಿವ್ಯಕ್ತಿ.b.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ,
            (ಅಭಿವ್ಯಕ್ತಿ.flags & GEMM_1_T) ? ಅಭಿವ್ಯಕ್ತಿ.a.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ : ಅಭಿವ್ಯಕ್ತಿ.a.ಅಡ್ಡಸಾಲುಗಳು
        );
    }

    ಸ್ಥಿರ ಶೂನ್ಯ make_ಅಭಿವ್ಯಕ್ತಿ(Mat_ಅಭಿವ್ಯಕ್ತಿ& res, ಪೂರ್ಣಾಂಕ flags, ಸ್ಥಿರಾಂಕ Mat& a, ಸ್ಥಿರಾಂಕ Mat& b,
                         ದ್ವಿಗುಣ alpha=1, ಸ್ಥಿರಾಂಕ Mat& c=Mat(), ದ್ವಿಗುಣ beta=1);

};

ಸ್ಥಿರ MatOp_GEMM g_MatOp_GEMM;

ವರ್ಗ MatOp_Invert CV_FINAL : ಸಾರ್ವಜನಿಕ MatOp
{
ಸಾರ್ವಜನಿಕ:
    MatOp_Invert() {}
    ವಾಸ್ತವಿಕ ~MatOp_Invert() {}

    ತಾರ್ಕಿಕ elementWise(ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& /*ಅಭಿವ್ಯಕ್ತಿ*/) ಸ್ಥಿರಾಂಕ CV_OVERRIDE { ಹಿಂತಿರುಗಿ ಸುಳ್ಳು; }
    ಶೂನ್ಯ assign(ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& ಅಭಿವ್ಯಕ್ತಿ, Mat& m, ಪೂರ್ಣಾಂಕ type=-1) ಸ್ಥಿರಾಂಕ CV_OVERRIDE;

    ಶೂನ್ಯ matmul(ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& expr1, ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& expr2, Mat_ಅಭಿವ್ಯಕ್ತಿ& res) ಸ್ಥಿರಾಂಕ CV_OVERRIDE;

    ಸ್ಥಿರ ಶೂನ್ಯ make_ಅಭಿವ್ಯಕ್ತಿ(Mat_ಅಭಿವ್ಯಕ್ತಿ& res, ಪೂರ್ಣಾಂಕ ವಿಧಾನ, ಸ್ಥಿರಾಂಕ Mat& m);
};

ಸ್ಥಿರ MatOp_Invert g_MatOp_Invert;

ವರ್ಗ MatOp_T CV_FINAL : ಸಾರ್ವಜನಿಕ MatOp
{
ಸಾರ್ವಜನಿಕ:
    MatOp_T() {}
    ವಾಸ್ತವಿಕ ~MatOp_T() {}

    ತಾರ್ಕಿಕ elementWise(ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& /*ಅಭಿವ್ಯಕ್ತಿ*/) ಸ್ಥಿರಾಂಕ CV_OVERRIDE { ಹಿಂತಿರುಗಿ ಸುಳ್ಳು; }
    ಶೂನ್ಯ assign(ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& ಅಭಿವ್ಯಕ್ತಿ, Mat& m, ಪೂರ್ಣಾಂಕ type=-1) ಸ್ಥಿರಾಂಕ CV_OVERRIDE;

    ಶೂನ್ಯ multiply(ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& e1, ದ್ವಿಗುಣ s, Mat_ಅಭಿವ್ಯಕ್ತಿ& res) ಸ್ಥಿರಾಂಕ CV_OVERRIDE;
    ಶೂನ್ಯ transpose(ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& ಅಭಿವ್ಯಕ್ತಿ, Mat_ಅಭಿವ್ಯಕ್ತಿ& res) ಸ್ಥಿರಾಂಕ CV_OVERRIDE;

    ಸ್ಥಿರ ಶೂನ್ಯ make_ಅಭಿವ್ಯಕ್ತಿ(Mat_ಅಭಿವ್ಯಕ್ತಿ& res, ಸ್ಥಿರಾಂಕ Mat& a, ದ್ವಿಗುಣ alpha=1);
};

ಸ್ಥಿರ MatOp_T g_MatOp_T;

ವರ್ಗ MatOp_Solve CV_FINAL : ಸಾರ್ವಜನಿಕ MatOp
{
ಸಾರ್ವಜನಿಕ:
    MatOp_Solve() {}
    ವಾಸ್ತವಿಕ ~MatOp_Solve() {}

    ತಾರ್ಕಿಕ elementWise(ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& /*ಅಭಿವ್ಯಕ್ತಿ*/) ಸ್ಥಿರಾಂಕ CV_OVERRIDE { ಹಿಂತಿರುಗಿ ಸುಳ್ಳು; }
    ಶೂನ್ಯ assign(ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& ಅಭಿವ್ಯಕ್ತಿ, Mat& m, ಪೂರ್ಣಾಂಕ type=-1) ಸ್ಥಿರಾಂಕ CV_OVERRIDE;

    ಸ್ಥಿರ ಶೂನ್ಯ make_ಅಭಿವ್ಯಕ್ತಿ(Mat_ಅಭಿವ್ಯಕ್ತಿ& res, ಪೂರ್ಣಾಂಕ ವಿಧಾನ, ಸ್ಥಿರಾಂಕ Mat& a, ಸ್ಥಿರಾಂಕ Mat& b);
};

ಸ್ಥಿರ MatOp_Solve g_MatOp_Solve;

ವರ್ಗ MatOp_Initializer CV_FINAL : ಸಾರ್ವಜನಿಕ MatOp
{
ಸಾರ್ವಜನಿಕ:
    MatOp_Initializer() {}
    ವಾಸ್ತವಿಕ ~MatOp_Initializer() {}

    ತಾರ್ಕಿಕ elementWise(ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& /*ಅಭಿವ್ಯಕ್ತಿ*/) ಸ್ಥಿರಾಂಕ CV_OVERRIDE { ಹಿಂತಿರುಗಿ ಸುಳ್ಳು; }
    ಶೂನ್ಯ assign(ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& ಅಭಿವ್ಯಕ್ತಿ, Mat& m, ಪೂರ್ಣಾಂಕ type=-1) ಸ್ಥಿರಾಂಕ CV_OVERRIDE;

    ಶೂನ್ಯ multiply(ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& e, ದ್ವಿಗುಣ s, Mat_ಅಭಿವ್ಯಕ್ತಿ& res) ಸ್ಥಿರಾಂಕ CV_OVERRIDE;

    ಸ್ಥಿರ ಶೂನ್ಯ make_ಅಭಿವ್ಯಕ್ತಿ(Mat_ಅಭಿವ್ಯಕ್ತಿ& res, ಪೂರ್ಣಾಂಕ ವಿಧಾನ, ಗಾತ್ರ_2 sz, ಪೂರ್ಣಾಂಕ type, ದ್ವಿಗುಣ alpha=1);
    ಸ್ಥಿರ ಶೂನ್ಯ make_ಅಭಿವ್ಯಕ್ತಿ(Mat_ಅಭಿವ್ಯಕ್ತಿ& res, ಪೂರ್ಣಾಂಕ ವಿಧಾನ, ಪೂರ್ಣಾಂಕ ndims, ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* sizes, ಪೂರ್ಣಾಂಕ type, ದ್ವಿಗುಣ alpha=1);
};

ಸ್ಥಿರ MatOp_Initializer* getGlobalMatOpInitializer()
{
    CV_SINGLETON_LAZY_INIT(MatOp_Initializer, ಹೊಸ MatOp_Initializer())
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ತಾರ್ಕಿಕ is_ತಟಸ್ಥ(ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& e) { ಹಿಂತಿರುಗಿ e.op == &g_Mat_Op_ತಟಸ್ಥ; }
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ತಾರ್ಕಿಕ is_ಕೂಡಿಸು_Ex(ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& e) { ಹಿಂತಿರುಗಿ e.op == &g_Mat_Op_ಕೂಡಿಸು_Ex; }
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ತಾರ್ಕಿಕ isScaled(ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& e) { ಹಿಂತಿರುಗಿ is_ಕೂಡಿಸು_Ex(e) && (!e.b.ದತ್ತಾಂಶ || e.beta == 0) && e.s == Scalar(); }
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ತಾರ್ಕಿಕ isBin(ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& e, ಅಕ್ಷರ c) { ಹಿಂತಿರುಗಿ e.op == &g_MatOp_Bin && e.flags == c; }
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ತಾರ್ಕಿಕ isCmp(ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& e) { ಹಿಂತಿರುಗಿ e.op == &g_MatOp_Cmp; }
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ತಾರ್ಕಿಕ isReciprocal(ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& e) { ಹಿಂತಿರುಗಿ isBin(e,'/') && (!e.b.ದತ್ತಾಂಶ || e.beta == 0); }
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ತಾರ್ಕಿಕ isT(ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& e) { ಹಿಂತಿರುಗಿ e.op == &g_MatOp_T; }
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ತಾರ್ಕಿಕ isInv(ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& e) { ಹಿಂತಿರುಗಿ e.op == &g_MatOp_Invert; }
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ತಾರ್ಕಿಕ isSolve(ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& e) { ಹಿಂತಿರುಗಿ e.op == &g_MatOp_Solve; }
//static inline bool isGEMM(const MatExpr& e) { return e.op == &g_MatOp_GEMM; }
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ತಾರ್ಕಿಕ isMatProd(ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& e) { ಹಿಂತಿರುಗಿ e.op == &g_MatOp_GEMM && (!e.c.ದತ್ತಾಂಶ || e.beta == 0); }
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ತಾರ್ಕಿಕ isInitializer(ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& e) { ಹಿಂತಿರುಗಿ e.op == getGlobalMatOpInitializer(); }

/////////////////////////////////////////////////////////////////////////////////////////////////////

MatOp::MatOp() {}
MatOp::~MatOp() {}


ತಾರ್ಕಿಕ MatOp::elementWise(ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& /*ಅಭಿವ್ಯಕ್ತಿ*/) ಸ್ಥಿರಾಂಕ
{
    ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
}

ಶೂನ್ಯ MatOp::roi(ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& ಅಭಿವ್ಯಕ್ತಿ, ಸ್ಥಿರಾಂಕ Range& ಅಡ್ಡಸಾಲು_Range_2, ಸ್ಥಿರಾಂಕ Range& ಲಂಬಸಾಲು_Range_2, Mat_ಅಭಿವ್ಯಕ್ತಿ& e) ಸ್ಥಿರಾಂಕ
{
    ಆದರೆ( elementWise(ಅಭಿವ್ಯಕ್ತಿ) )
    {
        e = Mat_ಅಭಿವ್ಯಕ್ತಿ(ಅಭಿವ್ಯಕ್ತಿ.op, ಅಭಿವ್ಯಕ್ತಿ.flags, Mat(), Mat(), Mat(),
                    ಅಭಿವ್ಯಕ್ತಿ.alpha, ಅಭಿವ್ಯಕ್ತಿ.beta, ಅಭಿವ್ಯಕ್ತಿ.s);
        ಆದರೆ(ಅಭಿವ್ಯಕ್ತಿ.a.ದತ್ತಾಂಶ)
            e.a = ಅಭಿವ್ಯಕ್ತಿ.a(ಅಡ್ಡಸಾಲು_Range_2, ಲಂಬಸಾಲು_Range_2);
        ಆದರೆ(ಅಭಿವ್ಯಕ್ತಿ.b.ದತ್ತಾಂಶ)
            e.b = ಅಭಿವ್ಯಕ್ತಿ.b(ಅಡ್ಡಸಾಲು_Range_2, ಲಂಬಸಾಲು_Range_2);
        ಆದರೆ(ಅಭಿವ್ಯಕ್ತಿ.c.ದತ್ತಾಂಶ)
            e.c = ಅಭಿವ್ಯಕ್ತಿ.c(ಅಡ್ಡಸಾಲು_Range_2, ಲಂಬಸಾಲು_Range_2);
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        Mat m;
        ಅಭಿವ್ಯಕ್ತಿ.op->assign(ಅಭಿವ್ಯಕ್ತಿ, m);
        e = Mat_ಅಭಿವ್ಯಕ್ತಿ(&g_Mat_Op_ತಟಸ್ಥ, 0, m(ಅಡ್ಡಸಾಲು_Range_2, ಲಂಬಸಾಲು_Range_2), Mat(), Mat());
    }
}

ಶೂನ್ಯ MatOp::diag(ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& ಅಭಿವ್ಯಕ್ತಿ, ಪೂರ್ಣಾಂಕ d, Mat_ಅಭಿವ್ಯಕ್ತಿ& e) ಸ್ಥಿರಾಂಕ
{
    ಆದರೆ( elementWise(ಅಭಿವ್ಯಕ್ತಿ) )
    {
        e = Mat_ಅಭಿವ್ಯಕ್ತಿ(ಅಭಿವ್ಯಕ್ತಿ.op, ಅಭಿವ್ಯಕ್ತಿ.flags, Mat(), Mat(), Mat(),
                    ಅಭಿವ್ಯಕ್ತಿ.alpha, ಅಭಿವ್ಯಕ್ತಿ.beta, ಅಭಿವ್ಯಕ್ತಿ.s);
        ಆದರೆ(ಅಭಿವ್ಯಕ್ತಿ.a.ದತ್ತಾಂಶ)
            e.a = ಅಭಿವ್ಯಕ್ತಿ.a.diag(d);
        ಆದರೆ(ಅಭಿವ್ಯಕ್ತಿ.b.ದತ್ತಾಂಶ)
            e.b = ಅಭಿವ್ಯಕ್ತಿ.b.diag(d);
        ಆದರೆ(ಅಭಿವ್ಯಕ್ತಿ.c.ದತ್ತಾಂಶ)
            e.c = ಅಭಿವ್ಯಕ್ತಿ.c.diag(d);
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        Mat m;
        ಅಭಿವ್ಯಕ್ತಿ.op->assign(ಅಭಿವ್ಯಕ್ತಿ, m);
        e = Mat_ಅಭಿವ್ಯಕ್ತಿ(&g_Mat_Op_ತಟಸ್ಥ, 0, m.diag(d), Mat(), Mat());
    }
}


ಶೂನ್ಯ MatOp::aug_ನಿಯೋಜಿಸು_ಕೂಡಿಸು(ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& ಅಭಿವ್ಯಕ್ತಿ, Mat& m) ಸ್ಥಿರಾಂಕ
{
    Mat ತಾತ್ಕಾಲಿಕ_2;
    ಅಭಿವ್ಯಕ್ತಿ.op->assign(ಅಭಿವ್ಯಕ್ತಿ, ತಾತ್ಕಾಲಿಕ_2);
    m += ತಾತ್ಕಾಲಿಕ_2;
}


ಶೂನ್ಯ MatOp::aug_ನಿಯೋಜಿಸು_ಕಳೆ(ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& ಅಭಿವ್ಯಕ್ತಿ, Mat& m) ಸ್ಥಿರಾಂಕ
{
    Mat ತಾತ್ಕಾಲಿಕ_2;
    ಅಭಿವ್ಯಕ್ತಿ.op->assign(ಅಭಿವ್ಯಕ್ತಿ, ತಾತ್ಕಾಲಿಕ_2);
    m -= ತಾತ್ಕಾಲಿಕ_2;
}


ಶೂನ್ಯ MatOp::aug_ನಿಯೋಜಿಸು_Multiply(ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& ಅಭಿವ್ಯಕ್ತಿ, Mat& m) ಸ್ಥಿರಾಂಕ
{
    Mat ತಾತ್ಕಾಲಿಕ_2;
    ಅಭಿವ್ಯಕ್ತಿ.op->assign(ಅಭಿವ್ಯಕ್ತಿ, ತಾತ್ಕಾಲಿಕ_2);
    m *= ತಾತ್ಕಾಲಿಕ_2;
}


ಶೂನ್ಯ MatOp::aug_ನಿಯೋಜಿಸು_Divide(ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& ಅಭಿವ್ಯಕ್ತಿ, Mat& m) ಸ್ಥಿರಾಂಕ
{
    Mat ತಾತ್ಕಾಲಿಕ_2;
    ಅಭಿವ್ಯಕ್ತಿ.op->assign(ಅಭಿವ್ಯಕ್ತಿ, ತಾತ್ಕಾಲಿಕ_2);
    m /= ತಾತ್ಕಾಲಿಕ_2;
}


ಶೂನ್ಯ MatOp::aug_ನಿಯೋಜಿಸು_And(ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& ಅಭಿವ್ಯಕ್ತಿ, Mat& m) ಸ್ಥಿರಾಂಕ
{
    Mat ತಾತ್ಕಾಲಿಕ_2;
    ಅಭಿವ್ಯಕ್ತಿ.op->assign(ಅಭಿವ್ಯಕ್ತಿ, ತಾತ್ಕಾಲಿಕ_2);
    m &= ತಾತ್ಕಾಲಿಕ_2;
}


ಶೂನ್ಯ MatOp::aug_ನಿಯೋಜಿಸು_Or(ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& ಅಭಿವ್ಯಕ್ತಿ, Mat& m) ಸ್ಥಿರಾಂಕ
{
    Mat ತಾತ್ಕಾಲಿಕ_2;
    ಅಭಿವ್ಯಕ್ತಿ.op->assign(ಅಭಿವ್ಯಕ್ತಿ, ತಾತ್ಕಾಲಿಕ_2);
    m |= ತಾತ್ಕಾಲಿಕ_2;
}


ಶೂನ್ಯ MatOp::aug_ನಿಯೋಜಿಸು_Xor(ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& ಅಭಿವ್ಯಕ್ತಿ, Mat& m) ಸ್ಥಿರಾಂಕ
{
    Mat ತಾತ್ಕಾಲಿಕ_2;
    ಅಭಿವ್ಯಕ್ತಿ.op->assign(ಅಭಿವ್ಯಕ್ತಿ, ತಾತ್ಕಾಲಿಕ_2);
    m ^= ತಾತ್ಕಾಲಿಕ_2;
}


ಶೂನ್ಯ MatOp::ಕೂಡಿಸು(ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& e1, ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& e2, Mat_ಅಭಿವ್ಯಕ್ತಿ& res) ಸ್ಥಿರಾಂಕ
{
    CV_INSTRUMENT_REGION();

    ಆದರೆ( ಇದು == e2.op )
    {
        ದ್ವಿಗುಣ alpha = 1, beta = 1;
        Scalar s;
        Mat m1, m2;
        ಆದರೆ( is_ಕೂಡಿಸು_Ex(e1) && (!e1.b.ದತ್ತಾಂಶ || e1.beta == 0) )
        {
            m1 = e1.a;
            alpha = e1.alpha;
            s = e1.s;
        }
        ಇಲ್ಲದಿದ್ದರೆ
            e1.op->assign(e1, m1);

        ಆದರೆ( is_ಕೂಡಿಸು_Ex(e2) && (!e2.b.ದತ್ತಾಂಶ || e2.beta == 0) )
        {
            m2 = e2.a;
            beta = e2.alpha;
            s += e2.s;
        }
        ಇಲ್ಲದಿದ್ದರೆ
            e2.op->assign(e2, m2);
        Mat_Op_ಕೂಡಿಸು_Ex::make_ಅಭಿವ್ಯಕ್ತಿ(res, m1, m2, alpha, beta, s);
    }
    ಇಲ್ಲದಿದ್ದರೆ
        e2.op->ಕೂಡಿಸು(e1, e2, res);
}


ಶೂನ್ಯ MatOp::ಕೂಡಿಸು(ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& expr1, ಸ್ಥಿರಾಂಕ Scalar& s, Mat_ಅಭಿವ್ಯಕ್ತಿ& res) ಸ್ಥಿರಾಂಕ
{
    CV_INSTRUMENT_REGION();

    Mat m1;
    expr1.op->assign(expr1, m1);
    Mat_Op_ಕೂಡಿಸು_Ex::make_ಅಭಿವ್ಯಕ್ತಿ(res, m1, Mat(), 1, 0, s);
}


ಶೂನ್ಯ MatOp::ಕಳೆ(ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& e1, ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& e2, Mat_ಅಭಿವ್ಯಕ್ತಿ& res) ಸ್ಥಿರಾಂಕ
{
    CV_INSTRUMENT_REGION();

    ಆದರೆ( ಇದು == e2.op )
    {
        ದ್ವಿಗುಣ alpha = 1, beta = -1;
        Scalar s;
        Mat m1, m2;
        ಆದರೆ( is_ಕೂಡಿಸು_Ex(e1) && (!e1.b.ದತ್ತಾಂಶ || e1.beta == 0) )
        {
            m1 = e1.a;
            alpha = e1.alpha;
            s = e1.s;
        }
        ಇಲ್ಲದಿದ್ದರೆ
            e1.op->assign(e1, m1);

        ಆದರೆ( is_ಕೂಡಿಸು_Ex(e2) && (!e2.b.ದತ್ತಾಂಶ || e2.beta == 0) )
        {
            m2 = e2.a;
            beta = -e2.alpha;
            s -= e2.s;
        }
        ಇಲ್ಲದಿದ್ದರೆ
            e2.op->assign(e2, m2);
        Mat_Op_ಕೂಡಿಸು_Ex::make_ಅಭಿವ್ಯಕ್ತಿ(res, m1, m2, alpha, beta, s);
    }
    ಇಲ್ಲದಿದ್ದರೆ
        e2.op->ಕಳೆ(e1, e2, res);
}


ಶೂನ್ಯ MatOp::ಕಳೆ(ಸ್ಥಿರಾಂಕ Scalar& s, ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& ಅಭಿವ್ಯಕ್ತಿ, Mat_ಅಭಿವ್ಯಕ್ತಿ& res) ಸ್ಥಿರಾಂಕ
{
    CV_INSTRUMENT_REGION();

    Mat m;
    ಅಭಿವ್ಯಕ್ತಿ.op->assign(ಅಭಿವ್ಯಕ್ತಿ, m);
    Mat_Op_ಕೂಡಿಸು_Ex::make_ಅಭಿವ್ಯಕ್ತಿ(res, m, Mat(), -1, 0, s);
}


ಶೂನ್ಯ MatOp::multiply(ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& e1, ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& e2, Mat_ಅಭಿವ್ಯಕ್ತಿ& res, ದ್ವಿಗುಣ scale) ಸ್ಥಿರಾಂಕ
{
    CV_INSTRUMENT_REGION();

    ಆದರೆ( ಇದು == e2.op )
    {
        Mat m1, m2;

        ಆದರೆ( isReciprocal(e1) )
        {
            ಆದರೆ( isScaled(e2) )
            {
                scale *= e2.alpha;
                m2 = e2.a;
            }
            ಇಲ್ಲದಿದ್ದರೆ
                e2.op->assign(e2, m2);

            MatOp_Bin::make_ಅಭಿವ್ಯಕ್ತಿ(res, '/', m2, e1.a, scale/e1.alpha);
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            ಅಕ್ಷರ op = '*';
            ಆದರೆ( isScaled(e1) )
            {
                m1 = e1.a;
                scale *= e1.alpha;
            }
            ಇಲ್ಲದಿದ್ದರೆ
                e1.op->assign(e1, m1);

            ಆದರೆ( isScaled(e2) )
            {
                m2 = e2.a;
                scale *= e2.alpha;
            }
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( isReciprocal(e2) )
            {
                op = '/';
                m2 = e2.a;
                scale *= e2.alpha;
            }
            ಇಲ್ಲದಿದ್ದರೆ
                e2.op->assign(e2, m2);

            MatOp_Bin::make_ಅಭಿವ್ಯಕ್ತಿ(res, op, m1, m2, scale);
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ
        e2.op->multiply(e1, e2, res, scale);
}


ಶೂನ್ಯ MatOp::multiply(ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& ಅಭಿವ್ಯಕ್ತಿ, ದ್ವಿಗುಣ s, Mat_ಅಭಿವ್ಯಕ್ತಿ& res) ಸ್ಥಿರಾಂಕ
{
    CV_INSTRUMENT_REGION();

    Mat m;
    ಅಭಿವ್ಯಕ್ತಿ.op->assign(ಅಭಿವ್ಯಕ್ತಿ, m);
    Mat_Op_ಕೂಡಿಸು_Ex::make_ಅಭಿವ್ಯಕ್ತಿ(res, m, Mat(), s, 0);
}


ಶೂನ್ಯ MatOp::divide(ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& e1, ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& e2, Mat_ಅಭಿವ್ಯಕ್ತಿ& res, ದ್ವಿಗುಣ scale) ಸ್ಥಿರಾಂಕ
{
    CV_INSTRUMENT_REGION();

    ಆದರೆ( ಇದು == e2.op )
    {
        ಆದರೆ( isReciprocal(e1) && isReciprocal(e2) )
            MatOp_Bin::make_ಅಭಿವ್ಯಕ್ತಿ(res, '/', e2.a, e1.a, e1.alpha/e2.alpha);
        ಇಲ್ಲದಿದ್ದರೆ
        {
            Mat m1, m2;
            ಅಕ್ಷರ op = '/';

            ಆದರೆ( isScaled(e1) )
            {
                m1 = e1.a;
                scale *= e1.alpha;
            }
            ಇಲ್ಲದಿದ್ದರೆ
                e1.op->assign(e1, m1);

            ಆದರೆ( isScaled(e2) )
            {
                m2 = e2.a;
                scale /= e2.alpha;
            }
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( isReciprocal(e2) )
            {
                m2 = e2.a;
                scale /= e2.alpha;
                op = '*';
            }
            ಇಲ್ಲದಿದ್ದರೆ
                e2.op->assign(e2, m2);
            MatOp_Bin::make_ಅಭಿವ್ಯಕ್ತಿ(res, op, m1, m2, scale);
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ
        e2.op->divide(e1, e2, res, scale);
}


ಶೂನ್ಯ MatOp::divide(ದ್ವಿಗುಣ s, ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& ಅಭಿವ್ಯಕ್ತಿ, Mat_ಅಭಿವ್ಯಕ್ತಿ& res) ಸ್ಥಿರಾಂಕ
{
    CV_INSTRUMENT_REGION();

    Mat m;
    ಅಭಿವ್ಯಕ್ತಿ.op->assign(ಅಭಿವ್ಯಕ್ತಿ, m);
    MatOp_Bin::make_ಅಭಿವ್ಯಕ್ತಿ(res, '/', m, Mat(), s);
}


ಶೂನ್ಯ MatOp::ನಿರಪೇಕ್ಷ(ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& ಅಭಿವ್ಯಕ್ತಿ, Mat_ಅಭಿವ್ಯಕ್ತಿ& res) ಸ್ಥಿರಾಂಕ
{
    CV_INSTRUMENT_REGION();

    Mat m;
    ಅಭಿವ್ಯಕ್ತಿ.op->assign(ಅಭಿವ್ಯಕ್ತಿ, m);
    MatOp_Bin::make_ಅಭಿವ್ಯಕ್ತಿ(res, 'a', m, Mat());
}


ಶೂನ್ಯ MatOp::transpose(ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& ಅಭಿವ್ಯಕ್ತಿ, Mat_ಅಭಿವ್ಯಕ್ತಿ& res) ಸ್ಥಿರಾಂಕ
{
    CV_INSTRUMENT_REGION();

    Mat m;
    ಅಭಿವ್ಯಕ್ತಿ.op->assign(ಅಭಿವ್ಯಕ್ತಿ, m);
    MatOp_T::make_ಅಭಿವ್ಯಕ್ತಿ(res, m, 1);
}


ಶೂನ್ಯ MatOp::matmul(ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& e1, ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& e2, Mat_ಅಭಿವ್ಯಕ್ತಿ& res) ಸ್ಥಿರಾಂಕ
{
    ಆದರೆ( ಇದು == e2.op )
    {
        ದ್ವಿಗುಣ scale = 1;
        ಪೂರ್ಣಾಂಕ flags = 0;
        Mat m1, m2;

        ಆದರೆ( isT(e1) )
        {
            flags = cv::GEMM_1_T;
            scale = e1.alpha;
            m1 = e1.a;
        }
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( isScaled(e1) )
        {
            scale = e1.alpha;
            m1 = e1.a;
        }
        ಇಲ್ಲದಿದ್ದರೆ
            e1.op->assign(e1, m1);

        ಆದರೆ( isT(e2) )
        {
            flags |= cv::GEMM_2_T;
            scale *= e2.alpha;
            m2 = e2.a;
        }
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( isScaled(e2) )
        {
            scale *= e2.alpha;
            m2 = e2.a;
        }
        ಇಲ್ಲದಿದ್ದರೆ
            e2.op->assign(e2, m2);

        MatOp_GEMM::make_ಅಭಿವ್ಯಕ್ತಿ(res, flags, m1, m2, scale);
    }
    ಇಲ್ಲದಿದ್ದರೆ
        e2.op->matmul(e1, e2, res);
}


ಶೂನ್ಯ MatOp::invert(ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& ಅಭಿವ್ಯಕ್ತಿ, ಪೂರ್ಣಾಂಕ ವಿಧಾನ, Mat_ಅಭಿವ್ಯಕ್ತಿ& res) ಸ್ಥಿರಾಂಕ
{
    Mat m;
    ಅಭಿವ್ಯಕ್ತಿ.op->assign(ಅಭಿವ್ಯಕ್ತಿ, m);
    MatOp_Invert::make_ಅಭಿವ್ಯಕ್ತಿ(res, ವಿಧಾನ, m);
}


ಗಾತ್ರ_2 MatOp::size(ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& ಅಭಿವ್ಯಕ್ತಿ) ಸ್ಥಿರಾಂಕ
{
    ಹಿಂತಿರುಗಿ !ಅಭಿವ್ಯಕ್ತಿ.a.empty() ? ಅಭಿವ್ಯಕ್ತಿ.a.size() : ಅಭಿವ್ಯಕ್ತಿ.b.empty() ? ಅಭಿವ್ಯಕ್ತಿ.b.size() : ಅಭಿವ್ಯಕ್ತಿ.c.size();
}

ಪೂರ್ಣಾಂಕ MatOp::type(ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& ಅಭಿವ್ಯಕ್ತಿ) ಸ್ಥಿರಾಂಕ
{
    CV_INSTRUMENT_REGION();

    ಹಿಂತಿರುಗಿ !ಅಭಿವ್ಯಕ್ತಿ.a.empty() ? ಅಭಿವ್ಯಕ್ತಿ.a.type() : ಅಭಿವ್ಯಕ್ತಿ.b.empty() ? ಅಭಿವ್ಯಕ್ತಿ.b.type() : ಅಭಿವ್ಯಕ್ತಿ.c.type();
}

//////////////////////////////////////////////////////////////////////////////////////////////////

Mat_ಅಭಿವ್ಯಕ್ತಿ::Mat_ಅಭಿವ್ಯಕ್ತಿ(ಸ್ಥಿರಾಂಕ Mat& m) : op(&g_Mat_Op_ತಟಸ್ಥ), flags(0), a(m), b(Mat()), c(Mat()), alpha(1), beta(0), s(Scalar())
{
}

Mat_ಅಭಿವ್ಯಕ್ತಿ Mat_ಅಭಿವ್ಯಕ್ತಿ::ಅಡ್ಡಸಾಲು(ಪೂರ್ಣಾಂಕ y) ಸ್ಥಿರಾಂಕ
{
    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    op->roi(*ಇದು, Range(y, y+1), Range::all(), e);
    ಹಿಂತಿರುಗಿ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ Mat_ಅಭಿವ್ಯಕ್ತಿ::ಲಂಬಸಾಲು(ಪೂರ್ಣಾಂಕ x) ಸ್ಥಿರಾಂಕ
{
    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    op->roi(*ಇದು, Range::all(), Range(x, x+1), e);
    ಹಿಂತಿರುಗಿ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ Mat_ಅಭಿವ್ಯಕ್ತಿ::diag(ಪೂರ್ಣಾಂಕ d) ಸ್ಥಿರಾಂಕ
{
    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    op->diag(*ಇದು, d, e);
    ಹಿಂತಿರುಗಿ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ Mat_ಅಭಿವ್ಯಕ್ತಿ::ಕ್ರಿಯಾಕಾರಕ()( ಸ್ಥಿರಾಂಕ Range& ಅಡ್ಡಸಾಲು_Range_2, ಸ್ಥಿರಾಂಕ Range& ಲಂಬಸಾಲು_Range_2 ) ಸ್ಥಿರಾಂಕ
{
    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    op->roi(*ಇದು, ಅಡ್ಡಸಾಲು_Range_2, ಲಂಬಸಾಲು_Range_2, e);
    ಹಿಂತಿರುಗಿ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ Mat_ಅಭಿವ್ಯಕ್ತಿ::ಕ್ರಿಯಾಕಾರಕ()( ಸ್ಥಿರಾಂಕ Rect& roi ) ಸ್ಥಿರಾಂಕ
{
    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    op->roi(*ಇದು, Range(roi.y, roi.y + roi.ಎತ್ತರ), Range(roi.x, roi.x + roi.ಅಗಲ), e);
    ಹಿಂತಿರುಗಿ e;
}

Mat Mat_ಅಭಿವ್ಯಕ್ತಿ::cross(ಸ್ಥಿರಾಂಕ Mat& m) ಸ್ಥಿರಾಂಕ
{
    ಹಿಂತಿರುಗಿ ((Mat)*ಇದು).cross(m);
}

ದ್ವಿಗುಣ Mat_ಅಭಿವ್ಯಕ್ತಿ::dot(ಸ್ಥಿರಾಂಕ Mat& m) ಸ್ಥಿರಾಂಕ
{
    ಹಿಂತಿರುಗಿ ((Mat)*ಇದು).dot(m);
}

Mat_ಅಭಿವ್ಯಕ್ತಿ Mat_ಅಭಿವ್ಯಕ್ತಿ::t() ಸ್ಥಿರಾಂಕ
{
    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    op->transpose(*ಇದು, e);
    ಹಿಂತಿರುಗಿ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ Mat_ಅಭಿವ್ಯಕ್ತಿ::inv(ಪೂರ್ಣಾಂಕ ವಿಧಾನ) ಸ್ಥಿರಾಂಕ
{
    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    op->invert(*ಇದು, ವಿಧಾನ, e);
    ಹಿಂತಿರುಗಿ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ Mat_ಅಭಿವ್ಯಕ್ತಿ::mul(ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& e, ದ್ವಿಗುಣ scale) ಸ್ಥಿರಾಂಕ
{
    Mat_ಅಭಿವ್ಯಕ್ತಿ en;
    op->multiply(*ಇದು, e, en, scale);
    ಹಿಂತಿರುಗಿ en;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ Mat_ಅಭಿವ್ಯಕ್ತಿ::mul(ಸ್ಥಿರಾಂಕ Mat& m, ದ್ವಿಗುಣ scale) ಸ್ಥಿರಾಂಕ
{
    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    op->multiply(*ಇದು, Mat_ಅಭಿವ್ಯಕ್ತಿ(m), e, scale);
    ಹಿಂತಿರುಗಿ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ ಕ್ರಿಯಾಕಾರಕ + (ಸ್ಥಿರಾಂಕ Mat& a, ಸ್ಥಿರಾಂಕ Mat& b)
{
    checkOperandsExist(a, b);
    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    Mat_Op_ಕೂಡಿಸು_Ex::make_ಅಭಿವ್ಯಕ್ತಿ(e, a, b, 1, 1);
    ಹಿಂತಿರುಗಿ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ ಕ್ರಿಯಾಕಾರಕ + (ಸ್ಥಿರಾಂಕ Mat& a, ಸ್ಥಿರಾಂಕ Scalar& s)
{
    checkOperandsExist(a);
    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    Mat_Op_ಕೂಡಿಸು_Ex::make_ಅಭಿವ್ಯಕ್ತಿ(e, a, Mat(), 1, 0, s);
    ಹಿಂತಿರುಗಿ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ ಕ್ರಿಯಾಕಾರಕ + (ಸ್ಥಿರಾಂಕ Scalar& s, ಸ್ಥಿರಾಂಕ Mat& a)
{
    checkOperandsExist(a);
    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    Mat_Op_ಕೂಡಿಸು_Ex::make_ಅಭಿವ್ಯಕ್ತಿ(e, a, Mat(), 1, 0, s);
    ಹಿಂತಿರುಗಿ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ ಕ್ರಿಯಾಕಾರಕ + (ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& e, ಸ್ಥಿರಾಂಕ Mat& m)
{
    Mat_ಅಭಿವ್ಯಕ್ತಿ en;
    e.op->ಕೂಡಿಸು(e, Mat_ಅಭಿವ್ಯಕ್ತಿ(m), en);
    ಹಿಂತಿರುಗಿ en;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ ಕ್ರಿಯಾಕಾರಕ + (ಸ್ಥಿರಾಂಕ Mat& m, ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& e)
{
    checkOperandsExist(m);
    Mat_ಅಭಿವ್ಯಕ್ತಿ en;
    e.op->ಕೂಡಿಸು(e, Mat_ಅಭಿವ್ಯಕ್ತಿ(m), en);
    ಹಿಂತಿರುಗಿ en;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ ಕ್ರಿಯಾಕಾರಕ + (ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& e, ಸ್ಥಿರಾಂಕ Scalar& s)
{
    Mat_ಅಭಿವ್ಯಕ್ತಿ en;
    e.op->ಕೂಡಿಸು(e, s, en);
    ಹಿಂತಿರುಗಿ en;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ ಕ್ರಿಯಾಕಾರಕ + (ಸ್ಥಿರಾಂಕ Scalar& s, ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& e)
{
    Mat_ಅಭಿವ್ಯಕ್ತಿ en;
    e.op->ಕೂಡಿಸು(e, s, en);
    ಹಿಂತಿರುಗಿ en;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ ಕ್ರಿಯಾಕಾರಕ + (ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& e1, ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& e2)
{
    Mat_ಅಭಿವ್ಯಕ್ತಿ en;
    e1.op->ಕೂಡಿಸು(e1, e2, en);
    ಹಿಂತಿರುಗಿ en;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ ಕ್ರಿಯಾಕಾರಕ - (ಸ್ಥಿರಾಂಕ Mat& a, ಸ್ಥಿರಾಂಕ Mat& b)
{
    checkOperandsExist(a, b);
    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    Mat_Op_ಕೂಡಿಸು_Ex::make_ಅಭಿವ್ಯಕ್ತಿ(e, a, b, 1, -1);
    ಹಿಂತಿರುಗಿ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ ಕ್ರಿಯಾಕಾರಕ - (ಸ್ಥಿರಾಂಕ Mat& a, ಸ್ಥಿರಾಂಕ Scalar& s)
{
    checkOperandsExist(a);
    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    Mat_Op_ಕೂಡಿಸು_Ex::make_ಅಭಿವ್ಯಕ್ತಿ(e, a, Mat(), 1, 0, -s);
    ಹಿಂತಿರುಗಿ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ ಕ್ರಿಯಾಕಾರಕ - (ಸ್ಥಿರಾಂಕ Scalar& s, ಸ್ಥಿರಾಂಕ Mat& a)
{
    checkOperandsExist(a);
    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    Mat_Op_ಕೂಡಿಸು_Ex::make_ಅಭಿವ್ಯಕ್ತಿ(e, a, Mat(), -1, 0, s);
    ಹಿಂತಿರುಗಿ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ ಕ್ರಿಯಾಕಾರಕ - (ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& e, ಸ್ಥಿರಾಂಕ Mat& m)
{
    checkOperandsExist(m);
    Mat_ಅಭಿವ್ಯಕ್ತಿ en;
    e.op->ಕಳೆ(e, Mat_ಅಭಿವ್ಯಕ್ತಿ(m), en);
    ಹಿಂತಿರುಗಿ en;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ ಕ್ರಿಯಾಕಾರಕ - (ಸ್ಥಿರಾಂಕ Mat& m, ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& e)
{
    checkOperandsExist(m);
    Mat_ಅಭಿವ್ಯಕ್ತಿ en;
    e.op->ಕಳೆ(Mat_ಅಭಿವ್ಯಕ್ತಿ(m), e, en);
    ಹಿಂತಿರುಗಿ en;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ ಕ್ರಿಯಾಕಾರಕ - (ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& e, ಸ್ಥಿರಾಂಕ Scalar& s)
{
    Mat_ಅಭಿವ್ಯಕ್ತಿ en;
    e.op->ಕೂಡಿಸು(e, -s, en);
    ಹಿಂತಿರುಗಿ en;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ ಕ್ರಿಯಾಕಾರಕ - (ಸ್ಥಿರಾಂಕ Scalar& s, ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& e)
{
    Mat_ಅಭಿವ್ಯಕ್ತಿ en;
    e.op->ಕಳೆ(s, e, en);
    ಹಿಂತಿರುಗಿ en;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ ಕ್ರಿಯಾಕಾರಕ - (ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& e1, ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& e2)
{
    Mat_ಅಭಿವ್ಯಕ್ತಿ en;
    e1.op->ಕಳೆ(e1, e2, en);
    ಹಿಂತಿರುಗಿ en;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ ಕ್ರಿಯಾಕಾರಕ - (ಸ್ಥಿರಾಂಕ Mat& m)
{
    checkOperandsExist(m);
    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    Mat_Op_ಕೂಡಿಸು_Ex::make_ಅಭಿವ್ಯಕ್ತಿ(e, m, Mat(), -1, 0);
    ಹಿಂತಿರುಗಿ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ ಕ್ರಿಯಾಕಾರಕ - (ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& e)
{
    Mat_ಅಭಿವ್ಯಕ್ತಿ en;
    e.op->ಕಳೆ(Scalar(0), e, en);
    ಹಿಂತಿರುಗಿ en;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ ಕ್ರಿಯಾಕಾರಕ * (ಸ್ಥಿರಾಂಕ Mat& a, ಸ್ಥಿರಾಂಕ Mat& b)
{
    checkOperandsExist(a, b);
    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    MatOp_GEMM::make_ಅಭಿವ್ಯಕ್ತಿ(e, 0, a, b);
    ಹಿಂತಿರುಗಿ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ ಕ್ರಿಯಾಕಾರಕ * (ಸ್ಥಿರಾಂಕ Mat& a, ದ್ವಿಗುಣ s)
{
    checkOperandsExist(a);
    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    Mat_Op_ಕೂಡಿಸು_Ex::make_ಅಭಿವ್ಯಕ್ತಿ(e, a, Mat(), s, 0);
    ಹಿಂತಿರುಗಿ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ ಕ್ರಿಯಾಕಾರಕ * (ದ್ವಿಗುಣ s, ಸ್ಥಿರಾಂಕ Mat& a)
{
    checkOperandsExist(a);
    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    Mat_Op_ಕೂಡಿಸು_Ex::make_ಅಭಿವ್ಯಕ್ತಿ(e, a, Mat(), s, 0);
    ಹಿಂತಿರುಗಿ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ ಕ್ರಿಯಾಕಾರಕ * (ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& e, ಸ್ಥಿರಾಂಕ Mat& m)
{
    Mat_ಅಭಿವ್ಯಕ್ತಿ en;
    e.op->matmul(e, Mat_ಅಭಿವ್ಯಕ್ತಿ(m), en);
    ಹಿಂತಿರುಗಿ en;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ ಕ್ರಿಯಾಕಾರಕ * (ಸ್ಥಿರಾಂಕ Mat& m, ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& e)
{
    checkOperandsExist(m);
    Mat_ಅಭಿವ್ಯಕ್ತಿ en;
    e.op->matmul(Mat_ಅಭಿವ್ಯಕ್ತಿ(m), e, en);
    ಹಿಂತಿರುಗಿ en;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ ಕ್ರಿಯಾಕಾರಕ * (ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& e, ದ್ವಿಗುಣ s)
{
    Mat_ಅಭಿವ್ಯಕ್ತಿ en;
    e.op->multiply(e, s, en);
    ಹಿಂತಿರುಗಿ en;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ ಕ್ರಿಯಾಕಾರಕ * (ದ್ವಿಗುಣ s, ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& e)
{
    Mat_ಅಭಿವ್ಯಕ್ತಿ en;
    e.op->multiply(e, s, en);
    ಹಿಂತಿರುಗಿ en;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ ಕ್ರಿಯಾಕಾರಕ * (ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& e1, ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& e2)
{
    Mat_ಅಭಿವ್ಯಕ್ತಿ en;
    e1.op->matmul(e1, e2, en);
    ಹಿಂತಿರುಗಿ en;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ ಕ್ರಿಯಾಕಾರಕ / (ಸ್ಥಿರಾಂಕ Mat& a, ಸ್ಥಿರಾಂಕ Mat& b)
{
    checkOperandsExist(a, b);
    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    MatOp_Bin::make_ಅಭಿವ್ಯಕ್ತಿ(e, '/', a, b);
    ಹಿಂತಿರುಗಿ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ ಕ್ರಿಯಾಕಾರಕ / (ಸ್ಥಿರಾಂಕ Mat& a, ದ್ವಿಗುಣ s)
{
    checkOperandsExist(a);
    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    Mat_Op_ಕೂಡಿಸು_Ex::make_ಅಭಿವ್ಯಕ್ತಿ(e, a, Mat(), 1./s, 0);
    ಹಿಂತಿರುಗಿ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ ಕ್ರಿಯಾಕಾರಕ / (ದ್ವಿಗುಣ s, ಸ್ಥಿರಾಂಕ Mat& a)
{
    checkOperandsExist(a);
    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    MatOp_Bin::make_ಅಭಿವ್ಯಕ್ತಿ(e, '/', a, Mat(), s);
    ಹಿಂತಿರುಗಿ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ ಕ್ರಿಯಾಕಾರಕ / (ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& e, ಸ್ಥಿರಾಂಕ Mat& m)
{
    Mat_ಅಭಿವ್ಯಕ್ತಿ en;
    e.op->divide(e, Mat_ಅಭಿವ್ಯಕ್ತಿ(m), en);
    ಹಿಂತಿರುಗಿ en;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ ಕ್ರಿಯಾಕಾರಕ / (ಸ್ಥಿರಾಂಕ Mat& m, ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& e)
{
    checkOperandsExist(m);
    Mat_ಅಭಿವ್ಯಕ್ತಿ en;
    e.op->divide(Mat_ಅಭಿವ್ಯಕ್ತಿ(m), e, en);
    ಹಿಂತಿರುಗಿ en;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ ಕ್ರಿಯಾಕಾರಕ / (ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& e, ದ್ವಿಗುಣ s)
{
    Mat_ಅಭಿವ್ಯಕ್ತಿ en;
    e.op->multiply(e, 1./s, en);
    ಹಿಂತಿರುಗಿ en;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ ಕ್ರಿಯಾಕಾರಕ / (ದ್ವಿಗುಣ s, ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& e)
{
    Mat_ಅಭಿವ್ಯಕ್ತಿ en;
    e.op->divide(s, e, en);
    ಹಿಂತಿರುಗಿ en;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ ಕ್ರಿಯಾಕಾರಕ / (ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& e1, ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& e2)
{
    Mat_ಅಭಿವ್ಯಕ್ತಿ en;
    e1.op->divide(e1, e2, en);
    ಹಿಂತಿರುಗಿ en;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ ಕ್ರಿಯಾಕಾರಕ < (ಸ್ಥಿರಾಂಕ Mat& a, ಸ್ಥಿರಾಂಕ Mat& b)
{
    checkOperandsExist(a, b);
    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    MatOp_Cmp::make_ಅಭಿವ್ಯಕ್ತಿ(e, cv::CMP_LT, a, b);
    ಹಿಂತಿರುಗಿ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ ಕ್ರಿಯಾಕಾರಕ < (ಸ್ಥಿರಾಂಕ Mat& a, ದ್ವಿಗುಣ s)
{
    checkOperandsExist(a);
    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    MatOp_Cmp::make_ಅಭಿವ್ಯಕ್ತಿ(e, cv::CMP_LT, a, s);
    ಹಿಂತಿರುಗಿ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ ಕ್ರಿಯಾಕಾರಕ < (ದ್ವಿಗುಣ s, ಸ್ಥಿರಾಂಕ Mat& a)
{
    checkOperandsExist(a);
    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    MatOp_Cmp::make_ಅಭಿವ್ಯಕ್ತಿ(e, cv::CMP_GT, a, s);
    ಹಿಂತಿರುಗಿ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ ಕ್ರಿಯಾಕಾರಕ <= (ಸ್ಥಿರಾಂಕ Mat& a, ಸ್ಥಿರಾಂಕ Mat& b)
{
    checkOperandsExist(a, b);
    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    MatOp_Cmp::make_ಅಭಿವ್ಯಕ್ತಿ(e, cv::CMP_LE, a, b);
    ಹಿಂತಿರುಗಿ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ ಕ್ರಿಯಾಕಾರಕ <= (ಸ್ಥಿರಾಂಕ Mat& a, ದ್ವಿಗುಣ s)
{
    checkOperandsExist(a);
    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    MatOp_Cmp::make_ಅಭಿವ್ಯಕ್ತಿ(e, cv::CMP_LE, a, s);
    ಹಿಂತಿರುಗಿ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ ಕ್ರಿಯಾಕಾರಕ <= (ದ್ವಿಗುಣ s, ಸ್ಥಿರಾಂಕ Mat& a)
{
    checkOperandsExist(a);
    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    MatOp_Cmp::make_ಅಭಿವ್ಯಕ್ತಿ(e, cv::CMP_GE, a, s);
    ಹಿಂತಿರುಗಿ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ ಕ್ರಿಯಾಕಾರಕ == (ಸ್ಥಿರಾಂಕ Mat& a, ಸ್ಥಿರಾಂಕ Mat& b)
{
    checkOperandsExist(a, b);
    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    MatOp_Cmp::make_ಅಭಿವ್ಯಕ್ತಿ(e, cv::CMP_EQ, a, b);
    ಹಿಂತಿರುಗಿ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ ಕ್ರಿಯಾಕಾರಕ == (ಸ್ಥಿರಾಂಕ Mat& a, ದ್ವಿಗುಣ s)
{
    checkOperandsExist(a);
    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    MatOp_Cmp::make_ಅಭಿವ್ಯಕ್ತಿ(e, cv::CMP_EQ, a, s);
    ಹಿಂತಿರುಗಿ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ ಕ್ರಿಯಾಕಾರಕ == (ದ್ವಿಗುಣ s, ಸ್ಥಿರಾಂಕ Mat& a)
{
    checkOperandsExist(a);
    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    MatOp_Cmp::make_ಅಭಿವ್ಯಕ್ತಿ(e, cv::CMP_EQ, a, s);
    ಹಿಂತಿರುಗಿ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ ಕ್ರಿಯಾಕಾರಕ != (ಸ್ಥಿರಾಂಕ Mat& a, ಸ್ಥಿರಾಂಕ Mat& b)
{
    checkOperandsExist(a, b);
    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    MatOp_Cmp::make_ಅಭಿವ್ಯಕ್ತಿ(e, cv::CMP_NE, a, b);
    ಹಿಂತಿರುಗಿ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ ಕ್ರಿಯಾಕಾರಕ != (ಸ್ಥಿರಾಂಕ Mat& a, ದ್ವಿಗುಣ s)
{
    checkOperandsExist(a);
    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    MatOp_Cmp::make_ಅಭಿವ್ಯಕ್ತಿ(e, cv::CMP_NE, a, s);
    ಹಿಂತಿರುಗಿ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ ಕ್ರಿಯಾಕಾರಕ != (ದ್ವಿಗುಣ s, ಸ್ಥಿರಾಂಕ Mat& a)
{
    checkOperandsExist(a);
    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    MatOp_Cmp::make_ಅಭಿವ್ಯಕ್ತಿ(e, cv::CMP_NE, a, s);
    ಹಿಂತಿರುಗಿ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ ಕ್ರಿಯಾಕಾರಕ >= (ಸ್ಥಿರಾಂಕ Mat& a, ಸ್ಥಿರಾಂಕ Mat& b)
{
    checkOperandsExist(a, b);
    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    MatOp_Cmp::make_ಅಭಿವ್ಯಕ್ತಿ(e, cv::CMP_GE, a, b);
    ಹಿಂತಿರುಗಿ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ ಕ್ರಿಯಾಕಾರಕ >= (ಸ್ಥಿರಾಂಕ Mat& a, ದ್ವಿಗುಣ s)
{
    checkOperandsExist(a);
    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    MatOp_Cmp::make_ಅಭಿವ್ಯಕ್ತಿ(e, cv::CMP_GE, a, s);
    ಹಿಂತಿರುಗಿ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ ಕ್ರಿಯಾಕಾರಕ >= (ದ್ವಿಗುಣ s, ಸ್ಥಿರಾಂಕ Mat& a)
{
    checkOperandsExist(a);
    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    MatOp_Cmp::make_ಅಭಿವ್ಯಕ್ತಿ(e, cv::CMP_LE, a, s);
    ಹಿಂತಿರುಗಿ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ ಕ್ರಿಯಾಕಾರಕ > (ಸ್ಥಿರಾಂಕ Mat& a, ಸ್ಥಿರಾಂಕ Mat& b)
{
    checkOperandsExist(a, b);
    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    MatOp_Cmp::make_ಅಭಿವ್ಯಕ್ತಿ(e, cv::CMP_GT, a, b);
    ಹಿಂತಿರುಗಿ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ ಕ್ರಿಯಾಕಾರಕ > (ಸ್ಥಿರಾಂಕ Mat& a, ದ್ವಿಗುಣ s)
{
    checkOperandsExist(a);
    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    MatOp_Cmp::make_ಅಭಿವ್ಯಕ್ತಿ(e, cv::CMP_GT, a, s);
    ಹಿಂತಿರುಗಿ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ ಕ್ರಿಯಾಕಾರಕ > (ದ್ವಿಗುಣ s, ಸ್ಥಿರಾಂಕ Mat& a)
{
    checkOperandsExist(a);
    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    MatOp_Cmp::make_ಅಭಿವ್ಯಕ್ತಿ(e, cv::CMP_LT, a, s);
    ಹಿಂತಿರುಗಿ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ min(ಸ್ಥಿರಾಂಕ Mat& a, ಸ್ಥಿರಾಂಕ Mat& b)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a, b);
    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    MatOp_Bin::make_ಅಭಿವ್ಯಕ್ತಿ(e, 'm', a, b);
    ಹಿಂತಿರುಗಿ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ min(ಸ್ಥಿರಾಂಕ Mat& a, ದ್ವಿಗುಣ s)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    MatOp_Bin::make_ಅಭಿವ್ಯಕ್ತಿ(e, 'n', a, s);
    ಹಿಂತಿರುಗಿ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ min(ದ್ವಿಗುಣ s, ಸ್ಥಿರಾಂಕ Mat& a)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    MatOp_Bin::make_ಅಭಿವ್ಯಕ್ತಿ(e, 'n', a, s);
    ಹಿಂತಿರುಗಿ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ max(ಸ್ಥಿರಾಂಕ Mat& a, ಸ್ಥಿರಾಂಕ Mat& b)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a, b);
    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    MatOp_Bin::make_ಅಭಿವ್ಯಕ್ತಿ(e, 'M', a, b);
    ಹಿಂತಿರುಗಿ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ max(ಸ್ಥಿರಾಂಕ Mat& a, ದ್ವಿಗುಣ s)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    MatOp_Bin::make_ಅಭಿವ್ಯಕ್ತಿ(e, 'N', a, s);
    ಹಿಂತಿರುಗಿ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ max(ದ್ವಿಗುಣ s, ಸ್ಥಿರಾಂಕ Mat& a)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    MatOp_Bin::make_ಅಭಿವ್ಯಕ್ತಿ(e, 'N', a, s);
    ಹಿಂತಿರುಗಿ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ ಕ್ರಿಯಾಕಾರಕ & (ಸ್ಥಿರಾಂಕ Mat& a, ಸ್ಥಿರಾಂಕ Mat& b)
{
    checkOperandsExist(a, b);
    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    MatOp_Bin::make_ಅಭಿವ್ಯಕ್ತಿ(e, '&', a, b);
    ಹಿಂತಿರುಗಿ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ ಕ್ರಿಯಾಕಾರಕ & (ಸ್ಥಿರಾಂಕ Mat& a, ಸ್ಥಿರಾಂಕ Scalar& s)
{
    checkOperandsExist(a);
    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    MatOp_Bin::make_ಅಭಿವ್ಯಕ್ತಿ(e, '&', a, s);
    ಹಿಂತಿರುಗಿ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ ಕ್ರಿಯಾಕಾರಕ & (ಸ್ಥಿರಾಂಕ Scalar& s, ಸ್ಥಿರಾಂಕ Mat& a)
{
    checkOperandsExist(a);
    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    MatOp_Bin::make_ಅಭಿವ್ಯಕ್ತಿ(e, '&', a, s);
    ಹಿಂತಿರುಗಿ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ ಕ್ರಿಯಾಕಾರಕ | (ಸ್ಥಿರಾಂಕ Mat& a, ಸ್ಥಿರಾಂಕ Mat& b)
{
    checkOperandsExist(a, b);
    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    MatOp_Bin::make_ಅಭಿವ್ಯಕ್ತಿ(e, '|', a, b);
    ಹಿಂತಿರುಗಿ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ ಕ್ರಿಯಾಕಾರಕ | (ಸ್ಥಿರಾಂಕ Mat& a, ಸ್ಥಿರಾಂಕ Scalar& s)
{
    checkOperandsExist(a);
    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    MatOp_Bin::make_ಅಭಿವ್ಯಕ್ತಿ(e, '|', a, s);
    ಹಿಂತಿರುಗಿ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ ಕ್ರಿಯಾಕಾರಕ | (ಸ್ಥಿರಾಂಕ Scalar& s, ಸ್ಥಿರಾಂಕ Mat& a)
{
    checkOperandsExist(a);
    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    MatOp_Bin::make_ಅಭಿವ್ಯಕ್ತಿ(e, '|', a, s);
    ಹಿಂತಿರುಗಿ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ ಕ್ರಿಯಾಕಾರಕ ^ (ಸ್ಥಿರಾಂಕ Mat& a, ಸ್ಥಿರಾಂಕ Mat& b)
{
    checkOperandsExist(a, b);
    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    MatOp_Bin::make_ಅಭಿವ್ಯಕ್ತಿ(e, '^', a, b);
    ಹಿಂತಿರುಗಿ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ ಕ್ರಿಯಾಕಾರಕ ^ (ಸ್ಥಿರಾಂಕ Mat& a, ಸ್ಥಿರಾಂಕ Scalar& s)
{
    checkOperandsExist(a);
    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    MatOp_Bin::make_ಅಭಿವ್ಯಕ್ತಿ(e, '^', a, s);
    ಹಿಂತಿರುಗಿ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ ಕ್ರಿಯಾಕಾರಕ ^ (ಸ್ಥಿರಾಂಕ Scalar& s, ಸ್ಥಿರಾಂಕ Mat& a)
{
    checkOperandsExist(a);
    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    MatOp_Bin::make_ಅಭಿವ್ಯಕ್ತಿ(e, '^', a, s);
    ಹಿಂತಿರುಗಿ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ ಕ್ರಿಯಾಕಾರಕ ~(ಸ್ಥಿರಾಂಕ Mat& a)
{
    checkOperandsExist(a);
    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    MatOp_Bin::make_ಅಭಿವ್ಯಕ್ತಿ(e, '~', a, Scalar());
    ಹಿಂತಿರುಗಿ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ ನಿರಪೇಕ್ಷ(ಸ್ಥಿರಾಂಕ Mat& a)
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(a);
    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    MatOp_Bin::make_ಅಭಿವ್ಯಕ್ತಿ(e, 'a', a, Scalar());
    ಹಿಂತಿರುಗಿ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ ನಿರಪೇಕ್ಷ(ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& e)
{
    CV_INSTRUMENT_REGION();

    Mat_ಅಭಿವ್ಯಕ್ತಿ en;
    e.op->ನಿರಪೇಕ್ಷ(e, en);
    ಹಿಂತಿರುಗಿ en;
}


ಗಾತ್ರ_2 Mat_ಅಭಿವ್ಯಕ್ತಿ::size() ಸ್ಥಿರಾಂಕ
{
    ಆದರೆ( isT(*ಇದು) || isInv(*ಇದು) )
        ಹಿಂತಿರುಗಿ ಗಾತ್ರ_2(a.ಅಡ್ಡಸಾಲುಗಳು, a.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ);
    ಆದರೆ( isSolve(*ಇದು) )
        ಹಿಂತಿರುಗಿ ಗಾತ್ರ_2(b.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, a.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ);
    ಆದರೆ( isInitializer(*ಇದು) )
        ಹಿಂತಿರುಗಿ a.size();
    ಹಿಂತಿರುಗಿ op ? op->size(*ಇದು) : ಗಾತ್ರ_2();
}


ಪೂರ್ಣಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ::type() ಸ್ಥಿರಾಂಕ
{
    CV_INSTRUMENT_REGION();

    ಆದರೆ( isInitializer(*ಇದು) )
        ಹಿಂತಿರುಗಿ a.type();
    ಆದರೆ( isCmp(*ಇದು) )
        ಹಿಂತಿರುಗಿ CV_MAKETYPE(CV_8U, a.channels());
    ಹಿಂತಿರುಗಿ op ? op->type(*ಇದು) : -1;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////

ಶೂನ್ಯ Mat_Op_ತಟಸ್ಥ::assign(ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& e, Mat& m, ಪೂರ್ಣಾಂಕ _type) ಸ್ಥಿರಾಂಕ
{
    ಆದರೆ( _type == -1 || _type == e.a.type() )
        m = e.a;
    ಇಲ್ಲದಿದ್ದರೆ
    {
        CV_Assert( CV_MAT_CN(_type) == e.a.channels() );
        e.a.convert_ಗಮ್ಯ(m, _type);
    }
}

ಸಾಲೊಳಗೆ ಶೂನ್ಯ Mat_Op_ತಟಸ್ಥ::make_ಅಭಿವ್ಯಕ್ತಿ(Mat_ಅಭಿವ್ಯಕ್ತಿ& res, ಸ್ಥಿರಾಂಕ Mat& m)
{
    res = Mat_ಅಭಿವ್ಯಕ್ತಿ(&g_Mat_Op_ತಟಸ್ಥ, 0, m, Mat(), Mat(), 1, 0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

ಶೂನ್ಯ Mat_Op_ಕೂಡಿಸು_Ex::assign(ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& e, Mat& m, ಪೂರ್ಣಾಂಕ _type) ಸ್ಥಿರಾಂಕ
{
    Mat ತಾತ್ಕಾಲಿಕ_2, &dst = _type == -1 || e.a.type() == _type ? m : ತಾತ್ಕಾಲಿಕ_2;
    ಆದರೆ( e.b.ದತ್ತಾಂಶ )
    {
        ಆದರೆ( e.s == Scalar() || !e.s.isReal() )
        {
            ಆದರೆ( e.alpha == 1 )
            {
                ಆದರೆ( e.beta == 1 )
                    cv::ಕೂಡಿಸು(e.a, e.b, dst);
                ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( e.beta == -1 )
                    cv::ಕಳೆ(e.a, e.b, dst);
                ಇಲ್ಲದಿದ್ದರೆ
                    cv::scale_ಕೂಡಿಸು(e.b, e.beta, e.a, dst);
            }
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( e.beta == 1 )
            {
                ಆದರೆ( e.alpha == -1 )
                    cv::ಕಳೆ(e.b, e.a, dst);
                ಇಲ್ಲದಿದ್ದರೆ
                    cv::scale_ಕೂಡಿಸು(e.a, e.alpha, e.b, dst);
            }
            ಇಲ್ಲದಿದ್ದರೆ
                cv::ಕೂಡಿಸು_Weighted(e.a, e.alpha, e.b, e.beta, 0, dst);

            ಆದರೆ( !e.s.isReal() )
                cv::ಕೂಡಿಸು(dst, e.s, dst);
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            ಆದರೆ (e.a.channels() > 1)
                CV_LOG_ONCE_WARNING(ಖಾಲಿ, "OpenCV/MatExpr: processing of multi-channel arrays might be changed in the future: "
                                          "https://github.com/opencv/opencv/issues/16739");
            cv::ಕೂಡಿಸು_Weighted(e.a, e.alpha, e.b, e.beta, e.s[0], dst);
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( e.s.isReal() && (dst.ದತ್ತಾಂಶ != m.ದತ್ತಾಂಶ || fabs(e.alpha) != 1))
    {
        ಆದರೆ (e.a.channels() > 1 && e.s[0] != 0.0)
            CV_LOG_ONCE_WARNING(ಖಾಲಿ, "OpenCV/MatExpr: processing of multi-channel arrays might be changed in the future: "
                                      "https://github.com/opencv/opencv/issues/16739");
        e.a.convert_ಗಮ್ಯ(m, _type, e.alpha, e.s[0]);
        ಹಿಂತಿರುಗಿ;
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( e.alpha == 1 )
        cv::ಕೂಡಿಸು(e.a, e.s, dst);
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( e.alpha == -1 )
        cv::ಕಳೆ(e.s, e.a, dst);
    ಇಲ್ಲದಿದ್ದರೆ
    {
        e.a.convert_ಗಮ್ಯ(dst, e.a.type(), e.alpha);
        cv::ಕೂಡಿಸು(dst, e.s, dst);
    }

    ಆದರೆ( dst.ದತ್ತಾಂಶ != m.ದತ್ತಾಂಶ )
        dst.convert_ಗಮ್ಯ(m, m.type());
}


ಶೂನ್ಯ Mat_Op_ಕೂಡಿಸು_Ex::ಕೂಡಿಸು(ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& e, ಸ್ಥಿರಾಂಕ Scalar& s, Mat_ಅಭಿವ್ಯಕ್ತಿ& res) ಸ್ಥಿರಾಂಕ
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.s += s;
}


ಶೂನ್ಯ Mat_Op_ಕೂಡಿಸು_Ex::ಕಳೆ(ಸ್ಥಿರಾಂಕ Scalar& s, ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& e, Mat_ಅಭಿವ್ಯಕ್ತಿ& res) ಸ್ಥಿರಾಂಕ
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha = -res.alpha;
    res.beta = -res.beta;
    res.s = s - res.s;
}

ಶೂನ್ಯ Mat_Op_ಕೂಡಿಸು_Ex::multiply(ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& e, ದ್ವಿಗುಣ s, Mat_ಅಭಿವ್ಯಕ್ತಿ& res) ಸ್ಥಿರಾಂಕ
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha *= s;
    res.beta *= s;
    res.s *= s;
}

ಶೂನ್ಯ Mat_Op_ಕೂಡಿಸು_Ex::divide(ದ್ವಿಗುಣ s, ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& e, Mat_ಅಭಿವ್ಯಕ್ತಿ& res) ಸ್ಥಿರಾಂಕ
{
    CV_INSTRUMENT_REGION();

    ಆದರೆ( isScaled(e) )
        MatOp_Bin::make_ಅಭಿವ್ಯಕ್ತಿ(res, '/', e.a, Mat(), s/e.alpha);
    ಇಲ್ಲದಿದ್ದರೆ
        MatOp::divide(s, e, res);
}


ಶೂನ್ಯ Mat_Op_ಕೂಡಿಸು_Ex::transpose(ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& e, Mat_ಅಭಿವ್ಯಕ್ತಿ& res) ಸ್ಥಿರಾಂಕ
{
    CV_INSTRUMENT_REGION();

    ಆದರೆ( isScaled(e) )
        MatOp_T::make_ಅಭಿವ್ಯಕ್ತಿ(res, e.a, e.alpha);
    ಇಲ್ಲದಿದ್ದರೆ
        MatOp::transpose(e, res);
}

ಶೂನ್ಯ Mat_Op_ಕೂಡಿಸು_Ex::ನಿರಪೇಕ್ಷ(ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& e, Mat_ಅಭಿವ್ಯಕ್ತಿ& res) ಸ್ಥಿರಾಂಕ
{
    CV_INSTRUMENT_REGION();

    ಆದರೆ( (!e.b.ದತ್ತಾಂಶ || e.beta == 0) && fabs(e.alpha) == 1 )
        MatOp_Bin::make_ಅಭಿವ್ಯಕ್ತಿ(res, 'a', e.a, -e.s*e.alpha);
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( e.b.ದತ್ತಾಂಶ && e.alpha + e.beta == 0 && e.alpha*e.beta == -1 )
        MatOp_Bin::make_ಅಭಿವ್ಯಕ್ತಿ(res, 'a', e.a, e.b);
    ಇಲ್ಲದಿದ್ದರೆ
        MatOp::ನಿರಪೇಕ್ಷ(e, res);
}

ಸಾಲೊಳಗೆ ಶೂನ್ಯ Mat_Op_ಕೂಡಿಸು_Ex::make_ಅಭಿವ್ಯಕ್ತಿ(Mat_ಅಭಿವ್ಯಕ್ತಿ& res, ಸ್ಥಿರಾಂಕ Mat& a, ಸ್ಥಿರಾಂಕ Mat& b, ದ್ವಿಗುಣ alpha, ದ್ವಿಗುಣ beta, ಸ್ಥಿರಾಂಕ Scalar& s)
{
    res = Mat_ಅಭಿವ್ಯಕ್ತಿ(&g_Mat_Op_ಕೂಡಿಸು_Ex, 0, a, b, Mat(), alpha, beta, s);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ಶೂನ್ಯ MatOp_Bin::assign(ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& e, Mat& m, ಪೂರ್ಣಾಂಕ _type) ಸ್ಥಿರಾಂಕ
{
    Mat ತಾತ್ಕಾಲಿಕ_2, &dst = _type == -1 || e.a.type() == _type ? m : ತಾತ್ಕಾಲಿಕ_2;

    ಆದರೆ( e.flags == '*' )
        cv::multiply(e.a, e.b, dst, e.alpha);
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( e.flags == '/' && e.b.ದತ್ತಾಂಶ )
        cv::divide(e.a, e.b, dst, e.alpha);
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( e.flags == '/' && !e.b.ದತ್ತಾಂಶ )
        cv::divide(e.alpha, e.a, dst );
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( e.flags == '&' && e.b.ದತ್ತಾಂಶ )
        bitwise_and(e.a, e.b, dst);
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( e.flags == '&' && !e.b.ದತ್ತಾಂಶ )
        bitwise_and(e.a, e.s, dst);
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( e.flags == '|' && e.b.ದತ್ತಾಂಶ )
        bitwise_or(e.a, e.b, dst);
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( e.flags == '|' && !e.b.ದತ್ತಾಂಶ )
        bitwise_or(e.a, e.s, dst);
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( e.flags == '^' && e.b.ದತ್ತಾಂಶ )
        bitwise_xor(e.a, e.b, dst);
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( e.flags == '^' && !e.b.ದತ್ತಾಂಶ )
        bitwise_xor(e.a, e.s, dst);
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( e.flags == '~' && !e.b.ದತ್ತಾಂಶ )
        bitwise_not(e.a, dst);
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( e.flags == 'm' )
        cv::min(e.a, e.b, dst);
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( e.flags == 'n' )
        cv::min(e.a, e.s[0], dst);
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( e.flags == 'M' )
        cv::max(e.a, e.b, dst);
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( e.flags == 'N' )
        cv::max(e.a, e.s[0], dst);
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( e.flags == 'a' && e.b.ದತ್ತಾಂಶ )
        cv::absdiff(e.a, e.b, dst);
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( e.flags == 'a' && !e.b.ದತ್ತಾಂಶ )
        cv::absdiff(e.a, e.s, dst);
    ಇಲ್ಲದಿದ್ದರೆ
        CV_Error(cv::Error::StsError, "Unknown operation");

    ಆದರೆ( dst.ದತ್ತಾಂಶ != m.ದತ್ತಾಂಶ )
        dst.convert_ಗಮ್ಯ(m, _type);
}

ಶೂನ್ಯ MatOp_Bin::multiply(ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& e, ದ್ವಿಗುಣ s, Mat_ಅಭಿವ್ಯಕ್ತಿ& res) ಸ್ಥಿರಾಂಕ
{
    CV_INSTRUMENT_REGION();

    ಆದರೆ( e.flags == '*' || e.flags == '/' )
    {
        res = e;
        res.alpha *= s;
    }
    ಇಲ್ಲದಿದ್ದರೆ
        MatOp::multiply(e, s, res);
}

ಶೂನ್ಯ MatOp_Bin::divide(ದ್ವಿಗುಣ s, ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& e, Mat_ಅಭಿವ್ಯಕ್ತಿ& res) ಸ್ಥಿರಾಂಕ
{
    CV_INSTRUMENT_REGION();

    ಆದರೆ( e.flags == '/' && (!e.b.ದತ್ತಾಂಶ || e.beta == 0) )
        Mat_Op_ಕೂಡಿಸು_Ex::make_ಅಭಿವ್ಯಕ್ತಿ(res, e.a, Mat(), s/e.alpha, 0);
    ಇಲ್ಲದಿದ್ದರೆ
        MatOp::divide(s, e, res);
}

ಸಾಲೊಳಗೆ ಶೂನ್ಯ MatOp_Bin::make_ಅಭಿವ್ಯಕ್ತಿ(Mat_ಅಭಿವ್ಯಕ್ತಿ& res, ಅಕ್ಷರ op, ಸ್ಥಿರಾಂಕ Mat& a, ಸ್ಥಿರಾಂಕ Mat& b, ದ್ವಿಗುಣ scale)
{
    res = Mat_ಅಭಿವ್ಯಕ್ತಿ(&g_MatOp_Bin, op, a, b, Mat(), scale, b.ದತ್ತಾಂಶ ? 1 : 0);
}

ಸಾಲೊಳಗೆ ಶೂನ್ಯ MatOp_Bin::make_ಅಭಿವ್ಯಕ್ತಿ(Mat_ಅಭಿವ್ಯಕ್ತಿ& res, ಅಕ್ಷರ op, ಸ್ಥಿರಾಂಕ Mat& a, ಸ್ಥಿರಾಂಕ Scalar& s)
{
    res = Mat_ಅಭಿವ್ಯಕ್ತಿ(&g_MatOp_Bin, op, a, Mat(), Mat(), 1, 0, s);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

ಶೂನ್ಯ MatOp_Cmp::assign(ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& e, Mat& m, ಪೂರ್ಣಾಂಕ _type) ಸ್ಥಿರಾಂಕ
{
    Mat ತಾತ್ಕಾಲಿಕ_2, &dst = _type == -1 || _type == CV_8U ? m : ತಾತ್ಕಾಲಿಕ_2;

    ಆದರೆ( e.b.ದತ್ತಾಂಶ )
        cv::compare(e.a, e.b, dst, e.flags);
    ಇಲ್ಲದಿದ್ದರೆ
        cv::compare(e.a, e.alpha, dst, e.flags);

    ಆದರೆ( dst.ದತ್ತಾಂಶ != m.ದತ್ತಾಂಶ )
        dst.convert_ಗಮ್ಯ(m, _type);
}

ಸಾಲೊಳಗೆ ಶೂನ್ಯ MatOp_Cmp::make_ಅಭಿವ್ಯಕ್ತಿ(Mat_ಅಭಿವ್ಯಕ್ತಿ& res, ಪೂರ್ಣಾಂಕ cmpop, ಸ್ಥಿರಾಂಕ Mat& a, ಸ್ಥಿರಾಂಕ Mat& b)
{
    res = Mat_ಅಭಿವ್ಯಕ್ತಿ(&g_MatOp_Cmp, cmpop, a, b, Mat(), 1, 1);
}

ಸಾಲೊಳಗೆ ಶೂನ್ಯ MatOp_Cmp::make_ಅಭಿವ್ಯಕ್ತಿ(Mat_ಅಭಿವ್ಯಕ್ತಿ& res, ಪೂರ್ಣಾಂಕ cmpop, ಸ್ಥಿರಾಂಕ Mat& a, ದ್ವಿಗುಣ alpha)
{
    res = Mat_ಅಭಿವ್ಯಕ್ತಿ(&g_MatOp_Cmp, cmpop, a, Mat(), Mat(), alpha, 1);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

ಶೂನ್ಯ MatOp_T::assign(ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& e, Mat& m, ಪೂರ್ಣಾಂಕ _type) ಸ್ಥಿರಾಂಕ
{
    Mat ತಾತ್ಕಾಲಿಕ_2, &dst = _type == -1 || _type == e.a.type() ? m : ತಾತ್ಕಾಲಿಕ_2;

    cv::transpose(e.a, dst);

    ಆದರೆ( dst.ದತ್ತಾಂಶ != m.ದತ್ತಾಂಶ || e.alpha != 1 )
        dst.convert_ಗಮ್ಯ(m, _type, e.alpha);
}

ಶೂನ್ಯ MatOp_T::multiply(ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& e, ದ್ವಿಗುಣ s, Mat_ಅಭಿವ್ಯಕ್ತಿ& res) ಸ್ಥಿರಾಂಕ
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha *= s;
}

ಶೂನ್ಯ MatOp_T::transpose(ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& e, Mat_ಅಭಿವ್ಯಕ್ತಿ& res) ಸ್ಥಿರಾಂಕ
{
    CV_INSTRUMENT_REGION();

    ಆದರೆ( e.alpha == 1 )
        Mat_Op_ತಟಸ್ಥ::make_ಅಭಿವ್ಯಕ್ತಿ(res, e.a);
    ಇಲ್ಲದಿದ್ದರೆ
        Mat_Op_ಕೂಡಿಸು_Ex::make_ಅಭಿವ್ಯಕ್ತಿ(res, e.a, Mat(), e.alpha, 0);
}

ಸಾಲೊಳಗೆ ಶೂನ್ಯ MatOp_T::make_ಅಭಿವ್ಯಕ್ತಿ(Mat_ಅಭಿವ್ಯಕ್ತಿ& res, ಸ್ಥಿರಾಂಕ Mat& a, ದ್ವಿಗುಣ alpha)
{
    res = Mat_ಅಭಿವ್ಯಕ್ತಿ(&g_MatOp_T, 0, a, Mat(), Mat(), alpha, 0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

ಶೂನ್ಯ MatOp_GEMM::assign(ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& e, Mat& m, ಪೂರ್ಣಾಂಕ _type) ಸ್ಥಿರಾಂಕ
{
    Mat ತಾತ್ಕಾಲಿಕ_2, &dst = _type == -1 || _type == e.a.type() ? m : ತಾತ್ಕಾಲಿಕ_2;

    cv::gemm(e.a, e.b, e.alpha, e.c, e.beta, dst, e.flags);
    ಆದರೆ( dst.ದತ್ತಾಂಶ != m.ದತ್ತಾಂಶ )
        dst.convert_ಗಮ್ಯ(m, _type);
}

ಶೂನ್ಯ MatOp_GEMM::ಕೂಡಿಸು(ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& e1, ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& e2, Mat_ಅಭಿವ್ಯಕ್ತಿ& res) ಸ್ಥಿರಾಂಕ
{
    CV_INSTRUMENT_REGION();

    ತಾರ್ಕಿಕ i1 = is_ತಟಸ್ಥ(e1), i2 = is_ತಟಸ್ಥ(e2);
    ದ್ವಿಗುಣ alpha1 = i1 ? 1 : e1.alpha, alpha2 = i2 ? 1 : e2.alpha;

    ಆದರೆ( isMatProd(e1) && (i2 || isScaled(e2) || isT(e2)) )
        MatOp_GEMM::make_ಅಭಿವ್ಯಕ್ತಿ(res, (e1.flags & ~cv::GEMM_3_T)|(isT(e2) ? cv::GEMM_3_T : 0),
                             e1.a, e1.b, alpha1, e2.a, alpha2);
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( isMatProd(e2) && (i1 || isScaled(e1) || isT(e1)) )
        MatOp_GEMM::make_ಅಭಿವ್ಯಕ್ತಿ(res, (e2.flags & ~cv::GEMM_3_T)|(isT(e1) ? cv::GEMM_3_T : 0),
                             e2.a, e2.b, alpha2, e1.a, alpha1);
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( ಇದು == e2.op )
        MatOp::ಕೂಡಿಸು(e1, e2, res);
    ಇಲ್ಲದಿದ್ದರೆ
        e2.op->ಕೂಡಿಸು(e1, e2, res);
}

ಶೂನ್ಯ MatOp_GEMM::ಕಳೆ(ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& e1, ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& e2, Mat_ಅಭಿವ್ಯಕ್ತಿ& res) ಸ್ಥಿರಾಂಕ
{
    CV_INSTRUMENT_REGION();

    ತಾರ್ಕಿಕ i1 = is_ತಟಸ್ಥ(e1), i2 = is_ತಟಸ್ಥ(e2);
    ದ್ವಿಗುಣ alpha1 = i1 ? 1 : e1.alpha, alpha2 = i2 ? 1 : e2.alpha;

    ಆದರೆ( isMatProd(e1) && (i2 || isScaled(e2) || isT(e2)) )
        MatOp_GEMM::make_ಅಭಿವ್ಯಕ್ತಿ(res, (e1.flags & ~cv::GEMM_3_T)|(isT(e2) ? cv::GEMM_3_T : 0),
                             e1.a, e1.b, alpha1, e2.a, -alpha2);
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( isMatProd(e2) && (i1 || isScaled(e1) || isT(e1)) )
        MatOp_GEMM::make_ಅಭಿವ್ಯಕ್ತಿ(res, (e2.flags & ~cv::GEMM_3_T)|(isT(e1) ? cv::GEMM_3_T : 0),
                            e2.a, e2.b, -alpha2, e1.a, alpha1);
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( ಇದು == e2.op )
        MatOp::ಕಳೆ(e1, e2, res);
    ಇಲ್ಲದಿದ್ದರೆ
        e2.op->ಕಳೆ(e1, e2, res);
}

ಶೂನ್ಯ MatOp_GEMM::multiply(ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& e, ದ್ವಿಗುಣ s, Mat_ಅಭಿವ್ಯಕ್ತಿ& res) ಸ್ಥಿರಾಂಕ
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha *= s;
    res.beta *= s;
}

ಶೂನ್ಯ MatOp_GEMM::transpose(ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& e, Mat_ಅಭಿವ್ಯಕ್ತಿ& res) ಸ್ಥಿರಾಂಕ
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.flags = (!(e.flags & cv::GEMM_1_T) ? cv::GEMM_2_T : 0) |
                (!(e.flags & cv::GEMM_2_T) ? cv::GEMM_1_T : 0) |
                (!(e.flags & cv::GEMM_3_T) ? cv::GEMM_3_T : 0);
    swap(res.a, res.b);
}

ಸಾಲೊಳಗೆ ಶೂನ್ಯ MatOp_GEMM::make_ಅಭಿವ್ಯಕ್ತಿ(Mat_ಅಭಿವ್ಯಕ್ತಿ& res, ಪೂರ್ಣಾಂಕ flags, ಸ್ಥಿರಾಂಕ Mat& a, ಸ್ಥಿರಾಂಕ Mat& b,
                                 ದ್ವಿಗುಣ alpha, ಸ್ಥಿರಾಂಕ Mat& c, ದ್ವಿಗುಣ beta)
{
    res = Mat_ಅಭಿವ್ಯಕ್ತಿ(&g_MatOp_GEMM, flags, a, b, c, alpha, beta);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

ಶೂನ್ಯ MatOp_Invert::assign(ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& e, Mat& m, ಪೂರ್ಣಾಂಕ _type) ಸ್ಥಿರಾಂಕ
{
    Mat ತಾತ್ಕಾಲಿಕ_2, &dst = _type == -1 || _type == e.a.type() ? m : ತಾತ್ಕಾಲಿಕ_2;

    cv::invert(e.a, dst, e.flags);
    ಆದರೆ( dst.ದತ್ತಾಂಶ != m.ದತ್ತಾಂಶ )
        dst.convert_ಗಮ್ಯ(m, _type);
}

ಶೂನ್ಯ MatOp_Invert::matmul(ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& e1, ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& e2, Mat_ಅಭಿವ್ಯಕ್ತಿ& res) ಸ್ಥಿರಾಂಕ
{
    ಆದರೆ( isInv(e1) && is_ತಟಸ್ಥ(e2) )
        MatOp_Solve::make_ಅಭಿವ್ಯಕ್ತಿ(res, e1.flags, e1.a, e2.a);
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( ಇದು == e2.op )
        MatOp::matmul(e1, e2, res);
    ಇಲ್ಲದಿದ್ದರೆ
        e2.op->matmul(e1, e2, res);
}

ಸಾಲೊಳಗೆ ಶೂನ್ಯ MatOp_Invert::make_ಅಭಿವ್ಯಕ್ತಿ(Mat_ಅಭಿವ್ಯಕ್ತಿ& res, ಪೂರ್ಣಾಂಕ ವಿಧಾನ, ಸ್ಥಿರಾಂಕ Mat& m)
{
    res = Mat_ಅಭಿವ್ಯಕ್ತಿ(&g_MatOp_Invert, ವಿಧಾನ, m, Mat(), Mat(), 1, 0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

ಶೂನ್ಯ MatOp_Solve::assign(ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& e, Mat& m, ಪೂರ್ಣಾಂಕ _type) ಸ್ಥಿರಾಂಕ
{
    Mat ತಾತ್ಕಾಲಿಕ_2, &dst = _type == -1 || _type == e.a.type() ? m : ತಾತ್ಕಾಲಿಕ_2;

    cv::solve(e.a, e.b, dst, e.flags);
    ಆದರೆ( dst.ದತ್ತಾಂಶ != m.ದತ್ತಾಂಶ )
        dst.convert_ಗಮ್ಯ(m, _type);
}

ಸಾಲೊಳಗೆ ಶೂನ್ಯ MatOp_Solve::make_ಅಭಿವ್ಯಕ್ತಿ(Mat_ಅಭಿವ್ಯಕ್ತಿ& res, ಪೂರ್ಣಾಂಕ ವಿಧಾನ, ಸ್ಥಿರಾಂಕ Mat& a, ಸ್ಥಿರಾಂಕ Mat& b)
{
    res = Mat_ಅಭಿವ್ಯಕ್ತಿ(&g_MatOp_Solve, ವಿಧಾನ, a, b, Mat(), 1, 1);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

ಶೂನ್ಯ MatOp_Initializer::assign(ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& e, Mat& m, ಪೂರ್ಣಾಂಕ _type) ಸ್ಥಿರಾಂಕ
{
    ಆದರೆ( _type == -1 )
        _type = e.a.type();

    m.create(e.a.size, _type);

    ಆದರೆ( e.flags == 'I' && e.a.dims <= 2 )
        ಗಣ_ತಟಸ್ಥ(m, Scalar(e.alpha));
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( e.flags == '0' )
        m = Scalar();
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( e.flags == '1' )
        m = Scalar(e.alpha);
    ಇಲ್ಲದಿದ್ದರೆ
        CV_Error(cv::Error::StsError, "Invalid matrix initializer type");
}

ಶೂನ್ಯ MatOp_Initializer::multiply(ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& e, ದ್ವಿಗುಣ s, Mat_ಅಭಿವ್ಯಕ್ತಿ& res) ಸ್ಥಿರಾಂಕ
{
    CV_INSTRUMENT_REGION();

    res = e;
    res.alpha *= s;
}

ಸಾಲೊಳಗೆ ಶೂನ್ಯ MatOp_Initializer::make_ಅಭಿವ್ಯಕ್ತಿ(Mat_ಅಭಿವ್ಯಕ್ತಿ& res, ಪೂರ್ಣಾಂಕ ವಿಧಾನ, ಗಾತ್ರ_2 sz, ಪೂರ್ಣಾಂಕ type, ದ್ವಿಗುಣ alpha)
{
    res = Mat_ಅಭಿವ್ಯಕ್ತಿ(getGlobalMatOpInitializer(), ವಿಧಾನ, Mat(sz, type, (ಶೂನ್ಯ*)(size_t)0xEEEEEEEE), Mat(), Mat(), alpha, 0);
}

ಸಾಲೊಳಗೆ ಶೂನ್ಯ MatOp_Initializer::make_ಅಭಿವ್ಯಕ್ತಿ(Mat_ಅಭಿವ್ಯಕ್ತಿ& res, ಪೂರ್ಣಾಂಕ ವಿಧಾನ, ಪೂರ್ಣಾಂಕ ndims, ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* sizes, ಪೂರ್ಣಾಂಕ type, ದ್ವಿಗುಣ alpha)
{
    res = Mat_ಅಭಿವ್ಯಕ್ತಿ(getGlobalMatOpInitializer(), ವಿಧಾನ, Mat(ndims, sizes, type, (ಶೂನ್ಯ*)(size_t)0xEEEEEEEE), Mat(), Mat(), alpha, 0);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

Mat_ಅಭಿವ್ಯಕ್ತಿ Mat::t() ಸ್ಥಿರಾಂಕ
{
    CV_INSTRUMENT_REGION();

    checkOperandsExist(*ಇದು);
    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    MatOp_T::make_ಅಭಿವ್ಯಕ್ತಿ(e, *ಇದು);
    ಹಿಂತಿರುಗಿ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ Mat::inv(ಪೂರ್ಣಾಂಕ ವಿಧಾನ) ಸ್ಥಿರಾಂಕ
{
    CV_INSTRUMENT_REGION();

    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    MatOp_Invert::make_ಅಭಿವ್ಯಕ್ತಿ(e, ವಿಧಾನ, *ಇದು);
    ಹಿಂತಿರುಗಿ e;
}


Mat_ಅಭಿವ್ಯಕ್ತಿ Mat::mul(ಒಳಹರಿವು_Array m, ದ್ವಿಗುಣ scale) ಸ್ಥಿರಾಂಕ
{
    CV_INSTRUMENT_REGION();

    Mat b = m.getMat();
    // Unless the argument is a refcounted Mat/UMat, the header returned by getMat() may be
    // a non-owning view of caller memory (e.g. a scalar bound to _InputArray(const double&),
    // a Matx or a Vec) that the returned MatExpr can outlive, so snapshot it.
    // See https://github.com/opencv/opencv/issues/23577
    ಆದರೆ( !m.isMat() && !m.isUMat() )
        b = b.clone();

    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    MatOp_Bin::make_ಅಭಿವ್ಯಕ್ತಿ(e, '*', *ಇದು, b, scale);
    ಹಿಂತಿರುಗಿ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ Mat::zeros(ಪೂರ್ಣಾಂಕ ಅಡ್ಡಸಾಲುಗಳು, ಪೂರ್ಣಾಂಕ ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, ಪೂರ್ಣಾಂಕ type)
{
    CV_INSTRUMENT_REGION();

    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    MatOp_Initializer::make_ಅಭಿವ್ಯಕ್ತಿ(e, '0', ಗಾತ್ರ_2(ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, ಅಡ್ಡಸಾಲುಗಳು), type);
    ಹಿಂತಿರುಗಿ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ Mat::zeros(ಗಾತ್ರ_2 size, ಪೂರ್ಣಾಂಕ type)
{
    CV_INSTRUMENT_REGION();

    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    MatOp_Initializer::make_ಅಭಿವ್ಯಕ್ತಿ(e, '0', size, type);
    ಹಿಂತಿರುಗಿ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ Mat::zeros(ಪೂರ್ಣಾಂಕ ndims, ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* sizes, ಪೂರ್ಣಾಂಕ type)
{
    CV_INSTRUMENT_REGION();

    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    MatOp_Initializer::make_ಅಭಿವ್ಯಕ್ತಿ(e, '0', ndims, sizes, type);
    ಹಿಂತಿರುಗಿ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ Mat::zeros(ಸ್ಥಿರಾಂಕ MatShape& shape, ಪೂರ್ಣಾಂಕ type)
{
    CV_INSTRUMENT_REGION();

    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    MatOp_Initializer::make_ಅಭಿವ್ಯಕ್ತಿ(e, '0', shape.dims, shape.p, type);
    ಹಿಂತಿರುಗಿ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ Mat::ones(ಪೂರ್ಣಾಂಕ ಅಡ್ಡಸಾಲುಗಳು, ಪೂರ್ಣಾಂಕ ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, ಪೂರ್ಣಾಂಕ type)
{
    CV_INSTRUMENT_REGION();

    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    MatOp_Initializer::make_ಅಭಿವ್ಯಕ್ತಿ(e, '1', ಗಾತ್ರ_2(ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, ಅಡ್ಡಸಾಲುಗಳು), type);
    ಹಿಂತಿರುಗಿ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ Mat::ones(ಗಾತ್ರ_2 size, ಪೂರ್ಣಾಂಕ type)
{
    CV_INSTRUMENT_REGION();

    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    MatOp_Initializer::make_ಅಭಿವ್ಯಕ್ತಿ(e, '1', size, type);
    ಹಿಂತಿರುಗಿ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ Mat::ones(ಪೂರ್ಣಾಂಕ ndims, ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* sizes, ಪೂರ್ಣಾಂಕ type)
{
    CV_INSTRUMENT_REGION();

    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    MatOp_Initializer::make_ಅಭಿವ್ಯಕ್ತಿ(e, '1', ndims, sizes, type);
    ಹಿಂತಿರುಗಿ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ Mat::ones(ಸ್ಥಿರಾಂಕ MatShape& shape, ಪೂರ್ಣಾಂಕ type)
{
    CV_INSTRUMENT_REGION();

    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    MatOp_Initializer::make_ಅಭಿವ್ಯಕ್ತಿ(e, '1', shape.dims, shape.p, type);
    ಹಿಂತಿರುಗಿ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ Mat::eye(ಪೂರ್ಣಾಂಕ ಅಡ್ಡಸಾಲುಗಳು, ಪೂರ್ಣಾಂಕ ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, ಪೂರ್ಣಾಂಕ type)
{
    CV_INSTRUMENT_REGION();

    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    MatOp_Initializer::make_ಅಭಿವ್ಯಕ್ತಿ(e, 'I', ಗಾತ್ರ_2(ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, ಅಡ್ಡಸಾಲುಗಳು), type);
    ಹಿಂತಿರುಗಿ e;
}

Mat_ಅಭಿವ್ಯಕ್ತಿ Mat::eye(ಗಾತ್ರ_2 size, ಪೂರ್ಣಾಂಕ type)
{
    CV_INSTRUMENT_REGION();

    Mat_ಅಭಿವ್ಯಕ್ತಿ e;
    MatOp_Initializer::make_ಅಭಿವ್ಯಕ್ತಿ(e, 'I', size, type);
    ಹಿಂತಿರುಗಿ e;
}

ಶೂನ್ಯ Mat_ಅಭಿವ್ಯಕ್ತಿ::swap(Mat_ಅಭಿವ್ಯಕ್ತಿ& other)
{
    ಬಳಸಿ std::swap;

    swap(op, other.op);
    swap(flags, other.flags);

    swap(a, other.a);
    swap(b, other.b);
    swap(c, other.c);

    swap(alpha, other.alpha);
    swap(beta, other.beta);

    swap(s, other.s);
}

ಒಳಹರಿವು_Array_2::ಒಳಹರಿವು_Array_2(ಸ್ಥಿರಾಂಕ Mat_ಅಭಿವ್ಯಕ್ತಿ& ಅಭಿವ್ಯಕ್ತಿ)
{
    ಆದರೆ (!is_ತಟಸ್ಥ(ಅಭಿವ್ಯಕ್ತಿ))
    {
        Mat ಫಲಿತಾಂಶ = ಅಭಿವ್ಯಕ್ತಿ;  // TODO improve through refcount == 1 of expr.a (inplace operation is possible - except gemm?)
        Mat_ಅಭಿವ್ಯಕ್ತಿ ಫಲಿತಾಂಶ_ಅಭಿವ್ಯಕ್ತಿ(ಫಲಿತಾಂಶ);
        swap(const_cast<Mat_ಅಭಿವ್ಯಕ್ತಿ&>(ಅಭಿವ್ಯಕ್ತಿ), ಫಲಿತಾಂಶ_ಅಭಿವ್ಯಕ್ತಿ);
    }
    CV_Assert(is_ತಟಸ್ಥ(ಅಭಿವ್ಯಕ್ತಿ));
    init(FIXED_TYPE + FIXED_ಗಾತ್ರ + MAT + ACCESS_READ, &ಅಭಿವ್ಯಕ್ತಿ.a);
}

} // cv::
