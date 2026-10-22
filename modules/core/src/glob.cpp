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
        WIN32_پیدا_کن_DATAW داده;
#وإلا
        WIN32_پیدا_کن_DATAA داده;
#پایان_اگر
        HANDLE handle;
        dirent ent;
#إذا defined(WINRT) || defined(_WIN32_WCE)
        DIR() { }
        ~DIR()
        {
            إذا (ent.d_name)
                حذف[] ent.d_name;
        }
#پایان_اگر
    };

    DIR* opendir(ثابت حرف* مسیر)
    {
        DIR* dir = جديد DIR;
        dir->ent.d_name = 0;
#إذا defined(WINRT) || defined(_WIN32_WCE)
        cv::String پر_مسیر = cv::String(مسیر) + "\\*";
        wchar_t wfull_مسیر[MAX_مسیر];
        size_t copied = mbstowcs(wfull_مسیر, پر_مسیر.c_str(), MAX_مسیر);
        CV_Assert((copied != MAX_مسیر) && (copied != (size_t)-1));
        dir->handle = ::پیدا_کن_اولی_File_Ex_W(wfull_مسیر, پیدا_کن_Ex_Info_Standard,
                        &dir->داده, پیدا_کن_Ex_جستجو_Name_تطابق, عدم, 0);
#وإلا
        dir->handle = ::پیدا_کن_اولی_File_Ex_A((cv::String(مسیر) + "\\*").c_str(),
            پیدا_کن_Ex_Info_Standard, &dir->داده, پیدا_کن_Ex_جستجو_Name_تطابق, عدم, 0);
#پایان_اگر
        إذا(dir->handle == INVALID_HANDLE_مقدار)
        {
            /*closedir will do all cleanup*/
            حذف dir;
            أرجع 0;
        }
        أرجع dir;
    }

    dirent* readdir(DIR* dir)
    {
#إذا defined(WINRT) || defined(_WIN32_WCE)
        إذا (dir->ent.d_name != 0)
        {
            إذا (::پیدا_کن_بعدی_File_W(dir->handle, &dir->داده) != TRUE)
                أرجع 0;
        }
        size_t asize = wcstombs(عدم, dir->داده.cFileName, 0);
        CV_Assert((asize != 0) && (asize != (size_t)-1));
        حرف* aname = جديد حرف[asize+1];
        aname[asize] = 0;
        wcstombs(aname, dir->داده.cFileName, asize);
        حذف[] dir->ent.d_name;
        dir->ent.d_name = aname;
#وإلا
        إذا (dir->ent.d_name != 0)
        {
            إذا (::پیدا_کن_بعدی_File_A(dir->handle, &dir->داده) != TRUE)
                أرجع 0;
        }
        dir->ent.d_name = dir->داده.cFileName;
#پایان_اگر
        أرجع &dir->ent;
    }

    فراغ closedir(DIR* dir)
    {
        ::پیدا_کن_Close(dir->handle);
        حذف dir;
    }


}
#وإلا // defined _WIN32 || defined WINCE
# include <dirent.h>
# include <sys/stat.h>
ثابت حرف dir_separators[] = "/";
#پایان_اگر // defined _WIN32 || defined WINCE
#پایان_اگر // OPENCV_HAVE_FILESYSTEM_SUPPORT


#إذا OPENCV_HAVE_FILESYSTEM_SUPPORT
ساكن منطقي isDir(ثابت cv::String& مسیر, DIR* dir)
{
#إذا defined _WIN32 || defined _WIN32_WCE
    DWORD صفات;
    BOOL وضعیت = TRUE;
    إذا (dir)
        صفات = dir->داده.dw_File_صفات;
    وإلا
    {
        WIN32_FILE_ATTRIBUTE_داده all_attrs;
#إذا defined WINRT || defined _WIN32_WCE
        wchar_t wpath[MAX_مسیر];
        size_t copied = mbstowcs(wpath, مسیر.c_str(), MAX_مسیر);
        CV_Assert((copied != MAX_مسیر) && (copied != (size_t)-1));
        وضعیت = ::Get_File_صفات_Ex_W(wpath, GetFileExInfoStandard, &all_attrs);
#وإلا
        وضعیت = ::Get_File_صفات_Ex_A(مسیر.c_str(), GetFileExInfoStandard, &all_attrs);
#پایان_اگر
        صفات = all_attrs.dw_File_صفات;
    }

    أرجع وضعیت && ((صفات & FILE_ATTRIBUTE_DIRECTORY) != 0);
#وإلا
    CV_استفادهنشده(dir);
    بنية stat stat_buf;
    إذا (0 != stat( مسیر.c_str(), &stat_buf))
        أرجع خطأ;
    صحيح is_dir = S_ISDIR( stat_buf.st_نما);
    أرجع is_dir != 0;
#پایان_اگر
}
#پایان_اگر // OPENCV_HAVE_FILESYSTEM_SUPPORT

