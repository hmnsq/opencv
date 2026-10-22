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

#ଅନ୍ତର୍ଭାଵ "precomp.hpp"

#ଅନ୍ତର୍ଭାଵ <opencv2/core/utils/configuration.private.hpp>
#ଅନ୍ତର୍ଭାଵ <opencv2/core/utils/trace.private.hpp>

#ଅନ୍ତର୍ଭାଵ "opencv2/core/parallel/parallel_backend.hpp"
#ଅନ୍ତର୍ଭାଵ "parallel/parallel.hpp"

#ଯଦି defined _WIN32 || defined WINCE
    #ଅନ୍ତର୍ଭାଵ <windows.h>
    #undef small
    #undef min
    #undef max
    #undef abs
#endif

#ଯଦି defined __unix__ || defined __APPLE__ || defined __GLIBC__ \
    || defined __HAIKU__ || defined __EMSCRIPTEN__ \
    || defined __FreeBSD__ || defined __NetBSD__ || defined __OpenBSD__
    #ଅନ୍ତର୍ଭାଵ <unistd.h>
    #ଅନ୍ତର୍ଭାଵ <stdio.h>
    #ଅନ୍ତର୍ଭାଵ <sys/types.h>
    #ଅନ୍ତର୍ଭାଵ <fstream>
    #ଯଦି defined __ANDROID__
        #ଅନ୍ତର୍ଭାଵ <sys/sysconf.h>
        #ଅନ୍ତର୍ଭାଵ <sys/syscall.h>
        #ଅନ୍ତର୍ଭାଵ <sched.h>
    #ନଚେତଯଦି defined __APPLE__
        #ଅନ୍ତର୍ଭାଵ <sys/sysctl.h>
    #endif
#endif

#ଯଦି defined (__QNX__)
    #ଅନ୍ତର୍ଭାଵ <sys/syspage.h>
#endif

#ifndef OPENCV_DISABLE_THREAD_SUPPORT
    #ଅନ୍ତର୍ଭାଵ <thread>
#endif

#ifdef _OPENMP
    #define HAVE_OPENMP
#endif

#ifdef __APPLE__
    #define HAVE_GCD
#endif

#ଯଦି defined _MSC_VER
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

#ଯଦି defined HAVE_TBB
    #ifndef TBB_SUPPRESS_DEPRECATED_MESSAGES  // supress warning
    #define TBB_SUPPRESS_DEPRECATED_MESSAGES 1
    #endif
    #ଅନ୍ତର୍ଭାଵ "tbb/tbb.h"
    #ଅନ୍ତର୍ଭାଵ "tbb/task.h"
    #ଯଦି TBB_INTERFACE_VERSION >= 8000
        #ଅନ୍ତର୍ଭାଵ "tbb/task_arena.h"
    #endif
    #undef min
    #undef max
#ନଚେତଯଦି defined HAVE_HPX
    #ଅନ୍ତର୍ଭାଵ <hpx/parallel/algorithms/for_loop.hpp>
    #ଅନ୍ତର୍ଭାଵ <hpx/parallel/execution.hpp>
    //
    #ଅନ୍ତର୍ଭାଵ <hpx/hpx_start.hpp>
    #ଅନ୍ତର୍ଭାଵ <hpx/hpx_suspend.hpp>
    #ଅନ୍ତର୍ଭାଵ <hpx/include/apply.hpp>
    #ଅନ୍ତର୍ଭାଵ <hpx/util/yield_while.hpp>
    #ଅନ୍ତର୍ଭାଵ <hpx/include/threadmanager.hpp>

#ନଚେତଯଦି defined HAVE_OPENMP
    #ଅନ୍ତର୍ଭାଵ <omp.h>
#ନଚେତଯଦି defined HAVE_GCD
    #ଅନ୍ତର୍ଭାଵ <dispatch/dispatch.h>
    #ଅନ୍ତର୍ଭାଵ <pthread.h>
#ନଚେତଯଦି defined HAVE_CONCURRENCY
    #ଅନ୍ତର୍ଭାଵ <ppl.h>
#ନଚେତଯଦି defined HAVE_PTHREADS_PF
    #ଅନ୍ତର୍ଭାଵ <pthread.h>
#endif


#ଯଦି defined HAVE_TBB
#  define CV_PARALLEL_FRAMEWORK "tbb"
#ନଚେତଯଦି defined HAVE_HPX
#  define CV_PARALLEL_FRAMEWORK "hpx"
#ନଚେତଯଦି defined HAVE_OPENMP
#  define CV_PARALLEL_FRAMEWORK "openmp"
#ନଚେତଯଦି defined HAVE_GCD
#  define CV_PARALLEL_FRAMEWORK "gcd"
#ନଚେତଯଦି defined WINRT
#  define CV_PARALLEL_FRAMEWORK "winrt-concurrency"
#ନଚେତଯଦି defined HAVE_CONCURRENCY
#  define CV_PARALLEL_FRAMEWORK "ms-concurrency"
#ନଚେତଯଦି defined HAVE_PTHREADS_PF
#  define CV_PARALLEL_FRAMEWORK "pthreads"
#endif

#ଅନ୍ତର୍ଭାଵ <atomic>

#ଅନ୍ତର୍ଭାଵ "parallel_impl.hpp"

#ଅନ୍ତର୍ଭାଵ "opencv2/core/detail/exception_ptr.hpp"  // CV__EXCEPTION_PTR = 1 if std::exception_ptr is available

#ଅନ୍ତର୍ଭାଵ <opencv2/core/utils/fp_control_utils.hpp>
#ଅନ୍ତର୍ଭାଵ <opencv2/core/utils/fp_control.private.hpp>

using ନାମାକାଶ cv;

