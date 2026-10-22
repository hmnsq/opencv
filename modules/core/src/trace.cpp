// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ಅನ್ತರ್ಭಾವ "precomp.hpp"

#ಅನ್ತರ್ಭಾವ <opencv2/core/utils/trace.hpp>
#ಅನ್ತರ್ಭಾವ <opencv2/core/utils/trace.private.hpp>
#ಅನ್ತರ್ಭಾವ <opencv2/core/utils/configuration.private.hpp>

#ಅನ್ತರ್ಭಾವ <opencv2/core/opencl/ocl_defs.hpp>

#ಅನ್ತರ್ಭಾವ <cstdarg> // va_start

#ಅನ್ತರ್ಭಾವ <sstream>
#ಅನ್ತರ್ಭಾವ <ostream>
#ಅನ್ತರ್ಭಾವ <fstream>

#ಆದರೆ 0
#ವ್ಯಾಖ್ಯಾನಿಸು CV_LOG(...) CV_LOG_INFO(ಖಾಲಿ, __VA_ARGS__)
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು CV_LOG(...) {}
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ 0
#ವ್ಯಾಖ್ಯಾನಿಸು CV_LOG_ITT(...) CV_LOG_INFO(ಖಾಲಿ, __VA_ARGS__)
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು CV_LOG_ITT(...) {}
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ 1
#ವ್ಯಾಖ್ಯಾನಿಸು CV_LOG_TRACE_BAILOUT(ಟ್ಯಾಗ್, ...) CV_LOG_INFO(ಟ್ಯಾಗ್, __VA_ARGS__)
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು CV_LOG_TRACE_BAILOUT(...) {}
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ 0
#ವ್ಯಾಖ್ಯಾನಿಸು CV_LOG_PARALLEL(ಟ್ಯಾಗ್, ...) CV_LOG_INFO(ಟ್ಯಾಗ್, __VA_ARGS__)
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು CV_LOG_PARALLEL(...) {}
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ 0
#ವ್ಯಾಖ್ಯಾನಿಸು CV_LOG_CTX_STAT(ಟ್ಯಾಗ್, ...) CV_LOG_INFO(ಟ್ಯಾಗ್, __VA_ARGS__)
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು CV_LOG_CTX_STAT(...) {}
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ 0
#ವ್ಯಾಖ್ಯಾನಿಸು CV_LOG_SKIP(ಟ್ಯಾಗ್, ...) CV_LOG_INFO(ಟ್ಯಾಗ್, __VA_ARGS__)
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು CV_LOG_SKIP(...) {}
#ಆದರೆ_ಅಂತ್ಯ

