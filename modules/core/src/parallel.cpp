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

#અન્તર્ભાવ "precomp.hpp"

#અન્તર્ભાવ <opencv2/core/utils/configuration.private.hpp>
#અન્તર્ભાવ <opencv2/core/utils/trace.private.hpp>

#અન્તર્ભાવ "opencv2/core/parallel/parallel_backend.hpp"
#અન્તર્ભાવ "parallel/parallel.hpp"

#જો defined _WIN32 || defined WINCE
    #અન્તર્ભાવ <windows.h>
    #undef small
    #undef min
    #undef max
    #undef abs
#endif

#જો defined __unix__ || defined __APPLE__ || defined __GLIBC__ \
    || defined __HAIKU__ || defined __EMSCRIPTEN__ \
    || defined __FreeBSD__ || defined __NetBSD__ || defined __OpenBSD__
    #અન્તર્ભાવ <unistd.h>
    #અન્તર્ભાવ <stdio.h>
    #અન્તર્ભાવ <sys/types.h>
    #અન્તર્ભાવ <fstream>
    #જો defined __ANDROID__
        #અન્તર્ભાવ <sys/sysconf.h>
        #અન્તર્ભાવ <sys/syscall.h>
        #અન્તર્ભાવ <sched.h>
    #અથવાજો defined __APPLE__
        #અન્તર્ભાવ <sys/sysctl.h>
    #endif
#endif

#જો defined (__QNX__)
    #અન્તર્ભાવ <sys/syspage.h>
#endif

#ifndef OPENCV_DISABLE_THREAD_SUPPORT
    #અન્તર્ભાવ <thread>
#endif

#ifdef _OPENMP
    #define HAVE_OPENMP
#endif

#ifdef __APPLE__
    #define HAVE_GCD
#endif

#જો defined _MSC_VER
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

#જો defined HAVE_TBB
    #ifndef TBB_SUPPRESS_DEPRECATED_MESSAGES  // supress warning
    #define TBB_SUPPRESS_DEPRECATED_MESSAGES 1
    #endif
    #અન્તર્ભાવ "tbb/tbb.h"
    #અન્તર્ભાવ "tbb/task.h"
    #જો TBB_INTERFACE_VERSION >= 8000
        #અન્તર્ભાવ "tbb/task_arena.h"
    #endif
    #undef min
    #undef max
#અથવાજો defined HAVE_HPX
    #અન્તર્ભાવ <hpx/parallel/algorithms/for_loop.hpp>
    #અન્તર્ભાવ <hpx/parallel/execution.hpp>
    //
    #અન્તર્ભાવ <hpx/hpx_start.hpp>
    #અન્તર્ભાવ <hpx/hpx_suspend.hpp>
    #અન્તર્ભાવ <hpx/include/apply.hpp>
    #અન્તર્ભાવ <hpx/util/yield_while.hpp>
    #અન્તર્ભાવ <hpx/include/threadmanager.hpp>

#અથવાજો defined HAVE_OPENMP
    #અન્તર્ભાવ <omp.h>
#અથવાજો defined HAVE_GCD
    #અન્તર્ભાવ <dispatch/dispatch.h>
    #અન્તર્ભાવ <pthread.h>
#અથવાજો defined HAVE_CONCURRENCY
    #અન્તર્ભાવ <ppl.h>
#અથવાજો defined HAVE_PTHREADS_PF
    #અન્તર્ભાવ <pthread.h>
#endif


#જો defined HAVE_TBB
#  define CV_PARALLEL_FRAMEWORK "tbb"
#અથવાજો defined HAVE_HPX
#  define CV_PARALLEL_FRAMEWORK "hpx"
#અથવાજો defined HAVE_OPENMP
#  define CV_PARALLEL_FRAMEWORK "openmp"
#અથવાજો defined HAVE_GCD
#  define CV_PARALLEL_FRAMEWORK "gcd"
#અથવાજો defined WINRT
#  define CV_PARALLEL_FRAMEWORK "winrt-concurrency"
#અથવાજો defined HAVE_CONCURRENCY
#  define CV_PARALLEL_FRAMEWORK "ms-concurrency"
#અથવાજો defined HAVE_PTHREADS_PF
#  define CV_PARALLEL_FRAMEWORK "pthreads"
#endif

#અન્તર્ભાવ <atomic>

#અન્તર્ભાવ "parallel_impl.hpp"

#અન્તર્ભાવ "opencv2/core/detail/exception_ptr.hpp"  // CV__EXCEPTION_PTR = 1 if std::exception_ptr is available

#અન્તર્ભાવ <opencv2/core/utils/fp_control_utils.hpp>
#અન્તર્ભાવ <opencv2/core/utils/fp_control.private.hpp>

using નામાકાશ cv;

