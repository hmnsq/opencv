// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#அந்தர்பாவ "../precomp.hpp"

#அந்தர்பாவ <opencv2/core/utils/configuration.private.hpp>

#அந்தர்பாவ <opencv2/core/utils/logger.hpp>

#அந்தர்பாவ "opencv2/core/utils/filesystem.private.hpp"
#அந்தர்பாவ "opencv2/core/utils/filesystem.hpp"

//#define DEBUG_FS_UTILS

#ifdef DEBUG_FS_UTILS
#அந்தர்பாவ <iostream>
#define DBG(...) __VA_ARGS__
#இல்லையெனில்
#define DBG(...)
#endif


#எனில் OPENCV_HAVE_FILESYSTEM_SUPPORT

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#undef NOMINMAX
#define NOMINMAX
#அந்தர்பாவ <windows.h>
#அந்தர்பாவ <direct.h>
#அந்தர்பாவ <sys/types.h>
#அந்தர்பாவ <sys/stat.h>
#அந்தர்பாவ <errno.h>
#அந்தர்பாவ <io.h>
#அந்தர்பாவ <stdio.h>
#அல்லதெனில் defined __linux__ || defined __APPLE__ || defined __HAIKU__ || defined __FreeBSD__ || defined __GNU__ || defined __EMSCRIPTEN__ || defined __QNX__ || defined __CYGWIN__
#அந்தர்பாவ <sys/types.h>
#அந்தர்பாவ <sys/stat.h>
#அந்தர்பாவ <fcntl.h>
#அந்தர்பாவ <unistd.h>
#அந்தர்பாவ <errno.h>
#endif

#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT

நாமாகாஶ cv { நாமாகாஶ utils { நாமாகாஶ fs {

#ifdef _WIN32
நிலையான மாறிலி எழுத்து native_separator = '\\';
#இல்லையெனில்
நிலையான மாறிலி எழுத்து native_separator = '/';
#endif

நிலையான உள்வரி
தருக்க isPathSeparator(எழுத்து c)
{
    திரும்பு c == '/' || c == '\\';
}

cv::String join(மாறிலி cv::String& base, மாறிலி cv::String& path)
{
    எனில் (base.empty())
        திரும்பு path;
    எனில் (path.empty())
        திரும்பு base;

    தருக்க baseSep = isPathSeparator(base[base.size() - 1]);
    தருக்க pathSep = isPathSeparator(path[0]);
    String result;
    எனில் (baseSep && pathSep)
    {
        result = base + path.substr(1);
    }
    இல்லையெனில் எனில் (!baseSep && !pathSep)
    {
        result = base + native_separator + path;
    }
    இல்லையெனில்
    {
        result = base + path;
    }
    திரும்பு result;
}

CV_EXPORTS cv::String getParent(மாறிலி cv::String &path)
{
    std::string::size_type loc = path.find_last_of("/\\");
    எனில் (loc == std::string::npos)
        திரும்பு std::string();
    திரும்பு std::string(path, 0, loc);
}

CV_EXPORTS std::wstring getParent(மாறிலி std::wstring& path)
{
    std::wstring::size_type loc = path.find_last_of(L"/\\");
    எனில் (loc == std::wstring::npos)
        திரும்பு std::wstring();
    திரும்பு std::wstring(path, 0, loc);
}

#எனில் OPENCV_HAVE_FILESYSTEM_SUPPORT

cv::String canonical(மாறிலி cv::String& path)
{
    cv::String result;
#ifdef _WIN32
    மாறிலி எழுத்து* result_str = _fullpath(வெற்று, path.c_str(), 0);
#இல்லையெனில்
    மாறிலி எழுத்து* result_str = realpath(path.c_str(), வெற்று);
#endif
    எனில் (result_str)
    {
        result = cv::String(result_str);
        விடுவி((வெறுமை*)result_str);
    }
    திரும்பு result.empty() ? path : result;
}


தருக்க exists(மாறிலி cv::String& path)
{
    CV_INSTRUMENT_REGION();

#எனில் defined _WIN32 || defined WINCE
    BOOL status = TRUE;
    {
        WIN32_FILE_ATTRIBUTE_DATA all_attrs;
#ifdef WINRT
        wchar_t wpath[MAX_PATH];
        size_t copied = mbstowcs(wpath, path.c_str(), MAX_PATH);
        CV_Assert((copied != MAX_PATH) && (copied != (size_t)-1));
        status = ::GetFileAttributesExW(wpath, GetFileExInfoStandard, &all_attrs);
#இல்லையெனில்
        status = ::GetFileAttributesExA(path.c_str(), GetFileExInfoStandard, &all_attrs);
#endif
    }

    திரும்பு !!status;
#இல்லையெனில்
    கட்டமைப்பு stat stat_buf;
    திரும்பு (0 == stat(path.c_str(), &stat_buf));
#endif
}

CV_EXPORTS வெறுமை remove_all(மாறிலி cv::String& path)
{
    எனில் (!exists(path))
        திரும்பு;
    எனில் (isDirectory(path))
    {
        std::vector<String> entries;
        utils::fs::glob(path, cv::String(), entries, பொய், உண்மை);
        க்கு (size_t i = 0; i < entries.size(); i++)
        {
            மாறிலி String& e = entries[i];
            remove_all(e);
        }
#ifdef _MSC_VER
        தருக்க result = _rmdir(path.c_str()) == 0;
#இல்லையெனில்
        தருக்க result = rmdir(path.c_str()) == 0;
#endif
        எனில் (!result)
        {
            CV_LOG_ERROR(வெற்று, "Can't remove directory: " << path);
        }
    }
    இல்லையெனில்
    {
#ifdef _MSC_VER
        தருக்க result = _unlink(path.c_str()) == 0;
#இல்லையெனில்
        தருக்க result = unlink(path.c_str()) == 0;
#endif
        எனில் (!result)
        {
            CV_LOG_ERROR(வெற்று, "Can't remove file: " << path);
        }
    }
}


cv::String getcwd()
{
    CV_INSTRUMENT_REGION();
    cv::AutoBuffer<எழுத்து, 4096> buf;
#எனில் defined WIN32 || defined _WIN32 || defined WINCE
#ifdef WINRT
    திரும்பு cv::String();
#இல்லையெனில்
    DWORD sz = GetCurrentDirectoryA(0, வெற்று);
    buf.allocate((size_t)sz);
    sz = GetCurrentDirectoryA((DWORD)buf.size(), buf.data());
    திரும்பு cv::String(buf.data(), (size_t)sz);
#endif
#அல்லதெனில் defined __linux__ || defined __APPLE__ || defined __HAIKU__ || defined __FreeBSD__ || defined __EMSCRIPTEN__ || defined __QNX__ || defined __CYGWIN__
    க்கு(;;)
    {
        எழுத்து* p = ::getcwd(buf.data(), buf.size());
        எனில் (p == வெற்று)
        {
            எனில் (errno == ERANGE)
            {
                buf.allocate(buf.size() * 2);
                தொடர்;
            }
            திரும்பு cv::String();
        }
        நிறுத்து;
    }
    திரும்பு cv::String(buf.data(), (size_t)strlen(buf.data()));
#இல்லையெனில்
    திரும்பு cv::String();
#endif
}


தருக்க createDirectory(மாறிலி cv::String& path)
{
    CV_INSTRUMENT_REGION();
#எனில் defined WIN32 || defined _WIN32 || defined WINCE
#ifdef WINRT
    wchar_t wpath[MAX_PATH];
    size_t copied = mbstowcs(wpath, path.c_str(), MAX_PATH);
    CV_Assert((copied != MAX_PATH) && (copied != (size_t)-1));
    முழு result = CreateDirectoryA(wpath, வெற்று) ? 0 : -1;
#இல்லையெனில்
    முழு result = _mkdir(path.c_str());
#endif
#அல்லதெனில் defined __linux__ || defined __APPLE__ || defined __HAIKU__ || defined __FreeBSD__ || defined __EMSCRIPTEN__ || defined __QNX__ || defined __CYGWIN__
    முழு result = mkdir(path.c_str(), 0777);
#இல்லையெனில்
    முழு result = -1;
#endif

    எனில் (result == -1)
    {
        திரும்பு isDirectory(path);
    }
    திரும்பு உண்மை;
}

தருக்க createDirectories(மாறிலி cv::String& path_)
{
    cv::String path = path_;
    க்கு (;;)
    {
        எழுத்து last_char = path.empty() ? 0 : path[path.length() - 1];
        எனில் (isPathSeparator(last_char))
        {
            path = path.substr(0, path.length() - 1);
            தொடர்;
        }
        நிறுத்து;
    }

    எனில் (path.empty() || path == "./" || path == ".\\" || path == ".")
        திரும்பு உண்மை;
    எனில் (isDirectory(path))
        திரும்பு உண்மை;

    size_t pos = path.rfind('/');
    எனில் (pos == cv::String::npos)
        pos = path.rfind('\\');
    எனில் (pos != cv::String::npos)
    {
        cv::String parent_directory = path.substr(0, pos);
        எனில் (!parent_directory.empty())
        {
            எனில் (!createDirectories(parent_directory))
                திரும்பு பொய்;
        }
    }

    திரும்பு createDirectory(path);
}

#ifdef _WIN32

கட்டமைப்பு FileLock::Impl
{
    Impl(மாறிலி எழுத்து* fname)
    {
        // http://support.microsoft.com/kb/316609
        முழு இலக்கம்_Retries = 5;
        செய்
        {
            handle = ::CreateFileA(fname, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, வெற்று,
                                OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, வெற்று);
            எனில் (INVALID_HANDLE_மதிப்பு == handle)
            {
                எனில் (ERROR_SHARING_VIOLATION == GetLastError())
                {
                    இலக்கம்_Retries--;
                    Sleep(250);
                    தொடர்;
                }
                இல்லையெனில்
                {
                    CV_Error_(Error::StsAssert, ("Can't open lock file: %s", fname));
                }
            }
            நிறுத்து;
        } போது (இலக்கம்_Retries > 0);
    }
    ~Impl()
    {
        எனில் (INVALID_HANDLE_மதிப்பு != handle)
        {
            ::CloseHandle(handle);
        }
    }

    தருக்க lock()
    {
        OVERLAPPED overlapped;
        std::memset(&overlapped, 0, அளவு(overlapped));
        திரும்பு !!::LockFileEx(handle, LOCKFILE_EXCLUSIVE_LOCK, 0, MAXDWORD, MAXDWORD, &overlapped);
    }
    தருக்க unlock()
    {
        OVERLAPPED overlapped;
        std::memset(&overlapped, 0, அளவு(overlapped));
        திரும்பு !!::UnlockFileEx(handle, 0, MAXDWORD, MAXDWORD, &overlapped);
    }

    தருக்க lock_shared()
    {
        OVERLAPPED overlapped;
        std::memset(&overlapped, 0, அளவு(overlapped));
        திரும்பு !!::LockFileEx(handle, 0, 0, MAXDWORD, MAXDWORD, &overlapped);
    }
    தருக்க unlock_shared()
    {
        திரும்பு unlock();
    }

    HANDLE handle;

தனிப்பட்ட:
    Impl(மாறிலி Impl&); // disabled
    Impl& operator=(மாறிலி Impl&); // disabled
};

#அல்லதெனில் defined __linux__ || defined __APPLE__ || defined __HAIKU__ || defined __FreeBSD__ || defined __GNU__ || defined __EMSCRIPTEN__ || defined __QNX__ || defined __CYGWIN__

கட்டமைப்பு FileLock::Impl
{
    Impl(மாறிலி எழுத்து* fname)
    {
        handle = ::open(fname, O_RDWR);
        CV_Assert(handle != -1);
    }
    ~Impl()
    {
        எனில் (handle >= 0)
            ::close(handle);
    }

    தருக்க lock()
    {
        கட்டமைப்பு ::flock l;
        std::memset(&l, 0, அளவு(l));
        l.l_type = F_WRLCK;
        l.l_whence = SEEK_SET;
        l.l_start = 0;
        l.l_len = 0;
        DBG(std::cout << "Lock..." << std::endl);
        தருக்க res = -1 != ::fcntl(handle, F_SETLKW, &l);
        திரும்பு res;
    }
    தருக்க unlock()
    {
        கட்டமைப்பு ::flock l;
        std::memset(&l, 0, அளவு(l));
        l.l_type = F_UNLCK;
        l.l_whence = SEEK_SET;
        l.l_start = 0;
        l.l_len = 0;
        DBG(std::cout << "Unlock..." << std::endl);
        தருக்க res = -1 != ::fcntl(handle, F_SETLK, &l);
        திரும்பு res;
    }

    தருக்க lock_shared()
    {
        கட்டமைப்பு ::flock l;
        std::memset(&l, 0, அளவு(l));
        l.l_type = F_RDLCK;
        l.l_whence = SEEK_SET;
        l.l_start = 0;
        l.l_len = 0;
        DBG(std::cout << "Lock read..." << std::endl);
        தருக்க res = -1 != ::fcntl(handle, F_SETLKW, &l);
        திரும்பு res;
    }
    தருக்க unlock_shared()
    {
        திரும்பு unlock();
    }

    முழு handle;

தனிப்பட்ட:
    Impl(மாறிலி Impl&); // disabled
    Impl& operator=(மாறிலி Impl&); // disabled
};

#endif

FileLock::FileLock(மாறிலி எழுத்து* fname)
    : pImpl(புதிய Impl(fname))
{
    // nothing
}
FileLock::~FileLock()
{
    delete pImpl;
    pImpl = வெற்று;
}

வெறுமை FileLock::lock() { CV_Assert(pImpl->lock()); }
வெறுமை FileLock::unlock() { CV_Assert(pImpl->unlock()); }
வெறுமை FileLock::lock_shared() { CV_Assert(pImpl->lock_shared()); }
வெறுமை FileLock::unlock_shared() { CV_Assert(pImpl->unlock_shared()); }



cv::String getCacheDirectory(மாறிலி எழுத்து* sub_directory_name, மாறிலி எழுத்து* configuration_name)
{
    String cache_path;
    எனில் (configuration_name)
    {
        cache_path = utils::getConfigurationParameterString(configuration_name, "");
    }
    எனில் (cache_path.empty())
    {
        cv::String default_cache_path;
#ifdef _WIN32
        wchar_t tmp_path_buf_w[MAX_PATH+1] = {0};
        DWORD res = GetTempPathW(MAX_PATH, tmp_path_buf_w);
        எனில் (res > 0 && res <= MAX_PATH)
        {
            // Convert from UTF-16 to UTF-8 to support Unicode paths
            முழு len = WideCharToMultiByte(CP_UTF8, 0, tmp_path_buf_w, -1, வெற்று, 0, வெற்று, வெற்று);
            எனில் (len > 0)
            {
                std::vector<எழுத்து> utf8_buf(len);
                WideCharToMultiByte(CP_UTF8, 0, tmp_path_buf_w, -1, utf8_buf.data(), len, வெற்று, வெற்று);
                default_cache_path = std::string(utf8_buf.data());
            }
        }
#அல்லதெனில் defined __ANDROID__
        // no defaults
#அல்லதெனில் defined __APPLE__
        மாறிலி std::string tmpdir_env = utils::getConfigurationParameterString("TMPDIR");
        எனில் (!tmpdir_env.empty() && utils::fs::isDirectory(tmpdir_env))
        {
            default_cache_path = tmpdir_env;
        }
        இல்லையெனில்
        {
            default_cache_path = "/tmp/";
            CV_LOG_WARNING(வெற்று, "Using world accessible cache directory. This may be not secure: " << default_cache_path);
        }
#அல்லதெனில் defined __linux__ || defined __HAIKU__ || defined __FreeBSD__ || defined __GNU__
        // https://specifications.freedesktop.org/basedir-spec/basedir-spec-latest.html
        எனில் (default_cache_path.empty())
        {
            மாறிலி std::string xdg_cache_env = utils::getConfigurationParameterString("XDG_CACHE_HOME");
            எனில் (!xdg_cache_env.empty() && utils::fs::isDirectory(xdg_cache_env))
            {
                default_cache_path = xdg_cache_env;
            }
        }
        எனில் (default_cache_path.empty())
        {
            மாறிலி std::string home_env = utils::getConfigurationParameterString("HOME");
            எனில் (!home_env.empty() && utils::fs::isDirectory(home_env))
            {
                cv::String home_path = home_env;
                cv::String home_cache_path = utils::fs::join(home_path, ".cache/");
                எனில் (utils::fs::isDirectory(home_cache_path))
                {
                    default_cache_path = home_cache_path;
                }
            }
        }
        எனில் (default_cache_path.empty())
        {
            மாறிலி எழுத்து* temp_path = "/var/tmp/";
            எனில் (utils::fs::isDirectory(temp_path))
            {
                default_cache_path = temp_path;
                CV_LOG_WARNING(வெற்று, "Using world accessible cache directory. This may be not secure: " << default_cache_path);
            }
        }
        எனில் (default_cache_path.empty())
        {
            default_cache_path = "/tmp/";
            CV_LOG_WARNING(வெற்று, "Using world accessible cache directory. This may be not secure: " << default_cache_path);
        }
#இல்லையெனில்
        // no defaults
#endif
        CV_LOG_VERBOSE(வெற்று, 0, "default_cache_path = " << default_cache_path);
        எனில் (!default_cache_path.empty())
        {
            எனில் (utils::fs::isDirectory(default_cache_path))
            {
                cv::String default_cache_path_base = utils::fs::join(default_cache_path, "opencv");
                default_cache_path = utils::fs::join(default_cache_path_base, CVAUX_STR(CV_VERSION_MAJOR) ".x" CV_VERSION_STATUS);
                எனில் (utils::getConfigurationParameterBool("OPENCV_CACHE_SHOW_CLEANUP_MESSAGE", உண்மை)
                    && !utils::fs::isDirectory(default_cache_path))
                {
                    std::vector<cv::String> existedCacheDirs;
                    முயல்
                    {
                        utils::fs::glob_relative(default_cache_path_base, "*", existedCacheDirs, பொய், உண்மை);
                    }
                    பிடி (...)
                    {
                        // ignore
                    }
                    எனில் (!existedCacheDirs.empty())
                    {
                        CV_LOG_WARNING(வெற்று, "Creating new OpenCV cache directory: " << default_cache_path);
                        CV_LOG_WARNING(வெற்று, "There are several neighbour directories, probably created by old OpenCV versions.");
                        CV_LOG_WARNING(வெற்று, "Feel free to cleanup these unused directories:");
                        க்கு (size_t i = 0; i < existedCacheDirs.size(); i++)
                        {
                            CV_LOG_WARNING(வெற்று, "  - " << existedCacheDirs[i]);
                        }
                        CV_LOG_WARNING(வெற்று, "Note: This message is showed only once.");
                    }
                }
                எனில் (sub_directory_name && sub_directory_name[0] != '\0')
                    default_cache_path = utils::fs::join(default_cache_path, cv::String(sub_directory_name) + native_separator);
                எனில் (!utils::fs::createDirectories(default_cache_path))
                {
                    CV_LOG_DEBUG(வெற்று, "Can't create OpenCV cache sub-directory: " << default_cache_path);
                }
                இல்லையெனில்
                {
                    cache_path = default_cache_path;
                }
            }
            இல்லையெனில்
            {
                CV_LOG_INFO(வெற்று, "Can't find default cache directory (does it exist?): " << default_cache_path);
            }
        }
        இல்லையெனில்
        {
            CV_LOG_DEBUG(வெற்று, "OpenCV has no support to discover default cache directory on the current platform");
        }
    }
    இல்லையெனில்
    {
        எனில் (cache_path == "disabled")
            திரும்பு cache_path;
        எனில் (!isDirectory(cache_path))
        {
            CV_LOG_WARNING(வெற்று, "Specified non-existed directory, creating OpenCV sub-directory for caching purposes: " << cache_path);
            எனில் (!createDirectories(cache_path))
            {
                CV_LOG_ERROR(வெற்று, "Can't create OpenCV cache sub-directory: " << cache_path);
                cache_path.clear();
            }
        }
    }
    CV_Assert(cache_path.empty() || utils::fs::isDirectory(cache_path));
    எனில் (!cache_path.empty())
    {
        எனில் (!isPathSeparator(cache_path[cache_path.size() - 1]))
        {
            cache_path += native_separator;
        }
    }
    திரும்பு cache_path;
}

#இல்லையெனில்
#define NOT_IMPLEMENTED CV_Error(Error::StsNotImplemented, "");
cv::String canonical(மாறிலி cv::String& /*path*/) { NOT_IMPLEMENTED }
தருக்க exists(மாறிலி cv::String& /*path*/) { NOT_IMPLEMENTED }
வெறுமை remove_all(மாறிலி cv::String& /*path*/) { NOT_IMPLEMENTED }
cv::String getcwd() { NOT_IMPLEMENTED }
தருக்க createDirectory(மாறிலி cv::String& /*path*/) { NOT_IMPLEMENTED }
தருக்க createDirectories(மாறிலி cv::String& /*path*/) { NOT_IMPLEMENTED }
cv::String getCacheDirectory(மாறிலி எழுத்து* /*sub_directory_name*/, மாறிலி எழுத்து* /*configuration_name = NULL*/) { NOT_IMPLEMENTED }
#undef NOT_IMPLEMENTED
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT

}}} // namespace


#எனில் OPENCV_HAVE_FILESYSTEM_SUPPORT
#அந்தர்பாவ "plugin_loader.impl.hpp"
#endif
