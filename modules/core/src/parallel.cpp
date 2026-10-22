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

#जर defined _WIN32 || defined WINCE
    #अन्तर्भाव <windows.h>
    #undef small
    #undef min
    #undef max
    #undef असल
#endif

#जर defined __unix__ || defined __APPLE__ || defined __GLIBC__ \
    || defined __HAIKU__ || defined __EMSCRIPTEN__ \
    || defined __FreeBSD__ || defined __NetBSD__ || defined __OpenBSD__
    #अन्तर्भाव <unistd.h>
    #अन्तर्भाव <stdio.h>
    #अन्तर्भाव <sys/types.h>
    #अन्तर्भाव <fstream>
    #जर defined __ANDROID__
        #अन्तर्भाव <sys/sysconf.h>
        #अन्तर्भाव <sys/syscall.h>
        #अन्तर्भाव <sched.h>
    #किंवाजर defined __APPLE__
        #अन्तर्भाव <sys/sysctl.h>
    #endif
#endif

#जर defined (__QNX__)
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

#जर defined _MSC_VER
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

#जर defined HAVE_TBB
    #ifndef TBB_SUPPRESS_DEPRECATED_MESSAGES  // supress warning
    #define TBB_SUPPRESS_DEPRECATED_MESSAGES 1
    #endif
    #अन्तर्भाव "tbb/tbb.h"
    #अन्तर्भाव "tbb/task.h"
    #जर TBB_INTERFACE_VERSION >= 8000
        #अन्तर्भाव "tbb/task_arena.h"
    #endif
    #undef min
    #undef max
#किंवाजर defined HAVE_HPX
    #अन्तर्भाव <hpx/parallel/algorithms/for_loop.hpp>
    #अन्तर्भाव <hpx/parallel/execution.hpp>
    //
    #अन्तर्भाव <hpx/hpx_start.hpp>
    #अन्तर्भाव <hpx/hpx_suspend.hpp>
    #अन्तर्भाव <hpx/include/apply.hpp>
    #अन्तर्भाव <hpx/util/yield_while.hpp>
    #अन्तर्भाव <hpx/include/threadmanager.hpp>

#किंवाजर defined HAVE_OPENMP
    #अन्तर्भाव <omp.h>
#किंवाजर defined HAVE_GCD
    #अन्तर्भाव <dispatch/dispatch.h>
    #अन्तर्भाव <pthread.h>
#किंवाजर defined HAVE_CONCURRENCY
    #अन्तर्भाव <ppl.h>
#किंवाजर defined HAVE_PTHREADS_PF
    #अन्तर्भाव <pthread.h>
#endif


#जर defined HAVE_TBB
#  define CV_PARALLEL_FRAMEWORK "tbb"
#किंवाजर defined HAVE_HPX
#  define CV_PARALLEL_FRAMEWORK "hpx"
#किंवाजर defined HAVE_OPENMP
#  define CV_PARALLEL_FRAMEWORK "openmp"
#किंवाजर defined HAVE_GCD
#  define CV_PARALLEL_FRAMEWORK "gcd"
#किंवाजर defined WINRT
#  define CV_PARALLEL_FRAMEWORK "winrt-concurrency"
#किंवाजर defined HAVE_CONCURRENCY
#  define CV_PARALLEL_FRAMEWORK "ms-concurrency"
#किंवाजर defined HAVE_PTHREADS_PF
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
    स्थिर शून्य SyncNodes(cv::instr::Instr_गाठ *p_गाठ)
    {
        std::vector<cv::instr::गाठ_Data_Tls*> data;
        p_गाठ->m_payload.m_tls.gather(data);

        uint64 ticksMax = 0;
        पूर्णांक    threads  = 0;
        साठी(माप_प्रकार i = 0; i < data.size(); i++)
        {
            जर(data[i] && data[i]->m_ticksTotal)
            {
                ticksMax = MAX(ticksMax, data[i]->m_ticksTotal);
                p_गाठ->m_payload.m_ticksTotal -= data[i]->m_ticksTotal;
                data[i]->m_ticksTotal = 0;
                threads++;
            }
        }
        p_गाठ->m_payload.m_ticksTotal += ticksMax;
        p_गाठ->m_payload.m_threads = MAX(p_गाठ->m_payload.m_threads, threads);

        साठी(माप_प्रकार i = 0; i < p_गाठ->m_childs.size(); i++)
            SyncNodes(p_गाठ->m_childs[i]);
    }
