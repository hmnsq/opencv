// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ಅನ್ತರ್ಭಾವ "precomp.hpp"
#ಅನ್ತರ್ಭಾವ "opencv2/core/bindings_utils.hpp"
#ಅನ್ತರ್ಭಾವ <sstream>
#ಅನ್ತರ್ಭಾವ <iomanip>
#ಅನ್ತರ್ಭಾವ <opencv2/core/utils/filesystem.hpp>
#ಅನ್ತರ್ಭಾವ <opencv2/core/utils/filesystem.private.hpp>

ನಾಮಾಕಾಶ cv {
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ std::ostream& operator<<(std::ostream& os, ಧ್ರುವ Rect& rect)
{
    ಪ್ರತ್ಯಾಗಮ os << "[x=" << rect.x << ", y=" << rect.y << ", w=" << rect.width << ", h=" << rect.height << ']';
}
ನಾಮಾಕಾಶ utils {

String dump_ಒಳಹರಿವು_Array(ಒಳಹರಿವು_Array argument)
{
    ಯದಿ (&argument == &noArray())
        ಪ್ರತ್ಯಾಗಮ "InputArray: noArray()";
    std::ostringstream ss;
    ss << "InputArray:";
    ಯತ್ನ {
        ಕುರು {
            ss << (argument.empty() ? " empty()=true" : " empty()=false");
            ss << cv::format(" kind=0x%08llx", (long long ಪೂರ್ಣಾಙ್ಕ)argument.kind());
            ss << cv::format(" flags=0x%08llx", (long long ಪೂರ್ಣಾಙ್ಕ)argument.getFlags());
            ಯದಿ (argument.getObj() == ರಿಕ್ತ)
            {
                ss << " obj=NULL";
                ಭಙ್ಗ; // done
            }
            ss << cv::format(" total(-1)=%lld", (long long ಪೂರ್ಣಾಙ್ಕ)argument.total(-1));
            ಪೂರ್ಣಾಙ್ಕ dims = argument.dims(-1);
            ss << cv::format(" dims(-1)=%d", dims);
            ಯದಿ (dims <= 2)
            {
                Size size = argument.size(-1);
                ss << cv::format(" size(-1)=%dx%d", size.width, size.height);
            }
            ಅನ್ಯಥಾ
            {
                ಪೂರ್ಣಾಙ್ಕ sz[CV_MAX_DIM] = {0};
                argument.sizend(sz, -1);
                ss << " size(-1)=[";
                ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < dims; i++)
                {
                    ಯದಿ (i > 0)
                        ss << ' ';
                    ss << sz[i];
                }
                ss << "]";
            }
            ss << " type(-1)=" << cv::typeToString(argument.type(-1));
        } ಯಾವತ್ (0);
    }
    ಗೃಹಾಣ (ಧ್ರುವ std::exception& e)
    {
        ss << " ERROR: exception occurred: " << e.what();
    }
    ಗೃಹಾಣ (...)
    {
        ss << " ERROR: unknown exception occurred, dump is non-complete";
    }
    ಪ್ರತ್ಯಾಗಮ ss.str();
}

CV_EXPORTS_W String dump_ಒಳಹರಿವು_Array_Of_Arrays(ಒಳಹರಿವು_Array_Of_Arrays argument)
{
    ಯದಿ (&argument == &noArray())
        ಪ್ರತ್ಯಾಗಮ "InputArrayOfArrays: noArray()";
    std::ostringstream ss;
    ss << "InputArrayOfArrays:";
    ಯತ್ನ {
        ಕುರು {
            ss << (argument.empty() ? " empty()=true" : " empty()=false");
            ss << cv::format(" kind=0x%08llx", (long long ಪೂರ್ಣಾಙ್ಕ)argument.kind());
            ss << cv::format(" flags=0x%08llx", (long long ಪೂರ್ಣಾಙ್ಕ)argument.getFlags());
            ಯದಿ (argument.getObj() == ರಿಕ್ತ)
            {
                ss << " obj=NULL";
                ಭಙ್ಗ; // done
            }
            ss << cv::format(" total(-1)=%lld", (long long ಪೂರ್ಣಾಙ್ಕ)argument.total(-1));
            ss << cv::format(" dims(-1)=%d", argument.dims(-1));
            Size size = argument.size(-1);
            ss << cv::format(" size(-1)=%dx%d", size.width, size.height);
            ಯದಿ (argument.total(-1) > 0)
            {
                ss << " type(0)=" << cv::typeToString(argument.type(0));
                ಪೂರ್ಣಾಙ್ಕ dims = argument.dims(0);
                ss << cv::format(" dims(0)=%d", dims);
                ಯದಿ (dims <= 2)
                {
                    Size size0 = argument.size(0);
                    ss << cv::format(" size(0)=%dx%d", size0.width, size0.height);
                }
                ಅನ್ಯಥಾ
                {
                    ಪೂರ್ಣಾಙ್ಕ sz[CV_MAX_DIM] = {0};
                    argument.sizend(sz, 0);
                    ss << " size(0)=[";
                    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < dims; i++)
                    {
                        ಯದಿ (i > 0)
                            ss << ' ';
                        ss << sz[i];
                    }
                    ss << "]";
                }
            }
        } ಯಾವತ್ (0);
    }
    ಗೃಹಾಣ (ಧ್ರುವ std::exception& e)
    {
        ss << " ERROR: exception occurred: " << e.what();
    }
    ಗೃಹಾಣ (...)
    {
        ss << " ERROR: unknown exception occurred, dump is non-complete";
    }
    ಪ್ರತ್ಯಾಗಮ ss.str();
}

