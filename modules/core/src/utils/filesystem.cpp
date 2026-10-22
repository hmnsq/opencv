// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ضمن "../precomp.hpp"

#ضمن <opencv2/core/utils/configuration.private.hpp>

#ضمن <opencv2/core/utils/logger.hpp>

#ضمن "opencv2/core/utils/filesystem.private.hpp"
#ضمن "opencv2/core/utils/filesystem.hpp"

//#define DEBUG_FS_UTILS

#جے_تعریف_شدہ DEBUG_FS_UTILS
#ضمن <iostream>
#تعریف_کرو DBG(...) __VA_ARGS__
#وإلا
#تعریف_کرو DBG(...)
#جے_انت


#إذا OPENCV_HAVE_FILESYSTEM_SUPPORT

#جے_تعریف_شدہ _WIN32
#تعریف_کرو WIN32_LEAN_AND_اوسط
#تعریف_رد NOMINMAX
#تعریف_کرو NOMINMAX
#ضمن <windows.h>
#ضمن <direct.h>
#ضمن <sys/types.h>
#ضمن <sys/stat.h>
#ضمن <errno.h>
#ضمن <io.h>
#ضمن <stdio.h>
#وإلاإذا defined __linux__ || defined __APPLE__ || defined __HAIKU__ || defined __FreeBSD__ || defined __GNU__ || defined __EMSCRIPTEN__ || defined __QNX__ || defined __CYGWIN__
#ضمن <sys/types.h>
#ضمن <sys/stat.h>
#ضمن <fcntl.h>
#ضمن <unistd.h>
#ضمن <errno.h>
#جے_انت

#جے_انت // OPENCV_HAVE_FILESYSTEM_SUPPORT

