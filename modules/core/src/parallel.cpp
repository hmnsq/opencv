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
    #अपरिभाषय small
    #अपरिभाषय min
    #अपरिभाषय max
    #अपरिभाषय निरपेक्षम्
#यदि_अन्तः

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
    #नोचेद्यदि defined __APPLE__
        #अन्तर्भाव <sys/sysctl.h>
    #यदि_अन्तः
#यदि_अन्तः

#यदि defined (__QNX__)
    #अन्तर्भाव <sys/syspage.h>
#यदि_अन्तः

#यदि_अपरिभाषितम् OPENCV_DISABLE_THREAD_SUPPORT
    #अन्तर्भाव <thread>
#यदि_अन्तः

#यदि_परिभाषितम् _OPENMP
    #परिभाषय HAVE_OPENMP
#यदि_अन्तः

#यदि_परिभाषितम् __APPLE__
    #परिभाषय HAVE_GCD
#यदि_अन्तः

#यदि defined _MSC_VER
    #परिभाषय HAVE_CONCURRENCY
#यदि_अन्तः

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
    #यदि_अपरिभाषितम् TBB_SUPPRESS_DEPRECATED_MESSAGES  // supress warning
    #परिभाषय TBB_SUPPRESS_DEPRECATED_MESSAGES 1
    #यदि_अन्तः
    #अन्तर्भाव "tbb/tbb.h"
    #अन्तर्भाव "tbb/task.h"
    #यदि TBB_INTERFACE_संस्करणम् >= 8000
        #अन्तर्भाव "tbb/task_arena.h"
    #यदि_अन्तः
    #अपरिभाषय min
    #अपरिभाषय max
#नोचेद्यदि defined HAVE_HPX
    #अन्तर्भाव <hpx/parallel/algorithms/for_loop.hpp>
    #अन्तर्भाव <hpx/parallel/execution.hpp>
    //
    #अन्तर्भाव <hpx/hpx_start.hpp>
    #अन्तर्भाव <hpx/hpx_suspend.hpp>
    #अन्तर्भाव <hpx/include/apply.hpp>
    #अन्तर्भाव <hpx/util/yield_while.hpp>
    #अन्तर्भाव <hpx/include/threadmanager.hpp>

#नोचेद्यदि defined HAVE_OPENMP
    #अन्तर्भाव <omp.h>
#नोचेद्यदि defined HAVE_GCD
    #अन्तर्भाव <dispatch/dispatch.h>
    #अन्तर्भाव <pthread.h>
#नोचेद्यदि defined HAVE_CONCURRENCY
    #अन्तर्भाव <ppl.h>
#नोचेद्यदि defined HAVE_PTHREADS_PF
    #अन्तर्भाव <pthread.h>
#यदि_अन्तः


#यदि defined HAVE_TBB
#  define CV_PARALLEL_FRAMEWORK "tbb"
#नोचेद्यदि defined HAVE_HPX
#  define CV_PARALLEL_FRAMEWORK "hpx"
#नोचेद्यदि defined HAVE_OPENMP
#  define CV_PARALLEL_FRAMEWORK "openmp"
#नोचेद्यदि defined HAVE_GCD
#  define CV_PARALLEL_FRAMEWORK "gcd"
#नोचेद्यदि defined WINRT
#  define CV_PARALLEL_FRAMEWORK "winrt-concurrency"
#नोचेद्यदि defined HAVE_CONCURRENCY
#  define CV_PARALLEL_FRAMEWORK "ms-concurrency"
#नोचेद्यदि defined HAVE_PTHREADS_PF
#  define CV_PARALLEL_FRAMEWORK "pthreads"
#यदि_अन्तः

#अन्तर्भाव <atomic>

#अन्तर्भाव "parallel_impl.hpp"

#अन्तर्भाव "opencv2/core/detail/exception_ptr.hpp"  // CV__EXCEPTION_PTR = 1 if std::exception_ptr is available

#अन्तर्भाव <opencv2/core/utils/fp_control_utils.hpp>
#अन्तर्भाव <opencv2/core/utils/fp_control.private.hpp>

उपयुज्य नामाकाश cv;

