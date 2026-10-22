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

#అన్తర్భావ "precomp.hpp"

#అన్తర్భావ "opencv2/core/utils/filesystem.hpp"
#అన్తర్భావ "opencv2/core/utils/filesystem.private.hpp"

#ఒకవేళ OPENCV_HAVE_FILESYSTEM_SUPPORT
#ఒకవేళ defined _WIN32 || defined WINCE
# include <windows.h>
స్థిరరాశి అక్షరం dir_separators[] = "/\\";

నామాకాశ
{
    నిర్మాణం dirent
    {
        స్థిరరాశి అక్షరం* d_name;
    };

    నిర్మాణం DIR
    {
#ఒకవేళ defined(WINRT) || defined(_WIN32_WCE)
        WIN32_కనుగొను_DATAW దత్తాంశం;
#లేకపోతే
        WIN32_కనుగొను_DATAA దత్తాంశం;
#అయితే_ముగింపు
        HANDLE handle;
        dirent ent;
#ఒకవేళ defined(WINRT) || defined(_WIN32_WCE)
        DIR() { }
        ~DIR()
        {
            ఒకవేళ (ent.d_name)
                తొలగించు[] ent.d_name;
        }
#అయితే_ముగింపు
    };

    DIR* opendir(స్థిరరాశి అక్షరం* మార్గం)
    {
        DIR* dir = కొత్త DIR;
        dir->ent.d_name = 0;
#ఒకవేళ defined(WINRT) || defined(_WIN32_WCE)
        cv::String నిండిన_మార్గం = cv::String(మార్గం) + "\\*";
        wchar_t wfull_మార్గం[MAX_మార్గం];
        size_t copied = mbstowcs(wfull_మార్గం, నిండిన_మార్గం.c_str(), MAX_మార్గం);
        CV_Assert((copied != MAX_మార్గం) && (copied != (size_t)-1));
        dir->handle = ::కనుగొను_మొదటి_File_Ex_W(wfull_మార్గం, కనుగొను_Ex_Info_Standard,
                        &dir->దత్తాంశం, కనుగొను_Ex_శోధన_Name_సరిపోలిక, ఖాళీ, 0);
#లేకపోతే
        dir->handle = ::కనుగొను_మొదటి_File_Ex_A((cv::String(మార్గం) + "\\*").c_str(),
            కనుగొను_Ex_Info_Standard, &dir->దత్తాంశం, కనుగొను_Ex_శోధన_Name_సరిపోలిక, ఖాళీ, 0);
#అయితే_ముగింపు
        ఒకవేళ(dir->handle == INVALID_HANDLE_విలువ)
        {
            /*closedir will do all cleanup*/
            తొలగించు dir;
            తిరిగి 0;
        }
        తిరిగి dir;
    }

    dirent* readdir(DIR* dir)
    {
#ఒకవేళ defined(WINRT) || defined(_WIN32_WCE)
        ఒకవేళ (dir->ent.d_name != 0)
        {
            ఒకవేళ (::కనుగొను_తదుపరి_File_W(dir->handle, &dir->దత్తాంశం) != TRUE)
                తిరిగి 0;
        }
        size_t asize = wcstombs(ఖాళీ, dir->దత్తాంశం.cFileName, 0);
        CV_Assert((asize != 0) && (asize != (size_t)-1));
        అక్షరం* aname = కొత్త అక్షరం[asize+1];
        aname[asize] = 0;
        wcstombs(aname, dir->దత్తాంశం.cFileName, asize);
        తొలగించు[] dir->ent.d_name;
        dir->ent.d_name = aname;
#లేకపోతే
        ఒకవేళ (dir->ent.d_name != 0)
        {
            ఒకవేళ (::కనుగొను_తదుపరి_File_A(dir->handle, &dir->దత్తాంశం) != TRUE)
                తిరిగి 0;
        }
        dir->ent.d_name = dir->దత్తాంశం.cFileName;
#అయితే_ముగింపు
        తిరిగి &dir->ent;
    }

    శూన్య closedir(DIR* dir)
    {
        ::కనుగొను_Close(dir->handle);
        తొలగించు dir;
    }


}
#లేకపోతే // defined _WIN32 || defined WINCE
# include <dirent.h>
# include <sys/stat.h>
స్థిరరాశి అక్షరం dir_separators[] = "/";
#అయితే_ముగింపు // defined _WIN32 || defined WINCE
#అయితే_ముగింపు // OPENCV_HAVE_FILESYSTEM_SUPPORT


