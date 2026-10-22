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

#অন্তর্ভাব "precomp.hpp"

#অন্তর্ভাব <opencv2/core/utils/configuration.private.hpp>
#অন্তর্ভাব <opencv2/core/utils/trace.private.hpp>

#অন্তর্ভাব "opencv2/core/parallel/parallel_backend.hpp"
#অন্তর্ভাব "parallel/parallel.hpp"

#যদি defined _WIN32 || defined WINCE
    #অন্তর্ভাব <windows.h>
    #undef small
    #undef min
    #undef max
    #undef abs
#endif

#যদি defined __unix__ || defined __APPLE__ || defined __GLIBC__ \
    || defined __HAIKU__ || defined __EMSCRIPTEN__ \
    || defined __FreeBSD__ || defined __NetBSD__ || defined __OpenBSD__
    #অন্তর্ভাব <unistd.h>
    #অন্তর্ভাব <stdio.h>
    #অন্তর্ভাব <sys/types.h>
    #অন্তর্ভাব <fstream>
    #যদি defined __ANDROID__
        #অন্তর্ভাব <sys/sysconf.h>
        #অন্তর্ভাব <sys/syscall.h>
        #অন্তর্ভাব <sched.h>
    #নোচেদ্যদি defined __APPLE__
        #অন্তর্ভাব <sys/sysctl.h>
    #endif
#endif

#যদি defined (__QNX__)
    #অন্তর্ভাব <sys/syspage.h>
#endif

#ifndef OPENCV_DISABLE_THREAD_SUPPORT
    #অন্তর্ভাব <thread>
#endif

#ifdef _OPENMP
    #define HAVE_OPENMP
#endif

#ifdef __APPLE__
    #define HAVE_GCD
#endif

#যদি defined _MSC_VER
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

#যদি defined HAVE_TBB
    #ifndef TBB_SUPPRESS_DEPRECATED_MESSAGES  // supress warning
    #define TBB_SUPPRESS_DEPRECATED_MESSAGES 1
    #endif
    #অন্তর্ভাব "tbb/tbb.h"
    #অন্তর্ভাব "tbb/task.h"
    #যদি TBB_INTERFACE_VERSION >= 8000
        #অন্তর্ভাব "tbb/task_arena.h"
    #endif
    #undef min
    #undef max
#নোচেদ্যদি defined HAVE_HPX
    #অন্তর্ভাব <hpx/parallel/algorithms/for_loop.hpp>
    #অন্তর্ভাব <hpx/parallel/execution.hpp>
    //
    #অন্তর্ভাব <hpx/hpx_start.hpp>
    #অন্তর্ভাব <hpx/hpx_suspend.hpp>
    #অন্তর্ভাব <hpx/include/apply.hpp>
    #অন্তর্ভাব <hpx/util/yield_while.hpp>
    #অন্তর্ভাব <hpx/include/threadmanager.hpp>

#নোচেদ্যদি defined HAVE_OPENMP
    #অন্তর্ভাব <omp.h>
#নোচেদ্যদি defined HAVE_GCD
    #অন্তর্ভাব <dispatch/dispatch.h>
    #অন্তর্ভাব <pthread.h>
#নোচেদ্যদি defined HAVE_CONCURRENCY
    #অন্তর্ভাব <ppl.h>
#নোচেদ্যদি defined HAVE_PTHREADS_PF
    #অন্তর্ভাব <pthread.h>
#endif


#যদি defined HAVE_TBB
#  define CV_PARALLEL_FRAMEWORK "tbb"
#নোচেদ্যদি defined HAVE_HPX
#  define CV_PARALLEL_FRAMEWORK "hpx"
#নোচেদ্যদি defined HAVE_OPENMP
#  define CV_PARALLEL_FRAMEWORK "openmp"
#নোচেদ্যদি defined HAVE_GCD
#  define CV_PARALLEL_FRAMEWORK "gcd"
#নোচেদ্যদি defined WINRT
#  define CV_PARALLEL_FRAMEWORK "winrt-concurrency"
#নোচেদ্যদি defined HAVE_CONCURRENCY
#  define CV_PARALLEL_FRAMEWORK "ms-concurrency"
#নোচেদ্যদি defined HAVE_PTHREADS_PF
#  define CV_PARALLEL_FRAMEWORK "pthreads"
#endif

#অন্তর্ভাব <atomic>

#অন্তর্ভাব "parallel_impl.hpp"

#অন্তর্ভাব "opencv2/core/detail/exception_ptr.hpp"  // CV__EXCEPTION_PTR = 1 if std::exception_ptr is available

#অন্তর্ভাব <opencv2/core/utils/fp_control_utils.hpp>
#অন্তর্ভাব <opencv2/core/utils/fp_control.private.hpp>

using নামাকাশ cv;

