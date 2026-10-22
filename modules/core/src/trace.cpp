// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#অন্তর্ভাৱ "precomp.hpp"

#অন্তর্ভাৱ <opencv2/core/utils/trace.hpp>
#অন্তর্ভাৱ <opencv2/core/utils/trace.private.hpp>
#অন্তর্ভাৱ <opencv2/core/utils/configuration.private.hpp>

#অন্তর্ভাৱ <opencv2/core/opencl/ocl_defs.hpp>

#অন্তর্ভাৱ <cstdarg> // va_start

#অন্তর্ভাৱ <sstream>
#অন্তর্ভাৱ <ostream>
#অন্তর্ভাৱ <fstream>

#যদি 0
#define CV_LOG(...) CV_LOG_INFO(রিক্ত, __VA_ARGS__)
#অন্যথা
#define CV_LOG(...) {}
#endif

#যদি 0
#define CV_LOG_ITT(...) CV_LOG_INFO(রিক্ত, __VA_ARGS__)
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

স্থির তার্কিক getParameterTraceEnable()
{
    স্থির তার্কিক param_traceEnable = utils::getConfigurationParameterBool("OPENCV_TRACE", মিছা);
    ঘূৰাই param_traceEnable;
}

// TODO lazy configuration flags
স্থির পূর্ণাঙ্ক param_maxRegionDepthOpenCV = (পূর্ণাঙ্ক)utils::getConfigurationParameterSizeT("OPENCV_TRACE_DEPTH_OPENCV", 1);
স্থির পূর্ণাঙ্ক param_maxRegionChildrenOpenCV = (পূর্ণাঙ্ক)utils::getConfigurationParameterSizeT("OPENCV_TRACE_MAX_CHILDREN_OPENCV", 1000);
স্থির পূর্ণাঙ্ক param_maxRegionChildren = (পূর্ণাঙ্ক)utils::getConfigurationParameterSizeT("OPENCV_TRACE_MAX_CHILDREN", 10000);

স্থির ধ্রুৱ cv::String& getParameterTraceLocation()
{
    স্থির cv::String param_traceLocation = utils::getConfigurationParameterString("OPENCV_TRACE_LOCATION", "OpenCVTrace");
    ঘূৰাই param_traceLocation;
}

#ifdef HAVE_OPENCL
স্থির তার্কিক param_synchronizeOpenCL = utils::getConfigurationParameterBool("OPENCV_TRACE_SYNC_OPENCL", মিছা);
#endif

#ifdef OPENCV_WITH_ITT
স্থির তার্কিক param_ITT_registerParentScope = utils::getConfigurationParameterBool("OPENCV_TRACE_ITT_PARENT", মিছা);
#endif

স্থির ধ্রুৱ ৱর্ণ* _spaces(পূর্ণাঙ্ক গুনতি_2)
{
    স্থির ধ্রুৱ ৱর্ণ buf[64] =
"                                                               ";
    ঘূৰাই &buf[63 - (গুনতি_2 & 63)];
}

/**
 * Text-based trace messages
 */
