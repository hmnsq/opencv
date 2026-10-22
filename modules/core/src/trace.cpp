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
#تعریف_کرو CV_LOG(...) CV_LOG_INFO(عدم, __VA_ARGS__)
#وإلا
#تعریف_کرو CV_LOG(...) {}
#جے_انت

#إذا 0
#تعریف_کرو CV_LOG_ITT(...) CV_LOG_INFO(عدم, __VA_ARGS__)
#وإلا
#تعریف_کرو CV_LOG_ITT(...) {}
#جے_انت

#إذا 1
#تعریف_کرو CV_LOG_TRACE_BAILOUT(ٹیگ, ...) CV_LOG_INFO(ٹیگ, __VA_ARGS__)
#وإلا
#تعریف_کرو CV_LOG_TRACE_BAILOUT(...) {}
#جے_انت

#إذا 0
#تعریف_کرو CV_LOG_PARALLEL(ٹیگ, ...) CV_LOG_INFO(ٹیگ, __VA_ARGS__)
#وإلا
#تعریف_کرو CV_LOG_PARALLEL(...) {}
#جے_انت

#إذا 0
#تعریف_کرو CV_LOG_CTX_STAT(ٹیگ, ...) CV_LOG_INFO(ٹیگ, __VA_ARGS__)
#وإلا
#تعریف_کرو CV_LOG_CTX_STAT(...) {}
#جے_انت

#إذا 0
#تعریف_کرو CV_LOG_SKIP(ٹیگ, ...) CV_LOG_INFO(ٹیگ, __VA_ARGS__)
#وإلا
#تعریف_کرو CV_LOG_SKIP(...) {}
#جے_انت

