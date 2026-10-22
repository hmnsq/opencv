// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#अन्तर्भाव "opencv2/core/ocl_genbase.hpp"

#यदि defined(_MSC_VER)
    #pragma warning(push)
    #pragma warning(disable : 4100)
    #pragma warning(disable : 4702)
#अथवायदि defined(__clang__)
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wunused-parameter"
#अथवायदि defined(__GNUC__)
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

CV_EXPORTS_W तार्किक haveOpenCL() { फर्काऊ असत्य; }
CV_EXPORTS_W तार्किक useOpenCL() { फर्काऊ असत्य; }
CV_EXPORTS_W तार्किक haveAmdBlas() { फर्काऊ असत्य; }
CV_EXPORTS_W तार्किक haveAmdFft() { फर्काऊ असत्य; }
CV_EXPORTS_W शून्य setUseOpenCL(तार्किक flag) { /* nothing */ }
CV_EXPORTS_W शून्य finish() { /* nothing */ }

CV_EXPORTS तार्किक haveSVM() { फर्काऊ असत्य; }

Device::Device() CV_NOEXCEPT : p(रिक्त) { }
Device::Device(शून्य* d) : p(रिक्त) { OCL_NOT_AVAILABLE(); }
Device::Device(अचर Device& d) : p(रिक्त) { }
Device& Device::चालक=(अचर Device& d) { फर्काऊ *एतत्; }
Device::Device(Device&&) CV_NOEXCEPT : p(रिक्त) { }
Device& Device::चालक=(Device&&) CV_NOEXCEPT { फर्काऊ *एतत्; }
Device::~Device() { }

शून्य Device::set(शून्य* d) { OCL_NOT_AVAILABLE(); }

String Device::name() अचर { OCL_NOT_AVAILABLE(); }
String Device::extensions() अचर { OCL_NOT_AVAILABLE(); }
तार्किक Device::isExtensionSupported(अचर String& extensionName) अचर { OCL_NOT_AVAILABLE(); }
String Device::version() अचर { OCL_NOT_AVAILABLE(); }
String Device::vendorName() अचर { OCL_NOT_AVAILABLE(); }
String Device::OpenCL_C_Version() अचर { OCL_NOT_AVAILABLE(); }
String Device::OpenCLVersion() अचर { OCL_NOT_AVAILABLE(); }
पूर्णाङ्क Device::deviceVersionMajor() अचर { OCL_NOT_AVAILABLE(); }
पूर्णाङ्क Device::deviceVersionMinor() अचर { OCL_NOT_AVAILABLE(); }
String Device::driverVersion() अचर { OCL_NOT_AVAILABLE(); }
शून्य* Device::ptr() अचर { /*OCL_NOT_AVAILABLE();*/ फर्काऊ रिक्त; }

पूर्णाङ्क Device::type() अचर { OCL_NOT_AVAILABLE(); }

पूर्णाङ्क Device::addressBits() अचर { OCL_NOT_AVAILABLE(); }
तार्किक Device::available() अचर { OCL_NOT_AVAILABLE(); }
तार्किक Device::compilerAvailable() अचर { OCL_NOT_AVAILABLE(); }
तार्किक Device::linkerAvailable() अचर { OCL_NOT_AVAILABLE(); }

पूर्णाङ्क Device::doubleFPConfig() अचर { OCL_NOT_AVAILABLE(); }
पूर्णाङ्क Device::singleFPConfig() अचर { OCL_NOT_AVAILABLE(); }
पूर्णाङ्क Device::halfFPConfig() अचर { OCL_NOT_AVAILABLE(); }

तार्किक Device::hasFP64() अचर { OCL_NOT_AVAILABLE(); }
तार्किक Device::hasFP16() अचर { OCL_NOT_AVAILABLE(); }

तार्किक Device::endianLittle() अचर { OCL_NOT_AVAILABLE(); }
तार्किक Device::errorCorrectionSupport() अचर { OCL_NOT_AVAILABLE(); }

पूर्णाङ्क Device::executionCapabilities() अचर { OCL_NOT_AVAILABLE(); }

माप_प्रकार Device::globalMemCacheSize() अचर { OCL_NOT_AVAILABLE(); }

