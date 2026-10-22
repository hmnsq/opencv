// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ଅନ୍ତର୍ଭାଵ "precomp.hpp"

#ଅନ୍ତର୍ଭାଵ <opencv2/core/utils/trace.hpp>
#ଅନ୍ତର୍ଭାଵ <opencv2/core/utils/trace.private.hpp>
#ଅନ୍ତର୍ଭାଵ <opencv2/core/utils/configuration.private.hpp>

#ଅନ୍ତର୍ଭାଵ <opencv2/core/opencl/ocl_defs.hpp>

#ଅନ୍ତର୍ଭାଵ <cstdarg> // va_start

#ଅନ୍ତର୍ଭାଵ <sstream>
#ଅନ୍ତର୍ଭାଵ <ostream>
#ଅନ୍ତର୍ଭାଵ <fstream>

#ଯଦି 0
#define CV_LOG(...) CV_LOG_INFO(ଖାଲି, __VA_ARGS__)
#ଅନ୍ୟଥା
#define CV_LOG(...) {}
#endif

#ଯଦି 0
#define CV_LOG_ITT(...) CV_LOG_INFO(ଖାଲି, __VA_ARGS__)
#ଅନ୍ୟଥା
#define CV_LOG_ITT(...) {}
#endif

#ଯଦି 1
#define CV_LOG_TRACE_BAILOUT(tag, ...) CV_LOG_INFO(tag, __VA_ARGS__)
#ଅନ୍ୟଥା
#define CV_LOG_TRACE_BAILOUT(...) {}
#endif

#ଯଦି 0
#define CV_LOG_PARALLEL(tag, ...) CV_LOG_INFO(tag, __VA_ARGS__)
#ଅନ୍ୟଥା
#define CV_LOG_PARALLEL(...) {}
#endif

#ଯଦି 0
#define CV_LOG_CTX_STAT(tag, ...) CV_LOG_INFO(tag, __VA_ARGS__)
#ଅନ୍ୟଥା
#define CV_LOG_CTX_STAT(...) {}
#endif

#ଯଦି 0
#define CV_LOG_SKIP(tag, ...) CV_LOG_INFO(tag, __VA_ARGS__)
#ଅନ୍ୟଥା
#define CV_LOG_SKIP(...) {}
#endif

