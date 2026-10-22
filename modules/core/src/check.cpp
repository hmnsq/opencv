// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#అన్తర్భావ "precomp.hpp"

#అన్తర్భావ <sstream>

#అన్తర్భావ "opencv2/core/check.hpp"

నామాకాశ cv {

స్థిరరాశి అక్షరం* లోతు_గమ్యం_String(పూర్ణాంకం లోతు)
{
    స్థిరరాశి అక్షరం* s = detail::లోతు_గమ్యం_String_2(లోతు);
    తిరిగి s ? s : "<invalid depth>";
}

cv::String type_గమ్యం_String(పూర్ణాంకం type)
{
    cv::String s = detail::type_గమ్యం_String_2(type);
    ఒకవేళ (s.empty())
    {
        స్థిర cv::String invalidType("<invalid type>");
        తిరిగి invalidType;
    }
    తిరిగి s;
}


నామాకాశ detail {

స్థిర స్థిరరాశి అక్షరం* getTestOpPhraseStr(గుర్తులేని testOp)
{
    స్థిర స్థిరరాశి అక్షరం* పేర్లు[] = { "{custom check}", "equal to", "not equal to", "less than or equal to", "less than", "greater than or equal to", "greater than" };
    CV_DbgAssert(testOp < CV_చివరి_TEST_OP);
    తిరిగి testOp < CV_చివరి_TEST_OP ? పేర్లు[testOp] : "???";
}
స్థిర స్థిరరాశి అక్షరం* getTestOpMath(గుర్తులేని testOp)
{
    స్థిర స్థిరరాశి అక్షరం* పేర్లు[] = { "???", "==", "!=", "<=", "<", ">=", ">" };
    CV_DbgAssert(testOp < CV_చివరి_TEST_OP);
    తిరిగి testOp < CV_చివరి_TEST_OP ? పేర్లు[testOp] : "???";
}

స్థిరరాశి అక్షరం* లోతు_గమ్యం_String_2(పూర్ణాంకం లోతు)
{
    స్థిర స్థిరరాశి అక్షరం* లోతు_పేర్లు[] = { "CV_8U", "CV_8S", "CV_16U", "CV_16S", "CV_32S", "CV_32F", "CV_64F", "CV_16F",
                                        "CV_16BF", "CV_Bool", "CV_64U", "CV_64S", "CV_32U",
                                        "CV_8F", "CV_8FNUZ" };
    తిరిగి (లోతు < CV_లోతు_CURR_MAX && లోతు >= 0) ? లోతు_పేర్లు[లోతు] : ఖాళీ;
}

cv::String type_గమ్యం_String_2(పూర్ణాంకం type)
{
    పూర్ణాంకం లోతు = CV_MAT_లోతు(type);
    పూర్ణాంకం cn = CV_MAT_CN(type);
    ఒకవేళ (లోతు >= 0 && లోతు < CV_లోతు_CURR_MAX)
        తిరిగి cv::format("%sC%d", లోతు_గమ్యం_String_2(లోతు), cn);
    తిరిగి cv::String();
}

మూస<రకం_పేరు T> స్థిర CV_NORETURN
శూన్య check_failed_auto_(స్థిరరాశి T& v1, స్థిరరాశి T& v2, స్థిరరాశి CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << " (expected: '" << ctx.p1_str << " " << getTestOpMath(ctx.testOp) << " " << ctx.p2_str << "'), where" << std::పంక్తి_ముగింపు
        << "    '" << ctx.p1_str << "' is " << v1 << std::పంక్తి_ముగింపు;
    ఒకవేళ (ctx.testOp != TEST_CUSTOM && ctx.testOp < CV_చివరి_TEST_OP)
    {
        ss << "must be " << getTestOpPhraseStr(ctx.testOp) << std::పంక్తి_ముగింపు;
    }
    ss  << "    '" << ctx.p2_str << "' is " << v2;
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.పంక్తి);
}
శూన్య check_failed_Mat_లోతు(స్థిరరాశి పూర్ణాంకం v1, స్థిరరాశి పూర్ణాంకం v2, స్థిరరాశి CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << " (expected: '" << ctx.p1_str << " " << getTestOpMath(ctx.testOp) << " " << ctx.p2_str << "'), where" << std::పంక్తి_ముగింపు
        << "    '" << ctx.p1_str << "' is " << v1 << " (" << లోతు_గమ్యం_String(v1) << ")" << std::పంక్తి_ముగింపు;
    ఒకవేళ (ctx.testOp != TEST_CUSTOM && ctx.testOp < CV_చివరి_TEST_OP)
    {
        ss << "must be " << getTestOpPhraseStr(ctx.testOp) << std::పంక్తి_ముగింపు;
    }
    ss  << "    '" << ctx.p2_str << "' is " << v2 << " (" << లోతు_గమ్యం_String(v2) << ")";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.పంక్తి);
}
శూన్య check_failed_MatType(స్థిరరాశి పూర్ణాంకం v1, స్థిరరాశి పూర్ణాంకం v2, స్థిరరాశి CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << " (expected: '" << ctx.p1_str << " " << getTestOpMath(ctx.testOp) << " " << ctx.p2_str << "'), where" << std::పంక్తి_ముగింపు
        << "    '" << ctx.p1_str << "' is " << v1 << " (" << type_గమ్యం_String(v1) << ")" << std::పంక్తి_ముగింపు;
    ఒకవేళ (ctx.testOp != TEST_CUSTOM && ctx.testOp < CV_చివరి_TEST_OP)
    {
        ss << "must be " << getTestOpPhraseStr(ctx.testOp) << std::పంక్తి_ముగింపు;
    }
    ss  << "    '" << ctx.p2_str << "' is " << v2 << " (" << type_గమ్యం_String(v2) << ")";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.పంక్తి);
}
శూన్య check_failed_MatChannels(స్థిరరాశి పూర్ణాంకం v1, స్థిరరాశి పూర్ణాంకం v2, స్థిరరాశి CheckContext& ctx)
{
    check_failed_auto_<పూర్ణాంకం>(v1, v2, ctx);
}
శూన్య check_failed_auto(స్థిరరాశి తార్కిక v1, స్థిరరాశి తార్కిక v2, స్థిరరాశి CheckContext& ctx)
{
    check_failed_auto_<తార్కిక>(v1, v2, ctx);
}
శూన్య check_failed_auto(స్థిరరాశి పూర్ణాంకం v1, స్థిరరాశి పూర్ణాంకం v2, స్థిరరాశి CheckContext& ctx)
{
    check_failed_auto_<పూర్ణాంకం>(v1, v2, ctx);
}
శూన్య check_failed_auto(స్థిరరాశి size_t v1, స్థిరరాశి size_t v2, స్థిరరాశి CheckContext& ctx)
{
    check_failed_auto_<size_t>(v1, v2, ctx);
}
శూన్య check_failed_auto(స్థిరరాశి దశాంశం v1, స్థిరరాశి దశాంశం v2, స్థిరరాశి CheckContext& ctx)
{
    check_failed_auto_<దశాంశం>(v1, v2, ctx);
}
శూన్య check_failed_auto(స్థిరరాశి ద్విగుణ v1, స్థిరరాశి ద్విగుణ v2, స్థిరరాశి CheckContext& ctx)
{
    check_failed_auto_<ద్విగుణ>(v1, v2, ctx);
}
శూన్య check_failed_auto(స్థిరరాశి కొలత_2<పూర్ణాంకం> v1, స్థిరరాశి కొలత_2<పూర్ణాంకం> v2, స్థిరరాశి CheckContext& ctx)
{
    check_failed_auto_< కొలత_2<పూర్ణాంకం> >(v1, v2, ctx);
}


