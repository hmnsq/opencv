// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

//
// Not a standalone header, part of filesystem.cpp
//

#అన్తర్భావ "opencv2/core/utils/plugin_loader.private.hpp"

#ఒకవేళ !OPENCV_HAVE_FILESYSTEM_SUPPORT
#దోషం "Invalid build configuration"
#అయితే_ముగింపు

#ఒకవేళ 0  // TODO
#నిర్వచితమైతే NDEBUG
#నిర్వచించు CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_DEBUG + 1
#లేకపోతే
#నిర్వచించు CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_VERBOSE + 1
#అయితే_ముగింపు
#అన్తర్భావ <opencv2/core/utils/logger.hpp>
#అయితే_ముగింపు

నామాకాశ cv { నామాకాశ plugin { నామాకాశ impl {

DynamicLib::DynamicLib(స్థిరరాశి File_System_మార్గం_t& filename)
    : handle(0), fname(filename), disableAutoUnloading_(అబద్ధం)
{
    library_ఎక్కించు(filename);
}

DynamicLib::~DynamicLib()
{
    ఒకవేళ (!disableAutoUnloading_)
    {
        libraryRelease();
    }
    లేకపోతే ఒకవేళ (handle)
    {
        CV_LOG_INFO(ఖాళీ, "skip auto unloading (disabled): " << గమ్యం_Printable_మార్గం(fname));
        handle = 0;
    }
}

శూన్య* DynamicLib::get_చిహ్నం(స్థిరరాశి అక్షరం* చిహ్నం_Name) స్థిరరాశి
{
    ఒకవేళ (!handle)
    {
        తిరిగి 0;
    }
    శూన్య* res = get_చిహ్నం_2(handle, చిహ్నం_Name);
    ఒకవేళ (!res)
    {
        CV_LOG_DEBUG(ఖాళీ, "No symbol '" << చిహ్నం_Name << "' in " << గమ్యం_Printable_మార్గం(fname));
    }
    తిరిగి res;
}

std::string DynamicLib::getName() స్థిరరాశి
{
    తిరిగి గమ్యం_Printable_మార్గం(fname);
}

శూన్య DynamicLib::library_ఎక్కించు(స్థిరరాశి File_System_మార్గం_t& filename)
{
    handle = library_ఎక్కించు_2(filename);
    CV_LOG_INFO(ఖాళీ, "load " << గమ్యం_Printable_మార్గం(filename) << " => " << (handle ? "OK" : "FAILED"));
}

శూన్య DynamicLib::libraryRelease()
{
    ఒకవేళ (handle)
    {
        CV_LOG_INFO(ఖాళీ, "unload "<< గమ్యం_Printable_మార్గం(fname));
        libraryRelease_(handle);
        handle = 0;
    }
}

}}}  // namespace
