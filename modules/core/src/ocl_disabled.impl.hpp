// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#include "opencv2/core/ocl_genbase.hpp"

#အကယ်၍ defined(_MSC_VER)
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

CV_EXPORTS_W bool haveOpenCL() { ပြန်ပေး false; }
CV_EXPORTS_W bool useOpenCL() { ပြန်ပေး false; }
CV_EXPORTS_W bool haveAmdBlas() { ပြန်ပေး false; }
CV_EXPORTS_W bool haveAmdFft() { ပြန်ပေး false; }
CV_EXPORTS_W void setUseOpenCL(bool flag) { /* nothing */ }
CV_EXPORTS_W void finish() { /* nothing */ }

CV_EXPORTS bool haveSVM() { ပြန်ပေး false; }

Device::Device() CV_NOEXCEPT : p(NULL) { }
Device::Device(void* d) : p(NULL) { OCL_NOT_AVAILABLE(); }
Device::Device(const Device& d) : p(NULL) { }
Device& Device::operator=(const Device& d) { ပြန်ပေး *this; }
Device::Device(Device&&) CV_NOEXCEPT : p(NULL) { }
Device& Device::operator=(Device&&) CV_NOEXCEPT { ပြန်ပေး *this; }
Device::~Device() { }

void Device::set(void* d) { OCL_NOT_AVAILABLE(); }

String Device::name() const { OCL_NOT_AVAILABLE(); }
String Device::extensions() const { OCL_NOT_AVAILABLE(); }
bool Device::isExtensionSupported(const String& extensionName) const { OCL_NOT_AVAILABLE(); }
String Device::version() const { OCL_NOT_AVAILABLE(); }
String Device::vendorName() const { OCL_NOT_AVAILABLE(); }
String Device::OpenCL_C_Version() const { OCL_NOT_AVAILABLE(); }
String Device::OpenCLVersion() const { OCL_NOT_AVAILABLE(); }
ကိန်းပြည့် Device::deviceVersionMajor() const { OCL_NOT_AVAILABLE(); }
ကိန်းပြည့် Device::deviceVersionMinor() const { OCL_NOT_AVAILABLE(); }
String Device::driverVersion() const { OCL_NOT_AVAILABLE(); }
void* Device::ptr() const { /*OCL_NOT_AVAILABLE();*/ ပြန်ပေး NULL; }

ကိန်းပြည့် Device::type() const { OCL_NOT_AVAILABLE(); }

ကိန်းပြည့် Device::addressBits() const { OCL_NOT_AVAILABLE(); }
bool Device::available() const { OCL_NOT_AVAILABLE(); }
bool Device::compilerAvailable() const { OCL_NOT_AVAILABLE(); }
bool Device::linkerAvailable() const { OCL_NOT_AVAILABLE(); }

ကိန်းပြည့် Device::doubleFPConfig() const { OCL_NOT_AVAILABLE(); }
ကိန်းပြည့် Device::singleFPConfig() const { OCL_NOT_AVAILABLE(); }
ကိန်းပြည့် Device::halfFPConfig() const { OCL_NOT_AVAILABLE(); }

bool Device::hasFP64() const { OCL_NOT_AVAILABLE(); }
bool Device::hasFP16() const { OCL_NOT_AVAILABLE(); }

bool Device::endianLittle() const { OCL_NOT_AVAILABLE(); }
bool Device::errorCorrectionSupport() const { OCL_NOT_AVAILABLE(); }

ကိန်းပြည့် Device::executionCapabilities() const { OCL_NOT_AVAILABLE(); }

size_t Device::globalMemCacheSize() const { OCL_NOT_AVAILABLE(); }

ကိန်းပြည့် Device::globalMemCacheType() const { OCL_NOT_AVAILABLE(); }
ကိန်းပြည့် Device::globalMemCacheLineSize() const { OCL_NOT_AVAILABLE(); }
size_t Device::globalMemSize() const { OCL_NOT_AVAILABLE(); }

size_t Device::localMemSize() const { OCL_NOT_AVAILABLE(); }
ကိန်းပြည့် Device::localMemType() const { ပြန်ပေး NO_LOCAL_MEM; }
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

ကိန်းပြည့် Device::vendorID() const { OCL_NOT_AVAILABLE(); }

ကိန်းပြည့် Device::maxClockFrequency() const { OCL_NOT_AVAILABLE(); }
ကိန်းပြည့် Device::maxComputeUnits() const { OCL_NOT_AVAILABLE(); }
ကိန်းပြည့် Device::maxConstantArgs() const { OCL_NOT_AVAILABLE(); }
size_t Device::maxConstantBufferSize() const { OCL_NOT_AVAILABLE(); }