ନାମାକାଶ cv {

ParallelLoopBody::~ParallelLoopBody() {}

using ନାମାକାଶ cv::parallel;

ନାମାକାଶ {

#ifdef ENABLE_INSTRUMENTATION
    ସ୍ଥିର ଶୂନ୍ୟ SyncNodes(cv::instr::Instr_ଗଣ୍ଠି *p_ଗଣ୍ଠି)
    {
        std::vector<cv::instr::ଗଣ୍ଠି_Data_Tls*> data;
        p_ଗଣ୍ଠି->m_payload.m_tls.gather(data);

        uint64 ticksMax = 0;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା    threads  = 0;
        ପାଇଁ(size_t i = 0; i < data.size(); i++)
        {
            ଯଦି(data[i] && data[i]->m_ticksTotal)
            {
                ticksMax = MAX(ticksMax, data[i]->m_ticksTotal);
                p_ଗଣ୍ଠି->m_payload.m_ticksTotal -= data[i]->m_ticksTotal;
                data[i]->m_ticksTotal = 0;
                threads++;
            }
        }
        p_ଗଣ୍ଠି->m_payload.m_ticksTotal += ticksMax;
        p_ଗଣ୍ଠି->m_payload.m_threads = MAX(p_ଗଣ୍ଠି->m_payload.m_threads, threads);

        ପାଇଁ(size_t i = 0; i < p_ଗଣ୍ଠି->m_childs.size(); i++)
            SyncNodes(p_ଗଣ୍ଠି->m_childs[i]);
    }
#endif

    ଶ୍ରେଣୀ ParallelLoopBodyWrapperContext
    {
    ସର୍ବସାଧାରଣ:
        ParallelLoopBodyWrapperContext(ଅଚଳ cv::ParallelLoopBody& _body, ଅଚଳ cv::Range& _r, ଦ୍ୱିଗୁଣ _nstripes) :
            is_rng_used(ମିଥ୍ୟା), hasException(ମିଥ୍ୟା)
        {

            body = &_body;
            wholeRange = _r;
            ଦ୍ୱିଗୁଣ len = wholeRange.end - wholeRange.start;
            nstripes = cvRound(_nstripes <= 0 ? len : MIN(MAX(_nstripes, 1.), len));

            // propagate main thread state
            rng = cv::theRNG();
#ଯଦି OPENCV_SUPPORTS_FP_DENORMALS_HINT && OPENCV_IMPL_FP_HINTS
            details::saveFPDenormalsState(fp_denormals_base_state);
#endif

#ifdef OPENCV_TRACE
            trace_ମୂଳ_Region = CV_TRACE_NS::details::getCurrentRegion();
            trace_ମୂଳ_Context = CV_TRACE_NS::details::getTraceManager().tls.get();
#endif

#ifdef ENABLE_INSTRUMENTATION
            p_Thread_ମୂଳ = cv::instr::getInstrumentTLSStruct().p_Current_ଗଣ୍ଠି;
#endif
        }
        ଶୂନ୍ୟ finalize()
        {
#ifdef ENABLE_INSTRUMENTATION
            ପାଇଁ(size_t i = 0; i < p_Thread_ମୂଳ->m_childs.size(); i++)
                SyncNodes(p_Thread_ମୂଳ->m_childs[i]);
#endif
            ଯଦି (is_rng_used)
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
            ଯଦି (trace_ମୂଳ_Region)
                CV_TRACE_NS::details::parallelForFinalize(*trace_ମୂଳ_Region);
#endif

            ଯଦି (hasException)
            {
#ଯଦି CV__EXCEPTION_PTR
                std::rethrow_exception(pException);
#ଅନ୍ୟଥା
                CV_Error(Error::StsError, "Exception in parallel_for() body: " + exception_message);
#endif
            }
        }
        ~ParallelLoopBodyWrapperContext() {}

        ଅଚଳ cv::ParallelLoopBody* body;
        cv::Range wholeRange;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା nstripes;
        cv::RNG rng;
        mutable ତାର୍କିକ is_rng_used;
#ifdef OPENCV_TRACE
        CV_TRACE_NS::details::Region* trace_ମୂଳ_Region;
        CV_TRACE_NS::details::TraceManagerThreadLocal* trace_ମୂଳ_Context;
#endif
#ifdef ENABLE_INSTRUMENTATION
        cv::instr::Instr_ଗଣ୍ଠି *p_Thread_ମୂଳ;
#endif
        ତାର୍କିକ hasException;
#ଯଦି CV__EXCEPTION_PTR
        std::exception_ptr pException;
#ଅନ୍ୟଥା
        cv::String exception_message;
#endif
#ଯଦି CV__EXCEPTION_PTR
        ଶୂନ୍ୟ recordException()
#ଅନ୍ୟଥା
        ଶୂନ୍ୟ recordException(ଅଚଳ cv::String& ସନ୍ଦେଶ)
#endif
        {
#ifndef CV_THREAD_SANITIZER
            ଯଦି (!hasException)
#endif
            {
                cv::AutoLock lock(cv::getInitializationMutex());
                ଯଦି (!hasException)
                {
                    hasException = ସତ୍ୟ;
#ଯଦି CV__EXCEPTION_PTR
                    pException = std::current_exception();
#ଅନ୍ୟଥା
                    exception_message = ସନ୍ଦେଶ;
#endif
                }
            }
        }

#ଯଦି OPENCV_SUPPORTS_FP_DENORMALS_HINT && OPENCV_IMPL_FP_HINTS
        details::FPDenormalsModeState fp_denormals_base_state;
#endif

    ବ୍ୟକ୍ତିଗତ:
        ParallelLoopBodyWrapperContext(ଅଚଳ ParallelLoopBodyWrapperContext&); // disabled
        ParallelLoopBodyWrapperContext& operator=(ଅଚଳ ParallelLoopBodyWrapperContext&); // disabled
    };

    ଶ୍ରେଣୀ ParallelLoopBodyWrapper : ସର୍ବସାଧାରଣ cv::ParallelLoopBody
    {
    ସର୍ବସାଧାରଣ:
        ParallelLoopBodyWrapper(ParallelLoopBodyWrapperContext& ctx_) :
            ctx(ctx_)
        {
        }
        ~ParallelLoopBodyWrapper()
        {
        }
        ଶୂନ୍ୟ operator()(ଅଚଳ cv::Range& sr) ଅଚଳ CV_OVERRIDE
        {
#ifdef OPENCV_TRACE
            // TODO CV_TRACE_NS::details::setCurrentRegion(rootRegion);
            ଯଦି (ctx.trace_ମୂଳ_Region && ctx.trace_ମୂଳ_Context)
                CV_TRACE_NS::details::parallel_For_Set_ମୂଳ_Region(*ctx.trace_ମୂଳ_Region, *ctx.trace_ମୂଳ_Context);
            CV__TRACE_OPENCV_FUNCTION_NAME("parallel_for_body");
            ଯଦି (ctx.trace_ମୂଳ_Region)
                CV_TRACE_NS::details::parallelForAttachNestedRegion(*ctx.trace_ମୂଳ_Region);
#endif

#ifdef ENABLE_INSTRUMENTATION
            {
                cv::instr::InstrTLSStruct *pInstrTLS = &cv::instr::getInstrumentTLSStruct();
                pInstrTLS->p_Current_ଗଣ୍ଠି = ctx.p_Thread_ମୂଳ; // Initialize TLS node for thread
            }
            CV_INSTRUMENT_REGION();
#endif

            // propagate main thread state
            cv::theRNG() = ctx.rng;
#ଯଦି OPENCV_SUPPORTS_FP_DENORMALS_HINT && OPENCV_IMPL_FP_HINTS
            FPDenormalsIgnoreHintScope fp_denormals_scope(ctx.fp_denormals_base_state);
#endif

            cv::Range r;
            cv::Range wholeRange = ctx.wholeRange;
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା nstripes = ctx.nstripes;
            r.start = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(wholeRange.start +
                            ((uint64)sr.start*(wholeRange.end - wholeRange.start) + nstripes/2)/nstripes);
            r.end = sr.end >= nstripes ? wholeRange.end : (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(wholeRange.start +
                            ((uint64)sr.end*(wholeRange.end - wholeRange.start) + nstripes/2)/nstripes);

#ifdef OPENCV_TRACE
            CV_TRACE_ARG_ମୂଲ୍ୟ(range_start, "range.start", (int64)r.start);
            CV_TRACE_ARG_ମୂଲ୍ୟ(range_end, "range.end", (int64)r.end);
#endif

            ଚେଷ୍ଟା
            {
                (*ctx.body)(r);
            }
#ଯଦି CV__EXCEPTION_PTR
            ଧର (...)
            {
                ctx.recordException();
            }
#ଅନ୍ୟଥା
            ଧର (ଅଚଳ cv::Exception& e)
            {
                ctx.recordException(e.what());
            }
            ଧର (ଅଚଳ std::exception& e)
            {
                ctx.recordException(e.what());
            }
            ଧର (...)
            {
                ctx.recordException("Unknown exception");
            }
#endif

            ଯଦି (!ctx.is_rng_used && !(cv::theRNG() == ctx.rng))
                ctx.is_rng_used = ସତ୍ୟ;
        }
        cv::Range stripeRange() ଅଚଳ { ଫେରନ୍ତୁ cv::Range(0, ctx.nstripes); }

    ସୁରକ୍ଷିତ:
        ParallelLoopBodyWrapperContext& ctx;
    };

#ଯଦି defined HAVE_TBB
    ଶ୍ରେଣୀ ProxyLoopBody : ସର୍ବସାଧାରଣ ParallelLoopBodyWrapper
    {
    ସର୍ବସାଧାରଣ:
        ProxyLoopBody(ParallelLoopBodyWrapperContext& ctx_)
        : ParallelLoopBodyWrapper(ctx_)
        {}

        ଶୂନ୍ୟ operator ()(ଅଚଳ tbb::blocked_range<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>& range) ଅଚଳ
        {
            ଏହି->ParallelLoopBodyWrapper::operator()(cv::Range(range.begin(), range.end()));
        }

        ଶୂନ୍ୟ operator ()() ଅଚଳ  // run parallel job
        {
            cv::Range range = ଏହି->stripeRange();
            tbb::parallel_for(tbb::blocked_range<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(range.start, range.end), *ଏହି);
        }
    };
#ନଚେତଯଦି defined HAVE_HPX
    ଶ୍ରେଣୀ ProxyLoopBody : ସର୍ବସାଧାରଣ ParallelLoopBodyWrapper
    {
    ସର୍ବସାଧାରଣ:
        ProxyLoopBody(ParallelLoopBodyWrapperContext& ctx_)
                : ParallelLoopBodyWrapper(ctx_)
        {}

        ଶୂନ୍ୟ operator ()() ଅଚଳ  // run parallel job
        {
            cv::Range stripeRange = ଏହି->stripeRange();
            hpx::parallel::for_loop(
                    hpx::parallel::execution::par,
                    stripeRange.start, stripeRange.end,
                    [&](ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା &i) { ;
                        ଏହି->ParallelLoopBodyWrapper::operator()(
                                cv::Range(i, i + 1));
                    });
        }
    };
#ନଚେତଯଦି defined HAVE_OPENMP
    ପ୍ରକାରବ୍ୟାଖ୍ୟା ParallelLoopBodyWrapper ProxyLoopBody;
#ନଚେତଯଦି defined HAVE_GCD
    ପ୍ରକାରବ୍ୟାଖ୍ୟା ParallelLoopBodyWrapper ProxyLoopBody;
    ସ୍ଥିର ଶୂନ୍ୟ block_function(ଶୂନ୍ୟ* context, size_t index)
    {
        ProxyLoopBody* ptr_body = static_cast<ProxyLoopBody*>(context);
        (*ptr_body)(cv::Range((ପୂର୍ଣ୍ଣସଂଖ୍ୟା)index, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)index + 1));
    }
#ନଚେତଯଦି defined WINRT || defined HAVE_CONCURRENCY
    ଶ୍ରେଣୀ ProxyLoopBody : ସର୍ବସାଧାରଣ ParallelLoopBodyWrapper
    {
    ସର୍ବସାଧାରଣ:
        ProxyLoopBody(ParallelLoopBodyWrapperContext& ctx)
        : ParallelLoopBodyWrapper(ctx)
        {}

        ଶୂନ୍ୟ operator ()(ପୂର୍ଣ୍ଣସଂଖ୍ୟା i) ଅଚଳ
        {
            ଏହି->ParallelLoopBodyWrapper::operator()(cv::Range(i, i + 1));
        }
    };

    // PPL's default auto_partitioner silently skips whole chunks of the range on
    // ARM64 (unfixed MSVC bug, developercommunity.visualstudio.com/t/1027444).
    template <typename Body>
    ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ pplParallelFor(ପୂର୍ଣ୍ଣସଂଖ୍ୟା first, ପୂର୍ଣ୍ଣସଂଖ୍ୟା last, ଅଚଳ Body& body)
    {
#ଯଦି defined(_M_ARM64) || defined(_M_ARM64EC)
        Concurrency::parallel_for(first, last, body, Concurrency::static_partitioner());
#ଅନ୍ୟଥା
        Concurrency::parallel_for(first, last, body);
#endif
    }
#ଅନ୍ୟଥା
    ପ୍ରକାରବ୍ୟାଖ୍ୟା ParallelLoopBodyWrapper ProxyLoopBody;
#endif

#ଯଦି defined HAVE_TBB
    #ଯଦି TBB_INTERFACE_VERSION >= 8000
        ସ୍ଥିର tbb::task_arena tbbArena(tbb::task_arena::automatic);
    #ଅନ୍ୟଥା
        ସ୍ଥିର tbb::task_scheduler_init tbbScheduler(tbb::task_scheduler_init::deferred);
    #endif
#ନଚେତଯଦି defined HAVE_HPX
// nothing for HPX
#ନଚେତଯଦି defined HAVE_OPENMP
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା _initMaxThreads()
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା maxThreads = omp_get_max_threads();
    ଫେରନ୍ତୁ maxThreads;
}
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_Threads_Max = _initMaxThreads();
#ନଚେତଯଦି defined HAVE_GCD
// nothing for GCD
#ନଚେତଯଦି defined WINRT
// nothing for WINRT
#ନଚେତଯଦି defined HAVE_CONCURRENCY

ଶ୍ରେଣୀ SchedPtr
{
    Concurrency::Scheduler* sched_;
ସର୍ବସାଧାରଣ:
    Concurrency::Scheduler* operator->() { ଫେରନ୍ତୁ sched_; }
    operator Concurrency::Scheduler*() { ଫେରନ୍ତୁ sched_; }

    ଶୂନ୍ୟ operator=(Concurrency::Scheduler* sched)
    {
        ଯଦି (sched_) sched_->Release();
        sched_ = sched;
    }

    SchedPtr() : sched_(0) {}
    ~SchedPtr() {}
};
ସ୍ଥିର SchedPtr pplScheduler;

#endif

} // namespace anon

