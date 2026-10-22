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
    #undef small
    #undef min
    #undef max
    #undef abs
#endif

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
    #وگرنهاگر defined __APPLE__
        #شامل <sys/sysctl.h>
    #endif
#endif

#اگر defined (__QNX__)
    #شامل <sys/syspage.h>
#endif

#ifndef OPENCV_DISABLE_THREAD_SUPPORT
    #شامل <thread>
#endif

#ifdef _OPENMP
    #define HAVE_OPENMP
#endif

#ifdef __APPLE__
    #define HAVE_GCD
#endif

#اگر defined _MSC_VER
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

#اگر defined HAVE_TBB
    #ifndef TBB_SUPPRESS_DEPRECATED_MESSAGES  // supress warning
    #define TBB_SUPPRESS_DEPRECATED_MESSAGES 1
    #endif
    #شامل "tbb/tbb.h"
    #شامل "tbb/task.h"
    #اگر TBB_INTERFACE_VERSION >= 8000
        #شامل "tbb/task_arena.h"
    #endif
    #undef min
    #undef max
#وگرنهاگر defined HAVE_HPX
    #شامل <hpx/parallel/algorithms/for_loop.hpp>
    #شامل <hpx/parallel/execution.hpp>
    //
    #شامل <hpx/hpx_start.hpp>
    #شامل <hpx/hpx_suspend.hpp>
    #شامل <hpx/include/apply.hpp>
    #شامل <hpx/util/yield_while.hpp>
    #شامل <hpx/include/threadmanager.hpp>

#وگرنهاگر defined HAVE_OPENMP
    #شامل <omp.h>
#وگرنهاگر defined HAVE_GCD
    #شامل <dispatch/dispatch.h>
    #شامل <pthread.h>
#وگرنهاگر defined HAVE_CONCURRENCY
    #شامل <ppl.h>
#وگرنهاگر defined HAVE_PTHREADS_PF
    #شامل <pthread.h>
#endif


#اگر defined HAVE_TBB
#  define CV_PARALLEL_FRAMEWORK "tbb"
#وگرنهاگر defined HAVE_HPX
#  define CV_PARALLEL_FRAMEWORK "hpx"
#وگرنهاگر defined HAVE_OPENMP
#  define CV_PARALLEL_FRAMEWORK "openmp"
#وگرنهاگر defined HAVE_GCD
#  define CV_PARALLEL_FRAMEWORK "gcd"
#وگرنهاگر defined WINRT
#  define CV_PARALLEL_FRAMEWORK "winrt-concurrency"
#وگرنهاگر defined HAVE_CONCURRENCY
#  define CV_PARALLEL_FRAMEWORK "ms-concurrency"
#وگرنهاگر defined HAVE_PTHREADS_PF
#  define CV_PARALLEL_FRAMEWORK "pthreads"
#endif

#شامل <atomic>

#شامل "parallel_impl.hpp"

#شامل "opencv2/core/detail/exception_ptr.hpp"  // CV__EXCEPTION_PTR = 1 if std::exception_ptr is available

#شامل <opencv2/core/utils/fp_control_utils.hpp>
#شامل <opencv2/core/utils/fp_control.private.hpp>

using فضاینام cv;

