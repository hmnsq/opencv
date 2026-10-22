// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#અન્તર્ભાવ "precomp.hpp"

#અન્તર્ભાવ <opencv2/core/utils/trace.hpp>
#અન્તર્ભાવ <opencv2/core/utils/trace.private.hpp>
#અન્તર્ભાવ <opencv2/core/utils/configuration.private.hpp>

#અન્તર્ભાવ <opencv2/core/opencl/ocl_defs.hpp>

#અન્તર્ભાવ <cstdarg> // va_start

#અન્તર્ભાવ <sstream>
#અન્તર્ભાવ <ostream>
#અન્તર્ભાવ <fstream>

#જો 0
#define CV_LOG(...) CV_LOG_INFO(ખાલી, __VA_ARGS__)
#નહીંતર
#define CV_LOG(...) {}
#endif

#જો 0
#define CV_LOG_ITT(...) CV_LOG_INFO(ખાલી, __VA_ARGS__)
#નહીંતર
#define CV_LOG_ITT(...) {}
#endif

#જો 1
#define CV_LOG_TRACE_BAILOUT(tag, ...) CV_LOG_INFO(tag, __VA_ARGS__)
#નહીંતર
#define CV_LOG_TRACE_BAILOUT(...) {}
#endif

#જો 0
#define CV_LOG_PARALLEL(tag, ...) CV_LOG_INFO(tag, __VA_ARGS__)
#નહીંતર
#define CV_LOG_PARALLEL(...) {}
#endif

#જો 0
#define CV_LOG_CTX_STAT(tag, ...) CV_LOG_INFO(tag, __VA_ARGS__)
#નહીંતર
#define CV_LOG_CTX_STAT(...) {}
#endif

#જો 0
#define CV_LOG_SKIP(tag, ...) CV_LOG_INFO(tag, __VA_ARGS__)
#નહીંતર
#define CV_LOG_SKIP(...) {}
#endif

