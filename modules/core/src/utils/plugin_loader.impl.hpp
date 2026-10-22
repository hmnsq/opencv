// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

//
// Not a standalone header, part of filesystem.cpp
//

#شامل "opencv2/core/utils/plugin_loader.private.hpp"

#اگر !OPENCV_HAVE_FILESYSTEM_SUPPORT
#غلطی "Invalid build configuration"
#اگر_ختم

#اگر 0  // TODO
#اگر_تعریف_شدہ NDEBUG
#تعریف_کرو CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_DEBUG + 1
#ورنہ
#تعریف_کرو CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_VERBOSE + 1
#اگر_ختم
#شامل <opencv2/core/utils/logger.hpp>
#اگر_ختم

نامخلا cv { نامخلا plugin { نامخلا impl {

DynamicLib::DynamicLib(ثابت File_System_راستہ_t& filename)
    : handle(0), fname(filename), disableAutoUnloading_(جھوٹ)
{
    library_بھرو(filename);
}

DynamicLib::~DynamicLib()
{
    اگر (!disableAutoUnloading_)
    {
        libraryRelease();
    }
    ورنہ اگر (handle)
    {
        CV_LOG_INFO(تہی, "skip auto unloading (disabled): " << مقصد_Printable_راستہ(fname));
        handle = 0;
    }
}

خالی* DynamicLib::get_علامت(ثابت حرف* علامت_Name) ثابت
{
    اگر (!handle)
    {
        واپس 0;
    }
    خالی* res = get_علامت_2(handle, علامت_Name);
    اگر (!res)
    {
        CV_LOG_DEBUG(تہی, "No symbol '" << علامت_Name << "' in " << مقصد_Printable_راستہ(fname));
    }
    واپس res;
}

std::string DynamicLib::getName() ثابت
{
    واپس مقصد_Printable_راستہ(fname);
}

خالی DynamicLib::library_بھرو(ثابت File_System_راستہ_t& filename)
{
    handle = library_بھرو_2(filename);
    CV_LOG_INFO(تہی, "load " << مقصد_Printable_راستہ(filename) << " => " << (handle ? "OK" : "FAILED"));
}

خالی DynamicLib::libraryRelease()
{
    اگر (handle)
    {
        CV_LOG_INFO(تہی, "unload "<< مقصد_Printable_راستہ(fname));
        libraryRelease_(handle);
        handle = 0;
    }
}

}}}  // namespace
