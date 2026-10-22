// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#அந்தர்பாவ "precomp.hpp"

#அந்தர்பாவ <opencv2/core/utils/trace.hpp>
#அந்தர்பாவ <opencv2/core/utils/trace.private.hpp>
#அந்தர்பாவ <opencv2/core/utils/configuration.private.hpp>

#அந்தர்பாவ <opencv2/core/opencl/ocl_defs.hpp>

#அந்தர்பாவ <cstdarg> // va_start

#அந்தர்பாவ <sstream>
#அந்தர்பாவ <ostream>
#அந்தர்பாவ <fstream>

#எனில் 0
#define CV_LOG(...) CV_LOG_INFO(வெற்று, __VA_ARGS__)
#இல்லையெனில்
#define CV_LOG(...) {}
#endif

#எனில் 0
#define CV_LOG_ITT(...) CV_LOG_INFO(வெற்று, __VA_ARGS__)
#இல்லையெனில்
#define CV_LOG_ITT(...) {}
#endif

#எனில் 1
#define CV_LOG_TRACE_BAILOUT(tag, ...) CV_LOG_INFO(tag, __VA_ARGS__)
#இல்லையெனில்
#define CV_LOG_TRACE_BAILOUT(...) {}
#endif

#எனில் 0
#define CV_LOG_PARALLEL(tag, ...) CV_LOG_INFO(tag, __VA_ARGS__)
#இல்லையெனில்
#define CV_LOG_PARALLEL(...) {}
#endif

#எனில் 0
#define CV_LOG_CTX_STAT(tag, ...) CV_LOG_INFO(tag, __VA_ARGS__)
#இல்லையெனில்
#define CV_LOG_CTX_STAT(...) {}
#endif

#எனில் 0
#define CV_LOG_SKIP(tag, ...) CV_LOG_INFO(tag, __VA_ARGS__)
#இல்லையெனில்
#define CV_LOG_SKIP(...) {}
#endif

