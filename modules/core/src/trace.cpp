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

#якщо 0
#define CV_LOG(...) CV_LOG_INFO(NULL, __VA_ARGS__)
#інакше
#define CV_LOG(...) {}
#endif

#якщо 0
#define CV_LOG_ITT(...) CV_LOG_INFO(NULL, __VA_ARGS__)
#інакше
#define CV_LOG_ITT(...) {}
#endif

#якщо 1
#define CV_LOG_TRACE_BAILOUT(tag, ...) CV_LOG_INFO(tag, __VA_ARGS__)
#інакше
#define CV_LOG_TRACE_BAILOUT(...) {}
#endif

#якщо 0
#define CV_LOG_PARALLEL(tag, ...) CV_LOG_INFO(tag, __VA_ARGS__)
#інакше
#define CV_LOG_PARALLEL(...) {}
#endif

#якщо 0
#define CV_LOG_CTX_STAT(tag, ...) CV_LOG_INFO(tag, __VA_ARGS__)
#інакше
#define CV_LOG_CTX_STAT(...) {}
#endif

#якщо 0
#define CV_LOG_SKIP(tag, ...) CV_LOG_INFO(tag, __VA_ARGS__)
#інакше
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
    повернути param_traceEnable;
}

// TODO lazy configuration flags
static ціле param_maxRegionDepthOpenCV = (ціле)utils::getConfigurationParameterSizeT("OPENCV_TRACE_DEPTH_OPENCV", 1);
static ціле param_maxRegionChildrenOpenCV = (ціле)utils::getConfigurationParameterSizeT("OPENCV_TRACE_MAX_CHILDREN_OPENCV", 1000);
static ціле param_maxRegionChildren = (ціле)utils::getConfigurationParameterSizeT("OPENCV_TRACE_MAX_CHILDREN", 10000);

static const cv::String& getParameterTraceLocation()
{
    static cv::String param_traceLocation = utils::getConfigurationParameterString("OPENCV_TRACE_LOCATION", "OpenCVTrace");
    повернути param_traceLocation;
}

#ifdef HAVE_OPENCL
static bool param_synchronizeOpenCL = utils::getConfigurationParameterBool("OPENCV_TRACE_SYNC_OPENCL", false);
#endif

#ifdef OPENCV_WITH_ITT
static bool param_ITT_registerParentScope = utils::getConfigurationParameterBool("OPENCV_TRACE_ITT_PARENT", false);
#endif

static const char* _spaces(ціле count)
{
    static const char buf[64] =
"                                                               ";
    повернути &buf[63 - (count & 63)];
}

/**
 * Text-based trace messages
 */
class TraceMessage
{
public:
    char buffer[1024];
    size_t len;
    bool hasError;

    TraceMessage() :
        len(0),
        hasError(false)
    {}

    bool printf(const char* format, ...)
    {
        char* buf = &buffer[len];
        size_t sz = sizeof(buffer) - len;
        va_list ap;
        va_start(ap, format);
        ціле n = cv_vsnprintf(buf, (ціле)sz, format, ap);
        va_end(ap);
        якщо (n < 0 || (size_t)n > sz)
        {
            hasError = true;
            повернути false;
        }
        len += n;
        повернути true;
    }

