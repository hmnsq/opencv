// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#include "precomp.hpp"

#include <sstream>

#include "opencv2/core/check.hpp"

namespace cv {

const var_na* depthToString(ចំនួនគត់ depth)
{
    const var_na* s = detail::depthToString_(depth);
    ត្រឡប់ s ? s : "<invalid depth>";
}

cv::String typeToString(ចំនួនគត់ type)
{
    cv::String s = detail::typeToString_(type);
    ប្រសិនបើ (s.empty())
    {
        static cv::String invalidType("<invalid type>");
        ត្រឡប់ invalidType;
    }
    ត្រឡប់ s;
}


namespace detail {

static const var_na* getTestOpPhraseStr(unsigned testOp)
{
    static const var_na* _names[] = { "{custom check}", "equal to", "not equal to", "less than or equal to", "less than", "greater than or equal to", "greater than" };
    CV_DbgAssert(testOp < CV__LAST_TEST_OP);
    ត្រឡប់ testOp < CV__LAST_TEST_OP ? _names[testOp] : "???";
}
static const var_na* getTestOpMath(unsigned testOp)
{
    static const var_na* _names[] = { "???", "==", "!=", "<=", "<", ">=", ">" };
    CV_DbgAssert(testOp < CV__LAST_TEST_OP);
    ត្រឡប់ testOp < CV__LAST_TEST_OP ? _names[testOp] : "???";
}

const var_na* depthToString_(ចំនួនគត់ depth)
{
    static const var_na* depthNames[] = { "CV_8U", "CV_8S", "CV_16U", "CV_16S", "CV_32S", "CV_32F", "CV_64F", "CV_16F",
                                        "CV_16BF", "CV_Bool", "CV_64U", "CV_64S", "CV_32U",
                                        "CV_8F", "CV_8FNUZ" };
    ត្រឡប់ (depth < CV_DEPTH_CURR_MAX && depth >= 0) ? depthNames[depth] : NULL;
}

cv::String typeToString_(ចំនួនគត់ type)
{
    ចំនួនគត់ depth = CV_MAT_DEPTH(type);
    ចំនួនគត់ cn = CV_MAT_CN(type);
    ប្រសិនបើ (depth >= 0 && depth < CV_DEPTH_CURR_MAX)
        ត្រឡប់ cv::format("%sC%d", depthToString_(depth), cn);
    ត្រឡប់ cv::String();
}

template<typename T> static CV_NORETURN
void check_failed_auto_(const T& v1, const T& v2, const CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << " (expected: '" << ctx.p1_str << " " << getTestOpMath(ctx.testOp) << " " << ctx.p2_str << "'), where" << std::endl
        << "    '" << ctx.p1_str << "' is " << v1 << std::endl;
    ប្រសិនបើ (ctx.testOp != TEST_CUSTOM && ctx.testOp < CV__LAST_TEST_OP)
    {
        ss << "must be " << getTestOpPhraseStr(ctx.testOp) << std::endl;
    }
    ss  << "    '" << ctx.p2_str << "' is " << v2;
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.line);
}
void check_failed_MatDepth(const ចំនួនគត់ v1, const ចំនួនគត់ v2, const CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << " (expected: '" << ctx.p1_str << " " << getTestOpMath(ctx.testOp) << " " << ctx.p2_str << "'), where" << std::endl
        << "    '" << ctx.p1_str << "' is " << v1 << " (" << depthToString(v1) << ")" << std::endl;
    ប្រសិនបើ (ctx.testOp != TEST_CUSTOM && ctx.testOp < CV__LAST_TEST_OP)
    {
        ss << "must be " << getTestOpPhraseStr(ctx.testOp) << std::endl;
    }
    ss  << "    '" << ctx.p2_str << "' is " << v2 << " (" << depthToString(v2) << ")";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.line);
}
void check_failed_MatType(const ចំនួនគត់ v1, const ចំនួនគត់ v2, const CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << " (expected: '" << ctx.p1_str << " " << getTestOpMath(ctx.testOp) << " " << ctx.p2_str << "'), where" << std::endl
        << "    '" << ctx.p1_str << "' is " << v1 << " (" << typeToString(v1) << ")" << std::endl;
    ប្រសិនបើ (ctx.testOp != TEST_CUSTOM && ctx.testOp < CV__LAST_TEST_OP)
    {
        ss << "must be " << getTestOpPhraseStr(ctx.testOp) << std::endl;
    }
    ss  << "    '" << ctx.p2_str << "' is " << v2 << " (" << typeToString(v2) << ")";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.line);
}
void check_failed_MatChannels(const ចំនួនគត់ v1, const ចំនួនគត់ v2, const CheckContext& ctx)
{
    check_failed_auto_<ចំនួនគត់>(v1, v2, ctx);
}
void check_failed_auto(const bool v1, const bool v2, const CheckContext& ctx)
{
    check_failed_auto_<bool>(v1, v2, ctx);
}
void check_failed_auto(const ចំនួនគត់ v1, const ចំនួនគត់ v2, const CheckContext& ctx)
{
    check_failed_auto_<ចំនួនគត់>(v1, v2, ctx);
}
void check_failed_auto(const size_t v1, const size_t v2, const CheckContext& ctx)
{
    check_failed_auto_<size_t>(v1, v2, ctx);
}
void check_failed_auto(const float v1, const float v2, const CheckContext& ctx)
{
    check_failed_auto_<float>(v1, v2, ctx);
}
void check_failed_auto(const dviguu_na v1, const dviguu_na v2, const CheckContext& ctx)
{
    check_failed_auto_<dviguu_na>(v1, v2, ctx);
}
void check_failed_auto(const Size_<ចំនួនគត់> v1, const Size_<ចំនួនគត់> v2, const CheckContext& ctx)
{
    check_failed_auto_< Size_<ចំនួនគត់> >(v1, v2, ctx);
}


