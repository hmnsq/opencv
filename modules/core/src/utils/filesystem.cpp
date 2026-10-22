// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#include "../precomp.hpp"

#include <opencv2/core/utils/configuration.private.hpp>

#include <opencv2/core/utils/logger.hpp>

#include "opencv2/core/utils/filesystem.private.hpp"
#include "opencv2/core/utils/filesystem.hpp"

//#define DEBUG_FS_UTILS

#ifdef DEBUG_FS_UTILS
#include <iostream>
#define DBG(...) __VA_ARGS__
#иначе
#define DBG(...)
#endif


#если OPENCV_HAVE_FILESYSTEM_SUPPORT

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#undef NOMINMAX
#define NOMINMAX
#include <windows.h>
#include <direct.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <io.h>
#include <stdio.h>
#elif defined __linux__ || defined __APPLE__ || defined __HAIKU__ || defined __FreeBSD__ || defined __GNU__ || defined __EMSCRIPTEN__ || defined __QNX__ || defined __CYGWIN__
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#endif

#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT

namespace cv { namespace utils { namespace fs {

#ifdef _WIN32
static const char native_separator = '\\';
#иначе
static const char native_separator = '/';
#endif

static inline
bool isPathSeparator(char c)
{
    вернуть c == '/' || c == '\\';
}

cv::String join(const cv::String& base, const cv::String& path)
{
    если (base.empty())
        вернуть path;
    если (path.empty())
        вернуть base;

    bool baseSep = isPathSeparator(base[base.size() - 1]);
    bool pathSep = isPathSeparator(path[0]);
    String result;
    если (baseSep && pathSep)
    {
        result = base + path.substr(1);
    }
    иначе если (!baseSep && !pathSep)
    {
        result = base + native_separator + path;
    }
    иначе
    {
        result = base + path;
    }
    вернуть result;
}

CV_EXPORTS cv::String getParent(const cv::String &path)
{
    std::string::size_type loc = path.find_last_of("/\\");
    если (loc == std::string::npos)
        вернуть std::string();
    вернуть std::string(path, 0, loc);
}

CV_EXPORTS std::wstring getParent(const std::wstring& path)
{
    std::wstring::size_type loc = path.find_last_of(L"/\\");
    если (loc == std::wstring::npos)
        вернуть std::wstring();
    вернуть std::wstring(path, 0, loc);
}

#если OPENCV_HAVE_FILESYSTEM_SUPPORT

cv::String canonical(const cv::String& path)
{
    cv::String result;
#ifdef _WIN32
    const char* result_str = _fullpath(NULL, path.c_str(), 0);
#иначе
    const char* result_str = realpath(path.c_str(), NULL);
#endif
    если (result_str)
    {
        result = cv::String(result_str);
        free((void*)result_str);
    }
    вернуть result.empty() ? path : result;
}


bool exists(const cv::String& path)
{
    CV_INSTRUMENT_REGION();

#если defined _WIN32 || defined WINCE
    BOOL status = TRUE;
    {
        WIN32_FILE_ATTRIBUTE_DATA all_attrs;
#ifdef WINRT
        wchar_t wpath[MAX_PATH];
        size_t copied = mbstowcs(wpath, path.c_str(), MAX_PATH);
        CV_Assert((copied != MAX_PATH) && (copied != (size_t)-1));
        status = ::GetFileAttributesExW(wpath, GetFileExInfoStandard, &all_attrs);
#иначе
        status = ::GetFileAttributesExA(path.c_str(), GetFileExInfoStandard, &all_attrs);
#endif
    }

    вернуть !!status;
#иначе
    struct stat stat_buf;
    вернуть (0 == stat(path.c_str(), &stat_buf));
#endif
}

CV_EXPORTS void remove_all(const cv::String& path)
{
    если (!exists(path))
        вернуть;
    если (isDirectory(path))
    {
        std::vector<String> entries;
        utils::fs::glob(path, cv::String(), entries, false, true);
        для (size_t i = 0; i < entries.size(); i++)
        {
            const String& e = entries[i];
            remove_all(e);
        }
#ifdef _MSC_VER
        bool result = _rmdir(path.c_str()) == 0;
#иначе
        bool result = rmdir(path.c_str()) == 0;
#endif
        если (!result)
        {
            CV_LOG_ERROR(NULL, "Can't remove directory: " << path);
        }
    }
    иначе
    {
#ifdef _MSC_VER
        bool result = _unlink(path.c_str()) == 0;
#иначе
        bool result = unlink(path.c_str()) == 0;
#endif
        если (!result)
        {
            CV_LOG_ERROR(NULL, "Can't remove file: " << path);
        }
    }
}


cv::String getcwd()
{
    CV_INSTRUMENT_REGION();
    cv::AutoBuffer<char, 4096> buf;
#если defined WIN32 || defined _WIN32 || defined WINCE
#ifdef WINRT
    вернуть cv::String();
#иначе
    DWORD sz = GetCurrentDirectoryA(0, NULL);
    buf.allocate((size_t)sz);
    sz = GetCurrentDirectoryA((DWORD)buf.size(), buf.data());
    вернуть cv::String(buf.data(), (size_t)sz);
#endif
#elif defined __linux__ || defined __APPLE__ || defined __HAIKU__ || defined __FreeBSD__ || defined __EMSCRIPTEN__ || defined __QNX__ || defined __CYGWIN__
    для(;;)
    {
        char* p = ::getcwd(buf.data(), buf.size());
        если (p == NULL)
        {
            если (errno == ERANGE)
            {
                buf.allocate(buf.size() * 2);
                continue;
            }
            вернуть cv::String();
        }
        break;
    }
    вернуть cv::String(buf.data(), (size_t)strlen(buf.data()));
#иначе
    вернуть cv::String();
#endif
}


bool createDirectory(const cv::String& path)
{
    CV_INSTRUMENT_REGION();
#если defined WIN32 || defined _WIN32 || defined WINCE
#ifdef WINRT
    wchar_t wpath[MAX_PATH];
    size_t copied = mbstowcs(wpath, path.c_str(), MAX_PATH);
    CV_Assert((copied != MAX_PATH) && (copied != (size_t)-1));
    целое result = CreateDirectoryA(wpath, NULL) ? 0 : -1;
#иначе
    целое result = _mkdir(path.c_str());
#endif
#elif defined __linux__ || defined __APPLE__ || defined __HAIKU__ || defined __FreeBSD__ || defined __EMSCRIPTEN__ || defined __QNX__ || defined __CYGWIN__
    целое result = mkdir(path.c_str(), 0777);
#иначе
    целое result = -1;
#endif

    если (result == -1)
    {
        вернуть isDirectory(path);
    }
    вернуть true;
}

bool createDirectories(const cv::String& path_)
{
    cv::String path = path_;
    для (;;)
    {
        char last_char = path.empty() ? 0 : path[path.length() - 1];
        если (isPathSeparator(last_char))
        {
            path = path.substr(0, path.length() - 1);
            continue;
        }
        break;
    }

    если (path.empty() || path == "./" || path == ".\\" || path == ".")
        вернуть true;
    если (isDirectory(path))
        вернуть true;

    size_t pos = path.rfind('/');
    если (pos == cv::String::npos)
        pos = path.rfind('\\');
    если (pos != cv::String::npos)
    {
        cv::String parent_directory = path.substr(0, pos);
        если (!parent_directory.empty())
        {
            если (!createDirectories(parent_directory))
                вернуть false;
        }
    }

    вернуть createDirectory(path);
}

#ifdef _WIN32

struct FileLock::Impl
{
    Impl(const char* fname)
    {
        // http://support.microsoft.com/kb/316609
        целое numRetries = 5;
        do
        {
            handle = ::CreateFileA(fname, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
                                OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
            если (INVALID_HANDLE_VALUE == handle)
            {
                если (ERROR_SHARING_VIOLATION == GetLastError())
                {
                    numRetries--;
                    Sleep(250);
                    continue;
                }
                иначе
                {
                    CV_Error_(Error::StsAssert, ("Can't open lock file: %s", fname));
                }
            }
            break;
        } пока (numRetries > 0);
    }
    ~Impl()
    {
        если (INVALID_HANDLE_VALUE != handle)
        {
            ::CloseHandle(handle);
        }
    }

    bool lock()
    {
        OVERLAPPED overlapped;
        std::memset(&overlapped, 0, sizeof(overlapped));
        вернуть !!::LockFileEx(handle, LOCKFILE_EXCLUSIVE_LOCK, 0, MAXDWORD, MAXDWORD, &overlapped);
    }
    bool unlock()
    {
        OVERLAPPED overlapped;
        std::memset(&overlapped, 0, sizeof(overlapped));
        вернуть !!::UnlockFileEx(handle, 0, MAXDWORD, MAXDWORD, &overlapped);
    }

    bool lock_shared()
    {
        OVERLAPPED overlapped;
        std::memset(&overlapped, 0, sizeof(overlapped));
        вернуть !!::LockFileEx(handle, 0, 0, MAXDWORD, MAXDWORD, &overlapped);
    }
    bool unlock_shared()
    {
        вернуть unlock();
    }

    HANDLE handle;

private:
    Impl(const Impl&); // disabled
    Impl& operator=(const Impl&); // disabled
};

#elif defined __linux__ || defined __APPLE__ || defined __HAIKU__ || defined __FreeBSD__ || defined __GNU__ || defined __EMSCRIPTEN__ || defined __QNX__ || defined __CYGWIN__

struct FileLock::Impl
{
    Impl(const char* fname)
    {
        handle = ::open(fname, O_RDWR);
        CV_Assert(handle != -1);
    }
    ~Impl()
    {
        если (handle >= 0)
            ::close(handle);
    }

