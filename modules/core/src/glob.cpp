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
        WIN32_ڳوليو_DATAW معلومات;
#وإلا
        WIN32_ڳوليو_DATAA معلومات;
#جيڪڏهن_پورو
        HANDLE handle;
        dirent ent;
#إذا defined(WINRT) || defined(_WIN32_WCE)
        DIR() { }
        ~DIR()
        {
            إذا (ent.d_name)
                ميٽيو[] ent.d_name;
        }
#جيڪڏهن_پورو
    };

    DIR* opendir(ثابت حرف* رستو)
    {
        DIR* dir = جديد DIR;
        dir->ent.d_name = 0;
#إذا defined(WINRT) || defined(_WIN32_WCE)
        cv::String ڀريل_رستو = cv::String(رستو) + "\\*";
        wchar_t wfull_رستو[MAX_رستو];
        size_t copied = mbstowcs(wfull_رستو, ڀريل_رستو.c_str(), MAX_رستو);
        CV_Assert((copied != MAX_رستو) && (copied != (size_t)-1));
        dir->handle = ::ڳوليو_پهريون_File_Ex_W(wfull_رستو, ڳوليو_Ex_Info_Standard,
                        &dir->معلومات, ڳوليو_Ex_ڳولا_Name_ميلاپ, عدم, 0);
#وإلا
        dir->handle = ::ڳوليو_پهريون_File_Ex_A((cv::String(رستو) + "\\*").c_str(),
            ڳوليو_Ex_Info_Standard, &dir->معلومات, ڳوليو_Ex_ڳولا_Name_ميلاپ, عدم, 0);
#جيڪڏهن_پورو
        إذا(dir->handle == INVALID_HANDLE_قدر)
        {
            /*closedir will do all cleanup*/
            ميٽيو dir;
            أرجع 0;
        }
        أرجع dir;
    }

    dirent* readdir(DIR* dir)
    {
#إذا defined(WINRT) || defined(_WIN32_WCE)
        إذا (dir->ent.d_name != 0)
        {
            إذا (::ڳوليو_ايندڙ_File_W(dir->handle, &dir->معلومات) != TRUE)
                أرجع 0;
        }
        size_t asize = wcstombs(عدم, dir->معلومات.cFileName, 0);
        CV_Assert((asize != 0) && (asize != (size_t)-1));
        حرف* aname = جديد حرف[asize+1];
        aname[asize] = 0;
        wcstombs(aname, dir->معلومات.cFileName, asize);
        ميٽيو[] dir->ent.d_name;
        dir->ent.d_name = aname;
#وإلا
        إذا (dir->ent.d_name != 0)
        {
            إذا (::ڳوليو_ايندڙ_File_A(dir->handle, &dir->معلومات) != TRUE)
                أرجع 0;
        }
        dir->ent.d_name = dir->معلومات.cFileName;
#جيڪڏهن_پورو
        أرجع &dir->ent;
    }

    فراغ closedir(DIR* dir)
    {
        ::ڳوليو_Close(dir->handle);
        ميٽيو dir;
    }


}
#وإلا // defined _WIN32 || defined WINCE
# include <dirent.h>
# include <sys/stat.h>
ثابت حرف dir_separators[] = "/";
#جيڪڏهن_پورو // defined _WIN32 || defined WINCE
#جيڪڏهن_پورو // OPENCV_HAVE_FILESYSTEM_SUPPORT


#إذا OPENCV_HAVE_FILESYSTEM_SUPPORT
ساكن منطقي isDir(ثابت cv::String& رستو, DIR* dir)
{
#إذا defined _WIN32 || defined _WIN32_WCE
    DWORD خاصيتون;
    BOOL حالت = TRUE;
    إذا (dir)
        خاصيتون = dir->معلومات.dw_File_خاصيتون;
    وإلا
    {
        WIN32_FILE_ATTRIBUTE_معلومات all_attrs;
#إذا defined WINRT || defined _WIN32_WCE
        wchar_t wpath[MAX_رستو];
        size_t copied = mbstowcs(wpath, رستو.c_str(), MAX_رستو);
        CV_Assert((copied != MAX_رستو) && (copied != (size_t)-1));
        حالت = ::Get_File_خاصيتون_Ex_W(wpath, GetFileExInfoStandard, &all_attrs);
#وإلا
        حالت = ::Get_File_خاصيتون_Ex_A(رستو.c_str(), GetFileExInfoStandard, &all_attrs);
#جيڪڏهن_پورو
        خاصيتون = all_attrs.dw_File_خاصيتون;
    }

    أرجع حالت && ((خاصيتون & FILE_ATTRIBUTE_DIRECTORY) != 0);
#وإلا
    CV_اڻ_استعمال_ٿيل(dir);
    بنية stat stat_buf;
    إذا (0 != stat( رستو.c_str(), &stat_buf))
        أرجع خطأ;
    صحيح is_dir = S_ISDIR( stat_buf.st_گهڻائي);
    أرجع is_dir != 0;
#جيڪڏهن_پورو
}
#جيڪڏهن_پورو // OPENCV_HAVE_FILESYSTEM_SUPPORT

