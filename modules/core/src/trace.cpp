// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ضمن "precomp.hpp"

#ضمن <opencv2/core/utils/trace.hpp>
#ضمن <opencv2/core/utils/trace.private.hpp>
#ضمن <opencv2/core/utils/configuration.private.hpp>

#ضمن <opencv2/core/opencl/ocl_defs.hpp>

#ضمن <cstdarg> // va_start

#ضمن <sstream>
#ضمن <ostream>
#ضمن <fstream>

#إذا 0
#تعریف_کن CV_LOG(...) CV_LOG_INFO(عدم, __VA_ARGS__)
#وإلا
#تعریف_کن CV_LOG(...) {}
#پایان_اگر

#إذا 0
#تعریف_کن CV_LOG_ITT(...) CV_LOG_INFO(عدم, __VA_ARGS__)
#وإلا
#تعریف_کن CV_LOG_ITT(...) {}
#پایان_اگر

#إذا 1
#تعریف_کن CV_LOG_TRACE_BAILOUT(برچسب, ...) CV_LOG_INFO(برچسب, __VA_ARGS__)
#وإلا
#تعریف_کن CV_LOG_TRACE_BAILOUT(...) {}
#پایان_اگر

#إذا 0
#تعریف_کن CV_LOG_PARALLEL(برچسب, ...) CV_LOG_INFO(برچسب, __VA_ARGS__)
#وإلا
#تعریف_کن CV_LOG_PARALLEL(...) {}
#پایان_اگر

#إذا 0
#تعریف_کن CV_LOG_CTX_STAT(برچسب, ...) CV_LOG_INFO(برچسب, __VA_ARGS__)
#وإلا
#تعریف_کن CV_LOG_CTX_STAT(...) {}
#پایان_اگر

#إذا 0
#تعریف_کن CV_LOG_SKIP(برچسب, ...) CV_LOG_INFO(برچسب, __VA_ARGS__)
#وإلا
#تعریف_کن CV_LOG_SKIP(...) {}
#پایان_اگر

