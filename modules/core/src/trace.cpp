// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#अन्तर्भाव "precomp.hpp"

#अन्तर्भाव <opencv2/core/utils/trace.hpp>
#अन्तर्भाव <opencv2/core/utils/trace.private.hpp>
#अन्तर्भाव <opencv2/core/utils/configuration.private.hpp>

#अन्तर्भाव <opencv2/core/opencl/ocl_defs.hpp>

#अन्तर्भाव <cstdarg> // va_start

#अन्तर्भाव <sstream>
#अन्तर्भाव <ostream>
#अन्तर्भाव <fstream>

#यदि 0
#define CV_LOG(...) CV_LOG_INFO(रिक्त, __VA_ARGS__)
#अन्यथा
#define CV_LOG(...) {}
#endif

#यदि 0
#define CV_LOG_ITT(...) CV_LOG_INFO(रिक्त, __VA_ARGS__)
#अन्यथा
#define CV_LOG_ITT(...) {}
#endif

#यदि 1
#define CV_LOG_TRACE_BAILOUT(tag, ...) CV_LOG_INFO(tag, __VA_ARGS__)
#अन्यथा
#define CV_LOG_TRACE_BAILOUT(...) {}
#endif

#यदि 0
#define CV_LOG_PARALLEL(tag, ...) CV_LOG_INFO(tag, __VA_ARGS__)
#अन्यथा
#define CV_LOG_PARALLEL(...) {}
#endif

#यदि 0
#define CV_LOG_CTX_STAT(tag, ...) CV_LOG_INFO(tag, __VA_ARGS__)
#अन्यथा
#define CV_LOG_CTX_STAT(...) {}
#endif

#यदि 0
#define CV_LOG_SKIP(tag, ...) CV_LOG_INFO(tag, __VA_ARGS__)
#अन्यथा
#define CV_LOG_SKIP(...) {}
#endif

