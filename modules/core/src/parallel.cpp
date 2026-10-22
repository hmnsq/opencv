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
    #undef असल
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
    #अथवायदि defined __APPLE__
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
#अथवायदि defined HAVE_HPX
    #अन्तर्भाव <hpx/parallel/algorithms/for_loop.hpp>
    #अन्तर्भाव <hpx/parallel/execution.hpp>
    //
    #अन्तर्भाव <hpx/hpx_start.hpp>
    #अन्तर्भाव <hpx/hpx_suspend.hpp>
    #अन्तर्भाव <hpx/include/apply.hpp>
    #अन्तर्भाव <hpx/util/yield_while.hpp>
    #अन्तर्भाव <hpx/include/threadmanager.hpp>

#अथवायदि defined HAVE_OPENMP
    #अन्तर्भाव <omp.h>
#अथवायदि defined HAVE_GCD
    #अन्तर्भाव <dispatch/dispatch.h>
    #अन्तर्भाव <pthread.h>
#अथवायदि defined HAVE_CONCURRENCY
    #अन्तर्भाव <ppl.h>
#अथवायदि defined HAVE_PTHREADS_PF
    #अन्तर्भाव <pthread.h>
#endif


#यदि defined HAVE_TBB
#  define CV_PARALLEL_FRAMEWORK "tbb"
#अथवायदि defined HAVE_HPX
#  define CV_PARALLEL_FRAMEWORK "hpx"
#अथवायदि defined HAVE_OPENMP
#  define CV_PARALLEL_FRAMEWORK "openmp"
#अथवायदि defined HAVE_GCD
#  define CV_PARALLEL_FRAMEWORK "gcd"
#अथवायदि defined WINRT
#  define CV_PARALLEL_FRAMEWORK "winrt-concurrency"
#अथवायदि defined HAVE_CONCURRENCY
#  define CV_PARALLEL_FRAMEWORK "ms-concurrency"
#अथवायदि defined HAVE_PTHREADS_PF
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
    स्थिर शून्य SyncNodes(cv::instr::Instr_गाँठो *p_गाँठो)
    {
        std::vector<cv::instr::गाँठो_Data_Tls*> data;
        p_गाँठो->m_payload.m_tls.gather(data);

        uint64 ticksMax = 0;
        पूर्णाङ्क    threads  = 0;
        लागि(माप_प्रकार i = 0; i < data.size(); i++)
        {
            यदि(data[i] && data[i]->m_ticksTotal)
            {
                ticksMax = MAX(ticksMax, data[i]->m_ticksTotal);
                p_गाँठो->m_payload.m_ticksTotal -= data[i]->m_ticksTotal;
                data[i]->m_ticksTotal = 0;
                threads++;
            }
        }
        p_गाँठो->m_payload.m_ticksTotal += ticksMax;
        p_गाँठो->m_payload.m_threads = MAX(p_गाँठो->m_payload.m_threads, threads);

        लागि(माप_प्रकार i = 0; i < p_गाँठो->m_childs.size(); i++)
            SyncNodes(p_गाँठो->m_childs[i]);
    }
#endif

    वर्ग ParallelLoopBodyWrapperContext
    {
    सार्वजनिक:
        ParallelLoopBodyWrapperContext(अचर cv::ParallelLoopBody& _body, अचर cv::Range& _r, द्विगुण _nstripes) :
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
            trace_जरा_Region = CV_TRACE_NS::details::getCurrentRegion();
            trace_जरा_Context = CV_TRACE_NS::details::getTraceManager().tls.get();
#endif

#ifdef ENABLE_INSTRUMENTATION
            p_Thread_जरा = cv::instr::getInstrumentTLSStruct().p_Current_गाँठो;
#endif
        }
        शून्य finalize()
        {
#ifdef ENABLE_INSTRUMENTATION
            लागि(माप_प्रकार i = 0; i < p_Thread_जरा->m_childs.size(); i++)
                SyncNodes(p_Thread_जरा->m_childs[i]);
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
            यदि (trace_जरा_Region)
                CV_TRACE_NS::details::parallelForFinalize(*trace_जरा_Region);
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

        अचर cv::ParallelLoopBody* body;
        cv::Range wholeRange;
        पूर्णाङ्क nstripes;
        cv::RNG rng;
        mutable तार्किक is_rng_used;
#ifdef OPENCV_TRACE
        CV_TRACE_NS::details::Region* trace_जरा_Region;
        CV_TRACE_NS::details::TraceManagerThreadLocal* trace_जरा_Context;
#endif
#ifdef ENABLE_INSTRUMENTATION
        cv::instr::Instr_गाँठो *p_Thread_जरा;
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
        शून्य recordException(अचर cv::String& सन्देश)
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
                    exception_message = सन्देश;
#endif
                }
            }
        }

