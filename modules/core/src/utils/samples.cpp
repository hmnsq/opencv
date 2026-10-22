// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ضمن "../precomp.hpp"

#ضمن <vector>

#ضمن <opencv2/core/utils/logger.defines.hpp>
#تعریف_رد CV_LOG_STRIP_LEVEL
#تعریف_کرو CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_VERBOSE + 1
#ضمن "opencv2/core/utils/logger.hpp"
#ضمن "opencv2/core/utils/filesystem.hpp"
#ضمن "opencv2/core/utils/filesystem.private.hpp"

نطاق cv { نطاق samples {

ساكن cv::Ptr< std::vector<cv::String> > g_انکڑے_تلاش_رستہ;
ساكن cv::Ptr< std::vector<cv::String> > g_انکڑے_تلاش_subdir;

ساكن std::vector<cv::String>& get_انکڑے_تلاش_رستہ()
{
    إذا (g_انکڑے_تلاش_رستہ.empty())
        g_انکڑے_تلاش_رستہ.reset(جديد std::vector<cv::String>());
    أرجع *(g_انکڑے_تلاش_رستہ.get());
}

ساكن std::vector<cv::String>& get_انکڑے_تلاش_Sub_Directory()
{
    إذا (g_انکڑے_تلاش_subdir.empty())
    {
        g_انکڑے_تلاش_subdir.reset(جديد std::vector<cv::String>());
        g_انکڑے_تلاش_subdir->push_back("samples/data");
        g_انکڑے_تلاش_subdir->push_back("data");
        g_انکڑے_تلاش_subdir->push_back("");
    }
    أرجع *(g_انکڑے_تلاش_subdir.get());
}


CV_EXPORTS فراغ جوڑو_Samples_انکڑے_تلاش_رستہ(ثابت cv::String& رستہ)
{
    إذا (utils::fs::isDirectory(رستہ))
        get_انکڑے_تلاش_رستہ().push_back(رستہ);
}
CV_EXPORTS فراغ جوڑو_Samples_انکڑے_تلاش_Sub_Directory(ثابت cv::String& subdir)
{
    get_انکڑے_تلاش_Sub_Directory().push_back(subdir);
}

cv::String لبھو_File(ثابت cv::String& relative_رستہ, منطقي required, منطقي silent_منوال)
{
#إذا OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_LOG_DEBUG(عدم, cv::format("cv::samples::findFile('%s', %s)", relative_رستہ.c_str(), required ? "true" : "false"));
    cv::String نتیجہ = cv::utils::لبھو_انکڑے_File(relative_رستہ,
                                                "OPENCV_SAMPLES_DATA_PATH",
                                                &get_انکڑے_تلاش_رستہ(),
                                                &get_انکڑے_تلاش_Sub_Directory());
    إذا (نتیجہ != relative_رستہ && !silent_منوال)
    {
        CV_LOG_WARNING(عدم, "cv::samples::findFile('" << relative_رستہ << "') => '" << نتیجہ << "'");
    }
    إذا (نتیجہ.empty() && required)
        CV_Error(cv::Error::StsError, cv::format("OpenCV samples: Can't find required data file: %s", relative_رستہ.c_str()));
    أرجع نتیجہ;
#وإلا
    CV_بن_ورتیا(relative_رستہ);
    CV_بن_ورتیا(required);
    CV_بن_ورتیا(silent_منوال);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#جے_انت
}


}} // namespace
