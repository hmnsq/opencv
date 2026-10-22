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
    फर्काऊ param_traceEnable;
}

// TODO lazy configuration flags
स्थिर पूर्णाङ्क param_maxRegionDepthOpenCV = (पूर्णाङ्क)utils::getConfigurationParameterSizeT("OPENCV_TRACE_DEPTH_OPENCV", 1);
स्थिर पूर्णाङ्क param_maxRegionChildrenOpenCV = (पूर्णाङ्क)utils::getConfigurationParameterSizeT("OPENCV_TRACE_MAX_CHILDREN_OPENCV", 1000);
स्थिर पूर्णाङ्क param_maxRegionChildren = (पूर्णाङ्क)utils::getConfigurationParameterSizeT("OPENCV_TRACE_MAX_CHILDREN", 10000);

स्थिर अचर cv::String& getParameterTraceLocation()
{
    स्थिर cv::String param_traceLocation = utils::getConfigurationParameterString("OPENCV_TRACE_LOCATION", "OpenCVTrace");
    फर्काऊ param_traceLocation;
}

#ifdef HAVE_OPENCL
स्थिर तार्किक param_synchronizeOpenCL = utils::getConfigurationParameterBool("OPENCV_TRACE_SYNC_OPENCL", असत्य);
#endif

#ifdef OPENCV_WITH_ITT
स्थिर तार्किक param_ITT_registerParentScope = utils::getConfigurationParameterBool("OPENCV_TRACE_ITT_PARENT", असत्य);
#endif

स्थिर अचर वर्ण* _spaces(पूर्णाङ्क गन्ती_2)
{
    स्थिर अचर वर्ण buf[64] =
"                                                               ";
    फर्काऊ &buf[63 - (गन्ती_2 & 63)];
}

/**
 * Text-based trace messages
 */
