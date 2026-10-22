// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#అన్తర్భావ "opencv2/core/ocl_genbase.hpp"

#ఒకవేళ defined(_MSC_VER)
    #సూచన warning(push)
    #సూచన warning(disable : 4100)
    #సూచన warning(disable : 4702)
#లేదావేళ defined(__clang__)
    #సూచన clang diagnostic push
    #సూచన clang diagnostic ignored "-Wunused-parameter"
#లేదావేళ defined(__GNUC__)
    #సూచన GCC diagnostic push
    #సూచన GCC diagnostic ignored "-Wunused-parameter"
#అయితే_ముగింపు

నామాకాశ cv { నామాకాశ ocl {

స్థిర
CV_NORETURN శూన్య throw_no_ocl()
{
    CV_Error(Error::OpenCLApiCallError, "OpenCV build without OpenCL support");
}
#నిర్వచించు OCL_NOT_అందుబాటు() throw_no_ocl();

CV_EXPORTS_W తార్కిక haveOpenCL() { తిరిగి అబద్ధం; }
CV_EXPORTS_W తార్కిక useOpenCL() { తిరిగి అబద్ధం; }
CV_EXPORTS_W తార్కిక haveAmdBlas() { తిరిగి అబద్ధం; }
CV_EXPORTS_W తార్కిక haveAmdFft() { తిరిగి అబద్ధం; }
CV_EXPORTS_W శూన్య సమితి_Use_Open_CL(తార్కిక flag) { /* nothing */ }
CV_EXPORTS_W శూన్య ముగింపు_2() { /* nothing */ }

CV_EXPORTS తార్కిక haveSVM() { తిరిగి అబద్ధం; }

Device::Device() CV_NOEXCEPT : p(ఖాళీ) { }
Device::Device(శూన్య* d) : p(ఖాళీ) { OCL_NOT_అందుబాటు(); }
Device::Device(స్థిరరాశి Device& d) : p(ఖాళీ) { }
Device& Device::క్రియాకారకం=(స్థిరరాశి Device& d) { తిరిగి *ఇది; }
Device::Device(Device&&) CV_NOEXCEPT : p(ఖాళీ) { }
Device& Device::క్రియాకారకం=(Device&&) CV_NOEXCEPT { తిరిగి *ఇది; }
Device::~Device() { }

శూన్య Device::సమితి(శూన్య* d) { OCL_NOT_అందుబాటు(); }

String Device::name() స్థిరరాశి { OCL_NOT_అందుబాటు(); }
String Device::extensions() స్థిరరాశి { OCL_NOT_అందుబాటు(); }
తార్కిక Device::isExtensionSupported(స్థిరరాశి String& extensionName) స్థిరరాశి { OCL_NOT_అందుబాటు(); }
String Device::సంస్కరణ() స్థిరరాశి { OCL_NOT_అందుబాటు(); }
String Device::vendorName() స్థిరరాశి { OCL_NOT_అందుబాటు(); }
String Device::Open_CL_C_సంస్కరణ() స్థిరరాశి { OCL_NOT_అందుబాటు(); }
String Device::OpenCLVersion() స్థిరరాశి { OCL_NOT_అందుబాటు(); }
పూర్ణాంకం Device::device_సంస్కరణ_Major() స్థిరరాశి { OCL_NOT_అందుబాటు(); }
పూర్ణాంకం Device::device_సంస్కరణ_Minor() స్థిరరాశి { OCL_NOT_అందుబాటు(); }
String Device::driver_సంస్కరణ() స్థిరరాశి { OCL_NOT_అందుబాటు(); }
శూన్య* Device::ptr() స్థిరరాశి { /*OCL_NOT_AVAILABLE();*/ తిరిగి ఖాళీ; }

పూర్ణాంకం Device::type() స్థిరరాశి { OCL_NOT_అందుబాటు(); }

పూర్ణాంకం Device::చిరునామా_బిట్లు() స్థిరరాశి { OCL_NOT_అందుబాటు(); }
తార్కిక Device::అందుబాటు() స్థిరరాశి { OCL_NOT_అందుబాటు(); }
తార్కిక Device::compiler_అందుబాటు() స్థిరరాశి { OCL_NOT_అందుబాటు(); }
తార్కిక Device::linker_అందుబాటు() స్థిరరాశి { OCL_NOT_అందుబాటు(); }

పూర్ణాంకం Device::doubleFPConfig() స్థిరరాశి { OCL_NOT_అందుబాటు(); }
పూర్ణాంకం Device::singleFPConfig() స్థిరరాశి { OCL_NOT_అందుబాటు(); }
పూర్ణాంకం Device::halfFPConfig() స్థిరరాశి { OCL_NOT_అందుబాటు(); }

తార్కిక Device::hasFP64() స్థిరరాశి { OCL_NOT_అందుబాటు(); }
తార్కిక Device::hasFP16() స్థిరరాశి { OCL_NOT_అందుబాటు(); }

తార్కిక Device::endianLittle() స్థిరరాశి { OCL_NOT_అందుబాటు(); }
తార్కిక Device::errorCorrectionSupport() స్థిరరాశి { OCL_NOT_అందుబాటు(); }

పూర్ణాంకం Device::executionCapabilities() స్థిరరాశి { OCL_NOT_అందుబాటు(); }

size_t Device::global_Mem_Cache_కొలత() స్థిరరాశి { OCL_NOT_అందుబాటు(); }

పూర్ణాంకం Device::globalMemCacheType() స్థిరరాశి { OCL_NOT_అందుబాటు(); }
పూర్ణాంకం Device::global_Mem_Cache_పంక్తి_కొలత() స్థిరరాశి { OCL_NOT_అందుబాటు(); }
size_t Device::global_Mem_కొలత() స్థిరరాశి { OCL_NOT_అందుబాటు(); }

size_t Device::local_Mem_కొలత() స్థిరరాశి { OCL_NOT_అందుబాటు(); }
పూర్ణాంకం Device::localMemType() స్థిరరాశి { తిరిగి NO_LOCAL_MEM; }
తార్కిక Device::host_Unified_జ్ఞాపకం_2() స్థిరరాశి { OCL_NOT_అందుబాటు(); }

తార్కిక Device::imageSupport() స్థిరరాశి { OCL_NOT_అందుబాటు(); }

తార్కిక Device::image_From_బఫరు_Support() స్థిరరాశి { OCL_NOT_అందుబాటు(); }
uint Device::imagePitchAlignment() స్థిరరాశి { OCL_NOT_అందుబాటు(); }
uint Device::image_పునాది_చిరునామా_Alignment() స్థిరరాశి { OCL_NOT_అందుబాటు(); }

తార్కిక Device::intelSubgroupsSupport() స్థిరరాశి { OCL_NOT_అందుబాటు(); }

size_t Device::image2_DMax_వెడల్పు() స్థిరరాశి { OCL_NOT_అందుబాటు(); }
size_t Device::image2_DMax_ఎత్తు() స్థిరరాశి { OCL_NOT_అందుబాటు(); }

size_t Device::image3_DMax_వెడల్పు() స్థిరరాశి { OCL_NOT_అందుబాటు(); }
size_t Device::image3_DMax_ఎత్తు() స్థిరరాశి { OCL_NOT_అందుబాటు(); }
size_t Device::image3_DMax_లోతు() స్థిరరాశి { OCL_NOT_అందుబాటు(); }

size_t Device::image_Max_బఫరు_కొలత() స్థిరరాశి { OCL_NOT_అందుబాటు(); }
size_t Device::image_Max_Array_కొలత() స్థిరరాశి { OCL_NOT_అందుబాటు(); }

పూర్ణాంకం Device::vendorID() స్థిరరాశి { OCL_NOT_అందుబాటు(); }

పూర్ణాంకం Device::maxClockFrequency() స్థిరరాశి { OCL_NOT_అందుబాటు(); }
పూర్ణాంకం Device::maxComputeUnits() స్థిరరాశి { OCL_NOT_అందుబాటు(); }
పూర్ణాంకం Device::max_Constant_వాదనలు() స్థిరరాశి { OCL_NOT_అందుబాటు(); }
size_t Device::max_Constant_బఫరు_కొలత() స్థిరరాశి { OCL_NOT_అందుబాటు(); }

size_t Device::max_Mem_Alloc_కొలత() స్థిరరాశి { OCL_NOT_అందుబాటు(); }
size_t Device::max_Parameter_కొలత() స్థిరరాశి { OCL_NOT_అందుబాటు(); }

పూర్ణాంకం Device::max_Read_Image_వాదనలు() స్థిరరాశి { OCL_NOT_అందుబాటు(); }
పూర్ణాంకం Device::max_Write_Image_వాదనలు() స్థిరరాశి { OCL_NOT_అందుబాటు(); }
పూర్ణాంకం Device::maxSamplers() స్థిరరాశి { OCL_NOT_అందుబాటు(); }

size_t Device::max_Work_సమూహం_కొలత() స్థిరరాశి { OCL_NOT_అందుబాటు(); }
పూర్ణాంకం Device::max_Work_అంశం_Dims() స్థిరరాశి { OCL_NOT_అందుబాటు(); }
శూన్య Device::max_Work_అంశం_Sizes(size_t*) స్థిరరాశి { OCL_NOT_అందుబాటు(); }

పూర్ణాంకం Device::mem_పునాది_Addr_Align() స్థిరరాశి { OCL_NOT_అందుబాటు(); }

పూర్ణాంకం Device::native_సదిశ_వెడల్పు_Char() స్థిరరాశి { OCL_NOT_అందుబాటు(); }
పూర్ణాంకం Device::native_సదిశ_వెడల్పు_Short() స్థిరరాశి { OCL_NOT_అందుబాటు(); }
పూర్ణాంకం Device::native_సదిశ_వెడల్పు_Int() స్థిరరాశి { OCL_NOT_అందుబాటు(); }
పూర్ణాంకం Device::native_సదిశ_వెడల్పు_Long() స్థిరరాశి { OCL_NOT_అందుబాటు(); }
పూర్ణాంకం Device::native_సదిశ_వెడల్పు_Float() స్థిరరాశి { OCL_NOT_అందుబాటు(); }
పూర్ణాంకం Device::native_సదిశ_వెడల్పు_Double() స్థిరరాశి { OCL_NOT_అందుబాటు(); }
పూర్ణాంకం Device::native_సదిశ_వెడల్పు_Half() స్థిరరాశి { OCL_NOT_అందుబాటు(); }

పూర్ణాంకం Device::preferred_సదిశ_వెడల్పు_Char() స్థిరరాశి { OCL_NOT_అందుబాటు(); }
పూర్ణాంకం Device::preferred_సదిశ_వెడల్పు_Short() స్థిరరాశి { OCL_NOT_అందుబాటు(); }
పూర్ణాంకం Device::preferred_సదిశ_వెడల్పు_Int() స్థిరరాశి { OCL_NOT_అందుబాటు(); }
పూర్ణాంకం Device::preferred_సదిశ_వెడల్పు_Long() స్థిరరాశి { OCL_NOT_అందుబాటు(); }
పూర్ణాంకం Device::preferred_సదిశ_వెడల్పు_Float() స్థిరరాశి { OCL_NOT_అందుబాటు(); }
పూర్ణాంకం Device::preferred_సదిశ_వెడల్పు_Double() స్థిరరాశి { OCL_NOT_అందుబాటు(); }
పూర్ణాంకం Device::preferred_సదిశ_వెడల్పు_Half() స్థిరరాశి { OCL_NOT_అందుబాటు(); }

size_t Device::printf_బఫరు_కొలత() స్థిరరాశి { OCL_NOT_అందుబాటు(); }
size_t Device::profilingTimerResolution() స్థిరరాశి { OCL_NOT_అందుబాటు(); }

/* static */
స్థిరరాశి Device& Device::getDefault()
{
    స్థిర Device dummy;
    తిరిగి dummy;
}

/* static */ Device Device::fromHandle(శూన్య* d) { OCL_NOT_అందుబాటు(); }


Context::Context() CV_NOEXCEPT : p(ఖాళీ) { }
Context::Context(పూర్ణాంకం dtype) : p(ఖాళీ) { }
Context::~Context() { }
Context::Context(స్థిరరాశి Context& c) : p(ఖాళీ) { }
Context& Context::క్రియాకారకం=(స్థిరరాశి Context& c) { తిరిగి *ఇది; }
Context::Context(Context&&) CV_NOEXCEPT : p(ఖాళీ) { }
Context& Context::క్రియాకారకం=(Context&&) CV_NOEXCEPT { తిరిగి *ఇది; }

తార్కిక Context::create() { తిరిగి అబద్ధం; }
తార్కిక Context::create(పూర్ణాంకం dtype) { తిరిగి అబద్ధం; }
size_t Context::ndevices() స్థిరరాశి { తిరిగి 0; }
Device& Context::device(size_t idx) స్థిరరాశి { OCL_NOT_అందుబాటు(); }
కార్యక్రమం_2 Context::getProg(స్థిరరాశి కార్యక్రమం_ఆధారం& prog, స్థిరరాశి String& buildopt, String& errmsg) { OCL_NOT_అందుబాటు(); }
శూన్య Context::unloadProg(కార్యక్రమం_2& prog) { }

/* static */
Context& Context::getDefault()
{
    స్థిర Context dummy;
    తిరిగి dummy;
}
శూన్య* Context::ptr() స్థిరరాశి { తిరిగి ఖాళీ; }

శూన్య* Context::getOpenCLContextProperty(పూర్ణాంకం /*propertyId*/) స్థిరరాశి { OCL_NOT_అందుబాటు(); }

తార్కిక Context::useSVM() స్థిరరాశి { తిరిగి అబద్ధం; }
శూన్య Context::సమితి_Use_SVM(తార్కిక enabled) { }

Context::UserContext::~UserContext() { }

శూన్య Context::సమితి_User_Context(std::type_సూచీ /*typeId*/, స్థిరరాశి std::shared_ptr<Context::UserContext>& /*userContext*/) { OCL_NOT_అందుబాటు(); }
std::shared_ptr<Context::UserContext> Context::getUserContext(std::type_సూచీ /*typeId*/) { OCL_NOT_అందుబాటు(); }

/* static */ Context Context::fromHandle(శూన్య* context) { OCL_NOT_అందుబాటు(); }
/* static */ Context Context::fromDevice(స్థిరరాశి ocl::Device& device) { OCL_NOT_అందుబాటు(); }
/* static */ Context Context::create(స్థిరరాశి std::string& configuration) { OCL_NOT_అందుబాటు(); }

శూన్య Context::release() { }


Platform::Platform() CV_NOEXCEPT : p(ఖాళీ) { }
Platform::~Platform() { }
Platform::Platform(స్థిరరాశి Platform&) : p(ఖాళీ) { }
Platform& Platform::క్రియాకారకం=(స్థిరరాశి Platform&) { తిరిగి *ఇది; }
Platform::Platform(Platform&&) CV_NOEXCEPT : p(ఖాళీ) { }
Platform& Platform::క్రియాకారకం=(Platform&&) CV_NOEXCEPT { తిరిగి *ఇది; }

శూన్య* Platform::ptr() స్థిరరాశి { తిరిగి ఖాళీ; }

/* static */
Platform& Platform::getDefault()
{
    స్థిర Platform dummy;
    తిరిగి dummy;
}

శూన్య attachContext(స్థిరరాశి String& platformName, శూన్య* platformID, శూన్య* context, శూన్య* deviceID) { OCL_NOT_అందుబాటు(); }
శూన్య convert_From_బఫరు(శూన్య* cl_mem_బఫరు, size_t అడుగు, పూర్ణాంకం అడ్డువరుసలు, పూర్ణాంకం నిలువుల_సంఖ్య, పూర్ణాంకం type, UMat& dst) { OCL_NOT_అందుబాటు(); }
శూన్య convertFromImage(శూన్య* cl_mem_image, UMat& dst) { OCL_NOT_అందుబాటు(); }

శూన్య initializeContextFromHandle(Context& ctx, శూన్య* platform, శూన్య* context, శూన్య* device) { OCL_NOT_అందుబాటు(); }

వరుస::వరుస() CV_NOEXCEPT : p(ఖాళీ) { }
వరుస::వరుస(స్థిరరాశి Context& c, స్థిరరాశి Device& d) : p(ఖాళీ) { OCL_NOT_అందుబాటు(); }
వరుస::~వరుస() { }
వరుస::వరుస(స్థిరరాశి వరుస& q) {}
వరుస& వరుస::క్రియాకారకం=(స్థిరరాశి వరుస& q) { తిరిగి *ఇది; }
వరుస::వరుస(వరుస&&) CV_NOEXCEPT : p(ఖాళీ) { }
వరుస& వరుస::క్రియాకారకం=(వరుస&&) CV_NOEXCEPT { తిరిగి *ఇది; }

తార్కిక వరుస::create(స్థిరరాశి Context& c, స్థిరరాశి Device& d) { OCL_NOT_అందుబాటు(); }
శూన్య వరుస::ముగింపు_2() {}
శూన్య* వరుస::ptr() స్థిరరాశి { తిరిగి ఖాళీ; }
/* static */
వరుస& వరుస::getDefault()
{
    స్థిర వరుస dummy;
    తిరిగి dummy;
}

/// @brief Returns OpenCL command queue with enable profiling mode support
స్థిరరాశి వరుస& వరుస::get_Profiling_వరుస() స్థిరరాశి { OCL_NOT_అందుబాటు(); }


KernelArg::KernelArg() CV_NOEXCEPT
    : flags(0), m(0), obj(0), sz(0), wscale(1), iwscale(1)
{
}

KernelArg::KernelArg(పూర్ణాంకం _flags, UMat* _m, పూర్ణాంకం _wscale, పూర్ణాంకం _iwscale, స్థిరరాశి శూన్య* _obj, size_t _sz)
    : flags(_flags), m(_m), obj(_obj), sz(_sz), wscale(_wscale), iwscale(_iwscale)
{
    OCL_NOT_అందుబాటు();
}

KernelArg KernelArg::Constant(స్థిరరాశి Mat& m)
{
    OCL_NOT_అందుబాటు();
}


Kernel::Kernel() CV_NOEXCEPT : p(ఖాళీ) { }
Kernel::Kernel(స్థిరరాశి అక్షరం* kname, స్థిరరాశి కార్యక్రమం_2& prog) : p(ఖాళీ) { OCL_NOT_అందుబాటు(); }
Kernel::Kernel(స్థిరరాశి అక్షరం* kname, స్థిరరాశి కార్యక్రమం_ఆధారం& prog, స్థిరరాశి String& buildopts, String* errmsg) : p(ఖాళీ) { OCL_NOT_అందుబాటు(); }
Kernel::~Kernel() { }
Kernel::Kernel(స్థిరరాశి Kernel& k) : p(ఖాళీ) { }
Kernel& Kernel::క్రియాకారకం=(స్థిరరాశి Kernel& k) { తిరిగి *ఇది; }
Kernel::Kernel(Kernel&&) CV_NOEXCEPT : p(ఖాళీ) { }
Kernel& Kernel::క్రియాకారకం=(Kernel&&) CV_NOEXCEPT { తిరిగి *ఇది; }

తార్కిక Kernel::empty() స్థిరరాశి { తిరిగి నిజం; }
తార్కిక Kernel::create(స్థిరరాశి అక్షరం* kname, స్థిరరాశి కార్యక్రమం_2& prog) { OCL_NOT_అందుబాటు(); }
తార్కిక Kernel::create(స్థిరరాశి అక్షరం* kname, స్థిరరాశి కార్యక్రమం_ఆధారం& prog, స్థిరరాశి String& buildopts, String* errmsg) { OCL_NOT_అందుబాటు(); }

పూర్ణాంకం Kernel::సమితి(పూర్ణాంకం i, స్థిరరాశి శూన్య* value, size_t sz) { OCL_NOT_అందుబాటు(); }
పూర్ణాంకం Kernel::సమితి(పూర్ణాంకం i, స్థిరరాశి Image2D& image2D) { OCL_NOT_అందుబాటు(); }
పూర్ణాంకం Kernel::సమితి(పూర్ణాంకం i, స్థిరరాశి UMat& m) { OCL_NOT_అందుబాటు(); }
పూర్ణాంకం Kernel::సమితి(పూర్ణాంకం i, స్థిరరాశి KernelArg& arg) { OCL_NOT_అందుబాటు(); }

తార్కిక Kernel::నిరంతరం(పూర్ణాంకం dims, size_t globalsize[], size_t localsize[], తార్కిక సమకాలీకరణ, స్థిరరాశి వరుస& q) { OCL_NOT_అందుబాటు(); }
తార్కిక Kernel::నిరంతరం_Task(తార్కిక సమకాలీకరణ, స్థిరరాశి వరుస& q) { OCL_NOT_అందుబాటు(); }

int64 Kernel::నిరంతరం_Profiling(పూర్ణాంకం dims, size_t globalsize[], size_t localsize[], స్థిరరాశి వరుస& q) { OCL_NOT_అందుబాటు(); }

size_t Kernel::work_సమూహం_కొలత() స్థిరరాశి { OCL_NOT_అందుబాటు(); }
size_t Kernel::prefered_Work_సమూహం_కొలత_గుణిజం() స్థిరరాశి { OCL_NOT_అందుబాటు(); }
తార్కిక Kernel::compile_Work_సమూహం_కొలత(size_t wsz[]) స్థిరరాశి { OCL_NOT_అందుబాటు(); }
size_t Kernel::local_Mem_కొలత() స్థిరరాశి { OCL_NOT_అందుబాటు(); }

శూన్య* Kernel::ptr() స్థిరరాశి { తిరిగి ఖాళీ; }


కార్యక్రమం_2::కార్యక్రమం_2() CV_NOEXCEPT : p(ఖాళీ) { }
కార్యక్రమం_2::కార్యక్రమం_2(స్థిరరాశి కార్యక్రమం_ఆధారం& src, స్థిరరాశి String& buildflags, String& errmsg) : p(ఖాళీ) { OCL_NOT_అందుబాటు(); }
కార్యక్రమం_2::కార్యక్రమం_2(స్థిరరాశి కార్యక్రమం_2& prog) : p(ఖాళీ) { }
కార్యక్రమం_2& కార్యక్రమం_2::క్రియాకారకం=(స్థిరరాశి కార్యక్రమం_2& prog) { తిరిగి *ఇది; }
కార్యక్రమం_2::కార్యక్రమం_2(కార్యక్రమం_2&&) CV_NOEXCEPT : p(ఖాళీ) { }
కార్యక్రమం_2& కార్యక్రమం_2::క్రియాకారకం=(కార్యక్రమం_2&&) CV_NOEXCEPT { తిరిగి *ఇది; }
కార్యక్రమం_2::~కార్యక్రమం_2() { }

తార్కిక కార్యక్రమం_2::create(స్థిరరాశి కార్యక్రమం_ఆధారం& src, స్థిరరాశి String& buildflags, String& errmsg) { OCL_NOT_అందుబాటు(); }

శూన్య* కార్యక్రమం_2::ptr() స్థిరరాశి { తిరిగి ఖాళీ; }

శూన్య కార్యక్రమం_2::get_ద్విమాన(std::vector<అక్షరం>& binary) స్థిరరాశి { OCL_NOT_అందుబాటు(); }

తార్కిక కార్యక్రమం_2::పఠ(స్థిరరాశి String& buf, స్థిరరాశి String& buildflags) { OCL_NOT_అందుబాటు(); }
తార్కిక కార్యక్రమం_2::లిఖ(String& buf) స్థిరరాశి { OCL_NOT_అందుబాటు(); }
స్థిరరాశి కార్యక్రమం_ఆధారం& కార్యక్రమం_2::ఆధారం() స్థిరరాశి { OCL_NOT_అందుబాటు(); }
String కార్యక్రమం_2::get_ఉపసర్గ() స్థిరరాశి { OCL_NOT_అందుబాటు(); }
/* static */ String కార్యక్రమం_2::get_ఉపసర్గ(స్థిరరాశి String& buildflags) { OCL_NOT_అందుబాటు(); }


కార్యక్రమం_ఆధారం::కార్యక్రమం_ఆధారం() CV_NOEXCEPT : p(ఖాళీ) { }
కార్యక్రమం_ఆధారం::కార్యక్రమం_ఆధారం(స్థిరరాశి String& module, స్థిరరాశి String& name, స్థిరరాశి String& సంకేతం_Str, స్థిరరాశి String& సంకేతం_Hash) : p(ఖాళీ) { }
కార్యక్రమం_ఆధారం::కార్యక్రమం_ఆధారం(స్థిరరాశి String& prog) : p(ఖాళీ) { }
కార్యక్రమం_ఆధారం::కార్యక్రమం_ఆధారం(స్థిరరాశి అక్షరం* prog) : p(ఖాళీ) { }
కార్యక్రమం_ఆధారం::~కార్యక్రమం_ఆధారం() { }
కార్యక్రమం_ఆధారం::కార్యక్రమం_ఆధారం(స్థిరరాశి కార్యక్రమం_ఆధారం& prog) : p(ఖాళీ) { }
కార్యక్రమం_ఆధారం& కార్యక్రమం_ఆధారం::క్రియాకారకం=(స్థిరరాశి కార్యక్రమం_ఆధారం& prog) { తిరిగి *ఇది; }
కార్యక్రమం_ఆధారం::కార్యక్రమం_ఆధారం(కార్యక్రమం_ఆధారం&&) CV_NOEXCEPT : p(ఖాళీ) { }
కార్యక్రమం_ఆధారం& కార్యక్రమం_ఆధారం::క్రియాకారకం=(కార్యక్రమం_ఆధారం&&) CV_NOEXCEPT { తిరిగి *ఇది; }

స్థిరరాశి String& కార్యక్రమం_ఆధారం::ఆధారం() స్థిరరాశి { OCL_NOT_అందుబాటు(); }
కార్యక్రమం_ఆధారం::hash_t కార్యక్రమం_ఆధారం::hash() స్థిరరాశి { OCL_NOT_అందుబాటు(); }

/* static */ కార్యక్రమం_ఆధారం కార్యక్రమం_ఆధారం::from_ద్విమాన(స్థిరరాశి String& module, స్థిరరాశి String& name, స్థిరరాశి గుర్తులేని అక్షరం* binary, స్థిరరాశి size_t size, స్థిరరాశి cv::String& buildOptions) { OCL_NOT_అందుబాటు(); }
/* static */ కార్యక్రమం_ఆధారం కార్యక్రమం_ఆధారం::fromSPIR(స్థిరరాశి String& module, స్థిరరాశి String& name, స్థిరరాశి గుర్తులేని అక్షరం* binary, స్థిరరాశి size_t size, స్థిరరాశి cv::String& buildOptions) { OCL_NOT_అందుబాటు(); }


PlatformInfo::PlatformInfo() CV_NOEXCEPT : p(ఖాళీ) { }
PlatformInfo::PlatformInfo(శూన్య* id) : p(ఖాళీ) { OCL_NOT_అందుబాటు(); }
PlatformInfo::~PlatformInfo() { }

PlatformInfo::PlatformInfo(స్థిరరాశి PlatformInfo& i) : p(ఖాళీ) { }
PlatformInfo& PlatformInfo::క్రియాకారకం=(స్థిరరాశి PlatformInfo& i) { తిరిగి *ఇది; }
PlatformInfo::PlatformInfo(PlatformInfo&&) CV_NOEXCEPT : p(ఖాళీ) { }
PlatformInfo& PlatformInfo::క్రియాకారకం=(PlatformInfo&&) CV_NOEXCEPT { తిరిగి *ఇది; }

String PlatformInfo::name() స్థిరరాశి { OCL_NOT_అందుబాటు(); }
String PlatformInfo::vendor() స్థిరరాశి { OCL_NOT_అందుబాటు(); }
String PlatformInfo::సంస్కరణ() స్థిరరాశి { OCL_NOT_అందుబాటు(); }
పూర్ణాంకం PlatformInfo::device_సంఖ్య() స్థిరరాశి { OCL_NOT_అందుబాటు(); }
శూన్య PlatformInfo::getDevice(Device& device, పూర్ణాంకం d) స్థిరరాశి { OCL_NOT_అందుబాటు(); }

స్థిరరాశి అక్షరం* convertTypeStr(పూర్ణాంకం sdepth, పూర్ణాంకం ddepth, పూర్ణాంకం cn, అక్షరం* buf) { OCL_NOT_అందుబాటు(); }
స్థిరరాశి అక్షరం* type_గమ్యం_Str(పూర్ణాంకం t) { OCL_NOT_అందుబాటు(); }
స్థిరరాశి అక్షరం* memop_Type_గమ్యం_Str(పూర్ణాంకం t) { OCL_NOT_అందుబాటు(); }
స్థిరరాశి అక్షరం* vecop_Type_గమ్యం_Str(పూర్ణాంకం t) { OCL_NOT_అందుబాటు(); }
స్థిరరాశి అక్షరం* getOpenCLErrorString(పూర్ణాంకం error_సంకేతం) { OCL_NOT_అందుబాటు(); }
String kernel_గమ్యం_Str(ఆదానం_Array _kernel, పూర్ణాంకం ddepth, స్థిరరాశి అక్షరం* name) { OCL_NOT_అందుబాటు(); }
శూన్య getPlatfomsInfo(std::vector<PlatformInfo>& platform_info) { OCL_NOT_అందుబాటు(); }


పూర్ణాంకం predict_Optimal_సదిశ_వెడల్పు(ఆదానం_Array src1, ఆదానం_Array src2, ఆదానం_Array src3,
        ఆదానం_Array src4, ఆదానం_Array src5, ఆదానం_Array src6,
        ఆదానం_Array src7, ఆదానం_Array src8, ఆదానం_Array src9,
        Ocl_సదిశ_Strategy strat)
{ OCL_NOT_అందుబాటు(); }

పూర్ణాంకం check_Optimal_సదిశ_వెడల్పు(స్థిరరాశి పూర్ణాంకం *సదిశ_Widths,
        ఆదానం_Array src1, ఆదానం_Array src2, ఆదానం_Array src3,
        ఆదానం_Array src4, ఆదానం_Array src5, ఆదానం_Array src6,
        ఆదానం_Array src7, ఆదానం_Array src8, ఆదానం_Array src9,
        Ocl_సదిశ_Strategy strat)
{ OCL_NOT_అందుబాటు(); }

పూర్ణాంకం predict_Optimal_సదిశ_వెడల్పు_Max(ఆదానం_Array src1, ఆదానం_Array src2, ఆదానం_Array src3,
        ఆదానం_Array src4, ఆదానం_Array src5, ఆదానం_Array src6,
        ఆదానం_Array src7, ఆదానం_Array src8, ఆదానం_Array src9)
{ OCL_NOT_అందుబాటు(); }

శూన్య build_Options_కూడు_మాతృక_Description(String& buildOptions, స్థిరరాశి String& name, ఆదానం_Array _m) { OCL_NOT_అందుబాటు(); }


Image2D::Image2D() CV_NOEXCEPT : p(ఖాళీ) { }
Image2D::Image2D(స్థిరరాశి UMat &src, తార్కిక norm, తార్కిక alias) { OCL_NOT_అందుబాటు(); }
Image2D::Image2D(స్థిరరాశి Image2D & i) : p(ఖాళీ) { OCL_NOT_అందుబాటు(); }
Image2D::~Image2D() { }
Image2D& Image2D::క్రియాకారకం=(స్థిరరాశి Image2D & i) { తిరిగి *ఇది; }
Image2D::Image2D(Image2D&&) CV_NOEXCEPT : p(ఖాళీ) { }
Image2D& Image2D::క్రియాకారకం=(Image2D&&) CV_NOEXCEPT { తిరిగి *ఇది; }

/* static */ తార్కిక Image2D::canCreateAlias(స్థిరరాశి UMat &u) { OCL_NOT_అందుబాటు(); }
/* static */ తార్కిక Image2D::isFormatSupported(పూర్ణాంకం లోతు, పూర్ణాంకం cn, తార్కిక norm) { OCL_NOT_అందుబాటు(); }

శూన్య* Image2D::ptr() స్థిరరాశి { తిరిగి ఖాళీ; }


Timer::Timer(స్థిరరాశి వరుస& q) : p(ఖాళీ) {}
Timer::~Timer() {}
శూన్య Timer::ప్రారంభం() { OCL_NOT_అందుబాటు(); }
శూన్య Timer::stop() { OCL_NOT_అందుబాటు();}

uint64 Timer::durationNS() స్థిరరాశి { OCL_NOT_అందుబాటు(); }

MatAllocator* getOpenCLAllocator() { తిరిగి ఖాళీ; }

internal::కార్యక్రమం_నమోదు::క్రియాకారకం కార్యక్రమం_ఆధారం&() స్థిరరాశి { OCL_NOT_అందుబాటు(); }


నిర్మాణం OpenCLExecutionContext::Impl
{
    Impl() = అప్రమేయ;
};

Context& OpenCLExecutionContext::getContext() స్థిరరాశి { OCL_NOT_అందుబాటు(); }
Device& OpenCLExecutionContext::getDevice() స్థిరరాశి { OCL_NOT_అందుబాటు(); }
వరుస& OpenCLExecutionContext::get_వరుస() స్థిరరాశి { OCL_NOT_అందుబాటు(); }

తార్కిక OpenCLExecutionContext::useOpenCL() స్థిరరాశి { తిరిగి అబద్ధం; }
శూన్య OpenCLExecutionContext::సమితి_Use_Open_CL(తార్కిక flag) { }

స్థిర
OpenCLExecutionContext& getDummyOpenCLExecutionContext()
{
    స్థిర OpenCLExecutionContext dummy;
    తిరిగి dummy;
}

/* static */
OpenCLExecutionContext& OpenCLExecutionContext::getCurrent() { తిరిగి getDummyOpenCLExecutionContext(); }

/* static */
OpenCLExecutionContext& OpenCLExecutionContext::getCurrentRef() { తిరిగి getDummyOpenCLExecutionContext(); }

శూన్య OpenCLExecutionContext::bind() స్థిరరాశి { OCL_NOT_అందుబాటు(); }

OpenCLExecutionContext OpenCLExecutionContext::clone_With_New_వరుస(స్థిరరాశి ocl::వరుస& q) స్థిరరాశి { OCL_NOT_అందుబాటు(); }
OpenCLExecutionContext OpenCLExecutionContext::clone_With_New_వరుస() స్థిరరాశి { OCL_NOT_అందుబాటు(); }

/* static */ OpenCLExecutionContext OpenCLExecutionContext::create(స్థిరరాశి std::string& platformName, శూన్య* platformID, శూన్య* context, శూన్య* deviceID) { OCL_NOT_అందుబాటు(); }
/* static */ OpenCLExecutionContext OpenCLExecutionContext::create(స్థిరరాశి Context& context, స్థిరరాశి Device& device, స్థిరరాశి ocl::వరుస& వరుస_2) { OCL_NOT_అందుబాటు(); }
/* static */ OpenCLExecutionContext OpenCLExecutionContext::create(స్థిరరాశి Context& context, స్థిరరాశి Device& device) { OCL_NOT_అందుబాటు(); }

శూన్య OpenCLExecutionContext::release() { }

}}

#ఒకవేళ defined(_MSC_VER)
    #సూచన warning(pop)
#లేదావేళ defined(__clang__)
    #సూచన clang diagnostic pop
#లేదావేళ defined(__GNUC__)
    #సూచన GCC diagnostic pop
#అయితే_ముగింపు
