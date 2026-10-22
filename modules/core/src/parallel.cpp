/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                           License Agreement
//                For Open Source Computer Vision Library
//
// Copyright (C) 2000-2008, Intel Corporation, all rights reserved.
// Copyright (C) 2009-2011, Willow Garage Inc., all rights reserved.
// Third party copyrights are property of their respective owners.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//   * Redistribution's of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//   * Redistribution's in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
//   * The name of the copyright holders may not be used to endorse or promote products
//     derived from this software without specific prior written permission.
//
// This software is provided by the copyright holders and contributors "as is" and
// any express or implied warranties, including, but not limited to, the implied
// warranties of merchantability and fitness for a particular purpose are disclaimed.
// In no event shall the Intel Corporation or contributors be liable for any direct,
// indirect, incidental, special, exemplary, or consequential damages
// (including, but not limited to, procurement of substitute goods or services;
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//
//M*/

#include "precomp.hpp"

#include <opencv2/core/utils/configuration.private.hpp>
#include <opencv2/core/utils/trace.private.hpp>

#include "opencv2/core/parallel/parallel_backend.hpp"
#include "parallel/parallel.hpp"

#如果 defined _WIN32 || defined WINCE
    #include <windows.h>
    #undef small
    #undef min
    #undef max
    #undef abs
#endif

#如果 defined __unix__ || defined __APPLE__ || defined __GLIBC__ \
    || defined __HAIKU__ || defined __EMSCRIPTEN__ \
    || defined __FreeBSD__ || defined __NetBSD__ || defined __OpenBSD__
    #include <unistd.h>
    #include <stdio.h>
    #include <sys/types.h>
    #include <fstream>
    #如果 defined __ANDROID__
        #include <sys/sysconf.h>
        #include <sys/syscall.h>
        #include <sched.h>
    #elif defined __APPLE__
        #include <sys/sysctl.h>
    #endif
#endif

#如果 defined (__QNX__)
    #include <sys/syspage.h>
#endif

#ifndef OPENCV_DISABLE_THREAD_SUPPORT
    #include <thread>
#endif

#ifdef _OPENMP
    #define HAVE_OPENMP
#endif

#ifdef __APPLE__
    #define HAVE_GCD
#endif

#如果 defined _MSC_VER
    #define HAVE_CONCURRENCY
#endif

/* IMPORTANT: always use the same order of defines
   - HAVE_TBB         - 3rdparty library, should be explicitly enabled
   - HAVE_HPX         - 3rdparty library, should be explicitly enabled
   - HAVE_OPENMP      - integrated to compiler, should be explicitly enabled
   - HAVE_GCD         - system wide, used automatically        (APPLE only)
   - WINRT            - system wide, used automatically        (Windows RT only)
   - HAVE_CONCURRENCY - part of runtime, used automatically    (Windows only - MSVS 10, MSVS 11)
   - HAVE_PTHREADS_PF - pthreads if available
*/

#如果 defined HAVE_TBB
    #ifndef TBB_SUPPRESS_DEPRECATED_MESSAGES  // supress warning
    #define TBB_SUPPRESS_DEPRECATED_MESSAGES 1
    #endif
    #include "tbb/tbb.h"
    #include "tbb/task.h"
    #如果 TBB_INTERFACE_VERSION >= 8000
        #include "tbb/task_arena.h"
    #endif
    #undef min
    #undef max
#elif defined HAVE_HPX
    #include <hpx/parallel/algorithms/for_loop.hpp>
    #include <hpx/parallel/execution.hpp>
    //
    #include <hpx/hpx_start.hpp>
    #include <hpx/hpx_suspend.hpp>
    #include <hpx/include/apply.hpp>
    #include <hpx/util/yield_while.hpp>
    #include <hpx/include/threadmanager.hpp>

#elif defined HAVE_OPENMP
    #include <omp.h>
#elif defined HAVE_GCD
    #include <dispatch/dispatch.h>
    #include <pthread.h>
#elif defined HAVE_CONCURRENCY
    #include <ppl.h>
#elif defined HAVE_PTHREADS_PF
    #include <pthread.h>
#endif