শ্ৰেণী TraceMessage
{
সার্ৱজনিক:
    ৱর্ণ buffer[1024];
    size_t len;
    তার্কিক hasError;

    TraceMessage() :
        len(0),
        hasError(মিছা)
    {}

    তার্কিক printf(ধ্রুৱ ৱর্ণ* format, ...)
    {
        ৱর্ণ* buf = &buffer[len];
        size_t sz = পরিমাণ(buffer) - len;
        va_list ap;
        va_start(ap, format);
        পূর্ণাঙ্ক n = cv_vsnprintf(buf, (পূর্ণাঙ্ক)sz, format, ap);
        va_end(ap);
        যদি (n < 0 || (size_t)n > sz)
        {
            hasError = সঁচা;
            ঘূৰাই মিছা;
        }
        len += n;
        ঘূৰাই সঁচা;
    }

    তার্কিক formatlocation(ধ্রুৱ Region::LocationStaticStorage& location)
    {
        ঘূৰাই এতত্->printf("l,%lld,\"%s\",%d,\"%s\",0x%llX\n",
                (long long পূর্ণাঙ্ক)(*location.ppExtra)->global_location_id,
                location.filename,
                location.সাৰি,
                location.name,
                (long long পূর্ণাঙ্ক)(location.flags & ~0xF0000000));
    }
    তার্কিক formatRegionEnter(ধ্রুৱ Region& region)
    {
        তার্কিক ok = এতত্->printf("b,%d,%lld,%lld,%lld",
                (পূর্ণাঙ্ক)region.pImpl->threadID,
                (long long পূর্ণাঙ্ক)region.pImpl->beginTimestamp,
                (long long পূর্ণাঙ্ক)((*region.pImpl->location.ppExtra)->global_location_id),
                (long long পূর্ণাঙ্ক)region.pImpl->global_region_id);
        যদি (region.pImpl->parentRegion && region.pImpl->parentRegion->pImpl)
        {
            যদি (region.pImpl->parentRegion->pImpl->threadID != region.pImpl->threadID)
                ok &= এতত্->printf(",parentThread=%d,parent=%lld",
                        (পূর্ণাঙ্ক)region.pImpl->parentRegion->pImpl->threadID,
                        (long long পূর্ণাঙ্ক)region.pImpl->parentRegion->pImpl->global_region_id);
        }
        ok &= এতত্->printf("\n");
        ঘূৰাই ok;
    }
    তার্কিক formatRegionLeave(ধ্রুৱ Region& region, ধ্রুৱ RegionStatistics& result)
    {
        CV_DbgAssert(region.pImpl->endTimestamp - region.pImpl->beginTimestamp == result.duration);
        তার্কিক ok = এতত্->printf("e,%d,%lld,%lld,%lld,%lld",
                (পূর্ণাঙ্ক)region.pImpl->threadID,
                (long long পূর্ণাঙ্ক)region.pImpl->endTimestamp,
                (long long পূর্ণাঙ্ক)(*region.pImpl->location.ppExtra)->global_location_id,
                (long long পূর্ণাঙ্ক)region.pImpl->global_region_id,
                (long long পূর্ণাঙ্ক)result.duration);
        যদি (result.currentSkippedRegions)
            ok &= এতত্->printf(",skip=%d", (পূর্ণাঙ্ক)result.currentSkippedRegions);
#ifdef HAVE_IPP
        যদি (result.durationImplIPP)
            ok &= এতত্->printf(",tIPP=%lld", (long long পূর্ণাঙ্ক)result.durationImplIPP);
#endif
#ifdef HAVE_OPENCL
        যদি (result.durationImplOpenCL)
            ok &= এতত্->printf(",tOCL=%lld", (long long পূর্ণাঙ্ক)result.durationImplOpenCL);
#endif
        ok &= এতত্->printf("\n");
        ঘূৰাই ok;
    }
    তার্কিক recordRegionArg(ধ্রুৱ Region& region, ধ্রুৱ TraceArg& arg, ধ্রুৱ ৱর্ণ* value)
    {
        ঘূৰাই এতত্->printf("a,%d,%lld,%lld,\"%s\",\"%s\"\n",
                region.pImpl->threadID,
                (long long পূর্ণাঙ্ক)region.pImpl->beginTimestamp,
                (long long পূর্ণাঙ্ক)region.pImpl->global_region_id,
                arg.name,
                value);
    }
};


#ifdef OPENCV_WITH_ITT
স্থির __itt_domain* domain = রিক্ত;

স্থির তার্কিক isITTEnabled()
{
    স্থির চঞ্চল তার্কিক isInitialized = মিছা;
    স্থির তার্কিক isEnabled = মিছা;
    যদি (!isInitialized)
    {
        cv::AutoLock lock(cv::getInitializationMutex());
        যদি (!isInitialized)
        {
            তার্কিক param_traceITTEnable = utils::getConfigurationParameterBool("OPENCV_TRACE_ITT_ENABLE", সঁচা);
            যদি (param_traceITTEnable)
            {
                isEnabled = !!(__itt_api_version());
                CV_LOG_ITT("ITT is " << (isEnabled ? "enabled" : "disabled"));
                domain = __itt_domain_create("OpenCVTrace");
            }
            অন্যথা
            {
                CV_LOG_ITT("ITT is disabled through OpenCV parameter");
                isEnabled = মিছা;
            }
            isInitialized = সঁচা;
        }
    }
    ঘূৰাই isEnabled;
}
#endif


