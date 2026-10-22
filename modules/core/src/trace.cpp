// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#include "precomp.hpp"

#include <opencv2/core/utils/trace.hpp>
#include <opencv2/core/utils/trace.private.hpp>
#include <opencv2/core/utils/configuration.private.hpp>

#include <opencv2/core/opencl/ocl_defs.hpp>

#include <cstdarg> // va_start

#include <sstream>
#include <ostream>
#include <fstream>

#ຖ້າ 0
#define CV_LOG(...) CV_LOG_INFO(NULL, __VA_ARGS__)
#ບໍ່ດັ່ງນັ້ນ
#define CV_LOG(...) {}
#endif

#ຖ້າ 0
#define CV_LOG_ITT(...) CV_LOG_INFO(NULL, __VA_ARGS__)
#ບໍ່ດັ່ງນັ້ນ
#define CV_LOG_ITT(...) {}
#endif

#ຖ້າ 1
#define CV_LOG_TRACE_BAILOUT(tag, ...) CV_LOG_INFO(tag, __VA_ARGS__)
#ບໍ່ດັ່ງນັ້ນ
#define CV_LOG_TRACE_BAILOUT(...) {}
#endif

#ຖ້າ 0
#define CV_LOG_PARALLEL(tag, ...) CV_LOG_INFO(tag, __VA_ARGS__)
#ບໍ່ດັ່ງນັ້ນ
#define CV_LOG_PARALLEL(...) {}
#endif

#ຖ້າ 0
#define CV_LOG_CTX_STAT(tag, ...) CV_LOG_INFO(tag, __VA_ARGS__)
#ບໍ່ດັ່ງນັ້ນ
#define CV_LOG_CTX_STAT(...) {}
#endif

#ຖ້າ 0
#define CV_LOG_SKIP(tag, ...) CV_LOG_INFO(tag, __VA_ARGS__)
#ບໍ່ດັ່ງນັ້ນ
#define CV_LOG_SKIP(...) {}
#endif

namespace cv {
namespace utils {
namespace trace {
namespace details {

#ifdef OPENCV_TRACE

#ifdef _MSC_VER
#pragma warning(disable:4065) // switch statement contains 'default' but no 'case' labels
#endif

static bool getParameterTraceEnable()
{
    static bool param_traceEnable = utils::getConfigurationParameterBool("OPENCV_TRACE", false);
    ສົ່ງຄືນ param_traceEnable;
}

// TODO lazy configuration flags
static ຈຳນວນເຕັມ param_maxRegionDepthOpenCV = (ຈຳນວນເຕັມ)utils::getConfigurationParameterSizeT("OPENCV_TRACE_DEPTH_OPENCV", 1);
static ຈຳນວນເຕັມ param_maxRegionChildrenOpenCV = (ຈຳນວນເຕັມ)utils::getConfigurationParameterSizeT("OPENCV_TRACE_MAX_CHILDREN_OPENCV", 1000);
static ຈຳນວນເຕັມ param_maxRegionChildren = (ຈຳນວນເຕັມ)utils::getConfigurationParameterSizeT("OPENCV_TRACE_MAX_CHILDREN", 10000);

static const cv::String& getParameterTraceLocation()
{
    static cv::String param_traceLocation = utils::getConfigurationParameterString("OPENCV_TRACE_LOCATION", "OpenCVTrace");
    ສົ່ງຄືນ param_traceLocation;
}

#ifdef HAVE_OPENCL
static bool param_synchronizeOpenCL = utils::getConfigurationParameterBool("OPENCV_TRACE_SYNC_OPENCL", false);
#endif

#ifdef OPENCV_WITH_ITT
static bool param_ITT_registerParentScope = utils::getConfigurationParameterBool("OPENCV_TRACE_ITT_PARENT", false);
#endif

static const var_na* _spaces(ຈຳນວນເຕັມ count)
{
    static const var_na buf[64] =
"                                                               ";
    ສົ່ງຄືນ &buf[63 - (count & 63)];
}

/**
 * Text-based trace messages
 */
class TraceMessage
{
public:
    var_na buffer[1024];
    size_t len;
    bool hasError;

    TraceMessage() :
        len(0),
        hasError(false)
    {}

    bool printf(const var_na* format, ...)
    {
        var_na* buf = &buffer[len];
        size_t sz = parimaa_na(buffer) - len;
        va_list ap;
        va_start(ap, format);
        ຈຳນວນເຕັມ n = cv_vsnprintf(buf, (ຈຳນວນເຕັມ)sz, format, ap);
        va_end(ap);
        ຖ້າ (n < 0 || (size_t)n > sz)
        {
            hasError = true;
            ສົ່ງຄືນ false;
        }
        len += n;
        ສົ່ງຄືນ true;
    }

