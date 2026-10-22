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

#ಅನ್ತರ್ಭಾವ "precomp.hpp"

#ಅನ್ತರ್ಭಾವ <opencv2/core/utils/configuration.private.hpp>
#ಅನ್ತರ್ಭಾವ <opencv2/core/utils/trace.private.hpp>

#ಅನ್ತರ್ಭಾವ "opencv2/core/parallel/parallel_backend.hpp"
#ಅನ್ತರ್ಭಾವ "parallel/parallel.hpp"

#ಆದರೆ defined _WIN32 || defined WINCE
    #ಅನ್ತರ್ಭಾವ <windows.h>
    #ವ್ಯಾಖ್ಯಾನ_ರದ್ದು small
    #ವ್ಯಾಖ್ಯಾನ_ರದ್ದು min
    #ವ್ಯಾಖ್ಯಾನ_ರದ್ದು max
    #ವ್ಯಾಖ್ಯಾನ_ರದ್ದು ನಿರಪೇಕ್ಷ
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ defined __unix__ || defined __APPLE__ || defined __GLIBC__ \
    || defined __HAIKU__ || defined __EMSCRIPTEN__ \
    || defined __FreeBSD__ || defined __NetBSD__ || defined __OpenBSD__
    #ಅನ್ತರ್ಭಾವ <unistd.h>
    #ಅನ್ತರ್ಭಾವ <stdio.h>
    #ಅನ್ತರ್ಭಾವ <sys/types.h>
    #ಅನ್ತರ್ಭಾವ <fstream>
    #ಆದರೆ defined __ANDROID__
        #ಅನ್ತರ್ಭಾವ <sys/sysconf.h>
        #ಅನ್ತರ್ಭಾವ <sys/syscall.h>
        #ಅನ್ತರ್ಭಾವ <sched.h>
    #ಅಥವಾಆದರೆ defined __APPLE__
        #ಅನ್ತರ್ಭಾವ <sys/sysctl.h>
    #ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ defined (__QNX__)
    #ಅನ್ತರ್ಭಾವ <sys/syspage.h>
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ OPENCV_DISABLE_THREAD_SUPPORT
    #ಅನ್ತರ್ಭಾವ <thread>
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ _OPENMP
    #ವ್ಯಾಖ್ಯಾನಿಸು HAVE_OPENMP
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __APPLE__
    #ವ್ಯಾಖ್ಯಾನಿಸು HAVE_ಮಹತ್ತಮ_ಸಾಮಾನ್ಯ_ಅಪವರ್ತನ
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ defined _MSC_VER
    #ವ್ಯಾಖ್ಯಾನಿಸು HAVE_CONCURRENCY
#ಆದರೆ_ಅಂತ್ಯ

/* IMPORTANT: always use the same order of defines
   - HAVE_TBB         - 3rdparty library, should be explicitly enabled
   - HAVE_HPX         - 3rdparty library, should be explicitly enabled
   - HAVE_OPENMP      - integrated to compiler, should be explicitly enabled
   - HAVE_GCD         - system wide, used automatically        (APPLE only)
   - WINRT            - system wide, used automatically        (Windows RT only)
   - HAVE_CONCURRENCY - part of runtime, used automatically    (Windows only - MSVS 10, MSVS 11)
   - HAVE_PTHREADS_PF - pthreads if available
*/

#ಆದರೆ defined HAVE_TBB
    #ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ TBB_SUPPRESS_DEPRECATED_MESSAGES  // supress warning
    #ವ್ಯಾಖ್ಯಾನಿಸು TBB_SUPPRESS_DEPRECATED_MESSAGES 1
    #ಆದರೆ_ಅಂತ್ಯ
    #ಅನ್ತರ್ಭಾವ "tbb/tbb.h"
    #ಅನ್ತರ್ಭಾವ "tbb/task.h"
    #ಆದರೆ TBB_INTERFACE_ಆವೃತ್ತಿ >= 8000
        #ಅನ್ತರ್ಭಾವ "tbb/task_arena.h"
    #ಆದರೆ_ಅಂತ್ಯ
    #ವ್ಯಾಖ್ಯಾನ_ರದ್ದು min
    #ವ್ಯಾಖ್ಯಾನ_ರದ್ದು max
#ಅಥವಾಆದರೆ defined HAVE_HPX
    #ಅನ್ತರ್ಭಾವ <hpx/parallel/algorithms/for_loop.hpp>
    #ಅನ್ತರ್ಭಾವ <hpx/parallel/execution.hpp>
    //
    #ಅನ್ತರ್ಭಾವ <hpx/hpx_start.hpp>
    #ಅನ್ತರ್ಭಾವ <hpx/hpx_suspend.hpp>
    #ಅನ್ತರ್ಭಾವ <hpx/include/apply.hpp>
    #ಅನ್ತರ್ಭಾವ <hpx/util/yield_while.hpp>
    #ಅನ್ತರ್ಭಾವ <hpx/include/threadmanager.hpp>

#ಅಥವಾಆದರೆ defined HAVE_OPENMP
    #ಅನ್ತರ್ಭಾವ <omp.h>
#ಅಥವಾಆದರೆ defined HAVE_ಮಹತ್ತಮ_ಸಾಮಾನ್ಯ_ಅಪವರ್ತನ
    #ಅನ್ತರ್ಭಾವ <dispatch/dispatch.h>
    #ಅನ್ತರ್ಭಾವ <pthread.h>
#ಅಥವಾಆದರೆ defined HAVE_CONCURRENCY
    #ಅನ್ತರ್ಭಾವ <ppl.h>
#ಅಥವಾಆದರೆ defined HAVE_PTHREADS_PF
    #ಅನ್ತರ್ಭಾವ <pthread.h>
#ಆದರೆ_ಅಂತ್ಯ


#ಆದರೆ defined HAVE_TBB
#  define CV_PARALLEL_FRAMEWORK "tbb"
#ಅಥವಾಆದರೆ defined HAVE_HPX
#  define CV_PARALLEL_FRAMEWORK "hpx"
#ಅಥವಾಆದರೆ defined HAVE_OPENMP
#  define CV_PARALLEL_FRAMEWORK "openmp"
#ಅಥವಾಆದರೆ defined HAVE_ಮಹತ್ತಮ_ಸಾಮಾನ್ಯ_ಅಪವರ್ತನ
#  define CV_PARALLEL_FRAMEWORK "gcd"
#ಅಥವಾಆದರೆ defined WINRT
#  define CV_PARALLEL_FRAMEWORK "winrt-concurrency"
#ಅಥವಾಆದರೆ defined HAVE_CONCURRENCY
#  define CV_PARALLEL_FRAMEWORK "ms-concurrency"
#ಅಥವಾಆದರೆ defined HAVE_PTHREADS_PF
#  define CV_PARALLEL_FRAMEWORK "pthreads"
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ <atomic>

#ಅನ್ತರ್ಭಾವ "parallel_impl.hpp"

#ಅನ್ತರ್ಭಾವ "opencv2/core/detail/exception_ptr.hpp"  // CV__EXCEPTION_PTR = 1 if std::exception_ptr is available

#ಅನ್ತರ್ಭಾವ <opencv2/core/utils/fp_control_utils.hpp>
#ಅನ್ತರ್ಭಾವ <opencv2/core/utils/fp_control.private.hpp>

