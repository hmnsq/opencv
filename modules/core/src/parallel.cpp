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

#ਅਨ੍ਤਰ੍ਭਾਵ "precomp.hpp"

#ਅਨ੍ਤਰ੍ਭਾਵ <opencv2/core/utils/configuration.private.hpp>
#ਅਨ੍ਤਰ੍ਭਾਵ <opencv2/core/utils/trace.private.hpp>

#ਅਨ੍ਤਰ੍ਭਾਵ "opencv2/core/parallel/parallel_backend.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "parallel/parallel.hpp"

#ਜੇ defined _WIN32 || defined WINCE
    #ਅਨ੍ਤਰ੍ਭਾਵ <windows.h>
    #ਪਰਿਭਾਸ਼ਾ_ਰੱਦ small
    #ਪਰਿਭਾਸ਼ਾ_ਰੱਦ min
    #ਪਰਿਭਾਸ਼ਾ_ਰੱਦ max
    #ਪਰਿਭਾਸ਼ਾ_ਰੱਦ ਨਿਰਪੇਖ
#ਜੇ_ਅੰਤ

#ਜੇ defined __unix__ || defined __APPLE__ || defined __GLIBC__ \
    || defined __HAIKU__ || defined __EMSCRIPTEN__ \
    || defined __FreeBSD__ || defined __NetBSD__ || defined __OpenBSD__
    #ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
    #ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
    #ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
    #ਅਨ੍ਤਰ੍ਭਾਵ <fstream>
    #ਜੇ defined __ANDROID__
        #ਅਨ੍ਤਰ੍ਭਾਵ <sys/sysconf.h>
        #ਅਨ੍ਤਰ੍ਭਾਵ <sys/syscall.h>
        #ਅਨ੍ਤਰ੍ਭਾਵ <sched.h>
    #ਜਾਂਜੇ defined __APPLE__
        #ਅਨ੍ਤਰ੍ਭਾਵ <sys/sysctl.h>
    #ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ

#ਜੇ defined (__QNX__)
    #ਅਨ੍ਤਰ੍ਭਾਵ <sys/syspage.h>
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OPENCV_DISABLE_THREAD_SUPPORT
    #ਅਨ੍ਤਰ੍ਭਾਵ <thread>
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ _OPENMP
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HAVE_OPENMP
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __APPLE__
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HAVE_ਮਹੱਤਮ_ਸਾਂਝਾ_ਭਾਜਕ
#ਜੇ_ਅੰਤ

#ਜੇ defined _MSC_VER
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HAVE_CONCURRENCY
#ਜੇ_ਅੰਤ

/* IMPORTANT: always use the same order of defines
   - HAVE_TBB         - 3rdparty library, should be explicitly enabled
   - HAVE_HPX         - 3rdparty library, should be explicitly enabled
   - HAVE_OPENMP      - integrated to compiler, should be explicitly enabled
   - HAVE_GCD         - system wide, used automatically        (APPLE only)
   - WINRT            - system wide, used automatically        (Windows RT only)
   - HAVE_CONCURRENCY - part of runtime, used automatically    (Windows only - MSVS 10, MSVS 11)
   - HAVE_PTHREADS_PF - pthreads if available
*/

#ਜੇ defined HAVE_TBB
    #ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ TBB_SUPPRESS_DEPRECATED_MESSAGES  // supress warning
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TBB_SUPPRESS_DEPRECATED_MESSAGES 1
    #ਜੇ_ਅੰਤ
    #ਅਨ੍ਤਰ੍ਭਾਵ "tbb/tbb.h"
    #ਅਨ੍ਤਰ੍ਭਾਵ "tbb/task.h"
    #ਜੇ TBB_INTERFACE_ਸੰਸਕਰਣ >= 8000
        #ਅਨ੍ਤਰ੍ਭਾਵ "tbb/task_arena.h"
    #ਜੇ_ਅੰਤ
    #ਪਰਿਭਾਸ਼ਾ_ਰੱਦ min
    #ਪਰਿਭਾਸ਼ਾ_ਰੱਦ max
#ਜਾਂਜੇ defined HAVE_HPX
    #ਅਨ੍ਤਰ੍ਭਾਵ <hpx/parallel/algorithms/for_loop.hpp>
    #ਅਨ੍ਤਰ੍ਭਾਵ <hpx/parallel/execution.hpp>
    //
    #ਅਨ੍ਤਰ੍ਭਾਵ <hpx/hpx_start.hpp>
    #ਅਨ੍ਤਰ੍ਭਾਵ <hpx/hpx_suspend.hpp>
    #ਅਨ੍ਤਰ੍ਭਾਵ <hpx/include/apply.hpp>
    #ਅਨ੍ਤਰ੍ਭਾਵ <hpx/util/yield_while.hpp>
    #ਅਨ੍ਤਰ੍ਭਾਵ <hpx/include/threadmanager.hpp>

#ਜਾਂਜੇ defined HAVE_OPENMP
    #ਅਨ੍ਤਰ੍ਭਾਵ <omp.h>
#ਜਾਂਜੇ defined HAVE_ਮਹੱਤਮ_ਸਾਂਝਾ_ਭਾਜਕ
    #ਅਨ੍ਤਰ੍ਭਾਵ <dispatch/dispatch.h>
    #ਅਨ੍ਤਰ੍ਭਾਵ <pthread.h>
#ਜਾਂਜੇ defined HAVE_CONCURRENCY
    #ਅਨ੍ਤਰ੍ਭਾਵ <ppl.h>
#ਜਾਂਜੇ defined HAVE_PTHREADS_PF
    #ਅਨ੍ਤਰ੍ਭਾਵ <pthread.h>
#ਜੇ_ਅੰਤ


#ਜੇ defined HAVE_TBB
#  define CV_PARALLEL_FRAMEWORK "tbb"
#ਜਾਂਜੇ defined HAVE_HPX
#  define CV_PARALLEL_FRAMEWORK "hpx"
#ਜਾਂਜੇ defined HAVE_OPENMP
#  define CV_PARALLEL_FRAMEWORK "openmp"
#ਜਾਂਜੇ defined HAVE_ਮਹੱਤਮ_ਸਾਂਝਾ_ਭਾਜਕ
#  define CV_PARALLEL_FRAMEWORK "gcd"
#ਜਾਂਜੇ defined WINRT
#  define CV_PARALLEL_FRAMEWORK "winrt-concurrency"
#ਜਾਂਜੇ defined HAVE_CONCURRENCY
#  define CV_PARALLEL_FRAMEWORK "ms-concurrency"
#ਜਾਂਜੇ defined HAVE_PTHREADS_PF
#  define CV_PARALLEL_FRAMEWORK "pthreads"
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ <atomic>

#ਅਨ੍ਤਰ੍ਭਾਵ "parallel_impl.hpp"