নামাকাশ cv {

ParallelLoopBody::~ParallelLoopBody() {}

using নামাকাশ cv::parallel;

নামাকাশ {

#ifdef ENABLE_INSTRUMENTATION
    স্থির শূন্য SyncNodes(cv::instr::Instr_গ্রন্থি *p_গ্রন্থি)
    {
        std::vector<cv::instr::গ্রন্থি_Data_Tls*> data;
        p_গ্রন্থি->m_payload.m_tls.gather(data);

        uint64 ticksMax = 0;
        পূর্ণাঙ্ক    threads  = 0;
        প্রতি(size_t i = 0; i < data.size(); i++)
        {
            যদি(data[i] && data[i]->m_ticksTotal)
            {
                ticksMax = MAX(ticksMax, data[i]->m_ticksTotal);
                p_গ্রন্থি->m_payload.m_ticksTotal -= data[i]->m_ticksTotal;
                data[i]->m_ticksTotal = 0;
                threads++;
            }
        }
        p_গ্রন্থি->m_payload.m_ticksTotal += ticksMax;
        p_গ্রন্থি->m_payload.m_threads = MAX(p_গ্রন্থি->m_payload.m_threads, threads);

        প্রতি(size_t i = 0; i < p_গ্রন্থি->m_childs.size(); i++)
            SyncNodes(p_গ্রন্থি->m_childs[i]);
    }
#endif

    বর্গ ParallelLoopBodyWrapperContext
    {
    সার্বজনিক:
        ParallelLoopBodyWrapperContext(ধ্রুব cv::ParallelLoopBody& _body, ধ্রুব cv::Range& _r, দ্বিগুণ _nstripes) :
            is_rng_used(অসত্য), hasException(অসত্য)
        {

            body = &_body;
            wholeRange = _r;
            দ্বিগুণ len = wholeRange.end - wholeRange.start;
            nstripes = cvRound(_nstripes <= 0 ? len : MIN(MAX(_nstripes, 1.), len));

            // propagate main thread state
            rng = cv::theRNG();
#যদি OPENCV_SUPPORTS_FP_DENORMALS_HINT && OPENCV_IMPL_FP_HINTS
            details::saveFPDenormalsState(fp_denormals_base_state);
#endif

#ifdef OPENCV_TRACE
            trace_মূল_Region = CV_TRACE_NS::details::getCurrentRegion();
            trace_মূল_Context = CV_TRACE_NS::details::getTraceManager().tls.get();
#endif

#ifdef ENABLE_INSTRUMENTATION
            p_Thread_মূল = cv::instr::getInstrumentTLSStruct().p_Current_গ্রন্থি;
#endif
        }
        শূন্য finalize()
        {
#ifdef ENABLE_INSTRUMENTATION
            প্রতি(size_t i = 0; i < p_Thread_মূল->m_childs.size(); i++)
                SyncNodes(p_Thread_মূল->m_childs[i]);
#endif
            যদি (is_rng_used)
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
            যদি (trace_মূল_Region)
                CV_TRACE_NS::details::parallelForFinalize(*trace_মূল_Region);
#endif

            যদি (hasException)
            {
#যদি CV__EXCEPTION_PTR
                std::rethrow_exception(pException);
#অন্যথা
                CV_Error(Error::StsError, "Exception in parallel_for() body: " + exception_message);
#endif
            }
        }
        ~ParallelLoopBodyWrapperContext() {}

        ধ্রুব cv::ParallelLoopBody* body;
        cv::Range wholeRange;
        পূর্ণাঙ্ক nstripes;
        cv::RNG rng;
        mutable তার্কিক is_rng_used;
#ifdef OPENCV_TRACE
        CV_TRACE_NS::details::Region* trace_মূল_Region;
        CV_TRACE_NS::details::TraceManagerThreadLocal* trace_মূল_Context;
#endif
#ifdef ENABLE_INSTRUMENTATION
        cv::instr::Instr_গ্রন্থি *p_Thread_মূল;
#endif
        তার্কিক hasException;
#যদি CV__EXCEPTION_PTR
        std::exception_ptr pException;
#অন্যথা
        cv::String exception_message;
#endif
#যদি CV__EXCEPTION_PTR
        শূন্য recordException()
#অন্যথা
        শূন্য recordException(ধ্রুব cv::String& বার্তা)
#endif
        {
#ifndef CV_THREAD_SANITIZER
            যদি (!hasException)
#endif
            {
                cv::AutoLock lock(cv::getInitializationMutex());
                যদি (!hasException)
                {
                    hasException = সত্য;
#যদি CV__EXCEPTION_PTR
                    pException = std::current_exception();
#অন্যথা
                    exception_message = বার্তা;
#endif
                }
            }
        }

#যদি OPENCV_SUPPORTS_FP_DENORMALS_HINT && OPENCV_IMPL_FP_HINTS
        details::FPDenormalsModeState fp_denormals_base_state;
#endif

    গুপ্ত:
        ParallelLoopBodyWrapperContext(ধ্রুব ParallelLoopBodyWrapperContext&); // disabled
        ParallelLoopBodyWrapperContext& operator=(ধ্রুব ParallelLoopBodyWrapperContext&); // disabled
    };

    বর্গ ParallelLoopBodyWrapper : সার্বজনিক cv::ParallelLoopBody
    {
    সার্বজনিক:
        ParallelLoopBodyWrapper(ParallelLoopBodyWrapperContext& ctx_) :
            ctx(ctx_)
        {
        }
        ~ParallelLoopBodyWrapper()
        {
        }
        শূন্য operator()(ধ্রুব cv::Range& sr) ধ্রুব CV_OVERRIDE
        {
#ifdef OPENCV_TRACE
            // TODO CV_TRACE_NS::details::setCurrentRegion(rootRegion);
            যদি (ctx.trace_মূল_Region && ctx.trace_মূল_Context)
                CV_TRACE_NS::details::parallel_For_Set_মূল_Region(*ctx.trace_মূল_Region, *ctx.trace_মূল_Context);
            CV__TRACE_OPENCV_FUNCTION_NAME("parallel_for_body");
            যদি (ctx.trace_মূল_Region)
                CV_TRACE_NS::details::parallelForAttachNestedRegion(*ctx.trace_মূল_Region);
#endif

#ifdef ENABLE_INSTRUMENTATION
            {
                cv::instr::InstrTLSStruct *pInstrTLS = &cv::instr::getInstrumentTLSStruct();
                pInstrTLS->p_Current_গ্রন্থি = ctx.p_Thread_মূল; // Initialize TLS node for thread
            }
            CV_INSTRUMENT_REGION();
#endif

            // propagate main thread state
            cv::theRNG() = ctx.rng;
#যদি OPENCV_SUPPORTS_FP_DENORMALS_HINT && OPENCV_IMPL_FP_HINTS
            FPDenormalsIgnoreHintScope fp_denormals_scope(ctx.fp_denormals_base_state);
#endif

            cv::Range r;
            cv::Range wholeRange = ctx.wholeRange;
            পূর্ণাঙ্ক nstripes = ctx.nstripes;
            r.start = (পূর্ণাঙ্ক)(wholeRange.start +
                            ((uint64)sr.start*(wholeRange.end - wholeRange.start) + nstripes/2)/nstripes);
            r.end = sr.end >= nstripes ? wholeRange.end : (পূর্ণাঙ্ক)(wholeRange.start +
                            ((uint64)sr.end*(wholeRange.end - wholeRange.start) + nstripes/2)/nstripes);

#ifdef OPENCV_TRACE
            CV_TRACE_ARG_মান(range_start, "range.start", (int64)r.start);
            CV_TRACE_ARG_মান(range_end, "range.end", (int64)r.end);
#endif

            যত্ন
            {
                (*ctx.body)(r);
            }
#যদি CV__EXCEPTION_PTR
            গৃহাণ (...)
            {
                ctx.recordException();
            }
#অন্যথা
            গৃহাণ (ধ্রুব cv::Exception& e)
            {
                ctx.recordException(e.what());
            }
            গৃহাণ (ধ্রুব std::exception& e)
            {
                ctx.recordException(e.what());
            }
            গৃহাণ (...)
            {
                ctx.recordException("Unknown exception");
            }
#endif

            যদি (!ctx.is_rng_used && !(cv::theRNG() == ctx.rng))
                ctx.is_rng_used = সত্য;
        }
        cv::Range stripeRange() ধ্রুব { প্রত্যাগম cv::Range(0, ctx.nstripes); }

    রক্ষিত:
        ParallelLoopBodyWrapperContext& ctx;
    };

#যদি defined HAVE_TBB
    বর্গ ProxyLoopBody : সার্বজনিক ParallelLoopBodyWrapper
    {
    সার্বজনিক:
        ProxyLoopBody(ParallelLoopBodyWrapperContext& ctx_)
        : ParallelLoopBodyWrapper(ctx_)
        {}

        শূন্য operator ()(ধ্রুব tbb::blocked_range<পূর্ণাঙ্ক>& range) ধ্রুব
        {
            এতত্->ParallelLoopBodyWrapper::operator()(cv::Range(range.begin(), range.end()));
        }

        শূন্য operator ()() ধ্রুব  // run parallel job
        {
            cv::Range range = এতত্->stripeRange();
            tbb::parallel_for(tbb::blocked_range<পূর্ণাঙ্ক>(range.start, range.end), *এতত্);
        }
    };
#নোচেদ্যদি defined HAVE_HPX
    বর্গ ProxyLoopBody : সার্বজনিক ParallelLoopBodyWrapper
    {
    সার্বজনিক:
        ProxyLoopBody(ParallelLoopBodyWrapperContext& ctx_)
                : ParallelLoopBodyWrapper(ctx_)
        {}

        শূন্য operator ()() ধ্রুব  // run parallel job
        {
            cv::Range stripeRange = এতত্->stripeRange();
            hpx::parallel::for_loop(
                    hpx::parallel::execution::par,
                    stripeRange.start, stripeRange.end,
                    [&](ধ্রুব পূর্ণাঙ্ক &i) { ;
                        এতত্->ParallelLoopBodyWrapper::operator()(
                                cv::Range(i, i + 1));
                    });
        }
    };
#নোচেদ্যদি defined HAVE_OPENMP
    প্রকারনির্দেশ ParallelLoopBodyWrapper ProxyLoopBody;
#নোচেদ্যদি defined HAVE_GCD
    প্রকারনির্দেশ ParallelLoopBodyWrapper ProxyLoopBody;
    স্থির শূন্য block_function(শূন্য* context, size_t index)
    {
        ProxyLoopBody* ptr_body = static_cast<ProxyLoopBody*>(context);
        (*ptr_body)(cv::Range((পূর্ণাঙ্ক)index, (পূর্ণাঙ্ক)index + 1));
    }
#নোচেদ্যদি defined WINRT || defined HAVE_CONCURRENCY
    বর্গ ProxyLoopBody : সার্বজনিক ParallelLoopBodyWrapper
    {
    সার্বজনিক:
        ProxyLoopBody(ParallelLoopBodyWrapperContext& ctx)
        : ParallelLoopBodyWrapper(ctx)
        {}

        শূন্য operator ()(পূর্ণাঙ্ক i) ধ্রুব
        {
            এতত্->ParallelLoopBodyWrapper::operator()(cv::Range(i, i + 1));
        }
    };

    // PPL's default auto_partitioner silently skips whole chunks of the range on
    // ARM64 (unfixed MSVC bug, developercommunity.visualstudio.com/t/1027444).
    template <typename Body>
    স্থির অন্তঃস্থ শূন্য pplParallelFor(পূর্ণাঙ্ক first, পূর্ণাঙ্ক last, ধ্রুব Body& body)
    {
#যদি defined(_M_ARM64) || defined(_M_ARM64EC)
        Concurrency::parallel_for(first, last, body, Concurrency::static_partitioner());
#অন্যথা
        Concurrency::parallel_for(first, last, body);
#endif
    }
#অন্যথা
    প্রকারনির্দেশ ParallelLoopBodyWrapper ProxyLoopBody;
#endif

#যদি defined HAVE_TBB
    #যদি TBB_INTERFACE_VERSION >= 8000
        স্থির tbb::task_arena tbbArena(tbb::task_arena::automatic);
    #অন্যথা
        স্থির tbb::task_scheduler_init tbbScheduler(tbb::task_scheduler_init::deferred);
    #endif
#নোচেদ্যদি defined HAVE_HPX
// nothing for HPX
#নোচেদ্যদি defined HAVE_OPENMP
স্থির অন্তঃস্থ পূর্ণাঙ্ক _initMaxThreads()
{
    পূর্ণাঙ্ক maxThreads = omp_get_max_threads();
    প্রত্যাগম maxThreads;
}
স্থির পূর্ণাঙ্ক অঙ্ক_Threads_Max = _initMaxThreads();
#নোচেদ্যদি defined HAVE_GCD
// nothing for GCD
#নোচেদ্যদি defined WINRT
// nothing for WINRT
#নোচেদ্যদি defined HAVE_CONCURRENCY

বর্গ SchedPtr
{
    Concurrency::Scheduler* sched_;
সার্বজনিক:
    Concurrency::Scheduler* operator->() { প্রত্যাগম sched_; }
    operator Concurrency::Scheduler*() { প্রত্যাগম sched_; }

    শূন্য operator=(Concurrency::Scheduler* sched)
    {
        যদি (sched_) sched_->Release();
        sched_ = sched;
    }

    SchedPtr() : sched_(0) {}
    ~SchedPtr() {}
};
স্থির SchedPtr pplScheduler;

#endif

} // namespace anon

/* ================================   parallel_for_  ================================ */

স্থির শূন্য parallel_for_impl(ধ্রুব cv::Range& range, ধ্রুব cv::ParallelLoopBody& body, দ্বিগুণ nstripes); // forward declaration

শূন্য parallel_for_(ধ্রুব cv::Range& range, ধ্রুব cv::ParallelLoopBody& body, দ্বিগুণ nstripes)
{
#ifdef OPENCV_TRACE
    CV__TRACE_OPENCV_FUNCTION_NAME_("parallel_for", 0);
    CV_TRACE_ARG_মান(range_start, "range.start", (int64)range.start);
    CV_TRACE_ARG_মান(range_end, "range.end", (int64)range.end);
    CV_TRACE_ARG_মান(nstripes, "nstripes", (int64)nstripes);
#endif

    CV_INSTRUMENT_REGION_MT_FORK();
    যদি (range.empty())
        প্রত্যাগম;

    স্থির std::atomic<তার্কিক> flagNestedParallelFor(অসত্য);
    তার্কিক isNotNestedRegion = !flagNestedParallelFor.load();
    যদি (isNotNestedRegion)
      isNotNestedRegion = !flagNestedParallelFor.exchange(সত্য);
    যদি (isNotNestedRegion)
    {
        যত্ন
        {
            parallel_for_impl(range, body, nstripes);
            flagNestedParallelFor = অসত্য;
        }
        গৃহাণ (...)
        {
            flagNestedParallelFor = অসত্য;
            ক্ষিপ;
        }
    }
    অন্যথা // nested parallel_for_() calls are not parallelized
    {
        CV_UNUSED(nstripes);
        body(range);
    }
}

স্থির
শূন্য parallel_for_cb(পূর্ণাঙ্ক start, পূর্ণাঙ্ক end, শূন্য* data)
{
    CV_DbgAssert(data);
    ধ্রুব cv::ParallelLoopBody& body = *(ধ্রুব cv::ParallelLoopBody*)data;
    body(Range(start, end));
}

স্থির শূন্য parallel_for_impl(ধ্রুব cv::Range& range, ধ্রুব cv::ParallelLoopBody& body, দ্বিগুণ nstripes)
{
    using নামাকাশ cv::parallel;
    যদি ((অঙ্ক_Threads < 0 || অঙ্ক_Threads > 1) && range.end - range.start > 1)
    {
        ParallelLoopBodyWrapperContext ctx(body, range, nstripes);
        ProxyLoopBody pbody(ctx);
        cv::Range stripeRange = pbody.stripeRange();
        যদি( stripeRange.end - stripeRange.start == 1 )
        {
            body(range);
            প্রত্যাগম;
        }

        std::shared_ptr<ParallelForAPI>& api = getCurrentParallelForAPI();
        যদি (api)
        {
            CV_CheckEQ(stripeRange.start, 0, "");
            api->parallel_for(stripeRange.end, parallel_for_cb, (শূন্য*)&pbody);
            ctx.finalize();  // propagate exceptions if exists
            প্রত্যাগম;
        }

#ifdef CV_PARALLEL_FRAMEWORK
#যদি defined HAVE_TBB

#যদি TBB_INTERFACE_VERSION >= 8000
        tbbArena.execute(pbody);
#অন্যথা
        pbody();
#endif

#নোচেদ্যদি defined HAVE_HPX
        pbody();

#নোচেদ্যদি defined HAVE_OPENMP

        #pragma omp parallel প্রতি schedule(dynamic) অঙ্ক_threads(অঙ্ক_Threads > 0 ? অঙ্ক_Threads : অঙ্ক_Threads_Max)
        প্রতি (পূর্ণাঙ্ক i = stripeRange.start; i < stripeRange.end; ++i)
            pbody(Range(i, i + 1));

#নোচেদ্যদি defined HAVE_GCD

        dispatch_queue_t concurrent_queue = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0);
        dispatch_apply_f(stripeRange.end - stripeRange.start, concurrent_queue, &pbody, block_function);

#নোচেদ্যদি defined WINRT

        pplParallelFor(stripeRange.start, stripeRange.end, pbody);

#নোচেদ্যদি defined HAVE_CONCURRENCY

        যদি(!pplScheduler || pplScheduler->Id() == Concurrency::CurrentScheduler::Id())
        {
            pplParallelFor(stripeRange.start, stripeRange.end, pbody);
        }
        অন্যথা
        {
            pplScheduler->Attach();
            pplParallelFor(stripeRange.start, stripeRange.end, pbody);
            Concurrency::CurrentScheduler::Detach();
        }

#নোচেদ্যদি defined HAVE_PTHREADS_PF

        parallel_for_pthreads(pbody.stripeRange(), pbody, pbody.stripeRange().size());

#অন্যথা

#error You have hacked and compiling with unsupported parallel framework

#endif

        ctx.finalize();  // propagate exceptions if exists
        প্রত্যাগম;
#endif // CV_PARALLEL_FRAMEWORK
    }

    body(range);
}


