// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#അന്തര്ഭാവ "precomp.hpp"

#അന്തര്ഭാവ <opencv2/core/utils/trace.hpp>
#അന്തര്ഭാവ <opencv2/core/utils/trace.private.hpp>
#അന്തര്ഭാവ <opencv2/core/utils/configuration.private.hpp>

#അന്തര്ഭാവ <opencv2/core/opencl/ocl_defs.hpp>

#അന്തര്ഭാവ <cstdarg> // va_start

#അന്തര്ഭാവ <sstream>
#അന്തര്ഭാവ <ostream>
#അന്തര്ഭാവ <fstream>

#എങ്കിൽ 0
#define CV_LOG(...) CV_LOG_INFO(ശൂന്യ, __VA_ARGS__)
#അല്ലെങ്കിൽ
#define CV_LOG(...) {}
#endif

#എങ്കിൽ 0
#define CV_LOG_ITT(...) CV_LOG_INFO(ശൂന്യ, __VA_ARGS__)
#അല്ലെങ്കിൽ
#define CV_LOG_ITT(...) {}
#endif

#എങ്കിൽ 1
#define CV_LOG_TRACE_BAILOUT(tag, ...) CV_LOG_INFO(tag, __VA_ARGS__)
#അല്ലെങ്കിൽ
#define CV_LOG_TRACE_BAILOUT(...) {}
#endif

#എങ്കിൽ 0
#define CV_LOG_PARALLEL(tag, ...) CV_LOG_INFO(tag, __VA_ARGS__)
#അല്ലെങ്കിൽ
#define CV_LOG_PARALLEL(...) {}
#endif

#എങ്കിൽ 0
#define CV_LOG_CTX_STAT(tag, ...) CV_LOG_INFO(tag, __VA_ARGS__)
#അല്ലെങ്കിൽ
#define CV_LOG_CTX_STAT(...) {}
#endif

#എങ്കിൽ 0
#define CV_LOG_SKIP(tag, ...) CV_LOG_INFO(tag, __VA_ARGS__)
#അല്ലെങ്കിൽ
#define CV_LOG_SKIP(...) {}
#endif

