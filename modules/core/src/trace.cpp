// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#شامل "precomp.hpp"

#شامل <opencv2/core/utils/trace.hpp>
#شامل <opencv2/core/utils/trace.private.hpp>
#شامل <opencv2/core/utils/configuration.private.hpp>

#شامل <opencv2/core/opencl/ocl_defs.hpp>

#شامل <cstdarg> // va_start

#شامل <sstream>
#شامل <ostream>
#شامل <fstream>

#اگر 0
#تعریف_کرو CV_LOG(...) CV_LOG_INFO(تہی, __VA_ARGS__)
#ورنہ
#تعریف_کرو CV_LOG(...) {}
#اگر_ختم

#اگر 0
#تعریف_کرو CV_LOG_ITT(...) CV_LOG_INFO(تہی, __VA_ARGS__)
#ورنہ
#تعریف_کرو CV_LOG_ITT(...) {}
#اگر_ختم

#اگر 1
#تعریف_کرو CV_LOG_TRACE_BAILOUT(لیبل, ...) CV_LOG_INFO(لیبل, __VA_ARGS__)
#ورنہ
#تعریف_کرو CV_LOG_TRACE_BAILOUT(...) {}
#اگر_ختم

#اگر 0
#تعریف_کرو CV_LOG_PARALLEL(لیبل, ...) CV_LOG_INFO(لیبل, __VA_ARGS__)
#ورنہ
#تعریف_کرو CV_LOG_PARALLEL(...) {}
#اگر_ختم

#اگر 0
#تعریف_کرو CV_LOG_CTX_STAT(لیبل, ...) CV_LOG_INFO(لیبل, __VA_ARGS__)
#ورنہ
#تعریف_کرو CV_LOG_CTX_STAT(...) {}
#اگر_ختم

#اگر 0
#تعریف_کرو CV_LOG_SKIP(لیبل, ...) CV_LOG_INFO(لیبل, __VA_ARGS__)
#ورنہ
#تعریف_کرو CV_LOG_SKIP(...) {}
#اگر_ختم

