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
#אחרת
#define DBG(...)
#endif


#אם OPENCV_HAVE_FILESYSTEM_SUPPORT

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
#אחרת
ساكن ثابت حرف native_separator = '/';
#endif

ساكن مضمن
منطقي isPathSeparator(حرف c)
{
    החזר c == '/' || c == '\\';
}

cv::String join(ثابت cv::String& base, ثابت cv::String& path)
{
    אם (base.empty())
        החזר path;
    אם (path.empty())
        החזר base;

    منطقي baseSep = isPathSeparator(base[base.size() - 1]);
    منطقي pathSep = isPathSeparator(path[0]);
    String result;
    אם (baseSep && pathSep)
    {
        result = base + path.substr(1);
    }
    אחרת אם (!baseSep && !pathSep)
    {
        result = base + native_separator + path;
    }
    אחרת
    {
        result = base + path;
    }
    החזר result;
}

CV_EXPORTS cv::String getParent(ثابت cv::String &path)
{
    std::string::size_type loc = path.find_last_of("/\\");
    אם (loc == std::string::npos)
        החזר std::string();
    החזר std::string(path, 0, loc);
}

CV_EXPORTS std::wstring getParent(ثابت std::wstring& path)
{
    std::wstring::size_type loc = path.find_last_of(L"/\\");
    אם (loc == std::wstring::npos)
        החזר std::wstring();
    החזר std::wstring(path, 0, loc);
}

#אם OPENCV_HAVE_FILESYSTEM_SUPPORT

cv::String canonical(ثابت cv::String& path)
{
    cv::String result;
#ifdef _WIN32
    ثابت حرف* result_str = _fullpath(عدم, path.c_str(), 0);
#אחרת
    ثابت حرف* result_str = realpath(path.c_str(), عدم);
#endif
    אם (result_str)
    {
        result = cv::String(result_str);
        حرر((فراغ*)result_str);
    }
    החזר result.empty() ? path : result;
}


منطقي exists(ثابت cv::String& path)
{
    CV_INSTRUMENT_REGION();

#אם defined _WIN32 || defined WINCE
    BOOL status = TRUE;
    {
        WIN32_FILE_ATTRIBUTE_DATA all_attrs;
#ifdef WINRT
        wchar_t wpath[MAX_PATH];
        size_t copied = mbstowcs(wpath, path.c_str(), MAX_PATH);
        CV_Assert((copied != MAX_PATH) && (copied != (size_t)-1));
        status = ::GetFileAttributesExW(wpath, GetFileExInfoStandard, &all_attrs);
#אחרת
        status = ::GetFileAttributesExA(path.c_str(), GetFileExInfoStandard, &all_attrs);
#endif
    }

    החזר !!status;
#אחרת
    بنية stat stat_buf;
    החזר (0 == stat(path.c_str(), &stat_buf));
#endif
}

CV_EXPORTS فراغ remove_all(ثابت cv::String& path)
{
    אם (!exists(path))
        החזר;
    אם (isDirectory(path))
    {
        std::vector<String> entries;
        utils::fs::glob(path, cv::String(), entries, שקר, אמת);
        עבור (size_t i = 0; i < entries.size(); i++)
        {
            ثابت String& e = entries[i];
            remove_all(e);
        }
#ifdef _MSC_VER
        منطقي result = _rmdir(path.c_str()) == 0;
#אחרת
        منطقي result = rmdir(path.c_str()) == 0;
#endif
        אם (!result)
        {
            CV_LOG_ERROR(عدم, "Can't remove directory: " << path);
        }
    }
    אחרת
    {
#ifdef _MSC_VER
        منطقي result = _unlink(path.c_str()) == 0;
#אחרת
        منطقي result = unlink(path.c_str()) == 0;
#endif
        אם (!result)
        {
            CV_LOG_ERROR(عدم, "Can't remove file: " << path);
        }
    }
}


cv::String getcwd()
{
    CV_INSTRUMENT_REGION();
    cv::AutoBuffer<حرف, 4096> buf;
#אם defined WIN32 || defined _WIN32 || defined WINCE
#ifdef WINRT
    החזר cv::String();
#אחרת
    DWORD sz = GetCurrentDirectoryA(0, عدم);
    buf.allocate((size_t)sz);
    sz = GetCurrentDirectoryA((DWORD)buf.size(), buf.data());
    החזר cv::String(buf.data(), (size_t)sz);
#endif
#وإلاإذا defined __linux__ || defined __APPLE__ || defined __HAIKU__ || defined __FreeBSD__ || defined __EMSCRIPTEN__ || defined __QNX__ || defined __CYGWIN__
    עבור(;;)
    {
        حرف* p = ::getcwd(buf.data(), buf.size());
        אם (p == عدم)
        {
            אם (errno == ERANGE)
            {
                buf.allocate(buf.size() * 2);
                استمر;
            }
            החזר cv::String();
        }
        اكسر;
    }
    החזר cv::String(buf.data(), (size_t)strlen(buf.data()));
#אחרת
    החזר cv::String();
#endif
}


