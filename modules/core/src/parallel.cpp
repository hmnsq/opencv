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

#அந்தர்பாவ "precomp.hpp"

#அந்தர்பாவ <opencv2/core/utils/configuration.private.hpp>
#அந்தர்பாவ <opencv2/core/utils/trace.private.hpp>

#அந்தர்பாவ "opencv2/core/parallel/parallel_backend.hpp"
#அந்தர்பாவ "parallel/parallel.hpp"

#எனில் defined _WIN32 || defined WINCE
    #அந்தர்பாவ <windows.h>
    #undef small
    #undef min
    #undef max
    #undef abs
#endif

#எனில் defined __unix__ || defined __APPLE__ || defined __GLIBC__ \
    || defined __HAIKU__ || defined __EMSCRIPTEN__ \
    || defined __FreeBSD__ || defined __NetBSD__ || defined __OpenBSD__
    #அந்தர்பாவ <unistd.h>
    #அந்தர்பாவ <stdio.h>
    #அந்தர்பாவ <sys/types.h>
    #அந்தர்பாவ <fstream>
    #எனில் defined __ANDROID__
        #அந்தர்பாவ <sys/sysconf.h>
        #அந்தர்பாவ <sys/syscall.h>
        #அந்தர்பாவ <sched.h>
    #அல்லதெனில் defined __APPLE__
        #அந்தர்பாவ <sys/sysctl.h>
    #endif
#endif

#எனில் defined (__QNX__)
    #அந்தர்பாவ <sys/syspage.h>
#endif

#ifndef OPENCV_DISABLE_THREAD_SUPPORT
    #அந்தர்பாவ <thread>
#endif

#ifdef _OPENMP
    #define HAVE_OPENMP
#endif

#ifdef __APPLE__
    #define HAVE_GCD
#endif

#எனில் defined _MSC_VER
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

#எனில் defined HAVE_TBB
    #ifndef TBB_SUPPRESS_DEPRECATED_MESSAGES  // supress warning
    #define TBB_SUPPRESS_DEPRECATED_MESSAGES 1
    #endif
    #அந்தர்பாவ "tbb/tbb.h"
    #அந்தர்பாவ "tbb/task.h"
    #எனில் TBB_INTERFACE_VERSION >= 8000
        #அந்தர்பாவ "tbb/task_arena.h"
    #endif
    #undef min
    #undef max
#அல்லதெனில் defined HAVE_HPX
    #அந்தர்பாவ <hpx/parallel/algorithms/for_loop.hpp>
    #அந்தர்பாவ <hpx/parallel/execution.hpp>
    //
    #அந்தர்பாவ <hpx/hpx_start.hpp>
    #அந்தர்பாவ <hpx/hpx_suspend.hpp>
    #அந்தர்பாவ <hpx/include/apply.hpp>
    #அந்தர்பாவ <hpx/util/yield_while.hpp>
    #அந்தர்பாவ <hpx/include/threadmanager.hpp>

#அல்லதெனில் defined HAVE_OPENMP
    #அந்தர்பாவ <omp.h>
#அல்லதெனில் defined HAVE_GCD
    #அந்தர்பாவ <dispatch/dispatch.h>
    #அந்தர்பாவ <pthread.h>
#அல்லதெனில் defined HAVE_CONCURRENCY
    #அந்தர்பாவ <ppl.h>
#அல்லதெனில் defined HAVE_PTHREADS_PF
    #அந்தர்பாவ <pthread.h>
#endif


#எனில் defined HAVE_TBB
#  define CV_PARALLEL_FRAMEWORK "tbb"
#அல்லதெனில் defined HAVE_HPX
#  define CV_PARALLEL_FRAMEWORK "hpx"
#அல்லதெனில் defined HAVE_OPENMP
#  define CV_PARALLEL_FRAMEWORK "openmp"
#அல்லதெனில் defined HAVE_GCD
#  define CV_PARALLEL_FRAMEWORK "gcd"
#அல்லதெனில் defined WINRT
#  define CV_PARALLEL_FRAMEWORK "winrt-concurrency"
#அல்லதெனில் defined HAVE_CONCURRENCY
#  define CV_PARALLEL_FRAMEWORK "ms-concurrency"
#அல்லதெனில் defined HAVE_PTHREADS_PF
#  define CV_PARALLEL_FRAMEWORK "pthreads"
#endif

#அந்தர்பாவ <atomic>

#அந்தர்பாவ "parallel_impl.hpp"

#அந்தர்பாவ "opencv2/core/detail/exception_ptr.hpp"  // CV__EXCEPTION_PTR = 1 if std::exception_ptr is available

#அந்தர்பாவ <opencv2/core/utils/fp_control_utils.hpp>
#அந்தர்பாவ <opencv2/core/utils/fp_control.private.hpp>

using நாமாகாஶ cv;

