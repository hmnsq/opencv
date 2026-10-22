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
#define CV_LOG(...) CV_LOG_INFO(عدم, __VA_ARGS__)
#وإلا
#define CV_LOG(...) {}
#endif

#إذا 0
#define CV_LOG_ITT(...) CV_LOG_INFO(عدم, __VA_ARGS__)
#وإلا
#define CV_LOG_ITT(...) {}
#endif

#إذا 1
#define CV_LOG_TRACE_BAILOUT(tag, ...) CV_LOG_INFO(tag, __VA_ARGS__)
#وإلا
#define CV_LOG_TRACE_BAILOUT(...) {}
#endif

#إذا 0
#define CV_LOG_PARALLEL(tag, ...) CV_LOG_INFO(tag, __VA_ARGS__)
#وإلا
#define CV_LOG_PARALLEL(...) {}
#endif

#إذا 0
#define CV_LOG_CTX_STAT(tag, ...) CV_LOG_INFO(tag, __VA_ARGS__)
#وإلا
#define CV_LOG_CTX_STAT(...) {}
#endif

#إذا 0
#define CV_LOG_SKIP(tag, ...) CV_LOG_INFO(tag, __VA_ARGS__)
#وإلا
#define CV_LOG_SKIP(...) {}
#endif

نطاق cv {
نطاق utils {
نطاق trace {
نطاق details {

#ifdef OPENCV_TRACE

#ifdef _MSC_VER
#pragma warning(disable:4065) // switch statement contains 'default' but no 'case' labels
#endif

ساكن منطقي getParameterTraceEnable()
{
    ساكن منطقي param_traceEnable = utils::getConfigurationParameterBool("OPENCV_TRACE", خطأ);
    أرجع param_traceEnable;
}

// TODO lazy configuration flags
ساكن صحيح param_maxRegionDepthOpenCV = (صحيح)utils::getConfigurationParameterSizeT("OPENCV_TRACE_DEPTH_OPENCV", 1);
ساكن صحيح param_maxRegionChildrenOpenCV = (صحيح)utils::getConfigurationParameterSizeT("OPENCV_TRACE_MAX_CHILDREN_OPENCV", 1000);
ساكن صحيح param_maxRegionChildren = (صحيح)utils::getConfigurationParameterSizeT("OPENCV_TRACE_MAX_CHILDREN", 10000);

ساكن ثابت cv::String& getParameterTraceLocation()
{
    ساكن cv::String param_traceLocation = utils::getConfigurationParameterString("OPENCV_TRACE_LOCATION", "OpenCVTrace");
    أرجع param_traceLocation;
}

#ifdef HAVE_OPENCL
ساكن منطقي param_synchronizeOpenCL = utils::getConfigurationParameterBool("OPENCV_TRACE_SYNC_OPENCL", خطأ);
#endif

#ifdef OPENCV_WITH_ITT
ساكن منطقي param_ITT_registerParentScope = utils::getConfigurationParameterBool("OPENCV_TRACE_ITT_PARENT", خطأ);
#endif

ساكن ثابت حرف* _spaces(صحيح سانى_2)
{
    ساكن ثابت حرف buf[64] =
"                                                               ";
    أرجع &buf[63 - (سانى_2 & 63)];
}

/**
 * Text-based trace messages
 */
صنف TraceMessage
{
عام:
    حرف buffer[1024];
    size_t len;
    منطقي hasError;

    TraceMessage() :
        len(0),
        hasError(خطأ)
    {}

    منطقي printf(ثابت حرف* format, ...)
    {
        حرف* buf = &buffer[len];
        size_t sz = حجم(buffer) - len;
        va_list ap;
        va_start(ap, format);
        صحيح n = cv_vsnprintf(buf, (صحيح)sz, format, ap);
        va_end(ap);
        إذا (n < 0 || (size_t)n > sz)
        {
            hasError = true;
            أرجع خطأ;
        }
        len += n;
        أرجع true;
    }

    منطقي formatlocation(ثابت Region::LocationStaticStorage& location)
    {
        أرجع هذا->printf("l,%lld,\"%s\",%d,\"%s\",0x%llX\n",
                (long long صحيح)(*location.ppExtra)->global_location_id,
                location.filename,
                location.قۇر,
                location.name,
                (long long صحيح)(location.flags & ~0xF0000000));
    }
    منطقي formatRegionEnter(ثابت Region& region)
    {
        منطقي ok = هذا->printf("b,%d,%lld,%lld,%lld",
                (صحيح)region.pImpl->threadID,
                (long long صحيح)region.pImpl->beginTimestamp,
                (long long صحيح)((*region.pImpl->location.ppExtra)->global_location_id),
                (long long صحيح)region.pImpl->global_region_id);
        إذا (region.pImpl->parentRegion && region.pImpl->parentRegion->pImpl)
        {
            إذا (region.pImpl->parentRegion->pImpl->threadID != region.pImpl->threadID)
                ok &= هذا->printf(",parentThread=%d,parent=%lld",
                        (صحيح)region.pImpl->parentRegion->pImpl->threadID,
                        (long long صحيح)region.pImpl->parentRegion->pImpl->global_region_id);
        }
        ok &= هذا->printf("\n");
        أرجع ok;
    }
    منطقي formatRegionLeave(ثابت Region& region, ثابت RegionStatistics& result)
    {
        CV_DbgAssert(region.pImpl->endTimestamp - region.pImpl->beginTimestamp == result.duration);
        منطقي ok = هذا->printf("e,%d,%lld,%lld,%lld,%lld",
                (صحيح)region.pImpl->threadID,
                (long long صحيح)region.pImpl->endTimestamp,
                (long long صحيح)(*region.pImpl->location.ppExtra)->global_location_id,
                (long long صحيح)region.pImpl->global_region_id,
                (long long صحيح)result.duration);
        إذا (result.currentSkippedRegions)
            ok &= هذا->printf(",skip=%d", (صحيح)result.currentSkippedRegions);
#ifdef HAVE_IPP
        إذا (result.durationImplIPP)
            ok &= هذا->printf(",tIPP=%lld", (long long صحيح)result.durationImplIPP);
#endif
#ifdef HAVE_OPENCL
        إذا (result.durationImplOpenCL)
            ok &= هذا->printf(",tOCL=%lld", (long long صحيح)result.durationImplOpenCL);
#endif
        ok &= هذا->printf("\n");
        أرجع ok;
    }
    منطقي recordRegionArg(ثابت Region& region, ثابت TraceArg& arg, ثابت حرف* value)
    {
        أرجع هذا->printf("a,%d,%lld,%lld,\"%s\",\"%s\"\n",
                region.pImpl->threadID,
                (long long صحيح)region.pImpl->beginTimestamp,
                (long long صحيح)region.pImpl->global_region_id,
                arg.name,
                value);
    }
};


#ifdef OPENCV_WITH_ITT
ساكن __itt_domain* domain = عدم;

ساكن منطقي isITTEnabled()
{
    ساكن متطاير منطقي isInitialized = خطأ;
    ساكن منطقي isEnabled = خطأ;
    إذا (!isInitialized)
    {
        cv::AutoLock lock(cv::getInitializationMutex());
        إذا (!isInitialized)
        {
            منطقي param_traceITTEnable = utils::getConfigurationParameterBool("OPENCV_TRACE_ITT_ENABLE", true);
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
            isInitialized = true;
        }
    }
    أرجع isEnabled;
}
#endif


Region::LocationExtraData::LocationExtraData(ثابت LocationStaticStorage& location)
{
    CV_UNUSED(location);
    ساكن صحيح g_location_id_counter = 0;
    global_location_id = CV_XADD(&g_location_id_counter, 1) + 1;
    CV_LOG("Register location: " << global_location_id << " (" << (فراغ*)&location << ")"
            << std::endl << "    file: " << location.filename
            << std::endl << "    line: " << location.قۇر
            << std::endl << "    name: " << location.name);
#ifdef OPENCV_WITH_ITT
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
#endif
}

/*static*/ Region::LocationExtraData* Region::LocationExtraData::init(ثابت Region::LocationStaticStorage& location)
{
    LocationExtraData** pLocationExtra = location.ppExtra;
    CV_DbgAssert(pLocationExtra);
    إذا (*pLocationExtra == عدم)
    {
        cv::AutoLock lock(cv::getInitializationMutex());
        إذا (*pLocationExtra == عدم)
        {
            *pLocationExtra = جديد Region::LocationExtraData(location);
            TraceStorage* s = getTraceManager().trace_storage.get();
            إذا (s)
            {
                TraceMessage ئۇچۇر;
                ئۇچۇر.formatlocation(location);
                s->put(ئۇچۇر);
            }
        }
    }
    أرجع *pLocationExtra;
}


Region::Impl::Impl(TraceManagerThreadLocal& ctx, Region* parentRegion_, Region& region_, ثابت LocationStaticStorage& location_, int64 beginTimestamp_) :
    location(location_),
    region(region_),
    parentRegion(parentRegion_),
    threadID(ctx.threadID),
    global_region_id(++ctx.region_counter),
    beginTimestamp(beginTimestamp_),
    endTimestamp(0),
    direct_Children_سانى(0)
#ifdef OPENCV_WITH_ITT
    ,itt_id_registered(خطأ)
    ,itt_id(__itt_null)
#endif
{
    CV_DbgAssert(ctx.currentActiveRegion == parentRegion);
    region.pImpl = هذا;

    registerRegion(ctx);

    enterRegion(ctx);
}

Region::Impl::~Impl()
{
#ifdef OPENCV_WITH_ITT
    إذا (itt_id_registered)
    {
        CV_LOG_ITT(" Destroy ITT region: I=" << (فراغ*)هذا);
        __itt_id_destroy(domain, itt_id);
        itt_id_registered = خطأ;
    }
#endif
    region.pImpl = عدم;
}

فراغ Region::Impl::enterRegion(TraceManagerThreadLocal& ctx)
{
    ctx.currentActiveRegion = &region;

    إذا (location.flags & REGION_FLAG_FUNCTION)
    {
        إذا ((location.flags & REGION_FLAG_APP_CODE) == 0)
        {
            ctx.regionDepthOpenCV++;
        }
        ctx.regionDepth++;
    }

    TraceStorage* s = ctx.getStorage();
    إذا (s)
    {
        TraceMessage ئۇچۇر;
        ئۇچۇر.formatRegionEnter(region);
        s->put(ئۇچۇر);
    }
#ifdef OPENCV_WITH_ITT
    إذا (isITTEnabled())
    {
        __itt_id parentID = __itt_null;
        إذا (param_ITT_registerParentScope && parentRegion && parentRegion->pImpl && parentRegion->pImpl->itt_id_registered && (location.flags & REGION_FLAG_REGION_FORCE) == 0)
            parentID = parentRegion->pImpl->itt_id;
        __itt_task_begin(domain, itt_id, parentID, (*location.ppExtra)->ittHandle_name);
    }
#endif
}

فراغ Region::Impl::leaveRegion(TraceManagerThreadLocal& ctx)
{
    int64 duration = endTimestamp - beginTimestamp; CV_UNUSED(duration);
    RegionStatistics result;
    ctx.stat.grab(result);
    ctx.totalSkippedEvents += result.currentSkippedRegions;
    CV_LOG(_spaces(ctx.getCurrentDepth()*4) << "leaveRegion(): " << (فراغ*)هذا << " " << result);
#ifdef OPENCV_WITH_ITT
    إذا (isITTEnabled())
    {
        إذا (result.currentSkippedRegions)
        {
            __itt_metadata_add(domain, itt_id, __itt_string_handle_create("skipped trace entries"), __itt_metadata_u32, 1, &result.currentSkippedRegions);
        }
#ifdef HAVE_IPP
        إذا (result.durationImplIPP)
            __itt_metadata_add(domain, itt_id, __itt_string_handle_create("tIPP"), __itt_metadata_u64, 1, &result.durationImplIPP);
#endif
#ifdef HAVE_OPENCL
        إذا (result.durationImplOpenCL)
            __itt_metadata_add(domain, itt_id, __itt_string_handle_create("tOpenCL"), __itt_metadata_u64, 1, &result.durationImplOpenCL);
#endif
        __itt_task_end(domain);
    }
#endif
    TraceStorage* s = ctx.getStorage();
    إذا (s)
    {
        TraceMessage ئۇچۇر;
        ئۇچۇر.formatRegionLeave(region, result);
        s->put(ئۇچۇر);
    }

    إذا (location.flags & REGION_FLAG_FUNCTION)
    {
        إذا ((location.flags & REGION_FLAG_APP_CODE) == 0)
        {
            ctx.regionDepthOpenCV--;
        }
        ctx.regionDepth--;
    }

    ctx.currentActiveRegion = parentRegion;
}

فراغ Region::Impl::release()
{
    delete هذا;
}

فراغ Region::Impl::registerRegion(TraceManagerThreadLocal& ctx)
{
#ifdef OPENCV_WITH_ITT
    إذا (isITTEnabled())
    {
        إذا (!itt_id_registered)
        {
            CV_LOG_ITT(" Register ITT region: I=" << (فراغ*)هذا << " " << ctx.threadID << "-" << global_region_id);
#إذا 1 // workaround for some ITT backends
            itt_id = __itt_id_make((فراغ*)(intptr_t)(((int64)(ctx.threadID + 1) << 32) | global_region_id), global_region_id);
#وإلا
            itt_id = __itt_id_make((فراغ*)(intptr_t)(ctx.threadID + 1), global_region_id);
#endif
            __itt_id_create(domain, itt_id);
            itt_id_registered = true;
        }
    }
#وإلا
    CV_UNUSED(ctx);
#endif
}

فراغ RegionStatisticsStatus::enableSkipMode(صحيح depth)
{
    CV_DbgAssert(_skipDepth < 0);
    CV_LOG_SKIP(عدم, "SKIP-ENABLE: depth=" << depth);
    _skipDepth = depth;
}
فراغ RegionStatisticsStatus::checkResetSkipMode(صحيح leaveDepth)
{
    إذا (leaveDepth <= _skipDepth)
    {
        CV_LOG_SKIP(عدم, "SKIP-RESET: leaveDepth=" << leaveDepth << " skipDepth=" << _skipDepth);
        _skipDepth = -1;
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
    CV_LOG(_spaces(ctx.getCurrentDepth()*4) << "Region(): " << (فراغ*)هذا << ": " << location.name);

    Region* parentRegion = ctx.دۆۋە_چوققا_Region();
    ثابت Region::LocationStaticStorage* parentLocation = ctx.دۆۋە_چوققا_Location();

    إذا (location.flags & REGION_FLAG_REGION_NEXT)
    {
        إذا (parentRegion && parentRegion->pImpl)
        {
            CV_DbgAssert((parentRegion->pImpl->location.flags & REGION_FLAG_FUNCTION) == 0);
            parentRegion->destroy(); parentRegion->implFlags = 0;
            parentRegion = ctx.دۆۋە_چوققا_Region();
            parentLocation = ctx.دۆۋە_چوققا_Location();
        }
    }

    صحيح parentChildren = 0;
    إذا (parentRegion && parentRegion->pImpl)
    {
        إذا (parentLocation == عدم)
        {
            // parallel_for_body code path
            parentChildren = CV_XADD(&parentRegion->pImpl->direct_Children_سانى, 1) + 1;
        }
        وإلا
        {
            parentChildren = ++parentRegion->pImpl->direct_Children_سانى;
        }
    }

    int64 beginTimestamp = getTimestampNS();

    صحيح currentDepth = ctx.getCurrentDepth() + 1;
    بدل (location.flags & REGION_FLAG_IMPL_MASK)
    {
#ifdef HAVE_IPP
    حالة REGION_FLAG_IMPL_IPP:
        إذا (!ctx.stat_status.ignoreDepthImplIPP)
            ctx.stat_status.ignoreDepthImplIPP = currentDepth;
        اكسر;
#endif
#ifdef HAVE_OPENCL
    حالة REGION_FLAG_IMPL_OPENCL:
        إذا (!ctx.stat_status.ignoreDepthImplOpenCL)
            ctx.stat_status.ignoreDepthImplOpenCL = currentDepth;
        اكسر;
#endif
    افتراضي:
        اكسر;
    }

    ctx.دۆۋە_ئىتتىرىش(هذا, &location, beginTimestamp);
    implFlags |= REGION_FLAG_NEED_دۆۋە_چىقىرىش;

    إذا ((location.flags & REGION_FLAG_REGION_FORCE) == 0)
    {
        إذا (ctx.stat_status._skipDepth >= 0 && currentDepth > ctx.stat_status._skipDepth)
        {
            CV_LOG(_spaces(ctx.getCurrentDepth()*4) << "Parent region is disabled. Bailout");
            ctx.stat.currentSkippedRegions++;
            أرجع;
        }

        إذا (param_maxRegionChildrenOpenCV > 0 && (location.flags & REGION_FLAG_APP_CODE) == 0 && parentLocation && (parentLocation->flags & REGION_FLAG_APP_CODE) == 0)
        {
            إذا (parentChildren >= param_maxRegionChildrenOpenCV)
            {
                CV_LOG_TRACE_BAILOUT(عدم, _spaces(ctx.getCurrentDepth()*4) << "OpenCV parent region exceeds children count. Bailout");
                ctx.stat_status.enableSkipMode(currentDepth - 1);
                ctx.stat.currentSkippedRegions++;
                DEBUG_ONLY(ctx.dump_دۆۋە(std::cout, خطأ));
                أرجع;
            }
        }
        إذا (param_maxRegionChildren > 0 && parentChildren >= param_maxRegionChildren)
        {
            CV_LOG_TRACE_BAILOUT(عدم, _spaces(ctx.getCurrentDepth()*4) << "Parent region exceeds children count. Bailout");
            ctx.stat_status.enableSkipMode(currentDepth - 1);
            ctx.stat.currentSkippedRegions++;
            DEBUG_ONLY(ctx.dump_دۆۋە(std::cout, خطأ));
            أرجع;
        }
    }

    LocationExtraData::init(location);

    إذا ((*location.ppExtra)->global_location_id == 0)
    {
        CV_LOG_TRACE_BAILOUT(عدم, _spaces(ctx.getCurrentDepth()*4) << "Region location is disabled. Bailout");
        ctx.stat_status.enableSkipMode(currentDepth);
        ctx.stat.currentSkippedRegions++;
        أرجع;
    }

    إذا (parentLocation && (parentLocation->flags & REGION_FLAG_SKIP_NESTED))
    {
        CV_LOG(_spaces(ctx.getCurrentDepth()*4) << "Parent region disables inner regions. Bailout");
        ctx.stat_status.enableSkipMode(currentDepth);
        ctx.stat.currentSkippedRegions++;
        أرجع;
    }

    إذا (param_maxRegionDepthOpenCV)
    {
        إذا ((location.flags & REGION_FLAG_APP_CODE) == 0)
        {
            إذا (ctx.regionDepthOpenCV >= param_maxRegionDepthOpenCV)
            {
                CV_LOG(_spaces(ctx.getCurrentDepth()*4) << "OpenCV region depth is exceed = " << ctx.regionDepthOpenCV << ". Bailout");
                إذا (ctx.stat.currentSkippedRegions == 0)
                {
                    DEBUG_ONLY(ctx.dump_دۆۋە(std::cout, خطأ));
                }
                ctx.stat_status.enableSkipMode(currentDepth);
                ctx.stat.currentSkippedRegions++;
                أرجع;
            }
        }
    }

    جديد Impl(ctx, parentRegion, *هذا, location, beginTimestamp);
    CV_DbgAssert(pImpl != عدم);
    implFlags |= REGION_FLAG__ACTIVE;

    // parallel_for path
    إذا (parentRegion && parentRegion->pImpl)
    {
        إذا (parentLocation == عدم)
        {
            pImpl->direct_Children_سانى = parentChildren;
        }
    }
}

فراغ Region::destroy()
{
    CV_DbgAssert(implFlags != 0);

    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    CV_LOG(_spaces(ctx.getCurrentDepth()*4) << "Region::destruct(): " << (فراغ*)هذا << " pImpl=" << pImpl << " implFlags=" << implFlags << ' ' << (ctx.دۆۋە_چوققا_Location() ? ctx.دۆۋە_چوققا_Location()->name : "<unknown>"));

    CV_DbgAssert(implFlags & REGION_FLAG_NEED_دۆۋە_چىقىرىش);
    ثابت صحيح currentDepth = ctx.getCurrentDepth(); CV_UNUSED(currentDepth);

    CV_LOG_CTX_STAT(عدم, _spaces(currentDepth*4) << ctx.stat << ' ' << ctx.stat_status);

    ثابت Region::LocationStaticStorage* location = ctx.دۆۋە_چوققا_Location();
    Impl::OptimizationPath myCodePath = Impl::CODE_PATH_PLAIN;
    إذا (location)
    {
        بدل (location->flags & REGION_FLAG_IMPL_MASK)
        {
#ifdef HAVE_IPP
        حالة REGION_FLAG_IMPL_IPP:
            myCodePath = Impl::CODE_PATH_IPP;
            اكسر;
#endif
#ifdef HAVE_OPENCL
        حالة REGION_FLAG_IMPL_OPENCL:
            إذا (param_synchronizeOpenCL && cv::ocl::isOpenCLActivated())
                cv::ocl::finish();
            myCodePath = Impl::CODE_PATH_OPENCL;
            اكسر;
#endif
        افتراضي:
            اكسر;
        }
    }

    int64 endTimestamp = getTimestampNS();
    int64 duration = endTimestamp - ctx.دۆۋە_چوققا_Begin_Timestamp();

    منطقي active = isActive();

    إذا (active)
        ctx.stat.duration = duration;
    وإلا إذا (ctx.دۆۋە.size() == ctx.parallel_for_دۆۋە_size + 1)
        ctx.stat.duration += duration;

    بدل (myCodePath) {
        حالة Impl::CODE_PATH_PLAIN:
            // nothing
            اكسر;
#ifdef HAVE_IPP
        حالة Impl::CODE_PATH_IPP:
            إذا (ctx.stat_status.ignoreDepthImplIPP == currentDepth)
            {
                ctx.stat.durationImplIPP += duration;
                ctx.stat_status.ignoreDepthImplIPP = 0;
            }
            وإلا إذا (active)
            {
                ctx.stat.durationImplIPP = duration;
            }
            اكسر;
#endif
#ifdef HAVE_OPENCL
        حالة Impl::CODE_PATH_OPENCL:
            إذا (ctx.stat_status.ignoreDepthImplOpenCL == currentDepth)
            {
                ctx.stat.durationImplOpenCL += duration;
                ctx.stat_status.ignoreDepthImplOpenCL = 0;
            }
            وإلا إذا (active)
            {
                ctx.stat.durationImplOpenCL = duration;
            }
            اكسر;
#endif
        افتراضي:
            اكسر;
    }

    إذا (pImpl)
    {
        CV_DbgAssert((implFlags & (REGION_FLAG__ACTIVE | REGION_FLAG_NEED_دۆۋە_چىقىرىش)) == (REGION_FLAG__ACTIVE | REGION_FLAG_NEED_دۆۋە_چىقىرىش));
        CV_DbgAssert(ctx.دۆۋە_چوققا_Region() == هذا);
        pImpl->endTimestamp = endTimestamp;
        pImpl->leaveRegion(ctx);
        pImpl->release();
        pImpl = عدم;
        DEBUG_ONLY(implFlags &= ~REGION_FLAG__ACTIVE);
    }
    وإلا
    {
        CV_DbgAssert(ctx.stat_status._skipDepth <= currentDepth);
    }

    إذا (implFlags & REGION_FLAG_NEED_دۆۋە_چىقىرىش)
    {
        CV_DbgAssert(ctx.دۆۋە_چوققا_Region() == هذا);
        ctx.دۆۋە_چىقىرىش();
        ctx.stat_status.checkResetSkipMode(currentDepth);
        DEBUG_ONLY(implFlags &= ~REGION_FLAG_NEED_دۆۋە_چىقىرىش);
    }
    CV_LOG_CTX_STAT(عدم, _spaces(currentDepth*4) << "===> " << ctx.stat << ' ' << ctx.stat_status);
}


TraceManagerThreadLocal::~TraceManagerThreadLocal()
{
}

فراغ TraceManagerThreadLocal::dump_دۆۋە(std::ostream& out, منطقي onlyFunctions) ثابت
{
    std::stringstream ss;
    std::deque<دۆۋە_Entry>::const_iterator it = دۆۋە.begin();
    std::deque<دۆۋە_Entry>::const_iterator end = دۆۋە.end();
    صحيح depth = 0;
    لكل (; it != end; ++it)
    {
        ثابت Region::LocationStaticStorage* location = it->location;
        إذا (location)
        {
            إذا (!onlyFunctions || (location->flags & REGION_FLAG_FUNCTION))
            {
                ss << _spaces(4*depth) << location->name << std::endl;
                depth++;
            }
        }
        وإلا
        {
            ss << _spaces(4*depth) << "<unknown>" << std::endl;
            depth++;
        }
    }
    out << ss.str();
}

صنف AsyncTraceStorage CV_FINAL : عام TraceStorage
{
    mutable std::ofstream out;
عام:
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

    منطقي put(ثابت TraceMessage& ئۇچۇر) ثابت CV_OVERRIDE
    {
        إذا (ئۇچۇر.hasError)
            أرجع خطأ;
        out << ئۇچۇر.buffer;
        //DEBUG_ONLY(std::flush(out)); // TODO configure flag
        أرجع true;
    }
};

صنف SyncTraceStorage CV_FINAL : عام TraceStorage
{
    mutable std::ofstream out;
    mutable cv::Mutex mutex;
عام:
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

    منطقي put(ثابت TraceMessage& ئۇچۇر) ثابت CV_OVERRIDE
    {
        إذا (ئۇچۇر.hasError)
            أرجع خطأ;
        {
            cv::AutoLock l(mutex);
            out << ئۇچۇر.buffer;
            std::flush(out); // TODO configure flag
        }
        أرجع true;
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
            TraceMessage ئۇچۇر;
            ثابت حرف* pos = strrchr(filepath.c_str(), '/'); // extract filename
#ifdef _WIN32
            إذا (!pos)
                pos = strrchr(filepath.c_str(), '\\');
#endif
            إذا (!pos)
                pos = filepath.c_str();
            وإلا
                pos += 1; // fix to skip extra slash in filename beginning
            ئۇچۇر.printf("#thread file: %s\n", pos);
            global->put(ئۇچۇر);
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

    isInitialized = true;
    CV_LOG("TraceManager ctor: " << (فراغ*)هذا);

    CV_LOG("TraceManager configure()");
    activated = getParameterTraceEnable();

    إذا (activated)
        trace_storage.reset(جديد SyncTraceStorage(std::string(getParameterTraceLocation()) + ".txt"));

#ifdef OPENCV_WITH_ITT
    إذا (isITTEnabled())
    {
        activated = true; // force trace pipeline activation (without OpenCV storage)
        __itt_region_begin(domain, __itt_null, __itt_null, __itt_string_handle_create("OpenCVTrace"));
    }
#endif
}
TraceManager::~TraceManager()
{
    CV_LOG("TraceManager dtor: " << (فراغ*)هذا);

#ifdef OPENCV_WITH_ITT
    إذا (isITTEnabled())
    {
        __itt_region_end(domain, __itt_null);
    }
#endif

    std::vector<TraceManagerThreadLocal*> threads_ctx;
    tls.gather(threads_ctx);
    size_t totalEvents = 0, totalSkippedEvents = 0;
    لكل (size_t i = 0; i < threads_ctx.size(); i++)
    {
        TraceManagerThreadLocal* ctx = threads_ctx[i];
        إذا (ctx)
        {
            totalEvents += ctx->region_counter;
            totalSkippedEvents += ctx->totalSkippedEvents;
        }
    }
    إذا (totalEvents || activated)
    {
        CV_LOG_INFO(عدم, "Trace: Total events: " << totalEvents);
    }
    إذا (totalSkippedEvents)
    {
        CV_LOG_WARNING(عدم, "Trace: Total skipped events: " << totalSkippedEvents);
    }

    // This is a global static object, so process starts shutdown here
    // Turn off trace
    cv::__termination = true; // also set in DllMain() notifications handler for DLL_PROCESS_DETACH
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
        CV_UNUSED(m); // TODO
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

فراغ parallel_For_Set_يىلتىز_Region(ثابت Region& يىلتىز_Region, ثابت TraceManagerThreadLocal& يىلتىز_ctx)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();

    إذا (ctx.dummy_دۆۋە_چوققا.region == &يىلتىز_Region) // already attached
        أرجع;

    CV_Assert(ctx.dummy_دۆۋە_چوققا.region == عدم);
    ctx.dummy_دۆۋە_چوققا = TraceManagerThreadLocal::دۆۋە_Entry(const_cast<Region*>(&يىلتىز_Region), عدم, -1);

    إذا (&ctx == &يىلتىز_ctx)
    {
        ctx.stat.grab(ctx.parallel_for_stat);
        ctx.parallel_for_stat_status = ctx.stat_status;
        ctx.parallel_for_دۆۋە_size = ctx.دۆۋە.size();
        أرجع;
    }

    CV_Assert(ctx.دۆۋە.empty());

    ctx.currentActiveRegion = const_cast<Region*>(&يىلتىز_Region);

    ctx.regionDepth = يىلتىز_ctx.regionDepth;
    ctx.regionDepthOpenCV = يىلتىز_ctx.regionDepthOpenCV;

    ctx.parallel_for_دۆۋە_size = 0;

    ctx.stat_status.propagateFrom(يىلتىز_ctx.stat_status);
}

فراغ parallelForAttachNestedRegion(ثابت Region& يىلتىز_Region)
{
    CV_UNUSED(يىلتىز_Region);
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();

    CV_DbgAssert(ctx.dummy_دۆۋە_چوققا.region == &يىلتىز_Region);

    Region* region = ctx.getCurrentActiveRegion();
    CV_LOG_PARALLEL(عدم, " PARALLEL_FOR: " << (فراغ*)region << " ==> " << &يىلتىز_Region);
    إذا (!region)
        أرجع;

#ifdef OPENCV_WITH_ITT
    إذا (!يىلتىز_Region.pImpl || !يىلتىز_Region.pImpl->itt_id_registered)
        أرجع;

    إذا (!region->pImpl)
        أرجع;

    CV_LOG_PARALLEL(عدم, " PARALLEL_FOR ITT: " << (فراغ*)يىلتىز_Region.pImpl->itt_id.d1 << ":" << يىلتىز_Region.pImpl->itt_id.d2 << ":" << (فراغ*)يىلتىز_Region.pImpl->itt_id.d3 << " => "
                                 << (فراغ*)region->pImpl->itt_id.d1 << ":" << region->pImpl->itt_id.d2 << ":" << (فراغ*)region->pImpl->itt_id.d3);
    __itt_relation_add(domain, region->pImpl->itt_id, __itt_relation_is_child_of, يىلتىز_Region.pImpl->itt_id);
#endif
}

فراغ parallelForFinalize(ثابت Region& يىلتىز_Region)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();

    int64 endTimestamp = getTimestampNS();
    int64 duration = endTimestamp - ctx.دۆۋە_چوققا_Begin_Timestamp();
    CV_LOG_PARALLEL(عدم, "parallel_for duration: " << duration << " " << &يىلتىز_Region);

    std::vector<TraceManagerThreadLocal*> threads_ctx;
    getTraceManager().tls.gather(threads_ctx);
    RegionStatistics parallel_for_stat;
    لكل (size_t i = 0; i < threads_ctx.size(); i++)
    {
        TraceManagerThreadLocal* child_ctx = threads_ctx[i];

        إذا (child_ctx && child_ctx->دۆۋە_چوققا_Region() == &يىلتىز_Region)
        {
            CV_LOG_PARALLEL(عدم, "Thread=" << child_ctx->threadID << " " << child_ctx->stat);
            RegionStatistics child_stat;
            child_ctx->stat.grab(child_stat);
            parallel_for_stat.append(child_stat);
            إذا (child_ctx != &ctx)
            {
                child_ctx->dummy_دۆۋە_چوققا = TraceManagerThreadLocal::دۆۋە_Entry();
            }
            وإلا
            {
                ctx.parallel_for_stat.grab(ctx.stat);
                ctx.stat_status = ctx.parallel_for_stat_status;
                child_ctx->dummy_دۆۋە_چوققا = TraceManagerThreadLocal::دۆۋە_Entry();
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

بنية TraceArg::ExtraData
{
#ifdef OPENCV_WITH_ITT
    // Special fields for ITT
    __itt_string_handle* متطاير ittHandle_name;
#endif
    ExtraData(TraceManagerThreadLocal& ctx, ثابت TraceArg& arg)
    {
        CV_UNUSED(ctx); CV_UNUSED(arg);
#ifdef OPENCV_WITH_ITT
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
#endif
    }
};

ساكن فراغ initTraceArg(TraceManagerThreadLocal& ctx, ثابت TraceArg& arg)
{
    TraceArg::ExtraData** pExtra = arg.ppExtra;
    إذا (*pExtra == عدم)
    {
        cv::AutoLock lock(cv::getInitializationMutex());
        إذا (*pExtra == عدم)
        {
            *pExtra = جديد TraceArg::ExtraData(ctx, arg);
        }
    }
}
فراغ traceArg(ثابت TraceArg& arg, ثابت حرف* value)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    Region* region = ctx.getCurrentActiveRegion();
    إذا (!region)
        أرجع;
    CV_Assert(region->pImpl);
    initTraceArg(ctx, arg);
    إذا (!value)
        value = "<null>";
#ifdef OPENCV_WITH_ITT
    إذا (isITTEnabled())
    {
        __itt_metadata_str_add(domain, region->pImpl->itt_id, (*arg.ppExtra)->ittHandle_name, value, strlen(value));
    }
#endif
}
فراغ traceArg(ثابت TraceArg& arg, صحيح value)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    Region* region = ctx.getCurrentActiveRegion();
    إذا (!region)
        أرجع;
    CV_Assert(region->pImpl);
    initTraceArg(ctx, arg);
#ifdef OPENCV_WITH_ITT
    إذا (isITTEnabled())
    {
        __itt_metadata_add(domain, region->pImpl->itt_id, (*arg.ppExtra)->ittHandle_name, حجم(صحيح) == 4 ? __itt_metadata_s32 : __itt_metadata_s64, 1, &value);
    }
#وإلا
    CV_UNUSED(value);
#endif
}
فراغ traceArg(ثابت TraceArg& arg, int64 value)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    Region* region = ctx.getCurrentActiveRegion();
    إذا (!region)
        أرجع;
    CV_Assert(region->pImpl);
    initTraceArg(ctx, arg);
#ifdef OPENCV_WITH_ITT
    إذا (isITTEnabled())
    {
        __itt_metadata_add(domain, region->pImpl->itt_id, (*arg.ppExtra)->ittHandle_name, __itt_metadata_s64, 1, &value);
    }
#وإلا
    CV_UNUSED(value);
#endif
}
فراغ traceArg(ثابت TraceArg& arg, مزدوج value)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    Region* region = ctx.getCurrentActiveRegion();
    إذا (!region)
        أرجع;
    CV_Assert(region->pImpl);
    initTraceArg(ctx, arg);
#ifdef OPENCV_WITH_ITT
    إذا (isITTEnabled())
    {
        __itt_metadata_add(domain, region->pImpl->itt_id, (*arg.ppExtra)->ittHandle_name, __itt_metadata_double, 1, &value);
    }
#وإلا
    CV_UNUSED(value);
#endif
}

#وإلا

Region::Region(ثابت LocationStaticStorage&) : pImpl(عدم), implFlags(0) {}
فراغ Region::destroy() {}

فراغ traceArg(ثابت TraceArg&, ثابت حرف*) {}
فراغ traceArg(ثابت TraceArg&, صحيح) {};
فراغ traceArg(ثابت TraceArg&, int64) {};
فراغ traceArg(ثابت TraceArg&, مزدوج) {};

#endif

}}}} // namespace