ನಾಮಾಕಾಶ cv {
ನಾಮಾಕಾಶ utils {
ನಾಮಾಕಾಶ trace {
ನಾಮಾಕಾಶ details {

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ OPENCV_TRACE

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ _MSC_VER
#ಸೂಚನೆ warning(disable:4065) // switch statement contains 'default' but no 'case' labels
#ಆದರೆ_ಅಂತ್ಯ

ಸ್ಥಿರ ತಾರ್ಕಿಕ getParameterTraceEnable()
{
    ಸ್ಥಿರ ತಾರ್ಕಿಕ param_traceEnable = utils::getConfigurationParameterBool("OPENCV_TRACE", ಸುಳ್ಳು);
    ಹಿಂತಿರುಗಿ param_traceEnable;
}

// TODO lazy configuration flags
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ param_max_Region_ಆಳ_Open_CV = (ಪೂರ್ಣಾಂಕ)utils::get_Configuration_Parameter_ಗಾತ್ರ_T("OPENCV_TRACE_DEPTH_OPENCV", 1);
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ param_maxRegionChildrenOpenCV = (ಪೂರ್ಣಾಂಕ)utils::get_Configuration_Parameter_ಗಾತ್ರ_T("OPENCV_TRACE_MAX_CHILDREN_OPENCV", 1000);
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ param_maxRegionChildren = (ಪೂರ್ಣಾಂಕ)utils::get_Configuration_Parameter_ಗಾತ್ರ_T("OPENCV_TRACE_MAX_CHILDREN", 10000);

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ cv::String& getParameterTraceLocation()
{
    ಸ್ಥಿರ cv::String param_traceLocation = utils::getConfigurationParameterString("OPENCV_TRACE_LOCATION", "OpenCVTrace");
    ಹಿಂತಿರುಗಿ param_traceLocation;
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_OPENCL
ಸ್ಥಿರ ತಾರ್ಕಿಕ param_synchronizeOpenCL = utils::getConfigurationParameterBool("OPENCV_TRACE_SYNC_OPENCL", ಸುಳ್ಳು);
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ OPENCV_WITH_ITT
ಸ್ಥಿರ ತಾರ್ಕಿಕ param_ITT_ರಿಜಿಸ್ಟರ್_ಪೋಷಕ_Scope = utils::getConfigurationParameterBool("OPENCV_TRACE_ITT_PARENT", ಸುಳ್ಳು);
#ಆದರೆ_ಅಂತ್ಯ

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* _spaces(ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_3)
{
    ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ buf[64] =
"                                                               ";
    ಹಿಂತಿರುಗಿ &buf[63 - (ಎಣಿಕೆ_3 & 63)];
}

/**
 * Text-based trace messages
 */
ವರ್ಗ Trace_ಸಂದೇಶ
{
ಸಾರ್ವಜನಿಕ:
    ಅಕ್ಷರ ಬಫರ್[1024];
    size_t len;
    ತಾರ್ಕಿಕ hasError;

    Trace_ಸಂದೇಶ() :
        len(0),
        hasError(ಸುಳ್ಳು)
    {}

    ತಾರ್ಕಿಕ ಬರೆ(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* format, ...)
    {
        ಅಕ್ಷರ* buf = &ಬಫರ್[len];
        size_t sz = ಗಾತ್ರ(ಬಫರ್) - len;
        va_list ap;
        va_start(ap, format);
        ಪೂರ್ಣಾಂಕ n = cv_vsnprintf(buf, (ಪೂರ್ಣಾಂಕ)sz, format, ap);
        va_end(ap);
        ಆದರೆ (n < 0 || (size_t)n > sz)
        {
            hasError = ನಿಜ;
            ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
        }
        len += n;
        ಹಿಂತಿರುಗಿ ನಿಜ;
    }

    ತಾರ್ಕಿಕ formatlocation(ಸ್ಥಿರಾಂಕ Region::LocationStaticStorage& location)
    {
        ಹಿಂತಿರುಗಿ ಇದು->ಬರೆ("l,%lld,\"%s\",%d,\"%s\",0x%llX\n",
                (ದೀರ್ಘ ದೀರ್ಘ ಪೂರ್ಣಾಂಕ)(*location.ppExtra)->global_location_id,
                location.filename,
                location.ಸಾಲು,
                location.name,
                (ದೀರ್ಘ ದೀರ್ಘ ಪೂರ್ಣಾಂಕ)(location.flags & ~0xF0000000));
    }
    ತಾರ್ಕಿಕ formatRegionEnter(ಸ್ಥಿರಾಂಕ Region& region)
    {
        ತಾರ್ಕಿಕ ok = ಇದು->ಬರೆ("b,%d,%lld,%lld,%lld",
                (ಪೂರ್ಣಾಂಕ)region.pImpl->threadID,
                (ದೀರ್ಘ ದೀರ್ಘ ಪೂರ್ಣಾಂಕ)region.pImpl->beginTimestamp,
                (ದೀರ್ಘ ದೀರ್ಘ ಪೂರ್ಣಾಂಕ)((*region.pImpl->location.ppExtra)->global_location_id),
                (ದೀರ್ಘ ದೀರ್ಘ ಪೂರ್ಣಾಂಕ)region.pImpl->global_region_id);
        ಆದರೆ (region.pImpl->ಪೋಷಕ_Region && region.pImpl->ಪೋಷಕ_Region->pImpl)
        {
            ಆದರೆ (region.pImpl->ಪೋಷಕ_Region->pImpl->threadID != region.pImpl->threadID)
                ok &= ಇದು->ಬರೆ(",parentThread=%d,parent=%lld",
                        (ಪೂರ್ಣಾಂಕ)region.pImpl->ಪೋಷಕ_Region->pImpl->threadID,
                        (ದೀರ್ಘ ದೀರ್ಘ ಪೂರ್ಣಾಂಕ)region.pImpl->ಪೋಷಕ_Region->pImpl->global_region_id);
        }
        ok &= ಇದು->ಬರೆ("\n");
        ಹಿಂತಿರುಗಿ ok;
    }
    ತಾರ್ಕಿಕ formatRegionLeave(ಸ್ಥಿರಾಂಕ Region& region, ಸ್ಥಿರಾಂಕ RegionStatistics& ಫಲಿತಾಂಶ)
    {
        CV_DbgAssert(region.pImpl->ಕೊನೆ_Timestamp - region.pImpl->beginTimestamp == ಫಲಿತಾಂಶ.duration);
        ತಾರ್ಕಿಕ ok = ಇದು->ಬರೆ("e,%d,%lld,%lld,%lld,%lld",
                (ಪೂರ್ಣಾಂಕ)region.pImpl->threadID,
                (ದೀರ್ಘ ದೀರ್ಘ ಪೂರ್ಣಾಂಕ)region.pImpl->ಕೊನೆ_Timestamp,
                (ದೀರ್ಘ ದೀರ್ಘ ಪೂರ್ಣಾಂಕ)(*region.pImpl->location.ppExtra)->global_location_id,
                (ದೀರ್ಘ ದೀರ್ಘ ಪೂರ್ಣಾಂಕ)region.pImpl->global_region_id,
                (ದೀರ್ಘ ದೀರ್ಘ ಪೂರ್ಣಾಂಕ)ಫಲಿತಾಂಶ.duration);
        ಆದರೆ (ಫಲಿತಾಂಶ.currentSkippedRegions)
            ok &= ಇದು->ಬರೆ(",skip=%d", (ಪೂರ್ಣಾಂಕ)ಫಲಿತಾಂಶ.currentSkippedRegions);
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_IPP
        ಆದರೆ (ಫಲಿತಾಂಶ.durationImplIPP)
            ok &= ಇದು->ಬರೆ(",tIPP=%lld", (ದೀರ್ಘ ದೀರ್ಘ ಪೂರ್ಣಾಂಕ)ಫಲಿತಾಂಶ.durationImplIPP);
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_OPENCL
        ಆದರೆ (ಫಲಿತಾಂಶ.durationImplOpenCL)
            ok &= ಇದು->ಬರೆ(",tOCL=%lld", (ದೀರ್ಘ ದೀರ್ಘ ಪೂರ್ಣಾಂಕ)ಫಲಿತಾಂಶ.durationImplOpenCL);
#ಆದರೆ_ಅಂತ್ಯ
        ok &= ಇದು->ಬರೆ("\n");
        ಹಿಂತಿರುಗಿ ok;
    }
    ತಾರ್ಕಿಕ ದಾಖಲೆ_Region_Arg(ಸ್ಥಿರಾಂಕ Region& region, ಸ್ಥಿರಾಂಕ TraceArg& arg, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* value)
    {
        ಹಿಂತಿರುಗಿ ಇದು->ಬರೆ("a,%d,%lld,%lld,\"%s\",\"%s\"\n",
                region.pImpl->threadID,
                (ದೀರ್ಘ ದೀರ್ಘ ಪೂರ್ಣಾಂಕ)region.pImpl->beginTimestamp,
                (ದೀರ್ಘ ದೀರ್ಘ ಪೂರ್ಣಾಂಕ)region.pImpl->global_region_id,
                arg.name,
                value);
    }
};


#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ OPENCV_WITH_ITT
ಸ್ಥಿರ __itt_domain* domain = ಖಾಲಿ;

ಸ್ಥಿರ ತಾರ್ಕಿಕ isITTEnabled()
{
    ಸ್ಥಿರ ಅಸ್ಥಿರ ತಾರ್ಕಿಕ isInitialized = ಸುಳ್ಳು;
    ಸ್ಥಿರ ತಾರ್ಕಿಕ isEnabled = ಸುಳ್ಳು;
    ಆದರೆ (!isInitialized)
    {
        cv::Auto_ಬೀಗ lock(cv::getInitializationMutex());
        ಆದರೆ (!isInitialized)
        {
            ತಾರ್ಕಿಕ param_traceITTEnable = utils::getConfigurationParameterBool("OPENCV_TRACE_ITT_ENABLE", ನಿಜ);
            ಆದರೆ (param_traceITTEnable)
            {
                isEnabled = !!(__itt_api_version());
                CV_LOG_ITT("ITT is " << (isEnabled ? "enabled" : "disabled"));
                domain = __itt_domain_create("OpenCVTrace");
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                CV_LOG_ITT("ITT is disabled through OpenCV parameter");
                isEnabled = ಸುಳ್ಳು;
            }
            isInitialized = ನಿಜ;
        }
    }
    ಹಿಂತಿರುಗಿ isEnabled;
}
#ಆದರೆ_ಅಂತ್ಯ


Region::Location_Extra_ದತ್ತಾಂಶ::Location_Extra_ದತ್ತಾಂಶ(ಸ್ಥಿರಾಂಕ LocationStaticStorage& location)
{
    CV_ಬಳಸದ(location);
    ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ g_location_id_counter = 0;
    global_location_id = CV_XADD(&g_location_id_counter, 1) + 1;
    CV_LOG("Register location: " << global_location_id << " (" << (ಶೂನ್ಯ*)&location << ")"
            << std::ಸಾಲು_ಅಂತ್ಯ << "    file: " << location.filename
            << std::ಸಾಲು_ಅಂತ್ಯ << "    line: " << location.ಸಾಲು
            << std::ಸಾಲು_ಅಂತ್ಯ << "    name: " << location.name);
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ OPENCV_WITH_ITT
    ಆದರೆ (isITTEnabled())
    {
        // Caching is not required here, because there is builtin cache.
        // https://software.intel.com/en-us/node/544203:
        //     Consecutive calls to __itt_string_handle_create with the same name return the same value.
        ittHandle_name = __itt_string_handle_create(location.name);
        ittHandle_filename = __itt_string_handle_create(location.filename);
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        ittHandle_name = 0;
        ittHandle_filename = 0;
    }
#ಆದರೆ_ಅಂತ್ಯ
}

/*static*/ Region::Location_Extra_ದತ್ತಾಂಶ* Region::Location_Extra_ದತ್ತಾಂಶ::init(ಸ್ಥಿರಾಂಕ Region::LocationStaticStorage& location)
{
    Location_Extra_ದತ್ತಾಂಶ** pLocationExtra = location.ppExtra;
    CV_DbgAssert(pLocationExtra);
    ಆದರೆ (*pLocationExtra == ಖಾಲಿ)
    {
        cv::Auto_ಬೀಗ lock(cv::getInitializationMutex());
        ಆದರೆ (*pLocationExtra == ಖಾಲಿ)
        {
            *pLocationExtra = ಹೊಸ Region::Location_Extra_ದತ್ತಾಂಶ(location);
            TraceStorage* s = getTraceManager().trace_storage.get();
            ಆದರೆ (s)
            {
                Trace_ಸಂದೇಶ ಸಂದೇಶ;
                ಸಂದೇಶ.formatlocation(location);
                s->put(ಸಂದೇಶ);
            }
        }
    }
    ಹಿಂತಿರುಗಿ *pLocationExtra;
}


Region::Impl::Impl(TraceManagerThreadLocal& ctx, Region* ಪೋಷಕ_Region_2, Region& region_, ಸ್ಥಿರಾಂಕ LocationStaticStorage& location_, int64 beginTimestamp_) :
    location(location_),
    region(region_),
    ಪೋಷಕ_Region(ಪೋಷಕ_Region_2),
    threadID(ctx.threadID),
    global_region_id(++ctx.region_counter),
    beginTimestamp(beginTimestamp_),
    ಕೊನೆ_Timestamp(0),
    ನೇರ_Children_ಎಣಿಕೆ(0)
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ OPENCV_WITH_ITT
    ,itt_id_registered(ಸುಳ್ಳು)
    ,itt_id(__itt_null)
#ಆದರೆ_ಅಂತ್ಯ
{
    CV_DbgAssert(ctx.current_ಸಕ್ರಿಯ_Region == ಪೋಷಕ_Region);
    region.pImpl = ಇದು;

    ರಿಜಿಸ್ಟರ್_Region(ctx);

    enterRegion(ctx);
}

Region::Impl::~Impl()
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ OPENCV_WITH_ITT
    ಆದರೆ (itt_id_registered)
    {
        CV_LOG_ITT(" Destroy ITT region: I=" << (ಶೂನ್ಯ*)ಇದು);
        __itt_id_destroy(domain, itt_id);
        itt_id_registered = ಸುಳ್ಳು;
    }
#ಆದರೆ_ಅಂತ್ಯ
    region.pImpl = ಖಾಲಿ;
}

ಶೂನ್ಯ Region::Impl::enterRegion(TraceManagerThreadLocal& ctx)
{
    ctx.current_ಸಕ್ರಿಯ_Region = &region;

    ಆದರೆ (location.flags & REGION_FLAG_FUNCTION)
    {
        ಆದರೆ ((location.flags & REGION_FLAG_APP_ಸಂಕೇತ) == 0)
        {
            ctx.region_ಆಳ_Open_CV++;
        }
        ctx.region_ಆಳ++;
    }

    TraceStorage* s = ctx.getStorage();
    ಆದರೆ (s)
    {
        Trace_ಸಂದೇಶ ಸಂದೇಶ;
        ಸಂದೇಶ.formatRegionEnter(region);
        s->put(ಸಂದೇಶ);
    }
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ OPENCV_WITH_ITT
    ಆದರೆ (isITTEnabled())
    {
        __itt_id ಪೋಷಕ_ID = __itt_null;
        ಆದರೆ (param_ITT_ರಿಜಿಸ್ಟರ್_ಪೋಷಕ_Scope && ಪೋಷಕ_Region && ಪೋಷಕ_Region->pImpl && ಪೋಷಕ_Region->pImpl->itt_id_registered && (location.flags & REGION_FLAG_REGION_FORCE) == 0)
            ಪೋಷಕ_ID = ಪೋಷಕ_Region->pImpl->itt_id;
        __itt_task_begin(domain, itt_id, ಪೋಷಕ_ID, (*location.ppExtra)->ittHandle_name);
    }
#ಆದರೆ_ಅಂತ್ಯ
}

ಶೂನ್ಯ Region::Impl::leaveRegion(TraceManagerThreadLocal& ctx)
{
    int64 duration = ಕೊನೆ_Timestamp - beginTimestamp; CV_ಬಳಸದ(duration);
    RegionStatistics ಫಲಿತಾಂಶ;
    ctx.stat.grab(ಫಲಿತಾಂಶ);
    ctx.ಒಟ್ಟು_Skipped_Events += ಫಲಿತಾಂಶ.currentSkippedRegions;
    CV_LOG(_spaces(ctx.get_Current_ಆಳ()*4) << "leaveRegion(): " << (ಶೂನ್ಯ*)ಇದು << " " << ಫಲಿತಾಂಶ);
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ OPENCV_WITH_ITT
    ಆದರೆ (isITTEnabled())
    {
        ಆದರೆ (ಫಲಿತಾಂಶ.currentSkippedRegions)
        {
            __itt_metadata_add(domain, itt_id, __itt_string_handle_create("skipped trace entries"), __itt_metadata_u32, 1, &ಫಲಿತಾಂಶ.currentSkippedRegions);
        }
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_IPP
        ಆದರೆ (ಫಲಿತಾಂಶ.durationImplIPP)
            __itt_metadata_add(domain, itt_id, __itt_string_handle_create("tIPP"), __itt_metadata_u64, 1, &ಫಲಿತಾಂಶ.durationImplIPP);
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_OPENCL
        ಆದರೆ (ಫಲಿತಾಂಶ.durationImplOpenCL)
            __itt_metadata_add(domain, itt_id, __itt_string_handle_create("tOpenCL"), __itt_metadata_u64, 1, &ಫಲಿತಾಂಶ.durationImplOpenCL);
#ಆದರೆ_ಅಂತ್ಯ
        __itt_task_end(domain);
    }
#ಆದರೆ_ಅಂತ್ಯ
    TraceStorage* s = ctx.getStorage();
    ಆದರೆ (s)
    {
        Trace_ಸಂದೇಶ ಸಂದೇಶ;
        ಸಂದೇಶ.formatRegionLeave(region, ಫಲಿತಾಂಶ);
        s->put(ಸಂದೇಶ);
    }

    ಆದರೆ (location.flags & REGION_FLAG_FUNCTION)
    {
        ಆದರೆ ((location.flags & REGION_FLAG_APP_ಸಂಕೇತ) == 0)
        {
            ctx.region_ಆಳ_Open_CV--;
        }
        ctx.region_ಆಳ--;
    }

    ctx.current_ಸಕ್ರಿಯ_Region = ಪೋಷಕ_Region;
}

ಶೂನ್ಯ Region::Impl::release()
{
    ಅಳಿಸು ಇದು;
}

ಶೂನ್ಯ Region::Impl::ರಿಜಿಸ್ಟರ್_Region(TraceManagerThreadLocal& ctx)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ OPENCV_WITH_ITT
    ಆದರೆ (isITTEnabled())
    {
        ಆದರೆ (!itt_id_registered)
        {
            CV_LOG_ITT(" Register ITT region: I=" << (ಶೂನ್ಯ*)ಇದು << " " << ctx.threadID << "-" << global_region_id);
#ಆದರೆ 1 // workaround for some ITT backends
            itt_id = __itt_id_make((ಶೂನ್ಯ*)(intptr_t)(((int64)(ctx.threadID + 1) << 32) | global_region_id), global_region_id);
#ಇಲ್ಲದಿದ್ದರೆ
            itt_id = __itt_id_make((ಶೂನ್ಯ*)(intptr_t)(ctx.threadID + 1), global_region_id);
#ಆದರೆ_ಅಂತ್ಯ
            __itt_id_create(domain, itt_id);
            itt_id_registered = ನಿಜ;
        }
    }
#ಇಲ್ಲದಿದ್ದರೆ
    CV_ಬಳಸದ(ctx);
#ಆದರೆ_ಅಂತ್ಯ
}

ಶೂನ್ಯ Region_Statistics_ಸ್ಥಿತಿ::enable_Skip_ಬಹುಲಕ(ಪೂರ್ಣಾಂಕ ಆಳ)
{
    CV_DbgAssert(skip_ಆಳ < 0);
    CV_LOG_SKIP(ಖಾಲಿ, "SKIP-ENABLE: depth=" << ಆಳ);
    skip_ಆಳ = ಆಳ;
}
ಶೂನ್ಯ Region_Statistics_ಸ್ಥಿತಿ::check_Reset_Skip_ಬಹುಲಕ(ಪೂರ್ಣಾಂಕ leave_ಆಳ)
{
    ಆದರೆ (leave_ಆಳ <= skip_ಆಳ)
    {
        CV_LOG_SKIP(ಖಾಲಿ, "SKIP-RESET: leaveDepth=" << leave_ಆಳ << " skipDepth=" << skip_ಆಳ);
        skip_ಆಳ = -1;
    }
}

Region::Region(ಸ್ಥಿರಾಂಕ LocationStaticStorage& location) :
    pImpl(ಖಾಲಿ),
    implFlags(0)
{
    // Checks:
    // - global enable flag
    // - parent region is disabled
    // - children count threshold
    // - region location
    // - depth (opencv nested calls)
    ಆದರೆ (!TraceManager::isActivated())
    {
        CV_LOG("Trace is disabled. Bailout");
        ಹಿಂತಿರುಗಿ;
    }

    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    CV_LOG(_spaces(ctx.get_Current_ಆಳ()*4) << "Region(): " << (ಶೂನ್ಯ*)ಇದು << ": " << location.name);

    Region* ಪೋಷಕ_Region = ctx.ರಾಶಿ_ತುದಿ_Region();
    ಸ್ಥಿರಾಂಕ Region::LocationStaticStorage* ಪೋಷಕ_Location = ctx.ರಾಶಿ_ತುದಿ_Location();

    ಆದರೆ (location.flags & REGION_FLAG_REGION_ಮುಂದಿನ)
    {
        ಆದರೆ (ಪೋಷಕ_Region && ಪೋಷಕ_Region->pImpl)
        {
            CV_DbgAssert((ಪೋಷಕ_Region->pImpl->location.flags & REGION_FLAG_FUNCTION) == 0);
            ಪೋಷಕ_Region->destroy(); ಪೋಷಕ_Region->implFlags = 0;
            ಪೋಷಕ_Region = ctx.ರಾಶಿ_ತುದಿ_Region();
            ಪೋಷಕ_Location = ctx.ರಾಶಿ_ತುದಿ_Location();
        }
    }

    ಪೂರ್ಣಾಂಕ ಪೋಷಕ_Children = 0;
    ಆದರೆ (ಪೋಷಕ_Region && ಪೋಷಕ_Region->pImpl)
    {
        ಆದರೆ (ಪೋಷಕ_Location == ಖಾಲಿ)
        {
            // parallel_for_body code path
            ಪೋಷಕ_Children = CV_XADD(&ಪೋಷಕ_Region->pImpl->ನೇರ_Children_ಎಣಿಕೆ, 1) + 1;
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            ಪೋಷಕ_Children = ++ಪೋಷಕ_Region->pImpl->ನೇರ_Children_ಎಣಿಕೆ;
        }
    }

    int64 beginTimestamp = getTimestampNS();

    ಪೂರ್ಣಾಂಕ current_ಆಳ = ctx.get_Current_ಆಳ() + 1;
    ಆಯ್ಕೆ (location.flags & REGION_FLAG_IMPL_ಮುಖವಾಡ)
    {
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_IPP
    ಸ್ಥಿತಿ REGION_FLAG_IMPL_IPP:
        ಆದರೆ (!ctx.stat_ಸ್ಥಿತಿ.ignore_ಆಳ_Impl_IPP)
            ctx.stat_ಸ್ಥಿತಿ.ignore_ಆಳ_Impl_IPP = current_ಆಳ;
        ನಿಲ್ಲಿಸು;
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_OPENCL
    ಸ್ಥಿತಿ REGION_FLAG_IMPL_OPENCL:
        ಆದರೆ (!ctx.stat_ಸ್ಥಿತಿ.ignore_ಆಳ_Impl_Open_CL)
            ctx.stat_ಸ್ಥಿತಿ.ignore_ಆಳ_Impl_Open_CL = current_ಆಳ;
        ನಿಲ್ಲಿಸು;
#ಆದರೆ_ಅಂತ್ಯ
    ಪೂರ್ವನಿಯೋಜಿತ:
        ನಿಲ್ಲಿಸು;
    }

    ctx.ರಾಶಿ_ತಳ್ಳು(ಇದು, &location, beginTimestamp);
    implFlags |= REGION_FLAG_NEED_ರಾಶಿ_ತೆಗೆ;

    ಆದರೆ ((location.flags & REGION_FLAG_REGION_FORCE) == 0)
    {
        ಆದರೆ (ctx.stat_ಸ್ಥಿತಿ.skip_ಆಳ >= 0 && current_ಆಳ > ctx.stat_ಸ್ಥಿತಿ.skip_ಆಳ)
        {
            CV_LOG(_spaces(ctx.get_Current_ಆಳ()*4) << "Parent region is disabled. Bailout");
            ctx.stat.currentSkippedRegions++;
            ಹಿಂತಿರುಗಿ;
        }

        ಆದರೆ (param_maxRegionChildrenOpenCV > 0 && (location.flags & REGION_FLAG_APP_ಸಂಕೇತ) == 0 && ಪೋಷಕ_Location && (ಪೋಷಕ_Location->flags & REGION_FLAG_APP_ಸಂಕೇತ) == 0)
        {
            ಆದರೆ (ಪೋಷಕ_Children >= param_maxRegionChildrenOpenCV)
            {
                CV_LOG_TRACE_BAILOUT(ಖಾಲಿ, _spaces(ctx.get_Current_ಆಳ()*4) << "OpenCV parent region exceeds children count. Bailout");
                ctx.stat_ಸ್ಥಿತಿ.enable_Skip_ಬಹುಲಕ(current_ಆಳ - 1);
                ctx.stat.currentSkippedRegions++;
                DEBUG_ONLY(ctx.dump_ರಾಶಿ(std::ಹೊರಗೆ, ಸುಳ್ಳು));
                ಹಿಂತಿರುಗಿ;
            }
        }
        ಆದರೆ (param_maxRegionChildren > 0 && ಪೋಷಕ_Children >= param_maxRegionChildren)
        {
            CV_LOG_TRACE_BAILOUT(ಖಾಲಿ, _spaces(ctx.get_Current_ಆಳ()*4) << "Parent region exceeds children count. Bailout");
            ctx.stat_ಸ್ಥಿತಿ.enable_Skip_ಬಹುಲಕ(current_ಆಳ - 1);
            ctx.stat.currentSkippedRegions++;
            DEBUG_ONLY(ctx.dump_ರಾಶಿ(std::ಹೊರಗೆ, ಸುಳ್ಳು));
            ಹಿಂತಿರುಗಿ;
        }
    }

    Location_Extra_ದತ್ತಾಂಶ::init(location);

    ಆದರೆ ((*location.ppExtra)->global_location_id == 0)
    {
        CV_LOG_TRACE_BAILOUT(ಖಾಲಿ, _spaces(ctx.get_Current_ಆಳ()*4) << "Region location is disabled. Bailout");
        ctx.stat_ಸ್ಥಿತಿ.enable_Skip_ಬಹುಲಕ(current_ಆಳ);
        ctx.stat.currentSkippedRegions++;
        ಹಿಂತಿರುಗಿ;
    }

    ಆದರೆ (ಪೋಷಕ_Location && (ಪೋಷಕ_Location->flags & REGION_FLAG_SKIP_NESTED))
    {
        CV_LOG(_spaces(ctx.get_Current_ಆಳ()*4) << "Parent region disables inner regions. Bailout");
        ctx.stat_ಸ್ಥಿತಿ.enable_Skip_ಬಹುಲಕ(current_ಆಳ);
        ctx.stat.currentSkippedRegions++;
        ಹಿಂತಿರುಗಿ;
    }

    ಆದರೆ (param_max_Region_ಆಳ_Open_CV)
    {
        ಆದರೆ ((location.flags & REGION_FLAG_APP_ಸಂಕೇತ) == 0)
        {
            ಆದರೆ (ctx.region_ಆಳ_Open_CV >= param_max_Region_ಆಳ_Open_CV)
            {
                CV_LOG(_spaces(ctx.get_Current_ಆಳ()*4) << "OpenCV region depth is exceed = " << ctx.region_ಆಳ_Open_CV << ". Bailout");
                ಆದರೆ (ctx.stat.currentSkippedRegions == 0)
                {
                    DEBUG_ONLY(ctx.dump_ರಾಶಿ(std::ಹೊರಗೆ, ಸುಳ್ಳು));
                }
                ctx.stat_ಸ್ಥಿತಿ.enable_Skip_ಬಹುಲಕ(current_ಆಳ);
                ctx.stat.currentSkippedRegions++;
                ಹಿಂತಿರುಗಿ;
            }
        }
    }

    ಹೊಸ Impl(ctx, ಪೋಷಕ_Region, *ಇದು, location, beginTimestamp);
    CV_DbgAssert(pImpl != ಖಾಲಿ);
    implFlags |= REGION_FLAG_ಸಕ್ರಿಯ;

    // parallel_for path
    ಆದರೆ (ಪೋಷಕ_Region && ಪೋಷಕ_Region->pImpl)
    {
        ಆದರೆ (ಪೋಷಕ_Location == ಖಾಲಿ)
        {
            pImpl->ನೇರ_Children_ಎಣಿಕೆ = ಪೋಷಕ_Children;
        }
    }
}

ಶೂನ್ಯ Region::destroy()
{
    CV_DbgAssert(implFlags != 0);

    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    CV_LOG(_spaces(ctx.get_Current_ಆಳ()*4) << "Region::destruct(): " << (ಶೂನ್ಯ*)ಇದು << " pImpl=" << pImpl << " implFlags=" << implFlags << ' ' << (ctx.ರಾಶಿ_ತುದಿ_Location() ? ctx.ರಾಶಿ_ತುದಿ_Location()->name : "<unknown>"));

    CV_DbgAssert(implFlags & REGION_FLAG_NEED_ರಾಶಿ_ತೆಗೆ);
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ current_ಆಳ = ctx.get_Current_ಆಳ(); CV_ಬಳಸದ(current_ಆಳ);

    CV_LOG_CTX_STAT(ಖಾಲಿ, _spaces(current_ಆಳ*4) << ctx.stat << ' ' << ctx.stat_ಸ್ಥಿತಿ);

    ಸ್ಥಿರಾಂಕ Region::LocationStaticStorage* location = ctx.ರಾಶಿ_ತುದಿ_Location();
    Impl::Optimization_ಮಾರ್ಗ my_ಸಂಕೇತ_ಮಾರ್ಗ = Impl::ಸಂಕೇತ_ಮಾರ್ಗ_PLAIN;
    ಆದರೆ (location)
    {
        ಆಯ್ಕೆ (location->flags & REGION_FLAG_IMPL_ಮುಖವಾಡ)
        {
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_IPP
        ಸ್ಥಿತಿ REGION_FLAG_IMPL_IPP:
            my_ಸಂಕೇತ_ಮಾರ್ಗ = Impl::ಸಂಕೇತ_ಮಾರ್ಗ_IPP;
            ನಿಲ್ಲಿಸು;
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_OPENCL
        ಸ್ಥಿತಿ REGION_FLAG_IMPL_OPENCL:
            ಆದರೆ (param_synchronizeOpenCL && cv::ocl::isOpenCLActivated())
                cv::ocl::ಮುಕ್ತಾಯ_2();
            my_ಸಂಕೇತ_ಮಾರ್ಗ = Impl::ಸಂಕೇತ_ಮಾರ್ಗ_OPENCL;
            ನಿಲ್ಲಿಸು;
#ಆದರೆ_ಅಂತ್ಯ
        ಪೂರ್ವನಿಯೋಜಿತ:
            ನಿಲ್ಲಿಸು;
        }
    }

    int64 ಕೊನೆ_Timestamp = getTimestampNS();
    int64 duration = ಕೊನೆ_Timestamp - ctx.ರಾಶಿ_ತುದಿ_Begin_Timestamp();

    ತಾರ್ಕಿಕ ಸಕ್ರಿಯ = is_ಸಕ್ರಿಯ_2();

    ಆದರೆ (ಸಕ್ರಿಯ)
        ctx.stat.duration = duration;
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ctx.ರಾಶಿ.size() == ctx.parallel_for_ರಾಶಿ_ಗಾತ್ರ + 1)
        ctx.stat.duration += duration;

    ಆಯ್ಕೆ (my_ಸಂಕೇತ_ಮಾರ್ಗ) {
        ಸ್ಥಿತಿ Impl::ಸಂಕೇತ_ಮಾರ್ಗ_PLAIN:
            // nothing
            ನಿಲ್ಲಿಸು;
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_IPP
        ಸ್ಥಿತಿ Impl::ಸಂಕೇತ_ಮಾರ್ಗ_IPP:
            ಆದರೆ (ctx.stat_ಸ್ಥಿತಿ.ignore_ಆಳ_Impl_IPP == current_ಆಳ)
            {
                ctx.stat.durationImplIPP += duration;
                ctx.stat_ಸ್ಥಿತಿ.ignore_ಆಳ_Impl_IPP = 0;
            }
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಸಕ್ರಿಯ)
            {
                ctx.stat.durationImplIPP = duration;
            }
            ನಿಲ್ಲಿಸು;
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_OPENCL
        ಸ್ಥಿತಿ Impl::ಸಂಕೇತ_ಮಾರ್ಗ_OPENCL:
            ಆದರೆ (ctx.stat_ಸ್ಥಿತಿ.ignore_ಆಳ_Impl_Open_CL == current_ಆಳ)
            {
                ctx.stat.durationImplOpenCL += duration;
                ctx.stat_ಸ್ಥಿತಿ.ignore_ಆಳ_Impl_Open_CL = 0;
            }
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಸಕ್ರಿಯ)
            {
                ctx.stat.durationImplOpenCL = duration;
            }
            ನಿಲ್ಲಿಸು;
#ಆದರೆ_ಅಂತ್ಯ
        ಪೂರ್ವನಿಯೋಜಿತ:
            ನಿಲ್ಲಿಸು;
    }

    ಆದರೆ (pImpl)
    {
        CV_DbgAssert((implFlags & (REGION_FLAG_ಸಕ್ರಿಯ | REGION_FLAG_NEED_ರಾಶಿ_ತೆಗೆ)) == (REGION_FLAG_ಸಕ್ರಿಯ | REGION_FLAG_NEED_ರಾಶಿ_ತೆಗೆ));
        CV_DbgAssert(ctx.ರಾಶಿ_ತುದಿ_Region() == ಇದು);
        pImpl->ಕೊನೆ_Timestamp = ಕೊನೆ_Timestamp;
        pImpl->leaveRegion(ctx);
        pImpl->release();
        pImpl = ಖಾಲಿ;
        DEBUG_ONLY(implFlags &= ~REGION_FLAG_ಸಕ್ರಿಯ);
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        CV_DbgAssert(ctx.stat_ಸ್ಥಿತಿ.skip_ಆಳ <= current_ಆಳ);
    }

    ಆದರೆ (implFlags & REGION_FLAG_NEED_ರಾಶಿ_ತೆಗೆ)
    {
        CV_DbgAssert(ctx.ರಾಶಿ_ತುದಿ_Region() == ಇದು);
        ctx.ರಾಶಿ_ತೆಗೆ();
        ctx.stat_ಸ್ಥಿತಿ.check_Reset_Skip_ಬಹುಲಕ(current_ಆಳ);
        DEBUG_ONLY(implFlags &= ~REGION_FLAG_NEED_ರಾಶಿ_ತೆಗೆ);
    }
    CV_LOG_CTX_STAT(ಖಾಲಿ, _spaces(current_ಆಳ*4) << "===> " << ctx.stat << ' ' << ctx.stat_ಸ್ಥಿತಿ);
}


