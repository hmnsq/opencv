// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ضمن "opencv2/core/ocl_genbase.hpp"

#إذا defined(_MSC_VER)
    #ہدایت warning(push)
    #ہدایت warning(disable : 4100)
    #ہدایت warning(disable : 4702)
#وإلاإذا defined(__clang__)
    #ہدایت clang diagnostic push
    #ہدایت clang diagnostic ignored "-Wunused-parameter"
#وإلاإذا defined(__GNUC__)
    #ہدایت GCC diagnostic push
    #ہدایت GCC diagnostic ignored "-Wunused-parameter"
#جے_انت

نطاق cv { نطاق ocl {

ساكن
CV_NORETURN فراغ throw_no_ocl()
{
    CV_Error(Error::OpenCLApiCallError, "OpenCV build without OpenCL support");
}
#تعریف_کرو OCL_NOT_دستیاب() throw_no_ocl();

CV_EXPORTS_W منطقي haveOpenCL() { أرجع خطأ; }
CV_EXPORTS_W منطقي useOpenCL() { أرجع خطأ; }
CV_EXPORTS_W منطقي haveAmdBlas() { أرجع خطأ; }
CV_EXPORTS_W منطقي haveAmdFft() { أرجع خطأ; }
CV_EXPORTS_W فراغ سیٹ_Use_Open_CL(منطقي flag) { /* nothing */ }
CV_EXPORTS_W فراغ انت_2() { /* nothing */ }

CV_EXPORTS منطقي haveSVM() { أرجع خطأ; }

Device::Device() CV_NOEXCEPT : p(عدم) { }
Device::Device(فراغ* d) : p(عدم) { OCL_NOT_دستیاب(); }
Device::Device(ثابت Device& d) : p(عدم) { }
Device& Device::عامل=(ثابت Device& d) { أرجع *هذا; }
Device::Device(Device&&) CV_NOEXCEPT : p(عدم) { }
Device& Device::عامل=(Device&&) CV_NOEXCEPT { أرجع *هذا; }
Device::~Device() { }

فراغ Device::سیٹ(فراغ* d) { OCL_NOT_دستیاب(); }

String Device::name() ثابت { OCL_NOT_دستیاب(); }
String Device::extensions() ثابت { OCL_NOT_دستیاب(); }
منطقي Device::isExtensionSupported(ثابت String& extensionName) ثابت { OCL_NOT_دستیاب(); }
String Device::نسخہ() ثابت { OCL_NOT_دستیاب(); }
String Device::vendorName() ثابت { OCL_NOT_دستیاب(); }
String Device::Open_CL_C_نسخہ() ثابت { OCL_NOT_دستیاب(); }
String Device::OpenCLVersion() ثابت { OCL_NOT_دستیاب(); }
صحيح Device::device_نسخہ_Major() ثابت { OCL_NOT_دستیاب(); }
صحيح Device::device_نسخہ_Minor() ثابت { OCL_NOT_دستیاب(); }
String Device::driver_نسخہ() ثابت { OCL_NOT_دستیاب(); }
فراغ* Device::ptr() ثابت { /*OCL_NOT_AVAILABLE();*/ أرجع عدم; }

صحيح Device::type() ثابت { OCL_NOT_دستیاب(); }

صحيح Device::پتہ_بٹ() ثابت { OCL_NOT_دستیاب(); }
منطقي Device::دستیاب() ثابت { OCL_NOT_دستیاب(); }
منطقي Device::compiler_دستیاب() ثابت { OCL_NOT_دستیاب(); }
منطقي Device::linker_دستیاب() ثابت { OCL_NOT_دستیاب(); }

صحيح Device::doubleFPConfig() ثابت { OCL_NOT_دستیاب(); }
صحيح Device::singleFPConfig() ثابت { OCL_NOT_دستیاب(); }
صحيح Device::halfFPConfig() ثابت { OCL_NOT_دستیاب(); }

منطقي Device::hasFP64() ثابت { OCL_NOT_دستیاب(); }
منطقي Device::hasFP16() ثابت { OCL_NOT_دستیاب(); }

منطقي Device::endianLittle() ثابت { OCL_NOT_دستیاب(); }
منطقي Device::errorCorrectionSupport() ثابت { OCL_NOT_دستیاب(); }

صحيح Device::executionCapabilities() ثابت { OCL_NOT_دستیاب(); }

size_t Device::global_Mem_Cache_ناپ() ثابت { OCL_NOT_دستیاب(); }

صحيح Device::globalMemCacheType() ثابت { OCL_NOT_دستیاب(); }
صحيح Device::global_Mem_Cache_سطر_ناپ() ثابت { OCL_NOT_دستیاب(); }
size_t Device::global_Mem_ناپ() ثابت { OCL_NOT_دستیاب(); }

size_t Device::local_Mem_ناپ() ثابت { OCL_NOT_دستیاب(); }
صحيح Device::localMemType() ثابت { أرجع NO_LOCAL_MEM; }
منطقي Device::host_Unified_حافظہ_2() ثابت { OCL_NOT_دستیاب(); }

منطقي Device::imageSupport() ثابت { OCL_NOT_دستیاب(); }

منطقي Device::image_From_بفر_Support() ثابت { OCL_NOT_دستیاب(); }
uint Device::imagePitchAlignment() ثابت { OCL_NOT_دستیاب(); }
uint Device::image_ادھار_پتہ_Alignment() ثابت { OCL_NOT_دستیاب(); }

منطقي Device::intelSubgroupsSupport() ثابت { OCL_NOT_دستیاب(); }

size_t Device::image2_DMax_چوڑائی() ثابت { OCL_NOT_دستیاب(); }
size_t Device::image2_DMax_اچائی() ثابت { OCL_NOT_دستیاب(); }

size_t Device::image3_DMax_چوڑائی() ثابت { OCL_NOT_دستیاب(); }
size_t Device::image3_DMax_اچائی() ثابت { OCL_NOT_دستیاب(); }
size_t Device::image3_DMax_ڈونگھائی() ثابت { OCL_NOT_دستیاب(); }

size_t Device::image_Max_بفر_ناپ() ثابت { OCL_NOT_دستیاب(); }
size_t Device::image_Max_Array_ناپ() ثابت { OCL_NOT_دستیاب(); }

صحيح Device::vendorID() ثابت { OCL_NOT_دستیاب(); }

صحيح Device::maxClockFrequency() ثابت { OCL_NOT_دستیاب(); }
صحيح Device::maxComputeUnits() ثابت { OCL_NOT_دستیاب(); }
صحيح Device::max_Constant_دلیلاں() ثابت { OCL_NOT_دستیاب(); }
size_t Device::max_Constant_بفر_ناپ() ثابت { OCL_NOT_دستیاب(); }

size_t Device::max_Mem_Alloc_ناپ() ثابت { OCL_NOT_دستیاب(); }
size_t Device::max_Parameter_ناپ() ثابت { OCL_NOT_دستیاب(); }

صحيح Device::max_Read_Image_دلیلاں() ثابت { OCL_NOT_دستیاب(); }
صحيح Device::max_Write_Image_دلیلاں() ثابت { OCL_NOT_دستیاب(); }
صحيح Device::maxSamplers() ثابت { OCL_NOT_دستیاب(); }

size_t Device::max_Work_گروہ_ناپ() ثابت { OCL_NOT_دستیاب(); }
صحيح Device::max_Work_شے_Dims() ثابت { OCL_NOT_دستیاب(); }
فراغ Device::max_Work_شے_Sizes(size_t*) ثابت { OCL_NOT_دستیاب(); }

صحيح Device::mem_ادھار_Addr_Align() ثابت { OCL_NOT_دستیاب(); }

صحيح Device::native_سمتیہ_چوڑائی_Char() ثابت { OCL_NOT_دستیاب(); }
صحيح Device::native_سمتیہ_چوڑائی_Short() ثابت { OCL_NOT_دستیاب(); }
صحيح Device::native_سمتیہ_چوڑائی_Int() ثابت { OCL_NOT_دستیاب(); }
صحيح Device::native_سمتیہ_چوڑائی_Long() ثابت { OCL_NOT_دستیاب(); }
صحيح Device::native_سمتیہ_چوڑائی_Float() ثابت { OCL_NOT_دستیاب(); }
صحيح Device::native_سمتیہ_چوڑائی_Double() ثابت { OCL_NOT_دستیاب(); }
صحيح Device::native_سمتیہ_چوڑائی_Half() ثابت { OCL_NOT_دستیاب(); }

صحيح Device::preferred_سمتیہ_چوڑائی_Char() ثابت { OCL_NOT_دستیاب(); }
صحيح Device::preferred_سمتیہ_چوڑائی_Short() ثابت { OCL_NOT_دستیاب(); }
صحيح Device::preferred_سمتیہ_چوڑائی_Int() ثابت { OCL_NOT_دستیاب(); }
صحيح Device::preferred_سمتیہ_چوڑائی_Long() ثابت { OCL_NOT_دستیاب(); }
صحيح Device::preferred_سمتیہ_چوڑائی_Float() ثابت { OCL_NOT_دستیاب(); }
صحيح Device::preferred_سمتیہ_چوڑائی_Double() ثابت { OCL_NOT_دستیاب(); }
صحيح Device::preferred_سمتیہ_چوڑائی_Half() ثابت { OCL_NOT_دستیاب(); }

size_t Device::printf_بفر_ناپ() ثابت { OCL_NOT_دستیاب(); }
size_t Device::profilingTimerResolution() ثابت { OCL_NOT_دستیاب(); }

/* static */
ثابت Device& Device::getDefault()
{
    ساكن Device dummy;
    أرجع dummy;
}

/* static */ Device Device::fromHandle(فراغ* d) { OCL_NOT_دستیاب(); }


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
Device& Context::device(size_t idx) ثابت { OCL_NOT_دستیاب(); }
پروگرام_2 Context::getProg(ثابت پروگرام_سروت& prog, ثابت String& buildopt, String& errmsg) { OCL_NOT_دستیاب(); }
فراغ Context::unloadProg(پروگرام_2& prog) { }

/* static */
Context& Context::getDefault()
{
    ساكن Context dummy;
    أرجع dummy;
}
فراغ* Context::ptr() ثابت { أرجع عدم; }

فراغ* Context::getOpenCLContextProperty(صحيح /*propertyId*/) ثابت { OCL_NOT_دستیاب(); }

منطقي Context::useSVM() ثابت { أرجع خطأ; }
فراغ Context::سیٹ_Use_SVM(منطقي enabled) { }

Context::UserContext::~UserContext() { }

فراغ Context::سیٹ_User_Context(std::type_اشاریہ /*typeId*/, ثابت std::shared_ptr<Context::UserContext>& /*userContext*/) { OCL_NOT_دستیاب(); }
std::shared_ptr<Context::UserContext> Context::getUserContext(std::type_اشاریہ /*typeId*/) { OCL_NOT_دستیاب(); }

/* static */ Context Context::fromHandle(فراغ* context) { OCL_NOT_دستیاب(); }
/* static */ Context Context::fromDevice(ثابت ocl::Device& device) { OCL_NOT_دستیاب(); }
/* static */ Context Context::create(ثابت std::string& configuration) { OCL_NOT_دستیاب(); }

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

فراغ attachContext(ثابت String& platformName, فراغ* platformID, فراغ* context, فراغ* deviceID) { OCL_NOT_دستیاب(); }
فراغ convert_From_بفر(فراغ* cl_mem_بفر, size_t قدم, صحيح صفاں, صحيح خانے_گنتی, صحيح type, UMat& dst) { OCL_NOT_دستیاب(); }
فراغ convertFromImage(فراغ* cl_mem_image, UMat& dst) { OCL_NOT_دستیاب(); }

فراغ initializeContextFromHandle(Context& ctx, فراغ* platform, فراغ* context, فراغ* device) { OCL_NOT_دستیاب(); }

قطار::قطار() CV_NOEXCEPT : p(عدم) { }
قطار::قطار(ثابت Context& c, ثابت Device& d) : p(عدم) { OCL_NOT_دستیاب(); }
قطار::~قطار() { }
قطار::قطار(ثابت قطار& q) {}
قطار& قطار::عامل=(ثابت قطار& q) { أرجع *هذا; }
قطار::قطار(قطار&&) CV_NOEXCEPT : p(عدم) { }
قطار& قطار::عامل=(قطار&&) CV_NOEXCEPT { أرجع *هذا; }

منطقي قطار::create(ثابت Context& c, ثابت Device& d) { OCL_NOT_دستیاب(); }
فراغ قطار::انت_2() {}
فراغ* قطار::ptr() ثابت { أرجع عدم; }
/* static */
قطار& قطار::getDefault()
{
    ساكن قطار dummy;
    أرجع dummy;
}

/// @brief Returns OpenCL command queue with enable profiling mode support
ثابت قطار& قطار::get_Profiling_قطار() ثابت { OCL_NOT_دستیاب(); }


KernelArg::KernelArg() CV_NOEXCEPT
    : flags(0), m(0), obj(0), sz(0), wscale(1), iwscale(1)
{
}

KernelArg::KernelArg(صحيح _flags, UMat* _m, صحيح _wscale, صحيح _iwscale, ثابت فراغ* _obj, size_t _sz)
    : flags(_flags), m(_m), obj(_obj), sz(_sz), wscale(_wscale), iwscale(_iwscale)
{
    OCL_NOT_دستیاب();
}

KernelArg KernelArg::Constant(ثابت Mat& m)
{
    OCL_NOT_دستیاب();
}


Kernel::Kernel() CV_NOEXCEPT : p(عدم) { }
Kernel::Kernel(ثابت حرف* kname, ثابت پروگرام_2& prog) : p(عدم) { OCL_NOT_دستیاب(); }
Kernel::Kernel(ثابت حرف* kname, ثابت پروگرام_سروت& prog, ثابت String& buildopts, String* errmsg) : p(عدم) { OCL_NOT_دستیاب(); }
Kernel::~Kernel() { }
Kernel::Kernel(ثابت Kernel& k) : p(عدم) { }
Kernel& Kernel::عامل=(ثابت Kernel& k) { أرجع *هذا; }
Kernel::Kernel(Kernel&&) CV_NOEXCEPT : p(عدم) { }
Kernel& Kernel::عامل=(Kernel&&) CV_NOEXCEPT { أرجع *هذا; }

منطقي Kernel::empty() ثابت { أرجع سچ; }
منطقي Kernel::create(ثابت حرف* kname, ثابت پروگرام_2& prog) { OCL_NOT_دستیاب(); }
منطقي Kernel::create(ثابت حرف* kname, ثابت پروگرام_سروت& prog, ثابت String& buildopts, String* errmsg) { OCL_NOT_دستیاب(); }

صحيح Kernel::سیٹ(صحيح i, ثابت فراغ* value, size_t sz) { OCL_NOT_دستیاب(); }
صحيح Kernel::سیٹ(صحيح i, ثابت Image2D& image2D) { OCL_NOT_دستیاب(); }
صحيح Kernel::سیٹ(صحيح i, ثابت UMat& m) { OCL_NOT_دستیاب(); }
صحيح Kernel::سیٹ(صحيح i, ثابت KernelArg& arg) { OCL_NOT_دستیاب(); }

منطقي Kernel::لگاتار(صحيح dims, size_t globalsize[], size_t localsize[], منطقي متزامن, ثابت قطار& q) { OCL_NOT_دستیاب(); }
منطقي Kernel::لگاتار_Task(منطقي متزامن, ثابت قطار& q) { OCL_NOT_دستیاب(); }

int64 Kernel::لگاتار_Profiling(صحيح dims, size_t globalsize[], size_t localsize[], ثابت قطار& q) { OCL_NOT_دستیاب(); }

size_t Kernel::work_گروہ_ناپ() ثابت { OCL_NOT_دستیاب(); }
size_t Kernel::prefered_Work_گروہ_ناپ_مضرب() ثابت { OCL_NOT_دستیاب(); }
منطقي Kernel::compile_Work_گروہ_ناپ(size_t wsz[]) ثابت { OCL_NOT_دستیاب(); }
size_t Kernel::local_Mem_ناپ() ثابت { OCL_NOT_دستیاب(); }

فراغ* Kernel::ptr() ثابت { أرجع عدم; }


پروگرام_2::پروگرام_2() CV_NOEXCEPT : p(عدم) { }
پروگرام_2::پروگرام_2(ثابت پروگرام_سروت& src, ثابت String& buildflags, String& errmsg) : p(عدم) { OCL_NOT_دستیاب(); }
پروگرام_2::پروگرام_2(ثابت پروگرام_2& prog) : p(عدم) { }
پروگرام_2& پروگرام_2::عامل=(ثابت پروگرام_2& prog) { أرجع *هذا; }
پروگرام_2::پروگرام_2(پروگرام_2&&) CV_NOEXCEPT : p(عدم) { }
پروگرام_2& پروگرام_2::عامل=(پروگرام_2&&) CV_NOEXCEPT { أرجع *هذا; }
پروگرام_2::~پروگرام_2() { }

منطقي پروگرام_2::create(ثابت پروگرام_سروت& src, ثابت String& buildflags, String& errmsg) { OCL_NOT_دستیاب(); }

فراغ* پروگرام_2::ptr() ثابت { أرجع عدم; }

فراغ پروگرام_2::get_ثنائی(std::vector<حرف>& binary) ثابت { OCL_NOT_دستیاب(); }

منطقي پروگرام_2::اقرأ(ثابت String& buf, ثابت String& buildflags) { OCL_NOT_دستیاب(); }
منطقي پروگرام_2::اكتب(String& buf) ثابت { OCL_NOT_دستیاب(); }
ثابت پروگرام_سروت& پروگرام_2::سروت() ثابت { OCL_NOT_دستیاب(); }
String پروگرام_2::get_اگیتر() ثابت { OCL_NOT_دستیاب(); }
/* static */ String پروگرام_2::get_اگیتر(ثابت String& buildflags) { OCL_NOT_دستیاب(); }


پروگرام_سروت::پروگرام_سروت() CV_NOEXCEPT : p(عدم) { }
پروگرام_سروت::پروگرام_سروت(ثابت String& module, ثابت String& name, ثابت String& کوڈ_Str, ثابت String& کوڈ_Hash) : p(عدم) { }
پروگرام_سروت::پروگرام_سروت(ثابت String& prog) : p(عدم) { }
پروگرام_سروت::پروگرام_سروت(ثابت حرف* prog) : p(عدم) { }
پروگرام_سروت::~پروگرام_سروت() { }
پروگرام_سروت::پروگرام_سروت(ثابت پروگرام_سروت& prog) : p(عدم) { }
پروگرام_سروت& پروگرام_سروت::عامل=(ثابت پروگرام_سروت& prog) { أرجع *هذا; }
پروگرام_سروت::پروگرام_سروت(پروگرام_سروت&&) CV_NOEXCEPT : p(عدم) { }
پروگرام_سروت& پروگرام_سروت::عامل=(پروگرام_سروت&&) CV_NOEXCEPT { أرجع *هذا; }

ثابت String& پروگرام_سروت::سروت() ثابت { OCL_NOT_دستیاب(); }
پروگرام_سروت::hash_t پروگرام_سروت::hash() ثابت { OCL_NOT_دستیاب(); }

/* static */ پروگرام_سروت پروگرام_سروت::from_ثنائی(ثابت String& module, ثابت String& name, ثابت بنانشانی حرف* binary, ثابت size_t size, ثابت cv::String& buildOptions) { OCL_NOT_دستیاب(); }
/* static */ پروگرام_سروت پروگرام_سروت::fromSPIR(ثابت String& module, ثابت String& name, ثابت بنانشانی حرف* binary, ثابت size_t size, ثابت cv::String& buildOptions) { OCL_NOT_دستیاب(); }


PlatformInfo::PlatformInfo() CV_NOEXCEPT : p(عدم) { }
PlatformInfo::PlatformInfo(فراغ* id) : p(عدم) { OCL_NOT_دستیاب(); }
PlatformInfo::~PlatformInfo() { }

PlatformInfo::PlatformInfo(ثابت PlatformInfo& i) : p(عدم) { }
PlatformInfo& PlatformInfo::عامل=(ثابت PlatformInfo& i) { أرجع *هذا; }
PlatformInfo::PlatformInfo(PlatformInfo&&) CV_NOEXCEPT : p(عدم) { }
PlatformInfo& PlatformInfo::عامل=(PlatformInfo&&) CV_NOEXCEPT { أرجع *هذا; }

String PlatformInfo::name() ثابت { OCL_NOT_دستیاب(); }
String PlatformInfo::vendor() ثابت { OCL_NOT_دستیاب(); }
String PlatformInfo::نسخہ() ثابت { OCL_NOT_دستیاب(); }
صحيح PlatformInfo::device_نمبر() ثابت { OCL_NOT_دستیاب(); }
فراغ PlatformInfo::getDevice(Device& device, صحيح d) ثابت { OCL_NOT_دستیاب(); }

ثابت حرف* convertTypeStr(صحيح sdepth, صحيح ddepth, صحيح cn, حرف* buf) { OCL_NOT_دستیاب(); }
ثابت حرف* type_منزل_Str(صحيح t) { OCL_NOT_دستیاب(); }
ثابت حرف* memop_Type_منزل_Str(صحيح t) { OCL_NOT_دستیاب(); }
ثابت حرف* vecop_Type_منزل_Str(صحيح t) { OCL_NOT_دستیاب(); }
ثابت حرف* getOpenCLErrorString(صحيح error_کوڈ) { OCL_NOT_دستیاب(); }
String kernel_منزل_Str(داخلہ_Array _kernel, صحيح ddepth, ثابت حرف* name) { OCL_NOT_دستیاب(); }
فراغ getPlatfomsInfo(std::vector<PlatformInfo>& platform_info) { OCL_NOT_دستیاب(); }


صحيح predict_Optimal_سمتیہ_چوڑائی(داخلہ_Array src1, داخلہ_Array src2, داخلہ_Array src3,
        داخلہ_Array src4, داخلہ_Array src5, داخلہ_Array src6,
        داخلہ_Array src7, داخلہ_Array src8, داخلہ_Array src9,
        Ocl_سمتیہ_Strategy strat)
{ OCL_NOT_دستیاب(); }

صحيح check_Optimal_سمتیہ_چوڑائی(ثابت صحيح *سمتیہ_Widths,
        داخلہ_Array src1, داخلہ_Array src2, داخلہ_Array src3,
        داخلہ_Array src4, داخلہ_Array src5, داخلہ_Array src6,
        داخلہ_Array src7, داخلہ_Array src8, داخلہ_Array src9,
        Ocl_سمتیہ_Strategy strat)
{ OCL_NOT_دستیاب(); }

صحيح predict_Optimal_سمتیہ_چوڑائی_Max(داخلہ_Array src1, داخلہ_Array src2, داخلہ_Array src3,
        داخلہ_Array src4, داخلہ_Array src5, داخلہ_Array src6,
        داخلہ_Array src7, داخلہ_Array src8, داخلہ_Array src9)
{ OCL_NOT_دستیاب(); }

فراغ build_Options_جوڑو_میٹرکس_Description(String& buildOptions, ثابت String& name, داخلہ_Array _m) { OCL_NOT_دستیاب(); }


Image2D::Image2D() CV_NOEXCEPT : p(عدم) { }
Image2D::Image2D(ثابت UMat &src, منطقي norm, منطقي alias) { OCL_NOT_دستیاب(); }
Image2D::Image2D(ثابت Image2D & i) : p(عدم) { OCL_NOT_دستیاب(); }
Image2D::~Image2D() { }
Image2D& Image2D::عامل=(ثابت Image2D & i) { أرجع *هذا; }
Image2D::Image2D(Image2D&&) CV_NOEXCEPT : p(عدم) { }
Image2D& Image2D::عامل=(Image2D&&) CV_NOEXCEPT { أرجع *هذا; }

/* static */ منطقي Image2D::canCreateAlias(ثابت UMat &u) { OCL_NOT_دستیاب(); }
/* static */ منطقي Image2D::isFormatSupported(صحيح ڈونگھائی, صحيح cn, منطقي norm) { OCL_NOT_دستیاب(); }

فراغ* Image2D::ptr() ثابت { أرجع عدم; }


Timer::Timer(ثابت قطار& q) : p(عدم) {}
Timer::~Timer() {}
فراغ Timer::شروع() { OCL_NOT_دستیاب(); }
فراغ Timer::stop() { OCL_NOT_دستیاب();}

uint64 Timer::durationNS() ثابت { OCL_NOT_دستیاب(); }

MatAllocator* getOpenCLAllocator() { أرجع عدم; }

internal::پروگرام_اندراج::عامل پروگرام_سروت&() ثابت { OCL_NOT_دستیاب(); }


بنية OpenCLExecutionContext::Impl
{
    Impl() = افتراضي;
};

Context& OpenCLExecutionContext::getContext() ثابت { OCL_NOT_دستیاب(); }
Device& OpenCLExecutionContext::getDevice() ثابت { OCL_NOT_دستیاب(); }
قطار& OpenCLExecutionContext::get_قطار() ثابت { OCL_NOT_دستیاب(); }

منطقي OpenCLExecutionContext::useOpenCL() ثابت { أرجع خطأ; }
فراغ OpenCLExecutionContext::سیٹ_Use_Open_CL(منطقي flag) { }

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

فراغ OpenCLExecutionContext::bind() ثابت { OCL_NOT_دستیاب(); }

OpenCLExecutionContext OpenCLExecutionContext::clone_With_New_قطار(ثابت ocl::قطار& q) ثابت { OCL_NOT_دستیاب(); }
OpenCLExecutionContext OpenCLExecutionContext::clone_With_New_قطار() ثابت { OCL_NOT_دستیاب(); }

/* static */ OpenCLExecutionContext OpenCLExecutionContext::create(ثابت std::string& platformName, فراغ* platformID, فراغ* context, فراغ* deviceID) { OCL_NOT_دستیاب(); }
/* static */ OpenCLExecutionContext OpenCLExecutionContext::create(ثابت Context& context, ثابت Device& device, ثابت ocl::قطار& قطار_2) { OCL_NOT_دستیاب(); }
/* static */ OpenCLExecutionContext OpenCLExecutionContext::create(ثابت Context& context, ثابت Device& device) { OCL_NOT_دستیاب(); }

فراغ OpenCLExecutionContext::release() { }

}}

#إذا defined(_MSC_VER)
    #ہدایت warning(pop)
#وإلاإذا defined(__clang__)
    #ہدایت clang diagnostic pop
#وإلاإذا defined(__GNUC__)
    #ہدایت GCC diagnostic pop
#جے_انت