نطاق cv {
نطاق utils {
نطاق trace {
نطاق details {

#اگر_تعریف_شده OPENCV_TRACE

#اگر_تعریف_شده _MSC_VER
#رهنمود warning(disable:4065) // switch statement contains 'default' but no 'case' labels
#پایان_اگر

ساكن منطقي getParameterTraceEnable()
{
    ساكن منطقي param_traceEnable = utils::getConfigurationParameterBool("OPENCV_TRACE", خطأ);
    أرجع param_traceEnable;
}

// TODO lazy configuration flags
ساكن صحيح param_max_Region_عمق_Open_CV = (صحيح)utils::get_Configuration_Parameter_اندازه_T("OPENCV_TRACE_DEPTH_OPENCV", 1);
ساكن صحيح param_maxRegionChildrenOpenCV = (صحيح)utils::get_Configuration_Parameter_اندازه_T("OPENCV_TRACE_MAX_CHILDREN_OPENCV", 1000);
ساكن صحيح param_maxRegionChildren = (صحيح)utils::get_Configuration_Parameter_اندازه_T("OPENCV_TRACE_MAX_CHILDREN", 10000);

ساكن ثابت cv::String& getParameterTraceLocation()
{
    ساكن cv::String param_traceLocation = utils::getConfigurationParameterString("OPENCV_TRACE_LOCATION", "OpenCVTrace");
    أرجع param_traceLocation;
}

#اگر_تعریف_شده HAVE_OPENCL
ساكن منطقي param_synchronizeOpenCL = utils::getConfigurationParameterBool("OPENCV_TRACE_SYNC_OPENCL", خطأ);
#پایان_اگر

#اگر_تعریف_شده OPENCV_WITH_ITT
ساكن منطقي param_ITT_ثبات_والد_Scope = utils::getConfigurationParameterBool("OPENCV_TRACE_ITT_PARENT", خطأ);
#پایان_اگر

ساكن ثابت حرف* _spaces(صحيح شمار_2)
{
    ساكن ثابت حرف buf[64] =
"                                                               ";
    أرجع &buf[63 - (شمار_2 & 63)];
}

/**
 * Text-based trace messages
 */
صنف Trace_پیام
{
عام:
    حرف میانگیر[1024];
    size_t len;
    منطقي hasError;

    Trace_پیام() :
        len(0),
        hasError(خطأ)
    {}

    منطقي چاپ_کن(ثابت حرف* format, ...)
    {
        حرف* buf = &میانگیر[len];
        size_t sz = حجم(میانگیر) - len;
        va_list ap;
        va_start(ap, format);
        صحيح n = cv_vsnprintf(buf, (صحيح)sz, format, ap);
        va_end(ap);
        إذا (n < 0 || (size_t)n > sz)
        {
            hasError = درست;
            أرجع خطأ;
        }
        len += n;
        أرجع درست;
    }

    منطقي formatlocation(ثابت Region::LocationStaticStorage& location)
    {
        أرجع هذا->چاپ_کن("l,%lld,\"%s\",%d,\"%s\",0x%llX\n",
                (دراز دراز صحيح)(*location.ppExtra)->global_location_id,
                location.filename,
                location.سطر,
                location.name,
                (دراز دراز صحيح)(location.flags & ~0xF0000000));
    }
    منطقي formatRegionEnter(ثابت Region& region)
    {
        منطقي ok = هذا->چاپ_کن("b,%d,%lld,%lld,%lld",
                (صحيح)region.pImpl->threadID,
                (دراز دراز صحيح)region.pImpl->beginTimestamp,
                (دراز دراز صحيح)((*region.pImpl->location.ppExtra)->global_location_id),
                (دراز دراز صحيح)region.pImpl->global_region_id);
        إذا (region.pImpl->والد_Region && region.pImpl->والد_Region->pImpl)
        {
            إذا (region.pImpl->والد_Region->pImpl->threadID != region.pImpl->threadID)
                ok &= هذا->چاپ_کن(",parentThread=%d,parent=%lld",
                        (صحيح)region.pImpl->والد_Region->pImpl->threadID,
                        (دراز دراز صحيح)region.pImpl->والد_Region->pImpl->global_region_id);
        }
        ok &= هذا->چاپ_کن("\n");
        أرجع ok;
    }
    منطقي formatRegionLeave(ثابت Region& region, ثابت RegionStatistics& نتیجه)
    {
        CV_DbgAssert(region.pImpl->انتها_Timestamp - region.pImpl->beginTimestamp == نتیجه.duration);
        منطقي ok = هذا->چاپ_کن("e,%d,%lld,%lld,%lld,%lld",
                (صحيح)region.pImpl->threadID,
                (دراز دراز صحيح)region.pImpl->انتها_Timestamp,
                (دراز دراز صحيح)(*region.pImpl->location.ppExtra)->global_location_id,
                (دراز دراز صحيح)region.pImpl->global_region_id,
                (دراز دراز صحيح)نتیجه.duration);
        إذا (نتیجه.currentSkippedRegions)
            ok &= هذا->چاپ_کن(",skip=%d", (صحيح)نتیجه.currentSkippedRegions);
#اگر_تعریف_شده HAVE_IPP
        إذا (نتیجه.durationImplIPP)
            ok &= هذا->چاپ_کن(",tIPP=%lld", (دراز دراز صحيح)نتیجه.durationImplIPP);
#پایان_اگر
#اگر_تعریف_شده HAVE_OPENCL
        إذا (نتیجه.durationImplOpenCL)
            ok &= هذا->چاپ_کن(",tOCL=%lld", (دراز دراز صحيح)نتیجه.durationImplOpenCL);
#پایان_اگر
        ok &= هذا->چاپ_کن("\n");
        أرجع ok;
    }
    منطقي رکورد_Region_Arg(ثابت Region& region, ثابت TraceArg& arg, ثابت حرف* value)
    {
        أرجع هذا->چاپ_کن("a,%d,%lld,%lld,\"%s\",\"%s\"\n",
                region.pImpl->threadID,
                (دراز دراز صحيح)region.pImpl->beginTimestamp,
                (دراز دراز صحيح)region.pImpl->global_region_id,
                arg.name,
                value);
    }
};


#اگر_تعریف_شده OPENCV_WITH_ITT
ساكن __itt_domain* domain = عدم;

ساكن منطقي isITTEnabled()
{
    ساكن متطاير منطقي isInitialized = خطأ;
    ساكن منطقي isEnabled = خطأ;
    إذا (!isInitialized)
    {
        cv::Auto_قفل lock(cv::getInitializationMutex());
        إذا (!isInitialized)
        {
            منطقي param_traceITTEnable = utils::getConfigurationParameterBool("OPENCV_TRACE_ITT_ENABLE", درست);
            إذا (param_traceITTEnable)
            {
                isEnabled = !!(__itt_api_version());
                CV_LOG_ITT("ITT is " << (isEnabled ? "enabled" : "disabled"));
                domain = __itt_domain_create("OpenCVTrace");
            }
            وإلا
            {
                CV_LOG_ITT("ITT is disabled through OpenCV parameter");
                isEnabled = خطأ;
            }
            isInitialized = درست;
        }
    }
    أرجع isEnabled;
}
#پایان_اگر


Region::Location_Extra_داده::Location_Extra_داده(ثابت LocationStaticStorage& location)
{
    CV_استفادهنشده(location);
    ساكن صحيح g_location_id_counter = 0;
    global_location_id = CV_XADD(&g_location_id_counter, 1) + 1;
    CV_LOG("Register location: " << global_location_id << " (" << (فراغ*)&location << ")"
            << std::پایان_سطر << "    file: " << location.filename
            << std::پایان_سطر << "    line: " << location.سطر
            << std::پایان_سطر << "    name: " << location.name);
#اگر_تعریف_شده OPENCV_WITH_ITT
    إذا (isITTEnabled())
    {
        // Caching is not required here, because there is builtin cache.
        // https://software.intel.com/en-us/node/544203:
        //     Consecutive calls to __itt_string_handle_create with the same name return the same value.
        ittHandle_name = __itt_string_handle_create(location.name);
        ittHandle_filename = __itt_string_handle_create(location.filename);
    }
    وإلا
    {
        ittHandle_name = 0;
        ittHandle_filename = 0;
    }
#پایان_اگر
}

/*static*/ Region::Location_Extra_داده* Region::Location_Extra_داده::init(ثابت Region::LocationStaticStorage& location)
{
    Location_Extra_داده** pLocationExtra = location.ppExtra;
    CV_DbgAssert(pLocationExtra);
    إذا (*pLocationExtra == عدم)
    {
        cv::Auto_قفل lock(cv::getInitializationMutex());
        إذا (*pLocationExtra == عدم)
        {
            *pLocationExtra = جديد Region::Location_Extra_داده(location);
            TraceStorage* s = getTraceManager().trace_storage.get();
            إذا (s)
            {
                Trace_پیام پیام;
                پیام.formatlocation(location);
                s->put(پیام);
            }
        }
    }
    أرجع *pLocationExtra;
}


Region::Impl::Impl(TraceManagerThreadLocal& ctx, Region* والد_Region_2, Region& region_, ثابت LocationStaticStorage& location_, int64 beginTimestamp_) :
    location(location_),
    region(region_),
    والد_Region(والد_Region_2),
    threadID(ctx.threadID),
    global_region_id(++ctx.region_counter),
    beginTimestamp(beginTimestamp_),
    انتها_Timestamp(0),
    مستقیم_Children_شمار(0)
#اگر_تعریف_شده OPENCV_WITH_ITT
    ,itt_id_registered(خطأ)
    ,itt_id(__itt_null)
#پایان_اگر
{
    CV_DbgAssert(ctx.current_فعال_Region == والد_Region);
    region.pImpl = هذا;

    ثبات_Region(ctx);

    enterRegion(ctx);
}

Region::Impl::~Impl()
{
#اگر_تعریف_شده OPENCV_WITH_ITT
    إذا (itt_id_registered)
    {
        CV_LOG_ITT(" Destroy ITT region: I=" << (فراغ*)هذا);
        __itt_id_destroy(domain, itt_id);
        itt_id_registered = خطأ;
    }
#پایان_اگر
    region.pImpl = عدم;
}

فراغ Region::Impl::enterRegion(TraceManagerThreadLocal& ctx)
{
    ctx.current_فعال_Region = &region;

    إذا (location.flags & REGION_FLAG_FUNCTION)
    {
        إذا ((location.flags & REGION_FLAG_APP_کد) == 0)
        {
            ctx.region_عمق_Open_CV++;
        }
        ctx.region_عمق++;
    }

    TraceStorage* s = ctx.getStorage();
    إذا (s)
    {
        Trace_پیام پیام;
        پیام.formatRegionEnter(region);
        s->put(پیام);
    }
#اگر_تعریف_شده OPENCV_WITH_ITT
    إذا (isITTEnabled())
    {
        __itt_id والد_ID = __itt_null;
        إذا (param_ITT_ثبات_والد_Scope && والد_Region && والد_Region->pImpl && والد_Region->pImpl->itt_id_registered && (location.flags & REGION_FLAG_REGION_FORCE) == 0)
            والد_ID = والد_Region->pImpl->itt_id;
        __itt_task_begin(domain, itt_id, والد_ID, (*location.ppExtra)->ittHandle_name);
    }
#پایان_اگر
}

فراغ Region::Impl::leaveRegion(TraceManagerThreadLocal& ctx)
{
    int64 duration = انتها_Timestamp - beginTimestamp; CV_استفادهنشده(duration);
    RegionStatistics نتیجه;
    ctx.stat.grab(نتیجه);
    ctx.مجموع_Skipped_Events += نتیجه.currentSkippedRegions;
    CV_LOG(_spaces(ctx.get_Current_عمق()*4) << "leaveRegion(): " << (فراغ*)هذا << " " << نتیجه);
#اگر_تعریف_شده OPENCV_WITH_ITT
    إذا (isITTEnabled())
    {
        إذا (نتیجه.currentSkippedRegions)
        {
            __itt_metadata_add(domain, itt_id, __itt_string_handle_create("skipped trace entries"), __itt_metadata_u32, 1, &نتیجه.currentSkippedRegions);
        }
#اگر_تعریف_شده HAVE_IPP
        إذا (نتیجه.durationImplIPP)
            __itt_metadata_add(domain, itt_id, __itt_string_handle_create("tIPP"), __itt_metadata_u64, 1, &نتیجه.durationImplIPP);
#پایان_اگر
#اگر_تعریف_شده HAVE_OPENCL
        إذا (نتیجه.durationImplOpenCL)
            __itt_metadata_add(domain, itt_id, __itt_string_handle_create("tOpenCL"), __itt_metadata_u64, 1, &نتیجه.durationImplOpenCL);
#پایان_اگر
        __itt_task_end(domain);
    }
#پایان_اگر
    TraceStorage* s = ctx.getStorage();
    إذا (s)
    {
        Trace_پیام پیام;
        پیام.formatRegionLeave(region, نتیجه);
        s->put(پیام);
    }

    إذا (location.flags & REGION_FLAG_FUNCTION)
    {
        إذا ((location.flags & REGION_FLAG_APP_کد) == 0)
        {
            ctx.region_عمق_Open_CV--;
        }
        ctx.region_عمق--;
    }

    ctx.current_فعال_Region = والد_Region;
}

فراغ Region::Impl::release()
{
    حذف هذا;
}

فراغ Region::Impl::ثبات_Region(TraceManagerThreadLocal& ctx)
{
#اگر_تعریف_شده OPENCV_WITH_ITT
    إذا (isITTEnabled())
    {
        إذا (!itt_id_registered)
        {
            CV_LOG_ITT(" Register ITT region: I=" << (فراغ*)هذا << " " << ctx.threadID << "-" << global_region_id);
#إذا 1 // workaround for some ITT backends
            itt_id = __itt_id_make((فراغ*)(intptr_t)(((int64)(ctx.threadID + 1) << 32) | global_region_id), global_region_id);
#وإلا
            itt_id = __itt_id_make((فراغ*)(intptr_t)(ctx.threadID + 1), global_region_id);
#پایان_اگر
            __itt_id_create(domain, itt_id);
            itt_id_registered = درست;
        }
    }
#وإلا
    CV_استفادهنشده(ctx);
#پایان_اگر
}

فراغ Region_Statistics_وضعیت::enable_Skip_نما(صحيح عمق)
{
    CV_DbgAssert(skip_عمق < 0);
    CV_LOG_SKIP(عدم, "SKIP-ENABLE: depth=" << عمق);
    skip_عمق = عمق;
}
فراغ Region_Statistics_وضعیت::check_Reset_Skip_نما(صحيح leave_عمق)
{
    إذا (leave_عمق <= skip_عمق)
    {
        CV_LOG_SKIP(عدم, "SKIP-RESET: leaveDepth=" << leave_عمق << " skipDepth=" << skip_عمق);
        skip_عمق = -1;
    }
}

Region::Region(ثابت LocationStaticStorage& location) :
    pImpl(عدم),
    implFlags(0)
{
    // Checks:
    // - global enable flag
    // - parent region is disabled
    // - children count threshold
    // - region location
    // - depth (opencv nested calls)
    إذا (!TraceManager::isActivated())
    {
        CV_LOG("Trace is disabled. Bailout");
        أرجع;
    }

    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    CV_LOG(_spaces(ctx.get_Current_عمق()*4) << "Region(): " << (فراغ*)هذا << ": " << location.name);

    Region* والد_Region = ctx.پشته_بالاترین_Region();
    ثابت Region::LocationStaticStorage* والد_Location = ctx.پشته_بالاترین_Location();

    إذا (location.flags & REGION_FLAG_REGION_بعدی)
    {
        إذا (والد_Region && والد_Region->pImpl)
        {
            CV_DbgAssert((والد_Region->pImpl->location.flags & REGION_FLAG_FUNCTION) == 0);
            والد_Region->destroy(); والد_Region->implFlags = 0;
            والد_Region = ctx.پشته_بالاترین_Region();
            والد_Location = ctx.پشته_بالاترین_Location();
        }
    }

    صحيح والد_Children = 0;
    إذا (والد_Region && والد_Region->pImpl)
    {
        إذا (والد_Location == عدم)
        {
            // parallel_for_body code path
            والد_Children = CV_XADD(&والد_Region->pImpl->مستقیم_Children_شمار, 1) + 1;
        }
        وإلا
        {
            والد_Children = ++والد_Region->pImpl->مستقیم_Children_شمار;
        }
    }

    int64 beginTimestamp = getTimestampNS();

    صحيح current_عمق = ctx.get_Current_عمق() + 1;
    بدل (location.flags & REGION_FLAG_IMPL_ماسک)
    {
#اگر_تعریف_شده HAVE_IPP
    حالة REGION_FLAG_IMPL_IPP:
        إذا (!ctx.stat_وضعیت.ignore_عمق_Impl_IPP)
            ctx.stat_وضعیت.ignore_عمق_Impl_IPP = current_عمق;
        اكسر;
#پایان_اگر
#اگر_تعریف_شده HAVE_OPENCL
    حالة REGION_FLAG_IMPL_OPENCL:
        إذا (!ctx.stat_وضعیت.ignore_عمق_Impl_Open_CL)
            ctx.stat_وضعیت.ignore_عمق_Impl_Open_CL = current_عمق;
        اكسر;
#پایان_اگر
    افتراضي:
        اكسر;
    }

    ctx.پشته_افزودن(هذا, &location, beginTimestamp);
    implFlags |= REGION_FLAG_NEED_پشته_برداشت;

    إذا ((location.flags & REGION_FLAG_REGION_FORCE) == 0)
    {
        إذا (ctx.stat_وضعیت.skip_عمق >= 0 && current_عمق > ctx.stat_وضعیت.skip_عمق)
        {
            CV_LOG(_spaces(ctx.get_Current_عمق()*4) << "Parent region is disabled. Bailout");
            ctx.stat.currentSkippedRegions++;
            أرجع;
        }

        إذا (param_maxRegionChildrenOpenCV > 0 && (location.flags & REGION_FLAG_APP_کد) == 0 && والد_Location && (والد_Location->flags & REGION_FLAG_APP_کد) == 0)
        {
            إذا (والد_Children >= param_maxRegionChildrenOpenCV)
            {
                CV_LOG_TRACE_BAILOUT(عدم, _spaces(ctx.get_Current_عمق()*4) << "OpenCV parent region exceeds children count. Bailout");
                ctx.stat_وضعیت.enable_Skip_نما(current_عمق - 1);
                ctx.stat.currentSkippedRegions++;
                DEBUG_ONLY(ctx.dump_پشته(std::خروجی, خطأ));
                أرجع;
            }
        }
        إذا (param_maxRegionChildren > 0 && والد_Children >= param_maxRegionChildren)
        {
            CV_LOG_TRACE_BAILOUT(عدم, _spaces(ctx.get_Current_عمق()*4) << "Parent region exceeds children count. Bailout");
            ctx.stat_وضعیت.enable_Skip_نما(current_عمق - 1);
            ctx.stat.currentSkippedRegions++;
            DEBUG_ONLY(ctx.dump_پشته(std::خروجی, خطأ));
            أرجع;
        }
    }

    Location_Extra_داده::init(location);

    إذا ((*location.ppExtra)->global_location_id == 0)
    {
        CV_LOG_TRACE_BAILOUT(عدم, _spaces(ctx.get_Current_عمق()*4) << "Region location is disabled. Bailout");
        ctx.stat_وضعیت.enable_Skip_نما(current_عمق);
        ctx.stat.currentSkippedRegions++;
        أرجع;
    }

    إذا (والد_Location && (والد_Location->flags & REGION_FLAG_SKIP_NESTED))
    {
        CV_LOG(_spaces(ctx.get_Current_عمق()*4) << "Parent region disables inner regions. Bailout");
        ctx.stat_وضعیت.enable_Skip_نما(current_عمق);
        ctx.stat.currentSkippedRegions++;
        أرجع;
    }

    إذا (param_max_Region_عمق_Open_CV)
    {
        إذا ((location.flags & REGION_FLAG_APP_کد) == 0)
        {
            إذا (ctx.region_عمق_Open_CV >= param_max_Region_عمق_Open_CV)
            {
                CV_LOG(_spaces(ctx.get_Current_عمق()*4) << "OpenCV region depth is exceed = " << ctx.region_عمق_Open_CV << ". Bailout");
                إذا (ctx.stat.currentSkippedRegions == 0)
                {
                    DEBUG_ONLY(ctx.dump_پشته(std::خروجی, خطأ));
                }
                ctx.stat_وضعیت.enable_Skip_نما(current_عمق);
                ctx.stat.currentSkippedRegions++;
                أرجع;
            }
        }
    }

    جديد Impl(ctx, والد_Region, *هذا, location, beginTimestamp);
    CV_DbgAssert(pImpl != عدم);
    implFlags |= REGION_FLAG_فعال;

    // parallel_for path
    إذا (والد_Region && والد_Region->pImpl)
    {
        إذا (والد_Location == عدم)
        {
            pImpl->مستقیم_Children_شمار = والد_Children;
        }
    }
}

فراغ Region::destroy()
{
    CV_DbgAssert(implFlags != 0);

    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    CV_LOG(_spaces(ctx.get_Current_عمق()*4) << "Region::destruct(): " << (فراغ*)هذا << " pImpl=" << pImpl << " implFlags=" << implFlags << ' ' << (ctx.پشته_بالاترین_Location() ? ctx.پشته_بالاترین_Location()->name : "<unknown>"));

    CV_DbgAssert(implFlags & REGION_FLAG_NEED_پشته_برداشت);
    ثابت صحيح current_عمق = ctx.get_Current_عمق(); CV_استفادهنشده(current_عمق);

    CV_LOG_CTX_STAT(عدم, _spaces(current_عمق*4) << ctx.stat << ' ' << ctx.stat_وضعیت);

    ثابت Region::LocationStaticStorage* location = ctx.پشته_بالاترین_Location();
    Impl::Optimization_مسیر my_کد_مسیر = Impl::کد_مسیر_PLAIN;
    إذا (location)
    {
        بدل (location->flags & REGION_FLAG_IMPL_ماسک)
        {
#اگر_تعریف_شده HAVE_IPP
        حالة REGION_FLAG_IMPL_IPP:
            my_کد_مسیر = Impl::کد_مسیر_IPP;
            اكسر;
#پایان_اگر
#اگر_تعریف_شده HAVE_OPENCL
        حالة REGION_FLAG_IMPL_OPENCL:
            إذا (param_synchronizeOpenCL && cv::ocl::isOpenCLActivated())
                cv::ocl::پایان_2();
            my_کد_مسیر = Impl::کد_مسیر_OPENCL;
            اكسر;
#پایان_اگر
        افتراضي:
            اكسر;
        }
    }

    int64 انتها_Timestamp = getTimestampNS();
    int64 duration = انتها_Timestamp - ctx.پشته_بالاترین_Begin_Timestamp();

    منطقي فعال = is_فعال_2();

    إذا (فعال)
        ctx.stat.duration = duration;
    وإلا إذا (ctx.پشته.size() == ctx.parallel_for_پشته_اندازه + 1)
        ctx.stat.duration += duration;

    بدل (my_کد_مسیر) {
        حالة Impl::کد_مسیر_PLAIN:
            // nothing
            اكسر;
#اگر_تعریف_شده HAVE_IPP
        حالة Impl::کد_مسیر_IPP:
            إذا (ctx.stat_وضعیت.ignore_عمق_Impl_IPP == current_عمق)
            {
                ctx.stat.durationImplIPP += duration;
                ctx.stat_وضعیت.ignore_عمق_Impl_IPP = 0;
            }
            وإلا إذا (فعال)
            {
                ctx.stat.durationImplIPP = duration;
            }
            اكسر;
#پایان_اگر
#اگر_تعریف_شده HAVE_OPENCL
        حالة Impl::کد_مسیر_OPENCL:
            إذا (ctx.stat_وضعیت.ignore_عمق_Impl_Open_CL == current_عمق)
            {
                ctx.stat.durationImplOpenCL += duration;
                ctx.stat_وضعیت.ignore_عمق_Impl_Open_CL = 0;
            }
            وإلا إذا (فعال)
            {
                ctx.stat.durationImplOpenCL = duration;
            }
            اكسر;
#پایان_اگر
        افتراضي:
            اكسر;
    }

    إذا (pImpl)
    {
        CV_DbgAssert((implFlags & (REGION_FLAG_فعال | REGION_FLAG_NEED_پشته_برداشت)) == (REGION_FLAG_فعال | REGION_FLAG_NEED_پشته_برداشت));
        CV_DbgAssert(ctx.پشته_بالاترین_Region() == هذا);
        pImpl->انتها_Timestamp = انتها_Timestamp;
        pImpl->leaveRegion(ctx);
        pImpl->release();
        pImpl = عدم;
        DEBUG_ONLY(implFlags &= ~REGION_FLAG_فعال);
    }
    وإلا
    {
        CV_DbgAssert(ctx.stat_وضعیت.skip_عمق <= current_عمق);
    }

    إذا (implFlags & REGION_FLAG_NEED_پشته_برداشت)
    {
        CV_DbgAssert(ctx.پشته_بالاترین_Region() == هذا);
        ctx.پشته_برداشت();
        ctx.stat_وضعیت.check_Reset_Skip_نما(current_عمق);
        DEBUG_ONLY(implFlags &= ~REGION_FLAG_NEED_پشته_برداشت);
    }
    CV_LOG_CTX_STAT(عدم, _spaces(current_عمق*4) << "===> " << ctx.stat << ' ' << ctx.stat_وضعیت);
}


TraceManagerThreadLocal::~TraceManagerThreadLocal()
{
}

فراغ TraceManagerThreadLocal::dump_پشته(std::ostream& out, منطقي onlyFunctions) ثابت
{
    std::stringstream ss;
    std::deque<پشته_مدخل>::const_iterator it = پشته.begin();
    std::deque<پشته_مدخل>::const_iterator end = پشته.end();
    صحيح عمق = 0;
    لكل (; it != end; ++it)
    {
        ثابت Region::LocationStaticStorage* location = it->location;
        إذا (location)
        {
            إذا (!onlyFunctions || (location->flags & REGION_FLAG_FUNCTION))
            {
                ss << _spaces(4*عمق) << location->name << std::پایان_سطر;
                عمق++;
            }
        }
        وإلا
        {
            ss << _spaces(4*عمق) << "<unknown>" << std::پایان_سطر;
            عمق++;
        }
    }
    out << ss.str();
}

صنف AsyncTraceStorage CV_FINAL : عام TraceStorage
{
    تغییرپذیر std::ofstream out;
عام:
    ثابت std::string name;

    AsyncTraceStorage(ثابت std::string& filename) :
        out(filename.c_str(), std::ios::trunc),
        name(filename)
    {
        out << "#description: OpenCV trace file" << std::پایان_سطر;
        out << "#version: 1.0" << std::پایان_سطر;
    }
    ~AsyncTraceStorage()
    {
        out.close();
    }

    منطقي put(ثابت Trace_پیام& پیام) ثابت CV_OVERRIDE
    {
        إذا (پیام.hasError)
            أرجع خطأ;
        out << پیام.میانگیر;
        //DEBUG_ONLY(std::flush(out)); // TODO configure flag
        أرجع درست;
    }
};

صنف SyncTraceStorage CV_FINAL : عام TraceStorage
{
    تغییرپذیر std::ofstream out;
    تغییرپذیر cv::Mutex mutex;
عام:
    ثابت std::string name;

    SyncTraceStorage(ثابت std::string& filename) :
        out(filename.c_str(), std::ios::trunc),
        name(filename)
    {
        out << "#description: OpenCV trace file" << std::پایان_سطر;
        out << "#version: 1.0" << std::پایان_سطر;
    }
    ~SyncTraceStorage()
    {
        cv::Auto_قفل l(mutex);
        out.close();
    }

    منطقي put(ثابت Trace_پیام& پیام) ثابت CV_OVERRIDE
    {
        إذا (پیام.hasError)
            أرجع خطأ;
        {
            cv::Auto_قفل l(mutex);
            out << پیام.میانگیر;
            std::flush(out); // TODO configure flag
        }
        أرجع درست;
    }
};


TraceStorage* TraceManagerThreadLocal::getStorage() ثابت
{
    // TODO configuration option for stdout/single trace file
    إذا (storage.empty())
    {
        TraceStorage* global = getTraceManager().trace_storage.get();
        إذا (global)
        {
            ثابت std::string filepath = cv::format("%s-%03d.txt", getParameterTraceLocation().c_str(), threadID).c_str();
            Trace_پیام پیام;
            ثابت حرف* pos = strrchr(filepath.c_str(), '/'); // extract filename
#اگر_تعریف_شده _WIN32
            إذا (!pos)
                pos = strrchr(filepath.c_str(), '\\');
#پایان_اگر
            إذا (!pos)
                pos = filepath.c_str();
            وإلا
                pos += 1; // fix to skip extra slash in filename beginning
            پیام.چاپ_کن("#thread file: %s\n", pos);
            global->put(پیام);
            storage.reset(جديد AsyncTraceStorage(filepath));
        }
    }
    أرجع storage.get();
}



ساكن منطقي activated = خطأ;
ساكن منطقي isInitialized = خطأ;

TraceManager::TraceManager()
{
    (فراغ)cv::getTimestampNS();

    isInitialized = درست;
    CV_LOG("TraceManager ctor: " << (فراغ*)هذا);

    CV_LOG("TraceManager configure()");
    activated = getParameterTraceEnable();

    إذا (activated)
        trace_storage.reset(جديد SyncTraceStorage(std::string(getParameterTraceLocation()) + ".txt"));

#اگر_تعریف_شده OPENCV_WITH_ITT
    إذا (isITTEnabled())
    {
        activated = درست; // force trace pipeline activation (without OpenCV storage)
        __itt_region_begin(domain, __itt_null, __itt_null, __itt_string_handle_create("OpenCVTrace"));
    }
#پایان_اگر
}
TraceManager::~TraceManager()
{
    CV_LOG("TraceManager dtor: " << (فراغ*)هذا);

#اگر_تعریف_شده OPENCV_WITH_ITT
    إذا (isITTEnabled())
    {
        __itt_region_end(domain, __itt_null);
    }
#پایان_اگر

    std::vector<TraceManagerThreadLocal*> نخها_ctx;
    tls.gather(نخها_ctx);
    size_t مجموع_Events = 0, مجموع_Skipped_Events = 0;
    لكل (size_t i = 0; i < نخها_ctx.size(); i++)
    {
        TraceManagerThreadLocal* ctx = نخها_ctx[i];
        إذا (ctx)
        {
            مجموع_Events += ctx->region_counter;
            مجموع_Skipped_Events += ctx->مجموع_Skipped_Events;
        }
    }
    إذا (مجموع_Events || activated)
    {
        CV_LOG_INFO(عدم, "Trace: Total events: " << مجموع_Events);
    }
    إذا (مجموع_Skipped_Events)
    {
        CV_LOG_WARNING(عدم, "Trace: Total skipped events: " << مجموع_Skipped_Events);
    }

    // This is a global static object, so process starts shutdown here
    // Turn off trace
    cv::__termination = درست; // also set in DllMain() notifications handler for DLL_PROCESS_DETACH
    activated = خطأ;
}

منطقي TraceManager::isActivated()
{
    // Check if process starts shutdown, and set earlyExit to true
    // to prevent further instrumentation processing earlier.
    إذا (cv::__termination)
    {
        activated = خطأ;
        أرجع خطأ;
    }

    إذا (!isInitialized)
    {
        TraceManager& m = getTraceManager();
        CV_استفادهنشده(m); // TODO
    }

    أرجع activated;
}


ساكن TraceManager* getTraceManagerCallOnce()
{
    ساكن TraceManager globalInstance;
    أرجع &globalInstance;
}
TraceManager& getTraceManager()
{
    CV_SINGLETON_LAZY_INIT_REF(TraceManager, getTraceManagerCallOnce())
}

فراغ parallel_For_مجموعه_ریشه_Region(ثابت Region& ریشه_Region, ثابت TraceManagerThreadLocal& ریشه_ctx)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();

    إذا (ctx.dummy_پشته_بالاترین.region == &ریشه_Region) // already attached
        أرجع;

    CV_Assert(ctx.dummy_پشته_بالاترین.region == عدم);
    ctx.dummy_پشته_بالاترین = TraceManagerThreadLocal::پشته_مدخل(const_cast<Region*>(&ریشه_Region), عدم, -1);

    إذا (&ctx == &ریشه_ctx)
    {
        ctx.stat.grab(ctx.parallel_for_stat);
        ctx.parallel_for_stat_وضعیت = ctx.stat_وضعیت;
        ctx.parallel_for_پشته_اندازه = ctx.پشته.size();
        أرجع;
    }

    CV_Assert(ctx.پشته.empty());

    ctx.current_فعال_Region = const_cast<Region*>(&ریشه_Region);

    ctx.region_عمق = ریشه_ctx.region_عمق;
    ctx.region_عمق_Open_CV = ریشه_ctx.region_عمق_Open_CV;

    ctx.parallel_for_پشته_اندازه = 0;

    ctx.stat_وضعیت.propagateFrom(ریشه_ctx.stat_وضعیت);
}

فراغ parallelForAttachNestedRegion(ثابت Region& ریشه_Region)
{
    CV_استفادهنشده(ریشه_Region);
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();

    CV_DbgAssert(ctx.dummy_پشته_بالاترین.region == &ریشه_Region);

    Region* region = ctx.get_Current_فعال_Region();
    CV_LOG_PARALLEL(عدم, " PARALLEL_FOR: " << (فراغ*)region << " ==> " << &ریشه_Region);
    إذا (!region)
        أرجع;

#اگر_تعریف_شده OPENCV_WITH_ITT
    إذا (!ریشه_Region.pImpl || !ریشه_Region.pImpl->itt_id_registered)
        أرجع;

    إذا (!region->pImpl)
        أرجع;

    CV_LOG_PARALLEL(عدم, " PARALLEL_FOR ITT: " << (فراغ*)ریشه_Region.pImpl->itt_id.d1 << ":" << ریشه_Region.pImpl->itt_id.d2 << ":" << (فراغ*)ریشه_Region.pImpl->itt_id.d3 << " => "
                                 << (فراغ*)region->pImpl->itt_id.d1 << ":" << region->pImpl->itt_id.d2 << ":" << (فراغ*)region->pImpl->itt_id.d3);
    __itt_relation_add(domain, region->pImpl->itt_id, __itt_relation_is_child_of, ریشه_Region.pImpl->itt_id);
#پایان_اگر
}

فراغ parallelForFinalize(ثابت Region& ریشه_Region)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();

    int64 انتها_Timestamp = getTimestampNS();
    int64 duration = انتها_Timestamp - ctx.پشته_بالاترین_Begin_Timestamp();
    CV_LOG_PARALLEL(عدم, "parallel_for duration: " << duration << " " << &ریشه_Region);

    std::vector<TraceManagerThreadLocal*> نخها_ctx;
    getTraceManager().tls.gather(نخها_ctx);
    RegionStatistics parallel_for_stat;
    لكل (size_t i = 0; i < نخها_ctx.size(); i++)
    {
        TraceManagerThreadLocal* فرزند_ctx = نخها_ctx[i];

        إذا (فرزند_ctx && فرزند_ctx->پشته_بالاترین_Region() == &ریشه_Region)
        {
            CV_LOG_PARALLEL(عدم, "Thread=" << فرزند_ctx->threadID << " " << فرزند_ctx->stat);
            RegionStatistics فرزند_stat;
            فرزند_ctx->stat.grab(فرزند_stat);
            parallel_for_stat.append(فرزند_stat);
            إذا (فرزند_ctx != &ctx)
            {
                فرزند_ctx->dummy_پشته_بالاترین = TraceManagerThreadLocal::پشته_مدخل();
            }
            وإلا
            {
                ctx.parallel_for_stat.grab(ctx.stat);
                ctx.stat_وضعیت = ctx.parallel_for_stat_وضعیت;
                فرزند_ctx->dummy_پشته_بالاترین = TraceManagerThreadLocal::پشته_مدخل();
            }
        }
    }

    عائم parallel_coeff = std::min(1.0f, duration / (عائم)(parallel_for_stat.duration));
    CV_LOG_PARALLEL(عدم, "parallel_coeff=" << 1.0f / parallel_coeff);
    CV_LOG_PARALLEL(عدم, parallel_for_stat);
    إذا (parallel_coeff != 1.0f)
    {
        parallel_for_stat.multiply(parallel_coeff);
        CV_LOG_PARALLEL(عدم, parallel_for_stat);
    }
    parallel_for_stat.duration = 0;
    ctx.stat.append(parallel_for_stat);
    CV_LOG_PARALLEL(عدم, ctx.stat);
}

بنية TraceArg::Extra_داده
{
#اگر_تعریف_شده OPENCV_WITH_ITT
    // Special fields for ITT
    __itt_string_handle* متطاير ittHandle_name;
#پایان_اگر
    Extra_داده(TraceManagerThreadLocal& ctx, ثابت TraceArg& arg)
    {
        CV_استفادهنشده(ctx); CV_استفادهنشده(arg);
#اگر_تعریف_شده OPENCV_WITH_ITT
        إذا (isITTEnabled())
        {
            // Caching is not required here, because there is builtin cache.
            // https://software.intel.com/en-us/node/544203:
            //     Consecutive calls to __itt_string_handle_create with the same name return the same value.
            ittHandle_name = __itt_string_handle_create(arg.name);
        }
        وإلا
        {
            ittHandle_name = 0;
        }
#پایان_اگر
    }
};

ساكن فراغ initTraceArg(TraceManagerThreadLocal& ctx, ثابت TraceArg& arg)
{
    TraceArg::Extra_داده** pExtra = arg.ppExtra;
    إذا (*pExtra == عدم)
    {
        cv::Auto_قفل lock(cv::getInitializationMutex());
        إذا (*pExtra == عدم)
        {
            *pExtra = جديد TraceArg::Extra_داده(ctx, arg);
        }
    }
}
فراغ traceArg(ثابت TraceArg& arg, ثابت حرف* value)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    Region* region = ctx.get_Current_فعال_Region();
    إذا (!region)
        أرجع;
    CV_Assert(region->pImpl);
    initTraceArg(ctx, arg);
    إذا (!value)
        value = "<null>";
#اگر_تعریف_شده OPENCV_WITH_ITT
    إذا (isITTEnabled())
    {
        __itt_metadata_str_add(domain, region->pImpl->itt_id, (*arg.ppExtra)->ittHandle_name, value, طول_رشته(value));
    }
#پایان_اگر
}
فراغ traceArg(ثابت TraceArg& arg, صحيح value)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    Region* region = ctx.get_Current_فعال_Region();
    إذا (!region)
        أرجع;
    CV_Assert(region->pImpl);
    initTraceArg(ctx, arg);
#اگر_تعریف_شده OPENCV_WITH_ITT
    إذا (isITTEnabled())
    {
        __itt_metadata_add(domain, region->pImpl->itt_id, (*arg.ppExtra)->ittHandle_name, حجم(صحيح) == 4 ? __itt_metadata_s32 : __itt_metadata_s64, 1, &value);
    }
#وإلا
    CV_استفادهنشده(value);
#پایان_اگر
}
فراغ traceArg(ثابت TraceArg& arg, int64 value)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    Region* region = ctx.get_Current_فعال_Region();
    إذا (!region)
        أرجع;
    CV_Assert(region->pImpl);
    initTraceArg(ctx, arg);