#endif

    वर्ग ParallelLoopBodyWrapperContext
    {
    सार्वजनिक:
        ParallelLoopBodyWrapperContext(स्थिरांक cv::ParallelLoopBody& _body, स्थिरांक cv::Range& _r, द्विगुण _nstripes) :
            is_rng_used(असत्य), hasException(असत्य)
        {

            body = &_body;
            wholeRange = _r;
            द्विगुण len = wholeRange.end - wholeRange.start;
            nstripes = cvRound(_nstripes <= 0 ? len : MIN(MAX(_nstripes, 1.), len));

            // propagate main thread state
            rng = cv::theRNG();
#जर OPENCV_SUPPORTS_FP_DENORMALS_HINT && OPENCV_IMPL_FP_HINTS
            details::saveFPDenormalsState(fp_denormals_base_state);
#endif

#ifdef OPENCV_TRACE
            trace_मूळ_Region = CV_TRACE_NS::details::getCurrentRegion();
            trace_मूळ_Context = CV_TRACE_NS::details::getTraceManager().tls.get();
#endif

#ifdef ENABLE_INSTRUMENTATION
            p_Thread_मूळ = cv::instr::getInstrumentTLSStruct().p_Current_गाठ;
#endif
        }
        शून्य finalize()
        {
#ifdef ENABLE_INSTRUMENTATION
            साठी(माप_प्रकार i = 0; i < p_Thread_मूळ->m_childs.size(); i++)
                SyncNodes(p_Thread_मूळ->m_childs[i]);
#endif
            जर (is_rng_used)
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
            जर (trace_मूळ_Region)
                CV_TRACE_NS::details::parallelForFinalize(*trace_मूळ_Region);
#endif

            जर (hasException)
            {
#जर CV__EXCEPTION_PTR
                std::rethrow_exception(pException);
#अन्यथा
                CV_Error(Error::StsError, "Exception in parallel_for() body: " + exception_message);
#endif
            }
        }
        ~ParallelLoopBodyWrapperContext() {}

        स्थिरांक cv::ParallelLoopBody* body;
        cv::Range wholeRange;
        पूर्णांक nstripes;
        cv::RNG rng;
        mutable तार्किक is_rng_used;
#ifdef OPENCV_TRACE
        CV_TRACE_NS::details::Region* trace_मूळ_Region;
        CV_TRACE_NS::details::TraceManagerThreadLocal* trace_मूळ_Context;
#endif
#ifdef ENABLE_INSTRUMENTATION
        cv::instr::Instr_गाठ *p_Thread_मूळ;
#endif
        तार्किक hasException;
#जर CV__EXCEPTION_PTR
        std::exception_ptr pException;
#अन्यथा
        cv::String exception_message;
#endif
#जर CV__EXCEPTION_PTR
        शून्य recordException()
#अन्यथा
        शून्य recordException(स्थिरांक cv::String& संदेश)
#endif
        {
#ifndef CV_THREAD_SANITIZER
            जर (!hasException)
#endif
            {
                cv::AutoLock lock(cv::getInitializationMutex());
                जर (!hasException)
                {
                    hasException = सत्य;
#जर CV__EXCEPTION_PTR
                    pException = std::current_exception();
#अन्यथा
                    exception_message = संदेश;
#endif
                }
            }
        }

#जर OPENCV_SUPPORTS_FP_DENORMALS_HINT && OPENCV_IMPL_FP_HINTS
        details::FPDenormalsModeState fp_denormals_base_state;
#endif

    खाजगी:
        ParallelLoopBodyWrapperContext(स्थिरांक ParallelLoopBodyWrapperContext&); // disabled
        ParallelLoopBodyWrapperContext& चालक=(स्थिरांक ParallelLoopBodyWrapperContext&); // disabled
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
        शून्य चालक()(स्थिरांक cv::Range& sr) स्थिरांक CV_OVERRIDE
        {
#ifdef OPENCV_TRACE
            // TODO CV_TRACE_NS::details::setCurrentRegion(rootRegion);
            जर (ctx.trace_मूळ_Region && ctx.trace_मूळ_Context)
                CV_TRACE_NS::details::parallel_For_Set_मूळ_Region(*ctx.trace_मूळ_Region, *ctx.trace_मूळ_Context);
            CV__TRACE_OPENCV_FUNCTION_NAME("parallel_for_body");
            जर (ctx.trace_मूळ_Region)
                CV_TRACE_NS::details::parallelForAttachNestedRegion(*ctx.trace_मूळ_Region);
#endif

#ifdef ENABLE_INSTRUMENTATION
            {
                cv::instr::InstrTLSStruct *pInstrTLS = &cv::instr::getInstrumentTLSStruct();
                pInstrTLS->p_Current_गाठ = ctx.p_Thread_मूळ; // Initialize TLS node for thread
            }
            CV_INSTRUMENT_REGION();
#endif

            // propagate main thread state
            cv::theRNG() = ctx.rng;
#जर OPENCV_SUPPORTS_FP_DENORMALS_HINT && OPENCV_IMPL_FP_HINTS
            FPDenormalsIgnoreHintScope fp_denormals_scope(ctx.fp_denormals_base_state);
#endif

            cv::Range r;
            cv::Range wholeRange = ctx.wholeRange;
            पूर्णांक nstripes = ctx.nstripes;
            r.start = (पूर्णांक)(wholeRange.start +
                            ((uint64)sr.start*(wholeRange.end - wholeRange.start) + nstripes/2)/nstripes);
            r.end = sr.end >= nstripes ? wholeRange.end : (पूर्णांक)(wholeRange.start +
                            ((uint64)sr.end*(wholeRange.end - wholeRange.start) + nstripes/2)/nstripes);

#ifdef OPENCV_TRACE
            CV_TRACE_ARG_मूल्य(range_start, "range.start", (int64)r.start);
            CV_TRACE_ARG_मूल्य(range_end, "range.end", (int64)r.end);
#endif

            प्रयत्न
            {
                (*ctx.body)(r);
            }
#जर CV__EXCEPTION_PTR
            पकडा (...)
            {
                ctx.recordException();
            }
#अन्यथा
            पकडा (स्थिरांक cv::Exception& e)
            {
                ctx.recordException(e.what());
            }
            पकडा (स्थिरांक std::exception& e)
            {
                ctx.recordException(e.what());
            }
            पकडा (...)
            {
                ctx.recordException("Unknown exception");
            }
#endif

            जर (!ctx.is_rng_used && !(cv::theRNG() == ctx.rng))
                ctx.is_rng_used = सत्य;
        }
        cv::Range stripeRange() स्थिरांक { परत cv::Range(0, ctx.nstripes); }

    संरक्षित:
        ParallelLoopBodyWrapperContext& ctx;
    };