size_t Device::maxMemAllocSize() const { OCL_NOT_AVAILABLE(); }
size_t Device::maxParameterSize() const { OCL_NOT_AVAILABLE(); }

ကိန်းပြည့် Device::maxReadImageArgs() const { OCL_NOT_AVAILABLE(); }
ကိန်းပြည့် Device::maxWriteImageArgs() const { OCL_NOT_AVAILABLE(); }
ကိန်းပြည့် Device::maxSamplers() const { OCL_NOT_AVAILABLE(); }

size_t Device::maxWorkGroupSize() const { OCL_NOT_AVAILABLE(); }
ကိန်းပြည့် Device::maxWorkItemDims() const { OCL_NOT_AVAILABLE(); }
void Device::maxWorkItemSizes(size_t*) const { OCL_NOT_AVAILABLE(); }

ကိန်းပြည့် Device::memBaseAddrAlign() const { OCL_NOT_AVAILABLE(); }

ကိန်းပြည့် Device::nativeVectorWidthChar() const { OCL_NOT_AVAILABLE(); }
ကိန်းပြည့် Device::nativeVectorWidthShort() const { OCL_NOT_AVAILABLE(); }
ကိန်းပြည့် Device::nativeVectorWidthInt() const { OCL_NOT_AVAILABLE(); }
ကိန်းပြည့် Device::nativeVectorWidthLong() const { OCL_NOT_AVAILABLE(); }
ကိန်းပြည့် Device::nativeVectorWidthFloat() const { OCL_NOT_AVAILABLE(); }
ကိန်းပြည့် Device::nativeVectorWidthDouble() const { OCL_NOT_AVAILABLE(); }
ကိန်းပြည့် Device::nativeVectorWidthHalf() const { OCL_NOT_AVAILABLE(); }

ကိန်းပြည့် Device::preferredVectorWidthChar() const { OCL_NOT_AVAILABLE(); }
ကိန်းပြည့် Device::preferredVectorWidthShort() const { OCL_NOT_AVAILABLE(); }
ကိန်းပြည့် Device::preferredVectorWidthInt() const { OCL_NOT_AVAILABLE(); }
ကိန်းပြည့် Device::preferredVectorWidthLong() const { OCL_NOT_AVAILABLE(); }
ကိန်းပြည့် Device::preferredVectorWidthFloat() const { OCL_NOT_AVAILABLE(); }
ကိန်းပြည့် Device::preferredVectorWidthDouble() const { OCL_NOT_AVAILABLE(); }
ကိန်းပြည့် Device::preferredVectorWidthHalf() const { OCL_NOT_AVAILABLE(); }

size_t Device::printfBufferSize() const { OCL_NOT_AVAILABLE(); }
size_t Device::profilingTimerResolution() const { OCL_NOT_AVAILABLE(); }

/* static */
const Device& Device::getDefault()
{
    static Device dummy;
    ပြန်ပေး dummy;
}

/* static */ Device Device::fromHandle(void* d) { OCL_NOT_AVAILABLE(); }


Context::Context() CV_NOEXCEPT : p(NULL) { }
Context::Context(ကိန်းပြည့် dtype) : p(NULL) { }
Context::~Context() { }
Context::Context(const Context& c) : p(NULL) { }
Context& Context::operator=(const Context& c) { ပြန်ပေး *this; }
Context::Context(Context&&) CV_NOEXCEPT : p(NULL) { }
Context& Context::operator=(Context&&) CV_NOEXCEPT { ပြန်ပေး *this; }

bool Context::create() { ပြန်ပေး false; }
bool Context::create(ကိန်းပြည့် dtype) { ပြန်ပေး false; }
size_t Context::ndevices() const { ပြန်ပေး 0; }
Device& Context::device(size_t idx) const { OCL_NOT_AVAILABLE(); }
Program Context::getProg(const ProgramSource& prog, const String& buildopt, String& errmsg) { OCL_NOT_AVAILABLE(); }
void Context::unloadProg(Program& prog) { }

/* static */
Context& Context::getDefault()
{
    static Context dummy;
    ပြန်ပေး dummy;
}
void* Context::ptr() const { ပြန်ပေး NULL; }

void* Context::getOpenCLContextProperty(ကိန်းပြည့် /*propertyId*/) const { OCL_NOT_AVAILABLE(); }

