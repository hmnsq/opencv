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

#شامل "precomp.hpp"

#شامل <opencv2/core/utils/configuration.private.hpp>
#شامل <opencv2/core/utils/trace.private.hpp>

#شامل "opencv2/core/parallel/parallel_backend.hpp"
#شامل "parallel/parallel.hpp"

#اگر defined _WIN32 || defined WINCE
    #شامل <windows.h>
    #تعریف_منسوخ small
    #تعریف_منسوخ min
    #تعریف_منسوخ max
    #تعریف_منسوخ مطلق_قدر
#اگر_ختم

#اگر defined __unix__ || defined __APPLE__ || defined __GLIBC__ \
    || defined __HAIKU__ || defined __EMSCRIPTEN__ \
    || defined __FreeBSD__ || defined __NetBSD__ || defined __OpenBSD__
    #شامل <unistd.h>
    #شامل <stdio.h>
    #شامل <sys/types.h>
    #شامل <fstream>
    #اگر defined __ANDROID__
        #شامل <sys/sysconf.h>
        #شامل <sys/syscall.h>
        #شامل <sched.h>
    #وگرنہ_اگر defined __APPLE__
        #شامل <sys/sysctl.h>
    #اگر_ختم
#اگر_ختم

#اگر defined (__QNX__)
    #شامل <sys/syspage.h>
#اگر_ختم

#اگر_غیر_تعریف_شدہ OPENCV_DISABLE_THREAD_SUPPORT
    #شامل <thread>
#اگر_ختم

#اگر_تعریف_شدہ _OPENMP
    #تعریف_کرو HAVE_OPENMP
#اگر_ختم

#اگر_تعریف_شدہ __APPLE__
    #تعریف_کرو HAVE_عاد_اعظم
#اگر_ختم

#اگر defined _MSC_VER
    #تعریف_کرو HAVE_CONCURRENCY
#اگر_ختم

/* IMPORTANT: always use the same order of defines
   - HAVE_TBB         - 3rdparty library, should be explicitly enabled
   - HAVE_HPX         - 3rdparty library, should be explicitly enabled
   - HAVE_OPENMP      - integrated to compiler, should be explicitly enabled
   - HAVE_GCD         - system wide, used automatically        (APPLE only)
   - WINRT            - system wide, used automatically        (Windows RT only)
   - HAVE_CONCURRENCY - part of runtime, used automatically    (Windows only - MSVS 10, MSVS 11)
   - HAVE_PTHREADS_PF - pthreads if available
*/

#اگر defined HAVE_TBB
    #اگر_غیر_تعریف_شدہ TBB_SUPPRESS_DEPRECATED_MESSAGES  // supress warning
    #تعریف_کرو TBB_SUPPRESS_DEPRECATED_MESSAGES 1
    #اگر_ختم
    #شامل "tbb/tbb.h"
    #شامل "tbb/task.h"
    #اگر TBB_INTERFACE_نسخہ >= 8000
        #شامل "tbb/task_arena.h"
    #اگر_ختم
    #تعریف_منسوخ min
    #تعریف_منسوخ max
#وگرنہ_اگر defined HAVE_HPX
    #شامل <hpx/parallel/algorithms/for_loop.hpp>
    #شامل <hpx/parallel/execution.hpp>
    //
    #شامل <hpx/hpx_start.hpp>
    #شامل <hpx/hpx_suspend.hpp>
    #شامل <hpx/include/apply.hpp>
    #شامل <hpx/util/yield_while.hpp>
    #شامل <hpx/include/threadmanager.hpp>

#وگرنہ_اگر defined HAVE_OPENMP
    #شامل <omp.h>
#وگرنہ_اگر defined HAVE_عاد_اعظم
    #شامل <dispatch/dispatch.h>
    #شامل <pthread.h>
#وگرنہ_اگر defined HAVE_CONCURRENCY
    #شامل <ppl.h>
#وگرنہ_اگر defined HAVE_PTHREADS_PF
    #شامل <pthread.h>
#اگر_ختم


#اگر defined HAVE_TBB
#  define CV_PARALLEL_FRAMEWORK "tbb"
#وگرنہ_اگر defined HAVE_HPX
#  define CV_PARALLEL_FRAMEWORK "hpx"
#وگرنہ_اگر defined HAVE_OPENMP
#  define CV_PARALLEL_FRAMEWORK "openmp"
#وگرنہ_اگر defined HAVE_عاد_اعظم
#  define CV_PARALLEL_FRAMEWORK "gcd"
#وگرنہ_اگر defined WINRT
#  define CV_PARALLEL_FRAMEWORK "winrt-concurrency"
#وگرنہ_اگر defined HAVE_CONCURRENCY
#  define CV_PARALLEL_FRAMEWORK "ms-concurrency"
#وگرنہ_اگر defined HAVE_PTHREADS_PF
#  define CV_PARALLEL_FRAMEWORK "pthreads"
#اگر_ختم

#شامل <atomic>

#شامل "parallel_impl.hpp"

#شامل "opencv2/core/detail/exception_ptr.hpp"  // CV__EXCEPTION_PTR = 1 if std::exception_ptr is available