Region::LocationExtraData::LocationExtraData(ধ্রুৱ LocationStaticStorage& location)
{
    CV_UNUSED(location);
    স্থির পূর্ণাঙ্ক g_location_id_counter = 0;
    global_location_id = CV_XADD(&g_location_id_counter, 1) + 1;
    CV_LOG("Register location: " << global_location_id << " (" << (শূন্য*)&location << ")"
            << std::endl << "    file: " << location.filename
            << std::endl << "    line: " << location.সাৰি
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

/*static*/ Region::LocationExtraData* Region::LocationExtraData::init(ধ্রুৱ Region::LocationStaticStorage& location)
{
    LocationExtraData** pLocationExtra = location.ppExtra;
    CV_DbgAssert(pLocationExtra);
    যদি (*pLocationExtra == রিক্ত)
    {
        cv::AutoLock lock(cv::getInitializationMutex());
        যদি (*pLocationExtra == রিক্ত)
        {
            *pLocationExtra = নূতন Region::LocationExtraData(location);
            TraceStorage* s = getTraceManager().trace_storage.get();
            যদি (s)
            {
                TraceMessage বাৰ্তা;
                বাৰ্তা.formatlocation(location);
                s->put(বাৰ্তা);
            }
        }
    }
    ঘূৰাই *pLocationExtra;
}


Region::Impl::Impl(TraceManagerThreadLocal& ctx, Region* parentRegion_, Region& region_, ধ্রুৱ LocationStaticStorage& location_, int64 beginTimestamp_) :
    location(location_),
    region(region_),
    parentRegion(parentRegion_),
    threadID(ctx.threadID),
    global_region_id(++ctx.region_counter),
    beginTimestamp(beginTimestamp_),
    endTimestamp(0),
    direct_Children_গুনতি(0)
#ifdef OPENCV_WITH_ITT
    ,itt_id_registered(মিছা)
    ,itt_id(__itt_null)
#endif
{
    CV_DbgAssert(ctx.currentActiveRegion == parentRegion);
    region.pImpl = এতত্;

    registerRegion(ctx);

    enterRegion(ctx);
}

Region::Impl::~Impl()
{
#ifdef OPENCV_WITH_ITT
    যদি (itt_id_registered)
    {
        CV_LOG_ITT(" Destroy ITT region: I=" << (শূন্য*)এতত্);
        __itt_id_destroy(domain, itt_id);
        itt_id_registered = মিছা;
    }
#endif
    region.pImpl = রিক্ত;
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
        TraceMessage বাৰ্তা;
        বাৰ্তা.formatRegionEnter(region);
        s->put(বাৰ্তা);
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
    RegionStatistics result;
    ctx.stat.grab(result);
    ctx.totalSkippedEvents += result.currentSkippedRegions;
    CV_LOG(_spaces(ctx.getCurrentDepth()*4) << "leaveRegion(): " << (শূন্য*)এতত্ << " " << result);
#ifdef OPENCV_WITH_ITT
    যদি (isITTEnabled())
    {
        যদি (result.currentSkippedRegions)
        {
            __itt_metadata_add(domain, itt_id, __itt_string_handle_create("skipped trace entries"), __itt_metadata_u32, 1, &result.currentSkippedRegions);
        }
#ifdef HAVE_IPP
        যদি (result.durationImplIPP)
            __itt_metadata_add(domain, itt_id, __itt_string_handle_create("tIPP"), __itt_metadata_u64, 1, &result.durationImplIPP);
#endif
#ifdef HAVE_OPENCL
        যদি (result.durationImplOpenCL)
            __itt_metadata_add(domain, itt_id, __itt_string_handle_create("tOpenCL"), __itt_metadata_u64, 1, &result.durationImplOpenCL);
#endif
        __itt_task_end(domain);
    }
#endif
    TraceStorage* s = ctx.getStorage();
    যদি (s)
    {
        TraceMessage বাৰ্তা;
        বাৰ্তা.formatRegionLeave(region, result);
        s->put(বাৰ্তা);
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
    delete এতত্;
}

শূন্য Region::Impl::registerRegion(TraceManagerThreadLocal& ctx)
{
#ifdef OPENCV_WITH_ITT
    যদি (isITTEnabled())
    {
        যদি (!itt_id_registered)
        {
            CV_LOG_ITT(" Register ITT region: I=" << (শূন্য*)এতত্ << " " << ctx.threadID << "-" << global_region_id);
#যদি 1 // workaround for some ITT backends
            itt_id = __itt_id_make((শূন্য*)(intptr_t)(((int64)(ctx.threadID + 1) << 32) | global_region_id), global_region_id);
#অন্যথা
            itt_id = __itt_id_make((শূন্য*)(intptr_t)(ctx.threadID + 1), global_region_id);
#endif
            __itt_id_create(domain, itt_id);
            itt_id_registered = সঁচা;
        }
    }
#অন্যথা
    CV_UNUSED(ctx);
#endif
}

শূন্য RegionStatisticsStatus::enableSkipMode(পূর্ণাঙ্ক depth)
{
    CV_DbgAssert(_skipDepth < 0);
    CV_LOG_SKIP(রিক্ত, "SKIP-ENABLE: depth=" << depth);
    _skipDepth = depth;
}
শূন্য RegionStatisticsStatus::checkResetSkipMode(পূর্ণাঙ্ক leaveDepth)
{
    যদি (leaveDepth <= _skipDepth)
    {
        CV_LOG_SKIP(রিক্ত, "SKIP-RESET: leaveDepth=" << leaveDepth << " skipDepth=" << _skipDepth);
        _skipDepth = -1;
    }
}

Region::Region(ধ্রুৱ LocationStaticStorage& location) :
    pImpl(রিক্ত),
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
        ঘূৰাই;
    }

    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    CV_LOG(_spaces(ctx.getCurrentDepth()*4) << "Region(): " << (শূন্য*)এতত্ << ": " << location.name);

    Region* parentRegion = ctx.স্তূপ_শীৰ্ষ_Region();
    ধ্রুৱ Region::LocationStaticStorage* parentLocation = ctx.স্তূপ_শীৰ্ষ_Location();

    যদি (location.flags & REGION_FLAG_REGION_NEXT)
    {
        যদি (parentRegion && parentRegion->pImpl)
        {
            CV_DbgAssert((parentRegion->pImpl->location.flags & REGION_FLAG_FUNCTION) == 0);
            parentRegion->destroy(); parentRegion->implFlags = 0;
            parentRegion = ctx.স্তূপ_শীৰ্ষ_Region();
            parentLocation = ctx.স্তূপ_শীৰ্ষ_Location();
        }
    }

    পূর্ণাঙ্ক parentChildren = 0;
    যদি (parentRegion && parentRegion->pImpl)
    {
        যদি (parentLocation == রিক্ত)
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

    পূর্ণাঙ্ক currentDepth = ctx.getCurrentDepth() + 1;
    ৱিকল্প (location.flags & REGION_FLAG_IMPL_MASK)
    {
#ifdef HAVE_IPP
    পক্ষ REGION_FLAG_IMPL_IPP:
        যদি (!ctx.stat_status.ignoreDepthImplIPP)
            ctx.stat_status.ignoreDepthImplIPP = currentDepth;
        ভঙ্গ;
#endif
#ifdef HAVE_OPENCL
    পক্ষ REGION_FLAG_IMPL_OPENCL:
        যদি (!ctx.stat_status.ignoreDepthImplOpenCL)
            ctx.stat_status.ignoreDepthImplOpenCL = currentDepth;
        ভঙ্গ;
#endif
    স্ৱভাৱ:
        ভঙ্গ;
    }

    ctx.স্তূপ_ঠেলো(এতত্, &location, beginTimestamp);
    implFlags |= REGION_FLAG_NEED_স্তূপ_বেৰ_কৰো;

    যদি ((location.flags & REGION_FLAG_REGION_FORCE) == 0)
    {
        যদি (ctx.stat_status._skipDepth >= 0 && currentDepth > ctx.stat_status._skipDepth)
        {
            CV_LOG(_spaces(ctx.getCurrentDepth()*4) << "Parent region is disabled. Bailout");
            ctx.stat.currentSkippedRegions++;
            ঘূৰাই;
        }

        যদি (param_maxRegionChildrenOpenCV > 0 && (location.flags & REGION_FLAG_APP_CODE) == 0 && parentLocation && (parentLocation->flags & REGION_FLAG_APP_CODE) == 0)
        {
            যদি (parentChildren >= param_maxRegionChildrenOpenCV)
            {
                CV_LOG_TRACE_BAILOUT(রিক্ত, _spaces(ctx.getCurrentDepth()*4) << "OpenCV parent region exceeds children count. Bailout");
                ctx.stat_status.enableSkipMode(currentDepth - 1);
                ctx.stat.currentSkippedRegions++;
                DEBUG_ONLY(ctx.dump_স্তূপ(std::cout, মিছা));
                ঘূৰাই;
            }
        }
        যদি (param_maxRegionChildren > 0 && parentChildren >= param_maxRegionChildren)
        {
            CV_LOG_TRACE_BAILOUT(রিক্ত, _spaces(ctx.getCurrentDepth()*4) << "Parent region exceeds children count. Bailout");
            ctx.stat_status.enableSkipMode(currentDepth - 1);
            ctx.stat.currentSkippedRegions++;
            DEBUG_ONLY(ctx.dump_স্তূপ(std::cout, মিছা));
            ঘূৰাই;
        }
    }

    LocationExtraData::init(location);

    যদি ((*location.ppExtra)->global_location_id == 0)
    {
        CV_LOG_TRACE_BAILOUT(রিক্ত, _spaces(ctx.getCurrentDepth()*4) << "Region location is disabled. Bailout");
        ctx.stat_status.enableSkipMode(currentDepth);
        ctx.stat.currentSkippedRegions++;
        ঘূৰাই;
    }

    যদি (parentLocation && (parentLocation->flags & REGION_FLAG_SKIP_NESTED))
    {
        CV_LOG(_spaces(ctx.getCurrentDepth()*4) << "Parent region disables inner regions. Bailout");
        ctx.stat_status.enableSkipMode(currentDepth);
        ctx.stat.currentSkippedRegions++;
        ঘূৰাই;
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
                    DEBUG_ONLY(ctx.dump_স্তূপ(std::cout, মিছা));
                }
                ctx.stat_status.enableSkipMode(currentDepth);
                ctx.stat.currentSkippedRegions++;
                ঘূৰাই;
            }
        }
    }

    নূতন Impl(ctx, parentRegion, *এতত্, location, beginTimestamp);
    CV_DbgAssert(pImpl != রিক্ত);
    implFlags |= REGION_FLAG__ACTIVE;

    // parallel_for path
    যদি (parentRegion && parentRegion->pImpl)
    {
        যদি (parentLocation == রিক্ত)
        {
            pImpl->direct_Children_গুনতি = parentChildren;
        }
    }
}

