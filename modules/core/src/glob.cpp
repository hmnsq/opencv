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

#إذا OPENCV_HAVE_FILESYSTEM_SUPPORT
#إذا defined _WIN32 || defined WINCE
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
#إذا defined(WINRT) || defined(_WIN32_WCE)
        WIN32_پیدا_کړه_DATAW معلومات;
#وإلا
        WIN32_پیدا_کړه_DATAA معلومات;
#د_که_پای
        HANDLE handle;
        dirent ent;
#إذا defined(WINRT) || defined(_WIN32_WCE)
        DIR() { }
        ~DIR()
        {
            إذا (ent.d_name)
                ړنګول[] ent.d_name;
        }
#د_که_پای
    };

    DIR* opendir(ثابت حرف* لار)
    {
        DIR* dir = جديد DIR;
        dir->ent.d_name = 0;
#إذا defined(WINRT) || defined(_WIN32_WCE)
        cv::String ډک_لار = cv::String(لار) + "\\*";
        wchar_t wfull_لار[MAX_لار];
        size_t copied = mbstowcs(wfull_لار, ډک_لار.c_str(), MAX_لار);
        CV_Assert((copied != MAX_لار) && (copied != (size_t)-1));
        dir->handle = ::پیدا_کړه_لومړی_File_Ex_W(wfull_لار, پیدا_کړه_Ex_Info_Standard,
                        &dir->معلومات, پیدا_کړه_Ex_پلټنه_Name_سمون, عدم, 0);
#وإلا
        dir->handle = ::پیدا_کړه_لومړی_File_Ex_A((cv::String(لار) + "\\*").c_str(),
            پیدا_کړه_Ex_Info_Standard, &dir->معلومات, پیدا_کړه_Ex_پلټنه_Name_سمون, عدم, 0);
#د_که_پای
        إذا(dir->handle == INVALID_HANDLE_ارزښت)
        {
            /*closedir will do all cleanup*/
            ړنګول dir;
            أرجع 0;
        }
        أرجع dir;
    }

    dirent* readdir(DIR* dir)
    {
#إذا defined(WINRT) || defined(_WIN32_WCE)
        إذا (dir->ent.d_name != 0)
        {
            إذا (::پیدا_کړه_بل_File_W(dir->handle, &dir->معلومات) != TRUE)
                أرجع 0;
        }
        size_t asize = wcstombs(عدم, dir->معلومات.cFileName, 0);
        CV_Assert((asize != 0) && (asize != (size_t)-1));
        حرف* aname = جديد حرف[asize+1];
        aname[asize] = 0;
        wcstombs(aname, dir->معلومات.cFileName, asize);
        ړنګول[] dir->ent.d_name;
        dir->ent.d_name = aname;
#وإلا
        إذا (dir->ent.d_name != 0)
        {
            إذا (::پیدا_کړه_بل_File_A(dir->handle, &dir->معلومات) != TRUE)
                أرجع 0;
        }
        dir->ent.d_name = dir->معلومات.cFileName;
#د_که_پای
        أرجع &dir->ent;
    }

    فراغ closedir(DIR* dir)
    {
        ::پیدا_کړه_Close(dir->handle);
        ړنګول dir;
    }


}
#وإلا // defined _WIN32 || defined WINCE
# include <dirent.h>
# include <sys/stat.h>
ثابت حرف dir_separators[] = "/";
#د_که_پای // defined _WIN32 || defined WINCE
#د_که_پای // OPENCV_HAVE_FILESYSTEM_SUPPORT


#إذا OPENCV_HAVE_FILESYSTEM_SUPPORT
ساكن منطقي isDir(ثابت cv::String& لار, DIR* dir)
{
#إذا defined _WIN32 || defined _WIN32_WCE
    DWORD ځانګړتیاوې;
    BOOL حالت = TRUE;
    إذا (dir)
        ځانګړتیاوې = dir->معلومات.dw_File_ځانګړتیاوې;
    وإلا
    {
        WIN32_FILE_ATTRIBUTE_معلومات all_attrs;
#إذا defined WINRT || defined _WIN32_WCE
        wchar_t wpath[MAX_لار];
        size_t copied = mbstowcs(wpath, لار.c_str(), MAX_لار);
        CV_Assert((copied != MAX_لار) && (copied != (size_t)-1));
        حالت = ::Get_File_ځانګړتیاوې_Ex_W(wpath, GetFileExInfoStandard, &all_attrs);
#وإلا
        حالت = ::Get_File_ځانګړتیاوې_Ex_A(لار.c_str(), GetFileExInfoStandard, &all_attrs);
#د_که_پای
        ځانګړتیاوې = all_attrs.dw_File_ځانګړتیاوې;
    }

    أرجع حالت && ((ځانګړتیاوې & FILE_ATTRIBUTE_DIRECTORY) != 0);
#وإلا
    CV_ناکارول_شوی(dir);
    بنية stat stat_buf;
    إذا (0 != stat( لار.c_str(), &stat_buf))
        أرجع خطأ;
    صحيح is_dir = S_ISDIR( stat_buf.st_ډېرپېښ);
    أرجع is_dir != 0;
#د_که_پای
}
#د_که_پای // OPENCV_HAVE_FILESYSTEM_SUPPORT

منطقي cv::utils::fs::isDirectory(ثابت cv::String& لار)
{
#إذا OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_INSTRUMENT_REGION();
    أرجع isDir(لار, عدم);
#وإلا
    CV_ناکارول_شوی(لار);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#د_که_پای
}

