// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#অন্তর্ভাব "opencv2/core/ocl_genbase.hpp"

#যদি defined(_MSC_VER)
    #pragma warning(push)
    #pragma warning(disable : 4100)
    #pragma warning(disable : 4702)
#নতুবা_যদি defined(__clang__)
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wunused-parameter"
#নতুবা_যদি defined(__GNUC__)
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

CV_EXPORTS_W যৌক্তিক haveOpenCL() { ফেরত মিথ্যা; }
CV_EXPORTS_W যৌক্তিক useOpenCL() { ফেরত মিথ্যা; }
CV_EXPORTS_W যৌক্তিক haveAmdBlas() { ফেরত মিথ্যা; }
CV_EXPORTS_W যৌক্তিক haveAmdFft() { ফেরত মিথ্যা; }
CV_EXPORTS_W শূন্য setUseOpenCL(যৌক্তিক flag) { /* nothing */ }
CV_EXPORTS_W শূন্য finish() { /* nothing */ }

CV_EXPORTS যৌক্তিক haveSVM() { ফেরত মিথ্যা; }

Device::Device() CV_NOEXCEPT : p(ফাঁকা) { }
Device::Device(শূন্য* d) : p(ফাঁকা) { OCL_NOT_AVAILABLE(); }
Device::Device(ধ্রুবক Device& d) : p(ফাঁকা) { }
Device& Device::operator=(ধ্রুবক Device& d) { ফেরত *এই; }
Device::Device(Device&&) CV_NOEXCEPT : p(ফাঁকা) { }
Device& Device::operator=(Device&&) CV_NOEXCEPT { ফেরত *এই; }
Device::~Device() { }

শূন্য Device::set(শূন্য* d) { OCL_NOT_AVAILABLE(); }

String Device::name() ধ্রুবক { OCL_NOT_AVAILABLE(); }
String Device::extensions() ধ্রুবক { OCL_NOT_AVAILABLE(); }
যৌক্তিক Device::isExtensionSupported(ধ্রুবক String& extensionName) ধ্রুবক { OCL_NOT_AVAILABLE(); }
String Device::version() ধ্রুবক { OCL_NOT_AVAILABLE(); }
String Device::vendorName() ধ্রুবক { OCL_NOT_AVAILABLE(); }
String Device::OpenCL_C_Version() ধ্রুবক { OCL_NOT_AVAILABLE(); }
String Device::OpenCLVersion() ধ্রুবক { OCL_NOT_AVAILABLE(); }
পূর্ণসংখ্যা Device::deviceVersionMajor() ধ্রুবক { OCL_NOT_AVAILABLE(); }
পূর্ণসংখ্যা Device::deviceVersionMinor() ধ্রুবক { OCL_NOT_AVAILABLE(); }
String Device::driverVersion() ধ্রুবক { OCL_NOT_AVAILABLE(); }
শূন্য* Device::ptr() ধ্রুবক { /*OCL_NOT_AVAILABLE();*/ ফেরত ফাঁকা; }

পূর্ণসংখ্যা Device::type() ধ্রুবক { OCL_NOT_AVAILABLE(); }

পূর্ণসংখ্যা Device::addressBits() ধ্রুবক { OCL_NOT_AVAILABLE(); }
যৌক্তিক Device::available() ধ্রুবক { OCL_NOT_AVAILABLE(); }
যৌক্তিক Device::compilerAvailable() ধ্রুবক { OCL_NOT_AVAILABLE(); }
যৌক্তিক Device::linkerAvailable() ধ্রুবক { OCL_NOT_AVAILABLE(); }

পূর্ণসংখ্যা Device::doubleFPConfig() ধ্রুবক { OCL_NOT_AVAILABLE(); }
পূর্ণসংখ্যা Device::singleFPConfig() ধ্রুবক { OCL_NOT_AVAILABLE(); }
পূর্ণসংখ্যা Device::halfFPConfig() ধ্রুবক { OCL_NOT_AVAILABLE(); }

যৌক্তিক Device::hasFP64() ধ্রুবক { OCL_NOT_AVAILABLE(); }
যৌক্তিক Device::hasFP16() ধ্রুবক { OCL_NOT_AVAILABLE(); }

