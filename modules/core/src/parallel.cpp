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
    #تعریف_رد small
    #تعریف_رد min
    #تعریف_رد max
    #تعریف_رد مطلق_قدر
#جے_انت

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
    #جے_انت
#جے_انت

#إذا defined (__QNX__)
    #ضمن <sys/syspage.h>
#جے_انت

#جے_بنا_تعریف OPENCV_DISABLE_THREAD_SUPPORT
    #ضمن <thread>
#جے_انت

#جے_تعریف_شدہ _OPENMP
    #تعریف_کرو HAVE_OPENMP
#جے_انت

#جے_تعریف_شدہ __APPLE__
    #تعریف_کرو HAVE_عاد_اعظم
#جے_انت

#إذا defined _MSC_VER
    #تعریف_کرو HAVE_CONCURRENCY
#جے_انت

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
    #جے_بنا_تعریف TBB_SUPPRESS_DEPRECATED_MESSAGES  // supress warning
    #تعریف_کرو TBB_SUPPRESS_DEPRECATED_MESSAGES 1
    #جے_انت
    #ضمن "tbb/tbb.h"
    #ضمن "tbb/task.h"
    #إذا TBB_INTERFACE_نسخہ >= 8000
        #ضمن "tbb/task_arena.h"
    #جے_انت
    #تعریف_رد min
    #تعریف_رد max
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
#وإلاإذا defined HAVE_عاد_اعظم
    #ضمن <dispatch/dispatch.h>
    #ضمن <pthread.h>
#وإلاإذا defined HAVE_CONCURRENCY
    #ضمن <ppl.h>
#وإلاإذا defined HAVE_PTHREADS_PF
    #ضمن <pthread.h>
#جے_انت


#إذا defined HAVE_TBB
#  define CV_PARALLEL_FRAMEWORK "tbb"
#وإلاإذا defined HAVE_HPX
#  define CV_PARALLEL_FRAMEWORK "hpx"
#وإلاإذا defined HAVE_OPENMP
#  define CV_PARALLEL_FRAMEWORK "openmp"
#وإلاإذا defined HAVE_عاد_اعظم
#  define CV_PARALLEL_FRAMEWORK "gcd"
#وإلاإذا defined WINRT
#  define CV_PARALLEL_FRAMEWORK "winrt-concurrency"
#وإلاإذا defined HAVE_CONCURRENCY
#  define CV_PARALLEL_FRAMEWORK "ms-concurrency"
#وإلاإذا defined HAVE_PTHREADS_PF
#  define CV_PARALLEL_FRAMEWORK "pthreads"
#جے_انت

#ضمن <atomic>

#ضمن "parallel_impl.hpp"

#ضمن "opencv2/core/detail/exception_ptr.hpp"  // CV__EXCEPTION_PTR = 1 if std::exception_ptr is available

#ضمن <opencv2/core/utils/fp_control_utils.hpp>
#ضمن <opencv2/core/utils/fp_control.private.hpp>

ورتوں نطاق cv;

