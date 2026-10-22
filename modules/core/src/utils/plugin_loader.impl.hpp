// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

//
// Not a standalone header, part of filesystem.cpp
//

#ಅನ್ತರ್ಭಾವ "opencv2/core/utils/plugin_loader.private.hpp"

#ಆದರೆ !OPENCV_HAVE_FILESYSTEM_SUPPORT
#ದೋಷ "Invalid build configuration"
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ 0  // TODO
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ NDEBUG
#ವ್ಯಾಖ್ಯಾನಿಸು CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_DEBUG + 1
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_VERBOSE + 1
#ಆದರೆ_ಅಂತ್ಯ
#ಅನ್ತರ್ಭಾವ <opencv2/core/utils/logger.hpp>
#ಆದರೆ_ಅಂತ್ಯ

ನಾಮಾಕಾಶ cv { ನಾಮಾಕಾಶ plugin { ನಾಮಾಕಾಶ impl {

DynamicLib::DynamicLib(ಸ್ಥಿರಾಂಕ File_System_ಮಾರ್ಗ_t& filename)
    : handle(0), fname(filename), disableAutoUnloading_(ಸುಳ್ಳು)
{
    library_ತುಂಬು(filename);
}

DynamicLib::~DynamicLib()
{
    ಆದರೆ (!disableAutoUnloading_)
    {
        libraryRelease();
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (handle)
    {
        CV_LOG_INFO(ಖಾಲಿ, "skip auto unloading (disabled): " << ಗಮ್ಯ_Printable_ಮಾರ್ಗ(fname));
        handle = 0;
    }
}

ಶೂನ್ಯ* DynamicLib::get_ಚಿಹ್ನೆ(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* ಚಿಹ್ನೆ_Name) ಸ್ಥಿರಾಂಕ
{
    ಆದರೆ (!handle)
    {
        ಹಿಂತಿರುಗಿ 0;
    }
    ಶೂನ್ಯ* res = get_ಚಿಹ್ನೆ_2(handle, ಚಿಹ್ನೆ_Name);
    ಆದರೆ (!res)
    {
        CV_LOG_DEBUG(ಖಾಲಿ, "No symbol '" << ಚಿಹ್ನೆ_Name << "' in " << ಗಮ್ಯ_Printable_ಮಾರ್ಗ(fname));
    }
    ಹಿಂತಿರುಗಿ res;
}

std::string DynamicLib::getName() ಸ್ಥಿರಾಂಕ
{
    ಹಿಂತಿರುಗಿ ಗಮ್ಯ_Printable_ಮಾರ್ಗ(fname);
}

ಶೂನ್ಯ DynamicLib::library_ತುಂಬು(ಸ್ಥಿರಾಂಕ File_System_ಮಾರ್ಗ_t& filename)
{
    handle = library_ತುಂಬು_2(filename);
    CV_LOG_INFO(ಖಾಲಿ, "load " << ಗಮ್ಯ_Printable_ಮಾರ್ಗ(filename) << " => " << (handle ? "OK" : "FAILED"));
}

ಶೂನ್ಯ DynamicLib::libraryRelease()
{
    ಆದರೆ (handle)
    {
        CV_LOG_INFO(ಖಾಲಿ, "unload "<< ಗಮ್ಯ_Printable_ಮಾರ್ಗ(fname));
        libraryRelease_(handle);
        handle = 0;
    }
}

}}}  // namespace