वर्ग TraceMessage
{
सार्वजनिक:
    वर्ण buffer[1024];
    माप_प्रकार len;
    तार्किक hasError;

    TraceMessage() :
        len(0),
        hasError(असत्य)
    {}

    तार्किक म_लिखो(अचर वर्ण* format, ...)
    {
        वर्ण* buf = &buffer[len];
        माप_प्रकार sz = परिमाण(buffer) - len;
        बहु_सूची ap;
        बहु_शुरू(ap, format);
        पूर्णाङ्क n = cv_vsnprintf(buf, (पूर्णाङ्क)sz, format, ap);
        बहु_पूर्ण(ap);
        यदि (n < 0 || (माप_प्रकार)n > sz)
        {
            hasError = सत्य;
            फर्काऊ असत्य;
        }
        len += n;
        फर्काऊ सत्य;
    }

    तार्किक formatlocation(अचर Region::LocationStaticStorage& location)
    {
        फर्काऊ एतत्->म_लिखो("l,%lld,\"%s\",%d,\"%s\",0x%llX\n",
                (दीर्घ दीर्घ पूर्णाङ्क)(*location.ppExtra)->global_location_id,
                location.filename,
                location.पङ्क्ति,
                location.name,
                (दीर्घ दीर्घ पूर्णाङ्क)(location.flags & ~0xF0000000));
    }
    तार्किक formatRegionEnter(अचर Region& region)
    {
        तार्किक ok = एतत्->म_लिखो("b,%d,%lld,%lld,%lld",
                (पूर्णाङ्क)region.pImpl->threadID,
                (दीर्घ दीर्घ पूर्णाङ्क)region.pImpl->beginTimestamp,
                (दीर्घ दीर्घ पूर्णाङ्क)((*region.pImpl->location.ppExtra)->global_location_id),
                (दीर्घ दीर्घ पूर्णाङ्क)region.pImpl->global_region_id);
        यदि (region.pImpl->parentRegion && region.pImpl->parentRegion->pImpl)
        {
            यदि (region.pImpl->parentRegion->pImpl->threadID != region.pImpl->threadID)
                ok &= एतत्->म_लिखो(",parentThread=%d,parent=%lld",
                        (पूर्णाङ्क)region.pImpl->parentRegion->pImpl->threadID,
                        (दीर्घ दीर्घ पूर्णाङ्क)region.pImpl->parentRegion->pImpl->global_region_id);
        }
        ok &= एतत्->म_लिखो("\n");
        फर्काऊ ok;
    }
    तार्किक formatRegionLeave(अचर Region& region, अचर RegionStatistics& result)
    {
        CV_DbgAssert(region.pImpl->endTimestamp - region.pImpl->beginTimestamp == result.duration);
        तार्किक ok = एतत्->म_लिखो("e,%d,%lld,%lld,%lld,%lld",
                (पूर्णाङ्क)region.pImpl->threadID,
                (दीर्घ दीर्घ पूर्णाङ्क)region.pImpl->endTimestamp,
                (दीर्घ दीर्घ पूर्णाङ्क)(*region.pImpl->location.ppExtra)->global_location_id,
                (दीर्घ दीर्घ पूर्णाङ्क)region.pImpl->global_region_id,
                (दीर्घ दीर्घ पूर्णाङ्क)result.duration);
        यदि (result.currentSkippedRegions)
            ok &= एतत्->म_लिखो(",skip=%d", (पूर्णाङ्क)result.currentSkippedRegions);
#ifdef HAVE_IPP
        यदि (result.durationImplIPP)
            ok &= एतत्->म_लिखो(",tIPP=%lld", (दीर्घ दीर्घ पूर्णाङ्क)result.durationImplIPP);
#endif
#ifdef HAVE_OPENCL
        यदि (result.durationImplOpenCL)
            ok &= एतत्->म_लिखो(",tOCL=%lld", (दीर्घ दीर्घ पूर्णाङ्क)result.durationImplOpenCL);
#endif
        ok &= एतत्->म_लिखो("\n");
        फर्काऊ ok;
    }
    तार्किक recordRegionArg(अचर Region& region, अचर TraceArg& arg, अचर वर्ण* value)
    {
        फर्काऊ एतत्->म_लिखो("a,%d,%lld,%lld,\"%s\",\"%s\"\n",
                region.pImpl->threadID,
                (दीर्घ दीर्घ पूर्णाङ्क)region.pImpl->beginTimestamp,
                (दीर्घ दीर्घ पूर्णाङ्क)region.pImpl->global_region_id,
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
    फर्काऊ isEnabled;
}
#endif


Region::LocationExtraData::LocationExtraData(अचर LocationStaticStorage& location)
{
    CV_UNUSED(location);
    स्थिर पूर्णाङ्क g_location_id_counter = 0;
    global_location_id = CV_XADD(&g_location_id_counter, 1) + 1;
    CV_LOG("Register location: " << global_location_id << " (" << (शून्य*)&location << ")"
            << std::endl << "    file: " << location.filename
            << std::endl << "    line: " << location.पङ्क्ति
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

/*static*/ Region::LocationExtraData* Region::LocationExtraData::init(अचर Region::LocationStaticStorage& location)
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
                TraceMessage सन्देश;
                सन्देश.formatlocation(location);
                s->put(सन्देश);
            }
        }
    }
    फर्काऊ *pLocationExtra;
}


Region::Impl::Impl(TraceManagerThreadLocal& ctx, Region* parentRegion_, Region& region_, अचर LocationStaticStorage& location_, int64 beginTimestamp_) :
    location(location_),
    region(region_),
    parentRegion(parentRegion_),
    threadID(ctx.threadID),
    global_region_id(++ctx.region_counter),
    beginTimestamp(beginTimestamp_),
    endTimestamp(0),
    direct_Children_गन्ती(0)
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
        TraceMessage सन्देश;
        सन्देश.formatRegionEnter(region);
        s->put(सन्देश);
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
        TraceMessage सन्देश;
        सन्देश.formatRegionLeave(region, result);
        s->put(सन्देश);
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