#जर defined HAVE_TBB
    वर्ग ProxyLoopBody : सार्वजनिक ParallelLoopBodyWrapper
    {
    सार्वजनिक:
        ProxyLoopBody(ParallelLoopBodyWrapperContext& ctx_)
        : ParallelLoopBodyWrapper(ctx_)
        {}

        शून्य चालक ()(स्थिरांक tbb::blocked_range<पूर्णांक>& range) स्थिरांक
        {
            हे->ParallelLoopBodyWrapper::चालक()(cv::Range(range.begin(), range.end()));
        }

        शून्य चालक ()() स्थिरांक  // run parallel job
        {
            cv::Range range = हे->stripeRange();
            tbb::parallel_for(tbb::blocked_range<पूर्णांक>(range.start, range.end), *हे);
        }
    };
#किंवाजर defined HAVE_HPX
    वर्ग ProxyLoopBody : सार्वजनिक ParallelLoopBodyWrapper
    {
    सार्वजनिक:
        ProxyLoopBody(ParallelLoopBodyWrapperContext& ctx_)
                : ParallelLoopBodyWrapper(ctx_)
        {}

        शून्य चालक ()() स्थिरांक  // run parallel job
        {
            cv::Range stripeRange = हे->stripeRange();
            hpx::parallel::for_loop(
                    hpx::parallel::execution::par,
                    stripeRange.start, stripeRange.end,
                    [&](स्थिरांक पूर्णांक &i) { ;
                        हे->ParallelLoopBodyWrapper::चालक()(
                                cv::Range(i, i + 1));
                    });
        }
    };
#किंवाजर defined HAVE_OPENMP
    प्रकारव्याख्या ParallelLoopBodyWrapper ProxyLoopBody;
#किंवाजर defined HAVE_GCD
    प्रकारव्याख्या ParallelLoopBodyWrapper ProxyLoopBody;
    स्थिर शून्य block_function(शून्य* context, माप_प्रकार index)
    {
        ProxyLoopBody* ptr_body = static_cast<ProxyLoopBody*>(context);
        (*ptr_body)(cv::Range((पूर्णांक)index, (पूर्णांक)index + 1));
    }
