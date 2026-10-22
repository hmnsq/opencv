// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

//
// Not a standalone header, part of filesystem.cpp
//

#അന്തര്ഭാവ "opencv2/core/utils/plugin_loader.private.hpp"

#എങ്കിൽ !OPENCV_HAVE_FILESYSTEM_SUPPORT
#error "Invalid build configuration"
#endif

#എങ്കിൽ 0  // TODO
#ifdef NDEBUG
#define CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_DEBUG + 1
#അല്ലെങ്കിൽ
#define CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_VERBOSE + 1
#endif
#അന്തര്ഭാവ <opencv2/core/utils/logger.hpp>
#endif

നാമാകാശ cv { നാമാകാശ plugin { നാമാകാശ impl {

DynamicLib::DynamicLib(സ്ഥിരം FileSystemPath_t& filename)
    : handle(0), fname(filename), disableAutoUnloading_(അസത്യം)
{
    libraryLoad(filename);
}

DynamicLib::~DynamicLib()
{
    എങ്കിൽ (!disableAutoUnloading_)
    {
        libraryRelease();
    }
    അല്ലെങ്കിൽ എങ്കിൽ (handle)
    {
        CV_LOG_INFO(ശൂന്യ, "skip auto unloading (disabled): " << toPrintablePath(fname));
        handle = 0;
    }
}

ശൂന്യം* DynamicLib::get_ചിഹ്നം(സ്ഥിരം അക്ഷരം* ചിഹ്നം_Name) സ്ഥിരം
{
    എങ്കിൽ (!handle)
    {
        തിരികെ 0;
    }
    ശൂന്യം* res = get_ചിഹ്നം_2(handle, ചിഹ്നം_Name);
    എങ്കിൽ (!res)
    {
        CV_LOG_DEBUG(ശൂന്യ, "No symbol '" << ചിഹ്നം_Name << "' in " << toPrintablePath(fname));
    }
    തിരികെ res;
}

std::string DynamicLib::getName() സ്ഥിരം
{
    തിരികെ toPrintablePath(fname);
}

ശൂന്യം DynamicLib::libraryLoad(സ്ഥിരം FileSystemPath_t& filename)
{
    handle = libraryLoad_(filename);
    CV_LOG_INFO(ശൂന്യ, "load " << toPrintablePath(filename) << " => " << (handle ? "OK" : "FAILED"));
}

ശൂന്യം DynamicLib::libraryRelease()
{
    എങ്കിൽ (handle)
    {
        CV_LOG_INFO(ശൂന്യ, "unload "<< toPrintablePath(fname));
        libraryRelease_(handle);
        handle = 0;
    }
}

}}}  // namespace
