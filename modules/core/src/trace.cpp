// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ಅನ್ತರ್ಭಾವ "precomp.hpp"

#ಅನ್ತರ್ಭಾವ <opencv2/core/utils/trace.hpp>
#ಅನ್ತರ್ಭಾವ <opencv2/core/utils/trace.private.hpp>
#ಅನ್ತರ್ಭಾವ <opencv2/core/utils/configuration.private.hpp>

#ಅನ್ತರ್ಭಾವ <opencv2/core/opencl/ocl_defs.hpp>

#ಅನ್ತರ್ಭಾವ <cstdarg> // va_start

#ಅನ್ತರ್ಭಾವ <sstream>
#ಅನ್ತರ್ಭಾವ <ostream>
#ಅನ್ತರ್ಭಾವ <fstream>

#ಯದಿ 0
#define CV_LOG(...) CV_LOG_INFO(ರಿಕ್ತ, __VA_ARGS__)
#ಅನ್ಯಥಾ
#define CV_LOG(...) {}
#endif

#ಯದಿ 0
#define CV_LOG_ITT(...) CV_LOG_INFO(ರಿಕ್ತ, __VA_ARGS__)
#ಅನ್ಯಥಾ
#define CV_LOG_ITT(...) {}
#endif

#ಯದಿ 1
#define CV_LOG_TRACE_BAILOUT(tag, ...) CV_LOG_INFO(tag, __VA_ARGS__)
#ಅನ್ಯಥಾ
#define CV_LOG_TRACE_BAILOUT(...) {}
#endif

#ಯದಿ 0
#define CV_LOG_PARALLEL(tag, ...) CV_LOG_INFO(tag, __VA_ARGS__)
#ಅನ್ಯಥಾ
#define CV_LOG_PARALLEL(...) {}
#endif

#ಯದಿ 0
#define CV_LOG_CTX_STAT(tag, ...) CV_LOG_INFO(tag, __VA_ARGS__)
#ಅನ್ಯಥಾ
#define CV_LOG_CTX_STAT(...) {}
#endif

#ಯದಿ 0
#define CV_LOG_SKIP(tag, ...) CV_LOG_INFO(tag, __VA_ARGS__)
#ಅನ್ಯಥಾ
#define CV_LOG_SKIP(...) {}
#endif

