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

#ಅನ್ತರ್ಭಾವ "precomp.hpp"

#ಅನ್ತರ್ಭಾವ <opencv2/core/utils/configuration.private.hpp>
#ಅನ್ತರ್ಭಾವ <opencv2/core/utils/trace.private.hpp>

#ಅನ್ತರ್ಭಾವ "opencv2/core/parallel/parallel_backend.hpp"
#ಅನ್ತರ್ಭಾವ "parallel/parallel.hpp"

#ಯದಿ defined _WIN32 || defined WINCE
    #ಅನ್ತರ್ಭಾವ <windows.h>
    #undef small
    #undef min
    #undef max
    #undef abs
#endif

#ಯದಿ defined __unix__ || defined __APPLE__ || defined __GLIBC__ \
    || defined __HAIKU__ || defined __EMSCRIPTEN__ \
    || defined __FreeBSD__ || defined __NetBSD__ || defined __OpenBSD__
    #ಅನ್ತರ್ಭಾವ <unistd.h>
    #ಅನ್ತರ್ಭಾವ <stdio.h>
    #ಅನ್ತರ್ಭಾವ <sys/types.h>
    #ಅನ್ತರ್ಭಾವ <fstream>
    #ಯದಿ defined __ANDROID__
        #ಅನ್ತರ್ಭಾವ <sys/sysconf.h>
        #ಅನ್ತರ್ಭಾವ <sys/syscall.h>
        #ಅನ್ತರ್ಭಾವ <sched.h>
    #ನೋಚೇದ್ಯದಿ defined __APPLE__
        #ಅನ್ತರ್ಭಾವ <sys/sysctl.h>
    #endif
#endif

#ಯದಿ defined (__QNX__)
    #ಅನ್ತರ್ಭಾವ <sys/syspage.h>
#endif

#ifndef OPENCV_DISABLE_THREAD_SUPPORT
    #ಅನ್ತರ್ಭಾವ <thread>
#endif

#ifdef _OPENMP
    #define HAVE_OPENMP
#endif

#ifdef __APPLE__
    #define HAVE_GCD
#endif

#ಯದಿ defined _MSC_VER
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

#ಯದಿ defined HAVE_TBB
    #ifndef TBB_SUPPRESS_DEPRECATED_MESSAGES  // supress warning
    #define TBB_SUPPRESS_DEPRECATED_MESSAGES 1
    #endif
    #ಅನ್ತರ್ಭಾವ "tbb/tbb.h"
    #ಅನ್ತರ್ಭಾವ "tbb/task.h"
    #ಯದಿ TBB_INTERFACE_VERSION >= 8000
        #ಅನ್ತರ್ಭಾವ "tbb/task_arena.h"
    #endif
    #undef min
    #undef max
#ನೋಚೇದ್ಯದಿ defined HAVE_HPX
    #ಅನ್ತರ್ಭಾವ <hpx/parallel/algorithms/for_loop.hpp>
    #ಅನ್ತರ್ಭಾವ <hpx/parallel/execution.hpp>
    //
    #ಅನ್ತರ್ಭಾವ <hpx/hpx_start.hpp>
    #ಅನ್ತರ್ಭಾವ <hpx/hpx_suspend.hpp>
    #ಅನ್ತರ್ಭಾವ <hpx/include/apply.hpp>
    #ಅನ್ತರ್ಭಾವ <hpx/util/yield_while.hpp>
    #ಅನ್ತರ್ಭಾವ <hpx/include/threadmanager.hpp>

#ನೋಚೇದ್ಯದಿ defined HAVE_OPENMP
    #ಅನ್ತರ್ಭಾವ <omp.h>
#ನೋಚೇದ್ಯದಿ defined HAVE_GCD
    #ಅನ್ತರ್ಭಾವ <dispatch/dispatch.h>
    #ಅನ್ತರ್ಭಾವ <pthread.h>
#ನೋಚೇದ್ಯದಿ defined HAVE_CONCURRENCY
    #ಅನ್ತರ್ಭಾವ <ppl.h>
#ನೋಚೇದ್ಯದಿ defined HAVE_PTHREADS_PF
    #ಅನ್ತರ್ಭಾವ <pthread.h>
#endif


#ಯದಿ defined HAVE_TBB
#  define CV_PARALLEL_FRAMEWORK "tbb"
#ನೋಚೇದ್ಯದಿ defined HAVE_HPX
#  define CV_PARALLEL_FRAMEWORK "hpx"
#ನೋಚೇದ್ಯದಿ defined HAVE_OPENMP
#  define CV_PARALLEL_FRAMEWORK "openmp"
#ನೋಚೇದ್ಯದಿ defined HAVE_GCD
#  define CV_PARALLEL_FRAMEWORK "gcd"
#ನೋಚೇದ್ಯದಿ defined WINRT
#  define CV_PARALLEL_FRAMEWORK "winrt-concurrency"
#ನೋಚೇದ್ಯದಿ defined HAVE_CONCURRENCY
#  define CV_PARALLEL_FRAMEWORK "ms-concurrency"
#ನೋಚೇದ್ಯದಿ defined HAVE_PTHREADS_PF
#  define CV_PARALLEL_FRAMEWORK "pthreads"
#endif

#ಅನ್ತರ್ಭಾವ <atomic>

#ಅನ್ತರ್ಭಾವ "parallel_impl.hpp"

#ಅನ್ತರ್ಭಾವ "opencv2/core/detail/exception_ptr.hpp"  // CV__EXCEPTION_PTR = 1 if std::exception_ptr is available

#ಅನ್ತರ್ಭಾವ <opencv2/core/utils/fp_control_utils.hpp>
#ಅನ್ತರ್ಭಾವ <opencv2/core/utils/fp_control.private.hpp>

using ನಾಮಾಕಾಶ cv;

