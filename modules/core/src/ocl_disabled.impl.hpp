// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ಅನ್ತರ್ಭಾವ "opencv2/core/ocl_genbase.hpp"

#ಯದಿ defined(_MSC_VER)
    #pragma warning(push)
    #pragma warning(disable : 4100)
    #pragma warning(disable : 4702)
#ನೋಚೇದ್ಯದಿ defined(__clang__)
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wunused-parameter"
#ನೋಚೇದ್ಯದಿ defined(__GNUC__)
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

ನಾಮಾಕಾಶ cv { ನಾಮಾಕಾಶ ocl {

ಸ್ಥಿರ
CV_NORETURN ಶೂನ್ಯ throw_no_ocl()
{
    CV_Error(Error::OpenCLApiCallError, "OpenCV build without OpenCL support");
}
#define OCL_NOT_AVAILABLE() throw_no_ocl();

CV_EXPORTS_W ತಾರ್ಕಿಕ haveOpenCL() { ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ; }
CV_EXPORTS_W ತಾರ್ಕಿಕ useOpenCL() { ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ; }
CV_EXPORTS_W ತಾರ್ಕಿಕ haveAmdBlas() { ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ; }
CV_EXPORTS_W ತಾರ್ಕಿಕ haveAmdFft() { ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ; }
CV_EXPORTS_W ಶೂನ್ಯ setUseOpenCL(ತಾರ್ಕಿಕ flag) { /* nothing */ }
CV_EXPORTS_W ಶೂನ್ಯ finish() { /* nothing */ }

CV_EXPORTS ತಾರ್ಕಿಕ haveSVM() { ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ; }

Device::Device() CV_NOEXCEPT : p(ರಿಕ್ತ) { }
Device::Device(ಶೂನ್ಯ* d) : p(ರಿಕ್ತ) { OCL_NOT_AVAILABLE(); }
Device::Device(ಧ್ರುವ Device& d) : p(ರಿಕ್ತ) { }
Device& Device::operator=(ಧ್ರುವ Device& d) { ಪ್ರತ್ಯಾಗಮ *ಏತತ್; }
Device::Device(Device&&) CV_NOEXCEPT : p(ರಿಕ್ತ) { }
Device& Device::operator=(Device&&) CV_NOEXCEPT { ಪ್ರತ್ಯಾಗಮ *ಏತತ್; }
Device::~Device() { }

ಶೂನ್ಯ Device::set(ಶೂನ್ಯ* d) { OCL_NOT_AVAILABLE(); }

String Device::name() ಧ್ರುವ { OCL_NOT_AVAILABLE(); }
String Device::extensions() ಧ್ರುವ { OCL_NOT_AVAILABLE(); }
ತಾರ್ಕಿಕ Device::isExtensionSupported(ಧ್ರುವ String& extensionName) ಧ್ರುವ { OCL_NOT_AVAILABLE(); }
String Device::version() ಧ್ರುವ { OCL_NOT_AVAILABLE(); }
String Device::vendorName() ಧ್ರುವ { OCL_NOT_AVAILABLE(); }
String Device::OpenCL_C_Version() ಧ್ರುವ { OCL_NOT_AVAILABLE(); }
String Device::OpenCLVersion() ಧ್ರುವ { OCL_NOT_AVAILABLE(); }
ಪೂರ್ಣಾಙ್ಕ Device::deviceVersionMajor() ಧ್ರುವ { OCL_NOT_AVAILABLE(); }
ಪೂರ್ಣಾಙ್ಕ Device::deviceVersionMinor() ಧ್ರುವ { OCL_NOT_AVAILABLE(); }
String Device::driverVersion() ಧ್ರುವ { OCL_NOT_AVAILABLE(); }
ಶೂನ್ಯ* Device::ptr() ಧ್ರುವ { /*OCL_NOT_AVAILABLE();*/ ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ; }

ಪೂರ್ಣಾಙ್ಕ Device::type() ಧ್ರುವ { OCL_NOT_AVAILABLE(); }

ಪೂರ್ಣಾಙ್ಕ Device::addressBits() ಧ್ರುವ { OCL_NOT_AVAILABLE(); }
ತಾರ್ಕಿಕ Device::available() ಧ್ರುವ { OCL_NOT_AVAILABLE(); }
ತಾರ್ಕಿಕ Device::compilerAvailable() ಧ್ರುವ { OCL_NOT_AVAILABLE(); }
ತಾರ್ಕಿಕ Device::linkerAvailable() ಧ್ರುವ { OCL_NOT_AVAILABLE(); }

ಪೂರ್ಣಾಙ್ಕ Device::doubleFPConfig() ಧ್ರುವ { OCL_NOT_AVAILABLE(); }
ಪೂರ್ಣಾಙ್ಕ Device::singleFPConfig() ಧ್ರುವ { OCL_NOT_AVAILABLE(); }
ಪೂರ್ಣಾಙ್ಕ Device::halfFPConfig() ಧ್ರುವ { OCL_NOT_AVAILABLE(); }

ತಾರ್ಕಿಕ Device::hasFP64() ಧ್ರುವ { OCL_NOT_AVAILABLE(); }
ತಾರ್ಕಿಕ Device::hasFP16() ಧ್ರುವ { OCL_NOT_AVAILABLE(); }

ತಾರ್ಕಿಕ Device::endianLittle() ಧ್ರುವ { OCL_NOT_AVAILABLE(); }
ತಾರ್ಕಿಕ Device::errorCorrectionSupport() ಧ್ರುವ { OCL_NOT_AVAILABLE(); }

ಪೂರ್ಣಾಙ್ಕ Device::executionCapabilities() ಧ್ರುವ { OCL_NOT_AVAILABLE(); }

size_t Device::globalMemCacheSize() ಧ್ರುವ { OCL_NOT_AVAILABLE(); }

ಪೂರ್ಣಾಙ್ಕ Device::globalMemCacheType() ಧ್ರುವ { OCL_NOT_AVAILABLE(); }
ಪೂರ್ಣಾಙ್ಕ Device::global_Mem_Cache_ಸಾಲು_Size() ಧ್ರುವ { OCL_NOT_AVAILABLE(); }
size_t Device::globalMemSize() ಧ್ರುವ { OCL_NOT_AVAILABLE(); }

size_t Device::localMemSize() ಧ್ರುವ { OCL_NOT_AVAILABLE(); }
ಪೂರ್ಣಾಙ್ಕ Device::localMemType() ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ NO_LOCAL_MEM; }
ತಾರ್ಕಿಕ Device::hostUnifiedMemory() ಧ್ರುವ { OCL_NOT_AVAILABLE(); }

ತಾರ್ಕಿಕ Device::imageSupport() ಧ್ರುವ { OCL_NOT_AVAILABLE(); }

ತಾರ್ಕಿಕ Device::imageFromBufferSupport() ಧ್ರುವ { OCL_NOT_AVAILABLE(); }
uint Device::imagePitchAlignment() ಧ್ರುವ { OCL_NOT_AVAILABLE(); }
uint Device::imageBaseAddressAlignment() ಧ್ರುವ { OCL_NOT_AVAILABLE(); }

ತಾರ್ಕಿಕ Device::intelSubgroupsSupport() ಧ್ರುವ { OCL_NOT_AVAILABLE(); }

size_t Device::image2DMaxWidth() ಧ್ರುವ { OCL_NOT_AVAILABLE(); }
size_t Device::image2DMaxHeight() ಧ್ರುವ { OCL_NOT_AVAILABLE(); }

size_t Device::image3DMaxWidth() ಧ್ರುವ { OCL_NOT_AVAILABLE(); }
size_t Device::image3DMaxHeight() ಧ್ರುವ { OCL_NOT_AVAILABLE(); }
size_t Device::image3DMaxDepth() ಧ್ರುವ { OCL_NOT_AVAILABLE(); }

size_t Device::imageMaxBufferSize() ಧ್ರುವ { OCL_NOT_AVAILABLE(); }
size_t Device::imageMaxArraySize() ಧ್ರುವ { OCL_NOT_AVAILABLE(); }

ಪೂರ್ಣಾಙ್ಕ Device::vendorID() ಧ್ರುವ { OCL_NOT_AVAILABLE(); }

ಪೂರ್ಣಾಙ್ಕ Device::maxClockFrequency() ಧ್ರುವ { OCL_NOT_AVAILABLE(); }
ಪೂರ್ಣಾಙ್ಕ Device::maxComputeUnits() ಧ್ರುವ { OCL_NOT_AVAILABLE(); }
ಪೂರ್ಣಾಙ್ಕ Device::maxConstantArgs() ಧ್ರುವ { OCL_NOT_AVAILABLE(); }
size_t Device::maxConstantBufferSize() ಧ್ರುವ { OCL_NOT_AVAILABLE(); }

size_t Device::maxMemAllocSize() ಧ್ರುವ { OCL_NOT_AVAILABLE(); }
size_t Device::maxParameterSize() ಧ್ರುವ { OCL_NOT_AVAILABLE(); }

ಪೂರ್ಣಾಙ್ಕ Device::maxReadImageArgs() ಧ್ರುವ { OCL_NOT_AVAILABLE(); }
ಪೂರ್ಣಾಙ್ಕ Device::maxWriteImageArgs() ಧ್ರುವ { OCL_NOT_AVAILABLE(); }
ಪೂರ್ಣಾಙ್ಕ Device::maxSamplers() ಧ್ರುವ { OCL_NOT_AVAILABLE(); }

size_t Device::maxWorkGroupSize() ಧ್ರುವ { OCL_NOT_AVAILABLE(); }
ಪೂರ್ಣಾಙ್ಕ Device::maxWorkItemDims() ಧ್ರುವ { OCL_NOT_AVAILABLE(); }
ಶೂನ್ಯ Device::maxWorkItemSizes(size_t*) ಧ್ರುವ { OCL_NOT_AVAILABLE(); }

ಪೂರ್ಣಾಙ್ಕ Device::memBaseAddrAlign() ಧ್ರುವ { OCL_NOT_AVAILABLE(); }

ಪೂರ್ಣಾಙ್ಕ Device::nativeVectorWidthChar() ಧ್ರುವ { OCL_NOT_AVAILABLE(); }
ಪೂರ್ಣಾಙ್ಕ Device::nativeVectorWidthShort() ಧ್ರುವ { OCL_NOT_AVAILABLE(); }
ಪೂರ್ಣಾಙ್ಕ Device::nativeVectorWidthInt() ಧ್ರುವ { OCL_NOT_AVAILABLE(); }
ಪೂರ್ಣಾಙ್ಕ Device::nativeVectorWidthLong() ಧ್ರುವ { OCL_NOT_AVAILABLE(); }
ಪೂರ್ಣಾಙ್ಕ Device::nativeVectorWidthFloat() ಧ್ರುವ { OCL_NOT_AVAILABLE(); }
ಪೂರ್ಣಾಙ್ಕ Device::nativeVectorWidthDouble() ಧ್ರುವ { OCL_NOT_AVAILABLE(); }
ಪೂರ್ಣಾಙ್ಕ Device::nativeVectorWidthHalf() ಧ್ರುವ { OCL_NOT_AVAILABLE(); }

ಪೂರ್ಣಾಙ್ಕ Device::preferredVectorWidthChar() ಧ್ರುವ { OCL_NOT_AVAILABLE(); }
ಪೂರ್ಣಾಙ್ಕ Device::preferredVectorWidthShort() ಧ್ರುವ { OCL_NOT_AVAILABLE(); }
ಪೂರ್ಣಾಙ್ಕ Device::preferredVectorWidthInt() ಧ್ರುವ { OCL_NOT_AVAILABLE(); }
ಪೂರ್ಣಾಙ್ಕ Device::preferredVectorWidthLong() ಧ್ರುವ { OCL_NOT_AVAILABLE(); }
ಪೂರ್ಣಾಙ್ಕ Device::preferredVectorWidthFloat() ಧ್ರುವ { OCL_NOT_AVAILABLE(); }
ಪೂರ್ಣಾಙ್ಕ Device::preferredVectorWidthDouble() ಧ್ರುವ { OCL_NOT_AVAILABLE(); }
ಪೂರ್ಣಾಙ್ಕ Device::preferredVectorWidthHalf() ಧ್ರುವ { OCL_NOT_AVAILABLE(); }

size_t Device::printfBufferSize() ಧ್ರುವ { OCL_NOT_AVAILABLE(); }
size_t Device::profilingTimerResolution() ಧ್ರುವ { OCL_NOT_AVAILABLE(); }

/* static */
ಧ್ರುವ Device& Device::getDefault()
{
    ಸ್ಥಿರ Device dummy;
    ಪ್ರತ್ಯಾಗಮ dummy;
}

/* static */ Device Device::fromHandle(ಶೂನ್ಯ* d) { OCL_NOT_AVAILABLE(); }


Context::Context() CV_NOEXCEPT : p(ರಿಕ್ತ) { }
Context::Context(ಪೂರ್ಣಾಙ್ಕ dtype) : p(ರಿಕ್ತ) { }
Context::~Context() { }
Context::Context(ಧ್ರುವ Context& c) : p(ರಿಕ್ತ) { }
Context& Context::operator=(ಧ್ರುವ Context& c) { ಪ್ರತ್ಯಾಗಮ *ಏತತ್; }
Context::Context(Context&&) CV_NOEXCEPT : p(ರಿಕ್ತ) { }
Context& Context::operator=(Context&&) CV_NOEXCEPT { ಪ್ರತ್ಯಾಗಮ *ಏತತ್; }

ತಾರ್ಕಿಕ Context::create() { ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ; }
ತಾರ್ಕಿಕ Context::create(ಪೂರ್ಣಾಙ್ಕ dtype) { ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ; }
size_t Context::ndevices() ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ 0; }
Device& Context::device(size_t idx) ಧ್ರುವ { OCL_NOT_AVAILABLE(); }
Program Context::getProg(ಧ್ರುವ ProgramSource& prog, ಧ್ರುವ String& buildopt, String& errmsg) { OCL_NOT_AVAILABLE(); }
ಶೂನ್ಯ Context::unloadProg(Program& prog) { }

