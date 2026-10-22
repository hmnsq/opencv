// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#অন্তর্ভাৱ "precomp.hpp"

#অন্তর্ভাৱ <sstream>

#অন্তর্ভাৱ "opencv2/core/check.hpp"

নামাকাশ cv {

ধ্রুৱ ৱর্ণ* depthToString(পূর্ণাঙ্ক depth)
{
    ধ্রুৱ ৱর্ণ* s = detail::depthToString_(depth);
    ঘূৰাই s ? s : "<invalid depth>";
}

cv::String typeToString(পূর্ণাঙ্ক type)
{
    cv::String s = detail::typeToString_(type);
    যদি (s.empty())
    {
        স্থির cv::String invalidType("<invalid type>");
        ঘূৰাই invalidType;
    }
    ঘূৰাই s;
}


নামাকাশ detail {

স্থির ধ্রুৱ ৱর্ণ* getTestOpPhraseStr(unsigned testOp)
{
    স্থির ধ্রুৱ ৱর্ণ* _names[] = { "{custom check}", "equal to", "not equal to", "less than or equal to", "less than", "greater than or equal to", "greater than" };
    CV_DbgAssert(testOp < CV__LAST_TEST_OP);
    ঘূৰাই testOp < CV__LAST_TEST_OP ? _names[testOp] : "???";
}
স্থির ধ্রুৱ ৱর্ণ* getTestOpMath(unsigned testOp)
{
    স্থির ধ্রুৱ ৱর্ণ* _names[] = { "???", "==", "!=", "<=", "<", ">=", ">" };
    CV_DbgAssert(testOp < CV__LAST_TEST_OP);
    ঘূৰাই testOp < CV__LAST_TEST_OP ? _names[testOp] : "???";
}

ধ্রুৱ ৱর্ণ* depthToString_(পূর্ণাঙ্ক depth)
{
    স্থির ধ্রুৱ ৱর্ণ* depthNames[] = { "CV_8U", "CV_8S", "CV_16U", "CV_16S", "CV_32S", "CV_32F", "CV_64F", "CV_16F",
                                        "CV_16BF", "CV_Bool", "CV_64U", "CV_64S", "CV_32U",
                                        "CV_8F", "CV_8FNUZ" };
    ঘূৰাই (depth < CV_DEPTH_CURR_MAX && depth >= 0) ? depthNames[depth] : রিক্ত;
}

cv::String typeToString_(পূর্ণাঙ্ক type)
{
    পূর্ণাঙ্ক depth = CV_MAT_DEPTH(type);
    পূর্ণাঙ্ক cn = CV_MAT_CN(type);
    যদি (depth >= 0 && depth < CV_DEPTH_CURR_MAX)
        ঘূৰাই cv::format("%sC%d", depthToString_(depth), cn);
    ঘূৰাই cv::String();
}

template<typename T> স্থির CV_NORETURN
শূন্য check_failed_auto_(ধ্রুৱ T& v1, ধ্রুৱ T& v2, ধ্রুৱ CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << " (expected: '" << ctx.p1_str << " " << getTestOpMath(ctx.testOp) << " " << ctx.p2_str << "'), where" << std::endl
        << "    '" << ctx.p1_str << "' is " << v1 << std::endl;
    যদি (ctx.testOp != TEST_CUSTOM && ctx.testOp < CV__LAST_TEST_OP)
    {
        ss << "must be " << getTestOpPhraseStr(ctx.testOp) << std::endl;
    }
    ss  << "    '" << ctx.p2_str << "' is " << v2;
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.সাৰি);
}
শূন্য check_failed_MatDepth(ধ্রুৱ পূর্ণাঙ্ক v1, ধ্রুৱ পূর্ণাঙ্ক v2, ধ্রুৱ CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << " (expected: '" << ctx.p1_str << " " << getTestOpMath(ctx.testOp) << " " << ctx.p2_str << "'), where" << std::endl
        << "    '" << ctx.p1_str << "' is " << v1 << " (" << depthToString(v1) << ")" << std::endl;
    যদি (ctx.testOp != TEST_CUSTOM && ctx.testOp < CV__LAST_TEST_OP)
    {
        ss << "must be " << getTestOpPhraseStr(ctx.testOp) << std::endl;
    }
    ss  << "    '" << ctx.p2_str << "' is " << v2 << " (" << depthToString(v2) << ")";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.সাৰি);
}
শূন্য check_failed_MatType(ধ্রুৱ পূর্ণাঙ্ক v1, ধ্রুৱ পূর্ণাঙ্ক v2, ধ্রুৱ CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << " (expected: '" << ctx.p1_str << " " << getTestOpMath(ctx.testOp) << " " << ctx.p2_str << "'), where" << std::endl
        << "    '" << ctx.p1_str << "' is " << v1 << " (" << typeToString(v1) << ")" << std::endl;
    যদি (ctx.testOp != TEST_CUSTOM && ctx.testOp < CV__LAST_TEST_OP)
    {
        ss << "must be " << getTestOpPhraseStr(ctx.testOp) << std::endl;
    }
    ss  << "    '" << ctx.p2_str << "' is " << v2 << " (" << typeToString(v2) << ")";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.সাৰি);
}
শূন্য check_failed_MatChannels(ধ্রুৱ পূর্ণাঙ্ক v1, ধ্রুৱ পূর্ণাঙ্ক v2, ধ্রুৱ CheckContext& ctx)
{
    check_failed_auto_<পূর্ণাঙ্ক>(v1, v2, ctx);
}
শূন্য check_failed_auto(ধ্রুৱ তার্কিক v1, ধ্রুৱ তার্কিক v2, ধ্রুৱ CheckContext& ctx)
{
    check_failed_auto_<তার্কিক>(v1, v2, ctx);
}
শূন্য check_failed_auto(ধ্রুৱ পূর্ণাঙ্ক v1, ধ্রুৱ পূর্ণাঙ্ক v2, ধ্রুৱ CheckContext& ctx)
{
    check_failed_auto_<পূর্ণাঙ্ক>(v1, v2, ctx);
}
শূন্য check_failed_auto(ধ্রুৱ size_t v1, ধ্রুৱ size_t v2, ধ্রুৱ CheckContext& ctx)
{
    check_failed_auto_<size_t>(v1, v2, ctx);
}
শূন্য check_failed_auto(ধ্রুৱ ভিন্ন v1, ধ্রুৱ ভিন্ন v2, ধ্রুৱ CheckContext& ctx)
{
    check_failed_auto_<ভিন্ন>(v1, v2, ctx);
}
শূন্য check_failed_auto(ধ্রুৱ দ্ৱিগুণ v1, ধ্রুৱ দ্ৱিগুণ v2, ধ্রুৱ CheckContext& ctx)
{
    check_failed_auto_<দ্ৱিগুণ>(v1, v2, ctx);
}
শূন্য check_failed_auto(ধ্রুৱ Size_<পূর্ণাঙ্ক> v1, ধ্রুৱ Size_<পূর্ণাঙ্ক> v2, ধ্রুৱ CheckContext& ctx)
{
    check_failed_auto_< Size_<পূর্ণাঙ্ক> >(v1, v2, ctx);
}


