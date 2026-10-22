// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#अन्तर्भाव "../precomp.hpp"

#अन्तर्भाव <opencv2/core/utils/configuration.private.hpp>

#अन्तर्भाव <opencv2/core/utils/logger.hpp>

#अन्तर्भाव "opencv2/core/utils/filesystem.private.hpp"
#अन्तर्भाव "opencv2/core/utils/filesystem.hpp"

//#define DEBUG_FS_UTILS

#यदि_परिभाषितम् DEBUG_FS_UTILS
#अन्तर्भाव <iostream>
#परिभाषय DBG(...) __VA_ARGS__
#अन्यथा
#परिभाषय DBG(...)
#यदि_अन्तः


#यदि OPENCV_HAVE_FILESYSTEM_SUPPORT

#यदि_परिभाषितम् _WIN32
#परिभाषय WIN32_LEAN_AND_माध्यम्
#अपरिभाषय NOMINMAX
#परिभाषय NOMINMAX
#अन्तर्भाव <windows.h>
#अन्तर्भाव <direct.h>
#अन्तर्भाव <sys/types.h>
#अन्तर्भाव <sys/stat.h>
#अन्तर्भाव <errno.h>
#अन्तर्भाव <io.h>
#अन्तर्भाव <stdio.h>
#नोचेद्यदि defined __linux__ || defined __APPLE__ || defined __HAIKU__ || defined __FreeBSD__ || defined __GNU__ || defined __EMSCRIPTEN__ || defined __QNX__ || defined __CYGWIN__
#अन्तर्भाव <sys/types.h>
#अन्तर्भाव <sys/stat.h>
#अन्तर्भाव <fcntl.h>
#अन्तर्भाव <unistd.h>
#अन्तर्भाव <errno.h>
#यदि_अन्तः

#यदि_अन्तः // OPENCV_HAVE_FILESYSTEM_SUPPORT