ನಾಮಾಕಾಶ cv {

ParallelLoopBody::~ParallelLoopBody() {}

using ನಾಮಾಕಾಶ cv::parallel;

ನಾಮಾಕಾಶ {

#ifdef ENABLE_INSTRUMENTATION
    ಸ್ಥಿರ ಶೂನ್ಯ SyncNodes(cv::instr::Instr_ಗಂಟು *p_ಗಂಟು)
    {
        std::vector<cv::instr::ಗಂಟು_Data_Tls*> data;
        p_ಗಂಟು->m_payload.m_tls.gather(data);

        uint64 ticksMax = 0;
        ಪೂರ್ಣಾಙ್ಕ    threads  = 0;
        ಪ್ರತಿ(size_t i = 0; i < data.size(); i++)
        {
            ಯದಿ(data[i] && data[i]->m_ticksTotal)
            {
                ticksMax = MAX(ticksMax, data[i]->m_ticksTotal);
                p_ಗಂಟು->m_payload.m_ticksTotal -= data[i]->m_ticksTotal;
                data[i]->m_ticksTotal = 0;
                threads++;
            }
        }
        p_ಗಂಟು->m_payload.m_ticksTotal += ticksMax;
        p_ಗಂಟು->m_payload.m_threads = MAX(p_ಗಂಟು->m_payload.m_threads, threads);

        ಪ್ರತಿ(size_t i = 0; i < p_ಗಂಟು->m_childs.size(); i++)
            SyncNodes(p_ಗಂಟು->m_childs[i]);
    }
#endif

    ವರ್ಗ ParallelLoopBodyWrapperContext
    {
    ಸಾರ್ವಜನಿಕ:
        ParallelLoopBodyWrapperContext(ಧ್ರುವ cv::ParallelLoopBody& _body, ಧ್ರುವ cv::Range& _r, ದ್ವಿಗುಣ _nstripes) :
            is_rng_used(ಅಸತ್ಯ), hasException(ಅಸತ್ಯ)
        {

            body = &_body;
            wholeRange = _r;
            ದ್ವಿಗುಣ len = wholeRange.end - wholeRange.start;
            nstripes = cvRound(_nstripes <= 0 ? len : MIN(MAX(_nstripes, 1.), len));

            // propagate main thread state
            rng = cv::theRNG();
#ಯದಿ OPENCV_SUPPORTS_FP_DENORMALS_HINT && OPENCV_IMPL_FP_HINTS
            details::saveFPDenormalsState(fp_denormals_base_state);
#endif

#ifdef OPENCV_TRACE
            trace_ಬೇರು_Region = CV_TRACE_NS::details::getCurrentRegion();
            trace_ಬೇರು_Context = CV_TRACE_NS::details::getTraceManager().tls.get();
#endif

#ifdef ENABLE_INSTRUMENTATION
            p_Thread_ಬೇರು = cv::instr::getInstrumentTLSStruct().p_Current_ಗಂಟು;
#endif
        }
        ಶೂನ್ಯ finalize()
        {
#ifdef ENABLE_INSTRUMENTATION
            ಪ್ರತಿ(size_t i = 0; i < p_Thread_ಬೇರು->m_childs.size(); i++)
                SyncNodes(p_Thread_ಬೇರು->m_childs[i]);
#endif
            ಯದಿ (is_rng_used)
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
            ಯದಿ (trace_ಬೇರು_Region)
                CV_TRACE_NS::details::parallelForFinalize(*trace_ಬೇರು_Region);
#endif

            ಯದಿ (hasException)
            {
#ಯದಿ CV__EXCEPTION_PTR
                std::rethrow_exception(pException);
#ಅನ್ಯಥಾ
                CV_Error(Error::StsError, "Exception in parallel_for() body: " + exception_message);
#endif
            }
        }
        ~ParallelLoopBodyWrapperContext() {}

        ಧ್ರುವ cv::ParallelLoopBody* body;
        cv::Range wholeRange;
        ಪೂರ್ಣಾಙ್ಕ nstripes;
        cv::RNG rng;
        mutable ತಾರ್ಕಿಕ is_rng_used;
#ifdef OPENCV_TRACE
        CV_TRACE_NS::details::Region* trace_ಬೇರು_Region;
        CV_TRACE_NS::details::TraceManagerThreadLocal* trace_ಬೇರು_Context;
#endif
#ifdef ENABLE_INSTRUMENTATION
        cv::instr::Instr_ಗಂಟು *p_Thread_ಬೇರು;
#endif
        ತಾರ್ಕಿಕ hasException;
#ಯದಿ CV__EXCEPTION_PTR
        std::exception_ptr pException;
#ಅನ್ಯಥಾ
        cv::String exception_message;
#endif
#ಯದಿ CV__EXCEPTION_PTR
        ಶೂನ್ಯ recordException()
#ಅನ್ಯಥಾ
        ಶೂನ್ಯ recordException(ಧ್ರುವ cv::String& ಸಂದೇಶ)
#endif
        {
#ifndef CV_THREAD_SANITIZER
            ಯದಿ (!hasException)
#endif
            {
                cv::AutoLock lock(cv::getInitializationMutex());
                ಯದಿ (!hasException)
                {
                    hasException = ಸತ್ಯ;
#ಯದಿ CV__EXCEPTION_PTR
                    pException = std::current_exception();
#ಅನ್ಯಥಾ
                    exception_message = ಸಂದೇಶ;
#endif
                }
            }
        }

#ಯದಿ OPENCV_SUPPORTS_FP_DENORMALS_HINT && OPENCV_IMPL_FP_HINTS
        details::FPDenormalsModeState fp_denormals_base_state;
#endif

    ಗುಪ್ತ:
        ParallelLoopBodyWrapperContext(ಧ್ರುವ ParallelLoopBodyWrapperContext&); // disabled
        ParallelLoopBodyWrapperContext& operator=(ಧ್ರುವ ParallelLoopBodyWrapperContext&); // disabled
    };

    ವರ್ಗ ParallelLoopBodyWrapper : ಸಾರ್ವಜನಿಕ cv::ParallelLoopBody
    {
    ಸಾರ್ವಜನಿಕ:
        ParallelLoopBodyWrapper(ParallelLoopBodyWrapperContext& ctx_) :
            ctx(ctx_)
        {
        }
        ~ParallelLoopBodyWrapper()
        {
        }
        ಶೂನ್ಯ operator()(ಧ್ರುವ cv::Range& sr) ಧ್ರುವ CV_OVERRIDE
        {
#ifdef OPENCV_TRACE
            // TODO CV_TRACE_NS::details::setCurrentRegion(rootRegion);
            ಯದಿ (ctx.trace_ಬೇರು_Region && ctx.trace_ಬೇರು_Context)
                CV_TRACE_NS::details::parallel_For_Set_ಬೇರು_Region(*ctx.trace_ಬೇರು_Region, *ctx.trace_ಬೇರು_Context);
            CV__TRACE_OPENCV_FUNCTION_NAME("parallel_for_body");
            ಯದಿ (ctx.trace_ಬೇರು_Region)
                CV_TRACE_NS::details::parallelForAttachNestedRegion(*ctx.trace_ಬೇರು_Region);
#endif

#ifdef ENABLE_INSTRUMENTATION
            {
                cv::instr::InstrTLSStruct *pInstrTLS = &cv::instr::getInstrumentTLSStruct();
                pInstrTLS->p_Current_ಗಂಟು = ctx.p_Thread_ಬೇರು; // Initialize TLS node for thread
            }
            CV_INSTRUMENT_REGION();
#endif

            // propagate main thread state
            cv::theRNG() = ctx.rng;
#ಯದಿ OPENCV_SUPPORTS_FP_DENORMALS_HINT && OPENCV_IMPL_FP_HINTS
            FPDenormalsIgnoreHintScope fp_denormals_scope(ctx.fp_denormals_base_state);
#endif

            cv::Range r;
            cv::Range wholeRange = ctx.wholeRange;
            ಪೂರ್ಣಾಙ್ಕ nstripes = ctx.nstripes;
            r.start = (ಪೂರ್ಣಾಙ್ಕ)(wholeRange.start +
                            ((uint64)sr.start*(wholeRange.end - wholeRange.start) + nstripes/2)/nstripes);
            r.end = sr.end >= nstripes ? wholeRange.end : (ಪೂರ್ಣಾಙ್ಕ)(wholeRange.start +
                            ((uint64)sr.end*(wholeRange.end - wholeRange.start) + nstripes/2)/nstripes);

#ifdef OPENCV_TRACE
            CV_TRACE_ARG_ಮೌಲ್ಯ(range_start, "range.start", (int64)r.start);
            CV_TRACE_ARG_ಮೌಲ್ಯ(range_end, "range.end", (int64)r.end);
#endif

            ಯತ್ನ
            {
                (*ctx.body)(r);
            }
#ಯದಿ CV__EXCEPTION_PTR
            ಗೃಹಾಣ (...)
            {
                ctx.recordException();
            }
#ಅನ್ಯಥಾ
            ಗೃಹಾಣ (ಧ್ರುವ cv::Exception& e)
            {
                ctx.recordException(e.what());
            }
            ಗೃಹಾಣ (ಧ್ರುವ std::exception& e)
            {
                ctx.recordException(e.what());
            }
            ಗೃಹಾಣ (...)
            {
                ctx.recordException("Unknown exception");
            }
#endif

            ಯದಿ (!ctx.is_rng_used && !(cv::theRNG() == ctx.rng))
                ctx.is_rng_used = ಸತ್ಯ;
        }
        cv::Range stripeRange() ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ cv::Range(0, ctx.nstripes); }

    ರಕ್ಷಿತ:
        ParallelLoopBodyWrapperContext& ctx;
    };

#ಯದಿ defined HAVE_TBB
    ವರ್ಗ ProxyLoopBody : ಸಾರ್ವಜನಿಕ ParallelLoopBodyWrapper
    {
    ಸಾರ್ವಜನಿಕ:
        ProxyLoopBody(ParallelLoopBodyWrapperContext& ctx_)
        : ParallelLoopBodyWrapper(ctx_)
        {}

        ಶೂನ್ಯ operator ()(ಧ್ರುವ tbb::blocked_range<ಪೂರ್ಣಾಙ್ಕ>& range) ಧ್ರುವ
        {
            ಏತತ್->ParallelLoopBodyWrapper::operator()(cv::Range(range.begin(), range.end()));
        }

        ಶೂನ್ಯ operator ()() ಧ್ರುವ  // run parallel job
        {
            cv::Range range = ಏತತ್->stripeRange();
            tbb::parallel_for(tbb::blocked_range<ಪೂರ್ಣಾಙ್ಕ>(range.start, range.end), *ಏತತ್);
        }
    };
#ನೋಚೇದ್ಯದಿ defined HAVE_HPX
    ವರ್ಗ ProxyLoopBody : ಸಾರ್ವಜನಿಕ ParallelLoopBodyWrapper
    {
    ಸಾರ್ವಜನಿಕ:
        ProxyLoopBody(ParallelLoopBodyWrapperContext& ctx_)
                : ParallelLoopBodyWrapper(ctx_)
        {}

        ಶೂನ್ಯ operator ()() ಧ್ರುವ  // run parallel job
        {
            cv::Range stripeRange = ಏತತ್->stripeRange();
            hpx::parallel::for_loop(
                    hpx::parallel::execution::par,
                    stripeRange.start, stripeRange.end,
                    [&](ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ &i) { ;
                        ಏತತ್->ParallelLoopBodyWrapper::operator()(
                                cv::Range(i, i + 1));
                    });
        }
    };
#ನೋಚೇದ್ಯದಿ defined HAVE_OPENMP
    ಪ್ರಕಾರನಿರ್ದೇಶ ParallelLoopBodyWrapper ProxyLoopBody;
#ನೋಚೇದ್ಯದಿ defined HAVE_GCD
    ಪ್ರಕಾರನಿರ್ದೇಶ ParallelLoopBodyWrapper ProxyLoopBody;
    ಸ್ಥಿರ ಶೂನ್ಯ block_function(ಶೂನ್ಯ* context, size_t index)
    {
        ProxyLoopBody* ptr_body = static_cast<ProxyLoopBody*>(context);
        (*ptr_body)(cv::Range((ಪೂರ್ಣಾಙ್ಕ)index, (ಪೂರ್ಣಾಙ್ಕ)index + 1));
    }
#ನೋಚೇದ್ಯದಿ defined WINRT || defined HAVE_CONCURRENCY
    ವರ್ಗ ProxyLoopBody : ಸಾರ್ವಜನಿಕ ParallelLoopBodyWrapper
    {
    ಸಾರ್ವಜನಿಕ:
        ProxyLoopBody(ParallelLoopBodyWrapperContext& ctx)
        : ParallelLoopBodyWrapper(ctx)
        {}

        ಶೂನ್ಯ operator ()(ಪೂರ್ಣಾಙ್ಕ i) ಧ್ರುವ
        {
            ಏತತ್->ParallelLoopBodyWrapper::operator()(cv::Range(i, i + 1));
        }
    };

    // PPL's default auto_partitioner silently skips whole chunks of the range on
    // ARM64 (unfixed MSVC bug, developercommunity.visualstudio.com/t/1027444).
    template <typename Body>
    ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ pplParallelFor(ಪೂರ್ಣಾಙ್ಕ first, ಪೂರ್ಣಾಙ್ಕ last, ಧ್ರುವ Body& body)
    {
#ಯದಿ defined(_M_ARM64) || defined(_M_ARM64EC)
        Concurrency::parallel_for(first, last, body, Concurrency::static_partitioner());
#ಅನ್ಯಥಾ
        Concurrency::parallel_for(first, last, body);
#endif
    }
#ಅನ್ಯಥಾ
    ಪ್ರಕಾರನಿರ್ದೇಶ ParallelLoopBodyWrapper ProxyLoopBody;
#endif

#ಯದಿ defined HAVE_TBB
    #ಯದಿ TBB_INTERFACE_VERSION >= 8000
        ಸ್ಥಿರ tbb::task_arena tbbArena(tbb::task_arena::automatic);
    #ಅನ್ಯಥಾ
        ಸ್ಥಿರ tbb::task_scheduler_init tbbScheduler(tbb::task_scheduler_init::deferred);
    #endif
#ನೋಚೇದ್ಯದಿ defined HAVE_HPX
// nothing for HPX
#ನೋಚೇದ್ಯದಿ defined HAVE_OPENMP
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ _initMaxThreads()
{
    ಪೂರ್ಣಾಙ್ಕ maxThreads = omp_get_max_threads();
    ಪ್ರತ್ಯಾಗಮ maxThreads;
}
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_Threads_Max = _initMaxThreads();
#ನೋಚೇದ್ಯದಿ defined HAVE_GCD
// nothing for GCD
#ನೋಚೇದ್ಯದಿ defined WINRT
// nothing for WINRT
#ನೋಚೇದ್ಯದಿ defined HAVE_CONCURRENCY

ವರ್ಗ SchedPtr
{
    Concurrency::Scheduler* sched_;
ಸಾರ್ವಜನಿಕ:
    Concurrency::Scheduler* operator->() { ಪ್ರತ್ಯಾಗಮ sched_; }
    operator Concurrency::Scheduler*() { ಪ್ರತ್ಯಾಗಮ sched_; }

    ಶೂನ್ಯ operator=(Concurrency::Scheduler* sched)
    {
        ಯದಿ (sched_) sched_->Release();
        sched_ = sched;
    }

    SchedPtr() : sched_(0) {}
    ~SchedPtr() {}
};
ಸ್ಥಿರ SchedPtr pplScheduler;

#endif

} // namespace anon