पूर्णाङ्क Device::globalMemCacheType() अचर { OCL_NOT_AVAILABLE(); }
पूर्णाङ्क Device::global_Mem_Cache_पङ्क्ति_Size() अचर { OCL_NOT_AVAILABLE(); }
माप_प्रकार Device::globalMemSize() अचर { OCL_NOT_AVAILABLE(); }

माप_प्रकार Device::localMemSize() अचर { OCL_NOT_AVAILABLE(); }
पूर्णाङ्क Device::localMemType() अचर { फर्काऊ NO_LOCAL_MEM; }
तार्किक Device::hostUnifiedMemory() अचर { OCL_NOT_AVAILABLE(); }

तार्किक Device::imageSupport() अचर { OCL_NOT_AVAILABLE(); }

तार्किक Device::imageFromBufferSupport() अचर { OCL_NOT_AVAILABLE(); }
uint Device::imagePitchAlignment() अचर { OCL_NOT_AVAILABLE(); }
uint Device::imageBaseAddressAlignment() अचर { OCL_NOT_AVAILABLE(); }

तार्किक Device::intelSubgroupsSupport() अचर { OCL_NOT_AVAILABLE(); }

माप_प्रकार Device::image2DMaxWidth() अचर { OCL_NOT_AVAILABLE(); }
माप_प्रकार Device::image2DMaxHeight() अचर { OCL_NOT_AVAILABLE(); }

माप_प्रकार Device::image3DMaxWidth() अचर { OCL_NOT_AVAILABLE(); }
माप_प्रकार Device::image3DMaxHeight() अचर { OCL_NOT_AVAILABLE(); }
माप_प्रकार Device::image3DMaxDepth() अचर { OCL_NOT_AVAILABLE(); }

माप_प्रकार Device::imageMaxBufferSize() अचर { OCL_NOT_AVAILABLE(); }
माप_प्रकार Device::imageMaxArraySize() अचर { OCL_NOT_AVAILABLE(); }

पूर्णाङ्क Device::vendorID() अचर { OCL_NOT_AVAILABLE(); }

पूर्णाङ्क Device::maxClockFrequency() अचर { OCL_NOT_AVAILABLE(); }
पूर्णाङ्क Device::maxComputeUnits() अचर { OCL_NOT_AVAILABLE(); }
पूर्णाङ्क Device::maxConstantArgs() अचर { OCL_NOT_AVAILABLE(); }
माप_प्रकार Device::maxConstantBufferSize() अचर { OCL_NOT_AVAILABLE(); }

माप_प्रकार Device::maxMemAllocSize() अचर { OCL_NOT_AVAILABLE(); }
माप_प्रकार Device::maxParameterSize() अचर { OCL_NOT_AVAILABLE(); }

पूर्णाङ्क Device::maxReadImageArgs() अचर { OCL_NOT_AVAILABLE(); }
पूर्णाङ्क Device::maxWriteImageArgs() अचर { OCL_NOT_AVAILABLE(); }
पूर्णाङ्क Device::maxSamplers() अचर { OCL_NOT_AVAILABLE(); }

माप_प्रकार Device::maxWorkGroupSize() अचर { OCL_NOT_AVAILABLE(); }
पूर्णाङ्क Device::maxWorkItemDims() अचर { OCL_NOT_AVAILABLE(); }
शून्य Device::maxWorkItemSizes(माप_प्रकार*) अचर { OCL_NOT_AVAILABLE(); }

पूर्णाङ्क Device::memBaseAddrAlign() अचर { OCL_NOT_AVAILABLE(); }

पूर्णाङ्क Device::nativeVectorWidthChar() अचर { OCL_NOT_AVAILABLE(); }
पूर्णाङ्क Device::nativeVectorWidthShort() अचर { OCL_NOT_AVAILABLE(); }
पूर्णाङ्क Device::nativeVectorWidthInt() अचर { OCL_NOT_AVAILABLE(); }
पूर्णाङ्क Device::nativeVectorWidthLong() अचर { OCL_NOT_AVAILABLE(); }
पूर्णाङ्क Device::nativeVectorWidthFloat() अचर { OCL_NOT_AVAILABLE(); }
पूर्णाङ्क Device::nativeVectorWidthDouble() अचर { OCL_NOT_AVAILABLE(); }
पूर्णाङ्क Device::nativeVectorWidthHalf() अचर { OCL_NOT_AVAILABLE(); }