CV_EXPORTS_W String dump_ಒಳಹರಿವು_Output_Array(ಒಳಹರಿವು_Output_Array argument)
{
    ಯದಿ (&argument == &noArray())
        ಪ್ರತ್ಯಾಗಮ "InputOutputArray: noArray()";
    std::ostringstream ss;
    ss << "InputOutputArray:";
    ಯತ್ನ {
        ಕುರು {
            ss << (argument.empty() ? " empty()=true" : " empty()=false");
            ss << cv::format(" kind=0x%08llx", (long long ಪೂರ್ಣಾಙ್ಕ)argument.kind());
            ss << cv::format(" flags=0x%08llx", (long long ಪೂರ್ಣಾಙ್ಕ)argument.getFlags());
            ಯದಿ (argument.getObj() == ರಿಕ್ತ)
            {
                ss << " obj=NULL";
                ಭಙ್ಗ; // done
            }
            ss << cv::format(" total(-1)=%lld", (long long ಪೂರ್ಣಾಙ್ಕ)argument.total(-1));
            ಪೂರ್ಣಾಙ್ಕ dims = argument.dims(-1);
            ss << cv::format(" dims(-1)=%d", dims);
            ಯದಿ (dims <= 2)
            {
                Size size = argument.size(-1);
                ss << cv::format(" size(-1)=%dx%d", size.width, size.height);
            }
            ಅನ್ಯಥಾ
            {
                ಪೂರ್ಣಾಙ್ಕ sz[CV_MAX_DIM] = {0};
                argument.sizend(sz, -1);
                ss << " size(-1)=[";
                ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < dims; i++)
                {
                    ಯದಿ (i > 0)
                        ss << ' ';
                    ss << sz[i];
                }
                ss << "]";
            }
            ss << " type(-1)=" << cv::typeToString(argument.type(-1));
        } ಯಾವತ್ (0);
    }
    ಗೃಹಾಣ (ಧ್ರುವ std::exception& e)
    {
        ss << " ERROR: exception occurred: " << e.what();
    }
    ಗೃಹಾಣ (...)
    {
        ss << " ERROR: unknown exception occurred, dump is non-complete";
    }
    ಪ್ರತ್ಯಾಗಮ ss.str();
}

