// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ಅನ್ತರ್ಭಾವ "precomp.hpp"

#ಅನ್ತರ್ಭಾವ "parallel_impl.hpp"

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_PTHREADS_PF
#ಅನ್ತರ್ಭಾವ <pthread.h>

#ಅನ್ತರ್ಭಾವ <opencv2/core/utils/configuration.private.hpp>

#ಅನ್ತರ್ಭಾವ <opencv2/core/utils/logger.defines.hpp>
//#undef CV_LOG_STRIP_LEVEL
//#define CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_VERBOSE + 1
#ಅನ್ತರ್ಭಾವ <opencv2/core/utils/logger.hpp>

#ಅನ್ತರ್ಭಾವ <opencv2/core/utils/trace.private.hpp>

//#define CV_PROFILE_THREADS 64
//#define getTickCount getCPUTickCount  // use this if getTickCount() calls are expensive (and getCPUTickCount() is accurate)

//#define CV_USE_GLOBAL_WORKERS_COND_VAR  // not effective on many-core systems (10+)

#ಅನ್ತರ್ಭಾವ <atomic>

// Spin lock's OS-level yield
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ DECLARE_CV_YIELD
DECLARE_CV_YIELD
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ CV_YIELD
# include <thread>
# define CV_YIELD() std::this_thread::yield()
#ಆದರೆ_ಅಂತ್ಯ // CV_YIELD

// Spin lock's CPU-level yield (required for Hyper-Threading)
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ DECLARE_CV_PAUSE
DECLARE_CV_PAUSE
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ CV_PAUSE
# ಆದರೆ defined __GNUC__ && (defined __i386__ || defined __x86_64__)
#   include <x86intrin.h> /* for __rdtsc */
#   ಆದರೆ !defined(__SSE2__)
      ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ cv_non_sse_mm_pause() { __asm__ __volatile__ ("rep; nop"); }
#     define _mm_pause cv_non_sse_mm_pause
#   endif
// With Skylake CPUs and above, _mm_pause takes 140 cycles so no need for a loop.
#   define CV_PAUSE(v) ಮಾಡು { (ಶೂನ್ಯ)v; _mm_pause(); } ಆಗ (0)
# elif defined __GNUC__ && defined __aarch64__
#   define CV_PAUSE(v) ಮಾಡು { ಗಾಗಿ (ಪೂರ್ಣಾಂಕ __delay = (v); __delay > 0; --__delay) { asm ಅಸ್ಥಿರ("yield" ::: "memory"); } } ಆಗ (0)
# elif defined __GNUC__ && defined __arm__
#   define CV_PAUSE(v) ಮಾಡು { ಗಾಗಿ (ಪೂರ್ಣಾಂಕ __delay = (v); __delay > 0; --__delay) { asm ಅಸ್ಥಿರ("" ::: "memory"); } } ಆಗ (0)
# elif defined __GNUC__ && defined __mips__ && __mips_isa_rev >= 2
#   define CV_PAUSE(v) ಮಾಡು { ಗಾಗಿ (ಪೂರ್ಣಾಂಕ __delay = (v); __delay > 0; --__delay) { asm ಅಸ್ಥಿರ("pause" ::: "memory"); } } ಆಗ (0)
# elif defined __APPLE__ && defined __POWERPC__
#   define CV_PAUSE(v) ಮಾಡು { ಗಾಗಿ (ಪೂರ್ಣಾಂಕ __delay = (v); __delay > 0; --__delay) { asm ಅಸ್ಥಿರ("or r27,r27,r27" ::: "memory"); } } ಆಗ (0)
# elif defined __GNUC__ && defined __PPC64__
#   define CV_PAUSE(v) ಮಾಡು { ಗಾಗಿ (ಪೂರ್ಣಾಂಕ __delay = (v); __delay > 0; --__delay) { asm ಅಸ್ಥಿರ("or 27,27,27" ::: "memory"); } } ಆಗ (0)
# elif defined __GNUC__ && defined __riscv
// PAUSE HINT is not part of RISC-V ISA yet, but is under discussion now. For details see:
// https://github.com/riscv/riscv-isa-manual/pull/398
// https://github.com/riscv/riscv-isa-manual/issues/43
// #   define CV_PAUSE(v) do { for (int __delay = (v); __delay > 0; --__delay) { asm volatile("pause"); } } while (0)
#   define CV_PAUSE(v) ಮಾಡು { ಗಾಗಿ (ಪೂರ್ಣಾಂಕ __delay = (v); __delay > 0; --__delay) { asm ಅಸ್ಥಿರ("nop"); } } ಆಗ (0)
# elif defined __GNUC__ && defined __loongarch__
#   define CV_PAUSE(v) ಮಾಡು { ಗಾಗಿ (ಪೂರ್ಣಾಂಕ __delay = (v); __delay > 0; --__delay) { asm ಅಸ್ಥಿರ("nop"); } } ಆಗ (0)
# ಇಲ್ಲದಿದ್ದರೆ
#   warning "Can't detect 'pause' (CPU-yield) instruction on the target platform. Specify CV_PAUSE() definition via compiler flags."
#   define CV_PAUSE(...) ಮಾಡು { /* no-op: works, but not effective */ } ಆಗ (0)
# endif
#ಆದರೆ_ಅಂತ್ಯ // CV_PAUSE