نطاق cv {

ParallelLoopBody::~ParallelLoopBody() {}

ورتوں نطاق cv::parallel;

نطاق {

#جے_تعریف_شدہ ENABLE_INSTRUMENTATION
    ساكن فراغ SyncNodes(cv::instr::Instr_گرہ *p_گرہ)
    {
        std::vector<cv::instr::گرہ_انکڑے_Tls*> انکڑے;
        p_گرہ->m_payload.m_tls.gather(انکڑے);

        uint64 ticksMax = 0;
        صحيح    دھاگے  = 0;
        لكل(size_t i = 0; i < انکڑے.size(); i++)
        {
            إذا(انکڑے[i] && انکڑے[i]->m_ticks_کل)
            {
                ticksMax = MAX(ticksMax, انکڑے[i]->m_ticks_کل);
                p_گرہ->m_payload.m_ticks_کل -= انکڑے[i]->m_ticks_کل;
                انکڑے[i]->m_ticks_کل = 0;
                دھاگے++;
            }
        }
        p_گرہ->m_payload.m_ticks_کل += ticksMax;
        p_گرہ->m_payload.m_دھاگے = MAX(p_گرہ->m_payload.m_دھاگے, دھاگے);

        لكل(size_t i = 0; i < p_گرہ->m_childs.size(); i++)
            SyncNodes(p_گرہ->m_childs[i]);
    }
#جے_انت

    صنف ParallelLoopBodyWrapperContext
    {
    عام:
        ParallelLoopBodyWrapperContext(ثابت cv::ParallelLoopBody& _body, ثابت cv::Range& _r, مزدوج _nstripes) :
            is_rng_ورتیا(خطأ), hasException(خطأ)
        {

            body = &_body;
            سارا_Range = _r;
            مزدوج len = سارا_Range.end - سارا_Range.شروع;
            nstripes = cv_گیڑا(_nstripes <= 0 ? len : MIN(MAX(_nstripes, 1.), len));

            // propagate main thread state
            rng = cv::theRNG();
#إذا OPENCV_SUPPORTS_FP_DENORMALS_HINT && OPENCV_IMPL_FP_HINTS
            details::saveFPDenormalsState(fp_denormals_ادھار_state);
#جے_انت

#جے_تعریف_شدہ OPENCV_TRACE
            trace_جڑ_Region = CV_TRACE_NS::details::getCurrentRegion();
            trace_جڑ_Context = CV_TRACE_NS::details::getTraceManager().tls.get();
#جے_انت

#جے_تعریف_شدہ ENABLE_INSTRUMENTATION
            p_Thread_جڑ = cv::instr::getInstrumentTLSStruct().p_Current_گرہ;
#جے_انت
        }
        فراغ finalize()
        {
#جے_تعریف_شدہ ENABLE_INSTRUMENTATION
            لكل(size_t i = 0; i < p_Thread_جڑ->m_childs.size(); i++)
                SyncNodes(p_Thread_جڑ->m_childs[i]);
#جے_انت
            إذا (is_rng_ورتیا)
            {
                // Some parallel backends execute nested jobs in the main thread,
                // so we need to restore initial RNG state here.
                cv::theRNG() = rng;
                // We can't properly update RNG state based on RNG usage in worker threads,
                // so lets just change main thread RNG state to the next value.
                // Note: this behaviour is not equal to single-threaded mode.
                cv::theRNG().next();
            }
#جے_تعریف_شدہ OPENCV_TRACE
            إذا (trace_جڑ_Region)
                CV_TRACE_NS::details::parallelForFinalize(*trace_جڑ_Region);
#جے_انت

            إذا (hasException)
            {
#إذا CV__EXCEPTION_PTR
                std::rethrow_exception(pException);
#وإلا
                CV_Error(Error::StsError, "Exception in parallel_for() body: " + exception_سنیہا);
#جے_انت
            }
        }
        ~ParallelLoopBodyWrapperContext() {}

        ثابت cv::ParallelLoopBody* body;
        cv::Range سارا_Range;
        صحيح nstripes;
        cv::RNG rng;
        بدلنجوگ منطقي is_rng_ورتیا;
#جے_تعریف_شدہ OPENCV_TRACE
        CV_TRACE_NS::details::Region* trace_جڑ_Region;
        CV_TRACE_NS::details::TraceManagerThreadLocal* trace_جڑ_Context;
#جے_انت
#جے_تعریف_شدہ ENABLE_INSTRUMENTATION
        cv::instr::Instr_گرہ *p_Thread_جڑ;
#جے_انت
        منطقي hasException;
#إذا CV__EXCEPTION_PTR
        std::exception_ptr pException;
#وإلا
        cv::String exception_سنیہا;
#جے_انت
#إذا CV__EXCEPTION_PTR
        فراغ ریکارڈ_Exception()
#وإلا
        فراغ ریکارڈ_Exception(ثابت cv::String& پیغام)
#جے_انت
        {
#جے_بنا_تعریف CV_THREAD_SANITIZER
            إذا (!hasException)
#جے_انت
            {
                cv::Auto_جندرا lock(cv::getInitializationMutex());
                إذا (!hasException)
                {
                    hasException = سچ;
#إذا CV__EXCEPTION_PTR
                    pException = std::current_exception();
#وإلا
                    exception_سنیہا = پیغام;
#جے_انت
                }
            }
        }

#إذا OPENCV_SUPPORTS_FP_DENORMALS_HINT && OPENCV_IMPL_FP_HINTS
        details::FPDenormals_منوال_State fp_denormals_ادھار_state;
#جے_انت

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
#جے_تعریف_شدہ OPENCV_TRACE
            // TODO CV_TRACE_NS::details::setCurrentRegion(rootRegion);
            إذا (ctx.trace_جڑ_Region && ctx.trace_جڑ_Context)
                CV_TRACE_NS::details::parallel_For_سیٹ_جڑ_Region(*ctx.trace_جڑ_Region, *ctx.trace_جڑ_Context);
            CV__TRACE_OPENCV_FUNCTION_NAME("parallel_for_body");
            إذا (ctx.trace_جڑ_Region)
                CV_TRACE_NS::details::parallelForAttachNestedRegion(*ctx.trace_جڑ_Region);
#جے_انت

#جے_تعریف_شدہ ENABLE_INSTRUMENTATION
            {
                cv::instr::InstrTLSStruct *pInstrTLS = &cv::instr::getInstrumentTLSStruct();
                pInstrTLS->p_Current_گرہ = ctx.p_Thread_جڑ; // Initialize TLS node for thread
            }
            CV_INSTRUMENT_REGION();
#جے_انت

            // propagate main thread state
            cv::theRNG() = ctx.rng;
#إذا OPENCV_SUPPORTS_FP_DENORMALS_HINT && OPENCV_IMPL_FP_HINTS
            FPDenormalsIgnoreHintScope fp_denormals_scope(ctx.fp_denormals_ادھار_state);
#جے_انت

            cv::Range r;
            cv::Range سارا_Range = ctx.سارا_Range;
            صحيح nstripes = ctx.nstripes;
            r.شروع = (صحيح)(سارا_Range.شروع +
                            ((uint64)sr.شروع*(سارا_Range.end - سارا_Range.شروع) + nstripes/2)/nstripes);
            r.end = sr.end >= nstripes ? سارا_Range.end : (صحيح)(سارا_Range.شروع +
                            ((uint64)sr.end*(سارا_Range.end - سارا_Range.شروع) + nstripes/2)/nstripes);

#جے_تعریف_شدہ OPENCV_TRACE
            CV_TRACE_ARG_قدر(range_شروع, "range.start", (int64)r.شروع);
            CV_TRACE_ARG_قدر(range_سرا, "range.end", (int64)r.end);
#جے_انت

            حاول
            {
                (*ctx.body)(r);
            }
#إذا CV__EXCEPTION_PTR
            امسك (...)
            {
                ctx.ریکارڈ_Exception();
            }
#وإلا
            امسك (ثابت cv::Exception& e)
            {
                ctx.ریکارڈ_Exception(e.what());
            }
            امسك (ثابت std::exception& e)
            {
                ctx.ریکارڈ_Exception(e.what());
            }
            امسك (...)
            {
                ctx.ریکارڈ_Exception("Unknown exception");
            }
#جے_انت

            إذا (!ctx.is_rng_ورتیا && !(cv::theRNG() == ctx.rng))
                ctx.is_rng_ورتیا = سچ;
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
            tbb::parallel_for(tbb::blocked_range<صحيح>(range.شروع, range.end), *هذا);
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
                    stripeRange.شروع, stripeRange.end,
                    [&](ثابت صحيح &i) { ;
                        هذا->ParallelLoopBodyWrapper::عامل()(
                                cv::Range(i, i + 1));
                    });
        }
    };
