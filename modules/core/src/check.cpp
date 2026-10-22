// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#شامل "precomp.hpp"

#شامل <sstream>

#شامل "opencv2/core/check.hpp"

نامخلا cv {

ثابت حرف* گہرائی_مقصد_String(صحیح گہرائی)
{
    ثابت حرف* s = detail::گہرائی_مقصد_String_2(گہرائی);
    واپس s ? s : "<invalid depth>";
}

cv::String type_مقصد_String(صحیح type)
{
    cv::String s = detail::type_مقصد_String_2(type);
    اگر (s.empty())
    {
        ساکن cv::String invalidType("<invalid type>");
        واپس invalidType;
    }
    واپس s;
}


نامخلا detail {

ساکن ثابت حرف* getTestOpPhraseStr(غیرعلامتی testOp)
{
    ساکن ثابت حرف* نام[] = { "{custom check}", "equal to", "not equal to", "less than or equal to", "less than", "greater than or equal to", "greater than" };
    CV_DbgAssert(testOp < CV_آخری_TEST_OP);
    واپس testOp < CV_آخری_TEST_OP ? نام[testOp] : "???";
}
ساکن ثابت حرف* getTestOpMath(غیرعلامتی testOp)
{
    ساکن ثابت حرف* نام[] = { "???", "==", "!=", "<=", "<", ">=", ">" };
    CV_DbgAssert(testOp < CV_آخری_TEST_OP);
    واپس testOp < CV_آخری_TEST_OP ? نام[testOp] : "???";
}

ثابت حرف* گہرائی_مقصد_String_2(صحیح گہرائی)
{
    ساکن ثابت حرف* گہرائی_نام[] = { "CV_8U", "CV_8S", "CV_16U", "CV_16S", "CV_32S", "CV_32F", "CV_64F", "CV_16F",
                                        "CV_16BF", "CV_Bool", "CV_64U", "CV_64S", "CV_32U",
                                        "CV_8F", "CV_8FNUZ" };
    واپس (گہرائی < CV_گہرائی_CURR_MAX && گہرائی >= 0) ? گہرائی_نام[گہرائی] : تہی;
}

cv::String type_مقصد_String_2(صحیح type)
{
    صحیح گہرائی = CV_MAT_گہرائی(type);
    صحیح cn = CV_MAT_CN(type);
    اگر (گہرائی >= 0 && گہرائی < CV_گہرائی_CURR_MAX)
        واپس cv::format("%sC%d", گہرائی_مقصد_String_2(گہرائی), cn);
    واپس cv::String();
}

سانچہ<قسم_نام T> ساکن CV_NORETURN
خالی check_failed_auto_(ثابت T& v1, ثابت T& v2, ثابت CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << " (expected: '" << ctx.p1_str << " " << getTestOpMath(ctx.testOp) << " " << ctx.p2_str << "'), where" << std::سطر_ختم
        << "    '" << ctx.p1_str << "' is " << v1 << std::سطر_ختم;
    اگر (ctx.testOp != TEST_CUSTOM && ctx.testOp < CV_آخری_TEST_OP)
    {
        ss << "must be " << getTestOpPhraseStr(ctx.testOp) << std::سطر_ختم;
    }
    ss  << "    '" << ctx.p2_str << "' is " << v2;
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.سطر);
}
خالی check_failed_Mat_گہرائی(ثابت صحیح v1, ثابت صحیح v2, ثابت CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << " (expected: '" << ctx.p1_str << " " << getTestOpMath(ctx.testOp) << " " << ctx.p2_str << "'), where" << std::سطر_ختم
        << "    '" << ctx.p1_str << "' is " << v1 << " (" << گہرائی_مقصد_String(v1) << ")" << std::سطر_ختم;
    اگر (ctx.testOp != TEST_CUSTOM && ctx.testOp < CV_آخری_TEST_OP)
    {
        ss << "must be " << getTestOpPhraseStr(ctx.testOp) << std::سطر_ختم;
    }
    ss  << "    '" << ctx.p2_str << "' is " << v2 << " (" << گہرائی_مقصد_String(v2) << ")";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.سطر);
}
خالی check_failed_MatType(ثابت صحیح v1, ثابت صحیح v2, ثابت CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << " (expected: '" << ctx.p1_str << " " << getTestOpMath(ctx.testOp) << " " << ctx.p2_str << "'), where" << std::سطر_ختم
        << "    '" << ctx.p1_str << "' is " << v1 << " (" << type_مقصد_String(v1) << ")" << std::سطر_ختم;
    اگر (ctx.testOp != TEST_CUSTOM && ctx.testOp < CV_آخری_TEST_OP)
    {
        ss << "must be " << getTestOpPhraseStr(ctx.testOp) << std::سطر_ختم;
    }
    ss  << "    '" << ctx.p2_str << "' is " << v2 << " (" << type_مقصد_String(v2) << ")";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.سطر);
}
خالی check_failed_MatChannels(ثابت صحیح v1, ثابت صحیح v2, ثابت CheckContext& ctx)
{
    check_failed_auto_<صحیح>(v1, v2, ctx);
}
خالی check_failed_auto(ثابت منطقی v1, ثابت منطقی v2, ثابت CheckContext& ctx)
{
    check_failed_auto_<منطقی>(v1, v2, ctx);
}
خالی check_failed_auto(ثابت صحیح v1, ثابت صحیح v2, ثابت CheckContext& ctx)
{
    check_failed_auto_<صحیح>(v1, v2, ctx);
}
خالی check_failed_auto(ثابت size_t v1, ثابت size_t v2, ثابت CheckContext& ctx)
{
    check_failed_auto_<size_t>(v1, v2, ctx);
}
خالی check_failed_auto(ثابت اعشاری v1, ثابت اعشاری v2, ثابت CheckContext& ctx)
{
    check_failed_auto_<اعشاری>(v1, v2, ctx);
}
خالی check_failed_auto(ثابت دوہرا v1, ثابت دوہرا v2, ثابت CheckContext& ctx)
{
    check_failed_auto_<دوہرا>(v1, v2, ctx);
}
خالی check_failed_auto(ثابت سائز_2<صحیح> v1, ثابت سائز_2<صحیح> v2, ثابت CheckContext& ctx)
{
    check_failed_auto_< سائز_2<صحیح> >(v1, v2, ctx);
}