TraceManagerThreadLocal::~TraceManagerThreadLocal()
{
}

ಶೂನ್ಯ TraceManagerThreadLocal::dump_ರಾಶಿ(std::ostream& out, ತಾರ್ಕಿಕ onlyFunctions) ಸ್ಥಿರಾಂಕ
{
    std::stringstream ss;
    std::deque<ರಾಶಿ_ನಮೂದು>::const_iterator it = ರಾಶಿ.begin();
    std::deque<ರಾಶಿ_ನಮೂದು>::const_iterator end = ರಾಶಿ.end();
    ಪೂರ್ಣಾಂಕ ಆಳ = 0;
    ಗಾಗಿ (; it != end; ++it)
    {
        ಸ್ಥಿರಾಂಕ Region::LocationStaticStorage* location = it->location;
        ಆದರೆ (location)
        {
            ಆದರೆ (!onlyFunctions || (location->flags & REGION_FLAG_FUNCTION))
            {
                ss << _spaces(4*ಆಳ) << location->name << std::ಸಾಲು_ಅಂತ್ಯ;
                ಆಳ++;
            }
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            ss << _spaces(4*ಆಳ) << "<unknown>" << std::ಸಾಲು_ಅಂತ್ಯ;
            ಆಳ++;
        }
    }
    out << ss.str();
}