ನಾಮಾಕಾಶ cv {
ನಾಮಾಕಾಶ utils {
ನಾಮಾಕಾಶ trace {
ನಾಮಾಕಾಶ details {

#ifdef OPENCV_TRACE

#ifdef _MSC_VER
#pragma warning(disable:4065) // switch statement contains 'default' but no 'case' labels
#endif

ಸ್ಥಿರ ತಾರ್ಕಿಕ getParameterTraceEnable()
{
    ಸ್ಥಿರ ತಾರ್ಕಿಕ param_traceEnable = utils::getConfigurationParameterBool("OPENCV_TRACE", ಅಸತ್ಯ);
    ಪ್ರತ್ಯಾಗಮ param_traceEnable;
}

// TODO lazy configuration flags
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ param_maxRegionDepthOpenCV = (ಪೂರ್ಣಾಙ್ಕ)utils::getConfigurationParameterSizeT("OPENCV_TRACE_DEPTH_OPENCV", 1);
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ param_maxRegionChildrenOpenCV = (ಪೂರ್ಣಾಙ್ಕ)utils::getConfigurationParameterSizeT("OPENCV_TRACE_MAX_CHILDREN_OPENCV", 1000);
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ param_maxRegionChildren = (ಪೂರ್ಣಾಙ್ಕ)utils::getConfigurationParameterSizeT("OPENCV_TRACE_MAX_CHILDREN", 10000);

ಸ್ಥಿರ ಧ್ರುವ cv::String& getParameterTraceLocation()
{
    ಸ್ಥಿರ cv::String param_traceLocation = utils::getConfigurationParameterString("OPENCV_TRACE_LOCATION", "OpenCVTrace");
    ಪ್ರತ್ಯಾಗಮ param_traceLocation;
}

#ifdef HAVE_OPENCL
ಸ್ಥಿರ ತಾರ್ಕಿಕ param_synchronizeOpenCL = utils::getConfigurationParameterBool("OPENCV_TRACE_SYNC_OPENCL", ಅಸತ್ಯ);
#endif

#ifdef OPENCV_WITH_ITT
ಸ್ಥಿರ ತಾರ್ಕಿಕ param_ITT_registerParentScope = utils::getConfigurationParameterBool("OPENCV_TRACE_ITT_PARENT", ಅಸತ್ಯ);
#endif

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ* _spaces(ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ_2)
{
    ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ buf[64] =
"                                                               ";
    ಪ್ರತ್ಯಾಗಮ &buf[63 - (ಎಣಿಕೆ_2 & 63)];
}

/**
 * Text-based trace messages
 */
ವರ್ಗ TraceMessage
{
ಸಾರ್ವಜನಿಕ:
    ವರ್ಣ buffer[1024];
    size_t len;
    ತಾರ್ಕಿಕ hasError;

    TraceMessage() :
        len(0),
        hasError(ಅಸತ್ಯ)
    {}

    ತಾರ್ಕಿಕ printf(ಧ್ರುವ ವರ್ಣ* format, ...)
    {
        ವರ್ಣ* buf = &buffer[len];
        size_t sz = ಪರಿಮಾಣ(buffer) - len;
        va_list ap;
        va_start(ap, format);
        ಪೂರ್ಣಾಙ್ಕ n = cv_vsnprintf(buf, (ಪೂರ್ಣಾಙ್ಕ)sz, format, ap);
        va_end(ap);
        ಯದಿ (n < 0 || (size_t)n > sz)
        {
            hasError = ಸತ್ಯ;
            ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
        }
        len += n;
        ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    }

    ತಾರ್ಕಿಕ formatlocation(ಧ್ರುವ Region::LocationStaticStorage& location)
    {
        ಪ್ರತ್ಯಾಗಮ ಏತತ್->printf("l,%lld,\"%s\",%d,\"%s\",0x%llX\n",
                (long long ಪೂರ್ಣಾಙ್ಕ)(*location.ppExtra)->global_location_id,
                location.filename,
                location.ಸಾಲು,
                location.name,
                (long long ಪೂರ್ಣಾಙ್ಕ)(location.flags & ~0xF0000000));
    }
    ತಾರ್ಕಿಕ formatRegionEnter(ಧ್ರುವ Region& region)
    {
        ತಾರ್ಕಿಕ ok = ಏತತ್->printf("b,%d,%lld,%lld,%lld",
                (ಪೂರ್ಣಾಙ್ಕ)region.pImpl->threadID,
                (long long ಪೂರ್ಣಾಙ್ಕ)region.pImpl->beginTimestamp,
                (long long ಪೂರ್ಣಾಙ್ಕ)((*region.pImpl->location.ppExtra)->global_location_id),
                (long long ಪೂರ್ಣಾಙ್ಕ)region.pImpl->global_region_id);
        ಯದಿ (region.pImpl->parentRegion && region.pImpl->parentRegion->pImpl)
        {
            ಯದಿ (region.pImpl->parentRegion->pImpl->threadID != region.pImpl->threadID)
                ok &= ಏತತ್->printf(",parentThread=%d,parent=%lld",
                        (ಪೂರ್ಣಾಙ್ಕ)region.pImpl->parentRegion->pImpl->threadID,
                        (long long ಪೂರ್ಣಾಙ್ಕ)region.pImpl->parentRegion->pImpl->global_region_id);
        }
        ok &= ಏತತ್->printf("\n");
        ಪ್ರತ್ಯಾಗಮ ok;
    }
    ತಾರ್ಕಿಕ formatRegionLeave(ಧ್ರುವ Region& region, ಧ್ರುವ RegionStatistics& result)
    {
        CV_DbgAssert(region.pImpl->endTimestamp - region.pImpl->beginTimestamp == result.duration);
        ತಾರ್ಕಿಕ ok = ಏತತ್->printf("e,%d,%lld,%lld,%lld,%lld",
                (ಪೂರ್ಣಾಙ್ಕ)region.pImpl->threadID,
                (long long ಪೂರ್ಣಾಙ್ಕ)region.pImpl->endTimestamp,
                (long long ಪೂರ್ಣಾಙ್ಕ)(*region.pImpl->location.ppExtra)->global_location_id,
                (long long ಪೂರ್ಣಾಙ್ಕ)region.pImpl->global_region_id,
                (long long ಪೂರ್ಣಾಙ್ಕ)result.duration);
        ಯದಿ (result.currentSkippedRegions)
            ok &= ಏತತ್->printf(",skip=%d", (ಪೂರ್ಣಾಙ್ಕ)result.currentSkippedRegions);
#ifdef HAVE_IPP
        ಯದಿ (result.durationImplIPP)
            ok &= ಏತತ್->printf(",tIPP=%lld", (long long ಪೂರ್ಣಾಙ್ಕ)result.durationImplIPP);
#endif
#ifdef HAVE_OPENCL
        ಯದಿ (result.durationImplOpenCL)
            ok &= ಏತತ್->printf(",tOCL=%lld", (long long ಪೂರ್ಣಾಙ್ಕ)result.durationImplOpenCL);
#endif
        ok &= ಏತತ್->printf("\n");
        ಪ್ರತ್ಯಾಗಮ ok;
    }
    ತಾರ್ಕಿಕ recordRegionArg(ಧ್ರುವ Region& region, ಧ್ರುವ TraceArg& arg, ಧ್ರುವ ವರ್ಣ* value)
    {
        ಪ್ರತ್ಯಾಗಮ ಏತತ್->printf("a,%d,%lld,%lld,\"%s\",\"%s\"\n",
                region.pImpl->threadID,
                (long long ಪೂರ್ಣಾಙ್ಕ)region.pImpl->beginTimestamp,
                (long long ಪೂರ್ಣಾಙ್ಕ)region.pImpl->global_region_id,
                arg.name,
                value);
    }
};


#ifdef OPENCV_WITH_ITT
ಸ್ಥಿರ __itt_domain* domain = ರಿಕ್ತ;

ಸ್ಥಿರ ತಾರ್ಕಿಕ isITTEnabled()
{
    ಸ್ಥಿರ ಚಞ್ಚಲ ತಾರ್ಕಿಕ isInitialized = ಅಸತ್ಯ;
    ಸ್ಥಿರ ತಾರ್ಕಿಕ isEnabled = ಅಸತ್ಯ;
    ಯದಿ (!isInitialized)
    {
        cv::AutoLock lock(cv::getInitializationMutex());
        ಯದಿ (!isInitialized)
        {
            ತಾರ್ಕಿಕ param_traceITTEnable = utils::getConfigurationParameterBool("OPENCV_TRACE_ITT_ENABLE", ಸತ್ಯ);
            ಯದಿ (param_traceITTEnable)
            {
                isEnabled = !!(__itt_api_version());
                CV_LOG_ITT("ITT is " << (isEnabled ? "enabled" : "disabled"));
                domain = __itt_domain_create("OpenCVTrace");
            }
            ಅನ್ಯಥಾ
            {
                CV_LOG_ITT("ITT is disabled through OpenCV parameter");
                isEnabled = ಅಸತ್ಯ;
            }
            isInitialized = ಸತ್ಯ;
        }
    }
    ಪ್ರತ್ಯಾಗಮ isEnabled;
}
#endif


Region::LocationExtraData::LocationExtraData(ಧ್ರುವ LocationStaticStorage& location)
{
    CV_UNUSED(location);
    ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ g_location_id_counter = 0;
    global_location_id = CV_XADD(&g_location_id_counter, 1) + 1;
    CV_LOG("Register location: " << global_location_id << " (" << (ಶೂನ್ಯ*)&location << ")"
            << std::endl << "    file: " << location.filename
            << std::endl << "    line: " << location.ಸಾಲು
            << std::endl << "    name: " << location.name);
#ifdef OPENCV_WITH_ITT
    ಯದಿ (isITTEnabled())
    {
        // Caching is not required here, because there is builtin cache.
        // https://software.intel.com/en-us/node/544203:
        //     Consecutive calls to __itt_string_handle_create with the same name return the same value.
        ittHandle_name = __itt_string_handle_create(location.name);
        ittHandle_filename = __itt_string_handle_create(location.filename);
    }
    ಅನ್ಯಥಾ
    {
        ittHandle_name = 0;
        ittHandle_filename = 0;
    }
#endif
}

/*static*/ Region::LocationExtraData* Region::LocationExtraData::init(ಧ್ರುವ Region::LocationStaticStorage& location)
{
    LocationExtraData** pLocationExtra = location.ppExtra;
    CV_DbgAssert(pLocationExtra);
    ಯದಿ (*pLocationExtra == ರಿಕ್ತ)
    {
        cv::AutoLock lock(cv::getInitializationMutex());
        ಯದಿ (*pLocationExtra == ರಿಕ್ತ)
        {
            *pLocationExtra = ನೂತನ Region::LocationExtraData(location);
            TraceStorage* s = getTraceManager().trace_storage.get();
            ಯದಿ (s)
            {
                TraceMessage ಸಂದೇಶ;
                ಸಂದೇಶ.formatlocation(location);
                s->put(ಸಂದೇಶ);
            }
        }
    }
    ಪ್ರತ್ಯಾಗಮ *pLocationExtra;
}


Region::Impl::Impl(TraceManagerThreadLocal& ctx, Region* parentRegion_, Region& region_, ಧ್ರುವ LocationStaticStorage& location_, int64 beginTimestamp_) :
    location(location_),
    region(region_),
    parentRegion(parentRegion_),
    threadID(ctx.threadID),
    global_region_id(++ctx.region_counter),
    beginTimestamp(beginTimestamp_),
    endTimestamp(0),
    direct_Children_ಎಣಿಕೆ(0)
#ifdef OPENCV_WITH_ITT
    ,itt_id_registered(ಅಸತ್ಯ)
    ,itt_id(__itt_null)
#endif
{
    CV_DbgAssert(ctx.currentActiveRegion == parentRegion);
    region.pImpl = ಏತತ್;

    registerRegion(ctx);

    enterRegion(ctx);
}

Region::Impl::~Impl()
{
#ifdef OPENCV_WITH_ITT
    ಯದಿ (itt_id_registered)
    {
        CV_LOG_ITT(" Destroy ITT region: I=" << (ಶೂನ್ಯ*)ಏತತ್);
        __itt_id_destroy(domain, itt_id);
        itt_id_registered = ಅಸತ್ಯ;
    }
#endif
    region.pImpl = ರಿಕ್ತ;
}

ಶೂನ್ಯ Region::Impl::enterRegion(TraceManagerThreadLocal& ctx)
{
    ctx.currentActiveRegion = &region;

    ಯದಿ (location.flags & REGION_FLAG_FUNCTION)
    {
        ಯದಿ ((location.flags & REGION_FLAG_APP_CODE) == 0)
        {
            ctx.regionDepthOpenCV++;
        }
        ctx.regionDepth++;
    }

    TraceStorage* s = ctx.getStorage();
    ಯದಿ (s)
    {
        TraceMessage ಸಂದೇಶ;
        ಸಂದೇಶ.formatRegionEnter(region);
        s->put(ಸಂದೇಶ);
    }
#ifdef OPENCV_WITH_ITT
    ಯದಿ (isITTEnabled())
    {
        __itt_id parentID = __itt_null;
        ಯದಿ (param_ITT_registerParentScope && parentRegion && parentRegion->pImpl && parentRegion->pImpl->itt_id_registered && (location.flags & REGION_FLAG_REGION_FORCE) == 0)
            parentID = parentRegion->pImpl->itt_id;
        __itt_task_begin(domain, itt_id, parentID, (*location.ppExtra)->ittHandle_name);
    }
#endif
}

ಶೂನ್ಯ Region::Impl::leaveRegion(TraceManagerThreadLocal& ctx)
{
    int64 duration = endTimestamp - beginTimestamp; CV_UNUSED(duration);
    RegionStatistics result;
    ctx.stat.grab(result);
    ctx.totalSkippedEvents += result.currentSkippedRegions;
    CV_LOG(_spaces(ctx.getCurrentDepth()*4) << "leaveRegion(): " << (ಶೂನ್ಯ*)ಏತತ್ << " " << result);
#ifdef OPENCV_WITH_ITT
    ಯದಿ (isITTEnabled())
    {
        ಯದಿ (result.currentSkippedRegions)
        {
            __itt_metadata_add(domain, itt_id, __itt_string_handle_create("skipped trace entries"), __itt_metadata_u32, 1, &result.currentSkippedRegions);
        }
#ifdef HAVE_IPP
        ಯದಿ (result.durationImplIPP)
            __itt_metadata_add(domain, itt_id, __itt_string_handle_create("tIPP"), __itt_metadata_u64, 1, &result.durationImplIPP);
#endif
#ifdef HAVE_OPENCL
        ಯದಿ (result.durationImplOpenCL)
            __itt_metadata_add(domain, itt_id, __itt_string_handle_create("tOpenCL"), __itt_metadata_u64, 1, &result.durationImplOpenCL);
#endif
        __itt_task_end(domain);
    }
#endif
    TraceStorage* s = ctx.getStorage();
    ಯದಿ (s)
    {
        TraceMessage ಸಂದೇಶ;
        ಸಂದೇಶ.formatRegionLeave(region, result);
        s->put(ಸಂದೇಶ);
    }

    ಯದಿ (location.flags & REGION_FLAG_FUNCTION)
    {
        ಯದಿ ((location.flags & REGION_FLAG_APP_CODE) == 0)
        {
            ctx.regionDepthOpenCV--;
        }
        ctx.regionDepth--;
    }

    ctx.currentActiveRegion = parentRegion;
}

ಶೂನ್ಯ Region::Impl::release()
{
    delete ಏತತ್;
}

ಶೂನ್ಯ Region::Impl::registerRegion(TraceManagerThreadLocal& ctx)
{
#ifdef OPENCV_WITH_ITT
    ಯದಿ (isITTEnabled())
    {
        ಯದಿ (!itt_id_registered)
        {
            CV_LOG_ITT(" Register ITT region: I=" << (ಶೂನ್ಯ*)ಏತತ್ << " " << ctx.threadID << "-" << global_region_id);
#ಯದಿ 1 // workaround for some ITT backends
            itt_id = __itt_id_make((ಶೂನ್ಯ*)(intptr_t)(((int64)(ctx.threadID + 1) << 32) | global_region_id), global_region_id);
#ಅನ್ಯಥಾ
            itt_id = __itt_id_make((ಶೂನ್ಯ*)(intptr_t)(ctx.threadID + 1), global_region_id);
#endif
            __itt_id_create(domain, itt_id);
            itt_id_registered = ಸತ್ಯ;
        }
    }
#ಅನ್ಯಥಾ
    CV_UNUSED(ctx);
#endif
}

ಶೂನ್ಯ RegionStatisticsStatus::enableSkipMode(ಪೂರ್ಣಾಙ್ಕ depth)
{
    CV_DbgAssert(_skipDepth < 0);
    CV_LOG_SKIP(ರಿಕ್ತ, "SKIP-ENABLE: depth=" << depth);
    _skipDepth = depth;
}
ಶೂನ್ಯ RegionStatisticsStatus::checkResetSkipMode(ಪೂರ್ಣಾಙ್ಕ leaveDepth)
{
    ಯದಿ (leaveDepth <= _skipDepth)
    {
        CV_LOG_SKIP(ರಿಕ್ತ, "SKIP-RESET: leaveDepth=" << leaveDepth << " skipDepth=" << _skipDepth);
        _skipDepth = -1;
    }
}

Region::Region(ಧ್ರುವ LocationStaticStorage& location) :
    pImpl(ರಿಕ್ತ),
    implFlags(0)
{
    // Checks:
    // - global enable flag
    // - parent region is disabled
    // - children count threshold
    // - region location
    // - depth (opencv nested calls)
    ಯದಿ (!TraceManager::isActivated())
    {
        CV_LOG("Trace is disabled. Bailout");
        ಪ್ರತ್ಯಾಗಮ;
    }

    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    CV_LOG(_spaces(ctx.getCurrentDepth()*4) << "Region(): " << (ಶೂನ್ಯ*)ಏತತ್ << ": " << location.name);

    Region* parentRegion = ctx.ರಾಶಿ_ತುದಿ_Region();
    ಧ್ರುವ Region::LocationStaticStorage* parentLocation = ctx.ರಾಶಿ_ತುದಿ_Location();

    ಯದಿ (location.flags & REGION_FLAG_REGION_NEXT)
    {
        ಯದಿ (parentRegion && parentRegion->pImpl)
        {
            CV_DbgAssert((parentRegion->pImpl->location.flags & REGION_FLAG_FUNCTION) == 0);
            parentRegion->destroy(); parentRegion->implFlags = 0;
            parentRegion = ctx.ರಾಶಿ_ತುದಿ_Region();
            parentLocation = ctx.ರಾಶಿ_ತುದಿ_Location();
        }
    }

    ಪೂರ್ಣಾಙ್ಕ parentChildren = 0;
    ಯದಿ (parentRegion && parentRegion->pImpl)
    {
        ಯದಿ (parentLocation == ರಿಕ್ತ)
        {
            // parallel_for_body code path
            parentChildren = CV_XADD(&parentRegion->pImpl->direct_Children_ಎಣಿಕೆ, 1) + 1;
        }
        ಅನ್ಯಥಾ
        {
            parentChildren = ++parentRegion->pImpl->direct_Children_ಎಣಿಕೆ;
        }
    }

    int64 beginTimestamp = getTimestampNS();

    ಪೂರ್ಣಾಙ್ಕ currentDepth = ctx.getCurrentDepth() + 1;
    ವಿಕಲ್ಪ (location.flags & REGION_FLAG_IMPL_MASK)
    {
#ifdef HAVE_IPP
    ಪಕ್ಷ REGION_FLAG_IMPL_IPP:
        ಯದಿ (!ctx.stat_status.ignoreDepthImplIPP)
            ctx.stat_status.ignoreDepthImplIPP = currentDepth;
        ಭಙ್ಗ;
#endif
#ifdef HAVE_OPENCL
    ಪಕ್ಷ REGION_FLAG_IMPL_OPENCL:
        ಯದಿ (!ctx.stat_status.ignoreDepthImplOpenCL)
            ctx.stat_status.ignoreDepthImplOpenCL = currentDepth;
        ಭಙ್ಗ;
#endif
    ಸ್ವಭಾವ:
        ಭಙ್ಗ;
    }

    ctx.ರಾಶಿ_ತಳ್ಳು(ಏತತ್, &location, beginTimestamp);
    implFlags |= REGION_FLAG_NEED_ರಾಶಿ_ತೆಗೆ;

    ಯದಿ ((location.flags & REGION_FLAG_REGION_FORCE) == 0)
    {
        ಯದಿ (ctx.stat_status._skipDepth >= 0 && currentDepth > ctx.stat_status._skipDepth)
        {
            CV_LOG(_spaces(ctx.getCurrentDepth()*4) << "Parent region is disabled. Bailout");
            ctx.stat.currentSkippedRegions++;
            ಪ್ರತ್ಯಾಗಮ;
        }

        ಯದಿ (param_maxRegionChildrenOpenCV > 0 && (location.flags & REGION_FLAG_APP_CODE) == 0 && parentLocation && (parentLocation->flags & REGION_FLAG_APP_CODE) == 0)
        {
            ಯದಿ (parentChildren >= param_maxRegionChildrenOpenCV)
            {
                CV_LOG_TRACE_BAILOUT(ರಿಕ್ತ, _spaces(ctx.getCurrentDepth()*4) << "OpenCV parent region exceeds children count. Bailout");
                ctx.stat_status.enableSkipMode(currentDepth - 1);
                ctx.stat.currentSkippedRegions++;
                DEBUG_ONLY(ctx.dump_ರಾಶಿ(std::cout, ಅಸತ್ಯ));
                ಪ್ರತ್ಯಾಗಮ;
            }
        }
        ಯದಿ (param_maxRegionChildren > 0 && parentChildren >= param_maxRegionChildren)
        {
            CV_LOG_TRACE_BAILOUT(ರಿಕ್ತ, _spaces(ctx.getCurrentDepth()*4) << "Parent region exceeds children count. Bailout");
            ctx.stat_status.enableSkipMode(currentDepth - 1);
            ctx.stat.currentSkippedRegions++;
            DEBUG_ONLY(ctx.dump_ರಾಶಿ(std::cout, ಅಸತ್ಯ));
            ಪ್ರತ್ಯಾಗಮ;
        }
    }

    LocationExtraData::init(location);

    ಯದಿ ((*location.ppExtra)->global_location_id == 0)
    {
        CV_LOG_TRACE_BAILOUT(ರಿಕ್ತ, _spaces(ctx.getCurrentDepth()*4) << "Region location is disabled. Bailout");
        ctx.stat_status.enableSkipMode(currentDepth);
        ctx.stat.currentSkippedRegions++;
        ಪ್ರತ್ಯಾಗಮ;
    }

    ಯದಿ (parentLocation && (parentLocation->flags & REGION_FLAG_SKIP_NESTED))
    {
        CV_LOG(_spaces(ctx.getCurrentDepth()*4) << "Parent region disables inner regions. Bailout");
        ctx.stat_status.enableSkipMode(currentDepth);
        ctx.stat.currentSkippedRegions++;
        ಪ್ರತ್ಯಾಗಮ;
    }

    ಯದಿ (param_maxRegionDepthOpenCV)
    {
        ಯದಿ ((location.flags & REGION_FLAG_APP_CODE) == 0)
        {
            ಯದಿ (ctx.regionDepthOpenCV >= param_maxRegionDepthOpenCV)
            {
                CV_LOG(_spaces(ctx.getCurrentDepth()*4) << "OpenCV region depth is exceed = " << ctx.regionDepthOpenCV << ". Bailout");
                ಯದಿ (ctx.stat.currentSkippedRegions == 0)
                {
                    DEBUG_ONLY(ctx.dump_ರಾಶಿ(std::cout, ಅಸತ್ಯ));
                }
                ctx.stat_status.enableSkipMode(currentDepth);
                ctx.stat.currentSkippedRegions++;
                ಪ್ರತ್ಯಾಗಮ;
            }
        }
    }

    ನೂತನ Impl(ctx, parentRegion, *ಏತತ್, location, beginTimestamp);
    CV_DbgAssert(pImpl != ರಿಕ್ತ);
    implFlags |= REGION_FLAG__ACTIVE;

    // parallel_for path
    ಯದಿ (parentRegion && parentRegion->pImpl)
    {
        ಯದಿ (parentLocation == ರಿಕ್ತ)
        {
            pImpl->direct_Children_ಎಣಿಕೆ = parentChildren;
        }
    }
}

ಶೂನ್ಯ Region::destroy()
{
    CV_DbgAssert(implFlags != 0);

    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    CV_LOG(_spaces(ctx.getCurrentDepth()*4) << "Region::destruct(): " << (ಶೂನ್ಯ*)ಏತತ್ << " pImpl=" << pImpl << " implFlags=" << implFlags << ' ' << (ctx.ರಾಶಿ_ತುದಿ_Location() ? ctx.ರಾಶಿ_ತುದಿ_Location()->name : "<unknown>"));

    CV_DbgAssert(implFlags & REGION_FLAG_NEED_ರಾಶಿ_ತೆಗೆ);
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ currentDepth = ctx.getCurrentDepth(); CV_UNUSED(currentDepth);

    CV_LOG_CTX_STAT(ರಿಕ್ತ, _spaces(currentDepth*4) << ctx.stat << ' ' << ctx.stat_status);

    ಧ್ರುವ Region::LocationStaticStorage* location = ctx.ರಾಶಿ_ತುದಿ_Location();
    Impl::OptimizationPath myCodePath = Impl::CODE_PATH_PLAIN;
    ಯದಿ (location)
    {
        ವಿಕಲ್ಪ (location->flags & REGION_FLAG_IMPL_MASK)
        {
#ifdef HAVE_IPP
        ಪಕ್ಷ REGION_FLAG_IMPL_IPP:
            myCodePath = Impl::CODE_PATH_IPP;
            ಭಙ್ಗ;
#endif
#ifdef HAVE_OPENCL
        ಪಕ್ಷ REGION_FLAG_IMPL_OPENCL:
            ಯದಿ (param_synchronizeOpenCL && cv::ocl::isOpenCLActivated())
                cv::ocl::finish();
            myCodePath = Impl::CODE_PATH_OPENCL;
            ಭಙ್ಗ;
#endif
        ಸ್ವಭಾವ:
            ಭಙ್ಗ;
        }
    }

    int64 endTimestamp = getTimestampNS();
    int64 duration = endTimestamp - ctx.ರಾಶಿ_ತುದಿ_Begin_Timestamp();

    ತಾರ್ಕಿಕ active = isActive();

    ಯದಿ (active)
        ctx.stat.duration = duration;
    ಅನ್ಯಥಾ ಯದಿ (ctx.ರಾಶಿ.size() == ctx.parallel_for_ರಾಶಿ_size + 1)
        ctx.stat.duration += duration;

    ವಿಕಲ್ಪ (myCodePath) {
        ಪಕ್ಷ Impl::CODE_PATH_PLAIN:
            // nothing
            ಭಙ್ಗ;
#ifdef HAVE_IPP
        ಪಕ್ಷ Impl::CODE_PATH_IPP:
            ಯದಿ (ctx.stat_status.ignoreDepthImplIPP == currentDepth)
            {
                ctx.stat.durationImplIPP += duration;
                ctx.stat_status.ignoreDepthImplIPP = 0;
            }
            ಅನ್ಯಥಾ ಯದಿ (active)
            {
                ctx.stat.durationImplIPP = duration;
            }
            ಭಙ್ಗ;
#endif
#ifdef HAVE_OPENCL
        ಪಕ್ಷ Impl::CODE_PATH_OPENCL:
            ಯದಿ (ctx.stat_status.ignoreDepthImplOpenCL == currentDepth)
            {
                ctx.stat.durationImplOpenCL += duration;
                ctx.stat_status.ignoreDepthImplOpenCL = 0;
            }
            ಅನ್ಯಥಾ ಯದಿ (active)
            {
                ctx.stat.durationImplOpenCL = duration;
            }
            ಭಙ್ಗ;
#endif
        ಸ್ವಭಾವ:
            ಭಙ್ಗ;
    }

    ಯದಿ (pImpl)
    {
        CV_DbgAssert((implFlags & (REGION_FLAG__ACTIVE | REGION_FLAG_NEED_ರಾಶಿ_ತೆಗೆ)) == (REGION_FLAG__ACTIVE | REGION_FLAG_NEED_ರಾಶಿ_ತೆಗೆ));
        CV_DbgAssert(ctx.ರಾಶಿ_ತುದಿ_Region() == ಏತತ್);
        pImpl->endTimestamp = endTimestamp;
        pImpl->leaveRegion(ctx);
        pImpl->release();
        pImpl = ರಿಕ್ತ;
        DEBUG_ONLY(implFlags &= ~REGION_FLAG__ACTIVE);
    }
    ಅನ್ಯಥಾ
    {
        CV_DbgAssert(ctx.stat_status._skipDepth <= currentDepth);
    }

    ಯದಿ (implFlags & REGION_FLAG_NEED_ರಾಶಿ_ತೆಗೆ)
    {
        CV_DbgAssert(ctx.ರಾಶಿ_ತುದಿ_Region() == ಏತತ್);
        ctx.ರಾಶಿ_ತೆಗೆ();
        ctx.stat_status.checkResetSkipMode(currentDepth);
        DEBUG_ONLY(implFlags &= ~REGION_FLAG_NEED_ರಾಶಿ_ತೆಗೆ);
    }
    CV_LOG_CTX_STAT(ರಿಕ್ತ, _spaces(currentDepth*4) << "===> " << ctx.stat << ' ' << ctx.stat_status);
}


TraceManagerThreadLocal::~TraceManagerThreadLocal()
{
}

ಶೂನ್ಯ TraceManagerThreadLocal::dump_ರಾಶಿ(std::ostream& out, ತಾರ್ಕಿಕ onlyFunctions) ಧ್ರುವ
{
    std::stringstream ss;
    std::deque<ರಾಶಿ_Entry>::const_iterator it = ರಾಶಿ.begin();
    std::deque<ರಾಶಿ_Entry>::const_iterator end = ರಾಶಿ.end();
    ಪೂರ್ಣಾಙ್ಕ depth = 0;
    ಪ್ರತಿ (; it != end; ++it)
    {
        ಧ್ರುವ Region::LocationStaticStorage* location = it->location;
        ಯದಿ (location)
        {
            ಯದಿ (!onlyFunctions || (location->flags & REGION_FLAG_FUNCTION))
            {
                ss << _spaces(4*depth) << location->name << std::endl;
                depth++;
            }
        }
        ಅನ್ಯಥಾ
        {
            ss << _spaces(4*depth) << "<unknown>" << std::endl;
            depth++;
        }
    }
    out << ss.str();
}

ವರ್ಗ AsyncTraceStorage CV_FINAL : ಸಾರ್ವಜನಿಕ TraceStorage
{
    mutable std::ofstream out;
ಸಾರ್ವಜನಿಕ:
    ಧ್ರುವ std::string name;

    AsyncTraceStorage(ಧ್ರುವ std::string& filename) :
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

    ತಾರ್ಕಿಕ put(ಧ್ರುವ TraceMessage& ಸಂದೇಶ) ಧ್ರುವ CV_OVERRIDE
    {
        ಯದಿ (ಸಂದೇಶ.hasError)
            ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
        out << ಸಂದೇಶ.buffer;
        //DEBUG_ONLY(std::flush(out)); // TODO configure flag
        ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    }
};

ವರ್ಗ SyncTraceStorage CV_FINAL : ಸಾರ್ವಜನಿಕ TraceStorage
{
    mutable std::ofstream out;
    mutable cv::Mutex mutex;
ಸಾರ್ವಜನಿಕ:
    ಧ್ರುವ std::string name;

    SyncTraceStorage(ಧ್ರುವ std::string& filename) :
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

    ತಾರ್ಕಿಕ put(ಧ್ರುವ TraceMessage& ಸಂದೇಶ) ಧ್ರುವ CV_OVERRIDE
    {
        ಯದಿ (ಸಂದೇಶ.hasError)
            ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
        {
            cv::AutoLock l(mutex);
            out << ಸಂದೇಶ.buffer;
            std::flush(out); // TODO configure flag
        }
        ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    }
};


TraceStorage* TraceManagerThreadLocal::getStorage() ಧ್ರುವ
{
    // TODO configuration option for stdout/single trace file
    ಯದಿ (storage.empty())
    {
        TraceStorage* global = getTraceManager().trace_storage.get();
        ಯದಿ (global)
        {
            ಧ್ರುವ std::string filepath = cv::format("%s-%03d.txt", getParameterTraceLocation().c_str(), threadID).c_str();
            TraceMessage ಸಂದೇಶ;
            ಧ್ರುವ ವರ್ಣ* pos = strrchr(filepath.c_str(), '/'); // extract filename
#ifdef _WIN32
            ಯದಿ (!pos)
                pos = strrchr(filepath.c_str(), '\\');
#endif
            ಯದಿ (!pos)
                pos = filepath.c_str();
            ಅನ್ಯಥಾ
                pos += 1; // fix to skip extra slash in filename beginning
            ಸಂದೇಶ.printf("#thread file: %s\n", pos);
            global->put(ಸಂದೇಶ);
            storage.reset(ನೂತನ AsyncTraceStorage(filepath));
        }
    }
    ಪ್ರತ್ಯಾಗಮ storage.get();
}



ಸ್ಥಿರ ತಾರ್ಕಿಕ activated = ಅಸತ್ಯ;
ಸ್ಥಿರ ತಾರ್ಕಿಕ isInitialized = ಅಸತ್ಯ;

TraceManager::TraceManager()
{
    (ಶೂನ್ಯ)cv::getTimestampNS();

    isInitialized = ಸತ್ಯ;
    CV_LOG("TraceManager ctor: " << (ಶೂನ್ಯ*)ಏತತ್);

    CV_LOG("TraceManager configure()");
    activated = getParameterTraceEnable();

    ಯದಿ (activated)
        trace_storage.reset(ನೂತನ SyncTraceStorage(std::string(getParameterTraceLocation()) + ".txt"));

#ifdef OPENCV_WITH_ITT
    ಯದಿ (isITTEnabled())
    {
        activated = ಸತ್ಯ; // force trace pipeline activation (without OpenCV storage)
        __itt_region_begin(domain, __itt_null, __itt_null, __itt_string_handle_create("OpenCVTrace"));
    }
#endif
}
TraceManager::~TraceManager()
{
    CV_LOG("TraceManager dtor: " << (ಶೂನ್ಯ*)ಏತತ್);

#ifdef OPENCV_WITH_ITT
    ಯದಿ (isITTEnabled())
    {
        __itt_region_end(domain, __itt_null);
    }
#endif

    std::vector<TraceManagerThreadLocal*> threads_ctx;
    tls.gather(threads_ctx);
    size_t totalEvents = 0, totalSkippedEvents = 0;
    ಪ್ರತಿ (size_t i = 0; i < threads_ctx.size(); i++)
    {
        TraceManagerThreadLocal* ctx = threads_ctx[i];
        ಯದಿ (ctx)
        {
            totalEvents += ctx->region_counter;
            totalSkippedEvents += ctx->totalSkippedEvents;
        }
    }
    ಯದಿ (totalEvents || activated)
    {
        CV_LOG_INFO(ರಿಕ್ತ, "Trace: Total events: " << totalEvents);
    }
    ಯದಿ (totalSkippedEvents)
    {
        CV_LOG_WARNING(ರಿಕ್ತ, "Trace: Total skipped events: " << totalSkippedEvents);
    }

    // This is a global static object, so process starts shutdown here
    // Turn off trace
    cv::__termination = ಸತ್ಯ; // also set in DllMain() notifications handler for DLL_PROCESS_DETACH
    activated = ಅಸತ್ಯ;
}

ತಾರ್ಕಿಕ TraceManager::isActivated()
{
    // Check if process starts shutdown, and set earlyExit to true
    // to prevent further instrumentation processing earlier.
    ಯದಿ (cv::__termination)
    {
        activated = ಅಸತ್ಯ;
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }

    ಯದಿ (!isInitialized)
    {
        TraceManager& m = getTraceManager();
        CV_UNUSED(m); // TODO
    }

    ಪ್ರತ್ಯಾಗಮ activated;
}


ಸ್ಥಿರ TraceManager* getTraceManagerCallOnce()
{
    ಸ್ಥಿರ TraceManager globalInstance;
    ಪ್ರತ್ಯಾಗಮ &globalInstance;
}
TraceManager& getTraceManager()
{
    CV_SINGLETON_LAZY_INIT_REF(TraceManager, getTraceManagerCallOnce())
}

ಶೂನ್ಯ parallel_For_Set_ಬೇರು_Region(ಧ್ರುವ Region& ಬೇರು_Region, ಧ್ರುವ TraceManagerThreadLocal& ಬೇರು_ctx)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();

    ಯದಿ (ctx.dummy_ರಾಶಿ_ತುದಿ.region == &ಬೇರು_Region) // already attached
        ಪ್ರತ್ಯಾಗಮ;

    CV_Assert(ctx.dummy_ರಾಶಿ_ತುದಿ.region == ರಿಕ್ತ);
    ctx.dummy_ರಾಶಿ_ತುದಿ = TraceManagerThreadLocal::ರಾಶಿ_Entry(const_cast<Region*>(&ಬೇರು_Region), ರಿಕ್ತ, -1);

    ಯದಿ (&ctx == &ಬೇರು_ctx)
    {
        ctx.stat.grab(ctx.parallel_for_stat);
        ctx.parallel_for_stat_status = ctx.stat_status;
        ctx.parallel_for_ರಾಶಿ_size = ctx.ರಾಶಿ.size();
        ಪ್ರತ್ಯಾಗಮ;
    }

    CV_Assert(ctx.ರಾಶಿ.empty());

    ctx.currentActiveRegion = const_cast<Region*>(&ಬೇರು_Region);

    ctx.regionDepth = ಬೇರು_ctx.regionDepth;
    ctx.regionDepthOpenCV = ಬೇರು_ctx.regionDepthOpenCV;

    ctx.parallel_for_ರಾಶಿ_size = 0;

    ctx.stat_status.propagateFrom(ಬೇರು_ctx.stat_status);
}

ಶೂನ್ಯ parallelForAttachNestedRegion(ಧ್ರುವ Region& ಬೇರು_Region)
{
    CV_UNUSED(ಬೇರು_Region);
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();

    CV_DbgAssert(ctx.dummy_ರಾಶಿ_ತುದಿ.region == &ಬೇರು_Region);

    Region* region = ctx.getCurrentActiveRegion();
    CV_LOG_PARALLEL(ರಿಕ್ತ, " PARALLEL_FOR: " << (ಶೂನ್ಯ*)region << " ==> " << &ಬೇರು_Region);
    ಯದಿ (!region)
        ಪ್ರತ್ಯಾಗಮ;

#ifdef OPENCV_WITH_ITT
    ಯದಿ (!ಬೇರು_Region.pImpl || !ಬೇರು_Region.pImpl->itt_id_registered)
        ಪ್ರತ್ಯಾಗಮ;

    ಯದಿ (!region->pImpl)
        ಪ್ರತ್ಯಾಗಮ;

    CV_LOG_PARALLEL(ರಿಕ್ತ, " PARALLEL_FOR ITT: " << (ಶೂನ್ಯ*)ಬೇರು_Region.pImpl->itt_id.d1 << ":" << ಬೇರು_Region.pImpl->itt_id.d2 << ":" << (ಶೂನ್ಯ*)ಬೇರು_Region.pImpl->itt_id.d3 << " => "
                                 << (ಶೂನ್ಯ*)region->pImpl->itt_id.d1 << ":" << region->pImpl->itt_id.d2 << ":" << (ಶೂನ್ಯ*)region->pImpl->itt_id.d3);
    __itt_relation_add(domain, region->pImpl->itt_id, __itt_relation_is_child_of, ಬೇರು_Region.pImpl->itt_id);
#endif
}

ಶೂನ್ಯ parallelForFinalize(ಧ್ರುವ Region& ಬೇರು_Region)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();

    int64 endTimestamp = getTimestampNS();
    int64 duration = endTimestamp - ctx.ರಾಶಿ_ತುದಿ_Begin_Timestamp();
    CV_LOG_PARALLEL(ರಿಕ್ತ, "parallel_for duration: " << duration << " " << &ಬೇರು_Region);

    std::vector<TraceManagerThreadLocal*> threads_ctx;
    getTraceManager().tls.gather(threads_ctx);
    RegionStatistics parallel_for_stat;
    ಪ್ರತಿ (size_t i = 0; i < threads_ctx.size(); i++)
    {
        TraceManagerThreadLocal* child_ctx = threads_ctx[i];

        ಯದಿ (child_ctx && child_ctx->ರಾಶಿ_ತುದಿ_Region() == &ಬೇರು_Region)
        {
            CV_LOG_PARALLEL(ರಿಕ್ತ, "Thread=" << child_ctx->threadID << " " << child_ctx->stat);
            RegionStatistics child_stat;
            child_ctx->stat.grab(child_stat);
            parallel_for_stat.append(child_stat);
            ಯದಿ (child_ctx != &ctx)
            {
                child_ctx->dummy_ರಾಶಿ_ತುದಿ = TraceManagerThreadLocal::ರಾಶಿ_Entry();
            }
            ಅನ್ಯಥಾ
            {
                ctx.parallel_for_stat.grab(ctx.stat);
                ctx.stat_status = ctx.parallel_for_stat_status;
                child_ctx->dummy_ರಾಶಿ_ತುದಿ = TraceManagerThreadLocal::ರಾಶಿ_Entry();
            }
        }
    }

    ಭಿನ್ನ parallel_coeff = std::min(1.0f, duration / (ಭಿನ್ನ)(parallel_for_stat.duration));
    CV_LOG_PARALLEL(ರಿಕ್ತ, "parallel_coeff=" << 1.0f / parallel_coeff);
    CV_LOG_PARALLEL(ರಿಕ್ತ, parallel_for_stat);
    ಯದಿ (parallel_coeff != 1.0f)
    {
        parallel_for_stat.multiply(parallel_coeff);
        CV_LOG_PARALLEL(ರಿಕ್ತ, parallel_for_stat);
    }
    parallel_for_stat.duration = 0;
    ctx.stat.append(parallel_for_stat);
    CV_LOG_PARALLEL(ರಿಕ್ತ, ctx.stat);
}

ಸಂರಚನಾ TraceArg::ExtraData
{
#ifdef OPENCV_WITH_ITT
    // Special fields for ITT
    __itt_string_handle* ಚಞ್ಚಲ ittHandle_name;
#endif
    ExtraData(TraceManagerThreadLocal& ctx, ಧ್ರುವ TraceArg& arg)
    {
        CV_UNUSED(ctx); CV_UNUSED(arg);
#ifdef OPENCV_WITH_ITT
        ಯದಿ (isITTEnabled())
        {
            // Caching is not required here, because there is builtin cache.
            // https://software.intel.com/en-us/node/544203:
            //     Consecutive calls to __itt_string_handle_create with the same name return the same value.
            ittHandle_name = __itt_string_handle_create(arg.name);
        }
        ಅನ್ಯಥಾ
        {
            ittHandle_name = 0;
        }
#endif
    }
};

ಸ್ಥಿರ ಶೂನ್ಯ initTraceArg(TraceManagerThreadLocal& ctx, ಧ್ರುವ TraceArg& arg)
{
    TraceArg::ExtraData** pExtra = arg.ppExtra;
    ಯದಿ (*pExtra == ರಿಕ್ತ)
    {
        cv::AutoLock lock(cv::getInitializationMutex());
        ಯದಿ (*pExtra == ರಿಕ್ತ)
        {
            *pExtra = ನೂತನ TraceArg::ExtraData(ctx, arg);
        }
    }
}
ಶೂನ್ಯ traceArg(ಧ್ರುವ TraceArg& arg, ಧ್ರುವ ವರ್ಣ* value)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    Region* region = ctx.getCurrentActiveRegion();
    ಯದಿ (!region)
        ಪ್ರತ್ಯಾಗಮ;
    CV_Assert(region->pImpl);
    initTraceArg(ctx, arg);
    ಯದಿ (!value)
        value = "<null>";
#ifdef OPENCV_WITH_ITT
    ಯದಿ (isITTEnabled())
    {
        __itt_metadata_str_add(domain, region->pImpl->itt_id, (*arg.ppExtra)->ittHandle_name, value, strlen(value));
    }
#endif
}
ಶೂನ್ಯ traceArg(ಧ್ರುವ TraceArg& arg, ಪೂರ್ಣಾಙ್ಕ value)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    Region* region = ctx.getCurrentActiveRegion();
    ಯದಿ (!region)
        ಪ್ರತ್ಯಾಗಮ;
    CV_Assert(region->pImpl);
    initTraceArg(ctx, arg);
#ifdef OPENCV_WITH_ITT
    ಯದಿ (isITTEnabled())
    {
        __itt_metadata_add(domain, region->pImpl->itt_id, (*arg.ppExtra)->ittHandle_name, ಪರಿಮಾಣ(ಪೂರ್ಣಾಙ್ಕ) == 4 ? __itt_metadata_s32 : __itt_metadata_s64, 1, &value);
    }
#ಅನ್ಯಥಾ
    CV_UNUSED(value);
#endif
}
ಶೂನ್ಯ traceArg(ಧ್ರುವ TraceArg& arg, int64 value)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    Region* region = ctx.getCurrentActiveRegion();
    ಯದಿ (!region)
        ಪ್ರತ್ಯಾಗಮ;
    CV_Assert(region->pImpl);
    initTraceArg(ctx, arg);
