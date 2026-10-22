// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ضمن "precomp.hpp"
#ضمن "opencv2/core/bindings_utils.hpp"
#ضمن <sstream>
#ضمن <iomanip>
#ضمن <opencv2/core/utils/filesystem.hpp>
#ضمن <opencv2/core/utils/filesystem.private.hpp>

نطاق cv {
ساكن مضمن std::ostream& چلوونکی<<(std::ostream& os, ثابت Rect& rect)
{
    أرجع os << "[x=" << rect.x << ", y=" << rect.y << ", w=" << rect.پلنوالی << ", h=" << rect.لوړوالی << ']';
}
نطاق utils {

String dump_ننوت_Array(ننوت_Array argument)
{
    إذا (&argument == &noArray())
        أرجع "InputArray: noArray()";
    std::ostringstream ss;
    ss << "InputArray:";
    حاول {
        افعل {
            ss << (argument.empty() ? " empty()=true" : " empty()=false");
            ss << cv::format(" kind=0x%08llx", (اوږد اوږد صحيح)argument.ډول());
            ss << cv::format(" flags=0x%08llx", (اوږد اوږد صحيح)argument.getFlags());
            إذا (argument.getObj() == عدم)
            {
                ss << " obj=NULL";
                اكسر; //بشپړ
            }
            ss << cv::format(" total(-1)=%lld", (اوږد اوږد صحيح)argument.ټول(-1));
            صحيح dims = argument.dims(-1);
            ss << cv::format(" dims(-1)=%d", dims);
            إذا (dims <= 2)
            {
                اندازه size = argument.size(-1);
                ss << cv::format(" size(-1)=%dx%d", size.پلنوالی, size.لوړوالی);
            }
            وإلا
            {
                صحيح sz[CV_MAX_DIM] = {0};
                argument.sizend(sz, -1);
                ss << " size(-1)=[";
                لكل (صحيح i = 0; i < dims; i++)
                {
                    إذا (i > 0)
                        ss << ' ';
                    ss << sz[i];
                }
                ss << "]";
            }
            ss << " type(-1)=" << cv::type_منزل_String(argument.type(-1));
        } طالما (0);
    }
    امسك (ثابت std::exception& e)
    {
        ss << " ERROR: exception occurred: " << e.what();
    }
    امسك (...)
    {
        ss << " ERROR: unknown exception occurred, dump is non-complete";
    }
    أرجع ss.str();
}

CV_EXPORTS_W String dump_ننوت_Array_د_Arrays(ننوت_Array_د_Arrays argument)
{
    إذا (&argument == &noArray())
        أرجع "InputArrayOfArrays: noArray()";
    std::ostringstream ss;
    ss << "InputArrayOfArrays:";
    حاول {
        افعل {
            ss << (argument.empty() ? " empty()=true" : " empty()=false");
            ss << cv::format(" kind=0x%08llx", (اوږد اوږد صحيح)argument.ډول());
            ss << cv::format(" flags=0x%08llx", (اوږد اوږد صحيح)argument.getFlags());
            إذا (argument.getObj() == عدم)
            {
                ss << " obj=NULL";
                اكسر; //بشپړ
            }
            ss << cv::format(" total(-1)=%lld", (اوږد اوږد صحيح)argument.ټول(-1));
            ss << cv::format(" dims(-1)=%d", argument.dims(-1));
            اندازه size = argument.size(-1);
            ss << cv::format(" size(-1)=%dx%d", size.پلنوالی, size.لوړوالی);
            إذا (argument.ټول(-1) > 0)
            {
                ss << " type(0)=" << cv::type_منزل_String(argument.type(0));
                صحيح dims = argument.dims(0);
                ss << cv::format(" dims(0)=%d", dims);
                إذا (dims <= 2)
                {
                    اندازه size0 = argument.size(0);
                    ss << cv::format(" size(0)=%dx%d", size0.پلنوالی, size0.لوړوالی);
                }
                وإلا
                {
                    صحيح sz[CV_MAX_DIM] = {0};
                    argument.sizend(sz, 0);
                    ss << " size(0)=[";
                    لكل (صحيح i = 0; i < dims; i++)
                    {
                        إذا (i > 0)
                            ss << ' ';
                        ss << sz[i];
                    }
                    ss << "]";
                }
            }
        } طالما (0);
    }
    امسك (ثابت std::exception& e)
    {
        ss << " ERROR: exception occurred: " << e.what();
    }
    امسك (...)
    {
        ss << " ERROR: unknown exception occurred, dump is non-complete";
    }
    أرجع ss.str();
}

CV_EXPORTS_W String dump_ننوت_Output_Array(ننوت_Output_Array argument)
{
    إذا (&argument == &noArray())
        أرجع "InputOutputArray: noArray()";
    std::ostringstream ss;
    ss << "InputOutputArray:";
    حاول {
        افعل {
            ss << (argument.empty() ? " empty()=true" : " empty()=false");
            ss << cv::format(" kind=0x%08llx", (اوږد اوږد صحيح)argument.ډول());
            ss << cv::format(" flags=0x%08llx", (اوږد اوږد صحيح)argument.getFlags());
            إذا (argument.getObj() == عدم)
            {
                ss << " obj=NULL";
                اكسر; //بشپړ
            }
            ss << cv::format(" total(-1)=%lld", (اوږد اوږد صحيح)argument.ټول(-1));
            صحيح dims = argument.dims(-1);
            ss << cv::format(" dims(-1)=%d", dims);
            إذا (dims <= 2)
            {
                اندازه size = argument.size(-1);
                ss << cv::format(" size(-1)=%dx%d", size.پلنوالی, size.لوړوالی);
            }
            وإلا
            {
                صحيح sz[CV_MAX_DIM] = {0};
                argument.sizend(sz, -1);
                ss << " size(-1)=[";
                لكل (صحيح i = 0; i < dims; i++)
                {
                    إذا (i > 0)
                        ss << ' ';
                    ss << sz[i];
                }
                ss << "]";
            }
            ss << " type(-1)=" << cv::type_منزل_String(argument.type(-1));
        } طالما (0);
    }
    امسك (ثابت std::exception& e)
    {
        ss << " ERROR: exception occurred: " << e.what();
    }
    امسك (...)
    {
        ss << " ERROR: unknown exception occurred, dump is non-complete";
    }
    أرجع ss.str();
}

CV_EXPORTS_W String dump_ننوت_Output_Array_د_Arrays(ننوت_Output_Array_د_Arrays argument)
{
    إذا (&argument == &noArray())
        أرجع "InputOutputArrayOfArrays: noArray()";
    std::ostringstream ss;
    ss << "InputOutputArrayOfArrays:";
    حاول {
        افعل {
            ss << (argument.empty() ? " empty()=true" : " empty()=false");
            ss << cv::format(" kind=0x%08llx", (اوږد اوږد صحيح)argument.ډول());
            ss << cv::format(" flags=0x%08llx", (اوږد اوږد صحيح)argument.getFlags());
            إذا (argument.getObj() == عدم)
            {
                ss << " obj=NULL";
                اكسر; //بشپړ
            }
            ss << cv::format(" total(-1)=%lld", (اوږد اوږد صحيح)argument.ټول(-1));
            ss << cv::format(" dims(-1)=%d", argument.dims(-1));
            اندازه size = argument.size(-1);
            ss << cv::format(" size(-1)=%dx%d", size.پلنوالی, size.لوړوالی);
            إذا (argument.ټول(-1) > 0)
            {
                ss << " type(0)=" << cv::type_منزل_String(argument.type(0));
                صحيح dims = argument.dims(0);
                ss << cv::format(" dims(0)=%d", dims);
                إذا (dims <= 2)
                {
                    اندازه size0 = argument.size(0);
                    ss << cv::format(" size(0)=%dx%d", size0.پلنوالی, size0.لوړوالی);
                }
                وإلا
                {
                    صحيح sz[CV_MAX_DIM] = {0};
                    argument.sizend(sz, 0);
                    ss << " size(0)=[";
                    لكل (صحيح i = 0; i < dims; i++)
                    {
                        إذا (i > 0)
                            ss << ' ';
                        ss << sz[i];
                    }
                    ss << "]";
                }
            }
        } طالما (0);
    }
    امسك (ثابت std::exception& e)
    {
        ss << " ERROR: exception occurred: " << e.what();
    }
    امسك (...)
    {
        ss << " ERROR: unknown exception occurred, dump is non-complete";
    }
    أرجع ss.str();
}

کينډۍ <صنف T, صنف Formatter>
ساكن مضمن String dump_ویکټور(ثابت std::vector<T>& vec, Formatter format)
{
    std::ostringstream oss("[", std::ios::ate);
    إذا (!vec.empty())
    {
        format(oss) << vec[0];
        لكل (std::size_t i = 1; i < vec.size(); ++i)
        {
            oss << ", ";
            format(oss) << vec[i];
        }
    }
    oss << "]";
    أرجع oss.str();
}

ساكن مضمن std::ostream& noFormat(std::ostream& os)
{
    أرجع os;
}

ساكن مضمن std::ostream& floatFormat(std::ostream& os)
{
    أرجع os << std::fixed << std::setprecision(2);
}

String dump_ویکټور_د_Int(ثابت std::vector<صحيح>& vec)
{
    أرجع dump_ویکټور(vec, &noFormat);
}

String dump_ویکټور_د_Double(ثابت std::vector<مزدوج>& vec)
{
    أرجع dump_ویکټور(vec, &floatFormat);
}

String dump_ویکټور_د_Rect(ثابت std::vector<Rect>& vec)
{
    أرجع dump_ویکټور(vec, &noFormat);
}


نطاق fs {
cv::String getCacheDirectoryForDownloads()
{
#إذا OPENCV_HAVE_FILESYSTEM_SUPPORT
    أرجع cv::utils::fs::getCacheDirectory("downloads", "OPENCV_DOWNLOADS_CACHE_DIR");
#وإلا
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#د_که_پای
}
} // namespace fs

}} // namespace