#如果 defined HAVE_TBB
#  define CV_PARALLEL_FRAMEWORK "tbb"
#elif defined HAVE_HPX
#  define CV_PARALLEL_FRAMEWORK "hpx"
#elif defined HAVE_OPENMP
#  define CV_PARALLEL_FRAMEWORK "openmp"
#elif defined HAVE_GCD
#  define CV_PARALLEL_FRAMEWORK "gcd"
#elif defined WINRT
#  define CV_PARALLEL_FRAMEWORK "winrt-concurrency"
#elif defined HAVE_CONCURRENCY
#  define CV_PARALLEL_FRAMEWORK "ms-concurrency"
#elif defined HAVE_PTHREADS_PF
#  define CV_PARALLEL_FRAMEWORK "pthreads"
#endif

#include <atomic>

#include "parallel_impl.hpp"

#include "opencv2/core/detail/exception_ptr.hpp"  // CV__EXCEPTION_PTR = 1 if std::exception_ptr is available

#include <opencv2/core/utils/fp_control_utils.hpp>
#include <opencv2/core/utils/fp_control.private.hpp>

using namespace cv;

namespace cv {

ParallelLoopBody::~ParallelLoopBody() {}

using namespace cv::parallel;

namespace {

#ifdef ENABLE_INSTRUMENTATION
    static void SyncNodes(cv::instr::InstrNode *pNode)
    {
        std::vector<cv::instr::NodeDataTls*> data;
        pNode->m_payload.m_tls.gather(data);

        uint64 ticksMax = 0;
        整数    threads  = 0;
        为(size_t i = 0; i < data.size(); i++)
        {
            如果(data[i] && data[i]->m_ticksTotal)
            {
                ticksMax = MAX(ticksMax, data[i]->m_ticksTotal);
                pNode->m_payload.m_ticksTotal -= data[i]->m_ticksTotal;
                data[i]->m_ticksTotal = 0;
                threads++;
            }
        }
        pNode->m_payload.m_ticksTotal += ticksMax;
        pNode->m_payload.m_threads = MAX(pNode->m_payload.m_threads, threads);

        为(size_t i = 0; i < pNode->m_childs.size(); i++)
            SyncNodes(pNode->m_childs[i]);
    }
#endif

    class ParallelLoopBodyWrapperContext
    {
    public:
        ParallelLoopBodyWrapperContext(const cv::ParallelLoopBody& _body, const cv::Range& _r, double _nstripes) :
            is_rng_used(false), hasException(false)
        {

            body = &_body;
            wholeRange = _r;
            double len = wholeRange.end - wholeRange.start;
            nstripes = cvRound(_nstripes <= 0 ? len : MIN(MAX(_nstripes, 1.), len));

            // propagate main thread state
            rng = cv::theRNG();
#如果 OPENCV_SUPPORTS_FP_DENORMALS_HINT && OPENCV_IMPL_FP_HINTS
            details::saveFPDenormalsState(fp_denormals_base_state);
#endif

#ifdef OPENCV_TRACE
            traceRootRegion = CV_TRACE_NS::details::getCurrentRegion();
            traceRootContext = CV_TRACE_NS::details::getTraceManager().tls.get();
#endif

#ifdef ENABLE_INSTRUMENTATION
            pThreadRoot = cv::instr::getInstrumentTLSStruct().pCurrentNode;
#endif
        }
        void finalize()
        {
#ifdef ENABLE_INSTRUMENTATION
            为(size_t i = 0; i < pThreadRoot->m_childs.size(); i++)
                SyncNodes(pThreadRoot->m_childs[i]);
#endif
            如果 (is_rng_used)
            {
                // Some parallel backends execute nested jobs in the main thread,
                // so we need to restore initial RNG state here.
                cv::theRNG() = rng;
                // We can't properly update RNG state based on RNG usage in worker threads,
                // so lets just change main thread RNG state to the next value.
                // Note: this behaviour is not equal to single-threaded mode.
                cv::theRNG().next();
            }
#ifdef OPENCV_TRACE
            如果 (traceRootRegion)
                CV_TRACE_NS::details::parallelForFinalize(*traceRootRegion);
#endif

            如果 (hasException)
            {
#如果 CV__EXCEPTION_PTR
                std::rethrow_exception(pException);
#否则
                CV_Error(Error::StsError, "Exception in parallel_for() body: " + exception_message);
#endif
            }
        }
        ~ParallelLoopBodyWrapperContext() {}