فضاینام cv {

ParallelLoopBody::~ParallelLoopBody() {}

using فضاینام cv::parallel;

فضاینام {

#ifdef ENABLE_INSTRUMENTATION
    ایستا تهی SyncNodes(cv::instr::Instr_گره *p_گره)
    {
        std::vector<cv::instr::گره_Data_Tls*> data;
        p_گره->m_payload.m_tls.gather(data);

        uint64 ticksMax = 0;
        صحیح    threads  = 0;
        برای(size_t i = 0; i < data.size(); i++)
        {
            اگر(data[i] && data[i]->m_ticksTotal)
            {
                ticksMax = MAX(ticksMax, data[i]->m_ticksTotal);
                p_گره->m_payload.m_ticksTotal -= data[i]->m_ticksTotal;
                data[i]->m_ticksTotal = 0;
                threads++;
            }
        }
        p_گره->m_payload.m_ticksTotal += ticksMax;
        p_گره->m_payload.m_threads = MAX(p_گره->m_payload.m_threads, threads);

        برای(size_t i = 0; i < p_گره->m_childs.size(); i++)
            SyncNodes(p_گره->m_childs[i]);
    }
#endif

    رده ParallelLoopBodyWrapperContext
    {
    عمومی:
        ParallelLoopBodyWrapperContext(ثابت cv::ParallelLoopBody& _body, ثابت cv::Range& _r, دوگانه _nstripes) :
            is_rng_used(نادرست), hasException(نادرست)
        {

            body = &_body;
            wholeRange = _r;
            دوگانه len = wholeRange.end - wholeRange.start;
            nstripes = cvRound(_nstripes <= 0 ? len : MIN(MAX(_nstripes, 1.), len));

            // propagate main thread state
            rng = cv::theRNG();
#اگر OPENCV_SUPPORTS_FP_DENORMALS_HINT && OPENCV_IMPL_FP_HINTS
            details::saveFPDenormalsState(fp_denormals_base_state);
#endif

#ifdef OPENCV_TRACE
            trace_ریشه_Region = CV_TRACE_NS::details::getCurrentRegion();
            trace_ریشه_Context = CV_TRACE_NS::details::getTraceManager().tls.get();
#endif

#ifdef ENABLE_INSTRUMENTATION
            p_Thread_ریشه = cv::instr::getInstrumentTLSStruct().p_Current_گره;
#endif
        }
        تهی finalize()
        {
#ifdef ENABLE_INSTRUMENTATION
            برای(size_t i = 0; i < p_Thread_ریشه->m_childs.size(); i++)
                SyncNodes(p_Thread_ریشه->m_childs[i]);
#endif
            اگر (is_rng_used)
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
            اگر (trace_ریشه_Region)
                CV_TRACE_NS::details::parallelForFinalize(*trace_ریشه_Region);
#endif

            اگر (hasException)
            {
#اگر CV__EXCEPTION_PTR
                std::rethrow_exception(pException);
#وگرنه
                CV_Error(Error::StsError, "Exception in parallel_for() body: " + exception_message);
#endif
            }
        }
        ~ParallelLoopBodyWrapperContext() {}

        ثابت cv::ParallelLoopBody* body;
        cv::Range wholeRange;
        صحیح nstripes;
        cv::RNG rng;
        mutable منطقی is_rng_used;
#ifdef OPENCV_TRACE
        CV_TRACE_NS::details::Region* trace_ریشه_Region;
        CV_TRACE_NS::details::TraceManagerThreadLocal* trace_ریشه_Context;
#endif
#ifdef ENABLE_INSTRUMENTATION
        cv::instr::Instr_گره *p_Thread_ریشه;
#endif
        منطقی hasException;
#اگر CV__EXCEPTION_PTR
        std::exception_ptr pException;
#وگرنه
        cv::String exception_message;
#endif
#اگر CV__EXCEPTION_PTR
        تهی recordException()
#وگرنه
        تهی recordException(ثابت cv::String& پیام)
#endif
        {
#ifndef CV_THREAD_SANITIZER
            اگر (!hasException)
#endif
            {
                cv::AutoLock lock(cv::getInitializationMutex());
                اگر (!hasException)
                {
                    hasException = درست;
#اگر CV__EXCEPTION_PTR
                    pException = std::current_exception();
#وگرنه
                    exception_message = پیام;
#endif
                }
            }
        }

#اگر OPENCV_SUPPORTS_FP_DENORMALS_HINT && OPENCV_IMPL_FP_HINTS
        details::FPDenormalsModeState fp_denormals_base_state;
#endif

    خصوصی:
        ParallelLoopBodyWrapperContext(ثابت ParallelLoopBodyWrapperContext&); // disabled
        ParallelLoopBodyWrapperContext& operator=(ثابت ParallelLoopBodyWrapperContext&); // disabled
    };

    رده ParallelLoopBodyWrapper : عمومی cv::ParallelLoopBody
    {
    عمومی:
        ParallelLoopBodyWrapper(ParallelLoopBodyWrapperContext& ctx_) :
            ctx(ctx_)
        {
        }
        ~ParallelLoopBodyWrapper()
        {
        }
        تهی operator()(ثابت cv::Range& sr) ثابت CV_OVERRIDE
        {
#ifdef OPENCV_TRACE
            // TODO CV_TRACE_NS::details::setCurrentRegion(rootRegion);
            اگر (ctx.trace_ریشه_Region && ctx.trace_ریشه_Context)
                CV_TRACE_NS::details::parallel_For_Set_ریشه_Region(*ctx.trace_ریشه_Region, *ctx.trace_ریشه_Context);
            CV__TRACE_OPENCV_FUNCTION_NAME("parallel_for_body");
            اگر (ctx.trace_ریشه_Region)
                CV_TRACE_NS::details::parallelForAttachNestedRegion(*ctx.trace_ریشه_Region);
#endif

#ifdef ENABLE_INSTRUMENTATION
            {
                cv::instr::InstrTLSStruct *pInstrTLS = &cv::instr::getInstrumentTLSStruct();
                pInstrTLS->p_Current_گره = ctx.p_Thread_ریشه; // Initialize TLS node for thread
            }
            CV_INSTRUMENT_REGION();
#endif

            // propagate main thread state
            cv::theRNG() = ctx.rng;
#اگر OPENCV_SUPPORTS_FP_DENORMALS_HINT && OPENCV_IMPL_FP_HINTS
            FPDenormalsIgnoreHintScope fp_denormals_scope(ctx.fp_denormals_base_state);
#endif

            cv::Range r;
            cv::Range wholeRange = ctx.wholeRange;
            صحیح nstripes = ctx.nstripes;
            r.start = (صحیح)(wholeRange.start +
                            ((uint64)sr.start*(wholeRange.end - wholeRange.start) + nstripes/2)/nstripes);
            r.end = sr.end >= nstripes ? wholeRange.end : (صحیح)(wholeRange.start +
                            ((uint64)sr.end*(wholeRange.end - wholeRange.start) + nstripes/2)/nstripes);

#ifdef OPENCV_TRACE
            CV_TRACE_ARG_مقدار(range_start, "range.start", (int64)r.start);
            CV_TRACE_ARG_مقدار(range_end, "range.end", (int64)r.end);
#endif

            تلاش
            {
                (*ctx.body)(r);
            }
#اگر CV__EXCEPTION_PTR
            بگیر (...)
            {
                ctx.recordException();
            }
#وگرنه
            بگیر (ثابت cv::Exception& e)
            {
                ctx.recordException(e.what());
            }
            بگیر (ثابت std::exception& e)
            {
                ctx.recordException(e.what());
            }
            بگیر (...)
            {
                ctx.recordException("Unknown exception");
            }
#endif

            اگر (!ctx.is_rng_used && !(cv::theRNG() == ctx.rng))
                ctx.is_rng_used = درست;
        }
        cv::Range stripeRange() ثابت { بازگشت cv::Range(0, ctx.nstripes); }

    محافظتشده:
        ParallelLoopBodyWrapperContext& ctx;
    };

#اگر defined HAVE_TBB
    رده ProxyLoopBody : عمومی ParallelLoopBodyWrapper
    {
    عمومی:
        ProxyLoopBody(ParallelLoopBodyWrapperContext& ctx_)
        : ParallelLoopBodyWrapper(ctx_)
        {}

        تهی operator ()(ثابت tbb::blocked_range<صحیح>& range) ثابت
        {
            این->ParallelLoopBodyWrapper::operator()(cv::Range(range.begin(), range.end()));
        }

        تهی operator ()() ثابت  // run parallel job
        {
            cv::Range range = این->stripeRange();
            tbb::parallel_for(tbb::blocked_range<صحیح>(range.start, range.end), *این);
        }
    };
#وگرنهاگر defined HAVE_HPX
    رده ProxyLoopBody : عمومی ParallelLoopBodyWrapper
    {
    عمومی:
        ProxyLoopBody(ParallelLoopBodyWrapperContext& ctx_)
                : ParallelLoopBodyWrapper(ctx_)
        {}

        تهی operator ()() ثابت  // run parallel job
        {
            cv::Range stripeRange = این->stripeRange();
            hpx::parallel::for_loop(
                    hpx::parallel::execution::par,
                    stripeRange.start, stripeRange.end,
                    [&](ثابت صحیح &i) { ;
                        این->ParallelLoopBodyWrapper::operator()(
                                cv::Range(i, i + 1));
                    });
        }
    };
#وگرنهاگر defined HAVE_OPENMP
    تعریفنوع ParallelLoopBodyWrapper ProxyLoopBody;
#وگرنهاگر defined HAVE_GCD
    تعریفنوع ParallelLoopBodyWrapper ProxyLoopBody;
    ایستا تهی block_function(تهی* context, size_t index)
    {
        ProxyLoopBody* ptr_body = static_cast<ProxyLoopBody*>(context);
        (*ptr_body)(cv::Range((صحیح)index, (صحیح)index + 1));
    }
#وگرنهاگر defined WINRT || defined HAVE_CONCURRENCY
    رده ProxyLoopBody : عمومی ParallelLoopBodyWrapper
    {
    عمومی:
        ProxyLoopBody(ParallelLoopBodyWrapperContext& ctx)
        : ParallelLoopBodyWrapper(ctx)
        {}

        تهی operator ()(صحیح i) ثابت
        {
            این->ParallelLoopBodyWrapper::operator()(cv::Range(i, i + 1));
        }
    };

    // PPL's default auto_partitioner silently skips whole chunks of the range on
    // ARM64 (unfixed MSVC bug, developercommunity.visualstudio.com/t/1027444).
    template <typename Body>
    ایستا درونخطی تهی pplParallelFor(صحیح first, صحیح last, ثابت Body& body)
    {
#اگر defined(_M_ARM64) || defined(_M_ARM64EC)
        Concurrency::parallel_for(first, last, body, Concurrency::static_partitioner());
#وگرنه
        Concurrency::parallel_for(first, last, body);
#endif
    }
#وگرنه
    تعریفنوع ParallelLoopBodyWrapper ProxyLoopBody;
#endif

#اگر defined HAVE_TBB
    #اگر TBB_INTERFACE_VERSION >= 8000
        ایستا tbb::task_arena tbbArena(tbb::task_arena::automatic);
    #وگرنه
        ایستا tbb::task_scheduler_init tbbScheduler(tbb::task_scheduler_init::deferred);
    #endif
#وگرنهاگر defined HAVE_HPX
// nothing for HPX
#وگرنهاگر defined HAVE_OPENMP
ایستا درونخطی صحیح _initMaxThreads()
{
    صحیح maxThreads = omp_get_max_threads();
    بازگشت maxThreads;
}
ایستا صحیح رقم_Threads_Max = _initMaxThreads();
#وگرنهاگر defined HAVE_GCD
// nothing for GCD
#وگرنهاگر defined WINRT
// nothing for WINRT
#وگرنهاگر defined HAVE_CONCURRENCY

رده SchedPtr
{
    Concurrency::Scheduler* sched_;
عمومی:
    Concurrency::Scheduler* operator->() { بازگشت sched_; }
    operator Concurrency::Scheduler*() { بازگشت sched_; }

    تهی operator=(Concurrency::Scheduler* sched)
    {
        اگر (sched_) sched_->Release();
        sched_ = sched;
    }

    SchedPtr() : sched_(0) {}
    ~SchedPtr() {}
};
ایستا SchedPtr pplScheduler;

#endif

} // namespace anon

/* ================================   parallel_for_  ================================ */

ایستا تهی parallel_for_impl(ثابت cv::Range& range, ثابت cv::ParallelLoopBody& body, دوگانه nstripes); // forward declaration

تهی parallel_for_(ثابت cv::Range& range, ثابت cv::ParallelLoopBody& body, دوگانه nstripes)
{
#ifdef OPENCV_TRACE
    CV__TRACE_OPENCV_FUNCTION_NAME_("parallel_for", 0);
    CV_TRACE_ARG_مقدار(range_start, "range.start", (int64)range.start);
    CV_TRACE_ARG_مقدار(range_end, "range.end", (int64)range.end);
    CV_TRACE_ARG_مقدار(nstripes, "nstripes", (int64)nstripes);
#endif

    CV_INSTRUMENT_REGION_MT_FORK();
    اگر (range.empty())
        بازگشت;

    ایستا std::atomic<منطقی> flagNestedParallelFor(نادرست);
    منطقی isNotNestedRegion = !flagNestedParallelFor.load();
    اگر (isNotNestedRegion)
      isNotNestedRegion = !flagNestedParallelFor.exchange(درست);
    اگر (isNotNestedRegion)
    {
        تلاش
        {
            parallel_for_impl(range, body, nstripes);
            flagNestedParallelFor = نادرست;
        }
        بگیر (...)
        {
            flagNestedParallelFor = نادرست;
            پرتابکن;
        }
    }
    وگرنه // nested parallel_for_() calls are not parallelized
    {
        CV_UNUSED(nstripes);
        body(range);
    }
}

ایستا
تهی parallel_for_cb(صحیح start, صحیح end, تهی* data)
{
    CV_DbgAssert(data);
    ثابت cv::ParallelLoopBody& body = *(ثابت cv::ParallelLoopBody*)data;
    body(Range(start, end));
}

ایستا تهی parallel_for_impl(ثابت cv::Range& range, ثابت cv::ParallelLoopBody& body, دوگانه nstripes)
{
    using فضاینام cv::parallel;
    اگر ((رقم_Threads < 0 || رقم_Threads > 1) && range.end - range.start > 1)
    {
        ParallelLoopBodyWrapperContext ctx(body, range, nstripes);
        ProxyLoopBody pbody(ctx);
        cv::Range stripeRange = pbody.stripeRange();
        اگر( stripeRange.end - stripeRange.start == 1 )
        {
            body(range);
            بازگشت;
        }

        std::shared_ptr<ParallelForAPI>& api = getCurrentParallelForAPI();
        اگر (api)
        {
            CV_CheckEQ(stripeRange.start, 0, "");
            api->parallel_for(stripeRange.end, parallel_for_cb, (تهی*)&pbody);
            ctx.finalize();  // propagate exceptions if exists
            بازگشت;
        }

#ifdef CV_PARALLEL_FRAMEWORK
#اگر defined HAVE_TBB

#اگر TBB_INTERFACE_VERSION >= 8000
        tbbArena.execute(pbody);
#وگرنه
        pbody();
#endif

#وگرنهاگر defined HAVE_HPX
        pbody();

#وگرنهاگر defined HAVE_OPENMP

        #pragma omp parallel برای schedule(dynamic) رقم_threads(رقم_Threads > 0 ? رقم_Threads : رقم_Threads_Max)
        برای (صحیح i = stripeRange.start; i < stripeRange.end; ++i)
            pbody(Range(i, i + 1));

#وگرنهاگر defined HAVE_GCD

        dispatch_queue_t concurrent_queue = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0);
        dispatch_apply_f(stripeRange.end - stripeRange.start, concurrent_queue, &pbody, block_function);

#وگرنهاگر defined WINRT

        pplParallelFor(stripeRange.start, stripeRange.end, pbody);

#وگرنهاگر defined HAVE_CONCURRENCY

        اگر(!pplScheduler || pplScheduler->Id() == Concurrency::CurrentScheduler::Id())
        {
            pplParallelFor(stripeRange.start, stripeRange.end, pbody);
        }
        وگرنه
        {
            pplScheduler->Attach();
            pplParallelFor(stripeRange.start, stripeRange.end, pbody);
            Concurrency::CurrentScheduler::Detach();
        }

#وگرنهاگر defined HAVE_PTHREADS_PF

        parallel_for_pthreads(pbody.stripeRange(), pbody, pbody.stripeRange().size());

#وگرنه

#error You have hacked and compiling with unsupported parallel framework

#endif

        ctx.finalize();  // propagate exceptions if exists
        بازگشت;
#endif // CV_PARALLEL_FRAMEWORK
    }

    body(range);
}


