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

#अन्तर्भाव "precomp.hpp"

#अन्तर्भाव <opencv2/core/utils/configuration.private.hpp>
#अन्तर्भाव <opencv2/core/utils/trace.private.hpp>

#अन्तर्भाव "opencv2/core/parallel/parallel_backend.hpp"
#अन्तर्भाव "parallel/parallel.hpp"

#यदि defined _WIN32 || defined WINCE
    #अन्तर्भाव <windows.h>
    #undef small
    #undef min
    #undef max
    #undef abs
#endif

#यदि defined __unix__ || defined __APPLE__ || defined __GLIBC__ \
    || defined __HAIKU__ || defined __EMSCRIPTEN__ \
    || defined __FreeBSD__ || defined __NetBSD__ || defined __OpenBSD__
    #अन्तर्भाव <unistd.h>
    #अन्तर्भाव <stdio.h>
    #अन्तर्भाव <sys/types.h>
    #अन्तर्भाव <fstream>
    #यदि defined __ANDROID__
        #अन्तर्भाव <sys/sysconf.h>
        #अन्तर्भाव <sys/syscall.h>
        #अन्तर्भाव <sched.h>
    #नोचेद्यदि defined __APPLE__
        #अन्तर्भाव <sys/sysctl.h>
    #endif
#endif

#यदि defined (__QNX__)
    #अन्तर्भाव <sys/syspage.h>
#endif

#ifndef OPENCV_DISABLE_THREAD_SUPPORT
    #अन्तर्भाव <thread>
#endif

#ifdef _OPENMP
    #define HAVE_OPENMP
#endif

#ifdef __APPLE__
    #define HAVE_GCD
#endif

#यदि defined _MSC_VER
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

#यदि defined HAVE_TBB
    #ifndef TBB_SUPPRESS_DEPRECATED_MESSAGES  // supress warning
    #define TBB_SUPPRESS_DEPRECATED_MESSAGES 1
    #endif
    #अन्तर्भाव "tbb/tbb.h"
    #अन्तर्भाव "tbb/task.h"
    #यदि TBB_INTERFACE_VERSION >= 8000
        #अन्तर्भाव "tbb/task_arena.h"
    #endif
    #undef min
    #undef max
#नोचेद्यदि defined HAVE_HPX
    #अन्तर्भाव <hpx/parallel/algorithms/for_loop.hpp>
    #अन्तर्भाव <hpx/parallel/execution.hpp>
    //
    #अन्तर्भाव <hpx/hpx_start.hpp>
    #अन्तर्भाव <hpx/hpx_suspend.hpp>
    #अन्तर्भाव <hpx/include/apply.hpp>
    #अन्तर्भाव <hpx/util/yield_while.hpp>
    #अन्तर्भाव <hpx/include/threadmanager.hpp>

#नोचेद्यदि defined HAVE_OPENMP
    #अन्तर्भाव <omp.h>
#नोचेद्यदि defined HAVE_GCD
    #अन्तर्भाव <dispatch/dispatch.h>
    #अन्तर्भाव <pthread.h>
#नोचेद्यदि defined HAVE_CONCURRENCY
    #अन्तर्भाव <ppl.h>
#नोचेद्यदि defined HAVE_PTHREADS_PF
    #अन्तर्भाव <pthread.h>
#endif


#यदि defined HAVE_TBB
#  define CV_PARALLEL_FRAMEWORK "tbb"
#नोचेद्यदि defined HAVE_HPX
#  define CV_PARALLEL_FRAMEWORK "hpx"
#नोचेद्यदि defined HAVE_OPENMP
#  define CV_PARALLEL_FRAMEWORK "openmp"
#नोचेद्यदि defined HAVE_GCD
#  define CV_PARALLEL_FRAMEWORK "gcd"
#नोचेद्यदि defined WINRT
#  define CV_PARALLEL_FRAMEWORK "winrt-concurrency"
#नोचेद्यदि defined HAVE_CONCURRENCY
#  define CV_PARALLEL_FRAMEWORK "ms-concurrency"
#नोचेद्यदि defined HAVE_PTHREADS_PF
#  define CV_PARALLEL_FRAMEWORK "pthreads"
#endif

#अन्तर्भाव <atomic>

#अन्तर्भाव "parallel_impl.hpp"

#अन्तर्भाव "opencv2/core/detail/exception_ptr.hpp"  // CV__EXCEPTION_PTR = 1 if std::exception_ptr is available

#अन्तर्भाव <opencv2/core/utils/fp_control_utils.hpp>
#अन्तर्भाव <opencv2/core/utils/fp_control.private.hpp>

using नामाकाश cv;