ವರ್ಗ AsyncTraceStorage CV_FINAL : ಸಾರ್ವಜನಿಕ TraceStorage
{
    ಬದಲಾಯಿಸಬಹುದಾದ std::ofstream out;
ಸಾರ್ವಜನಿಕ:
    ಸ್ಥಿರಾಂಕ std::string name;

    AsyncTraceStorage(ಸ್ಥಿರಾಂಕ std::string& filename) :
        out(filename.c_str(), std::ios::trunc),
        name(filename)
    {
        out << "#description: OpenCV trace file" << std::ಸಾಲು_ಅಂತ್ಯ;
        out << "#version: 1.0" << std::ಸಾಲು_ಅಂತ್ಯ;
    }
    ~AsyncTraceStorage()
    {
        out.close();
    }

    ತಾರ್ಕಿಕ put(ಸ್ಥಿರಾಂಕ Trace_ಸಂದೇಶ& ಸಂದೇಶ) ಸ್ಥಿರಾಂಕ CV_OVERRIDE
    {
        ಆದರೆ (ಸಂದೇಶ.hasError)
            ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
        out << ಸಂದೇಶ.ಬಫರ್;
        //DEBUG_ONLY(std::flush(out)); // TODO configure flag
        ಹಿಂತಿರುಗಿ ನಿಜ;
    }
};

