// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#अन्तर्भाव "precomp.hpp"

#अन्तर्भाव <sstream>

#अन्तर्भाव "opencv2/core/check.hpp"

नामाकाश cv {

अचर वर्ण* depthToString(पूर्णाङ्क depth)
{
    अचर वर्ण* s = detail::depthToString_(depth);
    फर्काऊ s ? s : "<invalid depth>";
}

cv::String typeToString(पूर्णाङ्क type)
{
    cv::String s = detail::typeToString_(type);
    यदि (s.empty())
    {
        स्थिर cv::String invalidType("<invalid type>");
        फर्काऊ invalidType;
    }
    फर्काऊ s;
}


नामाकाश detail {

स्थिर अचर वर्ण* getTestOpPhraseStr(अचिन्हित testOp)
{
    स्थिर अचर वर्ण* _names[] = { "{custom check}", "equal to", "not equal to", "less than or equal to", "less than", "greater than or equal to", "greater than" };
    CV_DbgAssert(testOp < CV__LAST_TEST_OP);
    फर्काऊ testOp < CV__LAST_TEST_OP ? _names[testOp] : "???";
}
स्थिर अचर वर्ण* getTestOpMath(अचिन्हित testOp)
{
    स्थिर अचर वर्ण* _names[] = { "???", "==", "!=", "<=", "<", ">=", ">" };
    CV_DbgAssert(testOp < CV__LAST_TEST_OP);
    फर्काऊ testOp < CV__LAST_TEST_OP ? _names[testOp] : "???";
}

अचर वर्ण* depthToString_(पूर्णाङ्क depth)
{
    स्थिर अचर वर्ण* depthNames[] = { "CV_8U", "CV_8S", "CV_16U", "CV_16S", "CV_32S", "CV_32F", "CV_64F", "CV_16F",
                                        "CV_16BF", "CV_Bool", "CV_64U", "CV_64S", "CV_32U",
                                        "CV_8F", "CV_8FNUZ" };
    फर्काऊ (depth < CV_DEPTH_CURR_MAX && depth >= 0) ? depthNames[depth] : रिक्त;
}

cv::String typeToString_(पूर्णाङ्क type)
{
    पूर्णाङ्क depth = CV_MAT_DEPTH(type);
    पूर्णाङ्क cn = CV_MAT_CN(type);
    यदि (depth >= 0 && depth < CV_DEPTH_CURR_MAX)
        फर्काऊ cv::format("%sC%d", depthToString_(depth), cn);
    फर्काऊ cv::String();
}

ढाँचा<typename T> स्थिर CV_NORETURN
शून्य check_failed_auto_(अचर T& v1, अचर T& v2, अचर CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << " (expected: '" << ctx.p1_str << " " << getTestOpMath(ctx.testOp) << " " << ctx.p2_str << "'), where" << std::endl
        << "    '" << ctx.p1_str << "' is " << v1 << std::endl;
    यदि (ctx.testOp != TEST_CUSTOM && ctx.testOp < CV__LAST_TEST_OP)
    {
        ss << "must be " << getTestOpPhraseStr(ctx.testOp) << std::endl;
    }
    ss  << "    '" << ctx.p2_str << "' is " << v2;
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.पङ्क्ति);
}
शून्य check_failed_MatDepth(अचर पूर्णाङ्क v1, अचर पूर्णाङ्क v2, अचर CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << " (expected: '" << ctx.p1_str << " " << getTestOpMath(ctx.testOp) << " " << ctx.p2_str << "'), where" << std::endl
        << "    '" << ctx.p1_str << "' is " << v1 << " (" << depthToString(v1) << ")" << std::endl;
    यदि (ctx.testOp != TEST_CUSTOM && ctx.testOp < CV__LAST_TEST_OP)
    {
        ss << "must be " << getTestOpPhraseStr(ctx.testOp) << std::endl;
    }
    ss  << "    '" << ctx.p2_str << "' is " << v2 << " (" << depthToString(v2) << ")";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.पङ्क्ति);
}
शून्य check_failed_MatType(अचर पूर्णाङ्क v1, अचर पूर्णाङ्क v2, अचर CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << " (expected: '" << ctx.p1_str << " " << getTestOpMath(ctx.testOp) << " " << ctx.p2_str << "'), where" << std::endl
        << "    '" << ctx.p1_str << "' is " << v1 << " (" << typeToString(v1) << ")" << std::endl;
    यदि (ctx.testOp != TEST_CUSTOM && ctx.testOp < CV__LAST_TEST_OP)
    {
        ss << "must be " << getTestOpPhraseStr(ctx.testOp) << std::endl;
    }
    ss  << "    '" << ctx.p2_str << "' is " << v2 << " (" << typeToString(v2) << ")";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.पङ्क्ति);
}
शून्य check_failed_MatChannels(अचर पूर्णाङ्क v1, अचर पूर्णाङ्क v2, अचर CheckContext& ctx)
{
    check_failed_auto_<पूर्णाङ्क>(v1, v2, ctx);
}
शून्य check_failed_auto(अचर तार्किक v1, अचर तार्किक v2, अचर CheckContext& ctx)
{
    check_failed_auto_<तार्किक>(v1, v2, ctx);
}
शून्य check_failed_auto(अचर पूर्णाङ्क v1, अचर पूर्णाङ्क v2, अचर CheckContext& ctx)
{
    check_failed_auto_<पूर्णाङ्क>(v1, v2, ctx);
}
शून्य check_failed_auto(अचर माप_प्रकार v1, अचर माप_प्रकार v2, अचर CheckContext& ctx)
{
    check_failed_auto_<माप_प्रकार>(v1, v2, ctx);
}
शून्य check_failed_auto(अचर भिन्न v1, अचर भिन्न v2, अचर CheckContext& ctx)
{
    check_failed_auto_<भिन्न>(v1, v2, ctx);
}
शून्य check_failed_auto(अचर द्विगुण v1, अचर द्विगुण v2, अचर CheckContext& ctx)
{
    check_failed_auto_<द्विगुण>(v1, v2, ctx);
}
शून्य check_failed_auto(अचर Size_<पूर्णाङ्क> v1, अचर Size_<पूर्णाङ्क> v2, अचर CheckContext& ctx)
{
    check_failed_auto_< Size_<पूर्णाङ्क> >(v1, v2, ctx);
}


