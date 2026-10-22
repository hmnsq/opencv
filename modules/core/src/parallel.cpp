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

#समावेश "precomp.hpp"

#समावेश <opencv2/core/utils/configuration.private.hpp>
#समावेश <opencv2/core/utils/trace.private.hpp>

#समावेश "opencv2/core/parallel/parallel_backend.hpp"
#समावेश "parallel/parallel.hpp"

#अगर defined _WIN32 || defined WINCE
    #समावेश <windows.h>
    #अघोषित small
    #अघोषित min
    #अघोषित max
    #अघोषित असल
#पूर्ण_अगर

#अगर defined __unix__ || defined __APPLE__ || defined __GLIBC__ \
    || defined __HAIKU__ || defined __EMSCRIPTEN__ \
    || defined __FreeBSD__ || defined __NetBSD__ || defined __OpenBSD__
    #समावेश <unistd.h>
    #समावेश <stdio.h>
    #समावेश <sys/types.h>
    #समावेश <fstream>
    #अगर defined __ANDROID__
        #समावेश <sys/sysconf.h>
        #समावेश <sys/syscall.h>
        #समावेश <sched.h>
    #या_अगर defined __APPLE__
        #समावेश <sys/sysctl.h>
    #पूर्ण_अगर
#पूर्ण_अगर

#अगर defined (__QNX__)
    #समावेश <sys/syspage.h>
#पूर्ण_अगर

#अगर_अघोषित OPENCV_DISABLE_THREAD_SUPPORT
    #समावेश <thread>
#पूर्ण_अगर

#अगर_घोषित _OPENMP
    #घोषणा HAVE_OPENMP
#पूर्ण_अगर

#अगर_घोषित __APPLE__
    #घोषणा HAVE_GCD
#पूर्ण_अगर

#अगर defined _MSC_VER
    #घोषणा HAVE_CONCURRENCY
#पूर्ण_अगर

/* IMPORTANT: always use the same order of defines
   - HAVE_TBB         - 3rdparty library, should be explicitly enabled
   - HAVE_HPX         - 3rdparty library, should be explicitly enabled
   - HAVE_OPENMP      - integrated to compiler, should be explicitly enabled
   - HAVE_GCD         - system wide, used automatically        (APPLE only)
   - WINRT            - system wide, used automatically        (Windows RT only)
   - HAVE_CONCURRENCY - part of runtime, used automatically    (Windows only - MSVS 10, MSVS 11)
   - HAVE_PTHREADS_PF - pthreads if available
*/

#अगर defined HAVE_TBB
    #अगर_अघोषित TBB_SUPPRESS_DEPRECATED_MESSAGES  // supress warning
    #घोषणा TBB_SUPPRESS_DEPRECATED_MESSAGES 1
    #पूर्ण_अगर
    #समावेश "tbb/tbb.h"
    #समावेश "tbb/task.h"
    #अगर TBB_INTERFACE_VERSION >= 8000
        #समावेश "tbb/task_arena.h"
    #पूर्ण_अगर
    #अघोषित min
    #अघोषित max
#या_अगर defined HAVE_HPX
    #समावेश <hpx/parallel/algorithms/for_loop.hpp>
    #समावेश <hpx/parallel/execution.hpp>
    //
    #समावेश <hpx/hpx_start.hpp>
    #समावेश <hpx/hpx_suspend.hpp>
    #समावेश <hpx/include/apply.hpp>
    #समावेश <hpx/util/yield_while.hpp>
    #समावेश <hpx/include/threadmanager.hpp>

#या_अगर defined HAVE_OPENMP
    #समावेश <omp.h>
#या_अगर defined HAVE_GCD
    #समावेश <dispatch/dispatch.h>
    #समावेश <pthread.h>
#या_अगर defined HAVE_CONCURRENCY
    #समावेश <ppl.h>
#या_अगर defined HAVE_PTHREADS_PF
    #समावेश <pthread.h>
#पूर्ण_अगर


#अगर defined HAVE_TBB
#  define CV_PARALLEL_FRAMEWORK "tbb"
#या_अगर defined HAVE_HPX
#  define CV_PARALLEL_FRAMEWORK "hpx"
#या_अगर defined HAVE_OPENMP
#  define CV_PARALLEL_FRAMEWORK "openmp"
#या_अगर defined HAVE_GCD
#  define CV_PARALLEL_FRAMEWORK "gcd"
#या_अगर defined WINRT
#  define CV_PARALLEL_FRAMEWORK "winrt-concurrency"
#या_अगर defined HAVE_CONCURRENCY
#  define CV_PARALLEL_FRAMEWORK "ms-concurrency"
#या_अगर defined HAVE_PTHREADS_PF
#  define CV_PARALLEL_FRAMEWORK "pthreads"
#पूर्ण_अगर

#समावेश <atomic>

#समावेश "parallel_impl.hpp"

#समावेश "opencv2/core/detail/exception_ptr.hpp"  // CV__EXCEPTION_PTR = 1 if std::exception_ptr is available