#إذا OPENCV_HAVE_FILESYSTEM_SUPPORT
ساكن منطقي wildcmp(ثابت حرف *string, ثابت حرف *wild)
{
    // Based on wildcmp written by Jack Handy - <A href="mailto:jakkhandy@hotmail.com">jakkhandy@hotmail.com</A>
    ثابت حرف *cp = 0, *mp = 0;

    طالما ((*string) && (*wild != '*'))
    {
        إذا ((*wild != *string) && (*wild != '?'))
        {
            أرجع خطأ;
        }

        wild++;
        string++;
    }

    طالما (*string)
    {
        إذا (*wild == '*')
        {
            إذا (!*++wild)
            {
                أرجع ریښتیا;
            }

            mp = wild;
            cp = string + 1;
        }
        وإلا إذا ((*wild == *string) || (*wild == '?'))
        {
            wild++;
            string++;
        }
        وإلا
        {
            wild = mp;
            string = cp++;
        }
    }

    طالما (*wild == '*')
    {
        wild++;
    }

    أرجع *wild == 0;
}

ساكن فراغ glob_rec(ثابت cv::String& directory, ثابت cv::String& wildchart, std::vector<cv::String>& پایله,
        منطقي recursive, منطقي includeDirectories, ثابت cv::String& لار_مختاړی)
{
    DIR *dir;

    إذا ((dir = opendir (directory.c_str())) != 0)
    {
        /* find all the files and directories within directory */
        حاول
        {
            بنية dirent *ent;
            طالما ((ent = readdir (dir)) != 0)
            {
                ثابت حرف* name = ent->d_name;
                إذا((name[0] == 0) || (name[0] == '.' && name[1] == 0) || (name[0] == '.' && name[1] == '.' && name[2] == 0))
                    استمر;

                cv::String لار = cv::utils::fs::join(directory, name);
                cv::String ثبت = cv::utils::fs::join(لار_مختاړی, name);

                إذا (isDir(لار, dir))
                {
                    إذا (recursive)
                        glob_rec(لار, wildchart, پایله, recursive, includeDirectories, ثبت);
                    إذا (!includeDirectories)
                        استمر;
                }

                إذا (wildchart.empty() || wildcmp(name, wildchart.c_str()))
                    پایله.push_back(ثبت);
            }
        }
        امسك (...)
        {
            closedir(dir);
            ارم;
        }
        closedir(dir);
    }
    وإلا
    {
        CV_Error_(cv::Error::Sts_Object_Not_وموندل, ("could not open directory: %s", directory.c_str()));
    }
}
#د_که_پای // OPENCV_HAVE_FILESYSTEM_SUPPORT

فراغ cv::glob(String نمونه, std::vector<String>& پایله, منطقي recursive)
{
#إذا OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_INSTRUMENT_REGION();

    پایله.clear();
    String لار, wildchart;

    إذا (isDir(نمونه, 0))
    {
        إذا(strchr(dir_separators, نمونه[نمونه.size() - 1]) != 0)
        {
            لار = نمونه.substr(0, نمونه.size() - 1);
        }
        وإلا
        {
            لار = نمونه;
        }
    }
    وإلا
    {
        size_t pos = نمونه.find_last_of(dir_separators);
        إذا (pos == String::npos)
        {
            wildchart = نمونه;
            لار = ".";
        }
        وإلا
        {
            لار = نمونه.substr(0, pos);
            wildchart = نمونه.substr(pos + 1);
        }
    }

    glob_rec(لار, wildchart, پایله, recursive, خطأ, لار);
    std::sort(پایله.begin(), پایله.end());
#وإلا // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_ناکارول_شوی(نمونه);
    CV_ناکارول_شوی(پایله);
    CV_ناکارول_شوی(recursive);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#د_که_پای // OPENCV_HAVE_FILESYSTEM_SUPPORT
}

فراغ cv::utils::fs::glob(ثابت cv::String& directory, ثابت cv::String& نمونه,
        std::vector<cv::String>& پایله,
        منطقي recursive, منطقي includeDirectories)
{
#إذا OPENCV_HAVE_FILESYSTEM_SUPPORT
    glob_rec(directory, نمونه, پایله, recursive, includeDirectories, directory);
    std::sort(پایله.begin(), پایله.end());
#وإلا // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_ناکارول_شوی(directory);
    CV_ناکارول_شوی(نمونه);
    CV_ناکارول_شوی(پایله);
    CV_ناکارول_شوی(recursive);
    CV_ناکارول_شوی(includeDirectories);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#د_که_پای // OPENCV_HAVE_FILESYSTEM_SUPPORT
}

فراغ cv::utils::fs::glob_relative(ثابت cv::String& directory, ثابت cv::String& نمونه,
        std::vector<cv::String>& پایله,
        منطقي recursive, منطقي includeDirectories)
{
#إذا OPENCV_HAVE_FILESYSTEM_SUPPORT
    glob_rec(directory, نمونه, پایله, recursive, includeDirectories, cv::String());
    std::sort(پایله.begin(), پایله.end());
#وإلا // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_ناکارول_شوی(directory);
    CV_ناکارول_شوی(نمونه);
    CV_ناکارول_شوی(پایله);
    CV_ناکارول_شوی(recursive);
    CV_ناکارول_شوی(includeDirectories);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#د_که_پای // OPENCV_HAVE_FILESYSTEM_SUPPORT
}
