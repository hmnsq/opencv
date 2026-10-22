// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ਅਨ੍ਤਰ੍ਭਾਵ "../precomp.hpp"

#ਅਨ੍ਤਰ੍ਭਾਵ <opencv2/core/utils/configuration.private.hpp>

#ਅਨ੍ਤਰ੍ਭਾਵ <opencv2/core/utils/logger.hpp>

#ਅਨ੍ਤਰ੍ਭਾਵ "opencv2/core/utils/filesystem.private.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "opencv2/core/utils/filesystem.hpp"

//#define DEBUG_FS_UTILS

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ DEBUG_FS_UTILS
#ਅਨ੍ਤਰ੍ਭਾਵ <iostream>
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DBG(...) __VA_ARGS__
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DBG(...)
#ਜੇ_ਅੰਤ


#ਜੇ OPENCV_HAVE_FILESYSTEM_SUPPORT

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ _WIN32
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ WIN32_LEAN_AND_ਔਸਤ
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ NOMINMAX
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NOMINMAX
#ਅਨ੍ਤਰ੍ਭਾਵ <windows.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <direct.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/stat.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <errno.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <io.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਜਾਂਜੇ defined __linux__ || defined __APPLE__ || defined __HAIKU__ || defined __FreeBSD__ || defined __GNU__ || defined __EMSCRIPTEN__ || defined __QNX__ || defined __CYGWIN__
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/stat.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <fcntl.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <errno.h>
#ਜੇ_ਅੰਤ

#ਜੇ_ਅੰਤ // OPENCV_HAVE_FILESYSTEM_SUPPORT