    bool formatlocation(const Region::LocationStaticStorage& location)
    {
        ສົ່ງຄືນ this->printf("l,%lld,\"%s\",%d,\"%s\",0x%llX\n",
                (long long ຈຳນວນເຕັມ)(*location.ppExtra)->global_location_id,
                location.filename,
                location.line,
                location.name,
                (long long ຈຳນວນເຕັມ)(location.flags & ~0xF0000000));
    }
    bool formatRegionEnter(const Region& region)
    {
        bool ok = this->printf("b,%d,%lld,%lld,%lld",
                (ຈຳນວນເຕັມ)region.pImpl->threadID,
                (long long ຈຳນວນເຕັມ)region.pImpl->beginTimestamp,
                (long long ຈຳນວນເຕັມ)((*region.pImpl->location.ppExtra)->global_location_id),
                (long long ຈຳນວນເຕັມ)region.pImpl->global_region_id);
        ຖ້າ (region.pImpl->parentRegion && region.pImpl->parentRegion->pImpl)
        {
            ຖ້າ (region.pImpl->parentRegion->pImpl->threadID != region.pImpl->threadID)
                ok &= this->printf(",parentThread=%d,parent=%lld",
                        (ຈຳນວນເຕັມ)region.pImpl->parentRegion->pImpl->threadID,
                        (long long ຈຳນວນເຕັມ)region.pImpl->parentRegion->pImpl->global_region_id);
        }
        ok &= this->printf("\n");
        ສົ່ງຄືນ ok;
    }
    bool formatRegionLeave(const Region& region, const RegionStatistics& result)
    {
        CV_DbgAssert(region.pImpl->endTimestamp - region.pImpl->beginTimestamp == result.duration);
        bool ok = this->printf("e,%d,%lld,%lld,%lld,%lld",
                (ຈຳນວນເຕັມ)region.pImpl->threadID,
                (long long ຈຳນວນເຕັມ)region.pImpl->endTimestamp,
                (long long ຈຳນວນເຕັມ)(*region.pImpl->location.ppExtra)->global_location_id,
                (long long ຈຳນວນເຕັມ)region.pImpl->global_region_id,
                (long long ຈຳນວນເຕັມ)result.duration);
        ຖ້າ (result.currentSkippedRegions)
            ok &= this->printf(",skip=%d", (ຈຳນວນເຕັມ)result.currentSkippedRegions);
#ifdef HAVE_IPP
        ຖ້າ (result.durationImplIPP)
            ok &= this->printf(",tIPP=%lld", (long long ຈຳນວນເຕັມ)result.durationImplIPP);
#endif
#ifdef HAVE_OPENCL
        ຖ້າ (result.durationImplOpenCL)
            ok &= this->printf(",tOCL=%lld", (long long ຈຳນວນເຕັມ)result.durationImplOpenCL);
#endif
        ok &= this->printf("\n");
        ສົ່ງຄືນ ok;
    }
    bool recordRegionArg(const Region& region, const TraceArg& arg, const var_na* value)
    {
        ສົ່ງຄືນ this->printf("a,%d,%lld,%lld,\"%s\",\"%s\"\n",
                region.pImpl->threadID,
                (long long ຈຳນວນເຕັມ)region.pImpl->beginTimestamp,
                (long long ຈຳນວນເຕັມ)region.pImpl->global_region_id,
                arg.name,
                value);
    }
};


#ifdef OPENCV_WITH_ITT
static __itt_domain* domain = NULL;

static bool isITTEnabled()
{
    static ca_ncala bool isInitialized = false;
    static bool isEnabled = false;
    ຖ້າ (!isInitialized)
    {
        cv::AutoLock lock(cv::getInitializationMutex());
        ຖ້າ (!isInitialized)
        {
            bool param_traceITTEnable = utils::getConfigurationParameterBool("OPENCV_TRACE_ITT_ENABLE", true);
            ຖ້າ (param_traceITTEnable)
            {
                isEnabled = !!(__itt_api_version());
                CV_LOG_ITT("ITT is " << (isEnabled ? "enabled" : "disabled"));
                domain = __itt_domain_create("OpenCVTrace");
            }
            ບໍ່ດັ່ງນັ້ນ
            {
                CV_LOG_ITT("ITT is disabled through OpenCV parameter");
                isEnabled = false;
            }
            isInitialized = true;
        }
    }
    ສົ່ງຄືນ isEnabled;
}
#endif


Region::LocationExtraData::LocationExtraData(const LocationStaticStorage& location)
{
    CV_UNUSED(location);
    static ຈຳນວນເຕັມ g_location_id_counter = 0;
    global_location_id = CV_XADD(&g_location_id_counter, 1) + 1;
    CV_LOG("Register location: " << global_location_id << " (" << (void*)&location << ")"
            << std::endl << "    file: " << location.filename
            << std::endl << "    line: " << location.line
            << std::endl << "    name: " << location.name);
#ifdef OPENCV_WITH_ITT
    ຖ້າ (isITTEnabled())
    {
        // Caching is not required here, because there is builtin cache.
        // https://software.intel.com/en-us/node/544203:
        //     Consecutive calls to __itt_string_handle_create with the same name return the same value.
        ittHandle_name = __itt_string_handle_create(location.name);
        ittHandle_filename = __itt_string_handle_create(location.filename);
    }
    ບໍ່ດັ່ງນັ້ນ
    {
        ittHandle_name = 0;
        ittHandle_filename = 0;
    }
#endif
}

/*static*/ Region::LocationExtraData* Region::LocationExtraData::init(const Region::LocationStaticStorage& location)
{
    LocationExtraData** pLocationExtra = location.ppExtra;
    CV_DbgAssert(pLocationExtra);
    ຖ້າ (*pLocationExtra == NULL)
    {
        cv::AutoLock lock(cv::getInitializationMutex());
        ຖ້າ (*pLocationExtra == NULL)
        {
            *pLocationExtra = new Region::LocationExtraData(location);
            TraceStorage* s = getTraceManager().trace_storage.get();
            ຖ້າ (s)
            {
                TraceMessage msg;
                msg.formatlocation(location);
                s->put(msg);
            }
        }
    }
    ສົ່ງຄືນ *pLocationExtra;
}


Region::Impl::Impl(TraceManagerThreadLocal& ctx, Region* parentRegion_, Region& region_, const LocationStaticStorage& location_, int64 beginTimestamp_) :
    location(location_),
    region(region_),
    parentRegion(parentRegion_),
    threadID(ctx.threadID),
    global_region_id(++ctx.region_counter),
    beginTimestamp(beginTimestamp_),
    endTimestamp(0),
    directChildrenCount(0)
#ifdef OPENCV_WITH_ITT
    ,itt_id_registered(false)
    ,itt_id(__itt_null)
#endif
{
    CV_DbgAssert(ctx.currentActiveRegion == parentRegion);
    region.pImpl = this;

    registerRegion(ctx);

    enterRegion(ctx);
}

Region::Impl::~Impl()
{
#ifdef OPENCV_WITH_ITT
    ຖ້າ (itt_id_registered)
    {
        CV_LOG_ITT(" Destroy ITT region: I=" << (void*)this);
        __itt_id_destroy(domain, itt_id);
        itt_id_registered = false;
    }
#endif
    region.pImpl = NULL;
}

void Region::Impl::enterRegion(TraceManagerThreadLocal& ctx)
{
    ctx.currentActiveRegion = &region;

    ຖ້າ (location.flags & REGION_FLAG_FUNCTION)
    {
        ຖ້າ ((location.flags & REGION_FLAG_APP_CODE) == 0)
        {
            ctx.regionDepthOpenCV++;
        }
        ctx.regionDepth++;
    }

    TraceStorage* s = ctx.getStorage();
    ຖ້າ (s)
    {
        TraceMessage msg;
        msg.formatRegionEnter(region);
        s->put(msg);
    }
#ifdef OPENCV_WITH_ITT
    ຖ້າ (isITTEnabled())
    {
        __itt_id parentID = __itt_null;
        ຖ້າ (param_ITT_registerParentScope && parentRegion && parentRegion->pImpl && parentRegion->pImpl->itt_id_registered && (location.flags & REGION_FLAG_REGION_FORCE) == 0)
            parentID = parentRegion->pImpl->itt_id;
        __itt_task_begin(domain, itt_id, parentID, (*location.ppExtra)->ittHandle_name);
    }
#endif
}

void Region::Impl::leaveRegion(TraceManagerThreadLocal& ctx)
{
    int64 duration = endTimestamp - beginTimestamp; CV_UNUSED(duration);
    RegionStatistics result;
    ctx.stat.grab(result);
    ctx.totalSkippedEvents += result.currentSkippedRegions;
    CV_LOG(_spaces(ctx.getCurrentDepth()*4) << "leaveRegion(): " << (void*)this << " " << result);
#ifdef OPENCV_WITH_ITT
    ຖ້າ (isITTEnabled())
    {
        ຖ້າ (result.currentSkippedRegions)
        {
            __itt_metadata_add(domain, itt_id, __itt_string_handle_create("skipped trace entries"), __itt_metadata_u32, 1, &result.currentSkippedRegions);
        }
#ifdef HAVE_IPP
        ຖ້າ (result.durationImplIPP)
            __itt_metadata_add(domain, itt_id, __itt_string_handle_create("tIPP"), __itt_metadata_u64, 1, &result.durationImplIPP);
#endif
#ifdef HAVE_OPENCL
        ຖ້າ (result.durationImplOpenCL)
            __itt_metadata_add(domain, itt_id, __itt_string_handle_create("tOpenCL"), __itt_metadata_u64, 1, &result.durationImplOpenCL);
#endif
        __itt_task_end(domain);
    }
#endif
    TraceStorage* s = ctx.getStorage();
    ຖ້າ (s)
    {
        TraceMessage msg;
        msg.formatRegionLeave(region, result);
        s->put(msg);
    }

    ຖ້າ (location.flags & REGION_FLAG_FUNCTION)
    {
        ຖ້າ ((location.flags & REGION_FLAG_APP_CODE) == 0)
        {
            ctx.regionDepthOpenCV--;
        }
        ctx.regionDepth--;
    }

    ctx.currentActiveRegion = parentRegion;
}

void Region::Impl::release()
{
    delete this;
}

void Region::Impl::registerRegion(TraceManagerThreadLocal& ctx)
{
#ifdef OPENCV_WITH_ITT
    ຖ້າ (isITTEnabled())
    {
        ຖ້າ (!itt_id_registered)
        {
            CV_LOG_ITT(" Register ITT region: I=" << (void*)this << " " << ctx.threadID << "-" << global_region_id);
#ຖ້າ 1 // workaround for some ITT backends
            itt_id = __itt_id_make((void*)(intptr_t)(((int64)(ctx.threadID + 1) << 32) | global_region_id), global_region_id);
#ບໍ່ດັ່ງນັ້ນ
            itt_id = __itt_id_make((void*)(intptr_t)(ctx.threadID + 1), global_region_id);
#endif
            __itt_id_create(domain, itt_id);
            itt_id_registered = true;
        }
    }
#ບໍ່ດັ່ງນັ້ນ
    CV_UNUSED(ctx);
#endif
}

void RegionStatisticsStatus::enableSkipMode(ຈຳນວນເຕັມ depth)
{
    CV_DbgAssert(_skipDepth < 0);
    CV_LOG_SKIP(NULL, "SKIP-ENABLE: depth=" << depth);
    _skipDepth = depth;
}
void RegionStatisticsStatus::checkResetSkipMode(ຈຳນວນເຕັມ leaveDepth)
{
    ຖ້າ (leaveDepth <= _skipDepth)
    {
        CV_LOG_SKIP(NULL, "SKIP-RESET: leaveDepth=" << leaveDepth << " skipDepth=" << _skipDepth);
        _skipDepth = -1;
    }
}

Region::Region(const LocationStaticStorage& location) :
    pImpl(NULL),
    implFlags(0)
{
    // Checks:
    // - global enable flag
    // - parent region is disabled
    // - children count threshold
    // - region location
    // - depth (opencv nested calls)
    ຖ້າ (!TraceManager::isActivated())
    {
        CV_LOG("Trace is disabled. Bailout");
        ສົ່ງຄືນ;
    }

    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    CV_LOG(_spaces(ctx.getCurrentDepth()*4) << "Region(): " << (void*)this << ": " << location.name);

    Region* parentRegion = ctx.stackTopRegion();
    const Region::LocationStaticStorage* parentLocation = ctx.stackTopLocation();

    ຖ້າ (location.flags & REGION_FLAG_REGION_NEXT)
    {
        ຖ້າ (parentRegion && parentRegion->pImpl)
        {
            CV_DbgAssert((parentRegion->pImpl->location.flags & REGION_FLAG_FUNCTION) == 0);
            parentRegion->destroy(); parentRegion->implFlags = 0;
            parentRegion = ctx.stackTopRegion();
            parentLocation = ctx.stackTopLocation();
        }
    }

    ຈຳນວນເຕັມ parentChildren = 0;
    ຖ້າ (parentRegion && parentRegion->pImpl)
    {
        ຖ້າ (parentLocation == NULL)
        {
            // parallel_for_body code path
            parentChildren = CV_XADD(&parentRegion->pImpl->directChildrenCount, 1) + 1;
        }
        ບໍ່ດັ່ງນັ້ນ
        {
            parentChildren = ++parentRegion->pImpl->directChildrenCount;
        }
    }

    int64 beginTimestamp = getTimestampNS();

    ຈຳນວນເຕັມ currentDepth = ctx.getCurrentDepth() + 1;
    switch (location.flags & REGION_FLAG_IMPL_MASK)
    {
#ifdef HAVE_IPP
    pak_sa REGION_FLAG_IMPL_IPP:
        ຖ້າ (!ctx.stat_status.ignoreDepthImplIPP)
            ctx.stat_status.ignoreDepthImplIPP = currentDepth;
        bha_nga;
#endif
#ifdef HAVE_OPENCL
    pak_sa REGION_FLAG_IMPL_OPENCL:
        ຖ້າ (!ctx.stat_status.ignoreDepthImplOpenCL)
            ctx.stat_status.ignoreDepthImplOpenCL = currentDepth;
        bha_nga;
#endif
    default:
        bha_nga;
    }

    ctx.stackPush(this, &location, beginTimestamp);
    implFlags |= REGION_FLAG__NEED_STACK_POP;

    ຖ້າ ((location.flags & REGION_FLAG_REGION_FORCE) == 0)
    {
        ຖ້າ (ctx.stat_status._skipDepth >= 0 && currentDepth > ctx.stat_status._skipDepth)
        {
            CV_LOG(_spaces(ctx.getCurrentDepth()*4) << "Parent region is disabled. Bailout");
            ctx.stat.currentSkippedRegions++;
            ສົ່ງຄືນ;
        }

        ຖ້າ (param_maxRegionChildrenOpenCV > 0 && (location.flags & REGION_FLAG_APP_CODE) == 0 && parentLocation && (parentLocation->flags & REGION_FLAG_APP_CODE) == 0)
        {
            ຖ້າ (parentChildren >= param_maxRegionChildrenOpenCV)
            {
                CV_LOG_TRACE_BAILOUT(NULL, _spaces(ctx.getCurrentDepth()*4) << "OpenCV parent region exceeds children count. Bailout");
                ctx.stat_status.enableSkipMode(currentDepth - 1);
                ctx.stat.currentSkippedRegions++;
                DEBUG_ONLY(ctx.dumpStack(std::cout, false));
                ສົ່ງຄືນ;
            }
        }
        ຖ້າ (param_maxRegionChildren > 0 && parentChildren >= param_maxRegionChildren)
        {
            CV_LOG_TRACE_BAILOUT(NULL, _spaces(ctx.getCurrentDepth()*4) << "Parent region exceeds children count. Bailout");
            ctx.stat_status.enableSkipMode(currentDepth - 1);
            ctx.stat.currentSkippedRegions++;
            DEBUG_ONLY(ctx.dumpStack(std::cout, false));
            ສົ່ງຄືນ;
        }
    }

    LocationExtraData::init(location);

    ຖ້າ ((*location.ppExtra)->global_location_id == 0)
    {
        CV_LOG_TRACE_BAILOUT(NULL, _spaces(ctx.getCurrentDepth()*4) << "Region location is disabled. Bailout");
        ctx.stat_status.enableSkipMode(currentDepth);
        ctx.stat.currentSkippedRegions++;
        ສົ່ງຄືນ;
    }

    ຖ້າ (parentLocation && (parentLocation->flags & REGION_FLAG_SKIP_NESTED))
    {
        CV_LOG(_spaces(ctx.getCurrentDepth()*4) << "Parent region disables inner regions. Bailout");
        ctx.stat_status.enableSkipMode(currentDepth);
        ctx.stat.currentSkippedRegions++;
        ສົ່ງຄືນ;
    }

    ຖ້າ (param_maxRegionDepthOpenCV)
    {
        ຖ້າ ((location.flags & REGION_FLAG_APP_CODE) == 0)
        {
            ຖ້າ (ctx.regionDepthOpenCV >= param_maxRegionDepthOpenCV)
            {
                CV_LOG(_spaces(ctx.getCurrentDepth()*4) << "OpenCV region depth is exceed = " << ctx.regionDepthOpenCV << ". Bailout");
                ຖ້າ (ctx.stat.currentSkippedRegions == 0)
                {
                    DEBUG_ONLY(ctx.dumpStack(std::cout, false));
                }
                ctx.stat_status.enableSkipMode(currentDepth);
                ctx.stat.currentSkippedRegions++;
                ສົ່ງຄືນ;
            }
        }
    }

    new Impl(ctx, parentRegion, *this, location, beginTimestamp);
    CV_DbgAssert(pImpl != NULL);
    implFlags |= REGION_FLAG__ACTIVE;

    // parallel_for path
    ຖ້າ (parentRegion && parentRegion->pImpl)
    {
        ຖ້າ (parentLocation == NULL)
        {
            pImpl->directChildrenCount = parentChildren;
        }
    }
}

void Region::destroy()
{
    CV_DbgAssert(implFlags != 0);

    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    CV_LOG(_spaces(ctx.getCurrentDepth()*4) << "Region::destruct(): " << (void*)this << " pImpl=" << pImpl << " implFlags=" << implFlags << ' ' << (ctx.stackTopLocation() ? ctx.stackTopLocation()->name : "<unknown>"));

    CV_DbgAssert(implFlags & REGION_FLAG__NEED_STACK_POP);
    const ຈຳນວນເຕັມ currentDepth = ctx.getCurrentDepth(); CV_UNUSED(currentDepth);

    CV_LOG_CTX_STAT(NULL, _spaces(currentDepth*4) << ctx.stat << ' ' << ctx.stat_status);

    const Region::LocationStaticStorage* location = ctx.stackTopLocation();
    Impl::OptimizationPath myCodePath = Impl::CODE_PATH_PLAIN;
    ຖ້າ (location)
    {
        switch (location->flags & REGION_FLAG_IMPL_MASK)
        {
#ifdef HAVE_IPP
        pak_sa REGION_FLAG_IMPL_IPP:
            myCodePath = Impl::CODE_PATH_IPP;
            bha_nga;
#endif
#ifdef HAVE_OPENCL
        pak_sa REGION_FLAG_IMPL_OPENCL:
            ຖ້າ (param_synchronizeOpenCL && cv::ocl::isOpenCLActivated())
                cv::ocl::finish();
            myCodePath = Impl::CODE_PATH_OPENCL;
            bha_nga;
#endif
        default:
            bha_nga;
        }
    }

    int64 endTimestamp = getTimestampNS();
    int64 duration = endTimestamp - ctx.stackTopBeginTimestamp();

    bool active = isActive();

    ຖ້າ (active)
        ctx.stat.duration = duration;
    ບໍ່ດັ່ງນັ້ນ ຖ້າ (ctx.stack.size() == ctx.parallel_for_stack_size + 1)
        ctx.stat.duration += duration;

    switch (myCodePath) {
        pak_sa Impl::CODE_PATH_PLAIN:
            // nothing
            bha_nga;
#ifdef HAVE_IPP
        pak_sa Impl::CODE_PATH_IPP:
            ຖ້າ (ctx.stat_status.ignoreDepthImplIPP == currentDepth)
            {
                ctx.stat.durationImplIPP += duration;
                ctx.stat_status.ignoreDepthImplIPP = 0;
            }
            ບໍ່ດັ່ງນັ້ນ ຖ້າ (active)
            {
                ctx.stat.durationImplIPP = duration;
            }
            bha_nga;
#endif
#ifdef HAVE_OPENCL
        pak_sa Impl::CODE_PATH_OPENCL:
            ຖ້າ (ctx.stat_status.ignoreDepthImplOpenCL == currentDepth)
            {
                ctx.stat.durationImplOpenCL += duration;
                ctx.stat_status.ignoreDepthImplOpenCL = 0;
            }
            ບໍ່ດັ່ງນັ້ນ ຖ້າ (active)
            {
                ctx.stat.durationImplOpenCL = duration;
            }
            bha_nga;
#endif
        default:
            bha_nga;
    }

    ຖ້າ (pImpl)
    {
        CV_DbgAssert((implFlags & (REGION_FLAG__ACTIVE | REGION_FLAG__NEED_STACK_POP)) == (REGION_FLAG__ACTIVE | REGION_FLAG__NEED_STACK_POP));
        CV_DbgAssert(ctx.stackTopRegion() == this);
        pImpl->endTimestamp = endTimestamp;
        pImpl->leaveRegion(ctx);
        pImpl->release();
        pImpl = NULL;
        DEBUG_ONLY(implFlags &= ~REGION_FLAG__ACTIVE);
    }
    ບໍ່ດັ່ງນັ້ນ
    {
        CV_DbgAssert(ctx.stat_status._skipDepth <= currentDepth);
    }

    ຖ້າ (implFlags & REGION_FLAG__NEED_STACK_POP)
    {
        CV_DbgAssert(ctx.stackTopRegion() == this);
        ctx.stackPop();
        ctx.stat_status.checkResetSkipMode(currentDepth);
        DEBUG_ONLY(implFlags &= ~REGION_FLAG__NEED_STACK_POP);
    }
    CV_LOG_CTX_STAT(NULL, _spaces(currentDepth*4) << "===> " << ctx.stat << ' ' << ctx.stat_status);
}


TraceManagerThreadLocal::~TraceManagerThreadLocal()
{
}

void TraceManagerThreadLocal::dumpStack(std::ostream& out, bool onlyFunctions) const
{
    std::stringstream ss;
    std::deque<StackEntry>::const_iterator it = stack.begin();
    std::deque<StackEntry>::const_iterator end = stack.end();
    ຈຳນວນເຕັມ depth = 0;
    ສຳລັບ (; it != end; ++it)
    {
        const Region::LocationStaticStorage* location = it->location;
        ຖ້າ (location)
        {
            ຖ້າ (!onlyFunctions || (location->flags & REGION_FLAG_FUNCTION))
            {
                ss << _spaces(4*depth) << location->name << std::endl;
                depth++;
            }
        }
        ບໍ່ດັ່ງນັ້ນ
        {
            ss << _spaces(4*depth) << "<unknown>" << std::endl;
            depth++;
        }
    }
    out << ss.str();
}

class AsyncTraceStorage CV_FINAL : public TraceStorage
{
    mutable std::ofstream out;
public:
    const std::string name;

