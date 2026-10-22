// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ಅನ್ತರ್ಭಾವ "../precomp.hpp"

#ಅನ್ತರ್ಭಾವ <vector>

#ಅನ್ತರ್ಭಾವ <opencv2/core/utils/logger.defines.hpp>
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು CV_LOG_STRIP_LEVEL
#ವ್ಯಾಖ್ಯಾನಿಸು CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_VERBOSE + 1
#ಅನ್ತರ್ಭಾವ "opencv2/core/utils/logger.hpp"
#ಅನ್ತರ್ಭಾವ "opencv2/core/utils/filesystem.hpp"
#ಅನ್ತರ್ಭಾವ "opencv2/core/utils/filesystem.private.hpp"

ನಾಮಾಕಾಶ cv { ನಾಮಾಕಾಶ samples {

ಸ್ಥಿರ cv::Ptr< std::vector<cv::String> > g_ದತ್ತಾಂಶ_ಹುಡುಕು_ಮಾರ್ಗ;
ಸ್ಥಿರ cv::Ptr< std::vector<cv::String> > g_ದತ್ತಾಂಶ_ಹುಡುಕು_subdir;

ಸ್ಥಿರ std::vector<cv::String>& get_ದತ್ತಾಂಶ_ಹುಡುಕು_ಮಾರ್ಗ()
{
    ಆದರೆ (g_ದತ್ತಾಂಶ_ಹುಡುಕು_ಮಾರ್ಗ.empty())
        g_ದತ್ತಾಂಶ_ಹುಡುಕು_ಮಾರ್ಗ.reset(ಹೊಸ std::vector<cv::String>());
    ಹಿಂತಿರುಗಿ *(g_ದತ್ತಾಂಶ_ಹುಡುಕು_ಮಾರ್ಗ.get());
}

ಸ್ಥಿರ std::vector<cv::String>& get_ದತ್ತಾಂಶ_ಹುಡುಕು_Sub_Directory()
{
    ಆದರೆ (g_ದತ್ತಾಂಶ_ಹುಡುಕು_subdir.empty())
    {
        g_ದತ್ತಾಂಶ_ಹುಡುಕು_subdir.reset(ಹೊಸ std::vector<cv::String>());
        g_ದತ್ತಾಂಶ_ಹುಡುಕು_subdir->push_back("samples/data");
        g_ದತ್ತಾಂಶ_ಹುಡುಕು_subdir->push_back("data");
        g_ದತ್ತಾಂಶ_ಹುಡುಕು_subdir->push_back("");
    }
    ಹಿಂತಿರುಗಿ *(g_ದತ್ತಾಂಶ_ಹುಡುಕು_subdir.get());
}


CV_EXPORTS ಶೂನ್ಯ ಕೂಡಿಸು_Samples_ದತ್ತಾಂಶ_ಹುಡುಕು_ಮಾರ್ಗ(ಸ್ಥಿರಾಂಕ cv::String& ಮಾರ್ಗ)
{
    ಆದರೆ (utils::fs::isDirectory(ಮಾರ್ಗ))
        get_ದತ್ತಾಂಶ_ಹುಡುಕು_ಮಾರ್ಗ().push_back(ಮಾರ್ಗ);
}
CV_EXPORTS ಶೂನ್ಯ ಕೂಡಿಸು_Samples_ದತ್ತಾಂಶ_ಹುಡುಕು_Sub_Directory(ಸ್ಥಿರಾಂಕ cv::String& subdir)
{
    get_ದತ್ತಾಂಶ_ಹುಡುಕು_Sub_Directory().push_back(subdir);
}

cv::String ಕಂಡುಹಿಡಿ_File(ಸ್ಥಿರಾಂಕ cv::String& relative_ಮಾರ್ಗ, ತಾರ್ಕಿಕ required, ತಾರ್ಕಿಕ silent_ಬಹುಲಕ)
{
#ಆದರೆ OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_LOG_DEBUG(ಖಾಲಿ, cv::format("cv::samples::findFile('%s', %s)", relative_ಮಾರ್ಗ.c_str(), required ? "true" : "false"));
    cv::String ಫಲಿತಾಂಶ = cv::utils::ಕಂಡುಹಿಡಿ_ದತ್ತಾಂಶ_File(relative_ಮಾರ್ಗ,
                                                "OPENCV_SAMPLES_DATA_PATH",
                                                &get_ದತ್ತಾಂಶ_ಹುಡುಕು_ಮಾರ್ಗ(),
                                                &get_ದತ್ತಾಂಶ_ಹುಡುಕು_Sub_Directory());
    ಆದರೆ (ಫಲಿತಾಂಶ != relative_ಮಾರ್ಗ && !silent_ಬಹುಲಕ)
    {
        CV_LOG_WARNING(ಖಾಲಿ, "cv::samples::findFile('" << relative_ಮಾರ್ಗ << "') => '" << ಫಲಿತಾಂಶ << "'");
    }
    ಆದರೆ (ಫಲಿತಾಂಶ.empty() && required)
        CV_Error(cv::Error::StsError, cv::format("OpenCV samples: Can't find required data file: %s", relative_ಮಾರ್ಗ.c_str()));
    ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
#ಇಲ್ಲದಿದ್ದರೆ
    CV_ಬಳಸದ(relative_ಮಾರ್ಗ);
    CV_ಬಳಸದ(required);
    CV_ಬಳಸದ(silent_ಬಹುಲಕ);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#ಆದರೆ_ಅಂತ್ಯ
}


}} // namespace