நாமாகாஶ cv {
நாமாகாஶ utils {
நாமாகாஶ trace {
நாமாகாஶ details {

#ifdef OPENCV_TRACE

#ifdef _MSC_VER
#pragma warning(disable:4065) // switch statement contains 'default' but no 'case' labels
#endif

நிலையான தருக்க getParameterTraceEnable()
{
    நிலையான தருக்க param_traceEnable = utils::getConfigurationParameterBool("OPENCV_TRACE", பொய்);
    திரும்பு param_traceEnable;
}

// TODO lazy configuration flags
நிலையான முழு param_maxRegionDepthOpenCV = (முழு)utils::getConfigurationParameterSizeT("OPENCV_TRACE_DEPTH_OPENCV", 1);
நிலையான முழு param_maxRegionChildrenOpenCV = (முழு)utils::getConfigurationParameterSizeT("OPENCV_TRACE_MAX_CHILDREN_OPENCV", 1000);
நிலையான முழு param_maxRegionChildren = (முழு)utils::getConfigurationParameterSizeT("OPENCV_TRACE_MAX_CHILDREN", 10000);

நிலையான மாறிலி cv::String& getParameterTraceLocation()
{
    நிலையான cv::String param_traceLocation = utils::getConfigurationParameterString("OPENCV_TRACE_LOCATION", "OpenCVTrace");
    திரும்பு param_traceLocation;
}

#ifdef HAVE_OPENCL
நிலையான தருக்க param_synchronizeOpenCL = utils::getConfigurationParameterBool("OPENCV_TRACE_SYNC_OPENCL", பொய்);
#endif

#ifdef OPENCV_WITH_ITT
நிலையான தருக்க param_ITT_registerParentScope = utils::getConfigurationParameterBool("OPENCV_TRACE_ITT_PARENT", பொய்);
#endif

நிலையான மாறிலி எழுத்து* _spaces(முழு மொத்தம்_2)
{
    நிலையான மாறிலி எழுத்து buf[64] =
"                                                               ";
    திரும்பு &buf[63 - (மொத்தம்_2 & 63)];
}

/**
 * Text-based trace messages
 */
வகுப்பு TraceMessage
{
பொது:
    எழுத்து buffer[1024];
    size_t len;
    தருக்க hasError;

    TraceMessage() :
        len(0),
        hasError(பொய்)
    {}

    தருக்க printf(மாறிலி எழுத்து* format, ...)
    {
        எழுத்து* buf = &buffer[len];
        size_t sz = அளவு(buffer) - len;
        va_list ap;
        va_start(ap, format);
        முழு n = cv_vsnprintf(buf, (முழு)sz, format, ap);
        va_end(ap);
        எனில் (n < 0 || (size_t)n > sz)
        {
            hasError = உண்மை;
            திரும்பு பொய்;
        }
        len += n;
        திரும்பு உண்மை;
    }

    தருக்க formatlocation(மாறிலி Region::LocationStaticStorage& location)
    {
        திரும்பு இது->printf("l,%lld,\"%s\",%d,\"%s\",0x%llX\n",
                (long long முழு)(*location.ppExtra)->global_location_id,
                location.filename,
                location.வரி,
                location.name,
                (long long முழு)(location.flags & ~0xF0000000));
    }
    தருக்க formatRegionEnter(மாறிலி Region& region)
    {
        தருக்க ok = இது->printf("b,%d,%lld,%lld,%lld",
                (முழு)region.pImpl->threadID,
                (long long முழு)region.pImpl->beginTimestamp,
                (long long முழு)((*region.pImpl->location.ppExtra)->global_location_id),
                (long long முழு)region.pImpl->global_region_id);
        எனில் (region.pImpl->parentRegion && region.pImpl->parentRegion->pImpl)
        {
            எனில் (region.pImpl->parentRegion->pImpl->threadID != region.pImpl->threadID)
                ok &= இது->printf(",parentThread=%d,parent=%lld",
                        (முழு)region.pImpl->parentRegion->pImpl->threadID,
                        (long long முழு)region.pImpl->parentRegion->pImpl->global_region_id);
        }
        ok &= இது->printf("\n");
        திரும்பு ok;
    }
    தருக்க formatRegionLeave(மாறிலி Region& region, மாறிலி RegionStatistics& result)
    {
        CV_DbgAssert(region.pImpl->endTimestamp - region.pImpl->beginTimestamp == result.duration);
        தருக்க ok = இது->printf("e,%d,%lld,%lld,%lld,%lld",
                (முழு)region.pImpl->threadID,
                (long long முழு)region.pImpl->endTimestamp,
                (long long முழு)(*region.pImpl->location.ppExtra)->global_location_id,
                (long long முழு)region.pImpl->global_region_id,
                (long long முழு)result.duration);
        எனில் (result.currentSkippedRegions)
            ok &= இது->printf(",skip=%d", (முழு)result.currentSkippedRegions);
#ifdef HAVE_IPP
        எனில் (result.durationImplIPP)
            ok &= இது->printf(",tIPP=%lld", (long long முழு)result.durationImplIPP);
#endif
#ifdef HAVE_OPENCL
        எனில் (result.durationImplOpenCL)
            ok &= இது->printf(",tOCL=%lld", (long long முழு)result.durationImplOpenCL);
#endif
        ok &= இது->printf("\n");
        திரும்பு ok;
    }
    தருக்க recordRegionArg(மாறிலி Region& region, மாறிலி TraceArg& arg, மாறிலி எழுத்து* value)
    {
        திரும்பு இது->printf("a,%d,%lld,%lld,\"%s\",\"%s\"\n",
                region.pImpl->threadID,
                (long long முழு)region.pImpl->beginTimestamp,
                (long long முழு)region.pImpl->global_region_id,
                arg.name,
                value);
    }
};


#ifdef OPENCV_WITH_ITT
நிலையான __itt_domain* domain = வெற்று;

நிலையான தருக்க isITTEnabled()
{
    நிலையான நிலையற்ற தருக்க isInitialized = பொய்;
    நிலையான தருக்க isEnabled = பொய்;
    எனில் (!isInitialized)
    {
        cv::AutoLock lock(cv::getInitializationMutex());
        எனில் (!isInitialized)
        {
            தருக்க param_traceITTEnable = utils::getConfigurationParameterBool("OPENCV_TRACE_ITT_ENABLE", உண்மை);
            எனில் (param_traceITTEnable)
            {
                isEnabled = !!(__itt_api_version());
                CV_LOG_ITT("ITT is " << (isEnabled ? "enabled" : "disabled"));
                domain = __itt_domain_create("OpenCVTrace");
            }
            இல்லையெனில்
            {
                CV_LOG_ITT("ITT is disabled through OpenCV parameter");
                isEnabled = பொய்;
            }
            isInitialized = உண்மை;
        }
    }
    திரும்பு isEnabled;
}
#endif


Region::LocationExtraData::LocationExtraData(மாறிலி LocationStaticStorage& location)
{
    CV_UNUSED(location);
    நிலையான முழு g_location_id_counter = 0;
    global_location_id = CV_XADD(&g_location_id_counter, 1) + 1;
    CV_LOG("Register location: " << global_location_id << " (" << (வெறுமை*)&location << ")"
            << std::endl << "    file: " << location.filename
            << std::endl << "    line: " << location.வரி
            << std::endl << "    name: " << location.name);
#ifdef OPENCV_WITH_ITT
    எனில் (isITTEnabled())
    {
        // Caching is not required here, because there is builtin cache.
        // https://software.intel.com/en-us/node/544203:
        //     Consecutive calls to __itt_string_handle_create with the same name return the same value.
        ittHandle_name = __itt_string_handle_create(location.name);
        ittHandle_filename = __itt_string_handle_create(location.filename);
    }
    இல்லையெனில்
    {
        ittHandle_name = 0;
        ittHandle_filename = 0;
    }
#endif
}

/*static*/ Region::LocationExtraData* Region::LocationExtraData::init(மாறிலி Region::LocationStaticStorage& location)
{
    LocationExtraData** pLocationExtra = location.ppExtra;
    CV_DbgAssert(pLocationExtra);
    எனில் (*pLocationExtra == வெற்று)
    {
        cv::AutoLock lock(cv::getInitializationMutex());
        எனில் (*pLocationExtra == வெற்று)
        {
            *pLocationExtra = புதிய Region::LocationExtraData(location);
            TraceStorage* s = getTraceManager().trace_storage.get();
            எனில் (s)
            {
                TraceMessage செய்தி;
                செய்தி.formatlocation(location);
                s->put(செய்தி);
            }
        }
    }
    திரும்பு *pLocationExtra;
}


Region::Impl::Impl(TraceManagerThreadLocal& ctx, Region* parentRegion_, Region& region_, மாறிலி LocationStaticStorage& location_, int64 beginTimestamp_) :
    location(location_),
    region(region_),
    parentRegion(parentRegion_),
    threadID(ctx.threadID),
    global_region_id(++ctx.region_counter),
    beginTimestamp(beginTimestamp_),
    endTimestamp(0),
    direct_Children_மொத்தம்(0)
#ifdef OPENCV_WITH_ITT
    ,itt_id_registered(பொய்)
    ,itt_id(__itt_null)
#endif
{
    CV_DbgAssert(ctx.currentActiveRegion == parentRegion);
    region.pImpl = இது;

    registerRegion(ctx);

    enterRegion(ctx);
}

Region::Impl::~Impl()
{
#ifdef OPENCV_WITH_ITT
    எனில் (itt_id_registered)
    {
        CV_LOG_ITT(" Destroy ITT region: I=" << (வெறுமை*)இது);
        __itt_id_destroy(domain, itt_id);
        itt_id_registered = பொய்;
    }
#endif
    region.pImpl = வெற்று;
}

வெறுமை Region::Impl::enterRegion(TraceManagerThreadLocal& ctx)
{
    ctx.currentActiveRegion = &region;

    எனில் (location.flags & REGION_FLAG_FUNCTION)
    {
        எனில் ((location.flags & REGION_FLAG_APP_CODE) == 0)
        {
            ctx.regionDepthOpenCV++;
        }
        ctx.regionDepth++;
    }

    TraceStorage* s = ctx.getStorage();
    எனில் (s)
    {
        TraceMessage செய்தி;
        செய்தி.formatRegionEnter(region);
        s->put(செய்தி);
    }
#ifdef OPENCV_WITH_ITT
    எனில் (isITTEnabled())
    {
        __itt_id parentID = __itt_null;
        எனில் (param_ITT_registerParentScope && parentRegion && parentRegion->pImpl && parentRegion->pImpl->itt_id_registered && (location.flags & REGION_FLAG_REGION_FORCE) == 0)
            parentID = parentRegion->pImpl->itt_id;
        __itt_task_begin(domain, itt_id, parentID, (*location.ppExtra)->ittHandle_name);
    }
#endif
}

வெறுமை Region::Impl::leaveRegion(TraceManagerThreadLocal& ctx)
{
    int64 duration = endTimestamp - beginTimestamp; CV_UNUSED(duration);
    RegionStatistics result;
    ctx.stat.grab(result);
    ctx.totalSkippedEvents += result.currentSkippedRegions;
    CV_LOG(_spaces(ctx.getCurrentDepth()*4) << "leaveRegion(): " << (வெறுமை*)இது << " " << result);
#ifdef OPENCV_WITH_ITT
    எனில் (isITTEnabled())
    {
        எனில் (result.currentSkippedRegions)
        {
            __itt_metadata_add(domain, itt_id, __itt_string_handle_create("skipped trace entries"), __itt_metadata_u32, 1, &result.currentSkippedRegions);
        }
#ifdef HAVE_IPP
        எனில் (result.durationImplIPP)
            __itt_metadata_add(domain, itt_id, __itt_string_handle_create("tIPP"), __itt_metadata_u64, 1, &result.durationImplIPP);
#endif
#ifdef HAVE_OPENCL
        எனில் (result.durationImplOpenCL)
            __itt_metadata_add(domain, itt_id, __itt_string_handle_create("tOpenCL"), __itt_metadata_u64, 1, &result.durationImplOpenCL);
#endif
        __itt_task_end(domain);
    }
#endif
    TraceStorage* s = ctx.getStorage();
    எனில் (s)
    {
        TraceMessage செய்தி;
        செய்தி.formatRegionLeave(region, result);
        s->put(செய்தி);
    }

    எனில் (location.flags & REGION_FLAG_FUNCTION)
    {
        எனில் ((location.flags & REGION_FLAG_APP_CODE) == 0)
        {
            ctx.regionDepthOpenCV--;
        }
        ctx.regionDepth--;
    }

    ctx.currentActiveRegion = parentRegion;
}

வெறுமை Region::Impl::release()
{
    delete இது;
}

வெறுமை Region::Impl::registerRegion(TraceManagerThreadLocal& ctx)
{
#ifdef OPENCV_WITH_ITT
    எனில் (isITTEnabled())
    {
        எனில் (!itt_id_registered)
        {
            CV_LOG_ITT(" Register ITT region: I=" << (வெறுமை*)இது << " " << ctx.threadID << "-" << global_region_id);
#எனில் 1 // workaround for some ITT backends
            itt_id = __itt_id_make((வெறுமை*)(intptr_t)(((int64)(ctx.threadID + 1) << 32) | global_region_id), global_region_id);
#இல்லையெனில்
            itt_id = __itt_id_make((வெறுமை*)(intptr_t)(ctx.threadID + 1), global_region_id);
#endif
            __itt_id_create(domain, itt_id);
            itt_id_registered = உண்மை;
        }
    }
#இல்லையெனில்
    CV_UNUSED(ctx);
#endif
}

வெறுமை RegionStatisticsStatus::enableSkipMode(முழு depth)
{
    CV_DbgAssert(_skipDepth < 0);
    CV_LOG_SKIP(வெற்று, "SKIP-ENABLE: depth=" << depth);
    _skipDepth = depth;
}
வெறுமை RegionStatisticsStatus::checkResetSkipMode(முழு leaveDepth)
{
    எனில் (leaveDepth <= _skipDepth)
    {
        CV_LOG_SKIP(வெற்று, "SKIP-RESET: leaveDepth=" << leaveDepth << " skipDepth=" << _skipDepth);
        _skipDepth = -1;
    }
}

Region::Region(மாறிலி LocationStaticStorage& location) :
    pImpl(வெற்று),
    implFlags(0)
{
    // Checks:
    // - global enable flag
    // - parent region is disabled
    // - children count threshold
    // - region location
    // - depth (opencv nested calls)
    எனில் (!TraceManager::isActivated())
    {
        CV_LOG("Trace is disabled. Bailout");
        திரும்பு;
    }

    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    CV_LOG(_spaces(ctx.getCurrentDepth()*4) << "Region(): " << (வெறுமை*)இது << ": " << location.name);

    Region* parentRegion = ctx.அடுக்கு_உச்சி_Region();
    மாறிலி Region::LocationStaticStorage* parentLocation = ctx.அடுக்கு_உச்சி_Location();

    எனில் (location.flags & REGION_FLAG_REGION_NEXT)
    {
        எனில் (parentRegion && parentRegion->pImpl)
        {
            CV_DbgAssert((parentRegion->pImpl->location.flags & REGION_FLAG_FUNCTION) == 0);
            parentRegion->destroy(); parentRegion->implFlags = 0;
            parentRegion = ctx.அடுக்கு_உச்சி_Region();
            parentLocation = ctx.அடுக்கு_உச்சி_Location();
        }
    }

    முழு parentChildren = 0;
    எனில் (parentRegion && parentRegion->pImpl)
    {
        எனில் (parentLocation == வெற்று)
        {
            // parallel_for_body code path
            parentChildren = CV_XADD(&parentRegion->pImpl->direct_Children_மொத்தம், 1) + 1;
        }
        இல்லையெனில்
        {
            parentChildren = ++parentRegion->pImpl->direct_Children_மொத்தம்;
        }
    }

    int64 beginTimestamp = getTimestampNS();

    முழு currentDepth = ctx.getCurrentDepth() + 1;
    தேர்வு (location.flags & REGION_FLAG_IMPL_MASK)
    {
#ifdef HAVE_IPP
    நிலை REGION_FLAG_IMPL_IPP:
        எனில் (!ctx.stat_status.ignoreDepthImplIPP)
            ctx.stat_status.ignoreDepthImplIPP = currentDepth;
        நிறுத்து;
#endif
#ifdef HAVE_OPENCL
    நிலை REGION_FLAG_IMPL_OPENCL:
        எனில் (!ctx.stat_status.ignoreDepthImplOpenCL)
            ctx.stat_status.ignoreDepthImplOpenCL = currentDepth;
        நிறுத்து;
#endif
    இயல்பு:
        நிறுத்து;
    }

    ctx.அடுக்கு_தள்ளு(இது, &location, beginTimestamp);
    implFlags |= REGION_FLAG_NEED_அடுக்கு_எடு;

    எனில் ((location.flags & REGION_FLAG_REGION_FORCE) == 0)
    {
        எனில் (ctx.stat_status._skipDepth >= 0 && currentDepth > ctx.stat_status._skipDepth)
        {
            CV_LOG(_spaces(ctx.getCurrentDepth()*4) << "Parent region is disabled. Bailout");
            ctx.stat.currentSkippedRegions++;
            திரும்பு;
        }

        எனில் (param_maxRegionChildrenOpenCV > 0 && (location.flags & REGION_FLAG_APP_CODE) == 0 && parentLocation && (parentLocation->flags & REGION_FLAG_APP_CODE) == 0)
        {
            எனில் (parentChildren >= param_maxRegionChildrenOpenCV)
            {
                CV_LOG_TRACE_BAILOUT(வெற்று, _spaces(ctx.getCurrentDepth()*4) << "OpenCV parent region exceeds children count. Bailout");
                ctx.stat_status.enableSkipMode(currentDepth - 1);
                ctx.stat.currentSkippedRegions++;
                DEBUG_ONLY(ctx.dump_அடுக்கு(std::cout, பொய்));
                திரும்பு;
            }
        }
        எனில் (param_maxRegionChildren > 0 && parentChildren >= param_maxRegionChildren)
        {
            CV_LOG_TRACE_BAILOUT(வெற்று, _spaces(ctx.getCurrentDepth()*4) << "Parent region exceeds children count. Bailout");
            ctx.stat_status.enableSkipMode(currentDepth - 1);
            ctx.stat.currentSkippedRegions++;
            DEBUG_ONLY(ctx.dump_அடுக்கு(std::cout, பொய்));
            திரும்பு;
        }
    }

    LocationExtraData::init(location);

    எனில் ((*location.ppExtra)->global_location_id == 0)
    {
        CV_LOG_TRACE_BAILOUT(வெற்று, _spaces(ctx.getCurrentDepth()*4) << "Region location is disabled. Bailout");
        ctx.stat_status.enableSkipMode(currentDepth);
        ctx.stat.currentSkippedRegions++;
        திரும்பு;
    }

    எனில் (parentLocation && (parentLocation->flags & REGION_FLAG_SKIP_NESTED))
    {
        CV_LOG(_spaces(ctx.getCurrentDepth()*4) << "Parent region disables inner regions. Bailout");
        ctx.stat_status.enableSkipMode(currentDepth);
        ctx.stat.currentSkippedRegions++;
        திரும்பு;
    }

    எனில் (param_maxRegionDepthOpenCV)
    {
        எனில் ((location.flags & REGION_FLAG_APP_CODE) == 0)
        {
            எனில் (ctx.regionDepthOpenCV >= param_maxRegionDepthOpenCV)
            {
                CV_LOG(_spaces(ctx.getCurrentDepth()*4) << "OpenCV region depth is exceed = " << ctx.regionDepthOpenCV << ". Bailout");
                எனில் (ctx.stat.currentSkippedRegions == 0)
                {
                    DEBUG_ONLY(ctx.dump_அடுக்கு(std::cout, பொய்));
                }
                ctx.stat_status.enableSkipMode(currentDepth);
                ctx.stat.currentSkippedRegions++;
                திரும்பு;
            }
        }
    }

    புதிய Impl(ctx, parentRegion, *இது, location, beginTimestamp);
    CV_DbgAssert(pImpl != வெற்று);
    implFlags |= REGION_FLAG__ACTIVE;

    // parallel_for path
    எனில் (parentRegion && parentRegion->pImpl)
    {
        எனில் (parentLocation == வெற்று)
        {
            pImpl->direct_Children_மொத்தம் = parentChildren;
        }
    }
}

வெறுமை Region::destroy()
{
    CV_DbgAssert(implFlags != 0);

    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    CV_LOG(_spaces(ctx.getCurrentDepth()*4) << "Region::destruct(): " << (வெறுமை*)இது << " pImpl=" << pImpl << " implFlags=" << implFlags << ' ' << (ctx.அடுக்கு_உச்சி_Location() ? ctx.அடுக்கு_உச்சி_Location()->name : "<unknown>"));

    CV_DbgAssert(implFlags & REGION_FLAG_NEED_அடுக்கு_எடு);
    மாறிலி முழு currentDepth = ctx.getCurrentDepth(); CV_UNUSED(currentDepth);

    CV_LOG_CTX_STAT(வெற்று, _spaces(currentDepth*4) << ctx.stat << ' ' << ctx.stat_status);

    மாறிலி Region::LocationStaticStorage* location = ctx.அடுக்கு_உச்சி_Location();
    Impl::OptimizationPath myCodePath = Impl::CODE_PATH_PLAIN;
    எனில் (location)
    {
        தேர்வு (location->flags & REGION_FLAG_IMPL_MASK)
        {
#ifdef HAVE_IPP
        நிலை REGION_FLAG_IMPL_IPP:
            myCodePath = Impl::CODE_PATH_IPP;
            நிறுத்து;
#endif
#ifdef HAVE_OPENCL
        நிலை REGION_FLAG_IMPL_OPENCL:
            எனில் (param_synchronizeOpenCL && cv::ocl::isOpenCLActivated())
                cv::ocl::finish();
            myCodePath = Impl::CODE_PATH_OPENCL;
            நிறுத்து;
#endif
        இயல்பு:
            நிறுத்து;
        }
    }

    int64 endTimestamp = getTimestampNS();
    int64 duration = endTimestamp - ctx.அடுக்கு_உச்சி_Begin_Timestamp();

    தருக்க active = isActive();

    எனில் (active)
        ctx.stat.duration = duration;
    இல்லையெனில் எனில் (ctx.அடுக்கு.size() == ctx.parallel_for_அடுக்கு_size + 1)
        ctx.stat.duration += duration;

    தேர்வு (myCodePath) {
        நிலை Impl::CODE_PATH_PLAIN:
            // nothing
            நிறுத்து;
#ifdef HAVE_IPP
        நிலை Impl::CODE_PATH_IPP:
            எனில் (ctx.stat_status.ignoreDepthImplIPP == currentDepth)
            {
                ctx.stat.durationImplIPP += duration;
                ctx.stat_status.ignoreDepthImplIPP = 0;
            }
            இல்லையெனில் எனில் (active)
            {
                ctx.stat.durationImplIPP = duration;
            }
            நிறுத்து;
#endif
#ifdef HAVE_OPENCL
        நிலை Impl::CODE_PATH_OPENCL:
            எனில் (ctx.stat_status.ignoreDepthImplOpenCL == currentDepth)
            {
                ctx.stat.durationImplOpenCL += duration;
                ctx.stat_status.ignoreDepthImplOpenCL = 0;
            }
            இல்லையெனில் எனில் (active)
            {
                ctx.stat.durationImplOpenCL = duration;
            }
            நிறுத்து;
#endif
        இயல்பு:
            நிறுத்து;
    }

    எனில் (pImpl)
    {
        CV_DbgAssert((implFlags & (REGION_FLAG__ACTIVE | REGION_FLAG_NEED_அடுக்கு_எடு)) == (REGION_FLAG__ACTIVE | REGION_FLAG_NEED_அடுக்கு_எடு));
        CV_DbgAssert(ctx.அடுக்கு_உச்சி_Region() == இது);
        pImpl->endTimestamp = endTimestamp;
        pImpl->leaveRegion(ctx);
        pImpl->release();
        pImpl = வெற்று;
        DEBUG_ONLY(implFlags &= ~REGION_FLAG__ACTIVE);
    }
    இல்லையெனில்
    {
        CV_DbgAssert(ctx.stat_status._skipDepth <= currentDepth);
    }

    எனில் (implFlags & REGION_FLAG_NEED_அடுக்கு_எடு)
    {
        CV_DbgAssert(ctx.அடுக்கு_உச்சி_Region() == இது);
        ctx.அடுக்கு_எடு();
        ctx.stat_status.checkResetSkipMode(currentDepth);
        DEBUG_ONLY(implFlags &= ~REGION_FLAG_NEED_அடுக்கு_எடு);
    }
    CV_LOG_CTX_STAT(வெற்று, _spaces(currentDepth*4) << "===> " << ctx.stat << ' ' << ctx.stat_status);
}


TraceManagerThreadLocal::~TraceManagerThreadLocal()
{
}

வெறுமை TraceManagerThreadLocal::dump_அடுக்கு(std::ostream& out, தருக்க onlyFunctions) மாறிலி
{
    std::stringstream ss;
    std::deque<அடுக்கு_Entry>::const_iterator it = அடுக்கு.begin();
    std::deque<அடுக்கு_Entry>::const_iterator end = அடுக்கு.end();
    முழு depth = 0;
    க்கு (; it != end; ++it)
    {
        மாறிலி Region::LocationStaticStorage* location = it->location;
        எனில் (location)
        {
            எனில் (!onlyFunctions || (location->flags & REGION_FLAG_FUNCTION))
            {
                ss << _spaces(4*depth) << location->name << std::endl;
                depth++;
            }
        }
        இல்லையெனில்
        {
            ss << _spaces(4*depth) << "<unknown>" << std::endl;
            depth++;
        }
    }
    out << ss.str();
}

வகுப்பு AsyncTraceStorage CV_FINAL : பொது TraceStorage
{
    mutable std::ofstream out;
பொது:
    மாறிலி std::string name;

    AsyncTraceStorage(மாறிலி std::string& filename) :
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

    தருக்க put(மாறிலி TraceMessage& செய்தி) மாறிலி CV_OVERRIDE
    {
        எனில் (செய்தி.hasError)
            திரும்பு பொய்;
        out << செய்தி.buffer;
        //DEBUG_ONLY(std::flush(out)); // TODO configure flag
        திரும்பு உண்மை;
    }
};

வகுப்பு SyncTraceStorage CV_FINAL : பொது TraceStorage
{
    mutable std::ofstream out;
    mutable cv::Mutex mutex;
பொது:
    மாறிலி std::string name;

    SyncTraceStorage(மாறிலி std::string& filename) :
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

    தருக்க put(மாறிலி TraceMessage& செய்தி) மாறிலி CV_OVERRIDE
    {
        எனில் (செய்தி.hasError)
            திரும்பு பொய்;
        {
            cv::AutoLock l(mutex);
            out << செய்தி.buffer;
            std::flush(out); // TODO configure flag
        }
        திரும்பு உண்மை;
    }
};


TraceStorage* TraceManagerThreadLocal::getStorage() மாறிலி
{
    // TODO configuration option for stdout/single trace file
    எனில் (storage.empty())
    {
        TraceStorage* global = getTraceManager().trace_storage.get();
        எனில் (global)
        {
            மாறிலி std::string filepath = cv::format("%s-%03d.txt", getParameterTraceLocation().c_str(), threadID).c_str();
            TraceMessage செய்தி;
            மாறிலி எழுத்து* pos = strrchr(filepath.c_str(), '/'); // extract filename
#ifdef _WIN32
            எனில் (!pos)
                pos = strrchr(filepath.c_str(), '\\');
#endif
            எனில் (!pos)
                pos = filepath.c_str();
            இல்லையெனில்
                pos += 1; // fix to skip extra slash in filename beginning
            செய்தி.printf("#thread file: %s\n", pos);
            global->put(செய்தி);
            storage.reset(புதிய AsyncTraceStorage(filepath));
        }
    }
    திரும்பு storage.get();
}



நிலையான தருக்க activated = பொய்;
நிலையான தருக்க isInitialized = பொய்;

TraceManager::TraceManager()
{
    (வெறுமை)cv::getTimestampNS();

    isInitialized = உண்மை;
    CV_LOG("TraceManager ctor: " << (வெறுமை*)இது);

    CV_LOG("TraceManager configure()");
    activated = getParameterTraceEnable();

    எனில் (activated)
        trace_storage.reset(புதிய SyncTraceStorage(std::string(getParameterTraceLocation()) + ".txt"));

#ifdef OPENCV_WITH_ITT
    எனில் (isITTEnabled())
    {
        activated = உண்மை; // force trace pipeline activation (without OpenCV storage)
        __itt_region_begin(domain, __itt_null, __itt_null, __itt_string_handle_create("OpenCVTrace"));
    }
#endif
}
TraceManager::~TraceManager()
{
    CV_LOG("TraceManager dtor: " << (வெறுமை*)இது);

#ifdef OPENCV_WITH_ITT
    எனில் (isITTEnabled())
    {
        __itt_region_end(domain, __itt_null);
    }
#endif

    std::vector<TraceManagerThreadLocal*> threads_ctx;
    tls.gather(threads_ctx);
    size_t totalEvents = 0, totalSkippedEvents = 0;
    க்கு (size_t i = 0; i < threads_ctx.size(); i++)
    {
        TraceManagerThreadLocal* ctx = threads_ctx[i];
        எனில் (ctx)
        {
            totalEvents += ctx->region_counter;
            totalSkippedEvents += ctx->totalSkippedEvents;
        }
    }
    எனில் (totalEvents || activated)
    {
        CV_LOG_INFO(வெற்று, "Trace: Total events: " << totalEvents);
    }
    எனில் (totalSkippedEvents)
    {
        CV_LOG_WARNING(வெற்று, "Trace: Total skipped events: " << totalSkippedEvents);
    }

    // This is a global static object, so process starts shutdown here
    // Turn off trace
    cv::__termination = உண்மை; // also set in DllMain() notifications handler for DLL_PROCESS_DETACH
    activated = பொய்;
}

தருக்க TraceManager::isActivated()
{
    // Check if process starts shutdown, and set earlyExit to true
    // to prevent further instrumentation processing earlier.
    எனில் (cv::__termination)
    {
        activated = பொய்;
        திரும்பு பொய்;
    }

    எனில் (!isInitialized)
    {
        TraceManager& m = getTraceManager();
        CV_UNUSED(m); // TODO
    }

    திரும்பு activated;
}


நிலையான TraceManager* getTraceManagerCallOnce()
{
    நிலையான TraceManager globalInstance;
    திரும்பு &globalInstance;
}
TraceManager& getTraceManager()
{
    CV_SINGLETON_LAZY_INIT_REF(TraceManager, getTraceManagerCallOnce())
}

வெறுமை parallel_For_Set_வேர்_Region(மாறிலி Region& வேர்_Region, மாறிலி TraceManagerThreadLocal& வேர்_ctx)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();

    எனில் (ctx.dummy_அடுக்கு_உச்சி.region == &வேர்_Region) // already attached
        திரும்பு;

    CV_Assert(ctx.dummy_அடுக்கு_உச்சி.region == வெற்று);
    ctx.dummy_அடுக்கு_உச்சி = TraceManagerThreadLocal::அடுக்கு_Entry(const_cast<Region*>(&வேர்_Region), வெற்று, -1);

    எனில் (&ctx == &வேர்_ctx)
    {
        ctx.stat.grab(ctx.parallel_for_stat);
        ctx.parallel_for_stat_status = ctx.stat_status;
        ctx.parallel_for_அடுக்கு_size = ctx.அடுக்கு.size();
        திரும்பு;
    }

    CV_Assert(ctx.அடுக்கு.empty());

    ctx.currentActiveRegion = const_cast<Region*>(&வேர்_Region);

    ctx.regionDepth = வேர்_ctx.regionDepth;
    ctx.regionDepthOpenCV = வேர்_ctx.regionDepthOpenCV;

    ctx.parallel_for_அடுக்கு_size = 0;

    ctx.stat_status.propagateFrom(வேர்_ctx.stat_status);
}

வெறுமை parallelForAttachNestedRegion(மாறிலி Region& வேர்_Region)
{
    CV_UNUSED(வேர்_Region);
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();

    CV_DbgAssert(ctx.dummy_அடுக்கு_உச்சி.region == &வேர்_Region);

    Region* region = ctx.getCurrentActiveRegion();
    CV_LOG_PARALLEL(வெற்று, " PARALLEL_FOR: " << (வெறுமை*)region << " ==> " << &வேர்_Region);
    எனில் (!region)
        திரும்பு;

#ifdef OPENCV_WITH_ITT
    எனில் (!வேர்_Region.pImpl || !வேர்_Region.pImpl->itt_id_registered)
        திரும்பு;

    எனில் (!region->pImpl)
        திரும்பு;

    CV_LOG_PARALLEL(வெற்று, " PARALLEL_FOR ITT: " << (வெறுமை*)வேர்_Region.pImpl->itt_id.d1 << ":" << வேர்_Region.pImpl->itt_id.d2 << ":" << (வெறுமை*)வேர்_Region.pImpl->itt_id.d3 << " => "
                                 << (வெறுமை*)region->pImpl->itt_id.d1 << ":" << region->pImpl->itt_id.d2 << ":" << (வெறுமை*)region->pImpl->itt_id.d3);
    __itt_relation_add(domain, region->pImpl->itt_id, __itt_relation_is_child_of, வேர்_Region.pImpl->itt_id);
#endif
}

வெறுமை parallelForFinalize(மாறிலி Region& வேர்_Region)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();

    int64 endTimestamp = getTimestampNS();
    int64 duration = endTimestamp - ctx.அடுக்கு_உச்சி_Begin_Timestamp();
    CV_LOG_PARALLEL(வெற்று, "parallel_for duration: " << duration << " " << &வேர்_Region);

    std::vector<TraceManagerThreadLocal*> threads_ctx;
    getTraceManager().tls.gather(threads_ctx);
    RegionStatistics parallel_for_stat;
    க்கு (size_t i = 0; i < threads_ctx.size(); i++)
    {
        TraceManagerThreadLocal* child_ctx = threads_ctx[i];

        எனில் (child_ctx && child_ctx->அடுக்கு_உச்சி_Region() == &வேர்_Region)
        {
            CV_LOG_PARALLEL(வெற்று, "Thread=" << child_ctx->threadID << " " << child_ctx->stat);
            RegionStatistics child_stat;
            child_ctx->stat.grab(child_stat);
            parallel_for_stat.append(child_stat);
            எனில் (child_ctx != &ctx)
            {
                child_ctx->dummy_அடுக்கு_உச்சி = TraceManagerThreadLocal::அடுக்கு_Entry();
            }
            இல்லையெனில்
            {
                ctx.parallel_for_stat.grab(ctx.stat);
                ctx.stat_status = ctx.parallel_for_stat_status;
                child_ctx->dummy_அடுக்கு_உச்சி = TraceManagerThreadLocal::அடுக்கு_Entry();
            }
        }
    }

    தசம parallel_coeff = std::min(1.0f, duration / (தசம)(parallel_for_stat.duration));
    CV_LOG_PARALLEL(வெற்று, "parallel_coeff=" << 1.0f / parallel_coeff);
    CV_LOG_PARALLEL(வெற்று, parallel_for_stat);
    எனில் (parallel_coeff != 1.0f)
    {
        parallel_for_stat.multiply(parallel_coeff);
        CV_LOG_PARALLEL(வெற்று, parallel_for_stat);
    }
    parallel_for_stat.duration = 0;
    ctx.stat.append(parallel_for_stat);
    CV_LOG_PARALLEL(வெற்று, ctx.stat);
}

கட்டமைப்பு TraceArg::ExtraData
{
#ifdef OPENCV_WITH_ITT
    // Special fields for ITT
    __itt_string_handle* நிலையற்ற ittHandle_name;
#endif
    ExtraData(TraceManagerThreadLocal& ctx, மாறிலி TraceArg& arg)
    {
        CV_UNUSED(ctx); CV_UNUSED(arg);
#ifdef OPENCV_WITH_ITT
        எனில் (isITTEnabled())
        {
            // Caching is not required here, because there is builtin cache.
            // https://software.intel.com/en-us/node/544203:
            //     Consecutive calls to __itt_string_handle_create with the same name return the same value.
            ittHandle_name = __itt_string_handle_create(arg.name);
        }
        இல்லையெனில்
        {
            ittHandle_name = 0;
        }
#endif
    }
};

நிலையான வெறுமை initTraceArg(TraceManagerThreadLocal& ctx, மாறிலி TraceArg& arg)
{
    TraceArg::ExtraData** pExtra = arg.ppExtra;
    எனில் (*pExtra == வெற்று)
    {
        cv::AutoLock lock(cv::getInitializationMutex());
        எனில் (*pExtra == வெற்று)
        {
            *pExtra = புதிய TraceArg::ExtraData(ctx, arg);
        }
    }
}
வெறுமை traceArg(மாறிலி TraceArg& arg, மாறிலி எழுத்து* value)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    Region* region = ctx.getCurrentActiveRegion();
    எனில் (!region)
        திரும்பு;
    CV_Assert(region->pImpl);
    initTraceArg(ctx, arg);
    எனில் (!value)
        value = "<null>";
#ifdef OPENCV_WITH_ITT
    எனில் (isITTEnabled())
    {
        __itt_metadata_str_add(domain, region->pImpl->itt_id, (*arg.ppExtra)->ittHandle_name, value, strlen(value));
    }
#endif
}
வெறுமை traceArg(மாறிலி TraceArg& arg, முழு value)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    Region* region = ctx.getCurrentActiveRegion();
    எனில் (!region)
        திரும்பு;
    CV_Assert(region->pImpl);
    initTraceArg(ctx, arg);
#ifdef OPENCV_WITH_ITT
    எனில் (isITTEnabled())
    {
        __itt_metadata_add(domain, region->pImpl->itt_id, (*arg.ppExtra)->ittHandle_name, அளவு(முழு) == 4 ? __itt_metadata_s32 : __itt_metadata_s64, 1, &value);
    }
#இல்லையெனில்
    CV_UNUSED(value);
#endif
}
வெறுமை traceArg(மாறிலி TraceArg& arg, int64 value)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    Region* region = ctx.getCurrentActiveRegion();
    எனில் (!region)
        திரும்பு;
    CV_Assert(region->pImpl);
    initTraceArg(ctx, arg);
