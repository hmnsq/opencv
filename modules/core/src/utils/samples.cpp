// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#अन्तर्भाव "../precomp.hpp"

#अन्तर्भाव <vector>

#अन्तर्भाव <opencv2/core/utils/logger.defines.hpp>
#अपरिभाषय CV_LOG_STRIP_LEVEL
#परिभाषय CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_VERBOSE + 1
#अन्तर्भाव "opencv2/core/utils/logger.hpp"
#अन्तर्भाव "opencv2/core/utils/filesystem.hpp"
#अन्तर्भाव "opencv2/core/utils/filesystem.private.hpp"

नामाकाश cv { नामाकाश samples {

स्थिर cv::Ptr< std::vector<cv::String> > g_दत्त_अन्वेषणम्_मार्गः;
स्थिर cv::Ptr< std::vector<cv::String> > g_दत्त_अन्वेषणम्_subdir;

स्थिर std::vector<cv::String>& get_दत्त_अन्वेषणम्_मार्गः()
{
    यदि (g_दत्त_अन्वेषणम्_मार्गः.empty())
        g_दत्त_अन्वेषणम्_मार्गः.reset(नूतन std::vector<cv::String>());
    प्रत्यागम *(g_दत्त_अन्वेषणम्_मार्गः.get());
}

स्थिर std::vector<cv::String>& get_दत्त_अन्वेषणम्_Sub_Directory()
{
    यदि (g_दत्त_अन्वेषणम्_subdir.empty())
    {
        g_दत्त_अन्वेषणम्_subdir.reset(नूतन std::vector<cv::String>());
        g_दत्त_अन्वेषणम्_subdir->push_back("samples/data");
        g_दत्त_अन्वेषणम्_subdir->push_back("data");
        g_दत्त_अन्वेषणम्_subdir->push_back("");
    }
    प्रत्यागम *(g_दत्त_अन्वेषणम्_subdir.get());
}


CV_EXPORTS शून्य सङ्कलय_Samples_दत्त_अन्वेषणम्_मार्गः(ध्रुव cv::String& मार्गः)
{
    यदि (utils::fs::isDirectory(मार्गः))
        get_दत्त_अन्वेषणम्_मार्गः().push_back(मार्गः);
}
CV_EXPORTS शून्य सङ्कलय_Samples_दत्त_अन्वेषणम्_Sub_Directory(ध्रुव cv::String& subdir)
{
    get_दत्त_अन्वेषणम्_Sub_Directory().push_back(subdir);
}

cv::String findFile(ध्रुव cv::String& relative_मार्गः, तार्किक required, तार्किक silent_बहुलकम्)
{
#यदि OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_LOG_DEBUG(रिक्त, cv::format("cv::samples::findFile('%s', %s)", relative_मार्गः.c_str(), required ? "true" : "false"));
    cv::String परिणाम = cv::utils::find_दत्त_File(relative_मार्गः,
                                                "OPENCV_SAMPLES_DATA_PATH",
                                                &get_दत्त_अन्वेषणम्_मार्गः(),
                                                &get_दत्त_अन्वेषणम्_Sub_Directory());
    यदि (परिणाम != relative_मार्गः && !silent_बहुलकम्)
    {
        CV_LOG_WARNING(रिक्त, "cv::samples::findFile('" << relative_मार्गः << "') => '" << परिणाम << "'");
    }
    यदि (परिणाम.empty() && required)
        CV_Error(cv::Error::StsError, cv::format("OpenCV samples: Can't find required data file: %s", relative_मार्गः.c_str()));
    प्रत्यागम परिणाम;
#अन्यथा
    CV_अप्रयुक्तम्(relative_मार्गः);
    CV_अप्रयुक्तम्(required);
    CV_अप्रयुक्तम्(silent_बहुलकम्);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#यदि_अन्तः
}


}} // namespace
