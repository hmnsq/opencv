// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#അന്തര്ഭാവ "precomp.hpp"
#അന്തര്ഭാവ "opencv2/core/bindings_utils.hpp"
#അന്തര്ഭാവ <sstream>
#അന്തര്ഭാവ <iomanip>
#അന്തര്ഭാവ <opencv2/core/utils/filesystem.hpp>
#അന്തര്ഭാവ <opencv2/core/utils/filesystem.private.hpp>

നാമാകാശ cv {
സ്ഥിതം വരിയിൽ std::ostream& operator<<(std::ostream& os, സ്ഥിരം Rect& rect)
{
    തിരികെ os << "[x=" << rect.x << ", y=" << rect.y << ", w=" << rect.width << ", h=" << rect.height << ']';
}
നാമാകാശ utils {

String dump_നിവേശം_Array(നിവേശം_Array argument)
{
    എങ്കിൽ (&argument == &noArray())
        തിരികെ "InputArray: noArray()";
    std::ostringstream ss;
    ss << "InputArray:";
    ശ്രമിക്കുക {
        ചെയ്യുക {
            ss << (argument.empty() ? " empty()=true" : " empty()=false");
            ss << cv::format(" kind=0x%08llx", (long long പൂർണ്ണസംഖ്യ)argument.kind());
            ss << cv::format(" flags=0x%08llx", (long long പൂർണ്ണസംഖ്യ)argument.getFlags());
            എങ്കിൽ (argument.getObj() == ശൂന്യ)
            {
                ss << " obj=NULL";
                നിർത്തുക; // done
            }
            ss << cv::format(" total(-1)=%lld", (long long പൂർണ്ണസംഖ്യ)argument.total(-1));
            പൂർണ്ണസംഖ്യ dims = argument.dims(-1);
            ss << cv::format(" dims(-1)=%d", dims);
            എങ്കിൽ (dims <= 2)
            {
                Size size = argument.size(-1);
                ss << cv::format(" size(-1)=%dx%d", size.width, size.height);
            }
            അല്ലെങ്കിൽ
            {
                പൂർണ്ണസംഖ്യ sz[CV_MAX_DIM] = {0};
                argument.sizend(sz, -1);
                ss << " size(-1)=[";
                വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < dims; i++)
                {
                    എങ്കിൽ (i > 0)
                        ss << ' ';
                    ss << sz[i];
                }
                ss << "]";
            }
            ss << " type(-1)=" << cv::typeToString(argument.type(-1));
        } ആകുമ്പോൾ (0);
    }
    പിടിക്കുക (സ്ഥിരം std::exception& e)
    {
        ss << " ERROR: exception occurred: " << e.what();
    }
    പിടിക്കുക (...)
    {
        ss << " ERROR: unknown exception occurred, dump is non-complete";
    }
    തിരികെ ss.str();
}

CV_EXPORTS_W String dump_നിവേശം_Array_Of_Arrays(നിവേശം_Array_Of_Arrays argument)
{
    എങ്കിൽ (&argument == &noArray())
        തിരികെ "InputArrayOfArrays: noArray()";
    std::ostringstream ss;
    ss << "InputArrayOfArrays:";
    ശ്രമിക്കുക {
        ചെയ്യുക {
            ss << (argument.empty() ? " empty()=true" : " empty()=false");
            ss << cv::format(" kind=0x%08llx", (long long പൂർണ്ണസംഖ്യ)argument.kind());
            ss << cv::format(" flags=0x%08llx", (long long പൂർണ്ണസംഖ്യ)argument.getFlags());
            എങ്കിൽ (argument.getObj() == ശൂന്യ)
            {
                ss << " obj=NULL";
                നിർത്തുക; // done
            }
            ss << cv::format(" total(-1)=%lld", (long long പൂർണ്ണസംഖ്യ)argument.total(-1));
            ss << cv::format(" dims(-1)=%d", argument.dims(-1));
            Size size = argument.size(-1);
            ss << cv::format(" size(-1)=%dx%d", size.width, size.height);
            എങ്കിൽ (argument.total(-1) > 0)
            {
                ss << " type(0)=" << cv::typeToString(argument.type(0));
                പൂർണ്ണസംഖ്യ dims = argument.dims(0);
                ss << cv::format(" dims(0)=%d", dims);
                എങ്കിൽ (dims <= 2)
                {
                    Size size0 = argument.size(0);
                    ss << cv::format(" size(0)=%dx%d", size0.width, size0.height);
                }
                അല്ലെങ്കിൽ
                {
                    പൂർണ്ണസംഖ്യ sz[CV_MAX_DIM] = {0};
                    argument.sizend(sz, 0);
                    ss << " size(0)=[";
                    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < dims; i++)
                    {
                        എങ്കിൽ (i > 0)
                            ss << ' ';
                        ss << sz[i];
                    }
                    ss << "]";
                }
            }
        } ആകുമ്പോൾ (0);
    }
    പിടിക്കുക (സ്ഥിരം std::exception& e)
    {
        ss << " ERROR: exception occurred: " << e.what();
    }
    പിടിക്കുക (...)
    {
        ss << " ERROR: unknown exception occurred, dump is non-complete";
    }
    തിരികെ ss.str();
}