#وإلاإذا defined HAVE_OPENMP
    تعريفنوع ParallelLoopBodyWrapper ProxyLoopBody;
#وإلاإذا defined HAVE_عاد_اعظم
    تعريفنوع ParallelLoopBodyWrapper ProxyLoopBody;
    ساكن فراغ بلاک_function(فراغ* context, size_t اشاریہ)
    {
        ProxyLoopBody* ptr_body = static_cast<ProxyLoopBody*>(context);
        (*ptr_body)(cv::Range((صحيح)اشاریہ, (صحيح)اشاریہ + 1));
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
    سانچا <قسم_ناں Body>
    ساكن مضمن فراغ pplParallelFor(صحيح first, صحيح آخری, ثابت Body& body)
    {
#إذا defined(_M_ARM64) || defined(_M_ARM64EC)
        Concurrency::parallel_for(first, آخری, body, Concurrency::static_partitioner());
#وإلا
        Concurrency::parallel_for(first, آخری, body);
#جے_انت
    }
#وإلا
    تعريفنوع ParallelLoopBodyWrapper ProxyLoopBody;
#جے_انت

#إذا defined HAVE_TBB
    #إذا TBB_INTERFACE_نسخہ >= 8000
        ساكن tbb::task_arena tbbArena(tbb::task_arena::automatic);
    #وإلا
        ساكن tbb::task_scheduler_init tbbScheduler(tbb::task_scheduler_init::deferred);
    #جے_انت
#وإلاإذا defined HAVE_HPX
// nothing for HPX
#وإلاإذا defined HAVE_OPENMP
ساكن مضمن صحيح init_Max_دھاگے()
{
    صحيح max_دھاگے = omp_get_max_دھاگے();
    أرجع max_دھاگے;
}
ساكن صحيح عدد_دھاگے_Max = init_Max_دھاگے();
#وإلاإذا defined HAVE_عاد_اعظم
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

#جے_انت

} // namespace anon

