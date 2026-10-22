// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

//
// Not a standalone header, part of filesystem.cpp
//

#ضمن "opencv2/core/utils/plugin_loader.private.hpp"

#إذا !OPENCV_HAVE_FILESYSTEM_SUPPORT
#تېروتنه "Invalid build configuration"
#د_که_پای

#إذا 0  // TODO
#که_تعریف_شوی NDEBUG
#تعریف_کړه CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_DEBUG + 1
#وإلا
#تعریف_کړه CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_VERBOSE + 1
#د_که_پای
#ضمن <opencv2/core/utils/logger.hpp>
#د_که_پای

نطاق cv { نطاق plugin { نطاق impl {

DynamicLib::DynamicLib(ثابت File_System_لار_t& filename)
    : handle(0), fname(filename), disableAutoUnloading_(خطأ)
{
    library_بارول(filename);
}

DynamicLib::~DynamicLib()
{
    إذا (!disableAutoUnloading_)
    {
        libraryRelease();
    }
    وإلا إذا (handle)
    {
        CV_LOG_INFO(عدم, "skip auto unloading (disabled): " << منزل_Printable_لار(fname));
        handle = 0;
    }
}

فراغ* DynamicLib::get_نښه(ثابت حرف* نښه_Name) ثابت
{
    إذا (!handle)
    {
        أرجع 0;
    }
    فراغ* res = get_نښه_2(handle, نښه_Name);
    إذا (!res)
    {
        CV_LOG_DEBUG(عدم, "No symbol '" << نښه_Name << "' in " << منزل_Printable_لار(fname));
    }
    أرجع res;
}

std::string DynamicLib::getName() ثابت
{
    أرجع منزل_Printable_لار(fname);
}

فراغ DynamicLib::library_بارول(ثابت File_System_لار_t& filename)
{
    handle = library_بارول_2(filename);
    CV_LOG_INFO(عدم, "load " << منزل_Printable_لار(filename) << " => " << (handle ? "OK" : "FAILED"));
}

فراغ DynamicLib::libraryRelease()
{
    إذا (handle)
    {
        CV_LOG_INFO(عدم, "unload "<< منزل_Printable_لار(fname));
        libraryRelease_(handle);
        handle = 0;
    }
}

}}}  // namespace
