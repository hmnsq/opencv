// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#شامل "opencv2/core/ocl_genbase.hpp"

#اگر defined(_MSC_VER)
    #ہدایت warning(push)
    #ہدایت warning(disable : 4100)
    #ہدایت warning(disable : 4702)
#وگرنہ_اگر defined(__clang__)
    #ہدایت clang diagnostic push
    #ہدایت clang diagnostic ignored "-Wunused-parameter"
#وگرنہ_اگر defined(__GNUC__)
    #ہدایت GCC diagnostic push
    #ہدایت GCC diagnostic ignored "-Wunused-parameter"
#اگر_ختم

نامخلا cv { نامخلا ocl {

ساکن
CV_NORETURN خالی throw_no_ocl()
{
    CV_Error(Error::OpenCLApiCallError, "OpenCV build without OpenCL support");
}
#تعریف_کرو OCL_NOT_دستیاب() throw_no_ocl();

CV_EXPORTS_W منطقی haveOpenCL() { واپس جھوٹ; }
CV_EXPORTS_W منطقی useOpenCL() { واپس جھوٹ; }
CV_EXPORTS_W منطقی haveAmdBlas() { واپس جھوٹ; }
CV_EXPORTS_W منطقی haveAmdFft() { واپس جھوٹ; }
CV_EXPORTS_W خالی سیٹ_Use_Open_CL(منطقی flag) { /* nothing */ }
CV_EXPORTS_W خالی اختتام_2() { /* nothing */ }

CV_EXPORTS منطقی haveSVM() { واپس جھوٹ; }

Device::Device() CV_NOEXCEPT : p(تہی) { }
Device::Device(خالی* d) : p(تہی) { OCL_NOT_دستیاب(); }
Device::Device(ثابت Device& d) : p(تہی) { }
Device& Device::عامل=(ثابت Device& d) { واپس *یہ; }
Device::Device(Device&&) CV_NOEXCEPT : p(تہی) { }
Device& Device::عامل=(Device&&) CV_NOEXCEPT { واپس *یہ; }
Device::~Device() { }

خالی Device::سیٹ(خالی* d) { OCL_NOT_دستیاب(); }

String Device::name() ثابت { OCL_NOT_دستیاب(); }
String Device::extensions() ثابت { OCL_NOT_دستیاب(); }
منطقی Device::isExtensionSupported(ثابت String& extensionName) ثابت { OCL_NOT_دستیاب(); }
String Device::نسخہ() ثابت { OCL_NOT_دستیاب(); }
String Device::vendorName() ثابت { OCL_NOT_دستیاب(); }
String Device::Open_CL_C_نسخہ() ثابت { OCL_NOT_دستیاب(); }
String Device::OpenCLVersion() ثابت { OCL_NOT_دستیاب(); }
صحیح Device::device_نسخہ_Major() ثابت { OCL_NOT_دستیاب(); }
صحیح Device::device_نسخہ_Minor() ثابت { OCL_NOT_دستیاب(); }
String Device::driver_نسخہ() ثابت { OCL_NOT_دستیاب(); }
خالی* Device::ptr() ثابت { /*OCL_NOT_AVAILABLE();*/ واپس تہی; }

صحیح Device::type() ثابت { OCL_NOT_دستیاب(); }

صحیح Device::پتہ_بٹ() ثابت { OCL_NOT_دستیاب(); }
منطقی Device::دستیاب() ثابت { OCL_NOT_دستیاب(); }
منطقی Device::compiler_دستیاب() ثابت { OCL_NOT_دستیاب(); }
منطقی Device::linker_دستیاب() ثابت { OCL_NOT_دستیاب(); }

صحیح Device::doubleFPConfig() ثابت { OCL_NOT_دستیاب(); }
صحیح Device::singleFPConfig() ثابت { OCL_NOT_دستیاب(); }
صحیح Device::halfFPConfig() ثابت { OCL_NOT_دستیاب(); }

منطقی Device::hasFP64() ثابت { OCL_NOT_دستیاب(); }
منطقی Device::hasFP16() ثابت { OCL_NOT_دستیاب(); }

منطقی Device::endianLittle() ثابت { OCL_NOT_دستیاب(); }
منطقی Device::errorCorrectionSupport() ثابت { OCL_NOT_دستیاب(); }

صحیح Device::executionCapabilities() ثابت { OCL_NOT_دستیاب(); }

size_t Device::global_Mem_Cache_سائز() ثابت { OCL_NOT_دستیاب(); }

صحیح Device::globalMemCacheType() ثابت { OCL_NOT_دستیاب(); }
صحیح Device::global_Mem_Cache_سطر_سائز() ثابت { OCL_NOT_دستیاب(); }
size_t Device::global_Mem_سائز() ثابت { OCL_NOT_دستیاب(); }

size_t Device::local_Mem_سائز() ثابت { OCL_NOT_دستیاب(); }
صحیح Device::localMemType() ثابت { واپس NO_LOCAL_MEM; }
منطقی Device::host_Unified_حافظہ_2() ثابت { OCL_NOT_دستیاب(); }

منطقی Device::imageSupport() ثابت { OCL_NOT_دستیاب(); }

منطقی Device::image_From_بفر_Support() ثابت { OCL_NOT_دستیاب(); }
uint Device::imagePitchAlignment() ثابت { OCL_NOT_دستیاب(); }
uint Device::image_بنیاد_پتہ_Alignment() ثابت { OCL_NOT_دستیاب(); }

منطقی Device::intelSubgroupsSupport() ثابت { OCL_NOT_دستیاب(); }

size_t Device::image2_DMax_چوڑائی() ثابت { OCL_NOT_دستیاب(); }
size_t Device::image2_DMax_اونچائی() ثابت { OCL_NOT_دستیاب(); }

size_t Device::image3_DMax_چوڑائی() ثابت { OCL_NOT_دستیاب(); }
size_t Device::image3_DMax_اونچائی() ثابت { OCL_NOT_دستیاب(); }
size_t Device::image3_DMax_گہرائی() ثابت { OCL_NOT_دستیاب(); }

size_t Device::image_Max_بفر_سائز() ثابت { OCL_NOT_دستیاب(); }
size_t Device::image_Max_Array_سائز() ثابت { OCL_NOT_دستیاب(); }

صحیح Device::vendorID() ثابت { OCL_NOT_دستیاب(); }

صحیح Device::maxClockFrequency() ثابت { OCL_NOT_دستیاب(); }
صحیح Device::maxComputeUnits() ثابت { OCL_NOT_دستیاب(); }
صحیح Device::max_Constant_دلائل() ثابت { OCL_NOT_دستیاب(); }
size_t Device::max_Constant_بفر_سائز() ثابت { OCL_NOT_دستیاب(); }

size_t Device::max_Mem_Alloc_سائز() ثابت { OCL_NOT_دستیاب(); }
size_t Device::max_Parameter_سائز() ثابت { OCL_NOT_دستیاب(); }

صحیح Device::max_Read_Image_دلائل() ثابت { OCL_NOT_دستیاب(); }
صحیح Device::max_Write_Image_دلائل() ثابت { OCL_NOT_دستیاب(); }
صحیح Device::maxSamplers() ثابت { OCL_NOT_دستیاب(); }

size_t Device::max_Work_گروہ_سائز() ثابت { OCL_NOT_دستیاب(); }
صحیح Device::max_Work_شے_Dims() ثابت { OCL_NOT_دستیاب(); }
خالی Device::max_Work_شے_Sizes(size_t*) ثابت { OCL_NOT_دستیاب(); }

صحیح Device::mem_بنیاد_Addr_Align() ثابت { OCL_NOT_دستیاب(); }

صحیح Device::native_سمتیہ_چوڑائی_Char() ثابت { OCL_NOT_دستیاب(); }
صحیح Device::native_سمتیہ_چوڑائی_Short() ثابت { OCL_NOT_دستیاب(); }
صحیح Device::native_سمتیہ_چوڑائی_Int() ثابت { OCL_NOT_دستیاب(); }
صحیح Device::native_سمتیہ_چوڑائی_Long() ثابت { OCL_NOT_دستیاب(); }
صحیح Device::native_سمتیہ_چوڑائی_Float() ثابت { OCL_NOT_دستیاب(); }
صحیح Device::native_سمتیہ_چوڑائی_Double() ثابت { OCL_NOT_دستیاب(); }
صحیح Device::native_سمتیہ_چوڑائی_Half() ثابت { OCL_NOT_دستیاب(); }

صحیح Device::preferred_سمتیہ_چوڑائی_Char() ثابت { OCL_NOT_دستیاب(); }
صحیح Device::preferred_سمتیہ_چوڑائی_Short() ثابت { OCL_NOT_دستیاب(); }
صحیح Device::preferred_سمتیہ_چوڑائی_Int() ثابت { OCL_NOT_دستیاب(); }
صحیح Device::preferred_سمتیہ_چوڑائی_Long() ثابت { OCL_NOT_دستیاب(); }
صحیح Device::preferred_سمتیہ_چوڑائی_Float() ثابت { OCL_NOT_دستیاب(); }
صحیح Device::preferred_سمتیہ_چوڑائی_Double() ثابت { OCL_NOT_دستیاب(); }
صحیح Device::preferred_سمتیہ_چوڑائی_Half() ثابت { OCL_NOT_دستیاب(); }

size_t Device::printf_بفر_سائز() ثابت { OCL_NOT_دستیاب(); }
size_t Device::profilingTimerResolution() ثابت { OCL_NOT_دستیاب(); }

/* static */
ثابت Device& Device::getDefault()
{
    ساکن Device dummy;
    واپس dummy;
}

/* static */ Device Device::fromHandle(خالی* d) { OCL_NOT_دستیاب(); }


Context::Context() CV_NOEXCEPT : p(تہی) { }
Context::Context(صحیح dtype) : p(تہی) { }
Context::~Context() { }
Context::Context(ثابت Context& c) : p(تہی) { }
Context& Context::عامل=(ثابت Context& c) { واپس *یہ; }
Context::Context(Context&&) CV_NOEXCEPT : p(تہی) { }
Context& Context::عامل=(Context&&) CV_NOEXCEPT { واپس *یہ; }

منطقی Context::create() { واپس جھوٹ; }
منطقی Context::create(صحیح dtype) { واپس جھوٹ; }
size_t Context::ndevices() ثابت { واپس 0; }
Device& Context::device(size_t idx) ثابت { OCL_NOT_دستیاب(); }
پروگرام_2 Context::getProg(ثابت پروگرام_ماخذ& prog, ثابت String& buildopt, String& errmsg) { OCL_NOT_دستیاب(); }
خالی Context::unloadProg(پروگرام_2& prog) { }

/* static */
Context& Context::getDefault()
{
    ساکن Context dummy;
    واپس dummy;
}
خالی* Context::ptr() ثابت { واپس تہی; }

خالی* Context::getOpenCLContextProperty(صحیح /*propertyId*/) ثابت { OCL_NOT_دستیاب(); }

منطقی Context::useSVM() ثابت { واپس جھوٹ; }
خالی Context::سیٹ_Use_SVM(منطقی enabled) { }

Context::UserContext::~UserContext() { }

خالی Context::سیٹ_User_Context(std::type_اشاریہ /*typeId*/, ثابت std::shared_ptr<Context::UserContext>& /*userContext*/) { OCL_NOT_دستیاب(); }
std::shared_ptr<Context::UserContext> Context::getUserContext(std::type_اشاریہ /*typeId*/) { OCL_NOT_دستیاب(); }

/* static */ Context Context::fromHandle(خالی* context) { OCL_NOT_دستیاب(); }
/* static */ Context Context::fromDevice(ثابت ocl::Device& device) { OCL_NOT_دستیاب(); }
/* static */ Context Context::create(ثابت std::string& configuration) { OCL_NOT_دستیاب(); }

خالی Context::release() { }


Platform::Platform() CV_NOEXCEPT : p(تہی) { }
Platform::~Platform() { }
Platform::Platform(ثابت Platform&) : p(تہی) { }
Platform& Platform::عامل=(ثابت Platform&) { واپس *یہ; }
Platform::Platform(Platform&&) CV_NOEXCEPT : p(تہی) { }
Platform& Platform::عامل=(Platform&&) CV_NOEXCEPT { واپس *یہ; }

خالی* Platform::ptr() ثابت { واپس تہی; }

/* static */
Platform& Platform::getDefault()
{
    ساکن Platform dummy;
    واپس dummy;
}

خالی attachContext(ثابت String& platformName, خالی* platformID, خالی* context, خالی* deviceID) { OCL_NOT_دستیاب(); }
خالی convert_From_بفر(خالی* cl_mem_بفر, size_t قدم, صحیح صفیں, صحیح ستون_تعداد, صحیح type, UMat& dst) { OCL_NOT_دستیاب(); }
خالی convertFromImage(خالی* cl_mem_image, UMat& dst) { OCL_NOT_دستیاب(); }

خالی initializeContextFromHandle(Context& ctx, خالی* platform, خالی* context, خالی* device) { OCL_NOT_دستیاب(); }

قطار::قطار() CV_NOEXCEPT : p(تہی) { }
قطار::قطار(ثابت Context& c, ثابت Device& d) : p(تہی) { OCL_NOT_دستیاب(); }
قطار::~قطار() { }
قطار::قطار(ثابت قطار& q) {}
قطار& قطار::عامل=(ثابت قطار& q) { واپس *یہ; }
قطار::قطار(قطار&&) CV_NOEXCEPT : p(تہی) { }
قطار& قطار::عامل=(قطار&&) CV_NOEXCEPT { واپس *یہ; }

منطقی قطار::create(ثابت Context& c, ثابت Device& d) { OCL_NOT_دستیاب(); }
خالی قطار::اختتام_2() {}
خالی* قطار::ptr() ثابت { واپس تہی; }
/* static */
قطار& قطار::getDefault()
{
    ساکن قطار dummy;
    واپس dummy;
}

/// @brief Returns OpenCL command queue with enable profiling mode support
ثابت قطار& قطار::get_Profiling_قطار() ثابت { OCL_NOT_دستیاب(); }


KernelArg::KernelArg() CV_NOEXCEPT
    : flags(0), m(0), obj(0), sz(0), wscale(1), iwscale(1)
{
}

KernelArg::KernelArg(صحیح _flags, UMat* _m, صحیح _wscale, صحیح _iwscale, ثابت خالی* _obj, size_t _sz)
    : flags(_flags), m(_m), obj(_obj), sz(_sz), wscale(_wscale), iwscale(_iwscale)
{
    OCL_NOT_دستیاب();
}

KernelArg KernelArg::Constant(ثابت Mat& m)
{
    OCL_NOT_دستیاب();
}


Kernel::Kernel() CV_NOEXCEPT : p(تہی) { }
Kernel::Kernel(ثابت حرف* kname, ثابت پروگرام_2& prog) : p(تہی) { OCL_NOT_دستیاب(); }
Kernel::Kernel(ثابت حرف* kname, ثابت پروگرام_ماخذ& prog, ثابت String& buildopts, String* errmsg) : p(تہی) { OCL_NOT_دستیاب(); }
Kernel::~Kernel() { }
Kernel::Kernel(ثابت Kernel& k) : p(تہی) { }
Kernel& Kernel::عامل=(ثابت Kernel& k) { واپس *یہ; }
Kernel::Kernel(Kernel&&) CV_NOEXCEPT : p(تہی) { }
Kernel& Kernel::عامل=(Kernel&&) CV_NOEXCEPT { واپس *یہ; }

منطقی Kernel::empty() ثابت { واپس سچ; }
منطقی Kernel::create(ثابت حرف* kname, ثابت پروگرام_2& prog) { OCL_NOT_دستیاب(); }
منطقی Kernel::create(ثابت حرف* kname, ثابت پروگرام_ماخذ& prog, ثابت String& buildopts, String* errmsg) { OCL_NOT_دستیاب(); }

صحیح Kernel::سیٹ(صحیح i, ثابت خالی* value, size_t sz) { OCL_NOT_دستیاب(); }
صحیح Kernel::سیٹ(صحیح i, ثابت Image2D& image2D) { OCL_NOT_دستیاب(); }
صحیح Kernel::سیٹ(صحیح i, ثابت UMat& m) { OCL_NOT_دستیاب(); }
صحیح Kernel::سیٹ(صحیح i, ثابت KernelArg& arg) { OCL_NOT_دستیاب(); }

منطقی Kernel::تسلسل(صحیح dims, size_t globalsize[], size_t localsize[], منطقی ہمآہنگ, ثابت قطار& q) { OCL_NOT_دستیاب(); }
منطقی Kernel::تسلسل_Task(منطقی ہمآہنگ, ثابت قطار& q) { OCL_NOT_دستیاب(); }

int64 Kernel::تسلسل_Profiling(صحیح dims, size_t globalsize[], size_t localsize[], ثابت قطار& q) { OCL_NOT_دستیاب(); }

size_t Kernel::work_گروہ_سائز() ثابت { OCL_NOT_دستیاب(); }
size_t Kernel::prefered_Work_گروہ_سائز_مضرب() ثابت { OCL_NOT_دستیاب(); }
منطقی Kernel::compile_Work_گروہ_سائز(size_t wsz[]) ثابت { OCL_NOT_دستیاب(); }
size_t Kernel::local_Mem_سائز() ثابت { OCL_NOT_دستیاب(); }

خالی* Kernel::ptr() ثابت { واپس تہی; }


پروگرام_2::پروگرام_2() CV_NOEXCEPT : p(تہی) { }
پروگرام_2::پروگرام_2(ثابت پروگرام_ماخذ& src, ثابت String& buildflags, String& errmsg) : p(تہی) { OCL_NOT_دستیاب(); }
پروگرام_2::پروگرام_2(ثابت پروگرام_2& prog) : p(تہی) { }
پروگرام_2& پروگرام_2::عامل=(ثابت پروگرام_2& prog) { واپس *یہ; }
پروگرام_2::پروگرام_2(پروگرام_2&&) CV_NOEXCEPT : p(تہی) { }
پروگرام_2& پروگرام_2::عامل=(پروگرام_2&&) CV_NOEXCEPT { واپس *یہ; }
پروگرام_2::~پروگرام_2() { }

منطقی پروگرام_2::create(ثابت پروگرام_ماخذ& src, ثابت String& buildflags, String& errmsg) { OCL_NOT_دستیاب(); }

خالی* پروگرام_2::ptr() ثابت { واپس تہی; }

خالی پروگرام_2::get_ثنائی(std::vector<حرف>& binary) ثابت { OCL_NOT_دستیاب(); }

منطقی پروگرام_2::اقرأ(ثابت String& buf, ثابت String& buildflags) { OCL_NOT_دستیاب(); }
منطقی پروگرام_2::اكتب(String& buf) ثابت { OCL_NOT_دستیاب(); }
ثابت پروگرام_ماخذ& پروگرام_2::ماخذ() ثابت { OCL_NOT_دستیاب(); }
String پروگرام_2::get_سابقہ() ثابت { OCL_NOT_دستیاب(); }
/* static */ String پروگرام_2::get_سابقہ(ثابت String& buildflags) { OCL_NOT_دستیاب(); }


پروگرام_ماخذ::پروگرام_ماخذ() CV_NOEXCEPT : p(تہی) { }
پروگرام_ماخذ::پروگرام_ماخذ(ثابت String& module, ثابت String& name, ثابت String& رمز_Str, ثابت String& رمز_Hash) : p(تہی) { }
پروگرام_ماخذ::پروگرام_ماخذ(ثابت String& prog) : p(تہی) { }
پروگرام_ماخذ::پروگرام_ماخذ(ثابت حرف* prog) : p(تہی) { }
پروگرام_ماخذ::~پروگرام_ماخذ() { }
پروگرام_ماخذ::پروگرام_ماخذ(ثابت پروگرام_ماخذ& prog) : p(تہی) { }
پروگرام_ماخذ& پروگرام_ماخذ::عامل=(ثابت پروگرام_ماخذ& prog) { واپس *یہ; }
پروگرام_ماخذ::پروگرام_ماخذ(پروگرام_ماخذ&&) CV_NOEXCEPT : p(تہی) { }
پروگرام_ماخذ& پروگرام_ماخذ::عامل=(پروگرام_ماخذ&&) CV_NOEXCEPT { واپس *یہ; }

ثابت String& پروگرام_ماخذ::ماخذ() ثابت { OCL_NOT_دستیاب(); }
پروگرام_ماخذ::hash_t پروگرام_ماخذ::hash() ثابت { OCL_NOT_دستیاب(); }

/* static */ پروگرام_ماخذ پروگرام_ماخذ::from_ثنائی(ثابت String& module, ثابت String& name, ثابت غیرعلامتی حرف* binary, ثابت size_t size, ثابت cv::String& buildOptions) { OCL_NOT_دستیاب(); }
/* static */ پروگرام_ماخذ پروگرام_ماخذ::fromSPIR(ثابت String& module, ثابت String& name, ثابت غیرعلامتی حرف* binary, ثابت size_t size, ثابت cv::String& buildOptions) { OCL_NOT_دستیاب(); }


PlatformInfo::PlatformInfo() CV_NOEXCEPT : p(تہی) { }
PlatformInfo::PlatformInfo(خالی* id) : p(تہی) { OCL_NOT_دستیاب(); }
PlatformInfo::~PlatformInfo() { }

PlatformInfo::PlatformInfo(ثابت PlatformInfo& i) : p(تہی) { }
PlatformInfo& PlatformInfo::عامل=(ثابت PlatformInfo& i) { واپس *یہ; }
PlatformInfo::PlatformInfo(PlatformInfo&&) CV_NOEXCEPT : p(تہی) { }
PlatformInfo& PlatformInfo::عامل=(PlatformInfo&&) CV_NOEXCEPT { واپس *یہ; }

String PlatformInfo::name() ثابت { OCL_NOT_دستیاب(); }
String PlatformInfo::vendor() ثابت { OCL_NOT_دستیاب(); }
String PlatformInfo::نسخہ() ثابت { OCL_NOT_دستیاب(); }
صحیح PlatformInfo::device_نمبر() ثابت { OCL_NOT_دستیاب(); }
خالی PlatformInfo::getDevice(Device& device, صحیح d) ثابت { OCL_NOT_دستیاب(); }

ثابت حرف* convertTypeStr(صحیح sdepth, صحیح ddepth, صحیح cn, حرف* buf) { OCL_NOT_دستیاب(); }
ثابت حرف* type_مقصد_Str(صحیح t) { OCL_NOT_دستیاب(); }
ثابت حرف* memop_Type_مقصد_Str(صحیح t) { OCL_NOT_دستیاب(); }
ثابت حرف* vecop_Type_مقصد_Str(صحیح t) { OCL_NOT_دستیاب(); }
ثابت حرف* getOpenCLErrorString(صحیح error_رمز) { OCL_NOT_دستیاب(); }
String kernel_مقصد_Str(داخلہ_Array _kernel, صحیح ddepth, ثابت حرف* name) { OCL_NOT_دستیاب(); }
خالی getPlatfomsInfo(std::vector<PlatformInfo>& platform_info) { OCL_NOT_دستیاب(); }


صحیح predict_Optimal_سمتیہ_چوڑائی(داخلہ_Array src1, داخلہ_Array src2, داخلہ_Array src3,
        داخلہ_Array src4, داخلہ_Array src5, داخلہ_Array src6,
        داخلہ_Array src7, داخلہ_Array src8, داخلہ_Array src9,
        Ocl_سمتیہ_Strategy strat)
{ OCL_NOT_دستیاب(); }

صحیح check_Optimal_سمتیہ_چوڑائی(ثابت صحیح *سمتیہ_Widths,
        داخلہ_Array src1, داخلہ_Array src2, داخلہ_Array src3,
        داخلہ_Array src4, داخلہ_Array src5, داخلہ_Array src6,
        داخلہ_Array src7, داخلہ_Array src8, داخلہ_Array src9,
        Ocl_سمتیہ_Strategy strat)
{ OCL_NOT_دستیاب(); }

صحیح predict_Optimal_سمتیہ_چوڑائی_Max(داخلہ_Array src1, داخلہ_Array src2, داخلہ_Array src3,
        داخلہ_Array src4, داخلہ_Array src5, داخلہ_Array src6,
        داخلہ_Array src7, داخلہ_Array src8, داخلہ_Array src9)
{ OCL_NOT_دستیاب(); }

خالی build_Options_جوڑو_میٹرکس_Description(String& buildOptions, ثابت String& name, داخلہ_Array _m) { OCL_NOT_دستیاب(); }


Image2D::Image2D() CV_NOEXCEPT : p(تہی) { }
Image2D::Image2D(ثابت UMat &src, منطقی norm, منطقی alias) { OCL_NOT_دستیاب(); }
Image2D::Image2D(ثابت Image2D & i) : p(تہی) { OCL_NOT_دستیاب(); }
Image2D::~Image2D() { }
Image2D& Image2D::عامل=(ثابت Image2D & i) { واپس *یہ; }
Image2D::Image2D(Image2D&&) CV_NOEXCEPT : p(تہی) { }
Image2D& Image2D::عامل=(Image2D&&) CV_NOEXCEPT { واپس *یہ; }

/* static */ منطقی Image2D::canCreateAlias(ثابت UMat &u) { OCL_NOT_دستیاب(); }
/* static */ منطقی Image2D::isFormatSupported(صحیح گہرائی, صحیح cn, منطقی norm) { OCL_NOT_دستیاب(); }

خالی* Image2D::ptr() ثابت { واپس تہی; }


Timer::Timer(ثابت قطار& q) : p(تہی) {}
Timer::~Timer() {}
خالی Timer::آغاز() { OCL_NOT_دستیاب(); }
خالی Timer::stop() { OCL_NOT_دستیاب();}

uint64 Timer::durationNS() ثابت { OCL_NOT_دستیاب(); }

MatAllocator* getOpenCLAllocator() { واپس تہی; }

internal::پروگرام_اندراج::عامل پروگرام_ماخذ&() ثابت { OCL_NOT_دستیاب(); }


ساخت OpenCLExecutionContext::Impl
{
    Impl() = طےشدہ;
};

Context& OpenCLExecutionContext::getContext() ثابت { OCL_NOT_دستیاب(); }
Device& OpenCLExecutionContext::getDevice() ثابت { OCL_NOT_دستیاب(); }
قطار& OpenCLExecutionContext::get_قطار() ثابت { OCL_NOT_دستیاب(); }

منطقی OpenCLExecutionContext::useOpenCL() ثابت { واپس جھوٹ; }
خالی OpenCLExecutionContext::سیٹ_Use_Open_CL(منطقی flag) { }

ساکن
OpenCLExecutionContext& getDummyOpenCLExecutionContext()
{
    ساکن OpenCLExecutionContext dummy;
    واپس dummy;
}

/* static */
OpenCLExecutionContext& OpenCLExecutionContext::getCurrent() { واپس getDummyOpenCLExecutionContext(); }

/* static */
OpenCLExecutionContext& OpenCLExecutionContext::getCurrentRef() { واپس getDummyOpenCLExecutionContext(); }

خالی OpenCLExecutionContext::bind() ثابت { OCL_NOT_دستیاب(); }

OpenCLExecutionContext OpenCLExecutionContext::clone_With_New_قطار(ثابت ocl::قطار& q) ثابت { OCL_NOT_دستیاب(); }
OpenCLExecutionContext OpenCLExecutionContext::clone_With_New_قطار() ثابت { OCL_NOT_دستیاب(); }

/* static */ OpenCLExecutionContext OpenCLExecutionContext::create(ثابت std::string& platformName, خالی* platformID, خالی* context, خالی* deviceID) { OCL_NOT_دستیاب(); }
/* static */ OpenCLExecutionContext OpenCLExecutionContext::create(ثابت Context& context, ثابت Device& device, ثابت ocl::قطار& قطار_2) { OCL_NOT_دستیاب(); }
/* static */ OpenCLExecutionContext OpenCLExecutionContext::create(ثابت Context& context, ثابت Device& device) { OCL_NOT_دستیاب(); }

خالی OpenCLExecutionContext::release() { }

}}

#اگر defined(_MSC_VER)
    #ہدایت warning(pop)
#وگرنہ_اگر defined(__clang__)
    #ہدایت clang diagnostic pop
#وگرنہ_اگر defined(__GNUC__)
    #ہدایت GCC diagnostic pop
#اگر_ختم
