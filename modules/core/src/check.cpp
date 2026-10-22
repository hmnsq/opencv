// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#অন্তর্ভাব "precomp.hpp"

#অন্তর্ভাব <sstream>

#অন্তর্ভাব "opencv2/core/check.hpp"

নামাকাশ cv {

ধ্রুবক অক্ষর* depthToString(পূর্ণসংখ্যা depth)
{
    ধ্রুবক অক্ষর* s = detail::depthToString_(depth);
    ফেরত s ? s : "<invalid depth>";
}

cv::String typeToString(পূর্ণসংখ্যা type)
{
    cv::String s = detail::typeToString_(type);
    যদি (s.empty())
    {
        স্থির cv::String invalidType("<invalid type>");
        ফেরত invalidType;
    }
    ফেরত s;
}


নামাকাশ detail {

স্থির ধ্রুবক অক্ষর* getTestOpPhraseStr(unsigned testOp)
{
    স্থির ধ্রুবক অক্ষর* _names[] = { "{custom check}", "equal to", "not equal to", "less than or equal to", "less than", "greater than or equal to", "greater than" };
    CV_DbgAssert(testOp < CV__LAST_TEST_OP);
    ফেরত testOp < CV__LAST_TEST_OP ? _names[testOp] : "???";
}
স্থির ধ্রুবক অক্ষর* getTestOpMath(unsigned testOp)
{
    স্থির ধ্রুবক অক্ষর* _names[] = { "???", "==", "!=", "<=", "<", ">=", ">" };
    CV_DbgAssert(testOp < CV__LAST_TEST_OP);
    ফেরত testOp < CV__LAST_TEST_OP ? _names[testOp] : "???";
}

ধ্রুবক অক্ষর* depthToString_(পূর্ণসংখ্যা depth)
{
    স্থির ধ্রুবক অক্ষর* depthNames[] = { "CV_8U", "CV_8S", "CV_16U", "CV_16S", "CV_32S", "CV_32F", "CV_64F", "CV_16F",
                                        "CV_16BF", "CV_Bool", "CV_64U", "CV_64S", "CV_32U",
                                        "CV_8F", "CV_8FNUZ" };
    ফেরত (depth < CV_DEPTH_CURR_MAX && depth >= 0) ? depthNames[depth] : ফাঁকা;
}

cv::String typeToString_(পূর্ণসংখ্যা type)
{
    পূর্ণসংখ্যা depth = CV_MAT_DEPTH(type);
    পূর্ণসংখ্যা cn = CV_MAT_CN(type);
    যদি (depth >= 0 && depth < CV_DEPTH_CURR_MAX)
        ফেরত cv::format("%sC%d", depthToString_(depth), cn);
    ফেরত cv::String();
}

template<typename T> স্থির CV_NORETURN
শূন্য check_failed_auto_(ধ্রুবক T& v1, ধ্রুবক T& v2, ধ্রুবক CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << " (expected: '" << ctx.p1_str << " " << getTestOpMath(ctx.testOp) << " " << ctx.p2_str << "'), where" << std::endl
        << "    '" << ctx.p1_str << "' is " << v1 << std::endl;
    যদি (ctx.testOp != TEST_CUSTOM && ctx.testOp < CV__LAST_TEST_OP)
    {
        ss << "must be " << getTestOpPhraseStr(ctx.testOp) << std::endl;
    }
    ss  << "    '" << ctx.p2_str << "' is " << v2;
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.সারি);
}
শূন্য check_failed_MatDepth(ধ্রুবক পূর্ণসংখ্যা v1, ধ্রুবক পূর্ণসংখ্যা v2, ধ্রুবক CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << " (expected: '" << ctx.p1_str << " " << getTestOpMath(ctx.testOp) << " " << ctx.p2_str << "'), where" << std::endl
        << "    '" << ctx.p1_str << "' is " << v1 << " (" << depthToString(v1) << ")" << std::endl;
    যদি (ctx.testOp != TEST_CUSTOM && ctx.testOp < CV__LAST_TEST_OP)
    {
        ss << "must be " << getTestOpPhraseStr(ctx.testOp) << std::endl;
    }
    ss  << "    '" << ctx.p2_str << "' is " << v2 << " (" << depthToString(v2) << ")";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.সারি);
}
শূন্য check_failed_MatType(ধ্রুবক পূর্ণসংখ্যা v1, ধ্রুবক পূর্ণসংখ্যা v2, ধ্রুবক CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << " (expected: '" << ctx.p1_str << " " << getTestOpMath(ctx.testOp) << " " << ctx.p2_str << "'), where" << std::endl
        << "    '" << ctx.p1_str << "' is " << v1 << " (" << typeToString(v1) << ")" << std::endl;
    যদি (ctx.testOp != TEST_CUSTOM && ctx.testOp < CV__LAST_TEST_OP)
    {
        ss << "must be " << getTestOpPhraseStr(ctx.testOp) << std::endl;
    }
    ss  << "    '" << ctx.p2_str << "' is " << v2 << " (" << typeToString(v2) << ")";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.সারি);
}
শূন্য check_failed_MatChannels(ধ্রুবক পূর্ণসংখ্যা v1, ধ্রুবক পূর্ণসংখ্যা v2, ধ্রুবক CheckContext& ctx)
{
    check_failed_auto_<পূর্ণসংখ্যা>(v1, v2, ctx);
}
শূন্য check_failed_auto(ধ্রুবক যৌক্তিক v1, ধ্রুবক যৌক্তিক v2, ধ্রুবক CheckContext& ctx)
{
    check_failed_auto_<যৌক্তিক>(v1, v2, ctx);
}
শূন্য check_failed_auto(ধ্রুবক পূর্ণসংখ্যা v1, ধ্রুবক পূর্ণসংখ্যা v2, ধ্রুবক CheckContext& ctx)
{
    check_failed_auto_<পূর্ণসংখ্যা>(v1, v2, ctx);
}
শূন্য check_failed_auto(ধ্রুবক size_t v1, ধ্রুবক size_t v2, ধ্রুবক CheckContext& ctx)
{
    check_failed_auto_<size_t>(v1, v2, ctx);
}
শূন্য check_failed_auto(ধ্রুবক দশমিক v1, ধ্রুবক দশমিক v2, ধ্রুবক CheckContext& ctx)
{
    check_failed_auto_<দশমিক>(v1, v2, ctx);
}
শূন্য check_failed_auto(ধ্রুবক দ্বিগুণ v1, ধ্রুবক দ্বিগুণ v2, ধ্রুবক CheckContext& ctx)
{
    check_failed_auto_<দ্বিগুণ>(v1, v2, ctx);
}
শূন্য check_failed_auto(ধ্রুবক আকার_3<পূর্ণসংখ্যা> v1, ধ্রুবক আকার_3<পূর্ণসংখ্যা> v2, ধ্রুবক CheckContext& ctx)
{
    check_failed_auto_< আকার_3<পূর্ণসংখ্যা> >(v1, v2, ctx);
}