ವರ್ಗ SyncTraceStorage CV_FINAL : ಸಾರ್ವಜನಿಕ TraceStorage
{
    ಬದಲಾಯಿಸಬಹುದಾದ std::ofstream out;
    ಬದಲಾಯಿಸಬಹುದಾದ cv::Mutex mutex;
ಸಾರ್ವಜನಿಕ:
    ಸ್ಥಿರಾಂಕ std::string name;

    SyncTraceStorage(ಸ್ಥಿರಾಂಕ std::string& filename) :
        out(filename.c_str(), std::ios::trunc),
        name(filename)
    {
        out << "#description: OpenCV trace file" << std::ಸಾಲು_ಅಂತ್ಯ;
        out << "#version: 1.0" << std::ಸಾಲು_ಅಂತ್ಯ;
    }
    ~SyncTraceStorage()
    {
        cv::Auto_ಬೀಗ l(mutex);
        out.close();
    }

    ತಾರ್ಕಿಕ put(ಸ್ಥಿರಾಂಕ Trace_ಸಂದೇಶ& ಸಂದೇಶ) ಸ್ಥಿರಾಂಕ CV_OVERRIDE
    {
        ಆದರೆ (ಸಂದೇಶ.hasError)
            ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
        {
            cv::Auto_ಬೀಗ l(mutex);
            out << ಸಂದೇಶ.ಬಫರ್;
            std::flush(out); // TODO configure flag
        }
        ಹಿಂತಿರುಗಿ ನಿಜ;
    }
};


