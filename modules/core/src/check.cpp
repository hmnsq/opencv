// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ଅନ୍ତର୍ଭାଵ "precomp.hpp"

#ଅନ୍ତର୍ଭାଵ <sstream>

#ଅନ୍ତର୍ଭାଵ "opencv2/core/check.hpp"

ନାମାକାଶ cv {

ଅଚଳ ଅକ୍ଷର* depthToString(ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth)
{
    ଅଚଳ ଅକ୍ଷର* s = detail::depthToString_(depth);
    ଫେରନ୍ତୁ s ? s : "<invalid depth>";
}

cv::String typeToString(ପୂର୍ଣ୍ଣସଂଖ୍ୟା type)
{
    cv::String s = detail::typeToString_(type);
    ଯଦି (s.empty())
    {
        ସ୍ଥିର cv::String invalidType("<invalid type>");
        ଫେରନ୍ତୁ invalidType;
    }
    ଫେରନ୍ତୁ s;
}


ନାମାକାଶ detail {

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର* getTestOpPhraseStr(unsigned testOp)
{
    ସ୍ଥିର ଅଚଳ ଅକ୍ଷର* _names[] = { "{custom check}", "equal to", "not equal to", "less than or equal to", "less than", "greater than or equal to", "greater than" };
    CV_DbgAssert(testOp < CV__LAST_TEST_OP);
    ଫେରନ୍ତୁ testOp < CV__LAST_TEST_OP ? _names[testOp] : "???";
}
ସ୍ଥିର ଅଚଳ ଅକ୍ଷର* getTestOpMath(unsigned testOp)
{
    ସ୍ଥିର ଅଚଳ ଅକ୍ଷର* _names[] = { "???", "==", "!=", "<=", "<", ">=", ">" };
    CV_DbgAssert(testOp < CV__LAST_TEST_OP);
    ଫେରନ୍ତୁ testOp < CV__LAST_TEST_OP ? _names[testOp] : "???";
}

ଅଚଳ ଅକ୍ଷର* depthToString_(ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth)
{
    ସ୍ଥିର ଅଚଳ ଅକ୍ଷର* depthNames[] = { "CV_8U", "CV_8S", "CV_16U", "CV_16S", "CV_32S", "CV_32F", "CV_64F", "CV_16F",
                                        "CV_16BF", "CV_Bool", "CV_64U", "CV_64S", "CV_32U",
                                        "CV_8F", "CV_8FNUZ" };
    ଫେରନ୍ତୁ (depth < CV_DEPTH_CURR_MAX && depth >= 0) ? depthNames[depth] : ଖାଲି;
}

cv::String typeToString_(ପୂର୍ଣ୍ଣସଂଖ୍ୟା type)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth = CV_MAT_DEPTH(type);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn = CV_MAT_CN(type);
    ଯଦି (depth >= 0 && depth < CV_DEPTH_CURR_MAX)
        ଫେରନ୍ତୁ cv::format("%sC%d", depthToString_(depth), cn);
    ଫେରନ୍ତୁ cv::String();
}

template<typename T> ସ୍ଥିର CV_NORETURN
ଶୂନ୍ୟ check_failed_auto_(ଅଚଳ T& v1, ଅଚଳ T& v2, ଅଚଳ CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << " (expected: '" << ctx.p1_str << " " << getTestOpMath(ctx.testOp) << " " << ctx.p2_str << "'), where" << std::endl
        << "    '" << ctx.p1_str << "' is " << v1 << std::endl;
    ଯଦି (ctx.testOp != TEST_CUSTOM && ctx.testOp < CV__LAST_TEST_OP)
    {
        ss << "must be " << getTestOpPhraseStr(ctx.testOp) << std::endl;
    }
    ss  << "    '" << ctx.p2_str << "' is " << v2;
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.ଧାଡ଼ି);
}
ଶୂନ୍ୟ check_failed_MatDepth(ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା v1, ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା v2, ଅଚଳ CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << " (expected: '" << ctx.p1_str << " " << getTestOpMath(ctx.testOp) << " " << ctx.p2_str << "'), where" << std::endl
        << "    '" << ctx.p1_str << "' is " << v1 << " (" << depthToString(v1) << ")" << std::endl;
    ଯଦି (ctx.testOp != TEST_CUSTOM && ctx.testOp < CV__LAST_TEST_OP)
    {
        ss << "must be " << getTestOpPhraseStr(ctx.testOp) << std::endl;
    }
    ss  << "    '" << ctx.p2_str << "' is " << v2 << " (" << depthToString(v2) << ")";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.ଧାଡ଼ି);
}
ଶୂନ୍ୟ check_failed_MatType(ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା v1, ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା v2, ଅଚଳ CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << " (expected: '" << ctx.p1_str << " " << getTestOpMath(ctx.testOp) << " " << ctx.p2_str << "'), where" << std::endl
        << "    '" << ctx.p1_str << "' is " << v1 << " (" << typeToString(v1) << ")" << std::endl;
    ଯଦି (ctx.testOp != TEST_CUSTOM && ctx.testOp < CV__LAST_TEST_OP)
    {
        ss << "must be " << getTestOpPhraseStr(ctx.testOp) << std::endl;
    }
    ss  << "    '" << ctx.p2_str << "' is " << v2 << " (" << typeToString(v2) << ")";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.ଧାଡ଼ି);
}
ଶୂନ୍ୟ check_failed_MatChannels(ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା v1, ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା v2, ଅଚଳ CheckContext& ctx)
{
    check_failed_auto_<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(v1, v2, ctx);
}
ଶୂନ୍ୟ check_failed_auto(ଅଚଳ ତାର୍କିକ v1, ଅଚଳ ତାର୍କିକ v2, ଅଚଳ CheckContext& ctx)
{
    check_failed_auto_<ତାର୍କିକ>(v1, v2, ctx);
}
ଶୂନ୍ୟ check_failed_auto(ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା v1, ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା v2, ଅଚଳ CheckContext& ctx)
{
    check_failed_auto_<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(v1, v2, ctx);
}
ଶୂନ୍ୟ check_failed_auto(ଅଚଳ size_t v1, ଅଚଳ size_t v2, ଅଚଳ CheckContext& ctx)
{
    check_failed_auto_<size_t>(v1, v2, ctx);
}
ଶୂନ୍ୟ check_failed_auto(ଅଚଳ ଦଶମିକ v1, ଅଚଳ ଦଶମିକ v2, ଅଚଳ CheckContext& ctx)
{
    check_failed_auto_<ଦଶମିକ>(v1, v2, ctx);
}
ଶୂନ୍ୟ check_failed_auto(ଅଚଳ ଦ୍ୱିଗୁଣ v1, ଅଚଳ ଦ୍ୱିଗୁଣ v2, ଅଚଳ CheckContext& ctx)
{
    check_failed_auto_<ଦ୍ୱିଗୁଣ>(v1, v2, ctx);
}
ଶୂନ୍ୟ check_failed_auto(ଅଚଳ Size_<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> v1, ଅଚଳ Size_<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> v2, ଅଚଳ CheckContext& ctx)
{
    check_failed_auto_< Size_<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> >(v1, v2, ctx);
}


