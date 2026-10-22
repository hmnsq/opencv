// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

//
// Not a standalone header, part of filesystem.cpp
//

#ضمن "opencv2/core/utils/plugin_loader.private.hpp"

#إذا !OPENCV_HAVE_FILESYSTEM_SUPPORT
#خطا "Invalid build configuration"
#پایان_اگر

#إذا 0  // TODO
#اگر_تعریف_شده NDEBUG
#تعریف_کن CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_DEBUG + 1
#وإلا
#تعریف_کن CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_VERBOSE + 1
#پایان_اگر
#ضمن <opencv2/core/utils/logger.hpp>
#پایان_اگر

نطاق cv { نطاق plugin { نطاق impl {

DynamicLib::DynamicLib(ثابت File_System_مسیر_t& filename)
    : handle(0), fname(filename), disableAutoUnloading_(خطأ)
{
    library_بارگیری(filename);
}

DynamicLib::~DynamicLib()
{
    إذا (!disableAutoUnloading_)
    {
        libraryRelease();
    }
    وإلا إذا (handle)
    {
        CV_LOG_INFO(عدم, "skip auto unloading (disabled): " << مقصد_Printable_مسیر(fname));
        handle = 0;
    }
}

فراغ* DynamicLib::get_نماد(ثابت حرف* نماد_Name) ثابت
{
    إذا (!handle)
    {
        أرجع 0;
    }
    فراغ* res = get_نماد_2(handle, نماد_Name);
    إذا (!res)
    {
        CV_LOG_DEBUG(عدم, "No symbol '" << نماد_Name << "' in " << مقصد_Printable_مسیر(fname));
    }
    أرجع res;
}

std::string DynamicLib::getName() ثابت
{
    أرجع مقصد_Printable_مسیر(fname);
}

فراغ DynamicLib::library_بارگیری(ثابت File_System_مسیر_t& filename)
{
    handle = library_بارگیری_2(filename);
    CV_LOG_INFO(عدم, "load " << مقصد_Printable_مسیر(filename) << " => " << (handle ? "OK" : "FAILED"));
}

فراغ DynamicLib::libraryRelease()
{
    إذا (handle)
    {
        CV_LOG_INFO(عدم, "unload "<< مقصد_Printable_مسیر(fname));
        libraryRelease_(handle);
        handle = 0;
    }
}

}}}  // namespace