Region::Region(अचर LocationStaticStorage& location) :
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
        फर्काऊ;
    }

    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    CV_LOG(_spaces(ctx.getCurrentDepth()*4) << "Region(): " << (शून्य*)एतत् << ": " << location.name);

    Region* parentRegion = ctx.थाक_शीर्ष_Region();
    अचर Region::LocationStaticStorage* parentLocation = ctx.थाक_शीर्ष_Location();

    यदि (location.flags & REGION_FLAG_REGION_NEXT)
    {
        यदि (parentRegion && parentRegion->pImpl)
        {
            CV_DbgAssert((parentRegion->pImpl->location.flags & REGION_FLAG_FUNCTION) == 0);
            parentRegion->destroy(); parentRegion->implFlags = 0;
            parentRegion = ctx.थाक_शीर्ष_Region();
            parentLocation = ctx.थाक_शीर्ष_Location();
        }
    }

    पूर्णाङ्क parentChildren = 0;
    यदि (parentRegion && parentRegion->pImpl)
    {
        यदि (parentLocation == रिक्त)
        {
            // parallel_for_body code path
            parentChildren = CV_XADD(&parentRegion->pImpl->direct_Children_गन्ती, 1) + 1;
        }
        अन्यथा
        {
            parentChildren = ++parentRegion->pImpl->direct_Children_गन्ती;
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
        रोक;
#endif
#ifdef HAVE_OPENCL
    पक्ष REGION_FLAG_IMPL_OPENCL:
        यदि (!ctx.stat_status.ignoreDepthImplOpenCL)
            ctx.stat_status.ignoreDepthImplOpenCL = currentDepth;
        रोक;
#endif
    स्वभाव:
        रोक;
    }

    ctx.थाक_थप(एतत्, &location, beginTimestamp);
    implFlags |= REGION_FLAG_NEED_थाक_झिक;

    यदि ((location.flags & REGION_FLAG_REGION_FORCE) == 0)
    {
        यदि (ctx.stat_status._skipDepth >= 0 && currentDepth > ctx.stat_status._skipDepth)
        {
            CV_LOG(_spaces(ctx.getCurrentDepth()*4) << "Parent region is disabled. Bailout");
            ctx.stat.currentSkippedRegions++;
            फर्काऊ;
        }

        यदि (param_maxRegionChildrenOpenCV > 0 && (location.flags & REGION_FLAG_APP_CODE) == 0 && parentLocation && (parentLocation->flags & REGION_FLAG_APP_CODE) == 0)
        {
            यदि (parentChildren >= param_maxRegionChildrenOpenCV)
            {
                CV_LOG_TRACE_BAILOUT(रिक्त, _spaces(ctx.getCurrentDepth()*4) << "OpenCV parent region exceeds children count. Bailout");
                ctx.stat_status.enableSkipMode(currentDepth - 1);
                ctx.stat.currentSkippedRegions++;
                DEBUG_ONLY(ctx.dump_थाक(std::cout, असत्य));
                फर्काऊ;
            }
        }
        यदि (param_maxRegionChildren > 0 && parentChildren >= param_maxRegionChildren)
        {
            CV_LOG_TRACE_BAILOUT(रिक्त, _spaces(ctx.getCurrentDepth()*4) << "Parent region exceeds children count. Bailout");
            ctx.stat_status.enableSkipMode(currentDepth - 1);
            ctx.stat.currentSkippedRegions++;
            DEBUG_ONLY(ctx.dump_थाक(std::cout, असत्य));
            फर्काऊ;
        }
    }

    LocationExtraData::init(location);

    यदि ((*location.ppExtra)->global_location_id == 0)
    {
        CV_LOG_TRACE_BAILOUT(रिक्त, _spaces(ctx.getCurrentDepth()*4) << "Region location is disabled. Bailout");
        ctx.stat_status.enableSkipMode(currentDepth);
        ctx.stat.currentSkippedRegions++;
        फर्काऊ;
    }

    यदि (parentLocation && (parentLocation->flags & REGION_FLAG_SKIP_NESTED))
    {
        CV_LOG(_spaces(ctx.getCurrentDepth()*4) << "Parent region disables inner regions. Bailout");
        ctx.stat_status.enableSkipMode(currentDepth);
        ctx.stat.currentSkippedRegions++;
        फर्काऊ;
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
                    DEBUG_ONLY(ctx.dump_थाक(std::cout, असत्य));
                }
                ctx.stat_status.enableSkipMode(currentDepth);
                ctx.stat.currentSkippedRegions++;
                फर्काऊ;
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
            pImpl->direct_Children_गन्ती = parentChildren;
        }
    }
}