નામાકાશ cv {
નામાકાશ utils {
નામાકાશ trace {
નામાકાશ details {

#ifdef OPENCV_TRACE

#ifdef _MSC_VER
#pragma warning(disable:4065) // switch statement contains 'default' but no 'case' labels
#endif

સ્થિર તાર્કિક getParameterTraceEnable()
{
    સ્થિર તાર્કિક param_traceEnable = utils::getConfigurationParameterBool("OPENCV_TRACE", ખોટું);
    પાછા param_traceEnable;
}

// TODO lazy configuration flags
સ્થિર પૂર્ણાંક param_maxRegionDepthOpenCV = (પૂર્ણાંક)utils::getConfigurationParameterSizeT("OPENCV_TRACE_DEPTH_OPENCV", 1);
સ્થિર પૂર્ણાંક param_maxRegionChildrenOpenCV = (પૂર્ણાંક)utils::getConfigurationParameterSizeT("OPENCV_TRACE_MAX_CHILDREN_OPENCV", 1000);
સ્થિર પૂર્ણાંક param_maxRegionChildren = (પૂર્ણાંક)utils::getConfigurationParameterSizeT("OPENCV_TRACE_MAX_CHILDREN", 10000);

સ્થિર અચલ cv::String& getParameterTraceLocation()
{
    સ્થિર cv::String param_traceLocation = utils::getConfigurationParameterString("OPENCV_TRACE_LOCATION", "OpenCVTrace");
    પાછા param_traceLocation;
}

#ifdef HAVE_OPENCL
સ્થિર તાર્કિક param_synchronizeOpenCL = utils::getConfigurationParameterBool("OPENCV_TRACE_SYNC_OPENCL", ખોટું);
#endif

#ifdef OPENCV_WITH_ITT
સ્થિર તાર્કિક param_ITT_registerParentScope = utils::getConfigurationParameterBool("OPENCV_TRACE_ITT_PARENT", ખોટું);
#endif

સ્થિર અચલ અક્ષર* _spaces(પૂર્ણાંક ગણતરી_2)
{
    સ્થિર અચલ અક્ષર buf[64] =
"                                                               ";
    પાછા &buf[63 - (ગણતરી_2 & 63)];
}

/**
 * Text-based trace messages
 */
વર્ગ TraceMessage
{
જાહેર:
    અક્ષર buffer[1024];
    size_t len;
    તાર્કિક hasError;

    TraceMessage() :
        len(0),
        hasError(ખોટું)
    {}

    તાર્કિક printf(અચલ અક્ષર* format, ...)
    {
        અક્ષર* buf = &buffer[len];
        size_t sz = કદ(buffer) - len;
        va_list ap;
        va_start(ap, format);
        પૂર્ણાંક n = cv_vsnprintf(buf, (પૂર્ણાંક)sz, format, ap);
        va_end(ap);
        જો (n < 0 || (size_t)n > sz)
        {
            hasError = સાચું;
            પાછા ખોટું;
        }
        len += n;
        પાછા સાચું;
    }

    તાર્કિક formatlocation(અચલ Region::LocationStaticStorage& location)
    {
        પાછા આ->printf("l,%lld,\"%s\",%d,\"%s\",0x%llX\n",
                (long long પૂર્ણાંક)(*location.ppExtra)->global_location_id,
                location.filename,
                location.લીટી,
                location.name,
                (long long પૂર્ણાંક)(location.flags & ~0xF0000000));
    }
    તાર્કિક formatRegionEnter(અચલ Region& region)
    {
        તાર્કિક ok = આ->printf("b,%d,%lld,%lld,%lld",
                (પૂર્ણાંક)region.pImpl->threadID,
                (long long પૂર્ણાંક)region.pImpl->beginTimestamp,
                (long long પૂર્ણાંક)((*region.pImpl->location.ppExtra)->global_location_id),
                (long long પૂર્ણાંક)region.pImpl->global_region_id);
        જો (region.pImpl->parentRegion && region.pImpl->parentRegion->pImpl)
        {
            જો (region.pImpl->parentRegion->pImpl->threadID != region.pImpl->threadID)
                ok &= આ->printf(",parentThread=%d,parent=%lld",
                        (પૂર્ણાંક)region.pImpl->parentRegion->pImpl->threadID,
                        (long long પૂર્ણાંક)region.pImpl->parentRegion->pImpl->global_region_id);
        }
        ok &= આ->printf("\n");
        પાછા ok;
    }
    તાર્કિક formatRegionLeave(અચલ Region& region, અચલ RegionStatistics& result)
    {
        CV_DbgAssert(region.pImpl->endTimestamp - region.pImpl->beginTimestamp == result.duration);
        તાર્કિક ok = આ->printf("e,%d,%lld,%lld,%lld,%lld",
                (પૂર્ણાંક)region.pImpl->threadID,
                (long long પૂર્ણાંક)region.pImpl->endTimestamp,
                (long long પૂર્ણાંક)(*region.pImpl->location.ppExtra)->global_location_id,
                (long long પૂર્ણાંક)region.pImpl->global_region_id,
                (long long પૂર્ણાંક)result.duration);
        જો (result.currentSkippedRegions)
            ok &= આ->printf(",skip=%d", (પૂર્ણાંક)result.currentSkippedRegions);
#ifdef HAVE_IPP
        જો (result.durationImplIPP)
            ok &= આ->printf(",tIPP=%lld", (long long પૂર્ણાંક)result.durationImplIPP);
#endif
#ifdef HAVE_OPENCL
        જો (result.durationImplOpenCL)
            ok &= આ->printf(",tOCL=%lld", (long long પૂર્ણાંક)result.durationImplOpenCL);
#endif
        ok &= આ->printf("\n");
        પાછા ok;
    }
    તાર્કિક recordRegionArg(અચલ Region& region, અચલ TraceArg& arg, અચલ અક્ષર* value)
    {
        પાછા આ->printf("a,%d,%lld,%lld,\"%s\",\"%s\"\n",
                region.pImpl->threadID,
                (long long પૂર્ણાંક)region.pImpl->beginTimestamp,
                (long long પૂર્ણાંક)region.pImpl->global_region_id,
                arg.name,
                value);
    }
};


#ifdef OPENCV_WITH_ITT
સ્થિર __itt_domain* domain = ખાલી;

સ્થિર તાર્કિક isITTEnabled()
{
    સ્થિર અસ્થિર તાર્કિક isInitialized = ખોટું;
    સ્થિર તાર્કિક isEnabled = ખોટું;
    જો (!isInitialized)
    {
        cv::AutoLock lock(cv::getInitializationMutex());
        જો (!isInitialized)
        {
            તાર્કિક param_traceITTEnable = utils::getConfigurationParameterBool("OPENCV_TRACE_ITT_ENABLE", સાચું);
            જો (param_traceITTEnable)
            {
                isEnabled = !!(__itt_api_version());
                CV_LOG_ITT("ITT is " << (isEnabled ? "enabled" : "disabled"));
                domain = __itt_domain_create("OpenCVTrace");
            }
            નહીંતર
            {
                CV_LOG_ITT("ITT is disabled through OpenCV parameter");
                isEnabled = ખોટું;
            }
            isInitialized = સાચું;
        }
    }
    પાછા isEnabled;
}
#endif


Region::LocationExtraData::LocationExtraData(અચલ LocationStaticStorage& location)
{
    CV_UNUSED(location);
    સ્થિર પૂર્ણાંક g_location_id_counter = 0;
    global_location_id = CV_XADD(&g_location_id_counter, 1) + 1;
    CV_LOG("Register location: " << global_location_id << " (" << (શૂન્ય*)&location << ")"
            << std::endl << "    file: " << location.filename
            << std::endl << "    line: " << location.લીટી
            << std::endl << "    name: " << location.name);
#ifdef OPENCV_WITH_ITT
    જો (isITTEnabled())
    {
        // Caching is not required here, because there is builtin cache.
        // https://software.intel.com/en-us/node/544203:
        //     Consecutive calls to __itt_string_handle_create with the same name return the same value.
        ittHandle_name = __itt_string_handle_create(location.name);
        ittHandle_filename = __itt_string_handle_create(location.filename);
    }
    નહીંતર
    {
        ittHandle_name = 0;
        ittHandle_filename = 0;
    }
#endif
}

/*static*/ Region::LocationExtraData* Region::LocationExtraData::init(અચલ Region::LocationStaticStorage& location)
{
    LocationExtraData** pLocationExtra = location.ppExtra;
    CV_DbgAssert(pLocationExtra);
    જો (*pLocationExtra == ખાલી)
    {
        cv::AutoLock lock(cv::getInitializationMutex());
        જો (*pLocationExtra == ખાલી)
        {
            *pLocationExtra = નવું Region::LocationExtraData(location);
            TraceStorage* s = getTraceManager().trace_storage.get();
            જો (s)
            {
                TraceMessage સંદેશ;
                સંદેશ.formatlocation(location);
                s->put(સંદેશ);
            }
        }
    }
    પાછા *pLocationExtra;
}


Region::Impl::Impl(TraceManagerThreadLocal& ctx, Region* parentRegion_, Region& region_, અચલ LocationStaticStorage& location_, int64 beginTimestamp_) :
    location(location_),
    region(region_),
    parentRegion(parentRegion_),
    threadID(ctx.threadID),
    global_region_id(++ctx.region_counter),
    beginTimestamp(beginTimestamp_),
    endTimestamp(0),
    direct_Children_ગણતરી(0)
#ifdef OPENCV_WITH_ITT
    ,itt_id_registered(ખોટું)
    ,itt_id(__itt_null)
#endif
{
    CV_DbgAssert(ctx.currentActiveRegion == parentRegion);
    region.pImpl = આ;

    registerRegion(ctx);

    enterRegion(ctx);
}

Region::Impl::~Impl()
{
#ifdef OPENCV_WITH_ITT
    જો (itt_id_registered)
    {
        CV_LOG_ITT(" Destroy ITT region: I=" << (શૂન્ય*)આ);
        __itt_id_destroy(domain, itt_id);
        itt_id_registered = ખોટું;
    }
#endif
    region.pImpl = ખાલી;
}

શૂન્ય Region::Impl::enterRegion(TraceManagerThreadLocal& ctx)
{
    ctx.currentActiveRegion = &region;

    જો (location.flags & REGION_FLAG_FUNCTION)
    {
        જો ((location.flags & REGION_FLAG_APP_CODE) == 0)
        {
            ctx.regionDepthOpenCV++;
        }
        ctx.regionDepth++;
    }

    TraceStorage* s = ctx.getStorage();
    જો (s)
    {
        TraceMessage સંદેશ;
        સંદેશ.formatRegionEnter(region);
        s->put(સંદેશ);
    }
#ifdef OPENCV_WITH_ITT
    જો (isITTEnabled())
    {
        __itt_id parentID = __itt_null;
        જો (param_ITT_registerParentScope && parentRegion && parentRegion->pImpl && parentRegion->pImpl->itt_id_registered && (location.flags & REGION_FLAG_REGION_FORCE) == 0)
            parentID = parentRegion->pImpl->itt_id;
        __itt_task_begin(domain, itt_id, parentID, (*location.ppExtra)->ittHandle_name);
    }
#endif
}

શૂન્ય Region::Impl::leaveRegion(TraceManagerThreadLocal& ctx)
{
    int64 duration = endTimestamp - beginTimestamp; CV_UNUSED(duration);
    RegionStatistics result;
    ctx.stat.grab(result);
    ctx.totalSkippedEvents += result.currentSkippedRegions;
    CV_LOG(_spaces(ctx.getCurrentDepth()*4) << "leaveRegion(): " << (શૂન્ય*)આ << " " << result);
#ifdef OPENCV_WITH_ITT
    જો (isITTEnabled())
    {
        જો (result.currentSkippedRegions)
        {
            __itt_metadata_add(domain, itt_id, __itt_string_handle_create("skipped trace entries"), __itt_metadata_u32, 1, &result.currentSkippedRegions);
        }
#ifdef HAVE_IPP
        જો (result.durationImplIPP)
            __itt_metadata_add(domain, itt_id, __itt_string_handle_create("tIPP"), __itt_metadata_u64, 1, &result.durationImplIPP);
#endif
#ifdef HAVE_OPENCL
        જો (result.durationImplOpenCL)
            __itt_metadata_add(domain, itt_id, __itt_string_handle_create("tOpenCL"), __itt_metadata_u64, 1, &result.durationImplOpenCL);
#endif
        __itt_task_end(domain);
    }
#endif
    TraceStorage* s = ctx.getStorage();
    જો (s)
    {
        TraceMessage સંદેશ;
        સંદેશ.formatRegionLeave(region, result);
        s->put(સંદેશ);
    }

    જો (location.flags & REGION_FLAG_FUNCTION)
    {
        જો ((location.flags & REGION_FLAG_APP_CODE) == 0)
        {
            ctx.regionDepthOpenCV--;
        }
        ctx.regionDepth--;
    }

    ctx.currentActiveRegion = parentRegion;
}

શૂન્ય Region::Impl::release()
{
    delete આ;
}

શૂન્ય Region::Impl::registerRegion(TraceManagerThreadLocal& ctx)
{
#ifdef OPENCV_WITH_ITT
    જો (isITTEnabled())
    {
        જો (!itt_id_registered)
        {
            CV_LOG_ITT(" Register ITT region: I=" << (શૂન્ય*)આ << " " << ctx.threadID << "-" << global_region_id);
#જો 1 // workaround for some ITT backends
            itt_id = __itt_id_make((શૂન્ય*)(intptr_t)(((int64)(ctx.threadID + 1) << 32) | global_region_id), global_region_id);
#નહીંતર
            itt_id = __itt_id_make((શૂન્ય*)(intptr_t)(ctx.threadID + 1), global_region_id);
#endif
            __itt_id_create(domain, itt_id);
            itt_id_registered = સાચું;
        }
    }
#નહીંતર
    CV_UNUSED(ctx);
#endif
}

શૂન્ય RegionStatisticsStatus::enableSkipMode(પૂર્ણાંક depth)
{
    CV_DbgAssert(_skipDepth < 0);
    CV_LOG_SKIP(ખાલી, "SKIP-ENABLE: depth=" << depth);
    _skipDepth = depth;
}
શૂન્ય RegionStatisticsStatus::checkResetSkipMode(પૂર્ણાંક leaveDepth)
{
    જો (leaveDepth <= _skipDepth)
    {
        CV_LOG_SKIP(ખાલી, "SKIP-RESET: leaveDepth=" << leaveDepth << " skipDepth=" << _skipDepth);
        _skipDepth = -1;
    }
}

Region::Region(અચલ LocationStaticStorage& location) :
    pImpl(ખાલી),
    implFlags(0)
{
    // Checks:
    // - global enable flag
    // - parent region is disabled
    // - children count threshold
    // - region location
    // - depth (opencv nested calls)
    જો (!TraceManager::isActivated())
    {
        CV_LOG("Trace is disabled. Bailout");
        પાછા;
    }

    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    CV_LOG(_spaces(ctx.getCurrentDepth()*4) << "Region(): " << (શૂન્ય*)આ << ": " << location.name);

    Region* parentRegion = ctx.ઢગલો_ટોચ_Region();
    અચલ Region::LocationStaticStorage* parentLocation = ctx.ઢગલો_ટોચ_Location();

    જો (location.flags & REGION_FLAG_REGION_NEXT)
    {
        જો (parentRegion && parentRegion->pImpl)
        {
            CV_DbgAssert((parentRegion->pImpl->location.flags & REGION_FLAG_FUNCTION) == 0);
            parentRegion->destroy(); parentRegion->implFlags = 0;
            parentRegion = ctx.ઢગલો_ટોચ_Region();
            parentLocation = ctx.ઢગલો_ટોચ_Location();
        }
    }

    પૂર્ણાંક parentChildren = 0;
    જો (parentRegion && parentRegion->pImpl)
    {
        જો (parentLocation == ખાલી)
        {
            // parallel_for_body code path
            parentChildren = CV_XADD(&parentRegion->pImpl->direct_Children_ગણતરી, 1) + 1;
        }
        નહીંતર
        {
            parentChildren = ++parentRegion->pImpl->direct_Children_ગણતરી;
        }
    }

    int64 beginTimestamp = getTimestampNS();

    પૂર્ણાંક currentDepth = ctx.getCurrentDepth() + 1;
    પસંદગી (location.flags & REGION_FLAG_IMPL_MASK)
    {
#ifdef HAVE_IPP
    સ્થિતિ REGION_FLAG_IMPL_IPP:
        જો (!ctx.stat_status.ignoreDepthImplIPP)
            ctx.stat_status.ignoreDepthImplIPP = currentDepth;
        તોડો;
#endif
#ifdef HAVE_OPENCL
    સ્થિતિ REGION_FLAG_IMPL_OPENCL:
        જો (!ctx.stat_status.ignoreDepthImplOpenCL)
            ctx.stat_status.ignoreDepthImplOpenCL = currentDepth;
        તોડો;
#endif
    મૂળભૂત:
        તોડો;
    }

    ctx.ઢગલો_ધકેલો(આ, &location, beginTimestamp);
    implFlags |= REGION_FLAG_NEED_ઢગલો_કાઢો;

    જો ((location.flags & REGION_FLAG_REGION_FORCE) == 0)
    {
        જો (ctx.stat_status._skipDepth >= 0 && currentDepth > ctx.stat_status._skipDepth)
        {
            CV_LOG(_spaces(ctx.getCurrentDepth()*4) << "Parent region is disabled. Bailout");
            ctx.stat.currentSkippedRegions++;
            પાછા;
        }

        જો (param_maxRegionChildrenOpenCV > 0 && (location.flags & REGION_FLAG_APP_CODE) == 0 && parentLocation && (parentLocation->flags & REGION_FLAG_APP_CODE) == 0)
        {
            જો (parentChildren >= param_maxRegionChildrenOpenCV)
            {
                CV_LOG_TRACE_BAILOUT(ખાલી, _spaces(ctx.getCurrentDepth()*4) << "OpenCV parent region exceeds children count. Bailout");
                ctx.stat_status.enableSkipMode(currentDepth - 1);
                ctx.stat.currentSkippedRegions++;
                DEBUG_ONLY(ctx.dump_ઢગલો(std::cout, ખોટું));
                પાછા;
            }
        }
        જો (param_maxRegionChildren > 0 && parentChildren >= param_maxRegionChildren)
        {
            CV_LOG_TRACE_BAILOUT(ખાલી, _spaces(ctx.getCurrentDepth()*4) << "Parent region exceeds children count. Bailout");
            ctx.stat_status.enableSkipMode(currentDepth - 1);
            ctx.stat.currentSkippedRegions++;
            DEBUG_ONLY(ctx.dump_ઢગલો(std::cout, ખોટું));
            પાછા;
        }
    }

    LocationExtraData::init(location);

    જો ((*location.ppExtra)->global_location_id == 0)
    {
        CV_LOG_TRACE_BAILOUT(ખાલી, _spaces(ctx.getCurrentDepth()*4) << "Region location is disabled. Bailout");
        ctx.stat_status.enableSkipMode(currentDepth);
        ctx.stat.currentSkippedRegions++;
        પાછા;
    }

    જો (parentLocation && (parentLocation->flags & REGION_FLAG_SKIP_NESTED))
    {
        CV_LOG(_spaces(ctx.getCurrentDepth()*4) << "Parent region disables inner regions. Bailout");
        ctx.stat_status.enableSkipMode(currentDepth);
        ctx.stat.currentSkippedRegions++;
        પાછા;
    }

    જો (param_maxRegionDepthOpenCV)
    {
        જો ((location.flags & REGION_FLAG_APP_CODE) == 0)
        {
            જો (ctx.regionDepthOpenCV >= param_maxRegionDepthOpenCV)
            {
                CV_LOG(_spaces(ctx.getCurrentDepth()*4) << "OpenCV region depth is exceed = " << ctx.regionDepthOpenCV << ". Bailout");
                જો (ctx.stat.currentSkippedRegions == 0)
                {
                    DEBUG_ONLY(ctx.dump_ઢગલો(std::cout, ખોટું));
                }
                ctx.stat_status.enableSkipMode(currentDepth);
                ctx.stat.currentSkippedRegions++;
                પાછા;
            }
        }
    }

    નવું Impl(ctx, parentRegion, *આ, location, beginTimestamp);
    CV_DbgAssert(pImpl != ખાલી);
    implFlags |= REGION_FLAG__ACTIVE;

    // parallel_for path
    જો (parentRegion && parentRegion->pImpl)
    {
        જો (parentLocation == ખાલી)
        {
            pImpl->direct_Children_ગણતરી = parentChildren;
        }
    }
}

શૂન્ય Region::destroy()
{
    CV_DbgAssert(implFlags != 0);

    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    CV_LOG(_spaces(ctx.getCurrentDepth()*4) << "Region::destruct(): " << (શૂન્ય*)આ << " pImpl=" << pImpl << " implFlags=" << implFlags << ' ' << (ctx.ઢગલો_ટોચ_Location() ? ctx.ઢગલો_ટોચ_Location()->name : "<unknown>"));

    CV_DbgAssert(implFlags & REGION_FLAG_NEED_ઢગલો_કાઢો);
    અચલ પૂર્ણાંક currentDepth = ctx.getCurrentDepth(); CV_UNUSED(currentDepth);

    CV_LOG_CTX_STAT(ખાલી, _spaces(currentDepth*4) << ctx.stat << ' ' << ctx.stat_status);

    અચલ Region::LocationStaticStorage* location = ctx.ઢગલો_ટોચ_Location();
    Impl::OptimizationPath myCodePath = Impl::CODE_PATH_PLAIN;
    જો (location)
    {
        પસંદગી (location->flags & REGION_FLAG_IMPL_MASK)
        {
#ifdef HAVE_IPP
        સ્થિતિ REGION_FLAG_IMPL_IPP:
            myCodePath = Impl::CODE_PATH_IPP;
            તોડો;
#endif
#ifdef HAVE_OPENCL
        સ્થિતિ REGION_FLAG_IMPL_OPENCL:
            જો (param_synchronizeOpenCL && cv::ocl::isOpenCLActivated())
                cv::ocl::finish();
            myCodePath = Impl::CODE_PATH_OPENCL;
            તોડો;
#endif
        મૂળભૂત:
            તોડો;
        }
    }

    int64 endTimestamp = getTimestampNS();
    int64 duration = endTimestamp - ctx.ઢગલો_ટોચ_Begin_Timestamp();

    તાર્કિક active = isActive();

    જો (active)
        ctx.stat.duration = duration;
    નહીંતર જો (ctx.ઢગલો.size() == ctx.parallel_for_ઢગલો_size + 1)
        ctx.stat.duration += duration;

    પસંદગી (myCodePath) {
        સ્થિતિ Impl::CODE_PATH_PLAIN:
            // nothing
            તોડો;
#ifdef HAVE_IPP
        સ્થિતિ Impl::CODE_PATH_IPP:
            જો (ctx.stat_status.ignoreDepthImplIPP == currentDepth)
            {
                ctx.stat.durationImplIPP += duration;
                ctx.stat_status.ignoreDepthImplIPP = 0;
            }
            નહીંતર જો (active)
            {
                ctx.stat.durationImplIPP = duration;
            }
            તોડો;
#endif
#ifdef HAVE_OPENCL
        સ્થિતિ Impl::CODE_PATH_OPENCL:
            જો (ctx.stat_status.ignoreDepthImplOpenCL == currentDepth)
            {
                ctx.stat.durationImplOpenCL += duration;
                ctx.stat_status.ignoreDepthImplOpenCL = 0;
            }
            નહીંતર જો (active)
            {
                ctx.stat.durationImplOpenCL = duration;
            }
            તોડો;
#endif
        મૂળભૂત:
            તોડો;
    }

    જો (pImpl)
    {
        CV_DbgAssert((implFlags & (REGION_FLAG__ACTIVE | REGION_FLAG_NEED_ઢગલો_કાઢો)) == (REGION_FLAG__ACTIVE | REGION_FLAG_NEED_ઢગલો_કાઢો));
        CV_DbgAssert(ctx.ઢગલો_ટોચ_Region() == આ);
        pImpl->endTimestamp = endTimestamp;
        pImpl->leaveRegion(ctx);
        pImpl->release();
        pImpl = ખાલી;
        DEBUG_ONLY(implFlags &= ~REGION_FLAG__ACTIVE);
    }
    નહીંતર
    {
        CV_DbgAssert(ctx.stat_status._skipDepth <= currentDepth);
    }

    જો (implFlags & REGION_FLAG_NEED_ઢગલો_કાઢો)
    {
        CV_DbgAssert(ctx.ઢગલો_ટોચ_Region() == આ);
        ctx.ઢગલો_કાઢો();
        ctx.stat_status.checkResetSkipMode(currentDepth);
        DEBUG_ONLY(implFlags &= ~REGION_FLAG_NEED_ઢગલો_કાઢો);
    }
    CV_LOG_CTX_STAT(ખાલી, _spaces(currentDepth*4) << "===> " << ctx.stat << ' ' << ctx.stat_status);
}


TraceManagerThreadLocal::~TraceManagerThreadLocal()
{
}

શૂન્ય TraceManagerThreadLocal::dump_ઢગલો(std::ostream& out, તાર્કિક onlyFunctions) અચલ
{
    std::stringstream ss;
    std::deque<ઢગલો_Entry>::const_iterator it = ઢગલો.begin();
    std::deque<ઢગલો_Entry>::const_iterator end = ઢગલો.end();
    પૂર્ણાંક depth = 0;
    માટે (; it != end; ++it)
    {
        અચલ Region::LocationStaticStorage* location = it->location;
        જો (location)
        {
            જો (!onlyFunctions || (location->flags & REGION_FLAG_FUNCTION))
            {
                ss << _spaces(4*depth) << location->name << std::endl;
                depth++;
            }
        }
        નહીંતર
        {
            ss << _spaces(4*depth) << "<unknown>" << std::endl;
            depth++;
        }
    }
    out << ss.str();
}

વર્ગ AsyncTraceStorage CV_FINAL : જાહેર TraceStorage
{
    mutable std::ofstream out;
જાહેર:
    અચલ std::string name;

    AsyncTraceStorage(અચલ std::string& filename) :
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

    તાર્કિક put(અચલ TraceMessage& સંદેશ) અચલ CV_OVERRIDE
    {
        જો (સંદેશ.hasError)
            પાછા ખોટું;
        out << સંદેશ.buffer;
        //DEBUG_ONLY(std::flush(out)); // TODO configure flag
        પાછા સાચું;
    }
};

વર્ગ SyncTraceStorage CV_FINAL : જાહેર TraceStorage
{
    mutable std::ofstream out;
    mutable cv::Mutex mutex;
જાહેર:
    અચલ std::string name;

    SyncTraceStorage(અચલ std::string& filename) :
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

    તાર્કિક put(અચલ TraceMessage& સંદેશ) અચલ CV_OVERRIDE
    {
        જો (સંદેશ.hasError)
            પાછા ખોટું;
        {
            cv::AutoLock l(mutex);
            out << સંદેશ.buffer;
            std::flush(out); // TODO configure flag
        }
        પાછા સાચું;
    }
};


TraceStorage* TraceManagerThreadLocal::getStorage() અચલ
{
    // TODO configuration option for stdout/single trace file
    જો (storage.empty())
    {
        TraceStorage* global = getTraceManager().trace_storage.get();
        જો (global)
        {
            અચલ std::string filepath = cv::format("%s-%03d.txt", getParameterTraceLocation().c_str(), threadID).c_str();
            TraceMessage સંદેશ;
            અચલ અક્ષર* pos = strrchr(filepath.c_str(), '/'); // extract filename
#ifdef _WIN32
            જો (!pos)
                pos = strrchr(filepath.c_str(), '\\');
#endif
            જો (!pos)
                pos = filepath.c_str();
            નહીંતર
                pos += 1; // fix to skip extra slash in filename beginning
            સંદેશ.printf("#thread file: %s\n", pos);
            global->put(સંદેશ);
            storage.reset(નવું AsyncTraceStorage(filepath));
        }
    }
    પાછા storage.get();
}



સ્થિર તાર્કિક activated = ખોટું;
સ્થિર તાર્કિક isInitialized = ખોટું;

TraceManager::TraceManager()
{
    (શૂન્ય)cv::getTimestampNS();

    isInitialized = સાચું;
    CV_LOG("TraceManager ctor: " << (શૂન્ય*)આ);

    CV_LOG("TraceManager configure()");
    activated = getParameterTraceEnable();

    જો (activated)
        trace_storage.reset(નવું SyncTraceStorage(std::string(getParameterTraceLocation()) + ".txt"));

#ifdef OPENCV_WITH_ITT
    જો (isITTEnabled())
    {
        activated = સાચું; // force trace pipeline activation (without OpenCV storage)
        __itt_region_begin(domain, __itt_null, __itt_null, __itt_string_handle_create("OpenCVTrace"));
    }
#endif
}
TraceManager::~TraceManager()
{
    CV_LOG("TraceManager dtor: " << (શૂન્ય*)આ);

#ifdef OPENCV_WITH_ITT
    જો (isITTEnabled())
    {
        __itt_region_end(domain, __itt_null);
    }
#endif

    std::vector<TraceManagerThreadLocal*> threads_ctx;
    tls.gather(threads_ctx);
    size_t totalEvents = 0, totalSkippedEvents = 0;
    માટે (size_t i = 0; i < threads_ctx.size(); i++)
    {
        TraceManagerThreadLocal* ctx = threads_ctx[i];
        જો (ctx)
        {
            totalEvents += ctx->region_counter;
            totalSkippedEvents += ctx->totalSkippedEvents;
        }
    }
    જો (totalEvents || activated)
    {
        CV_LOG_INFO(ખાલી, "Trace: Total events: " << totalEvents);
    }
    જો (totalSkippedEvents)
    {
        CV_LOG_WARNING(ખાલી, "Trace: Total skipped events: " << totalSkippedEvents);
    }

    // This is a global static object, so process starts shutdown here
    // Turn off trace
    cv::__termination = સાચું; // also set in DllMain() notifications handler for DLL_PROCESS_DETACH
    activated = ખોટું;
}

તાર્કિક TraceManager::isActivated()
{
    // Check if process starts shutdown, and set earlyExit to true
    // to prevent further instrumentation processing earlier.
    જો (cv::__termination)
    {
        activated = ખોટું;
        પાછા ખોટું;
    }

    જો (!isInitialized)
    {
        TraceManager& m = getTraceManager();
        CV_UNUSED(m); // TODO
    }

    પાછા activated;
}


સ્થિર TraceManager* getTraceManagerCallOnce()
{
    સ્થિર TraceManager globalInstance;
    પાછા &globalInstance;
}
TraceManager& getTraceManager()
{
    CV_SINGLETON_LAZY_INIT_REF(TraceManager, getTraceManagerCallOnce())
}

શૂન્ય parallel_For_Set_મૂળ_Region(અચલ Region& મૂળ_Region, અચલ TraceManagerThreadLocal& મૂળ_ctx)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();

    જો (ctx.dummy_ઢગલો_ટોચ.region == &મૂળ_Region) // already attached
        પાછા;

    CV_Assert(ctx.dummy_ઢગલો_ટોચ.region == ખાલી);
    ctx.dummy_ઢગલો_ટોચ = TraceManagerThreadLocal::ઢગલો_Entry(const_cast<Region*>(&મૂળ_Region), ખાલી, -1);

    જો (&ctx == &મૂળ_ctx)
    {
        ctx.stat.grab(ctx.parallel_for_stat);
        ctx.parallel_for_stat_status = ctx.stat_status;
        ctx.parallel_for_ઢગલો_size = ctx.ઢગલો.size();
        પાછા;
    }

    CV_Assert(ctx.ઢગલો.empty());

    ctx.currentActiveRegion = const_cast<Region*>(&મૂળ_Region);

    ctx.regionDepth = મૂળ_ctx.regionDepth;
    ctx.regionDepthOpenCV = મૂળ_ctx.regionDepthOpenCV;

    ctx.parallel_for_ઢગલો_size = 0;

    ctx.stat_status.propagateFrom(મૂળ_ctx.stat_status);
}

શૂન્ય parallelForAttachNestedRegion(અચલ Region& મૂળ_Region)
{
    CV_UNUSED(મૂળ_Region);
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();

    CV_DbgAssert(ctx.dummy_ઢગલો_ટોચ.region == &મૂળ_Region);

    Region* region = ctx.getCurrentActiveRegion();
    CV_LOG_PARALLEL(ખાલી, " PARALLEL_FOR: " << (શૂન્ય*)region << " ==> " << &મૂળ_Region);
    જો (!region)
        પાછા;

#ifdef OPENCV_WITH_ITT
    જો (!મૂળ_Region.pImpl || !મૂળ_Region.pImpl->itt_id_registered)
        પાછા;

    જો (!region->pImpl)
        પાછા;

    CV_LOG_PARALLEL(ખાલી, " PARALLEL_FOR ITT: " << (શૂન્ય*)મૂળ_Region.pImpl->itt_id.d1 << ":" << મૂળ_Region.pImpl->itt_id.d2 << ":" << (શૂન્ય*)મૂળ_Region.pImpl->itt_id.d3 << " => "
                                 << (શૂન્ય*)region->pImpl->itt_id.d1 << ":" << region->pImpl->itt_id.d2 << ":" << (શૂન્ય*)region->pImpl->itt_id.d3);
    __itt_relation_add(domain, region->pImpl->itt_id, __itt_relation_is_child_of, મૂળ_Region.pImpl->itt_id);
#endif
}

શૂન્ય parallelForFinalize(અચલ Region& મૂળ_Region)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();

    int64 endTimestamp = getTimestampNS();
    int64 duration = endTimestamp - ctx.ઢગલો_ટોચ_Begin_Timestamp();
    CV_LOG_PARALLEL(ખાલી, "parallel_for duration: " << duration << " " << &મૂળ_Region);

    std::vector<TraceManagerThreadLocal*> threads_ctx;
    getTraceManager().tls.gather(threads_ctx);
    RegionStatistics parallel_for_stat;
    માટે (size_t i = 0; i < threads_ctx.size(); i++)
    {
        TraceManagerThreadLocal* child_ctx = threads_ctx[i];

        જો (child_ctx && child_ctx->ઢગલો_ટોચ_Region() == &મૂળ_Region)
        {
            CV_LOG_PARALLEL(ખાલી, "Thread=" << child_ctx->threadID << " " << child_ctx->stat);
            RegionStatistics child_stat;
            child_ctx->stat.grab(child_stat);
            parallel_for_stat.append(child_stat);
            જો (child_ctx != &ctx)
            {
                child_ctx->dummy_ઢગલો_ટોચ = TraceManagerThreadLocal::ઢગલો_Entry();
            }
            નહીંતર
            {
                ctx.parallel_for_stat.grab(ctx.stat);
                ctx.stat_status = ctx.parallel_for_stat_status;
                child_ctx->dummy_ઢગલો_ટોચ = TraceManagerThreadLocal::ઢગલો_Entry();
            }
        }
    }

    દશાંશ parallel_coeff = std::min(1.0f, duration / (દશાંશ)(parallel_for_stat.duration));
    CV_LOG_PARALLEL(ખાલી, "parallel_coeff=" << 1.0f / parallel_coeff);
    CV_LOG_PARALLEL(ખાલી, parallel_for_stat);
    જો (parallel_coeff != 1.0f)
    {
        parallel_for_stat.multiply(parallel_coeff);
        CV_LOG_PARALLEL(ખાલી, parallel_for_stat);
    }
    parallel_for_stat.duration = 0;
    ctx.stat.append(parallel_for_stat);
    CV_LOG_PARALLEL(ખાલી, ctx.stat);
}

સંરચના TraceArg::ExtraData
{
#ifdef OPENCV_WITH_ITT
    // Special fields for ITT
    __itt_string_handle* અસ્થિર ittHandle_name;
#endif
    ExtraData(TraceManagerThreadLocal& ctx, અચલ TraceArg& arg)
    {
        CV_UNUSED(ctx); CV_UNUSED(arg);
#ifdef OPENCV_WITH_ITT
        જો (isITTEnabled())
        {
            // Caching is not required here, because there is builtin cache.
            // https://software.intel.com/en-us/node/544203:
            //     Consecutive calls to __itt_string_handle_create with the same name return the same value.
            ittHandle_name = __itt_string_handle_create(arg.name);
        }
        નહીંતર
        {
            ittHandle_name = 0;
        }
#endif
    }
};

સ્થિર શૂન્ય initTraceArg(TraceManagerThreadLocal& ctx, અચલ TraceArg& arg)
{
    TraceArg::ExtraData** pExtra = arg.ppExtra;
    જો (*pExtra == ખાલી)
    {
        cv::AutoLock lock(cv::getInitializationMutex());
        જો (*pExtra == ખાલી)
        {
            *pExtra = નવું TraceArg::ExtraData(ctx, arg);
        }
    }
}
શૂન્ય traceArg(અચલ TraceArg& arg, અચલ અક્ષર* value)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    Region* region = ctx.getCurrentActiveRegion();
    જો (!region)
        પાછા;
    CV_Assert(region->pImpl);
    initTraceArg(ctx, arg);
    જો (!value)
        value = "<null>";
#ifdef OPENCV_WITH_ITT
    જો (isITTEnabled())
    {
        __itt_metadata_str_add(domain, region->pImpl->itt_id, (*arg.ppExtra)->ittHandle_name, value, strlen(value));
    }
#endif
}
શૂન્ય traceArg(અચલ TraceArg& arg, પૂર્ણાંક value)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    Region* region = ctx.getCurrentActiveRegion();
    જો (!region)
        પાછા;
    CV_Assert(region->pImpl);
    initTraceArg(ctx, arg);
#ifdef OPENCV_WITH_ITT
    જો (isITTEnabled())
    {
        __itt_metadata_add(domain, region->pImpl->itt_id, (*arg.ppExtra)->ittHandle_name, કદ(પૂર્ણાંક) == 4 ? __itt_metadata_s32 : __itt_metadata_s64, 1, &value);
    }
#નહીંતર
    CV_UNUSED(value);
#endif
}
શૂન્ય traceArg(અચલ TraceArg& arg, int64 value)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    Region* region = ctx.getCurrentActiveRegion();
    જો (!region)
        પાછા;
    CV_Assert(region->pImpl);
    initTraceArg(ctx, arg);
