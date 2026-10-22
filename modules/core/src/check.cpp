// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#अन्तर्भाव "precomp.hpp"

#अन्तर्भाव <sstream>

#अन्तर्भाव "opencv2/core/check.hpp"

नामाकाश cv {

स्थिरांक अक्षर* depthToString(पूर्णांक depth)
{
    स्थिरांक अक्षर* s = detail::depthToString_(depth);
    परत s ? s : "<invalid depth>";
}

cv::String typeToString(पूर्णांक type)
{
    cv::String s = detail::typeToString_(type);
    जर (s.empty())
    {
        स्थिर cv::String invalidType("<invalid type>");
        परत invalidType;
    }
    परत s;
}


नामाकाश detail {

स्थिर स्थिरांक अक्षर* getTestOpPhraseStr(अचिन्हित testOp)
{
    स्थिर स्थिरांक अक्षर* _names[] = { "{custom check}", "equal to", "not equal to", "less than or equal to", "less than", "greater than or equal to", "greater than" };
    CV_DbgAssert(testOp < CV__LAST_TEST_OP);
    परत testOp < CV__LAST_TEST_OP ? _names[testOp] : "???";
}
स्थिर स्थिरांक अक्षर* getTestOpMath(अचिन्हित testOp)
{
    स्थिर स्थिरांक अक्षर* _names[] = { "???", "==", "!=", "<=", "<", ">=", ">" };
    CV_DbgAssert(testOp < CV__LAST_TEST_OP);
    परत testOp < CV__LAST_TEST_OP ? _names[testOp] : "???";
}

स्थिरांक अक्षर* depthToString_(पूर्णांक depth)
{
    स्थिर स्थिरांक अक्षर* depthNames[] = { "CV_8U", "CV_8S", "CV_16U", "CV_16S", "CV_32S", "CV_32F", "CV_64F", "CV_16F",
                                        "CV_16BF", "CV_Bool", "CV_64U", "CV_64S", "CV_32U",
                                        "CV_8F", "CV_8FNUZ" };
    परत (depth < CV_DEPTH_CURR_MAX && depth >= 0) ? depthNames[depth] : रिक्त;
}

cv::String typeToString_(पूर्णांक type)
{
    पूर्णांक depth = CV_MAT_DEPTH(type);
    पूर्णांक cn = CV_MAT_CN(type);
    जर (depth >= 0 && depth < CV_DEPTH_CURR_MAX)
        परत cv::format("%sC%d", depthToString_(depth), cn);
    परत cv::String();
}

ढाँचा<typename T> स्थिर CV_NORETURN
शून्य check_failed_auto_(स्थिरांक T& v1, स्थिरांक T& v2, स्थिरांक CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << " (expected: '" << ctx.p1_str << " " << getTestOpMath(ctx.testOp) << " " << ctx.p2_str << "'), where" << std::endl
        << "    '" << ctx.p1_str << "' is " << v1 << std::endl;
    जर (ctx.testOp != TEST_CUSTOM && ctx.testOp < CV__LAST_TEST_OP)
    {
        ss << "must be " << getTestOpPhraseStr(ctx.testOp) << std::endl;
    }
    ss  << "    '" << ctx.p2_str << "' is " << v2;
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.ओळ);
}
शून्य check_failed_MatDepth(स्थिरांक पूर्णांक v1, स्थिरांक पूर्णांक v2, स्थिरांक CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << " (expected: '" << ctx.p1_str << " " << getTestOpMath(ctx.testOp) << " " << ctx.p2_str << "'), where" << std::endl
        << "    '" << ctx.p1_str << "' is " << v1 << " (" << depthToString(v1) << ")" << std::endl;
    जर (ctx.testOp != TEST_CUSTOM && ctx.testOp < CV__LAST_TEST_OP)
    {
        ss << "must be " << getTestOpPhraseStr(ctx.testOp) << std::endl;
    }
    ss  << "    '" << ctx.p2_str << "' is " << v2 << " (" << depthToString(v2) << ")";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.ओळ);
}
शून्य check_failed_MatType(स्थिरांक पूर्णांक v1, स्थिरांक पूर्णांक v2, स्थिरांक CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << " (expected: '" << ctx.p1_str << " " << getTestOpMath(ctx.testOp) << " " << ctx.p2_str << "'), where" << std::endl
        << "    '" << ctx.p1_str << "' is " << v1 << " (" << typeToString(v1) << ")" << std::endl;
    जर (ctx.testOp != TEST_CUSTOM && ctx.testOp < CV__LAST_TEST_OP)
    {
        ss << "must be " << getTestOpPhraseStr(ctx.testOp) << std::endl;
    }
    ss  << "    '" << ctx.p2_str << "' is " << v2 << " (" << typeToString(v2) << ")";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.ओळ);
}
शून्य check_failed_MatChannels(स्थिरांक पूर्णांक v1, स्थिरांक पूर्णांक v2, स्थिरांक CheckContext& ctx)
{
    check_failed_auto_<पूर्णांक>(v1, v2, ctx);
}
शून्य check_failed_auto(स्थिरांक तार्किक v1, स्थिरांक तार्किक v2, स्थिरांक CheckContext& ctx)
{
    check_failed_auto_<तार्किक>(v1, v2, ctx);
}
शून्य check_failed_auto(स्थिरांक पूर्णांक v1, स्थिरांक पूर्णांक v2, स्थिरांक CheckContext& ctx)
{
    check_failed_auto_<पूर्णांक>(v1, v2, ctx);
}
शून्य check_failed_auto(स्थिरांक माप_प्रकार v1, स्थिरांक माप_प्रकार v2, स्थिरांक CheckContext& ctx)
{
    check_failed_auto_<माप_प्रकार>(v1, v2, ctx);
}
शून्य check_failed_auto(स्थिरांक दशांश v1, स्थिरांक दशांश v2, स्थिरांक CheckContext& ctx)
{
    check_failed_auto_<दशांश>(v1, v2, ctx);
}
शून्य check_failed_auto(स्थिरांक द्विगुण v1, स्थिरांक द्विगुण v2, स्थिरांक CheckContext& ctx)
{
    check_failed_auto_<द्विगुण>(v1, v2, ctx);
}
शून्य check_failed_auto(स्थिरांक Size_<पूर्णांक> v1, स्थिरांक Size_<पूर्णांक> v2, स्थिरांक CheckContext& ctx)
{
    check_failed_auto_< Size_<पूर्णांक> >(v1, v2, ctx);
}


