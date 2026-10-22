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

#അന്തര്ഭാവ "precomp.hpp"

#അന്തര്ഭാവ <opencv2/core/utils/configuration.private.hpp>
#അന്തര്ഭാവ <opencv2/core/utils/trace.private.hpp>

#അന്തര്ഭാവ "opencv2/core/parallel/parallel_backend.hpp"
#അന്തര്ഭാവ "parallel/parallel.hpp"

#എങ്കിൽ defined _WIN32 || defined WINCE
    #അന്തര്ഭാവ <windows.h>
    #undef small
    #undef min
    #undef max
    #undef abs
#endif

#എങ്കിൽ defined __unix__ || defined __APPLE__ || defined __GLIBC__ \
    || defined __HAIKU__ || defined __EMSCRIPTEN__ \
    || defined __FreeBSD__ || defined __NetBSD__ || defined __OpenBSD__
    #അന്തര്ഭാവ <unistd.h>
    #അന്തര്ഭാവ <stdio.h>
    #അന്തര്ഭാവ <sys/types.h>
    #അന്തര്ഭാവ <fstream>
    #എങ്കിൽ defined __ANDROID__
        #അന്തര്ഭാവ <sys/sysconf.h>
        #അന്തര്ഭാവ <sys/syscall.h>
        #അന്തര്ഭാവ <sched.h>
    #അല്ലെങ്കിലെങ്കിൽ defined __APPLE__
        #അന്തര്ഭാവ <sys/sysctl.h>
    #endif
#endif

#എങ്കിൽ defined (__QNX__)
    #അന്തര്ഭാവ <sys/syspage.h>
#endif

#ifndef OPENCV_DISABLE_THREAD_SUPPORT
    #അന്തര്ഭാവ <thread>
#endif

#ifdef _OPENMP
    #define HAVE_OPENMP
#endif

#ifdef __APPLE__
    #define HAVE_GCD
#endif

#എങ്കിൽ defined _MSC_VER
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

#എങ്കിൽ defined HAVE_TBB
    #ifndef TBB_SUPPRESS_DEPRECATED_MESSAGES  // supress warning
    #define TBB_SUPPRESS_DEPRECATED_MESSAGES 1
    #endif
    #അന്തര്ഭാവ "tbb/tbb.h"
    #അന്തര്ഭാവ "tbb/task.h"
    #എങ്കിൽ TBB_INTERFACE_VERSION >= 8000
        #അന്തര്ഭാവ "tbb/task_arena.h"
    #endif
    #undef min
    #undef max
#അല്ലെങ്കിലെങ്കിൽ defined HAVE_HPX
    #അന്തര്ഭാവ <hpx/parallel/algorithms/for_loop.hpp>
    #അന്തര്ഭാവ <hpx/parallel/execution.hpp>
    //
    #അന്തര്ഭാവ <hpx/hpx_start.hpp>
    #അന്തര്ഭാവ <hpx/hpx_suspend.hpp>
    #അന്തര്ഭാവ <hpx/include/apply.hpp>
    #അന്തര്ഭാവ <hpx/util/yield_while.hpp>
    #അന്തര്ഭാവ <hpx/include/threadmanager.hpp>

#അല്ലെങ്കിലെങ്കിൽ defined HAVE_OPENMP
    #അന്തര്ഭാവ <omp.h>
#അല്ലെങ്കിലെങ്കിൽ defined HAVE_GCD
    #അന്തര്ഭാവ <dispatch/dispatch.h>
    #അന്തര്ഭാവ <pthread.h>
#അല്ലെങ്കിലെങ്കിൽ defined HAVE_CONCURRENCY
    #അന്തര്ഭാവ <ppl.h>
#അല്ലെങ്കിലെങ്കിൽ defined HAVE_PTHREADS_PF
    #അന്തര്ഭാവ <pthread.h>
#endif


#എങ്കിൽ defined HAVE_TBB
#  define CV_PARALLEL_FRAMEWORK "tbb"
#അല്ലെങ്കിലെങ്കിൽ defined HAVE_HPX
#  define CV_PARALLEL_FRAMEWORK "hpx"
#അല്ലെങ്കിലെങ്കിൽ defined HAVE_OPENMP
#  define CV_PARALLEL_FRAMEWORK "openmp"
#അല്ലെങ്കിലെങ്കിൽ defined HAVE_GCD
#  define CV_PARALLEL_FRAMEWORK "gcd"
#അല്ലെങ്കിലെങ്കിൽ defined WINRT
#  define CV_PARALLEL_FRAMEWORK "winrt-concurrency"
#അല്ലെങ്കിലെങ്കിൽ defined HAVE_CONCURRENCY
#  define CV_PARALLEL_FRAMEWORK "ms-concurrency"
#അല്ലെങ്കിലെങ്കിൽ defined HAVE_PTHREADS_PF
#  define CV_PARALLEL_FRAMEWORK "pthreads"
#endif

#അന്തര്ഭാവ <atomic>

#അന്തര്ഭാവ "parallel_impl.hpp"

#അന്തര്ഭാവ "opencv2/core/detail/exception_ptr.hpp"  // CV__EXCEPTION_PTR = 1 if std::exception_ptr is available

#അന്തര്ഭാവ <opencv2/core/utils/fp_control_utils.hpp>
#അന്തര്ഭാവ <opencv2/core/utils/fp_control.private.hpp>

using നാമാകാശ cv;

