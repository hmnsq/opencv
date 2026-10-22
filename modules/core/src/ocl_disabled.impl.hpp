// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#અન્તર્ભાવ "opencv2/core/ocl_genbase.hpp"

#જો defined(_MSC_VER)
    #pragma warning(push)
    #pragma warning(disable : 4100)
    #pragma warning(disable : 4702)
#અથવાજો defined(__clang__)
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wunused-parameter"
#અથવાજો defined(__GNUC__)
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

નામાકાશ cv { નામાકાશ ocl {

સ્થિર
CV_NORETURN શૂન્ય throw_no_ocl()
{
    CV_Error(Error::OpenCLApiCallError, "OpenCV build without OpenCL support");
}
#define OCL_NOT_AVAILABLE() throw_no_ocl();

CV_EXPORTS_W તાર્કિક haveOpenCL() { પાછા ખોટું; }
CV_EXPORTS_W તાર્કિક useOpenCL() { પાછા ખોટું; }
CV_EXPORTS_W તાર્કિક haveAmdBlas() { પાછા ખોટું; }
CV_EXPORTS_W તાર્કિક haveAmdFft() { પાછા ખોટું; }
CV_EXPORTS_W શૂન્ય setUseOpenCL(તાર્કિક flag) { /* nothing */ }
CV_EXPORTS_W શૂન્ય finish() { /* nothing */ }

CV_EXPORTS તાર્કિક haveSVM() { પાછા ખોટું; }

Device::Device() CV_NOEXCEPT : p(ખાલી) { }
Device::Device(શૂન્ય* d) : p(ખાલી) { OCL_NOT_AVAILABLE(); }
Device::Device(અચલ Device& d) : p(ખાલી) { }
Device& Device::operator=(અચલ Device& d) { પાછા *આ; }
Device::Device(Device&&) CV_NOEXCEPT : p(ખાલી) { }
Device& Device::operator=(Device&&) CV_NOEXCEPT { પાછા *આ; }
Device::~Device() { }

શૂન્ય Device::set(શૂન્ય* d) { OCL_NOT_AVAILABLE(); }

String Device::name() અચલ { OCL_NOT_AVAILABLE(); }
String Device::extensions() અચલ { OCL_NOT_AVAILABLE(); }
તાર્કિક Device::isExtensionSupported(અચલ String& extensionName) અચલ { OCL_NOT_AVAILABLE(); }
String Device::version() અચલ { OCL_NOT_AVAILABLE(); }
String Device::vendorName() અચલ { OCL_NOT_AVAILABLE(); }
String Device::OpenCL_C_Version() અચલ { OCL_NOT_AVAILABLE(); }
String Device::OpenCLVersion() અચલ { OCL_NOT_AVAILABLE(); }
પૂર્ણાંક Device::deviceVersionMajor() અચલ { OCL_NOT_AVAILABLE(); }
પૂર્ણાંક Device::deviceVersionMinor() અચલ { OCL_NOT_AVAILABLE(); }
String Device::driverVersion() અચલ { OCL_NOT_AVAILABLE(); }
શૂન્ય* Device::ptr() અચલ { /*OCL_NOT_AVAILABLE();*/ પાછા ખાલી; }

પૂર્ણાંક Device::type() અચલ { OCL_NOT_AVAILABLE(); }

પૂર્ણાંક Device::addressBits() અચલ { OCL_NOT_AVAILABLE(); }
તાર્કિક Device::available() અચલ { OCL_NOT_AVAILABLE(); }
તાર્કિક Device::compilerAvailable() અચલ { OCL_NOT_AVAILABLE(); }
તાર્કિક Device::linkerAvailable() અચલ { OCL_NOT_AVAILABLE(); }

પૂર્ણાંક Device::doubleFPConfig() અચલ { OCL_NOT_AVAILABLE(); }
પૂર્ણાંક Device::singleFPConfig() અચલ { OCL_NOT_AVAILABLE(); }
પૂર્ણાંક Device::halfFPConfig() અચલ { OCL_NOT_AVAILABLE(); }

તાર્કિક Device::hasFP64() અચલ { OCL_NOT_AVAILABLE(); }
તાર્કિક Device::hasFP16() અચલ { OCL_NOT_AVAILABLE(); }

તાર્કિક Device::endianLittle() અચલ { OCL_NOT_AVAILABLE(); }
તાર્કિક Device::errorCorrectionSupport() અચલ { OCL_NOT_AVAILABLE(); }

પૂર્ણાંક Device::executionCapabilities() અચલ { OCL_NOT_AVAILABLE(); }

size_t Device::globalMemCacheSize() અચલ { OCL_NOT_AVAILABLE(); }

પૂર્ણાંક Device::globalMemCacheType() અચલ { OCL_NOT_AVAILABLE(); }
પૂર્ણાંક Device::global_Mem_Cache_લીટી_Size() અચલ { OCL_NOT_AVAILABLE(); }
size_t Device::globalMemSize() અચલ { OCL_NOT_AVAILABLE(); }

size_t Device::localMemSize() અચલ { OCL_NOT_AVAILABLE(); }
પૂર્ણાંક Device::localMemType() અચલ { પાછા NO_LOCAL_MEM; }
તાર્કિક Device::hostUnifiedMemory() અચલ { OCL_NOT_AVAILABLE(); }

તાર્કિક Device::imageSupport() અચલ { OCL_NOT_AVAILABLE(); }

તાર્કિક Device::imageFromBufferSupport() અચલ { OCL_NOT_AVAILABLE(); }
uint Device::imagePitchAlignment() અચલ { OCL_NOT_AVAILABLE(); }
uint Device::imageBaseAddressAlignment() અચલ { OCL_NOT_AVAILABLE(); }

તાર્કિક Device::intelSubgroupsSupport() અચલ { OCL_NOT_AVAILABLE(); }

size_t Device::image2DMaxWidth() અચલ { OCL_NOT_AVAILABLE(); }
size_t Device::image2DMaxHeight() અચલ { OCL_NOT_AVAILABLE(); }

size_t Device::image3DMaxWidth() અચલ { OCL_NOT_AVAILABLE(); }
size_t Device::image3DMaxHeight() અચલ { OCL_NOT_AVAILABLE(); }
size_t Device::image3DMaxDepth() અચલ { OCL_NOT_AVAILABLE(); }

size_t Device::imageMaxBufferSize() અચલ { OCL_NOT_AVAILABLE(); }
size_t Device::imageMaxArraySize() અચલ { OCL_NOT_AVAILABLE(); }

પૂર્ણાંક Device::vendorID() અચલ { OCL_NOT_AVAILABLE(); }

પૂર્ણાંક Device::maxClockFrequency() અચલ { OCL_NOT_AVAILABLE(); }
પૂર્ણાંક Device::maxComputeUnits() અચલ { OCL_NOT_AVAILABLE(); }
પૂર્ણાંક Device::maxConstantArgs() અચલ { OCL_NOT_AVAILABLE(); }
size_t Device::maxConstantBufferSize() અચલ { OCL_NOT_AVAILABLE(); }

size_t Device::maxMemAllocSize() અચલ { OCL_NOT_AVAILABLE(); }
size_t Device::maxParameterSize() અચલ { OCL_NOT_AVAILABLE(); }

પૂર્ણાંક Device::maxReadImageArgs() અચલ { OCL_NOT_AVAILABLE(); }
પૂર્ણાંક Device::maxWriteImageArgs() અચલ { OCL_NOT_AVAILABLE(); }
પૂર્ણાંક Device::maxSamplers() અચલ { OCL_NOT_AVAILABLE(); }

size_t Device::maxWorkGroupSize() અચલ { OCL_NOT_AVAILABLE(); }
પૂર્ણાંક Device::maxWorkItemDims() અચલ { OCL_NOT_AVAILABLE(); }
શૂન્ય Device::maxWorkItemSizes(size_t*) અચલ { OCL_NOT_AVAILABLE(); }

પૂર્ણાંક Device::memBaseAddrAlign() અચલ { OCL_NOT_AVAILABLE(); }

પૂર્ણાંક Device::nativeVectorWidthChar() અચલ { OCL_NOT_AVAILABLE(); }
પૂર્ણાંક Device::nativeVectorWidthShort() અચલ { OCL_NOT_AVAILABLE(); }
પૂર્ણાંક Device::nativeVectorWidthInt() અચલ { OCL_NOT_AVAILABLE(); }
પૂર્ણાંક Device::nativeVectorWidthLong() અચલ { OCL_NOT_AVAILABLE(); }
પૂર્ણાંક Device::nativeVectorWidthFloat() અચલ { OCL_NOT_AVAILABLE(); }
પૂર્ણાંક Device::nativeVectorWidthDouble() અચલ { OCL_NOT_AVAILABLE(); }
પૂર્ણાંક Device::nativeVectorWidthHalf() અચલ { OCL_NOT_AVAILABLE(); }

પૂર્ણાંક Device::preferredVectorWidthChar() અચલ { OCL_NOT_AVAILABLE(); }
પૂર્ણાંક Device::preferredVectorWidthShort() અચલ { OCL_NOT_AVAILABLE(); }
પૂર્ણાંક Device::preferredVectorWidthInt() અચલ { OCL_NOT_AVAILABLE(); }
પૂર્ણાંક Device::preferredVectorWidthLong() અચલ { OCL_NOT_AVAILABLE(); }
પૂર્ણાંક Device::preferredVectorWidthFloat() અચલ { OCL_NOT_AVAILABLE(); }
પૂર્ણાંક Device::preferredVectorWidthDouble() અચલ { OCL_NOT_AVAILABLE(); }
પૂર્ણાંક Device::preferredVectorWidthHalf() અચલ { OCL_NOT_AVAILABLE(); }

size_t Device::printfBufferSize() અચલ { OCL_NOT_AVAILABLE(); }
size_t Device::profilingTimerResolution() અચલ { OCL_NOT_AVAILABLE(); }

/* static */
અચલ Device& Device::getDefault()
{
    સ્થિર Device dummy;
    પાછા dummy;
}

/* static */ Device Device::fromHandle(શૂન્ય* d) { OCL_NOT_AVAILABLE(); }


Context::Context() CV_NOEXCEPT : p(ખાલી) { }
Context::Context(પૂર્ણાંક dtype) : p(ખાલી) { }
Context::~Context() { }
Context::Context(અચલ Context& c) : p(ખાલી) { }
Context& Context::operator=(અચલ Context& c) { પાછા *આ; }
Context::Context(Context&&) CV_NOEXCEPT : p(ખાલી) { }
Context& Context::operator=(Context&&) CV_NOEXCEPT { પાછા *આ; }

તાર્કિક Context::create() { પાછા ખોટું; }
તાર્કિક Context::create(પૂર્ણાંક dtype) { પાછા ખોટું; }
size_t Context::ndevices() અચલ { પાછા 0; }
Device& Context::device(size_t idx) અચલ { OCL_NOT_AVAILABLE(); }
Program Context::getProg(અચલ ProgramSource& prog, અચલ String& buildopt, String& errmsg) { OCL_NOT_AVAILABLE(); }
શૂન્ય Context::unloadProg(Program& prog) { }

/* static */
Context& Context::getDefault()
{
    સ્થિર Context dummy;
    પાછા dummy;
}
શૂન્ય* Context::ptr() અચલ { પાછા ખાલી; }

શૂન્ય* Context::getOpenCLContextProperty(પૂર્ણાંક /*propertyId*/) અચલ { OCL_NOT_AVAILABLE(); }

તાર્કિક Context::useSVM() અચલ { પાછા ખોટું; }
શૂન્ય Context::setUseSVM(તાર્કિક enabled) { }

Context::UserContext::~UserContext() { }

શૂન્ય Context::setUserContext(std::type_index /*typeId*/, અચલ std::shared_ptr<Context::UserContext>& /*userContext*/) { OCL_NOT_AVAILABLE(); }
std::shared_ptr<Context::UserContext> Context::getUserContext(std::type_index /*typeId*/) { OCL_NOT_AVAILABLE(); }

/* static */ Context Context::fromHandle(શૂન્ય* context) { OCL_NOT_AVAILABLE(); }
/* static */ Context Context::fromDevice(અચલ ocl::Device& device) { OCL_NOT_AVAILABLE(); }
/* static */ Context Context::create(અચલ std::string& configuration) { OCL_NOT_AVAILABLE(); }

શૂન્ય Context::release() { }


Platform::Platform() CV_NOEXCEPT : p(ખાલી) { }
Platform::~Platform() { }
Platform::Platform(અચલ Platform&) : p(ખાલી) { }
Platform& Platform::operator=(અચલ Platform&) { પાછા *આ; }
Platform::Platform(Platform&&) CV_NOEXCEPT : p(ખાલી) { }
Platform& Platform::operator=(Platform&&) CV_NOEXCEPT { પાછા *આ; }

શૂન્ય* Platform::ptr() અચલ { પાછા ખાલી; }

/* static */
Platform& Platform::getDefault()
{
    સ્થિર Platform dummy;
    પાછા dummy;
}

શૂન્ય attachContext(અચલ String& platformName, શૂન્ય* platformID, શૂન્ય* context, શૂન્ય* deviceID) { OCL_NOT_AVAILABLE(); }
શૂન્ય convertFromBuffer(શૂન્ય* cl_mem_buffer, size_t step, પૂર્ણાંક rows, પૂર્ણાંક cols, પૂર્ણાંક type, UMat& dst) { OCL_NOT_AVAILABLE(); }
શૂન્ય convertFromImage(શૂન્ય* cl_mem_image, UMat& dst) { OCL_NOT_AVAILABLE(); }

શૂન્ય initializeContextFromHandle(Context& ctx, શૂન્ય* platform, શૂન્ય* context, શૂન્ય* device) { OCL_NOT_AVAILABLE(); }

Queue::Queue() CV_NOEXCEPT : p(ખાલી) { }
Queue::Queue(અચલ Context& c, અચલ Device& d) : p(ખાલી) { OCL_NOT_AVAILABLE(); }
Queue::~Queue() { }
Queue::Queue(અચલ Queue& q) {}
Queue& Queue::operator=(અચલ Queue& q) { પાછા *આ; }
Queue::Queue(Queue&&) CV_NOEXCEPT : p(ખાલી) { }
Queue& Queue::operator=(Queue&&) CV_NOEXCEPT { પાછા *આ; }

તાર્કિક Queue::create(અચલ Context& c, અચલ Device& d) { OCL_NOT_AVAILABLE(); }
શૂન્ય Queue::finish() {}
શૂન્ય* Queue::ptr() અચલ { પાછા ખાલી; }
/* static */
Queue& Queue::getDefault()
{
    સ્થિર Queue dummy;
    પાછા dummy;
}

/// @brief Returns OpenCL command queue with enable profiling mode support
અચલ Queue& Queue::getProfilingQueue() અચલ { OCL_NOT_AVAILABLE(); }


KernelArg::KernelArg() CV_NOEXCEPT
    : flags(0), m(0), obj(0), sz(0), wscale(1), iwscale(1)
{
}

KernelArg::KernelArg(પૂર્ણાંક _flags, UMat* _m, પૂર્ણાંક _wscale, પૂર્ણાંક _iwscale, અચલ શૂન્ય* _obj, size_t _sz)
    : flags(_flags), m(_m), obj(_obj), sz(_sz), wscale(_wscale), iwscale(_iwscale)
{
    OCL_NOT_AVAILABLE();
}

KernelArg KernelArg::Constant(અચલ Mat& m)
{
    OCL_NOT_AVAILABLE();
}


Kernel::Kernel() CV_NOEXCEPT : p(ખાલી) { }
Kernel::Kernel(અચલ અક્ષર* kname, અચલ Program& prog) : p(ખાલી) { OCL_NOT_AVAILABLE(); }
Kernel::Kernel(અચલ અક્ષર* kname, અચલ ProgramSource& prog, અચલ String& buildopts, String* errmsg) : p(ખાલી) { OCL_NOT_AVAILABLE(); }
Kernel::~Kernel() { }
Kernel::Kernel(અચલ Kernel& k) : p(ખાલી) { }
Kernel& Kernel::operator=(અચલ Kernel& k) { પાછા *આ; }
Kernel::Kernel(Kernel&&) CV_NOEXCEPT : p(ખાલી) { }
Kernel& Kernel::operator=(Kernel&&) CV_NOEXCEPT { પાછા *આ; }

તાર્કિક Kernel::empty() અચલ { પાછા સાચું; }
તાર્કિક Kernel::create(અચલ અક્ષર* kname, અચલ Program& prog) { OCL_NOT_AVAILABLE(); }
તાર્કિક Kernel::create(અચલ અક્ષર* kname, અચલ ProgramSource& prog, અચલ String& buildopts, String* errmsg) { OCL_NOT_AVAILABLE(); }

પૂર્ણાંક Kernel::set(પૂર્ણાંક i, અચલ શૂન્ય* value, size_t sz) { OCL_NOT_AVAILABLE(); }
પૂર્ણાંક Kernel::set(પૂર્ણાંક i, અચલ Image2D& image2D) { OCL_NOT_AVAILABLE(); }
પૂર્ણાંક Kernel::set(પૂર્ણાંક i, અચલ UMat& m) { OCL_NOT_AVAILABLE(); }
પૂર્ણાંક Kernel::set(પૂર્ણાંક i, અચલ KernelArg& arg) { OCL_NOT_AVAILABLE(); }

તાર્કિક Kernel::run(પૂર્ણાંક dims, size_t globalsize[], size_t localsize[], તાર્કિક સમકાલિક, અચલ Queue& q) { OCL_NOT_AVAILABLE(); }
તાર્કિક Kernel::runTask(તાર્કિક સમકાલિક, અચલ Queue& q) { OCL_NOT_AVAILABLE(); }

int64 Kernel::runProfiling(પૂર્ણાંક dims, size_t globalsize[], size_t localsize[], અચલ Queue& q) { OCL_NOT_AVAILABLE(); }

size_t Kernel::workGroupSize() અચલ { OCL_NOT_AVAILABLE(); }
size_t Kernel::preferedWorkGroupSizeMultiple() અચલ { OCL_NOT_AVAILABLE(); }
તાર્કિક Kernel::compileWorkGroupSize(size_t wsz[]) અચલ { OCL_NOT_AVAILABLE(); }
size_t Kernel::localMemSize() અચલ { OCL_NOT_AVAILABLE(); }

શૂન્ય* Kernel::ptr() અચલ { પાછા ખાલી; }


Program::Program() CV_NOEXCEPT : p(ખાલી) { }
Program::Program(અચલ ProgramSource& src, અચલ String& buildflags, String& errmsg) : p(ખાલી) { OCL_NOT_AVAILABLE(); }
Program::Program(અચલ Program& prog) : p(ખાલી) { }
Program& Program::operator=(અચલ Program& prog) { પાછા *આ; }
Program::Program(Program&&) CV_NOEXCEPT : p(ખાલી) { }
Program& Program::operator=(Program&&) CV_NOEXCEPT { પાછા *આ; }
Program::~Program() { }

તાર્કિક Program::create(અચલ ProgramSource& src, અચલ String& buildflags, String& errmsg) { OCL_NOT_AVAILABLE(); }

શૂન્ય* Program::ptr() અચલ { પાછા ખાલી; }

શૂન્ય Program::get_દ્વિઅંકી(std::vector<અક્ષર>& binary) અચલ { OCL_NOT_AVAILABLE(); }

તાર્કિક Program::પઠ(અચલ String& buf, અચલ String& buildflags) { OCL_NOT_AVAILABLE(); }
તાર્કિક Program::લિખ(String& buf) અચલ { OCL_NOT_AVAILABLE(); }
અચલ ProgramSource& Program::source() અચલ { OCL_NOT_AVAILABLE(); }
String Program::getPrefix() અચલ { OCL_NOT_AVAILABLE(); }
/* static */ String Program::getPrefix(અચલ String& buildflags) { OCL_NOT_AVAILABLE(); }


ProgramSource::ProgramSource() CV_NOEXCEPT : p(ખાલી) { }
ProgramSource::ProgramSource(અચલ String& module, અચલ String& name, અચલ String& codeStr, અચલ String& codeHash) : p(ખાલી) { }
ProgramSource::ProgramSource(અચલ String& prog) : p(ખાલી) { }
ProgramSource::ProgramSource(અચલ અક્ષર* prog) : p(ખાલી) { }
ProgramSource::~ProgramSource() { }
ProgramSource::ProgramSource(અચલ ProgramSource& prog) : p(ખાલી) { }
ProgramSource& ProgramSource::operator=(અચલ ProgramSource& prog) { પાછા *આ; }
ProgramSource::ProgramSource(ProgramSource&&) CV_NOEXCEPT : p(ખાલી) { }
ProgramSource& ProgramSource::operator=(ProgramSource&&) CV_NOEXCEPT { પાછા *આ; }

અચલ String& ProgramSource::source() અચલ { OCL_NOT_AVAILABLE(); }
ProgramSource::hash_t ProgramSource::hash() અચલ { OCL_NOT_AVAILABLE(); }

/* static */ ProgramSource ProgramSource::from_દ્વિઅંકી(અચલ String& module, અચલ String& name, અચલ unsigned અક્ષર* binary, અચલ size_t size, અચલ cv::String& buildOptions) { OCL_NOT_AVAILABLE(); }
/* static */ ProgramSource ProgramSource::fromSPIR(અચલ String& module, અચલ String& name, અચલ unsigned અક્ષર* binary, અચલ size_t size, અચલ cv::String& buildOptions) { OCL_NOT_AVAILABLE(); }


PlatformInfo::PlatformInfo() CV_NOEXCEPT : p(ખાલી) { }
PlatformInfo::PlatformInfo(શૂન્ય* id) : p(ખાલી) { OCL_NOT_AVAILABLE(); }
PlatformInfo::~PlatformInfo() { }

PlatformInfo::PlatformInfo(અચલ PlatformInfo& i) : p(ખાલી) { }
PlatformInfo& PlatformInfo::operator=(અચલ PlatformInfo& i) { પાછા *આ; }
PlatformInfo::PlatformInfo(PlatformInfo&&) CV_NOEXCEPT : p(ખાલી) { }
PlatformInfo& PlatformInfo::operator=(PlatformInfo&&) CV_NOEXCEPT { પાછા *આ; }

String PlatformInfo::name() અચલ { OCL_NOT_AVAILABLE(); }
String PlatformInfo::vendor() અચલ { OCL_NOT_AVAILABLE(); }
String PlatformInfo::version() અચલ { OCL_NOT_AVAILABLE(); }
પૂર્ણાંક PlatformInfo::device_સંખ્યા() અચલ { OCL_NOT_AVAILABLE(); }
શૂન્ય PlatformInfo::getDevice(Device& device, પૂર્ણાંક d) અચલ { OCL_NOT_AVAILABLE(); }

અચલ અક્ષર* convertTypeStr(પૂર્ણાંક sdepth, પૂર્ણાંક ddepth, પૂર્ણાંક cn, અક્ષર* buf) { OCL_NOT_AVAILABLE(); }
અચલ અક્ષર* typeToStr(પૂર્ણાંક t) { OCL_NOT_AVAILABLE(); }
અચલ અક્ષર* memopTypeToStr(પૂર્ણાંક t) { OCL_NOT_AVAILABLE(); }
અચલ અક્ષર* vecopTypeToStr(પૂર્ણાંક t) { OCL_NOT_AVAILABLE(); }
અચલ અક્ષર* getOpenCLErrorString(પૂર્ણાંક errorCode) { OCL_NOT_AVAILABLE(); }
String kernelToStr(આગત_Array _kernel, પૂર્ણાંક ddepth, અચલ અક્ષર* name) { OCL_NOT_AVAILABLE(); }
શૂન્ય getPlatfomsInfo(std::vector<PlatformInfo>& platform_info) { OCL_NOT_AVAILABLE(); }


પૂર્ણાંક predictOptimalVectorWidth(આગત_Array src1, આગત_Array src2, આગત_Array src3,
        આગત_Array src4, આગત_Array src5, આગત_Array src6,
        આગત_Array src7, આગત_Array src8, આગત_Array src9,
        OclVectorStrategy strat)
{ OCL_NOT_AVAILABLE(); }

પૂર્ણાંક checkOptimalVectorWidth(અચલ પૂર્ણાંક *vectorWidths,
        આગત_Array src1, આગત_Array src2, આગત_Array src3,
        આગત_Array src4, આગત_Array src5, આગત_Array src6,
        આગત_Array src7, આગત_Array src8, આગત_Array src9,
        OclVectorStrategy strat)
{ OCL_NOT_AVAILABLE(); }

પૂર્ણાંક predictOptimalVectorWidthMax(આગત_Array src1, આગત_Array src2, આગત_Array src3,
        આગત_Array src4, આગત_Array src5, આગત_Array src6,
        આગત_Array src7, આગત_Array src8, આગત_Array src9)
{ OCL_NOT_AVAILABLE(); }

શૂન્ય buildOptionsAddMatrixDescription(String& buildOptions, અચલ String& name, આગત_Array _m) { OCL_NOT_AVAILABLE(); }


Image2D::Image2D() CV_NOEXCEPT : p(ખાલી) { }
Image2D::Image2D(અચલ UMat &src, તાર્કિક norm, તાર્કિક alias) { OCL_NOT_AVAILABLE(); }
Image2D::Image2D(અચલ Image2D & i) : p(ખાલી) { OCL_NOT_AVAILABLE(); }
Image2D::~Image2D() { }
Image2D& Image2D::operator=(અચલ Image2D & i) { પાછા *આ; }
Image2D::Image2D(Image2D&&) CV_NOEXCEPT : p(ખાલી) { }
Image2D& Image2D::operator=(Image2D&&) CV_NOEXCEPT { પાછા *આ; }

/* static */ તાર્કિક Image2D::canCreateAlias(અચલ UMat &u) { OCL_NOT_AVAILABLE(); }
/* static */ તાર્કિક Image2D::isFormatSupported(પૂર્ણાંક depth, પૂર્ણાંક cn, તાર્કિક norm) { OCL_NOT_AVAILABLE(); }

શૂન્ય* Image2D::ptr() અચલ { પાછા ખાલી; }


Timer::Timer(અચલ Queue& q) : p(ખાલી) {}
Timer::~Timer() {}
શૂન્ય Timer::start() { OCL_NOT_AVAILABLE(); }
શૂન્ય Timer::stop() { OCL_NOT_AVAILABLE();}

uint64 Timer::durationNS() અચલ { OCL_NOT_AVAILABLE(); }

MatAllocator* getOpenCLAllocator() { પાછા ખાલી; }

internal::ProgramEntry::operator ProgramSource&() અચલ { OCL_NOT_AVAILABLE(); }


સંરચના OpenCLExecutionContext::Impl
{
    Impl() = મૂળભૂત;
};

Context& OpenCLExecutionContext::getContext() અચલ { OCL_NOT_AVAILABLE(); }
Device& OpenCLExecutionContext::getDevice() અચલ { OCL_NOT_AVAILABLE(); }
Queue& OpenCLExecutionContext::getQueue() અચલ { OCL_NOT_AVAILABLE(); }

તાર્કિક OpenCLExecutionContext::useOpenCL() અચલ { પાછા ખોટું; }
શૂન્ય OpenCLExecutionContext::setUseOpenCL(તાર્કિક flag) { }

સ્થિર
OpenCLExecutionContext& getDummyOpenCLExecutionContext()
{
    સ્થિર OpenCLExecutionContext dummy;
    પાછા dummy;
}

/* static */
OpenCLExecutionContext& OpenCLExecutionContext::getCurrent() { પાછા getDummyOpenCLExecutionContext(); }

/* static */
OpenCLExecutionContext& OpenCLExecutionContext::getCurrentRef() { પાછા getDummyOpenCLExecutionContext(); }

શૂન્ય OpenCLExecutionContext::bind() અચલ { OCL_NOT_AVAILABLE(); }

OpenCLExecutionContext OpenCLExecutionContext::cloneWithNewQueue(અચલ ocl::Queue& q) અચલ { OCL_NOT_AVAILABLE(); }
OpenCLExecutionContext OpenCLExecutionContext::cloneWithNewQueue() અચલ { OCL_NOT_AVAILABLE(); }

/* static */ OpenCLExecutionContext OpenCLExecutionContext::create(અચલ std::string& platformName, શૂન્ય* platformID, શૂન્ય* context, શૂન્ય* deviceID) { OCL_NOT_AVAILABLE(); }
/* static */ OpenCLExecutionContext OpenCLExecutionContext::create(અચલ Context& context, અચલ Device& device, અચલ ocl::Queue& queue) { OCL_NOT_AVAILABLE(); }
/* static */ OpenCLExecutionContext OpenCLExecutionContext::create(અચલ Context& context, અચલ Device& device) { OCL_NOT_AVAILABLE(); }

શૂન્ય OpenCLExecutionContext::release() { }

}}

#જો defined(_MSC_VER)
    #pragma warning(pop)
#અથવાજો defined(__clang__)
    #pragma clang diagnostic pop
#અથવાજો defined(__GNUC__)
    #pragma GCC diagnostic pop
#endif
