// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#અન્તર્ભાવ "precomp.hpp"

#અન્તર્ભાવ <sstream>

#અન્તર્ભાવ "opencv2/core/check.hpp"

નામાકાશ cv {

અચલ અક્ષર* depthToString(પૂર્ણાંક depth)
{
    અચલ અક્ષર* s = detail::depthToString_(depth);
    પાછા s ? s : "<invalid depth>";
}

cv::String typeToString(પૂર્ણાંક type)
{
    cv::String s = detail::typeToString_(type);
    જો (s.empty())
    {
        સ્થિર cv::String invalidType("<invalid type>");
        પાછા invalidType;
    }
    પાછા s;
}


નામાકાશ detail {

સ્થિર અચલ અક્ષર* getTestOpPhraseStr(unsigned testOp)
{
    સ્થિર અચલ અક્ષર* _names[] = { "{custom check}", "equal to", "not equal to", "less than or equal to", "less than", "greater than or equal to", "greater than" };
    CV_DbgAssert(testOp < CV__LAST_TEST_OP);
    પાછા testOp < CV__LAST_TEST_OP ? _names[testOp] : "???";
}
સ્થિર અચલ અક્ષર* getTestOpMath(unsigned testOp)
{
    સ્થિર અચલ અક્ષર* _names[] = { "???", "==", "!=", "<=", "<", ">=", ">" };
    CV_DbgAssert(testOp < CV__LAST_TEST_OP);
    પાછા testOp < CV__LAST_TEST_OP ? _names[testOp] : "???";
}

અચલ અક્ષર* depthToString_(પૂર્ણાંક depth)
{
    સ્થિર અચલ અક્ષર* depthNames[] = { "CV_8U", "CV_8S", "CV_16U", "CV_16S", "CV_32S", "CV_32F", "CV_64F", "CV_16F",
                                        "CV_16BF", "CV_Bool", "CV_64U", "CV_64S", "CV_32U",
                                        "CV_8F", "CV_8FNUZ" };
    પાછા (depth < CV_DEPTH_CURR_MAX && depth >= 0) ? depthNames[depth] : ખાલી;
}

cv::String typeToString_(પૂર્ણાંક type)
{
    પૂર્ણાંક depth = CV_MAT_DEPTH(type);
    પૂર્ણાંક cn = CV_MAT_CN(type);
    જો (depth >= 0 && depth < CV_DEPTH_CURR_MAX)
        પાછા cv::format("%sC%d", depthToString_(depth), cn);
    પાછા cv::String();
}

template<typename T> સ્થિર CV_NORETURN
શૂન્ય check_failed_auto_(અચલ T& v1, અચલ T& v2, અચલ CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << " (expected: '" << ctx.p1_str << " " << getTestOpMath(ctx.testOp) << " " << ctx.p2_str << "'), where" << std::endl
        << "    '" << ctx.p1_str << "' is " << v1 << std::endl;
    જો (ctx.testOp != TEST_CUSTOM && ctx.testOp < CV__LAST_TEST_OP)
    {
        ss << "must be " << getTestOpPhraseStr(ctx.testOp) << std::endl;
    }
    ss  << "    '" << ctx.p2_str << "' is " << v2;
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.લીટી);
}
શૂન્ય check_failed_MatDepth(અચલ પૂર્ણાંક v1, અચલ પૂર્ણાંક v2, અચલ CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << " (expected: '" << ctx.p1_str << " " << getTestOpMath(ctx.testOp) << " " << ctx.p2_str << "'), where" << std::endl
        << "    '" << ctx.p1_str << "' is " << v1 << " (" << depthToString(v1) << ")" << std::endl;
    જો (ctx.testOp != TEST_CUSTOM && ctx.testOp < CV__LAST_TEST_OP)
    {
        ss << "must be " << getTestOpPhraseStr(ctx.testOp) << std::endl;
    }
    ss  << "    '" << ctx.p2_str << "' is " << v2 << " (" << depthToString(v2) << ")";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.લીટી);
}
શૂન્ય check_failed_MatType(અચલ પૂર્ણાંક v1, અચલ પૂર્ણાંક v2, અચલ CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << " (expected: '" << ctx.p1_str << " " << getTestOpMath(ctx.testOp) << " " << ctx.p2_str << "'), where" << std::endl
        << "    '" << ctx.p1_str << "' is " << v1 << " (" << typeToString(v1) << ")" << std::endl;
    જો (ctx.testOp != TEST_CUSTOM && ctx.testOp < CV__LAST_TEST_OP)
    {
        ss << "must be " << getTestOpPhraseStr(ctx.testOp) << std::endl;
    }
    ss  << "    '" << ctx.p2_str << "' is " << v2 << " (" << typeToString(v2) << ")";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.લીટી);
}
શૂન્ય check_failed_MatChannels(અચલ પૂર્ણાંક v1, અચલ પૂર્ણાંક v2, અચલ CheckContext& ctx)
{
    check_failed_auto_<પૂર્ણાંક>(v1, v2, ctx);
}
શૂન્ય check_failed_auto(અચલ તાર્કિક v1, અચલ તાર્કિક v2, અચલ CheckContext& ctx)
{
    check_failed_auto_<તાર્કિક>(v1, v2, ctx);
}
શૂન્ય check_failed_auto(અચલ પૂર્ણાંક v1, અચલ પૂર્ણાંક v2, અચલ CheckContext& ctx)
{
    check_failed_auto_<પૂર્ણાંક>(v1, v2, ctx);
}
શૂન્ય check_failed_auto(અચલ size_t v1, અચલ size_t v2, અચલ CheckContext& ctx)
{
    check_failed_auto_<size_t>(v1, v2, ctx);
}
શૂન્ય check_failed_auto(અચલ દશાંશ v1, અચલ દશાંશ v2, અચલ CheckContext& ctx)
{
    check_failed_auto_<દશાંશ>(v1, v2, ctx);
}
શૂન્ય check_failed_auto(અચલ દ્વિગુણ v1, અચલ દ્વિગુણ v2, અચલ CheckContext& ctx)
{
    check_failed_auto_<દ્વિગુણ>(v1, v2, ctx);
}
શૂન્ય check_failed_auto(અચલ Size_<પૂર્ણાંક> v1, અચલ Size_<પૂર્ણાંક> v2, અચલ CheckContext& ctx)
{
    check_failed_auto_< Size_<પૂર્ણાંક> >(v1, v2, ctx);
}