નામાકાશ cv {

ParallelLoopBody::~ParallelLoopBody() {}

using નામાકાશ cv::parallel;

નામાકાશ {

#ifdef ENABLE_INSTRUMENTATION
    સ્થિર શૂન્ય SyncNodes(cv::instr::Instr_ગાંઠ *p_ગાંઠ)
    {
        std::vector<cv::instr::ગાંઠ_Data_Tls*> data;
        p_ગાંઠ->m_payload.m_tls.gather(data);

        uint64 ticksMax = 0;
        પૂર્ણાંક    threads  = 0;
        માટે(size_t i = 0; i < data.size(); i++)
        {
            જો(data[i] && data[i]->m_ticksTotal)
            {
                ticksMax = MAX(ticksMax, data[i]->m_ticksTotal);
                p_ગાંઠ->m_payload.m_ticksTotal -= data[i]->m_ticksTotal;
                data[i]->m_ticksTotal = 0;
                threads++;
            }
        }
        p_ગાંઠ->m_payload.m_ticksTotal += ticksMax;
        p_ગાંઠ->m_payload.m_threads = MAX(p_ગાંઠ->m_payload.m_threads, threads);

        માટે(size_t i = 0; i < p_ગાંઠ->m_childs.size(); i++)
            SyncNodes(p_ગાંઠ->m_childs[i]);
    }
#endif

    વર્ગ ParallelLoopBodyWrapperContext
    {
    જાહેર:
        ParallelLoopBodyWrapperContext(અચલ cv::ParallelLoopBody& _body, અચલ cv::Range& _r, દ્વિગુણ _nstripes) :
            is_rng_used(ખોટું), hasException(ખોટું)
        {

            body = &_body;
            wholeRange = _r;
            દ્વિગુણ len = wholeRange.end - wholeRange.start;
            nstripes = cvRound(_nstripes <= 0 ? len : MIN(MAX(_nstripes, 1.), len));

            // propagate main thread state
            rng = cv::theRNG();
#જો OPENCV_SUPPORTS_FP_DENORMALS_HINT && OPENCV_IMPL_FP_HINTS
            details::saveFPDenormalsState(fp_denormals_base_state);
#endif

#ifdef OPENCV_TRACE
            trace_મૂળ_Region = CV_TRACE_NS::details::getCurrentRegion();
            trace_મૂળ_Context = CV_TRACE_NS::details::getTraceManager().tls.get();
#endif

#ifdef ENABLE_INSTRUMENTATION
            p_Thread_મૂળ = cv::instr::getInstrumentTLSStruct().p_Current_ગાંઠ;
#endif
        }
        શૂન્ય finalize()
        {
#ifdef ENABLE_INSTRUMENTATION
            માટે(size_t i = 0; i < p_Thread_મૂળ->m_childs.size(); i++)
                SyncNodes(p_Thread_મૂળ->m_childs[i]);
#endif
            જો (is_rng_used)
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
            જો (trace_મૂળ_Region)
                CV_TRACE_NS::details::parallelForFinalize(*trace_મૂળ_Region);
#endif

            જો (hasException)
            {
#જો CV__EXCEPTION_PTR
                std::rethrow_exception(pException);
#નહીંતર
                CV_Error(Error::StsError, "Exception in parallel_for() body: " + exception_message);
#endif
            }
        }
        ~ParallelLoopBodyWrapperContext() {}

        અચલ cv::ParallelLoopBody* body;
        cv::Range wholeRange;
        પૂર્ણાંક nstripes;
        cv::RNG rng;
        mutable તાર્કિક is_rng_used;
#ifdef OPENCV_TRACE
        CV_TRACE_NS::details::Region* trace_મૂળ_Region;
        CV_TRACE_NS::details::TraceManagerThreadLocal* trace_મૂળ_Context;
#endif
#ifdef ENABLE_INSTRUMENTATION
        cv::instr::Instr_ગાંઠ *p_Thread_મૂળ;
#endif
        તાર્કિક hasException;
#જો CV__EXCEPTION_PTR
        std::exception_ptr pException;
#નહીંતર
        cv::String exception_message;
#endif
#જો CV__EXCEPTION_PTR
        શૂન્ય recordException()
#નહીંતર
        શૂન્ય recordException(અચલ cv::String& સંદેશ)
#endif
        {
#ifndef CV_THREAD_SANITIZER
            જો (!hasException)
#endif
            {
                cv::AutoLock lock(cv::getInitializationMutex());
                જો (!hasException)
                {
                    hasException = સાચું;
#જો CV__EXCEPTION_PTR
                    pException = std::current_exception();
#નહીંતર
                    exception_message = સંદેશ;
#endif
                }
            }
        }

#જો OPENCV_SUPPORTS_FP_DENORMALS_HINT && OPENCV_IMPL_FP_HINTS
        details::FPDenormalsModeState fp_denormals_base_state;
#endif

    ખાનગી:
        ParallelLoopBodyWrapperContext(અચલ ParallelLoopBodyWrapperContext&); // disabled
        ParallelLoopBodyWrapperContext& operator=(અચલ ParallelLoopBodyWrapperContext&); // disabled
    };

    વર્ગ ParallelLoopBodyWrapper : જાહેર cv::ParallelLoopBody
    {
    જાહેર:
        ParallelLoopBodyWrapper(ParallelLoopBodyWrapperContext& ctx_) :
            ctx(ctx_)
        {
        }
        ~ParallelLoopBodyWrapper()
        {
        }
        શૂન્ય operator()(અચલ cv::Range& sr) અચલ CV_OVERRIDE
        {
#ifdef OPENCV_TRACE
            // TODO CV_TRACE_NS::details::setCurrentRegion(rootRegion);
            જો (ctx.trace_મૂળ_Region && ctx.trace_મૂળ_Context)
                CV_TRACE_NS::details::parallel_For_Set_મૂળ_Region(*ctx.trace_મૂળ_Region, *ctx.trace_મૂળ_Context);
            CV__TRACE_OPENCV_FUNCTION_NAME("parallel_for_body");
            જો (ctx.trace_મૂળ_Region)
                CV_TRACE_NS::details::parallelForAttachNestedRegion(*ctx.trace_મૂળ_Region);
#endif

#ifdef ENABLE_INSTRUMENTATION
            {
                cv::instr::InstrTLSStruct *pInstrTLS = &cv::instr::getInstrumentTLSStruct();
                pInstrTLS->p_Current_ગાંઠ = ctx.p_Thread_મૂળ; // Initialize TLS node for thread
            }
            CV_INSTRUMENT_REGION();
#endif

            // propagate main thread state
            cv::theRNG() = ctx.rng;
#જો OPENCV_SUPPORTS_FP_DENORMALS_HINT && OPENCV_IMPL_FP_HINTS
            FPDenormalsIgnoreHintScope fp_denormals_scope(ctx.fp_denormals_base_state);
#endif

            cv::Range r;
            cv::Range wholeRange = ctx.wholeRange;
            પૂર્ણાંક nstripes = ctx.nstripes;
            r.start = (પૂર્ણાંક)(wholeRange.start +
                            ((uint64)sr.start*(wholeRange.end - wholeRange.start) + nstripes/2)/nstripes);
            r.end = sr.end >= nstripes ? wholeRange.end : (પૂર્ણાંક)(wholeRange.start +
                            ((uint64)sr.end*(wholeRange.end - wholeRange.start) + nstripes/2)/nstripes);

#ifdef OPENCV_TRACE
            CV_TRACE_ARG_કિંમત(range_start, "range.start", (int64)r.start);
            CV_TRACE_ARG_કિંમત(range_end, "range.end", (int64)r.end);
#endif

            પ્રયાસ
            {
                (*ctx.body)(r);
            }
#જો CV__EXCEPTION_PTR
            પકડો (...)
            {
                ctx.recordException();
            }
#નહીંતર
            પકડો (અચલ cv::Exception& e)
            {
                ctx.recordException(e.what());
            }
            પકડો (અચલ std::exception& e)
            {
                ctx.recordException(e.what());
            }
            પકડો (...)
            {
                ctx.recordException("Unknown exception");
            }
#endif

            જો (!ctx.is_rng_used && !(cv::theRNG() == ctx.rng))
                ctx.is_rng_used = સાચું;
        }
        cv::Range stripeRange() અચલ { પાછા cv::Range(0, ctx.nstripes); }

    સુરક્ષિત:
        ParallelLoopBodyWrapperContext& ctx;
    };

#જો defined HAVE_TBB
    વર્ગ ProxyLoopBody : જાહેર ParallelLoopBodyWrapper
    {
    જાહેર:
        ProxyLoopBody(ParallelLoopBodyWrapperContext& ctx_)
        : ParallelLoopBodyWrapper(ctx_)
        {}

        શૂન્ય operator ()(અચલ tbb::blocked_range<પૂર્ણાંક>& range) અચલ
        {
            આ->ParallelLoopBodyWrapper::operator()(cv::Range(range.begin(), range.end()));
        }

        શૂન્ય operator ()() અચલ  // run parallel job
        {
            cv::Range range = આ->stripeRange();
            tbb::parallel_for(tbb::blocked_range<પૂર્ણાંક>(range.start, range.end), *આ);
        }
    };
#અથવાજો defined HAVE_HPX
    વર્ગ ProxyLoopBody : જાહેર ParallelLoopBodyWrapper
    {
    જાહેર:
        ProxyLoopBody(ParallelLoopBodyWrapperContext& ctx_)
                : ParallelLoopBodyWrapper(ctx_)
        {}

        શૂન્ય operator ()() અચલ  // run parallel job
        {
            cv::Range stripeRange = આ->stripeRange();
            hpx::parallel::for_loop(
                    hpx::parallel::execution::par,
                    stripeRange.start, stripeRange.end,
                    [&](અચલ પૂર્ણાંક &i) { ;
                        આ->ParallelLoopBodyWrapper::operator()(
                                cv::Range(i, i + 1));
                    });
        }
    };
#અથવાજો defined HAVE_OPENMP
    પ્રકારવ્યાખ્યા ParallelLoopBodyWrapper ProxyLoopBody;
#અથવાજો defined HAVE_GCD
    પ્રકારવ્યાખ્યા ParallelLoopBodyWrapper ProxyLoopBody;
    સ્થિર શૂન્ય block_function(શૂન્ય* context, size_t index)
    {
        ProxyLoopBody* ptr_body = static_cast<ProxyLoopBody*>(context);
        (*ptr_body)(cv::Range((પૂર્ણાંક)index, (પૂર્ણાંક)index + 1));
    }
#અથવાજો defined WINRT || defined HAVE_CONCURRENCY
    વર્ગ ProxyLoopBody : જાહેર ParallelLoopBodyWrapper
    {
    જાહેર:
        ProxyLoopBody(ParallelLoopBodyWrapperContext& ctx)
        : ParallelLoopBodyWrapper(ctx)
        {}

        શૂન્ય operator ()(પૂર્ણાંક i) અચલ
        {
            આ->ParallelLoopBodyWrapper::operator()(cv::Range(i, i + 1));
        }
    };

    // PPL's default auto_partitioner silently skips whole chunks of the range on
    // ARM64 (unfixed MSVC bug, developercommunity.visualstudio.com/t/1027444).
    template <typename Body>
    સ્થિર અંતઃરેખ શૂન્ય pplParallelFor(પૂર્ણાંક first, પૂર્ણાંક last, અચલ Body& body)
    {
#જો defined(_M_ARM64) || defined(_M_ARM64EC)
        Concurrency::parallel_for(first, last, body, Concurrency::static_partitioner());
#નહીંતર
        Concurrency::parallel_for(first, last, body);
#endif
    }
#નહીંતર
    પ્રકારવ્યાખ્યા ParallelLoopBodyWrapper ProxyLoopBody;
#endif

#જો defined HAVE_TBB
    #જો TBB_INTERFACE_VERSION >= 8000
        સ્થિર tbb::task_arena tbbArena(tbb::task_arena::automatic);
    #નહીંતર
        સ્થિર tbb::task_scheduler_init tbbScheduler(tbb::task_scheduler_init::deferred);
    #endif
#અથવાજો defined HAVE_HPX
// nothing for HPX
#અથવાજો defined HAVE_OPENMP
સ્થિર અંતઃરેખ પૂર્ણાંક _initMaxThreads()
{
    પૂર્ણાંક maxThreads = omp_get_max_threads();
    પાછા maxThreads;
}
સ્થિર પૂર્ણાંક અંક_Threads_Max = _initMaxThreads();
#અથવાજો defined HAVE_GCD
// nothing for GCD
#અથવાજો defined WINRT
// nothing for WINRT
#અથવાજો defined HAVE_CONCURRENCY

વર્ગ SchedPtr
{
    Concurrency::Scheduler* sched_;
જાહેર:
    Concurrency::Scheduler* operator->() { પાછા sched_; }
    operator Concurrency::Scheduler*() { પાછા sched_; }

    શૂન્ય operator=(Concurrency::Scheduler* sched)
    {
        જો (sched_) sched_->Release();
        sched_ = sched;
    }

    SchedPtr() : sched_(0) {}
    ~SchedPtr() {}
};
સ્થિર SchedPtr pplScheduler;

#endif

} // namespace anon

/* ================================   parallel_for_  ================================ */

સ્થિર શૂન્ય parallel_for_impl(અચલ cv::Range& range, અચલ cv::ParallelLoopBody& body, દ્વિગુણ nstripes); // forward declaration

શૂન્ય parallel_for_(અચલ cv::Range& range, અચલ cv::ParallelLoopBody& body, દ્વિગુણ nstripes)
{
#ifdef OPENCV_TRACE
    CV__TRACE_OPENCV_FUNCTION_NAME_("parallel_for", 0);
    CV_TRACE_ARG_કિંમત(range_start, "range.start", (int64)range.start);
    CV_TRACE_ARG_કિંમત(range_end, "range.end", (int64)range.end);
    CV_TRACE_ARG_કિંમત(nstripes, "nstripes", (int64)nstripes);
#endif

    CV_INSTRUMENT_REGION_MT_FORK();
    જો (range.empty())
        પાછા;

    સ્થિર std::atomic<તાર્કિક> flagNestedParallelFor(ખોટું);
    તાર્કિક isNotNestedRegion = !flagNestedParallelFor.load();
    જો (isNotNestedRegion)
      isNotNestedRegion = !flagNestedParallelFor.exchange(સાચું);
    જો (isNotNestedRegion)
    {
        પ્રયાસ
        {
            parallel_for_impl(range, body, nstripes);
            flagNestedParallelFor = ખોટું;
        }
        પકડો (...)
        {
            flagNestedParallelFor = ખોટું;
            ફેંકો;
        }
    }
    નહીંતર // nested parallel_for_() calls are not parallelized
    {
        CV_UNUSED(nstripes);
        body(range);
    }
}

સ્થિર
શૂન્ય parallel_for_cb(પૂર્ણાંક start, પૂર્ણાંક end, શૂન્ય* data)
{
    CV_DbgAssert(data);
    અચલ cv::ParallelLoopBody& body = *(અચલ cv::ParallelLoopBody*)data;
    body(Range(start, end));
}

સ્થિર શૂન્ય parallel_for_impl(અચલ cv::Range& range, અચલ cv::ParallelLoopBody& body, દ્વિગુણ nstripes)
{
    using નામાકાશ cv::parallel;
    જો ((અંક_Threads < 0 || અંક_Threads > 1) && range.end - range.start > 1)
    {
        ParallelLoopBodyWrapperContext ctx(body, range, nstripes);
        ProxyLoopBody pbody(ctx);
        cv::Range stripeRange = pbody.stripeRange();
        જો( stripeRange.end - stripeRange.start == 1 )
        {
            body(range);
            પાછા;
        }

        std::shared_ptr<ParallelForAPI>& api = getCurrentParallelForAPI();
        જો (api)
        {
            CV_CheckEQ(stripeRange.start, 0, "");
            api->parallel_for(stripeRange.end, parallel_for_cb, (શૂન્ય*)&pbody);
            ctx.finalize();  // propagate exceptions if exists
            પાછા;
        }

#ifdef CV_PARALLEL_FRAMEWORK
#જો defined HAVE_TBB

#જો TBB_INTERFACE_VERSION >= 8000
        tbbArena.execute(pbody);
#નહીંતર
        pbody();
#endif

#અથવાજો defined HAVE_HPX
        pbody();

#અથવાજો defined HAVE_OPENMP

        #pragma omp parallel માટે schedule(dynamic) અંક_threads(અંક_Threads > 0 ? અંક_Threads : અંક_Threads_Max)
        માટે (પૂર્ણાંક i = stripeRange.start; i < stripeRange.end; ++i)
            pbody(Range(i, i + 1));

#અથવાજો defined HAVE_GCD

        dispatch_queue_t concurrent_queue = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0);
        dispatch_apply_f(stripeRange.end - stripeRange.start, concurrent_queue, &pbody, block_function);

#અથવાજો defined WINRT

        pplParallelFor(stripeRange.start, stripeRange.end, pbody);

#અથવાજો defined HAVE_CONCURRENCY

        જો(!pplScheduler || pplScheduler->Id() == Concurrency::CurrentScheduler::Id())
        {
            pplParallelFor(stripeRange.start, stripeRange.end, pbody);
        }
        નહીંતર
        {
            pplScheduler->Attach();
            pplParallelFor(stripeRange.start, stripeRange.end, pbody);
            Concurrency::CurrentScheduler::Detach();
        }

#અથવાજો defined HAVE_PTHREADS_PF

        parallel_for_pthreads(pbody.stripeRange(), pbody, pbody.stripeRange().size());

#નહીંતર

#error You have hacked and compiling with unsupported parallel framework

#endif

        ctx.finalize();  // propagate exceptions if exists
        પાછા;
#endif // CV_PARALLEL_FRAMEWORK
    }

    body(range);
}


