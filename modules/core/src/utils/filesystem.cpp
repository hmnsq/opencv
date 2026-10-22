// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#అన్తర్భావ "../precomp.hpp"

#అన్తర్భావ <opencv2/core/utils/configuration.private.hpp>

#అన్తర్భావ <opencv2/core/utils/logger.hpp>

#అన్తర్భావ "opencv2/core/utils/filesystem.private.hpp"
#అన్తర్భావ "opencv2/core/utils/filesystem.hpp"

//#define DEBUG_FS_UTILS

#నిర్వచితమైతే DEBUG_FS_UTILS
#అన్తర్భావ <iostream>
#నిర్వచించు DBG(...) __VA_ARGS__
#లేకపోతే
#నిర్వచించు DBG(...)
#అయితే_ముగింపు


#ఒకవేళ OPENCV_HAVE_FILESYSTEM_SUPPORT

#నిర్వచితమైతే _WIN32
#నిర్వచించు WIN32_LEAN_AND_సగటు
#నిర్వచనం_రద్దు NOMINMAX
#నిర్వచించు NOMINMAX
#అన్తర్భావ <windows.h>
#అన్తర్భావ <direct.h>
#అన్తర్భావ <sys/types.h>
#అన్తర్భావ <sys/stat.h>
#అన్తర్భావ <errno.h>
#అన్తర్భావ <io.h>
#అన్తర్భావ <stdio.h>
#లేదావేళ defined __linux__ || defined __APPLE__ || defined __HAIKU__ || defined __FreeBSD__ || defined __GNU__ || defined __EMSCRIPTEN__ || defined __QNX__ || defined __CYGWIN__
#అన్తర్భావ <sys/types.h>
#అన్తర్భావ <sys/stat.h>
#అన్తర్భావ <fcntl.h>
#అన్తర్భావ <unistd.h>
#అన్తర్భావ <errno.h>
#అయితే_ముగింపు

#అయితే_ముగింపు // OPENCV_HAVE_FILESYSTEM_SUPPORT