صحیح get_رقم_Threads(تهی)
{
    std::shared_ptr<ParallelForAPI>& api = getCurrentParallelForAPI();
    اگر (api)
    {
        بازگشت api->get_رقم_Threads();
    }

    اگر (رقم_Threads == 0)
        بازگشت 1;

#اگر defined HAVE_TBB

#اگر TBB_INTERFACE_VERSION >= 9100
    بازگشت tbbArena.max_concurrency();
#وگرنهاگر TBB_INTERFACE_VERSION >= 8000
    بازگشت رقم_Threads > 0
        ? رقم_Threads
        : tbb::task_scheduler_init::default_رقم_threads();
#وگرنه
    بازگشت tbbScheduler.is_active()
           ? رقم_Threads
           : tbb::task_scheduler_init::default_رقم_threads();
#endif

#وگرنهاگر defined HAVE_HPX
    بازگشت رقم_Threads;

#وگرنهاگر defined HAVE_OPENMP

    بازگشت رقم_Threads > 0
           ? رقم_Threads
           : رقم_Threads_Max;


#وگرنهاگر defined HAVE_GCD

    بازگشت cv::get_عدد_Of_CPUs(); // the GCD thread pool limit

#وگرنهاگر defined WINRT

    بازگشت 0;

#وگرنهاگر defined HAVE_CONCURRENCY

    بازگشت (pplScheduler == 0)
        ? Concurrency::CurrentScheduler::Get()->Get_عدد_Of_Virtual_Processors()
        : (1 + pplScheduler->Get_عدد_Of_Virtual_Processors());

#وگرنهاگر defined HAVE_PTHREADS_PF

        بازگشت parallel_pthreads_get_threads_رقم();

#وگرنه

    بازگشت 1;

#endif
}

