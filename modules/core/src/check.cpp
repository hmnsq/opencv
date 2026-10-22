// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#شامل "precomp.hpp"

#شامل <sstream>

#شامل "opencv2/core/check.hpp"

فضاینام cv {

ثابت نویسه* depthToString(صحیح depth)
{
    ثابت نویسه* s = detail::depthToString_(depth);
    بازگشت s ? s : "<invalid depth>";
}

cv::String typeToString(صحیح type)
{
    cv::String s = detail::typeToString_(type);
    اگر (s.empty())
    {
        ایستا cv::String invalidType("<invalid type>");
        بازگشت invalidType;
    }
    بازگشت s;
}


فضاینام detail {

ایستا ثابت نویسه* getTestOpPhraseStr(unsigned testOp)
{
    ایستا ثابت نویسه* _names[] = { "{custom check}", "equal to", "not equal to", "less than or equal to", "less than", "greater than or equal to", "greater than" };
    CV_DbgAssert(testOp < CV__LAST_TEST_OP);
    بازگشت testOp < CV__LAST_TEST_OP ? _names[testOp] : "???";
}
ایستا ثابت نویسه* getTestOpMath(unsigned testOp)
{
    ایستا ثابت نویسه* _names[] = { "???", "==", "!=", "<=", "<", ">=", ">" };
    CV_DbgAssert(testOp < CV__LAST_TEST_OP);
    بازگشت testOp < CV__LAST_TEST_OP ? _names[testOp] : "???";
}

ثابت نویسه* depthToString_(صحیح depth)
{
    ایستا ثابت نویسه* depthNames[] = { "CV_8U", "CV_8S", "CV_16U", "CV_16S", "CV_32S", "CV_32F", "CV_64F", "CV_16F",
                                        "CV_16BF", "CV_Bool", "CV_64U", "CV_64S", "CV_32U",
                                        "CV_8F", "CV_8FNUZ" };
    بازگشت (depth < CV_DEPTH_CURR_MAX && depth >= 0) ? depthNames[depth] : پوچ;
}

cv::String typeToString_(صحیح type)
{
    صحیح depth = CV_MAT_DEPTH(type);
    صحیح cn = CV_MAT_CN(type);
    اگر (depth >= 0 && depth < CV_DEPTH_CURR_MAX)
        بازگشت cv::format("%sC%d", depthToString_(depth), cn);
    بازگشت cv::String();
}

template<typename T> ایستا CV_NORETURN
تهی check_failed_auto_(ثابت T& v1, ثابت T& v2, ثابت CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << " (expected: '" << ctx.p1_str << " " << getTestOpMath(ctx.testOp) << " " << ctx.p2_str << "'), where" << std::endl
        << "    '" << ctx.p1_str << "' is " << v1 << std::endl;
    اگر (ctx.testOp != TEST_CUSTOM && ctx.testOp < CV__LAST_TEST_OP)
    {
        ss << "must be " << getTestOpPhraseStr(ctx.testOp) << std::endl;
    }
    ss  << "    '" << ctx.p2_str << "' is " << v2;
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.سطر);
}
تهی check_failed_MatDepth(ثابت صحیح v1, ثابت صحیح v2, ثابت CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << " (expected: '" << ctx.p1_str << " " << getTestOpMath(ctx.testOp) << " " << ctx.p2_str << "'), where" << std::endl
        << "    '" << ctx.p1_str << "' is " << v1 << " (" << depthToString(v1) << ")" << std::endl;
    اگر (ctx.testOp != TEST_CUSTOM && ctx.testOp < CV__LAST_TEST_OP)
    {
        ss << "must be " << getTestOpPhraseStr(ctx.testOp) << std::endl;
    }
    ss  << "    '" << ctx.p2_str << "' is " << v2 << " (" << depthToString(v2) << ")";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.سطر);
}
تهی check_failed_MatType(ثابت صحیح v1, ثابت صحیح v2, ثابت CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << " (expected: '" << ctx.p1_str << " " << getTestOpMath(ctx.testOp) << " " << ctx.p2_str << "'), where" << std::endl
        << "    '" << ctx.p1_str << "' is " << v1 << " (" << typeToString(v1) << ")" << std::endl;
    اگر (ctx.testOp != TEST_CUSTOM && ctx.testOp < CV__LAST_TEST_OP)
    {
        ss << "must be " << getTestOpPhraseStr(ctx.testOp) << std::endl;
    }
    ss  << "    '" << ctx.p2_str << "' is " << v2 << " (" << typeToString(v2) << ")";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.سطر);
}
تهی check_failed_MatChannels(ثابت صحیح v1, ثابت صحیح v2, ثابت CheckContext& ctx)
{
    check_failed_auto_<صحیح>(v1, v2, ctx);
}
تهی check_failed_auto(ثابت منطقی v1, ثابت منطقی v2, ثابت CheckContext& ctx)
{
    check_failed_auto_<منطقی>(v1, v2, ctx);
}
تهی check_failed_auto(ثابت صحیح v1, ثابت صحیح v2, ثابت CheckContext& ctx)
{
    check_failed_auto_<صحیح>(v1, v2, ctx);
}
تهی check_failed_auto(ثابت size_t v1, ثابت size_t v2, ثابت CheckContext& ctx)
{
    check_failed_auto_<size_t>(v1, v2, ctx);
}
تهی check_failed_auto(ثابت اعشاری v1, ثابت اعشاری v2, ثابت CheckContext& ctx)
{
    check_failed_auto_<اعشاری>(v1, v2, ctx);
}
تهی check_failed_auto(ثابت دوگانه v1, ثابت دوگانه v2, ثابت CheckContext& ctx)
{
    check_failed_auto_<دوگانه>(v1, v2, ctx);
}
تهی check_failed_auto(ثابت Size_<صحیح> v1, ثابت Size_<صحیح> v2, ثابت CheckContext& ctx)
{
    check_failed_auto_< Size_<صحیح> >(v1, v2, ctx);
}


