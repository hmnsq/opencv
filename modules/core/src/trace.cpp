// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#समावेश "precomp.hpp"

#समावेश <opencv2/core/utils/trace.hpp>
#समावेश <opencv2/core/utils/trace.private.hpp>
#समावेश <opencv2/core/utils/configuration.private.hpp>

#समावेश <opencv2/core/opencl/ocl_defs.hpp>

#समावेश <cstdarg> // va_start

#समावेश <sstream>
#समावेश <ostream>
#समावेश <fstream>

#अगर 0
#घोषणा CV_LOG(...) CV_LOG_INFO(शून्य, __VA_ARGS__)
#अन्यथा
#घोषणा CV_LOG(...) {}
#पूर्ण_अगर

#अगर 0
#घोषणा CV_LOG_ITT(...) CV_LOG_INFO(शून्य, __VA_ARGS__)
#अन्यथा
#घोषणा CV_LOG_ITT(...) {}
#पूर्ण_अगर

#अगर 1
#घोषणा CV_LOG_TRACE_BAILOUT(tag, ...) CV_LOG_INFO(tag, __VA_ARGS__)
#अन्यथा
#घोषणा CV_LOG_TRACE_BAILOUT(...) {}
#पूर्ण_अगर

#अगर 0
#घोषणा CV_LOG_PARALLEL(tag, ...) CV_LOG_INFO(tag, __VA_ARGS__)
#अन्यथा
#घोषणा CV_LOG_PARALLEL(...) {}
#पूर्ण_अगर

#अगर 0
#घोषणा CV_LOG_CTX_STAT(tag, ...) CV_LOG_INFO(tag, __VA_ARGS__)
#अन्यथा
#घोषणा CV_LOG_CTX_STAT(...) {}
#पूर्ण_अगर

#अगर 0
#घोषणा CV_LOG_SKIP(tag, ...) CV_LOG_INFO(tag, __VA_ARGS__)
#अन्यथा
#घोषणा CV_LOG_SKIP(...) {}
#पूर्ण_अगर