/* ================================   parallel_for_  ================================ */

ସ୍ଥିର ଶୂନ୍ୟ parallel_for_impl(ଅଚଳ cv::Range& range, ଅଚଳ cv::ParallelLoopBody& body, ଦ୍ୱିଗୁଣ nstripes); // forward declaration

ଶୂନ୍ୟ parallel_for_(ଅଚଳ cv::Range& range, ଅଚଳ cv::ParallelLoopBody& body, ଦ୍ୱିଗୁଣ nstripes)
{
#ifdef OPENCV_TRACE
    CV__TRACE_OPENCV_FUNCTION_NAME_("parallel_for", 0);
    CV_TRACE_ARG_ମୂଲ୍ୟ(range_start, "range.start", (int64)range.start);
    CV_TRACE_ARG_ମୂଲ୍ୟ(range_end, "range.end", (int64)range.end);
    CV_TRACE_ARG_ମୂଲ୍ୟ(nstripes, "nstripes", (int64)nstripes);
#endif

    CV_INSTRUMENT_REGION_MT_FORK();
    ଯଦି (range.empty())
        ଫେରନ୍ତୁ;

    ସ୍ଥିର std::atomic<ତାର୍କିକ> flagNestedParallelFor(ମିଥ୍ୟା);
    ତାର୍କିକ isNotNestedRegion = !flagNestedParallelFor.load();
    ଯଦି (isNotNestedRegion)
      isNotNestedRegion = !flagNestedParallelFor.exchange(ସତ୍ୟ);
    ଯଦି (isNotNestedRegion)
    {
        ଚେଷ୍ଟା
        {
            parallel_for_impl(range, body, nstripes);
            flagNestedParallelFor = ମିଥ୍ୟା;
        }
        ଧର (...)
        {
            flagNestedParallelFor = ମିଥ୍ୟା;
            ଫିଙ୍ଗ;
        }
    }
    ଅନ୍ୟଥା // nested parallel_for_() calls are not parallelized
    {
        CV_UNUSED(nstripes);
        body(range);
    }
}