#समावेश <opencv2/core/utils/fp_control_utils.hpp>
#समावेश <opencv2/core/utils/fp_control.private.hpp>

using namespace cv;

namespace cv {

ParallelLoopBody::~ParallelLoopBody() {}

using namespace cv::parallel;

namespace {

#अगर_घोषित ENABLE_INSTRUMENTATION
    अटल व्योम SyncNodes(cv::instr::Instr_गाँठ *p_गाँठ)
    {
        std::vector<cv::instr::गाँठ_तथ्य_Tls*> तथ्य;
        p_गाँठ->m_payload.m_tls.gather(तथ्य);

        uint64 ticksMax = 0;
        पूर्णांक    threads  = 0;
        क्रम(माप_प्रकार i = 0; i < तथ्य.size(); i++)
        {
            अगर(तथ्य[i] && तथ्य[i]->m_ticks_योग)
            {
                ticksMax = MAX(ticksMax, तथ्य[i]->m_ticks_योग);
                p_गाँठ->m_payload.m_ticks_योग -= तथ्य[i]->m_ticks_योग;
                तथ्य[i]->m_ticks_योग = 0;
                threads++;
            }
        }
        p_गाँठ->m_payload.m_ticks_योग += ticksMax;
        p_गाँठ->m_payload.m_threads = MAX(p_गाँठ->m_payload.m_threads, threads);

        क्रम(माप_प्रकार i = 0; i < p_गाँठ->m_childs.size(); i++)
            SyncNodes(p_गाँठ->m_childs[i]);
    }
#पूर्ण_अगर

    श्रेणी ParallelLoopBodyWrapperContext
    {
    खुला:
        ParallelLoopBodyWrapperContext(स्थिर cv::ParallelLoopBody& _body, स्थिर cv::Range& _r, द्विगुन _nstripes) :
            is_rng_used(false), hasException(false)
        {

            body = &_body;
            wholeRange = _r;
            द्विगुन len = wholeRange.end - wholeRange.start;
            nstripes = cvRound(_nstripes <= 0 ? len : MIN(MAX(_nstripes, 1.), len));

            // propagate main thread state
            rng = cv::theRNG();
#अगर OPENCV_SUPPORTS_FP_DENORMALS_HINT && OPENCV_IMPL_FP_HINTS
            details::saveFPDenormalsState(fp_denormals_base_state);
#पूर्ण_अगर

#अगर_घोषित OPENCV_TRACE
            trace_मूल_Region = CV_TRACE_NS::details::getCurrentRegion();
            trace_मूल_Context = CV_TRACE_NS::details::getTraceManager().tls.लो();
#पूर्ण_अगर

#अगर_घोषित ENABLE_INSTRUMENTATION
            p_Thread_मूल = cv::instr::getInstrumentTLSStruct().p_Current_गाँठ;
#पूर्ण_अगर
        }
        व्योम finalize()
        {
#अगर_घोषित ENABLE_INSTRUMENTATION
            क्रम(माप_प्रकार i = 0; i < p_Thread_मूल->m_childs.size(); i++)
                SyncNodes(p_Thread_मूल->m_childs[i]);
#पूर्ण_अगर
            अगर (is_rng_used)
            {
                // Some parallel backends execute nested jobs in the main thread,
                // so we need to restore initial RNG state here.
                cv::theRNG() = rng;
                // We can't properly update RNG state based on RNG usage in worker threads,
                // so lets just change main thread RNG state to the next value.
                // Note: this behaviour is not equal to single-threaded mode.
                cv::theRNG().next();
            }
#अगर_घोषित OPENCV_TRACE
            अगर (trace_मूल_Region)
                CV_TRACE_NS::details::parallelForFinalize(*trace_मूल_Region);
#पूर्ण_अगर

            अगर (hasException)
            {
#अगर CV__EXCEPTION_PTR
                std::rethrow_exception(pException);
#अन्यथा
                CV_Error(Error::StsError, "Exception in parallel_for() body: " + exception_message);
#पूर्ण_अगर
            }
        }
        ~ParallelLoopBodyWrapperContext() {}

        स्थिर cv::ParallelLoopBody* body;
        cv::Range wholeRange;
        पूर्णांक nstripes;
        cv::RNG rng;
        mutable bool is_rng_used;
#अगर_घोषित OPENCV_TRACE
        CV_TRACE_NS::details::Region* trace_मूल_Region;
        CV_TRACE_NS::details::TraceManagerThreadLocal* trace_मूल_Context;
#पूर्ण_अगर
#अगर_घोषित ENABLE_INSTRUMENTATION
        cv::instr::Instr_गाँठ *p_Thread_मूल;
#पूर्ण_अगर
        bool hasException;
#अगर CV__EXCEPTION_PTR
        std::exception_ptr pException;
#अन्यथा
        cv::String exception_message;
#पूर्ण_अगर
#अगर CV__EXCEPTION_PTR
        व्योम recordException()
#अन्यथा
        व्योम recordException(स्थिर cv::String& संदेश)
#पूर्ण_अगर
        {
#अगर_अघोषित CV_THREAD_SANITIZER
            अगर (!hasException)
#पूर्ण_अगर
            {
                cv::AutoLock lock(cv::getInitializationMutex());
                अगर (!hasException)
                {
                    hasException = true;
#अगर CV__EXCEPTION_PTR
                    pException = std::current_exception();
#अन्यथा
                    exception_message = संदेश;
#पूर्ण_अगर
                }
            }
        }

#अगर OPENCV_SUPPORTS_FP_DENORMALS_HINT && OPENCV_IMPL_FP_HINTS
        details::FPDenormalsModeState fp_denormals_base_state;
#पूर्ण_अगर

    निजी:
        ParallelLoopBodyWrapperContext(स्थिर ParallelLoopBodyWrapperContext&); // disabled
        ParallelLoopBodyWrapperContext& चालक=(स्थिर ParallelLoopBodyWrapperContext&); // disabled
    };

