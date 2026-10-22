// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ضمن "../precomp.hpp"

#ضمن <vector>

#ضمن <opencv2/core/utils/logger.defines.hpp>
#وصف_رد CV_LOG_STRIP_LEVEL
#وصف_ڪريو CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_VERBOSE + 1
#ضمن "opencv2/core/utils/logger.hpp"
#ضمن "opencv2/core/utils/filesystem.hpp"
#ضمن "opencv2/core/utils/filesystem.private.hpp"

نطاق cv { نطاق samples {

ساكن cv::Ptr< std::vector<cv::String> > g_معلومات_ڳولا_رستو;
ساكن cv::Ptr< std::vector<cv::String> > g_معلومات_ڳولا_subdir;

ساكن std::vector<cv::String>& get_معلومات_ڳولا_رستو()
{
    إذا (g_معلومات_ڳولا_رستو.empty())
        g_معلومات_ڳولا_رستو.reset(جديد std::vector<cv::String>());
    أرجع *(g_معلومات_ڳولا_رستو.get());
}

ساكن std::vector<cv::String>& get_معلومات_ڳولا_Sub_Directory()
{
    إذا (g_معلومات_ڳولا_subdir.empty())
    {
        g_معلومات_ڳولا_subdir.reset(جديد std::vector<cv::String>());
        g_معلومات_ڳولا_subdir->push_back("samples/data");
        g_معلومات_ڳولا_subdir->push_back("data");
        g_معلومات_ڳولا_subdir->push_back("");
    }
    أرجع *(g_معلومات_ڳولا_subdir.get());
}


CV_EXPORTS فراغ جوڙيو_Samples_معلومات_ڳولا_رستو(ثابت cv::String& رستو)
{
    إذا (utils::fs::isDirectory(رستو))
        get_معلومات_ڳولا_رستو().push_back(رستو);
}
CV_EXPORTS فراغ جوڙيو_Samples_معلومات_ڳولا_Sub_Directory(ثابت cv::String& subdir)
{
    get_معلومات_ڳولا_Sub_Directory().push_back(subdir);
}

cv::String ڳوليو_File(ثابت cv::String& relative_رستو, منطقي required, منطقي silent_گهڻائي)
{
#إذا OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_LOG_DEBUG(عدم, cv::format("cv::samples::findFile('%s', %s)", relative_رستو.c_str(), required ? "true" : "false"));
    cv::String نتيجو = cv::utils::ڳوليو_معلومات_File(relative_رستو,
                                                "OPENCV_SAMPLES_DATA_PATH",
                                                &get_معلومات_ڳولا_رستو(),
                                                &get_معلومات_ڳولا_Sub_Directory());
    إذا (نتيجو != relative_رستو && !silent_گهڻائي)
    {
        CV_LOG_WARNING(عدم, "cv::samples::findFile('" << relative_رستو << "') => '" << نتيجو << "'");
    }
    إذا (نتيجو.empty() && required)
        CV_Error(cv::Error::StsError, cv::format("OpenCV samples: Can't find required data file: %s", relative_رستو.c_str()));
    أرجع نتيجو;
#وإلا
    CV_اڻ_استعمال_ٿيل(relative_رستو);
    CV_اڻ_استعمال_ٿيل(required);
    CV_اڻ_استعمال_ٿيل(silent_گهڻائي);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#جيڪڏهن_پورو
}


}} // namespace
