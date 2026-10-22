// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#अन्तर्भाव "precomp.hpp"

#अन्तर्भाव <sstream>

#अन्तर्भाव "opencv2/core/check.hpp"

नामाकाश cv {

ध्रुव वर्ण* depthToString(पूर्णाङ्क depth)
{
    ध्रुव वर्ण* s = detail::depthToString_(depth);
    पटिनिवत्त s ? s : "<invalid depth>";
}

cv::String typeToString(पूर्णाङ्क type)
{
    cv::String s = detail::typeToString_(type);
    सचे (s.empty())
    {
        स्थिर cv::String invalidType("<invalid type>");
        पटिनिवत्त invalidType;
    }
    पटिनिवत्त s;
}


नामाकाश detail {

स्थिर ध्रुव वर्ण* getTestOpPhraseStr(unsigned testOp)
{
    स्थिर ध्रुव वर्ण* _names[] = { "{custom check}", "equal to", "not equal to", "less than or equal to", "less than", "greater than or equal to", "greater than" };
    CV_DbgAssert(testOp < CV__LAST_TEST_OP);
    पटिनिवत्त testOp < CV__LAST_TEST_OP ? _names[testOp] : "???";
}
स्थिर ध्रुव वर्ण* getTestOpMath(unsigned testOp)
{
    स्थिर ध्रुव वर्ण* _names[] = { "???", "==", "!=", "<=", "<", ">=", ">" };
    CV_DbgAssert(testOp < CV__LAST_TEST_OP);
    पटिनिवत्त testOp < CV__LAST_TEST_OP ? _names[testOp] : "???";
}

ध्रुव वर्ण* depthToString_(पूर्णाङ्क depth)
{
    स्थिर ध्रुव वर्ण* depthNames[] = { "CV_8U", "CV_8S", "CV_16U", "CV_16S", "CV_32S", "CV_32F", "CV_64F", "CV_16F",
                                        "CV_16BF", "CV_Bool", "CV_64U", "CV_64S", "CV_32U",
                                        "CV_8F", "CV_8FNUZ" };
    पटिनिवत्त (depth < CV_DEPTH_CURR_MAX && depth >= 0) ? depthNames[depth] : रिक्त;
}

cv::String typeToString_(पूर्णाङ्क type)
{
    पूर्णाङ्क depth = CV_MAT_DEPTH(type);
    पूर्णाङ्क cn = CV_MAT_CN(type);
    सचे (depth >= 0 && depth < CV_DEPTH_CURR_MAX)
        पटिनिवत्त cv::format("%sC%d", depthToString_(depth), cn);
    पटिनिवत्त cv::String();
}

template<typename T> स्थिर CV_NORETURN
शून्य check_failed_auto_(ध्रुव T& v1, ध्रुव T& v2, ध्रुव CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << " (expected: '" << ctx.p1_str << " " << getTestOpMath(ctx.testOp) << " " << ctx.p2_str << "'), where" << std::endl
        << "    '" << ctx.p1_str << "' is " << v1 << std::endl;
    सचे (ctx.testOp != TEST_CUSTOM && ctx.testOp < CV__LAST_TEST_OP)
    {
        ss << "must be " << getTestOpPhraseStr(ctx.testOp) << std::endl;
    }
    ss  << "    '" << ctx.p2_str << "' is " << v2;
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.पङ्क्तिः);
}
शून्य check_failed_MatDepth(ध्रुव पूर्णाङ्क v1, ध्रुव पूर्णाङ्क v2, ध्रुव CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << " (expected: '" << ctx.p1_str << " " << getTestOpMath(ctx.testOp) << " " << ctx.p2_str << "'), where" << std::endl
        << "    '" << ctx.p1_str << "' is " << v1 << " (" << depthToString(v1) << ")" << std::endl;
    सचे (ctx.testOp != TEST_CUSTOM && ctx.testOp < CV__LAST_TEST_OP)
    {
        ss << "must be " << getTestOpPhraseStr(ctx.testOp) << std::endl;
    }
    ss  << "    '" << ctx.p2_str << "' is " << v2 << " (" << depthToString(v2) << ")";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.पङ्क्तिः);
}
शून्य check_failed_MatType(ध्रुव पूर्णाङ्क v1, ध्रुव पूर्णाङ्क v2, ध्रुव CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << " (expected: '" << ctx.p1_str << " " << getTestOpMath(ctx.testOp) << " " << ctx.p2_str << "'), where" << std::endl
        << "    '" << ctx.p1_str << "' is " << v1 << " (" << typeToString(v1) << ")" << std::endl;
    सचे (ctx.testOp != TEST_CUSTOM && ctx.testOp < CV__LAST_TEST_OP)
    {
        ss << "must be " << getTestOpPhraseStr(ctx.testOp) << std::endl;
    }
    ss  << "    '" << ctx.p2_str << "' is " << v2 << " (" << typeToString(v2) << ")";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.पङ्क्तिः);
}
शून्य check_failed_MatChannels(ध्रुव पूर्णाङ्क v1, ध्रुव पूर्णाङ्क v2, ध्रुव CheckContext& ctx)
{
    check_failed_auto_<पूर्णाङ्क>(v1, v2, ctx);
}
शून्य check_failed_auto(ध्रुव तार्किक v1, ध्रुव तार्किक v2, ध्रुव CheckContext& ctx)
{
    check_failed_auto_<तार्किक>(v1, v2, ctx);
}
शून्य check_failed_auto(ध्रुव पूर्णाङ्क v1, ध्रुव पूर्णाङ्क v2, ध्रुव CheckContext& ctx)
{
    check_failed_auto_<पूर्णाङ्क>(v1, v2, ctx);
}
शून्य check_failed_auto(ध्रुव size_t v1, ध्रुव size_t v2, ध्रुव CheckContext& ctx)
{
    check_failed_auto_<size_t>(v1, v2, ctx);
}
शून्य check_failed_auto(ध्रुव भिन्न v1, ध्रुव भिन्न v2, ध्रुव CheckContext& ctx)
{
    check_failed_auto_<भिन्न>(v1, v2, ctx);
}
शून्य check_failed_auto(ध्रुव द्विगुण v1, ध्रुव द्विगुण v2, ध्रुव CheckContext& ctx)
{
    check_failed_auto_<द्विगुण>(v1, v2, ctx);
}
शून्य check_failed_auto(ध्रुव Size_<पूर्णाङ्क> v1, ध्रुव Size_<पूर्णाङ्क> v2, ध्रुव CheckContext& ctx)
{
    check_failed_auto_< Size_<पूर्णाङ्क> >(v1, v2, ctx);
}