നാമാകാശ cv {

ParallelLoopBody::~ParallelLoopBody() {}

using നാമാകാശ cv::parallel;

നാമാകാശ {

#ifdef ENABLE_INSTRUMENTATION
    സ്ഥിതം ശൂന്യം SyncNodes(cv::instr::Instr_കണ്ണി *p_കണ്ണി)
    {
        std::vector<cv::instr::കണ്ണി_Data_Tls*> data;
        p_കണ്ണി->m_payload.m_tls.gather(data);

        uint64 ticksMax = 0;
        പൂർണ്ണസംഖ്യ    threads  = 0;
        വേണ്ടി(size_t i = 0; i < data.size(); i++)
        {
            എങ്കിൽ(data[i] && data[i]->m_ticksTotal)
            {
                ticksMax = MAX(ticksMax, data[i]->m_ticksTotal);
                p_കണ്ണി->m_payload.m_ticksTotal -= data[i]->m_ticksTotal;
                data[i]->m_ticksTotal = 0;
                threads++;
            }
        }
        p_കണ്ണി->m_payload.m_ticksTotal += ticksMax;
        p_കണ്ണി->m_payload.m_threads = MAX(p_കണ്ണി->m_payload.m_threads, threads);

        വേണ്ടി(size_t i = 0; i < p_കണ്ണി->m_childs.size(); i++)
            SyncNodes(p_കണ്ണി->m_childs[i]);
    }
#endif

    വർഗ്ഗം ParallelLoopBodyWrapperContext
    {
    പൊതു:
        ParallelLoopBodyWrapperContext(സ്ഥിരം cv::ParallelLoopBody& _body, സ്ഥിരം cv::Range& _r, ഇരട്ട _nstripes) :
            is_rng_used(അസത്യം), hasException(അസത്യം)
        {

            body = &_body;
            wholeRange = _r;
            ഇരട്ട len = wholeRange.end - wholeRange.start;
            nstripes = cvRound(_nstripes <= 0 ? len : MIN(MAX(_nstripes, 1.), len));

            // propagate main thread state
            rng = cv::theRNG();
#എങ്കിൽ OPENCV_SUPPORTS_FP_DENORMALS_HINT && OPENCV_IMPL_FP_HINTS
            details::saveFPDenormalsState(fp_denormals_base_state);
#endif

#ifdef OPENCV_TRACE
            trace_വേര്_Region = CV_TRACE_NS::details::getCurrentRegion();
            trace_വേര്_Context = CV_TRACE_NS::details::getTraceManager().tls.get();
#endif

#ifdef ENABLE_INSTRUMENTATION
            p_Thread_വേര് = cv::instr::getInstrumentTLSStruct().p_Current_കണ്ണി;
#endif
        }
        ശൂന്യം finalize()
        {
#ifdef ENABLE_INSTRUMENTATION
            വേണ്ടി(size_t i = 0; i < p_Thread_വേര്->m_childs.size(); i++)
                SyncNodes(p_Thread_വേര്->m_childs[i]);
#endif
            എങ്കിൽ (is_rng_used)
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
            എങ്കിൽ (trace_വേര്_Region)
                CV_TRACE_NS::details::parallelForFinalize(*trace_വേര്_Region);
#endif

            എങ്കിൽ (hasException)
            {
#എങ്കിൽ CV__EXCEPTION_PTR
                std::rethrow_exception(pException);
#അല്ലെങ്കിൽ
                CV_Error(Error::StsError, "Exception in parallel_for() body: " + exception_message);
#endif
            }
        }
        ~ParallelLoopBodyWrapperContext() {}

        സ്ഥിരം cv::ParallelLoopBody* body;
        cv::Range wholeRange;
        പൂർണ്ണസംഖ്യ nstripes;
        cv::RNG rng;
        mutable തർക്കം is_rng_used;
#ifdef OPENCV_TRACE
        CV_TRACE_NS::details::Region* trace_വേര്_Region;
        CV_TRACE_NS::details::TraceManagerThreadLocal* trace_വേര്_Context;
#endif
#ifdef ENABLE_INSTRUMENTATION
        cv::instr::Instr_കണ്ണി *p_Thread_വേര്;
#endif
        തർക്കം hasException;
#എങ്കിൽ CV__EXCEPTION_PTR
        std::exception_ptr pException;
#അല്ലെങ്കിൽ
        cv::String exception_message;
#endif
#എങ്കിൽ CV__EXCEPTION_PTR
        ശൂന്യം recordException()
#അല്ലെങ്കിൽ
        ശൂന്യം recordException(സ്ഥിരം cv::String& സന്ദേശം)
#endif
        {
#ifndef CV_THREAD_SANITIZER
            എങ്കിൽ (!hasException)
#endif
            {
                cv::AutoLock lock(cv::getInitializationMutex());
                എങ്കിൽ (!hasException)
                {
                    hasException = സത്യം;
#എങ്കിൽ CV__EXCEPTION_PTR
                    pException = std::current_exception();
#അല്ലെങ്കിൽ
                    exception_message = സന്ദേശം;
#endif
                }
            }
        }

#എങ്കിൽ OPENCV_SUPPORTS_FP_DENORMALS_HINT && OPENCV_IMPL_FP_HINTS
        details::FPDenormalsModeState fp_denormals_base_state;
#endif

    സ്വകാര്യ:
        ParallelLoopBodyWrapperContext(സ്ഥിരം ParallelLoopBodyWrapperContext&); // disabled
        ParallelLoopBodyWrapperContext& operator=(സ്ഥിരം ParallelLoopBodyWrapperContext&); // disabled
    };

    വർഗ്ഗം ParallelLoopBodyWrapper : പൊതു cv::ParallelLoopBody
    {
    പൊതു:
        ParallelLoopBodyWrapper(ParallelLoopBodyWrapperContext& ctx_) :
            ctx(ctx_)
        {
        }
        ~ParallelLoopBodyWrapper()
        {
        }
        ശൂന്യം operator()(സ്ഥിരം cv::Range& sr) സ്ഥിരം CV_OVERRIDE
        {
#ifdef OPENCV_TRACE
            // TODO CV_TRACE_NS::details::setCurrentRegion(rootRegion);
            എങ്കിൽ (ctx.trace_വേര്_Region && ctx.trace_വേര്_Context)
                CV_TRACE_NS::details::parallel_For_Set_വേര്_Region(*ctx.trace_വേര്_Region, *ctx.trace_വേര്_Context);
            CV__TRACE_OPENCV_FUNCTION_NAME("parallel_for_body");
            എങ്കിൽ (ctx.trace_വേര്_Region)
                CV_TRACE_NS::details::parallelForAttachNestedRegion(*ctx.trace_വേര്_Region);
#endif

#ifdef ENABLE_INSTRUMENTATION
            {
                cv::instr::InstrTLSStruct *pInstrTLS = &cv::instr::getInstrumentTLSStruct();
                pInstrTLS->p_Current_കണ്ണി = ctx.p_Thread_വേര്; // Initialize TLS node for thread
            }
            CV_INSTRUMENT_REGION();
#endif

            // propagate main thread state
            cv::theRNG() = ctx.rng;
#എങ്കിൽ OPENCV_SUPPORTS_FP_DENORMALS_HINT && OPENCV_IMPL_FP_HINTS
            FPDenormalsIgnoreHintScope fp_denormals_scope(ctx.fp_denormals_base_state);
#endif

            cv::Range r;
            cv::Range wholeRange = ctx.wholeRange;
            പൂർണ്ണസംഖ്യ nstripes = ctx.nstripes;
            r.start = (പൂർണ്ണസംഖ്യ)(wholeRange.start +
                            ((uint64)sr.start*(wholeRange.end - wholeRange.start) + nstripes/2)/nstripes);
            r.end = sr.end >= nstripes ? wholeRange.end : (പൂർണ്ണസംഖ്യ)(wholeRange.start +
                            ((uint64)sr.end*(wholeRange.end - wholeRange.start) + nstripes/2)/nstripes);

#ifdef OPENCV_TRACE
            CV_TRACE_ARG_മൂല്യം(range_start, "range.start", (int64)r.start);
            CV_TRACE_ARG_മൂല്യം(range_end, "range.end", (int64)r.end);
#endif

            ശ്രമിക്കുക
            {
                (*ctx.body)(r);
            }
#എങ്കിൽ CV__EXCEPTION_PTR
            പിടിക്കുക (...)
            {
                ctx.recordException();
            }
#അല്ലെങ്കിൽ
            പിടിക്കുക (സ്ഥിരം cv::Exception& e)
            {
                ctx.recordException(e.what());
            }
            പിടിക്കുക (സ്ഥിരം std::exception& e)
            {
                ctx.recordException(e.what());
            }
            പിടിക്കുക (...)
            {
                ctx.recordException("Unknown exception");
            }
#endif

            എങ്കിൽ (!ctx.is_rng_used && !(cv::theRNG() == ctx.rng))
                ctx.is_rng_used = സത്യം;
        }
        cv::Range stripeRange() സ്ഥിരം { തിരികെ cv::Range(0, ctx.nstripes); }

    സംരക്ഷിത:
        ParallelLoopBodyWrapperContext& ctx;
    };

#എങ്കിൽ defined HAVE_TBB
    വർഗ്ഗം ProxyLoopBody : പൊതു ParallelLoopBodyWrapper
    {
    പൊതു:
        ProxyLoopBody(ParallelLoopBodyWrapperContext& ctx_)
        : ParallelLoopBodyWrapper(ctx_)
        {}

        ശൂന്യം operator ()(സ്ഥിരം tbb::blocked_range<പൂർണ്ണസംഖ്യ>& range) സ്ഥിരം
        {
            ഇത്->ParallelLoopBodyWrapper::operator()(cv::Range(range.begin(), range.end()));
        }

        ശൂന്യം operator ()() സ്ഥിരം  // run parallel job
        {
            cv::Range range = ഇത്->stripeRange();
            tbb::parallel_for(tbb::blocked_range<പൂർണ്ണസംഖ്യ>(range.start, range.end), *ഇത്);
        }
    };
#അല്ലെങ്കിലെങ്കിൽ defined HAVE_HPX
    വർഗ്ഗം ProxyLoopBody : പൊതു ParallelLoopBodyWrapper
    {
    പൊതു:
        ProxyLoopBody(ParallelLoopBodyWrapperContext& ctx_)
                : ParallelLoopBodyWrapper(ctx_)
        {}

        ശൂന്യം operator ()() സ്ഥിരം  // run parallel job
        {
            cv::Range stripeRange = ഇത്->stripeRange();
            hpx::parallel::for_loop(
                    hpx::parallel::execution::par,
                    stripeRange.start, stripeRange.end,
                    [&](സ്ഥിരം പൂർണ്ണസംഖ്യ &i) { ;
                        ഇത്->ParallelLoopBodyWrapper::operator()(
                                cv::Range(i, i + 1));
                    });
        }
    };
#അല്ലെങ്കിലെങ്കിൽ defined HAVE_OPENMP
    തരനിർവചനം ParallelLoopBodyWrapper ProxyLoopBody;
#അല്ലെങ്കിലെങ്കിൽ defined HAVE_GCD
    തരനിർവചനം ParallelLoopBodyWrapper ProxyLoopBody;
    സ്ഥിതം ശൂന്യം block_function(ശൂന്യം* context, size_t index)
    {
        ProxyLoopBody* ptr_body = static_cast<ProxyLoopBody*>(context);
        (*ptr_body)(cv::Range((പൂർണ്ണസംഖ്യ)index, (പൂർണ്ണസംഖ്യ)index + 1));
    }
#അല്ലെങ്കിലെങ്കിൽ defined WINRT || defined HAVE_CONCURRENCY
    വർഗ്ഗം ProxyLoopBody : പൊതു ParallelLoopBodyWrapper
    {
    പൊതു:
        ProxyLoopBody(ParallelLoopBodyWrapperContext& ctx)
        : ParallelLoopBodyWrapper(ctx)
        {}

        ശൂന്യം operator ()(പൂർണ്ണസംഖ്യ i) സ്ഥിരം
        {
            ഇത്->ParallelLoopBodyWrapper::operator()(cv::Range(i, i + 1));
        }
    };

    // PPL's default auto_partitioner silently skips whole chunks of the range on
    // ARM64 (unfixed MSVC bug, developercommunity.visualstudio.com/t/1027444).
    template <typename Body>
    സ്ഥിതം വരിയിൽ ശൂന്യം pplParallelFor(പൂർണ്ണസംഖ്യ first, പൂർണ്ണസംഖ്യ last, സ്ഥിരം Body& body)
    {
#എങ്കിൽ defined(_M_ARM64) || defined(_M_ARM64EC)
        Concurrency::parallel_for(first, last, body, Concurrency::static_partitioner());
#അല്ലെങ്കിൽ
        Concurrency::parallel_for(first, last, body);
#endif
    }
#അല്ലെങ്കിൽ
    തരനിർവചനം ParallelLoopBodyWrapper ProxyLoopBody;
#endif

#എങ്കിൽ defined HAVE_TBB
    #എങ്കിൽ TBB_INTERFACE_VERSION >= 8000
        സ്ഥിതം tbb::task_arena tbbArena(tbb::task_arena::automatic);
    #അല്ലെങ്കിൽ
        സ്ഥിതം tbb::task_scheduler_init tbbScheduler(tbb::task_scheduler_init::deferred);
    #endif
#അല്ലെങ്കിലെങ്കിൽ defined HAVE_HPX
// nothing for HPX
#അല്ലെങ്കിലെങ്കിൽ defined HAVE_OPENMP
സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ _initMaxThreads()
{
    പൂർണ്ണസംഖ്യ maxThreads = omp_get_max_threads();
    തിരികെ maxThreads;
}
സ്ഥിതം പൂർണ്ണസംഖ്യ അക്കം_Threads_Max = _initMaxThreads();
#അല്ലെങ്കിലെങ്കിൽ defined HAVE_GCD
// nothing for GCD
#അല്ലെങ്കിലെങ്കിൽ defined WINRT
// nothing for WINRT
#അല്ലെങ്കിലെങ്കിൽ defined HAVE_CONCURRENCY

വർഗ്ഗം SchedPtr
{
    Concurrency::Scheduler* sched_;
പൊതു:
    Concurrency::Scheduler* operator->() { തിരികെ sched_; }
    operator Concurrency::Scheduler*() { തിരികെ sched_; }

    ശൂന്യം operator=(Concurrency::Scheduler* sched)
    {
        എങ്കിൽ (sched_) sched_->Release();
        sched_ = sched;
    }

    SchedPtr() : sched_(0) {}
    ~SchedPtr() {}
};
സ്ഥിതം SchedPtr pplScheduler;

#endif

} // namespace anon

/* ================================   parallel_for_  ================================ */

സ്ഥിതം ശൂന്യം parallel_for_impl(സ്ഥിരം cv::Range& range, സ്ഥിരം cv::ParallelLoopBody& body, ഇരട്ട nstripes); // forward declaration

ശൂന്യം parallel_for_(സ്ഥിരം cv::Range& range, സ്ഥിരം cv::ParallelLoopBody& body, ഇരട്ട nstripes)
{
#ifdef OPENCV_TRACE
    CV__TRACE_OPENCV_FUNCTION_NAME_("parallel_for", 0);
    CV_TRACE_ARG_മൂല്യം(range_start, "range.start", (int64)range.start);
    CV_TRACE_ARG_മൂല്യം(range_end, "range.end", (int64)range.end);
    CV_TRACE_ARG_മൂല്യം(nstripes, "nstripes", (int64)nstripes);
#endif

    CV_INSTRUMENT_REGION_MT_FORK();
    എങ്കിൽ (range.empty())
        തിരികെ;

    സ്ഥിതം std::atomic<തർക്കം> flagNestedParallelFor(അസത്യം);
    തർക്കം isNotNestedRegion = !flagNestedParallelFor.load();
    എങ്കിൽ (isNotNestedRegion)
      isNotNestedRegion = !flagNestedParallelFor.exchange(സത്യം);
    എങ്കിൽ (isNotNestedRegion)
    {
        ശ്രമിക്കുക
        {
            parallel_for_impl(range, body, nstripes);
            flagNestedParallelFor = അസത്യം;
        }
        പിടിക്കുക (...)
        {
            flagNestedParallelFor = അസത്യം;
            എറിയുക;
        }
    }
    അല്ലെങ്കിൽ // nested parallel_for_() calls are not parallelized
    {
        CV_UNUSED(nstripes);
        body(range);
    }
}

സ്ഥിതം
ശൂന്യം parallel_for_cb(പൂർണ്ണസംഖ്യ start, പൂർണ്ണസംഖ്യ end, ശൂന്യം* data)
{
    CV_DbgAssert(data);
    സ്ഥിരം cv::ParallelLoopBody& body = *(സ്ഥിരം cv::ParallelLoopBody*)data;
    body(Range(start, end));
}

സ്ഥിതം ശൂന്യം parallel_for_impl(സ്ഥിരം cv::Range& range, സ്ഥിരം cv::ParallelLoopBody& body, ഇരട്ട nstripes)
{
    using നാമാകാശ cv::parallel;
    എങ്കിൽ ((അക്കം_Threads < 0 || അക്കം_Threads > 1) && range.end - range.start > 1)
    {
        ParallelLoopBodyWrapperContext ctx(body, range, nstripes);
        ProxyLoopBody pbody(ctx);
        cv::Range stripeRange = pbody.stripeRange();
        എങ്കിൽ( stripeRange.end - stripeRange.start == 1 )
        {
            body(range);
            തിരികെ;
        }

        std::shared_ptr<ParallelForAPI>& api = getCurrentParallelForAPI();
        എങ്കിൽ (api)
        {
            CV_CheckEQ(stripeRange.start, 0, "");
            api->parallel_for(stripeRange.end, parallel_for_cb, (ശൂന്യം*)&pbody);
            ctx.finalize();  // propagate exceptions if exists
            തിരികെ;
        }

#ifdef CV_PARALLEL_FRAMEWORK
#എങ്കിൽ defined HAVE_TBB

#എങ്കിൽ TBB_INTERFACE_VERSION >= 8000
        tbbArena.execute(pbody);
#അല്ലെങ്കിൽ
        pbody();
#endif

#അല്ലെങ്കിലെങ്കിൽ defined HAVE_HPX
        pbody();

#അല്ലെങ്കിലെങ്കിൽ defined HAVE_OPENMP

        #pragma omp parallel വേണ്ടി schedule(dynamic) അക്കം_threads(അക്കം_Threads > 0 ? അക്കം_Threads : അക്കം_Threads_Max)
        വേണ്ടി (പൂർണ്ണസംഖ്യ i = stripeRange.start; i < stripeRange.end; ++i)
            pbody(Range(i, i + 1));

#അല്ലെങ്കിലെങ്കിൽ defined HAVE_GCD

        dispatch_queue_t concurrent_queue = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0);
        dispatch_apply_f(stripeRange.end - stripeRange.start, concurrent_queue, &pbody, block_function);

#അല്ലെങ്കിലെങ്കിൽ defined WINRT

        pplParallelFor(stripeRange.start, stripeRange.end, pbody);

#അല്ലെങ്കിലെങ്കിൽ defined HAVE_CONCURRENCY

        എങ്കിൽ(!pplScheduler || pplScheduler->Id() == Concurrency::CurrentScheduler::Id())
        {
            pplParallelFor(stripeRange.start, stripeRange.end, pbody);
        }
        അല്ലെങ്കിൽ
        {
            pplScheduler->Attach();
            pplParallelFor(stripeRange.start, stripeRange.end, pbody);
            Concurrency::CurrentScheduler::Detach();
        }

#അല്ലെങ്കിലെങ്കിൽ defined HAVE_PTHREADS_PF

        parallel_for_pthreads(pbody.stripeRange(), pbody, pbody.stripeRange().size());

#അല്ലെങ്കിൽ

#error You have hacked and compiling with unsupported parallel framework

#endif

        ctx.finalize();  // propagate exceptions if exists
        തിരികെ;
#endif // CV_PARALLEL_FRAMEWORK
    }

    body(range);
}


