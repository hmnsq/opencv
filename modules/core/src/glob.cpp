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
ثابت حرف dir_separators[] = "/\\";

نامخلا
{
    ساخت dirent
    {
        ثابت حرف* d_name;
    };

    ساخت DIR
    {
#اگر defined(WINRT) || defined(_WIN32_WCE)
        WIN32_ڈھونڈو_DATAW ڈیٹا;
#ورنہ
        WIN32_ڈھونڈو_DATAA ڈیٹا;
#اگر_ختم
        HANDLE handle;
        dirent ent;
#اگر defined(WINRT) || defined(_WIN32_WCE)
        DIR() { }
        ~DIR()
        {
            اگر (ent.d_name)
                حذف[] ent.d_name;
        }
#اگر_ختم
    };

    DIR* opendir(ثابت حرف* راستہ)
    {
        DIR* dir = نیا DIR;
        dir->ent.d_name = 0;
#اگر defined(WINRT) || defined(_WIN32_WCE)
        cv::String بھرا_راستہ = cv::String(راستہ) + "\\*";
        wchar_t wfull_راستہ[MAX_راستہ];
        size_t copied = mbstowcs(wfull_راستہ, بھرا_راستہ.c_str(), MAX_راستہ);
        CV_Assert((copied != MAX_راستہ) && (copied != (size_t)-1));
        dir->handle = ::ڈھونڈو_پہلا_File_Ex_W(wfull_راستہ, ڈھونڈو_Ex_Info_Standard,
                        &dir->ڈیٹا, ڈھونڈو_Ex_تلاش_Name_مطابقت, تہی, 0);
#ورنہ
        dir->handle = ::ڈھونڈو_پہلا_File_Ex_A((cv::String(راستہ) + "\\*").c_str(),
            ڈھونڈو_Ex_Info_Standard, &dir->ڈیٹا, ڈھونڈو_Ex_تلاش_Name_مطابقت, تہی, 0);
#اگر_ختم
        اگر(dir->handle == INVALID_HANDLE_قدر)
        {
            /*closedir will do all cleanup*/
            حذف dir;
            واپس 0;
        }
        واپس dir;
    }

    dirent* readdir(DIR* dir)
    {
#اگر defined(WINRT) || defined(_WIN32_WCE)
        اگر (dir->ent.d_name != 0)
        {
            اگر (::ڈھونڈو_اگلا_File_W(dir->handle, &dir->ڈیٹا) != TRUE)
                واپس 0;
        }
        size_t asize = wcstombs(تہی, dir->ڈیٹا.cFileName, 0);
        CV_Assert((asize != 0) && (asize != (size_t)-1));
        حرف* aname = نیا حرف[asize+1];
        aname[asize] = 0;
        wcstombs(aname, dir->ڈیٹا.cFileName, asize);
        حذف[] dir->ent.d_name;
        dir->ent.d_name = aname;
#ورنہ
        اگر (dir->ent.d_name != 0)
        {
            اگر (::ڈھونڈو_اگلا_File_A(dir->handle, &dir->ڈیٹا) != TRUE)
                واپس 0;
        }
        dir->ent.d_name = dir->ڈیٹا.cFileName;
#اگر_ختم
        واپس &dir->ent;
    }

    خالی closedir(DIR* dir)
    {
        ::ڈھونڈو_Close(dir->handle);
        حذف dir;
    }


}
#ورنہ // defined _WIN32 || defined WINCE
# include <dirent.h>
# include <sys/stat.h>
ثابت حرف dir_separators[] = "/";
#اگر_ختم // defined _WIN32 || defined WINCE
#اگر_ختم // OPENCV_HAVE_FILESYSTEM_SUPPORT


#اگر OPENCV_HAVE_FILESYSTEM_SUPPORT
ساکن منطقی isDir(ثابت cv::String& راستہ, DIR* dir)
{
#اگر defined _WIN32 || defined _WIN32_WCE
    DWORD اوصاف;
    BOOL حالت_2 = TRUE;
    اگر (dir)
        اوصاف = dir->ڈیٹا.dw_File_اوصاف;
    ورنہ
    {
        WIN32_FILE_ATTRIBUTE_ڈیٹا all_attrs;
#اگر defined WINRT || defined _WIN32_WCE
        wchar_t wpath[MAX_راستہ];
        size_t copied = mbstowcs(wpath, راستہ.c_str(), MAX_راستہ);
        CV_Assert((copied != MAX_راستہ) && (copied != (size_t)-1));
        حالت_2 = ::Get_File_اوصاف_Ex_W(wpath, GetFileExInfoStandard, &all_attrs);
#ورنہ
        حالت_2 = ::Get_File_اوصاف_Ex_A(راستہ.c_str(), GetFileExInfoStandard, &all_attrs);
#اگر_ختم
        اوصاف = all_attrs.dw_File_اوصاف;
    }

    واپس حالت_2 && ((اوصاف & FILE_ATTRIBUTE_DIRECTORY) != 0);
#ورنہ
    CV_غیر_مستعمل(dir);
    ساخت stat stat_buf;
    اگر (0 != stat( راستہ.c_str(), &stat_buf))
        واپس جھوٹ;
    صحیح is_dir = S_ISDIR( stat_buf.st_منوال);
    واپس is_dir != 0;
#اگر_ختم
}
#اگر_ختم // OPENCV_HAVE_FILESYSTEM_SUPPORT

منطقی cv::utils::fs::isDirectory(ثابت cv::String& راستہ)
{
#اگر OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_INSTRUMENT_REGION();
    واپس isDir(راستہ, تہی);
#ورنہ
    CV_غیر_مستعمل(راستہ);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#اگر_ختم
}