नामाकाश cv {

ParallelLoopBody::~ParallelLoopBody() {}

using नामाकाश cv::parallel;

नामाकाश {

#ifdef ENABLE_INSTRUMENTATION
    स्थिर शून्य SyncNodes(cv::instr::Instr_गाँठ *p_गाँठ)
    {
        std::vector<cv::instr::गाँठ_Data_Tls*> data;
        p_गाँठ->m_payload.m_tls.gather(data);

        uint64 ticksMax = 0;
        पूर्णाङ्क    threads  = 0;
        प्रति(size_t i = 0; i < data.size(); i++)
        {
            यदि(data[i] && data[i]->m_ticksTotal)
            {
                ticksMax = MAX(ticksMax, data[i]->m_ticksTotal);
                p_गाँठ->m_payload.m_ticksTotal -= data[i]->m_ticksTotal;
                data[i]->m_ticksTotal = 0;
                threads++;
            }
        }
        p_गाँठ->m_payload.m_ticksTotal += ticksMax;
        p_गाँठ->m_payload.m_threads = MAX(p_गाँठ->m_payload.m_threads, threads);

        प्रति(size_t i = 0; i < p_गाँठ->m_childs.size(); i++)
            SyncNodes(p_गाँठ->m_childs[i]);
    }
#endif

    वर्ग ParallelLoopBodyWrapperContext
    {
    सार्वजनिक:
        ParallelLoopBodyWrapperContext(ध्रुव cv::ParallelLoopBody& _body, ध्रुव cv::Range& _r, द्विगुण _nstripes) :
            is_rng_used(असत्य), hasException(असत्य)
        {

            body = &_body;
            wholeRange = _r;
            द्विगुण len = wholeRange.end - wholeRange.start;
            nstripes = cvRound(_nstripes <= 0 ? len : MIN(MAX(_nstripes, 1.), len));

            // propagate main thread state
            rng = cv::theRNG();
#यदि OPENCV_SUPPORTS_FP_DENORMALS_HINT && OPENCV_IMPL_FP_HINTS
            details::saveFPDenormalsState(fp_denormals_base_state);
#endif

#ifdef OPENCV_TRACE
            trace_मूल_Region = CV_TRACE_NS::details::getCurrentRegion();
            trace_मूल_Context = CV_TRACE_NS::details::getTraceManager().tls.get();
#endif

#ifdef ENABLE_INSTRUMENTATION
            p_Thread_मूल = cv::instr::getInstrumentTLSStruct().p_Current_गाँठ;
#endif
        }
        शून्य finalize()
        {
#ifdef ENABLE_INSTRUMENTATION
            प्रति(size_t i = 0; i < p_Thread_मूल->m_childs.size(); i++)
                SyncNodes(p_Thread_मूल->m_childs[i]);
#endif
            यदि (is_rng_used)
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
            यदि (trace_मूल_Region)
                CV_TRACE_NS::details::parallelForFinalize(*trace_मूल_Region);
#endif

            यदि (hasException)
            {
#यदि CV__EXCEPTION_PTR
                std::rethrow_exception(pException);
#अन्यथा
                CV_Error(Error::StsError, "Exception in parallel_for() body: " + exception_message);
#endif
            }
        }
        ~ParallelLoopBodyWrapperContext() {}

        ध्रुव cv::ParallelLoopBody* body;
        cv::Range wholeRange;
        पूर्णाङ्क nstripes;
        cv::RNG rng;
        mutable तार्किक is_rng_used;
#ifdef OPENCV_TRACE
        CV_TRACE_NS::details::Region* trace_मूल_Region;
        CV_TRACE_NS::details::TraceManagerThreadLocal* trace_मूल_Context;
#endif
#ifdef ENABLE_INSTRUMENTATION
        cv::instr::Instr_गाँठ *p_Thread_मूल;
#endif
        तार्किक hasException;
#यदि CV__EXCEPTION_PTR
        std::exception_ptr pException;
#अन्यथा
        cv::String exception_message;
#endif
#यदि CV__EXCEPTION_PTR
        शून्य recordException()
#अन्यथा
        शून्य recordException(ध्रुव cv::String& संदेश)
#endif
        {
#ifndef CV_THREAD_SANITIZER
            यदि (!hasException)
#endif
            {
                cv::AutoLock lock(cv::getInitializationMutex());
                यदि (!hasException)
                {
                    hasException = सत्य;
#यदि CV__EXCEPTION_PTR
                    pException = std::current_exception();
#अन्यथा
                    exception_message = संदेश;
#endif
                }
            }
        }

#यदि OPENCV_SUPPORTS_FP_DENORMALS_HINT && OPENCV_IMPL_FP_HINTS
        details::FPDenormalsModeState fp_denormals_base_state;
#endif

    गुप्त:
        ParallelLoopBodyWrapperContext(ध्रुव ParallelLoopBodyWrapperContext&); // disabled
        ParallelLoopBodyWrapperContext& operator=(ध्रुव ParallelLoopBodyWrapperContext&); // disabled
    };

    वर्ग ParallelLoopBodyWrapper : सार्वजनिक cv::ParallelLoopBody
    {
    सार्वजनिक:
        ParallelLoopBodyWrapper(ParallelLoopBodyWrapperContext& ctx_) :
            ctx(ctx_)
        {
        }
        ~ParallelLoopBodyWrapper()
        {
        }
        शून्य operator()(ध्रुव cv::Range& sr) ध्रुव CV_OVERRIDE
        {
#ifdef OPENCV_TRACE
            // TODO CV_TRACE_NS::details::setCurrentRegion(rootRegion);
            यदि (ctx.trace_मूल_Region && ctx.trace_मूल_Context)
                CV_TRACE_NS::details::parallel_For_Set_मूल_Region(*ctx.trace_मूल_Region, *ctx.trace_मूल_Context);
            CV__TRACE_OPENCV_FUNCTION_NAME("parallel_for_body");
            यदि (ctx.trace_मूल_Region)
                CV_TRACE_NS::details::parallelForAttachNestedRegion(*ctx.trace_मूल_Region);
#endif

#ifdef ENABLE_INSTRUMENTATION
            {
                cv::instr::InstrTLSStruct *pInstrTLS = &cv::instr::getInstrumentTLSStruct();
                pInstrTLS->p_Current_गाँठ = ctx.p_Thread_मूल; // Initialize TLS node for thread
            }
            CV_INSTRUMENT_REGION();
#endif

            // propagate main thread state
            cv::theRNG() = ctx.rng;
#यदि OPENCV_SUPPORTS_FP_DENORMALS_HINT && OPENCV_IMPL_FP_HINTS
            FPDenormalsIgnoreHintScope fp_denormals_scope(ctx.fp_denormals_base_state);
#endif

            cv::Range r;
            cv::Range wholeRange = ctx.wholeRange;
            पूर्णाङ्क nstripes = ctx.nstripes;
            r.start = (पूर्णाङ्क)(wholeRange.start +
                            ((uint64)sr.start*(wholeRange.end - wholeRange.start) + nstripes/2)/nstripes);
            r.end = sr.end >= nstripes ? wholeRange.end : (पूर्णाङ्क)(wholeRange.start +
                            ((uint64)sr.end*(wholeRange.end - wholeRange.start) + nstripes/2)/nstripes);

#ifdef OPENCV_TRACE
            CV_TRACE_ARG_मूल्य(range_start, "range.start", (int64)r.start);
            CV_TRACE_ARG_मूल्य(range_end, "range.end", (int64)r.end);
#endif

            यत्न
            {
                (*ctx.body)(r);
            }
#यदि CV__EXCEPTION_PTR
            गृहाण (...)
            {
                ctx.recordException();
            }
#अन्यथा
            गृहाण (ध्रुव cv::Exception& e)
            {
                ctx.recordException(e.what());
            }
            गृहाण (ध्रुव std::exception& e)
            {
                ctx.recordException(e.what());
            }
            गृहाण (...)
            {
                ctx.recordException("Unknown exception");
            }
#endif

            यदि (!ctx.is_rng_used && !(cv::theRNG() == ctx.rng))
                ctx.is_rng_used = सत्य;
        }
        cv::Range stripeRange() ध्रुव { प्रत्यागम cv::Range(0, ctx.nstripes); }

    रक्षित:
        ParallelLoopBodyWrapperContext& ctx;
    };

#यदि defined HAVE_TBB
    वर्ग ProxyLoopBody : सार्वजनिक ParallelLoopBodyWrapper
    {
    सार्वजनिक:
        ProxyLoopBody(ParallelLoopBodyWrapperContext& ctx_)
        : ParallelLoopBodyWrapper(ctx_)
        {}

        शून्य operator ()(ध्रुव tbb::blocked_range<पूर्णाङ्क>& range) ध्रुव
        {
            एतत्->ParallelLoopBodyWrapper::operator()(cv::Range(range.begin(), range.end()));
        }

        शून्य operator ()() ध्रुव  // run parallel job
        {
            cv::Range range = एतत्->stripeRange();
            tbb::parallel_for(tbb::blocked_range<पूर्णाङ्क>(range.start, range.end), *एतत्);
        }
    };
#नोचेद्यदि defined HAVE_HPX
    वर्ग ProxyLoopBody : सार्वजनिक ParallelLoopBodyWrapper
    {
    सार्वजनिक:
        ProxyLoopBody(ParallelLoopBodyWrapperContext& ctx_)
                : ParallelLoopBodyWrapper(ctx_)
        {}

        शून्य operator ()() ध्रुव  // run parallel job
        {
            cv::Range stripeRange = एतत्->stripeRange();
            hpx::parallel::for_loop(
                    hpx::parallel::execution::par,
                    stripeRange.start, stripeRange.end,
                    [&](ध्रुव पूर्णाङ्क &i) { ;
                        एतत्->ParallelLoopBodyWrapper::operator()(
                                cv::Range(i, i + 1));
                    });
        }
    };
#नोचेद्यदि defined HAVE_OPENMP
    प्रकारनिर्देश ParallelLoopBodyWrapper ProxyLoopBody;
#नोचेद्यदि defined HAVE_GCD
    प्रकारनिर्देश ParallelLoopBodyWrapper ProxyLoopBody;
    स्थिर शून्य block_function(शून्य* context, size_t index)
    {
        ProxyLoopBody* ptr_body = static_cast<ProxyLoopBody*>(context);
        (*ptr_body)(cv::Range((पूर्णाङ्क)index, (पूर्णाङ्क)index + 1));
    }
#नोचेद्यदि defined WINRT || defined HAVE_CONCURRENCY
    वर्ग ProxyLoopBody : सार्वजनिक ParallelLoopBodyWrapper
    {
    सार्वजनिक:
        ProxyLoopBody(ParallelLoopBodyWrapperContext& ctx)
        : ParallelLoopBodyWrapper(ctx)
        {}

        शून्य operator ()(पूर्णाङ्क i) ध्रुव
        {
            एतत्->ParallelLoopBodyWrapper::operator()(cv::Range(i, i + 1));
        }
    };

    // PPL's default auto_partitioner silently skips whole chunks of the range on
    // ARM64 (unfixed MSVC bug, developercommunity.visualstudio.com/t/1027444).
    template <typename Body>
    स्थिर अन्तःस्थ शून्य pplParallelFor(पूर्णाङ्क first, पूर्णाङ्क last, ध्रुव Body& body)
    {
#यदि defined(_M_ARM64) || defined(_M_ARM64EC)
        Concurrency::parallel_for(first, last, body, Concurrency::static_partitioner());
#अन्यथा
        Concurrency::parallel_for(first, last, body);
#endif
    }
#अन्यथा
    प्रकारनिर्देश ParallelLoopBodyWrapper ProxyLoopBody;
#endif

#यदि defined HAVE_TBB
    #यदि TBB_INTERFACE_VERSION >= 8000
        स्थिर tbb::task_arena tbbArena(tbb::task_arena::automatic);
    #अन्यथा
        स्थिर tbb::task_scheduler_init tbbScheduler(tbb::task_scheduler_init::deferred);
    #endif
#नोचेद्यदि defined HAVE_HPX
// nothing for HPX
#नोचेद्यदि defined HAVE_OPENMP
स्थिर अन्तःस्थ पूर्णाङ्क _initMaxThreads()
{
    पूर्णाङ्क maxThreads = omp_get_max_threads();
    प्रत्यागम maxThreads;
}
स्थिर पूर्णाङ्क अंक_Threads_Max = _initMaxThreads();
#नोचेद्यदि defined HAVE_GCD
// nothing for GCD
#नोचेद्यदि defined WINRT
// nothing for WINRT
#नोचेद्यदि defined HAVE_CONCURRENCY

वर्ग SchedPtr
{
    Concurrency::Scheduler* sched_;
सार्वजनिक:
    Concurrency::Scheduler* operator->() { प्रत्यागम sched_; }
    operator Concurrency::Scheduler*() { प्रत्यागम sched_; }

    शून्य operator=(Concurrency::Scheduler* sched)
    {
        यदि (sched_) sched_->Release();
        sched_ = sched;
    }

    SchedPtr() : sched_(0) {}
    ~SchedPtr() {}
};
स्थिर SchedPtr pplScheduler;

#endif

} // namespace anon

/* ================================   parallel_for_  ================================ */

स्थिर शून्य parallel_for_impl(ध्रुव cv::Range& range, ध्रुव cv::ParallelLoopBody& body, द्विगुण nstripes); // forward declaration

शून्य parallel_for_(ध्रुव cv::Range& range, ध्रुव cv::ParallelLoopBody& body, द्विगुण nstripes)
{
#ifdef OPENCV_TRACE
    CV__TRACE_OPENCV_FUNCTION_NAME_("parallel_for", 0);
    CV_TRACE_ARG_मूल्य(range_start, "range.start", (int64)range.start);
    CV_TRACE_ARG_मूल्य(range_end, "range.end", (int64)range.end);
    CV_TRACE_ARG_मूल्य(nstripes, "nstripes", (int64)nstripes);
#endif

    CV_INSTRUMENT_REGION_MT_FORK();
    यदि (range.empty())
        प्रत्यागम;

    स्थिर std::atomic<तार्किक> flagNestedParallelFor(असत्य);
    तार्किक isNotNestedRegion = !flagNestedParallelFor.load();
    यदि (isNotNestedRegion)
      isNotNestedRegion = !flagNestedParallelFor.exchange(सत्य);
    यदि (isNotNestedRegion)
    {
        यत्न
        {
            parallel_for_impl(range, body, nstripes);
            flagNestedParallelFor = असत्य;
        }
        गृहाण (...)
        {
            flagNestedParallelFor = असत्य;
            क्षिप;
        }
    }
    अन्यथा // nested parallel_for_() calls are not parallelized
    {
        CV_UNUSED(nstripes);
        body(range);
    }
}

स्थिर
शून्य parallel_for_cb(पूर्णाङ्क start, पूर्णाङ्क end, शून्य* data)
{
    CV_DbgAssert(data);
    ध्रुव cv::ParallelLoopBody& body = *(ध्रुव cv::ParallelLoopBody*)data;
    body(Range(start, end));
}

स्थिर शून्य parallel_for_impl(ध्रुव cv::Range& range, ध्रुव cv::ParallelLoopBody& body, द्विगुण nstripes)
{
    using नामाकाश cv::parallel;
    यदि ((अंक_Threads < 0 || अंक_Threads > 1) && range.end - range.start > 1)
    {
        ParallelLoopBodyWrapperContext ctx(body, range, nstripes);
        ProxyLoopBody pbody(ctx);
        cv::Range stripeRange = pbody.stripeRange();
        यदि( stripeRange.end - stripeRange.start == 1 )
        {
            body(range);
            प्रत्यागम;
        }

        std::shared_ptr<ParallelForAPI>& api = getCurrentParallelForAPI();
        यदि (api)
        {
            CV_CheckEQ(stripeRange.start, 0, "");
            api->parallel_for(stripeRange.end, parallel_for_cb, (शून्य*)&pbody);
            ctx.finalize();  // propagate exceptions if exists
            प्रत्यागम;
        }

#ifdef CV_PARALLEL_FRAMEWORK
#यदि defined HAVE_TBB

#यदि TBB_INTERFACE_VERSION >= 8000
        tbbArena.execute(pbody);
#अन्यथा
        pbody();
#endif

#नोचेद्यदि defined HAVE_HPX
        pbody();

#नोचेद्यदि defined HAVE_OPENMP

        #pragma omp parallel प्रति schedule(dynamic) अंक_threads(अंक_Threads > 0 ? अंक_Threads : अंक_Threads_Max)
        प्रति (पूर्णाङ्क i = stripeRange.start; i < stripeRange.end; ++i)
            pbody(Range(i, i + 1));

#नोचेद्यदि defined HAVE_GCD

        dispatch_queue_t concurrent_queue = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0);
        dispatch_apply_f(stripeRange.end - stripeRange.start, concurrent_queue, &pbody, block_function);

#नोचेद्यदि defined WINRT

        pplParallelFor(stripeRange.start, stripeRange.end, pbody);

#नोचेद्यदि defined HAVE_CONCURRENCY

        यदि(!pplScheduler || pplScheduler->Id() == Concurrency::CurrentScheduler::Id())
        {
            pplParallelFor(stripeRange.start, stripeRange.end, pbody);
        }
        अन्यथा
        {
            pplScheduler->Attach();
            pplParallelFor(stripeRange.start, stripeRange.end, pbody);
            Concurrency::CurrentScheduler::Detach();
        }

#नोचेद्यदि defined HAVE_PTHREADS_PF

        parallel_for_pthreads(pbody.stripeRange(), pbody, pbody.stripeRange().size());

#अन्यथा

#error You have hacked and compiling with unsupported parallel framework

#endif

        ctx.finalize();  // propagate exceptions if exists
        प्रत्यागम;
#endif // CV_PARALLEL_FRAMEWORK
    }

    body(range);
}


