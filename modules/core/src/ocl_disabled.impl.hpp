// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#অন্তর্ভাৱ "opencv2/core/ocl_genbase.hpp"

#যদি defined(_MSC_VER)
    #pragma warning(push)
    #pragma warning(disable : 4100)
    #pragma warning(disable : 4702)
#নোচেদ্যদি defined(__clang__)
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wunused-parameter"
#নোচেদ্যদি defined(__GNUC__)
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

নামাকাশ cv { নামাকাশ ocl {

স্থির
CV_NORETURN শূন্য throw_no_ocl()
{
    CV_Error(Error::OpenCLApiCallError, "OpenCV build without OpenCL support");
}
#define OCL_NOT_AVAILABLE() throw_no_ocl();

CV_EXPORTS_W তার্কিক haveOpenCL() { ঘূৰাই মিছা; }
CV_EXPORTS_W তার্কিক useOpenCL() { ঘূৰাই মিছা; }
CV_EXPORTS_W তার্কিক haveAmdBlas() { ঘূৰাই মিছা; }
CV_EXPORTS_W তার্কিক haveAmdFft() { ঘূৰাই মিছা; }
CV_EXPORTS_W শূন্য setUseOpenCL(তার্কিক flag) { /* nothing */ }
CV_EXPORTS_W শূন্য finish() { /* nothing */ }

CV_EXPORTS তার্কিক haveSVM() { ঘূৰাই মিছা; }

Device::Device() CV_NOEXCEPT : p(রিক্ত) { }
Device::Device(শূন্য* d) : p(রিক্ত) { OCL_NOT_AVAILABLE(); }
Device::Device(ধ্রুৱ Device& d) : p(রিক্ত) { }
Device& Device::operator=(ধ্রুৱ Device& d) { ঘূৰাই *এতত্; }
Device::Device(Device&&) CV_NOEXCEPT : p(রিক্ত) { }
Device& Device::operator=(Device&&) CV_NOEXCEPT { ঘূৰাই *এতত্; }
Device::~Device() { }

শূন্য Device::set(শূন্য* d) { OCL_NOT_AVAILABLE(); }

String Device::name() ধ্রুৱ { OCL_NOT_AVAILABLE(); }
String Device::extensions() ধ্রুৱ { OCL_NOT_AVAILABLE(); }
তার্কিক Device::isExtensionSupported(ধ্রুৱ String& extensionName) ধ্রুৱ { OCL_NOT_AVAILABLE(); }
String Device::version() ধ্রুৱ { OCL_NOT_AVAILABLE(); }
String Device::vendorName() ধ্রুৱ { OCL_NOT_AVAILABLE(); }
String Device::OpenCL_C_Version() ধ্রুৱ { OCL_NOT_AVAILABLE(); }
String Device::OpenCLVersion() ধ্রুৱ { OCL_NOT_AVAILABLE(); }
পূর্ণাঙ্ক Device::deviceVersionMajor() ধ্রুৱ { OCL_NOT_AVAILABLE(); }
পূর্ণাঙ্ক Device::deviceVersionMinor() ধ্রুৱ { OCL_NOT_AVAILABLE(); }
String Device::driverVersion() ধ্রুৱ { OCL_NOT_AVAILABLE(); }
শূন্য* Device::ptr() ধ্রুৱ { /*OCL_NOT_AVAILABLE();*/ ঘূৰাই রিক্ত; }

পূর্ণাঙ্ক Device::type() ধ্রুৱ { OCL_NOT_AVAILABLE(); }

পূর্ণাঙ্ক Device::addressBits() ধ্রুৱ { OCL_NOT_AVAILABLE(); }
তার্কিক Device::available() ধ্রুৱ { OCL_NOT_AVAILABLE(); }
তার্কিক Device::compilerAvailable() ধ্রুৱ { OCL_NOT_AVAILABLE(); }
তার্কিক Device::linkerAvailable() ধ্রুৱ { OCL_NOT_AVAILABLE(); }

পূর্ণাঙ্ক Device::doubleFPConfig() ধ্রুৱ { OCL_NOT_AVAILABLE(); }
পূর্ণাঙ্ক Device::singleFPConfig() ধ্রুৱ { OCL_NOT_AVAILABLE(); }
পূর্ণাঙ্ক Device::halfFPConfig() ধ্রুৱ { OCL_NOT_AVAILABLE(); }

তার্কিক Device::hasFP64() ধ্রুৱ { OCL_NOT_AVAILABLE(); }
তার্কিক Device::hasFP16() ধ্রুৱ { OCL_NOT_AVAILABLE(); }

তার্কিক Device::endianLittle() ধ্রুৱ { OCL_NOT_AVAILABLE(); }
তার্কিক Device::errorCorrectionSupport() ধ্রুৱ { OCL_NOT_AVAILABLE(); }

পূর্ণাঙ্ক Device::executionCapabilities() ধ্রুৱ { OCL_NOT_AVAILABLE(); }

size_t Device::globalMemCacheSize() ধ্রুৱ { OCL_NOT_AVAILABLE(); }

পূর্ণাঙ্ক Device::globalMemCacheType() ধ্রুৱ { OCL_NOT_AVAILABLE(); }
পূর্ণাঙ্ক Device::global_Mem_Cache_সাৰি_Size() ধ্রুৱ { OCL_NOT_AVAILABLE(); }
size_t Device::globalMemSize() ধ্রুৱ { OCL_NOT_AVAILABLE(); }

size_t Device::localMemSize() ধ্রুৱ { OCL_NOT_AVAILABLE(); }
পূর্ণাঙ্ক Device::localMemType() ধ্রুৱ { ঘূৰাই NO_LOCAL_MEM; }
তার্কিক Device::hostUnifiedMemory() ধ্রুৱ { OCL_NOT_AVAILABLE(); }

তার্কিক Device::imageSupport() ধ্রুৱ { OCL_NOT_AVAILABLE(); }

তার্কিক Device::imageFromBufferSupport() ধ্রুৱ { OCL_NOT_AVAILABLE(); }
uint Device::imagePitchAlignment() ধ্রুৱ { OCL_NOT_AVAILABLE(); }
uint Device::imageBaseAddressAlignment() ধ্রুৱ { OCL_NOT_AVAILABLE(); }

তার্কিক Device::intelSubgroupsSupport() ধ্রুৱ { OCL_NOT_AVAILABLE(); }

size_t Device::image2DMaxWidth() ধ্রুৱ { OCL_NOT_AVAILABLE(); }
size_t Device::image2DMaxHeight() ধ্রুৱ { OCL_NOT_AVAILABLE(); }

size_t Device::image3DMaxWidth() ধ্রুৱ { OCL_NOT_AVAILABLE(); }
size_t Device::image3DMaxHeight() ধ্রুৱ { OCL_NOT_AVAILABLE(); }
size_t Device::image3DMaxDepth() ধ্রুৱ { OCL_NOT_AVAILABLE(); }

size_t Device::imageMaxBufferSize() ধ্রুৱ { OCL_NOT_AVAILABLE(); }
size_t Device::imageMaxArraySize() ধ্রুৱ { OCL_NOT_AVAILABLE(); }

পূর্ণাঙ্ক Device::vendorID() ধ্রুৱ { OCL_NOT_AVAILABLE(); }

পূর্ণাঙ্ক Device::maxClockFrequency() ধ্রুৱ { OCL_NOT_AVAILABLE(); }
পূর্ণাঙ্ক Device::maxComputeUnits() ধ্রুৱ { OCL_NOT_AVAILABLE(); }
পূর্ণাঙ্ক Device::maxConstantArgs() ধ্রুৱ { OCL_NOT_AVAILABLE(); }
size_t Device::maxConstantBufferSize() ধ্রুৱ { OCL_NOT_AVAILABLE(); }

size_t Device::maxMemAllocSize() ধ্রুৱ { OCL_NOT_AVAILABLE(); }
size_t Device::maxParameterSize() ধ্রুৱ { OCL_NOT_AVAILABLE(); }

পূর্ণাঙ্ক Device::maxReadImageArgs() ধ্রুৱ { OCL_NOT_AVAILABLE(); }
পূর্ণাঙ্ক Device::maxWriteImageArgs() ধ্রুৱ { OCL_NOT_AVAILABLE(); }
পূর্ণাঙ্ক Device::maxSamplers() ধ্রুৱ { OCL_NOT_AVAILABLE(); }

size_t Device::maxWorkGroupSize() ধ্রুৱ { OCL_NOT_AVAILABLE(); }
পূর্ণাঙ্ক Device::maxWorkItemDims() ধ্রুৱ { OCL_NOT_AVAILABLE(); }
শূন্য Device::maxWorkItemSizes(size_t*) ধ্রুৱ { OCL_NOT_AVAILABLE(); }

পূর্ণাঙ্ক Device::memBaseAddrAlign() ধ্রুৱ { OCL_NOT_AVAILABLE(); }

পূর্ণাঙ্ক Device::nativeVectorWidthChar() ধ্রুৱ { OCL_NOT_AVAILABLE(); }
পূর্ণাঙ্ক Device::nativeVectorWidthShort() ধ্রুৱ { OCL_NOT_AVAILABLE(); }
পূর্ণাঙ্ক Device::nativeVectorWidthInt() ধ্রুৱ { OCL_NOT_AVAILABLE(); }
পূর্ণাঙ্ক Device::nativeVectorWidthLong() ধ্রুৱ { OCL_NOT_AVAILABLE(); }
পূর্ণাঙ্ক Device::nativeVectorWidthFloat() ধ্রুৱ { OCL_NOT_AVAILABLE(); }
পূর্ণাঙ্ক Device::nativeVectorWidthDouble() ধ্রুৱ { OCL_NOT_AVAILABLE(); }
পূর্ণাঙ্ক Device::nativeVectorWidthHalf() ধ্রুৱ { OCL_NOT_AVAILABLE(); }

পূর্ণাঙ্ক Device::preferredVectorWidthChar() ধ্রুৱ { OCL_NOT_AVAILABLE(); }
পূর্ণাঙ্ক Device::preferredVectorWidthShort() ধ্রুৱ { OCL_NOT_AVAILABLE(); }
পূর্ণাঙ্ক Device::preferredVectorWidthInt() ধ্রুৱ { OCL_NOT_AVAILABLE(); }
পূর্ণাঙ্ক Device::preferredVectorWidthLong() ধ্রুৱ { OCL_NOT_AVAILABLE(); }
পূর্ণাঙ্ক Device::preferredVectorWidthFloat() ধ্রুৱ { OCL_NOT_AVAILABLE(); }
পূর্ণাঙ্ক Device::preferredVectorWidthDouble() ধ্রুৱ { OCL_NOT_AVAILABLE(); }
পূর্ণাঙ্ক Device::preferredVectorWidthHalf() ধ্রুৱ { OCL_NOT_AVAILABLE(); }

size_t Device::printfBufferSize() ধ্রুৱ { OCL_NOT_AVAILABLE(); }
size_t Device::profilingTimerResolution() ধ্রুৱ { OCL_NOT_AVAILABLE(); }

/* static */
ধ্রুৱ Device& Device::getDefault()
{
    স্থির Device dummy;
    ঘূৰাই dummy;
}

/* static */ Device Device::fromHandle(শূন্য* d) { OCL_NOT_AVAILABLE(); }


Context::Context() CV_NOEXCEPT : p(রিক্ত) { }
Context::Context(পূর্ণাঙ্ক dtype) : p(রিক্ত) { }
Context::~Context() { }
Context::Context(ধ্রুৱ Context& c) : p(রিক্ত) { }
Context& Context::operator=(ধ্রুৱ Context& c) { ঘূৰাই *এতত্; }
Context::Context(Context&&) CV_NOEXCEPT : p(রিক্ত) { }
Context& Context::operator=(Context&&) CV_NOEXCEPT { ঘূৰাই *এতত্; }

তার্কিক Context::create() { ঘূৰাই মিছা; }
তার্কিক Context::create(পূর্ণাঙ্ক dtype) { ঘূৰাই মিছা; }
size_t Context::ndevices() ধ্রুৱ { ঘূৰাই 0; }
Device& Context::device(size_t idx) ধ্রুৱ { OCL_NOT_AVAILABLE(); }
Program Context::getProg(ধ্রুৱ ProgramSource& prog, ধ্রুৱ String& buildopt, String& errmsg) { OCL_NOT_AVAILABLE(); }
শূন্য Context::unloadProg(Program& prog) { }

/* static */
Context& Context::getDefault()
{
    স্থির Context dummy;
    ঘূৰাই dummy;
}
শূন্য* Context::ptr() ধ্রুৱ { ঘূৰাই রিক্ত; }

শূন্য* Context::getOpenCLContextProperty(পূর্ণাঙ্ক /*propertyId*/) ধ্রুৱ { OCL_NOT_AVAILABLE(); }

তার্কিক Context::useSVM() ধ্রুৱ { ঘূৰাই মিছা; }
শূন্য Context::setUseSVM(তার্কিক enabled) { }

Context::UserContext::~UserContext() { }

শূন্য Context::setUserContext(std::type_index /*typeId*/, ধ্রুৱ std::shared_ptr<Context::UserContext>& /*userContext*/) { OCL_NOT_AVAILABLE(); }
std::shared_ptr<Context::UserContext> Context::getUserContext(std::type_index /*typeId*/) { OCL_NOT_AVAILABLE(); }

/* static */ Context Context::fromHandle(শূন্য* context) { OCL_NOT_AVAILABLE(); }
/* static */ Context Context::fromDevice(ধ্রুৱ ocl::Device& device) { OCL_NOT_AVAILABLE(); }
/* static */ Context Context::create(ধ্রুৱ std::string& configuration) { OCL_NOT_AVAILABLE(); }

শূন্য Context::release() { }


Platform::Platform() CV_NOEXCEPT : p(রিক্ত) { }
Platform::~Platform() { }
Platform::Platform(ধ্রুৱ Platform&) : p(রিক্ত) { }
Platform& Platform::operator=(ধ্রুৱ Platform&) { ঘূৰাই *এতত্; }
Platform::Platform(Platform&&) CV_NOEXCEPT : p(রিক্ত) { }
Platform& Platform::operator=(Platform&&) CV_NOEXCEPT { ঘূৰাই *এতত্; }

শূন্য* Platform::ptr() ধ্রুৱ { ঘূৰাই রিক্ত; }

/* static */
Platform& Platform::getDefault()
{
    স্থির Platform dummy;
    ঘূৰাই dummy;
}

শূন্য attachContext(ধ্রুৱ String& platformName, শূন্য* platformID, শূন্য* context, শূন্য* deviceID) { OCL_NOT_AVAILABLE(); }
শূন্য convertFromBuffer(শূন্য* cl_mem_buffer, size_t step, পূর্ণাঙ্ক rows, পূর্ণাঙ্ক cols, পূর্ণাঙ্ক type, UMat& dst) { OCL_NOT_AVAILABLE(); }
শূন্য convertFromImage(শূন্য* cl_mem_image, UMat& dst) { OCL_NOT_AVAILABLE(); }

শূন্য initializeContextFromHandle(Context& ctx, শূন্য* platform, শূন্য* context, শূন্য* device) { OCL_NOT_AVAILABLE(); }

Queue::Queue() CV_NOEXCEPT : p(রিক্ত) { }
Queue::Queue(ধ্রুৱ Context& c, ধ্রুৱ Device& d) : p(রিক্ত) { OCL_NOT_AVAILABLE(); }
Queue::~Queue() { }
Queue::Queue(ধ্রুৱ Queue& q) {}
Queue& Queue::operator=(ধ্রুৱ Queue& q) { ঘূৰাই *এতত্; }
Queue::Queue(Queue&&) CV_NOEXCEPT : p(রিক্ত) { }
Queue& Queue::operator=(Queue&&) CV_NOEXCEPT { ঘূৰাই *এতত্; }

তার্কিক Queue::create(ধ্রুৱ Context& c, ধ্রুৱ Device& d) { OCL_NOT_AVAILABLE(); }
শূন্য Queue::finish() {}
শূন্য* Queue::ptr() ধ্রুৱ { ঘূৰাই রিক্ত; }
/* static */
Queue& Queue::getDefault()
{
    স্থির Queue dummy;
    ঘূৰাই dummy;
}

/// @brief Returns OpenCL command queue with enable profiling mode support
ধ্রুৱ Queue& Queue::getProfilingQueue() ধ্রুৱ { OCL_NOT_AVAILABLE(); }


KernelArg::KernelArg() CV_NOEXCEPT
    : flags(0), m(0), obj(0), sz(0), wscale(1), iwscale(1)
{
}

KernelArg::KernelArg(পূর্ণাঙ্ক _flags, UMat* _m, পূর্ণাঙ্ক _wscale, পূর্ণাঙ্ক _iwscale, ধ্রুৱ শূন্য* _obj, size_t _sz)
    : flags(_flags), m(_m), obj(_obj), sz(_sz), wscale(_wscale), iwscale(_iwscale)
{
    OCL_NOT_AVAILABLE();
}

KernelArg KernelArg::Constant(ধ্রুৱ Mat& m)
{
    OCL_NOT_AVAILABLE();
}


Kernel::Kernel() CV_NOEXCEPT : p(রিক্ত) { }
Kernel::Kernel(ধ্রুৱ ৱর্ণ* kname, ধ্রুৱ Program& prog) : p(রিক্ত) { OCL_NOT_AVAILABLE(); }
Kernel::Kernel(ধ্রুৱ ৱর্ণ* kname, ধ্রুৱ ProgramSource& prog, ধ্রুৱ String& buildopts, String* errmsg) : p(রিক্ত) { OCL_NOT_AVAILABLE(); }
Kernel::~Kernel() { }
Kernel::Kernel(ধ্রুৱ Kernel& k) : p(রিক্ত) { }
Kernel& Kernel::operator=(ধ্রুৱ Kernel& k) { ঘূৰাই *এতত্; }
Kernel::Kernel(Kernel&&) CV_NOEXCEPT : p(রিক্ত) { }
Kernel& Kernel::operator=(Kernel&&) CV_NOEXCEPT { ঘূৰাই *এতত্; }

তার্কিক Kernel::empty() ধ্রুৱ { ঘূৰাই সঁচা; }
তার্কিক Kernel::create(ধ্রুৱ ৱর্ণ* kname, ধ্রুৱ Program& prog) { OCL_NOT_AVAILABLE(); }
তার্কিক Kernel::create(ধ্রুৱ ৱর্ণ* kname, ধ্রুৱ ProgramSource& prog, ধ্রুৱ String& buildopts, String* errmsg) { OCL_NOT_AVAILABLE(); }

পূর্ণাঙ্ক Kernel::set(পূর্ণাঙ্ক i, ধ্রুৱ শূন্য* value, size_t sz) { OCL_NOT_AVAILABLE(); }
পূর্ণাঙ্ক Kernel::set(পূর্ণাঙ্ক i, ধ্রুৱ Image2D& image2D) { OCL_NOT_AVAILABLE(); }
পূর্ণাঙ্ক Kernel::set(পূর্ণাঙ্ক i, ধ্রুৱ UMat& m) { OCL_NOT_AVAILABLE(); }
পূর্ণাঙ্ক Kernel::set(পূর্ণাঙ্ক i, ধ্রুৱ KernelArg& arg) { OCL_NOT_AVAILABLE(); }

তার্কিক Kernel::run(পূর্ণাঙ্ক dims, size_t globalsize[], size_t localsize[], তার্কিক সমকাল, ধ্রুৱ Queue& q) { OCL_NOT_AVAILABLE(); }
তার্কিক Kernel::runTask(তার্কিক সমকাল, ধ্রুৱ Queue& q) { OCL_NOT_AVAILABLE(); }

int64 Kernel::runProfiling(পূর্ণাঙ্ক dims, size_t globalsize[], size_t localsize[], ধ্রুৱ Queue& q) { OCL_NOT_AVAILABLE(); }

size_t Kernel::workGroupSize() ধ্রুৱ { OCL_NOT_AVAILABLE(); }
size_t Kernel::preferedWorkGroupSizeMultiple() ধ্রুৱ { OCL_NOT_AVAILABLE(); }
তার্কিক Kernel::compileWorkGroupSize(size_t wsz[]) ধ্রুৱ { OCL_NOT_AVAILABLE(); }
size_t Kernel::localMemSize() ধ্রুৱ { OCL_NOT_AVAILABLE(); }

শূন্য* Kernel::ptr() ধ্রুৱ { ঘূৰাই রিক্ত; }


Program::Program() CV_NOEXCEPT : p(রিক্ত) { }
Program::Program(ধ্রুৱ ProgramSource& src, ধ্রুৱ String& buildflags, String& errmsg) : p(রিক্ত) { OCL_NOT_AVAILABLE(); }
Program::Program(ধ্রুৱ Program& prog) : p(রিক্ত) { }
Program& Program::operator=(ধ্রুৱ Program& prog) { ঘূৰাই *এতত্; }
Program::Program(Program&&) CV_NOEXCEPT : p(রিক্ত) { }
Program& Program::operator=(Program&&) CV_NOEXCEPT { ঘূৰাই *এতত্; }
Program::~Program() { }

তার্কিক Program::create(ধ্রুৱ ProgramSource& src, ধ্রুৱ String& buildflags, String& errmsg) { OCL_NOT_AVAILABLE(); }

শূন্য* Program::ptr() ধ্রুৱ { ঘূৰাই রিক্ত; }

শূন্য Program::get_দ্বিমিক(std::vector<ৱর্ণ>& binary) ধ্রুৱ { OCL_NOT_AVAILABLE(); }

তার্কিক Program::পঠ(ধ্রুৱ String& buf, ধ্রুৱ String& buildflags) { OCL_NOT_AVAILABLE(); }
তার্কিক Program::লিখ(String& buf) ধ্রুৱ { OCL_NOT_AVAILABLE(); }
ধ্রুৱ ProgramSource& Program::source() ধ্রুৱ { OCL_NOT_AVAILABLE(); }
String Program::getPrefix() ধ্রুৱ { OCL_NOT_AVAILABLE(); }
/* static */ String Program::getPrefix(ধ্রুৱ String& buildflags) { OCL_NOT_AVAILABLE(); }


ProgramSource::ProgramSource() CV_NOEXCEPT : p(রিক্ত) { }
ProgramSource::ProgramSource(ধ্রুৱ String& module, ধ্রুৱ String& name, ধ্রুৱ String& codeStr, ধ্রুৱ String& codeHash) : p(রিক্ত) { }
ProgramSource::ProgramSource(ধ্রুৱ String& prog) : p(রিক্ত) { }
ProgramSource::ProgramSource(ধ্রুৱ ৱর্ণ* prog) : p(রিক্ত) { }
ProgramSource::~ProgramSource() { }
ProgramSource::ProgramSource(ধ্রুৱ ProgramSource& prog) : p(রিক্ত) { }
ProgramSource& ProgramSource::operator=(ধ্রুৱ ProgramSource& prog) { ঘূৰাই *এতত্; }
ProgramSource::ProgramSource(ProgramSource&&) CV_NOEXCEPT : p(রিক্ত) { }
ProgramSource& ProgramSource::operator=(ProgramSource&&) CV_NOEXCEPT { ঘূৰাই *এতত্; }

ধ্রুৱ String& ProgramSource::source() ধ্রুৱ { OCL_NOT_AVAILABLE(); }
ProgramSource::hash_t ProgramSource::hash() ধ্রুৱ { OCL_NOT_AVAILABLE(); }

/* static */ ProgramSource ProgramSource::from_দ্বিমিক(ধ্রুৱ String& module, ধ্রুৱ String& name, ধ্রুৱ unsigned ৱর্ণ* binary, ধ্রুৱ size_t size, ধ্রুৱ cv::String& buildOptions) { OCL_NOT_AVAILABLE(); }
/* static */ ProgramSource ProgramSource::fromSPIR(ধ্রুৱ String& module, ধ্রুৱ String& name, ধ্রুৱ unsigned ৱর্ণ* binary, ধ্রুৱ size_t size, ধ্রুৱ cv::String& buildOptions) { OCL_NOT_AVAILABLE(); }


PlatformInfo::PlatformInfo() CV_NOEXCEPT : p(রিক্ত) { }
PlatformInfo::PlatformInfo(শূন্য* id) : p(রিক্ত) { OCL_NOT_AVAILABLE(); }
PlatformInfo::~PlatformInfo() { }

PlatformInfo::PlatformInfo(ধ্রুৱ PlatformInfo& i) : p(রিক্ত) { }
PlatformInfo& PlatformInfo::operator=(ধ্রুৱ PlatformInfo& i) { ঘূৰাই *এতত্; }
PlatformInfo::PlatformInfo(PlatformInfo&&) CV_NOEXCEPT : p(রিক্ত) { }
PlatformInfo& PlatformInfo::operator=(PlatformInfo&&) CV_NOEXCEPT { ঘূৰাই *এতত্; }

String PlatformInfo::name() ধ্রুৱ { OCL_NOT_AVAILABLE(); }
String PlatformInfo::vendor() ধ্রুৱ { OCL_NOT_AVAILABLE(); }
String PlatformInfo::version() ধ্রুৱ { OCL_NOT_AVAILABLE(); }
পূর্ণাঙ্ক PlatformInfo::device_সংখ্যা() ধ্রুৱ { OCL_NOT_AVAILABLE(); }
শূন্য PlatformInfo::getDevice(Device& device, পূর্ণাঙ্ক d) ধ্রুৱ { OCL_NOT_AVAILABLE(); }

ধ্রুৱ ৱর্ণ* convertTypeStr(পূর্ণাঙ্ক sdepth, পূর্ণাঙ্ক ddepth, পূর্ণাঙ্ক cn, ৱর্ণ* buf) { OCL_NOT_AVAILABLE(); }
ধ্রুৱ ৱর্ণ* typeToStr(পূর্ণাঙ্ক t) { OCL_NOT_AVAILABLE(); }
ধ্রুৱ ৱর্ণ* memopTypeToStr(পূর্ণাঙ্ক t) { OCL_NOT_AVAILABLE(); }
ধ্রুৱ ৱর্ণ* vecopTypeToStr(পূর্ণাঙ্ক t) { OCL_NOT_AVAILABLE(); }
ধ্রুৱ ৱর্ণ* getOpenCLErrorString(পূর্ণাঙ্ক errorCode) { OCL_NOT_AVAILABLE(); }
String kernelToStr(প্ৰবেশ_Array _kernel, পূর্ণাঙ্ক ddepth, ধ্রুৱ ৱর্ণ* name) { OCL_NOT_AVAILABLE(); }
শূন্য getPlatfomsInfo(std::vector<PlatformInfo>& platform_info) { OCL_NOT_AVAILABLE(); }


পূর্ণাঙ্ক predictOptimalVectorWidth(প্ৰবেশ_Array src1, প্ৰবেশ_Array src2, প্ৰবেশ_Array src3,
        প্ৰবেশ_Array src4, প্ৰবেশ_Array src5, প্ৰবেশ_Array src6,
        প্ৰবেশ_Array src7, প্ৰবেশ_Array src8, প্ৰবেশ_Array src9,
        OclVectorStrategy strat)
{ OCL_NOT_AVAILABLE(); }

পূর্ণাঙ্ক checkOptimalVectorWidth(ধ্রুৱ পূর্ণাঙ্ক *vectorWidths,
        প্ৰবেশ_Array src1, প্ৰবেশ_Array src2, প্ৰবেশ_Array src3,
        প্ৰবেশ_Array src4, প্ৰবেশ_Array src5, প্ৰবেশ_Array src6,
        প্ৰবেশ_Array src7, প্ৰবেশ_Array src8, প্ৰবেশ_Array src9,
        OclVectorStrategy strat)
{ OCL_NOT_AVAILABLE(); }

পূর্ণাঙ্ক predictOptimalVectorWidthMax(প্ৰবেশ_Array src1, প্ৰবেশ_Array src2, প্ৰবেশ_Array src3,
        প্ৰবেশ_Array src4, প্ৰবেশ_Array src5, প্ৰবেশ_Array src6,
        প্ৰবেশ_Array src7, প্ৰবেশ_Array src8, প্ৰবেশ_Array src9)
{ OCL_NOT_AVAILABLE(); }

শূন্য buildOptionsAddMatrixDescription(String& buildOptions, ধ্রুৱ String& name, প্ৰবেশ_Array _m) { OCL_NOT_AVAILABLE(); }


Image2D::Image2D() CV_NOEXCEPT : p(রিক্ত) { }
Image2D::Image2D(ধ্রুৱ UMat &src, তার্কিক norm, তার্কিক alias) { OCL_NOT_AVAILABLE(); }
Image2D::Image2D(ধ্রুৱ Image2D & i) : p(রিক্ত) { OCL_NOT_AVAILABLE(); }
Image2D::~Image2D() { }
Image2D& Image2D::operator=(ধ্রুৱ Image2D & i) { ঘূৰাই *এতত্; }
Image2D::Image2D(Image2D&&) CV_NOEXCEPT : p(রিক্ত) { }
Image2D& Image2D::operator=(Image2D&&) CV_NOEXCEPT { ঘূৰাই *এতত্; }

/* static */ তার্কিক Image2D::canCreateAlias(ধ্রুৱ UMat &u) { OCL_NOT_AVAILABLE(); }
/* static */ তার্কিক Image2D::isFormatSupported(পূর্ণাঙ্ক depth, পূর্ণাঙ্ক cn, তার্কিক norm) { OCL_NOT_AVAILABLE(); }

শূন্য* Image2D::ptr() ধ্রুৱ { ঘূৰাই রিক্ত; }


Timer::Timer(ধ্রুৱ Queue& q) : p(রিক্ত) {}
Timer::~Timer() {}
শূন্য Timer::start() { OCL_NOT_AVAILABLE(); }
শূন্য Timer::stop() { OCL_NOT_AVAILABLE();}

uint64 Timer::durationNS() ধ্রুৱ { OCL_NOT_AVAILABLE(); }

MatAllocator* getOpenCLAllocator() { ঘূৰাই রিক্ত; }

internal::ProgramEntry::operator ProgramSource&() ধ্রুৱ { OCL_NOT_AVAILABLE(); }


সংরচনা OpenCLExecutionContext::Impl
{
    Impl() = স্ৱভাৱ;
};

Context& OpenCLExecutionContext::getContext() ধ্রুৱ { OCL_NOT_AVAILABLE(); }
Device& OpenCLExecutionContext::getDevice() ধ্রুৱ { OCL_NOT_AVAILABLE(); }
Queue& OpenCLExecutionContext::getQueue() ধ্রুৱ { OCL_NOT_AVAILABLE(); }

তার্কিক OpenCLExecutionContext::useOpenCL() ধ্রুৱ { ঘূৰাই মিছা; }
শূন্য OpenCLExecutionContext::setUseOpenCL(তার্কিক flag) { }

স্থির
OpenCLExecutionContext& getDummyOpenCLExecutionContext()
{
    স্থির OpenCLExecutionContext dummy;
    ঘূৰাই dummy;
}

/* static */
OpenCLExecutionContext& OpenCLExecutionContext::getCurrent() { ঘূৰাই getDummyOpenCLExecutionContext(); }

/* static */
OpenCLExecutionContext& OpenCLExecutionContext::getCurrentRef() { ঘূৰাই getDummyOpenCLExecutionContext(); }

শূন্য OpenCLExecutionContext::bind() ধ্রুৱ { OCL_NOT_AVAILABLE(); }

OpenCLExecutionContext OpenCLExecutionContext::cloneWithNewQueue(ধ্রুৱ ocl::Queue& q) ধ্রুৱ { OCL_NOT_AVAILABLE(); }
OpenCLExecutionContext OpenCLExecutionContext::cloneWithNewQueue() ধ্রুৱ { OCL_NOT_AVAILABLE(); }

/* static */ OpenCLExecutionContext OpenCLExecutionContext::create(ধ্রুৱ std::string& platformName, শূন্য* platformID, শূন্য* context, শূন্য* deviceID) { OCL_NOT_AVAILABLE(); }
/* static */ OpenCLExecutionContext OpenCLExecutionContext::create(ধ্রুৱ Context& context, ধ্রুৱ Device& device, ধ্রুৱ ocl::Queue& queue) { OCL_NOT_AVAILABLE(); }
/* static */ OpenCLExecutionContext OpenCLExecutionContext::create(ধ্রুৱ Context& context, ধ্রুৱ Device& device) { OCL_NOT_AVAILABLE(); }

শূন্য OpenCLExecutionContext::release() { }

}}

#যদি defined(_MSC_VER)
    #pragma warning(pop)
#নোচেদ্যদি defined(__clang__)
    #pragma clang diagnostic pop
#নোচেদ্যদি defined(__GNUC__)
    #pragma GCC diagnostic pop
#endif
