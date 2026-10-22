// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#অন্তর্ভাৱ "precomp.hpp"
#অন্তর্ভাৱ "opencv2/core/bindings_utils.hpp"
#অন্তর্ভাৱ <sstream>
#অন্তর্ভাৱ <iomanip>
#অন্তর্ভাৱ <opencv2/core/utils/filesystem.hpp>
#অন্তর্ভাৱ <opencv2/core/utils/filesystem.private.hpp>

নামাকাশ cv {
স্থির অন্তঃস্থ std::ostream& operator<<(std::ostream& os, ধ্রুৱ Rect& rect)
{
    ঘূৰাই os << "[x=" << rect.x << ", y=" << rect.y << ", w=" << rect.width << ", h=" << rect.height << ']';
}
নামাকাশ utils {

String dump_প্ৰবেশ_Array(প্ৰবেশ_Array argument)
{
    যদি (&argument == &noArray())
        ঘূৰাই "InputArray: noArray()";
    std::ostringstream ss;
    ss << "InputArray:";
    যত্ন {
        কৰা {
            ss << (argument.empty() ? " empty()=true" : " empty()=false");
            ss << cv::format(" kind=0x%08llx", (long long পূর্ণাঙ্ক)argument.kind());
            ss << cv::format(" flags=0x%08llx", (long long পূর্ণাঙ্ক)argument.getFlags());
            যদি (argument.getObj() == রিক্ত)
            {
                ss << " obj=NULL";
                ভঙ্গ; // done
            }
            ss << cv::format(" total(-1)=%lld", (long long পূর্ণাঙ্ক)argument.total(-1));
            পূর্ণাঙ্ক dims = argument.dims(-1);
            ss << cv::format(" dims(-1)=%d", dims);
            যদি (dims <= 2)
            {
                Size size = argument.size(-1);
                ss << cv::format(" size(-1)=%dx%d", size.width, size.height);
            }
            অন্যথা
            {
                পূর্ণাঙ্ক sz[CV_MAX_DIM] = {0};
                argument.sizend(sz, -1);
                ss << " size(-1)=[";
                বাবে (পূর্ণাঙ্ক i = 0; i < dims; i++)
                {
                    যদি (i > 0)
                        ss << ' ';
                    ss << sz[i];
                }
                ss << "]";
            }
            ss << " type(-1)=" << cv::typeToString(argument.type(-1));
        } যেতিয়ালৈ (0);
    }
    গৃহাণ (ধ্রুৱ std::exception& e)
    {
        ss << " ERROR: exception occurred: " << e.what();
    }
    গৃহাণ (...)
    {
        ss << " ERROR: unknown exception occurred, dump is non-complete";
    }
    ঘূৰাই ss.str();
}

CV_EXPORTS_W String dump_প্ৰবেশ_Array_Of_Arrays(প্ৰবেশ_Array_Of_Arrays argument)
{
    যদি (&argument == &noArray())
        ঘূৰাই "InputArrayOfArrays: noArray()";
    std::ostringstream ss;
    ss << "InputArrayOfArrays:";
    যত্ন {
        কৰা {
            ss << (argument.empty() ? " empty()=true" : " empty()=false");
            ss << cv::format(" kind=0x%08llx", (long long পূর্ণাঙ্ক)argument.kind());
            ss << cv::format(" flags=0x%08llx", (long long পূর্ণাঙ্ক)argument.getFlags());
            যদি (argument.getObj() == রিক্ত)
            {
                ss << " obj=NULL";
                ভঙ্গ; // done
            }
            ss << cv::format(" total(-1)=%lld", (long long পূর্ণাঙ্ক)argument.total(-1));
            ss << cv::format(" dims(-1)=%d", argument.dims(-1));
            Size size = argument.size(-1);
            ss << cv::format(" size(-1)=%dx%d", size.width, size.height);
            যদি (argument.total(-1) > 0)
            {
                ss << " type(0)=" << cv::typeToString(argument.type(0));
                পূর্ণাঙ্ক dims = argument.dims(0);
                ss << cv::format(" dims(0)=%d", dims);
                যদি (dims <= 2)
                {
                    Size size0 = argument.size(0);
                    ss << cv::format(" size(0)=%dx%d", size0.width, size0.height);
                }
                অন্যথা
                {
                    পূর্ণাঙ্ক sz[CV_MAX_DIM] = {0};
                    argument.sizend(sz, 0);
                    ss << " size(0)=[";
                    বাবে (পূর্ণাঙ্ক i = 0; i < dims; i++)
                    {
                        যদি (i > 0)
                            ss << ' ';
                        ss << sz[i];
                    }
                    ss << "]";
                }
            }
        } যেতিয়ালৈ (0);
    }
    গৃহাণ (ধ্রুৱ std::exception& e)
    {
        ss << " ERROR: exception occurred: " << e.what();
    }
    গৃহাণ (...)
    {
        ss << " ERROR: unknown exception occurred, dump is non-complete";
    }
    ঘূৰাই ss.str();
}

CV_EXPORTS_W String dump_প্ৰবেশ_Output_Array(প্ৰবেশ_Output_Array argument)
{
    যদি (&argument == &noArray())
        ঘূৰাই "InputOutputArray: noArray()";
    std::ostringstream ss;
    ss << "InputOutputArray:";
    যত্ন {
        কৰা {
            ss << (argument.empty() ? " empty()=true" : " empty()=false");
            ss << cv::format(" kind=0x%08llx", (long long পূর্ণাঙ্ক)argument.kind());
            ss << cv::format(" flags=0x%08llx", (long long পূর্ণাঙ্ক)argument.getFlags());
            যদি (argument.getObj() == রিক্ত)
            {
                ss << " obj=NULL";
                ভঙ্গ; // done
            }
            ss << cv::format(" total(-1)=%lld", (long long পূর্ণাঙ্ক)argument.total(-1));
            পূর্ণাঙ্ক dims = argument.dims(-1);
            ss << cv::format(" dims(-1)=%d", dims);
            যদি (dims <= 2)
            {
                Size size = argument.size(-1);
                ss << cv::format(" size(-1)=%dx%d", size.width, size.height);
            }
            অন্যথা
            {
                পূর্ণাঙ্ক sz[CV_MAX_DIM] = {0};
                argument.sizend(sz, -1);
                ss << " size(-1)=[";
                বাবে (পূর্ণাঙ্ক i = 0; i < dims; i++)
                {
                    যদি (i > 0)
                        ss << ' ';
                    ss << sz[i];
                }
                ss << "]";
            }
            ss << " type(-1)=" << cv::typeToString(argument.type(-1));
        } যেতিয়ালৈ (0);
    }
    গৃহাণ (ধ্রুৱ std::exception& e)
    {
        ss << " ERROR: exception occurred: " << e.what();
    }
    গৃহাণ (...)
    {
        ss << " ERROR: unknown exception occurred, dump is non-complete";
    }
    ঘূৰাই ss.str();
}

CV_EXPORTS_W String dump_প্ৰবেশ_Output_Array_Of_Arrays(প্ৰবেশ_Output_Array_Of_Arrays argument)
{
    যদি (&argument == &noArray())
        ঘূৰাই "InputOutputArrayOfArrays: noArray()";
    std::ostringstream ss;
    ss << "InputOutputArrayOfArrays:";
    যত্ন {
        কৰা {
            ss << (argument.empty() ? " empty()=true" : " empty()=false");
            ss << cv::format(" kind=0x%08llx", (long long পূর্ণাঙ্ক)argument.kind());
            ss << cv::format(" flags=0x%08llx", (long long পূর্ণাঙ্ক)argument.getFlags());
            যদি (argument.getObj() == রিক্ত)
            {
                ss << " obj=NULL";
                ভঙ্গ; // done
            }
            ss << cv::format(" total(-1)=%lld", (long long পূর্ণাঙ্ক)argument.total(-1));
            ss << cv::format(" dims(-1)=%d", argument.dims(-1));
            Size size = argument.size(-1);
            ss << cv::format(" size(-1)=%dx%d", size.width, size.height);
            যদি (argument.total(-1) > 0)
            {
                ss << " type(0)=" << cv::typeToString(argument.type(0));
                পূর্ণাঙ্ক dims = argument.dims(0);
                ss << cv::format(" dims(0)=%d", dims);
                যদি (dims <= 2)
                {
                    Size size0 = argument.size(0);
                    ss << cv::format(" size(0)=%dx%d", size0.width, size0.height);
                }
                অন্যথা
                {
                    পূর্ণাঙ্ক sz[CV_MAX_DIM] = {0};
                    argument.sizend(sz, 0);
                    ss << " size(0)=[";
                    বাবে (পূর্ণাঙ্ক i = 0; i < dims; i++)
                    {
                        যদি (i > 0)
                            ss << ' ';
                        ss << sz[i];
                    }
                    ss << "]";
                }
            }
        } যেতিয়ালৈ (0);
    }
    গৃহাণ (ধ্রুৱ std::exception& e)
    {
        ss << " ERROR: exception occurred: " << e.what();
    }
    গৃহাণ (...)
    {
        ss << " ERROR: unknown exception occurred, dump is non-complete";
    }
    ঘূৰাই ss.str();
}

template <শ্ৰেণী T, শ্ৰেণী Formatter>
স্থির অন্তঃস্থ String dumpVector(ধ্রুৱ std::vector<T>& vec, Formatter format)
{
    std::ostringstream oss("[", std::ios::ate);
    যদি (!vec.empty())
    {
        format(oss) << vec[0];
        বাবে (std::size_t i = 1; i < vec.size(); ++i)
        {
            oss << ", ";
            format(oss) << vec[i];
        }
    }
    oss << "]";
    ঘূৰাই oss.str();
}

স্থির অন্তঃস্থ std::ostream& noFormat(std::ostream& os)
{
    ঘূৰাই os;
}

স্থির অন্তঃস্থ std::ostream& floatFormat(std::ostream& os)
{
    ঘূৰাই os << std::fixed << std::setprecision(2);
}

String dumpVectorOfInt(ধ্রুৱ std::vector<পূর্ণাঙ্ক>& vec)
{
    ঘূৰাই dumpVector(vec, &noFormat);
}

String dumpVectorOfDouble(ধ্রুৱ std::vector<দ্ৱিগুণ>& vec)
{
    ঘূৰাই dumpVector(vec, &floatFormat);
}

String dumpVectorOfRect(ধ্রুৱ std::vector<Rect>& vec)
{
    ঘূৰাই dumpVector(vec, &noFormat);
}


নামাকাশ fs {
cv::String getCacheDirectoryForDownloads()
{
#যদি OPENCV_HAVE_FILESYSTEM_SUPPORT
    ঘূৰাই cv::utils::fs::getCacheDirectory("downloads", "OPENCV_DOWNLOADS_CACHE_DIR");
#অন্যথা
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#endif
}
} // namespace fs

}} // namespace