#यदि OPENCV_SUPPORTS_FP_DENORMALS_HINT && OPENCV_IMPL_FP_HINTS
        details::FPDenormalsModeState fp_denormals_base_state;
#endif

    गुप्त:
        ParallelLoopBodyWrapperContext(अचर ParallelLoopBodyWrapperContext&); // disabled
        ParallelLoopBodyWrapperContext& चालक=(अचर ParallelLoopBodyWrapperContext&); // disabled
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
        शून्य चालक()(अचर cv::Range& sr) अचर CV_OVERRIDE
        {
#ifdef OPENCV_TRACE
            // TODO CV_TRACE_NS::details::setCurrentRegion(rootRegion);
            यदि (ctx.trace_जरा_Region && ctx.trace_जरा_Context)
                CV_TRACE_NS::details::parallel_For_Set_जरा_Region(*ctx.trace_जरा_Region, *ctx.trace_जरा_Context);
            CV__TRACE_OPENCV_FUNCTION_NAME("parallel_for_body");
            यदि (ctx.trace_जरा_Region)
                CV_TRACE_NS::details::parallelForAttachNestedRegion(*ctx.trace_जरा_Region);
#endif

#ifdef ENABLE_INSTRUMENTATION
            {
                cv::instr::InstrTLSStruct *pInstrTLS = &cv::instr::getInstrumentTLSStruct();
                pInstrTLS->p_Current_गाँठो = ctx.p_Thread_जरा; // Initialize TLS node for thread
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
            CV_TRACE_ARG_मान(range_start, "range.start", (int64)r.start);
            CV_TRACE_ARG_मान(range_end, "range.end", (int64)r.end);
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
            गृहाण (अचर cv::Exception& e)
            {
                ctx.recordException(e.what());
            }
            गृहाण (अचर std::exception& e)
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
        cv::Range stripeRange() अचर { फर्काऊ cv::Range(0, ctx.nstripes); }

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

        शून्य चालक ()(अचर tbb::blocked_range<पूर्णाङ्क>& range) अचर
        {
            एतत्->ParallelLoopBodyWrapper::चालक()(cv::Range(range.begin(), range.end()));
        }

        शून्य चालक ()() अचर  // run parallel job
        {
            cv::Range range = एतत्->stripeRange();
            tbb::parallel_for(tbb::blocked_range<पूर्णाङ्क>(range.start, range.end), *एतत्);
        }
    };
#अथवायदि defined HAVE_HPX
    वर्ग ProxyLoopBody : सार्वजनिक ParallelLoopBodyWrapper
    {
    सार्वजनिक:
        ProxyLoopBody(ParallelLoopBodyWrapperContext& ctx_)
                : ParallelLoopBodyWrapper(ctx_)
        {}

        शून्य चालक ()() अचर  // run parallel job
        {
            cv::Range stripeRange = एतत्->stripeRange();
            hpx::parallel::for_loop(
                    hpx::parallel::execution::par,
                    stripeRange.start, stripeRange.end,
                    [&](अचर पूर्णाङ्क &i) { ;
                        एतत्->ParallelLoopBodyWrapper::चालक()(
                                cv::Range(i, i + 1));
                    });
        }
    };
#अथवायदि defined HAVE_OPENMP
    प्रकारनिर्देश ParallelLoopBodyWrapper ProxyLoopBody;
#अथवायदि defined HAVE_GCD
    प्रकारनिर्देश ParallelLoopBodyWrapper ProxyLoopBody;
    स्थिर शून्य block_function(शून्य* context, माप_प्रकार index)
    {
        ProxyLoopBody* ptr_body = static_cast<ProxyLoopBody*>(context);
        (*ptr_body)(cv::Range((पूर्णाङ्क)index, (पूर्णाङ्क)index + 1));
    }
#अथवायदि defined WINRT || defined HAVE_CONCURRENCY
    वर्ग ProxyLoopBody : सार्वजनिक ParallelLoopBodyWrapper
    {
    सार्वजनिक:
        ProxyLoopBody(ParallelLoopBodyWrapperContext& ctx)
        : ParallelLoopBodyWrapper(ctx)
        {}

        शून्य चालक ()(पूर्णाङ्क i) अचर
        {
            एतत्->ParallelLoopBodyWrapper::चालक()(cv::Range(i, i + 1));
        }
    };

    // PPL's default auto_partitioner silently skips whole chunks of the range on
    // ARM64 (unfixed MSVC bug, developercommunity.visualstudio.com/t/1027444).
    ढाँचा <typename Body>
    स्थिर अन्तःस्थ शून्य pplParallelFor(पूर्णाङ्क first, पूर्णाङ्क last, अचर Body& body)
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
#अथवायदि defined HAVE_HPX
// nothing for HPX
#अथवायदि defined HAVE_OPENMP
स्थिर अन्तःस्थ पूर्णाङ्क _initMaxThreads()
{
    पूर्णाङ्क maxThreads = omp_get_max_threads();
    फर्काऊ maxThreads;
}
स्थिर पूर्णाङ्क अङ्क_Threads_Max = _initMaxThreads();
#अथवायदि defined HAVE_GCD
// nothing for GCD
#अथवायदि defined WINRT
// nothing for WINRT
#अथवायदि defined HAVE_CONCURRENCY

वर्ग SchedPtr
{
    Concurrency::Scheduler* sched_;
सार्वजनिक:
    Concurrency::Scheduler* चालक->() { फर्काऊ sched_; }
    चालक Concurrency::Scheduler*() { फर्काऊ sched_; }

    शून्य चालक=(Concurrency::Scheduler* sched)
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

स्थिर शून्य parallel_for_impl(अचर cv::Range& range, अचर cv::ParallelLoopBody& body, द्विगुण nstripes); // forward declaration

शून्य parallel_for_(अचर cv::Range& range, अचर cv::ParallelLoopBody& body, द्विगुण nstripes)
{
#ifdef OPENCV_TRACE
    CV__TRACE_OPENCV_FUNCTION_NAME_("parallel_for", 0);
    CV_TRACE_ARG_मान(range_start, "range.start", (int64)range.start);
    CV_TRACE_ARG_मान(range_end, "range.end", (int64)range.end);
    CV_TRACE_ARG_मान(nstripes, "nstripes", (int64)nstripes);
#endif

    CV_INSTRUMENT_REGION_MT_FORK();
    यदि (range.empty())
        फर्काऊ;

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
    अचर cv::ParallelLoopBody& body = *(अचर cv::ParallelLoopBody*)data;
    body(Range(start, end));
}

स्थिर शून्य parallel_for_impl(अचर cv::Range& range, अचर cv::ParallelLoopBody& body, द्विगुण nstripes)
{
    using नामाकाश cv::parallel;
    यदि ((अङ्क_Threads < 0 || अङ्क_Threads > 1) && range.end - range.start > 1)
    {
        ParallelLoopBodyWrapperContext ctx(body, range, nstripes);
        ProxyLoopBody pbody(ctx);
        cv::Range stripeRange = pbody.stripeRange();
        यदि( stripeRange.end - stripeRange.start == 1 )
        {
            body(range);
            फर्काऊ;
        }

        std::shared_ptr<ParallelForAPI>& api = getCurrentParallelForAPI();
        यदि (api)
        {
            CV_CheckEQ(stripeRange.start, 0, "");
            api->parallel_for(stripeRange.end, parallel_for_cb, (शून्य*)&pbody);
            ctx.finalize();  // propagate exceptions if exists
            फर्काऊ;
        }

#ifdef CV_PARALLEL_FRAMEWORK
#यदि defined HAVE_TBB

#यदि TBB_INTERFACE_VERSION >= 8000
        tbbArena.execute(pbody);
#अन्यथा
        pbody();
#endif

#अथवायदि defined HAVE_HPX
        pbody();

#अथवायदि defined HAVE_OPENMP

        #pragma omp parallel लागि schedule(dynamic) अङ्क_threads(अङ्क_Threads > 0 ? अङ्क_Threads : अङ्क_Threads_Max)
        लागि (पूर्णाङ्क i = stripeRange.start; i < stripeRange.end; ++i)
            pbody(Range(i, i + 1));

#अथवायदि defined HAVE_GCD

        dispatch_queue_t concurrent_queue = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0);
        dispatch_apply_f(stripeRange.end - stripeRange.start, concurrent_queue, &pbody, block_function);

#अथवायदि defined WINRT

        pplParallelFor(stripeRange.start, stripeRange.end, pbody);

#अथवायदि defined HAVE_CONCURRENCY

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

#अथवायदि defined HAVE_PTHREADS_PF

        parallel_for_pthreads(pbody.stripeRange(), pbody, pbody.stripeRange().size());

#अन्यथा

#error You have hacked and compiling with unsupported parallel framework

#endif

        ctx.finalize();  // propagate exceptions if exists
        फर्काऊ;
#endif // CV_PARALLEL_FRAMEWORK
    }

    body(range);
}


पूर्णाङ्क get_अङ्क_Threads(शून्य)
{
    std::shared_ptr<ParallelForAPI>& api = getCurrentParallelForAPI();
    यदि (api)
    {
        फर्काऊ api->get_अङ्क_Threads();
    }

    यदि (अङ्क_Threads == 0)
        फर्काऊ 1;

#यदि defined HAVE_TBB

#यदि TBB_INTERFACE_VERSION >= 9100
    फर्काऊ tbbArena.max_concurrency();
#अथवायदि TBB_INTERFACE_VERSION >= 8000
    फर्काऊ अङ्क_Threads > 0
        ? अङ्क_Threads
        : tbb::task_scheduler_init::default_अङ्क_threads();
#अन्यथा
    फर्काऊ tbbScheduler.is_active()
           ? अङ्क_Threads
           : tbb::task_scheduler_init::default_अङ्क_threads();
#endif

#अथवायदि defined HAVE_HPX
    फर्काऊ अङ्क_Threads;

#अथवायदि defined HAVE_OPENMP

    फर्काऊ अङ्क_Threads > 0
           ? अङ्क_Threads
           : अङ्क_Threads_Max;


#अथवायदि defined HAVE_GCD

    फर्काऊ cv::get_सङ्ख्या_Of_CPUs(); // the GCD thread pool limit

#अथवायदि defined WINRT

    फर्काऊ 0;

#अथवायदि defined HAVE_CONCURRENCY

    फर्काऊ (pplScheduler == 0)
        ? Concurrency::CurrentScheduler::Get()->Get_सङ्ख्या_Of_Virtual_Processors()
        : (1 + pplScheduler->Get_सङ्ख्या_Of_Virtual_Processors());

#अथवायदि defined HAVE_PTHREADS_PF

        फर्काऊ parallel_pthreads_get_threads_अङ्क();

#अन्यथा

    फर्काऊ 1;

#endif
}

अचिन्हित default_सङ्ख्या_Of_Threads()
{
#ifdef __ANDROID__
    // many modern phones/tables have 4-core CPUs. Let's use no more
    // than 2 threads by default not to overheat the devices
    अचर अचिन्हित पूर्णाङ्क default_सङ्ख्या_of_threads = 2;
#अन्यथा
    अचर अचिन्हित पूर्णाङ्क default_सङ्ख्या_of_threads = (अचिन्हित पूर्णाङ्क)std::max(1, cv::get_सङ्ख्या_Of_CPUs());
#endif

    अचिन्हित result = default_सङ्ख्या_of_threads;

    स्थिर पूर्णाङ्क config_अङ्क_threads = (पूर्णाङ्क)utils::getConfigurationParameterSizeT("OPENCV_FOR_THREADS_NUM", 0);

    यदि (config_अङ्क_threads)
    {
        result = (अचिन्हित)std::max(1, config_अङ्क_threads);
        //do we need upper limit of threads number?
    }
    फर्काऊ result;
}

शून्य set_अङ्क_Threads( पूर्णाङ्क threads_ )
{
    CV_UNUSED(threads_);

    पूर्णाङ्क threads = (threads_ < 0) ? default_सङ्ख्या_Of_Threads() : (अचिन्हित)threads_;
    अङ्क_Threads = threads;

    std::shared_ptr<ParallelForAPI>& api = getCurrentParallelForAPI();
    यदि (api)
    {
        api->set_अङ्क_Threads(अङ्क_Threads);
    }

#ifdef HAVE_TBB

#यदि TBB_INTERFACE_VERSION >= 8000
    यदि(tbbArena.is_active()) tbbArena.terminate();
    यदि(threads > 0) tbbArena.initialize(threads);
#अन्यथा
    यदि(tbbScheduler.is_active()) tbbScheduler.terminate();
    यदि(threads > 0) tbbScheduler.initialize(threads);
#endif

#अथवायदि defined HAVE_HPX
    फर्काऊ; // nothing needed as numThreads is used

#अथवायदि defined HAVE_OPENMP

    फर्काऊ; // nothing needed as num_threads clause is used in #pragma omp parallel for

#अथवायदि defined HAVE_GCD

    // unsupported
    // there is only private dispatch_queue_set_width() and only for desktop

#अथवायदि defined WINRT

    फर्काऊ;

#अथवायदि defined HAVE_CONCURRENCY

    यदि (threads <= 0)
    {
        pplScheduler = 0;
    }
    अन्यथा यदि (threads == 1)
    {
        // Concurrency always uses >=2 threads, so we just disable it if 1 thread is requested
        अङ्क_Threads = 0;
    }
    अन्यथा यदि (pplScheduler == 0 || 1 + pplScheduler->Get_सङ्ख्या_Of_Virtual_Processors() != (अचिन्हित पूर्णाङ्क)threads)
    {
        pplScheduler = Concurrency::Scheduler::Create(Concurrency::SchedulerPolicy(2,
                       Concurrency::MinConcurrency, threads-1,
                       Concurrency::MaxConcurrency, threads-1));
    }

#अथवायदि defined HAVE_PTHREADS_PF

    parallel_pthreads_set_threads_अङ्क(threads);

#endif
}


पूर्णाङ्क get_Thread_अङ्क()
{
    std::shared_ptr<ParallelForAPI>& api = getCurrentParallelForAPI();
    यदि (api)
    {
        फर्काऊ api->get_Thread_अङ्क();
    }

#यदि defined HAVE_TBB
    #यदि TBB_INTERFACE_VERSION >= 9100
        फर्काऊ tbb::this_task_arena::current_thread_index();
    #अथवायदि TBB_INTERFACE_VERSION >= 8000
        फर्काऊ tbb::task_arena::current_thread_index();
    #अन्यथा
        फर्काऊ 0;
    #endif
#अथवायदि defined HAVE_HPX
    फर्काऊ (पूर्णाङ्क)(hpx::get_अङ्क_worker_threads());
#अथवायदि defined HAVE_OPENMP
    फर्काऊ omp_get_thread_अङ्क();
#अथवायदि defined HAVE_GCD
    फर्काऊ (पूर्णाङ्क)(माप_प्रकार)(शून्य*)pthread_self(); // no zero-based indexing
#अथवायदि defined WINRT
    फर्काऊ 0;
#अथवायदि defined HAVE_CONCURRENCY
    फर्काऊ std::max(0, (पूर्णाङ्क)Concurrency::Context::VirtualProcessorId()); // zero for master thread, unique number for others but not necessary 1,2,3,...
#अथवायदि defined HAVE_PTHREADS_PF
    फर्काऊ (पूर्णाङ्क)(माप_प्रकार)(शून्य*)pthread_self(); // no zero-based indexing
#अन्यथा
    फर्काऊ 0;
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
std::string getFileContents(अचर वर्ण *filename)
{
    std::ifstream ifs(filename);
    यदि (!ifs.is_open())
        फर्काऊ std::string();

    std::string content( (std::istreambuf_iterator<वर्ण>(ifs) ),
                         (std::istreambuf_iterator<वर्ण>()    ) );

    यदि (ifs.fail())
        फर्काऊ std::string();

    फर्काऊ content;
}

स्थिर अन्तःस्थ
पूर्णाङ्क get_सङ्ख्या_Of_CPUs_Impl(अचर वर्ण *filename)
{
   std::string file_contents = getFileContents(filename);
   यदि(file_contents.empty())
       फर्काऊ 0;

   वर्ण *pbuf = const_cast<वर्ण*>(file_contents.c_str());
   //parse string of form "0-1,3,5-7,10,13-15"
   पूर्णाङ्क cpusAvailable = 0;

   जबसम्म(*pbuf)
   {
      अचर वर्ण* pos = pbuf;
      तार्किक range = असत्य;
      जबसम्म(*pbuf && *pbuf != ',')
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
          माला_पूछो(pos, "%d-%d", &rstart, &rend);
          cpusAvailable += rend - rstart + 1;
      }

   }
   फर्काऊ cpusAvailable;
}
#endif

#यदि defined CV_HAVE_CGROUPS
स्थिर अन्तःस्थ
अचिन्हित get_सङ्ख्या_Of_CPUs_CFSv2()
{
    पूर्णाङ्क cfs_quota = 0;
    पूर्णाङ्क cfs_period = 0;

    std::ifstream ss_cpu_max("/sys/fs/cgroup/cpu.max", std::ios::in | std::ios::binary);
    ss_cpu_max >> cfs_quota >> cfs_period;

    यदि (ss_cpu_max.fail() || cfs_quota < 1 || cfs_period < 1) /* values must not be 0 or negative */
        फर्काऊ 0;

    फर्काऊ (अचिन्हित)max(1, cfs_quota/cfs_period);
}

स्थिर अन्तःस्थ
अचिन्हित get_सङ्ख्या_Of_CPUs_CFSv1()
{
    पूर्णाङ्क cfs_quota = 0;
    {
        std::ifstream ss_period("/sys/fs/cgroup/cpu/cpu.cfs_quota_us", std::ios::in | std::ios::binary);
        ss_period >> cfs_quota;

        यदि (ss_period.fail() || cfs_quota < 1) /* cfs_quota must not be 0 or negative */
            फर्काऊ 0;
    }

    पूर्णाङ्क cfs_period = 0;
    {
        std::ifstream ss_quota("/sys/fs/cgroup/cpu/cpu.cfs_period_us", std::ios::in | std::ios::binary);
        ss_quota >> cfs_period;

        यदि (ss_quota.fail() || cfs_period < 1)
            फर्काऊ 0;
    }

    फर्काऊ (अचिन्हित)max(1, cfs_quota/cfs_period);
}
#endif

ढाँचा <typename T> स्थिर अन्तःस्थ
T minNonZero(अचर T& val_1, अचर T& val_2)
{
    यदि ((val_1 != 0) && (val_2 != 0))
        फर्काऊ std::min(val_1, val_2);
    फर्काऊ (val_1 != 0) ? val_1 : val_2;
}

#ifndef OPENCV_DISABLE_THREAD_SUPPORT
स्थिर
पूर्णाङ्क get_सङ्ख्या_Of_CPUs_2()
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
    अचिन्हित ncpus = std::thread::hardware_concurrency(); /* If the value is not well defined or not computable, returns 0 */
#अन्यथा
    अचिन्हित ncpus = 0; /* 0 means we have to find out some other way */
#endif

#यदि defined _WIN32

    SYSTEM_INFO sysinfo = {};
#यदि (defined(_M_ARM) || defined(_M_ARM64) || defined(_M_ARM64EC) || defined(_M_X64) || defined(WINRT)) && _WIN32_WINNT >= 0x501
    GetNativeSystemInfo( &sysinfo );
#अन्यथा
    GetSystemInfo( &sysinfo );
#endif
    अचिन्हित ncpus_sysinfo = sysinfo.dw_सङ्ख्या_Of_Processors;
    ncpus = minNonZero(ncpus, ncpus_sysinfo);

#अथवायदि defined __APPLE__

    पूर्णाङ्क अङ्क_CPU=0;
    पूर्णाङ्क mib[4];
    माप_प्रकार len = परिमाण(अङ्क_CPU);

    /* set the mib for hw.ncpu */
    mib[0] = CTL_HW;
    mib[1] = HW_AVAILCPU;  // alternatively, try HW_NCPU;

    /* get the number of CPUs from the system */
    sysctl(mib, 2, &अङ्क_CPU, &len, रिक्त, 0);

    यदि( अङ्क_CPU < 1 )
    {
        mib[1] = HW_NCPU;
        sysctl( mib, 2, &अङ्क_CPU, &len, रिक्त, 0 );

        यदि( अङ्क_CPU < 1 )
            अङ्क_CPU = 1;
    }

    ncpus = minNonZero(ncpus, (अचिन्हित)अङ्क_CPU);

#अथवायदि defined CV_CPU_GROUPS_1

#यदि defined CV_HAVE_CGROUPS
    स्थिर अचिन्हित ncpus_impl_cpuset = (अचिन्हित)get_सङ्ख्या_Of_CPUs_Impl("/sys/fs/cgroup/cpuset/cpuset.cpus");
    ncpus = minNonZero(ncpus, ncpus_impl_cpuset);

    स्थिर अचिन्हित ncpus_impl_cfs_v1 = get_सङ्ख्या_Of_CPUs_CFSv1();
    ncpus = minNonZero(ncpus, ncpus_impl_cfs_v1);

    स्थिर अचिन्हित ncpus_impl_cfs_v2 = get_सङ्ख्या_Of_CPUs_CFSv2();
    ncpus = minNonZero(ncpus, ncpus_impl_cfs_v2);
#endif

    स्थिर अचिन्हित ncpus_impl_devices = (अचिन्हित)get_सङ्ख्या_Of_CPUs_Impl("/sys/devices/system/cpu/online");
    ncpus = minNonZero(ncpus, ncpus_impl_devices);

#endif

#यदि defined _GNU_SOURCE \
    && !defined(__MINGW32__) /* not implemented (2020-03) */ \
    && !defined(__EMSCRIPTEN__) \
    && !defined(__ANDROID__)  // TODO: add check for modern Android NDK

    cpu_set_t cpu_set;
    यदि (0 == sched_getaffinity(0, परिमाण(cpu_set), &cpu_set))
    {
        अचिन्हित cpu_गन्ती_cpu_set = CPU_COUNT(&cpu_set);
        ncpus = minNonZero(ncpus, cpu_गन्ती_cpu_set);
    }

#endif

#यदि !defined(_WIN32) && !defined(__APPLE__) && defined(_SC_NPROCESSORS_ONLN)

    स्थिर अचिन्हित cpu_गन्ती_sysconf = (अचिन्हित)sysconf( _SC_NPROCESSORS_ONLN );
    ncpus = minNonZero(ncpus, cpu_गन्ती_sysconf);
#अथवायदि defined (__QNX__)
    स्थिर अचिन्हित cpu_गन्ती_sysconf = _syspage_ptr->अङ्क_cpu;
    ncpus = minNonZero(ncpus, cpu_गन्ती_sysconf);
#endif

    फर्काऊ ncpus != 0 ? ncpus : 1;
#अन्यथा //  OPENCV_SEMIHOSTING
    फर्काऊ 1;
#endif //OPENCV_SEMIHOSTING
}

पूर्णाङ्क get_सङ्ख्या_Of_CPUs()
{
    स्थिर पूर्णाङ्क nCPUs = get_सङ्ख्या_Of_CPUs_2();
    फर्काऊ nCPUs;  // cached value
}

#अन्यथा  // OPENCV_DISABLE_THREAD_SUPPORT
पूर्णाङ्क get_सङ्ख्या_Of_CPUs()
{
    फर्काऊ 1;
}
#endif  // OPENCV_DISABLE_THREAD_SUPPORT

अचर वर्ण* currentParallelFramework()
{
    std::shared_ptr<ParallelForAPI>& api = getCurrentParallelForAPI();
    यदि (api)
    {
        फर्काऊ api->getName();
    }
#ifdef CV_PARALLEL_FRAMEWORK
    फर्काऊ CV_PARALLEL_FRAMEWORK;
#अन्यथा
    फर्काऊ रिक्त;
#endif
}

}  // namespace cv::