நாமாகாஶ cv {

ParallelLoopBody::~ParallelLoopBody() {}

using நாமாகாஶ cv::parallel;

நாமாகாஶ {

#ifdef ENABLE_INSTRUMENTATION
    நிலையான வெறுமை SyncNodes(cv::instr::Instr_முனை *p_முனை)
    {
        std::vector<cv::instr::முனை_Data_Tls*> data;
        p_முனை->m_payload.m_tls.gather(data);

        uint64 ticksMax = 0;
        முழு    threads  = 0;
        க்கு(size_t i = 0; i < data.size(); i++)
        {
            எனில்(data[i] && data[i]->m_ticksTotal)
            {
                ticksMax = MAX(ticksMax, data[i]->m_ticksTotal);
                p_முனை->m_payload.m_ticksTotal -= data[i]->m_ticksTotal;
                data[i]->m_ticksTotal = 0;
                threads++;
            }
        }
        p_முனை->m_payload.m_ticksTotal += ticksMax;
        p_முனை->m_payload.m_threads = MAX(p_முனை->m_payload.m_threads, threads);

        க்கு(size_t i = 0; i < p_முனை->m_childs.size(); i++)
            SyncNodes(p_முனை->m_childs[i]);
    }
#endif

    வகுப்பு ParallelLoopBodyWrapperContext
    {
    பொது:
        ParallelLoopBodyWrapperContext(மாறிலி cv::ParallelLoopBody& _body, மாறிலி cv::Range& _r, இரட்டை _nstripes) :
            is_rng_used(பொய்), hasException(பொய்)
        {

            body = &_body;
            wholeRange = _r;
            இரட்டை len = wholeRange.end - wholeRange.start;
            nstripes = cvRound(_nstripes <= 0 ? len : MIN(MAX(_nstripes, 1.), len));

            // propagate main thread state
            rng = cv::theRNG();
#எனில் OPENCV_SUPPORTS_FP_DENORMALS_HINT && OPENCV_IMPL_FP_HINTS
            details::saveFPDenormalsState(fp_denormals_base_state);
#endif

#ifdef OPENCV_TRACE
            trace_வேர்_Region = CV_TRACE_NS::details::getCurrentRegion();
            trace_வேர்_Context = CV_TRACE_NS::details::getTraceManager().tls.get();
#endif

#ifdef ENABLE_INSTRUMENTATION
            p_Thread_வேர் = cv::instr::getInstrumentTLSStruct().p_Current_முனை;
#endif
        }
        வெறுமை finalize()
        {
#ifdef ENABLE_INSTRUMENTATION
            க்கு(size_t i = 0; i < p_Thread_வேர்->m_childs.size(); i++)
                SyncNodes(p_Thread_வேர்->m_childs[i]);
#endif
            எனில் (is_rng_used)
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
            எனில் (trace_வேர்_Region)
                CV_TRACE_NS::details::parallelForFinalize(*trace_வேர்_Region);
#endif

            எனில் (hasException)
            {
#எனில் CV__EXCEPTION_PTR
                std::rethrow_exception(pException);
#இல்லையெனில்
                CV_Error(Error::StsError, "Exception in parallel_for() body: " + exception_message);
#endif
            }
        }
        ~ParallelLoopBodyWrapperContext() {}

        மாறிலி cv::ParallelLoopBody* body;
        cv::Range wholeRange;
        முழு nstripes;
        cv::RNG rng;
        mutable தருக்க is_rng_used;
#ifdef OPENCV_TRACE
        CV_TRACE_NS::details::Region* trace_வேர்_Region;
        CV_TRACE_NS::details::TraceManagerThreadLocal* trace_வேர்_Context;
#endif
#ifdef ENABLE_INSTRUMENTATION
        cv::instr::Instr_முனை *p_Thread_வேர்;
#endif
        தருக்க hasException;
#எனில் CV__EXCEPTION_PTR
        std::exception_ptr pException;
#இல்லையெனில்
        cv::String exception_message;
#endif
#எனில் CV__EXCEPTION_PTR
        வெறுமை recordException()
#இல்லையெனில்
        வெறுமை recordException(மாறிலி cv::String& செய்தி)
#endif
        {
#ifndef CV_THREAD_SANITIZER
            எனில் (!hasException)
#endif
            {
                cv::AutoLock lock(cv::getInitializationMutex());
                எனில் (!hasException)
                {
                    hasException = உண்மை;
#எனில் CV__EXCEPTION_PTR
                    pException = std::current_exception();
#இல்லையெனில்
                    exception_message = செய்தி;
#endif
                }
            }
        }

#எனில் OPENCV_SUPPORTS_FP_DENORMALS_HINT && OPENCV_IMPL_FP_HINTS
        details::FPDenormalsModeState fp_denormals_base_state;
#endif

    தனிப்பட்ட:
        ParallelLoopBodyWrapperContext(மாறிலி ParallelLoopBodyWrapperContext&); // disabled
        ParallelLoopBodyWrapperContext& operator=(மாறிலி ParallelLoopBodyWrapperContext&); // disabled
    };

    வகுப்பு ParallelLoopBodyWrapper : பொது cv::ParallelLoopBody
    {
    பொது:
        ParallelLoopBodyWrapper(ParallelLoopBodyWrapperContext& ctx_) :
            ctx(ctx_)
        {
        }
        ~ParallelLoopBodyWrapper()
        {
        }
        வெறுமை operator()(மாறிலி cv::Range& sr) மாறிலி CV_OVERRIDE
        {
#ifdef OPENCV_TRACE
            // TODO CV_TRACE_NS::details::setCurrentRegion(rootRegion);
            எனில் (ctx.trace_வேர்_Region && ctx.trace_வேர்_Context)
                CV_TRACE_NS::details::parallel_For_Set_வேர்_Region(*ctx.trace_வேர்_Region, *ctx.trace_வேர்_Context);
            CV__TRACE_OPENCV_FUNCTION_NAME("parallel_for_body");
            எனில் (ctx.trace_வேர்_Region)
                CV_TRACE_NS::details::parallelForAttachNestedRegion(*ctx.trace_வேர்_Region);
#endif

#ifdef ENABLE_INSTRUMENTATION
            {
                cv::instr::InstrTLSStruct *pInstrTLS = &cv::instr::getInstrumentTLSStruct();
                pInstrTLS->p_Current_முனை = ctx.p_Thread_வேர்; // Initialize TLS node for thread
            }
            CV_INSTRUMENT_REGION();
#endif

            // propagate main thread state
            cv::theRNG() = ctx.rng;
#எனில் OPENCV_SUPPORTS_FP_DENORMALS_HINT && OPENCV_IMPL_FP_HINTS
            FPDenormalsIgnoreHintScope fp_denormals_scope(ctx.fp_denormals_base_state);
#endif

            cv::Range r;
            cv::Range wholeRange = ctx.wholeRange;
            முழு nstripes = ctx.nstripes;
            r.start = (முழு)(wholeRange.start +
                            ((uint64)sr.start*(wholeRange.end - wholeRange.start) + nstripes/2)/nstripes);
            r.end = sr.end >= nstripes ? wholeRange.end : (முழு)(wholeRange.start +
                            ((uint64)sr.end*(wholeRange.end - wholeRange.start) + nstripes/2)/nstripes);

#ifdef OPENCV_TRACE
            CV_TRACE_ARG_மதிப்பு(range_start, "range.start", (int64)r.start);
            CV_TRACE_ARG_மதிப்பு(range_end, "range.end", (int64)r.end);
#endif

            முயல்
            {
                (*ctx.body)(r);
            }
#எனில் CV__EXCEPTION_PTR
            பிடி (...)
            {
                ctx.recordException();
            }
#இல்லையெனில்
            பிடி (மாறிலி cv::Exception& e)
            {
                ctx.recordException(e.what());
            }
            பிடி (மாறிலி std::exception& e)
            {
                ctx.recordException(e.what());
            }
            பிடி (...)
            {
                ctx.recordException("Unknown exception");
            }
#endif

            எனில் (!ctx.is_rng_used && !(cv::theRNG() == ctx.rng))
                ctx.is_rng_used = உண்மை;
        }
        cv::Range stripeRange() மாறிலி { திரும்பு cv::Range(0, ctx.nstripes); }

    பாதுகாக்கப்பட்ட:
        ParallelLoopBodyWrapperContext& ctx;
    };

#எனில் defined HAVE_TBB
    வகுப்பு ProxyLoopBody : பொது ParallelLoopBodyWrapper
    {
    பொது:
        ProxyLoopBody(ParallelLoopBodyWrapperContext& ctx_)
        : ParallelLoopBodyWrapper(ctx_)
        {}

        வெறுமை operator ()(மாறிலி tbb::blocked_range<முழு>& range) மாறிலி
        {
            இது->ParallelLoopBodyWrapper::operator()(cv::Range(range.begin(), range.end()));
        }

        வெறுமை operator ()() மாறிலி  // run parallel job
        {
            cv::Range range = இது->stripeRange();
            tbb::parallel_for(tbb::blocked_range<முழு>(range.start, range.end), *இது);
        }
    };
#அல்லதெனில் defined HAVE_HPX
    வகுப்பு ProxyLoopBody : பொது ParallelLoopBodyWrapper
    {
    பொது:
        ProxyLoopBody(ParallelLoopBodyWrapperContext& ctx_)
                : ParallelLoopBodyWrapper(ctx_)
        {}

        வெறுமை operator ()() மாறிலி  // run parallel job
        {
            cv::Range stripeRange = இது->stripeRange();
            hpx::parallel::for_loop(
                    hpx::parallel::execution::par,
                    stripeRange.start, stripeRange.end,
                    [&](மாறிலி முழு &i) { ;
                        இது->ParallelLoopBodyWrapper::operator()(
                                cv::Range(i, i + 1));
                    });
        }
    };
#அல்லதெனில் defined HAVE_OPENMP
    வகைவரை ParallelLoopBodyWrapper ProxyLoopBody;
#அல்லதெனில் defined HAVE_GCD
    வகைவரை ParallelLoopBodyWrapper ProxyLoopBody;
    நிலையான வெறுமை block_function(வெறுமை* context, size_t index)
    {
        ProxyLoopBody* ptr_body = static_cast<ProxyLoopBody*>(context);
        (*ptr_body)(cv::Range((முழு)index, (முழு)index + 1));
    }
#அல்லதெனில் defined WINRT || defined HAVE_CONCURRENCY
    வகுப்பு ProxyLoopBody : பொது ParallelLoopBodyWrapper
    {
    பொது:
        ProxyLoopBody(ParallelLoopBodyWrapperContext& ctx)
        : ParallelLoopBodyWrapper(ctx)
        {}

        வெறுமை operator ()(முழு i) மாறிலி
        {
            இது->ParallelLoopBodyWrapper::operator()(cv::Range(i, i + 1));
        }
    };

    // PPL's default auto_partitioner silently skips whole chunks of the range on
    // ARM64 (unfixed MSVC bug, developercommunity.visualstudio.com/t/1027444).
    template <typename Body>
    நிலையான உள்வரி வெறுமை pplParallelFor(முழு first, முழு last, மாறிலி Body& body)
    {
#எனில் defined(_M_ARM64) || defined(_M_ARM64EC)
        Concurrency::parallel_for(first, last, body, Concurrency::static_partitioner());
#இல்லையெனில்
        Concurrency::parallel_for(first, last, body);
#endif
    }
#இல்லையெனில்
    வகைவரை ParallelLoopBodyWrapper ProxyLoopBody;
#endif

#எனில் defined HAVE_TBB
    #எனில் TBB_INTERFACE_VERSION >= 8000
        நிலையான tbb::task_arena tbbArena(tbb::task_arena::automatic);
    #இல்லையெனில்
        நிலையான tbb::task_scheduler_init tbbScheduler(tbb::task_scheduler_init::deferred);
    #endif
#அல்லதெனில் defined HAVE_HPX
// nothing for HPX
#அல்லதெனில் defined HAVE_OPENMP
நிலையான உள்வரி முழு _initMaxThreads()
{
    முழு maxThreads = omp_get_max_threads();
    திரும்பு maxThreads;
}
நிலையான முழு இலக்கம்_Threads_Max = _initMaxThreads();
#அல்லதெனில் defined HAVE_GCD
// nothing for GCD
#அல்லதெனில் defined WINRT
// nothing for WINRT
#அல்லதெனில் defined HAVE_CONCURRENCY

வகுப்பு SchedPtr
{
    Concurrency::Scheduler* sched_;
பொது:
    Concurrency::Scheduler* operator->() { திரும்பு sched_; }
    operator Concurrency::Scheduler*() { திரும்பு sched_; }

    வெறுமை operator=(Concurrency::Scheduler* sched)
    {
        எனில் (sched_) sched_->Release();
        sched_ = sched;
    }

    SchedPtr() : sched_(0) {}
    ~SchedPtr() {}
};
நிலையான SchedPtr pplScheduler;

#endif

} // namespace anon

/* ================================   parallel_for_  ================================ */

நிலையான வெறுமை parallel_for_impl(மாறிலி cv::Range& range, மாறிலி cv::ParallelLoopBody& body, இரட்டை nstripes); // forward declaration

வெறுமை parallel_for_(மாறிலி cv::Range& range, மாறிலி cv::ParallelLoopBody& body, இரட்டை nstripes)
{
#ifdef OPENCV_TRACE
    CV__TRACE_OPENCV_FUNCTION_NAME_("parallel_for", 0);
    CV_TRACE_ARG_மதிப்பு(range_start, "range.start", (int64)range.start);
    CV_TRACE_ARG_மதிப்பு(range_end, "range.end", (int64)range.end);
    CV_TRACE_ARG_மதிப்பு(nstripes, "nstripes", (int64)nstripes);
#endif

    CV_INSTRUMENT_REGION_MT_FORK();
    எனில் (range.empty())
        திரும்பு;

    நிலையான std::atomic<தருக்க> flagNestedParallelFor(பொய்);
    தருக்க isNotNestedRegion = !flagNestedParallelFor.load();
    எனில் (isNotNestedRegion)
      isNotNestedRegion = !flagNestedParallelFor.exchange(உண்மை);
    எனில் (isNotNestedRegion)
    {
        முயல்
        {
            parallel_for_impl(range, body, nstripes);
            flagNestedParallelFor = பொய்;
        }
        பிடி (...)
        {
            flagNestedParallelFor = பொய்;
            எறி;
        }
    }
    இல்லையெனில் // nested parallel_for_() calls are not parallelized
    {
        CV_UNUSED(nstripes);
        body(range);
    }
}

நிலையான
வெறுமை parallel_for_cb(முழு start, முழு end, வெறுமை* data)
{
    CV_DbgAssert(data);
    மாறிலி cv::ParallelLoopBody& body = *(மாறிலி cv::ParallelLoopBody*)data;
    body(Range(start, end));
}

நிலையான வெறுமை parallel_for_impl(மாறிலி cv::Range& range, மாறிலி cv::ParallelLoopBody& body, இரட்டை nstripes)
{
    using நாமாகாஶ cv::parallel;
    எனில் ((இலக்கம்_Threads < 0 || இலக்கம்_Threads > 1) && range.end - range.start > 1)
    {
        ParallelLoopBodyWrapperContext ctx(body, range, nstripes);
        ProxyLoopBody pbody(ctx);
        cv::Range stripeRange = pbody.stripeRange();
        எனில்( stripeRange.end - stripeRange.start == 1 )
        {
            body(range);
            திரும்பு;
        }

        std::shared_ptr<ParallelForAPI>& api = getCurrentParallelForAPI();
        எனில் (api)
        {
            CV_CheckEQ(stripeRange.start, 0, "");
            api->parallel_for(stripeRange.end, parallel_for_cb, (வெறுமை*)&pbody);
            ctx.finalize();  // propagate exceptions if exists
            திரும்பு;
        }

#ifdef CV_PARALLEL_FRAMEWORK
#எனில் defined HAVE_TBB

#எனில் TBB_INTERFACE_VERSION >= 8000
        tbbArena.execute(pbody);
#இல்லையெனில்
        pbody();
#endif

#அல்லதெனில் defined HAVE_HPX
        pbody();

#அல்லதெனில் defined HAVE_OPENMP

        #pragma omp parallel க்கு schedule(dynamic) இலக்கம்_threads(இலக்கம்_Threads > 0 ? இலக்கம்_Threads : இலக்கம்_Threads_Max)
        க்கு (முழு i = stripeRange.start; i < stripeRange.end; ++i)
            pbody(Range(i, i + 1));

#அல்லதெனில் defined HAVE_GCD

        dispatch_queue_t concurrent_queue = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0);
        dispatch_apply_f(stripeRange.end - stripeRange.start, concurrent_queue, &pbody, block_function);

#அல்லதெனில் defined WINRT

        pplParallelFor(stripeRange.start, stripeRange.end, pbody);

#அல்லதெனில் defined HAVE_CONCURRENCY

        எனில்(!pplScheduler || pplScheduler->Id() == Concurrency::CurrentScheduler::Id())
        {
            pplParallelFor(stripeRange.start, stripeRange.end, pbody);
        }
        இல்லையெனில்
        {
            pplScheduler->Attach();
            pplParallelFor(stripeRange.start, stripeRange.end, pbody);
            Concurrency::CurrentScheduler::Detach();
        }

#அல்லதெனில் defined HAVE_PTHREADS_PF

        parallel_for_pthreads(pbody.stripeRange(), pbody, pbody.stripeRange().size());

#இல்லையெனில்

#error You have hacked and compiling with unsupported parallel framework

#endif

        ctx.finalize();  // propagate exceptions if exists
        திரும்பு;
#endif // CV_PARALLEL_FRAMEWORK
    }

    body(range);
}


