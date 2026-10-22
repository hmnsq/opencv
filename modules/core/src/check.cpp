// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ضمن "precomp.hpp"

#ضمن <sstream>

#ضمن "opencv2/core/check.hpp"

نطاق cv {

ثابت حرف* ژوروالی_منزل_String(صحيح ژوروالی)
{
    ثابت حرف* s = detail::ژوروالی_منزل_String_2(ژوروالی);
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

ساكن ثابت حرف* getTestOpPhraseStr(بینښه testOp)
{
    ساكن ثابت حرف* نومونه[] = { "{custom check}", "equal to", "not equal to", "less than or equal to", "less than", "greater than or equal to", "greater than" };
    CV_DbgAssert(testOp < CV_وروستی_TEST_OP);
    أرجع testOp < CV_وروستی_TEST_OP ? نومونه[testOp] : "???";
}
ساكن ثابت حرف* getTestOpMath(بینښه testOp)
{
    ساكن ثابت حرف* نومونه[] = { "???", "==", "!=", "<=", "<", ">=", ">" };
    CV_DbgAssert(testOp < CV_وروستی_TEST_OP);
    أرجع testOp < CV_وروستی_TEST_OP ? نومونه[testOp] : "???";
}

ثابت حرف* ژوروالی_منزل_String_2(صحيح ژوروالی)
{
    ساكن ثابت حرف* ژوروالی_نومونه[] = { "CV_8U", "CV_8S", "CV_16U", "CV_16S", "CV_32S", "CV_32F", "CV_64F", "CV_16F",
                                        "CV_16BF", "CV_Bool", "CV_64U", "CV_64S", "CV_32U",
                                        "CV_8F", "CV_8FNUZ" };
    أرجع (ژوروالی < CV_ژوروالی_CURR_MAX && ژوروالی >= 0) ? ژوروالی_نومونه[ژوروالی] : عدم;
}

cv::String type_منزل_String_2(صحيح type)
{
    صحيح ژوروالی = CV_MAT_ژوروالی(type);
    صحيح cn = CV_MAT_CN(type);
    إذا (ژوروالی >= 0 && ژوروالی < CV_ژوروالی_CURR_MAX)
        أرجع cv::format("%sC%d", ژوروالی_منزل_String_2(ژوروالی), cn);
    أرجع cv::String();
}

کينډۍ<ډول_نوم T> ساكن CV_NORETURN
فراغ check_failed_auto_(ثابت T& v1, ثابت T& v2, ثابت CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << " (expected: '" << ctx.p1_str << " " << getTestOpMath(ctx.testOp) << " " << ctx.p2_str << "'), where" << std::کرښه_پای
        << "    '" << ctx.p1_str << "' is " << v1 << std::کرښه_پای;
    إذا (ctx.testOp != TEST_CUSTOM && ctx.testOp < CV_وروستی_TEST_OP)
    {
        ss << "must be " << getTestOpPhraseStr(ctx.testOp) << std::کرښه_پای;
    }
    ss  << "    '" << ctx.p2_str << "' is " << v2;
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.کرښه);
}
فراغ check_failed_Mat_ژوروالی(ثابت صحيح v1, ثابت صحيح v2, ثابت CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << " (expected: '" << ctx.p1_str << " " << getTestOpMath(ctx.testOp) << " " << ctx.p2_str << "'), where" << std::کرښه_پای
        << "    '" << ctx.p1_str << "' is " << v1 << " (" << ژوروالی_منزل_String(v1) << ")" << std::کرښه_پای;
    إذا (ctx.testOp != TEST_CUSTOM && ctx.testOp < CV_وروستی_TEST_OP)
    {
        ss << "must be " << getTestOpPhraseStr(ctx.testOp) << std::کرښه_پای;
    }
    ss  << "    '" << ctx.p2_str << "' is " << v2 << " (" << ژوروالی_منزل_String(v2) << ")";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.کرښه);
}
فراغ check_failed_MatType(ثابت صحيح v1, ثابت صحيح v2, ثابت CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << " (expected: '" << ctx.p1_str << " " << getTestOpMath(ctx.testOp) << " " << ctx.p2_str << "'), where" << std::کرښه_پای
        << "    '" << ctx.p1_str << "' is " << v1 << " (" << type_منزل_String(v1) << ")" << std::کرښه_پای;
    إذا (ctx.testOp != TEST_CUSTOM && ctx.testOp < CV_وروستی_TEST_OP)
    {
        ss << "must be " << getTestOpPhraseStr(ctx.testOp) << std::کرښه_پای;
    }
    ss  << "    '" << ctx.p2_str << "' is " << v2 << " (" << type_منزل_String(v2) << ")";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.کرښه);
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


کينډۍ<ډول_نوم T> ساكن CV_NORETURN
فراغ check_failed_auto_(ثابت T& v, ثابت CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << ":" << std::کرښه_پای
        << "    '" << ctx.p2_str << "'" << std::کرښه_پای
        << "where" << std::کرښه_پای
        << "    '" << ctx.p1_str << "' is " << v;
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.کرښه);
}
فراغ check_failed_Mat_ژوروالی(ثابت صحيح v, ثابت CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << ":" << std::کرښه_پای
        << "    '" << ctx.p2_str << "'" << std::کرښه_پای
        << "where" << std::کرښه_پای
        << "    '" << ctx.p1_str << "' is " << v << " (" << ژوروالی_منزل_String(v) << ")";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.کرښه);
}
فراغ check_failed_MatType(ثابت صحيح v, ثابت CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << ":" << std::کرښه_پای
        << "    '" << ctx.p2_str << "'" << std::کرښه_پای
        << "where" << std::کرښه_پای
        << "    '" << ctx.p1_str << "' is " << v << " (" << type_منزل_String(v) << ")";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.کرښه);
}
فراغ check_failed_MatChannels(ثابت صحيح v, ثابت CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << ":" << std::کرښه_پای
        << "    '" << ctx.p2_str << "'" << std::کرښه_پای
        << "where" << std::کرښه_پای
        << "    '" << ctx.p1_str << "' is " << v;
    cv::error(cv::Error::Bad_عدد_Channels, ss.str(), ctx.func, ctx.file, ctx.کرښه);
}
فراغ check_failed_true(ثابت منطقي v, ثابت CheckContext& ctx)
{
    CV_ناکارول_شوی(v);
    std::stringstream ss;
    ss  << ctx.message << ":" << std::کرښه_پای
        << "    '" << ctx.p1_str << "' must be 'true'";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.کرښه);
}
فراغ check_failed_false(ثابت منطقي v, ثابت CheckContext& ctx)
{
    CV_ناکارول_شوی(v);
    std::stringstream ss;
    ss  << ctx.message << ":" << std::کرښه_پای
        << "    '" << ctx.p1_str << "' must be 'false'";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.کرښه);
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
