// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ଅନ୍ତର୍ଭାଵ "../precomp.hpp"

#ଅନ୍ତର୍ଭାଵ <vector>

#ଅନ୍ତର୍ଭାଵ <opencv2/core/utils/logger.defines.hpp>
#undef CV_LOG_STRIP_LEVEL
#define CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_VERBOSE + 1
#ଅନ୍ତର୍ଭାଵ "opencv2/core/utils/logger.hpp"
#ଅନ୍ତର୍ଭାଵ "opencv2/core/utils/filesystem.hpp"
#ଅନ୍ତର୍ଭାଵ "opencv2/core/utils/filesystem.private.hpp"

ନାମାକାଶ cv { ନାମାକାଶ samples {

ସ୍ଥିର cv::Ptr< std::vector<cv::String> > g_data_ସନ୍ଧାନ_path;
ସ୍ଥିର cv::Ptr< std::vector<cv::String> > g_data_ସନ୍ଧାନ_subdir;

ସ୍ଥିର std::vector<cv::String>& get_Data_ସନ୍ଧାନ_Path()
{
    ଯଦି (g_data_ସନ୍ଧାନ_path.empty())
        g_data_ସନ୍ଧାନ_path.reset(ନୂତନ std::vector<cv::String>());
    ଫେରନ୍ତୁ *(g_data_ସନ୍ଧାନ_path.get());
}

ସ୍ଥିର std::vector<cv::String>& get_Data_ସନ୍ଧାନ_Sub_Directory()
{
    ଯଦି (g_data_ସନ୍ଧାନ_subdir.empty())
    {
        g_data_ସନ୍ଧାନ_subdir.reset(ନୂତନ std::vector<cv::String>());
        g_data_ସନ୍ଧାନ_subdir->push_back("samples/data");
        g_data_ସନ୍ଧାନ_subdir->push_back("data");
        g_data_ସନ୍ଧାନ_subdir->push_back("");
    }
    ଫେରନ୍ତୁ *(g_data_ସନ୍ଧାନ_subdir.get());
}


CV_EXPORTS ଶୂନ୍ୟ add_Samples_Data_ସନ୍ଧାନ_Path(ଅଚଳ cv::String& path)
{
    ଯଦି (utils::fs::isDirectory(path))
        get_Data_ସନ୍ଧାନ_Path().push_back(path);
}
CV_EXPORTS ଶୂନ୍ୟ add_Samples_Data_ସନ୍ଧାନ_Sub_Directory(ଅଚଳ cv::String& subdir)
{
    get_Data_ସନ୍ଧାନ_Sub_Directory().push_back(subdir);
}

cv::String findFile(ଅଚଳ cv::String& relative_path, ତାର୍କିକ required, ତାର୍କିକ silentMode)
{
#ଯଦି OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_LOG_DEBUG(ଖାଲି, cv::format("cv::samples::findFile('%s', %s)", relative_path.c_str(), required ? "true" : "false"));
    cv::String result = cv::utils::findDataFile(relative_path,
                                                "OPENCV_SAMPLES_DATA_PATH",
                                                &get_Data_ସନ୍ଧାନ_Path(),
                                                &get_Data_ସନ୍ଧାନ_Sub_Directory());
    ଯଦି (result != relative_path && !silentMode)
    {
        CV_LOG_WARNING(ଖାଲି, "cv::samples::findFile('" << relative_path << "') => '" << result << "'");
    }
    ଯଦି (result.empty() && required)
        CV_Error(cv::Error::StsError, cv::format("OpenCV samples: Can't find required data file: %s", relative_path.c_str()));
    ଫେରନ୍ତୁ result;
#ଅନ୍ୟଥା
    CV_UNUSED(relative_path);
    CV_UNUSED(required);
    CV_UNUSED(silentMode);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#endif
}


}} // namespace
