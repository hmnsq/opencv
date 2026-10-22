// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

//
// Not a standalone header, part of va_intel.cpp
//

#అన్తర్భావ "opencv2/core/utils/plugin_loader.private.hpp"  // DynamicLib
#అన్తర్భావ "opencv2/core/utils/configuration.private.hpp"

నామాకాశ cv { నామాకాశ detail {

రకనిర్వచనం VAStatus (*FN_vaDeriveImage)(VADisplay dpy, VASurfaceID surface, VAImage *image);
రకనిర్వచనం VAStatus (*FN_vaDestroyImage)(VADisplay dpy, VAImageID image);
రకనిర్వచనం VAStatus (*FN_va_Map_బఫరు)(VADisplay dpy, VABufferID buf_id, శూన్య **pbuf);
రకనిర్వచనం VAStatus (*FN_vaSyncSurface)(VADisplay dpy, VASurfaceID render_లక్ష్యం);
రకనిర్వచనం VAStatus (*FN_va_Unmap_బఫరు)(VADisplay dpy, VABufferID buf_id);
రకనిర్వచనం పూర్ణాంకం (*FN_va_Max_అంకె_Image_Formats)(VADisplay dpy);
రకనిర్వచనం VAStatus (*FN_vaQueryImageFormats)(VADisplay dpy, VAImageFormat *format_list, పూర్ణాంకం *అంకె_formats);
రకనిర్వచనం VAStatus (*FN_vaCreateImage)(VADisplay dpy, VAImageFormat *format, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు, VAImage *image);
రకనిర్వచనం VAStatus (*FN_vaPutImage)(VADisplay dpy, VASurfaceID surface, VAImageID image, పూర్ణాంకం src_x, పూర్ణాంకం src_y, గుర్తులేని పూర్ణాంకం src_వెడల్పు, గుర్తులేని పూర్ణాంకం src_ఎత్తు, పూర్ణాంకం dest_x, పూర్ణాంకం dest_y, గుర్తులేని పూర్ణాంకం dest_వెడల్పు, గుర్తులేని పూర్ణాంకం dest_ఎత్తు);
రకనిర్వచనం VAStatus (*FN_vaGetImage)(VADisplay dpy, VASurfaceID surface, పూర్ణాంకం x, పూర్ణాంకం y, గుర్తులేని పూర్ణాంకం వెడల్పు, గుర్తులేని పూర్ణాంకం ఎత్తు, VAImageID image);

స్థిర FN_vaDeriveImage fn_vaDeriveImage = ఖాళీ;
స్థిర FN_vaDestroyImage fn_vaDestroyImage = ఖాళీ;
స్థిర FN_va_Map_బఫరు fn_va_Map_బఫరు = ఖాళీ;
స్థిర FN_vaSyncSurface fn_vaSyncSurface = ఖాళీ;
స్థిర FN_va_Unmap_బఫరు fn_va_Unmap_బఫరు = ఖాళీ;
స్థిర FN_va_Max_అంకె_Image_Formats fn_va_Max_అంకె_Image_Formats = ఖాళీ;
స్థిర FN_vaQueryImageFormats fn_vaQueryImageFormats = ఖాళీ;
స్థిర FN_vaCreateImage fn_vaCreateImage = ఖాళీ;
స్థిర FN_vaPutImage fn_vaPutImage = ఖాళీ;
స్థిర FN_vaGetImage fn_vaGetImage = ఖాళీ;

#నిర్వచించు vaDeriveImage fn_vaDeriveImage
#నిర్వచించు vaDestroyImage fn_vaDestroyImage
#నిర్వచించు va_Map_బఫరు fn_va_Map_బఫరు
#నిర్వచించు vaSyncSurface fn_vaSyncSurface
#నిర్వచించు va_Unmap_బఫరు fn_va_Unmap_బఫరు
#నిర్వచించు va_Max_అంకె_Image_Formats fn_va_Max_అంకె_Image_Formats
#నిర్వచించు vaQueryImageFormats fn_vaQueryImageFormats
#నిర్వచించు vaCreateImage fn_vaCreateImage
#నిర్వచించు vaPutImage fn_vaPutImage
#నిర్వచించు vaGetImage fn_vaGetImage


స్థిర std::shared_ptr<cv::plugin::impl::DynamicLib> ఎక్కించు_Lib_VA()
{
    std::shared_ptr<cv::plugin::impl::DynamicLib> lib;
    స్థిరరాశి std::string env_మార్గం = utils::getConfigurationParameterString("OPENCV_LIBVA_RUNTIME");
    ఒకవేళ (!env_మార్గం.empty())
    {
        lib = std::make_shared<cv::plugin::impl::DynamicLib>(env_మార్గం);
        తిరిగి lib;
    }
    స్థిర స్థిరరాశి అక్షరం* స్థిరరాశి candidates[] = {
        "libva.so",
        "libva.so.2",
        "libva.so.1",
    };
    కొరకు (పూర్ణాంకం i = 0; i < 3; ++i)
    {
        lib = std::make_shared<cv::plugin::impl::DynamicLib>(candidates[i]);
        ఒకవేళ (lib->isLoaded())
            ఆపు;
    }
    తిరిగి lib;
}
స్థిర శూన్య init_libva()
{
    స్థిర తార్కిక initialized = అబద్ధం;
    స్థిర స్వయంచాలక library = ఎక్కించు_Lib_VA();
    ఒకవేళ (!initialized)
    {
        ఒకవేళ (!library || !library->isLoaded())
        {
            library.reset();
            CV_Error(cv::Error::StsBadFunc, "OpenCV can't load VA library (libva)");
        }
        స్వయంచాలక& lib = *library.get();
#నిర్వచించు VA_ఎక్కించు_చిహ్నం(name) fn_ ## name = reinterpret_cast<FN_ ## name>(lib.get_చిహ్నం(#name)); \
        ఒకవేళ (!fn_ ## name) \
        { \
            library.reset(); \
            initialized = నిజం; \
            CV_Error_(cv::Error::StsBadFunc, ("OpenCV can't load VA library (libva), missing symbol: %s", #name)); \
        }

        VA_ఎక్కించు_చిహ్నం(vaDeriveImage);
        VA_ఎక్కించు_చిహ్నం(vaDestroyImage);
        VA_ఎక్కించు_చిహ్నం(va_Map_బఫరు);
        VA_ఎక్కించు_చిహ్నం(vaSyncSurface);
        VA_ఎక్కించు_చిహ్నం(va_Unmap_బఫరు);
        VA_ఎక్కించు_చిహ్నం(va_Max_అంకె_Image_Formats);
        VA_ఎక్కించు_చిహ్నం(vaQueryImageFormats);
        VA_ఎక్కించు_చిహ్నం(vaCreateImage);
        VA_ఎక్కించు_చిహ్నం(vaPutImage);
        VA_ఎక్కించు_చిహ్నం(vaGetImage);
        initialized = నిజం;
    }
    ఒకవేళ (!library)
        CV_Error(cv::Error::StsBadFunc, "OpenCV can't load/initialize VA library (libva)");
}

}}  // namespace
