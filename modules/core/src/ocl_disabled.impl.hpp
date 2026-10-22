// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#अन्तर्भाव "opencv2/core/ocl_genbase.hpp"

#जर defined(_MSC_VER)
    #pragma warning(push)
    #pragma warning(disable : 4100)
    #pragma warning(disable : 4702)
#किंवाजर defined(__clang__)
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wunused-parameter"
#किंवाजर defined(__GNUC__)
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

CV_EXPORTS_W तार्किक haveOpenCL() { परत असत्य; }
CV_EXPORTS_W तार्किक useOpenCL() { परत असत्य; }
CV_EXPORTS_W तार्किक haveAmdBlas() { परत असत्य; }
CV_EXPORTS_W तार्किक haveAmdFft() { परत असत्य; }
CV_EXPORTS_W शून्य setUseOpenCL(तार्किक flag) { /* nothing */ }
CV_EXPORTS_W शून्य finish() { /* nothing */ }

CV_EXPORTS तार्किक haveSVM() { परत असत्य; }

Device::Device() CV_NOEXCEPT : p(रिक्त) { }
Device::Device(शून्य* d) : p(रिक्त) { OCL_NOT_AVAILABLE(); }
Device::Device(स्थिरांक Device& d) : p(रिक्त) { }
Device& Device::चालक=(स्थिरांक Device& d) { परत *हे; }
Device::Device(Device&&) CV_NOEXCEPT : p(रिक्त) { }
Device& Device::चालक=(Device&&) CV_NOEXCEPT { परत *हे; }
Device::~Device() { }

शून्य Device::set(शून्य* d) { OCL_NOT_AVAILABLE(); }

String Device::name() स्थिरांक { OCL_NOT_AVAILABLE(); }
String Device::extensions() स्थिरांक { OCL_NOT_AVAILABLE(); }
तार्किक Device::isExtensionSupported(स्थिरांक String& extensionName) स्थिरांक { OCL_NOT_AVAILABLE(); }
String Device::version() स्थिरांक { OCL_NOT_AVAILABLE(); }
String Device::vendorName() स्थिरांक { OCL_NOT_AVAILABLE(); }
String Device::OpenCL_C_Version() स्थिरांक { OCL_NOT_AVAILABLE(); }
String Device::OpenCLVersion() स्थिरांक { OCL_NOT_AVAILABLE(); }
पूर्णांक Device::deviceVersionMajor() स्थिरांक { OCL_NOT_AVAILABLE(); }
पूर्णांक Device::deviceVersionMinor() स्थिरांक { OCL_NOT_AVAILABLE(); }
String Device::driverVersion() स्थिरांक { OCL_NOT_AVAILABLE(); }
शून्य* Device::ptr() स्थिरांक { /*OCL_NOT_AVAILABLE();*/ परत रिक्त; }

पूर्णांक Device::type() स्थिरांक { OCL_NOT_AVAILABLE(); }

पूर्णांक Device::addressBits() स्थिरांक { OCL_NOT_AVAILABLE(); }
तार्किक Device::available() स्थिरांक { OCL_NOT_AVAILABLE(); }
तार्किक Device::compilerAvailable() स्थिरांक { OCL_NOT_AVAILABLE(); }
तार्किक Device::linkerAvailable() स्थिरांक { OCL_NOT_AVAILABLE(); }

पूर्णांक Device::doubleFPConfig() स्थिरांक { OCL_NOT_AVAILABLE(); }
पूर्णांक Device::singleFPConfig() स्थिरांक { OCL_NOT_AVAILABLE(); }
पूर्णांक Device::halfFPConfig() स्थिरांक { OCL_NOT_AVAILABLE(); }

तार्किक Device::hasFP64() स्थिरांक { OCL_NOT_AVAILABLE(); }
तार्किक Device::hasFP16() स्थिरांक { OCL_NOT_AVAILABLE(); }

तार्किक Device::endianLittle() स्थिरांक { OCL_NOT_AVAILABLE(); }
तार्किक Device::errorCorrectionSupport() स्थिरांक { OCL_NOT_AVAILABLE(); }

पूर्णांक Device::executionCapabilities() स्थिरांक { OCL_NOT_AVAILABLE(); }

माप_प्रकार Device::globalMemCacheSize() स्थिरांक { OCL_NOT_AVAILABLE(); }