നാമാകാശ cv {
നാമാകാശ utils {
നാമാകാശ trace {
നാമാകാശ details {

#ifdef OPENCV_TRACE

#ifdef _MSC_VER
#pragma warning(disable:4065) // switch statement contains 'default' but no 'case' labels
#endif

സ്ഥിതം തർക്കം getParameterTraceEnable()
{
    സ്ഥിതം തർക്കം param_traceEnable = utils::getConfigurationParameterBool("OPENCV_TRACE", അസത്യം);
    തിരികെ param_traceEnable;
}

// TODO lazy configuration flags
സ്ഥിതം പൂർണ്ണസംഖ്യ param_maxRegionDepthOpenCV = (പൂർണ്ണസംഖ്യ)utils::getConfigurationParameterSizeT("OPENCV_TRACE_DEPTH_OPENCV", 1);
സ്ഥിതം പൂർണ്ണസംഖ്യ param_maxRegionChildrenOpenCV = (പൂർണ്ണസംഖ്യ)utils::getConfigurationParameterSizeT("OPENCV_TRACE_MAX_CHILDREN_OPENCV", 1000);
സ്ഥിതം പൂർണ്ണസംഖ്യ param_maxRegionChildren = (പൂർണ്ണസംഖ്യ)utils::getConfigurationParameterSizeT("OPENCV_TRACE_MAX_CHILDREN", 10000);

സ്ഥിതം സ്ഥിരം cv::String& getParameterTraceLocation()
{
    സ്ഥിതം cv::String param_traceLocation = utils::getConfigurationParameterString("OPENCV_TRACE_LOCATION", "OpenCVTrace");
    തിരികെ param_traceLocation;
}

#ifdef HAVE_OPENCL
സ്ഥിതം തർക്കം param_synchronizeOpenCL = utils::getConfigurationParameterBool("OPENCV_TRACE_SYNC_OPENCL", അസത്യം);
#endif

#ifdef OPENCV_WITH_ITT
സ്ഥിതം തർക്കം param_ITT_registerParentScope = utils::getConfigurationParameterBool("OPENCV_TRACE_ITT_PARENT", അസത്യം);
#endif

സ്ഥിതം സ്ഥിരം അക്ഷരം* _spaces(പൂർണ്ണസംഖ്യ എണ്ണം_2)
{
    സ്ഥിതം സ്ഥിരം അക്ഷരം buf[64] =
"                                                               ";
    തിരികെ &buf[63 - (എണ്ണം_2 & 63)];
}

/**
 * Text-based trace messages
 */
വർഗ്ഗം TraceMessage
{
പൊതു:
    അക്ഷരം buffer[1024];
    size_t len;
    തർക്കം hasError;

    TraceMessage() :
        len(0),
        hasError(അസത്യം)
    {}

    തർക്കം printf(സ്ഥിരം അക്ഷരം* format, ...)
    {
        അക്ഷരം* buf = &buffer[len];
        size_t sz = വലുപ്പം(buffer) - len;
        va_list ap;
        va_start(ap, format);
        പൂർണ്ണസംഖ്യ n = cv_vsnprintf(buf, (പൂർണ്ണസംഖ്യ)sz, format, ap);
        va_end(ap);
        എങ്കിൽ (n < 0 || (size_t)n > sz)
        {
            hasError = സത്യം;
            തിരികെ അസത്യം;
        }
        len += n;
        തിരികെ സത്യം;
    }

    തർക്കം formatlocation(സ്ഥിരം Region::LocationStaticStorage& location)
    {
        തിരികെ ഇത്->printf("l,%lld,\"%s\",%d,\"%s\",0x%llX\n",
                (long long പൂർണ്ണസംഖ്യ)(*location.ppExtra)->global_location_id,
                location.filename,
                location.വരി,
                location.name,
                (long long പൂർണ്ണസംഖ്യ)(location.flags & ~0xF0000000));
    }
    തർക്കം formatRegionEnter(സ്ഥിരം Region& region)
    {
        തർക്കം ok = ഇത്->printf("b,%d,%lld,%lld,%lld",
                (പൂർണ്ണസംഖ്യ)region.pImpl->threadID,
                (long long പൂർണ്ണസംഖ്യ)region.pImpl->beginTimestamp,
                (long long പൂർണ്ണസംഖ്യ)((*region.pImpl->location.ppExtra)->global_location_id),
                (long long പൂർണ്ണസംഖ്യ)region.pImpl->global_region_id);
        എങ്കിൽ (region.pImpl->parentRegion && region.pImpl->parentRegion->pImpl)
        {
            എങ്കിൽ (region.pImpl->parentRegion->pImpl->threadID != region.pImpl->threadID)
                ok &= ഇത്->printf(",parentThread=%d,parent=%lld",
                        (പൂർണ്ണസംഖ്യ)region.pImpl->parentRegion->pImpl->threadID,
                        (long long പൂർണ്ണസംഖ്യ)region.pImpl->parentRegion->pImpl->global_region_id);
        }
        ok &= ഇത്->printf("\n");
        തിരികെ ok;
    }
    തർക്കം formatRegionLeave(സ്ഥിരം Region& region, സ്ഥിരം RegionStatistics& result)
    {
        CV_DbgAssert(region.pImpl->endTimestamp - region.pImpl->beginTimestamp == result.duration);
        തർക്കം ok = ഇത്->printf("e,%d,%lld,%lld,%lld,%lld",
                (പൂർണ്ണസംഖ്യ)region.pImpl->threadID,
                (long long പൂർണ്ണസംഖ്യ)region.pImpl->endTimestamp,
                (long long പൂർണ്ണസംഖ്യ)(*region.pImpl->location.ppExtra)->global_location_id,
                (long long പൂർണ്ണസംഖ്യ)region.pImpl->global_region_id,
                (long long പൂർണ്ണസംഖ്യ)result.duration);
        എങ്കിൽ (result.currentSkippedRegions)
            ok &= ഇത്->printf(",skip=%d", (പൂർണ്ണസംഖ്യ)result.currentSkippedRegions);
#ifdef HAVE_IPP
        എങ്കിൽ (result.durationImplIPP)
            ok &= ഇത്->printf(",tIPP=%lld", (long long പൂർണ്ണസംഖ്യ)result.durationImplIPP);
#endif
#ifdef HAVE_OPENCL
        എങ്കിൽ (result.durationImplOpenCL)
            ok &= ഇത്->printf(",tOCL=%lld", (long long പൂർണ്ണസംഖ്യ)result.durationImplOpenCL);
#endif
        ok &= ഇത്->printf("\n");
        തിരികെ ok;
    }
    തർക്കം recordRegionArg(സ്ഥിരം Region& region, സ്ഥിരം TraceArg& arg, സ്ഥിരം അക്ഷരം* value)
    {
        തിരികെ ഇത്->printf("a,%d,%lld,%lld,\"%s\",\"%s\"\n",
                region.pImpl->threadID,
                (long long പൂർണ്ണസംഖ്യ)region.pImpl->beginTimestamp,
                (long long പൂർണ്ണസംഖ്യ)region.pImpl->global_region_id,
                arg.name,
                value);
    }
};


#ifdef OPENCV_WITH_ITT
സ്ഥിതം __itt_domain* domain = ശൂന്യ;

സ്ഥിതം തർക്കം isITTEnabled()
{
    സ്ഥിതം അസ്ഥിര തർക്കം isInitialized = അസത്യം;
    സ്ഥിതം തർക്കം isEnabled = അസത്യം;
    എങ്കിൽ (!isInitialized)
    {
        cv::AutoLock lock(cv::getInitializationMutex());
        എങ്കിൽ (!isInitialized)
        {
            തർക്കം param_traceITTEnable = utils::getConfigurationParameterBool("OPENCV_TRACE_ITT_ENABLE", സത്യം);
            എങ്കിൽ (param_traceITTEnable)
            {
                isEnabled = !!(__itt_api_version());
                CV_LOG_ITT("ITT is " << (isEnabled ? "enabled" : "disabled"));
                domain = __itt_domain_create("OpenCVTrace");
            }
            അല്ലെങ്കിൽ
            {
                CV_LOG_ITT("ITT is disabled through OpenCV parameter");
                isEnabled = അസത്യം;
            }
            isInitialized = സത്യം;
        }
    }
    തിരികെ isEnabled;
}
#endif


Region::LocationExtraData::LocationExtraData(സ്ഥിരം LocationStaticStorage& location)
{
    CV_UNUSED(location);
    സ്ഥിതം പൂർണ്ണസംഖ്യ g_location_id_counter = 0;
    global_location_id = CV_XADD(&g_location_id_counter, 1) + 1;
    CV_LOG("Register location: " << global_location_id << " (" << (ശൂന്യം*)&location << ")"
            << std::endl << "    file: " << location.filename
            << std::endl << "    line: " << location.വരി
            << std::endl << "    name: " << location.name);
#ifdef OPENCV_WITH_ITT
    എങ്കിൽ (isITTEnabled())
    {
        // Caching is not required here, because there is builtin cache.
        // https://software.intel.com/en-us/node/544203:
        //     Consecutive calls to __itt_string_handle_create with the same name return the same value.
        ittHandle_name = __itt_string_handle_create(location.name);
        ittHandle_filename = __itt_string_handle_create(location.filename);
    }
    അല്ലെങ്കിൽ
    {
        ittHandle_name = 0;
        ittHandle_filename = 0;
    }
#endif
}

/*static*/ Region::LocationExtraData* Region::LocationExtraData::init(സ്ഥിരം Region::LocationStaticStorage& location)
{
    LocationExtraData** pLocationExtra = location.ppExtra;
    CV_DbgAssert(pLocationExtra);
    എങ്കിൽ (*pLocationExtra == ശൂന്യ)
    {
        cv::AutoLock lock(cv::getInitializationMutex());
        എങ്കിൽ (*pLocationExtra == ശൂന്യ)
        {
            *pLocationExtra = പുതിയ Region::LocationExtraData(location);
            TraceStorage* s = getTraceManager().trace_storage.get();
            എങ്കിൽ (s)
            {
                TraceMessage സന്ദേശം;
                സന്ദേശം.formatlocation(location);
                s->put(സന്ദേശം);
            }
        }
    }
    തിരികെ *pLocationExtra;
}


Region::Impl::Impl(TraceManagerThreadLocal& ctx, Region* parentRegion_, Region& region_, സ്ഥിരം LocationStaticStorage& location_, int64 beginTimestamp_) :
    location(location_),
    region(region_),
    parentRegion(parentRegion_),
    threadID(ctx.threadID),
    global_region_id(++ctx.region_counter),
    beginTimestamp(beginTimestamp_),
    endTimestamp(0),
    direct_Children_എണ്ണം(0)
#ifdef OPENCV_WITH_ITT
    ,itt_id_registered(അസത്യം)
    ,itt_id(__itt_null)
#endif
{
    CV_DbgAssert(ctx.currentActiveRegion == parentRegion);
    region.pImpl = ഇത്;

    registerRegion(ctx);

    enterRegion(ctx);
}

Region::Impl::~Impl()
{
#ifdef OPENCV_WITH_ITT
    എങ്കിൽ (itt_id_registered)
    {
        CV_LOG_ITT(" Destroy ITT region: I=" << (ശൂന്യം*)ഇത്);
        __itt_id_destroy(domain, itt_id);
        itt_id_registered = അസത്യം;
    }
#endif
    region.pImpl = ശൂന്യ;
}

ശൂന്യം Region::Impl::enterRegion(TraceManagerThreadLocal& ctx)
{
    ctx.currentActiveRegion = &region;

    എങ്കിൽ (location.flags & REGION_FLAG_FUNCTION)
    {
        എങ്കിൽ ((location.flags & REGION_FLAG_APP_CODE) == 0)
        {
            ctx.regionDepthOpenCV++;
        }
        ctx.regionDepth++;
    }

    TraceStorage* s = ctx.getStorage();
    എങ്കിൽ (s)
    {
        TraceMessage സന്ദേശം;
        സന്ദേശം.formatRegionEnter(region);
        s->put(സന്ദേശം);
    }
#ifdef OPENCV_WITH_ITT
    എങ്കിൽ (isITTEnabled())
    {
        __itt_id parentID = __itt_null;
        എങ്കിൽ (param_ITT_registerParentScope && parentRegion && parentRegion->pImpl && parentRegion->pImpl->itt_id_registered && (location.flags & REGION_FLAG_REGION_FORCE) == 0)
            parentID = parentRegion->pImpl->itt_id;
        __itt_task_begin(domain, itt_id, parentID, (*location.ppExtra)->ittHandle_name);
    }
#endif
}

ശൂന്യം Region::Impl::leaveRegion(TraceManagerThreadLocal& ctx)
{
    int64 duration = endTimestamp - beginTimestamp; CV_UNUSED(duration);
    RegionStatistics result;
    ctx.stat.grab(result);
    ctx.totalSkippedEvents += result.currentSkippedRegions;
    CV_LOG(_spaces(ctx.getCurrentDepth()*4) << "leaveRegion(): " << (ശൂന്യം*)ഇത് << " " << result);
#ifdef OPENCV_WITH_ITT
    എങ്കിൽ (isITTEnabled())
    {
        എങ്കിൽ (result.currentSkippedRegions)
        {
            __itt_metadata_add(domain, itt_id, __itt_string_handle_create("skipped trace entries"), __itt_metadata_u32, 1, &result.currentSkippedRegions);
        }
#ifdef HAVE_IPP
        എങ്കിൽ (result.durationImplIPP)
            __itt_metadata_add(domain, itt_id, __itt_string_handle_create("tIPP"), __itt_metadata_u64, 1, &result.durationImplIPP);
#endif
#ifdef HAVE_OPENCL
        എങ്കിൽ (result.durationImplOpenCL)
            __itt_metadata_add(domain, itt_id, __itt_string_handle_create("tOpenCL"), __itt_metadata_u64, 1, &result.durationImplOpenCL);
#endif
        __itt_task_end(domain);
    }
#endif
    TraceStorage* s = ctx.getStorage();
    എങ്കിൽ (s)
    {
        TraceMessage സന്ദേശം;
        സന്ദേശം.formatRegionLeave(region, result);
        s->put(സന്ദേശം);
    }

    എങ്കിൽ (location.flags & REGION_FLAG_FUNCTION)
    {
        എങ്കിൽ ((location.flags & REGION_FLAG_APP_CODE) == 0)
        {
            ctx.regionDepthOpenCV--;
        }
        ctx.regionDepth--;
    }

    ctx.currentActiveRegion = parentRegion;
}

ശൂന്യം Region::Impl::release()
{
    delete ഇത്;
}

ശൂന്യം Region::Impl::registerRegion(TraceManagerThreadLocal& ctx)
{
#ifdef OPENCV_WITH_ITT
    എങ്കിൽ (isITTEnabled())
    {
        എങ്കിൽ (!itt_id_registered)
        {
            CV_LOG_ITT(" Register ITT region: I=" << (ശൂന്യം*)ഇത് << " " << ctx.threadID << "-" << global_region_id);
#എങ്കിൽ 1 // workaround for some ITT backends
            itt_id = __itt_id_make((ശൂന്യം*)(intptr_t)(((int64)(ctx.threadID + 1) << 32) | global_region_id), global_region_id);
#അല്ലെങ്കിൽ
            itt_id = __itt_id_make((ശൂന്യം*)(intptr_t)(ctx.threadID + 1), global_region_id);
#endif
            __itt_id_create(domain, itt_id);
            itt_id_registered = സത്യം;
        }
    }
#അല്ലെങ്കിൽ
    CV_UNUSED(ctx);
#endif
}

ശൂന്യം RegionStatisticsStatus::enableSkipMode(പൂർണ്ണസംഖ്യ depth)
{
    CV_DbgAssert(_skipDepth < 0);
    CV_LOG_SKIP(ശൂന്യ, "SKIP-ENABLE: depth=" << depth);
    _skipDepth = depth;
}
ശൂന്യം RegionStatisticsStatus::checkResetSkipMode(പൂർണ്ണസംഖ്യ leaveDepth)
{
    എങ്കിൽ (leaveDepth <= _skipDepth)
    {
        CV_LOG_SKIP(ശൂന്യ, "SKIP-RESET: leaveDepth=" << leaveDepth << " skipDepth=" << _skipDepth);
        _skipDepth = -1;
    }
}

Region::Region(സ്ഥിരം LocationStaticStorage& location) :
    pImpl(ശൂന്യ),
    implFlags(0)
{
    // Checks:
    // - global enable flag
    // - parent region is disabled
    // - children count threshold
    // - region location
    // - depth (opencv nested calls)
    എങ്കിൽ (!TraceManager::isActivated())
    {
        CV_LOG("Trace is disabled. Bailout");
        തിരികെ;
    }

    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    CV_LOG(_spaces(ctx.getCurrentDepth()*4) << "Region(): " << (ശൂന്യം*)ഇത് << ": " << location.name);

    Region* parentRegion = ctx.അട്ടി_മുകൾ_Region();
    സ്ഥിരം Region::LocationStaticStorage* parentLocation = ctx.അട്ടി_മുകൾ_Location();

    എങ്കിൽ (location.flags & REGION_FLAG_REGION_NEXT)
    {
        എങ്കിൽ (parentRegion && parentRegion->pImpl)
        {
            CV_DbgAssert((parentRegion->pImpl->location.flags & REGION_FLAG_FUNCTION) == 0);
            parentRegion->destroy(); parentRegion->implFlags = 0;
            parentRegion = ctx.അട്ടി_മുകൾ_Region();
            parentLocation = ctx.അട്ടി_മുകൾ_Location();
        }
    }

    പൂർണ്ണസംഖ്യ parentChildren = 0;
    എങ്കിൽ (parentRegion && parentRegion->pImpl)
    {
        എങ്കിൽ (parentLocation == ശൂന്യ)
        {
            // parallel_for_body code path
            parentChildren = CV_XADD(&parentRegion->pImpl->direct_Children_എണ്ണം, 1) + 1;
        }
        അല്ലെങ്കിൽ
        {
            parentChildren = ++parentRegion->pImpl->direct_Children_എണ്ണം;
        }
    }

    int64 beginTimestamp = getTimestampNS();

    പൂർണ്ണസംഖ്യ currentDepth = ctx.getCurrentDepth() + 1;
    തിരഞ്ഞെടുപ്പ് (location.flags & REGION_FLAG_IMPL_MASK)
    {
#ifdef HAVE_IPP
    അവസ്ഥ REGION_FLAG_IMPL_IPP:
        എങ്കിൽ (!ctx.stat_status.ignoreDepthImplIPP)
            ctx.stat_status.ignoreDepthImplIPP = currentDepth;
        നിർത്തുക;
#endif
#ifdef HAVE_OPENCL
    അവസ്ഥ REGION_FLAG_IMPL_OPENCL:
        എങ്കിൽ (!ctx.stat_status.ignoreDepthImplOpenCL)
            ctx.stat_status.ignoreDepthImplOpenCL = currentDepth;
        നിർത്തുക;
#endif
    സ്ഥിരസ്ഥിതി:
        നിർത്തുക;
    }

    ctx.അട്ടി_തള്ളുക(ഇത്, &location, beginTimestamp);
    implFlags |= REGION_FLAG_NEED_അട്ടി_എടുക്കുക;

    എങ്കിൽ ((location.flags & REGION_FLAG_REGION_FORCE) == 0)
    {
        എങ്കിൽ (ctx.stat_status._skipDepth >= 0 && currentDepth > ctx.stat_status._skipDepth)
        {
            CV_LOG(_spaces(ctx.getCurrentDepth()*4) << "Parent region is disabled. Bailout");
            ctx.stat.currentSkippedRegions++;
            തിരികെ;
        }

        എങ്കിൽ (param_maxRegionChildrenOpenCV > 0 && (location.flags & REGION_FLAG_APP_CODE) == 0 && parentLocation && (parentLocation->flags & REGION_FLAG_APP_CODE) == 0)
        {
            എങ്കിൽ (parentChildren >= param_maxRegionChildrenOpenCV)
            {
                CV_LOG_TRACE_BAILOUT(ശൂന്യ, _spaces(ctx.getCurrentDepth()*4) << "OpenCV parent region exceeds children count. Bailout");
                ctx.stat_status.enableSkipMode(currentDepth - 1);
                ctx.stat.currentSkippedRegions++;
                DEBUG_ONLY(ctx.dump_അട്ടി(std::cout, അസത്യം));
                തിരികെ;
            }
        }
        എങ്കിൽ (param_maxRegionChildren > 0 && parentChildren >= param_maxRegionChildren)
        {
            CV_LOG_TRACE_BAILOUT(ശൂന്യ, _spaces(ctx.getCurrentDepth()*4) << "Parent region exceeds children count. Bailout");
            ctx.stat_status.enableSkipMode(currentDepth - 1);
            ctx.stat.currentSkippedRegions++;
            DEBUG_ONLY(ctx.dump_അട്ടി(std::cout, അസത്യം));
            തിരികെ;
        }
    }

    LocationExtraData::init(location);

    എങ്കിൽ ((*location.ppExtra)->global_location_id == 0)
    {
        CV_LOG_TRACE_BAILOUT(ശൂന്യ, _spaces(ctx.getCurrentDepth()*4) << "Region location is disabled. Bailout");
        ctx.stat_status.enableSkipMode(currentDepth);
        ctx.stat.currentSkippedRegions++;
        തിരികെ;
    }

    എങ്കിൽ (parentLocation && (parentLocation->flags & REGION_FLAG_SKIP_NESTED))
    {
        CV_LOG(_spaces(ctx.getCurrentDepth()*4) << "Parent region disables inner regions. Bailout");
        ctx.stat_status.enableSkipMode(currentDepth);
        ctx.stat.currentSkippedRegions++;
        തിരികെ;
    }

    എങ്കിൽ (param_maxRegionDepthOpenCV)
    {
        എങ്കിൽ ((location.flags & REGION_FLAG_APP_CODE) == 0)
        {
            എങ്കിൽ (ctx.regionDepthOpenCV >= param_maxRegionDepthOpenCV)
            {
                CV_LOG(_spaces(ctx.getCurrentDepth()*4) << "OpenCV region depth is exceed = " << ctx.regionDepthOpenCV << ". Bailout");
                എങ്കിൽ (ctx.stat.currentSkippedRegions == 0)
                {
                    DEBUG_ONLY(ctx.dump_അട്ടി(std::cout, അസത്യം));
                }
                ctx.stat_status.enableSkipMode(currentDepth);
                ctx.stat.currentSkippedRegions++;
                തിരികെ;
            }
        }
    }

    പുതിയ Impl(ctx, parentRegion, *ഇത്, location, beginTimestamp);
    CV_DbgAssert(pImpl != ശൂന്യ);
    implFlags |= REGION_FLAG__ACTIVE;

    // parallel_for path
    എങ്കിൽ (parentRegion && parentRegion->pImpl)
    {
        എങ്കിൽ (parentLocation == ശൂന്യ)
        {
            pImpl->direct_Children_എണ്ണം = parentChildren;
        }
    }
}

ശൂന്യം Region::destroy()
{
    CV_DbgAssert(implFlags != 0);

    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    CV_LOG(_spaces(ctx.getCurrentDepth()*4) << "Region::destruct(): " << (ശൂന്യം*)ഇത് << " pImpl=" << pImpl << " implFlags=" << implFlags << ' ' << (ctx.അട്ടി_മുകൾ_Location() ? ctx.അട്ടി_മുകൾ_Location()->name : "<unknown>"));

    CV_DbgAssert(implFlags & REGION_FLAG_NEED_അട്ടി_എടുക്കുക);
    സ്ഥിരം പൂർണ്ണസംഖ്യ currentDepth = ctx.getCurrentDepth(); CV_UNUSED(currentDepth);

    CV_LOG_CTX_STAT(ശൂന്യ, _spaces(currentDepth*4) << ctx.stat << ' ' << ctx.stat_status);

    സ്ഥിരം Region::LocationStaticStorage* location = ctx.അട്ടി_മുകൾ_Location();
    Impl::OptimizationPath myCodePath = Impl::CODE_PATH_PLAIN;
    എങ്കിൽ (location)
    {
        തിരഞ്ഞെടുപ്പ് (location->flags & REGION_FLAG_IMPL_MASK)
        {
#ifdef HAVE_IPP
        അവസ്ഥ REGION_FLAG_IMPL_IPP:
            myCodePath = Impl::CODE_PATH_IPP;
            നിർത്തുക;
#endif
#ifdef HAVE_OPENCL
        അവസ്ഥ REGION_FLAG_IMPL_OPENCL:
            എങ്കിൽ (param_synchronizeOpenCL && cv::ocl::isOpenCLActivated())
                cv::ocl::finish();
            myCodePath = Impl::CODE_PATH_OPENCL;
            നിർത്തുക;
#endif
        സ്ഥിരസ്ഥിതി:
            നിർത്തുക;
        }
    }

    int64 endTimestamp = getTimestampNS();
    int64 duration = endTimestamp - ctx.അട്ടി_മുകൾ_Begin_Timestamp();

    തർക്കം active = isActive();

    എങ്കിൽ (active)
        ctx.stat.duration = duration;
    അല്ലെങ്കിൽ എങ്കിൽ (ctx.അട്ടി.size() == ctx.parallel_for_അട്ടി_size + 1)
        ctx.stat.duration += duration;

    തിരഞ്ഞെടുപ്പ് (myCodePath) {
        അവസ്ഥ Impl::CODE_PATH_PLAIN:
            // nothing
            നിർത്തുക;
#ifdef HAVE_IPP
        അവസ്ഥ Impl::CODE_PATH_IPP:
            എങ്കിൽ (ctx.stat_status.ignoreDepthImplIPP == currentDepth)
            {
                ctx.stat.durationImplIPP += duration;
                ctx.stat_status.ignoreDepthImplIPP = 0;
            }
            അല്ലെങ്കിൽ എങ്കിൽ (active)
            {
                ctx.stat.durationImplIPP = duration;
            }
            നിർത്തുക;
#endif
#ifdef HAVE_OPENCL
        അവസ്ഥ Impl::CODE_PATH_OPENCL:
            എങ്കിൽ (ctx.stat_status.ignoreDepthImplOpenCL == currentDepth)
            {
                ctx.stat.durationImplOpenCL += duration;
                ctx.stat_status.ignoreDepthImplOpenCL = 0;
            }
            അല്ലെങ്കിൽ എങ്കിൽ (active)
            {
                ctx.stat.durationImplOpenCL = duration;
            }
            നിർത്തുക;
#endif
        സ്ഥിരസ്ഥിതി:
            നിർത്തുക;
    }

    എങ്കിൽ (pImpl)
    {
        CV_DbgAssert((implFlags & (REGION_FLAG__ACTIVE | REGION_FLAG_NEED_അട്ടി_എടുക്കുക)) == (REGION_FLAG__ACTIVE | REGION_FLAG_NEED_അട്ടി_എടുക്കുക));
        CV_DbgAssert(ctx.അട്ടി_മുകൾ_Region() == ഇത്);
        pImpl->endTimestamp = endTimestamp;
        pImpl->leaveRegion(ctx);
        pImpl->release();
        pImpl = ശൂന്യ;
        DEBUG_ONLY(implFlags &= ~REGION_FLAG__ACTIVE);
    }
    അല്ലെങ്കിൽ
    {
        CV_DbgAssert(ctx.stat_status._skipDepth <= currentDepth);
    }

    എങ്കിൽ (implFlags & REGION_FLAG_NEED_അട്ടി_എടുക്കുക)
    {
        CV_DbgAssert(ctx.അട്ടി_മുകൾ_Region() == ഇത്);
        ctx.അട്ടി_എടുക്കുക();
        ctx.stat_status.checkResetSkipMode(currentDepth);
        DEBUG_ONLY(implFlags &= ~REGION_FLAG_NEED_അട്ടി_എടുക്കുക);
    }
    CV_LOG_CTX_STAT(ശൂന്യ, _spaces(currentDepth*4) << "===> " << ctx.stat << ' ' << ctx.stat_status);
}


TraceManagerThreadLocal::~TraceManagerThreadLocal()
{
}

ശൂന്യം TraceManagerThreadLocal::dump_അട്ടി(std::ostream& out, തർക്കം onlyFunctions) സ്ഥിരം
{
    std::stringstream ss;
    std::deque<അട്ടി_Entry>::const_iterator it = അട്ടി.begin();
    std::deque<അട്ടി_Entry>::const_iterator end = അട്ടി.end();
    പൂർണ്ണസംഖ്യ depth = 0;
    വേണ്ടി (; it != end; ++it)
    {
        സ്ഥിരം Region::LocationStaticStorage* location = it->location;
        എങ്കിൽ (location)
        {
            എങ്കിൽ (!onlyFunctions || (location->flags & REGION_FLAG_FUNCTION))
            {
                ss << _spaces(4*depth) << location->name << std::endl;
                depth++;
            }
        }
        അല്ലെങ്കിൽ
        {
            ss << _spaces(4*depth) << "<unknown>" << std::endl;
            depth++;
        }
    }
    out << ss.str();
}

വർഗ്ഗം AsyncTraceStorage CV_FINAL : പൊതു TraceStorage
{
    mutable std::ofstream out;
പൊതു:
    സ്ഥിരം std::string name;

    AsyncTraceStorage(സ്ഥിരം std::string& filename) :
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

    തർക്കം put(സ്ഥിരം TraceMessage& സന്ദേശം) സ്ഥിരം CV_OVERRIDE
    {
        എങ്കിൽ (സന്ദേശം.hasError)
            തിരികെ അസത്യം;
        out << സന്ദേശം.buffer;
        //DEBUG_ONLY(std::flush(out)); // TODO configure flag
        തിരികെ സത്യം;
    }
};

വർഗ്ഗം SyncTraceStorage CV_FINAL : പൊതു TraceStorage
{
    mutable std::ofstream out;
    mutable cv::Mutex mutex;
പൊതു:
    സ്ഥിരം std::string name;

    SyncTraceStorage(സ്ഥിരം std::string& filename) :
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

    തർക്കം put(സ്ഥിരം TraceMessage& സന്ദേശം) സ്ഥിരം CV_OVERRIDE
    {
        എങ്കിൽ (സന്ദേശം.hasError)
            തിരികെ അസത്യം;
        {
            cv::AutoLock l(mutex);
            out << സന്ദേശം.buffer;
            std::flush(out); // TODO configure flag
        }
        തിരികെ സത്യം;
    }
};


TraceStorage* TraceManagerThreadLocal::getStorage() സ്ഥിരം
{
    // TODO configuration option for stdout/single trace file
    എങ്കിൽ (storage.empty())
    {
        TraceStorage* global = getTraceManager().trace_storage.get();
        എങ്കിൽ (global)
        {
            സ്ഥിരം std::string filepath = cv::format("%s-%03d.txt", getParameterTraceLocation().c_str(), threadID).c_str();
            TraceMessage സന്ദേശം;
            സ്ഥിരം അക്ഷരം* pos = strrchr(filepath.c_str(), '/'); // extract filename
#ifdef _WIN32
            എങ്കിൽ (!pos)
                pos = strrchr(filepath.c_str(), '\\');
#endif
            എങ്കിൽ (!pos)
                pos = filepath.c_str();
            അല്ലെങ്കിൽ
                pos += 1; // fix to skip extra slash in filename beginning
            സന്ദേശം.printf("#thread file: %s\n", pos);
            global->put(സന്ദേശം);
            storage.reset(പുതിയ AsyncTraceStorage(filepath));
        }
    }
    തിരികെ storage.get();
}



സ്ഥിതം തർക്കം activated = അസത്യം;
സ്ഥിതം തർക്കം isInitialized = അസത്യം;

TraceManager::TraceManager()
{
    (ശൂന്യം)cv::getTimestampNS();

    isInitialized = സത്യം;
    CV_LOG("TraceManager ctor: " << (ശൂന്യം*)ഇത്);

    CV_LOG("TraceManager configure()");
    activated = getParameterTraceEnable();

    എങ്കിൽ (activated)
        trace_storage.reset(പുതിയ SyncTraceStorage(std::string(getParameterTraceLocation()) + ".txt"));

#ifdef OPENCV_WITH_ITT
    എങ്കിൽ (isITTEnabled())
    {
        activated = സത്യം; // force trace pipeline activation (without OpenCV storage)
        __itt_region_begin(domain, __itt_null, __itt_null, __itt_string_handle_create("OpenCVTrace"));
    }
#endif
}
TraceManager::~TraceManager()
{
    CV_LOG("TraceManager dtor: " << (ശൂന്യം*)ഇത്);

#ifdef OPENCV_WITH_ITT
    എങ്കിൽ (isITTEnabled())
    {
        __itt_region_end(domain, __itt_null);
    }
#endif

    std::vector<TraceManagerThreadLocal*> threads_ctx;
    tls.gather(threads_ctx);
    size_t totalEvents = 0, totalSkippedEvents = 0;
    വേണ്ടി (size_t i = 0; i < threads_ctx.size(); i++)
    {
        TraceManagerThreadLocal* ctx = threads_ctx[i];
        എങ്കിൽ (ctx)
        {
            totalEvents += ctx->region_counter;
            totalSkippedEvents += ctx->totalSkippedEvents;
        }
    }
    എങ്കിൽ (totalEvents || activated)
    {
        CV_LOG_INFO(ശൂന്യ, "Trace: Total events: " << totalEvents);
    }
    എങ്കിൽ (totalSkippedEvents)
    {
        CV_LOG_WARNING(ശൂന്യ, "Trace: Total skipped events: " << totalSkippedEvents);
    }

    // This is a global static object, so process starts shutdown here
    // Turn off trace
    cv::__termination = സത്യം; // also set in DllMain() notifications handler for DLL_PROCESS_DETACH
    activated = അസത്യം;
}

തർക്കം TraceManager::isActivated()
{
    // Check if process starts shutdown, and set earlyExit to true
    // to prevent further instrumentation processing earlier.
    എങ്കിൽ (cv::__termination)
    {
        activated = അസത്യം;
        തിരികെ അസത്യം;
    }

    എങ്കിൽ (!isInitialized)
    {
        TraceManager& m = getTraceManager();
        CV_UNUSED(m); // TODO
    }

    തിരികെ activated;
}


സ്ഥിതം TraceManager* getTraceManagerCallOnce()
{
    സ്ഥിതം TraceManager globalInstance;
    തിരികെ &globalInstance;
}
TraceManager& getTraceManager()
{
    CV_SINGLETON_LAZY_INIT_REF(TraceManager, getTraceManagerCallOnce())
}

ശൂന്യം parallel_For_Set_വേര്_Region(സ്ഥിരം Region& വേര്_Region, സ്ഥിരം TraceManagerThreadLocal& വേര്_ctx)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();

    എങ്കിൽ (ctx.dummy_അട്ടി_മുകൾ.region == &വേര്_Region) // already attached
        തിരികെ;

    CV_Assert(ctx.dummy_അട്ടി_മുകൾ.region == ശൂന്യ);
    ctx.dummy_അട്ടി_മുകൾ = TraceManagerThreadLocal::അട്ടി_Entry(const_cast<Region*>(&വേര്_Region), ശൂന്യ, -1);

    എങ്കിൽ (&ctx == &വേര്_ctx)
    {
        ctx.stat.grab(ctx.parallel_for_stat);
        ctx.parallel_for_stat_status = ctx.stat_status;
        ctx.parallel_for_അട്ടി_size = ctx.അട്ടി.size();
        തിരികെ;
    }

    CV_Assert(ctx.അട്ടി.empty());

    ctx.currentActiveRegion = const_cast<Region*>(&വേര്_Region);

    ctx.regionDepth = വേര്_ctx.regionDepth;
    ctx.regionDepthOpenCV = വേര്_ctx.regionDepthOpenCV;

    ctx.parallel_for_അട്ടി_size = 0;

    ctx.stat_status.propagateFrom(വേര്_ctx.stat_status);
}

ശൂന്യം parallelForAttachNestedRegion(സ്ഥിരം Region& വേര്_Region)
{
    CV_UNUSED(വേര്_Region);
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();

    CV_DbgAssert(ctx.dummy_അട്ടി_മുകൾ.region == &വേര്_Region);

    Region* region = ctx.getCurrentActiveRegion();
    CV_LOG_PARALLEL(ശൂന്യ, " PARALLEL_FOR: " << (ശൂന്യം*)region << " ==> " << &വേര്_Region);
    എങ്കിൽ (!region)
        തിരികെ;

#ifdef OPENCV_WITH_ITT
    എങ്കിൽ (!വേര്_Region.pImpl || !വേര്_Region.pImpl->itt_id_registered)
        തിരികെ;

    എങ്കിൽ (!region->pImpl)
        തിരികെ;

    CV_LOG_PARALLEL(ശൂന്യ, " PARALLEL_FOR ITT: " << (ശൂന്യം*)വേര്_Region.pImpl->itt_id.d1 << ":" << വേര്_Region.pImpl->itt_id.d2 << ":" << (ശൂന്യം*)വേര്_Region.pImpl->itt_id.d3 << " => "
                                 << (ശൂന്യം*)region->pImpl->itt_id.d1 << ":" << region->pImpl->itt_id.d2 << ":" << (ശൂന്യം*)region->pImpl->itt_id.d3);
    __itt_relation_add(domain, region->pImpl->itt_id, __itt_relation_is_child_of, വേര്_Region.pImpl->itt_id);
#endif
}

ശൂന്യം parallelForFinalize(സ്ഥിരം Region& വേര്_Region)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();

    int64 endTimestamp = getTimestampNS();
    int64 duration = endTimestamp - ctx.അട്ടി_മുകൾ_Begin_Timestamp();
    CV_LOG_PARALLEL(ശൂന്യ, "parallel_for duration: " << duration << " " << &വേര്_Region);

    std::vector<TraceManagerThreadLocal*> threads_ctx;
    getTraceManager().tls.gather(threads_ctx);
    RegionStatistics parallel_for_stat;
    വേണ്ടി (size_t i = 0; i < threads_ctx.size(); i++)
    {
        TraceManagerThreadLocal* child_ctx = threads_ctx[i];

        എങ്കിൽ (child_ctx && child_ctx->അട്ടി_മുകൾ_Region() == &വേര്_Region)
        {
            CV_LOG_PARALLEL(ശൂന്യ, "Thread=" << child_ctx->threadID << " " << child_ctx->stat);
            RegionStatistics child_stat;
            child_ctx->stat.grab(child_stat);
            parallel_for_stat.append(child_stat);
            എങ്കിൽ (child_ctx != &ctx)
            {
                child_ctx->dummy_അട്ടി_മുകൾ = TraceManagerThreadLocal::അട്ടി_Entry();
            }
            അല്ലെങ്കിൽ
            {
                ctx.parallel_for_stat.grab(ctx.stat);
                ctx.stat_status = ctx.parallel_for_stat_status;
                child_ctx->dummy_അട്ടി_മുകൾ = TraceManagerThreadLocal::അട്ടി_Entry();
            }
        }
    }

    ദശാംശം parallel_coeff = std::min(1.0f, duration / (ദശാംശം)(parallel_for_stat.duration));
    CV_LOG_PARALLEL(ശൂന്യ, "parallel_coeff=" << 1.0f / parallel_coeff);
    CV_LOG_PARALLEL(ശൂന്യ, parallel_for_stat);
    എങ്കിൽ (parallel_coeff != 1.0f)
    {
        parallel_for_stat.multiply(parallel_coeff);
        CV_LOG_PARALLEL(ശൂന്യ, parallel_for_stat);
    }
    parallel_for_stat.duration = 0;
    ctx.stat.append(parallel_for_stat);
    CV_LOG_PARALLEL(ശൂന്യ, ctx.stat);
}

ഘടന TraceArg::ExtraData
{
#ifdef OPENCV_WITH_ITT
    // Special fields for ITT
    __itt_string_handle* അസ്ഥിര ittHandle_name;
#endif
    ExtraData(TraceManagerThreadLocal& ctx, സ്ഥിരം TraceArg& arg)
    {
        CV_UNUSED(ctx); CV_UNUSED(arg);
#ifdef OPENCV_WITH_ITT
        എങ്കിൽ (isITTEnabled())
        {
            // Caching is not required here, because there is builtin cache.
            // https://software.intel.com/en-us/node/544203:
            //     Consecutive calls to __itt_string_handle_create with the same name return the same value.
            ittHandle_name = __itt_string_handle_create(arg.name);
        }
        അല്ലെങ്കിൽ
        {
            ittHandle_name = 0;
        }
#endif
    }
};

സ്ഥിതം ശൂന്യം initTraceArg(TraceManagerThreadLocal& ctx, സ്ഥിരം TraceArg& arg)
{
    TraceArg::ExtraData** pExtra = arg.ppExtra;
    എങ്കിൽ (*pExtra == ശൂന്യ)
    {
        cv::AutoLock lock(cv::getInitializationMutex());
        എങ്കിൽ (*pExtra == ശൂന്യ)
        {
            *pExtra = പുതിയ TraceArg::ExtraData(ctx, arg);
        }
    }
}
ശൂന്യം traceArg(സ്ഥിരം TraceArg& arg, സ്ഥിരം അക്ഷരം* value)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    Region* region = ctx.getCurrentActiveRegion();
    എങ്കിൽ (!region)
        തിരികെ;
    CV_Assert(region->pImpl);
    initTraceArg(ctx, arg);
    എങ്കിൽ (!value)
        value = "<null>";
#ifdef OPENCV_WITH_ITT
    എങ്കിൽ (isITTEnabled())
    {
        __itt_metadata_str_add(domain, region->pImpl->itt_id, (*arg.ppExtra)->ittHandle_name, value, strlen(value));
    }
#endif
}
ശൂന്യം traceArg(സ്ഥിരം TraceArg& arg, പൂർണ്ണസംഖ്യ value)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    Region* region = ctx.getCurrentActiveRegion();
    എങ്കിൽ (!region)
        തിരികെ;
    CV_Assert(region->pImpl);
    initTraceArg(ctx, arg);
#ifdef OPENCV_WITH_ITT
    എങ്കിൽ (isITTEnabled())
    {
        __itt_metadata_add(domain, region->pImpl->itt_id, (*arg.ppExtra)->ittHandle_name, വലുപ്പം(പൂർണ്ണസംഖ്യ) == 4 ? __itt_metadata_s32 : __itt_metadata_s64, 1, &value);
    }
#അല്ലെങ്കിൽ
    CV_UNUSED(value);
#endif
}
ശൂന്യം traceArg(സ്ഥിരം TraceArg& arg, int64 value)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    Region* region = ctx.getCurrentActiveRegion();
    എങ്കിൽ (!region)
        തിരികെ;
    CV_Assert(region->pImpl);
    initTraceArg(ctx, arg);
