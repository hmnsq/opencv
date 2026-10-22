// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ਅਨ੍ਤਰ੍ਭਾਵ "precomp.hpp"

#ਅਨ੍ਤਰ੍ਭਾਵ <opencv2/core/utils/trace.hpp>
#ਅਨ੍ਤਰ੍ਭਾਵ <opencv2/core/utils/trace.private.hpp>
#ਅਨ੍ਤਰ੍ਭਾਵ <opencv2/core/utils/configuration.private.hpp>

#ਅਨ੍ਤਰ੍ਭਾਵ <opencv2/core/opencl/ocl_defs.hpp>

#ਅਨ੍ਤਰ੍ਭਾਵ <cstdarg> // va_start

#ਅਨ੍ਤਰ੍ਭਾਵ <sstream>
#ਅਨ੍ਤਰ੍ਭਾਵ <ostream>
#ਅਨ੍ਤਰ੍ਭਾਵ <fstream>

#ਜੇ 0
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CV_LOG(...) CV_LOG_INFO(ਨਿਰਾ, __VA_ARGS__)
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CV_LOG(...) {}
#ਜੇ_ਅੰਤ

#ਜੇ 0
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CV_LOG_ITT(...) CV_LOG_INFO(ਨਿਰਾ, __VA_ARGS__)
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CV_LOG_ITT(...) {}
#ਜੇ_ਅੰਤ

#ਜੇ 1
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CV_LOG_TRACE_BAILOUT(ਟੈਗ, ...) CV_LOG_INFO(ਟੈਗ, __VA_ARGS__)
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CV_LOG_TRACE_BAILOUT(...) {}
#ਜੇ_ਅੰਤ

#ਜੇ 0
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CV_LOG_PARALLEL(ਟੈਗ, ...) CV_LOG_INFO(ਟੈਗ, __VA_ARGS__)
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CV_LOG_PARALLEL(...) {}
#ਜੇ_ਅੰਤ

#ਜੇ 0
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CV_LOG_CTX_STAT(ਟੈਗ, ...) CV_LOG_INFO(ਟੈਗ, __VA_ARGS__)
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CV_LOG_CTX_STAT(...) {}
#ਜੇ_ਅੰਤ

#ਜੇ 0
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CV_LOG_SKIP(ਟੈਗ, ...) CV_LOG_INFO(ਟੈਗ, __VA_ARGS__)
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CV_LOG_SKIP(...) {}
#ਜੇ_ਅੰਤ