    श्रेणी ParallelLoopBodyWrapper : खुला cv::ParallelLoopBody
    {
    खुला:
        ParallelLoopBodyWrapper(ParallelLoopBodyWrapperContext& ctx_) :
            ctx(ctx_)
        {
        }
        ~ParallelLoopBodyWrapper()
        {
        }
        व्योम चालक()(स्थिर cv::Range& sr) स्थिर CV_OVERRIDE
        {
#अगर_घोषित OPENCV_TRACE
            // TODO CV_TRACE_NS::details::setCurrentRegion(rootRegion);
            अगर (ctx.trace_मूल_Region && ctx.trace_मूल_Context)
                CV_TRACE_NS::details::parallel_For_Set_मूल_Region(*ctx.trace_मूल_Region, *ctx.trace_मूल_Context);
            CV__TRACE_OPENCV_FUNCTION_NAME("parallel_for_body");
            अगर (ctx.trace_मूल_Region)
                CV_TRACE_NS::details::parallelForAttachNestedRegion(*ctx.trace_मूल_Region);
#पूर्ण_अगर

#अगर_घोषित ENABLE_INSTRUMENTATION
            {
                cv::instr::InstrTLSStruct *pInstrTLS = &cv::instr::getInstrumentTLSStruct();
                pInstrTLS->p_Current_गाँठ = ctx.p_Thread_मूल; // Initialize TLS node for thread
            }
            CV_INSTRUMENT_REGION();
#पूर्ण_अगर

            // propagate main thread state
            cv::theRNG() = ctx.rng;
#अगर OPENCV_SUPPORTS_FP_DENORMALS_HINT && OPENCV_IMPL_FP_HINTS
            FPDenormalsIgnoreHintScope fp_denormals_scope(ctx.fp_denormals_base_state);
#पूर्ण_अगर

            cv::Range r;
            cv::Range wholeRange = ctx.wholeRange;
            पूर्णांक nstripes = ctx.nstripes;
            r.start = (पूर्णांक)(wholeRange.start +
                            ((uint64)sr.start*(wholeRange.end - wholeRange.start) + nstripes/2)/nstripes);
            r.end = sr.end >= nstripes ? wholeRange.end : (पूर्णांक)(wholeRange.start +
                            ((uint64)sr.end*(wholeRange.end - wholeRange.start) + nstripes/2)/nstripes);

#अगर_घोषित OPENCV_TRACE
            CV_TRACE_ARG_मूल्य(range_start, "range.start", (int64)r.start);
            CV_TRACE_ARG_मूल्य(range_end, "range.end", (int64)r.end);
#पूर्ण_अगर

            प्रयास
            {
                (*ctx.body)(r);
            }
#अगर CV__EXCEPTION_PTR
            पकड़ो (...)
            {
                ctx.recordException();
            }
#अन्यथा
            पकड़ो (स्थिर cv::Exception& e)
            {
                ctx.recordException(e.what());
            }
            पकड़ो (स्थिर std::exception& e)
            {
                ctx.recordException(e.what());
            }
            पकड़ो (...)
            {
                ctx.recordException("Unknown exception");
            }
#पूर्ण_अगर

            अगर (!ctx.is_rng_used && !(cv::theRNG() == ctx.rng))
                ctx.is_rng_used = true;
        }
        cv::Range stripeRange() स्थिर { वापस cv::Range(0, ctx.nstripes); }

    रक्षित:
        ParallelLoopBodyWrapperContext& ctx;
    };

#अगर defined HAVE_TBB
    श्रेणी ProxyLoopBody : खुला ParallelLoopBodyWrapper
    {
    खुला:
        ProxyLoopBody(ParallelLoopBodyWrapperContext& ctx_)
        : ParallelLoopBodyWrapper(ctx_)
        {}

        व्योम चालक ()(स्थिर tbb::blocked_range<पूर्णांक>& range) स्थिर
        {
            यह->ParallelLoopBodyWrapper::चालक()(cv::Range(range.begin(), range.end()));
        }

        व्योम चालक ()() स्थिर  // run parallel job
        {
            cv::Range range = यह->stripeRange();
            tbb::parallel_for(tbb::blocked_range<पूर्णांक>(range.start, range.end), *यह);
        }
    };
#या_अगर defined HAVE_HPX
    श्रेणी ProxyLoopBody : खुला ParallelLoopBodyWrapper
    {
    खुला:
        ProxyLoopBody(ParallelLoopBodyWrapperContext& ctx_)
                : ParallelLoopBodyWrapper(ctx_)
        {}

        व्योम चालक ()() स्थिर  // run parallel job
        {
            cv::Range stripeRange = यह->stripeRange();
            hpx::parallel::for_loop(
                    hpx::parallel::execution::par,
                    stripeRange.start, stripeRange.end,
                    [&](स्थिर पूर्णांक &i) { ;
                        यह->ParallelLoopBodyWrapper::चालक()(
                                cv::Range(i, i + 1));
                    });
        }
    };
#या_अगर defined HAVE_OPENMP
    प्रकार ParallelLoopBodyWrapper ProxyLoopBody;
#या_अगर defined HAVE_GCD
    प्रकार ParallelLoopBodyWrapper ProxyLoopBody;
    अटल व्योम block_function(व्योम* context, माप_प्रकार सूचकांक)
    {
        ProxyLoopBody* ptr_body = static_cast<ProxyLoopBody*>(context);
        (*ptr_body)(cv::Range((पूर्णांक)सूचकांक, (पूर्णांक)सूचकांक + 1));
    }
#या_अगर defined WINRT || defined HAVE_CONCURRENCY
    श्रेणी ProxyLoopBody : खुला ParallelLoopBodyWrapper
    {
    खुला:
        ProxyLoopBody(ParallelLoopBodyWrapperContext& ctx)
        : ParallelLoopBodyWrapper(ctx)
        {}

        व्योम चालक ()(पूर्णांक i) स्थिर
        {
            यह->ParallelLoopBodyWrapper::चालक()(cv::Range(i, i + 1));
        }
    };