TraceStorage* TraceManagerThreadLocal::getStorage() ಸ್ಥಿರಾಂಕ
{
    // TODO configuration option for stdout/single trace file
    ಆದರೆ (storage.empty())
    {
        TraceStorage* global = getTraceManager().trace_storage.get();
        ಆದರೆ (global)
        {
            ಸ್ಥಿರಾಂಕ std::string filepath = cv::format("%s-%03d.txt", getParameterTraceLocation().c_str(), threadID).c_str();
            Trace_ಸಂದೇಶ ಸಂದೇಶ;
            ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* pos = strrchr(filepath.c_str(), '/'); // extract filename
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ _WIN32
            ಆದರೆ (!pos)
                pos = strrchr(filepath.c_str(), '\\');
#ಆದರೆ_ಅಂತ್ಯ
            ಆದರೆ (!pos)
                pos = filepath.c_str();
            ಇಲ್ಲದಿದ್ದರೆ
                pos += 1; // fix to skip extra slash in filename beginning
            ಸಂದೇಶ.ಬರೆ("#thread file: %s\n", pos);
            global->put(ಸಂದೇಶ);
            storage.reset(ಹೊಸ AsyncTraceStorage(filepath));
        }
    }
    ಹಿಂತಿರುಗಿ storage.get();
}



ಸ್ಥಿರ ತಾರ್ಕಿಕ activated = ಸುಳ್ಳು;
ಸ್ಥಿರ ತಾರ್ಕಿಕ isInitialized = ಸುಳ್ಳು;

