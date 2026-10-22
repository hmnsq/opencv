// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#అన్తర్భావ "precomp.hpp"
#అన్తర్భావ "opencv2/core/bindings_utils.hpp"
#అన్తర్భావ <sstream>
#అన్తర్భావ <iomanip>
#అన్తర్భావ <opencv2/core/utils/filesystem.hpp>
#అన్తర్భావ <opencv2/core/utils/filesystem.private.hpp>

నామాకాశ cv {
స్థిర అంతఃవరుస std::ostream& క్రియాకారకం<<(std::ostream& os, స్థిరరాశి Rect& rect)
{
    తిరిగి os << "[x=" << rect.x << ", y=" << rect.y << ", w=" << rect.వెడల్పు << ", h=" << rect.ఎత్తు << ']';
}
నామాకాశ utils {

String dump_ఆదానం_Array(ఆదానం_Array argument)
{
    ఒకవేళ (&argument == &noArray())
        తిరిగి "InputArray: noArray()";
    std::ostringstream ss;
    ss << "InputArray:";
    ప్రయత్నించు {
        చేయి {
            ss << (argument.empty() ? " empty()=true" : " empty()=false");
            ss << cv::format(" kind=0x%08llx", (దీర్ఘ దీర్ఘ పూర్ణాంకం)argument.రకం());
            ss << cv::format(" flags=0x%08llx", (దీర్ఘ దీర్ఘ పూర్ణాంకం)argument.getFlags());
            ఒకవేళ (argument.getObj() == ఖాళీ)
            {
                ss << " obj=NULL";
                ఆపు; //పూర్తి
            }
            ss << cv::format(" total(-1)=%lld", (దీర్ఘ దీర్ఘ పూర్ణాంకం)argument.మొత్తం(-1));
            పూర్ణాంకం dims = argument.dims(-1);
            ss << cv::format(" dims(-1)=%d", dims);
            ఒకవేళ (dims <= 2)
            {
                కొలత size = argument.size(-1);
                ss << cv::format(" size(-1)=%dx%d", size.వెడల్పు, size.ఎత్తు);
            }
            లేకపోతే
            {
                పూర్ణాంకం sz[CV_MAX_DIM] = {0};
                argument.sizend(sz, -1);
                ss << " size(-1)=[";
                కొరకు (పూర్ణాంకం i = 0; i < dims; i++)
                {
                    ఒకవేళ (i > 0)
                        ss << ' ';
                    ss << sz[i];
                }
                ss << "]";
            }
            ss << " type(-1)=" << cv::type_గమ్యం_String(argument.type(-1));
        } అయితే (0);
    }
    పట్టుకో (స్థిరరాశి std::exception& e)
    {
        ss << " ERROR: exception occurred: " << e.what();
    }
    పట్టుకో (...)
    {
        ss << " ERROR: unknown exception occurred, dump is non-complete";
    }
    తిరిగి ss.str();
}

CV_EXPORTS_W String dump_ఆదానం_Array_యొక్క_Arrays(ఆదానం_Array_యొక్క_Arrays argument)
{
    ఒకవేళ (&argument == &noArray())
        తిరిగి "InputArrayOfArrays: noArray()";
    std::ostringstream ss;
    ss << "InputArrayOfArrays:";
    ప్రయత్నించు {
        చేయి {
            ss << (argument.empty() ? " empty()=true" : " empty()=false");
            ss << cv::format(" kind=0x%08llx", (దీర్ఘ దీర్ఘ పూర్ణాంకం)argument.రకం());
            ss << cv::format(" flags=0x%08llx", (దీర్ఘ దీర్ఘ పూర్ణాంకం)argument.getFlags());
            ఒకవేళ (argument.getObj() == ఖాళీ)
            {
                ss << " obj=NULL";
                ఆపు; //పూర్తి
            }
            ss << cv::format(" total(-1)=%lld", (దీర్ఘ దీర్ఘ పూర్ణాంకం)argument.మొత్తం(-1));
            ss << cv::format(" dims(-1)=%d", argument.dims(-1));
            కొలత size = argument.size(-1);
            ss << cv::format(" size(-1)=%dx%d", size.వెడల్పు, size.ఎత్తు);
            ఒకవేళ (argument.మొత్తం(-1) > 0)
            {
                ss << " type(0)=" << cv::type_గమ్యం_String(argument.type(0));
                పూర్ణాంకం dims = argument.dims(0);
                ss << cv::format(" dims(0)=%d", dims);
                ఒకవేళ (dims <= 2)
                {
                    కొలత size0 = argument.size(0);
                    ss << cv::format(" size(0)=%dx%d", size0.వెడల్పు, size0.ఎత్తు);
                }
                లేకపోతే
                {
                    పూర్ణాంకం sz[CV_MAX_DIM] = {0};
                    argument.sizend(sz, 0);
                    ss << " size(0)=[";
                    కొరకు (పూర్ణాంకం i = 0; i < dims; i++)
                    {
                        ఒకవేళ (i > 0)
                            ss << ' ';
                        ss << sz[i];
                    }
                    ss << "]";
                }
            }
        } అయితే (0);
    }
    పట్టుకో (స్థిరరాశి std::exception& e)
    {
        ss << " ERROR: exception occurred: " << e.what();
    }
    పట్టుకో (...)
    {
        ss << " ERROR: unknown exception occurred, dump is non-complete";
    }
    తిరిగి ss.str();
}

CV_EXPORTS_W String dump_ఆదానం_Output_Array(ఆదానం_Output_Array argument)
{
    ఒకవేళ (&argument == &noArray())
        తిరిగి "InputOutputArray: noArray()";
    std::ostringstream ss;
    ss << "InputOutputArray:";
    ప్రయత్నించు {
        చేయి {
            ss << (argument.empty() ? " empty()=true" : " empty()=false");
            ss << cv::format(" kind=0x%08llx", (దీర్ఘ దీర్ఘ పూర్ణాంకం)argument.రకం());
            ss << cv::format(" flags=0x%08llx", (దీర్ఘ దీర్ఘ పూర్ణాంకం)argument.getFlags());
            ఒకవేళ (argument.getObj() == ఖాళీ)
            {
                ss << " obj=NULL";
                ఆపు; //పూర్తి
            }
            ss << cv::format(" total(-1)=%lld", (దీర్ఘ దీర్ఘ పూర్ణాంకం)argument.మొత్తం(-1));
            పూర్ణాంకం dims = argument.dims(-1);
            ss << cv::format(" dims(-1)=%d", dims);
            ఒకవేళ (dims <= 2)
            {
                కొలత size = argument.size(-1);
                ss << cv::format(" size(-1)=%dx%d", size.వెడల్పు, size.ఎత్తు);
            }
            లేకపోతే
            {
                పూర్ణాంకం sz[CV_MAX_DIM] = {0};
                argument.sizend(sz, -1);
                ss << " size(-1)=[";
                కొరకు (పూర్ణాంకం i = 0; i < dims; i++)
                {
                    ఒకవేళ (i > 0)
                        ss << ' ';
                    ss << sz[i];
                }
                ss << "]";
            }
            ss << " type(-1)=" << cv::type_గమ్యం_String(argument.type(-1));
        } అయితే (0);
    }
    పట్టుకో (స్థిరరాశి std::exception& e)
    {
        ss << " ERROR: exception occurred: " << e.what();
    }
    పట్టుకో (...)
    {
        ss << " ERROR: unknown exception occurred, dump is non-complete";
    }
    తిరిగి ss.str();
}

CV_EXPORTS_W String dump_ఆదానం_Output_Array_యొక్క_Arrays(ఆదానం_Output_Array_యొక్క_Arrays argument)
{
    ఒకవేళ (&argument == &noArray())
        తిరిగి "InputOutputArrayOfArrays: noArray()";
    std::ostringstream ss;
    ss << "InputOutputArrayOfArrays:";
    ప్రయత్నించు {
        చేయి {
            ss << (argument.empty() ? " empty()=true" : " empty()=false");
            ss << cv::format(" kind=0x%08llx", (దీర్ఘ దీర్ఘ పూర్ణాంకం)argument.రకం());
            ss << cv::format(" flags=0x%08llx", (దీర్ఘ దీర్ఘ పూర్ణాంకం)argument.getFlags());
            ఒకవేళ (argument.getObj() == ఖాళీ)
            {
                ss << " obj=NULL";
                ఆపు; //పూర్తి
            }
            ss << cv::format(" total(-1)=%lld", (దీర్ఘ దీర్ఘ పూర్ణాంకం)argument.మొత్తం(-1));
            ss << cv::format(" dims(-1)=%d", argument.dims(-1));
            కొలత size = argument.size(-1);
            ss << cv::format(" size(-1)=%dx%d", size.వెడల్పు, size.ఎత్తు);
            ఒకవేళ (argument.మొత్తం(-1) > 0)
            {
                ss << " type(0)=" << cv::type_గమ్యం_String(argument.type(0));
                పూర్ణాంకం dims = argument.dims(0);
                ss << cv::format(" dims(0)=%d", dims);
                ఒకవేళ (dims <= 2)
                {
                    కొలత size0 = argument.size(0);
                    ss << cv::format(" size(0)=%dx%d", size0.వెడల్పు, size0.ఎత్తు);
                }
                లేకపోతే
                {
                    పూర్ణాంకం sz[CV_MAX_DIM] = {0};
                    argument.sizend(sz, 0);
                    ss << " size(0)=[";
                    కొరకు (పూర్ణాంకం i = 0; i < dims; i++)
                    {
                        ఒకవేళ (i > 0)
                            ss << ' ';
                        ss << sz[i];
                    }
                    ss << "]";
                }
            }
        } అయితే (0);
    }
    పట్టుకో (స్థిరరాశి std::exception& e)
    {
        ss << " ERROR: exception occurred: " << e.what();
    }
    పట్టుకో (...)
    {
        ss << " ERROR: unknown exception occurred, dump is non-complete";
    }
    తిరిగి ss.str();
}

మూస <తరగతి T, తరగతి Formatter>
స్థిర అంతఃవరుస String dump_సదిశ(స్థిరరాశి std::vector<T>& vec, Formatter format)
{
    std::ostringstream oss("[", std::ios::ate);
    ఒకవేళ (!vec.empty())
    {
        format(oss) << vec[0];
        కొరకు (std::size_t i = 1; i < vec.size(); ++i)
        {
            oss << ", ";
            format(oss) << vec[i];
        }
    }
    oss << "]";
    తిరిగి oss.str();
}

స్థిర అంతఃవరుస std::ostream& noFormat(std::ostream& os)
{
    తిరిగి os;
}

స్థిర అంతఃవరుస std::ostream& floatFormat(std::ostream& os)
{
    తిరిగి os << std::fixed << std::setprecision(2);
}

String dump_సదిశ_యొక్క_Int(స్థిరరాశి std::vector<పూర్ణాంకం>& vec)
{
    తిరిగి dump_సదిశ(vec, &noFormat);
}

String dump_సదిశ_యొక్క_Double(స్థిరరాశి std::vector<ద్విగుణ>& vec)
{
    తిరిగి dump_సదిశ(vec, &floatFormat);
}

String dump_సదిశ_యొక్క_Rect(స్థిరరాశి std::vector<Rect>& vec)
{
    తిరిగి dump_సదిశ(vec, &noFormat);
}


నామాకాశ fs {
cv::String getCacheDirectoryForDownloads()
{
#ఒకవేళ OPENCV_HAVE_FILESYSTEM_SUPPORT
    తిరిగి cv::utils::fs::getCacheDirectory("downloads", "OPENCV_DOWNLOADS_CACHE_DIR");
#లేకపోతే
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#అయితే_ముగింపు
}
} // namespace fs

}} // namespace
