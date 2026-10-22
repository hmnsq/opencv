// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#شامل "precomp.hpp"

#شامل <opencv2/core/utils/trace.hpp>
#شامل <opencv2/core/utils/trace.private.hpp>
#شامل <opencv2/core/utils/configuration.private.hpp>

#شامل <opencv2/core/opencl/ocl_defs.hpp>

#شامل <cstdarg> // va_start

#شامل <sstream>
#شامل <ostream>
#شامل <fstream>

#اگر 0
#define CV_LOG(...) CV_LOG_INFO(پوچ, __VA_ARGS__)
#وگرنه
#define CV_LOG(...) {}
#endif

#اگر 0
#define CV_LOG_ITT(...) CV_LOG_INFO(پوچ, __VA_ARGS__)
#وگرنه
#define CV_LOG_ITT(...) {}
#endif

#اگر 1
#define CV_LOG_TRACE_BAILOUT(tag, ...) CV_LOG_INFO(tag, __VA_ARGS__)
#وگرنه
#define CV_LOG_TRACE_BAILOUT(...) {}
#endif

#اگر 0
#define CV_LOG_PARALLEL(tag, ...) CV_LOG_INFO(tag, __VA_ARGS__)
#وگرنه
#define CV_LOG_PARALLEL(...) {}
#endif

#اگر 0
#define CV_LOG_CTX_STAT(tag, ...) CV_LOG_INFO(tag, __VA_ARGS__)
#وگرنه
#define CV_LOG_CTX_STAT(...) {}
#endif

#اگر 0
#define CV_LOG_SKIP(tag, ...) CV_LOG_INFO(tag, __VA_ARGS__)
#وگرنه
#define CV_LOG_SKIP(...) {}
#endif