नामाकाश cv {

ParallelLoopBody::~ParallelLoopBody() {}

उपयुज्य नामाकाश cv::parallel;

नामाकाश {

#यदि_परिभाषितम् ENABLE_INSTRUMENTATION
    स्थिर शून्य SyncNodes(cv::instr::Instr_ग्रन्थिः *p_ग्रन्थिः)
    {
        std::vector<cv::instr::ग्रन्थिः_दत्त_Tls*> दत्त;
        p_ग्रन्थिः->m_payload.m_tls.gather(दत्त);

        uint64 ticksMax = 0;
        पूर्णाङ्क    तन्तवः  = 0;
        प्रति(size_t i = 0; i < दत्त.size(); i++)
        {
            यदि(दत्त[i] && दत्त[i]->m_ticks_योग)
            {
                ticksMax = MAX(ticksMax, दत्त[i]->m_ticks_योग);
                p_ग्रन्थिः->m_payload.m_ticks_योग -= दत्त[i]->m_ticks_योग;
                दत्त[i]->m_ticks_योग = 0;
                तन्तवः++;
            }
        }
        p_ग्रन्थिः->m_payload.m_ticks_योग += ticksMax;
        p_ग्रन्थिः->m_payload.m_तन्तवः = MAX(p_ग्रन्थिः->m_payload.m_तन्तवः, तन्तवः);

        प्रति(size_t i = 0; i < p_ग्रन्थिः->m_childs.size(); i++)
            SyncNodes(p_ग्रन्थिः->m_childs[i]);
    }
#यदि_अन्तः

    वर्ग ParallelLoopBodyWrapperContext
    {
    सार्वजनिक:
        ParallelLoopBodyWrapperContext(ध्रुव cv::ParallelLoopBody& _body, ध्रुव cv::Range& _r, द्विगुण _nstripes) :
            is_rng_प्रयुक्तम्(असत्य), hasException(असत्य)
        {

            body = &_body;
            सम्पूर्णम्_Range = _r;
            द्विगुण len = सम्पूर्णम्_Range.end - सम्पूर्णम्_Range.start;
            nstripes = cv_आवर्तनम्(_nstripes <= 0 ? len : MIN(MAX(_nstripes, 1.), len));

            // propagate main thread state
            rng = cv::theRNG();
#यदि OPENCV_SUPPORTS_FP_DENORMALS_HINT && OPENCV_IMPL_FP_HINTS
            details::saveFPDenormalsState(fp_denormals_आधारः_state);
#यदि_अन्तः

#यदि_परिभाषितम् OPENCV_TRACE
            trace_मूलम्_Region = CV_TRACE_NS::details::getCurrentRegion();
            trace_मूलम्_Context = CV_TRACE_NS::details::getTraceManager().tls.get();
#यदि_अन्तः

#यदि_परिभाषितम् ENABLE_INSTRUMENTATION
            p_Thread_मूलम् = cv::instr::getInstrumentTLSStruct().p_Current_ग्रन्थिः;
#यदि_अन्तः
        }
        शून्य finalize()
        {
#यदि_परिभाषितम् ENABLE_INSTRUMENTATION
            प्रति(size_t i = 0; i < p_Thread_मूलम्->m_childs.size(); i++)
                SyncNodes(p_Thread_मूलम्->m_childs[i]);
#यदि_अन्तः
            यदि (is_rng_प्रयुक्तम्)
            {
                // Some parallel backends execute nested jobs in the main thread,
                // so we need to restore initial RNG state here.
                cv::theRNG() = rng;
                // We can't properly update RNG state based on RNG usage in worker threads,
                // so lets just change main thread RNG state to the next value.
                // Note: this behaviour is not equal to single-threaded mode.
                cv::theRNG().next();
            }
#यदि_परिभाषितम् OPENCV_TRACE
            यदि (trace_मूलम्_Region)
                CV_TRACE_NS::details::parallelForFinalize(*trace_मूलम्_Region);
#यदि_अन्तः

            यदि (hasException)
            {
#यदि CV__EXCEPTION_PTR
                std::rethrow_exception(pException);
#अन्यथा
                CV_Error(Error::StsError, "Exception in parallel_for() body: " + exception_सन्देशः);
#यदि_अन्तः
            }
        }
        ~ParallelLoopBodyWrapperContext() {}

        ध्रुव cv::ParallelLoopBody* body;
        cv::Range सम्पूर्णम्_Range;
        पूर्णाङ्क nstripes;
        cv::RNG rng;
        परिवर्तनीय तार्किक is_rng_प्रयुक्तम्;
#यदि_परिभाषितम् OPENCV_TRACE
        CV_TRACE_NS::details::Region* trace_मूलम्_Region;
        CV_TRACE_NS::details::TraceManagerThreadLocal* trace_मूलम्_Context;
#यदि_अन्तः
#यदि_परिभाषितम् ENABLE_INSTRUMENTATION
        cv::instr::Instr_ग्रन्थिः *p_Thread_मूलम्;
#यदि_अन्तः
        तार्किक hasException;
#यदि CV__EXCEPTION_PTR
        std::exception_ptr pException;
#अन्यथा
        cv::String exception_सन्देशः;
#यदि_अन्तः
#यदि CV__EXCEPTION_PTR
        शून्य लेखः_Exception()
#अन्यथा
        शून्य लेखः_Exception(ध्रुव cv::String& सन्देशः)
#यदि_अन्तः
        {
#यदि_अपरिभाषितम् CV_THREAD_SANITIZER
            यदि (!hasException)
#यदि_अन्तः
            {
                cv::Auto_तालकम् lock(cv::getInitializationMutex());
                यदि (!hasException)
                {
                    hasException = सत्य;
#यदि CV__EXCEPTION_PTR
                    pException = std::current_exception();
#अन्यथा
                    exception_सन्देशः = सन्देशः;
#यदि_अन्तः
                }
            }
        }

#यदि OPENCV_SUPPORTS_FP_DENORMALS_HINT && OPENCV_IMPL_FP_HINTS
        details::FPDenormals_बहुलकम्_State fp_denormals_आधारः_state;
#यदि_अन्तः

    गुप्त:
        ParallelLoopBodyWrapperContext(ध्रुव ParallelLoopBodyWrapperContext&); // disabled
        ParallelLoopBodyWrapperContext& संक्रियकः=(ध्रुव ParallelLoopBodyWrapperContext&); // disabled
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
        शून्य संक्रियकः()(ध्रुव cv::Range& sr) ध्रुव CV_OVERRIDE
        {
#यदि_परिभाषितम् OPENCV_TRACE
            // TODO CV_TRACE_NS::details::setCurrentRegion(rootRegion);
            यदि (ctx.trace_मूलम्_Region && ctx.trace_मूलम्_Context)
                CV_TRACE_NS::details::parallel_For_समुच्चयः_मूलम्_Region(*ctx.trace_मूलम्_Region, *ctx.trace_मूलम्_Context);
            CV__TRACE_OPENCV_FUNCTION_NAME("parallel_for_body");
            यदि (ctx.trace_मूलम्_Region)
                CV_TRACE_NS::details::parallelForAttachNestedRegion(*ctx.trace_मूलम्_Region);
#यदि_अन्तः

#यदि_परिभाषितम् ENABLE_INSTRUMENTATION
            {
                cv::instr::InstrTLSStruct *pInstrTLS = &cv::instr::getInstrumentTLSStruct();
                pInstrTLS->p_Current_ग्रन्थिः = ctx.p_Thread_मूलम्; // Initialize TLS node for thread
            }
            CV_INSTRUMENT_REGION();
#यदि_अन्तः

            // propagate main thread state
            cv::theRNG() = ctx.rng;
#यदि OPENCV_SUPPORTS_FP_DENORMALS_HINT && OPENCV_IMPL_FP_HINTS
            FPDenormalsIgnoreHintScope fp_denormals_scope(ctx.fp_denormals_आधारः_state);
#यदि_अन्तः

            cv::Range r;
            cv::Range सम्पूर्णम्_Range = ctx.सम्पूर्णम्_Range;
            पूर्णाङ्क nstripes = ctx.nstripes;
            r.start = (पूर्णाङ्क)(सम्पूर्णम्_Range.start +
                            ((uint64)sr.start*(सम्पूर्णम्_Range.end - सम्पूर्णम्_Range.start) + nstripes/2)/nstripes);
            r.end = sr.end >= nstripes ? सम्पूर्णम्_Range.end : (पूर्णाङ्क)(सम्पूर्णम्_Range.start +
                            ((uint64)sr.end*(सम्पूर्णम्_Range.end - सम्पूर्णम्_Range.start) + nstripes/2)/nstripes);

#यदि_परिभाषितम् OPENCV_TRACE
            CV_TRACE_ARG_मानम्(range_start, "range.start", (int64)r.start);
            CV_TRACE_ARG_मानम्(range_अन्तः, "range.end", (int64)r.end);
#यदि_अन्तः

            यत्न
            {
                (*ctx.body)(r);
            }
#यदि CV__EXCEPTION_PTR
            गृहाण (...)
            {
                ctx.लेखः_Exception();
            }
#अन्यथा
            गृहाण (ध्रुव cv::Exception& e)
            {
                ctx.लेखः_Exception(e.what());
            }
            गृहाण (ध्रुव std::exception& e)
            {
                ctx.लेखः_Exception(e.what());
            }
            गृहाण (...)
            {
                ctx.लेखः_Exception("Unknown exception");
            }
#यदि_अन्तः

            यदि (!ctx.is_rng_प्रयुक्तम् && !(cv::theRNG() == ctx.rng))
                ctx.is_rng_प्रयुक्तम् = सत्य;
        }
        cv::Range stripeRange() ध्रुव { प्रत्यागम cv::Range(0, ctx.nstripes); }

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

        शून्य संक्रियकः ()(ध्रुव tbb::blocked_range<पूर्णाङ्क>& range) ध्रुव
        {
            एतत्->ParallelLoopBodyWrapper::संक्रियकः()(cv::Range(range.begin(), range.end()));
        }

        शून्य संक्रियकः ()() ध्रुव  // run parallel job
        {
            cv::Range range = एतत्->stripeRange();
            tbb::parallel_for(tbb::blocked_range<पूर्णाङ्क>(range.start, range.end), *एतत्);
        }
    };
#नोचेद्यदि defined HAVE_HPX
    वर्ग ProxyLoopBody : सार्वजनिक ParallelLoopBodyWrapper
    {
    सार्वजनिक:
        ProxyLoopBody(ParallelLoopBodyWrapperContext& ctx_)
                : ParallelLoopBodyWrapper(ctx_)
        {}

        शून्य संक्रियकः ()() ध्रुव  // run parallel job
        {
            cv::Range stripeRange = एतत्->stripeRange();
            hpx::parallel::for_loop(
                    hpx::parallel::execution::par,
                    stripeRange.start, stripeRange.end,
                    [&](ध्रुव पूर्णाङ्क &i) { ;
                        एतत्->ParallelLoopBodyWrapper::संक्रियकः()(
                                cv::Range(i, i + 1));
                    });
        }
    };
#नोचेद्यदि defined HAVE_OPENMP
    प्रकारनिर्देश ParallelLoopBodyWrapper ProxyLoopBody;
#नोचेद्यदि defined HAVE_GCD
    प्रकारनिर्देश ParallelLoopBodyWrapper ProxyLoopBody;
    स्थिर शून्य खण्डम्_function(शून्य* context, size_t सूचकांक)
    {
        ProxyLoopBody* ptr_body = static_cast<ProxyLoopBody*>(context);
        (*ptr_body)(cv::Range((पूर्णाङ्क)सूचकांक, (पूर्णाङ्क)सूचकांक + 1));
    }
#नोचेद्यदि defined WINRT || defined HAVE_CONCURRENCY
    वर्ग ProxyLoopBody : सार्वजनिक ParallelLoopBodyWrapper
    {
    सार्वजनिक:
        ProxyLoopBody(ParallelLoopBodyWrapperContext& ctx)
        : ParallelLoopBodyWrapper(ctx)
        {}

        शून्य संक्रियकः ()(पूर्णाङ्क i) ध्रुव
        {
            एतत्->ParallelLoopBodyWrapper::संक्रियकः()(cv::Range(i, i + 1));
        }
    };

    // PPL's default auto_partitioner silently skips whole chunks of the range on
    // ARM64 (unfixed MSVC bug, developercommunity.visualstudio.com/t/1027444).
    प्रतिरूपम् <प्रकारनाम Body>
    स्थिर अन्तःस्थ शून्य pplParallelFor(पूर्णाङ्क first, पूर्णाङ्क चरमः, ध्रुव Body& body)
    {
#यदि defined(_M_ARM64) || defined(_M_ARM64EC)
        Concurrency::parallel_for(first, चरमः, body, Concurrency::static_partitioner());
#अन्यथा
        Concurrency::parallel_for(first, चरमः, body);
#यदि_अन्तः
    }
#अन्यथा
    प्रकारनिर्देश ParallelLoopBodyWrapper ProxyLoopBody;
#यदि_अन्तः

#यदि defined HAVE_TBB
    #यदि TBB_INTERFACE_संस्करणम् >= 8000
        स्थिर tbb::task_arena tbbArena(tbb::task_arena::automatic);
    #अन्यथा
        स्थिर tbb::task_scheduler_init tbbScheduler(tbb::task_scheduler_init::deferred);
    #यदि_अन्तः
#नोचेद्यदि defined HAVE_HPX
// nothing for HPX
#नोचेद्यदि defined HAVE_OPENMP
स्थिर अन्तःस्थ पूर्णाङ्क init_Max_तन्तवः()
{
    पूर्णाङ्क max_तन्तवः = omp_get_max_तन्तवः();
    प्रत्यागम max_तन्तवः;
}
स्थिर पूर्णाङ्क अङ्कः_तन्तवः_Max = init_Max_तन्तवः();
#नोचेद्यदि defined HAVE_GCD
// nothing for GCD
#नोचेद्यदि defined WINRT
// nothing for WINRT
#नोचेद्यदि defined HAVE_CONCURRENCY

वर्ग SchedPtr
{
    Concurrency::Scheduler* sched_;
सार्वजनिक:
    Concurrency::Scheduler* संक्रियकः->() { प्रत्यागम sched_; }
    संक्रियकः Concurrency::Scheduler*() { प्रत्यागम sched_; }

    शून्य संक्रियकः=(Concurrency::Scheduler* sched)
    {
        यदि (sched_) sched_->Release();
        sched_ = sched;
    }

    SchedPtr() : sched_(0) {}
    ~SchedPtr() {}
};
स्थिर SchedPtr pplScheduler;

#यदि_अन्तः

} // namespace anon

/* ================================   parallel_for_  ================================ */

स्थिर शून्य parallel_for_impl(ध्रुव cv::Range& range, ध्रुव cv::ParallelLoopBody& body, द्विगुण nstripes); // forward declaration

शून्य parallel_for_(ध्रुव cv::Range& range, ध्रुव cv::ParallelLoopBody& body, द्विगुण nstripes)
{
#यदि_परिभाषितम् OPENCV_TRACE
    CV__TRACE_OPENCV_FUNCTION_NAME_("parallel_for", 0);
    CV_TRACE_ARG_मानम्(range_start, "range.start", (int64)range.start);
    CV_TRACE_ARG_मानम्(range_अन्तः, "range.end", (int64)range.end);
    CV_TRACE_ARG_मानम्(nstripes, "nstripes", (int64)nstripes);
#यदि_अन्तः

    CV_INSTRUMENT_REGION_MT_FORK();
    यदि (range.empty())
        प्रत्यागम;

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
        CV_अप्रयुक्तम्(nstripes);
        body(range);
    }
}

