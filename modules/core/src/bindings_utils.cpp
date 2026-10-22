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
ಸ್ಥಿರ ಸಾಲೊಳಗೆ std::ostream& ಕ್ರಿಯಾಕಾರಕ<<(std::ostream& os, ಸ್ಥಿರಾಂಕ Rect& rect)
{
    ಹಿಂತಿರುಗಿ os << "[x=" << rect.x << ", y=" << rect.y << ", w=" << rect.ಅಗಲ << ", h=" << rect.ಎತ್ತರ << ']';
}
ನಾಮಾಕಾಶ utils {

String dump_ಒಳಹರಿವು_Array(ಒಳಹರಿವು_Array argument)
{
    ಆದರೆ (&argument == &noArray())
        ಹಿಂತಿರುಗಿ "InputArray: noArray()";
    std::ostringstream ss;
    ss << "InputArray:";
    ಪ್ರಯತ್ನಿಸು {
        ಮಾಡು {
            ss << (argument.empty() ? " empty()=true" : " empty()=false");
            ss << cv::format(" kind=0x%08llx", (ದೀರ್ಘ ದೀರ್ಘ ಪೂರ್ಣಾಂಕ)argument.ಬಗೆ());
            ss << cv::format(" flags=0x%08llx", (ದೀರ್ಘ ದೀರ್ಘ ಪೂರ್ಣಾಂಕ)argument.getFlags());
            ಆದರೆ (argument.getObj() == ಖಾಲಿ)
            {
                ss << " obj=NULL";
                ನಿಲ್ಲಿಸು; //ಮುಗಿದ
            }
            ss << cv::format(" total(-1)=%lld", (ದೀರ್ಘ ದೀರ್ಘ ಪೂರ್ಣಾಂಕ)argument.ಒಟ್ಟು(-1));
            ಪೂರ್ಣಾಂಕ dims = argument.dims(-1);
            ss << cv::format(" dims(-1)=%d", dims);
            ಆದರೆ (dims <= 2)
            {
                ಗಾತ್ರ_2 size = argument.size(-1);
                ss << cv::format(" size(-1)=%dx%d", size.ಅಗಲ, size.ಎತ್ತರ);
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                ಪೂರ್ಣಾಂಕ sz[CV_MAX_DIM] = {0};
                argument.sizend(sz, -1);
                ss << " size(-1)=[";
                ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < dims; i++)
                {
                    ಆದರೆ (i > 0)
                        ss << ' ';
                    ss << sz[i];
                }
                ss << "]";
            }
            ss << " type(-1)=" << cv::type_ಗಮ್ಯ_String(argument.type(-1));
        } ಆಗ (0);
    }
    ಹಿಡಿ (ಸ್ಥಿರಾಂಕ std::exception& e)
    {
        ss << " ERROR: exception occurred: " << e.what();
    }
    ಹಿಡಿ (...)
    {
        ss << " ERROR: unknown exception occurred, dump is non-complete";
    }
    ಹಿಂತಿರುಗಿ ss.str();
}

CV_EXPORTS_W String dump_ಒಳಹರಿವು_Array_ದ_Arrays(ಒಳಹರಿವು_Array_ದ_Arrays argument)
{
    ಆದರೆ (&argument == &noArray())
        ಹಿಂತಿರುಗಿ "InputArrayOfArrays: noArray()";
    std::ostringstream ss;
    ss << "InputArrayOfArrays:";
    ಪ್ರಯತ್ನಿಸು {
        ಮಾಡು {
            ss << (argument.empty() ? " empty()=true" : " empty()=false");
            ss << cv::format(" kind=0x%08llx", (ದೀರ್ಘ ದೀರ್ಘ ಪೂರ್ಣಾಂಕ)argument.ಬಗೆ());
            ss << cv::format(" flags=0x%08llx", (ದೀರ್ಘ ದೀರ್ಘ ಪೂರ್ಣಾಂಕ)argument.getFlags());
            ಆದರೆ (argument.getObj() == ಖಾಲಿ)
            {
                ss << " obj=NULL";
                ನಿಲ್ಲಿಸು; //ಮುಗಿದ
            }
            ss << cv::format(" total(-1)=%lld", (ದೀರ್ಘ ದೀರ್ಘ ಪೂರ್ಣಾಂಕ)argument.ಒಟ್ಟು(-1));
            ss << cv::format(" dims(-1)=%d", argument.dims(-1));
            ಗಾತ್ರ_2 size = argument.size(-1);
            ss << cv::format(" size(-1)=%dx%d", size.ಅಗಲ, size.ಎತ್ತರ);
            ಆದರೆ (argument.ಒಟ್ಟು(-1) > 0)
            {
                ss << " type(0)=" << cv::type_ಗಮ್ಯ_String(argument.type(0));
                ಪೂರ್ಣಾಂಕ dims = argument.dims(0);
                ss << cv::format(" dims(0)=%d", dims);
                ಆದರೆ (dims <= 2)
                {
                    ಗಾತ್ರ_2 size0 = argument.size(0);
                    ss << cv::format(" size(0)=%dx%d", size0.ಅಗಲ, size0.ಎತ್ತರ);
                }
                ಇಲ್ಲದಿದ್ದರೆ
                {
                    ಪೂರ್ಣಾಂಕ sz[CV_MAX_DIM] = {0};
                    argument.sizend(sz, 0);
                    ss << " size(0)=[";
                    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < dims; i++)
                    {
                        ಆದರೆ (i > 0)
                            ss << ' ';
                        ss << sz[i];
                    }
                    ss << "]";
                }
            }
        } ಆಗ (0);
    }
    ಹಿಡಿ (ಸ್ಥಿರಾಂಕ std::exception& e)
    {
        ss << " ERROR: exception occurred: " << e.what();
    }
    ಹಿಡಿ (...)
    {
        ss << " ERROR: unknown exception occurred, dump is non-complete";
    }
    ಹಿಂತಿರುಗಿ ss.str();
}