शून्य Region::destroy()
{
    CV_DbgAssert(implFlags != 0);

    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    CV_LOG(_spaces(ctx.getCurrentDepth()*4) << "Region::destruct(): " << (शून्य*)एतत् << " pImpl=" << pImpl << " implFlags=" << implFlags << ' ' << (ctx.थाक_शीर्ष_Location() ? ctx.थाक_शीर्ष_Location()->name : "<unknown>"));

    CV_DbgAssert(implFlags & REGION_FLAG_NEED_थाक_झिक);
    अचर पूर्णाङ्क currentDepth = ctx.getCurrentDepth(); CV_UNUSED(currentDepth);

    CV_LOG_CTX_STAT(रिक्त, _spaces(currentDepth*4) << ctx.stat << ' ' << ctx.stat_status);

    अचर Region::LocationStaticStorage* location = ctx.थाक_शीर्ष_Location();
    Impl::OptimizationPath myCodePath = Impl::CODE_PATH_PLAIN;
    यदि (location)
    {
        विकल्प (location->flags & REGION_FLAG_IMPL_MASK)
        {
#ifdef HAVE_IPP
        पक्ष REGION_FLAG_IMPL_IPP:
            myCodePath = Impl::CODE_PATH_IPP;
            रोक;
#endif
#ifdef HAVE_OPENCL
        पक्ष REGION_FLAG_IMPL_OPENCL:
            यदि (param_synchronizeOpenCL && cv::ocl::isOpenCLActivated())
                cv::ocl::finish();
            myCodePath = Impl::CODE_PATH_OPENCL;
            रोक;
#endif
        स्वभाव:
            रोक;
        }
    }

    int64 endTimestamp = getTimestampNS();
    int64 duration = endTimestamp - ctx.थाक_शीर्ष_Begin_Timestamp();

    तार्किक active = isActive();

    यदि (active)
        ctx.stat.duration = duration;
    अन्यथा यदि (ctx.थाक.size() == ctx.parallel_for_थाक_size + 1)
        ctx.stat.duration += duration;

    विकल्प (myCodePath) {
        पक्ष Impl::CODE_PATH_PLAIN:
            // nothing
            रोक;
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
            रोक;
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
            रोक;
#endif
        स्वभाव:
            रोक;
    }

    यदि (pImpl)
    {
        CV_DbgAssert((implFlags & (REGION_FLAG__ACTIVE | REGION_FLAG_NEED_थाक_झिक)) == (REGION_FLAG__ACTIVE | REGION_FLAG_NEED_थाक_झिक));
        CV_DbgAssert(ctx.थाक_शीर्ष_Region() == एतत्);
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

    यदि (implFlags & REGION_FLAG_NEED_थाक_झिक)
    {
        CV_DbgAssert(ctx.थाक_शीर्ष_Region() == एतत्);
        ctx.थाक_झिक();
        ctx.stat_status.checkResetSkipMode(currentDepth);
        DEBUG_ONLY(implFlags &= ~REGION_FLAG_NEED_थाक_झिक);
    }
    CV_LOG_CTX_STAT(रिक्त, _spaces(currentDepth*4) << "===> " << ctx.stat << ' ' << ctx.stat_status);
}


TraceManagerThreadLocal::~TraceManagerThreadLocal()
{
}

शून्य TraceManagerThreadLocal::dump_थाक(std::ostream& out, तार्किक onlyFunctions) अचर
{
    std::stringstream ss;
    std::deque<थाक_Entry>::const_iterator it = थाक.begin();
    std::deque<थाक_Entry>::const_iterator end = थाक.end();
    पूर्णाङ्क depth = 0;
    लागि (; it != end; ++it)
    {
        अचर Region::LocationStaticStorage* location = it->location;
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
    अचर std::string name;

    AsyncTraceStorage(अचर std::string& filename) :
        out(filename.c_str(), std::ios::trunc),
        name(filename)
    {
        out << "#description: OpenCV trace file" << std::endl;
        out << "#version: 1.0" << std::endl;
    }
    ~AsyncTraceStorage()
    {
        out.बंद();
    }

    तार्किक put(अचर TraceMessage& सन्देश) अचर CV_OVERRIDE
    {
        यदि (सन्देश.hasError)
            फर्काऊ असत्य;
        out << सन्देश.buffer;
        //DEBUG_ONLY(std::flush(out)); // TODO configure flag
        फर्काऊ सत्य;
    }
};

वर्ग SyncTraceStorage CV_FINAL : सार्वजनिक TraceStorage
{
    mutable std::ofstream out;
    mutable cv::Mutex mutex;
सार्वजनिक:
    अचर std::string name;

    SyncTraceStorage(अचर std::string& filename) :
        out(filename.c_str(), std::ios::trunc),
        name(filename)
    {
        out << "#description: OpenCV trace file" << std::endl;
        out << "#version: 1.0" << std::endl;
    }
    ~SyncTraceStorage()
    {
        cv::AutoLock l(mutex);
        out.बंद();
    }

    तार्किक put(अचर TraceMessage& सन्देश) अचर CV_OVERRIDE
    {
        यदि (सन्देश.hasError)
            फर्काऊ असत्य;
        {
            cv::AutoLock l(mutex);
            out << सन्देश.buffer;
            std::flush(out); // TODO configure flag
        }
        फर्काऊ सत्य;
    }
};


TraceStorage* TraceManagerThreadLocal::getStorage() अचर
{
    // TODO configuration option for stdout/single trace file
    यदि (storage.empty())
    {
        TraceStorage* global = getTraceManager().trace_storage.get();
        यदि (global)
        {
            अचर std::string filepath = cv::format("%s-%03d.txt", getParameterTraceLocation().c_str(), threadID).c_str();
            TraceMessage सन्देश;
            अचर वर्ण* pos = म_खोजप(filepath.c_str(), '/'); // extract filename
#ifdef _WIN32
            यदि (!pos)
                pos = म_खोजप(filepath.c_str(), '\\');
#endif
            यदि (!pos)
                pos = filepath.c_str();
            अन्यथा
                pos += 1; // fix to skip extra slash in filename beginning
            सन्देश.म_लिखो("#thread file: %s\n", pos);
            global->put(सन्देश);
            storage.reset(नूतन AsyncTraceStorage(filepath));
        }
    }
    फर्काऊ storage.get();
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
    माप_प्रकार totalEvents = 0, totalSkippedEvents = 0;
    लागि (माप_प्रकार i = 0; i < threads_ctx.size(); i++)
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
        फर्काऊ असत्य;
    }

    यदि (!isInitialized)
    {
        TraceManager& m = getTraceManager();
        CV_UNUSED(m); // TODO
    }

    फर्काऊ activated;
}


स्थिर TraceManager* getTraceManagerCallOnce()
{
    स्थिर TraceManager globalInstance;
    फर्काऊ &globalInstance;
}
TraceManager& getTraceManager()
{
    CV_SINGLETON_LAZY_INIT_REF(TraceManager, getTraceManagerCallOnce())
}

शून्य parallel_For_Set_जरा_Region(अचर Region& जरा_Region, अचर TraceManagerThreadLocal& जरा_ctx)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();

    यदि (ctx.dummy_थाक_शीर्ष.region == &जरा_Region) // already attached
        फर्काऊ;

    CV_Assert(ctx.dummy_थाक_शीर्ष.region == रिक्त);
    ctx.dummy_थाक_शीर्ष = TraceManagerThreadLocal::थाक_Entry(const_cast<Region*>(&जरा_Region), रिक्त, -1);

    यदि (&ctx == &जरा_ctx)
    {
        ctx.stat.grab(ctx.parallel_for_stat);
        ctx.parallel_for_stat_status = ctx.stat_status;
        ctx.parallel_for_थाक_size = ctx.थाक.size();
        फर्काऊ;
    }

    CV_Assert(ctx.थाक.empty());

    ctx.currentActiveRegion = const_cast<Region*>(&जरा_Region);

    ctx.regionDepth = जरा_ctx.regionDepth;
    ctx.regionDepthOpenCV = जरा_ctx.regionDepthOpenCV;

    ctx.parallel_for_थाक_size = 0;

    ctx.stat_status.propagateFrom(जरा_ctx.stat_status);
}