#किंवाजर defined WINRT || defined HAVE_CONCURRENCY
    वर्ग ProxyLoopBody : सार्वजनिक ParallelLoopBodyWrapper
    {
    सार्वजनिक:
        ProxyLoopBody(ParallelLoopBodyWrapperContext& ctx)
        : ParallelLoopBodyWrapper(ctx)
        {}

        शून्य चालक ()(पूर्णांक i) स्थिरांक
        {
            हे->ParallelLoopBodyWrapper::चालक()(cv::Range(i, i + 1));
        }
    };

    // PPL's default auto_partitioner silently skips whole chunks of the range on
    // ARM64 (unfixed MSVC bug, developercommunity.visualstudio.com/t/1027444).
    ढाँचा <typename Body>
    स्थिर ओळीत शून्य pplParallelFor(पूर्णांक first, पूर्णांक last, स्थिरांक Body& body)
    {
#जर defined(_M_ARM64) || defined(_M_ARM64EC)
        Concurrency::parallel_for(first, last, body, Concurrency::static_partitioner());
#अन्यथा
        Concurrency::parallel_for(first, last, body);
#endif
    }
#अन्यथा
    प्रकारव्याख्या ParallelLoopBodyWrapper ProxyLoopBody;
#endif

#जर defined HAVE_TBB
    #जर TBB_INTERFACE_VERSION >= 8000
        स्थिर tbb::task_arena tbbArena(tbb::task_arena::automatic);
    #अन्यथा
        स्थिर tbb::task_scheduler_init tbbScheduler(tbb::task_scheduler_init::deferred);
    #endif
#किंवाजर defined HAVE_HPX
// nothing for HPX
#किंवाजर defined HAVE_OPENMP
स्थिर ओळीत पूर्णांक _initMaxThreads()
{
    पूर्णांक maxThreads = omp_get_max_threads();
    परत maxThreads;
}
स्थिर पूर्णांक अंक_Threads_Max = _initMaxThreads();
#किंवाजर defined HAVE_GCD
// nothing for GCD
#किंवाजर defined WINRT
// nothing for WINRT
#किंवाजर defined HAVE_CONCURRENCY

वर्ग SchedPtr
{
    Concurrency::Scheduler* sched_;
सार्वजनिक:
    Concurrency::Scheduler* चालक->() { परत sched_; }
    चालक Concurrency::Scheduler*() { परत sched_; }

    शून्य चालक=(Concurrency::Scheduler* sched)
    {
        जर (sched_) sched_->Release();
        sched_ = sched;
    }

    SchedPtr() : sched_(0) {}
    ~SchedPtr() {}
};
स्थिर SchedPtr pplScheduler;

#endif

} // namespace anon

/* ================================   parallel_for_  ================================ */

स्थिर शून्य parallel_for_impl(स्थिरांक cv::Range& range, स्थिरांक cv::ParallelLoopBody& body, द्विगुण nstripes); // forward declaration

शून्य parallel_for_(स्थिरांक cv::Range& range, स्थिरांक cv::ParallelLoopBody& body, द्विगुण nstripes)
{
#ifdef OPENCV_TRACE
    CV__TRACE_OPENCV_FUNCTION_NAME_("parallel_for", 0);
    CV_TRACE_ARG_मूल्य(range_start, "range.start", (int64)range.start);
    CV_TRACE_ARG_मूल्य(range_end, "range.end", (int64)range.end);
    CV_TRACE_ARG_मूल्य(nstripes, "nstripes", (int64)nstripes);
#endif

    CV_INSTRUMENT_REGION_MT_FORK();
    जर (range.empty())
        परत;

    स्थिर std::atomic<तार्किक> flagNestedParallelFor(असत्य);
    तार्किक isNotNestedRegion = !flagNestedParallelFor.load();
    जर (isNotNestedRegion)
      isNotNestedRegion = !flagNestedParallelFor.exchange(सत्य);
    जर (isNotNestedRegion)
    {
        प्रयत्न
        {
            parallel_for_impl(range, body, nstripes);
            flagNestedParallelFor = असत्य;
        }
        पकडा (...)
        {
            flagNestedParallelFor = असत्य;
            फेका;
        }
    }
    अन्यथा // nested parallel_for_() calls are not parallelized
    {
        CV_UNUSED(nstripes);
        body(range);
    }
}

स्थिर
शून्य parallel_for_cb(पूर्णांक start, पूर्णांक end, शून्य* data)
{
    CV_DbgAssert(data);
    स्थिरांक cv::ParallelLoopBody& body = *(स्थिरांक cv::ParallelLoopBody*)data;
    body(Range(start, end));
}