نطاق cv { نطاق utils { نطاق fs {

#جے_تعریف_شدہ _WIN32
ساكن ثابت حرف native_separator = '\\';
#وإلا
ساكن ثابت حرف native_separator = '/';
#جے_انت

ساكن مضمن
منطقي is_رستہ_Separator(حرف c)
{
    أرجع c == '/' || c == '\\';
}

cv::String join(ثابت cv::String& ادھار, ثابت cv::String& رستہ)
{
    إذا (ادھار.empty())
        أرجع رستہ;
    إذا (رستہ.empty())
        أرجع ادھار;

    منطقي ادھار_Sep = is_رستہ_Separator(ادھار[ادھار.size() - 1]);
    منطقي رستہ_Sep = is_رستہ_Separator(رستہ[0]);
    String نتیجہ;
    إذا (ادھار_Sep && رستہ_Sep)
    {
        نتیجہ = ادھار + رستہ.substr(1);
    }
    وإلا إذا (!ادھار_Sep && !رستہ_Sep)
    {
        نتیجہ = ادھار + native_separator + رستہ;
    }
    وإلا
    {
        نتیجہ = ادھار + رستہ;
    }
    أرجع نتیجہ;
}

CV_EXPORTS cv::String get_ماپا(ثابت cv::String &رستہ)
{
    std::string::size_type loc = رستہ.find_last_of("/\\");
    إذا (loc == std::string::npos)
        أرجع std::string();
    أرجع std::string(رستہ, 0, loc);
}

CV_EXPORTS std::wstring get_ماپا(ثابت std::wstring& رستہ)
{
    std::wstring::size_type loc = رستہ.find_last_of(L"/\\");
    إذا (loc == std::wstring::npos)
        أرجع std::wstring();
    أرجع std::wstring(رستہ, 0, loc);
}

#إذا OPENCV_HAVE_FILESYSTEM_SUPPORT

cv::String canonical(ثابت cv::String& رستہ)
{
    cv::String نتیجہ;
#جے_تعریف_شدہ _WIN32
    ثابت حرف* نتیجہ_str = _fullpath(عدم, رستہ.c_str(), 0);
#وإلا
    ثابت حرف* نتیجہ_str = realpath(رستہ.c_str(), عدم);
#جے_انت
    إذا (نتیجہ_str)
    {
        نتیجہ = cv::String(نتیجہ_str);
        حرر((فراغ*)نتیجہ_str);
    }
    أرجع نتیجہ.empty() ? رستہ : نتیجہ;
}


منطقي exists(ثابت cv::String& رستہ)
{
    CV_INSTRUMENT_REGION();

#إذا defined _WIN32 || defined WINCE
    BOOL حالت = TRUE;
    {
        WIN32_FILE_ATTRIBUTE_انکڑے all_attrs;
#جے_تعریف_شدہ WINRT
        wchar_t wpath[MAX_رستہ];
        size_t copied = mbstowcs(wpath, رستہ.c_str(), MAX_رستہ);
        CV_Assert((copied != MAX_رستہ) && (copied != (size_t)-1));
        حالت = ::Get_File_گن_Ex_W(wpath, GetFileExInfoStandard, &all_attrs);
#وإلا
        حالت = ::Get_File_گن_Ex_A(رستہ.c_str(), GetFileExInfoStandard, &all_attrs);
#جے_انت
    }

    أرجع !!حالت;
#وإلا
    بنية stat stat_buf;
    أرجع (0 == stat(رستہ.c_str(), &stat_buf));
#جے_انت
}

CV_EXPORTS فراغ remove_all(ثابت cv::String& رستہ)
{
    إذا (!exists(رستہ))
        أرجع;
    إذا (isDirectory(رستہ))
    {
        std::vector<String> entries;
        utils::fs::glob(رستہ, cv::String(), entries, خطأ, سچ);
        لكل (size_t i = 0; i < entries.size(); i++)
        {
            ثابت String& e = entries[i];
            remove_all(e);
        }
#جے_تعریف_شدہ _MSC_VER
        منطقي نتیجہ = _rmdir(رستہ.c_str()) == 0;
#وإلا
        منطقي نتیجہ = rmdir(رستہ.c_str()) == 0;
#جے_انت
        إذا (!نتیجہ)
        {
            CV_LOG_ERROR(عدم, "Can't remove directory: " << رستہ);
        }
    }
    وإلا
    {
#جے_تعریف_شدہ _MSC_VER
        منطقي نتیجہ = _unlink(رستہ.c_str()) == 0;
#وإلا
        منطقي نتیجہ = unlink(رستہ.c_str()) == 0;
#جے_انت
        إذا (!نتیجہ)
        {
            CV_LOG_ERROR(عدم, "Can't remove file: " << رستہ);
        }
    }
}


cv::String getcwd()
{
    CV_INSTRUMENT_REGION();
    cv::Auto_بفر<حرف, 4096> buf;
#إذا defined WIN32 || defined _WIN32 || defined WINCE
#جے_تعریف_شدہ WINRT
    أرجع cv::String();
#وإلا
    DWORD sz = GetCurrentDirectoryA(0, عدم);
    buf.allocate((size_t)sz);
    sz = GetCurrentDirectoryA((DWORD)buf.size(), buf.انکڑے());
    أرجع cv::String(buf.انکڑے(), (size_t)sz);
#جے_انت
#وإلاإذا defined __linux__ || defined __APPLE__ || defined __HAIKU__ || defined __FreeBSD__ || defined __EMSCRIPTEN__ || defined __QNX__ || defined __CYGWIN__
    لكل(;;)
    {
        حرف* p = ::getcwd(buf.انکڑے(), buf.size());
        إذا (p == عدم)
        {
            إذا (errno == ERANGE)
            {
                buf.allocate(buf.size() * 2);
                استمر;
            }
            أرجع cv::String();
        }
        اكسر;
    }
    أرجع cv::String(buf.انکڑے(), (size_t)لڑی_لمبائی(buf.انکڑے()));
#وإلا
    أرجع cv::String();
#جے_انت
}


منطقي createDirectory(ثابت cv::String& رستہ)
{
    CV_INSTRUMENT_REGION();
#إذا defined WIN32 || defined _WIN32 || defined WINCE
#جے_تعریف_شدہ WINRT
    wchar_t wpath[MAX_رستہ];
    size_t copied = mbstowcs(wpath, رستہ.c_str(), MAX_رستہ);
    CV_Assert((copied != MAX_رستہ) && (copied != (size_t)-1));
    صحيح نتیجہ = CreateDirectoryA(wpath, عدم) ? 0 : -1;
#وإلا
    صحيح نتیجہ = _mkdir(رستہ.c_str());
#جے_انت
#وإلاإذا defined __linux__ || defined __APPLE__ || defined __HAIKU__ || defined __FreeBSD__ || defined __EMSCRIPTEN__ || defined __QNX__ || defined __CYGWIN__
    صحيح نتیجہ = mkdir(رستہ.c_str(), 0777);
#وإلا
    صحيح نتیجہ = -1;
#جے_انت

    إذا (نتیجہ == -1)
    {
        أرجع isDirectory(رستہ);
    }
    أرجع سچ;
}

منطقي createDirectories(ثابت cv::String& رستہ_3)
{
    cv::String رستہ = رستہ_3;
    لكل (;;)
    {
        حرف آخری_char = رستہ.empty() ? 0 : رستہ[رستہ.length() - 1];
        إذا (is_رستہ_Separator(آخری_char))
        {
            رستہ = رستہ.substr(0, رستہ.length() - 1);
            استمر;
        }
        اكسر;
    }

    إذا (رستہ.empty() || رستہ == "./" || رستہ == ".\\" || رستہ == ".")
        أرجع سچ;
    إذا (isDirectory(رستہ))
        أرجع سچ;

    size_t pos = رستہ.rfind('/');
    إذا (pos == cv::String::npos)
        pos = رستہ.rfind('\\');
    إذا (pos != cv::String::npos)
    {
        cv::String ماپا_directory = رستہ.substr(0, pos);
        إذا (!ماپا_directory.empty())
        {
            إذا (!createDirectories(ماپا_directory))
                أرجع خطأ;
        }
    }

    أرجع createDirectory(رستہ);
}

#جے_تعریف_شدہ _WIN32

بنية File_جندرا::Impl
{
    Impl(ثابت حرف* fname)
    {
        // http://support.microsoft.com/kb/316609
        صحيح عدد_Retries = 5;
        افعل
        {
            handle = ::CreateFileA(fname, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, عدم,
                                OPEN_EXISTING, FILE_ATTRIBUTE_عام, عدم);
            إذا (INVALID_HANDLE_قدر == handle)
            {
                إذا (ERROR_SHARING_VIOLATION == Get_آخری_Error())
                {
                    عدد_Retries--;
                    Sleep(250);
                    استمر;
                }
                وإلا
                {
                    CV_Error_(Error::StsAssert, ("Can't open lock file: %s", fname));
                }
            }
            اكسر;
        } طالما (عدد_Retries > 0);
    }
    ~Impl()
    {
        إذا (INVALID_HANDLE_قدر != handle)
        {
            ::CloseHandle(handle);
        }
    }

    منطقي lock()
    {
        OVERLAPPED overlapped;
        std::memset(&overlapped, 0, حجم(overlapped));
        أرجع !!::جندرا_File_Ex(handle, LOCKFILE_EXCLUSIVE_جندرا, 0, MAXDWORD, MAXDWORD, &overlapped);
    }
    منطقي unlock()
    {
        OVERLAPPED overlapped;
        std::memset(&overlapped, 0, حجم(overlapped));
        أرجع !!::UnlockFileEx(handle, 0, MAXDWORD, MAXDWORD, &overlapped);
    }

    منطقي جندرا_shared()
    {
        OVERLAPPED overlapped;
        std::memset(&overlapped, 0, حجم(overlapped));
        أرجع !!::جندرا_File_Ex(handle, 0, 0, MAXDWORD, MAXDWORD, &overlapped);
    }
    منطقي unlock_shared()
    {
        أرجع unlock();
    }

    HANDLE handle;

خاص:
    Impl(ثابت Impl&); // disabled
    Impl& عامل=(ثابت Impl&); // disabled
};

#وإلاإذا defined __linux__ || defined __APPLE__ || defined __HAIKU__ || defined __FreeBSD__ || defined __GNU__ || defined __EMSCRIPTEN__ || defined __QNX__ || defined __CYGWIN__

بنية File_جندرا::Impl
{
    Impl(ثابت حرف* fname)
    {
        handle = ::open(fname, O_RDWR);
        CV_Assert(handle != -1);
    }
    ~Impl()
    {
        إذا (handle >= 0)
            ::close(handle);
    }

    منطقي lock()
    {
        بنية ::flock l;
        std::memset(&l, 0, حجم(l));
        l.l_type = F_WRLCK;
        l.l_whence = SEEK_SET;
        l.l_شروع = 0;
        l.l_len = 0;
        DBG(std::باہر << "Lock..." << std::سطر_انت);
        منطقي res = -1 != ::fcntl(handle, F_SETLKW, &l);
        أرجع res;
    }
    منطقي unlock()
    {
        بنية ::flock l;
        std::memset(&l, 0, حجم(l));
        l.l_type = F_UNLCK;
        l.l_whence = SEEK_SET;
        l.l_شروع = 0;
        l.l_len = 0;
        DBG(std::باہر << "Unlock..." << std::سطر_انت);
        منطقي res = -1 != ::fcntl(handle, F_SETLK, &l);
        أرجع res;
    }

    منطقي جندرا_shared()
    {
        بنية ::flock l;
        std::memset(&l, 0, حجم(l));
        l.l_type = F_RDLCK;
        l.l_whence = SEEK_SET;
        l.l_شروع = 0;
        l.l_len = 0;
        DBG(std::باہر << "Lock read..." << std::سطر_انت);
        منطقي res = -1 != ::fcntl(handle, F_SETLKW, &l);
        أرجع res;
    }
    منطقي unlock_shared()
    {
        أرجع unlock();
    }

    صحيح handle;

خاص:
    Impl(ثابت Impl&); // disabled
    Impl& عامل=(ثابت Impl&); // disabled
};

#جے_انت

File_جندرا::File_جندرا(ثابت حرف* fname)
    : pImpl(جديد Impl(fname))
{
    // nothing
}
File_جندرا::~File_جندرا()
{
    مٹاؤ pImpl;
    pImpl = عدم;
}

فراغ File_جندرا::lock() { CV_Assert(pImpl->lock()); }
فراغ File_جندرا::unlock() { CV_Assert(pImpl->unlock()); }
فراغ File_جندرا::جندرا_shared() { CV_Assert(pImpl->جندرا_shared()); }
فراغ File_جندرا::unlock_shared() { CV_Assert(pImpl->unlock_shared()); }



cv::String getCacheDirectory(ثابت حرف* sub_directory_name, ثابت حرف* configuration_name)
{
    String cache_رستہ_2;
    إذا (configuration_name)
    {
        cache_رستہ_2 = utils::getConfigurationParameterString(configuration_name, "");
    }
    إذا (cache_رستہ_2.empty())
    {
        cv::String default_cache_رستہ;
#جے_تعریف_شدہ _WIN32
        wchar_t tmp_رستہ_buf_w[MAX_رستہ+1] = {0};
        DWORD res = Get_عارضی_رستہ_W(MAX_رستہ, tmp_رستہ_buf_w);
        إذا (res > 0 && res <= MAX_رستہ)
        {
            // Convert from UTF-16 to UTF-8 to support Unicode paths
            صحيح len = Wide_Char_منزل_Multi_Byte(CP_UTF8, 0, tmp_رستہ_buf_w, -1, عدم, 0, عدم, عدم);
            إذا (len > 0)
            {
                std::vector<حرف> utf8_buf(len);
                Wide_Char_منزل_Multi_Byte(CP_UTF8, 0, tmp_رستہ_buf_w, -1, utf8_buf.انکڑے(), len, عدم, عدم);
                default_cache_رستہ = std::string(utf8_buf.انکڑے());
            }
        }
#وإلاإذا defined __ANDROID__
        // no defaults
#وإلاإذا defined __APPLE__
        ثابت std::string tmpdir_env = utils::getConfigurationParameterString("TMPDIR");
        إذا (!tmpdir_env.empty() && utils::fs::isDirectory(tmpdir_env))
        {
            default_cache_رستہ = tmpdir_env;
        }
        وإلا
        {
            default_cache_رستہ = "/tmp/";
            CV_LOG_WARNING(عدم, "Using world accessible cache directory. This may be not secure: " << default_cache_رستہ);
        }
#وإلاإذا defined __linux__ || defined __HAIKU__ || defined __FreeBSD__ || defined __GNU__
        // https://specifications.freedesktop.org/basedir-spec/basedir-spec-latest.html
        إذا (default_cache_رستہ.empty())
        {
            ثابت std::string xdg_cache_env = utils::getConfigurationParameterString("XDG_CACHE_HOME");
            إذا (!xdg_cache_env.empty() && utils::fs::isDirectory(xdg_cache_env))
            {
                default_cache_رستہ = xdg_cache_env;
            }
        }
        إذا (default_cache_رستہ.empty())
        {
            ثابت std::string home_env = utils::getConfigurationParameterString("HOME");
            إذا (!home_env.empty() && utils::fs::isDirectory(home_env))
            {
                cv::String home_رستہ = home_env;
                cv::String home_cache_رستہ = utils::fs::join(home_رستہ, ".cache/");
                إذا (utils::fs::isDirectory(home_cache_رستہ))
                {
                    default_cache_رستہ = home_cache_رستہ;
                }
            }
        }
        إذا (default_cache_رستہ.empty())
        {
            ثابت حرف* عارضی_رستہ = "/var/tmp/";
            إذا (utils::fs::isDirectory(عارضی_رستہ))
            {
                default_cache_رستہ = عارضی_رستہ;
                CV_LOG_WARNING(عدم, "Using world accessible cache directory. This may be not secure: " << default_cache_رستہ);
            }
        }
        إذا (default_cache_رستہ.empty())
        {
            default_cache_رستہ = "/tmp/";
            CV_LOG_WARNING(عدم, "Using world accessible cache directory. This may be not secure: " << default_cache_رستہ);
        }
#وإلا
        // no defaults
#جے_انت
        CV_LOG_VERBOSE(عدم, 0, "default_cache_path = " << default_cache_رستہ);
        إذا (!default_cache_رستہ.empty())
        {
            إذا (utils::fs::isDirectory(default_cache_رستہ))
            {
                cv::String default_cache_رستہ_ادھار = utils::fs::join(default_cache_رستہ, "opencv");
                default_cache_رستہ = utils::fs::join(default_cache_رستہ_ادھار, CVAUX_STR(CV_نسخہ_MAJOR) ".x" CV_نسخہ_حالت);
                إذا (utils::getConfigurationParameterBool("OPENCV_CACHE_SHOW_CLEANUP_MESSAGE", سچ)
                    && !utils::fs::isDirectory(default_cache_رستہ))
                {
                    std::vector<cv::String> existedCacheDirs;
                    حاول
                    {
                        utils::fs::glob_relative(default_cache_رستہ_ادھار, "*", existedCacheDirs, خطأ, سچ);
                    }
                    امسك (...)
                    {
                        // ignore
                    }
                    إذا (!existedCacheDirs.empty())
                    {
                        CV_LOG_WARNING(عدم, "Creating new OpenCV cache directory: " << default_cache_رستہ);
                        CV_LOG_WARNING(عدم, "There are several neighbour directories, probably created by old OpenCV versions.");
                        CV_LOG_WARNING(عدم, "Feel free to cleanup these unused directories:");
                        لكل (size_t i = 0; i < existedCacheDirs.size(); i++)
                        {
                            CV_LOG_WARNING(عدم, "  - " << existedCacheDirs[i]);
                        }
                        CV_LOG_WARNING(عدم, "Note: This message is showed only once.");
                    }
                }
                إذا (sub_directory_name && sub_directory_name[0] != '\0')
                    default_cache_رستہ = utils::fs::join(default_cache_رستہ, cv::String(sub_directory_name) + native_separator);
                إذا (!utils::fs::createDirectories(default_cache_رستہ))
                {
                    CV_LOG_DEBUG(عدم, "Can't create OpenCV cache sub-directory: " << default_cache_رستہ);
                }
                وإلا
                {
                    cache_رستہ_2 = default_cache_رستہ;
                }
            }
            وإلا
            {
                CV_LOG_INFO(عدم, "Can't find default cache directory (does it exist?): " << default_cache_رستہ);
            }
        }
        وإلا
        {
            CV_LOG_DEBUG(عدم, "OpenCV has no support to discover default cache directory on the current platform");
        }
    }
    وإلا
    {
        إذا (cache_رستہ_2 == "disabled")
            أرجع cache_رستہ_2;
        إذا (!isDirectory(cache_رستہ_2))
        {
            CV_LOG_WARNING(عدم, "Specified non-existed directory, creating OpenCV sub-directory for caching purposes: " << cache_رستہ_2);
            إذا (!createDirectories(cache_رستہ_2))
            {
                CV_LOG_ERROR(عدم, "Can't create OpenCV cache sub-directory: " << cache_رستہ_2);
                cache_رستہ_2.clear();
            }
        }
    }
    CV_Assert(cache_رستہ_2.empty() || utils::fs::isDirectory(cache_رستہ_2));
    إذا (!cache_رستہ_2.empty())
    {
        إذا (!is_رستہ_Separator(cache_رستہ_2[cache_رستہ_2.size() - 1]))
        {
            cache_رستہ_2 += native_separator;
        }
    }
    أرجع cache_رستہ_2;
}

#وإلا
#تعریف_کرو NOT_IMPLEMENTED CV_Error(Error::StsNotImplemented, "");
cv::String canonical(ثابت cv::String& /*رستہ*/) { NOT_IMPLEMENTED }
منطقي exists(ثابت cv::String& /*رستہ*/) { NOT_IMPLEMENTED }
فراغ remove_all(ثابت cv::String& /*رستہ*/) { NOT_IMPLEMENTED }
cv::String getcwd() { NOT_IMPLEMENTED }
منطقي createDirectory(ثابت cv::String& /*رستہ*/) { NOT_IMPLEMENTED }
منطقي createDirectories(ثابت cv::String& /*رستہ*/) { NOT_IMPLEMENTED }
cv::String getCacheDirectory(ثابت حرف* /*sub_directory_name*/, ثابت حرف* /*configuration_name = NULL*/) { NOT_IMPLEMENTED }
#تعریف_رد NOT_IMPLEMENTED
#جے_انت // OPENCV_HAVE_FILESYSTEM_SUPPORT

}}} // namespace


#إذا OPENCV_HAVE_FILESYSTEM_SUPPORT
#ضمن "plugin_loader.impl.hpp"
#جے_انت