TraceManager::TraceManager()
{
    (ಶೂನ್ಯ)cv::getTimestampNS();

    isInitialized = ನಿಜ;
    CV_LOG("TraceManager ctor: " << (ಶೂನ್ಯ*)ಇದು);

    CV_LOG("TraceManager configure()");
    activated = getParameterTraceEnable();

    ಆದರೆ (activated)
        trace_storage.reset(ಹೊಸ SyncTraceStorage(std::string(getParameterTraceLocation()) + ".txt"));

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ OPENCV_WITH_ITT
    ಆದರೆ (isITTEnabled())
    {
        activated = ನಿಜ; // force trace pipeline activation (without OpenCV storage)
        __itt_region_begin(domain, __itt_null, __itt_null, __itt_string_handle_create("OpenCVTrace"));
    }
#ಆದರೆ_ಅಂತ್ಯ
}
TraceManager::~TraceManager()
{
    CV_LOG("TraceManager dtor: " << (ಶೂನ್ಯ*)ಇದು);

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ OPENCV_WITH_ITT
    ಆದರೆ (isITTEnabled())
    {
        __itt_region_end(domain, __itt_null);
    }
#ಆದರೆ_ಅಂತ್ಯ

    std::vector<TraceManagerThreadLocal*> ಎಳೆಗಳು_ctx;
    tls.gather(ಎಳೆಗಳು_ctx);
    size_t ಒಟ್ಟು_Events = 0, ಒಟ್ಟು_Skipped_Events = 0;
    ಗಾಗಿ (size_t i = 0; i < ಎಳೆಗಳು_ctx.size(); i++)
    {
        TraceManagerThreadLocal* ctx = ಎಳೆಗಳು_ctx[i];
        ಆದರೆ (ctx)
        {
            ಒಟ್ಟು_Events += ctx->region_counter;
            ಒಟ್ಟು_Skipped_Events += ctx->ಒಟ್ಟು_Skipped_Events;
        }
    }
    ಆದರೆ (ಒಟ್ಟು_Events || activated)
    {
        CV_LOG_INFO(ಖಾಲಿ, "Trace: Total events: " << ಒಟ್ಟು_Events);
    }
    ಆದರೆ (ಒಟ್ಟು_Skipped_Events)
    {
        CV_LOG_WARNING(ಖಾಲಿ, "Trace: Total skipped events: " << ಒಟ್ಟು_Skipped_Events);
    }

    // This is a global static object, so process starts shutdown here
    // Turn off trace
    cv::__termination = ನಿಜ; // also set in DllMain() notifications handler for DLL_PROCESS_DETACH
    activated = ಸುಳ್ಳು;
}

ತಾರ್ಕಿಕ TraceManager::isActivated()
{
    // Check if process starts shutdown, and set earlyExit to true
    // to prevent further instrumentation processing earlier.
    ಆದರೆ (cv::__termination)
    {
        activated = ಸುಳ್ಳು;
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
    }

    ಆದರೆ (!isInitialized)
    {
        TraceManager& m = getTraceManager();
        CV_ಬಳಸದ(m); // TODO
    }

    ಹಿಂತಿರುಗಿ activated;
}


ಸ್ಥಿರ TraceManager* getTraceManagerCallOnce()
{
    ಸ್ಥಿರ TraceManager globalInstance;
    ಹಿಂತಿರುಗಿ &globalInstance;
}
TraceManager& getTraceManager()
{
    CV_SINGLETON_LAZY_INIT_REF(TraceManager, getTraceManagerCallOnce())
}

ಶೂನ್ಯ parallel_For_ಗಣ_ಬೇರು_Region(ಸ್ಥಿರಾಂಕ Region& ಬೇರು_Region, ಸ್ಥಿರಾಂಕ TraceManagerThreadLocal& ಬೇರು_ctx)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();

    ಆದರೆ (ctx.dummy_ರಾಶಿ_ತುದಿ.region == &ಬೇರು_Region) // already attached
        ಹಿಂತಿರುಗಿ;

    CV_Assert(ctx.dummy_ರಾಶಿ_ತುದಿ.region == ಖಾಲಿ);
    ctx.dummy_ರಾಶಿ_ತುದಿ = TraceManagerThreadLocal::ರಾಶಿ_ನಮೂದು(const_cast<Region*>(&ಬೇರು_Region), ಖಾಲಿ, -1);

    ಆದರೆ (&ctx == &ಬೇರು_ctx)
    {
        ctx.stat.grab(ctx.parallel_for_stat);
        ctx.parallel_for_stat_ಸ್ಥಿತಿ = ctx.stat_ಸ್ಥಿತಿ;
        ctx.parallel_for_ರಾಶಿ_ಗಾತ್ರ = ctx.ರಾಶಿ.size();
        ಹಿಂತಿರುಗಿ;
    }

    CV_Assert(ctx.ರಾಶಿ.empty());

    ctx.current_ಸಕ್ರಿಯ_Region = const_cast<Region*>(&ಬೇರು_Region);

    ctx.region_ಆಳ = ಬೇರು_ctx.region_ಆಳ;
    ctx.region_ಆಳ_Open_CV = ಬೇರು_ctx.region_ಆಳ_Open_CV;

    ctx.parallel_for_ರಾಶಿ_ಗಾತ್ರ = 0;

    ctx.stat_ಸ್ಥಿತಿ.propagateFrom(ಬೇರು_ctx.stat_ಸ್ಥಿತಿ);
}

ಶೂನ್ಯ parallelForAttachNestedRegion(ಸ್ಥಿರಾಂಕ Region& ಬೇರು_Region)
{
    CV_ಬಳಸದ(ಬೇರು_Region);
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();

    CV_DbgAssert(ctx.dummy_ರಾಶಿ_ತುದಿ.region == &ಬೇರು_Region);

    Region* region = ctx.get_Current_ಸಕ್ರಿಯ_Region();
    CV_LOG_PARALLEL(ಖಾಲಿ, " PARALLEL_FOR: " << (ಶೂನ್ಯ*)region << " ==> " << &ಬೇರು_Region);
    ಆದರೆ (!region)
        ಹಿಂತಿರುಗಿ;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ OPENCV_WITH_ITT
    ಆದರೆ (!ಬೇರು_Region.pImpl || !ಬೇರು_Region.pImpl->itt_id_registered)
        ಹಿಂತಿರುಗಿ;

    ಆದರೆ (!region->pImpl)
        ಹಿಂತಿರುಗಿ;

    CV_LOG_PARALLEL(ಖಾಲಿ, " PARALLEL_FOR ITT: " << (ಶೂನ್ಯ*)ಬೇರು_Region.pImpl->itt_id.d1 << ":" << ಬೇರು_Region.pImpl->itt_id.d2 << ":" << (ಶೂನ್ಯ*)ಬೇರು_Region.pImpl->itt_id.d3 << " => "
                                 << (ಶೂನ್ಯ*)region->pImpl->itt_id.d1 << ":" << region->pImpl->itt_id.d2 << ":" << (ಶೂನ್ಯ*)region->pImpl->itt_id.d3);
    __itt_relation_add(domain, region->pImpl->itt_id, __itt_relation_is_child_of, ಬೇರು_Region.pImpl->itt_id);
#ಆದರೆ_ಅಂತ್ಯ
}