    // PPL's default auto_partitioner silently skips whole chunks of the range on
    // ARM64 (unfixed MSVC bug, developercommunity.visualstudio.com/t/1027444).
    ढाँचा <typename Body>
    अटल अंतरभूत व्योम pplParallelFor(पूर्णांक first, पूर्णांक last, स्थिर Body& body)
    {
#अगर defined(_M_ARM64) || defined(_M_ARM64EC)
        Concurrency::parallel_for(first, last, body, Concurrency::static_partitioner());
#अन्यथा
        Concurrency::parallel_for(first, last, body);
#पूर्ण_अगर
    }
#अन्यथा
    प्रकार ParallelLoopBodyWrapper ProxyLoopBody;
#पूर्ण_अगर

#अगर defined HAVE_TBB
    #अगर TBB_INTERFACE_VERSION >= 8000
        अटल tbb::task_arena tbbArena(tbb::task_arena::automatic);
    #अन्यथा
        अटल tbb::task_scheduler_init tbbScheduler(tbb::task_scheduler_init::deferred);
    #पूर्ण_अगर
#या_अगर defined HAVE_HPX
// nothing for HPX
#या_अगर defined HAVE_OPENMP
अटल अंतरभूत पूर्णांक _initMaxThreads()
{
    पूर्णांक maxThreads = omp_get_max_threads();
    वापस maxThreads;
}
अटल पूर्णांक अंक_Threads_Max = _initMaxThreads();
#या_अगर defined HAVE_GCD
// nothing for GCD
#या_अगर defined WINRT
// nothing for WINRT
#या_अगर defined HAVE_CONCURRENCY

श्रेणी SchedPtr
{
    Concurrency::Scheduler* sched_;
खुला:
    Concurrency::Scheduler* चालक->() { वापस sched_; }
    चालक Concurrency::Scheduler*() { वापस sched_; }

    व्योम चालक=(Concurrency::Scheduler* sched)
    {
        अगर (sched_) sched_->Release();
        sched_ = sched;
    }

    SchedPtr() : sched_(0) {}
    ~SchedPtr() {}
};
अटल SchedPtr pplScheduler;

#पूर्ण_अगर

} // namespace anon

/* ================================   parallel_for_  ================================ */

अटल व्योम parallel_for_impl(स्थिर cv::Range& range, स्थिर cv::ParallelLoopBody& body, द्विगुन nstripes); // forward declaration

व्योम parallel_for_(स्थिर cv::Range& range, स्थिर cv::ParallelLoopBody& body, द्विगुन nstripes)
{
#अगर_घोषित OPENCV_TRACE
    CV__TRACE_OPENCV_FUNCTION_NAME_("parallel_for", 0);
    CV_TRACE_ARG_मूल्य(range_start, "range.start", (int64)range.start);
    CV_TRACE_ARG_मूल्य(range_end, "range.end", (int64)range.end);
    CV_TRACE_ARG_मूल्य(nstripes, "nstripes", (int64)nstripes);
#पूर्ण_अगर

    CV_INSTRUMENT_REGION_MT_FORK();
    अगर (range.empty())
        वापस;

    अटल std::atomic<bool> flagNestedParallelFor(false);
    bool isNotNestedRegion = !flagNestedParallelFor.load();
    अगर (isNotNestedRegion)
      isNotNestedRegion = !flagNestedParallelFor.exchange(true);
    अगर (isNotNestedRegion)
    {
        प्रयास
        {
            parallel_for_impl(range, body, nstripes);
            flagNestedParallelFor = false;
        }
        पकड़ो (...)
        {
            flagNestedParallelFor = false;
            फेंको;
        }
    }
    अन्यथा // nested parallel_for_() calls are not parallelized
    {
        CV_UNUSED(nstripes);
        body(range);
    }
}

अटल
व्योम parallel_for_cb(पूर्णांक start, पूर्णांक end, व्योम* तथ्य)
{
    CV_DbgAssert(तथ्य);
    स्थिर cv::ParallelLoopBody& body = *(स्थिर cv::ParallelLoopBody*)तथ्य;
    body(Range(start, end));
}

अटल व्योम parallel_for_impl(स्थिर cv::Range& range, स्थिर cv::ParallelLoopBody& body, द्विगुन nstripes)
{
    using namespace cv::parallel;
    अगर ((अंक_Threads < 0 || अंक_Threads > 1) && range.end - range.start > 1)
    {
        ParallelLoopBodyWrapperContext ctx(body, range, nstripes);
        ProxyLoopBody pbody(ctx);
        cv::Range stripeRange = pbody.stripeRange();
        अगर( stripeRange.end - stripeRange.start == 1 )
        {
            body(range);
            वापस;
        }

        std::shared_ptr<ParallelForAPI>& api = getCurrentParallelForAPI();
        अगर (api)
        {
            CV_CheckEQ(stripeRange.start, 0, "");
            api->parallel_for(stripeRange.end, parallel_for_cb, (व्योम*)&pbody);
            ctx.finalize();  // propagate exceptions if exists
            वापस;
        }

#अगर_घोषित CV_PARALLEL_FRAMEWORK
#अगर defined HAVE_TBB

#अगर TBB_INTERFACE_VERSION >= 8000
        tbbArena.execute(pbody);
#अन्यथा
        pbody();
#पूर्ण_अगर

#या_अगर defined HAVE_HPX
        pbody();

#या_अगर defined HAVE_OPENMP

        #आशय omp parallel क्रम schedule(dynamic) अंक_threads(अंक_Threads > 0 ? अंक_Threads : अंक_Threads_Max)
        क्रम (पूर्णांक i = stripeRange.start; i < stripeRange.end; ++i)
            pbody(Range(i, i + 1));

#या_अगर defined HAVE_GCD

        dispatch_queue_t concurrent_queue = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0);
        dispatch_apply_f(stripeRange.end - stripeRange.start, concurrent_queue, &pbody, block_function);

#या_अगर defined WINRT

        pplParallelFor(stripeRange.start, stripeRange.end, pbody);

#या_अगर defined HAVE_CONCURRENCY

        अगर(!pplScheduler || pplScheduler->Id() == Concurrency::CurrentScheduler::Id())
        {
            pplParallelFor(stripeRange.start, stripeRange.end, pbody);
        }
        अन्यथा
        {
            pplScheduler->Attach();
            pplParallelFor(stripeRange.start, stripeRange.end, pbody);
            Concurrency::CurrentScheduler::Detach();
        }

#या_अगर defined HAVE_PTHREADS_PF

        parallel_for_pthreads(pbody.stripeRange(), pbody, pbody.stripeRange().size());

#अन्यथा

#त्रुटि You have hacked and compiling with unsupported parallel framework

#पूर्ण_अगर

        ctx.finalize();  // propagate exceptions if exists
        वापस;
#पूर्ण_अगर // CV_PARALLEL_FRAMEWORK
    }

