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

#ضمن "precomp.hpp"

#ضمن <opencv2/core/utils/configuration.private.hpp>
#ضمن <opencv2/core/utils/trace.private.hpp>

#ضمن "opencv2/core/parallel/parallel_backend.hpp"
#ضمن "parallel/parallel.hpp"

#إذا defined _WIN32 || defined WINCE
    #ضمن <windows.h>
    #وصف_رد small
    #وصف_رد min
    #وصف_رد max
    #وصف_رد مطلق_قدر
#جيڪڏهن_پورو

#إذا defined __unix__ || defined __APPLE__ || defined __GLIBC__ \
    || defined __HAIKU__ || defined __EMSCRIPTEN__ \
    || defined __FreeBSD__ || defined __NetBSD__ || defined __OpenBSD__
    #ضمن <unistd.h>
    #ضمن <stdio.h>
    #ضمن <sys/types.h>
    #ضمن <fstream>
    #إذا defined __ANDROID__
        #ضمن <sys/sysconf.h>
        #ضمن <sys/syscall.h>
        #ضمن <sched.h>
    #وإلاإذا defined __APPLE__
        #ضمن <sys/sysctl.h>
    #جيڪڏهن_پورو
#جيڪڏهن_پورو

#إذا defined (__QNX__)
    #ضمن <sys/syspage.h>
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل OPENCV_DISABLE_THREAD_SUPPORT
    #ضمن <thread>
#جيڪڏهن_پورو

#جيڪڏهن_وصف_ٿيل _OPENMP
    #وصف_ڪريو HAVE_OPENMP
#جيڪڏهن_پورو

#جيڪڏهن_وصف_ٿيل __APPLE__
    #وصف_ڪريو HAVE_وڏو_گڏيل_ونڊيندڙ
#جيڪڏهن_پورو

#إذا defined _MSC_VER
    #وصف_ڪريو HAVE_CONCURRENCY
#جيڪڏهن_پورو

/* IMPORTANT: always use the same order of defines
   - HAVE_TBB         - 3rdparty library, should be explicitly enabled
   - HAVE_HPX         - 3rdparty library, should be explicitly enabled
   - HAVE_OPENMP      - integrated to compiler, should be explicitly enabled
   - HAVE_GCD         - system wide, used automatically        (APPLE only)
   - WINRT            - system wide, used automatically        (Windows RT only)
   - HAVE_CONCURRENCY - part of runtime, used automatically    (Windows only - MSVS 10, MSVS 11)
   - HAVE_PTHREADS_PF - pthreads if available
*/

#إذا defined HAVE_TBB
    #جيڪڏهن_وصف_نه_ٿيل TBB_SUPPRESS_DEPRECATED_MESSAGES  // supress warning
    #وصف_ڪريو TBB_SUPPRESS_DEPRECATED_MESSAGES 1
    #جيڪڏهن_پورو
    #ضمن "tbb/tbb.h"
    #ضمن "tbb/task.h"
    #إذا TBB_INTERFACE_نسخو >= 8000
        #ضمن "tbb/task_arena.h"
    #جيڪڏهن_پورو
    #وصف_رد min
    #وصف_رد max
#وإلاإذا defined HAVE_HPX
    #ضمن <hpx/parallel/algorithms/for_loop.hpp>
    #ضمن <hpx/parallel/execution.hpp>
    //
    #ضمن <hpx/hpx_start.hpp>
    #ضمن <hpx/hpx_suspend.hpp>
    #ضمن <hpx/include/apply.hpp>
    #ضمن <hpx/util/yield_while.hpp>
    #ضمن <hpx/include/threadmanager.hpp>

#وإلاإذا defined HAVE_OPENMP
    #ضمن <omp.h>
#وإلاإذا defined HAVE_وڏو_گڏيل_ونڊيندڙ
    #ضمن <dispatch/dispatch.h>
    #ضمن <pthread.h>
#وإلاإذا defined HAVE_CONCURRENCY
    #ضمن <ppl.h>
#وإلاإذا defined HAVE_PTHREADS_PF
    #ضمن <pthread.h>
#جيڪڏهن_پورو


#إذا defined HAVE_TBB
#  define CV_PARALLEL_FRAMEWORK "tbb"
#وإلاإذا defined HAVE_HPX
#  define CV_PARALLEL_FRAMEWORK "hpx"
#وإلاإذا defined HAVE_OPENMP
#  define CV_PARALLEL_FRAMEWORK "openmp"
#وإلاإذا defined HAVE_وڏو_گڏيل_ونڊيندڙ
#  define CV_PARALLEL_FRAMEWORK "gcd"
#وإلاإذا defined WINRT
#  define CV_PARALLEL_FRAMEWORK "winrt-concurrency"
#وإلاإذا defined HAVE_CONCURRENCY
#  define CV_PARALLEL_FRAMEWORK "ms-concurrency"
#وإلاإذا defined HAVE_PTHREADS_PF
#  define CV_PARALLEL_FRAMEWORK "pthreads"
#جيڪڏهن_پورو

#ضمن <atomic>

#ضمن "parallel_impl.hpp"

#ضمن "opencv2/core/detail/exception_ptr.hpp"  // CV__EXCEPTION_PTR = 1 if std::exception_ptr is available

