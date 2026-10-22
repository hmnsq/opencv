// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ضمن "opencv2/core/ocl_genbase.hpp"

#إذا defined(_MSC_VER)
    #لارښوونه warning(push)
    #لارښوونه warning(disable : 4100)
    #لارښوونه warning(disable : 4702)
#وإلاإذا defined(__clang__)
    #لارښوونه clang diagnostic push
    #لارښوونه clang diagnostic ignored "-Wunused-parameter"
#وإلاإذا defined(__GNUC__)
    #لارښوونه GCC diagnostic push
    #لارښوونه GCC diagnostic ignored "-Wunused-parameter"
#د_که_پای

نطاق cv { نطاق ocl {

ساكن
CV_NORETURN فراغ throw_no_ocl()
{
    CV_Error(Error::OpenCLApiCallError, "OpenCV build without OpenCL support");
}
#تعریف_کړه OCL_NOT_شته() throw_no_ocl();

CV_EXPORTS_W منطقي haveOpenCL() { أرجع خطأ; }
CV_EXPORTS_W منطقي useOpenCL() { أرجع خطأ; }
CV_EXPORTS_W منطقي haveAmdBlas() { أرجع خطأ; }
CV_EXPORTS_W منطقي haveAmdFft() { أرجع خطأ; }
CV_EXPORTS_W فراغ سېټ_Use_Open_CL(منطقي flag) { /* nothing */ }
CV_EXPORTS_W فراغ پای_2() { /* nothing */ }

CV_EXPORTS منطقي haveSVM() { أرجع خطأ; }

Device::Device() CV_NOEXCEPT : p(عدم) { }
Device::Device(فراغ* d) : p(عدم) { OCL_NOT_شته(); }
Device::Device(ثابت Device& d) : p(عدم) { }
Device& Device::چلوونکی=(ثابت Device& d) { أرجع *هذا; }
Device::Device(Device&&) CV_NOEXCEPT : p(عدم) { }
Device& Device::چلوونکی=(Device&&) CV_NOEXCEPT { أرجع *هذا; }
Device::~Device() { }

فراغ Device::سېټ(فراغ* d) { OCL_NOT_شته(); }

String Device::name() ثابت { OCL_NOT_شته(); }
String Device::extensions() ثابت { OCL_NOT_شته(); }
منطقي Device::isExtensionSupported(ثابت String& extensionName) ثابت { OCL_NOT_شته(); }
String Device::نسخه() ثابت { OCL_NOT_شته(); }
String Device::vendorName() ثابت { OCL_NOT_شته(); }
String Device::Open_CL_C_نسخه() ثابت { OCL_NOT_شته(); }
String Device::OpenCLVersion() ثابت { OCL_NOT_شته(); }
صحيح Device::device_نسخه_Major() ثابت { OCL_NOT_شته(); }
صحيح Device::device_نسخه_Minor() ثابت { OCL_NOT_شته(); }
String Device::driver_نسخه() ثابت { OCL_NOT_شته(); }
فراغ* Device::ptr() ثابت { /*OCL_NOT_AVAILABLE();*/ أرجع عدم; }

صحيح Device::type() ثابت { OCL_NOT_شته(); }

صحيح Device::پته_بټونه() ثابت { OCL_NOT_شته(); }
منطقي Device::شته() ثابت { OCL_NOT_شته(); }
منطقي Device::compiler_شته() ثابت { OCL_NOT_شته(); }
منطقي Device::linker_شته() ثابت { OCL_NOT_شته(); }

صحيح Device::doubleFPConfig() ثابت { OCL_NOT_شته(); }
صحيح Device::singleFPConfig() ثابت { OCL_NOT_شته(); }
صحيح Device::halfFPConfig() ثابت { OCL_NOT_شته(); }

منطقي Device::hasFP64() ثابت { OCL_NOT_شته(); }
منطقي Device::hasFP16() ثابت { OCL_NOT_شته(); }

منطقي Device::endianLittle() ثابت { OCL_NOT_شته(); }
منطقي Device::errorCorrectionSupport() ثابت { OCL_NOT_شته(); }

صحيح Device::executionCapabilities() ثابت { OCL_NOT_شته(); }

size_t Device::global_Mem_Cache_اندازه() ثابت { OCL_NOT_شته(); }

صحيح Device::globalMemCacheType() ثابت { OCL_NOT_شته(); }
صحيح Device::global_Mem_Cache_کرښه_اندازه() ثابت { OCL_NOT_شته(); }
size_t Device::global_Mem_اندازه() ثابت { OCL_NOT_شته(); }

size_t Device::local_Mem_اندازه() ثابت { OCL_NOT_شته(); }
صحيح Device::localMemType() ثابت { أرجع NO_LOCAL_MEM; }
منطقي Device::host_Unified_حافظه_2() ثابت { OCL_NOT_شته(); }

منطقي Device::imageSupport() ثابت { OCL_NOT_شته(); }

منطقي Device::image_From_بفر_Support() ثابت { OCL_NOT_شته(); }
uint Device::imagePitchAlignment() ثابت { OCL_NOT_شته(); }
uint Device::image_بنسټ_پته_Alignment() ثابت { OCL_NOT_شته(); }

منطقي Device::intelSubgroupsSupport() ثابت { OCL_NOT_شته(); }

size_t Device::image2_DMax_پلنوالی() ثابت { OCL_NOT_شته(); }
size_t Device::image2_DMax_لوړوالی() ثابت { OCL_NOT_شته(); }

size_t Device::image3_DMax_پلنوالی() ثابت { OCL_NOT_شته(); }
size_t Device::image3_DMax_لوړوالی() ثابت { OCL_NOT_شته(); }
size_t Device::image3_DMax_ژوروالی() ثابت { OCL_NOT_شته(); }

size_t Device::image_Max_بفر_اندازه() ثابت { OCL_NOT_شته(); }
size_t Device::image_Max_Array_اندازه() ثابت { OCL_NOT_شته(); }

صحيح Device::vendorID() ثابت { OCL_NOT_شته(); }

صحيح Device::maxClockFrequency() ثابت { OCL_NOT_شته(); }
صحيح Device::maxComputeUnits() ثابت { OCL_NOT_شته(); }
صحيح Device::max_Constant_دلیلونه() ثابت { OCL_NOT_شته(); }
size_t Device::max_Constant_بفر_اندازه() ثابت { OCL_NOT_شته(); }

size_t Device::max_Mem_Alloc_اندازه() ثابت { OCL_NOT_شته(); }
size_t Device::max_Parameter_اندازه() ثابت { OCL_NOT_شته(); }

صحيح Device::max_Read_Image_دلیلونه() ثابت { OCL_NOT_شته(); }
صحيح Device::max_Write_Image_دلیلونه() ثابت { OCL_NOT_شته(); }
صحيح Device::maxSamplers() ثابت { OCL_NOT_شته(); }

size_t Device::max_Work_ډله_اندازه() ثابت { OCL_NOT_شته(); }
صحيح Device::max_Work_توکی_Dims() ثابت { OCL_NOT_شته(); }
فراغ Device::max_Work_توکی_Sizes(size_t*) ثابت { OCL_NOT_شته(); }

صحيح Device::mem_بنسټ_Addr_Align() ثابت { OCL_NOT_شته(); }

صحيح Device::native_ویکټور_پلنوالی_Char() ثابت { OCL_NOT_شته(); }
صحيح Device::native_ویکټور_پلنوالی_Short() ثابت { OCL_NOT_شته(); }
صحيح Device::native_ویکټور_پلنوالی_Int() ثابت { OCL_NOT_شته(); }
صحيح Device::native_ویکټور_پلنوالی_Long() ثابت { OCL_NOT_شته(); }
صحيح Device::native_ویکټور_پلنوالی_Float() ثابت { OCL_NOT_شته(); }
صحيح Device::native_ویکټور_پلنوالی_Double() ثابت { OCL_NOT_شته(); }
صحيح Device::native_ویکټور_پلنوالی_Half() ثابت { OCL_NOT_شته(); }

صحيح Device::preferred_ویکټور_پلنوالی_Char() ثابت { OCL_NOT_شته(); }
صحيح Device::preferred_ویکټور_پلنوالی_Short() ثابت { OCL_NOT_شته(); }
صحيح Device::preferred_ویکټور_پلنوالی_Int() ثابت { OCL_NOT_شته(); }
صحيح Device::preferred_ویکټور_پلنوالی_Long() ثابت { OCL_NOT_شته(); }
صحيح Device::preferred_ویکټور_پلنوالی_Float() ثابت { OCL_NOT_شته(); }
صحيح Device::preferred_ویکټور_پلنوالی_Double() ثابت { OCL_NOT_شته(); }
صحيح Device::preferred_ویکټور_پلنوالی_Half() ثابت { OCL_NOT_شته(); }

size_t Device::printf_بفر_اندازه() ثابت { OCL_NOT_شته(); }
size_t Device::profilingTimerResolution() ثابت { OCL_NOT_شته(); }

/* static */
ثابت Device& Device::getDefault()
{
    ساكن Device dummy;
    أرجع dummy;
}

/* static */ Device Device::fromHandle(فراغ* d) { OCL_NOT_شته(); }


Context::Context() CV_NOEXCEPT : p(عدم) { }
Context::Context(صحيح dtype) : p(عدم) { }
Context::~Context() { }
Context::Context(ثابت Context& c) : p(عدم) { }
Context& Context::چلوونکی=(ثابت Context& c) { أرجع *هذا; }
Context::Context(Context&&) CV_NOEXCEPT : p(عدم) { }
Context& Context::چلوونکی=(Context&&) CV_NOEXCEPT { أرجع *هذا; }

منطقي Context::create() { أرجع خطأ; }
منطقي Context::create(صحيح dtype) { أرجع خطأ; }
size_t Context::ndevices() ثابت { أرجع 0; }
Device& Context::device(size_t idx) ثابت { OCL_NOT_شته(); }
پروګرام_2 Context::getProg(ثابت پروګرام_سرچینه& prog, ثابت String& buildopt, String& errmsg) { OCL_NOT_شته(); }
فراغ Context::unloadProg(پروګرام_2& prog) { }

/* static */
Context& Context::getDefault()
{
    ساكن Context dummy;
    أرجع dummy;
}
فراغ* Context::ptr() ثابت { أرجع عدم; }

فراغ* Context::getOpenCLContextProperty(صحيح /*propertyId*/) ثابت { OCL_NOT_شته(); }

منطقي Context::useSVM() ثابت { أرجع خطأ; }
فراغ Context::سېټ_Use_SVM(منطقي enabled) { }

Context::UserContext::~UserContext() { }

فراغ Context::سېټ_User_Context(std::type_شاخص /*typeId*/, ثابت std::shared_ptr<Context::UserContext>& /*userContext*/) { OCL_NOT_شته(); }
std::shared_ptr<Context::UserContext> Context::getUserContext(std::type_شاخص /*typeId*/) { OCL_NOT_شته(); }

/* static */ Context Context::fromHandle(فراغ* context) { OCL_NOT_شته(); }
/* static */ Context Context::fromDevice(ثابت ocl::Device& device) { OCL_NOT_شته(); }
/* static */ Context Context::create(ثابت std::string& configuration) { OCL_NOT_شته(); }

فراغ Context::release() { }


Platform::Platform() CV_NOEXCEPT : p(عدم) { }
Platform::~Platform() { }
Platform::Platform(ثابت Platform&) : p(عدم) { }
Platform& Platform::چلوونکی=(ثابت Platform&) { أرجع *هذا; }
Platform::Platform(Platform&&) CV_NOEXCEPT : p(عدم) { }
Platform& Platform::چلوونکی=(Platform&&) CV_NOEXCEPT { أرجع *هذا; }

فراغ* Platform::ptr() ثابت { أرجع عدم; }

/* static */
Platform& Platform::getDefault()
{
    ساكن Platform dummy;
    أرجع dummy;
}

فراغ attachContext(ثابت String& platformName, فراغ* platformID, فراغ* context, فراغ* deviceID) { OCL_NOT_شته(); }
فراغ convert_From_بفر(فراغ* cl_mem_بفر, size_t ګام, صحيح لیکې, صحيح ستون_شمېر, صحيح type, UMat& dst) { OCL_NOT_شته(); }
فراغ convertFromImage(فراغ* cl_mem_image, UMat& dst) { OCL_NOT_شته(); }

فراغ initializeContextFromHandle(Context& ctx, فراغ* platform, فراغ* context, فراغ* device) { OCL_NOT_شته(); }

کتار::کتار() CV_NOEXCEPT : p(عدم) { }
کتار::کتار(ثابت Context& c, ثابت Device& d) : p(عدم) { OCL_NOT_شته(); }
کتار::~کتار() { }
کتار::کتار(ثابت کتار& q) {}
کتار& کتار::چلوونکی=(ثابت کتار& q) { أرجع *هذا; }
کتار::کتار(کتار&&) CV_NOEXCEPT : p(عدم) { }
کتار& کتار::چلوونکی=(کتار&&) CV_NOEXCEPT { أرجع *هذا; }

منطقي کتار::create(ثابت Context& c, ثابت Device& d) { OCL_NOT_شته(); }
فراغ کتار::پای_2() {}
فراغ* کتار::ptr() ثابت { أرجع عدم; }
/* static */
کتار& کتار::getDefault()
{
    ساكن کتار dummy;
    أرجع dummy;
}

/// @brief Returns OpenCL command queue with enable profiling mode support
ثابت کتار& کتار::get_Profiling_کتار() ثابت { OCL_NOT_شته(); }


KernelArg::KernelArg() CV_NOEXCEPT
    : flags(0), m(0), obj(0), sz(0), wscale(1), iwscale(1)
{
}

KernelArg::KernelArg(صحيح _flags, UMat* _m, صحيح _wscale, صحيح _iwscale, ثابت فراغ* _obj, size_t _sz)
    : flags(_flags), m(_m), obj(_obj), sz(_sz), wscale(_wscale), iwscale(_iwscale)
{
    OCL_NOT_شته();
}

KernelArg KernelArg::Constant(ثابت Mat& m)
{
    OCL_NOT_شته();
}


Kernel::Kernel() CV_NOEXCEPT : p(عدم) { }
Kernel::Kernel(ثابت حرف* kname, ثابت پروګرام_2& prog) : p(عدم) { OCL_NOT_شته(); }
Kernel::Kernel(ثابت حرف* kname, ثابت پروګرام_سرچینه& prog, ثابت String& buildopts, String* errmsg) : p(عدم) { OCL_NOT_شته(); }
Kernel::~Kernel() { }
Kernel::Kernel(ثابت Kernel& k) : p(عدم) { }
Kernel& Kernel::چلوونکی=(ثابت Kernel& k) { أرجع *هذا; }
Kernel::Kernel(Kernel&&) CV_NOEXCEPT : p(عدم) { }
Kernel& Kernel::چلوونکی=(Kernel&&) CV_NOEXCEPT { أرجع *هذا; }

منطقي Kernel::empty() ثابت { أرجع ریښتیا; }
منطقي Kernel::create(ثابت حرف* kname, ثابت پروګرام_2& prog) { OCL_NOT_شته(); }
منطقي Kernel::create(ثابت حرف* kname, ثابت پروګرام_سرچینه& prog, ثابت String& buildopts, String* errmsg) { OCL_NOT_شته(); }

صحيح Kernel::سېټ(صحيح i, ثابت فراغ* value, size_t sz) { OCL_NOT_شته(); }
صحيح Kernel::سېټ(صحيح i, ثابت Image2D& image2D) { OCL_NOT_شته(); }
صحيح Kernel::سېټ(صحيح i, ثابت UMat& m) { OCL_NOT_شته(); }
صحيح Kernel::سېټ(صحيح i, ثابت KernelArg& arg) { OCL_NOT_شته(); }

منطقي Kernel::پرلپسې(صحيح dims, size_t globalsize[], size_t localsize[], منطقي متزامن, ثابت کتار& q) { OCL_NOT_شته(); }
منطقي Kernel::پرلپسې_Task(منطقي متزامن, ثابت کتار& q) { OCL_NOT_شته(); }

int64 Kernel::پرلپسې_Profiling(صحيح dims, size_t globalsize[], size_t localsize[], ثابت کتار& q) { OCL_NOT_شته(); }

size_t Kernel::work_ډله_اندازه() ثابت { OCL_NOT_شته(); }
size_t Kernel::prefered_Work_ډله_اندازه_مضرب() ثابت { OCL_NOT_شته(); }
منطقي Kernel::compile_Work_ډله_اندازه(size_t wsz[]) ثابت { OCL_NOT_شته(); }
size_t Kernel::local_Mem_اندازه() ثابت { OCL_NOT_شته(); }

فراغ* Kernel::ptr() ثابت { أرجع عدم; }


پروګرام_2::پروګرام_2() CV_NOEXCEPT : p(عدم) { }
پروګرام_2::پروګرام_2(ثابت پروګرام_سرچینه& src, ثابت String& buildflags, String& errmsg) : p(عدم) { OCL_NOT_شته(); }
پروګرام_2::پروګرام_2(ثابت پروګرام_2& prog) : p(عدم) { }
پروګرام_2& پروګرام_2::چلوونکی=(ثابت پروګرام_2& prog) { أرجع *هذا; }
پروګرام_2::پروګرام_2(پروګرام_2&&) CV_NOEXCEPT : p(عدم) { }
پروګرام_2& پروګرام_2::چلوونکی=(پروګرام_2&&) CV_NOEXCEPT { أرجع *هذا; }
پروګرام_2::~پروګرام_2() { }

منطقي پروګرام_2::create(ثابت پروګرام_سرچینه& src, ثابت String& buildflags, String& errmsg) { OCL_NOT_شته(); }

فراغ* پروګرام_2::ptr() ثابت { أرجع عدم; }

فراغ پروګرام_2::get_دوګونی(std::vector<حرف>& binary) ثابت { OCL_NOT_شته(); }

منطقي پروګرام_2::اقرأ(ثابت String& buf, ثابت String& buildflags) { OCL_NOT_شته(); }
منطقي پروګرام_2::اكتب(String& buf) ثابت { OCL_NOT_شته(); }
ثابت پروګرام_سرچینه& پروګرام_2::سرچینه() ثابت { OCL_NOT_شته(); }
String پروګرام_2::get_مختاړی() ثابت { OCL_NOT_شته(); }
/* static */ String پروګرام_2::get_مختاړی(ثابت String& buildflags) { OCL_NOT_شته(); }


پروګرام_سرچینه::پروګرام_سرچینه() CV_NOEXCEPT : p(عدم) { }
پروګرام_سرچینه::پروګرام_سرچینه(ثابت String& module, ثابت String& name, ثابت String& کوډ_Str, ثابت String& کوډ_Hash) : p(عدم) { }
پروګرام_سرچینه::پروګرام_سرچینه(ثابت String& prog) : p(عدم) { }
پروګرام_سرچینه::پروګرام_سرچینه(ثابت حرف* prog) : p(عدم) { }
پروګرام_سرچینه::~پروګرام_سرچینه() { }
پروګرام_سرچینه::پروګرام_سرچینه(ثابت پروګرام_سرچینه& prog) : p(عدم) { }
پروګرام_سرچینه& پروګرام_سرچینه::چلوونکی=(ثابت پروګرام_سرچینه& prog) { أرجع *هذا; }
پروګرام_سرچینه::پروګرام_سرچینه(پروګرام_سرچینه&&) CV_NOEXCEPT : p(عدم) { }
پروګرام_سرچینه& پروګرام_سرچینه::چلوونکی=(پروګرام_سرچینه&&) CV_NOEXCEPT { أرجع *هذا; }

ثابت String& پروګرام_سرچینه::سرچینه() ثابت { OCL_NOT_شته(); }
پروګرام_سرچینه::hash_t پروګرام_سرچینه::hash() ثابت { OCL_NOT_شته(); }

/* static */ پروګرام_سرچینه پروګرام_سرچینه::from_دوګونی(ثابت String& module, ثابت String& name, ثابت بینښه حرف* binary, ثابت size_t size, ثابت cv::String& buildOptions) { OCL_NOT_شته(); }
/* static */ پروګرام_سرچینه پروګرام_سرچینه::fromSPIR(ثابت String& module, ثابت String& name, ثابت بینښه حرف* binary, ثابت size_t size, ثابت cv::String& buildOptions) { OCL_NOT_شته(); }


PlatformInfo::PlatformInfo() CV_NOEXCEPT : p(عدم) { }
PlatformInfo::PlatformInfo(فراغ* id) : p(عدم) { OCL_NOT_شته(); }
PlatformInfo::~PlatformInfo() { }

PlatformInfo::PlatformInfo(ثابت PlatformInfo& i) : p(عدم) { }
PlatformInfo& PlatformInfo::چلوونکی=(ثابت PlatformInfo& i) { أرجع *هذا; }
PlatformInfo::PlatformInfo(PlatformInfo&&) CV_NOEXCEPT : p(عدم) { }
PlatformInfo& PlatformInfo::چلوونکی=(PlatformInfo&&) CV_NOEXCEPT { أرجع *هذا; }

String PlatformInfo::name() ثابت { OCL_NOT_شته(); }
String PlatformInfo::vendor() ثابت { OCL_NOT_شته(); }
String PlatformInfo::نسخه() ثابت { OCL_NOT_شته(); }
صحيح PlatformInfo::device_شمېره() ثابت { OCL_NOT_شته(); }
فراغ PlatformInfo::getDevice(Device& device, صحيح d) ثابت { OCL_NOT_شته(); }

ثابت حرف* convertTypeStr(صحيح sdepth, صحيح ddepth, صحيح cn, حرف* buf) { OCL_NOT_شته(); }
ثابت حرف* type_منزل_Str(صحيح t) { OCL_NOT_شته(); }
ثابت حرف* memop_Type_منزل_Str(صحيح t) { OCL_NOT_شته(); }
ثابت حرف* vecop_Type_منزل_Str(صحيح t) { OCL_NOT_شته(); }
ثابت حرف* getOpenCLErrorString(صحيح error_کوډ) { OCL_NOT_شته(); }
String kernel_منزل_Str(ننوت_Array _kernel, صحيح ddepth, ثابت حرف* name) { OCL_NOT_شته(); }
فراغ getPlatfomsInfo(std::vector<PlatformInfo>& platform_info) { OCL_NOT_شته(); }


صحيح predict_Optimal_ویکټور_پلنوالی(ننوت_Array src1, ننوت_Array src2, ننوت_Array src3,
        ننوت_Array src4, ننوت_Array src5, ننوت_Array src6,
        ننوت_Array src7, ننوت_Array src8, ننوت_Array src9,
        Ocl_ویکټور_Strategy strat)
{ OCL_NOT_شته(); }

صحيح check_Optimal_ویکټور_پلنوالی(ثابت صحيح *ویکټور_Widths,
        ننوت_Array src1, ننوت_Array src2, ننوت_Array src3,
        ننوت_Array src4, ننوت_Array src5, ننوت_Array src6,
        ننوت_Array src7, ننوت_Array src8, ننوت_Array src9,
        Ocl_ویکټور_Strategy strat)
{ OCL_NOT_شته(); }

صحيح predict_Optimal_ویکټور_پلنوالی_Max(ننوت_Array src1, ننوت_Array src2, ننوت_Array src3,
        ننوت_Array src4, ننوت_Array src5, ننوت_Array src6,
        ننوت_Array src7, ننوت_Array src8, ننوت_Array src9)
{ OCL_NOT_شته(); }

فراغ build_Options_جمع_کړه_ماتریکس_Description(String& buildOptions, ثابت String& name, ننوت_Array _m) { OCL_NOT_شته(); }


Image2D::Image2D() CV_NOEXCEPT : p(عدم) { }
Image2D::Image2D(ثابت UMat &src, منطقي norm, منطقي alias) { OCL_NOT_شته(); }
Image2D::Image2D(ثابت Image2D & i) : p(عدم) { OCL_NOT_شته(); }
Image2D::~Image2D() { }
Image2D& Image2D::چلوونکی=(ثابت Image2D & i) { أرجع *هذا; }
Image2D::Image2D(Image2D&&) CV_NOEXCEPT : p(عدم) { }
Image2D& Image2D::چلوونکی=(Image2D&&) CV_NOEXCEPT { أرجع *هذا; }

/* static */ منطقي Image2D::canCreateAlias(ثابت UMat &u) { OCL_NOT_شته(); }
/* static */ منطقي Image2D::isFormatSupported(صحيح ژوروالی, صحيح cn, منطقي norm) { OCL_NOT_شته(); }

فراغ* Image2D::ptr() ثابت { أرجع عدم; }


Timer::Timer(ثابت کتار& q) : p(عدم) {}
Timer::~Timer() {}
فراغ Timer::پیل() { OCL_NOT_شته(); }
فراغ Timer::stop() { OCL_NOT_شته();}

uint64 Timer::durationNS() ثابت { OCL_NOT_شته(); }

MatAllocator* getOpenCLAllocator() { أرجع عدم; }

internal::پروګرام_ثبت::چلوونکی پروګرام_سرچینه&() ثابت { OCL_NOT_شته(); }


بنية OpenCLExecutionContext::Impl
{
    Impl() = افتراضي;
};

Context& OpenCLExecutionContext::getContext() ثابت { OCL_NOT_شته(); }
Device& OpenCLExecutionContext::getDevice() ثابت { OCL_NOT_شته(); }
کتار& OpenCLExecutionContext::get_کتار() ثابت { OCL_NOT_شته(); }

منطقي OpenCLExecutionContext::useOpenCL() ثابت { أرجع خطأ; }
فراغ OpenCLExecutionContext::سېټ_Use_Open_CL(منطقي flag) { }

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

فراغ OpenCLExecutionContext::bind() ثابت { OCL_NOT_شته(); }

OpenCLExecutionContext OpenCLExecutionContext::clone_With_New_کتار(ثابت ocl::کتار& q) ثابت { OCL_NOT_شته(); }
OpenCLExecutionContext OpenCLExecutionContext::clone_With_New_کتار() ثابت { OCL_NOT_شته(); }

/* static */ OpenCLExecutionContext OpenCLExecutionContext::create(ثابت std::string& platformName, فراغ* platformID, فراغ* context, فراغ* deviceID) { OCL_NOT_شته(); }
/* static */ OpenCLExecutionContext OpenCLExecutionContext::create(ثابت Context& context, ثابت Device& device, ثابت ocl::کتار& کتار_2) { OCL_NOT_شته(); }
/* static */ OpenCLExecutionContext OpenCLExecutionContext::create(ثابت Context& context, ثابت Device& device) { OCL_NOT_شته(); }

فراغ OpenCLExecutionContext::release() { }

}}

#إذا defined(_MSC_VER)
    #لارښوونه warning(pop)
#وإلاإذا defined(__clang__)
    #لارښوونه clang diagnostic pop
#وإلاإذا defined(__GNUC__)
    #لارښوونه GCC diagnostic pop
#د_که_پای