ସ୍ଥିର
ଶୂନ୍ୟ parallel_for_cb(ପୂର୍ଣ୍ଣସଂଖ୍ୟା start, ପୂର୍ଣ୍ଣସଂଖ୍ୟା end, ଶୂନ୍ୟ* data)
{
    CV_DbgAssert(data);
    ଅଚଳ cv::ParallelLoopBody& body = *(ଅଚଳ cv::ParallelLoopBody*)data;
    body(Range(start, end));
}

ସ୍ଥିର ଶୂନ୍ୟ parallel_for_impl(ଅଚଳ cv::Range& range, ଅଚଳ cv::ParallelLoopBody& body, ଦ୍ୱିଗୁଣ nstripes)
{
    using ନାମାକାଶ cv::parallel;
    ଯଦି ((ଅଙ୍କ_Threads < 0 || ଅଙ୍କ_Threads > 1) && range.end - range.start > 1)
    {
        ParallelLoopBodyWrapperContext ctx(body, range, nstripes);
        ProxyLoopBody pbody(ctx);
        cv::Range stripeRange = pbody.stripeRange();
        ଯଦି( stripeRange.end - stripeRange.start == 1 )
        {
            body(range);
            ଫେରନ୍ତୁ;
        }

        std::shared_ptr<ParallelForAPI>& api = getCurrentParallelForAPI();
        ଯଦି (api)
        {
            CV_CheckEQ(stripeRange.start, 0, "");
            api->parallel_for(stripeRange.end, parallel_for_cb, (ଶୂନ୍ୟ*)&pbody);
            ctx.finalize();  // propagate exceptions if exists
            ଫେରନ୍ତୁ;
        }

#ifdef CV_PARALLEL_FRAMEWORK
#ଯଦି defined HAVE_TBB

#ଯଦି TBB_INTERFACE_VERSION >= 8000
        tbbArena.execute(pbody);
#ଅନ୍ୟଥା
        pbody();
#endif

#ନଚେତଯଦି defined HAVE_HPX
        pbody();

#ନଚେତଯଦି defined HAVE_OPENMP

        #pragma omp parallel ପାଇଁ schedule(dynamic) ଅଙ୍କ_threads(ଅଙ୍କ_Threads > 0 ? ଅଙ୍କ_Threads : ଅଙ୍କ_Threads_Max)
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = stripeRange.start; i < stripeRange.end; ++i)
            pbody(Range(i, i + 1));

#ନଚେତଯଦି defined HAVE_GCD

        dispatch_queue_t concurrent_queue = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0);
        dispatch_apply_f(stripeRange.end - stripeRange.start, concurrent_queue, &pbody, block_function);

