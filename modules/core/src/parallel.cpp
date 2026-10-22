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

#אם defined _WIN32 || defined WINCE
    #ضمن <windows.h>
    #undef small
    #undef min
    #undef max
    #undef abs
#endif

#אם defined __unix__ || defined __APPLE__ || defined __GLIBC__ \
    || defined __HAIKU__ || defined __EMSCRIPTEN__ \
    || defined __FreeBSD__ || defined __NetBSD__ || defined __OpenBSD__
    #ضمن <unistd.h>
    #ضمن <stdio.h>
    #ضمن <sys/types.h>
    #ضمن <fstream>
    #אם defined __ANDROID__
        #ضمن <sys/sysconf.h>
        #ضمن <sys/syscall.h>
        #ضمن <sched.h>
    #وإلاإذا defined __APPLE__
        #ضمن <sys/sysctl.h>
    #endif
#endif

#אם defined (__QNX__)
    #ضمن <sys/syspage.h>
#endif

#ifndef OPENCV_DISABLE_THREAD_SUPPORT
    #ضمن <thread>
#endif

#ifdef _OPENMP
    #define HAVE_OPENMP
#endif

#ifdef __APPLE__
    #define HAVE_GCD
#endif

#אם defined _MSC_VER
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

#אם defined HAVE_TBB
    #ifndef TBB_SUPPRESS_DEPRECATED_MESSAGES  // supress warning
    #define TBB_SUPPRESS_DEPRECATED_MESSAGES 1
    #endif
    #ضمن "tbb/tbb.h"
    #ضمن "tbb/task.h"
    #אם TBB_INTERFACE_VERSION >= 8000
        #ضمن "tbb/task_arena.h"
    #endif
    #undef min
    #undef max
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
#وإلاإذا defined HAVE_GCD
    #ضمن <dispatch/dispatch.h>
    #ضمن <pthread.h>
#وإلاإذا defined HAVE_CONCURRENCY
    #ضمن <ppl.h>
#وإلاإذا defined HAVE_PTHREADS_PF
    #ضمن <pthread.h>
#endif


#אם defined HAVE_TBB
#  define CV_PARALLEL_FRAMEWORK "tbb"
#وإلاإذا defined HAVE_HPX
#  define CV_PARALLEL_FRAMEWORK "hpx"
#وإلاإذا defined HAVE_OPENMP
#  define CV_PARALLEL_FRAMEWORK "openmp"
#وإلاإذا defined HAVE_GCD
#  define CV_PARALLEL_FRAMEWORK "gcd"
#وإلاإذا defined WINRT
#  define CV_PARALLEL_FRAMEWORK "winrt-concurrency"
#وإلاإذا defined HAVE_CONCURRENCY
#  define CV_PARALLEL_FRAMEWORK "ms-concurrency"
#وإلاإذا defined HAVE_PTHREADS_PF
#  define CV_PARALLEL_FRAMEWORK "pthreads"
#endif

#ضمن <atomic>

#ضمن "parallel_impl.hpp"

#ضمن "opencv2/core/detail/exception_ptr.hpp"  // CV__EXCEPTION_PTR = 1 if std::exception_ptr is available

#ضمن <opencv2/core/utils/fp_control_utils.hpp>
#ضمن <opencv2/core/utils/fp_control.private.hpp>

using نطاق cv;