पूर्णाङ्क get_अंक_Threads(शून्य)
{
    std::shared_ptr<ParallelForAPI>& api = getCurrentParallelForAPI();
    यदि (api)
    {
        प्रत्यागम api->get_अंक_Threads();
    }

    यदि (अंक_Threads == 0)
        प्रत्यागम 1;

#यदि defined HAVE_TBB

#यदि TBB_INTERFACE_VERSION >= 9100
    प्रत्यागम tbbArena.max_concurrency();
#नोचेद्यदि TBB_INTERFACE_VERSION >= 8000
    प्रत्यागम अंक_Threads > 0
        ? अंक_Threads
        : tbb::task_scheduler_init::default_अंक_threads();
#अन्यथा
    प्रत्यागम tbbScheduler.is_active()
           ? अंक_Threads
           : tbb::task_scheduler_init::default_अंक_threads();
#endif

#नोचेद्यदि defined HAVE_HPX
    प्रत्यागम अंक_Threads;

#नोचेद्यदि defined HAVE_OPENMP

    प्रत्यागम अंक_Threads > 0
           ? अंक_Threads
           : अंक_Threads_Max;


#नोचेद्यदि defined HAVE_GCD

    प्रत्यागम cv::get_संख्या_Of_CPUs(); // the GCD thread pool limit

#नोचेद्यदि defined WINRT

    प्रत्यागम 0;

#नोचेद्यदि defined HAVE_CONCURRENCY

    प्रत्यागम (pplScheduler == 0)
        ? Concurrency::CurrentScheduler::Get()->Get_संख्या_Of_Virtual_Processors()
        : (1 + pplScheduler->Get_संख्या_Of_Virtual_Processors());

#नोचेद्यदि defined HAVE_PTHREADS_PF

        प्रत्यागम parallel_pthreads_get_threads_अंक();

#अन्यथा

    प्रत्यागम 1;

#endif
}