namespace cv {
namespace utils {
namespace trace {
namespace details {

#अगर_घोषित OPENCV_TRACE

#अगर_घोषित _MSC_VER
#आशय warning(disable:4065) // switch statement contains 'default' but no 'case' labels
#पूर्ण_अगर

अटल bool getParameterTraceEnable()
{
    अटल bool param_traceEnable = utils::getConfigurationParameterBool("OPENCV_TRACE", false);
    वापस param_traceEnable;
}

// TODO lazy configuration flags
अटल पूर्णांक param_maxRegionDepthOpenCV = (पूर्णांक)utils::get_Configuration_Parameter_आकार_T("OPENCV_TRACE_DEPTH_OPENCV", 1);
अटल पूर्णांक param_maxRegionChildrenOpenCV = (पूर्णांक)utils::get_Configuration_Parameter_आकार_T("OPENCV_TRACE_MAX_CHILDREN_OPENCV", 1000);
अटल पूर्णांक param_maxRegionChildren = (पूर्णांक)utils::get_Configuration_Parameter_आकार_T("OPENCV_TRACE_MAX_CHILDREN", 10000);

अटल स्थिर cv::String& getParameterTraceLocation()
{
    अटल cv::String param_traceLocation = utils::getConfigurationParameterString("OPENCV_TRACE_LOCATION", "OpenCVTrace");
    वापस param_traceLocation;
}

#अगर_घोषित HAVE_OPENCL
अटल bool param_synchronizeOpenCL = utils::getConfigurationParameterBool("OPENCV_TRACE_SYNC_OPENCL", false);
#पूर्ण_अगर

#अगर_घोषित OPENCV_WITH_ITT
अटल bool param_ITT_registerParentScope = utils::getConfigurationParameterBool("OPENCV_TRACE_ITT_PARENT", false);
#पूर्ण_अगर

अटल स्थिर अक्षर* _spaces(पूर्णांक गिनती_2)
{
    अटल स्थिर अक्षर buf[64] =
"                                                               ";
    वापस &buf[63 - (गिनती_2 & 63)];
}

/**
 * Text-based trace messages
 */
श्रेणी TraceMessage
{
खुला:
    अक्षर बफर[1024];
    माप_प्रकार len;
    bool hasError;

    TraceMessage() :
        len(0),
        hasError(false)
    {}

    bool म_लिखो(स्थिर अक्षर* format, ...)
    {
        अक्षर* buf = &बफर[len];
        माप_प्रकार sz = माप(बफर) - len;
        बहु_सूची ap;
        बहु_शुरू(ap, format);
        पूर्णांक n = cv_vsnprintf(buf, (पूर्णांक)sz, format, ap);
        बहु_पूर्ण(ap);
        अगर (n < 0 || (माप_प्रकार)n > sz)
        {
            hasError = true;
            वापस false;
        }
        len += n;
        वापस true;
    }

    bool formatlocation(स्थिर Region::LocationStaticStorage& location)
    {
        वापस यह->म_लिखो("l,%lld,\"%s\",%d,\"%s\",0x%llX\n",
                (दीर्घ दीर्घ पूर्णांक)(*location.ppExtra)->global_location_id,
                location.filename,
                location.पंक्ति,
                location.name,
                (दीर्घ दीर्घ पूर्णांक)(location.flags & ~0xF0000000));
    }
    bool formatRegionEnter(स्थिर Region& region)
    {
        bool ok = यह->म_लिखो("b,%d,%lld,%lld,%lld",
                (पूर्णांक)region.pImpl->threadID,
                (दीर्घ दीर्घ पूर्णांक)region.pImpl->beginTimestamp,
                (दीर्घ दीर्घ पूर्णांक)((*region.pImpl->location.ppExtra)->global_location_id),
                (दीर्घ दीर्घ पूर्णांक)region.pImpl->global_region_id);
        अगर (region.pImpl->parentRegion && region.pImpl->parentRegion->pImpl)
        {
            अगर (region.pImpl->parentRegion->pImpl->threadID != region.pImpl->threadID)
                ok &= यह->म_लिखो(",parentThread=%d,parent=%lld",
                        (पूर्णांक)region.pImpl->parentRegion->pImpl->threadID,
                        (दीर्घ दीर्घ पूर्णांक)region.pImpl->parentRegion->pImpl->global_region_id);
        }
        ok &= यह->म_लिखो("\n");
        वापस ok;
    }
    bool formatRegionLeave(स्थिर Region& region, स्थिर RegionStatistics& परिणाम)
    {
        CV_DbgAssert(region.pImpl->endTimestamp - region.pImpl->beginTimestamp == परिणाम.duration);
        bool ok = यह->म_लिखो("e,%d,%lld,%lld,%lld,%lld",
                (पूर्णांक)region.pImpl->threadID,
                (दीर्घ दीर्घ पूर्णांक)region.pImpl->endTimestamp,
                (दीर्घ दीर्घ पूर्णांक)(*region.pImpl->location.ppExtra)->global_location_id,
                (दीर्घ दीर्घ पूर्णांक)region.pImpl->global_region_id,
                (दीर्घ दीर्घ पूर्णांक)परिणाम.duration);
        अगर (परिणाम.currentSkippedRegions)
            ok &= यह->म_लिखो(",skip=%d", (पूर्णांक)परिणाम.currentSkippedRegions);
#अगर_घोषित HAVE_IPP
        अगर (परिणाम.durationImplIPP)
            ok &= यह->म_लिखो(",tIPP=%lld", (दीर्घ दीर्घ पूर्णांक)परिणाम.durationImplIPP);
#पूर्ण_अगर
#अगर_घोषित HAVE_OPENCL
        अगर (परिणाम.durationImplOpenCL)
            ok &= यह->म_लिखो(",tOCL=%lld", (दीर्घ दीर्घ पूर्णांक)परिणाम.durationImplOpenCL);
#पूर्ण_अगर
        ok &= यह->म_लिखो("\n");
        वापस ok;
    }
    bool recordRegionArg(स्थिर Region& region, स्थिर TraceArg& arg, स्थिर अक्षर* value)
    {
        वापस यह->म_लिखो("a,%d,%lld,%lld,\"%s\",\"%s\"\n",
                region.pImpl->threadID,
                (दीर्घ दीर्घ पूर्णांक)region.pImpl->beginTimestamp,
                (दीर्घ दीर्घ पूर्णांक)region.pImpl->global_region_id,
                arg.name,
                value);
    }
};


#अगर_घोषित OPENCV_WITH_ITT
अटल __itt_domain* domain = शून्य;

अटल bool isITTEnabled()
{
    अटल अस्थिर bool isInitialized = false;
    अटल bool isEnabled = false;
    अगर (!isInitialized)
    {
        cv::AutoLock lock(cv::getInitializationMutex());
        अगर (!isInitialized)
        {
            bool param_traceITTEnable = utils::getConfigurationParameterBool("OPENCV_TRACE_ITT_ENABLE", true);
            अगर (param_traceITTEnable)
            {
                isEnabled = !!(__itt_api_version());
                CV_LOG_ITT("ITT is " << (isEnabled ? "enabled" : "disabled"));
                domain = __itt_domain_create("OpenCVTrace");
            }
            अन्यथा
            {
                CV_LOG_ITT("ITT is disabled through OpenCV parameter");
                isEnabled = false;
            }
            isInitialized = true;
        }
    }
    वापस isEnabled;
}
#पूर्ण_अगर


Region::Location_Extra_तथ्य::Location_Extra_तथ्य(स्थिर LocationStaticStorage& location)
{
    CV_UNUSED(location);
    अटल पूर्णांक g_location_id_गणक = 0;
    global_location_id = CV_XADD(&g_location_id_गणक, 1) + 1;
    CV_LOG("Register location: " << global_location_id << " (" << (व्योम*)&location << ")"
            << std::प_पूर्ण << "    file: " << location.filename
            << std::प_पूर्ण << "    line: " << location.पंक्ति
            << std::प_पूर्ण << "    name: " << location.name);
#अगर_घोषित OPENCV_WITH_ITT
    अगर (isITTEnabled())
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
#पूर्ण_अगर
}

/*static*/ Region::Location_Extra_तथ्य* Region::Location_Extra_तथ्य::init(स्थिर Region::LocationStaticStorage& location)
{
    Location_Extra_तथ्य** pLocationExtra = location.ppExtra;
    CV_DbgAssert(pLocationExtra);
    अगर (*pLocationExtra == शून्य)
    {
        cv::AutoLock lock(cv::getInitializationMutex());
        अगर (*pLocationExtra == शून्य)
        {
            *pLocationExtra = नया Region::Location_Extra_तथ्य(location);
            TraceStorage* s = getTraceManager().trace_storage.लो();
            अगर (s)
            {
                TraceMessage संदेश;
                संदेश.formatlocation(location);
                s->दो(संदेश);
            }
        }
    }
    वापस *pLocationExtra;
}


Region::Impl::Impl(TraceManagerThreadLocal& ctx, Region* parentRegion_, Region& region_, स्थिर LocationStaticStorage& location_, int64 beginTimestamp_) :
    location(location_),
    region(region_),
    parentRegion(parentRegion_),
    threadID(ctx.threadID),
    global_region_id(++ctx.region_गणक),
    beginTimestamp(beginTimestamp_),
    endTimestamp(0),
    direct_Children_गिनती(0)
#अगर_घोषित OPENCV_WITH_ITT
    ,itt_id_registered(false)
    ,itt_id(__itt_null)
#पूर्ण_अगर
{
    CV_DbgAssert(ctx.currentActiveRegion == parentRegion);
    region.pImpl = यह;

    registerRegion(ctx);

    enterRegion(ctx);
}

Region::Impl::~Impl()
{
#अगर_घोषित OPENCV_WITH_ITT
    अगर (itt_id_registered)
    {
        CV_LOG_ITT(" Destroy ITT region: I=" << (व्योम*)यह);
        __itt_id_destroy(domain, itt_id);
        itt_id_registered = false;
    }
#पूर्ण_अगर
    region.pImpl = शून्य;
}

व्योम Region::Impl::enterRegion(TraceManagerThreadLocal& ctx)
{
    ctx.currentActiveRegion = &region;

    अगर (location.flags & REGION_FLAG_FUNCTION)
    {
        अगर ((location.flags & REGION_FLAG_APP_CODE) == 0)
        {
            ctx.regionDepthOpenCV++;
        }
        ctx.regionDepth++;
    }

    TraceStorage* s = ctx.getStorage();
    अगर (s)
    {
        TraceMessage संदेश;
        संदेश.formatRegionEnter(region);
        s->दो(संदेश);
    }
#अगर_घोषित OPENCV_WITH_ITT
    अगर (isITTEnabled())
    {
        __itt_id parentID = __itt_null;
        अगर (param_ITT_registerParentScope && parentRegion && parentRegion->pImpl && parentRegion->pImpl->itt_id_registered && (location.flags & REGION_FLAG_REGION_FORCE) == 0)
            parentID = parentRegion->pImpl->itt_id;
        __itt_task_begin(domain, itt_id, parentID, (*location.ppExtra)->ittHandle_name);
    }
#पूर्ण_अगर
}

व्योम Region::Impl::leaveRegion(TraceManagerThreadLocal& ctx)
{
    int64 duration = endTimestamp - beginTimestamp; CV_UNUSED(duration);
    RegionStatistics परिणाम;
    ctx.stat.grab(परिणाम);
    ctx.योग_Skipped_Events += परिणाम.currentSkippedRegions;
    CV_LOG(_spaces(ctx.getCurrentDepth()*4) << "leaveRegion(): " << (व्योम*)यह << " " << परिणाम);
#अगर_घोषित OPENCV_WITH_ITT
    अगर (isITTEnabled())
    {
        अगर (परिणाम.currentSkippedRegions)
        {
            __itt_metadata_add(domain, itt_id, __itt_string_handle_create("skipped trace entries"), __itt_metadata_u32, 1, &परिणाम.currentSkippedRegions);
        }
#अगर_घोषित HAVE_IPP
        अगर (परिणाम.durationImplIPP)
            __itt_metadata_add(domain, itt_id, __itt_string_handle_create("tIPP"), __itt_metadata_u64, 1, &परिणाम.durationImplIPP);
#पूर्ण_अगर
#अगर_घोषित HAVE_OPENCL
        अगर (परिणाम.durationImplOpenCL)
            __itt_metadata_add(domain, itt_id, __itt_string_handle_create("tOpenCL"), __itt_metadata_u64, 1, &परिणाम.durationImplOpenCL);
#पूर्ण_अगर
        __itt_task_end(domain);
    }
#पूर्ण_अगर
    TraceStorage* s = ctx.getStorage();
    अगर (s)
    {
        TraceMessage संदेश;
        संदेश.formatRegionLeave(region, परिणाम);
        s->दो(संदेश);
    }

    अगर (location.flags & REGION_FLAG_FUNCTION)
    {
        अगर ((location.flags & REGION_FLAG_APP_CODE) == 0)
        {
            ctx.regionDepthOpenCV--;
        }
        ctx.regionDepth--;
    }

    ctx.currentActiveRegion = parentRegion;
}

व्योम Region::Impl::release()
{
    मिटाओ यह;
}

व्योम Region::Impl::registerRegion(TraceManagerThreadLocal& ctx)
{
#अगर_घोषित OPENCV_WITH_ITT
    अगर (isITTEnabled())
    {
        अगर (!itt_id_registered)
        {
            CV_LOG_ITT(" Register ITT region: I=" << (व्योम*)यह << " " << ctx.threadID << "-" << global_region_id);
#अगर 1 // workaround for some ITT backends
            itt_id = __itt_id_make((व्योम*)(intptr_t)(((int64)(ctx.threadID + 1) << 32) | global_region_id), global_region_id);
#अन्यथा
            itt_id = __itt_id_make((व्योम*)(intptr_t)(ctx.threadID + 1), global_region_id);
#पूर्ण_अगर
            __itt_id_create(domain, itt_id);
            itt_id_registered = true;
        }
    }
#अन्यथा
    CV_UNUSED(ctx);
#पूर्ण_अगर
}

व्योम RegionStatisticsStatus::enableSkipMode(पूर्णांक depth)
{
    CV_DbgAssert(_skipDepth < 0);
    CV_LOG_SKIP(शून्य, "SKIP-ENABLE: depth=" << depth);
    _skipDepth = depth;
}
व्योम RegionStatisticsStatus::checkResetSkipMode(पूर्णांक leaveDepth)
{
    अगर (leaveDepth <= _skipDepth)
    {
        CV_LOG_SKIP(शून्य, "SKIP-RESET: leaveDepth=" << leaveDepth << " skipDepth=" << _skipDepth);
        _skipDepth = -1;
    }
}

Region::Region(स्थिर LocationStaticStorage& location) :
    pImpl(शून्य),
    implFlags(0)
{
    // Checks:
    // - global enable flag
    // - parent region is disabled
    // - children count threshold
    // - region location
    // - depth (opencv nested calls)
    अगर (!TraceManager::isActivated())
    {
        CV_LOG("Trace is disabled. Bailout");
        वापस;
    }

    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    CV_LOG(_spaces(ctx.getCurrentDepth()*4) << "Region(): " << (व्योम*)यह << ": " << location.name);

    Region* parentRegion = ctx.ढेर_शीर्ष_Region();
    स्थिर Region::LocationStaticStorage* parentLocation = ctx.ढेर_शीर्ष_Location();

    अगर (location.flags & REGION_FLAG_REGION_अग्र)
    {
        अगर (parentRegion && parentRegion->pImpl)
        {
            CV_DbgAssert((parentRegion->pImpl->location.flags & REGION_FLAG_FUNCTION) == 0);
            parentRegion->destroy(); parentRegion->implFlags = 0;
            parentRegion = ctx.ढेर_शीर्ष_Region();
            parentLocation = ctx.ढेर_शीर्ष_Location();
        }
    }

    पूर्णांक parentChildren = 0;
    अगर (parentRegion && parentRegion->pImpl)
    {
        अगर (parentLocation == शून्य)
        {
            // parallel_for_body code path
            parentChildren = CV_XADD(&parentRegion->pImpl->direct_Children_गिनती, 1) + 1;
        }
        अन्यथा
        {
            parentChildren = ++parentRegion->pImpl->direct_Children_गिनती;
        }
    }

    int64 beginTimestamp = getTimestampNS();

    पूर्णांक currentDepth = ctx.getCurrentDepth() + 1;
    चयन (location.flags & REGION_FLAG_IMPL_MASK)
    {
#अगर_घोषित HAVE_IPP
    हाल REGION_FLAG_IMPL_IPP:
        अगर (!ctx.stat_status.ignoreDepthImplIPP)
            ctx.stat_status.ignoreDepthImplIPP = currentDepth;
        अवरोध;
#पूर्ण_अगर
#अगर_घोषित HAVE_OPENCL
    हाल REGION_FLAG_IMPL_OPENCL:
        अगर (!ctx.stat_status.ignoreDepthImplOpenCL)
            ctx.stat_status.ignoreDepthImplOpenCL = currentDepth;
        अवरोध;
#पूर्ण_अगर
    शेष:
        अवरोध;
    }

    ctx.ढेर_धकेलो(यह, &location, beginTimestamp);
    implFlags |= REGION_FLAG_NEED_ढेर_निकालो;

    अगर ((location.flags & REGION_FLAG_REGION_FORCE) == 0)
    {
        अगर (ctx.stat_status._skipDepth >= 0 && currentDepth > ctx.stat_status._skipDepth)
        {
            CV_LOG(_spaces(ctx.getCurrentDepth()*4) << "Parent region is disabled. Bailout");
            ctx.stat.currentSkippedRegions++;
            वापस;
        }

        अगर (param_maxRegionChildrenOpenCV > 0 && (location.flags & REGION_FLAG_APP_CODE) == 0 && parentLocation && (parentLocation->flags & REGION_FLAG_APP_CODE) == 0)
        {
            अगर (parentChildren >= param_maxRegionChildrenOpenCV)
            {
                CV_LOG_TRACE_BAILOUT(शून्य, _spaces(ctx.getCurrentDepth()*4) << "OpenCV parent region exceeds children count. Bailout");
                ctx.stat_status.enableSkipMode(currentDepth - 1);
                ctx.stat.currentSkippedRegions++;
                DEBUG_ONLY(ctx.dump_ढेर(std::अ_बाहर, false));
                वापस;
            }
        }
        अगर (param_maxRegionChildren > 0 && parentChildren >= param_maxRegionChildren)
        {
            CV_LOG_TRACE_BAILOUT(शून्य, _spaces(ctx.getCurrentDepth()*4) << "Parent region exceeds children count. Bailout");
            ctx.stat_status.enableSkipMode(currentDepth - 1);
            ctx.stat.currentSkippedRegions++;
            DEBUG_ONLY(ctx.dump_ढेर(std::अ_बाहर, false));
            वापस;
        }
    }

    Location_Extra_तथ्य::init(location);

    अगर ((*location.ppExtra)->global_location_id == 0)
    {
        CV_LOG_TRACE_BAILOUT(शून्य, _spaces(ctx.getCurrentDepth()*4) << "Region location is disabled. Bailout");
        ctx.stat_status.enableSkipMode(currentDepth);
        ctx.stat.currentSkippedRegions++;
        वापस;
    }

    अगर (parentLocation && (parentLocation->flags & REGION_FLAG_SKIP_NESTED))
    {
        CV_LOG(_spaces(ctx.getCurrentDepth()*4) << "Parent region disables inner regions. Bailout");
        ctx.stat_status.enableSkipMode(currentDepth);
        ctx.stat.currentSkippedRegions++;
        वापस;
    }

    अगर (param_maxRegionDepthOpenCV)
    {
        अगर ((location.flags & REGION_FLAG_APP_CODE) == 0)
        {
            अगर (ctx.regionDepthOpenCV >= param_maxRegionDepthOpenCV)
            {
                CV_LOG(_spaces(ctx.getCurrentDepth()*4) << "OpenCV region depth is exceed = " << ctx.regionDepthOpenCV << ". Bailout");
                अगर (ctx.stat.currentSkippedRegions == 0)
                {
                    DEBUG_ONLY(ctx.dump_ढेर(std::अ_बाहर, false));
                }
                ctx.stat_status.enableSkipMode(currentDepth);
                ctx.stat.currentSkippedRegions++;
                वापस;
            }
        }
    }

    नया Impl(ctx, parentRegion, *यह, location, beginTimestamp);
    CV_DbgAssert(pImpl != शून्य);
    implFlags |= REGION_FLAG__ACTIVE;

    // parallel_for path
    अगर (parentRegion && parentRegion->pImpl)
    {
        अगर (parentLocation == शून्य)
        {
            pImpl->direct_Children_गिनती = parentChildren;
        }
    }
}

व्योम Region::destroy()
{
    CV_DbgAssert(implFlags != 0);

    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    CV_LOG(_spaces(ctx.getCurrentDepth()*4) << "Region::destruct(): " << (व्योम*)यह << " pImpl=" << pImpl << " implFlags=" << implFlags << ' ' << (ctx.ढेर_शीर्ष_Location() ? ctx.ढेर_शीर्ष_Location()->name : "<unknown>"));

    CV_DbgAssert(implFlags & REGION_FLAG_NEED_ढेर_निकालो);
    स्थिर पूर्णांक currentDepth = ctx.getCurrentDepth(); CV_UNUSED(currentDepth);

    CV_LOG_CTX_STAT(शून्य, _spaces(currentDepth*4) << ctx.stat << ' ' << ctx.stat_status);

    स्थिर Region::LocationStaticStorage* location = ctx.ढेर_शीर्ष_Location();
    Impl::OptimizationPath myCodePath = Impl::CODE_PATH_PLAIN;
    अगर (location)
    {
        चयन (location->flags & REGION_FLAG_IMPL_MASK)
        {
#अगर_घोषित HAVE_IPP
        हाल REGION_FLAG_IMPL_IPP:
            myCodePath = Impl::CODE_PATH_IPP;
            अवरोध;
#पूर्ण_अगर
#अगर_घोषित HAVE_OPENCL
        हाल REGION_FLAG_IMPL_OPENCL:
            अगर (param_synchronizeOpenCL && cv::ocl::isOpenCLActivated())
                cv::ocl::finish();
            myCodePath = Impl::CODE_PATH_OPENCL;
            अवरोध;
#पूर्ण_अगर
        शेष:
            अवरोध;
        }
    }

    int64 endTimestamp = getTimestampNS();
    int64 duration = endTimestamp - ctx.ढेर_शीर्ष_Begin_Timestamp();

    bool active = isActive();

    अगर (active)
        ctx.stat.duration = duration;
    अन्यथा अगर (ctx.ढेर.size() == ctx.parallel_for_ढेर_आकार + 1)
        ctx.stat.duration += duration;

    चयन (myCodePath) {
        हाल Impl::CODE_PATH_PLAIN:
            // nothing
            अवरोध;
#अगर_घोषित HAVE_IPP
        हाल Impl::CODE_PATH_IPP:
            अगर (ctx.stat_status.ignoreDepthImplIPP == currentDepth)
            {
                ctx.stat.durationImplIPP += duration;
                ctx.stat_status.ignoreDepthImplIPP = 0;
            }
            अन्यथा अगर (active)
            {
                ctx.stat.durationImplIPP = duration;
            }
            अवरोध;
#पूर्ण_अगर
#अगर_घोषित HAVE_OPENCL
        हाल Impl::CODE_PATH_OPENCL:
            अगर (ctx.stat_status.ignoreDepthImplOpenCL == currentDepth)
            {
                ctx.stat.durationImplOpenCL += duration;
                ctx.stat_status.ignoreDepthImplOpenCL = 0;
            }
            अन्यथा अगर (active)
            {
                ctx.stat.durationImplOpenCL = duration;
            }
            अवरोध;
#पूर्ण_अगर
        शेष:
            अवरोध;
    }

    अगर (pImpl)
    {
        CV_DbgAssert((implFlags & (REGION_FLAG__ACTIVE | REGION_FLAG_NEED_ढेर_निकालो)) == (REGION_FLAG__ACTIVE | REGION_FLAG_NEED_ढेर_निकालो));
        CV_DbgAssert(ctx.ढेर_शीर्ष_Region() == यह);
        pImpl->endTimestamp = endTimestamp;
        pImpl->leaveRegion(ctx);
        pImpl->release();
        pImpl = शून्य;
        DEBUG_ONLY(implFlags &= ~REGION_FLAG__ACTIVE);
    }
    अन्यथा
    {
        CV_DbgAssert(ctx.stat_status._skipDepth <= currentDepth);
    }

    अगर (implFlags & REGION_FLAG_NEED_ढेर_निकालो)
    {
        CV_DbgAssert(ctx.ढेर_शीर्ष_Region() == यह);
        ctx.ढेर_निकालो();
        ctx.stat_status.checkResetSkipMode(currentDepth);
        DEBUG_ONLY(implFlags &= ~REGION_FLAG_NEED_ढेर_निकालो);
    }
    CV_LOG_CTX_STAT(शून्य, _spaces(currentDepth*4) << "===> " << ctx.stat << ' ' << ctx.stat_status);
}


TraceManagerThreadLocal::~TraceManagerThreadLocal()
{
}

व्योम TraceManagerThreadLocal::dump_ढेर(std::ब_प्रवाह& out, bool onlyFunctions) स्थिर
{
    std::stringstream ss;
    std::deque<ढेर_Entry>::const_iterator it = ढेर.begin();
    std::deque<ढेर_Entry>::const_iterator end = ढेर.end();
    पूर्णांक depth = 0;
    क्रम (; it != end; ++it)
    {
        स्थिर Region::LocationStaticStorage* location = it->location;
        अगर (location)
        {
            अगर (!onlyFunctions || (location->flags & REGION_FLAG_FUNCTION))
            {
                ss << _spaces(4*depth) << location->name << std::प_पूर्ण;
                depth++;
            }
        }
        अन्यथा
        {
            ss << _spaces(4*depth) << "<unknown>" << std::प_पूर्ण;
            depth++;
        }
    }
    out << ss.str();
}

श्रेणी AsyncTraceStorage CV_FINAL : खुला TraceStorage
{
    mutable std::बख_प्रवाह out;
खुला:
    स्थिर std::string name;

    AsyncTraceStorage(स्थिर std::string& filename) :
        out(filename.c_str(), std::पनप::छाँटो),
        name(filename)
    {
        out << "#description: OpenCV trace file" << std::प_पूर्ण;
        out << "#version: 1.0" << std::प_पूर्ण;
    }
    ~AsyncTraceStorage()
    {
        out.बंद();
    }

    bool दो(स्थिर TraceMessage& संदेश) स्थिर CV_OVERRIDE
    {
        अगर (संदेश.hasError)
            वापस false;
        out << संदेश.बफर;
        //DEBUG_ONLY(std::flush(out)); // TODO configure flag
        वापस true;
    }
};

श्रेणी SyncTraceStorage CV_FINAL : खुला TraceStorage
{
    mutable std::बख_प्रवाह out;
    mutable cv::Mutex mutex;
खुला:
    स्थिर std::string name;

    SyncTraceStorage(स्थिर std::string& filename) :
        out(filename.c_str(), std::पनप::छाँटो),
        name(filename)
    {
        out << "#description: OpenCV trace file" << std::प_पूर्ण;
        out << "#version: 1.0" << std::प_पूर्ण;
    }
    ~SyncTraceStorage()
    {
        cv::AutoLock l(mutex);
        out.बंद();
    }

    bool दो(स्थिर TraceMessage& संदेश) स्थिर CV_OVERRIDE
    {
        अगर (संदेश.hasError)
            वापस false;
        {
            cv::AutoLock l(mutex);
            out << संदेश.बफर;
            std::सफाई(out); // TODO configure flag
        }
        वापस true;
    }
};


TraceStorage* TraceManagerThreadLocal::getStorage() स्थिर
{
    // TODO configuration option for stdout/single trace file
    अगर (storage.empty())
    {
        TraceStorage* global = getTraceManager().trace_storage.लो();
        अगर (global)
        {
            स्थिर std::string filepath = cv::format("%s-%03d.txt", getParameterTraceLocation().c_str(), threadID).c_str();
            TraceMessage संदेश;
            स्थिर अक्षर* pos = म_खोजप(filepath.c_str(), '/'); // extract filename
#अगर_घोषित _WIN32
            अगर (!pos)
                pos = म_खोजप(filepath.c_str(), '\\');
#पूर्ण_अगर
            अगर (!pos)
                pos = filepath.c_str();
            अन्यथा
                pos += 1; // fix to skip extra slash in filename beginning
            संदेश.म_लिखो("#thread file: %s\n", pos);
            global->दो(संदेश);
            storage.reset(नया AsyncTraceStorage(filepath));
        }
    }
    वापस storage.लो();
}



अटल bool activated = false;
अटल bool isInitialized = false;

TraceManager::TraceManager()
{
    (व्योम)cv::getTimestampNS();

    isInitialized = true;
    CV_LOG("TraceManager ctor: " << (व्योम*)यह);

    CV_LOG("TraceManager configure()");
    activated = getParameterTraceEnable();

    अगर (activated)
        trace_storage.reset(नया SyncTraceStorage(std::string(getParameterTraceLocation()) + ".txt"));

#अगर_घोषित OPENCV_WITH_ITT
    अगर (isITTEnabled())
    {
        activated = true; // force trace pipeline activation (without OpenCV storage)
        __itt_region_begin(domain, __itt_null, __itt_null, __itt_string_handle_create("OpenCVTrace"));
    }
#पूर्ण_अगर
}
TraceManager::~TraceManager()
{
    CV_LOG("TraceManager dtor: " << (व्योम*)यह);

#अगर_घोषित OPENCV_WITH_ITT
    अगर (isITTEnabled())
    {
        __itt_region_end(domain, __itt_null);
    }
#पूर्ण_अगर

    std::vector<TraceManagerThreadLocal*> threads_ctx;
    tls.gather(threads_ctx);
    माप_प्रकार योग_Events = 0, योग_Skipped_Events = 0;
    क्रम (माप_प्रकार i = 0; i < threads_ctx.size(); i++)
    {
        TraceManagerThreadLocal* ctx = threads_ctx[i];
        अगर (ctx)
        {
            योग_Events += ctx->region_गणक;
            योग_Skipped_Events += ctx->योग_Skipped_Events;
        }
    }
    अगर (योग_Events || activated)
    {
        CV_LOG_INFO(शून्य, "Trace: Total events: " << योग_Events);
    }
    अगर (योग_Skipped_Events)
    {
        CV_LOG_WARNING(शून्य, "Trace: Total skipped events: " << योग_Skipped_Events);
    }

    // This is a global static object, so process starts shutdown here
    // Turn off trace
    cv::__termination = true; // also set in DllMain() notifications handler for DLL_PROCESS_DETACH
    activated = false;
}

bool TraceManager::isActivated()
{
    // Check if process starts shutdown, and set earlyExit to true
    // to prevent further instrumentation processing earlier.
    अगर (cv::__termination)
    {
        activated = false;
        वापस false;
    }

    अगर (!isInitialized)
    {
        TraceManager& m = getTraceManager();
        CV_UNUSED(m); // TODO
    }

    वापस activated;
}


अटल TraceManager* getTraceManagerCallOnce()
{
    अटल TraceManager globalInstance;
    वापस &globalInstance;
}
TraceManager& getTraceManager()
{
    CV_SINGLETON_LAZY_INIT_REF(TraceManager, getTraceManagerCallOnce())
}

व्योम parallel_For_Set_मूल_Region(स्थिर Region& मूल_Region, स्थिर TraceManagerThreadLocal& मूल_ctx)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();