#ఒకవేళ OPENCV_HAVE_FILESYSTEM_SUPPORT
స్థిర తార్కిక isDir(స్థిరరాశి cv::String& మార్గం, DIR* dir)
{
#ఒకవేళ defined _WIN32 || defined _WIN32_WCE
    DWORD లక్షణాలు;
    BOOL స్థితి_2 = TRUE;
    ఒకవేళ (dir)
        లక్షణాలు = dir->దత్తాంశం.dw_File_లక్షణాలు;
    లేకపోతే
    {
        WIN32_FILE_ATTRIBUTE_దత్తాంశం all_attrs;
#ఒకవేళ defined WINRT || defined _WIN32_WCE
        wchar_t wpath[MAX_మార్గం];
        size_t copied = mbstowcs(wpath, మార్గం.c_str(), MAX_మార్గం);
        CV_Assert((copied != MAX_మార్గం) && (copied != (size_t)-1));
        స్థితి_2 = ::Get_File_లక్షణాలు_Ex_W(wpath, GetFileExInfoStandard, &all_attrs);
#లేకపోతే
        స్థితి_2 = ::Get_File_లక్షణాలు_Ex_A(మార్గం.c_str(), GetFileExInfoStandard, &all_attrs);
#అయితే_ముగింపు
        లక్షణాలు = all_attrs.dw_File_లక్షణాలు;
    }

    తిరిగి స్థితి_2 && ((లక్షణాలు & FILE_ATTRIBUTE_DIRECTORY) != 0);
#లేకపోతే
    CV_వాడని(dir);
    నిర్మాణం stat stat_buf;
    ఒకవేళ (0 != stat( మార్గం.c_str(), &stat_buf))
        తిరిగి అబద్ధం;
    పూర్ణాంకం is_dir = S_ISDIR( stat_buf.st_బహుళకం);
    తిరిగి is_dir != 0;
#అయితే_ముగింపు
}
#అయితే_ముగింపు // OPENCV_HAVE_FILESYSTEM_SUPPORT

తార్కిక cv::utils::fs::isDirectory(స్థిరరాశి cv::String& మార్గం)
{
#ఒకవేళ OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_INSTRUMENT_REGION();
    తిరిగి isDir(మార్గం, ఖాళీ);
#లేకపోతే
    CV_వాడని(మార్గం);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#అయితే_ముగింపు
}

#ఒకవేళ OPENCV_HAVE_FILESYSTEM_SUPPORT
స్థిర తార్కిక wildcmp(స్థిరరాశి అక్షరం *string, స్థిరరాశి అక్షరం *wild)
{
    // Based on wildcmp written by Jack Handy - <A href="mailto:jakkhandy@hotmail.com">jakkhandy@hotmail.com</A>
    స్థిరరాశి అక్షరం *cp = 0, *mp = 0;

    అయితే ((*string) && (*wild != '*'))
    {
        ఒకవేళ ((*wild != *string) && (*wild != '?'))
        {
            తిరిగి అబద్ధం;
        }

        wild++;
        string++;
    }

    అయితే (*string)
    {
        ఒకవేళ (*wild == '*')
        {
            ఒకవేళ (!*++wild)
            {
                తిరిగి నిజం;
            }

            mp = wild;
            cp = string + 1;
        }
        లేకపోతే ఒకవేళ ((*wild == *string) || (*wild == '?'))
        {
            wild++;
            string++;
        }
        లేకపోతే
        {
            wild = mp;
            string = cp++;
        }
    }

    అయితే (*wild == '*')
    {
        wild++;
    }

    తిరిగి *wild == 0;
}

