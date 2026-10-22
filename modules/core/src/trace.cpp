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
#परिभाषय CV_LOG(...) CV_LOG_INFO(रिक्त, __VA_ARGS__)
#अन्यथा
#परिभाषय CV_LOG(...) {}
#यदि_अन्तः

#यदि 0
#परिभाषय CV_LOG_ITT(...) CV_LOG_INFO(रिक्त, __VA_ARGS__)
#अन्यथा
#परिभाषय CV_LOG_ITT(...) {}
#यदि_अन्तः

#यदि 1
#परिभाषय CV_LOG_TRACE_BAILOUT(चिह्नकम्, ...) CV_LOG_INFO(चिह्नकम्, __VA_ARGS__)
#अन्यथा
#परिभाषय CV_LOG_TRACE_BAILOUT(...) {}
#यदि_अन्तः

#यदि 0
#परिभाषय CV_LOG_PARALLEL(चिह्नकम्, ...) CV_LOG_INFO(चिह्नकम्, __VA_ARGS__)
#अन्यथा
#परिभाषय CV_LOG_PARALLEL(...) {}
#यदि_अन्तः

#यदि 0
#परिभाषय CV_LOG_CTX_STAT(चिह्नकम्, ...) CV_LOG_INFO(चिह्नकम्, __VA_ARGS__)
#अन्यथा
#परिभाषय CV_LOG_CTX_STAT(...) {}
#यदि_अन्तः

#यदि 0
#परिभाषय CV_LOG_SKIP(चिह्नकम्, ...) CV_LOG_INFO(चिह्नकम्, __VA_ARGS__)
#अन्यथा
#परिभाषय CV_LOG_SKIP(...) {}
#यदि_अन्तः