#ਅਨ੍ਤਰ੍ਭਾਵ "opencv2/core/detail/exception_ptr.hpp"  // CV__EXCEPTION_PTR = 1 if std::exception_ptr is available

#ਅਨ੍ਤਰ੍ਭਾਵ <opencv2/core/utils/fp_control_utils.hpp>
#ਅਨ੍ਤਰ੍ਭਾਵ <opencv2/core/utils/fp_control.private.hpp>

ਵਰਤੋਂ ਨਾਮਾਕਾਸ਼ cv;

ਨਾਮਾਕਾਸ਼ cv {

ParallelLoopBody::~ParallelLoopBody() {}

ਵਰਤੋਂ ਨਾਮਾਕਾਸ਼ cv::parallel;

ਨਾਮਾਕਾਸ਼ {

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ENABLE_INSTRUMENTATION
    ਸਥਿਰ ਖਾਲੀ SyncNodes(cv::instr::Instr_ਗੰਢ *p_ਗੰਢ)
    {
        std::vector<cv::instr::ਗੰਢ_ਅੰਕੜੇ_Tls*> ਅੰਕੜੇ;
        p_ਗੰਢ->m_payload.m_tls.gather(ਅੰਕੜੇ);

        uint64 ticksMax = 0;
        ਪੂਰਨ    ਧਾਗੇ  = 0;
        ਲਈ(size_t i = 0; i < ਅੰਕੜੇ.size(); i++)
        {
            ਜੇ(ਅੰਕੜੇ[i] && ਅੰਕੜੇ[i]->m_ticks_ਕੁੱਲ)
            {
                ticksMax = MAX(ticksMax, ਅੰਕੜੇ[i]->m_ticks_ਕੁੱਲ);
                p_ਗੰਢ->m_payload.m_ticks_ਕੁੱਲ -= ਅੰਕੜੇ[i]->m_ticks_ਕੁੱਲ;
                ਅੰਕੜੇ[i]->m_ticks_ਕੁੱਲ = 0;
                ਧਾਗੇ++;
            }
        }
        p_ਗੰਢ->m_payload.m_ticks_ਕੁੱਲ += ticksMax;
        p_ਗੰਢ->m_payload.m_ਧਾਗੇ = MAX(p_ਗੰਢ->m_payload.m_ਧਾਗੇ, ਧਾਗੇ);

        ਲਈ(size_t i = 0; i < p_ਗੰਢ->m_childs.size(); i++)
            SyncNodes(p_ਗੰਢ->m_childs[i]);
    }
#ਜੇ_ਅੰਤ

    ਵਰਗ ParallelLoopBodyWrapperContext
    {
    ਜਨਤਕ:
        ParallelLoopBodyWrapperContext(ਅਚਲ cv::ParallelLoopBody& _body, ਅਚਲ cv::Range& _r, ਦੁੱਗਣਾ _nstripes) :
            is_rng_ਵਰਤਿਆ(ਝੂਠ), hasException(ਝੂਠ)
        {

            body = &_body;
            ਸਾਰਾ_Range = _r;
            ਦੁੱਗਣਾ len = ਸਾਰਾ_Range.end - ਸਾਰਾ_Range.ਸ਼ੁਰੂ;
            nstripes = cv_ਗੇੜਾ(_nstripes <= 0 ? len : MIN(MAX(_nstripes, 1.), len));

            // propagate main thread state
            rng = cv::theRNG();
#ਜੇ OPENCV_SUPPORTS_FP_DENORMALS_HINT && OPENCV_IMPL_FP_HINTS
            details::saveFPDenormalsState(fp_denormals_ਅਧਾਰ_state);
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ OPENCV_TRACE
            trace_ਜੜ੍ਹ_Region = CV_TRACE_NS::details::getCurrentRegion();
            trace_ਜੜ੍ਹ_Context = CV_TRACE_NS::details::getTraceManager().tls.get();
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ENABLE_INSTRUMENTATION
            p_Thread_ਜੜ੍ਹ = cv::instr::getInstrumentTLSStruct().p_Current_ਗੰਢ;
#ਜੇ_ਅੰਤ
        }
        ਖਾਲੀ finalize()
        {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ENABLE_INSTRUMENTATION
            ਲਈ(size_t i = 0; i < p_Thread_ਜੜ੍ਹ->m_childs.size(); i++)
                SyncNodes(p_Thread_ਜੜ੍ਹ->m_childs[i]);
#ਜੇ_ਅੰਤ
            ਜੇ (is_rng_ਵਰਤਿਆ)
            {
                // Some parallel backends execute nested jobs in the main thread,
                // so we need to restore initial RNG state here.
                cv::theRNG() = rng;
                // We can't properly update RNG state based on RNG usage in worker threads,
                // so lets just change main thread RNG state to the next value.
                // Note: this behaviour is not equal to single-threaded mode.
                cv::theRNG().next();
            }
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ OPENCV_TRACE
            ਜੇ (trace_ਜੜ੍ਹ_Region)
                CV_TRACE_NS::details::parallelForFinalize(*trace_ਜੜ੍ਹ_Region);
#ਜੇ_ਅੰਤ

            ਜੇ (hasException)
            {
#ਜੇ CV__EXCEPTION_PTR
                std::rethrow_exception(pException);
#ਨਹੀਂਤਾਂ
                CV_Error(Error::StsError, "Exception in parallel_for() body: " + exception_ਸੁਨੇਹਾ);
#ਜੇ_ਅੰਤ
            }
        }
        ~ParallelLoopBodyWrapperContext() {}

        ਅਚਲ cv::ParallelLoopBody* body;
        cv::Range ਸਾਰਾ_Range;
        ਪੂਰਨ nstripes;
        cv::RNG rng;
        ਬਦਲਣਯੋਗ ਤਾਰਕਿਕ is_rng_ਵਰਤਿਆ;
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ OPENCV_TRACE
        CV_TRACE_NS::details::Region* trace_ਜੜ੍ਹ_Region;
        CV_TRACE_NS::details::TraceManagerThreadLocal* trace_ਜੜ੍ਹ_Context;
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ENABLE_INSTRUMENTATION
        cv::instr::Instr_ਗੰਢ *p_Thread_ਜੜ੍ਹ;
#ਜੇ_ਅੰਤ
        ਤਾਰਕਿਕ hasException;
#ਜੇ CV__EXCEPTION_PTR
        std::exception_ptr pException;
#ਨਹੀਂਤਾਂ
        cv::String exception_ਸੁਨੇਹਾ;
#ਜੇ_ਅੰਤ
#ਜੇ CV__EXCEPTION_PTR
        ਖਾਲੀ ਰਿਕਾਰਡ_Exception()
#ਨਹੀਂਤਾਂ
        ਖਾਲੀ ਰਿਕਾਰਡ_Exception(ਅਚਲ cv::String& ਸੁਨੇਹਾ)
#ਜੇ_ਅੰਤ
        {
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ CV_THREAD_SANITIZER
            ਜੇ (!hasException)
#ਜੇ_ਅੰਤ
            {
                cv::Auto_ਤਾਲਾ lock(cv::getInitializationMutex());
                ਜੇ (!hasException)
                {
                    hasException = ਸੱਚ;
#ਜੇ CV__EXCEPTION_PTR
                    pException = std::current_exception();
#ਨਹੀਂਤਾਂ
                    exception_ਸੁਨੇਹਾ = ਸੁਨੇਹਾ;
#ਜੇ_ਅੰਤ
                }
            }
        }

#ਜੇ OPENCV_SUPPORTS_FP_DENORMALS_HINT && OPENCV_IMPL_FP_HINTS
        details::FPDenormals_ਬਹੁਲਕ_State fp_denormals_ਅਧਾਰ_state;
#ਜੇ_ਅੰਤ

    ਨਿਜੀ:
        ParallelLoopBodyWrapperContext(ਅਚਲ ParallelLoopBodyWrapperContext&); // disabled
        ParallelLoopBodyWrapperContext& ਸੰਚਾਲਕ=(ਅਚਲ ParallelLoopBodyWrapperContext&); // disabled
    };

    ਵਰਗ ParallelLoopBodyWrapper : ਜਨਤਕ cv::ParallelLoopBody
    {
    ਜਨਤਕ:
        ParallelLoopBodyWrapper(ParallelLoopBodyWrapperContext& ctx_) :
            ctx(ctx_)
        {
        }
        ~ParallelLoopBodyWrapper()
        {
        }
        ਖਾਲੀ ਸੰਚਾਲਕ()(ਅਚਲ cv::Range& sr) ਅਚਲ CV_OVERRIDE
        {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ OPENCV_TRACE
            // TODO CV_TRACE_NS::details::setCurrentRegion(rootRegion);
            ਜੇ (ctx.trace_ਜੜ੍ਹ_Region && ctx.trace_ਜੜ੍ਹ_Context)
                CV_TRACE_NS::details::parallel_For_ਸੈੱਟ_ਜੜ੍ਹ_Region(*ctx.trace_ਜੜ੍ਹ_Region, *ctx.trace_ਜੜ੍ਹ_Context);
            CV__TRACE_OPENCV_FUNCTION_NAME("parallel_for_body");
            ਜੇ (ctx.trace_ਜੜ੍ਹ_Region)
                CV_TRACE_NS::details::parallelForAttachNestedRegion(*ctx.trace_ਜੜ੍ਹ_Region);
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ENABLE_INSTRUMENTATION
            {
                cv::instr::InstrTLSStruct *pInstrTLS = &cv::instr::getInstrumentTLSStruct();
                pInstrTLS->p_Current_ਗੰਢ = ctx.p_Thread_ਜੜ੍ਹ; // Initialize TLS node for thread
            }
            CV_INSTRUMENT_REGION();
#ਜੇ_ਅੰਤ

            // propagate main thread state
            cv::theRNG() = ctx.rng;
#ਜੇ OPENCV_SUPPORTS_FP_DENORMALS_HINT && OPENCV_IMPL_FP_HINTS
            FPDenormalsIgnoreHintScope fp_denormals_scope(ctx.fp_denormals_ਅਧਾਰ_state);
#ਜੇ_ਅੰਤ

            cv::Range r;
            cv::Range ਸਾਰਾ_Range = ctx.ਸਾਰਾ_Range;
            ਪੂਰਨ nstripes = ctx.nstripes;
            r.ਸ਼ੁਰੂ = (ਪੂਰਨ)(ਸਾਰਾ_Range.ਸ਼ੁਰੂ +
                            ((uint64)sr.ਸ਼ੁਰੂ*(ਸਾਰਾ_Range.end - ਸਾਰਾ_Range.ਸ਼ੁਰੂ) + nstripes/2)/nstripes);
            r.end = sr.end >= nstripes ? ਸਾਰਾ_Range.end : (ਪੂਰਨ)(ਸਾਰਾ_Range.ਸ਼ੁਰੂ +
                            ((uint64)sr.end*(ਸਾਰਾ_Range.end - ਸਾਰਾ_Range.ਸ਼ੁਰੂ) + nstripes/2)/nstripes);

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ OPENCV_TRACE
            CV_TRACE_ARG_ਮੁੱਲ(range_ਸ਼ੁਰੂ, "range.start", (int64)r.ਸ਼ੁਰੂ);
            CV_TRACE_ARG_ਮੁੱਲ(range_ਸਿਰਾ, "range.end", (int64)r.end);
#ਜੇ_ਅੰਤ

            ਕੋਸ਼ਿਸ਼
            {
                (*ctx.body)(r);
            }
#ਜੇ CV__EXCEPTION_PTR
            ਫੜੋ (...)
            {
                ctx.ਰਿਕਾਰਡ_Exception();
            }
#ਨਹੀਂਤਾਂ
            ਫੜੋ (ਅਚਲ cv::Exception& e)
            {
                ctx.ਰਿਕਾਰਡ_Exception(e.what());
            }
            ਫੜੋ (ਅਚਲ std::exception& e)
            {
                ctx.ਰਿਕਾਰਡ_Exception(e.what());
            }
            ਫੜੋ (...)
            {
                ctx.ਰਿਕਾਰਡ_Exception("Unknown exception");
            }
#ਜੇ_ਅੰਤ

            ਜੇ (!ctx.is_rng_ਵਰਤਿਆ && !(cv::theRNG() == ctx.rng))
                ctx.is_rng_ਵਰਤਿਆ = ਸੱਚ;
        }
        cv::Range stripeRange() ਅਚਲ { ਵਾਪਸ cv::Range(0, ctx.nstripes); }

    ਸੁਰੱਖਿਅਤ:
        ParallelLoopBodyWrapperContext& ctx;
    };

#ਜੇ defined HAVE_TBB
    ਵਰਗ ProxyLoopBody : ਜਨਤਕ ParallelLoopBodyWrapper
    {
    ਜਨਤਕ:
        ProxyLoopBody(ParallelLoopBodyWrapperContext& ctx_)
        : ParallelLoopBodyWrapper(ctx_)
        {}

        ਖਾਲੀ ਸੰਚਾਲਕ ()(ਅਚਲ tbb::blocked_range<ਪੂਰਨ>& range) ਅਚਲ
        {
            ਇਹ->ParallelLoopBodyWrapper::ਸੰਚਾਲਕ()(cv::Range(range.begin(), range.end()));
        }

        ਖਾਲੀ ਸੰਚਾਲਕ ()() ਅਚਲ  // run parallel job
        {
            cv::Range range = ਇਹ->stripeRange();
            tbb::parallel_for(tbb::blocked_range<ਪੂਰਨ>(range.ਸ਼ੁਰੂ, range.end), *ਇਹ);
        }
    };
#ਜਾਂਜੇ defined HAVE_HPX
    ਵਰਗ ProxyLoopBody : ਜਨਤਕ ParallelLoopBodyWrapper
    {
    ਜਨਤਕ:
        ProxyLoopBody(ParallelLoopBodyWrapperContext& ctx_)
                : ParallelLoopBodyWrapper(ctx_)
        {}

        ਖਾਲੀ ਸੰਚਾਲਕ ()() ਅਚਲ  // run parallel job
        {
            cv::Range stripeRange = ਇਹ->stripeRange();
            hpx::parallel::for_loop(
                    hpx::parallel::execution::par,
                    stripeRange.ਸ਼ੁਰੂ, stripeRange.end,
                    [&](ਅਚਲ ਪੂਰਨ &i) { ;
                        ਇਹ->ParallelLoopBodyWrapper::ਸੰਚਾਲਕ()(
                                cv::Range(i, i + 1));
                    });
        }
    };
#ਜਾਂਜੇ defined HAVE_OPENMP
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ParallelLoopBodyWrapper ProxyLoopBody;
#ਜਾਂਜੇ defined HAVE_ਮਹੱਤਮ_ਸਾਂਝਾ_ਭਾਜਕ
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ParallelLoopBodyWrapper ProxyLoopBody;
    ਸਥਿਰ ਖਾਲੀ ਖੰਡ_function(ਖਾਲੀ* context, size_t ਸੂਚਕਾਂਕ)
    {
        ProxyLoopBody* ptr_body = static_cast<ProxyLoopBody*>(context);
        (*ptr_body)(cv::Range((ਪੂਰਨ)ਸੂਚਕਾਂਕ, (ਪੂਰਨ)ਸੂਚਕਾਂਕ + 1));
    }
#ਜਾਂਜੇ defined WINRT || defined HAVE_CONCURRENCY
    ਵਰਗ ProxyLoopBody : ਜਨਤਕ ParallelLoopBodyWrapper
    {
    ਜਨਤਕ:
        ProxyLoopBody(ParallelLoopBodyWrapperContext& ctx)
        : ParallelLoopBodyWrapper(ctx)
        {}

        ਖਾਲੀ ਸੰਚਾਲਕ ()(ਪੂਰਨ i) ਅਚਲ
        {
            ਇਹ->ParallelLoopBodyWrapper::ਸੰਚਾਲਕ()(cv::Range(i, i + 1));
        }
    };

    // PPL's default auto_partitioner silently skips whole chunks of the range on
    // ARM64 (unfixed MSVC bug, developercommunity.visualstudio.com/t/1027444).
    ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Body>
    ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ pplParallelFor(ਪੂਰਨ first, ਪੂਰਨ ਆਖਰੀ, ਅਚਲ Body& body)
    {
#ਜੇ defined(_M_ARM64) || defined(_M_ARM64EC)
        Concurrency::parallel_for(first, ਆਖਰੀ, body, Concurrency::static_partitioner());
#ਨਹੀਂਤਾਂ
        Concurrency::parallel_for(first, ਆਖਰੀ, body);
#ਜੇ_ਅੰਤ
    }
#ਨਹੀਂਤਾਂ
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ParallelLoopBodyWrapper ProxyLoopBody;
#ਜੇ_ਅੰਤ

#ਜੇ defined HAVE_TBB
    #ਜੇ TBB_INTERFACE_ਸੰਸਕਰਣ >= 8000
        ਸਥਿਰ tbb::task_arena tbbArena(tbb::task_arena::automatic);
    #ਨਹੀਂਤਾਂ
        ਸਥਿਰ tbb::task_scheduler_init tbbScheduler(tbb::task_scheduler_init::deferred);
    #ਜੇ_ਅੰਤ
#ਜਾਂਜੇ defined HAVE_HPX
// nothing for HPX
#ਜਾਂਜੇ defined HAVE_OPENMP
ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ init_Max_ਧਾਗੇ()
{
    ਪੂਰਨ max_ਧਾਗੇ = omp_get_max_ਧਾਗੇ();
    ਵਾਪਸ max_ਧਾਗੇ;
}
ਸਥਿਰ ਪੂਰਨ ਅੰਕ_ਧਾਗੇ_Max = init_Max_ਧਾਗੇ();
#ਜਾਂਜੇ defined HAVE_ਮਹੱਤਮ_ਸਾਂਝਾ_ਭਾਜਕ
// nothing for GCD
#ਜਾਂਜੇ defined WINRT
// nothing for WINRT
#ਜਾਂਜੇ defined HAVE_CONCURRENCY

ਵਰਗ SchedPtr
{
    Concurrency::Scheduler* sched_;
ਜਨਤਕ:
    Concurrency::Scheduler* ਸੰਚਾਲਕ->() { ਵਾਪਸ sched_; }
    ਸੰਚਾਲਕ Concurrency::Scheduler*() { ਵਾਪਸ sched_; }

    ਖਾਲੀ ਸੰਚਾਲਕ=(Concurrency::Scheduler* sched)
    {
        ਜੇ (sched_) sched_->Release();
        sched_ = sched;
    }

    SchedPtr() : sched_(0) {}
    ~SchedPtr() {}
};
ਸਥਿਰ SchedPtr pplScheduler;

#ਜੇ_ਅੰਤ

} // namespace anon

/* ================================   parallel_for_  ================================ */

ਸਥਿਰ ਖਾਲੀ parallel_for_impl(ਅਚਲ cv::Range& range, ਅਚਲ cv::ParallelLoopBody& body, ਦੁੱਗਣਾ nstripes); // forward declaration

ਖਾਲੀ parallel_for_(ਅਚਲ cv::Range& range, ਅਚਲ cv::ParallelLoopBody& body, ਦੁੱਗਣਾ nstripes)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ OPENCV_TRACE
    CV__TRACE_OPENCV_FUNCTION_NAME_("parallel_for", 0);
    CV_TRACE_ARG_ਮੁੱਲ(range_ਸ਼ੁਰੂ, "range.start", (int64)range.ਸ਼ੁਰੂ);
    CV_TRACE_ARG_ਮੁੱਲ(range_ਸਿਰਾ, "range.end", (int64)range.end);
    CV_TRACE_ARG_ਮੁੱਲ(nstripes, "nstripes", (int64)nstripes);
#ਜੇ_ਅੰਤ

    CV_INSTRUMENT_REGION_MT_FORK();
    ਜੇ (range.empty())
        ਵਾਪਸ;

    ਸਥਿਰ std::atomic<ਤਾਰਕਿਕ> flagNestedParallelFor(ਝੂਠ);
    ਤਾਰਕਿਕ isNotNestedRegion = !flagNestedParallelFor.load();
    ਜੇ (isNotNestedRegion)
      isNotNestedRegion = !flagNestedParallelFor.exchange(ਸੱਚ);
    ਜੇ (isNotNestedRegion)
    {
        ਕੋਸ਼ਿਸ਼
        {
            parallel_for_impl(range, body, nstripes);
            flagNestedParallelFor = ਝੂਠ;
        }
        ਫੜੋ (...)
        {
            flagNestedParallelFor = ਝੂਠ;
            ਸੁੱਟੋ;
        }
    }
    ਨਹੀਂਤਾਂ // nested parallel_for_() calls are not parallelized
    {
        CV_ਅਣਵਰਤਿਆ(nstripes);
        body(range);
    }
}