#ضمن <opencv2/core/utils/fp_control_utils.hpp>
#ضمن <opencv2/core/utils/fp_control.private.hpp>

استعمال نطاق cv;

نطاق cv {

ParallelLoopBody::~ParallelLoopBody() {}

استعمال نطاق cv::parallel;

نطاق {

#جيڪڏهن_وصف_ٿيل ENABLE_INSTRUMENTATION
    ساكن فراغ SyncNodes(cv::instr::Instr_ڳنڍ *p_ڳنڍ)
    {
        std::vector<cv::instr::ڳنڍ_معلومات_Tls*> معلومات;
        p_ڳنڍ->m_payload.m_tls.gather(معلومات);

        uint64 ticksMax = 0;
        صحيح    ڌاڳا  = 0;
        لكل(size_t i = 0; i < معلومات.size(); i++)
        {
            إذا(معلومات[i] && معلومات[i]->m_ticks_ڪل)
            {
                ticksMax = MAX(ticksMax, معلومات[i]->m_ticks_ڪل);
                p_ڳنڍ->m_payload.m_ticks_ڪل -= معلومات[i]->m_ticks_ڪل;
                معلومات[i]->m_ticks_ڪل = 0;
                ڌاڳا++;
            }
        }
        p_ڳنڍ->m_payload.m_ticks_ڪل += ticksMax;
        p_ڳنڍ->m_payload.m_ڌاڳا = MAX(p_ڳنڍ->m_payload.m_ڌاڳا, ڌاڳا);

        لكل(size_t i = 0; i < p_ڳنڍ->m_childs.size(); i++)
            SyncNodes(p_ڳنڍ->m_childs[i]);
    }
#جيڪڏهن_پورو

    صنف ParallelLoopBodyWrapperContext
    {
    عام:
        ParallelLoopBodyWrapperContext(ثابت cv::ParallelLoopBody& _body, ثابت cv::Range& _r, مزدوج _nstripes) :
            is_rng_استعمال_ٿيل(خطأ), hasException(خطأ)
        {

            body = &_body;
            سڄو_Range = _r;
            مزدوج len = سڄو_Range.end - سڄو_Range.شروعات;
            nstripes = cv_ڦيرو(_nstripes <= 0 ? len : MIN(MAX(_nstripes, 1.), len));

            // propagate main thread state
            rng = cv::theRNG();
#إذا OPENCV_SUPPORTS_FP_DENORMALS_HINT && OPENCV_IMPL_FP_HINTS
            details::saveFPDenormalsState(fp_denormals_بنياد_state);
#جيڪڏهن_پورو

#جيڪڏهن_وصف_ٿيل OPENCV_TRACE
            trace_پاڙ_Region = CV_TRACE_NS::details::getCurrentRegion();
            trace_پاڙ_Context = CV_TRACE_NS::details::getTraceManager().tls.get();
#جيڪڏهن_پورو

#جيڪڏهن_وصف_ٿيل ENABLE_INSTRUMENTATION
            p_Thread_پاڙ = cv::instr::getInstrumentTLSStruct().p_Current_ڳنڍ;
#جيڪڏهن_پورو
        }
        فراغ finalize()
        {
#جيڪڏهن_وصف_ٿيل ENABLE_INSTRUMENTATION
            لكل(size_t i = 0; i < p_Thread_پاڙ->m_childs.size(); i++)
                SyncNodes(p_Thread_پاڙ->m_childs[i]);
#جيڪڏهن_پورو
            إذا (is_rng_استعمال_ٿيل)
            {
                // Some parallel backends execute nested jobs in the main thread,
                // so we need to restore initial RNG state here.
                cv::theRNG() = rng;
                // We can't properly update RNG state based on RNG usage in worker threads,
                // so lets just change main thread RNG state to the next value.
                // Note: this behaviour is not equal to single-threaded mode.
                cv::theRNG().next();
            }
#جيڪڏهن_وصف_ٿيل OPENCV_TRACE
            إذا (trace_پاڙ_Region)
                CV_TRACE_NS::details::parallelForFinalize(*trace_پاڙ_Region);
#جيڪڏهن_پورو

            إذا (hasException)
            {
#إذا CV__EXCEPTION_PTR
                std::rethrow_exception(pException);
#وإلا
                CV_Error(Error::StsError, "Exception in parallel_for() body: " + exception_پيغام);
#جيڪڏهن_پورو
            }
        }
        ~ParallelLoopBodyWrapperContext() {}

        ثابت cv::ParallelLoopBody* body;
        cv::Range سڄو_Range;
        صحيح nstripes;
        cv::RNG rng;
        بدلجندڙ منطقي is_rng_استعمال_ٿيل;
#جيڪڏهن_وصف_ٿيل OPENCV_TRACE
        CV_TRACE_NS::details::Region* trace_پاڙ_Region;
        CV_TRACE_NS::details::TraceManagerThreadLocal* trace_پاڙ_Context;
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل ENABLE_INSTRUMENTATION
        cv::instr::Instr_ڳنڍ *p_Thread_پاڙ;
#جيڪڏهن_پورو
        منطقي hasException;
#إذا CV__EXCEPTION_PTR
        std::exception_ptr pException;
#وإلا
        cv::String exception_پيغام;
#جيڪڏهن_پورو
#إذا CV__EXCEPTION_PTR
        فراغ رڪارڊ_Exception()
#وإلا
        فراغ رڪارڊ_Exception(ثابت cv::String& نياپو)
#جيڪڏهن_پورو
        {
#جيڪڏهن_وصف_نه_ٿيل CV_THREAD_SANITIZER
            إذا (!hasException)
#جيڪڏهن_پورو
            {
                cv::Auto_تالو lock(cv::getInitializationMutex());
                إذا (!hasException)
                {
                    hasException = سچ;
#إذا CV__EXCEPTION_PTR
                    pException = std::current_exception();
#وإلا
                    exception_پيغام = نياپو;
#جيڪڏهن_پورو
                }
            }
        }

#إذا OPENCV_SUPPORTS_FP_DENORMALS_HINT && OPENCV_IMPL_FP_HINTS
        details::FPDenormals_گهڻائي_State fp_denormals_بنياد_state;
#جيڪڏهن_پورو

    خاص:
        ParallelLoopBodyWrapperContext(ثابت ParallelLoopBodyWrapperContext&); // disabled
        ParallelLoopBodyWrapperContext& عامل=(ثابت ParallelLoopBodyWrapperContext&); // disabled
    };

    صنف ParallelLoopBodyWrapper : عام cv::ParallelLoopBody
    {
    عام:
        ParallelLoopBodyWrapper(ParallelLoopBodyWrapperContext& ctx_) :
            ctx(ctx_)
        {
        }
        ~ParallelLoopBodyWrapper()
        {
        }
        فراغ عامل()(ثابت cv::Range& sr) ثابت CV_OVERRIDE
        {
#جيڪڏهن_وصف_ٿيل OPENCV_TRACE
            // TODO CV_TRACE_NS::details::setCurrentRegion(rootRegion);
            إذا (ctx.trace_پاڙ_Region && ctx.trace_پاڙ_Context)
                CV_TRACE_NS::details::parallel_For_سيٽ_پاڙ_Region(*ctx.trace_پاڙ_Region, *ctx.trace_پاڙ_Context);
            CV__TRACE_OPENCV_FUNCTION_NAME("parallel_for_body");
            إذا (ctx.trace_پاڙ_Region)
                CV_TRACE_NS::details::parallelForAttachNestedRegion(*ctx.trace_پاڙ_Region);
#جيڪڏهن_پورو

#جيڪڏهن_وصف_ٿيل ENABLE_INSTRUMENTATION
            {
                cv::instr::InstrTLSStruct *pInstrTLS = &cv::instr::getInstrumentTLSStruct();
                pInstrTLS->p_Current_ڳنڍ = ctx.p_Thread_پاڙ; // Initialize TLS node for thread
            }
            CV_INSTRUMENT_REGION();
#جيڪڏهن_پورو

            // propagate main thread state
            cv::theRNG() = ctx.rng;
#إذا OPENCV_SUPPORTS_FP_DENORMALS_HINT && OPENCV_IMPL_FP_HINTS
            FPDenormalsIgnoreHintScope fp_denormals_scope(ctx.fp_denormals_بنياد_state);
#جيڪڏهن_پورو

            cv::Range r;
            cv::Range سڄو_Range = ctx.سڄو_Range;
            صحيح nstripes = ctx.nstripes;
            r.شروعات = (صحيح)(سڄو_Range.شروعات +
                            ((uint64)sr.شروعات*(سڄو_Range.end - سڄو_Range.شروعات) + nstripes/2)/nstripes);
            r.end = sr.end >= nstripes ? سڄو_Range.end : (صحيح)(سڄو_Range.شروعات +
                            ((uint64)sr.end*(سڄو_Range.end - سڄو_Range.شروعات) + nstripes/2)/nstripes);

#جيڪڏهن_وصف_ٿيل OPENCV_TRACE
            CV_TRACE_ARG_قدر(range_شروعات, "range.start", (int64)r.شروعات);
            CV_TRACE_ARG_قدر(range_پڇاڙي, "range.end", (int64)r.end);
#جيڪڏهن_پورو

            حاول
            {
                (*ctx.body)(r);
            }
#إذا CV__EXCEPTION_PTR
            امسك (...)
            {
                ctx.رڪارڊ_Exception();
            }
#وإلا
            امسك (ثابت cv::Exception& e)
            {
                ctx.رڪارڊ_Exception(e.what());
            }
            امسك (ثابت std::exception& e)
            {
                ctx.رڪارڊ_Exception(e.what());
            }
            امسك (...)
            {
                ctx.رڪارڊ_Exception("Unknown exception");
            }
#جيڪڏهن_پورو

            إذا (!ctx.is_rng_استعمال_ٿيل && !(cv::theRNG() == ctx.rng))
                ctx.is_rng_استعمال_ٿيل = سچ;
        }
        cv::Range stripeRange() ثابت { أرجع cv::Range(0, ctx.nstripes); }

    محمي:
        ParallelLoopBodyWrapperContext& ctx;
    };

#إذا defined HAVE_TBB
    صنف ProxyLoopBody : عام ParallelLoopBodyWrapper
    {
    عام:
        ProxyLoopBody(ParallelLoopBodyWrapperContext& ctx_)
        : ParallelLoopBodyWrapper(ctx_)
        {}

        فراغ عامل ()(ثابت tbb::blocked_range<صحيح>& range) ثابت
        {
            هذا->ParallelLoopBodyWrapper::عامل()(cv::Range(range.begin(), range.end()));
        }

        فراغ عامل ()() ثابت  // run parallel job
        {
            cv::Range range = هذا->stripeRange();
            tbb::parallel_for(tbb::blocked_range<صحيح>(range.شروعات, range.end), *هذا);
        }
    };
#وإلاإذا defined HAVE_HPX
    صنف ProxyLoopBody : عام ParallelLoopBodyWrapper
    {
    عام:
        ProxyLoopBody(ParallelLoopBodyWrapperContext& ctx_)
                : ParallelLoopBodyWrapper(ctx_)
        {}

        فراغ عامل ()() ثابت  // run parallel job
        {
            cv::Range stripeRange = هذا->stripeRange();
            hpx::parallel::for_loop(
                    hpx::parallel::execution::par,
                    stripeRange.شروعات, stripeRange.end,
                    [&](ثابت صحيح &i) { ;
                        هذا->ParallelLoopBodyWrapper::عامل()(
                                cv::Range(i, i + 1));
                    });
        }
    };
#وإلاإذا defined HAVE_OPENMP
    تعريفنوع ParallelLoopBodyWrapper ProxyLoopBody;
#وإلاإذا defined HAVE_وڏو_گڏيل_ونڊيندڙ
    تعريفنوع ParallelLoopBodyWrapper ProxyLoopBody;
    ساكن فراغ بلاڪ_function(فراغ* context, size_t اشاريو)
    {
        ProxyLoopBody* ptr_body = static_cast<ProxyLoopBody*>(context);
        (*ptr_body)(cv::Range((صحيح)اشاريو, (صحيح)اشاريو + 1));
    }
#وإلاإذا defined WINRT || defined HAVE_CONCURRENCY
    صنف ProxyLoopBody : عام ParallelLoopBodyWrapper
    {
    عام:
        ProxyLoopBody(ParallelLoopBodyWrapperContext& ctx)
        : ParallelLoopBodyWrapper(ctx)
        {}

        فراغ عامل ()(صحيح i) ثابت
        {
            هذا->ParallelLoopBodyWrapper::عامل()(cv::Range(i, i + 1));
        }
    };

    // PPL's default auto_partitioner silently skips whole chunks of the range on
    // ARM64 (unfixed MSVC bug, developercommunity.visualstudio.com/t/1027444).
    سانچو <قسم_نالو Body>
    ساكن مضمن فراغ pplParallelFor(صحيح first, صحيح آخري, ثابت Body& body)
    {
#إذا defined(_M_ARM64) || defined(_M_ARM64EC)
        Concurrency::parallel_for(first, آخري, body, Concurrency::static_partitioner());
#وإلا
        Concurrency::parallel_for(first, آخري, body);
#جيڪڏهن_پورو
    }
#وإلا
    تعريفنوع ParallelLoopBodyWrapper ProxyLoopBody;
#جيڪڏهن_پورو

#إذا defined HAVE_TBB
    #إذا TBB_INTERFACE_نسخو >= 8000
        ساكن tbb::task_arena tbbArena(tbb::task_arena::automatic);
    #وإلا
        ساكن tbb::task_scheduler_init tbbScheduler(tbb::task_scheduler_init::deferred);
    #جيڪڏهن_پورو
#وإلاإذا defined HAVE_HPX
// nothing for HPX
#وإلاإذا defined HAVE_OPENMP
ساكن مضمن صحيح init_Max_ڌاڳا()
{
    صحيح max_ڌاڳا = omp_get_max_ڌاڳا();
    أرجع max_ڌاڳا;
}
ساكن صحيح انگ_ڌاڳا_Max = init_Max_ڌاڳا();
#وإلاإذا defined HAVE_وڏو_گڏيل_ونڊيندڙ
// nothing for GCD
#وإلاإذا defined WINRT
// nothing for WINRT
#وإلاإذا defined HAVE_CONCURRENCY

صنف SchedPtr
{
    Concurrency::Scheduler* sched_;
عام:
    Concurrency::Scheduler* عامل->() { أرجع sched_; }
    عامل Concurrency::Scheduler*() { أرجع sched_; }

    فراغ عامل=(Concurrency::Scheduler* sched)
    {
        إذا (sched_) sched_->Release();
        sched_ = sched;
    }

    SchedPtr() : sched_(0) {}
    ~SchedPtr() {}
};
ساكن SchedPtr pplScheduler;

#جيڪڏهن_پورو

} // namespace anon

/* ================================   parallel_for_  ================================ */

ساكن فراغ parallel_for_impl(ثابت cv::Range& range, ثابت cv::ParallelLoopBody& body, مزدوج nstripes); // forward declaration

فراغ parallel_for_(ثابت cv::Range& range, ثابت cv::ParallelLoopBody& body, مزدوج nstripes)
{
#جيڪڏهن_وصف_ٿيل OPENCV_TRACE
    CV__TRACE_OPENCV_FUNCTION_NAME_("parallel_for", 0);
    CV_TRACE_ARG_قدر(range_شروعات, "range.start", (int64)range.شروعات);
    CV_TRACE_ARG_قدر(range_پڇاڙي, "range.end", (int64)range.end);
    CV_TRACE_ARG_قدر(nstripes, "nstripes", (int64)nstripes);
#جيڪڏهن_پورو

    CV_INSTRUMENT_REGION_MT_FORK();
    إذا (range.empty())
        أرجع;

    ساكن std::atomic<منطقي> flagNestedParallelFor(خطأ);
    منطقي isNotNestedRegion = !flagNestedParallelFor.load();
    إذا (isNotNestedRegion)
      isNotNestedRegion = !flagNestedParallelFor.exchange(سچ);
    إذا (isNotNestedRegion)
    {
        حاول
        {
            parallel_for_impl(range, body, nstripes);
            flagNestedParallelFor = خطأ;
        }
        امسك (...)
        {
            flagNestedParallelFor = خطأ;
            ارم;
        }
    }
    وإلا // nested parallel_for_() calls are not parallelized
    {
        CV_اڻ_استعمال_ٿيل(nstripes);
        body(range);
    }
}

ساكن
فراغ parallel_for_cb(صحيح شروعات, صحيح end, فراغ* معلومات)
{
    CV_DbgAssert(معلومات);
    ثابت cv::ParallelLoopBody& body = *(ثابت cv::ParallelLoopBody*)معلومات;
    body(Range(شروعات, end));
}

ساكن فراغ parallel_for_impl(ثابت cv::Range& range, ثابت cv::ParallelLoopBody& body, مزدوج nstripes)
{
    استعمال نطاق cv::parallel;
    إذا ((انگ_ڌاڳا < 0 || انگ_ڌاڳا > 1) && range.end - range.شروعات > 1)
    {
        ParallelLoopBodyWrapperContext ctx(body, range, nstripes);
        ProxyLoopBody pbody(ctx);
        cv::Range stripeRange = pbody.stripeRange();
        إذا( stripeRange.end - stripeRange.شروعات == 1 )
        {
            body(range);
            أرجع;
        }

        std::shared_ptr<ParallelForAPI>& api = getCurrentParallelForAPI();
        إذا (api)
        {
            CV_CheckEQ(stripeRange.شروعات, 0, "");
            api->parallel_for(stripeRange.end, parallel_for_cb, (فراغ*)&pbody);
            ctx.finalize();  // propagate exceptions if exists
            أرجع;
        }

#جيڪڏهن_وصف_ٿيل CV_PARALLEL_FRAMEWORK
#إذا defined HAVE_TBB

#إذا TBB_INTERFACE_نسخو >= 8000
        tbbArena.execute(pbody);
#وإلا
        pbody();
#جيڪڏهن_پورو

#وإلاإذا defined HAVE_HPX
        pbody();

#وإلاإذا defined HAVE_OPENMP

        #هدايت omp parallel لكل وقت_جدول(dynamic) انگ_ڌاڳا_2(انگ_ڌاڳا > 0 ? انگ_ڌاڳا : انگ_ڌاڳا_Max)
        لكل (صحيح i = stripeRange.شروعات; i < stripeRange.end; ++i)
            pbody(Range(i, i + 1));

#وإلاإذا defined HAVE_وڏو_گڏيل_ونڊيندڙ

        dispatch_قطار_t concurrent_قطار = dispatch_get_global_قطار(DISPATCH_قطار_ترجيح_DEFAULT, 0);
        dispatch_apply_f(stripeRange.end - stripeRange.شروعات, concurrent_قطار, &pbody, بلاڪ_function);

#وإلاإذا defined WINRT

        pplParallelFor(stripeRange.شروعات, stripeRange.end, pbody);

#وإلاإذا defined HAVE_CONCURRENCY

        إذا(!pplScheduler || pplScheduler->Id() == Concurrency::CurrentScheduler::Id())
        {
            pplParallelFor(stripeRange.شروعات, stripeRange.end, pbody);
        }
        وإلا
        {
            pplScheduler->Attach();
            pplParallelFor(stripeRange.شروعات, stripeRange.end, pbody);
            Concurrency::CurrentScheduler::Detach();
        }

#وإلاإذا defined HAVE_PTHREADS_PF

        parallel_for_pthreads(pbody.stripeRange(), pbody, pbody.stripeRange().size());

#وإلا

#غلطي You have hacked and compiling with unsupported parallel framework

#جيڪڏهن_پورو

        ctx.finalize();  // propagate exceptions if exists
        أرجع;
#جيڪڏهن_پورو // CV_PARALLEL_FRAMEWORK
    }

    body(range);
}


صحيح get_انگ_ڌاڳا(فراغ)
{
    std::shared_ptr<ParallelForAPI>& api = getCurrentParallelForAPI();
    إذا (api)
    {
        أرجع api->get_انگ_ڌاڳا();
    }

    إذا (انگ_ڌاڳا == 0)
        أرجع 1;

#إذا defined HAVE_TBB

#إذا TBB_INTERFACE_نسخو >= 9100
    أرجع tbbArena.max_concurrency();
#وإلاإذا TBB_INTERFACE_نسخو >= 8000
    أرجع انگ_ڌاڳا > 0
        ? انگ_ڌاڳا
        : tbb::task_scheduler_init::default_انگ_ڌاڳا();
#وإلا
    أرجع tbbScheduler.is_سرگرم()
           ? انگ_ڌاڳا
           : tbb::task_scheduler_init::default_انگ_ڌاڳا();
#جيڪڏهن_پورو

#وإلاإذا defined HAVE_HPX
    أرجع انگ_ڌاڳا;

#وإلاإذا defined HAVE_OPENMP

    أرجع انگ_ڌاڳا > 0
           ? انگ_ڌاڳا
           : انگ_ڌاڳا_Max;


#وإلاإذا defined HAVE_وڏو_گڏيل_ونڊيندڙ

    أرجع cv::get_عدد_جو_CPUs(); // the GCD thread pool limit

#وإلاإذا defined WINRT

    أرجع 0;

#وإلاإذا defined HAVE_CONCURRENCY

    أرجع (pplScheduler == 0)
        ? Concurrency::CurrentScheduler::Get()->Get_عدد_جو_Virtual_Processors()
        : (1 + pplScheduler->Get_عدد_جو_Virtual_Processors());

#وإلاإذا defined HAVE_PTHREADS_PF

        أرجع parallel_pthreads_get_ڌاڳا_انگ();

#وإلا

    أرجع 1;

#جيڪڏهن_پورو
}

بنانشان default_عدد_جو_ڌاڳا()
{
#جيڪڏهن_وصف_ٿيل __ANDROID__
    // many modern phones/tables have 4-core CPUs. Let's use no more
    // than 2 threads by default not to overheat the devices
    ثابت بنانشان صحيح default_عدد_جو_ڌاڳا_2 = 2;
#وإلا
    ثابت بنانشان صحيح default_عدد_جو_ڌاڳا_2 = (بنانشان صحيح)std::max(1, cv::get_عدد_جو_CPUs());
#جيڪڏهن_پورو

    بنانشان نتيجو = default_عدد_جو_ڌاڳا_2;

    ساكن صحيح config_انگ_ڌاڳا = (صحيح)utils::get_Configuration_Parameter_سائيز_T("OPENCV_FOR_THREADS_NUM", 0);

    إذا (config_انگ_ڌاڳا)
    {
        نتيجو = (بنانشان)std::max(1, config_انگ_ڌاڳا);
        //do we need upper limit of threads number?
    }
    أرجع نتيجو;
}

فراغ سيٽ_انگ_ڌاڳا( صحيح ڌاڳا_2 )
{
    CV_اڻ_استعمال_ٿيل(ڌاڳا_2);

    صحيح ڌاڳا = (ڌاڳا_2 < 0) ? default_عدد_جو_ڌاڳا() : (بنانشان)ڌاڳا_2;
    انگ_ڌاڳا = ڌاڳا;

    std::shared_ptr<ParallelForAPI>& api = getCurrentParallelForAPI();
    إذا (api)
    {
        api->سيٽ_انگ_ڌاڳا(انگ_ڌاڳا);
    }

#جيڪڏهن_وصف_ٿيل HAVE_TBB

#إذا TBB_INTERFACE_نسخو >= 8000
    إذا(tbbArena.is_سرگرم()) tbbArena.terminate();
    إذا(ڌاڳا > 0) tbbArena.initialize(ڌاڳا);
#وإلا
    إذا(tbbScheduler.is_سرگرم()) tbbScheduler.terminate();
    إذا(ڌاڳا > 0) tbbScheduler.initialize(ڌاڳا);
#جيڪڏهن_پورو

#وإلاإذا defined HAVE_HPX
    أرجع; // nothing needed as numThreads is used

#وإلاإذا defined HAVE_OPENMP

    أرجع; // nothing needed as num_threads clause is used in #pragma omp parallel for

#وإلاإذا defined HAVE_وڏو_گڏيل_ونڊيندڙ

    // unsupported
    // there is only private dispatch_queue_set_width() and only for desktop

#وإلاإذا defined WINRT

    أرجع;

#وإلاإذا defined HAVE_CONCURRENCY

    إذا (ڌاڳا <= 0)
    {
        pplScheduler = 0;
    }
    وإلا إذا (ڌاڳا == 1)
    {
        // Concurrency always uses >=2 threads, so we just disable it if 1 thread is requested
        انگ_ڌاڳا = 0;
    }
    وإلا إذا (pplScheduler == 0 || 1 + pplScheduler->Get_عدد_جو_Virtual_Processors() != (بنانشان صحيح)ڌاڳا)
    {
        pplScheduler = Concurrency::Scheduler::Create(Concurrency::SchedulerPolicy(2,
                       Concurrency::MinConcurrency, ڌاڳا-1,
                       Concurrency::MaxConcurrency, ڌاڳا-1));
    }

#وإلاإذا defined HAVE_PTHREADS_PF

    parallel_pthreads_سيٽ_ڌاڳا_انگ(ڌاڳا);

#جيڪڏهن_پورو
}


صحيح get_Thread_انگ()
{
    std::shared_ptr<ParallelForAPI>& api = getCurrentParallelForAPI();
    إذا (api)
    {
        أرجع api->get_Thread_انگ();
    }

#إذا defined HAVE_TBB
    #إذا TBB_INTERFACE_نسخو >= 9100
        أرجع tbb::this_task_arena::current_thread_اشاريو();
    #وإلاإذا TBB_INTERFACE_نسخو >= 8000
        أرجع tbb::task_arena::current_thread_اشاريو();
    #وإلا
        أرجع 0;
    #جيڪڏهن_پورو
#وإلاإذا defined HAVE_HPX
    أرجع (صحيح)(hpx::get_انگ_ڪم_ڪندڙ_ڌاڳا());
#وإلاإذا defined HAVE_OPENMP
    أرجع omp_get_thread_انگ();
#وإلاإذا defined HAVE_وڏو_گڏيل_ونڊيندڙ
    أرجع (صحيح)(size_t)(فراغ*)pthread_self(); // no zero-based indexing
#وإلاإذا defined WINRT
    أرجع 0;
#وإلاإذا defined HAVE_CONCURRENCY
    أرجع std::max(0, (صحيح)Concurrency::Context::VirtualProcessorId()); // zero for master thread, unique number for others but not necessary 1,2,3,...
#وإلاإذا defined HAVE_PTHREADS_PF
    أرجع (صحيح)(size_t)(فراغ*)pthread_self(); // no zero-based indexing
#وإلا
    أرجع 0;
#جيڪڏهن_پورو
}


#إذا defined __linux__ || defined __GLIBC__ || defined __HAIKU__ || defined __ANDROID__
  #وصف_ڪريو CV_CPU_GROUPS_1
#جيڪڏهن_پورو

#إذا defined __linux__ || defined __ANDROID__
  #وصف_ڪريو CV_HAVE_CGROUPS 1
#جيڪڏهن_پورو

#إذا defined CV_CPU_GROUPS_1
ساكن مضمن
std::string getFileContents(ثابت حرف *filename)
{
    std::ifstream ifs(filename);
    إذا (!ifs.is_open())
        أرجع std::string();

    std::string content( (std::istreambuf_iterator<حرف>(ifs) ),
                         (std::istreambuf_iterator<حرف>()    ) );

    إذا (ifs.fail())
        أرجع std::string();

    أرجع content;
}

ساكن مضمن
صحيح get_عدد_جو_CPUs_Impl(ثابت حرف *filename)
{
   std::string file_contents = getFileContents(filename);
   إذا(file_contents.empty())
       أرجع 0;

   حرف *pbuf = const_cast<حرف*>(file_contents.c_str());
   //parse string of form "0-1,3,5-7,10,13-15"
   صحيح cpus_موجود = 0;

   طالما(*pbuf)
   {
      ثابت حرف* pos = pbuf;
      منطقي range = خطأ;
      طالما(*pbuf && *pbuf != ',')
      {
          إذا(*pbuf == '-') range = سچ;
          ++pbuf;
      }
      إذا(*pbuf) *pbuf++ = 0;
      إذا(!range)
        ++cpus_موجود;
      وإلا
      {
          صحيح rstart = 0, rend = 0;
          لڙي_مان_پڙهو(pos, "%d-%d", &rstart, &rend);
          cpus_موجود += rend - rstart + 1;
      }

   }
   أرجع cpus_موجود;
}
#جيڪڏهن_پورو

#إذا defined CV_HAVE_CGROUPS
ساكن مضمن
بنانشان get_عدد_جو_CPUs_CFSv2()
{
    صحيح cfs_quota = 0;
    صحيح cfs_period = 0;

    std::ifstream ss_cpu_max("/sys/fs/cgroup/cpu.max", std::ios::in | std::ios::binary);
    ss_cpu_max >> cfs_quota >> cfs_period;

    إذا (ss_cpu_max.fail() || cfs_quota < 1 || cfs_period < 1) /* values must not be 0 or negative */
        أرجع 0;

    أرجع (بنانشان)max(1, cfs_quota/cfs_period);
}

ساكن مضمن
بنانشان get_عدد_جو_CPUs_CFSv1()
{
    صحيح cfs_quota = 0;
    {
        std::ifstream ss_period("/sys/fs/cgroup/cpu/cpu.cfs_quota_us", std::ios::in | std::ios::binary);
        ss_period >> cfs_quota;

        إذا (ss_period.fail() || cfs_quota < 1) /* cfs_quota must not be 0 or negative */
            أرجع 0;
    }

    صحيح cfs_period = 0;
    {
        std::ifstream ss_quota("/sys/fs/cgroup/cpu/cpu.cfs_period_us", std::ios::in | std::ios::binary);
        ss_quota >> cfs_period;

        إذا (ss_quota.fail() || cfs_period < 1)
            أرجع 0;
    }

    أرجع (بنانشان)max(1, cfs_quota/cfs_period);
}
#جيڪڏهن_پورو

سانچو <قسم_نالو T> ساكن مضمن
T minNonZero(ثابت T& val_1, ثابت T& val_2)
{
    إذا ((val_1 != 0) && (val_2 != 0))
        أرجع std::min(val_1, val_2);
    أرجع (val_1 != 0) ? val_1 : val_2;
}

#جيڪڏهن_وصف_نه_ٿيل OPENCV_DISABLE_THREAD_SUPPORT
ساكن
صحيح get_عدد_جو_CPUs_2()
{
#جيڪڏهن_وصف_نه_ٿيل OPENCV_SEMIHOSTING
    /*
     * Logic here is to try different methods of getting CPU counts and return
     * the minimum most value as it has high probablity of being right and safe.
     * Return 1 if we get 0 or not found on all methods.
    */
#إذا !defined(__MINGW32__) /* not implemented (2020-03) */

    /*
     * Check for this standard C++11 way, we do not return directly because
     * running in a docker or K8s environment will mean this is the host
     * machines config not the containers or pods and as per docs this value
     * must be "considered only a hint".
    */
    بنانشان ncpus = std::thread::hardware_concurrency(); /* If the value is not well defined or not computable, returns 0 */
#وإلا
    بنانشان ncpus = 0; /* 0 means we have to find out some other way */
#جيڪڏهن_پورو

#إذا defined _WIN32

    SYSTEM_INFO sysinfo = {};
#إذا (defined(_M_ARM) || defined(_M_ARM64) || defined(_M_ARM64EC) || defined(_M_X64) || defined(WINRT)) && _WIN32_WINNT >= 0x501
    GetNativeSystemInfo( &sysinfo );
#وإلا
    GetSystemInfo( &sysinfo );
#جيڪڏهن_پورو
    بنانشان ncpus_sysinfo = sysinfo.dw_عدد_جو_Processors;
    ncpus = minNonZero(ncpus, ncpus_sysinfo);

#وإلاإذا defined __APPLE__

    صحيح انگ_CPU=0;
    صحيح mib[4];
    size_t len = حجم(انگ_CPU);

    /* set the mib for hw.ncpu */
    mib[0] = CTL_HW;
    mib[1] = HW_AVAILCPU;  // alternatively, try HW_NCPU;

    /* get the number of CPUs from the system */
    sysctl(mib, 2, &انگ_CPU, &len, عدم, 0);

    إذا( انگ_CPU < 1 )
    {
        mib[1] = HW_NCPU;
        sysctl( mib, 2, &انگ_CPU, &len, عدم, 0 );

        إذا( انگ_CPU < 1 )
            انگ_CPU = 1;
    }

    ncpus = minNonZero(ncpus, (بنانشان)انگ_CPU);

#وإلاإذا defined CV_CPU_GROUPS_1

#إذا defined CV_HAVE_CGROUPS
    ساكن بنانشان ncpus_impl_cpuset = (بنانشان)get_عدد_جو_CPUs_Impl("/sys/fs/cgroup/cpuset/cpuset.cpus");
    ncpus = minNonZero(ncpus, ncpus_impl_cpuset);

    ساكن بنانشان ncpus_impl_cfs_v1 = get_عدد_جو_CPUs_CFSv1();
    ncpus = minNonZero(ncpus, ncpus_impl_cfs_v1);

    ساكن بنانشان ncpus_impl_cfs_v2 = get_عدد_جو_CPUs_CFSv2();
    ncpus = minNonZero(ncpus, ncpus_impl_cfs_v2);
#جيڪڏهن_پورو

    ساكن بنانشان ncpus_impl_devices = (بنانشان)get_عدد_جو_CPUs_Impl("/sys/devices/system/cpu/online");
    ncpus = minNonZero(ncpus, ncpus_impl_devices);

#جيڪڏهن_پورو

#إذا defined _GNU_SOURCE \
    && !defined(__MINGW32__) /* not implemented (2020-03) */ \
    && !defined(__EMSCRIPTEN__) \
    && !defined(__ANDROID__)  // TODO: add check for modern Android NDK

    cpu_set_t cpu_سيٽ;
    إذا (0 == sched_getaffinity(0, حجم(cpu_سيٽ), &cpu_سيٽ))
    {
        بنانشان cpu_ڳڻپ_cpu_سيٽ = CPU_COUNT(&cpu_سيٽ);
        ncpus = minNonZero(ncpus, cpu_ڳڻپ_cpu_سيٽ);
    }

#جيڪڏهن_پورو

#إذا !defined(_WIN32) && !defined(__APPLE__) && defined(_SC_NPROCESSORS_ONLN)

    ساكن بنانشان cpu_ڳڻپ_sysconf = (بنانشان)sysconf( _SC_NPROCESSORS_ONLN );
    ncpus = minNonZero(ncpus, cpu_ڳڻپ_sysconf);
#وإلاإذا defined (__QNX__)
    ساكن بنانشان cpu_ڳڻپ_sysconf = _syspage_ptr->انگ_cpu;
    ncpus = minNonZero(ncpus, cpu_ڳڻپ_sysconf);
#جيڪڏهن_پورو

    أرجع ncpus != 0 ? ncpus : 1;
#وإلا //  OPENCV_SEMIHOSTING
    أرجع 1;
#جيڪڏهن_پورو //OPENCV_SEMIHOSTING
}

صحيح get_عدد_جو_CPUs()
{
    ساكن صحيح nCPUs = get_عدد_جو_CPUs_2();
    أرجع nCPUs;  // cached value
}

#وإلا  // OPENCV_DISABLE_THREAD_SUPPORT
صحيح get_عدد_جو_CPUs()
{
    أرجع 1;
}
#جيڪڏهن_پورو  // OPENCV_DISABLE_THREAD_SUPPORT

ثابت حرف* currentParallelFramework()
{
    std::shared_ptr<ParallelForAPI>& api = getCurrentParallelForAPI();
    إذا (api)
    {
        أرجع api->getName();
    }
#جيڪڏهن_وصف_ٿيل CV_PARALLEL_FRAMEWORK
    أرجع CV_PARALLEL_FRAMEWORK;
#وإلا
    أرجع عدم;
#جيڪڏهن_پورو
}

}  // namespace cv::
