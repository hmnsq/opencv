// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ଅନ୍ତର୍ଭାଵ "precomp.hpp"
#ଅନ୍ତର୍ଭାଵ "opencv2/core/bindings_utils.hpp"
#ଅନ୍ତର୍ଭାଵ <sstream>
#ଅନ୍ତର୍ଭାଵ <iomanip>
#ଅନ୍ତର୍ଭାଵ <opencv2/core/utils/filesystem.hpp>
#ଅନ୍ତର୍ଭାଵ <opencv2/core/utils/filesystem.private.hpp>

ନାମାକାଶ cv {
ସ୍ଥିର ଧାଡ଼ିଭିତରେ std::ostream& operator<<(std::ostream& os, ଅଚଳ Rect& rect)
{
    ଫେରନ୍ତୁ os << "[x=" << rect.x << ", y=" << rect.y << ", w=" << rect.width << ", h=" << rect.height << ']';
}
ନାମାକାଶ utils {

String dump_ନିବେଶ_Array(ନିବେଶ_Array argument)
{
    ଯଦି (&argument == &noArray())
        ଫେରନ୍ତୁ "InputArray: noArray()";
    std::ostringstream ss;
    ss << "InputArray:";
    ଚେଷ୍ଟା {
        କର {
            ss << (argument.empty() ? " empty()=true" : " empty()=false");
            ss << cv::format(" kind=0x%08llx", (long long ପୂର୍ଣ୍ଣସଂଖ୍ୟା)argument.kind());
            ss << cv::format(" flags=0x%08llx", (long long ପୂର୍ଣ୍ଣସଂଖ୍ୟା)argument.getFlags());
            ଯଦି (argument.getObj() == ଖାଲି)
            {
                ss << " obj=NULL";
                ଭାଙ୍ଗ; // done
            }
            ss << cv::format(" total(-1)=%lld", (long long ପୂର୍ଣ୍ଣସଂଖ୍ୟା)argument.total(-1));
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା dims = argument.dims(-1);
            ss << cv::format(" dims(-1)=%d", dims);
            ଯଦି (dims <= 2)
            {
                Size size = argument.size(-1);
                ss << cv::format(" size(-1)=%dx%d", size.width, size.height);
            }
            ଅନ୍ୟଥା
            {
                ପୂର୍ଣ୍ଣସଂଖ୍ୟା sz[CV_MAX_DIM] = {0};
                argument.sizend(sz, -1);
                ss << " size(-1)=[";
                ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < dims; i++)
                {
                    ଯଦି (i > 0)
                        ss << ' ';
                    ss << sz[i];
                }
                ss << "]";
            }
            ss << " type(-1)=" << cv::typeToString(argument.type(-1));
        } ଯେତେବେଳେ (0);
    }
    ଧର (ଅଚଳ std::exception& e)
    {
        ss << " ERROR: exception occurred: " << e.what();
    }
    ଧର (...)
    {
        ss << " ERROR: unknown exception occurred, dump is non-complete";
    }
    ଫେରନ୍ତୁ ss.str();
}

CV_EXPORTS_W String dump_ନିବେଶ_Array_Of_Arrays(ନିବେଶ_Array_Of_Arrays argument)
{
    ଯଦି (&argument == &noArray())
        ଫେରନ୍ତୁ "InputArrayOfArrays: noArray()";
    std::ostringstream ss;
    ss << "InputArrayOfArrays:";
    ଚେଷ୍ଟା {
        କର {
            ss << (argument.empty() ? " empty()=true" : " empty()=false");
            ss << cv::format(" kind=0x%08llx", (long long ପୂର୍ଣ୍ଣସଂଖ୍ୟା)argument.kind());
            ss << cv::format(" flags=0x%08llx", (long long ପୂର୍ଣ୍ଣସଂଖ୍ୟା)argument.getFlags());
            ଯଦି (argument.getObj() == ଖାଲି)
            {
                ss << " obj=NULL";
                ଭାଙ୍ଗ; // done
            }
            ss << cv::format(" total(-1)=%lld", (long long ପୂର୍ଣ୍ଣସଂଖ୍ୟା)argument.total(-1));
            ss << cv::format(" dims(-1)=%d", argument.dims(-1));
            Size size = argument.size(-1);
            ss << cv::format(" size(-1)=%dx%d", size.width, size.height);
            ଯଦି (argument.total(-1) > 0)
            {
                ss << " type(0)=" << cv::typeToString(argument.type(0));
                ପୂର୍ଣ୍ଣସଂଖ୍ୟା dims = argument.dims(0);
                ss << cv::format(" dims(0)=%d", dims);
                ଯଦି (dims <= 2)
                {
                    Size size0 = argument.size(0);
                    ss << cv::format(" size(0)=%dx%d", size0.width, size0.height);
                }
                ଅନ୍ୟଥା
                {
                    ପୂର୍ଣ୍ଣସଂଖ୍ୟା sz[CV_MAX_DIM] = {0};
                    argument.sizend(sz, 0);
                    ss << " size(0)=[";
                    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < dims; i++)
                    {
                        ଯଦି (i > 0)
                            ss << ' ';
                        ss << sz[i];
                    }
                    ss << "]";
                }
            }
        } ଯେତେବେଳେ (0);
    }
    ଧର (ଅଚଳ std::exception& e)
    {
        ss << " ERROR: exception occurred: " << e.what();
    }
    ଧର (...)
    {
        ss << " ERROR: unknown exception occurred, dump is non-complete";
    }
    ଫେରନ୍ତୁ ss.str();
}