మూస<రకం_పేరు T> స్థిర CV_NORETURN
శూన్య check_failed_auto_(స్థిరరాశి T& v, స్థిరరాశి CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << ":" << std::పంక్తి_ముగింపు
        << "    '" << ctx.p2_str << "'" << std::పంక్తి_ముగింపు
        << "where" << std::పంక్తి_ముగింపు
        << "    '" << ctx.p1_str << "' is " << v;
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.పంక్తి);
}
శూన్య check_failed_Mat_లోతు(స్థిరరాశి పూర్ణాంకం v, స్థిరరాశి CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << ":" << std::పంక్తి_ముగింపు
        << "    '" << ctx.p2_str << "'" << std::పంక్తి_ముగింపు
        << "where" << std::పంక్తి_ముగింపు
        << "    '" << ctx.p1_str << "' is " << v << " (" << లోతు_గమ్యం_String(v) << ")";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.పంక్తి);
}
శూన్య check_failed_MatType(స్థిరరాశి పూర్ణాంకం v, స్థిరరాశి CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << ":" << std::పంక్తి_ముగింపు
        << "    '" << ctx.p2_str << "'" << std::పంక్తి_ముగింపు
        << "where" << std::పంక్తి_ముగింపు
        << "    '" << ctx.p1_str << "' is " << v << " (" << type_గమ్యం_String(v) << ")";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.పంక్తి);
}
శూన్య check_failed_MatChannels(స్థిరరాశి పూర్ణాంకం v, స్థిరరాశి CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << ":" << std::పంక్తి_ముగింపు
        << "    '" << ctx.p2_str << "'" << std::పంక్తి_ముగింపు
        << "where" << std::పంక్తి_ముగింపు
        << "    '" << ctx.p1_str << "' is " << v;
    cv::error(cv::Error::Bad_అంకె_Channels, ss.str(), ctx.func, ctx.file, ctx.పంక్తి);
}
శూన్య check_failed_true(స్థిరరాశి తార్కిక v, స్థిరరాశి CheckContext& ctx)
{
    CV_వాడని(v);
    std::stringstream ss;
    ss  << ctx.message << ":" << std::పంక్తి_ముగింపు
        << "    '" << ctx.p1_str << "' must be 'true'";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.పంక్తి);
}
శూన్య check_failed_false(స్థిరరాశి తార్కిక v, స్థిరరాశి CheckContext& ctx)
{
    CV_వాడని(v);
    std::stringstream ss;
    ss  << ctx.message << ":" << std::పంక్తి_ముగింపు
        << "    '" << ctx.p1_str << "' must be 'false'";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.పంక్తి);
}
శూన్య check_failed_auto(స్థిరరాశి పూర్ణాంకం v, స్థిరరాశి CheckContext& ctx)
{
    check_failed_auto_<పూర్ణాంకం>(v, ctx);
}
శూన్య check_failed_auto(స్థిరరాశి size_t v, స్థిరరాశి CheckContext& ctx)
{
    check_failed_auto_<size_t>(v, ctx);
}
శూన్య check_failed_auto(స్థిరరాశి దశాంశం v, స్థిరరాశి CheckContext& ctx)
{
    check_failed_auto_<దశాంశం>(v, ctx);
}
శూన్య check_failed_auto(స్థిరరాశి ద్విగుణ v, స్థిరరాశి CheckContext& ctx)
{
    check_failed_auto_<ద్విగుణ>(v, ctx);
}
శూన్య check_failed_auto(స్థిరరాశి కొలత_2<పూర్ణాంకం> v, స్థిరరాశి CheckContext& ctx)
{
    check_failed_auto_< కొలత_2<పూర్ణాంకం> >(v, ctx);
}
శూన్య check_failed_auto(స్థిరరాశి std::string& v, స్థిరరాశి CheckContext& ctx)
{
    check_failed_auto_< std::string >(v, ctx);
}


}} // namespace
