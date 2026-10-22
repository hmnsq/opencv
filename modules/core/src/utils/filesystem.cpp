// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#شامل "../precomp.hpp"

#شامل <opencv2/core/utils/configuration.private.hpp>

#شامل <opencv2/core/utils/logger.hpp>

#شامل "opencv2/core/utils/filesystem.private.hpp"
#شامل "opencv2/core/utils/filesystem.hpp"

//#define DEBUG_FS_UTILS

#ifdef DEBUG_FS_UTILS
#شامل <iostream>
#define DBG(...) __VA_ARGS__
#وگرنه
#define DBG(...)
#endif


#اگر OPENCV_HAVE_FILESYSTEM_SUPPORT

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#undef NOMINMAX
#define NOMINMAX
#شامل <windows.h>
#شامل <direct.h>
#شامل <sys/types.h>
#شامل <sys/stat.h>
#شامل <errno.h>
#شامل <io.h>
#شامل <stdio.h>
#وگرنهاگر defined __linux__ || defined __APPLE__ || defined __HAIKU__ || defined __FreeBSD__ || defined __GNU__ || defined __EMSCRIPTEN__ || defined __QNX__ || defined __CYGWIN__
#شامل <sys/types.h>
#شامل <sys/stat.h>
#شامل <fcntl.h>
#شامل <unistd.h>
#شامل <errno.h>
#endif

#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT

فضاینام cv { فضاینام utils { فضاینام fs {

#ifdef _WIN32
ایستا ثابت نویسه native_separator = '\\';
#وگرنه
ایستا ثابت نویسه native_separator = '/';
#endif

ایستا درونخطی
منطقی isPathSeparator(نویسه c)
{
    بازگشت c == '/' || c == '\\';
}

cv::String join(ثابت cv::String& base, ثابت cv::String& path)
{
    اگر (base.empty())
        بازگشت path;
    اگر (path.empty())
        بازگشت base;

    منطقی baseSep = isPathSeparator(base[base.size() - 1]);
    منطقی pathSep = isPathSeparator(path[0]);
    String result;
    اگر (baseSep && pathSep)
    {
        result = base + path.substr(1);
    }
    وگرنه اگر (!baseSep && !pathSep)
    {
        result = base + native_separator + path;
    }
    وگرنه
    {
        result = base + path;
    }
    بازگشت result;
}

CV_EXPORTS cv::String getParent(ثابت cv::String &path)
{
    std::string::size_type loc = path.find_last_of("/\\");
    اگر (loc == std::string::npos)
        بازگشت std::string();
    بازگشت std::string(path, 0, loc);
}

CV_EXPORTS std::wstring getParent(ثابت std::wstring& path)
{
    std::wstring::size_type loc = path.find_last_of(L"/\\");
    اگر (loc == std::wstring::npos)
        بازگشت std::wstring();
    بازگشت std::wstring(path, 0, loc);
}

#اگر OPENCV_HAVE_FILESYSTEM_SUPPORT

cv::String canonical(ثابت cv::String& path)
{
    cv::String result;
#ifdef _WIN32
    ثابت نویسه* result_str = _fullpath(پوچ, path.c_str(), 0);
#وگرنه
    ثابت نویسه* result_str = realpath(path.c_str(), پوچ);
#endif
    اگر (result_str)
    {
        result = cv::String(result_str);
        آزاد((تهی*)result_str);
    }
    بازگشت result.empty() ? path : result;
}


منطقی exists(ثابت cv::String& path)
{
    CV_INSTRUMENT_REGION();

#اگر defined _WIN32 || defined WINCE
    BOOL status = TRUE;
    {
        WIN32_FILE_ATTRIBUTE_DATA all_attrs;
#ifdef WINRT
        wchar_t wpath[MAX_PATH];
        size_t copied = mbstowcs(wpath, path.c_str(), MAX_PATH);
        CV_Assert((copied != MAX_PATH) && (copied != (size_t)-1));
        status = ::GetFileAttributesExW(wpath, GetFileExInfoStandard, &all_attrs);
#وگرنه
        status = ::GetFileAttributesExA(path.c_str(), GetFileExInfoStandard, &all_attrs);
#endif
    }

    بازگشت !!status;
#وگرنه
    ساختار stat stat_buf;
    بازگشت (0 == stat(path.c_str(), &stat_buf));
#endif
}

CV_EXPORTS تهی remove_all(ثابت cv::String& path)
{
    اگر (!exists(path))
        بازگشت;
    اگر (isDirectory(path))
    {
        std::vector<String> entries;
        utils::fs::glob(path, cv::String(), entries, نادرست, درست);
        برای (size_t i = 0; i < entries.size(); i++)
        {
            ثابت String& e = entries[i];
            remove_all(e);
        }
#ifdef _MSC_VER
        منطقی result = _rmdir(path.c_str()) == 0;
#وگرنه
        منطقی result = rmdir(path.c_str()) == 0;
#endif
        اگر (!result)
        {
            CV_LOG_ERROR(پوچ, "Can't remove directory: " << path);
        }
    }
    وگرنه
    {
#ifdef _MSC_VER
        منطقی result = _unlink(path.c_str()) == 0;
#وگرنه
        منطقی result = unlink(path.c_str()) == 0;
#endif
        اگر (!result)
        {
            CV_LOG_ERROR(پوچ, "Can't remove file: " << path);
        }
    }
}


cv::String getcwd()
{
    CV_INSTRUMENT_REGION();
    cv::AutoBuffer<نویسه, 4096> buf;
#اگر defined WIN32 || defined _WIN32 || defined WINCE
#ifdef WINRT
    بازگشت cv::String();
#وگرنه
    DWORD sz = GetCurrentDirectoryA(0, پوچ);
    buf.allocate((size_t)sz);
    sz = GetCurrentDirectoryA((DWORD)buf.size(), buf.data());
    بازگشت cv::String(buf.data(), (size_t)sz);
#endif
#وگرنهاگر defined __linux__ || defined __APPLE__ || defined __HAIKU__ || defined __FreeBSD__ || defined __EMSCRIPTEN__ || defined __QNX__ || defined __CYGWIN__
    برای(;;)
    {
        نویسه* p = ::getcwd(buf.data(), buf.size());
        اگر (p == پوچ)
        {
            اگر (errno == ERANGE)
            {
                buf.allocate(buf.size() * 2);
                ادامه;
            }
            بازگشت cv::String();
        }
        بشکن;
    }
    بازگشت cv::String(buf.data(), (size_t)strlen(buf.data()));
#وگرنه
    بازگشت cv::String();
#endif
}


منطقی createDirectory(ثابت cv::String& path)
{
    CV_INSTRUMENT_REGION();
#اگر defined WIN32 || defined _WIN32 || defined WINCE
#ifdef WINRT
    wchar_t wpath[MAX_PATH];
    size_t copied = mbstowcs(wpath, path.c_str(), MAX_PATH);
    CV_Assert((copied != MAX_PATH) && (copied != (size_t)-1));
    صحیح result = CreateDirectoryA(wpath, پوچ) ? 0 : -1;
#وگرنه
    صحیح result = _mkdir(path.c_str());
#endif
#وگرنهاگر defined __linux__ || defined __APPLE__ || defined __HAIKU__ || defined __FreeBSD__ || defined __EMSCRIPTEN__ || defined __QNX__ || defined __CYGWIN__
    صحیح result = mkdir(path.c_str(), 0777);
#وگرنه
    صحیح result = -1;
#endif

    اگر (result == -1)
    {
        بازگشت isDirectory(path);
    }
    بازگشت درست;
}

منطقی createDirectories(ثابت cv::String& path_)
{
    cv::String path = path_;
    برای (;;)
    {
        نویسه last_char = path.empty() ? 0 : path[path.length() - 1];
        اگر (isPathSeparator(last_char))
        {
            path = path.substr(0, path.length() - 1);
            ادامه;
        }
        بشکن;
    }

    اگر (path.empty() || path == "./" || path == ".\\" || path == ".")
        بازگشت درست;
    اگر (isDirectory(path))
        بازگشت درست;

    size_t pos = path.rfind('/');
    اگر (pos == cv::String::npos)
        pos = path.rfind('\\');
    اگر (pos != cv::String::npos)
    {
        cv::String parent_directory = path.substr(0, pos);
        اگر (!parent_directory.empty())
        {
            اگر (!createDirectories(parent_directory))
                بازگشت نادرست;
        }
    }

    بازگشت createDirectory(path);
}

#ifdef _WIN32

ساختار FileLock::Impl
{
    Impl(ثابت نویسه* fname)
    {
        // http://support.microsoft.com/kb/316609
        صحیح رقم_Retries = 5;
        انجامبده
        {
            handle = ::CreateFileA(fname, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, پوچ,
                                OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, پوچ);
            اگر (INVALID_HANDLE_مقدار == handle)
            {
                اگر (ERROR_SHARING_VIOLATION == GetLastError())
                {
                    رقم_Retries--;
                    Sleep(250);
                    ادامه;
                }
                وگرنه
                {
                    CV_Error_(Error::StsAssert, ("Can't open lock file: %s", fname));
                }
            }
            بشکن;
        } وقتی (رقم_Retries > 0);
    }
    ~Impl()
    {
        اگر (INVALID_HANDLE_مقدار != handle)
        {
            ::CloseHandle(handle);
        }
    }

    منطقی lock()
    {
        OVERLAPPED overlapped;
        std::memset(&overlapped, 0, اندازه(overlapped));
        بازگشت !!::LockFileEx(handle, LOCKFILE_EXCLUSIVE_LOCK, 0, MAXDWORD, MAXDWORD, &overlapped);
    }
    منطقی unlock()
    {
        OVERLAPPED overlapped;
        std::memset(&overlapped, 0, اندازه(overlapped));
        بازگشت !!::UnlockFileEx(handle, 0, MAXDWORD, MAXDWORD, &overlapped);
    }

    منطقی lock_shared()
    {
        OVERLAPPED overlapped;
        std::memset(&overlapped, 0, اندازه(overlapped));
        بازگشت !!::LockFileEx(handle, 0, 0, MAXDWORD, MAXDWORD, &overlapped);
    }
    منطقی unlock_shared()
    {
        بازگشت unlock();
    }

    HANDLE handle;

خصوصی:
    Impl(ثابت Impl&); // disabled
    Impl& operator=(ثابت Impl&); // disabled
};

#وگرنهاگر defined __linux__ || defined __APPLE__ || defined __HAIKU__ || defined __FreeBSD__ || defined __GNU__ || defined __EMSCRIPTEN__ || defined __QNX__ || defined __CYGWIN__

ساختار FileLock::Impl
{
    Impl(ثابت نویسه* fname)
    {
        handle = ::open(fname, O_RDWR);
        CV_Assert(handle != -1);
    }
    ~Impl()
    {
        اگر (handle >= 0)
            ::close(handle);
    }

    منطقی lock()
    {
        ساختار ::flock l;
        std::memset(&l, 0, اندازه(l));
        l.l_type = F_WRLCK;
        l.l_whence = SEEK_SET;
        l.l_start = 0;
        l.l_len = 0;
        DBG(std::cout << "Lock..." << std::endl);
        منطقی res = -1 != ::fcntl(handle, F_SETLKW, &l);
        بازگشت res;
    }
    منطقی unlock()
    {
        ساختار ::flock l;
        std::memset(&l, 0, اندازه(l));
        l.l_type = F_UNLCK;
        l.l_whence = SEEK_SET;
        l.l_start = 0;
        l.l_len = 0;
        DBG(std::cout << "Unlock..." << std::endl);
        منطقی res = -1 != ::fcntl(handle, F_SETLK, &l);
        بازگشت res;
    }

    منطقی lock_shared()
    {
        ساختار ::flock l;
        std::memset(&l, 0, اندازه(l));
        l.l_type = F_RDLCK;
        l.l_whence = SEEK_SET;
        l.l_start = 0;
        l.l_len = 0;
        DBG(std::cout << "Lock read..." << std::endl);
        منطقی res = -1 != ::fcntl(handle, F_SETLKW, &l);
        بازگشت res;
    }
    منطقی unlock_shared()
    {
        بازگشت unlock();
    }

    صحیح handle;

خصوصی:
    Impl(ثابت Impl&); // disabled
    Impl& operator=(ثابت Impl&); // disabled
};

#endif

FileLock::FileLock(ثابت نویسه* fname)
    : pImpl(جدید Impl(fname))
{
    // nothing
}
FileLock::~FileLock()
{
    delete pImpl;
    pImpl = پوچ;
}

تهی FileLock::lock() { CV_Assert(pImpl->lock()); }
تهی FileLock::unlock() { CV_Assert(pImpl->unlock()); }
تهی FileLock::lock_shared() { CV_Assert(pImpl->lock_shared()); }
تهی FileLock::unlock_shared() { CV_Assert(pImpl->unlock_shared()); }



cv::String getCacheDirectory(ثابت نویسه* sub_directory_name, ثابت نویسه* configuration_name)
{
    String cache_path;
    اگر (configuration_name)
    {
        cache_path = utils::getConfigurationParameterString(configuration_name, "");
    }
    اگر (cache_path.empty())
    {
        cv::String default_cache_path;
#ifdef _WIN32
        wchar_t tmp_path_buf_w[MAX_PATH+1] = {0};
        DWORD res = GetTempPathW(MAX_PATH, tmp_path_buf_w);
        اگر (res > 0 && res <= MAX_PATH)
        {
            // Convert from UTF-16 to UTF-8 to support Unicode paths
            صحیح len = WideCharToMultiByte(CP_UTF8, 0, tmp_path_buf_w, -1, پوچ, 0, پوچ, پوچ);
            اگر (len > 0)
            {
                std::vector<نویسه> utf8_buf(len);
                WideCharToMultiByte(CP_UTF8, 0, tmp_path_buf_w, -1, utf8_buf.data(), len, پوچ, پوچ);
                default_cache_path = std::string(utf8_buf.data());
            }
        }
#وگرنهاگر defined __ANDROID__
        // no defaults
#وگرنهاگر defined __APPLE__
        ثابت std::string tmpdir_env = utils::getConfigurationParameterString("TMPDIR");
        اگر (!tmpdir_env.empty() && utils::fs::isDirectory(tmpdir_env))
        {
            default_cache_path = tmpdir_env;
        }
        وگرنه
        {
            default_cache_path = "/tmp/";
            CV_LOG_WARNING(پوچ, "Using world accessible cache directory. This may be not secure: " << default_cache_path);
        }
#وگرنهاگر defined __linux__ || defined __HAIKU__ || defined __FreeBSD__ || defined __GNU__
        // https://specifications.freedesktop.org/basedir-spec/basedir-spec-latest.html
        اگر (default_cache_path.empty())
        {
            ثابت std::string xdg_cache_env = utils::getConfigurationParameterString("XDG_CACHE_HOME");
            اگر (!xdg_cache_env.empty() && utils::fs::isDirectory(xdg_cache_env))
            {
                default_cache_path = xdg_cache_env;
            }
        }
        اگر (default_cache_path.empty())
        {
            ثابت std::string home_env = utils::getConfigurationParameterString("HOME");
            اگر (!home_env.empty() && utils::fs::isDirectory(home_env))
            {
                cv::String home_path = home_env;
                cv::String home_cache_path = utils::fs::join(home_path, ".cache/");
                اگر (utils::fs::isDirectory(home_cache_path))
                {
                    default_cache_path = home_cache_path;
                }
            }
        }
        اگر (default_cache_path.empty())
        {
            ثابت نویسه* temp_path = "/var/tmp/";
            اگر (utils::fs::isDirectory(temp_path))
            {
                default_cache_path = temp_path;
                CV_LOG_WARNING(پوچ, "Using world accessible cache directory. This may be not secure: " << default_cache_path);
            }
        }
        اگر (default_cache_path.empty())
        {
            default_cache_path = "/tmp/";
            CV_LOG_WARNING(پوچ, "Using world accessible cache directory. This may be not secure: " << default_cache_path);
        }
#وگرنه
        // no defaults
#endif
        CV_LOG_VERBOSE(پوچ, 0, "default_cache_path = " << default_cache_path);
        اگر (!default_cache_path.empty())
        {
            اگر (utils::fs::isDirectory(default_cache_path))
            {
                cv::String default_cache_path_base = utils::fs::join(default_cache_path, "opencv");
                default_cache_path = utils::fs::join(default_cache_path_base, CVAUX_STR(CV_VERSION_MAJOR) ".x" CV_VERSION_STATUS);
                اگر (utils::getConfigurationParameterBool("OPENCV_CACHE_SHOW_CLEANUP_MESSAGE", درست)
                    && !utils::fs::isDirectory(default_cache_path))
                {
                    std::vector<cv::String> existedCacheDirs;
                    تلاش
                    {
                        utils::fs::glob_relative(default_cache_path_base, "*", existedCacheDirs, نادرست, درست);
                    }
                    بگیر (...)
                    {
                        // ignore
                    }
                    اگر (!existedCacheDirs.empty())
                    {
                        CV_LOG_WARNING(پوچ, "Creating new OpenCV cache directory: " << default_cache_path);
                        CV_LOG_WARNING(پوچ, "There are several neighbour directories, probably created by old OpenCV versions.");
                        CV_LOG_WARNING(پوچ, "Feel free to cleanup these unused directories:");
                        برای (size_t i = 0; i < existedCacheDirs.size(); i++)
                        {
                            CV_LOG_WARNING(پوچ, "  - " << existedCacheDirs[i]);
                        }
                        CV_LOG_WARNING(پوچ, "Note: This message is showed only once.");
                    }
                }
                اگر (sub_directory_name && sub_directory_name[0] != '\0')
                    default_cache_path = utils::fs::join(default_cache_path, cv::String(sub_directory_name) + native_separator);
                اگر (!utils::fs::createDirectories(default_cache_path))
                {
                    CV_LOG_DEBUG(پوچ, "Can't create OpenCV cache sub-directory: " << default_cache_path);
                }
                وگرنه
                {
                    cache_path = default_cache_path;
                }
            }
            وگرنه
            {
                CV_LOG_INFO(پوچ, "Can't find default cache directory (does it exist?): " << default_cache_path);
            }
        }
        وگرنه
        {
            CV_LOG_DEBUG(پوچ, "OpenCV has no support to discover default cache directory on the current platform");
        }
    }
    وگرنه
    {
        اگر (cache_path == "disabled")
            بازگشت cache_path;
        اگر (!isDirectory(cache_path))
        {
            CV_LOG_WARNING(پوچ, "Specified non-existed directory, creating OpenCV sub-directory for caching purposes: " << cache_path);
            اگر (!createDirectories(cache_path))
            {
                CV_LOG_ERROR(پوچ, "Can't create OpenCV cache sub-directory: " << cache_path);
                cache_path.clear();
            }
        }
    }
    CV_Assert(cache_path.empty() || utils::fs::isDirectory(cache_path));
    اگر (!cache_path.empty())
    {
        اگر (!isPathSeparator(cache_path[cache_path.size() - 1]))
        {
            cache_path += native_separator;
        }
    }
    بازگشت cache_path;
}

#وگرنه
#define NOT_IMPLEMENTED CV_Error(Error::StsNotImplemented, "");
cv::String canonical(ثابت cv::String& /*path*/) { NOT_IMPLEMENTED }
منطقی exists(ثابت cv::String& /*path*/) { NOT_IMPLEMENTED }
تهی remove_all(ثابت cv::String& /*path*/) { NOT_IMPLEMENTED }
cv::String getcwd() { NOT_IMPLEMENTED }
منطقی createDirectory(ثابت cv::String& /*path*/) { NOT_IMPLEMENTED }
منطقی createDirectories(ثابت cv::String& /*path*/) { NOT_IMPLEMENTED }
cv::String getCacheDirectory(ثابت نویسه* /*sub_directory_name*/, ثابت نویسه* /*configuration_name = NULL*/) { NOT_IMPLEMENTED }
#undef NOT_IMPLEMENTED
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT

}}} // namespace


#اگر OPENCV_HAVE_FILESYSTEM_SUPPORT
#شامل "plugin_loader.impl.hpp"
#endif
