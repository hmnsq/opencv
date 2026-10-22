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

স্থির cv::Ptr< std::vector<cv::String> > g_তথ্য_অনুসন্ধান_path;
স্থির cv::Ptr< std::vector<cv::String> > g_তথ্য_অনুসন্ধান_subdir;

স্থির std::vector<cv::String>& get_তথ্য_অনুসন্ধান_Path()
{
    যদি (g_তথ্য_অনুসন্ধান_path.empty())
        g_তথ্য_অনুসন্ধান_path.reset(নতুন std::vector<cv::String>());
    ফেরত *(g_তথ্য_অনুসন্ধান_path.get());
}

স্থির std::vector<cv::String>& get_তথ্য_অনুসন্ধান_Sub_Directory()
{
    যদি (g_তথ্য_অনুসন্ধান_subdir.empty())
    {
        g_তথ্য_অনুসন্ধান_subdir.reset(নতুন std::vector<cv::String>());
        g_তথ্য_অনুসন্ধান_subdir->push_back("samples/data");
        g_তথ্য_অনুসন্ধান_subdir->push_back("data");
        g_তথ্য_অনুসন্ধান_subdir->push_back("");
    }
    ফেরত *(g_তথ্য_অনুসন্ধান_subdir.get());
}


CV_EXPORTS শূন্য add_Samples_তথ্য_অনুসন্ধান_Path(ধ্রুবক cv::String& path)
{
    যদি (utils::fs::isDirectory(path))
        get_তথ্য_অনুসন্ধান_Path().push_back(path);
}
CV_EXPORTS শূন্য add_Samples_তথ্য_অনুসন্ধান_Sub_Directory(ধ্রুবক cv::String& subdir)
{
    get_তথ্য_অনুসন্ধান_Sub_Directory().push_back(subdir);
}

cv::String findFile(ধ্রুবক cv::String& relative_path, যৌক্তিক required, যৌক্তিক silentMode)
{
#যদি OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_LOG_DEBUG(ফাঁকা, cv::format("cv::samples::findFile('%s', %s)", relative_path.c_str(), required ? "true" : "false"));
    cv::String ফলাফল = cv::utils::find_তথ্য_File(relative_path,
                                                "OPENCV_SAMPLES_DATA_PATH",
                                                &get_তথ্য_অনুসন্ধান_Path(),
                                                &get_তথ্য_অনুসন্ধান_Sub_Directory());
    যদি (ফলাফল != relative_path && !silentMode)
    {
        CV_LOG_WARNING(ফাঁকা, "cv::samples::findFile('" << relative_path << "') => '" << ফলাফল << "'");
    }
    যদি (ফলাফল.empty() && required)
        CV_Error(cv::Error::StsError, cv::format("OpenCV samples: Can't find required data file: %s", relative_path.c_str()));
    ফেরত ফলাফল;
#অন্যথা
    CV_UNUSED(relative_path);
    CV_UNUSED(required);
    CV_UNUSED(silentMode);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#endif
}


}} // namespace