#اگر_تعریف_شده OPENCV_WITH_ITT
    إذا (isITTEnabled())
    {
        __itt_metadata_add(domain, region->pImpl->itt_id, (*arg.ppExtra)->ittHandle_name, __itt_metadata_s64, 1, &value);
    }
#وإلا
    CV_استفادهنشده(value);
#پایان_اگر
}
فراغ traceArg(ثابت TraceArg& arg, مزدوج value)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    Region* region = ctx.get_Current_فعال_Region();
    إذا (!region)
        أرجع;
    CV_Assert(region->pImpl);
    initTraceArg(ctx, arg);
#اگر_تعریف_شده OPENCV_WITH_ITT
    إذا (isITTEnabled())
    {
        __itt_metadata_add(domain, region->pImpl->itt_id, (*arg.ppExtra)->ittHandle_name, __itt_metadata_double, 1, &value);
    }
#وإلا
    CV_استفادهنشده(value);
#پایان_اگر
}

#وإلا

Region::Region(ثابت LocationStaticStorage&) : pImpl(عدم), implFlags(0) {}
فراغ Region::destroy() {}

فراغ traceArg(ثابت TraceArg&, ثابت حرف*) {}
فراغ traceArg(ثابت TraceArg&, صحيح) {};
فراغ traceArg(ثابت TraceArg&, int64) {};
فراغ traceArg(ثابت TraceArg&, مزدوج) {};

#پایان_اگر

}}}} // namespace