منطقي createDirectory(ثابت cv::String& path)
{
    CV_INSTRUMENT_REGION();
#אם defined WIN32 || defined _WIN32 || defined WINCE
#ifdef WINRT
    wchar_t wpath[MAX_PATH];
    size_t copied = mbstowcs(wpath, path.c_str(), MAX_PATH);
    CV_Assert((copied != MAX_PATH) && (copied != (size_t)-1));
    صحيح result = CreateDirectoryA(wpath, عدم) ? 0 : -1;
#אחרת
    صحيح result = _mkdir(path.c_str());
#endif
#وإلاإذا defined __linux__ || defined __APPLE__ || defined __HAIKU__ || defined __FreeBSD__ || defined __EMSCRIPTEN__ || defined __QNX__ || defined __CYGWIN__
    صحيح result = mkdir(path.c_str(), 0777);
#אחרת
    صحيح result = -1;
#endif

    אם (result == -1)
    {
        החזר isDirectory(path);
    }
    החזר אמת;
}

منطقي createDirectories(ثابت cv::String& path_)
{
    cv::String path = path_;
    עבור (;;)
    {
        حرف last_char = path.empty() ? 0 : path[path.length() - 1];
        אם (isPathSeparator(last_char))
        {
            path = path.substr(0, path.length() - 1);
            استمر;
        }
        اكسر;
    }

    אם (path.empty() || path == "./" || path == ".\\" || path == ".")
        החזר אמת;
    אם (isDirectory(path))
        החזר אמת;

    size_t pos = path.rfind('/');
    אם (pos == cv::String::npos)
        pos = path.rfind('\\');
    אם (pos != cv::String::npos)
    {
        cv::String parent_directory = path.substr(0, pos);
        אם (!parent_directory.empty())
        {
            אם (!createDirectories(parent_directory))
                החזר שקר;
        }
    }

    החזר createDirectory(path);
}

#ifdef _WIN32