नामाकाश cv {
नामाकाश utils {
नामाकाश trace {
नामाकाश details {

#ifdef OPENCV_TRACE

#ifdef _MSC_VER
#pragma warning(disable:4065) // switch statement contains 'default' but no 'case' labels
#endif

स्थिर तार्किक getParameterTraceEnable()
{
    स्थिर तार्किक param_traceEnable = utils::getConfigurationParameterBool("OPENCV_TRACE", असत्य);
    प्रत्यागम param_traceEnable;
}

// TODO lazy configuration flags
स्थिर पूर्णाङ्क param_maxRegionDepthOpenCV = (पूर्णाङ्क)utils::getConfigurationParameterSizeT("OPENCV_TRACE_DEPTH_OPENCV", 1);
स्थिर पूर्णाङ्क param_maxRegionChildrenOpenCV = (पूर्णाङ्क)utils::getConfigurationParameterSizeT("OPENCV_TRACE_MAX_CHILDREN_OPENCV", 1000);
स्थिर पूर्णाङ्क param_maxRegionChildren = (पूर्णाङ्क)utils::getConfigurationParameterSizeT("OPENCV_TRACE_MAX_CHILDREN", 10000);

स्थिर ध्रुव cv::String& getParameterTraceLocation()
{
    स्थिर cv::String param_traceLocation = utils::getConfigurationParameterString("OPENCV_TRACE_LOCATION", "OpenCVTrace");
    प्रत्यागम param_traceLocation;
}

#ifdef HAVE_OPENCL
स्थिर तार्किक param_synchronizeOpenCL = utils::getConfigurationParameterBool("OPENCV_TRACE_SYNC_OPENCL", असत्य);
#endif

#ifdef OPENCV_WITH_ITT
स्थिर तार्किक param_ITT_registerParentScope = utils::getConfigurationParameterBool("OPENCV_TRACE_ITT_PARENT", असत्य);
#endif

स्थिर ध्रुव वर्ण* _spaces(पूर्णाङ्क मोजणी_2)
{
    स्थिर ध्रुव वर्ण buf[64] =
"                                                               ";
    प्रत्यागम &buf[63 - (मोजणी_2 & 63)];
}

/**
 * Text-based trace messages
 */
वर्ग TraceMessage
{
सार्वजनिक:
    वर्ण buffer[1024];
    size_t len;
    तार्किक hasError;

    TraceMessage() :
        len(0),
        hasError(असत्य)
    {}

    तार्किक printf(ध्रुव वर्ण* format, ...)
    {
        वर्ण* buf = &buffer[len];
        size_t sz = परिमाण(buffer) - len;
        va_list ap;
        va_start(ap, format);
        पूर्णाङ्क n = cv_vsnprintf(buf, (पूर्णाङ्क)sz, format, ap);
        va_end(ap);
        यदि (n < 0 || (size_t)n > sz)
        {
            hasError = सत्य;
            प्रत्यागम असत्य;
        }
        len += n;
        प्रत्यागम सत्य;
    }

    तार्किक formatlocation(ध्रुव Region::LocationStaticStorage& location)
    {
        प्रत्यागम एतत्->printf("l,%lld,\"%s\",%d,\"%s\",0x%llX\n",
                (long long पूर्णाङ्क)(*location.ppExtra)->global_location_id,
                location.filename,
                location.ओळ,
                location.name,
                (long long पूर्णाङ्क)(location.flags & ~0xF0000000));
    }
    तार्किक formatRegionEnter(ध्रुव Region& region)
    {
        तार्किक ok = एतत्->printf("b,%d,%lld,%lld,%lld",
                (पूर्णाङ्क)region.pImpl->threadID,
                (long long पूर्णाङ्क)region.pImpl->beginTimestamp,
                (long long पूर्णाङ्क)((*region.pImpl->location.ppExtra)->global_location_id),
                (long long पूर्णाङ्क)region.pImpl->global_region_id);
        यदि (region.pImpl->parentRegion && region.pImpl->parentRegion->pImpl)
        {
            यदि (region.pImpl->parentRegion->pImpl->threadID != region.pImpl->threadID)
                ok &= एतत्->printf(",parentThread=%d,parent=%lld",
                        (पूर्णाङ्क)region.pImpl->parentRegion->pImpl->threadID,
                        (long long पूर्णाङ्क)region.pImpl->parentRegion->pImpl->global_region_id);
        }
        ok &= एतत्->printf("\n");
        प्रत्यागम ok;
    }
    तार्किक formatRegionLeave(ध्रुव Region& region, ध्रुव RegionStatistics& result)
    {
        CV_DbgAssert(region.pImpl->endTimestamp - region.pImpl->beginTimestamp == result.duration);
        तार्किक ok = एतत्->printf("e,%d,%lld,%lld,%lld,%lld",
                (पूर्णाङ्क)region.pImpl->threadID,
                (long long पूर्णाङ्क)region.pImpl->endTimestamp,
                (long long पूर्णाङ्क)(*region.pImpl->location.ppExtra)->global_location_id,
                (long long पूर्णाङ्क)region.pImpl->global_region_id,
                (long long पूर्णाङ्क)result.duration);
        यदि (result.currentSkippedRegions)
            ok &= एतत्->printf(",skip=%d", (पूर्णाङ्क)result.currentSkippedRegions);
#ifdef HAVE_IPP
        यदि (result.durationImplIPP)
            ok &= एतत्->printf(",tIPP=%lld", (long long पूर्णाङ्क)result.durationImplIPP);
#endif
#ifdef HAVE_OPENCL
        यदि (result.durationImplOpenCL)
            ok &= एतत्->printf(",tOCL=%lld", (long long पूर्णाङ्क)result.durationImplOpenCL);
#endif
        ok &= एतत्->printf("\n");
        प्रत्यागम ok;
    }
    तार्किक recordRegionArg(ध्रुव Region& region, ध्रुव TraceArg& arg, ध्रुव वर्ण* value)
    {
        प्रत्यागम एतत्->printf("a,%d,%lld,%lld,\"%s\",\"%s\"\n",
                region.pImpl->threadID,
                (long long पूर्णाङ्क)region.pImpl->beginTimestamp,
                (long long पूर्णाङ्क)region.pImpl->global_region_id,
                arg.name,
                value);
    }
};


#ifdef OPENCV_WITH_ITT
स्थिर __itt_domain* domain = रिक्त;

स्थिर तार्किक isITTEnabled()
{
    स्थिर चञ्चल तार्किक isInitialized = असत्य;
    स्थिर तार्किक isEnabled = असत्य;
    यदि (!isInitialized)
    {
        cv::AutoLock lock(cv::getInitializationMutex());
        यदि (!isInitialized)
        {
            तार्किक param_traceITTEnable = utils::getConfigurationParameterBool("OPENCV_TRACE_ITT_ENABLE", सत्य);
            यदि (param_traceITTEnable)
            {
                isEnabled = !!(__itt_api_version());
                CV_LOG_ITT("ITT is " << (isEnabled ? "enabled" : "disabled"));
                domain = __itt_domain_create("OpenCVTrace");
            }
            अन्यथा
            {
                CV_LOG_ITT("ITT is disabled through OpenCV parameter");
                isEnabled = असत्य;
            }
            isInitialized = सत्य;
        }
    }
    प्रत्यागम isEnabled;
}
#endif


Region::LocationExtraData::LocationExtraData(ध्रुव LocationStaticStorage& location)
{
    CV_UNUSED(location);
    स्थिर पूर्णाङ्क g_location_id_counter = 0;
    global_location_id = CV_XADD(&g_location_id_counter, 1) + 1;
    CV_LOG("Register location: " << global_location_id << " (" << (शून्य*)&location << ")"
            << std::endl << "    file: " << location.filename
            << std::endl << "    line: " << location.ओळ
            << std::endl << "    name: " << location.name);
#ifdef OPENCV_WITH_ITT
    यदि (isITTEnabled())
    {
        // Caching is not required here, because there is builtin cache.
        // https://software.intel.com/en-us/node/544203:
        //     Consecutive calls to __itt_string_handle_create with the same name return the same value.
        ittHandle_name = __itt_string_handle_create(location.name);
        ittHandle_filename = __itt_string_handle_create(location.filename);
    }
    अन्यथा
    {
        ittHandle_name = 0;
        ittHandle_filename = 0;
    }
#endif
}

/*static*/ Region::LocationExtraData* Region::LocationExtraData::init(ध्रुव Region::LocationStaticStorage& location)
{
    LocationExtraData** pLocationExtra = location.ppExtra;
    CV_DbgAssert(pLocationExtra);
    यदि (*pLocationExtra == रिक्त)
    {
        cv::AutoLock lock(cv::getInitializationMutex());
        यदि (*pLocationExtra == रिक्त)
        {
            *pLocationExtra = नूतन Region::LocationExtraData(location);
            TraceStorage* s = getTraceManager().trace_storage.get();
            यदि (s)
            {
                TraceMessage संदेश;
                संदेश.formatlocation(location);
                s->put(संदेश);
            }
        }
    }
    प्रत्यागम *pLocationExtra;
}


Region::Impl::Impl(TraceManagerThreadLocal& ctx, Region* parentRegion_, Region& region_, ध्रुव LocationStaticStorage& location_, int64 beginTimestamp_) :
    location(location_),
    region(region_),
    parentRegion(parentRegion_),
    threadID(ctx.threadID),
    global_region_id(++ctx.region_counter),
    beginTimestamp(beginTimestamp_),
    endTimestamp(0),
    direct_Children_मोजणी(0)
#ifdef OPENCV_WITH_ITT
    ,itt_id_registered(असत्य)
    ,itt_id(__itt_null)
#endif
{
    CV_DbgAssert(ctx.currentActiveRegion == parentRegion);
    region.pImpl = एतत्;

    registerRegion(ctx);

    enterRegion(ctx);
}

Region::Impl::~Impl()
{
#ifdef OPENCV_WITH_ITT
    यदि (itt_id_registered)
    {
        CV_LOG_ITT(" Destroy ITT region: I=" << (शून्य*)एतत्);
        __itt_id_destroy(domain, itt_id);
        itt_id_registered = असत्य;
    }
#endif
    region.pImpl = रिक्त;
}

शून्य Region::Impl::enterRegion(TraceManagerThreadLocal& ctx)
{
    ctx.currentActiveRegion = &region;

    यदि (location.flags & REGION_FLAG_FUNCTION)
    {
        यदि ((location.flags & REGION_FLAG_APP_CODE) == 0)
        {
            ctx.regionDepthOpenCV++;
        }
        ctx.regionDepth++;
    }

    TraceStorage* s = ctx.getStorage();
    यदि (s)
    {
        TraceMessage संदेश;
        संदेश.formatRegionEnter(region);
        s->put(संदेश);
    }
#ifdef OPENCV_WITH_ITT
    यदि (isITTEnabled())
    {
        __itt_id parentID = __itt_null;
        यदि (param_ITT_registerParentScope && parentRegion && parentRegion->pImpl && parentRegion->pImpl->itt_id_registered && (location.flags & REGION_FLAG_REGION_FORCE) == 0)
            parentID = parentRegion->pImpl->itt_id;
        __itt_task_begin(domain, itt_id, parentID, (*location.ppExtra)->ittHandle_name);
    }
#endif
}

शून्य Region::Impl::leaveRegion(TraceManagerThreadLocal& ctx)
{
    int64 duration = endTimestamp - beginTimestamp; CV_UNUSED(duration);
    RegionStatistics result;
    ctx.stat.grab(result);
    ctx.totalSkippedEvents += result.currentSkippedRegions;
    CV_LOG(_spaces(ctx.getCurrentDepth()*4) << "leaveRegion(): " << (शून्य*)एतत् << " " << result);
#ifdef OPENCV_WITH_ITT
    यदि (isITTEnabled())
    {
        यदि (result.currentSkippedRegions)
        {
            __itt_metadata_add(domain, itt_id, __itt_string_handle_create("skipped trace entries"), __itt_metadata_u32, 1, &result.currentSkippedRegions);
        }
#ifdef HAVE_IPP
        यदि (result.durationImplIPP)
            __itt_metadata_add(domain, itt_id, __itt_string_handle_create("tIPP"), __itt_metadata_u64, 1, &result.durationImplIPP);
#endif
#ifdef HAVE_OPENCL
        यदि (result.durationImplOpenCL)
            __itt_metadata_add(domain, itt_id, __itt_string_handle_create("tOpenCL"), __itt_metadata_u64, 1, &result.durationImplOpenCL);
#endif
        __itt_task_end(domain);
    }
#endif
    TraceStorage* s = ctx.getStorage();
    यदि (s)
    {
        TraceMessage संदेश;
        संदेश.formatRegionLeave(region, result);
        s->put(संदेश);
    }

    यदि (location.flags & REGION_FLAG_FUNCTION)
    {
        यदि ((location.flags & REGION_FLAG_APP_CODE) == 0)
        {
            ctx.regionDepthOpenCV--;
        }
        ctx.regionDepth--;
    }

    ctx.currentActiveRegion = parentRegion;
}

शून्य Region::Impl::release()
{
    delete एतत्;
}

शून्य Region::Impl::registerRegion(TraceManagerThreadLocal& ctx)
{
#ifdef OPENCV_WITH_ITT
    यदि (isITTEnabled())
    {
        यदि (!itt_id_registered)
        {
            CV_LOG_ITT(" Register ITT region: I=" << (शून्य*)एतत् << " " << ctx.threadID << "-" << global_region_id);
#यदि 1 // workaround for some ITT backends
            itt_id = __itt_id_make((शून्य*)(intptr_t)(((int64)(ctx.threadID + 1) << 32) | global_region_id), global_region_id);
#अन्यथा
            itt_id = __itt_id_make((शून्य*)(intptr_t)(ctx.threadID + 1), global_region_id);
#endif
            __itt_id_create(domain, itt_id);
            itt_id_registered = सत्य;
        }
    }
#अन्यथा
    CV_UNUSED(ctx);
#endif
}

शून्य RegionStatisticsStatus::enableSkipMode(पूर्णाङ्क depth)
{
    CV_DbgAssert(_skipDepth < 0);
    CV_LOG_SKIP(रिक्त, "SKIP-ENABLE: depth=" << depth);
    _skipDepth = depth;
}
शून्य RegionStatisticsStatus::checkResetSkipMode(पूर्णाङ्क leaveDepth)
{
    यदि (leaveDepth <= _skipDepth)
    {
        CV_LOG_SKIP(रिक्त, "SKIP-RESET: leaveDepth=" << leaveDepth << " skipDepth=" << _skipDepth);
        _skipDepth = -1;
    }
}

Region::Region(ध्रुव LocationStaticStorage& location) :
    pImpl(रिक्त),
    implFlags(0)
{
    // Checks:
    // - global enable flag
    // - parent region is disabled
    // - children count threshold
    // - region location
    // - depth (opencv nested calls)
    यदि (!TraceManager::isActivated())
    {
        CV_LOG("Trace is disabled. Bailout");
        प्रत्यागम;
    }

    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    CV_LOG(_spaces(ctx.getCurrentDepth()*4) << "Region(): " << (शून्य*)एतत् << ": " << location.name);

    Region* parentRegion = ctx.चळत_शीर्ष_Region();
    ध्रुव Region::LocationStaticStorage* parentLocation = ctx.चळत_शीर्ष_Location();

    यदि (location.flags & REGION_FLAG_REGION_NEXT)
    {
        यदि (parentRegion && parentRegion->pImpl)
        {
            CV_DbgAssert((parentRegion->pImpl->location.flags & REGION_FLAG_FUNCTION) == 0);
            parentRegion->destroy(); parentRegion->implFlags = 0;
            parentRegion = ctx.चळत_शीर्ष_Region();
            parentLocation = ctx.चळत_शीर्ष_Location();
        }
    }

    पूर्णाङ्क parentChildren = 0;
    यदि (parentRegion && parentRegion->pImpl)
    {
        यदि (parentLocation == रिक्त)
        {
            // parallel_for_body code path
            parentChildren = CV_XADD(&parentRegion->pImpl->direct_Children_मोजणी, 1) + 1;
        }
        अन्यथा
        {
            parentChildren = ++parentRegion->pImpl->direct_Children_मोजणी;
        }
    }

    int64 beginTimestamp = getTimestampNS();

    पूर्णाङ्क currentDepth = ctx.getCurrentDepth() + 1;
    विकल्प (location.flags & REGION_FLAG_IMPL_MASK)
    {
#ifdef HAVE_IPP
    पक्ष REGION_FLAG_IMPL_IPP:
        यदि (!ctx.stat_status.ignoreDepthImplIPP)
            ctx.stat_status.ignoreDepthImplIPP = currentDepth;
        भङ्ग;
#endif
#ifdef HAVE_OPENCL
    पक्ष REGION_FLAG_IMPL_OPENCL:
        यदि (!ctx.stat_status.ignoreDepthImplOpenCL)
            ctx.stat_status.ignoreDepthImplOpenCL = currentDepth;
        भङ्ग;
#endif
    स्वभाव:
        भङ्ग;
    }

    ctx.चळत_ढकला(एतत्, &location, beginTimestamp);
    implFlags |= REGION_FLAG_NEED_चळत_काढा;

    यदि ((location.flags & REGION_FLAG_REGION_FORCE) == 0)
    {
        यदि (ctx.stat_status._skipDepth >= 0 && currentDepth > ctx.stat_status._skipDepth)
        {
            CV_LOG(_spaces(ctx.getCurrentDepth()*4) << "Parent region is disabled. Bailout");
            ctx.stat.currentSkippedRegions++;
            प्रत्यागम;
        }

        यदि (param_maxRegionChildrenOpenCV > 0 && (location.flags & REGION_FLAG_APP_CODE) == 0 && parentLocation && (parentLocation->flags & REGION_FLAG_APP_CODE) == 0)
        {
            यदि (parentChildren >= param_maxRegionChildrenOpenCV)
            {
                CV_LOG_TRACE_BAILOUT(रिक्त, _spaces(ctx.getCurrentDepth()*4) << "OpenCV parent region exceeds children count. Bailout");
                ctx.stat_status.enableSkipMode(currentDepth - 1);
                ctx.stat.currentSkippedRegions++;
                DEBUG_ONLY(ctx.dump_चळत(std::cout, असत्य));
                प्रत्यागम;
            }
        }
        यदि (param_maxRegionChildren > 0 && parentChildren >= param_maxRegionChildren)
        {
            CV_LOG_TRACE_BAILOUT(रिक्त, _spaces(ctx.getCurrentDepth()*4) << "Parent region exceeds children count. Bailout");
            ctx.stat_status.enableSkipMode(currentDepth - 1);
            ctx.stat.currentSkippedRegions++;
            DEBUG_ONLY(ctx.dump_चळत(std::cout, असत्य));
            प्रत्यागम;
        }
    }

    LocationExtraData::init(location);

    यदि ((*location.ppExtra)->global_location_id == 0)
    {
        CV_LOG_TRACE_BAILOUT(रिक्त, _spaces(ctx.getCurrentDepth()*4) << "Region location is disabled. Bailout");
        ctx.stat_status.enableSkipMode(currentDepth);
        ctx.stat.currentSkippedRegions++;
        प्रत्यागम;
    }

    यदि (parentLocation && (parentLocation->flags & REGION_FLAG_SKIP_NESTED))
    {
        CV_LOG(_spaces(ctx.getCurrentDepth()*4) << "Parent region disables inner regions. Bailout");
        ctx.stat_status.enableSkipMode(currentDepth);
        ctx.stat.currentSkippedRegions++;
        प्रत्यागम;
    }

    यदि (param_maxRegionDepthOpenCV)
    {
        यदि ((location.flags & REGION_FLAG_APP_CODE) == 0)
        {
            यदि (ctx.regionDepthOpenCV >= param_maxRegionDepthOpenCV)
            {
                CV_LOG(_spaces(ctx.getCurrentDepth()*4) << "OpenCV region depth is exceed = " << ctx.regionDepthOpenCV << ". Bailout");
                यदि (ctx.stat.currentSkippedRegions == 0)
                {
                    DEBUG_ONLY(ctx.dump_चळत(std::cout, असत्य));
                }
                ctx.stat_status.enableSkipMode(currentDepth);
                ctx.stat.currentSkippedRegions++;
                प्रत्यागम;
            }
        }
    }

    नूतन Impl(ctx, parentRegion, *एतत्, location, beginTimestamp);
    CV_DbgAssert(pImpl != रिक्त);
    implFlags |= REGION_FLAG__ACTIVE;

    // parallel_for path
    यदि (parentRegion && parentRegion->pImpl)
    {
        यदि (parentLocation == रिक्त)
        {
            pImpl->direct_Children_मोजणी = parentChildren;
        }
    }
}

शून्य Region::destroy()
{
    CV_DbgAssert(implFlags != 0);

    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    CV_LOG(_spaces(ctx.getCurrentDepth()*4) << "Region::destruct(): " << (शून्य*)एतत् << " pImpl=" << pImpl << " implFlags=" << implFlags << ' ' << (ctx.चळत_शीर्ष_Location() ? ctx.चळत_शीर्ष_Location()->name : "<unknown>"));

    CV_DbgAssert(implFlags & REGION_FLAG_NEED_चळत_काढा);
    ध्रुव पूर्णाङ्क currentDepth = ctx.getCurrentDepth(); CV_UNUSED(currentDepth);

    CV_LOG_CTX_STAT(रिक्त, _spaces(currentDepth*4) << ctx.stat << ' ' << ctx.stat_status);

    ध्रुव Region::LocationStaticStorage* location = ctx.चळत_शीर्ष_Location();
    Impl::OptimizationPath myCodePath = Impl::CODE_PATH_PLAIN;
    यदि (location)
    {
        विकल्प (location->flags & REGION_FLAG_IMPL_MASK)
        {
#ifdef HAVE_IPP
        पक्ष REGION_FLAG_IMPL_IPP:
            myCodePath = Impl::CODE_PATH_IPP;
            भङ्ग;
#endif
#ifdef HAVE_OPENCL
        पक्ष REGION_FLAG_IMPL_OPENCL:
            यदि (param_synchronizeOpenCL && cv::ocl::isOpenCLActivated())
                cv::ocl::finish();
            myCodePath = Impl::CODE_PATH_OPENCL;
            भङ्ग;
#endif
        स्वभाव:
            भङ्ग;
        }
    }

    int64 endTimestamp = getTimestampNS();
    int64 duration = endTimestamp - ctx.चळत_शीर्ष_Begin_Timestamp();

    तार्किक active = isActive();

    यदि (active)
        ctx.stat.duration = duration;
    अन्यथा यदि (ctx.चळत.size() == ctx.parallel_for_चळत_size + 1)
        ctx.stat.duration += duration;

    विकल्प (myCodePath) {
        पक्ष Impl::CODE_PATH_PLAIN:
            // nothing
            भङ्ग;
#ifdef HAVE_IPP
        पक्ष Impl::CODE_PATH_IPP:
            यदि (ctx.stat_status.ignoreDepthImplIPP == currentDepth)
            {
                ctx.stat.durationImplIPP += duration;
                ctx.stat_status.ignoreDepthImplIPP = 0;
            }
            अन्यथा यदि (active)
            {
                ctx.stat.durationImplIPP = duration;
            }
            भङ्ग;
#endif
#ifdef HAVE_OPENCL
        पक्ष Impl::CODE_PATH_OPENCL:
            यदि (ctx.stat_status.ignoreDepthImplOpenCL == currentDepth)
            {
                ctx.stat.durationImplOpenCL += duration;
                ctx.stat_status.ignoreDepthImplOpenCL = 0;
            }
            अन्यथा यदि (active)
            {
                ctx.stat.durationImplOpenCL = duration;
            }
            भङ्ग;
#endif
        स्वभाव:
            भङ्ग;
    }

    यदि (pImpl)
    {
        CV_DbgAssert((implFlags & (REGION_FLAG__ACTIVE | REGION_FLAG_NEED_चळत_काढा)) == (REGION_FLAG__ACTIVE | REGION_FLAG_NEED_चळत_काढा));
        CV_DbgAssert(ctx.चळत_शीर्ष_Region() == एतत्);
        pImpl->endTimestamp = endTimestamp;
        pImpl->leaveRegion(ctx);
        pImpl->release();
        pImpl = रिक्त;
        DEBUG_ONLY(implFlags &= ~REGION_FLAG__ACTIVE);
    }
    अन्यथा
    {
        CV_DbgAssert(ctx.stat_status._skipDepth <= currentDepth);
    }

    यदि (implFlags & REGION_FLAG_NEED_चळत_काढा)
    {
        CV_DbgAssert(ctx.चळत_शीर्ष_Region() == एतत्);
        ctx.चळत_काढा();
        ctx.stat_status.checkResetSkipMode(currentDepth);
        DEBUG_ONLY(implFlags &= ~REGION_FLAG_NEED_चळत_काढा);
    }
    CV_LOG_CTX_STAT(रिक्त, _spaces(currentDepth*4) << "===> " << ctx.stat << ' ' << ctx.stat_status);
}


TraceManagerThreadLocal::~TraceManagerThreadLocal()
{
}

शून्य TraceManagerThreadLocal::dump_चळत(std::ostream& out, तार्किक onlyFunctions) ध्रुव
{
    std::stringstream ss;
    std::deque<चळत_Entry>::const_iterator it = चळत.begin();
    std::deque<चळत_Entry>::const_iterator end = चळत.end();
    पूर्णाङ्क depth = 0;
    प्रति (; it != end; ++it)
    {
        ध्रुव Region::LocationStaticStorage* location = it->location;
        यदि (location)
        {
            यदि (!onlyFunctions || (location->flags & REGION_FLAG_FUNCTION))
            {
                ss << _spaces(4*depth) << location->name << std::endl;
                depth++;
            }
        }
        अन्यथा
        {
            ss << _spaces(4*depth) << "<unknown>" << std::endl;
            depth++;
        }
    }
    out << ss.str();
}

वर्ग AsyncTraceStorage CV_FINAL : सार्वजनिक TraceStorage
{
    mutable std::ofstream out;
सार्वजनिक:
    ध्रुव std::string name;

    AsyncTraceStorage(ध्रुव std::string& filename) :
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

    तार्किक put(ध्रुव TraceMessage& संदेश) ध्रुव CV_OVERRIDE
    {
        यदि (संदेश.hasError)
            प्रत्यागम असत्य;
        out << संदेश.buffer;
        //DEBUG_ONLY(std::flush(out)); // TODO configure flag
        प्रत्यागम सत्य;
    }
};

वर्ग SyncTraceStorage CV_FINAL : सार्वजनिक TraceStorage
{
    mutable std::ofstream out;
    mutable cv::Mutex mutex;
सार्वजनिक:
    ध्रुव std::string name;

    SyncTraceStorage(ध्रुव std::string& filename) :
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

    तार्किक put(ध्रुव TraceMessage& संदेश) ध्रुव CV_OVERRIDE
    {
        यदि (संदेश.hasError)
            प्रत्यागम असत्य;
        {
            cv::AutoLock l(mutex);
            out << संदेश.buffer;
            std::flush(out); // TODO configure flag
        }
        प्रत्यागम सत्य;
    }
};


TraceStorage* TraceManagerThreadLocal::getStorage() ध्रुव
{
    // TODO configuration option for stdout/single trace file
    यदि (storage.empty())
    {
        TraceStorage* global = getTraceManager().trace_storage.get();
        यदि (global)
        {
            ध्रुव std::string filepath = cv::format("%s-%03d.txt", getParameterTraceLocation().c_str(), threadID).c_str();
            TraceMessage संदेश;
            ध्रुव वर्ण* pos = strrchr(filepath.c_str(), '/'); // extract filename
#ifdef _WIN32
            यदि (!pos)
                pos = strrchr(filepath.c_str(), '\\');
#endif
            यदि (!pos)
                pos = filepath.c_str();
            अन्यथा
                pos += 1; // fix to skip extra slash in filename beginning
            संदेश.printf("#thread file: %s\n", pos);
            global->put(संदेश);
            storage.reset(नूतन AsyncTraceStorage(filepath));
        }
    }
    प्रत्यागम storage.get();
}



स्थिर तार्किक activated = असत्य;
स्थिर तार्किक isInitialized = असत्य;

TraceManager::TraceManager()
{
    (शून्य)cv::getTimestampNS();

    isInitialized = सत्य;
    CV_LOG("TraceManager ctor: " << (शून्य*)एतत्);

    CV_LOG("TraceManager configure()");
    activated = getParameterTraceEnable();

    यदि (activated)
        trace_storage.reset(नूतन SyncTraceStorage(std::string(getParameterTraceLocation()) + ".txt"));

#ifdef OPENCV_WITH_ITT
    यदि (isITTEnabled())
    {
        activated = सत्य; // force trace pipeline activation (without OpenCV storage)
        __itt_region_begin(domain, __itt_null, __itt_null, __itt_string_handle_create("OpenCVTrace"));
    }
#endif
}
TraceManager::~TraceManager()
{
    CV_LOG("TraceManager dtor: " << (शून्य*)एतत्);

#ifdef OPENCV_WITH_ITT
    यदि (isITTEnabled())
    {
        __itt_region_end(domain, __itt_null);
    }
#endif

    std::vector<TraceManagerThreadLocal*> threads_ctx;
    tls.gather(threads_ctx);
    size_t totalEvents = 0, totalSkippedEvents = 0;
    प्रति (size_t i = 0; i < threads_ctx.size(); i++)
    {
        TraceManagerThreadLocal* ctx = threads_ctx[i];
        यदि (ctx)
        {
            totalEvents += ctx->region_counter;
            totalSkippedEvents += ctx->totalSkippedEvents;
        }
    }
    यदि (totalEvents || activated)
    {
        CV_LOG_INFO(रिक्त, "Trace: Total events: " << totalEvents);
    }
    यदि (totalSkippedEvents)
    {
        CV_LOG_WARNING(रिक्त, "Trace: Total skipped events: " << totalSkippedEvents);
    }

    // This is a global static object, so process starts shutdown here
    // Turn off trace
    cv::__termination = सत्य; // also set in DllMain() notifications handler for DLL_PROCESS_DETACH
    activated = असत्य;
}

तार्किक TraceManager::isActivated()
{
    // Check if process starts shutdown, and set earlyExit to true
    // to prevent further instrumentation processing earlier.
    यदि (cv::__termination)
    {
        activated = असत्य;
        प्रत्यागम असत्य;
    }

    यदि (!isInitialized)
    {
        TraceManager& m = getTraceManager();
        CV_UNUSED(m); // TODO
    }

    प्रत्यागम activated;
}


स्थिर TraceManager* getTraceManagerCallOnce()
{
    स्थिर TraceManager globalInstance;
    प्रत्यागम &globalInstance;
}
TraceManager& getTraceManager()
{
    CV_SINGLETON_LAZY_INIT_REF(TraceManager, getTraceManagerCallOnce())
}

शून्य parallel_For_Set_मूळ_Region(ध्रुव Region& मूळ_Region, ध्रुव TraceManagerThreadLocal& मूळ_ctx)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();

    यदि (ctx.dummy_चळत_शीर्ष.region == &मूळ_Region) // already attached
        प्रत्यागम;

    CV_Assert(ctx.dummy_चळत_शीर्ष.region == रिक्त);
    ctx.dummy_चळत_शीर्ष = TraceManagerThreadLocal::चळत_Entry(const_cast<Region*>(&मूळ_Region), रिक्त, -1);

    यदि (&ctx == &मूळ_ctx)
    {
        ctx.stat.grab(ctx.parallel_for_stat);
        ctx.parallel_for_stat_status = ctx.stat_status;
        ctx.parallel_for_चळत_size = ctx.चळत.size();
        प्रत्यागम;
    }

    CV_Assert(ctx.चळत.empty());

    ctx.currentActiveRegion = const_cast<Region*>(&मूळ_Region);

    ctx.regionDepth = मूळ_ctx.regionDepth;
    ctx.regionDepthOpenCV = मूळ_ctx.regionDepthOpenCV;

    ctx.parallel_for_चळत_size = 0;

    ctx.stat_status.propagateFrom(मूळ_ctx.stat_status);
}

शून्य parallelForAttachNestedRegion(ध्रुव Region& मूळ_Region)
{
    CV_UNUSED(मूळ_Region);
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();

    CV_DbgAssert(ctx.dummy_चळत_शीर्ष.region == &मूळ_Region);

    Region* region = ctx.getCurrentActiveRegion();
    CV_LOG_PARALLEL(रिक्त, " PARALLEL_FOR: " << (शून्य*)region << " ==> " << &मूळ_Region);
    यदि (!region)
        प्रत्यागम;

#ifdef OPENCV_WITH_ITT
    यदि (!मूळ_Region.pImpl || !मूळ_Region.pImpl->itt_id_registered)
        प्रत्यागम;

    यदि (!region->pImpl)
        प्रत्यागम;

    CV_LOG_PARALLEL(रिक्त, " PARALLEL_FOR ITT: " << (शून्य*)मूळ_Region.pImpl->itt_id.d1 << ":" << मूळ_Region.pImpl->itt_id.d2 << ":" << (शून्य*)मूळ_Region.pImpl->itt_id.d3 << " => "
                                 << (शून्य*)region->pImpl->itt_id.d1 << ":" << region->pImpl->itt_id.d2 << ":" << (शून्य*)region->pImpl->itt_id.d3);
    __itt_relation_add(domain, region->pImpl->itt_id, __itt_relation_is_child_of, मूळ_Region.pImpl->itt_id);
#endif
}

शून्य parallelForFinalize(ध्रुव Region& मूळ_Region)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();

    int64 endTimestamp = getTimestampNS();
    int64 duration = endTimestamp - ctx.चळत_शीर्ष_Begin_Timestamp();
    CV_LOG_PARALLEL(रिक्त, "parallel_for duration: " << duration << " " << &मूळ_Region);

    std::vector<TraceManagerThreadLocal*> threads_ctx;
    getTraceManager().tls.gather(threads_ctx);
    RegionStatistics parallel_for_stat;
    प्रति (size_t i = 0; i < threads_ctx.size(); i++)
    {
        TraceManagerThreadLocal* child_ctx = threads_ctx[i];

        यदि (child_ctx && child_ctx->चळत_शीर्ष_Region() == &मूळ_Region)
        {
            CV_LOG_PARALLEL(रिक्त, "Thread=" << child_ctx->threadID << " " << child_ctx->stat);
            RegionStatistics child_stat;
            child_ctx->stat.grab(child_stat);
            parallel_for_stat.append(child_stat);
            यदि (child_ctx != &ctx)
            {
                child_ctx->dummy_चळत_शीर्ष = TraceManagerThreadLocal::चळत_Entry();
            }
            अन्यथा
            {
                ctx.parallel_for_stat.grab(ctx.stat);
                ctx.stat_status = ctx.parallel_for_stat_status;
                child_ctx->dummy_चळत_शीर्ष = TraceManagerThreadLocal::चळत_Entry();
            }
        }
    }

    भिन्न parallel_coeff = std::min(1.0f, duration / (भिन्न)(parallel_for_stat.duration));
    CV_LOG_PARALLEL(रिक्त, "parallel_coeff=" << 1.0f / parallel_coeff);
    CV_LOG_PARALLEL(रिक्त, parallel_for_stat);
    यदि (parallel_coeff != 1.0f)
    {
        parallel_for_stat.multiply(parallel_coeff);
        CV_LOG_PARALLEL(रिक्त, parallel_for_stat);
    }
    parallel_for_stat.duration = 0;
    ctx.stat.append(parallel_for_stat);
    CV_LOG_PARALLEL(रिक्त, ctx.stat);
}

संरचना TraceArg::ExtraData
{
#ifdef OPENCV_WITH_ITT
    // Special fields for ITT
    __itt_string_handle* चञ्चल ittHandle_name;
#endif
    ExtraData(TraceManagerThreadLocal& ctx, ध्रुव TraceArg& arg)
    {
        CV_UNUSED(ctx); CV_UNUSED(arg);
#ifdef OPENCV_WITH_ITT
        यदि (isITTEnabled())
        {
            // Caching is not required here, because there is builtin cache.
            // https://software.intel.com/en-us/node/544203:
            //     Consecutive calls to __itt_string_handle_create with the same name return the same value.
            ittHandle_name = __itt_string_handle_create(arg.name);
        }
        अन्यथा
        {
            ittHandle_name = 0;
        }
#endif
    }
};

स्थिर शून्य initTraceArg(TraceManagerThreadLocal& ctx, ध्रुव TraceArg& arg)
{
    TraceArg::ExtraData** pExtra = arg.ppExtra;
    यदि (*pExtra == रिक्त)
    {
        cv::AutoLock lock(cv::getInitializationMutex());
        यदि (*pExtra == रिक्त)
        {
            *pExtra = नूतन TraceArg::ExtraData(ctx, arg);
        }
    }
}
शून्य traceArg(ध्रुव TraceArg& arg, ध्रुव वर्ण* value)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    Region* region = ctx.getCurrentActiveRegion();
    यदि (!region)
        प्रत्यागम;
    CV_Assert(region->pImpl);
    initTraceArg(ctx, arg);
    यदि (!value)
        value = "<null>";
#ifdef OPENCV_WITH_ITT
    यदि (isITTEnabled())
    {
        __itt_metadata_str_add(domain, region->pImpl->itt_id, (*arg.ppExtra)->ittHandle_name, value, strlen(value));
    }
#endif
}
शून्य traceArg(ध्रुव TraceArg& arg, पूर्णाङ्क value)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    Region* region = ctx.getCurrentActiveRegion();
    यदि (!region)
        प्रत्यागम;
    CV_Assert(region->pImpl);
    initTraceArg(ctx, arg);
#ifdef OPENCV_WITH_ITT
    यदि (isITTEnabled())
    {
        __itt_metadata_add(domain, region->pImpl->itt_id, (*arg.ppExtra)->ittHandle_name, परिमाण(पूर्णाङ्क) == 4 ? __itt_metadata_s32 : __itt_metadata_s64, 1, &value);
    }
#अन्यथा
    CV_UNUSED(value);
#endif
}
शून्य traceArg(ध्रुव TraceArg& arg, int64 value)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    Region* region = ctx.getCurrentActiveRegion();
    यदि (!region)
        प्रत्यागम;
    CV_Assert(region->pImpl);
    initTraceArg(ctx, arg);
