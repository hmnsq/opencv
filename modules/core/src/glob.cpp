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

#ಅನ್ತರ್ಭಾವ "precomp.hpp"

#ಅನ್ತರ್ಭಾವ "opencv2/core/utils/filesystem.hpp"
#ಅನ್ತರ್ಭಾವ "opencv2/core/utils/filesystem.private.hpp"

#ಆದರೆ OPENCV_HAVE_FILESYSTEM_SUPPORT
#ಆದರೆ defined _WIN32 || defined WINCE
# include <windows.h>
ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ dir_separators[] = "/\\";

ನಾಮಾಕಾಶ
{
    ರಚನೆ dirent
    {
        ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* d_name;
    };

    ರಚನೆ DIR
    {
#ಆದರೆ defined(WINRT) || defined(_WIN32_WCE)
        WIN32_ಕಂಡುಹಿಡಿ_DATAW ದತ್ತಾಂಶ;
#ಇಲ್ಲದಿದ್ದರೆ
        WIN32_ಕಂಡುಹಿಡಿ_DATAA ದತ್ತಾಂಶ;
#ಆದರೆ_ಅಂತ್ಯ
        HANDLE handle;
        dirent ent;
#ಆದರೆ defined(WINRT) || defined(_WIN32_WCE)
        DIR() { }
        ~DIR()
        {
            ಆದರೆ (ent.d_name)
                ಅಳಿಸು[] ent.d_name;
        }
#ಆದರೆ_ಅಂತ್ಯ
    };

    DIR* opendir(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* ಮಾರ್ಗ)
    {
        DIR* dir = ಹೊಸ DIR;
        dir->ent.d_name = 0;
#ಆದರೆ defined(WINRT) || defined(_WIN32_WCE)
        cv::String ತುಂಬಿದ_ಮಾರ್ಗ = cv::String(ಮಾರ್ಗ) + "\\*";
        wchar_t wfull_ಮಾರ್ಗ[MAX_ಮಾರ್ಗ];
        size_t copied = mbstowcs(wfull_ಮಾರ್ಗ, ತುಂಬಿದ_ಮಾರ್ಗ.c_str(), MAX_ಮಾರ್ಗ);
        CV_Assert((copied != MAX_ಮಾರ್ಗ) && (copied != (size_t)-1));
        dir->handle = ::ಕಂಡುಹಿಡಿ_ಮೊದಲ_File_Ex_W(wfull_ಮಾರ್ಗ, ಕಂಡುಹಿಡಿ_Ex_Info_Standard,
                        &dir->ದತ್ತಾಂಶ, ಕಂಡುಹಿಡಿ_Ex_ಹುಡುಕು_Name_ಹೊಂದಿಕೆ, ಖಾಲಿ, 0);
#ಇಲ್ಲದಿದ್ದರೆ
        dir->handle = ::ಕಂಡುಹಿಡಿ_ಮೊದಲ_File_Ex_A((cv::String(ಮಾರ್ಗ) + "\\*").c_str(),
            ಕಂಡುಹಿಡಿ_Ex_Info_Standard, &dir->ದತ್ತಾಂಶ, ಕಂಡುಹಿಡಿ_Ex_ಹುಡುಕು_Name_ಹೊಂದಿಕೆ, ಖಾಲಿ, 0);
#ಆದರೆ_ಅಂತ್ಯ
        ಆದರೆ(dir->handle == INVALID_HANDLE_ಮೌಲ್ಯ)
        {
            /*closedir will do all cleanup*/
            ಅಳಿಸು dir;
            ಹಿಂತಿರುಗಿ 0;
        }
        ಹಿಂತಿರುಗಿ dir;
    }

    dirent* readdir(DIR* dir)
    {
#ಆದರೆ defined(WINRT) || defined(_WIN32_WCE)
        ಆದರೆ (dir->ent.d_name != 0)
        {
            ಆದರೆ (::ಕಂಡುಹಿಡಿ_ಮುಂದಿನ_File_W(dir->handle, &dir->ದತ್ತಾಂಶ) != TRUE)
                ಹಿಂತಿರುಗಿ 0;
        }
        size_t asize = wcstombs(ಖಾಲಿ, dir->ದತ್ತಾಂಶ.cFileName, 0);
        CV_Assert((asize != 0) && (asize != (size_t)-1));
        ಅಕ್ಷರ* aname = ಹೊಸ ಅಕ್ಷರ[asize+1];
        aname[asize] = 0;
        wcstombs(aname, dir->ದತ್ತಾಂಶ.cFileName, asize);
        ಅಳಿಸು[] dir->ent.d_name;
        dir->ent.d_name = aname;
#ಇಲ್ಲದಿದ್ದರೆ
        ಆದರೆ (dir->ent.d_name != 0)
        {
            ಆದರೆ (::ಕಂಡುಹಿಡಿ_ಮುಂದಿನ_File_A(dir->handle, &dir->ದತ್ತಾಂಶ) != TRUE)
                ಹಿಂತಿರುಗಿ 0;
        }
        dir->ent.d_name = dir->ದತ್ತಾಂಶ.cFileName;
#ಆದರೆ_ಅಂತ್ಯ
        ಹಿಂತಿರುಗಿ &dir->ent;
    }

    ಶೂನ್ಯ closedir(DIR* dir)
    {
        ::ಕಂಡುಹಿಡಿ_Close(dir->handle);
        ಅಳಿಸು dir;
    }


}
#ಇಲ್ಲದಿದ್ದರೆ // defined _WIN32 || defined WINCE
# include <dirent.h>
# include <sys/stat.h>
ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ dir_separators[] = "/";
#ಆದರೆ_ಅಂತ್ಯ // defined _WIN32 || defined WINCE
#ಆದರೆ_ಅಂತ್ಯ // OPENCV_HAVE_FILESYSTEM_SUPPORT