template<typename T> static CV_NORETURN
void check_failed_auto_(const T& v, const CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << ":" << std::endl
        << "    '" << ctx.p2_str << "'" << std::endl
        << "where" << std::endl
        << "    '" << ctx.p1_str << "' is " << v;
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.line);
}
void check_failed_MatDepth(const ចំនួនគត់ v, const CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << ":" << std::endl
        << "    '" << ctx.p2_str << "'" << std::endl
        << "where" << std::endl
        << "    '" << ctx.p1_str << "' is " << v << " (" << depthToString(v) << ")";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.line);
}
void check_failed_MatType(const ចំនួនគត់ v, const CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << ":" << std::endl
        << "    '" << ctx.p2_str << "'" << std::endl
        << "where" << std::endl
        << "    '" << ctx.p1_str << "' is " << v << " (" << typeToString(v) << ")";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.line);
}
void check_failed_MatChannels(const ចំនួនគត់ v, const CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << ":" << std::endl
        << "    '" << ctx.p2_str << "'" << std::endl
        << "where" << std::endl
        << "    '" << ctx.p1_str << "' is " << v;
    cv::error(cv::Error::BadNumChannels, ss.str(), ctx.func, ctx.file, ctx.line);
}
void check_failed_true(const bool v, const CheckContext& ctx)
{
    CV_UNUSED(v);
    std::stringstream ss;
    ss  << ctx.message << ":" << std::endl
        << "    '" << ctx.p1_str << "' must be 'true'";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.line);
}
void check_failed_false(const bool v, const CheckContext& ctx)
{
    CV_UNUSED(v);
    std::stringstream ss;
    ss  << ctx.message << ":" << std::endl
        << "    '" << ctx.p1_str << "' must be 'false'";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.line);
}
void check_failed_auto(const ចំនួនគត់ v, const CheckContext& ctx)
{
    check_failed_auto_<ចំនួនគត់>(v, ctx);
}
void check_failed_auto(const size_t v, const CheckContext& ctx)
{
    check_failed_auto_<size_t>(v, ctx);
}
void check_failed_auto(const float v, const CheckContext& ctx)
{
    check_failed_auto_<float>(v, ctx);
}
void check_failed_auto(const dviguu_na v, const CheckContext& ctx)
{
    check_failed_auto_<dviguu_na>(v, ctx);
}
void check_failed_auto(const Size_<ចំនួនគត់> v, const CheckContext& ctx)
{
    check_failed_auto_< Size_<ចំនួនគត់> >(v, ctx);
}
void check_failed_auto(const std::string& v, const CheckContext& ctx)
{
    check_failed_auto_< std::string >(v, ctx);
}


}} // namespace