CV_EXPORTS_W String dump_ନିବେଶ_Output_Array(ନିବେଶ_Output_Array argument)
{
    ଯଦି (&argument == &noArray())
        ଫେରନ୍ତୁ "InputOutputArray: noArray()";
    std::ostringstream ss;
    ss << "InputOutputArray:";
    ଚେଷ୍ଟା {
        କର {
            ss << (argument.empty() ? " empty()=true" : " empty()=false");
            ss << cv::format(" kind=0x%08llx", (long long ପୂର୍ଣ୍ଣସଂଖ୍ୟା)argument.kind());
            ss << cv::format(" flags=0x%08llx", (long long ପୂର୍ଣ୍ଣସଂଖ୍ୟା)argument.getFlags());
            ଯଦି (argument.getObj() == ଖାଲି)
            {
                ss << " obj=NULL";
                ଭାଙ୍ଗ; // done
            }
            ss << cv::format(" total(-1)=%lld", (long long ପୂର୍ଣ୍ଣସଂଖ୍ୟା)argument.total(-1));
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା dims = argument.dims(-1);
            ss << cv::format(" dims(-1)=%d", dims);
            ଯଦି (dims <= 2)
            {
                Size size = argument.size(-1);
                ss << cv::format(" size(-1)=%dx%d", size.width, size.height);
            }
            ଅନ୍ୟଥା
            {
                ପୂର୍ଣ୍ଣସଂଖ୍ୟା sz[CV_MAX_DIM] = {0};
                argument.sizend(sz, -1);
                ss << " size(-1)=[";
                ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < dims; i++)
                {
                    ଯଦି (i > 0)
                        ss << ' ';
                    ss << sz[i];
                }
                ss << "]";
            }
            ss << " type(-1)=" << cv::typeToString(argument.type(-1));
        } ଯେତେବେଳେ (0);
    }
    ଧର (ଅଚଳ std::exception& e)
    {
        ss << " ERROR: exception occurred: " << e.what();
    }
    ଧର (...)
    {
        ss << " ERROR: unknown exception occurred, dump is non-complete";
    }
    ଫେରନ୍ତୁ ss.str();
}

