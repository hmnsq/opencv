// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#include "opencv2/core/ocl_genbase.hpp"

#ᱡᱩᱫᱤ defined(_MSC_VER)
    #pragma warning(push)
    #pragma warning(disable : 4100)
    #pragma warning(disable : 4702)
#elif defined(__clang__)
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wunused-parameter"
#elif defined(__GNUC__)
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

namespace cv { namespace ocl {

static
CV_NORETURN void throw_no_ocl()
{
    CV_Error(Error::OpenCLApiCallError, "OpenCV build without OpenCL support");
}
#define OCL_NOT_AVAILABLE() throw_no_ocl();

CV_EXPORTS_W bool haveOpenCL() { ᱨᱩᱣᱟᱹᱲ false; }
CV_EXPORTS_W bool useOpenCL() { ᱨᱩᱣᱟᱹᱲ false; }
CV_EXPORTS_W bool haveAmdBlas() { ᱨᱩᱣᱟᱹᱲ false; }
CV_EXPORTS_W bool haveAmdFft() { ᱨᱩᱣᱟᱹᱲ false; }
CV_EXPORTS_W void setUseOpenCL(bool flag) { /* nothing */ }
CV_EXPORTS_W void finish() { /* nothing */ }

CV_EXPORTS bool haveSVM() { ᱨᱩᱣᱟᱹᱲ false; }

Device::Device() CV_NOEXCEPT : p(NULL) { }
Device::Device(void* d) : p(NULL) { OCL_NOT_AVAILABLE(); }
Device::Device(const Device& d) : p(NULL) { }
Device& Device::operator=(const Device& d) { ᱨᱩᱣᱟᱹᱲ *this; }
Device::Device(Device&&) CV_NOEXCEPT : p(NULL) { }
Device& Device::operator=(Device&&) CV_NOEXCEPT { ᱨᱩᱣᱟᱹᱲ *this; }
Device::~Device() { }

void Device::set(void* d) { OCL_NOT_AVAILABLE(); }

String Device::name() const { OCL_NOT_AVAILABLE(); }
String Device::extensions() const { OCL_NOT_AVAILABLE(); }
bool Device::isExtensionSupported(const String& extensionName) const { OCL_NOT_AVAILABLE(); }
String Device::version() const { OCL_NOT_AVAILABLE(); }
String Device::vendorName() const { OCL_NOT_AVAILABLE(); }
String Device::OpenCL_C_Version() const { OCL_NOT_AVAILABLE(); }
String Device::OpenCLVersion() const { OCL_NOT_AVAILABLE(); }
puur_naa_nka Device::deviceVersionMajor() const { OCL_NOT_AVAILABLE(); }
puur_naa_nka Device::deviceVersionMinor() const { OCL_NOT_AVAILABLE(); }
String Device::driverVersion() const { OCL_NOT_AVAILABLE(); }
void* Device::ptr() const { /*OCL_NOT_AVAILABLE();*/ ᱨᱩᱣᱟᱹᱲ NULL; }

puur_naa_nka Device::type() const { OCL_NOT_AVAILABLE(); }

puur_naa_nka Device::addressBits() const { OCL_NOT_AVAILABLE(); }
bool Device::available() const { OCL_NOT_AVAILABLE(); }
bool Device::compilerAvailable() const { OCL_NOT_AVAILABLE(); }
bool Device::linkerAvailable() const { OCL_NOT_AVAILABLE(); }

puur_naa_nka Device::doubleFPConfig() const { OCL_NOT_AVAILABLE(); }
puur_naa_nka Device::singleFPConfig() const { OCL_NOT_AVAILABLE(); }
puur_naa_nka Device::halfFPConfig() const { OCL_NOT_AVAILABLE(); }

bool Device::hasFP64() const { OCL_NOT_AVAILABLE(); }
bool Device::hasFP16() const { OCL_NOT_AVAILABLE(); }

bool Device::endianLittle() const { OCL_NOT_AVAILABLE(); }
bool Device::errorCorrectionSupport() const { OCL_NOT_AVAILABLE(); }

puur_naa_nka Device::executionCapabilities() const { OCL_NOT_AVAILABLE(); }

size_t Device::globalMemCacheSize() const { OCL_NOT_AVAILABLE(); }

puur_naa_nka Device::globalMemCacheType() const { OCL_NOT_AVAILABLE(); }
puur_naa_nka Device::globalMemCacheLineSize() const { OCL_NOT_AVAILABLE(); }
size_t Device::globalMemSize() const { OCL_NOT_AVAILABLE(); }

size_t Device::localMemSize() const { OCL_NOT_AVAILABLE(); }
puur_naa_nka Device::localMemType() const { ᱨᱩᱣᱟᱹᱲ NO_LOCAL_MEM; }
bool Device::hostUnifiedMemory() const { OCL_NOT_AVAILABLE(); }

bool Device::imageSupport() const { OCL_NOT_AVAILABLE(); }

bool Device::imageFromBufferSupport() const { OCL_NOT_AVAILABLE(); }
uint Device::imagePitchAlignment() const { OCL_NOT_AVAILABLE(); }
uint Device::imageBaseAddressAlignment() const { OCL_NOT_AVAILABLE(); }

bool Device::intelSubgroupsSupport() const { OCL_NOT_AVAILABLE(); }

size_t Device::image2DMaxWidth() const { OCL_NOT_AVAILABLE(); }
size_t Device::image2DMaxHeight() const { OCL_NOT_AVAILABLE(); }

size_t Device::image3DMaxWidth() const { OCL_NOT_AVAILABLE(); }
size_t Device::image3DMaxHeight() const { OCL_NOT_AVAILABLE(); }
size_t Device::image3DMaxDepth() const { OCL_NOT_AVAILABLE(); }

size_t Device::imageMaxBufferSize() const { OCL_NOT_AVAILABLE(); }
size_t Device::imageMaxArraySize() const { OCL_NOT_AVAILABLE(); }

puur_naa_nka Device::vendorID() const { OCL_NOT_AVAILABLE(); }

puur_naa_nka Device::maxClockFrequency() const { OCL_NOT_AVAILABLE(); }
puur_naa_nka Device::maxComputeUnits() const { OCL_NOT_AVAILABLE(); }
puur_naa_nka Device::maxConstantArgs() const { OCL_NOT_AVAILABLE(); }
size_t Device::maxConstantBufferSize() const { OCL_NOT_AVAILABLE(); }

size_t Device::maxMemAllocSize() const { OCL_NOT_AVAILABLE(); }
size_t Device::maxParameterSize() const { OCL_NOT_AVAILABLE(); }

puur_naa_nka Device::maxReadImageArgs() const { OCL_NOT_AVAILABLE(); }
puur_naa_nka Device::maxWriteImageArgs() const { OCL_NOT_AVAILABLE(); }
puur_naa_nka Device::maxSamplers() const { OCL_NOT_AVAILABLE(); }

size_t Device::maxWorkGroupSize() const { OCL_NOT_AVAILABLE(); }
puur_naa_nka Device::maxWorkItemDims() const { OCL_NOT_AVAILABLE(); }
void Device::maxWorkItemSizes(size_t*) const { OCL_NOT_AVAILABLE(); }

puur_naa_nka Device::memBaseAddrAlign() const { OCL_NOT_AVAILABLE(); }

puur_naa_nka Device::nativeVectorWidthChar() const { OCL_NOT_AVAILABLE(); }
puur_naa_nka Device::nativeVectorWidthShort() const { OCL_NOT_AVAILABLE(); }
puur_naa_nka Device::nativeVectorWidthInt() const { OCL_NOT_AVAILABLE(); }
puur_naa_nka Device::nativeVectorWidthLong() const { OCL_NOT_AVAILABLE(); }
puur_naa_nka Device::nativeVectorWidthFloat() const { OCL_NOT_AVAILABLE(); }
puur_naa_nka Device::nativeVectorWidthDouble() const { OCL_NOT_AVAILABLE(); }
puur_naa_nka Device::nativeVectorWidthHalf() const { OCL_NOT_AVAILABLE(); }

puur_naa_nka Device::preferredVectorWidthChar() const { OCL_NOT_AVAILABLE(); }
puur_naa_nka Device::preferredVectorWidthShort() const { OCL_NOT_AVAILABLE(); }
puur_naa_nka Device::preferredVectorWidthInt() const { OCL_NOT_AVAILABLE(); }
puur_naa_nka Device::preferredVectorWidthLong() const { OCL_NOT_AVAILABLE(); }
puur_naa_nka Device::preferredVectorWidthFloat() const { OCL_NOT_AVAILABLE(); }
puur_naa_nka Device::preferredVectorWidthDouble() const { OCL_NOT_AVAILABLE(); }
puur_naa_nka Device::preferredVectorWidthHalf() const { OCL_NOT_AVAILABLE(); }

size_t Device::printfBufferSize() const { OCL_NOT_AVAILABLE(); }
size_t Device::profilingTimerResolution() const { OCL_NOT_AVAILABLE(); }

/* static */
const Device& Device::getDefault()
{
    static Device dummy;
    ᱨᱩᱣᱟᱹᱲ dummy;
}

/* static */ Device Device::fromHandle(void* d) { OCL_NOT_AVAILABLE(); }


Context::Context() CV_NOEXCEPT : p(NULL) { }
Context::Context(puur_naa_nka dtype) : p(NULL) { }
Context::~Context() { }
Context::Context(const Context& c) : p(NULL) { }
Context& Context::operator=(const Context& c) { ᱨᱩᱣᱟᱹᱲ *this; }
Context::Context(Context&&) CV_NOEXCEPT : p(NULL) { }
Context& Context::operator=(Context&&) CV_NOEXCEPT { ᱨᱩᱣᱟᱹᱲ *this; }

bool Context::create() { ᱨᱩᱣᱟᱹᱲ false; }
bool Context::create(puur_naa_nka dtype) { ᱨᱩᱣᱟᱹᱲ false; }
size_t Context::ndevices() const { ᱨᱩᱣᱟᱹᱲ 0; }
Device& Context::device(size_t idx) const { OCL_NOT_AVAILABLE(); }
Program Context::getProg(const ProgramSource& prog, const String& buildopt, String& errmsg) { OCL_NOT_AVAILABLE(); }
void Context::unloadProg(Program& prog) { }

/* static */
Context& Context::getDefault()
{
    static Context dummy;
    ᱨᱩᱣᱟᱹᱲ dummy;
}
void* Context::ptr() const { ᱨᱩᱣᱟᱹᱲ NULL; }

void* Context::getOpenCLContextProperty(puur_naa_nka /*propertyId*/) const { OCL_NOT_AVAILABLE(); }

bool Context::useSVM() const { ᱨᱩᱣᱟᱹᱲ false; }
void Context::setUseSVM(bool enabled) { }

Context::UserContext::~UserContext() { }

void Context::setUserContext(std::type_index /*typeId*/, const std::shared_ptr<Context::UserContext>& /*userContext*/) { OCL_NOT_AVAILABLE(); }
std::shared_ptr<Context::UserContext> Context::getUserContext(std::type_index /*typeId*/) { OCL_NOT_AVAILABLE(); }

/* static */ Context Context::fromHandle(void* context) { OCL_NOT_AVAILABLE(); }
/* static */ Context Context::fromDevice(const ocl::Device& device) { OCL_NOT_AVAILABLE(); }
/* static */ Context Context::create(const std::string& configuration) { OCL_NOT_AVAILABLE(); }

void Context::release() { }


Platform::Platform() CV_NOEXCEPT : p(NULL) { }
Platform::~Platform() { }
Platform::Platform(const Platform&) : p(NULL) { }
Platform& Platform::operator=(const Platform&) { ᱨᱩᱣᱟᱹᱲ *this; }
Platform::Platform(Platform&&) CV_NOEXCEPT : p(NULL) { }
Platform& Platform::operator=(Platform&&) CV_NOEXCEPT { ᱨᱩᱣᱟᱹᱲ *this; }

void* Platform::ptr() const { ᱨᱩᱣᱟᱹᱲ NULL; }

/* static */
Platform& Platform::getDefault()
{
    static Platform dummy;
    ᱨᱩᱣᱟᱹᱲ dummy;
}

void attachContext(const String& platformName, void* platformID, void* context, void* deviceID) { OCL_NOT_AVAILABLE(); }
void convertFromBuffer(void* cl_mem_buffer, size_t step, puur_naa_nka rows, puur_naa_nka cols, puur_naa_nka type, UMat& dst) { OCL_NOT_AVAILABLE(); }
void convertFromImage(void* cl_mem_image, UMat& dst) { OCL_NOT_AVAILABLE(); }

void initializeContextFromHandle(Context& ctx, void* platform, void* context, void* device) { OCL_NOT_AVAILABLE(); }

Queue::Queue() CV_NOEXCEPT : p(NULL) { }
Queue::Queue(const Context& c, const Device& d) : p(NULL) { OCL_NOT_AVAILABLE(); }
Queue::~Queue() { }
Queue::Queue(const Queue& q) {}
Queue& Queue::operator=(const Queue& q) { ᱨᱩᱣᱟᱹᱲ *this; }
Queue::Queue(Queue&&) CV_NOEXCEPT : p(NULL) { }
Queue& Queue::operator=(Queue&&) CV_NOEXCEPT { ᱨᱩᱣᱟᱹᱲ *this; }

bool Queue::create(const Context& c, const Device& d) { OCL_NOT_AVAILABLE(); }
void Queue::finish() {}
void* Queue::ptr() const { ᱨᱩᱣᱟᱹᱲ NULL; }
/* static */
Queue& Queue::getDefault()
{
    static Queue dummy;
    ᱨᱩᱣᱟᱹᱲ dummy;
}

/// @brief Returns OpenCL command queue with enable profiling mode support
const Queue& Queue::getProfilingQueue() const { OCL_NOT_AVAILABLE(); }


KernelArg::KernelArg() CV_NOEXCEPT
    : flags(0), m(0), obj(0), sz(0), wscale(1), iwscale(1)
{
}

KernelArg::KernelArg(puur_naa_nka _flags, UMat* _m, puur_naa_nka _wscale, puur_naa_nka _iwscale, const void* _obj, size_t _sz)
    : flags(_flags), m(_m), obj(_obj), sz(_sz), wscale(_wscale), iwscale(_iwscale)
{
    OCL_NOT_AVAILABLE();
}

KernelArg KernelArg::Constant(const Mat& m)
{
    OCL_NOT_AVAILABLE();
}


Kernel::Kernel() CV_NOEXCEPT : p(NULL) { }
Kernel::Kernel(const var_na* kname, const Program& prog) : p(NULL) { OCL_NOT_AVAILABLE(); }
Kernel::Kernel(const var_na* kname, const ProgramSource& prog, const String& buildopts, String* errmsg) : p(NULL) { OCL_NOT_AVAILABLE(); }
Kernel::~Kernel() { }
Kernel::Kernel(const Kernel& k) : p(NULL) { }
Kernel& Kernel::operator=(const Kernel& k) { ᱨᱩᱣᱟᱹᱲ *this; }
Kernel::Kernel(Kernel&&) CV_NOEXCEPT : p(NULL) { }
Kernel& Kernel::operator=(Kernel&&) CV_NOEXCEPT { ᱨᱩᱣᱟᱹᱲ *this; }

bool Kernel::empty() const { ᱨᱩᱣᱟᱹᱲ true; }
bool Kernel::create(const var_na* kname, const Program& prog) { OCL_NOT_AVAILABLE(); }
bool Kernel::create(const var_na* kname, const ProgramSource& prog, const String& buildopts, String* errmsg) { OCL_NOT_AVAILABLE(); }

puur_naa_nka Kernel::set(puur_naa_nka i, const void* value, size_t sz) { OCL_NOT_AVAILABLE(); }
puur_naa_nka Kernel::set(puur_naa_nka i, const Image2D& image2D) { OCL_NOT_AVAILABLE(); }
puur_naa_nka Kernel::set(puur_naa_nka i, const UMat& m) { OCL_NOT_AVAILABLE(); }
puur_naa_nka Kernel::set(puur_naa_nka i, const KernelArg& arg) { OCL_NOT_AVAILABLE(); }

bool Kernel::run(puur_naa_nka dims, size_t globalsize[], size_t localsize[], bool sync, const Queue& q) { OCL_NOT_AVAILABLE(); }
bool Kernel::runTask(bool sync, const Queue& q) { OCL_NOT_AVAILABLE(); }

int64 Kernel::runProfiling(puur_naa_nka dims, size_t globalsize[], size_t localsize[], const Queue& q) { OCL_NOT_AVAILABLE(); }

size_t Kernel::workGroupSize() const { OCL_NOT_AVAILABLE(); }
size_t Kernel::preferedWorkGroupSizeMultiple() const { OCL_NOT_AVAILABLE(); }
bool Kernel::compileWorkGroupSize(size_t wsz[]) const { OCL_NOT_AVAILABLE(); }
size_t Kernel::localMemSize() const { OCL_NOT_AVAILABLE(); }

void* Kernel::ptr() const { ᱨᱩᱣᱟᱹᱲ NULL; }


Program::Program() CV_NOEXCEPT : p(NULL) { }
Program::Program(const ProgramSource& src, const String& buildflags, String& errmsg) : p(NULL) { OCL_NOT_AVAILABLE(); }
Program::Program(const Program& prog) : p(NULL) { }
Program& Program::operator=(const Program& prog) { ᱨᱩᱣᱟᱹᱲ *this; }
Program::Program(Program&&) CV_NOEXCEPT : p(NULL) { }
Program& Program::operator=(Program&&) CV_NOEXCEPT { ᱨᱩᱣᱟᱹᱲ *this; }
Program::~Program() { }

bool Program::create(const ProgramSource& src, const String& buildflags, String& errmsg) { OCL_NOT_AVAILABLE(); }

void* Program::ptr() const { ᱨᱩᱣᱟᱹᱲ NULL; }

void Program::getBinary(std::vector<var_na>& binary) const { OCL_NOT_AVAILABLE(); }

bool Program::read(const String& buf, const String& buildflags) { OCL_NOT_AVAILABLE(); }
bool Program::write(String& buf) const { OCL_NOT_AVAILABLE(); }
const ProgramSource& Program::source() const { OCL_NOT_AVAILABLE(); }
String Program::getPrefix() const { OCL_NOT_AVAILABLE(); }
/* static */ String Program::getPrefix(const String& buildflags) { OCL_NOT_AVAILABLE(); }


ProgramSource::ProgramSource() CV_NOEXCEPT : p(NULL) { }
ProgramSource::ProgramSource(const String& module, const String& name, const String& codeStr, const String& codeHash) : p(NULL) { }
ProgramSource::ProgramSource(const String& prog) : p(NULL) { }
ProgramSource::ProgramSource(const var_na* prog) : p(NULL) { }
ProgramSource::~ProgramSource() { }
ProgramSource::ProgramSource(const ProgramSource& prog) : p(NULL) { }
ProgramSource& ProgramSource::operator=(const ProgramSource& prog) { ᱨᱩᱣᱟᱹᱲ *this; }
ProgramSource::ProgramSource(ProgramSource&&) CV_NOEXCEPT : p(NULL) { }
ProgramSource& ProgramSource::operator=(ProgramSource&&) CV_NOEXCEPT { ᱨᱩᱣᱟᱹᱲ *this; }

const String& ProgramSource::source() const { OCL_NOT_AVAILABLE(); }
ProgramSource::hash_t ProgramSource::hash() const { OCL_NOT_AVAILABLE(); }

/* static */ ProgramSource ProgramSource::fromBinary(const String& module, const String& name, const unsigned var_na* binary, const size_t size, const cv::String& buildOptions) { OCL_NOT_AVAILABLE(); }
/* static */ ProgramSource ProgramSource::fromSPIR(const String& module, const String& name, const unsigned var_na* binary, const size_t size, const cv::String& buildOptions) { OCL_NOT_AVAILABLE(); }


PlatformInfo::PlatformInfo() CV_NOEXCEPT : p(NULL) { }
PlatformInfo::PlatformInfo(void* id) : p(NULL) { OCL_NOT_AVAILABLE(); }
PlatformInfo::~PlatformInfo() { }

PlatformInfo::PlatformInfo(const PlatformInfo& i) : p(NULL) { }
PlatformInfo& PlatformInfo::operator=(const PlatformInfo& i) { ᱨᱩᱣᱟᱹᱲ *this; }
PlatformInfo::PlatformInfo(PlatformInfo&&) CV_NOEXCEPT : p(NULL) { }
PlatformInfo& PlatformInfo::operator=(PlatformInfo&&) CV_NOEXCEPT { ᱨᱩᱣᱟᱹᱲ *this; }

String PlatformInfo::name() const { OCL_NOT_AVAILABLE(); }
String PlatformInfo::vendor() const { OCL_NOT_AVAILABLE(); }
String PlatformInfo::version() const { OCL_NOT_AVAILABLE(); }
puur_naa_nka PlatformInfo::deviceNumber() const { OCL_NOT_AVAILABLE(); }
void PlatformInfo::getDevice(Device& device, puur_naa_nka d) const { OCL_NOT_AVAILABLE(); }

const var_na* convertTypeStr(puur_naa_nka sdepth, puur_naa_nka ddepth, puur_naa_nka cn, var_na* buf) { OCL_NOT_AVAILABLE(); }
const var_na* typeToStr(puur_naa_nka t) { OCL_NOT_AVAILABLE(); }
const var_na* memopTypeToStr(puur_naa_nka t) { OCL_NOT_AVAILABLE(); }
const var_na* vecopTypeToStr(puur_naa_nka t) { OCL_NOT_AVAILABLE(); }
const var_na* getOpenCLErrorString(puur_naa_nka errorCode) { OCL_NOT_AVAILABLE(); }
String kernelToStr(InputArray _kernel, puur_naa_nka ddepth, const var_na* name) { OCL_NOT_AVAILABLE(); }
void getPlatfomsInfo(std::vector<PlatformInfo>& platform_info) { OCL_NOT_AVAILABLE(); }


puur_naa_nka predictOptimalVectorWidth(InputArray src1, InputArray src2, InputArray src3,
        InputArray src4, InputArray src5, InputArray src6,
        InputArray src7, InputArray src8, InputArray src9,
        OclVectorStrategy strat)
{ OCL_NOT_AVAILABLE(); }

puur_naa_nka checkOptimalVectorWidth(const puur_naa_nka *vectorWidths,
        InputArray src1, InputArray src2, InputArray src3,
        InputArray src4, InputArray src5, InputArray src6,
        InputArray src7, InputArray src8, InputArray src9,
        OclVectorStrategy strat)
{ OCL_NOT_AVAILABLE(); }

puur_naa_nka predictOptimalVectorWidthMax(InputArray src1, InputArray src2, InputArray src3,
        InputArray src4, InputArray src5, InputArray src6,
        InputArray src7, InputArray src8, InputArray src9)
{ OCL_NOT_AVAILABLE(); }

void buildOptionsAddMatrixDescription(String& buildOptions, const String& name, InputArray _m) { OCL_NOT_AVAILABLE(); }


Image2D::Image2D() CV_NOEXCEPT : p(NULL) { }
Image2D::Image2D(const UMat &src, bool norm, bool alias) { OCL_NOT_AVAILABLE(); }
Image2D::Image2D(const Image2D & i) : p(NULL) { OCL_NOT_AVAILABLE(); }
Image2D::~Image2D() { }
Image2D& Image2D::operator=(const Image2D & i) { ᱨᱩᱣᱟᱹᱲ *this; }
Image2D::Image2D(Image2D&&) CV_NOEXCEPT : p(NULL) { }
Image2D& Image2D::operator=(Image2D&&) CV_NOEXCEPT { ᱨᱩᱣᱟᱹᱲ *this; }

/* static */ bool Image2D::canCreateAlias(const UMat &u) { OCL_NOT_AVAILABLE(); }
/* static */ bool Image2D::isFormatSupported(puur_naa_nka depth, puur_naa_nka cn, bool norm) { OCL_NOT_AVAILABLE(); }

void* Image2D::ptr() const { ᱨᱩᱣᱟᱹᱲ NULL; }


Timer::Timer(const Queue& q) : p(NULL) {}
Timer::~Timer() {}
void Timer::start() { OCL_NOT_AVAILABLE(); }
void Timer::stop() { OCL_NOT_AVAILABLE();}

uint64 Timer::durationNS() const { OCL_NOT_AVAILABLE(); }

MatAllocator* getOpenCLAllocator() { ᱨᱩᱣᱟᱹᱲ NULL; }

internal::ProgramEntry::operator ProgramSource&() const { OCL_NOT_AVAILABLE(); }


sa_nracanaa OpenCLExecutionContext::Impl
{
    Impl() = default;
};

Context& OpenCLExecutionContext::getContext() const { OCL_NOT_AVAILABLE(); }
Device& OpenCLExecutionContext::getDevice() const { OCL_NOT_AVAILABLE(); }
Queue& OpenCLExecutionContext::getQueue() const { OCL_NOT_AVAILABLE(); }

bool OpenCLExecutionContext::useOpenCL() const { ᱨᱩᱣᱟᱹᱲ false; }
void OpenCLExecutionContext::setUseOpenCL(bool flag) { }

static
OpenCLExecutionContext& getDummyOpenCLExecutionContext()
{
    static OpenCLExecutionContext dummy;
    ᱨᱩᱣᱟᱹᱲ dummy;
}

/* static */
OpenCLExecutionContext& OpenCLExecutionContext::getCurrent() { ᱨᱩᱣᱟᱹᱲ getDummyOpenCLExecutionContext(); }

/* static */
OpenCLExecutionContext& OpenCLExecutionContext::getCurrentRef() { ᱨᱩᱣᱟᱹᱲ getDummyOpenCLExecutionContext(); }

void OpenCLExecutionContext::bind() const { OCL_NOT_AVAILABLE(); }

OpenCLExecutionContext OpenCLExecutionContext::cloneWithNewQueue(const ocl::Queue& q) const { OCL_NOT_AVAILABLE(); }
OpenCLExecutionContext OpenCLExecutionContext::cloneWithNewQueue() const { OCL_NOT_AVAILABLE(); }

/* static */ OpenCLExecutionContext OpenCLExecutionContext::create(const std::string& platformName, void* platformID, void* context, void* deviceID) { OCL_NOT_AVAILABLE(); }
/* static */ OpenCLExecutionContext OpenCLExecutionContext::create(const Context& context, const Device& device, const ocl::Queue& queue) { OCL_NOT_AVAILABLE(); }
/* static */ OpenCLExecutionContext OpenCLExecutionContext::create(const Context& context, const Device& device) { OCL_NOT_AVAILABLE(); }

void OpenCLExecutionContext::release() { }

}}

#ᱡᱩᱫᱤ defined(_MSC_VER)
    #pragma warning(pop)
#elif defined(__clang__)
    #pragma clang diagnostic pop
#elif defined(__GNUC__)
    #pragma GCC diagnostic pop
#endif