ਨਾਮਾਕਾਸ਼ cv {
ਨਾਮਾਕਾਸ਼ utils {
ਨਾਮਾਕਾਸ਼ trace {
ਨਾਮਾਕਾਸ਼ details {

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ OPENCV_TRACE

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ _MSC_VER
#ਹਦਾਇਤ warning(disable:4065) // switch statement contains 'default' but no 'case' labels
#ਜੇ_ਅੰਤ

ਸਥਿਰ ਤਾਰਕਿਕ getParameterTraceEnable()
{
    ਸਥਿਰ ਤਾਰਕਿਕ param_traceEnable = utils::getConfigurationParameterBool("OPENCV_TRACE", ਝੂਠ);
    ਵਾਪਸ param_traceEnable;
}

// TODO lazy configuration flags
ਸਥਿਰ ਪੂਰਨ param_max_Region_ਡੂੰਘਾਈ_Open_CV = (ਪੂਰਨ)utils::get_Configuration_Parameter_ਮਾਪ_T("OPENCV_TRACE_DEPTH_OPENCV", 1);
ਸਥਿਰ ਪੂਰਨ param_maxRegionChildrenOpenCV = (ਪੂਰਨ)utils::get_Configuration_Parameter_ਮਾਪ_T("OPENCV_TRACE_MAX_CHILDREN_OPENCV", 1000);
ਸਥਿਰ ਪੂਰਨ param_maxRegionChildren = (ਪੂਰਨ)utils::get_Configuration_Parameter_ਮਾਪ_T("OPENCV_TRACE_MAX_CHILDREN", 10000);

ਸਥਿਰ ਅਚਲ cv::String& getParameterTraceLocation()
{
    ਸਥਿਰ cv::String param_traceLocation = utils::getConfigurationParameterString("OPENCV_TRACE_LOCATION", "OpenCVTrace");
    ਵਾਪਸ param_traceLocation;
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_OPENCL
ਸਥਿਰ ਤਾਰਕਿਕ param_synchronizeOpenCL = utils::getConfigurationParameterBool("OPENCV_TRACE_SYNC_OPENCL", ਝੂਠ);
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ OPENCV_WITH_ITT
ਸਥਿਰ ਤਾਰਕਿਕ param_ITT_ਰਜਿਸਟਰ_ਮਾਪਾ_Scope = utils::getConfigurationParameterBool("OPENCV_TRACE_ITT_PARENT", ਝੂਠ);
#ਜੇ_ਅੰਤ

ਸਥਿਰ ਅਚਲ ਅੱਖਰ* _spaces(ਪੂਰਨ ਗਿਣਤੀ_3)
{
    ਸਥਿਰ ਅਚਲ ਅੱਖਰ buf[64] =
"                                                               ";
    ਵਾਪਸ &buf[63 - (ਗਿਣਤੀ_3 & 63)];
}

/**
 * Text-based trace messages
 */
ਵਰਗ Trace_ਸੁਨੇਹਾ
{
ਜਨਤਕ:
    ਅੱਖਰ ਬਫਰ[1024];
    size_t len;
    ਤਾਰਕਿਕ hasError;

    Trace_ਸੁਨੇਹਾ() :
        len(0),
        hasError(ਝੂਠ)
    {}

    ਤਾਰਕਿਕ ਲਿਖੋ(ਅਚਲ ਅੱਖਰ* format, ...)
    {
        ਅੱਖਰ* buf = &ਬਫਰ[len];
        size_t sz = ਆਕਾਰ(ਬਫਰ) - len;
        va_list ap;
        va_start(ap, format);
        ਪੂਰਨ n = cv_vsnprintf(buf, (ਪੂਰਨ)sz, format, ap);
        va_end(ap);
        ਜੇ (n < 0 || (size_t)n > sz)
        {
            hasError = ਸੱਚ;
            ਵਾਪਸ ਝੂਠ;
        }
        len += n;
        ਵਾਪਸ ਸੱਚ;
    }

    ਤਾਰਕਿਕ formatlocation(ਅਚਲ Region::LocationStaticStorage& location)
    {
        ਵਾਪਸ ਇਹ->ਲਿਖੋ("l,%lld,\"%s\",%d,\"%s\",0x%llX\n",
                (ਲੰਮਾ ਲੰਮਾ ਪੂਰਨ)(*location.ppExtra)->global_location_id,
                location.filename,
                location.ਸਤਰ,
                location.name,
                (ਲੰਮਾ ਲੰਮਾ ਪੂਰਨ)(location.flags & ~0xF0000000));
    }
    ਤਾਰਕਿਕ formatRegionEnter(ਅਚਲ Region& region)
    {
        ਤਾਰਕਿਕ ok = ਇਹ->ਲਿਖੋ("b,%d,%lld,%lld,%lld",
                (ਪੂਰਨ)region.pImpl->threadID,
                (ਲੰਮਾ ਲੰਮਾ ਪੂਰਨ)region.pImpl->beginTimestamp,
                (ਲੰਮਾ ਲੰਮਾ ਪੂਰਨ)((*region.pImpl->location.ppExtra)->global_location_id),
                (ਲੰਮਾ ਲੰਮਾ ਪੂਰਨ)region.pImpl->global_region_id);
        ਜੇ (region.pImpl->ਮਾਪਾ_Region && region.pImpl->ਮਾਪਾ_Region->pImpl)
        {
            ਜੇ (region.pImpl->ਮਾਪਾ_Region->pImpl->threadID != region.pImpl->threadID)
                ok &= ਇਹ->ਲਿਖੋ(",parentThread=%d,parent=%lld",
                        (ਪੂਰਨ)region.pImpl->ਮਾਪਾ_Region->pImpl->threadID,
                        (ਲੰਮਾ ਲੰਮਾ ਪੂਰਨ)region.pImpl->ਮਾਪਾ_Region->pImpl->global_region_id);
        }
        ok &= ਇਹ->ਲਿਖੋ("\n");
        ਵਾਪਸ ok;
    }
    ਤਾਰਕਿਕ formatRegionLeave(ਅਚਲ Region& region, ਅਚਲ RegionStatistics& ਨਤੀਜਾ)
    {
        CV_DbgAssert(region.pImpl->ਸਿਰਾ_Timestamp - region.pImpl->beginTimestamp == ਨਤੀਜਾ.duration);
        ਤਾਰਕਿਕ ok = ਇਹ->ਲਿਖੋ("e,%d,%lld,%lld,%lld,%lld",
                (ਪੂਰਨ)region.pImpl->threadID,
                (ਲੰਮਾ ਲੰਮਾ ਪੂਰਨ)region.pImpl->ਸਿਰਾ_Timestamp,
                (ਲੰਮਾ ਲੰਮਾ ਪੂਰਨ)(*region.pImpl->location.ppExtra)->global_location_id,
                (ਲੰਮਾ ਲੰਮਾ ਪੂਰਨ)region.pImpl->global_region_id,
                (ਲੰਮਾ ਲੰਮਾ ਪੂਰਨ)ਨਤੀਜਾ.duration);
        ਜੇ (ਨਤੀਜਾ.currentSkippedRegions)
            ok &= ਇਹ->ਲਿਖੋ(",skip=%d", (ਪੂਰਨ)ਨਤੀਜਾ.currentSkippedRegions);
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_IPP
        ਜੇ (ਨਤੀਜਾ.durationImplIPP)
            ok &= ਇਹ->ਲਿਖੋ(",tIPP=%lld", (ਲੰਮਾ ਲੰਮਾ ਪੂਰਨ)ਨਤੀਜਾ.durationImplIPP);
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_OPENCL
        ਜੇ (ਨਤੀਜਾ.durationImplOpenCL)
            ok &= ਇਹ->ਲਿਖੋ(",tOCL=%lld", (ਲੰਮਾ ਲੰਮਾ ਪੂਰਨ)ਨਤੀਜਾ.durationImplOpenCL);
#ਜੇ_ਅੰਤ
        ok &= ਇਹ->ਲਿਖੋ("\n");
        ਵਾਪਸ ok;
    }
    ਤਾਰਕਿਕ ਰਿਕਾਰਡ_Region_Arg(ਅਚਲ Region& region, ਅਚਲ TraceArg& arg, ਅਚਲ ਅੱਖਰ* value)
    {
        ਵਾਪਸ ਇਹ->ਲਿਖੋ("a,%d,%lld,%lld,\"%s\",\"%s\"\n",
                region.pImpl->threadID,
                (ਲੰਮਾ ਲੰਮਾ ਪੂਰਨ)region.pImpl->beginTimestamp,
                (ਲੰਮਾ ਲੰਮਾ ਪੂਰਨ)region.pImpl->global_region_id,
                arg.name,
                value);
    }
};


#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ OPENCV_WITH_ITT
ਸਥਿਰ __itt_domain* domain = ਨਿਰਾ;

ਸਥਿਰ ਤਾਰਕਿਕ isITTEnabled()
{
    ਸਥਿਰ ਅਸਥਿਰ ਤਾਰਕਿਕ isInitialized = ਝੂਠ;
    ਸਥਿਰ ਤਾਰਕਿਕ isEnabled = ਝੂਠ;
    ਜੇ (!isInitialized)
    {
        cv::Auto_ਤਾਲਾ lock(cv::getInitializationMutex());
        ਜੇ (!isInitialized)
        {
            ਤਾਰਕਿਕ param_traceITTEnable = utils::getConfigurationParameterBool("OPENCV_TRACE_ITT_ENABLE", ਸੱਚ);
            ਜੇ (param_traceITTEnable)
            {
                isEnabled = !!(__itt_api_version());
                CV_LOG_ITT("ITT is " << (isEnabled ? "enabled" : "disabled"));
                domain = __itt_domain_create("OpenCVTrace");
            }
            ਨਹੀਂਤਾਂ
            {
                CV_LOG_ITT("ITT is disabled through OpenCV parameter");
                isEnabled = ਝੂਠ;
            }
            isInitialized = ਸੱਚ;
        }
    }
    ਵਾਪਸ isEnabled;
}
#ਜੇ_ਅੰਤ


Region::Location_Extra_ਅੰਕੜੇ::Location_Extra_ਅੰਕੜੇ(ਅਚਲ LocationStaticStorage& location)
{
    CV_ਅਣਵਰਤਿਆ(location);
    ਸਥਿਰ ਪੂਰਨ g_location_id_counter = 0;
    global_location_id = CV_XADD(&g_location_id_counter, 1) + 1;
    CV_LOG("Register location: " << global_location_id << " (" << (ਖਾਲੀ*)&location << ")"
            << std::ਸਤਰ_ਅੰਤ << "    file: " << location.filename
            << std::ਸਤਰ_ਅੰਤ << "    line: " << location.ਸਤਰ
            << std::ਸਤਰ_ਅੰਤ << "    name: " << location.name);
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ OPENCV_WITH_ITT
    ਜੇ (isITTEnabled())
    {
        // Caching is not required here, because there is builtin cache.
        // https://software.intel.com/en-us/node/544203:
        //     Consecutive calls to __itt_string_handle_create with the same name return the same value.
        ittHandle_name = __itt_string_handle_create(location.name);
        ittHandle_filename = __itt_string_handle_create(location.filename);
    }
    ਨਹੀਂਤਾਂ
    {
        ittHandle_name = 0;
        ittHandle_filename = 0;
    }
#ਜੇ_ਅੰਤ
}

/*static*/ Region::Location_Extra_ਅੰਕੜੇ* Region::Location_Extra_ਅੰਕੜੇ::init(ਅਚਲ Region::LocationStaticStorage& location)
{
    Location_Extra_ਅੰਕੜੇ** pLocationExtra = location.ppExtra;
    CV_DbgAssert(pLocationExtra);
    ਜੇ (*pLocationExtra == ਨਿਰਾ)
    {
        cv::Auto_ਤਾਲਾ lock(cv::getInitializationMutex());
        ਜੇ (*pLocationExtra == ਨਿਰਾ)
        {
            *pLocationExtra = ਨਵਾਂ Region::Location_Extra_ਅੰਕੜੇ(location);
            TraceStorage* s = getTraceManager().trace_storage.get();
            ਜੇ (s)
            {
                Trace_ਸੁਨੇਹਾ ਸੁਨੇਹਾ;
                ਸੁਨੇਹਾ.formatlocation(location);
                s->put(ਸੁਨੇਹਾ);
            }
        }
    }
    ਵਾਪਸ *pLocationExtra;
}


Region::Impl::Impl(TraceManagerThreadLocal& ctx, Region* ਮਾਪਾ_Region_2, Region& region_, ਅਚਲ LocationStaticStorage& location_, int64 beginTimestamp_) :
    location(location_),
    region(region_),
    ਮਾਪਾ_Region(ਮਾਪਾ_Region_2),
    threadID(ctx.threadID),
    global_region_id(++ctx.region_counter),
    beginTimestamp(beginTimestamp_),
    ਸਿਰਾ_Timestamp(0),
    ਸਿੱਧਾ_Children_ਗਿਣਤੀ(0)
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ OPENCV_WITH_ITT
    ,itt_id_registered(ਝੂਠ)
    ,itt_id(__itt_null)
#ਜੇ_ਅੰਤ
{
    CV_DbgAssert(ctx.current_ਸਰਗਰਮ_Region == ਮਾਪਾ_Region);
    region.pImpl = ਇਹ;

    ਰਜਿਸਟਰ_Region(ctx);

    enterRegion(ctx);
}

Region::Impl::~Impl()
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ OPENCV_WITH_ITT
    ਜੇ (itt_id_registered)
    {
        CV_LOG_ITT(" Destroy ITT region: I=" << (ਖਾਲੀ*)ਇਹ);
        __itt_id_destroy(domain, itt_id);
        itt_id_registered = ਝੂਠ;
    }
#ਜੇ_ਅੰਤ
    region.pImpl = ਨਿਰਾ;
}

ਖਾਲੀ Region::Impl::enterRegion(TraceManagerThreadLocal& ctx)
{
    ctx.current_ਸਰਗਰਮ_Region = &region;

    ਜੇ (location.flags & REGION_FLAG_FUNCTION)
    {
        ਜੇ ((location.flags & REGION_FLAG_APP_ਕੋਡ) == 0)
        {
            ctx.region_ਡੂੰਘਾਈ_Open_CV++;
        }
        ctx.region_ਡੂੰਘਾਈ++;
    }

    TraceStorage* s = ctx.getStorage();
    ਜੇ (s)
    {
        Trace_ਸੁਨੇਹਾ ਸੁਨੇਹਾ;
        ਸੁਨੇਹਾ.formatRegionEnter(region);
        s->put(ਸੁਨੇਹਾ);
    }
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ OPENCV_WITH_ITT
    ਜੇ (isITTEnabled())
    {
        __itt_id ਮਾਪਾ_ID = __itt_null;
        ਜੇ (param_ITT_ਰਜਿਸਟਰ_ਮਾਪਾ_Scope && ਮਾਪਾ_Region && ਮਾਪਾ_Region->pImpl && ਮਾਪਾ_Region->pImpl->itt_id_registered && (location.flags & REGION_FLAG_REGION_FORCE) == 0)
            ਮਾਪਾ_ID = ਮਾਪਾ_Region->pImpl->itt_id;
        __itt_task_begin(domain, itt_id, ਮਾਪਾ_ID, (*location.ppExtra)->ittHandle_name);
    }
#ਜੇ_ਅੰਤ
}

ਖਾਲੀ Region::Impl::leaveRegion(TraceManagerThreadLocal& ctx)
{
    int64 duration = ਸਿਰਾ_Timestamp - beginTimestamp; CV_ਅਣਵਰਤਿਆ(duration);
    RegionStatistics ਨਤੀਜਾ;
    ctx.stat.grab(ਨਤੀਜਾ);
    ctx.ਕੁੱਲ_Skipped_Events += ਨਤੀਜਾ.currentSkippedRegions;
    CV_LOG(_spaces(ctx.get_Current_ਡੂੰਘਾਈ()*4) << "leaveRegion(): " << (ਖਾਲੀ*)ਇਹ << " " << ਨਤੀਜਾ);
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ OPENCV_WITH_ITT
    ਜੇ (isITTEnabled())
    {
        ਜੇ (ਨਤੀਜਾ.currentSkippedRegions)
        {
            __itt_metadata_add(domain, itt_id, __itt_string_handle_create("skipped trace entries"), __itt_metadata_u32, 1, &ਨਤੀਜਾ.currentSkippedRegions);
        }
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_IPP
        ਜੇ (ਨਤੀਜਾ.durationImplIPP)
            __itt_metadata_add(domain, itt_id, __itt_string_handle_create("tIPP"), __itt_metadata_u64, 1, &ਨਤੀਜਾ.durationImplIPP);
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_OPENCL
        ਜੇ (ਨਤੀਜਾ.durationImplOpenCL)
            __itt_metadata_add(domain, itt_id, __itt_string_handle_create("tOpenCL"), __itt_metadata_u64, 1, &ਨਤੀਜਾ.durationImplOpenCL);
#ਜੇ_ਅੰਤ
        __itt_task_end(domain);
    }
#ਜੇ_ਅੰਤ
    TraceStorage* s = ctx.getStorage();
    ਜੇ (s)
    {
        Trace_ਸੁਨੇਹਾ ਸੁਨੇਹਾ;
        ਸੁਨੇਹਾ.formatRegionLeave(region, ਨਤੀਜਾ);
        s->put(ਸੁਨੇਹਾ);
    }

    ਜੇ (location.flags & REGION_FLAG_FUNCTION)
    {
        ਜੇ ((location.flags & REGION_FLAG_APP_ਕੋਡ) == 0)
        {
            ctx.region_ਡੂੰਘਾਈ_Open_CV--;
        }
        ctx.region_ਡੂੰਘਾਈ--;
    }

    ctx.current_ਸਰਗਰਮ_Region = ਮਾਪਾ_Region;
}

ਖਾਲੀ Region::Impl::release()
{
    ਮਿਟਾਓ ਇਹ;
}

ਖਾਲੀ Region::Impl::ਰਜਿਸਟਰ_Region(TraceManagerThreadLocal& ctx)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ OPENCV_WITH_ITT
    ਜੇ (isITTEnabled())
    {
        ਜੇ (!itt_id_registered)
        {
            CV_LOG_ITT(" Register ITT region: I=" << (ਖਾਲੀ*)ਇਹ << " " << ctx.threadID << "-" << global_region_id);
#ਜੇ 1 // workaround for some ITT backends
            itt_id = __itt_id_make((ਖਾਲੀ*)(intptr_t)(((int64)(ctx.threadID + 1) << 32) | global_region_id), global_region_id);
#ਨਹੀਂਤਾਂ
            itt_id = __itt_id_make((ਖਾਲੀ*)(intptr_t)(ctx.threadID + 1), global_region_id);
#ਜੇ_ਅੰਤ
            __itt_id_create(domain, itt_id);
            itt_id_registered = ਸੱਚ;
        }
    }
#ਨਹੀਂਤਾਂ
    CV_ਅਣਵਰਤਿਆ(ctx);
#ਜੇ_ਅੰਤ
}

ਖਾਲੀ Region_Statistics_ਸਥਿਤੀ::enable_Skip_ਬਹੁਲਕ(ਪੂਰਨ ਡੂੰਘਾਈ)
{
    CV_DbgAssert(skip_ਡੂੰਘਾਈ < 0);
    CV_LOG_SKIP(ਨਿਰਾ, "SKIP-ENABLE: depth=" << ਡੂੰਘਾਈ);
    skip_ਡੂੰਘਾਈ = ਡੂੰਘਾਈ;
}
ਖਾਲੀ Region_Statistics_ਸਥਿਤੀ::check_Reset_Skip_ਬਹੁਲਕ(ਪੂਰਨ leave_ਡੂੰਘਾਈ)
{
    ਜੇ (leave_ਡੂੰਘਾਈ <= skip_ਡੂੰਘਾਈ)
    {
        CV_LOG_SKIP(ਨਿਰਾ, "SKIP-RESET: leaveDepth=" << leave_ਡੂੰਘਾਈ << " skipDepth=" << skip_ਡੂੰਘਾਈ);
        skip_ਡੂੰਘਾਈ = -1;
    }
}

Region::Region(ਅਚਲ LocationStaticStorage& location) :
    pImpl(ਨਿਰਾ),
    implFlags(0)
{
    // Checks:
    // - global enable flag
    // - parent region is disabled
    // - children count threshold
    // - region location
    // - depth (opencv nested calls)
    ਜੇ (!TraceManager::isActivated())
    {
        CV_LOG("Trace is disabled. Bailout");
        ਵਾਪਸ;
    }

    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    CV_LOG(_spaces(ctx.get_Current_ਡੂੰਘਾਈ()*4) << "Region(): " << (ਖਾਲੀ*)ਇਹ << ": " << location.name);

    Region* ਮਾਪਾ_Region = ctx.ਢੇਰ_ਸਿਖਰ_Region();
    ਅਚਲ Region::LocationStaticStorage* ਮਾਪਾ_Location = ctx.ਢੇਰ_ਸਿਖਰ_Location();

    ਜੇ (location.flags & REGION_FLAG_REGION_ਅਗਲਾ)
    {
        ਜੇ (ਮਾਪਾ_Region && ਮਾਪਾ_Region->pImpl)
        {
            CV_DbgAssert((ਮਾਪਾ_Region->pImpl->location.flags & REGION_FLAG_FUNCTION) == 0);
            ਮਾਪਾ_Region->destroy(); ਮਾਪਾ_Region->implFlags = 0;
            ਮਾਪਾ_Region = ctx.ਢੇਰ_ਸਿਖਰ_Region();
            ਮਾਪਾ_Location = ctx.ਢੇਰ_ਸਿਖਰ_Location();
        }
    }

    ਪੂਰਨ ਮਾਪਾ_Children = 0;
    ਜੇ (ਮਾਪਾ_Region && ਮਾਪਾ_Region->pImpl)
    {
        ਜੇ (ਮਾਪਾ_Location == ਨਿਰਾ)
        {
            // parallel_for_body code path
            ਮਾਪਾ_Children = CV_XADD(&ਮਾਪਾ_Region->pImpl->ਸਿੱਧਾ_Children_ਗਿਣਤੀ, 1) + 1;
        }
        ਨਹੀਂਤਾਂ
        {
            ਮਾਪਾ_Children = ++ਮਾਪਾ_Region->pImpl->ਸਿੱਧਾ_Children_ਗਿਣਤੀ;
        }
    }

    int64 beginTimestamp = getTimestampNS();

    ਪੂਰਨ current_ਡੂੰਘਾਈ = ctx.get_Current_ਡੂੰਘਾਈ() + 1;
    ਚੋਣ (location.flags & REGION_FLAG_IMPL_ਨਕਾਬ)
    {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_IPP
    ਹਾਲਤ REGION_FLAG_IMPL_IPP:
        ਜੇ (!ctx.stat_ਸਥਿਤੀ.ignore_ਡੂੰਘਾਈ_Impl_IPP)
            ctx.stat_ਸਥਿਤੀ.ignore_ਡੂੰਘਾਈ_Impl_IPP = current_ਡੂੰਘਾਈ;
        ਰੋਕੋ;
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_OPENCL
    ਹਾਲਤ REGION_FLAG_IMPL_OPENCL:
        ਜੇ (!ctx.stat_ਸਥਿਤੀ.ignore_ਡੂੰਘਾਈ_Impl_Open_CL)
            ctx.stat_ਸਥਿਤੀ.ignore_ਡੂੰਘਾਈ_Impl_Open_CL = current_ਡੂੰਘਾਈ;
        ਰੋਕੋ;
#ਜੇ_ਅੰਤ
    ਮੂਲ:
        ਰੋਕੋ;
    }

    ctx.ਢੇਰ_ਧੱਕੋ(ਇਹ, &location, beginTimestamp);
    implFlags |= REGION_FLAG_NEED_ਢੇਰ_ਕੱਢੋ;

    ਜੇ ((location.flags & REGION_FLAG_REGION_FORCE) == 0)
    {
        ਜੇ (ctx.stat_ਸਥਿਤੀ.skip_ਡੂੰਘਾਈ >= 0 && current_ਡੂੰਘਾਈ > ctx.stat_ਸਥਿਤੀ.skip_ਡੂੰਘਾਈ)
        {
            CV_LOG(_spaces(ctx.get_Current_ਡੂੰਘਾਈ()*4) << "Parent region is disabled. Bailout");
            ctx.stat.currentSkippedRegions++;
            ਵਾਪਸ;
        }

        ਜੇ (param_maxRegionChildrenOpenCV > 0 && (location.flags & REGION_FLAG_APP_ਕੋਡ) == 0 && ਮਾਪਾ_Location && (ਮਾਪਾ_Location->flags & REGION_FLAG_APP_ਕੋਡ) == 0)
        {
            ਜੇ (ਮਾਪਾ_Children >= param_maxRegionChildrenOpenCV)
            {
                CV_LOG_TRACE_BAILOUT(ਨਿਰਾ, _spaces(ctx.get_Current_ਡੂੰਘਾਈ()*4) << "OpenCV parent region exceeds children count. Bailout");
                ctx.stat_ਸਥਿਤੀ.enable_Skip_ਬਹੁਲਕ(current_ਡੂੰਘਾਈ - 1);
                ctx.stat.currentSkippedRegions++;
                DEBUG_ONLY(ctx.dump_ਢੇਰ(std::ਬਾਹਰ, ਝੂਠ));
                ਵਾਪਸ;
            }
        }
        ਜੇ (param_maxRegionChildren > 0 && ਮਾਪਾ_Children >= param_maxRegionChildren)
        {
            CV_LOG_TRACE_BAILOUT(ਨਿਰਾ, _spaces(ctx.get_Current_ਡੂੰਘਾਈ()*4) << "Parent region exceeds children count. Bailout");
            ctx.stat_ਸਥਿਤੀ.enable_Skip_ਬਹੁਲਕ(current_ਡੂੰਘਾਈ - 1);
            ctx.stat.currentSkippedRegions++;
            DEBUG_ONLY(ctx.dump_ਢੇਰ(std::ਬਾਹਰ, ਝੂਠ));
            ਵਾਪਸ;
        }
    }

    Location_Extra_ਅੰਕੜੇ::init(location);

    ਜੇ ((*location.ppExtra)->global_location_id == 0)
    {
        CV_LOG_TRACE_BAILOUT(ਨਿਰਾ, _spaces(ctx.get_Current_ਡੂੰਘਾਈ()*4) << "Region location is disabled. Bailout");
        ctx.stat_ਸਥਿਤੀ.enable_Skip_ਬਹੁਲਕ(current_ਡੂੰਘਾਈ);
        ctx.stat.currentSkippedRegions++;
        ਵਾਪਸ;
    }

    ਜੇ (ਮਾਪਾ_Location && (ਮਾਪਾ_Location->flags & REGION_FLAG_SKIP_NESTED))
    {
        CV_LOG(_spaces(ctx.get_Current_ਡੂੰਘਾਈ()*4) << "Parent region disables inner regions. Bailout");
        ctx.stat_ਸਥਿਤੀ.enable_Skip_ਬਹੁਲਕ(current_ਡੂੰਘਾਈ);
        ctx.stat.currentSkippedRegions++;
        ਵਾਪਸ;
    }

    ਜੇ (param_max_Region_ਡੂੰਘਾਈ_Open_CV)
    {
        ਜੇ ((location.flags & REGION_FLAG_APP_ਕੋਡ) == 0)
        {
            ਜੇ (ctx.region_ਡੂੰਘਾਈ_Open_CV >= param_max_Region_ਡੂੰਘਾਈ_Open_CV)
            {
                CV_LOG(_spaces(ctx.get_Current_ਡੂੰਘਾਈ()*4) << "OpenCV region depth is exceed = " << ctx.region_ਡੂੰਘਾਈ_Open_CV << ". Bailout");
                ਜੇ (ctx.stat.currentSkippedRegions == 0)
                {
                    DEBUG_ONLY(ctx.dump_ਢੇਰ(std::ਬਾਹਰ, ਝੂਠ));
                }
                ctx.stat_ਸਥਿਤੀ.enable_Skip_ਬਹੁਲਕ(current_ਡੂੰਘਾਈ);
                ctx.stat.currentSkippedRegions++;
                ਵਾਪਸ;
            }
        }
    }

    ਨਵਾਂ Impl(ctx, ਮਾਪਾ_Region, *ਇਹ, location, beginTimestamp);
    CV_DbgAssert(pImpl != ਨਿਰਾ);
    implFlags |= REGION_FLAG_ਸਰਗਰਮ;

    // parallel_for path
    ਜੇ (ਮਾਪਾ_Region && ਮਾਪਾ_Region->pImpl)
    {
        ਜੇ (ਮਾਪਾ_Location == ਨਿਰਾ)
        {
            pImpl->ਸਿੱਧਾ_Children_ਗਿਣਤੀ = ਮਾਪਾ_Children;
        }
    }
}

ਖਾਲੀ Region::destroy()
{
    CV_DbgAssert(implFlags != 0);

    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    CV_LOG(_spaces(ctx.get_Current_ਡੂੰਘਾਈ()*4) << "Region::destruct(): " << (ਖਾਲੀ*)ਇਹ << " pImpl=" << pImpl << " implFlags=" << implFlags << ' ' << (ctx.ਢੇਰ_ਸਿਖਰ_Location() ? ctx.ਢੇਰ_ਸਿਖਰ_Location()->name : "<unknown>"));

    CV_DbgAssert(implFlags & REGION_FLAG_NEED_ਢੇਰ_ਕੱਢੋ);
    ਅਚਲ ਪੂਰਨ current_ਡੂੰਘਾਈ = ctx.get_Current_ਡੂੰਘਾਈ(); CV_ਅਣਵਰਤਿਆ(current_ਡੂੰਘਾਈ);

    CV_LOG_CTX_STAT(ਨਿਰਾ, _spaces(current_ਡੂੰਘਾਈ*4) << ctx.stat << ' ' << ctx.stat_ਸਥਿਤੀ);

    ਅਚਲ Region::LocationStaticStorage* location = ctx.ਢੇਰ_ਸਿਖਰ_Location();
    Impl::Optimization_ਰਸਤਾ my_ਕੋਡ_ਰਸਤਾ = Impl::ਕੋਡ_ਰਸਤਾ_PLAIN;
    ਜੇ (location)
    {
        ਚੋਣ (location->flags & REGION_FLAG_IMPL_ਨਕਾਬ)
        {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_IPP
        ਹਾਲਤ REGION_FLAG_IMPL_IPP:
            my_ਕੋਡ_ਰਸਤਾ = Impl::ਕੋਡ_ਰਸਤਾ_IPP;
            ਰੋਕੋ;
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_OPENCL
        ਹਾਲਤ REGION_FLAG_IMPL_OPENCL:
            ਜੇ (param_synchronizeOpenCL && cv::ocl::isOpenCLActivated())
                cv::ocl::ਅੰਤ_2();
            my_ਕੋਡ_ਰਸਤਾ = Impl::ਕੋਡ_ਰਸਤਾ_OPENCL;
            ਰੋਕੋ;
#ਜੇ_ਅੰਤ
        ਮੂਲ:
            ਰੋਕੋ;
        }
    }

    int64 ਸਿਰਾ_Timestamp = getTimestampNS();
    int64 duration = ਸਿਰਾ_Timestamp - ctx.ਢੇਰ_ਸਿਖਰ_Begin_Timestamp();

    ਤਾਰਕਿਕ ਸਰਗਰਮ = is_ਸਰਗਰਮ_2();

    ਜੇ (ਸਰਗਰਮ)
        ctx.stat.duration = duration;
    ਨਹੀਂਤਾਂ ਜੇ (ctx.ਢੇਰ.size() == ctx.parallel_for_ਢੇਰ_ਮਾਪ + 1)
        ctx.stat.duration += duration;

    ਚੋਣ (my_ਕੋਡ_ਰਸਤਾ) {
        ਹਾਲਤ Impl::ਕੋਡ_ਰਸਤਾ_PLAIN:
            // nothing
            ਰੋਕੋ;
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_IPP
        ਹਾਲਤ Impl::ਕੋਡ_ਰਸਤਾ_IPP:
            ਜੇ (ctx.stat_ਸਥਿਤੀ.ignore_ਡੂੰਘਾਈ_Impl_IPP == current_ਡੂੰਘਾਈ)
            {
                ctx.stat.durationImplIPP += duration;
                ctx.stat_ਸਥਿਤੀ.ignore_ਡੂੰਘਾਈ_Impl_IPP = 0;
            }
            ਨਹੀਂਤਾਂ ਜੇ (ਸਰਗਰਮ)
            {
                ctx.stat.durationImplIPP = duration;
            }
            ਰੋਕੋ;
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_OPENCL
        ਹਾਲਤ Impl::ਕੋਡ_ਰਸਤਾ_OPENCL:
            ਜੇ (ctx.stat_ਸਥਿਤੀ.ignore_ਡੂੰਘਾਈ_Impl_Open_CL == current_ਡੂੰਘਾਈ)
            {
                ctx.stat.durationImplOpenCL += duration;
                ctx.stat_ਸਥਿਤੀ.ignore_ਡੂੰਘਾਈ_Impl_Open_CL = 0;
            }
            ਨਹੀਂਤਾਂ ਜੇ (ਸਰਗਰਮ)
            {
                ctx.stat.durationImplOpenCL = duration;
            }
            ਰੋਕੋ;
#ਜੇ_ਅੰਤ
        ਮੂਲ:
            ਰੋਕੋ;
    }

    ਜੇ (pImpl)
    {
        CV_DbgAssert((implFlags & (REGION_FLAG_ਸਰਗਰਮ | REGION_FLAG_NEED_ਢੇਰ_ਕੱਢੋ)) == (REGION_FLAG_ਸਰਗਰਮ | REGION_FLAG_NEED_ਢੇਰ_ਕੱਢੋ));
        CV_DbgAssert(ctx.ਢੇਰ_ਸਿਖਰ_Region() == ਇਹ);
        pImpl->ਸਿਰਾ_Timestamp = ਸਿਰਾ_Timestamp;
        pImpl->leaveRegion(ctx);
        pImpl->release();
        pImpl = ਨਿਰਾ;
        DEBUG_ONLY(implFlags &= ~REGION_FLAG_ਸਰਗਰਮ);
    }
    ਨਹੀਂਤਾਂ
    {
        CV_DbgAssert(ctx.stat_ਸਥਿਤੀ.skip_ਡੂੰਘਾਈ <= current_ਡੂੰਘਾਈ);
    }

    ਜੇ (implFlags & REGION_FLAG_NEED_ਢੇਰ_ਕੱਢੋ)
    {
        CV_DbgAssert(ctx.ਢੇਰ_ਸਿਖਰ_Region() == ਇਹ);
        ctx.ਢੇਰ_ਕੱਢੋ();
        ctx.stat_ਸਥਿਤੀ.check_Reset_Skip_ਬਹੁਲਕ(current_ਡੂੰਘਾਈ);
        DEBUG_ONLY(implFlags &= ~REGION_FLAG_NEED_ਢੇਰ_ਕੱਢੋ);
    }
    CV_LOG_CTX_STAT(ਨਿਰਾ, _spaces(current_ਡੂੰਘਾਈ*4) << "===> " << ctx.stat << ' ' << ctx.stat_ਸਥਿਤੀ);
}


TraceManagerThreadLocal::~TraceManagerThreadLocal()
{
}

ਖਾਲੀ TraceManagerThreadLocal::dump_ਢੇਰ(std::ostream& out, ਤਾਰਕਿਕ onlyFunctions) ਅਚਲ
{
    std::stringstream ss;
    std::deque<ਢੇਰ_ਇੰਦਰਾਜ>::const_iterator it = ਢੇਰ.begin();
    std::deque<ਢੇਰ_ਇੰਦਰਾਜ>::const_iterator end = ਢੇਰ.end();
    ਪੂਰਨ ਡੂੰਘਾਈ = 0;
    ਲਈ (; it != end; ++it)
    {
        ਅਚਲ Region::LocationStaticStorage* location = it->location;
        ਜੇ (location)
        {
            ਜੇ (!onlyFunctions || (location->flags & REGION_FLAG_FUNCTION))
            {
                ss << _spaces(4*ਡੂੰਘਾਈ) << location->name << std::ਸਤਰ_ਅੰਤ;
                ਡੂੰਘਾਈ++;
            }
        }
        ਨਹੀਂਤਾਂ
        {
            ss << _spaces(4*ਡੂੰਘਾਈ) << "<unknown>" << std::ਸਤਰ_ਅੰਤ;
            ਡੂੰਘਾਈ++;
        }
    }
    out << ss.str();
}

ਵਰਗ AsyncTraceStorage CV_FINAL : ਜਨਤਕ TraceStorage
{
    ਬਦਲਣਯੋਗ std::ofstream out;
ਜਨਤਕ:
    ਅਚਲ std::string name;

    AsyncTraceStorage(ਅਚਲ std::string& filename) :
        out(filename.c_str(), std::ios::trunc),
        name(filename)
    {
        out << "#description: OpenCV trace file" << std::ਸਤਰ_ਅੰਤ;
        out << "#version: 1.0" << std::ਸਤਰ_ਅੰਤ;
    }
    ~AsyncTraceStorage()
    {
        out.close();
    }

    ਤਾਰਕਿਕ put(ਅਚਲ Trace_ਸੁਨੇਹਾ& ਸੁਨੇਹਾ) ਅਚਲ CV_OVERRIDE
    {
        ਜੇ (ਸੁਨੇਹਾ.hasError)
            ਵਾਪਸ ਝੂਠ;
        out << ਸੁਨੇਹਾ.ਬਫਰ;
        //DEBUG_ONLY(std::flush(out)); // TODO configure flag
        ਵਾਪਸ ਸੱਚ;
    }
};

ਵਰਗ SyncTraceStorage CV_FINAL : ਜਨਤਕ TraceStorage
{
    ਬਦਲਣਯੋਗ std::ofstream out;
    ਬਦਲਣਯੋਗ cv::Mutex mutex;
ਜਨਤਕ:
    ਅਚਲ std::string name;

    SyncTraceStorage(ਅਚਲ std::string& filename) :
        out(filename.c_str(), std::ios::trunc),
        name(filename)
    {
        out << "#description: OpenCV trace file" << std::ਸਤਰ_ਅੰਤ;
        out << "#version: 1.0" << std::ਸਤਰ_ਅੰਤ;
    }
    ~SyncTraceStorage()
    {
        cv::Auto_ਤਾਲਾ l(mutex);
        out.close();
    }

    ਤਾਰਕਿਕ put(ਅਚਲ Trace_ਸੁਨੇਹਾ& ਸੁਨੇਹਾ) ਅਚਲ CV_OVERRIDE
    {
        ਜੇ (ਸੁਨੇਹਾ.hasError)
            ਵਾਪਸ ਝੂਠ;
        {
            cv::Auto_ਤਾਲਾ l(mutex);
            out << ਸੁਨੇਹਾ.ਬਫਰ;
            std::flush(out); // TODO configure flag
        }
        ਵਾਪਸ ਸੱਚ;
    }
};


TraceStorage* TraceManagerThreadLocal::getStorage() ਅਚਲ
{
    // TODO configuration option for stdout/single trace file
    ਜੇ (storage.empty())
    {
        TraceStorage* global = getTraceManager().trace_storage.get();
        ਜੇ (global)
        {
            ਅਚਲ std::string filepath = cv::format("%s-%03d.txt", getParameterTraceLocation().c_str(), threadID).c_str();
            Trace_ਸੁਨੇਹਾ ਸੁਨੇਹਾ;
            ਅਚਲ ਅੱਖਰ* pos = strrchr(filepath.c_str(), '/'); // extract filename
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ _WIN32
            ਜੇ (!pos)
                pos = strrchr(filepath.c_str(), '\\');
#ਜੇ_ਅੰਤ
            ਜੇ (!pos)
                pos = filepath.c_str();
            ਨਹੀਂਤਾਂ
                pos += 1; // fix to skip extra slash in filename beginning
            ਸੁਨੇਹਾ.ਲਿਖੋ("#thread file: %s\n", pos);
            global->put(ਸੁਨੇਹਾ);
            storage.reset(ਨਵਾਂ AsyncTraceStorage(filepath));
        }
    }
    ਵਾਪਸ storage.get();
}



ਸਥਿਰ ਤਾਰਕਿਕ activated = ਝੂਠ;
ਸਥਿਰ ਤਾਰਕਿਕ isInitialized = ਝੂਠ;

TraceManager::TraceManager()
{
    (ਖਾਲੀ)cv::getTimestampNS();

    isInitialized = ਸੱਚ;
    CV_LOG("TraceManager ctor: " << (ਖਾਲੀ*)ਇਹ);

    CV_LOG("TraceManager configure()");
    activated = getParameterTraceEnable();

    ਜੇ (activated)
        trace_storage.reset(ਨਵਾਂ SyncTraceStorage(std::string(getParameterTraceLocation()) + ".txt"));

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ OPENCV_WITH_ITT
    ਜੇ (isITTEnabled())
    {
        activated = ਸੱਚ; // force trace pipeline activation (without OpenCV storage)
        __itt_region_begin(domain, __itt_null, __itt_null, __itt_string_handle_create("OpenCVTrace"));
    }
#ਜੇ_ਅੰਤ
}
TraceManager::~TraceManager()
{
    CV_LOG("TraceManager dtor: " << (ਖਾਲੀ*)ਇਹ);

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ OPENCV_WITH_ITT
    ਜੇ (isITTEnabled())
    {
        __itt_region_end(domain, __itt_null);
    }
#ਜੇ_ਅੰਤ

    std::vector<TraceManagerThreadLocal*> ਧਾਗੇ_ctx;
    tls.gather(ਧਾਗੇ_ctx);
    size_t ਕੁੱਲ_Events = 0, ਕੁੱਲ_Skipped_Events = 0;
    ਲਈ (size_t i = 0; i < ਧਾਗੇ_ctx.size(); i++)
    {
        TraceManagerThreadLocal* ctx = ਧਾਗੇ_ctx[i];
        ਜੇ (ctx)
        {
            ਕੁੱਲ_Events += ctx->region_counter;
            ਕੁੱਲ_Skipped_Events += ctx->ਕੁੱਲ_Skipped_Events;
        }
    }
    ਜੇ (ਕੁੱਲ_Events || activated)
    {
        CV_LOG_INFO(ਨਿਰਾ, "Trace: Total events: " << ਕੁੱਲ_Events);
    }
    ਜੇ (ਕੁੱਲ_Skipped_Events)
    {
        CV_LOG_WARNING(ਨਿਰਾ, "Trace: Total skipped events: " << ਕੁੱਲ_Skipped_Events);
    }

    // This is a global static object, so process starts shutdown here
    // Turn off trace
    cv::__termination = ਸੱਚ; // also set in DllMain() notifications handler for DLL_PROCESS_DETACH
    activated = ਝੂਠ;
}

ਤਾਰਕਿਕ TraceManager::isActivated()
{
    // Check if process starts shutdown, and set earlyExit to true
    // to prevent further instrumentation processing earlier.
    ਜੇ (cv::__termination)
    {
        activated = ਝੂਠ;
        ਵਾਪਸ ਝੂਠ;
    }

    ਜੇ (!isInitialized)
    {
        TraceManager& m = getTraceManager();
        CV_ਅਣਵਰਤਿਆ(m); // TODO
    }

    ਵਾਪਸ activated;
}


ਸਥਿਰ TraceManager* getTraceManagerCallOnce()
{
    ਸਥਿਰ TraceManager globalInstance;
    ਵਾਪਸ &globalInstance;
}
TraceManager& getTraceManager()
{
    CV_SINGLETON_LAZY_INIT_REF(TraceManager, getTraceManagerCallOnce())
}

ਖਾਲੀ parallel_For_ਸੈੱਟ_ਜੜ੍ਹ_Region(ਅਚਲ Region& ਜੜ੍ਹ_Region, ਅਚਲ TraceManagerThreadLocal& ਜੜ੍ਹ_ctx)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();

    ਜੇ (ctx.dummy_ਢੇਰ_ਸਿਖਰ.region == &ਜੜ੍ਹ_Region) // already attached
        ਵਾਪਸ;

    CV_Assert(ctx.dummy_ਢੇਰ_ਸਿਖਰ.region == ਨਿਰਾ);
    ctx.dummy_ਢੇਰ_ਸਿਖਰ = TraceManagerThreadLocal::ਢੇਰ_ਇੰਦਰਾਜ(const_cast<Region*>(&ਜੜ੍ਹ_Region), ਨਿਰਾ, -1);

    ਜੇ (&ctx == &ਜੜ੍ਹ_ctx)
    {
        ctx.stat.grab(ctx.parallel_for_stat);
        ctx.parallel_for_stat_ਸਥਿਤੀ = ctx.stat_ਸਥਿਤੀ;
        ctx.parallel_for_ਢੇਰ_ਮਾਪ = ctx.ਢੇਰ.size();
        ਵਾਪਸ;
    }

    CV_Assert(ctx.ਢੇਰ.empty());

    ctx.current_ਸਰਗਰਮ_Region = const_cast<Region*>(&ਜੜ੍ਹ_Region);

    ctx.region_ਡੂੰਘਾਈ = ਜੜ੍ਹ_ctx.region_ਡੂੰਘਾਈ;
    ctx.region_ਡੂੰਘਾਈ_Open_CV = ਜੜ੍ਹ_ctx.region_ਡੂੰਘਾਈ_Open_CV;

    ctx.parallel_for_ਢੇਰ_ਮਾਪ = 0;

    ctx.stat_ਸਥਿਤੀ.propagateFrom(ਜੜ੍ਹ_ctx.stat_ਸਥਿਤੀ);
}

ਖਾਲੀ parallelForAttachNestedRegion(ਅਚਲ Region& ਜੜ੍ਹ_Region)
{
    CV_ਅਣਵਰਤਿਆ(ਜੜ੍ਹ_Region);
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();

    CV_DbgAssert(ctx.dummy_ਢੇਰ_ਸਿਖਰ.region == &ਜੜ੍ਹ_Region);

    Region* region = ctx.get_Current_ਸਰਗਰਮ_Region();
    CV_LOG_PARALLEL(ਨਿਰਾ, " PARALLEL_FOR: " << (ਖਾਲੀ*)region << " ==> " << &ਜੜ੍ਹ_Region);
    ਜੇ (!region)
        ਵਾਪਸ;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ OPENCV_WITH_ITT
    ਜੇ (!ਜੜ੍ਹ_Region.pImpl || !ਜੜ੍ਹ_Region.pImpl->itt_id_registered)
        ਵਾਪਸ;

    ਜੇ (!region->pImpl)
        ਵਾਪਸ;

    CV_LOG_PARALLEL(ਨਿਰਾ, " PARALLEL_FOR ITT: " << (ਖਾਲੀ*)ਜੜ੍ਹ_Region.pImpl->itt_id.d1 << ":" << ਜੜ੍ਹ_Region.pImpl->itt_id.d2 << ":" << (ਖਾਲੀ*)ਜੜ੍ਹ_Region.pImpl->itt_id.d3 << " => "
                                 << (ਖਾਲੀ*)region->pImpl->itt_id.d1 << ":" << region->pImpl->itt_id.d2 << ":" << (ਖਾਲੀ*)region->pImpl->itt_id.d3);
    __itt_relation_add(domain, region->pImpl->itt_id, __itt_relation_is_child_of, ਜੜ੍ਹ_Region.pImpl->itt_id);
#ਜੇ_ਅੰਤ
}

ਖਾਲੀ parallelForFinalize(ਅਚਲ Region& ਜੜ੍ਹ_Region)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();

    int64 ਸਿਰਾ_Timestamp = getTimestampNS();
    int64 duration = ਸਿਰਾ_Timestamp - ctx.ਢੇਰ_ਸਿਖਰ_Begin_Timestamp();
    CV_LOG_PARALLEL(ਨਿਰਾ, "parallel_for duration: " << duration << " " << &ਜੜ੍ਹ_Region);

    std::vector<TraceManagerThreadLocal*> ਧਾਗੇ_ctx;
    getTraceManager().tls.gather(ਧਾਗੇ_ctx);
    RegionStatistics parallel_for_stat;
    ਲਈ (size_t i = 0; i < ਧਾਗੇ_ctx.size(); i++)
    {
        TraceManagerThreadLocal* ਬੱਚਾ_ctx = ਧਾਗੇ_ctx[i];

        ਜੇ (ਬੱਚਾ_ctx && ਬੱਚਾ_ctx->ਢੇਰ_ਸਿਖਰ_Region() == &ਜੜ੍ਹ_Region)
        {
            CV_LOG_PARALLEL(ਨਿਰਾ, "Thread=" << ਬੱਚਾ_ctx->threadID << " " << ਬੱਚਾ_ctx->stat);
            RegionStatistics ਬੱਚਾ_stat;
            ਬੱਚਾ_ctx->stat.grab(ਬੱਚਾ_stat);
            parallel_for_stat.append(ਬੱਚਾ_stat);
            ਜੇ (ਬੱਚਾ_ctx != &ctx)
            {
                ਬੱਚਾ_ctx->dummy_ਢੇਰ_ਸਿਖਰ = TraceManagerThreadLocal::ਢੇਰ_ਇੰਦਰਾਜ();
            }
            ਨਹੀਂਤਾਂ
            {
                ctx.parallel_for_stat.grab(ctx.stat);
                ctx.stat_ਸਥਿਤੀ = ctx.parallel_for_stat_ਸਥਿਤੀ;
                ਬੱਚਾ_ctx->dummy_ਢੇਰ_ਸਿਖਰ = TraceManagerThreadLocal::ਢੇਰ_ਇੰਦਰਾਜ();
            }
        }
    }

    ਦਸ਼ਮਲਵ parallel_coeff = std::min(1.0f, duration / (ਦਸ਼ਮਲਵ)(parallel_for_stat.duration));
    CV_LOG_PARALLEL(ਨਿਰਾ, "parallel_coeff=" << 1.0f / parallel_coeff);
    CV_LOG_PARALLEL(ਨਿਰਾ, parallel_for_stat);
    ਜੇ (parallel_coeff != 1.0f)
    {
        parallel_for_stat.multiply(parallel_coeff);
        CV_LOG_PARALLEL(ਨਿਰਾ, parallel_for_stat);
    }
    parallel_for_stat.duration = 0;
    ctx.stat.append(parallel_for_stat);
    CV_LOG_PARALLEL(ਨਿਰਾ, ctx.stat);
}

ਬਣਤਰ TraceArg::Extra_ਅੰਕੜੇ
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ OPENCV_WITH_ITT
    // Special fields for ITT
    __itt_string_handle* ਅਸਥਿਰ ittHandle_name;
#ਜੇ_ਅੰਤ
    Extra_ਅੰਕੜੇ(TraceManagerThreadLocal& ctx, ਅਚਲ TraceArg& arg)
    {
        CV_ਅਣਵਰਤਿਆ(ctx); CV_ਅਣਵਰਤਿਆ(arg);
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ OPENCV_WITH_ITT
        ਜੇ (isITTEnabled())
        {
            // Caching is not required here, because there is builtin cache.
            // https://software.intel.com/en-us/node/544203:
            //     Consecutive calls to __itt_string_handle_create with the same name return the same value.
            ittHandle_name = __itt_string_handle_create(arg.name);
        }
        ਨਹੀਂਤਾਂ
        {
            ittHandle_name = 0;
        }
#ਜੇ_ਅੰਤ
    }
};