પૂર્ણાંક get_અંક_Threads(શૂન્ય)
{
    std::shared_ptr<ParallelForAPI>& api = getCurrentParallelForAPI();
    જો (api)
    {
        પાછા api->get_અંક_Threads();
    }

    જો (અંક_Threads == 0)
        પાછા 1;

#જો defined HAVE_TBB

#જો TBB_INTERFACE_VERSION >= 9100
    પાછા tbbArena.max_concurrency();
#અથવાજો TBB_INTERFACE_VERSION >= 8000
    પાછા અંક_Threads > 0
        ? અંક_Threads
        : tbb::task_scheduler_init::default_અંક_threads();
#નહીંતર
    પાછા tbbScheduler.is_active()
           ? અંક_Threads
           : tbb::task_scheduler_init::default_અંક_threads();
#endif

#અથવાજો defined HAVE_HPX
    પાછા અંક_Threads;

#અથવાજો defined HAVE_OPENMP

    પાછા અંક_Threads > 0
           ? અંક_Threads
           : અંક_Threads_Max;


#અથવાજો defined HAVE_GCD

    પાછા cv::get_સંખ્યા_Of_CPUs(); // the GCD thread pool limit

#અથવાજો defined WINRT

    પાછા 0;

#અથવાજો defined HAVE_CONCURRENCY

    પાછા (pplScheduler == 0)
        ? Concurrency::CurrentScheduler::Get()->Get_સંખ્યા_Of_Virtual_Processors()
        : (1 + pplScheduler->Get_સંખ્યા_Of_Virtual_Processors());

#અથવાજો defined HAVE_PTHREADS_PF

        પાછા parallel_pthreads_get_threads_અંક();

#નહીંતર

    પાછા 1;

#endif
}