template<typename T> ସ୍ଥିର CV_NORETURN
ଶୂନ୍ୟ check_failed_auto_(ଅଚଳ T& v, ଅଚଳ CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << ":" << std::endl
        << "    '" << ctx.p2_str << "'" << std::endl
        << "where" << std::endl
        << "    '" << ctx.p1_str << "' is " << v;
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.ଧାଡ଼ି);
}
ଶୂନ୍ୟ check_failed_MatDepth(ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା v, ଅଚଳ CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << ":" << std::endl
        << "    '" << ctx.p2_str << "'" << std::endl
        << "where" << std::endl
        << "    '" << ctx.p1_str << "' is " << v << " (" << depthToString(v) << ")";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.ଧାଡ଼ି);
}
ଶୂନ୍ୟ check_failed_MatType(ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା v, ଅଚଳ CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << ":" << std::endl
        << "    '" << ctx.p2_str << "'" << std::endl
        << "where" << std::endl
        << "    '" << ctx.p1_str << "' is " << v << " (" << typeToString(v) << ")";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.ଧାଡ଼ି);
}
ଶୂନ୍ୟ check_failed_MatChannels(ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା v, ଅଚଳ CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << ":" << std::endl
        << "    '" << ctx.p2_str << "'" << std::endl
        << "where" << std::endl
        << "    '" << ctx.p1_str << "' is " << v;
    cv::error(cv::Error::Bad_ଅଙ୍କ_Channels, ss.str(), ctx.func, ctx.file, ctx.ଧାଡ଼ି);
}
ଶୂନ୍ୟ check_failed_true(ଅଚଳ ତାର୍କିକ v, ଅଚଳ CheckContext& ctx)
{
    CV_UNUSED(v);
    std::stringstream ss;
    ss  << ctx.message << ":" << std::endl
        << "    '" << ctx.p1_str << "' must be 'true'";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.ଧାଡ଼ି);
}
ଶୂନ୍ୟ check_failed_false(ଅଚଳ ତାର୍କିକ v, ଅଚଳ CheckContext& ctx)
{
    CV_UNUSED(v);
    std::stringstream ss;
    ss  << ctx.message << ":" << std::endl
        << "    '" << ctx.p1_str << "' must be 'false'";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.ଧାଡ଼ି);
}
ଶୂନ୍ୟ check_failed_auto(ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା v, ଅଚଳ CheckContext& ctx)
{
    check_failed_auto_<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(v, ctx);
}
ଶୂନ୍ୟ check_failed_auto(ଅଚଳ size_t v, ଅଚଳ CheckContext& ctx)
{
    check_failed_auto_<size_t>(v, ctx);
}
ଶୂନ୍ୟ check_failed_auto(ଅଚଳ ଦଶମିକ v, ଅଚଳ CheckContext& ctx)
{
    check_failed_auto_<ଦଶମିକ>(v, ctx);
}
ଶୂନ୍ୟ check_failed_auto(ଅଚଳ ଦ୍ୱିଗୁଣ v, ଅଚଳ CheckContext& ctx)
{
    check_failed_auto_<ଦ୍ୱିଗୁଣ>(v, ctx);
}
ଶୂନ୍ୟ check_failed_auto(ଅଚଳ Size_<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> v, ଅଚଳ CheckContext& ctx)
{
    check_failed_auto_< Size_<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> >(v, ctx);
}
ଶୂନ୍ୟ check_failed_auto(ଅଚଳ std::string& v, ଅଚଳ CheckContext& ctx)
{
    check_failed_auto_< std::string >(v, ctx);
}


}} // namespace