/* ================================   parallel_for_  ================================ */

ساكن فراغ parallel_for_impl(ثابت cv::Range& range, ثابت cv::ParallelLoopBody& body, مزدوج nstripes); // forward declaration

فراغ parallel_for_(ثابت cv::Range& range, ثابت cv::ParallelLoopBody& body, مزدوج nstripes)
{
#جے_تعریف_شدہ OPENCV_TRACE
    CV__TRACE_OPENCV_FUNCTION_NAME_("parallel_for", 0);
    CV_TRACE_ARG_قدر(range_شروع, "range.start", (int64)range.شروع);
    CV_TRACE_ARG_قدر(range_سرا, "range.end", (int64)range.end);
    CV_TRACE_ARG_قدر(nstripes, "nstripes", (int64)nstripes);
#جے_انت

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
        CV_بن_ورتیا(nstripes);
        body(range);
    }
}

ساكن
فراغ parallel_for_cb(صحيح شروع, صحيح end, فراغ* انکڑے)
{
    CV_DbgAssert(انکڑے);
    ثابت cv::ParallelLoopBody& body = *(ثابت cv::ParallelLoopBody*)انکڑے;
    body(Range(شروع, end));
}

ساكن فراغ parallel_for_impl(ثابت cv::Range& range, ثابت cv::ParallelLoopBody& body, مزدوج nstripes)
{
    ورتوں نطاق cv::parallel;
    إذا ((عدد_دھاگے < 0 || عدد_دھاگے > 1) && range.end - range.شروع > 1)
    {
        ParallelLoopBodyWrapperContext ctx(body, range, nstripes);
        ProxyLoopBody pbody(ctx);
        cv::Range stripeRange = pbody.stripeRange();
        إذا( stripeRange.end - stripeRange.شروع == 1 )
        {
            body(range);
            أرجع;
        }

        std::shared_ptr<ParallelForAPI>& api = getCurrentParallelForAPI();
        إذا (api)
        {
            CV_CheckEQ(stripeRange.شروع, 0, "");
            api->parallel_for(stripeRange.end, parallel_for_cb, (فراغ*)&pbody);
            ctx.finalize();  // propagate exceptions if exists
            أرجع;
        }

#جے_تعریف_شدہ CV_PARALLEL_FRAMEWORK
#إذا defined HAVE_TBB

#إذا TBB_INTERFACE_نسخہ >= 8000
        tbbArena.execute(pbody);
#وإلا
        pbody();
#جے_انت

#وإلاإذا defined HAVE_HPX
        pbody();

#وإلاإذا defined HAVE_OPENMP

        #ہدایت omp parallel لكل ویلا_سارنی(dynamic) عدد_دھاگے_2(عدد_دھاگے > 0 ? عدد_دھاگے : عدد_دھاگے_Max)
        لكل (صحيح i = stripeRange.شروع; i < stripeRange.end; ++i)
            pbody(Range(i, i + 1));

#وإلاإذا defined HAVE_عاد_اعظم

        dispatch_قطار_t concurrent_قطار = dispatch_get_global_قطار(DISPATCH_قطار_ترجیح_DEFAULT, 0);
        dispatch_apply_f(stripeRange.end - stripeRange.شروع, concurrent_قطار, &pbody, بلاک_function);

#وإلاإذا defined WINRT

        pplParallelFor(stripeRange.شروع, stripeRange.end, pbody);

#وإلاإذا defined HAVE_CONCURRENCY

        إذا(!pplScheduler || pplScheduler->Id() == Concurrency::CurrentScheduler::Id())
        {
            pplParallelFor(stripeRange.شروع, stripeRange.end, pbody);
        }
        وإلا
        {
            pplScheduler->Attach();
            pplParallelFor(stripeRange.شروع, stripeRange.end, pbody);
            Concurrency::CurrentScheduler::Detach();
        }

#وإلاإذا defined HAVE_PTHREADS_PF

        parallel_for_pthreads(pbody.stripeRange(), pbody, pbody.stripeRange().size());

#وإلا

#غلطی You have hacked and compiling with unsupported parallel framework

#جے_انت

        ctx.finalize();  // propagate exceptions if exists
        أرجع;
#جے_انت // CV_PARALLEL_FRAMEWORK
    }

    body(range);
}


