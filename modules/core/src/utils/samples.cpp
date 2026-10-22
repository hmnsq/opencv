// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ਅਨ੍ਤਰ੍ਭਾਵ "../precomp.hpp"

#ਅਨ੍ਤਰ੍ਭਾਵ <vector>

#ਅਨ੍ਤਰ੍ਭਾਵ <opencv2/core/utils/logger.defines.hpp>
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ CV_LOG_STRIP_LEVEL
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_VERBOSE + 1
#ਅਨ੍ਤਰ੍ਭਾਵ "opencv2/core/utils/logger.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "opencv2/core/utils/filesystem.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "opencv2/core/utils/filesystem.private.hpp"

ਨਾਮਾਕਾਸ਼ cv { ਨਾਮਾਕਾਸ਼ samples {

ਸਥਿਰ cv::Ptr< std::vector<cv::String> > g_ਅੰਕੜੇ_ਖੋਜ_ਰਸਤਾ;
ਸਥਿਰ cv::Ptr< std::vector<cv::String> > g_ਅੰਕੜੇ_ਖੋਜ_subdir;

ਸਥਿਰ std::vector<cv::String>& get_ਅੰਕੜੇ_ਖੋਜ_ਰਸਤਾ()
{
    ਜੇ (g_ਅੰਕੜੇ_ਖੋਜ_ਰਸਤਾ.empty())
        g_ਅੰਕੜੇ_ਖੋਜ_ਰਸਤਾ.reset(ਨਵਾਂ std::vector<cv::String>());
    ਵਾਪਸ *(g_ਅੰਕੜੇ_ਖੋਜ_ਰਸਤਾ.get());
}

ਸਥਿਰ std::vector<cv::String>& get_ਅੰਕੜੇ_ਖੋਜ_Sub_Directory()
{
    ਜੇ (g_ਅੰਕੜੇ_ਖੋਜ_subdir.empty())
    {
        g_ਅੰਕੜੇ_ਖੋਜ_subdir.reset(ਨਵਾਂ std::vector<cv::String>());
        g_ਅੰਕੜੇ_ਖੋਜ_subdir->push_back("samples/data");
        g_ਅੰਕੜੇ_ਖੋਜ_subdir->push_back("data");
        g_ਅੰਕੜੇ_ਖੋਜ_subdir->push_back("");
    }
    ਵਾਪਸ *(g_ਅੰਕੜੇ_ਖੋਜ_subdir.get());
}


CV_EXPORTS ਖਾਲੀ ਜੋੜੋ_Samples_ਅੰਕੜੇ_ਖੋਜ_ਰਸਤਾ(ਅਚਲ cv::String& ਰਸਤਾ)
{
    ਜੇ (utils::fs::isDirectory(ਰਸਤਾ))
        get_ਅੰਕੜੇ_ਖੋਜ_ਰਸਤਾ().push_back(ਰਸਤਾ);
}
CV_EXPORTS ਖਾਲੀ ਜੋੜੋ_Samples_ਅੰਕੜੇ_ਖੋਜ_Sub_Directory(ਅਚਲ cv::String& subdir)
{
    get_ਅੰਕੜੇ_ਖੋਜ_Sub_Directory().push_back(subdir);
}

cv::String ਲੱਭੋ_File(ਅਚਲ cv::String& relative_ਰਸਤਾ, ਤਾਰਕਿਕ required, ਤਾਰਕਿਕ silent_ਬਹੁਲਕ)
{
#ਜੇ OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_LOG_DEBUG(ਨਿਰਾ, cv::format("cv::samples::findFile('%s', %s)", relative_ਰਸਤਾ.c_str(), required ? "true" : "false"));
    cv::String ਨਤੀਜਾ = cv::utils::ਲੱਭੋ_ਅੰਕੜੇ_File(relative_ਰਸਤਾ,
                                                "OPENCV_SAMPLES_DATA_PATH",
                                                &get_ਅੰਕੜੇ_ਖੋਜ_ਰਸਤਾ(),
                                                &get_ਅੰਕੜੇ_ਖੋਜ_Sub_Directory());
    ਜੇ (ਨਤੀਜਾ != relative_ਰਸਤਾ && !silent_ਬਹੁਲਕ)
    {
        CV_LOG_WARNING(ਨਿਰਾ, "cv::samples::findFile('" << relative_ਰਸਤਾ << "') => '" << ਨਤੀਜਾ << "'");
    }
    ਜੇ (ਨਤੀਜਾ.empty() && required)
        CV_Error(cv::Error::StsError, cv::format("OpenCV samples: Can't find required data file: %s", relative_ਰਸਤਾ.c_str()));
    ਵਾਪਸ ਨਤੀਜਾ;
#ਨਹੀਂਤਾਂ
    CV_ਅਣਵਰਤਿਆ(relative_ਰਸਤਾ);
    CV_ਅਣਵਰਤਿਆ(required);
    CV_ਅਣਵਰਤਿਆ(silent_ਬਹੁਲਕ);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#ਜੇ_ਅੰਤ
}


}} // namespace
