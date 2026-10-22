// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

//
// Not a standalone header, part of filesystem.cpp
//

#ضمن "opencv2/core/utils/plugin_loader.private.hpp"

#إذا !OPENCV_HAVE_FILESYSTEM_SUPPORT
#غلطي "Invalid build configuration"
#جيڪڏهن_پورو

#إذا 0  // TODO
#جيڪڏهن_وصف_ٿيل NDEBUG
#وصف_ڪريو CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_DEBUG + 1
#وإلا
#وصف_ڪريو CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_VERBOSE + 1
#جيڪڏهن_پورو
#ضمن <opencv2/core/utils/logger.hpp>
#جيڪڏهن_پورو

نطاق cv { نطاق plugin { نطاق impl {

DynamicLib::DynamicLib(ثابت File_System_رستو_t& filename)
    : handle(0), fname(filename), disableAutoUnloading_(خطأ)
{
    library_ڀريو(filename);
}

DynamicLib::~DynamicLib()
{
    إذا (!disableAutoUnloading_)
    {
        libraryRelease();
    }
    وإلا إذا (handle)
    {
        CV_LOG_INFO(عدم, "skip auto unloading (disabled): " << منزل_Printable_رستو(fname));
        handle = 0;
    }
}

فراغ* DynamicLib::get_نشان(ثابت حرف* نشان_Name) ثابت
{
    إذا (!handle)
    {
        أرجع 0;
    }
    فراغ* res = get_نشان_2(handle, نشان_Name);
    إذا (!res)
    {
        CV_LOG_DEBUG(عدم, "No symbol '" << نشان_Name << "' in " << منزل_Printable_رستو(fname));
    }
    أرجع res;
}

std::string DynamicLib::getName() ثابت
{
    أرجع منزل_Printable_رستو(fname);
}

فراغ DynamicLib::library_ڀريو(ثابت File_System_رستو_t& filename)
{
    handle = library_ڀريو_2(filename);
    CV_LOG_INFO(عدم, "load " << منزل_Printable_رستو(filename) << " => " << (handle ? "OK" : "FAILED"));
}

فراغ DynamicLib::libraryRelease()
{
    إذا (handle)
    {
        CV_LOG_INFO(عدم, "unload "<< منزل_Printable_رستو(fname));
        libraryRelease_(handle);
        handle = 0;
    }
}

}}}  // namespace
