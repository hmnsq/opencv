// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#અન્તર્ભાવ "../precomp.hpp"

#અન્તર્ભાવ <vector>

#અન્તર્ભાવ <opencv2/core/utils/logger.defines.hpp>
#undef CV_LOG_STRIP_LEVEL
#define CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_VERBOSE + 1
#અન્તર્ભાવ "opencv2/core/utils/logger.hpp"
#અન્તર્ભાવ "opencv2/core/utils/filesystem.hpp"
#અન્તર્ભાવ "opencv2/core/utils/filesystem.private.hpp"

નામાકાશ cv { નામાકાશ samples {

સ્થિર cv::Ptr< std::vector<cv::String> > g_data_શોધ_path;
સ્થિર cv::Ptr< std::vector<cv::String> > g_data_શોધ_subdir;

સ્થિર std::vector<cv::String>& get_Data_શોધ_Path()
{
    જો (g_data_શોધ_path.empty())
        g_data_શોધ_path.reset(નવું std::vector<cv::String>());
    પાછા *(g_data_શોધ_path.get());
}

સ્થિર std::vector<cv::String>& get_Data_શોધ_Sub_Directory()
{
    જો (g_data_શોધ_subdir.empty())
    {
        g_data_શોધ_subdir.reset(નવું std::vector<cv::String>());
        g_data_શોધ_subdir->push_back("samples/data");
        g_data_શોધ_subdir->push_back("data");
        g_data_શોધ_subdir->push_back("");
    }
    પાછા *(g_data_શોધ_subdir.get());
}


CV_EXPORTS શૂન્ય add_Samples_Data_શોધ_Path(અચલ cv::String& path)
{
    જો (utils::fs::isDirectory(path))
        get_Data_શોધ_Path().push_back(path);
}
CV_EXPORTS શૂન્ય add_Samples_Data_શોધ_Sub_Directory(અચલ cv::String& subdir)
{
    get_Data_શોધ_Sub_Directory().push_back(subdir);
}

cv::String findFile(અચલ cv::String& relative_path, તાર્કિક required, તાર્કિક silentMode)
{
#જો OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_LOG_DEBUG(ખાલી, cv::format("cv::samples::findFile('%s', %s)", relative_path.c_str(), required ? "true" : "false"));
    cv::String result = cv::utils::findDataFile(relative_path,
                                                "OPENCV_SAMPLES_DATA_PATH",
                                                &get_Data_શોધ_Path(),
                                                &get_Data_શોધ_Sub_Directory());
    જો (result != relative_path && !silentMode)
    {
        CV_LOG_WARNING(ખાલી, "cv::samples::findFile('" << relative_path << "') => '" << result << "'");
    }
    જો (result.empty() && required)
        CV_Error(cv::Error::StsError, cv::format("OpenCV samples: Can't find required data file: %s", relative_path.c_str()));
    પાછા result;
#નહીંતર
    CV_UNUSED(relative_path);
    CV_UNUSED(required);
    CV_UNUSED(silentMode);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#endif
}


}} // namespace