/* ================================   parallel_for_  ================================ */

ಸ್ಥಿರ ಶೂನ್ಯ parallel_for_impl(ಧ್ರುವ cv::Range& range, ಧ್ರುವ cv::ParallelLoopBody& body, ದ್ವಿಗುಣ nstripes); // forward declaration

ಶೂನ್ಯ parallel_for_(ಧ್ರುವ cv::Range& range, ಧ್ರುವ cv::ParallelLoopBody& body, ದ್ವಿಗುಣ nstripes)
{
#ifdef OPENCV_TRACE
    CV__TRACE_OPENCV_FUNCTION_NAME_("parallel_for", 0);
    CV_TRACE_ARG_ಮೌಲ್ಯ(range_start, "range.start", (int64)range.start);
    CV_TRACE_ARG_ಮೌಲ್ಯ(range_end, "range.end", (int64)range.end);
    CV_TRACE_ARG_ಮೌಲ್ಯ(nstripes, "nstripes", (int64)nstripes);
#endif

    CV_INSTRUMENT_REGION_MT_FORK();
    ಯದಿ (range.empty())
        ಪ್ರತ್ಯಾಗಮ;

    ಸ್ಥಿರ std::atomic<ತಾರ್ಕಿಕ> flagNestedParallelFor(ಅಸತ್ಯ);
    ತಾರ್ಕಿಕ isNotNestedRegion = !flagNestedParallelFor.load();
    ಯದಿ (isNotNestedRegion)
      isNotNestedRegion = !flagNestedParallelFor.exchange(ಸತ್ಯ);
    ಯದಿ (isNotNestedRegion)
    {
        ಯತ್ನ
        {
            parallel_for_impl(range, body, nstripes);
            flagNestedParallelFor = ಅಸತ್ಯ;
        }
        ಗೃಹಾಣ (...)
        {
            flagNestedParallelFor = ಅಸತ್ಯ;
            ಕ್ಷಿಪ;
        }
    }
    ಅನ್ಯಥಾ // nested parallel_for_() calls are not parallelized
    {
        CV_UNUSED(nstripes);
        body(range);
    }
}

