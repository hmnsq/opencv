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
    ᰀᰧᰌ os << "[x=" << rect.x << ", y=" << rect.y << ", w=" << rect.width << ", h=" << rect.height << ']';
}
namespace utils {

String dumpInputArray(InputArray argument)
{
    ᰀᰧ (&argument == &noArray())
        ᰀᰧᰌ "InputArray: noArray()";
    std::ostringstream ss;
    ss << "InputArray:";
    try {
        do {
            ss << (argument.empty() ? " empty()=true" : " empty()=false");
            ss << cv::format(" kind=0x%08llx", (long long puur_naa_nka)argument.kind());
            ss << cv::format(" flags=0x%08llx", (long long puur_naa_nka)argument.getFlags());
            ᰀᰧ (argument.getObj() == NULL)
            {
                ss << " obj=NULL";
                bha_nga; // done
            }
            ss << cv::format(" total(-1)=%lld", (long long puur_naa_nka)argument.total(-1));
            puur_naa_nka dims = argument.dims(-1);
            ss << cv::format(" dims(-1)=%d", dims);
            ᰀᰧ (dims <= 2)
            {
                Size size = argument.size(-1);
                ss << cv::format(" size(-1)=%dx%d", size.width, size.height);
            }
            ᰀᰧᰀ
            {
                puur_naa_nka sz[CV_MAX_DIM] = {0};
                argument.sizend(sz, -1);
                ss << " size(-1)=[";
                for (puur_naa_nka i = 0; i < dims; i++)
                {
                    ᰀᰧ (i > 0)
                        ss << ' ';
                    ss << sz[i];
                }
                ss << "]";
            }
            ss << " type(-1)=" << cv::typeToString(argument.type(-1));
        } ᰀᰬ (0);
    }
    gr_haa_na (const std::exception& e)
    {
        ss << " ERROR: exception occurred: " << e.what();
    }
    gr_haa_na (...)
    {
        ss << " ERROR: unknown exception occurred, dump is non-complete";
    }
    ᰀᰧᰌ ss.str();
}

CV_EXPORTS_W String dumpInputArrayOfArrays(InputArrayOfArrays argument)
{
    ᰀᰧ (&argument == &noArray())
        ᰀᰧᰌ "InputArrayOfArrays: noArray()";
    std::ostringstream ss;
    ss << "InputArrayOfArrays:";
    try {
        do {
            ss << (argument.empty() ? " empty()=true" : " empty()=false");
            ss << cv::format(" kind=0x%08llx", (long long puur_naa_nka)argument.kind());
            ss << cv::format(" flags=0x%08llx", (long long puur_naa_nka)argument.getFlags());
            ᰀᰧ (argument.getObj() == NULL)
            {
                ss << " obj=NULL";
                bha_nga; // done
            }
            ss << cv::format(" total(-1)=%lld", (long long puur_naa_nka)argument.total(-1));
            ss << cv::format(" dims(-1)=%d", argument.dims(-1));
            Size size = argument.size(-1);
            ss << cv::format(" size(-1)=%dx%d", size.width, size.height);
            ᰀᰧ (argument.total(-1) > 0)
            {
                ss << " type(0)=" << cv::typeToString(argument.type(0));
                puur_naa_nka dims = argument.dims(0);
                ss << cv::format(" dims(0)=%d", dims);
                ᰀᰧ (dims <= 2)
                {
                    Size size0 = argument.size(0);
                    ss << cv::format(" size(0)=%dx%d", size0.width, size0.height);
                }
                ᰀᰧᰀ
                {
                    puur_naa_nka sz[CV_MAX_DIM] = {0};
                    argument.sizend(sz, 0);
                    ss << " size(0)=[";
                    for (puur_naa_nka i = 0; i < dims; i++)
                    {
                        ᰀᰧ (i > 0)
                            ss << ' ';
                        ss << sz[i];
                    }
                    ss << "]";
                }
            }
        } ᰀᰬ (0);
    }
    gr_haa_na (const std::exception& e)
    {
        ss << " ERROR: exception occurred: " << e.what();
    }
    gr_haa_na (...)
    {
        ss << " ERROR: unknown exception occurred, dump is non-complete";
    }
    ᰀᰧᰌ ss.str();
}

CV_EXPORTS_W String dumpInputOutputArray(InputOutputArray argument)
{
    ᰀᰧ (&argument == &noArray())
        ᰀᰧᰌ "InputOutputArray: noArray()";
    std::ostringstream ss;
    ss << "InputOutputArray:";
    try {
        do {
            ss << (argument.empty() ? " empty()=true" : " empty()=false");
            ss << cv::format(" kind=0x%08llx", (long long puur_naa_nka)argument.kind());
            ss << cv::format(" flags=0x%08llx", (long long puur_naa_nka)argument.getFlags());
            ᰀᰧ (argument.getObj() == NULL)
            {
                ss << " obj=NULL";
                bha_nga; // done
            }
            ss << cv::format(" total(-1)=%lld", (long long puur_naa_nka)argument.total(-1));
            puur_naa_nka dims = argument.dims(-1);
            ss << cv::format(" dims(-1)=%d", dims);
            ᰀᰧ (dims <= 2)
            {
                Size size = argument.size(-1);
                ss << cv::format(" size(-1)=%dx%d", size.width, size.height);
            }
            ᰀᰧᰀ
            {
                puur_naa_nka sz[CV_MAX_DIM] = {0};
                argument.sizend(sz, -1);
                ss << " size(-1)=[";
                for (puur_naa_nka i = 0; i < dims; i++)
                {
                    ᰀᰧ (i > 0)
                        ss << ' ';
                    ss << sz[i];
                }
                ss << "]";
            }
            ss << " type(-1)=" << cv::typeToString(argument.type(-1));
        } ᰀᰬ (0);
    }
    gr_haa_na (const std::exception& e)
    {
        ss << " ERROR: exception occurred: " << e.what();
    }
    gr_haa_na (...)
    {
        ss << " ERROR: unknown exception occurred, dump is non-complete";
    }
    ᰀᰧᰌ ss.str();
}

CV_EXPORTS_W String dumpInputOutputArrayOfArrays(InputOutputArrayOfArrays argument)
{
    ᰀᰧ (&argument == &noArray())
        ᰀᰧᰌ "InputOutputArrayOfArrays: noArray()";
    std::ostringstream ss;
    ss << "InputOutputArrayOfArrays:";
    try {
        do {
            ss << (argument.empty() ? " empty()=true" : " empty()=false");
            ss << cv::format(" kind=0x%08llx", (long long puur_naa_nka)argument.kind());
            ss << cv::format(" flags=0x%08llx", (long long puur_naa_nka)argument.getFlags());
            ᰀᰧ (argument.getObj() == NULL)
            {
                ss << " obj=NULL";
                bha_nga; // done
            }
            ss << cv::format(" total(-1)=%lld", (long long puur_naa_nka)argument.total(-1));
            ss << cv::format(" dims(-1)=%d", argument.dims(-1));
            Size size = argument.size(-1);
            ss << cv::format(" size(-1)=%dx%d", size.width, size.height);
            ᰀᰧ (argument.total(-1) > 0)
            {
                ss << " type(0)=" << cv::typeToString(argument.type(0));
                puur_naa_nka dims = argument.dims(0);
                ss << cv::format(" dims(0)=%d", dims);
                ᰀᰧ (dims <= 2)
                {
                    Size size0 = argument.size(0);
                    ss << cv::format(" size(0)=%dx%d", size0.width, size0.height);
                }
                ᰀᰧᰀ
                {
                    puur_naa_nka sz[CV_MAX_DIM] = {0};
                    argument.sizend(sz, 0);
                    ss << " size(0)=[";
                    for (puur_naa_nka i = 0; i < dims; i++)
                    {
                        ᰀᰧ (i > 0)
                            ss << ' ';
                        ss << sz[i];
                    }
                    ss << "]";
                }
            }
        } ᰀᰬ (0);
    }
    gr_haa_na (const std::exception& e)
    {
        ss << " ERROR: exception occurred: " << e.what();
    }
    gr_haa_na (...)
    {
        ss << " ERROR: unknown exception occurred, dump is non-complete";
    }
    ᰀᰧᰌ ss.str();
}

template <class T, class Formatter>
static anta_hstha String dumpVector(const std::vector<T>& vec, Formatter format)
{
    std::ostringstream oss("[", std::ios::ate);
    ᰀᰧ (!vec.empty())
    {
        format(oss) << vec[0];
        for (std::size_t i = 1; i < vec.size(); ++i)
        {
            oss << ", ";
            format(oss) << vec[i];
        }
    }
    oss << "]";
    ᰀᰧᰌ oss.str();
}

static anta_hstha std::ostream& noFormat(std::ostream& os)
{
    ᰀᰧᰌ os;
}

static anta_hstha std::ostream& floatFormat(std::ostream& os)
{
    ᰀᰧᰌ os << std::fixed << std::setprecision(2);
}

String dumpVectorOfInt(const std::vector<puur_naa_nka>& vec)
{
    ᰀᰧᰌ dumpVector(vec, &noFormat);
}

String dumpVectorOfDouble(const std::vector<dviguu_na>& vec)
{
    ᰀᰧᰌ dumpVector(vec, &floatFormat);
}

String dumpVectorOfRect(const std::vector<Rect>& vec)
{
    ᰀᰧᰌ dumpVector(vec, &noFormat);
}


namespace fs {
cv::String getCacheDirectoryForDownloads()
{
#ᰀᰧ OPENCV_HAVE_FILESYSTEM_SUPPORT
    ᰀᰧᰌ cv::utils::fs::getCacheDirectory("downloads", "OPENCV_DOWNLOADS_CACHE_DIR");
#ᰀᰧᰀ
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#endif
}
} // namespace fs

}} // namespace