പൂർണ്ണസംഖ്യ get_അക്കം_Threads(ശൂന്യം)
{
    std::shared_ptr<ParallelForAPI>& api = getCurrentParallelForAPI();
    എങ്കിൽ (api)
    {
        തിരികെ api->get_അക്കം_Threads();
    }

    എങ്കിൽ (അക്കം_Threads == 0)
        തിരികെ 1;

#എങ്കിൽ defined HAVE_TBB

#എങ്കിൽ TBB_INTERFACE_VERSION >= 9100
    തിരികെ tbbArena.max_concurrency();
#അല്ലെങ്കിലെങ്കിൽ TBB_INTERFACE_VERSION >= 8000
    തിരികെ അക്കം_Threads > 0
        ? അക്കം_Threads
        : tbb::task_scheduler_init::default_അക്കം_threads();
#അല്ലെങ്കിൽ
    തിരികെ tbbScheduler.is_active()
           ? അക്കം_Threads
           : tbb::task_scheduler_init::default_അക്കം_threads();
#endif

#അല്ലെങ്കിലെങ്കിൽ defined HAVE_HPX
    തിരികെ അക്കം_Threads;

#അല്ലെങ്കിലെങ്കിൽ defined HAVE_OPENMP

    തിരികെ അക്കം_Threads > 0
           ? അക്കം_Threads
           : അക്കം_Threads_Max;


#അല്ലെങ്കിലെങ്കിൽ defined HAVE_GCD

    തിരികെ cv::get_സംഖ്യ_Of_CPUs(); // the GCD thread pool limit

#അല്ലെങ്കിലെങ്കിൽ defined WINRT

    തിരികെ 0;

#അല്ലെങ്കിലെങ്കിൽ defined HAVE_CONCURRENCY

    തിരികെ (pplScheduler == 0)
        ? Concurrency::CurrentScheduler::Get()->Get_സംഖ്യ_Of_Virtual_Processors()
        : (1 + pplScheduler->Get_സംഖ്യ_Of_Virtual_Processors());

#അല്ലെങ്കിലെങ്കിൽ defined HAVE_PTHREADS_PF

        തിരികെ parallel_pthreads_get_threads_അക്കം();

#അല്ലെങ്കിൽ

    തിരികെ 1;

#endif
}

