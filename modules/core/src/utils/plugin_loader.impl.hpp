// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

//
// Not a standalone header, part of filesystem.cpp
//

#अन्तर्भाव "opencv2/core/utils/plugin_loader.private.hpp"

#यदि !OPENCV_HAVE_FILESYSTEM_SUPPORT
#दोषः "Invalid build configuration"
#यदि_अन्तः

#यदि 0  // TODO
#यदि_परिभाषितम् NDEBUG
#परिभाषय CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_DEBUG + 1
#अन्यथा
#परिभाषय CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_VERBOSE + 1
#यदि_अन्तः
#अन्तर्भाव <opencv2/core/utils/logger.hpp>
#यदि_अन्तः

नामाकाश cv { नामाकाश plugin { नामाकाश impl {

DynamicLib::DynamicLib(ध्रुव File_System_मार्गः_t& filename)
    : handle(0), fname(filename), disableAutoUnloading_(असत्य)
{
    library_आरोपय(filename);
}

DynamicLib::~DynamicLib()
{
    यदि (!disableAutoUnloading_)
    {
        libraryRelease();
    }
    अन्यथा यदि (handle)
    {
        CV_LOG_INFO(रिक्त, "skip auto unloading (disabled): " << गन्तव्यम्_Printable_मार्गः(fname));
        handle = 0;
    }
}

शून्य* DynamicLib::get_चिह्नम्(ध्रुव वर्ण* चिह्नम्_Name) ध्रुव
{
    यदि (!handle)
    {
        प्रत्यागम 0;
    }
    शून्य* res = get_चिह्नम्_2(handle, चिह्नम्_Name);
    यदि (!res)
    {
        CV_LOG_DEBUG(रिक्त, "No symbol '" << चिह्नम्_Name << "' in " << गन्तव्यम्_Printable_मार्गः(fname));
    }
    प्रत्यागम res;
}

std::string DynamicLib::getName() ध्रुव
{
    प्रत्यागम गन्तव्यम्_Printable_मार्गः(fname);
}

शून्य DynamicLib::library_आरोपय(ध्रुव File_System_मार्गः_t& filename)
{
    handle = library_आरोपय_2(filename);
    CV_LOG_INFO(रिक्त, "load " << गन्तव्यम्_Printable_मार्गः(filename) << " => " << (handle ? "OK" : "FAILED"));
}

शून्य DynamicLib::libraryRelease()
{
    यदि (handle)
    {
        CV_LOG_INFO(रिक्त, "unload "<< गन्तव्यम्_Printable_मार्गः(fname));
        libraryRelease_(handle);
        handle = 0;
    }
}

}}}  // namespace