نطاق cv {

ParallelLoopBody::~ParallelLoopBody() {}

using نطاق cv::parallel;

نطاق {

#ifdef ENABLE_INSTRUMENTATION
    ساكن فراغ SyncNodes(cv::instr::InstrNode *pNode)
    {
        std::vector<cv::instr::NodeDataTls*> data;
        pNode->m_payload.m_tls.gather(data);

        uint64 ticksMax = 0;
        صحيح    threads  = 0;
        עבור(size_t i = 0; i < data.size(); i++)
        {
            אם(data[i] && data[i]->m_ticksTotal)
            {
                ticksMax = MAX(ticksMax, data[i]->m_ticksTotal);
                pNode->m_payload.m_ticksTotal -= data[i]->m_ticksTotal;
                data[i]->m_ticksTotal = 0;
                threads++;
            }
        }
        pNode->m_payload.m_ticksTotal += ticksMax;
        pNode->m_payload.m_threads = MAX(pNode->m_payload.m_threads, threads);

        עבור(size_t i = 0; i < pNode->m_childs.size(); i++)
            SyncNodes(pNode->m_childs[i]);
    }
#endif

    מחלקה ParallelLoopBodyWrapperContext
    {
    عام:
        ParallelLoopBodyWrapperContext(ثابت cv::ParallelLoopBody& _body, ثابت cv::Range& _r, مزدوج _nstripes) :
            is_rng_used(שקר), hasException(שקר)
        {

            body = &_body;
            wholeRange = _r;
            مزدوج len = wholeRange.end - wholeRange.start;
            nstripes = cvRound(_nstripes <= 0 ? len : MIN(MAX(_nstripes, 1.), len));

            // propagate main thread state
            rng = cv::theRNG();
#אם OPENCV_SUPPORTS_FP_DENORMALS_HINT && OPENCV_IMPL_FP_HINTS
            details::saveFPDenormalsState(fp_denormals_base_state);
#endif

#ifdef OPENCV_TRACE
            traceRootRegion = CV_TRACE_NS::details::getCurrentRegion();
            traceRootContext = CV_TRACE_NS::details::getTraceManager().tls.get();
#endif

#ifdef ENABLE_INSTRUMENTATION
            pThreadRoot = cv::instr::getInstrumentTLSStruct().pCurrentNode;
#endif
        }
        فراغ finalize()
        {
#ifdef ENABLE_INSTRUMENTATION
            עבור(size_t i = 0; i < pThreadRoot->m_childs.size(); i++)
                SyncNodes(pThreadRoot->m_childs[i]);
#endif
            אם (is_rng_used)
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
            אם (traceRootRegion)
                CV_TRACE_NS::details::parallelForFinalize(*traceRootRegion);
#endif

            אם (hasException)
            {
#אם CV__EXCEPTION_PTR
                std::rethrow_exception(pException);
#אחרת
                CV_Error(Error::StsError, "Exception in parallel_for() body: " + exception_message);
#endif
            }
        }
        ~ParallelLoopBodyWrapperContext() {}

        ثابت cv::ParallelLoopBody* body;
        cv::Range wholeRange;
        صحيح nstripes;
        cv::RNG rng;
        mutable منطقي is_rng_used;
#ifdef OPENCV_TRACE
        CV_TRACE_NS::details::Region* traceRootRegion;
        CV_TRACE_NS::details::TraceManagerThreadLocal* traceRootContext;
#endif
#ifdef ENABLE_INSTRUMENTATION
        cv::instr::InstrNode *pThreadRoot;
#endif
        منطقي hasException;
#אם CV__EXCEPTION_PTR
        std::exception_ptr pException;
#אחרת
        cv::String exception_message;
#endif
#אם CV__EXCEPTION_PTR
        فراغ recordException()
#אחרת
        فراغ recordException(ثابت cv::String& msg)
#endif
        {
#ifndef CV_THREAD_SANITIZER
            אם (!hasException)
#endif
            {
                cv::AutoLock lock(cv::getInitializationMutex());
                אם (!hasException)
                {
                    hasException = אמת;
#אם CV__EXCEPTION_PTR
                    pException = std::current_exception();
#אחרת
                    exception_message = msg;
#endif
                }
            }
        }

#אם OPENCV_SUPPORTS_FP_DENORMALS_HINT && OPENCV_IMPL_FP_HINTS
        details::FPDenormalsModeState fp_denormals_base_state;
#endif

    خاص:
        ParallelLoopBodyWrapperContext(ثابت ParallelLoopBodyWrapperContext&); // disabled
        ParallelLoopBodyWrapperContext& operator=(ثابت ParallelLoopBodyWrapperContext&); // disabled
    };

    מחלקה ParallelLoopBodyWrapper : عام cv::ParallelLoopBody
    {
    عام:
        ParallelLoopBodyWrapper(ParallelLoopBodyWrapperContext& ctx_) :
            ctx(ctx_)
        {
        }
        ~ParallelLoopBodyWrapper()
        {
        }
        فراغ operator()(ثابت cv::Range& sr) ثابت CV_OVERRIDE
        {
#ifdef OPENCV_TRACE
            // TODO CV_TRACE_NS::details::setCurrentRegion(rootRegion);
            אם (ctx.traceRootRegion && ctx.traceRootContext)
                CV_TRACE_NS::details::parallelForSetRootRegion(*ctx.traceRootRegion, *ctx.traceRootContext);
            CV__TRACE_OPENCV_FUNCTION_NAME("parallel_for_body");
            אם (ctx.traceRootRegion)
                CV_TRACE_NS::details::parallelForAttachNestedRegion(*ctx.traceRootRegion);
#endif

#ifdef ENABLE_INSTRUMENTATION
            {
                cv::instr::InstrTLSStruct *pInstrTLS = &cv::instr::getInstrumentTLSStruct();
                pInstrTLS->pCurrentNode = ctx.pThreadRoot; // Initialize TLS node for thread
            }
            CV_INSTRUMENT_REGION();
#endif

            // propagate main thread state
            cv::theRNG() = ctx.rng;
#אם OPENCV_SUPPORTS_FP_DENORMALS_HINT && OPENCV_IMPL_FP_HINTS
            FPDenormalsIgnoreHintScope fp_denormals_scope(ctx.fp_denormals_base_state);
#endif

            cv::Range r;
            cv::Range wholeRange = ctx.wholeRange;
            صحيح nstripes = ctx.nstripes;
            r.start = (صحيح)(wholeRange.start +
                            ((uint64)sr.start*(wholeRange.end - wholeRange.start) + nstripes/2)/nstripes);
            r.end = sr.end >= nstripes ? wholeRange.end : (صحيح)(wholeRange.start +
                            ((uint64)sr.end*(wholeRange.end - wholeRange.start) + nstripes/2)/nstripes);

#ifdef OPENCV_TRACE
            CV_TRACE_ARG_VALUE(range_start, "range.start", (int64)r.start);
            CV_TRACE_ARG_VALUE(range_end, "range.end", (int64)r.end);
#endif

            حاول
            {
                (*ctx.body)(r);
            }
#אם CV__EXCEPTION_PTR
            امسك (...)
            {
                ctx.recordException();
            }
#אחרת
            امسك (ثابت cv::Exception& e)
            {
                ctx.recordException(e.what());
            }
            امسك (ثابت std::exception& e)
            {
                ctx.recordException(e.what());
            }
            امسك (...)
            {
                ctx.recordException("Unknown exception");
            }
#endif

            אם (!ctx.is_rng_used && !(cv::theRNG() == ctx.rng))
                ctx.is_rng_used = אמת;
        }
        cv::Range stripeRange() ثابت { החזר cv::Range(0, ctx.nstripes); }

    محمي:
        ParallelLoopBodyWrapperContext& ctx;
    };

#אם defined HAVE_TBB
    מחלקה ProxyLoopBody : عام ParallelLoopBodyWrapper
    {
    عام:
        ProxyLoopBody(ParallelLoopBodyWrapperContext& ctx_)
        : ParallelLoopBodyWrapper(ctx_)
        {}

        فراغ operator ()(ثابت tbb::blocked_range<صحيح>& range) ثابت
        {
            هذا->ParallelLoopBodyWrapper::operator()(cv::Range(range.begin(), range.end()));
        }

        فراغ operator ()() ثابت  // run parallel job
        {
            cv::Range range = هذا->stripeRange();
            tbb::parallel_for(tbb::blocked_range<صحيح>(range.start, range.end), *هذا);
        }
    };
#وإلاإذا defined HAVE_HPX
    מחלקה ProxyLoopBody : عام ParallelLoopBodyWrapper
    {
    عام:
        ProxyLoopBody(ParallelLoopBodyWrapperContext& ctx_)
                : ParallelLoopBodyWrapper(ctx_)
        {}

        فراغ operator ()() ثابت  // run parallel job
        {
            cv::Range stripeRange = هذا->stripeRange();
            hpx::parallel::for_loop(
                    hpx::parallel::execution::par,
                    stripeRange.start, stripeRange.end,
                    [&](ثابت صحيح &i) { ;
                        هذا->ParallelLoopBodyWrapper::operator()(
                                cv::Range(i, i + 1));
                    });
        }
    };
#وإلاإذا defined HAVE_OPENMP
    تعريفنوع ParallelLoopBodyWrapper ProxyLoopBody;
#وإلاإذا defined HAVE_GCD
    تعريفنوع ParallelLoopBodyWrapper ProxyLoopBody;
    ساكن فراغ block_function(فراغ* context, size_t index)
    {
        ProxyLoopBody* ptr_body = static_cast<ProxyLoopBody*>(context);
        (*ptr_body)(cv::Range((صحيح)index, (صحيح)index + 1));
    }
#وإلاإذا defined WINRT || defined HAVE_CONCURRENCY
    מחלקה ProxyLoopBody : عام ParallelLoopBodyWrapper
    {
    عام:
        ProxyLoopBody(ParallelLoopBodyWrapperContext& ctx)
        : ParallelLoopBodyWrapper(ctx)
        {}

        فراغ operator ()(صحيح i) ثابت
        {
            هذا->ParallelLoopBodyWrapper::operator()(cv::Range(i, i + 1));
        }
    };

    // PPL's default auto_partitioner silently skips whole chunks of the range on
    // ARM64 (unfixed MSVC bug, developercommunity.visualstudio.com/t/1027444).
    template <typename Body>
    ساكن مضمن فراغ pplParallelFor(صحيح first, صحيح last, ثابت Body& body)
    {
#אם defined(_M_ARM64) || defined(_M_ARM64EC)
        Concurrency::parallel_for(first, last, body, Concurrency::static_partitioner());
#אחרת
        Concurrency::parallel_for(first, last, body);
#endif
    }
#אחרת
    تعريفنوع ParallelLoopBodyWrapper ProxyLoopBody;
#endif

#אם defined HAVE_TBB
    #אם TBB_INTERFACE_VERSION >= 8000
        ساكن tbb::task_arena tbbArena(tbb::task_arena::automatic);
    #אחרת
        ساكن tbb::task_scheduler_init tbbScheduler(tbb::task_scheduler_init::deferred);
    #endif
#وإلاإذا defined HAVE_HPX
// nothing for HPX
#وإلاإذا defined HAVE_OPENMP
ساكن مضمن صحيح _initMaxThreads()
{
    صحيح maxThreads = omp_get_max_threads();
    החזר maxThreads;
}
ساكن صحيح numThreadsMax = _initMaxThreads();
#وإلاإذا defined HAVE_GCD
// nothing for GCD
#وإلاإذا defined WINRT
// nothing for WINRT
#وإلاإذا defined HAVE_CONCURRENCY

מחלקה SchedPtr
{
    Concurrency::Scheduler* sched_;
عام:
    Concurrency::Scheduler* operator->() { החזר sched_; }
    operator Concurrency::Scheduler*() { החזר sched_; }

    فراغ operator=(Concurrency::Scheduler* sched)
    {
        אם (sched_) sched_->Release();
        sched_ = sched;
    }

    SchedPtr() : sched_(0) {}
    ~SchedPtr() {}
};
ساكن SchedPtr pplScheduler;

#endif

} // namespace anon

/* ================================   parallel_for_  ================================ */

ساكن فراغ parallel_for_impl(ثابت cv::Range& range, ثابت cv::ParallelLoopBody& body, مزدوج nstripes); // forward declaration

فراغ parallel_for_(ثابت cv::Range& range, ثابت cv::ParallelLoopBody& body, مزدوج nstripes)
{
#ifdef OPENCV_TRACE
    CV__TRACE_OPENCV_FUNCTION_NAME_("parallel_for", 0);
    CV_TRACE_ARG_VALUE(range_start, "range.start", (int64)range.start);
    CV_TRACE_ARG_VALUE(range_end, "range.end", (int64)range.end);
    CV_TRACE_ARG_VALUE(nstripes, "nstripes", (int64)nstripes);
#endif

    CV_INSTRUMENT_REGION_MT_FORK();
    אם (range.empty())
        החזר;

    ساكن std::atomic<منطقي> flagNestedParallelFor(שקר);
    منطقي isNotNestedRegion = !flagNestedParallelFor.load();
    אם (isNotNestedRegion)
      isNotNestedRegion = !flagNestedParallelFor.exchange(אמת);
    אם (isNotNestedRegion)
    {
        حاول
        {
            parallel_for_impl(range, body, nstripes);
            flagNestedParallelFor = שקר;
        }
        امسك (...)
        {
            flagNestedParallelFor = שקר;
            ارم;
        }
    }
    אחרת // nested parallel_for_() calls are not parallelized
    {
        CV_UNUSED(nstripes);
        body(range);
    }
}

ساكن
فراغ parallel_for_cb(صحيح start, صحيح end, فراغ* data)
{
    CV_DbgAssert(data);
    ثابت cv::ParallelLoopBody& body = *(ثابت cv::ParallelLoopBody*)data;
    body(Range(start, end));
}

ساكن فراغ parallel_for_impl(ثابت cv::Range& range, ثابت cv::ParallelLoopBody& body, مزدوج nstripes)
{
    using نطاق cv::parallel;
    אם ((numThreads < 0 || numThreads > 1) && range.end - range.start > 1)
    {
        ParallelLoopBodyWrapperContext ctx(body, range, nstripes);
        ProxyLoopBody pbody(ctx);
        cv::Range stripeRange = pbody.stripeRange();
        אם( stripeRange.end - stripeRange.start == 1 )
        {
            body(range);
            החזר;
        }

        std::shared_ptr<ParallelForAPI>& api = getCurrentParallelForAPI();
        אם (api)
        {
            CV_CheckEQ(stripeRange.start, 0, "");
            api->parallel_for(stripeRange.end, parallel_for_cb, (فراغ*)&pbody);
            ctx.finalize();  // propagate exceptions if exists
            החזר;
        }

#ifdef CV_PARALLEL_FRAMEWORK
#אם defined HAVE_TBB

#אם TBB_INTERFACE_VERSION >= 8000
        tbbArena.execute(pbody);
#אחרת
        pbody();
#endif

#وإلاإذا defined HAVE_HPX
        pbody();

#وإلاإذا defined HAVE_OPENMP

        #pragma omp parallel עבור schedule(dynamic) num_threads(numThreads > 0 ? numThreads : numThreadsMax)
        עבור (صحيح i = stripeRange.start; i < stripeRange.end; ++i)
            pbody(Range(i, i + 1));

#وإلاإذا defined HAVE_GCD

        dispatch_queue_t concurrent_queue = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0);
        dispatch_apply_f(stripeRange.end - stripeRange.start, concurrent_queue, &pbody, block_function);

#وإلاإذا defined WINRT

        pplParallelFor(stripeRange.start, stripeRange.end, pbody);

#وإلاإذا defined HAVE_CONCURRENCY

        אם(!pplScheduler || pplScheduler->Id() == Concurrency::CurrentScheduler::Id())
        {
            pplParallelFor(stripeRange.start, stripeRange.end, pbody);
        }
        אחרת
        {
            pplScheduler->Attach();
            pplParallelFor(stripeRange.start, stripeRange.end, pbody);
            Concurrency::CurrentScheduler::Detach();
        }

#وإلاإذا defined HAVE_PTHREADS_PF

        parallel_for_pthreads(pbody.stripeRange(), pbody, pbody.stripeRange().size());

#אחרת

#error You have hacked and compiling with unsupported parallel framework

#endif

        ctx.finalize();  // propagate exceptions if exists
        החזר;
#endif // CV_PARALLEL_FRAMEWORK
    }

    body(range);
}