स्थिर शून्य parallel_for_impl(स्थिरांक cv::Range& range, स्थिरांक cv::ParallelLoopBody& body, द्विगुण nstripes)
{
    using नामाकाश cv::parallel;
    जर ((अंक_Threads < 0 || अंक_Threads > 1) && range.end - range.start > 1)
    {
        ParallelLoopBodyWrapperContext ctx(body, range, nstripes);
        ProxyLoopBody pbody(ctx);
        cv::Range stripeRange = pbody.stripeRange();
        जर( stripeRange.end - stripeRange.start == 1 )
        {
            body(range);
            परत;
        }

        std::shared_ptr<ParallelForAPI>& api = getCurrentParallelForAPI();
        जर (api)
        {
            CV_CheckEQ(stripeRange.start, 0, "");
            api->parallel_for(stripeRange.end, parallel_for_cb, (शून्य*)&pbody);
            ctx.finalize();  // propagate exceptions if exists
            परत;
        }

#ifdef CV_PARALLEL_FRAMEWORK
#जर defined HAVE_TBB

#जर TBB_INTERFACE_VERSION >= 8000
        tbbArena.execute(pbody);
#अन्यथा
        pbody();
#endif

#किंवाजर defined HAVE_HPX
        pbody();

#किंवाजर defined HAVE_OPENMP

        #pragma omp parallel साठी schedule(dynamic) अंक_threads(अंक_Threads > 0 ? अंक_Threads : अंक_Threads_Max)
        साठी (पूर्णांक i = stripeRange.start; i < stripeRange.end; ++i)
            pbody(Range(i, i + 1));

#किंवाजर defined HAVE_GCD

        dispatch_queue_t concurrent_queue = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0);
        dispatch_apply_f(stripeRange.end - stripeRange.start, concurrent_queue, &pbody, block_function);

#किंवाजर defined WINRT

        pplParallelFor(stripeRange.start, stripeRange.end, pbody);

#किंवाजर defined HAVE_CONCURRENCY

        जर(!pplScheduler || pplScheduler->Id() == Concurrency::CurrentScheduler::Id())
        {
            pplParallelFor(stripeRange.start, stripeRange.end, pbody);
        }
        अन्यथा
        {
            pplScheduler->Attach();
            pplParallelFor(stripeRange.start, stripeRange.end, pbody);
            Concurrency::CurrentScheduler::Detach();
        }

#किंवाजर defined HAVE_PTHREADS_PF

        parallel_for_pthreads(pbody.stripeRange(), pbody, pbody.stripeRange().size());

#अन्यथा

#error You have hacked and compiling with unsupported parallel framework

#endif

        ctx.finalize();  // propagate exceptions if exists
        परत;
#endif // CV_PARALLEL_FRAMEWORK
    }

    body(range);
}


पूर्णांक get_अंक_Threads(शून्य)
{
    std::shared_ptr<ParallelForAPI>& api = getCurrentParallelForAPI();
    जर (api)
    {
        परत api->get_अंक_Threads();
    }

    जर (अंक_Threads == 0)
        परत 1;

#जर defined HAVE_TBB

#जर TBB_INTERFACE_VERSION >= 9100
    परत tbbArena.max_concurrency();
#किंवाजर TBB_INTERFACE_VERSION >= 8000
    परत अंक_Threads > 0
        ? अंक_Threads
        : tbb::task_scheduler_init::default_अंक_threads();
#अन्यथा
    परत tbbScheduler.is_active()
           ? अंक_Threads
           : tbb::task_scheduler_init::default_अंक_threads();
#endif

#किंवाजर defined HAVE_HPX
    परत अंक_Threads;

#किंवाजर defined HAVE_OPENMP

    परत अंक_Threads > 0
           ? अंक_Threads
           : अंक_Threads_Max;


#किंवाजर defined HAVE_GCD

    परत cv::get_संख्या_Of_CPUs(); // the GCD thread pool limit

#किंवाजर defined WINRT

    परत 0;

#किंवाजर defined HAVE_CONCURRENCY

    परत (pplScheduler == 0)
        ? Concurrency::CurrentScheduler::Get()->Get_संख्या_Of_Virtual_Processors()
        : (1 + pplScheduler->Get_संख्या_Of_Virtual_Processors());

#किंवाजर defined HAVE_PTHREADS_PF

        परत parallel_pthreads_get_threads_अंक();

#अन्यथा

    परत 1;

#endif
}