صحيح get_عدد_دھاگے(فراغ)
{
    std::shared_ptr<ParallelForAPI>& api = getCurrentParallelForAPI();
    إذا (api)
    {
        أرجع api->get_عدد_دھاگے();
    }

    إذا (عدد_دھاگے == 0)
        أرجع 1;

#إذا defined HAVE_TBB

#إذا TBB_INTERFACE_نسخہ >= 9100
    أرجع tbbArena.max_concurrency();
#وإلاإذا TBB_INTERFACE_نسخہ >= 8000
    أرجع عدد_دھاگے > 0
        ? عدد_دھاگے
        : tbb::task_scheduler_init::default_عدد_دھاگے();
#وإلا
    أرجع tbbScheduler.is_سرگرم()
           ? عدد_دھاگے
           : tbb::task_scheduler_init::default_عدد_دھاگے();
#جے_انت

#وإلاإذا defined HAVE_HPX
    أرجع عدد_دھاگے;

#وإلاإذا defined HAVE_OPENMP

    أرجع عدد_دھاگے > 0
           ? عدد_دھاگے
           : عدد_دھاگے_Max;


#وإلاإذا defined HAVE_عاد_اعظم

    أرجع cv::get_نمبر_دا_CPUs(); // the GCD thread pool limit

#وإلاإذا defined WINRT

    أرجع 0;

#وإلاإذا defined HAVE_CONCURRENCY

    أرجع (pplScheduler == 0)
        ? Concurrency::CurrentScheduler::Get()->Get_نمبر_دا_Virtual_Processors()
        : (1 + pplScheduler->Get_نمبر_دا_Virtual_Processors());

#وإلاإذا defined HAVE_PTHREADS_PF

        أرجع parallel_pthreads_get_دھاگے_عدد();

#وإلا

    أرجع 1;

#جے_انت
}