/* static */
Context& Context::getDefault()
{
    ಸ್ಥಿರ Context dummy;
    ಪ್ರತ್ಯಾಗಮ dummy;
}
ಶೂನ್ಯ* Context::ptr() ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ; }

ಶೂನ್ಯ* Context::getOpenCLContextProperty(ಪೂರ್ಣಾಙ್ಕ /*propertyId*/) ಧ್ರುವ { OCL_NOT_AVAILABLE(); }

ತಾರ್ಕಿಕ Context::useSVM() ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ; }
ಶೂನ್ಯ Context::setUseSVM(ತಾರ್ಕಿಕ enabled) { }

Context::UserContext::~UserContext() { }

ಶೂನ್ಯ Context::setUserContext(std::type_index /*typeId*/, ಧ್ರುವ std::shared_ptr<Context::UserContext>& /*userContext*/) { OCL_NOT_AVAILABLE(); }
std::shared_ptr<Context::UserContext> Context::getUserContext(std::type_index /*typeId*/) { OCL_NOT_AVAILABLE(); }

/* static */ Context Context::fromHandle(ಶೂನ್ಯ* context) { OCL_NOT_AVAILABLE(); }
/* static */ Context Context::fromDevice(ಧ್ರುವ ocl::Device& device) { OCL_NOT_AVAILABLE(); }
/* static */ Context Context::create(ಧ್ರುವ std::string& configuration) { OCL_NOT_AVAILABLE(); }