template<typename T> স্থির CV_NORETURN
শূন্য check_failed_auto_(ধ্রুবক T& v, ধ্রুবক CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << ":" << std::endl
        << "    '" << ctx.p2_str << "'" << std::endl
        << "where" << std::endl
        << "    '" << ctx.p1_str << "' is " << v;
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.সারি);
}
শূন্য check_failed_MatDepth(ধ্রুবক পূর্ণসংখ্যা v, ধ্রুবক CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << ":" << std::endl
        << "    '" << ctx.p2_str << "'" << std::endl
        << "where" << std::endl
        << "    '" << ctx.p1_str << "' is " << v << " (" << depthToString(v) << ")";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.সারি);
}
শূন্য check_failed_MatType(ধ্রুবক পূর্ণসংখ্যা v, ধ্রুবক CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << ":" << std::endl
        << "    '" << ctx.p2_str << "'" << std::endl
        << "where" << std::endl
        << "    '" << ctx.p1_str << "' is " << v << " (" << typeToString(v) << ")";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.সারি);
}
শূন্য check_failed_MatChannels(ধ্রুবক পূর্ণসংখ্যা v, ধ্রুবক CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << ":" << std::endl
        << "    '" << ctx.p2_str << "'" << std::endl
        << "where" << std::endl
        << "    '" << ctx.p1_str << "' is " << v;
    cv::error(cv::Error::Bad_অঙ্ক_Channels, ss.str(), ctx.func, ctx.file, ctx.সারি);
}
শূন্য check_failed_true(ধ্রুবক যৌক্তিক v, ধ্রুবক CheckContext& ctx)
{
    CV_UNUSED(v);
    std::stringstream ss;
    ss  << ctx.message << ":" << std::endl
        << "    '" << ctx.p1_str << "' must be 'true'";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.সারি);
}
শূন্য check_failed_false(ধ্রুবক যৌক্তিক v, ধ্রুবক CheckContext& ctx)
{
    CV_UNUSED(v);
    std::stringstream ss;
    ss  << ctx.message << ":" << std::endl
        << "    '" << ctx.p1_str << "' must be 'false'";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.সারি);
}
শূন্য check_failed_auto(ধ্রুবক পূর্ণসংখ্যা v, ধ্রুবক CheckContext& ctx)
{
    check_failed_auto_<পূর্ণসংখ্যা>(v, ctx);
}
শূন্য check_failed_auto(ধ্রুবক size_t v, ধ্রুবক CheckContext& ctx)
{
    check_failed_auto_<size_t>(v, ctx);
}
শূন্য check_failed_auto(ধ্রুবক দশমিক v, ধ্রুবক CheckContext& ctx)
{
    check_failed_auto_<দশমিক>(v, ctx);
}
শূন্য check_failed_auto(ধ্রুবক দ্বিগুণ v, ধ্রুবক CheckContext& ctx)
{
    check_failed_auto_<দ্বিগুণ>(v, ctx);
}
শূন্য check_failed_auto(ধ্রুবক আকার_3<পূর্ণসংখ্যা> v, ধ্রুবক CheckContext& ctx)
{
    check_failed_auto_< আকার_3<পূর্ণসংখ্যা> >(v, ctx);
}
শূন্য check_failed_auto(ধ্রুবক std::string& v, ধ্রুবক CheckContext& ctx)
{
    check_failed_auto_< std::string >(v, ctx);
}


}} // namespace