بنانشانی default_نمبر_دا_دھاگے()
{
#جے_تعریف_شدہ __ANDROID__
    // many modern phones/tables have 4-core CPUs. Let's use no more
    // than 2 threads by default not to overheat the devices
    ثابت بنانشانی صحيح default_نمبر_دا_دھاگے_2 = 2;
#وإلا
    ثابت بنانشانی صحيح default_نمبر_دا_دھاگے_2 = (بنانشانی صحيح)std::max(1, cv::get_نمبر_دا_CPUs());
#جے_انت

    بنانشانی نتیجہ = default_نمبر_دا_دھاگے_2;

    ساكن صحيح config_عدد_دھاگے = (صحيح)utils::get_Configuration_Parameter_ناپ_T("OPENCV_FOR_THREADS_NUM", 0);

    إذا (config_عدد_دھاگے)
    {
        نتیجہ = (بنانشانی)std::max(1, config_عدد_دھاگے);
        //do we need upper limit of threads number?
    }
    أرجع نتیجہ;
}

فراغ سیٹ_عدد_دھاگے( صحيح دھاگے_2 )
{
    CV_بن_ورتیا(دھاگے_2);

    صحيح دھاگے = (دھاگے_2 < 0) ? default_نمبر_دا_دھاگے() : (بنانشانی)دھاگے_2;
    عدد_دھاگے = دھاگے;

    std::shared_ptr<ParallelForAPI>& api = getCurrentParallelForAPI();
    إذا (api)
    {
        api->سیٹ_عدد_دھاگے(عدد_دھاگے);
    }

#جے_تعریف_شدہ HAVE_TBB

#إذا TBB_INTERFACE_نسخہ >= 8000
    إذا(tbbArena.is_سرگرم()) tbbArena.terminate();
    إذا(دھاگے > 0) tbbArena.initialize(دھاگے);
#وإلا
    إذا(tbbScheduler.is_سرگرم()) tbbScheduler.terminate();
    إذا(دھاگے > 0) tbbScheduler.initialize(دھاگے);
#جے_انت

#وإلاإذا defined HAVE_HPX
    أرجع; // nothing needed as numThreads is used

#وإلاإذا defined HAVE_OPENMP

    أرجع; // nothing needed as num_threads clause is used in #pragma omp parallel for

#وإلاإذا defined HAVE_عاد_اعظم

    // unsupported
    // there is only private dispatch_queue_set_width() and only for desktop

#وإلاإذا defined WINRT

    أرجع;

#وإلاإذا defined HAVE_CONCURRENCY

    إذا (دھاگے <= 0)
    {
        pplScheduler = 0;
    }
    وإلا إذا (دھاگے == 1)
    {
        // Concurrency always uses >=2 threads, so we just disable it if 1 thread is requested
        عدد_دھاگے = 0;
    }
    وإلا إذا (pplScheduler == 0 || 1 + pplScheduler->Get_نمبر_دا_Virtual_Processors() != (بنانشانی صحيح)دھاگے)
    {
        pplScheduler = Concurrency::Scheduler::Create(Concurrency::SchedulerPolicy(2,
                       Concurrency::MinConcurrency, دھاگے-1,
                       Concurrency::MaxConcurrency, دھاگے-1));
    }

#وإلاإذا defined HAVE_PTHREADS_PF

    parallel_pthreads_سیٹ_دھاگے_عدد(دھاگے);

#جے_انت
}


صحيح get_Thread_عدد()
{
    std::shared_ptr<ParallelForAPI>& api = getCurrentParallelForAPI();
    إذا (api)
    {
        أرجع api->get_Thread_عدد();
    }

#إذا defined HAVE_TBB
    #إذا TBB_INTERFACE_نسخہ >= 9100
        أرجع tbb::this_task_arena::current_thread_اشاریہ();
    #وإلاإذا TBB_INTERFACE_نسخہ >= 8000
        أرجع tbb::task_arena::current_thread_اشاریہ();
    #وإلا
        أرجع 0;
    #جے_انت
#وإلاإذا defined HAVE_HPX
    أرجع (صحيح)(hpx::get_عدد_کامہ_دھاگے());
#وإلاإذا defined HAVE_OPENMP
    أرجع omp_get_thread_عدد();
#وإلاإذا defined HAVE_عاد_اعظم
    أرجع (صحيح)(size_t)(فراغ*)pthread_self(); // no zero-based indexing
#وإلاإذا defined WINRT
    أرجع 0;
#وإلاإذا defined HAVE_CONCURRENCY
    أرجع std::max(0, (صحيح)Concurrency::Context::VirtualProcessorId()); // zero for master thread, unique number for others but not necessary 1,2,3,...
#وإلاإذا defined HAVE_PTHREADS_PF
    أرجع (صحيح)(size_t)(فراغ*)pthread_self(); // no zero-based indexing
#وإلا
    أرجع 0;
#جے_انت
}