#ಆದರೆ OPENCV_HAVE_FILESYSTEM_SUPPORT
ಸ್ಥಿರ ತಾರ್ಕಿಕ isDir(ಸ್ಥಿರಾಂಕ cv::String& ಮಾರ್ಗ, DIR* dir)
{
#ಆದರೆ defined _WIN32 || defined _WIN32_WCE
    DWORD ಗುಣಲಕ್ಷಣಗಳು;
    BOOL ಸ್ಥಿತಿ_2 = TRUE;
    ಆದರೆ (dir)
        ಗುಣಲಕ್ಷಣಗಳು = dir->ದತ್ತಾಂಶ.dw_File_ಗುಣಲಕ್ಷಣಗಳು;
    ಇಲ್ಲದಿದ್ದರೆ
    {
        WIN32_FILE_ATTRIBUTE_ದತ್ತಾಂಶ all_attrs;
#ಆದರೆ defined WINRT || defined _WIN32_WCE
        wchar_t wpath[MAX_ಮಾರ್ಗ];
        size_t copied = mbstowcs(wpath, ಮಾರ್ಗ.c_str(), MAX_ಮಾರ್ಗ);
        CV_Assert((copied != MAX_ಮಾರ್ಗ) && (copied != (size_t)-1));
        ಸ್ಥಿತಿ_2 = ::Get_File_ಗುಣಲಕ್ಷಣಗಳು_Ex_W(wpath, GetFileExInfoStandard, &all_attrs);
#ಇಲ್ಲದಿದ್ದರೆ
        ಸ್ಥಿತಿ_2 = ::Get_File_ಗುಣಲಕ್ಷಣಗಳು_Ex_A(ಮಾರ್ಗ.c_str(), GetFileExInfoStandard, &all_attrs);
#ಆದರೆ_ಅಂತ್ಯ
        ಗುಣಲಕ್ಷಣಗಳು = all_attrs.dw_File_ಗುಣಲಕ್ಷಣಗಳು;
    }

    ಹಿಂತಿರುಗಿ ಸ್ಥಿತಿ_2 && ((ಗುಣಲಕ್ಷಣಗಳು & FILE_ATTRIBUTE_DIRECTORY) != 0);
#ಇಲ್ಲದಿದ್ದರೆ
    CV_ಬಳಸದ(dir);
    ರಚನೆ stat stat_buf;
    ಆದರೆ (0 != stat( ಮಾರ್ಗ.c_str(), &stat_buf))
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
    ಪೂರ್ಣಾಂಕ is_dir = S_ISDIR( stat_buf.st_ಬಹುಲಕ);
    ಹಿಂತಿರುಗಿ is_dir != 0;
#ಆದರೆ_ಅಂತ್ಯ
}
#ಆದರೆ_ಅಂತ್ಯ // OPENCV_HAVE_FILESYSTEM_SUPPORT