CV_EXPORTS_W String dump_ಒಳಹರಿವು_Output_Array_Of_Arrays(ಒಳಹರಿವು_Output_Array_Of_Arrays argument)
{
    ಯದಿ (&argument == &noArray())
        ಪ್ರತ್ಯಾಗಮ "InputOutputArrayOfArrays: noArray()";
    std::ostringstream ss;
    ss << "InputOutputArrayOfArrays:";
    ಯತ್ನ {
        ಕುರು {
            ss << (argument.empty() ? " empty()=true" : " empty()=false");
            ss << cv::format(" kind=0x%08llx", (long long ಪೂರ್ಣಾಙ್ಕ)argument.kind());
            ss << cv::format(" flags=0x%08llx", (long long ಪೂರ್ಣಾಙ್ಕ)argument.getFlags());
            ಯದಿ (argument.getObj() == ರಿಕ್ತ)
            {
                ss << " obj=NULL";
                ಭಙ್ಗ; // done
            }
            ss << cv::format(" total(-1)=%lld", (long long ಪೂರ್ಣಾಙ್ಕ)argument.total(-1));
            ss << cv::format(" dims(-1)=%d", argument.dims(-1));
            Size size = argument.size(-1);
            ss << cv::format(" size(-1)=%dx%d", size.width, size.height);
            ಯದಿ (argument.total(-1) > 0)
            {
                ss << " type(0)=" << cv::typeToString(argument.type(0));
                ಪೂರ್ಣಾಙ್ಕ dims = argument.dims(0);
                ss << cv::format(" dims(0)=%d", dims);
                ಯದಿ (dims <= 2)
                {
                    Size size0 = argument.size(0);
                    ss << cv::format(" size(0)=%dx%d", size0.width, size0.height);
                }
                ಅನ್ಯಥಾ
                {
                    ಪೂರ್ಣಾಙ್ಕ sz[CV_MAX_DIM] = {0};
                    argument.sizend(sz, 0);
                    ss << " size(0)=[";
                    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < dims; i++)
                    {
                        ಯದಿ (i > 0)
                            ss << ' ';
                        ss << sz[i];
                    }
                    ss << "]";
                }
            }
        } ಯಾವತ್ (0);
    }
    ಗೃಹಾಣ (ಧ್ರುವ std::exception& e)
    {
        ss << " ERROR: exception occurred: " << e.what();
    }
    ಗೃಹಾಣ (...)
    {
        ss << " ERROR: unknown exception occurred, dump is non-complete";
    }
    ಪ್ರತ್ಯಾಗಮ ss.str();
}

template <ವರ್ಗ T, ವರ್ಗ Formatter>
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ String dumpVector(ಧ್ರುವ std::vector<T>& vec, Formatter format)
{
    std::ostringstream oss("[", std::ios::ate);
    ಯದಿ (!vec.empty())
    {
        format(oss) << vec[0];
        ಪ್ರತಿ (std::size_t i = 1; i < vec.size(); ++i)
        {
            oss << ", ";
            format(oss) << vec[i];
        }
    }
    oss << "]";
    ಪ್ರತ್ಯಾಗಮ oss.str();
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ std::ostream& noFormat(std::ostream& os)
{
    ಪ್ರತ್ಯಾಗಮ os;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ std::ostream& floatFormat(std::ostream& os)
{
    ಪ್ರತ್ಯಾಗಮ os << std::fixed << std::setprecision(2);
}

String dumpVectorOfInt(ಧ್ರುವ std::vector<ಪೂರ್ಣಾಙ್ಕ>& vec)
{
    ಪ್ರತ್ಯಾಗಮ dumpVector(vec, &noFormat);
}

String dumpVectorOfDouble(ಧ್ರುವ std::vector<ದ್ವಿಗುಣ>& vec)
{
    ಪ್ರತ್ಯಾಗಮ dumpVector(vec, &floatFormat);
}

String dumpVectorOfRect(ಧ್ರುವ std::vector<Rect>& vec)
{
    ಪ್ರತ್ಯಾಗಮ dumpVector(vec, &noFormat);
}


ನಾಮಾಕಾಶ fs {
cv::String getCacheDirectoryForDownloads()
{
#ಯದಿ OPENCV_HAVE_FILESYSTEM_SUPPORT
    ಪ್ರತ್ಯಾಗಮ cv::utils::fs::getCacheDirectory("downloads", "OPENCV_DOWNLOADS_CACHE_DIR");
#ಅನ್ಯಥಾ
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#endif
}
} // namespace fs

}} // namespace