#إذا defined __linux__ || defined __GLIBC__ || defined __HAIKU__ || defined __ANDROID__
  #تعریف_کرو CV_CPU_GROUPS_1
#جے_انت

#إذا defined __linux__ || defined __ANDROID__
  #تعریف_کرو CV_HAVE_CGROUPS 1
#جے_انت

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
صحيح get_نمبر_دا_CPUs_Impl(ثابت حرف *filename)
{
   std::string file_contents = getFileContents(filename);
   إذا(file_contents.empty())
       أرجع 0;

   حرف *pbuf = const_cast<حرف*>(file_contents.c_str());
   //parse string of form "0-1,3,5-7,10,13-15"
   صحيح cpus_دستیاب = 0;

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
        ++cpus_دستیاب;
      وإلا
      {
          صحيح rstart = 0, rend = 0;
          لڑی_توں_پڑھو(pos, "%d-%d", &rstart, &rend);
          cpus_دستیاب += rend - rstart + 1;
      }

   }
   أرجع cpus_دستیاب;
}
#جے_انت

#إذا defined CV_HAVE_CGROUPS
ساكن مضمن
بنانشانی get_نمبر_دا_CPUs_CFSv2()
{
    صحيح cfs_quota = 0;
    صحيح cfs_period = 0;

    std::ifstream ss_cpu_max("/sys/fs/cgroup/cpu.max", std::ios::in | std::ios::binary);
    ss_cpu_max >> cfs_quota >> cfs_period;

    إذا (ss_cpu_max.fail() || cfs_quota < 1 || cfs_period < 1) /* values must not be 0 or negative */
        أرجع 0;

    أرجع (بنانشانی)max(1, cfs_quota/cfs_period);
}

ساكن مضمن
بنانشانی get_نمبر_دا_CPUs_CFSv1()
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

    أرجع (بنانشانی)max(1, cfs_quota/cfs_period);
}
#جے_انت

سانچا <قسم_ناں T> ساكن مضمن
T minNonZero(ثابت T& val_1, ثابت T& val_2)
{
    إذا ((val_1 != 0) && (val_2 != 0))
        أرجع std::min(val_1, val_2);
    أرجع (val_1 != 0) ? val_1 : val_2;
}

