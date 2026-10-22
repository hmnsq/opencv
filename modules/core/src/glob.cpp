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
        WIN32_FIND_DATAW data;
#अन्यथा
        WIN32_FIND_DATAA data;
#endif
        HANDLE handle;
        dirent ent;
#यदि defined(WINRT) || defined(_WIN32_WCE)
        DIR() { }
        ~DIR()
        {
            यदि (ent.d_name)
                delete[] ent.d_name;
        }
#endif
    };

    DIR* opendir(ध्रुव वर्ण* path)
    {
        DIR* dir = नूतन DIR;
        dir->ent.d_name = 0;
#यदि defined(WINRT) || defined(_WIN32_WCE)
        cv::String full_path = cv::String(path) + "\\*";
        wchar_t wfull_path[MAX_PATH];
        size_t copied = mbstowcs(wfull_path, full_path.c_str(), MAX_PATH);
        CV_Assert((copied != MAX_PATH) && (copied != (size_t)-1));
        dir->handle = ::FindFirstFileExW(wfull_path, FindExInfoStandard,
                        &dir->data, Find_Ex_खोज_Name_Match, रिक्त, 0);
#अन्यथा
        dir->handle = ::FindFirstFileExA((cv::String(path) + "\\*").c_str(),
            FindExInfoStandard, &dir->data, Find_Ex_खोज_Name_Match, रिक्त, 0);
#endif
        यदि(dir->handle == INVALID_HANDLE_मूल्य)
        {
            /*closedir will do all cleanup*/
            delete dir;
            प्रत्यागम 0;
        }
        प्रत्यागम dir;
    }

    dirent* readdir(DIR* dir)
    {
#यदि defined(WINRT) || defined(_WIN32_WCE)
        यदि (dir->ent.d_name != 0)
        {
            यदि (::FindNextFileW(dir->handle, &dir->data) != TRUE)
                प्रत्यागम 0;
        }
        size_t asize = wcstombs(रिक्त, dir->data.cFileName, 0);
        CV_Assert((asize != 0) && (asize != (size_t)-1));
        वर्ण* aname = नूतन वर्ण[asize+1];
        aname[asize] = 0;
        wcstombs(aname, dir->data.cFileName, asize);
        delete[] dir->ent.d_name;
        dir->ent.d_name = aname;
#अन्यथा
        यदि (dir->ent.d_name != 0)
        {
            यदि (::FindNextFileA(dir->handle, &dir->data) != TRUE)
                प्रत्यागम 0;
        }
        dir->ent.d_name = dir->data.cFileName;
#endif
        प्रत्यागम &dir->ent;
    }

    शून्य closedir(DIR* dir)
    {
        ::FindClose(dir->handle);
        delete dir;
    }


}
#अन्यथा // defined _WIN32 || defined WINCE
# include <dirent.h>
# include <sys/stat.h>
ध्रुव वर्ण dir_separators[] = "/";
#endif // defined _WIN32 || defined WINCE
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT


#यदि OPENCV_HAVE_FILESYSTEM_SUPPORT
स्थिर तार्किक isDir(ध्रुव cv::String& path, DIR* dir)
{
#यदि defined _WIN32 || defined _WIN32_WCE
    DWORD attributes;
    BOOL status = TRUE;
    यदि (dir)
        attributes = dir->data.dwFileAttributes;
    अन्यथा
    {
        WIN32_FILE_ATTRIBUTE_DATA all_attrs;
#यदि defined WINRT || defined _WIN32_WCE
        wchar_t wpath[MAX_PATH];
        size_t copied = mbstowcs(wpath, path.c_str(), MAX_PATH);
        CV_Assert((copied != MAX_PATH) && (copied != (size_t)-1));
        status = ::GetFileAttributesExW(wpath, GetFileExInfoStandard, &all_attrs);
#अन्यथा
        status = ::GetFileAttributesExA(path.c_str(), GetFileExInfoStandard, &all_attrs);
#endif
        attributes = all_attrs.dwFileAttributes;
    }

    प्रत्यागम status && ((attributes & FILE_ATTRIBUTE_DIRECTORY) != 0);
#अन्यथा
    CV_UNUSED(dir);
    संरचना stat stat_buf;
    यदि (0 != stat( path.c_str(), &stat_buf))
        प्रत्यागम असत्य;
    पूर्णाङ्क is_dir = S_ISDIR( stat_buf.st_mode);
    प्रत्यागम is_dir != 0;
#endif
}
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT

तार्किक cv::utils::fs::isDirectory(ध्रुव cv::String& path)
{
#यदि OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_INSTRUMENT_REGION();
    प्रत्यागम isDir(path, रिक्त);
#अन्यथा
    CV_UNUSED(path);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#endif
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

स्थिर शून्य glob_rec(ध्रुव cv::String& directory, ध्रुव cv::String& wildchart, std::vector<cv::String>& result,
        तार्किक recursive, तार्किक includeDirectories, ध्रुव cv::String& pathPrefix)
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

                cv::String path = cv::utils::fs::join(directory, name);
                cv::String entry = cv::utils::fs::join(pathPrefix, name);

                यदि (isDir(path, dir))
                {
                    यदि (recursive)
                        glob_rec(path, wildchart, result, recursive, includeDirectories, entry);
                    यदि (!includeDirectories)
                        अनुवर्त;
                }

                यदि (wildchart.empty() || wildcmp(name, wildchart.c_str()))
                    result.push_back(entry);
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
        CV_Error_(cv::Error::StsObjectNotFound, ("could not open directory: %s", directory.c_str()));
    }
}
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT

शून्य cv::glob(String pattern, std::vector<String>& result, तार्किक recursive)
{
#यदि OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_INSTRUMENT_REGION();

    result.clear();
    String path, wildchart;

    यदि (isDir(pattern, 0))
    {
        यदि(strchr(dir_separators, pattern[pattern.size() - 1]) != 0)
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
        size_t pos = pattern.find_last_of(dir_separators);
        यदि (pos == String::npos)
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

    glob_rec(path, wildchart, result, recursive, असत्य, path);
    std::sort(result.begin(), result.end());
#अन्यथा // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_UNUSED(pattern);
    CV_UNUSED(result);
    CV_UNUSED(recursive);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT
}

शून्य cv::utils::fs::glob(ध्रुव cv::String& directory, ध्रुव cv::String& pattern,
        std::vector<cv::String>& result,
        तार्किक recursive, तार्किक includeDirectories)
{
#यदि OPENCV_HAVE_FILESYSTEM_SUPPORT
    glob_rec(directory, pattern, result, recursive, includeDirectories, directory);
    std::sort(result.begin(), result.end());
#अन्यथा // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_UNUSED(directory);
    CV_UNUSED(pattern);
    CV_UNUSED(result);
    CV_UNUSED(recursive);
    CV_UNUSED(includeDirectories);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT
}

शून्य cv::utils::fs::glob_relative(ध्रुव cv::String& directory, ध्रुव cv::String& pattern,
        std::vector<cv::String>& result,
        तार्किक recursive, तार्किक includeDirectories)
{
#यदि OPENCV_HAVE_FILESYSTEM_SUPPORT
    glob_rec(directory, pattern, result, recursive, includeDirectories, cv::String());
    std::sort(result.begin(), result.end());
#अन्यथा // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_UNUSED(directory);
    CV_UNUSED(pattern);
    CV_UNUSED(result);
    CV_UNUSED(recursive);
    CV_UNUSED(includeDirectories);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT
}