পূর্ণাঙ্ক get_অঙ্ক_Threads(শূন্য)
{
    std::shared_ptr<ParallelForAPI>& api = getCurrentParallelForAPI();
    যদি (api)
    {
        প্রত্যাগম api->get_অঙ্ক_Threads();
    }

    যদি (অঙ্ক_Threads == 0)
        প্রত্যাগম 1;

#যদি defined HAVE_TBB

#যদি TBB_INTERFACE_VERSION >= 9100
    প্রত্যাগম tbbArena.max_concurrency();
#নোচেদ্যদি TBB_INTERFACE_VERSION >= 8000
    প্রত্যাগম অঙ্ক_Threads > 0
        ? অঙ্ক_Threads
        : tbb::task_scheduler_init::default_অঙ্ক_threads();
#অন্যথা
    প্রত্যাগম tbbScheduler.is_active()
           ? অঙ্ক_Threads
           : tbb::task_scheduler_init::default_অঙ্ক_threads();
#endif

#নোচেদ্যদি defined HAVE_HPX
    প্রত্যাগম অঙ্ক_Threads;

#নোচেদ্যদি defined HAVE_OPENMP

    প্রত্যাগম অঙ্ক_Threads > 0
           ? অঙ্ক_Threads
           : অঙ্ক_Threads_Max;


#নোচেদ্যদি defined HAVE_GCD

    প্রত্যাগম cv::get_সংখ্যা_Of_CPUs(); // the GCD thread pool limit

#নোচেদ্যদি defined WINRT

    প্রত্যাগম 0;

#নোচেদ্যদি defined HAVE_CONCURRENCY

    প্রত্যাগম (pplScheduler == 0)
        ? Concurrency::CurrentScheduler::Get()->Get_সংখ্যা_Of_Virtual_Processors()
        : (1 + pplScheduler->Get_সংখ্যা_Of_Virtual_Processors());

#নোচেদ্যদি defined HAVE_PTHREADS_PF

        প্রত্যাগম parallel_pthreads_get_threads_অঙ্ক();

#অন্যথা

    প্রত্যাগম 1;

#endif
}