فضاینام cv {
فضاینام utils {
فضاینام trace {
فضاینام details {

#ifdef OPENCV_TRACE

#ifdef _MSC_VER
#pragma warning(disable:4065) // switch statement contains 'default' but no 'case' labels
#endif

ایستا منطقی getParameterTraceEnable()
{
    ایستا منطقی param_traceEnable = utils::getConfigurationParameterBool("OPENCV_TRACE", نادرست);
    بازگشت param_traceEnable;
}

// TODO lazy configuration flags
ایستا صحیح param_maxRegionDepthOpenCV = (صحیح)utils::getConfigurationParameterSizeT("OPENCV_TRACE_DEPTH_OPENCV", 1);
ایستا صحیح param_maxRegionChildrenOpenCV = (صحیح)utils::getConfigurationParameterSizeT("OPENCV_TRACE_MAX_CHILDREN_OPENCV", 1000);
ایستا صحیح param_maxRegionChildren = (صحیح)utils::getConfigurationParameterSizeT("OPENCV_TRACE_MAX_CHILDREN", 10000);

ایستا ثابت cv::String& getParameterTraceLocation()
{
    ایستا cv::String param_traceLocation = utils::getConfigurationParameterString("OPENCV_TRACE_LOCATION", "OpenCVTrace");
    بازگشت param_traceLocation;
}

#ifdef HAVE_OPENCL
ایستا منطقی param_synchronizeOpenCL = utils::getConfigurationParameterBool("OPENCV_TRACE_SYNC_OPENCL", نادرست);
#endif

#ifdef OPENCV_WITH_ITT
ایستا منطقی param_ITT_registerParentScope = utils::getConfigurationParameterBool("OPENCV_TRACE_ITT_PARENT", نادرست);
#endif

ایستا ثابت نویسه* _spaces(صحیح شمار_2)
{
    ایستا ثابت نویسه buf[64] =
"                                                               ";
    بازگشت &buf[63 - (شمار_2 & 63)];
}

/**
 * Text-based trace messages
 */
رده TraceMessage
{
عمومی:
    نویسه buffer[1024];
    size_t len;
    منطقی hasError;

    TraceMessage() :
        len(0),
        hasError(نادرست)
    {}

    منطقی printf(ثابت نویسه* format, ...)
    {
        نویسه* buf = &buffer[len];
        size_t sz = اندازه(buffer) - len;
        va_list ap;
        va_start(ap, format);
        صحیح n = cv_vsnprintf(buf, (صحیح)sz, format, ap);
        va_end(ap);
        اگر (n < 0 || (size_t)n > sz)
        {
            hasError = درست;
            بازگشت نادرست;
        }
        len += n;
        بازگشت درست;
    }

    منطقی formatlocation(ثابت Region::LocationStaticStorage& location)
    {
        بازگشت این->printf("l,%lld,\"%s\",%d,\"%s\",0x%llX\n",
                (long long صحیح)(*location.ppExtra)->global_location_id,
                location.filename,
                location.سطر,
                location.name,
                (long long صحیح)(location.flags & ~0xF0000000));
    }
    منطقی formatRegionEnter(ثابت Region& region)
    {
        منطقی ok = این->printf("b,%d,%lld,%lld,%lld",
                (صحیح)region.pImpl->threadID,
                (long long صحیح)region.pImpl->beginTimestamp,
                (long long صحیح)((*region.pImpl->location.ppExtra)->global_location_id),
                (long long صحیح)region.pImpl->global_region_id);
        اگر (region.pImpl->parentRegion && region.pImpl->parentRegion->pImpl)
        {
            اگر (region.pImpl->parentRegion->pImpl->threadID != region.pImpl->threadID)
                ok &= این->printf(",parentThread=%d,parent=%lld",
                        (صحیح)region.pImpl->parentRegion->pImpl->threadID,
                        (long long صحیح)region.pImpl->parentRegion->pImpl->global_region_id);
        }
        ok &= این->printf("\n");
        بازگشت ok;
    }
    منطقی formatRegionLeave(ثابت Region& region, ثابت RegionStatistics& result)
    {
        CV_DbgAssert(region.pImpl->endTimestamp - region.pImpl->beginTimestamp == result.duration);
        منطقی ok = این->printf("e,%d,%lld,%lld,%lld,%lld",
                (صحیح)region.pImpl->threadID,
                (long long صحیح)region.pImpl->endTimestamp,
                (long long صحیح)(*region.pImpl->location.ppExtra)->global_location_id,
                (long long صحیح)region.pImpl->global_region_id,
                (long long صحیح)result.duration);
        اگر (result.currentSkippedRegions)
            ok &= این->printf(",skip=%d", (صحیح)result.currentSkippedRegions);
#ifdef HAVE_IPP
        اگر (result.durationImplIPP)
            ok &= این->printf(",tIPP=%lld", (long long صحیح)result.durationImplIPP);
#endif
#ifdef HAVE_OPENCL
        اگر (result.durationImplOpenCL)
            ok &= این->printf(",tOCL=%lld", (long long صحیح)result.durationImplOpenCL);
#endif
        ok &= این->printf("\n");
        بازگشت ok;
    }
    منطقی recordRegionArg(ثابت Region& region, ثابت TraceArg& arg, ثابت نویسه* value)
    {
        بازگشت این->printf("a,%d,%lld,%lld,\"%s\",\"%s\"\n",
                region.pImpl->threadID,
                (long long صحیح)region.pImpl->beginTimestamp,
                (long long صحیح)region.pImpl->global_region_id,
                arg.name,
                value);
    }
};


#ifdef OPENCV_WITH_ITT
ایستا __itt_domain* domain = پوچ;

ایستا منطقی isITTEnabled()
{
    ایستا فرار منطقی isInitialized = نادرست;
    ایستا منطقی isEnabled = نادرست;
    اگر (!isInitialized)
    {
        cv::AutoLock lock(cv::getInitializationMutex());
        اگر (!isInitialized)
        {
            منطقی param_traceITTEnable = utils::getConfigurationParameterBool("OPENCV_TRACE_ITT_ENABLE", درست);
            اگر (param_traceITTEnable)
            {
                isEnabled = !!(__itt_api_version());
                CV_LOG_ITT("ITT is " << (isEnabled ? "enabled" : "disabled"));
                domain = __itt_domain_create("OpenCVTrace");
            }
            وگرنه
            {
                CV_LOG_ITT("ITT is disabled through OpenCV parameter");
                isEnabled = نادرست;
            }
            isInitialized = درست;
        }
    }
    بازگشت isEnabled;
}
#endif


Region::LocationExtraData::LocationExtraData(ثابت LocationStaticStorage& location)
{
    CV_UNUSED(location);
    ایستا صحیح g_location_id_counter = 0;
    global_location_id = CV_XADD(&g_location_id_counter, 1) + 1;
    CV_LOG("Register location: " << global_location_id << " (" << (تهی*)&location << ")"
            << std::endl << "    file: " << location.filename
            << std::endl << "    line: " << location.سطر
            << std::endl << "    name: " << location.name);
#ifdef OPENCV_WITH_ITT
    اگر (isITTEnabled())
    {
        // Caching is not required here, because there is builtin cache.
        // https://software.intel.com/en-us/node/544203:
        //     Consecutive calls to __itt_string_handle_create with the same name return the same value.
        ittHandle_name = __itt_string_handle_create(location.name);
        ittHandle_filename = __itt_string_handle_create(location.filename);
    }
    وگرنه
    {
        ittHandle_name = 0;
        ittHandle_filename = 0;
    }
#endif
}

/*static*/ Region::LocationExtraData* Region::LocationExtraData::init(ثابت Region::LocationStaticStorage& location)
{
    LocationExtraData** pLocationExtra = location.ppExtra;
    CV_DbgAssert(pLocationExtra);
    اگر (*pLocationExtra == پوچ)
    {
        cv::AutoLock lock(cv::getInitializationMutex());
        اگر (*pLocationExtra == پوچ)
        {
            *pLocationExtra = جدید Region::LocationExtraData(location);
            TraceStorage* s = getTraceManager().trace_storage.get();
            اگر (s)
            {
                TraceMessage پیام;
                پیام.formatlocation(location);
                s->put(پیام);
            }
        }
    }
    بازگشت *pLocationExtra;
}


Region::Impl::Impl(TraceManagerThreadLocal& ctx, Region* parentRegion_, Region& region_, ثابت LocationStaticStorage& location_, int64 beginTimestamp_) :
    location(location_),
    region(region_),
    parentRegion(parentRegion_),
    threadID(ctx.threadID),
    global_region_id(++ctx.region_counter),
    beginTimestamp(beginTimestamp_),
    endTimestamp(0),
    direct_Children_شمار(0)
#ifdef OPENCV_WITH_ITT
    ,itt_id_registered(نادرست)
    ,itt_id(__itt_null)
#endif
{
    CV_DbgAssert(ctx.currentActiveRegion == parentRegion);
    region.pImpl = این;

    registerRegion(ctx);

    enterRegion(ctx);
}

Region::Impl::~Impl()
{
#ifdef OPENCV_WITH_ITT
    اگر (itt_id_registered)
    {
        CV_LOG_ITT(" Destroy ITT region: I=" << (تهی*)این);
        __itt_id_destroy(domain, itt_id);
        itt_id_registered = نادرست;
    }
#endif
    region.pImpl = پوچ;
}

تهی Region::Impl::enterRegion(TraceManagerThreadLocal& ctx)
{
    ctx.currentActiveRegion = &region;

    اگر (location.flags & REGION_FLAG_FUNCTION)
    {
        اگر ((location.flags & REGION_FLAG_APP_CODE) == 0)
        {
            ctx.regionDepthOpenCV++;
        }
        ctx.regionDepth++;
    }

    TraceStorage* s = ctx.getStorage();
    اگر (s)
    {
        TraceMessage پیام;
        پیام.formatRegionEnter(region);
        s->put(پیام);
    }
#ifdef OPENCV_WITH_ITT
    اگر (isITTEnabled())
    {
        __itt_id parentID = __itt_null;
        اگر (param_ITT_registerParentScope && parentRegion && parentRegion->pImpl && parentRegion->pImpl->itt_id_registered && (location.flags & REGION_FLAG_REGION_FORCE) == 0)
            parentID = parentRegion->pImpl->itt_id;
        __itt_task_begin(domain, itt_id, parentID, (*location.ppExtra)->ittHandle_name);
    }
#endif
}

تهی Region::Impl::leaveRegion(TraceManagerThreadLocal& ctx)
{
    int64 duration = endTimestamp - beginTimestamp; CV_UNUSED(duration);
    RegionStatistics result;
    ctx.stat.grab(result);
    ctx.totalSkippedEvents += result.currentSkippedRegions;
    CV_LOG(_spaces(ctx.getCurrentDepth()*4) << "leaveRegion(): " << (تهی*)این << " " << result);
#ifdef OPENCV_WITH_ITT
    اگر (isITTEnabled())
    {
        اگر (result.currentSkippedRegions)
        {
            __itt_metadata_add(domain, itt_id, __itt_string_handle_create("skipped trace entries"), __itt_metadata_u32, 1, &result.currentSkippedRegions);
        }
#ifdef HAVE_IPP
        اگر (result.durationImplIPP)
            __itt_metadata_add(domain, itt_id, __itt_string_handle_create("tIPP"), __itt_metadata_u64, 1, &result.durationImplIPP);
#endif
#ifdef HAVE_OPENCL
        اگر (result.durationImplOpenCL)
            __itt_metadata_add(domain, itt_id, __itt_string_handle_create("tOpenCL"), __itt_metadata_u64, 1, &result.durationImplOpenCL);
#endif
        __itt_task_end(domain);
    }
#endif
    TraceStorage* s = ctx.getStorage();
    اگر (s)
    {
        TraceMessage پیام;
        پیام.formatRegionLeave(region, result);
        s->put(پیام);
    }

    اگر (location.flags & REGION_FLAG_FUNCTION)
    {
        اگر ((location.flags & REGION_FLAG_APP_CODE) == 0)
        {
            ctx.regionDepthOpenCV--;
        }
        ctx.regionDepth--;
    }

    ctx.currentActiveRegion = parentRegion;
}

تهی Region::Impl::release()
{
    delete این;
}

تهی Region::Impl::registerRegion(TraceManagerThreadLocal& ctx)
{
#ifdef OPENCV_WITH_ITT
    اگر (isITTEnabled())
    {
        اگر (!itt_id_registered)
        {
            CV_LOG_ITT(" Register ITT region: I=" << (تهی*)این << " " << ctx.threadID << "-" << global_region_id);
#اگر 1 // workaround for some ITT backends
            itt_id = __itt_id_make((تهی*)(intptr_t)(((int64)(ctx.threadID + 1) << 32) | global_region_id), global_region_id);
#وگرنه
            itt_id = __itt_id_make((تهی*)(intptr_t)(ctx.threadID + 1), global_region_id);
#endif
            __itt_id_create(domain, itt_id);
            itt_id_registered = درست;
        }
    }
#وگرنه
    CV_UNUSED(ctx);
#endif
}

تهی RegionStatisticsStatus::enableSkipMode(صحیح depth)
{
    CV_DbgAssert(_skipDepth < 0);
    CV_LOG_SKIP(پوچ, "SKIP-ENABLE: depth=" << depth);
    _skipDepth = depth;
}
تهی RegionStatisticsStatus::checkResetSkipMode(صحیح leaveDepth)
{
    اگر (leaveDepth <= _skipDepth)
    {
        CV_LOG_SKIP(پوچ, "SKIP-RESET: leaveDepth=" << leaveDepth << " skipDepth=" << _skipDepth);
        _skipDepth = -1;
    }
}

Region::Region(ثابت LocationStaticStorage& location) :
    pImpl(پوچ),
    implFlags(0)
{
    // Checks:
    // - global enable flag
    // - parent region is disabled
    // - children count threshold
    // - region location
    // - depth (opencv nested calls)
    اگر (!TraceManager::isActivated())
    {
        CV_LOG("Trace is disabled. Bailout");
        بازگشت;
    }

    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    CV_LOG(_spaces(ctx.getCurrentDepth()*4) << "Region(): " << (تهی*)این << ": " << location.name);

    Region* parentRegion = ctx.پشته_بالاترین_Region();
    ثابت Region::LocationStaticStorage* parentLocation = ctx.پشته_بالاترین_Location();

    اگر (location.flags & REGION_FLAG_REGION_NEXT)
    {
        اگر (parentRegion && parentRegion->pImpl)
        {
            CV_DbgAssert((parentRegion->pImpl->location.flags & REGION_FLAG_FUNCTION) == 0);
            parentRegion->destroy(); parentRegion->implFlags = 0;
            parentRegion = ctx.پشته_بالاترین_Region();
            parentLocation = ctx.پشته_بالاترین_Location();
        }
    }

    صحیح parentChildren = 0;
    اگر (parentRegion && parentRegion->pImpl)
    {
        اگر (parentLocation == پوچ)
        {
            // parallel_for_body code path
            parentChildren = CV_XADD(&parentRegion->pImpl->direct_Children_شمار, 1) + 1;
        }
        وگرنه
        {
            parentChildren = ++parentRegion->pImpl->direct_Children_شمار;
        }
    }

    int64 beginTimestamp = getTimestampNS();

    صحیح currentDepth = ctx.getCurrentDepth() + 1;
    انتخاب (location.flags & REGION_FLAG_IMPL_MASK)
    {
#ifdef HAVE_IPP
    حالت REGION_FLAG_IMPL_IPP:
        اگر (!ctx.stat_status.ignoreDepthImplIPP)
            ctx.stat_status.ignoreDepthImplIPP = currentDepth;
        بشکن;
#endif
#ifdef HAVE_OPENCL
    حالت REGION_FLAG_IMPL_OPENCL:
        اگر (!ctx.stat_status.ignoreDepthImplOpenCL)
            ctx.stat_status.ignoreDepthImplOpenCL = currentDepth;
        بشکن;
#endif
    پیشفرض:
        بشکن;
    }

    ctx.پشته_افزودن(این, &location, beginTimestamp);
    implFlags |= REGION_FLAG_NEED_پشته_برداشت;

    اگر ((location.flags & REGION_FLAG_REGION_FORCE) == 0)
    {
        اگر (ctx.stat_status._skipDepth >= 0 && currentDepth > ctx.stat_status._skipDepth)
        {
            CV_LOG(_spaces(ctx.getCurrentDepth()*4) << "Parent region is disabled. Bailout");
            ctx.stat.currentSkippedRegions++;
            بازگشت;
        }

        اگر (param_maxRegionChildrenOpenCV > 0 && (location.flags & REGION_FLAG_APP_CODE) == 0 && parentLocation && (parentLocation->flags & REGION_FLAG_APP_CODE) == 0)
        {
            اگر (parentChildren >= param_maxRegionChildrenOpenCV)
            {
                CV_LOG_TRACE_BAILOUT(پوچ, _spaces(ctx.getCurrentDepth()*4) << "OpenCV parent region exceeds children count. Bailout");
                ctx.stat_status.enableSkipMode(currentDepth - 1);
                ctx.stat.currentSkippedRegions++;
                DEBUG_ONLY(ctx.dump_پشته(std::cout, نادرست));
                بازگشت;
            }
        }
        اگر (param_maxRegionChildren > 0 && parentChildren >= param_maxRegionChildren)
        {
            CV_LOG_TRACE_BAILOUT(پوچ, _spaces(ctx.getCurrentDepth()*4) << "Parent region exceeds children count. Bailout");
            ctx.stat_status.enableSkipMode(currentDepth - 1);
            ctx.stat.currentSkippedRegions++;
            DEBUG_ONLY(ctx.dump_پشته(std::cout, نادرست));
            بازگشت;
        }
    }

    LocationExtraData::init(location);

    اگر ((*location.ppExtra)->global_location_id == 0)
    {
        CV_LOG_TRACE_BAILOUT(پوچ, _spaces(ctx.getCurrentDepth()*4) << "Region location is disabled. Bailout");
        ctx.stat_status.enableSkipMode(currentDepth);
        ctx.stat.currentSkippedRegions++;
        بازگشت;
    }

    اگر (parentLocation && (parentLocation->flags & REGION_FLAG_SKIP_NESTED))
    {
        CV_LOG(_spaces(ctx.getCurrentDepth()*4) << "Parent region disables inner regions. Bailout");
        ctx.stat_status.enableSkipMode(currentDepth);
        ctx.stat.currentSkippedRegions++;
        بازگشت;
    }

    اگر (param_maxRegionDepthOpenCV)
    {
        اگر ((location.flags & REGION_FLAG_APP_CODE) == 0)
        {
            اگر (ctx.regionDepthOpenCV >= param_maxRegionDepthOpenCV)
            {
                CV_LOG(_spaces(ctx.getCurrentDepth()*4) << "OpenCV region depth is exceed = " << ctx.regionDepthOpenCV << ". Bailout");
                اگر (ctx.stat.currentSkippedRegions == 0)
                {
                    DEBUG_ONLY(ctx.dump_پشته(std::cout, نادرست));
                }
                ctx.stat_status.enableSkipMode(currentDepth);
                ctx.stat.currentSkippedRegions++;
                بازگشت;
            }
        }
    }

    جدید Impl(ctx, parentRegion, *این, location, beginTimestamp);
    CV_DbgAssert(pImpl != پوچ);
    implFlags |= REGION_FLAG__ACTIVE;

    // parallel_for path
    اگر (parentRegion && parentRegion->pImpl)
    {
        اگر (parentLocation == پوچ)
        {
            pImpl->direct_Children_شمار = parentChildren;
        }
    }
}

تهی Region::destroy()
{
    CV_DbgAssert(implFlags != 0);

    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    CV_LOG(_spaces(ctx.getCurrentDepth()*4) << "Region::destruct(): " << (تهی*)این << " pImpl=" << pImpl << " implFlags=" << implFlags << ' ' << (ctx.پشته_بالاترین_Location() ? ctx.پشته_بالاترین_Location()->name : "<unknown>"));

    CV_DbgAssert(implFlags & REGION_FLAG_NEED_پشته_برداشت);
    ثابت صحیح currentDepth = ctx.getCurrentDepth(); CV_UNUSED(currentDepth);

    CV_LOG_CTX_STAT(پوچ, _spaces(currentDepth*4) << ctx.stat << ' ' << ctx.stat_status);

    ثابت Region::LocationStaticStorage* location = ctx.پشته_بالاترین_Location();
    Impl::OptimizationPath myCodePath = Impl::CODE_PATH_PLAIN;
    اگر (location)
    {
        انتخاب (location->flags & REGION_FLAG_IMPL_MASK)
        {
#ifdef HAVE_IPP
        حالت REGION_FLAG_IMPL_IPP:
            myCodePath = Impl::CODE_PATH_IPP;
            بشکن;
#endif
#ifdef HAVE_OPENCL
        حالت REGION_FLAG_IMPL_OPENCL:
            اگر (param_synchronizeOpenCL && cv::ocl::isOpenCLActivated())
                cv::ocl::finish();
            myCodePath = Impl::CODE_PATH_OPENCL;
            بشکن;
#endif
        پیشفرض:
            بشکن;
        }
    }

    int64 endTimestamp = getTimestampNS();
    int64 duration = endTimestamp - ctx.پشته_بالاترین_Begin_Timestamp();

    منطقی active = isActive();

    اگر (active)
        ctx.stat.duration = duration;
    وگرنه اگر (ctx.پشته.size() == ctx.parallel_for_پشته_size + 1)
        ctx.stat.duration += duration;

    انتخاب (myCodePath) {
        حالت Impl::CODE_PATH_PLAIN:
            // nothing
            بشکن;
#ifdef HAVE_IPP
        حالت Impl::CODE_PATH_IPP:
            اگر (ctx.stat_status.ignoreDepthImplIPP == currentDepth)
            {
                ctx.stat.durationImplIPP += duration;
                ctx.stat_status.ignoreDepthImplIPP = 0;
            }
            وگرنه اگر (active)
            {
                ctx.stat.durationImplIPP = duration;
            }
            بشکن;
#endif
#ifdef HAVE_OPENCL
        حالت Impl::CODE_PATH_OPENCL:
            اگر (ctx.stat_status.ignoreDepthImplOpenCL == currentDepth)
            {
                ctx.stat.durationImplOpenCL += duration;
                ctx.stat_status.ignoreDepthImplOpenCL = 0;
            }
            وگرنه اگر (active)
            {
                ctx.stat.durationImplOpenCL = duration;
            }
            بشکن;
#endif
        پیشفرض:
            بشکن;
    }

    اگر (pImpl)
    {
        CV_DbgAssert((implFlags & (REGION_FLAG__ACTIVE | REGION_FLAG_NEED_پشته_برداشت)) == (REGION_FLAG__ACTIVE | REGION_FLAG_NEED_پشته_برداشت));
        CV_DbgAssert(ctx.پشته_بالاترین_Region() == این);
        pImpl->endTimestamp = endTimestamp;
        pImpl->leaveRegion(ctx);
        pImpl->release();
        pImpl = پوچ;
        DEBUG_ONLY(implFlags &= ~REGION_FLAG__ACTIVE);
    }
    وگرنه
    {
        CV_DbgAssert(ctx.stat_status._skipDepth <= currentDepth);
    }

    اگر (implFlags & REGION_FLAG_NEED_پشته_برداشت)
    {
        CV_DbgAssert(ctx.پشته_بالاترین_Region() == این);
        ctx.پشته_برداشت();
        ctx.stat_status.checkResetSkipMode(currentDepth);
        DEBUG_ONLY(implFlags &= ~REGION_FLAG_NEED_پشته_برداشت);
    }
    CV_LOG_CTX_STAT(پوچ, _spaces(currentDepth*4) << "===> " << ctx.stat << ' ' << ctx.stat_status);
}


TraceManagerThreadLocal::~TraceManagerThreadLocal()
{
}

تهی TraceManagerThreadLocal::dump_پشته(std::ostream& out, منطقی onlyFunctions) ثابت
{
    std::stringstream ss;
    std::deque<پشته_Entry>::const_iterator it = پشته.begin();
    std::deque<پشته_Entry>::const_iterator end = پشته.end();
    صحیح depth = 0;
    برای (; it != end; ++it)
    {
        ثابت Region::LocationStaticStorage* location = it->location;
        اگر (location)
        {
            اگر (!onlyFunctions || (location->flags & REGION_FLAG_FUNCTION))
            {
                ss << _spaces(4*depth) << location->name << std::endl;
                depth++;
            }
        }
        وگرنه
        {
            ss << _spaces(4*depth) << "<unknown>" << std::endl;
            depth++;
        }
    }
    out << ss.str();
}

رده AsyncTraceStorage CV_FINAL : عمومی TraceStorage
{
    mutable std::ofstream out;
عمومی:
    ثابت std::string name;

    AsyncTraceStorage(ثابت std::string& filename) :
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

    منطقی put(ثابت TraceMessage& پیام) ثابت CV_OVERRIDE
    {
        اگر (پیام.hasError)
            بازگشت نادرست;
        out << پیام.buffer;
        //DEBUG_ONLY(std::flush(out)); // TODO configure flag
        بازگشت درست;
    }
};

رده SyncTraceStorage CV_FINAL : عمومی TraceStorage
{
    mutable std::ofstream out;
    mutable cv::Mutex mutex;
عمومی:
    ثابت std::string name;

    SyncTraceStorage(ثابت std::string& filename) :
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

    منطقی put(ثابت TraceMessage& پیام) ثابت CV_OVERRIDE
    {
        اگر (پیام.hasError)
            بازگشت نادرست;
        {
            cv::AutoLock l(mutex);
            out << پیام.buffer;
            std::flush(out); // TODO configure flag
        }
        بازگشت درست;
    }
};


TraceStorage* TraceManagerThreadLocal::getStorage() ثابت
{
    // TODO configuration option for stdout/single trace file
    اگر (storage.empty())
    {
        TraceStorage* global = getTraceManager().trace_storage.get();
        اگر (global)
        {
            ثابت std::string filepath = cv::format("%s-%03d.txt", getParameterTraceLocation().c_str(), threadID).c_str();
            TraceMessage پیام;
            ثابت نویسه* pos = strrchr(filepath.c_str(), '/'); // extract filename
#ifdef _WIN32
            اگر (!pos)
                pos = strrchr(filepath.c_str(), '\\');
#endif
            اگر (!pos)
                pos = filepath.c_str();
            وگرنه
                pos += 1; // fix to skip extra slash in filename beginning
            پیام.printf("#thread file: %s\n", pos);
            global->put(پیام);
            storage.reset(جدید AsyncTraceStorage(filepath));
        }
    }
    بازگشت storage.get();
}



ایستا منطقی activated = نادرست;
ایستا منطقی isInitialized = نادرست;

TraceManager::TraceManager()
{
    (تهی)cv::getTimestampNS();

    isInitialized = درست;
    CV_LOG("TraceManager ctor: " << (تهی*)این);

    CV_LOG("TraceManager configure()");
    activated = getParameterTraceEnable();

    اگر (activated)
        trace_storage.reset(جدید SyncTraceStorage(std::string(getParameterTraceLocation()) + ".txt"));

#ifdef OPENCV_WITH_ITT
    اگر (isITTEnabled())
    {
        activated = درست; // force trace pipeline activation (without OpenCV storage)
        __itt_region_begin(domain, __itt_null, __itt_null, __itt_string_handle_create("OpenCVTrace"));
    }
#endif
}
TraceManager::~TraceManager()
{
    CV_LOG("TraceManager dtor: " << (تهی*)این);

#ifdef OPENCV_WITH_ITT
    اگر (isITTEnabled())
    {
        __itt_region_end(domain, __itt_null);
    }
#endif

    std::vector<TraceManagerThreadLocal*> threads_ctx;
    tls.gather(threads_ctx);
    size_t totalEvents = 0, totalSkippedEvents = 0;
    برای (size_t i = 0; i < threads_ctx.size(); i++)
    {
        TraceManagerThreadLocal* ctx = threads_ctx[i];
        اگر (ctx)
        {
            totalEvents += ctx->region_counter;
            totalSkippedEvents += ctx->totalSkippedEvents;
        }
    }
    اگر (totalEvents || activated)
    {
        CV_LOG_INFO(پوچ, "Trace: Total events: " << totalEvents);
    }
    اگر (totalSkippedEvents)
    {
        CV_LOG_WARNING(پوچ, "Trace: Total skipped events: " << totalSkippedEvents);
    }

    // This is a global static object, so process starts shutdown here
    // Turn off trace
    cv::__termination = درست; // also set in DllMain() notifications handler for DLL_PROCESS_DETACH
    activated = نادرست;
}

منطقی TraceManager::isActivated()
{
    // Check if process starts shutdown, and set earlyExit to true
    // to prevent further instrumentation processing earlier.
    اگر (cv::__termination)
    {
        activated = نادرست;
        بازگشت نادرست;
    }

    اگر (!isInitialized)
    {
        TraceManager& m = getTraceManager();
        CV_UNUSED(m); // TODO
    }

    بازگشت activated;
}


ایستا TraceManager* getTraceManagerCallOnce()
{
    ایستا TraceManager globalInstance;
    بازگشت &globalInstance;
}
TraceManager& getTraceManager()
{
    CV_SINGLETON_LAZY_INIT_REF(TraceManager, getTraceManagerCallOnce())
}

تهی parallel_For_Set_ریشه_Region(ثابت Region& ریشه_Region, ثابت TraceManagerThreadLocal& ریشه_ctx)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();

    اگر (ctx.dummy_پشته_بالاترین.region == &ریشه_Region) // already attached
        بازگشت;

    CV_Assert(ctx.dummy_پشته_بالاترین.region == پوچ);
    ctx.dummy_پشته_بالاترین = TraceManagerThreadLocal::پشته_Entry(const_cast<Region*>(&ریشه_Region), پوچ, -1);

    اگر (&ctx == &ریشه_ctx)
    {
        ctx.stat.grab(ctx.parallel_for_stat);
        ctx.parallel_for_stat_status = ctx.stat_status;
        ctx.parallel_for_پشته_size = ctx.پشته.size();
        بازگشت;
    }

    CV_Assert(ctx.پشته.empty());

    ctx.currentActiveRegion = const_cast<Region*>(&ریشه_Region);

    ctx.regionDepth = ریشه_ctx.regionDepth;
    ctx.regionDepthOpenCV = ریشه_ctx.regionDepthOpenCV;

    ctx.parallel_for_پشته_size = 0;

    ctx.stat_status.propagateFrom(ریشه_ctx.stat_status);
}

تهی parallelForAttachNestedRegion(ثابت Region& ریشه_Region)
{
    CV_UNUSED(ریشه_Region);
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();

    CV_DbgAssert(ctx.dummy_پشته_بالاترین.region == &ریشه_Region);

    Region* region = ctx.getCurrentActiveRegion();
    CV_LOG_PARALLEL(پوچ, " PARALLEL_FOR: " << (تهی*)region << " ==> " << &ریشه_Region);
    اگر (!region)
        بازگشت;

#ifdef OPENCV_WITH_ITT
    اگر (!ریشه_Region.pImpl || !ریشه_Region.pImpl->itt_id_registered)
        بازگشت;

    اگر (!region->pImpl)
        بازگشت;

    CV_LOG_PARALLEL(پوچ, " PARALLEL_FOR ITT: " << (تهی*)ریشه_Region.pImpl->itt_id.d1 << ":" << ریشه_Region.pImpl->itt_id.d2 << ":" << (تهی*)ریشه_Region.pImpl->itt_id.d3 << " => "
                                 << (تهی*)region->pImpl->itt_id.d1 << ":" << region->pImpl->itt_id.d2 << ":" << (تهی*)region->pImpl->itt_id.d3);
    __itt_relation_add(domain, region->pImpl->itt_id, __itt_relation_is_child_of, ریشه_Region.pImpl->itt_id);
#endif
}

تهی parallelForFinalize(ثابت Region& ریشه_Region)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();

    int64 endTimestamp = getTimestampNS();
    int64 duration = endTimestamp - ctx.پشته_بالاترین_Begin_Timestamp();
    CV_LOG_PARALLEL(پوچ, "parallel_for duration: " << duration << " " << &ریشه_Region);

    std::vector<TraceManagerThreadLocal*> threads_ctx;
    getTraceManager().tls.gather(threads_ctx);
    RegionStatistics parallel_for_stat;
    برای (size_t i = 0; i < threads_ctx.size(); i++)
    {
        TraceManagerThreadLocal* child_ctx = threads_ctx[i];

        اگر (child_ctx && child_ctx->پشته_بالاترین_Region() == &ریشه_Region)
        {
            CV_LOG_PARALLEL(پوچ, "Thread=" << child_ctx->threadID << " " << child_ctx->stat);
            RegionStatistics child_stat;
            child_ctx->stat.grab(child_stat);
            parallel_for_stat.append(child_stat);
            اگر (child_ctx != &ctx)
            {
                child_ctx->dummy_پشته_بالاترین = TraceManagerThreadLocal::پشته_Entry();
            }
            وگرنه
            {
                ctx.parallel_for_stat.grab(ctx.stat);
                ctx.stat_status = ctx.parallel_for_stat_status;
                child_ctx->dummy_پشته_بالاترین = TraceManagerThreadLocal::پشته_Entry();
            }
        }
    }

    اعشاری parallel_coeff = std::min(1.0f, duration / (اعشاری)(parallel_for_stat.duration));
    CV_LOG_PARALLEL(پوچ, "parallel_coeff=" << 1.0f / parallel_coeff);
    CV_LOG_PARALLEL(پوچ, parallel_for_stat);
    اگر (parallel_coeff != 1.0f)
    {
        parallel_for_stat.multiply(parallel_coeff);
        CV_LOG_PARALLEL(پوچ, parallel_for_stat);
    }
    parallel_for_stat.duration = 0;
    ctx.stat.append(parallel_for_stat);
    CV_LOG_PARALLEL(پوچ, ctx.stat);
}

ساختار TraceArg::ExtraData
{
#ifdef OPENCV_WITH_ITT
    // Special fields for ITT
    __itt_string_handle* فرار ittHandle_name;
#endif
    ExtraData(TraceManagerThreadLocal& ctx, ثابت TraceArg& arg)
    {
        CV_UNUSED(ctx); CV_UNUSED(arg);
#ifdef OPENCV_WITH_ITT
        اگر (isITTEnabled())
        {
            // Caching is not required here, because there is builtin cache.
            // https://software.intel.com/en-us/node/544203:
            //     Consecutive calls to __itt_string_handle_create with the same name return the same value.
            ittHandle_name = __itt_string_handle_create(arg.name);
        }
        وگرنه
        {
            ittHandle_name = 0;
        }
#endif
    }
};

ایستا تهی initTraceArg(TraceManagerThreadLocal& ctx, ثابت TraceArg& arg)
{
    TraceArg::ExtraData** pExtra = arg.ppExtra;
    اگر (*pExtra == پوچ)
    {
        cv::AutoLock lock(cv::getInitializationMutex());
        اگر (*pExtra == پوچ)
        {
            *pExtra = جدید TraceArg::ExtraData(ctx, arg);
        }
    }
}
تهی traceArg(ثابت TraceArg& arg, ثابت نویسه* value)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    Region* region = ctx.getCurrentActiveRegion();
    اگر (!region)
        بازگشت;
    CV_Assert(region->pImpl);
    initTraceArg(ctx, arg);
    اگر (!value)
        value = "<null>";
#ifdef OPENCV_WITH_ITT
    اگر (isITTEnabled())
    {
        __itt_metadata_str_add(domain, region->pImpl->itt_id, (*arg.ppExtra)->ittHandle_name, value, strlen(value));
    }
#endif
}
تهی traceArg(ثابت TraceArg& arg, صحیح value)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    Region* region = ctx.getCurrentActiveRegion();
    اگر (!region)
        بازگشت;
    CV_Assert(region->pImpl);
    initTraceArg(ctx, arg);
#ifdef OPENCV_WITH_ITT
    اگر (isITTEnabled())
    {
        __itt_metadata_add(domain, region->pImpl->itt_id, (*arg.ppExtra)->ittHandle_name, اندازه(صحیح) == 4 ? __itt_metadata_s32 : __itt_metadata_s64, 1, &value);
    }
#وگرنه
    CV_UNUSED(value);
#endif
}
تهی traceArg(ثابت TraceArg& arg, int64 value)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    Region* region = ctx.getCurrentActiveRegion();
    اگر (!region)
        بازگشت;
    CV_Assert(region->pImpl);
    initTraceArg(ctx, arg);