template<typename T> स्थिर CV_NORETURN
शून्य check_failed_auto_(ध्रुव T& v, ध्रुव CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << ":" << std::endl
        << "    '" << ctx.p2_str << "'" << std::endl
        << "where" << std::endl
        << "    '" << ctx.p1_str << "' is " << v;
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.पङ्क्तिः);
}
शून्य check_failed_MatDepth(ध्रुव पूर्णाङ्क v, ध्रुव CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << ":" << std::endl
        << "    '" << ctx.p2_str << "'" << std::endl
        << "where" << std::endl
        << "    '" << ctx.p1_str << "' is " << v << " (" << depthToString(v) << ")";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.पङ्क्तिः);
}
शून्य check_failed_MatType(ध्रुव पूर्णाङ्क v, ध्रुव CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << ":" << std::endl
        << "    '" << ctx.p2_str << "'" << std::endl
        << "where" << std::endl
        << "    '" << ctx.p1_str << "' is " << v << " (" << typeToString(v) << ")";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.पङ्क्तिः);
}
शून्य check_failed_MatChannels(ध्रुव पूर्णाङ्क v, ध्रुव CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << ":" << std::endl
        << "    '" << ctx.p2_str << "'" << std::endl
        << "where" << std::endl
        << "    '" << ctx.p1_str << "' is " << v;
    cv::error(cv::Error::Bad_अङ्कः_Channels, ss.str(), ctx.func, ctx.file, ctx.पङ्क्तिः);
}
शून्य check_failed_true(ध्रुव तार्किक v, ध्रुव CheckContext& ctx)
{
    CV_UNUSED(v);
    std::stringstream ss;
    ss  << ctx.message << ":" << std::endl
        << "    '" << ctx.p1_str << "' must be 'true'";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.पङ्क्तिः);
}
शून्य check_failed_false(ध्रुव तार्किक v, ध्रुव CheckContext& ctx)
{
    CV_UNUSED(v);
    std::stringstream ss;
    ss  << ctx.message << ":" << std::endl
        << "    '" << ctx.p1_str << "' must be 'false'";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.पङ्क्तिः);
}
शून्य check_failed_auto(ध्रुव पूर्णाङ्क v, ध्रुव CheckContext& ctx)
{
    check_failed_auto_<पूर्णाङ्क>(v, ctx);
}
शून्य check_failed_auto(ध्रुव size_t v, ध्रुव CheckContext& ctx)
{
    check_failed_auto_<size_t>(v, ctx);
}
शून्य check_failed_auto(ध्रुव भिन्न v, ध्रुव CheckContext& ctx)
{
    check_failed_auto_<भिन्न>(v, ctx);
}
शून्य check_failed_auto(ध्रुव द्विगुण v, ध्रुव CheckContext& ctx)
{
    check_failed_auto_<द्विगुण>(v, ctx);
}
शून्य check_failed_auto(ध्रुव Size_<पूर्णाङ्क> v, ध्रुव CheckContext& ctx)
{
    check_failed_auto_< Size_<पूर्णाङ्क> >(v, ctx);
}
शून्य check_failed_auto(ध्रुव std::string& v, ध्रुव CheckContext& ctx)
{
    check_failed_auto_< std::string >(v, ctx);
}


}} // namespace