CV_EXPORTS_W String dump_ಒಳಹರಿವು_Output_Array(ಒಳಹರಿವು_Output_Array argument)
{
    ಆದರೆ (&argument == &noArray())
        ಹಿಂತಿರುಗಿ "InputOutputArray: noArray()";
    std::ostringstream ss;
    ss << "InputOutputArray:";
    ಪ್ರಯತ್ನಿಸು {
        ಮಾಡು {
            ss << (argument.empty() ? " empty()=true" : " empty()=false");
            ss << cv::format(" kind=0x%08llx", (ದೀರ್ಘ ದೀರ್ಘ ಪೂರ್ಣಾಂಕ)argument.ಬಗೆ());
            ss << cv::format(" flags=0x%08llx", (ದೀರ್ಘ ದೀರ್ಘ ಪೂರ್ಣಾಂಕ)argument.getFlags());
            ಆದರೆ (argument.getObj() == ಖಾಲಿ)
            {
                ss << " obj=NULL";
                ನಿಲ್ಲಿಸು; //ಮುಗಿದ
            }
            ss << cv::format(" total(-1)=%lld", (ದೀರ್ಘ ದೀರ್ಘ ಪೂರ್ಣಾಂಕ)argument.ಒಟ್ಟು(-1));
            ಪೂರ್ಣಾಂಕ dims = argument.dims(-1);
            ss << cv::format(" dims(-1)=%d", dims);
            ಆದರೆ (dims <= 2)
            {
                ಗಾತ್ರ_2 size = argument.size(-1);
                ss << cv::format(" size(-1)=%dx%d", size.ಅಗಲ, size.ಎತ್ತರ);
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                ಪೂರ್ಣಾಂಕ sz[CV_MAX_DIM] = {0};
                argument.sizend(sz, -1);
                ss << " size(-1)=[";
                ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < dims; i++)
                {
                    ಆದರೆ (i > 0)
                        ss << ' ';
                    ss << sz[i];
                }
                ss << "]";
            }
            ss << " type(-1)=" << cv::type_ಗಮ್ಯ_String(argument.type(-1));
        } ಆಗ (0);
    }
    ಹಿಡಿ (ಸ್ಥಿರಾಂಕ std::exception& e)
    {
        ss << " ERROR: exception occurred: " << e.what();
    }
    ಹಿಡಿ (...)
    {
        ss << " ERROR: unknown exception occurred, dump is non-complete";
    }
    ಹಿಂತಿರುಗಿ ss.str();
}