#ନଚେତଯଦି defined WINRT

        pplParallelFor(stripeRange.start, stripeRange.end, pbody);

#ନଚେତଯଦି defined HAVE_CONCURRENCY

        ଯଦି(!pplScheduler || pplScheduler->Id() == Concurrency::CurrentScheduler::Id())
        {
            pplParallelFor(stripeRange.start, stripeRange.end, pbody);
        }
        ଅନ୍ୟଥା
        {
            pplScheduler->Attach();
            pplParallelFor(stripeRange.start, stripeRange.end, pbody);
            Concurrency::CurrentScheduler::Detach();
        }

#ନଚେତଯଦି defined HAVE_PTHREADS_PF

        parallel_for_pthreads(pbody.stripeRange(), pbody, pbody.stripeRange().size());

#ଅନ୍ୟଥା

#error You have hacked and compiling with unsupported parallel framework

#endif

        ctx.finalize();  // propagate exceptions if exists
        ଫେରନ୍ତୁ;
#endif // CV_PARALLEL_FRAMEWORK
    }

    body(range);
}


ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_ଅଙ୍କ_Threads(ଶୂନ୍ୟ)
{
    std::shared_ptr<ParallelForAPI>& api = getCurrentParallelForAPI();
    ଯଦି (api)
    {
        ଫେରନ୍ତୁ api->get_ଅଙ୍କ_Threads();
    }

    ଯଦି (ଅଙ୍କ_Threads == 0)
        ଫେରନ୍ତୁ 1;

#ଯଦି defined HAVE_TBB

#ଯଦି TBB_INTERFACE_VERSION >= 9100
    ଫେରନ୍ତୁ tbbArena.max_concurrency();
#ନଚେତଯଦି TBB_INTERFACE_VERSION >= 8000
    ଫେରନ୍ତୁ ଅଙ୍କ_Threads > 0
        ? ଅଙ୍କ_Threads
        : tbb::task_scheduler_init::default_ଅଙ୍କ_threads();
#ଅନ୍ୟଥା
    ଫେରନ୍ତୁ tbbScheduler.is_active()
           ? ଅଙ୍କ_Threads
           : tbb::task_scheduler_init::default_ଅଙ୍କ_threads();
#endif

#ନଚେତଯଦି defined HAVE_HPX
    ଫେରନ୍ତୁ ଅଙ୍କ_Threads;

#ନଚେତଯଦି defined HAVE_OPENMP

    ଫେରନ୍ତୁ ଅଙ୍କ_Threads > 0
           ? ଅଙ୍କ_Threads
           : ଅଙ୍କ_Threads_Max;


#ନଚେତଯଦି defined HAVE_GCD

    ଫେରନ୍ତୁ cv::get_ସଂଖ୍ୟା_Of_CPUs(); // the GCD thread pool limit

#ନଚେତଯଦି defined WINRT

    ଫେରନ୍ତୁ 0;

#ନଚେତଯଦି defined HAVE_CONCURRENCY

    ଫେରନ୍ତୁ (pplScheduler == 0)
        ? Concurrency::CurrentScheduler::Get()->Get_ସଂଖ୍ୟା_Of_Virtual_Processors()
        : (1 + pplScheduler->Get_ସଂଖ୍ୟା_Of_Virtual_Processors());

#ନଚେତଯଦି defined HAVE_PTHREADS_PF

        ଫେରନ୍ତୁ parallel_pthreads_get_threads_ଅଙ୍କ();

#ଅନ୍ୟଥା

    ଫେରନ୍ତୁ 1;

#endif
}

