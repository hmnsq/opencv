// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#அந்தர்பாவ "precomp.hpp"
#அந்தர்பாவ "opencv2/core/bindings_utils.hpp"
#அந்தர்பாவ <sstream>
#அந்தர்பாவ <iomanip>
#அந்தர்பாவ <opencv2/core/utils/filesystem.hpp>
#அந்தர்பாவ <opencv2/core/utils/filesystem.private.hpp>

நாமாகாஶ cv {
நிலையான உள்வரி std::ostream& operator<<(std::ostream& os, மாறிலி Rect& rect)
{
    திரும்பு os << "[x=" << rect.x << ", y=" << rect.y << ", w=" << rect.width << ", h=" << rect.height << ']';
}
நாமாகாஶ utils {

String dump_உள்ளீடு_Array(உள்ளீடு_Array argument)
{
    எனில் (&argument == &noArray())
        திரும்பு "InputArray: noArray()";
    std::ostringstream ss;
    ss << "InputArray:";
    முயல் {
        செய் {
            ss << (argument.empty() ? " empty()=true" : " empty()=false");
            ss << cv::format(" kind=0x%08llx", (long long முழு)argument.kind());
            ss << cv::format(" flags=0x%08llx", (long long முழு)argument.getFlags());
            எனில் (argument.getObj() == வெற்று)
            {
                ss << " obj=NULL";
                நிறுத்து; // done
            }
            ss << cv::format(" total(-1)=%lld", (long long முழு)argument.total(-1));
            முழு dims = argument.dims(-1);
            ss << cv::format(" dims(-1)=%d", dims);
            எனில் (dims <= 2)
            {
                Size size = argument.size(-1);
                ss << cv::format(" size(-1)=%dx%d", size.width, size.height);
            }
            இல்லையெனில்
            {
                முழு sz[CV_MAX_DIM] = {0};
                argument.sizend(sz, -1);
                ss << " size(-1)=[";
                க்கு (முழு i = 0; i < dims; i++)
                {
                    எனில் (i > 0)
                        ss << ' ';
                    ss << sz[i];
                }
                ss << "]";
            }
            ss << " type(-1)=" << cv::typeToString(argument.type(-1));
        } போது (0);
    }
    பிடி (மாறிலி std::exception& e)
    {
        ss << " ERROR: exception occurred: " << e.what();
    }
    பிடி (...)
    {
        ss << " ERROR: unknown exception occurred, dump is non-complete";
    }
    திரும்பு ss.str();
}

CV_EXPORTS_W String dump_உள்ளீடு_Array_Of_Arrays(உள்ளீடு_Array_Of_Arrays argument)
{
    எனில் (&argument == &noArray())
        திரும்பு "InputArrayOfArrays: noArray()";
    std::ostringstream ss;
    ss << "InputArrayOfArrays:";
    முயல் {
        செய் {
            ss << (argument.empty() ? " empty()=true" : " empty()=false");
            ss << cv::format(" kind=0x%08llx", (long long முழு)argument.kind());
            ss << cv::format(" flags=0x%08llx", (long long முழு)argument.getFlags());
            எனில் (argument.getObj() == வெற்று)
            {
                ss << " obj=NULL";
                நிறுத்து; // done
            }
            ss << cv::format(" total(-1)=%lld", (long long முழு)argument.total(-1));
            ss << cv::format(" dims(-1)=%d", argument.dims(-1));
            Size size = argument.size(-1);
            ss << cv::format(" size(-1)=%dx%d", size.width, size.height);
            எனில் (argument.total(-1) > 0)
            {
                ss << " type(0)=" << cv::typeToString(argument.type(0));
                முழு dims = argument.dims(0);
                ss << cv::format(" dims(0)=%d", dims);
                எனில் (dims <= 2)
                {
                    Size size0 = argument.size(0);
                    ss << cv::format(" size(0)=%dx%d", size0.width, size0.height);
                }
                இல்லையெனில்
                {
                    முழு sz[CV_MAX_DIM] = {0};
                    argument.sizend(sz, 0);
                    ss << " size(0)=[";
                    க்கு (முழு i = 0; i < dims; i++)
                    {
                        எனில் (i > 0)
                            ss << ' ';
                        ss << sz[i];
                    }
                    ss << "]";
                }
            }
        } போது (0);
    }
    பிடி (மாறிலி std::exception& e)
    {
        ss << " ERROR: exception occurred: " << e.what();
    }
    பிடி (...)
    {
        ss << " ERROR: unknown exception occurred, dump is non-complete";
    }
    திரும்பு ss.str();
}

CV_EXPORTS_W String dump_உள்ளீடு_Output_Array(உள்ளீடு_Output_Array argument)
{
    எனில் (&argument == &noArray())
        திரும்பு "InputOutputArray: noArray()";
    std::ostringstream ss;
    ss << "InputOutputArray:";
    முயல் {
        செய் {
            ss << (argument.empty() ? " empty()=true" : " empty()=false");
            ss << cv::format(" kind=0x%08llx", (long long முழு)argument.kind());
            ss << cv::format(" flags=0x%08llx", (long long முழு)argument.getFlags());
            எனில் (argument.getObj() == வெற்று)
            {
                ss << " obj=NULL";
                நிறுத்து; // done
            }
            ss << cv::format(" total(-1)=%lld", (long long முழு)argument.total(-1));
            முழு dims = argument.dims(-1);
            ss << cv::format(" dims(-1)=%d", dims);
            எனில் (dims <= 2)
            {
                Size size = argument.size(-1);
                ss << cv::format(" size(-1)=%dx%d", size.width, size.height);
            }
            இல்லையெனில்
            {
                முழு sz[CV_MAX_DIM] = {0};
                argument.sizend(sz, -1);
                ss << " size(-1)=[";
                க்கு (முழு i = 0; i < dims; i++)
                {
                    எனில் (i > 0)
                        ss << ' ';
                    ss << sz[i];
                }
                ss << "]";
            }
            ss << " type(-1)=" << cv::typeToString(argument.type(-1));
        } போது (0);
    }
    பிடி (மாறிலி std::exception& e)
    {
        ss << " ERROR: exception occurred: " << e.what();
    }
    பிடி (...)
    {
        ss << " ERROR: unknown exception occurred, dump is non-complete";
    }
    திரும்பு ss.str();
}

CV_EXPORTS_W String dump_உள்ளீடு_Output_Array_Of_Arrays(உள்ளீடு_Output_Array_Of_Arrays argument)
{
    எனில் (&argument == &noArray())
        திரும்பு "InputOutputArrayOfArrays: noArray()";
    std::ostringstream ss;
    ss << "InputOutputArrayOfArrays:";
    முயல் {
        செய் {
            ss << (argument.empty() ? " empty()=true" : " empty()=false");
            ss << cv::format(" kind=0x%08llx", (long long முழு)argument.kind());
            ss << cv::format(" flags=0x%08llx", (long long முழு)argument.getFlags());
            எனில் (argument.getObj() == வெற்று)
            {
                ss << " obj=NULL";
                நிறுத்து; // done
            }
            ss << cv::format(" total(-1)=%lld", (long long முழு)argument.total(-1));
            ss << cv::format(" dims(-1)=%d", argument.dims(-1));
            Size size = argument.size(-1);
            ss << cv::format(" size(-1)=%dx%d", size.width, size.height);
            எனில் (argument.total(-1) > 0)
            {
                ss << " type(0)=" << cv::typeToString(argument.type(0));
                முழு dims = argument.dims(0);
                ss << cv::format(" dims(0)=%d", dims);
                எனில் (dims <= 2)
                {
                    Size size0 = argument.size(0);
                    ss << cv::format(" size(0)=%dx%d", size0.width, size0.height);
                }
                இல்லையெனில்
                {
                    முழு sz[CV_MAX_DIM] = {0};
                    argument.sizend(sz, 0);
                    ss << " size(0)=[";
                    க்கு (முழு i = 0; i < dims; i++)
                    {
                        எனில் (i > 0)
                            ss << ' ';
                        ss << sz[i];
                    }
                    ss << "]";
                }
            }
        } போது (0);
    }
    பிடி (மாறிலி std::exception& e)
    {
        ss << " ERROR: exception occurred: " << e.what();
    }
    பிடி (...)
    {
        ss << " ERROR: unknown exception occurred, dump is non-complete";
    }
    திரும்பு ss.str();
}

template <வகுப்பு T, வகுப்பு Formatter>
நிலையான உள்வரி String dumpVector(மாறிலி std::vector<T>& vec, Formatter format)
{
    std::ostringstream oss("[", std::ios::ate);
    எனில் (!vec.empty())
    {
        format(oss) << vec[0];
        க்கு (std::size_t i = 1; i < vec.size(); ++i)
        {
            oss << ", ";
            format(oss) << vec[i];
        }
    }
    oss << "]";
    திரும்பு oss.str();
}

நிலையான உள்வரி std::ostream& noFormat(std::ostream& os)
{
    திரும்பு os;
}

நிலையான உள்வரி std::ostream& floatFormat(std::ostream& os)
{
    திரும்பு os << std::fixed << std::setprecision(2);
}

String dumpVectorOfInt(மாறிலி std::vector<முழு>& vec)
{
    திரும்பு dumpVector(vec, &noFormat);
}

String dumpVectorOfDouble(மாறிலி std::vector<இரட்டை>& vec)
{
    திரும்பு dumpVector(vec, &floatFormat);
}

String dumpVectorOfRect(மாறிலி std::vector<Rect>& vec)
{
    திரும்பு dumpVector(vec, &noFormat);
}


நாமாகாஶ fs {
cv::String getCacheDirectoryForDownloads()
{
#எனில் OPENCV_HAVE_FILESYSTEM_SUPPORT
    திரும்பு cv::utils::fs::getCacheDirectory("downloads", "OPENCV_DOWNLOADS_CACHE_DIR");
#இல்லையெனில்
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#endif
}
} // namespace fs

}} // namespace