    अगर (ctx.dummy_ढेर_शीर्ष.region == &मूल_Region) // already attached
        वापस;

    CV_Assert(ctx.dummy_ढेर_शीर्ष.region == शून्य);
    ctx.dummy_ढेर_शीर्ष = TraceManagerThreadLocal::ढेर_Entry(const_cast<Region*>(&मूल_Region), शून्य, -1);

    अगर (&ctx == &मूल_ctx)
    {
        ctx.stat.grab(ctx.parallel_for_stat);
        ctx.parallel_for_stat_status = ctx.stat_status;
        ctx.parallel_for_ढेर_आकार = ctx.ढेर.size();
        वापस;
    }

    CV_Assert(ctx.ढेर.empty());

    ctx.currentActiveRegion = const_cast<Region*>(&मूल_Region);

    ctx.regionDepth = मूल_ctx.regionDepth;
    ctx.regionDepthOpenCV = मूल_ctx.regionDepthOpenCV;

    ctx.parallel_for_ढेर_आकार = 0;

    ctx.stat_status.propagateFrom(मूल_ctx.stat_status);
}

व्योम parallelForAttachNestedRegion(स्थिर Region& मूल_Region)
{
    CV_UNUSED(मूल_Region);
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();

    CV_DbgAssert(ctx.dummy_ढेर_शीर्ष.region == &मूल_Region);

    Region* region = ctx.getCurrentActiveRegion();
    CV_LOG_PARALLEL(शून्य, " PARALLEL_FOR: " << (व्योम*)region << " ==> " << &मूल_Region);
    अगर (!region)
        वापस;

#अगर_घोषित OPENCV_WITH_ITT
    अगर (!मूल_Region.pImpl || !मूल_Region.pImpl->itt_id_registered)
        वापस;

    अगर (!region->pImpl)
        वापस;

    CV_LOG_PARALLEL(शून्य, " PARALLEL_FOR ITT: " << (व्योम*)मूल_Region.pImpl->itt_id.d1 << ":" << मूल_Region.pImpl->itt_id.d2 << ":" << (व्योम*)मूल_Region.pImpl->itt_id.d3 << " => "
                                 << (व्योम*)region->pImpl->itt_id.d1 << ":" << region->pImpl->itt_id.d2 << ":" << (व्योम*)region->pImpl->itt_id.d3);
    __itt_relation_add(domain, region->pImpl->itt_id, __itt_relation_is_child_of, मूल_Region.pImpl->itt_id);
#पूर्ण_अगर
}

व्योम parallelForFinalize(स्थिर Region& मूल_Region)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();