صحيح getNumThreads(فراغ)
{
    std::shared_ptr<ParallelForAPI>& api = getCurrentParallelForAPI();
    אם (api)
    {
        החזר api->getNumThreads();
    }

    אם (numThreads == 0)
        החזר 1;

#אם defined HAVE_TBB

#אם TBB_INTERFACE_VERSION >= 9100
    החזר tbbArena.max_concurrency();
#وإلاإذا TBB_INTERFACE_VERSION >= 8000
    החזר numThreads > 0
        ? numThreads
        : tbb::task_scheduler_init::default_num_threads();
#אחרת
    החזר tbbScheduler.is_active()
           ? numThreads
           : tbb::task_scheduler_init::default_num_threads();
#endif

#وإلاإذا defined HAVE_HPX
    החזר numThreads;

#وإلاإذا defined HAVE_OPENMP

    החזר numThreads > 0
           ? numThreads
           : numThreadsMax;


#وإلاإذا defined HAVE_GCD

    החזר cv::getNumberOfCPUs(); // the GCD thread pool limit

#وإلاإذا defined WINRT

    החזר 0;

#وإلاإذا defined HAVE_CONCURRENCY

    החזר (pplScheduler == 0)
        ? Concurrency::CurrentScheduler::Get()->GetNumberOfVirtualProcessors()
        : (1 + pplScheduler->GetNumberOfVirtualProcessors());

#وإلاإذا defined HAVE_PTHREADS_PF

        החזר parallel_pthreads_get_threads_num();

#אחרת

    החזר 1;

#endif
}