ಶೂನ್ಯ Context::release() { }


Platform::Platform() CV_NOEXCEPT : p(ರಿಕ್ತ) { }
Platform::~Platform() { }
Platform::Platform(ಧ್ರುವ Platform&) : p(ರಿಕ್ತ) { }
Platform& Platform::operator=(ಧ್ರುವ Platform&) { ಪ್ರತ್ಯಾಗಮ *ಏತತ್; }
Platform::Platform(Platform&&) CV_NOEXCEPT : p(ರಿಕ್ತ) { }
Platform& Platform::operator=(Platform&&) CV_NOEXCEPT { ಪ್ರತ್ಯಾಗಮ *ಏತತ್; }

ಶೂನ್ಯ* Platform::ptr() ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ; }

/* static */
Platform& Platform::getDefault()
{
    ಸ್ಥಿರ Platform dummy;
    ಪ್ರತ್ಯಾಗಮ dummy;
}

ಶೂನ್ಯ attachContext(ಧ್ರುವ String& platformName, ಶೂನ್ಯ* platformID, ಶೂನ್ಯ* context, ಶೂನ್ಯ* deviceID) { OCL_NOT_AVAILABLE(); }
ಶೂನ್ಯ convertFromBuffer(ಶೂನ್ಯ* cl_mem_buffer, size_t step, ಪೂರ್ಣಾಙ್ಕ rows, ಪೂರ್ಣಾಙ್ಕ cols, ಪೂರ್ಣಾಙ್ಕ type, UMat& dst) { OCL_NOT_AVAILABLE(); }
ಶೂನ್ಯ convertFromImage(ಶೂನ್ಯ* cl_mem_image, UMat& dst) { OCL_NOT_AVAILABLE(); }