पूर्णांक Device::globalMemCacheType() स्थिरांक { OCL_NOT_AVAILABLE(); }
पूर्णांक Device::global_Mem_Cache_ओळ_Size() स्थिरांक { OCL_NOT_AVAILABLE(); }
माप_प्रकार Device::globalMemSize() स्थिरांक { OCL_NOT_AVAILABLE(); }

माप_प्रकार Device::localMemSize() स्थिरांक { OCL_NOT_AVAILABLE(); }
पूर्णांक Device::localMemType() स्थिरांक { परत NO_LOCAL_MEM; }
तार्किक Device::hostUnifiedMemory() स्थिरांक { OCL_NOT_AVAILABLE(); }

तार्किक Device::imageSupport() स्थिरांक { OCL_NOT_AVAILABLE(); }

तार्किक Device::imageFromBufferSupport() स्थिरांक { OCL_NOT_AVAILABLE(); }
uint Device::imagePitchAlignment() स्थिरांक { OCL_NOT_AVAILABLE(); }
uint Device::imageBaseAddressAlignment() स्थिरांक { OCL_NOT_AVAILABLE(); }

तार्किक Device::intelSubgroupsSupport() स्थिरांक { OCL_NOT_AVAILABLE(); }

माप_प्रकार Device::image2DMaxWidth() स्थिरांक { OCL_NOT_AVAILABLE(); }
माप_प्रकार Device::image2DMaxHeight() स्थिरांक { OCL_NOT_AVAILABLE(); }

माप_प्रकार Device::image3DMaxWidth() स्थिरांक { OCL_NOT_AVAILABLE(); }
माप_प्रकार Device::image3DMaxHeight() स्थिरांक { OCL_NOT_AVAILABLE(); }
माप_प्रकार Device::image3DMaxDepth() स्थिरांक { OCL_NOT_AVAILABLE(); }

माप_प्रकार Device::imageMaxBufferSize() स्थिरांक { OCL_NOT_AVAILABLE(); }
माप_प्रकार Device::imageMaxArraySize() स्थिरांक { OCL_NOT_AVAILABLE(); }

पूर्णांक Device::vendorID() स्थिरांक { OCL_NOT_AVAILABLE(); }

पूर्णांक Device::maxClockFrequency() स्थिरांक { OCL_NOT_AVAILABLE(); }
पूर्णांक Device::maxComputeUnits() स्थिरांक { OCL_NOT_AVAILABLE(); }
पूर्णांक Device::maxConstantArgs() स्थिरांक { OCL_NOT_AVAILABLE(); }
माप_प्रकार Device::maxConstantBufferSize() स्थिरांक { OCL_NOT_AVAILABLE(); }

माप_प्रकार Device::maxMemAllocSize() स्थिरांक { OCL_NOT_AVAILABLE(); }
माप_प्रकार Device::maxParameterSize() स्थिरांक { OCL_NOT_AVAILABLE(); }

पूर्णांक Device::maxReadImageArgs() स्थिरांक { OCL_NOT_AVAILABLE(); }
पूर्णांक Device::maxWriteImageArgs() स्थिरांक { OCL_NOT_AVAILABLE(); }
पूर्णांक Device::maxSamplers() स्थिरांक { OCL_NOT_AVAILABLE(); }

माप_प्रकार Device::maxWorkGroupSize() स्थिरांक { OCL_NOT_AVAILABLE(); }
पूर्णांक Device::maxWorkItemDims() स्थिरांक { OCL_NOT_AVAILABLE(); }
शून्य Device::maxWorkItemSizes(माप_प्रकार*) स्थिरांक { OCL_NOT_AVAILABLE(); }

पूर्णांक Device::memBaseAddrAlign() स्थिरांक { OCL_NOT_AVAILABLE(); }

पूर्णांक Device::nativeVectorWidthChar() स्थिरांक { OCL_NOT_AVAILABLE(); }
पूर्णांक Device::nativeVectorWidthShort() स्थिरांक { OCL_NOT_AVAILABLE(); }
पूर्णांक Device::nativeVectorWidthInt() स्थिरांक { OCL_NOT_AVAILABLE(); }
पूर्णांक Device::nativeVectorWidthLong() स्थिरांक { OCL_NOT_AVAILABLE(); }
पूर्णांक Device::nativeVectorWidthFloat() स्थिरांक { OCL_NOT_AVAILABLE(); }
पूर्णांक Device::nativeVectorWidthDouble() स्थिरांक { OCL_NOT_AVAILABLE(); }
पूर्णांक Device::nativeVectorWidthHalf() स्थिरांक { OCL_NOT_AVAILABLE(); }