शून्य parallelForAttachNestedRegion(अचर Region& जरा_Region)
{
    CV_UNUSED(जरा_Region);
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();

    CV_DbgAssert(ctx.dummy_थाक_शीर्ष.region == &जरा_Region);

    Region* region = ctx.getCurrentActiveRegion();
    CV_LOG_PARALLEL(रिक्त, " PARALLEL_FOR: " << (शून्य*)region << " ==> " << &जरा_Region);
    यदि (!region)
        फर्काऊ;

#ifdef OPENCV_WITH_ITT
    यदि (!जरा_Region.pImpl || !जरा_Region.pImpl->itt_id_registered)
        फर्काऊ;

    यदि (!region->pImpl)
        फर्काऊ;

    CV_LOG_PARALLEL(रिक्त, " PARALLEL_FOR ITT: " << (शून्य*)जरा_Region.pImpl->itt_id.d1 << ":" << जरा_Region.pImpl->itt_id.d2 << ":" << (शून्य*)जरा_Region.pImpl->itt_id.d3 << " => "
                                 << (शून्य*)region->pImpl->itt_id.d1 << ":" << region->pImpl->itt_id.d2 << ":" << (शून्य*)region->pImpl->itt_id.d3);
    __itt_relation_add(domain, region->pImpl->itt_id, __itt_relation_is_child_of, जरा_Region.pImpl->itt_id);
#endif
}

