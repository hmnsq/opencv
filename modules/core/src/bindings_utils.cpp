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
स्थिर अन्तःस्थ std::ostream& operator<<(std::ostream& os, ध्रुव Rect& rect)
{
    प्रत्यागम os << "[x=" << rect.x << ", y=" << rect.y << ", w=" << rect.width << ", h=" << rect.height << ']';
}
नामाकाश utils {

String dump_निवेशः_Array(निवेशः_Array argument)
{
    जइ (&argument == &noArray())
        प्रत्यागम "InputArray: noArray()";
    std::ostringstream ss;
    ss << "InputArray:";
    यत्न {
        करेहि {
            ss << (argument.empty() ? " empty()=true" : " empty()=false");
            ss << cv::format(" kind=0x%08llx", (long long पूर्णाङ्क)argument.kind());
            ss << cv::format(" flags=0x%08llx", (long long पूर्णाङ्क)argument.getFlags());
            जइ (argument.getObj() == रिक्त)
            {
                ss << " obj=NULL";
                भङ्ग; // done
            }
            ss << cv::format(" total(-1)=%lld", (long long पूर्णाङ्क)argument.total(-1));
            पूर्णाङ्क dims = argument.dims(-1);
            ss << cv::format(" dims(-1)=%d", dims);
            जइ (dims <= 2)
            {
                Size size = argument.size(-1);
                ss << cv::format(" size(-1)=%dx%d", size.width, size.height);
            }
            अण्णहा
            {
                पूर्णाङ्क sz[CV_MAX_DIM] = {0};
                argument.sizend(sz, -1);
                ss << " size(-1)=[";
                प्रति (पूर्णाङ्क i = 0; i < dims; i++)
                {
                    जइ (i > 0)
                        ss << ' ';
                    ss << sz[i];
                }
                ss << "]";
            }
            ss << " type(-1)=" << cv::typeToString(argument.type(-1));
        } जाव (0);
    }
    गृहाण (ध्रुव std::exception& e)
    {
        ss << " ERROR: exception occurred: " << e.what();
    }
    गृहाण (...)
    {
        ss << " ERROR: unknown exception occurred, dump is non-complete";
    }
    प्रत्यागम ss.str();
}

CV_EXPORTS_W String dump_निवेशः_Array_Of_Arrays(निवेशः_Array_Of_Arrays argument)
{
    जइ (&argument == &noArray())
        प्रत्यागम "InputArrayOfArrays: noArray()";
    std::ostringstream ss;
    ss << "InputArrayOfArrays:";
    यत्न {
        करेहि {
            ss << (argument.empty() ? " empty()=true" : " empty()=false");
            ss << cv::format(" kind=0x%08llx", (long long पूर्णाङ्क)argument.kind());
            ss << cv::format(" flags=0x%08llx", (long long पूर्णाङ्क)argument.getFlags());
            जइ (argument.getObj() == रिक्त)
            {
                ss << " obj=NULL";
                भङ्ग; // done
            }
            ss << cv::format(" total(-1)=%lld", (long long पूर्णाङ्क)argument.total(-1));
            ss << cv::format(" dims(-1)=%d", argument.dims(-1));
            Size size = argument.size(-1);
            ss << cv::format(" size(-1)=%dx%d", size.width, size.height);
            जइ (argument.total(-1) > 0)
            {
                ss << " type(0)=" << cv::typeToString(argument.type(0));
                पूर्णाङ्क dims = argument.dims(0);
                ss << cv::format(" dims(0)=%d", dims);
                जइ (dims <= 2)
                {
                    Size size0 = argument.size(0);
                    ss << cv::format(" size(0)=%dx%d", size0.width, size0.height);
                }
                अण्णहा
                {
                    पूर्णाङ्क sz[CV_MAX_DIM] = {0};
                    argument.sizend(sz, 0);
                    ss << " size(0)=[";
                    प्रति (पूर्णाङ्क i = 0; i < dims; i++)
                    {
                        जइ (i > 0)
                            ss << ' ';
                        ss << sz[i];
                    }
                    ss << "]";
                }
            }
        } जाव (0);
    }
    गृहाण (ध्रुव std::exception& e)
    {
        ss << " ERROR: exception occurred: " << e.what();
    }
    गृहाण (...)
    {
        ss << " ERROR: unknown exception occurred, dump is non-complete";
    }
    प्रत्यागम ss.str();
}

CV_EXPORTS_W String dump_निवेशः_Output_Array(निवेशः_Output_Array argument)
{
    जइ (&argument == &noArray())
        प्रत्यागम "InputOutputArray: noArray()";
    std::ostringstream ss;
    ss << "InputOutputArray:";
    यत्न {
        करेहि {
            ss << (argument.empty() ? " empty()=true" : " empty()=false");
            ss << cv::format(" kind=0x%08llx", (long long पूर्णाङ्क)argument.kind());
            ss << cv::format(" flags=0x%08llx", (long long पूर्णाङ्क)argument.getFlags());
            जइ (argument.getObj() == रिक्त)
            {
                ss << " obj=NULL";
                भङ्ग; // done
            }
            ss << cv::format(" total(-1)=%lld", (long long पूर्णाङ्क)argument.total(-1));
            पूर्णाङ्क dims = argument.dims(-1);
            ss << cv::format(" dims(-1)=%d", dims);
            जइ (dims <= 2)
            {
                Size size = argument.size(-1);
                ss << cv::format(" size(-1)=%dx%d", size.width, size.height);
            }
            अण्णहा
            {
                पूर्णाङ्क sz[CV_MAX_DIM] = {0};
                argument.sizend(sz, -1);
                ss << " size(-1)=[";
                प्रति (पूर्णाङ्क i = 0; i < dims; i++)
                {
                    जइ (i > 0)
                        ss << ' ';
                    ss << sz[i];
                }
                ss << "]";
            }
            ss << " type(-1)=" << cv::typeToString(argument.type(-1));
        } जाव (0);
    }
    गृहाण (ध्रुव std::exception& e)
    {
        ss << " ERROR: exception occurred: " << e.what();
    }
    गृहाण (...)
    {
        ss << " ERROR: unknown exception occurred, dump is non-complete";
    }
    प्रत्यागम ss.str();
}

CV_EXPORTS_W String dump_निवेशः_Output_Array_Of_Arrays(निवेशः_Output_Array_Of_Arrays argument)
{
    जइ (&argument == &noArray())
        प्रत्यागम "InputOutputArrayOfArrays: noArray()";
    std::ostringstream ss;
    ss << "InputOutputArrayOfArrays:";
    यत्न {
        करेहि {
            ss << (argument.empty() ? " empty()=true" : " empty()=false");
            ss << cv::format(" kind=0x%08llx", (long long पूर्णाङ्क)argument.kind());
            ss << cv::format(" flags=0x%08llx", (long long पूर्णाङ्क)argument.getFlags());
            जइ (argument.getObj() == रिक्त)
            {
                ss << " obj=NULL";
                भङ्ग; // done
            }
            ss << cv::format(" total(-1)=%lld", (long long पूर्णाङ्क)argument.total(-1));
            ss << cv::format(" dims(-1)=%d", argument.dims(-1));
            Size size = argument.size(-1);
            ss << cv::format(" size(-1)=%dx%d", size.width, size.height);
            जइ (argument.total(-1) > 0)
            {
                ss << " type(0)=" << cv::typeToString(argument.type(0));
                पूर्णाङ्क dims = argument.dims(0);
                ss << cv::format(" dims(0)=%d", dims);
                जइ (dims <= 2)
                {
                    Size size0 = argument.size(0);
                    ss << cv::format(" size(0)=%dx%d", size0.width, size0.height);
                }
                अण्णहा
                {
                    पूर्णाङ्क sz[CV_MAX_DIM] = {0};
                    argument.sizend(sz, 0);
                    ss << " size(0)=[";
                    प्रति (पूर्णाङ्क i = 0; i < dims; i++)
                    {
                        जइ (i > 0)
                            ss << ' ';
                        ss << sz[i];
                    }
                    ss << "]";
                }
            }
        } जाव (0);
    }
    गृहाण (ध्रुव std::exception& e)
    {
        ss << " ERROR: exception occurred: " << e.what();
    }
    गृहाण (...)
    {
        ss << " ERROR: unknown exception occurred, dump is non-complete";
    }
    प्रत्यागम ss.str();
}

template <वर्ग T, वर्ग Formatter>
स्थिर अन्तःस्थ String dumpVector(ध्रुव std::vector<T>& vec, Formatter format)
{
    std::ostringstream oss("[", std::ios::ate);
    जइ (!vec.empty())
    {
        format(oss) << vec[0];
        प्रति (std::size_t i = 1; i < vec.size(); ++i)
        {
            oss << ", ";
            format(oss) << vec[i];
        }
    }
    oss << "]";
    प्रत्यागम oss.str();
}

स्थिर अन्तःस्थ std::ostream& noFormat(std::ostream& os)
{
    प्रत्यागम os;
}

स्थिर अन्तःस्थ std::ostream& floatFormat(std::ostream& os)
{
    प्रत्यागम os << std::fixed << std::setprecision(2);
}

String dumpVectorOfInt(ध्रुव std::vector<पूर्णाङ्क>& vec)
{
    प्रत्यागम dumpVector(vec, &noFormat);
}

String dumpVectorOfDouble(ध्रुव std::vector<द्विगुण>& vec)
{
    प्रत्यागम dumpVector(vec, &floatFormat);
}

String dumpVectorOfRect(ध्रुव std::vector<Rect>& vec)
{
    प्रत्यागम dumpVector(vec, &noFormat);
}


नामाकाश fs {
cv::String getCacheDirectoryForDownloads()
{
#जइ OPENCV_HAVE_FILESYSTEM_SUPPORT
    प्रत्यागम cv::utils::fs::getCacheDirectory("downloads", "OPENCV_DOWNLOADS_CACHE_DIR");
#अण्णहा
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#endif
}
} // namespace fs

}} // namespace
