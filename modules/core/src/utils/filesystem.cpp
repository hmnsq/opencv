// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ಅನ್ತರ್ಭಾವ "../precomp.hpp"

#ಅನ್ತರ್ಭಾವ <opencv2/core/utils/configuration.private.hpp>

#ಅನ್ತರ್ಭಾವ <opencv2/core/utils/logger.hpp>

#ಅನ್ತರ್ಭಾವ "opencv2/core/utils/filesystem.private.hpp"
#ಅನ್ತರ್ಭಾವ "opencv2/core/utils/filesystem.hpp"

//#define DEBUG_FS_UTILS

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ DEBUG_FS_UTILS
#ಅನ್ತರ್ಭಾವ <iostream>
#ವ್ಯಾಖ್ಯಾನಿಸು DBG(...) __VA_ARGS__
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು DBG(...)
#ಆದರೆ_ಅಂತ್ಯ


#ಆದರೆ OPENCV_HAVE_FILESYSTEM_SUPPORT

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ _WIN32
#ವ್ಯಾಖ್ಯಾನಿಸು WIN32_LEAN_AND_ಸರಾಸರಿ
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು NOMINMAX
#ವ್ಯಾಖ್ಯಾನಿಸು NOMINMAX
#ಅನ್ತರ್ಭಾವ <windows.h>
#ಅನ್ತರ್ಭಾವ <direct.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಅನ್ತರ್ಭಾವ <sys/stat.h>
#ಅನ್ತರ್ಭಾವ <errno.h>
#ಅನ್ತರ್ಭಾವ <io.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅಥವಾಆದರೆ defined __linux__ || defined __APPLE__ || defined __HAIKU__ || defined __FreeBSD__ || defined __GNU__ || defined __EMSCRIPTEN__ || defined __QNX__ || defined __CYGWIN__
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಅನ್ತರ್ಭಾವ <sys/stat.h>
#ಅನ್ತರ್ಭಾವ <fcntl.h>
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಅನ್ತರ್ಭಾವ <errno.h>
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ_ಅಂತ್ಯ // OPENCV_HAVE_FILESYSTEM_SUPPORT