पूर्णाङ्क Device::preferredVectorWidthChar() अचर { OCL_NOT_AVAILABLE(); }
पूर्णाङ्क Device::preferredVectorWidthShort() अचर { OCL_NOT_AVAILABLE(); }
पूर्णाङ्क Device::preferredVectorWidthInt() अचर { OCL_NOT_AVAILABLE(); }
पूर्णाङ्क Device::preferredVectorWidthLong() अचर { OCL_NOT_AVAILABLE(); }
पूर्णाङ्क Device::preferredVectorWidthFloat() अचर { OCL_NOT_AVAILABLE(); }
पूर्णाङ्क Device::preferredVectorWidthDouble() अचर { OCL_NOT_AVAILABLE(); }
पूर्णाङ्क Device::preferredVectorWidthHalf() अचर { OCL_NOT_AVAILABLE(); }

माप_प्रकार Device::printfBufferSize() अचर { OCL_NOT_AVAILABLE(); }
माप_प्रकार Device::profilingTimerResolution() अचर { OCL_NOT_AVAILABLE(); }

/* static */
अचर Device& Device::getDefault()
{
    स्थिर Device dummy;
    फर्काऊ dummy;
}

/* static */ Device Device::fromHandle(शून्य* d) { OCL_NOT_AVAILABLE(); }


Context::Context() CV_NOEXCEPT : p(रिक्त) { }
Context::Context(पूर्णाङ्क dtype) : p(रिक्त) { }
Context::~Context() { }
Context::Context(अचर Context& c) : p(रिक्त) { }
Context& Context::चालक=(अचर Context& c) { फर्काऊ *एतत्; }
Context::Context(Context&&) CV_NOEXCEPT : p(रिक्त) { }
Context& Context::चालक=(Context&&) CV_NOEXCEPT { फर्काऊ *एतत्; }

तार्किक Context::create() { फर्काऊ असत्य; }
तार्किक Context::create(पूर्णाङ्क dtype) { फर्काऊ असत्य; }
माप_प्रकार Context::ndevices() अचर { फर्काऊ 0; }
Device& Context::device(माप_प्रकार idx) अचर { OCL_NOT_AVAILABLE(); }
Program Context::getProg(अचर ProgramSource& prog, अचर String& buildopt, String& errmsg) { OCL_NOT_AVAILABLE(); }
शून्य Context::unloadProg(Program& prog) { }

/* static */
Context& Context::getDefault()
{
    स्थिर Context dummy;
    फर्काऊ dummy;
}
शून्य* Context::ptr() अचर { फर्काऊ रिक्त; }

शून्य* Context::getOpenCLContextProperty(पूर्णाङ्क /*propertyId*/) अचर { OCL_NOT_AVAILABLE(); }

तार्किक Context::useSVM() अचर { फर्काऊ असत्य; }
शून्य Context::setUseSVM(तार्किक enabled) { }

Context::UserContext::~UserContext() { }

शून्य Context::setUserContext(std::type_index /*typeId*/, अचर std::shared_ptr<Context::UserContext>& /*userContext*/) { OCL_NOT_AVAILABLE(); }
std::shared_ptr<Context::UserContext> Context::getUserContext(std::type_index /*typeId*/) { OCL_NOT_AVAILABLE(); }

/* static */ Context Context::fromHandle(शून्य* context) { OCL_NOT_AVAILABLE(); }
/* static */ Context Context::fromDevice(अचर ocl::Device& device) { OCL_NOT_AVAILABLE(); }
/* static */ Context Context::create(अचर std::string& configuration) { OCL_NOT_AVAILABLE(); }

शून्य Context::release() { }


Platform::Platform() CV_NOEXCEPT : p(रिक्त) { }
Platform::~Platform() { }
Platform::Platform(अचर Platform&) : p(रिक्त) { }
Platform& Platform::चालक=(अचर Platform&) { फर्काऊ *एतत्; }
Platform::Platform(Platform&&) CV_NOEXCEPT : p(रिक्त) { }
Platform& Platform::चालक=(Platform&&) CV_NOEXCEPT { फर्काऊ *एतत्; }