ढाँचा<typename T> स्थिर CV_NORETURN
शून्य check_failed_auto_(स्थिरांक T& v, स्थिरांक CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << ":" << std::endl
        << "    '" << ctx.p2_str << "'" << std::endl
        << "where" << std::endl
        << "    '" << ctx.p1_str << "' is " << v;
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.ओळ);
}
शून्य check_failed_MatDepth(स्थिरांक पूर्णांक v, स्थिरांक CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << ":" << std::endl
        << "    '" << ctx.p2_str << "'" << std::endl
        << "where" << std::endl
        << "    '" << ctx.p1_str << "' is " << v << " (" << depthToString(v) << ")";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.ओळ);
}
शून्य check_failed_MatType(स्थिरांक पूर्णांक v, स्थिरांक CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << ":" << std::endl
        << "    '" << ctx.p2_str << "'" << std::endl
        << "where" << std::endl
        << "    '" << ctx.p1_str << "' is " << v << " (" << typeToString(v) << ")";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.ओळ);
}
शून्य check_failed_MatChannels(स्थिरांक पूर्णांक v, स्थिरांक CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << ":" << std::endl
        << "    '" << ctx.p2_str << "'" << std::endl
        << "where" << std::endl
        << "    '" << ctx.p1_str << "' is " << v;
    cv::error(cv::Error::Bad_अंक_Channels, ss.str(), ctx.func, ctx.file, ctx.ओळ);
}
शून्य check_failed_true(स्थिरांक तार्किक v, स्थिरांक CheckContext& ctx)
{
    CV_UNUSED(v);
    std::stringstream ss;
    ss  << ctx.message << ":" << std::endl
        << "    '" << ctx.p1_str << "' must be 'true'";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.ओळ);
}
शून्य check_failed_false(स्थिरांक तार्किक v, स्थिरांक CheckContext& ctx)
{
    CV_UNUSED(v);
    std::stringstream ss;
    ss  << ctx.message << ":" << std::endl
        << "    '" << ctx.p1_str << "' must be 'false'";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.ओळ);
}
शून्य check_failed_auto(स्थिरांक पूर्णांक v, स्थिरांक CheckContext& ctx)
{
    check_failed_auto_<पूर्णांक>(v, ctx);
}
शून्य check_failed_auto(स्थिरांक माप_प्रकार v, स्थिरांक CheckContext& ctx)
{
    check_failed_auto_<माप_प्रकार>(v, ctx);
}
शून्य check_failed_auto(स्थिरांक दशांश v, स्थिरांक CheckContext& ctx)
{
    check_failed_auto_<दशांश>(v, ctx);
}
शून्य check_failed_auto(स्थिरांक द्विगुण v, स्थिरांक CheckContext& ctx)
{
    check_failed_auto_<द्विगुण>(v, ctx);
}
शून्य check_failed_auto(स्थिरांक Size_<पूर्णांक> v, स्थिरांक CheckContext& ctx)
{
    check_failed_auto_< Size_<पूर्णांक> >(v, ctx);
}
शून्य check_failed_auto(स्थिरांक std::string& v, स्थिरांक CheckContext& ctx)
{
    check_failed_auto_< std::string >(v, ctx);
}


}} // namespace