#ifdef OPENCV_WITH_ITT
    ಯದಿ (isITTEnabled())
    {
        __itt_metadata_add(domain, region->pImpl->itt_id, (*arg.ppExtra)->ittHandle_name, __itt_metadata_s64, 1, &value);
    }
#ಅನ್ಯಥಾ
    CV_UNUSED(value);
#endif
}
ಶೂನ್ಯ traceArg(ಧ್ರುವ TraceArg& arg, ದ್ವಿಗುಣ value)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    Region* region = ctx.getCurrentActiveRegion();
    ಯದಿ (!region)
        ಪ್ರತ್ಯಾಗಮ;
    CV_Assert(region->pImpl);
    initTraceArg(ctx, arg);
#ifdef OPENCV_WITH_ITT
    ಯದಿ (isITTEnabled())
    {
        __itt_metadata_add(domain, region->pImpl->itt_id, (*arg.ppExtra)->ittHandle_name, __itt_metadata_double, 1, &value);
    }
#ಅನ್ಯಥಾ
    CV_UNUSED(value);
#endif
}

#ಅನ್ಯಥಾ

Region::Region(ಧ್ರುವ LocationStaticStorage&) : pImpl(ರಿಕ್ತ), implFlags(0) {}
ಶೂನ್ಯ Region::destroy() {}

ಶೂನ್ಯ traceArg(ಧ್ರುವ TraceArg&, ಧ್ರುವ ವರ್ಣ*) {}
ಶೂನ್ಯ traceArg(ಧ್ರುವ TraceArg&, ಪೂರ್ಣಾಙ್ಕ) {};
ಶೂನ್ಯ traceArg(ಧ್ರುವ TraceArg&, int64) {};
ಶೂನ್ಯ traceArg(ಧ್ರುವ TraceArg&, ದ್ವಿಗುಣ) {};

#endif

}}}} // namespace