#ifdef OPENCV_WITH_ITT
    اگر (isITTEnabled())
    {
        __itt_metadata_add(domain, region->pImpl->itt_id, (*arg.ppExtra)->ittHandle_name, __itt_metadata_s64, 1, &value);
    }
#وگرنه
    CV_UNUSED(value);
#endif
}
تهی traceArg(ثابت TraceArg& arg, دوگانه value)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    Region* region = ctx.getCurrentActiveRegion();
    اگر (!region)
        بازگشت;
    CV_Assert(region->pImpl);
    initTraceArg(ctx, arg);
#ifdef OPENCV_WITH_ITT
    اگر (isITTEnabled())
    {
        __itt_metadata_add(domain, region->pImpl->itt_id, (*arg.ppExtra)->ittHandle_name, __itt_metadata_double, 1, &value);
    }
#وگرنه
    CV_UNUSED(value);
#endif
}

#وگرنه

Region::Region(ثابت LocationStaticStorage&) : pImpl(پوچ), implFlags(0) {}
تهی Region::destroy() {}

تهی traceArg(ثابت TraceArg&, ثابت نویسه*) {}
تهی traceArg(ثابت TraceArg&, صحیح) {};
تهی traceArg(ثابت TraceArg&, int64) {};
تهی traceArg(ثابت TraceArg&, دوگانه) {};

#endif

}}}} // namespace