ತಾರ್ಕಿಕ cv::utils::fs::isDirectory(ಸ್ಥಿರಾಂಕ cv::String& ಮಾರ್ಗ)
{
#ಆದರೆ OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_INSTRUMENT_REGION();
    ಹಿಂತಿರುಗಿ isDir(ಮಾರ್ಗ, ಖಾಲಿ);
#ಇಲ್ಲದಿದ್ದರೆ
    CV_ಬಳಸದ(ಮಾರ್ಗ);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#ಆದರೆ_ಅಂತ್ಯ
}

#ಆದರೆ OPENCV_HAVE_FILESYSTEM_SUPPORT
ಸ್ಥಿರ ತಾರ್ಕಿಕ wildcmp(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *string, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *wild)
{
    // Based on wildcmp written by Jack Handy - <A href="mailto:jakkhandy@hotmail.com">jakkhandy@hotmail.com</A>
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *cp = 0, *mp = 0;

    ಆಗ ((*string) && (*wild != '*'))
    {
        ಆದರೆ ((*wild != *string) && (*wild != '?'))
        {
            ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
        }

        wild++;
        string++;
    }

    ಆಗ (*string)
    {
        ಆದರೆ (*wild == '*')
        {
            ಆದರೆ (!*++wild)
            {
                ಹಿಂತಿರುಗಿ ನಿಜ;
            }

            mp = wild;
            cp = string + 1;
        }
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ((*wild == *string) || (*wild == '?'))
        {
            wild++;
            string++;
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            wild = mp;
            string = cp++;
        }
    }

    ಆಗ (*wild == '*')
    {
        wild++;
    }

    ಹಿಂತಿರುಗಿ *wild == 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ glob_rec(ಸ್ಥಿರಾಂಕ cv::String& directory, ಸ್ಥಿರಾಂಕ cv::String& wildchart, std::vector<cv::String>& ಫಲಿತಾಂಶ,
        ತಾರ್ಕಿಕ recursive, ತಾರ್ಕಿಕ includeDirectories, ಸ್ಥಿರಾಂಕ cv::String& ಮಾರ್ಗ_ಪೂರ್ವಪ್ರತ್ಯಯ)
{
    DIR *dir;

    ಆದರೆ ((dir = opendir (directory.c_str())) != 0)
    {
        /* find all the files and directories within directory */
        ಪ್ರಯತ್ನಿಸು
        {
            ರಚನೆ dirent *ent;
            ಆಗ ((ent = readdir (dir)) != 0)
            {
                ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* name = ent->d_name;
                ಆದರೆ((name[0] == 0) || (name[0] == '.' && name[1] == 0) || (name[0] == '.' && name[1] == '.' && name[2] == 0))
                    ಮುಂದುವರಿ;

                cv::String ಮಾರ್ಗ = cv::utils::fs::join(directory, name);
                cv::String ನಮೂದು = cv::utils::fs::join(ಮಾರ್ಗ_ಪೂರ್ವಪ್ರತ್ಯಯ, name);

                ಆದರೆ (isDir(ಮಾರ್ಗ, dir))
                {
                    ಆದರೆ (recursive)
                        glob_rec(ಮಾರ್ಗ, wildchart, ಫಲಿತಾಂಶ, recursive, includeDirectories, ನಮೂದು);
                    ಆದರೆ (!includeDirectories)
                        ಮುಂದುವರಿ;
                }

                ಆದರೆ (wildchart.empty() || wildcmp(name, wildchart.c_str()))
                    ಫಲಿತಾಂಶ.push_back(ನಮೂದು);
            }
        }
        ಹಿಡಿ (...)
        {
            closedir(dir);
            ಎಸೆ;
        }
        closedir(dir);
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        CV_Error_(cv::Error::Sts_Object_Not_ಸಿಕ್ಕಿತು, ("could not open directory: %s", directory.c_str()));
    }
}
#ಆದರೆ_ಅಂತ್ಯ // OPENCV_HAVE_FILESYSTEM_SUPPORT

ಶೂನ್ಯ cv::glob(String ನಮೂನೆ, std::vector<String>& ಫಲಿತಾಂಶ, ತಾರ್ಕಿಕ recursive)
{
#ಆದರೆ OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_INSTRUMENT_REGION();

    ಫಲಿತಾಂಶ.clear();
    String ಮಾರ್ಗ, wildchart;

    ಆದರೆ (isDir(ನಮೂನೆ, 0))
    {
        ಆದರೆ(strchr(dir_separators, ನಮೂನೆ[ನಮೂನೆ.size() - 1]) != 0)
        {
            ಮಾರ್ಗ = ನಮೂನೆ.substr(0, ನಮೂನೆ.size() - 1);
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            ಮಾರ್ಗ = ನಮೂನೆ;
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        size_t pos = ನಮೂನೆ.find_last_of(dir_separators);
        ಆದರೆ (pos == String::npos)
        {
            wildchart = ನಮೂನೆ;
            ಮಾರ್ಗ = ".";
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            ಮಾರ್ಗ = ನಮೂನೆ.substr(0, pos);
            wildchart = ನಮೂನೆ.substr(pos + 1);
        }
    }

    glob_rec(ಮಾರ್ಗ, wildchart, ಫಲಿತಾಂಶ, recursive, ಸುಳ್ಳು, ಮಾರ್ಗ);
    std::sort(ಫಲಿತಾಂಶ.begin(), ಫಲಿತಾಂಶ.end());
#ಇಲ್ಲದಿದ್ದರೆ // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_ಬಳಸದ(ನಮೂನೆ);
    CV_ಬಳಸದ(ಫಲಿತಾಂಶ);
    CV_ಬಳಸದ(recursive);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#ಆದರೆ_ಅಂತ್ಯ // OPENCV_HAVE_FILESYSTEM_SUPPORT
}

ಶೂನ್ಯ cv::utils::fs::glob(ಸ್ಥಿರಾಂಕ cv::String& directory, ಸ್ಥಿರಾಂಕ cv::String& ನಮೂನೆ,
        std::vector<cv::String>& ಫಲಿತಾಂಶ,
        ತಾರ್ಕಿಕ recursive, ತಾರ್ಕಿಕ includeDirectories)
{
#ಆದರೆ OPENCV_HAVE_FILESYSTEM_SUPPORT
    glob_rec(directory, ನಮೂನೆ, ಫಲಿತಾಂಶ, recursive, includeDirectories, directory);
    std::sort(ಫಲಿತಾಂಶ.begin(), ಫಲಿತಾಂಶ.end());
#ಇಲ್ಲದಿದ್ದರೆ // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_ಬಳಸದ(directory);
    CV_ಬಳಸದ(ನಮೂನೆ);
    CV_ಬಳಸದ(ಫಲಿತಾಂಶ);
    CV_ಬಳಸದ(recursive);
    CV_ಬಳಸದ(includeDirectories);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#ಆದರೆ_ಅಂತ್ಯ // OPENCV_HAVE_FILESYSTEM_SUPPORT
}

ಶೂನ್ಯ cv::utils::fs::glob_relative(ಸ್ಥಿರಾಂಕ cv::String& directory, ಸ್ಥಿರಾಂಕ cv::String& ನಮೂನೆ,
        std::vector<cv::String>& ಫಲಿತಾಂಶ,
        ತಾರ್ಕಿಕ recursive, ತಾರ್ಕಿಕ includeDirectories)
{
#ಆದರೆ OPENCV_HAVE_FILESYSTEM_SUPPORT
    glob_rec(directory, ನಮೂನೆ, ಫಲಿತಾಂಶ, recursive, includeDirectories, cv::String());
    std::sort(ಫಲಿತಾಂಶ.begin(), ಫಲಿತಾಂಶ.end());
#ಇಲ್ಲದಿದ್ದರೆ // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_ಬಳಸದ(directory);
    CV_ಬಳಸದ(ನಮೂನೆ);
    CV_ಬಳಸದ(ಫಲಿತಾಂಶ);
    CV_ಬಳಸದ(recursive);
    CV_ಬಳಸದ(includeDirectories);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#ಆದರೆ_ಅಂತ್ಯ // OPENCV_HAVE_FILESYSTEM_SUPPORT
}
