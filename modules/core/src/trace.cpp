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
#تعریف_کړه CV_LOG(...) CV_LOG_INFO(عدم, __VA_ARGS__)
#وإلا
#تعریف_کړه CV_LOG(...) {}
#د_که_پای

#إذا 0
#تعریف_کړه CV_LOG_ITT(...) CV_LOG_INFO(عدم, __VA_ARGS__)
#وإلا
#تعریف_کړه CV_LOG_ITT(...) {}
#د_که_پای

#إذا 1
#تعریف_کړه CV_LOG_TRACE_BAILOUT(نښان, ...) CV_LOG_INFO(نښان, __VA_ARGS__)
#وإلا
#تعریف_کړه CV_LOG_TRACE_BAILOUT(...) {}
#د_که_پای

#إذا 0
#تعریف_کړه CV_LOG_PARALLEL(نښان, ...) CV_LOG_INFO(نښان, __VA_ARGS__)
#وإلا
#تعریف_کړه CV_LOG_PARALLEL(...) {}
#د_که_پای

#إذا 0
#تعریف_کړه CV_LOG_CTX_STAT(نښان, ...) CV_LOG_INFO(نښان, __VA_ARGS__)
#وإلا
#تعریف_کړه CV_LOG_CTX_STAT(...) {}
#د_که_پای

#إذا 0
#تعریف_کړه CV_LOG_SKIP(نښان, ...) CV_LOG_INFO(نښان, __VA_ARGS__)
#وإلا
#تعریف_کړه CV_LOG_SKIP(...) {}
#د_که_پای