    bool formatlocation(const Region::LocationStaticStorage& location)
    {
        повернути this->printf("l,%lld,\"%s\",%d,\"%s\",0x%llX\n",
                (long long ціле)(*location.ppExtra)->global_location_id,
                location.filename,
                location.line,
                location.name,
                (long long ціле)(location.flags & ~0xF0000000));
    }
    bool formatRegionEnter(const Region& region)
    {
        bool ok = this->printf("b,%d,%lld,%lld,%lld",
                (ціле)region.pImpl->threadID,
                (long long ціле)region.pImpl->beginTimestamp,
                (long long ціле)((*region.pImpl->location.ppExtra)->global_location_id),
                (long long ціле)region.pImpl->global_region_id);
        якщо (region.pImpl->parentRegion && region.pImpl->parentRegion->pImpl)
        {
            якщо (region.pImpl->parentRegion->pImpl->threadID != region.pImpl->threadID)
                ok &= this->printf(",parentThread=%d,parent=%lld",
                        (ціле)region.pImpl->parentRegion->pImpl->threadID,
                        (long long ціле)region.pImpl->parentRegion->pImpl->global_region_id);
        }
        ok &= this->printf("\n");
        повернути ok;
    }
    bool formatRegionLeave(const Region& region, const RegionStatistics& result)
    {
        CV_DbgAssert(region.pImpl->endTimestamp - region.pImpl->beginTimestamp == result.duration);
        bool ok = this->printf("e,%d,%lld,%lld,%lld,%lld",
                (ціле)region.pImpl->threadID,
                (long long ціле)region.pImpl->endTimestamp,
                (long long ціле)(*region.pImpl->location.ppExtra)->global_location_id,
                (long long ціле)region.pImpl->global_region_id,
                (long long ціле)result.duration);
        якщо (result.currentSkippedRegions)
            ok &= this->printf(",skip=%d", (ціле)result.currentSkippedRegions);
#ifdef HAVE_IPP
        якщо (result.durationImplIPP)
            ok &= this->printf(",tIPP=%lld", (long long ціле)result.durationImplIPP);
#endif
#ifdef HAVE_OPENCL
        якщо (result.durationImplOpenCL)
            ok &= this->printf(",tOCL=%lld", (long long ціле)result.durationImplOpenCL);
#endif
        ok &= this->printf("\n");
        повернути ok;
    }
    bool recordRegionArg(const Region& region, const TraceArg& arg, const char* value)
    {
        повернути this->printf("a,%d,%lld,%lld,\"%s\",\"%s\"\n",
                region.pImpl->threadID,
                (long long ціле)region.pImpl->beginTimestamp,
                (long long ціле)region.pImpl->global_region_id,
                arg.name,
                value);
    }
};


#ifdef OPENCV_WITH_ITT
static __itt_domain* domain = NULL;

static bool isITTEnabled()
{
    static volatile bool isInitialized = false;
    static bool isEnabled = false;
    якщо (!isInitialized)
    {
        cv::AutoLock lock(cv::getInitializationMutex());
        якщо (!isInitialized)
        {
            bool param_traceITTEnable = utils::getConfigurationParameterBool("OPENCV_TRACE_ITT_ENABLE", true);
            якщо (param_traceITTEnable)
            {
                isEnabled = !!(__itt_api_version());
                CV_LOG_ITT("ITT is " << (isEnabled ? "enabled" : "disabled"));
                domain = __itt_domain_create("OpenCVTrace");
            }
            інакше
            {
                CV_LOG_ITT("ITT is disabled through OpenCV parameter");
                isEnabled = false;
            }
            isInitialized = true;
        }
    }
    повернути isEnabled;
}
#endif