ಶೂನ್ಯ initializeContextFromHandle(Context& ctx, ಶೂನ್ಯ* platform, ಶೂನ್ಯ* context, ಶೂನ್ಯ* device) { OCL_NOT_AVAILABLE(); }

Queue::Queue() CV_NOEXCEPT : p(ರಿಕ್ತ) { }
Queue::Queue(ಧ್ರುವ Context& c, ಧ್ರುವ Device& d) : p(ರಿಕ್ತ) { OCL_NOT_AVAILABLE(); }
Queue::~Queue() { }
Queue::Queue(ಧ್ರುವ Queue& q) {}
Queue& Queue::operator=(ಧ್ರುವ Queue& q) { ಪ್ರತ್ಯಾಗಮ *ಏತತ್; }
Queue::Queue(Queue&&) CV_NOEXCEPT : p(ರಿಕ್ತ) { }
Queue& Queue::operator=(Queue&&) CV_NOEXCEPT { ಪ್ರತ್ಯಾಗಮ *ಏತತ್; }

ತಾರ್ಕಿಕ Queue::create(ಧ್ರುವ Context& c, ಧ್ರುವ Device& d) { OCL_NOT_AVAILABLE(); }
ಶೂನ್ಯ Queue::finish() {}
ಶೂನ್ಯ* Queue::ptr() ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ; }
/* static */
Queue& Queue::getDefault()
{
    ಸ್ಥಿರ Queue dummy;
    ಪ್ರತ್ಯಾಗಮ dummy;
}

/// @brief Returns OpenCL command queue with enable profiling mode support
ಧ್ರುವ Queue& Queue::getProfilingQueue() ಧ್ರುವ { OCL_NOT_AVAILABLE(); }