منطقي cv::utils::fs::isDirectory(ثابت cv::String& مسیر)
{
#إذا OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_INSTRUMENT_REGION();
    أرجع isDir(مسیر, عدم);
#وإلا
    CV_استفادهنشده(مسیر);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#پایان_اگر
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
                أرجع درست;
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

ساكن فراغ glob_rec(ثابت cv::String& directory, ثابت cv::String& wildchart, std::vector<cv::String>& نتیجه,
        منطقي recursive, منطقي includeDirectories, ثابت cv::String& مسیر_پیشوند)
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

                cv::String مسیر = cv::utils::fs::join(directory, name);
                cv::String مدخل = cv::utils::fs::join(مسیر_پیشوند, name);

                إذا (isDir(مسیر, dir))
                {
                    إذا (recursive)
                        glob_rec(مسیر, wildchart, نتیجه, recursive, includeDirectories, مدخل);
                    إذا (!includeDirectories)
                        استمر;
                }

                إذا (wildchart.empty() || wildcmp(name, wildchart.c_str()))
                    نتیجه.push_back(مدخل);
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
        CV_Error_(cv::Error::Sts_Object_Not_یافت, ("could not open directory: %s", directory.c_str()));
    }
}
#پایان_اگر // OPENCV_HAVE_FILESYSTEM_SUPPORT

فراغ cv::glob(String الگو, std::vector<String>& نتیجه, منطقي recursive)
{
#إذا OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_INSTRUMENT_REGION();

    نتیجه.clear();
    String مسیر, wildchart;

    إذا (isDir(الگو, 0))
    {
        إذا(strchr(dir_separators, الگو[الگو.size() - 1]) != 0)
        {
            مسیر = الگو.substr(0, الگو.size() - 1);
        }
        وإلا
        {
            مسیر = الگو;
        }
    }
    وإلا
    {
        size_t pos = الگو.find_last_of(dir_separators);
        إذا (pos == String::npos)
        {
            wildchart = الگو;
            مسیر = ".";
        }
        وإلا
        {
            مسیر = الگو.substr(0, pos);
            wildchart = الگو.substr(pos + 1);
        }
    }

    glob_rec(مسیر, wildchart, نتیجه, recursive, خطأ, مسیر);
    std::sort(نتیجه.begin(), نتیجه.end());
#وإلا // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_استفادهنشده(الگو);
    CV_استفادهنشده(نتیجه);
    CV_استفادهنشده(recursive);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#پایان_اگر // OPENCV_HAVE_FILESYSTEM_SUPPORT
}

فراغ cv::utils::fs::glob(ثابت cv::String& directory, ثابت cv::String& الگو,
        std::vector<cv::String>& نتیجه,
        منطقي recursive, منطقي includeDirectories)
{
#إذا OPENCV_HAVE_FILESYSTEM_SUPPORT
    glob_rec(directory, الگو, نتیجه, recursive, includeDirectories, directory);
    std::sort(نتیجه.begin(), نتیجه.end());
#وإلا // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_استفادهنشده(directory);
    CV_استفادهنشده(الگو);
    CV_استفادهنشده(نتیجه);
    CV_استفادهنشده(recursive);
    CV_استفادهنشده(includeDirectories);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#پایان_اگر // OPENCV_HAVE_FILESYSTEM_SUPPORT
}

فراغ cv::utils::fs::glob_relative(ثابت cv::String& directory, ثابت cv::String& الگو,
        std::vector<cv::String>& نتیجه,
        منطقي recursive, منطقي includeDirectories)
{
#إذا OPENCV_HAVE_FILESYSTEM_SUPPORT
    glob_rec(directory, الگو, نتیجه, recursive, includeDirectories, cv::String());
    std::sort(نتیجه.begin(), نتیجه.end());
#وإلا // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_استفادهنشده(directory);
    CV_استفادهنشده(الگو);
    CV_استفادهنشده(نتیجه);
    CV_استفادهنشده(recursive);
    CV_استفادهنشده(includeDirectories);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#پایان_اگر // OPENCV_HAVE_FILESYSTEM_SUPPORT
}
