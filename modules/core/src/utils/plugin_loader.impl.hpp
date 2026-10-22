// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

//
// Not a standalone header, part of filesystem.cpp
//

#अन्तर्भाव "opencv2/core/utils/plugin_loader.private.hpp"

#सचे !OPENCV_HAVE_FILESYSTEM_SUPPORT
#error "Invalid build configuration"
#endif

#सचे 0  // TODO
#ifdef NDEBUG
#define CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_DEBUG + 1
#अञ्ञथा
#define CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_VERBOSE + 1
#endif
#अन्तर्भाव <opencv2/core/utils/logger.hpp>
#endif

नामाकाश cv { नामाकाश plugin { नामाकाश impl {

DynamicLib::DynamicLib(ध्रुव FileSystemPath_t& filename)
    : handle(0), fname(filename), disableAutoUnloading_(मिच्छा)
{
    libraryLoad(filename);
}

DynamicLib::~DynamicLib()
{
    सचे (!disableAutoUnloading_)
    {
        libraryRelease();
    }
    अञ्ञथा सचे (handle)
    {
        CV_LOG_INFO(रिक्त, "skip auto unloading (disabled): " << toPrintablePath(fname));
        handle = 0;
    }
}

शून्य* DynamicLib::get_चिह्नम्(ध्रुव वर्ण* चिह्नम्_Name) ध्रुव
{
    सचे (!handle)
    {
        पटिनिवत्त 0;
    }
    शून्य* res = get_चिह्नम्_2(handle, चिह्नम्_Name);
    सचे (!res)
    {
        CV_LOG_DEBUG(रिक्त, "No symbol '" << चिह्नम्_Name << "' in " << toPrintablePath(fname));
    }
    पटिनिवत्त res;
}

std::string DynamicLib::getName() ध्रुव
{
    पटिनिवत्त toPrintablePath(fname);
}

शून्य DynamicLib::libraryLoad(ध्रुव FileSystemPath_t& filename)
{
    handle = libraryLoad_(filename);
    CV_LOG_INFO(रिक्त, "load " << toPrintablePath(filename) << " => " << (handle ? "OK" : "FAILED"));
}

शून्य DynamicLib::libraryRelease()
{
    सचे (handle)
    {
        CV_LOG_INFO(रिक्त, "unload "<< toPrintablePath(fname));
        libraryRelease_(handle);
        handle = 0;
    }
}

}}}  // namespace
