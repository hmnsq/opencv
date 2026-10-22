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
    戻る os << "[x=" << rect.x << ", y=" << rect.y << ", w=" << rect.width << ", h=" << rect.height << ']';
}
namespace utils {

String dumpInputArray(InputArray argument)
{
    もし (&argument == &noArray())
        戻る "InputArray: noArray()";
    std::ostringstream ss;
    ss << "InputArray:";
    try {
        do {
            ss << (argument.empty() ? " empty()=true" : " empty()=false");
            ss << cv::format(" kind=0x%08llx", (long long 整数)argument.kind());
            ss << cv::format(" flags=0x%08llx", (long long 整数)argument.getFlags());
            もし (argument.getObj() == NULL)
            {
                ss << " obj=NULL";
                break; // done
            }
            ss << cv::format(" total(-1)=%lld", (long long 整数)argument.total(-1));
            整数 dims = argument.dims(-1);
            ss << cv::format(" dims(-1)=%d", dims);
            もし (dims <= 2)
            {
                Size size = argument.size(-1);
                ss << cv::format(" size(-1)=%dx%d", size.width, size.height);
            }
            否则
            {
                整数 sz[CV_MAX_DIM] = {0};
                argument.sizend(sz, -1);
                ss << " size(-1)=[";
                ために (整数 i = 0; i < dims; i++)
                {
                    もし (i > 0)
                        ss << ' ';
                    ss << sz[i];
                }
                ss << "]";
            }
            ss << " type(-1)=" << cv::typeToString(argument.type(-1));
        } ながら (0);
    }
    catch (const std::exception& e)
    {
        ss << " ERROR: exception occurred: " << e.what();
    }
    catch (...)
    {
        ss << " ERROR: unknown exception occurred, dump is non-complete";
    }
    戻る ss.str();
}

CV_EXPORTS_W String dumpInputArrayOfArrays(InputArrayOfArrays argument)
{
    もし (&argument == &noArray())
        戻る "InputArrayOfArrays: noArray()";
    std::ostringstream ss;
    ss << "InputArrayOfArrays:";
    try {
        do {
            ss << (argument.empty() ? " empty()=true" : " empty()=false");
            ss << cv::format(" kind=0x%08llx", (long long 整数)argument.kind());
            ss << cv::format(" flags=0x%08llx", (long long 整数)argument.getFlags());
            もし (argument.getObj() == NULL)
            {
                ss << " obj=NULL";
                break; // done
            }
            ss << cv::format(" total(-1)=%lld", (long long 整数)argument.total(-1));
            ss << cv::format(" dims(-1)=%d", argument.dims(-1));
            Size size = argument.size(-1);
            ss << cv::format(" size(-1)=%dx%d", size.width, size.height);
            もし (argument.total(-1) > 0)
            {
                ss << " type(0)=" << cv::typeToString(argument.type(0));
                整数 dims = argument.dims(0);
                ss << cv::format(" dims(0)=%d", dims);
                もし (dims <= 2)
                {
                    Size size0 = argument.size(0);
                    ss << cv::format(" size(0)=%dx%d", size0.width, size0.height);
                }
                否则
                {
                    整数 sz[CV_MAX_DIM] = {0};
                    argument.sizend(sz, 0);
                    ss << " size(0)=[";
                    ために (整数 i = 0; i < dims; i++)
                    {
                        もし (i > 0)
                            ss << ' ';
                        ss << sz[i];
                    }
                    ss << "]";
                }
            }
        } ながら (0);
    }
    catch (const std::exception& e)
    {
        ss << " ERROR: exception occurred: " << e.what();
    }
    catch (...)
    {
        ss << " ERROR: unknown exception occurred, dump is non-complete";
    }
    戻る ss.str();
}

CV_EXPORTS_W String dumpInputOutputArray(InputOutputArray argument)
{
    もし (&argument == &noArray())
        戻る "InputOutputArray: noArray()";
    std::ostringstream ss;
    ss << "InputOutputArray:";
    try {
        do {
            ss << (argument.empty() ? " empty()=true" : " empty()=false");
            ss << cv::format(" kind=0x%08llx", (long long 整数)argument.kind());
            ss << cv::format(" flags=0x%08llx", (long long 整数)argument.getFlags());
            もし (argument.getObj() == NULL)
            {
                ss << " obj=NULL";
                break; // done
            }
            ss << cv::format(" total(-1)=%lld", (long long 整数)argument.total(-1));
            整数 dims = argument.dims(-1);
            ss << cv::format(" dims(-1)=%d", dims);
            もし (dims <= 2)
            {
                Size size = argument.size(-1);
                ss << cv::format(" size(-1)=%dx%d", size.width, size.height);
            }
            否则
            {
                整数 sz[CV_MAX_DIM] = {0};
                argument.sizend(sz, -1);
                ss << " size(-1)=[";
                ために (整数 i = 0; i < dims; i++)
                {
                    もし (i > 0)
                        ss << ' ';
                    ss << sz[i];
                }
                ss << "]";
            }
            ss << " type(-1)=" << cv::typeToString(argument.type(-1));
        } ながら (0);
    }
    catch (const std::exception& e)
    {
        ss << " ERROR: exception occurred: " << e.what();
    }
    catch (...)
    {
        ss << " ERROR: unknown exception occurred, dump is non-complete";
    }
    戻る ss.str();
}

CV_EXPORTS_W String dumpInputOutputArrayOfArrays(InputOutputArrayOfArrays argument)
{
    もし (&argument == &noArray())
        戻る "InputOutputArrayOfArrays: noArray()";
    std::ostringstream ss;
    ss << "InputOutputArrayOfArrays:";
    try {
        do {
            ss << (argument.empty() ? " empty()=true" : " empty()=false");
            ss << cv::format(" kind=0x%08llx", (long long 整数)argument.kind());
            ss << cv::format(" flags=0x%08llx", (long long 整数)argument.getFlags());
            もし (argument.getObj() == NULL)
            {
                ss << " obj=NULL";
                break; // done
            }
            ss << cv::format(" total(-1)=%lld", (long long 整数)argument.total(-1));
            ss << cv::format(" dims(-1)=%d", argument.dims(-1));
            Size size = argument.size(-1);
            ss << cv::format(" size(-1)=%dx%d", size.width, size.height);
            もし (argument.total(-1) > 0)
            {
                ss << " type(0)=" << cv::typeToString(argument.type(0));
                整数 dims = argument.dims(0);
                ss << cv::format(" dims(0)=%d", dims);
                もし (dims <= 2)
                {
                    Size size0 = argument.size(0);
                    ss << cv::format(" size(0)=%dx%d", size0.width, size0.height);
                }
                否则
                {
                    整数 sz[CV_MAX_DIM] = {0};
                    argument.sizend(sz, 0);
                    ss << " size(0)=[";
                    ために (整数 i = 0; i < dims; i++)
                    {
                        もし (i > 0)
                            ss << ' ';
                        ss << sz[i];
                    }
                    ss << "]";
                }
            }
        } ながら (0);
    }
    catch (const std::exception& e)
    {
        ss << " ERROR: exception occurred: " << e.what();
    }
    catch (...)
    {
        ss << " ERROR: unknown exception occurred, dump is non-complete";
    }
    戻る ss.str();
}

template <class T, class Formatter>
static inline String dumpVector(const std::vector<T>& vec, Formatter format)
{
    std::ostringstream oss("[", std::ios::ate);
    もし (!vec.empty())
    {
        format(oss) << vec[0];
        ために (std::size_t i = 1; i < vec.size(); ++i)
        {
            oss << ", ";
            format(oss) << vec[i];
        }
    }
    oss << "]";
    戻る oss.str();
}

static inline std::ostream& noFormat(std::ostream& os)
{
    戻る os;
}

static inline std::ostream& floatFormat(std::ostream& os)
{
    戻る os << std::fixed << std::setprecision(2);
}

String dumpVectorOfInt(const std::vector<整数>& vec)
{
    戻る dumpVector(vec, &noFormat);
}

String dumpVectorOfDouble(const std::vector<double>& vec)
{
    戻る dumpVector(vec, &floatFormat);
}

String dumpVectorOfRect(const std::vector<Rect>& vec)
{
    戻る dumpVector(vec, &noFormat);
}


namespace fs {
cv::String getCacheDirectoryForDownloads()
{
#もし OPENCV_HAVE_FILESYSTEM_SUPPORT
    戻る cv::utils::fs::getCacheDirectory("downloads", "OPENCV_DOWNLOADS_CACHE_DIR");
#否则
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#endif
}
} // namespace fs

}} // namespace