unsigned default_সংখ্যা_Of_Threads()
{
#ifdef __ANDROID__
    // many modern phones/tables have 4-core CPUs. Let's use no more
    // than 2 threads by default not to overheat the devices
    ধ্রুব unsigned পূর্ণাঙ্ক default_সংখ্যা_of_threads = 2;
#অন্যথা
    ধ্রুব unsigned পূর্ণাঙ্ক default_সংখ্যা_of_threads = (unsigned পূর্ণাঙ্ক)std::max(1, cv::get_সংখ্যা_Of_CPUs());
#endif

    unsigned result = default_সংখ্যা_of_threads;

    স্থির পূর্ণাঙ্ক config_অঙ্ক_threads = (পূর্ণাঙ্ক)utils::getConfigurationParameterSizeT("OPENCV_FOR_THREADS_NUM", 0);

    যদি (config_অঙ্ক_threads)
    {
        result = (unsigned)std::max(1, config_অঙ্ক_threads);
        //do we need upper limit of threads number?
    }
    প্রত্যাগম result;
}

শূন্য set_অঙ্ক_Threads( পূর্ণাঙ্ক threads_ )
{
    CV_UNUSED(threads_);

    পূর্ণাঙ্ক threads = (threads_ < 0) ? default_সংখ্যা_Of_Threads() : (unsigned)threads_;
    অঙ্ক_Threads = threads;

    std::shared_ptr<ParallelForAPI>& api = getCurrentParallelForAPI();
    যদি (api)
    {
        api->set_অঙ্ক_Threads(অঙ্ক_Threads);
    }

#ifdef HAVE_TBB

#যদি TBB_INTERFACE_VERSION >= 8000
    যদি(tbbArena.is_active()) tbbArena.terminate();
    যদি(threads > 0) tbbArena.initialize(threads);
#অন্যথা
    যদি(tbbScheduler.is_active()) tbbScheduler.terminate();
    যদি(threads > 0) tbbScheduler.initialize(threads);
#endif

#নোচেদ্যদি defined HAVE_HPX
    প্রত্যাগম; // nothing needed as numThreads is used

#নোচেদ্যদি defined HAVE_OPENMP

    প্রত্যাগম; // nothing needed as num_threads clause is used in #pragma omp parallel for

#নোচেদ্যদি defined HAVE_GCD

    // unsupported
    // there is only private dispatch_queue_set_width() and only for desktop

#নোচেদ্যদি defined WINRT

    প্রত্যাগম;

#নোচেদ্যদি defined HAVE_CONCURRENCY

    যদি (threads <= 0)
    {
        pplScheduler = 0;
    }
    অন্যথা যদি (threads == 1)
    {
        // Concurrency always uses >=2 threads, so we just disable it if 1 thread is requested
        অঙ্ক_Threads = 0;
    }
    অন্যথা যদি (pplScheduler == 0 || 1 + pplScheduler->Get_সংখ্যা_Of_Virtual_Processors() != (unsigned পূর্ণাঙ্ক)threads)
    {
        pplScheduler = Concurrency::Scheduler::Create(Concurrency::SchedulerPolicy(2,
                       Concurrency::MinConcurrency, threads-1,
                       Concurrency::MaxConcurrency, threads-1));
    }

#নোচেদ্যদি defined HAVE_PTHREADS_PF

    parallel_pthreads_set_threads_অঙ্ক(threads);

#endif
}