ਸਥਿਰ
ਖਾਲੀ parallel_for_cb(ਪੂਰਨ ਸ਼ੁਰੂ, ਪੂਰਨ end, ਖਾਲੀ* ਅੰਕੜੇ)
{
    CV_DbgAssert(ਅੰਕੜੇ);
    ਅਚਲ cv::ParallelLoopBody& body = *(ਅਚਲ cv::ParallelLoopBody*)ਅੰਕੜੇ;
    body(Range(ਸ਼ੁਰੂ, end));
}

ਸਥਿਰ ਖਾਲੀ parallel_for_impl(ਅਚਲ cv::Range& range, ਅਚਲ cv::ParallelLoopBody& body, ਦੁੱਗਣਾ nstripes)
{
    ਵਰਤੋਂ ਨਾਮਾਕਾਸ਼ cv::parallel;
    ਜੇ ((ਅੰਕ_ਧਾਗੇ < 0 || ਅੰਕ_ਧਾਗੇ > 1) && range.end - range.ਸ਼ੁਰੂ > 1)
    {
        ParallelLoopBodyWrapperContext ctx(body, range, nstripes);
        ProxyLoopBody pbody(ctx);
        cv::Range stripeRange = pbody.stripeRange();
        ਜੇ( stripeRange.end - stripeRange.ਸ਼ੁਰੂ == 1 )
        {
            body(range);
            ਵਾਪਸ;
        }

        std::shared_ptr<ParallelForAPI>& api = getCurrentParallelForAPI();
        ਜੇ (api)
        {
            CV_CheckEQ(stripeRange.ਸ਼ੁਰੂ, 0, "");
            api->parallel_for(stripeRange.end, parallel_for_cb, (ਖਾਲੀ*)&pbody);
            ctx.finalize();  // propagate exceptions if exists
            ਵਾਪਸ;
        }

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CV_PARALLEL_FRAMEWORK
#ਜੇ defined HAVE_TBB

#ਜੇ TBB_INTERFACE_ਸੰਸਕਰਣ >= 8000
        tbbArena.execute(pbody);
#ਨਹੀਂਤਾਂ
        pbody();
#ਜੇ_ਅੰਤ

#ਜਾਂਜੇ defined HAVE_HPX
        pbody();

#ਜਾਂਜੇ defined HAVE_OPENMP

        #ਹਦਾਇਤ omp parallel ਲਈ ਸਮਾਂ_ਸਾਰਣੀ(dynamic) ਅੰਕ_ਧਾਗੇ_2(ਅੰਕ_ਧਾਗੇ > 0 ? ਅੰਕ_ਧਾਗੇ : ਅੰਕ_ਧਾਗੇ_Max)
        ਲਈ (ਪੂਰਨ i = stripeRange.ਸ਼ੁਰੂ; i < stripeRange.end; ++i)
            pbody(Range(i, i + 1));

#ਜਾਂਜੇ defined HAVE_ਮਹੱਤਮ_ਸਾਂਝਾ_ਭਾਜਕ

        dispatch_ਕਤਾਰ_t concurrent_ਕਤਾਰ = dispatch_get_global_ਕਤਾਰ(DISPATCH_ਕਤਾਰ_ਤਰਜੀਹ_DEFAULT, 0);
        dispatch_apply_f(stripeRange.end - stripeRange.ਸ਼ੁਰੂ, concurrent_ਕਤਾਰ, &pbody, ਖੰਡ_function);

#ਜਾਂਜੇ defined WINRT

        pplParallelFor(stripeRange.ਸ਼ੁਰੂ, stripeRange.end, pbody);

#ਜਾਂਜੇ defined HAVE_CONCURRENCY

        ਜੇ(!pplScheduler || pplScheduler->Id() == Concurrency::CurrentScheduler::Id())
        {
            pplParallelFor(stripeRange.ਸ਼ੁਰੂ, stripeRange.end, pbody);
        }
        ਨਹੀਂਤਾਂ
        {
            pplScheduler->Attach();
            pplParallelFor(stripeRange.ਸ਼ੁਰੂ, stripeRange.end, pbody);
            Concurrency::CurrentScheduler::Detach();
        }

#ਜਾਂਜੇ defined HAVE_PTHREADS_PF

        parallel_for_pthreads(pbody.stripeRange(), pbody, pbody.stripeRange().size());

#ਨਹੀਂਤਾਂ

#ਗਲਤੀ You have hacked and compiling with unsupported parallel framework

#ਜੇ_ਅੰਤ

        ctx.finalize();  // propagate exceptions if exists
        ਵਾਪਸ;
#ਜੇ_ਅੰਤ // CV_PARALLEL_FRAMEWORK
    }

    body(range);
}