पूर्णांक Device::preferredVectorWidthChar() स्थिरांक { OCL_NOT_AVAILABLE(); }
पूर्णांक Device::preferredVectorWidthShort() स्थिरांक { OCL_NOT_AVAILABLE(); }
पूर्णांक Device::preferredVectorWidthInt() स्थिरांक { OCL_NOT_AVAILABLE(); }
पूर्णांक Device::preferredVectorWidthLong() स्थिरांक { OCL_NOT_AVAILABLE(); }
पूर्णांक Device::preferredVectorWidthFloat() स्थिरांक { OCL_NOT_AVAILABLE(); }
पूर्णांक Device::preferredVectorWidthDouble() स्थिरांक { OCL_NOT_AVAILABLE(); }
पूर्णांक Device::preferredVectorWidthHalf() स्थिरांक { OCL_NOT_AVAILABLE(); }

माप_प्रकार Device::printfBufferSize() स्थिरांक { OCL_NOT_AVAILABLE(); }
माप_प्रकार Device::profilingTimerResolution() स्थिरांक { OCL_NOT_AVAILABLE(); }

/* static */
स्थिरांक Device& Device::getDefault()
{
    स्थिर Device dummy;
    परत dummy;
}

/* static */ Device Device::fromHandle(शून्य* d) { OCL_NOT_AVAILABLE(); }


Context::Context() CV_NOEXCEPT : p(रिक्त) { }
Context::Context(पूर्णांक dtype) : p(रिक्त) { }
Context::~Context() { }
Context::Context(स्थिरांक Context& c) : p(रिक्त) { }
Context& Context::चालक=(स्थिरांक Context& c) { परत *हे; }
Context::Context(Context&&) CV_NOEXCEPT : p(रिक्त) { }
Context& Context::चालक=(Context&&) CV_NOEXCEPT { परत *हे; }

तार्किक Context::create() { परत असत्य; }
तार्किक Context::create(पूर्णांक dtype) { परत असत्य; }
माप_प्रकार Context::ndevices() स्थिरांक { परत 0; }
Device& Context::device(माप_प्रकार idx) स्थिरांक { OCL_NOT_AVAILABLE(); }
Program Context::getProg(स्थिरांक ProgramSource& prog, स्थिरांक String& buildopt, String& errmsg) { OCL_NOT_AVAILABLE(); }
शून्य Context::unloadProg(Program& prog) { }

/* static */
Context& Context::getDefault()
{
    स्थिर Context dummy;
    परत dummy;
}
शून्य* Context::ptr() स्थिरांक { परत रिक्त; }

शून्य* Context::getOpenCLContextProperty(पूर्णांक /*propertyId*/) स्थिरांक { OCL_NOT_AVAILABLE(); }

तार्किक Context::useSVM() स्थिरांक { परत असत्य; }
शून्य Context::setUseSVM(तार्किक enabled) { }

Context::UserContext::~UserContext() { }

शून्य Context::setUserContext(std::type_index /*typeId*/, स्थिरांक std::shared_ptr<Context::UserContext>& /*userContext*/) { OCL_NOT_AVAILABLE(); }
std::shared_ptr<Context::UserContext> Context::getUserContext(std::type_index /*typeId*/) { OCL_NOT_AVAILABLE(); }

/* static */ Context Context::fromHandle(शून्य* context) { OCL_NOT_AVAILABLE(); }
/* static */ Context Context::fromDevice(स्थिरांक ocl::Device& device) { OCL_NOT_AVAILABLE(); }
/* static */ Context Context::create(स्थिरांक std::string& configuration) { OCL_NOT_AVAILABLE(); }

शून्य Context::release() { }


Platform::Platform() CV_NOEXCEPT : p(रिक्त) { }
Platform::~Platform() { }
Platform::Platform(स्थिरांक Platform&) : p(रिक्त) { }
Platform& Platform::चालक=(स्थिरांक Platform&) { परत *हे; }
Platform::Platform(Platform&&) CV_NOEXCEPT : p(रिक्त) { }
Platform& Platform::चालक=(Platform&&) CV_NOEXCEPT { परत *हे; }