ढाँचा<typename T> स्थिर CV_NORETURN
शून्य check_failed_auto_(अचर T& v, अचर CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << ":" << std::endl
        << "    '" << ctx.p2_str << "'" << std::endl
        << "where" << std::endl
        << "    '" << ctx.p1_str << "' is " << v;
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.पङ्क्ति);
}
शून्य check_failed_MatDepth(अचर पूर्णाङ्क v, अचर CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << ":" << std::endl
        << "    '" << ctx.p2_str << "'" << std::endl
        << "where" << std::endl
        << "    '" << ctx.p1_str << "' is " << v << " (" << depthToString(v) << ")";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.पङ्क्ति);
}
शून्य check_failed_MatType(अचर पूर्णाङ्क v, अचर CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << ":" << std::endl
        << "    '" << ctx.p2_str << "'" << std::endl
        << "where" << std::endl
        << "    '" << ctx.p1_str << "' is " << v << " (" << typeToString(v) << ")";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.पङ्क्ति);
}
शून्य check_failed_MatChannels(अचर पूर्णाङ्क v, अचर CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << ":" << std::endl
        << "    '" << ctx.p2_str << "'" << std::endl
        << "where" << std::endl
        << "    '" << ctx.p1_str << "' is " << v;
    cv::error(cv::Error::Bad_अङ्क_Channels, ss.str(), ctx.func, ctx.file, ctx.पङ्क्ति);
}
शून्य check_failed_true(अचर तार्किक v, अचर CheckContext& ctx)
{
    CV_UNUSED(v);
    std::stringstream ss;
    ss  << ctx.message << ":" << std::endl
        << "    '" << ctx.p1_str << "' must be 'true'";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.पङ्क्ति);
}
शून्य check_failed_false(अचर तार्किक v, अचर CheckContext& ctx)
{
    CV_UNUSED(v);
    std::stringstream ss;
    ss  << ctx.message << ":" << std::endl
        << "    '" << ctx.p1_str << "' must be 'false'";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.पङ्क्ति);
}
शून्य check_failed_auto(अचर पूर्णाङ्क v, अचर CheckContext& ctx)
{
    check_failed_auto_<पूर्णाङ्क>(v, ctx);
}
शून्य check_failed_auto(अचर माप_प्रकार v, अचर CheckContext& ctx)
{
    check_failed_auto_<माप_प्रकार>(v, ctx);
}
शून्य check_failed_auto(अचर भिन्न v, अचर CheckContext& ctx)
{
    check_failed_auto_<भिन्न>(v, ctx);
}
शून्य check_failed_auto(अचर द्विगुण v, अचर CheckContext& ctx)
{
    check_failed_auto_<द्विगुण>(v, ctx);
}
शून्य check_failed_auto(अचर Size_<पूर्णाङ्क> v, अचर CheckContext& ctx)
{
    check_failed_auto_< Size_<पूर्णाङ्क> >(v, ctx);
}
शून्य check_failed_auto(अचर std::string& v, अचर CheckContext& ctx)
{
    check_failed_auto_< std::string >(v, ctx);
}


}} // namespace
