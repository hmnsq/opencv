// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#அந்தர்பாவ "opencv2/core/ocl_genbase.hpp"

#எனில் defined(_MSC_VER)
    #pragma warning(push)
    #pragma warning(disable : 4100)
    #pragma warning(disable : 4702)
#அல்லதெனில் defined(__clang__)
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wunused-parameter"
#அல்லதெனில் defined(__GNUC__)
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

நாமாகாஶ cv { நாமாகாஶ ocl {

நிலையான
CV_NORETURN வெறுமை throw_no_ocl()
{
    CV_Error(Error::OpenCLApiCallError, "OpenCV build without OpenCL support");
}
#define OCL_NOT_AVAILABLE() throw_no_ocl();

CV_EXPORTS_W தருக்க haveOpenCL() { திரும்பு பொய்; }
CV_EXPORTS_W தருக்க useOpenCL() { திரும்பு பொய்; }
CV_EXPORTS_W தருக்க haveAmdBlas() { திரும்பு பொய்; }
CV_EXPORTS_W தருக்க haveAmdFft() { திரும்பு பொய்; }
CV_EXPORTS_W வெறுமை setUseOpenCL(தருக்க flag) { /* nothing */ }
CV_EXPORTS_W வெறுமை finish() { /* nothing */ }

CV_EXPORTS தருக்க haveSVM() { திரும்பு பொய்; }

Device::Device() CV_NOEXCEPT : p(வெற்று) { }
Device::Device(வெறுமை* d) : p(வெற்று) { OCL_NOT_AVAILABLE(); }
Device::Device(மாறிலி Device& d) : p(வெற்று) { }
Device& Device::operator=(மாறிலி Device& d) { திரும்பு *இது; }
Device::Device(Device&&) CV_NOEXCEPT : p(வெற்று) { }
Device& Device::operator=(Device&&) CV_NOEXCEPT { திரும்பு *இது; }
Device::~Device() { }

வெறுமை Device::set(வெறுமை* d) { OCL_NOT_AVAILABLE(); }

String Device::name() மாறிலி { OCL_NOT_AVAILABLE(); }
String Device::extensions() மாறிலி { OCL_NOT_AVAILABLE(); }
தருக்க Device::isExtensionSupported(மாறிலி String& extensionName) மாறிலி { OCL_NOT_AVAILABLE(); }
String Device::version() மாறிலி { OCL_NOT_AVAILABLE(); }
String Device::vendorName() மாறிலி { OCL_NOT_AVAILABLE(); }
String Device::OpenCL_C_Version() மாறிலி { OCL_NOT_AVAILABLE(); }
String Device::OpenCLVersion() மாறிலி { OCL_NOT_AVAILABLE(); }
முழு Device::deviceVersionMajor() மாறிலி { OCL_NOT_AVAILABLE(); }
முழு Device::deviceVersionMinor() மாறிலி { OCL_NOT_AVAILABLE(); }
String Device::driverVersion() மாறிலி { OCL_NOT_AVAILABLE(); }
வெறுமை* Device::ptr() மாறிலி { /*OCL_NOT_AVAILABLE();*/ திரும்பு வெற்று; }

முழு Device::type() மாறிலி { OCL_NOT_AVAILABLE(); }

முழு Device::addressBits() மாறிலி { OCL_NOT_AVAILABLE(); }
தருக்க Device::available() மாறிலி { OCL_NOT_AVAILABLE(); }
தருக்க Device::compilerAvailable() மாறிலி { OCL_NOT_AVAILABLE(); }
தருக்க Device::linkerAvailable() மாறிலி { OCL_NOT_AVAILABLE(); }

முழு Device::doubleFPConfig() மாறிலி { OCL_NOT_AVAILABLE(); }
முழு Device::singleFPConfig() மாறிலி { OCL_NOT_AVAILABLE(); }
முழு Device::halfFPConfig() மாறிலி { OCL_NOT_AVAILABLE(); }

தருக்க Device::hasFP64() மாறிலி { OCL_NOT_AVAILABLE(); }
தருக்க Device::hasFP16() மாறிலி { OCL_NOT_AVAILABLE(); }

தருக்க Device::endianLittle() மாறிலி { OCL_NOT_AVAILABLE(); }
தருக்க Device::errorCorrectionSupport() மாறிலி { OCL_NOT_AVAILABLE(); }

முழு Device::executionCapabilities() மாறிலி { OCL_NOT_AVAILABLE(); }

size_t Device::globalMemCacheSize() மாறிலி { OCL_NOT_AVAILABLE(); }

முழு Device::globalMemCacheType() மாறிலி { OCL_NOT_AVAILABLE(); }
முழு Device::global_Mem_Cache_வரி_Size() மாறிலி { OCL_NOT_AVAILABLE(); }
size_t Device::globalMemSize() மாறிலி { OCL_NOT_AVAILABLE(); }

size_t Device::localMemSize() மாறிலி { OCL_NOT_AVAILABLE(); }
முழு Device::localMemType() மாறிலி { திரும்பு NO_LOCAL_MEM; }
தருக்க Device::hostUnifiedMemory() மாறிலி { OCL_NOT_AVAILABLE(); }

தருக்க Device::imageSupport() மாறிலி { OCL_NOT_AVAILABLE(); }

தருக்க Device::imageFromBufferSupport() மாறிலி { OCL_NOT_AVAILABLE(); }
uint Device::imagePitchAlignment() மாறிலி { OCL_NOT_AVAILABLE(); }
uint Device::imageBaseAddressAlignment() மாறிலி { OCL_NOT_AVAILABLE(); }

தருக்க Device::intelSubgroupsSupport() மாறிலி { OCL_NOT_AVAILABLE(); }

size_t Device::image2DMaxWidth() மாறிலி { OCL_NOT_AVAILABLE(); }
size_t Device::image2DMaxHeight() மாறிலி { OCL_NOT_AVAILABLE(); }

size_t Device::image3DMaxWidth() மாறிலி { OCL_NOT_AVAILABLE(); }
size_t Device::image3DMaxHeight() மாறிலி { OCL_NOT_AVAILABLE(); }
size_t Device::image3DMaxDepth() மாறிலி { OCL_NOT_AVAILABLE(); }

size_t Device::imageMaxBufferSize() மாறிலி { OCL_NOT_AVAILABLE(); }
size_t Device::imageMaxArraySize() மாறிலி { OCL_NOT_AVAILABLE(); }

முழு Device::vendorID() மாறிலி { OCL_NOT_AVAILABLE(); }

முழு Device::maxClockFrequency() மாறிலி { OCL_NOT_AVAILABLE(); }
முழு Device::maxComputeUnits() மாறிலி { OCL_NOT_AVAILABLE(); }
முழு Device::maxConstantArgs() மாறிலி { OCL_NOT_AVAILABLE(); }
size_t Device::maxConstantBufferSize() மாறிலி { OCL_NOT_AVAILABLE(); }

size_t Device::maxMemAllocSize() மாறிலி { OCL_NOT_AVAILABLE(); }
size_t Device::maxParameterSize() மாறிலி { OCL_NOT_AVAILABLE(); }

முழு Device::maxReadImageArgs() மாறிலி { OCL_NOT_AVAILABLE(); }
முழு Device::maxWriteImageArgs() மாறிலி { OCL_NOT_AVAILABLE(); }
முழு Device::maxSamplers() மாறிலி { OCL_NOT_AVAILABLE(); }

size_t Device::maxWorkGroupSize() மாறிலி { OCL_NOT_AVAILABLE(); }
முழு Device::maxWorkItemDims() மாறிலி { OCL_NOT_AVAILABLE(); }
வெறுமை Device::maxWorkItemSizes(size_t*) மாறிலி { OCL_NOT_AVAILABLE(); }

முழு Device::memBaseAddrAlign() மாறிலி { OCL_NOT_AVAILABLE(); }

முழு Device::nativeVectorWidthChar() மாறிலி { OCL_NOT_AVAILABLE(); }
முழு Device::nativeVectorWidthShort() மாறிலி { OCL_NOT_AVAILABLE(); }
முழு Device::nativeVectorWidthInt() மாறிலி { OCL_NOT_AVAILABLE(); }
முழு Device::nativeVectorWidthLong() மாறிலி { OCL_NOT_AVAILABLE(); }
முழு Device::nativeVectorWidthFloat() மாறிலி { OCL_NOT_AVAILABLE(); }
முழு Device::nativeVectorWidthDouble() மாறிலி { OCL_NOT_AVAILABLE(); }
முழு Device::nativeVectorWidthHalf() மாறிலி { OCL_NOT_AVAILABLE(); }

முழு Device::preferredVectorWidthChar() மாறிலி { OCL_NOT_AVAILABLE(); }
முழு Device::preferredVectorWidthShort() மாறிலி { OCL_NOT_AVAILABLE(); }
முழு Device::preferredVectorWidthInt() மாறிலி { OCL_NOT_AVAILABLE(); }
முழு Device::preferredVectorWidthLong() மாறிலி { OCL_NOT_AVAILABLE(); }
முழு Device::preferredVectorWidthFloat() மாறிலி { OCL_NOT_AVAILABLE(); }
முழு Device::preferredVectorWidthDouble() மாறிலி { OCL_NOT_AVAILABLE(); }
முழு Device::preferredVectorWidthHalf() மாறிலி { OCL_NOT_AVAILABLE(); }

size_t Device::printfBufferSize() மாறிலி { OCL_NOT_AVAILABLE(); }
size_t Device::profilingTimerResolution() மாறிலி { OCL_NOT_AVAILABLE(); }

/* static */
மாறிலி Device& Device::getDefault()
{
    நிலையான Device dummy;
    திரும்பு dummy;
}

/* static */ Device Device::fromHandle(வெறுமை* d) { OCL_NOT_AVAILABLE(); }


Context::Context() CV_NOEXCEPT : p(வெற்று) { }
Context::Context(முழு dtype) : p(வெற்று) { }
Context::~Context() { }
Context::Context(மாறிலி Context& c) : p(வெற்று) { }
Context& Context::operator=(மாறிலி Context& c) { திரும்பு *இது; }
Context::Context(Context&&) CV_NOEXCEPT : p(வெற்று) { }
Context& Context::operator=(Context&&) CV_NOEXCEPT { திரும்பு *இது; }

தருக்க Context::create() { திரும்பு பொய்; }
தருக்க Context::create(முழு dtype) { திரும்பு பொய்; }
size_t Context::ndevices() மாறிலி { திரும்பு 0; }
Device& Context::device(size_t idx) மாறிலி { OCL_NOT_AVAILABLE(); }
Program Context::getProg(மாறிலி ProgramSource& prog, மாறிலி String& buildopt, String& errmsg) { OCL_NOT_AVAILABLE(); }
வெறுமை Context::unloadProg(Program& prog) { }

/* static */
Context& Context::getDefault()
{
    நிலையான Context dummy;
    திரும்பு dummy;
}
வெறுமை* Context::ptr() மாறிலி { திரும்பு வெற்று; }

வெறுமை* Context::getOpenCLContextProperty(முழு /*propertyId*/) மாறிலி { OCL_NOT_AVAILABLE(); }

தருக்க Context::useSVM() மாறிலி { திரும்பு பொய்; }
வெறுமை Context::setUseSVM(தருக்க enabled) { }

Context::UserContext::~UserContext() { }

வெறுமை Context::setUserContext(std::type_index /*typeId*/, மாறிலி std::shared_ptr<Context::UserContext>& /*userContext*/) { OCL_NOT_AVAILABLE(); }
std::shared_ptr<Context::UserContext> Context::getUserContext(std::type_index /*typeId*/) { OCL_NOT_AVAILABLE(); }

/* static */ Context Context::fromHandle(வெறுமை* context) { OCL_NOT_AVAILABLE(); }
/* static */ Context Context::fromDevice(மாறிலி ocl::Device& device) { OCL_NOT_AVAILABLE(); }
/* static */ Context Context::create(மாறிலி std::string& configuration) { OCL_NOT_AVAILABLE(); }

வெறுமை Context::release() { }


Platform::Platform() CV_NOEXCEPT : p(வெற்று) { }
Platform::~Platform() { }
Platform::Platform(மாறிலி Platform&) : p(வெற்று) { }
Platform& Platform::operator=(மாறிலி Platform&) { திரும்பு *இது; }
Platform::Platform(Platform&&) CV_NOEXCEPT : p(வெற்று) { }
Platform& Platform::operator=(Platform&&) CV_NOEXCEPT { திரும்பு *இது; }

வெறுமை* Platform::ptr() மாறிலி { திரும்பு வெற்று; }

/* static */
Platform& Platform::getDefault()
{
    நிலையான Platform dummy;
    திரும்பு dummy;
}

வெறுமை attachContext(மாறிலி String& platformName, வெறுமை* platformID, வெறுமை* context, வெறுமை* deviceID) { OCL_NOT_AVAILABLE(); }
வெறுமை convertFromBuffer(வெறுமை* cl_mem_buffer, size_t step, முழு rows, முழு cols, முழு type, UMat& dst) { OCL_NOT_AVAILABLE(); }
வெறுமை convertFromImage(வெறுமை* cl_mem_image, UMat& dst) { OCL_NOT_AVAILABLE(); }

வெறுமை initializeContextFromHandle(Context& ctx, வெறுமை* platform, வெறுமை* context, வெறுமை* device) { OCL_NOT_AVAILABLE(); }

Queue::Queue() CV_NOEXCEPT : p(வெற்று) { }
Queue::Queue(மாறிலி Context& c, மாறிலி Device& d) : p(வெற்று) { OCL_NOT_AVAILABLE(); }
Queue::~Queue() { }
Queue::Queue(மாறிலி Queue& q) {}
Queue& Queue::operator=(மாறிலி Queue& q) { திரும்பு *இது; }
Queue::Queue(Queue&&) CV_NOEXCEPT : p(வெற்று) { }
Queue& Queue::operator=(Queue&&) CV_NOEXCEPT { திரும்பு *இது; }

தருக்க Queue::create(மாறிலி Context& c, மாறிலி Device& d) { OCL_NOT_AVAILABLE(); }
வெறுமை Queue::finish() {}
வெறுமை* Queue::ptr() மாறிலி { திரும்பு வெற்று; }
/* static */
Queue& Queue::getDefault()
{
    நிலையான Queue dummy;
    திரும்பு dummy;
}

/// @brief Returns OpenCL command queue with enable profiling mode support
மாறிலி Queue& Queue::getProfilingQueue() மாறிலி { OCL_NOT_AVAILABLE(); }


KernelArg::KernelArg() CV_NOEXCEPT
    : flags(0), m(0), obj(0), sz(0), wscale(1), iwscale(1)
{
}

KernelArg::KernelArg(முழு _flags, UMat* _m, முழு _wscale, முழு _iwscale, மாறிலி வெறுமை* _obj, size_t _sz)
    : flags(_flags), m(_m), obj(_obj), sz(_sz), wscale(_wscale), iwscale(_iwscale)
{
    OCL_NOT_AVAILABLE();
}

KernelArg KernelArg::Constant(மாறிலி Mat& m)
{
    OCL_NOT_AVAILABLE();
}


Kernel::Kernel() CV_NOEXCEPT : p(வெற்று) { }
Kernel::Kernel(மாறிலி எழுத்து* kname, மாறிலி Program& prog) : p(வெற்று) { OCL_NOT_AVAILABLE(); }
Kernel::Kernel(மாறிலி எழுத்து* kname, மாறிலி ProgramSource& prog, மாறிலி String& buildopts, String* errmsg) : p(வெற்று) { OCL_NOT_AVAILABLE(); }
Kernel::~Kernel() { }
Kernel::Kernel(மாறிலி Kernel& k) : p(வெற்று) { }
Kernel& Kernel::operator=(மாறிலி Kernel& k) { திரும்பு *இது; }
Kernel::Kernel(Kernel&&) CV_NOEXCEPT : p(வெற்று) { }
Kernel& Kernel::operator=(Kernel&&) CV_NOEXCEPT { திரும்பு *இது; }

தருக்க Kernel::empty() மாறிலி { திரும்பு உண்மை; }
தருக்க Kernel::create(மாறிலி எழுத்து* kname, மாறிலி Program& prog) { OCL_NOT_AVAILABLE(); }
தருக்க Kernel::create(மாறிலி எழுத்து* kname, மாறிலி ProgramSource& prog, மாறிலி String& buildopts, String* errmsg) { OCL_NOT_AVAILABLE(); }

முழு Kernel::set(முழு i, மாறிலி வெறுமை* value, size_t sz) { OCL_NOT_AVAILABLE(); }
முழு Kernel::set(முழு i, மாறிலி Image2D& image2D) { OCL_NOT_AVAILABLE(); }
முழு Kernel::set(முழு i, மாறிலி UMat& m) { OCL_NOT_AVAILABLE(); }
முழு Kernel::set(முழு i, மாறிலி KernelArg& arg) { OCL_NOT_AVAILABLE(); }

தருக்க Kernel::run(முழு dims, size_t globalsize[], size_t localsize[], தருக்க ஒத்திசை, மாறிலி Queue& q) { OCL_NOT_AVAILABLE(); }
தருக்க Kernel::runTask(தருக்க ஒத்திசை, மாறிலி Queue& q) { OCL_NOT_AVAILABLE(); }

int64 Kernel::runProfiling(முழு dims, size_t globalsize[], size_t localsize[], மாறிலி Queue& q) { OCL_NOT_AVAILABLE(); }

size_t Kernel::workGroupSize() மாறிலி { OCL_NOT_AVAILABLE(); }
size_t Kernel::preferedWorkGroupSizeMultiple() மாறிலி { OCL_NOT_AVAILABLE(); }
தருக்க Kernel::compileWorkGroupSize(size_t wsz[]) மாறிலி { OCL_NOT_AVAILABLE(); }
size_t Kernel::localMemSize() மாறிலி { OCL_NOT_AVAILABLE(); }

வெறுமை* Kernel::ptr() மாறிலி { திரும்பு வெற்று; }


Program::Program() CV_NOEXCEPT : p(வெற்று) { }
Program::Program(மாறிலி ProgramSource& src, மாறிலி String& buildflags, String& errmsg) : p(வெற்று) { OCL_NOT_AVAILABLE(); }
Program::Program(மாறிலி Program& prog) : p(வெற்று) { }
Program& Program::operator=(மாறிலி Program& prog) { திரும்பு *இது; }
Program::Program(Program&&) CV_NOEXCEPT : p(வெற்று) { }
Program& Program::operator=(Program&&) CV_NOEXCEPT { திரும்பு *இது; }
Program::~Program() { }

தருக்க Program::create(மாறிலி ProgramSource& src, மாறிலி String& buildflags, String& errmsg) { OCL_NOT_AVAILABLE(); }

வெறுமை* Program::ptr() மாறிலி { திரும்பு வெற்று; }

வெறுமை Program::get_இருமம்(std::vector<எழுத்து>& binary) மாறிலி { OCL_NOT_AVAILABLE(); }

தருக்க Program::பட(மாறிலி String& buf, மாறிலி String& buildflags) { OCL_NOT_AVAILABLE(); }
தருக்க Program::லிக(String& buf) மாறிலி { OCL_NOT_AVAILABLE(); }
மாறிலி ProgramSource& Program::source() மாறிலி { OCL_NOT_AVAILABLE(); }
String Program::getPrefix() மாறிலி { OCL_NOT_AVAILABLE(); }
/* static */ String Program::getPrefix(மாறிலி String& buildflags) { OCL_NOT_AVAILABLE(); }


ProgramSource::ProgramSource() CV_NOEXCEPT : p(வெற்று) { }
ProgramSource::ProgramSource(மாறிலி String& module, மாறிலி String& name, மாறிலி String& codeStr, மாறிலி String& codeHash) : p(வெற்று) { }
ProgramSource::ProgramSource(மாறிலி String& prog) : p(வெற்று) { }
ProgramSource::ProgramSource(மாறிலி எழுத்து* prog) : p(வெற்று) { }
ProgramSource::~ProgramSource() { }
ProgramSource::ProgramSource(மாறிலி ProgramSource& prog) : p(வெற்று) { }
ProgramSource& ProgramSource::operator=(மாறிலி ProgramSource& prog) { திரும்பு *இது; }
ProgramSource::ProgramSource(ProgramSource&&) CV_NOEXCEPT : p(வெற்று) { }
ProgramSource& ProgramSource::operator=(ProgramSource&&) CV_NOEXCEPT { திரும்பு *இது; }

மாறிலி String& ProgramSource::source() மாறிலி { OCL_NOT_AVAILABLE(); }
ProgramSource::hash_t ProgramSource::hash() மாறிலி { OCL_NOT_AVAILABLE(); }

/* static */ ProgramSource ProgramSource::from_இருமம்(மாறிலி String& module, மாறிலி String& name, மாறிலி unsigned எழுத்து* binary, மாறிலி size_t size, மாறிலி cv::String& buildOptions) { OCL_NOT_AVAILABLE(); }
/* static */ ProgramSource ProgramSource::fromSPIR(மாறிலி String& module, மாறிலி String& name, மாறிலி unsigned எழுத்து* binary, மாறிலி size_t size, மாறிலி cv::String& buildOptions) { OCL_NOT_AVAILABLE(); }


PlatformInfo::PlatformInfo() CV_NOEXCEPT : p(வெற்று) { }
PlatformInfo::PlatformInfo(வெறுமை* id) : p(வெற்று) { OCL_NOT_AVAILABLE(); }
PlatformInfo::~PlatformInfo() { }

PlatformInfo::PlatformInfo(மாறிலி PlatformInfo& i) : p(வெற்று) { }
PlatformInfo& PlatformInfo::operator=(மாறிலி PlatformInfo& i) { திரும்பு *இது; }
PlatformInfo::PlatformInfo(PlatformInfo&&) CV_NOEXCEPT : p(வெற்று) { }
PlatformInfo& PlatformInfo::operator=(PlatformInfo&&) CV_NOEXCEPT { திரும்பு *இது; }

String PlatformInfo::name() மாறிலி { OCL_NOT_AVAILABLE(); }
String PlatformInfo::vendor() மாறிலி { OCL_NOT_AVAILABLE(); }
String PlatformInfo::version() மாறிலி { OCL_NOT_AVAILABLE(); }
முழு PlatformInfo::device_எண்() மாறிலி { OCL_NOT_AVAILABLE(); }
வெறுமை PlatformInfo::getDevice(Device& device, முழு d) மாறிலி { OCL_NOT_AVAILABLE(); }

மாறிலி எழுத்து* convertTypeStr(முழு sdepth, முழு ddepth, முழு cn, எழுத்து* buf) { OCL_NOT_AVAILABLE(); }
மாறிலி எழுத்து* typeToStr(முழு t) { OCL_NOT_AVAILABLE(); }
மாறிலி எழுத்து* memopTypeToStr(முழு t) { OCL_NOT_AVAILABLE(); }
மாறிலி எழுத்து* vecopTypeToStr(முழு t) { OCL_NOT_AVAILABLE(); }
மாறிலி எழுத்து* getOpenCLErrorString(முழு errorCode) { OCL_NOT_AVAILABLE(); }
String kernelToStr(உள்ளீடு_Array _kernel, முழு ddepth, மாறிலி எழுத்து* name) { OCL_NOT_AVAILABLE(); }
வெறுமை getPlatfomsInfo(std::vector<PlatformInfo>& platform_info) { OCL_NOT_AVAILABLE(); }


முழு predictOptimalVectorWidth(உள்ளீடு_Array src1, உள்ளீடு_Array src2, உள்ளீடு_Array src3,
        உள்ளீடு_Array src4, உள்ளீடு_Array src5, உள்ளீடு_Array src6,
        உள்ளீடு_Array src7, உள்ளீடு_Array src8, உள்ளீடு_Array src9,
        OclVectorStrategy strat)
{ OCL_NOT_AVAILABLE(); }

முழு checkOptimalVectorWidth(மாறிலி முழு *vectorWidths,
        உள்ளீடு_Array src1, உள்ளீடு_Array src2, உள்ளீடு_Array src3,
        உள்ளீடு_Array src4, உள்ளீடு_Array src5, உள்ளீடு_Array src6,
        உள்ளீடு_Array src7, உள்ளீடு_Array src8, உள்ளீடு_Array src9,
        OclVectorStrategy strat)
{ OCL_NOT_AVAILABLE(); }

முழு predictOptimalVectorWidthMax(உள்ளீடு_Array src1, உள்ளீடு_Array src2, உள்ளீடு_Array src3,
        உள்ளீடு_Array src4, உள்ளீடு_Array src5, உள்ளீடு_Array src6,
        உள்ளீடு_Array src7, உள்ளீடு_Array src8, உள்ளீடு_Array src9)
{ OCL_NOT_AVAILABLE(); }

வெறுமை buildOptionsAddMatrixDescription(String& buildOptions, மாறிலி String& name, உள்ளீடு_Array _m) { OCL_NOT_AVAILABLE(); }


Image2D::Image2D() CV_NOEXCEPT : p(வெற்று) { }
Image2D::Image2D(மாறிலி UMat &src, தருக்க norm, தருக்க alias) { OCL_NOT_AVAILABLE(); }
Image2D::Image2D(மாறிலி Image2D & i) : p(வெற்று) { OCL_NOT_AVAILABLE(); }
Image2D::~Image2D() { }
Image2D& Image2D::operator=(மாறிலி Image2D & i) { திரும்பு *இது; }
Image2D::Image2D(Image2D&&) CV_NOEXCEPT : p(வெற்று) { }
Image2D& Image2D::operator=(Image2D&&) CV_NOEXCEPT { திரும்பு *இது; }

/* static */ தருக்க Image2D::canCreateAlias(மாறிலி UMat &u) { OCL_NOT_AVAILABLE(); }
/* static */ தருக்க Image2D::isFormatSupported(முழு depth, முழு cn, தருக்க norm) { OCL_NOT_AVAILABLE(); }

வெறுமை* Image2D::ptr() மாறிலி { திரும்பு வெற்று; }


Timer::Timer(மாறிலி Queue& q) : p(வெற்று) {}
Timer::~Timer() {}
வெறுமை Timer::start() { OCL_NOT_AVAILABLE(); }
வெறுமை Timer::stop() { OCL_NOT_AVAILABLE();}

uint64 Timer::durationNS() மாறிலி { OCL_NOT_AVAILABLE(); }

MatAllocator* getOpenCLAllocator() { திரும்பு வெற்று; }

internal::ProgramEntry::operator ProgramSource&() மாறிலி { OCL_NOT_AVAILABLE(); }


கட்டமைப்பு OpenCLExecutionContext::Impl
{
    Impl() = இயல்பு;
};

Context& OpenCLExecutionContext::getContext() மாறிலி { OCL_NOT_AVAILABLE(); }
Device& OpenCLExecutionContext::getDevice() மாறிலி { OCL_NOT_AVAILABLE(); }
Queue& OpenCLExecutionContext::getQueue() மாறிலி { OCL_NOT_AVAILABLE(); }

தருக்க OpenCLExecutionContext::useOpenCL() மாறிலி { திரும்பு பொய்; }
வெறுமை OpenCLExecutionContext::setUseOpenCL(தருக்க flag) { }

நிலையான
OpenCLExecutionContext& getDummyOpenCLExecutionContext()
{
    நிலையான OpenCLExecutionContext dummy;
    திரும்பு dummy;
}

/* static */
OpenCLExecutionContext& OpenCLExecutionContext::getCurrent() { திரும்பு getDummyOpenCLExecutionContext(); }

/* static */
OpenCLExecutionContext& OpenCLExecutionContext::getCurrentRef() { திரும்பு getDummyOpenCLExecutionContext(); }

வெறுமை OpenCLExecutionContext::bind() மாறிலி { OCL_NOT_AVAILABLE(); }

OpenCLExecutionContext OpenCLExecutionContext::cloneWithNewQueue(மாறிலி ocl::Queue& q) மாறிலி { OCL_NOT_AVAILABLE(); }
OpenCLExecutionContext OpenCLExecutionContext::cloneWithNewQueue() மாறிலி { OCL_NOT_AVAILABLE(); }

/* static */ OpenCLExecutionContext OpenCLExecutionContext::create(மாறிலி std::string& platformName, வெறுமை* platformID, வெறுமை* context, வெறுமை* deviceID) { OCL_NOT_AVAILABLE(); }
/* static */ OpenCLExecutionContext OpenCLExecutionContext::create(மாறிலி Context& context, மாறிலி Device& device, மாறிலி ocl::Queue& queue) { OCL_NOT_AVAILABLE(); }
/* static */ OpenCLExecutionContext OpenCLExecutionContext::create(மாறிலி Context& context, மாறிலி Device& device) { OCL_NOT_AVAILABLE(); }

வெறுமை OpenCLExecutionContext::release() { }

}}

#எனில் defined(_MSC_VER)
    #pragma warning(pop)
#அல்லதெனில் defined(__clang__)
    #pragma clang diagnostic pop
#அல்லதெனில் defined(__GNUC__)
    #pragma GCC diagnostic pop
#endif
