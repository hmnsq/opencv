// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#अन्तर्भाव "opencv2/core/ocl_genbase.hpp"

#यदि defined(_MSC_VER)
    #pragma warning(push)
    #pragma warning(disable : 4100)
    #pragma warning(disable : 4702)
#नोचेद्यदि defined(__clang__)
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wunused-parameter"
#नोचेद्यदि defined(__GNUC__)
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

नामाकाश cv { नामाकाश ocl {

स्थिर
CV_NORETURN शून्य throw_no_ocl()
{
    CV_Error(Error::OpenCLApiCallError, "OpenCV build without OpenCL support");
}
#define OCL_NOT_AVAILABLE() throw_no_ocl();

CV_EXPORTS_W तार्किक haveOpenCL() { प्रत्यागम असत्य; }
CV_EXPORTS_W तार्किक useOpenCL() { प्रत्यागम असत्य; }
CV_EXPORTS_W तार्किक haveAmdBlas() { प्रत्यागम असत्य; }
CV_EXPORTS_W तार्किक haveAmdFft() { प्रत्यागम असत्य; }
CV_EXPORTS_W शून्य setUseOpenCL(तार्किक flag) { /* nothing */ }
CV_EXPORTS_W शून्य finish() { /* nothing */ }

CV_EXPORTS तार्किक haveSVM() { प्रत्यागम असत्य; }

Device::Device() CV_NOEXCEPT : p(रिक्त) { }
Device::Device(शून्य* d) : p(रिक्त) { OCL_NOT_AVAILABLE(); }
Device::Device(ध्रुव Device& d) : p(रिक्त) { }
Device& Device::operator=(ध्रुव Device& d) { प्रत्यागम *एतत्; }
Device::Device(Device&&) CV_NOEXCEPT : p(रिक्त) { }
Device& Device::operator=(Device&&) CV_NOEXCEPT { प्रत्यागम *एतत्; }
Device::~Device() { }

शून्य Device::set(शून्य* d) { OCL_NOT_AVAILABLE(); }

String Device::name() ध्रुव { OCL_NOT_AVAILABLE(); }
String Device::extensions() ध्रुव { OCL_NOT_AVAILABLE(); }
तार्किक Device::isExtensionSupported(ध्रुव String& extensionName) ध्रुव { OCL_NOT_AVAILABLE(); }
String Device::version() ध्रुव { OCL_NOT_AVAILABLE(); }
String Device::vendorName() ध्रुव { OCL_NOT_AVAILABLE(); }
String Device::OpenCL_C_Version() ध्रुव { OCL_NOT_AVAILABLE(); }
String Device::OpenCLVersion() ध्रुव { OCL_NOT_AVAILABLE(); }
पूर्णाङ्क Device::deviceVersionMajor() ध्रुव { OCL_NOT_AVAILABLE(); }
पूर्णाङ्क Device::deviceVersionMinor() ध्रुव { OCL_NOT_AVAILABLE(); }
String Device::driverVersion() ध्रुव { OCL_NOT_AVAILABLE(); }
शून्य* Device::ptr() ध्रुव { /*OCL_NOT_AVAILABLE();*/ प्रत्यागम रिक्त; }

पूर्णाङ्क Device::type() ध्रुव { OCL_NOT_AVAILABLE(); }

पूर्णाङ्क Device::addressBits() ध्रुव { OCL_NOT_AVAILABLE(); }
तार्किक Device::available() ध्रुव { OCL_NOT_AVAILABLE(); }
तार्किक Device::compilerAvailable() ध्रुव { OCL_NOT_AVAILABLE(); }
तार्किक Device::linkerAvailable() ध्रुव { OCL_NOT_AVAILABLE(); }

पूर्णाङ्क Device::doubleFPConfig() ध्रुव { OCL_NOT_AVAILABLE(); }
पूर्णाङ्क Device::singleFPConfig() ध्रुव { OCL_NOT_AVAILABLE(); }
पूर्णाङ्क Device::halfFPConfig() ध्रुव { OCL_NOT_AVAILABLE(); }

तार्किक Device::hasFP64() ध्रुव { OCL_NOT_AVAILABLE(); }
तार्किक Device::hasFP16() ध्रुव { OCL_NOT_AVAILABLE(); }

तार्किक Device::endianLittle() ध्रुव { OCL_NOT_AVAILABLE(); }
तार्किक Device::errorCorrectionSupport() ध्रुव { OCL_NOT_AVAILABLE(); }

पूर्णाङ्क Device::executionCapabilities() ध्रुव { OCL_NOT_AVAILABLE(); }

size_t Device::globalMemCacheSize() ध्रुव { OCL_NOT_AVAILABLE(); }

पूर्णाङ्क Device::globalMemCacheType() ध्रुव { OCL_NOT_AVAILABLE(); }
पूर्णाङ्क Device::global_Mem_Cache_पंक्ति_Size() ध्रुव { OCL_NOT_AVAILABLE(); }
size_t Device::globalMemSize() ध्रुव { OCL_NOT_AVAILABLE(); }

size_t Device::localMemSize() ध्रुव { OCL_NOT_AVAILABLE(); }
पूर्णाङ्क Device::localMemType() ध्रुव { प्रत्यागम NO_LOCAL_MEM; }
तार्किक Device::hostUnifiedMemory() ध्रुव { OCL_NOT_AVAILABLE(); }

तार्किक Device::imageSupport() ध्रुव { OCL_NOT_AVAILABLE(); }

तार्किक Device::imageFromBufferSupport() ध्रुव { OCL_NOT_AVAILABLE(); }
uint Device::imagePitchAlignment() ध्रुव { OCL_NOT_AVAILABLE(); }
uint Device::imageBaseAddressAlignment() ध्रुव { OCL_NOT_AVAILABLE(); }

तार्किक Device::intelSubgroupsSupport() ध्रुव { OCL_NOT_AVAILABLE(); }

size_t Device::image2DMaxWidth() ध्रुव { OCL_NOT_AVAILABLE(); }
size_t Device::image2DMaxHeight() ध्रुव { OCL_NOT_AVAILABLE(); }

size_t Device::image3DMaxWidth() ध्रुव { OCL_NOT_AVAILABLE(); }
size_t Device::image3DMaxHeight() ध्रुव { OCL_NOT_AVAILABLE(); }
size_t Device::image3DMaxDepth() ध्रुव { OCL_NOT_AVAILABLE(); }

size_t Device::imageMaxBufferSize() ध्रुव { OCL_NOT_AVAILABLE(); }
size_t Device::imageMaxArraySize() ध्रुव { OCL_NOT_AVAILABLE(); }

पूर्णाङ्क Device::vendorID() ध्रुव { OCL_NOT_AVAILABLE(); }

पूर्णाङ्क Device::maxClockFrequency() ध्रुव { OCL_NOT_AVAILABLE(); }
पूर्णाङ्क Device::maxComputeUnits() ध्रुव { OCL_NOT_AVAILABLE(); }
पूर्णाङ्क Device::maxConstantArgs() ध्रुव { OCL_NOT_AVAILABLE(); }
size_t Device::maxConstantBufferSize() ध्रुव { OCL_NOT_AVAILABLE(); }

size_t Device::maxMemAllocSize() ध्रुव { OCL_NOT_AVAILABLE(); }
size_t Device::maxParameterSize() ध्रुव { OCL_NOT_AVAILABLE(); }

पूर्णाङ्क Device::maxReadImageArgs() ध्रुव { OCL_NOT_AVAILABLE(); }
पूर्णाङ्क Device::maxWriteImageArgs() ध्रुव { OCL_NOT_AVAILABLE(); }
पूर्णाङ्क Device::maxSamplers() ध्रुव { OCL_NOT_AVAILABLE(); }

size_t Device::maxWorkGroupSize() ध्रुव { OCL_NOT_AVAILABLE(); }
पूर्णाङ्क Device::maxWorkItemDims() ध्रुव { OCL_NOT_AVAILABLE(); }
शून्य Device::maxWorkItemSizes(size_t*) ध्रुव { OCL_NOT_AVAILABLE(); }

पूर्णाङ्क Device::memBaseAddrAlign() ध्रुव { OCL_NOT_AVAILABLE(); }

पूर्णाङ्क Device::nativeVectorWidthChar() ध्रुव { OCL_NOT_AVAILABLE(); }
पूर्णाङ्क Device::nativeVectorWidthShort() ध्रुव { OCL_NOT_AVAILABLE(); }
पूर्णाङ्क Device::nativeVectorWidthInt() ध्रुव { OCL_NOT_AVAILABLE(); }
पूर्णाङ्क Device::nativeVectorWidthLong() ध्रुव { OCL_NOT_AVAILABLE(); }
पूर्णाङ्क Device::nativeVectorWidthFloat() ध्रुव { OCL_NOT_AVAILABLE(); }
पूर्णाङ्क Device::nativeVectorWidthDouble() ध्रुव { OCL_NOT_AVAILABLE(); }
पूर्णाङ्क Device::nativeVectorWidthHalf() ध्रुव { OCL_NOT_AVAILABLE(); }

पूर्णाङ्क Device::preferredVectorWidthChar() ध्रुव { OCL_NOT_AVAILABLE(); }
पूर्णाङ्क Device::preferredVectorWidthShort() ध्रुव { OCL_NOT_AVAILABLE(); }
पूर्णाङ्क Device::preferredVectorWidthInt() ध्रुव { OCL_NOT_AVAILABLE(); }
पूर्णाङ्क Device::preferredVectorWidthLong() ध्रुव { OCL_NOT_AVAILABLE(); }
पूर्णाङ्क Device::preferredVectorWidthFloat() ध्रुव { OCL_NOT_AVAILABLE(); }
पूर्णाङ्क Device::preferredVectorWidthDouble() ध्रुव { OCL_NOT_AVAILABLE(); }
पूर्णाङ्क Device::preferredVectorWidthHalf() ध्रुव { OCL_NOT_AVAILABLE(); }

size_t Device::printfBufferSize() ध्रुव { OCL_NOT_AVAILABLE(); }
size_t Device::profilingTimerResolution() ध्रुव { OCL_NOT_AVAILABLE(); }

/* static */
ध्रुव Device& Device::getDefault()
{
    स्थिर Device dummy;
    प्रत्यागम dummy;
}

/* static */ Device Device::fromHandle(शून्य* d) { OCL_NOT_AVAILABLE(); }


Context::Context() CV_NOEXCEPT : p(रिक्त) { }
Context::Context(पूर्णाङ्क dtype) : p(रिक्त) { }
Context::~Context() { }
Context::Context(ध्रुव Context& c) : p(रिक्त) { }
Context& Context::operator=(ध्रुव Context& c) { प्रत्यागम *एतत्; }
Context::Context(Context&&) CV_NOEXCEPT : p(रिक्त) { }
Context& Context::operator=(Context&&) CV_NOEXCEPT { प्रत्यागम *एतत्; }

तार्किक Context::create() { प्रत्यागम असत्य; }
तार्किक Context::create(पूर्णाङ्क dtype) { प्रत्यागम असत्य; }
size_t Context::ndevices() ध्रुव { प्रत्यागम 0; }
Device& Context::device(size_t idx) ध्रुव { OCL_NOT_AVAILABLE(); }
Program Context::getProg(ध्रुव ProgramSource& prog, ध्रुव String& buildopt, String& errmsg) { OCL_NOT_AVAILABLE(); }
शून्य Context::unloadProg(Program& prog) { }

/* static */
Context& Context::getDefault()
{
    स्थिर Context dummy;
    प्रत्यागम dummy;
}
शून्य* Context::ptr() ध्रुव { प्रत्यागम रिक्त; }

शून्य* Context::getOpenCLContextProperty(पूर्णाङ्क /*propertyId*/) ध्रुव { OCL_NOT_AVAILABLE(); }

तार्किक Context::useSVM() ध्रुव { प्रत्यागम असत्य; }
शून्य Context::setUseSVM(तार्किक enabled) { }

Context::UserContext::~UserContext() { }

शून्य Context::setUserContext(std::type_index /*typeId*/, ध्रुव std::shared_ptr<Context::UserContext>& /*userContext*/) { OCL_NOT_AVAILABLE(); }
std::shared_ptr<Context::UserContext> Context::getUserContext(std::type_index /*typeId*/) { OCL_NOT_AVAILABLE(); }

/* static */ Context Context::fromHandle(शून्य* context) { OCL_NOT_AVAILABLE(); }
/* static */ Context Context::fromDevice(ध्रुव ocl::Device& device) { OCL_NOT_AVAILABLE(); }
/* static */ Context Context::create(ध्रुव std::string& configuration) { OCL_NOT_AVAILABLE(); }

शून्य Context::release() { }


Platform::Platform() CV_NOEXCEPT : p(रिक्त) { }
Platform::~Platform() { }
Platform::Platform(ध्रुव Platform&) : p(रिक्त) { }
Platform& Platform::operator=(ध्रुव Platform&) { प्रत्यागम *एतत्; }
Platform::Platform(Platform&&) CV_NOEXCEPT : p(रिक्त) { }
Platform& Platform::operator=(Platform&&) CV_NOEXCEPT { प्रत्यागम *एतत्; }

शून्य* Platform::ptr() ध्रुव { प्रत्यागम रिक्त; }

/* static */
Platform& Platform::getDefault()
{
    स्थिर Platform dummy;
    प्रत्यागम dummy;
}

शून्य attachContext(ध्रुव String& platformName, शून्य* platformID, शून्य* context, शून्य* deviceID) { OCL_NOT_AVAILABLE(); }
शून्य convertFromBuffer(शून्य* cl_mem_buffer, size_t step, पूर्णाङ्क rows, पूर्णाङ्क cols, पूर्णाङ्क type, UMat& dst) { OCL_NOT_AVAILABLE(); }
शून्य convertFromImage(शून्य* cl_mem_image, UMat& dst) { OCL_NOT_AVAILABLE(); }

शून्य initializeContextFromHandle(Context& ctx, शून्य* platform, शून्य* context, शून्य* device) { OCL_NOT_AVAILABLE(); }

Queue::Queue() CV_NOEXCEPT : p(रिक्त) { }
Queue::Queue(ध्रुव Context& c, ध्रुव Device& d) : p(रिक्त) { OCL_NOT_AVAILABLE(); }
Queue::~Queue() { }
Queue::Queue(ध्रुव Queue& q) {}
Queue& Queue::operator=(ध्रुव Queue& q) { प्रत्यागम *एतत्; }
Queue::Queue(Queue&&) CV_NOEXCEPT : p(रिक्त) { }
Queue& Queue::operator=(Queue&&) CV_NOEXCEPT { प्रत्यागम *एतत्; }

तार्किक Queue::create(ध्रुव Context& c, ध्रुव Device& d) { OCL_NOT_AVAILABLE(); }
शून्य Queue::finish() {}
शून्य* Queue::ptr() ध्रुव { प्रत्यागम रिक्त; }
/* static */
Queue& Queue::getDefault()
{
    स्थिर Queue dummy;
    प्रत्यागम dummy;
}

/// @brief Returns OpenCL command queue with enable profiling mode support
ध्रुव Queue& Queue::getProfilingQueue() ध्रुव { OCL_NOT_AVAILABLE(); }


KernelArg::KernelArg() CV_NOEXCEPT
    : flags(0), m(0), obj(0), sz(0), wscale(1), iwscale(1)
{
}

KernelArg::KernelArg(पूर्णाङ्क _flags, UMat* _m, पूर्णाङ्क _wscale, पूर्णाङ्क _iwscale, ध्रुव शून्य* _obj, size_t _sz)
    : flags(_flags), m(_m), obj(_obj), sz(_sz), wscale(_wscale), iwscale(_iwscale)
{
    OCL_NOT_AVAILABLE();
}

KernelArg KernelArg::Constant(ध्रुव Mat& m)
{
    OCL_NOT_AVAILABLE();
}


Kernel::Kernel() CV_NOEXCEPT : p(रिक्त) { }
Kernel::Kernel(ध्रुव वर्ण* kname, ध्रुव Program& prog) : p(रिक्त) { OCL_NOT_AVAILABLE(); }
Kernel::Kernel(ध्रुव वर्ण* kname, ध्रुव ProgramSource& prog, ध्रुव String& buildopts, String* errmsg) : p(रिक्त) { OCL_NOT_AVAILABLE(); }
Kernel::~Kernel() { }
Kernel::Kernel(ध्रुव Kernel& k) : p(रिक्त) { }
Kernel& Kernel::operator=(ध्रुव Kernel& k) { प्रत्यागम *एतत्; }
Kernel::Kernel(Kernel&&) CV_NOEXCEPT : p(रिक्त) { }
Kernel& Kernel::operator=(Kernel&&) CV_NOEXCEPT { प्रत्यागम *एतत्; }

तार्किक Kernel::empty() ध्रुव { प्रत्यागम सत्य; }
तार्किक Kernel::create(ध्रुव वर्ण* kname, ध्रुव Program& prog) { OCL_NOT_AVAILABLE(); }
तार्किक Kernel::create(ध्रुव वर्ण* kname, ध्रुव ProgramSource& prog, ध्रुव String& buildopts, String* errmsg) { OCL_NOT_AVAILABLE(); }

पूर्णाङ्क Kernel::set(पूर्णाङ्क i, ध्रुव शून्य* value, size_t sz) { OCL_NOT_AVAILABLE(); }
पूर्णाङ्क Kernel::set(पूर्णाङ्क i, ध्रुव Image2D& image2D) { OCL_NOT_AVAILABLE(); }
पूर्णाङ्क Kernel::set(पूर्णाङ्क i, ध्रुव UMat& m) { OCL_NOT_AVAILABLE(); }
पूर्णाङ्क Kernel::set(पूर्णाङ्क i, ध्रुव KernelArg& arg) { OCL_NOT_AVAILABLE(); }

तार्किक Kernel::run(पूर्णाङ्क dims, size_t globalsize[], size_t localsize[], तार्किक समकाल, ध्रुव Queue& q) { OCL_NOT_AVAILABLE(); }
तार्किक Kernel::runTask(तार्किक समकाल, ध्रुव Queue& q) { OCL_NOT_AVAILABLE(); }

int64 Kernel::runProfiling(पूर्णाङ्क dims, size_t globalsize[], size_t localsize[], ध्रुव Queue& q) { OCL_NOT_AVAILABLE(); }

size_t Kernel::workGroupSize() ध्रुव { OCL_NOT_AVAILABLE(); }
size_t Kernel::preferedWorkGroupSizeMultiple() ध्रुव { OCL_NOT_AVAILABLE(); }
तार्किक Kernel::compileWorkGroupSize(size_t wsz[]) ध्रुव { OCL_NOT_AVAILABLE(); }
size_t Kernel::localMemSize() ध्रुव { OCL_NOT_AVAILABLE(); }

शून्य* Kernel::ptr() ध्रुव { प्रत्यागम रिक्त; }


Program::Program() CV_NOEXCEPT : p(रिक्त) { }
Program::Program(ध्रुव ProgramSource& src, ध्रुव String& buildflags, String& errmsg) : p(रिक्त) { OCL_NOT_AVAILABLE(); }
Program::Program(ध्रुव Program& prog) : p(रिक्त) { }
Program& Program::operator=(ध्रुव Program& prog) { प्रत्यागम *एतत्; }
Program::Program(Program&&) CV_NOEXCEPT : p(रिक्त) { }
Program& Program::operator=(Program&&) CV_NOEXCEPT { प्रत्यागम *एतत्; }
Program::~Program() { }

तार्किक Program::create(ध्रुव ProgramSource& src, ध्रुव String& buildflags, String& errmsg) { OCL_NOT_AVAILABLE(); }

शून्य* Program::ptr() ध्रुव { प्रत्यागम रिक्त; }

शून्य Program::get_द्विआधारी(std::vector<वर्ण>& binary) ध्रुव { OCL_NOT_AVAILABLE(); }

तार्किक Program::पठ(ध्रुव String& buf, ध्रुव String& buildflags) { OCL_NOT_AVAILABLE(); }
तार्किक Program::लिख(String& buf) ध्रुव { OCL_NOT_AVAILABLE(); }
ध्रुव ProgramSource& Program::source() ध्रुव { OCL_NOT_AVAILABLE(); }
String Program::getPrefix() ध्रुव { OCL_NOT_AVAILABLE(); }
/* static */ String Program::getPrefix(ध्रुव String& buildflags) { OCL_NOT_AVAILABLE(); }


ProgramSource::ProgramSource() CV_NOEXCEPT : p(रिक्त) { }
ProgramSource::ProgramSource(ध्रुव String& module, ध्रुव String& name, ध्रुव String& codeStr, ध्रुव String& codeHash) : p(रिक्त) { }
ProgramSource::ProgramSource(ध्रुव String& prog) : p(रिक्त) { }
ProgramSource::ProgramSource(ध्रुव वर्ण* prog) : p(रिक्त) { }
ProgramSource::~ProgramSource() { }
ProgramSource::ProgramSource(ध्रुव ProgramSource& prog) : p(रिक्त) { }
ProgramSource& ProgramSource::operator=(ध्रुव ProgramSource& prog) { प्रत्यागम *एतत्; }
ProgramSource::ProgramSource(ProgramSource&&) CV_NOEXCEPT : p(रिक्त) { }
ProgramSource& ProgramSource::operator=(ProgramSource&&) CV_NOEXCEPT { प्रत्यागम *एतत्; }

ध्रुव String& ProgramSource::source() ध्रुव { OCL_NOT_AVAILABLE(); }
ProgramSource::hash_t ProgramSource::hash() ध्रुव { OCL_NOT_AVAILABLE(); }

/* static */ ProgramSource ProgramSource::from_द्विआधारी(ध्रुव String& module, ध्रुव String& name, ध्रुव unsigned वर्ण* binary, ध्रुव size_t size, ध्रुव cv::String& buildOptions) { OCL_NOT_AVAILABLE(); }
/* static */ ProgramSource ProgramSource::fromSPIR(ध्रुव String& module, ध्रुव String& name, ध्रुव unsigned वर्ण* binary, ध्रुव size_t size, ध्रुव cv::String& buildOptions) { OCL_NOT_AVAILABLE(); }


PlatformInfo::PlatformInfo() CV_NOEXCEPT : p(रिक्त) { }
PlatformInfo::PlatformInfo(शून्य* id) : p(रिक्त) { OCL_NOT_AVAILABLE(); }
PlatformInfo::~PlatformInfo() { }

PlatformInfo::PlatformInfo(ध्रुव PlatformInfo& i) : p(रिक्त) { }
PlatformInfo& PlatformInfo::operator=(ध्रुव PlatformInfo& i) { प्रत्यागम *एतत्; }
PlatformInfo::PlatformInfo(PlatformInfo&&) CV_NOEXCEPT : p(रिक्त) { }
PlatformInfo& PlatformInfo::operator=(PlatformInfo&&) CV_NOEXCEPT { प्रत्यागम *एतत्; }

String PlatformInfo::name() ध्रुव { OCL_NOT_AVAILABLE(); }
String PlatformInfo::vendor() ध्रुव { OCL_NOT_AVAILABLE(); }
String PlatformInfo::version() ध्रुव { OCL_NOT_AVAILABLE(); }
पूर्णाङ्क PlatformInfo::device_संख्या() ध्रुव { OCL_NOT_AVAILABLE(); }
शून्य PlatformInfo::getDevice(Device& device, पूर्णाङ्क d) ध्रुव { OCL_NOT_AVAILABLE(); }

ध्रुव वर्ण* convertTypeStr(पूर्णाङ्क sdepth, पूर्णाङ्क ddepth, पूर्णाङ्क cn, वर्ण* buf) { OCL_NOT_AVAILABLE(); }
ध्रुव वर्ण* typeToStr(पूर्णाङ्क t) { OCL_NOT_AVAILABLE(); }
ध्रुव वर्ण* memopTypeToStr(पूर्णाङ्क t) { OCL_NOT_AVAILABLE(); }
ध्रुव वर्ण* vecopTypeToStr(पूर्णाङ्क t) { OCL_NOT_AVAILABLE(); }
ध्रुव वर्ण* getOpenCLErrorString(पूर्णाङ्क errorCode) { OCL_NOT_AVAILABLE(); }
String kernelToStr(निवेश_Array _kernel, पूर्णाङ्क ddepth, ध्रुव वर्ण* name) { OCL_NOT_AVAILABLE(); }
शून्य getPlatfomsInfo(std::vector<PlatformInfo>& platform_info) { OCL_NOT_AVAILABLE(); }


पूर्णाङ्क predictOptimalVectorWidth(निवेश_Array src1, निवेश_Array src2, निवेश_Array src3,
        निवेश_Array src4, निवेश_Array src5, निवेश_Array src6,
        निवेश_Array src7, निवेश_Array src8, निवेश_Array src9,
        OclVectorStrategy strat)
{ OCL_NOT_AVAILABLE(); }

पूर्णाङ्क checkOptimalVectorWidth(ध्रुव पूर्णाङ्क *vectorWidths,
        निवेश_Array src1, निवेश_Array src2, निवेश_Array src3,
        निवेश_Array src4, निवेश_Array src5, निवेश_Array src6,
        निवेश_Array src7, निवेश_Array src8, निवेश_Array src9,
        OclVectorStrategy strat)
{ OCL_NOT_AVAILABLE(); }

पूर्णाङ्क predictOptimalVectorWidthMax(निवेश_Array src1, निवेश_Array src2, निवेश_Array src3,
        निवेश_Array src4, निवेश_Array src5, निवेश_Array src6,
        निवेश_Array src7, निवेश_Array src8, निवेश_Array src9)
{ OCL_NOT_AVAILABLE(); }

शून्य buildOptionsAddMatrixDescription(String& buildOptions, ध्रुव String& name, निवेश_Array _m) { OCL_NOT_AVAILABLE(); }


Image2D::Image2D() CV_NOEXCEPT : p(रिक्त) { }
Image2D::Image2D(ध्रुव UMat &src, तार्किक norm, तार्किक alias) { OCL_NOT_AVAILABLE(); }
Image2D::Image2D(ध्रुव Image2D & i) : p(रिक्त) { OCL_NOT_AVAILABLE(); }
Image2D::~Image2D() { }
Image2D& Image2D::operator=(ध्रुव Image2D & i) { प्रत्यागम *एतत्; }
Image2D::Image2D(Image2D&&) CV_NOEXCEPT : p(रिक्त) { }
Image2D& Image2D::operator=(Image2D&&) CV_NOEXCEPT { प्रत्यागम *एतत्; }

/* static */ तार्किक Image2D::canCreateAlias(ध्रुव UMat &u) { OCL_NOT_AVAILABLE(); }
/* static */ तार्किक Image2D::isFormatSupported(पूर्णाङ्क depth, पूर्णाङ्क cn, तार्किक norm) { OCL_NOT_AVAILABLE(); }

शून्य* Image2D::ptr() ध्रुव { प्रत्यागम रिक्त; }


Timer::Timer(ध्रुव Queue& q) : p(रिक्त) {}
Timer::~Timer() {}
शून्य Timer::start() { OCL_NOT_AVAILABLE(); }
शून्य Timer::stop() { OCL_NOT_AVAILABLE();}

uint64 Timer::durationNS() ध्रुव { OCL_NOT_AVAILABLE(); }

MatAllocator* getOpenCLAllocator() { प्रत्यागम रिक्त; }

internal::ProgramEntry::operator ProgramSource&() ध्रुव { OCL_NOT_AVAILABLE(); }


संरचना OpenCLExecutionContext::Impl
{
    Impl() = स्वभाव;
};

Context& OpenCLExecutionContext::getContext() ध्रुव { OCL_NOT_AVAILABLE(); }
Device& OpenCLExecutionContext::getDevice() ध्रुव { OCL_NOT_AVAILABLE(); }
Queue& OpenCLExecutionContext::getQueue() ध्रुव { OCL_NOT_AVAILABLE(); }

तार्किक OpenCLExecutionContext::useOpenCL() ध्रुव { प्रत्यागम असत्य; }
शून्य OpenCLExecutionContext::setUseOpenCL(तार्किक flag) { }

स्थिर
OpenCLExecutionContext& getDummyOpenCLExecutionContext()
{
    स्थिर OpenCLExecutionContext dummy;
    प्रत्यागम dummy;
}

/* static */
OpenCLExecutionContext& OpenCLExecutionContext::getCurrent() { प्रत्यागम getDummyOpenCLExecutionContext(); }

/* static */
OpenCLExecutionContext& OpenCLExecutionContext::getCurrentRef() { प्रत्यागम getDummyOpenCLExecutionContext(); }

शून्य OpenCLExecutionContext::bind() ध्रुव { OCL_NOT_AVAILABLE(); }

OpenCLExecutionContext OpenCLExecutionContext::cloneWithNewQueue(ध्रुव ocl::Queue& q) ध्रुव { OCL_NOT_AVAILABLE(); }
OpenCLExecutionContext OpenCLExecutionContext::cloneWithNewQueue() ध्रुव { OCL_NOT_AVAILABLE(); }

/* static */ OpenCLExecutionContext OpenCLExecutionContext::create(ध्रुव std::string& platformName, शून्य* platformID, शून्य* context, शून्य* deviceID) { OCL_NOT_AVAILABLE(); }
/* static */ OpenCLExecutionContext OpenCLExecutionContext::create(ध्रुव Context& context, ध्रुव Device& device, ध्रुव ocl::Queue& queue) { OCL_NOT_AVAILABLE(); }
/* static */ OpenCLExecutionContext OpenCLExecutionContext::create(ध्रुव Context& context, ध्रुव Device& device) { OCL_NOT_AVAILABLE(); }

शून्य OpenCLExecutionContext::release() { }

}}

#यदि defined(_MSC_VER)
    #pragma warning(pop)
#नोचेद्यदि defined(__clang__)
    #pragma clang diagnostic pop
#नोचेद्यदि defined(__GNUC__)
    #pragma GCC diagnostic pop
#endif
