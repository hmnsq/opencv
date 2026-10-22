// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#समावेश "precomp.hpp"

#समावेश <sstream>

#समावेश "opencv2/core/check.hpp"

namespace cv {

स्थिर अक्षर* depthToString(पूर्णांक depth)
{
    स्थिर अक्षर* s = detail::depthToString_(depth);
    वापस s ? s : "<invalid depth>";
}

cv::String typeToString(पूर्णांक type)
{
    cv::String s = detail::typeToString_(type);
    अगर (s.empty())
    {
        अटल cv::String invalidType("<invalid type>");
        वापस invalidType;
    }
    वापस s;
}


namespace detail {

अटल स्थिर अक्षर* getTestOpPhraseStr(अचिन्हित testOp)
{
    अटल स्थिर अक्षर* _names[] = { "{custom check}", "equal to", "not equal to", "less than or equal to", "less than", "greater than or equal to", "greater than" };
    CV_DbgAssert(testOp < CV__LAST_TEST_OP);
    वापस testOp < CV__LAST_TEST_OP ? _names[testOp] : "???";
}
अटल स्थिर अक्षर* getTestOpMath(अचिन्हित testOp)
{
    अटल स्थिर अक्षर* _names[] = { "???", "==", "!=", "<=", "<", ">=", ">" };
    CV_DbgAssert(testOp < CV__LAST_TEST_OP);
    वापस testOp < CV__LAST_TEST_OP ? _names[testOp] : "???";
}

स्थिर अक्षर* depthToString_(पूर्णांक depth)
{
    अटल स्थिर अक्षर* depthNames[] = { "CV_8U", "CV_8S", "CV_16U", "CV_16S", "CV_32S", "CV_32F", "CV_64F", "CV_16F",
                                        "CV_16BF", "CV_Bool", "CV_64U", "CV_64S", "CV_32U",
                                        "CV_8F", "CV_8FNUZ" };
    वापस (depth < CV_DEPTH_CURR_MAX && depth >= 0) ? depthNames[depth] : शून्य;
}

cv::String typeToString_(पूर्णांक type)
{
    पूर्णांक depth = CV_MAT_DEPTH(type);
    पूर्णांक cn = CV_MAT_CN(type);
    अगर (depth >= 0 && depth < CV_DEPTH_CURR_MAX)
        वापस cv::format("%sC%d", depthToString_(depth), cn);
    वापस cv::String();
}

ढाँचा<typename T> अटल CV_NORETURN
व्योम check_failed_auto_(स्थिर T& v1, स्थिर T& v2, स्थिर CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << " (expected: '" << ctx.p1_str << " " << getTestOpMath(ctx.testOp) << " " << ctx.p2_str << "'), where" << std::प_पूर्ण
        << "    '" << ctx.p1_str << "' is " << v1 << std::प_पूर्ण;
    अगर (ctx.testOp != TEST_CUSTOM && ctx.testOp < CV__LAST_TEST_OP)
    {
        ss << "must be " << getTestOpPhraseStr(ctx.testOp) << std::प_पूर्ण;
    }
    ss  << "    '" << ctx.p2_str << "' is " << v2;
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.पंक्ति);
}
व्योम check_failed_MatDepth(स्थिर पूर्णांक v1, स्थिर पूर्णांक v2, स्थिर CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << " (expected: '" << ctx.p1_str << " " << getTestOpMath(ctx.testOp) << " " << ctx.p2_str << "'), where" << std::प_पूर्ण
        << "    '" << ctx.p1_str << "' is " << v1 << " (" << depthToString(v1) << ")" << std::प_पूर्ण;
    अगर (ctx.testOp != TEST_CUSTOM && ctx.testOp < CV__LAST_TEST_OP)
    {
        ss << "must be " << getTestOpPhraseStr(ctx.testOp) << std::प_पूर्ण;
    }
    ss  << "    '" << ctx.p2_str << "' is " << v2 << " (" << depthToString(v2) << ")";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.पंक्ति);
}
व्योम check_failed_MatType(स्थिर पूर्णांक v1, स्थिर पूर्णांक v2, स्थिर CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << " (expected: '" << ctx.p1_str << " " << getTestOpMath(ctx.testOp) << " " << ctx.p2_str << "'), where" << std::प_पूर्ण
        << "    '" << ctx.p1_str << "' is " << v1 << " (" << typeToString(v1) << ")" << std::प_पूर्ण;
    अगर (ctx.testOp != TEST_CUSTOM && ctx.testOp < CV__LAST_TEST_OP)
    {
        ss << "must be " << getTestOpPhraseStr(ctx.testOp) << std::प_पूर्ण;
    }
    ss  << "    '" << ctx.p2_str << "' is " << v2 << " (" << typeToString(v2) << ")";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.पंक्ति);
}
व्योम check_failed_MatChannels(स्थिर पूर्णांक v1, स्थिर पूर्णांक v2, स्थिर CheckContext& ctx)
{
    check_failed_auto_<पूर्णांक>(v1, v2, ctx);
}
व्योम check_failed_auto(स्थिर bool v1, स्थिर bool v2, स्थिर CheckContext& ctx)
{
    check_failed_auto_<bool>(v1, v2, ctx);
}
व्योम check_failed_auto(स्थिर पूर्णांक v1, स्थिर पूर्णांक v2, स्थिर CheckContext& ctx)
{
    check_failed_auto_<पूर्णांक>(v1, v2, ctx);
}
व्योम check_failed_auto(स्थिर माप_प्रकार v1, स्थिर माप_प्रकार v2, स्थिर CheckContext& ctx)
{
    check_failed_auto_<माप_प्रकार>(v1, v2, ctx);
}
व्योम check_failed_auto(स्थिर भग्न v1, स्थिर भग्न v2, स्थिर CheckContext& ctx)
{
    check_failed_auto_<भग्न>(v1, v2, ctx);
}
व्योम check_failed_auto(स्थिर द्विगुन v1, स्थिर द्विगुन v2, स्थिर CheckContext& ctx)
{
    check_failed_auto_<द्विगुन>(v1, v2, ctx);
}
व्योम check_failed_auto(स्थिर आकार_2<पूर्णांक> v1, स्थिर आकार_2<पूर्णांक> v2, स्थिर CheckContext& ctx)
{
    check_failed_auto_< आकार_2<पूर्णांक> >(v1, v2, ctx);
}