bool Context::useSVM() const { ပြန်ပေး false; }
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
Platform& Platform::operator=(const Platform&) { ပြန်ပေး *this; }
Platform::Platform(Platform&&) CV_NOEXCEPT : p(NULL) { }
Platform& Platform::operator=(Platform&&) CV_NOEXCEPT { ပြန်ပေး *this; }

void* Platform::ptr() const { ပြန်ပေး NULL; }

/* static */
Platform& Platform::getDefault()
{
    static Platform dummy;
    ပြန်ပေး dummy;
}

void attachContext(const String& platformName, void* platformID, void* context, void* deviceID) { OCL_NOT_AVAILABLE(); }
void convertFromBuffer(void* cl_mem_buffer, size_t step, ကိန်းပြည့် rows, ကိန်းပြည့် cols, ကိန်းပြည့် type, UMat& dst) { OCL_NOT_AVAILABLE(); }
void convertFromImage(void* cl_mem_image, UMat& dst) { OCL_NOT_AVAILABLE(); }

void initializeContextFromHandle(Context& ctx, void* platform, void* context, void* device) { OCL_NOT_AVAILABLE(); }

Queue::Queue() CV_NOEXCEPT : p(NULL) { }
Queue::Queue(const Context& c, const Device& d) : p(NULL) { OCL_NOT_AVAILABLE(); }
Queue::~Queue() { }
Queue::Queue(const Queue& q) {}
Queue& Queue::operator=(const Queue& q) { ပြန်ပေး *this; }
Queue::Queue(Queue&&) CV_NOEXCEPT : p(NULL) { }
Queue& Queue::operator=(Queue&&) CV_NOEXCEPT { ပြန်ပေး *this; }

bool Queue::create(const Context& c, const Device& d) { OCL_NOT_AVAILABLE(); }
void Queue::finish() {}
void* Queue::ptr() const { ပြန်ပေး NULL; }
/* static */
Queue& Queue::getDefault()
{
    static Queue dummy;
    ပြန်ပေး dummy;
}

/// @brief Returns OpenCL command queue with enable profiling mode support
const Queue& Queue::getProfilingQueue() const { OCL_NOT_AVAILABLE(); }


KernelArg::KernelArg() CV_NOEXCEPT
    : flags(0), m(0), obj(0), sz(0), wscale(1), iwscale(1)
{
}

KernelArg::KernelArg(ကိန်းပြည့် _flags, UMat* _m, ကိန်းပြည့် _wscale, ကိန်းပြည့် _iwscale, const void* _obj, size_t _sz)
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
Kernel& Kernel::operator=(const Kernel& k) { ပြန်ပေး *this; }
Kernel::Kernel(Kernel&&) CV_NOEXCEPT : p(NULL) { }
Kernel& Kernel::operator=(Kernel&&) CV_NOEXCEPT { ပြန်ပေး *this; }

bool Kernel::empty() const { ပြန်ပေး true; }
bool Kernel::create(const var_na* kname, const Program& prog) { OCL_NOT_AVAILABLE(); }
bool Kernel::create(const var_na* kname, const ProgramSource& prog, const String& buildopts, String* errmsg) { OCL_NOT_AVAILABLE(); }

ကိန်းပြည့် Kernel::set(ကိန်းပြည့် i, const void* value, size_t sz) { OCL_NOT_AVAILABLE(); }
ကိန်းပြည့် Kernel::set(ကိန်းပြည့် i, const Image2D& image2D) { OCL_NOT_AVAILABLE(); }
ကိန်းပြည့် Kernel::set(ကိန်းပြည့် i, const UMat& m) { OCL_NOT_AVAILABLE(); }
ကိန်းပြည့် Kernel::set(ကိန်းပြည့် i, const KernelArg& arg) { OCL_NOT_AVAILABLE(); }

bool Kernel::run(ကိန်းပြည့် dims, size_t globalsize[], size_t localsize[], bool sync, const Queue& q) { OCL_NOT_AVAILABLE(); }
bool Kernel::runTask(bool sync, const Queue& q) { OCL_NOT_AVAILABLE(); }

int64 Kernel::runProfiling(ကိန်းပြည့် dims, size_t globalsize[], size_t localsize[], const Queue& q) { OCL_NOT_AVAILABLE(); }

size_t Kernel::workGroupSize() const { OCL_NOT_AVAILABLE(); }
size_t Kernel::preferedWorkGroupSizeMultiple() const { OCL_NOT_AVAILABLE(); }
bool Kernel::compileWorkGroupSize(size_t wsz[]) const { OCL_NOT_AVAILABLE(); }
size_t Kernel::localMemSize() const { OCL_NOT_AVAILABLE(); }