unsigned default_संख्या_Of_Threads()
{
#ifdef __ANDROID__
    // many modern phones/tables have 4-core CPUs. Let's use no more
    // than 2 threads by default not to overheat the devices
    ध्रुव unsigned पूर्णाङ्क default_संख्या_of_threads = 2;
#अन्यथा
    ध्रुव unsigned पूर्णाङ्क default_संख्या_of_threads = (unsigned पूर्णाङ्क)std::max(1, cv::get_संख्या_Of_CPUs());
#endif

    unsigned result = default_संख्या_of_threads;

    स्थिर पूर्णाङ्क config_अंक_threads = (पूर्णाङ्क)utils::getConfigurationParameterSizeT("OPENCV_FOR_THREADS_NUM", 0);

    यदि (config_अंक_threads)
    {
        result = (unsigned)std::max(1, config_अंक_threads);
        //do we need upper limit of threads number?
    }
    प्रत्यागम result;
}

शून्य set_अंक_Threads( पूर्णाङ्क threads_ )
{
    CV_UNUSED(threads_);

    पूर्णाङ्क threads = (threads_ < 0) ? default_संख्या_Of_Threads() : (unsigned)threads_;
    अंक_Threads = threads;

    std::shared_ptr<ParallelForAPI>& api = getCurrentParallelForAPI();
    यदि (api)
    {
        api->set_अंक_Threads(अंक_Threads);
    }

#ifdef HAVE_TBB

#यदि TBB_INTERFACE_VERSION >= 8000
    यदि(tbbArena.is_active()) tbbArena.terminate();
    यदि(threads > 0) tbbArena.initialize(threads);
#अन्यथा
    यदि(tbbScheduler.is_active()) tbbScheduler.terminate();
    यदि(threads > 0) tbbScheduler.initialize(threads);
#endif

#नोचेद्यदि defined HAVE_HPX
    प्रत्यागम; // nothing needed as numThreads is used

#नोचेद्यदि defined HAVE_OPENMP

    प्रत्यागम; // nothing needed as num_threads clause is used in #pragma omp parallel for

#नोचेद्यदि defined HAVE_GCD

    // unsupported
    // there is only private dispatch_queue_set_width() and only for desktop

#नोचेद्यदि defined WINRT

    प्रत्यागम;

#नोचेद्यदि defined HAVE_CONCURRENCY

    यदि (threads <= 0)
    {
        pplScheduler = 0;
    }
    अन्यथा यदि (threads == 1)
    {
        // Concurrency always uses >=2 threads, so we just disable it if 1 thread is requested
        अंक_Threads = 0;
    }
    अन्यथा यदि (pplScheduler == 0 || 1 + pplScheduler->Get_संख्या_Of_Virtual_Processors() != (unsigned पूर्णाङ्क)threads)
    {
        pplScheduler = Concurrency::Scheduler::Create(Concurrency::SchedulerPolicy(2,
                       Concurrency::MinConcurrency, threads-1,
                       Concurrency::MaxConcurrency, threads-1));
    }

#नोचेद्यदि defined HAVE_PTHREADS_PF

    parallel_pthreads_set_threads_अंक(threads);

#endif
}


