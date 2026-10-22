// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#समावेश "../precomp.hpp"

#समावेश <vector>

#समावेश <opencv2/core/utils/logger.defines.hpp>
#अघोषित CV_LOG_STRIP_LEVEL
#घोषणा CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_VERBOSE + 1
#समावेश "opencv2/core/utils/logger.hpp"
#समावेश "opencv2/core/utils/filesystem.hpp"
#समावेश "opencv2/core/utils/filesystem.private.hpp"

namespace cv { namespace samples {

अटल cv::Ptr< std::vector<cv::String> > g_तथ्य_खोज_path;
अटल cv::Ptr< std::vector<cv::String> > g_तथ्य_खोज_subdir;

अटल std::vector<cv::String>& get_तथ्य_खोज_Path()
{
    अगर (g_तथ्य_खोज_path.empty())
        g_तथ्य_खोज_path.reset(नया std::vector<cv::String>());
    वापस *(g_तथ्य_खोज_path.लो());
}

अटल std::vector<cv::String>& get_तथ्य_खोज_Sub_Directory()
{
    अगर (g_तथ्य_खोज_subdir.empty())
    {
        g_तथ्य_खोज_subdir.reset(नया std::vector<cv::String>());
        g_तथ्य_खोज_subdir->push_back("samples/data");
        g_तथ्य_खोज_subdir->push_back("data");
        g_तथ्य_खोज_subdir->push_back("");
    }
    वापस *(g_तथ्य_खोज_subdir.लो());
}


CV_EXPORTS व्योम add_Samples_तथ्य_खोज_Path(स्थिर cv::String& path)
{
    अगर (utils::fs::isDirectory(path))
        get_तथ्य_खोज_Path().push_back(path);
}
CV_EXPORTS व्योम add_Samples_तथ्य_खोज_Sub_Directory(स्थिर cv::String& subdir)
{
    get_तथ्य_खोज_Sub_Directory().push_back(subdir);
}

cv::String findFile(स्थिर cv::String& relative_path, bool required, bool silentMode)
{
#अगर OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_LOG_DEBUG(शून्य, cv::format("cv::samples::findFile('%s', %s)", relative_path.c_str(), required ? "true" : "false"));
    cv::String परिणाम = cv::utils::find_तथ्य_File(relative_path,
                                                "OPENCV_SAMPLES_DATA_PATH",
                                                &get_तथ्य_खोज_Path(),
                                                &get_तथ्य_खोज_Sub_Directory());
    अगर (परिणाम != relative_path && !silentMode)
    {
        CV_LOG_WARNING(शून्य, "cv::samples::findFile('" << relative_path << "') => '" << परिणाम << "'");
    }
    अगर (परिणाम.empty() && required)
        CV_Error(cv::Error::StsError, cv::format("OpenCV samples: Can't find required data file: %s", relative_path.c_str()));
    वापस परिणाम;
#अन्यथा
    CV_UNUSED(relative_path);
    CV_UNUSED(required);
    CV_UNUSED(silentMode);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#पूर्ण_अगर
}


}} // namespace