void* Kernel::ptr() const { ပြန်ပေး NULL; }


Program::Program() CV_NOEXCEPT : p(NULL) { }
Program::Program(const ProgramSource& src, const String& buildflags, String& errmsg) : p(NULL) { OCL_NOT_AVAILABLE(); }
Program::Program(const Program& prog) : p(NULL) { }
Program& Program::operator=(const Program& prog) { ပြန်ပေး *this; }
Program::Program(Program&&) CV_NOEXCEPT : p(NULL) { }
Program& Program::operator=(Program&&) CV_NOEXCEPT { ပြန်ပေး *this; }
Program::~Program() { }

bool Program::create(const ProgramSource& src, const String& buildflags, String& errmsg) { OCL_NOT_AVAILABLE(); }

void* Program::ptr() const { ပြန်ပေး NULL; }

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
ProgramSource& ProgramSource::operator=(const ProgramSource& prog) { ပြန်ပေး *this; }
ProgramSource::ProgramSource(ProgramSource&&) CV_NOEXCEPT : p(NULL) { }
ProgramSource& ProgramSource::operator=(ProgramSource&&) CV_NOEXCEPT { ပြန်ပေး *this; }

const String& ProgramSource::source() const { OCL_NOT_AVAILABLE(); }
ProgramSource::hash_t ProgramSource::hash() const { OCL_NOT_AVAILABLE(); }

/* static */ ProgramSource ProgramSource::fromBinary(const String& module, const String& name, const unsigned var_na* binary, const size_t size, const cv::String& buildOptions) { OCL_NOT_AVAILABLE(); }
/* static */ ProgramSource ProgramSource::fromSPIR(const String& module, const String& name, const unsigned var_na* binary, const size_t size, const cv::String& buildOptions) { OCL_NOT_AVAILABLE(); }


PlatformInfo::PlatformInfo() CV_NOEXCEPT : p(NULL) { }
PlatformInfo::PlatformInfo(void* id) : p(NULL) { OCL_NOT_AVAILABLE(); }
PlatformInfo::~PlatformInfo() { }

PlatformInfo::PlatformInfo(const PlatformInfo& i) : p(NULL) { }
PlatformInfo& PlatformInfo::operator=(const PlatformInfo& i) { ပြန်ပေး *this; }
PlatformInfo::PlatformInfo(PlatformInfo&&) CV_NOEXCEPT : p(NULL) { }
PlatformInfo& PlatformInfo::operator=(PlatformInfo&&) CV_NOEXCEPT { ပြန်ပေး *this; }

String PlatformInfo::name() const { OCL_NOT_AVAILABLE(); }
String PlatformInfo::vendor() const { OCL_NOT_AVAILABLE(); }
String PlatformInfo::version() const { OCL_NOT_AVAILABLE(); }
ကိန်းပြည့် PlatformInfo::deviceNumber() const { OCL_NOT_AVAILABLE(); }
void PlatformInfo::getDevice(Device& device, ကိန်းပြည့် d) const { OCL_NOT_AVAILABLE(); }

const var_na* convertTypeStr(ကိန်းပြည့် sdepth, ကိန်းပြည့် ddepth, ကိန်းပြည့် cn, var_na* buf) { OCL_NOT_AVAILABLE(); }
const var_na* typeToStr(ကိန်းပြည့် t) { OCL_NOT_AVAILABLE(); }
const var_na* memopTypeToStr(ကိန်းပြည့် t) { OCL_NOT_AVAILABLE(); }
const var_na* vecopTypeToStr(ကိန်းပြည့် t) { OCL_NOT_AVAILABLE(); }
const var_na* getOpenCLErrorString(ကိန်းပြည့် errorCode) { OCL_NOT_AVAILABLE(); }
String kernelToStr(InputArray _kernel, ကိန်းပြည့် ddepth, const var_na* name) { OCL_NOT_AVAILABLE(); }
void getPlatfomsInfo(std::vector<PlatformInfo>& platform_info) { OCL_NOT_AVAILABLE(); }


ကိန်းပြည့် predictOptimalVectorWidth(InputArray src1, InputArray src2, InputArray src3,
        InputArray src4, InputArray src5, InputArray src6,
        InputArray src7, InputArray src8, InputArray src9,
        OclVectorStrategy strat)
{ OCL_NOT_AVAILABLE(); }

