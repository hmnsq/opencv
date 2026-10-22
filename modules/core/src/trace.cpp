// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#అన్తర్భావ "precomp.hpp"

#అన్తర్భావ <opencv2/core/utils/trace.hpp>
#అన్తర్భావ <opencv2/core/utils/trace.private.hpp>
#అన్తర్భావ <opencv2/core/utils/configuration.private.hpp>

#అన్తర్భావ <opencv2/core/opencl/ocl_defs.hpp>

#అన్తర్భావ <cstdarg> // va_start

#అన్తర్భావ <sstream>
#అన్తర్భావ <ostream>
#అన్తర్భావ <fstream>

#ఒకవేళ 0
#నిర్వచించు CV_LOG(...) CV_LOG_INFO(ఖాళీ, __VA_ARGS__)
#లేకపోతే
#నిర్వచించు CV_LOG(...) {}
#అయితే_ముగింపు

#ఒకవేళ 0
#నిర్వచించు CV_LOG_ITT(...) CV_LOG_INFO(ఖాళీ, __VA_ARGS__)
#లేకపోతే
#నిర్వచించు CV_LOG_ITT(...) {}
#అయితే_ముగింపు

#ఒకవేళ 1
#నిర్వచించు CV_LOG_TRACE_BAILOUT(ట్యాగ్, ...) CV_LOG_INFO(ట్యాగ్, __VA_ARGS__)
#లేకపోతే
#నిర్వచించు CV_LOG_TRACE_BAILOUT(...) {}
#అయితే_ముగింపు

#ఒకవేళ 0
#నిర్వచించు CV_LOG_PARALLEL(ట్యాగ్, ...) CV_LOG_INFO(ట్యాగ్, __VA_ARGS__)
#లేకపోతే
#నిర్వచించు CV_LOG_PARALLEL(...) {}
#అయితే_ముగింపు

#ఒకవేళ 0
#నిర్వచించు CV_LOG_CTX_STAT(ట్యాగ్, ...) CV_LOG_INFO(ట్యాగ్, __VA_ARGS__)
#లేకపోతే
#నిర్వచించు CV_LOG_CTX_STAT(...) {}
#అయితే_ముగింపు

#ఒకవేళ 0
#నిర్వచించు CV_LOG_SKIP(ట్యాగ్, ...) CV_LOG_INFO(ట్యాగ్, __VA_ARGS__)
#లేకపోతే
#నిర్వచించు CV_LOG_SKIP(...) {}
#అయితే_ముగింపు

