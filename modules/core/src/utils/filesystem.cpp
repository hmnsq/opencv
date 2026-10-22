// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#অন্তর্ভাব "../precomp.hpp"

#অন্তর্ভাব <opencv2/core/utils/configuration.private.hpp>

#অন্তর্ভাব <opencv2/core/utils/logger.hpp>

#অন্তর্ভাব "opencv2/core/utils/filesystem.private.hpp"
#অন্তর্ভাব "opencv2/core/utils/filesystem.hpp"

//#define DEBUG_FS_UTILS

#ifdef DEBUG_FS_UTILS
#অন্তর্ভাব <iostream>
#define DBG(...) __VA_ARGS__
#অন্যথা
#define DBG(...)
#endif


#যদি OPENCV_HAVE_FILESYSTEM_SUPPORT

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#undef NOMINMAX
#define NOMINMAX
#অন্তর্ভাব <windows.h>
#অন্তর্ভাব <direct.h>
#অন্তর্ভাব <sys/types.h>
#অন্তর্ভাব <sys/stat.h>
#অন্তর্ভাব <errno.h>
#অন্তর্ভাব <io.h>
#অন্তর্ভাব <stdio.h>
#নোচেদ্যদি defined __linux__ || defined __APPLE__ || defined __HAIKU__ || defined __FreeBSD__ || defined __GNU__ || defined __EMSCRIPTEN__ || defined __QNX__ || defined __CYGWIN__
#অন্তর্ভাব <sys/types.h>
#অন্তর্ভাব <sys/stat.h>
#অন্তর্ভাব <fcntl.h>
#অন্তর্ভাব <unistd.h>
#অন্তর্ভাব <errno.h>
#endif

#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT

নামাকাশ cv { নামাকাশ utils { নামাকাশ fs {

#ifdef _WIN32
স্থির ধ্রুব বর্ণ native_separator = '\\';
#অন্যথা
স্থির ধ্রুব বর্ণ native_separator = '/';
#endif

স্থির অন্তঃস্থ
তার্কিক isPathSeparator(বর্ণ c)
{
    প্রত্যাগম c == '/' || c == '\\';
}

cv::String join(ধ্রুব cv::String& base, ধ্রুব cv::String& path)
{
    যদি (base.empty())
        প্রত্যাগম path;
    যদি (path.empty())
        প্রত্যাগম base;

    তার্কিক baseSep = isPathSeparator(base[base.size() - 1]);
    তার্কিক pathSep = isPathSeparator(path[0]);
    String result;
    যদি (baseSep && pathSep)
    {
        result = base + path.substr(1);
    }
    অন্যথা যদি (!baseSep && !pathSep)
    {
        result = base + native_separator + path;
    }
    অন্যথা
    {
        result = base + path;
    }
    প্রত্যাগম result;
}

CV_EXPORTS cv::String getParent(ধ্রুব cv::String &path)
{
    std::string::size_type loc = path.find_last_of("/\\");
    যদি (loc == std::string::npos)
        প্রত্যাগম std::string();
    প্রত্যাগম std::string(path, 0, loc);
}

CV_EXPORTS std::wstring getParent(ধ্রুব std::wstring& path)
{
    std::wstring::size_type loc = path.find_last_of(L"/\\");
    যদি (loc == std::wstring::npos)
        প্রত্যাগম std::wstring();
    প্রত্যাগম std::wstring(path, 0, loc);
}

#যদি OPENCV_HAVE_FILESYSTEM_SUPPORT

cv::String canonical(ধ্রুব cv::String& path)
{
    cv::String result;
#ifdef _WIN32
    ধ্রুব বর্ণ* result_str = _fullpath(রিক্ত, path.c_str(), 0);
#অন্যথা
    ধ্রুব বর্ণ* result_str = realpath(path.c_str(), রিক্ত);
#endif
    যদি (result_str)
    {
        result = cv::String(result_str);
        মুক্ত((শূন্য*)result_str);
    }
    প্রত্যাগম result.empty() ? path : result;
}


তার্কিক exists(ধ্রুব cv::String& path)
{
    CV_INSTRUMENT_REGION();

#যদি defined _WIN32 || defined WINCE
    BOOL status = TRUE;
    {
        WIN32_FILE_ATTRIBUTE_DATA all_attrs;
#ifdef WINRT
        wchar_t wpath[MAX_PATH];
        size_t copied = mbstowcs(wpath, path.c_str(), MAX_PATH);
        CV_Assert((copied != MAX_PATH) && (copied != (size_t)-1));
        status = ::GetFileAttributesExW(wpath, GetFileExInfoStandard, &all_attrs);
#অন্যথা
        status = ::GetFileAttributesExA(path.c_str(), GetFileExInfoStandard, &all_attrs);
#endif
    }

    প্রত্যাগম !!status;
#অন্যথা
    সংরচনা stat stat_buf;
    প্রত্যাগম (0 == stat(path.c_str(), &stat_buf));
#endif
}

CV_EXPORTS শূন্য remove_all(ধ্রুব cv::String& path)
{
    যদি (!exists(path))
        প্রত্যাগম;
    যদি (isDirectory(path))
    {
        std::vector<String> entries;
        utils::fs::glob(path, cv::String(), entries, অসত্য, সত্য);
        প্রতি (size_t i = 0; i < entries.size(); i++)
        {
            ধ্রুব String& e = entries[i];
            remove_all(e);
        }
#ifdef _MSC_VER
        তার্কিক result = _rmdir(path.c_str()) == 0;
#অন্যথা
        তার্কিক result = rmdir(path.c_str()) == 0;
#endif
        যদি (!result)
        {
            CV_LOG_ERROR(রিক্ত, "Can't remove directory: " << path);
        }
    }
    অন্যথা
    {
#ifdef _MSC_VER
        তার্কিক result = _unlink(path.c_str()) == 0;
#অন্যথা
        তার্কিক result = unlink(path.c_str()) == 0;
#endif
        যদি (!result)
        {
            CV_LOG_ERROR(রিক্ত, "Can't remove file: " << path);
        }
    }
}


cv::String getcwd()
{
    CV_INSTRUMENT_REGION();
    cv::AutoBuffer<বর্ণ, 4096> buf;
#যদি defined WIN32 || defined _WIN32 || defined WINCE
#ifdef WINRT
    প্রত্যাগম cv::String();
#অন্যথা
    DWORD sz = GetCurrentDirectoryA(0, রিক্ত);
    buf.allocate((size_t)sz);
    sz = GetCurrentDirectoryA((DWORD)buf.size(), buf.data());
    প্রত্যাগম cv::String(buf.data(), (size_t)sz);
#endif
#নোচেদ্যদি defined __linux__ || defined __APPLE__ || defined __HAIKU__ || defined __FreeBSD__ || defined __EMSCRIPTEN__ || defined __QNX__ || defined __CYGWIN__
    প্রতি(;;)
    {
        বর্ণ* p = ::getcwd(buf.data(), buf.size());
        যদি (p == রিক্ত)
        {
            যদি (errno == ERANGE)
            {
                buf.allocate(buf.size() * 2);
                অনুবর্ত;
            }
            প্রত্যাগম cv::String();
        }
        ভঙ্গ;
    }
    প্রত্যাগম cv::String(buf.data(), (size_t)strlen(buf.data()));
#অন্যথা
    প্রত্যাগম cv::String();
#endif
}


তার্কিক createDirectory(ধ্রুব cv::String& path)
{
    CV_INSTRUMENT_REGION();
#যদি defined WIN32 || defined _WIN32 || defined WINCE
#ifdef WINRT
    wchar_t wpath[MAX_PATH];
    size_t copied = mbstowcs(wpath, path.c_str(), MAX_PATH);
    CV_Assert((copied != MAX_PATH) && (copied != (size_t)-1));
    পূর্ণাঙ্ক result = CreateDirectoryA(wpath, রিক্ত) ? 0 : -1;
#অন্যথা
    পূর্ণাঙ্ক result = _mkdir(path.c_str());
#endif
#নোচেদ্যদি defined __linux__ || defined __APPLE__ || defined __HAIKU__ || defined __FreeBSD__ || defined __EMSCRIPTEN__ || defined __QNX__ || defined __CYGWIN__
    পূর্ণাঙ্ক result = mkdir(path.c_str(), 0777);
#অন্যথা
    পূর্ণাঙ্ক result = -1;
#endif

    যদি (result == -1)
    {
        প্রত্যাগম isDirectory(path);
    }
    প্রত্যাগম সত্য;
}

তার্কিক createDirectories(ধ্রুব cv::String& path_)
{
    cv::String path = path_;
    প্রতি (;;)
    {
        বর্ণ last_char = path.empty() ? 0 : path[path.length() - 1];
        যদি (isPathSeparator(last_char))
        {
            path = path.substr(0, path.length() - 1);
            অনুবর্ত;
        }
        ভঙ্গ;
    }

    যদি (path.empty() || path == "./" || path == ".\\" || path == ".")
        প্রত্যাগম সত্য;
    যদি (isDirectory(path))
        প্রত্যাগম সত্য;

    size_t pos = path.rfind('/');
    যদি (pos == cv::String::npos)
        pos = path.rfind('\\');
    যদি (pos != cv::String::npos)
    {
        cv::String parent_directory = path.substr(0, pos);
        যদি (!parent_directory.empty())
        {
            যদি (!createDirectories(parent_directory))
                প্রত্যাগম অসত্য;
        }
    }

    প্রত্যাগম createDirectory(path);
}

#ifdef _WIN32

সংরচনা FileLock::Impl
{
    Impl(ধ্রুব বর্ণ* fname)
    {
        // http://support.microsoft.com/kb/316609
        পূর্ণাঙ্ক অঙ্ক_Retries = 5;
        কুরু
        {
            handle = ::CreateFileA(fname, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, রিক্ত,
                                OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, রিক্ত);
            যদি (INVALID_HANDLE_মান == handle)
            {
                যদি (ERROR_SHARING_VIOLATION == GetLastError())
                {
                    অঙ্ক_Retries--;
                    Sleep(250);
                    অনুবর্ত;
                }
                অন্যথা
                {
                    CV_Error_(Error::StsAssert, ("Can't open lock file: %s", fname));
                }
            }
            ভঙ্গ;
        } যাবত্ (অঙ্ক_Retries > 0);
    }
    ~Impl()
    {
        যদি (INVALID_HANDLE_মান != handle)
        {
            ::CloseHandle(handle);
        }
    }

    তার্কিক lock()
    {
        OVERLAPPED overlapped;
        std::memset(&overlapped, 0, পরিমাণ(overlapped));
        প্রত্যাগম !!::LockFileEx(handle, LOCKFILE_EXCLUSIVE_LOCK, 0, MAXDWORD, MAXDWORD, &overlapped);
    }
    তার্কিক unlock()
    {
        OVERLAPPED overlapped;
        std::memset(&overlapped, 0, পরিমাণ(overlapped));
        প্রত্যাগম !!::UnlockFileEx(handle, 0, MAXDWORD, MAXDWORD, &overlapped);
    }

    তার্কিক lock_shared()
    {
        OVERLAPPED overlapped;
        std::memset(&overlapped, 0, পরিমাণ(overlapped));
        প্রত্যাগম !!::LockFileEx(handle, 0, 0, MAXDWORD, MAXDWORD, &overlapped);
    }
    তার্কিক unlock_shared()
    {
        প্রত্যাগম unlock();
    }

    HANDLE handle;

গুপ্ত:
    Impl(ধ্রুব Impl&); // disabled
    Impl& operator=(ধ্রুব Impl&); // disabled
};

#নোচেদ্যদি defined __linux__ || defined __APPLE__ || defined __HAIKU__ || defined __FreeBSD__ || defined __GNU__ || defined __EMSCRIPTEN__ || defined __QNX__ || defined __CYGWIN__

সংরচনা FileLock::Impl
{
    Impl(ধ্রুব বর্ণ* fname)
    {
        handle = ::open(fname, O_RDWR);
        CV_Assert(handle != -1);
    }
    ~Impl()
    {
        যদি (handle >= 0)
            ::close(handle);
    }

    তার্কিক lock()
    {
        সংরচনা ::flock l;
        std::memset(&l, 0, পরিমাণ(l));
        l.l_type = F_WRLCK;
        l.l_whence = SEEK_SET;
        l.l_start = 0;
        l.l_len = 0;
        DBG(std::cout << "Lock..." << std::endl);
        তার্কিক res = -1 != ::fcntl(handle, F_SETLKW, &l);
        প্রত্যাগম res;
    }
    তার্কিক unlock()
    {
        সংরচনা ::flock l;
        std::memset(&l, 0, পরিমাণ(l));
        l.l_type = F_UNLCK;
        l.l_whence = SEEK_SET;
        l.l_start = 0;
        l.l_len = 0;
        DBG(std::cout << "Unlock..." << std::endl);
        তার্কিক res = -1 != ::fcntl(handle, F_SETLK, &l);
        প্রত্যাগম res;
    }

    তার্কিক lock_shared()
    {
        সংরচনা ::flock l;
        std::memset(&l, 0, পরিমাণ(l));
        l.l_type = F_RDLCK;
        l.l_whence = SEEK_SET;
        l.l_start = 0;
        l.l_len = 0;
        DBG(std::cout << "Lock read..." << std::endl);
        তার্কিক res = -1 != ::fcntl(handle, F_SETLKW, &l);
        প্রত্যাগম res;
    }
    তার্কিক unlock_shared()
    {
        প্রত্যাগম unlock();
    }

    পূর্ণাঙ্ক handle;

গুপ্ত:
    Impl(ধ্রুব Impl&); // disabled
    Impl& operator=(ধ্রুব Impl&); // disabled
};

#endif

FileLock::FileLock(ধ্রুব বর্ণ* fname)
    : pImpl(নূতন Impl(fname))
{
    // nothing
}
FileLock::~FileLock()
{
    delete pImpl;
    pImpl = রিক্ত;
}

শূন্য FileLock::lock() { CV_Assert(pImpl->lock()); }
শূন্য FileLock::unlock() { CV_Assert(pImpl->unlock()); }
শূন্য FileLock::lock_shared() { CV_Assert(pImpl->lock_shared()); }
শূন্য FileLock::unlock_shared() { CV_Assert(pImpl->unlock_shared()); }



cv::String getCacheDirectory(ধ্রুব বর্ণ* sub_directory_name, ধ্রুব বর্ণ* configuration_name)
{
    String cache_path;
    যদি (configuration_name)
    {
        cache_path = utils::getConfigurationParameterString(configuration_name, "");
    }
    যদি (cache_path.empty())
    {
        cv::String default_cache_path;
#ifdef _WIN32
        wchar_t tmp_path_buf_w[MAX_PATH+1] = {0};
        DWORD res = GetTempPathW(MAX_PATH, tmp_path_buf_w);
        যদি (res > 0 && res <= MAX_PATH)
        {
            // Convert from UTF-16 to UTF-8 to support Unicode paths
            পূর্ণাঙ্ক len = WideCharToMultiByte(CP_UTF8, 0, tmp_path_buf_w, -1, রিক্ত, 0, রিক্ত, রিক্ত);
            যদি (len > 0)
            {
                std::vector<বর্ণ> utf8_buf(len);
                WideCharToMultiByte(CP_UTF8, 0, tmp_path_buf_w, -1, utf8_buf.data(), len, রিক্ত, রিক্ত);
                default_cache_path = std::string(utf8_buf.data());
            }
        }
#নোচেদ্যদি defined __ANDROID__
        // no defaults
#নোচেদ্যদি defined __APPLE__
        ধ্রুব std::string tmpdir_env = utils::getConfigurationParameterString("TMPDIR");
        যদি (!tmpdir_env.empty() && utils::fs::isDirectory(tmpdir_env))
        {
            default_cache_path = tmpdir_env;
        }
        অন্যথা
        {
            default_cache_path = "/tmp/";
            CV_LOG_WARNING(রিক্ত, "Using world accessible cache directory. This may be not secure: " << default_cache_path);
        }
#নোচেদ্যদি defined __linux__ || defined __HAIKU__ || defined __FreeBSD__ || defined __GNU__
        // https://specifications.freedesktop.org/basedir-spec/basedir-spec-latest.html
        যদি (default_cache_path.empty())
        {
            ধ্রুব std::string xdg_cache_env = utils::getConfigurationParameterString("XDG_CACHE_HOME");
            যদি (!xdg_cache_env.empty() && utils::fs::isDirectory(xdg_cache_env))
            {
                default_cache_path = xdg_cache_env;
            }
        }
        যদি (default_cache_path.empty())
        {
            ধ্রুব std::string home_env = utils::getConfigurationParameterString("HOME");
            যদি (!home_env.empty() && utils::fs::isDirectory(home_env))
            {
                cv::String home_path = home_env;
                cv::String home_cache_path = utils::fs::join(home_path, ".cache/");
                যদি (utils::fs::isDirectory(home_cache_path))
                {
                    default_cache_path = home_cache_path;
                }
            }
        }
        যদি (default_cache_path.empty())
        {
            ধ্রুব বর্ণ* temp_path = "/var/tmp/";
            যদি (utils::fs::isDirectory(temp_path))
            {
                default_cache_path = temp_path;
                CV_LOG_WARNING(রিক্ত, "Using world accessible cache directory. This may be not secure: " << default_cache_path);
            }
        }
        যদি (default_cache_path.empty())
        {
            default_cache_path = "/tmp/";
            CV_LOG_WARNING(রিক্ত, "Using world accessible cache directory. This may be not secure: " << default_cache_path);
        }
#অন্যথা
        // no defaults
#endif
        CV_LOG_VERBOSE(রিক্ত, 0, "default_cache_path = " << default_cache_path);
        যদি (!default_cache_path.empty())
        {
            যদি (utils::fs::isDirectory(default_cache_path))
            {
                cv::String default_cache_path_base = utils::fs::join(default_cache_path, "opencv");
                default_cache_path = utils::fs::join(default_cache_path_base, CVAUX_STR(CV_VERSION_MAJOR) ".x" CV_VERSION_STATUS);
                যদি (utils::getConfigurationParameterBool("OPENCV_CACHE_SHOW_CLEANUP_MESSAGE", সত্য)
                    && !utils::fs::isDirectory(default_cache_path))
                {
                    std::vector<cv::String> existedCacheDirs;
                    যত্ন
                    {
                        utils::fs::glob_relative(default_cache_path_base, "*", existedCacheDirs, অসত্য, সত্য);
                    }
                    গৃহাণ (...)
                    {
                        // ignore
                    }
                    যদি (!existedCacheDirs.empty())
                    {
                        CV_LOG_WARNING(রিক্ত, "Creating new OpenCV cache directory: " << default_cache_path);
                        CV_LOG_WARNING(রিক্ত, "There are several neighbour directories, probably created by old OpenCV versions.");
                        CV_LOG_WARNING(রিক্ত, "Feel free to cleanup these unused directories:");
                        প্রতি (size_t i = 0; i < existedCacheDirs.size(); i++)
                        {
                            CV_LOG_WARNING(রিক্ত, "  - " << existedCacheDirs[i]);
                        }
                        CV_LOG_WARNING(রিক্ত, "Note: This message is showed only once.");
                    }
                }
                যদি (sub_directory_name && sub_directory_name[0] != '\0')
                    default_cache_path = utils::fs::join(default_cache_path, cv::String(sub_directory_name) + native_separator);
                যদি (!utils::fs::createDirectories(default_cache_path))
                {
                    CV_LOG_DEBUG(রিক্ত, "Can't create OpenCV cache sub-directory: " << default_cache_path);
                }
                অন্যথা
                {
                    cache_path = default_cache_path;
                }
            }
            অন্যথা
            {
                CV_LOG_INFO(রিক্ত, "Can't find default cache directory (does it exist?): " << default_cache_path);
            }
        }
        অন্যথা
        {
            CV_LOG_DEBUG(রিক্ত, "OpenCV has no support to discover default cache directory on the current platform");
        }
    }
    অন্যথা
    {
        যদি (cache_path == "disabled")
            প্রত্যাগম cache_path;
        যদি (!isDirectory(cache_path))
        {
            CV_LOG_WARNING(রিক্ত, "Specified non-existed directory, creating OpenCV sub-directory for caching purposes: " << cache_path);
            যদি (!createDirectories(cache_path))
            {
                CV_LOG_ERROR(রিক্ত, "Can't create OpenCV cache sub-directory: " << cache_path);
                cache_path.clear();
            }
        }
    }
    CV_Assert(cache_path.empty() || utils::fs::isDirectory(cache_path));
    যদি (!cache_path.empty())
    {
        যদি (!isPathSeparator(cache_path[cache_path.size() - 1]))
        {
            cache_path += native_separator;
        }
    }
    প্রত্যাগম cache_path;
}

#অন্যথা
#define NOT_IMPLEMENTED CV_Error(Error::StsNotImplemented, "");
cv::String canonical(ধ্রুব cv::String& /*path*/) { NOT_IMPLEMENTED }
তার্কিক exists(ধ্রুব cv::String& /*path*/) { NOT_IMPLEMENTED }
শূন্য remove_all(ধ্রুব cv::String& /*path*/) { NOT_IMPLEMENTED }
cv::String getcwd() { NOT_IMPLEMENTED }
তার্কিক createDirectory(ধ্রুব cv::String& /*path*/) { NOT_IMPLEMENTED }
তার্কিক createDirectories(ধ্রুব cv::String& /*path*/) { NOT_IMPLEMENTED }
cv::String getCacheDirectory(ধ্রুব বর্ণ* /*sub_directory_name*/, ধ্রুব বর্ণ* /*configuration_name = NULL*/) { NOT_IMPLEMENTED }
#undef NOT_IMPLEMENTED
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT

}}} // namespace


#যদি OPENCV_HAVE_FILESYSTEM_SUPPORT
#অন্তর্ভাব "plugin_loader.impl.hpp"
#endif
