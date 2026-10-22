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

#समावेश "precomp.hpp"

#समावेश "opencv2/core/utils/filesystem.hpp"
#समावेश "opencv2/core/utils/filesystem.private.hpp"

#अगर OPENCV_HAVE_FILESYSTEM_SUPPORT
#अगर defined _WIN32 || defined WINCE
# include <windows.h>
स्थिर अक्षर dir_separators[] = "/\\";

namespace
{
    काष्ठा dirent
    {
        स्थिर अक्षर* d_name;
    };

    काष्ठा सूची
    {
#अगर defined(WINRT) || defined(_WIN32_WCE)
        WIN32_FIND_DATAW तथ्य;
#अन्यथा
        WIN32_FIND_DATAA तथ्य;
#पूर्ण_अगर
        HANDLE handle;
        dirent ent;
#अगर defined(WINRT) || defined(_WIN32_WCE)
        सूची() { }
        ~सूची()
        {
            अगर (ent.d_name)
                मिटाओ[] ent.d_name;
        }
#पूर्ण_अगर
    };

    सूची* सूची_खोलो(स्थिर अक्षर* path)
    {
        सूची* dir = नया सूची;
        dir->ent.d_name = 0;
#अगर defined(WINRT) || defined(_WIN32_WCE)
        cv::String full_path = cv::String(path) + "\\*";
        ब_अक्षर_प्रकार wfull_path[MAX_PATH];
        माप_प्रकार copied = mbstowcs(wfull_path, full_path.c_str(), MAX_PATH);
        CV_Assert((copied != MAX_PATH) && (copied != (माप_प्रकार)-1));
        dir->handle = ::FindFirstFileExW(wfull_path, FindExInfoStandard,
                        &dir->तथ्य, Find_Ex_खोज_Name_Match, शून्य, 0);
#अन्यथा
        dir->handle = ::FindFirstFileExA((cv::String(path) + "\\*").c_str(),
            FindExInfoStandard, &dir->तथ्य, Find_Ex_खोज_Name_Match, शून्य, 0);
#पूर्ण_अगर
        अगर(dir->handle == INVALID_HANDLE_मूल्य)
        {
            /*closedir will do all cleanup*/
            मिटाओ dir;
            वापस 0;
        }
        वापस dir;
    }

    dirent* सूची_पढ़ो(सूची* dir)
    {
#अगर defined(WINRT) || defined(_WIN32_WCE)
        अगर (dir->ent.d_name != 0)
        {
            अगर (::Find_अग्र_File_W(dir->handle, &dir->तथ्य) != TRUE)
                वापस 0;
        }
        माप_प्रकार asize = wcstombs(शून्य, dir->तथ्य.cFileName, 0);
        CV_Assert((asize != 0) && (asize != (माप_प्रकार)-1));
        अक्षर* aname = नया अक्षर[asize+1];
        aname[asize] = 0;
        wcstombs(aname, dir->तथ्य.cFileName, asize);
        मिटाओ[] dir->ent.d_name;
        dir->ent.d_name = aname;
#अन्यथा
        अगर (dir->ent.d_name != 0)
        {
            अगर (::Find_अग्र_File_A(dir->handle, &dir->तथ्य) != TRUE)
                वापस 0;
        }
        dir->ent.d_name = dir->तथ्य.cFileName;
#पूर्ण_अगर
        वापस &dir->ent;
    }

    व्योम बंद_सूची(सूची* dir)
    {
        ::FindClose(dir->handle);
        मिटाओ dir;
    }


}
#अन्यथा // defined _WIN32 || defined WINCE
# include <dirent.h>
# include <sys/stat.h>
स्थिर अक्षर dir_separators[] = "/";
#पूर्ण_अगर // defined _WIN32 || defined WINCE
#पूर्ण_अगर // OPENCV_HAVE_FILESYSTEM_SUPPORT


#अगर OPENCV_HAVE_FILESYSTEM_SUPPORT
अटल bool isDir(स्थिर cv::String& path, सूची* dir)
{
#अगर defined _WIN32 || defined _WIN32_WCE
    DWORD attributes;
    BOOL status = TRUE;
    अगर (dir)
        attributes = dir->तथ्य.dwFileAttributes;
    अन्यथा
    {
        WIN32_FILE_ATTRIBUTE_तथ्य all_attrs;
#अगर defined WINRT || defined _WIN32_WCE
        ब_अक्षर_प्रकार wpath[MAX_PATH];
        माप_प्रकार copied = mbstowcs(wpath, path.c_str(), MAX_PATH);
        CV_Assert((copied != MAX_PATH) && (copied != (माप_प्रकार)-1));
        status = ::GetFileAttributesExW(wpath, GetFileExInfoStandard, &all_attrs);
#अन्यथा
        status = ::GetFileAttributesExA(path.c_str(), GetFileExInfoStandard, &all_attrs);
#पूर्ण_अगर
        attributes = all_attrs.dwFileAttributes;
    }

    वापस status && ((attributes & FILE_ATTRIBUTE_DIRECTORY) != 0);
#अन्यथा
    CV_UNUSED(dir);
    काष्ठा stat stat_buf;
    अगर (0 != stat( path.c_str(), &stat_buf))
        वापस false;
    पूर्णांक is_dir = S_ISDIR( stat_buf.st_mode);
    वापस is_dir != 0;
#पूर्ण_अगर
}
#पूर्ण_अगर // OPENCV_HAVE_FILESYSTEM_SUPPORT