unsigned default_عدد_Of_Threads()
{
#ifdef __ANDROID__
    // many modern phones/tables have 4-core CPUs. Let's use no more
    // than 2 threads by default not to overheat the devices
    ثابت unsigned صحیح default_عدد_of_threads = 2;
#وگرنه
    ثابت unsigned صحیح default_عدد_of_threads = (unsigned صحیح)std::max(1, cv::get_عدد_Of_CPUs());
#endif

    unsigned result = default_عدد_of_threads;

    ایستا صحیح config_رقم_threads = (صحیح)utils::getConfigurationParameterSizeT("OPENCV_FOR_THREADS_NUM", 0);

    اگر (config_رقم_threads)
    {
        result = (unsigned)std::max(1, config_رقم_threads);
        //do we need upper limit of threads number?
    }
    بازگشت result;
}

تهی set_رقم_Threads( صحیح threads_ )
{
    CV_UNUSED(threads_);

    صحیح threads = (threads_ < 0) ? default_عدد_Of_Threads() : (unsigned)threads_;
    رقم_Threads = threads;

    std::shared_ptr<ParallelForAPI>& api = getCurrentParallelForAPI();
    اگر (api)
    {
        api->set_رقم_Threads(رقم_Threads);
    }

#ifdef HAVE_TBB

#اگر TBB_INTERFACE_VERSION >= 8000
    اگر(tbbArena.is_active()) tbbArena.terminate();
    اگر(threads > 0) tbbArena.initialize(threads);
#وگرنه
    اگر(tbbScheduler.is_active()) tbbScheduler.terminate();
    اگر(threads > 0) tbbScheduler.initialize(threads);
#endif

#وگرنهاگر defined HAVE_HPX
    بازگشت; // nothing needed as numThreads is used

#وگرنهاگر defined HAVE_OPENMP

    بازگشت; // nothing needed as num_threads clause is used in #pragma omp parallel for

#وگرنهاگر defined HAVE_GCD

    // unsupported
    // there is only private dispatch_queue_set_width() and only for desktop

#وگرنهاگر defined WINRT

    بازگشت;

#وگرنهاگر defined HAVE_CONCURRENCY

    اگر (threads <= 0)
    {
        pplScheduler = 0;
    }
    وگرنه اگر (threads == 1)
    {
        // Concurrency always uses >=2 threads, so we just disable it if 1 thread is requested
        رقم_Threads = 0;
    }
    وگرنه اگر (pplScheduler == 0 || 1 + pplScheduler->Get_عدد_Of_Virtual_Processors() != (unsigned صحیح)threads)
    {
        pplScheduler = Concurrency::Scheduler::Create(Concurrency::SchedulerPolicy(2,
                       Concurrency::MinConcurrency, threads-1,
                       Concurrency::MaxConcurrency, threads-1));
    }

#وگرنهاگر defined HAVE_PTHREADS_PF

    parallel_pthreads_set_threads_رقم(threads);

#endif
}


