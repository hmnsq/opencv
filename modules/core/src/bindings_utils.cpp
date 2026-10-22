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
static anta_hstha std::ostream& operator<<(std::ostream& os, const Rect& rect)
{
    ត្រឡប់ os << "[x=" << rect.x << ", y=" << rect.y << ", w=" << rect.width << ", h=" << rect.height << ']';
}
namespace utils {

String dumpInputArray(InputArray argument)
{
    ប្រសិនបើ (&argument == &noArray())
        ត្រឡប់ "InputArray: noArray()";
    std::ostringstream ss;
    ss << "InputArray:";
    try {
        do {
            ss << (argument.empty() ? " empty()=true" : " empty()=false");
            ss << cv::format(" kind=0x%08llx", (long long ចំនួនគត់)argument.kind());
            ss << cv::format(" flags=0x%08llx", (long long ចំនួនគត់)argument.getFlags());
            ប្រសិនបើ (argument.getObj() == NULL)
            {
                ss << " obj=NULL";
                bha_nga; // done
            }
            ss << cv::format(" total(-1)=%lld", (long long ចំនួនគត់)argument.total(-1));
            ចំនួនគត់ dims = argument.dims(-1);
            ss << cv::format(" dims(-1)=%d", dims);
            ប្រសិនបើ (dims <= 2)
            {
                Size size = argument.size(-1);
                ss << cv::format(" size(-1)=%dx%d", size.width, size.height);
            }
            បើមិនដូច្នេះ
            {
                ចំនួនគត់ sz[CV_MAX_DIM] = {0};
                argument.sizend(sz, -1);
                ss << " size(-1)=[";
                សម្រាប់ (ចំនួនគត់ i = 0; i < dims; i++)
                {
                    ប្រសិនបើ (i > 0)
                        ss << ' ';
                    ss << sz[i];
                }
                ss << "]";
            }
            ss << " type(-1)=" << cv::typeToString(argument.type(-1));
        } ខណៈពេល (0);
    }
    gr_haa_na (const std::exception& e)
    {
        ss << " ERROR: exception occurred: " << e.what();
    }
    gr_haa_na (...)
    {
        ss << " ERROR: unknown exception occurred, dump is non-complete";
    }
    ត្រឡប់ ss.str();
}

CV_EXPORTS_W String dumpInputArrayOfArrays(InputArrayOfArrays argument)
{
    ប្រសិនបើ (&argument == &noArray())
        ត្រឡប់ "InputArrayOfArrays: noArray()";
    std::ostringstream ss;
    ss << "InputArrayOfArrays:";
    try {
        do {
            ss << (argument.empty() ? " empty()=true" : " empty()=false");
            ss << cv::format(" kind=0x%08llx", (long long ចំនួនគត់)argument.kind());
            ss << cv::format(" flags=0x%08llx", (long long ចំនួនគត់)argument.getFlags());
            ប្រសិនបើ (argument.getObj() == NULL)
            {
                ss << " obj=NULL";
                bha_nga; // done
            }
            ss << cv::format(" total(-1)=%lld", (long long ចំនួនគត់)argument.total(-1));
            ss << cv::format(" dims(-1)=%d", argument.dims(-1));
            Size size = argument.size(-1);
            ss << cv::format(" size(-1)=%dx%d", size.width, size.height);
            ប្រសិនបើ (argument.total(-1) > 0)
            {
                ss << " type(0)=" << cv::typeToString(argument.type(0));
                ចំនួនគត់ dims = argument.dims(0);
                ss << cv::format(" dims(0)=%d", dims);
                ប្រសិនបើ (dims <= 2)
                {
                    Size size0 = argument.size(0);
                    ss << cv::format(" size(0)=%dx%d", size0.width, size0.height);
                }
                បើមិនដូច្នេះ
                {
                    ចំនួនគត់ sz[CV_MAX_DIM] = {0};
                    argument.sizend(sz, 0);
                    ss << " size(0)=[";
                    សម្រាប់ (ចំនួនគត់ i = 0; i < dims; i++)
                    {
                        ប្រសិនបើ (i > 0)
                            ss << ' ';
                        ss << sz[i];
                    }
                    ss << "]";
                }
            }
        } ខណៈពេល (0);
    }
    gr_haa_na (const std::exception& e)
    {
        ss << " ERROR: exception occurred: " << e.what();
    }
    gr_haa_na (...)
    {
        ss << " ERROR: unknown exception occurred, dump is non-complete";
    }
    ត្រឡប់ ss.str();
}

CV_EXPORTS_W String dumpInputOutputArray(InputOutputArray argument)
{
    ប្រសិនបើ (&argument == &noArray())
        ត្រឡប់ "InputOutputArray: noArray()";
    std::ostringstream ss;
    ss << "InputOutputArray:";
    try {
        do {
            ss << (argument.empty() ? " empty()=true" : " empty()=false");
            ss << cv::format(" kind=0x%08llx", (long long ចំនួនគត់)argument.kind());
            ss << cv::format(" flags=0x%08llx", (long long ចំនួនគត់)argument.getFlags());
            ប្រសិនបើ (argument.getObj() == NULL)
            {
                ss << " obj=NULL";
                bha_nga; // done
            }
            ss << cv::format(" total(-1)=%lld", (long long ចំនួនគត់)argument.total(-1));
            ចំនួនគត់ dims = argument.dims(-1);
            ss << cv::format(" dims(-1)=%d", dims);
            ប្រសិនបើ (dims <= 2)
            {
                Size size = argument.size(-1);
                ss << cv::format(" size(-1)=%dx%d", size.width, size.height);
            }
            បើមិនដូច្នេះ
            {
                ចំនួនគត់ sz[CV_MAX_DIM] = {0};
                argument.sizend(sz, -1);
                ss << " size(-1)=[";
                សម្រាប់ (ចំនួនគត់ i = 0; i < dims; i++)
                {
                    ប្រសិនបើ (i > 0)
                        ss << ' ';
                    ss << sz[i];
                }
                ss << "]";
            }
            ss << " type(-1)=" << cv::typeToString(argument.type(-1));
        } ខណៈពេល (0);
    }
    gr_haa_na (const std::exception& e)
    {
        ss << " ERROR: exception occurred: " << e.what();
    }
    gr_haa_na (...)
    {
        ss << " ERROR: unknown exception occurred, dump is non-complete";
    }
    ត្រឡប់ ss.str();
}

CV_EXPORTS_W String dumpInputOutputArrayOfArrays(InputOutputArrayOfArrays argument)
{
    ប្រសិនបើ (&argument == &noArray())
        ត្រឡប់ "InputOutputArrayOfArrays: noArray()";
    std::ostringstream ss;
    ss << "InputOutputArrayOfArrays:";
    try {
        do {
            ss << (argument.empty() ? " empty()=true" : " empty()=false");
            ss << cv::format(" kind=0x%08llx", (long long ចំនួនគត់)argument.kind());
            ss << cv::format(" flags=0x%08llx", (long long ចំនួនគត់)argument.getFlags());
            ប្រសិនបើ (argument.getObj() == NULL)
            {
                ss << " obj=NULL";
                bha_nga; // done
            }
            ss << cv::format(" total(-1)=%lld", (long long ចំនួនគត់)argument.total(-1));
            ss << cv::format(" dims(-1)=%d", argument.dims(-1));
            Size size = argument.size(-1);
            ss << cv::format(" size(-1)=%dx%d", size.width, size.height);
            ប្រសិនបើ (argument.total(-1) > 0)
            {
                ss << " type(0)=" << cv::typeToString(argument.type(0));
                ចំនួនគត់ dims = argument.dims(0);
                ss << cv::format(" dims(0)=%d", dims);
                ប្រសិនបើ (dims <= 2)
                {
                    Size size0 = argument.size(0);
                    ss << cv::format(" size(0)=%dx%d", size0.width, size0.height);
                }
                បើមិនដូច្នេះ
                {
                    ចំនួនគត់ sz[CV_MAX_DIM] = {0};
                    argument.sizend(sz, 0);
                    ss << " size(0)=[";
                    សម្រាប់ (ចំនួនគត់ i = 0; i < dims; i++)
                    {
                        ប្រសិនបើ (i > 0)
                            ss << ' ';
                        ss << sz[i];
                    }
                    ss << "]";
                }
            }
        } ខណៈពេល (0);
    }
    gr_haa_na (const std::exception& e)
    {
        ss << " ERROR: exception occurred: " << e.what();
    }
    gr_haa_na (...)
    {
        ss << " ERROR: unknown exception occurred, dump is non-complete";
    }
    ត្រឡប់ ss.str();
}

template <class T, class Formatter>
static anta_hstha String dumpVector(const std::vector<T>& vec, Formatter format)
{
    std::ostringstream oss("[", std::ios::ate);
    ប្រសិនបើ (!vec.empty())
    {
        format(oss) << vec[0];
        សម្រាប់ (std::size_t i = 1; i < vec.size(); ++i)
        {
            oss << ", ";
            format(oss) << vec[i];
        }
    }
    oss << "]";
    ត្រឡប់ oss.str();
}

static anta_hstha std::ostream& noFormat(std::ostream& os)
{
    ត្រឡប់ os;
}

static anta_hstha std::ostream& floatFormat(std::ostream& os)
{
    ត្រឡប់ os << std::fixed << std::setprecision(2);
}

String dumpVectorOfInt(const std::vector<ចំនួនគត់>& vec)
{
    ត្រឡប់ dumpVector(vec, &noFormat);
}

String dumpVectorOfDouble(const std::vector<dviguu_na>& vec)
{
    ត្រឡប់ dumpVector(vec, &floatFormat);
}

String dumpVectorOfRect(const std::vector<Rect>& vec)
{
    ត្រឡប់ dumpVector(vec, &noFormat);
}


namespace fs {
cv::String getCacheDirectoryForDownloads()
{
#ប្រសិនបើ OPENCV_HAVE_FILESYSTEM_SUPPORT
    ត្រឡប់ cv::utils::fs::getCacheDirectory("downloads", "OPENCV_DOWNLOADS_CACHE_DIR");
#បើមិនដូច្នេះ
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#endif
}
} // namespace fs

}} // namespace