ಸ್ಥಿರ
ಶೂನ್ಯ parallel_for_cb(ಪೂರ್ಣಾಙ್ಕ start, ಪೂರ್ಣಾಙ್ಕ end, ಶೂನ್ಯ* data)
{
    CV_DbgAssert(data);
    ಧ್ರುವ cv::ParallelLoopBody& body = *(ಧ್ರುವ cv::ParallelLoopBody*)data;
    body(Range(start, end));
}

ಸ್ಥಿರ ಶೂನ್ಯ parallel_for_impl(ಧ್ರುವ cv::Range& range, ಧ್ರುವ cv::ParallelLoopBody& body, ದ್ವಿಗುಣ nstripes)
{
    using ನಾಮಾಕಾಶ cv::parallel;
    ಯದಿ ((ಅಂಕೆ_Threads < 0 || ಅಂಕೆ_Threads > 1) && range.end - range.start > 1)
    {
        ParallelLoopBodyWrapperContext ctx(body, range, nstripes);
        ProxyLoopBody pbody(ctx);
        cv::Range stripeRange = pbody.stripeRange();
        ಯದಿ( stripeRange.end - stripeRange.start == 1 )
        {
            body(range);
            ಪ್ರತ್ಯಾಗಮ;
        }

        std::shared_ptr<ParallelForAPI>& api = getCurrentParallelForAPI();
        ಯದಿ (api)
        {
            CV_CheckEQ(stripeRange.start, 0, "");
            api->parallel_for(stripeRange.end, parallel_for_cb, (ಶೂನ್ಯ*)&pbody);
            ctx.finalize();  // propagate exceptions if exists
            ಪ್ರತ್ಯಾಗಮ;
        }

#ifdef CV_PARALLEL_FRAMEWORK
#ಯದಿ defined HAVE_TBB

#ಯದಿ TBB_INTERFACE_VERSION >= 8000
        tbbArena.execute(pbody);
#ಅನ್ಯಥಾ
        pbody();
#endif

#ನೋಚೇದ್ಯದಿ defined HAVE_HPX
        pbody();

#ನೋಚೇದ್ಯದಿ defined HAVE_OPENMP

        #pragma omp parallel ಪ್ರತಿ schedule(dynamic) ಅಂಕೆ_threads(ಅಂಕೆ_Threads > 0 ? ಅಂಕೆ_Threads : ಅಂಕೆ_Threads_Max)
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = stripeRange.start; i < stripeRange.end; ++i)
            pbody(Range(i, i + 1));

#ನೋಚೇದ್ಯದಿ defined HAVE_GCD

        dispatch_queue_t concurrent_queue = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0);
        dispatch_apply_f(stripeRange.end - stripeRange.start, concurrent_queue, &pbody, block_function);