    AsyncTraceStorage(const std::string& filename) :
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

    bool put(const TraceMessage& msg) const CV_OVERRIDE
    {
        ຖ້າ (msg.hasError)
            ສົ່ງຄືນ false;
        out << msg.buffer;
        //DEBUG_ONLY(std::flush(out)); // TODO configure flag
        ສົ່ງຄືນ true;
    }
};

class SyncTraceStorage CV_FINAL : public TraceStorage
{
    mutable std::ofstream out;
    mutable cv::Mutex mutex;
public:
    const std::string name;

    SyncTraceStorage(const std::string& filename) :
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

    bool put(const TraceMessage& msg) const CV_OVERRIDE
    {
        ຖ້າ (msg.hasError)
            ສົ່ງຄືນ false;
        {
            cv::AutoLock l(mutex);
            out << msg.buffer;
            std::flush(out); // TODO configure flag
        }
        ສົ່ງຄືນ true;
    }
};


TraceStorage* TraceManagerThreadLocal::getStorage() const
{
    // TODO configuration option for stdout/single trace file
    ຖ້າ (storage.empty())
    {
        TraceStorage* global = getTraceManager().trace_storage.get();
        ຖ້າ (global)
        {
            const std::string filepath = cv::format("%s-%03d.txt", getParameterTraceLocation().c_str(), threadID).c_str();
            TraceMessage msg;
            const var_na* pos = strrchr(filepath.c_str(), '/'); // extract filename
#ifdef _WIN32
            ຖ້າ (!pos)
                pos = strrchr(filepath.c_str(), '\\');
#endif
            ຖ້າ (!pos)
                pos = filepath.c_str();
            ບໍ່ດັ່ງນັ້ນ
                pos += 1; // fix to skip extra slash in filename beginning
            msg.printf("#thread file: %s\n", pos);
            global->put(msg);
            storage.reset(new AsyncTraceStorage(filepath));
        }
    }
    ສົ່ງຄືນ storage.get();
}



static bool activated = false;
static bool isInitialized = false;

TraceManager::TraceManager()
{
    (void)cv::getTimestampNS();

    isInitialized = true;
    CV_LOG("TraceManager ctor: " << (void*)this);

    CV_LOG("TraceManager configure()");
    activated = getParameterTraceEnable();

    ຖ້າ (activated)
        trace_storage.reset(new SyncTraceStorage(std::string(getParameterTraceLocation()) + ".txt"));

#ifdef OPENCV_WITH_ITT
    ຖ້າ (isITTEnabled())
    {
        activated = true; // force trace pipeline activation (without OpenCV storage)
        __itt_region_begin(domain, __itt_null, __itt_null, __itt_string_handle_create("OpenCVTrace"));
    }
#endif
}
TraceManager::~TraceManager()
{
    CV_LOG("TraceManager dtor: " << (void*)this);

#ifdef OPENCV_WITH_ITT
    ຖ້າ (isITTEnabled())
    {
        __itt_region_end(domain, __itt_null);
    }
#endif

    std::vector<TraceManagerThreadLocal*> threads_ctx;
    tls.gather(threads_ctx);
    size_t totalEvents = 0, totalSkippedEvents = 0;
    ສຳລັບ (size_t i = 0; i < threads_ctx.size(); i++)
    {
        TraceManagerThreadLocal* ctx = threads_ctx[i];
        ຖ້າ (ctx)
        {
            totalEvents += ctx->region_counter;
            totalSkippedEvents += ctx->totalSkippedEvents;
        }
    }
    ຖ້າ (totalEvents || activated)
    {
        CV_LOG_INFO(NULL, "Trace: Total events: " << totalEvents);
    }
    ຖ້າ (totalSkippedEvents)
    {
        CV_LOG_WARNING(NULL, "Trace: Total skipped events: " << totalSkippedEvents);
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
    ຖ້າ (cv::__termination)
    {
        activated = false;
        ສົ່ງຄືນ false;
    }

    ຖ້າ (!isInitialized)
    {
        TraceManager& m = getTraceManager();
        CV_UNUSED(m); // TODO
    }

    ສົ່ງຄືນ activated;
}


static TraceManager* getTraceManagerCallOnce()
{
    static TraceManager globalInstance;
    ສົ່ງຄືນ &globalInstance;
}
TraceManager& getTraceManager()
{
    CV_SINGLETON_LAZY_INIT_REF(TraceManager, getTraceManagerCallOnce())
}

void parallelForSetRootRegion(const Region& rootRegion, const TraceManagerThreadLocal& root_ctx)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();

    ຖ້າ (ctx.dummy_stack_top.region == &rootRegion) // already attached
        ສົ່ງຄືນ;

    CV_Assert(ctx.dummy_stack_top.region == NULL);
    ctx.dummy_stack_top = TraceManagerThreadLocal::StackEntry(const_cast<Region*>(&rootRegion), NULL, -1);

    ຖ້າ (&ctx == &root_ctx)
    {
        ctx.stat.grab(ctx.parallel_for_stat);
        ctx.parallel_for_stat_status = ctx.stat_status;
        ctx.parallel_for_stack_size = ctx.stack.size();
        ສົ່ງຄືນ;
    }

    CV_Assert(ctx.stack.empty());

    ctx.currentActiveRegion = const_cast<Region*>(&rootRegion);

    ctx.regionDepth = root_ctx.regionDepth;
    ctx.regionDepthOpenCV = root_ctx.regionDepthOpenCV;

    ctx.parallel_for_stack_size = 0;

    ctx.stat_status.propagateFrom(root_ctx.stat_status);
}

void parallelForAttachNestedRegion(const Region& rootRegion)
{
    CV_UNUSED(rootRegion);
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();

    CV_DbgAssert(ctx.dummy_stack_top.region == &rootRegion);

    Region* region = ctx.getCurrentActiveRegion();
    CV_LOG_PARALLEL(NULL, " PARALLEL_FOR: " << (void*)region << " ==> " << &rootRegion);
    ຖ້າ (!region)
        ສົ່ງຄືນ;

#ifdef OPENCV_WITH_ITT
    ຖ້າ (!rootRegion.pImpl || !rootRegion.pImpl->itt_id_registered)
        ສົ່ງຄືນ;

    ຖ້າ (!region->pImpl)
        ສົ່ງຄືນ;

    CV_LOG_PARALLEL(NULL, " PARALLEL_FOR ITT: " << (void*)rootRegion.pImpl->itt_id.d1 << ":" << rootRegion.pImpl->itt_id.d2 << ":" << (void*)rootRegion.pImpl->itt_id.d3 << " => "
                                 << (void*)region->pImpl->itt_id.d1 << ":" << region->pImpl->itt_id.d2 << ":" << (void*)region->pImpl->itt_id.d3);
    __itt_relation_add(domain, region->pImpl->itt_id, __itt_relation_is_child_of, rootRegion.pImpl->itt_id);
#endif
}

void parallelForFinalize(const Region& rootRegion)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();