نطاق cv {
نطاق utils {
نطاق trace {
نطاق details {

#جے_تعریف_شدہ OPENCV_TRACE

#جے_تعریف_شدہ _MSC_VER
#ہدایت warning(disable:4065) // switch statement contains 'default' but no 'case' labels
#جے_انت

ساكن منطقي getParameterTraceEnable()
{
    ساكن منطقي param_traceEnable = utils::getConfigurationParameterBool("OPENCV_TRACE", خطأ);
    أرجع param_traceEnable;
}

// TODO lazy configuration flags
ساكن صحيح param_max_Region_ڈونگھائی_Open_CV = (صحيح)utils::get_Configuration_Parameter_ناپ_T("OPENCV_TRACE_DEPTH_OPENCV", 1);
ساكن صحيح param_maxRegionChildrenOpenCV = (صحيح)utils::get_Configuration_Parameter_ناپ_T("OPENCV_TRACE_MAX_CHILDREN_OPENCV", 1000);
ساكن صحيح param_maxRegionChildren = (صحيح)utils::get_Configuration_Parameter_ناپ_T("OPENCV_TRACE_MAX_CHILDREN", 10000);

ساكن ثابت cv::String& getParameterTraceLocation()
{
    ساكن cv::String param_traceLocation = utils::getConfigurationParameterString("OPENCV_TRACE_LOCATION", "OpenCVTrace");
    أرجع param_traceLocation;
}

#جے_تعریف_شدہ HAVE_OPENCL
ساكن منطقي param_synchronizeOpenCL = utils::getConfigurationParameterBool("OPENCV_TRACE_SYNC_OPENCL", خطأ);
#جے_انت

#جے_تعریف_شدہ OPENCV_WITH_ITT
ساكن منطقي param_ITT_رجسٹر_ماپا_Scope = utils::getConfigurationParameterBool("OPENCV_TRACE_ITT_PARENT", خطأ);
#جے_انت

ساكن ثابت حرف* _spaces(صحيح گنتی_2)
{
    ساكن ثابت حرف buf[64] =
"                                                               ";
    أرجع &buf[63 - (گنتی_2 & 63)];
}

/**
 * Text-based trace messages
 */
صنف Trace_سنیہا
{
عام:
    حرف بفر[1024];
    size_t len;
    منطقي hasError;

    Trace_سنیہا() :
        len(0),
        hasError(خطأ)
    {}

    منطقي لکھو(ثابت حرف* format, ...)
    {
        حرف* buf = &بفر[len];
        size_t sz = حجم(بفر) - len;
        va_list ap;
        va_start(ap, format);
        صحيح n = cv_vsnprintf(buf, (صحيح)sz, format, ap);
        va_end(ap);
        إذا (n < 0 || (size_t)n > sz)
        {
            hasError = سچ;
            أرجع خطأ;
        }
        len += n;
        أرجع سچ;
    }

    منطقي formatlocation(ثابت Region::LocationStaticStorage& location)
    {
        أرجع هذا->لکھو("l,%lld,\"%s\",%d,\"%s\",0x%llX\n",
                (لما لما صحيح)(*location.ppExtra)->global_location_id,
                location.filename,
                location.سطر,
                location.name,
                (لما لما صحيح)(location.flags & ~0xF0000000));
    }
    منطقي formatRegionEnter(ثابت Region& region)
    {
        منطقي ok = هذا->لکھو("b,%d,%lld,%lld,%lld",
                (صحيح)region.pImpl->threadID,
                (لما لما صحيح)region.pImpl->beginTimestamp,
                (لما لما صحيح)((*region.pImpl->location.ppExtra)->global_location_id),
                (لما لما صحيح)region.pImpl->global_region_id);
        إذا (region.pImpl->ماپا_Region && region.pImpl->ماپا_Region->pImpl)
        {
            إذا (region.pImpl->ماپا_Region->pImpl->threadID != region.pImpl->threadID)
                ok &= هذا->لکھو(",parentThread=%d,parent=%lld",
                        (صحيح)region.pImpl->ماپا_Region->pImpl->threadID,
                        (لما لما صحيح)region.pImpl->ماپا_Region->pImpl->global_region_id);
        }
        ok &= هذا->لکھو("\n");
        أرجع ok;
    }
    منطقي formatRegionLeave(ثابت Region& region, ثابت RegionStatistics& نتیجہ)
    {
        CV_DbgAssert(region.pImpl->سرا_Timestamp - region.pImpl->beginTimestamp == نتیجہ.duration);
        منطقي ok = هذا->لکھو("e,%d,%lld,%lld,%lld,%lld",
                (صحيح)region.pImpl->threadID,
                (لما لما صحيح)region.pImpl->سرا_Timestamp,
                (لما لما صحيح)(*region.pImpl->location.ppExtra)->global_location_id,
                (لما لما صحيح)region.pImpl->global_region_id,
                (لما لما صحيح)نتیجہ.duration);
        إذا (نتیجہ.currentSkippedRegions)
            ok &= هذا->لکھو(",skip=%d", (صحيح)نتیجہ.currentSkippedRegions);
#جے_تعریف_شدہ HAVE_IPP
        إذا (نتیجہ.durationImplIPP)
            ok &= هذا->لکھو(",tIPP=%lld", (لما لما صحيح)نتیجہ.durationImplIPP);
#جے_انت
#جے_تعریف_شدہ HAVE_OPENCL
        إذا (نتیجہ.durationImplOpenCL)
            ok &= هذا->لکھو(",tOCL=%lld", (لما لما صحيح)نتیجہ.durationImplOpenCL);
#جے_انت
        ok &= هذا->لکھو("\n");
        أرجع ok;
    }
    منطقي ریکارڈ_Region_Arg(ثابت Region& region, ثابت TraceArg& arg, ثابت حرف* value)
    {
        أرجع هذا->لکھو("a,%d,%lld,%lld,\"%s\",\"%s\"\n",
                region.pImpl->threadID,
                (لما لما صحيح)region.pImpl->beginTimestamp,
                (لما لما صحيح)region.pImpl->global_region_id,
                arg.name,
                value);
    }
};


#جے_تعریف_شدہ OPENCV_WITH_ITT
ساكن __itt_domain* domain = عدم;

ساكن منطقي isITTEnabled()
{
    ساكن متطاير منطقي isInitialized = خطأ;
    ساكن منطقي isEnabled = خطأ;
    إذا (!isInitialized)
    {
        cv::Auto_جندرا lock(cv::getInitializationMutex());
        إذا (!isInitialized)
        {
            منطقي param_traceITTEnable = utils::getConfigurationParameterBool("OPENCV_TRACE_ITT_ENABLE", سچ);
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
            isInitialized = سچ;
        }
    }
    أرجع isEnabled;
}
#جے_انت


Region::Location_Extra_انکڑے::Location_Extra_انکڑے(ثابت LocationStaticStorage& location)
{
    CV_بن_ورتیا(location);
    ساكن صحيح g_location_id_counter = 0;
    global_location_id = CV_XADD(&g_location_id_counter, 1) + 1;
    CV_LOG("Register location: " << global_location_id << " (" << (فراغ*)&location << ")"
            << std::سطر_انت << "    file: " << location.filename
            << std::سطر_انت << "    line: " << location.سطر
            << std::سطر_انت << "    name: " << location.name);
#جے_تعریف_شدہ OPENCV_WITH_ITT
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
#جے_انت
}

/*static*/ Region::Location_Extra_انکڑے* Region::Location_Extra_انکڑے::init(ثابت Region::LocationStaticStorage& location)
{
    Location_Extra_انکڑے** pLocationExtra = location.ppExtra;
    CV_DbgAssert(pLocationExtra);
    إذا (*pLocationExtra == عدم)
    {
        cv::Auto_جندرا lock(cv::getInitializationMutex());
        إذا (*pLocationExtra == عدم)
        {
            *pLocationExtra = جديد Region::Location_Extra_انکڑے(location);
            TraceStorage* s = getTraceManager().trace_storage.get();
            إذا (s)
            {
                Trace_سنیہا پیغام;
                پیغام.formatlocation(location);
                s->put(پیغام);
            }
        }
    }
    أرجع *pLocationExtra;
}


Region::Impl::Impl(TraceManagerThreadLocal& ctx, Region* ماپا_Region_2, Region& region_, ثابت LocationStaticStorage& location_, int64 beginTimestamp_) :
    location(location_),
    region(region_),
    ماپا_Region(ماپا_Region_2),
    threadID(ctx.threadID),
    global_region_id(++ctx.region_counter),
    beginTimestamp(beginTimestamp_),
    سرا_Timestamp(0),
    سدھا_Children_گنتی(0)
#جے_تعریف_شدہ OPENCV_WITH_ITT
    ,itt_id_registered(خطأ)
    ,itt_id(__itt_null)
#جے_انت
{
    CV_DbgAssert(ctx.current_سرگرم_Region == ماپا_Region);
    region.pImpl = هذا;

    رجسٹر_Region(ctx);

    enterRegion(ctx);
}

Region::Impl::~Impl()
{
#جے_تعریف_شدہ OPENCV_WITH_ITT
    إذا (itt_id_registered)
    {
        CV_LOG_ITT(" Destroy ITT region: I=" << (فراغ*)هذا);
        __itt_id_destroy(domain, itt_id);
        itt_id_registered = خطأ;
    }
#جے_انت
    region.pImpl = عدم;
}

فراغ Region::Impl::enterRegion(TraceManagerThreadLocal& ctx)
{
    ctx.current_سرگرم_Region = &region;

    إذا (location.flags & REGION_FLAG_FUNCTION)
    {
        إذا ((location.flags & REGION_FLAG_APP_کوڈ) == 0)
        {
            ctx.region_ڈونگھائی_Open_CV++;
        }
        ctx.region_ڈونگھائی++;
    }

    TraceStorage* s = ctx.getStorage();
    إذا (s)
    {
        Trace_سنیہا پیغام;
        پیغام.formatRegionEnter(region);
        s->put(پیغام);
    }
#جے_تعریف_شدہ OPENCV_WITH_ITT
    إذا (isITTEnabled())
    {
        __itt_id ماپا_ID = __itt_null;
        إذا (param_ITT_رجسٹر_ماپا_Scope && ماپا_Region && ماپا_Region->pImpl && ماپا_Region->pImpl->itt_id_registered && (location.flags & REGION_FLAG_REGION_FORCE) == 0)
            ماپا_ID = ماپا_Region->pImpl->itt_id;
        __itt_task_begin(domain, itt_id, ماپا_ID, (*location.ppExtra)->ittHandle_name);
    }
#جے_انت
}

فراغ Region::Impl::leaveRegion(TraceManagerThreadLocal& ctx)
{
    int64 duration = سرا_Timestamp - beginTimestamp; CV_بن_ورتیا(duration);
    RegionStatistics نتیجہ;
    ctx.stat.grab(نتیجہ);
    ctx.کل_Skipped_Events += نتیجہ.currentSkippedRegions;
    CV_LOG(_spaces(ctx.get_Current_ڈونگھائی()*4) << "leaveRegion(): " << (فراغ*)هذا << " " << نتیجہ);
#جے_تعریف_شدہ OPENCV_WITH_ITT
    إذا (isITTEnabled())
    {
        إذا (نتیجہ.currentSkippedRegions)
        {
            __itt_metadata_add(domain, itt_id, __itt_string_handle_create("skipped trace entries"), __itt_metadata_u32, 1, &نتیجہ.currentSkippedRegions);
        }
#جے_تعریف_شدہ HAVE_IPP
        إذا (نتیجہ.durationImplIPP)
            __itt_metadata_add(domain, itt_id, __itt_string_handle_create("tIPP"), __itt_metadata_u64, 1, &نتیجہ.durationImplIPP);
#جے_انت
#جے_تعریف_شدہ HAVE_OPENCL
        إذا (نتیجہ.durationImplOpenCL)
            __itt_metadata_add(domain, itt_id, __itt_string_handle_create("tOpenCL"), __itt_metadata_u64, 1, &نتیجہ.durationImplOpenCL);
#جے_انت
        __itt_task_end(domain);
    }
#جے_انت
    TraceStorage* s = ctx.getStorage();
    إذا (s)
    {
        Trace_سنیہا پیغام;
        پیغام.formatRegionLeave(region, نتیجہ);
        s->put(پیغام);
    }

    إذا (location.flags & REGION_FLAG_FUNCTION)
    {
        إذا ((location.flags & REGION_FLAG_APP_کوڈ) == 0)
        {
            ctx.region_ڈونگھائی_Open_CV--;
        }
        ctx.region_ڈونگھائی--;
    }

    ctx.current_سرگرم_Region = ماپا_Region;
}

فراغ Region::Impl::release()
{
    مٹاؤ هذا;
}

فراغ Region::Impl::رجسٹر_Region(TraceManagerThreadLocal& ctx)
{
#جے_تعریف_شدہ OPENCV_WITH_ITT
    إذا (isITTEnabled())
    {
        إذا (!itt_id_registered)
        {
            CV_LOG_ITT(" Register ITT region: I=" << (فراغ*)هذا << " " << ctx.threadID << "-" << global_region_id);
#إذا 1 // workaround for some ITT backends
            itt_id = __itt_id_make((فراغ*)(intptr_t)(((int64)(ctx.threadID + 1) << 32) | global_region_id), global_region_id);
#وإلا
            itt_id = __itt_id_make((فراغ*)(intptr_t)(ctx.threadID + 1), global_region_id);
#جے_انت
            __itt_id_create(domain, itt_id);
            itt_id_registered = سچ;
        }
    }
#وإلا
    CV_بن_ورتیا(ctx);
#جے_انت
}

فراغ Region_Statistics_حالت::enable_Skip_منوال(صحيح ڈونگھائی)
{
    CV_DbgAssert(skip_ڈونگھائی < 0);
    CV_LOG_SKIP(عدم, "SKIP-ENABLE: depth=" << ڈونگھائی);
    skip_ڈونگھائی = ڈونگھائی;
}
فراغ Region_Statistics_حالت::check_Reset_Skip_منوال(صحيح leave_ڈونگھائی)
{
    إذا (leave_ڈونگھائی <= skip_ڈونگھائی)
    {
        CV_LOG_SKIP(عدم, "SKIP-RESET: leaveDepth=" << leave_ڈونگھائی << " skipDepth=" << skip_ڈونگھائی);
        skip_ڈونگھائی = -1;
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
    CV_LOG(_spaces(ctx.get_Current_ڈونگھائی()*4) << "Region(): " << (فراغ*)هذا << ": " << location.name);

    Region* ماپا_Region = ctx.ڈھیر_چوٹی_Region();
    ثابت Region::LocationStaticStorage* ماپا_Location = ctx.ڈھیر_چوٹی_Location();

    إذا (location.flags & REGION_FLAG_REGION_اگلا)
    {
        إذا (ماپا_Region && ماپا_Region->pImpl)
        {
            CV_DbgAssert((ماپا_Region->pImpl->location.flags & REGION_FLAG_FUNCTION) == 0);
            ماپا_Region->destroy(); ماپا_Region->implFlags = 0;
            ماپا_Region = ctx.ڈھیر_چوٹی_Region();
            ماپا_Location = ctx.ڈھیر_چوٹی_Location();
        }
    }

    صحيح ماپا_Children = 0;
    إذا (ماپا_Region && ماپا_Region->pImpl)
    {
        إذا (ماپا_Location == عدم)
        {
            // parallel_for_body code path
            ماپا_Children = CV_XADD(&ماپا_Region->pImpl->سدھا_Children_گنتی, 1) + 1;
        }
        وإلا
        {
            ماپا_Children = ++ماپا_Region->pImpl->سدھا_Children_گنتی;
        }
    }

    int64 beginTimestamp = getTimestampNS();

    صحيح current_ڈونگھائی = ctx.get_Current_ڈونگھائی() + 1;
    بدل (location.flags & REGION_FLAG_IMPL_نقاب)
    {
#جے_تعریف_شدہ HAVE_IPP
    حالة REGION_FLAG_IMPL_IPP:
        إذا (!ctx.stat_حالت.ignore_ڈونگھائی_Impl_IPP)
            ctx.stat_حالت.ignore_ڈونگھائی_Impl_IPP = current_ڈونگھائی;
        اكسر;
#جے_انت
#جے_تعریف_شدہ HAVE_OPENCL
    حالة REGION_FLAG_IMPL_OPENCL:
        إذا (!ctx.stat_حالت.ignore_ڈونگھائی_Impl_Open_CL)
            ctx.stat_حالت.ignore_ڈونگھائی_Impl_Open_CL = current_ڈونگھائی;
        اكسر;
#جے_انت
    افتراضي:
        اكسر;
    }

    ctx.ڈھیر_ڈالو(هذا, &location, beginTimestamp);
    implFlags |= REGION_FLAG_NEED_ڈھیر_نکالو;

    إذا ((location.flags & REGION_FLAG_REGION_FORCE) == 0)
    {
        إذا (ctx.stat_حالت.skip_ڈونگھائی >= 0 && current_ڈونگھائی > ctx.stat_حالت.skip_ڈونگھائی)
        {
            CV_LOG(_spaces(ctx.get_Current_ڈونگھائی()*4) << "Parent region is disabled. Bailout");
            ctx.stat.currentSkippedRegions++;
            أرجع;
        }

        إذا (param_maxRegionChildrenOpenCV > 0 && (location.flags & REGION_FLAG_APP_کوڈ) == 0 && ماپا_Location && (ماپا_Location->flags & REGION_FLAG_APP_کوڈ) == 0)
        {
            إذا (ماپا_Children >= param_maxRegionChildrenOpenCV)
            {
                CV_LOG_TRACE_BAILOUT(عدم, _spaces(ctx.get_Current_ڈونگھائی()*4) << "OpenCV parent region exceeds children count. Bailout");
                ctx.stat_حالت.enable_Skip_منوال(current_ڈونگھائی - 1);
                ctx.stat.currentSkippedRegions++;
                DEBUG_ONLY(ctx.dump_ڈھیر(std::باہر, خطأ));
                أرجع;
            }
        }
        إذا (param_maxRegionChildren > 0 && ماپا_Children >= param_maxRegionChildren)
        {
            CV_LOG_TRACE_BAILOUT(عدم, _spaces(ctx.get_Current_ڈونگھائی()*4) << "Parent region exceeds children count. Bailout");
            ctx.stat_حالت.enable_Skip_منوال(current_ڈونگھائی - 1);
            ctx.stat.currentSkippedRegions++;
            DEBUG_ONLY(ctx.dump_ڈھیر(std::باہر, خطأ));
            أرجع;
        }
    }

    Location_Extra_انکڑے::init(location);

    إذا ((*location.ppExtra)->global_location_id == 0)
    {
        CV_LOG_TRACE_BAILOUT(عدم, _spaces(ctx.get_Current_ڈونگھائی()*4) << "Region location is disabled. Bailout");
        ctx.stat_حالت.enable_Skip_منوال(current_ڈونگھائی);
        ctx.stat.currentSkippedRegions++;
        أرجع;
    }

    إذا (ماپا_Location && (ماپا_Location->flags & REGION_FLAG_SKIP_NESTED))
    {
        CV_LOG(_spaces(ctx.get_Current_ڈونگھائی()*4) << "Parent region disables inner regions. Bailout");
        ctx.stat_حالت.enable_Skip_منوال(current_ڈونگھائی);
        ctx.stat.currentSkippedRegions++;
        أرجع;
    }

    إذا (param_max_Region_ڈونگھائی_Open_CV)
    {
        إذا ((location.flags & REGION_FLAG_APP_کوڈ) == 0)
        {
            إذا (ctx.region_ڈونگھائی_Open_CV >= param_max_Region_ڈونگھائی_Open_CV)
            {
                CV_LOG(_spaces(ctx.get_Current_ڈونگھائی()*4) << "OpenCV region depth is exceed = " << ctx.region_ڈونگھائی_Open_CV << ". Bailout");
                إذا (ctx.stat.currentSkippedRegions == 0)
                {
                    DEBUG_ONLY(ctx.dump_ڈھیر(std::باہر, خطأ));
                }
                ctx.stat_حالت.enable_Skip_منوال(current_ڈونگھائی);
                ctx.stat.currentSkippedRegions++;
                أرجع;
            }
        }
    }

    جديد Impl(ctx, ماپا_Region, *هذا, location, beginTimestamp);
    CV_DbgAssert(pImpl != عدم);
    implFlags |= REGION_FLAG_سرگرم;

    // parallel_for path
    إذا (ماپا_Region && ماپا_Region->pImpl)
    {
        إذا (ماپا_Location == عدم)
        {
            pImpl->سدھا_Children_گنتی = ماپا_Children;
        }
    }
}

فراغ Region::destroy()
{
    CV_DbgAssert(implFlags != 0);

    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    CV_LOG(_spaces(ctx.get_Current_ڈونگھائی()*4) << "Region::destruct(): " << (فراغ*)هذا << " pImpl=" << pImpl << " implFlags=" << implFlags << ' ' << (ctx.ڈھیر_چوٹی_Location() ? ctx.ڈھیر_چوٹی_Location()->name : "<unknown>"));

    CV_DbgAssert(implFlags & REGION_FLAG_NEED_ڈھیر_نکالو);
    ثابت صحيح current_ڈونگھائی = ctx.get_Current_ڈونگھائی(); CV_بن_ورتیا(current_ڈونگھائی);

    CV_LOG_CTX_STAT(عدم, _spaces(current_ڈونگھائی*4) << ctx.stat << ' ' << ctx.stat_حالت);

    ثابت Region::LocationStaticStorage* location = ctx.ڈھیر_چوٹی_Location();
    Impl::Optimization_رستہ my_کوڈ_رستہ = Impl::کوڈ_رستہ_PLAIN;
    إذا (location)
    {
        بدل (location->flags & REGION_FLAG_IMPL_نقاب)
        {
#جے_تعریف_شدہ HAVE_IPP
        حالة REGION_FLAG_IMPL_IPP:
            my_کوڈ_رستہ = Impl::کوڈ_رستہ_IPP;
            اكسر;
#جے_انت
#جے_تعریف_شدہ HAVE_OPENCL
        حالة REGION_FLAG_IMPL_OPENCL:
            إذا (param_synchronizeOpenCL && cv::ocl::isOpenCLActivated())
                cv::ocl::انت_2();
            my_کوڈ_رستہ = Impl::کوڈ_رستہ_OPENCL;
            اكسر;
#جے_انت
        افتراضي:
            اكسر;
        }
    }

    int64 سرا_Timestamp = getTimestampNS();
    int64 duration = سرا_Timestamp - ctx.ڈھیر_چوٹی_Begin_Timestamp();

    منطقي سرگرم = is_سرگرم_2();

    إذا (سرگرم)
        ctx.stat.duration = duration;
    وإلا إذا (ctx.ڈھیر.size() == ctx.parallel_for_ڈھیر_ناپ + 1)
        ctx.stat.duration += duration;

    بدل (my_کوڈ_رستہ) {
        حالة Impl::کوڈ_رستہ_PLAIN:
            // nothing
            اكسر;
#جے_تعریف_شدہ HAVE_IPP
        حالة Impl::کوڈ_رستہ_IPP:
            إذا (ctx.stat_حالت.ignore_ڈونگھائی_Impl_IPP == current_ڈونگھائی)
            {
                ctx.stat.durationImplIPP += duration;
                ctx.stat_حالت.ignore_ڈونگھائی_Impl_IPP = 0;
            }
            وإلا إذا (سرگرم)
            {
                ctx.stat.durationImplIPP = duration;
            }
            اكسر;
#جے_انت
#جے_تعریف_شدہ HAVE_OPENCL
        حالة Impl::کوڈ_رستہ_OPENCL:
            إذا (ctx.stat_حالت.ignore_ڈونگھائی_Impl_Open_CL == current_ڈونگھائی)
            {
                ctx.stat.durationImplOpenCL += duration;
                ctx.stat_حالت.ignore_ڈونگھائی_Impl_Open_CL = 0;
            }
            وإلا إذا (سرگرم)
            {
                ctx.stat.durationImplOpenCL = duration;
            }
            اكسر;
#جے_انت
        افتراضي:
            اكسر;
    }

    إذا (pImpl)
    {
        CV_DbgAssert((implFlags & (REGION_FLAG_سرگرم | REGION_FLAG_NEED_ڈھیر_نکالو)) == (REGION_FLAG_سرگرم | REGION_FLAG_NEED_ڈھیر_نکالو));
        CV_DbgAssert(ctx.ڈھیر_چوٹی_Region() == هذا);
        pImpl->سرا_Timestamp = سرا_Timestamp;
        pImpl->leaveRegion(ctx);
        pImpl->release();
        pImpl = عدم;
        DEBUG_ONLY(implFlags &= ~REGION_FLAG_سرگرم);
    }
    وإلا
    {
        CV_DbgAssert(ctx.stat_حالت.skip_ڈونگھائی <= current_ڈونگھائی);
    }

    إذا (implFlags & REGION_FLAG_NEED_ڈھیر_نکالو)
    {
        CV_DbgAssert(ctx.ڈھیر_چوٹی_Region() == هذا);
        ctx.ڈھیر_نکالو();
        ctx.stat_حالت.check_Reset_Skip_منوال(current_ڈونگھائی);
        DEBUG_ONLY(implFlags &= ~REGION_FLAG_NEED_ڈھیر_نکالو);
    }
    CV_LOG_CTX_STAT(عدم, _spaces(current_ڈونگھائی*4) << "===> " << ctx.stat << ' ' << ctx.stat_حالت);
}


TraceManagerThreadLocal::~TraceManagerThreadLocal()
{
}

فراغ TraceManagerThreadLocal::dump_ڈھیر(std::ostream& out, منطقي onlyFunctions) ثابت
{
    std::stringstream ss;
    std::deque<ڈھیر_اندراج>::const_iterator it = ڈھیر.begin();
    std::deque<ڈھیر_اندراج>::const_iterator end = ڈھیر.end();
    صحيح ڈونگھائی = 0;
    لكل (; it != end; ++it)
    {
        ثابت Region::LocationStaticStorage* location = it->location;
        إذا (location)
        {
            إذا (!onlyFunctions || (location->flags & REGION_FLAG_FUNCTION))
            {
                ss << _spaces(4*ڈونگھائی) << location->name << std::سطر_انت;
                ڈونگھائی++;
            }
        }
        وإلا
        {
            ss << _spaces(4*ڈونگھائی) << "<unknown>" << std::سطر_انت;
            ڈونگھائی++;
        }
    }
    out << ss.str();
}

صنف AsyncTraceStorage CV_FINAL : عام TraceStorage
{
    بدلنجوگ std::ofstream out;
عام:
    ثابت std::string name;

    AsyncTraceStorage(ثابت std::string& filename) :
        out(filename.c_str(), std::ios::trunc),
        name(filename)
    {
        out << "#description: OpenCV trace file" << std::سطر_انت;
        out << "#version: 1.0" << std::سطر_انت;
    }
    ~AsyncTraceStorage()
    {
        out.close();
    }

    منطقي put(ثابت Trace_سنیہا& پیغام) ثابت CV_OVERRIDE
    {
        إذا (پیغام.hasError)
            أرجع خطأ;
        out << پیغام.بفر;
        //DEBUG_ONLY(std::flush(out)); // TODO configure flag
        أرجع سچ;
    }
};

صنف SyncTraceStorage CV_FINAL : عام TraceStorage
{
    بدلنجوگ std::ofstream out;
    بدلنجوگ cv::Mutex mutex;
عام:
    ثابت std::string name;

    SyncTraceStorage(ثابت std::string& filename) :
        out(filename.c_str(), std::ios::trunc),
        name(filename)
    {
        out << "#description: OpenCV trace file" << std::سطر_انت;
        out << "#version: 1.0" << std::سطر_انت;
    }
    ~SyncTraceStorage()
    {
        cv::Auto_جندرا l(mutex);
        out.close();
    }

    منطقي put(ثابت Trace_سنیہا& پیغام) ثابت CV_OVERRIDE
    {
        إذا (پیغام.hasError)
            أرجع خطأ;
        {
            cv::Auto_جندرا l(mutex);
            out << پیغام.بفر;
            std::flush(out); // TODO configure flag
        }
        أرجع سچ;
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
            Trace_سنیہا پیغام;
            ثابت حرف* pos = strrchr(filepath.c_str(), '/'); // extract filename
#جے_تعریف_شدہ _WIN32
            إذا (!pos)
                pos = strrchr(filepath.c_str(), '\\');
#جے_انت
            إذا (!pos)
                pos = filepath.c_str();
            وإلا
                pos += 1; // fix to skip extra slash in filename beginning
            پیغام.لکھو("#thread file: %s\n", pos);
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

    isInitialized = سچ;
    CV_LOG("TraceManager ctor: " << (فراغ*)هذا);

    CV_LOG("TraceManager configure()");
    activated = getParameterTraceEnable();

    إذا (activated)
        trace_storage.reset(جديد SyncTraceStorage(std::string(getParameterTraceLocation()) + ".txt"));

#جے_تعریف_شدہ OPENCV_WITH_ITT
    إذا (isITTEnabled())
    {
        activated = سچ; // force trace pipeline activation (without OpenCV storage)
        __itt_region_begin(domain, __itt_null, __itt_null, __itt_string_handle_create("OpenCVTrace"));
    }
#جے_انت
}
TraceManager::~TraceManager()
{
    CV_LOG("TraceManager dtor: " << (فراغ*)هذا);

#جے_تعریف_شدہ OPENCV_WITH_ITT
    إذا (isITTEnabled())
    {
        __itt_region_end(domain, __itt_null);
    }
#جے_انت

    std::vector<TraceManagerThreadLocal*> دھاگے_ctx;
    tls.gather(دھاگے_ctx);
    size_t کل_Events = 0, کل_Skipped_Events = 0;
    لكل (size_t i = 0; i < دھاگے_ctx.size(); i++)
    {
        TraceManagerThreadLocal* ctx = دھاگے_ctx[i];
        إذا (ctx)
        {
            کل_Events += ctx->region_counter;
            کل_Skipped_Events += ctx->کل_Skipped_Events;
        }
    }
    إذا (کل_Events || activated)
    {
        CV_LOG_INFO(عدم, "Trace: Total events: " << کل_Events);
    }
    إذا (کل_Skipped_Events)
    {
        CV_LOG_WARNING(عدم, "Trace: Total skipped events: " << کل_Skipped_Events);
    }

    // This is a global static object, so process starts shutdown here
    // Turn off trace
    cv::__termination = سچ; // also set in DllMain() notifications handler for DLL_PROCESS_DETACH
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
        CV_بن_ورتیا(m); // TODO
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

فراغ parallel_For_سیٹ_جڑ_Region(ثابت Region& جڑ_Region, ثابت TraceManagerThreadLocal& جڑ_ctx)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();

    إذا (ctx.dummy_ڈھیر_چوٹی.region == &جڑ_Region) // already attached
        أرجع;

    CV_Assert(ctx.dummy_ڈھیر_چوٹی.region == عدم);
    ctx.dummy_ڈھیر_چوٹی = TraceManagerThreadLocal::ڈھیر_اندراج(const_cast<Region*>(&جڑ_Region), عدم, -1);

    إذا (&ctx == &جڑ_ctx)
    {
        ctx.stat.grab(ctx.parallel_for_stat);
        ctx.parallel_for_stat_حالت = ctx.stat_حالت;
        ctx.parallel_for_ڈھیر_ناپ = ctx.ڈھیر.size();
        أرجع;
    }

    CV_Assert(ctx.ڈھیر.empty());

    ctx.current_سرگرم_Region = const_cast<Region*>(&جڑ_Region);

    ctx.region_ڈونگھائی = جڑ_ctx.region_ڈونگھائی;
    ctx.region_ڈونگھائی_Open_CV = جڑ_ctx.region_ڈونگھائی_Open_CV;

    ctx.parallel_for_ڈھیر_ناپ = 0;

    ctx.stat_حالت.propagateFrom(جڑ_ctx.stat_حالت);
}

فراغ parallelForAttachNestedRegion(ثابت Region& جڑ_Region)
{
    CV_بن_ورتیا(جڑ_Region);
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();

    CV_DbgAssert(ctx.dummy_ڈھیر_چوٹی.region == &جڑ_Region);

    Region* region = ctx.get_Current_سرگرم_Region();
    CV_LOG_PARALLEL(عدم, " PARALLEL_FOR: " << (فراغ*)region << " ==> " << &جڑ_Region);
    إذا (!region)
        أرجع;

#جے_تعریف_شدہ OPENCV_WITH_ITT
    إذا (!جڑ_Region.pImpl || !جڑ_Region.pImpl->itt_id_registered)
        أرجع;

    إذا (!region->pImpl)
        أرجع;

    CV_LOG_PARALLEL(عدم, " PARALLEL_FOR ITT: " << (فراغ*)جڑ_Region.pImpl->itt_id.d1 << ":" << جڑ_Region.pImpl->itt_id.d2 << ":" << (فراغ*)جڑ_Region.pImpl->itt_id.d3 << " => "
                                 << (فراغ*)region->pImpl->itt_id.d1 << ":" << region->pImpl->itt_id.d2 << ":" << (فراغ*)region->pImpl->itt_id.d3);
    __itt_relation_add(domain, region->pImpl->itt_id, __itt_relation_is_child_of, جڑ_Region.pImpl->itt_id);
#جے_انت
}

فراغ parallelForFinalize(ثابت Region& جڑ_Region)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();

    int64 سرا_Timestamp = getTimestampNS();
    int64 duration = سرا_Timestamp - ctx.ڈھیر_چوٹی_Begin_Timestamp();
    CV_LOG_PARALLEL(عدم, "parallel_for duration: " << duration << " " << &جڑ_Region);

    std::vector<TraceManagerThreadLocal*> دھاگے_ctx;
    getTraceManager().tls.gather(دھاگے_ctx);
    RegionStatistics parallel_for_stat;
    لكل (size_t i = 0; i < دھاگے_ctx.size(); i++)
    {
        TraceManagerThreadLocal* بچہ_ctx = دھاگے_ctx[i];

        إذا (بچہ_ctx && بچہ_ctx->ڈھیر_چوٹی_Region() == &جڑ_Region)
        {
            CV_LOG_PARALLEL(عدم, "Thread=" << بچہ_ctx->threadID << " " << بچہ_ctx->stat);
            RegionStatistics بچہ_stat;
            بچہ_ctx->stat.grab(بچہ_stat);
            parallel_for_stat.append(بچہ_stat);
            إذا (بچہ_ctx != &ctx)
            {
                بچہ_ctx->dummy_ڈھیر_چوٹی = TraceManagerThreadLocal::ڈھیر_اندراج();
            }
            وإلا
            {
                ctx.parallel_for_stat.grab(ctx.stat);
                ctx.stat_حالت = ctx.parallel_for_stat_حالت;
                بچہ_ctx->dummy_ڈھیر_چوٹی = TraceManagerThreadLocal::ڈھیر_اندراج();
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

بنية TraceArg::Extra_انکڑے
{
#جے_تعریف_شدہ OPENCV_WITH_ITT
    // Special fields for ITT
    __itt_string_handle* متطاير ittHandle_name;
#جے_انت
    Extra_انکڑے(TraceManagerThreadLocal& ctx, ثابت TraceArg& arg)
    {
        CV_بن_ورتیا(ctx); CV_بن_ورتیا(arg);
#جے_تعریف_شدہ OPENCV_WITH_ITT
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
#جے_انت
    }
};

ساكن فراغ initTraceArg(TraceManagerThreadLocal& ctx, ثابت TraceArg& arg)
{
    TraceArg::Extra_انکڑے** pExtra = arg.ppExtra;
    إذا (*pExtra == عدم)
    {
        cv::Auto_جندرا lock(cv::getInitializationMutex());
        إذا (*pExtra == عدم)
        {
            *pExtra = جديد TraceArg::Extra_انکڑے(ctx, arg);
        }
    }
}
فراغ traceArg(ثابت TraceArg& arg, ثابت حرف* value)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    Region* region = ctx.get_Current_سرگرم_Region();
    إذا (!region)
        أرجع;
    CV_Assert(region->pImpl);
    initTraceArg(ctx, arg);
    إذا (!value)
        value = "<null>";
#جے_تعریف_شدہ OPENCV_WITH_ITT
    إذا (isITTEnabled())
    {
        __itt_metadata_str_add(domain, region->pImpl->itt_id, (*arg.ppExtra)->ittHandle_name, value, لڑی_لمبائی(value));
    }
#جے_انت
}
فراغ traceArg(ثابت TraceArg& arg, صحيح value)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    Region* region = ctx.get_Current_سرگرم_Region();
    إذا (!region)
        أرجع;
    CV_Assert(region->pImpl);
    initTraceArg(ctx, arg);
#جے_تعریف_شدہ OPENCV_WITH_ITT
    إذا (isITTEnabled())
    {
        __itt_metadata_add(domain, region->pImpl->itt_id, (*arg.ppExtra)->ittHandle_name, حجم(صحيح) == 4 ? __itt_metadata_s32 : __itt_metadata_s64, 1, &value);
    }
#وإلا
    CV_بن_ورتیا(value);
#جے_انت
}
فراغ traceArg(ثابت TraceArg& arg, int64 value)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    Region* region = ctx.get_Current_سرگرم_Region();
    إذا (!region)
        أرجع;
    CV_Assert(region->pImpl);
    initTraceArg(ctx, arg);
#جے_تعریف_شدہ OPENCV_WITH_ITT
    إذا (isITTEnabled())
    {
        __itt_metadata_add(domain, region->pImpl->itt_id, (*arg.ppExtra)->ittHandle_name, __itt_metadata_s64, 1, &value);
    }
#وإلا
    CV_بن_ورتیا(value);
#جے_انت
}
فراغ traceArg(ثابت TraceArg& arg, مزدوج value)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    Region* region = ctx.get_Current_سرگرم_Region();
    إذا (!region)
        أرجع;
    CV_Assert(region->pImpl);
    initTraceArg(ctx, arg);
#جے_تعریف_شدہ OPENCV_WITH_ITT
    إذا (isITTEnabled())
    {
        __itt_metadata_add(domain, region->pImpl->itt_id, (*arg.ppExtra)->ittHandle_name, __itt_metadata_double, 1, &value);
    }
#وإلا
    CV_بن_ورتیا(value);
#جے_انت
}

#وإلا

Region::Region(ثابت LocationStaticStorage&) : pImpl(عدم), implFlags(0) {}
فراغ Region::destroy() {}

فراغ traceArg(ثابت TraceArg&, ثابت حرف*) {}
فراغ traceArg(ثابت TraceArg&, صحيح) {};
فراغ traceArg(ثابت TraceArg&, int64) {};
فراغ traceArg(ثابت TraceArg&, مزدوج) {};

#جے_انت

}}}} // namespace