స్థిర శూన్య glob_rec(స్థిరరాశి cv::String& directory, స్థిరరాశి cv::String& wildchart, std::vector<cv::String>& ఫలితం,
        తార్కిక recursive, తార్కిక includeDirectories, స్థిరరాశి cv::String& మార్గం_ఉపసర్గ)
{
    DIR *dir;

    ఒకవేళ ((dir = opendir (directory.c_str())) != 0)
    {
        /* find all the files and directories within directory */
        ప్రయత్నించు
        {
            నిర్మాణం dirent *ent;
            అయితే ((ent = readdir (dir)) != 0)
            {
                స్థిరరాశి అక్షరం* name = ent->d_name;
                ఒకవేళ((name[0] == 0) || (name[0] == '.' && name[1] == 0) || (name[0] == '.' && name[1] == '.' && name[2] == 0))
                    కొనసాగు;

                cv::String మార్గం = cv::utils::fs::join(directory, name);
                cv::String నమోదు_2 = cv::utils::fs::join(మార్గం_ఉపసర్గ, name);

                ఒకవేళ (isDir(మార్గం, dir))
                {
                    ఒకవేళ (recursive)
                        glob_rec(మార్గం, wildchart, ఫలితం, recursive, includeDirectories, నమోదు_2);
                    ఒకవేళ (!includeDirectories)
                        కొనసాగు;
                }

                ఒకవేళ (wildchart.empty() || wildcmp(name, wildchart.c_str()))
                    ఫలితం.push_back(నమోదు_2);
            }
        }
        పట్టుకో (...)
        {
            closedir(dir);
            విసురు;
        }
        closedir(dir);
    }
    లేకపోతే
    {
        CV_Error_(cv::Error::Sts_Object_Not_దొరికింది, ("could not open directory: %s", directory.c_str()));
    }
}
#అయితే_ముగింపు // OPENCV_HAVE_FILESYSTEM_SUPPORT

శూన్య cv::glob(String నమూనా, std::vector<String>& ఫలితం, తార్కిక recursive)
{
#ఒకవేళ OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_INSTRUMENT_REGION();

    ఫలితం.clear();
    String మార్గం, wildchart;

    ఒకవేళ (isDir(నమూనా, 0))
    {
        ఒకవేళ(strchr(dir_separators, నమూనా[నమూనా.size() - 1]) != 0)
        {
            మార్గం = నమూనా.substr(0, నమూనా.size() - 1);
        }
        లేకపోతే
        {
            మార్గం = నమూనా;
        }
    }
    లేకపోతే
    {
        size_t pos = నమూనా.find_last_of(dir_separators);
        ఒకవేళ (pos == String::npos)
        {
            wildchart = నమూనా;
            మార్గం = ".";
        }
        లేకపోతే
        {
            మార్గం = నమూనా.substr(0, pos);
            wildchart = నమూనా.substr(pos + 1);
        }
    }

    glob_rec(మార్గం, wildchart, ఫలితం, recursive, అబద్ధం, మార్గం);
    std::sort(ఫలితం.begin(), ఫలితం.end());
#లేకపోతే // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_వాడని(నమూనా);
    CV_వాడని(ఫలితం);
    CV_వాడని(recursive);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#అయితే_ముగింపు // OPENCV_HAVE_FILESYSTEM_SUPPORT
}

శూన్య cv::utils::fs::glob(స్థిరరాశి cv::String& directory, స్థిరరాశి cv::String& నమూనా,
        std::vector<cv::String>& ఫలితం,
        తార్కిక recursive, తార్కిక includeDirectories)
{
#ఒకవేళ OPENCV_HAVE_FILESYSTEM_SUPPORT
    glob_rec(directory, నమూనా, ఫలితం, recursive, includeDirectories, directory);
    std::sort(ఫలితం.begin(), ఫలితం.end());
#లేకపోతే // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_వాడని(directory);
    CV_వాడని(నమూనా);
    CV_వాడని(ఫలితం);
    CV_వాడని(recursive);
    CV_వాడని(includeDirectories);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#అయితే_ముగింపు // OPENCV_HAVE_FILESYSTEM_SUPPORT
}

శూన్య cv::utils::fs::glob_relative(స్థిరరాశి cv::String& directory, స్థిరరాశి cv::String& నమూనా,
        std::vector<cv::String>& ఫలితం,
        తార్కిక recursive, తార్కిక includeDirectories)
{
#ఒకవేళ OPENCV_HAVE_FILESYSTEM_SUPPORT
    glob_rec(directory, నమూనా, ఫలితం, recursive, includeDirectories, cv::String());
    std::sort(ఫలితం.begin(), ఫలితం.end());
#లేకపోతే // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_వాడని(directory);
    CV_వాడని(నమూనా);
    CV_వాడని(ఫలితం);
    CV_వాడని(recursive);
    CV_వాడని(includeDirectories);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#అయితే_ముగింపు // OPENCV_HAVE_FILESYSTEM_SUPPORT
}