स्थिर
शून्य parallel_for_cb(पूर्णाङ्क start, पूर्णाङ्क end, शून्य* दत्त)
{
    CV_DbgAssert(दत्त);
    ध्रुव cv::ParallelLoopBody& body = *(ध्रुव cv::ParallelLoopBody*)दत्त;
    body(Range(start, end));
}

स्थिर शून्य parallel_for_impl(ध्रुव cv::Range& range, ध्रुव cv::ParallelLoopBody& body, द्विगुण nstripes)
{
    उपयुज्य नामाकाश cv::parallel;
    यदि ((अङ्कः_तन्तवः < 0 || अङ्कः_तन्तवः > 1) && range.end - range.start > 1)
    {
        ParallelLoopBodyWrapperContext ctx(body, range, nstripes);
        ProxyLoopBody pbody(ctx);
        cv::Range stripeRange = pbody.stripeRange();
        यदि( stripeRange.end - stripeRange.start == 1 )
        {
            body(range);
            प्रत्यागम;
        }

        std::shared_ptr<ParallelForAPI>& api = getCurrentParallelForAPI();
        यदि (api)
        {
            CV_CheckEQ(stripeRange.start, 0, "");
            api->parallel_for(stripeRange.end, parallel_for_cb, (शून्य*)&pbody);
            ctx.finalize();  // propagate exceptions if exists
            प्रत्यागम;
        }

#यदि_परिभाषितम् CV_PARALLEL_FRAMEWORK
#यदि defined HAVE_TBB

#यदि TBB_INTERFACE_संस्करणम् >= 8000
        tbbArena.execute(pbody);
#अन्यथा
        pbody();
#यदि_अन्तः

#नोचेद्यदि defined HAVE_HPX
        pbody();

#नोचेद्यदि defined HAVE_OPENMP

        #निर्देशः omp parallel प्रति कालसारणी(dynamic) अङ्कः_तन्तवः_2(अङ्कः_तन्तवः > 0 ? अङ्कः_तन्तवः : अङ्कः_तन्तवः_Max)
        प्रति (पूर्णाङ्क i = stripeRange.start; i < stripeRange.end; ++i)
            pbody(Range(i, i + 1));

#नोचेद्यदि defined HAVE_GCD

        dispatch_queue_t concurrent_queue = dispatch_get_global_queue(DISPATCH_QUEUE_प्राथमिकता_DEFAULT, 0);
        dispatch_apply_f(stripeRange.end - stripeRange.start, concurrent_queue, &pbody, खण्डम्_function);

#नोचेद्यदि defined WINRT

        pplParallelFor(stripeRange.start, stripeRange.end, pbody);

#नोचेद्यदि defined HAVE_CONCURRENCY

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

#नोचेद्यदि defined HAVE_PTHREADS_PF

        parallel_for_pthreads(pbody.stripeRange(), pbody, pbody.stripeRange().size());

#अन्यथा

#दोषः You have hacked and compiling with unsupported parallel framework

#यदि_अन्तः

        ctx.finalize();  // propagate exceptions if exists
        प्रत्यागम;
#यदि_अन्तः // CV_PARALLEL_FRAMEWORK
    }

    body(range);
}