ನಾಮಾಕಾಶ cv
{

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ CV_ಸಕ್ರಿಯ_WAIT_PAUSE_ಎಲ್ಲೆ = (ಪೂರ್ಣಾಂಕ)utils::get_Configuration_Parameter_ಗಾತ್ರ_T("OPENCV_THREAD_POOL_ACTIVE_WAIT_PAUSE_LIMIT", 16);  // iterations
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ CV_ಕೆಲಸಗಾರ_ಸಕ್ರಿಯ_WAIT = (ಪೂರ್ಣಾಂಕ)utils::get_Configuration_Parameter_ಗಾತ್ರ_T("OPENCV_THREAD_POOL_ACTIVE_WAIT_WORKER", 2000);  // iterations
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ CV_MAIN_THREAD_ಸಕ್ರಿಯ_WAIT = (ಪೂರ್ಣಾಂಕ)utils::get_Configuration_Parameter_ಗಾತ್ರ_T("OPENCV_THREAD_POOL_ACTIVE_WAIT_MAIN", 10000); // iterations

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ CV_ಕೆಲಸಗಾರ_ಸಕ್ರಿಯ_WAIT_ಎಳೆಗಳು_ಎಲ್ಲೆ = (ಪೂರ್ಣಾಂಕ)utils::get_Configuration_Parameter_ಗಾತ್ರ_T("OPENCV_THREAD_POOL_ACTIVE_WAIT_THREADS_LIMIT", 0); // number of real cores

ವರ್ಗ ಕೆಲಸಗಾರ_Thread;
ವರ್ಗ Parallel_ಕೆಲಸ;

ವರ್ಗ ThreadPool
{
ಸಾರ್ವಜನಿಕ:
    ಸ್ಥಿರ ThreadPool& instance()
    {
        CV_SINGLETON_LAZY_INIT_REF(ThreadPool, ಹೊಸ ThreadPool())
    }

    ಸ್ಥಿರ ಶೂನ್ಯ stop()
    {
        ThreadPool& manager = instance();
        manager.reconfigure(0);
    }

    ಶೂನ್ಯ reconfigure(ಚಿಹ್ನೆರಹಿತ new_ಎಳೆಗಳು_ಎಣಿಕೆ)
    {
        ಆದರೆ (new_ಎಳೆಗಳು_ಎಣಿಕೆ == ಎಳೆಗಳು.size())
            ಹಿಂತಿರುಗಿ;
        pthread_mutex_lock(&mutex);
        reconfigure_(new_ಎಳೆಗಳು_ಎಣಿಕೆ);
        pthread_mutex_unlock(&mutex);
    }
    ತಾರ್ಕಿಕ reconfigure_(ಚಿಹ್ನೆರಹಿತ new_ಎಳೆಗಳು_ಎಣಿಕೆ); // internal implementation

    ಶೂನ್ಯ ಸತತ(ಸ್ಥಿರಾಂಕ Range& range, ಸ್ಥಿರಾಂಕ ParallelLoopBody& body, ದ್ವಿಗುಣ nstripes);

    size_t get_ಅಂಕೆ_ದ_ಎಳೆಗಳು();

    ಶೂನ್ಯ ಗಣ_ಅಂಕೆ_ದ_ಎಳೆಗಳು(ಚಿಹ್ನೆರಹಿತ n);

    ThreadPool();

    ~ThreadPool();

    ಚಿಹ್ನೆರಹಿತ ಅಂಕೆ_ಎಳೆಗಳು_2;

    pthread_mutex_t mutex;  // guards fields (job/threads) from non-worker threads (concurrent parallel_for calls)
#ಆದರೆ defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
    pthread_cond_t cond_thread_wake;
#ಆದರೆ_ಅಂತ್ಯ

    pthread_mutex_t mutex_notify;
    pthread_cond_t cond_thread_task_complete;

    std::vector< Ptr<ಕೆಲಸಗಾರ_Thread> > ಎಳೆಗಳು;

    Ptr<Parallel_ಕೆಲಸ> ಕೆಲಸ;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CV_PROFILE_ಎಳೆಗಳು
    ದ್ವಿಗುಣ tickFreq;
    int64 ಕೆಲಸ_Submit_ಸಮಯ;
    ರಚನೆ ThreadStatistics
    {
        ThreadStatistics() : threadWait(0)
        {
            reset();
        }
        ಶೂನ್ಯ reset()
        {
            threadWake = 0;
            thread_Execute_ಆರಂಭ = 0;
            threadExecuteStop = 0;
            executedTasks = 0;
            keep_ಸಕ್ರಿಯ = ಸುಳ್ಳು;
            threadPing = get_Tick_ಎಣಿಕೆ();
        }
        int64 threadWait; // don't reset by default
        int64 threadPing; // don't reset by default
        int64 threadWake;
        int64 thread_Execute_ಆರಂಭ;
        int64 threadExecuteStop;
        int64 threadFree;
        ಚಿಹ್ನೆರಹಿತ executedTasks;
        ತಾರ್ಕಿಕ keep_ಸಕ್ರಿಯ;

        int64 dummy_[8]; // separate cache lines

        ಶೂನ್ಯ dump(ಪೂರ್ಣಾಂಕ id, int64 ತಳ_ಸಮಯ, ದ್ವಿಗುಣ tickFreq)
        {
            ಆದರೆ (id < 0)
                std::ಹೊರಗೆ << "Main: ";
            ಇಲ್ಲದಿದ್ದರೆ
                ಬರೆ("T%03d: ", id + 2);
            ಬರೆ("wait=% 10.1f   ping=% 6.1f",
                    threadWait > 0 ? (threadWait - ತಳ_ಸಮಯ) / tickFreq * 1e6 : -0.0,
                    threadPing > 0 ? (threadPing - ತಳ_ಸಮಯ) / tickFreq * 1e6 : -0.0);
            ಆದರೆ (threadWake > 0)
                ಬರೆ("   wake=% 6.1f",
                    (threadWake > 0 ? (threadWake - ತಳ_ಸಮಯ) / tickFreq * 1e6 : -0.0));
            ಆದರೆ (thread_Execute_ಆರಂಭ > 0)
            {
                ಬರೆ("   exec=% 6.1f - % 6.1f   tasksDone=%5u   free=% 6.1f",
                    (thread_Execute_ಆರಂಭ > 0 ? (thread_Execute_ಆರಂಭ - ತಳ_ಸಮಯ) / tickFreq * 1e6 : -0.0),
                    (threadExecuteStop > 0 ? (threadExecuteStop - ತಳ_ಸಮಯ) / tickFreq * 1e6 : -0.0),
                    executedTasks,
                    (threadFree > 0 ? (threadFree - ತಳ_ಸಮಯ) / tickFreq * 1e6 : -0.0));
                ಆದರೆ (id >= 0)
                    ಬರೆ(" active=%s\n", keep_ಸಕ್ರಿಯ ? "true" : "false");
                ಇಲ್ಲದಿದ್ದರೆ
                    ಬರೆ("\n");
            }
            ಇಲ್ಲದಿದ್ದರೆ
                ಬರೆ("   ------------------------------------------------------------------------------\n");
        }
    };
    ThreadStatistics ಎಳೆಗಳು_stat[CV_PROFILE_ಎಳೆಗಳು]; // 0 - main thread, 1..N - worker threads
#ಆದರೆ_ಅಂತ್ಯ

};

ವರ್ಗ ಕೆಲಸಗಾರ_Thread
{
ಸಾರ್ವಜನಿಕ:
    ThreadPool& thread_pool;
    ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ id;
    pthread_t posix_thread;
    ತಾರ್ಕಿಕ is_created;

    std::atomic<ತಾರ್ಕಿಕ> stop_thread;

    std::atomic<ತಾರ್ಕಿಕ> has_wake_signal;

    Ptr<Parallel_ಕೆಲಸ> ಕೆಲಸ;

    pthread_mutex_t mutex;
#ಆದರೆ !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
    ಅಸ್ಥಿರ ತಾರ್ಕಿಕ is_ಸಕ್ರಿಯ_2;
    pthread_cond_t cond_thread_wake;
#ಆದರೆ_ಅಂತ್ಯ

    ಕೆಲಸಗಾರ_Thread(ThreadPool& thread_pool_, ಚಿಹ್ನೆರಹಿತ id_) :
        thread_pool(thread_pool_),
        id(id_),
        posix_thread(0),
        is_created(ಸುಳ್ಳು),
        stop_thread(ಸುಳ್ಳು),
        has_wake_signal(ಸುಳ್ಳು)
#ಆದರೆ !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
        , is_ಸಕ್ರಿಯ_2(ನಿಜ)
#ಆದರೆ_ಅಂತ್ಯ
    {
        CV_LOG_VERBOSE(ಖಾಲಿ, 1, "MainThread: initializing new worker: " << id);
        ಪೂರ್ಣಾಂಕ res = pthread_mutex_init(&mutex, ಖಾಲಿ);
        ಆದರೆ (res != 0)
        {
            CV_LOG_ERROR(ಖಾಲಿ, id << ": Can't create thread mutex: res = " << res);
            ಹಿಂತಿರುಗಿ;
        }
#ಆದರೆ !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
        res = pthread_cond_init(&cond_thread_wake, ಖಾಲಿ);
        ಆದರೆ (res != 0)
        {
            CV_LOG_ERROR(ಖಾಲಿ, id << ": Can't create thread condition variable: res = " << res);
            ಹಿಂತಿರುಗಿ;
        }
#ಆದರೆ_ಅಂತ್ಯ
        res = pthread_create(&posix_thread, ಖಾಲಿ, thread_loop_wrapper, (ಶೂನ್ಯ*)ಇದು);
        ಆದರೆ (res != 0)
        {
            CV_LOG_ERROR(ಖಾಲಿ, id << ": Can't spawn new thread: res = " << res);
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            is_created = ನಿಜ;
        }
    }

    ~ಕೆಲಸಗಾರ_Thread()
    {
        CV_LOG_VERBOSE(ಖಾಲಿ, 1, "MainThread: destroy worker thread: " << id);
        ಆದರೆ (is_created)
        {
            ಆದರೆ (!stop_thread)
            {
                pthread_mutex_lock(&mutex);  // to avoid signal miss due pre-check
                stop_thread = ನಿಜ;
                pthread_mutex_unlock(&mutex);
#ಆದರೆ defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
                pthread_cond_broadcast(&thread_pool.cond_thread_wake);
#ಇಲ್ಲದಿದ್ದರೆ
                pthread_cond_signal(&cond_thread_wake);
#ಆದರೆ_ಅಂತ್ಯ
            }
            pthread_join(posix_thread, ಖಾಲಿ);
        }
#ಆದರೆ !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
        pthread_cond_destroy(&cond_thread_wake);
#ಆದರೆ_ಅಂತ್ಯ
        pthread_mutex_destroy(&mutex);
    }

    ಶೂನ್ಯ thread_body();
    ಸ್ಥಿರ ಶೂನ್ಯ* thread_loop_wrapper(ಶೂನ್ಯ* thread_object)
    {
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ OPENCV_WITH_ITT
        __itt_thread_set_name(cv::format("OpenCVThread-%03d", cv::utils::getThreadID()).c_str());
#ಆದರೆ_ಅಂತ್ಯ
        ((ಕೆಲಸಗಾರ_Thread*)thread_object)->thread_body();
        ಹಿಂತಿರುಗಿ 0;
    }
};

ವರ್ಗ Parallel_ಕೆಲಸ
{
ಸಾರ್ವಜನಿಕ:
    Parallel_ಕೆಲಸ(ಸ್ಥಿರಾಂಕ ThreadPool& thread_pool_, ಸ್ಥಿರಾಂಕ Range& range_, ಸ್ಥಿರಾಂಕ ParallelLoopBody& body_, ಪೂರ್ಣಾಂಕ nstripes_) :
        thread_pool(thread_pool_),
        body(body_),
        range(range_),
        nstripes((ಚಿಹ್ನೆರಹಿತ)nstripes_),
        is_completed(ಸುಳ್ಳು)
    {
        CV_LOG_VERBOSE(ಖಾಲಿ, 5, "ParallelJob::ParallelJob(" << (ಶೂನ್ಯ*)ಇದು << ")");
        current_task.store(0, std::memory_order_relaxed);
        ಸಕ್ರಿಯ_thread_ಎಣಿಕೆ.store(0, std::memory_order_relaxed);
        completed_thread_ಎಣಿಕೆ.store(0, std::memory_order_relaxed);
        dummy0_[0] = 0, dummy1_[0] = 0, dummy2_[0] = 0; // compiler warning
    }

    ~Parallel_ಕೆಲಸ()
    {
        CV_LOG_VERBOSE(ಖಾಲಿ, 5, "ParallelJob::~ParallelJob(" << (ಶೂನ್ಯ*)ಇದು << ")");
    }

    ಚಿಹ್ನೆರಹಿತ execute(ತಾರ್ಕಿಕ is_ಕೆಲಸಗಾರ_thread)
    {
        ಚಿಹ್ನೆರಹಿತ executed_tasks = 0;
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ task_ಎಣಿಕೆ = range.size();
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ಉಳಿದ_multiplier = std::min(nstripes,
                std::max(
                        std::min(100u, thread_pool.ಅಂಕೆ_ಎಳೆಗಳು_2 * 4),
                        thread_pool.ಅಂಕೆ_ಎಳೆಗಳು_2 * 2
                ));  // experimental value
        ಗಾಗಿ (;;)
        {
            ಪೂರ್ಣಾಂಕ chunk_ಗಾತ್ರ = std::max(1, (task_ಎಣಿಕೆ - current_task) / ಉಳಿದ_multiplier);
            ಪೂರ್ಣಾಂಕ id = current_task.fetch_add(chunk_ಗಾತ್ರ, std::memory_order_seq_cst);
            ಆದರೆ (id >= task_ಎಣಿಕೆ)
                ನಿಲ್ಲಿಸು; // no more free tasks

            executed_tasks += chunk_ಗಾತ್ರ;
            ಪೂರ್ಣಾಂಕ ಆರಂಭ_id = id;
            ಪೂರ್ಣಾಂಕ ಕೊನೆ_id = std::min(task_ಎಣಿಕೆ, id + chunk_ಗಾತ್ರ);
            CV_LOG_VERBOSE(ಖಾಲಿ, 9, "Thread: job " << ಆರಂಭ_id << "-" << ಕೊನೆ_id);

            //TODO: if (not pending exception)
            {
                body.ಕ್ರಿಯಾಕಾರಕ()(Range(range.ಆರಂಭ + ಆರಂಭ_id, range.ಆರಂಭ + ಕೊನೆ_id));
            }
            ಆದರೆ (is_ಕೆಲಸಗಾರ_thread && is_completed)
            {
                CV_LOG_ERROR(ಖಾಲಿ, "\t\t\t\tBUG! Job: " << (ಶೂನ್ಯ*)ಇದು << " " << id << " " << ಸಕ್ರಿಯ_thread_ಎಣಿಕೆ << " " << completed_thread_ಎಣಿಕೆ);
                CV_Assert(!is_completed); // TODO Dbg this
            }
        }
        ಹಿಂತಿರುಗಿ executed_tasks;
    }

    ಸ್ಥಿರಾಂಕ ThreadPool& thread_pool;
    ಸ್ಥಿರಾಂಕ ParallelLoopBody& body;
    ಸ್ಥಿರಾಂಕ Range range;
    ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ nstripes;

    std::atomic<ಪೂರ್ಣಾಂಕ> current_task;  // next free part of job
    int64 dummy0_[8];  // avoid cache-line reusing for the same atomics

    std::atomic<ಪೂರ್ಣಾಂಕ> ಸಕ್ರಿಯ_thread_ಎಣಿಕೆ;  // number of threads worked on this job
    int64 dummy1_[8];  // avoid cache-line reusing for the same atomics

    std::atomic<ಪೂರ್ಣಾಂಕ> completed_thread_ಎಣಿಕೆ;  // number of threads completed any activities on this job
    int64 dummy2_[8];  // avoid cache-line reusing for the same atomics

    std::atomic<ತಾರ್ಕಿಕ> is_completed;

    // TODO exception handling
};


// Disable thread sanitization check when CV_USE_GLOBAL_WORKERS_COND_VAR is not
// set because it triggers as the main thread reads isActive while the children
// thread writes it (but it all works out because a mutex is locked in the main
// thread and isActive re-checked).
// This is to solve issue #19463.
#ಆದರೆ !defined(CV_USE_GLOBAL_WORKERS_COND_VAR) && defined(__clang__) && defined(__has_feature)
#ಆದರೆ __has_feature(thread_sanitizer)
__attribute__((no_sanitize("thread")))
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ_ಅಂತ್ಯ
ಶೂನ್ಯ ಕೆಲಸಗಾರ_Thread::thread_body()
{
    (ಶೂನ್ಯ)cv::utils::getThreadID(); // notify OpenCV about new thread
    CV_LOG_VERBOSE(ಖಾಲಿ, 5, "Thread: new thread: " << id);

    ತಾರ್ಕಿಕ allow_ಸಕ್ರಿಯ_wait = ನಿಜ;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CV_PROFILE_ಎಳೆಗಳು
    ThreadPool::ThreadStatistics& stat = thread_pool.ಎಳೆಗಳು_stat[id + 1];
#ಆದರೆ_ಅಂತ್ಯ

    ಆಗ (!stop_thread)
    {
        CV_LOG_VERBOSE(ಖಾಲಿ, 5, "Thread: ... loop iteration: allow_active_wait=" << allow_ಸಕ್ರಿಯ_wait << "   has_wake_signal=" << has_wake_signal);
        ಆದರೆ (allow_ಸಕ್ರಿಯ_wait && CV_ಕೆಲಸಗಾರ_ಸಕ್ರಿಯ_WAIT > 0)
        {
            allow_ಸಕ್ರಿಯ_wait = ಸುಳ್ಳು;
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < CV_ಕೆಲಸಗಾರ_ಸಕ್ರಿಯ_WAIT; i++)
            {
                ಆದರೆ (has_wake_signal)
                    ನಿಲ್ಲಿಸು;
                ಆದರೆ (CV_ಸಕ್ರಿಯ_WAIT_PAUSE_ಎಲ್ಲೆ > 0 && (i < CV_ಸಕ್ರಿಯ_WAIT_PAUSE_ಎಲ್ಲೆ || (i & 1)))
                    CV_PAUSE(16);
                ಇಲ್ಲದಿದ್ದರೆ
                    CV_YIELD();
            }
        }
        pthread_mutex_lock(&mutex);
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CV_PROFILE_ಎಳೆಗಳು
        stat.threadWait = get_Tick_ಎಣಿಕೆ();
#ಆದರೆ_ಅಂತ್ಯ
        ಆಗ (!has_wake_signal) // to handle spurious wakeups
        {
            //CV_LOG_VERBOSE(NULL, 5, "Thread: wait (sleep) ...");
#ಆದರೆ defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
            pthread_cond_wait(&thread_pool.cond_thread_wake, &mutex);
#ಇಲ್ಲದಿದ್ದರೆ
            is_ಸಕ್ರಿಯ_2 = ಸುಳ್ಳು;
            pthread_cond_wait(&cond_thread_wake, &mutex);
            is_ಸಕ್ರಿಯ_2 = ನಿಜ;
#ಆದರೆ_ಅಂತ್ಯ
            CV_LOG_VERBOSE(ಖಾಲಿ, 5, "Thread: wake ... (has_wake_signal=" << has_wake_signal << " stop_thread=" << stop_thread << ")")
        }
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CV_PROFILE_ಎಳೆಗಳು
        stat.threadWake = get_Tick_ಎಣಿಕೆ();
#ಆದರೆ_ಅಂತ್ಯ

        CV_LOG_VERBOSE(ಖಾಲಿ, 5, "Thread: checking for new job");
        ಆದರೆ (CV_ಕೆಲಸಗಾರ_ಸಕ್ರಿಯ_WAIT_ಎಳೆಗಳು_ಎಲ್ಲೆ == 0)
            allow_ಸಕ್ರಿಯ_wait = ನಿಜ;
        Ptr<Parallel_ಕೆಲಸ> j_ptr; swap(j_ptr, ಕೆಲಸ);
        has_wake_signal = ಸುಳ್ಳು;
        pthread_mutex_unlock(&mutex);

        ಆದರೆ (!stop_thread)
        {
            Parallel_ಕೆಲಸ* j = j_ptr;
            ಆದರೆ (j)
            {
                CV_LOG_VERBOSE(ಖಾಲಿ, 5, "Thread: job size=" << j->range.size() << " done=" << j->current_task);
                ಆದರೆ (j->current_task < j->range.size())
                {
                    ಪೂರ್ಣಾಂಕ other = j->ಸಕ್ರಿಯ_thread_ಎಣಿಕೆ.fetch_add(1, std::memory_order_seq_cst);
                    CV_LOG_VERBOSE(ಖಾಲಿ, 5, "Thread: processing new job (with " << other << " other threads)"); CV_ಬಳಸದ(other);
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CV_PROFILE_ಎಳೆಗಳು
                    stat.thread_Execute_ಆರಂಭ = get_Tick_ಎಣಿಕೆ();
                    stat.executedTasks = j->execute(ನಿಜ);
                    stat.threadExecuteStop = get_Tick_ಎಣಿಕೆ();
#ಇಲ್ಲದಿದ್ದರೆ
                    j->execute(ನಿಜ);
#ಆದರೆ_ಅಂತ್ಯ
                    ಪೂರ್ಣಾಂಕ completed = j->completed_thread_ಎಣಿಕೆ.fetch_add(1, std::memory_order_seq_cst) + 1;
                    ಪೂರ್ಣಾಂಕ ಸಕ್ರಿಯ = j->ಸಕ್ರಿಯ_thread_ಎಣಿಕೆ.load(std::memory_order_acquire);
                    ಆದರೆ (CV_ಕೆಲಸಗಾರ_ಸಕ್ರಿಯ_WAIT_ಎಳೆಗಳು_ಎಲ್ಲೆ > 0)
                    {
                        allow_ಸಕ್ರಿಯ_wait = ನಿಜ;
                        ಆದರೆ (ಸಕ್ರಿಯ >= CV_ಕೆಲಸಗಾರ_ಸಕ್ರಿಯ_WAIT_ಎಳೆಗಳು_ಎಲ್ಲೆ && (id & 1) == 0) // turn off a half of threads
                            allow_ಸಕ್ರಿಯ_wait = ಸುಳ್ಳು;
                    }
                    CV_LOG_VERBOSE(ಖಾಲಿ, 5, "Thread: completed job processing: " << ಸಕ್ರಿಯ << " " << completed);
                    ಆದರೆ (ಸಕ್ರಿಯ == completed)
                    {
                        ತಾರ್ಕಿಕ need_signal = !j->is_completed;
                        j->is_completed = ನಿಜ;
                        j = ಖಾಲಿ; j_ptr.release();
                        ಆದರೆ (need_signal)
                        {
                            CV_LOG_VERBOSE(ಖಾಲಿ, 5, "Thread: job finished => notifying the main thread");
                            pthread_mutex_lock(&thread_pool.mutex_notify);  // to avoid signal miss due pre-check condition
                            //ಖಾಲಿಗಳು
                            pthread_mutex_unlock(&thread_pool.mutex_notify);
                            pthread_cond_broadcast/*pthread_cond_signal*/(&thread_pool.cond_thread_task_complete);
                        }
                    }
                }
                ಇಲ್ಲದಿದ್ದರೆ
                {
                    CV_LOG_VERBOSE(ಖಾಲಿ, 5, "Thread: no free job tasks");
                }
            }
        }
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CV_PROFILE_ಎಳೆಗಳು
        stat.threadFree = get_Tick_ಎಣಿಕೆ();
        stat.keep_ಸಕ್ರಿಯ = allow_ಸಕ್ರಿಯ_wait;
#ಆದರೆ_ಅಂತ್ಯ
    }
}

