// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ضمن "../precomp.hpp"

#ضمن <vector>

#ضمن <opencv2/core/utils/logger.defines.hpp>
#undef CV_LOG_STRIP_LEVEL
#define CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_VERBOSE + 1
#ضمن "opencv2/core/utils/logger.hpp"
#ضمن "opencv2/core/utils/filesystem.hpp"
#ضمن "opencv2/core/utils/filesystem.private.hpp"

نطاق cv { نطاق samples {

ساكن cv::Ptr< std::vector<cv::String> > g_data_search_path;
ساكن cv::Ptr< std::vector<cv::String> > g_data_search_subdir;

ساكن std::vector<cv::String>& _getDataSearchPath()
{
    𐤀𐤌 (g_data_search_path.empty())
        g_data_search_path.reset(جديد std::vector<cv::String>());
    أرجع *(g_data_search_path.get());
}

ساكن std::vector<cv::String>& _getDataSearchSubDirectory()
{
    𐤀𐤌 (g_data_search_subdir.empty())
    {
        g_data_search_subdir.reset(جديد std::vector<cv::String>());
        g_data_search_subdir->push_back("samples/data");
        g_data_search_subdir->push_back("data");
        g_data_search_subdir->push_back("");
    }
    أرجع *(g_data_search_subdir.get());
}


CV_EXPORTS فراغ addSamplesDataSearchPath(ثابت cv::String& path)
{
    𐤀𐤌 (utils::fs::isDirectory(path))
        _getDataSearchPath().push_back(path);
}
CV_EXPORTS فراغ addSamplesDataSearchSubDirectory(ثابت cv::String& subdir)
{
    _getDataSearchSubDirectory().push_back(subdir);
}

cv::String findFile(ثابت cv::String& relative_path, منطقي required, منطقي silentMode)
{
#𐤀𐤌 OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_LOG_DEBUG(عدم, cv::format("cv::samples::findFile('%s', %s)", relative_path.c_str(), required ? "true" : "false"));
    cv::String result = cv::utils::findDataFile(relative_path,
                                                "OPENCV_SAMPLES_DATA_PATH",
                                                &_getDataSearchPath(),
                                                &_getDataSearchSubDirectory());
    𐤀𐤌 (result != relative_path && !silentMode)
    {
        CV_LOG_WARNING(عدم, "cv::samples::findFile('" << relative_path << "') => '" << result << "'");
    }
    𐤀𐤌 (result.empty() && required)
        CV_Error(cv::Error::StsError, cv::format("OpenCV samples: Can't find required data file: %s", relative_path.c_str()));
    أرجع result;
#وإلا
    CV_UNUSED(relative_path);
    CV_UNUSED(required);
    CV_UNUSED(silentMode);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#endif
}


}} // namespace