शून्य* Platform::ptr() स्थिरांक { परत रिक्त; }

/* static */
Platform& Platform::getDefault()
{
    स्थिर Platform dummy;
    परत dummy;
}

शून्य attachContext(स्थिरांक String& platformName, शून्य* platformID, शून्य* context, शून्य* deviceID) { OCL_NOT_AVAILABLE(); }
शून्य convertFromBuffer(शून्य* cl_mem_buffer, माप_प्रकार step, पूर्णांक rows, पूर्णांक cols, पूर्णांक type, UMat& dst) { OCL_NOT_AVAILABLE(); }
शून्य convertFromImage(शून्य* cl_mem_image, UMat& dst) { OCL_NOT_AVAILABLE(); }

शून्य initializeContextFromHandle(Context& ctx, शून्य* platform, शून्य* context, शून्य* device) { OCL_NOT_AVAILABLE(); }

Queue::Queue() CV_NOEXCEPT : p(रिक्त) { }
Queue::Queue(स्थिरांक Context& c, स्थिरांक Device& d) : p(रिक्त) { OCL_NOT_AVAILABLE(); }
Queue::~Queue() { }
Queue::Queue(स्थिरांक Queue& q) {}
Queue& Queue::चालक=(स्थिरांक Queue& q) { परत *हे; }
Queue::Queue(Queue&&) CV_NOEXCEPT : p(रिक्त) { }
Queue& Queue::चालक=(Queue&&) CV_NOEXCEPT { परत *हे; }

तार्किक Queue::create(स्थिरांक Context& c, स्थिरांक Device& d) { OCL_NOT_AVAILABLE(); }
शून्य Queue::finish() {}
शून्य* Queue::ptr() स्थिरांक { परत रिक्त; }
/* static */
Queue& Queue::getDefault()
{
    स्थिर Queue dummy;
    परत dummy;
}

/// @brief Returns OpenCL command queue with enable profiling mode support
स्थिरांक Queue& Queue::getProfilingQueue() स्थिरांक { OCL_NOT_AVAILABLE(); }


KernelArg::KernelArg() CV_NOEXCEPT
    : flags(0), m(0), obj(0), sz(0), wscale(1), iwscale(1)
{
}

KernelArg::KernelArg(पूर्णांक _flags, UMat* _m, पूर्णांक _wscale, पूर्णांक _iwscale, स्थिरांक शून्य* _obj, माप_प्रकार _sz)
    : flags(_flags), m(_m), obj(_obj), sz(_sz), wscale(_wscale), iwscale(_iwscale)
{
    OCL_NOT_AVAILABLE();
}

KernelArg KernelArg::Constant(स्थिरांक Mat& m)
{
    OCL_NOT_AVAILABLE();
}


Kernel::Kernel() CV_NOEXCEPT : p(रिक्त) { }
Kernel::Kernel(स्थिरांक अक्षर* kname, स्थिरांक Program& prog) : p(रिक्त) { OCL_NOT_AVAILABLE(); }
Kernel::Kernel(स्थिरांक अक्षर* kname, स्थिरांक ProgramSource& prog, स्थिरांक String& buildopts, String* errmsg) : p(रिक्त) { OCL_NOT_AVAILABLE(); }
Kernel::~Kernel() { }
Kernel::Kernel(स्थिरांक Kernel& k) : p(रिक्त) { }
Kernel& Kernel::चालक=(स्थिरांक Kernel& k) { परत *हे; }
Kernel::Kernel(Kernel&&) CV_NOEXCEPT : p(रिक्त) { }
Kernel& Kernel::चालक=(Kernel&&) CV_NOEXCEPT { परत *हे; }

तार्किक Kernel::empty() स्थिरांक { परत सत्य; }
तार्किक Kernel::create(स्थिरांक अक्षर* kname, स्थिरांक Program& prog) { OCL_NOT_AVAILABLE(); }
तार्किक Kernel::create(स्थिरांक अक्षर* kname, स्थिरांक ProgramSource& prog, स्थिरांक String& buildopts, String* errmsg) { OCL_NOT_AVAILABLE(); }

