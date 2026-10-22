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

#شامل "precomp.hpp"

#شامل "opencv2/core/utils/filesystem.hpp"
#شامل "opencv2/core/utils/filesystem.private.hpp"

#اگر OPENCV_HAVE_FILESYSTEM_SUPPORT
#اگر defined _WIN32 || defined WINCE
# include <windows.h>
ثابت نویسه dir_separators[] = "/\\";

فضاینام
{
    ساختار dirent
    {
        ثابت نویسه* d_name;
    };

    ساختار DIR
    {
#اگر defined(WINRT) || defined(_WIN32_WCE)
        WIN32_FIND_DATAW data;
#وگرنه
        WIN32_FIND_DATAA data;
#endif
        HANDLE handle;
        dirent ent;
#اگر defined(WINRT) || defined(_WIN32_WCE)
        DIR() { }
        ~DIR()
        {
            اگر (ent.d_name)
                delete[] ent.d_name;
        }
#endif
    };

    DIR* opendir(ثابت نویسه* path)
    {
        DIR* dir = جدید DIR;
        dir->ent.d_name = 0;
#اگر defined(WINRT) || defined(_WIN32_WCE)
        cv::String full_path = cv::String(path) + "\\*";
        wchar_t wfull_path[MAX_PATH];
        size_t copied = mbstowcs(wfull_path, full_path.c_str(), MAX_PATH);
        CV_Assert((copied != MAX_PATH) && (copied != (size_t)-1));
        dir->handle = ::FindFirstFileExW(wfull_path, FindExInfoStandard,
                        &dir->data, Find_Ex_جستجو_Name_Match, پوچ, 0);
#وگرنه
        dir->handle = ::FindFirstFileExA((cv::String(path) + "\\*").c_str(),
            FindExInfoStandard, &dir->data, Find_Ex_جستجو_Name_Match, پوچ, 0);
#endif
        اگر(dir->handle == INVALID_HANDLE_مقدار)
        {
            /*closedir will do all cleanup*/
            delete dir;
            بازگشت 0;
        }
        بازگشت dir;
    }

    dirent* readdir(DIR* dir)
    {
#اگر defined(WINRT) || defined(_WIN32_WCE)
        اگر (dir->ent.d_name != 0)
        {
            اگر (::FindNextFileW(dir->handle, &dir->data) != TRUE)
                بازگشت 0;
        }
        size_t asize = wcstombs(پوچ, dir->data.cFileName, 0);
        CV_Assert((asize != 0) && (asize != (size_t)-1));
        نویسه* aname = جدید نویسه[asize+1];
        aname[asize] = 0;
        wcstombs(aname, dir->data.cFileName, asize);
        delete[] dir->ent.d_name;
        dir->ent.d_name = aname;
#وگرنه
        اگر (dir->ent.d_name != 0)
        {
            اگر (::FindNextFileA(dir->handle, &dir->data) != TRUE)
                بازگشت 0;
        }
        dir->ent.d_name = dir->data.cFileName;
#endif
        بازگشت &dir->ent;
    }

    تهی closedir(DIR* dir)
    {
        ::FindClose(dir->handle);
        delete dir;
    }


}
#وگرنه // defined _WIN32 || defined WINCE
# include <dirent.h>
# include <sys/stat.h>
ثابت نویسه dir_separators[] = "/";
#endif // defined _WIN32 || defined WINCE
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT


#اگر OPENCV_HAVE_FILESYSTEM_SUPPORT
ایستا منطقی isDir(ثابت cv::String& path, DIR* dir)
{
#اگر defined _WIN32 || defined _WIN32_WCE
    DWORD attributes;
    BOOL status = TRUE;
    اگر (dir)
        attributes = dir->data.dwFileAttributes;
    وگرنه
    {
        WIN32_FILE_ATTRIBUTE_DATA all_attrs;
#اگر defined WINRT || defined _WIN32_WCE
        wchar_t wpath[MAX_PATH];
        size_t copied = mbstowcs(wpath, path.c_str(), MAX_PATH);
        CV_Assert((copied != MAX_PATH) && (copied != (size_t)-1));
        status = ::GetFileAttributesExW(wpath, GetFileExInfoStandard, &all_attrs);
#وگرنه
        status = ::GetFileAttributesExA(path.c_str(), GetFileExInfoStandard, &all_attrs);
#endif
        attributes = all_attrs.dwFileAttributes;
    }

    بازگشت status && ((attributes & FILE_ATTRIBUTE_DIRECTORY) != 0);
#وگرنه
    CV_UNUSED(dir);
    ساختار stat stat_buf;
    اگر (0 != stat( path.c_str(), &stat_buf))
        بازگشت نادرست;
    صحیح is_dir = S_ISDIR( stat_buf.st_mode);
    بازگشت is_dir != 0;
#endif
}
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT

منطقی cv::utils::fs::isDirectory(ثابت cv::String& path)
{
#اگر OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_INSTRUMENT_REGION();
    بازگشت isDir(path, پوچ);
#وگرنه
    CV_UNUSED(path);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#endif
}

#اگر OPENCV_HAVE_FILESYSTEM_SUPPORT
ایستا منطقی wildcmp(ثابت نویسه *string, ثابت نویسه *wild)
{
    // Based on wildcmp written by Jack Handy - <A href="mailto:jakkhandy@hotmail.com">jakkhandy@hotmail.com</A>
    ثابت نویسه *cp = 0, *mp = 0;

    وقتی ((*string) && (*wild != '*'))
    {
        اگر ((*wild != *string) && (*wild != '?'))
        {
            بازگشت نادرست;
        }

        wild++;
        string++;
    }

    وقتی (*string)
    {
        اگر (*wild == '*')
        {
            اگر (!*++wild)
            {
                بازگشت درست;
            }

            mp = wild;
            cp = string + 1;
        }
        وگرنه اگر ((*wild == *string) || (*wild == '?'))
        {
            wild++;
            string++;
        }
        وگرنه
        {
            wild = mp;
            string = cp++;
        }
    }

    وقتی (*wild == '*')
    {
        wild++;
    }

    بازگشت *wild == 0;
}

ایستا تهی glob_rec(ثابت cv::String& directory, ثابت cv::String& wildchart, std::vector<cv::String>& result,
        منطقی recursive, منطقی includeDirectories, ثابت cv::String& pathPrefix)
{
    DIR *dir;

    اگر ((dir = opendir (directory.c_str())) != 0)
    {
        /* find all the files and directories within directory */
        تلاش
        {
            ساختار dirent *ent;
            وقتی ((ent = readdir (dir)) != 0)
            {
                ثابت نویسه* name = ent->d_name;
                اگر((name[0] == 0) || (name[0] == '.' && name[1] == 0) || (name[0] == '.' && name[1] == '.' && name[2] == 0))
                    ادامه;

                cv::String path = cv::utils::fs::join(directory, name);
                cv::String entry = cv::utils::fs::join(pathPrefix, name);

                اگر (isDir(path, dir))
                {
                    اگر (recursive)
                        glob_rec(path, wildchart, result, recursive, includeDirectories, entry);
                    اگر (!includeDirectories)
                        ادامه;
                }

                اگر (wildchart.empty() || wildcmp(name, wildchart.c_str()))
                    result.push_back(entry);
            }
        }
        بگیر (...)
        {
            closedir(dir);
            پرتابکن;
        }
        closedir(dir);
    }
    وگرنه
    {
        CV_Error_(cv::Error::StsObjectNotFound, ("could not open directory: %s", directory.c_str()));
    }
}
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT

تهی cv::glob(String pattern, std::vector<String>& result, منطقی recursive)
{
#اگر OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_INSTRUMENT_REGION();

    result.clear();
    String path, wildchart;

    اگر (isDir(pattern, 0))
    {
        اگر(strchr(dir_separators, pattern[pattern.size() - 1]) != 0)
        {
            path = pattern.substr(0, pattern.size() - 1);
        }
        وگرنه
        {
            path = pattern;
        }
    }
    وگرنه
    {
        size_t pos = pattern.find_last_of(dir_separators);
        اگر (pos == String::npos)
        {
            wildchart = pattern;
            path = ".";
        }
        وگرنه
        {
            path = pattern.substr(0, pos);
            wildchart = pattern.substr(pos + 1);
        }
    }

    glob_rec(path, wildchart, result, recursive, نادرست, path);
    std::sort(result.begin(), result.end());
#وگرنه // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_UNUSED(pattern);
    CV_UNUSED(result);
    CV_UNUSED(recursive);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT
}

تهی cv::utils::fs::glob(ثابت cv::String& directory, ثابت cv::String& pattern,
        std::vector<cv::String>& result,
        منطقی recursive, منطقی includeDirectories)
{
#اگر OPENCV_HAVE_FILESYSTEM_SUPPORT
    glob_rec(directory, pattern, result, recursive, includeDirectories, directory);
    std::sort(result.begin(), result.end());
#وگرنه // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_UNUSED(directory);
    CV_UNUSED(pattern);
    CV_UNUSED(result);
    CV_UNUSED(recursive);
    CV_UNUSED(includeDirectories);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT
}

تهی cv::utils::fs::glob_relative(ثابت cv::String& directory, ثابت cv::String& pattern,
        std::vector<cv::String>& result,
        منطقی recursive, منطقی includeDirectories)
{
#اگر OPENCV_HAVE_FILESYSTEM_SUPPORT
    glob_rec(directory, pattern, result, recursive, includeDirectories, cv::String());
    std::sort(result.begin(), result.end());
#وگرنه // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_UNUSED(directory);
    CV_UNUSED(pattern);
    CV_UNUSED(result);
    CV_UNUSED(recursive);
    CV_UNUSED(includeDirectories);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT
}