पूर्णाङ्क get_Thread_अंक()
{
    std::shared_ptr<ParallelForAPI>& api = getCurrentParallelForAPI();
    यदि (api)
    {
        प्रत्यागम api->get_Thread_अंक();
    }

#यदि defined HAVE_TBB
    #यदि TBB_INTERFACE_VERSION >= 9100
        प्रत्यागम tbb::this_task_arena::current_thread_index();
    #नोचेद्यदि TBB_INTERFACE_VERSION >= 8000
        प्रत्यागम tbb::task_arena::current_thread_index();
    #अन्यथा
        प्रत्यागम 0;
    #endif
#नोचेद्यदि defined HAVE_HPX
    प्रत्यागम (पूर्णाङ्क)(hpx::get_अंक_worker_threads());
#नोचेद्यदि defined HAVE_OPENMP
    प्रत्यागम omp_get_thread_अंक();
#नोचेद्यदि defined HAVE_GCD
    प्रत्यागम (पूर्णाङ्क)(size_t)(शून्य*)pthread_self(); // no zero-based indexing
#नोचेद्यदि defined WINRT
    प्रत्यागम 0;
#नोचेद्यदि defined HAVE_CONCURRENCY
    प्रत्यागम std::max(0, (पूर्णाङ्क)Concurrency::Context::VirtualProcessorId()); // zero for master thread, unique number for others but not necessary 1,2,3,...
#नोचेद्यदि defined HAVE_PTHREADS_PF
    प्रत्यागम (पूर्णाङ्क)(size_t)(शून्य*)pthread_self(); // no zero-based indexing
#अन्यथा
    प्रत्यागम 0;
#endif
}