ಶೂನ್ಯ parallelForFinalize(ಸ್ಥಿರಾಂಕ Region& ಬೇರು_Region)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();

    int64 ಕೊನೆ_Timestamp = getTimestampNS();
    int64 duration = ಕೊನೆ_Timestamp - ctx.ರಾಶಿ_ತುದಿ_Begin_Timestamp();
    CV_LOG_PARALLEL(ಖಾಲಿ, "parallel_for duration: " << duration << " " << &ಬೇರು_Region);

    std::vector<TraceManagerThreadLocal*> ಎಳೆಗಳು_ctx;
    getTraceManager().tls.gather(ಎಳೆಗಳು_ctx);
    RegionStatistics parallel_for_stat;
    ಗಾಗಿ (size_t i = 0; i < ಎಳೆಗಳು_ctx.size(); i++)
    {
        TraceManagerThreadLocal* ಮಗು_ctx = ಎಳೆಗಳು_ctx[i];

        ಆದರೆ (ಮಗು_ctx && ಮಗು_ctx->ರಾಶಿ_ತುದಿ_Region() == &ಬೇರು_Region)
        {
            CV_LOG_PARALLEL(ಖಾಲಿ, "Thread=" << ಮಗು_ctx->threadID << " " << ಮಗು_ctx->stat);
            RegionStatistics ಮಗು_stat;
            ಮಗು_ctx->stat.grab(ಮಗು_stat);
            parallel_for_stat.append(ಮಗು_stat);
            ಆದರೆ (ಮಗು_ctx != &ctx)
            {
                ಮಗು_ctx->dummy_ರಾಶಿ_ತುದಿ = TraceManagerThreadLocal::ರಾಶಿ_ನಮೂದು();
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                ctx.parallel_for_stat.grab(ctx.stat);
                ctx.stat_ಸ್ಥಿತಿ = ctx.parallel_for_stat_ಸ್ಥಿತಿ;
                ಮಗು_ctx->dummy_ರಾಶಿ_ತುದಿ = TraceManagerThreadLocal::ರಾಶಿ_ನಮೂದು();
            }
        }
    }

    ದಶಮಾಂಶ parallel_coeff = std::min(1.0f, duration / (ದಶಮಾಂಶ)(parallel_for_stat.duration));
    CV_LOG_PARALLEL(ಖಾಲಿ, "parallel_coeff=" << 1.0f / parallel_coeff);
    CV_LOG_PARALLEL(ಖಾಲಿ, parallel_for_stat);
    ಆದರೆ (parallel_coeff != 1.0f)
    {
        parallel_for_stat.multiply(parallel_coeff);
        CV_LOG_PARALLEL(ಖಾಲಿ, parallel_for_stat);
    }
    parallel_for_stat.duration = 0;
    ctx.stat.append(parallel_for_stat);
    CV_LOG_PARALLEL(ಖಾಲಿ, ctx.stat);
}

ರಚನೆ TraceArg::Extra_ದತ್ತಾಂಶ
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ OPENCV_WITH_ITT
    // Special fields for ITT
    __itt_string_handle* ಅಸ್ಥಿರ ittHandle_name;
#ಆದರೆ_ಅಂತ್ಯ
    Extra_ದತ್ತಾಂಶ(TraceManagerThreadLocal& ctx, ಸ್ಥಿರಾಂಕ TraceArg& arg)
    {
        CV_ಬಳಸದ(ctx); CV_ಬಳಸದ(arg);
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ OPENCV_WITH_ITT
        ಆದರೆ (isITTEnabled())
        {
            // Caching is not required here, because there is builtin cache.
            // https://software.intel.com/en-us/node/544203:
            //     Consecutive calls to __itt_string_handle_create with the same name return the same value.
            ittHandle_name = __itt_string_handle_create(arg.name);
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            ittHandle_name = 0;
        }
#ಆದರೆ_ಅಂತ್ಯ
    }
};

ಸ್ಥಿರ ಶೂನ್ಯ initTraceArg(TraceManagerThreadLocal& ctx, ಸ್ಥಿರಾಂಕ TraceArg& arg)
{
    TraceArg::Extra_ದತ್ತಾಂಶ** pExtra = arg.ppExtra;
    ಆದರೆ (*pExtra == ಖಾಲಿ)
    {
        cv::Auto_ಬೀಗ lock(cv::getInitializationMutex());
        ಆದರೆ (*pExtra == ಖಾಲಿ)
        {
            *pExtra = ಹೊಸ TraceArg::Extra_ದತ್ತಾಂಶ(ctx, arg);
        }
    }
}
ಶೂನ್ಯ traceArg(ಸ್ಥಿರಾಂಕ TraceArg& arg, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* value)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    Region* region = ctx.get_Current_ಸಕ್ರಿಯ_Region();
    ಆದರೆ (!region)
        ಹಿಂತಿರುಗಿ;
    CV_Assert(region->pImpl);
    initTraceArg(ctx, arg);
    ಆದರೆ (!value)
        value = "<null>";
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ OPENCV_WITH_ITT
    ಆದರೆ (isITTEnabled())
    {
        __itt_metadata_str_add(domain, region->pImpl->itt_id, (*arg.ppExtra)->ittHandle_name, value, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(value));
    }
#ಆದರೆ_ಅಂತ್ಯ
}
ಶೂನ್ಯ traceArg(ಸ್ಥಿರಾಂಕ TraceArg& arg, ಪೂರ್ಣಾಂಕ value)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    Region* region = ctx.get_Current_ಸಕ್ರಿಯ_Region();
    ಆದರೆ (!region)
        ಹಿಂತಿರುಗಿ;
    CV_Assert(region->pImpl);
    initTraceArg(ctx, arg);
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ OPENCV_WITH_ITT
    ಆದರೆ (isITTEnabled())
    {
        __itt_metadata_add(domain, region->pImpl->itt_id, (*arg.ppExtra)->ittHandle_name, ಗಾತ್ರ(ಪೂರ್ಣಾಂಕ) == 4 ? __itt_metadata_s32 : __itt_metadata_s64, 1, &value);
    }
#ಇಲ್ಲದಿದ್ದರೆ
    CV_ಬಳಸದ(value);
#ಆದರೆ_ಅಂತ್ಯ
}
ಶೂನ್ಯ traceArg(ಸ್ಥಿರಾಂಕ TraceArg& arg, int64 value)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    Region* region = ctx.get_Current_ಸಕ್ರಿಯ_Region();
    ಆದರೆ (!region)
        ಹಿಂತಿರುಗಿ;
    CV_Assert(region->pImpl);
    initTraceArg(ctx, arg);
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ OPENCV_WITH_ITT
    ಆದರೆ (isITTEnabled())
    {
        __itt_metadata_add(domain, region->pImpl->itt_id, (*arg.ppExtra)->ittHandle_name, __itt_metadata_s64, 1, &value);
    }
#ಇಲ್ಲದಿದ್ದರೆ
    CV_ಬಳಸದ(value);
#ಆದರೆ_ಅಂತ್ಯ
}
ಶೂನ್ಯ traceArg(ಸ್ಥಿರಾಂಕ TraceArg& arg, ದ್ವಿಗುಣ value)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    Region* region = ctx.get_Current_ಸಕ್ರಿಯ_Region();
    ಆದರೆ (!region)
        ಹಿಂತಿರುಗಿ;
    CV_Assert(region->pImpl);
    initTraceArg(ctx, arg);
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ OPENCV_WITH_ITT
    ಆದರೆ (isITTEnabled())
    {
        __itt_metadata_add(domain, region->pImpl->itt_id, (*arg.ppExtra)->ittHandle_name, __itt_metadata_double, 1, &value);
    }
#ಇಲ್ಲದಿದ್ದರೆ
    CV_ಬಳಸದ(value);
#ಆದರೆ_ಅಂತ್ಯ
}

#ಇಲ್ಲದಿದ್ದರೆ

Region::Region(ಸ್ಥಿರಾಂಕ LocationStaticStorage&) : pImpl(ಖಾಲಿ), implFlags(0) {}
ಶೂನ್ಯ Region::destroy() {}

ಶೂನ್ಯ traceArg(ಸ್ಥಿರಾಂಕ TraceArg&, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ*) {}
ಶೂನ್ಯ traceArg(ಸ್ಥಿರಾಂಕ TraceArg&, ಪೂರ್ಣಾಂಕ) {};
ಶೂನ್ಯ traceArg(ಸ್ಥಿರಾಂಕ TraceArg&, int64) {};
ಶೂನ್ಯ traceArg(ಸ್ಥಿರಾಂಕ TraceArg&, ದ್ವಿಗುಣ) {};

#ಆದರೆ_ಅಂತ್ಯ

}}}} // namespace
