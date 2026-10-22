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

#అన్తర్భావ "precomp.hpp"

#అన్తర్భావ <opencv2/core/utils/configuration.private.hpp>
#అన్తర్భావ <opencv2/core/utils/trace.private.hpp>

#అన్తర్భావ "opencv2/core/parallel/parallel_backend.hpp"
#అన్తర్భావ "parallel/parallel.hpp"

#ఒకవేళ defined _WIN32 || defined WINCE
    #అన్తర్భావ <windows.h>
    #నిర్వచనం_రద్దు small
    #నిర్వచనం_రద్దు min
    #నిర్వచనం_రద్దు max
    #నిర్వచనం_రద్దు నిరపేక్ష
#అయితే_ముగింపు

#ఒకవేళ defined __unix__ || defined __APPLE__ || defined __GLIBC__ \
    || defined __HAIKU__ || defined __EMSCRIPTEN__ \
    || defined __FreeBSD__ || defined __NetBSD__ || defined __OpenBSD__
    #అన్తర్భావ <unistd.h>
    #అన్తర్భావ <stdio.h>
    #అన్తర్భావ <sys/types.h>
    #అన్తర్భావ <fstream>
    #ఒకవేళ defined __ANDROID__
        #అన్తర్భావ <sys/sysconf.h>
        #అన్తర్భావ <sys/syscall.h>
        #అన్తర్భావ <sched.h>
    #లేదావేళ defined __APPLE__
        #అన్తర్భావ <sys/sysctl.h>
    #అయితే_ముగింపు
#అయితే_ముగింపు

#ఒకవేళ defined (__QNX__)
    #అన్తర్భావ <sys/syspage.h>
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే OPENCV_DISABLE_THREAD_SUPPORT
    #అన్తర్భావ <thread>
#అయితే_ముగింపు

#నిర్వచితమైతే _OPENMP
    #నిర్వచించు HAVE_OPENMP
#అయితే_ముగింపు

#నిర్వచితమైతే __APPLE__
    #నిర్వచించు HAVE_గరిష్ఠ_సామాన్య_భాజకం
#అయితే_ముగింపు

#ఒకవేళ defined _MSC_VER
    #నిర్వచించు HAVE_CONCURRENCY
#అయితే_ముగింపు

/* IMPORTANT: always use the same order of defines
   - HAVE_TBB         - 3rdparty library, should be explicitly enabled
   - HAVE_HPX         - 3rdparty library, should be explicitly enabled
   - HAVE_OPENMP      - integrated to compiler, should be explicitly enabled
   - HAVE_GCD         - system wide, used automatically        (APPLE only)
   - WINRT            - system wide, used automatically        (Windows RT only)
   - HAVE_CONCURRENCY - part of runtime, used automatically    (Windows only - MSVS 10, MSVS 11)
   - HAVE_PTHREADS_PF - pthreads if available
*/

#ఒకవేళ defined HAVE_TBB
    #నిర్వచితం_కాకపోతే TBB_SUPPRESS_DEPRECATED_MESSAGES  // supress warning
    #నిర్వచించు TBB_SUPPRESS_DEPRECATED_MESSAGES 1
    #అయితే_ముగింపు
    #అన్తర్భావ "tbb/tbb.h"
    #అన్తర్భావ "tbb/task.h"
    #ఒకవేళ TBB_INTERFACE_సంస్కరణ >= 8000
        #అన్తర్భావ "tbb/task_arena.h"
    #అయితే_ముగింపు
    #నిర్వచనం_రద్దు min
    #నిర్వచనం_రద్దు max
#లేదావేళ defined HAVE_HPX
    #అన్తర్భావ <hpx/parallel/algorithms/for_loop.hpp>
    #అన్తర్భావ <hpx/parallel/execution.hpp>
    //
    #అన్తర్భావ <hpx/hpx_start.hpp>
    #అన్తర్భావ <hpx/hpx_suspend.hpp>
    #అన్తర్భావ <hpx/include/apply.hpp>
    #అన్తర్భావ <hpx/util/yield_while.hpp>
    #అన్తర్భావ <hpx/include/threadmanager.hpp>

#లేదావేళ defined HAVE_OPENMP
    #అన్తర్భావ <omp.h>
#లేదావేళ defined HAVE_గరిష్ఠ_సామాన్య_భాజకం
    #అన్తర్భావ <dispatch/dispatch.h>
    #అన్తర్భావ <pthread.h>
#లేదావేళ defined HAVE_CONCURRENCY
    #అన్తర్భావ <ppl.h>
#లేదావేళ defined HAVE_PTHREADS_PF
    #అన్తర్భావ <pthread.h>
#అయితే_ముగింపు


#ఒకవేళ defined HAVE_TBB
#  define CV_PARALLEL_FRAMEWORK "tbb"
#లేదావేళ defined HAVE_HPX
#  define CV_PARALLEL_FRAMEWORK "hpx"
#లేదావేళ defined HAVE_OPENMP
#  define CV_PARALLEL_FRAMEWORK "openmp"
#లేదావేళ defined HAVE_గరిష్ఠ_సామాన్య_భాజకం
#  define CV_PARALLEL_FRAMEWORK "gcd"
#లేదావేళ defined WINRT
#  define CV_PARALLEL_FRAMEWORK "winrt-concurrency"
#లేదావేళ defined HAVE_CONCURRENCY
#  define CV_PARALLEL_FRAMEWORK "ms-concurrency"
#లేదావేళ defined HAVE_PTHREADS_PF
#  define CV_PARALLEL_FRAMEWORK "pthreads"
#అయితే_ముగింపు

#అన్తర్భావ <atomic>

#అన్తర్భావ "parallel_impl.hpp"

#అన్తర్భావ "opencv2/core/detail/exception_ptr.hpp"  // CV__EXCEPTION_PTR = 1 if std::exception_ptr is available