unsigned default_સંખ્યા_Of_Threads()
{
#ifdef __ANDROID__
    // many modern phones/tables have 4-core CPUs. Let's use no more
    // than 2 threads by default not to overheat the devices
    અચલ unsigned પૂર્ણાંક default_સંખ્યા_of_threads = 2;
#નહીંતર
    અચલ unsigned પૂર્ણાંક default_સંખ્યા_of_threads = (unsigned પૂર્ણાંક)std::max(1, cv::get_સંખ્યા_Of_CPUs());
#endif

    unsigned result = default_સંખ્યા_of_threads;

    સ્થિર પૂર્ણાંક config_અંક_threads = (પૂર્ણાંક)utils::getConfigurationParameterSizeT("OPENCV_FOR_THREADS_NUM", 0);

    જો (config_અંક_threads)
    {
        result = (unsigned)std::max(1, config_અંક_threads);
        //do we need upper limit of threads number?
    }
    પાછા result;
}

શૂન્ય set_અંક_Threads( પૂર્ણાંક threads_ )
{
    CV_UNUSED(threads_);

    પૂર્ણાંક threads = (threads_ < 0) ? default_સંખ્યા_Of_Threads() : (unsigned)threads_;
    અંક_Threads = threads;

    std::shared_ptr<ParallelForAPI>& api = getCurrentParallelForAPI();
    જો (api)
    {
        api->set_અંક_Threads(અંક_Threads);
    }

#ifdef HAVE_TBB

#જો TBB_INTERFACE_VERSION >= 8000
    જો(tbbArena.is_active()) tbbArena.terminate();
    જો(threads > 0) tbbArena.initialize(threads);
#નહીંતર
    જો(tbbScheduler.is_active()) tbbScheduler.terminate();
    જો(threads > 0) tbbScheduler.initialize(threads);
#endif

#અથવાજો defined HAVE_HPX
    પાછા; // nothing needed as numThreads is used

#અથવાજો defined HAVE_OPENMP

    પાછા; // nothing needed as num_threads clause is used in #pragma omp parallel for

#અથવાજો defined HAVE_GCD

    // unsupported
    // there is only private dispatch_queue_set_width() and only for desktop

#અથવાજો defined WINRT

    પાછા;

#અથવાજો defined HAVE_CONCURRENCY

    જો (threads <= 0)
    {
        pplScheduler = 0;
    }
    નહીંતર જો (threads == 1)
    {
        // Concurrency always uses >=2 threads, so we just disable it if 1 thread is requested
        અંક_Threads = 0;
    }
    નહીંતર જો (pplScheduler == 0 || 1 + pplScheduler->Get_સંખ્યા_Of_Virtual_Processors() != (unsigned પૂર્ણાંક)threads)
    {
        pplScheduler = Concurrency::Scheduler::Create(Concurrency::SchedulerPolicy(2,
                       Concurrency::MinConcurrency, threads-1,
                       Concurrency::MaxConcurrency, threads-1));
    }

#અથવાજો defined HAVE_PTHREADS_PF

    parallel_pthreads_set_threads_અંક(threads);

#endif
}


