// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#شامل "../precomp.hpp"

#شامل <vector>

#شامل <opencv2/core/utils/logger.defines.hpp>
#undef CV_LOG_STRIP_LEVEL
#define CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_VERBOSE + 1
#شامل "opencv2/core/utils/logger.hpp"
#شامل "opencv2/core/utils/filesystem.hpp"
#شامل "opencv2/core/utils/filesystem.private.hpp"

فضاینام cv { فضاینام samples {

ایستا cv::Ptr< std::vector<cv::String> > g_data_جستجو_path;
ایستا cv::Ptr< std::vector<cv::String> > g_data_جستجو_subdir;

ایستا std::vector<cv::String>& get_Data_جستجو_Path()
{
    اگر (g_data_جستجو_path.empty())
        g_data_جستجو_path.reset(جدید std::vector<cv::String>());
    بازگشت *(g_data_جستجو_path.get());
}

ایستا std::vector<cv::String>& get_Data_جستجو_Sub_Directory()
{
    اگر (g_data_جستجو_subdir.empty())
    {
        g_data_جستجو_subdir.reset(جدید std::vector<cv::String>());
        g_data_جستجو_subdir->push_back("samples/data");
        g_data_جستجو_subdir->push_back("data");
        g_data_جستجو_subdir->push_back("");
    }
    بازگشت *(g_data_جستجو_subdir.get());
}


CV_EXPORTS تهی add_Samples_Data_جستجو_Path(ثابت cv::String& path)
{
    اگر (utils::fs::isDirectory(path))
        get_Data_جستجو_Path().push_back(path);
}
CV_EXPORTS تهی add_Samples_Data_جستجو_Sub_Directory(ثابت cv::String& subdir)
{
    get_Data_جستجو_Sub_Directory().push_back(subdir);
}

cv::String findFile(ثابت cv::String& relative_path, منطقی required, منطقی silentMode)
{
#اگر OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_LOG_DEBUG(پوچ, cv::format("cv::samples::findFile('%s', %s)", relative_path.c_str(), required ? "true" : "false"));
    cv::String result = cv::utils::findDataFile(relative_path,
                                                "OPENCV_SAMPLES_DATA_PATH",
                                                &get_Data_جستجو_Path(),
                                                &get_Data_جستجو_Sub_Directory());
    اگر (result != relative_path && !silentMode)
    {
        CV_LOG_WARNING(پوچ, "cv::samples::findFile('" << relative_path << "') => '" << result << "'");
    }
    اگر (result.empty() && required)
        CV_Error(cv::Error::StsError, cv::format("OpenCV samples: Can't find required data file: %s", relative_path.c_str()));
    بازگشت result;
#وگرنه
    CV_UNUSED(relative_path);
    CV_UNUSED(required);
    CV_UNUSED(silentMode);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#endif
}


}} // namespace