ढाँचा<typename T> अटल CV_NORETURN
व्योम check_failed_auto_(स्थिर T& v, स्थिर CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << ":" << std::प_पूर्ण
        << "    '" << ctx.p2_str << "'" << std::प_पूर्ण
        << "where" << std::प_पूर्ण
        << "    '" << ctx.p1_str << "' is " << v;
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.पंक्ति);
}
व्योम check_failed_MatDepth(स्थिर पूर्णांक v, स्थिर CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << ":" << std::प_पूर्ण
        << "    '" << ctx.p2_str << "'" << std::प_पूर्ण
        << "where" << std::प_पूर्ण
        << "    '" << ctx.p1_str << "' is " << v << " (" << depthToString(v) << ")";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.पंक्ति);
}
व्योम check_failed_MatType(स्थिर पूर्णांक v, स्थिर CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << ":" << std::प_पूर्ण
        << "    '" << ctx.p2_str << "'" << std::प_पूर्ण
        << "where" << std::प_पूर्ण
        << "    '" << ctx.p1_str << "' is " << v << " (" << typeToString(v) << ")";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.पंक्ति);
}
व्योम check_failed_MatChannels(स्थिर पूर्णांक v, स्थिर CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << ":" << std::प_पूर्ण
        << "    '" << ctx.p2_str << "'" << std::प_पूर्ण
        << "where" << std::प_पूर्ण
        << "    '" << ctx.p1_str << "' is " << v;
    cv::error(cv::Error::Bad_अंक_Channels, ss.str(), ctx.func, ctx.file, ctx.पंक्ति);
}
व्योम check_failed_true(स्थिर bool v, स्थिर CheckContext& ctx)
{
    CV_UNUSED(v);
    std::stringstream ss;
    ss  << ctx.message << ":" << std::प_पूर्ण
        << "    '" << ctx.p1_str << "' must be 'true'";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.पंक्ति);
}
व्योम check_failed_false(स्थिर bool v, स्थिर CheckContext& ctx)
{
    CV_UNUSED(v);
    std::stringstream ss;
    ss  << ctx.message << ":" << std::प_पूर्ण
        << "    '" << ctx.p1_str << "' must be 'false'";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.पंक्ति);
}
व्योम check_failed_auto(स्थिर पूर्णांक v, स्थिर CheckContext& ctx)
{
    check_failed_auto_<पूर्णांक>(v, ctx);
}
व्योम check_failed_auto(स्थिर माप_प्रकार v, स्थिर CheckContext& ctx)
{
    check_failed_auto_<माप_प्रकार>(v, ctx);
}
व्योम check_failed_auto(स्थिर भग्न v, स्थिर CheckContext& ctx)
{
    check_failed_auto_<भग्न>(v, ctx);
}
व्योम check_failed_auto(स्थिर द्विगुन v, स्थिर CheckContext& ctx)
{
    check_failed_auto_<द्विगुन>(v, ctx);
}
व्योम check_failed_auto(स्थिर आकार_2<पूर्णांक> v, स्थिर CheckContext& ctx)
{
    check_failed_auto_< आकार_2<पूर्णांक> >(v, ctx);
}
व्योम check_failed_auto(स्थिर std::string& v, स्थिर CheckContext& ctx)
{
    check_failed_auto_< std::string >(v, ctx);
}


}} // namespace
