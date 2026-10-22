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
#وصف_ڪريو CV_LOG(...) CV_LOG_INFO(عدم, __VA_ARGS__)
#وإلا
#وصف_ڪريو CV_LOG(...) {}
#جيڪڏهن_پورو

#إذا 0
#وصف_ڪريو CV_LOG_ITT(...) CV_LOG_INFO(عدم, __VA_ARGS__)
#وإلا
#وصف_ڪريو CV_LOG_ITT(...) {}
#جيڪڏهن_پورو

#إذا 1
#وصف_ڪريو CV_LOG_TRACE_BAILOUT(ٽيگ, ...) CV_LOG_INFO(ٽيگ, __VA_ARGS__)
#وإلا
#وصف_ڪريو CV_LOG_TRACE_BAILOUT(...) {}
#جيڪڏهن_پورو

#إذا 0
#وصف_ڪريو CV_LOG_PARALLEL(ٽيگ, ...) CV_LOG_INFO(ٽيگ, __VA_ARGS__)
#وإلا
#وصف_ڪريو CV_LOG_PARALLEL(...) {}
#جيڪڏهن_پورو

#إذا 0
#وصف_ڪريو CV_LOG_CTX_STAT(ٽيگ, ...) CV_LOG_INFO(ٽيگ, __VA_ARGS__)
#وإلا
#وصف_ڪريو CV_LOG_CTX_STAT(...) {}
#جيڪڏهن_پورو

#إذا 0
#وصف_ڪريو CV_LOG_SKIP(ٽيگ, ...) CV_LOG_INFO(ٽيگ, __VA_ARGS__)
#وإلا
#وصف_ڪريو CV_LOG_SKIP(...) {}
#جيڪڏهن_پورو

