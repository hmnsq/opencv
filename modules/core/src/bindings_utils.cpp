// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#समावेश "precomp.hpp"
#समावेश "opencv2/core/bindings_utils.hpp"
#समावेश <sstream>
#समावेश <iomanip>
#समावेश <opencv2/core/utils/filesystem.hpp>
#समावेश <opencv2/core/utils/filesystem.private.hpp>

namespace cv {
अटल अंतरभूत std::ब_प्रवाह& चालक<<(std::ब_प्रवाह& os, स्थिर Rect& rect)
{
    वापस os << "[x=" << rect.x << ", y=" << rect.y << ", w=" << rect.width << ", h=" << rect.height << ']';
}
namespace utils {

String dump_निवेश_Array(निवेश_Array argument)
{
    अगर (&argument == &noArray())
        वापस "InputArray: noArray()";
    std::ostringstream ss;
    ss << "InputArray:";
    प्रयास {
        करो {
            ss << (argument.empty() ? " empty()=true" : " empty()=false");
            ss << cv::format(" kind=0x%08llx", (दीर्घ दीर्घ पूर्णांक)argument.kind());
            ss << cv::format(" flags=0x%08llx", (दीर्घ दीर्घ पूर्णांक)argument.getFlags());
            अगर (argument.getObj() == शून्य)
            {
                ss << " obj=NULL";
                अवरोध; // done
            }
            ss << cv::format(" total(-1)=%lld", (दीर्घ दीर्घ पूर्णांक)argument.योग(-1));
            पूर्णांक dims = argument.dims(-1);
            ss << cv::format(" dims(-1)=%d", dims);
            अगर (dims <= 2)
            {
                आकार size = argument.size(-1);
                ss << cv::format(" size(-1)=%dx%d", size.width, size.height);
            }
            अन्यथा
            {
                पूर्णांक sz[CV_MAX_DIM] = {0};
                argument.sizend(sz, -1);
                ss << " size(-1)=[";
                क्रम (पूर्णांक i = 0; i < dims; i++)
                {
                    अगर (i > 0)
                        ss << ' ';
                    ss << sz[i];
                }
                ss << "]";
            }
            ss << " type(-1)=" << cv::typeToString(argument.type(-1));
        } जबतक (0);
    }
    पकड़ो (स्थिर std::exception& e)
    {
        ss << " ERROR: exception occurred: " << e.what();
    }
    पकड़ो (...)
    {
        ss << " ERROR: unknown exception occurred, dump is non-complete";
    }
    वापस ss.str();
}

CV_EXPORTS_W String dump_निवेश_Array_Of_Arrays(निवेश_Array_Of_Arrays argument)
{
    अगर (&argument == &noArray())
        वापस "InputArrayOfArrays: noArray()";
    std::ostringstream ss;
    ss << "InputArrayOfArrays:";
    प्रयास {
        करो {
            ss << (argument.empty() ? " empty()=true" : " empty()=false");
            ss << cv::format(" kind=0x%08llx", (दीर्घ दीर्घ पूर्णांक)argument.kind());
            ss << cv::format(" flags=0x%08llx", (दीर्घ दीर्घ पूर्णांक)argument.getFlags());
            अगर (argument.getObj() == शून्य)
            {
                ss << " obj=NULL";
                अवरोध; // done
            }
            ss << cv::format(" total(-1)=%lld", (दीर्घ दीर्घ पूर्णांक)argument.योग(-1));
            ss << cv::format(" dims(-1)=%d", argument.dims(-1));
            आकार size = argument.size(-1);
            ss << cv::format(" size(-1)=%dx%d", size.width, size.height);
            अगर (argument.योग(-1) > 0)
            {
                ss << " type(0)=" << cv::typeToString(argument.type(0));
                पूर्णांक dims = argument.dims(0);
                ss << cv::format(" dims(0)=%d", dims);
                अगर (dims <= 2)
                {
                    आकार size0 = argument.size(0);
                    ss << cv::format(" size(0)=%dx%d", size0.width, size0.height);
                }
                अन्यथा
                {
                    पूर्णांक sz[CV_MAX_DIM] = {0};
                    argument.sizend(sz, 0);
                    ss << " size(0)=[";
                    क्रम (पूर्णांक i = 0; i < dims; i++)
                    {
                        अगर (i > 0)
                            ss << ' ';
                        ss << sz[i];
                    }
                    ss << "]";
                }
            }
        } जबतक (0);
    }
    पकड़ो (स्थिर std::exception& e)
    {
        ss << " ERROR: exception occurred: " << e.what();
    }
    पकड़ो (...)
    {
        ss << " ERROR: unknown exception occurred, dump is non-complete";
    }
    वापस ss.str();
}

CV_EXPORTS_W String dump_निवेश_Output_Array(निवेश_Output_Array argument)
{
    अगर (&argument == &noArray())
        वापस "InputOutputArray: noArray()";
    std::ostringstream ss;
    ss << "InputOutputArray:";
    प्रयास {
        करो {
            ss << (argument.empty() ? " empty()=true" : " empty()=false");
            ss << cv::format(" kind=0x%08llx", (दीर्घ दीर्घ पूर्णांक)argument.kind());
            ss << cv::format(" flags=0x%08llx", (दीर्घ दीर्घ पूर्णांक)argument.getFlags());
            अगर (argument.getObj() == शून्य)
            {
                ss << " obj=NULL";
                अवरोध; // done
            }
            ss << cv::format(" total(-1)=%lld", (दीर्घ दीर्घ पूर्णांक)argument.योग(-1));
            पूर्णांक dims = argument.dims(-1);
            ss << cv::format(" dims(-1)=%d", dims);
            अगर (dims <= 2)
            {
                आकार size = argument.size(-1);
                ss << cv::format(" size(-1)=%dx%d", size.width, size.height);
            }
            अन्यथा
            {
                पूर्णांक sz[CV_MAX_DIM] = {0};
                argument.sizend(sz, -1);
                ss << " size(-1)=[";
                क्रम (पूर्णांक i = 0; i < dims; i++)
                {
                    अगर (i > 0)
                        ss << ' ';
                    ss << sz[i];
                }
                ss << "]";
            }
            ss << " type(-1)=" << cv::typeToString(argument.type(-1));
        } जबतक (0);
    }
    पकड़ो (स्थिर std::exception& e)
    {
        ss << " ERROR: exception occurred: " << e.what();
    }
    पकड़ो (...)
    {
        ss << " ERROR: unknown exception occurred, dump is non-complete";
    }
    वापस ss.str();
}

CV_EXPORTS_W String dump_निवेश_Output_Array_Of_Arrays(निवेश_Output_Array_Of_Arrays argument)
{
    अगर (&argument == &noArray())
        वापस "InputOutputArrayOfArrays: noArray()";
    std::ostringstream ss;
    ss << "InputOutputArrayOfArrays:";
    प्रयास {
        करो {
            ss << (argument.empty() ? " empty()=true" : " empty()=false");
            ss << cv::format(" kind=0x%08llx", (दीर्घ दीर्घ पूर्णांक)argument.kind());
            ss << cv::format(" flags=0x%08llx", (दीर्घ दीर्घ पूर्णांक)argument.getFlags());
            अगर (argument.getObj() == शून्य)
            {
                ss << " obj=NULL";
                अवरोध; // done
            }
            ss << cv::format(" total(-1)=%lld", (दीर्घ दीर्घ पूर्णांक)argument.योग(-1));
            ss << cv::format(" dims(-1)=%d", argument.dims(-1));
            आकार size = argument.size(-1);
            ss << cv::format(" size(-1)=%dx%d", size.width, size.height);
            अगर (argument.योग(-1) > 0)
            {
                ss << " type(0)=" << cv::typeToString(argument.type(0));
                पूर्णांक dims = argument.dims(0);
                ss << cv::format(" dims(0)=%d", dims);
                अगर (dims <= 2)
                {
                    आकार size0 = argument.size(0);
                    ss << cv::format(" size(0)=%dx%d", size0.width, size0.height);
                }
                अन्यथा
                {
                    पूर्णांक sz[CV_MAX_DIM] = {0};
                    argument.sizend(sz, 0);
                    ss << " size(0)=[";
                    क्रम (पूर्णांक i = 0; i < dims; i++)
                    {
                        अगर (i > 0)
                            ss << ' ';
                        ss << sz[i];
                    }
                    ss << "]";
                }
            }
        } जबतक (0);
    }
    पकड़ो (स्थिर std::exception& e)
    {
        ss << " ERROR: exception occurred: " << e.what();
    }
    पकड़ो (...)
    {
        ss << " ERROR: unknown exception occurred, dump is non-complete";
    }
    वापस ss.str();
}

ढाँचा <श्रेणी T, श्रेणी Formatter>
अटल अंतरभूत String dumpVector(स्थिर std::vector<T>& vec, Formatter format)
{
    std::ostringstream oss("[", std::पनप::अंत_पर);
    अगर (!vec.empty())
    {
        format(oss) << vec[0];
        क्रम (std::माप_प्रकार i = 1; i < vec.size(); ++i)
        {
            oss << ", ";
            format(oss) << vec[i];
        }
    }
    oss << "]";
    वापस oss.str();
}

अटल अंतरभूत std::ब_प्रवाह& noFormat(std::ब_प्रवाह& os)
{
    वापस os;
}

अटल अंतरभूत std::ब_प्रवाह& floatFormat(std::ब_प्रवाह& os)
{
    वापस os << std::जड़ << std::रखो_सटीक(2);
}

String dumpVectorOfInt(स्थिर std::vector<पूर्णांक>& vec)
{
    वापस dumpVector(vec, &noFormat);
}

String dumpVectorOfDouble(स्थिर std::vector<द्विगुन>& vec)
{
    वापस dumpVector(vec, &floatFormat);
}

String dumpVectorOfRect(स्थिर std::vector<Rect>& vec)
{
    वापस dumpVector(vec, &noFormat);
}


namespace fs {
cv::String getCacheDirectoryForDownloads()
{
#अगर OPENCV_HAVE_FILESYSTEM_SUPPORT
    वापस cv::utils::fs::getCacheDirectory("downloads", "OPENCV_DOWNLOADS_CACHE_DIR");
#अन्यथा
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#पूर्ण_अगर
}
} // namespace fs

}} // namespace