unsigned default_സംഖ്യ_Of_Threads()
{
#ifdef __ANDROID__
    // many modern phones/tables have 4-core CPUs. Let's use no more
    // than 2 threads by default not to overheat the devices
    സ്ഥിരം unsigned പൂർണ്ണസംഖ്യ default_സംഖ്യ_of_threads = 2;
#അല്ലെങ്കിൽ
    സ്ഥിരം unsigned പൂർണ്ണസംഖ്യ default_സംഖ്യ_of_threads = (unsigned പൂർണ്ണസംഖ്യ)std::max(1, cv::get_സംഖ്യ_Of_CPUs());
#endif

    unsigned result = default_സംഖ്യ_of_threads;

    സ്ഥിതം പൂർണ്ണസംഖ്യ config_അക്കം_threads = (പൂർണ്ണസംഖ്യ)utils::getConfigurationParameterSizeT("OPENCV_FOR_THREADS_NUM", 0);

    എങ്കിൽ (config_അക്കം_threads)
    {
        result = (unsigned)std::max(1, config_അക്കം_threads);
        //do we need upper limit of threads number?
    }
    തിരികെ result;
}

ശൂന്യം set_അക്കം_Threads( പൂർണ്ണസംഖ്യ threads_ )
{
    CV_UNUSED(threads_);

    പൂർണ്ണസംഖ്യ threads = (threads_ < 0) ? default_സംഖ്യ_Of_Threads() : (unsigned)threads_;
    അക്കം_Threads = threads;

    std::shared_ptr<ParallelForAPI>& api = getCurrentParallelForAPI();
    എങ്കിൽ (api)
    {
        api->set_അക്കം_Threads(അക്കം_Threads);
    }

#ifdef HAVE_TBB

#എങ്കിൽ TBB_INTERFACE_VERSION >= 8000
    എങ്കിൽ(tbbArena.is_active()) tbbArena.terminate();
    എങ്കിൽ(threads > 0) tbbArena.initialize(threads);
#അല്ലെങ്കിൽ
    എങ്കിൽ(tbbScheduler.is_active()) tbbScheduler.terminate();
    എങ്കിൽ(threads > 0) tbbScheduler.initialize(threads);
#endif

#അല്ലെങ്കിലെങ്കിൽ defined HAVE_HPX
    തിരികെ; // nothing needed as numThreads is used

#അല്ലെങ്കിലെങ്കിൽ defined HAVE_OPENMP

    തിരികെ; // nothing needed as num_threads clause is used in #pragma omp parallel for

#അല്ലെങ്കിലെങ്കിൽ defined HAVE_GCD

    // unsupported
    // there is only private dispatch_queue_set_width() and only for desktop

#അല്ലെങ്കിലെങ്കിൽ defined WINRT

    തിരികെ;

#അല്ലെങ്കിലെങ്കിൽ defined HAVE_CONCURRENCY

    എങ്കിൽ (threads <= 0)
    {
        pplScheduler = 0;
    }
    അല്ലെങ്കിൽ എങ്കിൽ (threads == 1)
    {
        // Concurrency always uses >=2 threads, so we just disable it if 1 thread is requested
        അക്കം_Threads = 0;
    }
    അല്ലെങ്കിൽ എങ്കിൽ (pplScheduler == 0 || 1 + pplScheduler->Get_സംഖ്യ_Of_Virtual_Processors() != (unsigned പൂർണ്ണസംഖ്യ)threads)
    {
        pplScheduler = Concurrency::Scheduler::Create(Concurrency::SchedulerPolicy(2,
                       Concurrency::MinConcurrency, threads-1,
                       Concurrency::MaxConcurrency, threads-1));
    }

#അല്ലെങ്കിലെങ്കിൽ defined HAVE_PTHREADS_PF

    parallel_pthreads_set_threads_അക്കം(threads);

#endif
}


