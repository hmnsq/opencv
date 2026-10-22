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
    повернути os << "[x=" << rect.x << ", y=" << rect.y << ", w=" << rect.width << ", h=" << rect.height << ']';
}
namespace utils {

String dumpInputArray(InputArray argument)
{
    якщо (&argument == &noArray())
        повернути "InputArray: noArray()";
    std::ostringstream ss;
    ss << "InputArray:";
    try {
        do {
            ss << (argument.empty() ? " empty()=true" : " empty()=false");
            ss << cv::format(" kind=0x%08llx", (long long ціле)argument.kind());
            ss << cv::format(" flags=0x%08llx", (long long ціле)argument.getFlags());
            якщо (argument.getObj() == NULL)
            {
                ss << " obj=NULL";
                break; // done
            }
            ss << cv::format(" total(-1)=%lld", (long long ціле)argument.total(-1));
            ціле dims = argument.dims(-1);
            ss << cv::format(" dims(-1)=%d", dims);
            якщо (dims <= 2)
            {
                Size size = argument.size(-1);
                ss << cv::format(" size(-1)=%dx%d", size.width, size.height);
            }
            інакше
            {
                ціле sz[CV_MAX_DIM] = {0};
                argument.sizend(sz, -1);
                ss << " size(-1)=[";
                для (ціле i = 0; i < dims; i++)
                {
                    якщо (i > 0)
                        ss << ' ';
                    ss << sz[i];
                }
                ss << "]";
            }
            ss << " type(-1)=" << cv::typeToString(argument.type(-1));
        } поки (0);
    }
    catch (const std::exception& e)
    {
        ss << " ERROR: exception occurred: " << e.what();
    }
    catch (...)
    {
        ss << " ERROR: unknown exception occurred, dump is non-complete";
    }
    повернути ss.str();
}

CV_EXPORTS_W String dumpInputArrayOfArrays(InputArrayOfArrays argument)
{
    якщо (&argument == &noArray())
        повернути "InputArrayOfArrays: noArray()";
    std::ostringstream ss;
    ss << "InputArrayOfArrays:";
    try {
        do {
            ss << (argument.empty() ? " empty()=true" : " empty()=false");
            ss << cv::format(" kind=0x%08llx", (long long ціле)argument.kind());
            ss << cv::format(" flags=0x%08llx", (long long ціле)argument.getFlags());
            якщо (argument.getObj() == NULL)
            {
                ss << " obj=NULL";
                break; // done
            }
            ss << cv::format(" total(-1)=%lld", (long long ціле)argument.total(-1));
            ss << cv::format(" dims(-1)=%d", argument.dims(-1));
            Size size = argument.size(-1);
            ss << cv::format(" size(-1)=%dx%d", size.width, size.height);
            якщо (argument.total(-1) > 0)
            {
                ss << " type(0)=" << cv::typeToString(argument.type(0));
                ціле dims = argument.dims(0);
                ss << cv::format(" dims(0)=%d", dims);
                якщо (dims <= 2)
                {
                    Size size0 = argument.size(0);
                    ss << cv::format(" size(0)=%dx%d", size0.width, size0.height);
                }
                інакше
                {
                    ціле sz[CV_MAX_DIM] = {0};
                    argument.sizend(sz, 0);
                    ss << " size(0)=[";
                    для (ціле i = 0; i < dims; i++)
                    {
                        якщо (i > 0)
                            ss << ' ';
                        ss << sz[i];
                    }
                    ss << "]";
                }
            }
        } поки (0);
    }
    catch (const std::exception& e)
    {
        ss << " ERROR: exception occurred: " << e.what();
    }
    catch (...)
    {
        ss << " ERROR: unknown exception occurred, dump is non-complete";
    }
    повернути ss.str();
}

CV_EXPORTS_W String dumpInputOutputArray(InputOutputArray argument)
{
    якщо (&argument == &noArray())
        повернути "InputOutputArray: noArray()";
    std::ostringstream ss;
    ss << "InputOutputArray:";
    try {
        do {
            ss << (argument.empty() ? " empty()=true" : " empty()=false");
            ss << cv::format(" kind=0x%08llx", (long long ціле)argument.kind());
            ss << cv::format(" flags=0x%08llx", (long long ціле)argument.getFlags());
            якщо (argument.getObj() == NULL)
            {
                ss << " obj=NULL";
                break; // done
            }
            ss << cv::format(" total(-1)=%lld", (long long ціле)argument.total(-1));
            ціле dims = argument.dims(-1);
            ss << cv::format(" dims(-1)=%d", dims);
            якщо (dims <= 2)
            {
                Size size = argument.size(-1);
                ss << cv::format(" size(-1)=%dx%d", size.width, size.height);
            }
            інакше
            {
                ціле sz[CV_MAX_DIM] = {0};
                argument.sizend(sz, -1);
                ss << " size(-1)=[";
                для (ціле i = 0; i < dims; i++)
                {
                    якщо (i > 0)
                        ss << ' ';
                    ss << sz[i];
                }
                ss << "]";
            }
            ss << " type(-1)=" << cv::typeToString(argument.type(-1));
        } поки (0);
    }
    catch (const std::exception& e)
    {
        ss << " ERROR: exception occurred: " << e.what();
    }
    catch (...)
    {
        ss << " ERROR: unknown exception occurred, dump is non-complete";
    }
    повернути ss.str();
}

CV_EXPORTS_W String dumpInputOutputArrayOfArrays(InputOutputArrayOfArrays argument)
{
    якщо (&argument == &noArray())
        повернути "InputOutputArrayOfArrays: noArray()";
    std::ostringstream ss;
    ss << "InputOutputArrayOfArrays:";
    try {
        do {
            ss << (argument.empty() ? " empty()=true" : " empty()=false");
            ss << cv::format(" kind=0x%08llx", (long long ціле)argument.kind());
            ss << cv::format(" flags=0x%08llx", (long long ціле)argument.getFlags());
            якщо (argument.getObj() == NULL)
            {
                ss << " obj=NULL";
                break; // done
            }
            ss << cv::format(" total(-1)=%lld", (long long ціле)argument.total(-1));
            ss << cv::format(" dims(-1)=%d", argument.dims(-1));
            Size size = argument.size(-1);
            ss << cv::format(" size(-1)=%dx%d", size.width, size.height);
            якщо (argument.total(-1) > 0)
            {
                ss << " type(0)=" << cv::typeToString(argument.type(0));
                ціле dims = argument.dims(0);
                ss << cv::format(" dims(0)=%d", dims);
                якщо (dims <= 2)
                {
                    Size size0 = argument.size(0);
                    ss << cv::format(" size(0)=%dx%d", size0.width, size0.height);
                }
                інакше
                {
                    ціле sz[CV_MAX_DIM] = {0};
                    argument.sizend(sz, 0);
                    ss << " size(0)=[";
                    для (ціле i = 0; i < dims; i++)
                    {
                        якщо (i > 0)
                            ss << ' ';
                        ss << sz[i];
                    }
                    ss << "]";
                }
            }
        } поки (0);
    }
    catch (const std::exception& e)
    {
        ss << " ERROR: exception occurred: " << e.what();
    }
    catch (...)
    {
        ss << " ERROR: unknown exception occurred, dump is non-complete";
    }
    повернути ss.str();
}

template <class T, class Formatter>
static inline String dumpVector(const std::vector<T>& vec, Formatter format)
{
    std::ostringstream oss("[", std::ios::ate);
    якщо (!vec.empty())
    {
        format(oss) << vec[0];
        для (std::size_t i = 1; i < vec.size(); ++i)
        {
            oss << ", ";
            format(oss) << vec[i];
        }
    }
    oss << "]";
    повернути oss.str();
}

static inline std::ostream& noFormat(std::ostream& os)
{
    повернути os;
}

static inline std::ostream& floatFormat(std::ostream& os)
{
    повернути os << std::fixed << std::setprecision(2);
}

String dumpVectorOfInt(const std::vector<ціле>& vec)
{
    повернути dumpVector(vec, &noFormat);
}

String dumpVectorOfDouble(const std::vector<double>& vec)
{
    повернути dumpVector(vec, &floatFormat);
}

String dumpVectorOfRect(const std::vector<Rect>& vec)
{
    повернути dumpVector(vec, &noFormat);
}


namespace fs {
cv::String getCacheDirectoryForDownloads()
{
#якщо OPENCV_HAVE_FILESYSTEM_SUPPORT
    повернути cv::utils::fs::getCacheDirectory("downloads", "OPENCV_DOWNLOADS_CACHE_DIR");
#інакше
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#endif
}
} // namespace fs

}} // namespace