#اگر OPENCV_HAVE_FILESYSTEM_SUPPORT
ساکن منطقی wildcmp(ثابت حرف *string, ثابت حرف *wild)
{
    // Based on wildcmp written by Jack Handy - <A href="mailto:jakkhandy@hotmail.com">jakkhandy@hotmail.com</A>
    ثابت حرف *cp = 0, *mp = 0;

    جب ((*string) && (*wild != '*'))
    {
        اگر ((*wild != *string) && (*wild != '?'))
        {
            واپس جھوٹ;
        }

        wild++;
        string++;
    }

    جب (*string)
    {
        اگر (*wild == '*')
        {
            اگر (!*++wild)
            {
                واپس سچ;
            }

            mp = wild;
            cp = string + 1;
        }
        ورنہ اگر ((*wild == *string) || (*wild == '?'))
        {
            wild++;
            string++;
        }
        ورنہ
        {
            wild = mp;
            string = cp++;
        }
    }

    جب (*wild == '*')
    {
        wild++;
    }

    واپس *wild == 0;
}

ساکن خالی glob_rec(ثابت cv::String& directory, ثابت cv::String& wildchart, std::vector<cv::String>& نتیجہ,
        منطقی recursive, منطقی includeDirectories, ثابت cv::String& راستہ_سابقہ)
{
    DIR *dir;

    اگر ((dir = opendir (directory.c_str())) != 0)
    {
        /* find all the files and directories within directory */
        کوشش
        {
            ساخت dirent *ent;
            جب ((ent = readdir (dir)) != 0)
            {
                ثابت حرف* name = ent->d_name;
                اگر((name[0] == 0) || (name[0] == '.' && name[1] == 0) || (name[0] == '.' && name[1] == '.' && name[2] == 0))
                    جاری;

                cv::String راستہ = cv::utils::fs::join(directory, name);
                cv::String اندراج_2 = cv::utils::fs::join(راستہ_سابقہ, name);

                اگر (isDir(راستہ, dir))
                {
                    اگر (recursive)
                        glob_rec(راستہ, wildchart, نتیجہ, recursive, includeDirectories, اندراج_2);
                    اگر (!includeDirectories)
                        جاری;
                }

                اگر (wildchart.empty() || wildcmp(name, wildchart.c_str()))
                    نتیجہ.push_back(اندراج_2);
            }
        }
        پکڑو (...)
        {
            closedir(dir);
            پھینکو;
        }
        closedir(dir);
    }
    ورنہ
    {
        CV_Error_(cv::Error::Sts_Object_Not_ملا, ("could not open directory: %s", directory.c_str()));
    }
}
#اگر_ختم // OPENCV_HAVE_FILESYSTEM_SUPPORT

خالی cv::glob(String نمونہ, std::vector<String>& نتیجہ, منطقی recursive)
{
#اگر OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_INSTRUMENT_REGION();

    نتیجہ.clear();
    String راستہ, wildchart;

    اگر (isDir(نمونہ, 0))
    {
        اگر(strchr(dir_separators, نمونہ[نمونہ.size() - 1]) != 0)
        {
            راستہ = نمونہ.substr(0, نمونہ.size() - 1);
        }
        ورنہ
        {
            راستہ = نمونہ;
        }
    }
    ورنہ
    {
        size_t pos = نمونہ.find_last_of(dir_separators);
        اگر (pos == String::npos)
        {
            wildchart = نمونہ;
            راستہ = ".";
        }
        ورنہ
        {
            راستہ = نمونہ.substr(0, pos);
            wildchart = نمونہ.substr(pos + 1);
        }
    }

    glob_rec(راستہ, wildchart, نتیجہ, recursive, جھوٹ, راستہ);
    std::sort(نتیجہ.begin(), نتیجہ.end());
#ورنہ // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_غیر_مستعمل(نمونہ);
    CV_غیر_مستعمل(نتیجہ);
    CV_غیر_مستعمل(recursive);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#اگر_ختم // OPENCV_HAVE_FILESYSTEM_SUPPORT
}

خالی cv::utils::fs::glob(ثابت cv::String& directory, ثابت cv::String& نمونہ,
        std::vector<cv::String>& نتیجہ,
        منطقی recursive, منطقی includeDirectories)
{
#اگر OPENCV_HAVE_FILESYSTEM_SUPPORT
    glob_rec(directory, نمونہ, نتیجہ, recursive, includeDirectories, directory);
    std::sort(نتیجہ.begin(), نتیجہ.end());
#ورنہ // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_غیر_مستعمل(directory);
    CV_غیر_مستعمل(نمونہ);
    CV_غیر_مستعمل(نتیجہ);
    CV_غیر_مستعمل(recursive);
    CV_غیر_مستعمل(includeDirectories);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#اگر_ختم // OPENCV_HAVE_FILESYSTEM_SUPPORT
}

خالی cv::utils::fs::glob_relative(ثابت cv::String& directory, ثابت cv::String& نمونہ,
        std::vector<cv::String>& نتیجہ,
        منطقی recursive, منطقی includeDirectories)
{
#اگر OPENCV_HAVE_FILESYSTEM_SUPPORT
    glob_rec(directory, نمونہ, نتیجہ, recursive, includeDirectories, cv::String());
    std::sort(نتیجہ.begin(), نتیجہ.end());
#ورنہ // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_غیر_مستعمل(directory);
    CV_غیر_مستعمل(نمونہ);
    CV_غیر_مستعمل(نتیجہ);
    CV_غیر_مستعمل(recursive);
    CV_غیر_مستعمل(includeDirectories);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#اگر_ختم // OPENCV_HAVE_FILESYSTEM_SUPPORT
}