نطاق cv {
نطاق utils {
نطاق trace {
نطاق details {

#جيڪڏهن_وصف_ٿيل OPENCV_TRACE

#جيڪڏهن_وصف_ٿيل _MSC_VER
#هدايت warning(disable:4065) // switch statement contains 'default' but no 'case' labels
#جيڪڏهن_پورو

ساكن منطقي getParameterTraceEnable()
{
    ساكن منطقي param_traceEnable = utils::getConfigurationParameterBool("OPENCV_TRACE", خطأ);
    أرجع param_traceEnable;
}

// TODO lazy configuration flags
ساكن صحيح param_max_Region_اونهائي_Open_CV = (صحيح)utils::get_Configuration_Parameter_سائيز_T("OPENCV_TRACE_DEPTH_OPENCV", 1);
ساكن صحيح param_maxRegionChildrenOpenCV = (صحيح)utils::get_Configuration_Parameter_سائيز_T("OPENCV_TRACE_MAX_CHILDREN_OPENCV", 1000);
ساكن صحيح param_maxRegionChildren = (صحيح)utils::get_Configuration_Parameter_سائيز_T("OPENCV_TRACE_MAX_CHILDREN", 10000);

ساكن ثابت cv::String& getParameterTraceLocation()
{
    ساكن cv::String param_traceLocation = utils::getConfigurationParameterString("OPENCV_TRACE_LOCATION", "OpenCVTrace");
    أرجع param_traceLocation;
}

#جيڪڏهن_وصف_ٿيل HAVE_OPENCL
ساكن منطقي param_synchronizeOpenCL = utils::getConfigurationParameterBool("OPENCV_TRACE_SYNC_OPENCL", خطأ);
#جيڪڏهن_پورو

#جيڪڏهن_وصف_ٿيل OPENCV_WITH_ITT
ساكن منطقي param_ITT_رجسٽر_والد_Scope = utils::getConfigurationParameterBool("OPENCV_TRACE_ITT_PARENT", خطأ);
#جيڪڏهن_پورو

ساكن ثابت حرف* _spaces(صحيح ڳڻپ_2)
{
    ساكن ثابت حرف buf[64] =
"                                                               ";
    أرجع &buf[63 - (ڳڻپ_2 & 63)];
}

/**
 * Text-based trace messages
 */
صنف Trace_پيغام
{
عام:
    حرف بفر[1024];
    size_t len;
    منطقي hasError;

    Trace_پيغام() :
        len(0),
        hasError(خطأ)
    {}

    منطقي لکو(ثابت حرف* format, ...)
    {
        حرف* buf = &بفر[len];
        size_t sz = حجم(بفر) - len;
        va_list ap;
        va_start(ap, format);
        صحيح n = cv_vsnprintf(buf, (صحيح)sz, format, ap);
        va_end(ap);
        إذا (n < 0 || (size_t)n > sz)
        {
            hasError = سچ;
            أرجع خطأ;
        }
        len += n;
        أرجع سچ;
    }

    منطقي formatlocation(ثابت Region::LocationStaticStorage& location)
    {
        أرجع هذا->لکو("l,%lld,\"%s\",%d,\"%s\",0x%llX\n",
                (ڊگهو ڊگهو صحيح)(*location.ppExtra)->global_location_id,
                location.filename,
                location.سٽ,
                location.name,
                (ڊگهو ڊگهو صحيح)(location.flags & ~0xF0000000));
    }
    منطقي formatRegionEnter(ثابت Region& region)
    {
        منطقي ok = هذا->لکو("b,%d,%lld,%lld,%lld",
                (صحيح)region.pImpl->threadID,
                (ڊگهو ڊگهو صحيح)region.pImpl->beginTimestamp,
                (ڊگهو ڊگهو صحيح)((*region.pImpl->location.ppExtra)->global_location_id),
                (ڊگهو ڊگهو صحيح)region.pImpl->global_region_id);
        إذا (region.pImpl->والد_Region && region.pImpl->والد_Region->pImpl)
        {
            إذا (region.pImpl->والد_Region->pImpl->threadID != region.pImpl->threadID)
                ok &= هذا->لکو(",parentThread=%d,parent=%lld",
                        (صحيح)region.pImpl->والد_Region->pImpl->threadID,
                        (ڊگهو ڊگهو صحيح)region.pImpl->والد_Region->pImpl->global_region_id);
        }
        ok &= هذا->لکو("\n");
        أرجع ok;
    }
    منطقي formatRegionLeave(ثابت Region& region, ثابت RegionStatistics& نتيجو)
    {
        CV_DbgAssert(region.pImpl->پڇاڙي_Timestamp - region.pImpl->beginTimestamp == نتيجو.duration);
        منطقي ok = هذا->لکو("e,%d,%lld,%lld,%lld,%lld",
                (صحيح)region.pImpl->threadID,
                (ڊگهو ڊگهو صحيح)region.pImpl->پڇاڙي_Timestamp,
                (ڊگهو ڊگهو صحيح)(*region.pImpl->location.ppExtra)->global_location_id,
                (ڊگهو ڊگهو صحيح)region.pImpl->global_region_id,
                (ڊگهو ڊگهو صحيح)نتيجو.duration);
        إذا (نتيجو.currentSkippedRegions)
            ok &= هذا->لکو(",skip=%d", (صحيح)نتيجو.currentSkippedRegions);
#جيڪڏهن_وصف_ٿيل HAVE_IPP
        إذا (نتيجو.durationImplIPP)
            ok &= هذا->لکو(",tIPP=%lld", (ڊگهو ڊگهو صحيح)نتيجو.durationImplIPP);
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل HAVE_OPENCL
        إذا (نتيجو.durationImplOpenCL)
            ok &= هذا->لکو(",tOCL=%lld", (ڊگهو ڊگهو صحيح)نتيجو.durationImplOpenCL);
#جيڪڏهن_پورو
        ok &= هذا->لکو("\n");
        أرجع ok;
    }
    منطقي رڪارڊ_Region_Arg(ثابت Region& region, ثابت TraceArg& arg, ثابت حرف* value)
    {
        أرجع هذا->لکو("a,%d,%lld,%lld,\"%s\",\"%s\"\n",
                region.pImpl->threadID,
                (ڊگهو ڊگهو صحيح)region.pImpl->beginTimestamp,
                (ڊگهو ڊگهو صحيح)region.pImpl->global_region_id,
                arg.name,
                value);
    }
};


#جيڪڏهن_وصف_ٿيل OPENCV_WITH_ITT
ساكن __itt_domain* domain = عدم;

ساكن منطقي isITTEnabled()
{
    ساكن متطاير منطقي isInitialized = خطأ;
    ساكن منطقي isEnabled = خطأ;
    إذا (!isInitialized)
    {
        cv::Auto_تالو lock(cv::getInitializationMutex());
        إذا (!isInitialized)
        {
            منطقي param_traceITTEnable = utils::getConfigurationParameterBool("OPENCV_TRACE_ITT_ENABLE", سچ);
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
            isInitialized = سچ;
        }
    }
    أرجع isEnabled;
}
#جيڪڏهن_پورو


Region::Location_Extra_معلومات::Location_Extra_معلومات(ثابت LocationStaticStorage& location)
{
    CV_اڻ_استعمال_ٿيل(location);
    ساكن صحيح g_location_id_counter = 0;
    global_location_id = CV_XADD(&g_location_id_counter, 1) + 1;
    CV_LOG("Register location: " << global_location_id << " (" << (فراغ*)&location << ")"
            << std::سٽ_پڄاڻي << "    file: " << location.filename
            << std::سٽ_پڄاڻي << "    line: " << location.سٽ
            << std::سٽ_پڄاڻي << "    name: " << location.name);
#جيڪڏهن_وصف_ٿيل OPENCV_WITH_ITT
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
#جيڪڏهن_پورو
}

/*static*/ Region::Location_Extra_معلومات* Region::Location_Extra_معلومات::init(ثابت Region::LocationStaticStorage& location)
{
    Location_Extra_معلومات** pLocationExtra = location.ppExtra;
    CV_DbgAssert(pLocationExtra);
    إذا (*pLocationExtra == عدم)
    {
        cv::Auto_تالو lock(cv::getInitializationMutex());
        إذا (*pLocationExtra == عدم)
        {
            *pLocationExtra = جديد Region::Location_Extra_معلومات(location);
            TraceStorage* s = getTraceManager().trace_storage.get();
            إذا (s)
            {
                Trace_پيغام نياپو;
                نياپو.formatlocation(location);
                s->put(نياپو);
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
    پڇاڙي_Timestamp(0),
    سڌو_Children_ڳڻپ(0)
#جيڪڏهن_وصف_ٿيل OPENCV_WITH_ITT
    ,itt_id_registered(خطأ)
    ,itt_id(__itt_null)
#جيڪڏهن_پورو
{
    CV_DbgAssert(ctx.current_سرگرم_Region == والد_Region);
    region.pImpl = هذا;

    رجسٽر_Region(ctx);

    enterRegion(ctx);
}

Region::Impl::~Impl()
{
#جيڪڏهن_وصف_ٿيل OPENCV_WITH_ITT
    إذا (itt_id_registered)
    {
        CV_LOG_ITT(" Destroy ITT region: I=" << (فراغ*)هذا);
        __itt_id_destroy(domain, itt_id);
        itt_id_registered = خطأ;
    }
#جيڪڏهن_پورو
    region.pImpl = عدم;
}

فراغ Region::Impl::enterRegion(TraceManagerThreadLocal& ctx)
{
    ctx.current_سرگرم_Region = &region;

    إذا (location.flags & REGION_FLAG_FUNCTION)
    {
        إذا ((location.flags & REGION_FLAG_APP_ڪوڊ) == 0)
        {
            ctx.region_اونهائي_Open_CV++;
        }
        ctx.region_اونهائي++;
    }

    TraceStorage* s = ctx.getStorage();
    إذا (s)
    {
        Trace_پيغام نياپو;
        نياپو.formatRegionEnter(region);
        s->put(نياپو);
    }
#جيڪڏهن_وصف_ٿيل OPENCV_WITH_ITT
    إذا (isITTEnabled())
    {
        __itt_id والد_ID = __itt_null;
        إذا (param_ITT_رجسٽر_والد_Scope && والد_Region && والد_Region->pImpl && والد_Region->pImpl->itt_id_registered && (location.flags & REGION_FLAG_REGION_FORCE) == 0)
            والد_ID = والد_Region->pImpl->itt_id;
        __itt_task_begin(domain, itt_id, والد_ID, (*location.ppExtra)->ittHandle_name);
    }
#جيڪڏهن_پورو
}

فراغ Region::Impl::leaveRegion(TraceManagerThreadLocal& ctx)
{
    int64 duration = پڇاڙي_Timestamp - beginTimestamp; CV_اڻ_استعمال_ٿيل(duration);
    RegionStatistics نتيجو;
    ctx.stat.grab(نتيجو);
    ctx.ڪل_Skipped_Events += نتيجو.currentSkippedRegions;
    CV_LOG(_spaces(ctx.get_Current_اونهائي()*4) << "leaveRegion(): " << (فراغ*)هذا << " " << نتيجو);
#جيڪڏهن_وصف_ٿيل OPENCV_WITH_ITT
    إذا (isITTEnabled())
    {
        إذا (نتيجو.currentSkippedRegions)
        {
            __itt_metadata_add(domain, itt_id, __itt_string_handle_create("skipped trace entries"), __itt_metadata_u32, 1, &نتيجو.currentSkippedRegions);
        }
#جيڪڏهن_وصف_ٿيل HAVE_IPP
        إذا (نتيجو.durationImplIPP)
            __itt_metadata_add(domain, itt_id, __itt_string_handle_create("tIPP"), __itt_metadata_u64, 1, &نتيجو.durationImplIPP);
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل HAVE_OPENCL
        إذا (نتيجو.durationImplOpenCL)
            __itt_metadata_add(domain, itt_id, __itt_string_handle_create("tOpenCL"), __itt_metadata_u64, 1, &نتيجو.durationImplOpenCL);
#جيڪڏهن_پورو
        __itt_task_end(domain);
    }
#جيڪڏهن_پورو
    TraceStorage* s = ctx.getStorage();
    إذا (s)
    {
        Trace_پيغام نياپو;
        نياپو.formatRegionLeave(region, نتيجو);
        s->put(نياپو);
    }

    إذا (location.flags & REGION_FLAG_FUNCTION)
    {
        إذا ((location.flags & REGION_FLAG_APP_ڪوڊ) == 0)
        {
            ctx.region_اونهائي_Open_CV--;
        }
        ctx.region_اونهائي--;
    }

    ctx.current_سرگرم_Region = والد_Region;
}

فراغ Region::Impl::release()
{
    ميٽيو هذا;
}

فراغ Region::Impl::رجسٽر_Region(TraceManagerThreadLocal& ctx)
{
#جيڪڏهن_وصف_ٿيل OPENCV_WITH_ITT
    إذا (isITTEnabled())
    {
        إذا (!itt_id_registered)
        {
            CV_LOG_ITT(" Register ITT region: I=" << (فراغ*)هذا << " " << ctx.threadID << "-" << global_region_id);
#إذا 1 // workaround for some ITT backends
            itt_id = __itt_id_make((فراغ*)(intptr_t)(((int64)(ctx.threadID + 1) << 32) | global_region_id), global_region_id);
#وإلا
            itt_id = __itt_id_make((فراغ*)(intptr_t)(ctx.threadID + 1), global_region_id);
#جيڪڏهن_پورو
            __itt_id_create(domain, itt_id);
            itt_id_registered = سچ;
        }
    }
#وإلا
    CV_اڻ_استعمال_ٿيل(ctx);
#جيڪڏهن_پورو
}

فراغ Region_Statistics_حالت::enable_Skip_گهڻائي(صحيح اونهائي)
{
    CV_DbgAssert(skip_اونهائي < 0);
    CV_LOG_SKIP(عدم, "SKIP-ENABLE: depth=" << اونهائي);
    skip_اونهائي = اونهائي;
}
فراغ Region_Statistics_حالت::check_Reset_Skip_گهڻائي(صحيح leave_اونهائي)
{
    إذا (leave_اونهائي <= skip_اونهائي)
    {
        CV_LOG_SKIP(عدم, "SKIP-RESET: leaveDepth=" << leave_اونهائي << " skipDepth=" << skip_اونهائي);
        skip_اونهائي = -1;
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
    CV_LOG(_spaces(ctx.get_Current_اونهائي()*4) << "Region(): " << (فراغ*)هذا << ": " << location.name);

    Region* والد_Region = ctx.ڍير_چوٽي_Region();
    ثابت Region::LocationStaticStorage* والد_Location = ctx.ڍير_چوٽي_Location();

    إذا (location.flags & REGION_FLAG_REGION_ايندڙ)
    {
        إذا (والد_Region && والد_Region->pImpl)
        {
            CV_DbgAssert((والد_Region->pImpl->location.flags & REGION_FLAG_FUNCTION) == 0);
            والد_Region->destroy(); والد_Region->implFlags = 0;
            والد_Region = ctx.ڍير_چوٽي_Region();
            والد_Location = ctx.ڍير_چوٽي_Location();
        }
    }

    صحيح والد_Children = 0;
    إذا (والد_Region && والد_Region->pImpl)
    {
        إذا (والد_Location == عدم)
        {
            // parallel_for_body code path
            والد_Children = CV_XADD(&والد_Region->pImpl->سڌو_Children_ڳڻپ, 1) + 1;
        }
        وإلا
        {
            والد_Children = ++والد_Region->pImpl->سڌو_Children_ڳڻپ;
        }
    }

    int64 beginTimestamp = getTimestampNS();

    صحيح current_اونهائي = ctx.get_Current_اونهائي() + 1;
    بدل (location.flags & REGION_FLAG_IMPL_نقاب)
    {
#جيڪڏهن_وصف_ٿيل HAVE_IPP
    حالة REGION_FLAG_IMPL_IPP:
        إذا (!ctx.stat_حالت.ignore_اونهائي_Impl_IPP)
            ctx.stat_حالت.ignore_اونهائي_Impl_IPP = current_اونهائي;
        اكسر;
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل HAVE_OPENCL
    حالة REGION_FLAG_IMPL_OPENCL:
        إذا (!ctx.stat_حالت.ignore_اونهائي_Impl_Open_CL)
            ctx.stat_حالت.ignore_اونهائي_Impl_Open_CL = current_اونهائي;
        اكسر;
#جيڪڏهن_پورو
    افتراضي:
        اكسر;
    }

    ctx.ڍير_ڌڪو(هذا, &location, beginTimestamp);
    implFlags |= REGION_FLAG_NEED_ڍير_ڪڍو;

    إذا ((location.flags & REGION_FLAG_REGION_FORCE) == 0)
    {
        إذا (ctx.stat_حالت.skip_اونهائي >= 0 && current_اونهائي > ctx.stat_حالت.skip_اونهائي)
        {
            CV_LOG(_spaces(ctx.get_Current_اونهائي()*4) << "Parent region is disabled. Bailout");
            ctx.stat.currentSkippedRegions++;
            أرجع;
        }

        إذا (param_maxRegionChildrenOpenCV > 0 && (location.flags & REGION_FLAG_APP_ڪوڊ) == 0 && والد_Location && (والد_Location->flags & REGION_FLAG_APP_ڪوڊ) == 0)
        {
            إذا (والد_Children >= param_maxRegionChildrenOpenCV)
            {
                CV_LOG_TRACE_BAILOUT(عدم, _spaces(ctx.get_Current_اونهائي()*4) << "OpenCV parent region exceeds children count. Bailout");
                ctx.stat_حالت.enable_Skip_گهڻائي(current_اونهائي - 1);
                ctx.stat.currentSkippedRegions++;
                DEBUG_ONLY(ctx.dump_ڍير(std::ٻاهر, خطأ));
                أرجع;
            }
        }
        إذا (param_maxRegionChildren > 0 && والد_Children >= param_maxRegionChildren)
        {
            CV_LOG_TRACE_BAILOUT(عدم, _spaces(ctx.get_Current_اونهائي()*4) << "Parent region exceeds children count. Bailout");
            ctx.stat_حالت.enable_Skip_گهڻائي(current_اونهائي - 1);
            ctx.stat.currentSkippedRegions++;
            DEBUG_ONLY(ctx.dump_ڍير(std::ٻاهر, خطأ));
            أرجع;
        }
    }

    Location_Extra_معلومات::init(location);

    إذا ((*location.ppExtra)->global_location_id == 0)
    {
        CV_LOG_TRACE_BAILOUT(عدم, _spaces(ctx.get_Current_اونهائي()*4) << "Region location is disabled. Bailout");
        ctx.stat_حالت.enable_Skip_گهڻائي(current_اونهائي);
        ctx.stat.currentSkippedRegions++;
        أرجع;
    }

    إذا (والد_Location && (والد_Location->flags & REGION_FLAG_SKIP_NESTED))
    {
        CV_LOG(_spaces(ctx.get_Current_اونهائي()*4) << "Parent region disables inner regions. Bailout");
        ctx.stat_حالت.enable_Skip_گهڻائي(current_اونهائي);
        ctx.stat.currentSkippedRegions++;
        أرجع;
    }

    إذا (param_max_Region_اونهائي_Open_CV)
    {
        إذا ((location.flags & REGION_FLAG_APP_ڪوڊ) == 0)
        {
            إذا (ctx.region_اونهائي_Open_CV >= param_max_Region_اونهائي_Open_CV)
            {
                CV_LOG(_spaces(ctx.get_Current_اونهائي()*4) << "OpenCV region depth is exceed = " << ctx.region_اونهائي_Open_CV << ". Bailout");
                إذا (ctx.stat.currentSkippedRegions == 0)
                {
                    DEBUG_ONLY(ctx.dump_ڍير(std::ٻاهر, خطأ));
                }
                ctx.stat_حالت.enable_Skip_گهڻائي(current_اونهائي);
                ctx.stat.currentSkippedRegions++;
                أرجع;
            }
        }
    }

    جديد Impl(ctx, والد_Region, *هذا, location, beginTimestamp);
    CV_DbgAssert(pImpl != عدم);
    implFlags |= REGION_FLAG_سرگرم;

    // parallel_for path
    إذا (والد_Region && والد_Region->pImpl)
    {
        إذا (والد_Location == عدم)
        {
            pImpl->سڌو_Children_ڳڻپ = والد_Children;
        }
    }
}

فراغ Region::destroy()
{
    CV_DbgAssert(implFlags != 0);

    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    CV_LOG(_spaces(ctx.get_Current_اونهائي()*4) << "Region::destruct(): " << (فراغ*)هذا << " pImpl=" << pImpl << " implFlags=" << implFlags << ' ' << (ctx.ڍير_چوٽي_Location() ? ctx.ڍير_چوٽي_Location()->name : "<unknown>"));

    CV_DbgAssert(implFlags & REGION_FLAG_NEED_ڍير_ڪڍو);
    ثابت صحيح current_اونهائي = ctx.get_Current_اونهائي(); CV_اڻ_استعمال_ٿيل(current_اونهائي);

    CV_LOG_CTX_STAT(عدم, _spaces(current_اونهائي*4) << ctx.stat << ' ' << ctx.stat_حالت);

    ثابت Region::LocationStaticStorage* location = ctx.ڍير_چوٽي_Location();
    Impl::Optimization_رستو my_ڪوڊ_رستو = Impl::ڪوڊ_رستو_PLAIN;
    إذا (location)
    {
        بدل (location->flags & REGION_FLAG_IMPL_نقاب)
        {
#جيڪڏهن_وصف_ٿيل HAVE_IPP
        حالة REGION_FLAG_IMPL_IPP:
            my_ڪوڊ_رستو = Impl::ڪوڊ_رستو_IPP;
            اكسر;
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل HAVE_OPENCL
        حالة REGION_FLAG_IMPL_OPENCL:
            إذا (param_synchronizeOpenCL && cv::ocl::isOpenCLActivated())
                cv::ocl::پڄاڻي_2();
            my_ڪوڊ_رستو = Impl::ڪوڊ_رستو_OPENCL;
            اكسر;
#جيڪڏهن_پورو
        افتراضي:
            اكسر;
        }
    }

    int64 پڇاڙي_Timestamp = getTimestampNS();
    int64 duration = پڇاڙي_Timestamp - ctx.ڍير_چوٽي_Begin_Timestamp();

    منطقي سرگرم = is_سرگرم_2();

    إذا (سرگرم)
        ctx.stat.duration = duration;
    وإلا إذا (ctx.ڍير.size() == ctx.parallel_for_ڍير_سائيز + 1)
        ctx.stat.duration += duration;

    بدل (my_ڪوڊ_رستو) {
        حالة Impl::ڪوڊ_رستو_PLAIN:
            // nothing
            اكسر;
#جيڪڏهن_وصف_ٿيل HAVE_IPP
        حالة Impl::ڪوڊ_رستو_IPP:
            إذا (ctx.stat_حالت.ignore_اونهائي_Impl_IPP == current_اونهائي)
            {
                ctx.stat.durationImplIPP += duration;
                ctx.stat_حالت.ignore_اونهائي_Impl_IPP = 0;
            }
            وإلا إذا (سرگرم)
            {
                ctx.stat.durationImplIPP = duration;
            }
            اكسر;
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل HAVE_OPENCL
        حالة Impl::ڪوڊ_رستو_OPENCL:
            إذا (ctx.stat_حالت.ignore_اونهائي_Impl_Open_CL == current_اونهائي)
            {
                ctx.stat.durationImplOpenCL += duration;
                ctx.stat_حالت.ignore_اونهائي_Impl_Open_CL = 0;
            }
            وإلا إذا (سرگرم)
            {
                ctx.stat.durationImplOpenCL = duration;
            }
            اكسر;
#جيڪڏهن_پورو
        افتراضي:
            اكسر;
    }

    إذا (pImpl)
    {
        CV_DbgAssert((implFlags & (REGION_FLAG_سرگرم | REGION_FLAG_NEED_ڍير_ڪڍو)) == (REGION_FLAG_سرگرم | REGION_FLAG_NEED_ڍير_ڪڍو));
        CV_DbgAssert(ctx.ڍير_چوٽي_Region() == هذا);
        pImpl->پڇاڙي_Timestamp = پڇاڙي_Timestamp;
        pImpl->leaveRegion(ctx);
        pImpl->release();
        pImpl = عدم;
        DEBUG_ONLY(implFlags &= ~REGION_FLAG_سرگرم);
    }
    وإلا
    {
        CV_DbgAssert(ctx.stat_حالت.skip_اونهائي <= current_اونهائي);
    }

    إذا (implFlags & REGION_FLAG_NEED_ڍير_ڪڍو)
    {
        CV_DbgAssert(ctx.ڍير_چوٽي_Region() == هذا);
        ctx.ڍير_ڪڍو();
        ctx.stat_حالت.check_Reset_Skip_گهڻائي(current_اونهائي);
        DEBUG_ONLY(implFlags &= ~REGION_FLAG_NEED_ڍير_ڪڍو);
    }
    CV_LOG_CTX_STAT(عدم, _spaces(current_اونهائي*4) << "===> " << ctx.stat << ' ' << ctx.stat_حالت);
}


TraceManagerThreadLocal::~TraceManagerThreadLocal()
{
}

فراغ TraceManagerThreadLocal::dump_ڍير(std::ostream& out, منطقي onlyFunctions) ثابت
{
    std::stringstream ss;
    std::deque<ڍير_داخلا>::const_iterator it = ڍير.begin();
    std::deque<ڍير_داخلا>::const_iterator end = ڍير.end();
    صحيح اونهائي = 0;
    لكل (; it != end; ++it)
    {
        ثابت Region::LocationStaticStorage* location = it->location;
        إذا (location)
        {
            إذا (!onlyFunctions || (location->flags & REGION_FLAG_FUNCTION))
            {
                ss << _spaces(4*اونهائي) << location->name << std::سٽ_پڄاڻي;
                اونهائي++;
            }
        }
        وإلا
        {
            ss << _spaces(4*اونهائي) << "<unknown>" << std::سٽ_پڄاڻي;
            اونهائي++;
        }
    }
    out << ss.str();
}

صنف AsyncTraceStorage CV_FINAL : عام TraceStorage
{
    بدلجندڙ std::ofstream out;
عام:
    ثابت std::string name;

    AsyncTraceStorage(ثابت std::string& filename) :
        out(filename.c_str(), std::ios::trunc),
        name(filename)
    {
        out << "#description: OpenCV trace file" << std::سٽ_پڄاڻي;
        out << "#version: 1.0" << std::سٽ_پڄاڻي;
    }
    ~AsyncTraceStorage()
    {
        out.close();
    }

    منطقي put(ثابت Trace_پيغام& نياپو) ثابت CV_OVERRIDE
    {
        إذا (نياپو.hasError)
            أرجع خطأ;
        out << نياپو.بفر;
        //DEBUG_ONLY(std::flush(out)); // TODO configure flag
        أرجع سچ;
    }
};

صنف SyncTraceStorage CV_FINAL : عام TraceStorage
{
    بدلجندڙ std::ofstream out;
    بدلجندڙ cv::Mutex mutex;
عام:
    ثابت std::string name;

    SyncTraceStorage(ثابت std::string& filename) :
        out(filename.c_str(), std::ios::trunc),
        name(filename)
    {
        out << "#description: OpenCV trace file" << std::سٽ_پڄاڻي;
        out << "#version: 1.0" << std::سٽ_پڄاڻي;
    }
    ~SyncTraceStorage()
    {
        cv::Auto_تالو l(mutex);
        out.close();
    }

    منطقي put(ثابت Trace_پيغام& نياپو) ثابت CV_OVERRIDE
    {
        إذا (نياپو.hasError)
            أرجع خطأ;
        {
            cv::Auto_تالو l(mutex);
            out << نياپو.بفر;
            std::flush(out); // TODO configure flag
        }
        أرجع سچ;
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
            Trace_پيغام نياپو;
            ثابت حرف* pos = strrchr(filepath.c_str(), '/'); // extract filename
#جيڪڏهن_وصف_ٿيل _WIN32
            إذا (!pos)
                pos = strrchr(filepath.c_str(), '\\');
#جيڪڏهن_پورو
            إذا (!pos)
                pos = filepath.c_str();
            وإلا
                pos += 1; // fix to skip extra slash in filename beginning
            نياپو.لکو("#thread file: %s\n", pos);
            global->put(نياپو);
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

    isInitialized = سچ;
    CV_LOG("TraceManager ctor: " << (فراغ*)هذا);

    CV_LOG("TraceManager configure()");
    activated = getParameterTraceEnable();

    إذا (activated)
        trace_storage.reset(جديد SyncTraceStorage(std::string(getParameterTraceLocation()) + ".txt"));

#جيڪڏهن_وصف_ٿيل OPENCV_WITH_ITT
    إذا (isITTEnabled())
    {
        activated = سچ; // force trace pipeline activation (without OpenCV storage)
        __itt_region_begin(domain, __itt_null, __itt_null, __itt_string_handle_create("OpenCVTrace"));
    }
#جيڪڏهن_پورو
}
TraceManager::~TraceManager()
{
    CV_LOG("TraceManager dtor: " << (فراغ*)هذا);

#جيڪڏهن_وصف_ٿيل OPENCV_WITH_ITT
    إذا (isITTEnabled())
    {
        __itt_region_end(domain, __itt_null);
    }
#جيڪڏهن_پورو

    std::vector<TraceManagerThreadLocal*> ڌاڳا_ctx;
    tls.gather(ڌاڳا_ctx);
    size_t ڪل_Events = 0, ڪل_Skipped_Events = 0;
    لكل (size_t i = 0; i < ڌاڳا_ctx.size(); i++)
    {
        TraceManagerThreadLocal* ctx = ڌاڳا_ctx[i];
        إذا (ctx)
        {
            ڪل_Events += ctx->region_counter;
            ڪل_Skipped_Events += ctx->ڪل_Skipped_Events;
        }
    }
    إذا (ڪل_Events || activated)
    {
        CV_LOG_INFO(عدم, "Trace: Total events: " << ڪل_Events);
    }
    إذا (ڪل_Skipped_Events)
    {
        CV_LOG_WARNING(عدم, "Trace: Total skipped events: " << ڪل_Skipped_Events);
    }

    // This is a global static object, so process starts shutdown here
    // Turn off trace
    cv::__termination = سچ; // also set in DllMain() notifications handler for DLL_PROCESS_DETACH
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
        CV_اڻ_استعمال_ٿيل(m); // TODO
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

فراغ parallel_For_سيٽ_پاڙ_Region(ثابت Region& پاڙ_Region, ثابت TraceManagerThreadLocal& پاڙ_ctx)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();

    إذا (ctx.dummy_ڍير_چوٽي.region == &پاڙ_Region) // already attached
        أرجع;

    CV_Assert(ctx.dummy_ڍير_چوٽي.region == عدم);
    ctx.dummy_ڍير_چوٽي = TraceManagerThreadLocal::ڍير_داخلا(const_cast<Region*>(&پاڙ_Region), عدم, -1);

    إذا (&ctx == &پاڙ_ctx)
    {
        ctx.stat.grab(ctx.parallel_for_stat);
        ctx.parallel_for_stat_حالت = ctx.stat_حالت;
        ctx.parallel_for_ڍير_سائيز = ctx.ڍير.size();
        أرجع;
    }

    CV_Assert(ctx.ڍير.empty());

    ctx.current_سرگرم_Region = const_cast<Region*>(&پاڙ_Region);

    ctx.region_اونهائي = پاڙ_ctx.region_اونهائي;
    ctx.region_اونهائي_Open_CV = پاڙ_ctx.region_اونهائي_Open_CV;

    ctx.parallel_for_ڍير_سائيز = 0;

    ctx.stat_حالت.propagateFrom(پاڙ_ctx.stat_حالت);
}

فراغ parallelForAttachNestedRegion(ثابت Region& پاڙ_Region)
{
    CV_اڻ_استعمال_ٿيل(پاڙ_Region);
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();

    CV_DbgAssert(ctx.dummy_ڍير_چوٽي.region == &پاڙ_Region);

    Region* region = ctx.get_Current_سرگرم_Region();
    CV_LOG_PARALLEL(عدم, " PARALLEL_FOR: " << (فراغ*)region << " ==> " << &پاڙ_Region);
    إذا (!region)
        أرجع;

#جيڪڏهن_وصف_ٿيل OPENCV_WITH_ITT
    إذا (!پاڙ_Region.pImpl || !پاڙ_Region.pImpl->itt_id_registered)
        أرجع;

    إذا (!region->pImpl)
        أرجع;

    CV_LOG_PARALLEL(عدم, " PARALLEL_FOR ITT: " << (فراغ*)پاڙ_Region.pImpl->itt_id.d1 << ":" << پاڙ_Region.pImpl->itt_id.d2 << ":" << (فراغ*)پاڙ_Region.pImpl->itt_id.d3 << " => "
                                 << (فراغ*)region->pImpl->itt_id.d1 << ":" << region->pImpl->itt_id.d2 << ":" << (فراغ*)region->pImpl->itt_id.d3);
    __itt_relation_add(domain, region->pImpl->itt_id, __itt_relation_is_child_of, پاڙ_Region.pImpl->itt_id);
#جيڪڏهن_پورو
}

فراغ parallelForFinalize(ثابت Region& پاڙ_Region)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();

    int64 پڇاڙي_Timestamp = getTimestampNS();
    int64 duration = پڇاڙي_Timestamp - ctx.ڍير_چوٽي_Begin_Timestamp();
    CV_LOG_PARALLEL(عدم, "parallel_for duration: " << duration << " " << &پاڙ_Region);

    std::vector<TraceManagerThreadLocal*> ڌاڳا_ctx;
    getTraceManager().tls.gather(ڌاڳا_ctx);
    RegionStatistics parallel_for_stat;
    لكل (size_t i = 0; i < ڌاڳا_ctx.size(); i++)
    {
        TraceManagerThreadLocal* ٻار_ctx = ڌاڳا_ctx[i];

        إذا (ٻار_ctx && ٻار_ctx->ڍير_چوٽي_Region() == &پاڙ_Region)
        {
            CV_LOG_PARALLEL(عدم, "Thread=" << ٻار_ctx->threadID << " " << ٻار_ctx->stat);
            RegionStatistics ٻار_stat;
            ٻار_ctx->stat.grab(ٻار_stat);
            parallel_for_stat.append(ٻار_stat);
            إذا (ٻار_ctx != &ctx)
            {
                ٻار_ctx->dummy_ڍير_چوٽي = TraceManagerThreadLocal::ڍير_داخلا();
            }
            وإلا
            {
                ctx.parallel_for_stat.grab(ctx.stat);
                ctx.stat_حالت = ctx.parallel_for_stat_حالت;
                ٻار_ctx->dummy_ڍير_چوٽي = TraceManagerThreadLocal::ڍير_داخلا();
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

بنية TraceArg::Extra_معلومات
{
#جيڪڏهن_وصف_ٿيل OPENCV_WITH_ITT
    // Special fields for ITT
    __itt_string_handle* متطاير ittHandle_name;
#جيڪڏهن_پورو
    Extra_معلومات(TraceManagerThreadLocal& ctx, ثابت TraceArg& arg)
    {
        CV_اڻ_استعمال_ٿيل(ctx); CV_اڻ_استعمال_ٿيل(arg);
#جيڪڏهن_وصف_ٿيل OPENCV_WITH_ITT
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
#جيڪڏهن_پورو
    }
};

ساكن فراغ initTraceArg(TraceManagerThreadLocal& ctx, ثابت TraceArg& arg)
{
    TraceArg::Extra_معلومات** pExtra = arg.ppExtra;
    إذا (*pExtra == عدم)
    {
        cv::Auto_تالو lock(cv::getInitializationMutex());
        إذا (*pExtra == عدم)
        {
            *pExtra = جديد TraceArg::Extra_معلومات(ctx, arg);
        }
    }
}
فراغ traceArg(ثابت TraceArg& arg, ثابت حرف* value)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    Region* region = ctx.get_Current_سرگرم_Region();
    إذا (!region)
        أرجع;
    CV_Assert(region->pImpl);
    initTraceArg(ctx, arg);
    إذا (!value)
        value = "<null>";
#جيڪڏهن_وصف_ٿيل OPENCV_WITH_ITT
    إذا (isITTEnabled())
    {
        __itt_metadata_str_add(domain, region->pImpl->itt_id, (*arg.ppExtra)->ittHandle_name, value, لڙي_ڊيگهه(value));
    }
#جيڪڏهن_پورو
}
فراغ traceArg(ثابت TraceArg& arg, صحيح value)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    Region* region = ctx.get_Current_سرگرم_Region();
    إذا (!region)
        أرجع;
    CV_Assert(region->pImpl);
    initTraceArg(ctx, arg);
#جيڪڏهن_وصف_ٿيل OPENCV_WITH_ITT
    إذا (isITTEnabled())
    {
        __itt_metadata_add(domain, region->pImpl->itt_id, (*arg.ppExtra)->ittHandle_name, حجم(صحيح) == 4 ? __itt_metadata_s32 : __itt_metadata_s64, 1, &value);
    }
#وإلا
    CV_اڻ_استعمال_ٿيل(value);
#جيڪڏهن_پورو
}
فراغ traceArg(ثابت TraceArg& arg, int64 value)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    Region* region = ctx.get_Current_سرگرم_Region();
    إذا (!region)
        أرجع;
    CV_Assert(region->pImpl);
    initTraceArg(ctx, arg);
#جيڪڏهن_وصف_ٿيل OPENCV_WITH_ITT
    إذا (isITTEnabled())
    {
        __itt_metadata_add(domain, region->pImpl->itt_id, (*arg.ppExtra)->ittHandle_name, __itt_metadata_s64, 1, &value);
    }
#وإلا
    CV_اڻ_استعمال_ٿيل(value);
#جيڪڏهن_پورو
}
فراغ traceArg(ثابت TraceArg& arg, مزدوج value)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    Region* region = ctx.get_Current_سرگرم_Region();
    إذا (!region)
        أرجع;
    CV_Assert(region->pImpl);
    initTraceArg(ctx, arg);
#جيڪڏهن_وصف_ٿيل OPENCV_WITH_ITT
    إذا (isITTEnabled())
    {
        __itt_metadata_add(domain, region->pImpl->itt_id, (*arg.ppExtra)->ittHandle_name, __itt_metadata_double, 1, &value);
    }
#وإلا
    CV_اڻ_استعمال_ٿيل(value);
#جيڪڏهن_پورو
}

#وإلا

Region::Region(ثابت LocationStaticStorage&) : pImpl(عدم), implFlags(0) {}
فراغ Region::destroy() {}

فراغ traceArg(ثابت TraceArg&, ثابت حرف*) {}
فراغ traceArg(ثابت TraceArg&, صحيح) {};
فراغ traceArg(ثابت TraceArg&, int64) {};
فراغ traceArg(ثابت TraceArg&, مزدوج) {};

#جيڪڏهن_پورو

}}}} // namespace