ਸਥਿਰ ਖਾਲੀ initTraceArg(TraceManagerThreadLocal& ctx, ਅਚਲ TraceArg& arg)
{
    TraceArg::Extra_ਅੰਕੜੇ** pExtra = arg.ppExtra;
    ਜੇ (*pExtra == ਨਿਰਾ)
    {
        cv::Auto_ਤਾਲਾ lock(cv::getInitializationMutex());
        ਜੇ (*pExtra == ਨਿਰਾ)
        {
            *pExtra = ਨਵਾਂ TraceArg::Extra_ਅੰਕੜੇ(ctx, arg);
        }
    }
}
ਖਾਲੀ traceArg(ਅਚਲ TraceArg& arg, ਅਚਲ ਅੱਖਰ* value)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    Region* region = ctx.get_Current_ਸਰਗਰਮ_Region();
    ਜੇ (!region)
        ਵਾਪਸ;
    CV_Assert(region->pImpl);
    initTraceArg(ctx, arg);
    ਜੇ (!value)
        value = "<null>";
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ OPENCV_WITH_ITT
    ਜੇ (isITTEnabled())
    {
        __itt_metadata_str_add(domain, region->pImpl->itt_id, (*arg.ppExtra)->ittHandle_name, value, ਲੜੀ_ਲੰਬਾਈ(value));
    }
