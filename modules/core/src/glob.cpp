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

#ਅਨ੍ਤਰ੍ਭਾਵ "precomp.hpp"

#ਅਨ੍ਤਰ੍ਭਾਵ "opencv2/core/utils/filesystem.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "opencv2/core/utils/filesystem.private.hpp"

#ਜੇ OPENCV_HAVE_FILESYSTEM_SUPPORT
#ਜੇ defined _WIN32 || defined WINCE
# include <windows.h>
ਅਚਲ ਅੱਖਰ dir_separators[] = "/\\";

ਨਾਮਾਕਾਸ਼
{
    ਬਣਤਰ dirent
    {
        ਅਚਲ ਅੱਖਰ* d_name;
    };

    ਬਣਤਰ DIR
    {
#ਜੇ defined(WINRT) || defined(_WIN32_WCE)
        WIN32_ਲੱਭੋ_DATAW ਅੰਕੜੇ;
#ਨਹੀਂਤਾਂ
        WIN32_ਲੱਭੋ_DATAA ਅੰਕੜੇ;
#ਜੇ_ਅੰਤ
        HANDLE handle;
        dirent ent;
#ਜੇ defined(WINRT) || defined(_WIN32_WCE)
        DIR() { }
        ~DIR()
        {
            ਜੇ (ent.d_name)
                ਮਿਟਾਓ[] ent.d_name;
        }
#ਜੇ_ਅੰਤ
    };

    DIR* opendir(ਅਚਲ ਅੱਖਰ* ਰਸਤਾ)
    {
        DIR* dir = ਨਵਾਂ DIR;
        dir->ent.d_name = 0;
#ਜੇ defined(WINRT) || defined(_WIN32_WCE)
        cv::String ਭਰਿਆ_ਰਸਤਾ = cv::String(ਰਸਤਾ) + "\\*";
        wchar_t wfull_ਰਸਤਾ[MAX_ਰਸਤਾ];
        size_t copied = mbstowcs(wfull_ਰਸਤਾ, ਭਰਿਆ_ਰਸਤਾ.c_str(), MAX_ਰਸਤਾ);
        CV_Assert((copied != MAX_ਰਸਤਾ) && (copied != (size_t)-1));
        dir->handle = ::ਲੱਭੋ_ਪਹਿਲਾ_File_Ex_W(wfull_ਰਸਤਾ, ਲੱਭੋ_Ex_Info_Standard,
                        &dir->ਅੰਕੜੇ, ਲੱਭੋ_Ex_ਖੋਜ_Name_ਮੇਲ, ਨਿਰਾ, 0);
#ਨਹੀਂਤਾਂ
        dir->handle = ::ਲੱਭੋ_ਪਹਿਲਾ_File_Ex_A((cv::String(ਰਸਤਾ) + "\\*").c_str(),
            ਲੱਭੋ_Ex_Info_Standard, &dir->ਅੰਕੜੇ, ਲੱਭੋ_Ex_ਖੋਜ_Name_ਮੇਲ, ਨਿਰਾ, 0);
#ਜੇ_ਅੰਤ
        ਜੇ(dir->handle == INVALID_HANDLE_ਮੁੱਲ)
        {
            /*closedir will do all cleanup*/
            ਮਿਟਾਓ dir;
            ਵਾਪਸ 0;
        }
        ਵਾਪਸ dir;
    }

    dirent* readdir(DIR* dir)
    {
#ਜੇ defined(WINRT) || defined(_WIN32_WCE)
        ਜੇ (dir->ent.d_name != 0)
        {
            ਜੇ (::ਲੱਭੋ_ਅਗਲਾ_File_W(dir->handle, &dir->ਅੰਕੜੇ) != TRUE)
                ਵਾਪਸ 0;
        }
        size_t asize = wcstombs(ਨਿਰਾ, dir->ਅੰਕੜੇ.cFileName, 0);
        CV_Assert((asize != 0) && (asize != (size_t)-1));
        ਅੱਖਰ* aname = ਨਵਾਂ ਅੱਖਰ[asize+1];
        aname[asize] = 0;
        wcstombs(aname, dir->ਅੰਕੜੇ.cFileName, asize);
        ਮਿਟਾਓ[] dir->ent.d_name;
        dir->ent.d_name = aname;
#ਨਹੀਂਤਾਂ
        ਜੇ (dir->ent.d_name != 0)
        {
            ਜੇ (::ਲੱਭੋ_ਅਗਲਾ_File_A(dir->handle, &dir->ਅੰਕੜੇ) != TRUE)
                ਵਾਪਸ 0;
        }
        dir->ent.d_name = dir->ਅੰਕੜੇ.cFileName;
#ਜੇ_ਅੰਤ
        ਵਾਪਸ &dir->ent;
    }

    ਖਾਲੀ closedir(DIR* dir)
    {
        ::ਲੱਭੋ_Close(dir->handle);
        ਮਿਟਾਓ dir;
    }


}
#ਨਹੀਂਤਾਂ // defined _WIN32 || defined WINCE
# include <dirent.h>
# include <sys/stat.h>
ਅਚਲ ਅੱਖਰ dir_separators[] = "/";
#ਜੇ_ਅੰਤ // defined _WIN32 || defined WINCE
#ਜੇ_ਅੰਤ // OPENCV_HAVE_FILESYSTEM_SUPPORT


