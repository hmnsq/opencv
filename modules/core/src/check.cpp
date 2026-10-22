// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ضمن "precomp.hpp"

#ضمن <sstream>

#ضمن "opencv2/core/check.hpp"

نطاق cv {

ثابت حرف* depthToString(صحيح depth)
{
    ثابت حرف* s = detail::depthToString_(depth);
    ܦܢܐ s ? s : "<invalid depth>";
}

cv::String typeToString(صحيح type)
{
    cv::String s = detail::typeToString_(type);
    ܐܢ (s.empty())
    {
        ساكن cv::String invalidType("<invalid type>");
        ܦܢܐ invalidType;
    }
    ܦܢܐ s;
}


نطاق detail {

ساكن ثابت حرف* getTestOpPhraseStr(unsigned testOp)
{
    ساكن ثابت حرف* _names[] = { "{custom check}", "equal to", "not equal to", "less than or equal to", "less than", "greater than or equal to", "greater than" };
    CV_DbgAssert(testOp < CV__LAST_TEST_OP);
    ܦܢܐ testOp < CV__LAST_TEST_OP ? _names[testOp] : "???";
}
ساكن ثابت حرف* getTestOpMath(unsigned testOp)
{
    ساكن ثابت حرف* _names[] = { "???", "==", "!=", "<=", "<", ">=", ">" };
    CV_DbgAssert(testOp < CV__LAST_TEST_OP);
    ܦܢܐ testOp < CV__LAST_TEST_OP ? _names[testOp] : "???";
}

ثابت حرف* depthToString_(صحيح depth)
{
    ساكن ثابت حرف* depthNames[] = { "CV_8U", "CV_8S", "CV_16U", "CV_16S", "CV_32S", "CV_32F", "CV_64F", "CV_16F",
                                        "CV_16BF", "CV_Bool", "CV_64U", "CV_64S", "CV_32U",
                                        "CV_8F", "CV_8FNUZ" };
    ܦܢܐ (depth < CV_DEPTH_CURR_MAX && depth >= 0) ? depthNames[depth] : عدم;
}

cv::String typeToString_(صحيح type)
{
    صحيح depth = CV_MAT_DEPTH(type);
    صحيح cn = CV_MAT_CN(type);
    ܐܢ (depth >= 0 && depth < CV_DEPTH_CURR_MAX)
        ܦܢܐ cv::format("%sC%d", depthToString_(depth), cn);
    ܦܢܐ cv::String();
}

template<typename T> ساكن CV_NORETURN
فراغ check_failed_auto_(ثابت T& v1, ثابت T& v2, ثابت CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << " (expected: '" << ctx.p1_str << " " << getTestOpMath(ctx.testOp) << " " << ctx.p2_str << "'), where" << std::endl
        << "    '" << ctx.p1_str << "' is " << v1 << std::endl;
    ܐܢ (ctx.testOp != TEST_CUSTOM && ctx.testOp < CV__LAST_TEST_OP)
    {
        ss << "must be " << getTestOpPhraseStr(ctx.testOp) << std::endl;
    }
    ss  << "    '" << ctx.p2_str << "' is " << v2;
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.line);
}
فراغ check_failed_MatDepth(ثابت صحيح v1, ثابت صحيح v2, ثابت CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << " (expected: '" << ctx.p1_str << " " << getTestOpMath(ctx.testOp) << " " << ctx.p2_str << "'), where" << std::endl
        << "    '" << ctx.p1_str << "' is " << v1 << " (" << depthToString(v1) << ")" << std::endl;
    ܐܢ (ctx.testOp != TEST_CUSTOM && ctx.testOp < CV__LAST_TEST_OP)
    {
        ss << "must be " << getTestOpPhraseStr(ctx.testOp) << std::endl;
    }
    ss  << "    '" << ctx.p2_str << "' is " << v2 << " (" << depthToString(v2) << ")";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.line);
}
فراغ check_failed_MatType(ثابت صحيح v1, ثابت صحيح v2, ثابت CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << " (expected: '" << ctx.p1_str << " " << getTestOpMath(ctx.testOp) << " " << ctx.p2_str << "'), where" << std::endl
        << "    '" << ctx.p1_str << "' is " << v1 << " (" << typeToString(v1) << ")" << std::endl;
    ܐܢ (ctx.testOp != TEST_CUSTOM && ctx.testOp < CV__LAST_TEST_OP)
    {
        ss << "must be " << getTestOpPhraseStr(ctx.testOp) << std::endl;
    }
    ss  << "    '" << ctx.p2_str << "' is " << v2 << " (" << typeToString(v2) << ")";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.line);
}
فراغ check_failed_MatChannels(ثابت صحيح v1, ثابت صحيح v2, ثابت CheckContext& ctx)
{
    check_failed_auto_<صحيح>(v1, v2, ctx);
}
فراغ check_failed_auto(ثابت منطقي v1, ثابت منطقي v2, ثابت CheckContext& ctx)
{
    check_failed_auto_<منطقي>(v1, v2, ctx);
}
فراغ check_failed_auto(ثابت صحيح v1, ثابت صحيح v2, ثابت CheckContext& ctx)
{
    check_failed_auto_<صحيح>(v1, v2, ctx);
}
فراغ check_failed_auto(ثابت size_t v1, ثابت size_t v2, ثابت CheckContext& ctx)
{
    check_failed_auto_<size_t>(v1, v2, ctx);
}
فراغ check_failed_auto(ثابت عائم v1, ثابت عائم v2, ثابت CheckContext& ctx)
{
    check_failed_auto_<عائم>(v1, v2, ctx);
}
فراغ check_failed_auto(ثابت مزدوج v1, ثابت مزدوج v2, ثابت CheckContext& ctx)
{
    check_failed_auto_<مزدوج>(v1, v2, ctx);
}
فراغ check_failed_auto(ثابت Size_<صحيح> v1, ثابت Size_<صحيح> v2, ثابت CheckContext& ctx)
{
    check_failed_auto_< Size_<صحيح> >(v1, v2, ctx);
}