unsigned defaultNumberOfThreads()
{
#ifdef __ANDROID__
    // many modern phones/tables have 4-core CPUs. Let's use no more
    // than 2 threads by default not to overheat the devices
    ثابت unsigned صحيح default_number_of_threads = 2;
#אחרת
    ثابت unsigned صحيح default_number_of_threads = (unsigned صحيح)std::max(1, cv::getNumberOfCPUs());
#endif

    unsigned result = default_number_of_threads;

    ساكن صحيح config_num_threads = (صحيح)utils::getConfigurationParameterSizeT("OPENCV_FOR_THREADS_NUM", 0);

    אם (config_num_threads)
    {
        result = (unsigned)std::max(1, config_num_threads);
        //do we need upper limit of threads number?
    }
    החזר result;
}

فراغ setNumThreads( صحيح threads_ )
{
    CV_UNUSED(threads_);

    صحيح threads = (threads_ < 0) ? defaultNumberOfThreads() : (unsigned)threads_;
    numThreads = threads;

    std::shared_ptr<ParallelForAPI>& api = getCurrentParallelForAPI();
    אם (api)
    {
        api->setNumThreads(numThreads);
    }

#ifdef HAVE_TBB

#אם TBB_INTERFACE_VERSION >= 8000
    אם(tbbArena.is_active()) tbbArena.terminate();
    אם(threads > 0) tbbArena.initialize(threads);
#אחרת
    אם(tbbScheduler.is_active()) tbbScheduler.terminate();
    אם(threads > 0) tbbScheduler.initialize(threads);
#endif

#وإلاإذا defined HAVE_HPX
    החזר; // nothing needed as numThreads is used

#وإلاإذا defined HAVE_OPENMP

    החזר; // nothing needed as num_threads clause is used in #pragma omp parallel for

#وإلاإذا defined HAVE_GCD

    // unsupported
    // there is only private dispatch_queue_set_width() and only for desktop

#وإلاإذا defined WINRT

    החזר;

#وإلاإذا defined HAVE_CONCURRENCY

    אם (threads <= 0)
    {
        pplScheduler = 0;
    }
    אחרת אם (threads == 1)
    {
        // Concurrency always uses >=2 threads, so we just disable it if 1 thread is requested
        numThreads = 0;
    }
    אחרת אם (pplScheduler == 0 || 1 + pplScheduler->GetNumberOfVirtualProcessors() != (unsigned صحيح)threads)
    {
        pplScheduler = Concurrency::Scheduler::Create(Concurrency::SchedulerPolicy(2,
                       Concurrency::MinConcurrency, threads-1,
                       Concurrency::MaxConcurrency, threads-1));
    }

#وإلاإذا defined HAVE_PTHREADS_PF

    parallel_pthreads_set_threads_num(threads);

#endif
}


