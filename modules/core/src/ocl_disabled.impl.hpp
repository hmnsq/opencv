// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ಅನ್ತರ್ಭಾವ "opencv2/core/ocl_genbase.hpp"

#ಆದರೆ defined(_MSC_VER)
    #ಸೂಚನೆ warning(push)
    #ಸೂಚನೆ warning(disable : 4100)
    #ಸೂಚನೆ warning(disable : 4702)
#ಅಥವಾಆದರೆ defined(__clang__)
    #ಸೂಚನೆ clang diagnostic push
    #ಸೂಚನೆ clang diagnostic ignored "-Wunused-parameter"
#ಅಥವಾಆದರೆ defined(__GNUC__)
    #ಸೂಚನೆ GCC diagnostic push
    #ಸೂಚನೆ GCC diagnostic ignored "-Wunused-parameter"
#ಆದರೆ_ಅಂತ್ಯ

ನಾಮಾಕಾಶ cv { ನಾಮಾಕಾಶ ocl {

ಸ್ಥಿರ
CV_NORETURN ಶೂನ್ಯ throw_no_ocl()
{
    CV_Error(Error::OpenCLApiCallError, "OpenCV build without OpenCL support");
}
#ವ್ಯಾಖ್ಯಾನಿಸು OCL_NOT_ಲಭ್ಯ() throw_no_ocl();

CV_EXPORTS_W ತಾರ್ಕಿಕ haveOpenCL() { ಹಿಂತಿರುಗಿ ಸುಳ್ಳು; }
CV_EXPORTS_W ತಾರ್ಕಿಕ useOpenCL() { ಹಿಂತಿರುಗಿ ಸುಳ್ಳು; }
CV_EXPORTS_W ತಾರ್ಕಿಕ haveAmdBlas() { ಹಿಂತಿರುಗಿ ಸುಳ್ಳು; }
CV_EXPORTS_W ತಾರ್ಕಿಕ haveAmdFft() { ಹಿಂತಿರುಗಿ ಸುಳ್ಳು; }
CV_EXPORTS_W ಶೂನ್ಯ ಗಣ_Use_Open_CL(ತಾರ್ಕಿಕ flag) { /* nothing */ }
CV_EXPORTS_W ಶೂನ್ಯ ಮುಕ್ತಾಯ_2() { /* nothing */ }

CV_EXPORTS ತಾರ್ಕಿಕ haveSVM() { ಹಿಂತಿರುಗಿ ಸುಳ್ಳು; }

Device::Device() CV_NOEXCEPT : p(ಖಾಲಿ) { }
Device::Device(ಶೂನ್ಯ* d) : p(ಖಾಲಿ) { OCL_NOT_ಲಭ್ಯ(); }
Device::Device(ಸ್ಥಿರಾಂಕ Device& d) : p(ಖಾಲಿ) { }
Device& Device::ಕ್ರಿಯಾಕಾರಕ=(ಸ್ಥಿರಾಂಕ Device& d) { ಹಿಂತಿರುಗಿ *ಇದು; }
Device::Device(Device&&) CV_NOEXCEPT : p(ಖಾಲಿ) { }
Device& Device::ಕ್ರಿಯಾಕಾರಕ=(Device&&) CV_NOEXCEPT { ಹಿಂತಿರುಗಿ *ಇದು; }
Device::~Device() { }

ಶೂನ್ಯ Device::ಗಣ(ಶೂನ್ಯ* d) { OCL_NOT_ಲಭ್ಯ(); }

String Device::name() ಸ್ಥಿರಾಂಕ { OCL_NOT_ಲಭ್ಯ(); }
String Device::extensions() ಸ್ಥಿರಾಂಕ { OCL_NOT_ಲಭ್ಯ(); }
ತಾರ್ಕಿಕ Device::isExtensionSupported(ಸ್ಥಿರಾಂಕ String& extensionName) ಸ್ಥಿರಾಂಕ { OCL_NOT_ಲಭ್ಯ(); }
String Device::ಆವೃತ್ತಿ() ಸ್ಥಿರಾಂಕ { OCL_NOT_ಲಭ್ಯ(); }
String Device::vendorName() ಸ್ಥಿರಾಂಕ { OCL_NOT_ಲಭ್ಯ(); }
String Device::Open_CL_C_ಆವೃತ್ತಿ() ಸ್ಥಿರಾಂಕ { OCL_NOT_ಲಭ್ಯ(); }
String Device::OpenCLVersion() ಸ್ಥಿರಾಂಕ { OCL_NOT_ಲಭ್ಯ(); }
ಪೂರ್ಣಾಂಕ Device::device_ಆವೃತ್ತಿ_Major() ಸ್ಥಿರಾಂಕ { OCL_NOT_ಲಭ್ಯ(); }
ಪೂರ್ಣಾಂಕ Device::device_ಆವೃತ್ತಿ_Minor() ಸ್ಥಿರಾಂಕ { OCL_NOT_ಲಭ್ಯ(); }
String Device::driver_ಆವೃತ್ತಿ() ಸ್ಥಿರಾಂಕ { OCL_NOT_ಲಭ್ಯ(); }
ಶೂನ್ಯ* Device::ptr() ಸ್ಥಿರಾಂಕ { /*OCL_NOT_AVAILABLE();*/ ಹಿಂತಿರುಗಿ ಖಾಲಿ; }

ಪೂರ್ಣಾಂಕ Device::type() ಸ್ಥಿರಾಂಕ { OCL_NOT_ಲಭ್ಯ(); }

ಪೂರ್ಣಾಂಕ Device::ವಿಳಾಸ_ಬಿಟ್ಟುಗಳು() ಸ್ಥಿರಾಂಕ { OCL_NOT_ಲಭ್ಯ(); }
ತಾರ್ಕಿಕ Device::ಲಭ್ಯ() ಸ್ಥಿರಾಂಕ { OCL_NOT_ಲಭ್ಯ(); }
ತಾರ್ಕಿಕ Device::compiler_ಲಭ್ಯ() ಸ್ಥಿರಾಂಕ { OCL_NOT_ಲಭ್ಯ(); }
ತಾರ್ಕಿಕ Device::linker_ಲಭ್ಯ() ಸ್ಥಿರಾಂಕ { OCL_NOT_ಲಭ್ಯ(); }

ಪೂರ್ಣಾಂಕ Device::doubleFPConfig() ಸ್ಥಿರಾಂಕ { OCL_NOT_ಲಭ್ಯ(); }
ಪೂರ್ಣಾಂಕ Device::singleFPConfig() ಸ್ಥಿರಾಂಕ { OCL_NOT_ಲಭ್ಯ(); }
ಪೂರ್ಣಾಂಕ Device::halfFPConfig() ಸ್ಥಿರಾಂಕ { OCL_NOT_ಲಭ್ಯ(); }

ತಾರ್ಕಿಕ Device::hasFP64() ಸ್ಥಿರಾಂಕ { OCL_NOT_ಲಭ್ಯ(); }
ತಾರ್ಕಿಕ Device::hasFP16() ಸ್ಥಿರಾಂಕ { OCL_NOT_ಲಭ್ಯ(); }

ತಾರ್ಕಿಕ Device::endianLittle() ಸ್ಥಿರಾಂಕ { OCL_NOT_ಲಭ್ಯ(); }
ತಾರ್ಕಿಕ Device::errorCorrectionSupport() ಸ್ಥಿರಾಂಕ { OCL_NOT_ಲಭ್ಯ(); }

ಪೂರ್ಣಾಂಕ Device::executionCapabilities() ಸ್ಥಿರಾಂಕ { OCL_NOT_ಲಭ್ಯ(); }

size_t Device::global_Mem_Cache_ಗಾತ್ರ() ಸ್ಥಿರಾಂಕ { OCL_NOT_ಲಭ್ಯ(); }

ಪೂರ್ಣಾಂಕ Device::globalMemCacheType() ಸ್ಥಿರಾಂಕ { OCL_NOT_ಲಭ್ಯ(); }
ಪೂರ್ಣಾಂಕ Device::global_Mem_Cache_ಸಾಲು_ಗಾತ್ರ() ಸ್ಥಿರಾಂಕ { OCL_NOT_ಲಭ್ಯ(); }
size_t Device::global_Mem_ಗಾತ್ರ() ಸ್ಥಿರಾಂಕ { OCL_NOT_ಲಭ್ಯ(); }

size_t Device::local_Mem_ಗಾತ್ರ() ಸ್ಥಿರಾಂಕ { OCL_NOT_ಲಭ್ಯ(); }
ಪೂರ್ಣಾಂಕ Device::localMemType() ಸ್ಥಿರಾಂಕ { ಹಿಂತಿರುಗಿ NO_LOCAL_MEM; }
ತಾರ್ಕಿಕ Device::host_Unified_ಸ್ಮರಣೆ_2() ಸ್ಥಿರಾಂಕ { OCL_NOT_ಲಭ್ಯ(); }

ತಾರ್ಕಿಕ Device::imageSupport() ಸ್ಥಿರಾಂಕ { OCL_NOT_ಲಭ್ಯ(); }

ತಾರ್ಕಿಕ Device::image_From_ಬಫರ್_Support() ಸ್ಥಿರಾಂಕ { OCL_NOT_ಲಭ್ಯ(); }
uint Device::imagePitchAlignment() ಸ್ಥಿರಾಂಕ { OCL_NOT_ಲಭ್ಯ(); }
uint Device::image_ತಳ_ವಿಳಾಸ_Alignment() ಸ್ಥಿರಾಂಕ { OCL_NOT_ಲಭ್ಯ(); }

ತಾರ್ಕಿಕ Device::intelSubgroupsSupport() ಸ್ಥಿರಾಂಕ { OCL_NOT_ಲಭ್ಯ(); }

size_t Device::image2_DMax_ಅಗಲ() ಸ್ಥಿರಾಂಕ { OCL_NOT_ಲಭ್ಯ(); }
size_t Device::image2_DMax_ಎತ್ತರ() ಸ್ಥಿರಾಂಕ { OCL_NOT_ಲಭ್ಯ(); }

size_t Device::image3_DMax_ಅಗಲ() ಸ್ಥಿರಾಂಕ { OCL_NOT_ಲಭ್ಯ(); }
size_t Device::image3_DMax_ಎತ್ತರ() ಸ್ಥಿರಾಂಕ { OCL_NOT_ಲಭ್ಯ(); }
size_t Device::image3_DMax_ಆಳ() ಸ್ಥಿರಾಂಕ { OCL_NOT_ಲಭ್ಯ(); }

size_t Device::image_Max_ಬಫರ್_ಗಾತ್ರ() ಸ್ಥಿರಾಂಕ { OCL_NOT_ಲಭ್ಯ(); }
size_t Device::image_Max_Array_ಗಾತ್ರ() ಸ್ಥಿರಾಂಕ { OCL_NOT_ಲಭ್ಯ(); }

ಪೂರ್ಣಾಂಕ Device::vendorID() ಸ್ಥಿರಾಂಕ { OCL_NOT_ಲಭ್ಯ(); }

ಪೂರ್ಣಾಂಕ Device::maxClockFrequency() ಸ್ಥಿರಾಂಕ { OCL_NOT_ಲಭ್ಯ(); }
ಪೂರ್ಣಾಂಕ Device::maxComputeUnits() ಸ್ಥಿರಾಂಕ { OCL_NOT_ಲಭ್ಯ(); }
ಪೂರ್ಣಾಂಕ Device::max_Constant_ವಾದಗಳು() ಸ್ಥಿರಾಂಕ { OCL_NOT_ಲಭ್ಯ(); }
size_t Device::max_Constant_ಬಫರ್_ಗಾತ್ರ() ಸ್ಥಿರಾಂಕ { OCL_NOT_ಲಭ್ಯ(); }

size_t Device::max_Mem_Alloc_ಗಾತ್ರ() ಸ್ಥಿರಾಂಕ { OCL_NOT_ಲಭ್ಯ(); }
size_t Device::max_Parameter_ಗಾತ್ರ() ಸ್ಥಿರಾಂಕ { OCL_NOT_ಲಭ್ಯ(); }

ಪೂರ್ಣಾಂಕ Device::max_Read_Image_ವಾದಗಳು() ಸ್ಥಿರಾಂಕ { OCL_NOT_ಲಭ್ಯ(); }
ಪೂರ್ಣಾಂಕ Device::max_Write_Image_ವಾದಗಳು() ಸ್ಥಿರಾಂಕ { OCL_NOT_ಲಭ್ಯ(); }
ಪೂರ್ಣಾಂಕ Device::maxSamplers() ಸ್ಥಿರಾಂಕ { OCL_NOT_ಲಭ್ಯ(); }

size_t Device::max_Work_ಗುಂಪು_ಗಾತ್ರ() ಸ್ಥಿರಾಂಕ { OCL_NOT_ಲಭ್ಯ(); }
ಪೂರ್ಣಾಂಕ Device::max_Work_ಅಂಶ_Dims() ಸ್ಥಿರಾಂಕ { OCL_NOT_ಲಭ್ಯ(); }
ಶೂನ್ಯ Device::max_Work_ಅಂಶ_Sizes(size_t*) ಸ್ಥಿರಾಂಕ { OCL_NOT_ಲಭ್ಯ(); }

ಪೂರ್ಣಾಂಕ Device::mem_ತಳ_Addr_Align() ಸ್ಥಿರಾಂಕ { OCL_NOT_ಲಭ್ಯ(); }

ಪೂರ್ಣಾಂಕ Device::native_ಸದಿಶ_ಅಗಲ_Char() ಸ್ಥಿರಾಂಕ { OCL_NOT_ಲಭ್ಯ(); }
ಪೂರ್ಣಾಂಕ Device::native_ಸದಿಶ_ಅಗಲ_Short() ಸ್ಥಿರಾಂಕ { OCL_NOT_ಲಭ್ಯ(); }
ಪೂರ್ಣಾಂಕ Device::native_ಸದಿಶ_ಅಗಲ_Int() ಸ್ಥಿರಾಂಕ { OCL_NOT_ಲಭ್ಯ(); }
ಪೂರ್ಣಾಂಕ Device::native_ಸದಿಶ_ಅಗಲ_Long() ಸ್ಥಿರಾಂಕ { OCL_NOT_ಲಭ್ಯ(); }
ಪೂರ್ಣಾಂಕ Device::native_ಸದಿಶ_ಅಗಲ_Float() ಸ್ಥಿರಾಂಕ { OCL_NOT_ಲಭ್ಯ(); }
ಪೂರ್ಣಾಂಕ Device::native_ಸದಿಶ_ಅಗಲ_Double() ಸ್ಥಿರಾಂಕ { OCL_NOT_ಲಭ್ಯ(); }
ಪೂರ್ಣಾಂಕ Device::native_ಸದಿಶ_ಅಗಲ_Half() ಸ್ಥಿರಾಂಕ { OCL_NOT_ಲಭ್ಯ(); }

ಪೂರ್ಣಾಂಕ Device::preferred_ಸದಿಶ_ಅಗಲ_Char() ಸ್ಥಿರಾಂಕ { OCL_NOT_ಲಭ್ಯ(); }
ಪೂರ್ಣಾಂಕ Device::preferred_ಸದಿಶ_ಅಗಲ_Short() ಸ್ಥಿರಾಂಕ { OCL_NOT_ಲಭ್ಯ(); }
ಪೂರ್ಣಾಂಕ Device::preferred_ಸದಿಶ_ಅಗಲ_Int() ಸ್ಥಿರಾಂಕ { OCL_NOT_ಲಭ್ಯ(); }
ಪೂರ್ಣಾಂಕ Device::preferred_ಸದಿಶ_ಅಗಲ_Long() ಸ್ಥಿರಾಂಕ { OCL_NOT_ಲಭ್ಯ(); }
ಪೂರ್ಣಾಂಕ Device::preferred_ಸದಿಶ_ಅಗಲ_Float() ಸ್ಥಿರಾಂಕ { OCL_NOT_ಲಭ್ಯ(); }
ಪೂರ್ಣಾಂಕ Device::preferred_ಸದಿಶ_ಅಗಲ_Double() ಸ್ಥಿರಾಂಕ { OCL_NOT_ಲಭ್ಯ(); }
ಪೂರ್ಣಾಂಕ Device::preferred_ಸದಿಶ_ಅಗಲ_Half() ಸ್ಥಿರಾಂಕ { OCL_NOT_ಲಭ್ಯ(); }

size_t Device::printf_ಬಫರ್_ಗಾತ್ರ() ಸ್ಥಿರಾಂಕ { OCL_NOT_ಲಭ್ಯ(); }
size_t Device::profilingTimerResolution() ಸ್ಥಿರಾಂಕ { OCL_NOT_ಲಭ್ಯ(); }

/* static */
ಸ್ಥಿರಾಂಕ Device& Device::getDefault()
{
    ಸ್ಥಿರ Device dummy;
    ಹಿಂತಿರುಗಿ dummy;
}

/* static */ Device Device::fromHandle(ಶೂನ್ಯ* d) { OCL_NOT_ಲಭ್ಯ(); }


Context::Context() CV_NOEXCEPT : p(ಖಾಲಿ) { }
Context::Context(ಪೂರ್ಣಾಂಕ dtype) : p(ಖಾಲಿ) { }
Context::~Context() { }
Context::Context(ಸ್ಥಿರಾಂಕ Context& c) : p(ಖಾಲಿ) { }
Context& Context::ಕ್ರಿಯಾಕಾರಕ=(ಸ್ಥಿರಾಂಕ Context& c) { ಹಿಂತಿರುಗಿ *ಇದು; }
Context::Context(Context&&) CV_NOEXCEPT : p(ಖಾಲಿ) { }
Context& Context::ಕ್ರಿಯಾಕಾರಕ=(Context&&) CV_NOEXCEPT { ಹಿಂತಿರುಗಿ *ಇದು; }

ತಾರ್ಕಿಕ Context::create() { ಹಿಂತಿರುಗಿ ಸುಳ್ಳು; }
ತಾರ್ಕಿಕ Context::create(ಪೂರ್ಣಾಂಕ dtype) { ಹಿಂತಿರುಗಿ ಸುಳ್ಳು; }
size_t Context::ndevices() ಸ್ಥಿರಾಂಕ { ಹಿಂತಿರುಗಿ 0; }
Device& Context::device(size_t idx) ಸ್ಥಿರಾಂಕ { OCL_NOT_ಲಭ್ಯ(); }
ಕಾರ್ಯಕ್ರಮ_2 Context::getProg(ಸ್ಥಿರಾಂಕ ಕಾರ್ಯಕ್ರಮ_ಮೂಲ& prog, ಸ್ಥಿರಾಂಕ String& buildopt, String& errmsg) { OCL_NOT_ಲಭ್ಯ(); }
ಶೂನ್ಯ Context::unloadProg(ಕಾರ್ಯಕ್ರಮ_2& prog) { }

/* static */
Context& Context::getDefault()
{
    ಸ್ಥಿರ Context dummy;
    ಹಿಂತಿರುಗಿ dummy;
}
ಶೂನ್ಯ* Context::ptr() ಸ್ಥಿರಾಂಕ { ಹಿಂತಿರುಗಿ ಖಾಲಿ; }

ಶೂನ್ಯ* Context::getOpenCLContextProperty(ಪೂರ್ಣಾಂಕ /*propertyId*/) ಸ್ಥಿರಾಂಕ { OCL_NOT_ಲಭ್ಯ(); }

ತಾರ್ಕಿಕ Context::useSVM() ಸ್ಥಿರಾಂಕ { ಹಿಂತಿರುಗಿ ಸುಳ್ಳು; }
ಶೂನ್ಯ Context::ಗಣ_Use_SVM(ತಾರ್ಕಿಕ enabled) { }

Context::UserContext::~UserContext() { }

ಶೂನ್ಯ Context::ಗಣ_User_Context(std::type_ಸೂಚ್ಯಂಕ /*typeId*/, ಸ್ಥಿರಾಂಕ std::shared_ptr<Context::UserContext>& /*userContext*/) { OCL_NOT_ಲಭ್ಯ(); }
std::shared_ptr<Context::UserContext> Context::getUserContext(std::type_ಸೂಚ್ಯಂಕ /*typeId*/) { OCL_NOT_ಲಭ್ಯ(); }

/* static */ Context Context::fromHandle(ಶೂನ್ಯ* context) { OCL_NOT_ಲಭ್ಯ(); }
/* static */ Context Context::fromDevice(ಸ್ಥಿರಾಂಕ ocl::Device& device) { OCL_NOT_ಲಭ್ಯ(); }
/* static */ Context Context::create(ಸ್ಥಿರಾಂಕ std::string& configuration) { OCL_NOT_ಲಭ್ಯ(); }

ಶೂನ್ಯ Context::release() { }


Platform::Platform() CV_NOEXCEPT : p(ಖಾಲಿ) { }
Platform::~Platform() { }
Platform::Platform(ಸ್ಥಿರಾಂಕ Platform&) : p(ಖಾಲಿ) { }
Platform& Platform::ಕ್ರಿಯಾಕಾರಕ=(ಸ್ಥಿರಾಂಕ Platform&) { ಹಿಂತಿರುಗಿ *ಇದು; }
Platform::Platform(Platform&&) CV_NOEXCEPT : p(ಖಾಲಿ) { }
Platform& Platform::ಕ್ರಿಯಾಕಾರಕ=(Platform&&) CV_NOEXCEPT { ಹಿಂತಿರುಗಿ *ಇದು; }

ಶೂನ್ಯ* Platform::ptr() ಸ್ಥಿರಾಂಕ { ಹಿಂತಿರುಗಿ ಖಾಲಿ; }

/* static */
Platform& Platform::getDefault()
{
    ಸ್ಥಿರ Platform dummy;
    ಹಿಂತಿರುಗಿ dummy;
}

ಶೂನ್ಯ attachContext(ಸ್ಥಿರಾಂಕ String& platformName, ಶೂನ್ಯ* platformID, ಶೂನ್ಯ* context, ಶೂನ್ಯ* deviceID) { OCL_NOT_ಲಭ್ಯ(); }
ಶೂನ್ಯ convert_From_ಬಫರ್(ಶೂನ್ಯ* cl_mem_ಬಫರ್, size_t ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಡ್ಡಸಾಲುಗಳು, ಪೂರ್ಣಾಂಕ ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, ಪೂರ್ಣಾಂಕ type, UMat& dst) { OCL_NOT_ಲಭ್ಯ(); }
ಶೂನ್ಯ convertFromImage(ಶೂನ್ಯ* cl_mem_image, UMat& dst) { OCL_NOT_ಲಭ್ಯ(); }

ಶೂನ್ಯ initializeContextFromHandle(Context& ctx, ಶೂನ್ಯ* platform, ಶೂನ್ಯ* context, ಶೂನ್ಯ* device) { OCL_NOT_ಲಭ್ಯ(); }

ಸರತಿ::ಸರತಿ() CV_NOEXCEPT : p(ಖಾಲಿ) { }
ಸರತಿ::ಸರತಿ(ಸ್ಥಿರಾಂಕ Context& c, ಸ್ಥಿರಾಂಕ Device& d) : p(ಖಾಲಿ) { OCL_NOT_ಲಭ್ಯ(); }
ಸರತಿ::~ಸರತಿ() { }
ಸರತಿ::ಸರತಿ(ಸ್ಥಿರಾಂಕ ಸರತಿ& q) {}
ಸರತಿ& ಸರತಿ::ಕ್ರಿಯಾಕಾರಕ=(ಸ್ಥಿರಾಂಕ ಸರತಿ& q) { ಹಿಂತಿರುಗಿ *ಇದು; }
ಸರತಿ::ಸರತಿ(ಸರತಿ&&) CV_NOEXCEPT : p(ಖಾಲಿ) { }
ಸರತಿ& ಸರತಿ::ಕ್ರಿಯಾಕಾರಕ=(ಸರತಿ&&) CV_NOEXCEPT { ಹಿಂತಿರುಗಿ *ಇದು; }

ತಾರ್ಕಿಕ ಸರತಿ::create(ಸ್ಥಿರಾಂಕ Context& c, ಸ್ಥಿರಾಂಕ Device& d) { OCL_NOT_ಲಭ್ಯ(); }
ಶೂನ್ಯ ಸರತಿ::ಮುಕ್ತಾಯ_2() {}
ಶೂನ್ಯ* ಸರತಿ::ptr() ಸ್ಥಿರಾಂಕ { ಹಿಂತಿರುಗಿ ಖಾಲಿ; }
/* static */
ಸರತಿ& ಸರತಿ::getDefault()
{
    ಸ್ಥಿರ ಸರತಿ dummy;
    ಹಿಂತಿರುಗಿ dummy;
}

/// @brief Returns OpenCL command queue with enable profiling mode support
ಸ್ಥಿರಾಂಕ ಸರತಿ& ಸರತಿ::get_Profiling_ಸರತಿ() ಸ್ಥಿರಾಂಕ { OCL_NOT_ಲಭ್ಯ(); }


KernelArg::KernelArg() CV_NOEXCEPT
    : flags(0), m(0), obj(0), sz(0), wscale(1), iwscale(1)
{
}

KernelArg::KernelArg(ಪೂರ್ಣಾಂಕ _flags, UMat* _m, ಪೂರ್ಣಾಂಕ _wscale, ಪೂರ್ಣಾಂಕ _iwscale, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* _obj, size_t _sz)
    : flags(_flags), m(_m), obj(_obj), sz(_sz), wscale(_wscale), iwscale(_iwscale)
{
    OCL_NOT_ಲಭ್ಯ();
}

KernelArg KernelArg::Constant(ಸ್ಥಿರಾಂಕ Mat& m)
{
    OCL_NOT_ಲಭ್ಯ();
}


Kernel::Kernel() CV_NOEXCEPT : p(ಖಾಲಿ) { }
Kernel::Kernel(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* kname, ಸ್ಥಿರಾಂಕ ಕಾರ್ಯಕ್ರಮ_2& prog) : p(ಖಾಲಿ) { OCL_NOT_ಲಭ್ಯ(); }
Kernel::Kernel(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* kname, ಸ್ಥಿರಾಂಕ ಕಾರ್ಯಕ್ರಮ_ಮೂಲ& prog, ಸ್ಥಿರಾಂಕ String& buildopts, String* errmsg) : p(ಖಾಲಿ) { OCL_NOT_ಲಭ್ಯ(); }
Kernel::~Kernel() { }
Kernel::Kernel(ಸ್ಥಿರಾಂಕ Kernel& k) : p(ಖಾಲಿ) { }
Kernel& Kernel::ಕ್ರಿಯಾಕಾರಕ=(ಸ್ಥಿರಾಂಕ Kernel& k) { ಹಿಂತಿರುಗಿ *ಇದು; }
Kernel::Kernel(Kernel&&) CV_NOEXCEPT : p(ಖಾಲಿ) { }
Kernel& Kernel::ಕ್ರಿಯಾಕಾರಕ=(Kernel&&) CV_NOEXCEPT { ಹಿಂತಿರುಗಿ *ಇದು; }

ತಾರ್ಕಿಕ Kernel::empty() ಸ್ಥಿರಾಂಕ { ಹಿಂತಿರುಗಿ ನಿಜ; }
ತಾರ್ಕಿಕ Kernel::create(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* kname, ಸ್ಥಿರಾಂಕ ಕಾರ್ಯಕ್ರಮ_2& prog) { OCL_NOT_ಲಭ್ಯ(); }
ತಾರ್ಕಿಕ Kernel::create(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* kname, ಸ್ಥಿರಾಂಕ ಕಾರ್ಯಕ್ರಮ_ಮೂಲ& prog, ಸ್ಥಿರಾಂಕ String& buildopts, String* errmsg) { OCL_NOT_ಲಭ್ಯ(); }

ಪೂರ್ಣಾಂಕ Kernel::ಗಣ(ಪೂರ್ಣಾಂಕ i, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* value, size_t sz) { OCL_NOT_ಲಭ್ಯ(); }
ಪೂರ್ಣಾಂಕ Kernel::ಗಣ(ಪೂರ್ಣಾಂಕ i, ಸ್ಥಿರಾಂಕ Image2D& image2D) { OCL_NOT_ಲಭ್ಯ(); }
ಪೂರ್ಣಾಂಕ Kernel::ಗಣ(ಪೂರ್ಣಾಂಕ i, ಸ್ಥಿರಾಂಕ UMat& m) { OCL_NOT_ಲಭ್ಯ(); }
ಪೂರ್ಣಾಂಕ Kernel::ಗಣ(ಪೂರ್ಣಾಂಕ i, ಸ್ಥಿರಾಂಕ KernelArg& arg) { OCL_NOT_ಲಭ್ಯ(); }

ತಾರ್ಕಿಕ Kernel::ಸತತ(ಪೂರ್ಣಾಂಕ dims, size_t globalsize[], size_t localsize[], ತಾರ್ಕಿಕ ಸಮಕಾಲೀಕರಣ, ಸ್ಥಿರಾಂಕ ಸರತಿ& q) { OCL_NOT_ಲಭ್ಯ(); }
ತಾರ್ಕಿಕ Kernel::ಸತತ_Task(ತಾರ್ಕಿಕ ಸಮಕಾಲೀಕರಣ, ಸ್ಥಿರಾಂಕ ಸರತಿ& q) { OCL_NOT_ಲಭ್ಯ(); }

int64 Kernel::ಸತತ_Profiling(ಪೂರ್ಣಾಂಕ dims, size_t globalsize[], size_t localsize[], ಸ್ಥಿರಾಂಕ ಸರತಿ& q) { OCL_NOT_ಲಭ್ಯ(); }

size_t Kernel::work_ಗುಂಪು_ಗಾತ್ರ() ಸ್ಥಿರಾಂಕ { OCL_NOT_ಲಭ್ಯ(); }
size_t Kernel::prefered_Work_ಗುಂಪು_ಗಾತ್ರ_ಅಪವರ್ತ್ಯ() ಸ್ಥಿರಾಂಕ { OCL_NOT_ಲಭ್ಯ(); }
ತಾರ್ಕಿಕ Kernel::compile_Work_ಗುಂಪು_ಗಾತ್ರ(size_t wsz[]) ಸ್ಥಿರಾಂಕ { OCL_NOT_ಲಭ್ಯ(); }
size_t Kernel::local_Mem_ಗಾತ್ರ() ಸ್ಥಿರಾಂಕ { OCL_NOT_ಲಭ್ಯ(); }

ಶೂನ್ಯ* Kernel::ptr() ಸ್ಥಿರಾಂಕ { ಹಿಂತಿರುಗಿ ಖಾಲಿ; }


ಕಾರ್ಯಕ್ರಮ_2::ಕಾರ್ಯಕ್ರಮ_2() CV_NOEXCEPT : p(ಖಾಲಿ) { }
ಕಾರ್ಯಕ್ರಮ_2::ಕಾರ್ಯಕ್ರಮ_2(ಸ್ಥಿರಾಂಕ ಕಾರ್ಯಕ್ರಮ_ಮೂಲ& src, ಸ್ಥಿರಾಂಕ String& buildflags, String& errmsg) : p(ಖಾಲಿ) { OCL_NOT_ಲಭ್ಯ(); }
ಕಾರ್ಯಕ್ರಮ_2::ಕಾರ್ಯಕ್ರಮ_2(ಸ್ಥಿರಾಂಕ ಕಾರ್ಯಕ್ರಮ_2& prog) : p(ಖಾಲಿ) { }
ಕಾರ್ಯಕ್ರಮ_2& ಕಾರ್ಯಕ್ರಮ_2::ಕ್ರಿಯಾಕಾರಕ=(ಸ್ಥಿರಾಂಕ ಕಾರ್ಯಕ್ರಮ_2& prog) { ಹಿಂತಿರುಗಿ *ಇದು; }
ಕಾರ್ಯಕ್ರಮ_2::ಕಾರ್ಯಕ್ರಮ_2(ಕಾರ್ಯಕ್ರಮ_2&&) CV_NOEXCEPT : p(ಖಾಲಿ) { }
ಕಾರ್ಯಕ್ರಮ_2& ಕಾರ್ಯಕ್ರಮ_2::ಕ್ರಿಯಾಕಾರಕ=(ಕಾರ್ಯಕ್ರಮ_2&&) CV_NOEXCEPT { ಹಿಂತಿರುಗಿ *ಇದು; }
ಕಾರ್ಯಕ್ರಮ_2::~ಕಾರ್ಯಕ್ರಮ_2() { }

ತಾರ್ಕಿಕ ಕಾರ್ಯಕ್ರಮ_2::create(ಸ್ಥಿರಾಂಕ ಕಾರ್ಯಕ್ರಮ_ಮೂಲ& src, ಸ್ಥಿರಾಂಕ String& buildflags, String& errmsg) { OCL_NOT_ಲಭ್ಯ(); }

ಶೂನ್ಯ* ಕಾರ್ಯಕ್ರಮ_2::ptr() ಸ್ಥಿರಾಂಕ { ಹಿಂತಿರುಗಿ ಖಾಲಿ; }

ಶೂನ್ಯ ಕಾರ್ಯಕ್ರಮ_2::get_ದ್ವಿಮಾನ(std::vector<ಅಕ್ಷರ>& binary) ಸ್ಥಿರಾಂಕ { OCL_NOT_ಲಭ್ಯ(); }

ತಾರ್ಕಿಕ ಕಾರ್ಯಕ್ರಮ_2::ಪಠ(ಸ್ಥಿರಾಂಕ String& buf, ಸ್ಥಿರಾಂಕ String& buildflags) { OCL_NOT_ಲಭ್ಯ(); }
ತಾರ್ಕಿಕ ಕಾರ್ಯಕ್ರಮ_2::ಲಿಖ(String& buf) ಸ್ಥಿರಾಂಕ { OCL_NOT_ಲಭ್ಯ(); }
ಸ್ಥಿರಾಂಕ ಕಾರ್ಯಕ್ರಮ_ಮೂಲ& ಕಾರ್ಯಕ್ರಮ_2::ಮೂಲ() ಸ್ಥಿರಾಂಕ { OCL_NOT_ಲಭ್ಯ(); }
String ಕಾರ್ಯಕ್ರಮ_2::get_ಪೂರ್ವಪ್ರತ್ಯಯ() ಸ್ಥಿರಾಂಕ { OCL_NOT_ಲಭ್ಯ(); }
/* static */ String ಕಾರ್ಯಕ್ರಮ_2::get_ಪೂರ್ವಪ್ರತ್ಯಯ(ಸ್ಥಿರಾಂಕ String& buildflags) { OCL_NOT_ಲಭ್ಯ(); }


ಕಾರ್ಯಕ್ರಮ_ಮೂಲ::ಕಾರ್ಯಕ್ರಮ_ಮೂಲ() CV_NOEXCEPT : p(ಖಾಲಿ) { }
ಕಾರ್ಯಕ್ರಮ_ಮೂಲ::ಕಾರ್ಯಕ್ರಮ_ಮೂಲ(ಸ್ಥಿರಾಂಕ String& module, ಸ್ಥಿರಾಂಕ String& name, ಸ್ಥಿರಾಂಕ String& ಸಂಕೇತ_Str, ಸ್ಥಿರಾಂಕ String& ಸಂಕೇತ_Hash) : p(ಖಾಲಿ) { }
ಕಾರ್ಯಕ್ರಮ_ಮೂಲ::ಕಾರ್ಯಕ್ರಮ_ಮೂಲ(ಸ್ಥಿರಾಂಕ String& prog) : p(ಖಾಲಿ) { }
ಕಾರ್ಯಕ್ರಮ_ಮೂಲ::ಕಾರ್ಯಕ್ರಮ_ಮೂಲ(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* prog) : p(ಖಾಲಿ) { }
ಕಾರ್ಯಕ್ರಮ_ಮೂಲ::~ಕಾರ್ಯಕ್ರಮ_ಮೂಲ() { }
ಕಾರ್ಯಕ್ರಮ_ಮೂಲ::ಕಾರ್ಯಕ್ರಮ_ಮೂಲ(ಸ್ಥಿರಾಂಕ ಕಾರ್ಯಕ್ರಮ_ಮೂಲ& prog) : p(ಖಾಲಿ) { }
ಕಾರ್ಯಕ್ರಮ_ಮೂಲ& ಕಾರ್ಯಕ್ರಮ_ಮೂಲ::ಕ್ರಿಯಾಕಾರಕ=(ಸ್ಥಿರಾಂಕ ಕಾರ್ಯಕ್ರಮ_ಮೂಲ& prog) { ಹಿಂತಿರುಗಿ *ಇದು; }
ಕಾರ್ಯಕ್ರಮ_ಮೂಲ::ಕಾರ್ಯಕ್ರಮ_ಮೂಲ(ಕಾರ್ಯಕ್ರಮ_ಮೂಲ&&) CV_NOEXCEPT : p(ಖಾಲಿ) { }
ಕಾರ್ಯಕ್ರಮ_ಮೂಲ& ಕಾರ್ಯಕ್ರಮ_ಮೂಲ::ಕ್ರಿಯಾಕಾರಕ=(ಕಾರ್ಯಕ್ರಮ_ಮೂಲ&&) CV_NOEXCEPT { ಹಿಂತಿರುಗಿ *ಇದು; }

ಸ್ಥಿರಾಂಕ String& ಕಾರ್ಯಕ್ರಮ_ಮೂಲ::ಮೂಲ() ಸ್ಥಿರಾಂಕ { OCL_NOT_ಲಭ್ಯ(); }
ಕಾರ್ಯಕ್ರಮ_ಮೂಲ::hash_t ಕಾರ್ಯಕ್ರಮ_ಮೂಲ::hash() ಸ್ಥಿರಾಂಕ { OCL_NOT_ಲಭ್ಯ(); }

/* static */ ಕಾರ್ಯಕ್ರಮ_ಮೂಲ ಕಾರ್ಯಕ್ರಮ_ಮೂಲ::from_ದ್ವಿಮಾನ(ಸ್ಥಿರಾಂಕ String& module, ಸ್ಥಿರಾಂಕ String& name, ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ* binary, ಸ್ಥಿರಾಂಕ size_t size, ಸ್ಥಿರಾಂಕ cv::String& buildOptions) { OCL_NOT_ಲಭ್ಯ(); }
/* static */ ಕಾರ್ಯಕ್ರಮ_ಮೂಲ ಕಾರ್ಯಕ್ರಮ_ಮೂಲ::fromSPIR(ಸ್ಥಿರಾಂಕ String& module, ಸ್ಥಿರಾಂಕ String& name, ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ* binary, ಸ್ಥಿರಾಂಕ size_t size, ಸ್ಥಿರಾಂಕ cv::String& buildOptions) { OCL_NOT_ಲಭ್ಯ(); }


PlatformInfo::PlatformInfo() CV_NOEXCEPT : p(ಖಾಲಿ) { }
PlatformInfo::PlatformInfo(ಶೂನ್ಯ* id) : p(ಖಾಲಿ) { OCL_NOT_ಲಭ್ಯ(); }
PlatformInfo::~PlatformInfo() { }

PlatformInfo::PlatformInfo(ಸ್ಥಿರಾಂಕ PlatformInfo& i) : p(ಖಾಲಿ) { }
PlatformInfo& PlatformInfo::ಕ್ರಿಯಾಕಾರಕ=(ಸ್ಥಿರಾಂಕ PlatformInfo& i) { ಹಿಂತಿರುಗಿ *ಇದು; }
PlatformInfo::PlatformInfo(PlatformInfo&&) CV_NOEXCEPT : p(ಖಾಲಿ) { }
PlatformInfo& PlatformInfo::ಕ್ರಿಯಾಕಾರಕ=(PlatformInfo&&) CV_NOEXCEPT { ಹಿಂತಿರುಗಿ *ಇದು; }

String PlatformInfo::name() ಸ್ಥಿರಾಂಕ { OCL_NOT_ಲಭ್ಯ(); }
String PlatformInfo::vendor() ಸ್ಥಿರಾಂಕ { OCL_NOT_ಲಭ್ಯ(); }
String PlatformInfo::ಆವೃತ್ತಿ() ಸ್ಥಿರಾಂಕ { OCL_NOT_ಲಭ್ಯ(); }
ಪೂರ್ಣಾಂಕ PlatformInfo::device_ಸಂಖ್ಯೆ() ಸ್ಥಿರಾಂಕ { OCL_NOT_ಲಭ್ಯ(); }
ಶೂನ್ಯ PlatformInfo::getDevice(Device& device, ಪೂರ್ಣಾಂಕ d) ಸ್ಥಿರಾಂಕ { OCL_NOT_ಲಭ್ಯ(); }

ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* convertTypeStr(ಪೂರ್ಣಾಂಕ sdepth, ಪೂರ್ಣಾಂಕ ddepth, ಪೂರ್ಣಾಂಕ cn, ಅಕ್ಷರ* buf) { OCL_NOT_ಲಭ್ಯ(); }
ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* type_ಗಮ್ಯ_Str(ಪೂರ್ಣಾಂಕ t) { OCL_NOT_ಲಭ್ಯ(); }
ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* memop_Type_ಗಮ್ಯ_Str(ಪೂರ್ಣಾಂಕ t) { OCL_NOT_ಲಭ್ಯ(); }
ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* vecop_Type_ಗಮ್ಯ_Str(ಪೂರ್ಣಾಂಕ t) { OCL_NOT_ಲಭ್ಯ(); }
ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* getOpenCLErrorString(ಪೂರ್ಣಾಂಕ error_ಸಂಕೇತ) { OCL_NOT_ಲಭ್ಯ(); }
String kernel_ಗಮ್ಯ_Str(ಒಳಹರಿವು_Array _kernel, ಪೂರ್ಣಾಂಕ ddepth, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* name) { OCL_NOT_ಲಭ್ಯ(); }
ಶೂನ್ಯ getPlatfomsInfo(std::vector<PlatformInfo>& platform_info) { OCL_NOT_ಲಭ್ಯ(); }


ಪೂರ್ಣಾಂಕ predict_Optimal_ಸದಿಶ_ಅಗಲ(ಒಳಹರಿವು_Array src1, ಒಳಹರಿವು_Array src2, ಒಳಹರಿವು_Array src3,
        ಒಳಹರಿವು_Array src4, ಒಳಹರಿವು_Array src5, ಒಳಹರಿವು_Array src6,
        ಒಳಹರಿವು_Array src7, ಒಳಹರಿವು_Array src8, ಒಳಹರಿವು_Array src9,
        Ocl_ಸದಿಶ_Strategy strat)
{ OCL_NOT_ಲಭ್ಯ(); }

ಪೂರ್ಣಾಂಕ check_Optimal_ಸದಿಶ_ಅಗಲ(ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ *ಸದಿಶ_Widths,
        ಒಳಹರಿವು_Array src1, ಒಳಹರಿವು_Array src2, ಒಳಹರಿವು_Array src3,
        ಒಳಹರಿವು_Array src4, ಒಳಹರಿವು_Array src5, ಒಳಹರಿವು_Array src6,
        ಒಳಹರಿವು_Array src7, ಒಳಹರಿವು_Array src8, ಒಳಹರಿವು_Array src9,
        Ocl_ಸದಿಶ_Strategy strat)
{ OCL_NOT_ಲಭ್ಯ(); }

ಪೂರ್ಣಾಂಕ predict_Optimal_ಸದಿಶ_ಅಗಲ_Max(ಒಳಹರಿವು_Array src1, ಒಳಹರಿವು_Array src2, ಒಳಹರಿವು_Array src3,
        ಒಳಹರಿವು_Array src4, ಒಳಹರಿವು_Array src5, ಒಳಹರಿವು_Array src6,
        ಒಳಹರಿವು_Array src7, ಒಳಹರಿವು_Array src8, ಒಳಹರಿವು_Array src9)
{ OCL_NOT_ಲಭ್ಯ(); }

ಶೂನ್ಯ build_Options_ಕೂಡಿಸು_ಮಾತೃಕೆ_Description(String& buildOptions, ಸ್ಥಿರಾಂಕ String& name, ಒಳಹರಿವು_Array _m) { OCL_NOT_ಲಭ್ಯ(); }


Image2D::Image2D() CV_NOEXCEPT : p(ಖಾಲಿ) { }
Image2D::Image2D(ಸ್ಥಿರಾಂಕ UMat &src, ತಾರ್ಕಿಕ norm, ತಾರ್ಕಿಕ alias) { OCL_NOT_ಲಭ್ಯ(); }
Image2D::Image2D(ಸ್ಥಿರಾಂಕ Image2D & i) : p(ಖಾಲಿ) { OCL_NOT_ಲಭ್ಯ(); }
Image2D::~Image2D() { }
Image2D& Image2D::ಕ್ರಿಯಾಕಾರಕ=(ಸ್ಥಿರಾಂಕ Image2D & i) { ಹಿಂತಿರುಗಿ *ಇದು; }
Image2D::Image2D(Image2D&&) CV_NOEXCEPT : p(ಖಾಲಿ) { }
Image2D& Image2D::ಕ್ರಿಯಾಕಾರಕ=(Image2D&&) CV_NOEXCEPT { ಹಿಂತಿರುಗಿ *ಇದು; }

/* static */ ತಾರ್ಕಿಕ Image2D::canCreateAlias(ಸ್ಥಿರಾಂಕ UMat &u) { OCL_NOT_ಲಭ್ಯ(); }
/* static */ ತಾರ್ಕಿಕ Image2D::isFormatSupported(ಪೂರ್ಣಾಂಕ ಆಳ, ಪೂರ್ಣಾಂಕ cn, ತಾರ್ಕಿಕ norm) { OCL_NOT_ಲಭ್ಯ(); }

ಶೂನ್ಯ* Image2D::ptr() ಸ್ಥಿರಾಂಕ { ಹಿಂತಿರುಗಿ ಖಾಲಿ; }


Timer::Timer(ಸ್ಥಿರಾಂಕ ಸರತಿ& q) : p(ಖಾಲಿ) {}
Timer::~Timer() {}
ಶೂನ್ಯ Timer::ಆರಂಭ() { OCL_NOT_ಲಭ್ಯ(); }
ಶೂನ್ಯ Timer::stop() { OCL_NOT_ಲಭ್ಯ();}

uint64 Timer::durationNS() ಸ್ಥಿರಾಂಕ { OCL_NOT_ಲಭ್ಯ(); }

MatAllocator* getOpenCLAllocator() { ಹಿಂತಿರುಗಿ ಖಾಲಿ; }

internal::ಕಾರ್ಯಕ್ರಮ_ನಮೂದು::ಕ್ರಿಯಾಕಾರಕ ಕಾರ್ಯಕ್ರಮ_ಮೂಲ&() ಸ್ಥಿರಾಂಕ { OCL_NOT_ಲಭ್ಯ(); }


ರಚನೆ OpenCLExecutionContext::Impl
{
    Impl() = ಪೂರ್ವನಿಯೋಜಿತ;
};

Context& OpenCLExecutionContext::getContext() ಸ್ಥಿರಾಂಕ { OCL_NOT_ಲಭ್ಯ(); }
Device& OpenCLExecutionContext::getDevice() ಸ್ಥಿರಾಂಕ { OCL_NOT_ಲಭ್ಯ(); }
ಸರತಿ& OpenCLExecutionContext::get_ಸರತಿ() ಸ್ಥಿರಾಂಕ { OCL_NOT_ಲಭ್ಯ(); }

ತಾರ್ಕಿಕ OpenCLExecutionContext::useOpenCL() ಸ್ಥಿರಾಂಕ { ಹಿಂತಿರುಗಿ ಸುಳ್ಳು; }
ಶೂನ್ಯ OpenCLExecutionContext::ಗಣ_Use_Open_CL(ತಾರ್ಕಿಕ flag) { }

ಸ್ಥಿರ
OpenCLExecutionContext& getDummyOpenCLExecutionContext()
{
    ಸ್ಥಿರ OpenCLExecutionContext dummy;
    ಹಿಂತಿರುಗಿ dummy;
}

/* static */
OpenCLExecutionContext& OpenCLExecutionContext::getCurrent() { ಹಿಂತಿರುಗಿ getDummyOpenCLExecutionContext(); }

/* static */
OpenCLExecutionContext& OpenCLExecutionContext::getCurrentRef() { ಹಿಂತಿರುಗಿ getDummyOpenCLExecutionContext(); }

ಶೂನ್ಯ OpenCLExecutionContext::bind() ಸ್ಥಿರಾಂಕ { OCL_NOT_ಲಭ್ಯ(); }

OpenCLExecutionContext OpenCLExecutionContext::clone_With_New_ಸರತಿ(ಸ್ಥಿರಾಂಕ ocl::ಸರತಿ& q) ಸ್ಥಿರಾಂಕ { OCL_NOT_ಲಭ್ಯ(); }
OpenCLExecutionContext OpenCLExecutionContext::clone_With_New_ಸರತಿ() ಸ್ಥಿರಾಂಕ { OCL_NOT_ಲಭ್ಯ(); }

/* static */ OpenCLExecutionContext OpenCLExecutionContext::create(ಸ್ಥಿರಾಂಕ std::string& platformName, ಶೂನ್ಯ* platformID, ಶೂನ್ಯ* context, ಶೂನ್ಯ* deviceID) { OCL_NOT_ಲಭ್ಯ(); }
/* static */ OpenCLExecutionContext OpenCLExecutionContext::create(ಸ್ಥಿರಾಂಕ Context& context, ಸ್ಥಿರಾಂಕ Device& device, ಸ್ಥಿರಾಂಕ ocl::ಸರತಿ& ಸರತಿ_2) { OCL_NOT_ಲಭ್ಯ(); }
/* static */ OpenCLExecutionContext OpenCLExecutionContext::create(ಸ್ಥಿರಾಂಕ Context& context, ಸ್ಥಿರಾಂಕ Device& device) { OCL_NOT_ಲಭ್ಯ(); }

ಶೂನ್ಯ OpenCLExecutionContext::release() { }

}}

#ಆದರೆ defined(_MSC_VER)
    #ಸೂಚನೆ warning(pop)
#ಅಥವಾಆದರೆ defined(__clang__)
    #ಸೂಚನೆ clang diagnostic pop
#ಅಥವಾಆದರೆ defined(__GNUC__)
    #ಸೂಚನೆ GCC diagnostic pop
#ಆದರೆ_ಅಂತ್ಯ
