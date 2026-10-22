// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ಅನ್ತರ್ಭಾವ "precomp.hpp"

#ಅನ್ತರ್ಭಾವ <sstream>

#ಅನ್ತರ್ಭಾವ "opencv2/core/check.hpp"

ನಾಮಾಕಾಶ cv {

ಧ್ರುವ ವರ್ಣ* depthToString(ಪೂರ್ಣಾಙ್ಕ depth)
{
    ಧ್ರುವ ವರ್ಣ* s = detail::depthToString_(depth);
    ಪ್ರತ್ಯಾಗಮ s ? s : "<invalid depth>";
}

cv::String typeToString(ಪೂರ್ಣಾಙ್ಕ type)
{
    cv::String s = detail::typeToString_(type);
    ಯದಿ (s.empty())
    {
        ಸ್ಥಿರ cv::String invalidType("<invalid type>");
        ಪ್ರತ್ಯಾಗಮ invalidType;
    }
    ಪ್ರತ್ಯಾಗಮ s;
}


ನಾಮಾಕಾಶ detail {

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ* getTestOpPhraseStr(unsigned testOp)
{
    ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ* _names[] = { "{custom check}", "equal to", "not equal to", "less than or equal to", "less than", "greater than or equal to", "greater than" };
    CV_DbgAssert(testOp < CV__LAST_TEST_OP);
    ಪ್ರತ್ಯಾಗಮ testOp < CV__LAST_TEST_OP ? _names[testOp] : "???";
}
ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ* getTestOpMath(unsigned testOp)
{
    ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ* _names[] = { "???", "==", "!=", "<=", "<", ">=", ">" };
    CV_DbgAssert(testOp < CV__LAST_TEST_OP);
    ಪ್ರತ್ಯಾಗಮ testOp < CV__LAST_TEST_OP ? _names[testOp] : "???";
}

ಧ್ರುವ ವರ್ಣ* depthToString_(ಪೂರ್ಣಾಙ್ಕ depth)
{
    ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ* depthNames[] = { "CV_8U", "CV_8S", "CV_16U", "CV_16S", "CV_32S", "CV_32F", "CV_64F", "CV_16F",
                                        "CV_16BF", "CV_Bool", "CV_64U", "CV_64S", "CV_32U",
                                        "CV_8F", "CV_8FNUZ" };
    ಪ್ರತ್ಯಾಗಮ (depth < CV_DEPTH_CURR_MAX && depth >= 0) ? depthNames[depth] : ರಿಕ್ತ;
}

cv::String typeToString_(ಪೂರ್ಣಾಙ್ಕ type)
{
    ಪೂರ್ಣಾಙ್ಕ depth = CV_MAT_DEPTH(type);
    ಪೂರ್ಣಾಙ್ಕ cn = CV_MAT_CN(type);
    ಯದಿ (depth >= 0 && depth < CV_DEPTH_CURR_MAX)
        ಪ್ರತ್ಯಾಗಮ cv::format("%sC%d", depthToString_(depth), cn);
    ಪ್ರತ್ಯಾಗಮ cv::String();
}

template<typename T> ಸ್ಥಿರ CV_NORETURN
ಶೂನ್ಯ check_failed_auto_(ಧ್ರುವ T& v1, ಧ್ರುವ T& v2, ಧ್ರುವ CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << " (expected: '" << ctx.p1_str << " " << getTestOpMath(ctx.testOp) << " " << ctx.p2_str << "'), where" << std::endl
        << "    '" << ctx.p1_str << "' is " << v1 << std::endl;
    ಯದಿ (ctx.testOp != TEST_CUSTOM && ctx.testOp < CV__LAST_TEST_OP)
    {
        ss << "must be " << getTestOpPhraseStr(ctx.testOp) << std::endl;
    }
    ss  << "    '" << ctx.p2_str << "' is " << v2;
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.ಸಾಲು);
}
ಶೂನ್ಯ check_failed_MatDepth(ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ v1, ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ v2, ಧ್ರುವ CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << " (expected: '" << ctx.p1_str << " " << getTestOpMath(ctx.testOp) << " " << ctx.p2_str << "'), where" << std::endl
        << "    '" << ctx.p1_str << "' is " << v1 << " (" << depthToString(v1) << ")" << std::endl;
    ಯದಿ (ctx.testOp != TEST_CUSTOM && ctx.testOp < CV__LAST_TEST_OP)
    {
        ss << "must be " << getTestOpPhraseStr(ctx.testOp) << std::endl;
    }
    ss  << "    '" << ctx.p2_str << "' is " << v2 << " (" << depthToString(v2) << ")";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.ಸಾಲು);
}
ಶೂನ್ಯ check_failed_MatType(ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ v1, ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ v2, ಧ್ರುವ CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << " (expected: '" << ctx.p1_str << " " << getTestOpMath(ctx.testOp) << " " << ctx.p2_str << "'), where" << std::endl
        << "    '" << ctx.p1_str << "' is " << v1 << " (" << typeToString(v1) << ")" << std::endl;
    ಯದಿ (ctx.testOp != TEST_CUSTOM && ctx.testOp < CV__LAST_TEST_OP)
    {
        ss << "must be " << getTestOpPhraseStr(ctx.testOp) << std::endl;
    }
    ss  << "    '" << ctx.p2_str << "' is " << v2 << " (" << typeToString(v2) << ")";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.ಸಾಲು);
}
ಶೂನ್ಯ check_failed_MatChannels(ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ v1, ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ v2, ಧ್ರುವ CheckContext& ctx)
{
    check_failed_auto_<ಪೂರ್ಣಾಙ್ಕ>(v1, v2, ctx);
}
ಶೂನ್ಯ check_failed_auto(ಧ್ರುವ ತಾರ್ಕಿಕ v1, ಧ್ರುವ ತಾರ್ಕಿಕ v2, ಧ್ರುವ CheckContext& ctx)
{
    check_failed_auto_<ತಾರ್ಕಿಕ>(v1, v2, ctx);
}
ಶೂನ್ಯ check_failed_auto(ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ v1, ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ v2, ಧ್ರುವ CheckContext& ctx)
{
    check_failed_auto_<ಪೂರ್ಣಾಙ್ಕ>(v1, v2, ctx);
}
ಶೂನ್ಯ check_failed_auto(ಧ್ರುವ size_t v1, ಧ್ರುವ size_t v2, ಧ್ರುವ CheckContext& ctx)
{
    check_failed_auto_<size_t>(v1, v2, ctx);
}
ಶೂನ್ಯ check_failed_auto(ಧ್ರುವ ಭಿನ್ನ v1, ಧ್ರುವ ಭಿನ್ನ v2, ಧ್ರುವ CheckContext& ctx)
{
    check_failed_auto_<ಭಿನ್ನ>(v1, v2, ctx);
}
ಶೂನ್ಯ check_failed_auto(ಧ್ರುವ ದ್ವಿಗುಣ v1, ಧ್ರುವ ದ್ವಿಗುಣ v2, ಧ್ರುವ CheckContext& ctx)
{
    check_failed_auto_<ದ್ವಿಗುಣ>(v1, v2, ctx);
}
ಶೂನ್ಯ check_failed_auto(ಧ್ರುವ Size_<ಪೂರ್ಣಾಙ್ಕ> v1, ಧ್ರುವ Size_<ಪೂರ್ಣಾಙ್ಕ> v2, ಧ್ರುವ CheckContext& ctx)
{
    check_failed_auto_< Size_<ಪೂರ್ಣಾಙ್ಕ> >(v1, v2, ctx);
}