പൂർണ്ണസംഖ്യ get_Thread_അക്കം()
{
    std::shared_ptr<ParallelForAPI>& api = getCurrentParallelForAPI();
    എങ്കിൽ (api)
    {
        തിരികെ api->get_Thread_അക്കം();
    }

#എങ്കിൽ defined HAVE_TBB
    #എങ്കിൽ TBB_INTERFACE_VERSION >= 9100
        തിരികെ tbb::this_task_arena::current_thread_index();
    #അല്ലെങ്കിലെങ്കിൽ TBB_INTERFACE_VERSION >= 8000
        തിരികെ tbb::task_arena::current_thread_index();
    #അല്ലെങ്കിൽ
        തിരികെ 0;
    #endif
#അല്ലെങ്കിലെങ്കിൽ defined HAVE_HPX
    തിരികെ (പൂർണ്ണസംഖ്യ)(hpx::get_അക്കം_worker_threads());
#അല്ലെങ്കിലെങ്കിൽ defined HAVE_OPENMP
    തിരികെ omp_get_thread_അക്കം();
#അല്ലെങ്കിലെങ്കിൽ defined HAVE_GCD
    തിരികെ (പൂർണ്ണസംഖ്യ)(size_t)(ശൂന്യം*)pthread_self(); // no zero-based indexing
#അല്ലെങ്കിലെങ്കിൽ defined WINRT
    തിരികെ 0;
#അല്ലെങ്കിലെങ്കിൽ defined HAVE_CONCURRENCY
    തിരികെ std::max(0, (പൂർണ്ണസംഖ്യ)Concurrency::Context::VirtualProcessorId()); // zero for master thread, unique number for others but not necessary 1,2,3,...
#അല്ലെങ്കിലെങ്കിൽ defined HAVE_PTHREADS_PF
    തിരികെ (പൂർണ്ണസംഖ്യ)(size_t)(ശൂന്യം*)pthread_self(); // no zero-based indexing
#അല്ലെങ്കിൽ
    തിരികെ 0;
#endif
}


