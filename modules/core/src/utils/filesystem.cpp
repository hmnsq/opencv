// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#समावेश "../precomp.hpp"

#समावेश <opencv2/core/utils/configuration.private.hpp>

#समावेश <opencv2/core/utils/logger.hpp>

#समावेश "opencv2/core/utils/filesystem.private.hpp"
#समावेश "opencv2/core/utils/filesystem.hpp"

//#define DEBUG_FS_UTILS

#अगर_घोषित DEBUG_FS_UTILS
#समावेश <iostream>
#घोषणा DBG(...) __VA_ARGS__
#अन्यथा
#घोषणा DBG(...)
#पूर्ण_अगर


#अगर OPENCV_HAVE_FILESYSTEM_SUPPORT

#अगर_घोषित _WIN32
#घोषणा WIN32_LEAN_AND_MEAN
#अघोषित NOMINMAX
#घोषणा NOMINMAX
#समावेश <windows.h>
#समावेश <direct.h>
#समावेश <sys/types.h>
#समावेश <sys/stat.h>
#समावेश <errno.h>
#समावेश <io.h>
#समावेश <stdio.h>
#या_अगर defined __linux__ || defined __APPLE__ || defined __HAIKU__ || defined __FreeBSD__ || defined __GNU__ || defined __EMSCRIPTEN__ || defined __QNX__ || defined __CYGWIN__
#समावेश <sys/types.h>
#समावेश <sys/stat.h>
#समावेश <fcntl.h>
#समावेश <unistd.h>
#समावेश <errno.h>
#पूर्ण_अगर

#पूर्ण_अगर // OPENCV_HAVE_FILESYSTEM_SUPPORT