    body(range);
}


पूर्णांक get_अंक_Threads(व्योम)
{
    std::shared_ptr<ParallelForAPI>& api = getCurrentParallelForAPI();
    अगर (api)
    {
        वापस api->get_अंक_Threads();
    }

    अगर (अंक_Threads == 0)
        वापस 1;

#अगर defined HAVE_TBB

#अगर TBB_INTERFACE_VERSION >= 9100
    वापस tbbArena.max_concurrency();
#या_अगर TBB_INTERFACE_VERSION >= 8000
    वापस अंक_Threads > 0
        ? अंक_Threads
        : tbb::task_scheduler_init::default_अंक_threads();
#अन्यथा
    वापस tbbScheduler.is_active()
           ? अंक_Threads
           : tbb::task_scheduler_init::default_अंक_threads();
#पूर्ण_अगर

#या_अगर defined HAVE_HPX
    वापस अंक_Threads;

#या_अगर defined HAVE_OPENMP

    वापस अंक_Threads > 0
           ? अंक_Threads
           : अंक_Threads_Max;


#या_अगर defined HAVE_GCD

    वापस cv::get_संख्या_Of_CPUs(); // the GCD thread pool limit

#या_अगर defined WINRT

    वापस 0;

#या_अगर defined HAVE_CONCURRENCY

    वापस (pplScheduler == 0)
        ? Concurrency::CurrentScheduler::Get()->Get_संख्या_Of_Virtual_Processors()
        : (1 + pplScheduler->Get_संख्या_Of_Virtual_Processors());

#या_अगर defined HAVE_PTHREADS_PF

        वापस parallel_pthreads_get_threads_अंक();

#अन्यथा

    वापस 1;

#पूर्ण_अगर
}