ನಾಮಾಕಾಶ cv { ನಾಮಾಕಾಶ utils { ನಾಮಾಕಾಶ fs {

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ _WIN32
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ native_separator = '\\';
#ಇಲ್ಲದಿದ್ದರೆ
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ native_separator = '/';
#ಆದರೆ_ಅಂತ್ಯ

ಸ್ಥಿರ ಸಾಲೊಳಗೆ
ತಾರ್ಕಿಕ is_ಮಾರ್ಗ_Separator(ಅಕ್ಷರ c)
{
    ಹಿಂತಿರುಗಿ c == '/' || c == '\\';
}

cv::String join(ಸ್ಥಿರಾಂಕ cv::String& ತಳ, ಸ್ಥಿರಾಂಕ cv::String& ಮಾರ್ಗ)
{
    ಆದರೆ (ತಳ.empty())
        ಹಿಂತಿರುಗಿ ಮಾರ್ಗ;
    ಆದರೆ (ಮಾರ್ಗ.empty())
        ಹಿಂತಿರುಗಿ ತಳ;

    ತಾರ್ಕಿಕ ತಳ_Sep = is_ಮಾರ್ಗ_Separator(ತಳ[ತಳ.size() - 1]);
    ತಾರ್ಕಿಕ ಮಾರ್ಗ_Sep = is_ಮಾರ್ಗ_Separator(ಮಾರ್ಗ[0]);
    String ಫಲಿತಾಂಶ;
    ಆದರೆ (ತಳ_Sep && ಮಾರ್ಗ_Sep)
    {
        ಫಲಿತಾಂಶ = ತಳ + ಮಾರ್ಗ.substr(1);
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!ತಳ_Sep && !ಮಾರ್ಗ_Sep)
    {
        ಫಲಿತಾಂಶ = ತಳ + native_separator + ಮಾರ್ಗ;
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        ಫಲಿತಾಂಶ = ತಳ + ಮಾರ್ಗ;
    }
    ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}

CV_EXPORTS cv::String get_ಪೋಷಕ(ಸ್ಥಿರಾಂಕ cv::String &ಮಾರ್ಗ)
{
    std::string::size_type loc = ಮಾರ್ಗ.find_last_of("/\\");
    ಆದರೆ (loc == std::string::npos)
        ಹಿಂತಿರುಗಿ std::string();
    ಹಿಂತಿರುಗಿ std::string(ಮಾರ್ಗ, 0, loc);
}

CV_EXPORTS std::wstring get_ಪೋಷಕ(ಸ್ಥಿರಾಂಕ std::wstring& ಮಾರ್ಗ)
{
    std::wstring::size_type loc = ಮಾರ್ಗ.find_last_of(L"/\\");
    ಆದರೆ (loc == std::wstring::npos)
        ಹಿಂತಿರುಗಿ std::wstring();
    ಹಿಂತಿರುಗಿ std::wstring(ಮಾರ್ಗ, 0, loc);
}

#ಆದರೆ OPENCV_HAVE_FILESYSTEM_SUPPORT

cv::String canonical(ಸ್ಥಿರಾಂಕ cv::String& ಮಾರ್ಗ)
{
    cv::String ಫಲಿತಾಂಶ;
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ _WIN32
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* ಫಲಿತಾಂಶ_str = _fullpath(ಖಾಲಿ, ಮಾರ್ಗ.c_str(), 0);
#ಇಲ್ಲದಿದ್ದರೆ
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* ಫಲಿತಾಂಶ_str = realpath(ಮಾರ್ಗ.c_str(), ಖಾಲಿ);
#ಆದರೆ_ಅಂತ್ಯ
    ಆದರೆ (ಫಲಿತಾಂಶ_str)
    {
        ಫಲಿತಾಂಶ = cv::String(ಫಲಿತಾಂಶ_str);
        ಬಿಡುಗಡೆ((ಶೂನ್ಯ*)ಫಲಿತಾಂಶ_str);
    }
    ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ.empty() ? ಮಾರ್ಗ : ಫಲಿತಾಂಶ;
}


ತಾರ್ಕಿಕ exists(ಸ್ಥಿರಾಂಕ cv::String& ಮಾರ್ಗ)
{
    CV_INSTRUMENT_REGION();

#ಆದರೆ defined _WIN32 || defined WINCE
    BOOL ಸ್ಥಿತಿ_2 = TRUE;
    {
        WIN32_FILE_ATTRIBUTE_ದತ್ತಾಂಶ all_attrs;
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ WINRT
        wchar_t wpath[MAX_ಮಾರ್ಗ];
        size_t copied = mbstowcs(wpath, ಮಾರ್ಗ.c_str(), MAX_ಮಾರ್ಗ);
        CV_Assert((copied != MAX_ಮಾರ್ಗ) && (copied != (size_t)-1));
        ಸ್ಥಿತಿ_2 = ::Get_File_ಗುಣಲಕ್ಷಣಗಳು_Ex_W(wpath, GetFileExInfoStandard, &all_attrs);
#ಇಲ್ಲದಿದ್ದರೆ
        ಸ್ಥಿತಿ_2 = ::Get_File_ಗುಣಲಕ್ಷಣಗಳು_Ex_A(ಮಾರ್ಗ.c_str(), GetFileExInfoStandard, &all_attrs);
#ಆದರೆ_ಅಂತ್ಯ
    }

    ಹಿಂತಿರುಗಿ !!ಸ್ಥಿತಿ_2;
#ಇಲ್ಲದಿದ್ದರೆ
    ರಚನೆ stat stat_buf;
    ಹಿಂತಿರುಗಿ (0 == stat(ಮಾರ್ಗ.c_str(), &stat_buf));
#ಆದರೆ_ಅಂತ್ಯ
}

CV_EXPORTS ಶೂನ್ಯ remove_all(ಸ್ಥಿರಾಂಕ cv::String& ಮಾರ್ಗ)
{
    ಆದರೆ (!exists(ಮಾರ್ಗ))
        ಹಿಂತಿರುಗಿ;
    ಆದರೆ (isDirectory(ಮಾರ್ಗ))
    {
        std::vector<String> entries;
        utils::fs::glob(ಮಾರ್ಗ, cv::String(), entries, ಸುಳ್ಳು, ನಿಜ);
        ಗಾಗಿ (size_t i = 0; i < entries.size(); i++)
        {
            ಸ್ಥಿರಾಂಕ String& e = entries[i];
            remove_all(e);
        }
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ _MSC_VER
        ತಾರ್ಕಿಕ ಫಲಿತಾಂಶ = _rmdir(ಮಾರ್ಗ.c_str()) == 0;
#ಇಲ್ಲದಿದ್ದರೆ
        ತಾರ್ಕಿಕ ಫಲಿತಾಂಶ = rmdir(ಮಾರ್ಗ.c_str()) == 0;
#ಆದರೆ_ಅಂತ್ಯ
        ಆದರೆ (!ಫಲಿತಾಂಶ)
        {
            CV_LOG_ERROR(ಖಾಲಿ, "Can't remove directory: " << ಮಾರ್ಗ);
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ _MSC_VER
        ತಾರ್ಕಿಕ ಫಲಿತಾಂಶ = _unlink(ಮಾರ್ಗ.c_str()) == 0;
#ಇಲ್ಲದಿದ್ದರೆ
        ತಾರ್ಕಿಕ ಫಲಿತಾಂಶ = unlink(ಮಾರ್ಗ.c_str()) == 0;
#ಆದರೆ_ಅಂತ್ಯ
        ಆದರೆ (!ಫಲಿತಾಂಶ)
        {
            CV_LOG_ERROR(ಖಾಲಿ, "Can't remove file: " << ಮಾರ್ಗ);
        }
    }
}


cv::String getcwd()
{
    CV_INSTRUMENT_REGION();
    cv::Auto_ಬಫರ್<ಅಕ್ಷರ, 4096> buf;
#ಆದರೆ defined WIN32 || defined _WIN32 || defined WINCE
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ WINRT
    ಹಿಂತಿರುಗಿ cv::String();
#ಇಲ್ಲದಿದ್ದರೆ
    DWORD sz = GetCurrentDirectoryA(0, ಖಾಲಿ);
    buf.allocate((size_t)sz);
    sz = GetCurrentDirectoryA((DWORD)buf.size(), buf.ದತ್ತಾಂಶ());
    ಹಿಂತಿರುಗಿ cv::String(buf.ದತ್ತಾಂಶ(), (size_t)sz);
#ಆದರೆ_ಅಂತ್ಯ
#ಅಥವಾಆದರೆ defined __linux__ || defined __APPLE__ || defined __HAIKU__ || defined __FreeBSD__ || defined __EMSCRIPTEN__ || defined __QNX__ || defined __CYGWIN__
    ಗಾಗಿ(;;)
    {
        ಅಕ್ಷರ* p = ::getcwd(buf.ದತ್ತಾಂಶ(), buf.size());
        ಆದರೆ (p == ಖಾಲಿ)
        {
            ಆದರೆ (errno == ERANGE)
            {
                buf.allocate(buf.size() * 2);
                ಮುಂದುವರಿ;
            }
            ಹಿಂತಿರುಗಿ cv::String();
        }
        ನಿಲ್ಲಿಸು;
    }
    ಹಿಂತಿರುಗಿ cv::String(buf.ದತ್ತಾಂಶ(), (size_t)ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(buf.ದತ್ತಾಂಶ()));
#ಇಲ್ಲದಿದ್ದರೆ
    ಹಿಂತಿರುಗಿ cv::String();
#ಆದರೆ_ಅಂತ್ಯ
}


ತಾರ್ಕಿಕ createDirectory(ಸ್ಥಿರಾಂಕ cv::String& ಮಾರ್ಗ)
{
    CV_INSTRUMENT_REGION();
#ಆದರೆ defined WIN32 || defined _WIN32 || defined WINCE
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ WINRT
    wchar_t wpath[MAX_ಮಾರ್ಗ];
    size_t copied = mbstowcs(wpath, ಮಾರ್ಗ.c_str(), MAX_ಮಾರ್ಗ);
    CV_Assert((copied != MAX_ಮಾರ್ಗ) && (copied != (size_t)-1));
    ಪೂರ್ಣಾಂಕ ಫಲಿತಾಂಶ = CreateDirectoryA(wpath, ಖಾಲಿ) ? 0 : -1;
#ಇಲ್ಲದಿದ್ದರೆ
    ಪೂರ್ಣಾಂಕ ಫಲಿತಾಂಶ = _mkdir(ಮಾರ್ಗ.c_str());
#ಆದರೆ_ಅಂತ್ಯ
#ಅಥವಾಆದರೆ defined __linux__ || defined __APPLE__ || defined __HAIKU__ || defined __FreeBSD__ || defined __EMSCRIPTEN__ || defined __QNX__ || defined __CYGWIN__
    ಪೂರ್ಣಾಂಕ ಫಲಿತಾಂಶ = mkdir(ಮಾರ್ಗ.c_str(), 0777);
#ಇಲ್ಲದಿದ್ದರೆ
    ಪೂರ್ಣಾಂಕ ಫಲಿತಾಂಶ = -1;
#ಆದರೆ_ಅಂತ್ಯ

    ಆದರೆ (ಫಲಿತಾಂಶ == -1)
    {
        ಹಿಂತಿರುಗಿ isDirectory(ಮಾರ್ಗ);
    }
    ಹಿಂತಿರುಗಿ ನಿಜ;
}

ತಾರ್ಕಿಕ createDirectories(ಸ್ಥಿರಾಂಕ cv::String& ಮಾರ್ಗ_3)
{
    cv::String ಮಾರ್ಗ = ಮಾರ್ಗ_3;
    ಗಾಗಿ (;;)
    {
        ಅಕ್ಷರ ಕೊನೆಯ_char = ಮಾರ್ಗ.empty() ? 0 : ಮಾರ್ಗ[ಮಾರ್ಗ.length() - 1];
        ಆದರೆ (is_ಮಾರ್ಗ_Separator(ಕೊನೆಯ_char))
        {
            ಮಾರ್ಗ = ಮಾರ್ಗ.substr(0, ಮಾರ್ಗ.length() - 1);
            ಮುಂದುವರಿ;
        }
        ನಿಲ್ಲಿಸು;
    }

    ಆದರೆ (ಮಾರ್ಗ.empty() || ಮಾರ್ಗ == "./" || ಮಾರ್ಗ == ".\\" || ಮಾರ್ಗ == ".")
        ಹಿಂತಿರುಗಿ ನಿಜ;
    ಆದರೆ (isDirectory(ಮಾರ್ಗ))
        ಹಿಂತಿರುಗಿ ನಿಜ;

    size_t pos = ಮಾರ್ಗ.rfind('/');
    ಆದರೆ (pos == cv::String::npos)
        pos = ಮಾರ್ಗ.rfind('\\');
    ಆದರೆ (pos != cv::String::npos)
    {
        cv::String ಪೋಷಕ_directory = ಮಾರ್ಗ.substr(0, pos);
        ಆದರೆ (!ಪೋಷಕ_directory.empty())
        {
            ಆದರೆ (!createDirectories(ಪೋಷಕ_directory))
                ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
        }
    }

    ಹಿಂತಿರುಗಿ createDirectory(ಮಾರ್ಗ);
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ _WIN32

ರಚನೆ File_ಬೀಗ::Impl
{
    Impl(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* fname)
    {
        // http://support.microsoft.com/kb/316609
        ಪೂರ್ಣಾಂಕ ಅಂಕೆ_Retries = 5;
        ಮಾಡು
        {
            handle = ::CreateFileA(fname, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, ಖಾಲಿ,
                                OPEN_EXISTING, FILE_ATTRIBUTE_ಸಾಮಾನ್ಯ, ಖಾಲಿ);
            ಆದರೆ (INVALID_HANDLE_ಮೌಲ್ಯ == handle)
            {
                ಆದರೆ (ERROR_SHARING_VIOLATION == Get_ಕೊನೆಯ_Error())
                {
                    ಅಂಕೆ_Retries--;
                    Sleep(250);
                    ಮುಂದುವರಿ;
                }
                ಇಲ್ಲದಿದ್ದರೆ
                {
                    CV_Error_(Error::StsAssert, ("Can't open lock file: %s", fname));
                }
            }
            ನಿಲ್ಲಿಸು;
        } ಆಗ (ಅಂಕೆ_Retries > 0);
    }
    ~Impl()
    {
        ಆದರೆ (INVALID_HANDLE_ಮೌಲ್ಯ != handle)
        {
            ::CloseHandle(handle);
        }
    }

    ತಾರ್ಕಿಕ lock()
    {
        OVERLAPPED overlapped;
        std::memset(&overlapped, 0, ಗಾತ್ರ(overlapped));
        ಹಿಂತಿರುಗಿ !!::ಬೀಗ_File_Ex(handle, LOCKFILE_EXCLUSIVE_ಬೀಗ, 0, MAXDWORD, MAXDWORD, &overlapped);
    }
    ತಾರ್ಕಿಕ unlock()
    {
        OVERLAPPED overlapped;
        std::memset(&overlapped, 0, ಗಾತ್ರ(overlapped));
        ಹಿಂತಿರುಗಿ !!::UnlockFileEx(handle, 0, MAXDWORD, MAXDWORD, &overlapped);
    }

    ತಾರ್ಕಿಕ ಬೀಗ_shared()
    {
        OVERLAPPED overlapped;
        std::memset(&overlapped, 0, ಗಾತ್ರ(overlapped));
        ಹಿಂತಿರುಗಿ !!::ಬೀಗ_File_Ex(handle, 0, 0, MAXDWORD, MAXDWORD, &overlapped);
    }
    ತಾರ್ಕಿಕ unlock_shared()
    {
        ಹಿಂತಿರುಗಿ unlock();
    }

    HANDLE handle;

ಖಾಸಗಿ:
    Impl(ಸ್ಥಿರಾಂಕ Impl&); // disabled
    Impl& ಕ್ರಿಯಾಕಾರಕ=(ಸ್ಥಿರಾಂಕ Impl&); // disabled
};

#ಅಥವಾಆದರೆ defined __linux__ || defined __APPLE__ || defined __HAIKU__ || defined __FreeBSD__ || defined __GNU__ || defined __EMSCRIPTEN__ || defined __QNX__ || defined __CYGWIN__

ರಚನೆ File_ಬೀಗ::Impl
{
    Impl(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* fname)
    {
        handle = ::open(fname, O_RDWR);
        CV_Assert(handle != -1);
    }
    ~Impl()
    {
        ಆದರೆ (handle >= 0)
            ::close(handle);
    }

    ತಾರ್ಕಿಕ lock()
    {
        ರಚನೆ ::flock l;
        std::memset(&l, 0, ಗಾತ್ರ(l));
        l.l_type = F_WRLCK;
        l.l_whence = SEEK_SET;
        l.l_ಆರಂಭ = 0;
        l.l_len = 0;
        DBG(std::ಹೊರಗೆ << "Lock..." << std::ಸಾಲು_ಅಂತ್ಯ);
        ತಾರ್ಕಿಕ res = -1 != ::fcntl(handle, F_SETLKW, &l);
        ಹಿಂತಿರುಗಿ res;
    }
    ತಾರ್ಕಿಕ unlock()
    {
        ರಚನೆ ::flock l;
        std::memset(&l, 0, ಗಾತ್ರ(l));
        l.l_type = F_UNLCK;
        l.l_whence = SEEK_SET;
        l.l_ಆರಂಭ = 0;
        l.l_len = 0;
        DBG(std::ಹೊರಗೆ << "Unlock..." << std::ಸಾಲು_ಅಂತ್ಯ);
        ತಾರ್ಕಿಕ res = -1 != ::fcntl(handle, F_SETLK, &l);
        ಹಿಂತಿರುಗಿ res;
    }

    ತಾರ್ಕಿಕ ಬೀಗ_shared()
    {
        ರಚನೆ ::flock l;
        std::memset(&l, 0, ಗಾತ್ರ(l));
        l.l_type = F_RDLCK;
        l.l_whence = SEEK_SET;
        l.l_ಆರಂಭ = 0;
        l.l_len = 0;
        DBG(std::ಹೊರಗೆ << "Lock read..." << std::ಸಾಲು_ಅಂತ್ಯ);
        ತಾರ್ಕಿಕ res = -1 != ::fcntl(handle, F_SETLKW, &l);
        ಹಿಂತಿರುಗಿ res;
    }
    ತಾರ್ಕಿಕ unlock_shared()
    {
        ಹಿಂತಿರುಗಿ unlock();
    }

    ಪೂರ್ಣಾಂಕ handle;

ಖಾಸಗಿ:
    Impl(ಸ್ಥಿರಾಂಕ Impl&); // disabled
    Impl& ಕ್ರಿಯಾಕಾರಕ=(ಸ್ಥಿರಾಂಕ Impl&); // disabled
};

#ಆದರೆ_ಅಂತ್ಯ

File_ಬೀಗ::File_ಬೀಗ(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* fname)
    : pImpl(ಹೊಸ Impl(fname))
{
    // nothing
}
File_ಬೀಗ::~File_ಬೀಗ()
{
    ಅಳಿಸು pImpl;
    pImpl = ಖಾಲಿ;
}

ಶೂನ್ಯ File_ಬೀಗ::lock() { CV_Assert(pImpl->lock()); }
ಶೂನ್ಯ File_ಬೀಗ::unlock() { CV_Assert(pImpl->unlock()); }
ಶೂನ್ಯ File_ಬೀಗ::ಬೀಗ_shared() { CV_Assert(pImpl->ಬೀಗ_shared()); }
ಶೂನ್ಯ File_ಬೀಗ::unlock_shared() { CV_Assert(pImpl->unlock_shared()); }



cv::String getCacheDirectory(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* sub_directory_name, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* configuration_name)
{
    String cache_ಮಾರ್ಗ_2;
    ಆದರೆ (configuration_name)
    {
        cache_ಮಾರ್ಗ_2 = utils::getConfigurationParameterString(configuration_name, "");
    }
    ಆದರೆ (cache_ಮಾರ್ಗ_2.empty())
    {
        cv::String default_cache_ಮಾರ್ಗ;
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ _WIN32
        wchar_t tmp_ಮಾರ್ಗ_buf_w[MAX_ಮಾರ್ಗ+1] = {0};
        DWORD res = Get_ತಾತ್ಕಾಲಿಕ_ಮಾರ್ಗ_W(MAX_ಮಾರ್ಗ, tmp_ಮಾರ್ಗ_buf_w);
        ಆದರೆ (res > 0 && res <= MAX_ಮಾರ್ಗ)
        {
            // Convert from UTF-16 to UTF-8 to support Unicode paths
            ಪೂರ್ಣಾಂಕ len = Wide_Char_ಗಮ್ಯ_Multi_Byte(CP_UTF8, 0, tmp_ಮಾರ್ಗ_buf_w, -1, ಖಾಲಿ, 0, ಖಾಲಿ, ಖಾಲಿ);
            ಆದರೆ (len > 0)
            {
                std::vector<ಅಕ್ಷರ> utf8_buf(len);
                Wide_Char_ಗಮ್ಯ_Multi_Byte(CP_UTF8, 0, tmp_ಮಾರ್ಗ_buf_w, -1, utf8_buf.ದತ್ತಾಂಶ(), len, ಖಾಲಿ, ಖಾಲಿ);
                default_cache_ಮಾರ್ಗ = std::string(utf8_buf.ದತ್ತಾಂಶ());
            }
        }
#ಅಥವಾಆದರೆ defined __ANDROID__
        // no defaults
#ಅಥವಾಆದರೆ defined __APPLE__
        ಸ್ಥಿರಾಂಕ std::string tmpdir_env = utils::getConfigurationParameterString("TMPDIR");
        ಆದರೆ (!tmpdir_env.empty() && utils::fs::isDirectory(tmpdir_env))
        {
            default_cache_ಮಾರ್ಗ = tmpdir_env;
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            default_cache_ಮಾರ್ಗ = "/tmp/";
            CV_LOG_WARNING(ಖಾಲಿ, "Using world accessible cache directory. This may be not secure: " << default_cache_ಮಾರ್ಗ);
        }
#ಅಥವಾಆದರೆ defined __linux__ || defined __HAIKU__ || defined __FreeBSD__ || defined __GNU__
        // https://specifications.freedesktop.org/basedir-spec/basedir-spec-latest.html
        ಆದರೆ (default_cache_ಮಾರ್ಗ.empty())
        {
            ಸ್ಥಿರಾಂಕ std::string xdg_cache_env = utils::getConfigurationParameterString("XDG_CACHE_HOME");
            ಆದರೆ (!xdg_cache_env.empty() && utils::fs::isDirectory(xdg_cache_env))
            {
                default_cache_ಮಾರ್ಗ = xdg_cache_env;
            }
        }
        ಆದರೆ (default_cache_ಮಾರ್ಗ.empty())
        {
            ಸ್ಥಿರಾಂಕ std::string home_env = utils::getConfigurationParameterString("HOME");
            ಆದರೆ (!home_env.empty() && utils::fs::isDirectory(home_env))
            {
                cv::String home_ಮಾರ್ಗ = home_env;
                cv::String home_cache_ಮಾರ್ಗ = utils::fs::join(home_ಮಾರ್ಗ, ".cache/");
                ಆದರೆ (utils::fs::isDirectory(home_cache_ಮಾರ್ಗ))
                {
                    default_cache_ಮಾರ್ಗ = home_cache_ಮಾರ್ಗ;
                }
            }
        }
        ಆದರೆ (default_cache_ಮಾರ್ಗ.empty())
        {
            ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* ತಾತ್ಕಾಲಿಕ_ಮಾರ್ಗ = "/var/tmp/";
            ಆದರೆ (utils::fs::isDirectory(ತಾತ್ಕಾಲಿಕ_ಮಾರ್ಗ))
            {
                default_cache_ಮಾರ್ಗ = ತಾತ್ಕಾಲಿಕ_ಮಾರ್ಗ;
                CV_LOG_WARNING(ಖಾಲಿ, "Using world accessible cache directory. This may be not secure: " << default_cache_ಮಾರ್ಗ);
            }
        }
        ಆದರೆ (default_cache_ಮಾರ್ಗ.empty())
        {
            default_cache_ಮಾರ್ಗ = "/tmp/";
            CV_LOG_WARNING(ಖಾಲಿ, "Using world accessible cache directory. This may be not secure: " << default_cache_ಮಾರ್ಗ);
        }
#ಇಲ್ಲದಿದ್ದರೆ
        // no defaults
#ಆದರೆ_ಅಂತ್ಯ
        CV_LOG_VERBOSE(ಖಾಲಿ, 0, "default_cache_path = " << default_cache_ಮಾರ್ಗ);
        ಆದರೆ (!default_cache_ಮಾರ್ಗ.empty())
        {
            ಆದರೆ (utils::fs::isDirectory(default_cache_ಮಾರ್ಗ))
            {
                cv::String default_cache_ಮಾರ್ಗ_ತಳ = utils::fs::join(default_cache_ಮಾರ್ಗ, "opencv");
                default_cache_ಮಾರ್ಗ = utils::fs::join(default_cache_ಮಾರ್ಗ_ತಳ, CVAUX_STR(CV_ಆವೃತ್ತಿ_MAJOR) ".x" CV_ಆವೃತ್ತಿ_ಸ್ಥಿತಿ);
                ಆದರೆ (utils::getConfigurationParameterBool("OPENCV_CACHE_SHOW_CLEANUP_MESSAGE", ನಿಜ)
                    && !utils::fs::isDirectory(default_cache_ಮಾರ್ಗ))
                {
                    std::vector<cv::String> existedCacheDirs;
                    ಪ್ರಯತ್ನಿಸು
                    {
                        utils::fs::glob_relative(default_cache_ಮಾರ್ಗ_ತಳ, "*", existedCacheDirs, ಸುಳ್ಳು, ನಿಜ);
                    }
                    ಹಿಡಿ (...)
                    {
                        // ignore
                    }
                    ಆದರೆ (!existedCacheDirs.empty())
                    {
                        CV_LOG_WARNING(ಖಾಲಿ, "Creating new OpenCV cache directory: " << default_cache_ಮಾರ್ಗ);
                        CV_LOG_WARNING(ಖಾಲಿ, "There are several neighbour directories, probably created by old OpenCV versions.");
                        CV_LOG_WARNING(ಖಾಲಿ, "Feel free to cleanup these unused directories:");
                        ಗಾಗಿ (size_t i = 0; i < existedCacheDirs.size(); i++)
                        {
                            CV_LOG_WARNING(ಖಾಲಿ, "  - " << existedCacheDirs[i]);
                        }
                        CV_LOG_WARNING(ಖಾಲಿ, "Note: This message is showed only once.");
                    }
                }
                ಆದರೆ (sub_directory_name && sub_directory_name[0] != '\0')
                    default_cache_ಮಾರ್ಗ = utils::fs::join(default_cache_ಮಾರ್ಗ, cv::String(sub_directory_name) + native_separator);
                ಆದರೆ (!utils::fs::createDirectories(default_cache_ಮಾರ್ಗ))
                {
                    CV_LOG_DEBUG(ಖಾಲಿ, "Can't create OpenCV cache sub-directory: " << default_cache_ಮಾರ್ಗ);
                }
                ಇಲ್ಲದಿದ್ದರೆ
                {
                    cache_ಮಾರ್ಗ_2 = default_cache_ಮಾರ್ಗ;
                }
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                CV_LOG_INFO(ಖಾಲಿ, "Can't find default cache directory (does it exist?): " << default_cache_ಮಾರ್ಗ);
            }
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            CV_LOG_DEBUG(ಖಾಲಿ, "OpenCV has no support to discover default cache directory on the current platform");
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        ಆದರೆ (cache_ಮಾರ್ಗ_2 == "disabled")
            ಹಿಂತಿರುಗಿ cache_ಮಾರ್ಗ_2;
        ಆದರೆ (!isDirectory(cache_ಮಾರ್ಗ_2))
        {
            CV_LOG_WARNING(ಖಾಲಿ, "Specified non-existed directory, creating OpenCV sub-directory for caching purposes: " << cache_ಮಾರ್ಗ_2);
            ಆದರೆ (!createDirectories(cache_ಮಾರ್ಗ_2))
            {
                CV_LOG_ERROR(ಖಾಲಿ, "Can't create OpenCV cache sub-directory: " << cache_ಮಾರ್ಗ_2);
                cache_ಮಾರ್ಗ_2.clear();
            }
        }
    }
    CV_Assert(cache_ಮಾರ್ಗ_2.empty() || utils::fs::isDirectory(cache_ಮಾರ್ಗ_2));
    ಆದರೆ (!cache_ಮಾರ್ಗ_2.empty())
    {
        ಆದರೆ (!is_ಮಾರ್ಗ_Separator(cache_ಮಾರ್ಗ_2[cache_ಮಾರ್ಗ_2.size() - 1]))
        {
            cache_ಮಾರ್ಗ_2 += native_separator;
        }
    }
    ಹಿಂತಿರುಗಿ cache_ಮಾರ್ಗ_2;
}

#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು NOT_IMPLEMENTED CV_Error(Error::StsNotImplemented, "");
cv::String canonical(ಸ್ಥಿರಾಂಕ cv::String& /*ಮಾರ್ಗ*/) { NOT_IMPLEMENTED }
ತಾರ್ಕಿಕ exists(ಸ್ಥಿರಾಂಕ cv::String& /*ಮಾರ್ಗ*/) { NOT_IMPLEMENTED }
ಶೂನ್ಯ remove_all(ಸ್ಥಿರಾಂಕ cv::String& /*ಮಾರ್ಗ*/) { NOT_IMPLEMENTED }
cv::String getcwd() { NOT_IMPLEMENTED }
ತಾರ್ಕಿಕ createDirectory(ಸ್ಥಿರಾಂಕ cv::String& /*ಮಾರ್ಗ*/) { NOT_IMPLEMENTED }
ತಾರ್ಕಿಕ createDirectories(ಸ್ಥಿರಾಂಕ cv::String& /*ಮಾರ್ಗ*/) { NOT_IMPLEMENTED }
cv::String getCacheDirectory(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* /*sub_directory_name*/, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* /*configuration_name = NULL*/) { NOT_IMPLEMENTED }
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು NOT_IMPLEMENTED
#ಆದರೆ_ಅಂತ್ಯ // OPENCV_HAVE_FILESYSTEM_SUPPORT

}}} // namespace


#ಆದರೆ OPENCV_HAVE_FILESYSTEM_SUPPORT
#ಅನ್ತರ್ಭಾವ "plugin_loader.impl.hpp"
#ಆದರೆ_ಅಂತ್ಯ
