// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#அந்தர்பாவ "../precomp.hpp"

#அந்தர்பாவ <vector>

#அந்தர்பாவ <opencv2/core/utils/logger.defines.hpp>
#undef CV_LOG_STRIP_LEVEL
#define CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_VERBOSE + 1
#அந்தர்பாவ "opencv2/core/utils/logger.hpp"
#அந்தர்பாவ "opencv2/core/utils/filesystem.hpp"
#அந்தர்பாவ "opencv2/core/utils/filesystem.private.hpp"

நாமாகாஶ cv { நாமாகாஶ samples {

நிலையான cv::Ptr< std::vector<cv::String> > g_data_தேடல்_path;
நிலையான cv::Ptr< std::vector<cv::String> > g_data_தேடல்_subdir;

நிலையான std::vector<cv::String>& get_Data_தேடல்_Path()
{
    எனில் (g_data_தேடல்_path.empty())
        g_data_தேடல்_path.reset(புதிய std::vector<cv::String>());
    திரும்பு *(g_data_தேடல்_path.get());
}

நிலையான std::vector<cv::String>& get_Data_தேடல்_Sub_Directory()
{
    எனில் (g_data_தேடல்_subdir.empty())
    {
        g_data_தேடல்_subdir.reset(புதிய std::vector<cv::String>());
        g_data_தேடல்_subdir->push_back("samples/data");
        g_data_தேடல்_subdir->push_back("data");
        g_data_தேடல்_subdir->push_back("");
    }
    திரும்பு *(g_data_தேடல்_subdir.get());
}


CV_EXPORTS வெறுமை add_Samples_Data_தேடல்_Path(மாறிலி cv::String& path)
{
    எனில் (utils::fs::isDirectory(path))
        get_Data_தேடல்_Path().push_back(path);
}
CV_EXPORTS வெறுமை add_Samples_Data_தேடல்_Sub_Directory(மாறிலி cv::String& subdir)
{
    get_Data_தேடல்_Sub_Directory().push_back(subdir);
}

cv::String findFile(மாறிலி cv::String& relative_path, தருக்க required, தருக்க silentMode)
{
#எனில் OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_LOG_DEBUG(வெற்று, cv::format("cv::samples::findFile('%s', %s)", relative_path.c_str(), required ? "true" : "false"));
    cv::String result = cv::utils::findDataFile(relative_path,
                                                "OPENCV_SAMPLES_DATA_PATH",
                                                &get_Data_தேடல்_Path(),
                                                &get_Data_தேடல்_Sub_Directory());
    எனில் (result != relative_path && !silentMode)
    {
        CV_LOG_WARNING(வெற்று, "cv::samples::findFile('" << relative_path << "') => '" << result << "'");
    }
    எனில் (result.empty() && required)
        CV_Error(cv::Error::StsError, cv::format("OpenCV samples: Can't find required data file: %s", relative_path.c_str()));
    திரும்பு result;
#இல்லையெனில்
    CV_UNUSED(relative_path);
    CV_UNUSED(required);
    CV_UNUSED(silentMode);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#endif
}


}} // namespace
