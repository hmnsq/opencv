// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ضمن "precomp.hpp"

#ضمن <sstream>

#ضمن "opencv2/core/check.hpp"

نطاق cv {

ثابت حرف* ڈونگھائی_منزل_String(صحيح ڈونگھائی)
{
    ثابت حرف* s = detail::ڈونگھائی_منزل_String_2(ڈونگھائی);
    أرجع s ? s : "<invalid depth>";
}

cv::String type_منزل_String(صحيح type)
{
    cv::String s = detail::type_منزل_String_2(type);
    إذا (s.empty())
    {
        ساكن cv::String invalidType("<invalid type>");
        أرجع invalidType;
    }
    أرجع s;
}


نطاق detail {

ساكن ثابت حرف* getTestOpPhraseStr(بنانشانی testOp)
{
    ساكن ثابت حرف* ناں[] = { "{custom check}", "equal to", "not equal to", "less than or equal to", "less than", "greater than or equal to", "greater than" };
    CV_DbgAssert(testOp < CV_آخری_TEST_OP);
    أرجع testOp < CV_آخری_TEST_OP ? ناں[testOp] : "???";
}
ساكن ثابت حرف* getTestOpMath(بنانشانی testOp)
{
    ساكن ثابت حرف* ناں[] = { "???", "==", "!=", "<=", "<", ">=", ">" };
    CV_DbgAssert(testOp < CV_آخری_TEST_OP);
    أرجع testOp < CV_آخری_TEST_OP ? ناں[testOp] : "???";
}

ثابت حرف* ڈونگھائی_منزل_String_2(صحيح ڈونگھائی)
{
    ساكن ثابت حرف* ڈونگھائی_ناں[] = { "CV_8U", "CV_8S", "CV_16U", "CV_16S", "CV_32S", "CV_32F", "CV_64F", "CV_16F",
                                        "CV_16BF", "CV_Bool", "CV_64U", "CV_64S", "CV_32U",
                                        "CV_8F", "CV_8FNUZ" };
    أرجع (ڈونگھائی < CV_ڈونگھائی_CURR_MAX && ڈونگھائی >= 0) ? ڈونگھائی_ناں[ڈونگھائی] : عدم;
}

cv::String type_منزل_String_2(صحيح type)
{
    صحيح ڈونگھائی = CV_MAT_ڈونگھائی(type);
    صحيح cn = CV_MAT_CN(type);
    إذا (ڈونگھائی >= 0 && ڈونگھائی < CV_ڈونگھائی_CURR_MAX)
        أرجع cv::format("%sC%d", ڈونگھائی_منزل_String_2(ڈونگھائی), cn);
    أرجع cv::String();
}

سانچا<قسم_ناں T> ساكن CV_NORETURN
فراغ check_failed_auto_(ثابت T& v1, ثابت T& v2, ثابت CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << " (expected: '" << ctx.p1_str << " " << getTestOpMath(ctx.testOp) << " " << ctx.p2_str << "'), where" << std::سطر_انت
        << "    '" << ctx.p1_str << "' is " << v1 << std::سطر_انت;
    إذا (ctx.testOp != TEST_CUSTOM && ctx.testOp < CV_آخری_TEST_OP)
    {
        ss << "must be " << getTestOpPhraseStr(ctx.testOp) << std::سطر_انت;
    }
    ss  << "    '" << ctx.p2_str << "' is " << v2;
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.سطر);
}
فراغ check_failed_Mat_ڈونگھائی(ثابت صحيح v1, ثابت صحيح v2, ثابت CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << " (expected: '" << ctx.p1_str << " " << getTestOpMath(ctx.testOp) << " " << ctx.p2_str << "'), where" << std::سطر_انت
        << "    '" << ctx.p1_str << "' is " << v1 << " (" << ڈونگھائی_منزل_String(v1) << ")" << std::سطر_انت;
    إذا (ctx.testOp != TEST_CUSTOM && ctx.testOp < CV_آخری_TEST_OP)
    {
        ss << "must be " << getTestOpPhraseStr(ctx.testOp) << std::سطر_انت;
    }
    ss  << "    '" << ctx.p2_str << "' is " << v2 << " (" << ڈونگھائی_منزل_String(v2) << ")";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.سطر);
}
فراغ check_failed_MatType(ثابت صحيح v1, ثابت صحيح v2, ثابت CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << " (expected: '" << ctx.p1_str << " " << getTestOpMath(ctx.testOp) << " " << ctx.p2_str << "'), where" << std::سطر_انت
        << "    '" << ctx.p1_str << "' is " << v1 << " (" << type_منزل_String(v1) << ")" << std::سطر_انت;
    إذا (ctx.testOp != TEST_CUSTOM && ctx.testOp < CV_آخری_TEST_OP)
    {
        ss << "must be " << getTestOpPhraseStr(ctx.testOp) << std::سطر_انت;
    }
    ss  << "    '" << ctx.p2_str << "' is " << v2 << " (" << type_منزل_String(v2) << ")";
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
فراغ check_failed_auto(ثابت ناپ_2<صحيح> v1, ثابت ناپ_2<صحيح> v2, ثابت CheckContext& ctx)
{
    check_failed_auto_< ناپ_2<صحيح> >(v1, v2, ctx);
}


سانچا<قسم_ناں T> ساكن CV_NORETURN
فراغ check_failed_auto_(ثابت T& v, ثابت CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << ":" << std::سطر_انت
        << "    '" << ctx.p2_str << "'" << std::سطر_انت
        << "where" << std::سطر_انت
        << "    '" << ctx.p1_str << "' is " << v;
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.سطر);
}
فراغ check_failed_Mat_ڈونگھائی(ثابت صحيح v, ثابت CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << ":" << std::سطر_انت
        << "    '" << ctx.p2_str << "'" << std::سطر_انت
        << "where" << std::سطر_انت
        << "    '" << ctx.p1_str << "' is " << v << " (" << ڈونگھائی_منزل_String(v) << ")";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.سطر);
}
فراغ check_failed_MatType(ثابت صحيح v, ثابت CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << ":" << std::سطر_انت
        << "    '" << ctx.p2_str << "'" << std::سطر_انت
        << "where" << std::سطر_انت
        << "    '" << ctx.p1_str << "' is " << v << " (" << type_منزل_String(v) << ")";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.سطر);
}
فراغ check_failed_MatChannels(ثابت صحيح v, ثابت CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << ":" << std::سطر_انت
        << "    '" << ctx.p2_str << "'" << std::سطر_انت
        << "where" << std::سطر_انت
        << "    '" << ctx.p1_str << "' is " << v;
    cv::error(cv::Error::Bad_عدد_Channels, ss.str(), ctx.func, ctx.file, ctx.سطر);
}
فراغ check_failed_true(ثابت منطقي v, ثابت CheckContext& ctx)
{
    CV_بن_ورتیا(v);
    std::stringstream ss;
    ss  << ctx.message << ":" << std::سطر_انت
        << "    '" << ctx.p1_str << "' must be 'true'";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.سطر);
}
فراغ check_failed_false(ثابت منطقي v, ثابت CheckContext& ctx)
{
    CV_بن_ورتیا(v);
    std::stringstream ss;
    ss  << ctx.message << ":" << std::سطر_انت
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
فراغ check_failed_auto(ثابت ناپ_2<صحيح> v, ثابت CheckContext& ctx)
{
    check_failed_auto_< ناپ_2<صحيح> >(v, ctx);
}
فراغ check_failed_auto(ثابت std::string& v, ثابت CheckContext& ctx)
{
    check_failed_auto_< std::string >(v, ctx);
}


}} // namespace
