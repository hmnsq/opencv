// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#অন্তর্ভাব "precomp.hpp"

#অন্তর্ভাব <opencv2/core/utils/trace.hpp>
#অন্তর্ভাব <opencv2/core/utils/trace.private.hpp>
#অন্তর্ভাব <opencv2/core/utils/configuration.private.hpp>

#অন্তর্ভাব <opencv2/core/opencl/ocl_defs.hpp>

#অন্তর্ভাব <cstdarg> // va_start

#অন্তর্ভাব <sstream>
#অন্তর্ভাব <ostream>
#অন্তর্ভাব <fstream>

#যদি 0
#define CV_LOG(...) CV_LOG_INFO(ফাঁকা, __VA_ARGS__)
#অন্যথা
#define CV_LOG(...) {}
#endif

#যদি 0
#define CV_LOG_ITT(...) CV_LOG_INFO(ফাঁকা, __VA_ARGS__)
#অন্যথা
#define CV_LOG_ITT(...) {}
#endif

#যদি 1
#define CV_LOG_TRACE_BAILOUT(tag, ...) CV_LOG_INFO(tag, __VA_ARGS__)
#অন্যথা
#define CV_LOG_TRACE_BAILOUT(...) {}
#endif

#যদি 0
#define CV_LOG_PARALLEL(tag, ...) CV_LOG_INFO(tag, __VA_ARGS__)
#অন্যথা
#define CV_LOG_PARALLEL(...) {}
#endif

#যদি 0
#define CV_LOG_CTX_STAT(tag, ...) CV_LOG_INFO(tag, __VA_ARGS__)
#অন্যথা
#define CV_LOG_CTX_STAT(...) {}
#endif

#যদি 0
#define CV_LOG_SKIP(tag, ...) CV_LOG_INFO(tag, __VA_ARGS__)
#অন্যথা
#define CV_LOG_SKIP(...) {}
#endif