முழு get_இலக்கம்_Threads(வெறுமை)
{
    std::shared_ptr<ParallelForAPI>& api = getCurrentParallelForAPI();
    எனில் (api)
    {
        திரும்பு api->get_இலக்கம்_Threads();
    }

    எனில் (இலக்கம்_Threads == 0)
        திரும்பு 1;

#எனில் defined HAVE_TBB

#எனில் TBB_INTERFACE_VERSION >= 9100
    திரும்பு tbbArena.max_concurrency();
#அல்லதெனில் TBB_INTERFACE_VERSION >= 8000
    திரும்பு இலக்கம்_Threads > 0
        ? இலக்கம்_Threads
        : tbb::task_scheduler_init::default_இலக்கம்_threads();
#இல்லையெனில்
    திரும்பு tbbScheduler.is_active()
           ? இலக்கம்_Threads
           : tbb::task_scheduler_init::default_இலக்கம்_threads();
#endif

#அல்லதெனில் defined HAVE_HPX
    திரும்பு இலக்கம்_Threads;

#அல்லதெனில் defined HAVE_OPENMP

    திரும்பு இலக்கம்_Threads > 0
           ? இலக்கம்_Threads
           : இலக்கம்_Threads_Max;


#அல்லதெனில் defined HAVE_GCD

    திரும்பு cv::get_எண்_Of_CPUs(); // the GCD thread pool limit

#அல்லதெனில் defined WINRT

    திரும்பு 0;

#அல்லதெனில் defined HAVE_CONCURRENCY

    திரும்பு (pplScheduler == 0)
        ? Concurrency::CurrentScheduler::Get()->Get_எண்_Of_Virtual_Processors()
        : (1 + pplScheduler->Get_எண்_Of_Virtual_Processors());

#அல்லதெனில் defined HAVE_PTHREADS_PF

        திரும்பு parallel_pthreads_get_threads_இலக்கம்();

#இல்லையெனில்

    திரும்பு 1;

#endif
}

