// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ضمن "precomp.hpp"
#ضمن "opencv2/core/bindings_utils.hpp"
#ضمن <sstream>
#ضمن <iomanip>
#ضمن <opencv2/core/utils/filesystem.hpp>
#ضمن <opencv2/core/utils/filesystem.private.hpp>

نطاق cv {
ساكن مضمن std::ostream& operator<<(std::ostream& os, ثابت Rect& rect)
{
    ܦܢܐ os << "[x=" << rect.x << ", y=" << rect.y << ", w=" << rect.width << ", h=" << rect.height << ']';
}
نطاق utils {

String dumpInputArray(InputArray argument)
{
    ܐܢ (&argument == &noArray())
        ܦܢܐ "InputArray: noArray()";
    std::ostringstream ss;
    ss << "InputArray:";
    حاول {
        ܥܒܕ {
            ss << (argument.empty() ? " empty()=true" : " empty()=false");
            ss << cv::format(" kind=0x%08llx", (long long صحيح)argument.kind());
            ss << cv::format(" flags=0x%08llx", (long long صحيح)argument.getFlags());
            ܐܢ (argument.getObj() == عدم)
            {
                ss << " obj=NULL";
                اكسر; // done
            }
            ss << cv::format(" total(-1)=%lld", (long long صحيح)argument.total(-1));
            صحيح dims = argument.dims(-1);
            ss << cv::format(" dims(-1)=%d", dims);
            ܐܢ (dims <= 2)
            {
                Size size = argument.size(-1);
                ss << cv::format(" size(-1)=%dx%d", size.width, size.height);
            }
            ܐܚܪܢܐ
            {
                صحيح sz[CV_MAX_DIM] = {0};
                argument.sizend(sz, -1);
                ss << " size(-1)=[";
                لكل (صحيح i = 0; i < dims; i++)
                {
                    ܐܢ (i > 0)
                        ss << ' ';
                    ss << sz[i];
                }
                ss << "]";
            }
            ss << " type(-1)=" << cv::typeToString(argument.type(-1));
        } ܟܡܐ (0);
    }
    امسك (ثابت std::exception& e)
    {
        ss << " ERROR: exception occurred: " << e.what();
    }
    امسك (...)
    {
        ss << " ERROR: unknown exception occurred, dump is non-complete";
    }
    ܦܢܐ ss.str();
}

CV_EXPORTS_W String dumpInputArrayOfArrays(InputArrayOfArrays argument)
{
    ܐܢ (&argument == &noArray())
        ܦܢܐ "InputArrayOfArrays: noArray()";
    std::ostringstream ss;
    ss << "InputArrayOfArrays:";
    حاول {
        ܥܒܕ {
            ss << (argument.empty() ? " empty()=true" : " empty()=false");
            ss << cv::format(" kind=0x%08llx", (long long صحيح)argument.kind());
            ss << cv::format(" flags=0x%08llx", (long long صحيح)argument.getFlags());
            ܐܢ (argument.getObj() == عدم)
            {
                ss << " obj=NULL";
                اكسر; // done
            }
            ss << cv::format(" total(-1)=%lld", (long long صحيح)argument.total(-1));
            ss << cv::format(" dims(-1)=%d", argument.dims(-1));
            Size size = argument.size(-1);
            ss << cv::format(" size(-1)=%dx%d", size.width, size.height);
            ܐܢ (argument.total(-1) > 0)
            {
                ss << " type(0)=" << cv::typeToString(argument.type(0));
                صحيح dims = argument.dims(0);
                ss << cv::format(" dims(0)=%d", dims);
                ܐܢ (dims <= 2)
                {
                    Size size0 = argument.size(0);
                    ss << cv::format(" size(0)=%dx%d", size0.width, size0.height);
                }
                ܐܚܪܢܐ
                {
                    صحيح sz[CV_MAX_DIM] = {0};
                    argument.sizend(sz, 0);
                    ss << " size(0)=[";
                    لكل (صحيح i = 0; i < dims; i++)
                    {
                        ܐܢ (i > 0)
                            ss << ' ';
                        ss << sz[i];
                    }
                    ss << "]";
                }
            }
        } ܟܡܐ (0);
    }
    امسك (ثابت std::exception& e)
    {
        ss << " ERROR: exception occurred: " << e.what();
    }
    امسك (...)
    {
        ss << " ERROR: unknown exception occurred, dump is non-complete";
    }
    ܦܢܐ ss.str();
}

CV_EXPORTS_W String dumpInputOutputArray(InputOutputArray argument)
{
    ܐܢ (&argument == &noArray())
        ܦܢܐ "InputOutputArray: noArray()";
    std::ostringstream ss;
    ss << "InputOutputArray:";
    حاول {
        ܥܒܕ {
            ss << (argument.empty() ? " empty()=true" : " empty()=false");
            ss << cv::format(" kind=0x%08llx", (long long صحيح)argument.kind());
            ss << cv::format(" flags=0x%08llx", (long long صحيح)argument.getFlags());
            ܐܢ (argument.getObj() == عدم)
            {
                ss << " obj=NULL";
                اكسر; // done
            }
            ss << cv::format(" total(-1)=%lld", (long long صحيح)argument.total(-1));
            صحيح dims = argument.dims(-1);
            ss << cv::format(" dims(-1)=%d", dims);
            ܐܢ (dims <= 2)
            {
                Size size = argument.size(-1);
                ss << cv::format(" size(-1)=%dx%d", size.width, size.height);
            }
            ܐܚܪܢܐ
            {
                صحيح sz[CV_MAX_DIM] = {0};
                argument.sizend(sz, -1);
                ss << " size(-1)=[";
                لكل (صحيح i = 0; i < dims; i++)
                {
                    ܐܢ (i > 0)
                        ss << ' ';
                    ss << sz[i];
                }
                ss << "]";
            }
            ss << " type(-1)=" << cv::typeToString(argument.type(-1));
        } ܟܡܐ (0);
    }
    امسك (ثابت std::exception& e)
    {
        ss << " ERROR: exception occurred: " << e.what();
    }
    امسك (...)
    {
        ss << " ERROR: unknown exception occurred, dump is non-complete";
    }
    ܦܢܐ ss.str();
}

CV_EXPORTS_W String dumpInputOutputArrayOfArrays(InputOutputArrayOfArrays argument)
{
    ܐܢ (&argument == &noArray())
        ܦܢܐ "InputOutputArrayOfArrays: noArray()";
    std::ostringstream ss;
    ss << "InputOutputArrayOfArrays:";
    حاول {
        ܥܒܕ {
            ss << (argument.empty() ? " empty()=true" : " empty()=false");
            ss << cv::format(" kind=0x%08llx", (long long صحيح)argument.kind());
            ss << cv::format(" flags=0x%08llx", (long long صحيح)argument.getFlags());
            ܐܢ (argument.getObj() == عدم)
            {
                ss << " obj=NULL";
                اكسر; // done
            }
            ss << cv::format(" total(-1)=%lld", (long long صحيح)argument.total(-1));
            ss << cv::format(" dims(-1)=%d", argument.dims(-1));
            Size size = argument.size(-1);
            ss << cv::format(" size(-1)=%dx%d", size.width, size.height);
            ܐܢ (argument.total(-1) > 0)
            {
                ss << " type(0)=" << cv::typeToString(argument.type(0));
                صحيح dims = argument.dims(0);
                ss << cv::format(" dims(0)=%d", dims);
                ܐܢ (dims <= 2)
                {
                    Size size0 = argument.size(0);
                    ss << cv::format(" size(0)=%dx%d", size0.width, size0.height);
                }
                ܐܚܪܢܐ
                {
                    صحيح sz[CV_MAX_DIM] = {0};
                    argument.sizend(sz, 0);
                    ss << " size(0)=[";
                    لكل (صحيح i = 0; i < dims; i++)
                    {
                        ܐܢ (i > 0)
                            ss << ' ';
                        ss << sz[i];
                    }
                    ss << "]";
                }
            }
        } ܟܡܐ (0);
    }
    امسك (ثابت std::exception& e)
    {
        ss << " ERROR: exception occurred: " << e.what();
    }
    امسك (...)
    {
        ss << " ERROR: unknown exception occurred, dump is non-complete";
    }
    ܦܢܐ ss.str();
}

template <صنف T, صنف Formatter>
ساكن مضمن String dumpVector(ثابت std::vector<T>& vec, Formatter format)
{
    std::ostringstream oss("[", std::ios::ate);
    ܐܢ (!vec.empty())
    {
        format(oss) << vec[0];
        لكل (std::size_t i = 1; i < vec.size(); ++i)
        {
            oss << ", ";
            format(oss) << vec[i];
        }
    }
    oss << "]";
    ܦܢܐ oss.str();
}

ساكن مضمن std::ostream& noFormat(std::ostream& os)
{
    ܦܢܐ os;
}

ساكن مضمن std::ostream& floatFormat(std::ostream& os)
{
    ܦܢܐ os << std::fixed << std::setprecision(2);
}

String dumpVectorOfInt(ثابت std::vector<صحيح>& vec)
{
    ܦܢܐ dumpVector(vec, &noFormat);
}

String dumpVectorOfDouble(ثابت std::vector<مزدوج>& vec)
{
    ܦܢܐ dumpVector(vec, &floatFormat);
}

String dumpVectorOfRect(ثابت std::vector<Rect>& vec)
{
    ܦܢܐ dumpVector(vec, &noFormat);
}


نطاق fs {
cv::String getCacheDirectoryForDownloads()
{
#ܐܢ OPENCV_HAVE_FILESYSTEM_SUPPORT
    ܦܢܐ cv::utils::fs::getCacheDirectory("downloads", "OPENCV_DOWNLOADS_CACHE_DIR");
#ܐܚܪܢܐ
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#endif
}
} // namespace fs

}} // namespace