शून्य* Platform::ptr() अचर { फर्काऊ रिक्त; }

/* static */
Platform& Platform::getDefault()
{
    स्थिर Platform dummy;
    फर्काऊ dummy;
}

शून्य attachContext(अचर String& platformName, शून्य* platformID, शून्य* context, शून्य* deviceID) { OCL_NOT_AVAILABLE(); }
शून्य convertFromBuffer(शून्य* cl_mem_buffer, माप_प्रकार step, पूर्णाङ्क rows, पूर्णाङ्क cols, पूर्णाङ्क type, UMat& dst) { OCL_NOT_AVAILABLE(); }
शून्य convertFromImage(शून्य* cl_mem_image, UMat& dst) { OCL_NOT_AVAILABLE(); }

शून्य initializeContextFromHandle(Context& ctx, शून्य* platform, शून्य* context, शून्य* device) { OCL_NOT_AVAILABLE(); }

Queue::Queue() CV_NOEXCEPT : p(रिक्त) { }
Queue::Queue(अचर Context& c, अचर Device& d) : p(रिक्त) { OCL_NOT_AVAILABLE(); }
Queue::~Queue() { }
Queue::Queue(अचर Queue& q) {}
Queue& Queue::चालक=(अचर Queue& q) { फर्काऊ *एतत्; }
Queue::Queue(Queue&&) CV_NOEXCEPT : p(रिक्त) { }
Queue& Queue::चालक=(Queue&&) CV_NOEXCEPT { फर्काऊ *एतत्; }

तार्किक Queue::create(अचर Context& c, अचर Device& d) { OCL_NOT_AVAILABLE(); }
शून्य Queue::finish() {}
शून्य* Queue::ptr() अचर { फर्काऊ रिक्त; }
/* static */
Queue& Queue::getDefault()
{
    स्थिर Queue dummy;
    फर्काऊ dummy;
}

/// @brief Returns OpenCL command queue with enable profiling mode support
अचर Queue& Queue::getProfilingQueue() अचर { OCL_NOT_AVAILABLE(); }


KernelArg::KernelArg() CV_NOEXCEPT
    : flags(0), m(0), obj(0), sz(0), wscale(1), iwscale(1)
{
}

KernelArg::KernelArg(पूर्णाङ्क _flags, UMat* _m, पूर्णाङ्क _wscale, पूर्णाङ्क _iwscale, अचर शून्य* _obj, माप_प्रकार _sz)
    : flags(_flags), m(_m), obj(_obj), sz(_sz), wscale(_wscale), iwscale(_iwscale)
{
    OCL_NOT_AVAILABLE();
}

KernelArg KernelArg::Constant(अचर Mat& m)
{
    OCL_NOT_AVAILABLE();
}


Kernel::Kernel() CV_NOEXCEPT : p(रिक्त) { }
Kernel::Kernel(अचर वर्ण* kname, अचर Program& prog) : p(रिक्त) { OCL_NOT_AVAILABLE(); }
Kernel::Kernel(अचर वर्ण* kname, अचर ProgramSource& prog, अचर String& buildopts, String* errmsg) : p(रिक्त) { OCL_NOT_AVAILABLE(); }
Kernel::~Kernel() { }
Kernel::Kernel(अचर Kernel& k) : p(रिक्त) { }
Kernel& Kernel::चालक=(अचर Kernel& k) { फर्काऊ *एतत्; }
Kernel::Kernel(Kernel&&) CV_NOEXCEPT : p(रिक्त) { }
Kernel& Kernel::चालक=(Kernel&&) CV_NOEXCEPT { फर्काऊ *एतत्; }

तार्किक Kernel::empty() अचर { फर्काऊ सत्य; }
तार्किक Kernel::create(अचर वर्ण* kname, अचर Program& prog) { OCL_NOT_AVAILABLE(); }
तार्किक Kernel::create(अचर वर्ण* kname, अचर ProgramSource& prog, अचर String& buildopts, String* errmsg) { OCL_NOT_AVAILABLE(); }

