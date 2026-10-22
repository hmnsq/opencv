// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#अन्तर्भाव "../precomp.hpp"

#अन्तर्भाव <vector>

#अन्तर्भाव <opencv2/core/utils/logger.defines.hpp>
#undef CV_LOG_STRIP_LEVEL
#define CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_VERBOSE + 1
#अन्तर्भाव "opencv2/core/utils/logger.hpp"
#अन्तर्भाव "opencv2/core/utils/filesystem.hpp"
#अन्तर्भाव "opencv2/core/utils/filesystem.private.hpp"

नामाकाश cv { नामाकाश samples {

स्थिर cv::Ptr< std::vector<cv::String> > g_data_खोज_path;
स्थिर cv::Ptr< std::vector<cv::String> > g_data_खोज_subdir;

स्थिर std::vector<cv::String>& get_Data_खोज_Path()
{
    यदि (g_data_खोज_path.empty())
        g_data_खोज_path.reset(नूतन std::vector<cv::String>());
    फर्काऊ *(g_data_खोज_path.get());
}

स्थिर std::vector<cv::String>& get_Data_खोज_Sub_Directory()
{
    यदि (g_data_खोज_subdir.empty())
    {
        g_data_खोज_subdir.reset(नूतन std::vector<cv::String>());
        g_data_खोज_subdir->push_back("samples/data");
        g_data_खोज_subdir->push_back("data");
        g_data_खोज_subdir->push_back("");
    }
    फर्काऊ *(g_data_खोज_subdir.get());
}


CV_EXPORTS शून्य add_Samples_Data_खोज_Path(अचर cv::String& path)
{
    यदि (utils::fs::isDirectory(path))
        get_Data_खोज_Path().push_back(path);
}
CV_EXPORTS शून्य add_Samples_Data_खोज_Sub_Directory(अचर cv::String& subdir)
{
    get_Data_खोज_Sub_Directory().push_back(subdir);
}

cv::String findFile(अचर cv::String& relative_path, तार्किक required, तार्किक silentMode)
{
#यदि OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_LOG_DEBUG(रिक्त, cv::format("cv::samples::findFile('%s', %s)", relative_path.c_str(), required ? "true" : "false"));
    cv::String result = cv::utils::findDataFile(relative_path,
                                                "OPENCV_SAMPLES_DATA_PATH",
                                                &get_Data_खोज_Path(),
                                                &get_Data_खोज_Sub_Directory());
    यदि (result != relative_path && !silentMode)
    {
        CV_LOG_WARNING(रिक्त, "cv::samples::findFile('" << relative_path << "') => '" << result << "'");
    }
    यदि (result.empty() && required)
        CV_Error(cv::Error::StsError, cv::format("OpenCV samples: Can't find required data file: %s", relative_path.c_str()));
    फर्काऊ result;
#अन्यथा
    CV_UNUSED(relative_path);
    CV_UNUSED(required);
    CV_UNUSED(silentMode);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#endif
}


}} // namespace
