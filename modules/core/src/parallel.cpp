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

#만약 defined _WIN32 || defined WINCE
    #include <windows.h>
    #undef small
    #undef min
    #undef max
    #undef abs
#endif

#만약 defined __unix__ || defined __APPLE__ || defined __GLIBC__ \
    || defined __HAIKU__ || defined __EMSCRIPTEN__ \
    || defined __FreeBSD__ || defined __NetBSD__ || defined __OpenBSD__
    #include <unistd.h>
    #include <stdio.h>
    #include <sys/types.h>
    #include <fstream>
    #만약 defined __ANDROID__
        #include <sys/sysconf.h>
        #include <sys/syscall.h>
        #include <sched.h>
    #elif defined __APPLE__
        #include <sys/sysctl.h>
    #endif
#endif

#만약 defined (__QNX__)
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

#만약 defined _MSC_VER
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

#만약 defined HAVE_TBB
    #ifndef TBB_SUPPRESS_DEPRECATED_MESSAGES  // supress warning
    #define TBB_SUPPRESS_DEPRECATED_MESSAGES 1
    #endif
    #include "tbb/tbb.h"
    #include "tbb/task.h"
    #만약 TBB_INTERFACE_VERSION >= 8000
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


#만약 defined HAVE_TBB
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
        정수    threads  = 0;
        위해(size_t i = 0; i < data.size(); i++)
        {
            만약(data[i] && data[i]->m_ticksTotal)
            {
                ticksMax = MAX(ticksMax, data[i]->m_ticksTotal);
                pNode->m_payload.m_ticksTotal -= data[i]->m_ticksTotal;
                data[i]->m_ticksTotal = 0;
                threads++;
            }
        }
        pNode->m_payload.m_ticksTotal += ticksMax;
        pNode->m_payload.m_threads = MAX(pNode->m_payload.m_threads, threads);

        위해(size_t i = 0; i < pNode->m_childs.size(); i++)
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
#만약 OPENCV_SUPPORTS_FP_DENORMALS_HINT && OPENCV_IMPL_FP_HINTS
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
            위해(size_t i = 0; i < pThreadRoot->m_childs.size(); i++)
                SyncNodes(pThreadRoot->m_childs[i]);
#endif
            만약 (is_rng_used)
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
            만약 (traceRootRegion)
                CV_TRACE_NS::details::parallelForFinalize(*traceRootRegion);
#endif

            만약 (hasException)
            {
#만약 CV__EXCEPTION_PTR
                std::rethrow_exception(pException);
#아니면
                CV_Error(Error::StsError, "Exception in parallel_for() body: " + exception_message);
#endif
            }
        }
        ~ParallelLoopBodyWrapperContext() {}

        const cv::ParallelLoopBody* body;
        cv::Range wholeRange;
        정수 nstripes;
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
#만약 CV__EXCEPTION_PTR
        std::exception_ptr pException;
#아니면
        cv::String exception_message;
#endif
#만약 CV__EXCEPTION_PTR
        void recordException()
#아니면
        void recordException(const cv::String& msg)
#endif
        {
#ifndef CV_THREAD_SANITIZER
            만약 (!hasException)
#endif
            {
                cv::AutoLock lock(cv::getInitializationMutex());
                만약 (!hasException)
                {
                    hasException = true;
#만약 CV__EXCEPTION_PTR
                    pException = std::current_exception();
#아니면
                    exception_message = msg;
#endif
                }
            }
        }

#만약 OPENCV_SUPPORTS_FP_DENORMALS_HINT && OPENCV_IMPL_FP_HINTS
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
            만약 (ctx.traceRootRegion && ctx.traceRootContext)
                CV_TRACE_NS::details::parallelForSetRootRegion(*ctx.traceRootRegion, *ctx.traceRootContext);
            CV__TRACE_OPENCV_FUNCTION_NAME("parallel_for_body");
            만약 (ctx.traceRootRegion)
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
#만약 OPENCV_SUPPORTS_FP_DENORMALS_HINT && OPENCV_IMPL_FP_HINTS
            FPDenormalsIgnoreHintScope fp_denormals_scope(ctx.fp_denormals_base_state);