بنية FileLock::Impl
{
    Impl(ثابت حرف* fname)
    {
        // http://support.microsoft.com/kb/316609
        صحيح numRetries = 5;
        עשה
        {
            handle = ::CreateFileA(fname, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, عدم,
                                OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, عدم);
            אם (INVALID_HANDLE_VALUE == handle)
            {
                אם (ERROR_SHARING_VIOLATION == GetLastError())
                {
                    numRetries--;
                    Sleep(250);
                    استمر;
                }
                אחרת
                {
                    CV_Error_(Error::StsAssert, ("Can't open lock file: %s", fname));
                }
            }
            اكسر;
        } כלעוד (numRetries > 0);
    }
    ~Impl()
    {
        אם (INVALID_HANDLE_VALUE != handle)
        {
            ::CloseHandle(handle);
        }
    }

    منطقي lock()
    {
        OVERLAPPED overlapped;
        std::memset(&overlapped, 0, حجم(overlapped));
        החזר !!::LockFileEx(handle, LOCKFILE_EXCLUSIVE_LOCK, 0, MAXDWORD, MAXDWORD, &overlapped);
    }
    منطقي unlock()
    {
        OVERLAPPED overlapped;
        std::memset(&overlapped, 0, حجم(overlapped));
        החזר !!::UnlockFileEx(handle, 0, MAXDWORD, MAXDWORD, &overlapped);
    }

    منطقي lock_shared()
    {
        OVERLAPPED overlapped;
        std::memset(&overlapped, 0, حجم(overlapped));
        החזר !!::LockFileEx(handle, 0, 0, MAXDWORD, MAXDWORD, &overlapped);
    }
    منطقي unlock_shared()
    {
        החזר unlock();
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
        אם (handle >= 0)
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
        החזר res;
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
        החזר res;
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
        החזר res;
    }
    منطقي unlock_shared()
    {
        החזר unlock();
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
    אם (configuration_name)
    {
        cache_path = utils::getConfigurationParameterString(configuration_name, "");
    }
    אם (cache_path.empty())
    {
        cv::String default_cache_path;
#ifdef _WIN32
        wchar_t tmp_path_buf_w[MAX_PATH+1] = {0};
        DWORD res = GetTempPathW(MAX_PATH, tmp_path_buf_w);
        אם (res > 0 && res <= MAX_PATH)
        {
            // Convert from UTF-16 to UTF-8 to support Unicode paths
            صحيح len = WideCharToMultiByte(CP_UTF8, 0, tmp_path_buf_w, -1, عدم, 0, عدم, عدم);
            אם (len > 0)
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
        אם (!tmpdir_env.empty() && utils::fs::isDirectory(tmpdir_env))
        {
            default_cache_path = tmpdir_env;
        }
        אחרת
        {
            default_cache_path = "/tmp/";
            CV_LOG_WARNING(عدم, "Using world accessible cache directory. This may be not secure: " << default_cache_path);
        }
#وإلاإذا defined __linux__ || defined __HAIKU__ || defined __FreeBSD__ || defined __GNU__
        // https://specifications.freedesktop.org/basedir-spec/basedir-spec-latest.html
        אם (default_cache_path.empty())
        {
            ثابت std::string xdg_cache_env = utils::getConfigurationParameterString("XDG_CACHE_HOME");
            אם (!xdg_cache_env.empty() && utils::fs::isDirectory(xdg_cache_env))
            {
                default_cache_path = xdg_cache_env;
            }
        }
        אם (default_cache_path.empty())
        {
            ثابت std::string home_env = utils::getConfigurationParameterString("HOME");
            אם (!home_env.empty() && utils::fs::isDirectory(home_env))
            {
                cv::String home_path = home_env;
                cv::String home_cache_path = utils::fs::join(home_path, ".cache/");
                אם (utils::fs::isDirectory(home_cache_path))
                {
                    default_cache_path = home_cache_path;
                }
            }
        }
        אם (default_cache_path.empty())
        {
            ثابت حرف* temp_path = "/var/tmp/";
            אם (utils::fs::isDirectory(temp_path))
            {
                default_cache_path = temp_path;
                CV_LOG_WARNING(عدم, "Using world accessible cache directory. This may be not secure: " << default_cache_path);
            }
        }
        אם (default_cache_path.empty())
        {
            default_cache_path = "/tmp/";
            CV_LOG_WARNING(عدم, "Using world accessible cache directory. This may be not secure: " << default_cache_path);
        }
#אחרת
        // no defaults
#endif
        CV_LOG_VERBOSE(عدم, 0, "default_cache_path = " << default_cache_path);
        אם (!default_cache_path.empty())
        {
            אם (utils::fs::isDirectory(default_cache_path))
            {
                cv::String default_cache_path_base = utils::fs::join(default_cache_path, "opencv");
                default_cache_path = utils::fs::join(default_cache_path_base, CVAUX_STR(CV_VERSION_MAJOR) ".x" CV_VERSION_STATUS);
                אם (utils::getConfigurationParameterBool("OPENCV_CACHE_SHOW_CLEANUP_MESSAGE", אמת)
                    && !utils::fs::isDirectory(default_cache_path))
                {
                    std::vector<cv::String> existedCacheDirs;
                    حاول
                    {
                        utils::fs::glob_relative(default_cache_path_base, "*", existedCacheDirs, שקר, אמת);
                    }
                    امسك (...)
                    {
                        // ignore
                    }
                    אם (!existedCacheDirs.empty())
                    {
                        CV_LOG_WARNING(عدم, "Creating new OpenCV cache directory: " << default_cache_path);
                        CV_LOG_WARNING(عدم, "There are several neighbour directories, probably created by old OpenCV versions.");
                        CV_LOG_WARNING(عدم, "Feel free to cleanup these unused directories:");
                        עבור (size_t i = 0; i < existedCacheDirs.size(); i++)
                        {
                            CV_LOG_WARNING(عدم, "  - " << existedCacheDirs[i]);
                        }
                        CV_LOG_WARNING(عدم, "Note: This message is showed only once.");
                    }
                }
                אם (sub_directory_name && sub_directory_name[0] != '\0')
                    default_cache_path = utils::fs::join(default_cache_path, cv::String(sub_directory_name) + native_separator);
                אם (!utils::fs::createDirectories(default_cache_path))
                {
                    CV_LOG_DEBUG(عدم, "Can't create OpenCV cache sub-directory: " << default_cache_path);
                }
                אחרת
                {
                    cache_path = default_cache_path;
                }
            }
            אחרת
            {
                CV_LOG_INFO(عدم, "Can't find default cache directory (does it exist?): " << default_cache_path);
            }
        }
        אחרת
        {
            CV_LOG_DEBUG(عدم, "OpenCV has no support to discover default cache directory on the current platform");
        }
    }
    אחרת
    {
        אם (cache_path == "disabled")
            החזר cache_path;
        אם (!isDirectory(cache_path))
        {
            CV_LOG_WARNING(عدم, "Specified non-existed directory, creating OpenCV sub-directory for caching purposes: " << cache_path);
            אם (!createDirectories(cache_path))
            {
                CV_LOG_ERROR(عدم, "Can't create OpenCV cache sub-directory: " << cache_path);
                cache_path.clear();
            }
        }
    }
    CV_Assert(cache_path.empty() || utils::fs::isDirectory(cache_path));
    אם (!cache_path.empty())
    {
        אם (!isPathSeparator(cache_path[cache_path.size() - 1]))
        {
            cache_path += native_separator;
        }
    }
    החזר cache_path;
}

#אחרת
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


#אם OPENCV_HAVE_FILESYSTEM_SUPPORT
#ضمن "plugin_loader.impl.hpp"
#endif
