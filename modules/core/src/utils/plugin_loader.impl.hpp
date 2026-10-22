// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

//
// Not a standalone header, part of filesystem.cpp
//

#ਅਨ੍ਤਰ੍ਭਾਵ "opencv2/core/utils/plugin_loader.private.hpp"

#ਜੇ !OPENCV_HAVE_FILESYSTEM_SUPPORT
#ਗਲਤੀ "Invalid build configuration"
#ਜੇ_ਅੰਤ

#ਜੇ 0  // TODO
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ NDEBUG
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_DEBUG + 1
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_VERBOSE + 1
#ਜੇ_ਅੰਤ
#ਅਨ੍ਤਰ੍ਭਾਵ <opencv2/core/utils/logger.hpp>
#ਜੇ_ਅੰਤ

ਨਾਮਾਕਾਸ਼ cv { ਨਾਮਾਕਾਸ਼ plugin { ਨਾਮਾਕਾਸ਼ impl {

DynamicLib::DynamicLib(ਅਚਲ File_System_ਰਸਤਾ_t& filename)
    : handle(0), fname(filename), disableAutoUnloading_(ਝੂਠ)
{
    library_ਲੱਦੋ(filename);
}

DynamicLib::~DynamicLib()
{
    ਜੇ (!disableAutoUnloading_)
    {
        libraryRelease();
    }
    ਨਹੀਂਤਾਂ ਜੇ (handle)
    {
        CV_LOG_INFO(ਨਿਰਾ, "skip auto unloading (disabled): " << ਮੰਜ਼ਿਲ_Printable_ਰਸਤਾ(fname));
        handle = 0;
    }
}

ਖਾਲੀ* DynamicLib::get_ਚਿੰਨ੍ਹ(ਅਚਲ ਅੱਖਰ* ਚਿੰਨ੍ਹ_Name) ਅਚਲ
{
    ਜੇ (!handle)
    {
        ਵਾਪਸ 0;
    }
    ਖਾਲੀ* res = get_ਚਿੰਨ੍ਹ_2(handle, ਚਿੰਨ੍ਹ_Name);
    ਜੇ (!res)
    {
        CV_LOG_DEBUG(ਨਿਰਾ, "No symbol '" << ਚਿੰਨ੍ਹ_Name << "' in " << ਮੰਜ਼ਿਲ_Printable_ਰਸਤਾ(fname));
    }
    ਵਾਪਸ res;
}

std::string DynamicLib::getName() ਅਚਲ
{
    ਵਾਪਸ ਮੰਜ਼ਿਲ_Printable_ਰਸਤਾ(fname);
}

ਖਾਲੀ DynamicLib::library_ਲੱਦੋ(ਅਚਲ File_System_ਰਸਤਾ_t& filename)
{
    handle = library_ਲੱਦੋ_2(filename);
    CV_LOG_INFO(ਨਿਰਾ, "load " << ਮੰਜ਼ਿਲ_Printable_ਰਸਤਾ(filename) << " => " << (handle ? "OK" : "FAILED"));
}

ਖਾਲੀ DynamicLib::libraryRelease()
{
    ਜੇ (handle)
    {
        CV_LOG_INFO(ਨਿਰਾ, "unload "<< ਮੰਜ਼ਿਲ_Printable_ਰਸਤਾ(fname));
        libraryRelease_(handle);
        handle = 0;
    }
}

}}}  // namespace