নামাকাশ cv {
নামাকাশ utils {
নামাকাশ trace {
নামাকাশ details {

#ifdef OPENCV_TRACE

#ifdef _MSC_VER
#pragma warning(disable:4065) // switch statement contains 'default' but no 'case' labels
#endif

স্থির যৌক্তিক getParameterTraceEnable()
{
    স্থির যৌক্তিক param_traceEnable = utils::getConfigurationParameterBool("OPENCV_TRACE", মিথ্যা);
    ফেরত param_traceEnable;
}

// TODO lazy configuration flags
স্থির পূর্ণসংখ্যা param_maxRegionDepthOpenCV = (পূর্ণসংখ্যা)utils::get_Configuration_Parameter_আকার_T("OPENCV_TRACE_DEPTH_OPENCV", 1);
স্থির পূর্ণসংখ্যা param_maxRegionChildrenOpenCV = (পূর্ণসংখ্যা)utils::get_Configuration_Parameter_আকার_T("OPENCV_TRACE_MAX_CHILDREN_OPENCV", 1000);
স্থির পূর্ণসংখ্যা param_maxRegionChildren = (পূর্ণসংখ্যা)utils::get_Configuration_Parameter_আকার_T("OPENCV_TRACE_MAX_CHILDREN", 10000);

স্থির ধ্রুবক cv::String& getParameterTraceLocation()
{
    স্থির cv::String param_traceLocation = utils::getConfigurationParameterString("OPENCV_TRACE_LOCATION", "OpenCVTrace");
    ফেরত param_traceLocation;
}

#ifdef HAVE_OPENCL
স্থির যৌক্তিক param_synchronizeOpenCL = utils::getConfigurationParameterBool("OPENCV_TRACE_SYNC_OPENCL", মিথ্যা);
#endif

#ifdef OPENCV_WITH_ITT
স্থির যৌক্তিক param_ITT_registerParentScope = utils::getConfigurationParameterBool("OPENCV_TRACE_ITT_PARENT", মিথ্যা);
#endif

স্থির ধ্রুবক অক্ষর* _spaces(পূর্ণসংখ্যা গুনতি_2)
{
    স্থির ধ্রুবক অক্ষর buf[64] =
"                                                               ";
    ফেরত &buf[63 - (গুনতি_2 & 63)];
}

/**
 * Text-based trace messages
 */
শ্রেণী TraceMessage
{
সর্বজনীন:
    অক্ষর বাফার[1024];
    size_t len;
    যৌক্তিক hasError;

    TraceMessage() :
        len(0),
        hasError(মিথ্যা)
    {}

    যৌক্তিক printf(ধ্রুবক অক্ষর* format, ...)
    {
        অক্ষর* buf = &বাফার[len];
        size_t sz = আকার(বাফার) - len;
        va_list ap;
        va_start(ap, format);
        পূর্ণসংখ্যা n = cv_vsnprintf(buf, (পূর্ণসংখ্যা)sz, format, ap);
        va_end(ap);
        যদি (n < 0 || (size_t)n > sz)
        {
            hasError = সত্য;
            ফেরত মিথ্যা;
        }
        len += n;
        ফেরত সত্য;
    }

    যৌক্তিক formatlocation(ধ্রুবক Region::LocationStaticStorage& location)
    {
        ফেরত এই->printf("l,%lld,\"%s\",%d,\"%s\",0x%llX\n",
                (long long পূর্ণসংখ্যা)(*location.ppExtra)->global_location_id,
                location.filename,
                location.সারি,
                location.name,
                (long long পূর্ণসংখ্যা)(location.flags & ~0xF0000000));
    }
    যৌক্তিক formatRegionEnter(ধ্রুবক Region& region)
    {
        যৌক্তিক ok = এই->printf("b,%d,%lld,%lld,%lld",
                (পূর্ণসংখ্যা)region.pImpl->threadID,
                (long long পূর্ণসংখ্যা)region.pImpl->beginTimestamp,
                (long long পূর্ণসংখ্যা)((*region.pImpl->location.ppExtra)->global_location_id),
                (long long পূর্ণসংখ্যা)region.pImpl->global_region_id);
        যদি (region.pImpl->parentRegion && region.pImpl->parentRegion->pImpl)
        {
            যদি (region.pImpl->parentRegion->pImpl->threadID != region.pImpl->threadID)
                ok &= এই->printf(",parentThread=%d,parent=%lld",
                        (পূর্ণসংখ্যা)region.pImpl->parentRegion->pImpl->threadID,
                        (long long পূর্ণসংখ্যা)region.pImpl->parentRegion->pImpl->global_region_id);
        }
        ok &= এই->printf("\n");
        ফেরত ok;
    }
    যৌক্তিক formatRegionLeave(ধ্রুবক Region& region, ধ্রুবক RegionStatistics& ফলাফল)
    {
        CV_DbgAssert(region.pImpl->endTimestamp - region.pImpl->beginTimestamp == ফলাফল.duration);
        যৌক্তিক ok = এই->printf("e,%d,%lld,%lld,%lld,%lld",
                (পূর্ণসংখ্যা)region.pImpl->threadID,
                (long long পূর্ণসংখ্যা)region.pImpl->endTimestamp,
                (long long পূর্ণসংখ্যা)(*region.pImpl->location.ppExtra)->global_location_id,
                (long long পূর্ণসংখ্যা)region.pImpl->global_region_id,
                (long long পূর্ণসংখ্যা)ফলাফল.duration);
        যদি (ফলাফল.currentSkippedRegions)
            ok &= এই->printf(",skip=%d", (পূর্ণসংখ্যা)ফলাফল.currentSkippedRegions);
#ifdef HAVE_IPP
        যদি (ফলাফল.durationImplIPP)
            ok &= এই->printf(",tIPP=%lld", (long long পূর্ণসংখ্যা)ফলাফল.durationImplIPP);
#endif
#ifdef HAVE_OPENCL
        যদি (ফলাফল.durationImplOpenCL)
            ok &= এই->printf(",tOCL=%lld", (long long পূর্ণসংখ্যা)ফলাফল.durationImplOpenCL);
#endif
        ok &= এই->printf("\n");
        ফেরত ok;
    }
    যৌক্তিক recordRegionArg(ধ্রুবক Region& region, ধ্রুবক TraceArg& arg, ধ্রুবক অক্ষর* value)
    {
        ফেরত এই->printf("a,%d,%lld,%lld,\"%s\",\"%s\"\n",
                region.pImpl->threadID,
                (long long পূর্ণসংখ্যা)region.pImpl->beginTimestamp,
                (long long পূর্ণসংখ্যা)region.pImpl->global_region_id,
                arg.name,
                value);
    }
};


#ifdef OPENCV_WITH_ITT
স্থির __itt_domain* domain = ফাঁকা;

স্থির যৌক্তিক isITTEnabled()
{
    স্থির অস্থির যৌক্তিক isInitialized = মিথ্যা;
    স্থির যৌক্তিক isEnabled = মিথ্যা;
    যদি (!isInitialized)
    {
        cv::AutoLock lock(cv::getInitializationMutex());
        যদি (!isInitialized)
        {
            যৌক্তিক param_traceITTEnable = utils::getConfigurationParameterBool("OPENCV_TRACE_ITT_ENABLE", সত্য);
            যদি (param_traceITTEnable)
            {
                isEnabled = !!(__itt_api_version());
                CV_LOG_ITT("ITT is " << (isEnabled ? "enabled" : "disabled"));
                domain = __itt_domain_create("OpenCVTrace");
            }
            অন্যথা
            {
                CV_LOG_ITT("ITT is disabled through OpenCV parameter");
                isEnabled = মিথ্যা;
            }
            isInitialized = সত্য;
        }
    }
    ফেরত isEnabled;
}
#endif


Region::Location_Extra_তথ্য::Location_Extra_তথ্য(ধ্রুবক LocationStaticStorage& location)
{
    CV_UNUSED(location);
    স্থির পূর্ণসংখ্যা g_location_id_গণক = 0;
    global_location_id = CV_XADD(&g_location_id_গণক, 1) + 1;
    CV_LOG("Register location: " << global_location_id << " (" << (শূন্য*)&location << ")"
            << std::endl << "    file: " << location.filename
            << std::endl << "    line: " << location.সারি
            << std::endl << "    name: " << location.name);
#ifdef OPENCV_WITH_ITT
    যদি (isITTEnabled())
    {
        // Caching is not required here, because there is builtin cache.
        // https://software.intel.com/en-us/node/544203:
        //     Consecutive calls to __itt_string_handle_create with the same name return the same value.
        ittHandle_name = __itt_string_handle_create(location.name);
        ittHandle_filename = __itt_string_handle_create(location.filename);
    }
    অন্যথা
    {
        ittHandle_name = 0;
        ittHandle_filename = 0;
    }
#endif
}

/*static*/ Region::Location_Extra_তথ্য* Region::Location_Extra_তথ্য::init(ধ্রুবক Region::LocationStaticStorage& location)
{
    Location_Extra_তথ্য** pLocationExtra = location.ppExtra;
    CV_DbgAssert(pLocationExtra);
    যদি (*pLocationExtra == ফাঁকা)
    {
        cv::AutoLock lock(cv::getInitializationMutex());
        যদি (*pLocationExtra == ফাঁকা)
        {
            *pLocationExtra = নতুন Region::Location_Extra_তথ্য(location);
            TraceStorage* s = getTraceManager().trace_storage.get();
            যদি (s)
            {
                TraceMessage বার্তা;
                বার্তা.formatlocation(location);
                s->put(বার্তা);
            }
        }
    }
    ফেরত *pLocationExtra;
}


Region::Impl::Impl(TraceManagerThreadLocal& ctx, Region* parentRegion_, Region& region_, ধ্রুবক LocationStaticStorage& location_, int64 beginTimestamp_) :
    location(location_),
    region(region_),
    parentRegion(parentRegion_),
    threadID(ctx.threadID),
    global_region_id(++ctx.region_গণক),
    beginTimestamp(beginTimestamp_),
    endTimestamp(0),
    direct_Children_গুনতি(0)
#ifdef OPENCV_WITH_ITT
    ,itt_id_registered(মিথ্যা)
    ,itt_id(__itt_null)
#endif
{
    CV_DbgAssert(ctx.currentActiveRegion == parentRegion);
    region.pImpl = এই;

    registerRegion(ctx);

    enterRegion(ctx);
}

Region::Impl::~Impl()
{
#ifdef OPENCV_WITH_ITT
    যদি (itt_id_registered)
    {
        CV_LOG_ITT(" Destroy ITT region: I=" << (শূন্য*)এই);
        __itt_id_destroy(domain, itt_id);
        itt_id_registered = মিথ্যা;
    }
#endif
    region.pImpl = ফাঁকা;
}

শূন্য Region::Impl::enterRegion(TraceManagerThreadLocal& ctx)
{
    ctx.currentActiveRegion = &region;

    যদি (location.flags & REGION_FLAG_FUNCTION)
    {
        যদি ((location.flags & REGION_FLAG_APP_CODE) == 0)
        {
            ctx.regionDepthOpenCV++;
        }
        ctx.regionDepth++;
    }

    TraceStorage* s = ctx.getStorage();
    যদি (s)
    {
        TraceMessage বার্তা;
        বার্তা.formatRegionEnter(region);
        s->put(বার্তা);
    }
#ifdef OPENCV_WITH_ITT
    যদি (isITTEnabled())
    {
        __itt_id parentID = __itt_null;
        যদি (param_ITT_registerParentScope && parentRegion && parentRegion->pImpl && parentRegion->pImpl->itt_id_registered && (location.flags & REGION_FLAG_REGION_FORCE) == 0)
            parentID = parentRegion->pImpl->itt_id;
        __itt_task_begin(domain, itt_id, parentID, (*location.ppExtra)->ittHandle_name);
    }
#endif
}

শূন্য Region::Impl::leaveRegion(TraceManagerThreadLocal& ctx)
{
    int64 duration = endTimestamp - beginTimestamp; CV_UNUSED(duration);
    RegionStatistics ফলাফল;
    ctx.stat.grab(ফলাফল);
    ctx.যোগ_Skipped_Events += ফলাফল.currentSkippedRegions;
    CV_LOG(_spaces(ctx.getCurrentDepth()*4) << "leaveRegion(): " << (শূন্য*)এই << " " << ফলাফল);
#ifdef OPENCV_WITH_ITT
    যদি (isITTEnabled())
    {
        যদি (ফলাফল.currentSkippedRegions)
        {
            __itt_metadata_add(domain, itt_id, __itt_string_handle_create("skipped trace entries"), __itt_metadata_u32, 1, &ফলাফল.currentSkippedRegions);
        }
#ifdef HAVE_IPP
        যদি (ফলাফল.durationImplIPP)
            __itt_metadata_add(domain, itt_id, __itt_string_handle_create("tIPP"), __itt_metadata_u64, 1, &ফলাফল.durationImplIPP);
#endif
#ifdef HAVE_OPENCL
        যদি (ফলাফল.durationImplOpenCL)
            __itt_metadata_add(domain, itt_id, __itt_string_handle_create("tOpenCL"), __itt_metadata_u64, 1, &ফলাফল.durationImplOpenCL);
#endif
        __itt_task_end(domain);
    }
#endif
    TraceStorage* s = ctx.getStorage();
    যদি (s)
    {
        TraceMessage বার্তা;
        বার্তা.formatRegionLeave(region, ফলাফল);
        s->put(বার্তা);
    }

    যদি (location.flags & REGION_FLAG_FUNCTION)
    {
        যদি ((location.flags & REGION_FLAG_APP_CODE) == 0)
        {
            ctx.regionDepthOpenCV--;
        }
        ctx.regionDepth--;
    }

    ctx.currentActiveRegion = parentRegion;
}

শূন্য Region::Impl::release()
{
    delete এই;
}

শূন্য Region::Impl::registerRegion(TraceManagerThreadLocal& ctx)
{
#ifdef OPENCV_WITH_ITT
    যদি (isITTEnabled())
    {
        যদি (!itt_id_registered)
        {
            CV_LOG_ITT(" Register ITT region: I=" << (শূন্য*)এই << " " << ctx.threadID << "-" << global_region_id);
#যদি 1 // workaround for some ITT backends
            itt_id = __itt_id_make((শূন্য*)(intptr_t)(((int64)(ctx.threadID + 1) << 32) | global_region_id), global_region_id);
#অন্যথা
            itt_id = __itt_id_make((শূন্য*)(intptr_t)(ctx.threadID + 1), global_region_id);
#endif
            __itt_id_create(domain, itt_id);
            itt_id_registered = সত্য;
        }
    }
#অন্যথা
    CV_UNUSED(ctx);
#endif
}

শূন্য RegionStatisticsStatus::enableSkipMode(পূর্ণসংখ্যা depth)
{
    CV_DbgAssert(_skipDepth < 0);
    CV_LOG_SKIP(ফাঁকা, "SKIP-ENABLE: depth=" << depth);
    _skipDepth = depth;
}
শূন্য RegionStatisticsStatus::checkResetSkipMode(পূর্ণসংখ্যা leaveDepth)
{
    যদি (leaveDepth <= _skipDepth)
    {
        CV_LOG_SKIP(ফাঁকা, "SKIP-RESET: leaveDepth=" << leaveDepth << " skipDepth=" << _skipDepth);
        _skipDepth = -1;
    }
}

Region::Region(ধ্রুবক LocationStaticStorage& location) :
    pImpl(ফাঁকা),
    implFlags(0)
{
    // Checks:
    // - global enable flag
    // - parent region is disabled
    // - children count threshold
    // - region location
    // - depth (opencv nested calls)
    যদি (!TraceManager::isActivated())
    {
        CV_LOG("Trace is disabled. Bailout");
        ফেরত;
    }

    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    CV_LOG(_spaces(ctx.getCurrentDepth()*4) << "Region(): " << (শূন্য*)এই << ": " << location.name);

    Region* parentRegion = ctx.স্তূপ_শীর্ষ_Region();
    ধ্রুবক Region::LocationStaticStorage* parentLocation = ctx.স্তূপ_শীর্ষ_Location();

    যদি (location.flags & REGION_FLAG_REGION_পরবর্তী)
    {
        যদি (parentRegion && parentRegion->pImpl)
        {
            CV_DbgAssert((parentRegion->pImpl->location.flags & REGION_FLAG_FUNCTION) == 0);
            parentRegion->destroy(); parentRegion->implFlags = 0;
            parentRegion = ctx.স্তূপ_শীর্ষ_Region();
            parentLocation = ctx.স্তূপ_শীর্ষ_Location();
        }
    }

    পূর্ণসংখ্যা parentChildren = 0;
    যদি (parentRegion && parentRegion->pImpl)
    {
        যদি (parentLocation == ফাঁকা)
        {
            // parallel_for_body code path
            parentChildren = CV_XADD(&parentRegion->pImpl->direct_Children_গুনতি, 1) + 1;
        }
        অন্যথা
        {
            parentChildren = ++parentRegion->pImpl->direct_Children_গুনতি;
        }
    }

    int64 beginTimestamp = getTimestampNS();

    পূর্ণসংখ্যা currentDepth = ctx.getCurrentDepth() + 1;
    নির্বাচন (location.flags & REGION_FLAG_IMPL_MASK)
    {
#ifdef HAVE_IPP
    ক্ষেত্র REGION_FLAG_IMPL_IPP:
        যদি (!ctx.stat_status.ignoreDepthImplIPP)
            ctx.stat_status.ignoreDepthImplIPP = currentDepth;
        বিরতি;
#endif
#ifdef HAVE_OPENCL
    ক্ষেত্র REGION_FLAG_IMPL_OPENCL:
        যদি (!ctx.stat_status.ignoreDepthImplOpenCL)
            ctx.stat_status.ignoreDepthImplOpenCL = currentDepth;
        বিরতি;
#endif
    পূর্বনির্ধারিত:
        বিরতি;
    }

    ctx.স্তূপ_ঠেলো(এই, &location, beginTimestamp);
    implFlags |= REGION_FLAG_NEED_স্তূপ_বের_করো;

    যদি ((location.flags & REGION_FLAG_REGION_FORCE) == 0)
    {
        যদি (ctx.stat_status._skipDepth >= 0 && currentDepth > ctx.stat_status._skipDepth)
        {
            CV_LOG(_spaces(ctx.getCurrentDepth()*4) << "Parent region is disabled. Bailout");
            ctx.stat.currentSkippedRegions++;
            ফেরত;
        }

        যদি (param_maxRegionChildrenOpenCV > 0 && (location.flags & REGION_FLAG_APP_CODE) == 0 && parentLocation && (parentLocation->flags & REGION_FLAG_APP_CODE) == 0)
        {
            যদি (parentChildren >= param_maxRegionChildrenOpenCV)
            {
                CV_LOG_TRACE_BAILOUT(ফাঁকা, _spaces(ctx.getCurrentDepth()*4) << "OpenCV parent region exceeds children count. Bailout");
                ctx.stat_status.enableSkipMode(currentDepth - 1);
                ctx.stat.currentSkippedRegions++;
                DEBUG_ONLY(ctx.dump_স্তূপ(std::cout, মিথ্যা));
                ফেরত;
            }
        }
        যদি (param_maxRegionChildren > 0 && parentChildren >= param_maxRegionChildren)
        {
            CV_LOG_TRACE_BAILOUT(ফাঁকা, _spaces(ctx.getCurrentDepth()*4) << "Parent region exceeds children count. Bailout");
            ctx.stat_status.enableSkipMode(currentDepth - 1);
            ctx.stat.currentSkippedRegions++;
            DEBUG_ONLY(ctx.dump_স্তূপ(std::cout, মিথ্যা));
            ফেরত;
        }
    }

    Location_Extra_তথ্য::init(location);

    যদি ((*location.ppExtra)->global_location_id == 0)
    {
        CV_LOG_TRACE_BAILOUT(ফাঁকা, _spaces(ctx.getCurrentDepth()*4) << "Region location is disabled. Bailout");
        ctx.stat_status.enableSkipMode(currentDepth);
        ctx.stat.currentSkippedRegions++;
        ফেরত;
    }

    যদি (parentLocation && (parentLocation->flags & REGION_FLAG_SKIP_NESTED))
    {
        CV_LOG(_spaces(ctx.getCurrentDepth()*4) << "Parent region disables inner regions. Bailout");
        ctx.stat_status.enableSkipMode(currentDepth);
        ctx.stat.currentSkippedRegions++;
        ফেরত;
    }

    যদি (param_maxRegionDepthOpenCV)
    {
        যদি ((location.flags & REGION_FLAG_APP_CODE) == 0)
        {
            যদি (ctx.regionDepthOpenCV >= param_maxRegionDepthOpenCV)
            {
                CV_LOG(_spaces(ctx.getCurrentDepth()*4) << "OpenCV region depth is exceed = " << ctx.regionDepthOpenCV << ". Bailout");
                যদি (ctx.stat.currentSkippedRegions == 0)
                {
                    DEBUG_ONLY(ctx.dump_স্তূপ(std::cout, মিথ্যা));
                }
                ctx.stat_status.enableSkipMode(currentDepth);
                ctx.stat.currentSkippedRegions++;
                ফেরত;
            }
        }
    }

    নতুন Impl(ctx, parentRegion, *এই, location, beginTimestamp);
    CV_DbgAssert(pImpl != ফাঁকা);
    implFlags |= REGION_FLAG__ACTIVE;

    // parallel_for path
    যদি (parentRegion && parentRegion->pImpl)
    {
        যদি (parentLocation == ফাঁকা)
        {
            pImpl->direct_Children_গুনতি = parentChildren;
        }
    }
}

শূন্য Region::destroy()
{
    CV_DbgAssert(implFlags != 0);

    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    CV_LOG(_spaces(ctx.getCurrentDepth()*4) << "Region::destruct(): " << (শূন্য*)এই << " pImpl=" << pImpl << " implFlags=" << implFlags << ' ' << (ctx.স্তূপ_শীর্ষ_Location() ? ctx.স্তূপ_শীর্ষ_Location()->name : "<unknown>"));

    CV_DbgAssert(implFlags & REGION_FLAG_NEED_স্তূপ_বের_করো);
    ধ্রুবক পূর্ণসংখ্যা currentDepth = ctx.getCurrentDepth(); CV_UNUSED(currentDepth);

    CV_LOG_CTX_STAT(ফাঁকা, _spaces(currentDepth*4) << ctx.stat << ' ' << ctx.stat_status);

    ধ্রুবক Region::LocationStaticStorage* location = ctx.স্তূপ_শীর্ষ_Location();
    Impl::OptimizationPath myCodePath = Impl::CODE_PATH_PLAIN;
    যদি (location)
    {
        নির্বাচন (location->flags & REGION_FLAG_IMPL_MASK)
        {
#ifdef HAVE_IPP
        ক্ষেত্র REGION_FLAG_IMPL_IPP:
            myCodePath = Impl::CODE_PATH_IPP;
            বিরতি;
#endif
#ifdef HAVE_OPENCL
        ক্ষেত্র REGION_FLAG_IMPL_OPENCL:
            যদি (param_synchronizeOpenCL && cv::ocl::isOpenCLActivated())
                cv::ocl::finish();
            myCodePath = Impl::CODE_PATH_OPENCL;
            বিরতি;
#endif
        পূর্বনির্ধারিত:
            বিরতি;
        }
    }

    int64 endTimestamp = getTimestampNS();
    int64 duration = endTimestamp - ctx.স্তূপ_শীর্ষ_Begin_Timestamp();

    যৌক্তিক active = isActive();

    যদি (active)
        ctx.stat.duration = duration;
    অন্যথা যদি (ctx.স্তূপ.size() == ctx.parallel_for_স্তূপ_আকার + 1)
        ctx.stat.duration += duration;

    নির্বাচন (myCodePath) {
        ক্ষেত্র Impl::CODE_PATH_PLAIN:
            // nothing
            বিরতি;
#ifdef HAVE_IPP
        ক্ষেত্র Impl::CODE_PATH_IPP:
            যদি (ctx.stat_status.ignoreDepthImplIPP == currentDepth)
            {
                ctx.stat.durationImplIPP += duration;
                ctx.stat_status.ignoreDepthImplIPP = 0;
            }
            অন্যথা যদি (active)
            {
                ctx.stat.durationImplIPP = duration;
            }
            বিরতি;
#endif
#ifdef HAVE_OPENCL
        ক্ষেত্র Impl::CODE_PATH_OPENCL:
            যদি (ctx.stat_status.ignoreDepthImplOpenCL == currentDepth)
            {
                ctx.stat.durationImplOpenCL += duration;
                ctx.stat_status.ignoreDepthImplOpenCL = 0;
            }
            অন্যথা যদি (active)
            {
                ctx.stat.durationImplOpenCL = duration;
            }
            বিরতি;
#endif
        পূর্বনির্ধারিত:
            বিরতি;
    }

    যদি (pImpl)
    {
        CV_DbgAssert((implFlags & (REGION_FLAG__ACTIVE | REGION_FLAG_NEED_স্তূপ_বের_করো)) == (REGION_FLAG__ACTIVE | REGION_FLAG_NEED_স্তূপ_বের_করো));
        CV_DbgAssert(ctx.স্তূপ_শীর্ষ_Region() == এই);
        pImpl->endTimestamp = endTimestamp;
        pImpl->leaveRegion(ctx);
        pImpl->release();
        pImpl = ফাঁকা;
        DEBUG_ONLY(implFlags &= ~REGION_FLAG__ACTIVE);
    }
    অন্যথা
    {
        CV_DbgAssert(ctx.stat_status._skipDepth <= currentDepth);
    }

    যদি (implFlags & REGION_FLAG_NEED_স্তূপ_বের_করো)
    {
        CV_DbgAssert(ctx.স্তূপ_শীর্ষ_Region() == এই);
        ctx.স্তূপ_বের_করো();
        ctx.stat_status.checkResetSkipMode(currentDepth);
        DEBUG_ONLY(implFlags &= ~REGION_FLAG_NEED_স্তূপ_বের_করো);
    }
    CV_LOG_CTX_STAT(ফাঁকা, _spaces(currentDepth*4) << "===> " << ctx.stat << ' ' << ctx.stat_status);
}


TraceManagerThreadLocal::~TraceManagerThreadLocal()
{
}

শূন্য TraceManagerThreadLocal::dump_স্তূপ(std::ostream& out, যৌক্তিক onlyFunctions) ধ্রুবক
{
    std::stringstream ss;
    std::deque<স্তূপ_Entry>::const_iterator it = স্তূপ.begin();
    std::deque<স্তূপ_Entry>::const_iterator end = স্তূপ.end();
    পূর্ণসংখ্যা depth = 0;
    জন্য (; it != end; ++it)
    {
        ধ্রুবক Region::LocationStaticStorage* location = it->location;
        যদি (location)
        {
            যদি (!onlyFunctions || (location->flags & REGION_FLAG_FUNCTION))
            {
                ss << _spaces(4*depth) << location->name << std::endl;
                depth++;
            }
        }
        অন্যথা
        {
            ss << _spaces(4*depth) << "<unknown>" << std::endl;
            depth++;
        }
    }
    out << ss.str();
}

শ্রেণী AsyncTraceStorage CV_FINAL : সর্বজনীন TraceStorage
{
    mutable std::ofstream out;
সর্বজনীন:
    ধ্রুবক std::string name;

    AsyncTraceStorage(ধ্রুবক std::string& filename) :
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

    যৌক্তিক put(ধ্রুবক TraceMessage& বার্তা) ধ্রুবক CV_OVERRIDE
    {
        যদি (বার্তা.hasError)
            ফেরত মিথ্যা;
        out << বার্তা.বাফার;
        //DEBUG_ONLY(std::flush(out)); // TODO configure flag
        ফেরত সত্য;
    }
};

শ্রেণী SyncTraceStorage CV_FINAL : সর্বজনীন TraceStorage
{
    mutable std::ofstream out;
    mutable cv::Mutex mutex;
সর্বজনীন:
    ধ্রুবক std::string name;

    SyncTraceStorage(ধ্রুবক std::string& filename) :
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

    যৌক্তিক put(ধ্রুবক TraceMessage& বার্তা) ধ্রুবক CV_OVERRIDE
    {
        যদি (বার্তা.hasError)
            ফেরত মিথ্যা;
        {
            cv::AutoLock l(mutex);
            out << বার্তা.বাফার;
            std::flush(out); // TODO configure flag
        }
        ফেরত সত্য;
    }
};


TraceStorage* TraceManagerThreadLocal::getStorage() ধ্রুবক
{
    // TODO configuration option for stdout/single trace file
    যদি (storage.empty())
    {
        TraceStorage* global = getTraceManager().trace_storage.get();
        যদি (global)
        {
            ধ্রুবক std::string filepath = cv::format("%s-%03d.txt", getParameterTraceLocation().c_str(), threadID).c_str();
            TraceMessage বার্তা;
            ধ্রুবক অক্ষর* pos = strrchr(filepath.c_str(), '/'); // extract filename
#ifdef _WIN32
            যদি (!pos)
                pos = strrchr(filepath.c_str(), '\\');
#endif
            যদি (!pos)
                pos = filepath.c_str();
            অন্যথা
                pos += 1; // fix to skip extra slash in filename beginning
            বার্তা.printf("#thread file: %s\n", pos);
            global->put(বার্তা);
            storage.reset(নতুন AsyncTraceStorage(filepath));
        }
    }
    ফেরত storage.get();
}



স্থির যৌক্তিক activated = মিথ্যা;
স্থির যৌক্তিক isInitialized = মিথ্যা;

TraceManager::TraceManager()
{
    (শূন্য)cv::getTimestampNS();

    isInitialized = সত্য;
    CV_LOG("TraceManager ctor: " << (শূন্য*)এই);

    CV_LOG("TraceManager configure()");
    activated = getParameterTraceEnable();

    যদি (activated)
        trace_storage.reset(নতুন SyncTraceStorage(std::string(getParameterTraceLocation()) + ".txt"));

#ifdef OPENCV_WITH_ITT
    যদি (isITTEnabled())
    {
        activated = সত্য; // force trace pipeline activation (without OpenCV storage)
        __itt_region_begin(domain, __itt_null, __itt_null, __itt_string_handle_create("OpenCVTrace"));
    }
#endif
}
TraceManager::~TraceManager()
{
    CV_LOG("TraceManager dtor: " << (শূন্য*)এই);

#ifdef OPENCV_WITH_ITT
    যদি (isITTEnabled())
    {
        __itt_region_end(domain, __itt_null);
    }
#endif

    std::vector<TraceManagerThreadLocal*> threads_ctx;
    tls.gather(threads_ctx);
    size_t যোগ_Events = 0, যোগ_Skipped_Events = 0;
    জন্য (size_t i = 0; i < threads_ctx.size(); i++)
    {
        TraceManagerThreadLocal* ctx = threads_ctx[i];
        যদি (ctx)
        {
            যোগ_Events += ctx->region_গণক;
            যোগ_Skipped_Events += ctx->যোগ_Skipped_Events;
        }
    }
    যদি (যোগ_Events || activated)
    {
        CV_LOG_INFO(ফাঁকা, "Trace: Total events: " << যোগ_Events);
    }
    যদি (যোগ_Skipped_Events)
    {
        CV_LOG_WARNING(ফাঁকা, "Trace: Total skipped events: " << যোগ_Skipped_Events);
    }

    // This is a global static object, so process starts shutdown here
    // Turn off trace
    cv::__termination = সত্য; // also set in DllMain() notifications handler for DLL_PROCESS_DETACH
    activated = মিথ্যা;
}

যৌক্তিক TraceManager::isActivated()
{
    // Check if process starts shutdown, and set earlyExit to true
    // to prevent further instrumentation processing earlier.
    যদি (cv::__termination)
    {
        activated = মিথ্যা;
        ফেরত মিথ্যা;
    }

    যদি (!isInitialized)
    {
        TraceManager& m = getTraceManager();
        CV_UNUSED(m); // TODO
    }

    ফেরত activated;
}


স্থির TraceManager* getTraceManagerCallOnce()
{
    স্থির TraceManager globalInstance;
    ফেরত &globalInstance;
}
TraceManager& getTraceManager()
{
    CV_SINGLETON_LAZY_INIT_REF(TraceManager, getTraceManagerCallOnce())
}

শূন্য parallel_For_Set_মূল_Region(ধ্রুবক Region& মূল_Region, ধ্রুবক TraceManagerThreadLocal& মূল_ctx)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();

    যদি (ctx.dummy_স্তূপ_শীর্ষ.region == &মূল_Region) // already attached
        ফেরত;

    CV_Assert(ctx.dummy_স্তূপ_শীর্ষ.region == ফাঁকা);
    ctx.dummy_স্তূপ_শীর্ষ = TraceManagerThreadLocal::স্তূপ_Entry(const_cast<Region*>(&মূল_Region), ফাঁকা, -1);

    যদি (&ctx == &মূল_ctx)
    {
        ctx.stat.grab(ctx.parallel_for_stat);
        ctx.parallel_for_stat_status = ctx.stat_status;
        ctx.parallel_for_স্তূপ_আকার = ctx.স্তূপ.size();
        ফেরত;
    }

    CV_Assert(ctx.স্তূপ.empty());

    ctx.currentActiveRegion = const_cast<Region*>(&মূল_Region);

    ctx.regionDepth = মূল_ctx.regionDepth;
    ctx.regionDepthOpenCV = মূল_ctx.regionDepthOpenCV;

    ctx.parallel_for_স্তূপ_আকার = 0;

    ctx.stat_status.propagateFrom(মূল_ctx.stat_status);
}

শূন্য parallelForAttachNestedRegion(ধ্রুবক Region& মূল_Region)
{
    CV_UNUSED(মূল_Region);
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();

    CV_DbgAssert(ctx.dummy_স্তূপ_শীর্ষ.region == &মূল_Region);

    Region* region = ctx.getCurrentActiveRegion();
    CV_LOG_PARALLEL(ফাঁকা, " PARALLEL_FOR: " << (শূন্য*)region << " ==> " << &মূল_Region);
    যদি (!region)
        ফেরত;

#ifdef OPENCV_WITH_ITT
    যদি (!মূল_Region.pImpl || !মূল_Region.pImpl->itt_id_registered)
        ফেরত;

    যদি (!region->pImpl)
        ফেরত;

    CV_LOG_PARALLEL(ফাঁকা, " PARALLEL_FOR ITT: " << (শূন্য*)মূল_Region.pImpl->itt_id.d1 << ":" << মূল_Region.pImpl->itt_id.d2 << ":" << (শূন্য*)মূল_Region.pImpl->itt_id.d3 << " => "
                                 << (শূন্য*)region->pImpl->itt_id.d1 << ":" << region->pImpl->itt_id.d2 << ":" << (শূন্য*)region->pImpl->itt_id.d3);
    __itt_relation_add(domain, region->pImpl->itt_id, __itt_relation_is_child_of, মূল_Region.pImpl->itt_id);
#endif
}

শূন্য parallelForFinalize(ধ্রুবক Region& মূল_Region)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();

    int64 endTimestamp = getTimestampNS();
    int64 duration = endTimestamp - ctx.স্তূপ_শীর্ষ_Begin_Timestamp();
    CV_LOG_PARALLEL(ফাঁকা, "parallel_for duration: " << duration << " " << &মূল_Region);

    std::vector<TraceManagerThreadLocal*> threads_ctx;
    getTraceManager().tls.gather(threads_ctx);
    RegionStatistics parallel_for_stat;
    জন্য (size_t i = 0; i < threads_ctx.size(); i++)
    {
        TraceManagerThreadLocal* child_ctx = threads_ctx[i];

        যদি (child_ctx && child_ctx->স্তূপ_শীর্ষ_Region() == &মূল_Region)
        {
            CV_LOG_PARALLEL(ফাঁকা, "Thread=" << child_ctx->threadID << " " << child_ctx->stat);
            RegionStatistics child_stat;
            child_ctx->stat.grab(child_stat);
            parallel_for_stat.append(child_stat);
            যদি (child_ctx != &ctx)
            {
                child_ctx->dummy_স্তূপ_শীর্ষ = TraceManagerThreadLocal::স্তূপ_Entry();
            }
            অন্যথা
            {
                ctx.parallel_for_stat.grab(ctx.stat);
                ctx.stat_status = ctx.parallel_for_stat_status;
                child_ctx->dummy_স্তূপ_শীর্ষ = TraceManagerThreadLocal::স্তূপ_Entry();
            }
        }
    }

    দশমিক parallel_coeff = std::min(1.0f, duration / (দশমিক)(parallel_for_stat.duration));
    CV_LOG_PARALLEL(ফাঁকা, "parallel_coeff=" << 1.0f / parallel_coeff);
    CV_LOG_PARALLEL(ফাঁকা, parallel_for_stat);
    যদি (parallel_coeff != 1.0f)
    {
        parallel_for_stat.multiply(parallel_coeff);
        CV_LOG_PARALLEL(ফাঁকা, parallel_for_stat);
    }
    parallel_for_stat.duration = 0;
    ctx.stat.append(parallel_for_stat);
    CV_LOG_PARALLEL(ফাঁকা, ctx.stat);
}

কাঠামো TraceArg::Extra_তথ্য
{
#ifdef OPENCV_WITH_ITT
    // Special fields for ITT
    __itt_string_handle* অস্থির ittHandle_name;
#endif
    Extra_তথ্য(TraceManagerThreadLocal& ctx, ধ্রুবক TraceArg& arg)
    {
        CV_UNUSED(ctx); CV_UNUSED(arg);
#ifdef OPENCV_WITH_ITT
        যদি (isITTEnabled())
        {
            // Caching is not required here, because there is builtin cache.
            // https://software.intel.com/en-us/node/544203:
            //     Consecutive calls to __itt_string_handle_create with the same name return the same value.
            ittHandle_name = __itt_string_handle_create(arg.name);
        }
        অন্যথা
        {
            ittHandle_name = 0;
        }
#endif
    }
};

স্থির শূন্য initTraceArg(TraceManagerThreadLocal& ctx, ধ্রুবক TraceArg& arg)
{
    TraceArg::Extra_তথ্য** pExtra = arg.ppExtra;
    যদি (*pExtra == ফাঁকা)
    {
        cv::AutoLock lock(cv::getInitializationMutex());
        যদি (*pExtra == ফাঁকা)
        {
            *pExtra = নতুন TraceArg::Extra_তথ্য(ctx, arg);
        }
    }
}
শূন্য traceArg(ধ্রুবক TraceArg& arg, ধ্রুবক অক্ষর* value)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    Region* region = ctx.getCurrentActiveRegion();
    যদি (!region)
        ফেরত;
    CV_Assert(region->pImpl);
    initTraceArg(ctx, arg);
    যদি (!value)
        value = "<null>";
#ifdef OPENCV_WITH_ITT
    যদি (isITTEnabled())
    {
        __itt_metadata_str_add(domain, region->pImpl->itt_id, (*arg.ppExtra)->ittHandle_name, value, strlen(value));
    }
#endif
}
শূন্য traceArg(ধ্রুবক TraceArg& arg, পূর্ণসংখ্যা value)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    Region* region = ctx.getCurrentActiveRegion();
    যদি (!region)
        ফেরত;
    CV_Assert(region->pImpl);
    initTraceArg(ctx, arg);
#ifdef OPENCV_WITH_ITT
    যদি (isITTEnabled())
    {
        __itt_metadata_add(domain, region->pImpl->itt_id, (*arg.ppExtra)->ittHandle_name, আকার(পূর্ণসংখ্যা) == 4 ? __itt_metadata_s32 : __itt_metadata_s64, 1, &value);
    }
#অন্যথা
    CV_UNUSED(value);
#endif
}
শূন্য traceArg(ধ্রুবক TraceArg& arg, int64 value)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    Region* region = ctx.getCurrentActiveRegion();
    যদি (!region)
        ফেরত;
    CV_Assert(region->pImpl);
    initTraceArg(ctx, arg);