শূন্য Region::destroy()
{
    CV_DbgAssert(implFlags != 0);

    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    CV_LOG(_spaces(ctx.getCurrentDepth()*4) << "Region::destruct(): " << (শূন্য*)এতত্ << " pImpl=" << pImpl << " implFlags=" << implFlags << ' ' << (ctx.স্তূপ_শীৰ্ষ_Location() ? ctx.স্তূপ_শীৰ্ষ_Location()->name : "<unknown>"));

    CV_DbgAssert(implFlags & REGION_FLAG_NEED_স্তূপ_বেৰ_কৰো);
    ধ্রুৱ পূর্ণাঙ্ক currentDepth = ctx.getCurrentDepth(); CV_UNUSED(currentDepth);

    CV_LOG_CTX_STAT(রিক্ত, _spaces(currentDepth*4) << ctx.stat << ' ' << ctx.stat_status);

    ধ্রুৱ Region::LocationStaticStorage* location = ctx.স্তূপ_শীৰ্ষ_Location();
    Impl::OptimizationPath myCodePath = Impl::CODE_PATH_PLAIN;
    যদি (location)
    {
        ৱিকল্প (location->flags & REGION_FLAG_IMPL_MASK)
        {
#ifdef HAVE_IPP
        পক্ষ REGION_FLAG_IMPL_IPP:
            myCodePath = Impl::CODE_PATH_IPP;
            ভঙ্গ;
#endif
#ifdef HAVE_OPENCL
        পক্ষ REGION_FLAG_IMPL_OPENCL:
            যদি (param_synchronizeOpenCL && cv::ocl::isOpenCLActivated())
                cv::ocl::finish();
            myCodePath = Impl::CODE_PATH_OPENCL;
            ভঙ্গ;
#endif
        স্ৱভাৱ:
            ভঙ্গ;
        }
    }

    int64 endTimestamp = getTimestampNS();
    int64 duration = endTimestamp - ctx.স্তূপ_শীৰ্ষ_Begin_Timestamp();

    তার্কিক active = isActive();

    যদি (active)
        ctx.stat.duration = duration;
    অন্যথা যদি (ctx.স্তূপ.size() == ctx.parallel_for_স্তূপ_size + 1)
        ctx.stat.duration += duration;

    ৱিকল্প (myCodePath) {
        পক্ষ Impl::CODE_PATH_PLAIN:
            // nothing
            ভঙ্গ;
#ifdef HAVE_IPP
        পক্ষ Impl::CODE_PATH_IPP:
            যদি (ctx.stat_status.ignoreDepthImplIPP == currentDepth)
            {
                ctx.stat.durationImplIPP += duration;
                ctx.stat_status.ignoreDepthImplIPP = 0;
            }
            অন্যথা যদি (active)
            {
                ctx.stat.durationImplIPP = duration;
            }
            ভঙ্গ;
#endif
#ifdef HAVE_OPENCL
        পক্ষ Impl::CODE_PATH_OPENCL:
            যদি (ctx.stat_status.ignoreDepthImplOpenCL == currentDepth)
            {
                ctx.stat.durationImplOpenCL += duration;
                ctx.stat_status.ignoreDepthImplOpenCL = 0;
            }
            অন্যথা যদি (active)
            {
                ctx.stat.durationImplOpenCL = duration;
            }
            ভঙ্গ;
#endif
        স্ৱভাৱ:
            ভঙ্গ;
    }

    যদি (pImpl)
    {
        CV_DbgAssert((implFlags & (REGION_FLAG__ACTIVE | REGION_FLAG_NEED_স্তূপ_বেৰ_কৰো)) == (REGION_FLAG__ACTIVE | REGION_FLAG_NEED_স্তূপ_বেৰ_কৰো));
        CV_DbgAssert(ctx.স্তূপ_শীৰ্ষ_Region() == এতত্);
        pImpl->endTimestamp = endTimestamp;
        pImpl->leaveRegion(ctx);
        pImpl->release();
        pImpl = রিক্ত;
        DEBUG_ONLY(implFlags &= ~REGION_FLAG__ACTIVE);
    }
    অন্যথা
    {
        CV_DbgAssert(ctx.stat_status._skipDepth <= currentDepth);
    }

    যদি (implFlags & REGION_FLAG_NEED_স্তূপ_বেৰ_কৰো)
    {
        CV_DbgAssert(ctx.স্তূপ_শীৰ্ষ_Region() == এতত্);
        ctx.স্তূপ_বেৰ_কৰো();
        ctx.stat_status.checkResetSkipMode(currentDepth);
        DEBUG_ONLY(implFlags &= ~REGION_FLAG_NEED_স্তূপ_বেৰ_কৰো);
    }
    CV_LOG_CTX_STAT(রিক্ত, _spaces(currentDepth*4) << "===> " << ctx.stat << ' ' << ctx.stat_status);
}