#എങ്കിൽ defined __linux__ || defined __GLIBC__ || defined __HAIKU__ || defined __ANDROID__
  #define CV_CPU_GROUPS_1
#endif

#എങ്കിൽ defined __linux__ || defined __ANDROID__
  #define CV_HAVE_CGROUPS 1
#endif

#എങ്കിൽ defined CV_CPU_GROUPS_1
സ്ഥിതം വരിയിൽ
std::string getFileContents(സ്ഥിരം അക്ഷരം *filename)
{
    std::ifstream ifs(filename);
    എങ്കിൽ (!ifs.is_open())
        തിരികെ std::string();

    std::string content( (std::istreambuf_iterator<അക്ഷരം>(ifs) ),
                         (std::istreambuf_iterator<അക്ഷരം>()    ) );

    എങ്കിൽ (ifs.fail())
        തിരികെ std::string();

    തിരികെ content;
}

സ്ഥിതം വരിയിൽ
പൂർണ്ണസംഖ്യ get_സംഖ്യ_Of_CPUs_Impl(സ്ഥിരം അക്ഷരം *filename)
{
   std::string file_contents = getFileContents(filename);
   എങ്കിൽ(file_contents.empty())
       തിരികെ 0;

   അക്ഷരം *pbuf = const_cast<അക്ഷരം*>(file_contents.c_str());
   //parse string of form "0-1,3,5-7,10,13-15"
   പൂർണ്ണസംഖ്യ cpusAvailable = 0;

   ആകുമ്പോൾ(*pbuf)
   {
      സ്ഥിരം അക്ഷരം* pos = pbuf;
      തർക്കം range = അസത്യം;
      ആകുമ്പോൾ(*pbuf && *pbuf != ',')
      {
          എങ്കിൽ(*pbuf == '-') range = സത്യം;
          ++pbuf;
      }
      എങ്കിൽ(*pbuf) *pbuf++ = 0;
      എങ്കിൽ(!range)
        ++cpusAvailable;
      അല്ലെങ്കിൽ
      {
          പൂർണ്ണസംഖ്യ rstart = 0, rend = 0;
          sscanf(pos, "%d-%d", &rstart, &rend);
          cpusAvailable += rend - rstart + 1;
      }

   }
   തിരികെ cpusAvailable;
}
#endif