ಬಳಸಿ ನಾಮಾಕಾಶ cv;

ನಾಮಾಕಾಶ cv {

ParallelLoopBody::~ParallelLoopBody() {}

ಬಳಸಿ ನಾಮಾಕಾಶ cv::parallel;

ನಾಮಾಕಾಶ {

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ENABLE_INSTRUMENTATION
    ಸ್ಥಿರ ಶೂನ್ಯ SyncNodes(cv::instr::Instr_ಗಂಟು *p_ಗಂಟು)
    {
        std::vector<cv::instr::ಗಂಟು_ದತ್ತಾಂಶ_Tls*> ದತ್ತಾಂಶ;
        p_ಗಂಟು->m_payload.m_tls.gather(ದತ್ತಾಂಶ);

        uint64 ticksMax = 0;
        ಪೂರ್ಣಾಂಕ    ಎಳೆಗಳು  = 0;
        ಗಾಗಿ(size_t i = 0; i < ದತ್ತಾಂಶ.size(); i++)
        {
            ಆದರೆ(ದತ್ತಾಂಶ[i] && ದತ್ತಾಂಶ[i]->m_ticks_ಒಟ್ಟು)
            {
                ticksMax = MAX(ticksMax, ದತ್ತಾಂಶ[i]->m_ticks_ಒಟ್ಟು);
                p_ಗಂಟು->m_payload.m_ticks_ಒಟ್ಟು -= ದತ್ತಾಂಶ[i]->m_ticks_ಒಟ್ಟು;
                ದತ್ತಾಂಶ[i]->m_ticks_ಒಟ್ಟು = 0;
                ಎಳೆಗಳು++;
            }
        }
        p_ಗಂಟು->m_payload.m_ticks_ಒಟ್ಟು += ticksMax;
        p_ಗಂಟು->m_payload.m_ಎಳೆಗಳು = MAX(p_ಗಂಟು->m_payload.m_ಎಳೆಗಳು, ಎಳೆಗಳು);

        ಗಾಗಿ(size_t i = 0; i < p_ಗಂಟು->m_childs.size(); i++)
            SyncNodes(p_ಗಂಟು->m_childs[i]);
    }
#ಆದರೆ_ಅಂತ್ಯ

    ವರ್ಗ ParallelLoopBodyWrapperContext
    {
    ಸಾರ್ವಜನಿಕ:
        ParallelLoopBodyWrapperContext(ಸ್ಥಿರಾಂಕ cv::ParallelLoopBody& _body, ಸ್ಥಿರಾಂಕ cv::Range& _r, ದ್ವಿಗುಣ _nstripes) :
            is_rng_ಬಳಸಿದ(ಸುಳ್ಳು), hasException(ಸುಳ್ಳು)
        {

            body = &_body;
            ಸಂಪೂರ್ಣ_Range = _r;
            ದ್ವಿಗುಣ len = ಸಂಪೂರ್ಣ_Range.end - ಸಂಪೂರ್ಣ_Range.ಆರಂಭ;
            nstripes = cv_ಸುತ್ತು(_nstripes <= 0 ? len : MIN(MAX(_nstripes, 1.), len));

            // propagate main thread state
            rng = cv::theRNG();
#ಆದರೆ OPENCV_SUPPORTS_FP_DENORMALS_HINT && OPENCV_IMPL_FP_HINTS
            details::saveFPDenormalsState(fp_denormals_ತಳ_state);
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ OPENCV_TRACE
            trace_ಬೇರು_Region = CV_TRACE_NS::details::getCurrentRegion();
            trace_ಬೇರು_Context = CV_TRACE_NS::details::getTraceManager().tls.get();
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ENABLE_INSTRUMENTATION
            p_Thread_ಬೇರು = cv::instr::getInstrumentTLSStruct().p_Current_ಗಂಟು;
#ಆದರೆ_ಅಂತ್ಯ
        }
        ಶೂನ್ಯ finalize()
        {
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ENABLE_INSTRUMENTATION
            ಗಾಗಿ(size_t i = 0; i < p_Thread_ಬೇರು->m_childs.size(); i++)
                SyncNodes(p_Thread_ಬೇರು->m_childs[i]);
#ಆದರೆ_ಅಂತ್ಯ
            ಆದರೆ (is_rng_ಬಳಸಿದ)
            {
                // Some parallel backends execute nested jobs in the main thread,
                // so we need to restore initial RNG state here.
                cv::theRNG() = rng;
                // We can't properly update RNG state based on RNG usage in worker threads,
                // so lets just change main thread RNG state to the next value.
                // Note: this behaviour is not equal to single-threaded mode.
                cv::theRNG().next();
            }
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ OPENCV_TRACE
            ಆದರೆ (trace_ಬೇರು_Region)
                CV_TRACE_NS::details::parallelForFinalize(*trace_ಬೇರು_Region);
#ಆದರೆ_ಅಂತ್ಯ

            ಆದರೆ (hasException)
            {
#ಆದರೆ CV__EXCEPTION_PTR
                std::rethrow_exception(pException);
#ಇಲ್ಲದಿದ್ದರೆ
                CV_Error(Error::StsError, "Exception in parallel_for() body: " + exception_ಸಂದೇಶ);
#ಆದರೆ_ಅಂತ್ಯ
            }
        }
        ~ParallelLoopBodyWrapperContext() {}

        ಸ್ಥಿರಾಂಕ cv::ParallelLoopBody* body;
        cv::Range ಸಂಪೂರ್ಣ_Range;
        ಪೂರ್ಣಾಂಕ nstripes;
        cv::RNG rng;
        ಬದಲಾಯಿಸಬಹುದಾದ ತಾರ್ಕಿಕ is_rng_ಬಳಸಿದ;
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ OPENCV_TRACE
        CV_TRACE_NS::details::Region* trace_ಬೇರು_Region;
        CV_TRACE_NS::details::TraceManagerThreadLocal* trace_ಬೇರು_Context;
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ENABLE_INSTRUMENTATION
        cv::instr::Instr_ಗಂಟು *p_Thread_ಬೇರು;
#ಆದರೆ_ಅಂತ್ಯ
        ತಾರ್ಕಿಕ hasException;
#ಆದರೆ CV__EXCEPTION_PTR
        std::exception_ptr pException;
#ಇಲ್ಲದಿದ್ದರೆ
        cv::String exception_ಸಂದೇಶ;
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ CV__EXCEPTION_PTR
        ಶೂನ್ಯ ದಾಖಲೆ_Exception()
#ಇಲ್ಲದಿದ್ದರೆ
        ಶೂನ್ಯ ದಾಖಲೆ_Exception(ಸ್ಥಿರಾಂಕ cv::String& ಸಂದೇಶ)
#ಆದರೆ_ಅಂತ್ಯ
        {
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ CV_THREAD_SANITIZER
            ಆದರೆ (!hasException)
#ಆದರೆ_ಅಂತ್ಯ
            {
                cv::Auto_ಬೀಗ lock(cv::getInitializationMutex());
                ಆದರೆ (!hasException)
                {
                    hasException = ನಿಜ;
#ಆದರೆ CV__EXCEPTION_PTR
                    pException = std::current_exception();
#ಇಲ್ಲದಿದ್ದರೆ
                    exception_ಸಂದೇಶ = ಸಂದೇಶ;
#ಆದರೆ_ಅಂತ್ಯ
                }
            }
        }

#ಆದರೆ OPENCV_SUPPORTS_FP_DENORMALS_HINT && OPENCV_IMPL_FP_HINTS
        details::FPDenormals_ಬಹುಲಕ_State fp_denormals_ತಳ_state;
#ಆದರೆ_ಅಂತ್ಯ

    ಖಾಸಗಿ:
        ParallelLoopBodyWrapperContext(ಸ್ಥಿರಾಂಕ ParallelLoopBodyWrapperContext&); // disabled
        ParallelLoopBodyWrapperContext& ಕ್ರಿಯಾಕಾರಕ=(ಸ್ಥಿರಾಂಕ ParallelLoopBodyWrapperContext&); // disabled
    };

    ವರ್ಗ ParallelLoopBodyWrapper : ಸಾರ್ವಜನಿಕ cv::ParallelLoopBody
    {
    ಸಾರ್ವಜನಿಕ:
        ParallelLoopBodyWrapper(ParallelLoopBodyWrapperContext& ctx_) :
            ctx(ctx_)
        {
        }
        ~ParallelLoopBodyWrapper()
        {
        }
        ಶೂನ್ಯ ಕ್ರಿಯಾಕಾರಕ()(ಸ್ಥಿರಾಂಕ cv::Range& sr) ಸ್ಥಿರಾಂಕ CV_OVERRIDE
        {
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ OPENCV_TRACE
            // TODO CV_TRACE_NS::details::setCurrentRegion(rootRegion);
            ಆದರೆ (ctx.trace_ಬೇರು_Region && ctx.trace_ಬೇರು_Context)
                CV_TRACE_NS::details::parallel_For_ಗಣ_ಬೇರು_Region(*ctx.trace_ಬೇರು_Region, *ctx.trace_ಬೇರು_Context);
            CV__TRACE_OPENCV_FUNCTION_NAME("parallel_for_body");
            ಆದರೆ (ctx.trace_ಬೇರು_Region)
                CV_TRACE_NS::details::parallelForAttachNestedRegion(*ctx.trace_ಬೇರು_Region);
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ENABLE_INSTRUMENTATION
            {
                cv::instr::InstrTLSStruct *pInstrTLS = &cv::instr::getInstrumentTLSStruct();
                pInstrTLS->p_Current_ಗಂಟು = ctx.p_Thread_ಬೇರು; // Initialize TLS node for thread
            }
            CV_INSTRUMENT_REGION();
#ಆದರೆ_ಅಂತ್ಯ

            // propagate main thread state
            cv::theRNG() = ctx.rng;
#ಆದರೆ OPENCV_SUPPORTS_FP_DENORMALS_HINT && OPENCV_IMPL_FP_HINTS
            FPDenormalsIgnoreHintScope fp_denormals_scope(ctx.fp_denormals_ತಳ_state);
#ಆದರೆ_ಅಂತ್ಯ

            cv::Range r;
            cv::Range ಸಂಪೂರ್ಣ_Range = ctx.ಸಂಪೂರ್ಣ_Range;
            ಪೂರ್ಣಾಂಕ nstripes = ctx.nstripes;
            r.ಆರಂಭ = (ಪೂರ್ಣಾಂಕ)(ಸಂಪೂರ್ಣ_Range.ಆರಂಭ +
                            ((uint64)sr.ಆರಂಭ*(ಸಂಪೂರ್ಣ_Range.end - ಸಂಪೂರ್ಣ_Range.ಆರಂಭ) + nstripes/2)/nstripes);
            r.end = sr.end >= nstripes ? ಸಂಪೂರ್ಣ_Range.end : (ಪೂರ್ಣಾಂಕ)(ಸಂಪೂರ್ಣ_Range.ಆರಂಭ +
                            ((uint64)sr.end*(ಸಂಪೂರ್ಣ_Range.end - ಸಂಪೂರ್ಣ_Range.ಆರಂಭ) + nstripes/2)/nstripes);

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ OPENCV_TRACE
            CV_TRACE_ARG_ಮೌಲ್ಯ(range_ಆರಂಭ, "range.start", (int64)r.ಆರಂಭ);
            CV_TRACE_ARG_ಮೌಲ್ಯ(range_ಕೊನೆ, "range.end", (int64)r.end);
#ಆದರೆ_ಅಂತ್ಯ

            ಪ್ರಯತ್ನಿಸು
            {
                (*ctx.body)(r);
            }
#ಆದರೆ CV__EXCEPTION_PTR
            ಹಿಡಿ (...)
            {
                ctx.ದಾಖಲೆ_Exception();
            }
#ಇಲ್ಲದಿದ್ದರೆ
            ಹಿಡಿ (ಸ್ಥಿರಾಂಕ cv::Exception& e)
            {
                ctx.ದಾಖಲೆ_Exception(e.what());
            }
            ಹಿಡಿ (ಸ್ಥಿರಾಂಕ std::exception& e)
            {
                ctx.ದಾಖಲೆ_Exception(e.what());
            }
            ಹಿಡಿ (...)
            {
                ctx.ದಾಖಲೆ_Exception("Unknown exception");
            }
#ಆದರೆ_ಅಂತ್ಯ

            ಆದರೆ (!ctx.is_rng_ಬಳಸಿದ && !(cv::theRNG() == ctx.rng))
                ctx.is_rng_ಬಳಸಿದ = ನಿಜ;
        }
        cv::Range stripeRange() ಸ್ಥಿರಾಂಕ { ಹಿಂತಿರುಗಿ cv::Range(0, ctx.nstripes); }

    ಸಂರಕ್ಷಿತ:
        ParallelLoopBodyWrapperContext& ctx;
    };

#ಆದರೆ defined HAVE_TBB
    ವರ್ಗ ProxyLoopBody : ಸಾರ್ವಜನಿಕ ParallelLoopBodyWrapper
    {
    ಸಾರ್ವಜನಿಕ:
        ProxyLoopBody(ParallelLoopBodyWrapperContext& ctx_)
        : ParallelLoopBodyWrapper(ctx_)
        {}

        ಶೂನ್ಯ ಕ್ರಿಯಾಕಾರಕ ()(ಸ್ಥಿರಾಂಕ tbb::blocked_range<ಪೂರ್ಣಾಂಕ>& range) ಸ್ಥಿರಾಂಕ
        {
            ಇದು->ParallelLoopBodyWrapper::ಕ್ರಿಯಾಕಾರಕ()(cv::Range(range.begin(), range.end()));
        }

        ಶೂನ್ಯ ಕ್ರಿಯಾಕಾರಕ ()() ಸ್ಥಿರಾಂಕ  // run parallel job
        {
            cv::Range range = ಇದು->stripeRange();
            tbb::parallel_for(tbb::blocked_range<ಪೂರ್ಣಾಂಕ>(range.ಆರಂಭ, range.end), *ಇದು);
        }
    };
#ಅಥವಾಆದರೆ defined HAVE_HPX
    ವರ್ಗ ProxyLoopBody : ಸಾರ್ವಜನಿಕ ParallelLoopBodyWrapper
    {
    ಸಾರ್ವಜನಿಕ:
        ProxyLoopBody(ParallelLoopBodyWrapperContext& ctx_)
                : ParallelLoopBodyWrapper(ctx_)
        {}

        ಶೂನ್ಯ ಕ್ರಿಯಾಕಾರಕ ()() ಸ್ಥಿರಾಂಕ  // run parallel job
        {
            cv::Range stripeRange = ಇದು->stripeRange();
            hpx::parallel::for_loop(
                    hpx::parallel::execution::par,
                    stripeRange.ಆರಂಭ, stripeRange.end,
                    [&](ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ &i) { ;
                        ಇದು->ParallelLoopBodyWrapper::ಕ್ರಿಯಾಕಾರಕ()(
                                cv::Range(i, i + 1));
                    });
        }
    };
#ಅಥವಾಆದರೆ defined HAVE_OPENMP
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ParallelLoopBodyWrapper ProxyLoopBody;
#ಅಥವಾಆದರೆ defined HAVE_ಮಹತ್ತಮ_ಸಾಮಾನ್ಯ_ಅಪವರ್ತನ
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ParallelLoopBodyWrapper ProxyLoopBody;
    ಸ್ಥಿರ ಶೂನ್ಯ ಖಂಡ_function(ಶೂನ್ಯ* context, size_t ಸೂಚ್ಯಂಕ)
    {
        ProxyLoopBody* ptr_body = static_cast<ProxyLoopBody*>(context);
        (*ptr_body)(cv::Range((ಪೂರ್ಣಾಂಕ)ಸೂಚ್ಯಂಕ, (ಪೂರ್ಣಾಂಕ)ಸೂಚ್ಯಂಕ + 1));
    }
#ಅಥವಾಆದರೆ defined WINRT || defined HAVE_CONCURRENCY
    ವರ್ಗ ProxyLoopBody : ಸಾರ್ವಜನಿಕ ParallelLoopBodyWrapper
    {
    ಸಾರ್ವಜನಿಕ:
        ProxyLoopBody(ParallelLoopBodyWrapperContext& ctx)
        : ParallelLoopBodyWrapper(ctx)
        {}

        ಶೂನ್ಯ ಕ್ರಿಯಾಕಾರಕ ()(ಪೂರ್ಣಾಂಕ i) ಸ್ಥಿರಾಂಕ
        {
            ಇದು->ParallelLoopBodyWrapper::ಕ್ರಿಯಾಕಾರಕ()(cv::Range(i, i + 1));
        }
    };

    // PPL's default auto_partitioner silently skips whole chunks of the range on
    // ARM64 (unfixed MSVC bug, developercommunity.visualstudio.com/t/1027444).
    ಮಾದರಿ <ಬಗೆ_ಹೆಸರು Body>
    ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ pplParallelFor(ಪೂರ್ಣಾಂಕ first, ಪೂರ್ಣಾಂಕ ಕೊನೆಯ, ಸ್ಥಿರಾಂಕ Body& body)
    {
#ಆದರೆ defined(_M_ARM64) || defined(_M_ARM64EC)
        Concurrency::parallel_for(first, ಕೊನೆಯ, body, Concurrency::static_partitioner());
#ಇಲ್ಲದಿದ್ದರೆ
        Concurrency::parallel_for(first, ಕೊನೆಯ, body);
#ಆದರೆ_ಅಂತ್ಯ
    }
#ಇಲ್ಲದಿದ್ದರೆ
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ParallelLoopBodyWrapper ProxyLoopBody;
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ defined HAVE_TBB
    #ಆದರೆ TBB_INTERFACE_ಆವೃತ್ತಿ >= 8000
        ಸ್ಥಿರ tbb::task_arena tbbArena(tbb::task_arena::automatic);
    #ಇಲ್ಲದಿದ್ದರೆ
        ಸ್ಥಿರ tbb::task_scheduler_init tbbScheduler(tbb::task_scheduler_init::deferred);
    #ಆದರೆ_ಅಂತ್ಯ
#ಅಥವಾಆದರೆ defined HAVE_HPX
// nothing for HPX
#ಅಥವಾಆದರೆ defined HAVE_OPENMP
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ init_Max_ಎಳೆಗಳು()
{
    ಪೂರ್ಣಾಂಕ max_ಎಳೆಗಳು = omp_get_max_ಎಳೆಗಳು();
    ಹಿಂತಿರುಗಿ max_ಎಳೆಗಳು;
}
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಅಂಕೆ_ಎಳೆಗಳು_Max = init_Max_ಎಳೆಗಳು();
#ಅಥವಾಆದರೆ defined HAVE_ಮಹತ್ತಮ_ಸಾಮಾನ್ಯ_ಅಪವರ್ತನ
// nothing for GCD
#ಅಥವಾಆದರೆ defined WINRT
// nothing for WINRT
#ಅಥವಾಆದರೆ defined HAVE_CONCURRENCY

ವರ್ಗ SchedPtr
{
    Concurrency::Scheduler* sched_;
ಸಾರ್ವಜನಿಕ:
    Concurrency::Scheduler* ಕ್ರಿಯಾಕಾರಕ->() { ಹಿಂತಿರುಗಿ sched_; }
    ಕ್ರಿಯಾಕಾರಕ Concurrency::Scheduler*() { ಹಿಂತಿರುಗಿ sched_; }

    ಶೂನ್ಯ ಕ್ರಿಯಾಕಾರಕ=(Concurrency::Scheduler* sched)
    {
        ಆದರೆ (sched_) sched_->Release();
        sched_ = sched;
    }

    SchedPtr() : sched_(0) {}
    ~SchedPtr() {}
};
ಸ್ಥಿರ SchedPtr pplScheduler;

#ಆದರೆ_ಅಂತ್ಯ

} // namespace anon

/* ================================   parallel_for_  ================================ */

ಸ್ಥಿರ ಶೂನ್ಯ parallel_for_impl(ಸ್ಥಿರಾಂಕ cv::Range& range, ಸ್ಥಿರಾಂಕ cv::ParallelLoopBody& body, ದ್ವಿಗುಣ nstripes); // forward declaration

ಶೂನ್ಯ parallel_for_(ಸ್ಥಿರಾಂಕ cv::Range& range, ಸ್ಥಿರಾಂಕ cv::ParallelLoopBody& body, ದ್ವಿಗುಣ nstripes)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ OPENCV_TRACE
    CV__TRACE_OPENCV_FUNCTION_NAME_("parallel_for", 0);
    CV_TRACE_ARG_ಮೌಲ್ಯ(range_ಆರಂಭ, "range.start", (int64)range.ಆರಂಭ);
    CV_TRACE_ARG_ಮೌಲ್ಯ(range_ಕೊನೆ, "range.end", (int64)range.end);
    CV_TRACE_ARG_ಮೌಲ್ಯ(nstripes, "nstripes", (int64)nstripes);
#ಆದರೆ_ಅಂತ್ಯ

    CV_INSTRUMENT_REGION_MT_FORK();
    ಆದರೆ (range.empty())
        ಹಿಂತಿರುಗಿ;

    ಸ್ಥಿರ std::atomic<ತಾರ್ಕಿಕ> flagNestedParallelFor(ಸುಳ್ಳು);
    ತಾರ್ಕಿಕ isNotNestedRegion = !flagNestedParallelFor.load();
    ಆದರೆ (isNotNestedRegion)
      isNotNestedRegion = !flagNestedParallelFor.exchange(ನಿಜ);
    ಆದರೆ (isNotNestedRegion)
    {
        ಪ್ರಯತ್ನಿಸು
        {
            parallel_for_impl(range, body, nstripes);
            flagNestedParallelFor = ಸುಳ್ಳು;
        }
        ಹಿಡಿ (...)
        {
            flagNestedParallelFor = ಸುಳ್ಳು;
            ಎಸೆ;
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ // nested parallel_for_() calls are not parallelized
    {
        CV_ಬಳಸದ(nstripes);
        body(range);
    }
}

ಸ್ಥಿರ
ಶೂನ್ಯ parallel_for_cb(ಪೂರ್ಣಾಂಕ ಆರಂಭ, ಪೂರ್ಣಾಂಕ end, ಶೂನ್ಯ* ದತ್ತಾಂಶ)
{
    CV_DbgAssert(ದತ್ತಾಂಶ);
    ಸ್ಥಿರಾಂಕ cv::ParallelLoopBody& body = *(ಸ್ಥಿರಾಂಕ cv::ParallelLoopBody*)ದತ್ತಾಂಶ;
    body(Range(ಆರಂಭ, end));
}

ಸ್ಥಿರ ಶೂನ್ಯ parallel_for_impl(ಸ್ಥಿರಾಂಕ cv::Range& range, ಸ್ಥಿರಾಂಕ cv::ParallelLoopBody& body, ದ್ವಿಗುಣ nstripes)
{
    ಬಳಸಿ ನಾಮಾಕಾಶ cv::parallel;
    ಆದರೆ ((ಅಂಕೆ_ಎಳೆಗಳು < 0 || ಅಂಕೆ_ಎಳೆಗಳು > 1) && range.end - range.ಆರಂಭ > 1)
    {
        ParallelLoopBodyWrapperContext ctx(body, range, nstripes);
        ProxyLoopBody pbody(ctx);
        cv::Range stripeRange = pbody.stripeRange();
        ಆದರೆ( stripeRange.end - stripeRange.ಆರಂಭ == 1 )
        {
            body(range);
            ಹಿಂತಿರುಗಿ;
        }

        std::shared_ptr<ParallelForAPI>& api = getCurrentParallelForAPI();
        ಆದರೆ (api)
        {
            CV_CheckEQ(stripeRange.ಆರಂಭ, 0, "");
            api->parallel_for(stripeRange.end, parallel_for_cb, (ಶೂನ್ಯ*)&pbody);
            ctx.finalize();  // propagate exceptions if exists
            ಹಿಂತಿರುಗಿ;
        }

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CV_PARALLEL_FRAMEWORK
#ಆದರೆ defined HAVE_TBB

#ಆದರೆ TBB_INTERFACE_ಆವೃತ್ತಿ >= 8000
        tbbArena.execute(pbody);
#ಇಲ್ಲದಿದ್ದರೆ
        pbody();
#ಆದರೆ_ಅಂತ್ಯ

#ಅಥವಾಆದರೆ defined HAVE_HPX
        pbody();

#ಅಥವಾಆದರೆ defined HAVE_OPENMP

        #ಸೂಚನೆ omp parallel ಗಾಗಿ ವೇಳಾಪಟ್ಟಿ(dynamic) ಅಂಕೆ_ಎಳೆಗಳು_2(ಅಂಕೆ_ಎಳೆಗಳು > 0 ? ಅಂಕೆ_ಎಳೆಗಳು : ಅಂಕೆ_ಎಳೆಗಳು_Max)
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = stripeRange.ಆರಂಭ; i < stripeRange.end; ++i)
            pbody(Range(i, i + 1));

#ಅಥವಾಆದರೆ defined HAVE_ಮಹತ್ತಮ_ಸಾಮಾನ್ಯ_ಅಪವರ್ತನ

        dispatch_ಸರತಿ_t concurrent_ಸರತಿ = dispatch_get_global_ಸರತಿ(DISPATCH_ಸರತಿ_ಆದ್ಯತೆ_DEFAULT, 0);
        dispatch_apply_f(stripeRange.end - stripeRange.ಆರಂಭ, concurrent_ಸರತಿ, &pbody, ಖಂಡ_function);

#ಅಥವಾಆದರೆ defined WINRT

        pplParallelFor(stripeRange.ಆರಂಭ, stripeRange.end, pbody);

#ಅಥವಾಆದರೆ defined HAVE_CONCURRENCY

        ಆದರೆ(!pplScheduler || pplScheduler->Id() == Concurrency::CurrentScheduler::Id())
        {
            pplParallelFor(stripeRange.ಆರಂಭ, stripeRange.end, pbody);
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            pplScheduler->Attach();
            pplParallelFor(stripeRange.ಆರಂಭ, stripeRange.end, pbody);
            Concurrency::CurrentScheduler::Detach();
        }

#ಅಥವಾಆದರೆ defined HAVE_PTHREADS_PF

        parallel_for_pthreads(pbody.stripeRange(), pbody, pbody.stripeRange().size());

#ಇಲ್ಲದಿದ್ದರೆ

#ದೋಷ You have hacked and compiling with unsupported parallel framework

#ಆದರೆ_ಅಂತ್ಯ

        ctx.finalize();  // propagate exceptions if exists
        ಹಿಂತಿರುಗಿ;
#ಆದರೆ_ಅಂತ್ಯ // CV_PARALLEL_FRAMEWORK
    }

    body(range);
}


ಪೂರ್ಣಾಂಕ get_ಅಂಕೆ_ಎಳೆಗಳು(ಶೂನ್ಯ)
{
    std::shared_ptr<ParallelForAPI>& api = getCurrentParallelForAPI();
    ಆದರೆ (api)
    {
        ಹಿಂತಿರುಗಿ api->get_ಅಂಕೆ_ಎಳೆಗಳು();
    }

    ಆದರೆ (ಅಂಕೆ_ಎಳೆಗಳು == 0)
        ಹಿಂತಿರುಗಿ 1;

#ಆದರೆ defined HAVE_TBB

#ಆದರೆ TBB_INTERFACE_ಆವೃತ್ತಿ >= 9100
    ಹಿಂತಿರುಗಿ tbbArena.max_concurrency();
#ಅಥವಾಆದರೆ TBB_INTERFACE_ಆವೃತ್ತಿ >= 8000
    ಹಿಂತಿರುಗಿ ಅಂಕೆ_ಎಳೆಗಳು > 0
        ? ಅಂಕೆ_ಎಳೆಗಳು
        : tbb::task_scheduler_init::default_ಅಂಕೆ_ಎಳೆಗಳು();
#ಇಲ್ಲದಿದ್ದರೆ
    ಹಿಂತಿರುಗಿ tbbScheduler.is_ಸಕ್ರಿಯ()
           ? ಅಂಕೆ_ಎಳೆಗಳು
           : tbb::task_scheduler_init::default_ಅಂಕೆ_ಎಳೆಗಳು();
#ಆದರೆ_ಅಂತ್ಯ

#ಅಥವಾಆದರೆ defined HAVE_HPX
    ಹಿಂತಿರುಗಿ ಅಂಕೆ_ಎಳೆಗಳು;

#ಅಥವಾಆದರೆ defined HAVE_OPENMP

    ಹಿಂತಿರುಗಿ ಅಂಕೆ_ಎಳೆಗಳು > 0
           ? ಅಂಕೆ_ಎಳೆಗಳು
           : ಅಂಕೆ_ಎಳೆಗಳು_Max;


#ಅಥವಾಆದರೆ defined HAVE_ಮಹತ್ತಮ_ಸಾಮಾನ್ಯ_ಅಪವರ್ತನ

    ಹಿಂತಿರುಗಿ cv::get_ಸಂಖ್ಯೆ_ದ_CPUs(); // the GCD thread pool limit

#ಅಥವಾಆದರೆ defined WINRT

    ಹಿಂತಿರುಗಿ 0;

#ಅಥವಾಆದರೆ defined HAVE_CONCURRENCY

    ಹಿಂತಿರುಗಿ (pplScheduler == 0)
        ? Concurrency::CurrentScheduler::Get()->Get_ಸಂಖ್ಯೆ_ದ_Virtual_Processors()
        : (1 + pplScheduler->Get_ಸಂಖ್ಯೆ_ದ_Virtual_Processors());

#ಅಥವಾಆದರೆ defined HAVE_PTHREADS_PF

        ಹಿಂತಿರುಗಿ parallel_pthreads_get_ಎಳೆಗಳು_ಅಂಕೆ();

#ಇಲ್ಲದಿದ್ದರೆ

    ಹಿಂತಿರುಗಿ 1;

#ಆದರೆ_ಅಂತ್ಯ
}

ಚಿಹ್ನೆರಹಿತ default_ಸಂಖ್ಯೆ_ದ_ಎಳೆಗಳು()
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __ANDROID__
    // many modern phones/tables have 4-core CPUs. Let's use no more
    // than 2 threads by default not to overheat the devices
    ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ default_ಸಂಖ್ಯೆ_ದ_ಎಳೆಗಳು_2 = 2;
#ಇಲ್ಲದಿದ್ದರೆ
    ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ default_ಸಂಖ್ಯೆ_ದ_ಎಳೆಗಳು_2 = (ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ)std::max(1, cv::get_ಸಂಖ್ಯೆ_ದ_CPUs());
#ಆದರೆ_ಅಂತ್ಯ

    ಚಿಹ್ನೆರಹಿತ ಫಲಿತಾಂಶ = default_ಸಂಖ್ಯೆ_ದ_ಎಳೆಗಳು_2;

    ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ config_ಅಂಕೆ_ಎಳೆಗಳು = (ಪೂರ್ಣಾಂಕ)utils::get_Configuration_Parameter_ಗಾತ್ರ_T("OPENCV_FOR_THREADS_NUM", 0);

    ಆದರೆ (config_ಅಂಕೆ_ಎಳೆಗಳು)
    {
        ಫಲಿತಾಂಶ = (ಚಿಹ್ನೆರಹಿತ)std::max(1, config_ಅಂಕೆ_ಎಳೆಗಳು);
        //do we need upper limit of threads number?
    }
    ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}