KernelArg::KernelArg() CV_NOEXCEPT
    : flags(0), m(0), obj(0), sz(0), wscale(1), iwscale(1)
{
}

KernelArg::KernelArg(ಪೂರ್ಣಾಙ್ಕ _flags, UMat* _m, ಪೂರ್ಣಾಙ್ಕ _wscale, ಪೂರ್ಣಾಙ್ಕ _iwscale, ಧ್ರುವ ಶೂನ್ಯ* _obj, size_t _sz)
    : flags(_flags), m(_m), obj(_obj), sz(_sz), wscale(_wscale), iwscale(_iwscale)
{
    OCL_NOT_AVAILABLE();
}

KernelArg KernelArg::Constant(ಧ್ರುವ Mat& m)
{
    OCL_NOT_AVAILABLE();
}


Kernel::Kernel() CV_NOEXCEPT : p(ರಿಕ್ತ) { }
Kernel::Kernel(ಧ್ರುವ ವರ್ಣ* kname, ಧ್ರುವ Program& prog) : p(ರಿಕ್ತ) { OCL_NOT_AVAILABLE(); }
Kernel::Kernel(ಧ್ರುವ ವರ್ಣ* kname, ಧ್ರುವ ProgramSource& prog, ಧ್ರುವ String& buildopts, String* errmsg) : p(ರಿಕ್ತ) { OCL_NOT_AVAILABLE(); }
Kernel::~Kernel() { }
Kernel::Kernel(ಧ್ರುವ Kernel& k) : p(ರಿಕ್ತ) { }
Kernel& Kernel::operator=(ಧ್ರುವ Kernel& k) { ಪ್ರತ್ಯಾಗಮ *ಏತತ್; }
Kernel::Kernel(Kernel&&) CV_NOEXCEPT : p(ರಿಕ್ತ) { }
Kernel& Kernel::operator=(Kernel&&) CV_NOEXCEPT { ಪ್ರತ್ಯಾಗಮ *ಏತತ್; }

ತಾರ್ಕಿಕ Kernel::empty() ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ; }
ತಾರ್ಕಿಕ Kernel::create(ಧ್ರುವ ವರ್ಣ* kname, ಧ್ರುವ Program& prog) { OCL_NOT_AVAILABLE(); }
ತಾರ್ಕಿಕ Kernel::create(ಧ್ರುವ ವರ್ಣ* kname, ಧ್ರುವ ProgramSource& prog, ಧ್ರುವ String& buildopts, String* errmsg) { OCL_NOT_AVAILABLE(); }

ಪೂರ್ಣಾಙ್ಕ Kernel::set(ಪೂರ್ಣಾಙ್ಕ i, ಧ್ರುವ ಶೂನ್ಯ* value, size_t sz) { OCL_NOT_AVAILABLE(); }
ಪೂರ್ಣಾಙ್ಕ Kernel::set(ಪೂರ್ಣಾಙ್ಕ i, ಧ್ರುವ Image2D& image2D) { OCL_NOT_AVAILABLE(); }
ಪೂರ್ಣಾಙ್ಕ Kernel::set(ಪೂರ್ಣಾಙ್ಕ i, ಧ್ರುವ UMat& m) { OCL_NOT_AVAILABLE(); }
ಪೂರ್ಣಾಙ್ಕ Kernel::set(ಪೂರ್ಣಾಙ್ಕ i, ಧ್ರುವ KernelArg& arg) { OCL_NOT_AVAILABLE(); }

ತಾರ್ಕಿಕ Kernel::run(ಪೂರ್ಣಾಙ್ಕ dims, size_t globalsize[], size_t localsize[], ತಾರ್ಕಿಕ ಸಮಕಾಲ, ಧ್ರುವ Queue& q) { OCL_NOT_AVAILABLE(); }
ತಾರ್ಕಿಕ Kernel::runTask(ತಾರ್ಕಿಕ ಸಮಕಾಲ, ಧ್ರುವ Queue& q) { OCL_NOT_AVAILABLE(); }

int64 Kernel::runProfiling(ಪೂರ್ಣಾಙ್ಕ dims, size_t globalsize[], size_t localsize[], ಧ್ರುವ Queue& q) { OCL_NOT_AVAILABLE(); }

size_t Kernel::workGroupSize() ಧ್ರುವ { OCL_NOT_AVAILABLE(); }
size_t Kernel::preferedWorkGroupSizeMultiple() ಧ್ರುವ { OCL_NOT_AVAILABLE(); }
ತಾರ್ಕಿಕ Kernel::compileWorkGroupSize(size_t wsz[]) ಧ್ರುವ { OCL_NOT_AVAILABLE(); }
size_t Kernel::localMemSize() ಧ್ರುವ { OCL_NOT_AVAILABLE(); }

ಶೂನ್ಯ* Kernel::ptr() ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ; }