#شامل <opencv2/core/utils/fp_control_utils.hpp>
#شامل <opencv2/core/utils/fp_control.private.hpp>

استعمال نامخلا cv;

نامخلا cv {

ParallelLoopBody::~ParallelLoopBody() {}

استعمال نامخلا cv::parallel;

نامخلا {

#اگر_تعریف_شدہ ENABLE_INSTRUMENTATION
    ساکن خالی SyncNodes(cv::instr::Instr_گرہ *p_گرہ)
    {
        std::vector<cv::instr::گرہ_ڈیٹا_Tls*> ڈیٹا;
        p_گرہ->m_payload.m_tls.gather(ڈیٹا);

        uint64 ticksMax = 0;
        صحیح    دھاگے  = 0;
        برائے(size_t i = 0; i < ڈیٹا.size(); i++)
        {
            اگر(ڈیٹا[i] && ڈیٹا[i]->m_ticks_کل)
            {
                ticksMax = MAX(ticksMax, ڈیٹا[i]->m_ticks_کل);
                p_گرہ->m_payload.m_ticks_کل -= ڈیٹا[i]->m_ticks_کل;
                ڈیٹا[i]->m_ticks_کل = 0;
                دھاگے++;
            }
        }
        p_گرہ->m_payload.m_ticks_کل += ticksMax;
        p_گرہ->m_payload.m_دھاگے = MAX(p_گرہ->m_payload.m_دھاگے, دھاگے);

        برائے(size_t i = 0; i < p_گرہ->m_childs.size(); i++)
            SyncNodes(p_گرہ->m_childs[i]);
    }
#اگر_ختم

    جماعت ParallelLoopBodyWrapperContext
    {
    عوامی:
        ParallelLoopBodyWrapperContext(ثابت cv::ParallelLoopBody& _body, ثابت cv::Range& _r, دوہرا _nstripes) :
            is_rng_مستعمل(جھوٹ), hasException(جھوٹ)
        {

            body = &_body;
            پورا_Range = _r;
            دوہرا len = پورا_Range.end - پورا_Range.آغاز;
            nstripes = cv_باری(_nstripes <= 0 ? len : MIN(MAX(_nstripes, 1.), len));

            // propagate main thread state
            rng = cv::theRNG();
#اگر OPENCV_SUPPORTS_FP_DENORMALS_HINT && OPENCV_IMPL_FP_HINTS
            details::saveFPDenormalsState(fp_denormals_بنیاد_state);
#اگر_ختم

#اگر_تعریف_شدہ OPENCV_TRACE
            trace_جڑ_Region = CV_TRACE_NS::details::getCurrentRegion();
            trace_جڑ_Context = CV_TRACE_NS::details::getTraceManager().tls.get();
#اگر_ختم

#اگر_تعریف_شدہ ENABLE_INSTRUMENTATION
            p_Thread_جڑ = cv::instr::getInstrumentTLSStruct().p_Current_گرہ;
#اگر_ختم
        }
        خالی finalize()
        {
#اگر_تعریف_شدہ ENABLE_INSTRUMENTATION
            برائے(size_t i = 0; i < p_Thread_جڑ->m_childs.size(); i++)
                SyncNodes(p_Thread_جڑ->m_childs[i]);
#اگر_ختم
            اگر (is_rng_مستعمل)
            {
                // Some parallel backends execute nested jobs in the main thread,
                // so we need to restore initial RNG state here.
                cv::theRNG() = rng;
                // We can't properly update RNG state based on RNG usage in worker threads,
                // so lets just change main thread RNG state to the next value.
                // Note: this behaviour is not equal to single-threaded mode.
                cv::theRNG().next();
            }
#اگر_تعریف_شدہ OPENCV_TRACE
            اگر (trace_جڑ_Region)
                CV_TRACE_NS::details::parallelForFinalize(*trace_جڑ_Region);
#اگر_ختم

            اگر (hasException)
            {
#اگر CV__EXCEPTION_PTR
                std::rethrow_exception(pException);
#ورنہ
                CV_Error(Error::StsError, "Exception in parallel_for() body: " + exception_پیغام);
#اگر_ختم
            }
        }
        ~ParallelLoopBodyWrapperContext() {}

        ثابت cv::ParallelLoopBody* body;
        cv::Range پورا_Range;
        صحیح nstripes;
        cv::RNG rng;
        تغیرپذیر منطقی is_rng_مستعمل;
#اگر_تعریف_شدہ OPENCV_TRACE
        CV_TRACE_NS::details::Region* trace_جڑ_Region;
        CV_TRACE_NS::details::TraceManagerThreadLocal* trace_جڑ_Context;
#اگر_ختم
#اگر_تعریف_شدہ ENABLE_INSTRUMENTATION
        cv::instr::Instr_گرہ *p_Thread_جڑ;
#اگر_ختم
        منطقی hasException;
#اگر CV__EXCEPTION_PTR
        std::exception_ptr pException;
#ورنہ
        cv::String exception_پیغام;
#اگر_ختم
#اگر CV__EXCEPTION_PTR
        خالی ریکارڈ_Exception()
#ورنہ
        خالی ریکارڈ_Exception(ثابت cv::String& پیغام)
#اگر_ختم
        {
#اگر_غیر_تعریف_شدہ CV_THREAD_SANITIZER
            اگر (!hasException)
#اگر_ختم
            {
                cv::Auto_تالا lock(cv::getInitializationMutex());
                اگر (!hasException)
                {
                    hasException = سچ;
#اگر CV__EXCEPTION_PTR
                    pException = std::current_exception();
#ورنہ
                    exception_پیغام = پیغام;
#اگر_ختم
                }
            }
        }

#اگر OPENCV_SUPPORTS_FP_DENORMALS_HINT && OPENCV_IMPL_FP_HINTS
        details::FPDenormals_منوال_State fp_denormals_بنیاد_state;
#اگر_ختم

    نجی:
        ParallelLoopBodyWrapperContext(ثابت ParallelLoopBodyWrapperContext&); // disabled
        ParallelLoopBodyWrapperContext& عامل=(ثابت ParallelLoopBodyWrapperContext&); // disabled
    };

    جماعت ParallelLoopBodyWrapper : عوامی cv::ParallelLoopBody
    {
    عوامی:
        ParallelLoopBodyWrapper(ParallelLoopBodyWrapperContext& ctx_) :
            ctx(ctx_)
        {
        }
        ~ParallelLoopBodyWrapper()
        {
        }
        خالی عامل()(ثابت cv::Range& sr) ثابت CV_OVERRIDE
        {
#اگر_تعریف_شدہ OPENCV_TRACE
            // TODO CV_TRACE_NS::details::setCurrentRegion(rootRegion);
            اگر (ctx.trace_جڑ_Region && ctx.trace_جڑ_Context)
                CV_TRACE_NS::details::parallel_For_سیٹ_جڑ_Region(*ctx.trace_جڑ_Region, *ctx.trace_جڑ_Context);
            CV__TRACE_OPENCV_FUNCTION_NAME("parallel_for_body");
            اگر (ctx.trace_جڑ_Region)
                CV_TRACE_NS::details::parallelForAttachNestedRegion(*ctx.trace_جڑ_Region);
#اگر_ختم

#اگر_تعریف_شدہ ENABLE_INSTRUMENTATION
            {
                cv::instr::InstrTLSStruct *pInstrTLS = &cv::instr::getInstrumentTLSStruct();
                pInstrTLS->p_Current_گرہ = ctx.p_Thread_جڑ; // Initialize TLS node for thread
            }
            CV_INSTRUMENT_REGION();
#اگر_ختم

            // propagate main thread state
            cv::theRNG() = ctx.rng;
#اگر OPENCV_SUPPORTS_FP_DENORMALS_HINT && OPENCV_IMPL_FP_HINTS
            FPDenormalsIgnoreHintScope fp_denormals_scope(ctx.fp_denormals_بنیاد_state);
#اگر_ختم

            cv::Range r;
            cv::Range پورا_Range = ctx.پورا_Range;
            صحیح nstripes = ctx.nstripes;
            r.آغاز = (صحیح)(پورا_Range.آغاز +
                            ((uint64)sr.آغاز*(پورا_Range.end - پورا_Range.آغاز) + nstripes/2)/nstripes);
            r.end = sr.end >= nstripes ? پورا_Range.end : (صحیح)(پورا_Range.آغاز +
                            ((uint64)sr.end*(پورا_Range.end - پورا_Range.آغاز) + nstripes/2)/nstripes);

#اگر_تعریف_شدہ OPENCV_TRACE
            CV_TRACE_ARG_قدر(range_آغاز, "range.start", (int64)r.آغاز);
            CV_TRACE_ARG_قدر(range_انتہا, "range.end", (int64)r.end);
#اگر_ختم

            کوشش
            {
                (*ctx.body)(r);
            }
#اگر CV__EXCEPTION_PTR
            پکڑو (...)
            {
                ctx.ریکارڈ_Exception();
            }
#ورنہ
            پکڑو (ثابت cv::Exception& e)
            {
                ctx.ریکارڈ_Exception(e.what());
            }
            پکڑو (ثابت std::exception& e)
            {
                ctx.ریکارڈ_Exception(e.what());
            }
            پکڑو (...)
            {
                ctx.ریکارڈ_Exception("Unknown exception");
            }
#اگر_ختم

            اگر (!ctx.is_rng_مستعمل && !(cv::theRNG() == ctx.rng))
                ctx.is_rng_مستعمل = سچ;
        }
        cv::Range stripeRange() ثابت { واپس cv::Range(0, ctx.nstripes); }

    محفوظ:
        ParallelLoopBodyWrapperContext& ctx;
    };

#اگر defined HAVE_TBB
    جماعت ProxyLoopBody : عوامی ParallelLoopBodyWrapper
    {
    عوامی:
        ProxyLoopBody(ParallelLoopBodyWrapperContext& ctx_)
        : ParallelLoopBodyWrapper(ctx_)
        {}

        خالی عامل ()(ثابت tbb::blocked_range<صحیح>& range) ثابت
        {
            یہ->ParallelLoopBodyWrapper::عامل()(cv::Range(range.begin(), range.end()));
        }

        خالی عامل ()() ثابت  // run parallel job
        {
            cv::Range range = یہ->stripeRange();
            tbb::parallel_for(tbb::blocked_range<صحیح>(range.آغاز, range.end), *یہ);
        }
    };
#وگرنہ_اگر defined HAVE_HPX
    جماعت ProxyLoopBody : عوامی ParallelLoopBodyWrapper
    {
    عوامی:
        ProxyLoopBody(ParallelLoopBodyWrapperContext& ctx_)
                : ParallelLoopBodyWrapper(ctx_)
        {}

        خالی عامل ()() ثابت  // run parallel job
        {
            cv::Range stripeRange = یہ->stripeRange();
            hpx::parallel::for_loop(
                    hpx::parallel::execution::par,
                    stripeRange.آغاز, stripeRange.end,
                    [&](ثابت صحیح &i) { ;
                        یہ->ParallelLoopBodyWrapper::عامل()(
                                cv::Range(i, i + 1));
                    });
        }
    };
#وگرنہ_اگر defined HAVE_OPENMP
    قسمتعریف ParallelLoopBodyWrapper ProxyLoopBody;
#وگرنہ_اگر defined HAVE_عاد_اعظم
    قسمتعریف ParallelLoopBodyWrapper ProxyLoopBody;
    ساکن خالی بلاک_function(خالی* context, size_t اشاریہ)
    {
        ProxyLoopBody* ptr_body = static_cast<ProxyLoopBody*>(context);
        (*ptr_body)(cv::Range((صحیح)اشاریہ, (صحیح)اشاریہ + 1));
    }
#وگرنہ_اگر defined WINRT || defined HAVE_CONCURRENCY
    جماعت ProxyLoopBody : عوامی ParallelLoopBodyWrapper
    {
    عوامی:
        ProxyLoopBody(ParallelLoopBodyWrapperContext& ctx)
        : ParallelLoopBodyWrapper(ctx)
        {}

        خالی عامل ()(صحیح i) ثابت
        {
            یہ->ParallelLoopBodyWrapper::عامل()(cv::Range(i, i + 1));
        }
    };

    // PPL's default auto_partitioner silently skips whole chunks of the range on
    // ARM64 (unfixed MSVC bug, developercommunity.visualstudio.com/t/1027444).
    سانچہ <قسم_نام Body>
    ساکن اندرونی خالی pplParallelFor(صحیح first, صحیح آخری, ثابت Body& body)
    {
#اگر defined(_M_ARM64) || defined(_M_ARM64EC)
        Concurrency::parallel_for(first, آخری, body, Concurrency::static_partitioner());
#ورنہ
        Concurrency::parallel_for(first, آخری, body);
#اگر_ختم
    }
#ورنہ
    قسمتعریف ParallelLoopBodyWrapper ProxyLoopBody;
#اگر_ختم

#اگر defined HAVE_TBB
    #اگر TBB_INTERFACE_نسخہ >= 8000
        ساکن tbb::task_arena tbbArena(tbb::task_arena::automatic);
    #ورنہ
        ساکن tbb::task_scheduler_init tbbScheduler(tbb::task_scheduler_init::deferred);
    #اگر_ختم
#وگرنہ_اگر defined HAVE_HPX
// nothing for HPX
#وگرنہ_اگر defined HAVE_OPENMP
ساکن اندرونی صحیح init_Max_دھاگے()
{
    صحیح max_دھاگے = omp_get_max_دھاگے();
    واپس max_دھاگے;
}
ساکن صحیح عدد_دھاگے_Max = init_Max_دھاگے();
#وگرنہ_اگر defined HAVE_عاد_اعظم
// nothing for GCD
#وگرنہ_اگر defined WINRT
// nothing for WINRT
#وگرنہ_اگر defined HAVE_CONCURRENCY

جماعت SchedPtr
{
    Concurrency::Scheduler* sched_;
عوامی:
    Concurrency::Scheduler* عامل->() { واپس sched_; }
    عامل Concurrency::Scheduler*() { واپس sched_; }

    خالی عامل=(Concurrency::Scheduler* sched)
    {
        اگر (sched_) sched_->Release();
        sched_ = sched;
    }

    SchedPtr() : sched_(0) {}
    ~SchedPtr() {}
};
ساکن SchedPtr pplScheduler;

#اگر_ختم

} // namespace anon

/* ================================   parallel_for_  ================================ */

ساکن خالی parallel_for_impl(ثابت cv::Range& range, ثابت cv::ParallelLoopBody& body, دوہرا nstripes); // forward declaration

خالی parallel_for_(ثابت cv::Range& range, ثابت cv::ParallelLoopBody& body, دوہرا nstripes)
{
#اگر_تعریف_شدہ OPENCV_TRACE
    CV__TRACE_OPENCV_FUNCTION_NAME_("parallel_for", 0);
    CV_TRACE_ARG_قدر(range_آغاز, "range.start", (int64)range.آغاز);
    CV_TRACE_ARG_قدر(range_انتہا, "range.end", (int64)range.end);
    CV_TRACE_ARG_قدر(nstripes, "nstripes", (int64)nstripes);
#اگر_ختم

    CV_INSTRUMENT_REGION_MT_FORK();
    اگر (range.empty())
        واپس;

    ساکن std::atomic<منطقی> flagNestedParallelFor(جھوٹ);
    منطقی isNotNestedRegion = !flagNestedParallelFor.load();
    اگر (isNotNestedRegion)
      isNotNestedRegion = !flagNestedParallelFor.exchange(سچ);
    اگر (isNotNestedRegion)
    {
        کوشش
        {
            parallel_for_impl(range, body, nstripes);
            flagNestedParallelFor = جھوٹ;
        }
        پکڑو (...)
        {
            flagNestedParallelFor = جھوٹ;
            پھینکو;
        }
    }
    ورنہ // nested parallel_for_() calls are not parallelized
    {
        CV_غیر_مستعمل(nstripes);
        body(range);
    }
}

ساکن
خالی parallel_for_cb(صحیح آغاز, صحیح end, خالی* ڈیٹا)
{
    CV_DbgAssert(ڈیٹا);
    ثابت cv::ParallelLoopBody& body = *(ثابت cv::ParallelLoopBody*)ڈیٹا;
    body(Range(آغاز, end));
}

ساکن خالی parallel_for_impl(ثابت cv::Range& range, ثابت cv::ParallelLoopBody& body, دوہرا nstripes)
{
    استعمال نامخلا cv::parallel;
    اگر ((عدد_دھاگے < 0 || عدد_دھاگے > 1) && range.end - range.آغاز > 1)
    {
        ParallelLoopBodyWrapperContext ctx(body, range, nstripes);
        ProxyLoopBody pbody(ctx);
        cv::Range stripeRange = pbody.stripeRange();
        اگر( stripeRange.end - stripeRange.آغاز == 1 )
        {
            body(range);
            واپس;
        }

        std::shared_ptr<ParallelForAPI>& api = getCurrentParallelForAPI();
        اگر (api)
        {
            CV_CheckEQ(stripeRange.آغاز, 0, "");
            api->parallel_for(stripeRange.end, parallel_for_cb, (خالی*)&pbody);
            ctx.finalize();  // propagate exceptions if exists
            واپس;
        }

#اگر_تعریف_شدہ CV_PARALLEL_FRAMEWORK
#اگر defined HAVE_TBB

#اگر TBB_INTERFACE_نسخہ >= 8000
        tbbArena.execute(pbody);
#ورنہ
        pbody();
#اگر_ختم

#وگرنہ_اگر defined HAVE_HPX
        pbody();

#وگرنہ_اگر defined HAVE_OPENMP

        #ہدایت omp parallel برائے جدول_وقت(dynamic) عدد_دھاگے_2(عدد_دھاگے > 0 ? عدد_دھاگے : عدد_دھاگے_Max)
        برائے (صحیح i = stripeRange.آغاز; i < stripeRange.end; ++i)
            pbody(Range(i, i + 1));

#وگرنہ_اگر defined HAVE_عاد_اعظم

        dispatch_قطار_t concurrent_قطار = dispatch_get_global_قطار(DISPATCH_قطار_ترجیح_DEFAULT, 0);
        dispatch_apply_f(stripeRange.end - stripeRange.آغاز, concurrent_قطار, &pbody, بلاک_function);

#وگرنہ_اگر defined WINRT

        pplParallelFor(stripeRange.آغاز, stripeRange.end, pbody);

#وگرنہ_اگر defined HAVE_CONCURRENCY

        اگر(!pplScheduler || pplScheduler->Id() == Concurrency::CurrentScheduler::Id())
        {
            pplParallelFor(stripeRange.آغاز, stripeRange.end, pbody);
        }
        ورنہ
        {
            pplScheduler->Attach();
            pplParallelFor(stripeRange.آغاز, stripeRange.end, pbody);
            Concurrency::CurrentScheduler::Detach();
        }

#وگرنہ_اگر defined HAVE_PTHREADS_PF

        parallel_for_pthreads(pbody.stripeRange(), pbody, pbody.stripeRange().size());

#ورنہ

#غلطی You have hacked and compiling with unsupported parallel framework

#اگر_ختم

        ctx.finalize();  // propagate exceptions if exists
        واپس;
#اگر_ختم // CV_PARALLEL_FRAMEWORK
    }

    body(range);
}


صحیح get_عدد_دھاگے(خالی)
{
    std::shared_ptr<ParallelForAPI>& api = getCurrentParallelForAPI();
    اگر (api)
    {
        واپس api->get_عدد_دھاگے();
    }

    اگر (عدد_دھاگے == 0)
        واپس 1;

#اگر defined HAVE_TBB

#اگر TBB_INTERFACE_نسخہ >= 9100
    واپس tbbArena.max_concurrency();
#وگرنہ_اگر TBB_INTERFACE_نسخہ >= 8000
    واپس عدد_دھاگے > 0
        ? عدد_دھاگے
        : tbb::task_scheduler_init::default_عدد_دھاگے();
#ورنہ
    واپس tbbScheduler.is_فعال()
           ? عدد_دھاگے
           : tbb::task_scheduler_init::default_عدد_دھاگے();
#اگر_ختم

#وگرنہ_اگر defined HAVE_HPX
    واپس عدد_دھاگے;

#وگرنہ_اگر defined HAVE_OPENMP

    واپس عدد_دھاگے > 0
           ? عدد_دھاگے
           : عدد_دھاگے_Max;


#وگرنہ_اگر defined HAVE_عاد_اعظم

    واپس cv::get_نمبر_کا_CPUs(); // the GCD thread pool limit

#وگرنہ_اگر defined WINRT

    واپس 0;

#وگرنہ_اگر defined HAVE_CONCURRENCY

    واپس (pplScheduler == 0)
        ? Concurrency::CurrentScheduler::Get()->Get_نمبر_کا_Virtual_Processors()
        : (1 + pplScheduler->Get_نمبر_کا_Virtual_Processors());

#وگرنہ_اگر defined HAVE_PTHREADS_PF

        واپس parallel_pthreads_get_دھاگے_عدد();

#ورنہ

    واپس 1;

#اگر_ختم
}

غیرعلامتی default_نمبر_کا_دھاگے()
{
#اگر_تعریف_شدہ __ANDROID__
    // many modern phones/tables have 4-core CPUs. Let's use no more
    // than 2 threads by default not to overheat the devices
    ثابت غیرعلامتی صحیح default_نمبر_کا_دھاگے_2 = 2;
#ورنہ
    ثابت غیرعلامتی صحیح default_نمبر_کا_دھاگے_2 = (غیرعلامتی صحیح)std::max(1, cv::get_نمبر_کا_CPUs());
#اگر_ختم

    غیرعلامتی نتیجہ = default_نمبر_کا_دھاگے_2;

    ساکن صحیح config_عدد_دھاگے = (صحیح)utils::get_Configuration_Parameter_سائز_T("OPENCV_FOR_THREADS_NUM", 0);

    اگر (config_عدد_دھاگے)
    {
        نتیجہ = (غیرعلامتی)std::max(1, config_عدد_دھاگے);
        //do we need upper limit of threads number?
    }
    واپس نتیجہ;
}

خالی سیٹ_عدد_دھاگے( صحیح دھاگے_2 )
{
    CV_غیر_مستعمل(دھاگے_2);

    صحیح دھاگے = (دھاگے_2 < 0) ? default_نمبر_کا_دھاگے() : (غیرعلامتی)دھاگے_2;
    عدد_دھاگے = دھاگے;

    std::shared_ptr<ParallelForAPI>& api = getCurrentParallelForAPI();
    اگر (api)
    {
        api->سیٹ_عدد_دھاگے(عدد_دھاگے);
    }

#اگر_تعریف_شدہ HAVE_TBB

#اگر TBB_INTERFACE_نسخہ >= 8000
    اگر(tbbArena.is_فعال()) tbbArena.terminate();
    اگر(دھاگے > 0) tbbArena.initialize(دھاگے);
#ورنہ
    اگر(tbbScheduler.is_فعال()) tbbScheduler.terminate();
    اگر(دھاگے > 0) tbbScheduler.initialize(دھاگے);
#اگر_ختم

#وگرنہ_اگر defined HAVE_HPX
    واپس; // nothing needed as numThreads is used

#وگرنہ_اگر defined HAVE_OPENMP

    واپس; // nothing needed as num_threads clause is used in #pragma omp parallel for

#وگرنہ_اگر defined HAVE_عاد_اعظم

    // unsupported
    // there is only private dispatch_queue_set_width() and only for desktop

#وگرنہ_اگر defined WINRT

    واپس;

#وگرنہ_اگر defined HAVE_CONCURRENCY

    اگر (دھاگے <= 0)
    {
        pplScheduler = 0;
    }
    ورنہ اگر (دھاگے == 1)
    {
        // Concurrency always uses >=2 threads, so we just disable it if 1 thread is requested
        عدد_دھاگے = 0;
    }
    ورنہ اگر (pplScheduler == 0 || 1 + pplScheduler->Get_نمبر_کا_Virtual_Processors() != (غیرعلامتی صحیح)دھاگے)
    {
        pplScheduler = Concurrency::Scheduler::Create(Concurrency::SchedulerPolicy(2,
                       Concurrency::MinConcurrency, دھاگے-1,
                       Concurrency::MaxConcurrency, دھاگے-1));
    }

#وگرنہ_اگر defined HAVE_PTHREADS_PF

    parallel_pthreads_سیٹ_دھاگے_عدد(دھاگے);

#اگر_ختم
}


صحیح get_Thread_عدد()
{
    std::shared_ptr<ParallelForAPI>& api = getCurrentParallelForAPI();
    اگر (api)
    {
        واپس api->get_Thread_عدد();
    }

#اگر defined HAVE_TBB
    #اگر TBB_INTERFACE_نسخہ >= 9100
        واپس tbb::this_task_arena::current_thread_اشاریہ();
    #وگرنہ_اگر TBB_INTERFACE_نسخہ >= 8000
        واپس tbb::task_arena::current_thread_اشاریہ();
    #ورنہ
        واپس 0;
    #اگر_ختم
#وگرنہ_اگر defined HAVE_HPX
    واپس (صحیح)(hpx::get_عدد_کارکن_دھاگے());
#وگرنہ_اگر defined HAVE_OPENMP
    واپس omp_get_thread_عدد();
#وگرنہ_اگر defined HAVE_عاد_اعظم
    واپس (صحیح)(size_t)(خالی*)pthread_self(); // no zero-based indexing
#وگرنہ_اگر defined WINRT
    واپس 0;
#وگرنہ_اگر defined HAVE_CONCURRENCY
    واپس std::max(0, (صحیح)Concurrency::Context::VirtualProcessorId()); // zero for master thread, unique number for others but not necessary 1,2,3,...
#وگرنہ_اگر defined HAVE_PTHREADS_PF
    واپس (صحیح)(size_t)(خالی*)pthread_self(); // no zero-based indexing
#ورنہ
    واپس 0;
#اگر_ختم
}


#اگر defined __linux__ || defined __GLIBC__ || defined __HAIKU__ || defined __ANDROID__
  #تعریف_کرو CV_CPU_GROUPS_1
#اگر_ختم

#اگر defined __linux__ || defined __ANDROID__
  #تعریف_کرو CV_HAVE_CGROUPS 1
#اگر_ختم

#اگر defined CV_CPU_GROUPS_1
ساکن اندرونی
std::string getFileContents(ثابت حرف *filename)
{
    std::ifstream ifs(filename);
    اگر (!ifs.is_open())
        واپس std::string();

    std::string content( (std::istreambuf_iterator<حرف>(ifs) ),
                         (std::istreambuf_iterator<حرف>()    ) );

    اگر (ifs.fail())
        واپس std::string();

    واپس content;
}

ساکن اندرونی
صحیح get_نمبر_کا_CPUs_Impl(ثابت حرف *filename)
{
   std::string file_contents = getFileContents(filename);
   اگر(file_contents.empty())
       واپس 0;

   حرف *pbuf = const_cast<حرف*>(file_contents.c_str());
   //parse string of form "0-1,3,5-7,10,13-15"
   صحیح cpus_دستیاب = 0;

   جب(*pbuf)
   {
      ثابت حرف* pos = pbuf;
      منطقی range = جھوٹ;
      جب(*pbuf && *pbuf != ',')
      {
          اگر(*pbuf == '-') range = سچ;
          ++pbuf;
      }
      اگر(*pbuf) *pbuf++ = 0;
      اگر(!range)
        ++cpus_دستیاب;
      ورنہ
      {
          صحیح rstart = 0, rend = 0;
          لڑی_سے_پڑھو(pos, "%d-%d", &rstart, &rend);
          cpus_دستیاب += rend - rstart + 1;
      }

   }
   واپس cpus_دستیاب;
}
#اگر_ختم

#اگر defined CV_HAVE_CGROUPS
ساکن اندرونی
غیرعلامتی get_نمبر_کا_CPUs_CFSv2()
{
    صحیح cfs_quota = 0;
    صحیح cfs_period = 0;

    std::ifstream ss_cpu_max("/sys/fs/cgroup/cpu.max", std::ios::in | std::ios::binary);
    ss_cpu_max >> cfs_quota >> cfs_period;

    اگر (ss_cpu_max.fail() || cfs_quota < 1 || cfs_period < 1) /* values must not be 0 or negative */
        واپس 0;

    واپس (غیرعلامتی)max(1, cfs_quota/cfs_period);
}

ساکن اندرونی
غیرعلامتی get_نمبر_کا_CPUs_CFSv1()
{
    صحیح cfs_quota = 0;
    {
        std::ifstream ss_period("/sys/fs/cgroup/cpu/cpu.cfs_quota_us", std::ios::in | std::ios::binary);
        ss_period >> cfs_quota;

        اگر (ss_period.fail() || cfs_quota < 1) /* cfs_quota must not be 0 or negative */
            واپس 0;
    }

    صحیح cfs_period = 0;
    {
        std::ifstream ss_quota("/sys/fs/cgroup/cpu/cpu.cfs_period_us", std::ios::in | std::ios::binary);
        ss_quota >> cfs_period;

        اگر (ss_quota.fail() || cfs_period < 1)
            واپس 0;
    }

    واپس (غیرعلامتی)max(1, cfs_quota/cfs_period);
}
#اگر_ختم

سانچہ <قسم_نام T> ساکن اندرونی
T minNonZero(ثابت T& val_1, ثابت T& val_2)
{
    اگر ((val_1 != 0) && (val_2 != 0))
        واپس std::min(val_1, val_2);
    واپس (val_1 != 0) ? val_1 : val_2;
}

#اگر_غیر_تعریف_شدہ OPENCV_DISABLE_THREAD_SUPPORT
ساکن
صحیح get_نمبر_کا_CPUs_2()
{
#اگر_غیر_تعریف_شدہ OPENCV_SEMIHOSTING
    /*
     * Logic here is to try different methods of getting CPU counts and return
     * the minimum most value as it has high probablity of being right and safe.
     * Return 1 if we get 0 or not found on all methods.
    */
#اگر !defined(__MINGW32__) /* not implemented (2020-03) */

    /*
     * Check for this standard C++11 way, we do not return directly because
     * running in a docker or K8s environment will mean this is the host
     * machines config not the containers or pods and as per docs this value
     * must be "considered only a hint".
    */
    غیرعلامتی ncpus = std::thread::hardware_concurrency(); /* If the value is not well defined or not computable, returns 0 */
#ورنہ
    غیرعلامتی ncpus = 0; /* 0 means we have to find out some other way */
#اگر_ختم

#اگر defined _WIN32

    SYSTEM_INFO sysinfo = {};
#اگر (defined(_M_ARM) || defined(_M_ARM64) || defined(_M_ARM64EC) || defined(_M_X64) || defined(WINRT)) && _WIN32_WINNT >= 0x501
    GetNativeSystemInfo( &sysinfo );
#ورنہ
    GetSystemInfo( &sysinfo );
#اگر_ختم
    غیرعلامتی ncpus_sysinfo = sysinfo.dw_نمبر_کا_Processors;
    ncpus = minNonZero(ncpus, ncpus_sysinfo);

#وگرنہ_اگر defined __APPLE__

    صحیح عدد_CPU=0;
    صحیح mib[4];
    size_t len = حجم(عدد_CPU);

    /* set the mib for hw.ncpu */
    mib[0] = CTL_HW;
    mib[1] = HW_AVAILCPU;  // alternatively, try HW_NCPU;

    /* get the number of CPUs from the system */
    sysctl(mib, 2, &عدد_CPU, &len, تہی, 0);

    اگر( عدد_CPU < 1 )
    {
        mib[1] = HW_NCPU;
        sysctl( mib, 2, &عدد_CPU, &len, تہی, 0 );

        اگر( عدد_CPU < 1 )
            عدد_CPU = 1;
    }

    ncpus = minNonZero(ncpus, (غیرعلامتی)عدد_CPU);

#وگرنہ_اگر defined CV_CPU_GROUPS_1

#اگر defined CV_HAVE_CGROUPS
    ساکن غیرعلامتی ncpus_impl_cpuset = (غیرعلامتی)get_نمبر_کا_CPUs_Impl("/sys/fs/cgroup/cpuset/cpuset.cpus");
    ncpus = minNonZero(ncpus, ncpus_impl_cpuset);

    ساکن غیرعلامتی ncpus_impl_cfs_v1 = get_نمبر_کا_CPUs_CFSv1();
    ncpus = minNonZero(ncpus, ncpus_impl_cfs_v1);

    ساکن غیرعلامتی ncpus_impl_cfs_v2 = get_نمبر_کا_CPUs_CFSv2();
    ncpus = minNonZero(ncpus, ncpus_impl_cfs_v2);
#اگر_ختم

    ساکن غیرعلامتی ncpus_impl_devices = (غیرعلامتی)get_نمبر_کا_CPUs_Impl("/sys/devices/system/cpu/online");
    ncpus = minNonZero(ncpus, ncpus_impl_devices);

#اگر_ختم

#اگر defined _GNU_SOURCE \
    && !defined(__MINGW32__) /* not implemented (2020-03) */ \
    && !defined(__EMSCRIPTEN__) \
    && !defined(__ANDROID__)  // TODO: add check for modern Android NDK

    cpu_set_t cpu_سیٹ;
    اگر (0 == sched_getaffinity(0, حجم(cpu_سیٹ), &cpu_سیٹ))
    {
        غیرعلامتی cpu_گنتی_cpu_سیٹ = CPU_COUNT(&cpu_سیٹ);
        ncpus = minNonZero(ncpus, cpu_گنتی_cpu_سیٹ);
    }

#اگر_ختم

#اگر !defined(_WIN32) && !defined(__APPLE__) && defined(_SC_NPROCESSORS_ONLN)

    ساکن غیرعلامتی cpu_گنتی_sysconf = (غیرعلامتی)sysconf( _SC_NPROCESSORS_ONLN );
    ncpus = minNonZero(ncpus, cpu_گنتی_sysconf);
#وگرنہ_اگر defined (__QNX__)
    ساکن غیرعلامتی cpu_گنتی_sysconf = _syspage_ptr->عدد_cpu;
    ncpus = minNonZero(ncpus, cpu_گنتی_sysconf);
#اگر_ختم

    واپس ncpus != 0 ? ncpus : 1;
#ورنہ //  OPENCV_SEMIHOSTING
    واپس 1;
#اگر_ختم //OPENCV_SEMIHOSTING
}

صحیح get_نمبر_کا_CPUs()
{
    ساکن صحیح nCPUs = get_نمبر_کا_CPUs_2();
    واپس nCPUs;  // cached value
}

#ورنہ  // OPENCV_DISABLE_THREAD_SUPPORT
صحیح get_نمبر_کا_CPUs()
{
    واپس 1;
}
#اگر_ختم  // OPENCV_DISABLE_THREAD_SUPPORT

ثابت حرف* currentParallelFramework()
{
    std::shared_ptr<ParallelForAPI>& api = getCurrentParallelForAPI();
    اگر (api)
    {
        واپس api->getName();
    }
#اگر_تعریف_شدہ CV_PARALLEL_FRAMEWORK
    واپس CV_PARALLEL_FRAMEWORK;
#ورنہ
    واپس تہی;
#اگر_ختم
}

}  // namespace cv::
