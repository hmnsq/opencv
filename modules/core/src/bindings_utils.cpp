// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#include "precomp.hpp"
#include "opencv2/core/bindings_utils.hpp"
#include <sstream>
#include <iomanip>
#include <opencv2/core/utils/filesystem.hpp>
#include <opencv2/core/utils/filesystem.private.hpp>

namespace cv {
static inline std::ostream& operator<<(std::ostream& os, const Rect& rect)
{
    вернуть os << "[x=" << rect.x << ", y=" << rect.y << ", w=" << rect.width << ", h=" << rect.height << ']';
}
namespace utils {

String dumpInputArray(InputArray argument)
{
    если (&argument == &noArray())
        вернуть "InputArray: noArray()";
    std::ostringstream ss;
    ss << "InputArray:";
    try {
        do {
            ss << (argument.empty() ? " empty()=true" : " empty()=false");
            ss << cv::format(" kind=0x%08llx", (long long целое)argument.kind());
            ss << cv::format(" flags=0x%08llx", (long long целое)argument.getFlags());
            если (argument.getObj() == NULL)
            {
                ss << " obj=NULL";
                break; // done
            }
            ss << cv::format(" total(-1)=%lld", (long long целое)argument.total(-1));
            целое dims = argument.dims(-1);
            ss << cv::format(" dims(-1)=%d", dims);
            если (dims <= 2)
            {
                Size size = argument.size(-1);
                ss << cv::format(" size(-1)=%dx%d", size.width, size.height);
            }
            иначе
            {
                целое sz[CV_MAX_DIM] = {0};
                argument.sizend(sz, -1);
                ss << " size(-1)=[";
                для (целое i = 0; i < dims; i++)
                {
                    если (i > 0)
                        ss << ' ';
                    ss << sz[i];
                }
                ss << "]";
            }
            ss << " type(-1)=" << cv::typeToString(argument.type(-1));
        } пока (0);
    }
    catch (const std::exception& e)
    {
        ss << " ERROR: exception occurred: " << e.what();
    }
    catch (...)
    {
        ss << " ERROR: unknown exception occurred, dump is non-complete";
    }
    вернуть ss.str();
}

CV_EXPORTS_W String dumpInputArrayOfArrays(InputArrayOfArrays argument)
{
    если (&argument == &noArray())
        вернуть "InputArrayOfArrays: noArray()";
    std::ostringstream ss;
    ss << "InputArrayOfArrays:";
    try {
        do {
            ss << (argument.empty() ? " empty()=true" : " empty()=false");
            ss << cv::format(" kind=0x%08llx", (long long целое)argument.kind());
            ss << cv::format(" flags=0x%08llx", (long long целое)argument.getFlags());
            если (argument.getObj() == NULL)
            {
                ss << " obj=NULL";
                break; // done
            }
            ss << cv::format(" total(-1)=%lld", (long long целое)argument.total(-1));
            ss << cv::format(" dims(-1)=%d", argument.dims(-1));
            Size size = argument.size(-1);
            ss << cv::format(" size(-1)=%dx%d", size.width, size.height);
            если (argument.total(-1) > 0)
            {
                ss << " type(0)=" << cv::typeToString(argument.type(0));
                целое dims = argument.dims(0);
                ss << cv::format(" dims(0)=%d", dims);
                если (dims <= 2)
                {
                    Size size0 = argument.size(0);
                    ss << cv::format(" size(0)=%dx%d", size0.width, size0.height);
                }
                иначе
                {
                    целое sz[CV_MAX_DIM] = {0};
                    argument.sizend(sz, 0);
                    ss << " size(0)=[";
                    для (целое i = 0; i < dims; i++)
                    {
                        если (i > 0)
                            ss << ' ';
                        ss << sz[i];
                    }
                    ss << "]";
                }
            }
        } пока (0);
    }
    catch (const std::exception& e)
    {
        ss << " ERROR: exception occurred: " << e.what();
    }
    catch (...)
    {
        ss << " ERROR: unknown exception occurred, dump is non-complete";
    }
    вернуть ss.str();
}

CV_EXPORTS_W String dumpInputOutputArray(InputOutputArray argument)
{
    если (&argument == &noArray())
        вернуть "InputOutputArray: noArray()";
    std::ostringstream ss;
    ss << "InputOutputArray:";
    try {
        do {
            ss << (argument.empty() ? " empty()=true" : " empty()=false");
            ss << cv::format(" kind=0x%08llx", (long long целое)argument.kind());
            ss << cv::format(" flags=0x%08llx", (long long целое)argument.getFlags());
            если (argument.getObj() == NULL)
            {
                ss << " obj=NULL";
                break; // done
            }
            ss << cv::format(" total(-1)=%lld", (long long целое)argument.total(-1));
            целое dims = argument.dims(-1);
            ss << cv::format(" dims(-1)=%d", dims);
            если (dims <= 2)
            {
                Size size = argument.size(-1);
                ss << cv::format(" size(-1)=%dx%d", size.width, size.height);
            }
            иначе
            {
                целое sz[CV_MAX_DIM] = {0};
                argument.sizend(sz, -1);
                ss << " size(-1)=[";
                для (целое i = 0; i < dims; i++)
                {
                    если (i > 0)
                        ss << ' ';
                    ss << sz[i];
                }
                ss << "]";
            }
            ss << " type(-1)=" << cv::typeToString(argument.type(-1));
        } пока (0);
    }
    catch (const std::exception& e)
    {
        ss << " ERROR: exception occurred: " << e.what();
    }
    catch (...)
    {
        ss << " ERROR: unknown exception occurred, dump is non-complete";
    }
    вернуть ss.str();
}

CV_EXPORTS_W String dumpInputOutputArrayOfArrays(InputOutputArrayOfArrays argument)
{
    если (&argument == &noArray())
        вернуть "InputOutputArrayOfArrays: noArray()";
    std::ostringstream ss;
    ss << "InputOutputArrayOfArrays:";
    try {
        do {
            ss << (argument.empty() ? " empty()=true" : " empty()=false");
            ss << cv::format(" kind=0x%08llx", (long long целое)argument.kind());
            ss << cv::format(" flags=0x%08llx", (long long целое)argument.getFlags());
            если (argument.getObj() == NULL)
            {
                ss << " obj=NULL";
                break; // done
            }
            ss << cv::format(" total(-1)=%lld", (long long целое)argument.total(-1));
            ss << cv::format(" dims(-1)=%d", argument.dims(-1));
            Size size = argument.size(-1);
            ss << cv::format(" size(-1)=%dx%d", size.width, size.height);
            если (argument.total(-1) > 0)
            {
                ss << " type(0)=" << cv::typeToString(argument.type(0));
                целое dims = argument.dims(0);
                ss << cv::format(" dims(0)=%d", dims);
                если (dims <= 2)
                {
                    Size size0 = argument.size(0);
                    ss << cv::format(" size(0)=%dx%d", size0.width, size0.height);
                }
                иначе
                {
                    целое sz[CV_MAX_DIM] = {0};
                    argument.sizend(sz, 0);
                    ss << " size(0)=[";
                    для (целое i = 0; i < dims; i++)
                    {
                        если (i > 0)
                            ss << ' ';
                        ss << sz[i];
                    }
                    ss << "]";
                }
            }
        } пока (0);
    }
    catch (const std::exception& e)
    {
        ss << " ERROR: exception occurred: " << e.what();
    }
    catch (...)
    {
        ss << " ERROR: unknown exception occurred, dump is non-complete";
    }
    вернуть ss.str();
}

template <class T, class Formatter>
static inline String dumpVector(const std::vector<T>& vec, Formatter format)
{
    std::ostringstream oss("[", std::ios::ate);
    если (!vec.empty())
    {
        format(oss) << vec[0];
        для (std::size_t i = 1; i < vec.size(); ++i)
        {
            oss << ", ";
            format(oss) << vec[i];
        }
    }
    oss << "]";
    вернуть oss.str();
}

static inline std::ostream& noFormat(std::ostream& os)
{
    вернуть os;
}

static inline std::ostream& floatFormat(std::ostream& os)
{
    вернуть os << std::fixed << std::setprecision(2);
}

String dumpVectorOfInt(const std::vector<целое>& vec)
{
    вернуть dumpVector(vec, &noFormat);
}

String dumpVectorOfDouble(const std::vector<double>& vec)
{
    вернуть dumpVector(vec, &floatFormat);
}

String dumpVectorOfRect(const std::vector<Rect>& vec)
{
    вернуть dumpVector(vec, &noFormat);
}


namespace fs {
cv::String getCacheDirectoryForDownloads()
{
#если OPENCV_HAVE_FILESYSTEM_SUPPORT
    вернуть cv::utils::fs::getCacheDirectory("downloads", "OPENCV_DOWNLOADS_CACHE_DIR");
#иначе
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#endif
}
} // namespace fs

}} // namespace