        const cv::ParallelLoopBody* body;
        cv::Range wholeRange;
        整数 nstripes;
        cv::RNG rng;
        mutable bool is_rng_used;
#ifdef OPENCV_TRACE
        CV_TRACE_NS::details::Region* traceRootRegion;
        CV_TRACE_NS::details::TraceManagerThreadLocal* traceRootContext;
#endif
#ifdef ENABLE_INSTRUMENTATION
        cv::instr::InstrNode *pThreadRoot;
#endif
        bool hasException;
#如果 CV__EXCEPTION_PTR
        std::exception_ptr pException;
#否则
        cv::String exception_message;
#endif
#如果 CV__EXCEPTION_PTR
        void recordException()
#否则
        void recordException(const cv::String& msg)
#endif
        {
#ifndef CV_THREAD_SANITIZER
            如果 (!hasException)
#endif
            {
                cv::AutoLock lock(cv::getInitializationMutex());
                如果 (!hasException)
                {
                    hasException = true;
#如果 CV__EXCEPTION_PTR
                    pException = std::current_exception();
#否则
                    exception_message = msg;
#endif
                }
            }
        }

#如果 OPENCV_SUPPORTS_FP_DENORMALS_HINT && OPENCV_IMPL_FP_HINTS
        details::FPDenormalsModeState fp_denormals_base_state;
#endif

    private:
        ParallelLoopBodyWrapperContext(const ParallelLoopBodyWrapperContext&); // disabled
        ParallelLoopBodyWrapperContext& operator=(const ParallelLoopBodyWrapperContext&); // disabled
    };

    class ParallelLoopBodyWrapper : public cv::ParallelLoopBody
    {
    public:
        ParallelLoopBodyWrapper(ParallelLoopBodyWrapperContext& ctx_) :
            ctx(ctx_)
        {
        }
        ~ParallelLoopBodyWrapper()
        {
        }
        void operator()(const cv::Range& sr) const CV_OVERRIDE
        {
#ifdef OPENCV_TRACE
            // TODO CV_TRACE_NS::details::setCurrentRegion(rootRegion);
            如果 (ctx.traceRootRegion && ctx.traceRootContext)
                CV_TRACE_NS::details::parallelForSetRootRegion(*ctx.traceRootRegion, *ctx.traceRootContext);
            CV__TRACE_OPENCV_FUNCTION_NAME("parallel_for_body");
            如果 (ctx.traceRootRegion)
                CV_TRACE_NS::details::parallelForAttachNestedRegion(*ctx.traceRootRegion);
#endif

#ifdef ENABLE_INSTRUMENTATION
            {
                cv::instr::InstrTLSStruct *pInstrTLS = &cv::instr::getInstrumentTLSStruct();
                pInstrTLS->pCurrentNode = ctx.pThreadRoot; // Initialize TLS node for thread
            }
            CV_INSTRUMENT_REGION();
#endif

            // propagate main thread state
            cv::theRNG() = ctx.rng;
#如果 OPENCV_SUPPORTS_FP_DENORMALS_HINT && OPENCV_IMPL_FP_HINTS
            FPDenormalsIgnoreHintScope fp_denormals_scope(ctx.fp_denormals_base_state);
#endif

            cv::Range r;
            cv::Range wholeRange = ctx.wholeRange;
            整数 nstripes = ctx.nstripes;
            r.start = (整数)(wholeRange.start +
                            ((uint64)sr.start*(wholeRange.end - wholeRange.start) + nstripes/2)/nstripes);
            r.end = sr.end >= nstripes ? wholeRange.end : (整数)(wholeRange.start +
                            ((uint64)sr.end*(wholeRange.end - wholeRange.start) + nstripes/2)/nstripes);

#ifdef OPENCV_TRACE
            CV_TRACE_ARG_VALUE(range_start, "range.start", (int64)r.start);
            CV_TRACE_ARG_VALUE(range_end, "range.end", (int64)r.end);
#endif

            try
            {
                (*ctx.body)(r);
            }
#如果 CV__EXCEPTION_PTR
            catch (...)
            {
                ctx.recordException();
            }
#否则
            catch (const cv::Exception& e)
            {
                ctx.recordException(e.what());
            }
            catch (const std::exception& e)
            {
                ctx.recordException(e.what());
            }
            catch (...)
            {
                ctx.recordException("Unknown exception");
            }
#endif

            如果 (!ctx.is_rng_used && !(cv::theRNG() == ctx.rng))
                ctx.is_rng_used = true;
        }
        cv::Range stripeRange() const { 返回 cv::Range(0, ctx.nstripes); }

