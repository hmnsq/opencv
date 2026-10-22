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

#ضمن "precomp.hpp"

#ضمن "opencv2/core/utils/filesystem.hpp"
#ضمن "opencv2/core/utils/filesystem.private.hpp"

#ܐܢ OPENCV_HAVE_FILESYSTEM_SUPPORT
#ܐܢ defined _WIN32 || defined WINCE
# include <windows.h>
ثابت حرف dir_separators[] = "/\\";

نطاق
{
    بنية dirent
    {
        ثابت حرف* d_name;
    };

    بنية DIR
    {
#ܐܢ defined(WINRT) || defined(_WIN32_WCE)
        WIN32_FIND_DATAW data;
#ܐܚܪܢܐ
        WIN32_FIND_DATAA data;
#endif
        HANDLE handle;
        dirent ent;
#ܐܢ defined(WINRT) || defined(_WIN32_WCE)
        DIR() { }
        ~DIR()
        {
            ܐܢ (ent.d_name)
                delete[] ent.d_name;
        }
#endif
    };

    DIR* opendir(ثابت حرف* path)
    {
        DIR* dir = جديد DIR;
        dir->ent.d_name = 0;
#ܐܢ defined(WINRT) || defined(_WIN32_WCE)
        cv::String full_path = cv::String(path) + "\\*";
        wchar_t wfull_path[MAX_PATH];
        size_t copied = mbstowcs(wfull_path, full_path.c_str(), MAX_PATH);
        CV_Assert((copied != MAX_PATH) && (copied != (size_t)-1));
        dir->handle = ::FindFirstFileExW(wfull_path, FindExInfoStandard,
                        &dir->data, FindExSearchNameMatch, عدم, 0);
#ܐܚܪܢܐ
        dir->handle = ::FindFirstFileExA((cv::String(path) + "\\*").c_str(),
            FindExInfoStandard, &dir->data, FindExSearchNameMatch, عدم, 0);
#endif
        ܐܢ(dir->handle == INVALID_HANDLE_VALUE)
        {
            /*closedir will do all cleanup*/
            delete dir;
            ܦܢܐ 0;
        }
        ܦܢܐ dir;
    }

    dirent* readdir(DIR* dir)
    {
#ܐܢ defined(WINRT) || defined(_WIN32_WCE)
        ܐܢ (dir->ent.d_name != 0)
        {
            ܐܢ (::FindNextFileW(dir->handle, &dir->data) != TRUE)
                ܦܢܐ 0;
        }
        size_t asize = wcstombs(عدم, dir->data.cFileName, 0);
        CV_Assert((asize != 0) && (asize != (size_t)-1));
        حرف* aname = جديد حرف[asize+1];
        aname[asize] = 0;
        wcstombs(aname, dir->data.cFileName, asize);
        delete[] dir->ent.d_name;
        dir->ent.d_name = aname;
#ܐܚܪܢܐ
        ܐܢ (dir->ent.d_name != 0)
        {
            ܐܢ (::FindNextFileA(dir->handle, &dir->data) != TRUE)
                ܦܢܐ 0;
        }
        dir->ent.d_name = dir->data.cFileName;
#endif
        ܦܢܐ &dir->ent;
    }

    فراغ closedir(DIR* dir)
    {
        ::FindClose(dir->handle);
        delete dir;
    }


}
#ܐܚܪܢܐ // defined _WIN32 || defined WINCE
# include <dirent.h>
# include <sys/stat.h>
ثابت حرف dir_separators[] = "/";
#endif // defined _WIN32 || defined WINCE
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT


#ܐܢ OPENCV_HAVE_FILESYSTEM_SUPPORT
ساكن منطقي isDir(ثابت cv::String& path, DIR* dir)
{
#ܐܢ defined _WIN32 || defined _WIN32_WCE
    DWORD attributes;
    BOOL status = TRUE;
    ܐܢ (dir)
        attributes = dir->data.dwFileAttributes;
    ܐܚܪܢܐ
    {
        WIN32_FILE_ATTRIBUTE_DATA all_attrs;
#ܐܢ defined WINRT || defined _WIN32_WCE
        wchar_t wpath[MAX_PATH];
        size_t copied = mbstowcs(wpath, path.c_str(), MAX_PATH);
        CV_Assert((copied != MAX_PATH) && (copied != (size_t)-1));
        status = ::GetFileAttributesExW(wpath, GetFileExInfoStandard, &all_attrs);
#ܐܚܪܢܐ
        status = ::GetFileAttributesExA(path.c_str(), GetFileExInfoStandard, &all_attrs);
#endif
        attributes = all_attrs.dwFileAttributes;
    }

    ܦܢܐ status && ((attributes & FILE_ATTRIBUTE_DIRECTORY) != 0);
#ܐܚܪܢܐ
    CV_UNUSED(dir);
    بنية stat stat_buf;
    ܐܢ (0 != stat( path.c_str(), &stat_buf))
        ܦܢܐ ܕܓܠܐ;
    صحيح is_dir = S_ISDIR( stat_buf.st_mode);
    ܦܢܐ is_dir != 0;
#endif
}
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT

منطقي cv::utils::fs::isDirectory(ثابت cv::String& path)
{
#ܐܢ OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_INSTRUMENT_REGION();
    ܦܢܐ isDir(path, عدم);
#ܐܚܪܢܐ
    CV_UNUSED(path);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#endif
}

