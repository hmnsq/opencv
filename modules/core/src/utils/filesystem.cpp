// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ଅନ୍ତର୍ଭାଵ "../precomp.hpp"

#ଅନ୍ତର୍ଭାଵ <opencv2/core/utils/configuration.private.hpp>

#ଅନ୍ତର୍ଭାଵ <opencv2/core/utils/logger.hpp>

#ଅନ୍ତର୍ଭାଵ "opencv2/core/utils/filesystem.private.hpp"
#ଅନ୍ତର୍ଭାଵ "opencv2/core/utils/filesystem.hpp"

//#define DEBUG_FS_UTILS

#ifdef DEBUG_FS_UTILS
#ଅନ୍ତର୍ଭାଵ <iostream>
#define DBG(...) __VA_ARGS__
#ଅନ୍ୟଥା
#define DBG(...)
#endif


#ଯଦି OPENCV_HAVE_FILESYSTEM_SUPPORT

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#undef NOMINMAX
#define NOMINMAX
#ଅନ୍ତର୍ଭାଵ <windows.h>
#ଅନ୍ତର୍ଭାଵ <direct.h>
#ଅନ୍ତର୍ଭାଵ <sys/types.h>
#ଅନ୍ତର୍ଭାଵ <sys/stat.h>
#ଅନ୍ତର୍ଭାଵ <errno.h>
#ଅନ୍ତର୍ଭାଵ <io.h>
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ନଚେତଯଦି defined __linux__ || defined __APPLE__ || defined __HAIKU__ || defined __FreeBSD__ || defined __GNU__ || defined __EMSCRIPTEN__ || defined __QNX__ || defined __CYGWIN__
#ଅନ୍ତର୍ଭାଵ <sys/types.h>
#ଅନ୍ତର୍ଭାଵ <sys/stat.h>
#ଅନ୍ତର୍ଭାଵ <fcntl.h>
#ଅନ୍ତର୍ଭାଵ <unistd.h>
#ଅନ୍ତର୍ଭାଵ <errno.h>
#endif

#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT

ନାମାକାଶ cv { ନାମାକାଶ utils { ନାମାକାଶ fs {

#ifdef _WIN32
ସ୍ଥିର ଅଚଳ ଅକ୍ଷର native_separator = '\\';
#ଅନ୍ୟଥା
ସ୍ଥିର ଅଚଳ ଅକ୍ଷର native_separator = '/';
#endif

ସ୍ଥିର ଧାଡ଼ିଭିତରେ
ତାର୍କିକ isPathSeparator(ଅକ୍ଷର c)
{
    ଫେରନ୍ତୁ c == '/' || c == '\\';
}

cv::String join(ଅଚଳ cv::String& base, ଅଚଳ cv::String& path)
{
    ଯଦି (base.empty())
        ଫେରନ୍ତୁ path;
    ଯଦି (path.empty())
        ଫେରନ୍ତୁ base;

    ତାର୍କିକ baseSep = isPathSeparator(base[base.size() - 1]);
    ତାର୍କିକ pathSep = isPathSeparator(path[0]);
    String result;
    ଯଦି (baseSep && pathSep)
    {
        result = base + path.substr(1);
    }
    ଅନ୍ୟଥା ଯଦି (!baseSep && !pathSep)
    {
        result = base + native_separator + path;
    }
    ଅନ୍ୟଥା
    {
        result = base + path;
    }
    ଫେରନ୍ତୁ result;
}

CV_EXPORTS cv::String getParent(ଅଚଳ cv::String &path)
{
    std::string::size_type loc = path.find_last_of("/\\");
    ଯଦି (loc == std::string::npos)
        ଫେରନ୍ତୁ std::string();
    ଫେରନ୍ତୁ std::string(path, 0, loc);
}

CV_EXPORTS std::wstring getParent(ଅଚଳ std::wstring& path)
{
    std::wstring::size_type loc = path.find_last_of(L"/\\");
    ଯଦି (loc == std::wstring::npos)
        ଫେରନ୍ତୁ std::wstring();
    ଫେରନ୍ତୁ std::wstring(path, 0, loc);
}

#ଯଦି OPENCV_HAVE_FILESYSTEM_SUPPORT

cv::String canonical(ଅଚଳ cv::String& path)
{
    cv::String result;
#ifdef _WIN32
    ଅଚଳ ଅକ୍ଷର* result_str = _fullpath(ଖାଲି, path.c_str(), 0);
#ଅନ୍ୟଥା
    ଅଚଳ ଅକ୍ଷର* result_str = realpath(path.c_str(), ଖାଲି);
#endif
    ଯଦି (result_str)
    {
        result = cv::String(result_str);
        ମୁକ୍ତ((ଶୂନ୍ୟ*)result_str);
    }
    ଫେରନ୍ତୁ result.empty() ? path : result;
}


ତାର୍କିକ exists(ଅଚଳ cv::String& path)
{
    CV_INSTRUMENT_REGION();

#ଯଦି defined _WIN32 || defined WINCE
    BOOL status = TRUE;
    {
        WIN32_FILE_ATTRIBUTE_DATA all_attrs;
#ifdef WINRT
        wchar_t wpath[MAX_PATH];
        size_t copied = mbstowcs(wpath, path.c_str(), MAX_PATH);
        CV_Assert((copied != MAX_PATH) && (copied != (size_t)-1));
        status = ::GetFileAttributesExW(wpath, GetFileExInfoStandard, &all_attrs);
#ଅନ୍ୟଥା
        status = ::GetFileAttributesExA(path.c_str(), GetFileExInfoStandard, &all_attrs);
#endif
    }

    ଫେରନ୍ତୁ !!status;
#ଅନ୍ୟଥା
    ସଂରଚନା stat stat_buf;
    ଫେରନ୍ତୁ (0 == stat(path.c_str(), &stat_buf));
#endif
}

CV_EXPORTS ଶୂନ୍ୟ remove_all(ଅଚଳ cv::String& path)
{
    ଯଦି (!exists(path))
        ଫେରନ୍ତୁ;
    ଯଦି (isDirectory(path))
    {
        std::vector<String> entries;
        utils::fs::glob(path, cv::String(), entries, ମିଥ୍ୟା, ସତ୍ୟ);
        ପାଇଁ (size_t i = 0; i < entries.size(); i++)
        {
            ଅଚଳ String& e = entries[i];
            remove_all(e);
        }
#ifdef _MSC_VER
        ତାର୍କିକ result = _rmdir(path.c_str()) == 0;
#ଅନ୍ୟଥା
        ତାର୍କିକ result = rmdir(path.c_str()) == 0;
#endif
        ଯଦି (!result)
        {
            CV_LOG_ERROR(ଖାଲି, "Can't remove directory: " << path);
        }
    }
    ଅନ୍ୟଥା
    {
#ifdef _MSC_VER
        ତାର୍କିକ result = _unlink(path.c_str()) == 0;
#ଅନ୍ୟଥା
        ତାର୍କିକ result = unlink(path.c_str()) == 0;
#endif
        ଯଦି (!result)
        {
            CV_LOG_ERROR(ଖାଲି, "Can't remove file: " << path);
        }
    }
}


cv::String getcwd()
{
    CV_INSTRUMENT_REGION();
    cv::AutoBuffer<ଅକ୍ଷର, 4096> buf;
#ଯଦି defined WIN32 || defined _WIN32 || defined WINCE
#ifdef WINRT
    ଫେରନ୍ତୁ cv::String();
#ଅନ୍ୟଥା
    DWORD sz = GetCurrentDirectoryA(0, ଖାଲି);
    buf.allocate((size_t)sz);
    sz = GetCurrentDirectoryA((DWORD)buf.size(), buf.data());
    ଫେରନ୍ତୁ cv::String(buf.data(), (size_t)sz);
#endif
#ନଚେତଯଦି defined __linux__ || defined __APPLE__ || defined __HAIKU__ || defined __FreeBSD__ || defined __EMSCRIPTEN__ || defined __QNX__ || defined __CYGWIN__
    ପାଇଁ(;;)
    {
        ଅକ୍ଷର* p = ::getcwd(buf.data(), buf.size());
        ଯଦି (p == ଖାଲି)
        {
            ଯଦି (errno == ERANGE)
            {
                buf.allocate(buf.size() * 2);
                ଜାରି;
            }
            ଫେରନ୍ତୁ cv::String();
        }
        ଭାଙ୍ଗ;
    }
    ଫେରନ୍ତୁ cv::String(buf.data(), (size_t)strlen(buf.data()));
#ଅନ୍ୟଥା
    ଫେରନ୍ତୁ cv::String();
#endif
}


ତାର୍କିକ createDirectory(ଅଚଳ cv::String& path)
{
    CV_INSTRUMENT_REGION();
#ଯଦି defined WIN32 || defined _WIN32 || defined WINCE
#ifdef WINRT
    wchar_t wpath[MAX_PATH];
    size_t copied = mbstowcs(wpath, path.c_str(), MAX_PATH);
    CV_Assert((copied != MAX_PATH) && (copied != (size_t)-1));
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା result = CreateDirectoryA(wpath, ଖାଲି) ? 0 : -1;
#ଅନ୍ୟଥା
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା result = _mkdir(path.c_str());
#endif
#ନଚେତଯଦି defined __linux__ || defined __APPLE__ || defined __HAIKU__ || defined __FreeBSD__ || defined __EMSCRIPTEN__ || defined __QNX__ || defined __CYGWIN__
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା result = mkdir(path.c_str(), 0777);
#ଅନ୍ୟଥା
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା result = -1;
#endif

    ଯଦି (result == -1)
    {
        ଫେରନ୍ତୁ isDirectory(path);
    }
    ଫେରନ୍ତୁ ସତ୍ୟ;
}

ତାର୍କିକ createDirectories(ଅଚଳ cv::String& path_)
{
    cv::String path = path_;
    ପାଇଁ (;;)
    {
        ଅକ୍ଷର last_char = path.empty() ? 0 : path[path.length() - 1];
        ଯଦି (isPathSeparator(last_char))
        {
            path = path.substr(0, path.length() - 1);
            ଜାରି;
        }
        ଭାଙ୍ଗ;
    }

    ଯଦି (path.empty() || path == "./" || path == ".\\" || path == ".")
        ଫେରନ୍ତୁ ସତ୍ୟ;
    ଯଦି (isDirectory(path))
        ଫେରନ୍ତୁ ସତ୍ୟ;

    size_t pos = path.rfind('/');
    ଯଦି (pos == cv::String::npos)
        pos = path.rfind('\\');
    ଯଦି (pos != cv::String::npos)
    {
        cv::String parent_directory = path.substr(0, pos);
        ଯଦି (!parent_directory.empty())
        {
            ଯଦି (!createDirectories(parent_directory))
                ଫେରନ୍ତୁ ମିଥ୍ୟା;
        }
    }

    ଫେରନ୍ତୁ createDirectory(path);
}

#ifdef _WIN32

ସଂରଚନା FileLock::Impl
{
    Impl(ଅଚଳ ଅକ୍ଷର* fname)
    {
        // http://support.microsoft.com/kb/316609
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_Retries = 5;
        କର
        {
            handle = ::CreateFileA(fname, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, ଖାଲି,
                                OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, ଖାଲି);
            ଯଦି (INVALID_HANDLE_ମୂଲ୍ୟ == handle)
            {
                ଯଦି (ERROR_SHARING_VIOLATION == GetLastError())
                {
                    ଅଙ୍କ_Retries--;
                    Sleep(250);
                    ଜାରି;
                }
                ଅନ୍ୟଥା
                {
                    CV_Error_(Error::StsAssert, ("Can't open lock file: %s", fname));
                }
            }
            ଭାଙ୍ଗ;
        } ଯେତେବେଳେ (ଅଙ୍କ_Retries > 0);
    }
    ~Impl()
    {
        ଯଦି (INVALID_HANDLE_ମୂଲ୍ୟ != handle)
        {
            ::CloseHandle(handle);
        }
    }

    ତାର୍କିକ lock()
    {
        OVERLAPPED overlapped;
        std::memset(&overlapped, 0, ଆକାର(overlapped));
        ଫେରନ୍ତୁ !!::LockFileEx(handle, LOCKFILE_EXCLUSIVE_LOCK, 0, MAXDWORD, MAXDWORD, &overlapped);
    }
    ତାର୍କିକ unlock()
    {
        OVERLAPPED overlapped;
        std::memset(&overlapped, 0, ଆକାର(overlapped));
        ଫେରନ୍ତୁ !!::UnlockFileEx(handle, 0, MAXDWORD, MAXDWORD, &overlapped);
    }

    ତାର୍କିକ lock_shared()
    {
        OVERLAPPED overlapped;
        std::memset(&overlapped, 0, ଆକାର(overlapped));
        ଫେରନ୍ତୁ !!::LockFileEx(handle, 0, 0, MAXDWORD, MAXDWORD, &overlapped);
    }
    ତାର୍କିକ unlock_shared()
    {
        ଫେରନ୍ତୁ unlock();
    }

    HANDLE handle;

ବ୍ୟକ୍ତିଗତ:
    Impl(ଅଚଳ Impl&); // disabled
    Impl& operator=(ଅଚଳ Impl&); // disabled
};

#ନଚେତଯଦି defined __linux__ || defined __APPLE__ || defined __HAIKU__ || defined __FreeBSD__ || defined __GNU__ || defined __EMSCRIPTEN__ || defined __QNX__ || defined __CYGWIN__

ସଂରଚନା FileLock::Impl
{
    Impl(ଅଚଳ ଅକ୍ଷର* fname)
    {
        handle = ::open(fname, O_RDWR);
        CV_Assert(handle != -1);
    }
    ~Impl()
    {
        ଯଦି (handle >= 0)
            ::close(handle);
    }

    ତାର୍କିକ lock()
    {
        ସଂରଚନା ::flock l;
        std::memset(&l, 0, ଆକାର(l));
        l.l_type = F_WRLCK;
        l.l_whence = SEEK_SET;
        l.l_start = 0;
        l.l_len = 0;
        DBG(std::cout << "Lock..." << std::endl);
        ତାର୍କିକ res = -1 != ::fcntl(handle, F_SETLKW, &l);
        ଫେରନ୍ତୁ res;
    }
    ତାର୍କିକ unlock()
    {
        ସଂରଚନା ::flock l;
        std::memset(&l, 0, ଆକାର(l));
        l.l_type = F_UNLCK;
        l.l_whence = SEEK_SET;
        l.l_start = 0;
        l.l_len = 0;
        DBG(std::cout << "Unlock..." << std::endl);
        ତାର୍କିକ res = -1 != ::fcntl(handle, F_SETLK, &l);
        ଫେରନ୍ତୁ res;
    }

    ତାର୍କିକ lock_shared()
    {
        ସଂରଚନା ::flock l;
        std::memset(&l, 0, ଆକାର(l));
        l.l_type = F_RDLCK;
        l.l_whence = SEEK_SET;
        l.l_start = 0;
        l.l_len = 0;
        DBG(std::cout << "Lock read..." << std::endl);
        ତାର୍କିକ res = -1 != ::fcntl(handle, F_SETLKW, &l);
        ଫେରନ୍ତୁ res;
    }
    ତାର୍କିକ unlock_shared()
    {
        ଫେରନ୍ତୁ unlock();
    }

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା handle;

ବ୍ୟକ୍ତିଗତ:
    Impl(ଅଚଳ Impl&); // disabled
    Impl& operator=(ଅଚଳ Impl&); // disabled
};

#endif

FileLock::FileLock(ଅଚଳ ଅକ୍ଷର* fname)
    : pImpl(ନୂତନ Impl(fname))
{
    // nothing
}
FileLock::~FileLock()
{
    delete pImpl;
    pImpl = ଖାଲି;
}

ଶୂନ୍ୟ FileLock::lock() { CV_Assert(pImpl->lock()); }
ଶୂନ୍ୟ FileLock::unlock() { CV_Assert(pImpl->unlock()); }
ଶୂନ୍ୟ FileLock::lock_shared() { CV_Assert(pImpl->lock_shared()); }
ଶୂନ୍ୟ FileLock::unlock_shared() { CV_Assert(pImpl->unlock_shared()); }



cv::String getCacheDirectory(ଅଚଳ ଅକ୍ଷର* sub_directory_name, ଅଚଳ ଅକ୍ଷର* configuration_name)
{
    String cache_path;
    ଯଦି (configuration_name)
    {
        cache_path = utils::getConfigurationParameterString(configuration_name, "");
    }
    ଯଦି (cache_path.empty())
    {
        cv::String default_cache_path;
#ifdef _WIN32
        wchar_t tmp_path_buf_w[MAX_PATH+1] = {0};
        DWORD res = GetTempPathW(MAX_PATH, tmp_path_buf_w);
        ଯଦି (res > 0 && res <= MAX_PATH)
        {
            // Convert from UTF-16 to UTF-8 to support Unicode paths
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା len = WideCharToMultiByte(CP_UTF8, 0, tmp_path_buf_w, -1, ଖାଲି, 0, ଖାଲି, ଖାଲି);
            ଯଦି (len > 0)
            {
                std::vector<ଅକ୍ଷର> utf8_buf(len);
                WideCharToMultiByte(CP_UTF8, 0, tmp_path_buf_w, -1, utf8_buf.data(), len, ଖାଲି, ଖାଲି);
                default_cache_path = std::string(utf8_buf.data());
            }
        }
#ନଚେତଯଦି defined __ANDROID__
        // no defaults
#ନଚେତଯଦି defined __APPLE__
        ଅଚଳ std::string tmpdir_env = utils::getConfigurationParameterString("TMPDIR");
        ଯଦି (!tmpdir_env.empty() && utils::fs::isDirectory(tmpdir_env))
        {
            default_cache_path = tmpdir_env;
        }
        ଅନ୍ୟଥା
        {
            default_cache_path = "/tmp/";
            CV_LOG_WARNING(ଖାଲି, "Using world accessible cache directory. This may be not secure: " << default_cache_path);
        }
#ନଚେତଯଦି defined __linux__ || defined __HAIKU__ || defined __FreeBSD__ || defined __GNU__
        // https://specifications.freedesktop.org/basedir-spec/basedir-spec-latest.html
        ଯଦି (default_cache_path.empty())
        {
            ଅଚଳ std::string xdg_cache_env = utils::getConfigurationParameterString("XDG_CACHE_HOME");
            ଯଦି (!xdg_cache_env.empty() && utils::fs::isDirectory(xdg_cache_env))
            {
                default_cache_path = xdg_cache_env;
            }
        }
        ଯଦି (default_cache_path.empty())
        {
            ଅଚଳ std::string home_env = utils::getConfigurationParameterString("HOME");
            ଯଦି (!home_env.empty() && utils::fs::isDirectory(home_env))
            {
                cv::String home_path = home_env;
                cv::String home_cache_path = utils::fs::join(home_path, ".cache/");
                ଯଦି (utils::fs::isDirectory(home_cache_path))
                {
                    default_cache_path = home_cache_path;
                }
            }
        }
        ଯଦି (default_cache_path.empty())
        {
            ଅଚଳ ଅକ୍ଷର* temp_path = "/var/tmp/";
            ଯଦି (utils::fs::isDirectory(temp_path))
            {
                default_cache_path = temp_path;
                CV_LOG_WARNING(ଖାଲି, "Using world accessible cache directory. This may be not secure: " << default_cache_path);
            }
        }
        ଯଦି (default_cache_path.empty())
        {
            default_cache_path = "/tmp/";
            CV_LOG_WARNING(ଖାଲି, "Using world accessible cache directory. This may be not secure: " << default_cache_path);
        }
#ଅନ୍ୟଥା
        // no defaults
#endif
        CV_LOG_VERBOSE(ଖାଲି, 0, "default_cache_path = " << default_cache_path);
        ଯଦି (!default_cache_path.empty())
        {
            ଯଦି (utils::fs::isDirectory(default_cache_path))
            {
                cv::String default_cache_path_base = utils::fs::join(default_cache_path, "opencv");
                default_cache_path = utils::fs::join(default_cache_path_base, CVAUX_STR(CV_VERSION_MAJOR) ".x" CV_VERSION_STATUS);
                ଯଦି (utils::getConfigurationParameterBool("OPENCV_CACHE_SHOW_CLEANUP_MESSAGE", ସତ୍ୟ)
                    && !utils::fs::isDirectory(default_cache_path))
                {
                    std::vector<cv::String> existedCacheDirs;
                    ଚେଷ୍ଟା
                    {
                        utils::fs::glob_relative(default_cache_path_base, "*", existedCacheDirs, ମିଥ୍ୟା, ସତ୍ୟ);
                    }
                    ଧର (...)
                    {
                        // ignore
                    }
                    ଯଦି (!existedCacheDirs.empty())
                    {
                        CV_LOG_WARNING(ଖାଲି, "Creating new OpenCV cache directory: " << default_cache_path);
                        CV_LOG_WARNING(ଖାଲି, "There are several neighbour directories, probably created by old OpenCV versions.");
                        CV_LOG_WARNING(ଖାଲି, "Feel free to cleanup these unused directories:");
                        ପାଇଁ (size_t i = 0; i < existedCacheDirs.size(); i++)
                        {
                            CV_LOG_WARNING(ଖାଲି, "  - " << existedCacheDirs[i]);
                        }
                        CV_LOG_WARNING(ଖାଲି, "Note: This message is showed only once.");
                    }
                }
                ଯଦି (sub_directory_name && sub_directory_name[0] != '\0')
                    default_cache_path = utils::fs::join(default_cache_path, cv::String(sub_directory_name) + native_separator);
                ଯଦି (!utils::fs::createDirectories(default_cache_path))
                {
                    CV_LOG_DEBUG(ଖାଲି, "Can't create OpenCV cache sub-directory: " << default_cache_path);
                }
                ଅନ୍ୟଥା
                {
                    cache_path = default_cache_path;
                }
            }
            ଅନ୍ୟଥା
            {
                CV_LOG_INFO(ଖାଲି, "Can't find default cache directory (does it exist?): " << default_cache_path);
            }
        }
        ଅନ୍ୟଥା
        {
            CV_LOG_DEBUG(ଖାଲି, "OpenCV has no support to discover default cache directory on the current platform");
        }
    }
    ଅନ୍ୟଥା
    {
        ଯଦି (cache_path == "disabled")
            ଫେରନ୍ତୁ cache_path;
        ଯଦି (!isDirectory(cache_path))
        {
            CV_LOG_WARNING(ଖାଲି, "Specified non-existed directory, creating OpenCV sub-directory for caching purposes: " << cache_path);
            ଯଦି (!createDirectories(cache_path))
            {
                CV_LOG_ERROR(ଖାଲି, "Can't create OpenCV cache sub-directory: " << cache_path);
                cache_path.clear();
            }
        }
    }
    CV_Assert(cache_path.empty() || utils::fs::isDirectory(cache_path));
    ଯଦି (!cache_path.empty())
    {
        ଯଦି (!isPathSeparator(cache_path[cache_path.size() - 1]))
        {
            cache_path += native_separator;
        }
    }
    ଫେରନ୍ତୁ cache_path;
}

#ଅନ୍ୟଥା
#define NOT_IMPLEMENTED CV_Error(Error::StsNotImplemented, "");
cv::String canonical(ଅଚଳ cv::String& /*path*/) { NOT_IMPLEMENTED }
ତାର୍କିକ exists(ଅଚଳ cv::String& /*path*/) { NOT_IMPLEMENTED }
ଶୂନ୍ୟ remove_all(ଅଚଳ cv::String& /*path*/) { NOT_IMPLEMENTED }
cv::String getcwd() { NOT_IMPLEMENTED }
ତାର୍କିକ createDirectory(ଅଚଳ cv::String& /*path*/) { NOT_IMPLEMENTED }
ତାର୍କିକ createDirectories(ଅଚଳ cv::String& /*path*/) { NOT_IMPLEMENTED }
cv::String getCacheDirectory(ଅଚଳ ଅକ୍ଷର* /*sub_directory_name*/, ଅଚଳ ଅକ୍ଷର* /*configuration_name = NULL*/) { NOT_IMPLEMENTED }
#undef NOT_IMPLEMENTED
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT

}}} // namespace


#ଯଦି OPENCV_HAVE_FILESYSTEM_SUPPORT
#ଅନ୍ତର୍ଭାଵ "plugin_loader.impl.hpp"
#endif