ਪੂਰਨ get_ਅੰਕ_ਧਾਗੇ(ਖਾਲੀ)
{
    std::shared_ptr<ParallelForAPI>& api = getCurrentParallelForAPI();
    ਜੇ (api)
    {
        ਵਾਪਸ api->get_ਅੰਕ_ਧਾਗੇ();
    }

    ਜੇ (ਅੰਕ_ਧਾਗੇ == 0)
        ਵਾਪਸ 1;

#ਜੇ defined HAVE_TBB

#ਜੇ TBB_INTERFACE_ਸੰਸਕਰਣ >= 9100
    ਵਾਪਸ tbbArena.max_concurrency();
#ਜਾਂਜੇ TBB_INTERFACE_ਸੰਸਕਰਣ >= 8000
    ਵਾਪਸ ਅੰਕ_ਧਾਗੇ > 0
        ? ਅੰਕ_ਧਾਗੇ
        : tbb::task_scheduler_init::default_ਅੰਕ_ਧਾਗੇ();
#ਨਹੀਂਤਾਂ
    ਵਾਪਸ tbbScheduler.is_ਸਰਗਰਮ()
           ? ਅੰਕ_ਧਾਗੇ
           : tbb::task_scheduler_init::default_ਅੰਕ_ਧਾਗੇ();
#ਜੇ_ਅੰਤ

#ਜਾਂਜੇ defined HAVE_HPX
    ਵਾਪਸ ਅੰਕ_ਧਾਗੇ;

#ਜਾਂਜੇ defined HAVE_OPENMP

    ਵਾਪਸ ਅੰਕ_ਧਾਗੇ > 0
           ? ਅੰਕ_ਧਾਗੇ
           : ਅੰਕ_ਧਾਗੇ_Max;


#ਜਾਂਜੇ defined HAVE_ਮਹੱਤਮ_ਸਾਂਝਾ_ਭਾਜਕ

    ਵਾਪਸ cv::get_ਸੰਖਿਆ_ਦਾ_CPUs(); // the GCD thread pool limit

#ਜਾਂਜੇ defined WINRT

    ਵਾਪਸ 0;

#ਜਾਂਜੇ defined HAVE_CONCURRENCY

    ਵਾਪਸ (pplScheduler == 0)
        ? Concurrency::CurrentScheduler::Get()->Get_ਸੰਖਿਆ_ਦਾ_Virtual_Processors()
        : (1 + pplScheduler->Get_ਸੰਖਿਆ_ਦਾ_Virtual_Processors());

#ਜਾਂਜੇ defined HAVE_PTHREADS_PF

        ਵਾਪਸ parallel_pthreads_get_ਧਾਗੇ_ਅੰਕ();

#ਨਹੀਂਤਾਂ

    ਵਾਪਸ 1;

#ਜੇ_ਅੰਤ
}

