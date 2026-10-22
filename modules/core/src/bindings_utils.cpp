// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#شامل "precomp.hpp"
#شامل "opencv2/core/bindings_utils.hpp"
#شامل <sstream>
#شامل <iomanip>
#شامل <opencv2/core/utils/filesystem.hpp>
#شامل <opencv2/core/utils/filesystem.private.hpp>

نامخلا cv {
ساکن اندرونی std::ostream& عامل<<(std::ostream& os, ثابت Rect& rect)
{
    واپس os << "[x=" << rect.x << ", y=" << rect.y << ", w=" << rect.چوڑائی << ", h=" << rect.اونچائی << ']';
}
نامخلا utils {

String dump_داخلہ_Array(داخلہ_Array argument)
{
    اگر (&argument == &noArray())
        واپس "InputArray: noArray()";
    std::ostringstream ss;
    ss << "InputArray:";
    کوشش {
        کرو {
            ss << (argument.empty() ? " empty()=true" : " empty()=false");
            ss << cv::format(" kind=0x%08llx", (طویل طویل صحیح)argument.قسم());
            ss << cv::format(" flags=0x%08llx", (طویل طویل صحیح)argument.getFlags());
            اگر (argument.getObj() == تہی)
            {
                ss << " obj=NULL";
                توڑو; //مکمل
            }
            ss << cv::format(" total(-1)=%lld", (طویل طویل صحیح)argument.کل(-1));
            صحیح dims = argument.dims(-1);
            ss << cv::format(" dims(-1)=%d", dims);
            اگر (dims <= 2)
            {
                سائز size = argument.size(-1);
                ss << cv::format(" size(-1)=%dx%d", size.چوڑائی, size.اونچائی);
            }
            ورنہ
            {
                صحیح sz[CV_MAX_DIM] = {0};
                argument.sizend(sz, -1);
                ss << " size(-1)=[";
                برائے (صحیح i = 0; i < dims; i++)
                {
                    اگر (i > 0)
                        ss << ' ';
                    ss << sz[i];
                }
                ss << "]";
            }
            ss << " type(-1)=" << cv::type_مقصد_String(argument.type(-1));
        } جب (0);
    }
    پکڑو (ثابت std::exception& e)
    {
        ss << " ERROR: exception occurred: " << e.what();
    }
    پکڑو (...)
    {
        ss << " ERROR: unknown exception occurred, dump is non-complete";
    }
    واپس ss.str();
}

CV_EXPORTS_W String dump_داخلہ_Array_کا_Arrays(داخلہ_Array_کا_Arrays argument)
{
    اگر (&argument == &noArray())
        واپس "InputArrayOfArrays: noArray()";
    std::ostringstream ss;
    ss << "InputArrayOfArrays:";
    کوشش {
        کرو {
            ss << (argument.empty() ? " empty()=true" : " empty()=false");
            ss << cv::format(" kind=0x%08llx", (طویل طویل صحیح)argument.قسم());
            ss << cv::format(" flags=0x%08llx", (طویل طویل صحیح)argument.getFlags());
            اگر (argument.getObj() == تہی)
            {
                ss << " obj=NULL";
                توڑو; //مکمل
            }
            ss << cv::format(" total(-1)=%lld", (طویل طویل صحیح)argument.کل(-1));
            ss << cv::format(" dims(-1)=%d", argument.dims(-1));
            سائز size = argument.size(-1);
            ss << cv::format(" size(-1)=%dx%d", size.چوڑائی, size.اونچائی);
            اگر (argument.کل(-1) > 0)
            {
                ss << " type(0)=" << cv::type_مقصد_String(argument.type(0));
                صحیح dims = argument.dims(0);
                ss << cv::format(" dims(0)=%d", dims);
                اگر (dims <= 2)
                {
                    سائز size0 = argument.size(0);
                    ss << cv::format(" size(0)=%dx%d", size0.چوڑائی, size0.اونچائی);
                }
                ورنہ
                {
                    صحیح sz[CV_MAX_DIM] = {0};
                    argument.sizend(sz, 0);
                    ss << " size(0)=[";
                    برائے (صحیح i = 0; i < dims; i++)
                    {
                        اگر (i > 0)
                            ss << ' ';
                        ss << sz[i];
                    }
                    ss << "]";
                }
            }
        } جب (0);
    }
    پکڑو (ثابت std::exception& e)
    {
        ss << " ERROR: exception occurred: " << e.what();
    }
    پکڑو (...)
    {
        ss << " ERROR: unknown exception occurred, dump is non-complete";
    }
    واپس ss.str();
}

CV_EXPORTS_W String dump_داخلہ_Output_Array(داخلہ_Output_Array argument)
{
    اگر (&argument == &noArray())
        واپس "InputOutputArray: noArray()";
    std::ostringstream ss;
    ss << "InputOutputArray:";
    کوشش {
        کرو {
            ss << (argument.empty() ? " empty()=true" : " empty()=false");
            ss << cv::format(" kind=0x%08llx", (طویل طویل صحیح)argument.قسم());
            ss << cv::format(" flags=0x%08llx", (طویل طویل صحیح)argument.getFlags());
            اگر (argument.getObj() == تہی)
            {
                ss << " obj=NULL";
                توڑو; //مکمل
            }
            ss << cv::format(" total(-1)=%lld", (طویل طویل صحیح)argument.کل(-1));
            صحیح dims = argument.dims(-1);
            ss << cv::format(" dims(-1)=%d", dims);
            اگر (dims <= 2)
            {
                سائز size = argument.size(-1);
                ss << cv::format(" size(-1)=%dx%d", size.چوڑائی, size.اونچائی);
            }
            ورنہ
            {
                صحیح sz[CV_MAX_DIM] = {0};
                argument.sizend(sz, -1);
                ss << " size(-1)=[";
                برائے (صحیح i = 0; i < dims; i++)
                {
                    اگر (i > 0)
                        ss << ' ';
                    ss << sz[i];
                }
                ss << "]";
            }
            ss << " type(-1)=" << cv::type_مقصد_String(argument.type(-1));
        } جب (0);
    }
    پکڑو (ثابت std::exception& e)
    {
        ss << " ERROR: exception occurred: " << e.what();
    }
    پکڑو (...)
    {
        ss << " ERROR: unknown exception occurred, dump is non-complete";
    }
    واپس ss.str();
}

CV_EXPORTS_W String dump_داخلہ_Output_Array_کا_Arrays(داخلہ_Output_Array_کا_Arrays argument)
{
    اگر (&argument == &noArray())
        واپس "InputOutputArrayOfArrays: noArray()";
    std::ostringstream ss;
    ss << "InputOutputArrayOfArrays:";
    کوشش {
        کرو {
            ss << (argument.empty() ? " empty()=true" : " empty()=false");
            ss << cv::format(" kind=0x%08llx", (طویل طویل صحیح)argument.قسم());
            ss << cv::format(" flags=0x%08llx", (طویل طویل صحیح)argument.getFlags());
            اگر (argument.getObj() == تہی)
            {
                ss << " obj=NULL";
                توڑو; //مکمل
            }
            ss << cv::format(" total(-1)=%lld", (طویل طویل صحیح)argument.کل(-1));
            ss << cv::format(" dims(-1)=%d", argument.dims(-1));
            سائز size = argument.size(-1);
            ss << cv::format(" size(-1)=%dx%d", size.چوڑائی, size.اونچائی);
            اگر (argument.کل(-1) > 0)
            {
                ss << " type(0)=" << cv::type_مقصد_String(argument.type(0));
                صحیح dims = argument.dims(0);
                ss << cv::format(" dims(0)=%d", dims);
                اگر (dims <= 2)
                {
                    سائز size0 = argument.size(0);
                    ss << cv::format(" size(0)=%dx%d", size0.چوڑائی, size0.اونچائی);
                }
                ورنہ
                {
                    صحیح sz[CV_MAX_DIM] = {0};
                    argument.sizend(sz, 0);
                    ss << " size(0)=[";
                    برائے (صحیح i = 0; i < dims; i++)
                    {
                        اگر (i > 0)
                            ss << ' ';
                        ss << sz[i];
                    }
                    ss << "]";
                }
            }
        } جب (0);
    }
    پکڑو (ثابت std::exception& e)
    {
        ss << " ERROR: exception occurred: " << e.what();
    }
    پکڑو (...)
    {
        ss << " ERROR: unknown exception occurred, dump is non-complete";
    }
    واپس ss.str();
}

سانچہ <جماعت T, جماعت Formatter>
ساکن اندرونی String dump_سمتیہ(ثابت std::vector<T>& vec, Formatter format)
{
    std::ostringstream oss("[", std::ios::ate);
    اگر (!vec.empty())
    {
        format(oss) << vec[0];
        برائے (std::size_t i = 1; i < vec.size(); ++i)
        {
            oss << ", ";
            format(oss) << vec[i];
        }
    }
    oss << "]";
    واپس oss.str();
}

ساکن اندرونی std::ostream& noFormat(std::ostream& os)
{
    واپس os;
}

ساکن اندرونی std::ostream& floatFormat(std::ostream& os)
{
    واپس os << std::fixed << std::setprecision(2);
}

String dump_سمتیہ_کا_Int(ثابت std::vector<صحیح>& vec)
{
    واپس dump_سمتیہ(vec, &noFormat);
}

String dump_سمتیہ_کا_Double(ثابت std::vector<دوہرا>& vec)
{
    واپس dump_سمتیہ(vec, &floatFormat);
}

String dump_سمتیہ_کا_Rect(ثابت std::vector<Rect>& vec)
{
    واپس dump_سمتیہ(vec, &noFormat);
}


نامخلا fs {
cv::String getCacheDirectoryForDownloads()
{
#اگر OPENCV_HAVE_FILESYSTEM_SUPPORT
    واپس cv::utils::fs::getCacheDirectory("downloads", "OPENCV_DOWNLOADS_CACHE_DIR");
#ورنہ
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#اگر_ختم
}
} // namespace fs

}} // namespace