#جے_بنا_تعریف OPENCV_DISABLE_THREAD_SUPPORT
ساكن
صحيح get_نمبر_دا_CPUs_2()
{
#جے_بنا_تعریف OPENCV_SEMIHOSTING
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
    بنانشانی ncpus = std::thread::hardware_concurrency(); /* If the value is not well defined or not computable, returns 0 */
#وإلا
    بنانشانی ncpus = 0; /* 0 means we have to find out some other way */
#جے_انت

#إذا defined _WIN32

    SYSTEM_INFO sysinfo = {};
#إذا (defined(_M_ARM) || defined(_M_ARM64) || defined(_M_ARM64EC) || defined(_M_X64) || defined(WINRT)) && _WIN32_WINNT >= 0x501
    GetNativeSystemInfo( &sysinfo );
#وإلا
    GetSystemInfo( &sysinfo );
#جے_انت
    بنانشانی ncpus_sysinfo = sysinfo.dw_نمبر_دا_Processors;
    ncpus = minNonZero(ncpus, ncpus_sysinfo);

#وإلاإذا defined __APPLE__

    صحيح عدد_CPU=0;
    صحيح mib[4];
    size_t len = حجم(عدد_CPU);

    /* set the mib for hw.ncpu */
    mib[0] = CTL_HW;
    mib[1] = HW_AVAILCPU;  // alternatively, try HW_NCPU;

    /* get the number of CPUs from the system */
    sysctl(mib, 2, &عدد_CPU, &len, عدم, 0);

    إذا( عدد_CPU < 1 )
    {
        mib[1] = HW_NCPU;
        sysctl( mib, 2, &عدد_CPU, &len, عدم, 0 );

        إذا( عدد_CPU < 1 )
            عدد_CPU = 1;
    }

    ncpus = minNonZero(ncpus, (بنانشانی)عدد_CPU);

#وإلاإذا defined CV_CPU_GROUPS_1

#إذا defined CV_HAVE_CGROUPS
    ساكن بنانشانی ncpus_impl_cpuset = (بنانشانی)get_نمبر_دا_CPUs_Impl("/sys/fs/cgroup/cpuset/cpuset.cpus");
    ncpus = minNonZero(ncpus, ncpus_impl_cpuset);

    ساكن بنانشانی ncpus_impl_cfs_v1 = get_نمبر_دا_CPUs_CFSv1();
    ncpus = minNonZero(ncpus, ncpus_impl_cfs_v1);

    ساكن بنانشانی ncpus_impl_cfs_v2 = get_نمبر_دا_CPUs_CFSv2();
    ncpus = minNonZero(ncpus, ncpus_impl_cfs_v2);
#جے_انت

    ساكن بنانشانی ncpus_impl_devices = (بنانشانی)get_نمبر_دا_CPUs_Impl("/sys/devices/system/cpu/online");
    ncpus = minNonZero(ncpus, ncpus_impl_devices);

#جے_انت

#إذا defined _GNU_SOURCE \
    && !defined(__MINGW32__) /* not implemented (2020-03) */ \
    && !defined(__EMSCRIPTEN__) \
    && !defined(__ANDROID__)  // TODO: add check for modern Android NDK

    cpu_set_t cpu_سیٹ;
    إذا (0 == sched_getaffinity(0, حجم(cpu_سیٹ), &cpu_سیٹ))
    {
        بنانشانی cpu_گنتی_cpu_سیٹ = CPU_COUNT(&cpu_سیٹ);
        ncpus = minNonZero(ncpus, cpu_گنتی_cpu_سیٹ);
    }

#جے_انت

#إذا !defined(_WIN32) && !defined(__APPLE__) && defined(_SC_NPROCESSORS_ONLN)

    ساكن بنانشانی cpu_گنتی_sysconf = (بنانشانی)sysconf( _SC_NPROCESSORS_ONLN );
    ncpus = minNonZero(ncpus, cpu_گنتی_sysconf);
#وإلاإذا defined (__QNX__)
    ساكن بنانشانی cpu_گنتی_sysconf = _syspage_ptr->عدد_cpu;
    ncpus = minNonZero(ncpus, cpu_گنتی_sysconf);
#جے_انت

    أرجع ncpus != 0 ? ncpus : 1;
#وإلا //  OPENCV_SEMIHOSTING
    أرجع 1;
#جے_انت //OPENCV_SEMIHOSTING
}

صحيح get_نمبر_دا_CPUs()
{
    ساكن صحيح nCPUs = get_نمبر_دا_CPUs_2();
    أرجع nCPUs;  // cached value
}

#وإلا  // OPENCV_DISABLE_THREAD_SUPPORT
صحيح get_نمبر_دا_CPUs()
{
    أرجع 1;
}
#جے_انت  // OPENCV_DISABLE_THREAD_SUPPORT

ثابت حرف* currentParallelFramework()
{
    std::shared_ptr<ParallelForAPI>& api = getCurrentParallelForAPI();
    إذا (api)
    {
        أرجع api->getName();
    }
#جے_تعریف_شدہ CV_PARALLEL_FRAMEWORK
    أرجع CV_PARALLEL_FRAMEWORK;
#وإلا
    أرجع عدم;
#جے_انت
}

}  // namespace cv::