#എങ്കിൽ defined CV_HAVE_CGROUPS
സ്ഥിതം വരിയിൽ
unsigned get_സംഖ്യ_Of_CPUs_CFSv2()
{
    പൂർണ്ണസംഖ്യ cfs_quota = 0;
    പൂർണ്ണസംഖ്യ cfs_period = 0;

    std::ifstream ss_cpu_max("/sys/fs/cgroup/cpu.max", std::ios::in | std::ios::binary);
    ss_cpu_max >> cfs_quota >> cfs_period;

    എങ്കിൽ (ss_cpu_max.fail() || cfs_quota < 1 || cfs_period < 1) /* values must not be 0 or negative */
        തിരികെ 0;

    തിരികെ (unsigned)max(1, cfs_quota/cfs_period);
}

സ്ഥിതം വരിയിൽ
unsigned get_സംഖ്യ_Of_CPUs_CFSv1()
{
    പൂർണ്ണസംഖ്യ cfs_quota = 0;
    {
        std::ifstream ss_period("/sys/fs/cgroup/cpu/cpu.cfs_quota_us", std::ios::in | std::ios::binary);
        ss_period >> cfs_quota;

        എങ്കിൽ (ss_period.fail() || cfs_quota < 1) /* cfs_quota must not be 0 or negative */
            തിരികെ 0;
    }

    പൂർണ്ണസംഖ്യ cfs_period = 0;
    {
        std::ifstream ss_quota("/sys/fs/cgroup/cpu/cpu.cfs_period_us", std::ios::in | std::ios::binary);
        ss_quota >> cfs_period;

        എങ്കിൽ (ss_quota.fail() || cfs_period < 1)
            തിരികെ 0;
    }

    തിരികെ (unsigned)max(1, cfs_quota/cfs_period);
}
#endif

template <typename T> സ്ഥിതം വരിയിൽ
T minNonZero(സ്ഥിരം T& val_1, സ്ഥിരം T& val_2)
{
    എങ്കിൽ ((val_1 != 0) && (val_2 != 0))
        തിരികെ std::min(val_1, val_2);
    തിരികെ (val_1 != 0) ? val_1 : val_2;
}

