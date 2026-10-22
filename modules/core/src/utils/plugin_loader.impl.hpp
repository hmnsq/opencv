// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

//
// Not a standalone header, part of filesystem.cpp
//

#அந்தர்பாவ "opencv2/core/utils/plugin_loader.private.hpp"

#எனில் !OPENCV_HAVE_FILESYSTEM_SUPPORT
#error "Invalid build configuration"
#endif

#எனில் 0  // TODO
#ifdef NDEBUG
#define CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_DEBUG + 1
#இல்லையெனில்
#define CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_VERBOSE + 1
#endif
#அந்தர்பாவ <opencv2/core/utils/logger.hpp>
#endif

நாமாகாஶ cv { நாமாகாஶ plugin { நாமாகாஶ impl {

DynamicLib::DynamicLib(மாறிலி FileSystemPath_t& filename)
    : handle(0), fname(filename), disableAutoUnloading_(பொய்)
{
    libraryLoad(filename);
}

DynamicLib::~DynamicLib()
{
    எனில் (!disableAutoUnloading_)
    {
        libraryRelease();
    }
    இல்லையெனில் எனில் (handle)
    {
        CV_LOG_INFO(வெற்று, "skip auto unloading (disabled): " << toPrintablePath(fname));
        handle = 0;
    }
}

வெறுமை* DynamicLib::get_குறியீடு(மாறிலி எழுத்து* குறியீடு_Name) மாறிலி
{
    எனில் (!handle)
    {
        திரும்பு 0;
    }
    வெறுமை* res = get_குறியீடு_2(handle, குறியீடு_Name);
    எனில் (!res)
    {
        CV_LOG_DEBUG(வெற்று, "No symbol '" << குறியீடு_Name << "' in " << toPrintablePath(fname));
    }
    திரும்பு res;
}

std::string DynamicLib::getName() மாறிலி
{
    திரும்பு toPrintablePath(fname);
}

வெறுமை DynamicLib::libraryLoad(மாறிலி FileSystemPath_t& filename)
{
    handle = libraryLoad_(filename);
    CV_LOG_INFO(வெற்று, "load " << toPrintablePath(filename) << " => " << (handle ? "OK" : "FAILED"));
}

வெறுமை DynamicLib::libraryRelease()
{
    எனில் (handle)
    {
        CV_LOG_INFO(வெற்று, "unload "<< toPrintablePath(fname));
        libraryRelease_(handle);
        handle = 0;
    }
}

}}}  // namespace