पूर्णांक Kernel::set(पूर्णांक i, स्थिरांक शून्य* value, माप_प्रकार sz) { OCL_NOT_AVAILABLE(); }
पूर्णांक Kernel::set(पूर्णांक i, स्थिरांक Image2D& image2D) { OCL_NOT_AVAILABLE(); }
पूर्णांक Kernel::set(पूर्णांक i, स्थिरांक UMat& m) { OCL_NOT_AVAILABLE(); }
पूर्णांक Kernel::set(पूर्णांक i, स्थिरांक KernelArg& arg) { OCL_NOT_AVAILABLE(); }

तार्किक Kernel::run(पूर्णांक dims, माप_प्रकार globalsize[], माप_प्रकार localsize[], तार्किक समकालिक, स्थिरांक Queue& q) { OCL_NOT_AVAILABLE(); }
तार्किक Kernel::runTask(तार्किक समकालिक, स्थिरांक Queue& q) { OCL_NOT_AVAILABLE(); }

int64 Kernel::runProfiling(पूर्णांक dims, माप_प्रकार globalsize[], माप_प्रकार localsize[], स्थिरांक Queue& q) { OCL_NOT_AVAILABLE(); }

माप_प्रकार Kernel::workGroupSize() स्थिरांक { OCL_NOT_AVAILABLE(); }
माप_प्रकार Kernel::preferedWorkGroupSizeMultiple() स्थिरांक { OCL_NOT_AVAILABLE(); }
तार्किक Kernel::compileWorkGroupSize(माप_प्रकार wsz[]) स्थिरांक { OCL_NOT_AVAILABLE(); }
माप_प्रकार Kernel::localMemSize() स्थिरांक { OCL_NOT_AVAILABLE(); }

शून्य* Kernel::ptr() स्थिरांक { परत रिक्त; }


Program::Program() CV_NOEXCEPT : p(रिक्त) { }
Program::Program(स्थिरांक ProgramSource& src, स्थिरांक String& buildflags, String& errmsg) : p(रिक्त) { OCL_NOT_AVAILABLE(); }
Program::Program(स्थिरांक Program& prog) : p(रिक्त) { }
Program& Program::चालक=(स्थिरांक Program& prog) { परत *हे; }
Program::Program(Program&&) CV_NOEXCEPT : p(रिक्त) { }
Program& Program::चालक=(Program&&) CV_NOEXCEPT { परत *हे; }
Program::~Program() { }

तार्किक Program::create(स्थिरांक ProgramSource& src, स्थिरांक String& buildflags, String& errmsg) { OCL_NOT_AVAILABLE(); }

शून्य* Program::ptr() स्थिरांक { परत रिक्त; }

शून्य Program::get_द्विमान(std::vector<अक्षर>& binary) स्थिरांक { OCL_NOT_AVAILABLE(); }

तार्किक Program::पठ(स्थिरांक String& buf, स्थिरांक String& buildflags) { OCL_NOT_AVAILABLE(); }
तार्किक Program::लिख(String& buf) स्थिरांक { OCL_NOT_AVAILABLE(); }
स्थिरांक ProgramSource& Program::source() स्थिरांक { OCL_NOT_AVAILABLE(); }
String Program::getPrefix() स्थिरांक { OCL_NOT_AVAILABLE(); }
/* static */ String Program::getPrefix(स्थिरांक String& buildflags) { OCL_NOT_AVAILABLE(); }


ProgramSource::ProgramSource() CV_NOEXCEPT : p(रिक्त) { }
ProgramSource::ProgramSource(स्थिरांक String& module, स्थिरांक String& name, स्थिरांक String& codeStr, स्थिरांक String& codeHash) : p(रिक्त) { }
ProgramSource::ProgramSource(स्थिरांक String& prog) : p(रिक्त) { }
ProgramSource::ProgramSource(स्थिरांक अक्षर* prog) : p(रिक्त) { }
ProgramSource::~ProgramSource() { }
ProgramSource::ProgramSource(स्थिरांक ProgramSource& prog) : p(रिक्त) { }
ProgramSource& ProgramSource::चालक=(स्थिरांक ProgramSource& prog) { परत *हे; }
ProgramSource::ProgramSource(ProgramSource&&) CV_NOEXCEPT : p(रिक्त) { }
ProgramSource& ProgramSource::चालक=(ProgramSource&&) CV_NOEXCEPT { परत *हे; }