যৌক্তিক Device::endianLittle() ধ্রুবক { OCL_NOT_AVAILABLE(); }
যৌক্তিক Device::errorCorrectionSupport() ধ্রুবক { OCL_NOT_AVAILABLE(); }

পূর্ণসংখ্যা Device::executionCapabilities() ধ্রুবক { OCL_NOT_AVAILABLE(); }

size_t Device::global_Mem_Cache_আকার() ধ্রুবক { OCL_NOT_AVAILABLE(); }

পূর্ণসংখ্যা Device::globalMemCacheType() ধ্রুবক { OCL_NOT_AVAILABLE(); }
পূর্ণসংখ্যা Device::global_Mem_Cache_সারি_আকার() ধ্রুবক { OCL_NOT_AVAILABLE(); }
size_t Device::global_Mem_আকার() ধ্রুবক { OCL_NOT_AVAILABLE(); }

size_t Device::local_Mem_আকার() ধ্রুবক { OCL_NOT_AVAILABLE(); }
পূর্ণসংখ্যা Device::localMemType() ধ্রুবক { ফেরত NO_LOCAL_MEM; }
যৌক্তিক Device::hostUnifiedMemory() ধ্রুবক { OCL_NOT_AVAILABLE(); }

যৌক্তিক Device::imageSupport() ধ্রুবক { OCL_NOT_AVAILABLE(); }

যৌক্তিক Device::image_From_বাফার_Support() ধ্রুবক { OCL_NOT_AVAILABLE(); }
uint Device::imagePitchAlignment() ধ্রুবক { OCL_NOT_AVAILABLE(); }
uint Device::imageBaseAddressAlignment() ধ্রুবক { OCL_NOT_AVAILABLE(); }

যৌক্তিক Device::intelSubgroupsSupport() ধ্রুবক { OCL_NOT_AVAILABLE(); }

size_t Device::image2DMaxWidth() ধ্রুবক { OCL_NOT_AVAILABLE(); }
size_t Device::image2DMaxHeight() ধ্রুবক { OCL_NOT_AVAILABLE(); }

size_t Device::image3DMaxWidth() ধ্রুবক { OCL_NOT_AVAILABLE(); }
size_t Device::image3DMaxHeight() ধ্রুবক { OCL_NOT_AVAILABLE(); }
size_t Device::image3DMaxDepth() ধ্রুবক { OCL_NOT_AVAILABLE(); }

size_t Device::image_Max_বাফার_আকার() ধ্রুবক { OCL_NOT_AVAILABLE(); }
size_t Device::image_Max_Array_আকার() ধ্রুবক { OCL_NOT_AVAILABLE(); }

পূর্ণসংখ্যা Device::vendorID() ধ্রুবক { OCL_NOT_AVAILABLE(); }

পূর্ণসংখ্যা Device::maxClockFrequency() ধ্রুবক { OCL_NOT_AVAILABLE(); }
পূর্ণসংখ্যা Device::maxComputeUnits() ধ্রুবক { OCL_NOT_AVAILABLE(); }
পূর্ণসংখ্যা Device::maxConstantArgs() ধ্রুবক { OCL_NOT_AVAILABLE(); }
size_t Device::max_Constant_বাফার_আকার() ধ্রুবক { OCL_NOT_AVAILABLE(); }

size_t Device::max_Mem_Alloc_আকার() ধ্রুবক { OCL_NOT_AVAILABLE(); }
size_t Device::max_Parameter_আকার() ধ্রুবক { OCL_NOT_AVAILABLE(); }

পূর্ণসংখ্যা Device::maxReadImageArgs() ধ্রুবক { OCL_NOT_AVAILABLE(); }
পূর্ণসংখ্যা Device::maxWriteImageArgs() ধ্রুবক { OCL_NOT_AVAILABLE(); }
পূর্ণসংখ্যা Device::maxSamplers() ধ্রুবক { OCL_NOT_AVAILABLE(); }

