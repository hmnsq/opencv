// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#অন্তর্ভাব "precomp.hpp"
#অন্তর্ভাব "opencv2/core/bindings_utils.hpp"
#অন্তর্ভাব <sstream>
#অন্তর্ভাব <iomanip>
#অন্তর্ভাব <opencv2/core/utils/filesystem.hpp>
#অন্তর্ভাব <opencv2/core/utils/filesystem.private.hpp>

নামাকাশ cv {
স্থির অন্তঃরেখ std::ostream& operator<<(std::ostream& os, ধ্রুবক Rect& rect)
{
    ফেরত os << "[x=" << rect.x << ", y=" << rect.y << ", w=" << rect.width << ", h=" << rect.height << ']';
}
নামাকাশ utils {

String dump_প্রবেশ_Array(প্রবেশ_Array argument)
{
    যদি (&argument == &noArray())
        ফেরত "InputArray: noArray()";
    std::ostringstream ss;
    ss << "InputArray:";
    চেষ্টা {
        করো {
            ss << (argument.empty() ? " empty()=true" : " empty()=false");
            ss << cv::format(" kind=0x%08llx", (long long পূর্ণসংখ্যা)argument.kind());
            ss << cv::format(" flags=0x%08llx", (long long পূর্ণসংখ্যা)argument.getFlags());
            যদি (argument.getObj() == ফাঁকা)
            {
                ss << " obj=NULL";
                বিরতি; // done
            }
            ss << cv::format(" total(-1)=%lld", (long long পূর্ণসংখ্যা)argument.যোগ(-1));
            পূর্ণসংখ্যা dims = argument.dims(-1);
            ss << cv::format(" dims(-1)=%d", dims);
            যদি (dims <= 2)
            {
                আকার_2 size = argument.size(-1);
                ss << cv::format(" size(-1)=%dx%d", size.width, size.height);
            }
            অন্যথা
            {
                পূর্ণসংখ্যা sz[CV_MAX_DIM] = {0};
                argument.sizend(sz, -1);
                ss << " size(-1)=[";
                জন্য (পূর্ণসংখ্যা i = 0; i < dims; i++)
                {
                    যদি (i > 0)
                        ss << ' ';
                    ss << sz[i];
                }
                ss << "]";
            }
            ss << " type(-1)=" << cv::typeToString(argument.type(-1));
        } যখন (0);
    }
    ধরো (ধ্রুবক std::exception& e)
    {
        ss << " ERROR: exception occurred: " << e.what();
    }
    ধরো (...)
    {
        ss << " ERROR: unknown exception occurred, dump is non-complete";
    }
    ফেরত ss.str();
}

CV_EXPORTS_W String dump_প্রবেশ_Array_Of_Arrays(প্রবেশ_Array_Of_Arrays argument)
{
    যদি (&argument == &noArray())
        ফেরত "InputArrayOfArrays: noArray()";
    std::ostringstream ss;
    ss << "InputArrayOfArrays:";
    চেষ্টা {
        করো {
            ss << (argument.empty() ? " empty()=true" : " empty()=false");
            ss << cv::format(" kind=0x%08llx", (long long পূর্ণসংখ্যা)argument.kind());
            ss << cv::format(" flags=0x%08llx", (long long পূর্ণসংখ্যা)argument.getFlags());
            যদি (argument.getObj() == ফাঁকা)
            {
                ss << " obj=NULL";
                বিরতি; // done
            }
            ss << cv::format(" total(-1)=%lld", (long long পূর্ণসংখ্যা)argument.যোগ(-1));
            ss << cv::format(" dims(-1)=%d", argument.dims(-1));
            আকার_2 size = argument.size(-1);
            ss << cv::format(" size(-1)=%dx%d", size.width, size.height);
            যদি (argument.যোগ(-1) > 0)
            {
                ss << " type(0)=" << cv::typeToString(argument.type(0));
                পূর্ণসংখ্যা dims = argument.dims(0);
                ss << cv::format(" dims(0)=%d", dims);
                যদি (dims <= 2)
                {
                    আকার_2 size0 = argument.size(0);
                    ss << cv::format(" size(0)=%dx%d", size0.width, size0.height);
                }
                অন্যথা
                {
                    পূর্ণসংখ্যা sz[CV_MAX_DIM] = {0};
                    argument.sizend(sz, 0);
                    ss << " size(0)=[";
                    জন্য (পূর্ণসংখ্যা i = 0; i < dims; i++)
                    {
                        যদি (i > 0)
                            ss << ' ';
                        ss << sz[i];
                    }
                    ss << "]";
                }
            }
        } যখন (0);
    }
    ধরো (ধ্রুবক std::exception& e)
    {
        ss << " ERROR: exception occurred: " << e.what();
    }
    ধরো (...)
    {
        ss << " ERROR: unknown exception occurred, dump is non-complete";
    }
    ফেরত ss.str();
}

CV_EXPORTS_W String dump_প্রবেশ_Output_Array(প্রবেশ_Output_Array argument)
{
    যদি (&argument == &noArray())
        ফেরত "InputOutputArray: noArray()";
    std::ostringstream ss;
    ss << "InputOutputArray:";
    চেষ্টা {
        করো {
            ss << (argument.empty() ? " empty()=true" : " empty()=false");
            ss << cv::format(" kind=0x%08llx", (long long পূর্ণসংখ্যা)argument.kind());
            ss << cv::format(" flags=0x%08llx", (long long পূর্ণসংখ্যা)argument.getFlags());
            যদি (argument.getObj() == ফাঁকা)
            {
                ss << " obj=NULL";
                বিরতি; // done
            }
            ss << cv::format(" total(-1)=%lld", (long long পূর্ণসংখ্যা)argument.যোগ(-1));
            পূর্ণসংখ্যা dims = argument.dims(-1);
            ss << cv::format(" dims(-1)=%d", dims);
            যদি (dims <= 2)
            {
                আকার_2 size = argument.size(-1);
                ss << cv::format(" size(-1)=%dx%d", size.width, size.height);
            }
            অন্যথা
            {
                পূর্ণসংখ্যা sz[CV_MAX_DIM] = {0};
                argument.sizend(sz, -1);
                ss << " size(-1)=[";
                জন্য (পূর্ণসংখ্যা i = 0; i < dims; i++)
                {
                    যদি (i > 0)
                        ss << ' ';
                    ss << sz[i];
                }
                ss << "]";
            }
            ss << " type(-1)=" << cv::typeToString(argument.type(-1));
        } যখন (0);
    }
    ধরো (ধ্রুবক std::exception& e)
    {
        ss << " ERROR: exception occurred: " << e.what();
    }
    ধরো (...)
    {
        ss << " ERROR: unknown exception occurred, dump is non-complete";
    }
    ফেরত ss.str();
}

CV_EXPORTS_W String dump_প্রবেশ_Output_Array_Of_Arrays(প্রবেশ_Output_Array_Of_Arrays argument)
{
    যদি (&argument == &noArray())
        ফেরত "InputOutputArrayOfArrays: noArray()";
    std::ostringstream ss;
    ss << "InputOutputArrayOfArrays:";
    চেষ্টা {
        করো {
            ss << (argument.empty() ? " empty()=true" : " empty()=false");
            ss << cv::format(" kind=0x%08llx", (long long পূর্ণসংখ্যা)argument.kind());
            ss << cv::format(" flags=0x%08llx", (long long পূর্ণসংখ্যা)argument.getFlags());
            যদি (argument.getObj() == ফাঁকা)
            {
                ss << " obj=NULL";
                বিরতি; // done
            }
            ss << cv::format(" total(-1)=%lld", (long long পূর্ণসংখ্যা)argument.যোগ(-1));
            ss << cv::format(" dims(-1)=%d", argument.dims(-1));
            আকার_2 size = argument.size(-1);
            ss << cv::format(" size(-1)=%dx%d", size.width, size.height);
            যদি (argument.যোগ(-1) > 0)
            {
                ss << " type(0)=" << cv::typeToString(argument.type(0));
                পূর্ণসংখ্যা dims = argument.dims(0);
                ss << cv::format(" dims(0)=%d", dims);
                যদি (dims <= 2)
                {
                    আকার_2 size0 = argument.size(0);
                    ss << cv::format(" size(0)=%dx%d", size0.width, size0.height);
                }
                অন্যথা
                {
                    পূর্ণসংখ্যা sz[CV_MAX_DIM] = {0};
                    argument.sizend(sz, 0);
                    ss << " size(0)=[";
                    জন্য (পূর্ণসংখ্যা i = 0; i < dims; i++)
                    {
                        যদি (i > 0)
                            ss << ' ';
                        ss << sz[i];
                    }
                    ss << "]";
                }
            }
        } যখন (0);
    }
    ধরো (ধ্রুবক std::exception& e)
    {
        ss << " ERROR: exception occurred: " << e.what();
    }
    ধরো (...)
    {
        ss << " ERROR: unknown exception occurred, dump is non-complete";
    }
    ফেরত ss.str();
}

template <শ্রেণী T, শ্রেণী Formatter>
স্থির অন্তঃরেখ String dumpVector(ধ্রুবক std::vector<T>& vec, Formatter format)
{
    std::ostringstream oss("[", std::ios::ate);
    যদি (!vec.empty())
    {
        format(oss) << vec[0];
        জন্য (std::size_t i = 1; i < vec.size(); ++i)
        {
            oss << ", ";
            format(oss) << vec[i];
        }
    }
    oss << "]";
    ফেরত oss.str();
}

স্থির অন্তঃরেখ std::ostream& noFormat(std::ostream& os)
{
    ফেরত os;
}

স্থির অন্তঃরেখ std::ostream& floatFormat(std::ostream& os)
{
    ফেরত os << std::fixed << std::setprecision(2);
}

String dumpVectorOfInt(ধ্রুবক std::vector<পূর্ণসংখ্যা>& vec)
{
    ফেরত dumpVector(vec, &noFormat);
}

String dumpVectorOfDouble(ধ্রুবক std::vector<দ্বিগুণ>& vec)
{
    ফেরত dumpVector(vec, &floatFormat);
}

String dumpVectorOfRect(ধ্রুবক std::vector<Rect>& vec)
{
    ফেরত dumpVector(vec, &noFormat);
}


নামাকাশ fs {
cv::String getCacheDirectoryForDownloads()
{
#যদি OPENCV_HAVE_FILESYSTEM_SUPPORT
    ফেরত cv::utils::fs::getCacheDirectory("downloads", "OPENCV_DOWNLOADS_CACHE_DIR");
#অন্যথা
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#endif
}
} // namespace fs

}} // namespace
