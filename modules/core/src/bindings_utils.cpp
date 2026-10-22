// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#شامل "precomp.hpp"
#شامل "opencv2/core/bindings_utils.hpp"
#شامل <sstream>
#شامل <iomanip>
#شامل <opencv2/core/utils/filesystem.hpp>
#شامل <opencv2/core/utils/filesystem.private.hpp>

فضاینام cv {
ایستا درونخطی std::ostream& operator<<(std::ostream& os, ثابت Rect& rect)
{
    بازگشت os << "[x=" << rect.x << ", y=" << rect.y << ", w=" << rect.width << ", h=" << rect.height << ']';
}
فضاینام utils {

String dump_درون_داد_Array(درون_داد_Array argument)
{
    اگر (&argument == &noArray())
        بازگشت "InputArray: noArray()";
    std::ostringstream ss;
    ss << "InputArray:";
    تلاش {
        انجامبده {
            ss << (argument.empty() ? " empty()=true" : " empty()=false");
            ss << cv::format(" kind=0x%08llx", (long long صحیح)argument.kind());
            ss << cv::format(" flags=0x%08llx", (long long صحیح)argument.getFlags());
            اگر (argument.getObj() == پوچ)
            {
                ss << " obj=NULL";
                بشکن; // done
            }
            ss << cv::format(" total(-1)=%lld", (long long صحیح)argument.total(-1));
            صحیح dims = argument.dims(-1);
            ss << cv::format(" dims(-1)=%d", dims);
            اگر (dims <= 2)
            {
                Size size = argument.size(-1);
                ss << cv::format(" size(-1)=%dx%d", size.width, size.height);
            }
            وگرنه
            {
                صحیح sz[CV_MAX_DIM] = {0};
                argument.sizend(sz, -1);
                ss << " size(-1)=[";
                برای (صحیح i = 0; i < dims; i++)
                {
                    اگر (i > 0)
                        ss << ' ';
                    ss << sz[i];
                }
                ss << "]";
            }
            ss << " type(-1)=" << cv::typeToString(argument.type(-1));
        } وقتی (0);
    }
    بگیر (ثابت std::exception& e)
    {
        ss << " ERROR: exception occurred: " << e.what();
    }
    بگیر (...)
    {
        ss << " ERROR: unknown exception occurred, dump is non-complete";
    }
    بازگشت ss.str();
}

CV_EXPORTS_W String dump_درون_داد_Array_Of_Arrays(درون_داد_Array_Of_Arrays argument)
{
    اگر (&argument == &noArray())
        بازگشت "InputArrayOfArrays: noArray()";
    std::ostringstream ss;
    ss << "InputArrayOfArrays:";
    تلاش {
        انجامبده {
            ss << (argument.empty() ? " empty()=true" : " empty()=false");
            ss << cv::format(" kind=0x%08llx", (long long صحیح)argument.kind());
            ss << cv::format(" flags=0x%08llx", (long long صحیح)argument.getFlags());
            اگر (argument.getObj() == پوچ)
            {
                ss << " obj=NULL";
                بشکن; // done
            }
            ss << cv::format(" total(-1)=%lld", (long long صحیح)argument.total(-1));
            ss << cv::format(" dims(-1)=%d", argument.dims(-1));
            Size size = argument.size(-1);
            ss << cv::format(" size(-1)=%dx%d", size.width, size.height);
            اگر (argument.total(-1) > 0)
            {
                ss << " type(0)=" << cv::typeToString(argument.type(0));
                صحیح dims = argument.dims(0);
                ss << cv::format(" dims(0)=%d", dims);
                اگر (dims <= 2)
                {
                    Size size0 = argument.size(0);
                    ss << cv::format(" size(0)=%dx%d", size0.width, size0.height);
                }
                وگرنه
                {
                    صحیح sz[CV_MAX_DIM] = {0};
                    argument.sizend(sz, 0);
                    ss << " size(0)=[";
                    برای (صحیح i = 0; i < dims; i++)
                    {
                        اگر (i > 0)
                            ss << ' ';
                        ss << sz[i];
                    }
                    ss << "]";
                }
            }
        } وقتی (0);
    }
    بگیر (ثابت std::exception& e)
    {
        ss << " ERROR: exception occurred: " << e.what();
    }
    بگیر (...)
    {
        ss << " ERROR: unknown exception occurred, dump is non-complete";
    }
    بازگشت ss.str();
}

CV_EXPORTS_W String dump_درون_داد_Output_Array(درون_داد_Output_Array argument)
{
    اگر (&argument == &noArray())
        بازگشت "InputOutputArray: noArray()";
    std::ostringstream ss;
    ss << "InputOutputArray:";
    تلاش {
        انجامبده {
            ss << (argument.empty() ? " empty()=true" : " empty()=false");
            ss << cv::format(" kind=0x%08llx", (long long صحیح)argument.kind());
            ss << cv::format(" flags=0x%08llx", (long long صحیح)argument.getFlags());
            اگر (argument.getObj() == پوچ)
            {
                ss << " obj=NULL";
                بشکن; // done
            }
            ss << cv::format(" total(-1)=%lld", (long long صحیح)argument.total(-1));
            صحیح dims = argument.dims(-1);
            ss << cv::format(" dims(-1)=%d", dims);
            اگر (dims <= 2)
            {
                Size size = argument.size(-1);
                ss << cv::format(" size(-1)=%dx%d", size.width, size.height);
            }
            وگرنه
            {
                صحیح sz[CV_MAX_DIM] = {0};
                argument.sizend(sz, -1);
                ss << " size(-1)=[";
                برای (صحیح i = 0; i < dims; i++)
                {
                    اگر (i > 0)
                        ss << ' ';
                    ss << sz[i];
                }
                ss << "]";
            }
            ss << " type(-1)=" << cv::typeToString(argument.type(-1));
        } وقتی (0);
    }
    بگیر (ثابت std::exception& e)
    {
        ss << " ERROR: exception occurred: " << e.what();
    }
    بگیر (...)
    {
        ss << " ERROR: unknown exception occurred, dump is non-complete";
    }
    بازگشت ss.str();
}

CV_EXPORTS_W String dump_درون_داد_Output_Array_Of_Arrays(درون_داد_Output_Array_Of_Arrays argument)
{
    اگر (&argument == &noArray())
        بازگشت "InputOutputArrayOfArrays: noArray()";
    std::ostringstream ss;
    ss << "InputOutputArrayOfArrays:";
    تلاش {
        انجامبده {
            ss << (argument.empty() ? " empty()=true" : " empty()=false");
            ss << cv::format(" kind=0x%08llx", (long long صحیح)argument.kind());
            ss << cv::format(" flags=0x%08llx", (long long صحیح)argument.getFlags());
            اگر (argument.getObj() == پوچ)
            {
                ss << " obj=NULL";
                بشکن; // done
            }
            ss << cv::format(" total(-1)=%lld", (long long صحیح)argument.total(-1));
            ss << cv::format(" dims(-1)=%d", argument.dims(-1));
            Size size = argument.size(-1);
            ss << cv::format(" size(-1)=%dx%d", size.width, size.height);
            اگر (argument.total(-1) > 0)
            {
                ss << " type(0)=" << cv::typeToString(argument.type(0));
                صحیح dims = argument.dims(0);
                ss << cv::format(" dims(0)=%d", dims);
                اگر (dims <= 2)
                {
                    Size size0 = argument.size(0);
                    ss << cv::format(" size(0)=%dx%d", size0.width, size0.height);
                }
                وگرنه
                {
                    صحیح sz[CV_MAX_DIM] = {0};
                    argument.sizend(sz, 0);
                    ss << " size(0)=[";
                    برای (صحیح i = 0; i < dims; i++)
                    {
                        اگر (i > 0)
                            ss << ' ';
                        ss << sz[i];
                    }
                    ss << "]";
                }
            }
        } وقتی (0);
    }
    بگیر (ثابت std::exception& e)
    {
        ss << " ERROR: exception occurred: " << e.what();
    }
    بگیر (...)
    {
        ss << " ERROR: unknown exception occurred, dump is non-complete";
    }
    بازگشت ss.str();
}

template <رده T, رده Formatter>
ایستا درونخطی String dumpVector(ثابت std::vector<T>& vec, Formatter format)
{
    std::ostringstream oss("[", std::ios::ate);
    اگر (!vec.empty())
    {
        format(oss) << vec[0];
        برای (std::size_t i = 1; i < vec.size(); ++i)
        {
            oss << ", ";
            format(oss) << vec[i];
        }
    }
    oss << "]";
    بازگشت oss.str();
}

ایستا درونخطی std::ostream& noFormat(std::ostream& os)
{
    بازگشت os;
}

ایستا درونخطی std::ostream& floatFormat(std::ostream& os)
{
    بازگشت os << std::fixed << std::setprecision(2);
}

String dumpVectorOfInt(ثابت std::vector<صحیح>& vec)
{
    بازگشت dumpVector(vec, &noFormat);
}

String dumpVectorOfDouble(ثابت std::vector<دوگانه>& vec)
{
    بازگشت dumpVector(vec, &floatFormat);
}

String dumpVectorOfRect(ثابت std::vector<Rect>& vec)
{
    بازگشت dumpVector(vec, &noFormat);
}


فضاینام fs {
cv::String getCacheDirectoryForDownloads()
{
#اگر OPENCV_HAVE_FILESYSTEM_SUPPORT
    بازگشت cv::utils::fs::getCacheDirectory("downloads", "OPENCV_DOWNLOADS_CACHE_DIR");
#وگرنه
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#endif
}
} // namespace fs

}} // namespace
