// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#அந்தர்பாவ "precomp.hpp"

#அந்தர்பாவ <sstream>

#அந்தர்பாவ "opencv2/core/check.hpp"

நாமாகாஶ cv {

மாறிலி எழுத்து* depthToString(முழு depth)
{
    மாறிலி எழுத்து* s = detail::depthToString_(depth);
    திரும்பு s ? s : "<invalid depth>";
}

cv::String typeToString(முழு type)
{
    cv::String s = detail::typeToString_(type);
    எனில் (s.empty())
    {
        நிலையான cv::String invalidType("<invalid type>");
        திரும்பு invalidType;
    }
    திரும்பு s;
}


நாமாகாஶ detail {

நிலையான மாறிலி எழுத்து* getTestOpPhraseStr(unsigned testOp)
{
    நிலையான மாறிலி எழுத்து* _names[] = { "{custom check}", "equal to", "not equal to", "less than or equal to", "less than", "greater than or equal to", "greater than" };
    CV_DbgAssert(testOp < CV__LAST_TEST_OP);
    திரும்பு testOp < CV__LAST_TEST_OP ? _names[testOp] : "???";
}
நிலையான மாறிலி எழுத்து* getTestOpMath(unsigned testOp)
{
    நிலையான மாறிலி எழுத்து* _names[] = { "???", "==", "!=", "<=", "<", ">=", ">" };
    CV_DbgAssert(testOp < CV__LAST_TEST_OP);
    திரும்பு testOp < CV__LAST_TEST_OP ? _names[testOp] : "???";
}

மாறிலி எழுத்து* depthToString_(முழு depth)
{
    நிலையான மாறிலி எழுத்து* depthNames[] = { "CV_8U", "CV_8S", "CV_16U", "CV_16S", "CV_32S", "CV_32F", "CV_64F", "CV_16F",
                                        "CV_16BF", "CV_Bool", "CV_64U", "CV_64S", "CV_32U",
                                        "CV_8F", "CV_8FNUZ" };
    திரும்பு (depth < CV_DEPTH_CURR_MAX && depth >= 0) ? depthNames[depth] : வெற்று;
}

cv::String typeToString_(முழு type)
{
    முழு depth = CV_MAT_DEPTH(type);
    முழு cn = CV_MAT_CN(type);
    எனில் (depth >= 0 && depth < CV_DEPTH_CURR_MAX)
        திரும்பு cv::format("%sC%d", depthToString_(depth), cn);
    திரும்பு cv::String();
}

template<typename T> நிலையான CV_NORETURN
வெறுமை check_failed_auto_(மாறிலி T& v1, மாறிலி T& v2, மாறிலி CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << " (expected: '" << ctx.p1_str << " " << getTestOpMath(ctx.testOp) << " " << ctx.p2_str << "'), where" << std::endl
        << "    '" << ctx.p1_str << "' is " << v1 << std::endl;
    எனில் (ctx.testOp != TEST_CUSTOM && ctx.testOp < CV__LAST_TEST_OP)
    {
        ss << "must be " << getTestOpPhraseStr(ctx.testOp) << std::endl;
    }
    ss  << "    '" << ctx.p2_str << "' is " << v2;
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.வரி);
}
வெறுமை check_failed_MatDepth(மாறிலி முழு v1, மாறிலி முழு v2, மாறிலி CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << " (expected: '" << ctx.p1_str << " " << getTestOpMath(ctx.testOp) << " " << ctx.p2_str << "'), where" << std::endl
        << "    '" << ctx.p1_str << "' is " << v1 << " (" << depthToString(v1) << ")" << std::endl;
    எனில் (ctx.testOp != TEST_CUSTOM && ctx.testOp < CV__LAST_TEST_OP)
    {
        ss << "must be " << getTestOpPhraseStr(ctx.testOp) << std::endl;
    }
    ss  << "    '" << ctx.p2_str << "' is " << v2 << " (" << depthToString(v2) << ")";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.வரி);
}
வெறுமை check_failed_MatType(மாறிலி முழு v1, மாறிலி முழு v2, மாறிலி CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << " (expected: '" << ctx.p1_str << " " << getTestOpMath(ctx.testOp) << " " << ctx.p2_str << "'), where" << std::endl
        << "    '" << ctx.p1_str << "' is " << v1 << " (" << typeToString(v1) << ")" << std::endl;
    எனில் (ctx.testOp != TEST_CUSTOM && ctx.testOp < CV__LAST_TEST_OP)
    {
        ss << "must be " << getTestOpPhraseStr(ctx.testOp) << std::endl;
    }
    ss  << "    '" << ctx.p2_str << "' is " << v2 << " (" << typeToString(v2) << ")";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.வரி);
}
வெறுமை check_failed_MatChannels(மாறிலி முழு v1, மாறிலி முழு v2, மாறிலி CheckContext& ctx)
{
    check_failed_auto_<முழு>(v1, v2, ctx);
}
வெறுமை check_failed_auto(மாறிலி தருக்க v1, மாறிலி தருக்க v2, மாறிலி CheckContext& ctx)
{
    check_failed_auto_<தருக்க>(v1, v2, ctx);
}
வெறுமை check_failed_auto(மாறிலி முழு v1, மாறிலி முழு v2, மாறிலி CheckContext& ctx)
{
    check_failed_auto_<முழு>(v1, v2, ctx);
}
வெறுமை check_failed_auto(மாறிலி size_t v1, மாறிலி size_t v2, மாறிலி CheckContext& ctx)
{
    check_failed_auto_<size_t>(v1, v2, ctx);
}
வெறுமை check_failed_auto(மாறிலி தசம v1, மாறிலி தசம v2, மாறிலி CheckContext& ctx)
{
    check_failed_auto_<தசம>(v1, v2, ctx);
}
வெறுமை check_failed_auto(மாறிலி இரட்டை v1, மாறிலி இரட்டை v2, மாறிலி CheckContext& ctx)
{
    check_failed_auto_<இரட்டை>(v1, v2, ctx);
}
வெறுமை check_failed_auto(மாறிலி Size_<முழு> v1, மாறிலி Size_<முழு> v2, மாறிலி CheckContext& ctx)
{
    check_failed_auto_< Size_<முழு> >(v1, v2, ctx);
}