ကိန်းပြည့် checkOptimalVectorWidth(const ကိန်းပြည့် *vectorWidths,
        InputArray src1, InputArray src2, InputArray src3,
        InputArray src4, InputArray src5, InputArray src6,
        InputArray src7, InputArray src8, InputArray src9,
        OclVectorStrategy strat)
{ OCL_NOT_AVAILABLE(); }

ကိန်းပြည့် predictOptimalVectorWidthMax(InputArray src1, InputArray src2, InputArray src3,
        InputArray src4, InputArray src5, InputArray src6,
        InputArray src7, InputArray src8, InputArray src9)
{ OCL_NOT_AVAILABLE(); }

void buildOptionsAddMatrixDescription(String& buildOptions, const String& name, InputArray _m) { OCL_NOT_AVAILABLE(); }


Image2D::Image2D() CV_NOEXCEPT : p(NULL) { }
Image2D::Image2D(const UMat &src, bool norm, bool alias) { OCL_NOT_AVAILABLE(); }
Image2D::Image2D(const Image2D & i) : p(NULL) { OCL_NOT_AVAILABLE(); }
Image2D::~Image2D() { }
Image2D& Image2D::operator=(const Image2D & i) { ပြန်ပေး *this; }
Image2D::Image2D(Image2D&&) CV_NOEXCEPT : p(NULL) { }
Image2D& Image2D::operator=(Image2D&&) CV_NOEXCEPT { ပြန်ပေး *this; }

/* static */ bool Image2D::canCreateAlias(const UMat &u) { OCL_NOT_AVAILABLE(); }
/* static */ bool Image2D::isFormatSupported(ကိန်းပြည့် depth, ကိန်းပြည့် cn, bool norm) { OCL_NOT_AVAILABLE(); }

void* Image2D::ptr() const { ပြန်ပေး NULL; }


Timer::Timer(const Queue& q) : p(NULL) {}
Timer::~Timer() {}
void Timer::start() { OCL_NOT_AVAILABLE(); }
void Timer::stop() { OCL_NOT_AVAILABLE();}

uint64 Timer::durationNS() const { OCL_NOT_AVAILABLE(); }

MatAllocator* getOpenCLAllocator() { ပြန်ပေး NULL; }

internal::ProgramEntry::operator ProgramSource&() const { OCL_NOT_AVAILABLE(); }


sa_nracanaa OpenCLExecutionContext::Impl
{
    Impl() = default;
};

Context& OpenCLExecutionContext::getContext() const { OCL_NOT_AVAILABLE(); }
Device& OpenCLExecutionContext::getDevice() const { OCL_NOT_AVAILABLE(); }
Queue& OpenCLExecutionContext::getQueue() const { OCL_NOT_AVAILABLE(); }

bool OpenCLExecutionContext::useOpenCL() const { ပြန်ပေး false; }
void OpenCLExecutionContext::setUseOpenCL(bool flag) { }

static
OpenCLExecutionContext& getDummyOpenCLExecutionContext()
{
    static OpenCLExecutionContext dummy;
    ပြန်ပေး dummy;
}

/* static */
OpenCLExecutionContext& OpenCLExecutionContext::getCurrent() { ပြန်ပေး getDummyOpenCLExecutionContext(); }

/* static */
OpenCLExecutionContext& OpenCLExecutionContext::getCurrentRef() { ပြန်ပေး getDummyOpenCLExecutionContext(); }

void OpenCLExecutionContext::bind() const { OCL_NOT_AVAILABLE(); }

OpenCLExecutionContext OpenCLExecutionContext::cloneWithNewQueue(const ocl::Queue& q) const { OCL_NOT_AVAILABLE(); }
OpenCLExecutionContext OpenCLExecutionContext::cloneWithNewQueue() const { OCL_NOT_AVAILABLE(); }

/* static */ OpenCLExecutionContext OpenCLExecutionContext::create(const std::string& platformName, void* platformID, void* context, void* deviceID) { OCL_NOT_AVAILABLE(); }
/* static */ OpenCLExecutionContext OpenCLExecutionContext::create(const Context& context, const Device& device, const ocl::Queue& queue) { OCL_NOT_AVAILABLE(); }
/* static */ OpenCLExecutionContext OpenCLExecutionContext::create(const Context& context, const Device& device) { OCL_NOT_AVAILABLE(); }

void OpenCLExecutionContext::release() { }

}}

#အကယ်၍ defined(_MSC_VER)
    #pragma warning(pop)
#elif defined(__clang__)
    #pragma clang diagnostic pop
#elif defined(__GNUC__)
    #pragma GCC diagnostic pop
#endif