#endif

            cv::Range r;
            cv::Range wholeRange = ctx.wholeRange;
            정수 nstripes = ctx.nstripes;
            r.start = (정수)(wholeRange.start +
                            ((uint64)sr.start*(wholeRange.end - wholeRange.start) + nstripes/2)/nstripes);
            r.end = sr.end >= nstripes ? wholeRange.end : (정수)(wholeRange.start +
                            ((uint64)sr.end*(wholeRange.end - wholeRange.start) + nstripes/2)/nstripes);

#ifdef OPENCV_TRACE
            CV_TRACE_ARG_VALUE(range_start, "range.start", (int64)r.start);
            CV_TRACE_ARG_VALUE(range_end, "range.end", (int64)r.end);
#endif

            try
            {
                (*ctx.body)(r);
            }
#만약 CV__EXCEPTION_PTR
            catch (...)
            {
                ctx.recordException();
            }
#아니면
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

            만약 (!ctx.is_rng_used && !(cv::theRNG() == ctx.rng))
                ctx.is_rng_used = true;
        }
        cv::Range stripeRange() const { 반환 cv::Range(0, ctx.nstripes); }

    protected:
        ParallelLoopBodyWrapperContext& ctx;
    };

#만약 defined HAVE_TBB
    class ProxyLoopBody : public ParallelLoopBodyWrapper
    {
    public:
        ProxyLoopBody(ParallelLoopBodyWrapperContext& ctx_)
        : ParallelLoopBodyWrapper(ctx_)
        {}

        void operator ()(const tbb::blocked_range<정수>& range) const
        {
            this->ParallelLoopBodyWrapper::operator()(cv::Range(range.begin(), range.end()));
        }

        void operator ()() const  // run parallel job
        {
            cv::Range range = this->stripeRange();
            tbb::parallel_for(tbb::blocked_range<정수>(range.start, range.end), *this);
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
                    [&](const 정수 &i) { ;
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
        (*ptr_body)(cv::Range((정수)index, (정수)index + 1));
    }
#elif defined WINRT || defined HAVE_CONCURRENCY
    class ProxyLoopBody : public ParallelLoopBodyWrapper
    {
    public:
        ProxyLoopBody(ParallelLoopBodyWrapperContext& ctx)
        : ParallelLoopBodyWrapper(ctx)
        {}

        void operator ()(정수 i) const
        {
            this->ParallelLoopBodyWrapper::operator()(cv::Range(i, i + 1));
        }
    };

    // PPL's default auto_partitioner silently skips whole chunks of the range on
    // ARM64 (unfixed MSVC bug, developercommunity.visualstudio.com/t/1027444).
    template <typename Body>
    static inline void pplParallelFor(정수 first, 정수 last, const Body& body)
    {
#만약 defined(_M_ARM64) || defined(_M_ARM64EC)
        Concurrency::parallel_for(first, last, body, Concurrency::static_partitioner());
#아니면
        Concurrency::parallel_for(first, last, body);
#endif
    }
#아니면
    typedef ParallelLoopBodyWrapper ProxyLoopBody;
#endif

#만약 defined HAVE_TBB
    #만약 TBB_INTERFACE_VERSION >= 8000
        static tbb::task_arena tbbArena(tbb::task_arena::automatic);
    #아니면
        static tbb::task_scheduler_init tbbScheduler(tbb::task_scheduler_init::deferred);
    #endif
#elif defined HAVE_HPX
// nothing for HPX
#elif defined HAVE_OPENMP
static inline 정수 _initMaxThreads()
{
    정수 maxThreads = omp_get_max_threads();
    반환 maxThreads;
}
static 정수 numThreadsMax = _initMaxThreads();
#elif defined HAVE_GCD
// nothing for GCD
#elif defined WINRT
// nothing for WINRT
#elif defined HAVE_CONCURRENCY

class SchedPtr
{
    Concurrency::Scheduler* sched_;
public:
    Concurrency::Scheduler* operator->() { 반환 sched_; }
    operator Concurrency::Scheduler*() { 반환 sched_; }

    void operator=(Concurrency::Scheduler* sched)
    {
        만약 (sched_) sched_->Release();
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
    만약 (range.empty())
        반환;

    static std::atomic<bool> flagNestedParallelFor(false);
    bool isNotNestedRegion = !flagNestedParallelFor.load();
    만약 (isNotNestedRegion)
      isNotNestedRegion = !flagNestedParallelFor.exchange(true);
    만약 (isNotNestedRegion)
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
    아니면 // nested parallel_for_() calls are not parallelized
    {
        CV_UNUSED(nstripes);
        body(range);
    }
}

static
void parallel_for_cb(정수 start, 정수 end, void* data)
{
    CV_DbgAssert(data);
    const cv::ParallelLoopBody& body = *(const cv::ParallelLoopBody*)data;
    body(Range(start, end));
}

static void parallel_for_impl(const cv::Range& range, const cv::ParallelLoopBody& body, double nstripes)
{
    using namespace cv::parallel;
    만약 ((numThreads < 0 || numThreads > 1) && range.end - range.start > 1)
    {
        ParallelLoopBodyWrapperContext ctx(body, range, nstripes);
        ProxyLoopBody pbody(ctx);
        cv::Range stripeRange = pbody.stripeRange();
        만약( stripeRange.end - stripeRange.start == 1 )
        {
            body(range);
            반환;
        }

        std::shared_ptr<ParallelForAPI>& api = getCurrentParallelForAPI();
        만약 (api)
        {
            CV_CheckEQ(stripeRange.start, 0, "");
            api->parallel_for(stripeRange.end, parallel_for_cb, (void*)&pbody);
            ctx.finalize();  // propagate exceptions if exists
            반환;
        }

#ifdef CV_PARALLEL_FRAMEWORK
#만약 defined HAVE_TBB

#만약 TBB_INTERFACE_VERSION >= 8000
        tbbArena.execute(pbody);
#아니면
        pbody();
#endif

#elif defined HAVE_HPX
        pbody();

#elif defined HAVE_OPENMP

        #pragma omp parallel 위해 schedule(dynamic) num_threads(numThreads > 0 ? numThreads : numThreadsMax)
        위해 (정수 i = stripeRange.start; i < stripeRange.end; ++i)
            pbody(Range(i, i + 1));

#elif defined HAVE_GCD

        dispatch_queue_t concurrent_queue = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0);
        dispatch_apply_f(stripeRange.end - stripeRange.start, concurrent_queue, &pbody, block_function);

#elif defined WINRT

        pplParallelFor(stripeRange.start, stripeRange.end, pbody);

#elif defined HAVE_CONCURRENCY

        만약(!pplScheduler || pplScheduler->Id() == Concurrency::CurrentScheduler::Id())
        {
            pplParallelFor(stripeRange.start, stripeRange.end, pbody);
        }
        아니면
        {
            pplScheduler->Attach();
            pplParallelFor(stripeRange.start, stripeRange.end, pbody);
            Concurrency::CurrentScheduler::Detach();
        }

#elif defined HAVE_PTHREADS_PF

        parallel_for_pthreads(pbody.stripeRange(), pbody, pbody.stripeRange().size());

#아니면

#error You have hacked and compiling with unsupported parallel framework

#endif

        ctx.finalize();  // propagate exceptions if exists
        반환;
#endif // CV_PARALLEL_FRAMEWORK
    }

    body(range);
}


정수 getNumThreads(void)
{
    std::shared_ptr<ParallelForAPI>& api = getCurrentParallelForAPI();
    만약 (api)
    {
        반환 api->getNumThreads();
    }

    만약 (numThreads == 0)
        반환 1;

#만약 defined HAVE_TBB

#만약 TBB_INTERFACE_VERSION >= 9100
    반환 tbbArena.max_concurrency();
#elif TBB_INTERFACE_VERSION >= 8000
    반환 numThreads > 0
        ? numThreads
        : tbb::task_scheduler_init::default_num_threads();
#아니면
    반환 tbbScheduler.is_active()
           ? numThreads
           : tbb::task_scheduler_init::default_num_threads();
#endif

#elif defined HAVE_HPX
    반환 numThreads;

#elif defined HAVE_OPENMP

    반환 numThreads > 0
           ? numThreads
           : numThreadsMax;


#elif defined HAVE_GCD

    반환 cv::getNumberOfCPUs(); // the GCD thread pool limit

#elif defined WINRT

    반환 0;

#elif defined HAVE_CONCURRENCY

    반환 (pplScheduler == 0)
        ? Concurrency::CurrentScheduler::Get()->GetNumberOfVirtualProcessors()
        : (1 + pplScheduler->GetNumberOfVirtualProcessors());

#elif defined HAVE_PTHREADS_PF

        반환 parallel_pthreads_get_threads_num();

#아니면

    반환 1;

#endif
}

unsigned defaultNumberOfThreads()
{
#ifdef __ANDROID__
    // many modern phones/tables have 4-core CPUs. Let's use no more
    // than 2 threads by default not to overheat the devices
    const unsigned 정수 default_number_of_threads = 2;
#아니면
    const unsigned 정수 default_number_of_threads = (unsigned 정수)std::max(1, cv::getNumberOfCPUs());
#endif

    unsigned result = default_number_of_threads;

    static 정수 config_num_threads = (정수)utils::getConfigurationParameterSizeT("OPENCV_FOR_THREADS_NUM", 0);

    만약 (config_num_threads)
    {
        result = (unsigned)std::max(1, config_num_threads);
        //do we need upper limit of threads number?
    }
    반환 result;
}

void setNumThreads( 정수 threads_ )
{
    CV_UNUSED(threads_);

    정수 threads = (threads_ < 0) ? defaultNumberOfThreads() : (unsigned)threads_;
    numThreads = threads;

    std::shared_ptr<ParallelForAPI>& api = getCurrentParallelForAPI();
    만약 (api)
    {
        api->setNumThreads(numThreads);
    }

#ifdef HAVE_TBB

#만약 TBB_INTERFACE_VERSION >= 8000
    만약(tbbArena.is_active()) tbbArena.terminate();
    만약(threads > 0) tbbArena.initialize(threads);
#아니면
    만약(tbbScheduler.is_active()) tbbScheduler.terminate();
    만약(threads > 0) tbbScheduler.initialize(threads);
#endif

#elif defined HAVE_HPX
    반환; // nothing needed as numThreads is used

#elif defined HAVE_OPENMP

    반환; // nothing needed as num_threads clause is used in #pragma omp parallel for

#elif defined HAVE_GCD

    // unsupported
    // there is only private dispatch_queue_set_width() and only for desktop

#elif defined WINRT

    반환;

#elif defined HAVE_CONCURRENCY

    만약 (threads <= 0)
    {
        pplScheduler = 0;
    }
    아니면 만약 (threads == 1)
    {
        // Concurrency always uses >=2 threads, so we just disable it if 1 thread is requested
        numThreads = 0;
    }
    아니면 만약 (pplScheduler == 0 || 1 + pplScheduler->GetNumberOfVirtualProcessors() != (unsigned 정수)threads)
    {
        pplScheduler = Concurrency::Scheduler::Create(Concurrency::SchedulerPolicy(2,
                       Concurrency::MinConcurrency, threads-1,
                       Concurrency::MaxConcurrency, threads-1));
    }

#elif defined HAVE_PTHREADS_PF

    parallel_pthreads_set_threads_num(threads);

#endif
}


정수 getThreadNum()
{
    std::shared_ptr<ParallelForAPI>& api = getCurrentParallelForAPI();
    만약 (api)
    {
        반환 api->getThreadNum();
    }

#만약 defined HAVE_TBB
    #만약 TBB_INTERFACE_VERSION >= 9100
        반환 tbb::this_task_arena::current_thread_index();
    #elif TBB_INTERFACE_VERSION >= 8000
        반환 tbb::task_arena::current_thread_index();
    #아니면
        반환 0;
    #endif
#elif defined HAVE_HPX
    반환 (정수)(hpx::get_num_worker_threads());
#elif defined HAVE_OPENMP
    반환 omp_get_thread_num();
#elif defined HAVE_GCD
    반환 (정수)(size_t)(void*)pthread_self(); // no zero-based indexing
#elif defined WINRT
    반환 0;
#elif defined HAVE_CONCURRENCY
    반환 std::max(0, (정수)Concurrency::Context::VirtualProcessorId()); // zero for master thread, unique number for others but not necessary 1,2,3,...
#elif defined HAVE_PTHREADS_PF
    반환 (정수)(size_t)(void*)pthread_self(); // no zero-based indexing
#아니면
    반환 0;
#endif
}


#만약 defined __linux__ || defined __GLIBC__ || defined __HAIKU__ || defined __ANDROID__
  #define CV_CPU_GROUPS_1
#endif

#만약 defined __linux__ || defined __ANDROID__
  #define CV_HAVE_CGROUPS 1
#endif

#만약 defined CV_CPU_GROUPS_1
static inline
std::string getFileContents(const char *filename)
{
    std::ifstream ifs(filename);
    만약 (!ifs.is_open())
        반환 std::string();

    std::string content( (std::istreambuf_iterator<char>(ifs) ),
                         (std::istreambuf_iterator<char>()    ) );

    만약 (ifs.fail())
        반환 std::string();

    반환 content;
}

static inline
정수 getNumberOfCPUsImpl(const char *filename)
{
   std::string file_contents = getFileContents(filename);
   만약(file_contents.empty())
       반환 0;

   char *pbuf = const_cast<char*>(file_contents.c_str());
   //parse string of form "0-1,3,5-7,10,13-15"
   정수 cpusAvailable = 0;

   동안(*pbuf)
   {
      const char* pos = pbuf;
      bool range = false;
      동안(*pbuf && *pbuf != ',')
      {
          만약(*pbuf == '-') range = true;
          ++pbuf;
      }
      만약(*pbuf) *pbuf++ = 0;
      만약(!range)
        ++cpusAvailable;
      아니면
      {
          정수 rstart = 0, rend = 0;
          sscanf(pos, "%d-%d", &rstart, &rend);
          cpusAvailable += rend - rstart + 1;
      }

   }
   반환 cpusAvailable;
}
#endif

#만약 defined CV_HAVE_CGROUPS
static inline
unsigned getNumberOfCPUsCFSv2()
{
    정수 cfs_quota = 0;
    정수 cfs_period = 0;

    std::ifstream ss_cpu_max("/sys/fs/cgroup/cpu.max", std::ios::in | std::ios::binary);
    ss_cpu_max >> cfs_quota >> cfs_period;

    만약 (ss_cpu_max.fail() || cfs_quota < 1 || cfs_period < 1) /* values must not be 0 or negative */
        반환 0;

    반환 (unsigned)max(1, cfs_quota/cfs_period);
}

static inline
unsigned getNumberOfCPUsCFSv1()
{
    정수 cfs_quota = 0;
    {
        std::ifstream ss_period("/sys/fs/cgroup/cpu/cpu.cfs_quota_us", std::ios::in | std::ios::binary);
        ss_period >> cfs_quota;

        만약 (ss_period.fail() || cfs_quota < 1) /* cfs_quota must not be 0 or negative */
            반환 0;
    }

    정수 cfs_period = 0;
    {
        std::ifstream ss_quota("/sys/fs/cgroup/cpu/cpu.cfs_period_us", std::ios::in | std::ios::binary);
        ss_quota >> cfs_period;

        만약 (ss_quota.fail() || cfs_period < 1)
            반환 0;
    }

    반환 (unsigned)max(1, cfs_quota/cfs_period);
}
#endif

template <typename T> static inline
T minNonZero(const T& val_1, const T& val_2)
{
    만약 ((val_1 != 0) && (val_2 != 0))
        반환 std::min(val_1, val_2);
    반환 (val_1 != 0) ? val_1 : val_2;
}

#ifndef OPENCV_DISABLE_THREAD_SUPPORT
static
정수 getNumberOfCPUs_()
{
#ifndef OPENCV_SEMIHOSTING
    /*
     * Logic here is to try different methods of getting CPU counts and return
     * the minimum most value as it has high probablity of being right and safe.
     * Return 1 if we get 0 or not found on all methods.
    */
#만약 !defined(__MINGW32__) /* not implemented (2020-03) */

    /*
     * Check for this standard C++11 way, we do not return directly because
     * running in a docker or K8s environment will mean this is the host
     * machines config not the containers or pods and as per docs this value
     * must be "considered only a hint".
    */
    unsigned ncpus = std::thread::hardware_concurrency(); /* If the value is not well defined or not computable, returns 0 */
#아니면
    unsigned ncpus = 0; /* 0 means we have to find out some other way */
#endif

#만약 defined _WIN32

    SYSTEM_INFO sysinfo = {};
#만약 (defined(_M_ARM) || defined(_M_ARM64) || defined(_M_ARM64EC) || defined(_M_X64) || defined(WINRT)) && _WIN32_WINNT >= 0x501
    GetNativeSystemInfo( &sysinfo );
#아니면
    GetSystemInfo( &sysinfo );
#endif
    unsigned ncpus_sysinfo = sysinfo.dwNumberOfProcessors;
    ncpus = minNonZero(ncpus, ncpus_sysinfo);

#elif defined __APPLE__

    정수 numCPU=0;
    정수 mib[4];
    size_t len = sizeof(numCPU);

    /* set the mib for hw.ncpu */
    mib[0] = CTL_HW;
    mib[1] = HW_AVAILCPU;  // alternatively, try HW_NCPU;

    /* get the number of CPUs from the system */
    sysctl(mib, 2, &numCPU, &len, NULL, 0);

    만약( numCPU < 1 )
    {
        mib[1] = HW_NCPU;
        sysctl( mib, 2, &numCPU, &len, NULL, 0 );

        만약( numCPU < 1 )
            numCPU = 1;
    }

    ncpus = minNonZero(ncpus, (unsigned)numCPU);

#elif defined CV_CPU_GROUPS_1

#만약 defined CV_HAVE_CGROUPS
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

#만약 defined _GNU_SOURCE \
    && !defined(__MINGW32__) /* not implemented (2020-03) */ \
    && !defined(__EMSCRIPTEN__) \
    && !defined(__ANDROID__)  // TODO: add check for modern Android NDK

    cpu_set_t cpu_set;
    만약 (0 == sched_getaffinity(0, sizeof(cpu_set), &cpu_set))
    {
        unsigned cpu_count_cpu_set = CPU_COUNT(&cpu_set);
        ncpus = minNonZero(ncpus, cpu_count_cpu_set);
    }

#endif

#만약 !defined(_WIN32) && !defined(__APPLE__) && defined(_SC_NPROCESSORS_ONLN)

    static unsigned cpu_count_sysconf = (unsigned)sysconf( _SC_NPROCESSORS_ONLN );
    ncpus = minNonZero(ncpus, cpu_count_sysconf);
#elif defined (__QNX__)
    static unsigned cpu_count_sysconf = _syspage_ptr->num_cpu;
    ncpus = minNonZero(ncpus, cpu_count_sysconf);
#endif

    반환 ncpus != 0 ? ncpus : 1;
#아니면 //  OPENCV_SEMIHOSTING
    반환 1;
#endif //OPENCV_SEMIHOSTING
}

정수 getNumberOfCPUs()
{
    static 정수 nCPUs = getNumberOfCPUs_();
    반환 nCPUs;  // cached value
}

#아니면  // OPENCV_DISABLE_THREAD_SUPPORT
정수 getNumberOfCPUs()
{
    반환 1;
}
#endif  // OPENCV_DISABLE_THREAD_SUPPORT

const char* currentParallelFramework()
{
    std::shared_ptr<ParallelForAPI>& api = getCurrentParallelForAPI();
    만약 (api)
    {
        반환 api->getName();
    }
#ifdef CV_PARALLEL_FRAMEWORK
    반환 CV_PARALLEL_FRAMEWORK;
#아니면
    반환 NULL;
#endif
}

}  // namespace cv::