నామాకాశ cv {
నామాకాశ utils {
నామాకాశ trace {
నామాకాశ details {

#నిర్వచితమైతే OPENCV_TRACE

#నిర్వచితమైతే _MSC_VER
#సూచన warning(disable:4065) // switch statement contains 'default' but no 'case' labels
#అయితే_ముగింపు

స్థిర తార్కిక getParameterTraceEnable()
{
    స్థిర తార్కిక param_traceEnable = utils::getConfigurationParameterBool("OPENCV_TRACE", అబద్ధం);
    తిరిగి param_traceEnable;
}

// TODO lazy configuration flags
స్థిర పూర్ణాంకం param_max_Region_లోతు_Open_CV = (పూర్ణాంకం)utils::get_Configuration_Parameter_కొలత_T("OPENCV_TRACE_DEPTH_OPENCV", 1);
స్థిర పూర్ణాంకం param_maxRegionChildrenOpenCV = (పూర్ణాంకం)utils::get_Configuration_Parameter_కొలత_T("OPENCV_TRACE_MAX_CHILDREN_OPENCV", 1000);
స్థిర పూర్ణాంకం param_maxRegionChildren = (పూర్ణాంకం)utils::get_Configuration_Parameter_కొలత_T("OPENCV_TRACE_MAX_CHILDREN", 10000);

స్థిర స్థిరరాశి cv::String& getParameterTraceLocation()
{
    స్థిర cv::String param_traceLocation = utils::getConfigurationParameterString("OPENCV_TRACE_LOCATION", "OpenCVTrace");
    తిరిగి param_traceLocation;
}

#నిర్వచితమైతే HAVE_OPENCL
స్థిర తార్కిక param_synchronizeOpenCL = utils::getConfigurationParameterBool("OPENCV_TRACE_SYNC_OPENCL", అబద్ధం);
#అయితే_ముగింపు

#నిర్వచితమైతే OPENCV_WITH_ITT
స్థిర తార్కిక param_ITT_రిజిస్టరు_మాతృ_Scope = utils::getConfigurationParameterBool("OPENCV_TRACE_ITT_PARENT", అబద్ధం);
#అయితే_ముగింపు

స్థిర స్థిరరాశి అక్షరం* _spaces(పూర్ణాంకం లెక్క_2)
{
    స్థిర స్థిరరాశి అక్షరం buf[64] =
"                                                               ";
    తిరిగి &buf[63 - (లెక్క_2 & 63)];
}

/**
 * Text-based trace messages
 */
తరగతి Trace_సందేశం
{
బహిరంగ:
    అక్షరం బఫరు[1024];
    size_t len;
    తార్కిక hasError;

    Trace_సందేశం() :
        len(0),
        hasError(అబద్ధం)
    {}

    తార్కిక రాయి(స్థిరరాశి అక్షరం* format, ...)
    {
        అక్షరం* buf = &బఫరు[len];
        size_t sz = పరిమాణం(బఫరు) - len;
        va_list ap;
        va_start(ap, format);
        పూర్ణాంకం n = cv_vsnprintf(buf, (పూర్ణాంకం)sz, format, ap);
        va_end(ap);
        ఒకవేళ (n < 0 || (size_t)n > sz)
        {
            hasError = నిజం;
            తిరిగి అబద్ధం;
        }
        len += n;
        తిరిగి నిజం;
    }

    తార్కిక formatlocation(స్థిరరాశి Region::LocationStaticStorage& location)
    {
        తిరిగి ఇది->రాయి("l,%lld,\"%s\",%d,\"%s\",0x%llX\n",
                (దీర్ఘ దీర్ఘ పూర్ణాంకం)(*location.ppExtra)->global_location_id,
                location.filename,
                location.పంక్తి,
                location.name,
                (దీర్ఘ దీర్ఘ పూర్ణాంకం)(location.flags & ~0xF0000000));
    }
    తార్కిక formatRegionEnter(స్థిరరాశి Region& region)
    {
        తార్కిక ok = ఇది->రాయి("b,%d,%lld,%lld,%lld",
                (పూర్ణాంకం)region.pImpl->threadID,
                (దీర్ఘ దీర్ఘ పూర్ణాంకం)region.pImpl->beginTimestamp,
                (దీర్ఘ దీర్ఘ పూర్ణాంకం)((*region.pImpl->location.ppExtra)->global_location_id),
                (దీర్ఘ దీర్ఘ పూర్ణాంకం)region.pImpl->global_region_id);
        ఒకవేళ (region.pImpl->మాతృ_Region && region.pImpl->మాతృ_Region->pImpl)
        {
            ఒకవేళ (region.pImpl->మాతృ_Region->pImpl->threadID != region.pImpl->threadID)
                ok &= ఇది->రాయి(",parentThread=%d,parent=%lld",
                        (పూర్ణాంకం)region.pImpl->మాతృ_Region->pImpl->threadID,
                        (దీర్ఘ దీర్ఘ పూర్ణాంకం)region.pImpl->మాతృ_Region->pImpl->global_region_id);
        }
        ok &= ఇది->రాయి("\n");
        తిరిగి ok;
    }
    తార్కిక formatRegionLeave(స్థిరరాశి Region& region, స్థిరరాశి RegionStatistics& ఫలితం)
    {
        CV_DbgAssert(region.pImpl->చివర_Timestamp - region.pImpl->beginTimestamp == ఫలితం.duration);
        తార్కిక ok = ఇది->రాయి("e,%d,%lld,%lld,%lld,%lld",
                (పూర్ణాంకం)region.pImpl->threadID,
                (దీర్ఘ దీర్ఘ పూర్ణాంకం)region.pImpl->చివర_Timestamp,
                (దీర్ఘ దీర్ఘ పూర్ణాంకం)(*region.pImpl->location.ppExtra)->global_location_id,
                (దీర్ఘ దీర్ఘ పూర్ణాంకం)region.pImpl->global_region_id,
                (దీర్ఘ దీర్ఘ పూర్ణాంకం)ఫలితం.duration);
        ఒకవేళ (ఫలితం.currentSkippedRegions)
            ok &= ఇది->రాయి(",skip=%d", (పూర్ణాంకం)ఫలితం.currentSkippedRegions);
#నిర్వచితమైతే HAVE_IPP
        ఒకవేళ (ఫలితం.durationImplIPP)
            ok &= ఇది->రాయి(",tIPP=%lld", (దీర్ఘ దీర్ఘ పూర్ణాంకం)ఫలితం.durationImplIPP);
#అయితే_ముగింపు
#నిర్వచితమైతే HAVE_OPENCL
        ఒకవేళ (ఫలితం.durationImplOpenCL)
            ok &= ఇది->రాయి(",tOCL=%lld", (దీర్ఘ దీర్ఘ పూర్ణాంకం)ఫలితం.durationImplOpenCL);
#అయితే_ముగింపు
        ok &= ఇది->రాయి("\n");
        తిరిగి ok;
    }
    తార్కిక రికార్డు_Region_Arg(స్థిరరాశి Region& region, స్థిరరాశి TraceArg& arg, స్థిరరాశి అక్షరం* value)
    {
        తిరిగి ఇది->రాయి("a,%d,%lld,%lld,\"%s\",\"%s\"\n",
                region.pImpl->threadID,
                (దీర్ఘ దీర్ఘ పూర్ణాంకం)region.pImpl->beginTimestamp,
                (దీర్ఘ దీర్ఘ పూర్ణాంకం)region.pImpl->global_region_id,
                arg.name,
                value);
    }
};


#నిర్వచితమైతే OPENCV_WITH_ITT
స్థిర __itt_domain* domain = ఖాళీ;

స్థిర తార్కిక isITTEnabled()
{
    స్థిర అస్థిర తార్కిక isInitialized = అబద్ధం;
    స్థిర తార్కిక isEnabled = అబద్ధం;
    ఒకవేళ (!isInitialized)
    {
        cv::Auto_తాళం lock(cv::getInitializationMutex());
        ఒకవేళ (!isInitialized)
        {
            తార్కిక param_traceITTEnable = utils::getConfigurationParameterBool("OPENCV_TRACE_ITT_ENABLE", నిజం);
            ఒకవేళ (param_traceITTEnable)
            {
                isEnabled = !!(__itt_api_version());
                CV_LOG_ITT("ITT is " << (isEnabled ? "enabled" : "disabled"));
                domain = __itt_domain_create("OpenCVTrace");
            }
            లేకపోతే
            {
                CV_LOG_ITT("ITT is disabled through OpenCV parameter");
                isEnabled = అబద్ధం;
            }
            isInitialized = నిజం;
        }
    }
    తిరిగి isEnabled;
}
#అయితే_ముగింపు


Region::Location_Extra_దత్తాంశం::Location_Extra_దత్తాంశం(స్థిరరాశి LocationStaticStorage& location)
{
    CV_వాడని(location);
    స్థిర పూర్ణాంకం g_location_id_counter = 0;
    global_location_id = CV_XADD(&g_location_id_counter, 1) + 1;
    CV_LOG("Register location: " << global_location_id << " (" << (శూన్య*)&location << ")"
            << std::పంక్తి_ముగింపు << "    file: " << location.filename
            << std::పంక్తి_ముగింపు << "    line: " << location.పంక్తి
            << std::పంక్తి_ముగింపు << "    name: " << location.name);
#నిర్వచితమైతే OPENCV_WITH_ITT
    ఒకవేళ (isITTEnabled())
    {
        // Caching is not required here, because there is builtin cache.
        // https://software.intel.com/en-us/node/544203:
        //     Consecutive calls to __itt_string_handle_create with the same name return the same value.
        ittHandle_name = __itt_string_handle_create(location.name);
        ittHandle_filename = __itt_string_handle_create(location.filename);
    }
    లేకపోతే
    {
        ittHandle_name = 0;
        ittHandle_filename = 0;
    }
#అయితే_ముగింపు
}

/*static*/ Region::Location_Extra_దత్తాంశం* Region::Location_Extra_దత్తాంశం::init(స్థిరరాశి Region::LocationStaticStorage& location)
{
    Location_Extra_దత్తాంశం** pLocationExtra = location.ppExtra;
    CV_DbgAssert(pLocationExtra);
    ఒకవేళ (*pLocationExtra == ఖాళీ)
    {
        cv::Auto_తాళం lock(cv::getInitializationMutex());
        ఒకవేళ (*pLocationExtra == ఖాళీ)
        {
            *pLocationExtra = కొత్త Region::Location_Extra_దత్తాంశం(location);
            TraceStorage* s = getTraceManager().trace_storage.get();
            ఒకవేళ (s)
            {
                Trace_సందేశం సందేశం;
                సందేశం.formatlocation(location);
                s->put(సందేశం);
            }
        }
    }
    తిరిగి *pLocationExtra;
}


Region::Impl::Impl(TraceManagerThreadLocal& ctx, Region* మాతృ_Region_2, Region& region_, స్థిరరాశి LocationStaticStorage& location_, int64 beginTimestamp_) :
    location(location_),
    region(region_),
    మాతృ_Region(మాతృ_Region_2),
    threadID(ctx.threadID),
    global_region_id(++ctx.region_counter),
    beginTimestamp(beginTimestamp_),
    చివర_Timestamp(0),
    ప్రత్యక్ష_Children_లెక్క(0)
#నిర్వచితమైతే OPENCV_WITH_ITT
    ,itt_id_registered(అబద్ధం)
    ,itt_id(__itt_null)
#అయితే_ముగింపు
{
    CV_DbgAssert(ctx.current_చురుకైన_Region == మాతృ_Region);
    region.pImpl = ఇది;

    రిజిస్టరు_Region(ctx);

    enterRegion(ctx);
}

Region::Impl::~Impl()
{
#నిర్వచితమైతే OPENCV_WITH_ITT
    ఒకవేళ (itt_id_registered)
    {
        CV_LOG_ITT(" Destroy ITT region: I=" << (శూన్య*)ఇది);
        __itt_id_destroy(domain, itt_id);
        itt_id_registered = అబద్ధం;
    }
#అయితే_ముగింపు
    region.pImpl = ఖాళీ;
}

శూన్య Region::Impl::enterRegion(TraceManagerThreadLocal& ctx)
{
    ctx.current_చురుకైన_Region = &region;

    ఒకవేళ (location.flags & REGION_FLAG_FUNCTION)
    {
        ఒకవేళ ((location.flags & REGION_FLAG_APP_సంకేతం) == 0)
        {
            ctx.region_లోతు_Open_CV++;
        }
        ctx.region_లోతు++;
    }

    TraceStorage* s = ctx.getStorage();
    ఒకవేళ (s)
    {
        Trace_సందేశం సందేశం;
        సందేశం.formatRegionEnter(region);
        s->put(సందేశం);
    }
#నిర్వచితమైతే OPENCV_WITH_ITT
    ఒకవేళ (isITTEnabled())
    {
        __itt_id మాతృ_ID = __itt_null;
        ఒకవేళ (param_ITT_రిజిస్టరు_మాతృ_Scope && మాతృ_Region && మాతృ_Region->pImpl && మాతృ_Region->pImpl->itt_id_registered && (location.flags & REGION_FLAG_REGION_FORCE) == 0)
            మాతృ_ID = మాతృ_Region->pImpl->itt_id;
        __itt_task_begin(domain, itt_id, మాతృ_ID, (*location.ppExtra)->ittHandle_name);
    }
#అయితే_ముగింపు
}

శూన్య Region::Impl::leaveRegion(TraceManagerThreadLocal& ctx)
{
    int64 duration = చివర_Timestamp - beginTimestamp; CV_వాడని(duration);
    RegionStatistics ఫలితం;
    ctx.stat.grab(ఫలితం);
    ctx.మొత్తం_Skipped_Events += ఫలితం.currentSkippedRegions;
    CV_LOG(_spaces(ctx.get_Current_లోతు()*4) << "leaveRegion(): " << (శూన్య*)ఇది << " " << ఫలితం);
#నిర్వచితమైతే OPENCV_WITH_ITT
    ఒకవేళ (isITTEnabled())
    {
        ఒకవేళ (ఫలితం.currentSkippedRegions)
        {
            __itt_metadata_add(domain, itt_id, __itt_string_handle_create("skipped trace entries"), __itt_metadata_u32, 1, &ఫలితం.currentSkippedRegions);
        }
#నిర్వచితమైతే HAVE_IPP
        ఒకవేళ (ఫలితం.durationImplIPP)
            __itt_metadata_add(domain, itt_id, __itt_string_handle_create("tIPP"), __itt_metadata_u64, 1, &ఫలితం.durationImplIPP);
#అయితే_ముగింపు
#నిర్వచితమైతే HAVE_OPENCL
        ఒకవేళ (ఫలితం.durationImplOpenCL)
            __itt_metadata_add(domain, itt_id, __itt_string_handle_create("tOpenCL"), __itt_metadata_u64, 1, &ఫలితం.durationImplOpenCL);
#అయితే_ముగింపు
        __itt_task_end(domain);
    }
#అయితే_ముగింపు
    TraceStorage* s = ctx.getStorage();
    ఒకవేళ (s)
    {
        Trace_సందేశం సందేశం;
        సందేశం.formatRegionLeave(region, ఫలితం);
        s->put(సందేశం);
    }

    ఒకవేళ (location.flags & REGION_FLAG_FUNCTION)
    {
        ఒకవేళ ((location.flags & REGION_FLAG_APP_సంకేతం) == 0)
        {
            ctx.region_లోతు_Open_CV--;
        }
        ctx.region_లోతు--;
    }

    ctx.current_చురుకైన_Region = మాతృ_Region;
}

శూన్య Region::Impl::release()
{
    తొలగించు ఇది;
}

శూన్య Region::Impl::రిజిస్టరు_Region(TraceManagerThreadLocal& ctx)
{
#నిర్వచితమైతే OPENCV_WITH_ITT
    ఒకవేళ (isITTEnabled())
    {
        ఒకవేళ (!itt_id_registered)
        {
            CV_LOG_ITT(" Register ITT region: I=" << (శూన్య*)ఇది << " " << ctx.threadID << "-" << global_region_id);
#ఒకవేళ 1 // workaround for some ITT backends
            itt_id = __itt_id_make((శూన్య*)(intptr_t)(((int64)(ctx.threadID + 1) << 32) | global_region_id), global_region_id);
#లేకపోతే
            itt_id = __itt_id_make((శూన్య*)(intptr_t)(ctx.threadID + 1), global_region_id);
#అయితే_ముగింపు
            __itt_id_create(domain, itt_id);
            itt_id_registered = నిజం;
        }
    }
#లేకపోతే
    CV_వాడని(ctx);
#అయితే_ముగింపు
}

శూన్య Region_Statistics_స్థితి::enable_Skip_బహుళకం(పూర్ణాంకం లోతు)
{
    CV_DbgAssert(skip_లోతు < 0);
    CV_LOG_SKIP(ఖాళీ, "SKIP-ENABLE: depth=" << లోతు);
    skip_లోతు = లోతు;
}
శూన్య Region_Statistics_స్థితి::check_Reset_Skip_బహుళకం(పూర్ణాంకం leave_లోతు)
{
    ఒకవేళ (leave_లోతు <= skip_లోతు)
    {
        CV_LOG_SKIP(ఖాళీ, "SKIP-RESET: leaveDepth=" << leave_లోతు << " skipDepth=" << skip_లోతు);
        skip_లోతు = -1;
    }
}

Region::Region(స్థిరరాశి LocationStaticStorage& location) :
    pImpl(ఖాళీ),
    implFlags(0)
{
    // Checks:
    // - global enable flag
    // - parent region is disabled
    // - children count threshold
    // - region location
    // - depth (opencv nested calls)
    ఒకవేళ (!TraceManager::isActivated())
    {
        CV_LOG("Trace is disabled. Bailout");
        తిరిగి;
    }

    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    CV_LOG(_spaces(ctx.get_Current_లోతు()*4) << "Region(): " << (శూన్య*)ఇది << ": " << location.name);

    Region* మాతృ_Region = ctx.దొంతర_శిఖరం_Region();
    స్థిరరాశి Region::LocationStaticStorage* మాతృ_Location = ctx.దొంతర_శిఖరం_Location();

    ఒకవేళ (location.flags & REGION_FLAG_REGION_తదుపరి)
    {
        ఒకవేళ (మాతృ_Region && మాతృ_Region->pImpl)
        {
            CV_DbgAssert((మాతృ_Region->pImpl->location.flags & REGION_FLAG_FUNCTION) == 0);
            మాతృ_Region->destroy(); మాతృ_Region->implFlags = 0;
            మాతృ_Region = ctx.దొంతర_శిఖరం_Region();
            మాతృ_Location = ctx.దొంతర_శిఖరం_Location();
        }
    }

    పూర్ణాంకం మాతృ_Children = 0;
    ఒకవేళ (మాతృ_Region && మాతృ_Region->pImpl)
    {
        ఒకవేళ (మాతృ_Location == ఖాళీ)
        {
            // parallel_for_body code path
            మాతృ_Children = CV_XADD(&మాతృ_Region->pImpl->ప్రత్యక్ష_Children_లెక్క, 1) + 1;
        }
        లేకపోతే
        {
            మాతృ_Children = ++మాతృ_Region->pImpl->ప్రత్యక్ష_Children_లెక్క;
        }
    }

    int64 beginTimestamp = getTimestampNS();

    పూర్ణాంకం current_లోతు = ctx.get_Current_లోతు() + 1;
    ఎంపిక (location.flags & REGION_FLAG_IMPL_ముసుగు)
    {
#నిర్వచితమైతే HAVE_IPP
    స్థితి REGION_FLAG_IMPL_IPP:
        ఒకవేళ (!ctx.stat_స్థితి.ignore_లోతు_Impl_IPP)
            ctx.stat_స్థితి.ignore_లోతు_Impl_IPP = current_లోతు;
        ఆపు;
#అయితే_ముగింపు
#నిర్వచితమైతే HAVE_OPENCL
    స్థితి REGION_FLAG_IMPL_OPENCL:
        ఒకవేళ (!ctx.stat_స్థితి.ignore_లోతు_Impl_Open_CL)
            ctx.stat_స్థితి.ignore_లోతు_Impl_Open_CL = current_లోతు;
        ఆపు;
#అయితే_ముగింపు
    అప్రమేయ:
        ఆపు;
    }

    ctx.దొంతర_నెట్టు(ఇది, &location, beginTimestamp);
    implFlags |= REGION_FLAG_NEED_దొంతర_తీయి;

    ఒకవేళ ((location.flags & REGION_FLAG_REGION_FORCE) == 0)
    {
        ఒకవేళ (ctx.stat_స్థితి.skip_లోతు >= 0 && current_లోతు > ctx.stat_స్థితి.skip_లోతు)
        {
            CV_LOG(_spaces(ctx.get_Current_లోతు()*4) << "Parent region is disabled. Bailout");
            ctx.stat.currentSkippedRegions++;
            తిరిగి;
        }

        ఒకవేళ (param_maxRegionChildrenOpenCV > 0 && (location.flags & REGION_FLAG_APP_సంకేతం) == 0 && మాతృ_Location && (మాతృ_Location->flags & REGION_FLAG_APP_సంకేతం) == 0)
        {
            ఒకవేళ (మాతృ_Children >= param_maxRegionChildrenOpenCV)
            {
                CV_LOG_TRACE_BAILOUT(ఖాళీ, _spaces(ctx.get_Current_లోతు()*4) << "OpenCV parent region exceeds children count. Bailout");
                ctx.stat_స్థితి.enable_Skip_బహుళకం(current_లోతు - 1);
                ctx.stat.currentSkippedRegions++;
                DEBUG_ONLY(ctx.dump_దొంతర(std::బయటకు, అబద్ధం));
                తిరిగి;
            }
        }
        ఒకవేళ (param_maxRegionChildren > 0 && మాతృ_Children >= param_maxRegionChildren)
        {
            CV_LOG_TRACE_BAILOUT(ఖాళీ, _spaces(ctx.get_Current_లోతు()*4) << "Parent region exceeds children count. Bailout");
            ctx.stat_స్థితి.enable_Skip_బహుళకం(current_లోతు - 1);
            ctx.stat.currentSkippedRegions++;
            DEBUG_ONLY(ctx.dump_దొంతర(std::బయటకు, అబద్ధం));
            తిరిగి;
        }
    }

    Location_Extra_దత్తాంశం::init(location);

    ఒకవేళ ((*location.ppExtra)->global_location_id == 0)
    {
        CV_LOG_TRACE_BAILOUT(ఖాళీ, _spaces(ctx.get_Current_లోతు()*4) << "Region location is disabled. Bailout");
        ctx.stat_స్థితి.enable_Skip_బహుళకం(current_లోతు);
        ctx.stat.currentSkippedRegions++;
        తిరిగి;
    }

    ఒకవేళ (మాతృ_Location && (మాతృ_Location->flags & REGION_FLAG_SKIP_NESTED))
    {
        CV_LOG(_spaces(ctx.get_Current_లోతు()*4) << "Parent region disables inner regions. Bailout");
        ctx.stat_స్థితి.enable_Skip_బహుళకం(current_లోతు);
        ctx.stat.currentSkippedRegions++;
        తిరిగి;
    }

    ఒకవేళ (param_max_Region_లోతు_Open_CV)
    {
        ఒకవేళ ((location.flags & REGION_FLAG_APP_సంకేతం) == 0)
        {
            ఒకవేళ (ctx.region_లోతు_Open_CV >= param_max_Region_లోతు_Open_CV)
            {
                CV_LOG(_spaces(ctx.get_Current_లోతు()*4) << "OpenCV region depth is exceed = " << ctx.region_లోతు_Open_CV << ". Bailout");
                ఒకవేళ (ctx.stat.currentSkippedRegions == 0)
                {
                    DEBUG_ONLY(ctx.dump_దొంతర(std::బయటకు, అబద్ధం));
                }
                ctx.stat_స్థితి.enable_Skip_బహుళకం(current_లోతు);
                ctx.stat.currentSkippedRegions++;
                తిరిగి;
            }
        }
    }

    కొత్త Impl(ctx, మాతృ_Region, *ఇది, location, beginTimestamp);
    CV_DbgAssert(pImpl != ఖాళీ);
    implFlags |= REGION_FLAG_చురుకైన;

    // parallel_for path
    ఒకవేళ (మాతృ_Region && మాతృ_Region->pImpl)
    {
        ఒకవేళ (మాతృ_Location == ఖాళీ)
        {
            pImpl->ప్రత్యక్ష_Children_లెక్క = మాతృ_Children;
        }
    }
}

శూన్య Region::destroy()
{
    CV_DbgAssert(implFlags != 0);

    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    CV_LOG(_spaces(ctx.get_Current_లోతు()*4) << "Region::destruct(): " << (శూన్య*)ఇది << " pImpl=" << pImpl << " implFlags=" << implFlags << ' ' << (ctx.దొంతర_శిఖరం_Location() ? ctx.దొంతర_శిఖరం_Location()->name : "<unknown>"));

    CV_DbgAssert(implFlags & REGION_FLAG_NEED_దొంతర_తీయి);
    స్థిరరాశి పూర్ణాంకం current_లోతు = ctx.get_Current_లోతు(); CV_వాడని(current_లోతు);

    CV_LOG_CTX_STAT(ఖాళీ, _spaces(current_లోతు*4) << ctx.stat << ' ' << ctx.stat_స్థితి);

    స్థిరరాశి Region::LocationStaticStorage* location = ctx.దొంతర_శిఖరం_Location();
    Impl::Optimization_మార్గం my_సంకేతం_మార్గం = Impl::సంకేతం_మార్గం_PLAIN;
    ఒకవేళ (location)
    {
        ఎంపిక (location->flags & REGION_FLAG_IMPL_ముసుగు)
        {
#నిర్వచితమైతే HAVE_IPP
        స్థితి REGION_FLAG_IMPL_IPP:
            my_సంకేతం_మార్గం = Impl::సంకేతం_మార్గం_IPP;
            ఆపు;
#అయితే_ముగింపు
#నిర్వచితమైతే HAVE_OPENCL
        స్థితి REGION_FLAG_IMPL_OPENCL:
            ఒకవేళ (param_synchronizeOpenCL && cv::ocl::isOpenCLActivated())
                cv::ocl::ముగింపు_2();
            my_సంకేతం_మార్గం = Impl::సంకేతం_మార్గం_OPENCL;
            ఆపు;
#అయితే_ముగింపు
        అప్రమేయ:
            ఆపు;
        }
    }

    int64 చివర_Timestamp = getTimestampNS();
    int64 duration = చివర_Timestamp - ctx.దొంతర_శిఖరం_Begin_Timestamp();

    తార్కిక చురుకైన = is_చురుకైన_2();

    ఒకవేళ (చురుకైన)
        ctx.stat.duration = duration;
    లేకపోతే ఒకవేళ (ctx.దొంతర.size() == ctx.parallel_for_దొంతర_కొలత + 1)
        ctx.stat.duration += duration;

    ఎంపిక (my_సంకేతం_మార్గం) {
        స్థితి Impl::సంకేతం_మార్గం_PLAIN:
            // nothing
            ఆపు;
#నిర్వచితమైతే HAVE_IPP
        స్థితి Impl::సంకేతం_మార్గం_IPP:
            ఒకవేళ (ctx.stat_స్థితి.ignore_లోతు_Impl_IPP == current_లోతు)
            {
                ctx.stat.durationImplIPP += duration;
                ctx.stat_స్థితి.ignore_లోతు_Impl_IPP = 0;
            }
            లేకపోతే ఒకవేళ (చురుకైన)
            {
                ctx.stat.durationImplIPP = duration;
            }
            ఆపు;
#అయితే_ముగింపు
#నిర్వచితమైతే HAVE_OPENCL
        స్థితి Impl::సంకేతం_మార్గం_OPENCL:
            ఒకవేళ (ctx.stat_స్థితి.ignore_లోతు_Impl_Open_CL == current_లోతు)
            {
                ctx.stat.durationImplOpenCL += duration;
                ctx.stat_స్థితి.ignore_లోతు_Impl_Open_CL = 0;
            }
            లేకపోతే ఒకవేళ (చురుకైన)
            {
                ctx.stat.durationImplOpenCL = duration;
            }
            ఆపు;
#అయితే_ముగింపు
        అప్రమేయ:
            ఆపు;
    }

    ఒకవేళ (pImpl)
    {
        CV_DbgAssert((implFlags & (REGION_FLAG_చురుకైన | REGION_FLAG_NEED_దొంతర_తీయి)) == (REGION_FLAG_చురుకైన | REGION_FLAG_NEED_దొంతర_తీయి));
        CV_DbgAssert(ctx.దొంతర_శిఖరం_Region() == ఇది);
        pImpl->చివర_Timestamp = చివర_Timestamp;
        pImpl->leaveRegion(ctx);
        pImpl->release();
        pImpl = ఖాళీ;
        DEBUG_ONLY(implFlags &= ~REGION_FLAG_చురుకైన);
    }
    లేకపోతే
    {
        CV_DbgAssert(ctx.stat_స్థితి.skip_లోతు <= current_లోతు);
    }

    ఒకవేళ (implFlags & REGION_FLAG_NEED_దొంతర_తీయి)
    {
        CV_DbgAssert(ctx.దొంతర_శిఖరం_Region() == ఇది);
        ctx.దొంతర_తీయి();
        ctx.stat_స్థితి.check_Reset_Skip_బహుళకం(current_లోతు);
        DEBUG_ONLY(implFlags &= ~REGION_FLAG_NEED_దొంతర_తీయి);
    }
    CV_LOG_CTX_STAT(ఖాళీ, _spaces(current_లోతు*4) << "===> " << ctx.stat << ' ' << ctx.stat_స్థితి);
}


TraceManagerThreadLocal::~TraceManagerThreadLocal()
{
}

శూన్య TraceManagerThreadLocal::dump_దొంతర(std::ostream& out, తార్కిక onlyFunctions) స్థిరరాశి
{
    std::stringstream ss;
    std::deque<దొంతర_నమోదు>::const_iterator it = దొంతర.begin();
    std::deque<దొంతర_నమోదు>::const_iterator end = దొంతర.end();
    పూర్ణాంకం లోతు = 0;
    కొరకు (; it != end; ++it)
    {
        స్థిరరాశి Region::LocationStaticStorage* location = it->location;
        ఒకవేళ (location)
        {
            ఒకవేళ (!onlyFunctions || (location->flags & REGION_FLAG_FUNCTION))
            {
                ss << _spaces(4*లోతు) << location->name << std::పంక్తి_ముగింపు;
                లోతు++;
            }
        }
        లేకపోతే
        {
            ss << _spaces(4*లోతు) << "<unknown>" << std::పంక్తి_ముగింపు;
            లోతు++;
        }
    }
    out << ss.str();
}

తరగతి AsyncTraceStorage CV_FINAL : బహిరంగ TraceStorage
{
    మార్చదగిన std::ofstream out;
బహిరంగ:
    స్థిరరాశి std::string name;

    AsyncTraceStorage(స్థిరరాశి std::string& filename) :
        out(filename.c_str(), std::ios::trunc),
        name(filename)
    {
        out << "#description: OpenCV trace file" << std::పంక్తి_ముగింపు;
        out << "#version: 1.0" << std::పంక్తి_ముగింపు;
    }
    ~AsyncTraceStorage()
    {
        out.close();
    }

    తార్కిక put(స్థిరరాశి Trace_సందేశం& సందేశం) స్థిరరాశి CV_OVERRIDE
    {
        ఒకవేళ (సందేశం.hasError)
            తిరిగి అబద్ధం;
        out << సందేశం.బఫరు;
        //DEBUG_ONLY(std::flush(out)); // TODO configure flag
        తిరిగి నిజం;
    }
};

తరగతి SyncTraceStorage CV_FINAL : బహిరంగ TraceStorage
{
    మార్చదగిన std::ofstream out;
    మార్చదగిన cv::Mutex mutex;
బహిరంగ:
    స్థిరరాశి std::string name;

    SyncTraceStorage(స్థిరరాశి std::string& filename) :
        out(filename.c_str(), std::ios::trunc),
        name(filename)
    {
        out << "#description: OpenCV trace file" << std::పంక్తి_ముగింపు;
        out << "#version: 1.0" << std::పంక్తి_ముగింపు;
    }
    ~SyncTraceStorage()
    {
        cv::Auto_తాళం l(mutex);
        out.close();
    }

    తార్కిక put(స్థిరరాశి Trace_సందేశం& సందేశం) స్థిరరాశి CV_OVERRIDE
    {
        ఒకవేళ (సందేశం.hasError)
            తిరిగి అబద్ధం;
        {
            cv::Auto_తాళం l(mutex);
            out << సందేశం.బఫరు;
            std::flush(out); // TODO configure flag
        }
        తిరిగి నిజం;
    }
};


TraceStorage* TraceManagerThreadLocal::getStorage() స్థిరరాశి
{
    // TODO configuration option for stdout/single trace file
    ఒకవేళ (storage.empty())
    {
        TraceStorage* global = getTraceManager().trace_storage.get();
        ఒకవేళ (global)
        {
            స్థిరరాశి std::string filepath = cv::format("%s-%03d.txt", getParameterTraceLocation().c_str(), threadID).c_str();
            Trace_సందేశం సందేశం;
            స్థిరరాశి అక్షరం* pos = strrchr(filepath.c_str(), '/'); // extract filename
#నిర్వచితమైతే _WIN32
            ఒకవేళ (!pos)
                pos = strrchr(filepath.c_str(), '\\');
#అయితే_ముగింపు
            ఒకవేళ (!pos)
                pos = filepath.c_str();
            లేకపోతే
                pos += 1; // fix to skip extra slash in filename beginning
            సందేశం.రాయి("#thread file: %s\n", pos);
            global->put(సందేశం);
            storage.reset(కొత్త AsyncTraceStorage(filepath));
        }
    }
    తిరిగి storage.get();
}



స్థిర తార్కిక activated = అబద్ధం;
స్థిర తార్కిక isInitialized = అబద్ధం;

TraceManager::TraceManager()
{
    (శూన్య)cv::getTimestampNS();

    isInitialized = నిజం;
    CV_LOG("TraceManager ctor: " << (శూన్య*)ఇది);

    CV_LOG("TraceManager configure()");
    activated = getParameterTraceEnable();

    ఒకవేళ (activated)
        trace_storage.reset(కొత్త SyncTraceStorage(std::string(getParameterTraceLocation()) + ".txt"));

#నిర్వచితమైతే OPENCV_WITH_ITT
    ఒకవేళ (isITTEnabled())
    {
        activated = నిజం; // force trace pipeline activation (without OpenCV storage)
        __itt_region_begin(domain, __itt_null, __itt_null, __itt_string_handle_create("OpenCVTrace"));
    }
#అయితే_ముగింపు
}
TraceManager::~TraceManager()
{
    CV_LOG("TraceManager dtor: " << (శూన్య*)ఇది);

#నిర్వచితమైతే OPENCV_WITH_ITT
    ఒకవేళ (isITTEnabled())
    {
        __itt_region_end(domain, __itt_null);
    }
#అయితే_ముగింపు

    std::vector<TraceManagerThreadLocal*> దారాలు_ctx;
    tls.gather(దారాలు_ctx);
    size_t మొత్తం_Events = 0, మొత్తం_Skipped_Events = 0;
    కొరకు (size_t i = 0; i < దారాలు_ctx.size(); i++)
    {
        TraceManagerThreadLocal* ctx = దారాలు_ctx[i];
        ఒకవేళ (ctx)
        {
            మొత్తం_Events += ctx->region_counter;
            మొత్తం_Skipped_Events += ctx->మొత్తం_Skipped_Events;
        }
    }
    ఒకవేళ (మొత్తం_Events || activated)
    {
        CV_LOG_INFO(ఖాళీ, "Trace: Total events: " << మొత్తం_Events);
    }
    ఒకవేళ (మొత్తం_Skipped_Events)
    {
        CV_LOG_WARNING(ఖాళీ, "Trace: Total skipped events: " << మొత్తం_Skipped_Events);
    }

    // This is a global static object, so process starts shutdown here
    // Turn off trace
    cv::__termination = నిజం; // also set in DllMain() notifications handler for DLL_PROCESS_DETACH
    activated = అబద్ధం;
}

తార్కిక TraceManager::isActivated()
{
    // Check if process starts shutdown, and set earlyExit to true
    // to prevent further instrumentation processing earlier.
    ఒకవేళ (cv::__termination)
    {
        activated = అబద్ధం;
        తిరిగి అబద్ధం;
    }

    ఒకవేళ (!isInitialized)
    {
        TraceManager& m = getTraceManager();
        CV_వాడని(m); // TODO
    }

    తిరిగి activated;
}


స్థిర TraceManager* getTraceManagerCallOnce()
{
    స్థిర TraceManager globalInstance;
    తిరిగి &globalInstance;
}
TraceManager& getTraceManager()
{
    CV_SINGLETON_LAZY_INIT_REF(TraceManager, getTraceManagerCallOnce())
}

శూన్య parallel_For_సమితి_మూలం_Region(స్థిరరాశి Region& మూలం_Region, స్థిరరాశి TraceManagerThreadLocal& మూలం_ctx)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();

    ఒకవేళ (ctx.dummy_దొంతర_శిఖరం.region == &మూలం_Region) // already attached
        తిరిగి;

    CV_Assert(ctx.dummy_దొంతర_శిఖరం.region == ఖాళీ);
    ctx.dummy_దొంతర_శిఖరం = TraceManagerThreadLocal::దొంతర_నమోదు(const_cast<Region*>(&మూలం_Region), ఖాళీ, -1);

    ఒకవేళ (&ctx == &మూలం_ctx)
    {
        ctx.stat.grab(ctx.parallel_for_stat);
        ctx.parallel_for_stat_స్థితి = ctx.stat_స్థితి;
        ctx.parallel_for_దొంతర_కొలత = ctx.దొంతర.size();
        తిరిగి;
    }

    CV_Assert(ctx.దొంతర.empty());

    ctx.current_చురుకైన_Region = const_cast<Region*>(&మూలం_Region);

    ctx.region_లోతు = మూలం_ctx.region_లోతు;
    ctx.region_లోతు_Open_CV = మూలం_ctx.region_లోతు_Open_CV;

    ctx.parallel_for_దొంతర_కొలత = 0;

    ctx.stat_స్థితి.propagateFrom(మూలం_ctx.stat_స్థితి);
}

శూన్య parallelForAttachNestedRegion(స్థిరరాశి Region& మూలం_Region)
{
    CV_వాడని(మూలం_Region);
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();

    CV_DbgAssert(ctx.dummy_దొంతర_శిఖరం.region == &మూలం_Region);

    Region* region = ctx.get_Current_చురుకైన_Region();
    CV_LOG_PARALLEL(ఖాళీ, " PARALLEL_FOR: " << (శూన్య*)region << " ==> " << &మూలం_Region);
    ఒకవేళ (!region)
        తిరిగి;

#నిర్వచితమైతే OPENCV_WITH_ITT
    ఒకవేళ (!మూలం_Region.pImpl || !మూలం_Region.pImpl->itt_id_registered)
        తిరిగి;

    ఒకవేళ (!region->pImpl)
        తిరిగి;

    CV_LOG_PARALLEL(ఖాళీ, " PARALLEL_FOR ITT: " << (శూన్య*)మూలం_Region.pImpl->itt_id.d1 << ":" << మూలం_Region.pImpl->itt_id.d2 << ":" << (శూన్య*)మూలం_Region.pImpl->itt_id.d3 << " => "
                                 << (శూన్య*)region->pImpl->itt_id.d1 << ":" << region->pImpl->itt_id.d2 << ":" << (శూన్య*)region->pImpl->itt_id.d3);
    __itt_relation_add(domain, region->pImpl->itt_id, __itt_relation_is_child_of, మూలం_Region.pImpl->itt_id);
#అయితే_ముగింపు
}

శూన్య parallelForFinalize(స్థిరరాశి Region& మూలం_Region)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();

    int64 చివర_Timestamp = getTimestampNS();
    int64 duration = చివర_Timestamp - ctx.దొంతర_శిఖరం_Begin_Timestamp();
    CV_LOG_PARALLEL(ఖాళీ, "parallel_for duration: " << duration << " " << &మూలం_Region);

    std::vector<TraceManagerThreadLocal*> దారాలు_ctx;
    getTraceManager().tls.gather(దారాలు_ctx);
    RegionStatistics parallel_for_stat;
    కొరకు (size_t i = 0; i < దారాలు_ctx.size(); i++)
    {
        TraceManagerThreadLocal* బిడ్డ_ctx = దారాలు_ctx[i];

        ఒకవేళ (బిడ్డ_ctx && బిడ్డ_ctx->దొంతర_శిఖరం_Region() == &మూలం_Region)
        {
            CV_LOG_PARALLEL(ఖాళీ, "Thread=" << బిడ్డ_ctx->threadID << " " << బిడ్డ_ctx->stat);
            RegionStatistics బిడ్డ_stat;
            బిడ్డ_ctx->stat.grab(బిడ్డ_stat);
            parallel_for_stat.append(బిడ్డ_stat);
            ఒకవేళ (బిడ్డ_ctx != &ctx)
            {
                బిడ్డ_ctx->dummy_దొంతర_శిఖరం = TraceManagerThreadLocal::దొంతర_నమోదు();
            }
            లేకపోతే
            {
                ctx.parallel_for_stat.grab(ctx.stat);
                ctx.stat_స్థితి = ctx.parallel_for_stat_స్థితి;
                బిడ్డ_ctx->dummy_దొంతర_శిఖరం = TraceManagerThreadLocal::దొంతర_నమోదు();
            }
        }
    }

    దశాంశం parallel_coeff = std::min(1.0f, duration / (దశాంశం)(parallel_for_stat.duration));
    CV_LOG_PARALLEL(ఖాళీ, "parallel_coeff=" << 1.0f / parallel_coeff);
    CV_LOG_PARALLEL(ఖాళీ, parallel_for_stat);
    ఒకవేళ (parallel_coeff != 1.0f)
    {
        parallel_for_stat.multiply(parallel_coeff);
        CV_LOG_PARALLEL(ఖాళీ, parallel_for_stat);
    }
    parallel_for_stat.duration = 0;
    ctx.stat.append(parallel_for_stat);
    CV_LOG_PARALLEL(ఖాళీ, ctx.stat);
}

నిర్మాణం TraceArg::Extra_దత్తాంశం
{
#నిర్వచితమైతే OPENCV_WITH_ITT
    // Special fields for ITT
    __itt_string_handle* అస్థిర ittHandle_name;
#అయితే_ముగింపు
    Extra_దత్తాంశం(TraceManagerThreadLocal& ctx, స్థిరరాశి TraceArg& arg)
    {
        CV_వాడని(ctx); CV_వాడని(arg);
#నిర్వచితమైతే OPENCV_WITH_ITT
        ఒకవేళ (isITTEnabled())
        {
            // Caching is not required here, because there is builtin cache.
            // https://software.intel.com/en-us/node/544203:
            //     Consecutive calls to __itt_string_handle_create with the same name return the same value.
            ittHandle_name = __itt_string_handle_create(arg.name);
        }
        లేకపోతే
        {
            ittHandle_name = 0;
        }
#అయితే_ముగింపు
    }
};

స్థిర శూన్య initTraceArg(TraceManagerThreadLocal& ctx, స్థిరరాశి TraceArg& arg)
{
    TraceArg::Extra_దత్తాంశం** pExtra = arg.ppExtra;
    ఒకవేళ (*pExtra == ఖాళీ)
    {
        cv::Auto_తాళం lock(cv::getInitializationMutex());
        ఒకవేళ (*pExtra == ఖాళీ)
        {
            *pExtra = కొత్త TraceArg::Extra_దత్తాంశం(ctx, arg);
        }
    }
}
శూన్య traceArg(స్థిరరాశి TraceArg& arg, స్థిరరాశి అక్షరం* value)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    Region* region = ctx.get_Current_చురుకైన_Region();
    ఒకవేళ (!region)
        తిరిగి;
    CV_Assert(region->pImpl);
    initTraceArg(ctx, arg);
    ఒకవేళ (!value)
        value = "<null>";
#నిర్వచితమైతే OPENCV_WITH_ITT
    ఒకవేళ (isITTEnabled())
    {
        __itt_metadata_str_add(domain, region->pImpl->itt_id, (*arg.ppExtra)->ittHandle_name, value, అక్షరశ్రేణి_పొడవు(value));
    }
#అయితే_ముగింపు
}
శూన్య traceArg(స్థిరరాశి TraceArg& arg, పూర్ణాంకం value)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    Region* region = ctx.get_Current_చురుకైన_Region();
    ఒకవేళ (!region)
        తిరిగి;
    CV_Assert(region->pImpl);
    initTraceArg(ctx, arg);
#నిర్వచితమైతే OPENCV_WITH_ITT
    ఒకవేళ (isITTEnabled())
    {
        __itt_metadata_add(domain, region->pImpl->itt_id, (*arg.ppExtra)->ittHandle_name, పరిమాణం(పూర్ణాంకం) == 4 ? __itt_metadata_s32 : __itt_metadata_s64, 1, &value);
    }
#లేకపోతే
    CV_వాడని(value);
#అయితే_ముగింపు
}
శూన్య traceArg(స్థిరరాశి TraceArg& arg, int64 value)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    Region* region = ctx.get_Current_చురుకైన_Region();
    ఒకవేళ (!region)
        తిరిగి;
    CV_Assert(region->pImpl);
    initTraceArg(ctx, arg);
