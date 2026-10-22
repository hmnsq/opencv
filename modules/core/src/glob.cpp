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
        WIN32_لبھو_DATAW انکڑے;
#وإلا
        WIN32_لبھو_DATAA انکڑے;
#جے_انت
        HANDLE handle;
        dirent ent;
#إذا defined(WINRT) || defined(_WIN32_WCE)
        DIR() { }
        ~DIR()
        {
            إذا (ent.d_name)
                مٹاؤ[] ent.d_name;
        }
#جے_انت
    };

    DIR* opendir(ثابت حرف* رستہ)
    {
        DIR* dir = جديد DIR;
        dir->ent.d_name = 0;
#إذا defined(WINRT) || defined(_WIN32_WCE)
        cv::String بھریا_رستہ = cv::String(رستہ) + "\\*";
        wchar_t wfull_رستہ[MAX_رستہ];
        size_t copied = mbstowcs(wfull_رستہ, بھریا_رستہ.c_str(), MAX_رستہ);
        CV_Assert((copied != MAX_رستہ) && (copied != (size_t)-1));
        dir->handle = ::لبھو_پہلا_File_Ex_W(wfull_رستہ, لبھو_Ex_Info_Standard,
                        &dir->انکڑے, لبھو_Ex_تلاش_Name_میل, عدم, 0);
#وإلا
        dir->handle = ::لبھو_پہلا_File_Ex_A((cv::String(رستہ) + "\\*").c_str(),
            لبھو_Ex_Info_Standard, &dir->انکڑے, لبھو_Ex_تلاش_Name_میل, عدم, 0);
#جے_انت
        إذا(dir->handle == INVALID_HANDLE_قدر)
        {
            /*closedir will do all cleanup*/
            مٹاؤ dir;
            أرجع 0;
        }
        أرجع dir;
    }

    dirent* readdir(DIR* dir)
    {
#إذا defined(WINRT) || defined(_WIN32_WCE)
        إذا (dir->ent.d_name != 0)
        {
            إذا (::لبھو_اگلا_File_W(dir->handle, &dir->انکڑے) != TRUE)
                أرجع 0;
        }
        size_t asize = wcstombs(عدم, dir->انکڑے.cFileName, 0);
        CV_Assert((asize != 0) && (asize != (size_t)-1));
        حرف* aname = جديد حرف[asize+1];
        aname[asize] = 0;
        wcstombs(aname, dir->انکڑے.cFileName, asize);
        مٹاؤ[] dir->ent.d_name;
        dir->ent.d_name = aname;
#وإلا
        إذا (dir->ent.d_name != 0)
        {
            إذا (::لبھو_اگلا_File_A(dir->handle, &dir->انکڑے) != TRUE)
                أرجع 0;
        }
        dir->ent.d_name = dir->انکڑے.cFileName;
#جے_انت
        أرجع &dir->ent;
    }

    فراغ closedir(DIR* dir)
    {
        ::لبھو_Close(dir->handle);
        مٹاؤ dir;
    }


}
#وإلا // defined _WIN32 || defined WINCE
# include <dirent.h>
# include <sys/stat.h>
ثابت حرف dir_separators[] = "/";
#جے_انت // defined _WIN32 || defined WINCE
#جے_انت // OPENCV_HAVE_FILESYSTEM_SUPPORT


#إذا OPENCV_HAVE_FILESYSTEM_SUPPORT
ساكن منطقي isDir(ثابت cv::String& رستہ, DIR* dir)
{
#إذا defined _WIN32 || defined _WIN32_WCE
    DWORD گن;
    BOOL حالت = TRUE;
    إذا (dir)
        گن = dir->انکڑے.dw_File_گن;
    وإلا
    {
        WIN32_FILE_ATTRIBUTE_انکڑے all_attrs;
#إذا defined WINRT || defined _WIN32_WCE
        wchar_t wpath[MAX_رستہ];
        size_t copied = mbstowcs(wpath, رستہ.c_str(), MAX_رستہ);
        CV_Assert((copied != MAX_رستہ) && (copied != (size_t)-1));
        حالت = ::Get_File_گن_Ex_W(wpath, GetFileExInfoStandard, &all_attrs);
#وإلا
        حالت = ::Get_File_گن_Ex_A(رستہ.c_str(), GetFileExInfoStandard, &all_attrs);
#جے_انت
        گن = all_attrs.dw_File_گن;
    }

    أرجع حالت && ((گن & FILE_ATTRIBUTE_DIRECTORY) != 0);
#وإلا
    CV_بن_ورتیا(dir);
    بنية stat stat_buf;
    إذا (0 != stat( رستہ.c_str(), &stat_buf))
        أرجع خطأ;
    صحيح is_dir = S_ISDIR( stat_buf.st_منوال);
    أرجع is_dir != 0;
#جے_انت
}
#جے_انت // OPENCV_HAVE_FILESYSTEM_SUPPORT