स्थिरांक String& ProgramSource::source() स्थिरांक { OCL_NOT_AVAILABLE(); }
ProgramSource::hash_t ProgramSource::hash() स्थिरांक { OCL_NOT_AVAILABLE(); }

/* static */ ProgramSource ProgramSource::from_द्विमान(स्थिरांक String& module, स्थिरांक String& name, स्थिरांक अचिन्हित अक्षर* binary, स्थिरांक माप_प्रकार size, स्थिरांक cv::String& buildOptions) { OCL_NOT_AVAILABLE(); }
/* static */ ProgramSource ProgramSource::fromSPIR(स्थिरांक String& module, स्थिरांक String& name, स्थिरांक अचिन्हित अक्षर* binary, स्थिरांक माप_प्रकार size, स्थिरांक cv::String& buildOptions) { OCL_NOT_AVAILABLE(); }


PlatformInfo::PlatformInfo() CV_NOEXCEPT : p(रिक्त) { }
PlatformInfo::PlatformInfo(शून्य* id) : p(रिक्त) { OCL_NOT_AVAILABLE(); }
PlatformInfo::~PlatformInfo() { }

PlatformInfo::PlatformInfo(स्थिरांक PlatformInfo& i) : p(रिक्त) { }
PlatformInfo& PlatformInfo::चालक=(स्थिरांक PlatformInfo& i) { परत *हे; }
PlatformInfo::PlatformInfo(PlatformInfo&&) CV_NOEXCEPT : p(रिक्त) { }
PlatformInfo& PlatformInfo::चालक=(PlatformInfo&&) CV_NOEXCEPT { परत *हे; }

String PlatformInfo::name() स्थिरांक { OCL_NOT_AVAILABLE(); }
String PlatformInfo::vendor() स्थिरांक { OCL_NOT_AVAILABLE(); }
String PlatformInfo::version() स्थिरांक { OCL_NOT_AVAILABLE(); }
पूर्णांक PlatformInfo::device_संख्या() स्थिरांक { OCL_NOT_AVAILABLE(); }
शून्य PlatformInfo::getDevice(Device& device, पूर्णांक d) स्थिरांक { OCL_NOT_AVAILABLE(); }

स्थिरांक अक्षर* convertTypeStr(पूर्णांक sdepth, पूर्णांक ddepth, पूर्णांक cn, अक्षर* buf) { OCL_NOT_AVAILABLE(); }
स्थिरांक अक्षर* typeToStr(पूर्णांक t) { OCL_NOT_AVAILABLE(); }
स्थिरांक अक्षर* memopTypeToStr(पूर्णांक t) { OCL_NOT_AVAILABLE(); }
स्थिरांक अक्षर* vecopTypeToStr(पूर्णांक t) { OCL_NOT_AVAILABLE(); }
स्थिरांक अक्षर* getOpenCLErrorString(पूर्णांक errorCode) { OCL_NOT_AVAILABLE(); }
String kernelToStr(आदान_Array _kernel, पूर्णांक ddepth, स्थिरांक अक्षर* name) { OCL_NOT_AVAILABLE(); }
शून्य getPlatfomsInfo(std::vector<PlatformInfo>& platform_info) { OCL_NOT_AVAILABLE(); }


पूर्णांक predictOptimalVectorWidth(आदान_Array src1, आदान_Array src2, आदान_Array src3,
        आदान_Array src4, आदान_Array src5, आदान_Array src6,
        आदान_Array src7, आदान_Array src8, आदान_Array src9,
        OclVectorStrategy strat)
{ OCL_NOT_AVAILABLE(); }

पूर्णांक checkOptimalVectorWidth(स्थिरांक पूर्णांक *vectorWidths,
        आदान_Array src1, आदान_Array src2, आदान_Array src3,
        आदान_Array src4, आदान_Array src5, आदान_Array src6,
        आदान_Array src7, आदान_Array src8, आदान_Array src9,
        OclVectorStrategy strat)
{ OCL_NOT_AVAILABLE(); }

पूर्णांक predictOptimalVectorWidthMax(आदान_Array src1, आदान_Array src2, आदान_Array src3,
        आदान_Array src4, आदान_Array src5, आदान_Array src6,
        आदान_Array src7, आदान_Array src8, आदान_Array src9)
{ OCL_NOT_AVAILABLE(); }

