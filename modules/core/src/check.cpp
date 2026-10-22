// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ضمن "precomp.hpp"

#ضمن <sstream>

#ضمن "opencv2/core/check.hpp"

نطاق cv {

ثابت حرف* عمق_مقصد_String(صحيح عمق)
{
    ثابت حرف* s = detail::عمق_مقصد_String_2(عمق);
    أرجع s ? s : "<invalid depth>";
}

cv::String type_مقصد_String(صحيح type)
{
    cv::String s = detail::type_مقصد_String_2(type);
    إذا (s.empty())
    {
        ساكن cv::String invalidType("<invalid type>");
        أرجع invalidType;
    }
    أرجع s;
}


نطاق detail {

ساكن ثابت حرف* getTestOpPhraseStr(بیعلامت testOp)
{
    ساكن ثابت حرف* نامها[] = { "{custom check}", "equal to", "not equal to", "less than or equal to", "less than", "greater than or equal to", "greater than" };
    CV_DbgAssert(testOp < CV_آخرین_TEST_OP);
    أرجع testOp < CV_آخرین_TEST_OP ? نامها[testOp] : "???";
}
ساكن ثابت حرف* getTestOpMath(بیعلامت testOp)
{
    ساكن ثابت حرف* نامها[] = { "???", "==", "!=", "<=", "<", ">=", ">" };
    CV_DbgAssert(testOp < CV_آخرین_TEST_OP);
    أرجع testOp < CV_آخرین_TEST_OP ? نامها[testOp] : "???";
}

ثابت حرف* عمق_مقصد_String_2(صحيح عمق)
{
    ساكن ثابت حرف* عمق_نامها[] = { "CV_8U", "CV_8S", "CV_16U", "CV_16S", "CV_32S", "CV_32F", "CV_64F", "CV_16F",
                                        "CV_16BF", "CV_Bool", "CV_64U", "CV_64S", "CV_32U",
                                        "CV_8F", "CV_8FNUZ" };
    أرجع (عمق < CV_عمق_CURR_MAX && عمق >= 0) ? عمق_نامها[عمق] : عدم;
}

cv::String type_مقصد_String_2(صحيح type)
{
    صحيح عمق = CV_MAT_عمق(type);
    صحيح cn = CV_MAT_CN(type);
    إذا (عمق >= 0 && عمق < CV_عمق_CURR_MAX)
        أرجع cv::format("%sC%d", عمق_مقصد_String_2(عمق), cn);
    أرجع cv::String();
}

قالب<نام_نوع T> ساكن CV_NORETURN
فراغ check_failed_auto_(ثابت T& v1, ثابت T& v2, ثابت CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << " (expected: '" << ctx.p1_str << " " << getTestOpMath(ctx.testOp) << " " << ctx.p2_str << "'), where" << std::پایان_سطر
        << "    '" << ctx.p1_str << "' is " << v1 << std::پایان_سطر;
    إذا (ctx.testOp != TEST_CUSTOM && ctx.testOp < CV_آخرین_TEST_OP)
    {
        ss << "must be " << getTestOpPhraseStr(ctx.testOp) << std::پایان_سطر;
    }
    ss  << "    '" << ctx.p2_str << "' is " << v2;
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.سطر);
}
فراغ check_failed_Mat_عمق(ثابت صحيح v1, ثابت صحيح v2, ثابت CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << " (expected: '" << ctx.p1_str << " " << getTestOpMath(ctx.testOp) << " " << ctx.p2_str << "'), where" << std::پایان_سطر
        << "    '" << ctx.p1_str << "' is " << v1 << " (" << عمق_مقصد_String(v1) << ")" << std::پایان_سطر;
    إذا (ctx.testOp != TEST_CUSTOM && ctx.testOp < CV_آخرین_TEST_OP)
    {
        ss << "must be " << getTestOpPhraseStr(ctx.testOp) << std::پایان_سطر;
    }
    ss  << "    '" << ctx.p2_str << "' is " << v2 << " (" << عمق_مقصد_String(v2) << ")";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.سطر);
}
فراغ check_failed_MatType(ثابت صحيح v1, ثابت صحيح v2, ثابت CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << " (expected: '" << ctx.p1_str << " " << getTestOpMath(ctx.testOp) << " " << ctx.p2_str << "'), where" << std::پایان_سطر
        << "    '" << ctx.p1_str << "' is " << v1 << " (" << type_مقصد_String(v1) << ")" << std::پایان_سطر;
    إذا (ctx.testOp != TEST_CUSTOM && ctx.testOp < CV_آخرین_TEST_OP)
    {
        ss << "must be " << getTestOpPhraseStr(ctx.testOp) << std::پایان_سطر;
    }
    ss  << "    '" << ctx.p2_str << "' is " << v2 << " (" << type_مقصد_String(v2) << ")";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.سطر);
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
فراغ check_failed_auto(ثابت اندازه_2<صحيح> v1, ثابت اندازه_2<صحيح> v2, ثابت CheckContext& ctx)
{
    check_failed_auto_< اندازه_2<صحيح> >(v1, v2, ctx);
}


قالب<نام_نوع T> ساكن CV_NORETURN
فراغ check_failed_auto_(ثابت T& v, ثابت CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << ":" << std::پایان_سطر
        << "    '" << ctx.p2_str << "'" << std::پایان_سطر
        << "where" << std::پایان_سطر
        << "    '" << ctx.p1_str << "' is " << v;
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.سطر);
}
فراغ check_failed_Mat_عمق(ثابت صحيح v, ثابت CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << ":" << std::پایان_سطر
        << "    '" << ctx.p2_str << "'" << std::پایان_سطر
        << "where" << std::پایان_سطر
        << "    '" << ctx.p1_str << "' is " << v << " (" << عمق_مقصد_String(v) << ")";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.سطر);
}
فراغ check_failed_MatType(ثابت صحيح v, ثابت CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << ":" << std::پایان_سطر
        << "    '" << ctx.p2_str << "'" << std::پایان_سطر
        << "where" << std::پایان_سطر
        << "    '" << ctx.p1_str << "' is " << v << " (" << type_مقصد_String(v) << ")";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.سطر);
}
فراغ check_failed_MatChannels(ثابت صحيح v, ثابت CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << ":" << std::پایان_سطر
        << "    '" << ctx.p2_str << "'" << std::پایان_سطر
        << "where" << std::پایان_سطر
        << "    '" << ctx.p1_str << "' is " << v;
    cv::error(cv::Error::Bad_رقم_Channels, ss.str(), ctx.func, ctx.file, ctx.سطر);
}
فراغ check_failed_true(ثابت منطقي v, ثابت CheckContext& ctx)
{
    CV_استفادهنشده(v);
    std::stringstream ss;
    ss  << ctx.message << ":" << std::پایان_سطر
        << "    '" << ctx.p1_str << "' must be 'true'";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.سطر);
}
فراغ check_failed_false(ثابت منطقي v, ثابت CheckContext& ctx)
{
    CV_استفادهنشده(v);
    std::stringstream ss;
    ss  << ctx.message << ":" << std::پایان_سطر
        << "    '" << ctx.p1_str << "' must be 'false'";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.سطر);
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
فراغ check_failed_auto(ثابت اندازه_2<صحيح> v, ثابت CheckContext& ctx)
{
    check_failed_auto_< اندازه_2<صحيح> >(v, ctx);
}
فراغ check_failed_auto(ثابت std::string& v, ثابت CheckContext& ctx)
{
    check_failed_auto_< std::string >(v, ctx);
}


}} // namespace
