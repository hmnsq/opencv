// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#شامل "../precomp.hpp"

#شامل <opencv2/core/utils/configuration.private.hpp>

#شامل <opencv2/core/utils/logger.hpp>

#شامل "opencv2/core/utils/filesystem.private.hpp"
#شامل "opencv2/core/utils/filesystem.hpp"

//#define DEBUG_FS_UTILS

#اگر_تعریف_شدہ DEBUG_FS_UTILS
#شامل <iostream>
#تعریف_کرو DBG(...) __VA_ARGS__
#ورنہ
#تعریف_کرو DBG(...)
#اگر_ختم


#اگر OPENCV_HAVE_FILESYSTEM_SUPPORT

#اگر_تعریف_شدہ _WIN32
#تعریف_کرو WIN32_LEAN_AND_اوسط
#تعریف_منسوخ NOMINMAX
#تعریف_کرو NOMINMAX
#شامل <windows.h>
#شامل <direct.h>
#شامل <sys/types.h>
#شامل <sys/stat.h>
#شامل <errno.h>
#شامل <io.h>
#شامل <stdio.h>
#وگرنہ_اگر defined __linux__ || defined __APPLE__ || defined __HAIKU__ || defined __FreeBSD__ || defined __GNU__ || defined __EMSCRIPTEN__ || defined __QNX__ || defined __CYGWIN__
#شامل <sys/types.h>
#شامل <sys/stat.h>
#شامل <fcntl.h>
#شامل <unistd.h>
#شامل <errno.h>
#اگر_ختم

#اگر_ختم // OPENCV_HAVE_FILESYSTEM_SUPPORT