ନାମାକାଶ cv {
ନାମାକାଶ utils {
ନାମାକାଶ trace {
ନାମାକାଶ details {

#ifdef OPENCV_TRACE

#ifdef _MSC_VER
#pragma warning(disable:4065) // switch statement contains 'default' but no 'case' labels
#endif

ସ୍ଥିର ତାର୍କିକ getParameterTraceEnable()
{
    ସ୍ଥିର ତାର୍କିକ param_traceEnable = utils::getConfigurationParameterBool("OPENCV_TRACE", ମିଥ୍ୟା);
    ଫେରନ୍ତୁ param_traceEnable;
}

// TODO lazy configuration flags
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା param_maxRegionDepthOpenCV = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)utils::getConfigurationParameterSizeT("OPENCV_TRACE_DEPTH_OPENCV", 1);
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା param_maxRegionChildrenOpenCV = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)utils::getConfigurationParameterSizeT("OPENCV_TRACE_MAX_CHILDREN_OPENCV", 1000);
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା param_maxRegionChildren = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)utils::getConfigurationParameterSizeT("OPENCV_TRACE_MAX_CHILDREN", 10000);

ସ୍ଥିର ଅଚଳ cv::String& getParameterTraceLocation()
{
    ସ୍ଥିର cv::String param_traceLocation = utils::getConfigurationParameterString("OPENCV_TRACE_LOCATION", "OpenCVTrace");
    ଫେରନ୍ତୁ param_traceLocation;
}

#ifdef HAVE_OPENCL
ସ୍ଥିର ତାର୍କିକ param_synchronizeOpenCL = utils::getConfigurationParameterBool("OPENCV_TRACE_SYNC_OPENCL", ମିଥ୍ୟା);
#endif

#ifdef OPENCV_WITH_ITT
ସ୍ଥିର ତାର୍କିକ param_ITT_registerParentScope = utils::getConfigurationParameterBool("OPENCV_TRACE_ITT_PARENT", ମିଥ୍ୟା);
#endif

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର* _spaces(ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_3)
{
    ସ୍ଥିର ଅଚଳ ଅକ୍ଷର buf[64] =
"                                                               ";
    ଫେରନ୍ତୁ &buf[63 - (ଗଣନା_3 & 63)];
}

/**
 * Text-based trace messages
 */
ଶ୍ରେଣୀ TraceMessage
{
ସର୍ବସାଧାରଣ:
    ଅକ୍ଷର buffer[1024];
    size_t len;
    ତାର୍କିକ hasError;

    TraceMessage() :
        len(0),
        hasError(ମିଥ୍ୟା)
    {}

    ତାର୍କିକ printf(ଅଚଳ ଅକ୍ଷର* format, ...)
    {
        ଅକ୍ଷର* buf = &buffer[len];
        size_t sz = ଆକାର(buffer) - len;
        va_list ap;
        va_start(ap, format);
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା n = cv_vsnprintf(buf, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)sz, format, ap);
        va_end(ap);
        ଯଦି (n < 0 || (size_t)n > sz)
        {
            hasError = ସତ୍ୟ;
            ଫେରନ୍ତୁ ମିଥ୍ୟା;
        }
        len += n;
        ଫେରନ୍ତୁ ସତ୍ୟ;
    }

    ତାର୍କିକ formatlocation(ଅଚଳ Region::LocationStaticStorage& location)
    {
        ଫେରନ୍ତୁ ଏହି->printf("l,%lld,\"%s\",%d,\"%s\",0x%llX\n",
                (long long ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(*location.ppExtra)->global_location_id,
                location.filename,
                location.ଧାଡ଼ି,
                location.name,
                (long long ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(location.flags & ~0xF0000000));
    }
    ତାର୍କିକ formatRegionEnter(ଅଚଳ Region& region)
    {
        ତାର୍କିକ ok = ଏହି->printf("b,%d,%lld,%lld,%lld",
                (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)region.pImpl->threadID,
                (long long ପୂର୍ଣ୍ଣସଂଖ୍ୟା)region.pImpl->beginTimestamp,
                (long long ପୂର୍ଣ୍ଣସଂଖ୍ୟା)((*region.pImpl->location.ppExtra)->global_location_id),
                (long long ପୂର୍ଣ୍ଣସଂଖ୍ୟା)region.pImpl->global_region_id);
        ଯଦି (region.pImpl->parentRegion && region.pImpl->parentRegion->pImpl)
        {
            ଯଦି (region.pImpl->parentRegion->pImpl->threadID != region.pImpl->threadID)
                ok &= ଏହି->printf(",parentThread=%d,parent=%lld",
                        (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)region.pImpl->parentRegion->pImpl->threadID,
                        (long long ପୂର୍ଣ୍ଣସଂଖ୍ୟା)region.pImpl->parentRegion->pImpl->global_region_id);
        }
        ok &= ଏହି->printf("\n");
        ଫେରନ୍ତୁ ok;
    }
    ତାର୍କିକ formatRegionLeave(ଅଚଳ Region& region, ଅଚଳ RegionStatistics& result)
    {
        CV_DbgAssert(region.pImpl->endTimestamp - region.pImpl->beginTimestamp == result.duration);
        ତାର୍କିକ ok = ଏହି->printf("e,%d,%lld,%lld,%lld,%lld",
                (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)region.pImpl->threadID,
                (long long ପୂର୍ଣ୍ଣସଂଖ୍ୟା)region.pImpl->endTimestamp,
                (long long ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(*region.pImpl->location.ppExtra)->global_location_id,
                (long long ପୂର୍ଣ୍ଣସଂଖ୍ୟା)region.pImpl->global_region_id,
                (long long ପୂର୍ଣ୍ଣସଂଖ୍ୟା)result.duration);
        ଯଦି (result.currentSkippedRegions)
            ok &= ଏହି->printf(",skip=%d", (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)result.currentSkippedRegions);
#ifdef HAVE_IPP
        ଯଦି (result.durationImplIPP)
            ok &= ଏହି->printf(",tIPP=%lld", (long long ପୂର୍ଣ୍ଣସଂଖ୍ୟା)result.durationImplIPP);
#endif
#ifdef HAVE_OPENCL
        ଯଦି (result.durationImplOpenCL)
            ok &= ଏହି->printf(",tOCL=%lld", (long long ପୂର୍ଣ୍ଣସଂଖ୍ୟା)result.durationImplOpenCL);
#endif
        ok &= ଏହି->printf("\n");
        ଫେରନ୍ତୁ ok;
    }
    ତାର୍କିକ recordRegionArg(ଅଚଳ Region& region, ଅଚଳ TraceArg& arg, ଅଚଳ ଅକ୍ଷର* value)
    {
        ଫେରନ୍ତୁ ଏହି->printf("a,%d,%lld,%lld,\"%s\",\"%s\"\n",
                region.pImpl->threadID,
                (long long ପୂର୍ଣ୍ଣସଂଖ୍ୟା)region.pImpl->beginTimestamp,
                (long long ପୂର୍ଣ୍ଣସଂଖ୍ୟା)region.pImpl->global_region_id,
                arg.name,
                value);
    }
};


#ifdef OPENCV_WITH_ITT
ସ୍ଥିର __itt_domain* domain = ଖାଲି;

ସ୍ଥିର ତାର୍କିକ isITTEnabled()
{
    ସ୍ଥିର ଅସ୍ଥିର ତାର୍କିକ isInitialized = ମିଥ୍ୟା;
    ସ୍ଥିର ତାର୍କିକ isEnabled = ମିଥ୍ୟା;
    ଯଦି (!isInitialized)
    {
        cv::AutoLock lock(cv::getInitializationMutex());
        ଯଦି (!isInitialized)
        {
            ତାର୍କିକ param_traceITTEnable = utils::getConfigurationParameterBool("OPENCV_TRACE_ITT_ENABLE", ସତ୍ୟ);
            ଯଦି (param_traceITTEnable)
            {
                isEnabled = !!(__itt_api_version());
                CV_LOG_ITT("ITT is " << (isEnabled ? "enabled" : "disabled"));
                domain = __itt_domain_create("OpenCVTrace");
            }
            ଅନ୍ୟଥା
            {
                CV_LOG_ITT("ITT is disabled through OpenCV parameter");
                isEnabled = ମିଥ୍ୟା;
            }
            isInitialized = ସତ୍ୟ;
        }
    }
    ଫେରନ୍ତୁ isEnabled;
}
#endif


Region::LocationExtraData::LocationExtraData(ଅଚଳ LocationStaticStorage& location)
{
    CV_UNUSED(location);
    ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା g_location_id_counter = 0;
    global_location_id = CV_XADD(&g_location_id_counter, 1) + 1;
    CV_LOG("Register location: " << global_location_id << " (" << (ଶୂନ୍ୟ*)&location << ")"
            << std::endl << "    file: " << location.filename
            << std::endl << "    line: " << location.ଧାଡ଼ି
            << std::endl << "    name: " << location.name);
#ifdef OPENCV_WITH_ITT
    ଯଦି (isITTEnabled())
    {
        // Caching is not required here, because there is builtin cache.
        // https://software.intel.com/en-us/node/544203:
        //     Consecutive calls to __itt_string_handle_create with the same name return the same value.
        ittHandle_name = __itt_string_handle_create(location.name);
        ittHandle_filename = __itt_string_handle_create(location.filename);
    }
    ଅନ୍ୟଥା
    {
        ittHandle_name = 0;
        ittHandle_filename = 0;
    }
#endif
}

/*static*/ Region::LocationExtraData* Region::LocationExtraData::init(ଅଚଳ Region::LocationStaticStorage& location)
{
    LocationExtraData** pLocationExtra = location.ppExtra;
    CV_DbgAssert(pLocationExtra);
    ଯଦି (*pLocationExtra == ଖାଲି)
    {
        cv::AutoLock lock(cv::getInitializationMutex());
        ଯଦି (*pLocationExtra == ଖାଲି)
        {
            *pLocationExtra = ନୂତନ Region::LocationExtraData(location);
            TraceStorage* s = getTraceManager().trace_storage.get();
            ଯଦି (s)
            {
                TraceMessage ସନ୍ଦେଶ;
                ସନ୍ଦେଶ.formatlocation(location);
                s->put(ସନ୍ଦେଶ);
            }
        }
    }
    ଫେରନ୍ତୁ *pLocationExtra;
}


Region::Impl::Impl(TraceManagerThreadLocal& ctx, Region* parentRegion_, Region& region_, ଅଚଳ LocationStaticStorage& location_, int64 beginTimestamp_) :
    location(location_),
    region(region_),
    parentRegion(parentRegion_),
    threadID(ctx.threadID),
    global_region_id(++ctx.region_counter),
    beginTimestamp(beginTimestamp_),
    endTimestamp(0),
    direct_Children_ଗଣନା(0)
#ifdef OPENCV_WITH_ITT
    ,itt_id_registered(ମିଥ୍ୟା)
    ,itt_id(__itt_null)
#endif
{
    CV_DbgAssert(ctx.currentActiveRegion == parentRegion);
    region.pImpl = ଏହି;

    registerRegion(ctx);

    enterRegion(ctx);
}

Region::Impl::~Impl()
{
#ifdef OPENCV_WITH_ITT
    ଯଦି (itt_id_registered)
    {
        CV_LOG_ITT(" Destroy ITT region: I=" << (ଶୂନ୍ୟ*)ଏହି);
        __itt_id_destroy(domain, itt_id);
        itt_id_registered = ମିଥ୍ୟା;
    }
#endif
    region.pImpl = ଖାଲି;
}

ଶୂନ୍ୟ Region::Impl::enterRegion(TraceManagerThreadLocal& ctx)
{
    ctx.currentActiveRegion = &region;

    ଯଦି (location.flags & REGION_FLAG_FUNCTION)
    {
        ଯଦି ((location.flags & REGION_FLAG_APP_CODE) == 0)
        {
            ctx.regionDepthOpenCV++;
        }
        ctx.regionDepth++;
    }

    TraceStorage* s = ctx.getStorage();
    ଯଦି (s)
    {
        TraceMessage ସନ୍ଦେଶ;
        ସନ୍ଦେଶ.formatRegionEnter(region);
        s->put(ସନ୍ଦେଶ);
    }
#ifdef OPENCV_WITH_ITT
    ଯଦି (isITTEnabled())
    {
        __itt_id parentID = __itt_null;
        ଯଦି (param_ITT_registerParentScope && parentRegion && parentRegion->pImpl && parentRegion->pImpl->itt_id_registered && (location.flags & REGION_FLAG_REGION_FORCE) == 0)
            parentID = parentRegion->pImpl->itt_id;
        __itt_task_begin(domain, itt_id, parentID, (*location.ppExtra)->ittHandle_name);
    }
#endif
}

ଶୂନ୍ୟ Region::Impl::leaveRegion(TraceManagerThreadLocal& ctx)
{
    int64 duration = endTimestamp - beginTimestamp; CV_UNUSED(duration);
    RegionStatistics result;
    ctx.stat.grab(result);
    ctx.totalSkippedEvents += result.currentSkippedRegions;
    CV_LOG(_spaces(ctx.getCurrentDepth()*4) << "leaveRegion(): " << (ଶୂନ୍ୟ*)ଏହି << " " << result);
#ifdef OPENCV_WITH_ITT
    ଯଦି (isITTEnabled())
    {
        ଯଦି (result.currentSkippedRegions)
        {
            __itt_metadata_add(domain, itt_id, __itt_string_handle_create("skipped trace entries"), __itt_metadata_u32, 1, &result.currentSkippedRegions);
        }
#ifdef HAVE_IPP
        ଯଦି (result.durationImplIPP)
            __itt_metadata_add(domain, itt_id, __itt_string_handle_create("tIPP"), __itt_metadata_u64, 1, &result.durationImplIPP);
#endif
#ifdef HAVE_OPENCL
        ଯଦି (result.durationImplOpenCL)
            __itt_metadata_add(domain, itt_id, __itt_string_handle_create("tOpenCL"), __itt_metadata_u64, 1, &result.durationImplOpenCL);
#endif
        __itt_task_end(domain);
    }
#endif
    TraceStorage* s = ctx.getStorage();
    ଯଦି (s)
    {
        TraceMessage ସନ୍ଦେଶ;
        ସନ୍ଦେଶ.formatRegionLeave(region, result);
        s->put(ସନ୍ଦେଶ);
    }

    ଯଦି (location.flags & REGION_FLAG_FUNCTION)
    {
        ଯଦି ((location.flags & REGION_FLAG_APP_CODE) == 0)
        {
            ctx.regionDepthOpenCV--;
        }
        ctx.regionDepth--;
    }

    ctx.currentActiveRegion = parentRegion;
}

ଶୂନ୍ୟ Region::Impl::release()
{
    delete ଏହି;
}

ଶୂନ୍ୟ Region::Impl::registerRegion(TraceManagerThreadLocal& ctx)
{
#ifdef OPENCV_WITH_ITT
    ଯଦି (isITTEnabled())
    {
        ଯଦି (!itt_id_registered)
        {
            CV_LOG_ITT(" Register ITT region: I=" << (ଶୂନ୍ୟ*)ଏହି << " " << ctx.threadID << "-" << global_region_id);
#ଯଦି 1 // workaround for some ITT backends
            itt_id = __itt_id_make((ଶୂନ୍ୟ*)(intptr_t)(((int64)(ctx.threadID + 1) << 32) | global_region_id), global_region_id);
#ଅନ୍ୟଥା
            itt_id = __itt_id_make((ଶୂନ୍ୟ*)(intptr_t)(ctx.threadID + 1), global_region_id);
#endif
            __itt_id_create(domain, itt_id);
            itt_id_registered = ସତ୍ୟ;
        }
    }
#ଅନ୍ୟଥା
    CV_UNUSED(ctx);
#endif
}

ଶୂନ୍ୟ RegionStatisticsStatus::enableSkipMode(ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth)
{
    CV_DbgAssert(_skipDepth < 0);
    CV_LOG_SKIP(ଖାଲି, "SKIP-ENABLE: depth=" << depth);
    _skipDepth = depth;
}
ଶୂନ୍ୟ RegionStatisticsStatus::checkResetSkipMode(ପୂର୍ଣ୍ଣସଂଖ୍ୟା leaveDepth)
{
    ଯଦି (leaveDepth <= _skipDepth)
    {
        CV_LOG_SKIP(ଖାଲି, "SKIP-RESET: leaveDepth=" << leaveDepth << " skipDepth=" << _skipDepth);
        _skipDepth = -1;
    }
}

Region::Region(ଅଚଳ LocationStaticStorage& location) :
    pImpl(ଖାଲି),
    implFlags(0)
{
    // Checks:
    // - global enable flag
    // - parent region is disabled
    // - children count threshold
    // - region location
    // - depth (opencv nested calls)
    ଯଦି (!TraceManager::isActivated())
    {
        CV_LOG("Trace is disabled. Bailout");
        ଫେରନ୍ତୁ;
    }

    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    CV_LOG(_spaces(ctx.getCurrentDepth()*4) << "Region(): " << (ଶୂନ୍ୟ*)ଏହି << ": " << location.name);

    Region* parentRegion = ctx.ଗଦା_ଶୀର୍ଷ_Region();
    ଅଚଳ Region::LocationStaticStorage* parentLocation = ctx.ଗଦା_ଶୀର୍ଷ_Location();

    ଯଦି (location.flags & REGION_FLAG_REGION_NEXT)
    {
        ଯଦି (parentRegion && parentRegion->pImpl)
        {
            CV_DbgAssert((parentRegion->pImpl->location.flags & REGION_FLAG_FUNCTION) == 0);
            parentRegion->destroy(); parentRegion->implFlags = 0;
            parentRegion = ctx.ଗଦା_ଶୀର୍ଷ_Region();
            parentLocation = ctx.ଗଦା_ଶୀର୍ଷ_Location();
        }
    }

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା parentChildren = 0;
    ଯଦି (parentRegion && parentRegion->pImpl)
    {
        ଯଦି (parentLocation == ଖାଲି)
        {
            // parallel_for_body code path
            parentChildren = CV_XADD(&parentRegion->pImpl->direct_Children_ଗଣନା, 1) + 1;
        }
        ଅନ୍ୟଥା
        {
            parentChildren = ++parentRegion->pImpl->direct_Children_ଗଣନା;
        }
    }

    int64 beginTimestamp = getTimestampNS();

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା currentDepth = ctx.getCurrentDepth() + 1;
    ବାଛ (location.flags & REGION_FLAG_IMPL_MASK)
    {
#ifdef HAVE_IPP
    ସ୍ଥିତି REGION_FLAG_IMPL_IPP:
        ଯଦି (!ctx.stat_status.ignoreDepthImplIPP)
            ctx.stat_status.ignoreDepthImplIPP = currentDepth;
        ଭାଙ୍ଗ;
#endif
#ifdef HAVE_OPENCL
    ସ୍ଥିତି REGION_FLAG_IMPL_OPENCL:
        ଯଦି (!ctx.stat_status.ignoreDepthImplOpenCL)
            ctx.stat_status.ignoreDepthImplOpenCL = currentDepth;
        ଭାଙ୍ଗ;
#endif
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
        ଭାଙ୍ଗ;
    }

    ctx.ଗଦା_ଠେଲ(ଏହି, &location, beginTimestamp);
    implFlags |= REGION_FLAG_NEED_ଗଦା_କାଢ଼;

    ଯଦି ((location.flags & REGION_FLAG_REGION_FORCE) == 0)
    {
        ଯଦି (ctx.stat_status._skipDepth >= 0 && currentDepth > ctx.stat_status._skipDepth)
        {
            CV_LOG(_spaces(ctx.getCurrentDepth()*4) << "Parent region is disabled. Bailout");
            ctx.stat.currentSkippedRegions++;
            ଫେରନ୍ତୁ;
        }

        ଯଦି (param_maxRegionChildrenOpenCV > 0 && (location.flags & REGION_FLAG_APP_CODE) == 0 && parentLocation && (parentLocation->flags & REGION_FLAG_APP_CODE) == 0)
        {
            ଯଦି (parentChildren >= param_maxRegionChildrenOpenCV)
            {
                CV_LOG_TRACE_BAILOUT(ଖାଲି, _spaces(ctx.getCurrentDepth()*4) << "OpenCV parent region exceeds children count. Bailout");
                ctx.stat_status.enableSkipMode(currentDepth - 1);
                ctx.stat.currentSkippedRegions++;
                DEBUG_ONLY(ctx.dump_ଗଦା(std::cout, ମିଥ୍ୟା));
                ଫେରନ୍ତୁ;
            }
        }
        ଯଦି (param_maxRegionChildren > 0 && parentChildren >= param_maxRegionChildren)
        {
            CV_LOG_TRACE_BAILOUT(ଖାଲି, _spaces(ctx.getCurrentDepth()*4) << "Parent region exceeds children count. Bailout");
            ctx.stat_status.enableSkipMode(currentDepth - 1);
            ctx.stat.currentSkippedRegions++;
            DEBUG_ONLY(ctx.dump_ଗଦା(std::cout, ମିଥ୍ୟା));
            ଫେରନ୍ତୁ;
        }
    }

    LocationExtraData::init(location);

    ଯଦି ((*location.ppExtra)->global_location_id == 0)
    {
        CV_LOG_TRACE_BAILOUT(ଖାଲି, _spaces(ctx.getCurrentDepth()*4) << "Region location is disabled. Bailout");
        ctx.stat_status.enableSkipMode(currentDepth);
        ctx.stat.currentSkippedRegions++;
        ଫେରନ୍ତୁ;
    }

    ଯଦି (parentLocation && (parentLocation->flags & REGION_FLAG_SKIP_NESTED))
    {
        CV_LOG(_spaces(ctx.getCurrentDepth()*4) << "Parent region disables inner regions. Bailout");
        ctx.stat_status.enableSkipMode(currentDepth);
        ctx.stat.currentSkippedRegions++;
        ଫେରନ୍ତୁ;
    }

    ଯଦି (param_maxRegionDepthOpenCV)
    {
        ଯଦି ((location.flags & REGION_FLAG_APP_CODE) == 0)
        {
            ଯଦି (ctx.regionDepthOpenCV >= param_maxRegionDepthOpenCV)
            {
                CV_LOG(_spaces(ctx.getCurrentDepth()*4) << "OpenCV region depth is exceed = " << ctx.regionDepthOpenCV << ". Bailout");
                ଯଦି (ctx.stat.currentSkippedRegions == 0)
                {
                    DEBUG_ONLY(ctx.dump_ଗଦା(std::cout, ମିଥ୍ୟା));
                }
                ctx.stat_status.enableSkipMode(currentDepth);
                ctx.stat.currentSkippedRegions++;
                ଫେରନ୍ତୁ;
            }
        }
    }

    ନୂତନ Impl(ctx, parentRegion, *ଏହି, location, beginTimestamp);
    CV_DbgAssert(pImpl != ଖାଲି);
    implFlags |= REGION_FLAG__ACTIVE;

    // parallel_for path
    ଯଦି (parentRegion && parentRegion->pImpl)
    {
        ଯଦି (parentLocation == ଖାଲି)
        {
            pImpl->direct_Children_ଗଣନା = parentChildren;
        }
    }
}

ଶୂନ୍ୟ Region::destroy()
{
    CV_DbgAssert(implFlags != 0);

    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    CV_LOG(_spaces(ctx.getCurrentDepth()*4) << "Region::destruct(): " << (ଶୂନ୍ୟ*)ଏହି << " pImpl=" << pImpl << " implFlags=" << implFlags << ' ' << (ctx.ଗଦା_ଶୀର୍ଷ_Location() ? ctx.ଗଦା_ଶୀର୍ଷ_Location()->name : "<unknown>"));

    CV_DbgAssert(implFlags & REGION_FLAG_NEED_ଗଦା_କାଢ଼);
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା currentDepth = ctx.getCurrentDepth(); CV_UNUSED(currentDepth);

    CV_LOG_CTX_STAT(ଖାଲି, _spaces(currentDepth*4) << ctx.stat << ' ' << ctx.stat_status);

    ଅଚଳ Region::LocationStaticStorage* location = ctx.ଗଦା_ଶୀର୍ଷ_Location();
    Impl::OptimizationPath myCodePath = Impl::CODE_PATH_PLAIN;
    ଯଦି (location)
    {
        ବାଛ (location->flags & REGION_FLAG_IMPL_MASK)
        {
#ifdef HAVE_IPP
        ସ୍ଥିତି REGION_FLAG_IMPL_IPP:
            myCodePath = Impl::CODE_PATH_IPP;
            ଭାଙ୍ଗ;
#endif
#ifdef HAVE_OPENCL
        ସ୍ଥିତି REGION_FLAG_IMPL_OPENCL:
            ଯଦି (param_synchronizeOpenCL && cv::ocl::isOpenCLActivated())
                cv::ocl::finish();
            myCodePath = Impl::CODE_PATH_OPENCL;
            ଭାଙ୍ଗ;
#endif
        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
            ଭାଙ୍ଗ;
        }
    }

    int64 endTimestamp = getTimestampNS();
    int64 duration = endTimestamp - ctx.ଗଦା_ଶୀର୍ଷ_Begin_Timestamp();

    ତାର୍କିକ active = isActive();

    ଯଦି (active)
        ctx.stat.duration = duration;
    ଅନ୍ୟଥା ଯଦି (ctx.ଗଦା.size() == ctx.parallel_for_ଗଦା_size + 1)
        ctx.stat.duration += duration;

    ବାଛ (myCodePath) {
        ସ୍ଥିତି Impl::CODE_PATH_PLAIN:
            // nothing
            ଭାଙ୍ଗ;
#ifdef HAVE_IPP
        ସ୍ଥିତି Impl::CODE_PATH_IPP:
            ଯଦି (ctx.stat_status.ignoreDepthImplIPP == currentDepth)
            {
                ctx.stat.durationImplIPP += duration;
                ctx.stat_status.ignoreDepthImplIPP = 0;
            }
            ଅନ୍ୟଥା ଯଦି (active)
            {
                ctx.stat.durationImplIPP = duration;
            }
            ଭାଙ୍ଗ;
#endif
#ifdef HAVE_OPENCL
        ସ୍ଥିତି Impl::CODE_PATH_OPENCL:
            ଯଦି (ctx.stat_status.ignoreDepthImplOpenCL == currentDepth)
            {
                ctx.stat.durationImplOpenCL += duration;
                ctx.stat_status.ignoreDepthImplOpenCL = 0;
            }
            ଅନ୍ୟଥା ଯଦି (active)
            {
                ctx.stat.durationImplOpenCL = duration;
            }
            ଭାଙ୍ଗ;
#endif
        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
            ଭାଙ୍ଗ;
    }

    ଯଦି (pImpl)
    {
        CV_DbgAssert((implFlags & (REGION_FLAG__ACTIVE | REGION_FLAG_NEED_ଗଦା_କାଢ଼)) == (REGION_FLAG__ACTIVE | REGION_FLAG_NEED_ଗଦା_କାଢ଼));
        CV_DbgAssert(ctx.ଗଦା_ଶୀର୍ଷ_Region() == ଏହି);
        pImpl->endTimestamp = endTimestamp;
        pImpl->leaveRegion(ctx);
        pImpl->release();
        pImpl = ଖାଲି;
        DEBUG_ONLY(implFlags &= ~REGION_FLAG__ACTIVE);
    }
    ଅନ୍ୟଥା
    {
        CV_DbgAssert(ctx.stat_status._skipDepth <= currentDepth);
    }

    ଯଦି (implFlags & REGION_FLAG_NEED_ଗଦା_କାଢ଼)
    {
        CV_DbgAssert(ctx.ଗଦା_ଶୀର୍ଷ_Region() == ଏହି);
        ctx.ଗଦା_କାଢ଼();
        ctx.stat_status.checkResetSkipMode(currentDepth);
        DEBUG_ONLY(implFlags &= ~REGION_FLAG_NEED_ଗଦା_କାଢ଼);
    }
    CV_LOG_CTX_STAT(ଖାଲି, _spaces(currentDepth*4) << "===> " << ctx.stat << ' ' << ctx.stat_status);
}


TraceManagerThreadLocal::~TraceManagerThreadLocal()
{
}

ଶୂନ୍ୟ TraceManagerThreadLocal::dump_ଗଦା(std::ostream& out, ତାର୍କିକ onlyFunctions) ଅଚଳ
{
    std::stringstream ss;
    std::deque<ଗଦା_Entry>::const_iterator it = ଗଦା.begin();
    std::deque<ଗଦା_Entry>::const_iterator end = ଗଦା.end();
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth = 0;
    ପାଇଁ (; it != end; ++it)
    {
        ଅଚଳ Region::LocationStaticStorage* location = it->location;
        ଯଦି (location)
        {
            ଯଦି (!onlyFunctions || (location->flags & REGION_FLAG_FUNCTION))
            {
                ss << _spaces(4*depth) << location->name << std::endl;
                depth++;
            }
        }
        ଅନ୍ୟଥା
        {
            ss << _spaces(4*depth) << "<unknown>" << std::endl;
            depth++;
        }
    }
    out << ss.str();
}

ଶ୍ରେଣୀ AsyncTraceStorage CV_FINAL : ସର୍ବସାଧାରଣ TraceStorage
{
    mutable std::ofstream out;
ସର୍ବସାଧାରଣ:
    ଅଚଳ std::string name;

    AsyncTraceStorage(ଅଚଳ std::string& filename) :
        out(filename.c_str(), std::ios::trunc),
        name(filename)
    {
        out << "#description: OpenCV trace file" << std::endl;
        out << "#version: 1.0" << std::endl;
    }
    ~AsyncTraceStorage()
    {
        out.close();
    }

    ତାର୍କିକ put(ଅଚଳ TraceMessage& ସନ୍ଦେଶ) ଅଚଳ CV_OVERRIDE
    {
        ଯଦି (ସନ୍ଦେଶ.hasError)
            ଫେରନ୍ତୁ ମିଥ୍ୟା;
        out << ସନ୍ଦେଶ.buffer;
        //DEBUG_ONLY(std::flush(out)); // TODO configure flag
        ଫେରନ୍ତୁ ସତ୍ୟ;
    }
};

ଶ୍ରେଣୀ SyncTraceStorage CV_FINAL : ସର୍ବସାଧାରଣ TraceStorage
{
    mutable std::ofstream out;
    mutable cv::Mutex mutex;
ସର୍ବସାଧାରଣ:
    ଅଚଳ std::string name;

    SyncTraceStorage(ଅଚଳ std::string& filename) :
        out(filename.c_str(), std::ios::trunc),
        name(filename)
    {
        out << "#description: OpenCV trace file" << std::endl;
        out << "#version: 1.0" << std::endl;
    }
    ~SyncTraceStorage()
    {
        cv::AutoLock l(mutex);
        out.close();
    }

    ତାର୍କିକ put(ଅଚଳ TraceMessage& ସନ୍ଦେଶ) ଅଚଳ CV_OVERRIDE
    {
        ଯଦି (ସନ୍ଦେଶ.hasError)
            ଫେରନ୍ତୁ ମିଥ୍ୟା;
        {
            cv::AutoLock l(mutex);
            out << ସନ୍ଦେଶ.buffer;
            std::flush(out); // TODO configure flag
        }
        ଫେରନ୍ତୁ ସତ୍ୟ;
    }
};


TraceStorage* TraceManagerThreadLocal::getStorage() ଅଚଳ
{
    // TODO configuration option for stdout/single trace file
    ଯଦି (storage.empty())
    {
        TraceStorage* global = getTraceManager().trace_storage.get();
        ଯଦି (global)
        {
            ଅଚଳ std::string filepath = cv::format("%s-%03d.txt", getParameterTraceLocation().c_str(), threadID).c_str();
            TraceMessage ସନ୍ଦେଶ;
            ଅଚଳ ଅକ୍ଷର* pos = strrchr(filepath.c_str(), '/'); // extract filename
#ifdef _WIN32
            ଯଦି (!pos)
                pos = strrchr(filepath.c_str(), '\\');
#endif
            ଯଦି (!pos)
                pos = filepath.c_str();
            ଅନ୍ୟଥା
                pos += 1; // fix to skip extra slash in filename beginning
            ସନ୍ଦେଶ.printf("#thread file: %s\n", pos);
            global->put(ସନ୍ଦେଶ);
            storage.reset(ନୂତନ AsyncTraceStorage(filepath));
        }
    }
    ଫେରନ୍ତୁ storage.get();
}



ସ୍ଥିର ତାର୍କିକ activated = ମିଥ୍ୟା;
ସ୍ଥିର ତାର୍କିକ isInitialized = ମିଥ୍ୟା;

TraceManager::TraceManager()
{
    (ଶୂନ୍ୟ)cv::getTimestampNS();

    isInitialized = ସତ୍ୟ;
    CV_LOG("TraceManager ctor: " << (ଶୂନ୍ୟ*)ଏହି);

    CV_LOG("TraceManager configure()");
    activated = getParameterTraceEnable();

    ଯଦି (activated)
        trace_storage.reset(ନୂତନ SyncTraceStorage(std::string(getParameterTraceLocation()) + ".txt"));

#ifdef OPENCV_WITH_ITT
    ଯଦି (isITTEnabled())
    {
        activated = ସତ୍ୟ; // force trace pipeline activation (without OpenCV storage)
        __itt_region_begin(domain, __itt_null, __itt_null, __itt_string_handle_create("OpenCVTrace"));
    }
#endif
}
TraceManager::~TraceManager()
{
    CV_LOG("TraceManager dtor: " << (ଶୂନ୍ୟ*)ଏହି);

#ifdef OPENCV_WITH_ITT
    ଯଦି (isITTEnabled())
    {
        __itt_region_end(domain, __itt_null);
    }
#endif

    std::vector<TraceManagerThreadLocal*> threads_ctx;
    tls.gather(threads_ctx);
    size_t totalEvents = 0, totalSkippedEvents = 0;
    ପାଇଁ (size_t i = 0; i < threads_ctx.size(); i++)
    {
        TraceManagerThreadLocal* ctx = threads_ctx[i];
        ଯଦି (ctx)
        {
            totalEvents += ctx->region_counter;
            totalSkippedEvents += ctx->totalSkippedEvents;
        }
    }
    ଯଦି (totalEvents || activated)
    {
        CV_LOG_INFO(ଖାଲି, "Trace: Total events: " << totalEvents);
    }
    ଯଦି (totalSkippedEvents)
    {
        CV_LOG_WARNING(ଖାଲି, "Trace: Total skipped events: " << totalSkippedEvents);
    }

    // This is a global static object, so process starts shutdown here
    // Turn off trace
    cv::__termination = ସତ୍ୟ; // also set in DllMain() notifications handler for DLL_PROCESS_DETACH
    activated = ମିଥ୍ୟା;
}

ତାର୍କିକ TraceManager::isActivated()
{
    // Check if process starts shutdown, and set earlyExit to true
    // to prevent further instrumentation processing earlier.
    ଯଦି (cv::__termination)
    {
        activated = ମିଥ୍ୟା;
        ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }

    ଯଦି (!isInitialized)
    {
        TraceManager& m = getTraceManager();
        CV_UNUSED(m); // TODO
    }

    ଫେରନ୍ତୁ activated;
}


ସ୍ଥିର TraceManager* getTraceManagerCallOnce()
{
    ସ୍ଥିର TraceManager globalInstance;
    ଫେରନ୍ତୁ &globalInstance;
}
TraceManager& getTraceManager()
{
    CV_SINGLETON_LAZY_INIT_REF(TraceManager, getTraceManagerCallOnce())
}

ଶୂନ୍ୟ parallel_For_Set_ମୂଳ_Region(ଅଚଳ Region& ମୂଳ_Region, ଅଚଳ TraceManagerThreadLocal& ମୂଳ_ctx)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();

    ଯଦି (ctx.dummy_ଗଦା_ଶୀର୍ଷ.region == &ମୂଳ_Region) // already attached
        ଫେରନ୍ତୁ;

    CV_Assert(ctx.dummy_ଗଦା_ଶୀର୍ଷ.region == ଖାଲି);
    ctx.dummy_ଗଦା_ଶୀର୍ଷ = TraceManagerThreadLocal::ଗଦା_Entry(const_cast<Region*>(&ମୂଳ_Region), ଖାଲି, -1);

    ଯଦି (&ctx == &ମୂଳ_ctx)
    {
        ctx.stat.grab(ctx.parallel_for_stat);
        ctx.parallel_for_stat_status = ctx.stat_status;
        ctx.parallel_for_ଗଦା_size = ctx.ଗଦା.size();
        ଫେରନ୍ତୁ;
    }

    CV_Assert(ctx.ଗଦା.empty());

    ctx.currentActiveRegion = const_cast<Region*>(&ମୂଳ_Region);

    ctx.regionDepth = ମୂଳ_ctx.regionDepth;
    ctx.regionDepthOpenCV = ମୂଳ_ctx.regionDepthOpenCV;

    ctx.parallel_for_ଗଦା_size = 0;

    ctx.stat_status.propagateFrom(ମୂଳ_ctx.stat_status);
}

ଶୂନ୍ୟ parallelForAttachNestedRegion(ଅଚଳ Region& ମୂଳ_Region)
{
    CV_UNUSED(ମୂଳ_Region);
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();

    CV_DbgAssert(ctx.dummy_ଗଦା_ଶୀର୍ଷ.region == &ମୂଳ_Region);

    Region* region = ctx.getCurrentActiveRegion();
    CV_LOG_PARALLEL(ଖାଲି, " PARALLEL_FOR: " << (ଶୂନ୍ୟ*)region << " ==> " << &ମୂଳ_Region);
    ଯଦି (!region)
        ଫେରନ୍ତୁ;

#ifdef OPENCV_WITH_ITT
    ଯଦି (!ମୂଳ_Region.pImpl || !ମୂଳ_Region.pImpl->itt_id_registered)
        ଫେରନ୍ତୁ;

    ଯଦି (!region->pImpl)
        ଫେରନ୍ତୁ;

    CV_LOG_PARALLEL(ଖାଲି, " PARALLEL_FOR ITT: " << (ଶୂନ୍ୟ*)ମୂଳ_Region.pImpl->itt_id.d1 << ":" << ମୂଳ_Region.pImpl->itt_id.d2 << ":" << (ଶୂନ୍ୟ*)ମୂଳ_Region.pImpl->itt_id.d3 << " => "
                                 << (ଶୂନ୍ୟ*)region->pImpl->itt_id.d1 << ":" << region->pImpl->itt_id.d2 << ":" << (ଶୂନ୍ୟ*)region->pImpl->itt_id.d3);
    __itt_relation_add(domain, region->pImpl->itt_id, __itt_relation_is_child_of, ମୂଳ_Region.pImpl->itt_id);
#endif
}

ଶୂନ୍ୟ parallelForFinalize(ଅଚଳ Region& ମୂଳ_Region)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();

    int64 endTimestamp = getTimestampNS();
    int64 duration = endTimestamp - ctx.ଗଦା_ଶୀର୍ଷ_Begin_Timestamp();
    CV_LOG_PARALLEL(ଖାଲି, "parallel_for duration: " << duration << " " << &ମୂଳ_Region);

    std::vector<TraceManagerThreadLocal*> threads_ctx;
    getTraceManager().tls.gather(threads_ctx);
    RegionStatistics parallel_for_stat;
    ପାଇଁ (size_t i = 0; i < threads_ctx.size(); i++)
    {
        TraceManagerThreadLocal* child_ctx = threads_ctx[i];

        ଯଦି (child_ctx && child_ctx->ଗଦା_ଶୀର୍ଷ_Region() == &ମୂଳ_Region)
        {
            CV_LOG_PARALLEL(ଖାଲି, "Thread=" << child_ctx->threadID << " " << child_ctx->stat);
            RegionStatistics child_stat;
            child_ctx->stat.grab(child_stat);
            parallel_for_stat.append(child_stat);
            ଯଦି (child_ctx != &ctx)
            {
                child_ctx->dummy_ଗଦା_ଶୀର୍ଷ = TraceManagerThreadLocal::ଗଦା_Entry();
            }
            ଅନ୍ୟଥା
            {
                ctx.parallel_for_stat.grab(ctx.stat);
                ctx.stat_status = ctx.parallel_for_stat_status;
                child_ctx->dummy_ଗଦା_ଶୀର୍ଷ = TraceManagerThreadLocal::ଗଦା_Entry();
            }
        }
    }

    ଦଶମିକ parallel_coeff = std::min(1.0f, duration / (ଦଶମିକ)(parallel_for_stat.duration));
    CV_LOG_PARALLEL(ଖାଲି, "parallel_coeff=" << 1.0f / parallel_coeff);
    CV_LOG_PARALLEL(ଖାଲି, parallel_for_stat);
    ଯଦି (parallel_coeff != 1.0f)
    {
        parallel_for_stat.multiply(parallel_coeff);
        CV_LOG_PARALLEL(ଖାଲି, parallel_for_stat);
    }
    parallel_for_stat.duration = 0;
    ctx.stat.append(parallel_for_stat);
    CV_LOG_PARALLEL(ଖାଲି, ctx.stat);
}

ସଂରଚନା TraceArg::ExtraData
{
#ifdef OPENCV_WITH_ITT
    // Special fields for ITT
    __itt_string_handle* ଅସ୍ଥିର ittHandle_name;
#endif
    ExtraData(TraceManagerThreadLocal& ctx, ଅଚଳ TraceArg& arg)
    {
        CV_UNUSED(ctx); CV_UNUSED(arg);
#ifdef OPENCV_WITH_ITT
        ଯଦି (isITTEnabled())
        {
            // Caching is not required here, because there is builtin cache.
            // https://software.intel.com/en-us/node/544203:
            //     Consecutive calls to __itt_string_handle_create with the same name return the same value.
            ittHandle_name = __itt_string_handle_create(arg.name);
        }
        ଅନ୍ୟଥା
        {
            ittHandle_name = 0;
        }
#endif
    }
};

ସ୍ଥିର ଶୂନ୍ୟ initTraceArg(TraceManagerThreadLocal& ctx, ଅଚଳ TraceArg& arg)
{
    TraceArg::ExtraData** pExtra = arg.ppExtra;
    ଯଦି (*pExtra == ଖାଲି)
    {
        cv::AutoLock lock(cv::getInitializationMutex());
        ଯଦି (*pExtra == ଖାଲି)
        {
            *pExtra = ନୂତନ TraceArg::ExtraData(ctx, arg);
        }
    }
}
ଶୂନ୍ୟ traceArg(ଅଚଳ TraceArg& arg, ଅଚଳ ଅକ୍ଷର* value)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    Region* region = ctx.getCurrentActiveRegion();
    ଯଦି (!region)
        ଫେରନ୍ତୁ;
    CV_Assert(region->pImpl);
    initTraceArg(ctx, arg);
    ଯଦି (!value)
        value = "<null>";
#ifdef OPENCV_WITH_ITT
    ଯଦି (isITTEnabled())
    {
        __itt_metadata_str_add(domain, region->pImpl->itt_id, (*arg.ppExtra)->ittHandle_name, value, strlen(value));
    }
#endif
}
ଶୂନ୍ୟ traceArg(ଅଚଳ TraceArg& arg, ପୂର୍ଣ୍ଣସଂଖ୍ୟା value)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    Region* region = ctx.getCurrentActiveRegion();
    ଯଦି (!region)
        ଫେରନ୍ତୁ;
    CV_Assert(region->pImpl);
    initTraceArg(ctx, arg);