পূর্ণাঙ্ক get_Thread_অঙ্ক()
{
    std::shared_ptr<ParallelForAPI>& api = getCurrentParallelForAPI();
    যদি (api)
    {
        প্রত্যাগম api->get_Thread_অঙ্ক();
    }

#যদি defined HAVE_TBB
    #যদি TBB_INTERFACE_VERSION >= 9100
        প্রত্যাগম tbb::this_task_arena::current_thread_index();
    #নোচেদ্যদি TBB_INTERFACE_VERSION >= 8000
        প্রত্যাগম tbb::task_arena::current_thread_index();
    #অন্যথা
        প্রত্যাগম 0;
    #endif
#নোচেদ্যদি defined HAVE_HPX
    প্রত্যাগম (পূর্ণাঙ্ক)(hpx::get_অঙ্ক_worker_threads());
#নোচেদ্যদি defined HAVE_OPENMP
    প্রত্যাগম omp_get_thread_অঙ্ক();
#নোচেদ্যদি defined HAVE_GCD
    প্রত্যাগম (পূর্ণাঙ্ক)(size_t)(শূন্য*)pthread_self(); // no zero-based indexing
#নোচেদ্যদি defined WINRT
    প্রত্যাগম 0;
#নোচেদ্যদি defined HAVE_CONCURRENCY
    প্রত্যাগম std::max(0, (পূর্ণাঙ্ক)Concurrency::Context::VirtualProcessorId()); // zero for master thread, unique number for others but not necessary 1,2,3,...
#নোচেদ্যদি defined HAVE_PTHREADS_PF
    প্রত্যাগম (পূর্ণাঙ্ক)(size_t)(শূন্য*)pthread_self(); // no zero-based indexing
#অন্যথা
    প্রত্যাগম 0;
#endif
}