पूर्णाङ्क get_अङ्कः_तन्तवः(शून्य)
{
    std::shared_ptr<ParallelForAPI>& api = getCurrentParallelForAPI();
    यदि (api)
    {
        प्रत्यागम api->get_अङ्कः_तन्तवः();
    }

    यदि (अङ्कः_तन्तवः == 0)
        प्रत्यागम 1;

#यदि defined HAVE_TBB

#यदि TBB_INTERFACE_संस्करणम् >= 9100
    प्रत्यागम tbbArena.max_concurrency();
#नोचेद्यदि TBB_INTERFACE_संस्करणम् >= 8000
    प्रत्यागम अङ्कः_तन्तवः > 0
        ? अङ्कः_तन्तवः
        : tbb::task_scheduler_init::default_अङ्कः_तन्तवः();
#अन्यथा
    प्रत्यागम tbbScheduler.is_सक्रिय()
           ? अङ्कः_तन्तवः
           : tbb::task_scheduler_init::default_अङ्कः_तन्तवः();
#यदि_अन्तः

#नोचेद्यदि defined HAVE_HPX
    प्रत्यागम अङ्कः_तन्तवः;

#नोचेद्यदि defined HAVE_OPENMP

    प्रत्यागम अङ्कः_तन्तवः > 0
           ? अङ्कः_तन्तवः
           : अङ्कः_तन्तवः_Max;


#नोचेद्यदि defined HAVE_GCD

    प्रत्यागम cv::get_सङ्ख्या_सम्बन्धि_CPUs(); // the GCD thread pool limit

#नोचेद्यदि defined WINRT

    प्रत्यागम 0;

#नोचेद्यदि defined HAVE_CONCURRENCY

    प्रत्यागम (pplScheduler == 0)
        ? Concurrency::CurrentScheduler::Get()->Get_सङ्ख्या_सम्बन्धि_Virtual_Processors()
        : (1 + pplScheduler->Get_सङ्ख्या_सम्बन्धि_Virtual_Processors());

#नोचेद्यदि defined HAVE_PTHREADS_PF

        प्रत्यागम parallel_pthreads_get_तन्तवः_अङ्कः();

#अन्यथा

    प्रत्यागम 1;

#यदि_अन्तः
}

