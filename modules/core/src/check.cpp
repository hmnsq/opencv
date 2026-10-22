// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#അന്തര്ഭാവ "precomp.hpp"

#അന്തര്ഭാവ <sstream>

#അന്തര്ഭാവ "opencv2/core/check.hpp"

നാമാകാശ cv {

സ്ഥിരം അക്ഷരം* depthToString(പൂർണ്ണസംഖ്യ depth)
{
    സ്ഥിരം അക്ഷരം* s = detail::depthToString_(depth);
    തിരികെ s ? s : "<invalid depth>";
}

cv::String typeToString(പൂർണ്ണസംഖ്യ type)
{
    cv::String s = detail::typeToString_(type);
    എങ്കിൽ (s.empty())
    {
        സ്ഥിതം cv::String invalidType("<invalid type>");
        തിരികെ invalidType;
    }
    തിരികെ s;
}


നാമാകാശ detail {

സ്ഥിതം സ്ഥിരം അക്ഷരം* getTestOpPhraseStr(unsigned testOp)
{
    സ്ഥിതം സ്ഥിരം അക്ഷരം* _names[] = { "{custom check}", "equal to", "not equal to", "less than or equal to", "less than", "greater than or equal to", "greater than" };
    CV_DbgAssert(testOp < CV__LAST_TEST_OP);
    തിരികെ testOp < CV__LAST_TEST_OP ? _names[testOp] : "???";
}
സ്ഥിതം സ്ഥിരം അക്ഷരം* getTestOpMath(unsigned testOp)
{
    സ്ഥിതം സ്ഥിരം അക്ഷരം* _names[] = { "???", "==", "!=", "<=", "<", ">=", ">" };
    CV_DbgAssert(testOp < CV__LAST_TEST_OP);
    തിരികെ testOp < CV__LAST_TEST_OP ? _names[testOp] : "???";
}

സ്ഥിരം അക്ഷരം* depthToString_(പൂർണ്ണസംഖ്യ depth)
{
    സ്ഥിതം സ്ഥിരം അക്ഷരം* depthNames[] = { "CV_8U", "CV_8S", "CV_16U", "CV_16S", "CV_32S", "CV_32F", "CV_64F", "CV_16F",
                                        "CV_16BF", "CV_Bool", "CV_64U", "CV_64S", "CV_32U",
                                        "CV_8F", "CV_8FNUZ" };
    തിരികെ (depth < CV_DEPTH_CURR_MAX && depth >= 0) ? depthNames[depth] : ശൂന്യ;
}

cv::String typeToString_(പൂർണ്ണസംഖ്യ type)
{
    പൂർണ്ണസംഖ്യ depth = CV_MAT_DEPTH(type);
    പൂർണ്ണസംഖ്യ cn = CV_MAT_CN(type);
    എങ്കിൽ (depth >= 0 && depth < CV_DEPTH_CURR_MAX)
        തിരികെ cv::format("%sC%d", depthToString_(depth), cn);
    തിരികെ cv::String();
}

template<typename T> സ്ഥിതം CV_NORETURN
ശൂന്യം check_failed_auto_(സ്ഥിരം T& v1, സ്ഥിരം T& v2, സ്ഥിരം CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << " (expected: '" << ctx.p1_str << " " << getTestOpMath(ctx.testOp) << " " << ctx.p2_str << "'), where" << std::endl
        << "    '" << ctx.p1_str << "' is " << v1 << std::endl;
    എങ്കിൽ (ctx.testOp != TEST_CUSTOM && ctx.testOp < CV__LAST_TEST_OP)
    {
        ss << "must be " << getTestOpPhraseStr(ctx.testOp) << std::endl;
    }
    ss  << "    '" << ctx.p2_str << "' is " << v2;
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.വരി);
}
ശൂന്യം check_failed_MatDepth(സ്ഥിരം പൂർണ്ണസംഖ്യ v1, സ്ഥിരം പൂർണ്ണസംഖ്യ v2, സ്ഥിരം CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << " (expected: '" << ctx.p1_str << " " << getTestOpMath(ctx.testOp) << " " << ctx.p2_str << "'), where" << std::endl
        << "    '" << ctx.p1_str << "' is " << v1 << " (" << depthToString(v1) << ")" << std::endl;
    എങ്കിൽ (ctx.testOp != TEST_CUSTOM && ctx.testOp < CV__LAST_TEST_OP)
    {
        ss << "must be " << getTestOpPhraseStr(ctx.testOp) << std::endl;
    }
    ss  << "    '" << ctx.p2_str << "' is " << v2 << " (" << depthToString(v2) << ")";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.വരി);
}
ശൂന്യം check_failed_MatType(സ്ഥിരം പൂർണ്ണസംഖ്യ v1, സ്ഥിരം പൂർണ്ണസംഖ്യ v2, സ്ഥിരം CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << " (expected: '" << ctx.p1_str << " " << getTestOpMath(ctx.testOp) << " " << ctx.p2_str << "'), where" << std::endl
        << "    '" << ctx.p1_str << "' is " << v1 << " (" << typeToString(v1) << ")" << std::endl;
    എങ്കിൽ (ctx.testOp != TEST_CUSTOM && ctx.testOp < CV__LAST_TEST_OP)
    {
        ss << "must be " << getTestOpPhraseStr(ctx.testOp) << std::endl;
    }
    ss  << "    '" << ctx.p2_str << "' is " << v2 << " (" << typeToString(v2) << ")";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.വരി);
}
ശൂന്യം check_failed_MatChannels(സ്ഥിരം പൂർണ്ണസംഖ്യ v1, സ്ഥിരം പൂർണ്ണസംഖ്യ v2, സ്ഥിരം CheckContext& ctx)
{
    check_failed_auto_<പൂർണ്ണസംഖ്യ>(v1, v2, ctx);
}
ശൂന്യം check_failed_auto(സ്ഥിരം തർക്കം v1, സ്ഥിരം തർക്കം v2, സ്ഥിരം CheckContext& ctx)
{
    check_failed_auto_<തർക്കം>(v1, v2, ctx);
}
ശൂന്യം check_failed_auto(സ്ഥിരം പൂർണ്ണസംഖ്യ v1, സ്ഥിരം പൂർണ്ണസംഖ്യ v2, സ്ഥിരം CheckContext& ctx)
{
    check_failed_auto_<പൂർണ്ണസംഖ്യ>(v1, v2, ctx);
}
ശൂന്യം check_failed_auto(സ്ഥിരം size_t v1, സ്ഥിരം size_t v2, സ്ഥിരം CheckContext& ctx)
{
    check_failed_auto_<size_t>(v1, v2, ctx);
}
ശൂന്യം check_failed_auto(സ്ഥിരം ദശാംശം v1, സ്ഥിരം ദശാംശം v2, സ്ഥിരം CheckContext& ctx)
{
    check_failed_auto_<ദശാംശം>(v1, v2, ctx);
}
ശൂന്യം check_failed_auto(സ്ഥിരം ഇരട്ട v1, സ്ഥിരം ഇരട്ട v2, സ്ഥിരം CheckContext& ctx)
{
    check_failed_auto_<ഇരട്ട>(v1, v2, ctx);
}
ശൂന്യം check_failed_auto(സ്ഥിരം Size_<പൂർണ്ണസംഖ്യ> v1, സ്ഥിരം Size_<പൂർണ്ണസംഖ്യ> v2, സ്ഥിരം CheckContext& ctx)
{
    check_failed_auto_< Size_<പൂർണ്ണസംഖ്യ> >(v1, v2, ctx);
}