#ifndef OPENCV_DISABLE_THREAD_SUPPORT
സ്ഥിതം
പൂർണ്ണസംഖ്യ get_സംഖ്യ_Of_CPUs_2()
{
#ifndef OPENCV_SEMIHOSTING
    /*
     * Logic here is to try different methods of getting CPU counts and return
     * the minimum most value as it has high probablity of being right and safe.
     * Return 1 if we get 0 or not found on all methods.
    */
#എങ്കിൽ !defined(__MINGW32__) /* not implemented (2020-03) */

    /*
     * Check for this standard C++11 way, we do not return directly because
     * running in a docker or K8s environment will mean this is the host
     * machines config not the containers or pods and as per docs this value
     * must be "considered only a hint".
    */
    unsigned ncpus = std::thread::hardware_concurrency(); /* If the value is not well defined or not computable, returns 0 */
#അല്ലെങ്കിൽ
    unsigned ncpus = 0; /* 0 means we have to find out some other way */
#endif

#എങ്കിൽ defined _WIN32

    SYSTEM_INFO sysinfo = {};
#എങ്കിൽ (defined(_M_ARM) || defined(_M_ARM64) || defined(_M_ARM64EC) || defined(_M_X64) || defined(WINRT)) && _WIN32_WINNT >= 0x501
    GetNativeSystemInfo( &sysinfo );
#അല്ലെങ്കിൽ
    GetSystemInfo( &sysinfo );
#endif
    unsigned ncpus_sysinfo = sysinfo.dw_സംഖ്യ_Of_Processors;
    ncpus = minNonZero(ncpus, ncpus_sysinfo);

#അല്ലെങ്കിലെങ്കിൽ defined __APPLE__

    പൂർണ്ണസംഖ്യ അക്കം_CPU=0;
    പൂർണ്ണസംഖ്യ mib[4];
    size_t len = വലുപ്പം(അക്കം_CPU);

    /* set the mib for hw.ncpu */
    mib[0] = CTL_HW;
    mib[1] = HW_AVAILCPU;  // alternatively, try HW_NCPU;

    /* get the number of CPUs from the system */
    sysctl(mib, 2, &അക്കം_CPU, &len, ശൂന്യ, 0);

    എങ്കിൽ( അക്കം_CPU < 1 )
    {
        mib[1] = HW_NCPU;
        sysctl( mib, 2, &അക്കം_CPU, &len, ശൂന്യ, 0 );

        എങ്കിൽ( അക്കം_CPU < 1 )
            അക്കം_CPU = 1;
    }

    ncpus = minNonZero(ncpus, (unsigned)അക്കം_CPU);

#അല്ലെങ്കിലെങ്കിൽ defined CV_CPU_GROUPS_1

#എങ്കിൽ defined CV_HAVE_CGROUPS
    സ്ഥിതം unsigned ncpus_impl_cpuset = (unsigned)get_സംഖ്യ_Of_CPUs_Impl("/sys/fs/cgroup/cpuset/cpuset.cpus");
    ncpus = minNonZero(ncpus, ncpus_impl_cpuset);

    സ്ഥിതം unsigned ncpus_impl_cfs_v1 = get_സംഖ്യ_Of_CPUs_CFSv1();
    ncpus = minNonZero(ncpus, ncpus_impl_cfs_v1);

    സ്ഥിതം unsigned ncpus_impl_cfs_v2 = get_സംഖ്യ_Of_CPUs_CFSv2();
    ncpus = minNonZero(ncpus, ncpus_impl_cfs_v2);
#endif

    സ്ഥിതം unsigned ncpus_impl_devices = (unsigned)get_സംഖ്യ_Of_CPUs_Impl("/sys/devices/system/cpu/online");
    ncpus = minNonZero(ncpus, ncpus_impl_devices);

#endif

#എങ്കിൽ defined _GNU_SOURCE \
    && !defined(__MINGW32__) /* not implemented (2020-03) */ \
    && !defined(__EMSCRIPTEN__) \
    && !defined(__ANDROID__)  // TODO: add check for modern Android NDK

    cpu_set_t cpu_set;
    എങ്കിൽ (0 == sched_getaffinity(0, വലുപ്പം(cpu_set), &cpu_set))
    {
        unsigned cpu_എണ്ണം_cpu_set = CPU_COUNT(&cpu_set);
        ncpus = minNonZero(ncpus, cpu_എണ്ണം_cpu_set);
    }

#endif

#എങ്കിൽ !defined(_WIN32) && !defined(__APPLE__) && defined(_SC_NPROCESSORS_ONLN)

    സ്ഥിതം unsigned cpu_എണ്ണം_sysconf = (unsigned)sysconf( _SC_NPROCESSORS_ONLN );
    ncpus = minNonZero(ncpus, cpu_എണ്ണം_sysconf);
#അല്ലെങ്കിലെങ്കിൽ defined (__QNX__)
    സ്ഥിതം unsigned cpu_എണ്ണം_sysconf = _syspage_ptr->അക്കം_cpu;
    ncpus = minNonZero(ncpus, cpu_എണ്ണം_sysconf);
#endif

    തിരികെ ncpus != 0 ? ncpus : 1;
#അല്ലെങ്കിൽ //  OPENCV_SEMIHOSTING
    തിരികെ 1;
#endif //OPENCV_SEMIHOSTING
}

പൂർണ്ണസംഖ്യ get_സംഖ്യ_Of_CPUs()
{
    സ്ഥിതം പൂർണ്ണസംഖ്യ nCPUs = get_സംഖ്യ_Of_CPUs_2();
    തിരികെ nCPUs;  // cached value
}

#അല്ലെങ്കിൽ  // OPENCV_DISABLE_THREAD_SUPPORT
പൂർണ്ണസംഖ്യ get_സംഖ്യ_Of_CPUs()
{
    തിരികെ 1;
}
#endif  // OPENCV_DISABLE_THREAD_SUPPORT

സ്ഥിരം അക്ഷരം* currentParallelFramework()
{
    std::shared_ptr<ParallelForAPI>& api = getCurrentParallelForAPI();
    എങ്കിൽ (api)
    {
        തിരികെ api->getName();
    }
#ifdef CV_PARALLEL_FRAMEWORK
    തിരികെ CV_PARALLEL_FRAMEWORK;
#അല്ലെങ്കിൽ
    തിരികെ ശൂന്യ;
#endif
}

}  // namespace cv::
