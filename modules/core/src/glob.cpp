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

#અન્તર્ભાવ "precomp.hpp"

#અન્તર્ભાવ "opencv2/core/utils/filesystem.hpp"
#અન્તર્ભાવ "opencv2/core/utils/filesystem.private.hpp"

#જો OPENCV_HAVE_FILESYSTEM_SUPPORT
#જો defined _WIN32 || defined WINCE
# include <windows.h>
અચલ અક્ષર dir_separators[] = "/\\";

નામાકાશ
{
    સંરચના dirent
    {
        અચલ અક્ષર* d_name;
    };

    સંરચના DIR
    {
#જો defined(WINRT) || defined(_WIN32_WCE)
        WIN32_FIND_DATAW data;
#નહીંતર
        WIN32_FIND_DATAA data;
#endif
        HANDLE handle;
        dirent ent;
#જો defined(WINRT) || defined(_WIN32_WCE)
        DIR() { }
        ~DIR()
        {
            જો (ent.d_name)
                delete[] ent.d_name;
        }
#endif
    };

    DIR* opendir(અચલ અક્ષર* path)
    {
        DIR* dir = નવું DIR;
        dir->ent.d_name = 0;
#જો defined(WINRT) || defined(_WIN32_WCE)
        cv::String full_path = cv::String(path) + "\\*";
        wchar_t wfull_path[MAX_PATH];
        size_t copied = mbstowcs(wfull_path, full_path.c_str(), MAX_PATH);
        CV_Assert((copied != MAX_PATH) && (copied != (size_t)-1));
        dir->handle = ::FindFirstFileExW(wfull_path, FindExInfoStandard,
                        &dir->data, Find_Ex_શોધ_Name_Match, ખાલી, 0);
#નહીંતર
        dir->handle = ::FindFirstFileExA((cv::String(path) + "\\*").c_str(),
            FindExInfoStandard, &dir->data, Find_Ex_શોધ_Name_Match, ખાલી, 0);
#endif
        જો(dir->handle == INVALID_HANDLE_કિંમત)
        {
            /*closedir will do all cleanup*/
            delete dir;
            પાછા 0;
        }
        પાછા dir;
    }

    dirent* readdir(DIR* dir)
    {
#જો defined(WINRT) || defined(_WIN32_WCE)
        જો (dir->ent.d_name != 0)
        {
            જો (::FindNextFileW(dir->handle, &dir->data) != TRUE)
                પાછા 0;
        }
        size_t asize = wcstombs(ખાલી, dir->data.cFileName, 0);
        CV_Assert((asize != 0) && (asize != (size_t)-1));
        અક્ષર* aname = નવું અક્ષર[asize+1];
        aname[asize] = 0;
        wcstombs(aname, dir->data.cFileName, asize);
        delete[] dir->ent.d_name;
        dir->ent.d_name = aname;
#નહીંતર
        જો (dir->ent.d_name != 0)
        {
            જો (::FindNextFileA(dir->handle, &dir->data) != TRUE)
                પાછા 0;
        }
        dir->ent.d_name = dir->data.cFileName;
#endif
        પાછા &dir->ent;
    }

    શૂન્ય closedir(DIR* dir)
    {
        ::FindClose(dir->handle);
        delete dir;
    }


}
#નહીંતર // defined _WIN32 || defined WINCE
# include <dirent.h>
# include <sys/stat.h>
અચલ અક્ષર dir_separators[] = "/";
#endif // defined _WIN32 || defined WINCE
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT


#જો OPENCV_HAVE_FILESYSTEM_SUPPORT
સ્થિર તાર્કિક isDir(અચલ cv::String& path, DIR* dir)
{
#જો defined _WIN32 || defined _WIN32_WCE
    DWORD attributes;
    BOOL status = TRUE;
    જો (dir)
        attributes = dir->data.dwFileAttributes;
    નહીંતર
    {
        WIN32_FILE_ATTRIBUTE_DATA all_attrs;
#જો defined WINRT || defined _WIN32_WCE
        wchar_t wpath[MAX_PATH];
        size_t copied = mbstowcs(wpath, path.c_str(), MAX_PATH);
        CV_Assert((copied != MAX_PATH) && (copied != (size_t)-1));
        status = ::GetFileAttributesExW(wpath, GetFileExInfoStandard, &all_attrs);
#નહીંતર
        status = ::GetFileAttributesExA(path.c_str(), GetFileExInfoStandard, &all_attrs);
#endif
        attributes = all_attrs.dwFileAttributes;
    }

    પાછા status && ((attributes & FILE_ATTRIBUTE_DIRECTORY) != 0);
#નહીંતર
    CV_UNUSED(dir);
    સંરચના stat stat_buf;
    જો (0 != stat( path.c_str(), &stat_buf))
        પાછા ખોટું;
    પૂર્ણાંક is_dir = S_ISDIR( stat_buf.st_mode);
    પાછા is_dir != 0;
#endif
}
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT

તાર્કિક cv::utils::fs::isDirectory(અચલ cv::String& path)
{
#જો OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_INSTRUMENT_REGION();
    પાછા isDir(path, ખાલી);
#નહીંતર
    CV_UNUSED(path);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#endif
}

#જો OPENCV_HAVE_FILESYSTEM_SUPPORT
સ્થિર તાર્કિક wildcmp(અચલ અક્ષર *string, અચલ અક્ષર *wild)
{
    // Based on wildcmp written by Jack Handy - <A href="mailto:jakkhandy@hotmail.com">jakkhandy@hotmail.com</A>
    અચલ અક્ષર *cp = 0, *mp = 0;

    જ્યારે ((*string) && (*wild != '*'))
    {
        જો ((*wild != *string) && (*wild != '?'))
        {
            પાછા ખોટું;
        }

        wild++;
        string++;
    }

    જ્યારે (*string)
    {
        જો (*wild == '*')
        {
            જો (!*++wild)
            {
                પાછા સાચું;
            }

            mp = wild;
            cp = string + 1;
        }
        નહીંતર જો ((*wild == *string) || (*wild == '?'))
        {
            wild++;
            string++;
        }
        નહીંતર
        {
            wild = mp;
            string = cp++;
        }
    }

    જ્યારે (*wild == '*')
    {
        wild++;
    }

    પાછા *wild == 0;
}

સ્થિર શૂન્ય glob_rec(અચલ cv::String& directory, અચલ cv::String& wildchart, std::vector<cv::String>& result,
        તાર્કિક recursive, તાર્કિક includeDirectories, અચલ cv::String& pathPrefix)
{
    DIR *dir;

    જો ((dir = opendir (directory.c_str())) != 0)
    {
        /* find all the files and directories within directory */
        પ્રયાસ
        {
            સંરચના dirent *ent;
            જ્યારે ((ent = readdir (dir)) != 0)
            {
                અચલ અક્ષર* name = ent->d_name;
                જો((name[0] == 0) || (name[0] == '.' && name[1] == 0) || (name[0] == '.' && name[1] == '.' && name[2] == 0))
                    ચાલુ;

                cv::String path = cv::utils::fs::join(directory, name);
                cv::String entry = cv::utils::fs::join(pathPrefix, name);

                જો (isDir(path, dir))
                {
                    જો (recursive)
                        glob_rec(path, wildchart, result, recursive, includeDirectories, entry);
                    જો (!includeDirectories)
                        ચાલુ;
                }

                જો (wildchart.empty() || wildcmp(name, wildchart.c_str()))
                    result.push_back(entry);
            }
        }
        પકડો (...)
        {
            closedir(dir);
            ફેંકો;
        }
        closedir(dir);
    }
    નહીંતર
    {
        CV_Error_(cv::Error::StsObjectNotFound, ("could not open directory: %s", directory.c_str()));
    }
}
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT

શૂન્ય cv::glob(String pattern, std::vector<String>& result, તાર્કિક recursive)
{
#જો OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_INSTRUMENT_REGION();

    result.clear();
    String path, wildchart;

    જો (isDir(pattern, 0))
    {
        જો(strchr(dir_separators, pattern[pattern.size() - 1]) != 0)
        {
            path = pattern.substr(0, pattern.size() - 1);
        }
        નહીંતર
        {
            path = pattern;
        }
    }
    નહીંતર
    {
        size_t pos = pattern.find_last_of(dir_separators);
        જો (pos == String::npos)
        {
            wildchart = pattern;
            path = ".";
        }
        નહીંતર
        {
            path = pattern.substr(0, pos);
            wildchart = pattern.substr(pos + 1);
        }
    }

    glob_rec(path, wildchart, result, recursive, ખોટું, path);
    std::sort(result.begin(), result.end());
#નહીંતર // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_UNUSED(pattern);
    CV_UNUSED(result);
    CV_UNUSED(recursive);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT
}

શૂન્ય cv::utils::fs::glob(અચલ cv::String& directory, અચલ cv::String& pattern,
        std::vector<cv::String>& result,
        તાર્કિક recursive, તાર્કિક includeDirectories)
{
#જો OPENCV_HAVE_FILESYSTEM_SUPPORT
    glob_rec(directory, pattern, result, recursive, includeDirectories, directory);
    std::sort(result.begin(), result.end());
#નહીંતર // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_UNUSED(directory);
    CV_UNUSED(pattern);
    CV_UNUSED(result);
    CV_UNUSED(recursive);
    CV_UNUSED(includeDirectories);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT
}

શૂન્ય cv::utils::fs::glob_relative(અચલ cv::String& directory, અચલ cv::String& pattern,
        std::vector<cv::String>& result,
        તાર્કિક recursive, તાર્કિક includeDirectories)
{
#જો OPENCV_HAVE_FILESYSTEM_SUPPORT
    glob_rec(directory, pattern, result, recursive, includeDirectories, cv::String());
    std::sort(result.begin(), result.end());
#નહીંતર // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_UNUSED(directory);
    CV_UNUSED(pattern);
    CV_UNUSED(result);
    CV_UNUSED(recursive);
    CV_UNUSED(includeDirectories);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT
}