पूर्णाङ्क Kernel::set(पूर्णाङ्क i, अचर शून्य* value, माप_प्रकार sz) { OCL_NOT_AVAILABLE(); }
पूर्णाङ्क Kernel::set(पूर्णाङ्क i, अचर Image2D& image2D) { OCL_NOT_AVAILABLE(); }
पूर्णाङ्क Kernel::set(पूर्णाङ्क i, अचर UMat& m) { OCL_NOT_AVAILABLE(); }
पूर्णाङ्क Kernel::set(पूर्णाङ्क i, अचर KernelArg& arg) { OCL_NOT_AVAILABLE(); }

तार्किक Kernel::run(पूर्णाङ्क dims, माप_प्रकार globalsize[], माप_प्रकार localsize[], तार्किक समकाल, अचर Queue& q) { OCL_NOT_AVAILABLE(); }
तार्किक Kernel::runTask(तार्किक समकाल, अचर Queue& q) { OCL_NOT_AVAILABLE(); }

int64 Kernel::runProfiling(पूर्णाङ्क dims, माप_प्रकार globalsize[], माप_प्रकार localsize[], अचर Queue& q) { OCL_NOT_AVAILABLE(); }

माप_प्रकार Kernel::workGroupSize() अचर { OCL_NOT_AVAILABLE(); }
माप_प्रकार Kernel::preferedWorkGroupSizeMultiple() अचर { OCL_NOT_AVAILABLE(); }
तार्किक Kernel::compileWorkGroupSize(माप_प्रकार wsz[]) अचर { OCL_NOT_AVAILABLE(); }
माप_प्रकार Kernel::localMemSize() अचर { OCL_NOT_AVAILABLE(); }

शून्य* Kernel::ptr() अचर { फर्काऊ रिक्त; }


Program::Program() CV_NOEXCEPT : p(रिक्त) { }
Program::Program(अचर ProgramSource& src, अचर String& buildflags, String& errmsg) : p(रिक्त) { OCL_NOT_AVAILABLE(); }
Program::Program(अचर Program& prog) : p(रिक्त) { }
Program& Program::चालक=(अचर Program& prog) { फर्काऊ *एतत्; }
Program::Program(Program&&) CV_NOEXCEPT : p(रिक्त) { }
Program& Program::चालक=(Program&&) CV_NOEXCEPT { फर्काऊ *एतत्; }
Program::~Program() { }

तार्किक Program::create(अचर ProgramSource& src, अचर String& buildflags, String& errmsg) { OCL_NOT_AVAILABLE(); }

शून्य* Program::ptr() अचर { फर्काऊ रिक्त; }

शून्य Program::get_द्विआधारी(std::vector<वर्ण>& binary) अचर { OCL_NOT_AVAILABLE(); }

तार्किक Program::पठ(अचर String& buf, अचर String& buildflags) { OCL_NOT_AVAILABLE(); }
तार्किक Program::लिख(String& buf) अचर { OCL_NOT_AVAILABLE(); }
अचर ProgramSource& Program::source() अचर { OCL_NOT_AVAILABLE(); }
String Program::getPrefix() अचर { OCL_NOT_AVAILABLE(); }
/* static */ String Program::getPrefix(अचर String& buildflags) { OCL_NOT_AVAILABLE(); }


ProgramSource::ProgramSource() CV_NOEXCEPT : p(रिक्त) { }
ProgramSource::ProgramSource(अचर String& module, अचर String& name, अचर String& codeStr, अचर String& codeHash) : p(रिक्त) { }
ProgramSource::ProgramSource(अचर String& prog) : p(रिक्त) { }
ProgramSource::ProgramSource(अचर वर्ण* prog) : p(रिक्त) { }
ProgramSource::~ProgramSource() { }
ProgramSource::ProgramSource(अचर ProgramSource& prog) : p(रिक्त) { }
ProgramSource& ProgramSource::चालक=(अचर ProgramSource& prog) { फर्काऊ *एतत्; }
ProgramSource::ProgramSource(ProgramSource&&) CV_NOEXCEPT : p(रिक्त) { }
ProgramSource& ProgramSource::चालक=(ProgramSource&&) CV_NOEXCEPT { फर्काऊ *एतत्; }