template<typename T> স্থির CV_NORETURN
শূন্য check_failed_auto_(ধ্রুৱ T& v, ধ্রুৱ CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << ":" << std::endl
        << "    '" << ctx.p2_str << "'" << std::endl
        << "where" << std::endl
        << "    '" << ctx.p1_str << "' is " << v;
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.সাৰি);
}
শূন্য check_failed_MatDepth(ধ্রুৱ পূর্ণাঙ্ক v, ধ্রুৱ CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << ":" << std::endl
        << "    '" << ctx.p2_str << "'" << std::endl
        << "where" << std::endl
        << "    '" << ctx.p1_str << "' is " << v << " (" << depthToString(v) << ")";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.সাৰি);
}
শূন্য check_failed_MatType(ধ্রুৱ পূর্ণাঙ্ক v, ধ্রুৱ CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << ":" << std::endl
        << "    '" << ctx.p2_str << "'" << std::endl
        << "where" << std::endl
        << "    '" << ctx.p1_str << "' is " << v << " (" << typeToString(v) << ")";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.সাৰি);
}
শূন্য check_failed_MatChannels(ধ্রুৱ পূর্ণাঙ্ক v, ধ্রুৱ CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << ":" << std::endl
        << "    '" << ctx.p2_str << "'" << std::endl
        << "where" << std::endl
        << "    '" << ctx.p1_str << "' is " << v;
    cv::error(cv::Error::Bad_অঙ্ক_Channels, ss.str(), ctx.func, ctx.file, ctx.সাৰি);
}
শূন্য check_failed_true(ধ্রুৱ তার্কিক v, ধ্রুৱ CheckContext& ctx)
{
    CV_UNUSED(v);
    std::stringstream ss;
    ss  << ctx.message << ":" << std::endl
        << "    '" << ctx.p1_str << "' must be 'true'";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.সাৰি);
}
শূন্য check_failed_false(ধ্রুৱ তার্কিক v, ধ্রুৱ CheckContext& ctx)
{
    CV_UNUSED(v);
    std::stringstream ss;
    ss  << ctx.message << ":" << std::endl
        << "    '" << ctx.p1_str << "' must be 'false'";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.সাৰি);
}
শূন্য check_failed_auto(ধ্রুৱ পূর্ণাঙ্ক v, ধ্রুৱ CheckContext& ctx)
{
    check_failed_auto_<পূর্ণাঙ্ক>(v, ctx);
}
শূন্য check_failed_auto(ধ্রুৱ size_t v, ধ্রুৱ CheckContext& ctx)
{
    check_failed_auto_<size_t>(v, ctx);
}
শূন্য check_failed_auto(ধ্রুৱ ভিন্ন v, ধ্রুৱ CheckContext& ctx)
{
    check_failed_auto_<ভিন্ন>(v, ctx);
}
শূন্য check_failed_auto(ধ্রুৱ দ্ৱিগুণ v, ধ্রুৱ CheckContext& ctx)
{
    check_failed_auto_<দ্ৱিগুণ>(v, ctx);
}
শূন্য check_failed_auto(ধ্রুৱ Size_<পূর্ণাঙ্ক> v, ধ্রুৱ CheckContext& ctx)
{
    check_failed_auto_< Size_<পূর্ণাঙ্ক> >(v, ctx);
}
শূন্য check_failed_auto(ধ্রুৱ std::string& v, ধ্রুৱ CheckContext& ctx)
{
    check_failed_auto_< std::string >(v, ctx);
}


}} // namespace
