// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#અન્તર્ભાવ "precomp.hpp"
#અન્તર્ભાવ "opencv2/core/bindings_utils.hpp"
#અન્તર્ભાવ <sstream>
#અન્તર્ભાવ <iomanip>
#અન્તર્ભાવ <opencv2/core/utils/filesystem.hpp>
#અન્તર્ભાવ <opencv2/core/utils/filesystem.private.hpp>

નામાકાશ cv {
સ્થિર અંતઃરેખ std::ostream& operator<<(std::ostream& os, અચલ Rect& rect)
{
    પાછા os << "[x=" << rect.x << ", y=" << rect.y << ", w=" << rect.width << ", h=" << rect.height << ']';
}
નામાકાશ utils {

String dump_આગત_Array(આગત_Array argument)
{
    જો (&argument == &noArray())
        પાછા "InputArray: noArray()";
    std::ostringstream ss;
    ss << "InputArray:";
    પ્રયાસ {
        કરો {
            ss << (argument.empty() ? " empty()=true" : " empty()=false");
            ss << cv::format(" kind=0x%08llx", (long long પૂર્ણાંક)argument.kind());
            ss << cv::format(" flags=0x%08llx", (long long પૂર્ણાંક)argument.getFlags());
            જો (argument.getObj() == ખાલી)
            {
                ss << " obj=NULL";
                તોડો; // done
            }
            ss << cv::format(" total(-1)=%lld", (long long પૂર્ણાંક)argument.total(-1));
            પૂર્ણાંક dims = argument.dims(-1);
            ss << cv::format(" dims(-1)=%d", dims);
            જો (dims <= 2)
            {
                Size size = argument.size(-1);
                ss << cv::format(" size(-1)=%dx%d", size.width, size.height);
            }
            નહીંતર
            {
                પૂર્ણાંક sz[CV_MAX_DIM] = {0};
                argument.sizend(sz, -1);
                ss << " size(-1)=[";
                માટે (પૂર્ણાંક i = 0; i < dims; i++)
                {
                    જો (i > 0)
                        ss << ' ';
                    ss << sz[i];
                }
                ss << "]";
            }
            ss << " type(-1)=" << cv::typeToString(argument.type(-1));
        } જ્યારે (0);
    }
    પકડો (અચલ std::exception& e)
    {
        ss << " ERROR: exception occurred: " << e.what();
    }
    પકડો (...)
    {
        ss << " ERROR: unknown exception occurred, dump is non-complete";
    }
    પાછા ss.str();
}

CV_EXPORTS_W String dump_આગત_Array_Of_Arrays(આગત_Array_Of_Arrays argument)
{
    જો (&argument == &noArray())
        પાછા "InputArrayOfArrays: noArray()";
    std::ostringstream ss;
    ss << "InputArrayOfArrays:";
    પ્રયાસ {
        કરો {
            ss << (argument.empty() ? " empty()=true" : " empty()=false");
            ss << cv::format(" kind=0x%08llx", (long long પૂર્ણાંક)argument.kind());
            ss << cv::format(" flags=0x%08llx", (long long પૂર્ણાંક)argument.getFlags());
            જો (argument.getObj() == ખાલી)
            {
                ss << " obj=NULL";
                તોડો; // done
            }
            ss << cv::format(" total(-1)=%lld", (long long પૂર્ણાંક)argument.total(-1));
            ss << cv::format(" dims(-1)=%d", argument.dims(-1));
            Size size = argument.size(-1);
            ss << cv::format(" size(-1)=%dx%d", size.width, size.height);
            જો (argument.total(-1) > 0)
            {
                ss << " type(0)=" << cv::typeToString(argument.type(0));
                પૂર્ણાંક dims = argument.dims(0);
                ss << cv::format(" dims(0)=%d", dims);
                જો (dims <= 2)
                {
                    Size size0 = argument.size(0);
                    ss << cv::format(" size(0)=%dx%d", size0.width, size0.height);
                }
                નહીંતર
                {
                    પૂર્ણાંક sz[CV_MAX_DIM] = {0};
                    argument.sizend(sz, 0);
                    ss << " size(0)=[";
                    માટે (પૂર્ણાંક i = 0; i < dims; i++)
                    {
                        જો (i > 0)
                            ss << ' ';
                        ss << sz[i];
                    }
                    ss << "]";
                }
            }
        } જ્યારે (0);
    }
    પકડો (અચલ std::exception& e)
    {
        ss << " ERROR: exception occurred: " << e.what();
    }
    પકડો (...)
    {
        ss << " ERROR: unknown exception occurred, dump is non-complete";
    }
    પાછા ss.str();
}

CV_EXPORTS_W String dump_આગત_Output_Array(આગત_Output_Array argument)
{
    જો (&argument == &noArray())
        પાછા "InputOutputArray: noArray()";
    std::ostringstream ss;
    ss << "InputOutputArray:";
    પ્રયાસ {
        કરો {
            ss << (argument.empty() ? " empty()=true" : " empty()=false");
            ss << cv::format(" kind=0x%08llx", (long long પૂર્ણાંક)argument.kind());
            ss << cv::format(" flags=0x%08llx", (long long પૂર્ણાંક)argument.getFlags());
            જો (argument.getObj() == ખાલી)
            {
                ss << " obj=NULL";
                તોડો; // done
            }
            ss << cv::format(" total(-1)=%lld", (long long પૂર્ણાંક)argument.total(-1));
            પૂર્ણાંક dims = argument.dims(-1);
            ss << cv::format(" dims(-1)=%d", dims);
            જો (dims <= 2)
            {
                Size size = argument.size(-1);
                ss << cv::format(" size(-1)=%dx%d", size.width, size.height);
            }
            નહીંતર
            {
                પૂર્ણાંક sz[CV_MAX_DIM] = {0};
                argument.sizend(sz, -1);
                ss << " size(-1)=[";
                માટે (પૂર્ણાંક i = 0; i < dims; i++)
                {
                    જો (i > 0)
                        ss << ' ';
                    ss << sz[i];
                }
                ss << "]";
            }
            ss << " type(-1)=" << cv::typeToString(argument.type(-1));
        } જ્યારે (0);
    }
    પકડો (અચલ std::exception& e)
    {
        ss << " ERROR: exception occurred: " << e.what();
    }
    પકડો (...)
    {
        ss << " ERROR: unknown exception occurred, dump is non-complete";
    }
    પાછા ss.str();
}

CV_EXPORTS_W String dump_આગત_Output_Array_Of_Arrays(આગત_Output_Array_Of_Arrays argument)
{
    જો (&argument == &noArray())
        પાછા "InputOutputArrayOfArrays: noArray()";
    std::ostringstream ss;
    ss << "InputOutputArrayOfArrays:";
    પ્રયાસ {
        કરો {
            ss << (argument.empty() ? " empty()=true" : " empty()=false");
            ss << cv::format(" kind=0x%08llx", (long long પૂર્ણાંક)argument.kind());
            ss << cv::format(" flags=0x%08llx", (long long પૂર્ણાંક)argument.getFlags());
            જો (argument.getObj() == ખાલી)
            {
                ss << " obj=NULL";
                તોડો; // done
            }
            ss << cv::format(" total(-1)=%lld", (long long પૂર્ણાંક)argument.total(-1));
            ss << cv::format(" dims(-1)=%d", argument.dims(-1));
            Size size = argument.size(-1);
            ss << cv::format(" size(-1)=%dx%d", size.width, size.height);
            જો (argument.total(-1) > 0)
            {
                ss << " type(0)=" << cv::typeToString(argument.type(0));
                પૂર્ણાંક dims = argument.dims(0);
                ss << cv::format(" dims(0)=%d", dims);
                જો (dims <= 2)
                {
                    Size size0 = argument.size(0);
                    ss << cv::format(" size(0)=%dx%d", size0.width, size0.height);
                }
                નહીંતર
                {
                    પૂર્ણાંક sz[CV_MAX_DIM] = {0};
                    argument.sizend(sz, 0);
                    ss << " size(0)=[";
                    માટે (પૂર્ણાંક i = 0; i < dims; i++)
                    {
                        જો (i > 0)
                            ss << ' ';
                        ss << sz[i];
                    }
                    ss << "]";
                }
            }
        } જ્યારે (0);
    }
    પકડો (અચલ std::exception& e)
    {
        ss << " ERROR: exception occurred: " << e.what();
    }
    પકડો (...)
    {
        ss << " ERROR: unknown exception occurred, dump is non-complete";
    }
    પાછા ss.str();
}

template <વર્ગ T, વર્ગ Formatter>
સ્થિર અંતઃરેખ String dumpVector(અચલ std::vector<T>& vec, Formatter format)
{
    std::ostringstream oss("[", std::ios::ate);
    જો (!vec.empty())
    {
        format(oss) << vec[0];
        માટે (std::size_t i = 1; i < vec.size(); ++i)
        {
            oss << ", ";
            format(oss) << vec[i];
        }
    }
    oss << "]";
    પાછા oss.str();
}

સ્થિર અંતઃરેખ std::ostream& noFormat(std::ostream& os)
{
    પાછા os;
}

સ્થિર અંતઃરેખ std::ostream& floatFormat(std::ostream& os)
{
    પાછા os << std::fixed << std::setprecision(2);
}

String dumpVectorOfInt(અચલ std::vector<પૂર્ણાંક>& vec)
{
    પાછા dumpVector(vec, &noFormat);
}

String dumpVectorOfDouble(અચલ std::vector<દ્વિગુણ>& vec)
{
    પાછા dumpVector(vec, &floatFormat);
}

String dumpVectorOfRect(અચલ std::vector<Rect>& vec)
{
    પાછા dumpVector(vec, &noFormat);
}


નામાકાશ fs {
cv::String getCacheDirectoryForDownloads()
{
#જો OPENCV_HAVE_FILESYSTEM_SUPPORT
    પાછા cv::utils::fs::getCacheDirectory("downloads", "OPENCV_DOWNLOADS_CACHE_DIR");
#નહીંતર
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#endif
}
} // namespace fs

}} // namespace