    protected:
        ParallelLoopBodyWrapperContext& ctx;
    };

#如果 defined HAVE_TBB
    class ProxyLoopBody : public ParallelLoopBodyWrapper
    {
    public:
        ProxyLoopBody(ParallelLoopBodyWrapperContext& ctx_)
        : ParallelLoopBodyWrapper(ctx_)
        {}

        void operator ()(const tbb::blocked_range<整数>& range) const
        {
            this->ParallelLoopBodyWrapper::operator()(cv::Range(range.begin(), range.end()));
        }

        void operator ()() const  // run parallel job
        {
            cv::Range range = this->stripeRange();
            tbb::parallel_for(tbb::blocked_range<整数>(range.start, range.end), *this);
        }
    };
#elif defined HAVE_HPX
    class ProxyLoopBody : public ParallelLoopBodyWrapper
    {
    public:
        ProxyLoopBody(ParallelLoopBodyWrapperContext& ctx_)
                : ParallelLoopBodyWrapper(ctx_)
        {}

        void operator ()() const  // run parallel job
        {
            cv::Range stripeRange = this->stripeRange();
            hpx::parallel::for_loop(
                    hpx::parallel::execution::par,
                    stripeRange.start, stripeRange.end,
                    [&](const 整数 &i) { ;
                        this->ParallelLoopBodyWrapper::operator()(
                                cv::Range(i, i + 1));
                    });
        }
    };
#elif defined HAVE_OPENMP
    typedef ParallelLoopBodyWrapper ProxyLoopBody;
#elif defined HAVE_GCD
    typedef ParallelLoopBodyWrapper ProxyLoopBody;
    static void block_function(void* context, size_t index)
    {
        ProxyLoopBody* ptr_body = static_cast<ProxyLoopBody*>(context);
        (*ptr_body)(cv::Range((整数)index, (整数)index + 1));
    }
#elif defined WINRT || defined HAVE_CONCURRENCY
    class ProxyLoopBody : public ParallelLoopBodyWrapper
    {
    public:
        ProxyLoopBody(ParallelLoopBodyWrapperContext& ctx)
        : ParallelLoopBodyWrapper(ctx)
        {}

        void operator ()(整数 i) const
        {
            this->ParallelLoopBodyWrapper::operator()(cv::Range(i, i + 1));
        }
    };

    // PPL's default auto_partitioner silently skips whole chunks of the range on
    // ARM64 (unfixed MSVC bug, developercommunity.visualstudio.com/t/1027444).
    template <typename Body>
    static inline void pplParallelFor(整数 first, 整数 last, const Body& body)
    {
#如果 defined(_M_ARM64) || defined(_M_ARM64EC)
        Concurrency::parallel_for(first, last, body, Concurrency::static_partitioner());
#否则
        Concurrency::parallel_for(first, last, body);
#endif
    }
#否则
    typedef ParallelLoopBodyWrapper ProxyLoopBody;
#endif

#如果 defined HAVE_TBB
    #如果 TBB_INTERFACE_VERSION >= 8000
        static tbb::task_arena tbbArena(tbb::task_arena::automatic);
    #否则
        static tbb::task_scheduler_init tbbScheduler(tbb::task_scheduler_init::deferred);
    #endif
#elif defined HAVE_HPX
// nothing for HPX
#elif defined HAVE_OPENMP
static inline 整数 _initMaxThreads()
{
    整数 maxThreads = omp_get_max_threads();
    返回 maxThreads;
}
static 整数 numThreadsMax = _initMaxThreads();
#elif defined HAVE_GCD
// nothing for GCD
#elif defined WINRT
// nothing for WINRT
#elif defined HAVE_CONCURRENCY

class SchedPtr
{
    Concurrency::Scheduler* sched_;
public:
    Concurrency::Scheduler* operator->() { 返回 sched_; }
    operator Concurrency::Scheduler*() { 返回 sched_; }

    void operator=(Concurrency::Scheduler* sched)
    {
        如果 (sched_) sched_->Release();
        sched_ = sched;
    }