نامخلا cv { نامخلا utils { نامخلا fs {

#اگر_تعریف_شدہ _WIN32
ساکن ثابت حرف native_separator = '\\';
#ورنہ
ساکن ثابت حرف native_separator = '/';
#اگر_ختم

ساکن اندرونی
منطقی is_راستہ_Separator(حرف c)
{
    واپس c == '/' || c == '\\';
}

cv::String join(ثابت cv::String& بنیاد, ثابت cv::String& راستہ)
{
    اگر (بنیاد.empty())
        واپس راستہ;
    اگر (راستہ.empty())
        واپس بنیاد;

    منطقی بنیاد_Sep = is_راستہ_Separator(بنیاد[بنیاد.size() - 1]);
    منطقی راستہ_Sep = is_راستہ_Separator(راستہ[0]);
    String نتیجہ;
    اگر (بنیاد_Sep && راستہ_Sep)
    {
        نتیجہ = بنیاد + راستہ.substr(1);
    }
    ورنہ اگر (!بنیاد_Sep && !راستہ_Sep)
    {
        نتیجہ = بنیاد + native_separator + راستہ;
    }
    ورنہ
    {
        نتیجہ = بنیاد + راستہ;
    }
    واپس نتیجہ;
}

CV_EXPORTS cv::String get_والد(ثابت cv::String &راستہ)
{
    std::string::size_type loc = راستہ.find_last_of("/\\");
    اگر (loc == std::string::npos)
        واپس std::string();
    واپس std::string(راستہ, 0, loc);
}

CV_EXPORTS std::wstring get_والد(ثابت std::wstring& راستہ)
{
    std::wstring::size_type loc = راستہ.find_last_of(L"/\\");
    اگر (loc == std::wstring::npos)
        واپس std::wstring();
    واپس std::wstring(راستہ, 0, loc);
}

#اگر OPENCV_HAVE_FILESYSTEM_SUPPORT

cv::String canonical(ثابت cv::String& راستہ)
{
    cv::String نتیجہ;
#اگر_تعریف_شدہ _WIN32
    ثابت حرف* نتیجہ_str = _fullpath(تہی, راستہ.c_str(), 0);
#ورنہ
    ثابت حرف* نتیجہ_str = realpath(راستہ.c_str(), تہی);
#اگر_ختم
    اگر (نتیجہ_str)
    {
        نتیجہ = cv::String(نتیجہ_str);
        آزاد((خالی*)نتیجہ_str);
    }
    واپس نتیجہ.empty() ? راستہ : نتیجہ;
}


منطقی exists(ثابت cv::String& راستہ)
{
    CV_INSTRUMENT_REGION();

#اگر defined _WIN32 || defined WINCE
    BOOL حالت_2 = TRUE;
    {
        WIN32_FILE_ATTRIBUTE_ڈیٹا all_attrs;
#اگر_تعریف_شدہ WINRT
        wchar_t wpath[MAX_راستہ];
        size_t copied = mbstowcs(wpath, راستہ.c_str(), MAX_راستہ);
        CV_Assert((copied != MAX_راستہ) && (copied != (size_t)-1));
        حالت_2 = ::Get_File_اوصاف_Ex_W(wpath, GetFileExInfoStandard, &all_attrs);
#ورنہ
        حالت_2 = ::Get_File_اوصاف_Ex_A(راستہ.c_str(), GetFileExInfoStandard, &all_attrs);
#اگر_ختم
    }

    واپس !!حالت_2;
#ورنہ
    ساخت stat stat_buf;
    واپس (0 == stat(راستہ.c_str(), &stat_buf));
#اگر_ختم
}

CV_EXPORTS خالی remove_all(ثابت cv::String& راستہ)
{
    اگر (!exists(راستہ))
        واپس;
    اگر (isDirectory(راستہ))
    {
        std::vector<String> entries;
        utils::fs::glob(راستہ, cv::String(), entries, جھوٹ, سچ);
        برائے (size_t i = 0; i < entries.size(); i++)
        {
            ثابت String& e = entries[i];
            remove_all(e);
        }
#اگر_تعریف_شدہ _MSC_VER
        منطقی نتیجہ = _rmdir(راستہ.c_str()) == 0;
#ورنہ
        منطقی نتیجہ = rmdir(راستہ.c_str()) == 0;
#اگر_ختم
        اگر (!نتیجہ)
        {
            CV_LOG_ERROR(تہی, "Can't remove directory: " << راستہ);
        }
    }
    ورنہ
    {
#اگر_تعریف_شدہ _MSC_VER
        منطقی نتیجہ = _unlink(راستہ.c_str()) == 0;
#ورنہ
        منطقی نتیجہ = unlink(راستہ.c_str()) == 0;
#اگر_ختم
        اگر (!نتیجہ)
        {
            CV_LOG_ERROR(تہی, "Can't remove file: " << راستہ);
        }
    }
}


cv::String getcwd()
{
    CV_INSTRUMENT_REGION();
    cv::Auto_بفر<حرف, 4096> buf;
#اگر defined WIN32 || defined _WIN32 || defined WINCE
#اگر_تعریف_شدہ WINRT
    واپس cv::String();
#ورنہ
    DWORD sz = GetCurrentDirectoryA(0, تہی);
    buf.allocate((size_t)sz);
    sz = GetCurrentDirectoryA((DWORD)buf.size(), buf.ڈیٹا());
    واپس cv::String(buf.ڈیٹا(), (size_t)sz);
#اگر_ختم
#وگرنہ_اگر defined __linux__ || defined __APPLE__ || defined __HAIKU__ || defined __FreeBSD__ || defined __EMSCRIPTEN__ || defined __QNX__ || defined __CYGWIN__
    برائے(;;)
    {
        حرف* p = ::getcwd(buf.ڈیٹا(), buf.size());
        اگر (p == تہی)
        {
            اگر (errno == ERANGE)
            {
                buf.allocate(buf.size() * 2);
                جاری;
            }
            واپس cv::String();
        }
        توڑو;
    }
    واپس cv::String(buf.ڈیٹا(), (size_t)لڑی_لمبائی(buf.ڈیٹا()));
#ورنہ
    واپس cv::String();
#اگر_ختم
}


منطقی createDirectory(ثابت cv::String& راستہ)
{
    CV_INSTRUMENT_REGION();
#اگر defined WIN32 || defined _WIN32 || defined WINCE
#اگر_تعریف_شدہ WINRT
    wchar_t wpath[MAX_راستہ];
    size_t copied = mbstowcs(wpath, راستہ.c_str(), MAX_راستہ);
    CV_Assert((copied != MAX_راستہ) && (copied != (size_t)-1));
    صحیح نتیجہ = CreateDirectoryA(wpath, تہی) ? 0 : -1;
#ورنہ
    صحیح نتیجہ = _mkdir(راستہ.c_str());
#اگر_ختم
#وگرنہ_اگر defined __linux__ || defined __APPLE__ || defined __HAIKU__ || defined __FreeBSD__ || defined __EMSCRIPTEN__ || defined __QNX__ || defined __CYGWIN__
    صحیح نتیجہ = mkdir(راستہ.c_str(), 0777);
#ورنہ
    صحیح نتیجہ = -1;
#اگر_ختم

    اگر (نتیجہ == -1)
    {
        واپس isDirectory(راستہ);
    }
    واپس سچ;
}

منطقی createDirectories(ثابت cv::String& راستہ_3)
{
    cv::String راستہ = راستہ_3;
    برائے (;;)
    {
        حرف آخری_char = راستہ.empty() ? 0 : راستہ[راستہ.length() - 1];
        اگر (is_راستہ_Separator(آخری_char))
        {
            راستہ = راستہ.substr(0, راستہ.length() - 1);
            جاری;
        }
        توڑو;
    }

    اگر (راستہ.empty() || راستہ == "./" || راستہ == ".\\" || راستہ == ".")
        واپس سچ;
    اگر (isDirectory(راستہ))
        واپس سچ;

    size_t pos = راستہ.rfind('/');
    اگر (pos == cv::String::npos)
        pos = راستہ.rfind('\\');
    اگر (pos != cv::String::npos)
    {
        cv::String والد_directory = راستہ.substr(0, pos);
        اگر (!والد_directory.empty())
        {
            اگر (!createDirectories(والد_directory))
                واپس جھوٹ;
        }
    }

    واپس createDirectory(راستہ);
}

#اگر_تعریف_شدہ _WIN32

ساخت File_تالا::Impl
{
    Impl(ثابت حرف* fname)
    {
        // http://support.microsoft.com/kb/316609
        صحیح عدد_Retries = 5;
        کرو
        {
            handle = ::CreateFileA(fname, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, تہی,
                                OPEN_EXISTING, FILE_ATTRIBUTE_معمول, تہی);
            اگر (INVALID_HANDLE_قدر == handle)
            {
                اگر (ERROR_SHARING_VIOLATION == Get_آخری_Error())
                {
                    عدد_Retries--;
                    Sleep(250);
                    جاری;
                }
                ورنہ
                {
                    CV_Error_(Error::StsAssert, ("Can't open lock file: %s", fname));
                }
            }
            توڑو;
        } جب (عدد_Retries > 0);
    }
    ~Impl()
    {
        اگر (INVALID_HANDLE_قدر != handle)
        {
            ::CloseHandle(handle);
        }
    }

    منطقی lock()
    {
        OVERLAPPED overlapped;
        std::memset(&overlapped, 0, حجم(overlapped));
        واپس !!::تالا_File_Ex(handle, LOCKFILE_EXCLUSIVE_تالا, 0, MAXDWORD, MAXDWORD, &overlapped);
    }
    منطقی unlock()
    {
        OVERLAPPED overlapped;
        std::memset(&overlapped, 0, حجم(overlapped));
        واپس !!::UnlockFileEx(handle, 0, MAXDWORD, MAXDWORD, &overlapped);
    }

    منطقی تالا_shared()
    {
        OVERLAPPED overlapped;
        std::memset(&overlapped, 0, حجم(overlapped));
        واپس !!::تالا_File_Ex(handle, 0, 0, MAXDWORD, MAXDWORD, &overlapped);
    }
    منطقی unlock_shared()
    {
        واپس unlock();
    }

    HANDLE handle;

نجی:
    Impl(ثابت Impl&); // disabled
    Impl& عامل=(ثابت Impl&); // disabled
};

#وگرنہ_اگر defined __linux__ || defined __APPLE__ || defined __HAIKU__ || defined __FreeBSD__ || defined __GNU__ || defined __EMSCRIPTEN__ || defined __QNX__ || defined __CYGWIN__

ساخت File_تالا::Impl
{
    Impl(ثابت حرف* fname)
    {
        handle = ::open(fname, O_RDWR);
        CV_Assert(handle != -1);
    }
    ~Impl()
    {
        اگر (handle >= 0)
            ::close(handle);
    }

    منطقی lock()
    {
        ساخت ::flock l;
        std::memset(&l, 0, حجم(l));
        l.l_type = F_WRLCK;
        l.l_whence = SEEK_SET;
        l.l_آغاز = 0;
        l.l_len = 0;
        DBG(std::خارج << "Lock..." << std::سطر_ختم);
        منطقی res = -1 != ::fcntl(handle, F_SETLKW, &l);
        واپس res;
    }
    منطقی unlock()
    {
        ساخت ::flock l;
        std::memset(&l, 0, حجم(l));
        l.l_type = F_UNLCK;
        l.l_whence = SEEK_SET;
        l.l_آغاز = 0;
        l.l_len = 0;
        DBG(std::خارج << "Unlock..." << std::سطر_ختم);
        منطقی res = -1 != ::fcntl(handle, F_SETLK, &l);
        واپس res;
    }

    منطقی تالا_shared()
    {
        ساخت ::flock l;
        std::memset(&l, 0, حجم(l));
        l.l_type = F_RDLCK;
        l.l_whence = SEEK_SET;
        l.l_آغاز = 0;
        l.l_len = 0;
        DBG(std::خارج << "Lock read..." << std::سطر_ختم);
        منطقی res = -1 != ::fcntl(handle, F_SETLKW, &l);
        واپس res;
    }
    منطقی unlock_shared()
    {
        واپس unlock();
    }

    صحیح handle;

نجی:
    Impl(ثابت Impl&); // disabled
    Impl& عامل=(ثابت Impl&); // disabled
};

#اگر_ختم

File_تالا::File_تالا(ثابت حرف* fname)
    : pImpl(نیا Impl(fname))
{
    // nothing
}
File_تالا::~File_تالا()
{
    حذف pImpl;
    pImpl = تہی;
}

خالی File_تالا::lock() { CV_Assert(pImpl->lock()); }
خالی File_تالا::unlock() { CV_Assert(pImpl->unlock()); }
خالی File_تالا::تالا_shared() { CV_Assert(pImpl->تالا_shared()); }
خالی File_تالا::unlock_shared() { CV_Assert(pImpl->unlock_shared()); }



cv::String getCacheDirectory(ثابت حرف* sub_directory_name, ثابت حرف* configuration_name)
{
    String cache_راستہ_2;
    اگر (configuration_name)
    {
        cache_راستہ_2 = utils::getConfigurationParameterString(configuration_name, "");
    }
    اگر (cache_راستہ_2.empty())
    {
        cv::String default_cache_راستہ;
#اگر_تعریف_شدہ _WIN32
        wchar_t tmp_راستہ_buf_w[MAX_راستہ+1] = {0};
        DWORD res = Get_عارضی_راستہ_W(MAX_راستہ, tmp_راستہ_buf_w);
        اگر (res > 0 && res <= MAX_راستہ)
        {
            // Convert from UTF-16 to UTF-8 to support Unicode paths
            صحیح len = Wide_Char_مقصد_Multi_Byte(CP_UTF8, 0, tmp_راستہ_buf_w, -1, تہی, 0, تہی, تہی);
            اگر (len > 0)
            {
                std::vector<حرف> utf8_buf(len);
                Wide_Char_مقصد_Multi_Byte(CP_UTF8, 0, tmp_راستہ_buf_w, -1, utf8_buf.ڈیٹا(), len, تہی, تہی);
                default_cache_راستہ = std::string(utf8_buf.ڈیٹا());
            }
        }
#وگرنہ_اگر defined __ANDROID__
        // no defaults
#وگرنہ_اگر defined __APPLE__
        ثابت std::string tmpdir_env = utils::getConfigurationParameterString("TMPDIR");
        اگر (!tmpdir_env.empty() && utils::fs::isDirectory(tmpdir_env))
        {
            default_cache_راستہ = tmpdir_env;
        }
        ورنہ
        {
            default_cache_راستہ = "/tmp/";
            CV_LOG_WARNING(تہی, "Using world accessible cache directory. This may be not secure: " << default_cache_راستہ);
        }
#وگرنہ_اگر defined __linux__ || defined __HAIKU__ || defined __FreeBSD__ || defined __GNU__
        // https://specifications.freedesktop.org/basedir-spec/basedir-spec-latest.html
        اگر (default_cache_راستہ.empty())
        {
            ثابت std::string xdg_cache_env = utils::getConfigurationParameterString("XDG_CACHE_HOME");
            اگر (!xdg_cache_env.empty() && utils::fs::isDirectory(xdg_cache_env))
            {
                default_cache_راستہ = xdg_cache_env;
            }
        }
        اگر (default_cache_راستہ.empty())
        {
            ثابت std::string home_env = utils::getConfigurationParameterString("HOME");
            اگر (!home_env.empty() && utils::fs::isDirectory(home_env))
            {
                cv::String home_راستہ = home_env;
                cv::String home_cache_راستہ = utils::fs::join(home_راستہ, ".cache/");
                اگر (utils::fs::isDirectory(home_cache_راستہ))
                {
                    default_cache_راستہ = home_cache_راستہ;
                }
            }
        }
        اگر (default_cache_راستہ.empty())
        {
            ثابت حرف* عارضی_راستہ = "/var/tmp/";
            اگر (utils::fs::isDirectory(عارضی_راستہ))
            {
                default_cache_راستہ = عارضی_راستہ;
                CV_LOG_WARNING(تہی, "Using world accessible cache directory. This may be not secure: " << default_cache_راستہ);
            }
        }
        اگر (default_cache_راستہ.empty())
        {
            default_cache_راستہ = "/tmp/";
            CV_LOG_WARNING(تہی, "Using world accessible cache directory. This may be not secure: " << default_cache_راستہ);
        }
#ورنہ
        // no defaults
#اگر_ختم
        CV_LOG_VERBOSE(تہی, 0, "default_cache_path = " << default_cache_راستہ);
        اگر (!default_cache_راستہ.empty())
        {
            اگر (utils::fs::isDirectory(default_cache_راستہ))
            {
                cv::String default_cache_راستہ_بنیاد = utils::fs::join(default_cache_راستہ, "opencv");
                default_cache_راستہ = utils::fs::join(default_cache_راستہ_بنیاد, CVAUX_STR(CV_نسخہ_MAJOR) ".x" CV_نسخہ_حالت);
                اگر (utils::getConfigurationParameterBool("OPENCV_CACHE_SHOW_CLEANUP_MESSAGE", سچ)
                    && !utils::fs::isDirectory(default_cache_راستہ))
                {
                    std::vector<cv::String> existedCacheDirs;
                    کوشش
                    {
                        utils::fs::glob_relative(default_cache_راستہ_بنیاد, "*", existedCacheDirs, جھوٹ, سچ);
                    }
                    پکڑو (...)
                    {
                        // ignore
                    }
                    اگر (!existedCacheDirs.empty())
                    {
                        CV_LOG_WARNING(تہی, "Creating new OpenCV cache directory: " << default_cache_راستہ);
                        CV_LOG_WARNING(تہی, "There are several neighbour directories, probably created by old OpenCV versions.");
                        CV_LOG_WARNING(تہی, "Feel free to cleanup these unused directories:");
                        برائے (size_t i = 0; i < existedCacheDirs.size(); i++)
                        {
                            CV_LOG_WARNING(تہی, "  - " << existedCacheDirs[i]);
                        }
                        CV_LOG_WARNING(تہی, "Note: This message is showed only once.");
                    }
                }
                اگر (sub_directory_name && sub_directory_name[0] != '\0')
                    default_cache_راستہ = utils::fs::join(default_cache_راستہ, cv::String(sub_directory_name) + native_separator);
                اگر (!utils::fs::createDirectories(default_cache_راستہ))
                {
                    CV_LOG_DEBUG(تہی, "Can't create OpenCV cache sub-directory: " << default_cache_راستہ);
                }
                ورنہ
                {
                    cache_راستہ_2 = default_cache_راستہ;
                }
            }
            ورنہ
            {
                CV_LOG_INFO(تہی, "Can't find default cache directory (does it exist?): " << default_cache_راستہ);
            }
        }
        ورنہ
        {
            CV_LOG_DEBUG(تہی, "OpenCV has no support to discover default cache directory on the current platform");
        }
    }
    ورنہ
    {
        اگر (cache_راستہ_2 == "disabled")
            واپس cache_راستہ_2;
        اگر (!isDirectory(cache_راستہ_2))
        {
            CV_LOG_WARNING(تہی, "Specified non-existed directory, creating OpenCV sub-directory for caching purposes: " << cache_راستہ_2);
            اگر (!createDirectories(cache_راستہ_2))
            {
                CV_LOG_ERROR(تہی, "Can't create OpenCV cache sub-directory: " << cache_راستہ_2);
                cache_راستہ_2.clear();
            }
        }
    }
    CV_Assert(cache_راستہ_2.empty() || utils::fs::isDirectory(cache_راستہ_2));
    اگر (!cache_راستہ_2.empty())
    {
        اگر (!is_راستہ_Separator(cache_راستہ_2[cache_راستہ_2.size() - 1]))
        {
            cache_راستہ_2 += native_separator;
        }
    }
    واپس cache_راستہ_2;
}

#ورنہ
#تعریف_کرو NOT_IMPLEMENTED CV_Error(Error::StsNotImplemented, "");
cv::String canonical(ثابت cv::String& /*راستہ*/) { NOT_IMPLEMENTED }
منطقی exists(ثابت cv::String& /*راستہ*/) { NOT_IMPLEMENTED }
خالی remove_all(ثابت cv::String& /*راستہ*/) { NOT_IMPLEMENTED }
cv::String getcwd() { NOT_IMPLEMENTED }
منطقی createDirectory(ثابت cv::String& /*راستہ*/) { NOT_IMPLEMENTED }
منطقی createDirectories(ثابت cv::String& /*راستہ*/) { NOT_IMPLEMENTED }
cv::String getCacheDirectory(ثابت حرف* /*sub_directory_name*/, ثابت حرف* /*configuration_name = NULL*/) { NOT_IMPLEMENTED }
#تعریف_منسوخ NOT_IMPLEMENTED
#اگر_ختم // OPENCV_HAVE_FILESYSTEM_SUPPORT

}}} // namespace


#اگر OPENCV_HAVE_FILESYSTEM_SUPPORT
#شامل "plugin_loader.impl.hpp"
#اگر_ختم