    int64 endTimestamp = getTimestampNS();
    int64 duration = endTimestamp - ctx.ढेर_शीर्ष_Begin_Timestamp();
    CV_LOG_PARALLEL(शून्य, "parallel_for duration: " << duration << " " << &मूल_Region);

    std::vector<TraceManagerThreadLocal*> threads_ctx;
    getTraceManager().tls.gather(threads_ctx);
    RegionStatistics parallel_for_stat;
    क्रम (माप_प्रकार i = 0; i < threads_ctx.size(); i++)
    {
        TraceManagerThreadLocal* child_ctx = threads_ctx[i];

        अगर (child_ctx && child_ctx->ढेर_शीर्ष_Region() == &मूल_Region)
        {
            CV_LOG_PARALLEL(शून्य, "Thread=" << child_ctx->threadID << " " << child_ctx->stat);
            RegionStatistics child_stat;
            child_ctx->stat.grab(child_stat);
            parallel_for_stat.append(child_stat);
            अगर (child_ctx != &ctx)
            {
                child_ctx->dummy_ढेर_शीर्ष = TraceManagerThreadLocal::ढेर_Entry();
            }
            अन्यथा
            {
                ctx.parallel_for_stat.grab(ctx.stat);
                ctx.stat_status = ctx.parallel_for_stat_status;
                child_ctx->dummy_ढेर_शीर्ष = TraceManagerThreadLocal::ढेर_Entry();
            }
        }
    }