unsigned default_எண்_Of_Threads()
{
#ifdef __ANDROID__
    // many modern phones/tables have 4-core CPUs. Let's use no more
    // than 2 threads by default not to overheat the devices
    மாறிலி unsigned முழு default_எண்_of_threads = 2;
#இல்லையெனில்
    மாறிலி unsigned முழு default_எண்_of_threads = (unsigned முழு)std::max(1, cv::get_எண்_Of_CPUs());
#endif

    unsigned result = default_எண்_of_threads;

    நிலையான முழு config_இலக்கம்_threads = (முழு)utils::getConfigurationParameterSizeT("OPENCV_FOR_THREADS_NUM", 0);

    எனில் (config_இலக்கம்_threads)
    {
        result = (unsigned)std::max(1, config_இலக்கம்_threads);
        //do we need upper limit of threads number?
    }
    திரும்பு result;
}

வெறுமை set_இலக்கம்_Threads( முழு threads_ )
{
    CV_UNUSED(threads_);

    முழு threads = (threads_ < 0) ? default_எண்_Of_Threads() : (unsigned)threads_;
    இலக்கம்_Threads = threads;

    std::shared_ptr<ParallelForAPI>& api = getCurrentParallelForAPI();
    எனில் (api)
    {
        api->set_இலக்கம்_Threads(இலக்கம்_Threads);
    }

#ifdef HAVE_TBB

#எனில் TBB_INTERFACE_VERSION >= 8000
    எனில்(tbbArena.is_active()) tbbArena.terminate();
    எனில்(threads > 0) tbbArena.initialize(threads);
#இல்லையெனில்
    எனில்(tbbScheduler.is_active()) tbbScheduler.terminate();
    எனில்(threads > 0) tbbScheduler.initialize(threads);
#endif

#அல்லதெனில் defined HAVE_HPX
    திரும்பு; // nothing needed as numThreads is used

#அல்லதெனில் defined HAVE_OPENMP

    திரும்பு; // nothing needed as num_threads clause is used in #pragma omp parallel for

#அல்லதெனில் defined HAVE_GCD

    // unsupported
    // there is only private dispatch_queue_set_width() and only for desktop

#அல்லதெனில் defined WINRT

    திரும்பு;

#அல்லதெனில் defined HAVE_CONCURRENCY

    எனில் (threads <= 0)
    {
        pplScheduler = 0;
    }
    இல்லையெனில் எனில் (threads == 1)
    {
        // Concurrency always uses >=2 threads, so we just disable it if 1 thread is requested
        இலக்கம்_Threads = 0;
    }
    இல்லையெனில் எனில் (pplScheduler == 0 || 1 + pplScheduler->Get_எண்_Of_Virtual_Processors() != (unsigned முழு)threads)
    {
        pplScheduler = Concurrency::Scheduler::Create(Concurrency::SchedulerPolicy(2,
                       Concurrency::MinConcurrency, threads-1,
                       Concurrency::MaxConcurrency, threads-1));
    }

#அல்லதெனில் defined HAVE_PTHREADS_PF

    parallel_pthreads_set_threads_இலக்கம்(threads);

#endif
}