Program::Program() CV_NOEXCEPT : p(ರಿಕ್ತ) { }
Program::Program(ಧ್ರುವ ProgramSource& src, ಧ್ರುವ String& buildflags, String& errmsg) : p(ರಿಕ್ತ) { OCL_NOT_AVAILABLE(); }
Program::Program(ಧ್ರುವ Program& prog) : p(ರಿಕ್ತ) { }
Program& Program::operator=(ಧ್ರುವ Program& prog) { ಪ್ರತ್ಯಾಗಮ *ಏತತ್; }
Program::Program(Program&&) CV_NOEXCEPT : p(ರಿಕ್ತ) { }
Program& Program::operator=(Program&&) CV_NOEXCEPT { ಪ್ರತ್ಯಾಗಮ *ಏತತ್; }
Program::~Program() { }

ತಾರ್ಕಿಕ Program::create(ಧ್ರುವ ProgramSource& src, ಧ್ರುವ String& buildflags, String& errmsg) { OCL_NOT_AVAILABLE(); }

ಶೂನ್ಯ* Program::ptr() ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ; }

ಶೂನ್ಯ Program::get_ದ್ವಿಮಾನ(std::vector<ವರ್ಣ>& binary) ಧ್ರುವ { OCL_NOT_AVAILABLE(); }

ತಾರ್ಕಿಕ Program::ಪಠ(ಧ್ರುವ String& buf, ಧ್ರುವ String& buildflags) { OCL_NOT_AVAILABLE(); }
ತಾರ್ಕಿಕ Program::ಲಿಖ(String& buf) ಧ್ರುವ { OCL_NOT_AVAILABLE(); }
ಧ್ರುವ ProgramSource& Program::source() ಧ್ರುವ { OCL_NOT_AVAILABLE(); }
String Program::getPrefix() ಧ್ರುವ { OCL_NOT_AVAILABLE(); }
/* static */ String Program::getPrefix(ಧ್ರುವ String& buildflags) { OCL_NOT_AVAILABLE(); }


ProgramSource::ProgramSource() CV_NOEXCEPT : p(ರಿಕ್ತ) { }
ProgramSource::ProgramSource(ಧ್ರುವ String& module, ಧ್ರುವ String& name, ಧ್ರುವ String& codeStr, ಧ್ರುವ String& codeHash) : p(ರಿಕ್ತ) { }
ProgramSource::ProgramSource(ಧ್ರುವ String& prog) : p(ರಿಕ್ತ) { }
ProgramSource::ProgramSource(ಧ್ರುವ ವರ್ಣ* prog) : p(ರಿಕ್ತ) { }
ProgramSource::~ProgramSource() { }
ProgramSource::ProgramSource(ಧ್ರುವ ProgramSource& prog) : p(ರಿಕ್ತ) { }
ProgramSource& ProgramSource::operator=(ಧ್ರುವ ProgramSource& prog) { ಪ್ರತ್ಯಾಗಮ *ಏತತ್; }
ProgramSource::ProgramSource(ProgramSource&&) CV_NOEXCEPT : p(ರಿಕ್ತ) { }
ProgramSource& ProgramSource::operator=(ProgramSource&&) CV_NOEXCEPT { ಪ್ರತ್ಯಾಗಮ *ಏತತ್; }

ಧ್ರುವ String& ProgramSource::source() ಧ್ರುವ { OCL_NOT_AVAILABLE(); }
ProgramSource::hash_t ProgramSource::hash() ಧ್ರುವ { OCL_NOT_AVAILABLE(); }

/* static */ ProgramSource ProgramSource::from_ದ್ವಿಮಾನ(ಧ್ರುವ String& module, ಧ್ರುವ String& name, ಧ್ರುವ unsigned ವರ್ಣ* binary, ಧ್ರುವ size_t size, ಧ್ರುವ cv::String& buildOptions) { OCL_NOT_AVAILABLE(); }
/* static */ ProgramSource ProgramSource::fromSPIR(ಧ್ರುವ String& module, ಧ್ರುವ String& name, ಧ್ರುವ unsigned ವರ್ಣ* binary, ಧ್ರುವ size_t size, ಧ್ರುವ cv::String& buildOptions) { OCL_NOT_AVAILABLE(); }


PlatformInfo::PlatformInfo() CV_NOEXCEPT : p(ರಿಕ್ತ) { }
PlatformInfo::PlatformInfo(ಶೂನ್ಯ* id) : p(ರಿಕ್ತ) { OCL_NOT_AVAILABLE(); }
PlatformInfo::~PlatformInfo() { }