#ifdef OPENCV_WITH_ITT
    എങ്കിൽ (isITTEnabled())
    {
        __itt_metadata_add(domain, region->pImpl->itt_id, (*arg.ppExtra)->ittHandle_name, __itt_metadata_s64, 1, &value);
    }
#അല്ലെങ്കിൽ
    CV_UNUSED(value);
#endif
}
ശൂന്യം traceArg(സ്ഥിരം TraceArg& arg, ഇരട്ട value)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    Region* region = ctx.getCurrentActiveRegion();
    എങ്കിൽ (!region)
        തിരികെ;
    CV_Assert(region->pImpl);
    initTraceArg(ctx, arg);
#ifdef OPENCV_WITH_ITT
    എങ്കിൽ (isITTEnabled())
    {
        __itt_metadata_add(domain, region->pImpl->itt_id, (*arg.ppExtra)->ittHandle_name, __itt_metadata_double, 1, &value);
    }
#അല്ലെങ്കിൽ
    CV_UNUSED(value);
#endif
}

#അല്ലെങ്കിൽ

Region::Region(സ്ഥിരം LocationStaticStorage&) : pImpl(ശൂന്യ), implFlags(0) {}
ശൂന്യം Region::destroy() {}

ശൂന്യം traceArg(സ്ഥിരം TraceArg&, സ്ഥിരം അക്ഷരം*) {}
ശൂന്യം traceArg(സ്ഥിരം TraceArg&, പൂർണ്ണസംഖ്യ) {};
ശൂന്യം traceArg(സ്ഥിരം TraceArg&, int64) {};
ശൂന്യം traceArg(സ്ഥിരം TraceArg&, ഇരട്ട) {};

#endif

}}}} // namespace