अचिह्नित default_सङ्ख्या_सम्बन्धि_तन्तवः()
{
#यदि_परिभाषितम् __ANDROID__
    // many modern phones/tables have 4-core CPUs. Let's use no more
    // than 2 threads by default not to overheat the devices
    ध्रुव अचिह्नित पूर्णाङ्क default_सङ्ख्या_सम्बन्धि_तन्तवः_2 = 2;
#अन्यथा
    ध्रुव अचिह्नित पूर्णाङ्क default_सङ्ख्या_सम्बन्धि_तन्तवः_2 = (अचिह्नित पूर्णाङ्क)std::max(1, cv::get_सङ्ख्या_सम्बन्धि_CPUs());
#यदि_अन्तः

    अचिह्नित परिणाम = default_सङ्ख्या_सम्बन्धि_तन्तवः_2;

    स्थिर पूर्णाङ्क config_अङ्कः_तन्तवः = (पूर्णाङ्क)utils::get_Configuration_Parameter_आकार_T("OPENCV_FOR_THREADS_NUM", 0);

    यदि (config_अङ्कः_तन्तवः)
    {
        परिणाम = (अचिह्नित)std::max(1, config_अङ्कः_तन्तवः);
        //do we need upper limit of threads number?
    }
    प्रत्यागम परिणाम;
}