ਨਾਮਾਕਾਸ਼ cv { ਨਾਮਾਕਾਸ਼ utils { ਨਾਮਾਕਾਸ਼ fs {

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ _WIN32
ਸਥਿਰ ਅਚਲ ਅੱਖਰ native_separator = '\\';
#ਨਹੀਂਤਾਂ
ਸਥਿਰ ਅਚਲ ਅੱਖਰ native_separator = '/';
#ਜੇ_ਅੰਤ

ਸਥਿਰ ਸਤਰਅੰਦਰ
ਤਾਰਕਿਕ is_ਰਸਤਾ_Separator(ਅੱਖਰ c)
{
    ਵਾਪਸ c == '/' || c == '\\';
}

cv::String join(ਅਚਲ cv::String& ਅਧਾਰ, ਅਚਲ cv::String& ਰਸਤਾ)
{
    ਜੇ (ਅਧਾਰ.empty())
        ਵਾਪਸ ਰਸਤਾ;
    ਜੇ (ਰਸਤਾ.empty())
        ਵਾਪਸ ਅਧਾਰ;

    ਤਾਰਕਿਕ ਅਧਾਰ_Sep = is_ਰਸਤਾ_Separator(ਅਧਾਰ[ਅਧਾਰ.size() - 1]);
    ਤਾਰਕਿਕ ਰਸਤਾ_Sep = is_ਰਸਤਾ_Separator(ਰਸਤਾ[0]);
    String ਨਤੀਜਾ;
    ਜੇ (ਅਧਾਰ_Sep && ਰਸਤਾ_Sep)
    {
        ਨਤੀਜਾ = ਅਧਾਰ + ਰਸਤਾ.substr(1);
    }
    ਨਹੀਂਤਾਂ ਜੇ (!ਅਧਾਰ_Sep && !ਰਸਤਾ_Sep)
    {
        ਨਤੀਜਾ = ਅਧਾਰ + native_separator + ਰਸਤਾ;
    }
    ਨਹੀਂਤਾਂ
    {
        ਨਤੀਜਾ = ਅਧਾਰ + ਰਸਤਾ;
    }
    ਵਾਪਸ ਨਤੀਜਾ;
}

CV_EXPORTS cv::String get_ਮਾਪਾ(ਅਚਲ cv::String &ਰਸਤਾ)
{
    std::string::size_type loc = ਰਸਤਾ.find_last_of("/\\");
    ਜੇ (loc == std::string::npos)
        ਵਾਪਸ std::string();
    ਵਾਪਸ std::string(ਰਸਤਾ, 0, loc);
}

CV_EXPORTS std::wstring get_ਮਾਪਾ(ਅਚਲ std::wstring& ਰਸਤਾ)
{
    std::wstring::size_type loc = ਰਸਤਾ.find_last_of(L"/\\");
    ਜੇ (loc == std::wstring::npos)
        ਵਾਪਸ std::wstring();
    ਵਾਪਸ std::wstring(ਰਸਤਾ, 0, loc);
}

#ਜੇ OPENCV_HAVE_FILESYSTEM_SUPPORT

cv::String canonical(ਅਚਲ cv::String& ਰਸਤਾ)
{
    cv::String ਨਤੀਜਾ;
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ _WIN32
    ਅਚਲ ਅੱਖਰ* ਨਤੀਜਾ_str = _fullpath(ਨਿਰਾ, ਰਸਤਾ.c_str(), 0);
#ਨਹੀਂਤਾਂ
    ਅਚਲ ਅੱਖਰ* ਨਤੀਜਾ_str = realpath(ਰਸਤਾ.c_str(), ਨਿਰਾ);
#ਜੇ_ਅੰਤ
    ਜੇ (ਨਤੀਜਾ_str)
    {
        ਨਤੀਜਾ = cv::String(ਨਤੀਜਾ_str);
        ਮੁਕਤ((ਖਾਲੀ*)ਨਤੀਜਾ_str);
    }
    ਵਾਪਸ ਨਤੀਜਾ.empty() ? ਰਸਤਾ : ਨਤੀਜਾ;
}


ਤਾਰਕਿਕ exists(ਅਚਲ cv::String& ਰਸਤਾ)
{
    CV_INSTRUMENT_REGION();

#ਜੇ defined _WIN32 || defined WINCE
    BOOL ਸਥਿਤੀ = TRUE;
    {
        WIN32_FILE_ATTRIBUTE_ਅੰਕੜੇ all_attrs;
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ WINRT
        wchar_t wpath[MAX_ਰਸਤਾ];
        size_t copied = mbstowcs(wpath, ਰਸਤਾ.c_str(), MAX_ਰਸਤਾ);
        CV_Assert((copied != MAX_ਰਸਤਾ) && (copied != (size_t)-1));
        ਸਥਿਤੀ = ::Get_File_ਗੁਣ_Ex_W(wpath, GetFileExInfoStandard, &all_attrs);
#ਨਹੀਂਤਾਂ
        ਸਥਿਤੀ = ::Get_File_ਗੁਣ_Ex_A(ਰਸਤਾ.c_str(), GetFileExInfoStandard, &all_attrs);
#ਜੇ_ਅੰਤ
    }

    ਵਾਪਸ !!ਸਥਿਤੀ;
#ਨਹੀਂਤਾਂ
    ਬਣਤਰ stat stat_buf;
    ਵਾਪਸ (0 == stat(ਰਸਤਾ.c_str(), &stat_buf));
#ਜੇ_ਅੰਤ
}

CV_EXPORTS ਖਾਲੀ remove_all(ਅਚਲ cv::String& ਰਸਤਾ)
{
    ਜੇ (!exists(ਰਸਤਾ))
        ਵਾਪਸ;
    ਜੇ (isDirectory(ਰਸਤਾ))
    {
        std::vector<String> entries;
        utils::fs::glob(ਰਸਤਾ, cv::String(), entries, ਝੂਠ, ਸੱਚ);
        ਲਈ (size_t i = 0; i < entries.size(); i++)
        {
            ਅਚਲ String& e = entries[i];
            remove_all(e);
        }
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ _MSC_VER
        ਤਾਰਕਿਕ ਨਤੀਜਾ = _rmdir(ਰਸਤਾ.c_str()) == 0;
#ਨਹੀਂਤਾਂ
        ਤਾਰਕਿਕ ਨਤੀਜਾ = rmdir(ਰਸਤਾ.c_str()) == 0;
#ਜੇ_ਅੰਤ
        ਜੇ (!ਨਤੀਜਾ)
        {
            CV_LOG_ERROR(ਨਿਰਾ, "Can't remove directory: " << ਰਸਤਾ);
        }
    }
    ਨਹੀਂਤਾਂ
    {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ _MSC_VER
        ਤਾਰਕਿਕ ਨਤੀਜਾ = _unlink(ਰਸਤਾ.c_str()) == 0;
#ਨਹੀਂਤਾਂ
        ਤਾਰਕਿਕ ਨਤੀਜਾ = unlink(ਰਸਤਾ.c_str()) == 0;
#ਜੇ_ਅੰਤ
        ਜੇ (!ਨਤੀਜਾ)
        {
            CV_LOG_ERROR(ਨਿਰਾ, "Can't remove file: " << ਰਸਤਾ);
        }
    }
}


cv::String getcwd()
{
    CV_INSTRUMENT_REGION();
    cv::Auto_ਬਫਰ<ਅੱਖਰ, 4096> buf;
#ਜੇ defined WIN32 || defined _WIN32 || defined WINCE
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ WINRT
    ਵਾਪਸ cv::String();
#ਨਹੀਂਤਾਂ
    DWORD sz = GetCurrentDirectoryA(0, ਨਿਰਾ);
    buf.allocate((size_t)sz);
    sz = GetCurrentDirectoryA((DWORD)buf.size(), buf.ਅੰਕੜੇ());
    ਵਾਪਸ cv::String(buf.ਅੰਕੜੇ(), (size_t)sz);
#ਜੇ_ਅੰਤ
#ਜਾਂਜੇ defined __linux__ || defined __APPLE__ || defined __HAIKU__ || defined __FreeBSD__ || defined __EMSCRIPTEN__ || defined __QNX__ || defined __CYGWIN__
    ਲਈ(;;)
    {
        ਅੱਖਰ* p = ::getcwd(buf.ਅੰਕੜੇ(), buf.size());
        ਜੇ (p == ਨਿਰਾ)
        {
            ਜੇ (errno == ERANGE)
            {
                buf.allocate(buf.size() * 2);
                ਜਾਰੀ;
            }
            ਵਾਪਸ cv::String();
        }
        ਰੋਕੋ;
    }
    ਵਾਪਸ cv::String(buf.ਅੰਕੜੇ(), (size_t)ਲੜੀ_ਲੰਬਾਈ(buf.ਅੰਕੜੇ()));
#ਨਹੀਂਤਾਂ
    ਵਾਪਸ cv::String();
#ਜੇ_ਅੰਤ
}


ਤਾਰਕਿਕ createDirectory(ਅਚਲ cv::String& ਰਸਤਾ)
{
    CV_INSTRUMENT_REGION();
#ਜੇ defined WIN32 || defined _WIN32 || defined WINCE
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ WINRT
    wchar_t wpath[MAX_ਰਸਤਾ];
    size_t copied = mbstowcs(wpath, ਰਸਤਾ.c_str(), MAX_ਰਸਤਾ);
    CV_Assert((copied != MAX_ਰਸਤਾ) && (copied != (size_t)-1));
    ਪੂਰਨ ਨਤੀਜਾ = CreateDirectoryA(wpath, ਨਿਰਾ) ? 0 : -1;
#ਨਹੀਂਤਾਂ
    ਪੂਰਨ ਨਤੀਜਾ = _mkdir(ਰਸਤਾ.c_str());
#ਜੇ_ਅੰਤ
#ਜਾਂਜੇ defined __linux__ || defined __APPLE__ || defined __HAIKU__ || defined __FreeBSD__ || defined __EMSCRIPTEN__ || defined __QNX__ || defined __CYGWIN__
    ਪੂਰਨ ਨਤੀਜਾ = mkdir(ਰਸਤਾ.c_str(), 0777);
#ਨਹੀਂਤਾਂ
    ਪੂਰਨ ਨਤੀਜਾ = -1;
#ਜੇ_ਅੰਤ

    ਜੇ (ਨਤੀਜਾ == -1)
    {
        ਵਾਪਸ isDirectory(ਰਸਤਾ);
    }
    ਵਾਪਸ ਸੱਚ;
}

ਤਾਰਕਿਕ createDirectories(ਅਚਲ cv::String& ਰਸਤਾ_3)
{
    cv::String ਰਸਤਾ = ਰਸਤਾ_3;
    ਲਈ (;;)
    {
        ਅੱਖਰ ਆਖਰੀ_char = ਰਸਤਾ.empty() ? 0 : ਰਸਤਾ[ਰਸਤਾ.length() - 1];
        ਜੇ (is_ਰਸਤਾ_Separator(ਆਖਰੀ_char))
        {
            ਰਸਤਾ = ਰਸਤਾ.substr(0, ਰਸਤਾ.length() - 1);
            ਜਾਰੀ;
        }
        ਰੋਕੋ;
    }

    ਜੇ (ਰਸਤਾ.empty() || ਰਸਤਾ == "./" || ਰਸਤਾ == ".\\" || ਰਸਤਾ == ".")
        ਵਾਪਸ ਸੱਚ;
    ਜੇ (isDirectory(ਰਸਤਾ))
        ਵਾਪਸ ਸੱਚ;

    size_t pos = ਰਸਤਾ.rfind('/');
    ਜੇ (pos == cv::String::npos)
        pos = ਰਸਤਾ.rfind('\\');
    ਜੇ (pos != cv::String::npos)
    {
        cv::String ਮਾਪਾ_directory = ਰਸਤਾ.substr(0, pos);
        ਜੇ (!ਮਾਪਾ_directory.empty())
        {
            ਜੇ (!createDirectories(ਮਾਪਾ_directory))
                ਵਾਪਸ ਝੂਠ;
        }
    }

    ਵਾਪਸ createDirectory(ਰਸਤਾ);
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ _WIN32

ਬਣਤਰ File_ਤਾਲਾ::Impl
{
    Impl(ਅਚਲ ਅੱਖਰ* fname)
    {
        // http://support.microsoft.com/kb/316609
        ਪੂਰਨ ਅੰਕ_Retries = 5;
        ਕਰੋ
        {
            handle = ::CreateFileA(fname, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, ਨਿਰਾ,
                                OPEN_EXISTING, FILE_ATTRIBUTE_ਸਾਧਾਰਨ, ਨਿਰਾ);
            ਜੇ (INVALID_HANDLE_ਮੁੱਲ == handle)
            {
                ਜੇ (ERROR_SHARING_VIOLATION == Get_ਆਖਰੀ_Error())
                {
                    ਅੰਕ_Retries--;
                    Sleep(250);
                    ਜਾਰੀ;
                }
                ਨਹੀਂਤਾਂ
                {
                    CV_Error_(Error::StsAssert, ("Can't open lock file: %s", fname));
                }
            }
            ਰੋਕੋ;
        } ਜਦੋਂ (ਅੰਕ_Retries > 0);
    }
    ~Impl()
    {
        ਜੇ (INVALID_HANDLE_ਮੁੱਲ != handle)
        {
            ::CloseHandle(handle);
        }
    }

    ਤਾਰਕਿਕ lock()
    {
        OVERLAPPED overlapped;
        std::memset(&overlapped, 0, ਆਕਾਰ(overlapped));
        ਵਾਪਸ !!::ਤਾਲਾ_File_Ex(handle, LOCKFILE_EXCLUSIVE_ਤਾਲਾ, 0, MAXDWORD, MAXDWORD, &overlapped);
    }
    ਤਾਰਕਿਕ unlock()
    {
        OVERLAPPED overlapped;
        std::memset(&overlapped, 0, ਆਕਾਰ(overlapped));
        ਵਾਪਸ !!::UnlockFileEx(handle, 0, MAXDWORD, MAXDWORD, &overlapped);
    }

    ਤਾਰਕਿਕ ਤਾਲਾ_shared()
    {
        OVERLAPPED overlapped;
        std::memset(&overlapped, 0, ਆਕਾਰ(overlapped));
        ਵਾਪਸ !!::ਤਾਲਾ_File_Ex(handle, 0, 0, MAXDWORD, MAXDWORD, &overlapped);
    }
    ਤਾਰਕਿਕ unlock_shared()
    {
        ਵਾਪਸ unlock();
    }

    HANDLE handle;

ਨਿਜੀ:
    Impl(ਅਚਲ Impl&); // disabled
    Impl& ਸੰਚਾਲਕ=(ਅਚਲ Impl&); // disabled
};

#ਜਾਂਜੇ defined __linux__ || defined __APPLE__ || defined __HAIKU__ || defined __FreeBSD__ || defined __GNU__ || defined __EMSCRIPTEN__ || defined __QNX__ || defined __CYGWIN__

ਬਣਤਰ File_ਤਾਲਾ::Impl
{
    Impl(ਅਚਲ ਅੱਖਰ* fname)
    {
        handle = ::open(fname, O_RDWR);
        CV_Assert(handle != -1);
    }
    ~Impl()
    {
        ਜੇ (handle >= 0)
            ::close(handle);
    }

    ਤਾਰਕਿਕ lock()
    {
        ਬਣਤਰ ::flock l;
        std::memset(&l, 0, ਆਕਾਰ(l));
        l.l_type = F_WRLCK;
        l.l_whence = SEEK_SET;
        l.l_ਸ਼ੁਰੂ = 0;
        l.l_len = 0;
        DBG(std::ਬਾਹਰ << "Lock..." << std::ਸਤਰ_ਅੰਤ);
        ਤਾਰਕਿਕ res = -1 != ::fcntl(handle, F_SETLKW, &l);
        ਵਾਪਸ res;
    }
    ਤਾਰਕਿਕ unlock()
    {
        ਬਣਤਰ ::flock l;
        std::memset(&l, 0, ਆਕਾਰ(l));
        l.l_type = F_UNLCK;
        l.l_whence = SEEK_SET;
        l.l_ਸ਼ੁਰੂ = 0;
        l.l_len = 0;
        DBG(std::ਬਾਹਰ << "Unlock..." << std::ਸਤਰ_ਅੰਤ);
        ਤਾਰਕਿਕ res = -1 != ::fcntl(handle, F_SETLK, &l);
        ਵਾਪਸ res;
    }

    ਤਾਰਕਿਕ ਤਾਲਾ_shared()
    {
        ਬਣਤਰ ::flock l;
        std::memset(&l, 0, ਆਕਾਰ(l));
        l.l_type = F_RDLCK;
        l.l_whence = SEEK_SET;
        l.l_ਸ਼ੁਰੂ = 0;
        l.l_len = 0;
        DBG(std::ਬਾਹਰ << "Lock read..." << std::ਸਤਰ_ਅੰਤ);
        ਤਾਰਕਿਕ res = -1 != ::fcntl(handle, F_SETLKW, &l);
        ਵਾਪਸ res;
    }
    ਤਾਰਕਿਕ unlock_shared()
    {
        ਵਾਪਸ unlock();
    }

    ਪੂਰਨ handle;

ਨਿਜੀ:
    Impl(ਅਚਲ Impl&); // disabled
    Impl& ਸੰਚਾਲਕ=(ਅਚਲ Impl&); // disabled
};

#ਜੇ_ਅੰਤ

File_ਤਾਲਾ::File_ਤਾਲਾ(ਅਚਲ ਅੱਖਰ* fname)
    : pImpl(ਨਵਾਂ Impl(fname))
{
    // nothing
}
File_ਤਾਲਾ::~File_ਤਾਲਾ()
{
    ਮਿਟਾਓ pImpl;
    pImpl = ਨਿਰਾ;
}

ਖਾਲੀ File_ਤਾਲਾ::lock() { CV_Assert(pImpl->lock()); }
ਖਾਲੀ File_ਤਾਲਾ::unlock() { CV_Assert(pImpl->unlock()); }
ਖਾਲੀ File_ਤਾਲਾ::ਤਾਲਾ_shared() { CV_Assert(pImpl->ਤਾਲਾ_shared()); }
ਖਾਲੀ File_ਤਾਲਾ::unlock_shared() { CV_Assert(pImpl->unlock_shared()); }



cv::String getCacheDirectory(ਅਚਲ ਅੱਖਰ* sub_directory_name, ਅਚਲ ਅੱਖਰ* configuration_name)
{
    String cache_ਰਸਤਾ_2;
    ਜੇ (configuration_name)
    {
        cache_ਰਸਤਾ_2 = utils::getConfigurationParameterString(configuration_name, "");
    }
    ਜੇ (cache_ਰਸਤਾ_2.empty())
    {
        cv::String default_cache_ਰਸਤਾ;
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ _WIN32
        wchar_t tmp_ਰਸਤਾ_buf_w[MAX_ਰਸਤਾ+1] = {0};
        DWORD res = Get_ਆਰਜ਼ੀ_ਰਸਤਾ_W(MAX_ਰਸਤਾ, tmp_ਰਸਤਾ_buf_w);
        ਜੇ (res > 0 && res <= MAX_ਰਸਤਾ)
        {
            // Convert from UTF-16 to UTF-8 to support Unicode paths
            ਪੂਰਨ len = Wide_Char_ਮੰਜ਼ਿਲ_Multi_Byte(CP_UTF8, 0, tmp_ਰਸਤਾ_buf_w, -1, ਨਿਰਾ, 0, ਨਿਰਾ, ਨਿਰਾ);
            ਜੇ (len > 0)
            {
                std::vector<ਅੱਖਰ> utf8_buf(len);
                Wide_Char_ਮੰਜ਼ਿਲ_Multi_Byte(CP_UTF8, 0, tmp_ਰਸਤਾ_buf_w, -1, utf8_buf.ਅੰਕੜੇ(), len, ਨਿਰਾ, ਨਿਰਾ);
                default_cache_ਰਸਤਾ = std::string(utf8_buf.ਅੰਕੜੇ());
            }
        }
#ਜਾਂਜੇ defined __ANDROID__
        // no defaults
#ਜਾਂਜੇ defined __APPLE__
        ਅਚਲ std::string tmpdir_env = utils::getConfigurationParameterString("TMPDIR");
        ਜੇ (!tmpdir_env.empty() && utils::fs::isDirectory(tmpdir_env))
        {
            default_cache_ਰਸਤਾ = tmpdir_env;
        }
        ਨਹੀਂਤਾਂ
        {
            default_cache_ਰਸਤਾ = "/tmp/";
            CV_LOG_WARNING(ਨਿਰਾ, "Using world accessible cache directory. This may be not secure: " << default_cache_ਰਸਤਾ);
        }
#ਜਾਂਜੇ defined __linux__ || defined __HAIKU__ || defined __FreeBSD__ || defined __GNU__
        // https://specifications.freedesktop.org/basedir-spec/basedir-spec-latest.html
        ਜੇ (default_cache_ਰਸਤਾ.empty())
        {
            ਅਚਲ std::string xdg_cache_env = utils::getConfigurationParameterString("XDG_CACHE_HOME");
            ਜੇ (!xdg_cache_env.empty() && utils::fs::isDirectory(xdg_cache_env))
            {
                default_cache_ਰਸਤਾ = xdg_cache_env;
            }
        }
        ਜੇ (default_cache_ਰਸਤਾ.empty())
        {
            ਅਚਲ std::string home_env = utils::getConfigurationParameterString("HOME");
            ਜੇ (!home_env.empty() && utils::fs::isDirectory(home_env))
            {
                cv::String home_ਰਸਤਾ = home_env;
                cv::String home_cache_ਰਸਤਾ = utils::fs::join(home_ਰਸਤਾ, ".cache/");
                ਜੇ (utils::fs::isDirectory(home_cache_ਰਸਤਾ))
                {
                    default_cache_ਰਸਤਾ = home_cache_ਰਸਤਾ;
                }
            }
        }
        ਜੇ (default_cache_ਰਸਤਾ.empty())
        {
            ਅਚਲ ਅੱਖਰ* ਆਰਜ਼ੀ_ਰਸਤਾ = "/var/tmp/";
            ਜੇ (utils::fs::isDirectory(ਆਰਜ਼ੀ_ਰਸਤਾ))
            {
                default_cache_ਰਸਤਾ = ਆਰਜ਼ੀ_ਰਸਤਾ;
                CV_LOG_WARNING(ਨਿਰਾ, "Using world accessible cache directory. This may be not secure: " << default_cache_ਰਸਤਾ);
            }
        }
        ਜੇ (default_cache_ਰਸਤਾ.empty())
        {
            default_cache_ਰਸਤਾ = "/tmp/";
            CV_LOG_WARNING(ਨਿਰਾ, "Using world accessible cache directory. This may be not secure: " << default_cache_ਰਸਤਾ);
        }
#ਨਹੀਂਤਾਂ
        // no defaults
#ਜੇ_ਅੰਤ
        CV_LOG_VERBOSE(ਨਿਰਾ, 0, "default_cache_path = " << default_cache_ਰਸਤਾ);
        ਜੇ (!default_cache_ਰਸਤਾ.empty())
        {
            ਜੇ (utils::fs::isDirectory(default_cache_ਰਸਤਾ))
            {
                cv::String default_cache_ਰਸਤਾ_ਅਧਾਰ = utils::fs::join(default_cache_ਰਸਤਾ, "opencv");
                default_cache_ਰਸਤਾ = utils::fs::join(default_cache_ਰਸਤਾ_ਅਧਾਰ, CVAUX_STR(CV_ਸੰਸਕਰਣ_MAJOR) ".x" CV_ਸੰਸਕਰਣ_ਸਥਿਤੀ);
                ਜੇ (utils::getConfigurationParameterBool("OPENCV_CACHE_SHOW_CLEANUP_MESSAGE", ਸੱਚ)
                    && !utils::fs::isDirectory(default_cache_ਰਸਤਾ))
                {
                    std::vector<cv::String> existedCacheDirs;
                    ਕੋਸ਼ਿਸ਼
                    {
                        utils::fs::glob_relative(default_cache_ਰਸਤਾ_ਅਧਾਰ, "*", existedCacheDirs, ਝੂਠ, ਸੱਚ);
                    }
                    ਫੜੋ (...)
                    {
                        // ignore
                    }
                    ਜੇ (!existedCacheDirs.empty())
                    {
                        CV_LOG_WARNING(ਨਿਰਾ, "Creating new OpenCV cache directory: " << default_cache_ਰਸਤਾ);
                        CV_LOG_WARNING(ਨਿਰਾ, "There are several neighbour directories, probably created by old OpenCV versions.");
                        CV_LOG_WARNING(ਨਿਰਾ, "Feel free to cleanup these unused directories:");
                        ਲਈ (size_t i = 0; i < existedCacheDirs.size(); i++)
                        {
                            CV_LOG_WARNING(ਨਿਰਾ, "  - " << existedCacheDirs[i]);
                        }
                        CV_LOG_WARNING(ਨਿਰਾ, "Note: This message is showed only once.");
                    }
                }
                ਜੇ (sub_directory_name && sub_directory_name[0] != '\0')
                    default_cache_ਰਸਤਾ = utils::fs::join(default_cache_ਰਸਤਾ, cv::String(sub_directory_name) + native_separator);
                ਜੇ (!utils::fs::createDirectories(default_cache_ਰਸਤਾ))
                {
                    CV_LOG_DEBUG(ਨਿਰਾ, "Can't create OpenCV cache sub-directory: " << default_cache_ਰਸਤਾ);
                }
                ਨਹੀਂਤਾਂ
                {
                    cache_ਰਸਤਾ_2 = default_cache_ਰਸਤਾ;
                }
            }
            ਨਹੀਂਤਾਂ
            {
                CV_LOG_INFO(ਨਿਰਾ, "Can't find default cache directory (does it exist?): " << default_cache_ਰਸਤਾ);
            }
        }
        ਨਹੀਂਤਾਂ
        {
            CV_LOG_DEBUG(ਨਿਰਾ, "OpenCV has no support to discover default cache directory on the current platform");
        }
    }
    ਨਹੀਂਤਾਂ
    {
        ਜੇ (cache_ਰਸਤਾ_2 == "disabled")
            ਵਾਪਸ cache_ਰਸਤਾ_2;
        ਜੇ (!isDirectory(cache_ਰਸਤਾ_2))
        {
            CV_LOG_WARNING(ਨਿਰਾ, "Specified non-existed directory, creating OpenCV sub-directory for caching purposes: " << cache_ਰਸਤਾ_2);
            ਜੇ (!createDirectories(cache_ਰਸਤਾ_2))
            {
                CV_LOG_ERROR(ਨਿਰਾ, "Can't create OpenCV cache sub-directory: " << cache_ਰਸਤਾ_2);
                cache_ਰਸਤਾ_2.clear();
            }
        }
    }
    CV_Assert(cache_ਰਸਤਾ_2.empty() || utils::fs::isDirectory(cache_ਰਸਤਾ_2));
    ਜੇ (!cache_ਰਸਤਾ_2.empty())
    {
        ਜੇ (!is_ਰਸਤਾ_Separator(cache_ਰਸਤਾ_2[cache_ਰਸਤਾ_2.size() - 1]))
        {
            cache_ਰਸਤਾ_2 += native_separator;
        }
    }
    ਵਾਪਸ cache_ਰਸਤਾ_2;
}

#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NOT_IMPLEMENTED CV_Error(Error::StsNotImplemented, "");
cv::String canonical(ਅਚਲ cv::String& /*ਰਸਤਾ*/) { NOT_IMPLEMENTED }
ਤਾਰਕਿਕ exists(ਅਚਲ cv::String& /*ਰਸਤਾ*/) { NOT_IMPLEMENTED }
ਖਾਲੀ remove_all(ਅਚਲ cv::String& /*ਰਸਤਾ*/) { NOT_IMPLEMENTED }
cv::String getcwd() { NOT_IMPLEMENTED }
ਤਾਰਕਿਕ createDirectory(ਅਚਲ cv::String& /*ਰਸਤਾ*/) { NOT_IMPLEMENTED }
ਤਾਰਕਿਕ createDirectories(ਅਚਲ cv::String& /*ਰਸਤਾ*/) { NOT_IMPLEMENTED }
cv::String getCacheDirectory(ਅਚਲ ਅੱਖਰ* /*sub_directory_name*/, ਅਚਲ ਅੱਖਰ* /*configuration_name = NULL*/) { NOT_IMPLEMENTED }
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ NOT_IMPLEMENTED
#ਜੇ_ਅੰਤ // OPENCV_HAVE_FILESYSTEM_SUPPORT

}}} // namespace


#ਜੇ OPENCV_HAVE_FILESYSTEM_SUPPORT
#ਅਨ੍ਤਰ੍ਭਾਵ "plugin_loader.impl.hpp"
#ਜੇ_ਅੰਤ
