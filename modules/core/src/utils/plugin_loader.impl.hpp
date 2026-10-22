// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

//
// Not a standalone header, part of filesystem.cpp
//

#अन्तर्भाव "opencv2/core/utils/plugin_loader.private.hpp"

#जर !OPENCV_HAVE_FILESYSTEM_SUPPORT
#error "Invalid build configuration"
#endif

#जर 0  // TODO
#ifdef न_संशोधन
#define CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_DEBUG + 1
#अन्यथा
#define CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_VERBOSE + 1
#endif
#अन्तर्भाव <opencv2/core/utils/logger.hpp>
#endif

नामाकाश cv { नामाकाश plugin { नामाकाश impl {

DynamicLib::DynamicLib(स्थिरांक FileSystemPath_t& filename)
    : handle(0), fname(filename), disableAutoUnloading_(असत्य)
{
    libraryLoad(filename);
}

DynamicLib::~DynamicLib()
{
    जर (!disableAutoUnloading_)
    {
        libraryRelease();
    }
    अन्यथा जर (handle)
    {
        CV_LOG_INFO(रिक्त, "skip auto unloading (disabled): " << toPrintablePath(fname));
        handle = 0;
    }
}

शून्य* DynamicLib::get_चिन्ह(स्थिरांक अक्षर* चिन्ह_Name) स्थिरांक
{
    जर (!handle)
    {
        परत 0;
    }
    शून्य* res = get_चिन्ह_2(handle, चिन्ह_Name);
    जर (!res)
    {
        CV_LOG_DEBUG(रिक्त, "No symbol '" << चिन्ह_Name << "' in " << toPrintablePath(fname));
    }
    परत res;
}

std::string DynamicLib::getName() स्थिरांक
{
    परत toPrintablePath(fname);
}

शून्य DynamicLib::libraryLoad(स्थिरांक FileSystemPath_t& filename)
{
    handle = libraryLoad_(filename);
    CV_LOG_INFO(रिक्त, "load " << toPrintablePath(filename) << " => " << (handle ? "OK" : "FAILED"));
}

शून्य DynamicLib::libraryRelease()
{
    जर (handle)
    {
        CV_LOG_INFO(रिक्त, "unload "<< toPrintablePath(fname));
        libraryRelease_(handle);
        handle = 0;
    }
}

}}}  // namespace
