// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ಅನ್ತರ್ಭಾವ "../precomp.hpp"

#ಅನ್ತರ್ಭಾವ <opencv2/core/utils/configuration.private.hpp>

#ಅನ್ತರ್ಭಾವ <opencv2/core/utils/logger.hpp>

#ಅನ್ತರ್ಭಾವ "opencv2/core/utils/filesystem.private.hpp"
#ಅನ್ತರ್ಭಾವ "opencv2/core/utils/filesystem.hpp"

//#define DEBUG_FS_UTILS

#ifdef DEBUG_FS_UTILS
#ಅನ್ತರ್ಭಾವ <iostream>
#define DBG(...) __VA_ARGS__
#ಅನ್ಯಥಾ
#define DBG(...)
#endif


#ಯದಿ OPENCV_HAVE_FILESYSTEM_SUPPORT

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#undef NOMINMAX
#define NOMINMAX
#ಅನ್ತರ್ಭಾವ <windows.h>
#ಅನ್ತರ್ಭಾವ <direct.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಅನ್ತರ್ಭಾವ <sys/stat.h>
#ಅನ್ತರ್ಭಾವ <errno.h>
#ಅನ್ತರ್ಭಾವ <io.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ನೋಚೇದ್ಯದಿ defined __linux__ || defined __APPLE__ || defined __HAIKU__ || defined __FreeBSD__ || defined __GNU__ || defined __EMSCRIPTEN__ || defined __QNX__ || defined __CYGWIN__
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಅನ್ತರ್ಭಾವ <sys/stat.h>
#ಅನ್ತರ್ಭಾವ <fcntl.h>
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಅನ್ತರ್ಭಾವ <errno.h>
#endif

#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT

ನಾಮಾಕಾಶ cv { ನಾಮಾಕಾಶ utils { ನಾಮಾಕಾಶ fs {

#ifdef _WIN32
ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ native_separator = '\\';
#ಅನ್ಯಥಾ
ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ native_separator = '/';
#endif

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ
ತಾರ್ಕಿಕ isPathSeparator(ವರ್ಣ c)
{
    ಪ್ರತ್ಯಾಗಮ c == '/' || c == '\\';
}

cv::String join(ಧ್ರುವ cv::String& base, ಧ್ರುವ cv::String& path)
{
    ಯದಿ (base.empty())
        ಪ್ರತ್ಯಾಗಮ path;
    ಯದಿ (path.empty())
        ಪ್ರತ್ಯಾಗಮ base;

    ತಾರ್ಕಿಕ baseSep = isPathSeparator(base[base.size() - 1]);
    ತಾರ್ಕಿಕ pathSep = isPathSeparator(path[0]);
    String result;
    ಯದಿ (baseSep && pathSep)
    {
        result = base + path.substr(1);
    }
    ಅನ್ಯಥಾ ಯದಿ (!baseSep && !pathSep)
    {
        result = base + native_separator + path;
    }
    ಅನ್ಯಥಾ
    {
        result = base + path;
    }
    ಪ್ರತ್ಯಾಗಮ result;
}

CV_EXPORTS cv::String getParent(ಧ್ರುವ cv::String &path)
{
    std::string::size_type loc = path.find_last_of("/\\");
    ಯದಿ (loc == std::string::npos)
        ಪ್ರತ್ಯಾಗಮ std::string();
    ಪ್ರತ್ಯಾಗಮ std::string(path, 0, loc);
}

CV_EXPORTS std::wstring getParent(ಧ್ರುವ std::wstring& path)
{
    std::wstring::size_type loc = path.find_last_of(L"/\\");
    ಯದಿ (loc == std::wstring::npos)
        ಪ್ರತ್ಯಾಗಮ std::wstring();
    ಪ್ರತ್ಯಾಗಮ std::wstring(path, 0, loc);
}

#ಯದಿ OPENCV_HAVE_FILESYSTEM_SUPPORT

cv::String canonical(ಧ್ರುವ cv::String& path)
{
    cv::String result;
#ifdef _WIN32
    ಧ್ರುವ ವರ್ಣ* result_str = _fullpath(ರಿಕ್ತ, path.c_str(), 0);
#ಅನ್ಯಥಾ
    ಧ್ರುವ ವರ್ಣ* result_str = realpath(path.c_str(), ರಿಕ್ತ);
#endif
    ಯದಿ (result_str)
    {
        result = cv::String(result_str);
        ಮುಕ್ತ((ಶೂನ್ಯ*)result_str);
    }
    ಪ್ರತ್ಯಾಗಮ result.empty() ? path : result;
}


ತಾರ್ಕಿಕ exists(ಧ್ರುವ cv::String& path)
{
    CV_INSTRUMENT_REGION();

#ಯದಿ defined _WIN32 || defined WINCE
    BOOL status = TRUE;
    {
        WIN32_FILE_ATTRIBUTE_DATA all_attrs;
#ifdef WINRT
        wchar_t wpath[MAX_PATH];
        size_t copied = mbstowcs(wpath, path.c_str(), MAX_PATH);
        CV_Assert((copied != MAX_PATH) && (copied != (size_t)-1));
        status = ::GetFileAttributesExW(wpath, GetFileExInfoStandard, &all_attrs);
#ಅನ್ಯಥಾ
        status = ::GetFileAttributesExA(path.c_str(), GetFileExInfoStandard, &all_attrs);
#endif
    }

    ಪ್ರತ್ಯಾಗಮ !!status;
#ಅನ್ಯಥಾ
    ಸಂರಚನಾ stat stat_buf;
    ಪ್ರತ್ಯಾಗಮ (0 == stat(path.c_str(), &stat_buf));
#endif
}

CV_EXPORTS ಶೂನ್ಯ remove_all(ಧ್ರುವ cv::String& path)
{
    ಯದಿ (!exists(path))
        ಪ್ರತ್ಯಾಗಮ;
    ಯದಿ (isDirectory(path))
    {
        std::vector<String> entries;
        utils::fs::glob(path, cv::String(), entries, ಅಸತ್ಯ, ಸತ್ಯ);
        ಪ್ರತಿ (size_t i = 0; i < entries.size(); i++)
        {
            ಧ್ರುವ String& e = entries[i];
            remove_all(e);
        }
#ifdef _MSC_VER
        ತಾರ್ಕಿಕ result = _rmdir(path.c_str()) == 0;
#ಅನ್ಯಥಾ
        ತಾರ್ಕಿಕ result = rmdir(path.c_str()) == 0;
#endif
        ಯದಿ (!result)
        {
            CV_LOG_ERROR(ರಿಕ್ತ, "Can't remove directory: " << path);
        }
    }
    ಅನ್ಯಥಾ
    {
#ifdef _MSC_VER
        ತಾರ್ಕಿಕ result = _unlink(path.c_str()) == 0;
#ಅನ್ಯಥಾ
        ತಾರ್ಕಿಕ result = unlink(path.c_str()) == 0;
#endif
        ಯದಿ (!result)
        {
            CV_LOG_ERROR(ರಿಕ್ತ, "Can't remove file: " << path);
        }
    }
}


cv::String getcwd()
{
    CV_INSTRUMENT_REGION();
    cv::AutoBuffer<ವರ್ಣ, 4096> buf;
#ಯದಿ defined WIN32 || defined _WIN32 || defined WINCE
#ifdef WINRT
    ಪ್ರತ್ಯಾಗಮ cv::String();
#ಅನ್ಯಥಾ
    DWORD sz = GetCurrentDirectoryA(0, ರಿಕ್ತ);
    buf.allocate((size_t)sz);
    sz = GetCurrentDirectoryA((DWORD)buf.size(), buf.data());
    ಪ್ರತ್ಯಾಗಮ cv::String(buf.data(), (size_t)sz);
#endif
#ನೋಚೇದ್ಯದಿ defined __linux__ || defined __APPLE__ || defined __HAIKU__ || defined __FreeBSD__ || defined __EMSCRIPTEN__ || defined __QNX__ || defined __CYGWIN__
    ಪ್ರತಿ(;;)
    {
        ವರ್ಣ* p = ::getcwd(buf.data(), buf.size());
        ಯದಿ (p == ರಿಕ್ತ)
        {
            ಯದಿ (errno == ERANGE)
            {
                buf.allocate(buf.size() * 2);
                ಅನುವರ್ತ;
            }
            ಪ್ರತ್ಯಾಗಮ cv::String();
        }
        ಭಙ್ಗ;
    }
    ಪ್ರತ್ಯಾಗಮ cv::String(buf.data(), (size_t)strlen(buf.data()));
#ಅನ್ಯಥಾ
    ಪ್ರತ್ಯಾಗಮ cv::String();
#endif
}


ತಾರ್ಕಿಕ createDirectory(ಧ್ರುವ cv::String& path)
{
    CV_INSTRUMENT_REGION();
#ಯದಿ defined WIN32 || defined _WIN32 || defined WINCE
#ifdef WINRT
    wchar_t wpath[MAX_PATH];
    size_t copied = mbstowcs(wpath, path.c_str(), MAX_PATH);
    CV_Assert((copied != MAX_PATH) && (copied != (size_t)-1));
    ಪೂರ್ಣಾಙ್ಕ result = CreateDirectoryA(wpath, ರಿಕ್ತ) ? 0 : -1;
#ಅನ್ಯಥಾ
    ಪೂರ್ಣಾಙ್ಕ result = _mkdir(path.c_str());
#endif
#ನೋಚೇದ್ಯದಿ defined __linux__ || defined __APPLE__ || defined __HAIKU__ || defined __FreeBSD__ || defined __EMSCRIPTEN__ || defined __QNX__ || defined __CYGWIN__
    ಪೂರ್ಣಾಙ್ಕ result = mkdir(path.c_str(), 0777);
#ಅನ್ಯಥಾ
    ಪೂರ್ಣಾಙ್ಕ result = -1;
#endif

    ಯದಿ (result == -1)
    {
        ಪ್ರತ್ಯಾಗಮ isDirectory(path);
    }
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
}

ತಾರ್ಕಿಕ createDirectories(ಧ್ರುವ cv::String& path_)
{
    cv::String path = path_;
    ಪ್ರತಿ (;;)
    {
        ವರ್ಣ last_char = path.empty() ? 0 : path[path.length() - 1];
        ಯದಿ (isPathSeparator(last_char))
        {
            path = path.substr(0, path.length() - 1);
            ಅನುವರ್ತ;
        }
        ಭಙ್ಗ;
    }

    ಯದಿ (path.empty() || path == "./" || path == ".\\" || path == ".")
        ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    ಯದಿ (isDirectory(path))
        ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;

    size_t pos = path.rfind('/');
    ಯದಿ (pos == cv::String::npos)
        pos = path.rfind('\\');
    ಯದಿ (pos != cv::String::npos)
    {
        cv::String parent_directory = path.substr(0, pos);
        ಯದಿ (!parent_directory.empty())
        {
            ಯದಿ (!createDirectories(parent_directory))
                ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
        }
    }

    ಪ್ರತ್ಯಾಗಮ createDirectory(path);
}

#ifdef _WIN32

ಸಂರಚನಾ FileLock::Impl
{
    Impl(ಧ್ರುವ ವರ್ಣ* fname)
    {
        // http://support.microsoft.com/kb/316609
        ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_Retries = 5;
        ಕುರು
        {
            handle = ::CreateFileA(fname, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, ರಿಕ್ತ,
                                OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, ರಿಕ್ತ);
            ಯದಿ (INVALID_HANDLE_ಮೌಲ್ಯ == handle)
            {
                ಯದಿ (ERROR_SHARING_VIOLATION == GetLastError())
                {
                    ಅಂಕೆ_Retries--;
                    Sleep(250);
                    ಅನುವರ್ತ;
                }
                ಅನ್ಯಥಾ
                {
                    CV_Error_(Error::StsAssert, ("Can't open lock file: %s", fname));
                }
            }
            ಭಙ್ಗ;
        } ಯಾವತ್ (ಅಂಕೆ_Retries > 0);
    }
    ~Impl()
    {
        ಯದಿ (INVALID_HANDLE_ಮೌಲ್ಯ != handle)
        {
            ::CloseHandle(handle);
        }
    }

    ತಾರ್ಕಿಕ lock()
    {
        OVERLAPPED overlapped;
        std::memset(&overlapped, 0, ಪರಿಮಾಣ(overlapped));
        ಪ್ರತ್ಯಾಗಮ !!::LockFileEx(handle, LOCKFILE_EXCLUSIVE_LOCK, 0, MAXDWORD, MAXDWORD, &overlapped);
    }
    ತಾರ್ಕಿಕ unlock()
    {
        OVERLAPPED overlapped;
        std::memset(&overlapped, 0, ಪರಿಮಾಣ(overlapped));
        ಪ್ರತ್ಯಾಗಮ !!::UnlockFileEx(handle, 0, MAXDWORD, MAXDWORD, &overlapped);
    }

    ತಾರ್ಕಿಕ lock_shared()
    {
        OVERLAPPED overlapped;
        std::memset(&overlapped, 0, ಪರಿಮಾಣ(overlapped));
        ಪ್ರತ್ಯಾಗಮ !!::LockFileEx(handle, 0, 0, MAXDWORD, MAXDWORD, &overlapped);
    }
    ತಾರ್ಕಿಕ unlock_shared()
    {
        ಪ್ರತ್ಯಾಗಮ unlock();
    }

    HANDLE handle;

ಗುಪ್ತ:
    Impl(ಧ್ರುವ Impl&); // disabled
    Impl& operator=(ಧ್ರುವ Impl&); // disabled
};

#ನೋಚೇದ್ಯದಿ defined __linux__ || defined __APPLE__ || defined __HAIKU__ || defined __FreeBSD__ || defined __GNU__ || defined __EMSCRIPTEN__ || defined __QNX__ || defined __CYGWIN__

ಸಂರಚನಾ FileLock::Impl
{
    Impl(ಧ್ರುವ ವರ್ಣ* fname)
    {
        handle = ::open(fname, O_RDWR);
        CV_Assert(handle != -1);
    }
    ~Impl()
    {
        ಯದಿ (handle >= 0)
            ::close(handle);
    }

    ತಾರ್ಕಿಕ lock()
    {
        ಸಂರಚನಾ ::flock l;
        std::memset(&l, 0, ಪರಿಮಾಣ(l));
        l.l_type = F_WRLCK;
        l.l_whence = SEEK_SET;
        l.l_start = 0;
        l.l_len = 0;
        DBG(std::cout << "Lock..." << std::endl);
        ತಾರ್ಕಿಕ res = -1 != ::fcntl(handle, F_SETLKW, &l);
        ಪ್ರತ್ಯಾಗಮ res;
    }
    ತಾರ್ಕಿಕ unlock()
    {
        ಸಂರಚನಾ ::flock l;
        std::memset(&l, 0, ಪರಿಮಾಣ(l));
        l.l_type = F_UNLCK;
        l.l_whence = SEEK_SET;
        l.l_start = 0;
        l.l_len = 0;
        DBG(std::cout << "Unlock..." << std::endl);
        ತಾರ್ಕಿಕ res = -1 != ::fcntl(handle, F_SETLK, &l);
        ಪ್ರತ್ಯಾಗಮ res;
    }

    ತಾರ್ಕಿಕ lock_shared()
    {
        ಸಂರಚನಾ ::flock l;
        std::memset(&l, 0, ಪರಿಮಾಣ(l));
        l.l_type = F_RDLCK;
        l.l_whence = SEEK_SET;
        l.l_start = 0;
        l.l_len = 0;
        DBG(std::cout << "Lock read..." << std::endl);
        ತಾರ್ಕಿಕ res = -1 != ::fcntl(handle, F_SETLKW, &l);
        ಪ್ರತ್ಯಾಗಮ res;
    }
    ತಾರ್ಕಿಕ unlock_shared()
    {
        ಪ್ರತ್ಯಾಗಮ unlock();
    }

    ಪೂರ್ಣಾಙ್ಕ handle;

ಗುಪ್ತ:
    Impl(ಧ್ರುವ Impl&); // disabled
    Impl& operator=(ಧ್ರುವ Impl&); // disabled
};

#endif

FileLock::FileLock(ಧ್ರುವ ವರ್ಣ* fname)
    : pImpl(ನೂತನ Impl(fname))
{
    // nothing
}
FileLock::~FileLock()
{
    delete pImpl;
    pImpl = ರಿಕ್ತ;
}

ಶೂನ್ಯ FileLock::lock() { CV_Assert(pImpl->lock()); }
ಶೂನ್ಯ FileLock::unlock() { CV_Assert(pImpl->unlock()); }
ಶೂನ್ಯ FileLock::lock_shared() { CV_Assert(pImpl->lock_shared()); }
ಶೂನ್ಯ FileLock::unlock_shared() { CV_Assert(pImpl->unlock_shared()); }



cv::String getCacheDirectory(ಧ್ರುವ ವರ್ಣ* sub_directory_name, ಧ್ರುವ ವರ್ಣ* configuration_name)
{
    String cache_path;
    ಯದಿ (configuration_name)
    {
        cache_path = utils::getConfigurationParameterString(configuration_name, "");
    }
    ಯದಿ (cache_path.empty())
    {
        cv::String default_cache_path;
#ifdef _WIN32
        wchar_t tmp_path_buf_w[MAX_PATH+1] = {0};
        DWORD res = GetTempPathW(MAX_PATH, tmp_path_buf_w);
        ಯದಿ (res > 0 && res <= MAX_PATH)
        {
            // Convert from UTF-16 to UTF-8 to support Unicode paths
            ಪೂರ್ಣಾಙ್ಕ len = WideCharToMultiByte(CP_UTF8, 0, tmp_path_buf_w, -1, ರಿಕ್ತ, 0, ರಿಕ್ತ, ರಿಕ್ತ);
            ಯದಿ (len > 0)
            {
                std::vector<ವರ್ಣ> utf8_buf(len);
                WideCharToMultiByte(CP_UTF8, 0, tmp_path_buf_w, -1, utf8_buf.data(), len, ರಿಕ್ತ, ರಿಕ್ತ);
                default_cache_path = std::string(utf8_buf.data());
            }
        }
#ನೋಚೇದ್ಯದಿ defined __ANDROID__
        // no defaults
#ನೋಚೇದ್ಯದಿ defined __APPLE__
        ಧ್ರುವ std::string tmpdir_env = utils::getConfigurationParameterString("TMPDIR");
        ಯದಿ (!tmpdir_env.empty() && utils::fs::isDirectory(tmpdir_env))
        {
            default_cache_path = tmpdir_env;
        }
        ಅನ್ಯಥಾ
        {
            default_cache_path = "/tmp/";
            CV_LOG_WARNING(ರಿಕ್ತ, "Using world accessible cache directory. This may be not secure: " << default_cache_path);
        }
#ನೋಚೇದ್ಯದಿ defined __linux__ || defined __HAIKU__ || defined __FreeBSD__ || defined __GNU__
        // https://specifications.freedesktop.org/basedir-spec/basedir-spec-latest.html
        ಯದಿ (default_cache_path.empty())
        {
            ಧ್ರುವ std::string xdg_cache_env = utils::getConfigurationParameterString("XDG_CACHE_HOME");
            ಯದಿ (!xdg_cache_env.empty() && utils::fs::isDirectory(xdg_cache_env))
            {
                default_cache_path = xdg_cache_env;
            }
        }
        ಯದಿ (default_cache_path.empty())
        {
            ಧ್ರುವ std::string home_env = utils::getConfigurationParameterString("HOME");
            ಯದಿ (!home_env.empty() && utils::fs::isDirectory(home_env))
            {
                cv::String home_path = home_env;
                cv::String home_cache_path = utils::fs::join(home_path, ".cache/");
                ಯದಿ (utils::fs::isDirectory(home_cache_path))
                {
                    default_cache_path = home_cache_path;
                }
            }
        }
        ಯದಿ (default_cache_path.empty())
        {
            ಧ್ರುವ ವರ್ಣ* temp_path = "/var/tmp/";
            ಯದಿ (utils::fs::isDirectory(temp_path))
            {
                default_cache_path = temp_path;
                CV_LOG_WARNING(ರಿಕ್ತ, "Using world accessible cache directory. This may be not secure: " << default_cache_path);
            }
        }
        ಯದಿ (default_cache_path.empty())
        {
            default_cache_path = "/tmp/";
            CV_LOG_WARNING(ರಿಕ್ತ, "Using world accessible cache directory. This may be not secure: " << default_cache_path);
        }
#ಅನ್ಯಥಾ
        // no defaults
#endif
        CV_LOG_VERBOSE(ರಿಕ್ತ, 0, "default_cache_path = " << default_cache_path);
        ಯದಿ (!default_cache_path.empty())
        {
            ಯದಿ (utils::fs::isDirectory(default_cache_path))
            {
                cv::String default_cache_path_base = utils::fs::join(default_cache_path, "opencv");
                default_cache_path = utils::fs::join(default_cache_path_base, CVAUX_STR(CV_VERSION_MAJOR) ".x" CV_VERSION_STATUS);
                ಯದಿ (utils::getConfigurationParameterBool("OPENCV_CACHE_SHOW_CLEANUP_MESSAGE", ಸತ್ಯ)
                    && !utils::fs::isDirectory(default_cache_path))
                {
                    std::vector<cv::String> existedCacheDirs;
                    ಯತ್ನ
                    {
                        utils::fs::glob_relative(default_cache_path_base, "*", existedCacheDirs, ಅಸತ್ಯ, ಸತ್ಯ);
                    }
                    ಗೃಹಾಣ (...)
                    {
                        // ignore
                    }
                    ಯದಿ (!existedCacheDirs.empty())
                    {
                        CV_LOG_WARNING(ರಿಕ್ತ, "Creating new OpenCV cache directory: " << default_cache_path);
                        CV_LOG_WARNING(ರಿಕ್ತ, "There are several neighbour directories, probably created by old OpenCV versions.");
                        CV_LOG_WARNING(ರಿಕ್ತ, "Feel free to cleanup these unused directories:");
                        ಪ್ರತಿ (size_t i = 0; i < existedCacheDirs.size(); i++)
                        {
                            CV_LOG_WARNING(ರಿಕ್ತ, "  - " << existedCacheDirs[i]);
                        }
                        CV_LOG_WARNING(ರಿಕ್ತ, "Note: This message is showed only once.");
                    }
                }
                ಯದಿ (sub_directory_name && sub_directory_name[0] != '\0')
                    default_cache_path = utils::fs::join(default_cache_path, cv::String(sub_directory_name) + native_separator);
                ಯದಿ (!utils::fs::createDirectories(default_cache_path))
                {
                    CV_LOG_DEBUG(ರಿಕ್ತ, "Can't create OpenCV cache sub-directory: " << default_cache_path);
                }
                ಅನ್ಯಥಾ
                {
                    cache_path = default_cache_path;
                }
            }
            ಅನ್ಯಥಾ
            {
                CV_LOG_INFO(ರಿಕ್ತ, "Can't find default cache directory (does it exist?): " << default_cache_path);
            }
        }
        ಅನ್ಯಥಾ
        {
            CV_LOG_DEBUG(ರಿಕ್ತ, "OpenCV has no support to discover default cache directory on the current platform");
        }
    }
    ಅನ್ಯಥಾ
    {
        ಯದಿ (cache_path == "disabled")
            ಪ್ರತ್ಯಾಗಮ cache_path;
        ಯದಿ (!isDirectory(cache_path))
        {
            CV_LOG_WARNING(ರಿಕ್ತ, "Specified non-existed directory, creating OpenCV sub-directory for caching purposes: " << cache_path);
            ಯದಿ (!createDirectories(cache_path))
            {
                CV_LOG_ERROR(ರಿಕ್ತ, "Can't create OpenCV cache sub-directory: " << cache_path);
                cache_path.clear();
            }
        }
    }
    CV_Assert(cache_path.empty() || utils::fs::isDirectory(cache_path));
    ಯದಿ (!cache_path.empty())
    {
        ಯದಿ (!isPathSeparator(cache_path[cache_path.size() - 1]))
        {
            cache_path += native_separator;
        }
    }
    ಪ್ರತ್ಯಾಗಮ cache_path;
}

#ಅನ್ಯಥಾ
#define NOT_IMPLEMENTED CV_Error(Error::StsNotImplemented, "");
cv::String canonical(ಧ್ರುವ cv::String& /*path*/) { NOT_IMPLEMENTED }
ತಾರ್ಕಿಕ exists(ಧ್ರುವ cv::String& /*path*/) { NOT_IMPLEMENTED }
ಶೂನ್ಯ remove_all(ಧ್ರುವ cv::String& /*path*/) { NOT_IMPLEMENTED }
cv::String getcwd() { NOT_IMPLEMENTED }
ತಾರ್ಕಿಕ createDirectory(ಧ್ರುವ cv::String& /*path*/) { NOT_IMPLEMENTED }
ತಾರ್ಕಿಕ createDirectories(ಧ್ರುವ cv::String& /*path*/) { NOT_IMPLEMENTED }
cv::String getCacheDirectory(ಧ್ರುವ ವರ್ಣ* /*sub_directory_name*/, ಧ್ರುವ ವರ್ಣ* /*configuration_name = NULL*/) { NOT_IMPLEMENTED }
#undef NOT_IMPLEMENTED
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT

}}} // namespace


#ಯದಿ OPENCV_HAVE_FILESYSTEM_SUPPORT
#ಅನ್ತರ್ಭಾವ "plugin_loader.impl.hpp"
#endif