template<typename T> ಸ್ಥಿರ CV_NORETURN
ಶೂನ್ಯ check_failed_auto_(ಧ್ರುವ T& v, ಧ್ರುವ CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << ":" << std::endl
        << "    '" << ctx.p2_str << "'" << std::endl
        << "where" << std::endl
        << "    '" << ctx.p1_str << "' is " << v;
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.ಸಾಲು);
}
ಶೂನ್ಯ check_failed_MatDepth(ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ v, ಧ್ರುವ CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << ":" << std::endl
        << "    '" << ctx.p2_str << "'" << std::endl
        << "where" << std::endl
        << "    '" << ctx.p1_str << "' is " << v << " (" << depthToString(v) << ")";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.ಸಾಲು);
}
ಶೂನ್ಯ check_failed_MatType(ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ v, ಧ್ರುವ CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << ":" << std::endl
        << "    '" << ctx.p2_str << "'" << std::endl
        << "where" << std::endl
        << "    '" << ctx.p1_str << "' is " << v << " (" << typeToString(v) << ")";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.ಸಾಲು);
}
ಶೂನ್ಯ check_failed_MatChannels(ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ v, ಧ್ರುವ CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << ":" << std::endl
        << "    '" << ctx.p2_str << "'" << std::endl
        << "where" << std::endl
        << "    '" << ctx.p1_str << "' is " << v;
    cv::error(cv::Error::Bad_ಅಂಕೆ_Channels, ss.str(), ctx.func, ctx.file, ctx.ಸಾಲು);
}
ಶೂನ್ಯ check_failed_true(ಧ್ರುವ ತಾರ್ಕಿಕ v, ಧ್ರುವ CheckContext& ctx)
{
    CV_UNUSED(v);
    std::stringstream ss;
    ss  << ctx.message << ":" << std::endl
        << "    '" << ctx.p1_str << "' must be 'true'";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.ಸಾಲು);
}
ಶೂನ್ಯ check_failed_false(ಧ್ರುವ ತಾರ್ಕಿಕ v, ಧ್ರುವ CheckContext& ctx)
{
    CV_UNUSED(v);
    std::stringstream ss;
    ss  << ctx.message << ":" << std::endl
        << "    '" << ctx.p1_str << "' must be 'false'";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.ಸಾಲು);
}
ಶೂನ್ಯ check_failed_auto(ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ v, ಧ್ರುವ CheckContext& ctx)
{
    check_failed_auto_<ಪೂರ್ಣಾಙ್ಕ>(v, ctx);
}
ಶೂನ್ಯ check_failed_auto(ಧ್ರುವ size_t v, ಧ್ರುವ CheckContext& ctx)
{
    check_failed_auto_<size_t>(v, ctx);
}
ಶೂನ್ಯ check_failed_auto(ಧ್ರುವ ಭಿನ್ನ v, ಧ್ರುವ CheckContext& ctx)
{
    check_failed_auto_<ಭಿನ್ನ>(v, ctx);
}
ಶೂನ್ಯ check_failed_auto(ಧ್ರುವ ದ್ವಿಗುಣ v, ಧ್ರುವ CheckContext& ctx)
{
    check_failed_auto_<ದ್ವಿಗುಣ>(v, ctx);
}
ಶೂನ್ಯ check_failed_auto(ಧ್ರುವ Size_<ಪೂರ್ಣಾಙ್ಕ> v, ಧ್ರುವ CheckContext& ctx)
{
    check_failed_auto_< Size_<ಪೂರ್ಣಾಙ್ಕ> >(v, ctx);
}
ಶೂನ್ಯ check_failed_auto(ಧ್ರುವ std::string& v, ಧ್ರುವ CheckContext& ctx)
{
    check_failed_auto_< std::string >(v, ctx);
}


}} // namespace