size_t Device::max_Work_Group_আকার() ধ্রুবক { OCL_NOT_AVAILABLE(); }
পূর্ণসংখ্যা Device::maxWorkItemDims() ধ্রুবক { OCL_NOT_AVAILABLE(); }
শূন্য Device::maxWorkItemSizes(size_t*) ধ্রুবক { OCL_NOT_AVAILABLE(); }

পূর্ণসংখ্যা Device::memBaseAddrAlign() ধ্রুবক { OCL_NOT_AVAILABLE(); }

পূর্ণসংখ্যা Device::nativeVectorWidthChar() ধ্রুবক { OCL_NOT_AVAILABLE(); }
পূর্ণসংখ্যা Device::nativeVectorWidthShort() ধ্রুবক { OCL_NOT_AVAILABLE(); }
পূর্ণসংখ্যা Device::nativeVectorWidthInt() ধ্রুবক { OCL_NOT_AVAILABLE(); }
পূর্ণসংখ্যা Device::nativeVectorWidthLong() ধ্রুবক { OCL_NOT_AVAILABLE(); }
পূর্ণসংখ্যা Device::nativeVectorWidthFloat() ধ্রুবক { OCL_NOT_AVAILABLE(); }
পূর্ণসংখ্যা Device::nativeVectorWidthDouble() ধ্রুবক { OCL_NOT_AVAILABLE(); }
পূর্ণসংখ্যা Device::nativeVectorWidthHalf() ধ্রুবক { OCL_NOT_AVAILABLE(); }

পূর্ণসংখ্যা Device::preferredVectorWidthChar() ধ্রুবক { OCL_NOT_AVAILABLE(); }
পূর্ণসংখ্যা Device::preferredVectorWidthShort() ধ্রুবক { OCL_NOT_AVAILABLE(); }
পূর্ণসংখ্যা Device::preferredVectorWidthInt() ধ্রুবক { OCL_NOT_AVAILABLE(); }
পূর্ণসংখ্যা Device::preferredVectorWidthLong() ধ্রুবক { OCL_NOT_AVAILABLE(); }
পূর্ণসংখ্যা Device::preferredVectorWidthFloat() ধ্রুবক { OCL_NOT_AVAILABLE(); }
পূর্ণসংখ্যা Device::preferredVectorWidthDouble() ধ্রুবক { OCL_NOT_AVAILABLE(); }
পূর্ণসংখ্যা Device::preferredVectorWidthHalf() ধ্রুবক { OCL_NOT_AVAILABLE(); }

size_t Device::printf_বাফার_আকার() ধ্রুবক { OCL_NOT_AVAILABLE(); }
size_t Device::profilingTimerResolution() ধ্রুবক { OCL_NOT_AVAILABLE(); }

/* static */
ধ্রুবক Device& Device::getDefault()
{
    স্থির Device dummy;
    ফেরত dummy;
}

/* static */ Device Device::fromHandle(শূন্য* d) { OCL_NOT_AVAILABLE(); }


Context::Context() CV_NOEXCEPT : p(ফাঁকা) { }
Context::Context(পূর্ণসংখ্যা dtype) : p(ফাঁকা) { }
Context::~Context() { }
Context::Context(ধ্রুবক Context& c) : p(ফাঁকা) { }
Context& Context::operator=(ধ্রুবক Context& c) { ফেরত *এই; }
Context::Context(Context&&) CV_NOEXCEPT : p(ফাঁকা) { }
Context& Context::operator=(Context&&) CV_NOEXCEPT { ফেরত *এই; }

যৌক্তিক Context::create() { ফেরত মিথ্যা; }
যৌক্তিক Context::create(পূর্ণসংখ্যা dtype) { ফেরত মিথ্যা; }
size_t Context::ndevices() ধ্রুবক { ফেরত 0; }
Device& Context::device(size_t idx) ধ্রুবক { OCL_NOT_AVAILABLE(); }
Program Context::getProg(ধ্রুবক ProgramSource& prog, ধ্রুবক String& buildopt, String& errmsg) { OCL_NOT_AVAILABLE(); }
শূন্য Context::unloadProg(Program& prog) { }

