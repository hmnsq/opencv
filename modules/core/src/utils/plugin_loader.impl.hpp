// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

//
// Not a standalone header, part of filesystem.cpp
//

#અન્તર્ભાવ "opencv2/core/utils/plugin_loader.private.hpp"

#જો !OPENCV_HAVE_FILESYSTEM_SUPPORT
#error "Invalid build configuration"
#endif

#જો 0  // TODO
#ifdef NDEBUG
#define CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_DEBUG + 1
#નહીંતર
#define CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_VERBOSE + 1
#endif
#અન્તર્ભાવ <opencv2/core/utils/logger.hpp>
#endif

નામાકાશ cv { નામાકાશ plugin { નામાકાશ impl {

DynamicLib::DynamicLib(અચલ FileSystemPath_t& filename)
    : handle(0), fname(filename), disableAutoUnloading_(ખોટું)
{
    libraryLoad(filename);
}

DynamicLib::~DynamicLib()
{
    જો (!disableAutoUnloading_)
    {
        libraryRelease();
    }
    નહીંતર જો (handle)
    {
        CV_LOG_INFO(ખાલી, "skip auto unloading (disabled): " << toPrintablePath(fname));
        handle = 0;
    }
}

શૂન્ય* DynamicLib::get_પ્રતીક(અચલ અક્ષર* પ્રતીક_Name) અચલ
{
    જો (!handle)
    {
        પાછા 0;
    }
    શૂન્ય* res = get_પ્રતીક_2(handle, પ્રતીક_Name);
    જો (!res)
    {
        CV_LOG_DEBUG(ખાલી, "No symbol '" << પ્રતીક_Name << "' in " << toPrintablePath(fname));
    }
    પાછા res;
}

std::string DynamicLib::getName() અચલ
{
    પાછા toPrintablePath(fname);
}

શૂન્ય DynamicLib::libraryLoad(અચલ FileSystemPath_t& filename)
{
    handle = libraryLoad_(filename);
    CV_LOG_INFO(ખાલી, "load " << toPrintablePath(filename) << " => " << (handle ? "OK" : "FAILED"));
}

શૂન્ય DynamicLib::libraryRelease()
{
    જો (handle)
    {
        CV_LOG_INFO(ખાલી, "unload "<< toPrintablePath(fname));
        libraryRelease_(handle);
        handle = 0;
    }
}

}}}  // namespace
