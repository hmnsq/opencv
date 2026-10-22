// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

//
// Not a standalone header, part of filesystem.cpp
//

#समावेश "opencv2/core/utils/plugin_loader.private.hpp"

#अगर !OPENCV_HAVE_FILESYSTEM_SUPPORT
#त्रुटि "Invalid build configuration"
#पूर्ण_अगर

#अगर 0  // TODO
#अगर_घोषित न_संशोधन
#घोषणा CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_DEBUG + 1
#अन्यथा
#घोषणा CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_VERBOSE + 1
#पूर्ण_अगर
#समावेश <opencv2/core/utils/logger.hpp>
#पूर्ण_अगर

namespace cv { namespace plugin { namespace impl {

DynamicLib::DynamicLib(स्थिर FileSystemPath_t& filename)
    : handle(0), fname(filename), disableAutoUnloading_(false)
{
    libraryLoad(filename);
}

DynamicLib::~DynamicLib()
{
    अगर (!disableAutoUnloading_)
    {
        libraryRelease();
    }
    अन्यथा अगर (handle)
    {
        CV_LOG_INFO(शून्य, "skip auto unloading (disabled): " << toPrintablePath(fname));
        handle = 0;
    }
}

व्योम* DynamicLib::get_प्रतीक(स्थिर अक्षर* प्रतीक_Name) स्थिर
{
    अगर (!handle)
    {
        वापस 0;
    }
    व्योम* res = get_प्रतीक_2(handle, प्रतीक_Name);
    अगर (!res)
    {
        CV_LOG_DEBUG(शून्य, "No symbol '" << प्रतीक_Name << "' in " << toPrintablePath(fname));
    }
    वापस res;
}

std::string DynamicLib::getName() स्थिर
{
    वापस toPrintablePath(fname);
}

व्योम DynamicLib::libraryLoad(स्थिर FileSystemPath_t& filename)
{
    handle = libraryLoad_(filename);
    CV_LOG_INFO(शून्य, "load " << toPrintablePath(filename) << " => " << (handle ? "OK" : "FAILED"));
}

व्योम DynamicLib::libraryRelease()
{
    अगर (handle)
    {
        CV_LOG_INFO(शून्य, "unload "<< toPrintablePath(fname));
        libraryRelease_(handle);
        handle = 0;
    }
}

}}}  // namespace
