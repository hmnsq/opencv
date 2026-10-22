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
#নতুবা_যদি defined __linux__ || defined __APPLE__ || defined __HAIKU__ || defined __FreeBSD__ || defined __GNU__ || defined __EMSCRIPTEN__ || defined __QNX__ || defined __CYGWIN__
#অন্তর্ভাব <sys/types.h>
#অন্তর্ভাব <sys/stat.h>
#অন্তর্ভাব <fcntl.h>
#অন্তর্ভাব <unistd.h>
#অন্তর্ভাব <errno.h>
#endif

#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT

নামাকাশ cv { নামাকাশ utils { নামাকাশ fs {

#ifdef _WIN32
স্থির ধ্রুবক অক্ষর native_separator = '\\';
#অন্যথা
স্থির ধ্রুবক অক্ষর native_separator = '/';
#endif

স্থির অন্তঃরেখ
যৌক্তিক isPathSeparator(অক্ষর c)
{
    ফেরত c == '/' || c == '\\';
}

cv::String join(ধ্রুবক cv::String& base, ধ্রুবক cv::String& path)
{
    যদি (base.empty())
        ফেরত path;
    যদি (path.empty())
        ফেরত base;

    যৌক্তিক baseSep = isPathSeparator(base[base.size() - 1]);
    যৌক্তিক pathSep = isPathSeparator(path[0]);
    String ফলাফল;
    যদি (baseSep && pathSep)
    {
        ফলাফল = base + path.substr(1);
    }
    অন্যথা যদি (!baseSep && !pathSep)
    {
        ফলাফল = base + native_separator + path;
    }
    অন্যথা
    {
        ফলাফল = base + path;
    }
    ফেরত ফলাফল;
}

CV_EXPORTS cv::String getParent(ধ্রুবক cv::String &path)
{
    std::string::size_type loc = path.find_last_of("/\\");
    যদি (loc == std::string::npos)
        ফেরত std::string();
    ফেরত std::string(path, 0, loc);
}

CV_EXPORTS std::wstring getParent(ধ্রুবক std::wstring& path)
{
    std::wstring::size_type loc = path.find_last_of(L"/\\");
    যদি (loc == std::wstring::npos)
        ফেরত std::wstring();
    ফেরত std::wstring(path, 0, loc);
}

#যদি OPENCV_HAVE_FILESYSTEM_SUPPORT

cv::String canonical(ধ্রুবক cv::String& path)
{
    cv::String ফলাফল;
#ifdef _WIN32
    ধ্রুবক অক্ষর* ফলাফল_str = _fullpath(ফাঁকা, path.c_str(), 0);
#অন্যথা
    ধ্রুবক অক্ষর* ফলাফল_str = realpath(path.c_str(), ফাঁকা);
#endif
    যদি (ফলাফল_str)
    {
        ফলাফল = cv::String(ফলাফল_str);
        মুক্ত((শূন্য*)ফলাফল_str);
    }
    ফেরত ফলাফল.empty() ? path : ফলাফল;
}


যৌক্তিক exists(ধ্রুবক cv::String& path)
{
    CV_INSTRUMENT_REGION();

#যদি defined _WIN32 || defined WINCE
    BOOL status = TRUE;
    {
        WIN32_FILE_ATTRIBUTE_তথ্য all_attrs;
#ifdef WINRT
        wchar_t wpath[MAX_PATH];
        size_t copied = mbstowcs(wpath, path.c_str(), MAX_PATH);
        CV_Assert((copied != MAX_PATH) && (copied != (size_t)-1));
        status = ::GetFileAttributesExW(wpath, GetFileExInfoStandard, &all_attrs);
#অন্যথা
        status = ::GetFileAttributesExA(path.c_str(), GetFileExInfoStandard, &all_attrs);
#endif
    }

    ফেরত !!status;
#অন্যথা
    কাঠামো stat stat_buf;
    ফেরত (0 == stat(path.c_str(), &stat_buf));
#endif
}

CV_EXPORTS শূন্য remove_all(ধ্রুবক cv::String& path)
{
    যদি (!exists(path))
        ফেরত;
    যদি (isDirectory(path))
    {
        std::vector<String> entries;
        utils::fs::glob(path, cv::String(), entries, মিথ্যা, সত্য);
        জন্য (size_t i = 0; i < entries.size(); i++)
        {
            ধ্রুবক String& e = entries[i];
            remove_all(e);
        }
#ifdef _MSC_VER
        যৌক্তিক ফলাফল = _rmdir(path.c_str()) == 0;
#অন্যথা
        যৌক্তিক ফলাফল = rmdir(path.c_str()) == 0;
#endif
        যদি (!ফলাফল)
        {
            CV_LOG_ERROR(ফাঁকা, "Can't remove directory: " << path);
        }
    }
    অন্যথা
    {
#ifdef _MSC_VER
        যৌক্তিক ফলাফল = _unlink(path.c_str()) == 0;
#অন্যথা
        যৌক্তিক ফলাফল = unlink(path.c_str()) == 0;
#endif
        যদি (!ফলাফল)
        {
            CV_LOG_ERROR(ফাঁকা, "Can't remove file: " << path);
        }
    }
}


cv::String getcwd()
{
    CV_INSTRUMENT_REGION();
    cv::Auto_বাফার<অক্ষর, 4096> buf;
#যদি defined WIN32 || defined _WIN32 || defined WINCE
#ifdef WINRT
    ফেরত cv::String();
#অন্যথা
    DWORD sz = GetCurrentDirectoryA(0, ফাঁকা);
    buf.allocate((size_t)sz);
    sz = GetCurrentDirectoryA((DWORD)buf.size(), buf.তথ্য());
    ফেরত cv::String(buf.তথ্য(), (size_t)sz);
#endif
#নতুবা_যদি defined __linux__ || defined __APPLE__ || defined __HAIKU__ || defined __FreeBSD__ || defined __EMSCRIPTEN__ || defined __QNX__ || defined __CYGWIN__
    জন্য(;;)
    {
        অক্ষর* p = ::getcwd(buf.তথ্য(), buf.size());
        যদি (p == ফাঁকা)
        {
            যদি (errno == ERANGE)
            {
                buf.allocate(buf.size() * 2);
                চালু;
            }
            ফেরত cv::String();
        }
        বিরতি;
    }
    ফেরত cv::String(buf.তথ্য(), (size_t)strlen(buf.তথ্য()));
#অন্যথা
    ফেরত cv::String();
#endif
}


যৌক্তিক createDirectory(ধ্রুবক cv::String& path)
{
    CV_INSTRUMENT_REGION();
#যদি defined WIN32 || defined _WIN32 || defined WINCE
#ifdef WINRT
    wchar_t wpath[MAX_PATH];
    size_t copied = mbstowcs(wpath, path.c_str(), MAX_PATH);
    CV_Assert((copied != MAX_PATH) && (copied != (size_t)-1));
    পূর্ণসংখ্যা ফলাফল = CreateDirectoryA(wpath, ফাঁকা) ? 0 : -1;
#অন্যথা
    পূর্ণসংখ্যা ফলাফল = _mkdir(path.c_str());
#endif
#নতুবা_যদি defined __linux__ || defined __APPLE__ || defined __HAIKU__ || defined __FreeBSD__ || defined __EMSCRIPTEN__ || defined __QNX__ || defined __CYGWIN__
    পূর্ণসংখ্যা ফলাফল = mkdir(path.c_str(), 0777);
#অন্যথা
    পূর্ণসংখ্যা ফলাফল = -1;
#endif

    যদি (ফলাফল == -1)
    {
        ফেরত isDirectory(path);
    }
    ফেরত সত্য;
}

যৌক্তিক createDirectories(ধ্রুবক cv::String& path_)
{
    cv::String path = path_;
    জন্য (;;)
    {
        অক্ষর last_char = path.empty() ? 0 : path[path.length() - 1];
        যদি (isPathSeparator(last_char))
        {
            path = path.substr(0, path.length() - 1);
            চালু;
        }
        বিরতি;
    }

    যদি (path.empty() || path == "./" || path == ".\\" || path == ".")
        ফেরত সত্য;
    যদি (isDirectory(path))
        ফেরত সত্য;

    size_t pos = path.rfind('/');
    যদি (pos == cv::String::npos)
        pos = path.rfind('\\');
    যদি (pos != cv::String::npos)
    {
        cv::String parent_directory = path.substr(0, pos);
        যদি (!parent_directory.empty())
        {
            যদি (!createDirectories(parent_directory))
                ফেরত মিথ্যা;
        }
    }

    ফেরত createDirectory(path);
}

#ifdef _WIN32

কাঠামো FileLock::Impl
{
    Impl(ধ্রুবক অক্ষর* fname)
    {
        // http://support.microsoft.com/kb/316609
        পূর্ণসংখ্যা অঙ্ক_Retries = 5;
        করো
        {
            handle = ::CreateFileA(fname, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, ফাঁকা,
                                OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, ফাঁকা);
            যদি (INVALID_HANDLE_মান == handle)
            {
                যদি (ERROR_SHARING_VIOLATION == GetLastError())
                {
                    অঙ্ক_Retries--;
                    Sleep(250);
                    চালু;
                }
                অন্যথা
                {
                    CV_Error_(Error::StsAssert, ("Can't open lock file: %s", fname));
                }
            }
            বিরতি;
        } যখন (অঙ্ক_Retries > 0);
    }
    ~Impl()
    {
        যদি (INVALID_HANDLE_মান != handle)
        {
            ::CloseHandle(handle);
        }
    }

    যৌক্তিক lock()
    {
        OVERLAPPED overlapped;
        std::memset(&overlapped, 0, আকার(overlapped));
        ফেরত !!::LockFileEx(handle, LOCKFILE_EXCLUSIVE_LOCK, 0, MAXDWORD, MAXDWORD, &overlapped);
    }
    যৌক্তিক unlock()
    {
        OVERLAPPED overlapped;
        std::memset(&overlapped, 0, আকার(overlapped));
        ফেরত !!::UnlockFileEx(handle, 0, MAXDWORD, MAXDWORD, &overlapped);
    }

    যৌক্তিক lock_shared()
    {
        OVERLAPPED overlapped;
        std::memset(&overlapped, 0, আকার(overlapped));
        ফেরত !!::LockFileEx(handle, 0, 0, MAXDWORD, MAXDWORD, &overlapped);
    }
    যৌক্তিক unlock_shared()
    {
        ফেরত unlock();
    }

    HANDLE handle;

ব্যক্তিগত:
    Impl(ধ্রুবক Impl&); // disabled
    Impl& operator=(ধ্রুবক Impl&); // disabled
};

#নতুবা_যদি defined __linux__ || defined __APPLE__ || defined __HAIKU__ || defined __FreeBSD__ || defined __GNU__ || defined __EMSCRIPTEN__ || defined __QNX__ || defined __CYGWIN__

কাঠামো FileLock::Impl
{
    Impl(ধ্রুবক অক্ষর* fname)
    {
        handle = ::open(fname, O_RDWR);
        CV_Assert(handle != -1);
    }
    ~Impl()
    {
        যদি (handle >= 0)
            ::close(handle);
    }

    যৌক্তিক lock()
    {
        কাঠামো ::flock l;
        std::memset(&l, 0, আকার(l));
        l.l_type = F_WRLCK;
        l.l_whence = SEEK_SET;
        l.l_start = 0;
        l.l_len = 0;
        DBG(std::cout << "Lock..." << std::endl);
        যৌক্তিক res = -1 != ::fcntl(handle, F_SETLKW, &l);
        ফেরত res;
    }
    যৌক্তিক unlock()
    {
        কাঠামো ::flock l;
        std::memset(&l, 0, আকার(l));
        l.l_type = F_UNLCK;
        l.l_whence = SEEK_SET;
        l.l_start = 0;
        l.l_len = 0;
        DBG(std::cout << "Unlock..." << std::endl);
        যৌক্তিক res = -1 != ::fcntl(handle, F_SETLK, &l);
        ফেরত res;
    }

    যৌক্তিক lock_shared()
    {
        কাঠামো ::flock l;
        std::memset(&l, 0, আকার(l));
        l.l_type = F_RDLCK;
        l.l_whence = SEEK_SET;
        l.l_start = 0;
        l.l_len = 0;
        DBG(std::cout << "Lock read..." << std::endl);
        যৌক্তিক res = -1 != ::fcntl(handle, F_SETLKW, &l);
        ফেরত res;
    }
    যৌক্তিক unlock_shared()
    {
        ফেরত unlock();
    }

    পূর্ণসংখ্যা handle;

ব্যক্তিগত:
    Impl(ধ্রুবক Impl&); // disabled
    Impl& operator=(ধ্রুবক Impl&); // disabled
};

#endif

FileLock::FileLock(ধ্রুবক অক্ষর* fname)
    : pImpl(নতুন Impl(fname))
{
    // nothing
}
FileLock::~FileLock()
{
    delete pImpl;
    pImpl = ফাঁকা;
}

শূন্য FileLock::lock() { CV_Assert(pImpl->lock()); }
শূন্য FileLock::unlock() { CV_Assert(pImpl->unlock()); }
শূন্য FileLock::lock_shared() { CV_Assert(pImpl->lock_shared()); }
শূন্য FileLock::unlock_shared() { CV_Assert(pImpl->unlock_shared()); }



cv::String getCacheDirectory(ধ্রুবক অক্ষর* sub_directory_name, ধ্রুবক অক্ষর* configuration_name)
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
        DWORD res = Get_অস্থায়ী_Path_W(MAX_PATH, tmp_path_buf_w);
        যদি (res > 0 && res <= MAX_PATH)
        {
            // Convert from UTF-16 to UTF-8 to support Unicode paths
            পূর্ণসংখ্যা len = WideCharToMultiByte(CP_UTF8, 0, tmp_path_buf_w, -1, ফাঁকা, 0, ফাঁকা, ফাঁকা);
            যদি (len > 0)
            {
                std::vector<অক্ষর> utf8_buf(len);
                WideCharToMultiByte(CP_UTF8, 0, tmp_path_buf_w, -1, utf8_buf.তথ্য(), len, ফাঁকা, ফাঁকা);
                default_cache_path = std::string(utf8_buf.তথ্য());
            }
        }
#নতুবা_যদি defined __ANDROID__
        // no defaults
#নতুবা_যদি defined __APPLE__
        ধ্রুবক std::string tmpdir_env = utils::getConfigurationParameterString("TMPDIR");
        যদি (!tmpdir_env.empty() && utils::fs::isDirectory(tmpdir_env))
        {
            default_cache_path = tmpdir_env;
        }
        অন্যথা
        {
            default_cache_path = "/tmp/";
            CV_LOG_WARNING(ফাঁকা, "Using world accessible cache directory. This may be not secure: " << default_cache_path);
        }
#নতুবা_যদি defined __linux__ || defined __HAIKU__ || defined __FreeBSD__ || defined __GNU__
        // https://specifications.freedesktop.org/basedir-spec/basedir-spec-latest.html
        যদি (default_cache_path.empty())
        {
            ধ্রুবক std::string xdg_cache_env = utils::getConfigurationParameterString("XDG_CACHE_HOME");
            যদি (!xdg_cache_env.empty() && utils::fs::isDirectory(xdg_cache_env))
            {
                default_cache_path = xdg_cache_env;
            }
        }
        যদি (default_cache_path.empty())
        {
            ধ্রুবক std::string home_env = utils::getConfigurationParameterString("HOME");
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
            ধ্রুবক অক্ষর* অস্থায়ী_path = "/var/tmp/";
            যদি (utils::fs::isDirectory(অস্থায়ী_path))
            {
                default_cache_path = অস্থায়ী_path;
                CV_LOG_WARNING(ফাঁকা, "Using world accessible cache directory. This may be not secure: " << default_cache_path);
            }
        }
        যদি (default_cache_path.empty())
        {
            default_cache_path = "/tmp/";
            CV_LOG_WARNING(ফাঁকা, "Using world accessible cache directory. This may be not secure: " << default_cache_path);
        }
#অন্যথা
        // no defaults
#endif
        CV_LOG_VERBOSE(ফাঁকা, 0, "default_cache_path = " << default_cache_path);
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
                    চেষ্টা
                    {
                        utils::fs::glob_relative(default_cache_path_base, "*", existedCacheDirs, মিথ্যা, সত্য);
                    }
                    ধরো (...)
                    {
                        // ignore
                    }
                    যদি (!existedCacheDirs.empty())
                    {
                        CV_LOG_WARNING(ফাঁকা, "Creating new OpenCV cache directory: " << default_cache_path);
                        CV_LOG_WARNING(ফাঁকা, "There are several neighbour directories, probably created by old OpenCV versions.");
                        CV_LOG_WARNING(ফাঁকা, "Feel free to cleanup these unused directories:");
                        জন্য (size_t i = 0; i < existedCacheDirs.size(); i++)
                        {
                            CV_LOG_WARNING(ফাঁকা, "  - " << existedCacheDirs[i]);
                        }
                        CV_LOG_WARNING(ফাঁকা, "Note: This message is showed only once.");
                    }
                }
                যদি (sub_directory_name && sub_directory_name[0] != '\0')
                    default_cache_path = utils::fs::join(default_cache_path, cv::String(sub_directory_name) + native_separator);
                যদি (!utils::fs::createDirectories(default_cache_path))
                {
                    CV_LOG_DEBUG(ফাঁকা, "Can't create OpenCV cache sub-directory: " << default_cache_path);
                }
                অন্যথা
                {
                    cache_path = default_cache_path;
                }
            }
            অন্যথা
            {
                CV_LOG_INFO(ফাঁকা, "Can't find default cache directory (does it exist?): " << default_cache_path);
            }
        }
        অন্যথা
        {
            CV_LOG_DEBUG(ফাঁকা, "OpenCV has no support to discover default cache directory on the current platform");
        }
    }
    অন্যথা
    {
        যদি (cache_path == "disabled")
            ফেরত cache_path;
        যদি (!isDirectory(cache_path))
        {
            CV_LOG_WARNING(ফাঁকা, "Specified non-existed directory, creating OpenCV sub-directory for caching purposes: " << cache_path);
            যদি (!createDirectories(cache_path))
            {
                CV_LOG_ERROR(ফাঁকা, "Can't create OpenCV cache sub-directory: " << cache_path);
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
    ফেরত cache_path;
}

#অন্যথা
#define NOT_IMPLEMENTED CV_Error(Error::StsNotImplemented, "");
cv::String canonical(ধ্রুবক cv::String& /*path*/) { NOT_IMPLEMENTED }
যৌক্তিক exists(ধ্রুবক cv::String& /*path*/) { NOT_IMPLEMENTED }
শূন্য remove_all(ধ্রুবক cv::String& /*path*/) { NOT_IMPLEMENTED }
cv::String getcwd() { NOT_IMPLEMENTED }
যৌক্তিক createDirectory(ধ্রুবক cv::String& /*path*/) { NOT_IMPLEMENTED }
যৌক্তিক createDirectories(ধ্রুবক cv::String& /*path*/) { NOT_IMPLEMENTED }
cv::String getCacheDirectory(ধ্রুবক অক্ষর* /*sub_directory_name*/, ধ্রুবক অক্ষর* /*configuration_name = NULL*/) { NOT_IMPLEMENTED }
#undef NOT_IMPLEMENTED
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT

}}} // namespace


#যদি OPENCV_HAVE_FILESYSTEM_SUPPORT
#অন্তর্ভাব "plugin_loader.impl.hpp"
#endif