ಶೂನ್ಯ ಗಣ_ಅಂಕೆ_ಎಳೆಗಳು( ಪೂರ್ಣಾಂಕ ಎಳೆಗಳು_2 )
{
    CV_ಬಳಸದ(ಎಳೆಗಳು_2);

    ಪೂರ್ಣಾಂಕ ಎಳೆಗಳು = (ಎಳೆಗಳು_2 < 0) ? default_ಸಂಖ್ಯೆ_ದ_ಎಳೆಗಳು() : (ಚಿಹ್ನೆರಹಿತ)ಎಳೆಗಳು_2;
    ಅಂಕೆ_ಎಳೆಗಳು = ಎಳೆಗಳು;

    std::shared_ptr<ParallelForAPI>& api = getCurrentParallelForAPI();
    ಆದರೆ (api)
    {
        api->ಗಣ_ಅಂಕೆ_ಎಳೆಗಳು(ಅಂಕೆ_ಎಳೆಗಳು);
    }

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_TBB

#ಆದರೆ TBB_INTERFACE_ಆವೃತ್ತಿ >= 8000
    ಆದರೆ(tbbArena.is_ಸಕ್ರಿಯ()) tbbArena.terminate();
    ಆದರೆ(ಎಳೆಗಳು > 0) tbbArena.initialize(ಎಳೆಗಳು);
#ಇಲ್ಲದಿದ್ದರೆ
    ಆದರೆ(tbbScheduler.is_ಸಕ್ರಿಯ()) tbbScheduler.terminate();
    ಆದರೆ(ಎಳೆಗಳು > 0) tbbScheduler.initialize(ಎಳೆಗಳು);
#ಆದರೆ_ಅಂತ್ಯ

#ಅಥವಾಆದರೆ defined HAVE_HPX
    ಹಿಂತಿರುಗಿ; // nothing needed as numThreads is used

#ಅಥವಾಆದರೆ defined HAVE_OPENMP

    ಹಿಂತಿರುಗಿ; // nothing needed as num_threads clause is used in #pragma omp parallel for

#ಅಥವಾಆದರೆ defined HAVE_ಮಹತ್ತಮ_ಸಾಮಾನ್ಯ_ಅಪವರ್ತನ

    // unsupported
    // there is only private dispatch_queue_set_width() and only for desktop

#ಅಥವಾಆದರೆ defined WINRT

    ಹಿಂತಿರುಗಿ;

#ಅಥವಾಆದರೆ defined HAVE_CONCURRENCY

    ಆದರೆ (ಎಳೆಗಳು <= 0)
    {
        pplScheduler = 0;
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಎಳೆಗಳು == 1)
    {
        // Concurrency always uses >=2 threads, so we just disable it if 1 thread is requested
        ಅಂಕೆ_ಎಳೆಗಳು = 0;
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (pplScheduler == 0 || 1 + pplScheduler->Get_ಸಂಖ್ಯೆ_ದ_Virtual_Processors() != (ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ)ಎಳೆಗಳು)
    {
        pplScheduler = Concurrency::Scheduler::Create(Concurrency::SchedulerPolicy(2,
                       Concurrency::MinConcurrency, ಎಳೆಗಳು-1,
                       Concurrency::MaxConcurrency, ಎಳೆಗಳು-1));
    }

#ಅಥವಾಆದರೆ defined HAVE_PTHREADS_PF

    parallel_pthreads_ಗಣ_ಎಳೆಗಳು_ಅಂಕೆ(ಎಳೆಗಳು);

#ಆದರೆ_ಅಂತ್ಯ
}


ಪೂರ್ಣಾಂಕ get_Thread_ಅಂಕೆ()
{
    std::shared_ptr<ParallelForAPI>& api = getCurrentParallelForAPI();
    ಆದರೆ (api)
    {
        ಹಿಂತಿರುಗಿ api->get_Thread_ಅಂಕೆ();
    }

#ಆದರೆ defined HAVE_TBB
    #ಆದರೆ TBB_INTERFACE_ಆವೃತ್ತಿ >= 9100
        ಹಿಂತಿರುಗಿ tbb::this_task_arena::current_thread_ಸೂಚ್ಯಂಕ();
    #ಅಥವಾಆದರೆ TBB_INTERFACE_ಆವೃತ್ತಿ >= 8000
        ಹಿಂತಿರುಗಿ tbb::task_arena::current_thread_ಸೂಚ್ಯಂಕ();
    #ಇಲ್ಲದಿದ್ದರೆ
        ಹಿಂತಿರುಗಿ 0;
    #ಆದರೆ_ಅಂತ್ಯ
#ಅಥವಾಆದರೆ defined HAVE_HPX
    ಹಿಂತಿರುಗಿ (ಪೂರ್ಣಾಂಕ)(hpx::get_ಅಂಕೆ_ಕೆಲಸಗಾರ_ಎಳೆಗಳು());
#ಅಥವಾಆದರೆ defined HAVE_OPENMP
    ಹಿಂತಿರುಗಿ omp_get_thread_ಅಂಕೆ();
#ಅಥವಾಆದರೆ defined HAVE_ಮಹತ್ತಮ_ಸಾಮಾನ್ಯ_ಅಪವರ್ತನ
    ಹಿಂತಿರುಗಿ (ಪೂರ್ಣಾಂಕ)(size_t)(ಶೂನ್ಯ*)pthread_self(); // no zero-based indexing
#ಅಥವಾಆದರೆ defined WINRT
    ಹಿಂತಿರುಗಿ 0;
#ಅಥವಾಆದರೆ defined HAVE_CONCURRENCY
    ಹಿಂತಿರುಗಿ std::max(0, (ಪೂರ್ಣಾಂಕ)Concurrency::Context::VirtualProcessorId()); // zero for master thread, unique number for others but not necessary 1,2,3,...
#ಅಥವಾಆದರೆ defined HAVE_PTHREADS_PF
    ಹಿಂತಿರುಗಿ (ಪೂರ್ಣಾಂಕ)(size_t)(ಶೂನ್ಯ*)pthread_self(); // no zero-based indexing
#ಇಲ್ಲದಿದ್ದರೆ
    ಹಿಂತಿರುಗಿ 0;
#ಆದರೆ_ಅಂತ್ಯ
}


#ಆದರೆ defined __linux__ || defined __GLIBC__ || defined __HAIKU__ || defined __ANDROID__
  #ವ್ಯಾಖ್ಯಾನಿಸು CV_CPU_GROUPS_1
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ defined __linux__ || defined __ANDROID__
  #ವ್ಯಾಖ್ಯಾನಿಸು CV_HAVE_CGROUPS 1
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ defined CV_CPU_GROUPS_1
ಸ್ಥಿರ ಸಾಲೊಳಗೆ
std::string getFileContents(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *filename)
{
    std::ifstream ifs(filename);
    ಆದರೆ (!ifs.is_open())
        ಹಿಂತಿರುಗಿ std::string();

    std::string content( (std::istreambuf_iterator<ಅಕ್ಷರ>(ifs) ),
                         (std::istreambuf_iterator<ಅಕ್ಷರ>()    ) );

    ಆದರೆ (ifs.fail())
        ಹಿಂತಿರುಗಿ std::string();

    ಹಿಂತಿರುಗಿ content;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ
ಪೂರ್ಣಾಂಕ get_ಸಂಖ್ಯೆ_ದ_CPUs_Impl(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *filename)
{
   std::string file_contents = getFileContents(filename);
   ಆದರೆ(file_contents.empty())
       ಹಿಂತಿರುಗಿ 0;

   ಅಕ್ಷರ *pbuf = const_cast<ಅಕ್ಷರ*>(file_contents.c_str());
   //parse string of form "0-1,3,5-7,10,13-15"
   ಪೂರ್ಣಾಂಕ cpus_ಲಭ್ಯ = 0;

   ಆಗ(*pbuf)
   {
      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* pos = pbuf;
      ತಾರ್ಕಿಕ range = ಸುಳ್ಳು;
      ಆಗ(*pbuf && *pbuf != ',')
      {
          ಆದರೆ(*pbuf == '-') range = ನಿಜ;
          ++pbuf;
      }
      ಆದರೆ(*pbuf) *pbuf++ = 0;
      ಆದರೆ(!range)
        ++cpus_ಲಭ್ಯ;
      ಇಲ್ಲದಿದ್ದರೆ
      {
          ಪೂರ್ಣಾಂಕ rstart = 0, rend = 0;
          ಅಕ್ಷರಸರಣಿ_ಓದು(pos, "%d-%d", &rstart, &rend);
          cpus_ಲಭ್ಯ += rend - rstart + 1;
      }

   }
   ಹಿಂತಿರುಗಿ cpus_ಲಭ್ಯ;
}
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ defined CV_HAVE_CGROUPS
ಸ್ಥಿರ ಸಾಲೊಳಗೆ
ಚಿಹ್ನೆರಹಿತ get_ಸಂಖ್ಯೆ_ದ_CPUs_CFSv2()
{
    ಪೂರ್ಣಾಂಕ cfs_quota = 0;
    ಪೂರ್ಣಾಂಕ cfs_period = 0;

    std::ifstream ss_cpu_max("/sys/fs/cgroup/cpu.max", std::ios::in | std::ios::binary);
    ss_cpu_max >> cfs_quota >> cfs_period;

    ಆದರೆ (ss_cpu_max.fail() || cfs_quota < 1 || cfs_period < 1) /* values must not be 0 or negative */
        ಹಿಂತಿರುಗಿ 0;

    ಹಿಂತಿರುಗಿ (ಚಿಹ್ನೆರಹಿತ)max(1, cfs_quota/cfs_period);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ
ಚಿಹ್ನೆರಹಿತ get_ಸಂಖ್ಯೆ_ದ_CPUs_CFSv1()
{
    ಪೂರ್ಣಾಂಕ cfs_quota = 0;
    {
        std::ifstream ss_period("/sys/fs/cgroup/cpu/cpu.cfs_quota_us", std::ios::in | std::ios::binary);
        ss_period >> cfs_quota;

        ಆದರೆ (ss_period.fail() || cfs_quota < 1) /* cfs_quota must not be 0 or negative */
            ಹಿಂತಿರುಗಿ 0;
    }

    ಪೂರ್ಣಾಂಕ cfs_period = 0;
    {
        std::ifstream ss_quota("/sys/fs/cgroup/cpu/cpu.cfs_period_us", std::ios::in | std::ios::binary);
        ss_quota >> cfs_period;

        ಆದರೆ (ss_quota.fail() || cfs_period < 1)
            ಹಿಂತಿರುಗಿ 0;
    }

    ಹಿಂತಿರುಗಿ (ಚಿಹ್ನೆರಹಿತ)max(1, cfs_quota/cfs_period);
}
#ಆದರೆ_ಅಂತ್ಯ

ಮಾದರಿ <ಬಗೆ_ಹೆಸರು T> ಸ್ಥಿರ ಸಾಲೊಳಗೆ
T minNonZero(ಸ್ಥಿರಾಂಕ T& val_1, ಸ್ಥಿರಾಂಕ T& val_2)
{
    ಆದರೆ ((val_1 != 0) && (val_2 != 0))
        ಹಿಂತಿರುಗಿ std::min(val_1, val_2);
    ಹಿಂತಿರುಗಿ (val_1 != 0) ? val_1 : val_2;
}

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ OPENCV_DISABLE_THREAD_SUPPORT
ಸ್ಥಿರ
ಪೂರ್ಣಾಂಕ get_ಸಂಖ್ಯೆ_ದ_CPUs_2()
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ OPENCV_SEMIHOSTING
    /*
     * Logic here is to try different methods of getting CPU counts and return
     * the minimum most value as it has high probablity of being right and safe.
     * Return 1 if we get 0 or not found on all methods.
    */
#ಆದರೆ !defined(__MINGW32__) /* not implemented (2020-03) */

    /*
     * Check for this standard C++11 way, we do not return directly because
     * running in a docker or K8s environment will mean this is the host
     * machines config not the containers or pods and as per docs this value
     * must be "considered only a hint".
    */
    ಚಿಹ್ನೆರಹಿತ ncpus = std::thread::hardware_concurrency(); /* If the value is not well defined or not computable, returns 0 */
#ಇಲ್ಲದಿದ್ದರೆ
    ಚಿಹ್ನೆರಹಿತ ncpus = 0; /* 0 means we have to find out some other way */
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ defined _WIN32

    SYSTEM_INFO sysinfo = {};
#ಆದರೆ (defined(_M_ARM) || defined(_M_ARM64) || defined(_M_ARM64EC) || defined(_M_X64) || defined(WINRT)) && _WIN32_WINNT >= 0x501
    GetNativeSystemInfo( &sysinfo );
#ಇಲ್ಲದಿದ್ದರೆ
    GetSystemInfo( &sysinfo );
#ಆದರೆ_ಅಂತ್ಯ
    ಚಿಹ್ನೆರಹಿತ ncpus_sysinfo = sysinfo.dw_ಸಂಖ್ಯೆ_ದ_Processors;
    ncpus = minNonZero(ncpus, ncpus_sysinfo);

#ಅಥವಾಆದರೆ defined __APPLE__

    ಪೂರ್ಣಾಂಕ ಅಂಕೆ_CPU=0;
    ಪೂರ್ಣಾಂಕ mib[4];
    size_t len = ಗಾತ್ರ(ಅಂಕೆ_CPU);

    /* set the mib for hw.ncpu */
    mib[0] = CTL_HW;
    mib[1] = HW_AVAILCPU;  // alternatively, try HW_NCPU;

    /* get the number of CPUs from the system */
    sysctl(mib, 2, &ಅಂಕೆ_CPU, &len, ಖಾಲಿ, 0);

    ಆದರೆ( ಅಂಕೆ_CPU < 1 )
    {
        mib[1] = HW_NCPU;
        sysctl( mib, 2, &ಅಂಕೆ_CPU, &len, ಖಾಲಿ, 0 );

        ಆದರೆ( ಅಂಕೆ_CPU < 1 )
            ಅಂಕೆ_CPU = 1;
    }

    ncpus = minNonZero(ncpus, (ಚಿಹ್ನೆರಹಿತ)ಅಂಕೆ_CPU);

#ಅಥವಾಆದರೆ defined CV_CPU_GROUPS_1

#ಆದರೆ defined CV_HAVE_CGROUPS
    ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ncpus_impl_cpuset = (ಚಿಹ್ನೆರಹಿತ)get_ಸಂಖ್ಯೆ_ದ_CPUs_Impl("/sys/fs/cgroup/cpuset/cpuset.cpus");
    ncpus = minNonZero(ncpus, ncpus_impl_cpuset);

    ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ncpus_impl_cfs_v1 = get_ಸಂಖ್ಯೆ_ದ_CPUs_CFSv1();
    ncpus = minNonZero(ncpus, ncpus_impl_cfs_v1);

    ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ncpus_impl_cfs_v2 = get_ಸಂಖ್ಯೆ_ದ_CPUs_CFSv2();
    ncpus = minNonZero(ncpus, ncpus_impl_cfs_v2);
#ಆದರೆ_ಅಂತ್ಯ

    ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ncpus_impl_devices = (ಚಿಹ್ನೆರಹಿತ)get_ಸಂಖ್ಯೆ_ದ_CPUs_Impl("/sys/devices/system/cpu/online");
    ncpus = minNonZero(ncpus, ncpus_impl_devices);

#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ defined _GNU_SOURCE \
    && !defined(__MINGW32__) /* not implemented (2020-03) */ \
    && !defined(__EMSCRIPTEN__) \
    && !defined(__ANDROID__)  // TODO: add check for modern Android NDK

    cpu_set_t cpu_ಗಣ;
    ಆದರೆ (0 == sched_getaffinity(0, ಗಾತ್ರ(cpu_ಗಣ), &cpu_ಗಣ))
    {
        ಚಿಹ್ನೆರಹಿತ cpu_ಎಣಿಕೆ_cpu_ಗಣ = CPU_COUNT(&cpu_ಗಣ);
        ncpus = minNonZero(ncpus, cpu_ಎಣಿಕೆ_cpu_ಗಣ);
    }

#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ !defined(_WIN32) && !defined(__APPLE__) && defined(_SC_NPROCESSORS_ONLN)

    ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ cpu_ಎಣಿಕೆ_sysconf = (ಚಿಹ್ನೆರಹಿತ)sysconf( _SC_NPROCESSORS_ONLN );
    ncpus = minNonZero(ncpus, cpu_ಎಣಿಕೆ_sysconf);
#ಅಥವಾಆದರೆ defined (__QNX__)
    ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ cpu_ಎಣಿಕೆ_sysconf = _syspage_ptr->ಅಂಕೆ_cpu;
    ncpus = minNonZero(ncpus, cpu_ಎಣಿಕೆ_sysconf);
#ಆದರೆ_ಅಂತ್ಯ

    ಹಿಂತಿರುಗಿ ncpus != 0 ? ncpus : 1;
#ಇಲ್ಲದಿದ್ದರೆ //  OPENCV_SEMIHOSTING
    ಹಿಂತಿರುಗಿ 1;
#ಆದರೆ_ಅಂತ್ಯ //OPENCV_SEMIHOSTING
}

ಪೂರ್ಣಾಂಕ get_ಸಂಖ್ಯೆ_ದ_CPUs()
{
    ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ nCPUs = get_ಸಂಖ್ಯೆ_ದ_CPUs_2();
    ಹಿಂತಿರುಗಿ nCPUs;  // cached value
}

#ಇಲ್ಲದಿದ್ದರೆ  // OPENCV_DISABLE_THREAD_SUPPORT
ಪೂರ್ಣಾಂಕ get_ಸಂಖ್ಯೆ_ದ_CPUs()
{
    ಹಿಂತಿರುಗಿ 1;
}
#ಆದರೆ_ಅಂತ್ಯ  // OPENCV_DISABLE_THREAD_SUPPORT

ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* currentParallelFramework()
{
    std::shared_ptr<ParallelForAPI>& api = getCurrentParallelForAPI();
    ಆದರೆ (api)
    {
        ಹಿಂತಿರುಗಿ api->getName();
    }
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CV_PARALLEL_FRAMEWORK
    ಹಿಂತಿರುಗಿ CV_PARALLEL_FRAMEWORK;
#ಇಲ್ಲದಿದ್ದರೆ
    ಹಿಂತಿರುಗಿ ಖಾಲಿ;
#ಆದರೆ_ಅಂತ್ಯ
}

}  // namespace cv::
