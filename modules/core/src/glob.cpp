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

#include "precomp.hpp"

#include "opencv2/core/utils/filesystem.hpp"
#include "opencv2/core/utils/filesystem.private.hpp"

#만약 OPENCV_HAVE_FILESYSTEM_SUPPORT
#만약 defined _WIN32 || defined WINCE
# include <windows.h>
const char dir_separators[] = "/\\";

namespace
{
    struct dirent
    {
        const char* d_name;
    };

    struct DIR
    {
#만약 defined(WINRT) || defined(_WIN32_WCE)
        WIN32_FIND_DATAW data;
#아니면
        WIN32_FIND_DATAA data;
#endif
        HANDLE handle;
        dirent ent;
#만약 defined(WINRT) || defined(_WIN32_WCE)
        DIR() { }
        ~DIR()
        {
            만약 (ent.d_name)
                delete[] ent.d_name;
        }
#endif
    };

    DIR* opendir(const char* path)
    {
        DIR* dir = new DIR;
        dir->ent.d_name = 0;
#만약 defined(WINRT) || defined(_WIN32_WCE)
        cv::String full_path = cv::String(path) + "\\*";
        wchar_t wfull_path[MAX_PATH];
        size_t copied = mbstowcs(wfull_path, full_path.c_str(), MAX_PATH);
        CV_Assert((copied != MAX_PATH) && (copied != (size_t)-1));
        dir->handle = ::FindFirstFileExW(wfull_path, FindExInfoStandard,
                        &dir->data, FindExSearchNameMatch, NULL, 0);
#아니면
        dir->handle = ::FindFirstFileExA((cv::String(path) + "\\*").c_str(),
            FindExInfoStandard, &dir->data, FindExSearchNameMatch, NULL, 0);
#endif
        만약(dir->handle == INVALID_HANDLE_VALUE)
        {
            /*closedir will do all cleanup*/
            delete dir;
            반환 0;
        }
        반환 dir;
    }

    dirent* readdir(DIR* dir)
    {
#만약 defined(WINRT) || defined(_WIN32_WCE)
        만약 (dir->ent.d_name != 0)
        {
            만약 (::FindNextFileW(dir->handle, &dir->data) != TRUE)
                반환 0;
        }
        size_t asize = wcstombs(NULL, dir->data.cFileName, 0);
        CV_Assert((asize != 0) && (asize != (size_t)-1));
        char* aname = new char[asize+1];
        aname[asize] = 0;
        wcstombs(aname, dir->data.cFileName, asize);
        delete[] dir->ent.d_name;
        dir->ent.d_name = aname;
#아니면
        만약 (dir->ent.d_name != 0)
        {
            만약 (::FindNextFileA(dir->handle, &dir->data) != TRUE)
                반환 0;
        }
        dir->ent.d_name = dir->data.cFileName;
#endif
        반환 &dir->ent;
    }

    void closedir(DIR* dir)
    {
        ::FindClose(dir->handle);
        delete dir;
    }


}
#아니면 // defined _WIN32 || defined WINCE
# include <dirent.h>
# include <sys/stat.h>
const char dir_separators[] = "/";
#endif // defined _WIN32 || defined WINCE
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT


#만약 OPENCV_HAVE_FILESYSTEM_SUPPORT
static bool isDir(const cv::String& path, DIR* dir)
{
#만약 defined _WIN32 || defined _WIN32_WCE
    DWORD attributes;
    BOOL status = TRUE;
    만약 (dir)
        attributes = dir->data.dwFileAttributes;
    아니면
    {
        WIN32_FILE_ATTRIBUTE_DATA all_attrs;
#만약 defined WINRT || defined _WIN32_WCE
        wchar_t wpath[MAX_PATH];
        size_t copied = mbstowcs(wpath, path.c_str(), MAX_PATH);
        CV_Assert((copied != MAX_PATH) && (copied != (size_t)-1));
        status = ::GetFileAttributesExW(wpath, GetFileExInfoStandard, &all_attrs);
#아니면
        status = ::GetFileAttributesExA(path.c_str(), GetFileExInfoStandard, &all_attrs);
#endif
        attributes = all_attrs.dwFileAttributes;
    }

    반환 status && ((attributes & FILE_ATTRIBUTE_DIRECTORY) != 0);
#아니면
    CV_UNUSED(dir);
    struct stat stat_buf;
    만약 (0 != stat( path.c_str(), &stat_buf))
        반환 false;
    정수 is_dir = S_ISDIR( stat_buf.st_mode);
    반환 is_dir != 0;
#endif
}
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT

bool cv::utils::fs::isDirectory(const cv::String& path)
{
#만약 OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_INSTRUMENT_REGION();
    반환 isDir(path, NULL);
#아니면
    CV_UNUSED(path);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#endif
}

#만약 OPENCV_HAVE_FILESYSTEM_SUPPORT
static bool wildcmp(const char *string, const char *wild)
{
    // Based on wildcmp written by Jack Handy - <A href="mailto:jakkhandy@hotmail.com">jakkhandy@hotmail.com</A>
    const char *cp = 0, *mp = 0;

    동안 ((*string) && (*wild != '*'))
    {
        만약 ((*wild != *string) && (*wild != '?'))
        {
            반환 false;
        }

        wild++;
        string++;
    }

    동안 (*string)
    {
        만약 (*wild == '*')
        {
            만약 (!*++wild)
            {
                반환 true;
            }

            mp = wild;
            cp = string + 1;
        }
        아니면 만약 ((*wild == *string) || (*wild == '?'))
        {
            wild++;
            string++;
        }
        아니면
        {
            wild = mp;
            string = cp++;
        }
    }

    동안 (*wild == '*')
    {
        wild++;
    }

    반환 *wild == 0;
}

static void glob_rec(const cv::String& directory, const cv::String& wildchart, std::vector<cv::String>& result,
        bool recursive, bool includeDirectories, const cv::String& pathPrefix)
{
    DIR *dir;

    만약 ((dir = opendir (directory.c_str())) != 0)
    {
        /* find all the files and directories within directory */
        try
        {
            struct dirent *ent;
            동안 ((ent = readdir (dir)) != 0)
            {
                const char* name = ent->d_name;
                만약((name[0] == 0) || (name[0] == '.' && name[1] == 0) || (name[0] == '.' && name[1] == '.' && name[2] == 0))
                    continue;

                cv::String path = cv::utils::fs::join(directory, name);
                cv::String entry = cv::utils::fs::join(pathPrefix, name);

                만약 (isDir(path, dir))
                {
                    만약 (recursive)
                        glob_rec(path, wildchart, result, recursive, includeDirectories, entry);
                    만약 (!includeDirectories)
                        continue;
                }

                만약 (wildchart.empty() || wildcmp(name, wildchart.c_str()))
                    result.push_back(entry);
            }
        }
        catch (...)
        {
            closedir(dir);
            throw;
        }
        closedir(dir);
    }
    아니면
    {
        CV_Error_(cv::Error::StsObjectNotFound, ("could not open directory: %s", directory.c_str()));
    }
}
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT

void cv::glob(String pattern, std::vector<String>& result, bool recursive)
{
#만약 OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_INSTRUMENT_REGION();

    result.clear();
    String path, wildchart;

    만약 (isDir(pattern, 0))
    {
        만약(strchr(dir_separators, pattern[pattern.size() - 1]) != 0)
        {
            path = pattern.substr(0, pattern.size() - 1);
        }
        아니면
        {
            path = pattern;
        }
    }
    아니면
    {
        size_t pos = pattern.find_last_of(dir_separators);
        만약 (pos == String::npos)
        {
            wildchart = pattern;
            path = ".";
        }
        아니면
        {
            path = pattern.substr(0, pos);
            wildchart = pattern.substr(pos + 1);
        }
    }

    glob_rec(path, wildchart, result, recursive, false, path);
    std::sort(result.begin(), result.end());
#아니면 // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_UNUSED(pattern);
    CV_UNUSED(result);
    CV_UNUSED(recursive);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT
}

void cv::utils::fs::glob(const cv::String& directory, const cv::String& pattern,
        std::vector<cv::String>& result,
        bool recursive, bool includeDirectories)
{
#만약 OPENCV_HAVE_FILESYSTEM_SUPPORT
    glob_rec(directory, pattern, result, recursive, includeDirectories, directory);
    std::sort(result.begin(), result.end());
#아니면 // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_UNUSED(directory);
    CV_UNUSED(pattern);
    CV_UNUSED(result);
    CV_UNUSED(recursive);
    CV_UNUSED(includeDirectories);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT
}

void cv::utils::fs::glob_relative(const cv::String& directory, const cv::String& pattern,
        std::vector<cv::String>& result,
        bool recursive, bool includeDirectories)
{
#만약 OPENCV_HAVE_FILESYSTEM_SUPPORT
    glob_rec(directory, pattern, result, recursive, includeDirectories, cv::String());
    std::sort(result.begin(), result.end());
#아니면 // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_UNUSED(directory);
    CV_UNUSED(pattern);
    CV_UNUSED(result);
    CV_UNUSED(recursive);
    CV_UNUSED(includeDirectories);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT
}