#यदि defined __linux__ || defined __GLIBC__ || defined __HAIKU__ || defined __ANDROID__
  #define CV_CPU_GROUPS_1
#endif

#यदि defined __linux__ || defined __ANDROID__
  #define CV_HAVE_CGROUPS 1
#endif

#यदि defined CV_CPU_GROUPS_1
स्थिर अन्तःस्थ
std::string getFileContents(ध्रुव वर्ण *filename)
{
    std::ifstream ifs(filename);
    यदि (!ifs.is_open())
        प्रत्यागम std::string();

    std::string content( (std::istreambuf_iterator<वर्ण>(ifs) ),
                         (std::istreambuf_iterator<वर्ण>()    ) );

    यदि (ifs.fail())
        प्रत्यागम std::string();

    प्रत्यागम content;
}

स्थिर अन्तःस्थ
पूर्णाङ्क get_संख्या_Of_CPUs_Impl(ध्रुव वर्ण *filename)
{
   std::string file_contents = getFileContents(filename);
   यदि(file_contents.empty())
       प्रत्यागम 0;

   वर्ण *pbuf = const_cast<वर्ण*>(file_contents.c_str());
   //parse string of form "0-1,3,5-7,10,13-15"
   पूर्णाङ्क cpusAvailable = 0;

   यावत्(*pbuf)
   {
      ध्रुव वर्ण* pos = pbuf;
      तार्किक range = असत्य;
      यावत्(*pbuf && *pbuf != ',')
      {
          यदि(*pbuf == '-') range = सत्य;
          ++pbuf;
      }
      यदि(*pbuf) *pbuf++ = 0;
      यदि(!range)
        ++cpusAvailable;
      अन्यथा
      {
          पूर्णाङ्क rstart = 0, rend = 0;
          sscanf(pos, "%d-%d", &rstart, &rend);
          cpusAvailable += rend - rstart + 1;
      }

   }
   प्रत्यागम cpusAvailable;
}
#endif