    SchedPtr() : sched_(0) {}
    ~SchedPtr() {}
};
static SchedPtr pplScheduler;

#endif

} // namespace anon

/* ================================   parallel_for_  ================================ */

static void parallel_for_impl(const cv::Range& range, const cv::ParallelLoopBody& body, double nstripes); // forward declaration

void parallel_for_(const cv::Range& range, const cv::ParallelLoopBody& body, double nstripes)
{
#ifdef OPENCV_TRACE
    CV__TRACE_OPENCV_FUNCTION_NAME_("parallel_for", 0);
    CV_TRACE_ARG_VALUE(range_start, "range.start", (int64)range.start);
    CV_TRACE_ARG_VALUE(range_end, "range.end", (int64)range.end);
    CV_TRACE_ARG_VALUE(nstripes, "nstripes", (int64)nstripes);
#endif

    CV_INSTRUMENT_REGION_MT_FORK();
    如果 (range.empty())
        返回;

    static std::atomic<bool> flagNestedParallelFor(false);
    bool isNotNestedRegion = !flagNestedParallelFor.load();
    如果 (isNotNestedRegion)
      isNotNestedRegion = !flagNestedParallelFor.exchange(true);
    如果 (isNotNestedRegion)
    {
        try
        {
            parallel_for_impl(range, body, nstripes);
            flagNestedParallelFor = false;
        }
        catch (...)
        {
            flagNestedParallelFor = false;
            throw;
        }
    }
    否则 // nested parallel_for_() calls are not parallelized
    {
        CV_UNUSED(nstripes);
        body(range);
    }
}

static
void parallel_for_cb(整数 start, 整数 end, void* data)
{
    CV_DbgAssert(data);
    const cv::ParallelLoopBody& body = *(const cv::ParallelLoopBody*)data;
    body(Range(start, end));
}

static void parallel_for_impl(const cv::Range& range, const cv::ParallelLoopBody& body, double nstripes)
{
    using namespace cv::parallel;
    如果 ((numThreads < 0 || numThreads > 1) && range.end - range.start > 1)
    {
        ParallelLoopBodyWrapperContext ctx(body, range, nstripes);
        ProxyLoopBody pbody(ctx);
        cv::Range stripeRange = pbody.stripeRange();
        如果( stripeRange.end - stripeRange.start == 1 )
        {
            body(range);
            返回;
        }

        std::shared_ptr<ParallelForAPI>& api = getCurrentParallelForAPI();
        如果 (api)
        {
            CV_CheckEQ(stripeRange.start, 0, "");
            api->parallel_for(stripeRange.end, parallel_for_cb, (void*)&pbody);
            ctx.finalize();  // propagate exceptions if exists
            返回;
        }

#ifdef CV_PARALLEL_FRAMEWORK
#如果 defined HAVE_TBB

#如果 TBB_INTERFACE_VERSION >= 8000
        tbbArena.execute(pbody);
#否则
        pbody();
#endif

#elif defined HAVE_HPX
        pbody();

#elif defined HAVE_OPENMP

        #pragma omp parallel 为 schedule(dynamic) num_threads(numThreads > 0 ? numThreads : numThreadsMax)
        为 (整数 i = stripeRange.start; i < stripeRange.end; ++i)
            pbody(Range(i, i + 1));

#elif defined HAVE_GCD

        dispatch_queue_t concurrent_queue = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0);
        dispatch_apply_f(stripeRange.end - stripeRange.start, concurrent_queue, &pbody, block_function);

#elif defined WINRT

        pplParallelFor(stripeRange.start, stripeRange.end, pbody);

#elif defined HAVE_CONCURRENCY

        如果(!pplScheduler || pplScheduler->Id() == Concurrency::CurrentScheduler::Id())
        {
            pplParallelFor(stripeRange.start, stripeRange.end, pbody);
        }
        否则
        {
            pplScheduler->Attach();
            pplParallelFor(stripeRange.start, stripeRange.end, pbody);
            Concurrency::CurrentScheduler::Detach();
        }

#elif defined HAVE_PTHREADS_PF

        parallel_for_pthreads(pbody.stripeRange(), pbody, pbody.stripeRange().size());

#否则

#error You have hacked and compiling with unsupported parallel framework

#endif

        ctx.finalize();  // propagate exceptions if exists
        返回;
#endif // CV_PARALLEL_FRAMEWORK
    }

    body(range);
}