முழு get_Thread_இலக்கம்()
{
    std::shared_ptr<ParallelForAPI>& api = getCurrentParallelForAPI();
    எனில் (api)
    {
        திரும்பு api->get_Thread_இலக்கம்();
    }

#எனில் defined HAVE_TBB
    #எனில் TBB_INTERFACE_VERSION >= 9100
        திரும்பு tbb::this_task_arena::current_thread_index();
    #அல்லதெனில் TBB_INTERFACE_VERSION >= 8000
        திரும்பு tbb::task_arena::current_thread_index();
    #இல்லையெனில்
        திரும்பு 0;
    #endif
#அல்லதெனில் defined HAVE_HPX
    திரும்பு (முழு)(hpx::get_இலக்கம்_worker_threads());
#அல்லதெனில் defined HAVE_OPENMP
    திரும்பு omp_get_thread_இலக்கம்();
#அல்லதெனில் defined HAVE_GCD
    திரும்பு (முழு)(size_t)(வெறுமை*)pthread_self(); // no zero-based indexing
#அல்லதெனில் defined WINRT
    திரும்பு 0;
#அல்லதெனில் defined HAVE_CONCURRENCY
    திரும்பு std::max(0, (முழு)Concurrency::Context::VirtualProcessorId()); // zero for master thread, unique number for others but not necessary 1,2,3,...
#அல்லதெனில் defined HAVE_PTHREADS_PF
    திரும்பு (முழு)(size_t)(வெறுமை*)pthread_self(); // no zero-based indexing
#இல்லையெனில்
    திரும்பு 0;
#endif
}


