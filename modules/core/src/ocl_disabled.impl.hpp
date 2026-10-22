// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ଅନ୍ତର୍ଭାଵ "opencv2/core/ocl_genbase.hpp"

#ଯଦି defined(_MSC_VER)
    #pragma warning(push)
    #pragma warning(disable : 4100)
    #pragma warning(disable : 4702)
#ନଚେତଯଦି defined(__clang__)
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wunused-parameter"
#ନଚେତଯଦି defined(__GNUC__)
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

ନାମାକାଶ cv { ନାମାକାଶ ocl {

ସ୍ଥିର
CV_NORETURN ଶୂନ୍ୟ throw_no_ocl()
{
    CV_Error(Error::OpenCLApiCallError, "OpenCV build without OpenCL support");
}
#define OCL_NOT_AVAILABLE() throw_no_ocl();

CV_EXPORTS_W ତାର୍କିକ haveOpenCL() { ଫେରନ୍ତୁ ମିଥ୍ୟା; }
CV_EXPORTS_W ତାର୍କିକ useOpenCL() { ଫେରନ୍ତୁ ମିଥ୍ୟା; }
CV_EXPORTS_W ତାର୍କିକ haveAmdBlas() { ଫେରନ୍ତୁ ମିଥ୍ୟା; }
CV_EXPORTS_W ତାର୍କିକ haveAmdFft() { ଫେରନ୍ତୁ ମିଥ୍ୟା; }
CV_EXPORTS_W ଶୂନ୍ୟ setUseOpenCL(ତାର୍କିକ flag) { /* nothing */ }
CV_EXPORTS_W ଶୂନ୍ୟ finish() { /* nothing */ }

CV_EXPORTS ତାର୍କିକ haveSVM() { ଫେରନ୍ତୁ ମିଥ୍ୟା; }

Device::Device() CV_NOEXCEPT : p(ଖାଲି) { }
Device::Device(ଶୂନ୍ୟ* d) : p(ଖାଲି) { OCL_NOT_AVAILABLE(); }
Device::Device(ଅଚଳ Device& d) : p(ଖାଲି) { }
Device& Device::operator=(ଅଚଳ Device& d) { ଫେରନ୍ତୁ *ଏହି; }
Device::Device(Device&&) CV_NOEXCEPT : p(ଖାଲି) { }
Device& Device::operator=(Device&&) CV_NOEXCEPT { ଫେରନ୍ତୁ *ଏହି; }
Device::~Device() { }

ଶୂନ୍ୟ Device::set(ଶୂନ୍ୟ* d) { OCL_NOT_AVAILABLE(); }

String Device::name() ଅଚଳ { OCL_NOT_AVAILABLE(); }
String Device::extensions() ଅଚଳ { OCL_NOT_AVAILABLE(); }
ତାର୍କିକ Device::isExtensionSupported(ଅଚଳ String& extensionName) ଅଚଳ { OCL_NOT_AVAILABLE(); }
String Device::version() ଅଚଳ { OCL_NOT_AVAILABLE(); }
String Device::vendorName() ଅଚଳ { OCL_NOT_AVAILABLE(); }
String Device::OpenCL_C_Version() ଅଚଳ { OCL_NOT_AVAILABLE(); }
String Device::OpenCLVersion() ଅଚଳ { OCL_NOT_AVAILABLE(); }
ପୂର୍ଣ୍ଣସଂଖ୍ୟା Device::deviceVersionMajor() ଅଚଳ { OCL_NOT_AVAILABLE(); }
ପୂର୍ଣ୍ଣସଂଖ୍ୟା Device::deviceVersionMinor() ଅଚଳ { OCL_NOT_AVAILABLE(); }
String Device::driverVersion() ଅଚଳ { OCL_NOT_AVAILABLE(); }
ଶୂନ୍ୟ* Device::ptr() ଅଚଳ { /*OCL_NOT_AVAILABLE();*/ ଫେରନ୍ତୁ ଖାଲି; }

ପୂର୍ଣ୍ଣସଂଖ୍ୟା Device::type() ଅଚଳ { OCL_NOT_AVAILABLE(); }

ପୂର୍ଣ୍ଣସଂଖ୍ୟା Device::addressBits() ଅଚଳ { OCL_NOT_AVAILABLE(); }
ତାର୍କିକ Device::available() ଅଚଳ { OCL_NOT_AVAILABLE(); }
ତାର୍କିକ Device::compilerAvailable() ଅଚଳ { OCL_NOT_AVAILABLE(); }
ତାର୍କିକ Device::linkerAvailable() ଅଚଳ { OCL_NOT_AVAILABLE(); }

ପୂର୍ଣ୍ଣସଂଖ୍ୟା Device::doubleFPConfig() ଅଚଳ { OCL_NOT_AVAILABLE(); }
ପୂର୍ଣ୍ଣସଂଖ୍ୟା Device::singleFPConfig() ଅଚଳ { OCL_NOT_AVAILABLE(); }
ପୂର୍ଣ୍ଣସଂଖ୍ୟା Device::halfFPConfig() ଅଚଳ { OCL_NOT_AVAILABLE(); }

ତାର୍କିକ Device::hasFP64() ଅଚଳ { OCL_NOT_AVAILABLE(); }
ତାର୍କିକ Device::hasFP16() ଅଚଳ { OCL_NOT_AVAILABLE(); }

ତାର୍କିକ Device::endianLittle() ଅଚଳ { OCL_NOT_AVAILABLE(); }
ତାର୍କିକ Device::errorCorrectionSupport() ଅଚଳ { OCL_NOT_AVAILABLE(); }

ପୂର୍ଣ୍ଣସଂଖ୍ୟା Device::executionCapabilities() ଅଚଳ { OCL_NOT_AVAILABLE(); }

size_t Device::globalMemCacheSize() ଅଚଳ { OCL_NOT_AVAILABLE(); }

ପୂର୍ଣ୍ଣସଂଖ୍ୟା Device::globalMemCacheType() ଅଚଳ { OCL_NOT_AVAILABLE(); }
ପୂର୍ଣ୍ଣସଂଖ୍ୟା Device::global_Mem_Cache_ଧାଡ଼ି_Size() ଅଚଳ { OCL_NOT_AVAILABLE(); }
size_t Device::globalMemSize() ଅଚଳ { OCL_NOT_AVAILABLE(); }

size_t Device::localMemSize() ଅଚଳ { OCL_NOT_AVAILABLE(); }
ପୂର୍ଣ୍ଣସଂଖ୍ୟା Device::localMemType() ଅଚଳ { ଫେରନ୍ତୁ NO_LOCAL_MEM; }
ତାର୍କିକ Device::hostUnifiedMemory() ଅଚଳ { OCL_NOT_AVAILABLE(); }

ତାର୍କିକ Device::imageSupport() ଅଚଳ { OCL_NOT_AVAILABLE(); }

ତାର୍କିକ Device::imageFromBufferSupport() ଅଚଳ { OCL_NOT_AVAILABLE(); }
uint Device::imagePitchAlignment() ଅଚଳ { OCL_NOT_AVAILABLE(); }
uint Device::imageBaseAddressAlignment() ଅଚଳ { OCL_NOT_AVAILABLE(); }

ତାର୍କିକ Device::intelSubgroupsSupport() ଅଚଳ { OCL_NOT_AVAILABLE(); }

size_t Device::image2DMaxWidth() ଅଚଳ { OCL_NOT_AVAILABLE(); }
size_t Device::image2DMaxHeight() ଅଚଳ { OCL_NOT_AVAILABLE(); }

size_t Device::image3DMaxWidth() ଅଚଳ { OCL_NOT_AVAILABLE(); }
size_t Device::image3DMaxHeight() ଅଚଳ { OCL_NOT_AVAILABLE(); }
size_t Device::image3DMaxDepth() ଅଚଳ { OCL_NOT_AVAILABLE(); }

size_t Device::imageMaxBufferSize() ଅଚଳ { OCL_NOT_AVAILABLE(); }
size_t Device::imageMaxArraySize() ଅଚଳ { OCL_NOT_AVAILABLE(); }

ପୂର୍ଣ୍ଣସଂଖ୍ୟା Device::vendorID() ଅଚଳ { OCL_NOT_AVAILABLE(); }

ପୂର୍ଣ୍ଣସଂଖ୍ୟା Device::maxClockFrequency() ଅଚଳ { OCL_NOT_AVAILABLE(); }
ପୂର୍ଣ୍ଣସଂଖ୍ୟା Device::maxComputeUnits() ଅଚଳ { OCL_NOT_AVAILABLE(); }
ପୂର୍ଣ୍ଣସଂଖ୍ୟା Device::maxConstantArgs() ଅଚଳ { OCL_NOT_AVAILABLE(); }
size_t Device::maxConstantBufferSize() ଅଚଳ { OCL_NOT_AVAILABLE(); }

size_t Device::maxMemAllocSize() ଅଚଳ { OCL_NOT_AVAILABLE(); }
size_t Device::maxParameterSize() ଅଚଳ { OCL_NOT_AVAILABLE(); }

ପୂର୍ଣ୍ଣସଂଖ୍ୟା Device::maxReadImageArgs() ଅଚଳ { OCL_NOT_AVAILABLE(); }
ପୂର୍ଣ୍ଣସଂଖ୍ୟା Device::maxWriteImageArgs() ଅଚଳ { OCL_NOT_AVAILABLE(); }
ପୂର୍ଣ୍ଣସଂଖ୍ୟା Device::maxSamplers() ଅଚଳ { OCL_NOT_AVAILABLE(); }

size_t Device::maxWorkGroupSize() ଅଚଳ { OCL_NOT_AVAILABLE(); }
ପୂର୍ଣ୍ଣସଂଖ୍ୟା Device::maxWorkItemDims() ଅଚଳ { OCL_NOT_AVAILABLE(); }
ଶୂନ୍ୟ Device::maxWorkItemSizes(size_t*) ଅଚଳ { OCL_NOT_AVAILABLE(); }

ପୂର୍ଣ୍ଣସଂଖ୍ୟା Device::memBaseAddrAlign() ଅଚଳ { OCL_NOT_AVAILABLE(); }

ପୂର୍ଣ୍ଣସଂଖ୍ୟା Device::nativeVectorWidthChar() ଅଚଳ { OCL_NOT_AVAILABLE(); }
ପୂର୍ଣ୍ଣସଂଖ୍ୟା Device::nativeVectorWidthShort() ଅଚଳ { OCL_NOT_AVAILABLE(); }
ପୂର୍ଣ୍ଣସଂଖ୍ୟା Device::nativeVectorWidthInt() ଅଚଳ { OCL_NOT_AVAILABLE(); }
ପୂର୍ଣ୍ଣସଂଖ୍ୟା Device::nativeVectorWidthLong() ଅଚଳ { OCL_NOT_AVAILABLE(); }
ପୂର୍ଣ୍ଣସଂଖ୍ୟା Device::nativeVectorWidthFloat() ଅଚଳ { OCL_NOT_AVAILABLE(); }
ପୂର୍ଣ୍ଣସଂଖ୍ୟା Device::nativeVectorWidthDouble() ଅଚଳ { OCL_NOT_AVAILABLE(); }
ପୂର୍ଣ୍ଣସଂଖ୍ୟା Device::nativeVectorWidthHalf() ଅଚଳ { OCL_NOT_AVAILABLE(); }

ପୂର୍ଣ୍ଣସଂଖ୍ୟା Device::preferredVectorWidthChar() ଅଚଳ { OCL_NOT_AVAILABLE(); }
ପୂର୍ଣ୍ଣସଂଖ୍ୟା Device::preferredVectorWidthShort() ଅଚଳ { OCL_NOT_AVAILABLE(); }
ପୂର୍ଣ୍ଣସଂଖ୍ୟା Device::preferredVectorWidthInt() ଅଚଳ { OCL_NOT_AVAILABLE(); }
ପୂର୍ଣ୍ଣସଂଖ୍ୟା Device::preferredVectorWidthLong() ଅଚଳ { OCL_NOT_AVAILABLE(); }
ପୂର୍ଣ୍ଣସଂଖ୍ୟା Device::preferredVectorWidthFloat() ଅଚଳ { OCL_NOT_AVAILABLE(); }
ପୂର୍ଣ୍ଣସଂଖ୍ୟା Device::preferredVectorWidthDouble() ଅଚଳ { OCL_NOT_AVAILABLE(); }
ପୂର୍ଣ୍ଣସଂଖ୍ୟା Device::preferredVectorWidthHalf() ଅଚଳ { OCL_NOT_AVAILABLE(); }

size_t Device::printfBufferSize() ଅଚଳ { OCL_NOT_AVAILABLE(); }
size_t Device::profilingTimerResolution() ଅଚଳ { OCL_NOT_AVAILABLE(); }

/* static */
ଅଚଳ Device& Device::getDefault()
{
    ସ୍ଥିର Device dummy;
    ଫେରନ୍ତୁ dummy;
}

/* static */ Device Device::fromHandle(ଶୂନ୍ୟ* d) { OCL_NOT_AVAILABLE(); }


Context::Context() CV_NOEXCEPT : p(ଖାଲି) { }
Context::Context(ପୂର୍ଣ୍ଣସଂଖ୍ୟା dtype) : p(ଖାଲି) { }
Context::~Context() { }
Context::Context(ଅଚଳ Context& c) : p(ଖାଲି) { }
Context& Context::operator=(ଅଚଳ Context& c) { ଫେରନ୍ତୁ *ଏହି; }
Context::Context(Context&&) CV_NOEXCEPT : p(ଖାଲି) { }
Context& Context::operator=(Context&&) CV_NOEXCEPT { ଫେରନ୍ତୁ *ଏହି; }

ତାର୍କିକ Context::create() { ଫେରନ୍ତୁ ମିଥ୍ୟା; }
ତାର୍କିକ Context::create(ପୂର୍ଣ୍ଣସଂଖ୍ୟା dtype) { ଫେରନ୍ତୁ ମିଥ୍ୟା; }
size_t Context::ndevices() ଅଚଳ { ଫେରନ୍ତୁ 0; }
Device& Context::device(size_t idx) ଅଚଳ { OCL_NOT_AVAILABLE(); }
Program Context::getProg(ଅଚଳ ProgramSource& prog, ଅଚଳ String& buildopt, String& errmsg) { OCL_NOT_AVAILABLE(); }
ଶୂନ୍ୟ Context::unloadProg(Program& prog) { }

/* static */
Context& Context::getDefault()
{
    ସ୍ଥିର Context dummy;
    ଫେରନ୍ତୁ dummy;
}
ଶୂନ୍ୟ* Context::ptr() ଅଚଳ { ଫେରନ୍ତୁ ଖାଲି; }

ଶୂନ୍ୟ* Context::getOpenCLContextProperty(ପୂର୍ଣ୍ଣସଂଖ୍ୟା /*propertyId*/) ଅଚଳ { OCL_NOT_AVAILABLE(); }

ତାର୍କିକ Context::useSVM() ଅଚଳ { ଫେରନ୍ତୁ ମିଥ୍ୟା; }
ଶୂନ୍ୟ Context::setUseSVM(ତାର୍କିକ enabled) { }

Context::UserContext::~UserContext() { }

ଶୂନ୍ୟ Context::setUserContext(std::type_index /*typeId*/, ଅଚଳ std::shared_ptr<Context::UserContext>& /*userContext*/) { OCL_NOT_AVAILABLE(); }
std::shared_ptr<Context::UserContext> Context::getUserContext(std::type_index /*typeId*/) { OCL_NOT_AVAILABLE(); }

/* static */ Context Context::fromHandle(ଶୂନ୍ୟ* context) { OCL_NOT_AVAILABLE(); }
/* static */ Context Context::fromDevice(ଅଚଳ ocl::Device& device) { OCL_NOT_AVAILABLE(); }
/* static */ Context Context::create(ଅଚଳ std::string& configuration) { OCL_NOT_AVAILABLE(); }

ଶୂନ୍ୟ Context::release() { }


Platform::Platform() CV_NOEXCEPT : p(ଖାଲି) { }
Platform::~Platform() { }
Platform::Platform(ଅଚଳ Platform&) : p(ଖାଲି) { }
Platform& Platform::operator=(ଅଚଳ Platform&) { ଫେରନ୍ତୁ *ଏହି; }
Platform::Platform(Platform&&) CV_NOEXCEPT : p(ଖାଲି) { }
Platform& Platform::operator=(Platform&&) CV_NOEXCEPT { ଫେରନ୍ତୁ *ଏହି; }

ଶୂନ୍ୟ* Platform::ptr() ଅଚଳ { ଫେରନ୍ତୁ ଖାଲି; }

/* static */
Platform& Platform::getDefault()
{
    ସ୍ଥିର Platform dummy;
    ଫେରନ୍ତୁ dummy;
}

ଶୂନ୍ୟ attachContext(ଅଚଳ String& platformName, ଶୂନ୍ୟ* platformID, ଶୂନ୍ୟ* context, ଶୂନ୍ୟ* deviceID) { OCL_NOT_AVAILABLE(); }
ଶୂନ୍ୟ convertFromBuffer(ଶୂନ୍ୟ* cl_mem_buffer, size_t step, ପୂର୍ଣ୍ଣସଂଖ୍ୟା rows, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cols, ପୂର୍ଣ୍ଣସଂଖ୍ୟା type, UMat& dst) { OCL_NOT_AVAILABLE(); }
ଶୂନ୍ୟ convertFromImage(ଶୂନ୍ୟ* cl_mem_image, UMat& dst) { OCL_NOT_AVAILABLE(); }

ଶୂନ୍ୟ initializeContextFromHandle(Context& ctx, ଶୂନ୍ୟ* platform, ଶୂନ୍ୟ* context, ଶୂନ୍ୟ* device) { OCL_NOT_AVAILABLE(); }

Queue::Queue() CV_NOEXCEPT : p(ଖାଲି) { }
Queue::Queue(ଅଚଳ Context& c, ଅଚଳ Device& d) : p(ଖାଲି) { OCL_NOT_AVAILABLE(); }
Queue::~Queue() { }
Queue::Queue(ଅଚଳ Queue& q) {}
Queue& Queue::operator=(ଅଚଳ Queue& q) { ଫେରନ୍ତୁ *ଏହି; }
Queue::Queue(Queue&&) CV_NOEXCEPT : p(ଖାଲି) { }
Queue& Queue::operator=(Queue&&) CV_NOEXCEPT { ଫେରନ୍ତୁ *ଏହି; }

ତାର୍କିକ Queue::create(ଅଚଳ Context& c, ଅଚଳ Device& d) { OCL_NOT_AVAILABLE(); }
ଶୂନ୍ୟ Queue::finish() {}
ଶୂନ୍ୟ* Queue::ptr() ଅଚଳ { ଫେରନ୍ତୁ ଖାଲି; }
/* static */
Queue& Queue::getDefault()
{
    ସ୍ଥିର Queue dummy;
    ଫେରନ୍ତୁ dummy;
}

/// @brief Returns OpenCL command queue with enable profiling mode support
ଅଚଳ Queue& Queue::getProfilingQueue() ଅଚଳ { OCL_NOT_AVAILABLE(); }


KernelArg::KernelArg() CV_NOEXCEPT
    : flags(0), m(0), obj(0), sz(0), wscale(1), iwscale(1)
{
}

KernelArg::KernelArg(ପୂର୍ଣ୍ଣସଂଖ୍ୟା _flags, UMat* _m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା _wscale, ପୂର୍ଣ୍ଣସଂଖ୍ୟା _iwscale, ଅଚଳ ଶୂନ୍ୟ* _obj, size_t _sz)
    : flags(_flags), m(_m), obj(_obj), sz(_sz), wscale(_wscale), iwscale(_iwscale)
{
    OCL_NOT_AVAILABLE();
}

KernelArg KernelArg::Constant(ଅଚଳ Mat& m)
{
    OCL_NOT_AVAILABLE();
}


Kernel::Kernel() CV_NOEXCEPT : p(ଖାଲି) { }
Kernel::Kernel(ଅଚଳ ଅକ୍ଷର* kname, ଅଚଳ Program& prog) : p(ଖାଲି) { OCL_NOT_AVAILABLE(); }
Kernel::Kernel(ଅଚଳ ଅକ୍ଷର* kname, ଅଚଳ ProgramSource& prog, ଅଚଳ String& buildopts, String* errmsg) : p(ଖାଲି) { OCL_NOT_AVAILABLE(); }
Kernel::~Kernel() { }
Kernel::Kernel(ଅଚଳ Kernel& k) : p(ଖାଲି) { }
Kernel& Kernel::operator=(ଅଚଳ Kernel& k) { ଫେରନ୍ତୁ *ଏହି; }
Kernel::Kernel(Kernel&&) CV_NOEXCEPT : p(ଖାଲି) { }
Kernel& Kernel::operator=(Kernel&&) CV_NOEXCEPT { ଫେରନ୍ତୁ *ଏହି; }

ତାର୍କିକ Kernel::empty() ଅଚଳ { ଫେରନ୍ତୁ ସତ୍ୟ; }
ତାର୍କିକ Kernel::create(ଅଚଳ ଅକ୍ଷର* kname, ଅଚଳ Program& prog) { OCL_NOT_AVAILABLE(); }
ତାର୍କିକ Kernel::create(ଅଚଳ ଅକ୍ଷର* kname, ଅଚଳ ProgramSource& prog, ଅଚଳ String& buildopts, String* errmsg) { OCL_NOT_AVAILABLE(); }

ପୂର୍ଣ୍ଣସଂଖ୍ୟା Kernel::set(ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, ଅଚଳ ଶୂନ୍ୟ* value, size_t sz) { OCL_NOT_AVAILABLE(); }
ପୂର୍ଣ୍ଣସଂଖ୍ୟା Kernel::set(ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, ଅଚଳ Image2D& image2D) { OCL_NOT_AVAILABLE(); }
ପୂର୍ଣ୍ଣସଂଖ୍ୟା Kernel::set(ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, ଅଚଳ UMat& m) { OCL_NOT_AVAILABLE(); }
ପୂର୍ଣ୍ଣସଂଖ୍ୟା Kernel::set(ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, ଅଚଳ KernelArg& arg) { OCL_NOT_AVAILABLE(); }

ତାର୍କିକ Kernel::run(ପୂର୍ଣ୍ଣସଂଖ୍ୟା dims, size_t globalsize[], size_t localsize[], ତାର୍କିକ ସମକାଳ, ଅଚଳ Queue& q) { OCL_NOT_AVAILABLE(); }
ତାର୍କିକ Kernel::runTask(ତାର୍କିକ ସମକାଳ, ଅଚଳ Queue& q) { OCL_NOT_AVAILABLE(); }

int64 Kernel::runProfiling(ପୂର୍ଣ୍ଣସଂଖ୍ୟା dims, size_t globalsize[], size_t localsize[], ଅଚଳ Queue& q) { OCL_NOT_AVAILABLE(); }

size_t Kernel::workGroupSize() ଅଚଳ { OCL_NOT_AVAILABLE(); }
size_t Kernel::preferedWorkGroupSizeMultiple() ଅଚଳ { OCL_NOT_AVAILABLE(); }
ତାର୍କିକ Kernel::compileWorkGroupSize(size_t wsz[]) ଅଚଳ { OCL_NOT_AVAILABLE(); }
size_t Kernel::localMemSize() ଅଚଳ { OCL_NOT_AVAILABLE(); }

ଶୂନ୍ୟ* Kernel::ptr() ଅଚଳ { ଫେରନ୍ତୁ ଖାଲି; }


Program::Program() CV_NOEXCEPT : p(ଖାଲି) { }
Program::Program(ଅଚଳ ProgramSource& src, ଅଚଳ String& buildflags, String& errmsg) : p(ଖାଲି) { OCL_NOT_AVAILABLE(); }
Program::Program(ଅଚଳ Program& prog) : p(ଖାଲି) { }
Program& Program::operator=(ଅଚଳ Program& prog) { ଫେରନ୍ତୁ *ଏହି; }
Program::Program(Program&&) CV_NOEXCEPT : p(ଖାଲି) { }
Program& Program::operator=(Program&&) CV_NOEXCEPT { ଫେରନ୍ତୁ *ଏହି; }
Program::~Program() { }

ତାର୍କିକ Program::create(ଅଚଳ ProgramSource& src, ଅଚଳ String& buildflags, String& errmsg) { OCL_NOT_AVAILABLE(); }

ଶୂନ୍ୟ* Program::ptr() ଅଚଳ { ଫେରନ୍ତୁ ଖାଲି; }

ଶୂନ୍ୟ Program::get_ଦ୍ୱିମିକ(std::vector<ଅକ୍ଷର>& binary) ଅଚଳ { OCL_NOT_AVAILABLE(); }

ତାର୍କିକ Program::ପଠ(ଅଚଳ String& buf, ଅଚଳ String& buildflags) { OCL_NOT_AVAILABLE(); }
ତାର୍କିକ Program::ଲିଖ(String& buf) ଅଚଳ { OCL_NOT_AVAILABLE(); }
ଅଚଳ ProgramSource& Program::source() ଅଚଳ { OCL_NOT_AVAILABLE(); }
String Program::getPrefix() ଅଚଳ { OCL_NOT_AVAILABLE(); }
/* static */ String Program::getPrefix(ଅଚଳ String& buildflags) { OCL_NOT_AVAILABLE(); }


ProgramSource::ProgramSource() CV_NOEXCEPT : p(ଖାଲି) { }
ProgramSource::ProgramSource(ଅଚଳ String& module, ଅଚଳ String& name, ଅଚଳ String& codeStr, ଅଚଳ String& codeHash) : p(ଖାଲି) { }
ProgramSource::ProgramSource(ଅଚଳ String& prog) : p(ଖାଲି) { }
ProgramSource::ProgramSource(ଅଚଳ ଅକ୍ଷର* prog) : p(ଖାଲି) { }
ProgramSource::~ProgramSource() { }
ProgramSource::ProgramSource(ଅଚଳ ProgramSource& prog) : p(ଖାଲି) { }
ProgramSource& ProgramSource::operator=(ଅଚଳ ProgramSource& prog) { ଫେରନ୍ତୁ *ଏହି; }
ProgramSource::ProgramSource(ProgramSource&&) CV_NOEXCEPT : p(ଖାଲି) { }
ProgramSource& ProgramSource::operator=(ProgramSource&&) CV_NOEXCEPT { ଫେରନ୍ତୁ *ଏହି; }

ଅଚଳ String& ProgramSource::source() ଅଚଳ { OCL_NOT_AVAILABLE(); }
ProgramSource::hash_t ProgramSource::hash() ଅଚଳ { OCL_NOT_AVAILABLE(); }

/* static */ ProgramSource ProgramSource::from_ଦ୍ୱିମିକ(ଅଚଳ String& module, ଅଚଳ String& name, ଅଚଳ unsigned ଅକ୍ଷର* binary, ଅଚଳ size_t size, ଅଚଳ cv::String& buildOptions) { OCL_NOT_AVAILABLE(); }
/* static */ ProgramSource ProgramSource::fromSPIR(ଅଚଳ String& module, ଅଚଳ String& name, ଅଚଳ unsigned ଅକ୍ଷର* binary, ଅଚଳ size_t size, ଅଚଳ cv::String& buildOptions) { OCL_NOT_AVAILABLE(); }


PlatformInfo::PlatformInfo() CV_NOEXCEPT : p(ଖାଲି) { }
PlatformInfo::PlatformInfo(ଶୂନ୍ୟ* id) : p(ଖାଲି) { OCL_NOT_AVAILABLE(); }
PlatformInfo::~PlatformInfo() { }

PlatformInfo::PlatformInfo(ଅଚଳ PlatformInfo& i) : p(ଖାଲି) { }
PlatformInfo& PlatformInfo::operator=(ଅଚଳ PlatformInfo& i) { ଫେରନ୍ତୁ *ଏହି; }
PlatformInfo::PlatformInfo(PlatformInfo&&) CV_NOEXCEPT : p(ଖାଲି) { }
PlatformInfo& PlatformInfo::operator=(PlatformInfo&&) CV_NOEXCEPT { ଫେରନ୍ତୁ *ଏହି; }

String PlatformInfo::name() ଅଚଳ { OCL_NOT_AVAILABLE(); }
String PlatformInfo::vendor() ଅଚଳ { OCL_NOT_AVAILABLE(); }
String PlatformInfo::version() ଅଚଳ { OCL_NOT_AVAILABLE(); }
ପୂର୍ଣ୍ଣସଂଖ୍ୟା PlatformInfo::device_ସଂଖ୍ୟା() ଅଚଳ { OCL_NOT_AVAILABLE(); }
ଶୂନ୍ୟ PlatformInfo::getDevice(Device& device, ପୂର୍ଣ୍ଣସଂଖ୍ୟା d) ଅଚଳ { OCL_NOT_AVAILABLE(); }

ଅଚଳ ଅକ୍ଷର* convertTypeStr(ପୂର୍ଣ୍ଣସଂଖ୍ୟା sdepth, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ddepth, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn, ଅକ୍ଷର* buf) { OCL_NOT_AVAILABLE(); }
ଅଚଳ ଅକ୍ଷର* typeToStr(ପୂର୍ଣ୍ଣସଂଖ୍ୟା t) { OCL_NOT_AVAILABLE(); }
ଅଚଳ ଅକ୍ଷର* memopTypeToStr(ପୂର୍ଣ୍ଣସଂଖ୍ୟା t) { OCL_NOT_AVAILABLE(); }
ଅଚଳ ଅକ୍ଷର* vecopTypeToStr(ପୂର୍ଣ୍ଣସଂଖ୍ୟା t) { OCL_NOT_AVAILABLE(); }
ଅଚଳ ଅକ୍ଷର* getOpenCLErrorString(ପୂର୍ଣ୍ଣସଂଖ୍ୟା errorCode) { OCL_NOT_AVAILABLE(); }
String kernelToStr(ନିବେଶ_Array _kernel, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ddepth, ଅଚଳ ଅକ୍ଷର* name) { OCL_NOT_AVAILABLE(); }
ଶୂନ୍ୟ getPlatfomsInfo(std::vector<PlatformInfo>& platform_info) { OCL_NOT_AVAILABLE(); }


ପୂର୍ଣ୍ଣସଂଖ୍ୟା predictOptimalVectorWidth(ନିବେଶ_Array src1, ନିବେଶ_Array src2, ନିବେଶ_Array src3,
        ନିବେଶ_Array src4, ନିବେଶ_Array src5, ନିବେଶ_Array src6,
        ନିବେଶ_Array src7, ନିବେଶ_Array src8, ନିବେଶ_Array src9,
        OclVectorStrategy strat)
{ OCL_NOT_AVAILABLE(); }

ପୂର୍ଣ୍ଣସଂଖ୍ୟା checkOptimalVectorWidth(ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା *vectorWidths,
        ନିବେଶ_Array src1, ନିବେଶ_Array src2, ନିବେଶ_Array src3,
        ନିବେଶ_Array src4, ନିବେଶ_Array src5, ନିବେଶ_Array src6,
        ନିବେଶ_Array src7, ନିବେଶ_Array src8, ନିବେଶ_Array src9,
        OclVectorStrategy strat)
{ OCL_NOT_AVAILABLE(); }

ପୂର୍ଣ୍ଣସଂଖ୍ୟା predictOptimalVectorWidthMax(ନିବେଶ_Array src1, ନିବେଶ_Array src2, ନିବେଶ_Array src3,
        ନିବେଶ_Array src4, ନିବେଶ_Array src5, ନିବେଶ_Array src6,
        ନିବେଶ_Array src7, ନିବେଶ_Array src8, ନିବେଶ_Array src9)
{ OCL_NOT_AVAILABLE(); }

ଶୂନ୍ୟ buildOptionsAddMatrixDescription(String& buildOptions, ଅଚଳ String& name, ନିବେଶ_Array _m) { OCL_NOT_AVAILABLE(); }


Image2D::Image2D() CV_NOEXCEPT : p(ଖାଲି) { }
Image2D::Image2D(ଅଚଳ UMat &src, ତାର୍କିକ norm, ତାର୍କିକ alias) { OCL_NOT_AVAILABLE(); }
Image2D::Image2D(ଅଚଳ Image2D & i) : p(ଖାଲି) { OCL_NOT_AVAILABLE(); }
Image2D::~Image2D() { }
Image2D& Image2D::operator=(ଅଚଳ Image2D & i) { ଫେରନ୍ତୁ *ଏହି; }
Image2D::Image2D(Image2D&&) CV_NOEXCEPT : p(ଖାଲି) { }
Image2D& Image2D::operator=(Image2D&&) CV_NOEXCEPT { ଫେରନ୍ତୁ *ଏହି; }

/* static */ ତାର୍କିକ Image2D::canCreateAlias(ଅଚଳ UMat &u) { OCL_NOT_AVAILABLE(); }
/* static */ ତାର୍କିକ Image2D::isFormatSupported(ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn, ତାର୍କିକ norm) { OCL_NOT_AVAILABLE(); }

ଶୂନ୍ୟ* Image2D::ptr() ଅଚଳ { ଫେରନ୍ତୁ ଖାଲି; }


Timer::Timer(ଅଚଳ Queue& q) : p(ଖାଲି) {}
Timer::~Timer() {}
ଶୂନ୍ୟ Timer::start() { OCL_NOT_AVAILABLE(); }
ଶୂନ୍ୟ Timer::stop() { OCL_NOT_AVAILABLE();}

uint64 Timer::durationNS() ଅଚଳ { OCL_NOT_AVAILABLE(); }

MatAllocator* getOpenCLAllocator() { ଫେରନ୍ତୁ ଖାଲି; }

internal::ProgramEntry::operator ProgramSource&() ଅଚଳ { OCL_NOT_AVAILABLE(); }


ସଂରଚନା OpenCLExecutionContext::Impl
{
    Impl() = ପୂର୍ବନିର୍ଦ୍ଧାରିତ;
};

Context& OpenCLExecutionContext::getContext() ଅଚଳ { OCL_NOT_AVAILABLE(); }
Device& OpenCLExecutionContext::getDevice() ଅଚଳ { OCL_NOT_AVAILABLE(); }
Queue& OpenCLExecutionContext::getQueue() ଅଚଳ { OCL_NOT_AVAILABLE(); }

ତାର୍କିକ OpenCLExecutionContext::useOpenCL() ଅଚଳ { ଫେରନ୍ତୁ ମିଥ୍ୟା; }
ଶୂନ୍ୟ OpenCLExecutionContext::setUseOpenCL(ତାର୍କିକ flag) { }

ସ୍ଥିର
OpenCLExecutionContext& getDummyOpenCLExecutionContext()
{
    ସ୍ଥିର OpenCLExecutionContext dummy;
    ଫେରନ୍ତୁ dummy;
}

/* static */
OpenCLExecutionContext& OpenCLExecutionContext::getCurrent() { ଫେରନ୍ତୁ getDummyOpenCLExecutionContext(); }

/* static */
OpenCLExecutionContext& OpenCLExecutionContext::getCurrentRef() { ଫେରନ୍ତୁ getDummyOpenCLExecutionContext(); }

ଶୂନ୍ୟ OpenCLExecutionContext::bind() ଅଚଳ { OCL_NOT_AVAILABLE(); }

OpenCLExecutionContext OpenCLExecutionContext::cloneWithNewQueue(ଅଚଳ ocl::Queue& q) ଅଚଳ { OCL_NOT_AVAILABLE(); }
OpenCLExecutionContext OpenCLExecutionContext::cloneWithNewQueue() ଅଚଳ { OCL_NOT_AVAILABLE(); }

/* static */ OpenCLExecutionContext OpenCLExecutionContext::create(ଅଚଳ std::string& platformName, ଶୂନ୍ୟ* platformID, ଶୂନ୍ୟ* context, ଶୂନ୍ୟ* deviceID) { OCL_NOT_AVAILABLE(); }
/* static */ OpenCLExecutionContext OpenCLExecutionContext::create(ଅଚଳ Context& context, ଅଚଳ Device& device, ଅଚଳ ocl::Queue& queue) { OCL_NOT_AVAILABLE(); }
/* static */ OpenCLExecutionContext OpenCLExecutionContext::create(ଅଚଳ Context& context, ଅଚଳ Device& device) { OCL_NOT_AVAILABLE(); }

ଶୂନ୍ୟ OpenCLExecutionContext::release() { }

}}

#ଯଦି defined(_MSC_VER)
    #pragma warning(pop)
#ନଚେତଯଦି defined(__clang__)
    #pragma clang diagnostic pop
#ନଚେତଯଦି defined(__GNUC__)
    #pragma GCC diagnostic pop
#endif