صحیح get_Thread_رقم()
{
    std::shared_ptr<ParallelForAPI>& api = getCurrentParallelForAPI();
    اگر (api)
    {
        بازگشت api->get_Thread_رقم();
    }

#اگر defined HAVE_TBB
    #اگر TBB_INTERFACE_VERSION >= 9100
        بازگشت tbb::this_task_arena::current_thread_index();
    #وگرنهاگر TBB_INTERFACE_VERSION >= 8000
        بازگشت tbb::task_arena::current_thread_index();
    #وگرنه
        بازگشت 0;
    #endif
#وگرنهاگر defined HAVE_HPX
    بازگشت (صحیح)(hpx::get_رقم_worker_threads());
#وگرنهاگر defined HAVE_OPENMP
    بازگشت omp_get_thread_رقم();
#وگرنهاگر defined HAVE_GCD
    بازگشت (صحیح)(size_t)(تهی*)pthread_self(); // no zero-based indexing
#وگرنهاگر defined WINRT
    بازگشت 0;
#وگرنهاگر defined HAVE_CONCURRENCY
    بازگشت std::max(0, (صحیح)Concurrency::Context::VirtualProcessorId()); // zero for master thread, unique number for others but not necessary 1,2,3,...
#وگرنهاگر defined HAVE_PTHREADS_PF
    بازگشت (صحیح)(size_t)(تهی*)pthread_self(); // no zero-based indexing
#وگرنه
    بازگشت 0;
#endif
}