نطاق cv {
نطاق utils {
نطاق trace {
نطاق details {

#که_تعریف_شوی OPENCV_TRACE

#که_تعریف_شوی _MSC_VER
#لارښوونه warning(disable:4065) // switch statement contains 'default' but no 'case' labels
#د_که_پای

ساكن منطقي getParameterTraceEnable()
{
    ساكن منطقي param_traceEnable = utils::getConfigurationParameterBool("OPENCV_TRACE", خطأ);
    أرجع param_traceEnable;
}

// TODO lazy configuration flags
ساكن صحيح param_max_Region_ژوروالی_Open_CV = (صحيح)utils::get_Configuration_Parameter_اندازه_T("OPENCV_TRACE_DEPTH_OPENCV", 1);
ساكن صحيح param_maxRegionChildrenOpenCV = (صحيح)utils::get_Configuration_Parameter_اندازه_T("OPENCV_TRACE_MAX_CHILDREN_OPENCV", 1000);
ساكن صحيح param_maxRegionChildren = (صحيح)utils::get_Configuration_Parameter_اندازه_T("OPENCV_TRACE_MAX_CHILDREN", 10000);

ساكن ثابت cv::String& getParameterTraceLocation()
{
    ساكن cv::String param_traceLocation = utils::getConfigurationParameterString("OPENCV_TRACE_LOCATION", "OpenCVTrace");
    أرجع param_traceLocation;
}

#که_تعریف_شوی HAVE_OPENCL
ساكن منطقي param_synchronizeOpenCL = utils::getConfigurationParameterBool("OPENCV_TRACE_SYNC_OPENCL", خطأ);
#د_که_پای

#که_تعریف_شوی OPENCV_WITH_ITT
ساكن منطقي param_ITT_ثبتوونکی_پلار_Scope = utils::getConfigurationParameterBool("OPENCV_TRACE_ITT_PARENT", خطأ);
#د_که_پای

ساكن ثابت حرف* _spaces(صحيح شمېر_2)
{
    ساكن ثابت حرف buf[64] =
"                                                               ";
    أرجع &buf[63 - (شمېر_2 & 63)];
}

/**
 * Text-based trace messages
 */
صنف Trace_پیغام
{
عام:
    حرف بفر[1024];
    size_t len;
    منطقي hasError;

    Trace_پیغام() :
        len(0),
        hasError(خطأ)
    {}

    منطقي چاپ_کړه(ثابت حرف* format, ...)
    {
        حرف* buf = &بفر[len];
        size_t sz = حجم(بفر) - len;
        va_list ap;
        va_start(ap, format);
        صحيح n = cv_vsnprintf(buf, (صحيح)sz, format, ap);
        va_end(ap);
        إذا (n < 0 || (size_t)n > sz)
        {
            hasError = ریښتیا;
            أرجع خطأ;
        }
        len += n;
        أرجع ریښتیا;
    }

    منطقي formatlocation(ثابت Region::LocationStaticStorage& location)
    {
        أرجع هذا->چاپ_کړه("l,%lld,\"%s\",%d,\"%s\",0x%llX\n",
                (اوږد اوږد صحيح)(*location.ppExtra)->global_location_id,
                location.filename,
                location.کرښه,
                location.name,
                (اوږد اوږد صحيح)(location.flags & ~0xF0000000));
    }
    منطقي formatRegionEnter(ثابت Region& region)
    {
        منطقي ok = هذا->چاپ_کړه("b,%d,%lld,%lld,%lld",
                (صحيح)region.pImpl->threadID,
                (اوږد اوږد صحيح)region.pImpl->beginTimestamp,
                (اوږد اوږد صحيح)((*region.pImpl->location.ppExtra)->global_location_id),
                (اوږد اوږد صحيح)region.pImpl->global_region_id);
        إذا (region.pImpl->پلار_Region && region.pImpl->پلار_Region->pImpl)
        {
            إذا (region.pImpl->پلار_Region->pImpl->threadID != region.pImpl->threadID)
                ok &= هذا->چاپ_کړه(",parentThread=%d,parent=%lld",
                        (صحيح)region.pImpl->پلار_Region->pImpl->threadID,
                        (اوږد اوږد صحيح)region.pImpl->پلار_Region->pImpl->global_region_id);
        }
        ok &= هذا->چاپ_کړه("\n");
        أرجع ok;
    }
    منطقي formatRegionLeave(ثابت Region& region, ثابت RegionStatistics& پایله)
    {
        CV_DbgAssert(region.pImpl->پای_ټکی_Timestamp - region.pImpl->beginTimestamp == پایله.duration);
        منطقي ok = هذا->چاپ_کړه("e,%d,%lld,%lld,%lld,%lld",
                (صحيح)region.pImpl->threadID,
                (اوږد اوږد صحيح)region.pImpl->پای_ټکی_Timestamp,
                (اوږد اوږد صحيح)(*region.pImpl->location.ppExtra)->global_location_id,
                (اوږد اوږد صحيح)region.pImpl->global_region_id,
                (اوږد اوږد صحيح)پایله.duration);
        إذا (پایله.currentSkippedRegions)
            ok &= هذا->چاپ_کړه(",skip=%d", (صحيح)پایله.currentSkippedRegions);
#که_تعریف_شوی HAVE_IPP
        إذا (پایله.durationImplIPP)
            ok &= هذا->چاپ_کړه(",tIPP=%lld", (اوږد اوږد صحيح)پایله.durationImplIPP);
#د_که_پای
#که_تعریف_شوی HAVE_OPENCL
        إذا (پایله.durationImplOpenCL)
            ok &= هذا->چاپ_کړه(",tOCL=%lld", (اوږد اوږد صحيح)پایله.durationImplOpenCL);
#د_که_پای
        ok &= هذا->چاپ_کړه("\n");
        أرجع ok;
    }
    منطقي ریکارډ_Region_Arg(ثابت Region& region, ثابت TraceArg& arg, ثابت حرف* value)
    {
        أرجع هذا->چاپ_کړه("a,%d,%lld,%lld,\"%s\",\"%s\"\n",
                region.pImpl->threadID,
                (اوږد اوږد صحيح)region.pImpl->beginTimestamp,
                (اوږد اوږد صحيح)region.pImpl->global_region_id,
                arg.name,
                value);
    }
};


#که_تعریف_شوی OPENCV_WITH_ITT
ساكن __itt_domain* domain = عدم;

ساكن منطقي isITTEnabled()
{
    ساكن متطاير منطقي isInitialized = خطأ;
    ساكن منطقي isEnabled = خطأ;
    إذا (!isInitialized)
    {
        cv::Auto_قلف lock(cv::getInitializationMutex());
        إذا (!isInitialized)
        {
            منطقي param_traceITTEnable = utils::getConfigurationParameterBool("OPENCV_TRACE_ITT_ENABLE", ریښتیا);
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
            isInitialized = ریښتیا;
        }
    }
    أرجع isEnabled;
}
#د_که_پای


Region::Location_Extra_معلومات::Location_Extra_معلومات(ثابت LocationStaticStorage& location)
{
    CV_ناکارول_شوی(location);
    ساكن صحيح g_location_id_counter = 0;
    global_location_id = CV_XADD(&g_location_id_counter, 1) + 1;
    CV_LOG("Register location: " << global_location_id << " (" << (فراغ*)&location << ")"
            << std::کرښه_پای << "    file: " << location.filename
            << std::کرښه_پای << "    line: " << location.کرښه
            << std::کرښه_پای << "    name: " << location.name);
#که_تعریف_شوی OPENCV_WITH_ITT
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
#د_که_پای
}

/*static*/ Region::Location_Extra_معلومات* Region::Location_Extra_معلومات::init(ثابت Region::LocationStaticStorage& location)
{
    Location_Extra_معلومات** pLocationExtra = location.ppExtra;
    CV_DbgAssert(pLocationExtra);
    إذا (*pLocationExtra == عدم)
    {
        cv::Auto_قلف lock(cv::getInitializationMutex());
        إذا (*pLocationExtra == عدم)
        {
            *pLocationExtra = جديد Region::Location_Extra_معلومات(location);
            TraceStorage* s = getTraceManager().trace_storage.get();
            إذا (s)
            {
                Trace_پیغام پیغام;
                پیغام.formatlocation(location);
                s->put(پیغام);
            }
        }
    }
    أرجع *pLocationExtra;
}


Region::Impl::Impl(TraceManagerThreadLocal& ctx, Region* پلار_Region_2, Region& region_, ثابت LocationStaticStorage& location_, int64 beginTimestamp_) :
    location(location_),
    region(region_),
    پلار_Region(پلار_Region_2),
    threadID(ctx.threadID),
    global_region_id(++ctx.region_counter),
    beginTimestamp(beginTimestamp_),
    پای_ټکی_Timestamp(0),
    مستقیم_Children_شمېر(0)
#که_تعریف_شوی OPENCV_WITH_ITT
    ,itt_id_registered(خطأ)
    ,itt_id(__itt_null)
#د_که_پای
{
    CV_DbgAssert(ctx.current_فعال_Region == پلار_Region);
    region.pImpl = هذا;

    ثبتوونکی_Region(ctx);

    enterRegion(ctx);
}

Region::Impl::~Impl()
{
#که_تعریف_شوی OPENCV_WITH_ITT
    إذا (itt_id_registered)
    {
        CV_LOG_ITT(" Destroy ITT region: I=" << (فراغ*)هذا);
        __itt_id_destroy(domain, itt_id);
        itt_id_registered = خطأ;
    }
#د_که_پای
    region.pImpl = عدم;
}

فراغ Region::Impl::enterRegion(TraceManagerThreadLocal& ctx)
{
    ctx.current_فعال_Region = &region;

    إذا (location.flags & REGION_FLAG_FUNCTION)
    {
        إذا ((location.flags & REGION_FLAG_APP_کوډ) == 0)
        {
            ctx.region_ژوروالی_Open_CV++;
        }
        ctx.region_ژوروالی++;
    }

    TraceStorage* s = ctx.getStorage();
    إذا (s)
    {
        Trace_پیغام پیغام;
        پیغام.formatRegionEnter(region);
        s->put(پیغام);
    }
#که_تعریف_شوی OPENCV_WITH_ITT
    إذا (isITTEnabled())
    {
        __itt_id پلار_ID = __itt_null;
        إذا (param_ITT_ثبتوونکی_پلار_Scope && پلار_Region && پلار_Region->pImpl && پلار_Region->pImpl->itt_id_registered && (location.flags & REGION_FLAG_REGION_FORCE) == 0)
            پلار_ID = پلار_Region->pImpl->itt_id;
        __itt_task_begin(domain, itt_id, پلار_ID, (*location.ppExtra)->ittHandle_name);
    }
#د_که_پای
}

فراغ Region::Impl::leaveRegion(TraceManagerThreadLocal& ctx)
{
    int64 duration = پای_ټکی_Timestamp - beginTimestamp; CV_ناکارول_شوی(duration);
    RegionStatistics پایله;
    ctx.stat.grab(پایله);
    ctx.ټول_Skipped_Events += پایله.currentSkippedRegions;
    CV_LOG(_spaces(ctx.get_Current_ژوروالی()*4) << "leaveRegion(): " << (فراغ*)هذا << " " << پایله);
#که_تعریف_شوی OPENCV_WITH_ITT
    إذا (isITTEnabled())
    {
        إذا (پایله.currentSkippedRegions)
        {
            __itt_metadata_add(domain, itt_id, __itt_string_handle_create("skipped trace entries"), __itt_metadata_u32, 1, &پایله.currentSkippedRegions);
        }
#که_تعریف_شوی HAVE_IPP
        إذا (پایله.durationImplIPP)
            __itt_metadata_add(domain, itt_id, __itt_string_handle_create("tIPP"), __itt_metadata_u64, 1, &پایله.durationImplIPP);
#د_که_پای
#که_تعریف_شوی HAVE_OPENCL
        إذا (پایله.durationImplOpenCL)
            __itt_metadata_add(domain, itt_id, __itt_string_handle_create("tOpenCL"), __itt_metadata_u64, 1, &پایله.durationImplOpenCL);
#د_که_پای
        __itt_task_end(domain);
    }
#د_که_پای
    TraceStorage* s = ctx.getStorage();
    إذا (s)
    {
        Trace_پیغام پیغام;
        پیغام.formatRegionLeave(region, پایله);
        s->put(پیغام);
    }

    إذا (location.flags & REGION_FLAG_FUNCTION)
    {
        إذا ((location.flags & REGION_FLAG_APP_کوډ) == 0)
        {
            ctx.region_ژوروالی_Open_CV--;
        }
        ctx.region_ژوروالی--;
    }

    ctx.current_فعال_Region = پلار_Region;
}

فراغ Region::Impl::release()
{
    ړنګول هذا;
}

فراغ Region::Impl::ثبتوونکی_Region(TraceManagerThreadLocal& ctx)
{
#که_تعریف_شوی OPENCV_WITH_ITT
    إذا (isITTEnabled())
    {
        إذا (!itt_id_registered)
        {
            CV_LOG_ITT(" Register ITT region: I=" << (فراغ*)هذا << " " << ctx.threadID << "-" << global_region_id);
#إذا 1 // workaround for some ITT backends
            itt_id = __itt_id_make((فراغ*)(intptr_t)(((int64)(ctx.threadID + 1) << 32) | global_region_id), global_region_id);
#وإلا
            itt_id = __itt_id_make((فراغ*)(intptr_t)(ctx.threadID + 1), global_region_id);
#د_که_پای
            __itt_id_create(domain, itt_id);
            itt_id_registered = ریښتیا;
        }
    }
#وإلا
    CV_ناکارول_شوی(ctx);
#د_که_پای
}

فراغ Region_Statistics_حالت::enable_Skip_ډېرپېښ(صحيح ژوروالی)
{
    CV_DbgAssert(skip_ژوروالی < 0);
    CV_LOG_SKIP(عدم, "SKIP-ENABLE: depth=" << ژوروالی);
    skip_ژوروالی = ژوروالی;
}
فراغ Region_Statistics_حالت::check_Reset_Skip_ډېرپېښ(صحيح leave_ژوروالی)
{
    إذا (leave_ژوروالی <= skip_ژوروالی)
    {
        CV_LOG_SKIP(عدم, "SKIP-RESET: leaveDepth=" << leave_ژوروالی << " skipDepth=" << skip_ژوروالی);
        skip_ژوروالی = -1;
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
    CV_LOG(_spaces(ctx.get_Current_ژوروالی()*4) << "Region(): " << (فراغ*)هذا << ": " << location.name);

    Region* پلار_Region = ctx.ډېری_سر_Region();
    ثابت Region::LocationStaticStorage* پلار_Location = ctx.ډېری_سر_Location();

    إذا (location.flags & REGION_FLAG_REGION_بل)
    {
        إذا (پلار_Region && پلار_Region->pImpl)
        {
            CV_DbgAssert((پلار_Region->pImpl->location.flags & REGION_FLAG_FUNCTION) == 0);
            پلار_Region->destroy(); پلار_Region->implFlags = 0;
            پلار_Region = ctx.ډېری_سر_Region();
            پلار_Location = ctx.ډېری_سر_Location();
        }
    }

    صحيح پلار_Children = 0;
    إذا (پلار_Region && پلار_Region->pImpl)
    {
        إذا (پلار_Location == عدم)
        {
            // parallel_for_body code path
            پلار_Children = CV_XADD(&پلار_Region->pImpl->مستقیم_Children_شمېر, 1) + 1;
        }
        وإلا
        {
            پلار_Children = ++پلار_Region->pImpl->مستقیم_Children_شمېر;
        }
    }

    int64 beginTimestamp = getTimestampNS();

    صحيح current_ژوروالی = ctx.get_Current_ژوروالی() + 1;
    بدل (location.flags & REGION_FLAG_IMPL_ماسک)
    {
#که_تعریف_شوی HAVE_IPP
    حالة REGION_FLAG_IMPL_IPP:
        إذا (!ctx.stat_حالت.ignore_ژوروالی_Impl_IPP)
            ctx.stat_حالت.ignore_ژوروالی_Impl_IPP = current_ژوروالی;
        اكسر;
#د_که_پای
#که_تعریف_شوی HAVE_OPENCL
    حالة REGION_FLAG_IMPL_OPENCL:
        إذا (!ctx.stat_حالت.ignore_ژوروالی_Impl_Open_CL)
            ctx.stat_حالت.ignore_ژوروالی_Impl_Open_CL = current_ژوروالی;
        اكسر;
#د_که_پای
    افتراضي:
        اكسر;
    }

    ctx.ډېری_ټېل(هذا, &location, beginTimestamp);
    implFlags |= REGION_FLAG_NEED_ډېری_راوباسه;

    إذا ((location.flags & REGION_FLAG_REGION_FORCE) == 0)
    {
        إذا (ctx.stat_حالت.skip_ژوروالی >= 0 && current_ژوروالی > ctx.stat_حالت.skip_ژوروالی)
        {
            CV_LOG(_spaces(ctx.get_Current_ژوروالی()*4) << "Parent region is disabled. Bailout");
            ctx.stat.currentSkippedRegions++;
            أرجع;
        }

        إذا (param_maxRegionChildrenOpenCV > 0 && (location.flags & REGION_FLAG_APP_کوډ) == 0 && پلار_Location && (پلار_Location->flags & REGION_FLAG_APP_کوډ) == 0)
        {
            إذا (پلار_Children >= param_maxRegionChildrenOpenCV)
            {
                CV_LOG_TRACE_BAILOUT(عدم, _spaces(ctx.get_Current_ژوروالی()*4) << "OpenCV parent region exceeds children count. Bailout");
                ctx.stat_حالت.enable_Skip_ډېرپېښ(current_ژوروالی - 1);
                ctx.stat.currentSkippedRegions++;
                DEBUG_ONLY(ctx.dump_ډېری(std::وتنه, خطأ));
                أرجع;
            }
        }
        إذا (param_maxRegionChildren > 0 && پلار_Children >= param_maxRegionChildren)
        {
            CV_LOG_TRACE_BAILOUT(عدم, _spaces(ctx.get_Current_ژوروالی()*4) << "Parent region exceeds children count. Bailout");
            ctx.stat_حالت.enable_Skip_ډېرپېښ(current_ژوروالی - 1);
            ctx.stat.currentSkippedRegions++;
            DEBUG_ONLY(ctx.dump_ډېری(std::وتنه, خطأ));
            أرجع;
        }
    }

    Location_Extra_معلومات::init(location);

    إذا ((*location.ppExtra)->global_location_id == 0)
    {
        CV_LOG_TRACE_BAILOUT(عدم, _spaces(ctx.get_Current_ژوروالی()*4) << "Region location is disabled. Bailout");
        ctx.stat_حالت.enable_Skip_ډېرپېښ(current_ژوروالی);
        ctx.stat.currentSkippedRegions++;
        أرجع;
    }

    إذا (پلار_Location && (پلار_Location->flags & REGION_FLAG_SKIP_NESTED))
    {
        CV_LOG(_spaces(ctx.get_Current_ژوروالی()*4) << "Parent region disables inner regions. Bailout");
        ctx.stat_حالت.enable_Skip_ډېرپېښ(current_ژوروالی);
        ctx.stat.currentSkippedRegions++;
        أرجع;
    }

    إذا (param_max_Region_ژوروالی_Open_CV)
    {
        إذا ((location.flags & REGION_FLAG_APP_کوډ) == 0)
        {
            إذا (ctx.region_ژوروالی_Open_CV >= param_max_Region_ژوروالی_Open_CV)
            {
                CV_LOG(_spaces(ctx.get_Current_ژوروالی()*4) << "OpenCV region depth is exceed = " << ctx.region_ژوروالی_Open_CV << ". Bailout");
                إذا (ctx.stat.currentSkippedRegions == 0)
                {
                    DEBUG_ONLY(ctx.dump_ډېری(std::وتنه, خطأ));
                }
                ctx.stat_حالت.enable_Skip_ډېرپېښ(current_ژوروالی);
                ctx.stat.currentSkippedRegions++;
                أرجع;
            }
        }
    }

    جديد Impl(ctx, پلار_Region, *هذا, location, beginTimestamp);
    CV_DbgAssert(pImpl != عدم);
    implFlags |= REGION_FLAG_فعال;

    // parallel_for path
    إذا (پلار_Region && پلار_Region->pImpl)
    {
        إذا (پلار_Location == عدم)
        {
            pImpl->مستقیم_Children_شمېر = پلار_Children;
        }
    }
}

فراغ Region::destroy()
{
    CV_DbgAssert(implFlags != 0);

    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    CV_LOG(_spaces(ctx.get_Current_ژوروالی()*4) << "Region::destruct(): " << (فراغ*)هذا << " pImpl=" << pImpl << " implFlags=" << implFlags << ' ' << (ctx.ډېری_سر_Location() ? ctx.ډېری_سر_Location()->name : "<unknown>"));

    CV_DbgAssert(implFlags & REGION_FLAG_NEED_ډېری_راوباسه);
    ثابت صحيح current_ژوروالی = ctx.get_Current_ژوروالی(); CV_ناکارول_شوی(current_ژوروالی);

    CV_LOG_CTX_STAT(عدم, _spaces(current_ژوروالی*4) << ctx.stat << ' ' << ctx.stat_حالت);

    ثابت Region::LocationStaticStorage* location = ctx.ډېری_سر_Location();
    Impl::Optimization_لار my_کوډ_لار = Impl::کوډ_لار_PLAIN;
    إذا (location)
    {
        بدل (location->flags & REGION_FLAG_IMPL_ماسک)
        {
#که_تعریف_شوی HAVE_IPP
        حالة REGION_FLAG_IMPL_IPP:
            my_کوډ_لار = Impl::کوډ_لار_IPP;
            اكسر;
#د_که_پای
#که_تعریف_شوی HAVE_OPENCL
        حالة REGION_FLAG_IMPL_OPENCL:
            إذا (param_synchronizeOpenCL && cv::ocl::isOpenCLActivated())
                cv::ocl::پای_2();
            my_کوډ_لار = Impl::کوډ_لار_OPENCL;
            اكسر;
#د_که_پای
        افتراضي:
            اكسر;
        }
    }

    int64 پای_ټکی_Timestamp = getTimestampNS();
    int64 duration = پای_ټکی_Timestamp - ctx.ډېری_سر_Begin_Timestamp();

    منطقي فعال = is_فعال_2();

    إذا (فعال)
        ctx.stat.duration = duration;
    وإلا إذا (ctx.ډېری.size() == ctx.parallel_for_ډېری_اندازه + 1)
        ctx.stat.duration += duration;

    بدل (my_کوډ_لار) {
        حالة Impl::کوډ_لار_PLAIN:
            // nothing
            اكسر;
#که_تعریف_شوی HAVE_IPP
        حالة Impl::کوډ_لار_IPP:
            إذا (ctx.stat_حالت.ignore_ژوروالی_Impl_IPP == current_ژوروالی)
            {
                ctx.stat.durationImplIPP += duration;
                ctx.stat_حالت.ignore_ژوروالی_Impl_IPP = 0;
            }
            وإلا إذا (فعال)
            {
                ctx.stat.durationImplIPP = duration;
            }
            اكسر;
#د_که_پای
#که_تعریف_شوی HAVE_OPENCL
        حالة Impl::کوډ_لار_OPENCL:
            إذا (ctx.stat_حالت.ignore_ژوروالی_Impl_Open_CL == current_ژوروالی)
            {
                ctx.stat.durationImplOpenCL += duration;
                ctx.stat_حالت.ignore_ژوروالی_Impl_Open_CL = 0;
            }
            وإلا إذا (فعال)
            {
                ctx.stat.durationImplOpenCL = duration;
            }
            اكسر;
#د_که_پای
        افتراضي:
            اكسر;
    }

    إذا (pImpl)
    {
        CV_DbgAssert((implFlags & (REGION_FLAG_فعال | REGION_FLAG_NEED_ډېری_راوباسه)) == (REGION_FLAG_فعال | REGION_FLAG_NEED_ډېری_راوباسه));
        CV_DbgAssert(ctx.ډېری_سر_Region() == هذا);
        pImpl->پای_ټکی_Timestamp = پای_ټکی_Timestamp;
        pImpl->leaveRegion(ctx);
        pImpl->release();
        pImpl = عدم;
        DEBUG_ONLY(implFlags &= ~REGION_FLAG_فعال);
    }
    وإلا
    {
        CV_DbgAssert(ctx.stat_حالت.skip_ژوروالی <= current_ژوروالی);
    }

    إذا (implFlags & REGION_FLAG_NEED_ډېری_راوباسه)
    {
        CV_DbgAssert(ctx.ډېری_سر_Region() == هذا);
        ctx.ډېری_راوباسه();
        ctx.stat_حالت.check_Reset_Skip_ډېرپېښ(current_ژوروالی);
        DEBUG_ONLY(implFlags &= ~REGION_FLAG_NEED_ډېری_راوباسه);
    }
    CV_LOG_CTX_STAT(عدم, _spaces(current_ژوروالی*4) << "===> " << ctx.stat << ' ' << ctx.stat_حالت);
}


TraceManagerThreadLocal::~TraceManagerThreadLocal()
{
}

فراغ TraceManagerThreadLocal::dump_ډېری(std::ostream& out, منطقي onlyFunctions) ثابت
{
    std::stringstream ss;
    std::deque<ډېری_ثبت>::const_iterator it = ډېری.begin();
    std::deque<ډېری_ثبت>::const_iterator end = ډېری.end();
    صحيح ژوروالی = 0;
    لكل (; it != end; ++it)
    {
        ثابت Region::LocationStaticStorage* location = it->location;
        إذا (location)
        {
            إذا (!onlyFunctions || (location->flags & REGION_FLAG_FUNCTION))
            {
                ss << _spaces(4*ژوروالی) << location->name << std::کرښه_پای;
                ژوروالی++;
            }
        }
        وإلا
        {
            ss << _spaces(4*ژوروالی) << "<unknown>" << std::کرښه_پای;
            ژوروالی++;
        }
    }
    out << ss.str();
}

صنف AsyncTraceStorage CV_FINAL : عام TraceStorage
{
    بدلیدونکی std::ofstream out;
عام:
    ثابت std::string name;

    AsyncTraceStorage(ثابت std::string& filename) :
        out(filename.c_str(), std::ios::trunc),
        name(filename)
    {
        out << "#description: OpenCV trace file" << std::کرښه_پای;
        out << "#version: 1.0" << std::کرښه_پای;
    }
    ~AsyncTraceStorage()
    {
        out.close();
    }

    منطقي put(ثابت Trace_پیغام& پیغام) ثابت CV_OVERRIDE
    {
        إذا (پیغام.hasError)
            أرجع خطأ;
        out << پیغام.بفر;
        //DEBUG_ONLY(std::flush(out)); // TODO configure flag
        أرجع ریښتیا;
    }
};

صنف SyncTraceStorage CV_FINAL : عام TraceStorage
{
    بدلیدونکی std::ofstream out;
    بدلیدونکی cv::Mutex mutex;
عام:
    ثابت std::string name;

    SyncTraceStorage(ثابت std::string& filename) :
        out(filename.c_str(), std::ios::trunc),
        name(filename)
    {
        out << "#description: OpenCV trace file" << std::کرښه_پای;
        out << "#version: 1.0" << std::کرښه_پای;
    }
    ~SyncTraceStorage()
    {
        cv::Auto_قلف l(mutex);
        out.close();
    }

    منطقي put(ثابت Trace_پیغام& پیغام) ثابت CV_OVERRIDE
    {
        إذا (پیغام.hasError)
            أرجع خطأ;
        {
            cv::Auto_قلف l(mutex);
            out << پیغام.بفر;
            std::flush(out); // TODO configure flag
        }
        أرجع ریښتیا;
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
            Trace_پیغام پیغام;
            ثابت حرف* pos = strrchr(filepath.c_str(), '/'); // extract filename
#که_تعریف_شوی _WIN32
            إذا (!pos)
                pos = strrchr(filepath.c_str(), '\\');
#د_که_پای
            إذا (!pos)
                pos = filepath.c_str();
            وإلا
                pos += 1; // fix to skip extra slash in filename beginning
            پیغام.چاپ_کړه("#thread file: %s\n", pos);
            global->put(پیغام);
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

    isInitialized = ریښتیا;
    CV_LOG("TraceManager ctor: " << (فراغ*)هذا);

    CV_LOG("TraceManager configure()");
    activated = getParameterTraceEnable();

    إذا (activated)
        trace_storage.reset(جديد SyncTraceStorage(std::string(getParameterTraceLocation()) + ".txt"));

#که_تعریف_شوی OPENCV_WITH_ITT
    إذا (isITTEnabled())
    {
        activated = ریښتیا; // force trace pipeline activation (without OpenCV storage)
        __itt_region_begin(domain, __itt_null, __itt_null, __itt_string_handle_create("OpenCVTrace"));
    }
#د_که_پای
}
TraceManager::~TraceManager()
{
    CV_LOG("TraceManager dtor: " << (فراغ*)هذا);

#که_تعریف_شوی OPENCV_WITH_ITT
    إذا (isITTEnabled())
    {
        __itt_region_end(domain, __itt_null);
    }
#د_که_پای

    std::vector<TraceManagerThreadLocal*> تارونه_ctx;
    tls.gather(تارونه_ctx);
    size_t ټول_Events = 0, ټول_Skipped_Events = 0;
    لكل (size_t i = 0; i < تارونه_ctx.size(); i++)
    {
        TraceManagerThreadLocal* ctx = تارونه_ctx[i];
        إذا (ctx)
        {
            ټول_Events += ctx->region_counter;
            ټول_Skipped_Events += ctx->ټول_Skipped_Events;
        }
    }
    إذا (ټول_Events || activated)
    {
        CV_LOG_INFO(عدم, "Trace: Total events: " << ټول_Events);
    }
    إذا (ټول_Skipped_Events)
    {
        CV_LOG_WARNING(عدم, "Trace: Total skipped events: " << ټول_Skipped_Events);
    }

    // This is a global static object, so process starts shutdown here
    // Turn off trace
    cv::__termination = ریښتیا; // also set in DllMain() notifications handler for DLL_PROCESS_DETACH
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
        CV_ناکارول_شوی(m); // TODO
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

فراغ parallel_For_سېټ_ریښه_Region(ثابت Region& ریښه_Region, ثابت TraceManagerThreadLocal& ریښه_ctx)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();

    إذا (ctx.dummy_ډېری_سر.region == &ریښه_Region) // already attached
        أرجع;

    CV_Assert(ctx.dummy_ډېری_سر.region == عدم);
    ctx.dummy_ډېری_سر = TraceManagerThreadLocal::ډېری_ثبت(const_cast<Region*>(&ریښه_Region), عدم, -1);

    إذا (&ctx == &ریښه_ctx)
    {
        ctx.stat.grab(ctx.parallel_for_stat);
        ctx.parallel_for_stat_حالت = ctx.stat_حالت;
        ctx.parallel_for_ډېری_اندازه = ctx.ډېری.size();
        أرجع;
    }

    CV_Assert(ctx.ډېری.empty());

    ctx.current_فعال_Region = const_cast<Region*>(&ریښه_Region);

    ctx.region_ژوروالی = ریښه_ctx.region_ژوروالی;
    ctx.region_ژوروالی_Open_CV = ریښه_ctx.region_ژوروالی_Open_CV;

    ctx.parallel_for_ډېری_اندازه = 0;

    ctx.stat_حالت.propagateFrom(ریښه_ctx.stat_حالت);
}

فراغ parallelForAttachNestedRegion(ثابت Region& ریښه_Region)
{
    CV_ناکارول_شوی(ریښه_Region);
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();

    CV_DbgAssert(ctx.dummy_ډېری_سر.region == &ریښه_Region);

    Region* region = ctx.get_Current_فعال_Region();
    CV_LOG_PARALLEL(عدم, " PARALLEL_FOR: " << (فراغ*)region << " ==> " << &ریښه_Region);
    إذا (!region)
        أرجع;

#که_تعریف_شوی OPENCV_WITH_ITT
    إذا (!ریښه_Region.pImpl || !ریښه_Region.pImpl->itt_id_registered)
        أرجع;

    إذا (!region->pImpl)
        أرجع;

    CV_LOG_PARALLEL(عدم, " PARALLEL_FOR ITT: " << (فراغ*)ریښه_Region.pImpl->itt_id.d1 << ":" << ریښه_Region.pImpl->itt_id.d2 << ":" << (فراغ*)ریښه_Region.pImpl->itt_id.d3 << " => "
                                 << (فراغ*)region->pImpl->itt_id.d1 << ":" << region->pImpl->itt_id.d2 << ":" << (فراغ*)region->pImpl->itt_id.d3);
    __itt_relation_add(domain, region->pImpl->itt_id, __itt_relation_is_child_of, ریښه_Region.pImpl->itt_id);
#د_که_پای
}

فراغ parallelForFinalize(ثابت Region& ریښه_Region)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();

    int64 پای_ټکی_Timestamp = getTimestampNS();
    int64 duration = پای_ټکی_Timestamp - ctx.ډېری_سر_Begin_Timestamp();
    CV_LOG_PARALLEL(عدم, "parallel_for duration: " << duration << " " << &ریښه_Region);

    std::vector<TraceManagerThreadLocal*> تارونه_ctx;
    getTraceManager().tls.gather(تارونه_ctx);
    RegionStatistics parallel_for_stat;
    لكل (size_t i = 0; i < تارونه_ctx.size(); i++)
    {
        TraceManagerThreadLocal* ماشوم_ctx = تارونه_ctx[i];

        إذا (ماشوم_ctx && ماشوم_ctx->ډېری_سر_Region() == &ریښه_Region)
        {
            CV_LOG_PARALLEL(عدم, "Thread=" << ماشوم_ctx->threadID << " " << ماشوم_ctx->stat);
            RegionStatistics ماشوم_stat;
            ماشوم_ctx->stat.grab(ماشوم_stat);
            parallel_for_stat.append(ماشوم_stat);
            إذا (ماشوم_ctx != &ctx)
            {
                ماشوم_ctx->dummy_ډېری_سر = TraceManagerThreadLocal::ډېری_ثبت();
            }
            وإلا
            {
                ctx.parallel_for_stat.grab(ctx.stat);
                ctx.stat_حالت = ctx.parallel_for_stat_حالت;
                ماشوم_ctx->dummy_ډېری_سر = TraceManagerThreadLocal::ډېری_ثبت();
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

بنية TraceArg::Extra_معلومات
{
#که_تعریف_شوی OPENCV_WITH_ITT
    // Special fields for ITT
    __itt_string_handle* متطاير ittHandle_name;
#د_که_پای
    Extra_معلومات(TraceManagerThreadLocal& ctx, ثابت TraceArg& arg)
    {
        CV_ناکارول_شوی(ctx); CV_ناکارول_شوی(arg);
#که_تعریف_شوی OPENCV_WITH_ITT
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
#د_که_پای
    }
};

ساكن فراغ initTraceArg(TraceManagerThreadLocal& ctx, ثابت TraceArg& arg)
{
    TraceArg::Extra_معلومات** pExtra = arg.ppExtra;
    إذا (*pExtra == عدم)
    {
        cv::Auto_قلف lock(cv::getInitializationMutex());
        إذا (*pExtra == عدم)
        {
            *pExtra = جديد TraceArg::Extra_معلومات(ctx, arg);
        }
    }
}
فراغ traceArg(ثابت TraceArg& arg, ثابت حرف* value)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    Region* region = ctx.get_Current_فعال_Region();
    إذا (!region)
        أرجع;
    CV_Assert(region->pImpl);
    initTraceArg(ctx, arg);
    إذا (!value)
        value = "<null>";
#که_تعریف_شوی OPENCV_WITH_ITT
    إذا (isITTEnabled())
    {
        __itt_metadata_str_add(domain, region->pImpl->itt_id, (*arg.ppExtra)->ittHandle_name, value, مزي_اوږدوالی(value));
    }
#د_که_پای
}
فراغ traceArg(ثابت TraceArg& arg, صحيح value)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    Region* region = ctx.get_Current_فعال_Region();
    إذا (!region)
        أرجع;
    CV_Assert(region->pImpl);
    initTraceArg(ctx, arg);
#که_تعریف_شوی OPENCV_WITH_ITT
    إذا (isITTEnabled())
    {
        __itt_metadata_add(domain, region->pImpl->itt_id, (*arg.ppExtra)->ittHandle_name, حجم(صحيح) == 4 ? __itt_metadata_s32 : __itt_metadata_s64, 1, &value);
    }
#وإلا
    CV_ناکارول_شوی(value);
#د_که_پای
}
فراغ traceArg(ثابت TraceArg& arg, int64 value)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    Region* region = ctx.get_Current_فعال_Region();
    إذا (!region)
        أرجع;
    CV_Assert(region->pImpl);
    initTraceArg(ctx, arg);
#که_تعریف_شوی OPENCV_WITH_ITT
    إذا (isITTEnabled())
    {
        __itt_metadata_add(domain, region->pImpl->itt_id, (*arg.ppExtra)->ittHandle_name, __itt_metadata_s64, 1, &value);
    }
#وإلا
    CV_ناکارول_شوی(value);
#د_که_پای
}
فراغ traceArg(ثابت TraceArg& arg, مزدوج value)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    Region* region = ctx.get_Current_فعال_Region();
    إذا (!region)
        أرجع;
    CV_Assert(region->pImpl);
    initTraceArg(ctx, arg);
#که_تعریف_شوی OPENCV_WITH_ITT
    إذا (isITTEnabled())
    {
        __itt_metadata_add(domain, region->pImpl->itt_id, (*arg.ppExtra)->ittHandle_name, __itt_metadata_double, 1, &value);
    }
#وإلا
    CV_ناکارول_شوی(value);
#د_که_پای
}

#وإلا

Region::Region(ثابت LocationStaticStorage&) : pImpl(عدم), implFlags(0) {}
فراغ Region::destroy() {}

فراغ traceArg(ثابت TraceArg&, ثابت حرف*) {}
فراغ traceArg(ثابت TraceArg&, صحيح) {};
فراغ traceArg(ثابت TraceArg&, int64) {};
فراغ traceArg(ثابت TraceArg&, مزدوج) {};

#د_که_پای

}}}} // namespace