शून्य parallelForFinalize(अचर Region& जरा_Region)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();

    int64 endTimestamp = getTimestampNS();
    int64 duration = endTimestamp - ctx.थाक_शीर्ष_Begin_Timestamp();
    CV_LOG_PARALLEL(रिक्त, "parallel_for duration: " << duration << " " << &जरा_Region);

    std::vector<TraceManagerThreadLocal*> threads_ctx;
    getTraceManager().tls.gather(threads_ctx);
    RegionStatistics parallel_for_stat;
    लागि (माप_प्रकार i = 0; i < threads_ctx.size(); i++)
    {
        TraceManagerThreadLocal* child_ctx = threads_ctx[i];

        यदि (child_ctx && child_ctx->थाक_शीर्ष_Region() == &जरा_Region)
        {
            CV_LOG_PARALLEL(रिक्त, "Thread=" << child_ctx->threadID << " " << child_ctx->stat);
            RegionStatistics child_stat;
            child_ctx->stat.grab(child_stat);
            parallel_for_stat.append(child_stat);
            यदि (child_ctx != &ctx)
            {
                child_ctx->dummy_थाक_शीर्ष = TraceManagerThreadLocal::थाक_Entry();
            }
            अन्यथा
            {
                ctx.parallel_for_stat.grab(ctx.stat);
                ctx.stat_status = ctx.parallel_for_stat_status;
                child_ctx->dummy_थाक_शीर्ष = TraceManagerThreadLocal::थाक_Entry();
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
    ExtraData(TraceManagerThreadLocal& ctx, अचर TraceArg& arg)
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

स्थिर शून्य initTraceArg(TraceManagerThreadLocal& ctx, अचर TraceArg& arg)
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
शून्य traceArg(अचर TraceArg& arg, अचर वर्ण* value)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    Region* region = ctx.getCurrentActiveRegion();
    यदि (!region)
        फर्काऊ;
    CV_Assert(region->pImpl);
    initTraceArg(ctx, arg);
    यदि (!value)
        value = "<null>";
#ifdef OPENCV_WITH_ITT
    यदि (isITTEnabled())
    {
        __itt_metadata_str_add(domain, region->pImpl->itt_id, (*arg.ppExtra)->ittHandle_name, value, म_माप(value));
    }
#endif
}
शून्य traceArg(अचर TraceArg& arg, पूर्णाङ्क value)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    Region* region = ctx.getCurrentActiveRegion();
    यदि (!region)
        फर्काऊ;
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
शून्य traceArg(अचर TraceArg& arg, int64 value)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    Region* region = ctx.getCurrentActiveRegion();
    यदि (!region)
        फर्काऊ;
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
शून्य traceArg(अचर TraceArg& arg, द्विगुण value)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    Region* region = ctx.getCurrentActiveRegion();
    यदि (!region)
        फर्काऊ;
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

Region::Region(अचर LocationStaticStorage&) : pImpl(रिक्त), implFlags(0) {}
शून्य Region::destroy() {}

शून्य traceArg(अचर TraceArg&, अचर वर्ण*) {}
शून्य traceArg(अचर TraceArg&, पूर्णाङ्क) {};
शून्य traceArg(अचर TraceArg&, int64) {};
शून्य traceArg(अचर TraceArg&, द्विगुण) {};

#endif

}}}} // namespace