整数 getNumThreads(void)
{
    std::shared_ptr<ParallelForAPI>& api = getCurrentParallelForAPI();
    如果 (api)
    {
        返回 api->getNumThreads();
    }

    如果 (numThreads == 0)
        返回 1;

#如果 defined HAVE_TBB

#如果 TBB_INTERFACE_VERSION >= 9100
    返回 tbbArena.max_concurrency();
#elif TBB_INTERFACE_VERSION >= 8000
    返回 numThreads > 0
        ? numThreads
        : tbb::task_scheduler_init::default_num_threads();
#否则
    返回 tbbScheduler.is_active()
           ? numThreads
           : tbb::task_scheduler_init::default_num_threads();
#endif

#elif defined HAVE_HPX
    返回 numThreads;

#elif defined HAVE_OPENMP

    返回 numThreads > 0
           ? numThreads
           : numThreadsMax;


#elif defined HAVE_GCD

    返回 cv::getNumberOfCPUs(); // the GCD thread pool limit

#elif defined WINRT

    返回 0;

#elif defined HAVE_CONCURRENCY

    返回 (pplScheduler == 0)
        ? Concurrency::CurrentScheduler::Get()->GetNumberOfVirtualProcessors()
        : (1 + pplScheduler->GetNumberOfVirtualProcessors());

#elif defined HAVE_PTHREADS_PF

        返回 parallel_pthreads_get_threads_num();

#否则

    返回 1;

#endif
}

unsigned defaultNumberOfThreads()
{
#ifdef __ANDROID__
    // many modern phones/tables have 4-core CPUs. Let's use no more
    // than 2 threads by default not to overheat the devices
    const unsigned 整数 default_number_of_threads = 2;
#否则
    const unsigned 整数 default_number_of_threads = (unsigned 整数)std::max(1, cv::getNumberOfCPUs());
#endif

    unsigned result = default_number_of_threads;

    static 整数 config_num_threads = (整数)utils::getConfigurationParameterSizeT("OPENCV_FOR_THREADS_NUM", 0);

    如果 (config_num_threads)
    {
        result = (unsigned)std::max(1, config_num_threads);
        //do we need upper limit of threads number?
    }
    返回 result;
}

void setNumThreads( 整数 threads_ )
{
    CV_UNUSED(threads_);

    整数 threads = (threads_ < 0) ? defaultNumberOfThreads() : (unsigned)threads_;
    numThreads = threads;

    std::shared_ptr<ParallelForAPI>& api = getCurrentParallelForAPI();
    如果 (api)
    {
        api->setNumThreads(numThreads);
    }

#ifdef HAVE_TBB

#如果 TBB_INTERFACE_VERSION >= 8000
    如果(tbbArena.is_active()) tbbArena.terminate();
    如果(threads > 0) tbbArena.initialize(threads);
#否则
    如果(tbbScheduler.is_active()) tbbScheduler.terminate();
    如果(threads > 0) tbbScheduler.initialize(threads);
#endif

#elif defined HAVE_HPX
    返回; // nothing needed as numThreads is used

#elif defined HAVE_OPENMP

    返回; // nothing needed as num_threads clause is used in #pragma omp parallel for

#elif defined HAVE_GCD

    // unsupported
    // there is only private dispatch_queue_set_width() and only for desktop

#elif defined WINRT

    返回;

#elif defined HAVE_CONCURRENCY

    如果 (threads <= 0)
    {
        pplScheduler = 0;
    }
    否则 如果 (threads == 1)
    {
        // Concurrency always uses >=2 threads, so we just disable it if 1 thread is requested
        numThreads = 0;
    }
    否则 如果 (pplScheduler == 0 || 1 + pplScheduler->GetNumberOfVirtualProcessors() != (unsigned 整数)threads)
    {
        pplScheduler = Concurrency::Scheduler::Create(Concurrency::SchedulerPolicy(2,
                       Concurrency::MinConcurrency, threads-1,
                       Concurrency::MaxConcurrency, threads-1));
    }

#elif defined HAVE_PTHREADS_PF

    parallel_pthreads_set_threads_num(threads);

#endif
}


整数 getThreadNum()
{
    std::shared_ptr<ParallelForAPI>& api = getCurrentParallelForAPI();
    如果 (api)
    {
        返回 api->getThreadNum();
    }

#如果 defined HAVE_TBB
    #如果 TBB_INTERFACE_VERSION >= 9100
        返回 tbb::this_task_arena::current_thread_index();
    #elif TBB_INTERFACE_VERSION >= 8000
        返回 tbb::task_arena::current_thread_index();
    #否则
        返回 0;
    #endif
#elif defined HAVE_HPX
    返回 (整数)(hpx::get_num_worker_threads());
#elif defined HAVE_OPENMP
    返回 omp_get_thread_num();
#elif defined HAVE_GCD
    返回 (整数)(size_t)(void*)pthread_self(); // no zero-based indexing
#elif defined WINRT
    返回 0;
#elif defined HAVE_CONCURRENCY
    返回 std::max(0, (整数)Concurrency::Context::VirtualProcessorId()); // zero for master thread, unique number for others but not necessary 1,2,3,...
#elif defined HAVE_PTHREADS_PF
    返回 (整数)(size_t)(void*)pthread_self(); // no zero-based indexing
#否则
    返回 0;
#endif
}


