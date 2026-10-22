// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ਅਨ੍ਤਰ੍ਭਾਵ "precomp.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "opencv2/core/bindings_utils.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ <sstream>
#ਅਨ੍ਤਰ੍ਭਾਵ <iomanip>
#ਅਨ੍ਤਰ੍ਭਾਵ <opencv2/core/utils/filesystem.hpp>
#ਅਨ੍ਤਰ੍ਭਾਵ <opencv2/core/utils/filesystem.private.hpp>

ਨਾਮਾਕਾਸ਼ cv {
ਸਥਿਰ ਸਤਰਅੰਦਰ std::ostream& ਸੰਚਾਲਕ<<(std::ostream& os, ਅਚਲ Rect& rect)
{
    ਵਾਪਸ os << "[x=" << rect.x << ", y=" << rect.y << ", w=" << rect.ਚੌੜਾਈ << ", h=" << rect.ਉਚਾਈ << ']';
}
ਨਾਮਾਕਾਸ਼ utils {

String dump_ਨਿਵੇਸ਼_Array(ਨਿਵੇਸ਼_Array argument)
{
    ਜੇ (&argument == &noArray())
        ਵਾਪਸ "InputArray: noArray()";
    std::ostringstream ss;
    ss << "InputArray:";
    ਕੋਸ਼ਿਸ਼ {
        ਕਰੋ {
            ss << (argument.empty() ? " empty()=true" : " empty()=false");
            ss << cv::format(" kind=0x%08llx", (ਲੰਮਾ ਲੰਮਾ ਪੂਰਨ)argument.ਕਿਸਮ());
            ss << cv::format(" flags=0x%08llx", (ਲੰਮਾ ਲੰਮਾ ਪੂਰਨ)argument.getFlags());
            ਜੇ (argument.getObj() == ਨਿਰਾ)
            {
                ss << " obj=NULL";
                ਰੋਕੋ; //ਪੂਰਾ
            }
            ss << cv::format(" total(-1)=%lld", (ਲੰਮਾ ਲੰਮਾ ਪੂਰਨ)argument.ਕੁੱਲ(-1));
            ਪੂਰਨ dims = argument.dims(-1);
            ss << cv::format(" dims(-1)=%d", dims);
            ਜੇ (dims <= 2)
            {
                ਮਾਪ size = argument.size(-1);
                ss << cv::format(" size(-1)=%dx%d", size.ਚੌੜਾਈ, size.ਉਚਾਈ);
            }
            ਨਹੀਂਤਾਂ
            {
                ਪੂਰਨ sz[CV_MAX_DIM] = {0};
                argument.sizend(sz, -1);
                ss << " size(-1)=[";
                ਲਈ (ਪੂਰਨ i = 0; i < dims; i++)
                {
                    ਜੇ (i > 0)
                        ss << ' ';
                    ss << sz[i];
                }
                ss << "]";
            }
            ss << " type(-1)=" << cv::type_ਮੰਜ਼ਿਲ_String(argument.type(-1));
        } ਜਦੋਂ (0);
    }
    ਫੜੋ (ਅਚਲ std::exception& e)
    {
        ss << " ERROR: exception occurred: " << e.what();
    }
    ਫੜੋ (...)
    {
        ss << " ERROR: unknown exception occurred, dump is non-complete";
    }
    ਵਾਪਸ ss.str();
}

CV_EXPORTS_W String dump_ਨਿਵੇਸ਼_Array_ਦਾ_Arrays(ਨਿਵੇਸ਼_Array_ਦਾ_Arrays argument)
{
    ਜੇ (&argument == &noArray())
        ਵਾਪਸ "InputArrayOfArrays: noArray()";
    std::ostringstream ss;
    ss << "InputArrayOfArrays:";
    ਕੋਸ਼ਿਸ਼ {
        ਕਰੋ {
            ss << (argument.empty() ? " empty()=true" : " empty()=false");
            ss << cv::format(" kind=0x%08llx", (ਲੰਮਾ ਲੰਮਾ ਪੂਰਨ)argument.ਕਿਸਮ());
            ss << cv::format(" flags=0x%08llx", (ਲੰਮਾ ਲੰਮਾ ਪੂਰਨ)argument.getFlags());
            ਜੇ (argument.getObj() == ਨਿਰਾ)
            {
                ss << " obj=NULL";
                ਰੋਕੋ; //ਪੂਰਾ
            }
            ss << cv::format(" total(-1)=%lld", (ਲੰਮਾ ਲੰਮਾ ਪੂਰਨ)argument.ਕੁੱਲ(-1));
            ss << cv::format(" dims(-1)=%d", argument.dims(-1));
            ਮਾਪ size = argument.size(-1);
            ss << cv::format(" size(-1)=%dx%d", size.ਚੌੜਾਈ, size.ਉਚਾਈ);
            ਜੇ (argument.ਕੁੱਲ(-1) > 0)
            {
                ss << " type(0)=" << cv::type_ਮੰਜ਼ਿਲ_String(argument.type(0));
                ਪੂਰਨ dims = argument.dims(0);
                ss << cv::format(" dims(0)=%d", dims);
                ਜੇ (dims <= 2)
                {
                    ਮਾਪ size0 = argument.size(0);
                    ss << cv::format(" size(0)=%dx%d", size0.ਚੌੜਾਈ, size0.ਉਚਾਈ);
                }
                ਨਹੀਂਤਾਂ
                {
                    ਪੂਰਨ sz[CV_MAX_DIM] = {0};
                    argument.sizend(sz, 0);
                    ss << " size(0)=[";
                    ਲਈ (ਪੂਰਨ i = 0; i < dims; i++)
                    {
                        ਜੇ (i > 0)
                            ss << ' ';
                        ss << sz[i];
                    }
                    ss << "]";
                }
            }
        } ਜਦੋਂ (0);
    }
    ਫੜੋ (ਅਚਲ std::exception& e)
    {
        ss << " ERROR: exception occurred: " << e.what();
    }
    ਫੜੋ (...)
    {
        ss << " ERROR: unknown exception occurred, dump is non-complete";
    }
    ਵਾਪਸ ss.str();
}

CV_EXPORTS_W String dump_ਨਿਵੇਸ਼_Output_Array(ਨਿਵੇਸ਼_Output_Array argument)
{
    ਜੇ (&argument == &noArray())
        ਵਾਪਸ "InputOutputArray: noArray()";
    std::ostringstream ss;
    ss << "InputOutputArray:";
    ਕੋਸ਼ਿਸ਼ {
        ਕਰੋ {
            ss << (argument.empty() ? " empty()=true" : " empty()=false");
            ss << cv::format(" kind=0x%08llx", (ਲੰਮਾ ਲੰਮਾ ਪੂਰਨ)argument.ਕਿਸਮ());
            ss << cv::format(" flags=0x%08llx", (ਲੰਮਾ ਲੰਮਾ ਪੂਰਨ)argument.getFlags());
            ਜੇ (argument.getObj() == ਨਿਰਾ)
            {
                ss << " obj=NULL";
                ਰੋਕੋ; //ਪੂਰਾ
            }
            ss << cv::format(" total(-1)=%lld", (ਲੰਮਾ ਲੰਮਾ ਪੂਰਨ)argument.ਕੁੱਲ(-1));
            ਪੂਰਨ dims = argument.dims(-1);
            ss << cv::format(" dims(-1)=%d", dims);
            ਜੇ (dims <= 2)
            {
                ਮਾਪ size = argument.size(-1);
                ss << cv::format(" size(-1)=%dx%d", size.ਚੌੜਾਈ, size.ਉਚਾਈ);
            }
            ਨਹੀਂਤਾਂ
            {
                ਪੂਰਨ sz[CV_MAX_DIM] = {0};
                argument.sizend(sz, -1);
                ss << " size(-1)=[";
                ਲਈ (ਪੂਰਨ i = 0; i < dims; i++)
                {
                    ਜੇ (i > 0)
                        ss << ' ';
                    ss << sz[i];
                }
                ss << "]";
            }
            ss << " type(-1)=" << cv::type_ਮੰਜ਼ਿਲ_String(argument.type(-1));
        } ਜਦੋਂ (0);
    }
    ਫੜੋ (ਅਚਲ std::exception& e)
    {
        ss << " ERROR: exception occurred: " << e.what();
    }
    ਫੜੋ (...)
    {
        ss << " ERROR: unknown exception occurred, dump is non-complete";
    }
    ਵਾਪਸ ss.str();
}

CV_EXPORTS_W String dump_ਨਿਵੇਸ਼_Output_Array_ਦਾ_Arrays(ਨਿਵੇਸ਼_Output_Array_ਦਾ_Arrays argument)
{
    ਜੇ (&argument == &noArray())
        ਵਾਪਸ "InputOutputArrayOfArrays: noArray()";
    std::ostringstream ss;
    ss << "InputOutputArrayOfArrays:";
    ਕੋਸ਼ਿਸ਼ {
        ਕਰੋ {
            ss << (argument.empty() ? " empty()=true" : " empty()=false");
            ss << cv::format(" kind=0x%08llx", (ਲੰਮਾ ਲੰਮਾ ਪੂਰਨ)argument.ਕਿਸਮ());
            ss << cv::format(" flags=0x%08llx", (ਲੰਮਾ ਲੰਮਾ ਪੂਰਨ)argument.getFlags());
            ਜੇ (argument.getObj() == ਨਿਰਾ)
            {
                ss << " obj=NULL";
                ਰੋਕੋ; //ਪੂਰਾ
            }
            ss << cv::format(" total(-1)=%lld", (ਲੰਮਾ ਲੰਮਾ ਪੂਰਨ)argument.ਕੁੱਲ(-1));
            ss << cv::format(" dims(-1)=%d", argument.dims(-1));
            ਮਾਪ size = argument.size(-1);
            ss << cv::format(" size(-1)=%dx%d", size.ਚੌੜਾਈ, size.ਉਚਾਈ);
            ਜੇ (argument.ਕੁੱਲ(-1) > 0)
            {
                ss << " type(0)=" << cv::type_ਮੰਜ਼ਿਲ_String(argument.type(0));
                ਪੂਰਨ dims = argument.dims(0);
                ss << cv::format(" dims(0)=%d", dims);
                ਜੇ (dims <= 2)
                {
                    ਮਾਪ size0 = argument.size(0);
                    ss << cv::format(" size(0)=%dx%d", size0.ਚੌੜਾਈ, size0.ਉਚਾਈ);
                }
                ਨਹੀਂਤਾਂ
                {
                    ਪੂਰਨ sz[CV_MAX_DIM] = {0};
                    argument.sizend(sz, 0);
                    ss << " size(0)=[";
                    ਲਈ (ਪੂਰਨ i = 0; i < dims; i++)
                    {
                        ਜੇ (i > 0)
                            ss << ' ';
                        ss << sz[i];
                    }
                    ss << "]";
                }
            }
        } ਜਦੋਂ (0);
    }
    ਫੜੋ (ਅਚਲ std::exception& e)
    {
        ss << " ERROR: exception occurred: " << e.what();
    }
    ਫੜੋ (...)
    {
        ss << " ERROR: unknown exception occurred, dump is non-complete";
    }
    ਵਾਪਸ ss.str();
}

ਸਾਂਚਾ <ਵਰਗ T, ਵਰਗ Formatter>
ਸਥਿਰ ਸਤਰਅੰਦਰ String dump_ਸਦਿਸ਼(ਅਚਲ std::vector<T>& vec, Formatter format)
{
    std::ostringstream oss("[", std::ios::ate);
    ਜੇ (!vec.empty())
    {
        format(oss) << vec[0];
        ਲਈ (std::size_t i = 1; i < vec.size(); ++i)
        {
            oss << ", ";
            format(oss) << vec[i];
        }
    }
    oss << "]";
    ਵਾਪਸ oss.str();
}

ਸਥਿਰ ਸਤਰਅੰਦਰ std::ostream& noFormat(std::ostream& os)
{
    ਵਾਪਸ os;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ std::ostream& floatFormat(std::ostream& os)
{
    ਵਾਪਸ os << std::fixed << std::setprecision(2);
}

String dump_ਸਦਿਸ਼_ਦਾ_Int(ਅਚਲ std::vector<ਪੂਰਨ>& vec)
{
    ਵਾਪਸ dump_ਸਦਿਸ਼(vec, &noFormat);
}

String dump_ਸਦਿਸ਼_ਦਾ_Double(ਅਚਲ std::vector<ਦੁੱਗਣਾ>& vec)
{
    ਵਾਪਸ dump_ਸਦਿਸ਼(vec, &floatFormat);
}

String dump_ਸਦਿਸ਼_ਦਾ_Rect(ਅਚਲ std::vector<Rect>& vec)
{
    ਵਾਪਸ dump_ਸਦਿਸ਼(vec, &noFormat);
}


ਨਾਮਾਕਾਸ਼ fs {
cv::String getCacheDirectoryForDownloads()
{
#ਜੇ OPENCV_HAVE_FILESYSTEM_SUPPORT
    ਵਾਪਸ cv::utils::fs::getCacheDirectory("downloads", "OPENCV_DOWNLOADS_CACHE_DIR");
#ਨਹੀਂਤਾਂ
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#ਜੇ_ਅੰਤ
}
} // namespace fs

}} // namespace