#اگر defined __linux__ || defined __GLIBC__ || defined __HAIKU__ || defined __ANDROID__
  #define CV_CPU_GROUPS_1
#endif

#اگر defined __linux__ || defined __ANDROID__
  #define CV_HAVE_CGROUPS 1
#endif

#اگر defined CV_CPU_GROUPS_1
ایستا درونخطی
std::string getFileContents(ثابت نویسه *filename)
{
    std::ifstream ifs(filename);
    اگر (!ifs.is_open())
        بازگشت std::string();

    std::string content( (std::istreambuf_iterator<نویسه>(ifs) ),
                         (std::istreambuf_iterator<نویسه>()    ) );

    اگر (ifs.fail())
        بازگشت std::string();

    بازگشت content;
}

ایستا درونخطی
صحیح get_عدد_Of_CPUs_Impl(ثابت نویسه *filename)
{
   std::string file_contents = getFileContents(filename);
   اگر(file_contents.empty())
       بازگشت 0;

   نویسه *pbuf = const_cast<نویسه*>(file_contents.c_str());
   //parse string of form "0-1,3,5-7,10,13-15"
   صحیح cpusAvailable = 0;

   وقتی(*pbuf)
   {
      ثابت نویسه* pos = pbuf;
      منطقی range = نادرست;
      وقتی(*pbuf && *pbuf != ',')
      {
          اگر(*pbuf == '-') range = درست;
          ++pbuf;
      }
      اگر(*pbuf) *pbuf++ = 0;
      اگر(!range)
        ++cpusAvailable;
      وگرنه
      {
          صحیح rstart = 0, rend = 0;
          sscanf(pos, "%d-%d", &rstart, &rend);
          cpusAvailable += rend - rstart + 1;
      }

   }
   بازگشت cpusAvailable;
}
#endif