سانچہ<قسم_نام T> ساکن CV_NORETURN
خالی check_failed_auto_(ثابت T& v, ثابت CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << ":" << std::سطر_ختم
        << "    '" << ctx.p2_str << "'" << std::سطر_ختم
        << "where" << std::سطر_ختم
        << "    '" << ctx.p1_str << "' is " << v;
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.سطر);
}
خالی check_failed_Mat_گہرائی(ثابت صحیح v, ثابت CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << ":" << std::سطر_ختم
        << "    '" << ctx.p2_str << "'" << std::سطر_ختم
        << "where" << std::سطر_ختم
        << "    '" << ctx.p1_str << "' is " << v << " (" << گہرائی_مقصد_String(v) << ")";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.سطر);
}
خالی check_failed_MatType(ثابت صحیح v, ثابت CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << ":" << std::سطر_ختم
        << "    '" << ctx.p2_str << "'" << std::سطر_ختم
        << "where" << std::سطر_ختم
        << "    '" << ctx.p1_str << "' is " << v << " (" << type_مقصد_String(v) << ")";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.سطر);
}
خالی check_failed_MatChannels(ثابت صحیح v, ثابت CheckContext& ctx)
{
    std::stringstream ss;
    ss  << ctx.message << ":" << std::سطر_ختم
        << "    '" << ctx.p2_str << "'" << std::سطر_ختم
        << "where" << std::سطر_ختم
        << "    '" << ctx.p1_str << "' is " << v;
    cv::error(cv::Error::Bad_عدد_Channels, ss.str(), ctx.func, ctx.file, ctx.سطر);
}
خالی check_failed_true(ثابت منطقی v, ثابت CheckContext& ctx)
{
    CV_غیر_مستعمل(v);
    std::stringstream ss;
    ss  << ctx.message << ":" << std::سطر_ختم
        << "    '" << ctx.p1_str << "' must be 'true'";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.سطر);
}
خالی check_failed_false(ثابت منطقی v, ثابت CheckContext& ctx)
{
    CV_غیر_مستعمل(v);
    std::stringstream ss;
    ss  << ctx.message << ":" << std::سطر_ختم
        << "    '" << ctx.p1_str << "' must be 'false'";
    cv::error(cv::Error::StsError, ss.str(), ctx.func, ctx.file, ctx.سطر);
}
خالی check_failed_auto(ثابت صحیح v, ثابت CheckContext& ctx)
{
    check_failed_auto_<صحیح>(v, ctx);
}
خالی check_failed_auto(ثابت size_t v, ثابت CheckContext& ctx)
{
    check_failed_auto_<size_t>(v, ctx);
}
خالی check_failed_auto(ثابت اعشاری v, ثابت CheckContext& ctx)
{
    check_failed_auto_<اعشاری>(v, ctx);
}
خالی check_failed_auto(ثابت دوہرا v, ثابت CheckContext& ctx)
{
    check_failed_auto_<دوہرا>(v, ctx);
}
خالی check_failed_auto(ثابت سائز_2<صحیح> v, ثابت CheckContext& ctx)
{
    check_failed_auto_< سائز_2<صحیح> >(v, ctx);
}
خالی check_failed_auto(ثابت std::string& v, ثابت CheckContext& ctx)
{
    check_failed_auto_< std::string >(v, ctx);
}


}} // namespace