પૂર્ણાંક get_Thread_અંક()
{
    std::shared_ptr<ParallelForAPI>& api = getCurrentParallelForAPI();
    જો (api)
    {
        પાછા api->get_Thread_અંક();
    }

#જો defined HAVE_TBB
    #જો TBB_INTERFACE_VERSION >= 9100
        પાછા tbb::this_task_arena::current_thread_index();
    #અથવાજો TBB_INTERFACE_VERSION >= 8000
        પાછા tbb::task_arena::current_thread_index();
    #નહીંતર
        પાછા 0;
    #endif
#અથવાજો defined HAVE_HPX
    પાછા (પૂર્ણાંક)(hpx::get_અંક_worker_threads());
#અથવાજો defined HAVE_OPENMP
    પાછા omp_get_thread_અંક();
#અથવાજો defined HAVE_GCD
    પાછા (પૂર્ણાંક)(size_t)(શૂન્ય*)pthread_self(); // no zero-based indexing
#અથવાજો defined WINRT
    પાછા 0;
#અથવાજો defined HAVE_CONCURRENCY
    પાછા std::max(0, (પૂર્ણાંક)Concurrency::Context::VirtualProcessorId()); // zero for master thread, unique number for others but not necessary 1,2,3,...
#અથવાજો defined HAVE_PTHREADS_PF
    પાછા (પૂર્ણાંક)(size_t)(શૂન્ય*)pthread_self(); // no zero-based indexing
#નહીંતર
    પાછા 0;
#endif
}