#اگر defined CV_HAVE_CGROUPS
ایستا درونخطی
unsigned get_عدد_Of_CPUs_CFSv2()
{
    صحیح cfs_quota = 0;
    صحیح cfs_period = 0;

    std::ifstream ss_cpu_max("/sys/fs/cgroup/cpu.max", std::ios::in | std::ios::binary);
    ss_cpu_max >> cfs_quota >> cfs_period;

    اگر (ss_cpu_max.fail() || cfs_quota < 1 || cfs_period < 1) /* values must not be 0 or negative */
        بازگشت 0;

    بازگشت (unsigned)max(1, cfs_quota/cfs_period);
}

ایستا درونخطی
unsigned get_عدد_Of_CPUs_CFSv1()
{
    صحیح cfs_quota = 0;
    {
        std::ifstream ss_period("/sys/fs/cgroup/cpu/cpu.cfs_quota_us", std::ios::in | std::ios::binary);
        ss_period >> cfs_quota;

        اگر (ss_period.fail() || cfs_quota < 1) /* cfs_quota must not be 0 or negative */
            بازگشت 0;
    }

    صحیح cfs_period = 0;
    {
        std::ifstream ss_quota("/sys/fs/cgroup/cpu/cpu.cfs_period_us", std::ios::in | std::ios::binary);
        ss_quota >> cfs_period;

        اگر (ss_quota.fail() || cfs_period < 1)
            بازگشت 0;
    }

    بازگشت (unsigned)max(1, cfs_quota/cfs_period);
}
#endif

template <typename T> ایستا درونخطی
T minNonZero(ثابت T& val_1, ثابت T& val_2)
{
    اگر ((val_1 != 0) && (val_2 != 0))
        بازگشت std::min(val_1, val_2);
    بازگشت (val_1 != 0) ? val_1 : val_2;
}