अचिन्हित default_संख्या_Of_Threads()
{
#अगर_घोषित __ANDROID__
    // many modern phones/tables have 4-core CPUs. Let's use no more
    // than 2 threads by default not to overheat the devices
    स्थिर अचिन्हित पूर्णांक default_संख्या_of_threads = 2;
#अन्यथा
    स्थिर अचिन्हित पूर्णांक default_संख्या_of_threads = (अचिन्हित पूर्णांक)std::max(1, cv::get_संख्या_Of_CPUs());
#पूर्ण_अगर

    अचिन्हित परिणाम = default_संख्या_of_threads;

    अटल पूर्णांक config_अंक_threads = (पूर्णांक)utils::get_Configuration_Parameter_आकार_T("OPENCV_FOR_THREADS_NUM", 0);

    अगर (config_अंक_threads)
    {
        परिणाम = (अचिन्हित)std::max(1, config_अंक_threads);
        //do we need upper limit of threads number?
    }
    वापस परिणाम;
}

व्योम set_अंक_Threads( पूर्णांक threads_ )
{
    CV_UNUSED(threads_);

    पूर्णांक threads = (threads_ < 0) ? default_संख्या_Of_Threads() : (अचिन्हित)threads_;
    अंक_Threads = threads;

    std::shared_ptr<ParallelForAPI>& api = getCurrentParallelForAPI();
    अगर (api)
    {
        api->set_अंक_Threads(अंक_Threads);
    }

#अगर_घोषित HAVE_TBB

#अगर TBB_INTERFACE_VERSION >= 8000
    अगर(tbbArena.is_active()) tbbArena.terminate();
    अगर(threads > 0) tbbArena.initialize(threads);
#अन्यथा
    अगर(tbbScheduler.is_active()) tbbScheduler.terminate();
    अगर(threads > 0) tbbScheduler.initialize(threads);
#पूर्ण_अगर

#या_अगर defined HAVE_HPX
    वापस; // nothing needed as numThreads is used

#या_अगर defined HAVE_OPENMP

    वापस; // nothing needed as num_threads clause is used in #pragma omp parallel for

#या_अगर defined HAVE_GCD

    // unsupported
    // there is only private dispatch_queue_set_width() and only for desktop

#या_अगर defined WINRT

    वापस;

#या_अगर defined HAVE_CONCURRENCY

    अगर (threads <= 0)
    {
        pplScheduler = 0;
    }
    अन्यथा अगर (threads == 1)
    {
        // Concurrency always uses >=2 threads, so we just disable it if 1 thread is requested
        अंक_Threads = 0;
    }
    अन्यथा अगर (pplScheduler == 0 || 1 + pplScheduler->Get_संख्या_Of_Virtual_Processors() != (अचिन्हित पूर्णांक)threads)
    {
        pplScheduler = Concurrency::Scheduler::Create(Concurrency::SchedulerPolicy(2,
                       Concurrency::MinConcurrency, threads-1,
                       Concurrency::MaxConcurrency, threads-1));
    }

#या_अगर defined HAVE_PTHREADS_PF

    parallel_pthreads_set_threads_अंक(threads);

#पूर्ण_अगर
}


पूर्णांक get_Thread_अंक()
{
    std::shared_ptr<ParallelForAPI>& api = getCurrentParallelForAPI();
    अगर (api)
    {
        वापस api->get_Thread_अंक();
    }

#अगर defined HAVE_TBB
    #अगर TBB_INTERFACE_VERSION >= 9100
        वापस tbb::this_task_arena::current_thread_सूचकांक();
    #या_अगर TBB_INTERFACE_VERSION >= 8000
        वापस tbb::task_arena::current_thread_सूचकांक();
    #अन्यथा
        वापस 0;
    #पूर्ण_अगर
#या_अगर defined HAVE_HPX
    वापस (पूर्णांक)(hpx::get_अंक_worker_threads());
#या_अगर defined HAVE_OPENMP
    वापस omp_get_thread_अंक();
#या_अगर defined HAVE_GCD
    वापस (पूर्णांक)(माप_प्रकार)(व्योम*)pthread_self(); // no zero-based indexing
#या_अगर defined WINRT
    वापस 0;
#या_अगर defined HAVE_CONCURRENCY
    वापस std::max(0, (पूर्णांक)Concurrency::Context::VirtualProcessorId()); // zero for master thread, unique number for others but not necessary 1,2,3,...
#या_अगर defined HAVE_PTHREADS_PF
    वापस (पूर्णांक)(माप_प्रकार)(व्योम*)pthread_self(); // no zero-based indexing
#अन्यथा
    वापस 0;
#पूर्ण_अगर
}


