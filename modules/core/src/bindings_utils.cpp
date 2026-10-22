// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#अन्तर्भाव "precomp.hpp"
#अन्तर्भाव "opencv2/core/bindings_utils.hpp"
#अन्तर्भाव <sstream>
#अन्तर्भाव <iomanip>
#अन्तर्भाव <opencv2/core/utils/filesystem.hpp>
#अन्तर्भाव <opencv2/core/utils/filesystem.private.hpp>

नामाकाश cv {
स्थिर ओळीत std::ostream& चालक<<(std::ostream& os, स्थिरांक Rect& rect)
{
    परत os << "[x=" << rect.x << ", y=" << rect.y << ", w=" << rect.width << ", h=" << rect.height << ']';
}
नामाकाश utils {

String dump_आदान_Array(आदान_Array argument)
{
    जर (&argument == &noArray())
        परत "InputArray: noArray()";
    std::ostringstream ss;
    ss << "InputArray:";
    प्रयत्न {
        करा {
            ss << (argument.empty() ? " empty()=true" : " empty()=false");
            ss << cv::format(" kind=0x%08llx", (दीर्घ दीर्घ पूर्णांक)argument.kind());
            ss << cv::format(" flags=0x%08llx", (दीर्घ दीर्घ पूर्णांक)argument.getFlags());
            जर (argument.getObj() == रिक्त)
            {
                ss << " obj=NULL";
                थांबा; // done
            }
            ss << cv::format(" total(-1)=%lld", (दीर्घ दीर्घ पूर्णांक)argument.total(-1));
            पूर्णांक dims = argument.dims(-1);
            ss << cv::format(" dims(-1)=%d", dims);
            जर (dims <= 2)
            {
                Size size = argument.size(-1);
                ss << cv::format(" size(-1)=%dx%d", size.width, size.height);
            }
            अन्यथा
            {
                पूर्णांक sz[CV_MAX_DIM] = {0};
                argument.sizend(sz, -1);
                ss << " size(-1)=[";
                साठी (पूर्णांक i = 0; i < dims; i++)
                {
                    जर (i > 0)
                        ss << ' ';
                    ss << sz[i];
                }
                ss << "]";
            }
            ss << " type(-1)=" << cv::typeToString(argument.type(-1));
        } पर्यंत (0);
    }
    पकडा (स्थिरांक std::exception& e)
    {
        ss << " ERROR: exception occurred: " << e.what();
    }
    पकडा (...)
    {
        ss << " ERROR: unknown exception occurred, dump is non-complete";
    }
    परत ss.str();
}

CV_EXPORTS_W String dump_आदान_Array_Of_Arrays(आदान_Array_Of_Arrays argument)
{
    जर (&argument == &noArray())
        परत "InputArrayOfArrays: noArray()";
    std::ostringstream ss;
    ss << "InputArrayOfArrays:";
    प्रयत्न {
        करा {
            ss << (argument.empty() ? " empty()=true" : " empty()=false");
            ss << cv::format(" kind=0x%08llx", (दीर्घ दीर्घ पूर्णांक)argument.kind());
            ss << cv::format(" flags=0x%08llx", (दीर्घ दीर्घ पूर्णांक)argument.getFlags());
            जर (argument.getObj() == रिक्त)
            {
                ss << " obj=NULL";
                थांबा; // done
            }
            ss << cv::format(" total(-1)=%lld", (दीर्घ दीर्घ पूर्णांक)argument.total(-1));
            ss << cv::format(" dims(-1)=%d", argument.dims(-1));
            Size size = argument.size(-1);
            ss << cv::format(" size(-1)=%dx%d", size.width, size.height);
            जर (argument.total(-1) > 0)
            {
                ss << " type(0)=" << cv::typeToString(argument.type(0));
                पूर्णांक dims = argument.dims(0);
                ss << cv::format(" dims(0)=%d", dims);
                जर (dims <= 2)
                {
                    Size size0 = argument.size(0);
                    ss << cv::format(" size(0)=%dx%d", size0.width, size0.height);
                }
                अन्यथा
                {
                    पूर्णांक sz[CV_MAX_DIM] = {0};
                    argument.sizend(sz, 0);
                    ss << " size(0)=[";
                    साठी (पूर्णांक i = 0; i < dims; i++)
                    {
                        जर (i > 0)
                            ss << ' ';
                        ss << sz[i];
                    }
                    ss << "]";
                }
            }
        } पर्यंत (0);
    }
    पकडा (स्थिरांक std::exception& e)
    {
        ss << " ERROR: exception occurred: " << e.what();
    }
    पकडा (...)
    {
        ss << " ERROR: unknown exception occurred, dump is non-complete";
    }
    परत ss.str();
}

CV_EXPORTS_W String dump_आदान_Output_Array(आदान_Output_Array argument)
{
    जर (&argument == &noArray())
        परत "InputOutputArray: noArray()";
    std::ostringstream ss;
    ss << "InputOutputArray:";
    प्रयत्न {
        करा {
            ss << (argument.empty() ? " empty()=true" : " empty()=false");
            ss << cv::format(" kind=0x%08llx", (दीर्घ दीर्घ पूर्णांक)argument.kind());
            ss << cv::format(" flags=0x%08llx", (दीर्घ दीर्घ पूर्णांक)argument.getFlags());
            जर (argument.getObj() == रिक्त)
            {
                ss << " obj=NULL";
                थांबा; // done
            }
            ss << cv::format(" total(-1)=%lld", (दीर्घ दीर्घ पूर्णांक)argument.total(-1));
            पूर्णांक dims = argument.dims(-1);
            ss << cv::format(" dims(-1)=%d", dims);
            जर (dims <= 2)
            {
                Size size = argument.size(-1);
                ss << cv::format(" size(-1)=%dx%d", size.width, size.height);
            }
            अन्यथा
            {
                पूर्णांक sz[CV_MAX_DIM] = {0};
                argument.sizend(sz, -1);
                ss << " size(-1)=[";
                साठी (पूर्णांक i = 0; i < dims; i++)
                {
                    जर (i > 0)
                        ss << ' ';
                    ss << sz[i];
                }
                ss << "]";
            }
            ss << " type(-1)=" << cv::typeToString(argument.type(-1));
        } पर्यंत (0);
    }
    पकडा (स्थिरांक std::exception& e)
    {
        ss << " ERROR: exception occurred: " << e.what();
    }
    पकडा (...)
    {
        ss << " ERROR: unknown exception occurred, dump is non-complete";
    }
    परत ss.str();
}

CV_EXPORTS_W String dump_आदान_Output_Array_Of_Arrays(आदान_Output_Array_Of_Arrays argument)
{
    जर (&argument == &noArray())
        परत "InputOutputArrayOfArrays: noArray()";
    std::ostringstream ss;
    ss << "InputOutputArrayOfArrays:";
    प्रयत्न {
        करा {
            ss << (argument.empty() ? " empty()=true" : " empty()=false");
            ss << cv::format(" kind=0x%08llx", (दीर्घ दीर्घ पूर्णांक)argument.kind());
            ss << cv::format(" flags=0x%08llx", (दीर्घ दीर्घ पूर्णांक)argument.getFlags());
            जर (argument.getObj() == रिक्त)
            {
                ss << " obj=NULL";
                थांबा; // done
            }
            ss << cv::format(" total(-1)=%lld", (दीर्घ दीर्घ पूर्णांक)argument.total(-1));
            ss << cv::format(" dims(-1)=%d", argument.dims(-1));
            Size size = argument.size(-1);
            ss << cv::format(" size(-1)=%dx%d", size.width, size.height);
            जर (argument.total(-1) > 0)
            {
                ss << " type(0)=" << cv::typeToString(argument.type(0));
                पूर्णांक dims = argument.dims(0);
                ss << cv::format(" dims(0)=%d", dims);
                जर (dims <= 2)
                {
                    Size size0 = argument.size(0);
                    ss << cv::format(" size(0)=%dx%d", size0.width, size0.height);
                }
                अन्यथा
                {
                    पूर्णांक sz[CV_MAX_DIM] = {0};
                    argument.sizend(sz, 0);
                    ss << " size(0)=[";
                    साठी (पूर्णांक i = 0; i < dims; i++)
                    {
                        जर (i > 0)
                            ss << ' ';
                        ss << sz[i];
                    }
                    ss << "]";
                }
            }
        } पर्यंत (0);
    }
    पकडा (स्थिरांक std::exception& e)
    {
        ss << " ERROR: exception occurred: " << e.what();
    }
    पकडा (...)
    {
        ss << " ERROR: unknown exception occurred, dump is non-complete";
    }
    परत ss.str();
}

ढाँचा <वर्ग T, वर्ग Formatter>
स्थिर ओळीत String dumpVector(स्थिरांक std::vector<T>& vec, Formatter format)
{
    std::ostringstream oss("[", std::ios::ate);
    जर (!vec.empty())
    {
        format(oss) << vec[0];
        साठी (std::माप_प्रकार i = 1; i < vec.size(); ++i)
        {
            oss << ", ";
            format(oss) << vec[i];
        }
    }
    oss << "]";
    परत oss.str();
}

स्थिर ओळीत std::ostream& noFormat(std::ostream& os)
{
    परत os;
}

स्थिर ओळीत std::ostream& floatFormat(std::ostream& os)
{
    परत os << std::fixed << std::setprecision(2);
}

String dumpVectorOfInt(स्थिरांक std::vector<पूर्णांक>& vec)
{
    परत dumpVector(vec, &noFormat);
}

String dumpVectorOfDouble(स्थिरांक std::vector<द्विगुण>& vec)
{
    परत dumpVector(vec, &floatFormat);
}

String dumpVectorOfRect(स्थिरांक std::vector<Rect>& vec)
{
    परत dumpVector(vec, &noFormat);
}


नामाकाश fs {
cv::String getCacheDirectoryForDownloads()
{
#जर OPENCV_HAVE_FILESYSTEM_SUPPORT
    परत cv::utils::fs::getCacheDirectory("downloads", "OPENCV_DOWNLOADS_CACHE_DIR");
#अन्यथा
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#endif
}
} // namespace fs

}} // namespace