صحيح getThreadNum()
{
    std::shared_ptr<ParallelForAPI>& api = getCurrentParallelForAPI();
    אם (api)
    {
        החזר api->getThreadNum();
    }

#אם defined HAVE_TBB
    #אם TBB_INTERFACE_VERSION >= 9100
        החזר tbb::this_task_arena::current_thread_index();
    #وإلاإذا TBB_INTERFACE_VERSION >= 8000
        החזר tbb::task_arena::current_thread_index();
    #אחרת
        החזר 0;
    #endif
#وإلاإذا defined HAVE_HPX
    החזר (صحيح)(hpx::get_num_worker_threads());
#وإلاإذا defined HAVE_OPENMP
    החזר omp_get_thread_num();
#وإلاإذا defined HAVE_GCD
    החזר (صحيح)(size_t)(فراغ*)pthread_self(); // no zero-based indexing
#وإلاإذا defined WINRT
    החזר 0;
#وإلاإذا defined HAVE_CONCURRENCY
    החזר std::max(0, (صحيح)Concurrency::Context::VirtualProcessorId()); // zero for master thread, unique number for others but not necessary 1,2,3,...
#وإلاإذا defined HAVE_PTHREADS_PF
    החזר (صحيح)(size_t)(فراغ*)pthread_self(); // no zero-based indexing
#אחרת
    החזר 0;
#endif
}


