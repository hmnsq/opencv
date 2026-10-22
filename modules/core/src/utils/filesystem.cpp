// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#അന്തര്ഭാവ "../precomp.hpp"

#അന്തര്ഭാവ <opencv2/core/utils/configuration.private.hpp>

#അന്തര്ഭാവ <opencv2/core/utils/logger.hpp>

#അന്തര്ഭാവ "opencv2/core/utils/filesystem.private.hpp"
#അന്തര്ഭാവ "opencv2/core/utils/filesystem.hpp"

//#define DEBUG_FS_UTILS

#ifdef DEBUG_FS_UTILS
#അന്തര്ഭാവ <iostream>
#define DBG(...) __VA_ARGS__
#അല്ലെങ്കിൽ
#define DBG(...)
#endif


#എങ്കിൽ OPENCV_HAVE_FILESYSTEM_SUPPORT

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#undef NOMINMAX
#define NOMINMAX
#അന്തര്ഭാവ <windows.h>
#അന്തര്ഭാവ <direct.h>
#അന്തര്ഭാവ <sys/types.h>
#അന്തര്ഭാവ <sys/stat.h>
#അന്തര്ഭാവ <errno.h>
#അന്തര്ഭാവ <io.h>
#അന്തര്ഭാവ <stdio.h>
#അല്ലെങ്കിലെങ്കിൽ defined __linux__ || defined __APPLE__ || defined __HAIKU__ || defined __FreeBSD__ || defined __GNU__ || defined __EMSCRIPTEN__ || defined __QNX__ || defined __CYGWIN__
#അന്തര്ഭാവ <sys/types.h>
#അന്തര്ഭാവ <sys/stat.h>
#അന്തര്ഭാവ <fcntl.h>
#അന്തര്ഭാവ <unistd.h>
#അന്തര്ഭാവ <errno.h>
#endif

#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT

നാമാകാശ cv { നാമാകാശ utils { നാമാകാശ fs {

#ifdef _WIN32
സ്ഥിതം സ്ഥിരം അക്ഷരം native_separator = '\\';
#അല്ലെങ്കിൽ
സ്ഥിതം സ്ഥിരം അക്ഷരം native_separator = '/';
#endif

സ്ഥിതം വരിയിൽ
തർക്കം isPathSeparator(അക്ഷരം c)
{
    തിരികെ c == '/' || c == '\\';
}

cv::String join(സ്ഥിരം cv::String& base, സ്ഥിരം cv::String& path)
{
    എങ്കിൽ (base.empty())
        തിരികെ path;
    എങ്കിൽ (path.empty())
        തിരികെ base;

    തർക്കം baseSep = isPathSeparator(base[base.size() - 1]);
    തർക്കം pathSep = isPathSeparator(path[0]);
    String result;
    എങ്കിൽ (baseSep && pathSep)
    {
        result = base + path.substr(1);
    }
    അല്ലെങ്കിൽ എങ്കിൽ (!baseSep && !pathSep)
    {
        result = base + native_separator + path;
    }
    അല്ലെങ്കിൽ
    {
        result = base + path;
    }
    തിരികെ result;
}

CV_EXPORTS cv::String getParent(സ്ഥിരം cv::String &path)
{
    std::string::size_type loc = path.find_last_of("/\\");
    എങ്കിൽ (loc == std::string::npos)
        തിരികെ std::string();
    തിരികെ std::string(path, 0, loc);
}

CV_EXPORTS std::wstring getParent(സ്ഥിരം std::wstring& path)
{
    std::wstring::size_type loc = path.find_last_of(L"/\\");
    എങ്കിൽ (loc == std::wstring::npos)
        തിരികെ std::wstring();
    തിരികെ std::wstring(path, 0, loc);
}

#എങ്കിൽ OPENCV_HAVE_FILESYSTEM_SUPPORT

cv::String canonical(സ്ഥിരം cv::String& path)
{
    cv::String result;
#ifdef _WIN32
    സ്ഥിരം അക്ഷരം* result_str = _fullpath(ശൂന്യ, path.c_str(), 0);
#അല്ലെങ്കിൽ
    സ്ഥിരം അക്ഷരം* result_str = realpath(path.c_str(), ശൂന്യ);
#endif
    എങ്കിൽ (result_str)
    {
        result = cv::String(result_str);
        സ്വതന്ത്രം((ശൂന്യം*)result_str);
    }
    തിരികെ result.empty() ? path : result;
}


തർക്കം exists(സ്ഥിരം cv::String& path)
{
    CV_INSTRUMENT_REGION();

#എങ്കിൽ defined _WIN32 || defined WINCE
    BOOL status = TRUE;
    {
        WIN32_FILE_ATTRIBUTE_DATA all_attrs;
#ifdef WINRT
        wchar_t wpath[MAX_PATH];
        size_t copied = mbstowcs(wpath, path.c_str(), MAX_PATH);
        CV_Assert((copied != MAX_PATH) && (copied != (size_t)-1));
        status = ::GetFileAttributesExW(wpath, GetFileExInfoStandard, &all_attrs);
#അല്ലെങ്കിൽ
        status = ::GetFileAttributesExA(path.c_str(), GetFileExInfoStandard, &all_attrs);
#endif
    }

    തിരികെ !!status;
#അല്ലെങ്കിൽ
    ഘടന stat stat_buf;
    തിരികെ (0 == stat(path.c_str(), &stat_buf));
#endif
}

CV_EXPORTS ശൂന്യം remove_all(സ്ഥിരം cv::String& path)
{
    എങ്കിൽ (!exists(path))
        തിരികെ;
    എങ്കിൽ (isDirectory(path))
    {
        std::vector<String> entries;
        utils::fs::glob(path, cv::String(), entries, അസത്യം, സത്യം);
        വേണ്ടി (size_t i = 0; i < entries.size(); i++)
        {
            സ്ഥിരം String& e = entries[i];
            remove_all(e);
        }
#ifdef _MSC_VER
        തർക്കം result = _rmdir(path.c_str()) == 0;
#അല്ലെങ്കിൽ
        തർക്കം result = rmdir(path.c_str()) == 0;
#endif
        എങ്കിൽ (!result)
        {
            CV_LOG_ERROR(ശൂന്യ, "Can't remove directory: " << path);
        }
    }
    അല്ലെങ്കിൽ
    {
#ifdef _MSC_VER
        തർക്കം result = _unlink(path.c_str()) == 0;
#അല്ലെങ്കിൽ
        തർക്കം result = unlink(path.c_str()) == 0;
#endif
        എങ്കിൽ (!result)
        {
            CV_LOG_ERROR(ശൂന്യ, "Can't remove file: " << path);
        }
    }
}


cv::String getcwd()
{
    CV_INSTRUMENT_REGION();
    cv::AutoBuffer<അക്ഷരം, 4096> buf;
#എങ്കിൽ defined WIN32 || defined _WIN32 || defined WINCE
#ifdef WINRT
    തിരികെ cv::String();
#അല്ലെങ്കിൽ
    DWORD sz = GetCurrentDirectoryA(0, ശൂന്യ);
    buf.allocate((size_t)sz);
    sz = GetCurrentDirectoryA((DWORD)buf.size(), buf.data());
    തിരികെ cv::String(buf.data(), (size_t)sz);
#endif
#അല്ലെങ്കിലെങ്കിൽ defined __linux__ || defined __APPLE__ || defined __HAIKU__ || defined __FreeBSD__ || defined __EMSCRIPTEN__ || defined __QNX__ || defined __CYGWIN__
    വേണ്ടി(;;)
    {
        അക്ഷരം* p = ::getcwd(buf.data(), buf.size());
        എങ്കിൽ (p == ശൂന്യ)
        {
            എങ്കിൽ (errno == ERANGE)
            {
                buf.allocate(buf.size() * 2);
                തുടരുക;
            }
            തിരികെ cv::String();
        }
        നിർത്തുക;
    }
    തിരികെ cv::String(buf.data(), (size_t)strlen(buf.data()));
#അല്ലെങ്കിൽ
    തിരികെ cv::String();
#endif
}


തർക്കം createDirectory(സ്ഥിരം cv::String& path)
{
    CV_INSTRUMENT_REGION();
#എങ്കിൽ defined WIN32 || defined _WIN32 || defined WINCE
#ifdef WINRT
    wchar_t wpath[MAX_PATH];
    size_t copied = mbstowcs(wpath, path.c_str(), MAX_PATH);
    CV_Assert((copied != MAX_PATH) && (copied != (size_t)-1));
    പൂർണ്ണസംഖ്യ result = CreateDirectoryA(wpath, ശൂന്യ) ? 0 : -1;
#അല്ലെങ്കിൽ
    പൂർണ്ണസംഖ്യ result = _mkdir(path.c_str());
#endif
#അല്ലെങ്കിലെങ്കിൽ defined __linux__ || defined __APPLE__ || defined __HAIKU__ || defined __FreeBSD__ || defined __EMSCRIPTEN__ || defined __QNX__ || defined __CYGWIN__
    പൂർണ്ണസംഖ്യ result = mkdir(path.c_str(), 0777);
#അല്ലെങ്കിൽ
    പൂർണ്ണസംഖ്യ result = -1;
#endif

    എങ്കിൽ (result == -1)
    {
        തിരികെ isDirectory(path);
    }
    തിരികെ സത്യം;
}

തർക്കം createDirectories(സ്ഥിരം cv::String& path_)
{
    cv::String path = path_;
    വേണ്ടി (;;)
    {
        അക്ഷരം last_char = path.empty() ? 0 : path[path.length() - 1];
        എങ്കിൽ (isPathSeparator(last_char))
        {
            path = path.substr(0, path.length() - 1);
            തുടരുക;
        }
        നിർത്തുക;
    }

    എങ്കിൽ (path.empty() || path == "./" || path == ".\\" || path == ".")
        തിരികെ സത്യം;
    എങ്കിൽ (isDirectory(path))
        തിരികെ സത്യം;

    size_t pos = path.rfind('/');
    എങ്കിൽ (pos == cv::String::npos)
        pos = path.rfind('\\');
    എങ്കിൽ (pos != cv::String::npos)
    {
        cv::String parent_directory = path.substr(0, pos);
        എങ്കിൽ (!parent_directory.empty())
        {
            എങ്കിൽ (!createDirectories(parent_directory))
                തിരികെ അസത്യം;
        }
    }

    തിരികെ createDirectory(path);
}

#ifdef _WIN32

ഘടന FileLock::Impl
{
    Impl(സ്ഥിരം അക്ഷരം* fname)
    {
        // http://support.microsoft.com/kb/316609
        പൂർണ്ണസംഖ്യ അക്കം_Retries = 5;
        ചെയ്യുക
        {
            handle = ::CreateFileA(fname, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, ശൂന്യ,
                                OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, ശൂന്യ);
            എങ്കിൽ (INVALID_HANDLE_മൂല്യം == handle)
            {
                എങ്കിൽ (ERROR_SHARING_VIOLATION == GetLastError())
                {
                    അക്കം_Retries--;
                    Sleep(250);
                    തുടരുക;
                }
                അല്ലെങ്കിൽ
                {
                    CV_Error_(Error::StsAssert, ("Can't open lock file: %s", fname));
                }
            }
            നിർത്തുക;
        } ആകുമ്പോൾ (അക്കം_Retries > 0);
    }
    ~Impl()
    {
        എങ്കിൽ (INVALID_HANDLE_മൂല്യം != handle)
        {
            ::CloseHandle(handle);
        }
    }

    തർക്കം lock()
    {
        OVERLAPPED overlapped;
        std::memset(&overlapped, 0, വലുപ്പം(overlapped));
        തിരികെ !!::LockFileEx(handle, LOCKFILE_EXCLUSIVE_LOCK, 0, MAXDWORD, MAXDWORD, &overlapped);
    }
    തർക്കം unlock()
    {
        OVERLAPPED overlapped;
        std::memset(&overlapped, 0, വലുപ്പം(overlapped));
        തിരികെ !!::UnlockFileEx(handle, 0, MAXDWORD, MAXDWORD, &overlapped);
    }

    തർക്കം lock_shared()
    {
        OVERLAPPED overlapped;
        std::memset(&overlapped, 0, വലുപ്പം(overlapped));
        തിരികെ !!::LockFileEx(handle, 0, 0, MAXDWORD, MAXDWORD, &overlapped);
    }
    തർക്കം unlock_shared()
    {
        തിരികെ unlock();
    }

    HANDLE handle;

സ്വകാര്യ:
    Impl(സ്ഥിരം Impl&); // disabled
    Impl& operator=(സ്ഥിരം Impl&); // disabled
};

#അല്ലെങ്കിലെങ്കിൽ defined __linux__ || defined __APPLE__ || defined __HAIKU__ || defined __FreeBSD__ || defined __GNU__ || defined __EMSCRIPTEN__ || defined __QNX__ || defined __CYGWIN__

ഘടന FileLock::Impl
{
    Impl(സ്ഥിരം അക്ഷരം* fname)
    {
        handle = ::open(fname, O_RDWR);
        CV_Assert(handle != -1);
    }
    ~Impl()
    {
        എങ്കിൽ (handle >= 0)
            ::close(handle);
    }

    തർക്കം lock()
    {
        ഘടന ::flock l;
        std::memset(&l, 0, വലുപ്പം(l));
        l.l_type = F_WRLCK;
        l.l_whence = SEEK_SET;
        l.l_start = 0;
        l.l_len = 0;
        DBG(std::cout << "Lock..." << std::endl);
        തർക്കം res = -1 != ::fcntl(handle, F_SETLKW, &l);
        തിരികെ res;
    }
    തർക്കം unlock()
    {
        ഘടന ::flock l;
        std::memset(&l, 0, വലുപ്പം(l));
        l.l_type = F_UNLCK;
        l.l_whence = SEEK_SET;
        l.l_start = 0;
        l.l_len = 0;
        DBG(std::cout << "Unlock..." << std::endl);
        തർക്കം res = -1 != ::fcntl(handle, F_SETLK, &l);
        തിരികെ res;
    }

    തർക്കം lock_shared()
    {
        ഘടന ::flock l;
        std::memset(&l, 0, വലുപ്പം(l));
        l.l_type = F_RDLCK;
        l.l_whence = SEEK_SET;
        l.l_start = 0;
        l.l_len = 0;
        DBG(std::cout << "Lock read..." << std::endl);
        തർക്കം res = -1 != ::fcntl(handle, F_SETLKW, &l);
        തിരികെ res;
    }
    തർക്കം unlock_shared()
    {
        തിരികെ unlock();
    }

    പൂർണ്ണസംഖ്യ handle;

സ്വകാര്യ:
    Impl(സ്ഥിരം Impl&); // disabled
    Impl& operator=(സ്ഥിരം Impl&); // disabled
};

#endif

FileLock::FileLock(സ്ഥിരം അക്ഷരം* fname)
    : pImpl(പുതിയ Impl(fname))
{
    // nothing
}
FileLock::~FileLock()
{
    delete pImpl;
    pImpl = ശൂന്യ;
}

ശൂന്യം FileLock::lock() { CV_Assert(pImpl->lock()); }
ശൂന്യം FileLock::unlock() { CV_Assert(pImpl->unlock()); }
ശൂന്യം FileLock::lock_shared() { CV_Assert(pImpl->lock_shared()); }
ശൂന്യം FileLock::unlock_shared() { CV_Assert(pImpl->unlock_shared()); }



cv::String getCacheDirectory(സ്ഥിരം അക്ഷരം* sub_directory_name, സ്ഥിരം അക്ഷരം* configuration_name)
{
    String cache_path;
    എങ്കിൽ (configuration_name)
    {
        cache_path = utils::getConfigurationParameterString(configuration_name, "");
    }
    എങ്കിൽ (cache_path.empty())
    {
        cv::String default_cache_path;
#ifdef _WIN32
        wchar_t tmp_path_buf_w[MAX_PATH+1] = {0};
        DWORD res = GetTempPathW(MAX_PATH, tmp_path_buf_w);
        എങ്കിൽ (res > 0 && res <= MAX_PATH)
        {
            // Convert from UTF-16 to UTF-8 to support Unicode paths
            പൂർണ്ണസംഖ്യ len = WideCharToMultiByte(CP_UTF8, 0, tmp_path_buf_w, -1, ശൂന്യ, 0, ശൂന്യ, ശൂന്യ);
            എങ്കിൽ (len > 0)
            {
                std::vector<അക്ഷരം> utf8_buf(len);
                WideCharToMultiByte(CP_UTF8, 0, tmp_path_buf_w, -1, utf8_buf.data(), len, ശൂന്യ, ശൂന്യ);
                default_cache_path = std::string(utf8_buf.data());
            }
        }
#അല്ലെങ്കിലെങ്കിൽ defined __ANDROID__
        // no defaults
#അല്ലെങ്കിലെങ്കിൽ defined __APPLE__
        സ്ഥിരം std::string tmpdir_env = utils::getConfigurationParameterString("TMPDIR");
        എങ്കിൽ (!tmpdir_env.empty() && utils::fs::isDirectory(tmpdir_env))
        {
            default_cache_path = tmpdir_env;
        }
        അല്ലെങ്കിൽ
        {
            default_cache_path = "/tmp/";
            CV_LOG_WARNING(ശൂന്യ, "Using world accessible cache directory. This may be not secure: " << default_cache_path);
        }
#അല്ലെങ്കിലെങ്കിൽ defined __linux__ || defined __HAIKU__ || defined __FreeBSD__ || defined __GNU__
        // https://specifications.freedesktop.org/basedir-spec/basedir-spec-latest.html
        എങ്കിൽ (default_cache_path.empty())
        {
            സ്ഥിരം std::string xdg_cache_env = utils::getConfigurationParameterString("XDG_CACHE_HOME");
            എങ്കിൽ (!xdg_cache_env.empty() && utils::fs::isDirectory(xdg_cache_env))
            {
                default_cache_path = xdg_cache_env;
            }
        }
        എങ്കിൽ (default_cache_path.empty())
        {
            സ്ഥിരം std::string home_env = utils::getConfigurationParameterString("HOME");
            എങ്കിൽ (!home_env.empty() && utils::fs::isDirectory(home_env))
            {
                cv::String home_path = home_env;
                cv::String home_cache_path = utils::fs::join(home_path, ".cache/");
                എങ്കിൽ (utils::fs::isDirectory(home_cache_path))
                {
                    default_cache_path = home_cache_path;
                }
            }
        }
        എങ്കിൽ (default_cache_path.empty())
        {
            സ്ഥിരം അക്ഷരം* temp_path = "/var/tmp/";
            എങ്കിൽ (utils::fs::isDirectory(temp_path))
            {
                default_cache_path = temp_path;
                CV_LOG_WARNING(ശൂന്യ, "Using world accessible cache directory. This may be not secure: " << default_cache_path);
            }
        }
        എങ്കിൽ (default_cache_path.empty())
        {
            default_cache_path = "/tmp/";
            CV_LOG_WARNING(ശൂന്യ, "Using world accessible cache directory. This may be not secure: " << default_cache_path);
        }
#അല്ലെങ്കിൽ
        // no defaults
#endif
        CV_LOG_VERBOSE(ശൂന്യ, 0, "default_cache_path = " << default_cache_path);
        എങ്കിൽ (!default_cache_path.empty())
        {
            എങ്കിൽ (utils::fs::isDirectory(default_cache_path))
            {
                cv::String default_cache_path_base = utils::fs::join(default_cache_path, "opencv");
                default_cache_path = utils::fs::join(default_cache_path_base, CVAUX_STR(CV_VERSION_MAJOR) ".x" CV_VERSION_STATUS);
                എങ്കിൽ (utils::getConfigurationParameterBool("OPENCV_CACHE_SHOW_CLEANUP_MESSAGE", സത്യം)
                    && !utils::fs::isDirectory(default_cache_path))
                {
                    std::vector<cv::String> existedCacheDirs;
                    ശ്രമിക്കുക
                    {
                        utils::fs::glob_relative(default_cache_path_base, "*", existedCacheDirs, അസത്യം, സത്യം);
                    }
                    പിടിക്കുക (...)
                    {
                        // ignore
                    }
                    എങ്കിൽ (!existedCacheDirs.empty())
                    {
                        CV_LOG_WARNING(ശൂന്യ, "Creating new OpenCV cache directory: " << default_cache_path);
                        CV_LOG_WARNING(ശൂന്യ, "There are several neighbour directories, probably created by old OpenCV versions.");
                        CV_LOG_WARNING(ശൂന്യ, "Feel free to cleanup these unused directories:");
                        വേണ്ടി (size_t i = 0; i < existedCacheDirs.size(); i++)
                        {
                            CV_LOG_WARNING(ശൂന്യ, "  - " << existedCacheDirs[i]);
                        }
                        CV_LOG_WARNING(ശൂന്യ, "Note: This message is showed only once.");
                    }
                }
                എങ്കിൽ (sub_directory_name && sub_directory_name[0] != '\0')
                    default_cache_path = utils::fs::join(default_cache_path, cv::String(sub_directory_name) + native_separator);
                എങ്കിൽ (!utils::fs::createDirectories(default_cache_path))
                {
                    CV_LOG_DEBUG(ശൂന്യ, "Can't create OpenCV cache sub-directory: " << default_cache_path);
                }
                അല്ലെങ്കിൽ
                {
                    cache_path = default_cache_path;
                }
            }
            അല്ലെങ്കിൽ
            {
                CV_LOG_INFO(ശൂന്യ, "Can't find default cache directory (does it exist?): " << default_cache_path);
            }
        }
        അല്ലെങ്കിൽ
        {
            CV_LOG_DEBUG(ശൂന്യ, "OpenCV has no support to discover default cache directory on the current platform");
        }
    }
    അല്ലെങ്കിൽ
    {
        എങ്കിൽ (cache_path == "disabled")
            തിരികെ cache_path;
        എങ്കിൽ (!isDirectory(cache_path))
        {
            CV_LOG_WARNING(ശൂന്യ, "Specified non-existed directory, creating OpenCV sub-directory for caching purposes: " << cache_path);
            എങ്കിൽ (!createDirectories(cache_path))
            {
                CV_LOG_ERROR(ശൂന്യ, "Can't create OpenCV cache sub-directory: " << cache_path);
                cache_path.clear();
            }
        }
    }
    CV_Assert(cache_path.empty() || utils::fs::isDirectory(cache_path));
    എങ്കിൽ (!cache_path.empty())
    {
        എങ്കിൽ (!isPathSeparator(cache_path[cache_path.size() - 1]))
        {
            cache_path += native_separator;
        }
    }
    തിരികെ cache_path;
}

#അല്ലെങ്കിൽ
#define NOT_IMPLEMENTED CV_Error(Error::StsNotImplemented, "");
cv::String canonical(സ്ഥിരം cv::String& /*path*/) { NOT_IMPLEMENTED }
തർക്കം exists(സ്ഥിരം cv::String& /*path*/) { NOT_IMPLEMENTED }
ശൂന്യം remove_all(സ്ഥിരം cv::String& /*path*/) { NOT_IMPLEMENTED }
cv::String getcwd() { NOT_IMPLEMENTED }
തർക്കം createDirectory(സ്ഥിരം cv::String& /*path*/) { NOT_IMPLEMENTED }
തർക്കം createDirectories(സ്ഥിരം cv::String& /*path*/) { NOT_IMPLEMENTED }
cv::String getCacheDirectory(സ്ഥിരം അക്ഷരം* /*sub_directory_name*/, സ്ഥിരം അക്ഷരം* /*configuration_name = NULL*/) { NOT_IMPLEMENTED }
#undef NOT_IMPLEMENTED
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT

}}} // namespace


#എങ്കിൽ OPENCV_HAVE_FILESYSTEM_SUPPORT
#അന്തര്ഭാവ "plugin_loader.impl.hpp"
#endif