ਬਿਨਾਨਿਸ਼ਾਨੀ default_ਸੰਖਿਆ_ਦਾ_ਧਾਗੇ()
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __ANDROID__
    // many modern phones/tables have 4-core CPUs. Let's use no more
    // than 2 threads by default not to overheat the devices
    ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ default_ਸੰਖਿਆ_ਦਾ_ਧਾਗੇ_2 = 2;
#ਨਹੀਂਤਾਂ
    ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ default_ਸੰਖਿਆ_ਦਾ_ਧਾਗੇ_2 = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ)std::max(1, cv::get_ਸੰਖਿਆ_ਦਾ_CPUs());
#ਜੇ_ਅੰਤ

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਨਤੀਜਾ = default_ਸੰਖਿਆ_ਦਾ_ਧਾਗੇ_2;

    ਸਥਿਰ ਪੂਰਨ config_ਅੰਕ_ਧਾਗੇ = (ਪੂਰਨ)utils::get_Configuration_Parameter_ਮਾਪ_T("OPENCV_FOR_THREADS_NUM", 0);

    ਜੇ (config_ਅੰਕ_ਧਾਗੇ)
    {
        ਨਤੀਜਾ = (ਬਿਨਾਨਿਸ਼ਾਨੀ)std::max(1, config_ਅੰਕ_ਧਾਗੇ);
        //do we need upper limit of threads number?
    }
    ਵਾਪਸ ਨਤੀਜਾ;
}