ThreadPool::ThreadPool()
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CV_PROFILE_ಎಳೆಗಳು
    tickFreq = getTickFrequency();
#ಆದರೆ_ಅಂತ್ಯ

    ಪೂರ್ಣಾಂಕ res = 0;
    res |= pthread_mutex_init(&mutex, ಖಾಲಿ);
    res |= pthread_mutex_init(&mutex_notify, ಖಾಲಿ);
#ಆದರೆ defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
    res |= pthread_cond_init(&cond_thread_wake, ಖಾಲಿ);
#ಆದರೆ_ಅಂತ್ಯ
    res |= pthread_cond_init(&cond_thread_task_complete, ಖಾಲಿ);

    ಆದರೆ (0 != res)
    {
        CV_LOG_FATAL(ಖಾಲಿ, "Failed to initialize ThreadPool (pthreads)");
    }
    ಅಂಕೆ_ಎಳೆಗಳು_2 = default_ಸಂಖ್ಯೆ_ದ_ಎಳೆಗಳು();
}

ತಾರ್ಕಿಕ ThreadPool::reconfigure_(ಚಿಹ್ನೆರಹಿತ new_ಎಳೆಗಳು_ಎಣಿಕೆ)
{
    ಆದರೆ (new_ಎಳೆಗಳು_ಎಣಿಕೆ == ಎಳೆಗಳು.size())
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

    ಆದರೆ (new_ಎಳೆಗಳು_ಎಣಿಕೆ < ಎಳೆಗಳು.size())
    {
        CV_LOG_VERBOSE(ಖಾಲಿ, 1, "MainThread: reduce worker pool: " << ಎಳೆಗಳು.size() << " => " << new_ಎಳೆಗಳು_ಎಣಿಕೆ);
        std::vector< Ptr<ಕೆಲಸಗಾರ_Thread> > release_ಎಳೆಗಳು(ಎಳೆಗಳು.size() - new_ಎಳೆಗಳು_ಎಣಿಕೆ);
        ಗಾಗಿ (size_t i = new_ಎಳೆಗಳು_ಎಣಿಕೆ; i < ಎಳೆಗಳು.size(); ++i)
        {
            pthread_mutex_lock(&ಎಳೆಗಳು[i]->mutex);  // to avoid signal miss due pre-check
            ಎಳೆಗಳು[i]->stop_thread = ನಿಜ;
            ಎಳೆಗಳು[i]->has_wake_signal = ನಿಜ;
#ಆದರೆ !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
            pthread_mutex_unlock(&ಎಳೆಗಳು[i]->mutex);
            pthread_cond_broadcast/*pthread_cond_signal*/(&ಎಳೆಗಳು[i]->cond_thread_wake); // wake thread
#ಇಲ್ಲದಿದ್ದರೆ
            pthread_mutex_unlock(&ಎಳೆಗಳು[i]->mutex);
#ಆದರೆ_ಅಂತ್ಯ
            std::swap(ಎಳೆಗಳು[i], release_ಎಳೆಗಳು[i - new_ಎಳೆಗಳು_ಎಣಿಕೆ]);
        }
#ಆದರೆ defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
        CV_LOG_VERBOSE(ಖಾಲಿ, 1, "MainThread: notify worker threads about termination...");
        pthread_cond_broadcast(&cond_thread_wake); // wake all threads
#ಆದರೆ_ಅಂತ್ಯ
        ಎಳೆಗಳು.resize(new_ಎಳೆಗಳು_ಎಣಿಕೆ);
        release_ಎಳೆಗಳು.clear();  // calls thread_join which want to lock mutex
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        CV_LOG_VERBOSE(ಖಾಲಿ, 1, "MainThread: upgrade worker pool: " << ಎಳೆಗಳು.size() << " => " << new_ಎಳೆಗಳು_ಎಣಿಕೆ);
        ಗಾಗಿ (size_t i = ಎಳೆಗಳು.size(); i < new_ಎಳೆಗಳು_ಎಣಿಕೆ; ++i)
        {
            ಎಳೆಗಳು.push_back(Ptr<ಕೆಲಸಗಾರ_Thread>(ಹೊಸ ಕೆಲಸಗಾರ_Thread(*ಇದು, (ಚಿಹ್ನೆರಹಿತ)i))); // spawn more threads
        }
    }
    ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
}

