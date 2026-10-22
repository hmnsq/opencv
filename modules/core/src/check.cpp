// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ضمن "precomp.hpp"

#ضمن <sstream>

#ضمن "opencv2/core/check.hpp"

نطاق cv {

ثابت حرف* اونهائي_منزل_String(صحيح اونهائي)
{
    ثابت حرف* s = detail::اونهائي_منزل_String_2(اونهائي);
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

ساكن ثابت حرف* getTestOpPhraseStr(بنانشان testOp)
{
    ساكن ثابت حرف* نالا[] = { "{custom check}", "equal to", "not equal to", "less than or equal to", "less than", "greater than or equal to", "greater than" };
    CV_DbgAssert(testOp < CV_آخري_TEST_OP);
    أرجع testOp < CV_آخري_TEST_OP ? نالا[testOp] : "???";
}
ساكن ثابت حرف* getTestOpMath(بنانشان testOp)
{
    ساكن ثابت حرف* نالا[] = { "???", "==", "!=", "<=", "<", ">=", ">" };
    CV_DbgAssert(testOp < CV_آخري_TEST_OP);
    أرجع testOp < CV_آخري_TEST_OP ? نالا[testOp] : "???";
}

ثابت حرف* اونهائي_منزل_String_2(صحيح اونهائي)
{
    ساكن ثابت حرف* اونهائي_نالا[] = { "CV_8U", "CV_8S", "CV_16U", "CV_16S", "CV_32S", "CV_32F", "CV_64F", "CV_16F",
                                        "CV_16BF", "CV_Bool", "CV_64U", "CV_64S", "CV_32U",
                                        "CV_8F", "CV_8FNUZ" };
    أرجع (اونهائي < CV_اونهائي_CURR_MAX && اونهائي >= 0) ? اونهائي_نالا[اونهائي] : عدم;
}

cv::String type_منزل_String_2(صحيح type)
{
    صحيح اونهائي = CV_MAT_اونهائي(type);
    صحيح cn = CV_MAT_CN(type);
    إذا (اونهائي >= 0 && اونهائي < CV_اونهائي_CURR_MAX)
        أرجع cv::format("%sC%d", اونهائي_منزل_String_2(اونهائي), cn);
    أرجع cv::String();
}

سانچو<قسم_نالو T> ساكن CV_NORETURN
فراغ check_failed_auto_(ثابت T& v1, ثابت T& v2, ثابت CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << " (expected: '" << ctx.p1_str << " " << getTestOpMath(ctx.testOp) << " " << ctx.p2_str << "'), where" << std::سٽ_پڄاڻي
        << "    '" << ctx.p1_str << "' is " << v1 << std::سٽ_پڄاڻي;
    إذا (ctx.testOp != TEST_CUSTOM && ctx.testOp < CV_آخري_TEST_OP)
    {
        ss << "must be " << getTestOpPhraseStr(ctx.testOp) << std::سٽ_پڄاڻي;
    }
    ss  << "    '" << ctx.p2_str << "' is " << v2;
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.سٽ);
}
فراغ check_failed_Mat_اونهائي(ثابت صحيح v1, ثابت صحيح v2, ثابت CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << " (expected: '" << ctx.p1_str << " " << getTestOpMath(ctx.testOp) << " " << ctx.p2_str << "'), where" << std::سٽ_پڄاڻي
        << "    '" << ctx.p1_str << "' is " << v1 << " (" << اونهائي_منزل_String(v1) << ")" << std::سٽ_پڄاڻي;
    إذا (ctx.testOp != TEST_CUSTOM && ctx.testOp < CV_آخري_TEST_OP)
    {
        ss << "must be " << getTestOpPhraseStr(ctx.testOp) << std::سٽ_پڄاڻي;
    }
    ss  << "    '" << ctx.p2_str << "' is " << v2 << " (" << اونهائي_منزل_String(v2) << ")";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.سٽ);
}
فراغ check_failed_MatType(ثابت صحيح v1, ثابت صحيح v2, ثابت CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << " (expected: '" << ctx.p1_str << " " << getTestOpMath(ctx.testOp) << " " << ctx.p2_str << "'), where" << std::سٽ_پڄاڻي
        << "    '" << ctx.p1_str << "' is " << v1 << " (" << type_منزل_String(v1) << ")" << std::سٽ_پڄاڻي;
    إذا (ctx.testOp != TEST_CUSTOM && ctx.testOp < CV_آخري_TEST_OP)
    {
        ss << "must be " << getTestOpPhraseStr(ctx.testOp) << std::سٽ_پڄاڻي;
    }
    ss  << "    '" << ctx.p2_str << "' is " << v2 << " (" << type_منزل_String(v2) << ")";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.سٽ);
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
فراغ check_failed_auto(ثابت سائيز_2<صحيح> v1, ثابت سائيز_2<صحيح> v2, ثابت CheckContext& ctx)
{
    check_failed_auto_< سائيز_2<صحيح> >(v1, v2, ctx);
}