#ifdef OPENCV_WITH_ITT
    જો (isITTEnabled())
    {
        __itt_metadata_add(domain, region->pImpl->itt_id, (*arg.ppExtra)->ittHandle_name, __itt_metadata_s64, 1, &value);
    }
#નહીંતર
    CV_UNUSED(value);
#endif
}
શૂન્ય traceArg(અચલ TraceArg& arg, દ્વિગુણ value)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    Region* region = ctx.getCurrentActiveRegion();
    જો (!region)
        પાછા;
    CV_Assert(region->pImpl);
    initTraceArg(ctx, arg);
#ifdef OPENCV_WITH_ITT
    જો (isITTEnabled())
    {
        __itt_metadata_add(domain, region->pImpl->itt_id, (*arg.ppExtra)->ittHandle_name, __itt_metadata_double, 1, &value);
    }
#નહીંતર
    CV_UNUSED(value);
#endif
}

#નહીંતર

Region::Region(અચલ LocationStaticStorage&) : pImpl(ખાલી), implFlags(0) {}
શૂન્ય Region::destroy() {}

શૂન્ય traceArg(અચલ TraceArg&, અચલ અક્ષર*) {}
શૂન્ય traceArg(અચલ TraceArg&, પૂર્ણાંક) {};
શૂન્ય traceArg(અચલ TraceArg&, int64) {};
શૂન્ય traceArg(અચલ TraceArg&, દ્વિગુણ) {};

#endif

}}}} // namespace