ThreadPool::~ThreadPool()
{
    reconfigure(0);
    pthread_cond_destroy(&cond_thread_task_complete);
#ಆದರೆ defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
    pthread_cond_destroy(&cond_thread_wake);
#ಆದರೆ_ಅಂತ್ಯ
    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&mutex_notify);
}

ಶೂನ್ಯ ThreadPool::ಸತತ(ಸ್ಥಿರಾಂಕ Range& range, ಸ್ಥಿರಾಂಕ ParallelLoopBody& body, ದ್ವಿಗುಣ nstripes)
{
    CV_LOG_VERBOSE(ಖಾಲಿ, 1, "MainThread: new parallel job: num_threads=" << ಅಂಕೆ_ಎಳೆಗಳು_2 << "   range=" << range.size() << "   nstripes=" << nstripes << "   job=" << (ಶೂನ್ಯ*)ಕೆಲಸ);
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CV_PROFILE_ಎಳೆಗಳು
    ಕೆಲಸ_Submit_ಸಮಯ = get_Tick_ಎಣಿಕೆ();
    ಎಳೆಗಳು_stat[0].reset();
    ಎಳೆಗಳು_stat[0].threadWait = ಕೆಲಸ_Submit_ಸಮಯ;
    ಎಳೆಗಳು_stat[0].threadWake = ಕೆಲಸ_Submit_ಸಮಯ;
#ಆದರೆ_ಅಂತ್ಯ
    ಆದರೆ (get_ಅಂಕೆ_ದ_ಎಳೆಗಳು() > 1 &&
        ಕೆಲಸ == ಖಾಲಿ &&
        (range.size() * nstripes >= 2 || (range.size() > 1 && nstripes <= 0))
    )
    {
        pthread_mutex_lock(&mutex);
        ಆದರೆ (ಕೆಲಸ != ಖಾಲಿ)
        {
            pthread_mutex_unlock(&mutex);
            body(range);
            ಹಿಂತಿರುಗಿ;
        }
        reconfigure_(ಅಂಕೆ_ಎಳೆಗಳು_2 - 1);

        {
            CV_LOG_VERBOSE(ಖಾಲಿ, 1, "MainThread: initialize parallel job: " << range.size());
            ಕೆಲಸ = Ptr<Parallel_ಕೆಲಸ>(ಹೊಸ Parallel_ಕೆಲಸ(*ಇದು, range, body, nstripes));
            pthread_mutex_unlock(&mutex);

            CV_LOG_VERBOSE(ಖಾಲಿ, 5, "MainThread: wake worker threads...");
            size_t ಅಂಕೆ_ಎಳೆಗಳು_ಗಮ್ಯ_wake = std::min(static_cast<size_t>(range.size()), ಎಳೆಗಳು.size());
            ಗಾಗಿ (size_t i = 0; i < ಅಂಕೆ_ಎಳೆಗಳು_ಗಮ್ಯ_wake; ++i)
            {
                ಆದರೆ (ಕೆಲಸ->current_task >= ಕೆಲಸ->range.size())
                    ನಿಲ್ಲಿಸು;
                ಕೆಲಸಗಾರ_Thread& thread = *(ಎಳೆಗಳು[i].get());
                ಆದರೆ (
#ಆದರೆ defined(__clang__) && defined(__has_feature)
#ಆದರೆ __has_feature(thread_sanitizer)
                        1 ||  // Robust workaround to avoid data race warning of `thread.job`
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
                        thread.is_ಸಕ್ರಿಯ_2 ||
#ಆದರೆ_ಅಂತ್ಯ
                        thread.has_wake_signal
                        || !thread.ಕೆಲಸ.empty()  // #10881
                )
                {
                    pthread_mutex_lock(&thread.mutex);
                    thread.ಕೆಲಸ = ಕೆಲಸ;
#ಆದರೆ !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
                    ತಾರ್ಕಿಕ is_ಸಕ್ರಿಯ_2 = thread.is_ಸಕ್ರಿಯ_2;
#ಆದರೆ_ಅಂತ್ಯ
                    thread.has_wake_signal = ನಿಜ;
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CV_PROFILE_ಎಳೆಗಳು
                    ಎಳೆಗಳು_stat[i + 1].reset();
#ಆದರೆ_ಅಂತ್ಯ
                    pthread_mutex_unlock(&thread.mutex);
#ಆದರೆ !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
                    ಆದರೆ (!is_ಸಕ್ರಿಯ_2)
                    {
                        pthread_cond_broadcast/*pthread_cond_signal*/(&thread.cond_thread_wake); // wake thread
                    }
#ಆದರೆ_ಅಂತ್ಯ
                }
                ಇಲ್ಲದಿದ್ದರೆ
                {
                    CV_Assert(thread.ಕೆಲಸ.empty());
                    thread.ಕೆಲಸ = ಕೆಲಸ;
                    thread.has_wake_signal = ನಿಜ;
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CV_PROFILE_ಎಳೆಗಳು
                    ಎಳೆಗಳು_stat[i + 1].reset();
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
                    pthread_cond_broadcast/*pthread_cond_signal*/(&thread.cond_thread_wake); // wake thread
#ಆದರೆ_ಅಂತ್ಯ
                }
            }
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CV_PROFILE_ಎಳೆಗಳು
            ಎಳೆಗಳು_stat[0].threadPing = get_Tick_ಎಣಿಕೆ();
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
            pthread_cond_broadcast(&cond_thread_wake); // wake all threads
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CV_PROFILE_ಎಳೆಗಳು
            ಎಳೆಗಳು_stat[0].threadWake = get_Tick_ಎಣಿಕೆ();
#ಆದರೆ_ಅಂತ್ಯ
            CV_LOG_VERBOSE(ಖಾಲಿ, 5, "MainThread: wake worker threads... (done)");

            {
                Parallel_ಕೆಲಸ& j = *(ಇದು->ಕೆಲಸ);
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CV_PROFILE_ಎಳೆಗಳು
                ಎಳೆಗಳು_stat[0].thread_Execute_ಆರಂಭ = get_Tick_ಎಣಿಕೆ();
                ಎಳೆಗಳು_stat[0].executedTasks = j.execute(ಸುಳ್ಳು);
                ಎಳೆಗಳು_stat[0].threadExecuteStop = get_Tick_ಎಣಿಕೆ();
#ಇಲ್ಲದಿದ್ದರೆ
                j.execute(ಸುಳ್ಳು);
#ಆದರೆ_ಅಂತ್ಯ
                CV_Assert(j.current_task >= j.range.size());
                CV_LOG_VERBOSE(ಖಾಲಿ, 5, "MainThread: complete self-tasks: " << j.ಸಕ್ರಿಯ_thread_ಎಣಿಕೆ << " " << j.completed_thread_ಎಣಿಕೆ);
                ಆದರೆ (ಕೆಲಸ->is_completed || j.ಸಕ್ರಿಯ_thread_ಎಣಿಕೆ == 0)
                {
                    ಕೆಲಸ->is_completed = ನಿಜ;
                    CV_LOG_VERBOSE(ಖಾಲಿ, 5, "MainThread: no WIP worker threads");
                }
                ಇಲ್ಲದಿದ್ದರೆ
                {
                    ಆದರೆ (CV_MAIN_THREAD_ಸಕ್ರಿಯ_WAIT > 0)
                    {
                        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < CV_MAIN_THREAD_ಸಕ್ರಿಯ_WAIT; i++)  // don't spin too much in any case (inaccurate getTickCount())
                        {
                            ಆದರೆ (ಕೆಲಸ->is_completed)
                            {
                                CV_LOG_VERBOSE(ಖಾಲಿ, 5, "MainThread: job finalize (active wait) " << j.ಸಕ್ರಿಯ_thread_ಎಣಿಕೆ << " " << j.completed_thread_ಎಣಿಕೆ);
                                ನಿಲ್ಲಿಸು;
                            }
                            ಆದರೆ (CV_ಸಕ್ರಿಯ_WAIT_PAUSE_ಎಲ್ಲೆ > 0 && (i < CV_ಸಕ್ರಿಯ_WAIT_PAUSE_ಎಲ್ಲೆ || (i & 1)))
                                CV_PAUSE(16);
                            ಇಲ್ಲದಿದ್ದರೆ
                                CV_YIELD();
                        }
                    }
                    ಆದರೆ (!ಕೆಲಸ->is_completed)
                    {
                        CV_LOG_VERBOSE(ಖಾಲಿ, 5, "MainThread: prepare wait " << j.ಸಕ್ರಿಯ_thread_ಎಣಿಕೆ << " " << j.completed_thread_ಎಣಿಕೆ);
                        pthread_mutex_lock(&mutex_notify);
                        ಗಾಗಿ (;;)
                        {
                            ಆದರೆ (ಕೆಲಸ->is_completed)
                            {
                                CV_LOG_VERBOSE(ಖಾಲಿ, 5, "MainThread: job finalize (wait) " << j.ಸಕ್ರಿಯ_thread_ಎಣಿಕೆ << " " << j.completed_thread_ಎಣಿಕೆ);
                                ನಿಲ್ಲಿಸು;
                            }
                            CV_LOG_VERBOSE(ಖಾಲಿ, 5, "MainThread: wait completion (sleep) ...");
                            pthread_cond_wait(&cond_thread_task_complete, &mutex_notify);
                            CV_LOG_VERBOSE(ಖಾಲಿ, 5, "MainThread: wake");
                        }
                        pthread_mutex_unlock(&mutex_notify);
                    }
                }
            }
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CV_PROFILE_ಎಳೆಗಳು
            ಎಳೆಗಳು_stat[0].threadFree = get_Tick_ಎಣಿಕೆ();
            std::ಹೊರಗೆ << "Job: sz=" << range.size() << " nstripes=" << nstripes << "    Time: " << (ಎಳೆಗಳು_stat[0].threadFree - ಕೆಲಸ_Submit_ಸಮಯ) / tickFreq * 1e6 << " usec" << std::ಸಾಲು_ಅಂತ್ಯ;
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < (ಪೂರ್ಣಾಂಕ)ಎಳೆಗಳು.size() + 1; i++)
            {
                ಎಳೆಗಳು_stat[i].dump(i - 1, ಕೆಲಸ_Submit_ಸಮಯ, tickFreq);
            }
#ಆದರೆ_ಅಂತ್ಯ
            ಆದರೆ (ಕೆಲಸ)
            {
                pthread_mutex_lock(&mutex);
                CV_LOG_VERBOSE(ಖಾಲಿ, 5, "MainThread: job release");
                CV_Assert(ಕೆಲಸ->is_completed);
                ಕೆಲಸ.release();
                pthread_mutex_unlock(&mutex);
            }
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        body(range);
    }
}