#यदि defined CV_HAVE_CGROUPS
स्थिर अन्तःस्थ
unsigned get_संख्या_Of_CPUs_CFSv2()
{
    पूर्णाङ्क cfs_quota = 0;
    पूर्णाङ्क cfs_period = 0;

    std::ifstream ss_cpu_max("/sys/fs/cgroup/cpu.max", std::ios::in | std::ios::binary);
    ss_cpu_max >> cfs_quota >> cfs_period;

    यदि (ss_cpu_max.fail() || cfs_quota < 1 || cfs_period < 1) /* values must not be 0 or negative */
        प्रत्यागम 0;

    प्रत्यागम (unsigned)max(1, cfs_quota/cfs_period);
}

स्थिर अन्तःस्थ
unsigned get_संख्या_Of_CPUs_CFSv1()
{
    पूर्णाङ्क cfs_quota = 0;
    {
        std::ifstream ss_period("/sys/fs/cgroup/cpu/cpu.cfs_quota_us", std::ios::in | std::ios::binary);
        ss_period >> cfs_quota;

        यदि (ss_period.fail() || cfs_quota < 1) /* cfs_quota must not be 0 or negative */
            प्रत्यागम 0;
    }

    पूर्णाङ्क cfs_period = 0;
    {
        std::ifstream ss_quota("/sys/fs/cgroup/cpu/cpu.cfs_period_us", std::ios::in | std::ios::binary);
        ss_quota >> cfs_period;

        यदि (ss_quota.fail() || cfs_period < 1)
            प्रत्यागम 0;
    }

    प्रत्यागम (unsigned)max(1, cfs_quota/cfs_period);
}
#endif

template <typename T> स्थिर अन्तःस्थ
T minNonZero(ध्रुव T& val_1, ध्रुव T& val_2)
{
    यदि ((val_1 != 0) && (val_2 != 0))
        प्रत्यागम std::min(val_1, val_2);
    प्रत्यागम (val_1 != 0) ? val_1 : val_2;
}

