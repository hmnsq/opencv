// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ضمن "../precomp.hpp"

#ضمن <opencv2/core/utils/configuration.private.hpp>

#ضمن <opencv2/core/utils/logger.hpp>

#ضمن "opencv2/core/utils/filesystem.private.hpp"
#ضمن "opencv2/core/utils/filesystem.hpp"

//#define DEBUG_FS_UTILS

#جيڪڏهن_وصف_ٿيل DEBUG_FS_UTILS
#ضمن <iostream>
#وصف_ڪريو DBG(...) __VA_ARGS__
#وإلا
#وصف_ڪريو DBG(...)
#جيڪڏهن_پورو


#إذا OPENCV_HAVE_FILESYSTEM_SUPPORT

#جيڪڏهن_وصف_ٿيل _WIN32
#وصف_ڪريو WIN32_LEAN_AND_سراسري
#وصف_رد NOMINMAX
#وصف_ڪريو NOMINMAX
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
#جيڪڏهن_پورو

#جيڪڏهن_پورو // OPENCV_HAVE_FILESYSTEM_SUPPORT

نطاق cv { نطاق utils { نطاق fs {

#جيڪڏهن_وصف_ٿيل _WIN32
ساكن ثابت حرف native_separator = '\\';
#وإلا
ساكن ثابت حرف native_separator = '/';
#جيڪڏهن_پورو

ساكن مضمن
منطقي is_رستو_Separator(حرف c)
{
    أرجع c == '/' || c == '\\';
}

cv::String join(ثابت cv::String& بنياد, ثابت cv::String& رستو)
{
    إذا (بنياد.empty())
        أرجع رستو;
    إذا (رستو.empty())
        أرجع بنياد;

    منطقي بنياد_Sep = is_رستو_Separator(بنياد[بنياد.size() - 1]);
    منطقي رستو_Sep = is_رستو_Separator(رستو[0]);
    String نتيجو;
    إذا (بنياد_Sep && رستو_Sep)
    {
        نتيجو = بنياد + رستو.substr(1);
    }
    وإلا إذا (!بنياد_Sep && !رستو_Sep)
    {
        نتيجو = بنياد + native_separator + رستو;
    }
    وإلا
    {
        نتيجو = بنياد + رستو;
    }
    أرجع نتيجو;
}

CV_EXPORTS cv::String get_والد(ثابت cv::String &رستو)
{
    std::string::size_type loc = رستو.find_last_of("/\\");
    إذا (loc == std::string::npos)
        أرجع std::string();
    أرجع std::string(رستو, 0, loc);
}

CV_EXPORTS std::wstring get_والد(ثابت std::wstring& رستو)
{
    std::wstring::size_type loc = رستو.find_last_of(L"/\\");
    إذا (loc == std::wstring::npos)
        أرجع std::wstring();
    أرجع std::wstring(رستو, 0, loc);
}

#إذا OPENCV_HAVE_FILESYSTEM_SUPPORT

cv::String canonical(ثابت cv::String& رستو)
{
    cv::String نتيجو;
#جيڪڏهن_وصف_ٿيل _WIN32
    ثابت حرف* نتيجو_str = _fullpath(عدم, رستو.c_str(), 0);
#وإلا
    ثابت حرف* نتيجو_str = realpath(رستو.c_str(), عدم);
#جيڪڏهن_پورو
    إذا (نتيجو_str)
    {
        نتيجو = cv::String(نتيجو_str);
        حرر((فراغ*)نتيجو_str);
    }
    أرجع نتيجو.empty() ? رستو : نتيجو;
}


منطقي exists(ثابت cv::String& رستو)
{
    CV_INSTRUMENT_REGION();

#إذا defined _WIN32 || defined WINCE
    BOOL حالت = TRUE;
    {
        WIN32_FILE_ATTRIBUTE_معلومات all_attrs;
#جيڪڏهن_وصف_ٿيل WINRT
        wchar_t wpath[MAX_رستو];
        size_t copied = mbstowcs(wpath, رستو.c_str(), MAX_رستو);
        CV_Assert((copied != MAX_رستو) && (copied != (size_t)-1));
        حالت = ::Get_File_خاصيتون_Ex_W(wpath, GetFileExInfoStandard, &all_attrs);
#وإلا
        حالت = ::Get_File_خاصيتون_Ex_A(رستو.c_str(), GetFileExInfoStandard, &all_attrs);
#جيڪڏهن_پورو
    }

    أرجع !!حالت;
#وإلا
    بنية stat stat_buf;
    أرجع (0 == stat(رستو.c_str(), &stat_buf));
#جيڪڏهن_پورو
}

CV_EXPORTS فراغ remove_all(ثابت cv::String& رستو)
{
    إذا (!exists(رستو))
        أرجع;
    إذا (isDirectory(رستو))
    {
        std::vector<String> entries;
        utils::fs::glob(رستو, cv::String(), entries, خطأ, سچ);
        لكل (size_t i = 0; i < entries.size(); i++)
        {
            ثابت String& e = entries[i];
            remove_all(e);
        }
#جيڪڏهن_وصف_ٿيل _MSC_VER
        منطقي نتيجو = _rmdir(رستو.c_str()) == 0;
#وإلا
        منطقي نتيجو = rmdir(رستو.c_str()) == 0;
#جيڪڏهن_پورو
        إذا (!نتيجو)
        {
            CV_LOG_ERROR(عدم, "Can't remove directory: " << رستو);
        }
    }
    وإلا
    {
#جيڪڏهن_وصف_ٿيل _MSC_VER
        منطقي نتيجو = _unlink(رستو.c_str()) == 0;
#وإلا
        منطقي نتيجو = unlink(رستو.c_str()) == 0;
#جيڪڏهن_پورو
        إذا (!نتيجو)
        {
            CV_LOG_ERROR(عدم, "Can't remove file: " << رستو);
        }
    }
}


cv::String getcwd()
{
    CV_INSTRUMENT_REGION();
    cv::Auto_بفر<حرف, 4096> buf;
#إذا defined WIN32 || defined _WIN32 || defined WINCE
#جيڪڏهن_وصف_ٿيل WINRT
    أرجع cv::String();
#وإلا
    DWORD sz = GetCurrentDirectoryA(0, عدم);
    buf.allocate((size_t)sz);
    sz = GetCurrentDirectoryA((DWORD)buf.size(), buf.معلومات());
    أرجع cv::String(buf.معلومات(), (size_t)sz);
#جيڪڏهن_پورو
#وإلاإذا defined __linux__ || defined __APPLE__ || defined __HAIKU__ || defined __FreeBSD__ || defined __EMSCRIPTEN__ || defined __QNX__ || defined __CYGWIN__
    لكل(;;)
    {
        حرف* p = ::getcwd(buf.معلومات(), buf.size());
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
    أرجع cv::String(buf.معلومات(), (size_t)لڙي_ڊيگهه(buf.معلومات()));
#وإلا
    أرجع cv::String();
#جيڪڏهن_پورو
}


منطقي createDirectory(ثابت cv::String& رستو)
{
    CV_INSTRUMENT_REGION();
#إذا defined WIN32 || defined _WIN32 || defined WINCE
#جيڪڏهن_وصف_ٿيل WINRT
    wchar_t wpath[MAX_رستو];
    size_t copied = mbstowcs(wpath, رستو.c_str(), MAX_رستو);
    CV_Assert((copied != MAX_رستو) && (copied != (size_t)-1));
    صحيح نتيجو = CreateDirectoryA(wpath, عدم) ? 0 : -1;
#وإلا
    صحيح نتيجو = _mkdir(رستو.c_str());
#جيڪڏهن_پورو
#وإلاإذا defined __linux__ || defined __APPLE__ || defined __HAIKU__ || defined __FreeBSD__ || defined __EMSCRIPTEN__ || defined __QNX__ || defined __CYGWIN__
    صحيح نتيجو = mkdir(رستو.c_str(), 0777);
#وإلا
    صحيح نتيجو = -1;
#جيڪڏهن_پورو

    إذا (نتيجو == -1)
    {
        أرجع isDirectory(رستو);
    }
    أرجع سچ;
}

منطقي createDirectories(ثابت cv::String& رستو_3)
{
    cv::String رستو = رستو_3;
    لكل (;;)
    {
        حرف آخري_char = رستو.empty() ? 0 : رستو[رستو.length() - 1];
        إذا (is_رستو_Separator(آخري_char))
        {
            رستو = رستو.substr(0, رستو.length() - 1);
            استمر;
        }
        اكسر;
    }

    إذا (رستو.empty() || رستو == "./" || رستو == ".\\" || رستو == ".")
        أرجع سچ;
    إذا (isDirectory(رستو))
        أرجع سچ;

    size_t pos = رستو.rfind('/');
    إذا (pos == cv::String::npos)
        pos = رستو.rfind('\\');
    إذا (pos != cv::String::npos)
    {
        cv::String والد_directory = رستو.substr(0, pos);
        إذا (!والد_directory.empty())
        {
            إذا (!createDirectories(والد_directory))
                أرجع خطأ;
        }
    }

    أرجع createDirectory(رستو);
}

#جيڪڏهن_وصف_ٿيل _WIN32

بنية File_تالو::Impl
{
    Impl(ثابت حرف* fname)
    {
        // http://support.microsoft.com/kb/316609
        صحيح انگ_Retries = 5;
        افعل
        {
            handle = ::CreateFileA(fname, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, عدم,
                                OPEN_EXISTING, FILE_ATTRIBUTE_عام, عدم);
            إذا (INVALID_HANDLE_قدر == handle)
            {
                إذا (ERROR_SHARING_VIOLATION == Get_آخري_Error())
                {
                    انگ_Retries--;
                    Sleep(250);
                    استمر;
                }
                وإلا
                {
                    CV_Error_(Error::StsAssert, ("Can't open lock file: %s", fname));
                }
            }
            اكسر;
        } طالما (انگ_Retries > 0);
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
        أرجع !!::تالو_File_Ex(handle, LOCKFILE_EXCLUSIVE_تالو, 0, MAXDWORD, MAXDWORD, &overlapped);
    }
    منطقي unlock()
    {
        OVERLAPPED overlapped;
        std::memset(&overlapped, 0, حجم(overlapped));
        أرجع !!::UnlockFileEx(handle, 0, MAXDWORD, MAXDWORD, &overlapped);
    }

    منطقي تالو_shared()
    {
        OVERLAPPED overlapped;
        std::memset(&overlapped, 0, حجم(overlapped));
        أرجع !!::تالو_File_Ex(handle, 0, 0, MAXDWORD, MAXDWORD, &overlapped);
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

بنية File_تالو::Impl
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
        l.l_شروعات = 0;
        l.l_len = 0;
        DBG(std::ٻاهر << "Lock..." << std::سٽ_پڄاڻي);
        منطقي res = -1 != ::fcntl(handle, F_SETLKW, &l);
        أرجع res;
    }
    منطقي unlock()
    {
        بنية ::flock l;
        std::memset(&l, 0, حجم(l));
        l.l_type = F_UNLCK;
        l.l_whence = SEEK_SET;
        l.l_شروعات = 0;
        l.l_len = 0;
        DBG(std::ٻاهر << "Unlock..." << std::سٽ_پڄاڻي);
        منطقي res = -1 != ::fcntl(handle, F_SETLK, &l);
        أرجع res;
    }

    منطقي تالو_shared()
    {
        بنية ::flock l;
        std::memset(&l, 0, حجم(l));
        l.l_type = F_RDLCK;
        l.l_whence = SEEK_SET;
        l.l_شروعات = 0;
        l.l_len = 0;
        DBG(std::ٻاهر << "Lock read..." << std::سٽ_پڄاڻي);
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

#جيڪڏهن_پورو

File_تالو::File_تالو(ثابت حرف* fname)
    : pImpl(جديد Impl(fname))
{
    // nothing
}
File_تالو::~File_تالو()
{
    ميٽيو pImpl;
    pImpl = عدم;
}

فراغ File_تالو::lock() { CV_Assert(pImpl->lock()); }
فراغ File_تالو::unlock() { CV_Assert(pImpl->unlock()); }
فراغ File_تالو::تالو_shared() { CV_Assert(pImpl->تالو_shared()); }
فراغ File_تالو::unlock_shared() { CV_Assert(pImpl->unlock_shared()); }



cv::String getCacheDirectory(ثابت حرف* sub_directory_name, ثابت حرف* configuration_name)
{
    String cache_رستو_2;
    إذا (configuration_name)
    {
        cache_رستو_2 = utils::getConfigurationParameterString(configuration_name, "");
    }
    إذا (cache_رستو_2.empty())
    {
        cv::String default_cache_رستو;
#جيڪڏهن_وصف_ٿيل _WIN32
        wchar_t tmp_رستو_buf_w[MAX_رستو+1] = {0};
        DWORD res = Get_عارضي_رستو_W(MAX_رستو, tmp_رستو_buf_w);
        إذا (res > 0 && res <= MAX_رستو)
        {
            // Convert from UTF-16 to UTF-8 to support Unicode paths
            صحيح len = Wide_Char_منزل_Multi_Byte(CP_UTF8, 0, tmp_رستو_buf_w, -1, عدم, 0, عدم, عدم);
            إذا (len > 0)
            {
                std::vector<حرف> utf8_buf(len);
                Wide_Char_منزل_Multi_Byte(CP_UTF8, 0, tmp_رستو_buf_w, -1, utf8_buf.معلومات(), len, عدم, عدم);
                default_cache_رستو = std::string(utf8_buf.معلومات());
            }
        }
#وإلاإذا defined __ANDROID__
        // no defaults
#وإلاإذا defined __APPLE__
        ثابت std::string tmpdir_env = utils::getConfigurationParameterString("TMPDIR");
        إذا (!tmpdir_env.empty() && utils::fs::isDirectory(tmpdir_env))
        {
            default_cache_رستو = tmpdir_env;
        }
        وإلا
        {
            default_cache_رستو = "/tmp/";
            CV_LOG_WARNING(عدم, "Using world accessible cache directory. This may be not secure: " << default_cache_رستو);
        }
#وإلاإذا defined __linux__ || defined __HAIKU__ || defined __FreeBSD__ || defined __GNU__
        // https://specifications.freedesktop.org/basedir-spec/basedir-spec-latest.html
        إذا (default_cache_رستو.empty())
        {
            ثابت std::string xdg_cache_env = utils::getConfigurationParameterString("XDG_CACHE_HOME");
            إذا (!xdg_cache_env.empty() && utils::fs::isDirectory(xdg_cache_env))
            {
                default_cache_رستو = xdg_cache_env;
            }
        }
        إذا (default_cache_رستو.empty())
        {
            ثابت std::string home_env = utils::getConfigurationParameterString("HOME");
            إذا (!home_env.empty() && utils::fs::isDirectory(home_env))
            {
                cv::String home_رستو = home_env;
                cv::String home_cache_رستو = utils::fs::join(home_رستو, ".cache/");
                إذا (utils::fs::isDirectory(home_cache_رستو))
                {
                    default_cache_رستو = home_cache_رستو;
                }
            }
        }
        إذا (default_cache_رستو.empty())
        {
            ثابت حرف* عارضي_رستو = "/var/tmp/";
            إذا (utils::fs::isDirectory(عارضي_رستو))
            {
                default_cache_رستو = عارضي_رستو;
                CV_LOG_WARNING(عدم, "Using world accessible cache directory. This may be not secure: " << default_cache_رستو);
            }
        }
        إذا (default_cache_رستو.empty())
        {
            default_cache_رستو = "/tmp/";
            CV_LOG_WARNING(عدم, "Using world accessible cache directory. This may be not secure: " << default_cache_رستو);
        }
#وإلا
        // no defaults
#جيڪڏهن_پورو
        CV_LOG_VERBOSE(عدم, 0, "default_cache_path = " << default_cache_رستو);
        إذا (!default_cache_رستو.empty())
        {
            إذا (utils::fs::isDirectory(default_cache_رستو))
            {
                cv::String default_cache_رستو_بنياد = utils::fs::join(default_cache_رستو, "opencv");
                default_cache_رستو = utils::fs::join(default_cache_رستو_بنياد, CVAUX_STR(CV_نسخو_MAJOR) ".x" CV_نسخو_حالت);
                إذا (utils::getConfigurationParameterBool("OPENCV_CACHE_SHOW_CLEANUP_MESSAGE", سچ)
                    && !utils::fs::isDirectory(default_cache_رستو))
                {
                    std::vector<cv::String> existedCacheDirs;
                    حاول
                    {
                        utils::fs::glob_relative(default_cache_رستو_بنياد, "*", existedCacheDirs, خطأ, سچ);
                    }
                    امسك (...)
                    {
                        // ignore
                    }
                    إذا (!existedCacheDirs.empty())
                    {
                        CV_LOG_WARNING(عدم, "Creating new OpenCV cache directory: " << default_cache_رستو);
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
                    default_cache_رستو = utils::fs::join(default_cache_رستو, cv::String(sub_directory_name) + native_separator);
                إذا (!utils::fs::createDirectories(default_cache_رستو))
                {
                    CV_LOG_DEBUG(عدم, "Can't create OpenCV cache sub-directory: " << default_cache_رستو);
                }
                وإلا
                {
                    cache_رستو_2 = default_cache_رستو;
                }
            }
            وإلا
            {
                CV_LOG_INFO(عدم, "Can't find default cache directory (does it exist?): " << default_cache_رستو);
            }
        }
        وإلا
        {
            CV_LOG_DEBUG(عدم, "OpenCV has no support to discover default cache directory on the current platform");
        }
    }
    وإلا
    {
        إذا (cache_رستو_2 == "disabled")
            أرجع cache_رستو_2;
        إذا (!isDirectory(cache_رستو_2))
        {
            CV_LOG_WARNING(عدم, "Specified non-existed directory, creating OpenCV sub-directory for caching purposes: " << cache_رستو_2);
            إذا (!createDirectories(cache_رستو_2))
            {
                CV_LOG_ERROR(عدم, "Can't create OpenCV cache sub-directory: " << cache_رستو_2);
                cache_رستو_2.clear();
            }
        }
    }
    CV_Assert(cache_رستو_2.empty() || utils::fs::isDirectory(cache_رستو_2));
    إذا (!cache_رستو_2.empty())
    {
        إذا (!is_رستو_Separator(cache_رستو_2[cache_رستو_2.size() - 1]))
        {
            cache_رستو_2 += native_separator;
        }
    }
    أرجع cache_رستو_2;
}

#وإلا
#وصف_ڪريو NOT_IMPLEMENTED CV_Error(Error::StsNotImplemented, "");
cv::String canonical(ثابت cv::String& /*رستو*/) { NOT_IMPLEMENTED }
منطقي exists(ثابت cv::String& /*رستو*/) { NOT_IMPLEMENTED }
فراغ remove_all(ثابت cv::String& /*رستو*/) { NOT_IMPLEMENTED }
cv::String getcwd() { NOT_IMPLEMENTED }
منطقي createDirectory(ثابت cv::String& /*رستو*/) { NOT_IMPLEMENTED }
منطقي createDirectories(ثابت cv::String& /*رستو*/) { NOT_IMPLEMENTED }
cv::String getCacheDirectory(ثابت حرف* /*sub_directory_name*/, ثابت حرف* /*configuration_name = NULL*/) { NOT_IMPLEMENTED }
#وصف_رد NOT_IMPLEMENTED
#جيڪڏهن_پورو // OPENCV_HAVE_FILESYSTEM_SUPPORT

}}} // namespace


#إذا OPENCV_HAVE_FILESYSTEM_SUPPORT
#ضمن "plugin_loader.impl.hpp"
#جيڪڏهن_پورو