PlatformInfo::PlatformInfo(ಧ್ರುವ PlatformInfo& i) : p(ರಿಕ್ತ) { }
PlatformInfo& PlatformInfo::operator=(ಧ್ರುವ PlatformInfo& i) { ಪ್ರತ್ಯಾಗಮ *ಏತತ್; }
PlatformInfo::PlatformInfo(PlatformInfo&&) CV_NOEXCEPT : p(ರಿಕ್ತ) { }
PlatformInfo& PlatformInfo::operator=(PlatformInfo&&) CV_NOEXCEPT { ಪ್ರತ್ಯಾಗಮ *ಏತತ್; }

String PlatformInfo::name() ಧ್ರುವ { OCL_NOT_AVAILABLE(); }
String PlatformInfo::vendor() ಧ್ರುವ { OCL_NOT_AVAILABLE(); }
String PlatformInfo::version() ಧ್ರುವ { OCL_NOT_AVAILABLE(); }
ಪೂರ್ಣಾಙ್ಕ PlatformInfo::device_ಸಂಖ್ಯೆ() ಧ್ರುವ { OCL_NOT_AVAILABLE(); }
ಶೂನ್ಯ PlatformInfo::getDevice(Device& device, ಪೂರ್ಣಾಙ್ಕ d) ಧ್ರುವ { OCL_NOT_AVAILABLE(); }

ಧ್ರುವ ವರ್ಣ* convertTypeStr(ಪೂರ್ಣಾಙ್ಕ sdepth, ಪೂರ್ಣಾಙ್ಕ ddepth, ಪೂರ್ಣಾಙ್ಕ cn, ವರ್ಣ* buf) { OCL_NOT_AVAILABLE(); }
ಧ್ರುವ ವರ್ಣ* typeToStr(ಪೂರ್ಣಾಙ್ಕ t) { OCL_NOT_AVAILABLE(); }
ಧ್ರುವ ವರ್ಣ* memopTypeToStr(ಪೂರ್ಣಾಙ್ಕ t) { OCL_NOT_AVAILABLE(); }
ಧ್ರುವ ವರ್ಣ* vecopTypeToStr(ಪೂರ್ಣಾಙ್ಕ t) { OCL_NOT_AVAILABLE(); }
ಧ್ರುವ ವರ್ಣ* getOpenCLErrorString(ಪೂರ್ಣಾಙ್ಕ errorCode) { OCL_NOT_AVAILABLE(); }
String kernelToStr(ಒಳಹರಿವು_Array _kernel, ಪೂರ್ಣಾಙ್ಕ ddepth, ಧ್ರುವ ವರ್ಣ* name) { OCL_NOT_AVAILABLE(); }
ಶೂನ್ಯ getPlatfomsInfo(std::vector<PlatformInfo>& platform_info) { OCL_NOT_AVAILABLE(); }


ಪೂರ್ಣಾಙ್ಕ predictOptimalVectorWidth(ಒಳಹರಿವು_Array src1, ಒಳಹರಿವು_Array src2, ಒಳಹರಿವು_Array src3,
        ಒಳಹರಿವು_Array src4, ಒಳಹರಿವು_Array src5, ಒಳಹರಿವು_Array src6,
        ಒಳಹರಿವು_Array src7, ಒಳಹರಿವು_Array src8, ಒಳಹರಿವು_Array src9,
        OclVectorStrategy strat)
{ OCL_NOT_AVAILABLE(); }

ಪೂರ್ಣಾಙ್ಕ checkOptimalVectorWidth(ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ *vectorWidths,
        ಒಳಹರಿವು_Array src1, ಒಳಹರಿವು_Array src2, ಒಳಹರಿವು_Array src3,
        ಒಳಹರಿವು_Array src4, ಒಳಹರಿವು_Array src5, ಒಳಹರಿವು_Array src6,
        ಒಳಹರಿವು_Array src7, ಒಳಹರಿವು_Array src8, ಒಳಹರಿವು_Array src9,
        OclVectorStrategy strat)
{ OCL_NOT_AVAILABLE(); }

ಪೂರ್ಣಾಙ್ಕ predictOptimalVectorWidthMax(ಒಳಹರಿವು_Array src1, ಒಳಹರಿವು_Array src2, ಒಳಹರಿವು_Array src3,
        ಒಳಹರಿವು_Array src4, ಒಳಹರಿವು_Array src5, ಒಳಹರಿವು_Array src6,
        ಒಳಹರಿವು_Array src7, ಒಳಹರಿವು_Array src8, ಒಳಹರಿವು_Array src9)
{ OCL_NOT_AVAILABLE(); }

ಶೂನ್ಯ buildOptionsAddMatrixDescription(String& buildOptions, ಧ್ರುವ String& name, ಒಳಹರಿವು_Array _m) { OCL_NOT_AVAILABLE(); }


