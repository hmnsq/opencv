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

#אם OPENCV_HAVE_FILESYSTEM_SUPPORT
#אם defined _WIN32 || defined WINCE
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
#אם defined(WINRT) || defined(_WIN32_WCE)
        WIN32_FIND_DATAW data;
#אחרת
        WIN32_FIND_DATAA data;
#endif
        HANDLE handle;
        dirent ent;
#אם defined(WINRT) || defined(_WIN32_WCE)
        DIR() { }
        ~DIR()
        {
            אם (ent.d_name)
                delete[] ent.d_name;
        }
#endif
    };

    DIR* opendir(ثابت حرف* path)
    {
        DIR* dir = جديد DIR;
        dir->ent.d_name = 0;
#אם defined(WINRT) || defined(_WIN32_WCE)
        cv::String full_path = cv::String(path) + "\\*";
        wchar_t wfull_path[MAX_PATH];
        size_t copied = mbstowcs(wfull_path, full_path.c_str(), MAX_PATH);
        CV_Assert((copied != MAX_PATH) && (copied != (size_t)-1));
        dir->handle = ::FindFirstFileExW(wfull_path, FindExInfoStandard,
                        &dir->data, FindExSearchNameMatch, عدم, 0);
#אחרת
        dir->handle = ::FindFirstFileExA((cv::String(path) + "\\*").c_str(),
            FindExInfoStandard, &dir->data, FindExSearchNameMatch, عدم, 0);
#endif
        אם(dir->handle == INVALID_HANDLE_VALUE)
        {
            /*closedir will do all cleanup*/
            delete dir;
            החזר 0;
        }
        החזר dir;
    }

    dirent* readdir(DIR* dir)
    {
#אם defined(WINRT) || defined(_WIN32_WCE)
        אם (dir->ent.d_name != 0)
        {
            אם (::FindNextFileW(dir->handle, &dir->data) != TRUE)
                החזר 0;
        }
        size_t asize = wcstombs(عدم, dir->data.cFileName, 0);
        CV_Assert((asize != 0) && (asize != (size_t)-1));
        حرف* aname = جديد حرف[asize+1];
        aname[asize] = 0;
        wcstombs(aname, dir->data.cFileName, asize);
        delete[] dir->ent.d_name;
        dir->ent.d_name = aname;
#אחרת
        אם (dir->ent.d_name != 0)
        {
            אם (::FindNextFileA(dir->handle, &dir->data) != TRUE)
                החזר 0;
        }
        dir->ent.d_name = dir->data.cFileName;
#endif
        החזר &dir->ent;
    }

    فراغ closedir(DIR* dir)
    {
        ::FindClose(dir->handle);
        delete dir;
    }


}
#אחרת // defined _WIN32 || defined WINCE
# include <dirent.h>
# include <sys/stat.h>
ثابت حرف dir_separators[] = "/";
#endif // defined _WIN32 || defined WINCE
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT


#אם OPENCV_HAVE_FILESYSTEM_SUPPORT
ساكن منطقي isDir(ثابت cv::String& path, DIR* dir)
{
#אם defined _WIN32 || defined _WIN32_WCE
    DWORD attributes;
    BOOL status = TRUE;
    אם (dir)
        attributes = dir->data.dwFileAttributes;
    אחרת
    {
        WIN32_FILE_ATTRIBUTE_DATA all_attrs;
#אם defined WINRT || defined _WIN32_WCE
        wchar_t wpath[MAX_PATH];
        size_t copied = mbstowcs(wpath, path.c_str(), MAX_PATH);
        CV_Assert((copied != MAX_PATH) && (copied != (size_t)-1));
        status = ::GetFileAttributesExW(wpath, GetFileExInfoStandard, &all_attrs);
#אחרת
        status = ::GetFileAttributesExA(path.c_str(), GetFileExInfoStandard, &all_attrs);
#endif
        attributes = all_attrs.dwFileAttributes;
    }

    החזר status && ((attributes & FILE_ATTRIBUTE_DIRECTORY) != 0);
#אחרת
    CV_UNUSED(dir);
    بنية stat stat_buf;
    אם (0 != stat( path.c_str(), &stat_buf))
        החזר שקר;
    صحيح is_dir = S_ISDIR( stat_buf.st_mode);
    החזר is_dir != 0;
#endif
}
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT

منطقي cv::utils::fs::isDirectory(ثابت cv::String& path)
{
#אם OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_INSTRUMENT_REGION();
    החזר isDir(path, عدم);
#אחרת
    CV_UNUSED(path);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#endif
}

#אם OPENCV_HAVE_FILESYSTEM_SUPPORT
ساكن منطقي wildcmp(ثابت حرف *string, ثابت حرف *wild)
{
    // Based on wildcmp written by Jack Handy - <A href="mailto:jakkhandy@hotmail.com">jakkhandy@hotmail.com</A>
    ثابت حرف *cp = 0, *mp = 0;

    כלעוד ((*string) && (*wild != '*'))
    {
        אם ((*wild != *string) && (*wild != '?'))
        {
            החזר שקר;
        }

        wild++;
        string++;
    }

    כלעוד (*string)
    {
        אם (*wild == '*')
        {
            אם (!*++wild)
            {
                החזר אמת;
            }

            mp = wild;
            cp = string + 1;
        }
        אחרת אם ((*wild == *string) || (*wild == '?'))
        {
            wild++;
            string++;
        }
        אחרת
        {
            wild = mp;
            string = cp++;
        }
    }

    כלעוד (*wild == '*')
    {
        wild++;
    }

    החזר *wild == 0;
}

ساكن فراغ glob_rec(ثابت cv::String& directory, ثابت cv::String& wildchart, std::vector<cv::String>& result,
        منطقي recursive, منطقي includeDirectories, ثابت cv::String& pathPrefix)
{
    DIR *dir;

    אם ((dir = opendir (directory.c_str())) != 0)
    {
        /* find all the files and directories within directory */
        حاول
        {
            بنية dirent *ent;
            כלעוד ((ent = readdir (dir)) != 0)
            {
                ثابت حرف* name = ent->d_name;
                אם((name[0] == 0) || (name[0] == '.' && name[1] == 0) || (name[0] == '.' && name[1] == '.' && name[2] == 0))
                    استمر;

                cv::String path = cv::utils::fs::join(directory, name);
                cv::String entry = cv::utils::fs::join(pathPrefix, name);

                אם (isDir(path, dir))
                {
                    אם (recursive)
                        glob_rec(path, wildchart, result, recursive, includeDirectories, entry);
                    אם (!includeDirectories)
                        استمر;
                }

                אם (wildchart.empty() || wildcmp(name, wildchart.c_str()))
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
    אחרת
    {
        CV_Error_(cv::Error::StsObjectNotFound, ("could not open directory: %s", directory.c_str()));
    }
}
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT

فراغ cv::glob(String pattern, std::vector<String>& result, منطقي recursive)
{
#אם OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_INSTRUMENT_REGION();

    result.clear();
    String path, wildchart;

    אם (isDir(pattern, 0))
    {
        אם(strchr(dir_separators, pattern[pattern.size() - 1]) != 0)
        {
            path = pattern.substr(0, pattern.size() - 1);
        }
        אחרת
        {
            path = pattern;
        }
    }
    אחרת
    {
        size_t pos = pattern.find_last_of(dir_separators);
        אם (pos == String::npos)
        {
            wildchart = pattern;
            path = ".";
        }
        אחרת
        {
            path = pattern.substr(0, pos);
            wildchart = pattern.substr(pos + 1);
        }
    }

    glob_rec(path, wildchart, result, recursive, שקר, path);
    std::sort(result.begin(), result.end());
#אחרת // OPENCV_HAVE_FILESYSTEM_SUPPORT
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
#אם OPENCV_HAVE_FILESYSTEM_SUPPORT
    glob_rec(directory, pattern, result, recursive, includeDirectories, directory);
    std::sort(result.begin(), result.end());
#אחרת // OPENCV_HAVE_FILESYSTEM_SUPPORT
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
#אם OPENCV_HAVE_FILESYSTEM_SUPPORT
    glob_rec(directory, pattern, result, recursive, includeDirectories, cv::String());
    std::sort(result.begin(), result.end());
#אחרת // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_UNUSED(directory);
    CV_UNUSED(pattern);
    CV_UNUSED(result);
    CV_UNUSED(recursive);
    CV_UNUSED(includeDirectories);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT
}