    भग्न parallel_coeff = std::min(1.0f, duration / (भग्न)(parallel_for_stat.duration));
    CV_LOG_PARALLEL(शून्य, "parallel_coeff=" << 1.0f / parallel_coeff);
    CV_LOG_PARALLEL(शून्य, parallel_for_stat);
    अगर (parallel_coeff != 1.0f)
    {
        parallel_for_stat.multiply(parallel_coeff);
        CV_LOG_PARALLEL(शून्य, parallel_for_stat);
    }
    parallel_for_stat.duration = 0;
    ctx.stat.append(parallel_for_stat);
    CV_LOG_PARALLEL(शून्य, ctx.stat);
}

काष्ठा TraceArg::Extra_तथ्य
{
#अगर_घोषित OPENCV_WITH_ITT
    // Special fields for ITT
    __itt_string_handle* अस्थिर ittHandle_name;
#पूर्ण_अगर
    Extra_तथ्य(TraceManagerThreadLocal& ctx, स्थिर TraceArg& arg)
    {
        CV_UNUSED(ctx); CV_UNUSED(arg);
#अगर_घोषित OPENCV_WITH_ITT
        अगर (isITTEnabled())
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
#पूर्ण_अगर
    }
};

अटल व्योम initTraceArg(TraceManagerThreadLocal& ctx, स्थिर TraceArg& arg)
{
    TraceArg::Extra_तथ्य** pExtra = arg.ppExtra;
    अगर (*pExtra == शून्य)
    {
        cv::AutoLock lock(cv::getInitializationMutex());
        अगर (*pExtra == शून्य)
        {
            *pExtra = नया TraceArg::Extra_तथ्य(ctx, arg);
        }
    }
}
व्योम traceArg(स्थिर TraceArg& arg, स्थिर अक्षर* value)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    Region* region = ctx.getCurrentActiveRegion();
    अगर (!region)
        वापस;
    CV_Assert(region->pImpl);
    initTraceArg(ctx, arg);
    अगर (!value)
        value = "<null>";
#अगर_घोषित OPENCV_WITH_ITT
    अगर (isITTEnabled())
    {
        __itt_metadata_str_add(domain, region->pImpl->itt_id, (*arg.ppExtra)->ittHandle_name, value, म_माप(value));
    }
#पूर्ण_अगर
}
व्योम traceArg(स्थिर TraceArg& arg, पूर्णांक value)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    Region* region = ctx.getCurrentActiveRegion();
    अगर (!region)
        वापस;
    CV_Assert(region->pImpl);
    initTraceArg(ctx, arg);
#अगर_घोषित OPENCV_WITH_ITT
    अगर (isITTEnabled())
    {
        __itt_metadata_add(domain, region->pImpl->itt_id, (*arg.ppExtra)->ittHandle_name, माप(पूर्णांक) == 4 ? __itt_metadata_s32 : __itt_metadata_s64, 1, &value);
    }
#अन्यथा
    CV_UNUSED(value);
#पूर्ण_अगर
}
व्योम traceArg(स्थिर TraceArg& arg, int64 value)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    Region* region = ctx.getCurrentActiveRegion();
    अगर (!region)
        वापस;
    CV_Assert(region->pImpl);
    initTraceArg(ctx, arg);