نامخلا cv {
نامخلا utils {
نامخلا trace {
نامخلا details {

#اگر_تعریف_شدہ OPENCV_TRACE

#اگر_تعریف_شدہ _MSC_VER
#ہدایت warning(disable:4065) // switch statement contains 'default' but no 'case' labels
#اگر_ختم

ساکن منطقی getParameterTraceEnable()
{
    ساکن منطقی param_traceEnable = utils::getConfigurationParameterBool("OPENCV_TRACE", جھوٹ);
    واپس param_traceEnable;
}

// TODO lazy configuration flags
ساکن صحیح param_max_Region_گہرائی_Open_CV = (صحیح)utils::get_Configuration_Parameter_سائز_T("OPENCV_TRACE_DEPTH_OPENCV", 1);
ساکن صحیح param_maxRegionChildrenOpenCV = (صحیح)utils::get_Configuration_Parameter_سائز_T("OPENCV_TRACE_MAX_CHILDREN_OPENCV", 1000);
ساکن صحیح param_maxRegionChildren = (صحیح)utils::get_Configuration_Parameter_سائز_T("OPENCV_TRACE_MAX_CHILDREN", 10000);

ساکن ثابت cv::String& getParameterTraceLocation()
{
    ساکن cv::String param_traceLocation = utils::getConfigurationParameterString("OPENCV_TRACE_LOCATION", "OpenCVTrace");
    واپس param_traceLocation;
}

#اگر_تعریف_شدہ HAVE_OPENCL
ساکن منطقی param_synchronizeOpenCL = utils::getConfigurationParameterBool("OPENCV_TRACE_SYNC_OPENCL", جھوٹ);
#اگر_ختم

#اگر_تعریف_شدہ OPENCV_WITH_ITT
ساکن منطقی param_ITT_رجسٹر_والد_Scope = utils::getConfigurationParameterBool("OPENCV_TRACE_ITT_PARENT", جھوٹ);
#اگر_ختم

ساکن ثابت حرف* _spaces(صحیح گنتی_2)
{
    ساکن ثابت حرف buf[64] =
"                                                               ";
    واپس &buf[63 - (گنتی_2 & 63)];
}

/**
 * Text-based trace messages
 */
جماعت Trace_پیغام
{
عوامی:
    حرف بفر[1024];
    size_t len;
    منطقی hasError;

    Trace_پیغام() :
        len(0),
        hasError(جھوٹ)
    {}

    منطقی لکھو(ثابت حرف* format, ...)
    {
        حرف* buf = &بفر[len];
        size_t sz = حجم(بفر) - len;
        va_list ap;
        va_start(ap, format);
        صحیح n = cv_vsnprintf(buf, (صحیح)sz, format, ap);
        va_end(ap);
        اگر (n < 0 || (size_t)n > sz)
        {
            hasError = سچ;
            واپس جھوٹ;
        }
        len += n;
        واپس سچ;
    }

    منطقی formatlocation(ثابت Region::LocationStaticStorage& location)
    {
        واپس یہ->لکھو("l,%lld,\"%s\",%d,\"%s\",0x%llX\n",
                (طویل طویل صحیح)(*location.ppExtra)->global_location_id,
                location.filename,
                location.سطر,
                location.name,
                (طویل طویل صحیح)(location.flags & ~0xF0000000));
    }
    منطقی formatRegionEnter(ثابت Region& region)
    {
        منطقی ok = یہ->لکھو("b,%d,%lld,%lld,%lld",
                (صحیح)region.pImpl->threadID,
                (طویل طویل صحیح)region.pImpl->beginTimestamp,
                (طویل طویل صحیح)((*region.pImpl->location.ppExtra)->global_location_id),
                (طویل طویل صحیح)region.pImpl->global_region_id);
        اگر (region.pImpl->والد_Region && region.pImpl->والد_Region->pImpl)
        {
            اگر (region.pImpl->والد_Region->pImpl->threadID != region.pImpl->threadID)
                ok &= یہ->لکھو(",parentThread=%d,parent=%lld",
                        (صحیح)region.pImpl->والد_Region->pImpl->threadID,
                        (طویل طویل صحیح)region.pImpl->والد_Region->pImpl->global_region_id);
        }
        ok &= یہ->لکھو("\n");
        واپس ok;
    }
    منطقی formatRegionLeave(ثابت Region& region, ثابت RegionStatistics& نتیجہ)
    {
        CV_DbgAssert(region.pImpl->انتہا_Timestamp - region.pImpl->beginTimestamp == نتیجہ.duration);
        منطقی ok = یہ->لکھو("e,%d,%lld,%lld,%lld,%lld",
                (صحیح)region.pImpl->threadID,
                (طویل طویل صحیح)region.pImpl->انتہا_Timestamp,
                (طویل طویل صحیح)(*region.pImpl->location.ppExtra)->global_location_id,
                (طویل طویل صحیح)region.pImpl->global_region_id,
                (طویل طویل صحیح)نتیجہ.duration);
        اگر (نتیجہ.currentSkippedRegions)
            ok &= یہ->لکھو(",skip=%d", (صحیح)نتیجہ.currentSkippedRegions);
#اگر_تعریف_شدہ HAVE_IPP
        اگر (نتیجہ.durationImplIPP)
            ok &= یہ->لکھو(",tIPP=%lld", (طویل طویل صحیح)نتیجہ.durationImplIPP);
#اگر_ختم
#اگر_تعریف_شدہ HAVE_OPENCL
        اگر (نتیجہ.durationImplOpenCL)
            ok &= یہ->لکھو(",tOCL=%lld", (طویل طویل صحیح)نتیجہ.durationImplOpenCL);
#اگر_ختم
        ok &= یہ->لکھو("\n");
        واپس ok;
    }
    منطقی ریکارڈ_Region_Arg(ثابت Region& region, ثابت TraceArg& arg, ثابت حرف* value)
    {
        واپس یہ->لکھو("a,%d,%lld,%lld,\"%s\",\"%s\"\n",
                region.pImpl->threadID,
                (طویل طویل صحیح)region.pImpl->beginTimestamp,
                (طویل طویل صحیح)region.pImpl->global_region_id,
                arg.name,
                value);
    }
};


#اگر_تعریف_شدہ OPENCV_WITH_ITT
ساکن __itt_domain* domain = تہی;

ساکن منطقی isITTEnabled()
{
    ساکن متلون منطقی isInitialized = جھوٹ;
    ساکن منطقی isEnabled = جھوٹ;
    اگر (!isInitialized)
    {
        cv::Auto_تالا lock(cv::getInitializationMutex());
        اگر (!isInitialized)
        {
            منطقی param_traceITTEnable = utils::getConfigurationParameterBool("OPENCV_TRACE_ITT_ENABLE", سچ);
            اگر (param_traceITTEnable)
            {
                isEnabled = !!(__itt_api_version());
                CV_LOG_ITT("ITT is " << (isEnabled ? "enabled" : "disabled"));
                domain = __itt_domain_create("OpenCVTrace");
            }
            ورنہ
            {
                CV_LOG_ITT("ITT is disabled through OpenCV parameter");
                isEnabled = جھوٹ;
            }
            isInitialized = سچ;
        }
    }
    واپس isEnabled;
}
#اگر_ختم


Region::Location_Extra_ڈیٹا::Location_Extra_ڈیٹا(ثابت LocationStaticStorage& location)
{
    CV_غیر_مستعمل(location);
    ساکن صحیح g_location_id_شمار = 0;
    global_location_id = CV_XADD(&g_location_id_شمار, 1) + 1;
    CV_LOG("Register location: " << global_location_id << " (" << (خالی*)&location << ")"
            << std::سطر_ختم << "    file: " << location.filename
            << std::سطر_ختم << "    line: " << location.سطر
            << std::سطر_ختم << "    name: " << location.name);
#اگر_تعریف_شدہ OPENCV_WITH_ITT
    اگر (isITTEnabled())
    {
        // Caching is not required here, because there is builtin cache.
        // https://software.intel.com/en-us/node/544203:
        //     Consecutive calls to __itt_string_handle_create with the same name return the same value.
        ittHandle_name = __itt_string_handle_create(location.name);
        ittHandle_filename = __itt_string_handle_create(location.filename);
    }
    ورنہ
    {
        ittHandle_name = 0;
        ittHandle_filename = 0;
    }
#اگر_ختم
}

/*static*/ Region::Location_Extra_ڈیٹا* Region::Location_Extra_ڈیٹا::init(ثابت Region::LocationStaticStorage& location)
{
    Location_Extra_ڈیٹا** pLocationExtra = location.ppExtra;
    CV_DbgAssert(pLocationExtra);
    اگر (*pLocationExtra == تہی)
    {
        cv::Auto_تالا lock(cv::getInitializationMutex());
        اگر (*pLocationExtra == تہی)
        {
            *pLocationExtra = نیا Region::Location_Extra_ڈیٹا(location);
            TraceStorage* s = getTraceManager().trace_storage.get();
            اگر (s)
            {
                Trace_پیغام پیغام;
                پیغام.formatlocation(location);
                s->put(پیغام);
            }
        }
    }
    واپس *pLocationExtra;
}


Region::Impl::Impl(TraceManagerThreadLocal& ctx, Region* والد_Region_2, Region& region_, ثابت LocationStaticStorage& location_, int64 beginTimestamp_) :
    location(location_),
    region(region_),
    والد_Region(والد_Region_2),
    threadID(ctx.threadID),
    global_region_id(++ctx.region_شمار),
    beginTimestamp(beginTimestamp_),
    انتہا_Timestamp(0),
    براہ_راست_Children_گنتی(0)
#اگر_تعریف_شدہ OPENCV_WITH_ITT
    ,itt_id_registered(جھوٹ)
    ,itt_id(__itt_null)
#اگر_ختم
{
    CV_DbgAssert(ctx.current_فعال_Region == والد_Region);
    region.pImpl = یہ;

    رجسٹر_Region(ctx);

    enterRegion(ctx);
}

Region::Impl::~Impl()
{
#اگر_تعریف_شدہ OPENCV_WITH_ITT
    اگر (itt_id_registered)
    {
        CV_LOG_ITT(" Destroy ITT region: I=" << (خالی*)یہ);
        __itt_id_destroy(domain, itt_id);
        itt_id_registered = جھوٹ;
    }
#اگر_ختم
    region.pImpl = تہی;
}

خالی Region::Impl::enterRegion(TraceManagerThreadLocal& ctx)
{
    ctx.current_فعال_Region = &region;

    اگر (location.flags & REGION_FLAG_FUNCTION)
    {
        اگر ((location.flags & REGION_FLAG_APP_رمز) == 0)
        {
            ctx.region_گہرائی_Open_CV++;
        }
        ctx.region_گہرائی++;
    }

    TraceStorage* s = ctx.getStorage();
    اگر (s)
    {
        Trace_پیغام پیغام;
        پیغام.formatRegionEnter(region);
        s->put(پیغام);
    }
#اگر_تعریف_شدہ OPENCV_WITH_ITT
    اگر (isITTEnabled())
    {
        __itt_id والد_ID = __itt_null;
        اگر (param_ITT_رجسٹر_والد_Scope && والد_Region && والد_Region->pImpl && والد_Region->pImpl->itt_id_registered && (location.flags & REGION_FLAG_REGION_FORCE) == 0)
            والد_ID = والد_Region->pImpl->itt_id;
        __itt_task_begin(domain, itt_id, والد_ID, (*location.ppExtra)->ittHandle_name);
    }
#اگر_ختم
}

خالی Region::Impl::leaveRegion(TraceManagerThreadLocal& ctx)
{
    int64 duration = انتہا_Timestamp - beginTimestamp; CV_غیر_مستعمل(duration);
    RegionStatistics نتیجہ;
    ctx.stat.grab(نتیجہ);
    ctx.کل_Skipped_Events += نتیجہ.currentSkippedRegions;
    CV_LOG(_spaces(ctx.get_Current_گہرائی()*4) << "leaveRegion(): " << (خالی*)یہ << " " << نتیجہ);
#اگر_تعریف_شدہ OPENCV_WITH_ITT
    اگر (isITTEnabled())
    {
        اگر (نتیجہ.currentSkippedRegions)
        {
            __itt_metadata_add(domain, itt_id, __itt_string_handle_create("skipped trace entries"), __itt_metadata_u32, 1, &نتیجہ.currentSkippedRegions);
        }
#اگر_تعریف_شدہ HAVE_IPP
        اگر (نتیجہ.durationImplIPP)
            __itt_metadata_add(domain, itt_id, __itt_string_handle_create("tIPP"), __itt_metadata_u64, 1, &نتیجہ.durationImplIPP);
#اگر_ختم
#اگر_تعریف_شدہ HAVE_OPENCL
        اگر (نتیجہ.durationImplOpenCL)
            __itt_metadata_add(domain, itt_id, __itt_string_handle_create("tOpenCL"), __itt_metadata_u64, 1, &نتیجہ.durationImplOpenCL);
#اگر_ختم
        __itt_task_end(domain);
    }
#اگر_ختم
    TraceStorage* s = ctx.getStorage();
    اگر (s)
    {
        Trace_پیغام پیغام;
        پیغام.formatRegionLeave(region, نتیجہ);
        s->put(پیغام);
    }

    اگر (location.flags & REGION_FLAG_FUNCTION)
    {
        اگر ((location.flags & REGION_FLAG_APP_رمز) == 0)
        {
            ctx.region_گہرائی_Open_CV--;
        }
        ctx.region_گہرائی--;
    }

    ctx.current_فعال_Region = والد_Region;
}

خالی Region::Impl::release()
{
    حذف یہ;
}

خالی Region::Impl::رجسٹر_Region(TraceManagerThreadLocal& ctx)
{
#اگر_تعریف_شدہ OPENCV_WITH_ITT
    اگر (isITTEnabled())
    {
        اگر (!itt_id_registered)
        {
            CV_LOG_ITT(" Register ITT region: I=" << (خالی*)یہ << " " << ctx.threadID << "-" << global_region_id);
#اگر 1 // workaround for some ITT backends
            itt_id = __itt_id_make((خالی*)(intptr_t)(((int64)(ctx.threadID + 1) << 32) | global_region_id), global_region_id);
#ورنہ
            itt_id = __itt_id_make((خالی*)(intptr_t)(ctx.threadID + 1), global_region_id);
#اگر_ختم
            __itt_id_create(domain, itt_id);
            itt_id_registered = سچ;
        }
    }
#ورنہ
    CV_غیر_مستعمل(ctx);
#اگر_ختم
}

خالی Region_Statistics_حالت::enable_Skip_منوال(صحیح گہرائی)
{
    CV_DbgAssert(skip_گہرائی < 0);
    CV_LOG_SKIP(تہی, "SKIP-ENABLE: depth=" << گہرائی);
    skip_گہرائی = گہرائی;
}
خالی Region_Statistics_حالت::check_Reset_Skip_منوال(صحیح leave_گہرائی)
{
    اگر (leave_گہرائی <= skip_گہرائی)
    {
        CV_LOG_SKIP(تہی, "SKIP-RESET: leaveDepth=" << leave_گہرائی << " skipDepth=" << skip_گہرائی);
        skip_گہرائی = -1;
    }
}

Region::Region(ثابت LocationStaticStorage& location) :
    pImpl(تہی),
    implFlags(0)
{
    // Checks:
    // - global enable flag
    // - parent region is disabled
    // - children count threshold
    // - region location
    // - depth (opencv nested calls)
    اگر (!TraceManager::isActivated())
    {
        CV_LOG("Trace is disabled. Bailout");
        واپس;
    }

    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    CV_LOG(_spaces(ctx.get_Current_گہرائی()*4) << "Region(): " << (خالی*)یہ << ": " << location.name);

    Region* والد_Region = ctx.ڈھیر_چوٹی_Region();
    ثابت Region::LocationStaticStorage* والد_Location = ctx.ڈھیر_چوٹی_Location();

    اگر (location.flags & REGION_FLAG_REGION_اگلا)
    {
        اگر (والد_Region && والد_Region->pImpl)
        {
            CV_DbgAssert((والد_Region->pImpl->location.flags & REGION_FLAG_FUNCTION) == 0);
            والد_Region->destroy(); والد_Region->implFlags = 0;
            والد_Region = ctx.ڈھیر_چوٹی_Region();
            والد_Location = ctx.ڈھیر_چوٹی_Location();
        }
    }

    صحیح والد_Children = 0;
    اگر (والد_Region && والد_Region->pImpl)
    {
        اگر (والد_Location == تہی)
        {
            // parallel_for_body code path
            والد_Children = CV_XADD(&والد_Region->pImpl->براہ_راست_Children_گنتی, 1) + 1;
        }
        ورنہ
        {
            والد_Children = ++والد_Region->pImpl->براہ_راست_Children_گنتی;
        }
    }

    int64 beginTimestamp = getTimestampNS();

    صحیح current_گہرائی = ctx.get_Current_گہرائی() + 1;
    انتخاب (location.flags & REGION_FLAG_IMPL_نقاب)
    {
#اگر_تعریف_شدہ HAVE_IPP
    حالت REGION_FLAG_IMPL_IPP:
        اگر (!ctx.stat_حالت.ignore_گہرائی_Impl_IPP)
            ctx.stat_حالت.ignore_گہرائی_Impl_IPP = current_گہرائی;
        توڑو;
#اگر_ختم
#اگر_تعریف_شدہ HAVE_OPENCL
    حالت REGION_FLAG_IMPL_OPENCL:
        اگر (!ctx.stat_حالت.ignore_گہرائی_Impl_Open_CL)
            ctx.stat_حالت.ignore_گہرائی_Impl_Open_CL = current_گہرائی;
        توڑو;
#اگر_ختم
    طےشدہ:
        توڑو;
    }

    ctx.ڈھیر_ڈالو(یہ, &location, beginTimestamp);
    implFlags |= REGION_FLAG_NEED_ڈھیر_نکالو;

    اگر ((location.flags & REGION_FLAG_REGION_FORCE) == 0)
    {
        اگر (ctx.stat_حالت.skip_گہرائی >= 0 && current_گہرائی > ctx.stat_حالت.skip_گہرائی)
        {
            CV_LOG(_spaces(ctx.get_Current_گہرائی()*4) << "Parent region is disabled. Bailout");
            ctx.stat.currentSkippedRegions++;
            واپس;
        }

        اگر (param_maxRegionChildrenOpenCV > 0 && (location.flags & REGION_FLAG_APP_رمز) == 0 && والد_Location && (والد_Location->flags & REGION_FLAG_APP_رمز) == 0)
        {
            اگر (والد_Children >= param_maxRegionChildrenOpenCV)
            {
                CV_LOG_TRACE_BAILOUT(تہی, _spaces(ctx.get_Current_گہرائی()*4) << "OpenCV parent region exceeds children count. Bailout");
                ctx.stat_حالت.enable_Skip_منوال(current_گہرائی - 1);
                ctx.stat.currentSkippedRegions++;
                DEBUG_ONLY(ctx.dump_ڈھیر(std::خارج, جھوٹ));
                واپس;
            }
        }
        اگر (param_maxRegionChildren > 0 && والد_Children >= param_maxRegionChildren)
        {
            CV_LOG_TRACE_BAILOUT(تہی, _spaces(ctx.get_Current_گہرائی()*4) << "Parent region exceeds children count. Bailout");
            ctx.stat_حالت.enable_Skip_منوال(current_گہرائی - 1);
            ctx.stat.currentSkippedRegions++;
            DEBUG_ONLY(ctx.dump_ڈھیر(std::خارج, جھوٹ));
            واپس;
        }
    }

    Location_Extra_ڈیٹا::init(location);

    اگر ((*location.ppExtra)->global_location_id == 0)
    {
        CV_LOG_TRACE_BAILOUT(تہی, _spaces(ctx.get_Current_گہرائی()*4) << "Region location is disabled. Bailout");
        ctx.stat_حالت.enable_Skip_منوال(current_گہرائی);
        ctx.stat.currentSkippedRegions++;
        واپس;
    }

    اگر (والد_Location && (والد_Location->flags & REGION_FLAG_SKIP_NESTED))
    {
        CV_LOG(_spaces(ctx.get_Current_گہرائی()*4) << "Parent region disables inner regions. Bailout");
        ctx.stat_حالت.enable_Skip_منوال(current_گہرائی);
        ctx.stat.currentSkippedRegions++;
        واپس;
    }

    اگر (param_max_Region_گہرائی_Open_CV)
    {
        اگر ((location.flags & REGION_FLAG_APP_رمز) == 0)
        {
            اگر (ctx.region_گہرائی_Open_CV >= param_max_Region_گہرائی_Open_CV)
            {
                CV_LOG(_spaces(ctx.get_Current_گہرائی()*4) << "OpenCV region depth is exceed = " << ctx.region_گہرائی_Open_CV << ". Bailout");
                اگر (ctx.stat.currentSkippedRegions == 0)
                {
                    DEBUG_ONLY(ctx.dump_ڈھیر(std::خارج, جھوٹ));
                }
                ctx.stat_حالت.enable_Skip_منوال(current_گہرائی);
                ctx.stat.currentSkippedRegions++;
                واپس;
            }
        }
    }

    نیا Impl(ctx, والد_Region, *یہ, location, beginTimestamp);
    CV_DbgAssert(pImpl != تہی);
    implFlags |= REGION_FLAG_فعال;

    // parallel_for path
    اگر (والد_Region && والد_Region->pImpl)
    {
        اگر (والد_Location == تہی)
        {
            pImpl->براہ_راست_Children_گنتی = والد_Children;
        }
    }
}

خالی Region::destroy()
{
    CV_DbgAssert(implFlags != 0);

    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    CV_LOG(_spaces(ctx.get_Current_گہرائی()*4) << "Region::destruct(): " << (خالی*)یہ << " pImpl=" << pImpl << " implFlags=" << implFlags << ' ' << (ctx.ڈھیر_چوٹی_Location() ? ctx.ڈھیر_چوٹی_Location()->name : "<unknown>"));

    CV_DbgAssert(implFlags & REGION_FLAG_NEED_ڈھیر_نکالو);
    ثابت صحیح current_گہرائی = ctx.get_Current_گہرائی(); CV_غیر_مستعمل(current_گہرائی);

    CV_LOG_CTX_STAT(تہی, _spaces(current_گہرائی*4) << ctx.stat << ' ' << ctx.stat_حالت);

    ثابت Region::LocationStaticStorage* location = ctx.ڈھیر_چوٹی_Location();
    Impl::Optimization_راستہ my_رمز_راستہ = Impl::رمز_راستہ_PLAIN;
    اگر (location)
    {
        انتخاب (location->flags & REGION_FLAG_IMPL_نقاب)
        {
#اگر_تعریف_شدہ HAVE_IPP
        حالت REGION_FLAG_IMPL_IPP:
            my_رمز_راستہ = Impl::رمز_راستہ_IPP;
            توڑو;
#اگر_ختم
#اگر_تعریف_شدہ HAVE_OPENCL
        حالت REGION_FLAG_IMPL_OPENCL:
            اگر (param_synchronizeOpenCL && cv::ocl::isOpenCLActivated())
                cv::ocl::اختتام_2();
            my_رمز_راستہ = Impl::رمز_راستہ_OPENCL;
            توڑو;
#اگر_ختم
        طےشدہ:
            توڑو;
        }
    }

    int64 انتہا_Timestamp = getTimestampNS();
    int64 duration = انتہا_Timestamp - ctx.ڈھیر_چوٹی_Begin_Timestamp();

    منطقی فعال = is_فعال_2();

    اگر (فعال)
        ctx.stat.duration = duration;
    ورنہ اگر (ctx.ڈھیر.size() == ctx.parallel_for_ڈھیر_سائز + 1)
        ctx.stat.duration += duration;

    انتخاب (my_رمز_راستہ) {
        حالت Impl::رمز_راستہ_PLAIN:
            // nothing
            توڑو;
#اگر_تعریف_شدہ HAVE_IPP
        حالت Impl::رمز_راستہ_IPP:
            اگر (ctx.stat_حالت.ignore_گہرائی_Impl_IPP == current_گہرائی)
            {
                ctx.stat.durationImplIPP += duration;
                ctx.stat_حالت.ignore_گہرائی_Impl_IPP = 0;
            }
            ورنہ اگر (فعال)
            {
                ctx.stat.durationImplIPP = duration;
            }
            توڑو;
#اگر_ختم
#اگر_تعریف_شدہ HAVE_OPENCL
        حالت Impl::رمز_راستہ_OPENCL:
            اگر (ctx.stat_حالت.ignore_گہرائی_Impl_Open_CL == current_گہرائی)
            {
                ctx.stat.durationImplOpenCL += duration;
                ctx.stat_حالت.ignore_گہرائی_Impl_Open_CL = 0;
            }
            ورنہ اگر (فعال)
            {
                ctx.stat.durationImplOpenCL = duration;
            }
            توڑو;
#اگر_ختم
        طےشدہ:
            توڑو;
    }

    اگر (pImpl)
    {
        CV_DbgAssert((implFlags & (REGION_FLAG_فعال | REGION_FLAG_NEED_ڈھیر_نکالو)) == (REGION_FLAG_فعال | REGION_FLAG_NEED_ڈھیر_نکالو));
        CV_DbgAssert(ctx.ڈھیر_چوٹی_Region() == یہ);
        pImpl->انتہا_Timestamp = انتہا_Timestamp;
        pImpl->leaveRegion(ctx);
        pImpl->release();
        pImpl = تہی;
        DEBUG_ONLY(implFlags &= ~REGION_FLAG_فعال);
    }
    ورنہ
    {
        CV_DbgAssert(ctx.stat_حالت.skip_گہرائی <= current_گہرائی);
    }

    اگر (implFlags & REGION_FLAG_NEED_ڈھیر_نکالو)
    {
        CV_DbgAssert(ctx.ڈھیر_چوٹی_Region() == یہ);
        ctx.ڈھیر_نکالو();
        ctx.stat_حالت.check_Reset_Skip_منوال(current_گہرائی);
        DEBUG_ONLY(implFlags &= ~REGION_FLAG_NEED_ڈھیر_نکالو);
    }
    CV_LOG_CTX_STAT(تہی, _spaces(current_گہرائی*4) << "===> " << ctx.stat << ' ' << ctx.stat_حالت);
}


TraceManagerThreadLocal::~TraceManagerThreadLocal()
{
}

خالی TraceManagerThreadLocal::dump_ڈھیر(std::ostream& out, منطقی onlyFunctions) ثابت
{
    std::stringstream ss;
    std::deque<ڈھیر_اندراج>::const_iterator it = ڈھیر.begin();
    std::deque<ڈھیر_اندراج>::const_iterator end = ڈھیر.end();
    صحیح گہرائی = 0;
    برائے (; it != end; ++it)
    {
        ثابت Region::LocationStaticStorage* location = it->location;
        اگر (location)
        {
            اگر (!onlyFunctions || (location->flags & REGION_FLAG_FUNCTION))
            {
                ss << _spaces(4*گہرائی) << location->name << std::سطر_ختم;
                گہرائی++;
            }
        }
        ورنہ
        {
            ss << _spaces(4*گہرائی) << "<unknown>" << std::سطر_ختم;
            گہرائی++;
        }
    }
    out << ss.str();
}

جماعت AsyncTraceStorage CV_FINAL : عوامی TraceStorage
{
    تغیرپذیر std::ofstream out;
عوامی:
    ثابت std::string name;

    AsyncTraceStorage(ثابت std::string& filename) :
        out(filename.c_str(), std::ios::trunc),
        name(filename)
    {
        out << "#description: OpenCV trace file" << std::سطر_ختم;
        out << "#version: 1.0" << std::سطر_ختم;
    }
    ~AsyncTraceStorage()
    {
        out.close();
    }

    منطقی put(ثابت Trace_پیغام& پیغام) ثابت CV_OVERRIDE
    {
        اگر (پیغام.hasError)
            واپس جھوٹ;
        out << پیغام.بفر;
        //DEBUG_ONLY(std::flush(out)); // TODO configure flag
        واپس سچ;
    }
};

جماعت SyncTraceStorage CV_FINAL : عوامی TraceStorage
{
    تغیرپذیر std::ofstream out;
    تغیرپذیر cv::Mutex mutex;
عوامی:
    ثابت std::string name;

    SyncTraceStorage(ثابت std::string& filename) :
        out(filename.c_str(), std::ios::trunc),
        name(filename)
    {
        out << "#description: OpenCV trace file" << std::سطر_ختم;
        out << "#version: 1.0" << std::سطر_ختم;
    }
    ~SyncTraceStorage()
    {
        cv::Auto_تالا l(mutex);
        out.close();
    }

    منطقی put(ثابت Trace_پیغام& پیغام) ثابت CV_OVERRIDE
    {
        اگر (پیغام.hasError)
            واپس جھوٹ;
        {
            cv::Auto_تالا l(mutex);
            out << پیغام.بفر;
            std::flush(out); // TODO configure flag
        }
        واپس سچ;
    }
};


TraceStorage* TraceManagerThreadLocal::getStorage() ثابت
{
    // TODO configuration option for stdout/single trace file
    اگر (storage.empty())
    {
        TraceStorage* global = getTraceManager().trace_storage.get();
        اگر (global)
        {
            ثابت std::string filepath = cv::format("%s-%03d.txt", getParameterTraceLocation().c_str(), threadID).c_str();
            Trace_پیغام پیغام;
            ثابت حرف* pos = strrchr(filepath.c_str(), '/'); // extract filename
#اگر_تعریف_شدہ _WIN32
            اگر (!pos)
                pos = strrchr(filepath.c_str(), '\\');
#اگر_ختم
            اگر (!pos)
                pos = filepath.c_str();
            ورنہ
                pos += 1; // fix to skip extra slash in filename beginning
            پیغام.لکھو("#thread file: %s\n", pos);
            global->put(پیغام);
            storage.reset(نیا AsyncTraceStorage(filepath));
        }
    }
    واپس storage.get();
}



ساکن منطقی activated = جھوٹ;
ساکن منطقی isInitialized = جھوٹ;

TraceManager::TraceManager()
{
    (خالی)cv::getTimestampNS();

    isInitialized = سچ;
    CV_LOG("TraceManager ctor: " << (خالی*)یہ);

    CV_LOG("TraceManager configure()");
    activated = getParameterTraceEnable();

    اگر (activated)
        trace_storage.reset(نیا SyncTraceStorage(std::string(getParameterTraceLocation()) + ".txt"));

#اگر_تعریف_شدہ OPENCV_WITH_ITT
    اگر (isITTEnabled())
    {
        activated = سچ; // force trace pipeline activation (without OpenCV storage)
        __itt_region_begin(domain, __itt_null, __itt_null, __itt_string_handle_create("OpenCVTrace"));
    }
#اگر_ختم
}
TraceManager::~TraceManager()
{
    CV_LOG("TraceManager dtor: " << (خالی*)یہ);

#اگر_تعریف_شدہ OPENCV_WITH_ITT
    اگر (isITTEnabled())
    {
        __itt_region_end(domain, __itt_null);
    }
#اگر_ختم

    std::vector<TraceManagerThreadLocal*> دھاگے_ctx;
    tls.gather(دھاگے_ctx);
    size_t کل_Events = 0, کل_Skipped_Events = 0;
    برائے (size_t i = 0; i < دھاگے_ctx.size(); i++)
    {
        TraceManagerThreadLocal* ctx = دھاگے_ctx[i];
        اگر (ctx)
        {
            کل_Events += ctx->region_شمار;
            کل_Skipped_Events += ctx->کل_Skipped_Events;
        }
    }
    اگر (کل_Events || activated)
    {
        CV_LOG_INFO(تہی, "Trace: Total events: " << کل_Events);
    }
    اگر (کل_Skipped_Events)
    {
        CV_LOG_WARNING(تہی, "Trace: Total skipped events: " << کل_Skipped_Events);
    }

    // This is a global static object, so process starts shutdown here
    // Turn off trace
    cv::__termination = سچ; // also set in DllMain() notifications handler for DLL_PROCESS_DETACH
    activated = جھوٹ;
}

منطقی TraceManager::isActivated()
{
    // Check if process starts shutdown, and set earlyExit to true
    // to prevent further instrumentation processing earlier.
    اگر (cv::__termination)
    {
        activated = جھوٹ;
        واپس جھوٹ;
    }

    اگر (!isInitialized)
    {
        TraceManager& m = getTraceManager();
        CV_غیر_مستعمل(m); // TODO
    }

    واپس activated;
}


ساکن TraceManager* getTraceManagerCallOnce()
{
    ساکن TraceManager globalInstance;
    واپس &globalInstance;
}
TraceManager& getTraceManager()
{
    CV_SINGLETON_LAZY_INIT_REF(TraceManager, getTraceManagerCallOnce())
}

خالی parallel_For_سیٹ_جڑ_Region(ثابت Region& جڑ_Region, ثابت TraceManagerThreadLocal& جڑ_ctx)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();

    اگر (ctx.dummy_ڈھیر_چوٹی.region == &جڑ_Region) // already attached
        واپس;

    CV_Assert(ctx.dummy_ڈھیر_چوٹی.region == تہی);
    ctx.dummy_ڈھیر_چوٹی = TraceManagerThreadLocal::ڈھیر_اندراج(const_cast<Region*>(&جڑ_Region), تہی, -1);

    اگر (&ctx == &جڑ_ctx)
    {
        ctx.stat.grab(ctx.parallel_for_stat);
        ctx.parallel_for_stat_حالت = ctx.stat_حالت;
        ctx.parallel_for_ڈھیر_سائز = ctx.ڈھیر.size();
        واپس;
    }

    CV_Assert(ctx.ڈھیر.empty());

    ctx.current_فعال_Region = const_cast<Region*>(&جڑ_Region);

    ctx.region_گہرائی = جڑ_ctx.region_گہرائی;
    ctx.region_گہرائی_Open_CV = جڑ_ctx.region_گہرائی_Open_CV;

    ctx.parallel_for_ڈھیر_سائز = 0;

    ctx.stat_حالت.propagateFrom(جڑ_ctx.stat_حالت);
}

خالی parallelForAttachNestedRegion(ثابت Region& جڑ_Region)
{
    CV_غیر_مستعمل(جڑ_Region);
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();

    CV_DbgAssert(ctx.dummy_ڈھیر_چوٹی.region == &جڑ_Region);

    Region* region = ctx.get_Current_فعال_Region();
    CV_LOG_PARALLEL(تہی, " PARALLEL_FOR: " << (خالی*)region << " ==> " << &جڑ_Region);
    اگر (!region)
        واپس;

#اگر_تعریف_شدہ OPENCV_WITH_ITT
    اگر (!جڑ_Region.pImpl || !جڑ_Region.pImpl->itt_id_registered)
        واپس;

    اگر (!region->pImpl)
        واپس;

    CV_LOG_PARALLEL(تہی, " PARALLEL_FOR ITT: " << (خالی*)جڑ_Region.pImpl->itt_id.d1 << ":" << جڑ_Region.pImpl->itt_id.d2 << ":" << (خالی*)جڑ_Region.pImpl->itt_id.d3 << " => "
                                 << (خالی*)region->pImpl->itt_id.d1 << ":" << region->pImpl->itt_id.d2 << ":" << (خالی*)region->pImpl->itt_id.d3);
    __itt_relation_add(domain, region->pImpl->itt_id, __itt_relation_is_child_of, جڑ_Region.pImpl->itt_id);
#اگر_ختم
}

خالی parallelForFinalize(ثابت Region& جڑ_Region)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();

    int64 انتہا_Timestamp = getTimestampNS();
    int64 duration = انتہا_Timestamp - ctx.ڈھیر_چوٹی_Begin_Timestamp();
    CV_LOG_PARALLEL(تہی, "parallel_for duration: " << duration << " " << &جڑ_Region);

    std::vector<TraceManagerThreadLocal*> دھاگے_ctx;
    getTraceManager().tls.gather(دھاگے_ctx);
    RegionStatistics parallel_for_stat;
    برائے (size_t i = 0; i < دھاگے_ctx.size(); i++)
    {
        TraceManagerThreadLocal* بچہ_ctx = دھاگے_ctx[i];

        اگر (بچہ_ctx && بچہ_ctx->ڈھیر_چوٹی_Region() == &جڑ_Region)
        {
            CV_LOG_PARALLEL(تہی, "Thread=" << بچہ_ctx->threadID << " " << بچہ_ctx->stat);
            RegionStatistics بچہ_stat;
            بچہ_ctx->stat.grab(بچہ_stat);
            parallel_for_stat.append(بچہ_stat);
            اگر (بچہ_ctx != &ctx)
            {
                بچہ_ctx->dummy_ڈھیر_چوٹی = TraceManagerThreadLocal::ڈھیر_اندراج();
            }
            ورنہ
            {
                ctx.parallel_for_stat.grab(ctx.stat);
                ctx.stat_حالت = ctx.parallel_for_stat_حالت;
                بچہ_ctx->dummy_ڈھیر_چوٹی = TraceManagerThreadLocal::ڈھیر_اندراج();
            }
        }
    }

    اعشاری parallel_coeff = std::min(1.0f, duration / (اعشاری)(parallel_for_stat.duration));
    CV_LOG_PARALLEL(تہی, "parallel_coeff=" << 1.0f / parallel_coeff);
    CV_LOG_PARALLEL(تہی, parallel_for_stat);
    اگر (parallel_coeff != 1.0f)
    {
        parallel_for_stat.multiply(parallel_coeff);
        CV_LOG_PARALLEL(تہی, parallel_for_stat);
    }
    parallel_for_stat.duration = 0;
    ctx.stat.append(parallel_for_stat);
    CV_LOG_PARALLEL(تہی, ctx.stat);
}

ساخت TraceArg::Extra_ڈیٹا
{
#اگر_تعریف_شدہ OPENCV_WITH_ITT
    // Special fields for ITT
    __itt_string_handle* متلون ittHandle_name;
#اگر_ختم
    Extra_ڈیٹا(TraceManagerThreadLocal& ctx, ثابت TraceArg& arg)
    {
        CV_غیر_مستعمل(ctx); CV_غیر_مستعمل(arg);
#اگر_تعریف_شدہ OPENCV_WITH_ITT
        اگر (isITTEnabled())
        {
            // Caching is not required here, because there is builtin cache.
            // https://software.intel.com/en-us/node/544203:
            //     Consecutive calls to __itt_string_handle_create with the same name return the same value.
            ittHandle_name = __itt_string_handle_create(arg.name);
        }
        ورنہ
        {
            ittHandle_name = 0;
        }
#اگر_ختم
    }
};

ساکن خالی initTraceArg(TraceManagerThreadLocal& ctx, ثابت TraceArg& arg)
{
    TraceArg::Extra_ڈیٹا** pExtra = arg.ppExtra;
    اگر (*pExtra == تہی)
    {
        cv::Auto_تالا lock(cv::getInitializationMutex());
        اگر (*pExtra == تہی)
        {
            *pExtra = نیا TraceArg::Extra_ڈیٹا(ctx, arg);
        }
    }
}
خالی traceArg(ثابت TraceArg& arg, ثابت حرف* value)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    Region* region = ctx.get_Current_فعال_Region();
    اگر (!region)
        واپس;
    CV_Assert(region->pImpl);
    initTraceArg(ctx, arg);
    اگر (!value)
        value = "<null>";
#اگر_تعریف_شدہ OPENCV_WITH_ITT
    اگر (isITTEnabled())
    {
        __itt_metadata_str_add(domain, region->pImpl->itt_id, (*arg.ppExtra)->ittHandle_name, value, لڑی_لمبائی(value));
    }
#اگر_ختم
}
خالی traceArg(ثابت TraceArg& arg, صحیح value)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    Region* region = ctx.get_Current_فعال_Region();
    اگر (!region)
        واپس;
    CV_Assert(region->pImpl);
    initTraceArg(ctx, arg);
#اگر_تعریف_شدہ OPENCV_WITH_ITT
    اگر (isITTEnabled())
    {
        __itt_metadata_add(domain, region->pImpl->itt_id, (*arg.ppExtra)->ittHandle_name, حجم(صحیح) == 4 ? __itt_metadata_s32 : __itt_metadata_s64, 1, &value);
    }
#ورنہ
    CV_غیر_مستعمل(value);
#اگر_ختم
}
خالی traceArg(ثابت TraceArg& arg, int64 value)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    Region* region = ctx.get_Current_فعال_Region();
    اگر (!region)
        واپس;
    CV_Assert(region->pImpl);
    initTraceArg(ctx, arg);