TraceManagerThreadLocal::~TraceManagerThreadLocal()
{
}

শূন্য TraceManagerThreadLocal::dump_স্তূপ(std::ostream& out, তার্কিক onlyFunctions) ধ্রুৱ
{
    std::stringstream ss;
    std::deque<স্তূপ_Entry>::const_iterator it = স্তূপ.begin();
    std::deque<স্তূপ_Entry>::const_iterator end = স্তূপ.end();
    পূর্ণাঙ্ক depth = 0;
    বাবে (; it != end; ++it)
    {
        ধ্রুৱ Region::LocationStaticStorage* location = it->location;
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

শ্ৰেণী AsyncTraceStorage CV_FINAL : সার্ৱজনিক TraceStorage
{
    mutable std::ofstream out;
সার্ৱজনিক:
    ধ্রুৱ std::string name;

    AsyncTraceStorage(ধ্রুৱ std::string& filename) :
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

    তার্কিক put(ধ্রুৱ TraceMessage& বাৰ্তা) ধ্রুৱ CV_OVERRIDE
    {
        যদি (বাৰ্তা.hasError)
            ঘূৰাই মিছা;
        out << বাৰ্তা.buffer;
        //DEBUG_ONLY(std::flush(out)); // TODO configure flag
        ঘূৰাই সঁচা;
    }
};

শ্ৰেণী SyncTraceStorage CV_FINAL : সার্ৱজনিক TraceStorage
{
    mutable std::ofstream out;
    mutable cv::Mutex mutex;
সার্ৱজনিক:
    ধ্রুৱ std::string name;

    SyncTraceStorage(ধ্রুৱ std::string& filename) :
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

    তার্কিক put(ধ্রুৱ TraceMessage& বাৰ্তা) ধ্রুৱ CV_OVERRIDE
    {
        যদি (বাৰ্তা.hasError)
            ঘূৰাই মিছা;
        {
            cv::AutoLock l(mutex);
            out << বাৰ্তা.buffer;
            std::flush(out); // TODO configure flag
        }
        ঘূৰাই সঁচা;
    }
};


TraceStorage* TraceManagerThreadLocal::getStorage() ধ্রুৱ
{
    // TODO configuration option for stdout/single trace file
    যদি (storage.empty())
    {
        TraceStorage* global = getTraceManager().trace_storage.get();
        যদি (global)
        {
            ধ্রুৱ std::string filepath = cv::format("%s-%03d.txt", getParameterTraceLocation().c_str(), threadID).c_str();
            TraceMessage বাৰ্তা;
            ধ্রুৱ ৱর্ণ* pos = strrchr(filepath.c_str(), '/'); // extract filename
#ifdef _WIN32
            যদি (!pos)
                pos = strrchr(filepath.c_str(), '\\');
#endif
            যদি (!pos)
                pos = filepath.c_str();
            অন্যথা
                pos += 1; // fix to skip extra slash in filename beginning
            বাৰ্তা.printf("#thread file: %s\n", pos);
            global->put(বাৰ্তা);
            storage.reset(নূতন AsyncTraceStorage(filepath));
        }
    }
    ঘূৰাই storage.get();
}



স্থির তার্কিক activated = মিছা;
স্থির তার্কিক isInitialized = মিছা;

TraceManager::TraceManager()
{
    (শূন্য)cv::getTimestampNS();

    isInitialized = সঁচা;
    CV_LOG("TraceManager ctor: " << (শূন্য*)এতত্);

    CV_LOG("TraceManager configure()");
    activated = getParameterTraceEnable();

    যদি (activated)
        trace_storage.reset(নূতন SyncTraceStorage(std::string(getParameterTraceLocation()) + ".txt"));

#ifdef OPENCV_WITH_ITT
    যদি (isITTEnabled())
    {
        activated = সঁচা; // force trace pipeline activation (without OpenCV storage)
        __itt_region_begin(domain, __itt_null, __itt_null, __itt_string_handle_create("OpenCVTrace"));
    }
#endif
}
TraceManager::~TraceManager()
{
    CV_LOG("TraceManager dtor: " << (শূন্য*)এতত্);

#ifdef OPENCV_WITH_ITT
    যদি (isITTEnabled())
    {
        __itt_region_end(domain, __itt_null);
    }
#endif

    std::vector<TraceManagerThreadLocal*> threads_ctx;
    tls.gather(threads_ctx);
    size_t totalEvents = 0, totalSkippedEvents = 0;
    বাবে (size_t i = 0; i < threads_ctx.size(); i++)
    {
        TraceManagerThreadLocal* ctx = threads_ctx[i];
        যদি (ctx)
        {
            totalEvents += ctx->region_counter;
            totalSkippedEvents += ctx->totalSkippedEvents;
        }
    }
    যদি (totalEvents || activated)
    {
        CV_LOG_INFO(রিক্ত, "Trace: Total events: " << totalEvents);
    }
    যদি (totalSkippedEvents)
    {
        CV_LOG_WARNING(রিক্ত, "Trace: Total skipped events: " << totalSkippedEvents);
    }

    // This is a global static object, so process starts shutdown here
    // Turn off trace
    cv::__termination = সঁচা; // also set in DllMain() notifications handler for DLL_PROCESS_DETACH
    activated = মিছা;
}

তার্কিক TraceManager::isActivated()
{
    // Check if process starts shutdown, and set earlyExit to true
    // to prevent further instrumentation processing earlier.
    যদি (cv::__termination)
    {
        activated = মিছা;
        ঘূৰাই মিছা;
    }

    যদি (!isInitialized)
    {
        TraceManager& m = getTraceManager();
        CV_UNUSED(m); // TODO
    }

    ঘূৰাই activated;
}


স্থির TraceManager* getTraceManagerCallOnce()
{
    স্থির TraceManager globalInstance;
    ঘূৰাই &globalInstance;
}
TraceManager& getTraceManager()
{
    CV_SINGLETON_LAZY_INIT_REF(TraceManager, getTraceManagerCallOnce())
}

শূন্য parallel_For_Set_মূল_Region(ধ্রুৱ Region& মূল_Region, ধ্রুৱ TraceManagerThreadLocal& মূল_ctx)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();

    যদি (ctx.dummy_স্তূপ_শীৰ্ষ.region == &মূল_Region) // already attached
        ঘূৰাই;

    CV_Assert(ctx.dummy_স্তূপ_শীৰ্ষ.region == রিক্ত);
    ctx.dummy_স্তূপ_শীৰ্ষ = TraceManagerThreadLocal::স্তূপ_Entry(const_cast<Region*>(&মূল_Region), রিক্ত, -1);

    যদি (&ctx == &মূল_ctx)
    {
        ctx.stat.grab(ctx.parallel_for_stat);
        ctx.parallel_for_stat_status = ctx.stat_status;
        ctx.parallel_for_স্তূপ_size = ctx.স্তূপ.size();
        ঘূৰাই;
    }

    CV_Assert(ctx.স্তূপ.empty());

    ctx.currentActiveRegion = const_cast<Region*>(&মূল_Region);

    ctx.regionDepth = মূল_ctx.regionDepth;
    ctx.regionDepthOpenCV = মূল_ctx.regionDepthOpenCV;

    ctx.parallel_for_স্তূপ_size = 0;

    ctx.stat_status.propagateFrom(মূল_ctx.stat_status);
}

শূন্য parallelForAttachNestedRegion(ধ্রুৱ Region& মূল_Region)
{
    CV_UNUSED(মূল_Region);
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();

    CV_DbgAssert(ctx.dummy_স্তূপ_শীৰ্ষ.region == &মূল_Region);

    Region* region = ctx.getCurrentActiveRegion();
    CV_LOG_PARALLEL(রিক্ত, " PARALLEL_FOR: " << (শূন্য*)region << " ==> " << &মূল_Region);
    যদি (!region)
        ঘূৰাই;

#ifdef OPENCV_WITH_ITT
    যদি (!মূল_Region.pImpl || !মূল_Region.pImpl->itt_id_registered)
        ঘূৰাই;

    যদি (!region->pImpl)
        ঘূৰাই;

    CV_LOG_PARALLEL(রিক্ত, " PARALLEL_FOR ITT: " << (শূন্য*)মূল_Region.pImpl->itt_id.d1 << ":" << মূল_Region.pImpl->itt_id.d2 << ":" << (শূন্য*)মূল_Region.pImpl->itt_id.d3 << " => "
                                 << (শূন্য*)region->pImpl->itt_id.d1 << ":" << region->pImpl->itt_id.d2 << ":" << (শূন্য*)region->pImpl->itt_id.d3);
    __itt_relation_add(domain, region->pImpl->itt_id, __itt_relation_is_child_of, মূল_Region.pImpl->itt_id);
#endif
}

শূন্য parallelForFinalize(ধ্রুৱ Region& মূল_Region)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();

    int64 endTimestamp = getTimestampNS();
    int64 duration = endTimestamp - ctx.স্তূপ_শীৰ্ষ_Begin_Timestamp();
    CV_LOG_PARALLEL(রিক্ত, "parallel_for duration: " << duration << " " << &মূল_Region);

    std::vector<TraceManagerThreadLocal*> threads_ctx;
    getTraceManager().tls.gather(threads_ctx);
    RegionStatistics parallel_for_stat;
    বাবে (size_t i = 0; i < threads_ctx.size(); i++)
    {
        TraceManagerThreadLocal* child_ctx = threads_ctx[i];

        যদি (child_ctx && child_ctx->স্তূপ_শীৰ্ষ_Region() == &মূল_Region)
        {
            CV_LOG_PARALLEL(রিক্ত, "Thread=" << child_ctx->threadID << " " << child_ctx->stat);
            RegionStatistics child_stat;
            child_ctx->stat.grab(child_stat);
            parallel_for_stat.append(child_stat);
            যদি (child_ctx != &ctx)
            {
                child_ctx->dummy_স্তূপ_শীৰ্ষ = TraceManagerThreadLocal::স্তূপ_Entry();
            }
            অন্যথা
            {
                ctx.parallel_for_stat.grab(ctx.stat);
                ctx.stat_status = ctx.parallel_for_stat_status;
                child_ctx->dummy_স্তূপ_শীৰ্ষ = TraceManagerThreadLocal::স্তূপ_Entry();
            }
        }
    }

    ভিন্ন parallel_coeff = std::min(1.0f, duration / (ভিন্ন)(parallel_for_stat.duration));
    CV_LOG_PARALLEL(রিক্ত, "parallel_coeff=" << 1.0f / parallel_coeff);
    CV_LOG_PARALLEL(রিক্ত, parallel_for_stat);
    যদি (parallel_coeff != 1.0f)
    {
        parallel_for_stat.multiply(parallel_coeff);
        CV_LOG_PARALLEL(রিক্ত, parallel_for_stat);
    }
    parallel_for_stat.duration = 0;
    ctx.stat.append(parallel_for_stat);
    CV_LOG_PARALLEL(রিক্ত, ctx.stat);
}