अचिन्हित default_संख्या_Of_Threads()
{
#ifdef __ANDROID__
    // many modern phones/tables have 4-core CPUs. Let's use no more
    // than 2 threads by default not to overheat the devices
    स्थिरांक अचिन्हित पूर्णांक default_संख्या_of_threads = 2;
#अन्यथा
    स्थिरांक अचिन्हित पूर्णांक default_संख्या_of_threads = (अचिन्हित पूर्णांक)std::max(1, cv::get_संख्या_Of_CPUs());
#endif

    अचिन्हित result = default_संख्या_of_threads;

    स्थिर पूर्णांक config_अंक_threads = (पूर्णांक)utils::getConfigurationParameterSizeT("OPENCV_FOR_THREADS_NUM", 0);

    जर (config_अंक_threads)
    {
        result = (अचिन्हित)std::max(1, config_अंक_threads);
        //do we need upper limit of threads number?
    }
    परत result;
}

शून्य set_अंक_Threads( पूर्णांक threads_ )
{
    CV_UNUSED(threads_);

    पूर्णांक threads = (threads_ < 0) ? default_संख्या_Of_Threads() : (अचिन्हित)threads_;
    अंक_Threads = threads;

    std::shared_ptr<ParallelForAPI>& api = getCurrentParallelForAPI();
    जर (api)
    {
        api->set_अंक_Threads(अंक_Threads);
    }

#ifdef HAVE_TBB

#जर TBB_INTERFACE_VERSION >= 8000
    जर(tbbArena.is_active()) tbbArena.terminate();
    जर(threads > 0) tbbArena.initialize(threads);
#अन्यथा
    जर(tbbScheduler.is_active()) tbbScheduler.terminate();
    जर(threads > 0) tbbScheduler.initialize(threads);
#endif

#किंवाजर defined HAVE_HPX
    परत; // nothing needed as numThreads is used

#किंवाजर defined HAVE_OPENMP

    परत; // nothing needed as num_threads clause is used in #pragma omp parallel for

#किंवाजर defined HAVE_GCD

    // unsupported
    // there is only private dispatch_queue_set_width() and only for desktop

#किंवाजर defined WINRT

    परत;

#किंवाजर defined HAVE_CONCURRENCY

    जर (threads <= 0)
    {
        pplScheduler = 0;
    }
    अन्यथा जर (threads == 1)
    {
        // Concurrency always uses >=2 threads, so we just disable it if 1 thread is requested
        अंक_Threads = 0;
    }
    अन्यथा जर (pplScheduler == 0 || 1 + pplScheduler->Get_संख्या_Of_Virtual_Processors() != (अचिन्हित पूर्णांक)threads)
    {
        pplScheduler = Concurrency::Scheduler::Create(Concurrency::SchedulerPolicy(2,
                       Concurrency::MinConcurrency, threads-1,
                       Concurrency::MaxConcurrency, threads-1));
    }

#किंवाजर defined HAVE_PTHREADS_PF

    parallel_pthreads_set_threads_अंक(threads);

#endif
}


पूर्णांक get_Thread_अंक()
{
    std::shared_ptr<ParallelForAPI>& api = getCurrentParallelForAPI();
    जर (api)
    {
        परत api->get_Thread_अंक();
    }

#जर defined HAVE_TBB
    #जर TBB_INTERFACE_VERSION >= 9100
        परत tbb::this_task_arena::current_thread_index();
    #किंवाजर TBB_INTERFACE_VERSION >= 8000
        परत tbb::task_arena::current_thread_index();
    #अन्यथा
        परत 0;
    #endif
#किंवाजर defined HAVE_HPX
    परत (पूर्णांक)(hpx::get_अंक_worker_threads());
#किंवाजर defined HAVE_OPENMP
    परत omp_get_thread_अंक();
#किंवाजर defined HAVE_GCD
    परत (पूर्णांक)(माप_प्रकार)(शून्य*)pthread_self(); // no zero-based indexing
#किंवाजर defined WINRT
    परत 0;
#किंवाजर defined HAVE_CONCURRENCY
    परत std::max(0, (पूर्णांक)Concurrency::Context::VirtualProcessorId()); // zero for master thread, unique number for others but not necessary 1,2,3,...
#किंवाजर defined HAVE_PTHREADS_PF
    परत (पूर्णांक)(माप_प्रकार)(शून्य*)pthread_self(); // no zero-based indexing
#अन्यथा
    परत 0;
#endif
}


#जर defined __linux__ || defined __GLIBC__ || defined __HAIKU__ || defined __ANDROID__
  #define CV_CPU_GROUPS_1
#endif

#जर defined __linux__ || defined __ANDROID__
  #define CV_HAVE_CGROUPS 1
#endif

