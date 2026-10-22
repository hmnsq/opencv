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
    החזר os << "[x=" << rect.x << ", y=" << rect.y << ", w=" << rect.width << ", h=" << rect.height << ']';
}
نطاق utils {

String dumpInputArray(InputArray argument)
{
    אם (&argument == &noArray())
        החזר "InputArray: noArray()";
    std::ostringstream ss;
    ss << "InputArray:";
    حاول {
        עשה {
            ss << (argument.empty() ? " empty()=true" : " empty()=false");
            ss << cv::format(" kind=0x%08llx", (long long صحيح)argument.kind());
            ss << cv::format(" flags=0x%08llx", (long long صحيح)argument.getFlags());
            אם (argument.getObj() == عدم)
            {
                ss << " obj=NULL";
                اكسر; // done
            }
            ss << cv::format(" total(-1)=%lld", (long long صحيح)argument.total(-1));
            صحيح dims = argument.dims(-1);
            ss << cv::format(" dims(-1)=%d", dims);
            אם (dims <= 2)
            {
                Size size = argument.size(-1);
                ss << cv::format(" size(-1)=%dx%d", size.width, size.height);
            }
            אחרת
            {
                صحيح sz[CV_MAX_DIM] = {0};
                argument.sizend(sz, -1);
                ss << " size(-1)=[";
                עבור (صحيح i = 0; i < dims; i++)
                {
                    אם (i > 0)
                        ss << ' ';
                    ss << sz[i];
                }
                ss << "]";
            }
            ss << " type(-1)=" << cv::typeToString(argument.type(-1));
        } כלעוד (0);
    }
    امسك (ثابت std::exception& e)
    {
        ss << " ERROR: exception occurred: " << e.what();
    }
    امسك (...)
    {
        ss << " ERROR: unknown exception occurred, dump is non-complete";
    }
    החזר ss.str();
}

CV_EXPORTS_W String dumpInputArrayOfArrays(InputArrayOfArrays argument)
{
    אם (&argument == &noArray())
        החזר "InputArrayOfArrays: noArray()";
    std::ostringstream ss;
    ss << "InputArrayOfArrays:";
    حاول {
        עשה {
            ss << (argument.empty() ? " empty()=true" : " empty()=false");
            ss << cv::format(" kind=0x%08llx", (long long صحيح)argument.kind());
            ss << cv::format(" flags=0x%08llx", (long long صحيح)argument.getFlags());
            אם (argument.getObj() == عدم)
            {
                ss << " obj=NULL";
                اكسر; // done
            }
            ss << cv::format(" total(-1)=%lld", (long long صحيح)argument.total(-1));
            ss << cv::format(" dims(-1)=%d", argument.dims(-1));
            Size size = argument.size(-1);
            ss << cv::format(" size(-1)=%dx%d", size.width, size.height);
            אם (argument.total(-1) > 0)
            {
                ss << " type(0)=" << cv::typeToString(argument.type(0));
                صحيح dims = argument.dims(0);
                ss << cv::format(" dims(0)=%d", dims);
                אם (dims <= 2)
                {
                    Size size0 = argument.size(0);
                    ss << cv::format(" size(0)=%dx%d", size0.width, size0.height);
                }
                אחרת
                {
                    صحيح sz[CV_MAX_DIM] = {0};
                    argument.sizend(sz, 0);
                    ss << " size(0)=[";
                    עבור (صحيح i = 0; i < dims; i++)
                    {
                        אם (i > 0)
                            ss << ' ';
                        ss << sz[i];
                    }
                    ss << "]";
                }
            }
        } כלעוד (0);
    }
    امسك (ثابت std::exception& e)
    {
        ss << " ERROR: exception occurred: " << e.what();
    }
    امسك (...)
    {
        ss << " ERROR: unknown exception occurred, dump is non-complete";
    }
    החזר ss.str();
}

CV_EXPORTS_W String dumpInputOutputArray(InputOutputArray argument)
{
    אם (&argument == &noArray())
        החזר "InputOutputArray: noArray()";
    std::ostringstream ss;
    ss << "InputOutputArray:";
    حاول {
        עשה {
            ss << (argument.empty() ? " empty()=true" : " empty()=false");
            ss << cv::format(" kind=0x%08llx", (long long صحيح)argument.kind());
            ss << cv::format(" flags=0x%08llx", (long long صحيح)argument.getFlags());
            אם (argument.getObj() == عدم)
            {
                ss << " obj=NULL";
                اكسر; // done
            }
            ss << cv::format(" total(-1)=%lld", (long long صحيح)argument.total(-1));
            صحيح dims = argument.dims(-1);
            ss << cv::format(" dims(-1)=%d", dims);
            אם (dims <= 2)
            {
                Size size = argument.size(-1);
                ss << cv::format(" size(-1)=%dx%d", size.width, size.height);
            }
            אחרת
            {
                صحيح sz[CV_MAX_DIM] = {0};
                argument.sizend(sz, -1);
                ss << " size(-1)=[";
                עבור (صحيح i = 0; i < dims; i++)
                {
                    אם (i > 0)
                        ss << ' ';
                    ss << sz[i];
                }
                ss << "]";
            }
            ss << " type(-1)=" << cv::typeToString(argument.type(-1));
        } כלעוד (0);
    }
    امسك (ثابت std::exception& e)
    {
        ss << " ERROR: exception occurred: " << e.what();
    }
    امسك (...)
    {
        ss << " ERROR: unknown exception occurred, dump is non-complete";
    }
    החזר ss.str();
}

CV_EXPORTS_W String dumpInputOutputArrayOfArrays(InputOutputArrayOfArrays argument)
{
    אם (&argument == &noArray())
        החזר "InputOutputArrayOfArrays: noArray()";
    std::ostringstream ss;
    ss << "InputOutputArrayOfArrays:";
    حاول {
        עשה {
            ss << (argument.empty() ? " empty()=true" : " empty()=false");
            ss << cv::format(" kind=0x%08llx", (long long صحيح)argument.kind());
            ss << cv::format(" flags=0x%08llx", (long long صحيح)argument.getFlags());
            אם (argument.getObj() == عدم)
            {
                ss << " obj=NULL";
                اكسر; // done
            }
            ss << cv::format(" total(-1)=%lld", (long long صحيح)argument.total(-1));
            ss << cv::format(" dims(-1)=%d", argument.dims(-1));
            Size size = argument.size(-1);
            ss << cv::format(" size(-1)=%dx%d", size.width, size.height);
            אם (argument.total(-1) > 0)
            {
                ss << " type(0)=" << cv::typeToString(argument.type(0));
                صحيح dims = argument.dims(0);
                ss << cv::format(" dims(0)=%d", dims);
                אם (dims <= 2)
                {
                    Size size0 = argument.size(0);
                    ss << cv::format(" size(0)=%dx%d", size0.width, size0.height);
                }
                אחרת
                {
                    صحيح sz[CV_MAX_DIM] = {0};
                    argument.sizend(sz, 0);
                    ss << " size(0)=[";
                    עבור (صحيح i = 0; i < dims; i++)
                    {
                        אם (i > 0)
                            ss << ' ';
                        ss << sz[i];
                    }
                    ss << "]";
                }
            }
        } כלעוד (0);
    }
    امسك (ثابت std::exception& e)
    {
        ss << " ERROR: exception occurred: " << e.what();
    }
    امسك (...)
    {
        ss << " ERROR: unknown exception occurred, dump is non-complete";
    }
    החזר ss.str();
}

template <מחלקה T, מחלקה Formatter>
ساكن مضمن String dumpVector(ثابت std::vector<T>& vec, Formatter format)
{
    std::ostringstream oss("[", std::ios::ate);
    אם (!vec.empty())
    {
        format(oss) << vec[0];
        עבור (std::size_t i = 1; i < vec.size(); ++i)
        {
            oss << ", ";
            format(oss) << vec[i];
        }
    }
    oss << "]";
    החזר oss.str();
}

ساكن مضمن std::ostream& noFormat(std::ostream& os)
{
    החזר os;
}

ساكن مضمن std::ostream& floatFormat(std::ostream& os)
{
    החזר os << std::fixed << std::setprecision(2);
}

String dumpVectorOfInt(ثابت std::vector<صحيح>& vec)
{
    החזר dumpVector(vec, &noFormat);
}

String dumpVectorOfDouble(ثابت std::vector<مزدوج>& vec)
{
    החזר dumpVector(vec, &floatFormat);
}

String dumpVectorOfRect(ثابت std::vector<Rect>& vec)
{
    החזר dumpVector(vec, &noFormat);
}


نطاق fs {
cv::String getCacheDirectoryForDownloads()
{
#אם OPENCV_HAVE_FILESYSTEM_SUPPORT
    החזר cv::utils::fs::getCacheDirectory("downloads", "OPENCV_DOWNLOADS_CACHE_DIR");
#אחרת
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#endif
}
} // namespace fs

}} // namespace