bool cv::utils::fs::isDirectory(स्थिर cv::String& path)
{
#अगर OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_INSTRUMENT_REGION();
    वापस isDir(path, शून्य);
#अन्यथा
    CV_UNUSED(path);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#पूर्ण_अगर
}

#अगर OPENCV_HAVE_FILESYSTEM_SUPPORT
अटल bool wildcmp(स्थिर अक्षर *string, स्थिर अक्षर *wild)
{
    // Based on wildcmp written by Jack Handy - <A href="mailto:jakkhandy@hotmail.com">jakkhandy@hotmail.com</A>
    स्थिर अक्षर *cp = 0, *mp = 0;

    जबतक ((*string) && (*wild != '*'))
    {
        अगर ((*wild != *string) && (*wild != '?'))
        {
            वापस false;
        }

        wild++;
        string++;
    }

    जबतक (*string)
    {
        अगर (*wild == '*')
        {
            अगर (!*++wild)
            {
                वापस true;
            }

            mp = wild;
            cp = string + 1;
        }
        अन्यथा अगर ((*wild == *string) || (*wild == '?'))
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

    जबतक (*wild == '*')
    {
        wild++;
    }

    वापस *wild == 0;
}

अटल व्योम glob_rec(स्थिर cv::String& directory, स्थिर cv::String& wildchart, std::vector<cv::String>& परिणाम,
        bool recursive, bool includeDirectories, स्थिर cv::String& pathPrefix)
{
    सूची *dir;

    अगर ((dir = सूची_खोलो (directory.c_str())) != 0)
    {
        /* find all the files and directories within directory */
        प्रयास
        {
            काष्ठा dirent *ent;
            जबतक ((ent = सूची_पढ़ो (dir)) != 0)
            {
                स्थिर अक्षर* name = ent->d_name;
                अगर((name[0] == 0) || (name[0] == '.' && name[1] == 0) || (name[0] == '.' && name[1] == '.' && name[2] == 0))
                    जारी;

                cv::String path = cv::utils::fs::join(directory, name);
                cv::String entry = cv::utils::fs::join(pathPrefix, name);

                अगर (isDir(path, dir))
                {
                    अगर (recursive)
                        glob_rec(path, wildchart, परिणाम, recursive, includeDirectories, entry);
                    अगर (!includeDirectories)
                        जारी;
                }

                अगर (wildchart.empty() || wildcmp(name, wildchart.c_str()))
                    परिणाम.push_back(entry);
            }
        }
        पकड़ो (...)
        {
            बंद_सूची(dir);
            फेंको;
        }
        बंद_सूची(dir);
    }
    अन्यथा
    {
        CV_Error_(cv::Error::StsObjectNotFound, ("could not open directory: %s", directory.c_str()));
    }
}
#पूर्ण_अगर // OPENCV_HAVE_FILESYSTEM_SUPPORT

व्योम cv::glob(String pattern, std::vector<String>& परिणाम, bool recursive)
{
#अगर OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_INSTRUMENT_REGION();

    परिणाम.सफा();
    String path, wildchart;

    अगर (isDir(pattern, 0))
    {
        अगर(म_अक्षर(dir_separators, pattern[pattern.size() - 1]) != 0)
        {
            path = pattern.substr(0, pattern.size() - 1);
        }
        अन्यथा
        {
            path = pattern;
        }
    }
    अन्यथा
    {
        माप_प्रकार pos = pattern.find_last_of(dir_separators);
        अगर (pos == String::npos)
        {
            wildchart = pattern;
            path = ".";
        }
        अन्यथा
        {
            path = pattern.substr(0, pos);
            wildchart = pattern.substr(pos + 1);
        }
    }

    glob_rec(path, wildchart, परिणाम, recursive, false, path);
    std::sort(परिणाम.begin(), परिणाम.end());
#अन्यथा // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_UNUSED(pattern);
    CV_UNUSED(परिणाम);
    CV_UNUSED(recursive);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#पूर्ण_अगर // OPENCV_HAVE_FILESYSTEM_SUPPORT
}

व्योम cv::utils::fs::glob(स्थिर cv::String& directory, स्थिर cv::String& pattern,
        std::vector<cv::String>& परिणाम,
        bool recursive, bool includeDirectories)
{
#अगर OPENCV_HAVE_FILESYSTEM_SUPPORT
    glob_rec(directory, pattern, परिणाम, recursive, includeDirectories, directory);
    std::sort(परिणाम.begin(), परिणाम.end());
#अन्यथा // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_UNUSED(directory);
    CV_UNUSED(pattern);
    CV_UNUSED(परिणाम);
    CV_UNUSED(recursive);
    CV_UNUSED(includeDirectories);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#पूर्ण_अगर // OPENCV_HAVE_FILESYSTEM_SUPPORT
}

व्योम cv::utils::fs::glob_relative(स्थिर cv::String& directory, स्थिर cv::String& pattern,
        std::vector<cv::String>& परिणाम,
        bool recursive, bool includeDirectories)
{
#अगर OPENCV_HAVE_FILESYSTEM_SUPPORT
    glob_rec(directory, pattern, परिणाम, recursive, includeDirectories, cv::String());
    std::sort(परिणाम.begin(), परिणाम.end());
#अन्यथा // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_UNUSED(directory);
    CV_UNUSED(pattern);
    CV_UNUSED(परिणाम);
    CV_UNUSED(recursive);
    CV_UNUSED(includeDirectories);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#पूर्ण_अगर // OPENCV_HAVE_FILESYSTEM_SUPPORT
}
