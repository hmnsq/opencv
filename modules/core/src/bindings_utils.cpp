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
स्थिर अन्तःस्थ std::ostream& चालक<<(std::ostream& os, अचर Rect& rect)
{
    फर्काऊ os << "[x=" << rect.x << ", y=" << rect.y << ", w=" << rect.width << ", h=" << rect.height << ']';
}
नामाकाश utils {

String dump_आगत_Array(आगत_Array argument)
{
    यदि (&argument == &noArray())
        फर्काऊ "InputArray: noArray()";
    std::ostringstream ss;
    ss << "InputArray:";
    यत्न {
        गर {
            ss << (argument.empty() ? " empty()=true" : " empty()=false");
            ss << cv::format(" kind=0x%08llx", (दीर्घ दीर्घ पूर्णाङ्क)argument.kind());
            ss << cv::format(" flags=0x%08llx", (दीर्घ दीर्घ पूर्णाङ्क)argument.getFlags());
            यदि (argument.getObj() == रिक्त)
            {
                ss << " obj=NULL";
                रोक; // done
            }
            ss << cv::format(" total(-1)=%lld", (दीर्घ दीर्घ पूर्णाङ्क)argument.total(-1));
            पूर्णाङ्क dims = argument.dims(-1);
            ss << cv::format(" dims(-1)=%d", dims);
            यदि (dims <= 2)
            {
                Size size = argument.size(-1);
                ss << cv::format(" size(-1)=%dx%d", size.width, size.height);
            }
            अन्यथा
            {
                पूर्णाङ्क sz[CV_MAX_DIM] = {0};
                argument.sizend(sz, -1);
                ss << " size(-1)=[";
                लागि (पूर्णाङ्क i = 0; i < dims; i++)
                {
                    यदि (i > 0)
                        ss << ' ';
                    ss << sz[i];
                }
                ss << "]";
            }
            ss << " type(-1)=" << cv::typeToString(argument.type(-1));
        } जबसम्म (0);
    }
    गृहाण (अचर std::exception& e)
    {
        ss << " ERROR: exception occurred: " << e.what();
    }
    गृहाण (...)
    {
        ss << " ERROR: unknown exception occurred, dump is non-complete";
    }
    फर्काऊ ss.str();
}

CV_EXPORTS_W String dump_आगत_Array_Of_Arrays(आगत_Array_Of_Arrays argument)
{
    यदि (&argument == &noArray())
        फर्काऊ "InputArrayOfArrays: noArray()";
    std::ostringstream ss;
    ss << "InputArrayOfArrays:";
    यत्न {
        गर {
            ss << (argument.empty() ? " empty()=true" : " empty()=false");
            ss << cv::format(" kind=0x%08llx", (दीर्घ दीर्घ पूर्णाङ्क)argument.kind());
            ss << cv::format(" flags=0x%08llx", (दीर्घ दीर्घ पूर्णाङ्क)argument.getFlags());
            यदि (argument.getObj() == रिक्त)
            {
                ss << " obj=NULL";
                रोक; // done
            }
            ss << cv::format(" total(-1)=%lld", (दीर्घ दीर्घ पूर्णाङ्क)argument.total(-1));
            ss << cv::format(" dims(-1)=%d", argument.dims(-1));
            Size size = argument.size(-1);
            ss << cv::format(" size(-1)=%dx%d", size.width, size.height);
            यदि (argument.total(-1) > 0)
            {
                ss << " type(0)=" << cv::typeToString(argument.type(0));
                पूर्णाङ्क dims = argument.dims(0);
                ss << cv::format(" dims(0)=%d", dims);
                यदि (dims <= 2)
                {
                    Size size0 = argument.size(0);
                    ss << cv::format(" size(0)=%dx%d", size0.width, size0.height);
                }
                अन्यथा
                {
                    पूर्णाङ्क sz[CV_MAX_DIM] = {0};
                    argument.sizend(sz, 0);
                    ss << " size(0)=[";
                    लागि (पूर्णाङ्क i = 0; i < dims; i++)
                    {
                        यदि (i > 0)
                            ss << ' ';
                        ss << sz[i];
                    }
                    ss << "]";
                }
            }
        } जबसम्म (0);
    }
    गृहाण (अचर std::exception& e)
    {
        ss << " ERROR: exception occurred: " << e.what();
    }
    गृहाण (...)
    {
        ss << " ERROR: unknown exception occurred, dump is non-complete";
    }
    फर्काऊ ss.str();
}

CV_EXPORTS_W String dump_आगत_Output_Array(आगत_Output_Array argument)
{
    यदि (&argument == &noArray())
        फर्काऊ "InputOutputArray: noArray()";
    std::ostringstream ss;
    ss << "InputOutputArray:";
    यत्न {
        गर {
            ss << (argument.empty() ? " empty()=true" : " empty()=false");
            ss << cv::format(" kind=0x%08llx", (दीर्घ दीर्घ पूर्णाङ्क)argument.kind());
            ss << cv::format(" flags=0x%08llx", (दीर्घ दीर्घ पूर्णाङ्क)argument.getFlags());
            यदि (argument.getObj() == रिक्त)
            {
                ss << " obj=NULL";
                रोक; // done
            }
            ss << cv::format(" total(-1)=%lld", (दीर्घ दीर्घ पूर्णाङ्क)argument.total(-1));
            पूर्णाङ्क dims = argument.dims(-1);
            ss << cv::format(" dims(-1)=%d", dims);
            यदि (dims <= 2)
            {
                Size size = argument.size(-1);
                ss << cv::format(" size(-1)=%dx%d", size.width, size.height);
            }
            अन्यथा
            {
                पूर्णाङ्क sz[CV_MAX_DIM] = {0};
                argument.sizend(sz, -1);
                ss << " size(-1)=[";
                लागि (पूर्णाङ्क i = 0; i < dims; i++)
                {
                    यदि (i > 0)
                        ss << ' ';
                    ss << sz[i];
                }
                ss << "]";
            }
            ss << " type(-1)=" << cv::typeToString(argument.type(-1));
        } जबसम्म (0);
    }
    गृहाण (अचर std::exception& e)
    {
        ss << " ERROR: exception occurred: " << e.what();
    }
    गृहाण (...)
    {
        ss << " ERROR: unknown exception occurred, dump is non-complete";
    }
    फर्काऊ ss.str();
}

CV_EXPORTS_W String dump_आगत_Output_Array_Of_Arrays(आगत_Output_Array_Of_Arrays argument)
{
    यदि (&argument == &noArray())
        फर्काऊ "InputOutputArrayOfArrays: noArray()";
    std::ostringstream ss;
    ss << "InputOutputArrayOfArrays:";
    यत्न {
        गर {
            ss << (argument.empty() ? " empty()=true" : " empty()=false");
            ss << cv::format(" kind=0x%08llx", (दीर्घ दीर्घ पूर्णाङ्क)argument.kind());
            ss << cv::format(" flags=0x%08llx", (दीर्घ दीर्घ पूर्णाङ्क)argument.getFlags());
            यदि (argument.getObj() == रिक्त)
            {
                ss << " obj=NULL";
                रोक; // done
            }
            ss << cv::format(" total(-1)=%lld", (दीर्घ दीर्घ पूर्णाङ्क)argument.total(-1));
            ss << cv::format(" dims(-1)=%d", argument.dims(-1));
            Size size = argument.size(-1);
            ss << cv::format(" size(-1)=%dx%d", size.width, size.height);
            यदि (argument.total(-1) > 0)
            {
                ss << " type(0)=" << cv::typeToString(argument.type(0));
                पूर्णाङ्क dims = argument.dims(0);
                ss << cv::format(" dims(0)=%d", dims);
                यदि (dims <= 2)
                {
                    Size size0 = argument.size(0);
                    ss << cv::format(" size(0)=%dx%d", size0.width, size0.height);
                }
                अन्यथा
                {
                    पूर्णाङ्क sz[CV_MAX_DIM] = {0};
                    argument.sizend(sz, 0);
                    ss << " size(0)=[";
                    लागि (पूर्णाङ्क i = 0; i < dims; i++)
                    {
                        यदि (i > 0)
                            ss << ' ';
                        ss << sz[i];
                    }
                    ss << "]";
                }
            }
        } जबसम्म (0);
    }
    गृहाण (अचर std::exception& e)
    {
        ss << " ERROR: exception occurred: " << e.what();
    }
    गृहाण (...)
    {
        ss << " ERROR: unknown exception occurred, dump is non-complete";
    }
    फर्काऊ ss.str();
}

ढाँचा <वर्ग T, वर्ग Formatter>
स्थिर अन्तःस्थ String dumpVector(अचर std::vector<T>& vec, Formatter format)
{
    std::ostringstream oss("[", std::ios::ate);
    यदि (!vec.empty())
    {
        format(oss) << vec[0];
        लागि (std::माप_प्रकार i = 1; i < vec.size(); ++i)
        {
            oss << ", ";
            format(oss) << vec[i];
        }
    }
    oss << "]";
    फर्काऊ oss.str();
}

स्थिर अन्तःस्थ std::ostream& noFormat(std::ostream& os)
{
    फर्काऊ os;
}

स्थिर अन्तःस्थ std::ostream& floatFormat(std::ostream& os)
{
    फर्काऊ os << std::fixed << std::setprecision(2);
}

String dumpVectorOfInt(अचर std::vector<पूर्णाङ्क>& vec)
{
    फर्काऊ dumpVector(vec, &noFormat);
}

String dumpVectorOfDouble(अचर std::vector<द्विगुण>& vec)
{
    फर्काऊ dumpVector(vec, &floatFormat);
}

String dumpVectorOfRect(अचर std::vector<Rect>& vec)
{
    फर्काऊ dumpVector(vec, &noFormat);
}


नामाकाश fs {
cv::String getCacheDirectoryForDownloads()
{
#यदि OPENCV_HAVE_FILESYSTEM_SUPPORT
    फर्काऊ cv::utils::fs::getCacheDirectory("downloads", "OPENCV_DOWNLOADS_CACHE_DIR");
#अन्यथा
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#endif
}
} // namespace fs

}} // namespace