ਖਾਲੀ ਸੈੱਟ_ਅੰਕ_ਧਾਗੇ( ਪੂਰਨ ਧਾਗੇ_2 )
{
    CV_ਅਣਵਰਤਿਆ(ਧਾਗੇ_2);

    ਪੂਰਨ ਧਾਗੇ = (ਧਾਗੇ_2 < 0) ? default_ਸੰਖਿਆ_ਦਾ_ਧਾਗੇ() : (ਬਿਨਾਨਿਸ਼ਾਨੀ)ਧਾਗੇ_2;
    ਅੰਕ_ਧਾਗੇ = ਧਾਗੇ;

    std::shared_ptr<ParallelForAPI>& api = getCurrentParallelForAPI();
    ਜੇ (api)
    {
        api->ਸੈੱਟ_ਅੰਕ_ਧਾਗੇ(ਅੰਕ_ਧਾਗੇ);
    }

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_TBB

#ਜੇ TBB_INTERFACE_ਸੰਸਕਰਣ >= 8000
    ਜੇ(tbbArena.is_ਸਰਗਰਮ()) tbbArena.terminate();
    ਜੇ(ਧਾਗੇ > 0) tbbArena.initialize(ਧਾਗੇ);
#ਨਹੀਂਤਾਂ
    ਜੇ(tbbScheduler.is_ਸਰਗਰਮ()) tbbScheduler.terminate();
    ਜੇ(ਧਾਗੇ > 0) tbbScheduler.initialize(ਧਾਗੇ);
#ਜੇ_ਅੰਤ

#ਜਾਂਜੇ defined HAVE_HPX
    ਵਾਪਸ; // nothing needed as numThreads is used

#ਜਾਂਜੇ defined HAVE_OPENMP

    ਵਾਪਸ; // nothing needed as num_threads clause is used in #pragma omp parallel for

#ਜਾਂਜੇ defined HAVE_ਮਹੱਤਮ_ਸਾਂਝਾ_ਭਾਜਕ

    // unsupported
    // there is only private dispatch_queue_set_width() and only for desktop

#ਜਾਂਜੇ defined WINRT

    ਵਾਪਸ;

#ਜਾਂਜੇ defined HAVE_CONCURRENCY

    ਜੇ (ਧਾਗੇ <= 0)
    {
        pplScheduler = 0;
    }
    ਨਹੀਂਤਾਂ ਜੇ (ਧਾਗੇ == 1)
    {
        // Concurrency always uses >=2 threads, so we just disable it if 1 thread is requested
        ਅੰਕ_ਧਾਗੇ = 0;
    }
    ਨਹੀਂਤਾਂ ਜੇ (pplScheduler == 0 || 1 + pplScheduler->Get_ਸੰਖਿਆ_ਦਾ_Virtual_Processors() != (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ)ਧਾਗੇ)
    {
        pplScheduler = Concurrency::Scheduler::Create(Concurrency::SchedulerPolicy(2,
                       Concurrency::MinConcurrency, ਧਾਗੇ-1,
                       Concurrency::MaxConcurrency, ਧਾਗੇ-1));
    }

#ਜਾਂਜੇ defined HAVE_PTHREADS_PF

    parallel_pthreads_ਸੈੱਟ_ਧਾਗੇ_ਅੰਕ(ਧਾਗੇ);

#ਜੇ_ਅੰਤ
}


