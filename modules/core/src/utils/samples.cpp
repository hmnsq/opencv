// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#شامل "../precomp.hpp"

#شامل <vector>

#شامل <opencv2/core/utils/logger.defines.hpp>
#تعریف_منسوخ CV_LOG_STRIP_LEVEL
#تعریف_کرو CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_VERBOSE + 1
#شامل "opencv2/core/utils/logger.hpp"
#شامل "opencv2/core/utils/filesystem.hpp"
#شامل "opencv2/core/utils/filesystem.private.hpp"

نامخلا cv { نامخلا samples {

ساکن cv::Ptr< std::vector<cv::String> > g_ڈیٹا_تلاش_راستہ;
ساکن cv::Ptr< std::vector<cv::String> > g_ڈیٹا_تلاش_subdir;

ساکن std::vector<cv::String>& get_ڈیٹا_تلاش_راستہ()
{
    اگر (g_ڈیٹا_تلاش_راستہ.empty())
        g_ڈیٹا_تلاش_راستہ.reset(نیا std::vector<cv::String>());
    واپس *(g_ڈیٹا_تلاش_راستہ.get());
}

ساکن std::vector<cv::String>& get_ڈیٹا_تلاش_Sub_Directory()
{
    اگر (g_ڈیٹا_تلاش_subdir.empty())
    {
        g_ڈیٹا_تلاش_subdir.reset(نیا std::vector<cv::String>());
        g_ڈیٹا_تلاش_subdir->push_back("samples/data");
        g_ڈیٹا_تلاش_subdir->push_back("data");
        g_ڈیٹا_تلاش_subdir->push_back("");
    }
    واپس *(g_ڈیٹا_تلاش_subdir.get());
}


CV_EXPORTS خالی جوڑو_Samples_ڈیٹا_تلاش_راستہ(ثابت cv::String& راستہ)
{
    اگر (utils::fs::isDirectory(راستہ))
        get_ڈیٹا_تلاش_راستہ().push_back(راستہ);
}
CV_EXPORTS خالی جوڑو_Samples_ڈیٹا_تلاش_Sub_Directory(ثابت cv::String& subdir)
{
    get_ڈیٹا_تلاش_Sub_Directory().push_back(subdir);
}

cv::String ڈھونڈو_File(ثابت cv::String& relative_راستہ, منطقی required, منطقی silent_منوال)
{
#اگر OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_LOG_DEBUG(تہی, cv::format("cv::samples::findFile('%s', %s)", relative_راستہ.c_str(), required ? "true" : "false"));
    cv::String نتیجہ = cv::utils::ڈھونڈو_ڈیٹا_File(relative_راستہ,
                                                "OPENCV_SAMPLES_DATA_PATH",
                                                &get_ڈیٹا_تلاش_راستہ(),
                                                &get_ڈیٹا_تلاش_Sub_Directory());
    اگر (نتیجہ != relative_راستہ && !silent_منوال)
    {
        CV_LOG_WARNING(تہی, "cv::samples::findFile('" << relative_راستہ << "') => '" << نتیجہ << "'");
    }
    اگر (نتیجہ.empty() && required)
        CV_Error(cv::Error::StsError, cv::format("OpenCV samples: Can't find required data file: %s", relative_راستہ.c_str()));
    واپس نتیجہ;
#ورنہ
    CV_غیر_مستعمل(relative_راستہ);
    CV_غیر_مستعمل(required);
    CV_غیر_مستعمل(silent_منوال);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#اگر_ختم
}


}} // namespace