#যদি defined __linux__ || defined __GLIBC__ || defined __HAIKU__ || defined __ANDROID__
  #define CV_CPU_GROUPS_1
#endif

#যদি defined __linux__ || defined __ANDROID__
  #define CV_HAVE_CGROUPS 1
#endif

#যদি defined CV_CPU_GROUPS_1
স্থির অন্তঃস্থ
std::string getFileContents(ধ্রুব বর্ণ *filename)
{
    std::ifstream ifs(filename);
    যদি (!ifs.is_open())
        প্রত্যাগম std::string();

    std::string content( (std::istreambuf_iterator<বর্ণ>(ifs) ),
                         (std::istreambuf_iterator<বর্ণ>()    ) );

    যদি (ifs.fail())
        প্রত্যাগম std::string();

    প্রত্যাগম content;
}

স্থির অন্তঃস্থ
পূর্ণাঙ্ক get_সংখ্যা_Of_CPUs_Impl(ধ্রুব বর্ণ *filename)
{
   std::string file_contents = getFileContents(filename);
   যদি(file_contents.empty())
       প্রত্যাগম 0;

   বর্ণ *pbuf = const_cast<বর্ণ*>(file_contents.c_str());
   //parse string of form "0-1,3,5-7,10,13-15"
   পূর্ণাঙ্ক cpusAvailable = 0;

   যাবত্(*pbuf)
   {
      ধ্রুব বর্ণ* pos = pbuf;
      তার্কিক range = অসত্য;
      যাবত্(*pbuf && *pbuf != ',')
      {
          যদি(*pbuf == '-') range = সত্য;
          ++pbuf;
      }
      যদি(*pbuf) *pbuf++ = 0;
      যদি(!range)
        ++cpusAvailable;
      অন্যথা
      {
          পূর্ণাঙ্ক rstart = 0, rend = 0;
          sscanf(pos, "%d-%d", &rstart, &rend);
          cpusAvailable += rend - rstart + 1;
      }

   }
   প্রত্যাগম cpusAvailable;
}
#endif