#जर defined CV_CPU_GROUPS_1
स्थिर ओळीत
std::string getFileContents(स्थिरांक अक्षर *filename)
{
    std::ifstream ifs(filename);
    जर (!ifs.is_open())
        परत std::string();

    std::string content( (std::istreambuf_iterator<अक्षर>(ifs) ),
                         (std::istreambuf_iterator<अक्षर>()    ) );

    जर (ifs.fail())
        परत std::string();

    परत content;
}

स्थिर ओळीत
पूर्णांक get_संख्या_Of_CPUs_Impl(स्थिरांक अक्षर *filename)
{
   std::string file_contents = getFileContents(filename);
   जर(file_contents.empty())
       परत 0;

   अक्षर *pbuf = const_cast<अक्षर*>(file_contents.c_str());
   //parse string of form "0-1,3,5-7,10,13-15"
   पूर्णांक cpusAvailable = 0;

   पर्यंत(*pbuf)
   {
      स्थिरांक अक्षर* pos = pbuf;
      तार्किक range = असत्य;
      पर्यंत(*pbuf && *pbuf != ',')
      {
          जर(*pbuf == '-') range = सत्य;
          ++pbuf;
      }
      जर(*pbuf) *pbuf++ = 0;
      जर(!range)
        ++cpusAvailable;
      अन्यथा
      {
          पूर्णांक rstart = 0, rend = 0;
          माला_पूछो(pos, "%d-%d", &rstart, &rend);
          cpusAvailable += rend - rstart + 1;
      }

   }
   परत cpusAvailable;
}
#endif

#जर defined CV_HAVE_CGROUPS
स्थिर ओळीत
अचिन्हित get_संख्या_Of_CPUs_CFSv2()
{
    पूर्णांक cfs_quota = 0;
    पूर्णांक cfs_period = 0;

    std::ifstream ss_cpu_max("/sys/fs/cgroup/cpu.max", std::ios::in | std::ios::binary);
    ss_cpu_max >> cfs_quota >> cfs_period;

    जर (ss_cpu_max.fail() || cfs_quota < 1 || cfs_period < 1) /* values must not be 0 or negative */
        परत 0;

    परत (अचिन्हित)max(1, cfs_quota/cfs_period);
}

स्थिर ओळीत
अचिन्हित get_संख्या_Of_CPUs_CFSv1()
{
    पूर्णांक cfs_quota = 0;
    {
        std::ifstream ss_period("/sys/fs/cgroup/cpu/cpu.cfs_quota_us", std::ios::in | std::ios::binary);
        ss_period >> cfs_quota;

        जर (ss_period.fail() || cfs_quota < 1) /* cfs_quota must not be 0 or negative */
            परत 0;
    }

    पूर्णांक cfs_period = 0;
    {
        std::ifstream ss_quota("/sys/fs/cgroup/cpu/cpu.cfs_period_us", std::ios::in | std::ios::binary);
        ss_quota >> cfs_period;

        जर (ss_quota.fail() || cfs_period < 1)
            परत 0;
    }

    परत (अचिन्हित)max(1, cfs_quota/cfs_period);
}
#endif

ढाँचा <typename T> स्थिर ओळीत
T minNonZero(स्थिरांक T& val_1, स्थिरांक T& val_2)
{
    जर ((val_1 != 0) && (val_2 != 0))
        परत std::min(val_1, val_2);
    परत (val_1 != 0) ? val_1 : val_2;
}