/* static */
Context& Context::getDefault()
{
    স্থির Context dummy;
    ফেরত dummy;
}
শূন্য* Context::ptr() ধ্রুবক { ফেরত ফাঁকা; }

শূন্য* Context::getOpenCLContextProperty(পূর্ণসংখ্যা /*propertyId*/) ধ্রুবক { OCL_NOT_AVAILABLE(); }

যৌক্তিক Context::useSVM() ধ্রুবক { ফেরত মিথ্যা; }
শূন্য Context::setUseSVM(যৌক্তিক enabled) { }

Context::UserContext::~UserContext() { }

শূন্য Context::setUserContext(std::type_সূচক /*typeId*/, ধ্রুবক std::shared_ptr<Context::UserContext>& /*userContext*/) { OCL_NOT_AVAILABLE(); }
std::shared_ptr<Context::UserContext> Context::getUserContext(std::type_সূচক /*typeId*/) { OCL_NOT_AVAILABLE(); }

/* static */ Context Context::fromHandle(শূন্য* context) { OCL_NOT_AVAILABLE(); }
/* static */ Context Context::fromDevice(ধ্রুবক ocl::Device& device) { OCL_NOT_AVAILABLE(); }
/* static */ Context Context::create(ধ্রুবক std::string& configuration) { OCL_NOT_AVAILABLE(); }

শূন্য Context::release() { }


Platform::Platform() CV_NOEXCEPT : p(ফাঁকা) { }
Platform::~Platform() { }
Platform::Platform(ধ্রুবক Platform&) : p(ফাঁকা) { }
Platform& Platform::operator=(ধ্রুবক Platform&) { ফেরত *এই; }
Platform::Platform(Platform&&) CV_NOEXCEPT : p(ফাঁকা) { }
Platform& Platform::operator=(Platform&&) CV_NOEXCEPT { ফেরত *এই; }

শূন্য* Platform::ptr() ধ্রুবক { ফেরত ফাঁকা; }

/* static */
Platform& Platform::getDefault()
{
    স্থির Platform dummy;
    ফেরত dummy;
}

শূন্য attachContext(ধ্রুবক String& platformName, শূন্য* platformID, শূন্য* context, শূন্য* deviceID) { OCL_NOT_AVAILABLE(); }
শূন্য convert_From_বাফার(শূন্য* cl_mem_বাফার, size_t step, পূর্ণসংখ্যা rows, পূর্ণসংখ্যা cols, পূর্ণসংখ্যা type, UMat& dst) { OCL_NOT_AVAILABLE(); }
শূন্য convertFromImage(শূন্য* cl_mem_image, UMat& dst) { OCL_NOT_AVAILABLE(); }

শূন্য initializeContextFromHandle(Context& ctx, শূন্য* platform, শূন্য* context, শূন্য* device) { OCL_NOT_AVAILABLE(); }

Queue::Queue() CV_NOEXCEPT : p(ফাঁকা) { }
Queue::Queue(ধ্রুবক Context& c, ধ্রুবক Device& d) : p(ফাঁকা) { OCL_NOT_AVAILABLE(); }
Queue::~Queue() { }
Queue::Queue(ধ্রুবক Queue& q) {}
Queue& Queue::operator=(ধ্রুবক Queue& q) { ফেরত *এই; }
Queue::Queue(Queue&&) CV_NOEXCEPT : p(ফাঁকা) { }
Queue& Queue::operator=(Queue&&) CV_NOEXCEPT { ফেরত *এই; }

যৌক্তিক Queue::create(ধ্রুবক Context& c, ধ্রুবক Device& d) { OCL_NOT_AVAILABLE(); }
শূন্য Queue::finish() {}
শূন্য* Queue::ptr() ধ্রুবক { ফেরত ফাঁকা; }
/* static */
Queue& Queue::getDefault()
{
    স্থির Queue dummy;
    ফেরত dummy;
}

/// @brief Returns OpenCL command queue with enable profiling mode support
ধ্রুবক Queue& Queue::getProfilingQueue() ধ্রুবক { OCL_NOT_AVAILABLE(); }


KernelArg::KernelArg() CV_NOEXCEPT
    : flags(0), m(0), obj(0), sz(0), wscale(1), iwscale(1)
{
}

