// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#अन्तर्भाव "opencv2/core/ocl_genbase.hpp"

#यदि defined(_MSC_VER)
    #निर्देशः warning(push)
    #निर्देशः warning(disable : 4100)
    #निर्देशः warning(disable : 4702)
#नोचेद्यदि defined(__clang__)
    #निर्देशः clang diagnostic push
    #निर्देशः clang diagnostic ignored "-Wunused-parameter"
#नोचेद्यदि defined(__GNUC__)
    #निर्देशः GCC diagnostic push
    #निर्देशः GCC diagnostic ignored "-Wunused-parameter"
#यदि_अन्तः

नामाकाश cv { नामाकाश ocl {

स्थिर
CV_NORETURN शून्य throw_no_ocl()
{
    CV_Error(Error::OpenCLApiCallError, "OpenCV build without OpenCL support");
}
#परिभाषय OCL_NOT_उपलब्धम्() throw_no_ocl();

CV_EXPORTS_W तार्किक haveOpenCL() { प्रत्यागम असत्य; }
CV_EXPORTS_W तार्किक useOpenCL() { प्रत्यागम असत्य; }
CV_EXPORTS_W तार्किक haveAmdBlas() { प्रत्यागम असत्य; }
CV_EXPORTS_W तार्किक haveAmdFft() { प्रत्यागम असत्य; }
CV_EXPORTS_W शून्य समुच्चयः_Use_Open_CL(तार्किक flag) { /* nothing */ }
CV_EXPORTS_W शून्य finish() { /* nothing */ }

CV_EXPORTS तार्किक haveSVM() { प्रत्यागम असत्य; }

Device::Device() CV_NOEXCEPT : p(रिक्त) { }
Device::Device(शून्य* d) : p(रिक्त) { OCL_NOT_उपलब्धम्(); }
Device::Device(ध्रुव Device& d) : p(रिक्त) { }
Device& Device::संक्रियकः=(ध्रुव Device& d) { प्रत्यागम *एतत्; }
Device::Device(Device&&) CV_NOEXCEPT : p(रिक्त) { }
Device& Device::संक्रियकः=(Device&&) CV_NOEXCEPT { प्रत्यागम *एतत्; }
Device::~Device() { }

शून्य Device::समुच्चयः(शून्य* d) { OCL_NOT_उपलब्धम्(); }

String Device::name() ध्रुव { OCL_NOT_उपलब्धम्(); }
String Device::extensions() ध्रुव { OCL_NOT_उपलब्धम्(); }
तार्किक Device::isExtensionSupported(ध्रुव String& extensionName) ध्रुव { OCL_NOT_उपलब्धम्(); }
String Device::संस्करणम्() ध्रुव { OCL_NOT_उपलब्धम्(); }
String Device::vendorName() ध्रुव { OCL_NOT_उपलब्धम्(); }
String Device::Open_CL_C_संस्करणम्() ध्रुव { OCL_NOT_उपलब्धम्(); }
String Device::OpenCLVersion() ध्रुव { OCL_NOT_उपलब्धम्(); }
पूर्णाङ्क Device::device_संस्करणम्_Major() ध्रुव { OCL_NOT_उपलब्धम्(); }
पूर्णाङ्क Device::device_संस्करणम्_Minor() ध्रुव { OCL_NOT_उपलब्धम्(); }
String Device::driver_संस्करणम्() ध्रुव { OCL_NOT_उपलब्धम्(); }
शून्य* Device::ptr() ध्रुव { /*OCL_NOT_AVAILABLE();*/ प्रत्यागम रिक्त; }

पूर्णाङ्क Device::type() ध्रुव { OCL_NOT_उपलब्धम्(); }

पूर्णाङ्क Device::स्थानाङ्कः_द्वयङ्काः() ध्रुव { OCL_NOT_उपलब्धम्(); }
तार्किक Device::उपलब्धम्() ध्रुव { OCL_NOT_उपलब्धम्(); }
तार्किक Device::compiler_उपलब्धम्() ध्रुव { OCL_NOT_उपलब्धम्(); }
तार्किक Device::linker_उपलब्धम्() ध्रुव { OCL_NOT_उपलब्धम्(); }

पूर्णाङ्क Device::doubleFPConfig() ध्रुव { OCL_NOT_उपलब्धम्(); }
पूर्णाङ्क Device::singleFPConfig() ध्रुव { OCL_NOT_उपलब्धम्(); }
पूर्णाङ्क Device::halfFPConfig() ध्रुव { OCL_NOT_उपलब्धम्(); }

तार्किक Device::hasFP64() ध्रुव { OCL_NOT_उपलब्धम्(); }
तार्किक Device::hasFP16() ध्रुव { OCL_NOT_उपलब्धम्(); }

तार्किक Device::endianLittle() ध्रुव { OCL_NOT_उपलब्धम्(); }
तार्किक Device::errorCorrectionSupport() ध्रुव { OCL_NOT_उपलब्धम्(); }

पूर्णाङ्क Device::executionCapabilities() ध्रुव { OCL_NOT_उपलब्धम्(); }

size_t Device::global_Mem_Cache_आकार() ध्रुव { OCL_NOT_उपलब्धम्(); }

पूर्णाङ्क Device::globalMemCacheType() ध्रुव { OCL_NOT_उपलब्धम्(); }
पूर्णाङ्क Device::global_Mem_Cache_पङ्क्तिः_आकार() ध्रुव { OCL_NOT_उपलब्धम्(); }
size_t Device::global_Mem_आकार() ध्रुव { OCL_NOT_उपलब्धम्(); }

size_t Device::local_Mem_आकार() ध्रुव { OCL_NOT_उपलब्धम्(); }
पूर्णाङ्क Device::localMemType() ध्रुव { प्रत्यागम NO_LOCAL_MEM; }
तार्किक Device::host_Unified_स्मृतिः_2() ध्रुव { OCL_NOT_उपलब्धम्(); }

तार्किक Device::imageSupport() ध्रुव { OCL_NOT_उपलब्धम्(); }

तार्किक Device::image_From_बफर_Support() ध्रुव { OCL_NOT_उपलब्धम्(); }
uint Device::imagePitchAlignment() ध्रुव { OCL_NOT_उपलब्धम्(); }
uint Device::image_आधारः_स्थानाङ्कः_Alignment() ध्रुव { OCL_NOT_उपलब्धम्(); }

तार्किक Device::intelSubgroupsSupport() ध्रुव { OCL_NOT_उपलब्धम्(); }

size_t Device::image2_DMax_विस्तारः() ध्रुव { OCL_NOT_उपलब्धम्(); }
size_t Device::image2DMaxHeight() ध्रुव { OCL_NOT_उपलब्धम्(); }

size_t Device::image3_DMax_विस्तारः() ध्रुव { OCL_NOT_उपलब्धम्(); }
size_t Device::image3DMaxHeight() ध्रुव { OCL_NOT_उपलब्धम्(); }
size_t Device::image3_DMax_गभीरता() ध्रुव { OCL_NOT_उपलब्धम्(); }

size_t Device::image_Max_बफर_आकार() ध्रुव { OCL_NOT_उपलब्धम्(); }
size_t Device::image_Max_Array_आकार() ध्रुव { OCL_NOT_उपलब्धम्(); }

पूर्णाङ्क Device::vendorID() ध्रुव { OCL_NOT_उपलब्धम्(); }

पूर्णाङ्क Device::maxClockFrequency() ध्रुव { OCL_NOT_उपलब्धम्(); }
पूर्णाङ्क Device::maxComputeUnits() ध्रुव { OCL_NOT_उपलब्धम्(); }
पूर्णाङ्क Device::max_Constant_तर्काः() ध्रुव { OCL_NOT_उपलब्धम्(); }
size_t Device::max_Constant_बफर_आकार() ध्रुव { OCL_NOT_उपलब्धम्(); }

size_t Device::max_Mem_Alloc_आकार() ध्रुव { OCL_NOT_उपलब्धम्(); }
size_t Device::max_Parameter_आकार() ध्रुव { OCL_NOT_उपलब्धम्(); }

पूर्णाङ्क Device::max_Read_Image_तर्काः() ध्रुव { OCL_NOT_उपलब्धम्(); }
पूर्णाङ्क Device::max_Write_Image_तर्काः() ध्रुव { OCL_NOT_उपलब्धम्(); }
पूर्णाङ्क Device::maxSamplers() ध्रुव { OCL_NOT_उपलब्धम्(); }

size_t Device::max_Work_गणः_आकार() ध्रुव { OCL_NOT_उपलब्धम्(); }
पूर्णाङ्क Device::maxWorkItemDims() ध्रुव { OCL_NOT_उपलब्धम्(); }
शून्य Device::maxWorkItemSizes(size_t*) ध्रुव { OCL_NOT_उपलब्धम्(); }

पूर्णाङ्क Device::mem_आधारः_Addr_Align() ध्रुव { OCL_NOT_उपलब्धम्(); }

पूर्णाङ्क Device::native_सदिशम्_विस्तारः_Char() ध्रुव { OCL_NOT_उपलब्धम्(); }
पूर्णाङ्क Device::native_सदिशम्_विस्तारः_Short() ध्रुव { OCL_NOT_उपलब्धम्(); }
पूर्णाङ्क Device::native_सदिशम्_विस्तारः_Int() ध्रुव { OCL_NOT_उपलब्धम्(); }
पूर्णाङ्क Device::native_सदिशम्_विस्तारः_Long() ध्रुव { OCL_NOT_उपलब्धम्(); }
पूर्णाङ्क Device::native_सदिशम्_विस्तारः_Float() ध्रुव { OCL_NOT_उपलब्धम्(); }
पूर्णाङ्क Device::native_सदिशम्_विस्तारः_Double() ध्रुव { OCL_NOT_उपलब्धम्(); }
पूर्णाङ्क Device::native_सदिशम्_विस्तारः_Half() ध्रुव { OCL_NOT_उपलब्धम्(); }

पूर्णाङ्क Device::preferred_सदिशम्_विस्तारः_Char() ध्रुव { OCL_NOT_उपलब्धम्(); }
पूर्णाङ्क Device::preferred_सदिशम्_विस्तारः_Short() ध्रुव { OCL_NOT_उपलब्धम्(); }
पूर्णाङ्क Device::preferred_सदिशम्_विस्तारः_Int() ध्रुव { OCL_NOT_उपलब्धम्(); }
पूर्णाङ्क Device::preferred_सदिशम्_विस्तारः_Long() ध्रुव { OCL_NOT_उपलब्धम्(); }
पूर्णाङ्क Device::preferred_सदिशम्_विस्तारः_Float() ध्रुव { OCL_NOT_उपलब्धम्(); }
पूर्णाङ्क Device::preferred_सदिशम्_विस्तारः_Double() ध्रुव { OCL_NOT_उपलब्धम्(); }
पूर्णाङ्क Device::preferred_सदिशम्_विस्तारः_Half() ध्रुव { OCL_NOT_उपलब्धम्(); }

size_t Device::printf_बफर_आकार() ध्रुव { OCL_NOT_उपलब्धम्(); }
size_t Device::profilingTimerResolution() ध्रुव { OCL_NOT_उपलब्धम्(); }

/* static */
ध्रुव Device& Device::getDefault()
{
    स्थिर Device dummy;
    प्रत्यागम dummy;
}

/* static */ Device Device::fromHandle(शून्य* d) { OCL_NOT_उपलब्धम्(); }


Context::Context() CV_NOEXCEPT : p(रिक्त) { }
Context::Context(पूर्णाङ्क dtype) : p(रिक्त) { }
Context::~Context() { }
Context::Context(ध्रुव Context& c) : p(रिक्त) { }
Context& Context::संक्रियकः=(ध्रुव Context& c) { प्रत्यागम *एतत्; }
Context::Context(Context&&) CV_NOEXCEPT : p(रिक्त) { }
Context& Context::संक्रियकः=(Context&&) CV_NOEXCEPT { प्रत्यागम *एतत्; }

तार्किक Context::create() { प्रत्यागम असत्य; }
तार्किक Context::create(पूर्णाङ्क dtype) { प्रत्यागम असत्य; }
size_t Context::ndevices() ध्रुव { प्रत्यागम 0; }
Device& Context::device(size_t idx) ध्रुव { OCL_NOT_उपलब्धम्(); }
क्रमादेशः_2 Context::getProg(ध्रुव क्रमादेशः_Source& prog, ध्रुव String& buildopt, String& errmsg) { OCL_NOT_उपलब्धम्(); }
शून्य Context::unloadProg(क्रमादेशः_2& prog) { }

/* static */
Context& Context::getDefault()
{
    स्थिर Context dummy;
    प्रत्यागम dummy;
}
शून्य* Context::ptr() ध्रुव { प्रत्यागम रिक्त; }

शून्य* Context::getOpenCLContextProperty(पूर्णाङ्क /*propertyId*/) ध्रुव { OCL_NOT_उपलब्धम्(); }

तार्किक Context::useSVM() ध्रुव { प्रत्यागम असत्य; }
शून्य Context::समुच्चयः_Use_SVM(तार्किक enabled) { }

Context::UserContext::~UserContext() { }

शून्य Context::समुच्चयः_User_Context(std::type_सूचकांक /*typeId*/, ध्रुव std::shared_ptr<Context::UserContext>& /*userContext*/) { OCL_NOT_उपलब्धम्(); }
std::shared_ptr<Context::UserContext> Context::getUserContext(std::type_सूचकांक /*typeId*/) { OCL_NOT_उपलब्धम्(); }

/* static */ Context Context::fromHandle(शून्य* context) { OCL_NOT_उपलब्धम्(); }
/* static */ Context Context::fromDevice(ध्रुव ocl::Device& device) { OCL_NOT_उपलब्धम्(); }
/* static */ Context Context::create(ध्रुव std::string& configuration) { OCL_NOT_उपलब्धम्(); }

शून्य Context::release() { }


Platform::Platform() CV_NOEXCEPT : p(रिक्त) { }
Platform::~Platform() { }
Platform::Platform(ध्रुव Platform&) : p(रिक्त) { }
Platform& Platform::संक्रियकः=(ध्रुव Platform&) { प्रत्यागम *एतत्; }
Platform::Platform(Platform&&) CV_NOEXCEPT : p(रिक्त) { }
Platform& Platform::संक्रियकः=(Platform&&) CV_NOEXCEPT { प्रत्यागम *एतत्; }

शून्य* Platform::ptr() ध्रुव { प्रत्यागम रिक्त; }

/* static */
Platform& Platform::getDefault()
{
    स्थिर Platform dummy;
    प्रत्यागम dummy;
}

शून्य attachContext(ध्रुव String& platformName, शून्य* platformID, शून्य* context, शून्य* deviceID) { OCL_NOT_उपलब्धम्(); }
शून्य convert_From_बफर(शून्य* cl_mem_बफर, size_t चरणः, पूर्णाङ्क पङ्क्तिकाः, पूर्णाङ्क स्तम्भसङ्ख्या, पूर्णाङ्क type, UMat& dst) { OCL_NOT_उपलब्धम्(); }
शून्य convertFromImage(शून्य* cl_mem_image, UMat& dst) { OCL_NOT_उपलब्धम्(); }

शून्य initializeContextFromHandle(Context& ctx, शून्य* platform, शून्य* context, शून्य* device) { OCL_NOT_उपलब्धम्(); }

Queue::Queue() CV_NOEXCEPT : p(रिक्त) { }
Queue::Queue(ध्रुव Context& c, ध्रुव Device& d) : p(रिक्त) { OCL_NOT_उपलब्धम्(); }
Queue::~Queue() { }
Queue::Queue(ध्रुव Queue& q) {}
Queue& Queue::संक्रियकः=(ध्रुव Queue& q) { प्रत्यागम *एतत्; }
Queue::Queue(Queue&&) CV_NOEXCEPT : p(रिक्त) { }
Queue& Queue::संक्रियकः=(Queue&&) CV_NOEXCEPT { प्रत्यागम *एतत्; }

तार्किक Queue::create(ध्रुव Context& c, ध्रुव Device& d) { OCL_NOT_उपलब्धम्(); }
शून्य Queue::finish() {}
शून्य* Queue::ptr() ध्रुव { प्रत्यागम रिक्त; }
/* static */
Queue& Queue::getDefault()
{
    स्थिर Queue dummy;
    प्रत्यागम dummy;
}

/// @brief Returns OpenCL command queue with enable profiling mode support
ध्रुव Queue& Queue::getProfilingQueue() ध्रुव { OCL_NOT_उपलब्धम्(); }


KernelArg::KernelArg() CV_NOEXCEPT
    : flags(0), m(0), obj(0), sz(0), wscale(1), iwscale(1)
{
}

KernelArg::KernelArg(पूर्णाङ्क _flags, UMat* _m, पूर्णाङ्क _wscale, पूर्णाङ्क _iwscale, ध्रुव शून्य* _obj, size_t _sz)
    : flags(_flags), m(_m), obj(_obj), sz(_sz), wscale(_wscale), iwscale(_iwscale)
{
    OCL_NOT_उपलब्धम्();
}

KernelArg KernelArg::Constant(ध्रुव Mat& m)
{
    OCL_NOT_उपलब्धम्();
}


Kernel::Kernel() CV_NOEXCEPT : p(रिक्त) { }
Kernel::Kernel(ध्रुव वर्ण* kname, ध्रुव क्रमादेशः_2& prog) : p(रिक्त) { OCL_NOT_उपलब्धम्(); }
Kernel::Kernel(ध्रुव वर्ण* kname, ध्रुव क्रमादेशः_Source& prog, ध्रुव String& buildopts, String* errmsg) : p(रिक्त) { OCL_NOT_उपलब्धम्(); }
Kernel::~Kernel() { }
Kernel::Kernel(ध्रुव Kernel& k) : p(रिक्त) { }
Kernel& Kernel::संक्रियकः=(ध्रुव Kernel& k) { प्रत्यागम *एतत्; }
Kernel::Kernel(Kernel&&) CV_NOEXCEPT : p(रिक्त) { }
Kernel& Kernel::संक्रियकः=(Kernel&&) CV_NOEXCEPT { प्रत्यागम *एतत्; }

तार्किक Kernel::empty() ध्रुव { प्रत्यागम सत्य; }
तार्किक Kernel::create(ध्रुव वर्ण* kname, ध्रुव क्रमादेशः_2& prog) { OCL_NOT_उपलब्धम्(); }
तार्किक Kernel::create(ध्रुव वर्ण* kname, ध्रुव क्रमादेशः_Source& prog, ध्रुव String& buildopts, String* errmsg) { OCL_NOT_उपलब्धम्(); }

पूर्णाङ्क Kernel::समुच्चयः(पूर्णाङ्क i, ध्रुव शून्य* value, size_t sz) { OCL_NOT_उपलब्धम्(); }
पूर्णाङ्क Kernel::समुच्चयः(पूर्णाङ्क i, ध्रुव Image2D& image2D) { OCL_NOT_उपलब्धम्(); }
पूर्णाङ्क Kernel::समुच्चयः(पूर्णाङ्क i, ध्रुव UMat& m) { OCL_NOT_उपलब्धम्(); }
पूर्णाङ्क Kernel::समुच्चयः(पूर्णाङ्क i, ध्रुव KernelArg& arg) { OCL_NOT_उपलब्धम्(); }

तार्किक Kernel::सततम्(पूर्णाङ्क dims, size_t globalsize[], size_t localsize[], तार्किक समकाल, ध्रुव Queue& q) { OCL_NOT_उपलब्धम्(); }
तार्किक Kernel::सततम्_Task(तार्किक समकाल, ध्रुव Queue& q) { OCL_NOT_उपलब्धम्(); }

int64 Kernel::सततम्_Profiling(पूर्णाङ्क dims, size_t globalsize[], size_t localsize[], ध्रुव Queue& q) { OCL_NOT_उपलब्धम्(); }

size_t Kernel::work_गणः_आकार() ध्रुव { OCL_NOT_उपलब्धम्(); }
size_t Kernel::prefered_Work_गणः_आकार_गुणजः() ध्रुव { OCL_NOT_उपलब्धम्(); }
तार्किक Kernel::compile_Work_गणः_आकार(size_t wsz[]) ध्रुव { OCL_NOT_उपलब्धम्(); }
size_t Kernel::local_Mem_आकार() ध्रुव { OCL_NOT_उपलब्धम्(); }

शून्य* Kernel::ptr() ध्रुव { प्रत्यागम रिक्त; }


क्रमादेशः_2::क्रमादेशः_2() CV_NOEXCEPT : p(रिक्त) { }
क्रमादेशः_2::क्रमादेशः_2(ध्रुव क्रमादेशः_Source& src, ध्रुव String& buildflags, String& errmsg) : p(रिक्त) { OCL_NOT_उपलब्धम्(); }
क्रमादेशः_2::क्रमादेशः_2(ध्रुव क्रमादेशः_2& prog) : p(रिक्त) { }
क्रमादेशः_2& क्रमादेशः_2::संक्रियकः=(ध्रुव क्रमादेशः_2& prog) { प्रत्यागम *एतत्; }
क्रमादेशः_2::क्रमादेशः_2(क्रमादेशः_2&&) CV_NOEXCEPT : p(रिक्त) { }
क्रमादेशः_2& क्रमादेशः_2::संक्रियकः=(क्रमादेशः_2&&) CV_NOEXCEPT { प्रत्यागम *एतत्; }
क्रमादेशः_2::~क्रमादेशः_2() { }

तार्किक क्रमादेशः_2::create(ध्रुव क्रमादेशः_Source& src, ध्रुव String& buildflags, String& errmsg) { OCL_NOT_उपलब्धम्(); }

शून्य* क्रमादेशः_2::ptr() ध्रुव { प्रत्यागम रिक्त; }

शून्य क्रमादेशः_2::get_द्विमान(std::vector<वर्ण>& binary) ध्रुव { OCL_NOT_उपलब्धम्(); }

तार्किक क्रमादेशः_2::पठ(ध्रुव String& buf, ध्रुव String& buildflags) { OCL_NOT_उपलब्धम्(); }
तार्किक क्रमादेशः_2::लिख(String& buf) ध्रुव { OCL_NOT_उपलब्धम्(); }
ध्रुव क्रमादेशः_Source& क्रमादेशः_2::source() ध्रुव { OCL_NOT_उपलब्धम्(); }
String क्रमादेशः_2::get_उपसर्गः() ध्रुव { OCL_NOT_उपलब्धम्(); }
/* static */ String क्रमादेशः_2::get_उपसर्गः(ध्रुव String& buildflags) { OCL_NOT_उपलब्धम्(); }


क्रमादेशः_Source::क्रमादेशः_Source() CV_NOEXCEPT : p(रिक्त) { }
क्रमादेशः_Source::क्रमादेशः_Source(ध्रुव String& module, ध्रुव String& name, ध्रुव String& संहिता_Str, ध्रुव String& संहिता_Hash) : p(रिक्त) { }
क्रमादेशः_Source::क्रमादेशः_Source(ध्रुव String& prog) : p(रिक्त) { }
क्रमादेशः_Source::क्रमादेशः_Source(ध्रुव वर्ण* prog) : p(रिक्त) { }
क्रमादेशः_Source::~क्रमादेशः_Source() { }
क्रमादेशः_Source::क्रमादेशः_Source(ध्रुव क्रमादेशः_Source& prog) : p(रिक्त) { }
क्रमादेशः_Source& क्रमादेशः_Source::संक्रियकः=(ध्रुव क्रमादेशः_Source& prog) { प्रत्यागम *एतत्; }
क्रमादेशः_Source::क्रमादेशः_Source(क्रमादेशः_Source&&) CV_NOEXCEPT : p(रिक्त) { }
क्रमादेशः_Source& क्रमादेशः_Source::संक्रियकः=(क्रमादेशः_Source&&) CV_NOEXCEPT { प्रत्यागम *एतत्; }

ध्रुव String& क्रमादेशः_Source::source() ध्रुव { OCL_NOT_उपलब्धम्(); }
क्रमादेशः_Source::hash_t क्रमादेशः_Source::hash() ध्रुव { OCL_NOT_उपलब्धम्(); }

/* static */ क्रमादेशः_Source क्रमादेशः_Source::from_द्विमान(ध्रुव String& module, ध्रुव String& name, ध्रुव अचिह्नित वर्ण* binary, ध्रुव size_t size, ध्रुव cv::String& buildOptions) { OCL_NOT_उपलब्धम्(); }
/* static */ क्रमादेशः_Source क्रमादेशः_Source::fromSPIR(ध्रुव String& module, ध्रुव String& name, ध्रुव अचिह्नित वर्ण* binary, ध्रुव size_t size, ध्रुव cv::String& buildOptions) { OCL_NOT_उपलब्धम्(); }


PlatformInfo::PlatformInfo() CV_NOEXCEPT : p(रिक्त) { }
PlatformInfo::PlatformInfo(शून्य* id) : p(रिक्त) { OCL_NOT_उपलब्धम्(); }
PlatformInfo::~PlatformInfo() { }

PlatformInfo::PlatformInfo(ध्रुव PlatformInfo& i) : p(रिक्त) { }
PlatformInfo& PlatformInfo::संक्रियकः=(ध्रुव PlatformInfo& i) { प्रत्यागम *एतत्; }
PlatformInfo::PlatformInfo(PlatformInfo&&) CV_NOEXCEPT : p(रिक्त) { }
PlatformInfo& PlatformInfo::संक्रियकः=(PlatformInfo&&) CV_NOEXCEPT { प्रत्यागम *एतत्; }

String PlatformInfo::name() ध्रुव { OCL_NOT_उपलब्धम्(); }
String PlatformInfo::vendor() ध्रुव { OCL_NOT_उपलब्धम्(); }
String PlatformInfo::संस्करणम्() ध्रुव { OCL_NOT_उपलब्धम्(); }
पूर्णाङ्क PlatformInfo::device_सङ्ख्या() ध्रुव { OCL_NOT_उपलब्धम्(); }
शून्य PlatformInfo::getDevice(Device& device, पूर्णाङ्क d) ध्रुव { OCL_NOT_उपलब्धम्(); }

ध्रुव वर्ण* convertTypeStr(पूर्णाङ्क sdepth, पूर्णाङ्क ddepth, पूर्णाङ्क cn, वर्ण* buf) { OCL_NOT_उपलब्धम्(); }
ध्रुव वर्ण* type_गन्तव्यम्_Str(पूर्णाङ्क t) { OCL_NOT_उपलब्धम्(); }
ध्रुव वर्ण* memop_Type_गन्तव्यम्_Str(पूर्णाङ्क t) { OCL_NOT_उपलब्धम्(); }
ध्रुव वर्ण* vecop_Type_गन्तव्यम्_Str(पूर्णाङ्क t) { OCL_NOT_उपलब्धम्(); }
ध्रुव वर्ण* getOpenCLErrorString(पूर्णाङ्क error_संहिता) { OCL_NOT_उपलब्धम्(); }
String kernel_गन्तव्यम्_Str(निवेशः_Array _kernel, पूर्णाङ्क ddepth, ध्रुव वर्ण* name) { OCL_NOT_उपलब्धम्(); }
शून्य getPlatfomsInfo(std::vector<PlatformInfo>& platform_info) { OCL_NOT_उपलब्धम्(); }


पूर्णाङ्क predict_Optimal_सदिशम्_विस्तारः(निवेशः_Array src1, निवेशः_Array src2, निवेशः_Array src3,
        निवेशः_Array src4, निवेशः_Array src5, निवेशः_Array src6,
        निवेशः_Array src7, निवेशः_Array src8, निवेशः_Array src9,
        Ocl_सदिशम्_Strategy strat)
{ OCL_NOT_उपलब्धम्(); }

पूर्णाङ्क check_Optimal_सदिशम्_विस्तारः(ध्रुव पूर्णाङ्क *सदिशम्_Widths,
        निवेशः_Array src1, निवेशः_Array src2, निवेशः_Array src3,
        निवेशः_Array src4, निवेशः_Array src5, निवेशः_Array src6,
        निवेशः_Array src7, निवेशः_Array src8, निवेशः_Array src9,
        Ocl_सदिशम्_Strategy strat)
{ OCL_NOT_उपलब्धम्(); }

पूर्णाङ्क predict_Optimal_सदिशम्_विस्तारः_Max(निवेशः_Array src1, निवेशः_Array src2, निवेशः_Array src3,
        निवेशः_Array src4, निवेशः_Array src5, निवेशः_Array src6,
        निवेशः_Array src7, निवेशः_Array src8, निवेशः_Array src9)
{ OCL_NOT_उपलब्धम्(); }

शून्य build_Options_सङ्कलय_Matrix_Description(String& buildOptions, ध्रुव String& name, निवेशः_Array _m) { OCL_NOT_उपलब्धम्(); }


Image2D::Image2D() CV_NOEXCEPT : p(रिक्त) { }
Image2D::Image2D(ध्रुव UMat &src, तार्किक norm, तार्किक alias) { OCL_NOT_उपलब्धम्(); }
Image2D::Image2D(ध्रुव Image2D & i) : p(रिक्त) { OCL_NOT_उपलब्धम्(); }
Image2D::~Image2D() { }
Image2D& Image2D::संक्रियकः=(ध्रुव Image2D & i) { प्रत्यागम *एतत्; }
Image2D::Image2D(Image2D&&) CV_NOEXCEPT : p(रिक्त) { }
Image2D& Image2D::संक्रियकः=(Image2D&&) CV_NOEXCEPT { प्रत्यागम *एतत्; }

/* static */ तार्किक Image2D::canCreateAlias(ध्रुव UMat &u) { OCL_NOT_उपलब्धम्(); }
/* static */ तार्किक Image2D::isFormatSupported(पूर्णाङ्क गभीरता, पूर्णाङ्क cn, तार्किक norm) { OCL_NOT_उपलब्धम्(); }

शून्य* Image2D::ptr() ध्रुव { प्रत्यागम रिक्त; }


Timer::Timer(ध्रुव Queue& q) : p(रिक्त) {}
Timer::~Timer() {}
शून्य Timer::start() { OCL_NOT_उपलब्धम्(); }
शून्य Timer::stop() { OCL_NOT_उपलब्धम्();}

uint64 Timer::durationNS() ध्रुव { OCL_NOT_उपलब्धम्(); }

MatAllocator* getOpenCLAllocator() { प्रत्यागम रिक्त; }

internal::क्रमादेशः_प्रविष्टिः::संक्रियकः क्रमादेशः_Source&() ध्रुव { OCL_NOT_उपलब्धम्(); }


संरचना OpenCLExecutionContext::Impl
{
    Impl() = स्वभाव;
};

Context& OpenCLExecutionContext::getContext() ध्रुव { OCL_NOT_उपलब्धम्(); }
Device& OpenCLExecutionContext::getDevice() ध्रुव { OCL_NOT_उपलब्धम्(); }
Queue& OpenCLExecutionContext::getQueue() ध्रुव { OCL_NOT_उपलब्धम्(); }

तार्किक OpenCLExecutionContext::useOpenCL() ध्रुव { प्रत्यागम असत्य; }
शून्य OpenCLExecutionContext::समुच्चयः_Use_Open_CL(तार्किक flag) { }

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

शून्य OpenCLExecutionContext::bind() ध्रुव { OCL_NOT_उपलब्धम्(); }

OpenCLExecutionContext OpenCLExecutionContext::cloneWithNewQueue(ध्रुव ocl::Queue& q) ध्रुव { OCL_NOT_उपलब्धम्(); }
OpenCLExecutionContext OpenCLExecutionContext::cloneWithNewQueue() ध्रुव { OCL_NOT_उपलब्धम्(); }

/* static */ OpenCLExecutionContext OpenCLExecutionContext::create(ध्रुव std::string& platformName, शून्य* platformID, शून्य* context, शून्य* deviceID) { OCL_NOT_उपलब्धम्(); }
/* static */ OpenCLExecutionContext OpenCLExecutionContext::create(ध्रुव Context& context, ध्रुव Device& device, ध्रुव ocl::Queue& queue) { OCL_NOT_उपलब्धम्(); }
/* static */ OpenCLExecutionContext OpenCLExecutionContext::create(ध्रुव Context& context, ध्रुव Device& device) { OCL_NOT_उपलब्धम्(); }

शून्य OpenCLExecutionContext::release() { }

}}

#यदि defined(_MSC_VER)
    #निर्देशः warning(pop)
#नोचेद्यदि defined(__clang__)
    #निर्देशः clang diagnostic pop
#नोचेद्यदि defined(__GNUC__)
    #निर्देशः GCC diagnostic pop
#यदि_अन्तः