#જો defined __linux__ || defined __GLIBC__ || defined __HAIKU__ || defined __ANDROID__
  #define CV_CPU_GROUPS_1
#endif

#જો defined __linux__ || defined __ANDROID__
  #define CV_HAVE_CGROUPS 1
#endif

#જો defined CV_CPU_GROUPS_1
સ્થિર અંતઃરેખ
std::string getFileContents(અચલ અક્ષર *filename)
{
    std::ifstream ifs(filename);
    જો (!ifs.is_open())
        પાછા std::string();

    std::string content( (std::istreambuf_iterator<અક્ષર>(ifs) ),
                         (std::istreambuf_iterator<અક્ષર>()    ) );

    જો (ifs.fail())
        પાછા std::string();

    પાછા content;
}

સ્થિર અંતઃરેખ
પૂર્ણાંક get_સંખ્યા_Of_CPUs_Impl(અચલ અક્ષર *filename)
{
   std::string file_contents = getFileContents(filename);
   જો(file_contents.empty())
       પાછા 0;

   અક્ષર *pbuf = const_cast<અક્ષર*>(file_contents.c_str());
   //parse string of form "0-1,3,5-7,10,13-15"
   પૂર્ણાંક cpusAvailable = 0;

   જ્યારે(*pbuf)
   {
      અચલ અક્ષર* pos = pbuf;
      તાર્કિક range = ખોટું;
      જ્યારે(*pbuf && *pbuf != ',')
      {
          જો(*pbuf == '-') range = સાચું;
          ++pbuf;
      }
      જો(*pbuf) *pbuf++ = 0;
      જો(!range)
        ++cpusAvailable;
      નહીંતર
      {
          પૂર્ણાંક rstart = 0, rend = 0;
          sscanf(pos, "%d-%d", &rstart, &rend);
          cpusAvailable += rend - rstart + 1;
      }

   }
   પાછા cpusAvailable;
}
#endif