#אם defined __linux__ || defined __GLIBC__ || defined __HAIKU__ || defined __ANDROID__
  #define CV_CPU_GROUPS_1
#endif

#אם defined __linux__ || defined __ANDROID__
  #define CV_HAVE_CGROUPS 1
#endif

#אם defined CV_CPU_GROUPS_1
ساكن مضمن
std::string getFileContents(ثابت حرف *filename)
{
    std::ifstream ifs(filename);
    אם (!ifs.is_open())
        החזר std::string();

    std::string content( (std::istreambuf_iterator<حرف>(ifs) ),
                         (std::istreambuf_iterator<حرف>()    ) );

    אם (ifs.fail())
        החזר std::string();

    החזר content;
}

ساكن مضمن
صحيح getNumberOfCPUsImpl(ثابت حرف *filename)
{
   std::string file_contents = getFileContents(filename);
   אם(file_contents.empty())
       החזר 0;

   حرف *pbuf = const_cast<حرف*>(file_contents.c_str());
   //parse string of form "0-1,3,5-7,10,13-15"
   صحيح cpusAvailable = 0;

   כלעוד(*pbuf)
   {
      ثابت حرف* pos = pbuf;
      منطقي range = שקר;
      כלעוד(*pbuf && *pbuf != ',')
      {
          אם(*pbuf == '-') range = אמת;
          ++pbuf;
      }
      אם(*pbuf) *pbuf++ = 0;
      אם(!range)
        ++cpusAvailable;
      אחרת
      {
          صحيح rstart = 0, rend = 0;
          sscanf(pos, "%d-%d", &rstart, &rend);
          cpusAvailable += rend - rstart + 1;
      }

   }
   החזר cpusAvailable;
}
#endif

