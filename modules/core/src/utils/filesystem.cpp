// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ضمن "../precomp.hpp"

#ضمن <opencv2/core/utils/configuration.private.hpp>

#ضمن <opencv2/core/utils/logger.hpp>

#ضمن "opencv2/core/utils/filesystem.private.hpp"
#ضمن "opencv2/core/utils/filesystem.hpp"

//#define DEBUG_FS_UTILS

#ifdef DEBUG_FS_UTILS
#ضمن <iostream>
#define DBG(...) __VA_ARGS__
#ܐܚܪܢܐ
#define DBG(...)
#endif


#ܐܢ OPENCV_HAVE_FILESYSTEM_SUPPORT

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#undef NOMINMAX
#define NOMINMAX
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
#endif

#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT

نطاق cv { نطاق utils { نطاق fs {

#ifdef _WIN32
ساكن ثابت حرف native_separator = '\\';
#ܐܚܪܢܐ
ساكن ثابت حرف native_separator = '/';
#endif

ساكن مضمن
منطقي isPathSeparator(حرف c)
{
    ܦܢܐ c == '/' || c == '\\';
}

cv::String join(ثابت cv::String& base, ثابت cv::String& path)
{
    ܐܢ (base.empty())
        ܦܢܐ path;
    ܐܢ (path.empty())
        ܦܢܐ base;

    منطقي baseSep = isPathSeparator(base[base.size() - 1]);
    منطقي pathSep = isPathSeparator(path[0]);
    String result;
    ܐܢ (baseSep && pathSep)
    {
        result = base + path.substr(1);
    }
    ܐܚܪܢܐ ܐܢ (!baseSep && !pathSep)
    {
        result = base + native_separator + path;
    }
    ܐܚܪܢܐ
    {
        result = base + path;
    }
    ܦܢܐ result;
}

CV_EXPORTS cv::String getParent(ثابت cv::String &path)
{
    std::string::size_type loc = path.find_last_of("/\\");
    ܐܢ (loc == std::string::npos)
        ܦܢܐ std::string();
    ܦܢܐ std::string(path, 0, loc);
}

CV_EXPORTS std::wstring getParent(ثابت std::wstring& path)
{
    std::wstring::size_type loc = path.find_last_of(L"/\\");
    ܐܢ (loc == std::wstring::npos)
        ܦܢܐ std::wstring();
    ܦܢܐ std::wstring(path, 0, loc);
}

#ܐܢ OPENCV_HAVE_FILESYSTEM_SUPPORT

cv::String canonical(ثابت cv::String& path)
{
    cv::String result;
#ifdef _WIN32
    ثابت حرف* result_str = _fullpath(عدم, path.c_str(), 0);
#ܐܚܪܢܐ
    ثابت حرف* result_str = realpath(path.c_str(), عدم);
#endif
    ܐܢ (result_str)
    {
        result = cv::String(result_str);
        حرر((فراغ*)result_str);
    }
    ܦܢܐ result.empty() ? path : result;
}


منطقي exists(ثابت cv::String& path)
{
    CV_INSTRUMENT_REGION();

#ܐܢ defined _WIN32 || defined WINCE
    BOOL status = TRUE;
    {
        WIN32_FILE_ATTRIBUTE_DATA all_attrs;
#ifdef WINRT
        wchar_t wpath[MAX_PATH];
        size_t copied = mbstowcs(wpath, path.c_str(), MAX_PATH);
        CV_Assert((copied != MAX_PATH) && (copied != (size_t)-1));
        status = ::GetFileAttributesExW(wpath, GetFileExInfoStandard, &all_attrs);
#ܐܚܪܢܐ
        status = ::GetFileAttributesExA(path.c_str(), GetFileExInfoStandard, &all_attrs);
#endif
    }

    ܦܢܐ !!status;
#ܐܚܪܢܐ
    بنية stat stat_buf;
    ܦܢܐ (0 == stat(path.c_str(), &stat_buf));
#endif
}

CV_EXPORTS فراغ remove_all(ثابت cv::String& path)
{
    ܐܢ (!exists(path))
        ܦܢܐ;
    ܐܢ (isDirectory(path))
    {
        std::vector<String> entries;
        utils::fs::glob(path, cv::String(), entries, ܕܓܠܐ, ܫܪܪܐ);
        لكل (size_t i = 0; i < entries.size(); i++)
        {
            ثابت String& e = entries[i];
            remove_all(e);
        }
#ifdef _MSC_VER
        منطقي result = _rmdir(path.c_str()) == 0;
#ܐܚܪܢܐ
        منطقي result = rmdir(path.c_str()) == 0;
#endif
        ܐܢ (!result)
        {
            CV_LOG_ERROR(عدم, "Can't remove directory: " << path);
        }
    }
    ܐܚܪܢܐ
    {
#ifdef _MSC_VER
        منطقي result = _unlink(path.c_str()) == 0;
#ܐܚܪܢܐ
        منطقي result = unlink(path.c_str()) == 0;
#endif
        ܐܢ (!result)
        {
            CV_LOG_ERROR(عدم, "Can't remove file: " << path);
        }
    }
}


cv::String getcwd()
{
    CV_INSTRUMENT_REGION();
    cv::AutoBuffer<حرف, 4096> buf;
#ܐܢ defined WIN32 || defined _WIN32 || defined WINCE
#ifdef WINRT
    ܦܢܐ cv::String();
#ܐܚܪܢܐ
    DWORD sz = GetCurrentDirectoryA(0, عدم);
    buf.allocate((size_t)sz);
    sz = GetCurrentDirectoryA((DWORD)buf.size(), buf.data());
    ܦܢܐ cv::String(buf.data(), (size_t)sz);
#endif
#وإلاإذا defined __linux__ || defined __APPLE__ || defined __HAIKU__ || defined __FreeBSD__ || defined __EMSCRIPTEN__ || defined __QNX__ || defined __CYGWIN__
    لكل(;;)
    {
        حرف* p = ::getcwd(buf.data(), buf.size());
        ܐܢ (p == عدم)
        {
            ܐܢ (errno == ERANGE)
            {
                buf.allocate(buf.size() * 2);
                استمر;
            }
            ܦܢܐ cv::String();
        }
        اكسر;
    }
    ܦܢܐ cv::String(buf.data(), (size_t)strlen(buf.data()));
#ܐܚܪܢܐ
    ܦܢܐ cv::String();
#endif
}


منطقي createDirectory(ثابت cv::String& path)
{
    CV_INSTRUMENT_REGION();
#ܐܢ defined WIN32 || defined _WIN32 || defined WINCE
#ifdef WINRT
    wchar_t wpath[MAX_PATH];
    size_t copied = mbstowcs(wpath, path.c_str(), MAX_PATH);
    CV_Assert((copied != MAX_PATH) && (copied != (size_t)-1));
    صحيح result = CreateDirectoryA(wpath, عدم) ? 0 : -1;
#ܐܚܪܢܐ
    صحيح result = _mkdir(path.c_str());
#endif
#وإلاإذا defined __linux__ || defined __APPLE__ || defined __HAIKU__ || defined __FreeBSD__ || defined __EMSCRIPTEN__ || defined __QNX__ || defined __CYGWIN__
    صحيح result = mkdir(path.c_str(), 0777);
#ܐܚܪܢܐ
    صحيح result = -1;
#endif

    ܐܢ (result == -1)
    {
        ܦܢܐ isDirectory(path);
    }
    ܦܢܐ ܫܪܪܐ;
}

منطقي createDirectories(ثابت cv::String& path_)
{
    cv::String path = path_;
    لكل (;;)
    {
        حرف last_char = path.empty() ? 0 : path[path.length() - 1];
        ܐܢ (isPathSeparator(last_char))
        {
            path = path.substr(0, path.length() - 1);
            استمر;
        }
        اكسر;
    }

    ܐܢ (path.empty() || path == "./" || path == ".\\" || path == ".")
        ܦܢܐ ܫܪܪܐ;
    ܐܢ (isDirectory(path))
        ܦܢܐ ܫܪܪܐ;

    size_t pos = path.rfind('/');
    ܐܢ (pos == cv::String::npos)
        pos = path.rfind('\\');
    ܐܢ (pos != cv::String::npos)
    {
        cv::String parent_directory = path.substr(0, pos);
        ܐܢ (!parent_directory.empty())
        {
            ܐܢ (!createDirectories(parent_directory))
                ܦܢܐ ܕܓܠܐ;
        }
    }

    ܦܢܐ createDirectory(path);
}

#ifdef _WIN32

بنية FileLock::Impl
{
    Impl(ثابت حرف* fname)
    {
        // http://support.microsoft.com/kb/316609
        صحيح numRetries = 5;
        ܥܒܕ
        {
            handle = ::CreateFileA(fname, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, عدم,
                                OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, عدم);
            ܐܢ (INVALID_HANDLE_VALUE == handle)
            {
                ܐܢ (ERROR_SHARING_VIOLATION == GetLastError())
                {
                    numRetries--;
                    Sleep(250);
                    استمر;
                }
                ܐܚܪܢܐ
                {
                    CV_Error_(Error::StsAssert, ("Can't open lock file: %s", fname));
                }
            }
            اكسر;
        } ܟܡܐ (numRetries > 0);
    }
    ~Impl()
    {
        ܐܢ (INVALID_HANDLE_VALUE != handle)
        {
            ::CloseHandle(handle);
        }
    }

    منطقي lock()
    {
        OVERLAPPED overlapped;
        std::memset(&overlapped, 0, حجم(overlapped));
        ܦܢܐ !!::LockFileEx(handle, LOCKFILE_EXCLUSIVE_LOCK, 0, MAXDWORD, MAXDWORD, &overlapped);
    }
    منطقي unlock()
    {
        OVERLAPPED overlapped;
        std::memset(&overlapped, 0, حجم(overlapped));
        ܦܢܐ !!::UnlockFileEx(handle, 0, MAXDWORD, MAXDWORD, &overlapped);
    }

    منطقي lock_shared()
    {
        OVERLAPPED overlapped;
        std::memset(&overlapped, 0, حجم(overlapped));
        ܦܢܐ !!::LockFileEx(handle, 0, 0, MAXDWORD, MAXDWORD, &overlapped);
    }
    منطقي unlock_shared()
    {
        ܦܢܐ unlock();
    }

    HANDLE handle;

خاص:
    Impl(ثابت Impl&); // disabled
    Impl& operator=(ثابت Impl&); // disabled
};

#وإلاإذا defined __linux__ || defined __APPLE__ || defined __HAIKU__ || defined __FreeBSD__ || defined __GNU__ || defined __EMSCRIPTEN__ || defined __QNX__ || defined __CYGWIN__

بنية FileLock::Impl
{
    Impl(ثابت حرف* fname)
    {
        handle = ::open(fname, O_RDWR);
        CV_Assert(handle != -1);
    }
    ~Impl()
    {
        ܐܢ (handle >= 0)
            ::close(handle);
    }

    منطقي lock()
    {
        بنية ::flock l;
        std::memset(&l, 0, حجم(l));
        l.l_type = F_WRLCK;
        l.l_whence = SEEK_SET;
        l.l_start = 0;
        l.l_len = 0;
        DBG(std::cout << "Lock..." << std::endl);
        منطقي res = -1 != ::fcntl(handle, F_SETLKW, &l);
        ܦܢܐ res;
    }
    منطقي unlock()
    {
        بنية ::flock l;
        std::memset(&l, 0, حجم(l));
        l.l_type = F_UNLCK;
        l.l_whence = SEEK_SET;
        l.l_start = 0;
        l.l_len = 0;
        DBG(std::cout << "Unlock..." << std::endl);
        منطقي res = -1 != ::fcntl(handle, F_SETLK, &l);
        ܦܢܐ res;
    }

    منطقي lock_shared()
    {
        بنية ::flock l;
        std::memset(&l, 0, حجم(l));
        l.l_type = F_RDLCK;
        l.l_whence = SEEK_SET;
        l.l_start = 0;
        l.l_len = 0;
        DBG(std::cout << "Lock read..." << std::endl);
        منطقي res = -1 != ::fcntl(handle, F_SETLKW, &l);
        ܦܢܐ res;
    }
    منطقي unlock_shared()
    {
        ܦܢܐ unlock();
    }

    صحيح handle;

خاص:
    Impl(ثابت Impl&); // disabled
    Impl& operator=(ثابت Impl&); // disabled
};

#endif

FileLock::FileLock(ثابت حرف* fname)
    : pImpl(جديد Impl(fname))
{
    // nothing
}
FileLock::~FileLock()
{
    delete pImpl;
    pImpl = عدم;
}

فراغ FileLock::lock() { CV_Assert(pImpl->lock()); }
فراغ FileLock::unlock() { CV_Assert(pImpl->unlock()); }
فراغ FileLock::lock_shared() { CV_Assert(pImpl->lock_shared()); }
فراغ FileLock::unlock_shared() { CV_Assert(pImpl->unlock_shared()); }



cv::String getCacheDirectory(ثابت حرف* sub_directory_name, ثابت حرف* configuration_name)
{
    String cache_path;
    ܐܢ (configuration_name)
    {
        cache_path = utils::getConfigurationParameterString(configuration_name, "");
    }
    ܐܢ (cache_path.empty())
    {
        cv::String default_cache_path;
#ifdef _WIN32
        wchar_t tmp_path_buf_w[MAX_PATH+1] = {0};
        DWORD res = GetTempPathW(MAX_PATH, tmp_path_buf_w);
        ܐܢ (res > 0 && res <= MAX_PATH)
        {
            // Convert from UTF-16 to UTF-8 to support Unicode paths
            صحيح len = WideCharToMultiByte(CP_UTF8, 0, tmp_path_buf_w, -1, عدم, 0, عدم, عدم);
            ܐܢ (len > 0)
            {
                std::vector<حرف> utf8_buf(len);
                WideCharToMultiByte(CP_UTF8, 0, tmp_path_buf_w, -1, utf8_buf.data(), len, عدم, عدم);
                default_cache_path = std::string(utf8_buf.data());
            }
        }
#وإلاإذا defined __ANDROID__
        // no defaults
#وإلاإذا defined __APPLE__
        ثابت std::string tmpdir_env = utils::getConfigurationParameterString("TMPDIR");
        ܐܢ (!tmpdir_env.empty() && utils::fs::isDirectory(tmpdir_env))
        {
            default_cache_path = tmpdir_env;
        }
        ܐܚܪܢܐ
        {
            default_cache_path = "/tmp/";
            CV_LOG_WARNING(عدم, "Using world accessible cache directory. This may be not secure: " << default_cache_path);
        }
#وإلاإذا defined __linux__ || defined __HAIKU__ || defined __FreeBSD__ || defined __GNU__
        // https://specifications.freedesktop.org/basedir-spec/basedir-spec-latest.html
        ܐܢ (default_cache_path.empty())
        {
            ثابت std::string xdg_cache_env = utils::getConfigurationParameterString("XDG_CACHE_HOME");
            ܐܢ (!xdg_cache_env.empty() && utils::fs::isDirectory(xdg_cache_env))
            {
                default_cache_path = xdg_cache_env;
            }
        }
        ܐܢ (default_cache_path.empty())
        {
            ثابت std::string home_env = utils::getConfigurationParameterString("HOME");
            ܐܢ (!home_env.empty() && utils::fs::isDirectory(home_env))
            {
                cv::String home_path = home_env;
                cv::String home_cache_path = utils::fs::join(home_path, ".cache/");
                ܐܢ (utils::fs::isDirectory(home_cache_path))
                {
                    default_cache_path = home_cache_path;
                }
            }
        }
        ܐܢ (default_cache_path.empty())
        {
            ثابت حرف* temp_path = "/var/tmp/";
            ܐܢ (utils::fs::isDirectory(temp_path))
            {
                default_cache_path = temp_path;
                CV_LOG_WARNING(عدم, "Using world accessible cache directory. This may be not secure: " << default_cache_path);
            }
        }
        ܐܢ (default_cache_path.empty())
        {
            default_cache_path = "/tmp/";
            CV_LOG_WARNING(عدم, "Using world accessible cache directory. This may be not secure: " << default_cache_path);
        }
#ܐܚܪܢܐ
        // no defaults
#endif
        CV_LOG_VERBOSE(عدم, 0, "default_cache_path = " << default_cache_path);
        ܐܢ (!default_cache_path.empty())
        {
            ܐܢ (utils::fs::isDirectory(default_cache_path))
            {
                cv::String default_cache_path_base = utils::fs::join(default_cache_path, "opencv");
                default_cache_path = utils::fs::join(default_cache_path_base, CVAUX_STR(CV_VERSION_MAJOR) ".x" CV_VERSION_STATUS);
                ܐܢ (utils::getConfigurationParameterBool("OPENCV_CACHE_SHOW_CLEANUP_MESSAGE", ܫܪܪܐ)
                    && !utils::fs::isDirectory(default_cache_path))
                {
                    std::vector<cv::String> existedCacheDirs;
                    حاول
                    {
                        utils::fs::glob_relative(default_cache_path_base, "*", existedCacheDirs, ܕܓܠܐ, ܫܪܪܐ);
                    }
                    امسك (...)
                    {
                        // ignore
                    }
                    ܐܢ (!existedCacheDirs.empty())
                    {
                        CV_LOG_WARNING(عدم, "Creating new OpenCV cache directory: " << default_cache_path);
                        CV_LOG_WARNING(عدم, "There are several neighbour directories, probably created by old OpenCV versions.");
                        CV_LOG_WARNING(عدم, "Feel free to cleanup these unused directories:");
                        لكل (size_t i = 0; i < existedCacheDirs.size(); i++)
                        {
                            CV_LOG_WARNING(عدم, "  - " << existedCacheDirs[i]);
                        }
                        CV_LOG_WARNING(عدم, "Note: This message is showed only once.");
                    }
                }
                ܐܢ (sub_directory_name && sub_directory_name[0] != '\0')
                    default_cache_path = utils::fs::join(default_cache_path, cv::String(sub_directory_name) + native_separator);
                ܐܢ (!utils::fs::createDirectories(default_cache_path))
                {
                    CV_LOG_DEBUG(عدم, "Can't create OpenCV cache sub-directory: " << default_cache_path);
                }
                ܐܚܪܢܐ
                {
                    cache_path = default_cache_path;
                }
            }
            ܐܚܪܢܐ
            {
                CV_LOG_INFO(عدم, "Can't find default cache directory (does it exist?): " << default_cache_path);
            }
        }
        ܐܚܪܢܐ
        {
            CV_LOG_DEBUG(عدم, "OpenCV has no support to discover default cache directory on the current platform");
        }
    }
    ܐܚܪܢܐ
    {
        ܐܢ (cache_path == "disabled")
            ܦܢܐ cache_path;
        ܐܢ (!isDirectory(cache_path))
        {
            CV_LOG_WARNING(عدم, "Specified non-existed directory, creating OpenCV sub-directory for caching purposes: " << cache_path);
            ܐܢ (!createDirectories(cache_path))
            {
                CV_LOG_ERROR(عدم, "Can't create OpenCV cache sub-directory: " << cache_path);
                cache_path.clear();
            }
        }
    }
    CV_Assert(cache_path.empty() || utils::fs::isDirectory(cache_path));
    ܐܢ (!cache_path.empty())
    {
        ܐܢ (!isPathSeparator(cache_path[cache_path.size() - 1]))
        {
            cache_path += native_separator;
        }
    }
    ܦܢܐ cache_path;
}

#ܐܚܪܢܐ
#define NOT_IMPLEMENTED CV_Error(Error::StsNotImplemented, "");
cv::String canonical(ثابت cv::String& /*path*/) { NOT_IMPLEMENTED }
منطقي exists(ثابت cv::String& /*path*/) { NOT_IMPLEMENTED }
فراغ remove_all(ثابت cv::String& /*path*/) { NOT_IMPLEMENTED }
cv::String getcwd() { NOT_IMPLEMENTED }
منطقي createDirectory(ثابت cv::String& /*path*/) { NOT_IMPLEMENTED }
منطقي createDirectories(ثابت cv::String& /*path*/) { NOT_IMPLEMENTED }
cv::String getCacheDirectory(ثابت حرف* /*sub_directory_name*/, ثابت حرف* /*configuration_name = NULL*/) { NOT_IMPLEMENTED }
#undef NOT_IMPLEMENTED
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT

}}} // namespace


#ܐܢ OPENCV_HAVE_FILESYSTEM_SUPPORT
#ضمن "plugin_loader.impl.hpp"
#endif