সংরচনা TraceArg::ExtraData
{
#ifdef OPENCV_WITH_ITT
    // Special fields for ITT
    __itt_string_handle* চঞ্চল ittHandle_name;
#endif
    ExtraData(TraceManagerThreadLocal& ctx, ধ্রুৱ TraceArg& arg)
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

স্থির শূন্য initTraceArg(TraceManagerThreadLocal& ctx, ধ্রুৱ TraceArg& arg)
{
    TraceArg::ExtraData** pExtra = arg.ppExtra;
    যদি (*pExtra == রিক্ত)
    {
        cv::AutoLock lock(cv::getInitializationMutex());
        যদি (*pExtra == রিক্ত)
        {
            *pExtra = নূতন TraceArg::ExtraData(ctx, arg);
        }
    }
}
শূন্য traceArg(ধ্রুৱ TraceArg& arg, ধ্রুৱ ৱর্ণ* value)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    Region* region = ctx.getCurrentActiveRegion();
    যদি (!region)
        ঘূৰাই;
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
শূন্য traceArg(ধ্রুৱ TraceArg& arg, পূর্ণাঙ্ক value)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    Region* region = ctx.getCurrentActiveRegion();
    যদি (!region)
        ঘূৰাই;
    CV_Assert(region->pImpl);
    initTraceArg(ctx, arg);
#ifdef OPENCV_WITH_ITT
    যদি (isITTEnabled())
    {
        __itt_metadata_add(domain, region->pImpl->itt_id, (*arg.ppExtra)->ittHandle_name, পরিমাণ(পূর্ণাঙ্ক) == 4 ? __itt_metadata_s32 : __itt_metadata_s64, 1, &value);
    }
#অন্যথা
    CV_UNUSED(value);
#endif
}
শূন্য traceArg(ধ্রুৱ TraceArg& arg, int64 value)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    Region* region = ctx.getCurrentActiveRegion();
    যদি (!region)
        ঘূৰাই;
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
শূন্য traceArg(ধ্রুৱ TraceArg& arg, দ্ৱিগুণ value)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    Region* region = ctx.getCurrentActiveRegion();
    যদি (!region)
        ঘূৰাই;
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

Region::Region(ধ্রুৱ LocationStaticStorage&) : pImpl(রিক্ত), implFlags(0) {}
শূন্য Region::destroy() {}

শূন্য traceArg(ধ্রুৱ TraceArg&, ধ্রুৱ ৱর্ণ*) {}
শূন্য traceArg(ধ্রুৱ TraceArg&, পূর্ণাঙ্ক) {};
শূন্য traceArg(ধ্রুৱ TraceArg&, int64) {};
শূন্য traceArg(ধ্রুৱ TraceArg&, দ্ৱিগুণ) {};

#endif

}}}} // namespace
