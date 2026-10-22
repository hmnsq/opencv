// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ਅਨ੍ਤਰ੍ਭਾਵ "precomp.hpp"

#ਅਨ੍ਤਰ੍ਭਾਵ <sstream>

#ਅਨ੍ਤਰ੍ਭਾਵ "opencv2/core/check.hpp"

ਨਾਮਾਕਾਸ਼ cv {

ਅਚਲ ਅੱਖਰ* ਡੂੰਘਾਈ_ਮੰਜ਼ਿਲ_String(ਪੂਰਨ ਡੂੰਘਾਈ)
{
    ਅਚਲ ਅੱਖਰ* s = detail::ਡੂੰਘਾਈ_ਮੰਜ਼ਿਲ_String_2(ਡੂੰਘਾਈ);
    ਵਾਪਸ s ? s : "<invalid depth>";
}

cv::String type_ਮੰਜ਼ਿਲ_String(ਪੂਰਨ type)
{
    cv::String s = detail::type_ਮੰਜ਼ਿਲ_String_2(type);
    ਜੇ (s.empty())
    {
        ਸਥਿਰ cv::String invalidType("<invalid type>");
        ਵਾਪਸ invalidType;
    }
    ਵਾਪਸ s;
}


ਨਾਮਾਕਾਸ਼ detail {

ਸਥਿਰ ਅਚਲ ਅੱਖਰ* getTestOpPhraseStr(ਬਿਨਾਨਿਸ਼ਾਨੀ testOp)
{
    ਸਥਿਰ ਅਚਲ ਅੱਖਰ* ਨਾਂ[] = { "{custom check}", "equal to", "not equal to", "less than or equal to", "less than", "greater than or equal to", "greater than" };
    CV_DbgAssert(testOp < CV_ਆਖਰੀ_TEST_OP);
    ਵਾਪਸ testOp < CV_ਆਖਰੀ_TEST_OP ? ਨਾਂ[testOp] : "???";
}
ਸਥਿਰ ਅਚਲ ਅੱਖਰ* getTestOpMath(ਬਿਨਾਨਿਸ਼ਾਨੀ testOp)
{
    ਸਥਿਰ ਅਚਲ ਅੱਖਰ* ਨਾਂ[] = { "???", "==", "!=", "<=", "<", ">=", ">" };
    CV_DbgAssert(testOp < CV_ਆਖਰੀ_TEST_OP);
    ਵਾਪਸ testOp < CV_ਆਖਰੀ_TEST_OP ? ਨਾਂ[testOp] : "???";
}

ਅਚਲ ਅੱਖਰ* ਡੂੰਘਾਈ_ਮੰਜ਼ਿਲ_String_2(ਪੂਰਨ ਡੂੰਘਾਈ)
{
    ਸਥਿਰ ਅਚਲ ਅੱਖਰ* ਡੂੰਘਾਈ_ਨਾਂ[] = { "CV_8U", "CV_8S", "CV_16U", "CV_16S", "CV_32S", "CV_32F", "CV_64F", "CV_16F",
                                        "CV_16BF", "CV_Bool", "CV_64U", "CV_64S", "CV_32U",
                                        "CV_8F", "CV_8FNUZ" };
    ਵਾਪਸ (ਡੂੰਘਾਈ < CV_ਡੂੰਘਾਈ_CURR_MAX && ਡੂੰਘਾਈ >= 0) ? ਡੂੰਘਾਈ_ਨਾਂ[ਡੂੰਘਾਈ] : ਨਿਰਾ;
}

cv::String type_ਮੰਜ਼ਿਲ_String_2(ਪੂਰਨ type)
{
    ਪੂਰਨ ਡੂੰਘਾਈ = CV_MAT_ਡੂੰਘਾਈ(type);
    ਪੂਰਨ cn = CV_MAT_CN(type);
    ਜੇ (ਡੂੰਘਾਈ >= 0 && ਡੂੰਘਾਈ < CV_ਡੂੰਘਾਈ_CURR_MAX)
        ਵਾਪਸ cv::format("%sC%d", ਡੂੰਘਾਈ_ਮੰਜ਼ਿਲ_String_2(ਡੂੰਘਾਈ), cn);
    ਵਾਪਸ cv::String();
}

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T> ਸਥਿਰ CV_NORETURN
ਖਾਲੀ check_failed_auto_(ਅਚਲ T& v1, ਅਚਲ T& v2, ਅਚਲ CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << " (expected: '" << ctx.p1_str << " " << getTestOpMath(ctx.testOp) << " " << ctx.p2_str << "'), where" << std::ਸਤਰ_ਅੰਤ
        << "    '" << ctx.p1_str << "' is " << v1 << std::ਸਤਰ_ਅੰਤ;
    ਜੇ (ctx.testOp != TEST_CUSTOM && ctx.testOp < CV_ਆਖਰੀ_TEST_OP)
    {
        ss << "must be " << getTestOpPhraseStr(ctx.testOp) << std::ਸਤਰ_ਅੰਤ;
    }
    ss  << "    '" << ctx.p2_str << "' is " << v2;
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.ਸਤਰ);
}
ਖਾਲੀ check_failed_Mat_ਡੂੰਘਾਈ(ਅਚਲ ਪੂਰਨ v1, ਅਚਲ ਪੂਰਨ v2, ਅਚਲ CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << " (expected: '" << ctx.p1_str << " " << getTestOpMath(ctx.testOp) << " " << ctx.p2_str << "'), where" << std::ਸਤਰ_ਅੰਤ
        << "    '" << ctx.p1_str << "' is " << v1 << " (" << ਡੂੰਘਾਈ_ਮੰਜ਼ਿਲ_String(v1) << ")" << std::ਸਤਰ_ਅੰਤ;
    ਜੇ (ctx.testOp != TEST_CUSTOM && ctx.testOp < CV_ਆਖਰੀ_TEST_OP)
    {
        ss << "must be " << getTestOpPhraseStr(ctx.testOp) << std::ਸਤਰ_ਅੰਤ;
    }
    ss  << "    '" << ctx.p2_str << "' is " << v2 << " (" << ਡੂੰਘਾਈ_ਮੰਜ਼ਿਲ_String(v2) << ")";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.ਸਤਰ);
}
ਖਾਲੀ check_failed_MatType(ਅਚਲ ਪੂਰਨ v1, ਅਚਲ ਪੂਰਨ v2, ਅਚਲ CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << " (expected: '" << ctx.p1_str << " " << getTestOpMath(ctx.testOp) << " " << ctx.p2_str << "'), where" << std::ਸਤਰ_ਅੰਤ
        << "    '" << ctx.p1_str << "' is " << v1 << " (" << type_ਮੰਜ਼ਿਲ_String(v1) << ")" << std::ਸਤਰ_ਅੰਤ;
    ਜੇ (ctx.testOp != TEST_CUSTOM && ctx.testOp < CV_ਆਖਰੀ_TEST_OP)
    {
        ss << "must be " << getTestOpPhraseStr(ctx.testOp) << std::ਸਤਰ_ਅੰਤ;
    }
    ss  << "    '" << ctx.p2_str << "' is " << v2 << " (" << type_ਮੰਜ਼ਿਲ_String(v2) << ")";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.ਸਤਰ);
}
ਖਾਲੀ check_failed_MatChannels(ਅਚਲ ਪੂਰਨ v1, ਅਚਲ ਪੂਰਨ v2, ਅਚਲ CheckContext& ctx)
{
    check_failed_auto_<ਪੂਰਨ>(v1, v2, ctx);
}
ਖਾਲੀ check_failed_auto(ਅਚਲ ਤਾਰਕਿਕ v1, ਅਚਲ ਤਾਰਕਿਕ v2, ਅਚਲ CheckContext& ctx)
{
    check_failed_auto_<ਤਾਰਕਿਕ>(v1, v2, ctx);
}
ਖਾਲੀ check_failed_auto(ਅਚਲ ਪੂਰਨ v1, ਅਚਲ ਪੂਰਨ v2, ਅਚਲ CheckContext& ctx)
{
    check_failed_auto_<ਪੂਰਨ>(v1, v2, ctx);
}
ਖਾਲੀ check_failed_auto(ਅਚਲ size_t v1, ਅਚਲ size_t v2, ਅਚਲ CheckContext& ctx)
{
    check_failed_auto_<size_t>(v1, v2, ctx);
}
ਖਾਲੀ check_failed_auto(ਅਚਲ ਦਸ਼ਮਲਵ v1, ਅਚਲ ਦਸ਼ਮਲਵ v2, ਅਚਲ CheckContext& ctx)
{
    check_failed_auto_<ਦਸ਼ਮਲਵ>(v1, v2, ctx);
}
ਖਾਲੀ check_failed_auto(ਅਚਲ ਦੁੱਗਣਾ v1, ਅਚਲ ਦੁੱਗਣਾ v2, ਅਚਲ CheckContext& ctx)
{
    check_failed_auto_<ਦੁੱਗਣਾ>(v1, v2, ctx);
}
ਖਾਲੀ check_failed_auto(ਅਚਲ ਮਾਪ_2<ਪੂਰਨ> v1, ਅਚਲ ਮਾਪ_2<ਪੂਰਨ> v2, ਅਚਲ CheckContext& ctx)
{
    check_failed_auto_< ਮਾਪ_2<ਪੂਰਨ> >(v1, v2, ctx);
}


ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T> ਸਥਿਰ CV_NORETURN
ਖਾਲੀ check_failed_auto_(ਅਚਲ T& v, ਅਚਲ CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << ":" << std::ਸਤਰ_ਅੰਤ
        << "    '" << ctx.p2_str << "'" << std::ਸਤਰ_ਅੰਤ
        << "where" << std::ਸਤਰ_ਅੰਤ
        << "    '" << ctx.p1_str << "' is " << v;
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.ਸਤਰ);
}
ਖਾਲੀ check_failed_Mat_ਡੂੰਘਾਈ(ਅਚਲ ਪੂਰਨ v, ਅਚਲ CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << ":" << std::ਸਤਰ_ਅੰਤ
        << "    '" << ctx.p2_str << "'" << std::ਸਤਰ_ਅੰਤ
        << "where" << std::ਸਤਰ_ਅੰਤ
        << "    '" << ctx.p1_str << "' is " << v << " (" << ਡੂੰਘਾਈ_ਮੰਜ਼ਿਲ_String(v) << ")";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.ਸਤਰ);
}
ਖਾਲੀ check_failed_MatType(ਅਚਲ ਪੂਰਨ v, ਅਚਲ CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << ":" << std::ਸਤਰ_ਅੰਤ
        << "    '" << ctx.p2_str << "'" << std::ਸਤਰ_ਅੰਤ
        << "where" << std::ਸਤਰ_ਅੰਤ
        << "    '" << ctx.p1_str << "' is " << v << " (" << type_ਮੰਜ਼ਿਲ_String(v) << ")";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.ਸਤਰ);
}
ਖਾਲੀ check_failed_MatChannels(ਅਚਲ ਪੂਰਨ v, ਅਚਲ CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << ":" << std::ਸਤਰ_ਅੰਤ
        << "    '" << ctx.p2_str << "'" << std::ਸਤਰ_ਅੰਤ
        << "where" << std::ਸਤਰ_ਅੰਤ
        << "    '" << ctx.p1_str << "' is " << v;
    cv::error(cv::Error::Bad_ਅੰਕ_Channels, ss.str(), ctx.func, ctx.file, ctx.ਸਤਰ);
}
ਖਾਲੀ check_failed_true(ਅਚਲ ਤਾਰਕਿਕ v, ਅਚਲ CheckContext& ctx)
{
    CV_ਅਣਵਰਤਿਆ(v);
    std::stringstream ss;
    ss  << ctx.message << ":" << std::ਸਤਰ_ਅੰਤ
        << "    '" << ctx.p1_str << "' must be 'true'";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.ਸਤਰ);
}
ਖਾਲੀ check_failed_false(ਅਚਲ ਤਾਰਕਿਕ v, ਅਚਲ CheckContext& ctx)
{
    CV_ਅਣਵਰਤਿਆ(v);
    std::stringstream ss;
    ss  << ctx.message << ":" << std::ਸਤਰ_ਅੰਤ
        << "    '" << ctx.p1_str << "' must be 'false'";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.ਸਤਰ);
}
ਖਾਲੀ check_failed_auto(ਅਚਲ ਪੂਰਨ v, ਅਚਲ CheckContext& ctx)
{
    check_failed_auto_<ਪੂਰਨ>(v, ctx);
}
ਖਾਲੀ check_failed_auto(ਅਚਲ size_t v, ਅਚਲ CheckContext& ctx)
{
    check_failed_auto_<size_t>(v, ctx);
}
ਖਾਲੀ check_failed_auto(ਅਚਲ ਦਸ਼ਮਲਵ v, ਅਚਲ CheckContext& ctx)
{
    check_failed_auto_<ਦਸ਼ਮਲਵ>(v, ctx);
}
ਖਾਲੀ check_failed_auto(ਅਚਲ ਦੁੱਗਣਾ v, ਅਚਲ CheckContext& ctx)
{
    check_failed_auto_<ਦੁੱਗਣਾ>(v, ctx);
}
ਖਾਲੀ check_failed_auto(ਅਚਲ ਮਾਪ_2<ਪੂਰਨ> v, ਅਚਲ CheckContext& ctx)
{
    check_failed_auto_< ਮਾਪ_2<ਪੂਰਨ> >(v, ctx);
}
ਖਾਲੀ check_failed_auto(ਅਚਲ std::string& v, ਅਚਲ CheckContext& ctx)
{
    check_failed_auto_< std::string >(v, ctx);
}


}} // namespace