ਪੂਰਨ get_Thread_ਅੰਕ()
{
    std::shared_ptr<ParallelForAPI>& api = getCurrentParallelForAPI();
    ਜੇ (api)
    {
        ਵਾਪਸ api->get_Thread_ਅੰਕ();
    }

#ਜੇ defined HAVE_TBB
    #ਜੇ TBB_INTERFACE_ਸੰਸਕਰਣ >= 9100
        ਵਾਪਸ tbb::this_task_arena::current_thread_ਸੂਚਕਾਂਕ();
    #ਜਾਂਜੇ TBB_INTERFACE_ਸੰਸਕਰਣ >= 8000
        ਵਾਪਸ tbb::task_arena::current_thread_ਸੂਚਕਾਂਕ();
    #ਨਹੀਂਤਾਂ
        ਵਾਪਸ 0;
    #ਜੇ_ਅੰਤ
#ਜਾਂਜੇ defined HAVE_HPX
    ਵਾਪਸ (ਪੂਰਨ)(hpx::get_ਅੰਕ_ਕਾਮਾ_ਧਾਗੇ());
#ਜਾਂਜੇ defined HAVE_OPENMP
    ਵਾਪਸ omp_get_thread_ਅੰਕ();
#ਜਾਂਜੇ defined HAVE_ਮਹੱਤਮ_ਸਾਂਝਾ_ਭਾਜਕ
    ਵਾਪਸ (ਪੂਰਨ)(size_t)(ਖਾਲੀ*)pthread_self(); // no zero-based indexing
#ਜਾਂਜੇ defined WINRT
    ਵਾਪਸ 0;
#ਜਾਂਜੇ defined HAVE_CONCURRENCY
    ਵਾਪਸ std::max(0, (ਪੂਰਨ)Concurrency::Context::VirtualProcessorId()); // zero for master thread, unique number for others but not necessary 1,2,3,...
#ਜਾਂਜੇ defined HAVE_PTHREADS_PF
    ਵਾਪਸ (ਪੂਰਨ)(size_t)(ਖਾਲੀ*)pthread_self(); // no zero-based indexing
#ਨਹੀਂਤਾਂ
    ਵਾਪਸ 0;
#ਜੇ_ਅੰਤ
}


#ਜੇ defined __linux__ || defined __GLIBC__ || defined __HAIKU__ || defined __ANDROID__
  #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CV_CPU_GROUPS_1
#ਜੇ_ਅੰਤ

#ਜੇ defined __linux__ || defined __ANDROID__
  #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CV_HAVE_CGROUPS 1
#ਜੇ_ਅੰਤ