#అన్తర్భావ <opencv2/core/utils/fp_control_utils.hpp>
#అన్తర్భావ <opencv2/core/utils/fp_control.private.hpp>

ఉపయోగించి నామాకాశ cv;

నామాకాశ cv {

ParallelLoopBody::~ParallelLoopBody() {}

ఉపయోగించి నామాకాశ cv::parallel;

నామాకాశ {

#నిర్వచితమైతే ENABLE_INSTRUMENTATION
    స్థిర శూన్య SyncNodes(cv::instr::Instr_కణుపు *p_కణుపు)
    {
        std::vector<cv::instr::కణుపు_దత్తాంశం_Tls*> దత్తాంశం;
        p_కణుపు->m_payload.m_tls.gather(దత్తాంశం);

        uint64 ticksMax = 0;
        పూర్ణాంకం    దారాలు  = 0;
        కొరకు(size_t i = 0; i < దత్తాంశం.size(); i++)
        {
            ఒకవేళ(దత్తాంశం[i] && దత్తాంశం[i]->m_ticks_మొత్తం)
            {
                ticksMax = MAX(ticksMax, దత్తాంశం[i]->m_ticks_మొత్తం);
                p_కణుపు->m_payload.m_ticks_మొత్తం -= దత్తాంశం[i]->m_ticks_మొత్తం;
                దత్తాంశం[i]->m_ticks_మొత్తం = 0;
                దారాలు++;
            }
        }
        p_కణుపు->m_payload.m_ticks_మొత్తం += ticksMax;
        p_కణుపు->m_payload.m_దారాలు = MAX(p_కణుపు->m_payload.m_దారాలు, దారాలు);

        కొరకు(size_t i = 0; i < p_కణుపు->m_childs.size(); i++)
            SyncNodes(p_కణుపు->m_childs[i]);
    }
#అయితే_ముగింపు

    తరగతి ParallelLoopBodyWrapperContext
    {
    బహిరంగ:
        ParallelLoopBodyWrapperContext(స్థిరరాశి cv::ParallelLoopBody& _body, స్థిరరాశి cv::Range& _r, ద్విగుణ _nstripes) :
            is_rng_వాడిన(అబద్ధం), hasException(అబద్ధం)
        {

            body = &_body;
            సంపూర్ణ_Range = _r;
            ద్విగుణ len = సంపూర్ణ_Range.end - సంపూర్ణ_Range.ప్రారంభం;
            nstripes = cv_విడత(_nstripes <= 0 ? len : MIN(MAX(_nstripes, 1.), len));

            // propagate main thread state
            rng = cv::theRNG();
#ఒకవేళ OPENCV_SUPPORTS_FP_DENORMALS_HINT && OPENCV_IMPL_FP_HINTS
            details::saveFPDenormalsState(fp_denormals_పునాది_state);
#అయితే_ముగింపు

#నిర్వచితమైతే OPENCV_TRACE
            trace_మూలం_Region = CV_TRACE_NS::details::getCurrentRegion();
            trace_మూలం_Context = CV_TRACE_NS::details::getTraceManager().tls.get();
#అయితే_ముగింపు

#నిర్వచితమైతే ENABLE_INSTRUMENTATION
            p_Thread_మూలం = cv::instr::getInstrumentTLSStruct().p_Current_కణుపు;
#అయితే_ముగింపు
        }
        శూన్య finalize()
        {
#నిర్వచితమైతే ENABLE_INSTRUMENTATION
            కొరకు(size_t i = 0; i < p_Thread_మూలం->m_childs.size(); i++)
                SyncNodes(p_Thread_మూలం->m_childs[i]);
#అయితే_ముగింపు
            ఒకవేళ (is_rng_వాడిన)
            {
                // Some parallel backends execute nested jobs in the main thread,
                // so we need to restore initial RNG state here.
                cv::theRNG() = rng;
                // We can't properly update RNG state based on RNG usage in worker threads,
                // so lets just change main thread RNG state to the next value.
                // Note: this behaviour is not equal to single-threaded mode.
                cv::theRNG().next();
            }
#నిర్వచితమైతే OPENCV_TRACE
            ఒకవేళ (trace_మూలం_Region)
                CV_TRACE_NS::details::parallelForFinalize(*trace_మూలం_Region);
#అయితే_ముగింపు

            ఒకవేళ (hasException)
            {
#ఒకవేళ CV__EXCEPTION_PTR
                std::rethrow_exception(pException);
#లేకపోతే
                CV_Error(Error::StsError, "Exception in parallel_for() body: " + exception_సందేశం);
#అయితే_ముగింపు
            }
        }
        ~ParallelLoopBodyWrapperContext() {}

        స్థిరరాశి cv::ParallelLoopBody* body;
        cv::Range సంపూర్ణ_Range;
        పూర్ణాంకం nstripes;
        cv::RNG rng;
        మార్చదగిన తార్కిక is_rng_వాడిన;
#నిర్వచితమైతే OPENCV_TRACE
        CV_TRACE_NS::details::Region* trace_మూలం_Region;
        CV_TRACE_NS::details::TraceManagerThreadLocal* trace_మూలం_Context;
#అయితే_ముగింపు
#నిర్వచితమైతే ENABLE_INSTRUMENTATION
        cv::instr::Instr_కణుపు *p_Thread_మూలం;
#అయితే_ముగింపు
        తార్కిక hasException;
#ఒకవేళ CV__EXCEPTION_PTR
        std::exception_ptr pException;
#లేకపోతే
        cv::String exception_సందేశం;
#అయితే_ముగింపు
#ఒకవేళ CV__EXCEPTION_PTR
        శూన్య రికార్డు_Exception()
#లేకపోతే
        శూన్య రికార్డు_Exception(స్థిరరాశి cv::String& సందేశం)
#అయితే_ముగింపు
        {
#నిర్వచితం_కాకపోతే CV_THREAD_SANITIZER
            ఒకవేళ (!hasException)
#అయితే_ముగింపు
            {
                cv::Auto_తాళం lock(cv::getInitializationMutex());
                ఒకవేళ (!hasException)
                {
                    hasException = నిజం;
#ఒకవేళ CV__EXCEPTION_PTR
                    pException = std::current_exception();
#లేకపోతే
                    exception_సందేశం = సందేశం;
#అయితే_ముగింపు
                }
            }
        }

#ఒకవేళ OPENCV_SUPPORTS_FP_DENORMALS_HINT && OPENCV_IMPL_FP_HINTS
        details::FPDenormals_బహుళకం_State fp_denormals_పునాది_state;
#అయితే_ముగింపు

    అంతరంగ:
        ParallelLoopBodyWrapperContext(స్థిరరాశి ParallelLoopBodyWrapperContext&); // disabled
        ParallelLoopBodyWrapperContext& క్రియాకారకం=(స్థిరరాశి ParallelLoopBodyWrapperContext&); // disabled
    };

    తరగతి ParallelLoopBodyWrapper : బహిరంగ cv::ParallelLoopBody
    {
    బహిరంగ:
        ParallelLoopBodyWrapper(ParallelLoopBodyWrapperContext& ctx_) :
            ctx(ctx_)
        {
        }
        ~ParallelLoopBodyWrapper()
        {
        }
        శూన్య క్రియాకారకం()(స్థిరరాశి cv::Range& sr) స్థిరరాశి CV_OVERRIDE
        {
#నిర్వచితమైతే OPENCV_TRACE
            // TODO CV_TRACE_NS::details::setCurrentRegion(rootRegion);
            ఒకవేళ (ctx.trace_మూలం_Region && ctx.trace_మూలం_Context)
                CV_TRACE_NS::details::parallel_For_సమితి_మూలం_Region(*ctx.trace_మూలం_Region, *ctx.trace_మూలం_Context);
            CV__TRACE_OPENCV_FUNCTION_NAME("parallel_for_body");
            ఒకవేళ (ctx.trace_మూలం_Region)
                CV_TRACE_NS::details::parallelForAttachNestedRegion(*ctx.trace_మూలం_Region);
#అయితే_ముగింపు

#నిర్వచితమైతే ENABLE_INSTRUMENTATION
            {
                cv::instr::InstrTLSStruct *pInstrTLS = &cv::instr::getInstrumentTLSStruct();
                pInstrTLS->p_Current_కణుపు = ctx.p_Thread_మూలం; // Initialize TLS node for thread
            }
            CV_INSTRUMENT_REGION();
#అయితే_ముగింపు

            // propagate main thread state
            cv::theRNG() = ctx.rng;
#ఒకవేళ OPENCV_SUPPORTS_FP_DENORMALS_HINT && OPENCV_IMPL_FP_HINTS
            FPDenormalsIgnoreHintScope fp_denormals_scope(ctx.fp_denormals_పునాది_state);
#అయితే_ముగింపు

            cv::Range r;
            cv::Range సంపూర్ణ_Range = ctx.సంపూర్ణ_Range;
            పూర్ణాంకం nstripes = ctx.nstripes;
            r.ప్రారంభం = (పూర్ణాంకం)(సంపూర్ణ_Range.ప్రారంభం +
                            ((uint64)sr.ప్రారంభం*(సంపూర్ణ_Range.end - సంపూర్ణ_Range.ప్రారంభం) + nstripes/2)/nstripes);
            r.end = sr.end >= nstripes ? సంపూర్ణ_Range.end : (పూర్ణాంకం)(సంపూర్ణ_Range.ప్రారంభం +
                            ((uint64)sr.end*(సంపూర్ణ_Range.end - సంపూర్ణ_Range.ప్రారంభం) + nstripes/2)/nstripes);

#నిర్వచితమైతే OPENCV_TRACE
            CV_TRACE_ARG_విలువ(range_ప్రారంభం, "range.start", (int64)r.ప్రారంభం);
            CV_TRACE_ARG_విలువ(range_చివర, "range.end", (int64)r.end);
#అయితే_ముగింపు

            ప్రయత్నించు
            {
                (*ctx.body)(r);
            }
#ఒకవేళ CV__EXCEPTION_PTR
            పట్టుకో (...)
            {
                ctx.రికార్డు_Exception();
            }
#లేకపోతే
            పట్టుకో (స్థిరరాశి cv::Exception& e)
            {
                ctx.రికార్డు_Exception(e.what());
            }
            పట్టుకో (స్థిరరాశి std::exception& e)
            {
                ctx.రికార్డు_Exception(e.what());
            }
            పట్టుకో (...)
            {
                ctx.రికార్డు_Exception("Unknown exception");
            }
#అయితే_ముగింపు

            ఒకవేళ (!ctx.is_rng_వాడిన && !(cv::theRNG() == ctx.rng))
                ctx.is_rng_వాడిన = నిజం;
        }
        cv::Range stripeRange() స్థిరరాశి { తిరిగి cv::Range(0, ctx.nstripes); }

    రక్షిత:
        ParallelLoopBodyWrapperContext& ctx;
    };

#ఒకవేళ defined HAVE_TBB
    తరగతి ProxyLoopBody : బహిరంగ ParallelLoopBodyWrapper
    {
    బహిరంగ:
        ProxyLoopBody(ParallelLoopBodyWrapperContext& ctx_)
        : ParallelLoopBodyWrapper(ctx_)
        {}

        శూన్య క్రియాకారకం ()(స్థిరరాశి tbb::blocked_range<పూర్ణాంకం>& range) స్థిరరాశి
        {
            ఇది->ParallelLoopBodyWrapper::క్రియాకారకం()(cv::Range(range.begin(), range.end()));
        }

        శూన్య క్రియాకారకం ()() స్థిరరాశి  // run parallel job
        {
            cv::Range range = ఇది->stripeRange();
            tbb::parallel_for(tbb::blocked_range<పూర్ణాంకం>(range.ప్రారంభం, range.end), *ఇది);
        }
    };
#లేదావేళ defined HAVE_HPX
    తరగతి ProxyLoopBody : బహిరంగ ParallelLoopBodyWrapper
    {
    బహిరంగ:
        ProxyLoopBody(ParallelLoopBodyWrapperContext& ctx_)
                : ParallelLoopBodyWrapper(ctx_)
        {}

        శూన్య క్రియాకారకం ()() స్థిరరాశి  // run parallel job
        {
            cv::Range stripeRange = ఇది->stripeRange();
            hpx::parallel::for_loop(
                    hpx::parallel::execution::par,
                    stripeRange.ప్రారంభం, stripeRange.end,
                    [&](స్థిరరాశి పూర్ణాంకం &i) { ;
                        ఇది->ParallelLoopBodyWrapper::క్రియాకారకం()(
                                cv::Range(i, i + 1));
                    });
        }
    };
#లేదావేళ defined HAVE_OPENMP
    రకనిర్వచనం ParallelLoopBodyWrapper ProxyLoopBody;
#లేదావేళ defined HAVE_గరిష్ఠ_సామాన్య_భాజకం
    రకనిర్వచనం ParallelLoopBodyWrapper ProxyLoopBody;
    స్థిర శూన్య ఖండం_function(శూన్య* context, size_t సూచీ)
    {
        ProxyLoopBody* ptr_body = static_cast<ProxyLoopBody*>(context);
        (*ptr_body)(cv::Range((పూర్ణాంకం)సూచీ, (పూర్ణాంకం)సూచీ + 1));
    }
#లేదావేళ defined WINRT || defined HAVE_CONCURRENCY
    తరగతి ProxyLoopBody : బహిరంగ ParallelLoopBodyWrapper
    {
    బహిరంగ:
        ProxyLoopBody(ParallelLoopBodyWrapperContext& ctx)
        : ParallelLoopBodyWrapper(ctx)
        {}

        శూన్య క్రియాకారకం ()(పూర్ణాంకం i) స్థిరరాశి
        {
            ఇది->ParallelLoopBodyWrapper::క్రియాకారకం()(cv::Range(i, i + 1));
        }
    };

    // PPL's default auto_partitioner silently skips whole chunks of the range on
    // ARM64 (unfixed MSVC bug, developercommunity.visualstudio.com/t/1027444).
    మూస <రకం_పేరు Body>
    స్థిర అంతఃవరుస శూన్య pplParallelFor(పూర్ణాంకం first, పూర్ణాంకం చివరి, స్థిరరాశి Body& body)
    {
#ఒకవేళ defined(_M_ARM64) || defined(_M_ARM64EC)
        Concurrency::parallel_for(first, చివరి, body, Concurrency::static_partitioner());
#లేకపోతే
        Concurrency::parallel_for(first, చివరి, body);
#అయితే_ముగింపు
    }
#లేకపోతే
    రకనిర్వచనం ParallelLoopBodyWrapper ProxyLoopBody;
#అయితే_ముగింపు

#ఒకవేళ defined HAVE_TBB
    #ఒకవేళ TBB_INTERFACE_సంస్కరణ >= 8000
        స్థిర tbb::task_arena tbbArena(tbb::task_arena::automatic);
    #లేకపోతే
        స్థిర tbb::task_scheduler_init tbbScheduler(tbb::task_scheduler_init::deferred);
    #అయితే_ముగింపు
#లేదావేళ defined HAVE_HPX
// nothing for HPX
#లేదావేళ defined HAVE_OPENMP
స్థిర అంతఃవరుస పూర్ణాంకం init_Max_దారాలు()
{
    పూర్ణాంకం max_దారాలు = omp_get_max_దారాలు();
    తిరిగి max_దారాలు;
}
స్థిర పూర్ణాంకం అంకె_దారాలు_Max = init_Max_దారాలు();
#లేదావేళ defined HAVE_గరిష్ఠ_సామాన్య_భాజకం
// nothing for GCD
#లేదావేళ defined WINRT
// nothing for WINRT
#లేదావేళ defined HAVE_CONCURRENCY

తరగతి SchedPtr
{
    Concurrency::Scheduler* sched_;
బహిరంగ:
    Concurrency::Scheduler* క్రియాకారకం->() { తిరిగి sched_; }
    క్రియాకారకం Concurrency::Scheduler*() { తిరిగి sched_; }

    శూన్య క్రియాకారకం=(Concurrency::Scheduler* sched)
    {
        ఒకవేళ (sched_) sched_->Release();
        sched_ = sched;
    }

    SchedPtr() : sched_(0) {}
    ~SchedPtr() {}
};
స్థిర SchedPtr pplScheduler;

#అయితే_ముగింపు

} // namespace anon

/* ================================   parallel_for_  ================================ */

స్థిర శూన్య parallel_for_impl(స్థిరరాశి cv::Range& range, స్థిరరాశి cv::ParallelLoopBody& body, ద్విగుణ nstripes); // forward declaration

శూన్య parallel_for_(స్థిరరాశి cv::Range& range, స్థిరరాశి cv::ParallelLoopBody& body, ద్విగుణ nstripes)
{
#నిర్వచితమైతే OPENCV_TRACE
    CV__TRACE_OPENCV_FUNCTION_NAME_("parallel_for", 0);
    CV_TRACE_ARG_విలువ(range_ప్రారంభం, "range.start", (int64)range.ప్రారంభం);
    CV_TRACE_ARG_విలువ(range_చివర, "range.end", (int64)range.end);
    CV_TRACE_ARG_విలువ(nstripes, "nstripes", (int64)nstripes);
#అయితే_ముగింపు

    CV_INSTRUMENT_REGION_MT_FORK();
    ఒకవేళ (range.empty())
        తిరిగి;

    స్థిర std::atomic<తార్కిక> flagNestedParallelFor(అబద్ధం);
    తార్కిక isNotNestedRegion = !flagNestedParallelFor.load();
    ఒకవేళ (isNotNestedRegion)
      isNotNestedRegion = !flagNestedParallelFor.exchange(నిజం);
    ఒకవేళ (isNotNestedRegion)
    {
        ప్రయత్నించు
        {
            parallel_for_impl(range, body, nstripes);
            flagNestedParallelFor = అబద్ధం;
        }
        పట్టుకో (...)
        {
            flagNestedParallelFor = అబద్ధం;
            విసురు;
        }
    }
    లేకపోతే // nested parallel_for_() calls are not parallelized
    {
        CV_వాడని(nstripes);
        body(range);
    }
}

స్థిర
శూన్య parallel_for_cb(పూర్ణాంకం ప్రారంభం, పూర్ణాంకం end, శూన్య* దత్తాంశం)
{
    CV_DbgAssert(దత్తాంశం);
    స్థిరరాశి cv::ParallelLoopBody& body = *(స్థిరరాశి cv::ParallelLoopBody*)దత్తాంశం;
    body(Range(ప్రారంభం, end));
}

స్థిర శూన్య parallel_for_impl(స్థిరరాశి cv::Range& range, స్థిరరాశి cv::ParallelLoopBody& body, ద్విగుణ nstripes)
{
    ఉపయోగించి నామాకాశ cv::parallel;
    ఒకవేళ ((అంకె_దారాలు < 0 || అంకె_దారాలు > 1) && range.end - range.ప్రారంభం > 1)
    {
        ParallelLoopBodyWrapperContext ctx(body, range, nstripes);
        ProxyLoopBody pbody(ctx);
        cv::Range stripeRange = pbody.stripeRange();
        ఒకవేళ( stripeRange.end - stripeRange.ప్రారంభం == 1 )
        {
            body(range);
            తిరిగి;
        }

        std::shared_ptr<ParallelForAPI>& api = getCurrentParallelForAPI();
        ఒకవేళ (api)
        {
            CV_CheckEQ(stripeRange.ప్రారంభం, 0, "");
            api->parallel_for(stripeRange.end, parallel_for_cb, (శూన్య*)&pbody);
            ctx.finalize();  // propagate exceptions if exists
            తిరిగి;
        }

#నిర్వచితమైతే CV_PARALLEL_FRAMEWORK
#ఒకవేళ defined HAVE_TBB

#ఒకవేళ TBB_INTERFACE_సంస్కరణ >= 8000
        tbbArena.execute(pbody);
#లేకపోతే
        pbody();
#అయితే_ముగింపు

#లేదావేళ defined HAVE_HPX
        pbody();

#లేదావేళ defined HAVE_OPENMP

        #సూచన omp parallel కొరకు కాలపట్టిక(dynamic) అంకె_దారాలు_2(అంకె_దారాలు > 0 ? అంకె_దారాలు : అంకె_దారాలు_Max)
        కొరకు (పూర్ణాంకం i = stripeRange.ప్రారంభం; i < stripeRange.end; ++i)
            pbody(Range(i, i + 1));

#లేదావేళ defined HAVE_గరిష్ఠ_సామాన్య_భాజకం

        dispatch_వరుస_t concurrent_వరుస = dispatch_get_global_వరుస(DISPATCH_వరుస_ప్రాధాన్యత_DEFAULT, 0);
        dispatch_apply_f(stripeRange.end - stripeRange.ప్రారంభం, concurrent_వరుస, &pbody, ఖండం_function);

#లేదావేళ defined WINRT

        pplParallelFor(stripeRange.ప్రారంభం, stripeRange.end, pbody);

#లేదావేళ defined HAVE_CONCURRENCY

        ఒకవేళ(!pplScheduler || pplScheduler->Id() == Concurrency::CurrentScheduler::Id())
        {
            pplParallelFor(stripeRange.ప్రారంభం, stripeRange.end, pbody);
        }
        లేకపోతే
        {
            pplScheduler->Attach();
            pplParallelFor(stripeRange.ప్రారంభం, stripeRange.end, pbody);
            Concurrency::CurrentScheduler::Detach();
        }

#లేదావేళ defined HAVE_PTHREADS_PF

        parallel_for_pthreads(pbody.stripeRange(), pbody, pbody.stripeRange().size());

#లేకపోతే

#దోషం You have hacked and compiling with unsupported parallel framework

#అయితే_ముగింపు

        ctx.finalize();  // propagate exceptions if exists
        తిరిగి;
#అయితే_ముగింపు // CV_PARALLEL_FRAMEWORK
    }

    body(range);
}


పూర్ణాంకం get_అంకె_దారాలు(శూన్య)
{
    std::shared_ptr<ParallelForAPI>& api = getCurrentParallelForAPI();
    ఒకవేళ (api)
    {
        తిరిగి api->get_అంకె_దారాలు();
    }

    ఒకవేళ (అంకె_దారాలు == 0)
        తిరిగి 1;

#ఒకవేళ defined HAVE_TBB

#ఒకవేళ TBB_INTERFACE_సంస్కరణ >= 9100
    తిరిగి tbbArena.max_concurrency();
#లేదావేళ TBB_INTERFACE_సంస్కరణ >= 8000
    తిరిగి అంకె_దారాలు > 0
        ? అంకె_దారాలు
        : tbb::task_scheduler_init::default_అంకె_దారాలు();
#లేకపోతే
    తిరిగి tbbScheduler.is_చురుకైన()
           ? అంకె_దారాలు
           : tbb::task_scheduler_init::default_అంకె_దారాలు();
#అయితే_ముగింపు

#లేదావేళ defined HAVE_HPX
    తిరిగి అంకె_దారాలు;

#లేదావేళ defined HAVE_OPENMP

    తిరిగి అంకె_దారాలు > 0
           ? అంకె_దారాలు
           : అంకె_దారాలు_Max;


#లేదావేళ defined HAVE_గరిష్ఠ_సామాన్య_భాజకం

    తిరిగి cv::get_సంఖ్య_యొక్క_CPUs(); // the GCD thread pool limit

#లేదావేళ defined WINRT

    తిరిగి 0;

#లేదావేళ defined HAVE_CONCURRENCY

    తిరిగి (pplScheduler == 0)
        ? Concurrency::CurrentScheduler::Get()->Get_సంఖ్య_యొక్క_Virtual_Processors()
        : (1 + pplScheduler->Get_సంఖ్య_యొక్క_Virtual_Processors());

#లేదావేళ defined HAVE_PTHREADS_PF

        తిరిగి parallel_pthreads_get_దారాలు_అంకె();

#లేకపోతే

    తిరిగి 1;

#అయితే_ముగింపు
}

గుర్తులేని default_సంఖ్య_యొక్క_దారాలు()
{
#నిర్వచితమైతే __ANDROID__
    // many modern phones/tables have 4-core CPUs. Let's use no more
    // than 2 threads by default not to overheat the devices
    స్థిరరాశి గుర్తులేని పూర్ణాంకం default_సంఖ్య_యొక్క_దారాలు_2 = 2;
#లేకపోతే
    స్థిరరాశి గుర్తులేని పూర్ణాంకం default_సంఖ్య_యొక్క_దారాలు_2 = (గుర్తులేని పూర్ణాంకం)std::max(1, cv::get_సంఖ్య_యొక్క_CPUs());
#అయితే_ముగింపు

    గుర్తులేని ఫలితం = default_సంఖ్య_యొక్క_దారాలు_2;

    స్థిర పూర్ణాంకం config_అంకె_దారాలు = (పూర్ణాంకం)utils::get_Configuration_Parameter_కొలత_T("OPENCV_FOR_THREADS_NUM", 0);

    ఒకవేళ (config_అంకె_దారాలు)
    {
        ఫలితం = (గుర్తులేని)std::max(1, config_అంకె_దారాలు);
        //do we need upper limit of threads number?
    }
    తిరిగి ఫలితం;
}

శూన్య సమితి_అంకె_దారాలు( పూర్ణాంకం దారాలు_2 )
{
    CV_వాడని(దారాలు_2);

    పూర్ణాంకం దారాలు = (దారాలు_2 < 0) ? default_సంఖ్య_యొక్క_దారాలు() : (గుర్తులేని)దారాలు_2;
    అంకె_దారాలు = దారాలు;

    std::shared_ptr<ParallelForAPI>& api = getCurrentParallelForAPI();
    ఒకవేళ (api)
    {
        api->సమితి_అంకె_దారాలు(అంకె_దారాలు);
    }

#నిర్వచితమైతే HAVE_TBB

#ఒకవేళ TBB_INTERFACE_సంస్కరణ >= 8000
    ఒకవేళ(tbbArena.is_చురుకైన()) tbbArena.terminate();
    ఒకవేళ(దారాలు > 0) tbbArena.initialize(దారాలు);
#లేకపోతే
    ఒకవేళ(tbbScheduler.is_చురుకైన()) tbbScheduler.terminate();
    ఒకవేళ(దారాలు > 0) tbbScheduler.initialize(దారాలు);
#అయితే_ముగింపు

#లేదావేళ defined HAVE_HPX
    తిరిగి; // nothing needed as numThreads is used

#లేదావేళ defined HAVE_OPENMP

    తిరిగి; // nothing needed as num_threads clause is used in #pragma omp parallel for

#లేదావేళ defined HAVE_గరిష్ఠ_సామాన్య_భాజకం

    // unsupported
    // there is only private dispatch_queue_set_width() and only for desktop

#లేదావేళ defined WINRT

    తిరిగి;

#లేదావేళ defined HAVE_CONCURRENCY

    ఒకవేళ (దారాలు <= 0)
    {
        pplScheduler = 0;
    }
    లేకపోతే ఒకవేళ (దారాలు == 1)
    {
        // Concurrency always uses >=2 threads, so we just disable it if 1 thread is requested
        అంకె_దారాలు = 0;
    }
    లేకపోతే ఒకవేళ (pplScheduler == 0 || 1 + pplScheduler->Get_సంఖ్య_యొక్క_Virtual_Processors() != (గుర్తులేని పూర్ణాంకం)దారాలు)
    {
        pplScheduler = Concurrency::Scheduler::Create(Concurrency::SchedulerPolicy(2,
                       Concurrency::MinConcurrency, దారాలు-1,
                       Concurrency::MaxConcurrency, దారాలు-1));
    }

#లేదావేళ defined HAVE_PTHREADS_PF

    parallel_pthreads_సమితి_దారాలు_అంకె(దారాలు);

#అయితే_ముగింపు
}


పూర్ణాంకం get_Thread_అంకె()
{
    std::shared_ptr<ParallelForAPI>& api = getCurrentParallelForAPI();
    ఒకవేళ (api)
    {
        తిరిగి api->get_Thread_అంకె();
    }

#ఒకవేళ defined HAVE_TBB
    #ఒకవేళ TBB_INTERFACE_సంస్కరణ >= 9100
        తిరిగి tbb::this_task_arena::current_thread_సూచీ();
    #లేదావేళ TBB_INTERFACE_సంస్కరణ >= 8000
        తిరిగి tbb::task_arena::current_thread_సూచీ();
    #లేకపోతే
        తిరిగి 0;
    #అయితే_ముగింపు
#లేదావేళ defined HAVE_HPX
    తిరిగి (పూర్ణాంకం)(hpx::get_అంకె_కార్మికుడు_దారాలు());
#లేదావేళ defined HAVE_OPENMP
    తిరిగి omp_get_thread_అంకె();
#లేదావేళ defined HAVE_గరిష్ఠ_సామాన్య_భాజకం
    తిరిగి (పూర్ణాంకం)(size_t)(శూన్య*)pthread_self(); // no zero-based indexing
#లేదావేళ defined WINRT
    తిరిగి 0;
#లేదావేళ defined HAVE_CONCURRENCY
    తిరిగి std::max(0, (పూర్ణాంకం)Concurrency::Context::VirtualProcessorId()); // zero for master thread, unique number for others but not necessary 1,2,3,...
#లేదావేళ defined HAVE_PTHREADS_PF
    తిరిగి (పూర్ణాంకం)(size_t)(శూన్య*)pthread_self(); // no zero-based indexing
#లేకపోతే
    తిరిగి 0;
#అయితే_ముగింపు
}


#ఒకవేళ defined __linux__ || defined __GLIBC__ || defined __HAIKU__ || defined __ANDROID__
  #నిర్వచించు CV_CPU_GROUPS_1
#అయితే_ముగింపు

#ఒకవేళ defined __linux__ || defined __ANDROID__
  #నిర్వచించు CV_HAVE_CGROUPS 1
#అయితే_ముగింపు

#ఒకవేళ defined CV_CPU_GROUPS_1
స్థిర అంతఃవరుస
std::string getFileContents(స్థిరరాశి అక్షరం *filename)
{
    std::ifstream ifs(filename);
    ఒకవేళ (!ifs.is_open())
        తిరిగి std::string();

    std::string content( (std::istreambuf_iterator<అక్షరం>(ifs) ),
                         (std::istreambuf_iterator<అక్షరం>()    ) );

    ఒకవేళ (ifs.fail())
        తిరిగి std::string();

    తిరిగి content;
}

స్థిర అంతఃవరుస
పూర్ణాంకం get_సంఖ్య_యొక్క_CPUs_Impl(స్థిరరాశి అక్షరం *filename)
{
   std::string file_contents = getFileContents(filename);
   ఒకవేళ(file_contents.empty())
       తిరిగి 0;

   అక్షరం *pbuf = const_cast<అక్షరం*>(file_contents.c_str());
   //parse string of form "0-1,3,5-7,10,13-15"
   పూర్ణాంకం cpus_అందుబాటు = 0;

   అయితే(*pbuf)
   {
      స్థిరరాశి అక్షరం* pos = pbuf;
      తార్కిక range = అబద్ధం;
      అయితే(*pbuf && *pbuf != ',')
      {
          ఒకవేళ(*pbuf == '-') range = నిజం;
          ++pbuf;
      }
      ఒకవేళ(*pbuf) *pbuf++ = 0;
      ఒకవేళ(!range)
        ++cpus_అందుబాటు;
      లేకపోతే
      {
          పూర్ణాంకం rstart = 0, rend = 0;
          అక్షరశ్రేణి_చదువు(pos, "%d-%d", &rstart, &rend);
          cpus_అందుబాటు += rend - rstart + 1;
      }

   }
   తిరిగి cpus_అందుబాటు;
}
#అయితే_ముగింపు

#ఒకవేళ defined CV_HAVE_CGROUPS
స్థిర అంతఃవరుస
గుర్తులేని get_సంఖ్య_యొక్క_CPUs_CFSv2()
{
    పూర్ణాంకం cfs_quota = 0;
    పూర్ణాంకం cfs_period = 0;

    std::ifstream ss_cpu_max("/sys/fs/cgroup/cpu.max", std::ios::in | std::ios::binary);
    ss_cpu_max >> cfs_quota >> cfs_period;

    ఒకవేళ (ss_cpu_max.fail() || cfs_quota < 1 || cfs_period < 1) /* values must not be 0 or negative */
        తిరిగి 0;

    తిరిగి (గుర్తులేని)max(1, cfs_quota/cfs_period);
}

స్థిర అంతఃవరుస
గుర్తులేని get_సంఖ్య_యొక్క_CPUs_CFSv1()
{
    పూర్ణాంకం cfs_quota = 0;
    {
        std::ifstream ss_period("/sys/fs/cgroup/cpu/cpu.cfs_quota_us", std::ios::in | std::ios::binary);
        ss_period >> cfs_quota;

        ఒకవేళ (ss_period.fail() || cfs_quota < 1) /* cfs_quota must not be 0 or negative */
            తిరిగి 0;
    }

    పూర్ణాంకం cfs_period = 0;
    {
        std::ifstream ss_quota("/sys/fs/cgroup/cpu/cpu.cfs_period_us", std::ios::in | std::ios::binary);
        ss_quota >> cfs_period;

        ఒకవేళ (ss_quota.fail() || cfs_period < 1)
            తిరిగి 0;
    }

    తిరిగి (గుర్తులేని)max(1, cfs_quota/cfs_period);
}
#అయితే_ముగింపు

మూస <రకం_పేరు T> స్థిర అంతఃవరుస
T minNonZero(స్థిరరాశి T& val_1, స్థిరరాశి T& val_2)
{
    ఒకవేళ ((val_1 != 0) && (val_2 != 0))
        తిరిగి std::min(val_1, val_2);
    తిరిగి (val_1 != 0) ? val_1 : val_2;
}

#నిర్వచితం_కాకపోతే OPENCV_DISABLE_THREAD_SUPPORT
స్థిర
పూర్ణాంకం get_సంఖ్య_యొక్క_CPUs_2()
{
#నిర్వచితం_కాకపోతే OPENCV_SEMIHOSTING
    /*
     * Logic here is to try different methods of getting CPU counts and return
     * the minimum most value as it has high probablity of being right and safe.
     * Return 1 if we get 0 or not found on all methods.
    */
#ఒకవేళ !defined(__MINGW32__) /* not implemented (2020-03) */

    /*
     * Check for this standard C++11 way, we do not return directly because
     * running in a docker or K8s environment will mean this is the host
     * machines config not the containers or pods and as per docs this value
     * must be "considered only a hint".
    */
    గుర్తులేని ncpus = std::thread::hardware_concurrency(); /* If the value is not well defined or not computable, returns 0 */
#లేకపోతే
    గుర్తులేని ncpus = 0; /* 0 means we have to find out some other way */
#అయితే_ముగింపు

#ఒకవేళ defined _WIN32

    SYSTEM_INFO sysinfo = {};
#ఒకవేళ (defined(_M_ARM) || defined(_M_ARM64) || defined(_M_ARM64EC) || defined(_M_X64) || defined(WINRT)) && _WIN32_WINNT >= 0x501
    GetNativeSystemInfo( &sysinfo );
#లేకపోతే
    GetSystemInfo( &sysinfo );
#అయితే_ముగింపు
    గుర్తులేని ncpus_sysinfo = sysinfo.dw_సంఖ్య_యొక్క_Processors;
    ncpus = minNonZero(ncpus, ncpus_sysinfo);

#లేదావేళ defined __APPLE__

    పూర్ణాంకం అంకె_CPU=0;
    పూర్ణాంకం mib[4];
    size_t len = పరిమాణం(అంకె_CPU);

    /* set the mib for hw.ncpu */
    mib[0] = CTL_HW;
    mib[1] = HW_AVAILCPU;  // alternatively, try HW_NCPU;

    /* get the number of CPUs from the system */
    sysctl(mib, 2, &అంకె_CPU, &len, ఖాళీ, 0);

    ఒకవేళ( అంకె_CPU < 1 )
    {
        mib[1] = HW_NCPU;
        sysctl( mib, 2, &అంకె_CPU, &len, ఖాళీ, 0 );

        ఒకవేళ( అంకె_CPU < 1 )
            అంకె_CPU = 1;
    }

    ncpus = minNonZero(ncpus, (గుర్తులేని)అంకె_CPU);

#లేదావేళ defined CV_CPU_GROUPS_1

#ఒకవేళ defined CV_HAVE_CGROUPS
    స్థిర గుర్తులేని ncpus_impl_cpuset = (గుర్తులేని)get_సంఖ్య_యొక్క_CPUs_Impl("/sys/fs/cgroup/cpuset/cpuset.cpus");
    ncpus = minNonZero(ncpus, ncpus_impl_cpuset);

    స్థిర గుర్తులేని ncpus_impl_cfs_v1 = get_సంఖ్య_యొక్క_CPUs_CFSv1();
    ncpus = minNonZero(ncpus, ncpus_impl_cfs_v1);

    స్థిర గుర్తులేని ncpus_impl_cfs_v2 = get_సంఖ్య_యొక్క_CPUs_CFSv2();
    ncpus = minNonZero(ncpus, ncpus_impl_cfs_v2);
#అయితే_ముగింపు

    స్థిర గుర్తులేని ncpus_impl_devices = (గుర్తులేని)get_సంఖ్య_యొక్క_CPUs_Impl("/sys/devices/system/cpu/online");
    ncpus = minNonZero(ncpus, ncpus_impl_devices);

#అయితే_ముగింపు

#ఒకవేళ defined _GNU_SOURCE \
    && !defined(__MINGW32__) /* not implemented (2020-03) */ \
    && !defined(__EMSCRIPTEN__) \
    && !defined(__ANDROID__)  // TODO: add check for modern Android NDK

    cpu_set_t cpu_సమితి;
    ఒకవేళ (0 == sched_getaffinity(0, పరిమాణం(cpu_సమితి), &cpu_సమితి))
    {
        గుర్తులేని cpu_లెక్క_cpu_సమితి = CPU_COUNT(&cpu_సమితి);
        ncpus = minNonZero(ncpus, cpu_లెక్క_cpu_సమితి);
    }

#అయితే_ముగింపు

#ఒకవేళ !defined(_WIN32) && !defined(__APPLE__) && defined(_SC_NPROCESSORS_ONLN)

    స్థిర గుర్తులేని cpu_లెక్క_sysconf = (గుర్తులేని)sysconf( _SC_NPROCESSORS_ONLN );
    ncpus = minNonZero(ncpus, cpu_లెక్క_sysconf);
#లేదావేళ defined (__QNX__)
    స్థిర గుర్తులేని cpu_లెక్క_sysconf = _syspage_ptr->అంకె_cpu;
    ncpus = minNonZero(ncpus, cpu_లెక్క_sysconf);
#అయితే_ముగింపు

    తిరిగి ncpus != 0 ? ncpus : 1;
#లేకపోతే //  OPENCV_SEMIHOSTING
    తిరిగి 1;
#అయితే_ముగింపు //OPENCV_SEMIHOSTING
}

పూర్ణాంకం get_సంఖ్య_యొక్క_CPUs()
{
    స్థిర పూర్ణాంకం nCPUs = get_సంఖ్య_యొక్క_CPUs_2();
    తిరిగి nCPUs;  // cached value
}

#లేకపోతే  // OPENCV_DISABLE_THREAD_SUPPORT
పూర్ణాంకం get_సంఖ్య_యొక్క_CPUs()
{
    తిరిగి 1;
}
#అయితే_ముగింపు  // OPENCV_DISABLE_THREAD_SUPPORT

స్థిరరాశి అక్షరం* currentParallelFramework()
{
    std::shared_ptr<ParallelForAPI>& api = getCurrentParallelForAPI();
    ఒకవేళ (api)
    {
        తిరిగి api->getName();
    }
#నిర్వచితమైతే CV_PARALLEL_FRAMEWORK
    తిరిగి CV_PARALLEL_FRAMEWORK;
#లేకపోతే
    తిరిగి ఖాళీ;
#అయితే_ముగింపు
}

}  // namespace cv::