size_t ThreadPool::get_ಅಂಕೆ_ದ_ಎಳೆಗಳು()
{
    ಹಿಂತಿರುಗಿ ಅಂಕೆ_ಎಳೆಗಳು_2;
}

ಶೂನ್ಯ ThreadPool::ಗಣ_ಅಂಕೆ_ದ_ಎಳೆಗಳು(ಚಿಹ್ನೆರಹಿತ n)
{
    ಆದರೆ (n != ಅಂಕೆ_ಎಳೆಗಳು_2)
    {
        ಅಂಕೆ_ಎಳೆಗಳು_2 = n;
        ಆದರೆ (n == 1)
           ಆದರೆ (ಕೆಲಸ == ಖಾಲಿ) reconfigure(0);  // stop worker threads immediately
    }
}

size_t parallel_pthreads_get_ಎಳೆಗಳು_ಅಂಕೆ()
{
    ಹಿಂತಿರುಗಿ ThreadPool::instance().get_ಅಂಕೆ_ದ_ಎಳೆಗಳು();
}

ಶೂನ್ಯ parallel_pthreads_ಗಣ_ಎಳೆಗಳು_ಅಂಕೆ(ಪೂರ್ಣಾಂಕ num)
{
    ಆದರೆ(num < 0)
    {
        ThreadPool::instance().ಗಣ_ಅಂಕೆ_ದ_ಎಳೆಗಳು(0);
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        ThreadPool::instance().ಗಣ_ಅಂಕೆ_ದ_ಎಳೆಗಳು(ಚಿಹ್ನೆರಹಿತ(num));
    }
}

ಶೂನ್ಯ parallel_for_pthreads(ಸ್ಥಿರಾಂಕ Range& range, ಸ್ಥಿರಾಂಕ ParallelLoopBody& body, ದ್ವಿಗುಣ nstripes)
{
    ThreadPool::instance().ಸತತ(range, body, nstripes);
}

}

#ಆದರೆ_ಅಂತ್ಯ
