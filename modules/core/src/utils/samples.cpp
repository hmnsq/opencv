// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#অন্তর্ভাব "../precomp.hpp"

#অন্তর্ভাব <vector>

#অন্তর্ভাব <opencv2/core/utils/logger.defines.hpp>
#undef CV_LOG_STRIP_LEVEL
#define CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_VERBOSE + 1
#অন্তর্ভাব "opencv2/core/utils/logger.hpp"
#অন্তর্ভাব "opencv2/core/utils/filesystem.hpp"
#অন্তর্ভাব "opencv2/core/utils/filesystem.private.hpp"

নামাকাশ cv { নামাকাশ samples {

স্থির cv::Ptr< std::vector<cv::String> > g_data_অনুসন্ধান_path;
স্থির cv::Ptr< std::vector<cv::String> > g_data_অনুসন্ধান_subdir;

স্থির std::vector<cv::String>& get_Data_অনুসন্ধান_Path()
{
    যদি (g_data_অনুসন্ধান_path.empty())
        g_data_অনুসন্ধান_path.reset(নূতন std::vector<cv::String>());
    প্রত্যাগম *(g_data_অনুসন্ধান_path.get());
}

স্থির std::vector<cv::String>& get_Data_অনুসন্ধান_Sub_Directory()
{
    যদি (g_data_অনুসন্ধান_subdir.empty())
    {
        g_data_অনুসন্ধান_subdir.reset(নূতন std::vector<cv::String>());
        g_data_অনুসন্ধান_subdir->push_back("samples/data");
        g_data_অনুসন্ধান_subdir->push_back("data");
        g_data_অনুসন্ধান_subdir->push_back("");
    }
    প্রত্যাগম *(g_data_অনুসন্ধান_subdir.get());
}


CV_EXPORTS শূন্য add_Samples_Data_অনুসন্ধান_Path(ধ্রুব cv::String& path)
{
    যদি (utils::fs::isDirectory(path))
        get_Data_অনুসন্ধান_Path().push_back(path);
}
CV_EXPORTS শূন্য add_Samples_Data_অনুসন্ধান_Sub_Directory(ধ্রুব cv::String& subdir)
{
    get_Data_অনুসন্ধান_Sub_Directory().push_back(subdir);
}

cv::String findFile(ধ্রুব cv::String& relative_path, তার্কিক required, তার্কিক silentMode)
{
#যদি OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_LOG_DEBUG(রিক্ত, cv::format("cv::samples::findFile('%s', %s)", relative_path.c_str(), required ? "true" : "false"));
    cv::String result = cv::utils::findDataFile(relative_path,
                                                "OPENCV_SAMPLES_DATA_PATH",
                                                &get_Data_অনুসন্ধান_Path(),
                                                &get_Data_অনুসন্ধান_Sub_Directory());
    যদি (result != relative_path && !silentMode)
    {
        CV_LOG_WARNING(রিক্ত, "cv::samples::findFile('" << relative_path << "') => '" << result << "'");
    }
    যদি (result.empty() && required)
        CV_Error(cv::Error::StsError, cv::format("OpenCV samples: Can't find required data file: %s", relative_path.c_str()));
    প্রত্যাগম result;
#অন্যথা
    CV_UNUSED(relative_path);
    CV_UNUSED(required);
    CV_UNUSED(silentMode);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#endif
}


}} // namespace
