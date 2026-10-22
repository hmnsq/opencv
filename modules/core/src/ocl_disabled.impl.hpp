// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#समावेश "opencv2/core/ocl_genbase.hpp"

#अगर defined(_MSC_VER)
    #आशय warning(push)
    #आशय warning(disable : 4100)
    #आशय warning(disable : 4702)
#या_अगर defined(__clang__)
    #आशय clang diagnostic push
    #आशय clang diagnostic ignored "-Wunused-parameter"
#या_अगर defined(__GNUC__)
    #आशय GCC diagnostic push
    #आशय GCC diagnostic ignored "-Wunused-parameter"
#पूर्ण_अगर

namespace cv { namespace ocl {

अटल
CV_NORETURN व्योम throw_no_ocl()
{
    CV_Error(Error::OpenCLApiCallError, "OpenCV build without OpenCL support");
}
#घोषणा OCL_NOT_AVAILABLE() throw_no_ocl();

CV_EXPORTS_W bool haveOpenCL() { वापस false; }
CV_EXPORTS_W bool useOpenCL() { वापस false; }
CV_EXPORTS_W bool haveAmdBlas() { वापस false; }
CV_EXPORTS_W bool haveAmdFft() { वापस false; }
CV_EXPORTS_W व्योम setUseOpenCL(bool flag) { /* nothing */ }
CV_EXPORTS_W व्योम finish() { /* nothing */ }

CV_EXPORTS bool haveSVM() { वापस false; }

Device::Device() CV_NOEXCEPT : p(शून्य) { }
Device::Device(व्योम* d) : p(शून्य) { OCL_NOT_AVAILABLE(); }
Device::Device(स्थिर Device& d) : p(शून्य) { }
Device& Device::चालक=(स्थिर Device& d) { वापस *यह; }
Device::Device(Device&&) CV_NOEXCEPT : p(शून्य) { }
Device& Device::चालक=(Device&&) CV_NOEXCEPT { वापस *यह; }
Device::~Device() { }

व्योम Device::set(व्योम* d) { OCL_NOT_AVAILABLE(); }

String Device::name() स्थिर { OCL_NOT_AVAILABLE(); }
String Device::extensions() स्थिर { OCL_NOT_AVAILABLE(); }
bool Device::isExtensionSupported(स्थिर String& extensionName) स्थिर { OCL_NOT_AVAILABLE(); }
String Device::version() स्थिर { OCL_NOT_AVAILABLE(); }
String Device::vendorName() स्थिर { OCL_NOT_AVAILABLE(); }
String Device::OpenCL_C_Version() स्थिर { OCL_NOT_AVAILABLE(); }
String Device::OpenCLVersion() स्थिर { OCL_NOT_AVAILABLE(); }
पूर्णांक Device::deviceVersionMajor() स्थिर { OCL_NOT_AVAILABLE(); }
पूर्णांक Device::deviceVersionMinor() स्थिर { OCL_NOT_AVAILABLE(); }
String Device::driverVersion() स्थिर { OCL_NOT_AVAILABLE(); }
व्योम* Device::ptr() स्थिर { /*OCL_NOT_AVAILABLE();*/ वापस शून्य; }

पूर्णांक Device::type() स्थिर { OCL_NOT_AVAILABLE(); }

पूर्णांक Device::addressBits() स्थिर { OCL_NOT_AVAILABLE(); }
bool Device::available() स्थिर { OCL_NOT_AVAILABLE(); }
bool Device::compilerAvailable() स्थिर { OCL_NOT_AVAILABLE(); }
bool Device::linkerAvailable() स्थिर { OCL_NOT_AVAILABLE(); }

पूर्णांक Device::doubleFPConfig() स्थिर { OCL_NOT_AVAILABLE(); }
पूर्णांक Device::singleFPConfig() स्थिर { OCL_NOT_AVAILABLE(); }
पूर्णांक Device::halfFPConfig() स्थिर { OCL_NOT_AVAILABLE(); }

bool Device::hasFP64() स्थिर { OCL_NOT_AVAILABLE(); }
bool Device::hasFP16() स्थिर { OCL_NOT_AVAILABLE(); }

bool Device::endianLittle() स्थिर { OCL_NOT_AVAILABLE(); }
bool Device::errorCorrectionSupport() स्थिर { OCL_NOT_AVAILABLE(); }

पूर्णांक Device::executionCapabilities() स्थिर { OCL_NOT_AVAILABLE(); }

माप_प्रकार Device::global_Mem_Cache_आकार() स्थिर { OCL_NOT_AVAILABLE(); }

पूर्णांक Device::globalMemCacheType() स्थिर { OCL_NOT_AVAILABLE(); }
पूर्णांक Device::global_Mem_Cache_पंक्ति_आकार() स्थिर { OCL_NOT_AVAILABLE(); }
माप_प्रकार Device::global_Mem_आकार() स्थिर { OCL_NOT_AVAILABLE(); }

माप_प्रकार Device::local_Mem_आकार() स्थिर { OCL_NOT_AVAILABLE(); }
पूर्णांक Device::localMemType() स्थिर { वापस NO_LOCAL_MEM; }
bool Device::hostUnifiedMemory() स्थिर { OCL_NOT_AVAILABLE(); }

bool Device::imageSupport() स्थिर { OCL_NOT_AVAILABLE(); }

bool Device::image_From_बफर_Support() स्थिर { OCL_NOT_AVAILABLE(); }
uint Device::imagePitchAlignment() स्थिर { OCL_NOT_AVAILABLE(); }
uint Device::imageBaseAddressAlignment() स्थिर { OCL_NOT_AVAILABLE(); }

bool Device::intelSubgroupsSupport() स्थिर { OCL_NOT_AVAILABLE(); }

माप_प्रकार Device::image2DMaxWidth() स्थिर { OCL_NOT_AVAILABLE(); }
माप_प्रकार Device::image2DMaxHeight() स्थिर { OCL_NOT_AVAILABLE(); }

माप_प्रकार Device::image3DMaxWidth() स्थिर { OCL_NOT_AVAILABLE(); }
माप_प्रकार Device::image3DMaxHeight() स्थिर { OCL_NOT_AVAILABLE(); }
माप_प्रकार Device::image3DMaxDepth() स्थिर { OCL_NOT_AVAILABLE(); }

माप_प्रकार Device::image_Max_बफर_आकार() स्थिर { OCL_NOT_AVAILABLE(); }
माप_प्रकार Device::image_Max_Array_आकार() स्थिर { OCL_NOT_AVAILABLE(); }

पूर्णांक Device::vendorID() स्थिर { OCL_NOT_AVAILABLE(); }

पूर्णांक Device::maxClockFrequency() स्थिर { OCL_NOT_AVAILABLE(); }
पूर्णांक Device::maxComputeUnits() स्थिर { OCL_NOT_AVAILABLE(); }
पूर्णांक Device::maxConstantArgs() स्थिर { OCL_NOT_AVAILABLE(); }
माप_प्रकार Device::max_Constant_बफर_आकार() स्थिर { OCL_NOT_AVAILABLE(); }

माप_प्रकार Device::max_Mem_Alloc_आकार() स्थिर { OCL_NOT_AVAILABLE(); }
माप_प्रकार Device::max_Parameter_आकार() स्थिर { OCL_NOT_AVAILABLE(); }

पूर्णांक Device::maxReadImageArgs() स्थिर { OCL_NOT_AVAILABLE(); }
पूर्णांक Device::maxWriteImageArgs() स्थिर { OCL_NOT_AVAILABLE(); }
पूर्णांक Device::maxSamplers() स्थिर { OCL_NOT_AVAILABLE(); }

माप_प्रकार Device::max_Work_Group_आकार() स्थिर { OCL_NOT_AVAILABLE(); }
पूर्णांक Device::maxWorkItemDims() स्थिर { OCL_NOT_AVAILABLE(); }
व्योम Device::maxWorkItemSizes(माप_प्रकार*) स्थिर { OCL_NOT_AVAILABLE(); }

पूर्णांक Device::memBaseAddrAlign() स्थिर { OCL_NOT_AVAILABLE(); }

पूर्णांक Device::nativeVectorWidthChar() स्थिर { OCL_NOT_AVAILABLE(); }
पूर्णांक Device::nativeVectorWidthShort() स्थिर { OCL_NOT_AVAILABLE(); }
पूर्णांक Device::nativeVectorWidthInt() स्थिर { OCL_NOT_AVAILABLE(); }
पूर्णांक Device::nativeVectorWidthLong() स्थिर { OCL_NOT_AVAILABLE(); }
पूर्णांक Device::nativeVectorWidthFloat() स्थिर { OCL_NOT_AVAILABLE(); }
पूर्णांक Device::nativeVectorWidthDouble() स्थिर { OCL_NOT_AVAILABLE(); }
पूर्णांक Device::nativeVectorWidthHalf() स्थिर { OCL_NOT_AVAILABLE(); }

पूर्णांक Device::preferredVectorWidthChar() स्थिर { OCL_NOT_AVAILABLE(); }
पूर्णांक Device::preferredVectorWidthShort() स्थिर { OCL_NOT_AVAILABLE(); }
पूर्णांक Device::preferredVectorWidthInt() स्थिर { OCL_NOT_AVAILABLE(); }
पूर्णांक Device::preferredVectorWidthLong() स्थिर { OCL_NOT_AVAILABLE(); }
पूर्णांक Device::preferredVectorWidthFloat() स्थिर { OCL_NOT_AVAILABLE(); }
पूर्णांक Device::preferredVectorWidthDouble() स्थिर { OCL_NOT_AVAILABLE(); }
पूर्णांक Device::preferredVectorWidthHalf() स्थिर { OCL_NOT_AVAILABLE(); }

माप_प्रकार Device::printf_बफर_आकार() स्थिर { OCL_NOT_AVAILABLE(); }
माप_प्रकार Device::profilingTimerResolution() स्थिर { OCL_NOT_AVAILABLE(); }

/* static */
स्थिर Device& Device::getDefault()
{
    अटल Device dummy;
    वापस dummy;
}

/* static */ Device Device::fromHandle(व्योम* d) { OCL_NOT_AVAILABLE(); }


Context::Context() CV_NOEXCEPT : p(शून्य) { }
Context::Context(पूर्णांक dtype) : p(शून्य) { }
Context::~Context() { }
Context::Context(स्थिर Context& c) : p(शून्य) { }
Context& Context::चालक=(स्थिर Context& c) { वापस *यह; }
Context::Context(Context&&) CV_NOEXCEPT : p(शून्य) { }
Context& Context::चालक=(Context&&) CV_NOEXCEPT { वापस *यह; }

bool Context::create() { वापस false; }
bool Context::create(पूर्णांक dtype) { वापस false; }
माप_प्रकार Context::ndevices() स्थिर { वापस 0; }
Device& Context::device(माप_प्रकार idx) स्थिर { OCL_NOT_AVAILABLE(); }
Program Context::getProg(स्थिर ProgramSource& prog, स्थिर String& buildopt, String& errmsg) { OCL_NOT_AVAILABLE(); }
व्योम Context::unloadProg(Program& prog) { }

/* static */
Context& Context::getDefault()
{
    अटल Context dummy;
    वापस dummy;
}
व्योम* Context::ptr() स्थिर { वापस शून्य; }

व्योम* Context::getOpenCLContextProperty(पूर्णांक /*propertyId*/) स्थिर { OCL_NOT_AVAILABLE(); }

bool Context::useSVM() स्थिर { वापस false; }
व्योम Context::setUseSVM(bool enabled) { }

Context::UserContext::~UserContext() { }

व्योम Context::setUserContext(std::type_सूचकांक /*typeId*/, स्थिर std::shared_ptr<Context::UserContext>& /*userContext*/) { OCL_NOT_AVAILABLE(); }
std::shared_ptr<Context::UserContext> Context::getUserContext(std::type_सूचकांक /*typeId*/) { OCL_NOT_AVAILABLE(); }

/* static */ Context Context::fromHandle(व्योम* context) { OCL_NOT_AVAILABLE(); }
/* static */ Context Context::fromDevice(स्थिर ocl::Device& device) { OCL_NOT_AVAILABLE(); }
/* static */ Context Context::create(स्थिर std::string& configuration) { OCL_NOT_AVAILABLE(); }

व्योम Context::release() { }


Platform::Platform() CV_NOEXCEPT : p(शून्य) { }
Platform::~Platform() { }
Platform::Platform(स्थिर Platform&) : p(शून्य) { }
Platform& Platform::चालक=(स्थिर Platform&) { वापस *यह; }
Platform::Platform(Platform&&) CV_NOEXCEPT : p(शून्य) { }
Platform& Platform::चालक=(Platform&&) CV_NOEXCEPT { वापस *यह; }

व्योम* Platform::ptr() स्थिर { वापस शून्य; }

/* static */
Platform& Platform::getDefault()
{
    अटल Platform dummy;
    वापस dummy;
}

व्योम attachContext(स्थिर String& platformName, व्योम* platformID, व्योम* context, व्योम* deviceID) { OCL_NOT_AVAILABLE(); }
व्योम convert_From_बफर(व्योम* cl_mem_बफर, माप_प्रकार step, पूर्णांक rows, पूर्णांक cols, पूर्णांक type, UMat& dst) { OCL_NOT_AVAILABLE(); }
व्योम convertFromImage(व्योम* cl_mem_image, UMat& dst) { OCL_NOT_AVAILABLE(); }

व्योम initializeContextFromHandle(Context& ctx, व्योम* platform, व्योम* context, व्योम* device) { OCL_NOT_AVAILABLE(); }

Queue::Queue() CV_NOEXCEPT : p(शून्य) { }
Queue::Queue(स्थिर Context& c, स्थिर Device& d) : p(शून्य) { OCL_NOT_AVAILABLE(); }
Queue::~Queue() { }
Queue::Queue(स्थिर Queue& q) {}
Queue& Queue::चालक=(स्थिर Queue& q) { वापस *यह; }
Queue::Queue(Queue&&) CV_NOEXCEPT : p(शून्य) { }
Queue& Queue::चालक=(Queue&&) CV_NOEXCEPT { वापस *यह; }

bool Queue::create(स्थिर Context& c, स्थिर Device& d) { OCL_NOT_AVAILABLE(); }
व्योम Queue::finish() {}
व्योम* Queue::ptr() स्थिर { वापस शून्य; }
/* static */
Queue& Queue::getDefault()
{
    अटल Queue dummy;
    वापस dummy;
}

/// @brief Returns OpenCL command queue with enable profiling mode support
स्थिर Queue& Queue::getProfilingQueue() स्थिर { OCL_NOT_AVAILABLE(); }


KernelArg::KernelArg() CV_NOEXCEPT
    : flags(0), m(0), obj(0), sz(0), wscale(1), iwscale(1)
{
}

KernelArg::KernelArg(पूर्णांक _flags, UMat* _m, पूर्णांक _wscale, पूर्णांक _iwscale, स्थिर व्योम* _obj, माप_प्रकार _sz)
    : flags(_flags), m(_m), obj(_obj), sz(_sz), wscale(_wscale), iwscale(_iwscale)
{
    OCL_NOT_AVAILABLE();
}

KernelArg KernelArg::Constant(स्थिर Mat& m)
{
    OCL_NOT_AVAILABLE();
}


Kernel::Kernel() CV_NOEXCEPT : p(शून्य) { }
Kernel::Kernel(स्थिर अक्षर* kname, स्थिर Program& prog) : p(शून्य) { OCL_NOT_AVAILABLE(); }
Kernel::Kernel(स्थिर अक्षर* kname, स्थिर ProgramSource& prog, स्थिर String& buildopts, String* errmsg) : p(शून्य) { OCL_NOT_AVAILABLE(); }
Kernel::~Kernel() { }
Kernel::Kernel(स्थिर Kernel& k) : p(शून्य) { }
Kernel& Kernel::चालक=(स्थिर Kernel& k) { वापस *यह; }
Kernel::Kernel(Kernel&&) CV_NOEXCEPT : p(शून्य) { }
Kernel& Kernel::चालक=(Kernel&&) CV_NOEXCEPT { वापस *यह; }

bool Kernel::empty() स्थिर { वापस true; }
bool Kernel::create(स्थिर अक्षर* kname, स्थिर Program& prog) { OCL_NOT_AVAILABLE(); }
bool Kernel::create(स्थिर अक्षर* kname, स्थिर ProgramSource& prog, स्थिर String& buildopts, String* errmsg) { OCL_NOT_AVAILABLE(); }

पूर्णांक Kernel::set(पूर्णांक i, स्थिर व्योम* value, माप_प्रकार sz) { OCL_NOT_AVAILABLE(); }
पूर्णांक Kernel::set(पूर्णांक i, स्थिर Image2D& image2D) { OCL_NOT_AVAILABLE(); }
पूर्णांक Kernel::set(पूर्णांक i, स्थिर UMat& m) { OCL_NOT_AVAILABLE(); }
पूर्णांक Kernel::set(पूर्णांक i, स्थिर KernelArg& arg) { OCL_NOT_AVAILABLE(); }

bool Kernel::run(पूर्णांक dims, माप_प्रकार globalsize[], माप_प्रकार localsize[], bool sync, स्थिर Queue& q) { OCL_NOT_AVAILABLE(); }
bool Kernel::runTask(bool sync, स्थिर Queue& q) { OCL_NOT_AVAILABLE(); }

int64 Kernel::runProfiling(पूर्णांक dims, माप_प्रकार globalsize[], माप_प्रकार localsize[], स्थिर Queue& q) { OCL_NOT_AVAILABLE(); }

माप_प्रकार Kernel::work_Group_आकार() स्थिर { OCL_NOT_AVAILABLE(); }
माप_प्रकार Kernel::prefered_Work_Group_आकार_Multiple() स्थिर { OCL_NOT_AVAILABLE(); }
bool Kernel::compile_Work_Group_आकार(माप_प्रकार wsz[]) स्थिर { OCL_NOT_AVAILABLE(); }
माप_प्रकार Kernel::local_Mem_आकार() स्थिर { OCL_NOT_AVAILABLE(); }

व्योम* Kernel::ptr() स्थिर { वापस शून्य; }


Program::Program() CV_NOEXCEPT : p(शून्य) { }
Program::Program(स्थिर ProgramSource& src, स्थिर String& buildflags, String& errmsg) : p(शून्य) { OCL_NOT_AVAILABLE(); }
Program::Program(स्थिर Program& prog) : p(शून्य) { }
Program& Program::चालक=(स्थिर Program& prog) { वापस *यह; }
Program::Program(Program&&) CV_NOEXCEPT : p(शून्य) { }
Program& Program::चालक=(Program&&) CV_NOEXCEPT { वापस *यह; }
Program::~Program() { }

bool Program::create(स्थिर ProgramSource& src, स्थिर String& buildflags, String& errmsg) { OCL_NOT_AVAILABLE(); }

व्योम* Program::ptr() स्थिर { वापस शून्य; }

व्योम Program::get_द्विआधारी(std::vector<अक्षर>& द्विग) स्थिर { OCL_NOT_AVAILABLE(); }

bool Program::पढ़ो(स्थिर String& buf, स्थिर String& buildflags) { OCL_NOT_AVAILABLE(); }
bool Program::ग_लिखो(String& buf) स्थिर { OCL_NOT_AVAILABLE(); }
स्थिर ProgramSource& Program::source() स्थिर { OCL_NOT_AVAILABLE(); }
String Program::getPrefix() स्थिर { OCL_NOT_AVAILABLE(); }
/* static */ String Program::getPrefix(स्थिर String& buildflags) { OCL_NOT_AVAILABLE(); }


ProgramSource::ProgramSource() CV_NOEXCEPT : p(शून्य) { }
ProgramSource::ProgramSource(स्थिर String& module, स्थिर String& name, स्थिर String& codeStr, स्थिर String& codeHash) : p(शून्य) { }
ProgramSource::ProgramSource(स्थिर String& prog) : p(शून्य) { }
ProgramSource::ProgramSource(स्थिर अक्षर* prog) : p(शून्य) { }
ProgramSource::~ProgramSource() { }
ProgramSource::ProgramSource(स्थिर ProgramSource& prog) : p(शून्य) { }
ProgramSource& ProgramSource::चालक=(स्थिर ProgramSource& prog) { वापस *यह; }
ProgramSource::ProgramSource(ProgramSource&&) CV_NOEXCEPT : p(शून्य) { }
ProgramSource& ProgramSource::चालक=(ProgramSource&&) CV_NOEXCEPT { वापस *यह; }

स्थिर String& ProgramSource::source() स्थिर { OCL_NOT_AVAILABLE(); }
ProgramSource::hash_t ProgramSource::hash() स्थिर { OCL_NOT_AVAILABLE(); }

/* static */ ProgramSource ProgramSource::from_द्विआधारी(स्थिर String& module, स्थिर String& name, स्थिर अचिन्हित अक्षर* द्विग, स्थिर माप_प्रकार size, स्थिर cv::String& buildOptions) { OCL_NOT_AVAILABLE(); }
/* static */ ProgramSource ProgramSource::fromSPIR(स्थिर String& module, स्थिर String& name, स्थिर अचिन्हित अक्षर* द्विग, स्थिर माप_प्रकार size, स्थिर cv::String& buildOptions) { OCL_NOT_AVAILABLE(); }


PlatformInfo::PlatformInfo() CV_NOEXCEPT : p(शून्य) { }
PlatformInfo::PlatformInfo(व्योम* id) : p(शून्य) { OCL_NOT_AVAILABLE(); }
PlatformInfo::~PlatformInfo() { }

PlatformInfo::PlatformInfo(स्थिर PlatformInfo& i) : p(शून्य) { }
PlatformInfo& PlatformInfo::चालक=(स्थिर PlatformInfo& i) { वापस *यह; }
PlatformInfo::PlatformInfo(PlatformInfo&&) CV_NOEXCEPT : p(शून्य) { }
PlatformInfo& PlatformInfo::चालक=(PlatformInfo&&) CV_NOEXCEPT { वापस *यह; }

String PlatformInfo::name() स्थिर { OCL_NOT_AVAILABLE(); }
String PlatformInfo::vendor() स्थिर { OCL_NOT_AVAILABLE(); }
String PlatformInfo::version() स्थिर { OCL_NOT_AVAILABLE(); }
पूर्णांक PlatformInfo::device_संख्या() स्थिर { OCL_NOT_AVAILABLE(); }
व्योम PlatformInfo::getDevice(Device& device, पूर्णांक d) स्थिर { OCL_NOT_AVAILABLE(); }

स्थिर अक्षर* convertTypeStr(पूर्णांक sdepth, पूर्णांक ddepth, पूर्णांक cn, अक्षर* buf) { OCL_NOT_AVAILABLE(); }
स्थिर अक्षर* typeToStr(पूर्णांक t) { OCL_NOT_AVAILABLE(); }
स्थिर अक्षर* memopTypeToStr(पूर्णांक t) { OCL_NOT_AVAILABLE(); }
स्थिर अक्षर* vecopTypeToStr(पूर्णांक t) { OCL_NOT_AVAILABLE(); }
स्थिर अक्षर* getOpenCLErrorString(पूर्णांक errorCode) { OCL_NOT_AVAILABLE(); }
String kernelToStr(निवेश_Array _kernel, पूर्णांक ddepth, स्थिर अक्षर* name) { OCL_NOT_AVAILABLE(); }
व्योम getPlatfomsInfo(std::vector<PlatformInfo>& platform_info) { OCL_NOT_AVAILABLE(); }


पूर्णांक predictOptimalVectorWidth(निवेश_Array src1, निवेश_Array src2, निवेश_Array src3,
        निवेश_Array src4, निवेश_Array src5, निवेश_Array src6,
        निवेश_Array src7, निवेश_Array src8, निवेश_Array src9,
        OclVectorStrategy strat)
{ OCL_NOT_AVAILABLE(); }

पूर्णांक checkOptimalVectorWidth(स्थिर पूर्णांक *vectorWidths,
        निवेश_Array src1, निवेश_Array src2, निवेश_Array src3,
        निवेश_Array src4, निवेश_Array src5, निवेश_Array src6,
        निवेश_Array src7, निवेश_Array src8, निवेश_Array src9,
        OclVectorStrategy strat)
{ OCL_NOT_AVAILABLE(); }

पूर्णांक predictOptimalVectorWidthMax(निवेश_Array src1, निवेश_Array src2, निवेश_Array src3,
        निवेश_Array src4, निवेश_Array src5, निवेश_Array src6,
        निवेश_Array src7, निवेश_Array src8, निवेश_Array src9)
{ OCL_NOT_AVAILABLE(); }

व्योम buildOptionsAddMatrixDescription(String& buildOptions, स्थिर String& name, निवेश_Array _m) { OCL_NOT_AVAILABLE(); }


Image2D::Image2D() CV_NOEXCEPT : p(शून्य) { }
Image2D::Image2D(स्थिर UMat &src, bool norm, bool alias) { OCL_NOT_AVAILABLE(); }
Image2D::Image2D(स्थिर Image2D & i) : p(शून्य) { OCL_NOT_AVAILABLE(); }
Image2D::~Image2D() { }
Image2D& Image2D::चालक=(स्थिर Image2D & i) { वापस *यह; }
Image2D::Image2D(Image2D&&) CV_NOEXCEPT : p(शून्य) { }
Image2D& Image2D::चालक=(Image2D&&) CV_NOEXCEPT { वापस *यह; }

/* static */ bool Image2D::canCreateAlias(स्थिर UMat &u) { OCL_NOT_AVAILABLE(); }
/* static */ bool Image2D::isFormatSupported(पूर्णांक depth, पूर्णांक cn, bool norm) { OCL_NOT_AVAILABLE(); }

व्योम* Image2D::ptr() स्थिर { वापस शून्य; }


Timer::Timer(स्थिर Queue& q) : p(शून्य) {}
Timer::~Timer() {}
व्योम Timer::start() { OCL_NOT_AVAILABLE(); }
व्योम Timer::stop() { OCL_NOT_AVAILABLE();}

uint64 Timer::durationNS() स्थिर { OCL_NOT_AVAILABLE(); }

MatAllocator* getOpenCLAllocator() { वापस शून्य; }

internal::ProgramEntry::चालक ProgramSource&() स्थिर { OCL_NOT_AVAILABLE(); }


काष्ठा OpenCLExecutionContext::Impl
{
    Impl() = शेष;
};

Context& OpenCLExecutionContext::getContext() स्थिर { OCL_NOT_AVAILABLE(); }
Device& OpenCLExecutionContext::getDevice() स्थिर { OCL_NOT_AVAILABLE(); }
Queue& OpenCLExecutionContext::getQueue() स्थिर { OCL_NOT_AVAILABLE(); }

bool OpenCLExecutionContext::useOpenCL() स्थिर { वापस false; }
व्योम OpenCLExecutionContext::setUseOpenCL(bool flag) { }

अटल
OpenCLExecutionContext& getDummyOpenCLExecutionContext()
{
    अटल OpenCLExecutionContext dummy;
    वापस dummy;
}

/* static */
OpenCLExecutionContext& OpenCLExecutionContext::getCurrent() { वापस getDummyOpenCLExecutionContext(); }

/* static */
OpenCLExecutionContext& OpenCLExecutionContext::getCurrentRef() { वापस getDummyOpenCLExecutionContext(); }

व्योम OpenCLExecutionContext::bind() स्थिर { OCL_NOT_AVAILABLE(); }

OpenCLExecutionContext OpenCLExecutionContext::cloneWithNewQueue(स्थिर ocl::Queue& q) स्थिर { OCL_NOT_AVAILABLE(); }
OpenCLExecutionContext OpenCLExecutionContext::cloneWithNewQueue() स्थिर { OCL_NOT_AVAILABLE(); }

/* static */ OpenCLExecutionContext OpenCLExecutionContext::create(स्थिर std::string& platformName, व्योम* platformID, व्योम* context, व्योम* deviceID) { OCL_NOT_AVAILABLE(); }
/* static */ OpenCLExecutionContext OpenCLExecutionContext::create(स्थिर Context& context, स्थिर Device& device, स्थिर ocl::Queue& queue) { OCL_NOT_AVAILABLE(); }
/* static */ OpenCLExecutionContext OpenCLExecutionContext::create(स्थिर Context& context, स्थिर Device& device) { OCL_NOT_AVAILABLE(); }

व्योम OpenCLExecutionContext::release() { }

}}

#अगर defined(_MSC_VER)
    #आशय warning(pop)
#या_अगर defined(__clang__)
    #आशय clang diagnostic pop
#या_अगर defined(__GNUC__)
    #आशय GCC diagnostic pop
#पूर्ण_अगर