template<typename T> ساكن CV_NORETURN
فراغ check_failed_auto_(ثابت T& v, ثابت CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << ":" << std::endl
        << "    '" << ctx.p2_str << "'" << std::endl
        << "where" << std::endl
        << "    '" << ctx.p1_str << "' is " << v;
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.line);
}
فراغ check_failed_MatDepth(ثابت صحيح v, ثابت CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << ":" << std::endl
        << "    '" << ctx.p2_str << "'" << std::endl
        << "where" << std::endl
        << "    '" << ctx.p1_str << "' is " << v << " (" << depthToString(v) << ")";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.line);
}
فراغ check_failed_MatType(ثابت صحيح v, ثابت CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << ":" << std::endl
        << "    '" << ctx.p2_str << "'" << std::endl
        << "where" << std::endl
        << "    '" << ctx.p1_str << "' is " << v << " (" << typeToString(v) << ")";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.line);
}
فراغ check_failed_MatChannels(ثابت صحيح v, ثابت CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << ":" << std::endl
        << "    '" << ctx.p2_str << "'" << std::endl
        << "where" << std::endl
        << "    '" << ctx.p1_str << "' is " << v;
    cv::error(cv::Error::BadNumChannels, ss.str(), ctx.func, ctx.file, ctx.line);
}
فراغ check_failed_true(ثابت منطقي v, ثابت CheckContext& ctx)
{
    CV_UNUSED(v);
    std::stringstream ss;
    ss  << ctx.message << ":" << std::endl
        << "    '" << ctx.p1_str << "' must be 'true'";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.line);
}
فراغ check_failed_false(ثابت منطقي v, ثابت CheckContext& ctx)
{
    CV_UNUSED(v);
    std::stringstream ss;
    ss  << ctx.message << ":" << std::endl
        << "    '" << ctx.p1_str << "' must be 'false'";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.line);
}
فراغ check_failed_auto(ثابت صحيح v, ثابت CheckContext& ctx)
{
    check_failed_auto_<صحيح>(v, ctx);
}
فراغ check_failed_auto(ثابت size_t v, ثابت CheckContext& ctx)
{
    check_failed_auto_<size_t>(v, ctx);
}
فراغ check_failed_auto(ثابت عائم v, ثابت CheckContext& ctx)
{
    check_failed_auto_<عائم>(v, ctx);
}
فراغ check_failed_auto(ثابت مزدوج v, ثابت CheckContext& ctx)
{
    check_failed_auto_<مزدوج>(v, ctx);
}
فراغ check_failed_auto(ثابت Size_<صحيح> v, ثابت CheckContext& ctx)
{
    check_failed_auto_< Size_<صحيح> >(v, ctx);
}
فراغ check_failed_auto(ثابت std::string& v, ثابت CheckContext& ctx)
{
    check_failed_auto_< std::string >(v, ctx);
}


}} // namespace