#如果 defined __linux__ || defined __GLIBC__ || defined __HAIKU__ || defined __ANDROID__
  #define CV_CPU_GROUPS_1
#endif

#如果 defined __linux__ || defined __ANDROID__
  #define CV_HAVE_CGROUPS 1
#endif

#如果 defined CV_CPU_GROUPS_1
static inline
std::string getFileContents(const char *filename)
{
    std::ifstream ifs(filename);
    如果 (!ifs.is_open())
        返回 std::string();

    std::string content( (std::istreambuf_iterator<char>(ifs) ),
                         (std::istreambuf_iterator<char>()    ) );

    如果 (ifs.fail())
        返回 std::string();

    返回 content;
}

static inline
整数 getNumberOfCPUsImpl(const char *filename)
{
   std::string file_contents = getFileContents(filename);
   如果(file_contents.empty())
       返回 0;

   char *pbuf = const_cast<char*>(file_contents.c_str());
   //parse string of form "0-1,3,5-7,10,13-15"
   整数 cpusAvailable = 0;

   当(*pbuf)
   {
      const char* pos = pbuf;
      bool range = false;
      当(*pbuf && *pbuf != ',')
      {
          如果(*pbuf == '-') range = true;
          ++pbuf;
      }
      如果(*pbuf) *pbuf++ = 0;
      如果(!range)
        ++cpusAvailable;
      否则
      {
          整数 rstart = 0, rend = 0;
          sscanf(pos, "%d-%d", &rstart, &rend);
          cpusAvailable += rend - rstart + 1;
      }

   }
   返回 cpusAvailable;
}
#endif

#如果 defined CV_HAVE_CGROUPS
static inline
unsigned getNumberOfCPUsCFSv2()
{
    整数 cfs_quota = 0;
    整数 cfs_period = 0;

    std::ifstream ss_cpu_max("/sys/fs/cgroup/cpu.max", std::ios::in | std::ios::binary);
    ss_cpu_max >> cfs_quota >> cfs_period;

    如果 (ss_cpu_max.fail() || cfs_quota < 1 || cfs_period < 1) /* values must not be 0 or negative */
        返回 0;

    返回 (unsigned)max(1, cfs_quota/cfs_period);
}

static inline
unsigned getNumberOfCPUsCFSv1()
{
    整数 cfs_quota = 0;
    {
        std::ifstream ss_period("/sys/fs/cgroup/cpu/cpu.cfs_quota_us", std::ios::in | std::ios::binary);
        ss_period >> cfs_quota;

        如果 (ss_period.fail() || cfs_quota < 1) /* cfs_quota must not be 0 or negative */
            返回 0;
    }

    整数 cfs_period = 0;
    {
        std::ifstream ss_quota("/sys/fs/cgroup/cpu/cpu.cfs_period_us", std::ios::in | std::ios::binary);
        ss_quota >> cfs_period;

        如果 (ss_quota.fail() || cfs_period < 1)
            返回 0;
    }

    返回 (unsigned)max(1, cfs_quota/cfs_period);
}
#endif

template <typename T> static inline
T minNonZero(const T& val_1, const T& val_2)
{
    如果 ((val_1 != 0) && (val_2 != 0))
        返回 std::min(val_1, val_2);
    返回 (val_1 != 0) ? val_1 : val_2;
}