शून्य समुच्चयः_अङ्कः_तन्तवः( पूर्णाङ्क तन्तवः_2 )
{
    CV_अप्रयुक्तम्(तन्तवः_2);

    पूर्णाङ्क तन्तवः = (तन्तवः_2 < 0) ? default_सङ्ख्या_सम्बन्धि_तन्तवः() : (अचिह्नित)तन्तवः_2;
    अङ्कः_तन्तवः = तन्तवः;

    std::shared_ptr<ParallelForAPI>& api = getCurrentParallelForAPI();
    यदि (api)
    {
        api->समुच्चयः_अङ्कः_तन्तवः(अङ्कः_तन्तवः);
    }

#यदि_परिभाषितम् HAVE_TBB

#यदि TBB_INTERFACE_संस्करणम् >= 8000
    यदि(tbbArena.is_सक्रिय()) tbbArena.terminate();
    यदि(तन्तवः > 0) tbbArena.initialize(तन्तवः);
#अन्यथा
    यदि(tbbScheduler.is_सक्रिय()) tbbScheduler.terminate();
    यदि(तन्तवः > 0) tbbScheduler.initialize(तन्तवः);
#यदि_अन्तः

#नोचेद्यदि defined HAVE_HPX
    प्रत्यागम; // nothing needed as numThreads is used

#नोचेद्यदि defined HAVE_OPENMP

    प्रत्यागम; // nothing needed as num_threads clause is used in #pragma omp parallel for

#नोचेद्यदि defined HAVE_GCD

    // unsupported
    // there is only private dispatch_queue_set_width() and only for desktop

#नोचेद्यदि defined WINRT

    प्रत्यागम;

#नोचेद्यदि defined HAVE_CONCURRENCY

    यदि (तन्तवः <= 0)
    {
        pplScheduler = 0;
    }
    अन्यथा यदि (तन्तवः == 1)
    {
        // Concurrency always uses >=2 threads, so we just disable it if 1 thread is requested
        अङ्कः_तन्तवः = 0;
    }
    अन्यथा यदि (pplScheduler == 0 || 1 + pplScheduler->Get_सङ्ख्या_सम्बन्धि_Virtual_Processors() != (अचिह्नित पूर्णाङ्क)तन्तवः)
    {
        pplScheduler = Concurrency::Scheduler::Create(Concurrency::SchedulerPolicy(2,
                       Concurrency::MinConcurrency, तन्तवः-1,
                       Concurrency::MaxConcurrency, तन्तवः-1));
    }

#नोचेद्यदि defined HAVE_PTHREADS_PF

    parallel_pthreads_समुच्चयः_तन्तवः_अङ्कः(तन्तवः);

#यदि_अन्तः
}


पूर्णाङ्क get_Thread_अङ्कः()
{
    std::shared_ptr<ParallelForAPI>& api = getCurrentParallelForAPI();
    यदि (api)
    {
        प्रत्यागम api->get_Thread_अङ्कः();
    }

#यदि defined HAVE_TBB
    #यदि TBB_INTERFACE_संस्करणम् >= 9100
        प्रत्यागम tbb::this_task_arena::current_thread_सूचकांक();
    #नोचेद्यदि TBB_INTERFACE_संस्करणम् >= 8000
        प्रत्यागम tbb::task_arena::current_thread_सूचकांक();
    #अन्यथा
        प्रत्यागम 0;
    #यदि_अन्तः
#नोचेद्यदि defined HAVE_HPX
    प्रत्यागम (पूर्णाङ्क)(hpx::get_अङ्कः_कार्यकर्ता_तन्तवः());
#नोचेद्यदि defined HAVE_OPENMP
    प्रत्यागम omp_get_thread_अङ्कः();
#नोचेद्यदि defined HAVE_GCD
    प्रत्यागम (पूर्णाङ्क)(size_t)(शून्य*)pthread_self(); // no zero-based indexing
#नोचेद्यदि defined WINRT
    प्रत्यागम 0;
#नोचेद्यदि defined HAVE_CONCURRENCY
    प्रत्यागम std::max(0, (पूर्णाङ्क)Concurrency::Context::VirtualProcessorId()); // zero for master thread, unique number for others but not necessary 1,2,3,...
#नोचेद्यदि defined HAVE_PTHREADS_PF
    प्रत्यागम (पूर्णाङ्क)(size_t)(शून्य*)pthread_self(); // no zero-based indexing
#अन्यथा
    प्रत्यागम 0;
#यदि_अन्तः
}