#ܐܢ OPENCV_HAVE_FILESYSTEM_SUPPORT
ساكن منطقي wildcmp(ثابت حرف *string, ثابت حرف *wild)
{
    // Based on wildcmp written by Jack Handy - <A href="mailto:jakkhandy@hotmail.com">jakkhandy@hotmail.com</A>
    ثابت حرف *cp = 0, *mp = 0;

    ܟܡܐ ((*string) && (*wild != '*'))
    {
        ܐܢ ((*wild != *string) && (*wild != '?'))
        {
            ܦܢܐ ܕܓܠܐ;
        }

        wild++;
        string++;
    }

    ܟܡܐ (*string)
    {
        ܐܢ (*wild == '*')
        {
            ܐܢ (!*++wild)
            {
                ܦܢܐ ܫܪܪܐ;
            }

            mp = wild;
            cp = string + 1;
        }
        ܐܚܪܢܐ ܐܢ ((*wild == *string) || (*wild == '?'))
        {
            wild++;
            string++;
        }
        ܐܚܪܢܐ
        {
            wild = mp;
            string = cp++;
        }
    }

    ܟܡܐ (*wild == '*')
    {
        wild++;
    }

    ܦܢܐ *wild == 0;
}

ساكن فراغ glob_rec(ثابت cv::String& directory, ثابت cv::String& wildchart, std::vector<cv::String>& result,
        منطقي recursive, منطقي includeDirectories, ثابت cv::String& pathPrefix)
{
    DIR *dir;

    ܐܢ ((dir = opendir (directory.c_str())) != 0)
    {
        /* find all the files and directories within directory */
        حاول
        {
            بنية dirent *ent;
            ܟܡܐ ((ent = readdir (dir)) != 0)
            {
                ثابت حرف* name = ent->d_name;
                ܐܢ((name[0] == 0) || (name[0] == '.' && name[1] == 0) || (name[0] == '.' && name[1] == '.' && name[2] == 0))
                    استمر;

                cv::String path = cv::utils::fs::join(directory, name);
                cv::String entry = cv::utils::fs::join(pathPrefix, name);

                ܐܢ (isDir(path, dir))
                {
                    ܐܢ (recursive)
                        glob_rec(path, wildchart, result, recursive, includeDirectories, entry);
                    ܐܢ (!includeDirectories)
                        استمر;
                }

                ܐܢ (wildchart.empty() || wildcmp(name, wildchart.c_str()))
                    result.push_back(entry);
            }
        }
        امسك (...)
        {
            closedir(dir);
            ارم;
        }
        closedir(dir);
    }
    ܐܚܪܢܐ
    {
        CV_Error_(cv::Error::StsObjectNotFound, ("could not open directory: %s", directory.c_str()));
    }
}
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT

فراغ cv::glob(String pattern, std::vector<String>& result, منطقي recursive)
{
#ܐܢ OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_INSTRUMENT_REGION();

    result.clear();
    String path, wildchart;

    ܐܢ (isDir(pattern, 0))
    {
        ܐܢ(strchr(dir_separators, pattern[pattern.size() - 1]) != 0)
        {
            path = pattern.substr(0, pattern.size() - 1);
        }
        ܐܚܪܢܐ
        {
            path = pattern;
        }
    }
    ܐܚܪܢܐ
    {
        size_t pos = pattern.find_last_of(dir_separators);
        ܐܢ (pos == String::npos)
        {
            wildchart = pattern;
            path = ".";
        }
        ܐܚܪܢܐ
        {
            path = pattern.substr(0, pos);
            wildchart = pattern.substr(pos + 1);
        }
    }

    glob_rec(path, wildchart, result, recursive, ܕܓܠܐ, path);
    std::sort(result.begin(), result.end());
#ܐܚܪܢܐ // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_UNUSED(pattern);
    CV_UNUSED(result);
    CV_UNUSED(recursive);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT
}

فراغ cv::utils::fs::glob(ثابت cv::String& directory, ثابت cv::String& pattern,
        std::vector<cv::String>& result,
        منطقي recursive, منطقي includeDirectories)
{
#ܐܢ OPENCV_HAVE_FILESYSTEM_SUPPORT
    glob_rec(directory, pattern, result, recursive, includeDirectories, directory);
    std::sort(result.begin(), result.end());
#ܐܚܪܢܐ // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_UNUSED(directory);
    CV_UNUSED(pattern);
    CV_UNUSED(result);
    CV_UNUSED(recursive);
    CV_UNUSED(includeDirectories);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT
}

فراغ cv::utils::fs::glob_relative(ثابت cv::String& directory, ثابت cv::String& pattern,
        std::vector<cv::String>& result,
        منطقي recursive, منطقي includeDirectories)
{
#ܐܢ OPENCV_HAVE_FILESYSTEM_SUPPORT
    glob_rec(directory, pattern, result, recursive, includeDirectories, cv::String());
    std::sort(result.begin(), result.end());
#ܐܚܪܢܐ // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_UNUSED(directory);
    CV_UNUSED(pattern);
    CV_UNUSED(result);
    CV_UNUSED(recursive);
    CV_UNUSED(includeDirectories);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT
}
