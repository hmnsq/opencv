// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ضمن "../precomp.hpp"

#ضمن <opencv2/core/utils/configuration.private.hpp>

#ضمن <opencv2/core/utils/logger.hpp>

#ضمن "opencv2/core/utils/filesystem.private.hpp"
#ضمن "opencv2/core/utils/filesystem.hpp"

//#define DEBUG_FS_UTILS

#اگر_تعریف_شده DEBUG_FS_UTILS
#ضمن <iostream>
#تعریف_کن DBG(...) __VA_ARGS__
#وإلا
#تعریف_کن DBG(...)
#پایان_اگر


#إذا OPENCV_HAVE_FILESYSTEM_SUPPORT

#اگر_تعریف_شده _WIN32
#تعریف_کن WIN32_LEAN_AND_میانگین
#لغو_تعریف NOMINMAX
#تعریف_کن NOMINMAX
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
#پایان_اگر

#پایان_اگر // OPENCV_HAVE_FILESYSTEM_SUPPORT

نطاق cv { نطاق utils { نطاق fs {

#اگر_تعریف_شده _WIN32
ساكن ثابت حرف native_separator = '\\';
#وإلا
ساكن ثابت حرف native_separator = '/';
#پایان_اگر

ساكن مضمن
منطقي is_مسیر_Separator(حرف c)
{
    أرجع c == '/' || c == '\\';
}

cv::String join(ثابت cv::String& پایه, ثابت cv::String& مسیر)
{
    إذا (پایه.empty())
        أرجع مسیر;
    إذا (مسیر.empty())
        أرجع پایه;

    منطقي پایه_Sep = is_مسیر_Separator(پایه[پایه.size() - 1]);
    منطقي مسیر_Sep = is_مسیر_Separator(مسیر[0]);
    String نتیجه;
    إذا (پایه_Sep && مسیر_Sep)
    {
        نتیجه = پایه + مسیر.substr(1);
    }
    وإلا إذا (!پایه_Sep && !مسیر_Sep)
    {
        نتیجه = پایه + native_separator + مسیر;
    }
    وإلا
    {
        نتیجه = پایه + مسیر;
    }
    أرجع نتیجه;
}

CV_EXPORTS cv::String get_والد(ثابت cv::String &مسیر)
{
    std::string::size_type loc = مسیر.find_last_of("/\\");
    إذا (loc == std::string::npos)
        أرجع std::string();
    أرجع std::string(مسیر, 0, loc);
}

CV_EXPORTS std::wstring get_والد(ثابت std::wstring& مسیر)
{
    std::wstring::size_type loc = مسیر.find_last_of(L"/\\");
    إذا (loc == std::wstring::npos)
        أرجع std::wstring();
    أرجع std::wstring(مسیر, 0, loc);
}

#إذا OPENCV_HAVE_FILESYSTEM_SUPPORT

cv::String canonical(ثابت cv::String& مسیر)
{
    cv::String نتیجه;
#اگر_تعریف_شده _WIN32
    ثابت حرف* نتیجه_str = _fullpath(عدم, مسیر.c_str(), 0);
#وإلا
    ثابت حرف* نتیجه_str = realpath(مسیر.c_str(), عدم);
#پایان_اگر
    إذا (نتیجه_str)
    {
        نتیجه = cv::String(نتیجه_str);
        حرر((فراغ*)نتیجه_str);
    }
    أرجع نتیجه.empty() ? مسیر : نتیجه;
}


منطقي exists(ثابت cv::String& مسیر)
{
    CV_INSTRUMENT_REGION();

#إذا defined _WIN32 || defined WINCE
    BOOL وضعیت = TRUE;
    {
        WIN32_FILE_ATTRIBUTE_داده all_attrs;
#اگر_تعریف_شده WINRT
        wchar_t wpath[MAX_مسیر];
        size_t copied = mbstowcs(wpath, مسیر.c_str(), MAX_مسیر);
        CV_Assert((copied != MAX_مسیر) && (copied != (size_t)-1));
        وضعیت = ::Get_File_صفات_Ex_W(wpath, GetFileExInfoStandard, &all_attrs);
#وإلا
        وضعیت = ::Get_File_صفات_Ex_A(مسیر.c_str(), GetFileExInfoStandard, &all_attrs);
#پایان_اگر
    }

    أرجع !!وضعیت;
#وإلا
    بنية stat stat_buf;
    أرجع (0 == stat(مسیر.c_str(), &stat_buf));
#پایان_اگر
}

CV_EXPORTS فراغ remove_all(ثابت cv::String& مسیر)
{
    إذا (!exists(مسیر))
        أرجع;
    إذا (isDirectory(مسیر))
    {
        std::vector<String> entries;
        utils::fs::glob(مسیر, cv::String(), entries, خطأ, درست);
        لكل (size_t i = 0; i < entries.size(); i++)
        {
            ثابت String& e = entries[i];
            remove_all(e);
        }
#اگر_تعریف_شده _MSC_VER
        منطقي نتیجه = _rmdir(مسیر.c_str()) == 0;
#وإلا
        منطقي نتیجه = rmdir(مسیر.c_str()) == 0;
#پایان_اگر
        إذا (!نتیجه)
        {
            CV_LOG_ERROR(عدم, "Can't remove directory: " << مسیر);
        }
    }
    وإلا
    {
#اگر_تعریف_شده _MSC_VER
        منطقي نتیجه = _unlink(مسیر.c_str()) == 0;
#وإلا
        منطقي نتیجه = unlink(مسیر.c_str()) == 0;
#پایان_اگر
        إذا (!نتیجه)
        {
            CV_LOG_ERROR(عدم, "Can't remove file: " << مسیر);
        }
    }
}


cv::String getcwd()
{
    CV_INSTRUMENT_REGION();
    cv::Auto_میانگیر<حرف, 4096> buf;
#إذا defined WIN32 || defined _WIN32 || defined WINCE
#اگر_تعریف_شده WINRT
    أرجع cv::String();
#وإلا
    DWORD sz = GetCurrentDirectoryA(0, عدم);
    buf.allocate((size_t)sz);
    sz = GetCurrentDirectoryA((DWORD)buf.size(), buf.داده());
    أرجع cv::String(buf.داده(), (size_t)sz);
#پایان_اگر
#وإلاإذا defined __linux__ || defined __APPLE__ || defined __HAIKU__ || defined __FreeBSD__ || defined __EMSCRIPTEN__ || defined __QNX__ || defined __CYGWIN__
    لكل(;;)
    {
        حرف* p = ::getcwd(buf.داده(), buf.size());
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
    أرجع cv::String(buf.داده(), (size_t)طول_رشته(buf.داده()));
#وإلا
    أرجع cv::String();
#پایان_اگر
}


منطقي createDirectory(ثابت cv::String& مسیر)
{
    CV_INSTRUMENT_REGION();
#إذا defined WIN32 || defined _WIN32 || defined WINCE
#اگر_تعریف_شده WINRT
    wchar_t wpath[MAX_مسیر];
    size_t copied = mbstowcs(wpath, مسیر.c_str(), MAX_مسیر);
    CV_Assert((copied != MAX_مسیر) && (copied != (size_t)-1));
    صحيح نتیجه = CreateDirectoryA(wpath, عدم) ? 0 : -1;
#وإلا
    صحيح نتیجه = _mkdir(مسیر.c_str());
#پایان_اگر
#وإلاإذا defined __linux__ || defined __APPLE__ || defined __HAIKU__ || defined __FreeBSD__ || defined __EMSCRIPTEN__ || defined __QNX__ || defined __CYGWIN__
    صحيح نتیجه = mkdir(مسیر.c_str(), 0777);
#وإلا
    صحيح نتیجه = -1;
#پایان_اگر

    إذا (نتیجه == -1)
    {
        أرجع isDirectory(مسیر);
    }
    أرجع درست;
}

منطقي createDirectories(ثابت cv::String& مسیر_3)
{
    cv::String مسیر = مسیر_3;
    لكل (;;)
    {
        حرف آخرین_char = مسیر.empty() ? 0 : مسیر[مسیر.length() - 1];
        إذا (is_مسیر_Separator(آخرین_char))
        {
            مسیر = مسیر.substr(0, مسیر.length() - 1);
            استمر;
        }
        اكسر;
    }

    إذا (مسیر.empty() || مسیر == "./" || مسیر == ".\\" || مسیر == ".")
        أرجع درست;
    إذا (isDirectory(مسیر))
        أرجع درست;

    size_t pos = مسیر.rfind('/');
    إذا (pos == cv::String::npos)
        pos = مسیر.rfind('\\');
    إذا (pos != cv::String::npos)
    {
        cv::String والد_directory = مسیر.substr(0, pos);
        إذا (!والد_directory.empty())
        {
            إذا (!createDirectories(والد_directory))
                أرجع خطأ;
        }
    }

    أرجع createDirectory(مسیر);
}

#اگر_تعریف_شده _WIN32

بنية File_قفل::Impl
{
    Impl(ثابت حرف* fname)
    {
        // http://support.microsoft.com/kb/316609
        صحيح رقم_Retries = 5;
        افعل
        {
            handle = ::CreateFileA(fname, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, عدم,
                                OPEN_EXISTING, FILE_ATTRIBUTE_نرمال, عدم);
            إذا (INVALID_HANDLE_مقدار == handle)
            {
                إذا (ERROR_SHARING_VIOLATION == Get_آخرین_Error())
                {
                    رقم_Retries--;
                    Sleep(250);
                    استمر;
                }
                وإلا
                {
                    CV_Error_(Error::StsAssert, ("Can't open lock file: %s", fname));
                }
            }
            اكسر;
        } طالما (رقم_Retries > 0);
    }
    ~Impl()
    {
        إذا (INVALID_HANDLE_مقدار != handle)
        {
            ::CloseHandle(handle);
        }
    }

    منطقي lock()
    {
        OVERLAPPED overlapped;
        std::memset(&overlapped, 0, حجم(overlapped));
        أرجع !!::قفل_File_Ex(handle, LOCKFILE_EXCLUSIVE_قفل, 0, MAXDWORD, MAXDWORD, &overlapped);
    }
    منطقي unlock()
    {
        OVERLAPPED overlapped;
        std::memset(&overlapped, 0, حجم(overlapped));
        أرجع !!::UnlockFileEx(handle, 0, MAXDWORD, MAXDWORD, &overlapped);
    }

    منطقي قفل_shared()
    {
        OVERLAPPED overlapped;
        std::memset(&overlapped, 0, حجم(overlapped));
        أرجع !!::قفل_File_Ex(handle, 0, 0, MAXDWORD, MAXDWORD, &overlapped);
    }
    منطقي unlock_shared()
    {
        أرجع unlock();
    }

    HANDLE handle;

خاص:
    Impl(ثابت Impl&); // disabled
    Impl& عملگر=(ثابت Impl&); // disabled
};

#وإلاإذا defined __linux__ || defined __APPLE__ || defined __HAIKU__ || defined __FreeBSD__ || defined __GNU__ || defined __EMSCRIPTEN__ || defined __QNX__ || defined __CYGWIN__

بنية File_قفل::Impl
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
        DBG(std::خروجی << "Lock..." << std::پایان_سطر);
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
        DBG(std::خروجی << "Unlock..." << std::پایان_سطر);
        منطقي res = -1 != ::fcntl(handle, F_SETLK, &l);
        أرجع res;
    }

    منطقي قفل_shared()
    {
        بنية ::flock l;
        std::memset(&l, 0, حجم(l));
        l.l_type = F_RDLCK;
        l.l_whence = SEEK_SET;
        l.l_شروع = 0;
        l.l_len = 0;
        DBG(std::خروجی << "Lock read..." << std::پایان_سطر);
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
    Impl& عملگر=(ثابت Impl&); // disabled
};

#پایان_اگر

File_قفل::File_قفل(ثابت حرف* fname)
    : pImpl(جديد Impl(fname))
{
    // nothing
}
File_قفل::~File_قفل()
{
    حذف pImpl;
    pImpl = عدم;
}

فراغ File_قفل::lock() { CV_Assert(pImpl->lock()); }
فراغ File_قفل::unlock() { CV_Assert(pImpl->unlock()); }
فراغ File_قفل::قفل_shared() { CV_Assert(pImpl->قفل_shared()); }
فراغ File_قفل::unlock_shared() { CV_Assert(pImpl->unlock_shared()); }



cv::String getCacheDirectory(ثابت حرف* sub_directory_name, ثابت حرف* configuration_name)
{
    String cache_مسیر_2;
    إذا (configuration_name)
    {
        cache_مسیر_2 = utils::getConfigurationParameterString(configuration_name, "");
    }
    إذا (cache_مسیر_2.empty())
    {
        cv::String default_cache_مسیر;
#اگر_تعریف_شده _WIN32
        wchar_t tmp_مسیر_buf_w[MAX_مسیر+1] = {0};
        DWORD res = Get_موقت_مسیر_W(MAX_مسیر, tmp_مسیر_buf_w);
        إذا (res > 0 && res <= MAX_مسیر)
        {
            // Convert from UTF-16 to UTF-8 to support Unicode paths
            صحيح len = Wide_Char_مقصد_Multi_Byte(CP_UTF8, 0, tmp_مسیر_buf_w, -1, عدم, 0, عدم, عدم);
            إذا (len > 0)
            {
                std::vector<حرف> utf8_buf(len);
                Wide_Char_مقصد_Multi_Byte(CP_UTF8, 0, tmp_مسیر_buf_w, -1, utf8_buf.داده(), len, عدم, عدم);
                default_cache_مسیر = std::string(utf8_buf.داده());
            }
        }
#وإلاإذا defined __ANDROID__
        // no defaults
#وإلاإذا defined __APPLE__
        ثابت std::string tmpdir_env = utils::getConfigurationParameterString("TMPDIR");
        إذا (!tmpdir_env.empty() && utils::fs::isDirectory(tmpdir_env))
        {
            default_cache_مسیر = tmpdir_env;
        }
        وإلا
        {
            default_cache_مسیر = "/tmp/";
            CV_LOG_WARNING(عدم, "Using world accessible cache directory. This may be not secure: " << default_cache_مسیر);
        }
#وإلاإذا defined __linux__ || defined __HAIKU__ || defined __FreeBSD__ || defined __GNU__
        // https://specifications.freedesktop.org/basedir-spec/basedir-spec-latest.html
        إذا (default_cache_مسیر.empty())
        {
            ثابت std::string xdg_cache_env = utils::getConfigurationParameterString("XDG_CACHE_HOME");
            إذا (!xdg_cache_env.empty() && utils::fs::isDirectory(xdg_cache_env))
            {
                default_cache_مسیر = xdg_cache_env;
            }
        }
        إذا (default_cache_مسیر.empty())
        {
            ثابت std::string home_env = utils::getConfigurationParameterString("HOME");
            إذا (!home_env.empty() && utils::fs::isDirectory(home_env))
            {
                cv::String home_مسیر = home_env;
                cv::String home_cache_مسیر = utils::fs::join(home_مسیر, ".cache/");
                إذا (utils::fs::isDirectory(home_cache_مسیر))
                {
                    default_cache_مسیر = home_cache_مسیر;
                }
            }
        }
        إذا (default_cache_مسیر.empty())
        {
            ثابت حرف* موقت_مسیر = "/var/tmp/";
            إذا (utils::fs::isDirectory(موقت_مسیر))
            {
                default_cache_مسیر = موقت_مسیر;
                CV_LOG_WARNING(عدم, "Using world accessible cache directory. This may be not secure: " << default_cache_مسیر);
            }
        }
        إذا (default_cache_مسیر.empty())
        {
            default_cache_مسیر = "/tmp/";
            CV_LOG_WARNING(عدم, "Using world accessible cache directory. This may be not secure: " << default_cache_مسیر);
        }
#وإلا
        // no defaults
#پایان_اگر
        CV_LOG_VERBOSE(عدم, 0, "default_cache_path = " << default_cache_مسیر);
        إذا (!default_cache_مسیر.empty())
        {
            إذا (utils::fs::isDirectory(default_cache_مسیر))
            {
                cv::String default_cache_مسیر_پایه = utils::fs::join(default_cache_مسیر, "opencv");
                default_cache_مسیر = utils::fs::join(default_cache_مسیر_پایه, CVAUX_STR(CV_نسخه_MAJOR) ".x" CV_نسخه_وضعیت);
                إذا (utils::getConfigurationParameterBool("OPENCV_CACHE_SHOW_CLEANUP_MESSAGE", درست)
                    && !utils::fs::isDirectory(default_cache_مسیر))
                {
                    std::vector<cv::String> existedCacheDirs;
                    حاول
                    {
                        utils::fs::glob_relative(default_cache_مسیر_پایه, "*", existedCacheDirs, خطأ, درست);
                    }
                    امسك (...)
                    {
                        // ignore
                    }
                    إذا (!existedCacheDirs.empty())
                    {
                        CV_LOG_WARNING(عدم, "Creating new OpenCV cache directory: " << default_cache_مسیر);
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
                    default_cache_مسیر = utils::fs::join(default_cache_مسیر, cv::String(sub_directory_name) + native_separator);
                إذا (!utils::fs::createDirectories(default_cache_مسیر))
                {
                    CV_LOG_DEBUG(عدم, "Can't create OpenCV cache sub-directory: " << default_cache_مسیر);
                }
                وإلا
                {
                    cache_مسیر_2 = default_cache_مسیر;
                }
            }
            وإلا
            {
                CV_LOG_INFO(عدم, "Can't find default cache directory (does it exist?): " << default_cache_مسیر);
            }
        }
        وإلا
        {
            CV_LOG_DEBUG(عدم, "OpenCV has no support to discover default cache directory on the current platform");
        }
    }
    وإلا
    {
        إذا (cache_مسیر_2 == "disabled")
            أرجع cache_مسیر_2;
        إذا (!isDirectory(cache_مسیر_2))
        {
            CV_LOG_WARNING(عدم, "Specified non-existed directory, creating OpenCV sub-directory for caching purposes: " << cache_مسیر_2);
            إذا (!createDirectories(cache_مسیر_2))
            {
                CV_LOG_ERROR(عدم, "Can't create OpenCV cache sub-directory: " << cache_مسیر_2);
                cache_مسیر_2.clear();
            }
        }
    }
    CV_Assert(cache_مسیر_2.empty() || utils::fs::isDirectory(cache_مسیر_2));
    إذا (!cache_مسیر_2.empty())
    {
        إذا (!is_مسیر_Separator(cache_مسیر_2[cache_مسیر_2.size() - 1]))
        {
            cache_مسیر_2 += native_separator;
        }
    }
    أرجع cache_مسیر_2;
}

#وإلا
#تعریف_کن NOT_IMPLEMENTED CV_Error(Error::StsNotImplemented, "");
cv::String canonical(ثابت cv::String& /*مسیر*/) { NOT_IMPLEMENTED }
منطقي exists(ثابت cv::String& /*مسیر*/) { NOT_IMPLEMENTED }
فراغ remove_all(ثابت cv::String& /*مسیر*/) { NOT_IMPLEMENTED }
cv::String getcwd() { NOT_IMPLEMENTED }
منطقي createDirectory(ثابت cv::String& /*مسیر*/) { NOT_IMPLEMENTED }
منطقي createDirectories(ثابت cv::String& /*مسیر*/) { NOT_IMPLEMENTED }
cv::String getCacheDirectory(ثابت حرف* /*sub_directory_name*/, ثابت حرف* /*configuration_name = NULL*/) { NOT_IMPLEMENTED }
#لغو_تعریف NOT_IMPLEMENTED
#پایان_اگر // OPENCV_HAVE_FILESYSTEM_SUPPORT

}}} // namespace


#إذا OPENCV_HAVE_FILESYSTEM_SUPPORT
#ضمن "plugin_loader.impl.hpp"
#پایان_اگر