#यदि defined __linux__ || defined __GLIBC__ || defined __HAIKU__ || defined __ANDROID__
  #परिभाषय CV_CPU_GROUPS_1
#यदि_अन्तः

#यदि defined __linux__ || defined __ANDROID__
  #परिभाषय CV_HAVE_CGROUPS 1
#यदि_अन्तः

#यदि defined CV_CPU_GROUPS_1
स्थिर अन्तःस्थ
std::string getFileContents(ध्रुव वर्ण *filename)
{
    std::ifstream ifs(filename);
    यदि (!ifs.is_open())
        प्रत्यागम std::string();

    std::string content( (std::istreambuf_iterator<वर्ण>(ifs) ),
                         (std::istreambuf_iterator<वर्ण>()    ) );

    यदि (ifs.fail())
        प्रत्यागम std::string();

    प्रत्यागम content;
}

स्थिर अन्तःस्थ
पूर्णाङ्क get_सङ्ख्या_सम्बन्धि_CPUs_Impl(ध्रुव वर्ण *filename)
{
   std::string file_contents = getFileContents(filename);
   यदि(file_contents.empty())
       प्रत्यागम 0;

   वर्ण *pbuf = const_cast<वर्ण*>(file_contents.c_str());
   //parse string of form "0-1,3,5-7,10,13-15"
   पूर्णाङ्क cpus_उपलब्धम् = 0;

   यावत्(*pbuf)
   {
      ध्रुव वर्ण* pos = pbuf;
      तार्किक range = असत्य;
      यावत्(*pbuf && *pbuf != ',')
      {
          यदि(*pbuf == '-') range = सत्य;
          ++pbuf;
      }
      यदि(*pbuf) *pbuf++ = 0;
      यदि(!range)
        ++cpus_उपलब्धम्;
      अन्यथा
      {
          पूर्णाङ्क rstart = 0, rend = 0;
          वाक्यात्_पठ(pos, "%d-%d", &rstart, &rend);
          cpus_उपलब्धम् += rend - rstart + 1;
      }

   }
   प्रत्यागम cpus_उपलब्धम्;
}
#यदि_अन्तः

#यदि defined CV_HAVE_CGROUPS
स्थिर अन्तःस्थ
अचिह्नित get_सङ्ख्या_सम्बन्धि_CPUs_CFSv2()
{
    पूर्णाङ्क cfs_quota = 0;
    पूर्णाङ्क cfs_period = 0;

    std::ifstream ss_cpu_max("/sys/fs/cgroup/cpu.max", std::ios::in | std::ios::binary);
    ss_cpu_max >> cfs_quota >> cfs_period;

    यदि (ss_cpu_max.fail() || cfs_quota < 1 || cfs_period < 1) /* values must not be 0 or negative */
        प्रत्यागम 0;

    प्रत्यागम (अचिह्नित)max(1, cfs_quota/cfs_period);
}

स्थिर अन्तःस्थ
अचिह्नित get_सङ्ख्या_सम्बन्धि_CPUs_CFSv1()
{
    पूर्णाङ्क cfs_quota = 0;
    {
        std::ifstream ss_period("/sys/fs/cgroup/cpu/cpu.cfs_quota_us", std::ios::in | std::ios::binary);
        ss_period >> cfs_quota;

        यदि (ss_period.fail() || cfs_quota < 1) /* cfs_quota must not be 0 or negative */
            प्रत्यागम 0;
    }

    पूर्णाङ्क cfs_period = 0;
    {
        std::ifstream ss_quota("/sys/fs/cgroup/cpu/cpu.cfs_period_us", std::ios::in | std::ios::binary);
        ss_quota >> cfs_period;

        यदि (ss_quota.fail() || cfs_period < 1)
            प्रत्यागम 0;
    }

    प्रत्यागम (अचिह्नित)max(1, cfs_quota/cfs_period);
}
#यदि_अन्तः

प्रतिरूपम् <प्रकारनाम T> स्थिर अन्तःस्थ
T minNonZero(ध्रुव T& val_1, ध्रुव T& val_2)
{
    यदि ((val_1 != 0) && (val_2 != 0))
        प्रत्यागम std::min(val_1, val_2);
    प्रत्यागम (val_1 != 0) ? val_1 : val_2;
}

