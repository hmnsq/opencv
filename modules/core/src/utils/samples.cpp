// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ضمن "../precomp.hpp"

#ضمن <vector>

#ضمن <opencv2/core/utils/logger.defines.hpp>
#لغو_تعریف CV_LOG_STRIP_LEVEL
#تعریف_کن CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_VERBOSE + 1
#ضمن "opencv2/core/utils/logger.hpp"
#ضمن "opencv2/core/utils/filesystem.hpp"
#ضمن "opencv2/core/utils/filesystem.private.hpp"

نطاق cv { نطاق samples {

ساكن cv::Ptr< std::vector<cv::String> > g_داده_جستجو_مسیر;
ساكن cv::Ptr< std::vector<cv::String> > g_داده_جستجو_subdir;

ساكن std::vector<cv::String>& get_داده_جستجو_مسیر()
{
    إذا (g_داده_جستجو_مسیر.empty())
        g_داده_جستجو_مسیر.reset(جديد std::vector<cv::String>());
    أرجع *(g_داده_جستجو_مسیر.get());
}

ساكن std::vector<cv::String>& get_داده_جستجو_Sub_Directory()
{
    إذا (g_داده_جستجو_subdir.empty())
    {
        g_داده_جستجو_subdir.reset(جديد std::vector<cv::String>());
        g_داده_جستجو_subdir->push_back("samples/data");
        g_داده_جستجو_subdir->push_back("data");
        g_داده_جستجو_subdir->push_back("");
    }
    أرجع *(g_داده_جستجو_subdir.get());
}


CV_EXPORTS فراغ جمع_کن_Samples_داده_جستجو_مسیر(ثابت cv::String& مسیر)
{
    إذا (utils::fs::isDirectory(مسیر))
        get_داده_جستجو_مسیر().push_back(مسیر);
}
CV_EXPORTS فراغ جمع_کن_Samples_داده_جستجو_Sub_Directory(ثابت cv::String& subdir)
{
    get_داده_جستجو_Sub_Directory().push_back(subdir);
}

cv::String پیدا_کن_File(ثابت cv::String& relative_مسیر, منطقي required, منطقي silent_نما)
{
#إذا OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_LOG_DEBUG(عدم, cv::format("cv::samples::findFile('%s', %s)", relative_مسیر.c_str(), required ? "true" : "false"));
    cv::String نتیجه = cv::utils::پیدا_کن_داده_File(relative_مسیر,
                                                "OPENCV_SAMPLES_DATA_PATH",
                                                &get_داده_جستجو_مسیر(),
                                                &get_داده_جستجو_Sub_Directory());
    إذا (نتیجه != relative_مسیر && !silent_نما)
    {
        CV_LOG_WARNING(عدم, "cv::samples::findFile('" << relative_مسیر << "') => '" << نتیجه << "'");
    }
    إذا (نتیجه.empty() && required)
        CV_Error(cv::Error::StsError, cv::format("OpenCV samples: Can't find required data file: %s", relative_مسیر.c_str()));
    أرجع نتیجه;
#وإلا
    CV_استفادهنشده(relative_مسیر);
    CV_استفادهنشده(required);
    CV_استفادهنشده(silent_نما);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#پایان_اگر
}


}} // namespace