#ਜੇ_ਅੰਤ
}
ਖਾਲੀ traceArg(ਅਚਲ TraceArg& arg, ਪੂਰਨ value)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    Region* region = ctx.get_Current_ਸਰਗਰਮ_Region();
    ਜੇ (!region)
        ਵਾਪਸ;
    CV_Assert(region->pImpl);
    initTraceArg(ctx, arg);
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ OPENCV_WITH_ITT
    ਜੇ (isITTEnabled())
    {
        __itt_metadata_add(domain, region->pImpl->itt_id, (*arg.ppExtra)->ittHandle_name, ਆਕਾਰ(ਪੂਰਨ) == 4 ? __itt_metadata_s32 : __itt_metadata_s64, 1, &value);
    }
#ਨਹੀਂਤਾਂ
    CV_ਅਣਵਰਤਿਆ(value);
#ਜੇ_ਅੰਤ
}
ਖਾਲੀ traceArg(ਅਚਲ TraceArg& arg, int64 value)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    Region* region = ctx.get_Current_ਸਰਗਰਮ_Region();
    ਜੇ (!region)
        ਵਾਪਸ;
    CV_Assert(region->pImpl);
    initTraceArg(ctx, arg);
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ OPENCV_WITH_ITT
    ਜੇ (isITTEnabled())
    {
        __itt_metadata_add(domain, region->pImpl->itt_id, (*arg.ppExtra)->ittHandle_name, __itt_metadata_s64, 1, &value);
    }