KernelArg::KernelArg(পূর্ণসংখ্যা _flags, UMat* _m, পূর্ণসংখ্যা _wscale, পূর্ণসংখ্যা _iwscale, ধ্রুবক শূন্য* _obj, size_t _sz)
    : flags(_flags), m(_m), obj(_obj), sz(_sz), wscale(_wscale), iwscale(_iwscale)
{
    OCL_NOT_AVAILABLE();
}

KernelArg KernelArg::Constant(ধ্রুবক Mat& m)
{
    OCL_NOT_AVAILABLE();
}


Kernel::Kernel() CV_NOEXCEPT : p(ফাঁকা) { }
Kernel::Kernel(ধ্রুবক অক্ষর* kname, ধ্রুবক Program& prog) : p(ফাঁকা) { OCL_NOT_AVAILABLE(); }
Kernel::Kernel(ধ্রুবক অক্ষর* kname, ধ্রুবক ProgramSource& prog, ধ্রুবক String& buildopts, String* errmsg) : p(ফাঁকা) { OCL_NOT_AVAILABLE(); }
Kernel::~Kernel() { }
Kernel::Kernel(ধ্রুবক Kernel& k) : p(ফাঁকা) { }
Kernel& Kernel::operator=(ধ্রুবক Kernel& k) { ফেরত *এই; }
Kernel::Kernel(Kernel&&) CV_NOEXCEPT : p(ফাঁকা) { }
Kernel& Kernel::operator=(Kernel&&) CV_NOEXCEPT { ফেরত *এই; }

যৌক্তিক Kernel::empty() ধ্রুবক { ফেরত সত্য; }
যৌক্তিক Kernel::create(ধ্রুবক অক্ষর* kname, ধ্রুবক Program& prog) { OCL_NOT_AVAILABLE(); }
যৌক্তিক Kernel::create(ধ্রুবক অক্ষর* kname, ধ্রুবক ProgramSource& prog, ধ্রুবক String& buildopts, String* errmsg) { OCL_NOT_AVAILABLE(); }

পূর্ণসংখ্যা Kernel::set(পূর্ণসংখ্যা i, ধ্রুবক শূন্য* value, size_t sz) { OCL_NOT_AVAILABLE(); }
পূর্ণসংখ্যা Kernel::set(পূর্ণসংখ্যা i, ধ্রুবক Image2D& image2D) { OCL_NOT_AVAILABLE(); }
পূর্ণসংখ্যা Kernel::set(পূর্ণসংখ্যা i, ধ্রুবক UMat& m) { OCL_NOT_AVAILABLE(); }
পূর্ণসংখ্যা Kernel::set(পূর্ণসংখ্যা i, ধ্রুবক KernelArg& arg) { OCL_NOT_AVAILABLE(); }

যৌক্তিক Kernel::run(পূর্ণসংখ্যা dims, size_t globalsize[], size_t localsize[], যৌক্তিক সমলয়, ধ্রুবক Queue& q) { OCL_NOT_AVAILABLE(); }
যৌক্তিক Kernel::runTask(যৌক্তিক সমলয়, ধ্রুবক Queue& q) { OCL_NOT_AVAILABLE(); }

int64 Kernel::runProfiling(পূর্ণসংখ্যা dims, size_t globalsize[], size_t localsize[], ধ্রুবক Queue& q) { OCL_NOT_AVAILABLE(); }

size_t Kernel::work_Group_আকার() ধ্রুবক { OCL_NOT_AVAILABLE(); }
size_t Kernel::prefered_Work_Group_আকার_Multiple() ধ্রুবক { OCL_NOT_AVAILABLE(); }
যৌক্তিক Kernel::compile_Work_Group_আকার(size_t wsz[]) ধ্রুবক { OCL_NOT_AVAILABLE(); }
size_t Kernel::local_Mem_আকার() ধ্রুবক { OCL_NOT_AVAILABLE(); }

শূন্য* Kernel::ptr() ধ্রুবক { ফেরত ফাঁকা; }