#જો defined CV_HAVE_CGROUPS
સ્થિર અંતઃરેખ
unsigned get_સંખ્યા_Of_CPUs_CFSv2()
{
    પૂર્ણાંક cfs_quota = 0;
    પૂર્ણાંક cfs_period = 0;

    std::ifstream ss_cpu_max("/sys/fs/cgroup/cpu.max", std::ios::in | std::ios::binary);
    ss_cpu_max >> cfs_quota >> cfs_period;

    જો (ss_cpu_max.fail() || cfs_quota < 1 || cfs_period < 1) /* values must not be 0 or negative */
        પાછા 0;

    પાછા (unsigned)max(1, cfs_quota/cfs_period);
}

સ્થિર અંતઃરેખ
unsigned get_સંખ્યા_Of_CPUs_CFSv1()
{
    પૂર્ણાંક cfs_quota = 0;
    {
        std::ifstream ss_period("/sys/fs/cgroup/cpu/cpu.cfs_quota_us", std::ios::in | std::ios::binary);
        ss_period >> cfs_quota;

        જો (ss_period.fail() || cfs_quota < 1) /* cfs_quota must not be 0 or negative */
            પાછા 0;
    }

    પૂર્ણાંક cfs_period = 0;
    {
        std::ifstream ss_quota("/sys/fs/cgroup/cpu/cpu.cfs_period_us", std::ios::in | std::ios::binary);
        ss_quota >> cfs_period;

        જો (ss_quota.fail() || cfs_period < 1)
            પાછા 0;
    }

    પાછા (unsigned)max(1, cfs_quota/cfs_period);
}
#endif

template <typename T> સ્થિર અંતઃરેખ
T minNonZero(અચલ T& val_1, અચલ T& val_2)
{
    જો ((val_1 != 0) && (val_2 != 0))
        પાછા std::min(val_1, val_2);
    પાછા (val_1 != 0) ? val_1 : val_2;
}