#ਜੇ OPENCV_HAVE_FILESYSTEM_SUPPORT
ਸਥਿਰ ਤਾਰਕਿਕ isDir(ਅਚਲ cv::String& ਰਸਤਾ, DIR* dir)
{
#ਜੇ defined _WIN32 || defined _WIN32_WCE
    DWORD ਗੁਣ;
    BOOL ਸਥਿਤੀ = TRUE;
    ਜੇ (dir)
        ਗੁਣ = dir->ਅੰਕੜੇ.dw_File_ਗੁਣ;
    ਨਹੀਂਤਾਂ
    {
        WIN32_FILE_ATTRIBUTE_ਅੰਕੜੇ all_attrs;
#ਜੇ defined WINRT || defined _WIN32_WCE
        wchar_t wpath[MAX_ਰਸਤਾ];
        size_t copied = mbstowcs(wpath, ਰਸਤਾ.c_str(), MAX_ਰਸਤਾ);
        CV_Assert((copied != MAX_ਰਸਤਾ) && (copied != (size_t)-1));
        ਸਥਿਤੀ = ::Get_File_ਗੁਣ_Ex_W(wpath, GetFileExInfoStandard, &all_attrs);
#ਨਹੀਂਤਾਂ
        ਸਥਿਤੀ = ::Get_File_ਗੁਣ_Ex_A(ਰਸਤਾ.c_str(), GetFileExInfoStandard, &all_attrs);
#ਜੇ_ਅੰਤ
        ਗੁਣ = all_attrs.dw_File_ਗੁਣ;
    }

    ਵਾਪਸ ਸਥਿਤੀ && ((ਗੁਣ & FILE_ATTRIBUTE_DIRECTORY) != 0);
#ਨਹੀਂਤਾਂ
    CV_ਅਣਵਰਤਿਆ(dir);
    ਬਣਤਰ stat stat_buf;
    ਜੇ (0 != stat( ਰਸਤਾ.c_str(), &stat_buf))
        ਵਾਪਸ ਝੂਠ;
    ਪੂਰਨ is_dir = S_ISDIR( stat_buf.st_ਬਹੁਲਕ);
    ਵਾਪਸ is_dir != 0;
#ਜੇ_ਅੰਤ
}
#ਜੇ_ਅੰਤ // OPENCV_HAVE_FILESYSTEM_SUPPORT

ਤਾਰਕਿਕ cv::utils::fs::isDirectory(ਅਚਲ cv::String& ਰਸਤਾ)
{
#ਜੇ OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_INSTRUMENT_REGION();
    ਵਾਪਸ isDir(ਰਸਤਾ, ਨਿਰਾ);
#ਨਹੀਂਤਾਂ
    CV_ਅਣਵਰਤਿਆ(ਰਸਤਾ);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#ਜੇ_ਅੰਤ
}