#ਜੇ defined CV_CPU_GROUPS_1
ਸਥਿਰ ਸਤਰਅੰਦਰ
std::string getFileContents(ਅਚਲ ਅੱਖਰ *filename)
{
    std::ifstream ifs(filename);
    ਜੇ (!ifs.is_open())
        ਵਾਪਸ std::string();

    std::string content( (std::istreambuf_iterator<ਅੱਖਰ>(ifs) ),
                         (std::istreambuf_iterator<ਅੱਖਰ>()    ) );

    ਜੇ (ifs.fail())
        ਵਾਪਸ std::string();

    ਵਾਪਸ content;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ
ਪੂਰਨ get_ਸੰਖਿਆ_ਦਾ_CPUs_Impl(ਅਚਲ ਅੱਖਰ *filename)
{
   std::string file_contents = getFileContents(filename);
   ਜੇ(file_contents.empty())
       ਵਾਪਸ 0;

   ਅੱਖਰ *pbuf = const_cast<ਅੱਖਰ*>(file_contents.c_str());
   //parse string of form "0-1,3,5-7,10,13-15"
   ਪੂਰਨ cpus_ਉਪਲਬਧ = 0;

   ਜਦੋਂ(*pbuf)
   {
      ਅਚਲ ਅੱਖਰ* pos = pbuf;
      ਤਾਰਕਿਕ range = ਝੂਠ;
      ਜਦੋਂ(*pbuf && *pbuf != ',')
      {
          ਜੇ(*pbuf == '-') range = ਸੱਚ;
          ++pbuf;
      }
      ਜੇ(*pbuf) *pbuf++ = 0;
      ਜੇ(!range)
        ++cpus_ਉਪਲਬਧ;
      ਨਹੀਂਤਾਂ
      {
          ਪੂਰਨ rstart = 0, rend = 0;
          ਲੜੀ_ਤੋਂ_ਪੜ੍ਹੋ(pos, "%d-%d", &rstart, &rend);
          cpus_ਉਪਲਬਧ += rend - rstart + 1;
      }

   }
   ਵਾਪਸ cpus_ਉਪਲਬਧ;
}
#ਜੇ_ਅੰਤ

#ਜੇ defined CV_HAVE_CGROUPS
ਸਥਿਰ ਸਤਰਅੰਦਰ
ਬਿਨਾਨਿਸ਼ਾਨੀ get_ਸੰਖਿਆ_ਦਾ_CPUs_CFSv2()
{
    ਪੂਰਨ cfs_quota = 0;
    ਪੂਰਨ cfs_period = 0;

    std::ifstream ss_cpu_max("/sys/fs/cgroup/cpu.max", std::ios::in | std::ios::binary);
    ss_cpu_max >> cfs_quota >> cfs_period;

    ਜੇ (ss_cpu_max.fail() || cfs_quota < 1 || cfs_period < 1) /* values must not be 0 or negative */
        ਵਾਪਸ 0;

    ਵਾਪਸ (ਬਿਨਾਨਿਸ਼ਾਨੀ)max(1, cfs_quota/cfs_period);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ
ਬਿਨਾਨਿਸ਼ਾਨੀ get_ਸੰਖਿਆ_ਦਾ_CPUs_CFSv1()
{
    ਪੂਰਨ cfs_quota = 0;
    {
        std::ifstream ss_period("/sys/fs/cgroup/cpu/cpu.cfs_quota_us", std::ios::in | std::ios::binary);
        ss_period >> cfs_quota;

        ਜੇ (ss_period.fail() || cfs_quota < 1) /* cfs_quota must not be 0 or negative */
            ਵਾਪਸ 0;
    }

    ਪੂਰਨ cfs_period = 0;
    {
        std::ifstream ss_quota("/sys/fs/cgroup/cpu/cpu.cfs_period_us", std::ios::in | std::ios::binary);
        ss_quota >> cfs_period;

        ਜੇ (ss_quota.fail() || cfs_period < 1)
            ਵਾਪਸ 0;
    }

    ਵਾਪਸ (ਬਿਨਾਨਿਸ਼ਾਨੀ)max(1, cfs_quota/cfs_period);
}
#ਜੇ_ਅੰਤ

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T> ਸਥਿਰ ਸਤਰਅੰਦਰ
T minNonZero(ਅਚਲ T& val_1, ਅਚਲ T& val_2)
{
    ਜੇ ((val_1 != 0) && (val_2 != 0))
        ਵਾਪਸ std::min(val_1, val_2);
    ਵਾਪਸ (val_1 != 0) ? val_1 : val_2;
}

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OPENCV_DISABLE_THREAD_SUPPORT
ਸਥਿਰ
ਪੂਰਨ get_ਸੰਖਿਆ_ਦਾ_CPUs_2()
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OPENCV_SEMIHOSTING
    /*
     * Logic here is to try different methods of getting CPU counts and return
     * the minimum most value as it has high probablity of being right and safe.
     * Return 1 if we get 0 or not found on all methods.
    */
#ਜੇ !defined(__MINGW32__) /* not implemented (2020-03) */

    /*
     * Check for this standard C++11 way, we do not return directly because
     * running in a docker or K8s environment will mean this is the host
     * machines config not the containers or pods and as per docs this value
     * must be "considered only a hint".
    */
    ਬਿਨਾਨਿਸ਼ਾਨੀ ncpus = std::thread::hardware_concurrency(); /* If the value is not well defined or not computable, returns 0 */
#ਨਹੀਂਤਾਂ
    ਬਿਨਾਨਿਸ਼ਾਨੀ ncpus = 0; /* 0 means we have to find out some other way */
#ਜੇ_ਅੰਤ

#ਜੇ defined _WIN32

    SYSTEM_INFO sysinfo = {};
#ਜੇ (defined(_M_ARM) || defined(_M_ARM64) || defined(_M_ARM64EC) || defined(_M_X64) || defined(WINRT)) && _WIN32_WINNT >= 0x501
    GetNativeSystemInfo( &sysinfo );
#ਨਹੀਂਤਾਂ
    GetSystemInfo( &sysinfo );
#ਜੇ_ਅੰਤ
    ਬਿਨਾਨਿਸ਼ਾਨੀ ncpus_sysinfo = sysinfo.dw_ਸੰਖਿਆ_ਦਾ_Processors;
    ncpus = minNonZero(ncpus, ncpus_sysinfo);

#ਜਾਂਜੇ defined __APPLE__

    ਪੂਰਨ ਅੰਕ_CPU=0;
    ਪੂਰਨ mib[4];
    size_t len = ਆਕਾਰ(ਅੰਕ_CPU);

    /* set the mib for hw.ncpu */
    mib[0] = CTL_HW;
    mib[1] = HW_AVAILCPU;  // alternatively, try HW_NCPU;

    /* get the number of CPUs from the system */
    sysctl(mib, 2, &ਅੰਕ_CPU, &len, ਨਿਰਾ, 0);

    ਜੇ( ਅੰਕ_CPU < 1 )
    {
        mib[1] = HW_NCPU;
        sysctl( mib, 2, &ਅੰਕ_CPU, &len, ਨਿਰਾ, 0 );

        ਜੇ( ਅੰਕ_CPU < 1 )
            ਅੰਕ_CPU = 1;
    }

    ncpus = minNonZero(ncpus, (ਬਿਨਾਨਿਸ਼ਾਨੀ)ਅੰਕ_CPU);

#ਜਾਂਜੇ defined CV_CPU_GROUPS_1

#ਜੇ defined CV_HAVE_CGROUPS
    ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ncpus_impl_cpuset = (ਬਿਨਾਨਿਸ਼ਾਨੀ)get_ਸੰਖਿਆ_ਦਾ_CPUs_Impl("/sys/fs/cgroup/cpuset/cpuset.cpus");
    ncpus = minNonZero(ncpus, ncpus_impl_cpuset);

    ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ncpus_impl_cfs_v1 = get_ਸੰਖਿਆ_ਦਾ_CPUs_CFSv1();
    ncpus = minNonZero(ncpus, ncpus_impl_cfs_v1);

    ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ncpus_impl_cfs_v2 = get_ਸੰਖਿਆ_ਦਾ_CPUs_CFSv2();
    ncpus = minNonZero(ncpus, ncpus_impl_cfs_v2);
#ਜੇ_ਅੰਤ

    ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ncpus_impl_devices = (ਬਿਨਾਨਿਸ਼ਾਨੀ)get_ਸੰਖਿਆ_ਦਾ_CPUs_Impl("/sys/devices/system/cpu/online");
    ncpus = minNonZero(ncpus, ncpus_impl_devices);

#ਜੇ_ਅੰਤ

#ਜੇ defined _GNU_SOURCE \
    && !defined(__MINGW32__) /* not implemented (2020-03) */ \
    && !defined(__EMSCRIPTEN__) \
    && !defined(__ANDROID__)  // TODO: add check for modern Android NDK

    cpu_set_t cpu_ਸੈੱਟ;
    ਜੇ (0 == sched_getaffinity(0, ਆਕਾਰ(cpu_ਸੈੱਟ), &cpu_ਸੈੱਟ))
    {
        ਬਿਨਾਨਿਸ਼ਾਨੀ cpu_ਗਿਣਤੀ_cpu_ਸੈੱਟ = CPU_COUNT(&cpu_ਸੈੱਟ);
        ncpus = minNonZero(ncpus, cpu_ਗਿਣਤੀ_cpu_ਸੈੱਟ);
    }

#ਜੇ_ਅੰਤ

#ਜੇ !defined(_WIN32) && !defined(__APPLE__) && defined(_SC_NPROCESSORS_ONLN)

    ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ cpu_ਗਿਣਤੀ_sysconf = (ਬਿਨਾਨਿਸ਼ਾਨੀ)sysconf( _SC_NPROCESSORS_ONLN );
    ncpus = minNonZero(ncpus, cpu_ਗਿਣਤੀ_sysconf);
#ਜਾਂਜੇ defined (__QNX__)
    ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ cpu_ਗਿਣਤੀ_sysconf = _syspage_ptr->ਅੰਕ_cpu;
    ncpus = minNonZero(ncpus, cpu_ਗਿਣਤੀ_sysconf);
#ਜੇ_ਅੰਤ

    ਵਾਪਸ ncpus != 0 ? ncpus : 1;
#ਨਹੀਂਤਾਂ //  OPENCV_SEMIHOSTING
    ਵਾਪਸ 1;
#ਜੇ_ਅੰਤ //OPENCV_SEMIHOSTING
}

ਪੂਰਨ get_ਸੰਖਿਆ_ਦਾ_CPUs()
{
    ਸਥਿਰ ਪੂਰਨ nCPUs = get_ਸੰਖਿਆ_ਦਾ_CPUs_2();
    ਵਾਪਸ nCPUs;  // cached value
}

#ਨਹੀਂਤਾਂ  // OPENCV_DISABLE_THREAD_SUPPORT
ਪੂਰਨ get_ਸੰਖਿਆ_ਦਾ_CPUs()
{
    ਵਾਪਸ 1;
}
#ਜੇ_ਅੰਤ  // OPENCV_DISABLE_THREAD_SUPPORT

ਅਚਲ ਅੱਖਰ* currentParallelFramework()
{
    std::shared_ptr<ParallelForAPI>& api = getCurrentParallelForAPI();
    ਜੇ (api)
    {
        ਵਾਪਸ api->getName();
    }
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CV_PARALLEL_FRAMEWORK
    ਵਾਪਸ CV_PARALLEL_FRAMEWORK;
#ਨਹੀਂਤਾਂ
    ਵਾਪਸ ਨਿਰਾ;
#ਜੇ_ਅੰਤ
}

}  // namespace cv::