#எனில் defined __linux__ || defined __GLIBC__ || defined __HAIKU__ || defined __ANDROID__
  #define CV_CPU_GROUPS_1
#endif

#எனில் defined __linux__ || defined __ANDROID__
  #define CV_HAVE_CGROUPS 1
#endif

#எனில் defined CV_CPU_GROUPS_1
நிலையான உள்வரி
std::string getFileContents(மாறிலி எழுத்து *filename)
{
    std::ifstream ifs(filename);
    எனில் (!ifs.is_open())
        திரும்பு std::string();

    std::string content( (std::istreambuf_iterator<எழுத்து>(ifs) ),
                         (std::istreambuf_iterator<எழுத்து>()    ) );

    எனில் (ifs.fail())
        திரும்பு std::string();

    திரும்பு content;
}

நிலையான உள்வரி
முழு get_எண்_Of_CPUs_Impl(மாறிலி எழுத்து *filename)
{
   std::string file_contents = getFileContents(filename);
   எனில்(file_contents.empty())
       திரும்பு 0;

   எழுத்து *pbuf = const_cast<எழுத்து*>(file_contents.c_str());
   //parse string of form "0-1,3,5-7,10,13-15"
   முழு cpusAvailable = 0;

   போது(*pbuf)
   {
      மாறிலி எழுத்து* pos = pbuf;
      தருக்க range = பொய்;
      போது(*pbuf && *pbuf != ',')
      {
          எனில்(*pbuf == '-') range = உண்மை;
          ++pbuf;
      }
      எனில்(*pbuf) *pbuf++ = 0;
      எனில்(!range)
        ++cpusAvailable;
      இல்லையெனில்
      {
          முழு rstart = 0, rend = 0;
          sscanf(pos, "%d-%d", &rstart, &rend);
          cpusAvailable += rend - rstart + 1;
      }

   }
   திரும்பு cpusAvailable;
}
#endif