#যদি defined CV_HAVE_CGROUPS
স্থির অন্তঃস্থ
unsigned get_সংখ্যা_Of_CPUs_CFSv2()
{
    পূর্ণাঙ্ক cfs_quota = 0;
    পূর্ণাঙ্ক cfs_period = 0;

    std::ifstream ss_cpu_max("/sys/fs/cgroup/cpu.max", std::ios::in | std::ios::binary);
    ss_cpu_max >> cfs_quota >> cfs_period;

    যদি (ss_cpu_max.fail() || cfs_quota < 1 || cfs_period < 1) /* values must not be 0 or negative */
        প্রত্যাগম 0;

    প্রত্যাগম (unsigned)max(1, cfs_quota/cfs_period);
}

স্থির অন্তঃস্থ
unsigned get_সংখ্যা_Of_CPUs_CFSv1()
{
    পূর্ণাঙ্ক cfs_quota = 0;
    {
        std::ifstream ss_period("/sys/fs/cgroup/cpu/cpu.cfs_quota_us", std::ios::in | std::ios::binary);
        ss_period >> cfs_quota;

        যদি (ss_period.fail() || cfs_quota < 1) /* cfs_quota must not be 0 or negative */
            প্রত্যাগম 0;
    }

    পূর্ণাঙ্ক cfs_period = 0;
    {
        std::ifstream ss_quota("/sys/fs/cgroup/cpu/cpu.cfs_period_us", std::ios::in | std::ios::binary);
        ss_quota >> cfs_period;

        যদি (ss_quota.fail() || cfs_period < 1)
            প্রত্যাগম 0;
    }

    প্রত্যাগম (unsigned)max(1, cfs_quota/cfs_period);
}
#endif

template <typename T> স্থির অন্তঃস্থ
T minNonZero(ধ্রুব T& val_1, ধ্রুব T& val_2)
{
    যদি ((val_1 != 0) && (val_2 != 0))
        প্রত্যাগম std::min(val_1, val_2);
    প্রত্যাগম (val_1 != 0) ? val_1 : val_2;
}