#अगर_घोषित OPENCV_WITH_ITT
    अगर (isITTEnabled())
    {
        __itt_metadata_add(domain, region->pImpl->itt_id, (*arg.ppExtra)->ittHandle_name, __itt_metadata_s64, 1, &value);
    }
#अन्यथा
    CV_UNUSED(value);
#पूर्ण_अगर
}
व्योम traceArg(स्थिर TraceArg& arg, द्विगुन value)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    Region* region = ctx.getCurrentActiveRegion();
    अगर (!region)
        वापस;
    CV_Assert(region->pImpl);
    initTraceArg(ctx, arg);
#अगर_घोषित OPENCV_WITH_ITT
    अगर (isITTEnabled())
    {
        __itt_metadata_add(domain, region->pImpl->itt_id, (*arg.ppExtra)->ittHandle_name, __itt_metadata_double, 1, &value);
    }
#अन्यथा
    CV_UNUSED(value);
#पूर्ण_अगर
}

#अन्यथा

Region::Region(स्थिर LocationStaticStorage&) : pImpl(शून्य), implFlags(0) {}
व्योम Region::destroy() {}

व्योम traceArg(स्थिर TraceArg&, स्थिर अक्षर*) {}
व्योम traceArg(स्थिर TraceArg&, पूर्णांक) {};
व्योम traceArg(स्थिर TraceArg&, int64) {};
व्योम traceArg(स्थिर TraceArg&, द्विगुन) {};

#पूर्ण_अगर

}}}} // namespace
