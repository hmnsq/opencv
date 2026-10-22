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

#अन्तर्भाव "precomp.hpp"

#अन्तर्भाव "opencv2/core/utils/filesystem.hpp"
#अन्तर्भाव "opencv2/core/utils/filesystem.private.hpp"

#यदि OPENCV_HAVE_FILESYSTEM_SUPPORT
#यदि defined _WIN32 || defined WINCE
# include <windows.h>
ध्रुव वर्ण dir_separators[] = "/\\";

नामाकाश
{
    संरचना dirent
    {
        ध्रुव वर्ण* d_name;
    };

    संरचना DIR
    {
#यदि defined(WINRT) || defined(_WIN32_WCE)
        WIN32_FIND_DATAW दत्त;
#अन्यथा
        WIN32_FIND_DATAA दत्त;
#यदि_अन्तः
        HANDLE handle;
        dirent ent;
#यदि defined(WINRT) || defined(_WIN32_WCE)
        DIR() { }
        ~DIR()
        {
            यदि (ent.d_name)
                विलोपय[] ent.d_name;
        }
#यदि_अन्तः
    };

    DIR* opendir(ध्रुव वर्ण* मार्गः)
    {
        DIR* dir = नूतन DIR;
        dir->ent.d_name = 0;
#यदि defined(WINRT) || defined(_WIN32_WCE)
        cv::String पूर्णम्_मार्गः = cv::String(मार्गः) + "\\*";
        wchar_t wfull_मार्गः[MAX_मार्गः];
        size_t copied = mbstowcs(wfull_मार्गः, पूर्णम्_मार्गः.c_str(), MAX_मार्गः);
        CV_Assert((copied != MAX_मार्गः) && (copied != (size_t)-1));
        dir->handle = ::FindFirstFileExW(wfull_मार्गः, FindExInfoStandard,
                        &dir->दत्त, Find_Ex_अन्वेषणम्_Name_मेलनम्, रिक्त, 0);
#अन्यथा
        dir->handle = ::FindFirstFileExA((cv::String(मार्गः) + "\\*").c_str(),
            FindExInfoStandard, &dir->दत्त, Find_Ex_अन्वेषणम्_Name_मेलनम्, रिक्त, 0);
#यदि_अन्तः
        यदि(dir->handle == INVALID_HANDLE_मानम्)
        {
            /*closedir will do all cleanup*/
            विलोपय dir;
            प्रत्यागम 0;
        }
        प्रत्यागम dir;
    }

    dirent* readdir(DIR* dir)
    {
#यदि defined(WINRT) || defined(_WIN32_WCE)
        यदि (dir->ent.d_name != 0)
        {
            यदि (::Find_अग्र_File_W(dir->handle, &dir->दत्त) != TRUE)
                प्रत्यागम 0;
        }
        size_t asize = wcstombs(रिक्त, dir->दत्त.cFileName, 0);
        CV_Assert((asize != 0) && (asize != (size_t)-1));
        वर्ण* aname = नूतन वर्ण[asize+1];
        aname[asize] = 0;
        wcstombs(aname, dir->दत्त.cFileName, asize);
        विलोपय[] dir->ent.d_name;
        dir->ent.d_name = aname;
#अन्यथा
        यदि (dir->ent.d_name != 0)
        {
            यदि (::Find_अग्र_File_A(dir->handle, &dir->दत्त) != TRUE)
                प्रत्यागम 0;
        }
        dir->ent.d_name = dir->दत्त.cFileName;
#यदि_अन्तः
        प्रत्यागम &dir->ent;
    }

    शून्य closedir(DIR* dir)
    {
        ::FindClose(dir->handle);
        विलोपय dir;
    }


}
#अन्यथा // defined _WIN32 || defined WINCE
# include <dirent.h>
# include <sys/stat.h>
ध्रुव वर्ण dir_separators[] = "/";
#यदि_अन्तः // defined _WIN32 || defined WINCE
#यदि_अन्तः // OPENCV_HAVE_FILESYSTEM_SUPPORT


