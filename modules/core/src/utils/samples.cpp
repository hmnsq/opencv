// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#అన్తర్భావ "../precomp.hpp"

#అన్తర్భావ <vector>

#అన్తర్భావ <opencv2/core/utils/logger.defines.hpp>
#నిర్వచనం_రద్దు CV_LOG_STRIP_LEVEL
#నిర్వచించు CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_VERBOSE + 1
#అన్తర్భావ "opencv2/core/utils/logger.hpp"
#అన్తర్భావ "opencv2/core/utils/filesystem.hpp"
#అన్తర్భావ "opencv2/core/utils/filesystem.private.hpp"

నామాకాశ cv { నామాకాశ samples {

స్థిర cv::Ptr< std::vector<cv::String> > g_దత్తాంశం_శోధన_మార్గం;
స్థిర cv::Ptr< std::vector<cv::String> > g_దత్తాంశం_శోధన_subdir;

స్థిర std::vector<cv::String>& get_దత్తాంశం_శోధన_మార్గం()
{
    ఒకవేళ (g_దత్తాంశం_శోధన_మార్గం.empty())
        g_దత్తాంశం_శోధన_మార్గం.reset(కొత్త std::vector<cv::String>());
    తిరిగి *(g_దత్తాంశం_శోధన_మార్గం.get());
}

స్థిర std::vector<cv::String>& get_దత్తాంశం_శోధన_Sub_Directory()
{
    ఒకవేళ (g_దత్తాంశం_శోధన_subdir.empty())
    {
        g_దత్తాంశం_శోధన_subdir.reset(కొత్త std::vector<cv::String>());
        g_దత్తాంశం_శోధన_subdir->push_back("samples/data");
        g_దత్తాంశం_శోధన_subdir->push_back("data");
        g_దత్తాంశం_శోధన_subdir->push_back("");
    }
    తిరిగి *(g_దత్తాంశం_శోధన_subdir.get());
}


CV_EXPORTS శూన్య కూడు_Samples_దత్తాంశం_శోధన_మార్గం(స్థిరరాశి cv::String& మార్గం)
{
    ఒకవేళ (utils::fs::isDirectory(మార్గం))
        get_దత్తాంశం_శోధన_మార్గం().push_back(మార్గం);
}
CV_EXPORTS శూన్య కూడు_Samples_దత్తాంశం_శోధన_Sub_Directory(స్థిరరాశి cv::String& subdir)
{
    get_దత్తాంశం_శోధన_Sub_Directory().push_back(subdir);
}

cv::String కనుగొను_File(స్థిరరాశి cv::String& relative_మార్గం, తార్కిక required, తార్కిక silent_బహుళకం)
{
#ఒకవేళ OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_LOG_DEBUG(ఖాళీ, cv::format("cv::samples::findFile('%s', %s)", relative_మార్గం.c_str(), required ? "true" : "false"));
    cv::String ఫలితం = cv::utils::కనుగొను_దత్తాంశం_File(relative_మార్గం,
                                                "OPENCV_SAMPLES_DATA_PATH",
                                                &get_దత్తాంశం_శోధన_మార్గం(),
                                                &get_దత్తాంశం_శోధన_Sub_Directory());
    ఒకవేళ (ఫలితం != relative_మార్గం && !silent_బహుళకం)
    {
        CV_LOG_WARNING(ఖాళీ, "cv::samples::findFile('" << relative_మార్గం << "') => '" << ఫలితం << "'");
    }
    ఒకవేళ (ఫలితం.empty() && required)
        CV_Error(cv::Error::StsError, cv::format("OpenCV samples: Can't find required data file: %s", relative_మార్గం.c_str()));
    తిరిగి ఫలితం;
#లేకపోతే
    CV_వాడని(relative_మార్గం);
    CV_వాడని(required);
    CV_వాడని(silent_బహుళకం);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#అయితే_ముగింపు
}


}} // namespace