#ifndef OPENCV_DISABLE_THREAD_SUPPORT
ایستا
صحیح get_عدد_Of_CPUs_2()
{
#ifndef OPENCV_SEMIHOSTING
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
    unsigned ncpus = std::thread::hardware_concurrency(); /* If the value is not well defined or not computable, returns 0 */
#وگرنه
    unsigned ncpus = 0; /* 0 means we have to find out some other way */
#endif

#اگر defined _WIN32

    SYSTEM_INFO sysinfo = {};
#اگر (defined(_M_ARM) || defined(_M_ARM64) || defined(_M_ARM64EC) || defined(_M_X64) || defined(WINRT)) && _WIN32_WINNT >= 0x501
    GetNativeSystemInfo( &sysinfo );
#وگرنه
    GetSystemInfo( &sysinfo );
#endif
    unsigned ncpus_sysinfo = sysinfo.dw_عدد_Of_Processors;
    ncpus = minNonZero(ncpus, ncpus_sysinfo);

#وگرنهاگر defined __APPLE__

    صحیح رقم_CPU=0;
    صحیح mib[4];
    size_t len = اندازه(رقم_CPU);

    /* set the mib for hw.ncpu */
    mib[0] = CTL_HW;
    mib[1] = HW_AVAILCPU;  // alternatively, try HW_NCPU;

    /* get the number of CPUs from the system */
    sysctl(mib, 2, &رقم_CPU, &len, پوچ, 0);

    اگر( رقم_CPU < 1 )
    {
        mib[1] = HW_NCPU;
        sysctl( mib, 2, &رقم_CPU, &len, پوچ, 0 );

        اگر( رقم_CPU < 1 )
            رقم_CPU = 1;
    }

    ncpus = minNonZero(ncpus, (unsigned)رقم_CPU);

#وگرنهاگر defined CV_CPU_GROUPS_1

#اگر defined CV_HAVE_CGROUPS
    ایستا unsigned ncpus_impl_cpuset = (unsigned)get_عدد_Of_CPUs_Impl("/sys/fs/cgroup/cpuset/cpuset.cpus");
    ncpus = minNonZero(ncpus, ncpus_impl_cpuset);

    ایستا unsigned ncpus_impl_cfs_v1 = get_عدد_Of_CPUs_CFSv1();
    ncpus = minNonZero(ncpus, ncpus_impl_cfs_v1);

    ایستا unsigned ncpus_impl_cfs_v2 = get_عدد_Of_CPUs_CFSv2();
    ncpus = minNonZero(ncpus, ncpus_impl_cfs_v2);
#endif

    ایستا unsigned ncpus_impl_devices = (unsigned)get_عدد_Of_CPUs_Impl("/sys/devices/system/cpu/online");
    ncpus = minNonZero(ncpus, ncpus_impl_devices);

#endif

#اگر defined _GNU_SOURCE \
    && !defined(__MINGW32__) /* not implemented (2020-03) */ \
    && !defined(__EMSCRIPTEN__) \
    && !defined(__ANDROID__)  // TODO: add check for modern Android NDK

    cpu_set_t cpu_set;
    اگر (0 == sched_getaffinity(0, اندازه(cpu_set), &cpu_set))
    {
        unsigned cpu_شمار_cpu_set = CPU_COUNT(&cpu_set);
        ncpus = minNonZero(ncpus, cpu_شمار_cpu_set);
    }

#endif

#اگر !defined(_WIN32) && !defined(__APPLE__) && defined(_SC_NPROCESSORS_ONLN)

    ایستا unsigned cpu_شمار_sysconf = (unsigned)sysconf( _SC_NPROCESSORS_ONLN );
    ncpus = minNonZero(ncpus, cpu_شمار_sysconf);
#وگرنهاگر defined (__QNX__)
    ایستا unsigned cpu_شمار_sysconf = _syspage_ptr->رقم_cpu;
    ncpus = minNonZero(ncpus, cpu_شمار_sysconf);
#endif

    بازگشت ncpus != 0 ? ncpus : 1;
#وگرنه //  OPENCV_SEMIHOSTING
    بازگشت 1;
#endif //OPENCV_SEMIHOSTING
}

صحیح get_عدد_Of_CPUs()
{
    ایستا صحیح nCPUs = get_عدد_Of_CPUs_2();
    بازگشت nCPUs;  // cached value
}

#وگرنه  // OPENCV_DISABLE_THREAD_SUPPORT
صحیح get_عدد_Of_CPUs()
{
    بازگشت 1;
}
#endif  // OPENCV_DISABLE_THREAD_SUPPORT

ثابت نویسه* currentParallelFramework()
{
    std::shared_ptr<ParallelForAPI>& api = getCurrentParallelForAPI();
    اگر (api)
    {
        بازگشت api->getName();
    }
#ifdef CV_PARALLEL_FRAMEWORK
    بازگشت CV_PARALLEL_FRAMEWORK;
#وگرنه
    بازگشت پوچ;
#endif
}

}  // namespace cv::