शून्य buildOptionsAddMatrixDescription(String& buildOptions, स्थिरांक String& name, आदान_Array _m) { OCL_NOT_AVAILABLE(); }


Image2D::Image2D() CV_NOEXCEPT : p(रिक्त) { }
Image2D::Image2D(स्थिरांक UMat &src, तार्किक norm, तार्किक alias) { OCL_NOT_AVAILABLE(); }
Image2D::Image2D(स्थिरांक Image2D & i) : p(रिक्त) { OCL_NOT_AVAILABLE(); }
Image2D::~Image2D() { }
Image2D& Image2D::चालक=(स्थिरांक Image2D & i) { परत *हे; }
Image2D::Image2D(Image2D&&) CV_NOEXCEPT : p(रिक्त) { }
Image2D& Image2D::चालक=(Image2D&&) CV_NOEXCEPT { परत *हे; }

/* static */ तार्किक Image2D::canCreateAlias(स्थिरांक UMat &u) { OCL_NOT_AVAILABLE(); }
/* static */ तार्किक Image2D::isFormatSupported(पूर्णांक depth, पूर्णांक cn, तार्किक norm) { OCL_NOT_AVAILABLE(); }

शून्य* Image2D::ptr() स्थिरांक { परत रिक्त; }


Timer::Timer(स्थिरांक Queue& q) : p(रिक्त) {}
Timer::~Timer() {}
शून्य Timer::start() { OCL_NOT_AVAILABLE(); }
शून्य Timer::stop() { OCL_NOT_AVAILABLE();}

uint64 Timer::durationNS() स्थिरांक { OCL_NOT_AVAILABLE(); }

MatAllocator* getOpenCLAllocator() { परत रिक्त; }

internal::ProgramEntry::चालक ProgramSource&() स्थिरांक { OCL_NOT_AVAILABLE(); }


संरचना OpenCLExecutionContext::Impl
{
    Impl() = पूर्वनिर्धारित;
};

Context& OpenCLExecutionContext::getContext() स्थिरांक { OCL_NOT_AVAILABLE(); }
Device& OpenCLExecutionContext::getDevice() स्थिरांक { OCL_NOT_AVAILABLE(); }
Queue& OpenCLExecutionContext::getQueue() स्थिरांक { OCL_NOT_AVAILABLE(); }

तार्किक OpenCLExecutionContext::useOpenCL() स्थिरांक { परत असत्य; }
शून्य OpenCLExecutionContext::setUseOpenCL(तार्किक flag) { }

स्थिर
OpenCLExecutionContext& getDummyOpenCLExecutionContext()
{
    स्थिर OpenCLExecutionContext dummy;
    परत dummy;
}

/* static */
OpenCLExecutionContext& OpenCLExecutionContext::getCurrent() { परत getDummyOpenCLExecutionContext(); }

/* static */
OpenCLExecutionContext& OpenCLExecutionContext::getCurrentRef() { परत getDummyOpenCLExecutionContext(); }

शून्य OpenCLExecutionContext::bind() स्थिरांक { OCL_NOT_AVAILABLE(); }

OpenCLExecutionContext OpenCLExecutionContext::cloneWithNewQueue(स्थिरांक ocl::Queue& q) स्थिरांक { OCL_NOT_AVAILABLE(); }
OpenCLExecutionContext OpenCLExecutionContext::cloneWithNewQueue() स्थिरांक { OCL_NOT_AVAILABLE(); }

/* static */ OpenCLExecutionContext OpenCLExecutionContext::create(स्थिरांक std::string& platformName, शून्य* platformID, शून्य* context, शून्य* deviceID) { OCL_NOT_AVAILABLE(); }
/* static */ OpenCLExecutionContext OpenCLExecutionContext::create(स्थिरांक Context& context, स्थिरांक Device& device, स्थिरांक ocl::Queue& queue) { OCL_NOT_AVAILABLE(); }
/* static */ OpenCLExecutionContext OpenCLExecutionContext::create(स्थिरांक Context& context, स्थिरांक Device& device) { OCL_NOT_AVAILABLE(); }

शून्य OpenCLExecutionContext::release() { }

}}

#जर defined(_MSC_VER)
    #pragma warning(pop)
#किंवाजर defined(__clang__)
    #pragma clang diagnostic pop
#किंवाजर defined(__GNUC__)
    #pragma GCC diagnostic pop
#endif