#ifdef OPENCV_WITH_ITT
    যদি (isITTEnabled())
    {
        __itt_metadata_add(domain, region->pImpl->itt_id, (*arg.ppExtra)->ittHandle_name, __itt_metadata_s64, 1, &value);
    }
#অন্যথা
    CV_UNUSED(value);
#endif
}
শূন্য traceArg(ধ্রুবক TraceArg& arg, দ্বিগুণ value)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    Region* region = ctx.getCurrentActiveRegion();
    যদি (!region)
        ফেরত;
    CV_Assert(region->pImpl);
    initTraceArg(ctx, arg);
#ifdef OPENCV_WITH_ITT
    যদি (isITTEnabled())
    {
        __itt_metadata_add(domain, region->pImpl->itt_id, (*arg.ppExtra)->ittHandle_name, __itt_metadata_double, 1, &value);
    }
#অন্যথা
    CV_UNUSED(value);
#endif
}

#অন্যথা

Region::Region(ধ্রুবক LocationStaticStorage&) : pImpl(ফাঁকা), implFlags(0) {}
শূন্য Region::destroy() {}

শূন্য traceArg(ধ্রুবক TraceArg&, ধ্রুবক অক্ষর*) {}
শূন্য traceArg(ধ্রুবক TraceArg&, পূর্ণসংখ্যা) {};
শূন্য traceArg(ধ্রুবক TraceArg&, int64) {};
শূন্য traceArg(ধ্রুবক TraceArg&, দ্বিগুণ) {};

#endif

}}}} // namespace