#ifndef OPENCV_DISABLE_THREAD_SUPPORT
স্থির
পূর্ণাঙ্ক get_সংখ্যা_Of_CPUs_2()
{
#ifndef OPENCV_SEMIHOSTING
    /*
     * Logic here is to try different methods of getting CPU counts and return
     * the minimum most value as it has high probablity of being right and safe.
     * Return 1 if we get 0 or not found on all methods.
    */
#যদি !defined(__MINGW32__) /* not implemented (2020-03) */

    /*
     * Check for this standard C++11 way, we do not return directly because
     * running in a docker or K8s environment will mean this is the host
     * machines config not the containers or pods and as per docs this value
     * must be "considered only a hint".
    */
    unsigned ncpus = std::thread::hardware_concurrency(); /* If the value is not well defined or not computable, returns 0 */
#অন্যথা
    unsigned ncpus = 0; /* 0 means we have to find out some other way */
#endif

#যদি defined _WIN32

    SYSTEM_INFO sysinfo = {};
#যদি (defined(_M_ARM) || defined(_M_ARM64) || defined(_M_ARM64EC) || defined(_M_X64) || defined(WINRT)) && _WIN32_WINNT >= 0x501
    GetNativeSystemInfo( &sysinfo );
#অন্যথা
    GetSystemInfo( &sysinfo );
#endif
    unsigned ncpus_sysinfo = sysinfo.dw_সংখ্যা_Of_Processors;
    ncpus = minNonZero(ncpus, ncpus_sysinfo);

#নোচেদ্যদি defined __APPLE__

    পূর্ণাঙ্ক অঙ্ক_CPU=0;
    পূর্ণাঙ্ক mib[4];
    size_t len = পরিমাণ(অঙ্ক_CPU);

    /* set the mib for hw.ncpu */
    mib[0] = CTL_HW;
    mib[1] = HW_AVAILCPU;  // alternatively, try HW_NCPU;

    /* get the number of CPUs from the system */
    sysctl(mib, 2, &অঙ্ক_CPU, &len, রিক্ত, 0);

    যদি( অঙ্ক_CPU < 1 )
    {
        mib[1] = HW_NCPU;
        sysctl( mib, 2, &অঙ্ক_CPU, &len, রিক্ত, 0 );

        যদি( অঙ্ক_CPU < 1 )
            অঙ্ক_CPU = 1;
    }

    ncpus = minNonZero(ncpus, (unsigned)অঙ্ক_CPU);

#নোচেদ্যদি defined CV_CPU_GROUPS_1

#যদি defined CV_HAVE_CGROUPS
    স্থির unsigned ncpus_impl_cpuset = (unsigned)get_সংখ্যা_Of_CPUs_Impl("/sys/fs/cgroup/cpuset/cpuset.cpus");
    ncpus = minNonZero(ncpus, ncpus_impl_cpuset);

    স্থির unsigned ncpus_impl_cfs_v1 = get_সংখ্যা_Of_CPUs_CFSv1();
    ncpus = minNonZero(ncpus, ncpus_impl_cfs_v1);

    স্থির unsigned ncpus_impl_cfs_v2 = get_সংখ্যা_Of_CPUs_CFSv2();
    ncpus = minNonZero(ncpus, ncpus_impl_cfs_v2);
#endif

    স্থির unsigned ncpus_impl_devices = (unsigned)get_সংখ্যা_Of_CPUs_Impl("/sys/devices/system/cpu/online");
    ncpus = minNonZero(ncpus, ncpus_impl_devices);

#endif

#যদি defined _GNU_SOURCE \
    && !defined(__MINGW32__) /* not implemented (2020-03) */ \
    && !defined(__EMSCRIPTEN__) \
    && !defined(__ANDROID__)  // TODO: add check for modern Android NDK

    cpu_set_t cpu_set;
    যদি (0 == sched_getaffinity(0, পরিমাণ(cpu_set), &cpu_set))
    {
        unsigned cpu_গুনতি_cpu_set = CPU_COUNT(&cpu_set);
        ncpus = minNonZero(ncpus, cpu_গুনতি_cpu_set);
    }

#endif

#যদি !defined(_WIN32) && !defined(__APPLE__) && defined(_SC_NPROCESSORS_ONLN)

    স্থির unsigned cpu_গুনতি_sysconf = (unsigned)sysconf( _SC_NPROCESSORS_ONLN );
    ncpus = minNonZero(ncpus, cpu_গুনতি_sysconf);
#নোচেদ্যদি defined (__QNX__)
    স্থির unsigned cpu_গুনতি_sysconf = _syspage_ptr->অঙ্ক_cpu;
    ncpus = minNonZero(ncpus, cpu_গুনতি_sysconf);
#endif

    প্রত্যাগম ncpus != 0 ? ncpus : 1;
#অন্যথা //  OPENCV_SEMIHOSTING
    প্রত্যাগম 1;
#endif //OPENCV_SEMIHOSTING
}

পূর্ণাঙ্ক get_সংখ্যা_Of_CPUs()
{
    স্থির পূর্ণাঙ্ক nCPUs = get_সংখ্যা_Of_CPUs_2();
    প্রত্যাগম nCPUs;  // cached value
}

#অন্যথা  // OPENCV_DISABLE_THREAD_SUPPORT
পূর্ণাঙ্ক get_সংখ্যা_Of_CPUs()
{
    প্রত্যাগম 1;
}
#endif  // OPENCV_DISABLE_THREAD_SUPPORT

ধ্রুব বর্ণ* currentParallelFramework()
{
    std::shared_ptr<ParallelForAPI>& api = getCurrentParallelForAPI();
    যদি (api)
    {
        প্রত্যাগম api->getName();
    }
#ifdef CV_PARALLEL_FRAMEWORK
    প্রত্যাগম CV_PARALLEL_FRAMEWORK;
#অন্যথা
    প্রত্যাগম রিক্ত;
#endif
}

}  // namespace cv::
