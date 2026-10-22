/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                           License Agreement
//                For Open Source Computer Vision Library
//
// Copyright (C) 2000-2008, Intel Corporation, all rights reserved.
// Copyright (C) 2008-2013, Willow Garage Inc., all rights reserved.
// Third party copyrights are property of their respective owners.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//   * Redistribution's of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//   * Redistribution's in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and / or other materials provided with the distribution.
//
//   * The name of the copyright holders may not be used to endorse or promote products
//     derived from this software without specific prior written permission.
//
// This software is provided by the copyright holders and contributors "as is" and
// any express or implied warranties, including, but not limited to, the implied
// warranties of merchantability and fitness for a particular purpose are disclaimed.
// In no event shall the Intel Corporation or contributors be liable for any direct,
// indirect, incidental, special, exemplary, or consequential damages
// (including, but not limited to, procurement of substitute goods or services;
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//
//M*/

#അന്തര്ഭാവ "precomp.hpp"

#അന്തര്ഭാവ "opencv2/core/utils/filesystem.hpp"
#അന്തര്ഭാവ "opencv2/core/utils/filesystem.private.hpp"

#എങ്കിൽ OPENCV_HAVE_FILESYSTEM_SUPPORT
#എങ്കിൽ defined _WIN32 || defined WINCE
# include <windows.h>
സ്ഥിരം അക്ഷരം dir_separators[] = "/\\";

നാമാകാശ
{
    ഘടന dirent
    {
        സ്ഥിരം അക്ഷരം* d_name;
    };

    ഘടന DIR
    {
#എങ്കിൽ defined(WINRT) || defined(_WIN32_WCE)
        WIN32_FIND_DATAW data;
#അല്ലെങ്കിൽ
        WIN32_FIND_DATAA data;
#endif
        HANDLE handle;
        dirent ent;
#എങ്കിൽ defined(WINRT) || defined(_WIN32_WCE)
        DIR() { }
        ~DIR()
        {
            എങ്കിൽ (ent.d_name)
                delete[] ent.d_name;
        }
#endif
    };

    DIR* opendir(സ്ഥിരം അക്ഷരം* path)
    {
        DIR* dir = പുതിയ DIR;
        dir->ent.d_name = 0;
#എങ്കിൽ defined(WINRT) || defined(_WIN32_WCE)
        cv::String full_path = cv::String(path) + "\\*";
        wchar_t wfull_path[MAX_PATH];
        size_t copied = mbstowcs(wfull_path, full_path.c_str(), MAX_PATH);
        CV_Assert((copied != MAX_PATH) && (copied != (size_t)-1));
        dir->handle = ::FindFirstFileExW(wfull_path, FindExInfoStandard,
                        &dir->data, Find_Ex_തിരയൽ_Name_Match, ശൂന്യ, 0);
#അല്ലെങ്കിൽ
        dir->handle = ::FindFirstFileExA((cv::String(path) + "\\*").c_str(),
            FindExInfoStandard, &dir->data, Find_Ex_തിരയൽ_Name_Match, ശൂന്യ, 0);
#endif
        എങ്കിൽ(dir->handle == INVALID_HANDLE_മൂല്യം)
        {
            /*closedir will do all cleanup*/
            delete dir;
            തിരികെ 0;
        }
        തിരികെ dir;
    }

    dirent* readdir(DIR* dir)
    {
#എങ്കിൽ defined(WINRT) || defined(_WIN32_WCE)
        എങ്കിൽ (dir->ent.d_name != 0)
        {
            എങ്കിൽ (::FindNextFileW(dir->handle, &dir->data) != TRUE)
                തിരികെ 0;
        }
        size_t asize = wcstombs(ശൂന്യ, dir->data.cFileName, 0);
        CV_Assert((asize != 0) && (asize != (size_t)-1));
        അക്ഷരം* aname = പുതിയ അക്ഷരം[asize+1];
        aname[asize] = 0;
        wcstombs(aname, dir->data.cFileName, asize);
        delete[] dir->ent.d_name;
        dir->ent.d_name = aname;
#അല്ലെങ്കിൽ
        എങ്കിൽ (dir->ent.d_name != 0)
        {
            എങ്കിൽ (::FindNextFileA(dir->handle, &dir->data) != TRUE)
                തിരികെ 0;
        }
        dir->ent.d_name = dir->data.cFileName;
#endif
        തിരികെ &dir->ent;
    }

    ശൂന്യം closedir(DIR* dir)
    {
        ::FindClose(dir->handle);
        delete dir;
    }


}
#അല്ലെങ്കിൽ // defined _WIN32 || defined WINCE
# include <dirent.h>
# include <sys/stat.h>
സ്ഥിരം അക്ഷരം dir_separators[] = "/";
#endif // defined _WIN32 || defined WINCE
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT


#എങ്കിൽ OPENCV_HAVE_FILESYSTEM_SUPPORT
സ്ഥിതം തർക്കം isDir(സ്ഥിരം cv::String& path, DIR* dir)
{
#എങ്കിൽ defined _WIN32 || defined _WIN32_WCE
    DWORD attributes;
    BOOL status = TRUE;
    എങ്കിൽ (dir)
        attributes = dir->data.dwFileAttributes;
    അല്ലെങ്കിൽ
    {
        WIN32_FILE_ATTRIBUTE_DATA all_attrs;
#എങ്കിൽ defined WINRT || defined _WIN32_WCE
        wchar_t wpath[MAX_PATH];
        size_t copied = mbstowcs(wpath, path.c_str(), MAX_PATH);
        CV_Assert((copied != MAX_PATH) && (copied != (size_t)-1));
        status = ::GetFileAttributesExW(wpath, GetFileExInfoStandard, &all_attrs);
#അല്ലെങ്കിൽ
        status = ::GetFileAttributesExA(path.c_str(), GetFileExInfoStandard, &all_attrs);
#endif
        attributes = all_attrs.dwFileAttributes;
    }

    തിരികെ status && ((attributes & FILE_ATTRIBUTE_DIRECTORY) != 0);
#അല്ലെങ്കിൽ
    CV_UNUSED(dir);
    ഘടന stat stat_buf;
    എങ്കിൽ (0 != stat( path.c_str(), &stat_buf))
        തിരികെ അസത്യം;
    പൂർണ്ണസംഖ്യ is_dir = S_ISDIR( stat_buf.st_mode);
    തിരികെ is_dir != 0;
#endif
}
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT

തർക്കം cv::utils::fs::isDirectory(സ്ഥിരം cv::String& path)
{
#എങ്കിൽ OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_INSTRUMENT_REGION();
    തിരികെ isDir(path, ശൂന്യ);
#അല്ലെങ്കിൽ
    CV_UNUSED(path);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#endif
}

#എങ്കിൽ OPENCV_HAVE_FILESYSTEM_SUPPORT
സ്ഥിതം തർക്കം wildcmp(സ്ഥിരം അക്ഷരം *string, സ്ഥിരം അക്ഷരം *wild)
{
    // Based on wildcmp written by Jack Handy - <A href="mailto:jakkhandy@hotmail.com">jakkhandy@hotmail.com</A>
    സ്ഥിരം അക്ഷരം *cp = 0, *mp = 0;

    ആകുമ്പോൾ ((*string) && (*wild != '*'))
    {
        എങ്കിൽ ((*wild != *string) && (*wild != '?'))
        {
            തിരികെ അസത്യം;
        }

        wild++;
        string++;
    }

    ആകുമ്പോൾ (*string)
    {
        എങ്കിൽ (*wild == '*')
        {
            എങ്കിൽ (!*++wild)
            {
                തിരികെ സത്യം;
            }

            mp = wild;
            cp = string + 1;
        }
        അല്ലെങ്കിൽ എങ്കിൽ ((*wild == *string) || (*wild == '?'))
        {
            wild++;
            string++;
        }
        അല്ലെങ്കിൽ
        {
            wild = mp;
            string = cp++;
        }
    }

    ആകുമ്പോൾ (*wild == '*')
    {
        wild++;
    }

    തിരികെ *wild == 0;
}