    bool lock()
    {
        struct ::flock l;
        std::memset(&l, 0, sizeof(l));
        l.l_type = F_WRLCK;
        l.l_whence = SEEK_SET;
        l.l_start = 0;
        l.l_len = 0;
        DBG(std::cout << "Lock..." << std::endl);
        bool res = -1 != ::fcntl(handle, F_SETLKW, &l);
        вернуть res;
    }
    bool unlock()
    {
        struct ::flock l;
        std::memset(&l, 0, sizeof(l));
        l.l_type = F_UNLCK;
        l.l_whence = SEEK_SET;
        l.l_start = 0;
        l.l_len = 0;
        DBG(std::cout << "Unlock..." << std::endl);
        bool res = -1 != ::fcntl(handle, F_SETLK, &l);
        вернуть res;
    }

    bool lock_shared()
    {
        struct ::flock l;
        std::memset(&l, 0, sizeof(l));
        l.l_type = F_RDLCK;
        l.l_whence = SEEK_SET;
        l.l_start = 0;
        l.l_len = 0;
        DBG(std::cout << "Lock read..." << std::endl);
        bool res = -1 != ::fcntl(handle, F_SETLKW, &l);
        вернуть res;
    }
    bool unlock_shared()
    {
        вернуть unlock();
    }