#ನೋಚೇದ್ಯದಿ defined WINRT

        pplParallelFor(stripeRange.start, stripeRange.end, pbody);

#ನೋಚೇದ್ಯದಿ defined HAVE_CONCURRENCY

        ಯದಿ(!pplScheduler || pplScheduler->Id() == Concurrency::CurrentScheduler::Id())
        {
            pplParallelFor(stripeRange.start, stripeRange.end, pbody);
        }
        ಅನ್ಯಥಾ
        {
            pplScheduler->Attach();
            pplParallelFor(stripeRange.start, stripeRange.end, pbody);
            Concurrency::CurrentScheduler::Detach();
        }

#ನೋಚೇದ್ಯದಿ defined HAVE_PTHREADS_PF

        parallel_for_pthreads(pbody.stripeRange(), pbody, pbody.stripeRange().size());

#ಅನ್ಯಥಾ

#error You have hacked and compiling with unsupported parallel framework

#endif

        ctx.finalize();  // propagate exceptions if exists
        ಪ್ರತ್ಯಾಗಮ;
#endif // CV_PARALLEL_FRAMEWORK
    }

    body(range);
}


ಪೂರ್ಣಾಙ್ಕ get_ಅಂಕೆ_Threads(ಶೂನ್ಯ)
{
    std::shared_ptr<ParallelForAPI>& api = getCurrentParallelForAPI();
    ಯದಿ (api)
    {
        ಪ್ರತ್ಯಾಗಮ api->get_ಅಂಕೆ_Threads();
    }

    ಯದಿ (ಅಂಕೆ_Threads == 0)
        ಪ್ರತ್ಯಾಗಮ 1;

#ಯದಿ defined HAVE_TBB

#ಯದಿ TBB_INTERFACE_VERSION >= 9100
    ಪ್ರತ್ಯಾಗಮ tbbArena.max_concurrency();
#ನೋಚೇದ್ಯದಿ TBB_INTERFACE_VERSION >= 8000
    ಪ್ರತ್ಯಾಗಮ ಅಂಕೆ_Threads > 0
        ? ಅಂಕೆ_Threads
        : tbb::task_scheduler_init::default_ಅಂಕೆ_threads();
#ಅನ್ಯಥಾ
    ಪ್ರತ್ಯಾಗಮ tbbScheduler.is_active()
           ? ಅಂಕೆ_Threads
           : tbb::task_scheduler_init::default_ಅಂಕೆ_threads();
#endif

#ನೋಚೇದ್ಯದಿ defined HAVE_HPX
    ಪ್ರತ್ಯಾಗಮ ಅಂಕೆ_Threads;

#ನೋಚೇದ್ಯದಿ defined HAVE_OPENMP

    ಪ್ರತ್ಯಾಗಮ ಅಂಕೆ_Threads > 0
           ? ಅಂಕೆ_Threads
           : ಅಂಕೆ_Threads_Max;


#ನೋಚೇದ್ಯದಿ defined HAVE_GCD

    ಪ್ರತ್ಯಾಗಮ cv::get_ಸಂಖ್ಯೆ_Of_CPUs(); // the GCD thread pool limit

#ನೋಚೇದ್ಯದಿ defined WINRT

    ಪ್ರತ್ಯಾಗಮ 0;

#ನೋಚೇದ್ಯದಿ defined HAVE_CONCURRENCY

    ಪ್ರತ್ಯಾಗಮ (pplScheduler == 0)
        ? Concurrency::CurrentScheduler::Get()->Get_ಸಂಖ್ಯೆ_Of_Virtual_Processors()
        : (1 + pplScheduler->Get_ಸಂಖ್ಯೆ_Of_Virtual_Processors());

#ನೋಚೇದ್ಯದಿ defined HAVE_PTHREADS_PF

        ಪ್ರತ್ಯಾಗಮ parallel_pthreads_get_threads_ಅಂಕೆ();

#ಅನ್ಯಥಾ

    ಪ್ರತ್ಯಾಗಮ 1;

#endif
}