منطقي cv::utils::fs::isDirectory(ثابت cv::String& رستہ)
{
#إذا OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_INSTRUMENT_REGION();
    أرجع isDir(رستہ, عدم);
#وإلا
    CV_بن_ورتیا(رستہ);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#جے_انت
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
                أرجع سچ;
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

ساكن فراغ glob_rec(ثابت cv::String& directory, ثابت cv::String& wildchart, std::vector<cv::String>& نتیجہ,
        منطقي recursive, منطقي includeDirectories, ثابت cv::String& رستہ_اگیتر)
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

                cv::String رستہ = cv::utils::fs::join(directory, name);
                cv::String اندراج = cv::utils::fs::join(رستہ_اگیتر, name);

                إذا (isDir(رستہ, dir))
                {
                    إذا (recursive)
                        glob_rec(رستہ, wildchart, نتیجہ, recursive, includeDirectories, اندراج);
                    إذا (!includeDirectories)
                        استمر;
                }

                إذا (wildchart.empty() || wildcmp(name, wildchart.c_str()))
                    نتیجہ.push_back(اندراج);
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
        CV_Error_(cv::Error::Sts_Object_Not_لبھیا, ("could not open directory: %s", directory.c_str()));
    }
}
#جے_انت // OPENCV_HAVE_FILESYSTEM_SUPPORT

فراغ cv::glob(String نمونہ, std::vector<String>& نتیجہ, منطقي recursive)
{
#إذا OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_INSTRUMENT_REGION();

    نتیجہ.clear();
    String رستہ, wildchart;

    إذا (isDir(نمونہ, 0))
    {
        إذا(strchr(dir_separators, نمونہ[نمونہ.size() - 1]) != 0)
        {
            رستہ = نمونہ.substr(0, نمونہ.size() - 1);
        }
        وإلا
        {
            رستہ = نمونہ;
        }
    }
    وإلا
    {
        size_t pos = نمونہ.find_last_of(dir_separators);
        إذا (pos == String::npos)
        {
            wildchart = نمونہ;
            رستہ = ".";
        }
        وإلا
        {
            رستہ = نمونہ.substr(0, pos);
            wildchart = نمونہ.substr(pos + 1);
        }
    }

    glob_rec(رستہ, wildchart, نتیجہ, recursive, خطأ, رستہ);
    std::sort(نتیجہ.begin(), نتیجہ.end());
#وإلا // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_بن_ورتیا(نمونہ);
    CV_بن_ورتیا(نتیجہ);
    CV_بن_ورتیا(recursive);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#جے_انت // OPENCV_HAVE_FILESYSTEM_SUPPORT
}

فراغ cv::utils::fs::glob(ثابت cv::String& directory, ثابت cv::String& نمونہ,
        std::vector<cv::String>& نتیجہ,
        منطقي recursive, منطقي includeDirectories)
{
#إذا OPENCV_HAVE_FILESYSTEM_SUPPORT
    glob_rec(directory, نمونہ, نتیجہ, recursive, includeDirectories, directory);
    std::sort(نتیجہ.begin(), نتیجہ.end());
#وإلا // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_بن_ورتیا(directory);
    CV_بن_ورتیا(نمونہ);
    CV_بن_ورتیا(نتیجہ);
    CV_بن_ورتیا(recursive);
    CV_بن_ورتیا(includeDirectories);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#جے_انت // OPENCV_HAVE_FILESYSTEM_SUPPORT
}

فراغ cv::utils::fs::glob_relative(ثابت cv::String& directory, ثابت cv::String& نمونہ,
        std::vector<cv::String>& نتیجہ,
        منطقي recursive, منطقي includeDirectories)
{
#إذا OPENCV_HAVE_FILESYSTEM_SUPPORT
    glob_rec(directory, نمونہ, نتیجہ, recursive, includeDirectories, cv::String());
    std::sort(نتیجہ.begin(), نتیجہ.end());
#وإلا // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_بن_ورتیا(directory);
    CV_بن_ورتیا(نمونہ);
    CV_بن_ورتیا(نتیجہ);
    CV_بن_ورتیا(recursive);
    CV_بن_ورتیا(includeDirectories);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#جے_انت // OPENCV_HAVE_FILESYSTEM_SUPPORT
}