#अगर defined __linux__ || defined __GLIBC__ || defined __HAIKU__ || defined __ANDROID__
  #घोषणा CV_CPU_GROUPS_1
#पूर्ण_अगर

#अगर defined __linux__ || defined __ANDROID__
  #घोषणा CV_HAVE_CGROUPS 1
#पूर्ण_अगर

#अगर defined CV_CPU_GROUPS_1
अटल अंतरभूत
std::string getFileContents(स्थिर अक्षर *filename)
{
    std::अख_प्रवाह ifs(filename);
    अगर (!ifs.is_open())
        वापस std::string();

    std::string content( (std::istreambuf_iterator<अक्षर>(ifs) ),
                         (std::istreambuf_iterator<अक्षर>()    ) );

    अगर (ifs.हार())
        वापस std::string();

    वापस content;
}

अटल अंतरभूत
पूर्णांक get_संख्या_Of_CPUs_Impl(स्थिर अक्षर *filename)
{
   std::string file_contents = getFileContents(filename);
   अगर(file_contents.empty())
       वापस 0;

   अक्षर *pbuf = const_cast<अक्षर*>(file_contents.c_str());
   //parse string of form "0-1,3,5-7,10,13-15"
   पूर्णांक cpusAvailable = 0;

   जबतक(*pbuf)
   {
      स्थिर अक्षर* pos = pbuf;
      bool range = false;
      जबतक(*pbuf && *pbuf != ',')
      {
          अगर(*pbuf == '-') range = true;
          ++pbuf;
      }
      अगर(*pbuf) *pbuf++ = 0;
      अगर(!range)
        ++cpusAvailable;
      अन्यथा
      {
          पूर्णांक rstart = 0, rend = 0;
          माला_पूछो(pos, "%d-%d", &rstart, &rend);
          cpusAvailable += rend - rstart + 1;
      }

   }
   वापस cpusAvailable;
}
#पूर्ण_अगर

#अगर defined CV_HAVE_CGROUPS
अटल अंतरभूत
अचिन्हित get_संख्या_Of_CPUs_CFSv2()
{
    पूर्णांक cfs_quota = 0;
    पूर्णांक cfs_period = 0;

    std::अख_प्रवाह ss_cpu_max("/sys/fs/cgroup/cpu.max", std::पनप::in | std::पनप::द्विग);
    ss_cpu_max >> cfs_quota >> cfs_period;

    अगर (ss_cpu_max.हार() || cfs_quota < 1 || cfs_period < 1) /* values must not be 0 or negative */
        वापस 0;

    वापस (अचिन्हित)max(1, cfs_quota/cfs_period);
}

अटल अंतरभूत
अचिन्हित get_संख्या_Of_CPUs_CFSv1()
{
    पूर्णांक cfs_quota = 0;
    {
        std::अख_प्रवाह ss_period("/sys/fs/cgroup/cpu/cpu.cfs_quota_us", std::पनप::in | std::पनप::द्विग);
        ss_period >> cfs_quota;

        अगर (ss_period.हार() || cfs_quota < 1) /* cfs_quota must not be 0 or negative */
            वापस 0;
    }

    पूर्णांक cfs_period = 0;
    {
        std::अख_प्रवाह ss_quota("/sys/fs/cgroup/cpu/cpu.cfs_period_us", std::पनप::in | std::पनप::द्विग);
        ss_quota >> cfs_period;

        अगर (ss_quota.हार() || cfs_period < 1)
            वापस 0;
    }

    वापस (अचिन्हित)max(1, cfs_quota/cfs_period);
}
#पूर्ण_अगर

ढाँचा <typename T> अटल अंतरभूत
T minNonZero(स्थिर T& val_1, स्थिर T& val_2)
{
    अगर ((val_1 != 0) && (val_2 != 0))
        वापस std::min(val_1, val_2);
    वापस (val_1 != 0) ? val_1 : val_2;
}

