// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ضمن "opencv2/core/ocl_genbase.hpp"

#إذا defined(_MSC_VER)
    #رهنمود warning(push)
    #رهنمود warning(disable : 4100)
    #رهنمود warning(disable : 4702)
#وإلاإذا defined(__clang__)
    #رهنمود clang diagnostic push
    #رهنمود clang diagnostic ignored "-Wunused-parameter"
#وإلاإذا defined(__GNUC__)
    #رهنمود GCC diagnostic push
    #رهنمود GCC diagnostic ignored "-Wunused-parameter"
#پایان_اگر

نطاق cv { نطاق ocl {

ساكن
CV_NORETURN فراغ throw_no_ocl()
{
    CV_Error(Error::OpenCLApiCallError, "OpenCV build without OpenCL support");
}
#تعریف_کن OCL_NOT_موجود() throw_no_ocl();

CV_EXPORTS_W منطقي haveOpenCL() { أرجع خطأ; }
CV_EXPORTS_W منطقي useOpenCL() { أرجع خطأ; }
CV_EXPORTS_W منطقي haveAmdBlas() { أرجع خطأ; }
CV_EXPORTS_W منطقي haveAmdFft() { أرجع خطأ; }
CV_EXPORTS_W فراغ مجموعه_Use_Open_CL(منطقي flag) { /* nothing */ }
CV_EXPORTS_W فراغ پایان_2() { /* nothing */ }

CV_EXPORTS منطقي haveSVM() { أرجع خطأ; }

Device::Device() CV_NOEXCEPT : p(عدم) { }
Device::Device(فراغ* d) : p(عدم) { OCL_NOT_موجود(); }
Device::Device(ثابت Device& d) : p(عدم) { }
Device& Device::عملگر=(ثابت Device& d) { أرجع *هذا; }
Device::Device(Device&&) CV_NOEXCEPT : p(عدم) { }
Device& Device::عملگر=(Device&&) CV_NOEXCEPT { أرجع *هذا; }
Device::~Device() { }

فراغ Device::مجموعه(فراغ* d) { OCL_NOT_موجود(); }

String Device::name() ثابت { OCL_NOT_موجود(); }
String Device::extensions() ثابت { OCL_NOT_موجود(); }
منطقي Device::isExtensionSupported(ثابت String& extensionName) ثابت { OCL_NOT_موجود(); }
String Device::نسخه() ثابت { OCL_NOT_موجود(); }
String Device::vendorName() ثابت { OCL_NOT_موجود(); }
String Device::Open_CL_C_نسخه() ثابت { OCL_NOT_موجود(); }
String Device::OpenCLVersion() ثابت { OCL_NOT_موجود(); }
صحيح Device::device_نسخه_Major() ثابت { OCL_NOT_موجود(); }
صحيح Device::device_نسخه_Minor() ثابت { OCL_NOT_موجود(); }
String Device::driver_نسخه() ثابت { OCL_NOT_موجود(); }
فراغ* Device::ptr() ثابت { /*OCL_NOT_AVAILABLE();*/ أرجع عدم; }

صحيح Device::type() ثابت { OCL_NOT_موجود(); }

صحيح Device::آدرس_بیتها() ثابت { OCL_NOT_موجود(); }
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

size_t Device::global_Mem_Cache_اندازه() ثابت { OCL_NOT_موجود(); }

صحيح Device::globalMemCacheType() ثابت { OCL_NOT_موجود(); }
صحيح Device::global_Mem_Cache_سطر_اندازه() ثابت { OCL_NOT_موجود(); }
size_t Device::global_Mem_اندازه() ثابت { OCL_NOT_موجود(); }

size_t Device::local_Mem_اندازه() ثابت { OCL_NOT_موجود(); }
صحيح Device::localMemType() ثابت { أرجع NO_LOCAL_MEM; }
منطقي Device::host_Unified_حافظه_2() ثابت { OCL_NOT_موجود(); }

منطقي Device::imageSupport() ثابت { OCL_NOT_موجود(); }

منطقي Device::image_From_میانگیر_Support() ثابت { OCL_NOT_موجود(); }
uint Device::imagePitchAlignment() ثابت { OCL_NOT_موجود(); }
uint Device::image_پایه_آدرس_Alignment() ثابت { OCL_NOT_موجود(); }

منطقي Device::intelSubgroupsSupport() ثابت { OCL_NOT_موجود(); }

size_t Device::image2_DMax_عرض() ثابت { OCL_NOT_موجود(); }
size_t Device::image2_DMax_ارتفاع() ثابت { OCL_NOT_موجود(); }

size_t Device::image3_DMax_عرض() ثابت { OCL_NOT_موجود(); }
size_t Device::image3_DMax_ارتفاع() ثابت { OCL_NOT_موجود(); }
size_t Device::image3_DMax_عمق() ثابت { OCL_NOT_موجود(); }

size_t Device::image_Max_میانگیر_اندازه() ثابت { OCL_NOT_موجود(); }
size_t Device::image_Max_Array_اندازه() ثابت { OCL_NOT_موجود(); }

صحيح Device::vendorID() ثابت { OCL_NOT_موجود(); }

صحيح Device::maxClockFrequency() ثابت { OCL_NOT_موجود(); }
صحيح Device::maxComputeUnits() ثابت { OCL_NOT_موجود(); }
صحيح Device::max_Constant_آرگومانها() ثابت { OCL_NOT_موجود(); }
size_t Device::max_Constant_میانگیر_اندازه() ثابت { OCL_NOT_موجود(); }

size_t Device::max_Mem_Alloc_اندازه() ثابت { OCL_NOT_موجود(); }
size_t Device::max_Parameter_اندازه() ثابت { OCL_NOT_موجود(); }

صحيح Device::max_Read_Image_آرگومانها() ثابت { OCL_NOT_موجود(); }
صحيح Device::max_Write_Image_آرگومانها() ثابت { OCL_NOT_موجود(); }
صحيح Device::maxSamplers() ثابت { OCL_NOT_موجود(); }

size_t Device::max_Work_گروه_اندازه() ثابت { OCL_NOT_موجود(); }
صحيح Device::max_Work_قلم_Dims() ثابت { OCL_NOT_موجود(); }
فراغ Device::max_Work_قلم_Sizes(size_t*) ثابت { OCL_NOT_موجود(); }

صحيح Device::mem_پایه_Addr_Align() ثابت { OCL_NOT_موجود(); }

صحيح Device::native_وکتور_عرض_Char() ثابت { OCL_NOT_موجود(); }
صحيح Device::native_وکتور_عرض_Short() ثابت { OCL_NOT_موجود(); }
صحيح Device::native_وکتور_عرض_Int() ثابت { OCL_NOT_موجود(); }
صحيح Device::native_وکتور_عرض_Long() ثابت { OCL_NOT_موجود(); }
صحيح Device::native_وکتور_عرض_Float() ثابت { OCL_NOT_موجود(); }
صحيح Device::native_وکتور_عرض_Double() ثابت { OCL_NOT_موجود(); }
صحيح Device::native_وکتور_عرض_Half() ثابت { OCL_NOT_موجود(); }

صحيح Device::preferred_وکتور_عرض_Char() ثابت { OCL_NOT_موجود(); }
صحيح Device::preferred_وکتور_عرض_Short() ثابت { OCL_NOT_موجود(); }
صحيح Device::preferred_وکتور_عرض_Int() ثابت { OCL_NOT_موجود(); }
صحيح Device::preferred_وکتور_عرض_Long() ثابت { OCL_NOT_موجود(); }
صحيح Device::preferred_وکتور_عرض_Float() ثابت { OCL_NOT_موجود(); }
صحيح Device::preferred_وکتور_عرض_Double() ثابت { OCL_NOT_موجود(); }
صحيح Device::preferred_وکتور_عرض_Half() ثابت { OCL_NOT_موجود(); }

size_t Device::printf_میانگیر_اندازه() ثابت { OCL_NOT_موجود(); }
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
Context& Context::عملگر=(ثابت Context& c) { أرجع *هذا; }
Context::Context(Context&&) CV_NOEXCEPT : p(عدم) { }
Context& Context::عملگر=(Context&&) CV_NOEXCEPT { أرجع *هذا; }

منطقي Context::create() { أرجع خطأ; }
منطقي Context::create(صحيح dtype) { أرجع خطأ; }
size_t Context::ndevices() ثابت { أرجع 0; }
Device& Context::device(size_t idx) ثابت { OCL_NOT_موجود(); }
برنامه_2 Context::getProg(ثابت برنامه_منبع& prog, ثابت String& buildopt, String& errmsg) { OCL_NOT_موجود(); }
فراغ Context::unloadProg(برنامه_2& prog) { }

/* static */
Context& Context::getDefault()
{
    ساكن Context dummy;
    أرجع dummy;
}
فراغ* Context::ptr() ثابت { أرجع عدم; }

فراغ* Context::getOpenCLContextProperty(صحيح /*propertyId*/) ثابت { OCL_NOT_موجود(); }

منطقي Context::useSVM() ثابت { أرجع خطأ; }
فراغ Context::مجموعه_Use_SVM(منطقي enabled) { }

Context::UserContext::~UserContext() { }

فراغ Context::مجموعه_User_Context(std::type_شاخص /*typeId*/, ثابت std::shared_ptr<Context::UserContext>& /*userContext*/) { OCL_NOT_موجود(); }
std::shared_ptr<Context::UserContext> Context::getUserContext(std::type_شاخص /*typeId*/) { OCL_NOT_موجود(); }

/* static */ Context Context::fromHandle(فراغ* context) { OCL_NOT_موجود(); }
/* static */ Context Context::fromDevice(ثابت ocl::Device& device) { OCL_NOT_موجود(); }
/* static */ Context Context::create(ثابت std::string& configuration) { OCL_NOT_موجود(); }

فراغ Context::release() { }


Platform::Platform() CV_NOEXCEPT : p(عدم) { }
Platform::~Platform() { }
Platform::Platform(ثابت Platform&) : p(عدم) { }
Platform& Platform::عملگر=(ثابت Platform&) { أرجع *هذا; }
Platform::Platform(Platform&&) CV_NOEXCEPT : p(عدم) { }
Platform& Platform::عملگر=(Platform&&) CV_NOEXCEPT { أرجع *هذا; }

فراغ* Platform::ptr() ثابت { أرجع عدم; }

/* static */
Platform& Platform::getDefault()
{
    ساكن Platform dummy;
    أرجع dummy;
}

فراغ attachContext(ثابت String& platformName, فراغ* platformID, فراغ* context, فراغ* deviceID) { OCL_NOT_موجود(); }
فراغ convert_From_میانگیر(فراغ* cl_mem_میانگیر, size_t گام, صحيح ردیفها, صحيح تعداد_ستون, صحيح type, UMat& dst) { OCL_NOT_موجود(); }
فراغ convertFromImage(فراغ* cl_mem_image, UMat& dst) { OCL_NOT_موجود(); }

فراغ initializeContextFromHandle(Context& ctx, فراغ* platform, فراغ* context, فراغ* device) { OCL_NOT_موجود(); }

صف::صف() CV_NOEXCEPT : p(عدم) { }
صف::صف(ثابت Context& c, ثابت Device& d) : p(عدم) { OCL_NOT_موجود(); }
صف::~صف() { }
صف::صف(ثابت صف& q) {}
صف& صف::عملگر=(ثابت صف& q) { أرجع *هذا; }
صف::صف(صف&&) CV_NOEXCEPT : p(عدم) { }
صف& صف::عملگر=(صف&&) CV_NOEXCEPT { أرجع *هذا; }

منطقي صف::create(ثابت Context& c, ثابت Device& d) { OCL_NOT_موجود(); }
فراغ صف::پایان_2() {}
فراغ* صف::ptr() ثابت { أرجع عدم; }
/* static */
صف& صف::getDefault()
{
    ساكن صف dummy;
    أرجع dummy;
}

/// @brief Returns OpenCL command queue with enable profiling mode support
ثابت صف& صف::get_Profiling_صف() ثابت { OCL_NOT_موجود(); }


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
Kernel::Kernel(ثابت حرف* kname, ثابت برنامه_2& prog) : p(عدم) { OCL_NOT_موجود(); }
Kernel::Kernel(ثابت حرف* kname, ثابت برنامه_منبع& prog, ثابت String& buildopts, String* errmsg) : p(عدم) { OCL_NOT_موجود(); }
Kernel::~Kernel() { }
Kernel::Kernel(ثابت Kernel& k) : p(عدم) { }
Kernel& Kernel::عملگر=(ثابت Kernel& k) { أرجع *هذا; }
Kernel::Kernel(Kernel&&) CV_NOEXCEPT : p(عدم) { }
Kernel& Kernel::عملگر=(Kernel&&) CV_NOEXCEPT { أرجع *هذا; }

منطقي Kernel::empty() ثابت { أرجع درست; }
منطقي Kernel::create(ثابت حرف* kname, ثابت برنامه_2& prog) { OCL_NOT_موجود(); }
منطقي Kernel::create(ثابت حرف* kname, ثابت برنامه_منبع& prog, ثابت String& buildopts, String* errmsg) { OCL_NOT_موجود(); }

صحيح Kernel::مجموعه(صحيح i, ثابت فراغ* value, size_t sz) { OCL_NOT_موجود(); }
صحيح Kernel::مجموعه(صحيح i, ثابت Image2D& image2D) { OCL_NOT_موجود(); }
صحيح Kernel::مجموعه(صحيح i, ثابت UMat& m) { OCL_NOT_موجود(); }
صحيح Kernel::مجموعه(صحيح i, ثابت KernelArg& arg) { OCL_NOT_موجود(); }

منطقي Kernel::پیاپی(صحيح dims, size_t globalsize[], size_t localsize[], منطقي متزامن, ثابت صف& q) { OCL_NOT_موجود(); }
منطقي Kernel::پیاپی_Task(منطقي متزامن, ثابت صف& q) { OCL_NOT_موجود(); }

int64 Kernel::پیاپی_Profiling(صحيح dims, size_t globalsize[], size_t localsize[], ثابت صف& q) { OCL_NOT_موجود(); }

size_t Kernel::work_گروه_اندازه() ثابت { OCL_NOT_موجود(); }
size_t Kernel::prefered_Work_گروه_اندازه_مضرب() ثابت { OCL_NOT_موجود(); }
منطقي Kernel::compile_Work_گروه_اندازه(size_t wsz[]) ثابت { OCL_NOT_موجود(); }
size_t Kernel::local_Mem_اندازه() ثابت { OCL_NOT_موجود(); }

فراغ* Kernel::ptr() ثابت { أرجع عدم; }


برنامه_2::برنامه_2() CV_NOEXCEPT : p(عدم) { }
برنامه_2::برنامه_2(ثابت برنامه_منبع& src, ثابت String& buildflags, String& errmsg) : p(عدم) { OCL_NOT_موجود(); }
برنامه_2::برنامه_2(ثابت برنامه_2& prog) : p(عدم) { }
برنامه_2& برنامه_2::عملگر=(ثابت برنامه_2& prog) { أرجع *هذا; }
برنامه_2::برنامه_2(برنامه_2&&) CV_NOEXCEPT : p(عدم) { }
برنامه_2& برنامه_2::عملگر=(برنامه_2&&) CV_NOEXCEPT { أرجع *هذا; }
برنامه_2::~برنامه_2() { }

منطقي برنامه_2::create(ثابت برنامه_منبع& src, ثابت String& buildflags, String& errmsg) { OCL_NOT_موجود(); }

فراغ* برنامه_2::ptr() ثابت { أرجع عدم; }

فراغ برنامه_2::get_دودویی(std::vector<حرف>& binary) ثابت { OCL_NOT_موجود(); }

منطقي برنامه_2::اقرأ(ثابت String& buf, ثابت String& buildflags) { OCL_NOT_موجود(); }
منطقي برنامه_2::اكتب(String& buf) ثابت { OCL_NOT_موجود(); }
ثابت برنامه_منبع& برنامه_2::منبع() ثابت { OCL_NOT_موجود(); }
String برنامه_2::get_پیشوند() ثابت { OCL_NOT_موجود(); }
/* static */ String برنامه_2::get_پیشوند(ثابت String& buildflags) { OCL_NOT_موجود(); }


برنامه_منبع::برنامه_منبع() CV_NOEXCEPT : p(عدم) { }
برنامه_منبع::برنامه_منبع(ثابت String& module, ثابت String& name, ثابت String& کد_Str, ثابت String& کد_Hash) : p(عدم) { }
برنامه_منبع::برنامه_منبع(ثابت String& prog) : p(عدم) { }
برنامه_منبع::برنامه_منبع(ثابت حرف* prog) : p(عدم) { }
برنامه_منبع::~برنامه_منبع() { }
برنامه_منبع::برنامه_منبع(ثابت برنامه_منبع& prog) : p(عدم) { }
برنامه_منبع& برنامه_منبع::عملگر=(ثابت برنامه_منبع& prog) { أرجع *هذا; }
برنامه_منبع::برنامه_منبع(برنامه_منبع&&) CV_NOEXCEPT : p(عدم) { }
برنامه_منبع& برنامه_منبع::عملگر=(برنامه_منبع&&) CV_NOEXCEPT { أرجع *هذا; }

ثابت String& برنامه_منبع::منبع() ثابت { OCL_NOT_موجود(); }
برنامه_منبع::hash_t برنامه_منبع::hash() ثابت { OCL_NOT_موجود(); }

/* static */ برنامه_منبع برنامه_منبع::from_دودویی(ثابت String& module, ثابت String& name, ثابت بیعلامت حرف* binary, ثابت size_t size, ثابت cv::String& buildOptions) { OCL_NOT_موجود(); }
/* static */ برنامه_منبع برنامه_منبع::fromSPIR(ثابت String& module, ثابت String& name, ثابت بیعلامت حرف* binary, ثابت size_t size, ثابت cv::String& buildOptions) { OCL_NOT_موجود(); }


PlatformInfo::PlatformInfo() CV_NOEXCEPT : p(عدم) { }
PlatformInfo::PlatformInfo(فراغ* id) : p(عدم) { OCL_NOT_موجود(); }
PlatformInfo::~PlatformInfo() { }

PlatformInfo::PlatformInfo(ثابت PlatformInfo& i) : p(عدم) { }
PlatformInfo& PlatformInfo::عملگر=(ثابت PlatformInfo& i) { أرجع *هذا; }
PlatformInfo::PlatformInfo(PlatformInfo&&) CV_NOEXCEPT : p(عدم) { }
PlatformInfo& PlatformInfo::عملگر=(PlatformInfo&&) CV_NOEXCEPT { أرجع *هذا; }

String PlatformInfo::name() ثابت { OCL_NOT_موجود(); }
String PlatformInfo::vendor() ثابت { OCL_NOT_موجود(); }
String PlatformInfo::نسخه() ثابت { OCL_NOT_موجود(); }
صحيح PlatformInfo::device_عدد() ثابت { OCL_NOT_موجود(); }
فراغ PlatformInfo::getDevice(Device& device, صحيح d) ثابت { OCL_NOT_موجود(); }

ثابت حرف* convertTypeStr(صحيح sdepth, صحيح ddepth, صحيح cn, حرف* buf) { OCL_NOT_موجود(); }
ثابت حرف* type_مقصد_Str(صحيح t) { OCL_NOT_موجود(); }
ثابت حرف* memop_Type_مقصد_Str(صحيح t) { OCL_NOT_موجود(); }
ثابت حرف* vecop_Type_مقصد_Str(صحيح t) { OCL_NOT_موجود(); }
ثابت حرف* getOpenCLErrorString(صحيح error_کد) { OCL_NOT_موجود(); }
String kernel_مقصد_Str(درون_داد_Array _kernel, صحيح ddepth, ثابت حرف* name) { OCL_NOT_موجود(); }
فراغ getPlatfomsInfo(std::vector<PlatformInfo>& platform_info) { OCL_NOT_موجود(); }


صحيح predict_Optimal_وکتور_عرض(درون_داد_Array src1, درون_داد_Array src2, درون_داد_Array src3,
        درون_داد_Array src4, درون_داد_Array src5, درون_داد_Array src6,
        درون_داد_Array src7, درون_داد_Array src8, درون_داد_Array src9,
        Ocl_وکتور_Strategy strat)
{ OCL_NOT_موجود(); }

صحيح check_Optimal_وکتور_عرض(ثابت صحيح *وکتور_Widths,
        درون_داد_Array src1, درون_داد_Array src2, درون_داد_Array src3,
        درون_داد_Array src4, درون_داد_Array src5, درون_داد_Array src6,
        درون_داد_Array src7, درون_داد_Array src8, درون_داد_Array src9,
        Ocl_وکتور_Strategy strat)
{ OCL_NOT_موجود(); }

صحيح predict_Optimal_وکتور_عرض_Max(درون_داد_Array src1, درون_داد_Array src2, درون_داد_Array src3,
        درون_داد_Array src4, درون_داد_Array src5, درون_داد_Array src6,
        درون_داد_Array src7, درون_داد_Array src8, درون_داد_Array src9)
{ OCL_NOT_موجود(); }

فراغ build_Options_جمع_کن_ماتریس_Description(String& buildOptions, ثابت String& name, درون_داد_Array _m) { OCL_NOT_موجود(); }


Image2D::Image2D() CV_NOEXCEPT : p(عدم) { }
Image2D::Image2D(ثابت UMat &src, منطقي norm, منطقي alias) { OCL_NOT_موجود(); }
Image2D::Image2D(ثابت Image2D & i) : p(عدم) { OCL_NOT_موجود(); }
Image2D::~Image2D() { }
Image2D& Image2D::عملگر=(ثابت Image2D & i) { أرجع *هذا; }
Image2D::Image2D(Image2D&&) CV_NOEXCEPT : p(عدم) { }
Image2D& Image2D::عملگر=(Image2D&&) CV_NOEXCEPT { أرجع *هذا; }

/* static */ منطقي Image2D::canCreateAlias(ثابت UMat &u) { OCL_NOT_موجود(); }
/* static */ منطقي Image2D::isFormatSupported(صحيح عمق, صحيح cn, منطقي norm) { OCL_NOT_موجود(); }

فراغ* Image2D::ptr() ثابت { أرجع عدم; }


Timer::Timer(ثابت صف& q) : p(عدم) {}
Timer::~Timer() {}
فراغ Timer::شروع() { OCL_NOT_موجود(); }
فراغ Timer::stop() { OCL_NOT_موجود();}

uint64 Timer::durationNS() ثابت { OCL_NOT_موجود(); }

MatAllocator* getOpenCLAllocator() { أرجع عدم; }

internal::برنامه_مدخل::عملگر برنامه_منبع&() ثابت { OCL_NOT_موجود(); }


بنية OpenCLExecutionContext::Impl
{
    Impl() = افتراضي;
};

Context& OpenCLExecutionContext::getContext() ثابت { OCL_NOT_موجود(); }
Device& OpenCLExecutionContext::getDevice() ثابت { OCL_NOT_موجود(); }
صف& OpenCLExecutionContext::get_صف() ثابت { OCL_NOT_موجود(); }

منطقي OpenCLExecutionContext::useOpenCL() ثابت { أرجع خطأ; }
فراغ OpenCLExecutionContext::مجموعه_Use_Open_CL(منطقي flag) { }

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

OpenCLExecutionContext OpenCLExecutionContext::clone_With_New_صف(ثابت ocl::صف& q) ثابت { OCL_NOT_موجود(); }
OpenCLExecutionContext OpenCLExecutionContext::clone_With_New_صف() ثابت { OCL_NOT_موجود(); }

/* static */ OpenCLExecutionContext OpenCLExecutionContext::create(ثابت std::string& platformName, فراغ* platformID, فراغ* context, فراغ* deviceID) { OCL_NOT_موجود(); }
/* static */ OpenCLExecutionContext OpenCLExecutionContext::create(ثابت Context& context, ثابت Device& device, ثابت ocl::صف& صف_2) { OCL_NOT_موجود(); }
/* static */ OpenCLExecutionContext OpenCLExecutionContext::create(ثابت Context& context, ثابت Device& device) { OCL_NOT_موجود(); }

فراغ OpenCLExecutionContext::release() { }

}}

#إذا defined(_MSC_VER)
    #رهنمود warning(pop)
#وإلاإذا defined(__clang__)
    #رهنمود clang diagnostic pop
#وإلاإذا defined(__GNUC__)
    #رهنمود GCC diagnostic pop
#پایان_اگر
