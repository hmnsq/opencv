// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ಅನ್ತರ್ಭಾವ "precomp.hpp"

#ಅನ್ತರ್ಭಾವ <sstream>

#ಅನ್ತರ್ಭಾವ "opencv2/core/check.hpp"

ನಾಮಾಕಾಶ cv {

ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* ಆಳ_ಗಮ್ಯ_String(ಪೂರ್ಣಾಂಕ ಆಳ)
{
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* s = detail::ಆಳ_ಗಮ್ಯ_String_2(ಆಳ);
    ಹಿಂತಿರುಗಿ s ? s : "<invalid depth>";
}

cv::String type_ಗಮ್ಯ_String(ಪೂರ್ಣಾಂಕ type)
{
    cv::String s = detail::type_ಗಮ್ಯ_String_2(type);
    ಆದರೆ (s.empty())
    {
        ಸ್ಥಿರ cv::String invalidType("<invalid type>");
        ಹಿಂತಿರುಗಿ invalidType;
    }
    ಹಿಂತಿರುಗಿ s;
}


ನಾಮಾಕಾಶ detail {

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* getTestOpPhraseStr(ಚಿಹ್ನೆರಹಿತ testOp)
{
    ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* ಹೆಸರುಗಳು[] = { "{custom check}", "equal to", "not equal to", "less than or equal to", "less than", "greater than or equal to", "greater than" };
    CV_DbgAssert(testOp < CV_ಕೊನೆಯ_TEST_OP);
    ಹಿಂತಿರುಗಿ testOp < CV_ಕೊನೆಯ_TEST_OP ? ಹೆಸರುಗಳು[testOp] : "???";
}
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* getTestOpMath(ಚಿಹ್ನೆರಹಿತ testOp)
{
    ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* ಹೆಸರುಗಳು[] = { "???", "==", "!=", "<=", "<", ">=", ">" };
    CV_DbgAssert(testOp < CV_ಕೊನೆಯ_TEST_OP);
    ಹಿಂತಿರುಗಿ testOp < CV_ಕೊನೆಯ_TEST_OP ? ಹೆಸರುಗಳು[testOp] : "???";
}

ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* ಆಳ_ಗಮ್ಯ_String_2(ಪೂರ್ಣಾಂಕ ಆಳ)
{
    ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* ಆಳ_ಹೆಸರುಗಳು[] = { "CV_8U", "CV_8S", "CV_16U", "CV_16S", "CV_32S", "CV_32F", "CV_64F", "CV_16F",
                                        "CV_16BF", "CV_Bool", "CV_64U", "CV_64S", "CV_32U",
                                        "CV_8F", "CV_8FNUZ" };
    ಹಿಂತಿರುಗಿ (ಆಳ < CV_ಆಳ_CURR_MAX && ಆಳ >= 0) ? ಆಳ_ಹೆಸರುಗಳು[ಆಳ] : ಖಾಲಿ;
}

cv::String type_ಗಮ್ಯ_String_2(ಪೂರ್ಣಾಂಕ type)
{
    ಪೂರ್ಣಾಂಕ ಆಳ = CV_MAT_ಆಳ(type);
    ಪೂರ್ಣಾಂಕ cn = CV_MAT_CN(type);
    ಆದರೆ (ಆಳ >= 0 && ಆಳ < CV_ಆಳ_CURR_MAX)
        ಹಿಂತಿರುಗಿ cv::format("%sC%d", ಆಳ_ಗಮ್ಯ_String_2(ಆಳ), cn);
    ಹಿಂತಿರುಗಿ cv::String();
}

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T> ಸ್ಥಿರ CV_NORETURN
ಶೂನ್ಯ check_failed_auto_(ಸ್ಥಿರಾಂಕ T& v1, ಸ್ಥಿರಾಂಕ T& v2, ಸ್ಥಿರಾಂಕ CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << " (expected: '" << ctx.p1_str << " " << getTestOpMath(ctx.testOp) << " " << ctx.p2_str << "'), where" << std::ಸಾಲು_ಅಂತ್ಯ
        << "    '" << ctx.p1_str << "' is " << v1 << std::ಸಾಲು_ಅಂತ್ಯ;
    ಆದರೆ (ctx.testOp != TEST_CUSTOM && ctx.testOp < CV_ಕೊನೆಯ_TEST_OP)
    {
        ss << "must be " << getTestOpPhraseStr(ctx.testOp) << std::ಸಾಲು_ಅಂತ್ಯ;
    }
    ss  << "    '" << ctx.p2_str << "' is " << v2;
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.ಸಾಲು);
}
ಶೂನ್ಯ check_failed_Mat_ಆಳ(ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ v1, ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ v2, ಸ್ಥಿರಾಂಕ CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << " (expected: '" << ctx.p1_str << " " << getTestOpMath(ctx.testOp) << " " << ctx.p2_str << "'), where" << std::ಸಾಲು_ಅಂತ್ಯ
        << "    '" << ctx.p1_str << "' is " << v1 << " (" << ಆಳ_ಗಮ್ಯ_String(v1) << ")" << std::ಸಾಲು_ಅಂತ್ಯ;
    ಆದರೆ (ctx.testOp != TEST_CUSTOM && ctx.testOp < CV_ಕೊನೆಯ_TEST_OP)
    {
        ss << "must be " << getTestOpPhraseStr(ctx.testOp) << std::ಸಾಲು_ಅಂತ್ಯ;
    }
    ss  << "    '" << ctx.p2_str << "' is " << v2 << " (" << ಆಳ_ಗಮ್ಯ_String(v2) << ")";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.ಸಾಲು);
}
ಶೂನ್ಯ check_failed_MatType(ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ v1, ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ v2, ಸ್ಥಿರಾಂಕ CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << " (expected: '" << ctx.p1_str << " " << getTestOpMath(ctx.testOp) << " " << ctx.p2_str << "'), where" << std::ಸಾಲು_ಅಂತ್ಯ
        << "    '" << ctx.p1_str << "' is " << v1 << " (" << type_ಗಮ್ಯ_String(v1) << ")" << std::ಸಾಲು_ಅಂತ್ಯ;
    ಆದರೆ (ctx.testOp != TEST_CUSTOM && ctx.testOp < CV_ಕೊನೆಯ_TEST_OP)
    {
        ss << "must be " << getTestOpPhraseStr(ctx.testOp) << std::ಸಾಲು_ಅಂತ್ಯ;
    }
    ss  << "    '" << ctx.p2_str << "' is " << v2 << " (" << type_ಗಮ್ಯ_String(v2) << ")";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.ಸಾಲು);
}
ಶೂನ್ಯ check_failed_MatChannels(ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ v1, ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ v2, ಸ್ಥಿರಾಂಕ CheckContext& ctx)
{
    check_failed_auto_<ಪೂರ್ಣಾಂಕ>(v1, v2, ctx);
}
ಶೂನ್ಯ check_failed_auto(ಸ್ಥಿರಾಂಕ ತಾರ್ಕಿಕ v1, ಸ್ಥಿರಾಂಕ ತಾರ್ಕಿಕ v2, ಸ್ಥಿರಾಂಕ CheckContext& ctx)
{
    check_failed_auto_<ತಾರ್ಕಿಕ>(v1, v2, ctx);
}
ಶೂನ್ಯ check_failed_auto(ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ v1, ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ v2, ಸ್ಥಿರಾಂಕ CheckContext& ctx)
{
    check_failed_auto_<ಪೂರ್ಣಾಂಕ>(v1, v2, ctx);
}
ಶೂನ್ಯ check_failed_auto(ಸ್ಥಿರಾಂಕ size_t v1, ಸ್ಥಿರಾಂಕ size_t v2, ಸ್ಥಿರಾಂಕ CheckContext& ctx)
{
    check_failed_auto_<size_t>(v1, v2, ctx);
}
ಶೂನ್ಯ check_failed_auto(ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ v1, ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ v2, ಸ್ಥಿರಾಂಕ CheckContext& ctx)
{
    check_failed_auto_<ದಶಮಾಂಶ>(v1, v2, ctx);
}
ಶೂನ್ಯ check_failed_auto(ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ v1, ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ v2, ಸ್ಥಿರಾಂಕ CheckContext& ctx)
{
    check_failed_auto_<ದ್ವಿಗುಣ>(v1, v2, ctx);
}
ಶೂನ್ಯ check_failed_auto(ಸ್ಥಿರಾಂಕ ಗಾತ್ರ_3<ಪೂರ್ಣಾಂಕ> v1, ಸ್ಥಿರಾಂಕ ಗಾತ್ರ_3<ಪೂರ್ಣಾಂಕ> v2, ಸ್ಥಿರಾಂಕ CheckContext& ctx)
{
    check_failed_auto_< ಗಾತ್ರ_3<ಪೂರ್ಣಾಂಕ> >(v1, v2, ctx);
}


ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T> ಸ್ಥಿರ CV_NORETURN
ಶೂನ್ಯ check_failed_auto_(ಸ್ಥಿರಾಂಕ T& v, ಸ್ಥಿರಾಂಕ CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << ":" << std::ಸಾಲು_ಅಂತ್ಯ
        << "    '" << ctx.p2_str << "'" << std::ಸಾಲು_ಅಂತ್ಯ
        << "where" << std::ಸಾಲು_ಅಂತ್ಯ
        << "    '" << ctx.p1_str << "' is " << v;
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.ಸಾಲು);
}
ಶೂನ್ಯ check_failed_Mat_ಆಳ(ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ v, ಸ್ಥಿರಾಂಕ CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << ":" << std::ಸಾಲು_ಅಂತ್ಯ
        << "    '" << ctx.p2_str << "'" << std::ಸಾಲು_ಅಂತ್ಯ
        << "where" << std::ಸಾಲು_ಅಂತ್ಯ
        << "    '" << ctx.p1_str << "' is " << v << " (" << ಆಳ_ಗಮ್ಯ_String(v) << ")";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.ಸಾಲು);
}
ಶೂನ್ಯ check_failed_MatType(ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ v, ಸ್ಥಿರಾಂಕ CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << ":" << std::ಸಾಲು_ಅಂತ್ಯ
        << "    '" << ctx.p2_str << "'" << std::ಸಾಲು_ಅಂತ್ಯ
        << "where" << std::ಸಾಲು_ಅಂತ್ಯ
        << "    '" << ctx.p1_str << "' is " << v << " (" << type_ಗಮ್ಯ_String(v) << ")";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.ಸಾಲು);
}
ಶೂನ್ಯ check_failed_MatChannels(ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ v, ಸ್ಥಿರಾಂಕ CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << ":" << std::ಸಾಲು_ಅಂತ್ಯ
        << "    '" << ctx.p2_str << "'" << std::ಸಾಲು_ಅಂತ್ಯ
        << "where" << std::ಸಾಲು_ಅಂತ್ಯ
        << "    '" << ctx.p1_str << "' is " << v;
    cv::error(cv::Error::Bad_ಅಂಕೆ_Channels, ss.str(), ctx.func, ctx.file, ctx.ಸಾಲು);
}
ಶೂನ್ಯ check_failed_true(ಸ್ಥಿರಾಂಕ ತಾರ್ಕಿಕ v, ಸ್ಥಿರಾಂಕ CheckContext& ctx)
{
    CV_ಬಳಸದ(v);
    std::stringstream ss;
    ss  << ctx.message << ":" << std::ಸಾಲು_ಅಂತ್ಯ
        << "    '" << ctx.p1_str << "' must be 'true'";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.ಸಾಲು);
}
ಶೂನ್ಯ check_failed_false(ಸ್ಥಿರಾಂಕ ತಾರ್ಕಿಕ v, ಸ್ಥಿರಾಂಕ CheckContext& ctx)
{
    CV_ಬಳಸದ(v);
    std::stringstream ss;
    ss  << ctx.message << ":" << std::ಸಾಲು_ಅಂತ್ಯ
        << "    '" << ctx.p1_str << "' must be 'false'";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.ಸಾಲು);
}
ಶೂನ್ಯ check_failed_auto(ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ v, ಸ್ಥಿರಾಂಕ CheckContext& ctx)
{
    check_failed_auto_<ಪೂರ್ಣಾಂಕ>(v, ctx);
}
ಶೂನ್ಯ check_failed_auto(ಸ್ಥಿರಾಂಕ size_t v, ಸ್ಥಿರಾಂಕ CheckContext& ctx)
{
    check_failed_auto_<size_t>(v, ctx);
}
ಶೂನ್ಯ check_failed_auto(ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ v, ಸ್ಥಿರಾಂಕ CheckContext& ctx)
{
    check_failed_auto_<ದಶಮಾಂಶ>(v, ctx);
}
ಶೂನ್ಯ check_failed_auto(ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ v, ಸ್ಥಿರಾಂಕ CheckContext& ctx)
{
    check_failed_auto_<ದ್ವಿಗುಣ>(v, ctx);
}
ಶೂನ್ಯ check_failed_auto(ಸ್ಥಿರಾಂಕ ಗಾತ್ರ_3<ಪೂರ್ಣಾಂಕ> v, ಸ್ಥಿರಾಂಕ CheckContext& ctx)
{
    check_failed_auto_< ಗಾತ್ರ_3<ಪೂರ್ಣಾಂಕ> >(v, ctx);
}
ಶೂನ್ಯ check_failed_auto(ಸ್ಥಿರಾಂಕ std::string& v, ಸ್ಥಿರಾಂಕ CheckContext& ctx)
{
    check_failed_auto_< std::string >(v, ctx);
}


}} // namespace