unsigned default_ସଂଖ୍ୟା_Of_Threads()
{
#ifdef __ANDROID__
    // many modern phones/tables have 4-core CPUs. Let's use no more
    // than 2 threads by default not to overheat the devices
    ଅଚଳ unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା default_ସଂଖ୍ୟା_of_threads = 2;
#ଅନ୍ୟଥା
    ଅଚଳ unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା default_ସଂଖ୍ୟା_of_threads = (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା)std::max(1, cv::get_ସଂଖ୍ୟା_Of_CPUs());
#endif

    unsigned result = default_ସଂଖ୍ୟା_of_threads;

    ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା config_ଅଙ୍କ_threads = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)utils::getConfigurationParameterSizeT("OPENCV_FOR_THREADS_NUM", 0);

    ଯଦି (config_ଅଙ୍କ_threads)
    {
        result = (unsigned)std::max(1, config_ଅଙ୍କ_threads);
        //do we need upper limit of threads number?
    }
    ଫେରନ୍ତୁ result;
}

ଶୂନ୍ୟ set_ଅଙ୍କ_Threads( ପୂର୍ଣ୍ଣସଂଖ୍ୟା threads_ )
{
    CV_UNUSED(threads_);

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା threads = (threads_ < 0) ? default_ସଂଖ୍ୟା_Of_Threads() : (unsigned)threads_;
    ଅଙ୍କ_Threads = threads;

    std::shared_ptr<ParallelForAPI>& api = getCurrentParallelForAPI();
    ଯଦି (api)
    {
        api->set_ଅଙ୍କ_Threads(ଅଙ୍କ_Threads);
    }

#ifdef HAVE_TBB

#ଯଦି TBB_INTERFACE_VERSION >= 8000
    ଯଦି(tbbArena.is_active()) tbbArena.terminate();
    ଯଦି(threads > 0) tbbArena.initialize(threads);
#ଅନ୍ୟଥା
    ଯଦି(tbbScheduler.is_active()) tbbScheduler.terminate();
    ଯଦି(threads > 0) tbbScheduler.initialize(threads);
#endif

#ନଚେତଯଦି defined HAVE_HPX
    ଫେରନ୍ତୁ; // nothing needed as numThreads is used

#ନଚେତଯଦି defined HAVE_OPENMP

    ଫେରନ୍ତୁ; // nothing needed as num_threads clause is used in #pragma omp parallel for

#ନଚେତଯଦି defined HAVE_GCD

    // unsupported
    // there is only private dispatch_queue_set_width() and only for desktop

#ନଚେତଯଦି defined WINRT

    ଫେରନ୍ତୁ;

#ନଚେତଯଦି defined HAVE_CONCURRENCY

    ଯଦି (threads <= 0)
    {
        pplScheduler = 0;
    }
    ଅନ୍ୟଥା ଯଦି (threads == 1)
    {
        // Concurrency always uses >=2 threads, so we just disable it if 1 thread is requested
        ଅଙ୍କ_Threads = 0;
    }
    ଅନ୍ୟଥା ଯଦି (pplScheduler == 0 || 1 + pplScheduler->Get_ସଂଖ୍ୟା_Of_Virtual_Processors() != (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା)threads)
    {
        pplScheduler = Concurrency::Scheduler::Create(Concurrency::SchedulerPolicy(2,
                       Concurrency::MinConcurrency, threads-1,
                       Concurrency::MaxConcurrency, threads-1));
    }

#ନଚେତଯଦି defined HAVE_PTHREADS_PF

    parallel_pthreads_set_threads_ଅଙ୍କ(threads);

#endif
}


ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_Thread_ଅଙ୍କ()
{
    std::shared_ptr<ParallelForAPI>& api = getCurrentParallelForAPI();
    ଯଦି (api)
    {
        ଫେରନ୍ତୁ api->get_Thread_ଅଙ୍କ();
    }

#ଯଦି defined HAVE_TBB
    #ଯଦି TBB_INTERFACE_VERSION >= 9100
        ଫେରନ୍ତୁ tbb::this_task_arena::current_thread_index();
    #ନଚେତଯଦି TBB_INTERFACE_VERSION >= 8000
        ଫେରନ୍ତୁ tbb::task_arena::current_thread_index();
    #ଅନ୍ୟଥା
        ଫେରନ୍ତୁ 0;
    #endif
#ନଚେତଯଦି defined HAVE_HPX
    ଫେରନ୍ତୁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(hpx::get_ଅଙ୍କ_worker_threads());
#ନଚେତଯଦି defined HAVE_OPENMP
    ଫେରନ୍ତୁ omp_get_thread_ଅଙ୍କ();
#ନଚେତଯଦି defined HAVE_GCD
    ଫେରନ୍ତୁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(size_t)(ଶୂନ୍ୟ*)pthread_self(); // no zero-based indexing
#ନଚେତଯଦି defined WINRT
    ଫେରନ୍ତୁ 0;
#ନଚେତଯଦି defined HAVE_CONCURRENCY
    ଫେରନ୍ତୁ std::max(0, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)Concurrency::Context::VirtualProcessorId()); // zero for master thread, unique number for others but not necessary 1,2,3,...