नामाकाश cv {
नामाकाश utils {
नामाकाश trace {
नामाकाश details {

#यदि_परिभाषितम् OPENCV_TRACE

#यदि_परिभाषितम् _MSC_VER
#निर्देशः warning(disable:4065) // switch statement contains 'default' but no 'case' labels
#यदि_अन्तः

स्थिर तार्किक getParameterTraceEnable()
{
    स्थिर तार्किक param_traceEnable = utils::getConfigurationParameterBool("OPENCV_TRACE", असत्य);
    प्रत्यागम param_traceEnable;
}

// TODO lazy configuration flags
स्थिर पूर्णाङ्क param_max_Region_गभीरता_Open_CV = (पूर्णाङ्क)utils::get_Configuration_Parameter_आकार_T("OPENCV_TRACE_DEPTH_OPENCV", 1);
स्थिर पूर्णाङ्क param_maxRegionChildrenOpenCV = (पूर्णाङ्क)utils::get_Configuration_Parameter_आकार_T("OPENCV_TRACE_MAX_CHILDREN_OPENCV", 1000);
स्थिर पूर्णाङ्क param_maxRegionChildren = (पूर्णाङ्क)utils::get_Configuration_Parameter_आकार_T("OPENCV_TRACE_MAX_CHILDREN", 10000);

स्थिर ध्रुव cv::String& getParameterTraceLocation()
{
    स्थिर cv::String param_traceLocation = utils::getConfigurationParameterString("OPENCV_TRACE_LOCATION", "OpenCVTrace");
    प्रत्यागम param_traceLocation;
}

#यदि_परिभाषितम् HAVE_OPENCL
स्थिर तार्किक param_synchronizeOpenCL = utils::getConfigurationParameterBool("OPENCV_TRACE_SYNC_OPENCL", असत्य);
#यदि_अन्तः

#यदि_परिभाषितम् OPENCV_WITH_ITT
स्थिर तार्किक param_ITT_पञ्जिकाकोशः_Parent_Scope = utils::getConfigurationParameterBool("OPENCV_TRACE_ITT_PARENT", असत्य);
#यदि_अन्तः

स्थिर ध्रुव वर्ण* _spaces(पूर्णाङ्क गणनम्_2)
{
    स्थिर ध्रुव वर्ण buf[64] =
"                                                               ";
    प्रत्यागम &buf[63 - (गणनम्_2 & 63)];
}

/**
 * Text-based trace messages
 */
वर्ग Trace_सन्देशः
{
सार्वजनिक:
    वर्ण बफर[1024];
    size_t len;
    तार्किक hasError;

    Trace_सन्देशः() :
        len(0),
        hasError(असत्य)
    {}

    तार्किक प्रारूप_लिख(ध्रुव वर्ण* format, ...)
    {
        वर्ण* buf = &बफर[len];
        size_t sz = परिमाण(बफर) - len;
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
        प्रत्यागम एतत्->प्रारूप_लिख("l,%lld,\"%s\",%d,\"%s\",0x%llX\n",
                (दीर्घ दीर्घ पूर्णाङ्क)(*location.ppExtra)->global_location_id,
                location.filename,
                location.पङ्क्तिः,
                location.name,
                (दीर्घ दीर्घ पूर्णाङ्क)(location.flags & ~0xF0000000));
    }
    तार्किक formatRegionEnter(ध्रुव Region& region)
    {
        तार्किक ok = एतत्->प्रारूप_लिख("b,%d,%lld,%lld,%lld",
                (पूर्णाङ्क)region.pImpl->threadID,
                (दीर्घ दीर्घ पूर्णाङ्क)region.pImpl->beginTimestamp,
                (दीर्घ दीर्घ पूर्णाङ्क)((*region.pImpl->location.ppExtra)->global_location_id),
                (दीर्घ दीर्घ पूर्णाङ्क)region.pImpl->global_region_id);
        यदि (region.pImpl->parentRegion && region.pImpl->parentRegion->pImpl)
        {
            यदि (region.pImpl->parentRegion->pImpl->threadID != region.pImpl->threadID)
                ok &= एतत्->प्रारूप_लिख(",parentThread=%d,parent=%lld",
                        (पूर्णाङ्क)region.pImpl->parentRegion->pImpl->threadID,
                        (दीर्घ दीर्घ पूर्णाङ्क)region.pImpl->parentRegion->pImpl->global_region_id);
        }
        ok &= एतत्->प्रारूप_लिख("\n");
        प्रत्यागम ok;
    }
    तार्किक formatRegionLeave(ध्रुव Region& region, ध्रुव RegionStatistics& परिणाम)
    {
        CV_DbgAssert(region.pImpl->अन्तः_Timestamp - region.pImpl->beginTimestamp == परिणाम.duration);
        तार्किक ok = एतत्->प्रारूप_लिख("e,%d,%lld,%lld,%lld,%lld",
                (पूर्णाङ्क)region.pImpl->threadID,
                (दीर्घ दीर्घ पूर्णाङ्क)region.pImpl->अन्तः_Timestamp,
                (दीर्घ दीर्घ पूर्णाङ्क)(*region.pImpl->location.ppExtra)->global_location_id,
                (दीर्घ दीर्घ पूर्णाङ्क)region.pImpl->global_region_id,
                (दीर्घ दीर्घ पूर्णाङ्क)परिणाम.duration);
        यदि (परिणाम.currentSkippedRegions)
            ok &= एतत्->प्रारूप_लिख(",skip=%d", (पूर्णाङ्क)परिणाम.currentSkippedRegions);
#यदि_परिभाषितम् HAVE_IPP
        यदि (परिणाम.durationImplIPP)
            ok &= एतत्->प्रारूप_लिख(",tIPP=%lld", (दीर्घ दीर्घ पूर्णाङ्क)परिणाम.durationImplIPP);
#यदि_अन्तः
#यदि_परिभाषितम् HAVE_OPENCL
        यदि (परिणाम.durationImplOpenCL)
            ok &= एतत्->प्रारूप_लिख(",tOCL=%lld", (दीर्घ दीर्घ पूर्णाङ्क)परिणाम.durationImplOpenCL);
#यदि_अन्तः
        ok &= एतत्->प्रारूप_लिख("\n");
        प्रत्यागम ok;
    }
    तार्किक लेखः_Region_Arg(ध्रुव Region& region, ध्रुव TraceArg& arg, ध्रुव वर्ण* value)
    {
        प्रत्यागम एतत्->प्रारूप_लिख("a,%d,%lld,%lld,\"%s\",\"%s\"\n",
                region.pImpl->threadID,
                (दीर्घ दीर्घ पूर्णाङ्क)region.pImpl->beginTimestamp,
                (दीर्घ दीर्घ पूर्णाङ्क)region.pImpl->global_region_id,
                arg.name,
                value);
    }
};


#यदि_परिभाषितम् OPENCV_WITH_ITT
स्थिर __itt_domain* domain = रिक्त;

स्थिर तार्किक isITTEnabled()
{
    स्थिर चञ्चल तार्किक isInitialized = असत्य;
    स्थिर तार्किक isEnabled = असत्य;
    यदि (!isInitialized)
    {
        cv::Auto_तालकम् lock(cv::getInitializationMutex());
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
#यदि_अन्तः


Region::Location_Extra_दत्त::Location_Extra_दत्त(ध्रुव LocationStaticStorage& location)
{
    CV_अप्रयुक्तम्(location);
    स्थिर पूर्णाङ्क g_location_id_गणक = 0;
    global_location_id = CV_XADD(&g_location_id_गणक, 1) + 1;
    CV_LOG("Register location: " << global_location_id << " (" << (शून्य*)&location << ")"
            << std::पङ्क्त्यन्तः << "    file: " << location.filename
            << std::पङ्क्त्यन्तः << "    line: " << location.पङ्क्तिः
            << std::पङ्क्त्यन्तः << "    name: " << location.name);
#यदि_परिभाषितम् OPENCV_WITH_ITT
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
#यदि_अन्तः
}

/*static*/ Region::Location_Extra_दत्त* Region::Location_Extra_दत्त::init(ध्रुव Region::LocationStaticStorage& location)
{
    Location_Extra_दत्त** pLocationExtra = location.ppExtra;
    CV_DbgAssert(pLocationExtra);
    यदि (*pLocationExtra == रिक्त)
    {
        cv::Auto_तालकम् lock(cv::getInitializationMutex());
        यदि (*pLocationExtra == रिक्त)
        {
            *pLocationExtra = नूतन Region::Location_Extra_दत्त(location);
            TraceStorage* s = getTraceManager().trace_storage.get();
            यदि (s)
            {
                Trace_सन्देशः सन्देशः;
                सन्देशः.formatlocation(location);
                s->put(सन्देशः);
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
    global_region_id(++ctx.region_गणक),
    beginTimestamp(beginTimestamp_),
    अन्तः_Timestamp(0),
    प्रत्यक्षम्_Children_गणनम्(0)
#यदि_परिभाषितम् OPENCV_WITH_ITT
    ,itt_id_registered(असत्य)
    ,itt_id(__itt_null)
#यदि_अन्तः
{
    CV_DbgAssert(ctx.current_सक्रिय_Region == parentRegion);
    region.pImpl = एतत्;

    पञ्जिकाकोशः_Region(ctx);

    enterRegion(ctx);
}

Region::Impl::~Impl()
{
#यदि_परिभाषितम् OPENCV_WITH_ITT
    यदि (itt_id_registered)
    {
        CV_LOG_ITT(" Destroy ITT region: I=" << (शून्य*)एतत्);
        __itt_id_destroy(domain, itt_id);
        itt_id_registered = असत्य;
    }
#यदि_अन्तः
    region.pImpl = रिक्त;
}

शून्य Region::Impl::enterRegion(TraceManagerThreadLocal& ctx)
{
    ctx.current_सक्रिय_Region = &region;

    यदि (location.flags & REGION_FLAG_FUNCTION)
    {
        यदि ((location.flags & REGION_FLAG_APP_संहिता) == 0)
        {
            ctx.region_गभीरता_Open_CV++;
        }
        ctx.region_गभीरता++;
    }

    TraceStorage* s = ctx.getStorage();
    यदि (s)
    {
        Trace_सन्देशः सन्देशः;
        सन्देशः.formatRegionEnter(region);
        s->put(सन्देशः);
    }
#यदि_परिभाषितम् OPENCV_WITH_ITT
    यदि (isITTEnabled())
    {
        __itt_id parentID = __itt_null;
        यदि (param_ITT_पञ्जिकाकोशः_Parent_Scope && parentRegion && parentRegion->pImpl && parentRegion->pImpl->itt_id_registered && (location.flags & REGION_FLAG_REGION_FORCE) == 0)
            parentID = parentRegion->pImpl->itt_id;
        __itt_task_begin(domain, itt_id, parentID, (*location.ppExtra)->ittHandle_name);
    }
#यदि_अन्तः
}

शून्य Region::Impl::leaveRegion(TraceManagerThreadLocal& ctx)
{
    int64 duration = अन्तः_Timestamp - beginTimestamp; CV_अप्रयुक्तम्(duration);
    RegionStatistics परिणाम;
    ctx.stat.grab(परिणाम);
    ctx.योग_Skipped_Events += परिणाम.currentSkippedRegions;
    CV_LOG(_spaces(ctx.get_Current_गभीरता()*4) << "leaveRegion(): " << (शून्य*)एतत् << " " << परिणाम);
#यदि_परिभाषितम् OPENCV_WITH_ITT
    यदि (isITTEnabled())
    {
        यदि (परिणाम.currentSkippedRegions)
        {
            __itt_metadata_add(domain, itt_id, __itt_string_handle_create("skipped trace entries"), __itt_metadata_u32, 1, &परिणाम.currentSkippedRegions);
        }
#यदि_परिभाषितम् HAVE_IPP
        यदि (परिणाम.durationImplIPP)
            __itt_metadata_add(domain, itt_id, __itt_string_handle_create("tIPP"), __itt_metadata_u64, 1, &परिणाम.durationImplIPP);
#यदि_अन्तः
#यदि_परिभाषितम् HAVE_OPENCL
        यदि (परिणाम.durationImplOpenCL)
            __itt_metadata_add(domain, itt_id, __itt_string_handle_create("tOpenCL"), __itt_metadata_u64, 1, &परिणाम.durationImplOpenCL);
#यदि_अन्तः
        __itt_task_end(domain);
    }
#यदि_अन्तः
    TraceStorage* s = ctx.getStorage();
    यदि (s)
    {
        Trace_सन्देशः सन्देशः;
        सन्देशः.formatRegionLeave(region, परिणाम);
        s->put(सन्देशः);
    }

    यदि (location.flags & REGION_FLAG_FUNCTION)
    {
        यदि ((location.flags & REGION_FLAG_APP_संहिता) == 0)
        {
            ctx.region_गभीरता_Open_CV--;
        }
        ctx.region_गभीरता--;
    }

    ctx.current_सक्रिय_Region = parentRegion;
}

शून्य Region::Impl::release()
{
    विलोपय एतत्;
}

शून्य Region::Impl::पञ्जिकाकोशः_Region(TraceManagerThreadLocal& ctx)
{
#यदि_परिभाषितम् OPENCV_WITH_ITT
    यदि (isITTEnabled())
    {
        यदि (!itt_id_registered)
        {
            CV_LOG_ITT(" Register ITT region: I=" << (शून्य*)एतत् << " " << ctx.threadID << "-" << global_region_id);
#यदि 1 // workaround for some ITT backends
            itt_id = __itt_id_make((शून्य*)(intptr_t)(((int64)(ctx.threadID + 1) << 32) | global_region_id), global_region_id);
#अन्यथा
            itt_id = __itt_id_make((शून्य*)(intptr_t)(ctx.threadID + 1), global_region_id);
#यदि_अन्तः
            __itt_id_create(domain, itt_id);
            itt_id_registered = सत्य;
        }
    }
#अन्यथा
    CV_अप्रयुक्तम्(ctx);
#यदि_अन्तः
}

शून्य Region_Statistics_स्थितिः::enable_Skip_बहुलकम्(पूर्णाङ्क गभीरता)
{
    CV_DbgAssert(skip_गभीरता < 0);
    CV_LOG_SKIP(रिक्त, "SKIP-ENABLE: depth=" << गभीरता);
    skip_गभीरता = गभीरता;
}
शून्य Region_Statistics_स्थितिः::check_Reset_Skip_बहुलकम्(पूर्णाङ्क leave_गभीरता)
{
    यदि (leave_गभीरता <= skip_गभीरता)
    {
        CV_LOG_SKIP(रिक्त, "SKIP-RESET: leaveDepth=" << leave_गभीरता << " skipDepth=" << skip_गभीरता);
        skip_गभीरता = -1;
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
    CV_LOG(_spaces(ctx.get_Current_गभीरता()*4) << "Region(): " << (शून्य*)एतत् << ": " << location.name);

    Region* parentRegion = ctx.राशिः_शिखरम्_Region();
    ध्रुव Region::LocationStaticStorage* parentLocation = ctx.राशिः_शिखरम्_Location();

    यदि (location.flags & REGION_FLAG_REGION_अग्र)
    {
        यदि (parentRegion && parentRegion->pImpl)
        {
            CV_DbgAssert((parentRegion->pImpl->location.flags & REGION_FLAG_FUNCTION) == 0);
            parentRegion->destroy(); parentRegion->implFlags = 0;
            parentRegion = ctx.राशिः_शिखरम्_Region();
            parentLocation = ctx.राशिः_शिखरम्_Location();
        }
    }

    पूर्णाङ्क parentChildren = 0;
    यदि (parentRegion && parentRegion->pImpl)
    {
        यदि (parentLocation == रिक्त)
        {
            // parallel_for_body code path
            parentChildren = CV_XADD(&parentRegion->pImpl->प्रत्यक्षम्_Children_गणनम्, 1) + 1;
        }
        अन्यथा
        {
            parentChildren = ++parentRegion->pImpl->प्रत्यक्षम्_Children_गणनम्;
        }
    }

    int64 beginTimestamp = getTimestampNS();

    पूर्णाङ्क current_गभीरता = ctx.get_Current_गभीरता() + 1;
    विकल्प (location.flags & REGION_FLAG_IMPL_आवरणम्)
    {
#यदि_परिभाषितम् HAVE_IPP
    पक्ष REGION_FLAG_IMPL_IPP:
        यदि (!ctx.stat_स्थितिः.ignore_गभीरता_Impl_IPP)
            ctx.stat_स्थितिः.ignore_गभीरता_Impl_IPP = current_गभीरता;
        भङ्ग;
#यदि_अन्तः
#यदि_परिभाषितम् HAVE_OPENCL
    पक्ष REGION_FLAG_IMPL_OPENCL:
        यदि (!ctx.stat_स्थितिः.ignore_गभीरता_Impl_Open_CL)
            ctx.stat_स्थितिः.ignore_गभीरता_Impl_Open_CL = current_गभीरता;
        भङ्ग;
#यदि_अन्तः
    स्वभाव:
        भङ्ग;
    }

    ctx.राशिः_प्रक्षिप(एतत्, &location, beginTimestamp);
    implFlags |= REGION_FLAG_NEED_राशिः_निष्कासय;

    यदि ((location.flags & REGION_FLAG_REGION_FORCE) == 0)
    {
        यदि (ctx.stat_स्थितिः.skip_गभीरता >= 0 && current_गभीरता > ctx.stat_स्थितिः.skip_गभीरता)
        {
            CV_LOG(_spaces(ctx.get_Current_गभीरता()*4) << "Parent region is disabled. Bailout");
            ctx.stat.currentSkippedRegions++;
            प्रत्यागम;
        }

        यदि (param_maxRegionChildrenOpenCV > 0 && (location.flags & REGION_FLAG_APP_संहिता) == 0 && parentLocation && (parentLocation->flags & REGION_FLAG_APP_संहिता) == 0)
        {
            यदि (parentChildren >= param_maxRegionChildrenOpenCV)
            {
                CV_LOG_TRACE_BAILOUT(रिक्त, _spaces(ctx.get_Current_गभीरता()*4) << "OpenCV parent region exceeds children count. Bailout");
                ctx.stat_स्थितिः.enable_Skip_बहुलकम्(current_गभीरता - 1);
                ctx.stat.currentSkippedRegions++;
                DEBUG_ONLY(ctx.dump_राशिः(std::निर्गमः, असत्य));
                प्रत्यागम;
            }
        }
        यदि (param_maxRegionChildren > 0 && parentChildren >= param_maxRegionChildren)
        {
            CV_LOG_TRACE_BAILOUT(रिक्त, _spaces(ctx.get_Current_गभीरता()*4) << "Parent region exceeds children count. Bailout");
            ctx.stat_स्थितिः.enable_Skip_बहुलकम्(current_गभीरता - 1);
            ctx.stat.currentSkippedRegions++;
            DEBUG_ONLY(ctx.dump_राशिः(std::निर्गमः, असत्य));
            प्रत्यागम;
        }
    }

    Location_Extra_दत्त::init(location);

    यदि ((*location.ppExtra)->global_location_id == 0)
    {
        CV_LOG_TRACE_BAILOUT(रिक्त, _spaces(ctx.get_Current_गभीरता()*4) << "Region location is disabled. Bailout");
        ctx.stat_स्थितिः.enable_Skip_बहुलकम्(current_गभीरता);
        ctx.stat.currentSkippedRegions++;
        प्रत्यागम;
    }

    यदि (parentLocation && (parentLocation->flags & REGION_FLAG_SKIP_NESTED))
    {
        CV_LOG(_spaces(ctx.get_Current_गभीरता()*4) << "Parent region disables inner regions. Bailout");
        ctx.stat_स्थितिः.enable_Skip_बहुलकम्(current_गभीरता);
        ctx.stat.currentSkippedRegions++;
        प्रत्यागम;
    }

    यदि (param_max_Region_गभीरता_Open_CV)
    {
        यदि ((location.flags & REGION_FLAG_APP_संहिता) == 0)
        {
            यदि (ctx.region_गभीरता_Open_CV >= param_max_Region_गभीरता_Open_CV)
            {
                CV_LOG(_spaces(ctx.get_Current_गभीरता()*4) << "OpenCV region depth is exceed = " << ctx.region_गभीरता_Open_CV << ". Bailout");
                यदि (ctx.stat.currentSkippedRegions == 0)
                {
                    DEBUG_ONLY(ctx.dump_राशिः(std::निर्गमः, असत्य));
                }
                ctx.stat_स्थितिः.enable_Skip_बहुलकम्(current_गभीरता);
                ctx.stat.currentSkippedRegions++;
                प्रत्यागम;
            }
        }
    }

    नूतन Impl(ctx, parentRegion, *एतत्, location, beginTimestamp);
    CV_DbgAssert(pImpl != रिक्त);
    implFlags |= REGION_FLAG_सक्रिय;

    // parallel_for path
    यदि (parentRegion && parentRegion->pImpl)
    {
        यदि (parentLocation == रिक्त)
        {
            pImpl->प्रत्यक्षम्_Children_गणनम् = parentChildren;
        }
    }
}

शून्य Region::destroy()
{
    CV_DbgAssert(implFlags != 0);

    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    CV_LOG(_spaces(ctx.get_Current_गभीरता()*4) << "Region::destruct(): " << (शून्य*)एतत् << " pImpl=" << pImpl << " implFlags=" << implFlags << ' ' << (ctx.राशिः_शिखरम्_Location() ? ctx.राशिः_शिखरम्_Location()->name : "<unknown>"));

    CV_DbgAssert(implFlags & REGION_FLAG_NEED_राशिः_निष्कासय);
    ध्रुव पूर्णाङ्क current_गभीरता = ctx.get_Current_गभीरता(); CV_अप्रयुक्तम्(current_गभीरता);

    CV_LOG_CTX_STAT(रिक्त, _spaces(current_गभीरता*4) << ctx.stat << ' ' << ctx.stat_स्थितिः);

    ध्रुव Region::LocationStaticStorage* location = ctx.राशिः_शिखरम्_Location();
    Impl::Optimization_मार्गः my_संहिता_मार्गः = Impl::संहिता_मार्गः_PLAIN;
    यदि (location)
    {
        विकल्प (location->flags & REGION_FLAG_IMPL_आवरणम्)
        {
#यदि_परिभाषितम् HAVE_IPP
        पक्ष REGION_FLAG_IMPL_IPP:
            my_संहिता_मार्गः = Impl::संहिता_मार्गः_IPP;
            भङ्ग;
#यदि_अन्तः
#यदि_परिभाषितम् HAVE_OPENCL
        पक्ष REGION_FLAG_IMPL_OPENCL:
            यदि (param_synchronizeOpenCL && cv::ocl::isOpenCLActivated())
                cv::ocl::finish();
            my_संहिता_मार्गः = Impl::संहिता_मार्गः_OPENCL;
            भङ्ग;
#यदि_अन्तः
        स्वभाव:
            भङ्ग;
        }
    }

    int64 अन्तः_Timestamp = getTimestampNS();
    int64 duration = अन्तः_Timestamp - ctx.राशिः_शिखरम्_Begin_Timestamp();

    तार्किक सक्रिय = is_सक्रिय_2();

    यदि (सक्रिय)
        ctx.stat.duration = duration;
    अन्यथा यदि (ctx.राशिः.size() == ctx.parallel_for_राशिः_आकार + 1)
        ctx.stat.duration += duration;

    विकल्प (my_संहिता_मार्गः) {
        पक्ष Impl::संहिता_मार्गः_PLAIN:
            // nothing
            भङ्ग;
#यदि_परिभाषितम् HAVE_IPP
        पक्ष Impl::संहिता_मार्गः_IPP:
            यदि (ctx.stat_स्थितिः.ignore_गभीरता_Impl_IPP == current_गभीरता)
            {
                ctx.stat.durationImplIPP += duration;
                ctx.stat_स्थितिः.ignore_गभीरता_Impl_IPP = 0;
            }
            अन्यथा यदि (सक्रिय)
            {
                ctx.stat.durationImplIPP = duration;
            }
            भङ्ग;
#यदि_अन्तः
#यदि_परिभाषितम् HAVE_OPENCL
        पक्ष Impl::संहिता_मार्गः_OPENCL:
            यदि (ctx.stat_स्थितिः.ignore_गभीरता_Impl_Open_CL == current_गभीरता)
            {
                ctx.stat.durationImplOpenCL += duration;
                ctx.stat_स्थितिः.ignore_गभीरता_Impl_Open_CL = 0;
            }
            अन्यथा यदि (सक्रिय)
            {
                ctx.stat.durationImplOpenCL = duration;
            }
            भङ्ग;
#यदि_अन्तः
        स्वभाव:
            भङ्ग;
    }

    यदि (pImpl)
    {
        CV_DbgAssert((implFlags & (REGION_FLAG_सक्रिय | REGION_FLAG_NEED_राशिः_निष्कासय)) == (REGION_FLAG_सक्रिय | REGION_FLAG_NEED_राशिः_निष्कासय));
        CV_DbgAssert(ctx.राशिः_शिखरम्_Region() == एतत्);
        pImpl->अन्तः_Timestamp = अन्तः_Timestamp;
        pImpl->leaveRegion(ctx);
        pImpl->release();
        pImpl = रिक्त;
        DEBUG_ONLY(implFlags &= ~REGION_FLAG_सक्रिय);
    }
    अन्यथा
    {
        CV_DbgAssert(ctx.stat_स्थितिः.skip_गभीरता <= current_गभीरता);
    }

    यदि (implFlags & REGION_FLAG_NEED_राशिः_निष्कासय)
    {
        CV_DbgAssert(ctx.राशिः_शिखरम्_Region() == एतत्);
        ctx.राशिः_निष्कासय();
        ctx.stat_स्थितिः.check_Reset_Skip_बहुलकम्(current_गभीरता);
        DEBUG_ONLY(implFlags &= ~REGION_FLAG_NEED_राशिः_निष्कासय);
    }
    CV_LOG_CTX_STAT(रिक्त, _spaces(current_गभीरता*4) << "===> " << ctx.stat << ' ' << ctx.stat_स्थितिः);
}


TraceManagerThreadLocal::~TraceManagerThreadLocal()
{
}

शून्य TraceManagerThreadLocal::dump_राशिः(std::ostream& out, तार्किक onlyFunctions) ध्रुव
{
    std::stringstream ss;
    std::deque<राशिः_प्रविष्टिः>::const_iterator it = राशिः.begin();
    std::deque<राशिः_प्रविष्टिः>::const_iterator end = राशिः.end();
    पूर्णाङ्क गभीरता = 0;
    प्रति (; it != end; ++it)
    {
        ध्रुव Region::LocationStaticStorage* location = it->location;
        यदि (location)
        {
            यदि (!onlyFunctions || (location->flags & REGION_FLAG_FUNCTION))
            {
                ss << _spaces(4*गभीरता) << location->name << std::पङ्क्त्यन्तः;
                गभीरता++;
            }
        }
        अन्यथा
        {
            ss << _spaces(4*गभीरता) << "<unknown>" << std::पङ्क्त्यन्तः;
            गभीरता++;
        }
    }
    out << ss.str();
}

वर्ग AsyncTraceStorage CV_FINAL : सार्वजनिक TraceStorage
{
    परिवर्तनीय std::ofstream out;
सार्वजनिक:
    ध्रुव std::string name;

    AsyncTraceStorage(ध्रुव std::string& filename) :
        out(filename.c_str(), std::ios::trunc),
        name(filename)
    {
        out << "#description: OpenCV trace file" << std::पङ्क्त्यन्तः;
        out << "#version: 1.0" << std::पङ्क्त्यन्तः;
    }
    ~AsyncTraceStorage()
    {
        out.close();
    }

    तार्किक put(ध्रुव Trace_सन्देशः& सन्देशः) ध्रुव CV_OVERRIDE
    {
        यदि (सन्देशः.hasError)
            प्रत्यागम असत्य;
        out << सन्देशः.बफर;
        //DEBUG_ONLY(std::flush(out)); // TODO configure flag
        प्रत्यागम सत्य;
    }
};

वर्ग SyncTraceStorage CV_FINAL : सार्वजनिक TraceStorage
{
    परिवर्तनीय std::ofstream out;
    परिवर्तनीय cv::Mutex mutex;
सार्वजनिक:
    ध्रुव std::string name;

    SyncTraceStorage(ध्रुव std::string& filename) :
        out(filename.c_str(), std::ios::trunc),
        name(filename)
    {
        out << "#description: OpenCV trace file" << std::पङ्क्त्यन्तः;
        out << "#version: 1.0" << std::पङ्क्त्यन्तः;
    }
    ~SyncTraceStorage()
    {
        cv::Auto_तालकम् l(mutex);
        out.close();
    }

    तार्किक put(ध्रुव Trace_सन्देशः& सन्देशः) ध्रुव CV_OVERRIDE
    {
        यदि (सन्देशः.hasError)
            प्रत्यागम असत्य;
        {
            cv::Auto_तालकम् l(mutex);
            out << सन्देशः.बफर;
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
            Trace_सन्देशः सन्देशः;
            ध्रुव वर्ण* pos = strrchr(filepath.c_str(), '/'); // extract filename
#यदि_परिभाषितम् _WIN32
            यदि (!pos)
                pos = strrchr(filepath.c_str(), '\\');
#यदि_अन्तः
            यदि (!pos)
                pos = filepath.c_str();
            अन्यथा
                pos += 1; // fix to skip extra slash in filename beginning
            सन्देशः.प्रारूप_लिख("#thread file: %s\n", pos);
            global->put(सन्देशः);
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

#यदि_परिभाषितम् OPENCV_WITH_ITT
    यदि (isITTEnabled())
    {
        activated = सत्य; // force trace pipeline activation (without OpenCV storage)
        __itt_region_begin(domain, __itt_null, __itt_null, __itt_string_handle_create("OpenCVTrace"));
    }
#यदि_अन्तः
}
TraceManager::~TraceManager()
{
    CV_LOG("TraceManager dtor: " << (शून्य*)एतत्);

#यदि_परिभाषितम् OPENCV_WITH_ITT
    यदि (isITTEnabled())
    {
        __itt_region_end(domain, __itt_null);
    }
#यदि_अन्तः

    std::vector<TraceManagerThreadLocal*> तन्तवः_ctx;
    tls.gather(तन्तवः_ctx);
    size_t योग_Events = 0, योग_Skipped_Events = 0;
    प्रति (size_t i = 0; i < तन्तवः_ctx.size(); i++)
    {
        TraceManagerThreadLocal* ctx = तन्तवः_ctx[i];
        यदि (ctx)
        {
            योग_Events += ctx->region_गणक;
            योग_Skipped_Events += ctx->योग_Skipped_Events;
        }
    }
    यदि (योग_Events || activated)
    {
        CV_LOG_INFO(रिक्त, "Trace: Total events: " << योग_Events);
    }
    यदि (योग_Skipped_Events)
    {
        CV_LOG_WARNING(रिक्त, "Trace: Total skipped events: " << योग_Skipped_Events);
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
        CV_अप्रयुक्तम्(m); // TODO
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

शून्य parallel_For_समुच्चयः_मूलम्_Region(ध्रुव Region& मूलम्_Region, ध्रुव TraceManagerThreadLocal& मूलम्_ctx)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();

    यदि (ctx.dummy_राशिः_शिखरम्.region == &मूलम्_Region) // already attached
        प्रत्यागम;

    CV_Assert(ctx.dummy_राशिः_शिखरम्.region == रिक्त);
    ctx.dummy_राशिः_शिखरम् = TraceManagerThreadLocal::राशिः_प्रविष्टिः(const_cast<Region*>(&मूलम्_Region), रिक्त, -1);

    यदि (&ctx == &मूलम्_ctx)
    {
        ctx.stat.grab(ctx.parallel_for_stat);
        ctx.parallel_for_stat_स्थितिः = ctx.stat_स्थितिः;
        ctx.parallel_for_राशिः_आकार = ctx.राशिः.size();
        प्रत्यागम;
    }

    CV_Assert(ctx.राशिः.empty());

    ctx.current_सक्रिय_Region = const_cast<Region*>(&मूलम्_Region);

    ctx.region_गभीरता = मूलम्_ctx.region_गभीरता;
    ctx.region_गभीरता_Open_CV = मूलम्_ctx.region_गभीरता_Open_CV;

    ctx.parallel_for_राशिः_आकार = 0;

    ctx.stat_स्थितिः.propagateFrom(मूलम्_ctx.stat_स्थितिः);
}

शून्य parallelForAttachNestedRegion(ध्रुव Region& मूलम्_Region)
{
    CV_अप्रयुक्तम्(मूलम्_Region);
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();

    CV_DbgAssert(ctx.dummy_राशिः_शिखरम्.region == &मूलम्_Region);

    Region* region = ctx.get_Current_सक्रिय_Region();
    CV_LOG_PARALLEL(रिक्त, " PARALLEL_FOR: " << (शून्य*)region << " ==> " << &मूलम्_Region);
    यदि (!region)
        प्रत्यागम;

#यदि_परिभाषितम् OPENCV_WITH_ITT
    यदि (!मूलम्_Region.pImpl || !मूलम्_Region.pImpl->itt_id_registered)
        प्रत्यागम;

    यदि (!region->pImpl)
        प्रत्यागम;

    CV_LOG_PARALLEL(रिक्त, " PARALLEL_FOR ITT: " << (शून्य*)मूलम्_Region.pImpl->itt_id.d1 << ":" << मूलम्_Region.pImpl->itt_id.d2 << ":" << (शून्य*)मूलम्_Region.pImpl->itt_id.d3 << " => "
                                 << (शून्य*)region->pImpl->itt_id.d1 << ":" << region->pImpl->itt_id.d2 << ":" << (शून्य*)region->pImpl->itt_id.d3);
    __itt_relation_add(domain, region->pImpl->itt_id, __itt_relation_is_child_of, मूलम्_Region.pImpl->itt_id);
#यदि_अन्तः
}

शून्य parallelForFinalize(ध्रुव Region& मूलम्_Region)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();

    int64 अन्तः_Timestamp = getTimestampNS();
    int64 duration = अन्तः_Timestamp - ctx.राशिः_शिखरम्_Begin_Timestamp();
    CV_LOG_PARALLEL(रिक्त, "parallel_for duration: " << duration << " " << &मूलम्_Region);

    std::vector<TraceManagerThreadLocal*> तन्तवः_ctx;
    getTraceManager().tls.gather(तन्तवः_ctx);
    RegionStatistics parallel_for_stat;
    प्रति (size_t i = 0; i < तन्तवः_ctx.size(); i++)
    {
        TraceManagerThreadLocal* child_ctx = तन्तवः_ctx[i];

        यदि (child_ctx && child_ctx->राशिः_शिखरम्_Region() == &मूलम्_Region)
        {
            CV_LOG_PARALLEL(रिक्त, "Thread=" << child_ctx->threadID << " " << child_ctx->stat);
            RegionStatistics child_stat;
            child_ctx->stat.grab(child_stat);
            parallel_for_stat.append(child_stat);
            यदि (child_ctx != &ctx)
            {
                child_ctx->dummy_राशिः_शिखरम् = TraceManagerThreadLocal::राशिः_प्रविष्टिः();
            }
            अन्यथा
            {
                ctx.parallel_for_stat.grab(ctx.stat);
                ctx.stat_स्थितिः = ctx.parallel_for_stat_स्थितिः;
                child_ctx->dummy_राशिः_शिखरम् = TraceManagerThreadLocal::राशिः_प्रविष्टिः();
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

संरचना TraceArg::Extra_दत्त
{
#यदि_परिभाषितम् OPENCV_WITH_ITT
    // Special fields for ITT
    __itt_string_handle* चञ्चल ittHandle_name;
#यदि_अन्तः
    Extra_दत्त(TraceManagerThreadLocal& ctx, ध्रुव TraceArg& arg)
    {
        CV_अप्रयुक्तम्(ctx); CV_अप्रयुक्तम्(arg);
#यदि_परिभाषितम् OPENCV_WITH_ITT
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
#यदि_अन्तः
    }
};

स्थिर शून्य initTraceArg(TraceManagerThreadLocal& ctx, ध्रुव TraceArg& arg)
{
    TraceArg::Extra_दत्त** pExtra = arg.ppExtra;
    यदि (*pExtra == रिक्त)
    {
        cv::Auto_तालकम् lock(cv::getInitializationMutex());
        यदि (*pExtra == रिक्त)
        {
            *pExtra = नूतन TraceArg::Extra_दत्त(ctx, arg);
        }
    }
}
शून्य traceArg(ध्रुव TraceArg& arg, ध्रुव वर्ण* value)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    Region* region = ctx.get_Current_सक्रिय_Region();
    यदि (!region)
        प्रत्यागम;
    CV_Assert(region->pImpl);
    initTraceArg(ctx, arg);
    यदि (!value)
        value = "<null>";
#यदि_परिभाषितम् OPENCV_WITH_ITT
    यदि (isITTEnabled())
    {
        __itt_metadata_str_add(domain, region->pImpl->itt_id, (*arg.ppExtra)->ittHandle_name, value, वाक्यदैर्घ्यम्(value));
    }
#यदि_अन्तः
}
शून्य traceArg(ध्रुव TraceArg& arg, पूर्णाङ्क value)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    Region* region = ctx.get_Current_सक्रिय_Region();
    यदि (!region)
        प्रत्यागम;
    CV_Assert(region->pImpl);
    initTraceArg(ctx, arg);
#यदि_परिभाषितम् OPENCV_WITH_ITT
    यदि (isITTEnabled())
    {
        __itt_metadata_add(domain, region->pImpl->itt_id, (*arg.ppExtra)->ittHandle_name, परिमाण(पूर्णाङ्क) == 4 ? __itt_metadata_s32 : __itt_metadata_s64, 1, &value);
    }
#अन्यथा
    CV_अप्रयुक्तम्(value);
#यदि_अन्तः
}
शून्य traceArg(ध्रुव TraceArg& arg, int64 value)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    Region* region = ctx.get_Current_सक्रिय_Region();
    यदि (!region)
        प्रत्यागम;
    CV_Assert(region->pImpl);
    initTraceArg(ctx, arg);
#यदि_परिभाषितम् OPENCV_WITH_ITT
    यदि (isITTEnabled())
    {
        __itt_metadata_add(domain, region->pImpl->itt_id, (*arg.ppExtra)->ittHandle_name, __itt_metadata_s64, 1, &value);
    }
#अन्यथा
    CV_अप्रयुक्तम्(value);
#यदि_अन्तः
}
शून्य traceArg(ध्रुव TraceArg& arg, द्विगुण value)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    Region* region = ctx.get_Current_सक्रिय_Region();
    यदि (!region)
        प्रत्यागम;
    CV_Assert(region->pImpl);
    initTraceArg(ctx, arg);
#यदि_परिभाषितम् OPENCV_WITH_ITT
    यदि (isITTEnabled())
    {
        __itt_metadata_add(domain, region->pImpl->itt_id, (*arg.ppExtra)->ittHandle_name, __itt_metadata_double, 1, &value);
    }
#अन्यथा
    CV_अप्रयुक्तम्(value);
#यदि_अन्तः
}

#अन्यथा

Region::Region(ध्रुव LocationStaticStorage&) : pImpl(रिक्त), implFlags(0) {}
शून्य Region::destroy() {}

शून्य traceArg(ध्रुव TraceArg&, ध्रुव वर्ण*) {}
शून्य traceArg(ध्रुव TraceArg&, पूर्णाङ्क) {};
शून्य traceArg(ध्रुव TraceArg&, int64) {};
शून्य traceArg(ध्रुव TraceArg&, द्विगुण) {};

#यदि_अन्तः

}}}} // namespace