#ਜੇ OPENCV_HAVE_FILESYSTEM_SUPPORT
ਸਥਿਰ ਤਾਰਕਿਕ wildcmp(ਅਚਲ ਅੱਖਰ *string, ਅਚਲ ਅੱਖਰ *wild)
{
    // Based on wildcmp written by Jack Handy - <A href="mailto:jakkhandy@hotmail.com">jakkhandy@hotmail.com</A>
    ਅਚਲ ਅੱਖਰ *cp = 0, *mp = 0;

    ਜਦੋਂ ((*string) && (*wild != '*'))
    {
        ਜੇ ((*wild != *string) && (*wild != '?'))
        {
            ਵਾਪਸ ਝੂਠ;
        }

        wild++;
        string++;
    }

    ਜਦੋਂ (*string)
    {
        ਜੇ (*wild == '*')
        {
            ਜੇ (!*++wild)
            {
                ਵਾਪਸ ਸੱਚ;
            }

            mp = wild;
            cp = string + 1;
        }
        ਨਹੀਂਤਾਂ ਜੇ ((*wild == *string) || (*wild == '?'))
        {
            wild++;
            string++;
        }
        ਨਹੀਂਤਾਂ
        {
            wild = mp;
            string = cp++;
        }
    }

    ਜਦੋਂ (*wild == '*')
    {
        wild++;
    }

    ਵਾਪਸ *wild == 0;
}

ਸਥਿਰ ਖਾਲੀ glob_rec(ਅਚਲ cv::String& directory, ਅਚਲ cv::String& wildchart, std::vector<cv::String>& ਨਤੀਜਾ,
        ਤਾਰਕਿਕ recursive, ਤਾਰਕਿਕ includeDirectories, ਅਚਲ cv::String& ਰਸਤਾ_ਅਗੇਤਰ)
{
    DIR *dir;

    ਜੇ ((dir = opendir (directory.c_str())) != 0)
    {
        /* find all the files and directories within directory */
        ਕੋਸ਼ਿਸ਼
        {
            ਬਣਤਰ dirent *ent;
            ਜਦੋਂ ((ent = readdir (dir)) != 0)
            {
                ਅਚਲ ਅੱਖਰ* name = ent->d_name;
                ਜੇ((name[0] == 0) || (name[0] == '.' && name[1] == 0) || (name[0] == '.' && name[1] == '.' && name[2] == 0))
                    ਜਾਰੀ;

                cv::String ਰਸਤਾ = cv::utils::fs::join(directory, name);
                cv::String ਇੰਦਰਾਜ = cv::utils::fs::join(ਰਸਤਾ_ਅਗੇਤਰ, name);

                ਜੇ (isDir(ਰਸਤਾ, dir))
                {
                    ਜੇ (recursive)
                        glob_rec(ਰਸਤਾ, wildchart, ਨਤੀਜਾ, recursive, includeDirectories, ਇੰਦਰਾਜ);
                    ਜੇ (!includeDirectories)
                        ਜਾਰੀ;
                }

                ਜੇ (wildchart.empty() || wildcmp(name, wildchart.c_str()))
                    ਨਤੀਜਾ.push_back(ਇੰਦਰਾਜ);
            }
        }
        ਫੜੋ (...)
        {
            closedir(dir);
            ਸੁੱਟੋ;
        }
        closedir(dir);
    }
    ਨਹੀਂਤਾਂ
    {
        CV_Error_(cv::Error::Sts_Object_Not_ਮਿਲਿਆ, ("could not open directory: %s", directory.c_str()));
    }
}
#ਜੇ_ਅੰਤ // OPENCV_HAVE_FILESYSTEM_SUPPORT