unsigned default_ಸಂಖ್ಯೆ_Of_Threads()
{
#ifdef __ANDROID__
    // many modern phones/tables have 4-core CPUs. Let's use no more
    // than 2 threads by default not to overheat the devices
    ಧ್ರುವ unsigned ಪೂರ್ಣಾಙ್ಕ default_ಸಂಖ್ಯೆ_of_threads = 2;
#ಅನ್ಯಥಾ
    ಧ್ರುವ unsigned ಪೂರ್ಣಾಙ್ಕ default_ಸಂಖ್ಯೆ_of_threads = (unsigned ಪೂರ್ಣಾಙ್ಕ)std::max(1, cv::get_ಸಂಖ್ಯೆ_Of_CPUs());
#endif

    unsigned result = default_ಸಂಖ್ಯೆ_of_threads;

    ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ config_ಅಂಕೆ_threads = (ಪೂರ್ಣಾಙ್ಕ)utils::getConfigurationParameterSizeT("OPENCV_FOR_THREADS_NUM", 0);

    ಯದಿ (config_ಅಂಕೆ_threads)
    {
        result = (unsigned)std::max(1, config_ಅಂಕೆ_threads);
        //do we need upper limit of threads number?
    }
    ಪ್ರತ್ಯಾಗಮ result;
}

ಶೂನ್ಯ set_ಅಂಕೆ_Threads( ಪೂರ್ಣಾಙ್ಕ threads_ )
{
    CV_UNUSED(threads_);

    ಪೂರ್ಣಾಙ್ಕ threads = (threads_ < 0) ? default_ಸಂಖ್ಯೆ_Of_Threads() : (unsigned)threads_;
    ಅಂಕೆ_Threads = threads;

    std::shared_ptr<ParallelForAPI>& api = getCurrentParallelForAPI();
    ಯದಿ (api)
    {
        api->set_ಅಂಕೆ_Threads(ಅಂಕೆ_Threads);
    }

#ifdef HAVE_TBB

#ಯದಿ TBB_INTERFACE_VERSION >= 8000
    ಯದಿ(tbbArena.is_active()) tbbArena.terminate();
    ಯದಿ(threads > 0) tbbArena.initialize(threads);
#ಅನ್ಯಥಾ
    ಯದಿ(tbbScheduler.is_active()) tbbScheduler.terminate();
    ಯದಿ(threads > 0) tbbScheduler.initialize(threads);
#endif

#ನೋಚೇದ್ಯದಿ defined HAVE_HPX
    ಪ್ರತ್ಯಾಗಮ; // nothing needed as numThreads is used

#ನೋಚೇದ್ಯದಿ defined HAVE_OPENMP

    ಪ್ರತ್ಯಾಗಮ; // nothing needed as num_threads clause is used in #pragma omp parallel for

#ನೋಚೇದ್ಯದಿ defined HAVE_GCD

    // unsupported
    // there is only private dispatch_queue_set_width() and only for desktop

#ನೋಚೇದ್ಯದಿ defined WINRT

    ಪ್ರತ್ಯಾಗಮ;

#ನೋಚೇದ್ಯದಿ defined HAVE_CONCURRENCY

    ಯದಿ (threads <= 0)
    {
        pplScheduler = 0;
    }
    ಅನ್ಯಥಾ ಯದಿ (threads == 1)
    {
        // Concurrency always uses >=2 threads, so we just disable it if 1 thread is requested
        ಅಂಕೆ_Threads = 0;
    }
    ಅನ್ಯಥಾ ಯದಿ (pplScheduler == 0 || 1 + pplScheduler->Get_ಸಂಖ್ಯೆ_Of_Virtual_Processors() != (unsigned ಪೂರ್ಣಾಙ್ಕ)threads)
    {
        pplScheduler = Concurrency::Scheduler::Create(Concurrency::SchedulerPolicy(2,
                       Concurrency::MinConcurrency, threads-1,
                       Concurrency::MaxConcurrency, threads-1));
    }

#ನೋಚೇದ್ಯದಿ defined HAVE_PTHREADS_PF

    parallel_pthreads_set_threads_ಅಂಕೆ(threads);

#endif
}


ಪೂರ್ಣಾಙ್ಕ get_Thread_ಅಂಕೆ()
{
    std::shared_ptr<ParallelForAPI>& api = getCurrentParallelForAPI();
    ಯದಿ (api)
    {
        ಪ್ರತ್ಯಾಗಮ api->get_Thread_ಅಂಕೆ();
    }

#ಯದಿ defined HAVE_TBB
    #ಯದಿ TBB_INTERFACE_VERSION >= 9100
        ಪ್ರತ್ಯಾಗಮ tbb::this_task_arena::current_thread_index();
    #ನೋಚೇದ್ಯದಿ TBB_INTERFACE_VERSION >= 8000
        ಪ್ರತ್ಯಾಗಮ tbb::task_arena::current_thread_index();
    #ಅನ್ಯಥಾ
        ಪ್ರತ್ಯಾಗಮ 0;
    #endif
#ನೋಚೇದ್ಯದಿ defined HAVE_HPX
    ಪ್ರತ್ಯಾಗಮ (ಪೂರ್ಣಾಙ್ಕ)(hpx::get_ಅಂಕೆ_worker_threads());
#ನೋಚೇದ್ಯದಿ defined HAVE_OPENMP
    ಪ್ರತ್ಯಾಗಮ omp_get_thread_ಅಂಕೆ();
#ನೋಚೇದ್ಯದಿ defined HAVE_GCD
    ಪ್ರತ್ಯಾಗಮ (ಪೂರ್ಣಾಙ್ಕ)(size_t)(ಶೂನ್ಯ*)pthread_self(); // no zero-based indexing
#ನೋಚೇದ್ಯದಿ defined WINRT
    ಪ್ರತ್ಯಾಗಮ 0;
#ನೋಚೇದ್ಯದಿ defined HAVE_CONCURRENCY
    ಪ್ರತ್ಯಾಗಮ std::max(0, (ಪೂರ್ಣಾಙ್ಕ)Concurrency::Context::VirtualProcessorId()); // zero for master thread, unique number for others but not necessary 1,2,3,...
#ನೋಚೇದ್ಯದಿ defined HAVE_PTHREADS_PF
    ಪ್ರತ್ಯಾಗಮ (ಪೂರ್ಣಾಙ್ಕ)(size_t)(ಶೂನ್ಯ*)pthread_self(); // no zero-based indexing
#ಅನ್ಯಥಾ
    ಪ್ರತ್ಯಾಗಮ 0;
#endif
}