template<typename T> நிலையான CV_NORETURN
வெறுமை check_failed_auto_(மாறிலி T& v, மாறிலி CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << ":" << std::endl
        << "    '" << ctx.p2_str << "'" << std::endl
        << "where" << std::endl
        << "    '" << ctx.p1_str << "' is " << v;
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.வரி);
}
வெறுமை check_failed_MatDepth(மாறிலி முழு v, மாறிலி CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << ":" << std::endl
        << "    '" << ctx.p2_str << "'" << std::endl
        << "where" << std::endl
        << "    '" << ctx.p1_str << "' is " << v << " (" << depthToString(v) << ")";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.வரி);
}
வெறுமை check_failed_MatType(மாறிலி முழு v, மாறிலி CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << ":" << std::endl
        << "    '" << ctx.p2_str << "'" << std::endl
        << "where" << std::endl
        << "    '" << ctx.p1_str << "' is " << v << " (" << typeToString(v) << ")";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.வரி);
}
வெறுமை check_failed_MatChannels(மாறிலி முழு v, மாறிலி CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << ":" << std::endl
        << "    '" << ctx.p2_str << "'" << std::endl
        << "where" << std::endl
        << "    '" << ctx.p1_str << "' is " << v;
    cv::error(cv::Error::Bad_இலக்கம்_Channels, ss.str(), ctx.func, ctx.file, ctx.வரி);
}
வெறுமை check_failed_true(மாறிலி தருக்க v, மாறிலி CheckContext& ctx)
{
    CV_UNUSED(v);
    std::stringstream ss;
    ss  << ctx.message << ":" << std::endl
        << "    '" << ctx.p1_str << "' must be 'true'";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.வரி);
}
வெறுமை check_failed_false(மாறிலி தருக்க v, மாறிலி CheckContext& ctx)
{
    CV_UNUSED(v);
    std::stringstream ss;
    ss  << ctx.message << ":" << std::endl
        << "    '" << ctx.p1_str << "' must be 'false'";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.வரி);
}
வெறுமை check_failed_auto(மாறிலி முழு v, மாறிலி CheckContext& ctx)
{
    check_failed_auto_<முழு>(v, ctx);
}
வெறுமை check_failed_auto(மாறிலி size_t v, மாறிலி CheckContext& ctx)
{
    check_failed_auto_<size_t>(v, ctx);
}
வெறுமை check_failed_auto(மாறிலி தசம v, மாறிலி CheckContext& ctx)
{
    check_failed_auto_<தசம>(v, ctx);
}
வெறுமை check_failed_auto(மாறிலி இரட்டை v, மாறிலி CheckContext& ctx)
{
    check_failed_auto_<இரட்டை>(v, ctx);
}
வெறுமை check_failed_auto(மாறிலி Size_<முழு> v, மாறிலி CheckContext& ctx)
{
    check_failed_auto_< Size_<முழு> >(v, ctx);
}
வெறுமை check_failed_auto(மாறிலி std::string& v, மாறிலி CheckContext& ctx)
{
    check_failed_auto_< std::string >(v, ctx);
}


}} // namespace
