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

#অন্তর্ভাব "precomp.hpp"

#অন্তর্ভাব "opencv2/core/utils/filesystem.hpp"
#অন্তর্ভাব "opencv2/core/utils/filesystem.private.hpp"

#যদি OPENCV_HAVE_FILESYSTEM_SUPPORT
#যদি defined _WIN32 || defined WINCE
# include <windows.h>
ধ্রুবক অক্ষর dir_separators[] = "/\\";

নামাকাশ
{
    কাঠামো dirent
    {
        ধ্রুবক অক্ষর* d_name;
    };

    কাঠামো DIR
    {
#যদি defined(WINRT) || defined(_WIN32_WCE)
        WIN32_FIND_DATAW তথ্য;
#অন্যথা
        WIN32_FIND_DATAA তথ্য;
#endif
        HANDLE handle;
        dirent ent;
#যদি defined(WINRT) || defined(_WIN32_WCE)
        DIR() { }
        ~DIR()
        {
            যদি (ent.d_name)
                delete[] ent.d_name;
        }
#endif
    };

    DIR* opendir(ধ্রুবক অক্ষর* path)
    {
        DIR* dir = নতুন DIR;
        dir->ent.d_name = 0;
#যদি defined(WINRT) || defined(_WIN32_WCE)
        cv::String full_path = cv::String(path) + "\\*";
        wchar_t wfull_path[MAX_PATH];
        size_t copied = mbstowcs(wfull_path, full_path.c_str(), MAX_PATH);
        CV_Assert((copied != MAX_PATH) && (copied != (size_t)-1));
        dir->handle = ::FindFirstFileExW(wfull_path, FindExInfoStandard,
                        &dir->তথ্য, Find_Ex_অনুসন্ধান_Name_Match, ফাঁকা, 0);
#অন্যথা
        dir->handle = ::FindFirstFileExA((cv::String(path) + "\\*").c_str(),
            FindExInfoStandard, &dir->তথ্য, Find_Ex_অনুসন্ধান_Name_Match, ফাঁকা, 0);
#endif
        যদি(dir->handle == INVALID_HANDLE_মান)
        {
            /*closedir will do all cleanup*/
            delete dir;
            ফেরত 0;
        }
        ফেরত dir;
    }

    dirent* readdir(DIR* dir)
    {
#যদি defined(WINRT) || defined(_WIN32_WCE)
        যদি (dir->ent.d_name != 0)
        {
            যদি (::Find_পরবর্তী_File_W(dir->handle, &dir->তথ্য) != TRUE)
                ফেরত 0;
        }
        size_t asize = wcstombs(ফাঁকা, dir->তথ্য.cFileName, 0);
        CV_Assert((asize != 0) && (asize != (size_t)-1));
        অক্ষর* aname = নতুন অক্ষর[asize+1];
        aname[asize] = 0;
        wcstombs(aname, dir->তথ্য.cFileName, asize);
        delete[] dir->ent.d_name;
        dir->ent.d_name = aname;
#অন্যথা
        যদি (dir->ent.d_name != 0)
        {
            যদি (::Find_পরবর্তী_File_A(dir->handle, &dir->তথ্য) != TRUE)
                ফেরত 0;
        }
        dir->ent.d_name = dir->তথ্য.cFileName;
#endif
        ফেরত &dir->ent;
    }

    শূন্য closedir(DIR* dir)
    {
        ::FindClose(dir->handle);
        delete dir;
    }


}
#অন্যথা // defined _WIN32 || defined WINCE
# include <dirent.h>
# include <sys/stat.h>
ধ্রুবক অক্ষর dir_separators[] = "/";
#endif // defined _WIN32 || defined WINCE
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT


#যদি OPENCV_HAVE_FILESYSTEM_SUPPORT
স্থির যৌক্তিক isDir(ধ্রুবক cv::String& path, DIR* dir)
{
#যদি defined _WIN32 || defined _WIN32_WCE
    DWORD attributes;
    BOOL status = TRUE;
    যদি (dir)
        attributes = dir->তথ্য.dwFileAttributes;
    অন্যথা
    {
        WIN32_FILE_ATTRIBUTE_তথ্য all_attrs;
#যদি defined WINRT || defined _WIN32_WCE
        wchar_t wpath[MAX_PATH];
        size_t copied = mbstowcs(wpath, path.c_str(), MAX_PATH);
        CV_Assert((copied != MAX_PATH) && (copied != (size_t)-1));
        status = ::GetFileAttributesExW(wpath, GetFileExInfoStandard, &all_attrs);
#অন্যথা
        status = ::GetFileAttributesExA(path.c_str(), GetFileExInfoStandard, &all_attrs);
#endif
        attributes = all_attrs.dwFileAttributes;
    }

    ফেরত status && ((attributes & FILE_ATTRIBUTE_DIRECTORY) != 0);
#অন্যথা
    CV_UNUSED(dir);
    কাঠামো stat stat_buf;
    যদি (0 != stat( path.c_str(), &stat_buf))
        ফেরত মিথ্যা;
    পূর্ণসংখ্যা is_dir = S_ISDIR( stat_buf.st_mode);
    ফেরত is_dir != 0;
#endif
}
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT

যৌক্তিক cv::utils::fs::isDirectory(ধ্রুবক cv::String& path)
{
#যদি OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_INSTRUMENT_REGION();
    ফেরত isDir(path, ফাঁকা);
#অন্যথা
    CV_UNUSED(path);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#endif
}

