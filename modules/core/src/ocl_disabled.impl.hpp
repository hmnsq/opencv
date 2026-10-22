// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#شامل "opencv2/core/ocl_genbase.hpp"

#اگر defined(_MSC_VER)
    #pragma warning(push)
    #pragma warning(disable : 4100)
    #pragma warning(disable : 4702)
#وگرنهاگر defined(__clang__)
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wunused-parameter"
#وگرنهاگر defined(__GNUC__)
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

فضاینام cv { فضاینام ocl {

ایستا
CV_NORETURN تهی throw_no_ocl()
{
    CV_Error(Error::OpenCLApiCallError, "OpenCV build without OpenCL support");
}
#define OCL_NOT_AVAILABLE() throw_no_ocl();

CV_EXPORTS_W منطقی haveOpenCL() { بازگشت نادرست; }
CV_EXPORTS_W منطقی useOpenCL() { بازگشت نادرست; }
CV_EXPORTS_W منطقی haveAmdBlas() { بازگشت نادرست; }
CV_EXPORTS_W منطقی haveAmdFft() { بازگشت نادرست; }
CV_EXPORTS_W تهی setUseOpenCL(منطقی flag) { /* nothing */ }
CV_EXPORTS_W تهی finish() { /* nothing */ }

CV_EXPORTS منطقی haveSVM() { بازگشت نادرست; }

Device::Device() CV_NOEXCEPT : p(پوچ) { }
Device::Device(تهی* d) : p(پوچ) { OCL_NOT_AVAILABLE(); }
Device::Device(ثابت Device& d) : p(پوچ) { }
Device& Device::operator=(ثابت Device& d) { بازگشت *این; }
Device::Device(Device&&) CV_NOEXCEPT : p(پوچ) { }
Device& Device::operator=(Device&&) CV_NOEXCEPT { بازگشت *این; }
Device::~Device() { }

تهی Device::set(تهی* d) { OCL_NOT_AVAILABLE(); }

String Device::name() ثابت { OCL_NOT_AVAILABLE(); }
String Device::extensions() ثابت { OCL_NOT_AVAILABLE(); }
منطقی Device::isExtensionSupported(ثابت String& extensionName) ثابت { OCL_NOT_AVAILABLE(); }
String Device::version() ثابت { OCL_NOT_AVAILABLE(); }
String Device::vendorName() ثابت { OCL_NOT_AVAILABLE(); }
String Device::OpenCL_C_Version() ثابت { OCL_NOT_AVAILABLE(); }
String Device::OpenCLVersion() ثابت { OCL_NOT_AVAILABLE(); }
صحیح Device::deviceVersionMajor() ثابت { OCL_NOT_AVAILABLE(); }
صحیح Device::deviceVersionMinor() ثابت { OCL_NOT_AVAILABLE(); }
String Device::driverVersion() ثابت { OCL_NOT_AVAILABLE(); }
تهی* Device::ptr() ثابت { /*OCL_NOT_AVAILABLE();*/ بازگشت پوچ; }

صحیح Device::type() ثابت { OCL_NOT_AVAILABLE(); }

صحیح Device::addressBits() ثابت { OCL_NOT_AVAILABLE(); }
منطقی Device::available() ثابت { OCL_NOT_AVAILABLE(); }
منطقی Device::compilerAvailable() ثابت { OCL_NOT_AVAILABLE(); }
منطقی Device::linkerAvailable() ثابت { OCL_NOT_AVAILABLE(); }

صحیح Device::doubleFPConfig() ثابت { OCL_NOT_AVAILABLE(); }
صحیح Device::singleFPConfig() ثابت { OCL_NOT_AVAILABLE(); }
صحیح Device::halfFPConfig() ثابت { OCL_NOT_AVAILABLE(); }

منطقی Device::hasFP64() ثابت { OCL_NOT_AVAILABLE(); }
منطقی Device::hasFP16() ثابت { OCL_NOT_AVAILABLE(); }

منطقی Device::endianLittle() ثابت { OCL_NOT_AVAILABLE(); }
منطقی Device::errorCorrectionSupport() ثابت { OCL_NOT_AVAILABLE(); }

صحیح Device::executionCapabilities() ثابت { OCL_NOT_AVAILABLE(); }

size_t Device::globalMemCacheSize() ثابت { OCL_NOT_AVAILABLE(); }

صحیح Device::globalMemCacheType() ثابت { OCL_NOT_AVAILABLE(); }
صحیح Device::global_Mem_Cache_سطر_Size() ثابت { OCL_NOT_AVAILABLE(); }
size_t Device::globalMemSize() ثابت { OCL_NOT_AVAILABLE(); }

size_t Device::localMemSize() ثابت { OCL_NOT_AVAILABLE(); }
صحیح Device::localMemType() ثابت { بازگشت NO_LOCAL_MEM; }
منطقی Device::hostUnifiedMemory() ثابت { OCL_NOT_AVAILABLE(); }

منطقی Device::imageSupport() ثابت { OCL_NOT_AVAILABLE(); }

منطقی Device::imageFromBufferSupport() ثابت { OCL_NOT_AVAILABLE(); }
uint Device::imagePitchAlignment() ثابت { OCL_NOT_AVAILABLE(); }
uint Device::imageBaseAddressAlignment() ثابت { OCL_NOT_AVAILABLE(); }

منطقی Device::intelSubgroupsSupport() ثابت { OCL_NOT_AVAILABLE(); }

size_t Device::image2DMaxWidth() ثابت { OCL_NOT_AVAILABLE(); }
size_t Device::image2DMaxHeight() ثابت { OCL_NOT_AVAILABLE(); }

size_t Device::image3DMaxWidth() ثابت { OCL_NOT_AVAILABLE(); }
size_t Device::image3DMaxHeight() ثابت { OCL_NOT_AVAILABLE(); }
size_t Device::image3DMaxDepth() ثابت { OCL_NOT_AVAILABLE(); }

size_t Device::imageMaxBufferSize() ثابت { OCL_NOT_AVAILABLE(); }
size_t Device::imageMaxArraySize() ثابت { OCL_NOT_AVAILABLE(); }

صحیح Device::vendorID() ثابت { OCL_NOT_AVAILABLE(); }

صحیح Device::maxClockFrequency() ثابت { OCL_NOT_AVAILABLE(); }
صحیح Device::maxComputeUnits() ثابت { OCL_NOT_AVAILABLE(); }
صحیح Device::maxConstantArgs() ثابت { OCL_NOT_AVAILABLE(); }
size_t Device::maxConstantBufferSize() ثابت { OCL_NOT_AVAILABLE(); }

size_t Device::maxMemAllocSize() ثابت { OCL_NOT_AVAILABLE(); }
size_t Device::maxParameterSize() ثابت { OCL_NOT_AVAILABLE(); }

صحیح Device::maxReadImageArgs() ثابت { OCL_NOT_AVAILABLE(); }
صحیح Device::maxWriteImageArgs() ثابت { OCL_NOT_AVAILABLE(); }
صحیح Device::maxSamplers() ثابت { OCL_NOT_AVAILABLE(); }

size_t Device::maxWorkGroupSize() ثابت { OCL_NOT_AVAILABLE(); }
صحیح Device::maxWorkItemDims() ثابت { OCL_NOT_AVAILABLE(); }
تهی Device::maxWorkItemSizes(size_t*) ثابت { OCL_NOT_AVAILABLE(); }

صحیح Device::memBaseAddrAlign() ثابت { OCL_NOT_AVAILABLE(); }

صحیح Device::nativeVectorWidthChar() ثابت { OCL_NOT_AVAILABLE(); }
صحیح Device::nativeVectorWidthShort() ثابت { OCL_NOT_AVAILABLE(); }
صحیح Device::nativeVectorWidthInt() ثابت { OCL_NOT_AVAILABLE(); }
صحیح Device::nativeVectorWidthLong() ثابت { OCL_NOT_AVAILABLE(); }
صحیح Device::nativeVectorWidthFloat() ثابت { OCL_NOT_AVAILABLE(); }
صحیح Device::nativeVectorWidthDouble() ثابت { OCL_NOT_AVAILABLE(); }
صحیح Device::nativeVectorWidthHalf() ثابت { OCL_NOT_AVAILABLE(); }

صحیح Device::preferredVectorWidthChar() ثابت { OCL_NOT_AVAILABLE(); }
صحیح Device::preferredVectorWidthShort() ثابت { OCL_NOT_AVAILABLE(); }
صحیح Device::preferredVectorWidthInt() ثابت { OCL_NOT_AVAILABLE(); }
صحیح Device::preferredVectorWidthLong() ثابت { OCL_NOT_AVAILABLE(); }
صحیح Device::preferredVectorWidthFloat() ثابت { OCL_NOT_AVAILABLE(); }
صحیح Device::preferredVectorWidthDouble() ثابت { OCL_NOT_AVAILABLE(); }
صحیح Device::preferredVectorWidthHalf() ثابت { OCL_NOT_AVAILABLE(); }

size_t Device::printfBufferSize() ثابت { OCL_NOT_AVAILABLE(); }
size_t Device::profilingTimerResolution() ثابت { OCL_NOT_AVAILABLE(); }

/* static */
ثابت Device& Device::getDefault()
{
    ایستا Device dummy;
    بازگشت dummy;
}

/* static */ Device Device::fromHandle(تهی* d) { OCL_NOT_AVAILABLE(); }


Context::Context() CV_NOEXCEPT : p(پوچ) { }
Context::Context(صحیح dtype) : p(پوچ) { }
Context::~Context() { }
Context::Context(ثابت Context& c) : p(پوچ) { }
Context& Context::operator=(ثابت Context& c) { بازگشت *این; }
Context::Context(Context&&) CV_NOEXCEPT : p(پوچ) { }
Context& Context::operator=(Context&&) CV_NOEXCEPT { بازگشت *این; }

منطقی Context::create() { بازگشت نادرست; }
منطقی Context::create(صحیح dtype) { بازگشت نادرست; }
size_t Context::ndevices() ثابت { بازگشت 0; }
Device& Context::device(size_t idx) ثابت { OCL_NOT_AVAILABLE(); }
Program Context::getProg(ثابت ProgramSource& prog, ثابت String& buildopt, String& errmsg) { OCL_NOT_AVAILABLE(); }
تهی Context::unloadProg(Program& prog) { }

/* static */
Context& Context::getDefault()
{
    ایستا Context dummy;
    بازگشت dummy;
}
تهی* Context::ptr() ثابت { بازگشت پوچ; }

تهی* Context::getOpenCLContextProperty(صحیح /*propertyId*/) ثابت { OCL_NOT_AVAILABLE(); }

منطقی Context::useSVM() ثابت { بازگشت نادرست; }
تهی Context::setUseSVM(منطقی enabled) { }

Context::UserContext::~UserContext() { }

تهی Context::setUserContext(std::type_index /*typeId*/, ثابت std::shared_ptr<Context::UserContext>& /*userContext*/) { OCL_NOT_AVAILABLE(); }
std::shared_ptr<Context::UserContext> Context::getUserContext(std::type_index /*typeId*/) { OCL_NOT_AVAILABLE(); }

/* static */ Context Context::fromHandle(تهی* context) { OCL_NOT_AVAILABLE(); }
/* static */ Context Context::fromDevice(ثابت ocl::Device& device) { OCL_NOT_AVAILABLE(); }
/* static */ Context Context::create(ثابت std::string& configuration) { OCL_NOT_AVAILABLE(); }

تهی Context::release() { }


Platform::Platform() CV_NOEXCEPT : p(پوچ) { }
Platform::~Platform() { }
Platform::Platform(ثابت Platform&) : p(پوچ) { }
Platform& Platform::operator=(ثابت Platform&) { بازگشت *این; }
Platform::Platform(Platform&&) CV_NOEXCEPT : p(پوچ) { }
Platform& Platform::operator=(Platform&&) CV_NOEXCEPT { بازگشت *این; }

تهی* Platform::ptr() ثابت { بازگشت پوچ; }

/* static */
Platform& Platform::getDefault()
{
    ایستا Platform dummy;
    بازگشت dummy;
}

تهی attachContext(ثابت String& platformName, تهی* platformID, تهی* context, تهی* deviceID) { OCL_NOT_AVAILABLE(); }
تهی convertFromBuffer(تهی* cl_mem_buffer, size_t step, صحیح rows, صحیح cols, صحیح type, UMat& dst) { OCL_NOT_AVAILABLE(); }
تهی convertFromImage(تهی* cl_mem_image, UMat& dst) { OCL_NOT_AVAILABLE(); }

تهی initializeContextFromHandle(Context& ctx, تهی* platform, تهی* context, تهی* device) { OCL_NOT_AVAILABLE(); }

Queue::Queue() CV_NOEXCEPT : p(پوچ) { }
Queue::Queue(ثابت Context& c, ثابت Device& d) : p(پوچ) { OCL_NOT_AVAILABLE(); }
Queue::~Queue() { }
Queue::Queue(ثابت Queue& q) {}
Queue& Queue::operator=(ثابت Queue& q) { بازگشت *این; }
Queue::Queue(Queue&&) CV_NOEXCEPT : p(پوچ) { }
Queue& Queue::operator=(Queue&&) CV_NOEXCEPT { بازگشت *این; }

منطقی Queue::create(ثابت Context& c, ثابت Device& d) { OCL_NOT_AVAILABLE(); }
تهی Queue::finish() {}
تهی* Queue::ptr() ثابت { بازگشت پوچ; }
/* static */
Queue& Queue::getDefault()
{
    ایستا Queue dummy;
    بازگشت dummy;
}

/// @brief Returns OpenCL command queue with enable profiling mode support
ثابت Queue& Queue::getProfilingQueue() ثابت { OCL_NOT_AVAILABLE(); }


KernelArg::KernelArg() CV_NOEXCEPT
    : flags(0), m(0), obj(0), sz(0), wscale(1), iwscale(1)
{
}

KernelArg::KernelArg(صحیح _flags, UMat* _m, صحیح _wscale, صحیح _iwscale, ثابت تهی* _obj, size_t _sz)
    : flags(_flags), m(_m), obj(_obj), sz(_sz), wscale(_wscale), iwscale(_iwscale)
{
    OCL_NOT_AVAILABLE();
}

KernelArg KernelArg::Constant(ثابت Mat& m)
{
    OCL_NOT_AVAILABLE();
}


Kernel::Kernel() CV_NOEXCEPT : p(پوچ) { }
Kernel::Kernel(ثابت نویسه* kname, ثابت Program& prog) : p(پوچ) { OCL_NOT_AVAILABLE(); }
Kernel::Kernel(ثابت نویسه* kname, ثابت ProgramSource& prog, ثابت String& buildopts, String* errmsg) : p(پوچ) { OCL_NOT_AVAILABLE(); }
Kernel::~Kernel() { }
Kernel::Kernel(ثابت Kernel& k) : p(پوچ) { }
Kernel& Kernel::operator=(ثابت Kernel& k) { بازگشت *این; }
Kernel::Kernel(Kernel&&) CV_NOEXCEPT : p(پوچ) { }
Kernel& Kernel::operator=(Kernel&&) CV_NOEXCEPT { بازگشت *این; }

منطقی Kernel::empty() ثابت { بازگشت درست; }
منطقی Kernel::create(ثابت نویسه* kname, ثابت Program& prog) { OCL_NOT_AVAILABLE(); }
منطقی Kernel::create(ثابت نویسه* kname, ثابت ProgramSource& prog, ثابت String& buildopts, String* errmsg) { OCL_NOT_AVAILABLE(); }

صحیح Kernel::set(صحیح i, ثابت تهی* value, size_t sz) { OCL_NOT_AVAILABLE(); }
صحیح Kernel::set(صحیح i, ثابت Image2D& image2D) { OCL_NOT_AVAILABLE(); }
صحیح Kernel::set(صحیح i, ثابت UMat& m) { OCL_NOT_AVAILABLE(); }
صحیح Kernel::set(صحیح i, ثابت KernelArg& arg) { OCL_NOT_AVAILABLE(); }

منطقی Kernel::run(صحیح dims, size_t globalsize[], size_t localsize[], منطقی همگام, ثابت Queue& q) { OCL_NOT_AVAILABLE(); }
منطقی Kernel::runTask(منطقی همگام, ثابت Queue& q) { OCL_NOT_AVAILABLE(); }

int64 Kernel::runProfiling(صحیح dims, size_t globalsize[], size_t localsize[], ثابت Queue& q) { OCL_NOT_AVAILABLE(); }

size_t Kernel::workGroupSize() ثابت { OCL_NOT_AVAILABLE(); }
size_t Kernel::preferedWorkGroupSizeMultiple() ثابت { OCL_NOT_AVAILABLE(); }
منطقی Kernel::compileWorkGroupSize(size_t wsz[]) ثابت { OCL_NOT_AVAILABLE(); }
size_t Kernel::localMemSize() ثابت { OCL_NOT_AVAILABLE(); }

تهی* Kernel::ptr() ثابت { بازگشت پوچ; }


Program::Program() CV_NOEXCEPT : p(پوچ) { }
Program::Program(ثابت ProgramSource& src, ثابت String& buildflags, String& errmsg) : p(پوچ) { OCL_NOT_AVAILABLE(); }
Program::Program(ثابت Program& prog) : p(پوچ) { }
Program& Program::operator=(ثابت Program& prog) { بازگشت *این; }
Program::Program(Program&&) CV_NOEXCEPT : p(پوچ) { }
Program& Program::operator=(Program&&) CV_NOEXCEPT { بازگشت *این; }
Program::~Program() { }

منطقی Program::create(ثابت ProgramSource& src, ثابت String& buildflags, String& errmsg) { OCL_NOT_AVAILABLE(); }

تهی* Program::ptr() ثابت { بازگشت پوچ; }

تهی Program::get_دودویی(std::vector<نویسه>& binary) ثابت { OCL_NOT_AVAILABLE(); }

منطقی Program::اقرأ(ثابت String& buf, ثابت String& buildflags) { OCL_NOT_AVAILABLE(); }
منطقی Program::اكتب(String& buf) ثابت { OCL_NOT_AVAILABLE(); }
ثابت ProgramSource& Program::source() ثابت { OCL_NOT_AVAILABLE(); }
String Program::getPrefix() ثابت { OCL_NOT_AVAILABLE(); }
/* static */ String Program::getPrefix(ثابت String& buildflags) { OCL_NOT_AVAILABLE(); }


ProgramSource::ProgramSource() CV_NOEXCEPT : p(پوچ) { }
ProgramSource::ProgramSource(ثابت String& module, ثابت String& name, ثابت String& codeStr, ثابت String& codeHash) : p(پوچ) { }
ProgramSource::ProgramSource(ثابت String& prog) : p(پوچ) { }
ProgramSource::ProgramSource(ثابت نویسه* prog) : p(پوچ) { }
ProgramSource::~ProgramSource() { }
ProgramSource::ProgramSource(ثابت ProgramSource& prog) : p(پوچ) { }
ProgramSource& ProgramSource::operator=(ثابت ProgramSource& prog) { بازگشت *این; }
ProgramSource::ProgramSource(ProgramSource&&) CV_NOEXCEPT : p(پوچ) { }
ProgramSource& ProgramSource::operator=(ProgramSource&&) CV_NOEXCEPT { بازگشت *این; }

ثابت String& ProgramSource::source() ثابت { OCL_NOT_AVAILABLE(); }
ProgramSource::hash_t ProgramSource::hash() ثابت { OCL_NOT_AVAILABLE(); }

/* static */ ProgramSource ProgramSource::from_دودویی(ثابت String& module, ثابت String& name, ثابت unsigned نویسه* binary, ثابت size_t size, ثابت cv::String& buildOptions) { OCL_NOT_AVAILABLE(); }
/* static */ ProgramSource ProgramSource::fromSPIR(ثابت String& module, ثابت String& name, ثابت unsigned نویسه* binary, ثابت size_t size, ثابت cv::String& buildOptions) { OCL_NOT_AVAILABLE(); }


PlatformInfo::PlatformInfo() CV_NOEXCEPT : p(پوچ) { }
PlatformInfo::PlatformInfo(تهی* id) : p(پوچ) { OCL_NOT_AVAILABLE(); }
PlatformInfo::~PlatformInfo() { }

PlatformInfo::PlatformInfo(ثابت PlatformInfo& i) : p(پوچ) { }
PlatformInfo& PlatformInfo::operator=(ثابت PlatformInfo& i) { بازگشت *این; }
PlatformInfo::PlatformInfo(PlatformInfo&&) CV_NOEXCEPT : p(پوچ) { }
PlatformInfo& PlatformInfo::operator=(PlatformInfo&&) CV_NOEXCEPT { بازگشت *این; }

String PlatformInfo::name() ثابت { OCL_NOT_AVAILABLE(); }
String PlatformInfo::vendor() ثابت { OCL_NOT_AVAILABLE(); }
String PlatformInfo::version() ثابت { OCL_NOT_AVAILABLE(); }
صحیح PlatformInfo::device_عدد() ثابت { OCL_NOT_AVAILABLE(); }
تهی PlatformInfo::getDevice(Device& device, صحیح d) ثابت { OCL_NOT_AVAILABLE(); }

ثابت نویسه* convertTypeStr(صحیح sdepth, صحیح ddepth, صحیح cn, نویسه* buf) { OCL_NOT_AVAILABLE(); }
ثابت نویسه* typeToStr(صحیح t) { OCL_NOT_AVAILABLE(); }
ثابت نویسه* memopTypeToStr(صحیح t) { OCL_NOT_AVAILABLE(); }
ثابت نویسه* vecopTypeToStr(صحیح t) { OCL_NOT_AVAILABLE(); }
ثابت نویسه* getOpenCLErrorString(صحیح errorCode) { OCL_NOT_AVAILABLE(); }
String kernelToStr(درون_داد_Array _kernel, صحیح ddepth, ثابت نویسه* name) { OCL_NOT_AVAILABLE(); }
تهی getPlatfomsInfo(std::vector<PlatformInfo>& platform_info) { OCL_NOT_AVAILABLE(); }


صحیح predictOptimalVectorWidth(درون_داد_Array src1, درون_داد_Array src2, درون_داد_Array src3,
        درون_داد_Array src4, درون_داد_Array src5, درون_داد_Array src6,
        درون_داد_Array src7, درون_داد_Array src8, درون_داد_Array src9,
        OclVectorStrategy strat)
{ OCL_NOT_AVAILABLE(); }

صحیح checkOptimalVectorWidth(ثابت صحیح *vectorWidths,
        درون_داد_Array src1, درون_داد_Array src2, درون_داد_Array src3,
        درون_داد_Array src4, درون_داد_Array src5, درون_داد_Array src6,
        درون_داد_Array src7, درون_داد_Array src8, درون_داد_Array src9,
        OclVectorStrategy strat)
{ OCL_NOT_AVAILABLE(); }

صحیح predictOptimalVectorWidthMax(درون_داد_Array src1, درون_داد_Array src2, درون_داد_Array src3,
        درون_داد_Array src4, درون_داد_Array src5, درون_داد_Array src6,
        درون_داد_Array src7, درون_داد_Array src8, درون_داد_Array src9)
{ OCL_NOT_AVAILABLE(); }

تهی buildOptionsAddMatrixDescription(String& buildOptions, ثابت String& name, درون_داد_Array _m) { OCL_NOT_AVAILABLE(); }


Image2D::Image2D() CV_NOEXCEPT : p(پوچ) { }
Image2D::Image2D(ثابت UMat &src, منطقی norm, منطقی alias) { OCL_NOT_AVAILABLE(); }
Image2D::Image2D(ثابت Image2D & i) : p(پوچ) { OCL_NOT_AVAILABLE(); }
Image2D::~Image2D() { }
Image2D& Image2D::operator=(ثابت Image2D & i) { بازگشت *این; }
Image2D::Image2D(Image2D&&) CV_NOEXCEPT : p(پوچ) { }
Image2D& Image2D::operator=(Image2D&&) CV_NOEXCEPT { بازگشت *این; }

/* static */ منطقی Image2D::canCreateAlias(ثابت UMat &u) { OCL_NOT_AVAILABLE(); }
/* static */ منطقی Image2D::isFormatSupported(صحیح depth, صحیح cn, منطقی norm) { OCL_NOT_AVAILABLE(); }

تهی* Image2D::ptr() ثابت { بازگشت پوچ; }


Timer::Timer(ثابت Queue& q) : p(پوچ) {}
Timer::~Timer() {}
تهی Timer::start() { OCL_NOT_AVAILABLE(); }
تهی Timer::stop() { OCL_NOT_AVAILABLE();}

uint64 Timer::durationNS() ثابت { OCL_NOT_AVAILABLE(); }

MatAllocator* getOpenCLAllocator() { بازگشت پوچ; }

internal::ProgramEntry::operator ProgramSource&() ثابت { OCL_NOT_AVAILABLE(); }


ساختار OpenCLExecutionContext::Impl
{
    Impl() = پیشفرض;
};

Context& OpenCLExecutionContext::getContext() ثابت { OCL_NOT_AVAILABLE(); }
Device& OpenCLExecutionContext::getDevice() ثابت { OCL_NOT_AVAILABLE(); }
Queue& OpenCLExecutionContext::getQueue() ثابت { OCL_NOT_AVAILABLE(); }

منطقی OpenCLExecutionContext::useOpenCL() ثابت { بازگشت نادرست; }
تهی OpenCLExecutionContext::setUseOpenCL(منطقی flag) { }

ایستا
OpenCLExecutionContext& getDummyOpenCLExecutionContext()
{
    ایستا OpenCLExecutionContext dummy;
    بازگشت dummy;
}

/* static */
OpenCLExecutionContext& OpenCLExecutionContext::getCurrent() { بازگشت getDummyOpenCLExecutionContext(); }

/* static */
OpenCLExecutionContext& OpenCLExecutionContext::getCurrentRef() { بازگشت getDummyOpenCLExecutionContext(); }

تهی OpenCLExecutionContext::bind() ثابت { OCL_NOT_AVAILABLE(); }

OpenCLExecutionContext OpenCLExecutionContext::cloneWithNewQueue(ثابت ocl::Queue& q) ثابت { OCL_NOT_AVAILABLE(); }
OpenCLExecutionContext OpenCLExecutionContext::cloneWithNewQueue() ثابت { OCL_NOT_AVAILABLE(); }

/* static */ OpenCLExecutionContext OpenCLExecutionContext::create(ثابت std::string& platformName, تهی* platformID, تهی* context, تهی* deviceID) { OCL_NOT_AVAILABLE(); }
/* static */ OpenCLExecutionContext OpenCLExecutionContext::create(ثابت Context& context, ثابت Device& device, ثابت ocl::Queue& queue) { OCL_NOT_AVAILABLE(); }
/* static */ OpenCLExecutionContext OpenCLExecutionContext::create(ثابت Context& context, ثابت Device& device) { OCL_NOT_AVAILABLE(); }

تهی OpenCLExecutionContext::release() { }

}}

#اگر defined(_MSC_VER)
    #pragma warning(pop)
#وگرنهاگر defined(__clang__)
    #pragma clang diagnostic pop
#وگرنهاگر defined(__GNUC__)
    #pragma GCC diagnostic pop
#endif
