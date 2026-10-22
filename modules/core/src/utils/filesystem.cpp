// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ضمن "../precomp.hpp"

#ضمن <opencv2/core/utils/configuration.private.hpp>

#ضمن <opencv2/core/utils/logger.hpp>

#ضمن "opencv2/core/utils/filesystem.private.hpp"
#ضمن "opencv2/core/utils/filesystem.hpp"

//#define DEBUG_FS_UTILS

#که_تعریف_شوی DEBUG_FS_UTILS
#ضمن <iostream>
#تعریف_کړه DBG(...) __VA_ARGS__
#وإلا
#تعریف_کړه DBG(...)
#د_که_پای


#إذا OPENCV_HAVE_FILESYSTEM_SUPPORT

#که_تعریف_شوی _WIN32
#تعریف_کړه WIN32_LEAN_AND_منځنۍ
#تعریف_لغوه NOMINMAX
#تعریف_کړه NOMINMAX
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
#د_که_پای

#د_که_پای // OPENCV_HAVE_FILESYSTEM_SUPPORT

نطاق cv { نطاق utils { نطاق fs {

#که_تعریف_شوی _WIN32
ساكن ثابت حرف native_separator = '\\';
#وإلا
ساكن ثابت حرف native_separator = '/';
#د_که_پای

ساكن مضمن
منطقي is_لار_Separator(حرف c)
{
    أرجع c == '/' || c == '\\';
}

cv::String join(ثابت cv::String& بنسټ, ثابت cv::String& لار)
{
    إذا (بنسټ.empty())
        أرجع لار;
    إذا (لار.empty())
        أرجع بنسټ;

    منطقي بنسټ_Sep = is_لار_Separator(بنسټ[بنسټ.size() - 1]);
    منطقي لار_Sep = is_لار_Separator(لار[0]);
    String پایله;
    إذا (بنسټ_Sep && لار_Sep)
    {
        پایله = بنسټ + لار.substr(1);
    }
    وإلا إذا (!بنسټ_Sep && !لار_Sep)
    {
        پایله = بنسټ + native_separator + لار;
    }
    وإلا
    {
        پایله = بنسټ + لار;
    }
    أرجع پایله;
}

CV_EXPORTS cv::String get_پلار(ثابت cv::String &لار)
{
    std::string::size_type loc = لار.find_last_of("/\\");
    إذا (loc == std::string::npos)
        أرجع std::string();
    أرجع std::string(لار, 0, loc);
}

CV_EXPORTS std::wstring get_پلار(ثابت std::wstring& لار)
{
    std::wstring::size_type loc = لار.find_last_of(L"/\\");
    إذا (loc == std::wstring::npos)
        أرجع std::wstring();
    أرجع std::wstring(لار, 0, loc);
}

#إذا OPENCV_HAVE_FILESYSTEM_SUPPORT

cv::String canonical(ثابت cv::String& لار)
{
    cv::String پایله;
#که_تعریف_شوی _WIN32
    ثابت حرف* پایله_str = _fullpath(عدم, لار.c_str(), 0);
#وإلا
    ثابت حرف* پایله_str = realpath(لار.c_str(), عدم);
#د_که_پای
    إذا (پایله_str)
    {
        پایله = cv::String(پایله_str);
        حرر((فراغ*)پایله_str);
    }
    أرجع پایله.empty() ? لار : پایله;
}


منطقي exists(ثابت cv::String& لار)
{
    CV_INSTRUMENT_REGION();

#إذا defined _WIN32 || defined WINCE
    BOOL حالت = TRUE;
    {
        WIN32_FILE_ATTRIBUTE_معلومات all_attrs;
#که_تعریف_شوی WINRT
        wchar_t wpath[MAX_لار];
        size_t copied = mbstowcs(wpath, لار.c_str(), MAX_لار);
        CV_Assert((copied != MAX_لار) && (copied != (size_t)-1));
        حالت = ::Get_File_ځانګړتیاوې_Ex_W(wpath, GetFileExInfoStandard, &all_attrs);
#وإلا
        حالت = ::Get_File_ځانګړتیاوې_Ex_A(لار.c_str(), GetFileExInfoStandard, &all_attrs);
#د_که_پای
    }

    أرجع !!حالت;
#وإلا
    بنية stat stat_buf;
    أرجع (0 == stat(لار.c_str(), &stat_buf));
#د_که_پای
}

CV_EXPORTS فراغ remove_all(ثابت cv::String& لار)
{
    إذا (!exists(لار))
        أرجع;
    إذا (isDirectory(لار))
    {
        std::vector<String> entries;
        utils::fs::glob(لار, cv::String(), entries, خطأ, ریښتیا);
        لكل (size_t i = 0; i < entries.size(); i++)
        {
            ثابت String& e = entries[i];
            remove_all(e);
        }
#که_تعریف_شوی _MSC_VER
        منطقي پایله = _rmdir(لار.c_str()) == 0;
#وإلا
        منطقي پایله = rmdir(لار.c_str()) == 0;
#د_که_پای
        إذا (!پایله)
        {
            CV_LOG_ERROR(عدم, "Can't remove directory: " << لار);
        }
    }
    وإلا
    {
#که_تعریف_شوی _MSC_VER
        منطقي پایله = _unlink(لار.c_str()) == 0;
#وإلا
        منطقي پایله = unlink(لار.c_str()) == 0;
#د_که_پای
        إذا (!پایله)
        {
            CV_LOG_ERROR(عدم, "Can't remove file: " << لار);
        }
    }
}


cv::String getcwd()
{
    CV_INSTRUMENT_REGION();
    cv::Auto_بفر<حرف, 4096> buf;
#إذا defined WIN32 || defined _WIN32 || defined WINCE
#که_تعریف_شوی WINRT
    أرجع cv::String();
#وإلا
    DWORD sz = GetCurrentDirectoryA(0, عدم);
    buf.allocate((size_t)sz);
    sz = GetCurrentDirectoryA((DWORD)buf.size(), buf.معلومات());
    أرجع cv::String(buf.معلومات(), (size_t)sz);
#د_که_پای
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
    أرجع cv::String(buf.معلومات(), (size_t)مزي_اوږدوالی(buf.معلومات()));
#وإلا
    أرجع cv::String();
#د_که_پای
}


منطقي createDirectory(ثابت cv::String& لار)
{
    CV_INSTRUMENT_REGION();
#إذا defined WIN32 || defined _WIN32 || defined WINCE
#که_تعریف_شوی WINRT
    wchar_t wpath[MAX_لار];
    size_t copied = mbstowcs(wpath, لار.c_str(), MAX_لار);
    CV_Assert((copied != MAX_لار) && (copied != (size_t)-1));
    صحيح پایله = CreateDirectoryA(wpath, عدم) ? 0 : -1;
#وإلا
    صحيح پایله = _mkdir(لار.c_str());
#د_که_پای
#وإلاإذا defined __linux__ || defined __APPLE__ || defined __HAIKU__ || defined __FreeBSD__ || defined __EMSCRIPTEN__ || defined __QNX__ || defined __CYGWIN__
    صحيح پایله = mkdir(لار.c_str(), 0777);
#وإلا
    صحيح پایله = -1;
#د_که_پای

    إذا (پایله == -1)
    {
        أرجع isDirectory(لار);
    }
    أرجع ریښتیا;
}

منطقي createDirectories(ثابت cv::String& لار_3)
{
    cv::String لار = لار_3;
    لكل (;;)
    {
        حرف وروستی_char = لار.empty() ? 0 : لار[لار.length() - 1];
        إذا (is_لار_Separator(وروستی_char))
        {
            لار = لار.substr(0, لار.length() - 1);
            استمر;
        }
        اكسر;
    }

    إذا (لار.empty() || لار == "./" || لار == ".\\" || لار == ".")
        أرجع ریښتیا;
    إذا (isDirectory(لار))
        أرجع ریښتیا;

    size_t pos = لار.rfind('/');
    إذا (pos == cv::String::npos)
        pos = لار.rfind('\\');
    إذا (pos != cv::String::npos)
    {
        cv::String پلار_directory = لار.substr(0, pos);
        إذا (!پلار_directory.empty())
        {
            إذا (!createDirectories(پلار_directory))
                أرجع خطأ;
        }
    }

    أرجع createDirectory(لار);
}

#که_تعریف_شوی _WIN32

بنية File_قلف::Impl
{
    Impl(ثابت حرف* fname)
    {
        // http://support.microsoft.com/kb/316609
        صحيح عدد_Retries = 5;
        افعل
        {
            handle = ::CreateFileA(fname, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, عدم,
                                OPEN_EXISTING, FILE_ATTRIBUTE_نورمال, عدم);
            إذا (INVALID_HANDLE_ارزښت == handle)
            {
                إذا (ERROR_SHARING_VIOLATION == Get_وروستی_Error())
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
        إذا (INVALID_HANDLE_ارزښت != handle)
        {
            ::CloseHandle(handle);
        }
    }

    منطقي lock()
    {
        OVERLAPPED overlapped;
        std::memset(&overlapped, 0, حجم(overlapped));
        أرجع !!::قلف_File_Ex(handle, LOCKFILE_EXCLUSIVE_قلف, 0, MAXDWORD, MAXDWORD, &overlapped);
    }
    منطقي unlock()
    {
        OVERLAPPED overlapped;
        std::memset(&overlapped, 0, حجم(overlapped));
        أرجع !!::UnlockFileEx(handle, 0, MAXDWORD, MAXDWORD, &overlapped);
    }

    منطقي قلف_shared()
    {
        OVERLAPPED overlapped;
        std::memset(&overlapped, 0, حجم(overlapped));
        أرجع !!::قلف_File_Ex(handle, 0, 0, MAXDWORD, MAXDWORD, &overlapped);
    }
    منطقي unlock_shared()
    {
        أرجع unlock();
    }

    HANDLE handle;

خاص:
    Impl(ثابت Impl&); // disabled
    Impl& چلوونکی=(ثابت Impl&); // disabled
};

#وإلاإذا defined __linux__ || defined __APPLE__ || defined __HAIKU__ || defined __FreeBSD__ || defined __GNU__ || defined __EMSCRIPTEN__ || defined __QNX__ || defined __CYGWIN__

بنية File_قلف::Impl
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
        l.l_پیل = 0;
        l.l_len = 0;
        DBG(std::وتنه << "Lock..." << std::کرښه_پای);
        منطقي res = -1 != ::fcntl(handle, F_SETLKW, &l);
        أرجع res;
    }
    منطقي unlock()
    {
        بنية ::flock l;
        std::memset(&l, 0, حجم(l));
        l.l_type = F_UNLCK;
        l.l_whence = SEEK_SET;
        l.l_پیل = 0;
        l.l_len = 0;
        DBG(std::وتنه << "Unlock..." << std::کرښه_پای);
        منطقي res = -1 != ::fcntl(handle, F_SETLK, &l);
        أرجع res;
    }

    منطقي قلف_shared()
    {
        بنية ::flock l;
        std::memset(&l, 0, حجم(l));
        l.l_type = F_RDLCK;
        l.l_whence = SEEK_SET;
        l.l_پیل = 0;
        l.l_len = 0;
        DBG(std::وتنه << "Lock read..." << std::کرښه_پای);
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
    Impl& چلوونکی=(ثابت Impl&); // disabled
};

#د_که_پای

File_قلف::File_قلف(ثابت حرف* fname)
    : pImpl(جديد Impl(fname))
{
    // nothing
}
File_قلف::~File_قلف()
{
    ړنګول pImpl;
    pImpl = عدم;
}

فراغ File_قلف::lock() { CV_Assert(pImpl->lock()); }
فراغ File_قلف::unlock() { CV_Assert(pImpl->unlock()); }
فراغ File_قلف::قلف_shared() { CV_Assert(pImpl->قلف_shared()); }
فراغ File_قلف::unlock_shared() { CV_Assert(pImpl->unlock_shared()); }



cv::String getCacheDirectory(ثابت حرف* sub_directory_name, ثابت حرف* configuration_name)
{
    String cache_لار_2;
    إذا (configuration_name)
    {
        cache_لار_2 = utils::getConfigurationParameterString(configuration_name, "");
    }
    إذا (cache_لار_2.empty())
    {
        cv::String default_cache_لار;
#که_تعریف_شوی _WIN32
        wchar_t tmp_لار_buf_w[MAX_لار+1] = {0};
        DWORD res = Get_لنډمهاله_لار_W(MAX_لار, tmp_لار_buf_w);
        إذا (res > 0 && res <= MAX_لار)
        {
            // Convert from UTF-16 to UTF-8 to support Unicode paths
            صحيح len = Wide_Char_منزل_Multi_Byte(CP_UTF8, 0, tmp_لار_buf_w, -1, عدم, 0, عدم, عدم);
            إذا (len > 0)
            {
                std::vector<حرف> utf8_buf(len);
                Wide_Char_منزل_Multi_Byte(CP_UTF8, 0, tmp_لار_buf_w, -1, utf8_buf.معلومات(), len, عدم, عدم);
                default_cache_لار = std::string(utf8_buf.معلومات());
            }
        }
#وإلاإذا defined __ANDROID__
        // no defaults
#وإلاإذا defined __APPLE__
        ثابت std::string tmpdir_env = utils::getConfigurationParameterString("TMPDIR");
        إذا (!tmpdir_env.empty() && utils::fs::isDirectory(tmpdir_env))
        {
            default_cache_لار = tmpdir_env;
        }
        وإلا
        {
            default_cache_لار = "/tmp/";
            CV_LOG_WARNING(عدم, "Using world accessible cache directory. This may be not secure: " << default_cache_لار);
        }
#وإلاإذا defined __linux__ || defined __HAIKU__ || defined __FreeBSD__ || defined __GNU__
        // https://specifications.freedesktop.org/basedir-spec/basedir-spec-latest.html
        إذا (default_cache_لار.empty())
        {
            ثابت std::string xdg_cache_env = utils::getConfigurationParameterString("XDG_CACHE_HOME");
            إذا (!xdg_cache_env.empty() && utils::fs::isDirectory(xdg_cache_env))
            {
                default_cache_لار = xdg_cache_env;
            }
        }
        إذا (default_cache_لار.empty())
        {
            ثابت std::string home_env = utils::getConfigurationParameterString("HOME");
            إذا (!home_env.empty() && utils::fs::isDirectory(home_env))
            {
                cv::String home_لار = home_env;
                cv::String home_cache_لار = utils::fs::join(home_لار, ".cache/");
                إذا (utils::fs::isDirectory(home_cache_لار))
                {
                    default_cache_لار = home_cache_لار;
                }
            }
        }
        إذا (default_cache_لار.empty())
        {
            ثابت حرف* لنډمهاله_لار = "/var/tmp/";
            إذا (utils::fs::isDirectory(لنډمهاله_لار))
            {
                default_cache_لار = لنډمهاله_لار;
                CV_LOG_WARNING(عدم, "Using world accessible cache directory. This may be not secure: " << default_cache_لار);
            }
        }
        إذا (default_cache_لار.empty())
        {
            default_cache_لار = "/tmp/";
            CV_LOG_WARNING(عدم, "Using world accessible cache directory. This may be not secure: " << default_cache_لار);
        }
#وإلا
        // no defaults
#د_که_پای
        CV_LOG_VERBOSE(عدم, 0, "default_cache_path = " << default_cache_لار);
        إذا (!default_cache_لار.empty())
        {
            إذا (utils::fs::isDirectory(default_cache_لار))
            {
                cv::String default_cache_لار_بنسټ = utils::fs::join(default_cache_لار, "opencv");
                default_cache_لار = utils::fs::join(default_cache_لار_بنسټ, CVAUX_STR(CV_نسخه_MAJOR) ".x" CV_نسخه_حالت);
                إذا (utils::getConfigurationParameterBool("OPENCV_CACHE_SHOW_CLEANUP_MESSAGE", ریښتیا)
                    && !utils::fs::isDirectory(default_cache_لار))
                {
                    std::vector<cv::String> existedCacheDirs;
                    حاول
                    {
                        utils::fs::glob_relative(default_cache_لار_بنسټ, "*", existedCacheDirs, خطأ, ریښتیا);
                    }
                    امسك (...)
                    {
                        // ignore
                    }
                    إذا (!existedCacheDirs.empty())
                    {
                        CV_LOG_WARNING(عدم, "Creating new OpenCV cache directory: " << default_cache_لار);
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
                    default_cache_لار = utils::fs::join(default_cache_لار, cv::String(sub_directory_name) + native_separator);
                إذا (!utils::fs::createDirectories(default_cache_لار))
                {
                    CV_LOG_DEBUG(عدم, "Can't create OpenCV cache sub-directory: " << default_cache_لار);
                }
                وإلا
                {
                    cache_لار_2 = default_cache_لار;
                }
            }
            وإلا
            {
                CV_LOG_INFO(عدم, "Can't find default cache directory (does it exist?): " << default_cache_لار);
            }
        }
        وإلا
        {
            CV_LOG_DEBUG(عدم, "OpenCV has no support to discover default cache directory on the current platform");
        }
    }
    وإلا
    {
        إذا (cache_لار_2 == "disabled")
            أرجع cache_لار_2;
        إذا (!isDirectory(cache_لار_2))
        {
            CV_LOG_WARNING(عدم, "Specified non-existed directory, creating OpenCV sub-directory for caching purposes: " << cache_لار_2);
            إذا (!createDirectories(cache_لار_2))
            {
                CV_LOG_ERROR(عدم, "Can't create OpenCV cache sub-directory: " << cache_لار_2);
                cache_لار_2.clear();
            }
        }
    }
    CV_Assert(cache_لار_2.empty() || utils::fs::isDirectory(cache_لار_2));
    إذا (!cache_لار_2.empty())
    {
        إذا (!is_لار_Separator(cache_لار_2[cache_لار_2.size() - 1]))
        {
            cache_لار_2 += native_separator;
        }
    }
    أرجع cache_لار_2;
}

#وإلا
#تعریف_کړه NOT_IMPLEMENTED CV_Error(Error::StsNotImplemented, "");
cv::String canonical(ثابت cv::String& /*لار*/) { NOT_IMPLEMENTED }
منطقي exists(ثابت cv::String& /*لار*/) { NOT_IMPLEMENTED }
فراغ remove_all(ثابت cv::String& /*لار*/) { NOT_IMPLEMENTED }
cv::String getcwd() { NOT_IMPLEMENTED }
منطقي createDirectory(ثابت cv::String& /*لار*/) { NOT_IMPLEMENTED }
منطقي createDirectories(ثابت cv::String& /*لار*/) { NOT_IMPLEMENTED }
cv::String getCacheDirectory(ثابت حرف* /*sub_directory_name*/, ثابت حرف* /*configuration_name = NULL*/) { NOT_IMPLEMENTED }
#تعریف_لغوه NOT_IMPLEMENTED
#د_که_پای // OPENCV_HAVE_FILESYSTEM_SUPPORT

}}} // namespace


#إذا OPENCV_HAVE_FILESYSTEM_SUPPORT
#ضمن "plugin_loader.impl.hpp"
#د_که_پای