#ifdef OPENCV_WITH_ITT
    यदि (isITTEnabled())
    {
        __itt_metadata_add(domain, region->pImpl->itt_id, (*arg.ppExtra)->ittHandle_name, __itt_metadata_s64, 1, &value);
    }
#अन्यथा
    CV_UNUSED(value);
#endif
}
शून्य traceArg(ध्रुव TraceArg& arg, द्विगुण value)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    Region* region = ctx.getCurrentActiveRegion();
    यदि (!region)
        प्रत्यागम;
    CV_Assert(region->pImpl);
    initTraceArg(ctx, arg);
#ifdef OPENCV_WITH_ITT
    यदि (isITTEnabled())
    {
        __itt_metadata_add(domain, region->pImpl->itt_id, (*arg.ppExtra)->ittHandle_name, __itt_metadata_double, 1, &value);
    }
#अन्यथा
    CV_UNUSED(value);
#endif
}

#अन्यथा

Region::Region(ध्रुव LocationStaticStorage&) : pImpl(रिक्त), implFlags(0) {}
शून्य Region::destroy() {}

शून्य traceArg(ध्रुव TraceArg&, ध्रुव वर्ण*) {}
शून्य traceArg(ध्रुव TraceArg&, पूर्णाङ्क) {};
शून्य traceArg(ध्रुव TraceArg&, int64) {};
शून्य traceArg(ध्रुव TraceArg&, द्विगुण) {};

#endif

}}}} // namespace
