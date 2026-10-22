// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#અન્તર્ભાવ "../precomp.hpp"

#અન્તર્ભાવ <opencv2/core/utils/configuration.private.hpp>

#અન્તર્ભાવ <opencv2/core/utils/logger.hpp>

#અન્તર્ભાવ "opencv2/core/utils/filesystem.private.hpp"
#અન્તર્ભાવ "opencv2/core/utils/filesystem.hpp"

//#define DEBUG_FS_UTILS

#ifdef DEBUG_FS_UTILS
#અન્તર્ભાવ <iostream>
#define DBG(...) __VA_ARGS__
#નહીંતર
#define DBG(...)
#endif


#જો OPENCV_HAVE_FILESYSTEM_SUPPORT

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#undef NOMINMAX
#define NOMINMAX
#અન્તર્ભાવ <windows.h>
#અન્તર્ભાવ <direct.h>
#અન્તર્ભાવ <sys/types.h>
#અન્તર્ભાવ <sys/stat.h>
#અન્તર્ભાવ <errno.h>
#અન્તર્ભાવ <io.h>
#અન્તર્ભાવ <stdio.h>
#અથવાજો defined __linux__ || defined __APPLE__ || defined __HAIKU__ || defined __FreeBSD__ || defined __GNU__ || defined __EMSCRIPTEN__ || defined __QNX__ || defined __CYGWIN__
#અન્તર્ભાવ <sys/types.h>
#અન્તર્ભાવ <sys/stat.h>
#અન્તર્ભાવ <fcntl.h>
#અન્તર્ભાવ <unistd.h>
#અન્તર્ભાવ <errno.h>
#endif

#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT

નામાકાશ cv { નામાકાશ utils { નામાકાશ fs {

#ifdef _WIN32
સ્થિર અચલ અક્ષર native_separator = '\\';
#નહીંતર
સ્થિર અચલ અક્ષર native_separator = '/';
#endif

સ્થિર અંતઃરેખ
તાર્કિક isPathSeparator(અક્ષર c)
{
    પાછા c == '/' || c == '\\';
}

cv::String join(અચલ cv::String& base, અચલ cv::String& path)
{
    જો (base.empty())
        પાછા path;
    જો (path.empty())
        પાછા base;

    તાર્કિક baseSep = isPathSeparator(base[base.size() - 1]);
    તાર્કિક pathSep = isPathSeparator(path[0]);
    String result;
    જો (baseSep && pathSep)
    {
        result = base + path.substr(1);
    }
    નહીંતર જો (!baseSep && !pathSep)
    {
        result = base + native_separator + path;
    }
    નહીંતર
    {
        result = base + path;
    }
    પાછા result;
}

CV_EXPORTS cv::String getParent(અચલ cv::String &path)
{
    std::string::size_type loc = path.find_last_of("/\\");
    જો (loc == std::string::npos)
        પાછા std::string();
    પાછા std::string(path, 0, loc);
}

CV_EXPORTS std::wstring getParent(અચલ std::wstring& path)
{
    std::wstring::size_type loc = path.find_last_of(L"/\\");
    જો (loc == std::wstring::npos)
        પાછા std::wstring();
    પાછા std::wstring(path, 0, loc);
}

#જો OPENCV_HAVE_FILESYSTEM_SUPPORT

cv::String canonical(અચલ cv::String& path)
{
    cv::String result;
#ifdef _WIN32
    અચલ અક્ષર* result_str = _fullpath(ખાલી, path.c_str(), 0);
#નહીંતર
    અચલ અક્ષર* result_str = realpath(path.c_str(), ખાલી);
#endif
    જો (result_str)
    {
        result = cv::String(result_str);
        મુક્ત((શૂન્ય*)result_str);
    }
    પાછા result.empty() ? path : result;
}


તાર્કિક exists(અચલ cv::String& path)
{
    CV_INSTRUMENT_REGION();

#જો defined _WIN32 || defined WINCE
    BOOL status = TRUE;
    {
        WIN32_FILE_ATTRIBUTE_DATA all_attrs;
#ifdef WINRT
        wchar_t wpath[MAX_PATH];
        size_t copied = mbstowcs(wpath, path.c_str(), MAX_PATH);
        CV_Assert((copied != MAX_PATH) && (copied != (size_t)-1));
        status = ::GetFileAttributesExW(wpath, GetFileExInfoStandard, &all_attrs);
#નહીંતર
        status = ::GetFileAttributesExA(path.c_str(), GetFileExInfoStandard, &all_attrs);
#endif
    }

    પાછા !!status;
#નહીંતર
    સંરચના stat stat_buf;
    પાછા (0 == stat(path.c_str(), &stat_buf));
#endif
}

CV_EXPORTS શૂન્ય remove_all(અચલ cv::String& path)
{
    જો (!exists(path))
        પાછા;
    જો (isDirectory(path))
    {
        std::vector<String> entries;
        utils::fs::glob(path, cv::String(), entries, ખોટું, સાચું);
        માટે (size_t i = 0; i < entries.size(); i++)
        {
            અચલ String& e = entries[i];
            remove_all(e);
        }
#ifdef _MSC_VER
        તાર્કિક result = _rmdir(path.c_str()) == 0;
#નહીંતર
        તાર્કિક result = rmdir(path.c_str()) == 0;
#endif
        જો (!result)
        {
            CV_LOG_ERROR(ખાલી, "Can't remove directory: " << path);
        }
    }
    નહીંતર
    {
#ifdef _MSC_VER
        તાર્કિક result = _unlink(path.c_str()) == 0;
#નહીંતર
        તાર્કિક result = unlink(path.c_str()) == 0;
#endif
        જો (!result)
        {
            CV_LOG_ERROR(ખાલી, "Can't remove file: " << path);
        }
    }
}


cv::String getcwd()
{
    CV_INSTRUMENT_REGION();
    cv::AutoBuffer<અક્ષર, 4096> buf;
#જો defined WIN32 || defined _WIN32 || defined WINCE
#ifdef WINRT
    પાછા cv::String();
#નહીંતર
    DWORD sz = GetCurrentDirectoryA(0, ખાલી);
    buf.allocate((size_t)sz);
    sz = GetCurrentDirectoryA((DWORD)buf.size(), buf.data());
    પાછા cv::String(buf.data(), (size_t)sz);
#endif
#અથવાજો defined __linux__ || defined __APPLE__ || defined __HAIKU__ || defined __FreeBSD__ || defined __EMSCRIPTEN__ || defined __QNX__ || defined __CYGWIN__
    માટે(;;)
    {
        અક્ષર* p = ::getcwd(buf.data(), buf.size());
        જો (p == ખાલી)
        {
            જો (errno == ERANGE)
            {
                buf.allocate(buf.size() * 2);
                ચાલુ;
            }
            પાછા cv::String();
        }
        તોડો;
    }
    પાછા cv::String(buf.data(), (size_t)strlen(buf.data()));
#નહીંતર
    પાછા cv::String();
#endif
}


તાર્કિક createDirectory(અચલ cv::String& path)
{
    CV_INSTRUMENT_REGION();
#જો defined WIN32 || defined _WIN32 || defined WINCE
#ifdef WINRT
    wchar_t wpath[MAX_PATH];
    size_t copied = mbstowcs(wpath, path.c_str(), MAX_PATH);
    CV_Assert((copied != MAX_PATH) && (copied != (size_t)-1));
    પૂર્ણાંક result = CreateDirectoryA(wpath, ખાલી) ? 0 : -1;
#નહીંતર
    પૂર્ણાંક result = _mkdir(path.c_str());
#endif
#અથવાજો defined __linux__ || defined __APPLE__ || defined __HAIKU__ || defined __FreeBSD__ || defined __EMSCRIPTEN__ || defined __QNX__ || defined __CYGWIN__
    પૂર્ણાંક result = mkdir(path.c_str(), 0777);
#નહીંતર
    પૂર્ણાંક result = -1;
#endif

    જો (result == -1)
    {
        પાછા isDirectory(path);
    }
    પાછા સાચું;
}

તાર્કિક createDirectories(અચલ cv::String& path_)
{
    cv::String path = path_;
    માટે (;;)
    {
        અક્ષર last_char = path.empty() ? 0 : path[path.length() - 1];
        જો (isPathSeparator(last_char))
        {
            path = path.substr(0, path.length() - 1);
            ચાલુ;
        }
        તોડો;
    }

    જો (path.empty() || path == "./" || path == ".\\" || path == ".")
        પાછા સાચું;
    જો (isDirectory(path))
        પાછા સાચું;

    size_t pos = path.rfind('/');
    જો (pos == cv::String::npos)
        pos = path.rfind('\\');
    જો (pos != cv::String::npos)
    {
        cv::String parent_directory = path.substr(0, pos);
        જો (!parent_directory.empty())
        {
            જો (!createDirectories(parent_directory))
                પાછા ખોટું;
        }
    }

    પાછા createDirectory(path);
}

#ifdef _WIN32

સંરચના FileLock::Impl
{
    Impl(અચલ અક્ષર* fname)
    {
        // http://support.microsoft.com/kb/316609
        પૂર્ણાંક અંક_Retries = 5;
        કરો
        {
            handle = ::CreateFileA(fname, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, ખાલી,
                                OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, ખાલી);
            જો (INVALID_HANDLE_કિંમત == handle)
            {
                જો (ERROR_SHARING_VIOLATION == GetLastError())
                {
                    અંક_Retries--;
                    Sleep(250);
                    ચાલુ;
                }
                નહીંતર
                {
                    CV_Error_(Error::StsAssert, ("Can't open lock file: %s", fname));
                }
            }
            તોડો;
        } જ્યારે (અંક_Retries > 0);
    }
    ~Impl()
    {
        જો (INVALID_HANDLE_કિંમત != handle)
        {
            ::CloseHandle(handle);
        }
    }

    તાર્કિક lock()
    {
        OVERLAPPED overlapped;
        std::memset(&overlapped, 0, કદ(overlapped));
        પાછા !!::LockFileEx(handle, LOCKFILE_EXCLUSIVE_LOCK, 0, MAXDWORD, MAXDWORD, &overlapped);
    }
    તાર્કિક unlock()
    {
        OVERLAPPED overlapped;
        std::memset(&overlapped, 0, કદ(overlapped));
        પાછા !!::UnlockFileEx(handle, 0, MAXDWORD, MAXDWORD, &overlapped);
    }

    તાર્કિક lock_shared()
    {
        OVERLAPPED overlapped;
        std::memset(&overlapped, 0, કદ(overlapped));
        પાછા !!::LockFileEx(handle, 0, 0, MAXDWORD, MAXDWORD, &overlapped);
    }
    તાર્કિક unlock_shared()
    {
        પાછા unlock();
    }

    HANDLE handle;

ખાનગી:
    Impl(અચલ Impl&); // disabled
    Impl& operator=(અચલ Impl&); // disabled
};

#અથવાજો defined __linux__ || defined __APPLE__ || defined __HAIKU__ || defined __FreeBSD__ || defined __GNU__ || defined __EMSCRIPTEN__ || defined __QNX__ || defined __CYGWIN__

સંરચના FileLock::Impl
{
    Impl(અચલ અક્ષર* fname)
    {
        handle = ::open(fname, O_RDWR);
        CV_Assert(handle != -1);
    }
    ~Impl()
    {
        જો (handle >= 0)
            ::close(handle);
    }

    તાર્કિક lock()
    {
        સંરચના ::flock l;
        std::memset(&l, 0, કદ(l));
        l.l_type = F_WRLCK;
        l.l_whence = SEEK_SET;
        l.l_start = 0;
        l.l_len = 0;
        DBG(std::cout << "Lock..." << std::endl);
        તાર્કિક res = -1 != ::fcntl(handle, F_SETLKW, &l);
        પાછા res;
    }
    તાર્કિક unlock()
    {
        સંરચના ::flock l;
        std::memset(&l, 0, કદ(l));
        l.l_type = F_UNLCK;
        l.l_whence = SEEK_SET;
        l.l_start = 0;
        l.l_len = 0;
        DBG(std::cout << "Unlock..." << std::endl);
        તાર્કિક res = -1 != ::fcntl(handle, F_SETLK, &l);
        પાછા res;
    }

    તાર્કિક lock_shared()
    {
        સંરચના ::flock l;
        std::memset(&l, 0, કદ(l));
        l.l_type = F_RDLCK;
        l.l_whence = SEEK_SET;
        l.l_start = 0;
        l.l_len = 0;
        DBG(std::cout << "Lock read..." << std::endl);
        તાર્કિક res = -1 != ::fcntl(handle, F_SETLKW, &l);
        પાછા res;
    }
    તાર્કિક unlock_shared()
    {
        પાછા unlock();
    }

    પૂર્ણાંક handle;

ખાનગી:
    Impl(અચલ Impl&); // disabled
    Impl& operator=(અચલ Impl&); // disabled
};

#endif

FileLock::FileLock(અચલ અક્ષર* fname)
    : pImpl(નવું Impl(fname))
{
    // nothing
}
FileLock::~FileLock()
{
    delete pImpl;
    pImpl = ખાલી;
}

શૂન્ય FileLock::lock() { CV_Assert(pImpl->lock()); }
શૂન્ય FileLock::unlock() { CV_Assert(pImpl->unlock()); }
શૂન્ય FileLock::lock_shared() { CV_Assert(pImpl->lock_shared()); }
શૂન્ય FileLock::unlock_shared() { CV_Assert(pImpl->unlock_shared()); }



cv::String getCacheDirectory(અચલ અક્ષર* sub_directory_name, અચલ અક્ષર* configuration_name)
{
    String cache_path;
    જો (configuration_name)
    {
        cache_path = utils::getConfigurationParameterString(configuration_name, "");
    }
    જો (cache_path.empty())
    {
        cv::String default_cache_path;
#ifdef _WIN32
        wchar_t tmp_path_buf_w[MAX_PATH+1] = {0};
        DWORD res = GetTempPathW(MAX_PATH, tmp_path_buf_w);
        જો (res > 0 && res <= MAX_PATH)
        {
            // Convert from UTF-16 to UTF-8 to support Unicode paths
            પૂર્ણાંક len = WideCharToMultiByte(CP_UTF8, 0, tmp_path_buf_w, -1, ખાલી, 0, ખાલી, ખાલી);
            જો (len > 0)
            {
                std::vector<અક્ષર> utf8_buf(len);
                WideCharToMultiByte(CP_UTF8, 0, tmp_path_buf_w, -1, utf8_buf.data(), len, ખાલી, ખાલી);
                default_cache_path = std::string(utf8_buf.data());
            }
        }
#અથવાજો defined __ANDROID__
        // no defaults
#અથવાજો defined __APPLE__
        અચલ std::string tmpdir_env = utils::getConfigurationParameterString("TMPDIR");
        જો (!tmpdir_env.empty() && utils::fs::isDirectory(tmpdir_env))
        {
            default_cache_path = tmpdir_env;
        }
        નહીંતર
        {
            default_cache_path = "/tmp/";
            CV_LOG_WARNING(ખાલી, "Using world accessible cache directory. This may be not secure: " << default_cache_path);
        }
#અથવાજો defined __linux__ || defined __HAIKU__ || defined __FreeBSD__ || defined __GNU__
        // https://specifications.freedesktop.org/basedir-spec/basedir-spec-latest.html
        જો (default_cache_path.empty())
        {
            અચલ std::string xdg_cache_env = utils::getConfigurationParameterString("XDG_CACHE_HOME");
            જો (!xdg_cache_env.empty() && utils::fs::isDirectory(xdg_cache_env))
            {
                default_cache_path = xdg_cache_env;
            }
        }
        જો (default_cache_path.empty())
        {
            અચલ std::string home_env = utils::getConfigurationParameterString("HOME");
            જો (!home_env.empty() && utils::fs::isDirectory(home_env))
            {
                cv::String home_path = home_env;
                cv::String home_cache_path = utils::fs::join(home_path, ".cache/");
                જો (utils::fs::isDirectory(home_cache_path))
                {
                    default_cache_path = home_cache_path;
                }
            }
        }
        જો (default_cache_path.empty())
        {
            અચલ અક્ષર* temp_path = "/var/tmp/";
            જો (utils::fs::isDirectory(temp_path))
            {
                default_cache_path = temp_path;
                CV_LOG_WARNING(ખાલી, "Using world accessible cache directory. This may be not secure: " << default_cache_path);
            }
        }
        જો (default_cache_path.empty())
        {
            default_cache_path = "/tmp/";
            CV_LOG_WARNING(ખાલી, "Using world accessible cache directory. This may be not secure: " << default_cache_path);
        }
#નહીંતર
        // no defaults
#endif
        CV_LOG_VERBOSE(ખાલી, 0, "default_cache_path = " << default_cache_path);
        જો (!default_cache_path.empty())
        {
            જો (utils::fs::isDirectory(default_cache_path))
            {
                cv::String default_cache_path_base = utils::fs::join(default_cache_path, "opencv");
                default_cache_path = utils::fs::join(default_cache_path_base, CVAUX_STR(CV_VERSION_MAJOR) ".x" CV_VERSION_STATUS);
                જો (utils::getConfigurationParameterBool("OPENCV_CACHE_SHOW_CLEANUP_MESSAGE", સાચું)
                    && !utils::fs::isDirectory(default_cache_path))
                {
                    std::vector<cv::String> existedCacheDirs;
                    પ્રયાસ
                    {
                        utils::fs::glob_relative(default_cache_path_base, "*", existedCacheDirs, ખોટું, સાચું);
                    }
                    પકડો (...)
                    {
                        // ignore
                    }
                    જો (!existedCacheDirs.empty())
                    {
                        CV_LOG_WARNING(ખાલી, "Creating new OpenCV cache directory: " << default_cache_path);
                        CV_LOG_WARNING(ખાલી, "There are several neighbour directories, probably created by old OpenCV versions.");
                        CV_LOG_WARNING(ખાલી, "Feel free to cleanup these unused directories:");
                        માટે (size_t i = 0; i < existedCacheDirs.size(); i++)
                        {
                            CV_LOG_WARNING(ખાલી, "  - " << existedCacheDirs[i]);
                        }
                        CV_LOG_WARNING(ખાલી, "Note: This message is showed only once.");
                    }
                }
                જો (sub_directory_name && sub_directory_name[0] != '\0')
                    default_cache_path = utils::fs::join(default_cache_path, cv::String(sub_directory_name) + native_separator);
                જો (!utils::fs::createDirectories(default_cache_path))
                {
                    CV_LOG_DEBUG(ખાલી, "Can't create OpenCV cache sub-directory: " << default_cache_path);
                }
                નહીંતર
                {
                    cache_path = default_cache_path;
                }
            }
            નહીંતર
            {
                CV_LOG_INFO(ખાલી, "Can't find default cache directory (does it exist?): " << default_cache_path);
            }
        }
        નહીંતર
        {
            CV_LOG_DEBUG(ખાલી, "OpenCV has no support to discover default cache directory on the current platform");
        }
    }
    નહીંતર
    {
        જો (cache_path == "disabled")
            પાછા cache_path;
        જો (!isDirectory(cache_path))
        {
            CV_LOG_WARNING(ખાલી, "Specified non-existed directory, creating OpenCV sub-directory for caching purposes: " << cache_path);
            જો (!createDirectories(cache_path))
            {
                CV_LOG_ERROR(ખાલી, "Can't create OpenCV cache sub-directory: " << cache_path);
                cache_path.clear();
            }
        }
    }
    CV_Assert(cache_path.empty() || utils::fs::isDirectory(cache_path));
    જો (!cache_path.empty())
    {
        જો (!isPathSeparator(cache_path[cache_path.size() - 1]))
        {
            cache_path += native_separator;
        }
    }
    પાછા cache_path;
}

#નહીંતર
#define NOT_IMPLEMENTED CV_Error(Error::StsNotImplemented, "");
cv::String canonical(અચલ cv::String& /*path*/) { NOT_IMPLEMENTED }
તાર્કિક exists(અચલ cv::String& /*path*/) { NOT_IMPLEMENTED }
શૂન્ય remove_all(અચલ cv::String& /*path*/) { NOT_IMPLEMENTED }
cv::String getcwd() { NOT_IMPLEMENTED }
તાર્કિક createDirectory(અચલ cv::String& /*path*/) { NOT_IMPLEMENTED }
તાર્કિક createDirectories(અચલ cv::String& /*path*/) { NOT_IMPLEMENTED }
cv::String getCacheDirectory(અચલ અક્ષર* /*sub_directory_name*/, અચલ અક્ષર* /*configuration_name = NULL*/) { NOT_IMPLEMENTED }
#undef NOT_IMPLEMENTED
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT

}}} // namespace


#જો OPENCV_HAVE_FILESYSTEM_SUPPORT
#અન્તર્ભાવ "plugin_loader.impl.hpp"
#endif