Program::Program() CV_NOEXCEPT : p(ফাঁকা) { }
Program::Program(ধ্রুবক ProgramSource& src, ধ্রুবক String& buildflags, String& errmsg) : p(ফাঁকা) { OCL_NOT_AVAILABLE(); }
Program::Program(ধ্রুবক Program& prog) : p(ফাঁকা) { }
Program& Program::operator=(ধ্রুবক Program& prog) { ফেরত *এই; }
Program::Program(Program&&) CV_NOEXCEPT : p(ফাঁকা) { }
Program& Program::operator=(Program&&) CV_NOEXCEPT { ফেরত *এই; }
Program::~Program() { }

যৌক্তিক Program::create(ধ্রুবক ProgramSource& src, ধ্রুবক String& buildflags, String& errmsg) { OCL_NOT_AVAILABLE(); }

শূন্য* Program::ptr() ধ্রুবক { ফেরত ফাঁকা; }

শূন্য Program::get_দ্বিমিক(std::vector<অক্ষর>& binary) ধ্রুবক { OCL_NOT_AVAILABLE(); }

যৌক্তিক Program::পঠ(ধ্রুবক String& buf, ধ্রুবক String& buildflags) { OCL_NOT_AVAILABLE(); }
যৌক্তিক Program::লিখ(String& buf) ধ্রুবক { OCL_NOT_AVAILABLE(); }
ধ্রুবক ProgramSource& Program::source() ধ্রুবক { OCL_NOT_AVAILABLE(); }
String Program::getPrefix() ধ্রুবক { OCL_NOT_AVAILABLE(); }
/* static */ String Program::getPrefix(ধ্রুবক String& buildflags) { OCL_NOT_AVAILABLE(); }


ProgramSource::ProgramSource() CV_NOEXCEPT : p(ফাঁকা) { }
ProgramSource::ProgramSource(ধ্রুবক String& module, ধ্রুবক String& name, ধ্রুবক String& codeStr, ধ্রুবক String& codeHash) : p(ফাঁকা) { }
ProgramSource::ProgramSource(ধ্রুবক String& prog) : p(ফাঁকা) { }
ProgramSource::ProgramSource(ধ্রুবক অক্ষর* prog) : p(ফাঁকা) { }
ProgramSource::~ProgramSource() { }
ProgramSource::ProgramSource(ধ্রুবক ProgramSource& prog) : p(ফাঁকা) { }
ProgramSource& ProgramSource::operator=(ধ্রুবক ProgramSource& prog) { ফেরত *এই; }
ProgramSource::ProgramSource(ProgramSource&&) CV_NOEXCEPT : p(ফাঁকা) { }
ProgramSource& ProgramSource::operator=(ProgramSource&&) CV_NOEXCEPT { ফেরত *এই; }

ধ্রুবক String& ProgramSource::source() ধ্রুবক { OCL_NOT_AVAILABLE(); }
ProgramSource::hash_t ProgramSource::hash() ধ্রুবক { OCL_NOT_AVAILABLE(); }

/* static */ ProgramSource ProgramSource::from_দ্বিমিক(ধ্রুবক String& module, ধ্রুবক String& name, ধ্রুবক unsigned অক্ষর* binary, ধ্রুবক size_t size, ধ্রুবক cv::String& buildOptions) { OCL_NOT_AVAILABLE(); }
/* static */ ProgramSource ProgramSource::fromSPIR(ধ্রুবক String& module, ধ্রুবক String& name, ধ্রুবক unsigned অক্ষর* binary, ধ্রুবক size_t size, ধ্রুবক cv::String& buildOptions) { OCL_NOT_AVAILABLE(); }


PlatformInfo::PlatformInfo() CV_NOEXCEPT : p(ফাঁকা) { }
PlatformInfo::PlatformInfo(শূন্য* id) : p(ফাঁকা) { OCL_NOT_AVAILABLE(); }
PlatformInfo::~PlatformInfo() { }