#ਨਹੀਂਤਾਂ
    CV_ਅਣਵਰਤਿਆ(value);
#ਜੇ_ਅੰਤ
}
ਖਾਲੀ traceArg(ਅਚਲ TraceArg& arg, ਦੁੱਗਣਾ value)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    Region* region = ctx.get_Current_ਸਰਗਰਮ_Region();
    ਜੇ (!region)
        ਵਾਪਸ;
    CV_Assert(region->pImpl);
    initTraceArg(ctx, arg);
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ OPENCV_WITH_ITT
    ਜੇ (isITTEnabled())
    {
        __itt_metadata_add(domain, region->pImpl->itt_id, (*arg.ppExtra)->ittHandle_name, __itt_metadata_double, 1, &value);
    }
#ਨਹੀਂਤਾਂ
    CV_ਅਣਵਰਤਿਆ(value);
#ਜੇ_ਅੰਤ
}

#ਨਹੀਂਤਾਂ

Region::Region(ਅਚਲ LocationStaticStorage&) : pImpl(ਨਿਰਾ), implFlags(0) {}
ਖਾਲੀ Region::destroy() {}

ਖਾਲੀ traceArg(ਅਚਲ TraceArg&, ਅਚਲ ਅੱਖਰ*) {}
ਖਾਲੀ traceArg(ਅਚਲ TraceArg&, ਪੂਰਨ) {};
ਖਾਲੀ traceArg(ਅਚਲ TraceArg&, int64) {};
ਖਾਲੀ traceArg(ਅਚਲ TraceArg&, ਦੁੱਗਣਾ) {};

#ਜੇ_ਅੰਤ

}}}} // namespace