#اگر_تعریف_شدہ OPENCV_WITH_ITT
    اگر (isITTEnabled())
    {
        __itt_metadata_add(domain, region->pImpl->itt_id, (*arg.ppExtra)->ittHandle_name, __itt_metadata_s64, 1, &value);
    }
#ورنہ
    CV_غیر_مستعمل(value);
#اگر_ختم
}
خالی traceArg(ثابت TraceArg& arg, دوہرا value)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    Region* region = ctx.get_Current_فعال_Region();
    اگر (!region)
        واپس;
    CV_Assert(region->pImpl);
    initTraceArg(ctx, arg);
#اگر_تعریف_شدہ OPENCV_WITH_ITT
    اگر (isITTEnabled())
    {
        __itt_metadata_add(domain, region->pImpl->itt_id, (*arg.ppExtra)->ittHandle_name, __itt_metadata_double, 1, &value);
    }
#ورنہ
    CV_غیر_مستعمل(value);
#اگر_ختم
}

#ورنہ

Region::Region(ثابت LocationStaticStorage&) : pImpl(تہی), implFlags(0) {}
خالی Region::destroy() {}

خالی traceArg(ثابت TraceArg&, ثابت حرف*) {}
خالی traceArg(ثابت TraceArg&, صحیح) {};
خالی traceArg(ثابت TraceArg&, int64) {};
خالی traceArg(ثابت TraceArg&, دوہرا) {};

#اگر_ختم

}}}} // namespace