నామాకాశ cv { నామాకాశ utils { నామాకాశ fs {

#నిర్వచితమైతే _WIN32
స్థిర స్థిరరాశి అక్షరం native_separator = '\\';
#లేకపోతే
స్థిర స్థిరరాశి అక్షరం native_separator = '/';
#అయితే_ముగింపు

స్థిర అంతఃవరుస
తార్కిక is_మార్గం_Separator(అక్షరం c)
{
    తిరిగి c == '/' || c == '\\';
}

cv::String join(స్థిరరాశి cv::String& పునాది, స్థిరరాశి cv::String& మార్గం)
{
    ఒకవేళ (పునాది.empty())
        తిరిగి మార్గం;
    ఒకవేళ (మార్గం.empty())
        తిరిగి పునాది;

    తార్కిక పునాది_Sep = is_మార్గం_Separator(పునాది[పునాది.size() - 1]);
    తార్కిక మార్గం_Sep = is_మార్గం_Separator(మార్గం[0]);
    String ఫలితం;
    ఒకవేళ (పునాది_Sep && మార్గం_Sep)
    {
        ఫలితం = పునాది + మార్గం.substr(1);
    }
    లేకపోతే ఒకవేళ (!పునాది_Sep && !మార్గం_Sep)
    {
        ఫలితం = పునాది + native_separator + మార్గం;
    }
    లేకపోతే
    {
        ఫలితం = పునాది + మార్గం;
    }
    తిరిగి ఫలితం;
}

CV_EXPORTS cv::String get_మాతృ(స్థిరరాశి cv::String &మార్గం)
{
    std::string::size_type loc = మార్గం.find_last_of("/\\");
    ఒకవేళ (loc == std::string::npos)
        తిరిగి std::string();
    తిరిగి std::string(మార్గం, 0, loc);
}

CV_EXPORTS std::wstring get_మాతృ(స్థిరరాశి std::wstring& మార్గం)
{
    std::wstring::size_type loc = మార్గం.find_last_of(L"/\\");
    ఒకవేళ (loc == std::wstring::npos)
        తిరిగి std::wstring();
    తిరిగి std::wstring(మార్గం, 0, loc);
}

#ఒకవేళ OPENCV_HAVE_FILESYSTEM_SUPPORT

cv::String canonical(స్థిరరాశి cv::String& మార్గం)
{
    cv::String ఫలితం;
#నిర్వచితమైతే _WIN32
    స్థిరరాశి అక్షరం* ఫలితం_str = _fullpath(ఖాళీ, మార్గం.c_str(), 0);
#లేకపోతే
    స్థిరరాశి అక్షరం* ఫలితం_str = realpath(మార్గం.c_str(), ఖాళీ);
#అయితే_ముగింపు
    ఒకవేళ (ఫలితం_str)
    {
        ఫలితం = cv::String(ఫలితం_str);
        విడుదల((శూన్య*)ఫలితం_str);
    }
    తిరిగి ఫలితం.empty() ? మార్గం : ఫలితం;
}


తార్కిక exists(స్థిరరాశి cv::String& మార్గం)
{
    CV_INSTRUMENT_REGION();

#ఒకవేళ defined _WIN32 || defined WINCE
    BOOL స్థితి_2 = TRUE;
    {
        WIN32_FILE_ATTRIBUTE_దత్తాంశం all_attrs;
#నిర్వచితమైతే WINRT
        wchar_t wpath[MAX_మార్గం];
        size_t copied = mbstowcs(wpath, మార్గం.c_str(), MAX_మార్గం);
        CV_Assert((copied != MAX_మార్గం) && (copied != (size_t)-1));
        స్థితి_2 = ::Get_File_లక్షణాలు_Ex_W(wpath, GetFileExInfoStandard, &all_attrs);
#లేకపోతే
        స్థితి_2 = ::Get_File_లక్షణాలు_Ex_A(మార్గం.c_str(), GetFileExInfoStandard, &all_attrs);
#అయితే_ముగింపు
    }

    తిరిగి !!స్థితి_2;
#లేకపోతే
    నిర్మాణం stat stat_buf;
    తిరిగి (0 == stat(మార్గం.c_str(), &stat_buf));
#అయితే_ముగింపు
}

CV_EXPORTS శూన్య remove_all(స్థిరరాశి cv::String& మార్గం)
{
    ఒకవేళ (!exists(మార్గం))
        తిరిగి;
    ఒకవేళ (isDirectory(మార్గం))
    {
        std::vector<String> entries;
        utils::fs::glob(మార్గం, cv::String(), entries, అబద్ధం, నిజం);
        కొరకు (size_t i = 0; i < entries.size(); i++)
        {
            స్థిరరాశి String& e = entries[i];
            remove_all(e);
        }
#నిర్వచితమైతే _MSC_VER
        తార్కిక ఫలితం = _rmdir(మార్గం.c_str()) == 0;
#లేకపోతే
        తార్కిక ఫలితం = rmdir(మార్గం.c_str()) == 0;
#అయితే_ముగింపు
        ఒకవేళ (!ఫలితం)
        {
            CV_LOG_ERROR(ఖాళీ, "Can't remove directory: " << మార్గం);
        }
    }
    లేకపోతే
    {
#నిర్వచితమైతే _MSC_VER
        తార్కిక ఫలితం = _unlink(మార్గం.c_str()) == 0;
#లేకపోతే
        తార్కిక ఫలితం = unlink(మార్గం.c_str()) == 0;
#అయితే_ముగింపు
        ఒకవేళ (!ఫలితం)
        {
            CV_LOG_ERROR(ఖాళీ, "Can't remove file: " << మార్గం);
        }
    }
}


cv::String getcwd()
{
    CV_INSTRUMENT_REGION();
    cv::Auto_బఫరు<అక్షరం, 4096> buf;
#ఒకవేళ defined WIN32 || defined _WIN32 || defined WINCE
#నిర్వచితమైతే WINRT
    తిరిగి cv::String();
#లేకపోతే
    DWORD sz = GetCurrentDirectoryA(0, ఖాళీ);
    buf.allocate((size_t)sz);
    sz = GetCurrentDirectoryA((DWORD)buf.size(), buf.దత్తాంశం());
    తిరిగి cv::String(buf.దత్తాంశం(), (size_t)sz);
#అయితే_ముగింపు
#లేదావేళ defined __linux__ || defined __APPLE__ || defined __HAIKU__ || defined __FreeBSD__ || defined __EMSCRIPTEN__ || defined __QNX__ || defined __CYGWIN__
    కొరకు(;;)
    {
        అక్షరం* p = ::getcwd(buf.దత్తాంశం(), buf.size());
        ఒకవేళ (p == ఖాళీ)
        {
            ఒకవేళ (errno == ERANGE)
            {
                buf.allocate(buf.size() * 2);
                కొనసాగు;
            }
            తిరిగి cv::String();
        }
        ఆపు;
    }
    తిరిగి cv::String(buf.దత్తాంశం(), (size_t)అక్షరశ్రేణి_పొడవు(buf.దత్తాంశం()));
#లేకపోతే
    తిరిగి cv::String();
#అయితే_ముగింపు
}


తార్కిక createDirectory(స్థిరరాశి cv::String& మార్గం)
{
    CV_INSTRUMENT_REGION();
#ఒకవేళ defined WIN32 || defined _WIN32 || defined WINCE
#నిర్వచితమైతే WINRT
    wchar_t wpath[MAX_మార్గం];
    size_t copied = mbstowcs(wpath, మార్గం.c_str(), MAX_మార్గం);
    CV_Assert((copied != MAX_మార్గం) && (copied != (size_t)-1));
    పూర్ణాంకం ఫలితం = CreateDirectoryA(wpath, ఖాళీ) ? 0 : -1;
#లేకపోతే
    పూర్ణాంకం ఫలితం = _mkdir(మార్గం.c_str());
#అయితే_ముగింపు
#లేదావేళ defined __linux__ || defined __APPLE__ || defined __HAIKU__ || defined __FreeBSD__ || defined __EMSCRIPTEN__ || defined __QNX__ || defined __CYGWIN__
    పూర్ణాంకం ఫలితం = mkdir(మార్గం.c_str(), 0777);
#లేకపోతే
    పూర్ణాంకం ఫలితం = -1;
#అయితే_ముగింపు

    ఒకవేళ (ఫలితం == -1)
    {
        తిరిగి isDirectory(మార్గం);
    }
    తిరిగి నిజం;
}

తార్కిక createDirectories(స్థిరరాశి cv::String& మార్గం_3)
{
    cv::String మార్గం = మార్గం_3;
    కొరకు (;;)
    {
        అక్షరం చివరి_char = మార్గం.empty() ? 0 : మార్గం[మార్గం.length() - 1];
        ఒకవేళ (is_మార్గం_Separator(చివరి_char))
        {
            మార్గం = మార్గం.substr(0, మార్గం.length() - 1);
            కొనసాగు;
        }
        ఆపు;
    }

    ఒకవేళ (మార్గం.empty() || మార్గం == "./" || మార్గం == ".\\" || మార్గం == ".")
        తిరిగి నిజం;
    ఒకవేళ (isDirectory(మార్గం))
        తిరిగి నిజం;

    size_t pos = మార్గం.rfind('/');
    ఒకవేళ (pos == cv::String::npos)
        pos = మార్గం.rfind('\\');
    ఒకవేళ (pos != cv::String::npos)
    {
        cv::String మాతృ_directory = మార్గం.substr(0, pos);
        ఒకవేళ (!మాతృ_directory.empty())
        {
            ఒకవేళ (!createDirectories(మాతృ_directory))
                తిరిగి అబద్ధం;
        }
    }

    తిరిగి createDirectory(మార్గం);
}

#నిర్వచితమైతే _WIN32

నిర్మాణం File_తాళం::Impl
{
    Impl(స్థిరరాశి అక్షరం* fname)
    {
        // http://support.microsoft.com/kb/316609
        పూర్ణాంకం అంకె_Retries = 5;
        చేయి
        {
            handle = ::CreateFileA(fname, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, ఖాళీ,
                                OPEN_EXISTING, FILE_ATTRIBUTE_సాధారణ, ఖాళీ);
            ఒకవేళ (INVALID_HANDLE_విలువ == handle)
            {
                ఒకవేళ (ERROR_SHARING_VIOLATION == Get_చివరి_Error())
                {
                    అంకె_Retries--;
                    Sleep(250);
                    కొనసాగు;
                }
                లేకపోతే
                {
                    CV_Error_(Error::StsAssert, ("Can't open lock file: %s", fname));
                }
            }
            ఆపు;
        } అయితే (అంకె_Retries > 0);
    }
    ~Impl()
    {
        ఒకవేళ (INVALID_HANDLE_విలువ != handle)
        {
            ::CloseHandle(handle);
        }
    }

    తార్కిక lock()
    {
        OVERLAPPED overlapped;
        std::memset(&overlapped, 0, పరిమాణం(overlapped));
        తిరిగి !!::తాళం_File_Ex(handle, LOCKFILE_EXCLUSIVE_తాళం, 0, MAXDWORD, MAXDWORD, &overlapped);
    }
    తార్కిక unlock()
    {
        OVERLAPPED overlapped;
        std::memset(&overlapped, 0, పరిమాణం(overlapped));
        తిరిగి !!::UnlockFileEx(handle, 0, MAXDWORD, MAXDWORD, &overlapped);
    }

    తార్కిక తాళం_shared()
    {
        OVERLAPPED overlapped;
        std::memset(&overlapped, 0, పరిమాణం(overlapped));
        తిరిగి !!::తాళం_File_Ex(handle, 0, 0, MAXDWORD, MAXDWORD, &overlapped);
    }
    తార్కిక unlock_shared()
    {
        తిరిగి unlock();
    }

    HANDLE handle;

అంతరంగ:
    Impl(స్థిరరాశి Impl&); // disabled
    Impl& క్రియాకారకం=(స్థిరరాశి Impl&); // disabled
};

#లేదావేళ defined __linux__ || defined __APPLE__ || defined __HAIKU__ || defined __FreeBSD__ || defined __GNU__ || defined __EMSCRIPTEN__ || defined __QNX__ || defined __CYGWIN__

నిర్మాణం File_తాళం::Impl
{
    Impl(స్థిరరాశి అక్షరం* fname)
    {
        handle = ::open(fname, O_RDWR);
        CV_Assert(handle != -1);
    }
    ~Impl()
    {
        ఒకవేళ (handle >= 0)
            ::close(handle);
    }

    తార్కిక lock()
    {
        నిర్మాణం ::flock l;
        std::memset(&l, 0, పరిమాణం(l));
        l.l_type = F_WRLCK;
        l.l_whence = SEEK_SET;
        l.l_ప్రారంభం = 0;
        l.l_len = 0;
        DBG(std::బయటకు << "Lock..." << std::పంక్తి_ముగింపు);
        తార్కిక res = -1 != ::fcntl(handle, F_SETLKW, &l);
        తిరిగి res;
    }
    తార్కిక unlock()
    {
        నిర్మాణం ::flock l;
        std::memset(&l, 0, పరిమాణం(l));
        l.l_type = F_UNLCK;
        l.l_whence = SEEK_SET;
        l.l_ప్రారంభం = 0;
        l.l_len = 0;
        DBG(std::బయటకు << "Unlock..." << std::పంక్తి_ముగింపు);
        తార్కిక res = -1 != ::fcntl(handle, F_SETLK, &l);
        తిరిగి res;
    }

    తార్కిక తాళం_shared()
    {
        నిర్మాణం ::flock l;
        std::memset(&l, 0, పరిమాణం(l));
        l.l_type = F_RDLCK;
        l.l_whence = SEEK_SET;
        l.l_ప్రారంభం = 0;
        l.l_len = 0;
        DBG(std::బయటకు << "Lock read..." << std::పంక్తి_ముగింపు);
        తార్కిక res = -1 != ::fcntl(handle, F_SETLKW, &l);
        తిరిగి res;
    }
    తార్కిక unlock_shared()
    {
        తిరిగి unlock();
    }

    పూర్ణాంకం handle;

అంతరంగ:
    Impl(స్థిరరాశి Impl&); // disabled
    Impl& క్రియాకారకం=(స్థిరరాశి Impl&); // disabled
};

#అయితే_ముగింపు

File_తాళం::File_తాళం(స్థిరరాశి అక్షరం* fname)
    : pImpl(కొత్త Impl(fname))
{
    // nothing
}
File_తాళం::~File_తాళం()
{
    తొలగించు pImpl;
    pImpl = ఖాళీ;
}

శూన్య File_తాళం::lock() { CV_Assert(pImpl->lock()); }
శూన్య File_తాళం::unlock() { CV_Assert(pImpl->unlock()); }
శూన్య File_తాళం::తాళం_shared() { CV_Assert(pImpl->తాళం_shared()); }
శూన్య File_తాళం::unlock_shared() { CV_Assert(pImpl->unlock_shared()); }



cv::String getCacheDirectory(స్థిరరాశి అక్షరం* sub_directory_name, స్థిరరాశి అక్షరం* configuration_name)
{
    String cache_మార్గం_2;
    ఒకవేళ (configuration_name)
    {
        cache_మార్గం_2 = utils::getConfigurationParameterString(configuration_name, "");
    }
    ఒకవేళ (cache_మార్గం_2.empty())
    {
        cv::String default_cache_మార్గం;
#నిర్వచితమైతే _WIN32
        wchar_t tmp_మార్గం_buf_w[MAX_మార్గం+1] = {0};
        DWORD res = Get_తాత్కాలిక_మార్గం_W(MAX_మార్గం, tmp_మార్గం_buf_w);
        ఒకవేళ (res > 0 && res <= MAX_మార్గం)
        {
            // Convert from UTF-16 to UTF-8 to support Unicode paths
            పూర్ణాంకం len = Wide_Char_గమ్యం_Multi_Byte(CP_UTF8, 0, tmp_మార్గం_buf_w, -1, ఖాళీ, 0, ఖాళీ, ఖాళీ);
            ఒకవేళ (len > 0)
            {
                std::vector<అక్షరం> utf8_buf(len);
                Wide_Char_గమ్యం_Multi_Byte(CP_UTF8, 0, tmp_మార్గం_buf_w, -1, utf8_buf.దత్తాంశం(), len, ఖాళీ, ఖాళీ);
                default_cache_మార్గం = std::string(utf8_buf.దత్తాంశం());
            }
        }
#లేదావేళ defined __ANDROID__
        // no defaults
#లేదావేళ defined __APPLE__
        స్థిరరాశి std::string tmpdir_env = utils::getConfigurationParameterString("TMPDIR");
        ఒకవేళ (!tmpdir_env.empty() && utils::fs::isDirectory(tmpdir_env))
        {
            default_cache_మార్గం = tmpdir_env;
        }
        లేకపోతే
        {
            default_cache_మార్గం = "/tmp/";
            CV_LOG_WARNING(ఖాళీ, "Using world accessible cache directory. This may be not secure: " << default_cache_మార్గం);
        }
#లేదావేళ defined __linux__ || defined __HAIKU__ || defined __FreeBSD__ || defined __GNU__
        // https://specifications.freedesktop.org/basedir-spec/basedir-spec-latest.html
        ఒకవేళ (default_cache_మార్గం.empty())
        {
            స్థిరరాశి std::string xdg_cache_env = utils::getConfigurationParameterString("XDG_CACHE_HOME");
            ఒకవేళ (!xdg_cache_env.empty() && utils::fs::isDirectory(xdg_cache_env))
            {
                default_cache_మార్గం = xdg_cache_env;
            }
        }
        ఒకవేళ (default_cache_మార్గం.empty())
        {
            స్థిరరాశి std::string home_env = utils::getConfigurationParameterString("HOME");
            ఒకవేళ (!home_env.empty() && utils::fs::isDirectory(home_env))
            {
                cv::String home_మార్గం = home_env;
                cv::String home_cache_మార్గం = utils::fs::join(home_మార్గం, ".cache/");
                ఒకవేళ (utils::fs::isDirectory(home_cache_మార్గం))
                {
                    default_cache_మార్గం = home_cache_మార్గం;
                }
            }
        }
        ఒకవేళ (default_cache_మార్గం.empty())
        {
            స్థిరరాశి అక్షరం* తాత్కాలిక_మార్గం = "/var/tmp/";
            ఒకవేళ (utils::fs::isDirectory(తాత్కాలిక_మార్గం))
            {
                default_cache_మార్గం = తాత్కాలిక_మార్గం;
                CV_LOG_WARNING(ఖాళీ, "Using world accessible cache directory. This may be not secure: " << default_cache_మార్గం);
            }
        }
        ఒకవేళ (default_cache_మార్గం.empty())
        {
            default_cache_మార్గం = "/tmp/";
            CV_LOG_WARNING(ఖాళీ, "Using world accessible cache directory. This may be not secure: " << default_cache_మార్గం);
        }
#లేకపోతే
        // no defaults
#అయితే_ముగింపు
        CV_LOG_VERBOSE(ఖాళీ, 0, "default_cache_path = " << default_cache_మార్గం);
        ఒకవేళ (!default_cache_మార్గం.empty())
        {
            ఒకవేళ (utils::fs::isDirectory(default_cache_మార్గం))
            {
                cv::String default_cache_మార్గం_పునాది = utils::fs::join(default_cache_మార్గం, "opencv");
                default_cache_మార్గం = utils::fs::join(default_cache_మార్గం_పునాది, CVAUX_STR(CV_సంస్కరణ_MAJOR) ".x" CV_సంస్కరణ_స్థితి);
                ఒకవేళ (utils::getConfigurationParameterBool("OPENCV_CACHE_SHOW_CLEANUP_MESSAGE", నిజం)
                    && !utils::fs::isDirectory(default_cache_మార్గం))
                {
                    std::vector<cv::String> existedCacheDirs;
                    ప్రయత్నించు
                    {
                        utils::fs::glob_relative(default_cache_మార్గం_పునాది, "*", existedCacheDirs, అబద్ధం, నిజం);
                    }
                    పట్టుకో (...)
                    {
                        // ignore
                    }
                    ఒకవేళ (!existedCacheDirs.empty())
                    {
                        CV_LOG_WARNING(ఖాళీ, "Creating new OpenCV cache directory: " << default_cache_మార్గం);
                        CV_LOG_WARNING(ఖాళీ, "There are several neighbour directories, probably created by old OpenCV versions.");
                        CV_LOG_WARNING(ఖాళీ, "Feel free to cleanup these unused directories:");
                        కొరకు (size_t i = 0; i < existedCacheDirs.size(); i++)
                        {
                            CV_LOG_WARNING(ఖాళీ, "  - " << existedCacheDirs[i]);
                        }
                        CV_LOG_WARNING(ఖాళీ, "Note: This message is showed only once.");
                    }
                }
                ఒకవేళ (sub_directory_name && sub_directory_name[0] != '\0')
                    default_cache_మార్గం = utils::fs::join(default_cache_మార్గం, cv::String(sub_directory_name) + native_separator);
                ఒకవేళ (!utils::fs::createDirectories(default_cache_మార్గం))
                {
                    CV_LOG_DEBUG(ఖాళీ, "Can't create OpenCV cache sub-directory: " << default_cache_మార్గం);
                }
                లేకపోతే
                {
                    cache_మార్గం_2 = default_cache_మార్గం;
                }
            }
            లేకపోతే
            {
                CV_LOG_INFO(ఖాళీ, "Can't find default cache directory (does it exist?): " << default_cache_మార్గం);
            }
        }
        లేకపోతే
        {
            CV_LOG_DEBUG(ఖాళీ, "OpenCV has no support to discover default cache directory on the current platform");
        }
    }
    లేకపోతే
    {
        ఒకవేళ (cache_మార్గం_2 == "disabled")
            తిరిగి cache_మార్గం_2;
        ఒకవేళ (!isDirectory(cache_మార్గం_2))
        {
            CV_LOG_WARNING(ఖాళీ, "Specified non-existed directory, creating OpenCV sub-directory for caching purposes: " << cache_మార్గం_2);
            ఒకవేళ (!createDirectories(cache_మార్గం_2))
            {
                CV_LOG_ERROR(ఖాళీ, "Can't create OpenCV cache sub-directory: " << cache_మార్గం_2);
                cache_మార్గం_2.clear();
            }
        }
    }
    CV_Assert(cache_మార్గం_2.empty() || utils::fs::isDirectory(cache_మార్గం_2));
    ఒకవేళ (!cache_మార్గం_2.empty())
    {
        ఒకవేళ (!is_మార్గం_Separator(cache_మార్గం_2[cache_మార్గం_2.size() - 1]))
        {
            cache_మార్గం_2 += native_separator;
        }
    }
    తిరిగి cache_మార్గం_2;
}

#లేకపోతే
#నిర్వచించు NOT_IMPLEMENTED CV_Error(Error::StsNotImplemented, "");
cv::String canonical(స్థిరరాశి cv::String& /*మార్గం*/) { NOT_IMPLEMENTED }
తార్కిక exists(స్థిరరాశి cv::String& /*మార్గం*/) { NOT_IMPLEMENTED }
శూన్య remove_all(స్థిరరాశి cv::String& /*మార్గం*/) { NOT_IMPLEMENTED }
cv::String getcwd() { NOT_IMPLEMENTED }
తార్కిక createDirectory(స్థిరరాశి cv::String& /*మార్గం*/) { NOT_IMPLEMENTED }
తార్కిక createDirectories(స్థిరరాశి cv::String& /*మార్గం*/) { NOT_IMPLEMENTED }
cv::String getCacheDirectory(స్థిరరాశి అక్షరం* /*sub_directory_name*/, స్థిరరాశి అక్షరం* /*configuration_name = NULL*/) { NOT_IMPLEMENTED }
#నిర్వచనం_రద్దు NOT_IMPLEMENTED
#అయితే_ముగింపు // OPENCV_HAVE_FILESYSTEM_SUPPORT

}}} // namespace


#ఒకవేళ OPENCV_HAVE_FILESYSTEM_SUPPORT
#అన్తర్భావ "plugin_loader.impl.hpp"
#అయితే_ముగింపు