#ifdef OPENCV_WITH_ITT
    ଯଦି (isITTEnabled())
    {
        __itt_metadata_add(domain, region->pImpl->itt_id, (*arg.ppExtra)->ittHandle_name, ଆକାର(ପୂର୍ଣ୍ଣସଂଖ୍ୟା) == 4 ? __itt_metadata_s32 : __itt_metadata_s64, 1, &value);
    }
#ଅନ୍ୟଥା
    CV_UNUSED(value);
#endif
}
ଶୂନ୍ୟ traceArg(ଅଚଳ TraceArg& arg, int64 value)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    Region* region = ctx.getCurrentActiveRegion();
    ଯଦି (!region)
        ଫେରନ୍ତୁ;
    CV_Assert(region->pImpl);
    initTraceArg(ctx, arg);
#ifdef OPENCV_WITH_ITT
    ଯଦି (isITTEnabled())
    {
        __itt_metadata_add(domain, region->pImpl->itt_id, (*arg.ppExtra)->ittHandle_name, __itt_metadata_s64, 1, &value);
    }
#ଅନ୍ୟଥା
    CV_UNUSED(value);
#endif
}
ଶୂନ୍ୟ traceArg(ଅଚଳ TraceArg& arg, ଦ୍ୱିଗୁଣ value)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    Region* region = ctx.getCurrentActiveRegion();
    ଯଦି (!region)
        ଫେରନ୍ତୁ;
    CV_Assert(region->pImpl);
    initTraceArg(ctx, arg);
#ifdef OPENCV_WITH_ITT
    ଯଦି (isITTEnabled())
    {
        __itt_metadata_add(domain, region->pImpl->itt_id, (*arg.ppExtra)->ittHandle_name, __itt_metadata_double, 1, &value);
    }
#ଅନ୍ୟଥା
    CV_UNUSED(value);
#endif
}

#ଅନ୍ୟଥା

Region::Region(ଅଚଳ LocationStaticStorage&) : pImpl(ଖାଲି), implFlags(0) {}
ଶୂନ୍ୟ Region::destroy() {}

ଶୂନ୍ୟ traceArg(ଅଚଳ TraceArg&, ଅଚଳ ଅକ୍ଷର*) {}
ଶୂନ୍ୟ traceArg(ଅଚଳ TraceArg&, ପୂର୍ଣ୍ଣସଂଖ୍ୟା) {};
ଶୂନ୍ୟ traceArg(ଅଚଳ TraceArg&, int64) {};
ଶୂନ୍ୟ traceArg(ଅଚଳ TraceArg&, ଦ୍ୱିଗୁଣ) {};

#endif

}}}} // namespace
