// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ضمن "../precomp.hpp"

#ضمن <vector>

#ضمن <opencv2/core/utils/logger.defines.hpp>
#تعریف_لغوه CV_LOG_STRIP_LEVEL
#تعریف_کړه CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_VERBOSE + 1
#ضمن "opencv2/core/utils/logger.hpp"
#ضمن "opencv2/core/utils/filesystem.hpp"
#ضمن "opencv2/core/utils/filesystem.private.hpp"

نطاق cv { نطاق samples {

ساكن cv::Ptr< std::vector<cv::String> > g_معلومات_پلټنه_لار;
ساكن cv::Ptr< std::vector<cv::String> > g_معلومات_پلټنه_subdir;

ساكن std::vector<cv::String>& get_معلومات_پلټنه_لار()
{
    إذا (g_معلومات_پلټنه_لار.empty())
        g_معلومات_پلټنه_لار.reset(جديد std::vector<cv::String>());
    أرجع *(g_معلومات_پلټنه_لار.get());
}

ساكن std::vector<cv::String>& get_معلومات_پلټنه_Sub_Directory()
{
    إذا (g_معلومات_پلټنه_subdir.empty())
    {
        g_معلومات_پلټنه_subdir.reset(جديد std::vector<cv::String>());
        g_معلومات_پلټنه_subdir->push_back("samples/data");
        g_معلومات_پلټنه_subdir->push_back("data");
        g_معلومات_پلټنه_subdir->push_back("");
    }
    أرجع *(g_معلومات_پلټنه_subdir.get());
}


CV_EXPORTS فراغ جمع_کړه_Samples_معلومات_پلټنه_لار(ثابت cv::String& لار)
{
    إذا (utils::fs::isDirectory(لار))
        get_معلومات_پلټنه_لار().push_back(لار);
}
CV_EXPORTS فراغ جمع_کړه_Samples_معلومات_پلټنه_Sub_Directory(ثابت cv::String& subdir)
{
    get_معلومات_پلټنه_Sub_Directory().push_back(subdir);
}

cv::String پیدا_کړه_File(ثابت cv::String& relative_لار, منطقي required, منطقي silent_ډېرپېښ)
{
#إذا OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_LOG_DEBUG(عدم, cv::format("cv::samples::findFile('%s', %s)", relative_لار.c_str(), required ? "true" : "false"));
    cv::String پایله = cv::utils::پیدا_کړه_معلومات_File(relative_لار,
                                                "OPENCV_SAMPLES_DATA_PATH",
                                                &get_معلومات_پلټنه_لار(),
                                                &get_معلومات_پلټنه_Sub_Directory());
    إذا (پایله != relative_لار && !silent_ډېرپېښ)
    {
        CV_LOG_WARNING(عدم, "cv::samples::findFile('" << relative_لار << "') => '" << پایله << "'");
    }
    إذا (پایله.empty() && required)
        CV_Error(cv::Error::StsError, cv::format("OpenCV samples: Can't find required data file: %s", relative_لار.c_str()));
    أرجع پایله;
#وإلا
    CV_ناکارول_شوی(relative_لار);
    CV_ناکارول_شوی(required);
    CV_ناکارول_شوی(silent_ډېرپېښ);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#د_که_پای
}


}} // namespace