PlatformInfo::PlatformInfo(ধ্রুবক PlatformInfo& i) : p(ফাঁকা) { }
PlatformInfo& PlatformInfo::operator=(ধ্রুবক PlatformInfo& i) { ফেরত *এই; }
PlatformInfo::PlatformInfo(PlatformInfo&&) CV_NOEXCEPT : p(ফাঁকা) { }
PlatformInfo& PlatformInfo::operator=(PlatformInfo&&) CV_NOEXCEPT { ফেরত *এই; }

String PlatformInfo::name() ধ্রুবক { OCL_NOT_AVAILABLE(); }
String PlatformInfo::vendor() ধ্রুবক { OCL_NOT_AVAILABLE(); }
String PlatformInfo::version() ধ্রুবক { OCL_NOT_AVAILABLE(); }
পূর্ণসংখ্যা PlatformInfo::device_সংখ্যা() ধ্রুবক { OCL_NOT_AVAILABLE(); }
শূন্য PlatformInfo::getDevice(Device& device, পূর্ণসংখ্যা d) ধ্রুবক { OCL_NOT_AVAILABLE(); }

ধ্রুবক অক্ষর* convertTypeStr(পূর্ণসংখ্যা sdepth, পূর্ণসংখ্যা ddepth, পূর্ণসংখ্যা cn, অক্ষর* buf) { OCL_NOT_AVAILABLE(); }
ধ্রুবক অক্ষর* typeToStr(পূর্ণসংখ্যা t) { OCL_NOT_AVAILABLE(); }
ধ্রুবক অক্ষর* memopTypeToStr(পূর্ণসংখ্যা t) { OCL_NOT_AVAILABLE(); }
ধ্রুবক অক্ষর* vecopTypeToStr(পূর্ণসংখ্যা t) { OCL_NOT_AVAILABLE(); }
ধ্রুবক অক্ষর* getOpenCLErrorString(পূর্ণসংখ্যা errorCode) { OCL_NOT_AVAILABLE(); }
String kernelToStr(প্রবেশ_Array _kernel, পূর্ণসংখ্যা ddepth, ধ্রুবক অক্ষর* name) { OCL_NOT_AVAILABLE(); }
শূন্য getPlatfomsInfo(std::vector<PlatformInfo>& platform_info) { OCL_NOT_AVAILABLE(); }


পূর্ণসংখ্যা predictOptimalVectorWidth(প্রবেশ_Array src1, প্রবেশ_Array src2, প্রবেশ_Array src3,
        প্রবেশ_Array src4, প্রবেশ_Array src5, প্রবেশ_Array src6,
        প্রবেশ_Array src7, প্রবেশ_Array src8, প্রবেশ_Array src9,
        OclVectorStrategy strat)
{ OCL_NOT_AVAILABLE(); }

পূর্ণসংখ্যা checkOptimalVectorWidth(ধ্রুবক পূর্ণসংখ্যা *vectorWidths,
        প্রবেশ_Array src1, প্রবেশ_Array src2, প্রবেশ_Array src3,
        প্রবেশ_Array src4, প্রবেশ_Array src5, প্রবেশ_Array src6,
        প্রবেশ_Array src7, প্রবেশ_Array src8, প্রবেশ_Array src9,
        OclVectorStrategy strat)
{ OCL_NOT_AVAILABLE(); }

পূর্ণসংখ্যা predictOptimalVectorWidthMax(প্রবেশ_Array src1, প্রবেশ_Array src2, প্রবেশ_Array src3,
        প্রবেশ_Array src4, প্রবেশ_Array src5, প্রবেশ_Array src6,
        প্রবেশ_Array src7, প্রবেশ_Array src8, প্রবেশ_Array src9)
{ OCL_NOT_AVAILABLE(); }

শূন্য buildOptionsAddMatrixDescription(String& buildOptions, ধ্রুবক String& name, প্রবেশ_Array _m) { OCL_NOT_AVAILABLE(); }