Region::LocationExtraData::LocationExtraData(const LocationStaticStorage& location)
{
    CV_UNUSED(location);
    static ціле g_location_id_counter = 0;
    global_location_id = CV_XADD(&g_location_id_counter, 1) + 1;
    CV_LOG("Register location: " << global_location_id << " (" << (void*)&location << ")"
            << std::endl << "    file: " << location.filename
            << std::endl << "    line: " << location.line
            << std::endl << "    name: " << location.name);
#ifdef OPENCV_WITH_ITT
    якщо (isITTEnabled())
    {
        // Caching is not required here, because there is builtin cache.
        // https://software.intel.com/en-us/node/544203:
        //     Consecutive calls to __itt_string_handle_create with the same name return the same value.
        ittHandle_name = __itt_string_handle_create(location.name);
        ittHandle_filename = __itt_string_handle_create(location.filename);
    }
    інакше
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
    якщо (*pLocationExtra == NULL)
    {
        cv::AutoLock lock(cv::getInitializationMutex());
        якщо (*pLocationExtra == NULL)
        {
            *pLocationExtra = new Region::LocationExtraData(location);
            TraceStorage* s = getTraceManager().trace_storage.get();
            якщо (s)
            {
                TraceMessage msg;
                msg.formatlocation(location);
                s->put(msg);
            }
        }
    }
    повернути *pLocationExtra;
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
    якщо (itt_id_registered)
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

    якщо (location.flags & REGION_FLAG_FUNCTION)
    {
        якщо ((location.flags & REGION_FLAG_APP_CODE) == 0)
        {
            ctx.regionDepthOpenCV++;
        }
        ctx.regionDepth++;
    }

    TraceStorage* s = ctx.getStorage();
    якщо (s)
    {
        TraceMessage msg;
        msg.formatRegionEnter(region);
        s->put(msg);
    }
#ifdef OPENCV_WITH_ITT
    якщо (isITTEnabled())
    {
        __itt_id parentID = __itt_null;
        якщо (param_ITT_registerParentScope && parentRegion && parentRegion->pImpl && parentRegion->pImpl->itt_id_registered && (location.flags & REGION_FLAG_REGION_FORCE) == 0)
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
    якщо (isITTEnabled())
    {
        якщо (result.currentSkippedRegions)
        {
            __itt_metadata_add(domain, itt_id, __itt_string_handle_create("skipped trace entries"), __itt_metadata_u32, 1, &result.currentSkippedRegions);
        }
#ifdef HAVE_IPP
        якщо (result.durationImplIPP)
            __itt_metadata_add(domain, itt_id, __itt_string_handle_create("tIPP"), __itt_metadata_u64, 1, &result.durationImplIPP);
#endif
#ifdef HAVE_OPENCL
        якщо (result.durationImplOpenCL)
            __itt_metadata_add(domain, itt_id, __itt_string_handle_create("tOpenCL"), __itt_metadata_u64, 1, &result.durationImplOpenCL);
#endif
        __itt_task_end(domain);
    }
#endif
    TraceStorage* s = ctx.getStorage();
    якщо (s)
    {
        TraceMessage msg;
        msg.formatRegionLeave(region, result);
        s->put(msg);
    }

    якщо (location.flags & REGION_FLAG_FUNCTION)
    {
        якщо ((location.flags & REGION_FLAG_APP_CODE) == 0)
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
    якщо (isITTEnabled())
    {
        якщо (!itt_id_registered)
        {
            CV_LOG_ITT(" Register ITT region: I=" << (void*)this << " " << ctx.threadID << "-" << global_region_id);
#якщо 1 // workaround for some ITT backends
            itt_id = __itt_id_make((void*)(intptr_t)(((int64)(ctx.threadID + 1) << 32) | global_region_id), global_region_id);
#інакше
            itt_id = __itt_id_make((void*)(intptr_t)(ctx.threadID + 1), global_region_id);
#endif
            __itt_id_create(domain, itt_id);
            itt_id_registered = true;
        }
    }
#інакше
    CV_UNUSED(ctx);
#endif
}

void RegionStatisticsStatus::enableSkipMode(ціле depth)
{
    CV_DbgAssert(_skipDepth < 0);
    CV_LOG_SKIP(NULL, "SKIP-ENABLE: depth=" << depth);
    _skipDepth = depth;
}
void RegionStatisticsStatus::checkResetSkipMode(ціле leaveDepth)
{
    якщо (leaveDepth <= _skipDepth)
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
    якщо (!TraceManager::isActivated())
    {
        CV_LOG("Trace is disabled. Bailout");
        повернути;
    }

    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    CV_LOG(_spaces(ctx.getCurrentDepth()*4) << "Region(): " << (void*)this << ": " << location.name);

    Region* parentRegion = ctx.stackTopRegion();
    const Region::LocationStaticStorage* parentLocation = ctx.stackTopLocation();

    якщо (location.flags & REGION_FLAG_REGION_NEXT)
    {
        якщо (parentRegion && parentRegion->pImpl)
        {
            CV_DbgAssert((parentRegion->pImpl->location.flags & REGION_FLAG_FUNCTION) == 0);
            parentRegion->destroy(); parentRegion->implFlags = 0;
            parentRegion = ctx.stackTopRegion();
            parentLocation = ctx.stackTopLocation();
        }
    }

    ціле parentChildren = 0;
    якщо (parentRegion && parentRegion->pImpl)
    {
        якщо (parentLocation == NULL)
        {
            // parallel_for_body code path
            parentChildren = CV_XADD(&parentRegion->pImpl->directChildrenCount, 1) + 1;
        }
        інакше
        {
            parentChildren = ++parentRegion->pImpl->directChildrenCount;
        }
    }

    int64 beginTimestamp = getTimestampNS();

    ціле currentDepth = ctx.getCurrentDepth() + 1;
    switch (location.flags & REGION_FLAG_IMPL_MASK)
    {
#ifdef HAVE_IPP
    case REGION_FLAG_IMPL_IPP:
        якщо (!ctx.stat_status.ignoreDepthImplIPP)
            ctx.stat_status.ignoreDepthImplIPP = currentDepth;
        break;
#endif
#ifdef HAVE_OPENCL
    case REGION_FLAG_IMPL_OPENCL:
        якщо (!ctx.stat_status.ignoreDepthImplOpenCL)
            ctx.stat_status.ignoreDepthImplOpenCL = currentDepth;
        break;
#endif
    default:
        break;
    }

    ctx.stackPush(this, &location, beginTimestamp);
    implFlags |= REGION_FLAG__NEED_STACK_POP;

    якщо ((location.flags & REGION_FLAG_REGION_FORCE) == 0)
    {
        якщо (ctx.stat_status._skipDepth >= 0 && currentDepth > ctx.stat_status._skipDepth)
        {
            CV_LOG(_spaces(ctx.getCurrentDepth()*4) << "Parent region is disabled. Bailout");
            ctx.stat.currentSkippedRegions++;
            повернути;
        }

        якщо (param_maxRegionChildrenOpenCV > 0 && (location.flags & REGION_FLAG_APP_CODE) == 0 && parentLocation && (parentLocation->flags & REGION_FLAG_APP_CODE) == 0)
        {
            якщо (parentChildren >= param_maxRegionChildrenOpenCV)
            {
                CV_LOG_TRACE_BAILOUT(NULL, _spaces(ctx.getCurrentDepth()*4) << "OpenCV parent region exceeds children count. Bailout");
                ctx.stat_status.enableSkipMode(currentDepth - 1);
                ctx.stat.currentSkippedRegions++;
                DEBUG_ONLY(ctx.dumpStack(std::cout, false));
                повернути;
            }
        }
        якщо (param_maxRegionChildren > 0 && parentChildren >= param_maxRegionChildren)
        {
            CV_LOG_TRACE_BAILOUT(NULL, _spaces(ctx.getCurrentDepth()*4) << "Parent region exceeds children count. Bailout");
            ctx.stat_status.enableSkipMode(currentDepth - 1);
            ctx.stat.currentSkippedRegions++;
            DEBUG_ONLY(ctx.dumpStack(std::cout, false));
            повернути;
        }
    }

    LocationExtraData::init(location);

    якщо ((*location.ppExtra)->global_location_id == 0)
    {
        CV_LOG_TRACE_BAILOUT(NULL, _spaces(ctx.getCurrentDepth()*4) << "Region location is disabled. Bailout");
        ctx.stat_status.enableSkipMode(currentDepth);
        ctx.stat.currentSkippedRegions++;
        повернути;
    }

    якщо (parentLocation && (parentLocation->flags & REGION_FLAG_SKIP_NESTED))
    {
        CV_LOG(_spaces(ctx.getCurrentDepth()*4) << "Parent region disables inner regions. Bailout");
        ctx.stat_status.enableSkipMode(currentDepth);
        ctx.stat.currentSkippedRegions++;
        повернути;
    }

    якщо (param_maxRegionDepthOpenCV)
    {
        якщо ((location.flags & REGION_FLAG_APP_CODE) == 0)
        {
            якщо (ctx.regionDepthOpenCV >= param_maxRegionDepthOpenCV)
            {
                CV_LOG(_spaces(ctx.getCurrentDepth()*4) << "OpenCV region depth is exceed = " << ctx.regionDepthOpenCV << ". Bailout");
                якщо (ctx.stat.currentSkippedRegions == 0)
                {
                    DEBUG_ONLY(ctx.dumpStack(std::cout, false));
                }
                ctx.stat_status.enableSkipMode(currentDepth);
                ctx.stat.currentSkippedRegions++;
                повернути;
            }
        }
    }

    new Impl(ctx, parentRegion, *this, location, beginTimestamp);
    CV_DbgAssert(pImpl != NULL);
    implFlags |= REGION_FLAG__ACTIVE;

    // parallel_for path
    якщо (parentRegion && parentRegion->pImpl)
    {
        якщо (parentLocation == NULL)
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
    const ціле currentDepth = ctx.getCurrentDepth(); CV_UNUSED(currentDepth);

    CV_LOG_CTX_STAT(NULL, _spaces(currentDepth*4) << ctx.stat << ' ' << ctx.stat_status);

    const Region::LocationStaticStorage* location = ctx.stackTopLocation();
    Impl::OptimizationPath myCodePath = Impl::CODE_PATH_PLAIN;
    якщо (location)
    {
        switch (location->flags & REGION_FLAG_IMPL_MASK)
        {
#ifdef HAVE_IPP
        case REGION_FLAG_IMPL_IPP:
            myCodePath = Impl::CODE_PATH_IPP;
            break;
#endif
#ifdef HAVE_OPENCL
        case REGION_FLAG_IMPL_OPENCL:
            якщо (param_synchronizeOpenCL && cv::ocl::isOpenCLActivated())
                cv::ocl::finish();
            myCodePath = Impl::CODE_PATH_OPENCL;
            break;
#endif
        default:
            break;
        }
    }

    int64 endTimestamp = getTimestampNS();
    int64 duration = endTimestamp - ctx.stackTopBeginTimestamp();

    bool active = isActive();

    якщо (active)
        ctx.stat.duration = duration;
    інакше якщо (ctx.stack.size() == ctx.parallel_for_stack_size + 1)
        ctx.stat.duration += duration;

    switch (myCodePath) {
        case Impl::CODE_PATH_PLAIN:
            // nothing
            break;
#ifdef HAVE_IPP
        case Impl::CODE_PATH_IPP:
            якщо (ctx.stat_status.ignoreDepthImplIPP == currentDepth)
            {
                ctx.stat.durationImplIPP += duration;
                ctx.stat_status.ignoreDepthImplIPP = 0;
            }
            інакше якщо (active)
            {
                ctx.stat.durationImplIPP = duration;
            }
            break;
#endif
#ifdef HAVE_OPENCL
        case Impl::CODE_PATH_OPENCL:
            якщо (ctx.stat_status.ignoreDepthImplOpenCL == currentDepth)
            {
                ctx.stat.durationImplOpenCL += duration;
                ctx.stat_status.ignoreDepthImplOpenCL = 0;
            }
            інакше якщо (active)
            {
                ctx.stat.durationImplOpenCL = duration;
            }
            break;
#endif
        default:
            break;
    }

    якщо (pImpl)
    {
        CV_DbgAssert((implFlags & (REGION_FLAG__ACTIVE | REGION_FLAG__NEED_STACK_POP)) == (REGION_FLAG__ACTIVE | REGION_FLAG__NEED_STACK_POP));
        CV_DbgAssert(ctx.stackTopRegion() == this);
        pImpl->endTimestamp = endTimestamp;
        pImpl->leaveRegion(ctx);
        pImpl->release();
        pImpl = NULL;
        DEBUG_ONLY(implFlags &= ~REGION_FLAG__ACTIVE);
    }
    інакше
    {
        CV_DbgAssert(ctx.stat_status._skipDepth <= currentDepth);
    }

    якщо (implFlags & REGION_FLAG__NEED_STACK_POP)
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
    ціле depth = 0;
    для (; it != end; ++it)
    {
        const Region::LocationStaticStorage* location = it->location;
        якщо (location)
        {
            якщо (!onlyFunctions || (location->flags & REGION_FLAG_FUNCTION))
            {
                ss << _spaces(4*depth) << location->name << std::endl;
                depth++;
            }
        }
        інакше
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
        якщо (msg.hasError)
            повернути false;
        out << msg.buffer;
        //DEBUG_ONLY(std::flush(out)); // TODO configure flag
        повернути true;
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
        якщо (msg.hasError)
            повернути false;
        {
            cv::AutoLock l(mutex);
            out << msg.buffer;
            std::flush(out); // TODO configure flag
        }
        повернути true;
    }
};


TraceStorage* TraceManagerThreadLocal::getStorage() const
{
    // TODO configuration option for stdout/single trace file
    якщо (storage.empty())
    {
        TraceStorage* global = getTraceManager().trace_storage.get();
        якщо (global)
        {
            const std::string filepath = cv::format("%s-%03d.txt", getParameterTraceLocation().c_str(), threadID).c_str();
            TraceMessage msg;
            const char* pos = strrchr(filepath.c_str(), '/'); // extract filename
#ifdef _WIN32
            якщо (!pos)
                pos = strrchr(filepath.c_str(), '\\');
#endif
            якщо (!pos)
                pos = filepath.c_str();
            інакше
                pos += 1; // fix to skip extra slash in filename beginning
            msg.printf("#thread file: %s\n", pos);
            global->put(msg);
            storage.reset(new AsyncTraceStorage(filepath));
        }
    }
    повернути storage.get();
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

    якщо (activated)
        trace_storage.reset(new SyncTraceStorage(std::string(getParameterTraceLocation()) + ".txt"));

#ifdef OPENCV_WITH_ITT
    якщо (isITTEnabled())
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
    якщо (isITTEnabled())
    {
        __itt_region_end(domain, __itt_null);
    }
#endif

    std::vector<TraceManagerThreadLocal*> threads_ctx;
    tls.gather(threads_ctx);
    size_t totalEvents = 0, totalSkippedEvents = 0;
    для (size_t i = 0; i < threads_ctx.size(); i++)
    {
        TraceManagerThreadLocal* ctx = threads_ctx[i];
        якщо (ctx)
        {
            totalEvents += ctx->region_counter;
            totalSkippedEvents += ctx->totalSkippedEvents;
        }
    }
    якщо (totalEvents || activated)
    {
        CV_LOG_INFO(NULL, "Trace: Total events: " << totalEvents);
    }
    якщо (totalSkippedEvents)
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
    якщо (cv::__termination)
    {
        activated = false;
        повернути false;
    }

    якщо (!isInitialized)
    {
        TraceManager& m = getTraceManager();
        CV_UNUSED(m); // TODO
    }

    повернути activated;
}


static TraceManager* getTraceManagerCallOnce()
{
    static TraceManager globalInstance;
    повернути &globalInstance;
}
TraceManager& getTraceManager()
{
    CV_SINGLETON_LAZY_INIT_REF(TraceManager, getTraceManagerCallOnce())
}

void parallelForSetRootRegion(const Region& rootRegion, const TraceManagerThreadLocal& root_ctx)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();

    якщо (ctx.dummy_stack_top.region == &rootRegion) // already attached
        повернути;

    CV_Assert(ctx.dummy_stack_top.region == NULL);
    ctx.dummy_stack_top = TraceManagerThreadLocal::StackEntry(const_cast<Region*>(&rootRegion), NULL, -1);

    якщо (&ctx == &root_ctx)
    {
        ctx.stat.grab(ctx.parallel_for_stat);
        ctx.parallel_for_stat_status = ctx.stat_status;
        ctx.parallel_for_stack_size = ctx.stack.size();
        повернути;
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
    якщо (!region)
        повернути;

#ifdef OPENCV_WITH_ITT
    якщо (!rootRegion.pImpl || !rootRegion.pImpl->itt_id_registered)
        повернути;

    якщо (!region->pImpl)
        повернути;

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
    для (size_t i = 0; i < threads_ctx.size(); i++)
    {
        TraceManagerThreadLocal* child_ctx = threads_ctx[i];

        якщо (child_ctx && child_ctx->stackTopRegion() == &rootRegion)
        {
            CV_LOG_PARALLEL(NULL, "Thread=" << child_ctx->threadID << " " << child_ctx->stat);
            RegionStatistics child_stat;
            child_ctx->stat.grab(child_stat);
            parallel_for_stat.append(child_stat);
            якщо (child_ctx != &ctx)
            {
                child_ctx->dummy_stack_top = TraceManagerThreadLocal::StackEntry();
            }
            інакше
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
    якщо (parallel_coeff != 1.0f)
    {
        parallel_for_stat.multiply(parallel_coeff);
        CV_LOG_PARALLEL(NULL, parallel_for_stat);
    }
    parallel_for_stat.duration = 0;
    ctx.stat.append(parallel_for_stat);
    CV_LOG_PARALLEL(NULL, ctx.stat);
}

struct TraceArg::ExtraData
{
#ifdef OPENCV_WITH_ITT
    // Special fields for ITT
    __itt_string_handle* volatile ittHandle_name;
#endif
    ExtraData(TraceManagerThreadLocal& ctx, const TraceArg& arg)
    {
        CV_UNUSED(ctx); CV_UNUSED(arg);
#ifdef OPENCV_WITH_ITT
        якщо (isITTEnabled())
        {
            // Caching is not required here, because there is builtin cache.
            // https://software.intel.com/en-us/node/544203:
            //     Consecutive calls to __itt_string_handle_create with the same name return the same value.
            ittHandle_name = __itt_string_handle_create(arg.name);
        }
        інакше
        {
            ittHandle_name = 0;
        }
#endif
    }
};

static void initTraceArg(TraceManagerThreadLocal& ctx, const TraceArg& arg)
{
    TraceArg::ExtraData** pExtra = arg.ppExtra;
    якщо (*pExtra == NULL)
    {
        cv::AutoLock lock(cv::getInitializationMutex());
        якщо (*pExtra == NULL)
        {
            *pExtra = new TraceArg::ExtraData(ctx, arg);
        }
    }
}
void traceArg(const TraceArg& arg, const char* value)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    Region* region = ctx.getCurrentActiveRegion();
    якщо (!region)
        повернути;
    CV_Assert(region->pImpl);
    initTraceArg(ctx, arg);
    якщо (!value)
        value = "<null>";
#ifdef OPENCV_WITH_ITT
    якщо (isITTEnabled())
    {
        __itt_metadata_str_add(domain, region->pImpl->itt_id, (*arg.ppExtra)->ittHandle_name, value, strlen(value));
    }
#endif
}
void traceArg(const TraceArg& arg, ціле value)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    Region* region = ctx.getCurrentActiveRegion();
    якщо (!region)
        повернути;
    CV_Assert(region->pImpl);
    initTraceArg(ctx, arg);
#ifdef OPENCV_WITH_ITT
    якщо (isITTEnabled())
    {
        __itt_metadata_add(domain, region->pImpl->itt_id, (*arg.ppExtra)->ittHandle_name, sizeof(ціле) == 4 ? __itt_metadata_s32 : __itt_metadata_s64, 1, &value);
    }
#інакше
    CV_UNUSED(value);
#endif
}
void traceArg(const TraceArg& arg, int64 value)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    Region* region = ctx.getCurrentActiveRegion();
    якщо (!region)
        повернути;
    CV_Assert(region->pImpl);
    initTraceArg(ctx, arg);
#ifdef OPENCV_WITH_ITT
    якщо (isITTEnabled())
    {
        __itt_metadata_add(domain, region->pImpl->itt_id, (*arg.ppExtra)->ittHandle_name, __itt_metadata_s64, 1, &value);
    }
#інакше
    CV_UNUSED(value);
#endif
}
void traceArg(const TraceArg& arg, double value)
{
    TraceManagerThreadLocal& ctx = getTraceManager().tls.getRef();
    Region* region = ctx.getCurrentActiveRegion();
    якщо (!region)
        повернути;
    CV_Assert(region->pImpl);
    initTraceArg(ctx, arg);
#ifdef OPENCV_WITH_ITT
    якщо (isITTEnabled())
    {
        __itt_metadata_add(domain, region->pImpl->itt_id, (*arg.ppExtra)->ittHandle_name, __itt_metadata_double, 1, &value);
    }
#інакше
    CV_UNUSED(value);
#endif
}

#інакше

Region::Region(const LocationStaticStorage&) : pImpl(NULL), implFlags(0) {}
void Region::destroy() {}

void traceArg(const TraceArg&, const char*) {}
void traceArg(const TraceArg&, ціле) {};
void traceArg(const TraceArg&, int64) {};
void traceArg(const TraceArg&, double) {};

#endif

}}}} // namespace