#अगर_अघोषित OPENCV_DISABLE_THREAD_SUPPORT
अटल
पूर्णांक get_संख्या_Of_CPUs_2()
{
#अगर_अघोषित OPENCV_SEMIHOSTING
    /*
     * Logic here is to try different methods of getting CPU counts and return
     * the minimum most value as it has high probablity of being right and safe.
     * Return 1 if we get 0 or not found on all methods.
    */
#अगर !defined(__MINGW32__) /* not implemented (2020-03) */

    /*
     * Check for this standard C++11 way, we do not return directly because
     * running in a docker or K8s environment will mean this is the host
     * machines config not the containers or pods and as per docs this value
     * must be "considered only a hint".
    */
    अचिन्हित ncpus = std::thread::hardware_concurrency(); /* If the value is not well defined or not computable, returns 0 */
#अन्यथा
    अचिन्हित ncpus = 0; /* 0 means we have to find out some other way */
#पूर्ण_अगर

#अगर defined _WIN32

    SYSTEM_INFO sysinfo = {};
#अगर (defined(_M_ARM) || defined(_M_ARM64) || defined(_M_ARM64EC) || defined(_M_X64) || defined(WINRT)) && _WIN32_WINNT >= 0x501
    GetNativeSystemInfo( &sysinfo );
#अन्यथा
    GetSystemInfo( &sysinfo );
#पूर्ण_अगर
    अचिन्हित ncpus_sysinfo = sysinfo.dw_संख्या_Of_Processors;
    ncpus = minNonZero(ncpus, ncpus_sysinfo);

#या_अगर defined __APPLE__

    पूर्णांक अंक_CPU=0;
    पूर्णांक mib[4];
    माप_प्रकार len = माप(अंक_CPU);

    /* set the mib for hw.ncpu */
    mib[0] = CTL_HW;
    mib[1] = HW_AVAILCPU;  // alternatively, try HW_NCPU;

    /* get the number of CPUs from the system */
    sysctl(mib, 2, &अंक_CPU, &len, शून्य, 0);

    अगर( अंक_CPU < 1 )
    {
        mib[1] = HW_NCPU;
        sysctl( mib, 2, &अंक_CPU, &len, शून्य, 0 );

        अगर( अंक_CPU < 1 )
            अंक_CPU = 1;
    }

    ncpus = minNonZero(ncpus, (अचिन्हित)अंक_CPU);

#या_अगर defined CV_CPU_GROUPS_1

#अगर defined CV_HAVE_CGROUPS
    अटल अचिन्हित ncpus_impl_cpuset = (अचिन्हित)get_संख्या_Of_CPUs_Impl("/sys/fs/cgroup/cpuset/cpuset.cpus");
    ncpus = minNonZero(ncpus, ncpus_impl_cpuset);

    अटल अचिन्हित ncpus_impl_cfs_v1 = get_संख्या_Of_CPUs_CFSv1();
    ncpus = minNonZero(ncpus, ncpus_impl_cfs_v1);

    अटल अचिन्हित ncpus_impl_cfs_v2 = get_संख्या_Of_CPUs_CFSv2();
    ncpus = minNonZero(ncpus, ncpus_impl_cfs_v2);
#पूर्ण_अगर

    अटल अचिन्हित ncpus_impl_devices = (अचिन्हित)get_संख्या_Of_CPUs_Impl("/sys/devices/system/cpu/online");
    ncpus = minNonZero(ncpus, ncpus_impl_devices);

#पूर्ण_अगर

#अगर defined _GNU_SOURCE \
    && !defined(__MINGW32__) /* not implemented (2020-03) */ \
    && !defined(__EMSCRIPTEN__) \
    && !defined(__ANDROID__)  // TODO: add check for modern Android NDK

    cpu_set_t cpu_set;
    अगर (0 == sched_getaffinity(0, माप(cpu_set), &cpu_set))
    {
        अचिन्हित cpu_गिनती_cpu_set = CPU_COUNT(&cpu_set);
        ncpus = minNonZero(ncpus, cpu_गिनती_cpu_set);
    }

#पूर्ण_अगर

#अगर !defined(_WIN32) && !defined(__APPLE__) && defined(_SC_NPROCESSORS_ONLN)

    अटल अचिन्हित cpu_गिनती_sysconf = (अचिन्हित)sysconf( _SC_NPROCESSORS_ONLN );
    ncpus = minNonZero(ncpus, cpu_गिनती_sysconf);
#या_अगर defined (__QNX__)
    अटल अचिन्हित cpu_गिनती_sysconf = _syspage_ptr->अंक_cpu;
    ncpus = minNonZero(ncpus, cpu_गिनती_sysconf);
#पूर्ण_अगर

    वापस ncpus != 0 ? ncpus : 1;
#अन्यथा //  OPENCV_SEMIHOSTING
    वापस 1;
#पूर्ण_अगर //OPENCV_SEMIHOSTING
}

पूर्णांक get_संख्या_Of_CPUs()
{
    अटल पूर्णांक nCPUs = get_संख्या_Of_CPUs_2();
    वापस nCPUs;  // cached value
}

#अन्यथा  // OPENCV_DISABLE_THREAD_SUPPORT
पूर्णांक get_संख्या_Of_CPUs()
{
    वापस 1;
}
#पूर्ण_अगर  // OPENCV_DISABLE_THREAD_SUPPORT

स्थिर अक्षर* currentParallelFramework()
{
    std::shared_ptr<ParallelForAPI>& api = getCurrentParallelForAPI();
    अगर (api)
    {
        वापस api->getName();
    }
#अगर_घोषित CV_PARALLEL_FRAMEWORK
    वापस CV_PARALLEL_FRAMEWORK;
#अन्यथा
    वापस शून्य;
#पूर्ण_अगर
}

}  // namespace cv::