ਖਾਲੀ cv::glob(String ਨਮੂਨਾ, std::vector<String>& ਨਤੀਜਾ, ਤਾਰਕਿਕ recursive)
{
#ਜੇ OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_INSTRUMENT_REGION();

    ਨਤੀਜਾ.clear();
    String ਰਸਤਾ, wildchart;

    ਜੇ (isDir(ਨਮੂਨਾ, 0))
    {
        ਜੇ(strchr(dir_separators, ਨਮੂਨਾ[ਨਮੂਨਾ.size() - 1]) != 0)
        {
            ਰਸਤਾ = ਨਮੂਨਾ.substr(0, ਨਮੂਨਾ.size() - 1);
        }
        ਨਹੀਂਤਾਂ
        {
            ਰਸਤਾ = ਨਮੂਨਾ;
        }
    }
    ਨਹੀਂਤਾਂ
    {
        size_t pos = ਨਮੂਨਾ.find_last_of(dir_separators);
        ਜੇ (pos == String::npos)
        {
            wildchart = ਨਮੂਨਾ;
            ਰਸਤਾ = ".";
        }
        ਨਹੀਂਤਾਂ
        {
            ਰਸਤਾ = ਨਮੂਨਾ.substr(0, pos);
            wildchart = ਨਮੂਨਾ.substr(pos + 1);
        }
    }

    glob_rec(ਰਸਤਾ, wildchart, ਨਤੀਜਾ, recursive, ਝੂਠ, ਰਸਤਾ);
    std::sort(ਨਤੀਜਾ.begin(), ਨਤੀਜਾ.end());
#ਨਹੀਂਤਾਂ // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_ਅਣਵਰਤਿਆ(ਨਮੂਨਾ);
    CV_ਅਣਵਰਤਿਆ(ਨਤੀਜਾ);
    CV_ਅਣਵਰਤਿਆ(recursive);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#ਜੇ_ਅੰਤ // OPENCV_HAVE_FILESYSTEM_SUPPORT
}

ਖਾਲੀ cv::utils::fs::glob(ਅਚਲ cv::String& directory, ਅਚਲ cv::String& ਨਮੂਨਾ,
        std::vector<cv::String>& ਨਤੀਜਾ,
        ਤਾਰਕਿਕ recursive, ਤਾਰਕਿਕ includeDirectories)
{
#ਜੇ OPENCV_HAVE_FILESYSTEM_SUPPORT
    glob_rec(directory, ਨਮੂਨਾ, ਨਤੀਜਾ, recursive, includeDirectories, directory);
    std::sort(ਨਤੀਜਾ.begin(), ਨਤੀਜਾ.end());
#ਨਹੀਂਤਾਂ // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_ਅਣਵਰਤਿਆ(directory);
    CV_ਅਣਵਰਤਿਆ(ਨਮੂਨਾ);
    CV_ਅਣਵਰਤਿਆ(ਨਤੀਜਾ);
    CV_ਅਣਵਰਤਿਆ(recursive);
    CV_ਅਣਵਰਤਿਆ(includeDirectories);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#ਜੇ_ਅੰਤ // OPENCV_HAVE_FILESYSTEM_SUPPORT
}

ਖਾਲੀ cv::utils::fs::glob_relative(ਅਚਲ cv::String& directory, ਅਚਲ cv::String& ਨਮੂਨਾ,
        std::vector<cv::String>& ਨਤੀਜਾ,
        ਤਾਰਕਿਕ recursive, ਤਾਰਕਿਕ includeDirectories)
{
#ਜੇ OPENCV_HAVE_FILESYSTEM_SUPPORT
    glob_rec(directory, ਨਮੂਨਾ, ਨਤੀਜਾ, recursive, includeDirectories, cv::String());
    std::sort(ਨਤੀਜਾ.begin(), ਨਤੀਜਾ.end());
#ਨਹੀਂਤਾਂ // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_ਅਣਵਰਤਿਆ(directory);
    CV_ਅਣਵਰਤਿਆ(ਨਮੂਨਾ);
    CV_ਅਣਵਰਤਿਆ(ਨਤੀਜਾ);
    CV_ਅਣਵਰਤਿਆ(recursive);
    CV_ਅਣਵਰਤਿਆ(includeDirectories);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#ਜੇ_ਅੰਤ // OPENCV_HAVE_FILESYSTEM_SUPPORT
}