template<typename T> സ്ഥിതം CV_NORETURN
ശൂന്യം check_failed_auto_(സ്ഥിരം T& v, സ്ഥിരം CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << ":" << std::endl
        << "    '" << ctx.p2_str << "'" << std::endl
        << "where" << std::endl
        << "    '" << ctx.p1_str << "' is " << v;
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.വരി);
}
ശൂന്യം check_failed_MatDepth(സ്ഥിരം പൂർണ്ണസംഖ്യ v, സ്ഥിരം CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << ":" << std::endl
        << "    '" << ctx.p2_str << "'" << std::endl
        << "where" << std::endl
        << "    '" << ctx.p1_str << "' is " << v << " (" << depthToString(v) << ")";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.വരി);
}
ശൂന്യം check_failed_MatType(സ്ഥിരം പൂർണ്ണസംഖ്യ v, സ്ഥിരം CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << ":" << std::endl
        << "    '" << ctx.p2_str << "'" << std::endl
        << "where" << std::endl
        << "    '" << ctx.p1_str << "' is " << v << " (" << typeToString(v) << ")";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.വരി);
}
ശൂന്യം check_failed_MatChannels(സ്ഥിരം പൂർണ്ണസംഖ്യ v, സ്ഥിരം CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << ":" << std::endl
        << "    '" << ctx.p2_str << "'" << std::endl
        << "where" << std::endl
        << "    '" << ctx.p1_str << "' is " << v;
    cv::error(cv::Error::Bad_അക്കം_Channels, ss.str(), ctx.func, ctx.file, ctx.വരി);
}
ശൂന്യം check_failed_true(സ്ഥിരം തർക്കം v, സ്ഥിരം CheckContext& ctx)
{
    CV_UNUSED(v);
    std::stringstream ss;
    ss  << ctx.message << ":" << std::endl
        << "    '" << ctx.p1_str << "' must be 'true'";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.വരി);
}
ശൂന്യം check_failed_false(സ്ഥിരം തർക്കം v, സ്ഥിരം CheckContext& ctx)
{
    CV_UNUSED(v);
    std::stringstream ss;
    ss  << ctx.message << ":" << std::endl
        << "    '" << ctx.p1_str << "' must be 'false'";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.വരി);
}
ശൂന്യം check_failed_auto(സ്ഥിരം പൂർണ്ണസംഖ്യ v, സ്ഥിരം CheckContext& ctx)
{
    check_failed_auto_<പൂർണ്ണസംഖ്യ>(v, ctx);
}
ശൂന്യം check_failed_auto(സ്ഥിരം size_t v, സ്ഥിരം CheckContext& ctx)
{
    check_failed_auto_<size_t>(v, ctx);
}
ശൂന്യം check_failed_auto(സ്ഥിരം ദശാംശം v, സ്ഥിരം CheckContext& ctx)
{
    check_failed_auto_<ദശാംശം>(v, ctx);
}
ശൂന്യം check_failed_auto(സ്ഥിരം ഇരട്ട v, സ്ഥിരം CheckContext& ctx)
{
    check_failed_auto_<ഇരട്ട>(v, ctx);
}
ശൂന്യം check_failed_auto(സ്ഥിരം Size_<പൂർണ്ണസംഖ്യ> v, സ്ഥിരം CheckContext& ctx)
{
    check_failed_auto_< Size_<പൂർണ്ണസംഖ്യ> >(v, ctx);
}
ശൂന്യം check_failed_auto(സ്ഥിരം std::string& v, സ്ഥിരം CheckContext& ctx)
{
    check_failed_auto_< std::string >(v, ctx);
}


}} // namespace