സ്ഥിതം ശൂന്യം glob_rec(സ്ഥിരം cv::String& directory, സ്ഥിരം cv::String& wildchart, std::vector<cv::String>& result,
        തർക്കം recursive, തർക്കം includeDirectories, സ്ഥിരം cv::String& pathPrefix)
{
    DIR *dir;

    എങ്കിൽ ((dir = opendir (directory.c_str())) != 0)
    {
        /* find all the files and directories within directory */
        ശ്രമിക്കുക
        {
            ഘടന dirent *ent;
            ആകുമ്പോൾ ((ent = readdir (dir)) != 0)
            {
                സ്ഥിരം അക്ഷരം* name = ent->d_name;
                എങ്കിൽ((name[0] == 0) || (name[0] == '.' && name[1] == 0) || (name[0] == '.' && name[1] == '.' && name[2] == 0))
                    തുടരുക;

                cv::String path = cv::utils::fs::join(directory, name);
                cv::String entry = cv::utils::fs::join(pathPrefix, name);

                എങ്കിൽ (isDir(path, dir))
                {
                    എങ്കിൽ (recursive)
                        glob_rec(path, wildchart, result, recursive, includeDirectories, entry);
                    എങ്കിൽ (!includeDirectories)
                        തുടരുക;
                }

                എങ്കിൽ (wildchart.empty() || wildcmp(name, wildchart.c_str()))
                    result.push_back(entry);
            }
        }
        പിടിക്കുക (...)
        {
            closedir(dir);
            എറിയുക;
        }
        closedir(dir);
    }
    അല്ലെങ്കിൽ
    {
        CV_Error_(cv::Error::StsObjectNotFound, ("could not open directory: %s", directory.c_str()));
    }
}
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT

ശൂന്യം cv::glob(String pattern, std::vector<String>& result, തർക്കം recursive)
{
#എങ്കിൽ OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_INSTRUMENT_REGION();

    result.clear();
    String path, wildchart;

    എങ്കിൽ (isDir(pattern, 0))
    {
        എങ്കിൽ(strchr(dir_separators, pattern[pattern.size() - 1]) != 0)
        {
            path = pattern.substr(0, pattern.size() - 1);
        }
        അല്ലെങ്കിൽ
        {
            path = pattern;
        }
    }
    അല്ലെങ്കിൽ
    {
        size_t pos = pattern.find_last_of(dir_separators);
        എങ്കിൽ (pos == String::npos)
        {
            wildchart = pattern;
            path = ".";
        }
        അല്ലെങ്കിൽ
        {
            path = pattern.substr(0, pos);
            wildchart = pattern.substr(pos + 1);
        }
    }

    glob_rec(path, wildchart, result, recursive, അസത്യം, path);
    std::sort(result.begin(), result.end());
#അല്ലെങ്കിൽ // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_UNUSED(pattern);
    CV_UNUSED(result);
    CV_UNUSED(recursive);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT
}

ശൂന്യം cv::utils::fs::glob(സ്ഥിരം cv::String& directory, സ്ഥിരം cv::String& pattern,
        std::vector<cv::String>& result,
        തർക്കം recursive, തർക്കം includeDirectories)
{
#എങ്കിൽ OPENCV_HAVE_FILESYSTEM_SUPPORT
    glob_rec(directory, pattern, result, recursive, includeDirectories, directory);
    std::sort(result.begin(), result.end());
#അല്ലെങ്കിൽ // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_UNUSED(directory);
    CV_UNUSED(pattern);
    CV_UNUSED(result);
    CV_UNUSED(recursive);
    CV_UNUSED(includeDirectories);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT
}

ശൂന്യം cv::utils::fs::glob_relative(സ്ഥിരം cv::String& directory, സ്ഥിരം cv::String& pattern,
        std::vector<cv::String>& result,
        തർക്കം recursive, തർക്കം includeDirectories)
{
#എങ്കിൽ OPENCV_HAVE_FILESYSTEM_SUPPORT
    glob_rec(directory, pattern, result, recursive, includeDirectories, cv::String());
    std::sort(result.begin(), result.end());
#അല്ലെങ്കിൽ // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_UNUSED(directory);
    CV_UNUSED(pattern);
    CV_UNUSED(result);
    CV_UNUSED(recursive);
    CV_UNUSED(includeDirectories);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT
}
