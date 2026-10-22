// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ضمن "opencv2/core/ocl_genbase.hpp"

#إذا defined(_MSC_VER)
    #pragma warning(push)
    #pragma warning(disable : 4100)
    #pragma warning(disable : 4702)
#وإلاإذا defined(__clang__)
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wunused-parameter"
#وإلاإذا defined(__GNUC__)
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

نطاق cv { نطاق ocl {

ساكن
CV_NORETURN فراغ throw_no_ocl()
{
    CV_Error(Error::OpenCLApiCallError, "OpenCV build without OpenCL support");
}
#define OCL_NOT_AVAILABLE() throw_no_ocl();

CV_EXPORTS_W منطقي haveOpenCL() { أرجع خطأ; }
CV_EXPORTS_W منطقي useOpenCL() { أرجع خطأ; }
CV_EXPORTS_W منطقي haveAmdBlas() { أرجع خطأ; }
CV_EXPORTS_W منطقي haveAmdFft() { أرجع خطأ; }
CV_EXPORTS_W فراغ setUseOpenCL(منطقي flag) { /* nothing */ }
CV_EXPORTS_W فراغ finish() { /* nothing */ }

CV_EXPORTS منطقي haveSVM() { أرجع خطأ; }

Device::Device() CV_NOEXCEPT : p(عدم) { }
Device::Device(فراغ* d) : p(عدم) { OCL_NOT_AVAILABLE(); }
Device::Device(ثابت Device& d) : p(عدم) { }
Device& Device::operator=(ثابت Device& d) { أرجع *هذا; }
Device::Device(Device&&) CV_NOEXCEPT : p(عدم) { }
Device& Device::operator=(Device&&) CV_NOEXCEPT { أرجع *هذا; }
Device::~Device() { }

فراغ Device::set(فراغ* d) { OCL_NOT_AVAILABLE(); }

String Device::name() ثابت { OCL_NOT_AVAILABLE(); }
String Device::extensions() ثابت { OCL_NOT_AVAILABLE(); }
منطقي Device::isExtensionSupported(ثابت String& extensionName) ثابت { OCL_NOT_AVAILABLE(); }
String Device::version() ثابت { OCL_NOT_AVAILABLE(); }
String Device::vendorName() ثابت { OCL_NOT_AVAILABLE(); }
String Device::OpenCL_C_Version() ثابت { OCL_NOT_AVAILABLE(); }
String Device::OpenCLVersion() ثابت { OCL_NOT_AVAILABLE(); }
صحيح Device::deviceVersionMajor() ثابت { OCL_NOT_AVAILABLE(); }
صحيح Device::deviceVersionMinor() ثابت { OCL_NOT_AVAILABLE(); }
String Device::driverVersion() ثابت { OCL_NOT_AVAILABLE(); }
فراغ* Device::ptr() ثابت { /*OCL_NOT_AVAILABLE();*/ أرجع عدم; }

صحيح Device::type() ثابت { OCL_NOT_AVAILABLE(); }

صحيح Device::addressBits() ثابت { OCL_NOT_AVAILABLE(); }
منطقي Device::available() ثابت { OCL_NOT_AVAILABLE(); }
منطقي Device::compilerAvailable() ثابت { OCL_NOT_AVAILABLE(); }
منطقي Device::linkerAvailable() ثابت { OCL_NOT_AVAILABLE(); }

صحيح Device::doubleFPConfig() ثابت { OCL_NOT_AVAILABLE(); }
صحيح Device::singleFPConfig() ثابت { OCL_NOT_AVAILABLE(); }
صحيح Device::halfFPConfig() ثابت { OCL_NOT_AVAILABLE(); }

منطقي Device::hasFP64() ثابت { OCL_NOT_AVAILABLE(); }
منطقي Device::hasFP16() ثابت { OCL_NOT_AVAILABLE(); }

منطقي Device::endianLittle() ثابت { OCL_NOT_AVAILABLE(); }
منطقي Device::errorCorrectionSupport() ثابت { OCL_NOT_AVAILABLE(); }

صحيح Device::executionCapabilities() ثابت { OCL_NOT_AVAILABLE(); }

size_t Device::globalMemCacheSize() ثابت { OCL_NOT_AVAILABLE(); }

صحيح Device::globalMemCacheType() ثابت { OCL_NOT_AVAILABLE(); }
صحيح Device::global_Mem_Cache_سطر_Size() ثابت { OCL_NOT_AVAILABLE(); }
size_t Device::globalMemSize() ثابت { OCL_NOT_AVAILABLE(); }

size_t Device::localMemSize() ثابت { OCL_NOT_AVAILABLE(); }
صحيح Device::localMemType() ثابت { أرجع NO_LOCAL_MEM; }
منطقي Device::hostUnifiedMemory() ثابت { OCL_NOT_AVAILABLE(); }

منطقي Device::imageSupport() ثابت { OCL_NOT_AVAILABLE(); }

منطقي Device::imageFromBufferSupport() ثابت { OCL_NOT_AVAILABLE(); }
uint Device::imagePitchAlignment() ثابت { OCL_NOT_AVAILABLE(); }
uint Device::imageBaseAddressAlignment() ثابت { OCL_NOT_AVAILABLE(); }

منطقي Device::intelSubgroupsSupport() ثابت { OCL_NOT_AVAILABLE(); }

size_t Device::image2DMaxWidth() ثابت { OCL_NOT_AVAILABLE(); }
size_t Device::image2DMaxHeight() ثابت { OCL_NOT_AVAILABLE(); }

size_t Device::image3DMaxWidth() ثابت { OCL_NOT_AVAILABLE(); }
size_t Device::image3DMaxHeight() ثابت { OCL_NOT_AVAILABLE(); }
size_t Device::image3DMaxDepth() ثابت { OCL_NOT_AVAILABLE(); }

size_t Device::imageMaxBufferSize() ثابت { OCL_NOT_AVAILABLE(); }
size_t Device::imageMaxArraySize() ثابت { OCL_NOT_AVAILABLE(); }

صحيح Device::vendorID() ثابت { OCL_NOT_AVAILABLE(); }

صحيح Device::maxClockFrequency() ثابت { OCL_NOT_AVAILABLE(); }
صحيح Device::maxComputeUnits() ثابت { OCL_NOT_AVAILABLE(); }
صحيح Device::maxConstantArgs() ثابت { OCL_NOT_AVAILABLE(); }
size_t Device::maxConstantBufferSize() ثابت { OCL_NOT_AVAILABLE(); }

size_t Device::maxMemAllocSize() ثابت { OCL_NOT_AVAILABLE(); }
size_t Device::maxParameterSize() ثابت { OCL_NOT_AVAILABLE(); }

صحيح Device::maxReadImageArgs() ثابت { OCL_NOT_AVAILABLE(); }
صحيح Device::maxWriteImageArgs() ثابت { OCL_NOT_AVAILABLE(); }
صحيح Device::maxSamplers() ثابت { OCL_NOT_AVAILABLE(); }

size_t Device::maxWorkGroupSize() ثابت { OCL_NOT_AVAILABLE(); }
صحيح Device::maxWorkItemDims() ثابت { OCL_NOT_AVAILABLE(); }
فراغ Device::maxWorkItemSizes(size_t*) ثابت { OCL_NOT_AVAILABLE(); }

صحيح Device::memBaseAddrAlign() ثابت { OCL_NOT_AVAILABLE(); }

صحيح Device::nativeVectorWidthChar() ثابت { OCL_NOT_AVAILABLE(); }
صحيح Device::nativeVectorWidthShort() ثابت { OCL_NOT_AVAILABLE(); }
صحيح Device::nativeVectorWidthInt() ثابت { OCL_NOT_AVAILABLE(); }
صحيح Device::nativeVectorWidthLong() ثابت { OCL_NOT_AVAILABLE(); }
صحيح Device::nativeVectorWidthFloat() ثابت { OCL_NOT_AVAILABLE(); }
صحيح Device::nativeVectorWidthDouble() ثابت { OCL_NOT_AVAILABLE(); }
صحيح Device::nativeVectorWidthHalf() ثابت { OCL_NOT_AVAILABLE(); }

صحيح Device::preferredVectorWidthChar() ثابت { OCL_NOT_AVAILABLE(); }
صحيح Device::preferredVectorWidthShort() ثابت { OCL_NOT_AVAILABLE(); }
صحيح Device::preferredVectorWidthInt() ثابت { OCL_NOT_AVAILABLE(); }
صحيح Device::preferredVectorWidthLong() ثابت { OCL_NOT_AVAILABLE(); }
صحيح Device::preferredVectorWidthFloat() ثابت { OCL_NOT_AVAILABLE(); }
صحيح Device::preferredVectorWidthDouble() ثابت { OCL_NOT_AVAILABLE(); }
صحيح Device::preferredVectorWidthHalf() ثابت { OCL_NOT_AVAILABLE(); }

size_t Device::printfBufferSize() ثابت { OCL_NOT_AVAILABLE(); }
size_t Device::profilingTimerResolution() ثابت { OCL_NOT_AVAILABLE(); }

/* static */
ثابت Device& Device::getDefault()
{
    ساكن Device dummy;
    أرجع dummy;
}

/* static */ Device Device::fromHandle(فراغ* d) { OCL_NOT_AVAILABLE(); }


Context::Context() CV_NOEXCEPT : p(عدم) { }
Context::Context(صحيح dtype) : p(عدم) { }
Context::~Context() { }
Context::Context(ثابت Context& c) : p(عدم) { }
Context& Context::operator=(ثابت Context& c) { أرجع *هذا; }
Context::Context(Context&&) CV_NOEXCEPT : p(عدم) { }
Context& Context::operator=(Context&&) CV_NOEXCEPT { أرجع *هذا; }

منطقي Context::create() { أرجع خطأ; }
منطقي Context::create(صحيح dtype) { أرجع خطأ; }
size_t Context::ndevices() ثابت { أرجع 0; }
Device& Context::device(size_t idx) ثابت { OCL_NOT_AVAILABLE(); }
Program Context::getProg(ثابت ProgramSource& prog, ثابت String& buildopt, String& errmsg) { OCL_NOT_AVAILABLE(); }
فراغ Context::unloadProg(Program& prog) { }

/* static */
Context& Context::getDefault()
{
    ساكن Context dummy;
    أرجع dummy;
}
فراغ* Context::ptr() ثابت { أرجع عدم; }

فراغ* Context::getOpenCLContextProperty(صحيح /*propertyId*/) ثابت { OCL_NOT_AVAILABLE(); }

منطقي Context::useSVM() ثابت { أرجع خطأ; }
فراغ Context::setUseSVM(منطقي enabled) { }

Context::UserContext::~UserContext() { }

فراغ Context::setUserContext(std::type_index /*typeId*/, ثابت std::shared_ptr<Context::UserContext>& /*userContext*/) { OCL_NOT_AVAILABLE(); }
std::shared_ptr<Context::UserContext> Context::getUserContext(std::type_index /*typeId*/) { OCL_NOT_AVAILABLE(); }

/* static */ Context Context::fromHandle(فراغ* context) { OCL_NOT_AVAILABLE(); }
/* static */ Context Context::fromDevice(ثابت ocl::Device& device) { OCL_NOT_AVAILABLE(); }
/* static */ Context Context::create(ثابت std::string& configuration) { OCL_NOT_AVAILABLE(); }

فراغ Context::release() { }


Platform::Platform() CV_NOEXCEPT : p(عدم) { }
Platform::~Platform() { }
Platform::Platform(ثابت Platform&) : p(عدم) { }
Platform& Platform::operator=(ثابت Platform&) { أرجع *هذا; }
Platform::Platform(Platform&&) CV_NOEXCEPT : p(عدم) { }
Platform& Platform::operator=(Platform&&) CV_NOEXCEPT { أرجع *هذا; }

فراغ* Platform::ptr() ثابت { أرجع عدم; }

/* static */
Platform& Platform::getDefault()
{
    ساكن Platform dummy;
    أرجع dummy;
}

فراغ attachContext(ثابت String& platformName, فراغ* platformID, فراغ* context, فراغ* deviceID) { OCL_NOT_AVAILABLE(); }
فراغ convertFromBuffer(فراغ* cl_mem_buffer, size_t step, صحيح rows, صحيح cols, صحيح type, UMat& dst) { OCL_NOT_AVAILABLE(); }
فراغ convertFromImage(فراغ* cl_mem_image, UMat& dst) { OCL_NOT_AVAILABLE(); }

فراغ initializeContextFromHandle(Context& ctx, فراغ* platform, فراغ* context, فراغ* device) { OCL_NOT_AVAILABLE(); }

Queue::Queue() CV_NOEXCEPT : p(عدم) { }
Queue::Queue(ثابت Context& c, ثابت Device& d) : p(عدم) { OCL_NOT_AVAILABLE(); }
Queue::~Queue() { }
Queue::Queue(ثابت Queue& q) {}
Queue& Queue::operator=(ثابت Queue& q) { أرجع *هذا; }
Queue::Queue(Queue&&) CV_NOEXCEPT : p(عدم) { }
Queue& Queue::operator=(Queue&&) CV_NOEXCEPT { أرجع *هذا; }

منطقي Queue::create(ثابت Context& c, ثابت Device& d) { OCL_NOT_AVAILABLE(); }
فراغ Queue::finish() {}
فراغ* Queue::ptr() ثابت { أرجع عدم; }
/* static */
Queue& Queue::getDefault()
{
    ساكن Queue dummy;
    أرجع dummy;
}

/// @brief Returns OpenCL command queue with enable profiling mode support
ثابت Queue& Queue::getProfilingQueue() ثابت { OCL_NOT_AVAILABLE(); }


KernelArg::KernelArg() CV_NOEXCEPT
    : flags(0), m(0), obj(0), sz(0), wscale(1), iwscale(1)
{
}

KernelArg::KernelArg(صحيح _flags, UMat* _m, صحيح _wscale, صحيح _iwscale, ثابت فراغ* _obj, size_t _sz)
    : flags(_flags), m(_m), obj(_obj), sz(_sz), wscale(_wscale), iwscale(_iwscale)
{
    OCL_NOT_AVAILABLE();
}

KernelArg KernelArg::Constant(ثابت Mat& m)
{
    OCL_NOT_AVAILABLE();
}


Kernel::Kernel() CV_NOEXCEPT : p(عدم) { }
Kernel::Kernel(ثابت حرف* kname, ثابت Program& prog) : p(عدم) { OCL_NOT_AVAILABLE(); }
Kernel::Kernel(ثابت حرف* kname, ثابت ProgramSource& prog, ثابت String& buildopts, String* errmsg) : p(عدم) { OCL_NOT_AVAILABLE(); }
Kernel::~Kernel() { }
Kernel::Kernel(ثابت Kernel& k) : p(عدم) { }
Kernel& Kernel::operator=(ثابت Kernel& k) { أرجع *هذا; }
Kernel::Kernel(Kernel&&) CV_NOEXCEPT : p(عدم) { }
Kernel& Kernel::operator=(Kernel&&) CV_NOEXCEPT { أرجع *هذا; }

منطقي Kernel::empty() ثابت { أرجع true; }
منطقي Kernel::create(ثابت حرف* kname, ثابت Program& prog) { OCL_NOT_AVAILABLE(); }
منطقي Kernel::create(ثابت حرف* kname, ثابت ProgramSource& prog, ثابت String& buildopts, String* errmsg) { OCL_NOT_AVAILABLE(); }

صحيح Kernel::set(صحيح i, ثابت فراغ* value, size_t sz) { OCL_NOT_AVAILABLE(); }
صحيح Kernel::set(صحيح i, ثابت Image2D& image2D) { OCL_NOT_AVAILABLE(); }
صحيح Kernel::set(صحيح i, ثابت UMat& m) { OCL_NOT_AVAILABLE(); }
صحيح Kernel::set(صحيح i, ثابت KernelArg& arg) { OCL_NOT_AVAILABLE(); }

منطقي Kernel::run(صحيح dims, size_t globalsize[], size_t localsize[], منطقي متزامن, ثابت Queue& q) { OCL_NOT_AVAILABLE(); }
منطقي Kernel::runTask(منطقي متزامن, ثابت Queue& q) { OCL_NOT_AVAILABLE(); }

int64 Kernel::runProfiling(صحيح dims, size_t globalsize[], size_t localsize[], ثابت Queue& q) { OCL_NOT_AVAILABLE(); }

size_t Kernel::workGroupSize() ثابت { OCL_NOT_AVAILABLE(); }
size_t Kernel::preferedWorkGroupSizeMultiple() ثابت { OCL_NOT_AVAILABLE(); }
منطقي Kernel::compileWorkGroupSize(size_t wsz[]) ثابت { OCL_NOT_AVAILABLE(); }
size_t Kernel::localMemSize() ثابت { OCL_NOT_AVAILABLE(); }

فراغ* Kernel::ptr() ثابت { أرجع عدم; }


Program::Program() CV_NOEXCEPT : p(عدم) { }
Program::Program(ثابت ProgramSource& src, ثابت String& buildflags, String& errmsg) : p(عدم) { OCL_NOT_AVAILABLE(); }
Program::Program(ثابت Program& prog) : p(عدم) { }
Program& Program::operator=(ثابت Program& prog) { أرجع *هذا; }
Program::Program(Program&&) CV_NOEXCEPT : p(عدم) { }
Program& Program::operator=(Program&&) CV_NOEXCEPT { أرجع *هذا; }
Program::~Program() { }

منطقي Program::create(ثابت ProgramSource& src, ثابت String& buildflags, String& errmsg) { OCL_NOT_AVAILABLE(); }

فراغ* Program::ptr() ثابت { أرجع عدم; }

فراغ Program::get_ثنائی(std::vector<حرف>& binary) ثابت { OCL_NOT_AVAILABLE(); }

منطقي Program::اقرأ(ثابت String& buf, ثابت String& buildflags) { OCL_NOT_AVAILABLE(); }
منطقي Program::اكتب(String& buf) ثابت { OCL_NOT_AVAILABLE(); }
ثابت ProgramSource& Program::source() ثابت { OCL_NOT_AVAILABLE(); }
String Program::getPrefix() ثابت { OCL_NOT_AVAILABLE(); }
/* static */ String Program::getPrefix(ثابت String& buildflags) { OCL_NOT_AVAILABLE(); }


ProgramSource::ProgramSource() CV_NOEXCEPT : p(عدم) { }
ProgramSource::ProgramSource(ثابت String& module, ثابت String& name, ثابت String& codeStr, ثابت String& codeHash) : p(عدم) { }
ProgramSource::ProgramSource(ثابت String& prog) : p(عدم) { }
ProgramSource::ProgramSource(ثابت حرف* prog) : p(عدم) { }
ProgramSource::~ProgramSource() { }
ProgramSource::ProgramSource(ثابت ProgramSource& prog) : p(عدم) { }
ProgramSource& ProgramSource::operator=(ثابت ProgramSource& prog) { أرجع *هذا; }
ProgramSource::ProgramSource(ProgramSource&&) CV_NOEXCEPT : p(عدم) { }
ProgramSource& ProgramSource::operator=(ProgramSource&&) CV_NOEXCEPT { أرجع *هذا; }

ثابت String& ProgramSource::source() ثابت { OCL_NOT_AVAILABLE(); }
ProgramSource::hash_t ProgramSource::hash() ثابت { OCL_NOT_AVAILABLE(); }

/* static */ ProgramSource ProgramSource::from_ثنائی(ثابت String& module, ثابت String& name, ثابت unsigned حرف* binary, ثابت size_t size, ثابت cv::String& buildOptions) { OCL_NOT_AVAILABLE(); }
/* static */ ProgramSource ProgramSource::fromSPIR(ثابت String& module, ثابت String& name, ثابت unsigned حرف* binary, ثابت size_t size, ثابت cv::String& buildOptions) { OCL_NOT_AVAILABLE(); }


PlatformInfo::PlatformInfo() CV_NOEXCEPT : p(عدم) { }
PlatformInfo::PlatformInfo(فراغ* id) : p(عدم) { OCL_NOT_AVAILABLE(); }
PlatformInfo::~PlatformInfo() { }

PlatformInfo::PlatformInfo(ثابت PlatformInfo& i) : p(عدم) { }
PlatformInfo& PlatformInfo::operator=(ثابت PlatformInfo& i) { أرجع *هذا; }
PlatformInfo::PlatformInfo(PlatformInfo&&) CV_NOEXCEPT : p(عدم) { }
PlatformInfo& PlatformInfo::operator=(PlatformInfo&&) CV_NOEXCEPT { أرجع *هذا; }

String PlatformInfo::name() ثابت { OCL_NOT_AVAILABLE(); }
String PlatformInfo::vendor() ثابت { OCL_NOT_AVAILABLE(); }
String PlatformInfo::version() ثابت { OCL_NOT_AVAILABLE(); }
صحيح PlatformInfo::device_نمبر() ثابت { OCL_NOT_AVAILABLE(); }
فراغ PlatformInfo::getDevice(Device& device, صحيح d) ثابت { OCL_NOT_AVAILABLE(); }

ثابت حرف* convertTypeStr(صحيح sdepth, صحيح ddepth, صحيح cn, حرف* buf) { OCL_NOT_AVAILABLE(); }
ثابت حرف* typeToStr(صحيح t) { OCL_NOT_AVAILABLE(); }
ثابت حرف* memopTypeToStr(صحيح t) { OCL_NOT_AVAILABLE(); }
ثابت حرف* vecopTypeToStr(صحيح t) { OCL_NOT_AVAILABLE(); }
ثابت حرف* getOpenCLErrorString(صحيح errorCode) { OCL_NOT_AVAILABLE(); }
String kernelToStr(داخلہ_Array _kernel, صحيح ddepth, ثابت حرف* name) { OCL_NOT_AVAILABLE(); }
فراغ getPlatfomsInfo(std::vector<PlatformInfo>& platform_info) { OCL_NOT_AVAILABLE(); }


صحيح predictOptimalVectorWidth(داخلہ_Array src1, داخلہ_Array src2, داخلہ_Array src3,
        داخلہ_Array src4, داخلہ_Array src5, داخلہ_Array src6,
        داخلہ_Array src7, داخلہ_Array src8, داخلہ_Array src9,
        OclVectorStrategy strat)
{ OCL_NOT_AVAILABLE(); }

صحيح checkOptimalVectorWidth(ثابت صحيح *vectorWidths,
        داخلہ_Array src1, داخلہ_Array src2, داخلہ_Array src3,
        داخلہ_Array src4, داخلہ_Array src5, داخلہ_Array src6,
        داخلہ_Array src7, داخلہ_Array src8, داخلہ_Array src9,
        OclVectorStrategy strat)
{ OCL_NOT_AVAILABLE(); }

صحيح predictOptimalVectorWidthMax(داخلہ_Array src1, داخلہ_Array src2, داخلہ_Array src3,
        داخلہ_Array src4, داخلہ_Array src5, داخلہ_Array src6,
        داخلہ_Array src7, داخلہ_Array src8, داخلہ_Array src9)
{ OCL_NOT_AVAILABLE(); }

فراغ buildOptionsAddMatrixDescription(String& buildOptions, ثابت String& name, داخلہ_Array _m) { OCL_NOT_AVAILABLE(); }


Image2D::Image2D() CV_NOEXCEPT : p(عدم) { }
Image2D::Image2D(ثابت UMat &src, منطقي norm, منطقي alias) { OCL_NOT_AVAILABLE(); }
Image2D::Image2D(ثابت Image2D & i) : p(عدم) { OCL_NOT_AVAILABLE(); }
Image2D::~Image2D() { }
Image2D& Image2D::operator=(ثابت Image2D & i) { أرجع *هذا; }
Image2D::Image2D(Image2D&&) CV_NOEXCEPT : p(عدم) { }
Image2D& Image2D::operator=(Image2D&&) CV_NOEXCEPT { أرجع *هذا; }

/* static */ منطقي Image2D::canCreateAlias(ثابت UMat &u) { OCL_NOT_AVAILABLE(); }
/* static */ منطقي Image2D::isFormatSupported(صحيح depth, صحيح cn, منطقي norm) { OCL_NOT_AVAILABLE(); }

فراغ* Image2D::ptr() ثابت { أرجع عدم; }


Timer::Timer(ثابت Queue& q) : p(عدم) {}
Timer::~Timer() {}
فراغ Timer::start() { OCL_NOT_AVAILABLE(); }
فراغ Timer::stop() { OCL_NOT_AVAILABLE();}

uint64 Timer::durationNS() ثابت { OCL_NOT_AVAILABLE(); }

MatAllocator* getOpenCLAllocator() { أرجع عدم; }

internal::ProgramEntry::operator ProgramSource&() ثابت { OCL_NOT_AVAILABLE(); }


بنية OpenCLExecutionContext::Impl
{
    Impl() = افتراضي;
};

Context& OpenCLExecutionContext::getContext() ثابت { OCL_NOT_AVAILABLE(); }
Device& OpenCLExecutionContext::getDevice() ثابت { OCL_NOT_AVAILABLE(); }
Queue& OpenCLExecutionContext::getQueue() ثابت { OCL_NOT_AVAILABLE(); }

منطقي OpenCLExecutionContext::useOpenCL() ثابت { أرجع خطأ; }
فراغ OpenCLExecutionContext::setUseOpenCL(منطقي flag) { }

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

فراغ OpenCLExecutionContext::bind() ثابت { OCL_NOT_AVAILABLE(); }

OpenCLExecutionContext OpenCLExecutionContext::cloneWithNewQueue(ثابت ocl::Queue& q) ثابت { OCL_NOT_AVAILABLE(); }
OpenCLExecutionContext OpenCLExecutionContext::cloneWithNewQueue() ثابت { OCL_NOT_AVAILABLE(); }

/* static */ OpenCLExecutionContext OpenCLExecutionContext::create(ثابت std::string& platformName, فراغ* platformID, فراغ* context, فراغ* deviceID) { OCL_NOT_AVAILABLE(); }
/* static */ OpenCLExecutionContext OpenCLExecutionContext::create(ثابت Context& context, ثابت Device& device, ثابت ocl::Queue& queue) { OCL_NOT_AVAILABLE(); }
/* static */ OpenCLExecutionContext OpenCLExecutionContext::create(ثابت Context& context, ثابت Device& device) { OCL_NOT_AVAILABLE(); }

فراغ OpenCLExecutionContext::release() { }

}}

#إذا defined(_MSC_VER)
    #pragma warning(pop)
#وإلاإذا defined(__clang__)
    #pragma clang diagnostic pop
#وإلاإذا defined(__GNUC__)
    #pragma GCC diagnostic pop
#endif