CV_EXPORTS_W String dump_നിവേശം_Output_Array(നിവേശം_Output_Array argument)
{
    എങ്കിൽ (&argument == &noArray())
        തിരികെ "InputOutputArray: noArray()";
    std::ostringstream ss;
    ss << "InputOutputArray:";
    ശ്രമിക്കുക {
        ചെയ്യുക {
            ss << (argument.empty() ? " empty()=true" : " empty()=false");
            ss << cv::format(" kind=0x%08llx", (long long പൂർണ്ണസംഖ്യ)argument.kind());
            ss << cv::format(" flags=0x%08llx", (long long പൂർണ്ണസംഖ്യ)argument.getFlags());
            എങ്കിൽ (argument.getObj() == ശൂന്യ)
            {
                ss << " obj=NULL";
                നിർത്തുക; // done
            }
            ss << cv::format(" total(-1)=%lld", (long long പൂർണ്ണസംഖ്യ)argument.total(-1));
            പൂർണ്ണസംഖ്യ dims = argument.dims(-1);
            ss << cv::format(" dims(-1)=%d", dims);
            എങ്കിൽ (dims <= 2)
            {
                Size size = argument.size(-1);
                ss << cv::format(" size(-1)=%dx%d", size.width, size.height);
            }
            അല്ലെങ്കിൽ
            {
                പൂർണ്ണസംഖ്യ sz[CV_MAX_DIM] = {0};
                argument.sizend(sz, -1);
                ss << " size(-1)=[";
                വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < dims; i++)
                {
                    എങ്കിൽ (i > 0)
                        ss << ' ';
                    ss << sz[i];
                }
                ss << "]";
            }
            ss << " type(-1)=" << cv::typeToString(argument.type(-1));
        } ആകുമ്പോൾ (0);
    }
    പിടിക്കുക (സ്ഥിരം std::exception& e)
    {
        ss << " ERROR: exception occurred: " << e.what();
    }
    പിടിക്കുക (...)
    {
        ss << " ERROR: unknown exception occurred, dump is non-complete";
    }
    തിരികെ ss.str();
}

CV_EXPORTS_W String dump_നിവേശം_Output_Array_Of_Arrays(നിവേശം_Output_Array_Of_Arrays argument)
{
    എങ്കിൽ (&argument == &noArray())
        തിരികെ "InputOutputArrayOfArrays: noArray()";
    std::ostringstream ss;
    ss << "InputOutputArrayOfArrays:";
    ശ്രമിക്കുക {
        ചെയ്യുക {
            ss << (argument.empty() ? " empty()=true" : " empty()=false");
            ss << cv::format(" kind=0x%08llx", (long long പൂർണ്ണസംഖ്യ)argument.kind());
            ss << cv::format(" flags=0x%08llx", (long long പൂർണ്ണസംഖ്യ)argument.getFlags());
            എങ്കിൽ (argument.getObj() == ശൂന്യ)
            {
                ss << " obj=NULL";
                നിർത്തുക; // done
            }
            ss << cv::format(" total(-1)=%lld", (long long പൂർണ്ണസംഖ്യ)argument.total(-1));
            ss << cv::format(" dims(-1)=%d", argument.dims(-1));
            Size size = argument.size(-1);
            ss << cv::format(" size(-1)=%dx%d", size.width, size.height);
            എങ്കിൽ (argument.total(-1) > 0)
            {
                ss << " type(0)=" << cv::typeToString(argument.type(0));
                പൂർണ്ണസംഖ്യ dims = argument.dims(0);
                ss << cv::format(" dims(0)=%d", dims);
                എങ്കിൽ (dims <= 2)
                {
                    Size size0 = argument.size(0);
                    ss << cv::format(" size(0)=%dx%d", size0.width, size0.height);
                }
                അല്ലെങ്കിൽ
                {
                    പൂർണ്ണസംഖ്യ sz[CV_MAX_DIM] = {0};
                    argument.sizend(sz, 0);
                    ss << " size(0)=[";
                    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < dims; i++)
                    {
                        എങ്കിൽ (i > 0)
                            ss << ' ';
                        ss << sz[i];
                    }
                    ss << "]";
                }
            }
        } ആകുമ്പോൾ (0);
    }
    പിടിക്കുക (സ്ഥിരം std::exception& e)
    {
        ss << " ERROR: exception occurred: " << e.what();
    }
    പിടിക്കുക (...)
    {
        ss << " ERROR: unknown exception occurred, dump is non-complete";
    }
    തിരികെ ss.str();
}

template <വർഗ്ഗം T, വർഗ്ഗം Formatter>
സ്ഥിതം വരിയിൽ String dumpVector(സ്ഥിരം std::vector<T>& vec, Formatter format)
{
    std::ostringstream oss("[", std::ios::ate);
    എങ്കിൽ (!vec.empty())
    {
        format(oss) << vec[0];
        വേണ്ടി (std::size_t i = 1; i < vec.size(); ++i)
        {
            oss << ", ";
            format(oss) << vec[i];
        }
    }
    oss << "]";
    തിരികെ oss.str();
}

സ്ഥിതം വരിയിൽ std::ostream& noFormat(std::ostream& os)
{
    തിരികെ os;
}

സ്ഥിതം വരിയിൽ std::ostream& floatFormat(std::ostream& os)
{
    തിരികെ os << std::fixed << std::setprecision(2);
}

String dumpVectorOfInt(സ്ഥിരം std::vector<പൂർണ്ണസംഖ്യ>& vec)
{
    തിരികെ dumpVector(vec, &noFormat);
}

String dumpVectorOfDouble(സ്ഥിരം std::vector<ഇരട്ട>& vec)
{
    തിരികെ dumpVector(vec, &floatFormat);
}

String dumpVectorOfRect(സ്ഥിരം std::vector<Rect>& vec)
{
    തിരികെ dumpVector(vec, &noFormat);
}


നാമാകാശ fs {
cv::String getCacheDirectoryForDownloads()
{
#എങ്കിൽ OPENCV_HAVE_FILESYSTEM_SUPPORT
    തിരികെ cv::utils::fs::getCacheDirectory("downloads", "OPENCV_DOWNLOADS_CACHE_DIR");
#അല്ലെങ്കിൽ
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#endif
}
} // namespace fs

}} // namespace