नामाकाश cv { नामाकाश utils { नामाकाश fs {

#यदि_परिभाषितम् _WIN32
स्थिर ध्रुव वर्ण native_separator = '\\';
#अन्यथा
स्थिर ध्रुव वर्ण native_separator = '/';
#यदि_अन्तः

स्थिर अन्तःस्थ
तार्किक is_मार्गः_Separator(वर्ण c)
{
    प्रत्यागम c == '/' || c == '\\';
}

cv::String join(ध्रुव cv::String& आधारः, ध्रुव cv::String& मार्गः)
{
    यदि (आधारः.empty())
        प्रत्यागम मार्गः;
    यदि (मार्गः.empty())
        प्रत्यागम आधारः;

    तार्किक आधारः_Sep = is_मार्गः_Separator(आधारः[आधारः.size() - 1]);
    तार्किक मार्गः_Sep = is_मार्गः_Separator(मार्गः[0]);
    String परिणाम;
    यदि (आधारः_Sep && मार्गः_Sep)
    {
        परिणाम = आधारः + मार्गः.substr(1);
    }
    अन्यथा यदि (!आधारः_Sep && !मार्गः_Sep)
    {
        परिणाम = आधारः + native_separator + मार्गः;
    }
    अन्यथा
    {
        परिणाम = आधारः + मार्गः;
    }
    प्रत्यागम परिणाम;
}

CV_EXPORTS cv::String getParent(ध्रुव cv::String &मार्गः)
{
    std::string::size_type loc = मार्गः.find_last_of("/\\");
    यदि (loc == std::string::npos)
        प्रत्यागम std::string();
    प्रत्यागम std::string(मार्गः, 0, loc);
}

CV_EXPORTS std::wstring getParent(ध्रुव std::wstring& मार्गः)
{
    std::wstring::size_type loc = मार्गः.find_last_of(L"/\\");
    यदि (loc == std::wstring::npos)
        प्रत्यागम std::wstring();
    प्रत्यागम std::wstring(मार्गः, 0, loc);
}

#यदि OPENCV_HAVE_FILESYSTEM_SUPPORT

cv::String canonical(ध्रुव cv::String& मार्गः)
{
    cv::String परिणाम;
#यदि_परिभाषितम् _WIN32
    ध्रुव वर्ण* परिणाम_str = _fullpath(रिक्त, मार्गः.c_str(), 0);
#अन्यथा
    ध्रुव वर्ण* परिणाम_str = realpath(मार्गः.c_str(), रिक्त);
#यदि_अन्तः
    यदि (परिणाम_str)
    {
        परिणाम = cv::String(परिणाम_str);
        मुक्त((शून्य*)परिणाम_str);
    }
    प्रत्यागम परिणाम.empty() ? मार्गः : परिणाम;
}


तार्किक exists(ध्रुव cv::String& मार्गः)
{
    CV_INSTRUMENT_REGION();

#यदि defined _WIN32 || defined WINCE
    BOOL स्थितिः = TRUE;
    {
        WIN32_FILE_ATTRIBUTE_दत्त all_attrs;
#यदि_परिभाषितम् WINRT
        wchar_t wpath[MAX_मार्गः];
        size_t copied = mbstowcs(wpath, मार्गः.c_str(), MAX_मार्गः);
        CV_Assert((copied != MAX_मार्गः) && (copied != (size_t)-1));
        स्थितिः = ::Get_File_गुणाः_Ex_W(wpath, GetFileExInfoStandard, &all_attrs);
#अन्यथा
        स्थितिः = ::Get_File_गुणाः_Ex_A(मार्गः.c_str(), GetFileExInfoStandard, &all_attrs);
#यदि_अन्तः
    }

    प्रत्यागम !!स्थितिः;
#अन्यथा
    संरचना stat stat_buf;
    प्रत्यागम (0 == stat(मार्गः.c_str(), &stat_buf));
#यदि_अन्तः
}

CV_EXPORTS शून्य remove_all(ध्रुव cv::String& मार्गः)
{
    यदि (!exists(मार्गः))
        प्रत्यागम;
    यदि (isDirectory(मार्गः))
    {
        std::vector<String> entries;
        utils::fs::glob(मार्गः, cv::String(), entries, असत्य, सत्य);
        प्रति (size_t i = 0; i < entries.size(); i++)
        {
            ध्रुव String& e = entries[i];
            remove_all(e);
        }
#यदि_परिभाषितम् _MSC_VER
        तार्किक परिणाम = _rmdir(मार्गः.c_str()) == 0;
#अन्यथा
        तार्किक परिणाम = rmdir(मार्गः.c_str()) == 0;
#यदि_अन्तः
        यदि (!परिणाम)
        {
            CV_LOG_ERROR(रिक्त, "Can't remove directory: " << मार्गः);
        }
    }
    अन्यथा
    {
#यदि_परिभाषितम् _MSC_VER
        तार्किक परिणाम = _unlink(मार्गः.c_str()) == 0;
#अन्यथा
        तार्किक परिणाम = unlink(मार्गः.c_str()) == 0;
#यदि_अन्तः
        यदि (!परिणाम)
        {
            CV_LOG_ERROR(रिक्त, "Can't remove file: " << मार्गः);
        }
    }
}


cv::String getcwd()
{
    CV_INSTRUMENT_REGION();
    cv::Auto_बफर<वर्ण, 4096> buf;
#यदि defined WIN32 || defined _WIN32 || defined WINCE
#यदि_परिभाषितम् WINRT
    प्रत्यागम cv::String();
#अन्यथा
    DWORD sz = GetCurrentDirectoryA(0, रिक्त);
    buf.allocate((size_t)sz);
    sz = GetCurrentDirectoryA((DWORD)buf.size(), buf.दत्त());
    प्रत्यागम cv::String(buf.दत्त(), (size_t)sz);
#यदि_अन्तः
#नोचेद्यदि defined __linux__ || defined __APPLE__ || defined __HAIKU__ || defined __FreeBSD__ || defined __EMSCRIPTEN__ || defined __QNX__ || defined __CYGWIN__
    प्रति(;;)
    {
        वर्ण* p = ::getcwd(buf.दत्त(), buf.size());
        यदि (p == रिक्त)
        {
            यदि (errno == ERANGE)
            {
                buf.allocate(buf.size() * 2);
                अनुवर्त;
            }
            प्रत्यागम cv::String();
        }
        भङ्ग;
    }
    प्रत्यागम cv::String(buf.दत्त(), (size_t)वाक्यदैर्घ्यम्(buf.दत्त()));
#अन्यथा
    प्रत्यागम cv::String();
#यदि_अन्तः
}


तार्किक createDirectory(ध्रुव cv::String& मार्गः)
{
    CV_INSTRUMENT_REGION();
#यदि defined WIN32 || defined _WIN32 || defined WINCE
#यदि_परिभाषितम् WINRT
    wchar_t wpath[MAX_मार्गः];
    size_t copied = mbstowcs(wpath, मार्गः.c_str(), MAX_मार्गः);
    CV_Assert((copied != MAX_मार्गः) && (copied != (size_t)-1));
    पूर्णाङ्क परिणाम = CreateDirectoryA(wpath, रिक्त) ? 0 : -1;
#अन्यथा
    पूर्णाङ्क परिणाम = _mkdir(मार्गः.c_str());
#यदि_अन्तः
#नोचेद्यदि defined __linux__ || defined __APPLE__ || defined __HAIKU__ || defined __FreeBSD__ || defined __EMSCRIPTEN__ || defined __QNX__ || defined __CYGWIN__
    पूर्णाङ्क परिणाम = mkdir(मार्गः.c_str(), 0777);
#अन्यथा
    पूर्णाङ्क परिणाम = -1;
#यदि_अन्तः

    यदि (परिणाम == -1)
    {
        प्रत्यागम isDirectory(मार्गः);
    }
    प्रत्यागम सत्य;
}

तार्किक createDirectories(ध्रुव cv::String& मार्गः_3)
{
    cv::String मार्गः = मार्गः_3;
    प्रति (;;)
    {
        वर्ण चरमः_char = मार्गः.empty() ? 0 : मार्गः[मार्गः.length() - 1];
        यदि (is_मार्गः_Separator(चरमः_char))
        {
            मार्गः = मार्गः.substr(0, मार्गः.length() - 1);
            अनुवर्त;
        }
        भङ्ग;
    }

    यदि (मार्गः.empty() || मार्गः == "./" || मार्गः == ".\\" || मार्गः == ".")
        प्रत्यागम सत्य;
    यदि (isDirectory(मार्गः))
        प्रत्यागम सत्य;

    size_t pos = मार्गः.rfind('/');
    यदि (pos == cv::String::npos)
        pos = मार्गः.rfind('\\');
    यदि (pos != cv::String::npos)
    {
        cv::String parent_directory = मार्गः.substr(0, pos);
        यदि (!parent_directory.empty())
        {
            यदि (!createDirectories(parent_directory))
                प्रत्यागम असत्य;
        }
    }

    प्रत्यागम createDirectory(मार्गः);
}

#यदि_परिभाषितम् _WIN32

संरचना File_तालकम्::Impl
{
    Impl(ध्रुव वर्ण* fname)
    {
        // http://support.microsoft.com/kb/316609
        पूर्णाङ्क अङ्कः_Retries = 5;
        कुरु
        {
            handle = ::CreateFileA(fname, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, रिक्त,
                                OPEN_EXISTING, FILE_ATTRIBUTE_प्रसामान्यम्, रिक्त);
            यदि (INVALID_HANDLE_मानम् == handle)
            {
                यदि (ERROR_SHARING_VIOLATION == Get_चरमः_Error())
                {
                    अङ्कः_Retries--;
                    Sleep(250);
                    अनुवर्त;
                }
                अन्यथा
                {
                    CV_Error_(Error::StsAssert, ("Can't open lock file: %s", fname));
                }
            }
            भङ्ग;
        } यावत् (अङ्कः_Retries > 0);
    }
    ~Impl()
    {
        यदि (INVALID_HANDLE_मानम् != handle)
        {
            ::CloseHandle(handle);
        }
    }

    तार्किक lock()
    {
        OVERLAPPED overlapped;
        std::स्मृतिं_पूरय(&overlapped, 0, परिमाण(overlapped));
        प्रत्यागम !!::तालकम्_File_Ex(handle, LOCKFILE_EXCLUSIVE_तालकम्, 0, MAXDWORD, MAXDWORD, &overlapped);
    }
    तार्किक unlock()
    {
        OVERLAPPED overlapped;
        std::स्मृतिं_पूरय(&overlapped, 0, परिमाण(overlapped));
        प्रत्यागम !!::UnlockFileEx(handle, 0, MAXDWORD, MAXDWORD, &overlapped);
    }

    तार्किक तालकम्_shared()
    {
        OVERLAPPED overlapped;
        std::स्मृतिं_पूरय(&overlapped, 0, परिमाण(overlapped));
        प्रत्यागम !!::तालकम्_File_Ex(handle, 0, 0, MAXDWORD, MAXDWORD, &overlapped);
    }
    तार्किक unlock_shared()
    {
        प्रत्यागम unlock();
    }

    HANDLE handle;

गुप्त:
    Impl(ध्रुव Impl&); // disabled
    Impl& संक्रियकः=(ध्रुव Impl&); // disabled
};

#नोचेद्यदि defined __linux__ || defined __APPLE__ || defined __HAIKU__ || defined __FreeBSD__ || defined __GNU__ || defined __EMSCRIPTEN__ || defined __QNX__ || defined __CYGWIN__

संरचना File_तालकम्::Impl
{
    Impl(ध्रुव वर्ण* fname)
    {
        handle = ::उद्घाटय(fname, O_RDWR);
        CV_Assert(handle != -1);
    }
    ~Impl()
    {
        यदि (handle >= 0)
            ::close(handle);
    }

    तार्किक lock()
    {
        संरचना ::flock l;
        std::स्मृतिं_पूरय(&l, 0, परिमाण(l));
        l.l_type = F_WRLCK;
        l.l_whence = SEEK_SET;
        l.l_start = 0;
        l.l_len = 0;
        DBG(std::निर्गमः << "Lock..." << std::पङ्क्त्यन्तः);
        तार्किक res = -1 != ::fcntl(handle, F_SETLKW, &l);
        प्रत्यागम res;
    }
    तार्किक unlock()
    {
        संरचना ::flock l;
        std::स्मृतिं_पूरय(&l, 0, परिमाण(l));
        l.l_type = F_UNLCK;
        l.l_whence = SEEK_SET;
        l.l_start = 0;
        l.l_len = 0;
        DBG(std::निर्गमः << "Unlock..." << std::पङ्क्त्यन्तः);
        तार्किक res = -1 != ::fcntl(handle, F_SETLK, &l);
        प्रत्यागम res;
    }

    तार्किक तालकम्_shared()
    {
        संरचना ::flock l;
        std::स्मृतिं_पूरय(&l, 0, परिमाण(l));
        l.l_type = F_RDLCK;
        l.l_whence = SEEK_SET;
        l.l_start = 0;
        l.l_len = 0;
        DBG(std::निर्गमः << "Lock read..." << std::पङ्क्त्यन्तः);
        तार्किक res = -1 != ::fcntl(handle, F_SETLKW, &l);
        प्रत्यागम res;
    }
    तार्किक unlock_shared()
    {
        प्रत्यागम unlock();
    }

    पूर्णाङ्क handle;

गुप्त:
    Impl(ध्रुव Impl&); // disabled
    Impl& संक्रियकः=(ध्रुव Impl&); // disabled
};

#यदि_अन्तः

File_तालकम्::File_तालकम्(ध्रुव वर्ण* fname)
    : pImpl(नूतन Impl(fname))
{
    // nothing
}
File_तालकम्::~File_तालकम्()
{
    विलोपय pImpl;
    pImpl = रिक्त;
}

शून्य File_तालकम्::lock() { CV_Assert(pImpl->lock()); }
शून्य File_तालकम्::unlock() { CV_Assert(pImpl->unlock()); }
शून्य File_तालकम्::तालकम्_shared() { CV_Assert(pImpl->तालकम्_shared()); }
शून्य File_तालकम्::unlock_shared() { CV_Assert(pImpl->unlock_shared()); }



cv::String getCacheDirectory(ध्रुव वर्ण* sub_directory_name, ध्रुव वर्ण* configuration_name)
{
    String cache_मार्गः_2;
    यदि (configuration_name)
    {
        cache_मार्गः_2 = utils::getConfigurationParameterString(configuration_name, "");
    }
    यदि (cache_मार्गः_2.empty())
    {
        cv::String default_cache_मार्गः;
#यदि_परिभाषितम् _WIN32
        wchar_t tmp_मार्गः_buf_w[MAX_मार्गः+1] = {0};
        DWORD res = Get_अस्थायी_मार्गः_W(MAX_मार्गः, tmp_मार्गः_buf_w);
        यदि (res > 0 && res <= MAX_मार्गः)
        {
            // Convert from UTF-16 to UTF-8 to support Unicode paths
            पूर्णाङ्क len = Wide_Char_गन्तव्यम्_Multi_Byte(CP_UTF8, 0, tmp_मार्गः_buf_w, -1, रिक्त, 0, रिक्त, रिक्त);
            यदि (len > 0)
            {
                std::vector<वर्ण> utf8_buf(len);
                Wide_Char_गन्तव्यम्_Multi_Byte(CP_UTF8, 0, tmp_मार्गः_buf_w, -1, utf8_buf.दत्त(), len, रिक्त, रिक्त);
                default_cache_मार्गः = std::string(utf8_buf.दत्त());
            }
        }
#नोचेद्यदि defined __ANDROID__
        // no defaults
#नोचेद्यदि defined __APPLE__
        ध्रुव std::string tmpdir_env = utils::getConfigurationParameterString("TMPDIR");
        यदि (!tmpdir_env.empty() && utils::fs::isDirectory(tmpdir_env))
        {
            default_cache_मार्गः = tmpdir_env;
        }
        अन्यथा
        {
            default_cache_मार्गः = "/tmp/";
            CV_LOG_WARNING(रिक्त, "Using world accessible cache directory. This may be not secure: " << default_cache_मार्गः);
        }
#नोचेद्यदि defined __linux__ || defined __HAIKU__ || defined __FreeBSD__ || defined __GNU__
        // https://specifications.freedesktop.org/basedir-spec/basedir-spec-latest.html
        यदि (default_cache_मार्गः.empty())
        {
            ध्रुव std::string xdg_cache_env = utils::getConfigurationParameterString("XDG_CACHE_HOME");
            यदि (!xdg_cache_env.empty() && utils::fs::isDirectory(xdg_cache_env))
            {
                default_cache_मार्गः = xdg_cache_env;
            }
        }
        यदि (default_cache_मार्गः.empty())
        {
            ध्रुव std::string home_env = utils::getConfigurationParameterString("HOME");
            यदि (!home_env.empty() && utils::fs::isDirectory(home_env))
            {
                cv::String home_मार्गः = home_env;
                cv::String home_cache_मार्गः = utils::fs::join(home_मार्गः, ".cache/");
                यदि (utils::fs::isDirectory(home_cache_मार्गः))
                {
                    default_cache_मार्गः = home_cache_मार्गः;
                }
            }
        }
        यदि (default_cache_मार्गः.empty())
        {
            ध्रुव वर्ण* अस्थायी_मार्गः = "/var/tmp/";
            यदि (utils::fs::isDirectory(अस्थायी_मार्गः))
            {
                default_cache_मार्गः = अस्थायी_मार्गः;
                CV_LOG_WARNING(रिक्त, "Using world accessible cache directory. This may be not secure: " << default_cache_मार्गः);
            }
        }
        यदि (default_cache_मार्गः.empty())
        {
            default_cache_मार्गः = "/tmp/";
            CV_LOG_WARNING(रिक्त, "Using world accessible cache directory. This may be not secure: " << default_cache_मार्गः);
        }
#अन्यथा
        // no defaults
#यदि_अन्तः
        CV_LOG_VERBOSE(रिक्त, 0, "default_cache_path = " << default_cache_मार्गः);
        यदि (!default_cache_मार्गः.empty())
        {
            यदि (utils::fs::isDirectory(default_cache_मार्गः))
            {
                cv::String default_cache_मार्गः_आधारः = utils::fs::join(default_cache_मार्गः, "opencv");
                default_cache_मार्गः = utils::fs::join(default_cache_मार्गः_आधारः, CVAUX_STR(CV_संस्करणम्_MAJOR) ".x" CV_संस्करणम्_स्थितिः);
                यदि (utils::getConfigurationParameterBool("OPENCV_CACHE_SHOW_CLEANUP_MESSAGE", सत्य)
                    && !utils::fs::isDirectory(default_cache_मार्गः))
                {
                    std::vector<cv::String> existedCacheDirs;
                    यत्न
                    {
                        utils::fs::glob_relative(default_cache_मार्गः_आधारः, "*", existedCacheDirs, असत्य, सत्य);
                    }
                    गृहाण (...)
                    {
                        // ignore
                    }
                    यदि (!existedCacheDirs.empty())
                    {
                        CV_LOG_WARNING(रिक्त, "Creating new OpenCV cache directory: " << default_cache_मार्गः);
                        CV_LOG_WARNING(रिक्त, "There are several neighbour directories, probably created by old OpenCV versions.");
                        CV_LOG_WARNING(रिक्त, "Feel free to cleanup these unused directories:");
                        प्रति (size_t i = 0; i < existedCacheDirs.size(); i++)
                        {
                            CV_LOG_WARNING(रिक्त, "  - " << existedCacheDirs[i]);
                        }
                        CV_LOG_WARNING(रिक्त, "Note: This message is showed only once.");
                    }
                }
                यदि (sub_directory_name && sub_directory_name[0] != '\0')
                    default_cache_मार्गः = utils::fs::join(default_cache_मार्गः, cv::String(sub_directory_name) + native_separator);
                यदि (!utils::fs::createDirectories(default_cache_मार्गः))
                {
                    CV_LOG_DEBUG(रिक्त, "Can't create OpenCV cache sub-directory: " << default_cache_मार्गः);
                }
                अन्यथा
                {
                    cache_मार्गः_2 = default_cache_मार्गः;
                }
            }
            अन्यथा
            {
                CV_LOG_INFO(रिक्त, "Can't find default cache directory (does it exist?): " << default_cache_मार्गः);
            }
        }
        अन्यथा
        {
            CV_LOG_DEBUG(रिक्त, "OpenCV has no support to discover default cache directory on the current platform");
        }
    }
    अन्यथा
    {
        यदि (cache_मार्गः_2 == "disabled")
            प्रत्यागम cache_मार्गः_2;
        यदि (!isDirectory(cache_मार्गः_2))
        {
            CV_LOG_WARNING(रिक्त, "Specified non-existed directory, creating OpenCV sub-directory for caching purposes: " << cache_मार्गः_2);
            यदि (!createDirectories(cache_मार्गः_2))
            {
                CV_LOG_ERROR(रिक्त, "Can't create OpenCV cache sub-directory: " << cache_मार्गः_2);
                cache_मार्गः_2.clear();
            }
        }
    }
    CV_Assert(cache_मार्गः_2.empty() || utils::fs::isDirectory(cache_मार्गः_2));
    यदि (!cache_मार्गः_2.empty())
    {
        यदि (!is_मार्गः_Separator(cache_मार्गः_2[cache_मार्गः_2.size() - 1]))
        {
            cache_मार्गः_2 += native_separator;
        }
    }
    प्रत्यागम cache_मार्गः_2;
}

#अन्यथा
#परिभाषय NOT_IMPLEMENTED CV_Error(Error::StsNotImplemented, "");
cv::String canonical(ध्रुव cv::String& /*मार्गः*/) { NOT_IMPLEMENTED }
तार्किक exists(ध्रुव cv::String& /*मार्गः*/) { NOT_IMPLEMENTED }
शून्य remove_all(ध्रुव cv::String& /*मार्गः*/) { NOT_IMPLEMENTED }
cv::String getcwd() { NOT_IMPLEMENTED }
तार्किक createDirectory(ध्रुव cv::String& /*मार्गः*/) { NOT_IMPLEMENTED }
तार्किक createDirectories(ध्रुव cv::String& /*मार्गः*/) { NOT_IMPLEMENTED }
cv::String getCacheDirectory(ध्रुव वर्ण* /*sub_directory_name*/, ध्रुव वर्ण* /*configuration_name = NULL*/) { NOT_IMPLEMENTED }
#अपरिभाषय NOT_IMPLEMENTED
#यदि_अन्तः // OPENCV_HAVE_FILESYSTEM_SUPPORT

}}} // namespace


#यदि OPENCV_HAVE_FILESYSTEM_SUPPORT
#अन्तर्भाव "plugin_loader.impl.hpp"
#यदि_अन्तः