template<typename T> ایستا CV_NORETURN
تهی check_failed_auto_(ثابت T& v, ثابت CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << ":" << std::endl
        << "    '" << ctx.p2_str << "'" << std::endl
        << "where" << std::endl
        << "    '" << ctx.p1_str << "' is " << v;
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.سطر);
}
تهی check_failed_MatDepth(ثابت صحیح v, ثابت CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << ":" << std::endl
        << "    '" << ctx.p2_str << "'" << std::endl
        << "where" << std::endl
        << "    '" << ctx.p1_str << "' is " << v << " (" << depthToString(v) << ")";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.سطر);
}
تهی check_failed_MatType(ثابت صحیح v, ثابت CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << ":" << std::endl
        << "    '" << ctx.p2_str << "'" << std::endl
        << "where" << std::endl
        << "    '" << ctx.p1_str << "' is " << v << " (" << typeToString(v) << ")";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.سطر);
}
تهی check_failed_MatChannels(ثابت صحیح v, ثابت CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << ":" << std::endl
        << "    '" << ctx.p2_str << "'" << std::endl
        << "where" << std::endl
        << "    '" << ctx.p1_str << "' is " << v;
    cv::error(cv::Error::Bad_رقم_Channels, ss.str(), ctx.func, ctx.file, ctx.سطر);
}
تهی check_failed_true(ثابت منطقی v, ثابت CheckContext& ctx)
{
    CV_UNUSED(v);
    std::stringstream ss;
    ss  << ctx.message << ":" << std::endl
        << "    '" << ctx.p1_str << "' must be 'true'";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.سطر);
}
تهی check_failed_false(ثابت منطقی v, ثابت CheckContext& ctx)
{
    CV_UNUSED(v);
    std::stringstream ss;
    ss  << ctx.message << ":" << std::endl
        << "    '" << ctx.p1_str << "' must be 'false'";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.سطر);
}
تهی check_failed_auto(ثابت صحیح v, ثابت CheckContext& ctx)
{
    check_failed_auto_<صحیح>(v, ctx);
}
تهی check_failed_auto(ثابت size_t v, ثابت CheckContext& ctx)
{
    check_failed_auto_<size_t>(v, ctx);
}
تهی check_failed_auto(ثابت اعشاری v, ثابت CheckContext& ctx)
{
    check_failed_auto_<اعشاری>(v, ctx);
}
تهی check_failed_auto(ثابت دوگانه v, ثابت CheckContext& ctx)
{
    check_failed_auto_<دوگانه>(v, ctx);
}
تهی check_failed_auto(ثابت Size_<صحیح> v, ثابت CheckContext& ctx)
{
    check_failed_auto_< Size_<صحیح> >(v, ctx);
}
تهی check_failed_auto(ثابت std::string& v, ثابت CheckContext& ctx)
{
    check_failed_auto_< std::string >(v, ctx);
}


}} // namespace