CV_EXPORTS_W String dump_ନିବେଶ_Output_Array_Of_Arrays(ନିବେଶ_Output_Array_Of_Arrays argument)
{
    ଯଦି (&argument == &noArray())
        ଫେରନ୍ତୁ "InputOutputArrayOfArrays: noArray()";
    std::ostringstream ss;
    ss << "InputOutputArrayOfArrays:";
    ଚେଷ୍ଟା {
        କର {
            ss << (argument.empty() ? " empty()=true" : " empty()=false");
            ss << cv::format(" kind=0x%08llx", (long long ପୂର୍ଣ୍ଣସଂଖ୍ୟା)argument.kind());
            ss << cv::format(" flags=0x%08llx", (long long ପୂର୍ଣ୍ଣସଂଖ୍ୟା)argument.getFlags());
            ଯଦି (argument.getObj() == ଖାଲି)
            {
                ss << " obj=NULL";
                ଭାଙ୍ଗ; // done
            }
            ss << cv::format(" total(-1)=%lld", (long long ପୂର୍ଣ୍ଣସଂଖ୍ୟା)argument.total(-1));
            ss << cv::format(" dims(-1)=%d", argument.dims(-1));
            Size size = argument.size(-1);
            ss << cv::format(" size(-1)=%dx%d", size.width, size.height);
            ଯଦି (argument.total(-1) > 0)
            {
                ss << " type(0)=" << cv::typeToString(argument.type(0));
                ପୂର୍ଣ୍ଣସଂଖ୍ୟା dims = argument.dims(0);
                ss << cv::format(" dims(0)=%d", dims);
                ଯଦି (dims <= 2)
                {
                    Size size0 = argument.size(0);
                    ss << cv::format(" size(0)=%dx%d", size0.width, size0.height);
                }
                ଅନ୍ୟଥା
                {
                    ପୂର୍ଣ୍ଣସଂଖ୍ୟା sz[CV_MAX_DIM] = {0};
                    argument.sizend(sz, 0);
                    ss << " size(0)=[";
                    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < dims; i++)
                    {
                        ଯଦି (i > 0)
                            ss << ' ';
                        ss << sz[i];
                    }
                    ss << "]";
                }
            }
        } ଯେତେବେଳେ (0);
    }
    ଧର (ଅଚଳ std::exception& e)
    {
        ss << " ERROR: exception occurred: " << e.what();
    }
    ଧର (...)
    {
        ss << " ERROR: unknown exception occurred, dump is non-complete";
    }
    ଫେରନ୍ତୁ ss.str();
}

template <ଶ୍ରେଣୀ T, ଶ୍ରେଣୀ Formatter>
ସ୍ଥିର ଧାଡ଼ିଭିତରେ String dumpVector(ଅଚଳ std::vector<T>& vec, Formatter format)
{
    std::ostringstream oss("[", std::ios::ate);
    ଯଦି (!vec.empty())
    {
        format(oss) << vec[0];
        ପାଇଁ (std::size_t i = 1; i < vec.size(); ++i)
        {
            oss << ", ";
            format(oss) << vec[i];
        }
    }
    oss << "]";
    ଫେରନ୍ତୁ oss.str();
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ std::ostream& noFormat(std::ostream& os)
{
    ଫେରନ୍ତୁ os;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ std::ostream& floatFormat(std::ostream& os)
{
    ଫେରନ୍ତୁ os << std::fixed << std::setprecision(2);
}

String dumpVectorOfInt(ଅଚଳ std::vector<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>& vec)
{
    ଫେରନ୍ତୁ dumpVector(vec, &noFormat);
}

String dumpVectorOfDouble(ଅଚଳ std::vector<ଦ୍ୱିଗୁଣ>& vec)
{
    ଫେରନ୍ତୁ dumpVector(vec, &floatFormat);
}

String dumpVectorOfRect(ଅଚଳ std::vector<Rect>& vec)
{
    ଫେରନ୍ତୁ dumpVector(vec, &noFormat);
}


ନାମାକାଶ fs {
cv::String getCacheDirectoryForDownloads()
{
#ଯଦି OPENCV_HAVE_FILESYSTEM_SUPPORT
    ଫେରନ୍ତୁ cv::utils::fs::getCacheDirectory("downloads", "OPENCV_DOWNLOADS_CACHE_DIR");
#ଅନ୍ୟଥା
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#endif
}
} // namespace fs

}} // namespace