    целое handle;

private:
    Impl(const Impl&); // disabled
    Impl& operator=(const Impl&); // disabled
};

#endif

FileLock::FileLock(const char* fname)
    : pImpl(new Impl(fname))
{
    // nothing
}
FileLock::~FileLock()
{
    delete pImpl;
    pImpl = NULL;
}

void FileLock::lock() { CV_Assert(pImpl->lock()); }
void FileLock::unlock() { CV_Assert(pImpl->unlock()); }
void FileLock::lock_shared() { CV_Assert(pImpl->lock_shared()); }
void FileLock::unlock_shared() { CV_Assert(pImpl->unlock_shared()); }



cv::String getCacheDirectory(const char* sub_directory_name, const char* configuration_name)
{
    String cache_path;
    если (configuration_name)
    {
        cache_path = utils::getConfigurationParameterString(configuration_name, "");
    }
    если (cache_path.empty())
    {
        cv::String default_cache_path;
#ifdef _WIN32
        wchar_t tmp_path_buf_w[MAX_PATH+1] = {0};
        DWORD res = GetTempPathW(MAX_PATH, tmp_path_buf_w);
        если (res > 0 && res <= MAX_PATH)
        {
            // Convert from UTF-16 to UTF-8 to support Unicode paths
            целое len = WideCharToMultiByte(CP_UTF8, 0, tmp_path_buf_w, -1, NULL, 0, NULL, NULL);
            если (len > 0)
            {
                std::vector<char> utf8_buf(len);
                WideCharToMultiByte(CP_UTF8, 0, tmp_path_buf_w, -1, utf8_buf.data(), len, NULL, NULL);
                default_cache_path = std::string(utf8_buf.data());
            }
        }
#elif defined __ANDROID__
        // no defaults
#elif defined __APPLE__
        const std::string tmpdir_env = utils::getConfigurationParameterString("TMPDIR");
        если (!tmpdir_env.empty() && utils::fs::isDirectory(tmpdir_env))
        {
            default_cache_path = tmpdir_env;
        }
        иначе
        {
            default_cache_path = "/tmp/";
            CV_LOG_WARNING(NULL, "Using world accessible cache directory. This may be not secure: " << default_cache_path);
        }
#elif defined __linux__ || defined __HAIKU__ || defined __FreeBSD__ || defined __GNU__
        // https://specifications.freedesktop.org/basedir-spec/basedir-spec-latest.html
        если (default_cache_path.empty())
        {
            const std::string xdg_cache_env = utils::getConfigurationParameterString("XDG_CACHE_HOME");
            если (!xdg_cache_env.empty() && utils::fs::isDirectory(xdg_cache_env))
            {
                default_cache_path = xdg_cache_env;
            }
        }
        если (default_cache_path.empty())
        {
            const std::string home_env = utils::getConfigurationParameterString("HOME");
            если (!home_env.empty() && utils::fs::isDirectory(home_env))
            {
                cv::String home_path = home_env;
                cv::String home_cache_path = utils::fs::join(home_path, ".cache/");
                если (utils::fs::isDirectory(home_cache_path))
                {
                    default_cache_path = home_cache_path;
                }
            }
        }
        если (default_cache_path.empty())
        {
            const char* temp_path = "/var/tmp/";
            если (utils::fs::isDirectory(temp_path))
            {
                default_cache_path = temp_path;
                CV_LOG_WARNING(NULL, "Using world accessible cache directory. This may be not secure: " << default_cache_path);
            }
        }
        если (default_cache_path.empty())
        {
            default_cache_path = "/tmp/";
            CV_LOG_WARNING(NULL, "Using world accessible cache directory. This may be not secure: " << default_cache_path);
        }
#иначе
        // no defaults
#endif
        CV_LOG_VERBOSE(NULL, 0, "default_cache_path = " << default_cache_path);
        если (!default_cache_path.empty())
        {
            если (utils::fs::isDirectory(default_cache_path))
            {
                cv::String default_cache_path_base = utils::fs::join(default_cache_path, "opencv");
                default_cache_path = utils::fs::join(default_cache_path_base, CVAUX_STR(CV_VERSION_MAJOR) ".x" CV_VERSION_STATUS);
                если (utils::getConfigurationParameterBool("OPENCV_CACHE_SHOW_CLEANUP_MESSAGE", true)
                    && !utils::fs::isDirectory(default_cache_path))
                {
                    std::vector<cv::String> existedCacheDirs;
                    try
                    {
                        utils::fs::glob_relative(default_cache_path_base, "*", existedCacheDirs, false, true);
                    }
                    catch (...)
                    {
                        // ignore
                    }
                    если (!existedCacheDirs.empty())
                    {
                        CV_LOG_WARNING(NULL, "Creating new OpenCV cache directory: " << default_cache_path);
                        CV_LOG_WARNING(NULL, "There are several neighbour directories, probably created by old OpenCV versions.");
                        CV_LOG_WARNING(NULL, "Feel free to cleanup these unused directories:");
                        для (size_t i = 0; i < existedCacheDirs.size(); i++)
                        {
                            CV_LOG_WARNING(NULL, "  - " << existedCacheDirs[i]);
                        }
                        CV_LOG_WARNING(NULL, "Note: This message is showed only once.");
                    }
                }
                если (sub_directory_name && sub_directory_name[0] != '\0')
                    default_cache_path = utils::fs::join(default_cache_path, cv::String(sub_directory_name) + native_separator);
                если (!utils::fs::createDirectories(default_cache_path))
                {
                    CV_LOG_DEBUG(NULL, "Can't create OpenCV cache sub-directory: " << default_cache_path);
                }
                иначе
                {
                    cache_path = default_cache_path;
                }
            }
            иначе
            {
                CV_LOG_INFO(NULL, "Can't find default cache directory (does it exist?): " << default_cache_path);
            }
        }
        иначе
        {
            CV_LOG_DEBUG(NULL, "OpenCV has no support to discover default cache directory on the current platform");
        }
    }
    иначе
    {
        если (cache_path == "disabled")
            вернуть cache_path;
        если (!isDirectory(cache_path))
        {
            CV_LOG_WARNING(NULL, "Specified non-existed directory, creating OpenCV sub-directory for caching purposes: " << cache_path);
            если (!createDirectories(cache_path))
            {
                CV_LOG_ERROR(NULL, "Can't create OpenCV cache sub-directory: " << cache_path);
                cache_path.clear();
            }
        }
    }
    CV_Assert(cache_path.empty() || utils::fs::isDirectory(cache_path));
    если (!cache_path.empty())
    {
        если (!isPathSeparator(cache_path[cache_path.size() - 1]))
        {
            cache_path += native_separator;
        }
    }
    вернуть cache_path;
}

#иначе
#define NOT_IMPLEMENTED CV_Error(Error::StsNotImplemented, "");
cv::String canonical(const cv::String& /*path*/) { NOT_IMPLEMENTED }
bool exists(const cv::String& /*path*/) { NOT_IMPLEMENTED }
void remove_all(const cv::String& /*path*/) { NOT_IMPLEMENTED }
cv::String getcwd() { NOT_IMPLEMENTED }
bool createDirectory(const cv::String& /*path*/) { NOT_IMPLEMENTED }
bool createDirectories(const cv::String& /*path*/) { NOT_IMPLEMENTED }
cv::String getCacheDirectory(const char* /*sub_directory_name*/, const char* /*configuration_name = NULL*/) { NOT_IMPLEMENTED }
#undef NOT_IMPLEMENTED
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT

}}} // namespace


#если OPENCV_HAVE_FILESYSTEM_SUPPORT
#include "plugin_loader.impl.hpp"
#endif