#ନଚେତଯଦି defined HAVE_PTHREADS_PF
    ଫେରନ୍ତୁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(size_t)(ଶୂନ୍ୟ*)pthread_self(); // no zero-based indexing
#ଅନ୍ୟଥା
    ଫେରନ୍ତୁ 0;
#endif
}


#ଯଦି defined __linux__ || defined __GLIBC__ || defined __HAIKU__ || defined __ANDROID__
  #define CV_CPU_GROUPS_1
#endif

#ଯଦି defined __linux__ || defined __ANDROID__
  #define CV_HAVE_CGROUPS 1
#endif

#ଯଦି defined CV_CPU_GROUPS_1
ସ୍ଥିର ଧାଡ଼ିଭିତରେ
std::string getFileContents(ଅଚଳ ଅକ୍ଷର *filename)
{
    std::ifstream ifs(filename);
    ଯଦି (!ifs.is_open())
        ଫେରନ୍ତୁ std::string();

    std::string content( (std::istreambuf_iterator<ଅକ୍ଷର>(ifs) ),
                         (std::istreambuf_iterator<ଅକ୍ଷର>()    ) );

    ଯଦି (ifs.fail())
        ଫେରନ୍ତୁ std::string();

    ଫେରନ୍ତୁ content;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ
ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_ସଂଖ୍ୟା_Of_CPUs_Impl(ଅଚଳ ଅକ୍ଷର *filename)
{
   std::string file_contents = getFileContents(filename);
   ଯଦି(file_contents.empty())
       ଫେରନ୍ତୁ 0;

   ଅକ୍ଷର *pbuf = const_cast<ଅକ୍ଷର*>(file_contents.c_str());
   //parse string of form "0-1,3,5-7,10,13-15"
   ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpusAvailable = 0;

   ଯେତେବେଳେ(*pbuf)
   {
      ଅଚଳ ଅକ୍ଷର* pos = pbuf;
      ତାର୍କିକ range = ମିଥ୍ୟା;
      ଯେତେବେଳେ(*pbuf && *pbuf != ',')
      {
          ଯଦି(*pbuf == '-') range = ସତ୍ୟ;
          ++pbuf;
      }
      ଯଦି(*pbuf) *pbuf++ = 0;
      ଯଦି(!range)
        ++cpusAvailable;
      ଅନ୍ୟଥା
      {
          ପୂର୍ଣ୍ଣସଂଖ୍ୟା rstart = 0, rend = 0;
          sscanf(pos, "%d-%d", &rstart, &rend);
          cpusAvailable += rend - rstart + 1;
      }

   }
   ଫେରନ୍ତୁ cpusAvailable;
}
#endif

#ଯଦି defined CV_HAVE_CGROUPS
ସ୍ଥିର ଧାଡ଼ିଭିତରେ
unsigned get_ସଂଖ୍ୟା_Of_CPUs_CFSv2()
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା cfs_quota = 0;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା cfs_period = 0;

    std::ifstream ss_cpu_max("/sys/fs/cgroup/cpu.max", std::ios::in | std::ios::binary);
    ss_cpu_max >> cfs_quota >> cfs_period;

    ଯଦି (ss_cpu_max.fail() || cfs_quota < 1 || cfs_period < 1) /* values must not be 0 or negative */
        ଫେରନ୍ତୁ 0;

    ଫେରନ୍ତୁ (unsigned)max(1, cfs_quota/cfs_period);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ
unsigned get_ସଂଖ୍ୟା_Of_CPUs_CFSv1()
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା cfs_quota = 0;
    {
        std::ifstream ss_period("/sys/fs/cgroup/cpu/cpu.cfs_quota_us", std::ios::in | std::ios::binary);
        ss_period >> cfs_quota;

        ଯଦି (ss_period.fail() || cfs_quota < 1) /* cfs_quota must not be 0 or negative */
            ଫେରନ୍ତୁ 0;
    }

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା cfs_period = 0;
    {
        std::ifstream ss_quota("/sys/fs/cgroup/cpu/cpu.cfs_period_us", std::ios::in | std::ios::binary);
        ss_quota >> cfs_period;

        ଯଦି (ss_quota.fail() || cfs_period < 1)
            ଫେରନ୍ତୁ 0;
    }

    ଫେରନ୍ତୁ (unsigned)max(1, cfs_quota/cfs_period);
}
#endif

template <typename T> ସ୍ଥିର ଧାଡ଼ିଭିତରେ
T minNonZero(ଅଚଳ T& val_1, ଅଚଳ T& val_2)
{
    ଯଦି ((val_1 != 0) && (val_2 != 0))
        ଫେରନ୍ତୁ std::min(val_1, val_2);
    ଫେରନ୍ତୁ (val_1 != 0) ? val_1 : val_2;
}