سانچو<قسم_نالو T> ساكن CV_NORETURN
فراغ check_failed_auto_(ثابت T& v, ثابت CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << ":" << std::سٽ_پڄاڻي
        << "    '" << ctx.p2_str << "'" << std::سٽ_پڄاڻي
        << "where" << std::سٽ_پڄاڻي
        << "    '" << ctx.p1_str << "' is " << v;
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.سٽ);
}
فراغ check_failed_Mat_اونهائي(ثابت صحيح v, ثابت CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << ":" << std::سٽ_پڄاڻي
        << "    '" << ctx.p2_str << "'" << std::سٽ_پڄاڻي
        << "where" << std::سٽ_پڄاڻي
        << "    '" << ctx.p1_str << "' is " << v << " (" << اونهائي_منزل_String(v) << ")";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.سٽ);
}
فراغ check_failed_MatType(ثابت صحيح v, ثابت CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << ":" << std::سٽ_پڄاڻي
        << "    '" << ctx.p2_str << "'" << std::سٽ_پڄاڻي
        << "where" << std::سٽ_پڄاڻي
        << "    '" << ctx.p1_str << "' is " << v << " (" << type_منزل_String(v) << ")";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.سٽ);
}
فراغ check_failed_MatChannels(ثابت صحيح v, ثابت CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << ":" << std::سٽ_پڄاڻي
        << "    '" << ctx.p2_str << "'" << std::سٽ_پڄاڻي
        << "where" << std::سٽ_پڄاڻي
        << "    '" << ctx.p1_str << "' is " << v;
    cv::error(cv::Error::Bad_انگ_Channels, ss.str(), ctx.func, ctx.file, ctx.سٽ);
}
فراغ check_failed_true(ثابت منطقي v, ثابت CheckContext& ctx)
{
    CV_اڻ_استعمال_ٿيل(v);
    std::stringstream ss;
    ss  << ctx.message << ":" << std::سٽ_پڄاڻي
        << "    '" << ctx.p1_str << "' must be 'true'";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.سٽ);
}
فراغ check_failed_false(ثابت منطقي v, ثابت CheckContext& ctx)
{
    CV_اڻ_استعمال_ٿيل(v);
    std::stringstream ss;
    ss  << ctx.message << ":" << std::سٽ_پڄاڻي
        << "    '" << ctx.p1_str << "' must be 'false'";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.سٽ);
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
فراغ check_failed_auto(ثابت سائيز_2<صحيح> v, ثابت CheckContext& ctx)
{
    check_failed_auto_< سائيز_2<صحيح> >(v, ctx);
}
فراغ check_failed_auto(ثابت std::string& v, ثابت CheckContext& ctx)
{
    check_failed_auto_< std::string >(v, ctx);
}


}} // namespace