CV_EXPORTS_W String dump_ಒಳಹರಿವು_Output_Array_ದ_Arrays(ಒಳಹರಿವು_Output_Array_ದ_Arrays argument)
{
    ಆದರೆ (&argument == &noArray())
        ಹಿಂತಿರುಗಿ "InputOutputArrayOfArrays: noArray()";
    std::ostringstream ss;
    ss << "InputOutputArrayOfArrays:";
    ಪ್ರಯತ್ನಿಸು {
        ಮಾಡು {
            ss << (argument.empty() ? " empty()=true" : " empty()=false");
            ss << cv::format(" kind=0x%08llx", (ದೀರ್ಘ ದೀರ್ಘ ಪೂರ್ಣಾಂಕ)argument.ಬಗೆ());
            ss << cv::format(" flags=0x%08llx", (ದೀರ್ಘ ದೀರ್ಘ ಪೂರ್ಣಾಂಕ)argument.getFlags());
            ಆದರೆ (argument.getObj() == ಖಾಲಿ)
            {
                ss << " obj=NULL";
                ನಿಲ್ಲಿಸು; //ಮುಗಿದ
            }
            ss << cv::format(" total(-1)=%lld", (ದೀರ್ಘ ದೀರ್ಘ ಪೂರ್ಣಾಂಕ)argument.ಒಟ್ಟು(-1));
            ss << cv::format(" dims(-1)=%d", argument.dims(-1));
            ಗಾತ್ರ_2 size = argument.size(-1);
            ss << cv::format(" size(-1)=%dx%d", size.ಅಗಲ, size.ಎತ್ತರ);
            ಆದರೆ (argument.ಒಟ್ಟು(-1) > 0)
            {
                ss << " type(0)=" << cv::type_ಗಮ್ಯ_String(argument.type(0));
                ಪೂರ್ಣಾಂಕ dims = argument.dims(0);
                ss << cv::format(" dims(0)=%d", dims);
                ಆದರೆ (dims <= 2)
                {
                    ಗಾತ್ರ_2 size0 = argument.size(0);
                    ss << cv::format(" size(0)=%dx%d", size0.ಅಗಲ, size0.ಎತ್ತರ);
                }
                ಇಲ್ಲದಿದ್ದರೆ
                {
                    ಪೂರ್ಣಾಂಕ sz[CV_MAX_DIM] = {0};
                    argument.sizend(sz, 0);
                    ss << " size(0)=[";
                    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < dims; i++)
                    {
                        ಆದರೆ (i > 0)
                            ss << ' ';
                        ss << sz[i];
                    }
                    ss << "]";
                }
            }
        } ಆಗ (0);
    }
    ಹಿಡಿ (ಸ್ಥಿರಾಂಕ std::exception& e)
    {
        ss << " ERROR: exception occurred: " << e.what();
    }
    ಹಿಡಿ (...)
    {
        ss << " ERROR: unknown exception occurred, dump is non-complete";
    }
    ಹಿಂತಿರುಗಿ ss.str();
}

ಮಾದರಿ <ವರ್ಗ T, ವರ್ಗ Formatter>
ಸ್ಥಿರ ಸಾಲೊಳಗೆ String dump_ಸದಿಶ(ಸ್ಥಿರಾಂಕ std::vector<T>& vec, Formatter format)
{
    std::ostringstream oss("[", std::ios::ate);
    ಆದರೆ (!vec.empty())
    {
        format(oss) << vec[0];
        ಗಾಗಿ (std::size_t i = 1; i < vec.size(); ++i)
        {
            oss << ", ";
            format(oss) << vec[i];
        }
    }
    oss << "]";
    ಹಿಂತಿರುಗಿ oss.str();
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ std::ostream& noFormat(std::ostream& os)
{
    ಹಿಂತಿರುಗಿ os;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ std::ostream& floatFormat(std::ostream& os)
{
    ಹಿಂತಿರುಗಿ os << std::fixed << std::setprecision(2);
}

String dump_ಸದಿಶ_ದ_Int(ಸ್ಥಿರಾಂಕ std::vector<ಪೂರ್ಣಾಂಕ>& vec)
{
    ಹಿಂತಿರುಗಿ dump_ಸದಿಶ(vec, &noFormat);
}

String dump_ಸದಿಶ_ದ_Double(ಸ್ಥಿರಾಂಕ std::vector<ದ್ವಿಗುಣ>& vec)
{
    ಹಿಂತಿರುಗಿ dump_ಸದಿಶ(vec, &floatFormat);
}

String dump_ಸದಿಶ_ದ_Rect(ಸ್ಥಿರಾಂಕ std::vector<Rect>& vec)
{
    ಹಿಂತಿರುಗಿ dump_ಸದಿಶ(vec, &noFormat);
}


ನಾಮಾಕಾಶ fs {
cv::String getCacheDirectoryForDownloads()
{
#ಆದರೆ OPENCV_HAVE_FILESYSTEM_SUPPORT
    ಹಿಂತಿರುಗಿ cv::utils::fs::getCacheDirectory("downloads", "OPENCV_DOWNLOADS_CACHE_DIR");
#ಇಲ್ಲದಿದ್ದರೆ
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#ಆದರೆ_ಅಂತ್ಯ
}
} // namespace fs

}} // namespace