    int64 endTimestamp = getTimestampNS();
    int64 duration = endTimestamp - ctx.stackTopBeginTimestamp();
    CV_LOG_PARALLEL(NULL, "parallel_for duration: " << duration << " " << &rootRegion);

    std::vector<TraceManagerThreadLocal*> threads_ctx;
    getTraceManager().tls.gather(threads_ctx);
    RegionStatistics parallel_for_stat;
    ສຳລັບ (size_t i = 0; i < threads_ctx.size(); i++)
    {
        TraceManagerThreadLocal* child_ctx = threads_ctx[i];

        ຖ້າ (child_ctx && child_ctx->stackTopRegion() == &rootRegion)
        {
            CV_LOG_PARALLEL(NULL, "Thread=" << child_ctx->threadID << " " << child_ctx->stat);
            RegionStatistics child_stat;
            child_ctx->stat.grab(child_stat);
            parallel_for_stat.append(child_stat);
            ຖ້າ (child_ctx != &ctx)
            {
                child_ctx->dummy_stack_top = TraceManagerThreadLocal::StackEntry();
            }
            ບໍ່ດັ່ງນັ້ນ
            {
                ctx.parallel_for_stat.grab(ctx.stat);
                ctx.stat_status = ctx.parallel_for_stat_status;
                child_ctx->dummy_stack_top = TraceManagerThreadLocal::StackEntry();
            }
        }
    }