#ifndef OPENCV_DISABLE_THREAD_SUPPORT
ସ୍ଥିର
ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_ସଂଖ୍ୟା_Of_CPUs_2()
{
#ifndef OPENCV_SEMIHOSTING
    /*
     * Logic here is to try different methods of getting CPU counts and return
     * the minimum most value as it has high probablity of being right and safe.
     * Return 1 if we get 0 or not found on all methods.
    */
#ଯଦି !defined(__MINGW32__) /* not implemented (2020-03) */

    /*
     * Check for this standard C++11 way, we do not return directly because
     * running in a docker or K8s environment will mean this is the host
     * machines config not the containers or pods and as per docs this value
     * must be "considered only a hint".
    */
    unsigned ncpus = std::thread::hardware_concurrency(); /* If the value is not well defined or not computable, returns 0 */
#ଅନ୍ୟଥା
    unsigned ncpus = 0; /* 0 means we have to find out some other way */
#endif

#ଯଦି defined _WIN32

    SYSTEM_INFO sysinfo = {};
#ଯଦି (defined(_M_ARM) || defined(_M_ARM64) || defined(_M_ARM64EC) || defined(_M_X64) || defined(WINRT)) && _WIN32_WINNT >= 0x501
    GetNativeSystemInfo( &sysinfo );
#ଅନ୍ୟଥା
    GetSystemInfo( &sysinfo );
#endif
    unsigned ncpus_sysinfo = sysinfo.dw_ସଂଖ୍ୟା_Of_Processors;
    ncpus = minNonZero(ncpus, ncpus_sysinfo);

#ନଚେତଯଦି defined __APPLE__

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_CPU=0;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା mib[4];
    size_t len = ଆକାର(ଅଙ୍କ_CPU);

    /* set the mib for hw.ncpu */
    mib[0] = CTL_HW;
    mib[1] = HW_AVAILCPU;  // alternatively, try HW_NCPU;

    /* get the number of CPUs from the system */
    sysctl(mib, 2, &ଅଙ୍କ_CPU, &len, ଖାଲି, 0);

    ଯଦି( ଅଙ୍କ_CPU < 1 )
    {
        mib[1] = HW_NCPU;
        sysctl( mib, 2, &ଅଙ୍କ_CPU, &len, ଖାଲି, 0 );

        ଯଦି( ଅଙ୍କ_CPU < 1 )
            ଅଙ୍କ_CPU = 1;
    }

    ncpus = minNonZero(ncpus, (unsigned)ଅଙ୍କ_CPU);

#ନଚେତଯଦି defined CV_CPU_GROUPS_1

#ଯଦି defined CV_HAVE_CGROUPS
    ସ୍ଥିର unsigned ncpus_impl_cpuset = (unsigned)get_ସଂଖ୍ୟା_Of_CPUs_Impl("/sys/fs/cgroup/cpuset/cpuset.cpus");
    ncpus = minNonZero(ncpus, ncpus_impl_cpuset);

    ସ୍ଥିର unsigned ncpus_impl_cfs_v1 = get_ସଂଖ୍ୟା_Of_CPUs_CFSv1();
    ncpus = minNonZero(ncpus, ncpus_impl_cfs_v1);

    ସ୍ଥିର unsigned ncpus_impl_cfs_v2 = get_ସଂଖ୍ୟା_Of_CPUs_CFSv2();
    ncpus = minNonZero(ncpus, ncpus_impl_cfs_v2);
#endif

    ସ୍ଥିର unsigned ncpus_impl_devices = (unsigned)get_ସଂଖ୍ୟା_Of_CPUs_Impl("/sys/devices/system/cpu/online");
    ncpus = minNonZero(ncpus, ncpus_impl_devices);

#endif

#ଯଦି defined _GNU_SOURCE \
    && !defined(__MINGW32__) /* not implemented (2020-03) */ \
    && !defined(__EMSCRIPTEN__) \
    && !defined(__ANDROID__)  // TODO: add check for modern Android NDK

    cpu_set_t cpu_set;
    ଯଦି (0 == sched_getaffinity(0, ଆକାର(cpu_set), &cpu_set))
    {
        unsigned cpu_ଗଣନା_cpu_set = CPU_COUNT(&cpu_set);
        ncpus = minNonZero(ncpus, cpu_ଗଣନା_cpu_set);
    }

#endif

#ଯଦି !defined(_WIN32) && !defined(__APPLE__) && defined(_SC_NPROCESSORS_ONLN)

    ସ୍ଥିର unsigned cpu_ଗଣନା_sysconf = (unsigned)sysconf( _SC_NPROCESSORS_ONLN );
    ncpus = minNonZero(ncpus, cpu_ଗଣନା_sysconf);
#ନଚେତଯଦି defined (__QNX__)
    ସ୍ଥିର unsigned cpu_ଗଣନା_sysconf = _syspage_ptr->ଅଙ୍କ_cpu;
    ncpus = minNonZero(ncpus, cpu_ଗଣନା_sysconf);
#endif

    ଫେରନ୍ତୁ ncpus != 0 ? ncpus : 1;
#ଅନ୍ୟଥା //  OPENCV_SEMIHOSTING
    ଫେରନ୍ତୁ 1;
#endif //OPENCV_SEMIHOSTING
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_ସଂଖ୍ୟା_Of_CPUs()
{
    ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା nCPUs = get_ସଂଖ୍ୟା_Of_CPUs_2();
    ଫେରନ୍ତୁ nCPUs;  // cached value
}

#ଅନ୍ୟଥା  // OPENCV_DISABLE_THREAD_SUPPORT
ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_ସଂଖ୍ୟା_Of_CPUs()
{
    ଫେରନ୍ତୁ 1;
}
#endif  // OPENCV_DISABLE_THREAD_SUPPORT

ଅଚଳ ଅକ୍ଷର* currentParallelFramework()
{
    std::shared_ptr<ParallelForAPI>& api = getCurrentParallelForAPI();
    ଯଦି (api)
    {
        ଫେରନ୍ତୁ api->getName();
    }
#ifdef CV_PARALLEL_FRAMEWORK
    ଫେରନ୍ତୁ CV_PARALLEL_FRAMEWORK;
#ଅନ୍ୟଥା
    ଫେରନ୍ତୁ ଖାଲି;
#endif
}

}  // namespace cv::