#எனில் defined CV_HAVE_CGROUPS
நிலையான உள்வரி
unsigned get_எண்_Of_CPUs_CFSv2()
{
    முழு cfs_quota = 0;
    முழு cfs_period = 0;

    std::ifstream ss_cpu_max("/sys/fs/cgroup/cpu.max", std::ios::in | std::ios::binary);
    ss_cpu_max >> cfs_quota >> cfs_period;

    எனில் (ss_cpu_max.fail() || cfs_quota < 1 || cfs_period < 1) /* values must not be 0 or negative */
        திரும்பு 0;

    திரும்பு (unsigned)max(1, cfs_quota/cfs_period);
}

நிலையான உள்வரி
unsigned get_எண்_Of_CPUs_CFSv1()
{
    முழு cfs_quota = 0;
    {
        std::ifstream ss_period("/sys/fs/cgroup/cpu/cpu.cfs_quota_us", std::ios::in | std::ios::binary);
        ss_period >> cfs_quota;

        எனில் (ss_period.fail() || cfs_quota < 1) /* cfs_quota must not be 0 or negative */
            திரும்பு 0;
    }

    முழு cfs_period = 0;
    {
        std::ifstream ss_quota("/sys/fs/cgroup/cpu/cpu.cfs_period_us", std::ios::in | std::ios::binary);
        ss_quota >> cfs_period;

        எனில் (ss_quota.fail() || cfs_period < 1)
            திரும்பு 0;
    }

    திரும்பு (unsigned)max(1, cfs_quota/cfs_period);
}
#endif

template <typename T> நிலையான உள்வரி
T minNonZero(மாறிலி T& val_1, மாறிலி T& val_2)
{
    எனில் ((val_1 != 0) && (val_2 != 0))
        திரும்பு std::min(val_1, val_2);
    திரும்பு (val_1 != 0) ? val_1 : val_2;
}