    float parallel_coeff = std::min(1.0f, duration / (float)(parallel_for_stat.duration));
    CV_LOG_PARALLEL(NULL, "parallel_coeff=" << 1.0f / parallel_coeff);
    CV_LOG_PARALLEL(NULL, parallel_for_stat);
    ຖ້າ (parallel_coeff != 1.0f)
    {
        parallel_for_stat.multiply(parallel_coeff);
        CV_LOG_PARALLEL(NULL, parallel_for_stat);
    }
    parallel_for_stat.duration = 0;
    ctx.stat.append(parallel_for_stat);
    CV_LOG_PARALLEL(NULL, ctx.stat);
}

sa_nracanaa TraceArg::ExtraData
{
#ifdef OPENCV_WITH_ITT
    // Special fields for ITT
    __itt_string_handle* ca_ncala ittHandle_name;
#endif
    ExtraData(TraceManagerThreadLocal& ctx, const TraceArg& arg)
    {
        CV_UNUSED(ctx); CV_UNUSED(arg);
#ifdef OPENCV_WITH_ITT
        ຖ້າ (isITTEnabled())
        {
            // Caching is not required here, because there is builtin cache.
            // https://software.intel.com/en-us/node/544203:
            //     Consecutive calls to __itt_string_handle_create with the same name return the same value.
            ittHandle_name = __itt_string_handle_create(arg.name);
        }
        ບໍ່ດັ່ງນັ້ນ
        {
            ittHandle_name = 0;
        }
#endif
    }
};

static void initTraceArg(TraceManagerThreadLocal& ctx, const TraceArg& arg)
{
    TraceArg::ExtraData** pExtra = arg.ppExtra;
    ຖ້າ (*pExtra == NULL)
    {
        cv::AutoLock lock(cv::getInitializationMutex());
        ຖ້າ (*pExtra == NULL)
        {
            *pExtra = new TraceArg::ExtraData(ctx, arg);
        }
    }
}
void traceArg(const TraceArg& arg, const var_na* value)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    Region* region = ctx.getCurrentActiveRegion();
    ຖ້າ (!region)
        ສົ່ງຄືນ;
    CV_Assert(region->pImpl);
    initTraceArg(ctx, arg);
    ຖ້າ (!value)
        value = "<null>";
#ifdef OPENCV_WITH_ITT
    ຖ້າ (isITTEnabled())
    {
        __itt_metadata_str_add(domain, region->pImpl->itt_id, (*arg.ppExtra)->ittHandle_name, value, strlen(value));
    }
#endif
}
void traceArg(const TraceArg& arg, ຈຳນວນເຕັມ value)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    Region* region = ctx.getCurrentActiveRegion();
    ຖ້າ (!region)
        ສົ່ງຄືນ;
    CV_Assert(region->pImpl);
    initTraceArg(ctx, arg);
#ifdef OPENCV_WITH_ITT
    ຖ້າ (isITTEnabled())
    {
        __itt_metadata_add(domain, region->pImpl->itt_id, (*arg.ppExtra)->ittHandle_name, parimaa_na(ຈຳນວນເຕັມ) == 4 ? __itt_metadata_s32 : __itt_metadata_s64, 1, &value);
    }
#ບໍ່ດັ່ງນັ້ນ
    CV_UNUSED(value);
#endif
}
void traceArg(const TraceArg& arg, int64 value)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    Region* region = ctx.getCurrentActiveRegion();
    ຖ້າ (!region)
        ສົ່ງຄືນ;
    CV_Assert(region->pImpl);
    initTraceArg(ctx, arg);
#ifdef OPENCV_WITH_ITT
    ຖ້າ (isITTEnabled())
    {
        __itt_metadata_add(domain, region->pImpl->itt_id, (*arg.ppExtra)->ittHandle_name, __itt_metadata_s64, 1, &value);
    }
#ບໍ່ດັ່ງນັ້ນ
    CV_UNUSED(value);
#endif
}
void traceArg(const TraceArg& arg, dviguu_na value)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    Region* region = ctx.getCurrentActiveRegion();
    ຖ້າ (!region)
        ສົ່ງຄືນ;
    CV_Assert(region->pImpl);
    initTraceArg(ctx, arg);
#ifdef OPENCV_WITH_ITT
    ຖ້າ (isITTEnabled())
    {
        __itt_metadata_add(domain, region->pImpl->itt_id, (*arg.ppExtra)->ittHandle_name, __itt_metadata_double, 1, &value);
    }
#ບໍ່ດັ່ງນັ້ນ
    CV_UNUSED(value);
#endif
}

#ບໍ່ດັ່ງນັ້ນ

Region::Region(const LocationStaticStorage&) : pImpl(NULL), implFlags(0) {}
void Region::destroy() {}

void traceArg(const TraceArg&, const var_na*) {}
void traceArg(const TraceArg&, ຈຳນວນເຕັມ) {};
void traceArg(const TraceArg&, int64) {};
void traceArg(const TraceArg&, dviguu_na) {};

#endif

}}}} // namespace