#నిర్వచితమైతే OPENCV_WITH_ITT
    ఒకవేళ (isITTEnabled())
    {
        __itt_metadata_add(domain, region->pImpl->itt_id, (*arg.ppExtra)->ittHandle_name, __itt_metadata_s64, 1, &value);
    }
#లేకపోతే
    CV_వాడని(value);
#అయితే_ముగింపు
}
శూన్య traceArg(స్థిరరాశి TraceArg& arg, ద్విగుణ value)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    Region* region = ctx.get_Current_చురుకైన_Region();
    ఒకవేళ (!region)
        తిరిగి;
    CV_Assert(region->pImpl);
    initTraceArg(ctx, arg);
#నిర్వచితమైతే OPENCV_WITH_ITT
    ఒకవేళ (isITTEnabled())
    {
        __itt_metadata_add(domain, region->pImpl->itt_id, (*arg.ppExtra)->ittHandle_name, __itt_metadata_double, 1, &value);
    }
#లేకపోతే
    CV_వాడని(value);
#అయితే_ముగింపు
}

#లేకపోతే

Region::Region(స్థిరరాశి LocationStaticStorage&) : pImpl(ఖాళీ), implFlags(0) {}
శూన్య Region::destroy() {}

శూన్య traceArg(స్థిరరాశి TraceArg&, స్థిరరాశి అక్షరం*) {}
శూన్య traceArg(స్థిరరాశి TraceArg&, పూర్ణాంకం) {};
శూన్య traceArg(స్థిరరాశి TraceArg&, int64) {};
శూన్య traceArg(స్థిరరాశి TraceArg&, ద్విగుణ) {};

#అయితే_ముగింపు

}}}} // namespace
