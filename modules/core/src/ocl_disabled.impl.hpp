// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ਅਨ੍ਤਰ੍ਭਾਵ "opencv2/core/ocl_genbase.hpp"

#ਜੇ defined(_MSC_VER)
    #ਹਦਾਇਤ warning(push)
    #ਹਦਾਇਤ warning(disable : 4100)
    #ਹਦਾਇਤ warning(disable : 4702)
#ਜਾਂਜੇ defined(__clang__)
    #ਹਦਾਇਤ clang diagnostic push
    #ਹਦਾਇਤ clang diagnostic ignored "-Wunused-parameter"
#ਜਾਂਜੇ defined(__GNUC__)
    #ਹਦਾਇਤ GCC diagnostic push
    #ਹਦਾਇਤ GCC diagnostic ignored "-Wunused-parameter"
#ਜੇ_ਅੰਤ

ਨਾਮਾਕਾਸ਼ cv { ਨਾਮਾਕਾਸ਼ ocl {

ਸਥਿਰ
CV_NORETURN ਖਾਲੀ throw_no_ocl()
{
    CV_Error(Error::OpenCLApiCallError, "OpenCV build without OpenCL support");
}
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OCL_NOT_ਉਪਲਬਧ() throw_no_ocl();

CV_EXPORTS_W ਤਾਰਕਿਕ haveOpenCL() { ਵਾਪਸ ਝੂਠ; }
CV_EXPORTS_W ਤਾਰਕਿਕ useOpenCL() { ਵਾਪਸ ਝੂਠ; }
CV_EXPORTS_W ਤਾਰਕਿਕ haveAmdBlas() { ਵਾਪਸ ਝੂਠ; }
CV_EXPORTS_W ਤਾਰਕਿਕ haveAmdFft() { ਵਾਪਸ ਝੂਠ; }
CV_EXPORTS_W ਖਾਲੀ ਸੈੱਟ_Use_Open_CL(ਤਾਰਕਿਕ flag) { /* nothing */ }
CV_EXPORTS_W ਖਾਲੀ ਅੰਤ_2() { /* nothing */ }

CV_EXPORTS ਤਾਰਕਿਕ haveSVM() { ਵਾਪਸ ਝੂਠ; }

Device::Device() CV_NOEXCEPT : p(ਨਿਰਾ) { }
Device::Device(ਖਾਲੀ* d) : p(ਨਿਰਾ) { OCL_NOT_ਉਪਲਬਧ(); }
Device::Device(ਅਚਲ Device& d) : p(ਨਿਰਾ) { }
Device& Device::ਸੰਚਾਲਕ=(ਅਚਲ Device& d) { ਵਾਪਸ *ਇਹ; }
Device::Device(Device&&) CV_NOEXCEPT : p(ਨਿਰਾ) { }
Device& Device::ਸੰਚਾਲਕ=(Device&&) CV_NOEXCEPT { ਵਾਪਸ *ਇਹ; }
Device::~Device() { }

ਖਾਲੀ Device::ਸੈੱਟ(ਖਾਲੀ* d) { OCL_NOT_ਉਪਲਬਧ(); }

String Device::name() ਅਚਲ { OCL_NOT_ਉਪਲਬਧ(); }
String Device::extensions() ਅਚਲ { OCL_NOT_ਉਪਲਬਧ(); }
ਤਾਰਕਿਕ Device::isExtensionSupported(ਅਚਲ String& extensionName) ਅਚਲ { OCL_NOT_ਉਪਲਬਧ(); }
String Device::ਸੰਸਕਰਣ() ਅਚਲ { OCL_NOT_ਉਪਲਬਧ(); }
String Device::vendorName() ਅਚਲ { OCL_NOT_ਉਪਲਬਧ(); }
String Device::Open_CL_C_ਸੰਸਕਰਣ() ਅਚਲ { OCL_NOT_ਉਪਲਬਧ(); }
String Device::OpenCLVersion() ਅਚਲ { OCL_NOT_ਉਪਲਬਧ(); }
ਪੂਰਨ Device::device_ਸੰਸਕਰਣ_Major() ਅਚਲ { OCL_NOT_ਉਪਲਬਧ(); }
ਪੂਰਨ Device::device_ਸੰਸਕਰਣ_Minor() ਅਚਲ { OCL_NOT_ਉਪਲਬਧ(); }
String Device::driver_ਸੰਸਕਰਣ() ਅਚਲ { OCL_NOT_ਉਪਲਬਧ(); }
ਖਾਲੀ* Device::ptr() ਅਚਲ { /*OCL_NOT_AVAILABLE();*/ ਵਾਪਸ ਨਿਰਾ; }

ਪੂਰਨ Device::type() ਅਚਲ { OCL_NOT_ਉਪਲਬਧ(); }

ਪੂਰਨ Device::ਪਤਾ_ਬਿੱਟ() ਅਚਲ { OCL_NOT_ਉਪਲਬਧ(); }
ਤਾਰਕਿਕ Device::ਉਪਲਬਧ() ਅਚਲ { OCL_NOT_ਉਪਲਬਧ(); }
ਤਾਰਕਿਕ Device::compiler_ਉਪਲਬਧ() ਅਚਲ { OCL_NOT_ਉਪਲਬਧ(); }
ਤਾਰਕਿਕ Device::linker_ਉਪਲਬਧ() ਅਚਲ { OCL_NOT_ਉਪਲਬਧ(); }

ਪੂਰਨ Device::doubleFPConfig() ਅਚਲ { OCL_NOT_ਉਪਲਬਧ(); }
ਪੂਰਨ Device::singleFPConfig() ਅਚਲ { OCL_NOT_ਉਪਲਬਧ(); }
ਪੂਰਨ Device::halfFPConfig() ਅਚਲ { OCL_NOT_ਉਪਲਬਧ(); }

ਤਾਰਕਿਕ Device::hasFP64() ਅਚਲ { OCL_NOT_ਉਪਲਬਧ(); }
ਤਾਰਕਿਕ Device::hasFP16() ਅਚਲ { OCL_NOT_ਉਪਲਬਧ(); }

ਤਾਰਕਿਕ Device::endianLittle() ਅਚਲ { OCL_NOT_ਉਪਲਬਧ(); }
ਤਾਰਕਿਕ Device::errorCorrectionSupport() ਅਚਲ { OCL_NOT_ਉਪਲਬਧ(); }

ਪੂਰਨ Device::executionCapabilities() ਅਚਲ { OCL_NOT_ਉਪਲਬਧ(); }

size_t Device::global_Mem_Cache_ਮਾਪ() ਅਚਲ { OCL_NOT_ਉਪਲਬਧ(); }

ਪੂਰਨ Device::globalMemCacheType() ਅਚਲ { OCL_NOT_ਉਪਲਬਧ(); }
ਪੂਰਨ Device::global_Mem_Cache_ਸਤਰ_ਮਾਪ() ਅਚਲ { OCL_NOT_ਉਪਲਬਧ(); }
size_t Device::global_Mem_ਮਾਪ() ਅਚਲ { OCL_NOT_ਉਪਲਬਧ(); }

size_t Device::local_Mem_ਮਾਪ() ਅਚਲ { OCL_NOT_ਉਪਲਬਧ(); }
ਪੂਰਨ Device::localMemType() ਅਚਲ { ਵਾਪਸ NO_LOCAL_MEM; }
ਤਾਰਕਿਕ Device::host_Unified_ਸਮ੍ਰਿਤੀ_2() ਅਚਲ { OCL_NOT_ਉਪਲਬਧ(); }

ਤਾਰਕਿਕ Device::imageSupport() ਅਚਲ { OCL_NOT_ਉਪਲਬਧ(); }

ਤਾਰਕਿਕ Device::image_From_ਬਫਰ_Support() ਅਚਲ { OCL_NOT_ਉਪਲਬਧ(); }
uint Device::imagePitchAlignment() ਅਚਲ { OCL_NOT_ਉਪਲਬਧ(); }
uint Device::image_ਅਧਾਰ_ਪਤਾ_Alignment() ਅਚਲ { OCL_NOT_ਉਪਲਬਧ(); }

ਤਾਰਕਿਕ Device::intelSubgroupsSupport() ਅਚਲ { OCL_NOT_ਉਪਲਬਧ(); }

size_t Device::image2_DMax_ਚੌੜਾਈ() ਅਚਲ { OCL_NOT_ਉਪਲਬਧ(); }
size_t Device::image2_DMax_ਉਚਾਈ() ਅਚਲ { OCL_NOT_ਉਪਲਬਧ(); }

size_t Device::image3_DMax_ਚੌੜਾਈ() ਅਚਲ { OCL_NOT_ਉਪਲਬਧ(); }
size_t Device::image3_DMax_ਉਚਾਈ() ਅਚਲ { OCL_NOT_ਉਪਲਬਧ(); }
size_t Device::image3_DMax_ਡੂੰਘਾਈ() ਅਚਲ { OCL_NOT_ਉਪਲਬਧ(); }

size_t Device::image_Max_ਬਫਰ_ਮਾਪ() ਅਚਲ { OCL_NOT_ਉਪਲਬਧ(); }
size_t Device::image_Max_Array_ਮਾਪ() ਅਚਲ { OCL_NOT_ਉਪਲਬਧ(); }

ਪੂਰਨ Device::vendorID() ਅਚਲ { OCL_NOT_ਉਪਲਬਧ(); }

ਪੂਰਨ Device::maxClockFrequency() ਅਚਲ { OCL_NOT_ਉਪਲਬਧ(); }
ਪੂਰਨ Device::maxComputeUnits() ਅਚਲ { OCL_NOT_ਉਪਲਬਧ(); }
ਪੂਰਨ Device::max_Constant_ਦਲੀਲਾਂ() ਅਚਲ { OCL_NOT_ਉਪਲਬਧ(); }
size_t Device::max_Constant_ਬਫਰ_ਮਾਪ() ਅਚਲ { OCL_NOT_ਉਪਲਬਧ(); }

size_t Device::max_Mem_Alloc_ਮਾਪ() ਅਚਲ { OCL_NOT_ਉਪਲਬਧ(); }
size_t Device::max_Parameter_ਮਾਪ() ਅਚਲ { OCL_NOT_ਉਪਲਬਧ(); }

ਪੂਰਨ Device::max_Read_Image_ਦਲੀਲਾਂ() ਅਚਲ { OCL_NOT_ਉਪਲਬਧ(); }
ਪੂਰਨ Device::max_Write_Image_ਦਲੀਲਾਂ() ਅਚਲ { OCL_NOT_ਉਪਲਬਧ(); }
ਪੂਰਨ Device::maxSamplers() ਅਚਲ { OCL_NOT_ਉਪਲਬਧ(); }

size_t Device::max_Work_ਸਮੂਹ_ਮਾਪ() ਅਚਲ { OCL_NOT_ਉਪਲਬਧ(); }
ਪੂਰਨ Device::max_Work_ਵਸਤੂ_Dims() ਅਚਲ { OCL_NOT_ਉਪਲਬਧ(); }
ਖਾਲੀ Device::max_Work_ਵਸਤੂ_Sizes(size_t*) ਅਚਲ { OCL_NOT_ਉਪਲਬਧ(); }

ਪੂਰਨ Device::mem_ਅਧਾਰ_Addr_Align() ਅਚਲ { OCL_NOT_ਉਪਲਬਧ(); }

ਪੂਰਨ Device::native_ਸਦਿਸ਼_ਚੌੜਾਈ_Char() ਅਚਲ { OCL_NOT_ਉਪਲਬਧ(); }
ਪੂਰਨ Device::native_ਸਦਿਸ਼_ਚੌੜਾਈ_Short() ਅਚਲ { OCL_NOT_ਉਪਲਬਧ(); }
ਪੂਰਨ Device::native_ਸਦਿਸ਼_ਚੌੜਾਈ_Int() ਅਚਲ { OCL_NOT_ਉਪਲਬਧ(); }
ਪੂਰਨ Device::native_ਸਦਿਸ਼_ਚੌੜਾਈ_Long() ਅਚਲ { OCL_NOT_ਉਪਲਬਧ(); }
ਪੂਰਨ Device::native_ਸਦਿਸ਼_ਚੌੜਾਈ_Float() ਅਚਲ { OCL_NOT_ਉਪਲਬਧ(); }
ਪੂਰਨ Device::native_ਸਦਿਸ਼_ਚੌੜਾਈ_Double() ਅਚਲ { OCL_NOT_ਉਪਲਬਧ(); }
ਪੂਰਨ Device::native_ਸਦਿਸ਼_ਚੌੜਾਈ_Half() ਅਚਲ { OCL_NOT_ਉਪਲਬਧ(); }

ਪੂਰਨ Device::preferred_ਸਦਿਸ਼_ਚੌੜਾਈ_Char() ਅਚਲ { OCL_NOT_ਉਪਲਬਧ(); }
ਪੂਰਨ Device::preferred_ਸਦਿਸ਼_ਚੌੜਾਈ_Short() ਅਚਲ { OCL_NOT_ਉਪਲਬਧ(); }
ਪੂਰਨ Device::preferred_ਸਦਿਸ਼_ਚੌੜਾਈ_Int() ਅਚਲ { OCL_NOT_ਉਪਲਬਧ(); }
ਪੂਰਨ Device::preferred_ਸਦਿਸ਼_ਚੌੜਾਈ_Long() ਅਚਲ { OCL_NOT_ਉਪਲਬਧ(); }
ਪੂਰਨ Device::preferred_ਸਦਿਸ਼_ਚੌੜਾਈ_Float() ਅਚਲ { OCL_NOT_ਉਪਲਬਧ(); }
ਪੂਰਨ Device::preferred_ਸਦਿਸ਼_ਚੌੜਾਈ_Double() ਅਚਲ { OCL_NOT_ਉਪਲਬਧ(); }
ਪੂਰਨ Device::preferred_ਸਦਿਸ਼_ਚੌੜਾਈ_Half() ਅਚਲ { OCL_NOT_ਉਪਲਬਧ(); }

size_t Device::printf_ਬਫਰ_ਮਾਪ() ਅਚਲ { OCL_NOT_ਉਪਲਬਧ(); }
size_t Device::profilingTimerResolution() ਅਚਲ { OCL_NOT_ਉਪਲਬਧ(); }

/* static */
ਅਚਲ Device& Device::getDefault()
{
    ਸਥਿਰ Device dummy;
    ਵਾਪਸ dummy;
}

/* static */ Device Device::fromHandle(ਖਾਲੀ* d) { OCL_NOT_ਉਪਲਬਧ(); }


Context::Context() CV_NOEXCEPT : p(ਨਿਰਾ) { }
Context::Context(ਪੂਰਨ dtype) : p(ਨਿਰਾ) { }
Context::~Context() { }
Context::Context(ਅਚਲ Context& c) : p(ਨਿਰਾ) { }
Context& Context::ਸੰਚਾਲਕ=(ਅਚਲ Context& c) { ਵਾਪਸ *ਇਹ; }
Context::Context(Context&&) CV_NOEXCEPT : p(ਨਿਰਾ) { }
Context& Context::ਸੰਚਾਲਕ=(Context&&) CV_NOEXCEPT { ਵਾਪਸ *ਇਹ; }

ਤਾਰਕਿਕ Context::create() { ਵਾਪਸ ਝੂਠ; }
ਤਾਰਕਿਕ Context::create(ਪੂਰਨ dtype) { ਵਾਪਸ ਝੂਠ; }
size_t Context::ndevices() ਅਚਲ { ਵਾਪਸ 0; }
Device& Context::device(size_t idx) ਅਚਲ { OCL_NOT_ਉਪਲਬਧ(); }
ਪ੍ਰੋਗਰਾਮ_2 Context::getProg(ਅਚਲ ਪ੍ਰੋਗਰਾਮ_ਸਰੋਤ& prog, ਅਚਲ String& buildopt, String& errmsg) { OCL_NOT_ਉਪਲਬਧ(); }
ਖਾਲੀ Context::unloadProg(ਪ੍ਰੋਗਰਾਮ_2& prog) { }

/* static */
Context& Context::getDefault()
{
    ਸਥਿਰ Context dummy;
    ਵਾਪਸ dummy;
}
ਖਾਲੀ* Context::ptr() ਅਚਲ { ਵਾਪਸ ਨਿਰਾ; }

ਖਾਲੀ* Context::getOpenCLContextProperty(ਪੂਰਨ /*propertyId*/) ਅਚਲ { OCL_NOT_ਉਪਲਬਧ(); }

ਤਾਰਕਿਕ Context::useSVM() ਅਚਲ { ਵਾਪਸ ਝੂਠ; }
ਖਾਲੀ Context::ਸੈੱਟ_Use_SVM(ਤਾਰਕਿਕ enabled) { }

Context::UserContext::~UserContext() { }

ਖਾਲੀ Context::ਸੈੱਟ_User_Context(std::type_ਸੂਚਕਾਂਕ /*typeId*/, ਅਚਲ std::shared_ptr<Context::UserContext>& /*userContext*/) { OCL_NOT_ਉਪਲਬਧ(); }
std::shared_ptr<Context::UserContext> Context::getUserContext(std::type_ਸੂਚਕਾਂਕ /*typeId*/) { OCL_NOT_ਉਪਲਬਧ(); }

/* static */ Context Context::fromHandle(ਖਾਲੀ* context) { OCL_NOT_ਉਪਲਬਧ(); }
/* static */ Context Context::fromDevice(ਅਚਲ ocl::Device& device) { OCL_NOT_ਉਪਲਬਧ(); }
/* static */ Context Context::create(ਅਚਲ std::string& configuration) { OCL_NOT_ਉਪਲਬਧ(); }

ਖਾਲੀ Context::release() { }


Platform::Platform() CV_NOEXCEPT : p(ਨਿਰਾ) { }
Platform::~Platform() { }
Platform::Platform(ਅਚਲ Platform&) : p(ਨਿਰਾ) { }
Platform& Platform::ਸੰਚਾਲਕ=(ਅਚਲ Platform&) { ਵਾਪਸ *ਇਹ; }
Platform::Platform(Platform&&) CV_NOEXCEPT : p(ਨਿਰਾ) { }
Platform& Platform::ਸੰਚਾਲਕ=(Platform&&) CV_NOEXCEPT { ਵਾਪਸ *ਇਹ; }

ਖਾਲੀ* Platform::ptr() ਅਚਲ { ਵਾਪਸ ਨਿਰਾ; }

/* static */
Platform& Platform::getDefault()
{
    ਸਥਿਰ Platform dummy;
    ਵਾਪਸ dummy;
}

ਖਾਲੀ attachContext(ਅਚਲ String& platformName, ਖਾਲੀ* platformID, ਖਾਲੀ* context, ਖਾਲੀ* deviceID) { OCL_NOT_ਉਪਲਬਧ(); }
ਖਾਲੀ convert_From_ਬਫਰ(ਖਾਲੀ* cl_mem_ਬਫਰ, size_t ਕਦਮ, ਪੂਰਨ ਪੰਕਤੀਆਂ, ਪੂਰਨ ਖਾਨੇ_ਗਿਣਤੀ, ਪੂਰਨ type, UMat& dst) { OCL_NOT_ਉਪਲਬਧ(); }
ਖਾਲੀ convertFromImage(ਖਾਲੀ* cl_mem_image, UMat& dst) { OCL_NOT_ਉਪਲਬਧ(); }

ਖਾਲੀ initializeContextFromHandle(Context& ctx, ਖਾਲੀ* platform, ਖਾਲੀ* context, ਖਾਲੀ* device) { OCL_NOT_ਉਪਲਬਧ(); }

ਕਤਾਰ::ਕਤਾਰ() CV_NOEXCEPT : p(ਨਿਰਾ) { }
ਕਤਾਰ::ਕਤਾਰ(ਅਚਲ Context& c, ਅਚਲ Device& d) : p(ਨਿਰਾ) { OCL_NOT_ਉਪਲਬਧ(); }
ਕਤਾਰ::~ਕਤਾਰ() { }
ਕਤਾਰ::ਕਤਾਰ(ਅਚਲ ਕਤਾਰ& q) {}
ਕਤਾਰ& ਕਤਾਰ::ਸੰਚਾਲਕ=(ਅਚਲ ਕਤਾਰ& q) { ਵਾਪਸ *ਇਹ; }
ਕਤਾਰ::ਕਤਾਰ(ਕਤਾਰ&&) CV_NOEXCEPT : p(ਨਿਰਾ) { }
ਕਤਾਰ& ਕਤਾਰ::ਸੰਚਾਲਕ=(ਕਤਾਰ&&) CV_NOEXCEPT { ਵਾਪਸ *ਇਹ; }

ਤਾਰਕਿਕ ਕਤਾਰ::create(ਅਚਲ Context& c, ਅਚਲ Device& d) { OCL_NOT_ਉਪਲਬਧ(); }
ਖਾਲੀ ਕਤਾਰ::ਅੰਤ_2() {}
ਖਾਲੀ* ਕਤਾਰ::ptr() ਅਚਲ { ਵਾਪਸ ਨਿਰਾ; }
/* static */
ਕਤਾਰ& ਕਤਾਰ::getDefault()
{
    ਸਥਿਰ ਕਤਾਰ dummy;
    ਵਾਪਸ dummy;
}

/// @brief Returns OpenCL command queue with enable profiling mode support
ਅਚਲ ਕਤਾਰ& ਕਤਾਰ::get_Profiling_ਕਤਾਰ() ਅਚਲ { OCL_NOT_ਉਪਲਬਧ(); }


KernelArg::KernelArg() CV_NOEXCEPT
    : flags(0), m(0), obj(0), sz(0), wscale(1), iwscale(1)
{
}

KernelArg::KernelArg(ਪੂਰਨ _flags, UMat* _m, ਪੂਰਨ _wscale, ਪੂਰਨ _iwscale, ਅਚਲ ਖਾਲੀ* _obj, size_t _sz)
    : flags(_flags), m(_m), obj(_obj), sz(_sz), wscale(_wscale), iwscale(_iwscale)
{
    OCL_NOT_ਉਪਲਬਧ();
}

KernelArg KernelArg::Constant(ਅਚਲ Mat& m)
{
    OCL_NOT_ਉਪਲਬਧ();
}


Kernel::Kernel() CV_NOEXCEPT : p(ਨਿਰਾ) { }
Kernel::Kernel(ਅਚਲ ਅੱਖਰ* kname, ਅਚਲ ਪ੍ਰੋਗਰਾਮ_2& prog) : p(ਨਿਰਾ) { OCL_NOT_ਉਪਲਬਧ(); }
Kernel::Kernel(ਅਚਲ ਅੱਖਰ* kname, ਅਚਲ ਪ੍ਰੋਗਰਾਮ_ਸਰੋਤ& prog, ਅਚਲ String& buildopts, String* errmsg) : p(ਨਿਰਾ) { OCL_NOT_ਉਪਲਬਧ(); }
Kernel::~Kernel() { }
Kernel::Kernel(ਅਚਲ Kernel& k) : p(ਨਿਰਾ) { }
Kernel& Kernel::ਸੰਚਾਲਕ=(ਅਚਲ Kernel& k) { ਵਾਪਸ *ਇਹ; }
Kernel::Kernel(Kernel&&) CV_NOEXCEPT : p(ਨਿਰਾ) { }
Kernel& Kernel::ਸੰਚਾਲਕ=(Kernel&&) CV_NOEXCEPT { ਵਾਪਸ *ਇਹ; }

ਤਾਰਕਿਕ Kernel::empty() ਅਚਲ { ਵਾਪਸ ਸੱਚ; }
ਤਾਰਕਿਕ Kernel::create(ਅਚਲ ਅੱਖਰ* kname, ਅਚਲ ਪ੍ਰੋਗਰਾਮ_2& prog) { OCL_NOT_ਉਪਲਬਧ(); }
ਤਾਰਕਿਕ Kernel::create(ਅਚਲ ਅੱਖਰ* kname, ਅਚਲ ਪ੍ਰੋਗਰਾਮ_ਸਰੋਤ& prog, ਅਚਲ String& buildopts, String* errmsg) { OCL_NOT_ਉਪਲਬਧ(); }

ਪੂਰਨ Kernel::ਸੈੱਟ(ਪੂਰਨ i, ਅਚਲ ਖਾਲੀ* value, size_t sz) { OCL_NOT_ਉਪਲਬਧ(); }
ਪੂਰਨ Kernel::ਸੈੱਟ(ਪੂਰਨ i, ਅਚਲ Image2D& image2D) { OCL_NOT_ਉਪਲਬਧ(); }
ਪੂਰਨ Kernel::ਸੈੱਟ(ਪੂਰਨ i, ਅਚਲ UMat& m) { OCL_NOT_ਉਪਲਬਧ(); }
ਪੂਰਨ Kernel::ਸੈੱਟ(ਪੂਰਨ i, ਅਚਲ KernelArg& arg) { OCL_NOT_ਉਪਲਬਧ(); }

ਤਾਰਕਿਕ Kernel::ਲੜੀਵਾਰ(ਪੂਰਨ dims, size_t globalsize[], size_t localsize[], ਤਾਰਕਿਕ ਸਮਕਾਲੀ, ਅਚਲ ਕਤਾਰ& q) { OCL_NOT_ਉਪਲਬਧ(); }
ਤਾਰਕਿਕ Kernel::ਲੜੀਵਾਰ_Task(ਤਾਰਕਿਕ ਸਮਕਾਲੀ, ਅਚਲ ਕਤਾਰ& q) { OCL_NOT_ਉਪਲਬਧ(); }

int64 Kernel::ਲੜੀਵਾਰ_Profiling(ਪੂਰਨ dims, size_t globalsize[], size_t localsize[], ਅਚਲ ਕਤਾਰ& q) { OCL_NOT_ਉਪਲਬਧ(); }

size_t Kernel::work_ਸਮੂਹ_ਮਾਪ() ਅਚਲ { OCL_NOT_ਉਪਲਬਧ(); }
size_t Kernel::prefered_Work_ਸਮੂਹ_ਮਾਪ_ਗੁਣਜ() ਅਚਲ { OCL_NOT_ਉਪਲਬਧ(); }
ਤਾਰਕਿਕ Kernel::compile_Work_ਸਮੂਹ_ਮਾਪ(size_t wsz[]) ਅਚਲ { OCL_NOT_ਉਪਲਬਧ(); }
size_t Kernel::local_Mem_ਮਾਪ() ਅਚਲ { OCL_NOT_ਉਪਲਬਧ(); }

ਖਾਲੀ* Kernel::ptr() ਅਚਲ { ਵਾਪਸ ਨਿਰਾ; }


ਪ੍ਰੋਗਰਾਮ_2::ਪ੍ਰੋਗਰਾਮ_2() CV_NOEXCEPT : p(ਨਿਰਾ) { }
ਪ੍ਰੋਗਰਾਮ_2::ਪ੍ਰੋਗਰਾਮ_2(ਅਚਲ ਪ੍ਰੋਗਰਾਮ_ਸਰੋਤ& src, ਅਚਲ String& buildflags, String& errmsg) : p(ਨਿਰਾ) { OCL_NOT_ਉਪਲਬਧ(); }
ਪ੍ਰੋਗਰਾਮ_2::ਪ੍ਰੋਗਰਾਮ_2(ਅਚਲ ਪ੍ਰੋਗਰਾਮ_2& prog) : p(ਨਿਰਾ) { }
ਪ੍ਰੋਗਰਾਮ_2& ਪ੍ਰੋਗਰਾਮ_2::ਸੰਚਾਲਕ=(ਅਚਲ ਪ੍ਰੋਗਰਾਮ_2& prog) { ਵਾਪਸ *ਇਹ; }
ਪ੍ਰੋਗਰਾਮ_2::ਪ੍ਰੋਗਰਾਮ_2(ਪ੍ਰੋਗਰਾਮ_2&&) CV_NOEXCEPT : p(ਨਿਰਾ) { }
ਪ੍ਰੋਗਰਾਮ_2& ਪ੍ਰੋਗਰਾਮ_2::ਸੰਚਾਲਕ=(ਪ੍ਰੋਗਰਾਮ_2&&) CV_NOEXCEPT { ਵਾਪਸ *ਇਹ; }
ਪ੍ਰੋਗਰਾਮ_2::~ਪ੍ਰੋਗਰਾਮ_2() { }

ਤਾਰਕਿਕ ਪ੍ਰੋਗਰਾਮ_2::create(ਅਚਲ ਪ੍ਰੋਗਰਾਮ_ਸਰੋਤ& src, ਅਚਲ String& buildflags, String& errmsg) { OCL_NOT_ਉਪਲਬਧ(); }

ਖਾਲੀ* ਪ੍ਰੋਗਰਾਮ_2::ptr() ਅਚਲ { ਵਾਪਸ ਨਿਰਾ; }

ਖਾਲੀ ਪ੍ਰੋਗਰਾਮ_2::get_ਦੁਆਧਾਰੀ(std::vector<ਅੱਖਰ>& binary) ਅਚਲ { OCL_NOT_ਉਪਲਬਧ(); }

ਤਾਰਕਿਕ ਪ੍ਰੋਗਰਾਮ_2::ਪਠ(ਅਚਲ String& buf, ਅਚਲ String& buildflags) { OCL_NOT_ਉਪਲਬਧ(); }
ਤਾਰਕਿਕ ਪ੍ਰੋਗਰਾਮ_2::ਲਿਖ(String& buf) ਅਚਲ { OCL_NOT_ਉਪਲਬਧ(); }
ਅਚਲ ਪ੍ਰੋਗਰਾਮ_ਸਰੋਤ& ਪ੍ਰੋਗਰਾਮ_2::ਸਰੋਤ() ਅਚਲ { OCL_NOT_ਉਪਲਬਧ(); }
String ਪ੍ਰੋਗਰਾਮ_2::get_ਅਗੇਤਰ() ਅਚਲ { OCL_NOT_ਉਪਲਬਧ(); }
/* static */ String ਪ੍ਰੋਗਰਾਮ_2::get_ਅਗੇਤਰ(ਅਚਲ String& buildflags) { OCL_NOT_ਉਪਲਬਧ(); }


ਪ੍ਰੋਗਰਾਮ_ਸਰੋਤ::ਪ੍ਰੋਗਰਾਮ_ਸਰੋਤ() CV_NOEXCEPT : p(ਨਿਰਾ) { }
ਪ੍ਰੋਗਰਾਮ_ਸਰੋਤ::ਪ੍ਰੋਗਰਾਮ_ਸਰੋਤ(ਅਚਲ String& module, ਅਚਲ String& name, ਅਚਲ String& ਕੋਡ_Str, ਅਚਲ String& ਕੋਡ_Hash) : p(ਨਿਰਾ) { }
ਪ੍ਰੋਗਰਾਮ_ਸਰੋਤ::ਪ੍ਰੋਗਰਾਮ_ਸਰੋਤ(ਅਚਲ String& prog) : p(ਨਿਰਾ) { }
ਪ੍ਰੋਗਰਾਮ_ਸਰੋਤ::ਪ੍ਰੋਗਰਾਮ_ਸਰੋਤ(ਅਚਲ ਅੱਖਰ* prog) : p(ਨਿਰਾ) { }
ਪ੍ਰੋਗਰਾਮ_ਸਰੋਤ::~ਪ੍ਰੋਗਰਾਮ_ਸਰੋਤ() { }
ਪ੍ਰੋਗਰਾਮ_ਸਰੋਤ::ਪ੍ਰੋਗਰਾਮ_ਸਰੋਤ(ਅਚਲ ਪ੍ਰੋਗਰਾਮ_ਸਰੋਤ& prog) : p(ਨਿਰਾ) { }
ਪ੍ਰੋਗਰਾਮ_ਸਰੋਤ& ਪ੍ਰੋਗਰਾਮ_ਸਰੋਤ::ਸੰਚਾਲਕ=(ਅਚਲ ਪ੍ਰੋਗਰਾਮ_ਸਰੋਤ& prog) { ਵਾਪਸ *ਇਹ; }
ਪ੍ਰੋਗਰਾਮ_ਸਰੋਤ::ਪ੍ਰੋਗਰਾਮ_ਸਰੋਤ(ਪ੍ਰੋਗਰਾਮ_ਸਰੋਤ&&) CV_NOEXCEPT : p(ਨਿਰਾ) { }
ਪ੍ਰੋਗਰਾਮ_ਸਰੋਤ& ਪ੍ਰੋਗਰਾਮ_ਸਰੋਤ::ਸੰਚਾਲਕ=(ਪ੍ਰੋਗਰਾਮ_ਸਰੋਤ&&) CV_NOEXCEPT { ਵਾਪਸ *ਇਹ; }

ਅਚਲ String& ਪ੍ਰੋਗਰਾਮ_ਸਰੋਤ::ਸਰੋਤ() ਅਚਲ { OCL_NOT_ਉਪਲਬਧ(); }
ਪ੍ਰੋਗਰਾਮ_ਸਰੋਤ::hash_t ਪ੍ਰੋਗਰਾਮ_ਸਰੋਤ::hash() ਅਚਲ { OCL_NOT_ਉਪਲਬਧ(); }

/* static */ ਪ੍ਰੋਗਰਾਮ_ਸਰੋਤ ਪ੍ਰੋਗਰਾਮ_ਸਰੋਤ::from_ਦੁਆਧਾਰੀ(ਅਚਲ String& module, ਅਚਲ String& name, ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ* binary, ਅਚਲ size_t size, ਅਚਲ cv::String& buildOptions) { OCL_NOT_ਉਪਲਬਧ(); }
/* static */ ਪ੍ਰੋਗਰਾਮ_ਸਰੋਤ ਪ੍ਰੋਗਰਾਮ_ਸਰੋਤ::fromSPIR(ਅਚਲ String& module, ਅਚਲ String& name, ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ* binary, ਅਚਲ size_t size, ਅਚਲ cv::String& buildOptions) { OCL_NOT_ਉਪਲਬਧ(); }


PlatformInfo::PlatformInfo() CV_NOEXCEPT : p(ਨਿਰਾ) { }
PlatformInfo::PlatformInfo(ਖਾਲੀ* id) : p(ਨਿਰਾ) { OCL_NOT_ਉਪਲਬਧ(); }
PlatformInfo::~PlatformInfo() { }

PlatformInfo::PlatformInfo(ਅਚਲ PlatformInfo& i) : p(ਨਿਰਾ) { }
PlatformInfo& PlatformInfo::ਸੰਚਾਲਕ=(ਅਚਲ PlatformInfo& i) { ਵਾਪਸ *ਇਹ; }
PlatformInfo::PlatformInfo(PlatformInfo&&) CV_NOEXCEPT : p(ਨਿਰਾ) { }
PlatformInfo& PlatformInfo::ਸੰਚਾਲਕ=(PlatformInfo&&) CV_NOEXCEPT { ਵਾਪਸ *ਇਹ; }

String PlatformInfo::name() ਅਚਲ { OCL_NOT_ਉਪਲਬਧ(); }
String PlatformInfo::vendor() ਅਚਲ { OCL_NOT_ਉਪਲਬਧ(); }
String PlatformInfo::ਸੰਸਕਰਣ() ਅਚਲ { OCL_NOT_ਉਪਲਬਧ(); }
ਪੂਰਨ PlatformInfo::device_ਸੰਖਿਆ() ਅਚਲ { OCL_NOT_ਉਪਲਬਧ(); }
ਖਾਲੀ PlatformInfo::getDevice(Device& device, ਪੂਰਨ d) ਅਚਲ { OCL_NOT_ਉਪਲਬਧ(); }

ਅਚਲ ਅੱਖਰ* convertTypeStr(ਪੂਰਨ sdepth, ਪੂਰਨ ddepth, ਪੂਰਨ cn, ਅੱਖਰ* buf) { OCL_NOT_ਉਪਲਬਧ(); }
ਅਚਲ ਅੱਖਰ* type_ਮੰਜ਼ਿਲ_Str(ਪੂਰਨ t) { OCL_NOT_ਉਪਲਬਧ(); }
ਅਚਲ ਅੱਖਰ* memop_Type_ਮੰਜ਼ਿਲ_Str(ਪੂਰਨ t) { OCL_NOT_ਉਪਲਬਧ(); }
ਅਚਲ ਅੱਖਰ* vecop_Type_ਮੰਜ਼ਿਲ_Str(ਪੂਰਨ t) { OCL_NOT_ਉਪਲਬਧ(); }
ਅਚਲ ਅੱਖਰ* getOpenCLErrorString(ਪੂਰਨ error_ਕੋਡ) { OCL_NOT_ਉਪਲਬਧ(); }
String kernel_ਮੰਜ਼ਿਲ_Str(ਨਿਵੇਸ਼_Array _kernel, ਪੂਰਨ ddepth, ਅਚਲ ਅੱਖਰ* name) { OCL_NOT_ਉਪਲਬਧ(); }
ਖਾਲੀ getPlatfomsInfo(std::vector<PlatformInfo>& platform_info) { OCL_NOT_ਉਪਲਬਧ(); }


ਪੂਰਨ predict_Optimal_ਸਦਿਸ਼_ਚੌੜਾਈ(ਨਿਵੇਸ਼_Array src1, ਨਿਵੇਸ਼_Array src2, ਨਿਵੇਸ਼_Array src3,
        ਨਿਵੇਸ਼_Array src4, ਨਿਵੇਸ਼_Array src5, ਨਿਵੇਸ਼_Array src6,
        ਨਿਵੇਸ਼_Array src7, ਨਿਵੇਸ਼_Array src8, ਨਿਵੇਸ਼_Array src9,
        Ocl_ਸਦਿਸ਼_Strategy strat)
{ OCL_NOT_ਉਪਲਬਧ(); }

ਪੂਰਨ check_Optimal_ਸਦਿਸ਼_ਚੌੜਾਈ(ਅਚਲ ਪੂਰਨ *ਸਦਿਸ਼_Widths,
        ਨਿਵੇਸ਼_Array src1, ਨਿਵੇਸ਼_Array src2, ਨਿਵੇਸ਼_Array src3,
        ਨਿਵੇਸ਼_Array src4, ਨਿਵੇਸ਼_Array src5, ਨਿਵੇਸ਼_Array src6,
        ਨਿਵੇਸ਼_Array src7, ਨਿਵੇਸ਼_Array src8, ਨਿਵੇਸ਼_Array src9,
        Ocl_ਸਦਿਸ਼_Strategy strat)
{ OCL_NOT_ਉਪਲਬਧ(); }

ਪੂਰਨ predict_Optimal_ਸਦਿਸ਼_ਚੌੜਾਈ_Max(ਨਿਵੇਸ਼_Array src1, ਨਿਵੇਸ਼_Array src2, ਨਿਵੇਸ਼_Array src3,
        ਨਿਵੇਸ਼_Array src4, ਨਿਵੇਸ਼_Array src5, ਨਿਵੇਸ਼_Array src6,
        ਨਿਵੇਸ਼_Array src7, ਨਿਵੇਸ਼_Array src8, ਨਿਵੇਸ਼_Array src9)
{ OCL_NOT_ਉਪਲਬਧ(); }

ਖਾਲੀ build_Options_ਜੋੜੋ_ਮੈਟ੍ਰਿਕਸ_Description(String& buildOptions, ਅਚਲ String& name, ਨਿਵੇਸ਼_Array _m) { OCL_NOT_ਉਪਲਬਧ(); }


Image2D::Image2D() CV_NOEXCEPT : p(ਨਿਰਾ) { }
Image2D::Image2D(ਅਚਲ UMat &src, ਤਾਰਕਿਕ norm, ਤਾਰਕਿਕ alias) { OCL_NOT_ਉਪਲਬਧ(); }
Image2D::Image2D(ਅਚਲ Image2D & i) : p(ਨਿਰਾ) { OCL_NOT_ਉਪਲਬਧ(); }
Image2D::~Image2D() { }
Image2D& Image2D::ਸੰਚਾਲਕ=(ਅਚਲ Image2D & i) { ਵਾਪਸ *ਇਹ; }
Image2D::Image2D(Image2D&&) CV_NOEXCEPT : p(ਨਿਰਾ) { }
Image2D& Image2D::ਸੰਚਾਲਕ=(Image2D&&) CV_NOEXCEPT { ਵਾਪਸ *ਇਹ; }

/* static */ ਤਾਰਕਿਕ Image2D::canCreateAlias(ਅਚਲ UMat &u) { OCL_NOT_ਉਪਲਬਧ(); }
/* static */ ਤਾਰਕਿਕ Image2D::isFormatSupported(ਪੂਰਨ ਡੂੰਘਾਈ, ਪੂਰਨ cn, ਤਾਰਕਿਕ norm) { OCL_NOT_ਉਪਲਬਧ(); }

ਖਾਲੀ* Image2D::ptr() ਅਚਲ { ਵਾਪਸ ਨਿਰਾ; }


Timer::Timer(ਅਚਲ ਕਤਾਰ& q) : p(ਨਿਰਾ) {}
Timer::~Timer() {}
ਖਾਲੀ Timer::ਸ਼ੁਰੂ() { OCL_NOT_ਉਪਲਬਧ(); }
ਖਾਲੀ Timer::stop() { OCL_NOT_ਉਪਲਬਧ();}

uint64 Timer::durationNS() ਅਚਲ { OCL_NOT_ਉਪਲਬਧ(); }

MatAllocator* getOpenCLAllocator() { ਵਾਪਸ ਨਿਰਾ; }

internal::ਪ੍ਰੋਗਰਾਮ_ਇੰਦਰਾਜ::ਸੰਚਾਲਕ ਪ੍ਰੋਗਰਾਮ_ਸਰੋਤ&() ਅਚਲ { OCL_NOT_ਉਪਲਬਧ(); }


ਬਣਤਰ OpenCLExecutionContext::Impl
{
    Impl() = ਮੂਲ;
};

Context& OpenCLExecutionContext::getContext() ਅਚਲ { OCL_NOT_ਉਪਲਬਧ(); }
Device& OpenCLExecutionContext::getDevice() ਅਚਲ { OCL_NOT_ਉਪਲਬਧ(); }
ਕਤਾਰ& OpenCLExecutionContext::get_ਕਤਾਰ() ਅਚਲ { OCL_NOT_ਉਪਲਬਧ(); }

ਤਾਰਕਿਕ OpenCLExecutionContext::useOpenCL() ਅਚਲ { ਵਾਪਸ ਝੂਠ; }
ਖਾਲੀ OpenCLExecutionContext::ਸੈੱਟ_Use_Open_CL(ਤਾਰਕਿਕ flag) { }

ਸਥਿਰ
OpenCLExecutionContext& getDummyOpenCLExecutionContext()
{
    ਸਥਿਰ OpenCLExecutionContext dummy;
    ਵਾਪਸ dummy;
}

/* static */
OpenCLExecutionContext& OpenCLExecutionContext::getCurrent() { ਵਾਪਸ getDummyOpenCLExecutionContext(); }

/* static */
OpenCLExecutionContext& OpenCLExecutionContext::getCurrentRef() { ਵਾਪਸ getDummyOpenCLExecutionContext(); }

ਖਾਲੀ OpenCLExecutionContext::bind() ਅਚਲ { OCL_NOT_ਉਪਲਬਧ(); }

OpenCLExecutionContext OpenCLExecutionContext::clone_With_New_ਕਤਾਰ(ਅਚਲ ocl::ਕਤਾਰ& q) ਅਚਲ { OCL_NOT_ਉਪਲਬਧ(); }
OpenCLExecutionContext OpenCLExecutionContext::clone_With_New_ਕਤਾਰ() ਅਚਲ { OCL_NOT_ਉਪਲਬਧ(); }

/* static */ OpenCLExecutionContext OpenCLExecutionContext::create(ਅਚਲ std::string& platformName, ਖਾਲੀ* platformID, ਖਾਲੀ* context, ਖਾਲੀ* deviceID) { OCL_NOT_ਉਪਲਬਧ(); }
/* static */ OpenCLExecutionContext OpenCLExecutionContext::create(ਅਚਲ Context& context, ਅਚਲ Device& device, ਅਚਲ ocl::ਕਤਾਰ& ਕਤਾਰ_2) { OCL_NOT_ਉਪਲਬਧ(); }
/* static */ OpenCLExecutionContext OpenCLExecutionContext::create(ਅਚਲ Context& context, ਅਚਲ Device& device) { OCL_NOT_ਉਪਲਬਧ(); }

ਖਾਲੀ OpenCLExecutionContext::release() { }

}}

#ਜੇ defined(_MSC_VER)
    #ਹਦਾਇਤ warning(pop)
#ਜਾਂਜੇ defined(__clang__)
    #ਹਦਾਇਤ clang diagnostic pop
#ਜਾਂਜੇ defined(__GNUC__)
    #ਹਦਾਇਤ GCC diagnostic pop
#ਜੇ_ਅੰਤ