#אם defined CV_HAVE_CGROUPS
ساكن مضمن
unsigned getNumberOfCPUsCFSv2()
{
    صحيح cfs_quota = 0;
    صحيح cfs_period = 0;

    std::ifstream ss_cpu_max("/sys/fs/cgroup/cpu.max", std::ios::in | std::ios::binary);
    ss_cpu_max >> cfs_quota >> cfs_period;

    אם (ss_cpu_max.fail() || cfs_quota < 1 || cfs_period < 1) /* values must not be 0 or negative */
        החזר 0;

    החזר (unsigned)max(1, cfs_quota/cfs_period);
}

ساكن مضمن
unsigned getNumberOfCPUsCFSv1()
{
    صحيح cfs_quota = 0;
    {
        std::ifstream ss_period("/sys/fs/cgroup/cpu/cpu.cfs_quota_us", std::ios::in | std::ios::binary);
        ss_period >> cfs_quota;

        אם (ss_period.fail() || cfs_quota < 1) /* cfs_quota must not be 0 or negative */
            החזר 0;
    }

    صحيح cfs_period = 0;
    {
        std::ifstream ss_quota("/sys/fs/cgroup/cpu/cpu.cfs_period_us", std::ios::in | std::ios::binary);
        ss_quota >> cfs_period;

        אם (ss_quota.fail() || cfs_period < 1)
            החזר 0;
    }

    החזר (unsigned)max(1, cfs_quota/cfs_period);
}
#endif

template <typename T> ساكن مضمن
T minNonZero(ثابت T& val_1, ثابت T& val_2)
{
    אם ((val_1 != 0) && (val_2 != 0))
        החזר std::min(val_1, val_2);
    החזר (val_1 != 0) ? val_1 : val_2;
}

