// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

//
// Not a standalone header, part of filesystem.cpp
//

#ضمن "opencv2/core/utils/plugin_loader.private.hpp"

#إذا !OPENCV_HAVE_FILESYSTEM_SUPPORT
#غلطی "Invalid build configuration"
#جے_انت

#إذا 0  // TODO
#جے_تعریف_شدہ NDEBUG
#تعریف_کرو CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_DEBUG + 1
#وإلا
#تعریف_کرو CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_VERBOSE + 1
#جے_انت
#ضمن <opencv2/core/utils/logger.hpp>
#جے_انت

نطاق cv { نطاق plugin { نطاق impl {

DynamicLib::DynamicLib(ثابت File_System_رستہ_t& filename)
    : handle(0), fname(filename), disableAutoUnloading_(خطأ)
{
    library_لدو(filename);
}

DynamicLib::~DynamicLib()
{
    إذا (!disableAutoUnloading_)
    {
        libraryRelease();
    }
    وإلا إذا (handle)
    {
        CV_LOG_INFO(عدم, "skip auto unloading (disabled): " << منزل_Printable_رستہ(fname));
        handle = 0;
    }
}

فراغ* DynamicLib::get_علامت(ثابت حرف* علامت_Name) ثابت
{
    إذا (!handle)
    {
        أرجع 0;
    }
    فراغ* res = get_علامت_2(handle, علامت_Name);
    إذا (!res)
    {
        CV_LOG_DEBUG(عدم, "No symbol '" << علامت_Name << "' in " << منزل_Printable_رستہ(fname));
    }
    أرجع res;
}

std::string DynamicLib::getName() ثابت
{
    أرجع منزل_Printable_رستہ(fname);
}

فراغ DynamicLib::library_لدو(ثابت File_System_رستہ_t& filename)
{
    handle = library_لدو_2(filename);
    CV_LOG_INFO(عدم, "load " << منزل_Printable_رستہ(filename) << " => " << (handle ? "OK" : "FAILED"));
}

فراغ DynamicLib::libraryRelease()
{
    إذا (handle)
    {
        CV_LOG_INFO(عدم, "unload "<< منزل_Printable_رستہ(fname));
        libraryRelease_(handle);
        handle = 0;
    }
}

}}}  // namespace