अचर String& ProgramSource::source() अचर { OCL_NOT_AVAILABLE(); }
ProgramSource::hash_t ProgramSource::hash() अचर { OCL_NOT_AVAILABLE(); }

/* static */ ProgramSource ProgramSource::from_द्विआधारी(अचर String& module, अचर String& name, अचर अचिन्हित वर्ण* binary, अचर माप_प्रकार size, अचर cv::String& buildOptions) { OCL_NOT_AVAILABLE(); }
/* static */ ProgramSource ProgramSource::fromSPIR(अचर String& module, अचर String& name, अचर अचिन्हित वर्ण* binary, अचर माप_प्रकार size, अचर cv::String& buildOptions) { OCL_NOT_AVAILABLE(); }


PlatformInfo::PlatformInfo() CV_NOEXCEPT : p(रिक्त) { }
PlatformInfo::PlatformInfo(शून्य* id) : p(रिक्त) { OCL_NOT_AVAILABLE(); }
PlatformInfo::~PlatformInfo() { }

PlatformInfo::PlatformInfo(अचर PlatformInfo& i) : p(रिक्त) { }
PlatformInfo& PlatformInfo::चालक=(अचर PlatformInfo& i) { फर्काऊ *एतत्; }
PlatformInfo::PlatformInfo(PlatformInfo&&) CV_NOEXCEPT : p(रिक्त) { }
PlatformInfo& PlatformInfo::चालक=(PlatformInfo&&) CV_NOEXCEPT { फर्काऊ *एतत्; }

String PlatformInfo::name() अचर { OCL_NOT_AVAILABLE(); }
String PlatformInfo::vendor() अचर { OCL_NOT_AVAILABLE(); }
String PlatformInfo::version() अचर { OCL_NOT_AVAILABLE(); }
पूर्णाङ्क PlatformInfo::device_सङ्ख्या() अचर { OCL_NOT_AVAILABLE(); }
शून्य PlatformInfo::getDevice(Device& device, पूर्णाङ्क d) अचर { OCL_NOT_AVAILABLE(); }

अचर वर्ण* convertTypeStr(पूर्णाङ्क sdepth, पूर्णाङ्क ddepth, पूर्णाङ्क cn, वर्ण* buf) { OCL_NOT_AVAILABLE(); }
अचर वर्ण* typeToStr(पूर्णाङ्क t) { OCL_NOT_AVAILABLE(); }
अचर वर्ण* memopTypeToStr(पूर्णाङ्क t) { OCL_NOT_AVAILABLE(); }
अचर वर्ण* vecopTypeToStr(पूर्णाङ्क t) { OCL_NOT_AVAILABLE(); }
अचर वर्ण* getOpenCLErrorString(पूर्णाङ्क errorCode) { OCL_NOT_AVAILABLE(); }
String kernelToStr(आगत_Array _kernel, पूर्णाङ्क ddepth, अचर वर्ण* name) { OCL_NOT_AVAILABLE(); }
शून्य getPlatfomsInfo(std::vector<PlatformInfo>& platform_info) { OCL_NOT_AVAILABLE(); }


पूर्णाङ्क predictOptimalVectorWidth(आगत_Array src1, आगत_Array src2, आगत_Array src3,
        आगत_Array src4, आगत_Array src5, आगत_Array src6,
        आगत_Array src7, आगत_Array src8, आगत_Array src9,
        OclVectorStrategy strat)
{ OCL_NOT_AVAILABLE(); }

पूर्णाङ्क checkOptimalVectorWidth(अचर पूर्णाङ्क *vectorWidths,
        आगत_Array src1, आगत_Array src2, आगत_Array src3,
        आगत_Array src4, आगत_Array src5, आगत_Array src6,
        आगत_Array src7, आगत_Array src8, आगत_Array src9,
        OclVectorStrategy strat)
{ OCL_NOT_AVAILABLE(); }

पूर्णाङ्क predictOptimalVectorWidthMax(आगत_Array src1, आगत_Array src2, आगत_Array src3,
        आगत_Array src4, आगत_Array src5, आगत_Array src6,
        आगत_Array src7, आगत_Array src8, आगत_Array src9)
{ OCL_NOT_AVAILABLE(); }