Image2D::Image2D() CV_NOEXCEPT : p(ফাঁকা) { }
Image2D::Image2D(ধ্রুবক UMat &src, যৌক্তিক norm, যৌক্তিক alias) { OCL_NOT_AVAILABLE(); }
Image2D::Image2D(ধ্রুবক Image2D & i) : p(ফাঁকা) { OCL_NOT_AVAILABLE(); }
Image2D::~Image2D() { }
Image2D& Image2D::operator=(ধ্রুবক Image2D & i) { ফেরত *এই; }
Image2D::Image2D(Image2D&&) CV_NOEXCEPT : p(ফাঁকা) { }
Image2D& Image2D::operator=(Image2D&&) CV_NOEXCEPT { ফেরত *এই; }

/* static */ যৌক্তিক Image2D::canCreateAlias(ধ্রুবক UMat &u) { OCL_NOT_AVAILABLE(); }
/* static */ যৌক্তিক Image2D::isFormatSupported(পূর্ণসংখ্যা depth, পূর্ণসংখ্যা cn, যৌক্তিক norm) { OCL_NOT_AVAILABLE(); }

শূন্য* Image2D::ptr() ধ্রুবক { ফেরত ফাঁকা; }


Timer::Timer(ধ্রুবক Queue& q) : p(ফাঁকা) {}
Timer::~Timer() {}
শূন্য Timer::start() { OCL_NOT_AVAILABLE(); }
শূন্য Timer::stop() { OCL_NOT_AVAILABLE();}

uint64 Timer::durationNS() ধ্রুবক { OCL_NOT_AVAILABLE(); }

MatAllocator* getOpenCLAllocator() { ফেরত ফাঁকা; }

internal::ProgramEntry::operator ProgramSource&() ধ্রুবক { OCL_NOT_AVAILABLE(); }


কাঠামো OpenCLExecutionContext::Impl
{
    Impl() = পূর্বনির্ধারিত;
};

Context& OpenCLExecutionContext::getContext() ধ্রুবক { OCL_NOT_AVAILABLE(); }
Device& OpenCLExecutionContext::getDevice() ধ্রুবক { OCL_NOT_AVAILABLE(); }
Queue& OpenCLExecutionContext::getQueue() ধ্রুবক { OCL_NOT_AVAILABLE(); }

যৌক্তিক OpenCLExecutionContext::useOpenCL() ধ্রুবক { ফেরত মিথ্যা; }
শূন্য OpenCLExecutionContext::setUseOpenCL(যৌক্তিক flag) { }

স্থির
OpenCLExecutionContext& getDummyOpenCLExecutionContext()
{
    স্থির OpenCLExecutionContext dummy;
    ফেরত dummy;
}

/* static */
OpenCLExecutionContext& OpenCLExecutionContext::getCurrent() { ফেরত getDummyOpenCLExecutionContext(); }

/* static */
OpenCLExecutionContext& OpenCLExecutionContext::getCurrentRef() { ফেরত getDummyOpenCLExecutionContext(); }

শূন্য OpenCLExecutionContext::bind() ধ্রুবক { OCL_NOT_AVAILABLE(); }

OpenCLExecutionContext OpenCLExecutionContext::cloneWithNewQueue(ধ্রুবক ocl::Queue& q) ধ্রুবক { OCL_NOT_AVAILABLE(); }
OpenCLExecutionContext OpenCLExecutionContext::cloneWithNewQueue() ধ্রুবক { OCL_NOT_AVAILABLE(); }

/* static */ OpenCLExecutionContext OpenCLExecutionContext::create(ধ্রুবক std::string& platformName, শূন্য* platformID, শূন্য* context, শূন্য* deviceID) { OCL_NOT_AVAILABLE(); }
/* static */ OpenCLExecutionContext OpenCLExecutionContext::create(ধ্রুবক Context& context, ধ্রুবক Device& device, ধ্রুবক ocl::Queue& queue) { OCL_NOT_AVAILABLE(); }
/* static */ OpenCLExecutionContext OpenCLExecutionContext::create(ধ্রুবক Context& context, ধ্রুবক Device& device) { OCL_NOT_AVAILABLE(); }

শূন্য OpenCLExecutionContext::release() { }

}}

#যদি defined(_MSC_VER)
    #pragma warning(pop)
#নতুবা_যদি defined(__clang__)
    #pragma clang diagnostic pop
#নতুবা_যদি defined(__GNUC__)
    #pragma GCC diagnostic pop
#endif