#ಯದಿ defined __linux__ || defined __GLIBC__ || defined __HAIKU__ || defined __ANDROID__
  #define CV_CPU_GROUPS_1
#endif

#ಯದಿ defined __linux__ || defined __ANDROID__
  #define CV_HAVE_CGROUPS 1
#endif

#ಯದಿ defined CV_CPU_GROUPS_1
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ
std::string getFileContents(ಧ್ರುವ ವರ್ಣ *filename)
{
    std::ifstream ifs(filename);
    ಯದಿ (!ifs.is_open())
        ಪ್ರತ್ಯಾಗಮ std::string();

    std::string content( (std::istreambuf_iterator<ವರ್ಣ>(ifs) ),
                         (std::istreambuf_iterator<ವರ್ಣ>()    ) );

    ಯದಿ (ifs.fail())
        ಪ್ರತ್ಯಾಗಮ std::string();

    ಪ್ರತ್ಯಾಗಮ content;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ
ಪೂರ್ಣಾಙ್ಕ get_ಸಂಖ್ಯೆ_Of_CPUs_Impl(ಧ್ರುವ ವರ್ಣ *filename)
{
   std::string file_contents = getFileContents(filename);
   ಯದಿ(file_contents.empty())
       ಪ್ರತ್ಯಾಗಮ 0;

   ವರ್ಣ *pbuf = const_cast<ವರ್ಣ*>(file_contents.c_str());
   //parse string of form "0-1,3,5-7,10,13-15"
   ಪೂರ್ಣಾಙ್ಕ cpusAvailable = 0;

   ಯಾವತ್(*pbuf)
   {
      ಧ್ರುವ ವರ್ಣ* pos = pbuf;
      ತಾರ್ಕಿಕ range = ಅಸತ್ಯ;
      ಯಾವತ್(*pbuf && *pbuf != ',')
      {
          ಯದಿ(*pbuf == '-') range = ಸತ್ಯ;
          ++pbuf;
      }
      ಯದಿ(*pbuf) *pbuf++ = 0;
      ಯದಿ(!range)
        ++cpusAvailable;
      ಅನ್ಯಥಾ
      {
          ಪೂರ್ಣಾಙ್ಕ rstart = 0, rend = 0;
          sscanf(pos, "%d-%d", &rstart, &rend);
          cpusAvailable += rend - rstart + 1;
      }

   }
   ಪ್ರತ್ಯಾಗಮ cpusAvailable;
}
#endif

#ಯದಿ defined CV_HAVE_CGROUPS
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ
unsigned get_ಸಂಖ್ಯೆ_Of_CPUs_CFSv2()
{
    ಪೂರ್ಣಾಙ್ಕ cfs_quota = 0;
    ಪೂರ್ಣಾಙ್ಕ cfs_period = 0;

    std::ifstream ss_cpu_max("/sys/fs/cgroup/cpu.max", std::ios::in | std::ios::binary);
    ss_cpu_max >> cfs_quota >> cfs_period;

    ಯದಿ (ss_cpu_max.fail() || cfs_quota < 1 || cfs_period < 1) /* values must not be 0 or negative */
        ಪ್ರತ್ಯಾಗಮ 0;

    ಪ್ರತ್ಯಾಗಮ (unsigned)max(1, cfs_quota/cfs_period);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ
unsigned get_ಸಂಖ್ಯೆ_Of_CPUs_CFSv1()
{
    ಪೂರ್ಣಾಙ್ಕ cfs_quota = 0;
    {
        std::ifstream ss_period("/sys/fs/cgroup/cpu/cpu.cfs_quota_us", std::ios::in | std::ios::binary);
        ss_period >> cfs_quota;

        ಯದಿ (ss_period.fail() || cfs_quota < 1) /* cfs_quota must not be 0 or negative */
            ಪ್ರತ್ಯಾಗಮ 0;
    }

    ಪೂರ್ಣಾಙ್ಕ cfs_period = 0;
    {
        std::ifstream ss_quota("/sys/fs/cgroup/cpu/cpu.cfs_period_us", std::ios::in | std::ios::binary);
        ss_quota >> cfs_period;

        ಯದಿ (ss_quota.fail() || cfs_period < 1)
            ಪ್ರತ್ಯಾಗಮ 0;
    }

    ಪ್ರತ್ಯಾಗಮ (unsigned)max(1, cfs_quota/cfs_period);
}
#endif

template <typename T> ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ
T minNonZero(ಧ್ರುವ T& val_1, ಧ್ರುವ T& val_2)
{
    ಯದಿ ((val_1 != 0) && (val_2 != 0))
        ಪ್ರತ್ಯಾಗಮ std::min(val_1, val_2);
    ಪ್ರತ್ಯಾಗಮ (val_1 != 0) ? val_1 : val_2;
}

#ifndef OPENCV_DISABLE_THREAD_SUPPORT
ಸ್ಥಿರ
ಪೂರ್ಣಾಙ್ಕ get_ಸಂಖ್ಯೆ_Of_CPUs_2()
{
#ifndef OPENCV_SEMIHOSTING
    /*
     * Logic here is to try different methods of getting CPU counts and return
     * the minimum most value as it has high probablity of being right and safe.
     * Return 1 if we get 0 or not found on all methods.
    */
#ಯದಿ !defined(__MINGW32__) /* not implemented (2020-03) */

    /*
     * Check for this standard C++11 way, we do not return directly because
     * running in a docker or K8s environment will mean this is the host
     * machines config not the containers or pods and as per docs this value
     * must be "considered only a hint".
    */
    unsigned ncpus = std::thread::hardware_concurrency(); /* If the value is not well defined or not computable, returns 0 */
#ಅನ್ಯಥಾ
    unsigned ncpus = 0; /* 0 means we have to find out some other way */
#endif

#ಯದಿ defined _WIN32

    SYSTEM_INFO sysinfo = {};
#ಯದಿ (defined(_M_ARM) || defined(_M_ARM64) || defined(_M_ARM64EC) || defined(_M_X64) || defined(WINRT)) && _WIN32_WINNT >= 0x501
    GetNativeSystemInfo( &sysinfo );
#ಅನ್ಯಥಾ
    GetSystemInfo( &sysinfo );
#endif
    unsigned ncpus_sysinfo = sysinfo.dw_ಸಂಖ್ಯೆ_Of_Processors;
    ncpus = minNonZero(ncpus, ncpus_sysinfo);

#ನೋಚೇದ್ಯದಿ defined __APPLE__

    ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_CPU=0;
    ಪೂರ್ಣಾಙ್ಕ mib[4];
    size_t len = ಪರಿಮಾಣ(ಅಂಕೆ_CPU);

    /* set the mib for hw.ncpu */
    mib[0] = CTL_HW;
    mib[1] = HW_AVAILCPU;  // alternatively, try HW_NCPU;

    /* get the number of CPUs from the system */
    sysctl(mib, 2, &ಅಂಕೆ_CPU, &len, ರಿಕ್ತ, 0);

    ಯದಿ( ಅಂಕೆ_CPU < 1 )
    {
        mib[1] = HW_NCPU;
        sysctl( mib, 2, &ಅಂಕೆ_CPU, &len, ರಿಕ್ತ, 0 );

        ಯದಿ( ಅಂಕೆ_CPU < 1 )
            ಅಂಕೆ_CPU = 1;
    }

    ncpus = minNonZero(ncpus, (unsigned)ಅಂಕೆ_CPU);

#ನೋಚೇದ್ಯದಿ defined CV_CPU_GROUPS_1

#ಯದಿ defined CV_HAVE_CGROUPS
    ಸ್ಥಿರ unsigned ncpus_impl_cpuset = (unsigned)get_ಸಂಖ್ಯೆ_Of_CPUs_Impl("/sys/fs/cgroup/cpuset/cpuset.cpus");
    ncpus = minNonZero(ncpus, ncpus_impl_cpuset);

    ಸ್ಥಿರ unsigned ncpus_impl_cfs_v1 = get_ಸಂಖ್ಯೆ_Of_CPUs_CFSv1();
    ncpus = minNonZero(ncpus, ncpus_impl_cfs_v1);

    ಸ್ಥಿರ unsigned ncpus_impl_cfs_v2 = get_ಸಂಖ್ಯೆ_Of_CPUs_CFSv2();
    ncpus = minNonZero(ncpus, ncpus_impl_cfs_v2);
#endif

    ಸ್ಥಿರ unsigned ncpus_impl_devices = (unsigned)get_ಸಂಖ್ಯೆ_Of_CPUs_Impl("/sys/devices/system/cpu/online");
    ncpus = minNonZero(ncpus, ncpus_impl_devices);

#endif

#ಯದಿ defined _GNU_SOURCE \
    && !defined(__MINGW32__) /* not implemented (2020-03) */ \
    && !defined(__EMSCRIPTEN__) \
    && !defined(__ANDROID__)  // TODO: add check for modern Android NDK

    cpu_set_t cpu_set;
    ಯದಿ (0 == sched_getaffinity(0, ಪರಿಮಾಣ(cpu_set), &cpu_set))
    {
        unsigned cpu_ಎಣಿಕೆ_cpu_set = CPU_COUNT(&cpu_set);
        ncpus = minNonZero(ncpus, cpu_ಎಣಿಕೆ_cpu_set);
    }

#endif

#ಯದಿ !defined(_WIN32) && !defined(__APPLE__) && defined(_SC_NPROCESSORS_ONLN)

    ಸ್ಥಿರ unsigned cpu_ಎಣಿಕೆ_sysconf = (unsigned)sysconf( _SC_NPROCESSORS_ONLN );
    ncpus = minNonZero(ncpus, cpu_ಎಣಿಕೆ_sysconf);
#ನೋಚೇದ್ಯದಿ defined (__QNX__)
    ಸ್ಥಿರ unsigned cpu_ಎಣಿಕೆ_sysconf = _syspage_ptr->ಅಂಕೆ_cpu;
    ncpus = minNonZero(ncpus, cpu_ಎಣಿಕೆ_sysconf);
#endif

    ಪ್ರತ್ಯಾಗಮ ncpus != 0 ? ncpus : 1;
#ಅನ್ಯಥಾ //  OPENCV_SEMIHOSTING
    ಪ್ರತ್ಯಾಗಮ 1;
#endif //OPENCV_SEMIHOSTING
}

ಪೂರ್ಣಾಙ್ಕ get_ಸಂಖ್ಯೆ_Of_CPUs()
{
    ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ nCPUs = get_ಸಂಖ್ಯೆ_Of_CPUs_2();
    ಪ್ರತ್ಯಾಗಮ nCPUs;  // cached value
}

#ಅನ್ಯಥಾ  // OPENCV_DISABLE_THREAD_SUPPORT
ಪೂರ್ಣಾಙ್ಕ get_ಸಂಖ್ಯೆ_Of_CPUs()
{
    ಪ್ರತ್ಯಾಗಮ 1;
}
#endif  // OPENCV_DISABLE_THREAD_SUPPORT

ಧ್ರುವ ವರ್ಣ* currentParallelFramework()
{
    std::shared_ptr<ParallelForAPI>& api = getCurrentParallelForAPI();
    ಯದಿ (api)
    {
        ಪ್ರತ್ಯಾಗಮ api->getName();
    }
#ifdef CV_PARALLEL_FRAMEWORK
    ಪ್ರತ್ಯಾಗಮ CV_PARALLEL_FRAMEWORK;
#ಅನ್ಯಥಾ
    ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
#endif
}

}  // namespace cv::