#ifdef OPENCV_WITH_ITT
    எனில் (isITTEnabled())
    {
        __itt_metadata_add(domain, region->pImpl->itt_id, (*arg.ppExtra)->ittHandle_name, __itt_metadata_s64, 1, &value);
    }
#இல்லையெனில்
    CV_UNUSED(value);
#endif
}
வெறுமை traceArg(மாறிலி TraceArg& arg, இரட்டை value)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    Region* region = ctx.getCurrentActiveRegion();
    எனில் (!region)
        திரும்பு;
    CV_Assert(region->pImpl);
    initTraceArg(ctx, arg);
#ifdef OPENCV_WITH_ITT
    எனில் (isITTEnabled())
    {
        __itt_metadata_add(domain, region->pImpl->itt_id, (*arg.ppExtra)->ittHandle_name, __itt_metadata_double, 1, &value);
    }
#இல்லையெனில்
    CV_UNUSED(value);
#endif
}

#இல்லையெனில்

Region::Region(மாறிலி LocationStaticStorage&) : pImpl(வெற்று), implFlags(0) {}
வெறுமை Region::destroy() {}

வெறுமை traceArg(மாறிலி TraceArg&, மாறிலி எழுத்து*) {}
வெறுமை traceArg(மாறிலி TraceArg&, முழு) {};
வெறுமை traceArg(மாறிலி TraceArg&, int64) {};
வெறுமை traceArg(மாறிலி TraceArg&, இரட்டை) {};

#endif

}}}} // namespace