Image2D::Image2D() CV_NOEXCEPT : p(ರಿಕ್ತ) { }
Image2D::Image2D(ಧ್ರುವ UMat &src, ತಾರ್ಕಿಕ norm, ತಾರ್ಕಿಕ alias) { OCL_NOT_AVAILABLE(); }
Image2D::Image2D(ಧ್ರುವ Image2D & i) : p(ರಿಕ್ತ) { OCL_NOT_AVAILABLE(); }
Image2D::~Image2D() { }
Image2D& Image2D::operator=(ಧ್ರುವ Image2D & i) { ಪ್ರತ್ಯಾಗಮ *ಏತತ್; }
Image2D::Image2D(Image2D&&) CV_NOEXCEPT : p(ರಿಕ್ತ) { }
Image2D& Image2D::operator=(Image2D&&) CV_NOEXCEPT { ಪ್ರತ್ಯಾಗಮ *ಏತತ್; }

/* static */ ತಾರ್ಕಿಕ Image2D::canCreateAlias(ಧ್ರುವ UMat &u) { OCL_NOT_AVAILABLE(); }
/* static */ ತಾರ್ಕಿಕ Image2D::isFormatSupported(ಪೂರ್ಣಾಙ್ಕ depth, ಪೂರ್ಣಾಙ್ಕ cn, ತಾರ್ಕಿಕ norm) { OCL_NOT_AVAILABLE(); }

ಶೂನ್ಯ* Image2D::ptr() ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ; }


Timer::Timer(ಧ್ರುವ Queue& q) : p(ರಿಕ್ತ) {}
Timer::~Timer() {}
ಶೂನ್ಯ Timer::start() { OCL_NOT_AVAILABLE(); }
ಶೂನ್ಯ Timer::stop() { OCL_NOT_AVAILABLE();}

uint64 Timer::durationNS() ಧ್ರುವ { OCL_NOT_AVAILABLE(); }

MatAllocator* getOpenCLAllocator() { ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ; }

internal::ProgramEntry::operator ProgramSource&() ಧ್ರುವ { OCL_NOT_AVAILABLE(); }


ಸಂರಚನಾ OpenCLExecutionContext::Impl
{
    Impl() = ಸ್ವಭಾವ;
};

Context& OpenCLExecutionContext::getContext() ಧ್ರುವ { OCL_NOT_AVAILABLE(); }
Device& OpenCLExecutionContext::getDevice() ಧ್ರುವ { OCL_NOT_AVAILABLE(); }
Queue& OpenCLExecutionContext::getQueue() ಧ್ರುವ { OCL_NOT_AVAILABLE(); }

ತಾರ್ಕಿಕ OpenCLExecutionContext::useOpenCL() ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ; }
ಶೂನ್ಯ OpenCLExecutionContext::setUseOpenCL(ತಾರ್ಕಿಕ flag) { }

ಸ್ಥಿರ
OpenCLExecutionContext& getDummyOpenCLExecutionContext()
{
    ಸ್ಥಿರ OpenCLExecutionContext dummy;
    ಪ್ರತ್ಯಾಗಮ dummy;
}

/* static */
OpenCLExecutionContext& OpenCLExecutionContext::getCurrent() { ಪ್ರತ್ಯಾಗಮ getDummyOpenCLExecutionContext(); }

/* static */
OpenCLExecutionContext& OpenCLExecutionContext::getCurrentRef() { ಪ್ರತ್ಯಾಗಮ getDummyOpenCLExecutionContext(); }

ಶೂನ್ಯ OpenCLExecutionContext::bind() ಧ್ರುವ { OCL_NOT_AVAILABLE(); }

OpenCLExecutionContext OpenCLExecutionContext::cloneWithNewQueue(ಧ್ರುವ ocl::Queue& q) ಧ್ರುವ { OCL_NOT_AVAILABLE(); }
OpenCLExecutionContext OpenCLExecutionContext::cloneWithNewQueue() ಧ್ರುವ { OCL_NOT_AVAILABLE(); }

/* static */ OpenCLExecutionContext OpenCLExecutionContext::create(ಧ್ರುವ std::string& platformName, ಶೂನ್ಯ* platformID, ಶೂನ್ಯ* context, ಶೂನ್ಯ* deviceID) { OCL_NOT_AVAILABLE(); }
/* static */ OpenCLExecutionContext OpenCLExecutionContext::create(ಧ್ರುವ Context& context, ಧ್ರುವ Device& device, ಧ್ರುವ ocl::Queue& queue) { OCL_NOT_AVAILABLE(); }
/* static */ OpenCLExecutionContext OpenCLExecutionContext::create(ಧ್ರುವ Context& context, ಧ್ರುವ Device& device) { OCL_NOT_AVAILABLE(); }

ಶೂನ್ಯ OpenCLExecutionContext::release() { }

}}

#ಯದಿ defined(_MSC_VER)
    #pragma warning(pop)
#ನೋಚೇದ್ಯದಿ defined(__clang__)
    #pragma clang diagnostic pop
#ನೋಚೇದ್ಯದಿ defined(__GNUC__)
    #pragma GCC diagnostic pop
#endif