#ifndef OPENCV_DISABLE_THREAD_SUPPORT
ساكن
صحيح getNumberOfCPUs_()
{
#ifndef OPENCV_SEMIHOSTING
    /*
     * Logic here is to try different methods of getting CPU counts and return
     * the minimum most value as it has high probablity of being right and safe.
     * Return 1 if we get 0 or not found on all methods.
    */
#אם !defined(__MINGW32__) /* not implemented (2020-03) */

    /*
     * Check for this standard C++11 way, we do not return directly because
     * running in a docker or K8s environment will mean this is the host
     * machines config not the containers or pods and as per docs this value
     * must be "considered only a hint".
    */
    unsigned ncpus = std::thread::hardware_concurrency(); /* If the value is not well defined or not computable, returns 0 */
#אחרת
    unsigned ncpus = 0; /* 0 means we have to find out some other way */
#endif

#אם defined _WIN32

    SYSTEM_INFO sysinfo = {};
#אם (defined(_M_ARM) || defined(_M_ARM64) || defined(_M_ARM64EC) || defined(_M_X64) || defined(WINRT)) && _WIN32_WINNT >= 0x501
    GetNativeSystemInfo( &sysinfo );
#אחרת
    GetSystemInfo( &sysinfo );
#endif
    unsigned ncpus_sysinfo = sysinfo.dwNumberOfProcessors;
    ncpus = minNonZero(ncpus, ncpus_sysinfo);

#وإلاإذا defined __APPLE__

    صحيح numCPU=0;
    صحيح mib[4];
    size_t len = حجم(numCPU);

    /* set the mib for hw.ncpu */
    mib[0] = CTL_HW;
    mib[1] = HW_AVAILCPU;  // alternatively, try HW_NCPU;

    /* get the number of CPUs from the system */
    sysctl(mib, 2, &numCPU, &len, عدم, 0);

    אם( numCPU < 1 )
    {
        mib[1] = HW_NCPU;
        sysctl( mib, 2, &numCPU, &len, عدم, 0 );

        אם( numCPU < 1 )
            numCPU = 1;
    }

    ncpus = minNonZero(ncpus, (unsigned)numCPU);

#وإلاإذا defined CV_CPU_GROUPS_1

#אם defined CV_HAVE_CGROUPS
    ساكن unsigned ncpus_impl_cpuset = (unsigned)getNumberOfCPUsImpl("/sys/fs/cgroup/cpuset/cpuset.cpus");
    ncpus = minNonZero(ncpus, ncpus_impl_cpuset);

    ساكن unsigned ncpus_impl_cfs_v1 = getNumberOfCPUsCFSv1();
    ncpus = minNonZero(ncpus, ncpus_impl_cfs_v1);

    ساكن unsigned ncpus_impl_cfs_v2 = getNumberOfCPUsCFSv2();
    ncpus = minNonZero(ncpus, ncpus_impl_cfs_v2);
#endif

    ساكن unsigned ncpus_impl_devices = (unsigned)getNumberOfCPUsImpl("/sys/devices/system/cpu/online");
    ncpus = minNonZero(ncpus, ncpus_impl_devices);

#endif

#אם defined _GNU_SOURCE \
    && !defined(__MINGW32__) /* not implemented (2020-03) */ \
    && !defined(__EMSCRIPTEN__) \
    && !defined(__ANDROID__)  // TODO: add check for modern Android NDK

    cpu_set_t cpu_set;
    אם (0 == sched_getaffinity(0, حجم(cpu_set), &cpu_set))
    {
        unsigned cpu_count_cpu_set = CPU_COUNT(&cpu_set);
        ncpus = minNonZero(ncpus, cpu_count_cpu_set);
    }

#endif

#אם !defined(_WIN32) && !defined(__APPLE__) && defined(_SC_NPROCESSORS_ONLN)

    ساكن unsigned cpu_count_sysconf = (unsigned)sysconf( _SC_NPROCESSORS_ONLN );
    ncpus = minNonZero(ncpus, cpu_count_sysconf);
#وإلاإذا defined (__QNX__)
    ساكن unsigned cpu_count_sysconf = _syspage_ptr->num_cpu;
    ncpus = minNonZero(ncpus, cpu_count_sysconf);
#endif

    החזר ncpus != 0 ? ncpus : 1;
#אחרת //  OPENCV_SEMIHOSTING
    החזר 1;
#endif //OPENCV_SEMIHOSTING
}

صحيح getNumberOfCPUs()
{
    ساكن صحيح nCPUs = getNumberOfCPUs_();
    החזר nCPUs;  // cached value
}

#אחרת  // OPENCV_DISABLE_THREAD_SUPPORT
صحيح getNumberOfCPUs()
{
    החזר 1;
}
#endif  // OPENCV_DISABLE_THREAD_SUPPORT

ثابت حرف* currentParallelFramework()
{
    std::shared_ptr<ParallelForAPI>& api = getCurrentParallelForAPI();
    אם (api)
    {
        החזר api->getName();
    }
#ifdef CV_PARALLEL_FRAMEWORK
    החזר CV_PARALLEL_FRAMEWORK;
#אחרת
    החזר عدم;
#endif
}

}  // namespace cv::