#যদি OPENCV_HAVE_FILESYSTEM_SUPPORT
স্থির যৌক্তিক wildcmp(ধ্রুবক অক্ষর *string, ধ্রুবক অক্ষর *wild)
{
    // Based on wildcmp written by Jack Handy - <A href="mailto:jakkhandy@hotmail.com">jakkhandy@hotmail.com</A>
    ধ্রুবক অক্ষর *cp = 0, *mp = 0;

    যখন ((*string) && (*wild != '*'))
    {
        যদি ((*wild != *string) && (*wild != '?'))
        {
            ফেরত মিথ্যা;
        }

        wild++;
        string++;
    }

    যখন (*string)
    {
        যদি (*wild == '*')
        {
            যদি (!*++wild)
            {
                ফেরত সত্য;
            }

            mp = wild;
            cp = string + 1;
        }
        অন্যথা যদি ((*wild == *string) || (*wild == '?'))
        {
            wild++;
            string++;
        }
        অন্যথা
        {
            wild = mp;
            string = cp++;
        }
    }

    যখন (*wild == '*')
    {
        wild++;
    }

    ফেরত *wild == 0;
}

স্থির শূন্য glob_rec(ধ্রুবক cv::String& directory, ধ্রুবক cv::String& wildchart, std::vector<cv::String>& ফলাফল,
        যৌক্তিক recursive, যৌক্তিক includeDirectories, ধ্রুবক cv::String& pathPrefix)
{
    DIR *dir;

    যদি ((dir = opendir (directory.c_str())) != 0)
    {
        /* find all the files and directories within directory */
        চেষ্টা
        {
            কাঠামো dirent *ent;
            যখন ((ent = readdir (dir)) != 0)
            {
                ধ্রুবক অক্ষর* name = ent->d_name;
                যদি((name[0] == 0) || (name[0] == '.' && name[1] == 0) || (name[0] == '.' && name[1] == '.' && name[2] == 0))
                    চালু;

                cv::String path = cv::utils::fs::join(directory, name);
                cv::String entry = cv::utils::fs::join(pathPrefix, name);

                যদি (isDir(path, dir))
                {
                    যদি (recursive)
                        glob_rec(path, wildchart, ফলাফল, recursive, includeDirectories, entry);
                    যদি (!includeDirectories)
                        চালু;
                }

                যদি (wildchart.empty() || wildcmp(name, wildchart.c_str()))
                    ফলাফল.push_back(entry);
            }
        }
        ধরো (...)
        {
            closedir(dir);
            ছোড়ো;
        }
        closedir(dir);
    }
    অন্যথা
    {
        CV_Error_(cv::Error::StsObjectNotFound, ("could not open directory: %s", directory.c_str()));
    }
}
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT

শূন্য cv::glob(String pattern, std::vector<String>& ফলাফল, যৌক্তিক recursive)
{
#যদি OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_INSTRUMENT_REGION();

    ফলাফল.clear();
    String path, wildchart;

    যদি (isDir(pattern, 0))
    {
        যদি(strchr(dir_separators, pattern[pattern.size() - 1]) != 0)
        {
            path = pattern.substr(0, pattern.size() - 1);
        }
        অন্যথা
        {
            path = pattern;
        }
    }
    অন্যথা
    {
        size_t pos = pattern.find_last_of(dir_separators);
        যদি (pos == String::npos)
        {
            wildchart = pattern;
            path = ".";
        }
        অন্যথা
        {
            path = pattern.substr(0, pos);
            wildchart = pattern.substr(pos + 1);
        }
    }

    glob_rec(path, wildchart, ফলাফল, recursive, মিথ্যা, path);
    std::sort(ফলাফল.begin(), ফলাফল.end());
#অন্যথা // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_UNUSED(pattern);
    CV_UNUSED(ফলাফল);
    CV_UNUSED(recursive);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT
}

শূন্য cv::utils::fs::glob(ধ্রুবক cv::String& directory, ধ্রুবক cv::String& pattern,
        std::vector<cv::String>& ফলাফল,
        যৌক্তিক recursive, যৌক্তিক includeDirectories)
{
#যদি OPENCV_HAVE_FILESYSTEM_SUPPORT
    glob_rec(directory, pattern, ফলাফল, recursive, includeDirectories, directory);
    std::sort(ফলাফল.begin(), ফলাফল.end());
#অন্যথা // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_UNUSED(directory);
    CV_UNUSED(pattern);
    CV_UNUSED(ফলাফল);
    CV_UNUSED(recursive);
    CV_UNUSED(includeDirectories);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT
}

শূন্য cv::utils::fs::glob_relative(ধ্রুবক cv::String& directory, ধ্রুবক cv::String& pattern,
        std::vector<cv::String>& ফলাফল,
        যৌক্তিক recursive, যৌক্তিক includeDirectories)
{
#যদি OPENCV_HAVE_FILESYSTEM_SUPPORT
    glob_rec(directory, pattern, ফলাফল, recursive, includeDirectories, cv::String());
    std::sort(ফলাফল.begin(), ফলাফল.end());
#অন্যথা // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_UNUSED(directory);
    CV_UNUSED(pattern);
    CV_UNUSED(ফলাফল);
    CV_UNUSED(recursive);
    CV_UNUSED(includeDirectories);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT
}