namespace cv { namespace utils { namespace fs {

#अगर_घोषित _WIN32
अटल स्थिर अक्षर native_separator = '\\';
#अन्यथा
अटल स्थिर अक्षर native_separator = '/';
#पूर्ण_अगर

अटल अंतरभूत
bool isPathSeparator(अक्षर c)
{
    वापस c == '/' || c == '\\';
}

cv::String join(स्थिर cv::String& base, स्थिर cv::String& path)
{
    अगर (base.empty())
        वापस path;
    अगर (path.empty())
        वापस base;

    bool baseSep = isPathSeparator(base[base.size() - 1]);
    bool pathSep = isPathSeparator(path[0]);
    String परिणाम;
    अगर (baseSep && pathSep)
    {
        परिणाम = base + path.substr(1);
    }
    अन्यथा अगर (!baseSep && !pathSep)
    {
        परिणाम = base + native_separator + path;
    }
    अन्यथा
    {
        परिणाम = base + path;
    }
    वापस परिणाम;
}

CV_EXPORTS cv::String getParent(स्थिर cv::String &path)
{
    std::string::size_type loc = path.find_last_of("/\\");
    अगर (loc == std::string::npos)
        वापस std::string();
    वापस std::string(path, 0, loc);
}

CV_EXPORTS std::wstring getParent(स्थिर std::wstring& path)
{
    std::wstring::size_type loc = path.find_last_of(L"/\\");
    अगर (loc == std::wstring::npos)
        वापस std::wstring();
    वापस std::wstring(path, 0, loc);
}

#अगर OPENCV_HAVE_FILESYSTEM_SUPPORT

cv::String canonical(स्थिर cv::String& path)
{
    cv::String परिणाम;
#अगर_घोषित _WIN32
    स्थिर अक्षर* परिणाम_str = _fullpath(शून्य, path.c_str(), 0);
#अन्यथा
    स्थिर अक्षर* परिणाम_str = realpath(path.c_str(), शून्य);
#पूर्ण_अगर
    अगर (परिणाम_str)
    {
        परिणाम = cv::String(परिणाम_str);
        मुक्त((व्योम*)परिणाम_str);
    }
    वापस परिणाम.empty() ? path : परिणाम;
}


bool exists(स्थिर cv::String& path)
{
    CV_INSTRUMENT_REGION();

#अगर defined _WIN32 || defined WINCE
    BOOL status = TRUE;
    {
        WIN32_FILE_ATTRIBUTE_तथ्य all_attrs;
#अगर_घोषित WINRT
        ब_अक्षर_प्रकार wpath[MAX_PATH];
        माप_प्रकार copied = mbstowcs(wpath, path.c_str(), MAX_PATH);
        CV_Assert((copied != MAX_PATH) && (copied != (माप_प्रकार)-1));
        status = ::GetFileAttributesExW(wpath, GetFileExInfoStandard, &all_attrs);
#अन्यथा
        status = ::GetFileAttributesExA(path.c_str(), GetFileExInfoStandard, &all_attrs);
#पूर्ण_अगर
    }

    वापस !!status;
#अन्यथा
    काष्ठा stat stat_buf;
    वापस (0 == stat(path.c_str(), &stat_buf));
#पूर्ण_अगर
}

CV_EXPORTS व्योम remove_all(स्थिर cv::String& path)
{
    अगर (!exists(path))
        वापस;
    अगर (isDirectory(path))
    {
        std::vector<String> entries;
        utils::fs::glob(path, cv::String(), entries, false, true);
        क्रम (माप_प्रकार i = 0; i < entries.size(); i++)
        {
            स्थिर String& e = entries[i];
            remove_all(e);
        }
#अगर_घोषित _MSC_VER
        bool परिणाम = _rmdir(path.c_str()) == 0;
#अन्यथा
        bool परिणाम = सूची_हटाओ(path.c_str()) == 0;
#पूर्ण_अगर
        अगर (!परिणाम)
        {
            CV_LOG_ERROR(शून्य, "Can't remove directory: " << path);
        }
    }
    अन्यथा
    {
#अगर_घोषित _MSC_VER
        bool परिणाम = _unlink(path.c_str()) == 0;
#अन्यथा
        bool परिणाम = unlink(path.c_str()) == 0;
#पूर्ण_अगर
        अगर (!परिणाम)
        {
            CV_LOG_ERROR(शून्य, "Can't remove file: " << path);
        }
    }
}


cv::String getcwd()
{
    CV_INSTRUMENT_REGION();
    cv::Auto_बफर<अक्षर, 4096> buf;
#अगर defined WIN32 || defined _WIN32 || defined WINCE
#अगर_घोषित WINRT
    वापस cv::String();
#अन्यथा
    DWORD sz = GetCurrentDirectoryA(0, शून्य);
    buf.allocate((माप_प्रकार)sz);
    sz = GetCurrentDirectoryA((DWORD)buf.size(), buf.तथ्य());
    वापस cv::String(buf.तथ्य(), (माप_प्रकार)sz);
#पूर्ण_अगर
#या_अगर defined __linux__ || defined __APPLE__ || defined __HAIKU__ || defined __FreeBSD__ || defined __EMSCRIPTEN__ || defined __QNX__ || defined __CYGWIN__
    क्रम(;;)
    {
        अक्षर* p = ::getcwd(buf.तथ्य(), buf.size());
        अगर (p == शून्य)
        {
            अगर (त्रुटि_सं == दुस्फल)
            {
                buf.allocate(buf.size() * 2);
                जारी;
            }
            वापस cv::String();
        }
        अवरोध;
    }
    वापस cv::String(buf.तथ्य(), (माप_प्रकार)म_माप(buf.तथ्य()));
#अन्यथा
    वापस cv::String();
#पूर्ण_अगर
}


bool createDirectory(स्थिर cv::String& path)
{
    CV_INSTRUMENT_REGION();
#अगर defined WIN32 || defined _WIN32 || defined WINCE
#अगर_घोषित WINRT
    ब_अक्षर_प्रकार wpath[MAX_PATH];
    माप_प्रकार copied = mbstowcs(wpath, path.c_str(), MAX_PATH);
    CV_Assert((copied != MAX_PATH) && (copied != (माप_प्रकार)-1));
    पूर्णांक परिणाम = CreateDirectoryA(wpath, शून्य) ? 0 : -1;
#अन्यथा
    पूर्णांक परिणाम = _mkdir(path.c_str());
#पूर्ण_अगर
#या_अगर defined __linux__ || defined __APPLE__ || defined __HAIKU__ || defined __FreeBSD__ || defined __EMSCRIPTEN__ || defined __QNX__ || defined __CYGWIN__
    पूर्णांक परिणाम = सूची_गढ़ो(path.c_str(), 0777);
#अन्यथा
    पूर्णांक परिणाम = -1;
#पूर्ण_अगर

    अगर (परिणाम == -1)
    {
        वापस isDirectory(path);
    }
    वापस true;
}

bool createDirectories(स्थिर cv::String& path_)
{
    cv::String path = path_;
    क्रम (;;)
    {
        अक्षर last_char = path.empty() ? 0 : path[path.length() - 1];
        अगर (isPathSeparator(last_char))
        {
            path = path.substr(0, path.length() - 1);
            जारी;
        }
        अवरोध;
    }

    अगर (path.empty() || path == "./" || path == ".\\" || path == ".")
        वापस true;
    अगर (isDirectory(path))
        वापस true;

    माप_प्रकार pos = path.rfind('/');
    अगर (pos == cv::String::npos)
        pos = path.rfind('\\');
    अगर (pos != cv::String::npos)
    {
        cv::String parent_directory = path.substr(0, pos);
        अगर (!parent_directory.empty())
        {
            अगर (!createDirectories(parent_directory))
                वापस false;
        }
    }

    वापस createDirectory(path);
}

#अगर_घोषित _WIN32

काष्ठा FileLock::Impl
{
    Impl(स्थिर अक्षर* fname)
    {
        // http://support.microsoft.com/kb/316609
        पूर्णांक अंक_Retries = 5;
        करो
        {
            handle = ::CreateFileA(fname, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, शून्य,
                                OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, शून्य);
            अगर (INVALID_HANDLE_मूल्य == handle)
            {
                अगर (ERROR_SHARING_VIOLATION == GetLastError())
                {
                    अंक_Retries--;
                    Sleep(250);
                    जारी;
                }
                अन्यथा
                {
                    CV_Error_(Error::StsAssert, ("Can't open lock file: %s", fname));
                }
            }
            अवरोध;
        } जबतक (अंक_Retries > 0);
    }
    ~Impl()
    {
        अगर (INVALID_HANDLE_मूल्य != handle)
        {
            ::CloseHandle(handle);
        }
    }

    bool lock()
    {
        OVERLAPPED overlapped;
        std::स_रखो(&overlapped, 0, माप(overlapped));
        वापस !!::LockFileEx(handle, LOCKFILE_EXCLUSIVE_LOCK, 0, MAXDWORD, MAXDWORD, &overlapped);
    }
    bool unlock()
    {
        OVERLAPPED overlapped;
        std::स_रखो(&overlapped, 0, माप(overlapped));
        वापस !!::UnlockFileEx(handle, 0, MAXDWORD, MAXDWORD, &overlapped);
    }

    bool lock_shared()
    {
        OVERLAPPED overlapped;
        std::स_रखो(&overlapped, 0, माप(overlapped));
        वापस !!::LockFileEx(handle, 0, 0, MAXDWORD, MAXDWORD, &overlapped);
    }
    bool unlock_shared()
    {
        वापस unlock();
    }

    HANDLE handle;

निजी:
    Impl(स्थिर Impl&); // disabled
    Impl& चालक=(स्थिर Impl&); // disabled
};

#या_अगर defined __linux__ || defined __APPLE__ || defined __HAIKU__ || defined __FreeBSD__ || defined __GNU__ || defined __EMSCRIPTEN__ || defined __QNX__ || defined __CYGWIN__

काष्ठा FileLock::Impl
{
    Impl(स्थिर अक्षर* fname)
    {
        handle = ::खोलो(fname, O_RDWR);
        CV_Assert(handle != -1);
    }
    ~Impl()
    {
        अगर (handle >= 0)
            ::बंद(handle);
    }

    bool lock()
    {
        काष्ठा ::flock l;
        std::स_रखो(&l, 0, माप(l));
        l.l_type = F_WRLCK;
        l.l_whence = शुरू_से;
        l.l_start = 0;
        l.l_len = 0;
        DBG(std::अ_बाहर << "Lock..." << std::प_पूर्ण);
        bool res = -1 != ::fcntl(handle, F_SETLKW, &l);
        वापस res;
    }
    bool unlock()
    {
        काष्ठा ::flock l;
        std::स_रखो(&l, 0, माप(l));
        l.l_type = F_UNLCK;
        l.l_whence = शुरू_से;
        l.l_start = 0;
        l.l_len = 0;
        DBG(std::अ_बाहर << "Unlock..." << std::प_पूर्ण);
        bool res = -1 != ::fcntl(handle, F_SETLK, &l);
        वापस res;
    }

    bool lock_shared()
    {
        काष्ठा ::flock l;
        std::स_रखो(&l, 0, माप(l));
        l.l_type = F_RDLCK;
        l.l_whence = शुरू_से;
        l.l_start = 0;
        l.l_len = 0;
        DBG(std::अ_बाहर << "Lock read..." << std::प_पूर्ण);
        bool res = -1 != ::fcntl(handle, F_SETLKW, &l);
        वापस res;
    }
    bool unlock_shared()
    {
        वापस unlock();
    }

    पूर्णांक handle;

निजी:
    Impl(स्थिर Impl&); // disabled
    Impl& चालक=(स्थिर Impl&); // disabled
};

#पूर्ण_अगर

FileLock::FileLock(स्थिर अक्षर* fname)
    : pImpl(नया Impl(fname))
{
    // nothing
}
FileLock::~FileLock()
{
    मिटाओ pImpl;
    pImpl = शून्य;
}

व्योम FileLock::lock() { CV_Assert(pImpl->lock()); }
व्योम FileLock::unlock() { CV_Assert(pImpl->unlock()); }
व्योम FileLock::lock_shared() { CV_Assert(pImpl->lock_shared()); }
व्योम FileLock::unlock_shared() { CV_Assert(pImpl->unlock_shared()); }



cv::String getCacheDirectory(स्थिर अक्षर* sub_directory_name, स्थिर अक्षर* configuration_name)
{
    String cache_path;
    अगर (configuration_name)
    {
        cache_path = utils::getConfigurationParameterString(configuration_name, "");
    }
    अगर (cache_path.empty())
    {
        cv::String default_cache_path;
#अगर_घोषित _WIN32
        ब_अक्षर_प्रकार tmp_path_buf_w[MAX_PATH+1] = {0};
        DWORD res = Get_अस्थायी_Path_W(MAX_PATH, tmp_path_buf_w);
        अगर (res > 0 && res <= MAX_PATH)
        {
            // Convert from UTF-16 to UTF-8 to support Unicode paths
            पूर्णांक len = WideCharToMultiByte(CP_UTF8, 0, tmp_path_buf_w, -1, शून्य, 0, शून्य, शून्य);
            अगर (len > 0)
            {
                std::vector<अक्षर> utf8_buf(len);
                WideCharToMultiByte(CP_UTF8, 0, tmp_path_buf_w, -1, utf8_buf.तथ्य(), len, शून्य, शून्य);
                default_cache_path = std::string(utf8_buf.तथ्य());
            }
        }
#या_अगर defined __ANDROID__
        // no defaults
#या_अगर defined __APPLE__
        स्थिर std::string tmpdir_env = utils::getConfigurationParameterString("TMPDIR");
        अगर (!tmpdir_env.empty() && utils::fs::isDirectory(tmpdir_env))
        {
            default_cache_path = tmpdir_env;
        }
        अन्यथा
        {
            default_cache_path = "/tmp/";
            CV_LOG_WARNING(शून्य, "Using world accessible cache directory. This may be not secure: " << default_cache_path);
        }
#या_अगर defined __linux__ || defined __HAIKU__ || defined __FreeBSD__ || defined __GNU__
        // https://specifications.freedesktop.org/basedir-spec/basedir-spec-latest.html
        अगर (default_cache_path.empty())
        {
            स्थिर std::string xdg_cache_env = utils::getConfigurationParameterString("XDG_CACHE_HOME");
            अगर (!xdg_cache_env.empty() && utils::fs::isDirectory(xdg_cache_env))
            {
                default_cache_path = xdg_cache_env;
            }
        }
        अगर (default_cache_path.empty())
        {
            स्थिर std::string home_env = utils::getConfigurationParameterString("HOME");
            अगर (!home_env.empty() && utils::fs::isDirectory(home_env))
            {
                cv::String home_path = home_env;
                cv::String home_cache_path = utils::fs::join(home_path, ".cache/");
                अगर (utils::fs::isDirectory(home_cache_path))
                {
                    default_cache_path = home_cache_path;
                }
            }
        }
        अगर (default_cache_path.empty())
        {
            स्थिर अक्षर* अस्थायी_path = "/var/tmp/";
            अगर (utils::fs::isDirectory(अस्थायी_path))
            {
                default_cache_path = अस्थायी_path;
                CV_LOG_WARNING(शून्य, "Using world accessible cache directory. This may be not secure: " << default_cache_path);
            }
        }
        अगर (default_cache_path.empty())
        {
            default_cache_path = "/tmp/";
            CV_LOG_WARNING(शून्य, "Using world accessible cache directory. This may be not secure: " << default_cache_path);
        }
#अन्यथा
        // no defaults
#पूर्ण_अगर
        CV_LOG_VERBOSE(शून्य, 0, "default_cache_path = " << default_cache_path);
        अगर (!default_cache_path.empty())
        {
            अगर (utils::fs::isDirectory(default_cache_path))
            {
                cv::String default_cache_path_base = utils::fs::join(default_cache_path, "opencv");
                default_cache_path = utils::fs::join(default_cache_path_base, CVAUX_STR(CV_VERSION_MAJOR) ".x" CV_VERSION_STATUS);
                अगर (utils::getConfigurationParameterBool("OPENCV_CACHE_SHOW_CLEANUP_MESSAGE", true)
                    && !utils::fs::isDirectory(default_cache_path))
                {
                    std::vector<cv::String> existedCacheDirs;
                    प्रयास
                    {
                        utils::fs::glob_relative(default_cache_path_base, "*", existedCacheDirs, false, true);
                    }
                    पकड़ो (...)
                    {
                        // ignore
                    }
                    अगर (!existedCacheDirs.empty())
                    {
                        CV_LOG_WARNING(शून्य, "Creating new OpenCV cache directory: " << default_cache_path);
                        CV_LOG_WARNING(शून्य, "There are several neighbour directories, probably created by old OpenCV versions.");
                        CV_LOG_WARNING(शून्य, "Feel free to cleanup these unused directories:");
                        क्रम (माप_प्रकार i = 0; i < existedCacheDirs.size(); i++)
                        {
                            CV_LOG_WARNING(शून्य, "  - " << existedCacheDirs[i]);
                        }
                        CV_LOG_WARNING(शून्य, "Note: This message is showed only once.");
                    }
                }
                अगर (sub_directory_name && sub_directory_name[0] != '\0')
                    default_cache_path = utils::fs::join(default_cache_path, cv::String(sub_directory_name) + native_separator);
                अगर (!utils::fs::createDirectories(default_cache_path))
                {
                    CV_LOG_DEBUG(शून्य, "Can't create OpenCV cache sub-directory: " << default_cache_path);
                }
                अन्यथा
                {
                    cache_path = default_cache_path;
                }
            }
            अन्यथा
            {
                CV_LOG_INFO(शून्य, "Can't find default cache directory (does it exist?): " << default_cache_path);
            }
        }
        अन्यथा
        {
            CV_LOG_DEBUG(शून्य, "OpenCV has no support to discover default cache directory on the current platform");
        }
    }
    अन्यथा
    {
        अगर (cache_path == "disabled")
            वापस cache_path;
        अगर (!isDirectory(cache_path))
        {
            CV_LOG_WARNING(शून्य, "Specified non-existed directory, creating OpenCV sub-directory for caching purposes: " << cache_path);
            अगर (!createDirectories(cache_path))
            {
                CV_LOG_ERROR(शून्य, "Can't create OpenCV cache sub-directory: " << cache_path);
                cache_path.सफा();
            }
        }
    }
    CV_Assert(cache_path.empty() || utils::fs::isDirectory(cache_path));
    अगर (!cache_path.empty())
    {
        अगर (!isPathSeparator(cache_path[cache_path.size() - 1]))
        {
            cache_path += native_separator;
        }
    }
    वापस cache_path;
}

#अन्यथा
#घोषणा NOT_IMPLEMENTED CV_Error(Error::StsNotImplemented, "");
cv::String canonical(स्थिर cv::String& /*path*/) { NOT_IMPLEMENTED }
bool exists(स्थिर cv::String& /*path*/) { NOT_IMPLEMENTED }
व्योम remove_all(स्थिर cv::String& /*path*/) { NOT_IMPLEMENTED }
cv::String getcwd() { NOT_IMPLEMENTED }
bool createDirectory(स्थिर cv::String& /*path*/) { NOT_IMPLEMENTED }
bool createDirectories(स्थिर cv::String& /*path*/) { NOT_IMPLEMENTED }
cv::String getCacheDirectory(स्थिर अक्षर* /*sub_directory_name*/, स्थिर अक्षर* /*configuration_name = NULL*/) { NOT_IMPLEMENTED }
#अघोषित NOT_IMPLEMENTED
#पूर्ण_अगर // OPENCV_HAVE_FILESYSTEM_SUPPORT

}}} // namespace


#अगर OPENCV_HAVE_FILESYSTEM_SUPPORT
#समावेश "plugin_loader.impl.hpp"
#पूर्ण_अगर