#ifndef OPENCV_DISABLE_THREAD_SUPPORT
நிலையான
முழு get_எண்_Of_CPUs_2()
{
#ifndef OPENCV_SEMIHOSTING
    /*
     * Logic here is to try different methods of getting CPU counts and return
     * the minimum most value as it has high probablity of being right and safe.
     * Return 1 if we get 0 or not found on all methods.
    */
#எனில் !defined(__MINGW32__) /* not implemented (2020-03) */

    /*
     * Check for this standard C++11 way, we do not return directly because
     * running in a docker or K8s environment will mean this is the host
     * machines config not the containers or pods and as per docs this value
     * must be "considered only a hint".
    */
    unsigned ncpus = std::thread::hardware_concurrency(); /* If the value is not well defined or not computable, returns 0 */
#இல்லையெனில்
    unsigned ncpus = 0; /* 0 means we have to find out some other way */
#endif

#எனில் defined _WIN32

    SYSTEM_INFO sysinfo = {};
#எனில் (defined(_M_ARM) || defined(_M_ARM64) || defined(_M_ARM64EC) || defined(_M_X64) || defined(WINRT)) && _WIN32_WINNT >= 0x501
    GetNativeSystemInfo( &sysinfo );
#இல்லையெனில்
    GetSystemInfo( &sysinfo );
#endif
    unsigned ncpus_sysinfo = sysinfo.dw_எண்_Of_Processors;
    ncpus = minNonZero(ncpus, ncpus_sysinfo);

#அல்லதெனில் defined __APPLE__

    முழு இலக்கம்_CPU=0;
    முழு mib[4];
    size_t len = அளவு(இலக்கம்_CPU);

    /* set the mib for hw.ncpu */
    mib[0] = CTL_HW;
    mib[1] = HW_AVAILCPU;  // alternatively, try HW_NCPU;

    /* get the number of CPUs from the system */
    sysctl(mib, 2, &இலக்கம்_CPU, &len, வெற்று, 0);

    எனில்( இலக்கம்_CPU < 1 )
    {
        mib[1] = HW_NCPU;
        sysctl( mib, 2, &இலக்கம்_CPU, &len, வெற்று, 0 );

        எனில்( இலக்கம்_CPU < 1 )
            இலக்கம்_CPU = 1;
    }

    ncpus = minNonZero(ncpus, (unsigned)இலக்கம்_CPU);

#அல்லதெனில் defined CV_CPU_GROUPS_1

#எனில் defined CV_HAVE_CGROUPS
    நிலையான unsigned ncpus_impl_cpuset = (unsigned)get_எண்_Of_CPUs_Impl("/sys/fs/cgroup/cpuset/cpuset.cpus");
    ncpus = minNonZero(ncpus, ncpus_impl_cpuset);

    நிலையான unsigned ncpus_impl_cfs_v1 = get_எண்_Of_CPUs_CFSv1();
    ncpus = minNonZero(ncpus, ncpus_impl_cfs_v1);

    நிலையான unsigned ncpus_impl_cfs_v2 = get_எண்_Of_CPUs_CFSv2();
    ncpus = minNonZero(ncpus, ncpus_impl_cfs_v2);
#endif

    நிலையான unsigned ncpus_impl_devices = (unsigned)get_எண்_Of_CPUs_Impl("/sys/devices/system/cpu/online");
    ncpus = minNonZero(ncpus, ncpus_impl_devices);

#endif

#எனில் defined _GNU_SOURCE \
    && !defined(__MINGW32__) /* not implemented (2020-03) */ \
    && !defined(__EMSCRIPTEN__) \
    && !defined(__ANDROID__)  // TODO: add check for modern Android NDK

    cpu_set_t cpu_set;
    எனில் (0 == sched_getaffinity(0, அளவு(cpu_set), &cpu_set))
    {
        unsigned cpu_மொத்தம்_cpu_set = CPU_COUNT(&cpu_set);
        ncpus = minNonZero(ncpus, cpu_மொத்தம்_cpu_set);
    }

#endif

#எனில் !defined(_WIN32) && !defined(__APPLE__) && defined(_SC_NPROCESSORS_ONLN)

    நிலையான unsigned cpu_மொத்தம்_sysconf = (unsigned)sysconf( _SC_NPROCESSORS_ONLN );
    ncpus = minNonZero(ncpus, cpu_மொத்தம்_sysconf);
#அல்லதெனில் defined (__QNX__)
    நிலையான unsigned cpu_மொத்தம்_sysconf = _syspage_ptr->இலக்கம்_cpu;
    ncpus = minNonZero(ncpus, cpu_மொத்தம்_sysconf);
#endif

    திரும்பு ncpus != 0 ? ncpus : 1;
#இல்லையெனில் //  OPENCV_SEMIHOSTING
    திரும்பு 1;
#endif //OPENCV_SEMIHOSTING
}

முழு get_எண்_Of_CPUs()
{
    நிலையான முழு nCPUs = get_எண்_Of_CPUs_2();
    திரும்பு nCPUs;  // cached value
}

#இல்லையெனில்  // OPENCV_DISABLE_THREAD_SUPPORT
முழு get_எண்_Of_CPUs()
{
    திரும்பு 1;
}
#endif  // OPENCV_DISABLE_THREAD_SUPPORT

மாறிலி எழுத்து* currentParallelFramework()
{
    std::shared_ptr<ParallelForAPI>& api = getCurrentParallelForAPI();
    எனில் (api)
    {
        திரும்பு api->getName();
    }
#ifdef CV_PARALLEL_FRAMEWORK
    திரும்பு CV_PARALLEL_FRAMEWORK;
#இல்லையெனில்
    திரும்பு வெற்று;
#endif
}

}  // namespace cv::