#ifndef OPENCV_DISABLE_THREAD_SUPPORT
स्थिर
पूर्णाङ्क get_संख्या_Of_CPUs_2()
{
#ifndef OPENCV_SEMIHOSTING
    /*
     * Logic here is to try different methods of getting CPU counts and return
     * the minimum most value as it has high probablity of being right and safe.
     * Return 1 if we get 0 or not found on all methods.
    */
#यदि !defined(__MINGW32__) /* not implemented (2020-03) */

    /*
     * Check for this standard C++11 way, we do not return directly because
     * running in a docker or K8s environment will mean this is the host
     * machines config not the containers or pods and as per docs this value
     * must be "considered only a hint".
    */
    unsigned ncpus = std::thread::hardware_concurrency(); /* If the value is not well defined or not computable, returns 0 */
#अन्यथा
    unsigned ncpus = 0; /* 0 means we have to find out some other way */
#endif

#यदि defined _WIN32

    SYSTEM_INFO sysinfo = {};
#यदि (defined(_M_ARM) || defined(_M_ARM64) || defined(_M_ARM64EC) || defined(_M_X64) || defined(WINRT)) && _WIN32_WINNT >= 0x501
    GetNativeSystemInfo( &sysinfo );
#अन्यथा
    GetSystemInfo( &sysinfo );
#endif
    unsigned ncpus_sysinfo = sysinfo.dw_संख्या_Of_Processors;
    ncpus = minNonZero(ncpus, ncpus_sysinfo);

#नोचेद्यदि defined __APPLE__

    पूर्णाङ्क अंक_CPU=0;
    पूर्णाङ्क mib[4];
    size_t len = परिमाण(अंक_CPU);

    /* set the mib for hw.ncpu */
    mib[0] = CTL_HW;
    mib[1] = HW_AVAILCPU;  // alternatively, try HW_NCPU;

    /* get the number of CPUs from the system */
    sysctl(mib, 2, &अंक_CPU, &len, रिक्त, 0);

    यदि( अंक_CPU < 1 )
    {
        mib[1] = HW_NCPU;
        sysctl( mib, 2, &अंक_CPU, &len, रिक्त, 0 );

        यदि( अंक_CPU < 1 )
            अंक_CPU = 1;
    }

    ncpus = minNonZero(ncpus, (unsigned)अंक_CPU);

#नोचेद्यदि defined CV_CPU_GROUPS_1

#यदि defined CV_HAVE_CGROUPS
    स्थिर unsigned ncpus_impl_cpuset = (unsigned)get_संख्या_Of_CPUs_Impl("/sys/fs/cgroup/cpuset/cpuset.cpus");
    ncpus = minNonZero(ncpus, ncpus_impl_cpuset);

    स्थिर unsigned ncpus_impl_cfs_v1 = get_संख्या_Of_CPUs_CFSv1();
    ncpus = minNonZero(ncpus, ncpus_impl_cfs_v1);

    स्थिर unsigned ncpus_impl_cfs_v2 = get_संख्या_Of_CPUs_CFSv2();
    ncpus = minNonZero(ncpus, ncpus_impl_cfs_v2);
#endif

    स्थिर unsigned ncpus_impl_devices = (unsigned)get_संख्या_Of_CPUs_Impl("/sys/devices/system/cpu/online");
    ncpus = minNonZero(ncpus, ncpus_impl_devices);

#endif

#यदि defined _GNU_SOURCE \
    && !defined(__MINGW32__) /* not implemented (2020-03) */ \
    && !defined(__EMSCRIPTEN__) \
    && !defined(__ANDROID__)  // TODO: add check for modern Android NDK

    cpu_set_t cpu_set;
    यदि (0 == sched_getaffinity(0, परिमाण(cpu_set), &cpu_set))
    {
        unsigned cpu_गिनती_cpu_set = CPU_COUNT(&cpu_set);
        ncpus = minNonZero(ncpus, cpu_गिनती_cpu_set);
    }

#endif

#यदि !defined(_WIN32) && !defined(__APPLE__) && defined(_SC_NPROCESSORS_ONLN)

    स्थिर unsigned cpu_गिनती_sysconf = (unsigned)sysconf( _SC_NPROCESSORS_ONLN );
    ncpus = minNonZero(ncpus, cpu_गिनती_sysconf);
#नोचेद्यदि defined (__QNX__)
    स्थिर unsigned cpu_गिनती_sysconf = _syspage_ptr->अंक_cpu;
    ncpus = minNonZero(ncpus, cpu_गिनती_sysconf);
#endif

    प्रत्यागम ncpus != 0 ? ncpus : 1;
#अन्यथा //  OPENCV_SEMIHOSTING
    प्रत्यागम 1;
#endif //OPENCV_SEMIHOSTING
}

पूर्णाङ्क get_संख्या_Of_CPUs()
{
    स्थिर पूर्णाङ्क nCPUs = get_संख्या_Of_CPUs_2();
    प्रत्यागम nCPUs;  // cached value
}

#अन्यथा  // OPENCV_DISABLE_THREAD_SUPPORT
पूर्णाङ्क get_संख्या_Of_CPUs()
{
    प्रत्यागम 1;
}
#endif  // OPENCV_DISABLE_THREAD_SUPPORT

ध्रुव वर्ण* currentParallelFramework()
{
    std::shared_ptr<ParallelForAPI>& api = getCurrentParallelForAPI();
    यदि (api)
    {
        प्रत्यागम api->getName();
    }
#ifdef CV_PARALLEL_FRAMEWORK
    प्रत्यागम CV_PARALLEL_FRAMEWORK;
#अन्यथा
    प्रत्यागम रिक्त;
#endif
}

}  // namespace cv::