#ifndef OPENCV_DISABLE_THREAD_SUPPORT
static
整数 getNumberOfCPUs_()
{
#ifndef OPENCV_SEMIHOSTING
    /*
     * Logic here is to try different methods of getting CPU counts and return
     * the minimum most value as it has high probablity of being right and safe.
     * Return 1 if we get 0 or not found on all methods.
    */
#如果 !defined(__MINGW32__) /* not implemented (2020-03) */

    /*
     * Check for this standard C++11 way, we do not return directly because
     * running in a docker or K8s environment will mean this is the host
     * machines config not the containers or pods and as per docs this value
     * must be "considered only a hint".
    */
    unsigned ncpus = std::thread::hardware_concurrency(); /* If the value is not well defined or not computable, returns 0 */
#否则
    unsigned ncpus = 0; /* 0 means we have to find out some other way */
#endif

#如果 defined _WIN32

    SYSTEM_INFO sysinfo = {};
#如果 (defined(_M_ARM) || defined(_M_ARM64) || defined(_M_ARM64EC) || defined(_M_X64) || defined(WINRT)) && _WIN32_WINNT >= 0x501
    GetNativeSystemInfo( &sysinfo );
#否则
    GetSystemInfo( &sysinfo );
#endif
    unsigned ncpus_sysinfo = sysinfo.dwNumberOfProcessors;
    ncpus = minNonZero(ncpus, ncpus_sysinfo);

#elif defined __APPLE__

    整数 numCPU=0;
    整数 mib[4];
    size_t len = sizeof(numCPU);

    /* set the mib for hw.ncpu */
    mib[0] = CTL_HW;
    mib[1] = HW_AVAILCPU;  // alternatively, try HW_NCPU;

    /* get the number of CPUs from the system */
    sysctl(mib, 2, &numCPU, &len, NULL, 0);

    如果( numCPU < 1 )
    {
        mib[1] = HW_NCPU;
        sysctl( mib, 2, &numCPU, &len, NULL, 0 );

        如果( numCPU < 1 )
            numCPU = 1;
    }

    ncpus = minNonZero(ncpus, (unsigned)numCPU);

#elif defined CV_CPU_GROUPS_1

#如果 defined CV_HAVE_CGROUPS
    static unsigned ncpus_impl_cpuset = (unsigned)getNumberOfCPUsImpl("/sys/fs/cgroup/cpuset/cpuset.cpus");
    ncpus = minNonZero(ncpus, ncpus_impl_cpuset);

    static unsigned ncpus_impl_cfs_v1 = getNumberOfCPUsCFSv1();
    ncpus = minNonZero(ncpus, ncpus_impl_cfs_v1);

    static unsigned ncpus_impl_cfs_v2 = getNumberOfCPUsCFSv2();
    ncpus = minNonZero(ncpus, ncpus_impl_cfs_v2);
#endif

    static unsigned ncpus_impl_devices = (unsigned)getNumberOfCPUsImpl("/sys/devices/system/cpu/online");
    ncpus = minNonZero(ncpus, ncpus_impl_devices);

#endif

#如果 defined _GNU_SOURCE \
    && !defined(__MINGW32__) /* not implemented (2020-03) */ \
    && !defined(__EMSCRIPTEN__) \
    && !defined(__ANDROID__)  // TODO: add check for modern Android NDK

    cpu_set_t cpu_set;
    如果 (0 == sched_getaffinity(0, sizeof(cpu_set), &cpu_set))
    {
        unsigned cpu_count_cpu_set = CPU_COUNT(&cpu_set);
        ncpus = minNonZero(ncpus, cpu_count_cpu_set);
    }

#endif

#如果 !defined(_WIN32) && !defined(__APPLE__) && defined(_SC_NPROCESSORS_ONLN)

    static unsigned cpu_count_sysconf = (unsigned)sysconf( _SC_NPROCESSORS_ONLN );
    ncpus = minNonZero(ncpus, cpu_count_sysconf);
#elif defined (__QNX__)
    static unsigned cpu_count_sysconf = _syspage_ptr->num_cpu;
    ncpus = minNonZero(ncpus, cpu_count_sysconf);
#endif

    返回 ncpus != 0 ? ncpus : 1;
#否则 //  OPENCV_SEMIHOSTING
    返回 1;
#endif //OPENCV_SEMIHOSTING
}

整数 getNumberOfCPUs()
{
    static 整数 nCPUs = getNumberOfCPUs_();
    返回 nCPUs;  // cached value
}

#否则  // OPENCV_DISABLE_THREAD_SUPPORT
整数 getNumberOfCPUs()
{
    返回 1;
}
#endif  // OPENCV_DISABLE_THREAD_SUPPORT

const char* currentParallelFramework()
{
    std::shared_ptr<ParallelForAPI>& api = getCurrentParallelForAPI();
    如果 (api)
    {
        返回 api->getName();
    }
#ifdef CV_PARALLEL_FRAMEWORK
    返回 CV_PARALLEL_FRAMEWORK;
#否则
    返回 NULL;
#endif
}

}  // namespace cv::