#यदि_अपरिभाषितम् OPENCV_DISABLE_THREAD_SUPPORT
स्थिर
पूर्णाङ्क get_सङ्ख्या_सम्बन्धि_CPUs_2()
{
#यदि_अपरिभाषितम् OPENCV_SEMIHOSTING
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
    अचिह्नित ncpus = std::thread::hardware_concurrency(); /* If the value is not well defined or not computable, returns 0 */
#अन्यथा
    अचिह्नित ncpus = 0; /* 0 means we have to find out some other way */
#यदि_अन्तः

#यदि defined _WIN32

    SYSTEM_INFO sysinfo = {};
#यदि (defined(_M_ARM) || defined(_M_ARM64) || defined(_M_ARM64EC) || defined(_M_X64) || defined(WINRT)) && _WIN32_WINNT >= 0x501
    GetNativeSystemInfo( &sysinfo );
#अन्यथा
    GetSystemInfo( &sysinfo );
#यदि_अन्तः
    अचिह्नित ncpus_sysinfo = sysinfo.dw_सङ्ख्या_सम्बन्धि_Processors;
    ncpus = minNonZero(ncpus, ncpus_sysinfo);

#नोचेद्यदि defined __APPLE__

    पूर्णाङ्क अङ्कः_CPU=0;
    पूर्णाङ्क mib[4];
    size_t len = परिमाण(अङ्कः_CPU);

    /* set the mib for hw.ncpu */
    mib[0] = CTL_HW;
    mib[1] = HW_AVAILCPU;  // alternatively, try HW_NCPU;

    /* get the number of CPUs from the system */
    sysctl(mib, 2, &अङ्कः_CPU, &len, रिक्त, 0);

    यदि( अङ्कः_CPU < 1 )
    {
        mib[1] = HW_NCPU;
        sysctl( mib, 2, &अङ्कः_CPU, &len, रिक्त, 0 );

        यदि( अङ्कः_CPU < 1 )
            अङ्कः_CPU = 1;
    }

    ncpus = minNonZero(ncpus, (अचिह्नित)अङ्कः_CPU);

#नोचेद्यदि defined CV_CPU_GROUPS_1

#यदि defined CV_HAVE_CGROUPS
    स्थिर अचिह्नित ncpus_impl_cpuset = (अचिह्नित)get_सङ्ख्या_सम्बन्धि_CPUs_Impl("/sys/fs/cgroup/cpuset/cpuset.cpus");
    ncpus = minNonZero(ncpus, ncpus_impl_cpuset);

    स्थिर अचिह्नित ncpus_impl_cfs_v1 = get_सङ्ख्या_सम्बन्धि_CPUs_CFSv1();
    ncpus = minNonZero(ncpus, ncpus_impl_cfs_v1);

    स्थिर अचिह्नित ncpus_impl_cfs_v2 = get_सङ्ख्या_सम्बन्धि_CPUs_CFSv2();
    ncpus = minNonZero(ncpus, ncpus_impl_cfs_v2);
#यदि_अन्तः

    स्थिर अचिह्नित ncpus_impl_devices = (अचिह्नित)get_सङ्ख्या_सम्बन्धि_CPUs_Impl("/sys/devices/system/cpu/online");
    ncpus = minNonZero(ncpus, ncpus_impl_devices);

#यदि_अन्तः

#यदि defined _GNU_SOURCE \
    && !defined(__MINGW32__) /* not implemented (2020-03) */ \
    && !defined(__EMSCRIPTEN__) \
    && !defined(__ANDROID__)  // TODO: add check for modern Android NDK

    cpu_set_t cpu_समुच्चयः;
    यदि (0 == sched_getaffinity(0, परिमाण(cpu_समुच्चयः), &cpu_समुच्चयः))
    {
        अचिह्नित cpu_गणनम्_cpu_समुच्चयः = CPU_COUNT(&cpu_समुच्चयः);
        ncpus = minNonZero(ncpus, cpu_गणनम्_cpu_समुच्चयः);
    }

#यदि_अन्तः

#यदि !defined(_WIN32) && !defined(__APPLE__) && defined(_SC_NPROCESSORS_ONLN)

    स्थिर अचिह्नित cpu_गणनम्_sysconf = (अचिह्नित)sysconf( _SC_NPROCESSORS_ONLN );
    ncpus = minNonZero(ncpus, cpu_गणनम्_sysconf);
#नोचेद्यदि defined (__QNX__)
    स्थिर अचिह्नित cpu_गणनम्_sysconf = _syspage_ptr->अङ्कः_cpu;
    ncpus = minNonZero(ncpus, cpu_गणनम्_sysconf);
#यदि_अन्तः

    प्रत्यागम ncpus != 0 ? ncpus : 1;
#अन्यथा //  OPENCV_SEMIHOSTING
    प्रत्यागम 1;
#यदि_अन्तः //OPENCV_SEMIHOSTING
}

पूर्णाङ्क get_सङ्ख्या_सम्बन्धि_CPUs()
{
    स्थिर पूर्णाङ्क nCPUs = get_सङ्ख्या_सम्बन्धि_CPUs_2();
    प्रत्यागम nCPUs;  // cached value
}

#अन्यथा  // OPENCV_DISABLE_THREAD_SUPPORT
पूर्णाङ्क get_सङ्ख्या_सम्बन्धि_CPUs()
{
    प्रत्यागम 1;
}
#यदि_अन्तः  // OPENCV_DISABLE_THREAD_SUPPORT

ध्रुव वर्ण* currentParallelFramework()
{
    std::shared_ptr<ParallelForAPI>& api = getCurrentParallelForAPI();
    यदि (api)
    {
        प्रत्यागम api->getName();
    }
#यदि_परिभाषितम् CV_PARALLEL_FRAMEWORK
    प्रत्यागम CV_PARALLEL_FRAMEWORK;
#अन्यथा
    प्रत्यागम रिक्त;
#यदि_अन्तः
}

}  // namespace cv::