शून्य buildOptionsAddMatrixDescription(String& buildOptions, अचर String& name, आगत_Array _m) { OCL_NOT_AVAILABLE(); }


Image2D::Image2D() CV_NOEXCEPT : p(रिक्त) { }
Image2D::Image2D(अचर UMat &src, तार्किक norm, तार्किक alias) { OCL_NOT_AVAILABLE(); }
Image2D::Image2D(अचर Image2D & i) : p(रिक्त) { OCL_NOT_AVAILABLE(); }
Image2D::~Image2D() { }
Image2D& Image2D::चालक=(अचर Image2D & i) { फर्काऊ *एतत्; }
Image2D::Image2D(Image2D&&) CV_NOEXCEPT : p(रिक्त) { }
Image2D& Image2D::चालक=(Image2D&&) CV_NOEXCEPT { फर्काऊ *एतत्; }

/* static */ तार्किक Image2D::canCreateAlias(अचर UMat &u) { OCL_NOT_AVAILABLE(); }
/* static */ तार्किक Image2D::isFormatSupported(पूर्णाङ्क depth, पूर्णाङ्क cn, तार्किक norm) { OCL_NOT_AVAILABLE(); }

शून्य* Image2D::ptr() अचर { फर्काऊ रिक्त; }


Timer::Timer(अचर Queue& q) : p(रिक्त) {}
Timer::~Timer() {}
शून्य Timer::start() { OCL_NOT_AVAILABLE(); }
शून्य Timer::stop() { OCL_NOT_AVAILABLE();}

uint64 Timer::durationNS() अचर { OCL_NOT_AVAILABLE(); }

MatAllocator* getOpenCLAllocator() { फर्काऊ रिक्त; }

internal::ProgramEntry::चालक ProgramSource&() अचर { OCL_NOT_AVAILABLE(); }


संरचना OpenCLExecutionContext::Impl
{
    Impl() = स्वभाव;
};

Context& OpenCLExecutionContext::getContext() अचर { OCL_NOT_AVAILABLE(); }
Device& OpenCLExecutionContext::getDevice() अचर { OCL_NOT_AVAILABLE(); }
Queue& OpenCLExecutionContext::getQueue() अचर { OCL_NOT_AVAILABLE(); }

तार्किक OpenCLExecutionContext::useOpenCL() अचर { फर्काऊ असत्य; }
शून्य OpenCLExecutionContext::setUseOpenCL(तार्किक flag) { }

स्थिर
OpenCLExecutionContext& getDummyOpenCLExecutionContext()
{
    स्थिर OpenCLExecutionContext dummy;
    फर्काऊ dummy;
}

/* static */
OpenCLExecutionContext& OpenCLExecutionContext::getCurrent() { फर्काऊ getDummyOpenCLExecutionContext(); }

/* static */
OpenCLExecutionContext& OpenCLExecutionContext::getCurrentRef() { फर्काऊ getDummyOpenCLExecutionContext(); }

शून्य OpenCLExecutionContext::bind() अचर { OCL_NOT_AVAILABLE(); }

OpenCLExecutionContext OpenCLExecutionContext::cloneWithNewQueue(अचर ocl::Queue& q) अचर { OCL_NOT_AVAILABLE(); }
OpenCLExecutionContext OpenCLExecutionContext::cloneWithNewQueue() अचर { OCL_NOT_AVAILABLE(); }

/* static */ OpenCLExecutionContext OpenCLExecutionContext::create(अचर std::string& platformName, शून्य* platformID, शून्य* context, शून्य* deviceID) { OCL_NOT_AVAILABLE(); }
/* static */ OpenCLExecutionContext OpenCLExecutionContext::create(अचर Context& context, अचर Device& device, अचर ocl::Queue& queue) { OCL_NOT_AVAILABLE(); }
/* static */ OpenCLExecutionContext OpenCLExecutionContext::create(अचर Context& context, अचर Device& device) { OCL_NOT_AVAILABLE(); }

शून्य OpenCLExecutionContext::release() { }

}}

#यदि defined(_MSC_VER)
    #pragma warning(pop)
#अथवायदि defined(__clang__)
    #pragma clang diagnostic pop
#अथवायदि defined(__GNUC__)
    #pragma GCC diagnostic pop
#endif