template<typename T> સ્થિર CV_NORETURN
શૂન્ય check_failed_auto_(અચલ T& v, અચલ CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << ":" << std::endl
        << "    '" << ctx.p2_str << "'" << std::endl
        << "where" << std::endl
        << "    '" << ctx.p1_str << "' is " << v;
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.લીટી);
}
શૂન્ય check_failed_MatDepth(અચલ પૂર્ણાંક v, અચલ CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << ":" << std::endl
        << "    '" << ctx.p2_str << "'" << std::endl
        << "where" << std::endl
        << "    '" << ctx.p1_str << "' is " << v << " (" << depthToString(v) << ")";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.લીટી);
}
શૂન્ય check_failed_MatType(અચલ પૂર્ણાંક v, અચલ CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << ":" << std::endl
        << "    '" << ctx.p2_str << "'" << std::endl
        << "where" << std::endl
        << "    '" << ctx.p1_str << "' is " << v << " (" << typeToString(v) << ")";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.લીટી);
}
શૂન્ય check_failed_MatChannels(અચલ પૂર્ણાંક v, અચલ CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << ":" << std::endl
        << "    '" << ctx.p2_str << "'" << std::endl
        << "where" << std::endl
        << "    '" << ctx.p1_str << "' is " << v;
    cv::error(cv::Error::Bad_અંક_Channels, ss.str(), ctx.func, ctx.file, ctx.લીટી);
}
શૂન્ય check_failed_true(અચલ તાર્કિક v, અચલ CheckContext& ctx)
{
    CV_UNUSED(v);
    std::stringstream ss;
    ss  << ctx.message << ":" << std::endl
        << "    '" << ctx.p1_str << "' must be 'true'";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.લીટી);
}
શૂન્ય check_failed_false(અચલ તાર્કિક v, અચલ CheckContext& ctx)
{
    CV_UNUSED(v);
    std::stringstream ss;
    ss  << ctx.message << ":" << std::endl
        << "    '" << ctx.p1_str << "' must be 'false'";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.લીટી);
}
શૂન્ય check_failed_auto(અચલ પૂર્ણાંક v, અચલ CheckContext& ctx)
{
    check_failed_auto_<પૂર્ણાંક>(v, ctx);
}
શૂન્ય check_failed_auto(અચલ size_t v, અચલ CheckContext& ctx)
{
    check_failed_auto_<size_t>(v, ctx);
}
શૂન્ય check_failed_auto(અચલ દશાંશ v, અચલ CheckContext& ctx)
{
    check_failed_auto_<દશાંશ>(v, ctx);
}
શૂન્ય check_failed_auto(અચલ દ્વિગુણ v, અચલ CheckContext& ctx)
{
    check_failed_auto_<દ્વિગુણ>(v, ctx);
}
શૂન્ય check_failed_auto(અચલ Size_<પૂર્ણાંક> v, અચલ CheckContext& ctx)
{
    check_failed_auto_< Size_<પૂર્ણાંક> >(v, ctx);
}
શૂન્ય check_failed_auto(અચલ std::string& v, અચલ CheckContext& ctx)
{
    check_failed_auto_< std::string >(v, ctx);
}


}} // namespace