#ifndef OPENCV_DISABLE_THREAD_SUPPORT
સ્થિર
પૂર્ણાંક get_સંખ્યા_Of_CPUs_2()
{
#ifndef OPENCV_SEMIHOSTING
    /*
     * Logic here is to try different methods of getting CPU counts and return
     * the minimum most value as it has high probablity of being right and safe.
     * Return 1 if we get 0 or not found on all methods.
    */
#જો !defined(__MINGW32__) /* not implemented (2020-03) */

    /*
     * Check for this standard C++11 way, we do not return directly because
     * running in a docker or K8s environment will mean this is the host
     * machines config not the containers or pods and as per docs this value
     * must be "considered only a hint".
    */
    unsigned ncpus = std::thread::hardware_concurrency(); /* If the value is not well defined or not computable, returns 0 */
#નહીંતર
    unsigned ncpus = 0; /* 0 means we have to find out some other way */
#endif

#જો defined _WIN32

    SYSTEM_INFO sysinfo = {};
#જો (defined(_M_ARM) || defined(_M_ARM64) || defined(_M_ARM64EC) || defined(_M_X64) || defined(WINRT)) && _WIN32_WINNT >= 0x501
    GetNativeSystemInfo( &sysinfo );
#નહીંતર
    GetSystemInfo( &sysinfo );
#endif
    unsigned ncpus_sysinfo = sysinfo.dw_સંખ્યા_Of_Processors;
    ncpus = minNonZero(ncpus, ncpus_sysinfo);

#અથવાજો defined __APPLE__

    પૂર્ણાંક અંક_CPU=0;
    પૂર્ણાંક mib[4];
    size_t len = કદ(અંક_CPU);

    /* set the mib for hw.ncpu */
    mib[0] = CTL_HW;
    mib[1] = HW_AVAILCPU;  // alternatively, try HW_NCPU;

    /* get the number of CPUs from the system */
    sysctl(mib, 2, &અંક_CPU, &len, ખાલી, 0);

    જો( અંક_CPU < 1 )
    {
        mib[1] = HW_NCPU;
        sysctl( mib, 2, &અંક_CPU, &len, ખાલી, 0 );

        જો( અંક_CPU < 1 )
            અંક_CPU = 1;
    }

    ncpus = minNonZero(ncpus, (unsigned)અંક_CPU);

#અથવાજો defined CV_CPU_GROUPS_1

#જો defined CV_HAVE_CGROUPS
    સ્થિર unsigned ncpus_impl_cpuset = (unsigned)get_સંખ્યા_Of_CPUs_Impl("/sys/fs/cgroup/cpuset/cpuset.cpus");
    ncpus = minNonZero(ncpus, ncpus_impl_cpuset);

    સ્થિર unsigned ncpus_impl_cfs_v1 = get_સંખ્યા_Of_CPUs_CFSv1();
    ncpus = minNonZero(ncpus, ncpus_impl_cfs_v1);

    સ્થિર unsigned ncpus_impl_cfs_v2 = get_સંખ્યા_Of_CPUs_CFSv2();
    ncpus = minNonZero(ncpus, ncpus_impl_cfs_v2);
#endif

    સ્થિર unsigned ncpus_impl_devices = (unsigned)get_સંખ્યા_Of_CPUs_Impl("/sys/devices/system/cpu/online");
    ncpus = minNonZero(ncpus, ncpus_impl_devices);

#endif

#જો defined _GNU_SOURCE \
    && !defined(__MINGW32__) /* not implemented (2020-03) */ \
    && !defined(__EMSCRIPTEN__) \
    && !defined(__ANDROID__)  // TODO: add check for modern Android NDK

    cpu_set_t cpu_set;
    જો (0 == sched_getaffinity(0, કદ(cpu_set), &cpu_set))
    {
        unsigned cpu_ગણતરી_cpu_set = CPU_COUNT(&cpu_set);
        ncpus = minNonZero(ncpus, cpu_ગણતરી_cpu_set);
    }

#endif

#જો !defined(_WIN32) && !defined(__APPLE__) && defined(_SC_NPROCESSORS_ONLN)

    સ્થિર unsigned cpu_ગણતરી_sysconf = (unsigned)sysconf( _SC_NPROCESSORS_ONLN );
    ncpus = minNonZero(ncpus, cpu_ગણતરી_sysconf);
#અથવાજો defined (__QNX__)
    સ્થિર unsigned cpu_ગણતરી_sysconf = _syspage_ptr->અંક_cpu;
    ncpus = minNonZero(ncpus, cpu_ગણતરી_sysconf);
#endif

    પાછા ncpus != 0 ? ncpus : 1;
#નહીંતર //  OPENCV_SEMIHOSTING
    પાછા 1;
#endif //OPENCV_SEMIHOSTING
}

પૂર્ણાંક get_સંખ્યા_Of_CPUs()
{
    સ્થિર પૂર્ણાંક nCPUs = get_સંખ્યા_Of_CPUs_2();
    પાછા nCPUs;  // cached value
}

#નહીંતર  // OPENCV_DISABLE_THREAD_SUPPORT
પૂર્ણાંક get_સંખ્યા_Of_CPUs()
{
    પાછા 1;
}
#endif  // OPENCV_DISABLE_THREAD_SUPPORT

અચલ અક્ષર* currentParallelFramework()
{
    std::shared_ptr<ParallelForAPI>& api = getCurrentParallelForAPI();
    જો (api)
    {
        પાછા api->getName();
    }
#ifdef CV_PARALLEL_FRAMEWORK
    પાછા CV_PARALLEL_FRAMEWORK;
#નહીંતર
    પાછા ખાલી;
#endif
}

}  // namespace cv::
