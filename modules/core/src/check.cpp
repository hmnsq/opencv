// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#अन्तर्भाव "precomp.hpp"

#अन्तर्भाव <sstream>

#अन्तर्भाव "opencv2/core/check.hpp"

नामाकाश cv {

ध्रुव वर्ण* गभीरता_गन्तव्यम्_String(पूर्णाङ्क गभीरता)
{
    ध्रुव वर्ण* s = detail::गभीरता_गन्तव्यम्_String_2(गभीरता);
    प्रत्यागम s ? s : "<invalid depth>";
}

cv::String type_गन्तव्यम्_String(पूर्णाङ्क type)
{
    cv::String s = detail::type_गन्तव्यम्_String_2(type);
    यदि (s.empty())
    {
        स्थिर cv::String invalidType("<invalid type>");
        प्रत्यागम invalidType;
    }
    प्रत्यागम s;
}


नामाकाश detail {

स्थिर ध्रुव वर्ण* getTestOpPhraseStr(अचिह्नित testOp)
{
    स्थिर ध्रुव वर्ण* नामानि[] = { "{custom check}", "equal to", "not equal to", "less than or equal to", "less than", "greater than or equal to", "greater than" };
    CV_DbgAssert(testOp < CV_चरमः_TEST_OP);
    प्रत्यागम testOp < CV_चरमः_TEST_OP ? नामानि[testOp] : "???";
}
स्थिर ध्रुव वर्ण* getTestOpMath(अचिह्नित testOp)
{
    स्थिर ध्रुव वर्ण* नामानि[] = { "???", "==", "!=", "<=", "<", ">=", ">" };
    CV_DbgAssert(testOp < CV_चरमः_TEST_OP);
    प्रत्यागम testOp < CV_चरमः_TEST_OP ? नामानि[testOp] : "???";
}

ध्रुव वर्ण* गभीरता_गन्तव्यम्_String_2(पूर्णाङ्क गभीरता)
{
    स्थिर ध्रुव वर्ण* गभीरता_नामानि[] = { "CV_8U", "CV_8S", "CV_16U", "CV_16S", "CV_32S", "CV_32F", "CV_64F", "CV_16F",
                                        "CV_16BF", "CV_Bool", "CV_64U", "CV_64S", "CV_32U",
                                        "CV_8F", "CV_8FNUZ" };
    प्रत्यागम (गभीरता < CV_गभीरता_CURR_MAX && गभीरता >= 0) ? गभीरता_नामानि[गभीरता] : रिक्त;
}

cv::String type_गन्तव्यम्_String_2(पूर्णाङ्क type)
{
    पूर्णाङ्क गभीरता = CV_MAT_गभीरता(type);
    पूर्णाङ्क cn = CV_MAT_CN(type);
    यदि (गभीरता >= 0 && गभीरता < CV_गभीरता_CURR_MAX)
        प्रत्यागम cv::format("%sC%d", गभीरता_गन्तव्यम्_String_2(गभीरता), cn);
    प्रत्यागम cv::String();
}

प्रतिरूपम्<प्रकारनाम T> स्थिर CV_NORETURN
शून्य check_failed_auto_(ध्रुव T& v1, ध्रुव T& v2, ध्रुव CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << " (expected: '" << ctx.p1_str << " " << getTestOpMath(ctx.testOp) << " " << ctx.p2_str << "'), where" << std::पङ्क्त्यन्तः
        << "    '" << ctx.p1_str << "' is " << v1 << std::पङ्क्त्यन्तः;
    यदि (ctx.testOp != TEST_CUSTOM && ctx.testOp < CV_चरमः_TEST_OP)
    {
        ss << "must be " << getTestOpPhraseStr(ctx.testOp) << std::पङ्क्त्यन्तः;
    }
    ss  << "    '" << ctx.p2_str << "' is " << v2;
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.पङ्क्तिः);
}
शून्य check_failed_Mat_गभीरता(ध्रुव पूर्णाङ्क v1, ध्रुव पूर्णाङ्क v2, ध्रुव CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << " (expected: '" << ctx.p1_str << " " << getTestOpMath(ctx.testOp) << " " << ctx.p2_str << "'), where" << std::पङ्क्त्यन्तः
        << "    '" << ctx.p1_str << "' is " << v1 << " (" << गभीरता_गन्तव्यम्_String(v1) << ")" << std::पङ्क्त्यन्तः;
    यदि (ctx.testOp != TEST_CUSTOM && ctx.testOp < CV_चरमः_TEST_OP)
    {
        ss << "must be " << getTestOpPhraseStr(ctx.testOp) << std::पङ्क्त्यन्तः;
    }
    ss  << "    '" << ctx.p2_str << "' is " << v2 << " (" << गभीरता_गन्तव्यम्_String(v2) << ")";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.पङ्क्तिः);
}
शून्य check_failed_MatType(ध्रुव पूर्णाङ्क v1, ध्रुव पूर्णाङ्क v2, ध्रुव CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << " (expected: '" << ctx.p1_str << " " << getTestOpMath(ctx.testOp) << " " << ctx.p2_str << "'), where" << std::पङ्क्त्यन्तः
        << "    '" << ctx.p1_str << "' is " << v1 << " (" << type_गन्तव्यम्_String(v1) << ")" << std::पङ्क्त्यन्तः;
    यदि (ctx.testOp != TEST_CUSTOM && ctx.testOp < CV_चरमः_TEST_OP)
    {
        ss << "must be " << getTestOpPhraseStr(ctx.testOp) << std::पङ्क्त्यन्तः;
    }
    ss  << "    '" << ctx.p2_str << "' is " << v2 << " (" << type_गन्तव्यम्_String(v2) << ")";
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
शून्य check_failed_auto(ध्रुव आकार_2<पूर्णाङ्क> v1, ध्रुव आकार_2<पूर्णाङ्क> v2, ध्रुव CheckContext& ctx)
{
    check_failed_auto_< आकार_2<पूर्णाङ्क> >(v1, v2, ctx);
}


प्रतिरूपम्<प्रकारनाम T> स्थिर CV_NORETURN
शून्य check_failed_auto_(ध्रुव T& v, ध्रुव CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << ":" << std::पङ्क्त्यन्तः
        << "    '" << ctx.p2_str << "'" << std::पङ्क्त्यन्तः
        << "where" << std::पङ्क्त्यन्तः
        << "    '" << ctx.p1_str << "' is " << v;
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.पङ्क्तिः);
}
शून्य check_failed_Mat_गभीरता(ध्रुव पूर्णाङ्क v, ध्रुव CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << ":" << std::पङ्क्त्यन्तः
        << "    '" << ctx.p2_str << "'" << std::पङ्क्त्यन्तः
        << "where" << std::पङ्क्त्यन्तः
        << "    '" << ctx.p1_str << "' is " << v << " (" << गभीरता_गन्तव्यम्_String(v) << ")";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.पङ्क्तिः);
}
शून्य check_failed_MatType(ध्रुव पूर्णाङ्क v, ध्रुव CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << ":" << std::पङ्क्त्यन्तः
        << "    '" << ctx.p2_str << "'" << std::पङ्क्त्यन्तः
        << "where" << std::पङ्क्त्यन्तः
        << "    '" << ctx.p1_str << "' is " << v << " (" << type_गन्तव्यम्_String(v) << ")";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.पङ्क्तिः);
}
शून्य check_failed_MatChannels(ध्रुव पूर्णाङ्क v, ध्रुव CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << ":" << std::पङ्क्त्यन्तः
        << "    '" << ctx.p2_str << "'" << std::पङ्क्त्यन्तः
        << "where" << std::पङ्क्त्यन्तः
        << "    '" << ctx.p1_str << "' is " << v;
    cv::error(cv::Error::Bad_अङ्कः_Channels, ss.str(), ctx.func, ctx.file, ctx.पङ्क्तिः);
}
शून्य check_failed_true(ध्रुव तार्किक v, ध्रुव CheckContext& ctx)
{
    CV_अप्रयुक्तम्(v);
    std::stringstream ss;
    ss  << ctx.message << ":" << std::पङ्क्त्यन्तः
        << "    '" << ctx.p1_str << "' must be 'true'";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.पङ्क्तिः);
}
शून्य check_failed_false(ध्रुव तार्किक v, ध्रुव CheckContext& ctx)
{
    CV_अप्रयुक्तम्(v);
    std::stringstream ss;
    ss  << ctx.message << ":" << std::पङ्क्त्यन्तः
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
शून्य check_failed_auto(ध्रुव आकार_2<पूर्णाङ्क> v, ध्रुव CheckContext& ctx)
{
    check_failed_auto_< आकार_2<पूर्णाङ्क> >(v, ctx);
}
शून्य check_failed_auto(ध्रुव std::string& v, ध्रुव CheckContext& ctx)
{
    check_failed_auto_< std::string >(v, ctx);
}


}} // namespace