#यदि OPENCV_HAVE_FILESYSTEM_SUPPORT
स्थिर तार्किक isDir(ध्रुव cv::String& मार्गः, DIR* dir)
{
#यदि defined _WIN32 || defined _WIN32_WCE
    DWORD गुणाः;
    BOOL स्थितिः = TRUE;
    यदि (dir)
        गुणाः = dir->दत्त.dw_File_गुणाः;
    अन्यथा
    {
        WIN32_FILE_ATTRIBUTE_दत्त all_attrs;
#यदि defined WINRT || defined _WIN32_WCE
        wchar_t wpath[MAX_मार्गः];
        size_t copied = mbstowcs(wpath, मार्गः.c_str(), MAX_मार्गः);
        CV_Assert((copied != MAX_मार्गः) && (copied != (size_t)-1));
        स्थितिः = ::Get_File_गुणाः_Ex_W(wpath, GetFileExInfoStandard, &all_attrs);
#अन्यथा
        स्थितिः = ::Get_File_गुणाः_Ex_A(मार्गः.c_str(), GetFileExInfoStandard, &all_attrs);
#यदि_अन्तः
        गुणाः = all_attrs.dw_File_गुणाः;
    }

    प्रत्यागम स्थितिः && ((गुणाः & FILE_ATTRIBUTE_DIRECTORY) != 0);
#अन्यथा
    CV_अप्रयुक्तम्(dir);
    संरचना stat stat_buf;
    यदि (0 != stat( मार्गः.c_str(), &stat_buf))
        प्रत्यागम असत्य;
    पूर्णाङ्क is_dir = S_ISDIR( stat_buf.st_बहुलकम्);
    प्रत्यागम is_dir != 0;
#यदि_अन्तः
}
#यदि_अन्तः // OPENCV_HAVE_FILESYSTEM_SUPPORT

तार्किक cv::utils::fs::isDirectory(ध्रुव cv::String& मार्गः)
{
#यदि OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_INSTRUMENT_REGION();
    प्रत्यागम isDir(मार्गः, रिक्त);
#अन्यथा
    CV_अप्रयुक्तम्(मार्गः);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#यदि_अन्तः
}

#यदि OPENCV_HAVE_FILESYSTEM_SUPPORT
स्थिर तार्किक wildcmp(ध्रुव वर्ण *string, ध्रुव वर्ण *wild)
{
    // Based on wildcmp written by Jack Handy - <A href="mailto:jakkhandy@hotmail.com">jakkhandy@hotmail.com</A>
    ध्रुव वर्ण *cp = 0, *mp = 0;

    यावत् ((*string) && (*wild != '*'))
    {
        यदि ((*wild != *string) && (*wild != '?'))
        {
            प्रत्यागम असत्य;
        }

        wild++;
        string++;
    }

    यावत् (*string)
    {
        यदि (*wild == '*')
        {
            यदि (!*++wild)
            {
                प्रत्यागम सत्य;
            }

            mp = wild;
            cp = string + 1;
        }
        अन्यथा यदि ((*wild == *string) || (*wild == '?'))
        {
            wild++;
            string++;
        }
        अन्यथा
        {
            wild = mp;
            string = cp++;
        }
    }

    यावत् (*wild == '*')
    {
        wild++;
    }

    प्रत्यागम *wild == 0;
}