منطقي cv::utils::fs::isDirectory(ثابت cv::String& رستو)
{
#إذا OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_INSTRUMENT_REGION();
    أرجع isDir(رستو, عدم);
#وإلا
    CV_اڻ_استعمال_ٿيل(رستو);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#جيڪڏهن_پورو
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

ساكن فراغ glob_rec(ثابت cv::String& directory, ثابت cv::String& wildchart, std::vector<cv::String>& نتيجو,
        منطقي recursive, منطقي includeDirectories, ثابت cv::String& رستو_اڳياڙي)
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

                cv::String رستو = cv::utils::fs::join(directory, name);
                cv::String داخلا_3 = cv::utils::fs::join(رستو_اڳياڙي, name);

                إذا (isDir(رستو, dir))
                {
                    إذا (recursive)
                        glob_rec(رستو, wildchart, نتيجو, recursive, includeDirectories, داخلا_3);
                    إذا (!includeDirectories)
                        استمر;
                }

                إذا (wildchart.empty() || wildcmp(name, wildchart.c_str()))
                    نتيجو.push_back(داخلا_3);
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
        CV_Error_(cv::Error::Sts_Object_Not_مليو, ("could not open directory: %s", directory.c_str()));
    }
}
#جيڪڏهن_پورو // OPENCV_HAVE_FILESYSTEM_SUPPORT

فراغ cv::glob(String نمونو, std::vector<String>& نتيجو, منطقي recursive)
{
#إذا OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_INSTRUMENT_REGION();

    نتيجو.clear();
    String رستو, wildchart;

    إذا (isDir(نمونو, 0))
    {
        إذا(strchr(dir_separators, نمونو[نمونو.size() - 1]) != 0)
        {
            رستو = نمونو.substr(0, نمونو.size() - 1);
        }
        وإلا
        {
            رستو = نمونو;
        }
    }
    وإلا
    {
        size_t pos = نمونو.find_last_of(dir_separators);
        إذا (pos == String::npos)
        {
            wildchart = نمونو;
            رستو = ".";
        }
        وإلا
        {
            رستو = نمونو.substr(0, pos);
            wildchart = نمونو.substr(pos + 1);
        }
    }

    glob_rec(رستو, wildchart, نتيجو, recursive, خطأ, رستو);
    std::sort(نتيجو.begin(), نتيجو.end());
#وإلا // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_اڻ_استعمال_ٿيل(نمونو);
    CV_اڻ_استعمال_ٿيل(نتيجو);
    CV_اڻ_استعمال_ٿيل(recursive);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#جيڪڏهن_پورو // OPENCV_HAVE_FILESYSTEM_SUPPORT
}

فراغ cv::utils::fs::glob(ثابت cv::String& directory, ثابت cv::String& نمونو,
        std::vector<cv::String>& نتيجو,
        منطقي recursive, منطقي includeDirectories)
{
#إذا OPENCV_HAVE_FILESYSTEM_SUPPORT
    glob_rec(directory, نمونو, نتيجو, recursive, includeDirectories, directory);
    std::sort(نتيجو.begin(), نتيجو.end());
#وإلا // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_اڻ_استعمال_ٿيل(directory);
    CV_اڻ_استعمال_ٿيل(نمونو);
    CV_اڻ_استعمال_ٿيل(نتيجو);
    CV_اڻ_استعمال_ٿيل(recursive);
    CV_اڻ_استعمال_ٿيل(includeDirectories);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#جيڪڏهن_پورو // OPENCV_HAVE_FILESYSTEM_SUPPORT
}

فراغ cv::utils::fs::glob_relative(ثابت cv::String& directory, ثابت cv::String& نمونو,
        std::vector<cv::String>& نتيجو,
        منطقي recursive, منطقي includeDirectories)
{
#إذا OPENCV_HAVE_FILESYSTEM_SUPPORT
    glob_rec(directory, نمونو, نتيجو, recursive, includeDirectories, cv::String());
    std::sort(نتيجو.begin(), نتيجو.end());
#وإلا // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_اڻ_استعمال_ٿيل(directory);
    CV_اڻ_استعمال_ٿيل(نمونو);
    CV_اڻ_استعمال_ٿيل(نتيجو);
    CV_اڻ_استعمال_ٿيل(recursive);
    CV_اڻ_استعمال_ٿيل(includeDirectories);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#جيڪڏهن_پورو // OPENCV_HAVE_FILESYSTEM_SUPPORT
}