#ifndef OPENCV_DISABLE_THREAD_SUPPORT
स्थिर
पूर्णांक get_संख्या_Of_CPUs_2()
{
#ifndef OPENCV_SEMIHOSTING
    /*
     * Logic here is to try different methods of getting CPU counts and return
     * the minimum most value as it has high probablity of being right and safe.
     * Return 1 if we get 0 or not found on all methods.
    */
#जर !defined(__MINGW32__) /* not implemented (2020-03) */

    /*
     * Check for this standard C++11 way, we do not return directly because
     * running in a docker or K8s environment will mean this is the host
     * machines config not the containers or pods and as per docs this value
     * must be "considered only a hint".
    */
    अचिन्हित ncpus = std::thread::hardware_concurrency(); /* If the value is not well defined or not computable, returns 0 */
#अन्यथा
    अचिन्हित ncpus = 0; /* 0 means we have to find out some other way */
#endif

#जर defined _WIN32

    SYSTEM_INFO sysinfo = {};
#जर (defined(_M_ARM) || defined(_M_ARM64) || defined(_M_ARM64EC) || defined(_M_X64) || defined(WINRT)) && _WIN32_WINNT >= 0x501
    GetNativeSystemInfo( &sysinfo );
#अन्यथा
    GetSystemInfo( &sysinfo );
#endif
    अचिन्हित ncpus_sysinfo = sysinfo.dw_संख्या_Of_Processors;
    ncpus = minNonZero(ncpus, ncpus_sysinfo);

#किंवाजर defined __APPLE__

    पूर्णांक अंक_CPU=0;
    पूर्णांक mib[4];
    माप_प्रकार len = आकार(अंक_CPU);

    /* set the mib for hw.ncpu */
    mib[0] = CTL_HW;
    mib[1] = HW_AVAILCPU;  // alternatively, try HW_NCPU;

    /* get the number of CPUs from the system */
    sysctl(mib, 2, &अंक_CPU, &len, रिक्त, 0);

    जर( अंक_CPU < 1 )
    {
        mib[1] = HW_NCPU;
        sysctl( mib, 2, &अंक_CPU, &len, रिक्त, 0 );

        जर( अंक_CPU < 1 )
            अंक_CPU = 1;
    }

    ncpus = minNonZero(ncpus, (अचिन्हित)अंक_CPU);

#किंवाजर defined CV_CPU_GROUPS_1

#जर defined CV_HAVE_CGROUPS
    स्थिर अचिन्हित ncpus_impl_cpuset = (अचिन्हित)get_संख्या_Of_CPUs_Impl("/sys/fs/cgroup/cpuset/cpuset.cpus");
    ncpus = minNonZero(ncpus, ncpus_impl_cpuset);

    स्थिर अचिन्हित ncpus_impl_cfs_v1 = get_संख्या_Of_CPUs_CFSv1();
    ncpus = minNonZero(ncpus, ncpus_impl_cfs_v1);

    स्थिर अचिन्हित ncpus_impl_cfs_v2 = get_संख्या_Of_CPUs_CFSv2();
    ncpus = minNonZero(ncpus, ncpus_impl_cfs_v2);
#endif

    स्थिर अचिन्हित ncpus_impl_devices = (अचिन्हित)get_संख्या_Of_CPUs_Impl("/sys/devices/system/cpu/online");
    ncpus = minNonZero(ncpus, ncpus_impl_devices);

#endif

#जर defined _GNU_SOURCE \
    && !defined(__MINGW32__) /* not implemented (2020-03) */ \
    && !defined(__EMSCRIPTEN__) \
    && !defined(__ANDROID__)  // TODO: add check for modern Android NDK

    cpu_set_t cpu_set;
    जर (0 == sched_getaffinity(0, आकार(cpu_set), &cpu_set))
    {
        अचिन्हित cpu_मोजणी_cpu_set = CPU_COUNT(&cpu_set);
        ncpus = minNonZero(ncpus, cpu_मोजणी_cpu_set);
    }

#endif

#जर !defined(_WIN32) && !defined(__APPLE__) && defined(_SC_NPROCESSORS_ONLN)

    स्थिर अचिन्हित cpu_मोजणी_sysconf = (अचिन्हित)sysconf( _SC_NPROCESSORS_ONLN );
    ncpus = minNonZero(ncpus, cpu_मोजणी_sysconf);
#किंवाजर defined (__QNX__)
    स्थिर अचिन्हित cpu_मोजणी_sysconf = _syspage_ptr->अंक_cpu;
    ncpus = minNonZero(ncpus, cpu_मोजणी_sysconf);
#endif

    परत ncpus != 0 ? ncpus : 1;
#अन्यथा //  OPENCV_SEMIHOSTING
    परत 1;
#endif //OPENCV_SEMIHOSTING
}

पूर्णांक get_संख्या_Of_CPUs()
{
    स्थिर पूर्णांक nCPUs = get_संख्या_Of_CPUs_2();
    परत nCPUs;  // cached value
}

#अन्यथा  // OPENCV_DISABLE_THREAD_SUPPORT
पूर्णांक get_संख्या_Of_CPUs()
{
    परत 1;
}
#endif  // OPENCV_DISABLE_THREAD_SUPPORT

स्थिरांक अक्षर* currentParallelFramework()
{
    std::shared_ptr<ParallelForAPI>& api = getCurrentParallelForAPI();
    जर (api)
    {
        परत api->getName();
    }
#ifdef CV_PARALLEL_FRAMEWORK
    परत CV_PARALLEL_FRAMEWORK;
#अन्यथा
    परत रिक्त;
#endif
}

}  // namespace cv::