स्थिर शून्य glob_rec(ध्रुव cv::String& directory, ध्रुव cv::String& wildchart, std::vector<cv::String>& परिणाम,
        तार्किक recursive, तार्किक includeDirectories, ध्रुव cv::String& मार्गः_उपसर्गः)
{
    DIR *dir;

    यदि ((dir = opendir (directory.c_str())) != 0)
    {
        /* find all the files and directories within directory */
        यत्न
        {
            संरचना dirent *ent;
            यावत् ((ent = readdir (dir)) != 0)
            {
                ध्रुव वर्ण* name = ent->d_name;
                यदि((name[0] == 0) || (name[0] == '.' && name[1] == 0) || (name[0] == '.' && name[1] == '.' && name[2] == 0))
                    अनुवर्त;

                cv::String मार्गः = cv::utils::fs::join(directory, name);
                cv::String प्रविष्टिः = cv::utils::fs::join(मार्गः_उपसर्गः, name);

                यदि (isDir(मार्गः, dir))
                {
                    यदि (recursive)
                        glob_rec(मार्गः, wildchart, परिणाम, recursive, includeDirectories, प्रविष्टिः);
                    यदि (!includeDirectories)
                        अनुवर्त;
                }

                यदि (wildchart.empty() || wildcmp(name, wildchart.c_str()))
                    परिणाम.push_back(प्रविष्टिः);
            }
        }
        गृहाण (...)
        {
            closedir(dir);
            क्षिप;
        }
        closedir(dir);
    }
    अन्यथा
    {
        CV_Error_(cv::Error::Sts_Object_Not_प्राप्तम्, ("could not open directory: %s", directory.c_str()));
    }
}
#यदि_अन्तः // OPENCV_HAVE_FILESYSTEM_SUPPORT

शून्य cv::glob(String आदर्शः, std::vector<String>& परिणाम, तार्किक recursive)
{
#यदि OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_INSTRUMENT_REGION();

    परिणाम.clear();
    String मार्गः, wildchart;

    यदि (isDir(आदर्शः, 0))
    {
        यदि(strchr(dir_separators, आदर्शः[आदर्शः.size() - 1]) != 0)
        {
            मार्गः = आदर्शः.substr(0, आदर्शः.size() - 1);
        }
        अन्यथा
        {
            मार्गः = आदर्शः;
        }
    }
    अन्यथा
    {
        size_t pos = आदर्शः.find_last_of(dir_separators);
        यदि (pos == String::npos)
        {
            wildchart = आदर्शः;
            मार्गः = ".";
        }
        अन्यथा
        {
            मार्गः = आदर्शः.substr(0, pos);
            wildchart = आदर्शः.substr(pos + 1);
        }
    }

    glob_rec(मार्गः, wildchart, परिणाम, recursive, असत्य, मार्गः);
    std::sort(परिणाम.begin(), परिणाम.end());
#अन्यथा // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_अप्रयुक्तम्(आदर्शः);
    CV_अप्रयुक्तम्(परिणाम);
    CV_अप्रयुक्तम्(recursive);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#यदि_अन्तः // OPENCV_HAVE_FILESYSTEM_SUPPORT
}

शून्य cv::utils::fs::glob(ध्रुव cv::String& directory, ध्रुव cv::String& आदर्शः,
        std::vector<cv::String>& परिणाम,
        तार्किक recursive, तार्किक includeDirectories)
{
#यदि OPENCV_HAVE_FILESYSTEM_SUPPORT
    glob_rec(directory, आदर्शः, परिणाम, recursive, includeDirectories, directory);
    std::sort(परिणाम.begin(), परिणाम.end());
#अन्यथा // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_अप्रयुक्तम्(directory);
    CV_अप्रयुक्तम्(आदर्शः);
    CV_अप्रयुक्तम्(परिणाम);
    CV_अप्रयुक्तम्(recursive);
    CV_अप्रयुक्तम्(includeDirectories);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#यदि_अन्तः // OPENCV_HAVE_FILESYSTEM_SUPPORT
}

शून्य cv::utils::fs::glob_relative(ध्रुव cv::String& directory, ध्रुव cv::String& आदर्शः,
        std::vector<cv::String>& परिणाम,
        तार्किक recursive, तार्किक includeDirectories)
{
#यदि OPENCV_HAVE_FILESYSTEM_SUPPORT
    glob_rec(directory, आदर्शः, परिणाम, recursive, includeDirectories, cv::String());
    std::sort(परिणाम.begin(), परिणाम.end());
#अन्यथा // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_अप्रयुक्तम्(directory);
    CV_अप्रयुक्तम्(आदर्शः);
    CV_अप्रयुक्तम्(परिणाम);
    CV_अप्रयुक्तम्(recursive);
    CV_अप्रयुक्तम्(includeDirectories);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#यदि_अन्तः // OPENCV_HAVE_FILESYSTEM_SUPPORT
}
