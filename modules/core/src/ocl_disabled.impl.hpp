// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ضمن "opencv2/core/ocl_genbase.hpp"

#إذا defined(_MSC_VER)
    #هدايت warning(push)
    #هدايت warning(disable : 4100)
    #هدايت warning(disable : 4702)
#وإلاإذا defined(__clang__)
    #هدايت clang diagnostic push
    #هدايت clang diagnostic ignored "-Wunused-parameter"
#وإلاإذا defined(__GNUC__)
    #هدايت GCC diagnostic push
    #هدايت GCC diagnostic ignored "-Wunused-parameter"
#جيڪڏهن_پورو

نطاق cv { نطاق ocl {

ساكن
CV_NORETURN فراغ throw_no_ocl()
{
    CV_Error(Error::OpenCLApiCallError, "OpenCV build without OpenCL support");
}
#وصف_ڪريو OCL_NOT_موجود() throw_no_ocl();

CV_EXPORTS_W منطقي haveOpenCL() { أرجع خطأ; }
CV_EXPORTS_W منطقي useOpenCL() { أرجع خطأ; }
CV_EXPORTS_W منطقي haveAmdBlas() { أرجع خطأ; }
CV_EXPORTS_W منطقي haveAmdFft() { أرجع خطأ; }
CV_EXPORTS_W فراغ سيٽ_Use_Open_CL(منطقي flag) { /* nothing */ }
CV_EXPORTS_W فراغ پڄاڻي_2() { /* nothing */ }

CV_EXPORTS منطقي haveSVM() { أرجع خطأ; }

Device::Device() CV_NOEXCEPT : p(عدم) { }
Device::Device(فراغ* d) : p(عدم) { OCL_NOT_موجود(); }
Device::Device(ثابت Device& d) : p(عدم) { }
Device& Device::عامل=(ثابت Device& d) { أرجع *هذا; }
Device::Device(Device&&) CV_NOEXCEPT : p(عدم) { }
Device& Device::عامل=(Device&&) CV_NOEXCEPT { أرجع *هذا; }
Device::~Device() { }

فراغ Device::سيٽ(فراغ* d) { OCL_NOT_موجود(); }

String Device::name() ثابت { OCL_NOT_موجود(); }
String Device::extensions() ثابت { OCL_NOT_موجود(); }
منطقي Device::isExtensionSupported(ثابت String& extensionName) ثابت { OCL_NOT_موجود(); }
String Device::نسخو() ثابت { OCL_NOT_موجود(); }
String Device::vendorName() ثابت { OCL_NOT_موجود(); }
String Device::Open_CL_C_نسخو() ثابت { OCL_NOT_موجود(); }
String Device::OpenCLVersion() ثابت { OCL_NOT_موجود(); }
صحيح Device::device_نسخو_Major() ثابت { OCL_NOT_موجود(); }
صحيح Device::device_نسخو_Minor() ثابت { OCL_NOT_موجود(); }
String Device::driver_نسخو() ثابت { OCL_NOT_موجود(); }
فراغ* Device::ptr() ثابت { /*OCL_NOT_AVAILABLE();*/ أرجع عدم; }

صحيح Device::type() ثابت { OCL_NOT_موجود(); }

صحيح Device::پتو_بٽ() ثابت { OCL_NOT_موجود(); }
منطقي Device::موجود() ثابت { OCL_NOT_موجود(); }
منطقي Device::compiler_موجود() ثابت { OCL_NOT_موجود(); }
منطقي Device::linker_موجود() ثابت { OCL_NOT_موجود(); }

صحيح Device::doubleFPConfig() ثابت { OCL_NOT_موجود(); }
صحيح Device::singleFPConfig() ثابت { OCL_NOT_موجود(); }
صحيح Device::halfFPConfig() ثابت { OCL_NOT_موجود(); }

منطقي Device::hasFP64() ثابت { OCL_NOT_موجود(); }
منطقي Device::hasFP16() ثابت { OCL_NOT_موجود(); }

منطقي Device::endianLittle() ثابت { OCL_NOT_موجود(); }
منطقي Device::errorCorrectionSupport() ثابت { OCL_NOT_موجود(); }

صحيح Device::executionCapabilities() ثابت { OCL_NOT_موجود(); }

size_t Device::global_Mem_Cache_سائيز() ثابت { OCL_NOT_موجود(); }

صحيح Device::globalMemCacheType() ثابت { OCL_NOT_موجود(); }
صحيح Device::global_Mem_Cache_سٽ_سائيز() ثابت { OCL_NOT_موجود(); }
size_t Device::global_Mem_سائيز() ثابت { OCL_NOT_موجود(); }

size_t Device::local_Mem_سائيز() ثابت { OCL_NOT_موجود(); }
صحيح Device::localMemType() ثابت { أرجع NO_LOCAL_MEM; }
منطقي Device::host_Unified_يادگيري_2() ثابت { OCL_NOT_موجود(); }

منطقي Device::imageSupport() ثابت { OCL_NOT_موجود(); }

منطقي Device::image_From_بفر_Support() ثابت { OCL_NOT_موجود(); }
uint Device::imagePitchAlignment() ثابت { OCL_NOT_موجود(); }
uint Device::image_بنياد_پتو_Alignment() ثابت { OCL_NOT_موجود(); }

منطقي Device::intelSubgroupsSupport() ثابت { OCL_NOT_موجود(); }

size_t Device::image2_DMax_ويڪر() ثابت { OCL_NOT_موجود(); }
size_t Device::image2_DMax_اوچائي() ثابت { OCL_NOT_موجود(); }

size_t Device::image3_DMax_ويڪر() ثابت { OCL_NOT_موجود(); }
size_t Device::image3_DMax_اوچائي() ثابت { OCL_NOT_موجود(); }
size_t Device::image3_DMax_اونهائي() ثابت { OCL_NOT_موجود(); }

size_t Device::image_Max_بفر_سائيز() ثابت { OCL_NOT_موجود(); }
size_t Device::image_Max_Array_سائيز() ثابت { OCL_NOT_موجود(); }

صحيح Device::vendorID() ثابت { OCL_NOT_موجود(); }

صحيح Device::maxClockFrequency() ثابت { OCL_NOT_موجود(); }
صحيح Device::maxComputeUnits() ثابت { OCL_NOT_موجود(); }
صحيح Device::max_Constant_دليلون() ثابت { OCL_NOT_موجود(); }
size_t Device::max_Constant_بفر_سائيز() ثابت { OCL_NOT_موجود(); }

size_t Device::max_Mem_Alloc_سائيز() ثابت { OCL_NOT_موجود(); }
size_t Device::max_Parameter_سائيز() ثابت { OCL_NOT_موجود(); }

صحيح Device::max_Read_Image_دليلون() ثابت { OCL_NOT_موجود(); }
صحيح Device::max_Write_Image_دليلون() ثابت { OCL_NOT_موجود(); }
صحيح Device::maxSamplers() ثابت { OCL_NOT_موجود(); }

size_t Device::max_Work_گروهه_سائيز() ثابت { OCL_NOT_موجود(); }
صحيح Device::max_Work_شي_Dims() ثابت { OCL_NOT_موجود(); }
فراغ Device::max_Work_شي_Sizes(size_t*) ثابت { OCL_NOT_موجود(); }

صحيح Device::mem_بنياد_Addr_Align() ثابت { OCL_NOT_موجود(); }

صحيح Device::native_ويڪٽر_ويڪر_Char() ثابت { OCL_NOT_موجود(); }
صحيح Device::native_ويڪٽر_ويڪر_Short() ثابت { OCL_NOT_موجود(); }
صحيح Device::native_ويڪٽر_ويڪر_Int() ثابت { OCL_NOT_موجود(); }
صحيح Device::native_ويڪٽر_ويڪر_Long() ثابت { OCL_NOT_موجود(); }
صحيح Device::native_ويڪٽر_ويڪر_Float() ثابت { OCL_NOT_موجود(); }
صحيح Device::native_ويڪٽر_ويڪر_Double() ثابت { OCL_NOT_موجود(); }
صحيح Device::native_ويڪٽر_ويڪر_Half() ثابت { OCL_NOT_موجود(); }

صحيح Device::preferred_ويڪٽر_ويڪر_Char() ثابت { OCL_NOT_موجود(); }
صحيح Device::preferred_ويڪٽر_ويڪر_Short() ثابت { OCL_NOT_موجود(); }
صحيح Device::preferred_ويڪٽر_ويڪر_Int() ثابت { OCL_NOT_موجود(); }
صحيح Device::preferred_ويڪٽر_ويڪر_Long() ثابت { OCL_NOT_موجود(); }
صحيح Device::preferred_ويڪٽر_ويڪر_Float() ثابت { OCL_NOT_موجود(); }
صحيح Device::preferred_ويڪٽر_ويڪر_Double() ثابت { OCL_NOT_موجود(); }
صحيح Device::preferred_ويڪٽر_ويڪر_Half() ثابت { OCL_NOT_موجود(); }

size_t Device::printf_بفر_سائيز() ثابت { OCL_NOT_موجود(); }
size_t Device::profilingTimerResolution() ثابت { OCL_NOT_موجود(); }

/* static */
ثابت Device& Device::getDefault()
{
    ساكن Device dummy;
    أرجع dummy;
}

/* static */ Device Device::fromHandle(فراغ* d) { OCL_NOT_موجود(); }


Context::Context() CV_NOEXCEPT : p(عدم) { }
Context::Context(صحيح dtype) : p(عدم) { }
Context::~Context() { }
Context::Context(ثابت Context& c) : p(عدم) { }
Context& Context::عامل=(ثابت Context& c) { أرجع *هذا; }
Context::Context(Context&&) CV_NOEXCEPT : p(عدم) { }
Context& Context::عامل=(Context&&) CV_NOEXCEPT { أرجع *هذا; }

منطقي Context::create() { أرجع خطأ; }
منطقي Context::create(صحيح dtype) { أرجع خطأ; }
size_t Context::ndevices() ثابت { أرجع 0; }
Device& Context::device(size_t idx) ثابت { OCL_NOT_موجود(); }
پروگرام_2 Context::getProg(ثابت پروگرام_ذريعو& prog, ثابت String& buildopt, String& errmsg) { OCL_NOT_موجود(); }
فراغ Context::unloadProg(پروگرام_2& prog) { }

/* static */
Context& Context::getDefault()
{
    ساكن Context dummy;
    أرجع dummy;
}
فراغ* Context::ptr() ثابت { أرجع عدم; }

فراغ* Context::getOpenCLContextProperty(صحيح /*propertyId*/) ثابت { OCL_NOT_موجود(); }

منطقي Context::useSVM() ثابت { أرجع خطأ; }
فراغ Context::سيٽ_Use_SVM(منطقي enabled) { }

Context::UserContext::~UserContext() { }

فراغ Context::سيٽ_User_Context(std::type_اشاريو /*typeId*/, ثابت std::shared_ptr<Context::UserContext>& /*userContext*/) { OCL_NOT_موجود(); }
std::shared_ptr<Context::UserContext> Context::getUserContext(std::type_اشاريو /*typeId*/) { OCL_NOT_موجود(); }

/* static */ Context Context::fromHandle(فراغ* context) { OCL_NOT_موجود(); }
/* static */ Context Context::fromDevice(ثابت ocl::Device& device) { OCL_NOT_موجود(); }
/* static */ Context Context::create(ثابت std::string& configuration) { OCL_NOT_موجود(); }

فراغ Context::release() { }


Platform::Platform() CV_NOEXCEPT : p(عدم) { }
Platform::~Platform() { }
Platform::Platform(ثابت Platform&) : p(عدم) { }
Platform& Platform::عامل=(ثابت Platform&) { أرجع *هذا; }
Platform::Platform(Platform&&) CV_NOEXCEPT : p(عدم) { }
Platform& Platform::عامل=(Platform&&) CV_NOEXCEPT { أرجع *هذا; }

فراغ* Platform::ptr() ثابت { أرجع عدم; }

/* static */
Platform& Platform::getDefault()
{
    ساكن Platform dummy;
    أرجع dummy;
}

فراغ attachContext(ثابت String& platformName, فراغ* platformID, فراغ* context, فراغ* deviceID) { OCL_NOT_موجود(); }
فراغ convert_From_بفر(فراغ* cl_mem_بفر, size_t قدم, صحيح صفون, صحيح کانن_ڳڻپ, صحيح type, UMat& dst) { OCL_NOT_موجود(); }
فراغ convertFromImage(فراغ* cl_mem_image, UMat& dst) { OCL_NOT_موجود(); }

فراغ initializeContextFromHandle(Context& ctx, فراغ* platform, فراغ* context, فراغ* device) { OCL_NOT_موجود(); }

قطار::قطار() CV_NOEXCEPT : p(عدم) { }
قطار::قطار(ثابت Context& c, ثابت Device& d) : p(عدم) { OCL_NOT_موجود(); }
قطار::~قطار() { }
قطار::قطار(ثابت قطار& q) {}
قطار& قطار::عامل=(ثابت قطار& q) { أرجع *هذا; }
قطار::قطار(قطار&&) CV_NOEXCEPT : p(عدم) { }
قطار& قطار::عامل=(قطار&&) CV_NOEXCEPT { أرجع *هذا; }

منطقي قطار::create(ثابت Context& c, ثابت Device& d) { OCL_NOT_موجود(); }
فراغ قطار::پڄاڻي_2() {}
فراغ* قطار::ptr() ثابت { أرجع عدم; }
/* static */
قطار& قطار::getDefault()
{
    ساكن قطار dummy;
    أرجع dummy;
}

/// @brief Returns OpenCL command queue with enable profiling mode support
ثابت قطار& قطار::get_Profiling_قطار() ثابت { OCL_NOT_موجود(); }


KernelArg::KernelArg() CV_NOEXCEPT
    : flags(0), m(0), obj(0), sz(0), wscale(1), iwscale(1)
{
}

KernelArg::KernelArg(صحيح _flags, UMat* _m, صحيح _wscale, صحيح _iwscale, ثابت فراغ* _obj, size_t _sz)
    : flags(_flags), m(_m), obj(_obj), sz(_sz), wscale(_wscale), iwscale(_iwscale)
{
    OCL_NOT_موجود();
}

KernelArg KernelArg::Constant(ثابت Mat& m)
{
    OCL_NOT_موجود();
}


Kernel::Kernel() CV_NOEXCEPT : p(عدم) { }
Kernel::Kernel(ثابت حرف* kname, ثابت پروگرام_2& prog) : p(عدم) { OCL_NOT_موجود(); }
Kernel::Kernel(ثابت حرف* kname, ثابت پروگرام_ذريعو& prog, ثابت String& buildopts, String* errmsg) : p(عدم) { OCL_NOT_موجود(); }
Kernel::~Kernel() { }
Kernel::Kernel(ثابت Kernel& k) : p(عدم) { }
Kernel& Kernel::عامل=(ثابت Kernel& k) { أرجع *هذا; }
Kernel::Kernel(Kernel&&) CV_NOEXCEPT : p(عدم) { }
Kernel& Kernel::عامل=(Kernel&&) CV_NOEXCEPT { أرجع *هذا; }

منطقي Kernel::empty() ثابت { أرجع سچ; }
منطقي Kernel::create(ثابت حرف* kname, ثابت پروگرام_2& prog) { OCL_NOT_موجود(); }
منطقي Kernel::create(ثابت حرف* kname, ثابت پروگرام_ذريعو& prog, ثابت String& buildopts, String* errmsg) { OCL_NOT_موجود(); }

صحيح Kernel::سيٽ(صحيح i, ثابت فراغ* value, size_t sz) { OCL_NOT_موجود(); }
صحيح Kernel::سيٽ(صحيح i, ثابت Image2D& image2D) { OCL_NOT_موجود(); }
صحيح Kernel::سيٽ(صحيح i, ثابت UMat& m) { OCL_NOT_موجود(); }
صحيح Kernel::سيٽ(صحيح i, ثابت KernelArg& arg) { OCL_NOT_موجود(); }

منطقي Kernel::لاڳيتو(صحيح dims, size_t globalsize[], size_t localsize[], منطقي متزامن, ثابت قطار& q) { OCL_NOT_موجود(); }
منطقي Kernel::لاڳيتو_Task(منطقي متزامن, ثابت قطار& q) { OCL_NOT_موجود(); }

int64 Kernel::لاڳيتو_Profiling(صحيح dims, size_t globalsize[], size_t localsize[], ثابت قطار& q) { OCL_NOT_موجود(); }

size_t Kernel::work_گروهه_سائيز() ثابت { OCL_NOT_موجود(); }
size_t Kernel::prefered_Work_گروهه_سائيز_گڻو() ثابت { OCL_NOT_موجود(); }
منطقي Kernel::compile_Work_گروهه_سائيز(size_t wsz[]) ثابت { OCL_NOT_موجود(); }
size_t Kernel::local_Mem_سائيز() ثابت { OCL_NOT_موجود(); }

فراغ* Kernel::ptr() ثابت { أرجع عدم; }


پروگرام_2::پروگرام_2() CV_NOEXCEPT : p(عدم) { }
پروگرام_2::پروگرام_2(ثابت پروگرام_ذريعو& src, ثابت String& buildflags, String& errmsg) : p(عدم) { OCL_NOT_موجود(); }
پروگرام_2::پروگرام_2(ثابت پروگرام_2& prog) : p(عدم) { }
پروگرام_2& پروگرام_2::عامل=(ثابت پروگرام_2& prog) { أرجع *هذا; }
پروگرام_2::پروگرام_2(پروگرام_2&&) CV_NOEXCEPT : p(عدم) { }
پروگرام_2& پروگرام_2::عامل=(پروگرام_2&&) CV_NOEXCEPT { أرجع *هذا; }
پروگرام_2::~پروگرام_2() { }

منطقي پروگرام_2::create(ثابت پروگرام_ذريعو& src, ثابت String& buildflags, String& errmsg) { OCL_NOT_موجود(); }

فراغ* پروگرام_2::ptr() ثابت { أرجع عدم; }

فراغ پروگرام_2::get_ٻٽو(std::vector<حرف>& binary) ثابت { OCL_NOT_موجود(); }

منطقي پروگرام_2::اقرأ(ثابت String& buf, ثابت String& buildflags) { OCL_NOT_موجود(); }
منطقي پروگرام_2::اكتب(String& buf) ثابت { OCL_NOT_موجود(); }
ثابت پروگرام_ذريعو& پروگرام_2::ذريعو() ثابت { OCL_NOT_موجود(); }
String پروگرام_2::get_اڳياڙي() ثابت { OCL_NOT_موجود(); }
/* static */ String پروگرام_2::get_اڳياڙي(ثابت String& buildflags) { OCL_NOT_موجود(); }


پروگرام_ذريعو::پروگرام_ذريعو() CV_NOEXCEPT : p(عدم) { }
پروگرام_ذريعو::پروگرام_ذريعو(ثابت String& module, ثابت String& name, ثابت String& ڪوڊ_Str, ثابت String& ڪوڊ_Hash) : p(عدم) { }
پروگرام_ذريعو::پروگرام_ذريعو(ثابت String& prog) : p(عدم) { }
پروگرام_ذريعو::پروگرام_ذريعو(ثابت حرف* prog) : p(عدم) { }
پروگرام_ذريعو::~پروگرام_ذريعو() { }
پروگرام_ذريعو::پروگرام_ذريعو(ثابت پروگرام_ذريعو& prog) : p(عدم) { }
پروگرام_ذريعو& پروگرام_ذريعو::عامل=(ثابت پروگرام_ذريعو& prog) { أرجع *هذا; }
پروگرام_ذريعو::پروگرام_ذريعو(پروگرام_ذريعو&&) CV_NOEXCEPT : p(عدم) { }
پروگرام_ذريعو& پروگرام_ذريعو::عامل=(پروگرام_ذريعو&&) CV_NOEXCEPT { أرجع *هذا; }

ثابت String& پروگرام_ذريعو::ذريعو() ثابت { OCL_NOT_موجود(); }
پروگرام_ذريعو::hash_t پروگرام_ذريعو::hash() ثابت { OCL_NOT_موجود(); }

/* static */ پروگرام_ذريعو پروگرام_ذريعو::from_ٻٽو(ثابت String& module, ثابت String& name, ثابت بنانشان حرف* binary, ثابت size_t size, ثابت cv::String& buildOptions) { OCL_NOT_موجود(); }
/* static */ پروگرام_ذريعو پروگرام_ذريعو::fromSPIR(ثابت String& module, ثابت String& name, ثابت بنانشان حرف* binary, ثابت size_t size, ثابت cv::String& buildOptions) { OCL_NOT_موجود(); }


PlatformInfo::PlatformInfo() CV_NOEXCEPT : p(عدم) { }
PlatformInfo::PlatformInfo(فراغ* id) : p(عدم) { OCL_NOT_موجود(); }
PlatformInfo::~PlatformInfo() { }

PlatformInfo::PlatformInfo(ثابت PlatformInfo& i) : p(عدم) { }
PlatformInfo& PlatformInfo::عامل=(ثابت PlatformInfo& i) { أرجع *هذا; }
PlatformInfo::PlatformInfo(PlatformInfo&&) CV_NOEXCEPT : p(عدم) { }
PlatformInfo& PlatformInfo::عامل=(PlatformInfo&&) CV_NOEXCEPT { أرجع *هذا; }

String PlatformInfo::name() ثابت { OCL_NOT_موجود(); }
String PlatformInfo::vendor() ثابت { OCL_NOT_موجود(); }
String PlatformInfo::نسخو() ثابت { OCL_NOT_موجود(); }
صحيح PlatformInfo::device_عدد() ثابت { OCL_NOT_موجود(); }
فراغ PlatformInfo::getDevice(Device& device, صحيح d) ثابت { OCL_NOT_موجود(); }

ثابت حرف* convertTypeStr(صحيح sdepth, صحيح ddepth, صحيح cn, حرف* buf) { OCL_NOT_موجود(); }
ثابت حرف* type_منزل_Str(صحيح t) { OCL_NOT_موجود(); }
ثابت حرف* memop_Type_منزل_Str(صحيح t) { OCL_NOT_موجود(); }
ثابت حرف* vecop_Type_منزل_Str(صحيح t) { OCL_NOT_موجود(); }
ثابت حرف* getOpenCLErrorString(صحيح error_ڪوڊ) { OCL_NOT_موجود(); }
String kernel_منزل_Str(داخلا_Array _kernel, صحيح ddepth, ثابت حرف* name) { OCL_NOT_موجود(); }
فراغ getPlatfomsInfo(std::vector<PlatformInfo>& platform_info) { OCL_NOT_موجود(); }


صحيح predict_Optimal_ويڪٽر_ويڪر(داخلا_Array src1, داخلا_Array src2, داخلا_Array src3,
        داخلا_Array src4, داخلا_Array src5, داخلا_Array src6,
        داخلا_Array src7, داخلا_Array src8, داخلا_Array src9,
        Ocl_ويڪٽر_Strategy strat)
{ OCL_NOT_موجود(); }

صحيح check_Optimal_ويڪٽر_ويڪر(ثابت صحيح *ويڪٽر_Widths,
        داخلا_Array src1, داخلا_Array src2, داخلا_Array src3,
        داخلا_Array src4, داخلا_Array src5, داخلا_Array src6,
        داخلا_Array src7, داخلا_Array src8, داخلا_Array src9,
        Ocl_ويڪٽر_Strategy strat)
{ OCL_NOT_موجود(); }

صحيح predict_Optimal_ويڪٽر_ويڪر_Max(داخلا_Array src1, داخلا_Array src2, داخلا_Array src3,
        داخلا_Array src4, داخلا_Array src5, داخلا_Array src6,
        داخلا_Array src7, داخلا_Array src8, داخلا_Array src9)
{ OCL_NOT_موجود(); }

فراغ build_Options_جوڙيو_ميٽرڪس_Description(String& buildOptions, ثابت String& name, داخلا_Array _m) { OCL_NOT_موجود(); }


Image2D::Image2D() CV_NOEXCEPT : p(عدم) { }
Image2D::Image2D(ثابت UMat &src, منطقي norm, منطقي alias) { OCL_NOT_موجود(); }
Image2D::Image2D(ثابت Image2D & i) : p(عدم) { OCL_NOT_موجود(); }
Image2D::~Image2D() { }
Image2D& Image2D::عامل=(ثابت Image2D & i) { أرجع *هذا; }
Image2D::Image2D(Image2D&&) CV_NOEXCEPT : p(عدم) { }
Image2D& Image2D::عامل=(Image2D&&) CV_NOEXCEPT { أرجع *هذا; }

/* static */ منطقي Image2D::canCreateAlias(ثابت UMat &u) { OCL_NOT_موجود(); }
/* static */ منطقي Image2D::isFormatSupported(صحيح اونهائي, صحيح cn, منطقي norm) { OCL_NOT_موجود(); }

فراغ* Image2D::ptr() ثابت { أرجع عدم; }


Timer::Timer(ثابت قطار& q) : p(عدم) {}
Timer::~Timer() {}
فراغ Timer::شروعات() { OCL_NOT_موجود(); }
فراغ Timer::stop() { OCL_NOT_موجود();}

uint64 Timer::durationNS() ثابت { OCL_NOT_موجود(); }

MatAllocator* getOpenCLAllocator() { أرجع عدم; }

internal::پروگرام_داخلا::عامل پروگرام_ذريعو&() ثابت { OCL_NOT_موجود(); }


بنية OpenCLExecutionContext::Impl
{
    Impl() = افتراضي;
};

Context& OpenCLExecutionContext::getContext() ثابت { OCL_NOT_موجود(); }
Device& OpenCLExecutionContext::getDevice() ثابت { OCL_NOT_موجود(); }
قطار& OpenCLExecutionContext::get_قطار() ثابت { OCL_NOT_موجود(); }

منطقي OpenCLExecutionContext::useOpenCL() ثابت { أرجع خطأ; }
فراغ OpenCLExecutionContext::سيٽ_Use_Open_CL(منطقي flag) { }

ساكن
OpenCLExecutionContext& getDummyOpenCLExecutionContext()
{
    ساكن OpenCLExecutionContext dummy;
    أرجع dummy;
}

/* static */
OpenCLExecutionContext& OpenCLExecutionContext::getCurrent() { أرجع getDummyOpenCLExecutionContext(); }

/* static */
OpenCLExecutionContext& OpenCLExecutionContext::getCurrentRef() { أرجع getDummyOpenCLExecutionContext(); }

فراغ OpenCLExecutionContext::bind() ثابت { OCL_NOT_موجود(); }

OpenCLExecutionContext OpenCLExecutionContext::clone_With_New_قطار(ثابت ocl::قطار& q) ثابت { OCL_NOT_موجود(); }
OpenCLExecutionContext OpenCLExecutionContext::clone_With_New_قطار() ثابت { OCL_NOT_موجود(); }

/* static */ OpenCLExecutionContext OpenCLExecutionContext::create(ثابت std::string& platformName, فراغ* platformID, فراغ* context, فراغ* deviceID) { OCL_NOT_موجود(); }
/* static */ OpenCLExecutionContext OpenCLExecutionContext::create(ثابت Context& context, ثابت Device& device, ثابت ocl::قطار& قطار_2) { OCL_NOT_موجود(); }
/* static */ OpenCLExecutionContext OpenCLExecutionContext::create(ثابت Context& context, ثابت Device& device) { OCL_NOT_موجود(); }

فراغ OpenCLExecutionContext::release() { }

}}

#إذا defined(_MSC_VER)
    #هدايت warning(pop)
#وإلاإذا defined(__clang__)
    #هدايت clang diagnostic pop
#وإلاإذا defined(__GNUC__)
    #هدايت GCC diagnostic pop
#جيڪڏهن_پورو
