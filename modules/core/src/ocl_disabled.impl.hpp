// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#അന്തര്ഭാവ "opencv2/core/ocl_genbase.hpp"

#എങ്കിൽ defined(_MSC_VER)
    #pragma warning(push)
    #pragma warning(disable : 4100)
    #pragma warning(disable : 4702)
#അല്ലെങ്കിലെങ്കിൽ defined(__clang__)
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wunused-parameter"
#അല്ലെങ്കിലെങ്കിൽ defined(__GNUC__)
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

നാമാകാശ cv { നാമാകാശ ocl {

സ്ഥിതം
CV_NORETURN ശൂന്യം throw_no_ocl()
{
    CV_Error(Error::OpenCLApiCallError, "OpenCV build without OpenCL support");
}
#define OCL_NOT_AVAILABLE() throw_no_ocl();

CV_EXPORTS_W തർക്കം haveOpenCL() { തിരികെ അസത്യം; }
CV_EXPORTS_W തർക്കം useOpenCL() { തിരികെ അസത്യം; }
CV_EXPORTS_W തർക്കം haveAmdBlas() { തിരികെ അസത്യം; }
CV_EXPORTS_W തർക്കം haveAmdFft() { തിരികെ അസത്യം; }
CV_EXPORTS_W ശൂന്യം setUseOpenCL(തർക്കം flag) { /* nothing */ }
CV_EXPORTS_W ശൂന്യം finish() { /* nothing */ }

CV_EXPORTS തർക്കം haveSVM() { തിരികെ അസത്യം; }

Device::Device() CV_NOEXCEPT : p(ശൂന്യ) { }
Device::Device(ശൂന്യം* d) : p(ശൂന്യ) { OCL_NOT_AVAILABLE(); }
Device::Device(സ്ഥിരം Device& d) : p(ശൂന്യ) { }
Device& Device::operator=(സ്ഥിരം Device& d) { തിരികെ *ഇത്; }
Device::Device(Device&&) CV_NOEXCEPT : p(ശൂന്യ) { }
Device& Device::operator=(Device&&) CV_NOEXCEPT { തിരികെ *ഇത്; }
Device::~Device() { }

ശൂന്യം Device::set(ശൂന്യം* d) { OCL_NOT_AVAILABLE(); }

String Device::name() സ്ഥിരം { OCL_NOT_AVAILABLE(); }
String Device::extensions() സ്ഥിരം { OCL_NOT_AVAILABLE(); }
തർക്കം Device::isExtensionSupported(സ്ഥിരം String& extensionName) സ്ഥിരം { OCL_NOT_AVAILABLE(); }
String Device::version() സ്ഥിരം { OCL_NOT_AVAILABLE(); }
String Device::vendorName() സ്ഥിരം { OCL_NOT_AVAILABLE(); }
String Device::OpenCL_C_Version() സ്ഥിരം { OCL_NOT_AVAILABLE(); }
String Device::OpenCLVersion() സ്ഥിരം { OCL_NOT_AVAILABLE(); }
പൂർണ്ണസംഖ്യ Device::deviceVersionMajor() സ്ഥിരം { OCL_NOT_AVAILABLE(); }
പൂർണ്ണസംഖ്യ Device::deviceVersionMinor() സ്ഥിരം { OCL_NOT_AVAILABLE(); }
String Device::driverVersion() സ്ഥിരം { OCL_NOT_AVAILABLE(); }
ശൂന്യം* Device::ptr() സ്ഥിരം { /*OCL_NOT_AVAILABLE();*/ തിരികെ ശൂന്യ; }

പൂർണ്ണസംഖ്യ Device::type() സ്ഥിരം { OCL_NOT_AVAILABLE(); }

പൂർണ്ണസംഖ്യ Device::addressBits() സ്ഥിരം { OCL_NOT_AVAILABLE(); }
തർക്കം Device::available() സ്ഥിരം { OCL_NOT_AVAILABLE(); }
തർക്കം Device::compilerAvailable() സ്ഥിരം { OCL_NOT_AVAILABLE(); }
തർക്കം Device::linkerAvailable() സ്ഥിരം { OCL_NOT_AVAILABLE(); }

പൂർണ്ണസംഖ്യ Device::doubleFPConfig() സ്ഥിരം { OCL_NOT_AVAILABLE(); }
പൂർണ്ണസംഖ്യ Device::singleFPConfig() സ്ഥിരം { OCL_NOT_AVAILABLE(); }
പൂർണ്ണസംഖ്യ Device::halfFPConfig() സ്ഥിരം { OCL_NOT_AVAILABLE(); }

തർക്കം Device::hasFP64() സ്ഥിരം { OCL_NOT_AVAILABLE(); }
തർക്കം Device::hasFP16() സ്ഥിരം { OCL_NOT_AVAILABLE(); }

തർക്കം Device::endianLittle() സ്ഥിരം { OCL_NOT_AVAILABLE(); }
തർക്കം Device::errorCorrectionSupport() സ്ഥിരം { OCL_NOT_AVAILABLE(); }

പൂർണ്ണസംഖ്യ Device::executionCapabilities() സ്ഥിരം { OCL_NOT_AVAILABLE(); }

size_t Device::globalMemCacheSize() സ്ഥിരം { OCL_NOT_AVAILABLE(); }

പൂർണ്ണസംഖ്യ Device::globalMemCacheType() സ്ഥിരം { OCL_NOT_AVAILABLE(); }
പൂർണ്ണസംഖ്യ Device::global_Mem_Cache_വരി_Size() സ്ഥിരം { OCL_NOT_AVAILABLE(); }
size_t Device::globalMemSize() സ്ഥിരം { OCL_NOT_AVAILABLE(); }

size_t Device::localMemSize() സ്ഥിരം { OCL_NOT_AVAILABLE(); }
പൂർണ്ണസംഖ്യ Device::localMemType() സ്ഥിരം { തിരികെ NO_LOCAL_MEM; }
തർക്കം Device::hostUnifiedMemory() സ്ഥിരം { OCL_NOT_AVAILABLE(); }

തർക്കം Device::imageSupport() സ്ഥിരം { OCL_NOT_AVAILABLE(); }

തർക്കം Device::imageFromBufferSupport() സ്ഥിരം { OCL_NOT_AVAILABLE(); }
uint Device::imagePitchAlignment() സ്ഥിരം { OCL_NOT_AVAILABLE(); }
uint Device::imageBaseAddressAlignment() സ്ഥിരം { OCL_NOT_AVAILABLE(); }

തർക്കം Device::intelSubgroupsSupport() സ്ഥിരം { OCL_NOT_AVAILABLE(); }

size_t Device::image2DMaxWidth() സ്ഥിരം { OCL_NOT_AVAILABLE(); }
size_t Device::image2DMaxHeight() സ്ഥിരം { OCL_NOT_AVAILABLE(); }

size_t Device::image3DMaxWidth() സ്ഥിരം { OCL_NOT_AVAILABLE(); }
size_t Device::image3DMaxHeight() സ്ഥിരം { OCL_NOT_AVAILABLE(); }
size_t Device::image3DMaxDepth() സ്ഥിരം { OCL_NOT_AVAILABLE(); }

size_t Device::imageMaxBufferSize() സ്ഥിരം { OCL_NOT_AVAILABLE(); }
size_t Device::imageMaxArraySize() സ്ഥിരം { OCL_NOT_AVAILABLE(); }

പൂർണ്ണസംഖ്യ Device::vendorID() സ്ഥിരം { OCL_NOT_AVAILABLE(); }

പൂർണ്ണസംഖ്യ Device::maxClockFrequency() സ്ഥിരം { OCL_NOT_AVAILABLE(); }
പൂർണ്ണസംഖ്യ Device::maxComputeUnits() സ്ഥിരം { OCL_NOT_AVAILABLE(); }
പൂർണ്ണസംഖ്യ Device::maxConstantArgs() സ്ഥിരം { OCL_NOT_AVAILABLE(); }
size_t Device::maxConstantBufferSize() സ്ഥിരം { OCL_NOT_AVAILABLE(); }

size_t Device::maxMemAllocSize() സ്ഥിരം { OCL_NOT_AVAILABLE(); }
size_t Device::maxParameterSize() സ്ഥിരം { OCL_NOT_AVAILABLE(); }

പൂർണ്ണസംഖ്യ Device::maxReadImageArgs() സ്ഥിരം { OCL_NOT_AVAILABLE(); }
പൂർണ്ണസംഖ്യ Device::maxWriteImageArgs() സ്ഥിരം { OCL_NOT_AVAILABLE(); }
പൂർണ്ണസംഖ്യ Device::maxSamplers() സ്ഥിരം { OCL_NOT_AVAILABLE(); }

size_t Device::maxWorkGroupSize() സ്ഥിരം { OCL_NOT_AVAILABLE(); }
പൂർണ്ണസംഖ്യ Device::maxWorkItemDims() സ്ഥിരം { OCL_NOT_AVAILABLE(); }
ശൂന്യം Device::maxWorkItemSizes(size_t*) സ്ഥിരം { OCL_NOT_AVAILABLE(); }

പൂർണ്ണസംഖ്യ Device::memBaseAddrAlign() സ്ഥിരം { OCL_NOT_AVAILABLE(); }

പൂർണ്ണസംഖ്യ Device::nativeVectorWidthChar() സ്ഥിരം { OCL_NOT_AVAILABLE(); }
പൂർണ്ണസംഖ്യ Device::nativeVectorWidthShort() സ്ഥിരം { OCL_NOT_AVAILABLE(); }
പൂർണ്ണസംഖ്യ Device::nativeVectorWidthInt() സ്ഥിരം { OCL_NOT_AVAILABLE(); }
പൂർണ്ണസംഖ്യ Device::nativeVectorWidthLong() സ്ഥിരം { OCL_NOT_AVAILABLE(); }
പൂർണ്ണസംഖ്യ Device::nativeVectorWidthFloat() സ്ഥിരം { OCL_NOT_AVAILABLE(); }
പൂർണ്ണസംഖ്യ Device::nativeVectorWidthDouble() സ്ഥിരം { OCL_NOT_AVAILABLE(); }
പൂർണ്ണസംഖ്യ Device::nativeVectorWidthHalf() സ്ഥിരം { OCL_NOT_AVAILABLE(); }

പൂർണ്ണസംഖ്യ Device::preferredVectorWidthChar() സ്ഥിരം { OCL_NOT_AVAILABLE(); }
പൂർണ്ണസംഖ്യ Device::preferredVectorWidthShort() സ്ഥിരം { OCL_NOT_AVAILABLE(); }
പൂർണ്ണസംഖ്യ Device::preferredVectorWidthInt() സ്ഥിരം { OCL_NOT_AVAILABLE(); }
പൂർണ്ണസംഖ്യ Device::preferredVectorWidthLong() സ്ഥിരം { OCL_NOT_AVAILABLE(); }
പൂർണ്ണസംഖ്യ Device::preferredVectorWidthFloat() സ്ഥിരം { OCL_NOT_AVAILABLE(); }
പൂർണ്ണസംഖ്യ Device::preferredVectorWidthDouble() സ്ഥിരം { OCL_NOT_AVAILABLE(); }
പൂർണ്ണസംഖ്യ Device::preferredVectorWidthHalf() സ്ഥിരം { OCL_NOT_AVAILABLE(); }

size_t Device::printfBufferSize() സ്ഥിരം { OCL_NOT_AVAILABLE(); }
size_t Device::profilingTimerResolution() സ്ഥിരം { OCL_NOT_AVAILABLE(); }

/* static */
സ്ഥിരം Device& Device::getDefault()
{
    സ്ഥിതം Device dummy;
    തിരികെ dummy;
}

/* static */ Device Device::fromHandle(ശൂന്യം* d) { OCL_NOT_AVAILABLE(); }


Context::Context() CV_NOEXCEPT : p(ശൂന്യ) { }
Context::Context(പൂർണ്ണസംഖ്യ dtype) : p(ശൂന്യ) { }
Context::~Context() { }
Context::Context(സ്ഥിരം Context& c) : p(ശൂന്യ) { }
Context& Context::operator=(സ്ഥിരം Context& c) { തിരികെ *ഇത്; }
Context::Context(Context&&) CV_NOEXCEPT : p(ശൂന്യ) { }
Context& Context::operator=(Context&&) CV_NOEXCEPT { തിരികെ *ഇത്; }

തർക്കം Context::create() { തിരികെ അസത്യം; }
തർക്കം Context::create(പൂർണ്ണസംഖ്യ dtype) { തിരികെ അസത്യം; }
size_t Context::ndevices() സ്ഥിരം { തിരികെ 0; }
Device& Context::device(size_t idx) സ്ഥിരം { OCL_NOT_AVAILABLE(); }
Program Context::getProg(സ്ഥിരം ProgramSource& prog, സ്ഥിരം String& buildopt, String& errmsg) { OCL_NOT_AVAILABLE(); }
ശൂന്യം Context::unloadProg(Program& prog) { }

/* static */
Context& Context::getDefault()
{
    സ്ഥിതം Context dummy;
    തിരികെ dummy;
}
ശൂന്യം* Context::ptr() സ്ഥിരം { തിരികെ ശൂന്യ; }

ശൂന്യം* Context::getOpenCLContextProperty(പൂർണ്ണസംഖ്യ /*propertyId*/) സ്ഥിരം { OCL_NOT_AVAILABLE(); }

തർക്കം Context::useSVM() സ്ഥിരം { തിരികെ അസത്യം; }
ശൂന്യം Context::setUseSVM(തർക്കം enabled) { }

Context::UserContext::~UserContext() { }

ശൂന്യം Context::setUserContext(std::type_index /*typeId*/, സ്ഥിരം std::shared_ptr<Context::UserContext>& /*userContext*/) { OCL_NOT_AVAILABLE(); }
std::shared_ptr<Context::UserContext> Context::getUserContext(std::type_index /*typeId*/) { OCL_NOT_AVAILABLE(); }

/* static */ Context Context::fromHandle(ശൂന്യം* context) { OCL_NOT_AVAILABLE(); }
/* static */ Context Context::fromDevice(സ്ഥിരം ocl::Device& device) { OCL_NOT_AVAILABLE(); }
/* static */ Context Context::create(സ്ഥിരം std::string& configuration) { OCL_NOT_AVAILABLE(); }

ശൂന്യം Context::release() { }


Platform::Platform() CV_NOEXCEPT : p(ശൂന്യ) { }
Platform::~Platform() { }
Platform::Platform(സ്ഥിരം Platform&) : p(ശൂന്യ) { }
Platform& Platform::operator=(സ്ഥിരം Platform&) { തിരികെ *ഇത്; }
Platform::Platform(Platform&&) CV_NOEXCEPT : p(ശൂന്യ) { }
Platform& Platform::operator=(Platform&&) CV_NOEXCEPT { തിരികെ *ഇത്; }

ശൂന്യം* Platform::ptr() സ്ഥിരം { തിരികെ ശൂന്യ; }

/* static */
Platform& Platform::getDefault()
{
    സ്ഥിതം Platform dummy;
    തിരികെ dummy;
}

ശൂന്യം attachContext(സ്ഥിരം String& platformName, ശൂന്യം* platformID, ശൂന്യം* context, ശൂന്യം* deviceID) { OCL_NOT_AVAILABLE(); }
ശൂന്യം convertFromBuffer(ശൂന്യം* cl_mem_buffer, size_t step, പൂർണ്ണസംഖ്യ rows, പൂർണ്ണസംഖ്യ cols, പൂർണ്ണസംഖ്യ type, UMat& dst) { OCL_NOT_AVAILABLE(); }
ശൂന്യം convertFromImage(ശൂന്യം* cl_mem_image, UMat& dst) { OCL_NOT_AVAILABLE(); }

ശൂന്യം initializeContextFromHandle(Context& ctx, ശൂന്യം* platform, ശൂന്യം* context, ശൂന്യം* device) { OCL_NOT_AVAILABLE(); }

Queue::Queue() CV_NOEXCEPT : p(ശൂന്യ) { }
Queue::Queue(സ്ഥിരം Context& c, സ്ഥിരം Device& d) : p(ശൂന്യ) { OCL_NOT_AVAILABLE(); }
Queue::~Queue() { }
Queue::Queue(സ്ഥിരം Queue& q) {}
Queue& Queue::operator=(സ്ഥിരം Queue& q) { തിരികെ *ഇത്; }
Queue::Queue(Queue&&) CV_NOEXCEPT : p(ശൂന്യ) { }
Queue& Queue::operator=(Queue&&) CV_NOEXCEPT { തിരികെ *ഇത്; }

തർക്കം Queue::create(സ്ഥിരം Context& c, സ്ഥിരം Device& d) { OCL_NOT_AVAILABLE(); }
ശൂന്യം Queue::finish() {}
ശൂന്യം* Queue::ptr() സ്ഥിരം { തിരികെ ശൂന്യ; }
/* static */
Queue& Queue::getDefault()
{
    സ്ഥിതം Queue dummy;
    തിരികെ dummy;
}

/// @brief Returns OpenCL command queue with enable profiling mode support
സ്ഥിരം Queue& Queue::getProfilingQueue() സ്ഥിരം { OCL_NOT_AVAILABLE(); }


KernelArg::KernelArg() CV_NOEXCEPT
    : flags(0), m(0), obj(0), sz(0), wscale(1), iwscale(1)
{
}

KernelArg::KernelArg(പൂർണ്ണസംഖ്യ _flags, UMat* _m, പൂർണ്ണസംഖ്യ _wscale, പൂർണ്ണസംഖ്യ _iwscale, സ്ഥിരം ശൂന്യം* _obj, size_t _sz)
    : flags(_flags), m(_m), obj(_obj), sz(_sz), wscale(_wscale), iwscale(_iwscale)
{
    OCL_NOT_AVAILABLE();
}

KernelArg KernelArg::Constant(സ്ഥിരം Mat& m)
{
    OCL_NOT_AVAILABLE();
}


Kernel::Kernel() CV_NOEXCEPT : p(ശൂന്യ) { }
Kernel::Kernel(സ്ഥിരം അക്ഷരം* kname, സ്ഥിരം Program& prog) : p(ശൂന്യ) { OCL_NOT_AVAILABLE(); }
Kernel::Kernel(സ്ഥിരം അക്ഷരം* kname, സ്ഥിരം ProgramSource& prog, സ്ഥിരം String& buildopts, String* errmsg) : p(ശൂന്യ) { OCL_NOT_AVAILABLE(); }
Kernel::~Kernel() { }
Kernel::Kernel(സ്ഥിരം Kernel& k) : p(ശൂന്യ) { }
Kernel& Kernel::operator=(സ്ഥിരം Kernel& k) { തിരികെ *ഇത്; }
Kernel::Kernel(Kernel&&) CV_NOEXCEPT : p(ശൂന്യ) { }
Kernel& Kernel::operator=(Kernel&&) CV_NOEXCEPT { തിരികെ *ഇത്; }

തർക്കം Kernel::empty() സ്ഥിരം { തിരികെ സത്യം; }
തർക്കം Kernel::create(സ്ഥിരം അക്ഷരം* kname, സ്ഥിരം Program& prog) { OCL_NOT_AVAILABLE(); }
തർക്കം Kernel::create(സ്ഥിരം അക്ഷരം* kname, സ്ഥിരം ProgramSource& prog, സ്ഥിരം String& buildopts, String* errmsg) { OCL_NOT_AVAILABLE(); }

പൂർണ്ണസംഖ്യ Kernel::set(പൂർണ്ണസംഖ്യ i, സ്ഥിരം ശൂന്യം* value, size_t sz) { OCL_NOT_AVAILABLE(); }
പൂർണ്ണസംഖ്യ Kernel::set(പൂർണ്ണസംഖ്യ i, സ്ഥിരം Image2D& image2D) { OCL_NOT_AVAILABLE(); }
പൂർണ്ണസംഖ്യ Kernel::set(പൂർണ്ണസംഖ്യ i, സ്ഥിരം UMat& m) { OCL_NOT_AVAILABLE(); }
പൂർണ്ണസംഖ്യ Kernel::set(പൂർണ്ണസംഖ്യ i, സ്ഥിരം KernelArg& arg) { OCL_NOT_AVAILABLE(); }

തർക്കം Kernel::run(പൂർണ്ണസംഖ്യ dims, size_t globalsize[], size_t localsize[], തർക്കം സമന്വയം, സ്ഥിരം Queue& q) { OCL_NOT_AVAILABLE(); }
തർക്കം Kernel::runTask(തർക്കം സമന്വയം, സ്ഥിരം Queue& q) { OCL_NOT_AVAILABLE(); }

int64 Kernel::runProfiling(പൂർണ്ണസംഖ്യ dims, size_t globalsize[], size_t localsize[], സ്ഥിരം Queue& q) { OCL_NOT_AVAILABLE(); }

size_t Kernel::workGroupSize() സ്ഥിരം { OCL_NOT_AVAILABLE(); }
size_t Kernel::preferedWorkGroupSizeMultiple() സ്ഥിരം { OCL_NOT_AVAILABLE(); }
തർക്കം Kernel::compileWorkGroupSize(size_t wsz[]) സ്ഥിരം { OCL_NOT_AVAILABLE(); }
size_t Kernel::localMemSize() സ്ഥിരം { OCL_NOT_AVAILABLE(); }

ശൂന്യം* Kernel::ptr() സ്ഥിരം { തിരികെ ശൂന്യ; }


Program::Program() CV_NOEXCEPT : p(ശൂന്യ) { }
Program::Program(സ്ഥിരം ProgramSource& src, സ്ഥിരം String& buildflags, String& errmsg) : p(ശൂന്യ) { OCL_NOT_AVAILABLE(); }
Program::Program(സ്ഥിരം Program& prog) : p(ശൂന്യ) { }
Program& Program::operator=(സ്ഥിരം Program& prog) { തിരികെ *ഇത്; }
Program::Program(Program&&) CV_NOEXCEPT : p(ശൂന്യ) { }
Program& Program::operator=(Program&&) CV_NOEXCEPT { തിരികെ *ഇത്; }
Program::~Program() { }

തർക്കം Program::create(സ്ഥിരം ProgramSource& src, സ്ഥിരം String& buildflags, String& errmsg) { OCL_NOT_AVAILABLE(); }

ശൂന്യം* Program::ptr() സ്ഥിരം { തിരികെ ശൂന്യ; }

ശൂന്യം Program::get_ദ്വിമാന(std::vector<അക്ഷരം>& binary) സ്ഥിരം { OCL_NOT_AVAILABLE(); }

തർക്കം Program::പഠ(സ്ഥിരം String& buf, സ്ഥിരം String& buildflags) { OCL_NOT_AVAILABLE(); }
തർക്കം Program::ലിഖ(String& buf) സ്ഥിരം { OCL_NOT_AVAILABLE(); }
സ്ഥിരം ProgramSource& Program::source() സ്ഥിരം { OCL_NOT_AVAILABLE(); }
String Program::getPrefix() സ്ഥിരം { OCL_NOT_AVAILABLE(); }
/* static */ String Program::getPrefix(സ്ഥിരം String& buildflags) { OCL_NOT_AVAILABLE(); }


ProgramSource::ProgramSource() CV_NOEXCEPT : p(ശൂന്യ) { }
ProgramSource::ProgramSource(സ്ഥിരം String& module, സ്ഥിരം String& name, സ്ഥിരം String& codeStr, സ്ഥിരം String& codeHash) : p(ശൂന്യ) { }
ProgramSource::ProgramSource(സ്ഥിരം String& prog) : p(ശൂന്യ) { }
ProgramSource::ProgramSource(സ്ഥിരം അക്ഷരം* prog) : p(ശൂന്യ) { }
ProgramSource::~ProgramSource() { }
ProgramSource::ProgramSource(സ്ഥിരം ProgramSource& prog) : p(ശൂന്യ) { }
ProgramSource& ProgramSource::operator=(സ്ഥിരം ProgramSource& prog) { തിരികെ *ഇത്; }
ProgramSource::ProgramSource(ProgramSource&&) CV_NOEXCEPT : p(ശൂന്യ) { }
ProgramSource& ProgramSource::operator=(ProgramSource&&) CV_NOEXCEPT { തിരികെ *ഇത്; }

സ്ഥിരം String& ProgramSource::source() സ്ഥിരം { OCL_NOT_AVAILABLE(); }
ProgramSource::hash_t ProgramSource::hash() സ്ഥിരം { OCL_NOT_AVAILABLE(); }

/* static */ ProgramSource ProgramSource::from_ദ്വിമാന(സ്ഥിരം String& module, സ്ഥിരം String& name, സ്ഥിരം unsigned അക്ഷരം* binary, സ്ഥിരം size_t size, സ്ഥിരം cv::String& buildOptions) { OCL_NOT_AVAILABLE(); }
/* static */ ProgramSource ProgramSource::fromSPIR(സ്ഥിരം String& module, സ്ഥിരം String& name, സ്ഥിരം unsigned അക്ഷരം* binary, സ്ഥിരം size_t size, സ്ഥിരം cv::String& buildOptions) { OCL_NOT_AVAILABLE(); }


PlatformInfo::PlatformInfo() CV_NOEXCEPT : p(ശൂന്യ) { }
PlatformInfo::PlatformInfo(ശൂന്യം* id) : p(ശൂന്യ) { OCL_NOT_AVAILABLE(); }
PlatformInfo::~PlatformInfo() { }

PlatformInfo::PlatformInfo(സ്ഥിരം PlatformInfo& i) : p(ശൂന്യ) { }
PlatformInfo& PlatformInfo::operator=(സ്ഥിരം PlatformInfo& i) { തിരികെ *ഇത്; }
PlatformInfo::PlatformInfo(PlatformInfo&&) CV_NOEXCEPT : p(ശൂന്യ) { }
PlatformInfo& PlatformInfo::operator=(PlatformInfo&&) CV_NOEXCEPT { തിരികെ *ഇത്; }

String PlatformInfo::name() സ്ഥിരം { OCL_NOT_AVAILABLE(); }
String PlatformInfo::vendor() സ്ഥിരം { OCL_NOT_AVAILABLE(); }
String PlatformInfo::version() സ്ഥിരം { OCL_NOT_AVAILABLE(); }
പൂർണ്ണസംഖ്യ PlatformInfo::device_സംഖ്യ() സ്ഥിരം { OCL_NOT_AVAILABLE(); }
ശൂന്യം PlatformInfo::getDevice(Device& device, പൂർണ്ണസംഖ്യ d) സ്ഥിരം { OCL_NOT_AVAILABLE(); }

സ്ഥിരം അക്ഷരം* convertTypeStr(പൂർണ്ണസംഖ്യ sdepth, പൂർണ്ണസംഖ്യ ddepth, പൂർണ്ണസംഖ്യ cn, അക്ഷരം* buf) { OCL_NOT_AVAILABLE(); }
സ്ഥിരം അക്ഷരം* typeToStr(പൂർണ്ണസംഖ്യ t) { OCL_NOT_AVAILABLE(); }
സ്ഥിരം അക്ഷരം* memopTypeToStr(പൂർണ്ണസംഖ്യ t) { OCL_NOT_AVAILABLE(); }
സ്ഥിരം അക്ഷരം* vecopTypeToStr(പൂർണ്ണസംഖ്യ t) { OCL_NOT_AVAILABLE(); }
സ്ഥിരം അക്ഷരം* getOpenCLErrorString(പൂർണ്ണസംഖ്യ errorCode) { OCL_NOT_AVAILABLE(); }
String kernelToStr(നിവേശം_Array _kernel, പൂർണ്ണസംഖ്യ ddepth, സ്ഥിരം അക്ഷരം* name) { OCL_NOT_AVAILABLE(); }
ശൂന്യം getPlatfomsInfo(std::vector<PlatformInfo>& platform_info) { OCL_NOT_AVAILABLE(); }


പൂർണ്ണസംഖ്യ predictOptimalVectorWidth(നിവേശം_Array src1, നിവേശം_Array src2, നിവേശം_Array src3,
        നിവേശം_Array src4, നിവേശം_Array src5, നിവേശം_Array src6,
        നിവേശം_Array src7, നിവേശം_Array src8, നിവേശം_Array src9,
        OclVectorStrategy strat)
{ OCL_NOT_AVAILABLE(); }

പൂർണ്ണസംഖ്യ checkOptimalVectorWidth(സ്ഥിരം പൂർണ്ണസംഖ്യ *vectorWidths,
        നിവേശം_Array src1, നിവേശം_Array src2, നിവേശം_Array src3,
        നിവേശം_Array src4, നിവേശം_Array src5, നിവേശം_Array src6,
        നിവേശം_Array src7, നിവേശം_Array src8, നിവേശം_Array src9,
        OclVectorStrategy strat)
{ OCL_NOT_AVAILABLE(); }

പൂർണ്ണസംഖ്യ predictOptimalVectorWidthMax(നിവേശം_Array src1, നിവേശം_Array src2, നിവേശം_Array src3,
        നിവേശം_Array src4, നിവേശം_Array src5, നിവേശം_Array src6,
        നിവേശം_Array src7, നിവേശം_Array src8, നിവേശം_Array src9)
{ OCL_NOT_AVAILABLE(); }

ശൂന്യം buildOptionsAddMatrixDescription(String& buildOptions, സ്ഥിരം String& name, നിവേശം_Array _m) { OCL_NOT_AVAILABLE(); }


Image2D::Image2D() CV_NOEXCEPT : p(ശൂന്യ) { }
Image2D::Image2D(സ്ഥിരം UMat &src, തർക്കം norm, തർക്കം alias) { OCL_NOT_AVAILABLE(); }
Image2D::Image2D(സ്ഥിരം Image2D & i) : p(ശൂന്യ) { OCL_NOT_AVAILABLE(); }
Image2D::~Image2D() { }
Image2D& Image2D::operator=(സ്ഥിരം Image2D & i) { തിരികെ *ഇത്; }
Image2D::Image2D(Image2D&&) CV_NOEXCEPT : p(ശൂന്യ) { }
Image2D& Image2D::operator=(Image2D&&) CV_NOEXCEPT { തിരികെ *ഇത്; }

/* static */ തർക്കം Image2D::canCreateAlias(സ്ഥിരം UMat &u) { OCL_NOT_AVAILABLE(); }
/* static */ തർക്കം Image2D::isFormatSupported(പൂർണ്ണസംഖ്യ depth, പൂർണ്ണസംഖ്യ cn, തർക്കം norm) { OCL_NOT_AVAILABLE(); }

ശൂന്യം* Image2D::ptr() സ്ഥിരം { തിരികെ ശൂന്യ; }


Timer::Timer(സ്ഥിരം Queue& q) : p(ശൂന്യ) {}
Timer::~Timer() {}
ശൂന്യം Timer::start() { OCL_NOT_AVAILABLE(); }
ശൂന്യം Timer::stop() { OCL_NOT_AVAILABLE();}

uint64 Timer::durationNS() സ്ഥിരം { OCL_NOT_AVAILABLE(); }

MatAllocator* getOpenCLAllocator() { തിരികെ ശൂന്യ; }

internal::ProgramEntry::operator ProgramSource&() സ്ഥിരം { OCL_NOT_AVAILABLE(); }


ഘടന OpenCLExecutionContext::Impl
{
    Impl() = സ്ഥിരസ്ഥിതി;
};

Context& OpenCLExecutionContext::getContext() സ്ഥിരം { OCL_NOT_AVAILABLE(); }
Device& OpenCLExecutionContext::getDevice() സ്ഥിരം { OCL_NOT_AVAILABLE(); }
Queue& OpenCLExecutionContext::getQueue() സ്ഥിരം { OCL_NOT_AVAILABLE(); }

തർക്കം OpenCLExecutionContext::useOpenCL() സ്ഥിരം { തിരികെ അസത്യം; }
ശൂന്യം OpenCLExecutionContext::setUseOpenCL(തർക്കം flag) { }

സ്ഥിതം
OpenCLExecutionContext& getDummyOpenCLExecutionContext()
{
    സ്ഥിതം OpenCLExecutionContext dummy;
    തിരികെ dummy;
}

/* static */
OpenCLExecutionContext& OpenCLExecutionContext::getCurrent() { തിരികെ getDummyOpenCLExecutionContext(); }

/* static */
OpenCLExecutionContext& OpenCLExecutionContext::getCurrentRef() { തിരികെ getDummyOpenCLExecutionContext(); }

ശൂന്യം OpenCLExecutionContext::bind() സ്ഥിരം { OCL_NOT_AVAILABLE(); }

OpenCLExecutionContext OpenCLExecutionContext::cloneWithNewQueue(സ്ഥിരം ocl::Queue& q) സ്ഥിരം { OCL_NOT_AVAILABLE(); }
OpenCLExecutionContext OpenCLExecutionContext::cloneWithNewQueue() സ്ഥിരം { OCL_NOT_AVAILABLE(); }

/* static */ OpenCLExecutionContext OpenCLExecutionContext::create(സ്ഥിരം std::string& platformName, ശൂന്യം* platformID, ശൂന്യം* context, ശൂന്യം* deviceID) { OCL_NOT_AVAILABLE(); }
/* static */ OpenCLExecutionContext OpenCLExecutionContext::create(സ്ഥിരം Context& context, സ്ഥിരം Device& device, സ്ഥിരം ocl::Queue& queue) { OCL_NOT_AVAILABLE(); }
/* static */ OpenCLExecutionContext OpenCLExecutionContext::create(സ്ഥിരം Context& context, സ്ഥിരം Device& device) { OCL_NOT_AVAILABLE(); }

ശൂന്യം OpenCLExecutionContext::release() { }

}}

#എങ്കിൽ defined(_MSC_VER)
    #pragma warning(pop)
#അല്ലെങ്കിലെങ്കിൽ defined(__clang__)
    #pragma clang diagnostic pop
#അല്ലെങ്കിലെങ്കിൽ defined(__GNUC__)
    #pragma GCC diagnostic pop
#endif
