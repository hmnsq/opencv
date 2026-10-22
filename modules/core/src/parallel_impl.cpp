// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ಅನ್ತರ್ಭಾವ "precomp.hpp"

#ಅನ್ತರ್ಭಾವ "parallel_impl.hpp"

#ifdef HAVE_PTHREADS_PF
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
#ifdef DECLARE_CV_YIELD
DECLARE_CV_YIELD
#endif
#ifndef CV_YIELD
# include <thread>
# define CV_YIELD() std::this_thread::yield()
#endif // CV_YIELD

// Spin lock's CPU-level yield (required for Hyper-Threading)
#ifdef DECLARE_CV_PAUSE
DECLARE_CV_PAUSE
#endif
#ifndef CV_PAUSE
# ಯದಿ defined __GNUC__ && (defined __i386__ || defined __x86_64__)
#   include <x86intrin.h> /* for __rdtsc */
#   ಯದಿ !defined(__SSE2__)
      ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ cv_non_sse_mm_pause() { __asm__ __volatile__ ("rep; nop"); }
#     define _mm_pause cv_non_sse_mm_pause
#   endif
// With Skylake CPUs and above, _mm_pause takes 140 cycles so no need for a loop.
#   define CV_PAUSE(v) ಕುರು { (ಶೂನ್ಯ)v; _mm_pause(); } ಯಾವತ್ (0)
# elif defined __GNUC__ && defined __aarch64__
#   define CV_PAUSE(v) ಕುರು { ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ __delay = (v); __delay > 0; --__delay) { asm ಚಞ್ಚಲ("yield" ::: "memory"); } } ಯಾವತ್ (0)
# elif defined __GNUC__ && defined __arm__
#   define CV_PAUSE(v) ಕುರು { ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ __delay = (v); __delay > 0; --__delay) { asm ಚಞ್ಚಲ("" ::: "memory"); } } ಯಾವತ್ (0)
# elif defined __GNUC__ && defined __mips__ && __mips_isa_rev >= 2
#   define CV_PAUSE(v) ಕುರು { ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ __delay = (v); __delay > 0; --__delay) { asm ಚಞ್ಚಲ("pause" ::: "memory"); } } ಯಾವತ್ (0)
# elif defined __APPLE__ && defined __POWERPC__
#   define CV_PAUSE(v) ಕುರು { ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ __delay = (v); __delay > 0; --__delay) { asm ಚಞ್ಚಲ("or r27,r27,r27" ::: "memory"); } } ಯಾವತ್ (0)
# elif defined __GNUC__ && defined __PPC64__
#   define CV_PAUSE(v) ಕುರು { ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ __delay = (v); __delay > 0; --__delay) { asm ಚಞ್ಚಲ("or 27,27,27" ::: "memory"); } } ಯಾವತ್ (0)
# elif defined __GNUC__ && defined __riscv
// PAUSE HINT is not part of RISC-V ISA yet, but is under discussion now. For details see:
// https://github.com/riscv/riscv-isa-manual/pull/398
// https://github.com/riscv/riscv-isa-manual/issues/43
// #   define CV_PAUSE(v) do { for (int __delay = (v); __delay > 0; --__delay) { asm volatile("pause"); } } while (0)
#   define CV_PAUSE(v) ಕುರು { ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ __delay = (v); __delay > 0; --__delay) { asm ಚಞ್ಚಲ("nop"); } } ಯಾವತ್ (0)
# elif defined __GNUC__ && defined __loongarch__
#   define CV_PAUSE(v) ಕುರು { ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ __delay = (v); __delay > 0; --__delay) { asm ಚಞ್ಚಲ("nop"); } } ಯಾವತ್ (0)
# ಅನ್ಯಥಾ
#   warning "Can't detect 'pause' (CPU-yield) instruction on the target platform. Specify CV_PAUSE() definition via compiler flags."
#   define CV_PAUSE(...) ಕುರು { /* no-op: works, but not effective */ } ಯಾವತ್ (0)
# endif
#endif // CV_PAUSE


ನಾಮಾಕಾಶ cv
{

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ CV_ACTIVE_WAIT_PAUSE_LIMIT = (ಪೂರ್ಣಾಙ್ಕ)utils::getConfigurationParameterSizeT("OPENCV_THREAD_POOL_ACTIVE_WAIT_PAUSE_LIMIT", 16);  // iterations
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ CV_WORKER_ACTIVE_WAIT = (ಪೂರ್ಣಾಙ್ಕ)utils::getConfigurationParameterSizeT("OPENCV_THREAD_POOL_ACTIVE_WAIT_WORKER", 2000);  // iterations
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ CV_MAIN_THREAD_ACTIVE_WAIT = (ಪೂರ್ಣಾಙ್ಕ)utils::getConfigurationParameterSizeT("OPENCV_THREAD_POOL_ACTIVE_WAIT_MAIN", 10000); // iterations

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ CV_WORKER_ACTIVE_WAIT_THREADS_LIMIT = (ಪೂರ್ಣಾಙ್ಕ)utils::getConfigurationParameterSizeT("OPENCV_THREAD_POOL_ACTIVE_WAIT_THREADS_LIMIT", 0); // number of real cores

ವರ್ಗ WorkerThread;
ವರ್ಗ ParallelJob;

ವರ್ಗ ThreadPool
{
ಸಾರ್ವಜನಿಕ:
    ಸ್ಥಿರ ThreadPool& instance()
    {
        CV_SINGLETON_LAZY_INIT_REF(ThreadPool, ನೂತನ ThreadPool())
    }

    ಸ್ಥಿರ ಶೂನ್ಯ stop()
    {
        ThreadPool& manager = instance();
        manager.reconfigure(0);
    }

    ಶೂನ್ಯ reconfigure(unsigned new_threads_ಎಣಿಕೆ)
    {
        ಯದಿ (new_threads_ಎಣಿಕೆ == threads.size())
            ಪ್ರತ್ಯಾಗಮ;
        pthread_mutex_lock(&mutex);
        reconfigure_(new_threads_ಎಣಿಕೆ);
        pthread_mutex_unlock(&mutex);
    }
    ತಾರ್ಕಿಕ reconfigure_(unsigned new_threads_ಎಣಿಕೆ); // internal implementation

    ಶೂನ್ಯ run(ಧ್ರುವ Range& range, ಧ್ರುವ ParallelLoopBody& body, ದ್ವಿಗುಣ nstripes);

    size_t get_ಅಂಕೆ_Of_Threads();

    ಶೂನ್ಯ set_ಅಂಕೆ_Of_Threads(unsigned n);

    ThreadPool();

    ~ThreadPool();

    unsigned ಅಂಕೆ_threads;

    pthread_mutex_t mutex;  // guards fields (job/threads) from non-worker threads (concurrent parallel_for calls)
#ಯದಿ defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
    pthread_cond_t cond_thread_wake;
#endif

    pthread_mutex_t mutex_notify;
    pthread_cond_t cond_thread_task_complete;

    std::vector< Ptr<WorkerThread> > threads;

    Ptr<ParallelJob> job;

#ifdef CV_PROFILE_THREADS
    ದ್ವಿಗುಣ tickFreq;
    int64 jobSubmitTime;
    ಸಂರಚನಾ ThreadStatistics
    {
        ThreadStatistics() : threadWait(0)
        {
            reset();
        }
        ಶೂನ್ಯ reset()
        {
            threadWake = 0;
            threadExecuteStart = 0;
            threadExecuteStop = 0;
            executedTasks = 0;
            keepActive = ಅಸತ್ಯ;
            threadPing = get_Tick_ಎಣಿಕೆ();
        }
        int64 threadWait; // don't reset by default
        int64 threadPing; // don't reset by default
        int64 threadWake;
        int64 threadExecuteStart;
        int64 threadExecuteStop;
        int64 threadFree;
        unsigned executedTasks;
        ತಾರ್ಕಿಕ keepActive;

        int64 dummy_[8]; // separate cache lines

        ಶೂನ್ಯ dump(ಪೂರ್ಣಾಙ್ಕ id, int64 baseTime, ದ್ವಿಗುಣ tickFreq)
        {
            ಯದಿ (id < 0)
                std::cout << "Main: ";
            ಅನ್ಯಥಾ
                printf("T%03d: ", id + 2);
            printf("wait=% 10.1f   ping=% 6.1f",
                    threadWait > 0 ? (threadWait - baseTime) / tickFreq * 1e6 : -0.0,
                    threadPing > 0 ? (threadPing - baseTime) / tickFreq * 1e6 : -0.0);
            ಯದಿ (threadWake > 0)
                printf("   wake=% 6.1f",
                    (threadWake > 0 ? (threadWake - baseTime) / tickFreq * 1e6 : -0.0));
            ಯದಿ (threadExecuteStart > 0)
            {
                printf("   exec=% 6.1f - % 6.1f   tasksDone=%5u   free=% 6.1f",
                    (threadExecuteStart > 0 ? (threadExecuteStart - baseTime) / tickFreq * 1e6 : -0.0),
                    (threadExecuteStop > 0 ? (threadExecuteStop - baseTime) / tickFreq * 1e6 : -0.0),
                    executedTasks,
                    (threadFree > 0 ? (threadFree - baseTime) / tickFreq * 1e6 : -0.0));
                ಯದಿ (id >= 0)
                    printf(" active=%s\n", keepActive ? "true" : "false");
                ಅನ್ಯಥಾ
                    printf("\n");
            }
            ಅನ್ಯಥಾ
                printf("   ------------------------------------------------------------------------------\n");
        }
    };
    ThreadStatistics threads_stat[CV_PROFILE_THREADS]; // 0 - main thread, 1..N - worker threads
#endif

};

ವರ್ಗ WorkerThread
{
ಸಾರ್ವಜನಿಕ:
    ThreadPool& thread_pool;
    ಧ್ರುವ unsigned id;
    pthread_t posix_thread;
    ತಾರ್ಕಿಕ is_created;

    std::atomic<ತಾರ್ಕಿಕ> stop_thread;

    std::atomic<ತಾರ್ಕಿಕ> has_wake_signal;

    Ptr<ParallelJob> job;

    pthread_mutex_t mutex;
#ಯದಿ !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
    ಚಞ್ಚಲ ತಾರ್ಕಿಕ isActive;
    pthread_cond_t cond_thread_wake;
#endif

    WorkerThread(ThreadPool& thread_pool_, unsigned id_) :
        thread_pool(thread_pool_),
        id(id_),
        posix_thread(0),
        is_created(ಅಸತ್ಯ),
        stop_thread(ಅಸತ್ಯ),
        has_wake_signal(ಅಸತ್ಯ)
#ಯದಿ !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
        , isActive(ಸತ್ಯ)
#endif
    {
        CV_LOG_VERBOSE(ರಿಕ್ತ, 1, "MainThread: initializing new worker: " << id);
        ಪೂರ್ಣಾಙ್ಕ res = pthread_mutex_init(&mutex, ರಿಕ್ತ);
        ಯದಿ (res != 0)
        {
            CV_LOG_ERROR(ರಿಕ್ತ, id << ": Can't create thread mutex: res = " << res);
            ಪ್ರತ್ಯಾಗಮ;
        }
#ಯದಿ !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
        res = pthread_cond_init(&cond_thread_wake, ರಿಕ್ತ);
        ಯದಿ (res != 0)
        {
            CV_LOG_ERROR(ರಿಕ್ತ, id << ": Can't create thread condition variable: res = " << res);
            ಪ್ರತ್ಯಾಗಮ;
        }
#endif
        res = pthread_create(&posix_thread, ರಿಕ್ತ, thread_loop_wrapper, (ಶೂನ್ಯ*)ಏತತ್);
        ಯದಿ (res != 0)
        {
            CV_LOG_ERROR(ರಿಕ್ತ, id << ": Can't spawn new thread: res = " << res);
        }
        ಅನ್ಯಥಾ
        {
            is_created = ಸತ್ಯ;
        }
    }

    ~WorkerThread()
    {
        CV_LOG_VERBOSE(ರಿಕ್ತ, 1, "MainThread: destroy worker thread: " << id);
        ಯದಿ (is_created)
        {
            ಯದಿ (!stop_thread)
            {
                pthread_mutex_lock(&mutex);  // to avoid signal miss due pre-check
                stop_thread = ಸತ್ಯ;
                pthread_mutex_unlock(&mutex);
#ಯದಿ defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
                pthread_cond_broadcast(&thread_pool.cond_thread_wake);
#ಅನ್ಯಥಾ
                pthread_cond_signal(&cond_thread_wake);
#endif
            }
            pthread_join(posix_thread, ರಿಕ್ತ);
        }
#ಯದಿ !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
        pthread_cond_destroy(&cond_thread_wake);
#endif
        pthread_mutex_destroy(&mutex);
    }

    ಶೂನ್ಯ thread_body();
    ಸ್ಥಿರ ಶೂನ್ಯ* thread_loop_wrapper(ಶೂನ್ಯ* thread_object)
    {
#ifdef OPENCV_WITH_ITT
        __itt_thread_set_name(cv::format("OpenCVThread-%03d", cv::utils::getThreadID()).c_str());
#endif
        ((WorkerThread*)thread_object)->thread_body();
        ಪ್ರತ್ಯಾಗಮ 0;
    }
};

ವರ್ಗ ParallelJob
{
ಸಾರ್ವಜನಿಕ:
    ParallelJob(ಧ್ರುವ ThreadPool& thread_pool_, ಧ್ರುವ Range& range_, ಧ್ರುವ ParallelLoopBody& body_, ಪೂರ್ಣಾಙ್ಕ nstripes_) :
        thread_pool(thread_pool_),
        body(body_),
        range(range_),
        nstripes((unsigned)nstripes_),
        is_completed(ಅಸತ್ಯ)
    {
        CV_LOG_VERBOSE(ರಿಕ್ತ, 5, "ParallelJob::ParallelJob(" << (ಶೂನ್ಯ*)ಏತತ್ << ")");
        current_task.store(0, std::memory_order_relaxed);
        active_thread_ಎಣಿಕೆ.store(0, std::memory_order_relaxed);
        completed_thread_ಎಣಿಕೆ.store(0, std::memory_order_relaxed);
        dummy0_[0] = 0, dummy1_[0] = 0, dummy2_[0] = 0; // compiler warning
    }

    ~ParallelJob()
    {
        CV_LOG_VERBOSE(ರಿಕ್ತ, 5, "ParallelJob::~ParallelJob(" << (ಶೂನ್ಯ*)ಏತತ್ << ")");
    }

    unsigned execute(ತಾರ್ಕಿಕ is_worker_thread)
    {
        unsigned executed_tasks = 0;
        ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ task_ಎಣಿಕೆ = range.size();
        ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ remaining_multiplier = std::min(nstripes,
                std::max(
                        std::min(100u, thread_pool.ಅಂಕೆ_threads * 4),
                        thread_pool.ಅಂಕೆ_threads * 2
                ));  // experimental value
        ಪ್ರತಿ (;;)
        {
            ಪೂರ್ಣಾಙ್ಕ chunk_size = std::max(1, (task_ಎಣಿಕೆ - current_task) / remaining_multiplier);
            ಪೂರ್ಣಾಙ್ಕ id = current_task.fetch_add(chunk_size, std::memory_order_seq_cst);
            ಯದಿ (id >= task_ಎಣಿಕೆ)
                ಭಙ್ಗ; // no more free tasks

            executed_tasks += chunk_size;
            ಪೂರ್ಣಾಙ್ಕ start_id = id;
            ಪೂರ್ಣಾಙ್ಕ end_id = std::min(task_ಎಣಿಕೆ, id + chunk_size);
            CV_LOG_VERBOSE(ರಿಕ್ತ, 9, "Thread: job " << start_id << "-" << end_id);

            //TODO: if (not pending exception)
            {
                body.operator()(Range(range.start + start_id, range.start + end_id));
            }
            ಯದಿ (is_worker_thread && is_completed)
            {
                CV_LOG_ERROR(ರಿಕ್ತ, "\t\t\t\tBUG! Job: " << (ಶೂನ್ಯ*)ಏತತ್ << " " << id << " " << active_thread_ಎಣಿಕೆ << " " << completed_thread_ಎಣಿಕೆ);
                CV_Assert(!is_completed); // TODO Dbg this
            }
        }
        ಪ್ರತ್ಯಾಗಮ executed_tasks;
    }

    ಧ್ರುವ ThreadPool& thread_pool;
    ಧ್ರುವ ParallelLoopBody& body;
    ಧ್ರುವ Range range;
    ಧ್ರುವ unsigned nstripes;

    std::atomic<ಪೂರ್ಣಾಙ್ಕ> current_task;  // next free part of job
    int64 dummy0_[8];  // avoid cache-line reusing for the same atomics

    std::atomic<ಪೂರ್ಣಾಙ್ಕ> active_thread_ಎಣಿಕೆ;  // number of threads worked on this job
    int64 dummy1_[8];  // avoid cache-line reusing for the same atomics

    std::atomic<ಪೂರ್ಣಾಙ್ಕ> completed_thread_ಎಣಿಕೆ;  // number of threads completed any activities on this job
    int64 dummy2_[8];  // avoid cache-line reusing for the same atomics

    std::atomic<ತಾರ್ಕಿಕ> is_completed;

    // TODO exception handling
};


// Disable thread sanitization check when CV_USE_GLOBAL_WORKERS_COND_VAR is not
// set because it triggers as the main thread reads isActive while the children
// thread writes it (but it all works out because a mutex is locked in the main
// thread and isActive re-checked).
// This is to solve issue #19463.
#ಯದಿ !defined(CV_USE_GLOBAL_WORKERS_COND_VAR) && defined(__clang__) && defined(__has_feature)
#ಯದಿ __has_feature(thread_sanitizer)
__attribute__((no_sanitize("thread")))
#endif
#endif
ಶೂನ್ಯ WorkerThread::thread_body()
{
    (ಶೂನ್ಯ)cv::utils::getThreadID(); // notify OpenCV about new thread
    CV_LOG_VERBOSE(ರಿಕ್ತ, 5, "Thread: new thread: " << id);

    ತಾರ್ಕಿಕ allow_active_wait = ಸತ್ಯ;

#ifdef CV_PROFILE_THREADS
    ThreadPool::ThreadStatistics& stat = thread_pool.threads_stat[id + 1];
#endif

    ಯಾವತ್ (!stop_thread)
    {
        CV_LOG_VERBOSE(ರಿಕ್ತ, 5, "Thread: ... loop iteration: allow_active_wait=" << allow_active_wait << "   has_wake_signal=" << has_wake_signal);
        ಯದಿ (allow_active_wait && CV_WORKER_ACTIVE_WAIT > 0)
        {
            allow_active_wait = ಅಸತ್ಯ;
            ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < CV_WORKER_ACTIVE_WAIT; i++)
            {
                ಯದಿ (has_wake_signal)
                    ಭಙ್ಗ;
                ಯದಿ (CV_ACTIVE_WAIT_PAUSE_LIMIT > 0 && (i < CV_ACTIVE_WAIT_PAUSE_LIMIT || (i & 1)))
                    CV_PAUSE(16);
                ಅನ್ಯಥಾ
                    CV_YIELD();
            }
        }
        pthread_mutex_lock(&mutex);
#ifdef CV_PROFILE_THREADS
        stat.threadWait = get_Tick_ಎಣಿಕೆ();
#endif
        ಯಾವತ್ (!has_wake_signal) // to handle spurious wakeups
        {
            //CV_LOG_VERBOSE(NULL, 5, "Thread: wait (sleep) ...");
#ಯದಿ defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
            pthread_cond_wait(&thread_pool.cond_thread_wake, &mutex);
#ಅನ್ಯಥಾ
            isActive = ಅಸತ್ಯ;
            pthread_cond_wait(&cond_thread_wake, &mutex);
            isActive = ಸತ್ಯ;
#endif
            CV_LOG_VERBOSE(ರಿಕ್ತ, 5, "Thread: wake ... (has_wake_signal=" << has_wake_signal << " stop_thread=" << stop_thread << ")")
        }
#ifdef CV_PROFILE_THREADS
        stat.threadWake = get_Tick_ಎಣಿಕೆ();
#endif

        CV_LOG_VERBOSE(ರಿಕ್ತ, 5, "Thread: checking for new job");
        ಯದಿ (CV_WORKER_ACTIVE_WAIT_THREADS_LIMIT == 0)
            allow_active_wait = ಸತ್ಯ;
        Ptr<ParallelJob> j_ptr; swap(j_ptr, job);
        has_wake_signal = ಅಸತ್ಯ;
        pthread_mutex_unlock(&mutex);

        ಯದಿ (!stop_thread)
        {
            ParallelJob* j = j_ptr;
            ಯದಿ (j)
            {
                CV_LOG_VERBOSE(ರಿಕ್ತ, 5, "Thread: job size=" << j->range.size() << " done=" << j->current_task);
                ಯದಿ (j->current_task < j->range.size())
                {
                    ಪೂರ್ಣಾಙ್ಕ other = j->active_thread_ಎಣಿಕೆ.fetch_add(1, std::memory_order_seq_cst);
                    CV_LOG_VERBOSE(ರಿಕ್ತ, 5, "Thread: processing new job (with " << other << " other threads)"); CV_UNUSED(other);
#ifdef CV_PROFILE_THREADS
                    stat.threadExecuteStart = get_Tick_ಎಣಿಕೆ();
                    stat.executedTasks = j->execute(ಸತ್ಯ);
                    stat.threadExecuteStop = get_Tick_ಎಣಿಕೆ();
#ಅನ್ಯಥಾ
                    j->execute(ಸತ್ಯ);
#endif
                    ಪೂರ್ಣಾಙ್ಕ completed = j->completed_thread_ಎಣಿಕೆ.fetch_add(1, std::memory_order_seq_cst) + 1;
                    ಪೂರ್ಣಾಙ್ಕ active = j->active_thread_ಎಣಿಕೆ.load(std::memory_order_acquire);
                    ಯದಿ (CV_WORKER_ACTIVE_WAIT_THREADS_LIMIT > 0)
                    {
                        allow_active_wait = ಸತ್ಯ;
                        ಯದಿ (active >= CV_WORKER_ACTIVE_WAIT_THREADS_LIMIT && (id & 1) == 0) // turn off a half of threads
                            allow_active_wait = ಅಸತ್ಯ;
                    }
                    CV_LOG_VERBOSE(ರಿಕ್ತ, 5, "Thread: completed job processing: " << active << " " << completed);
                    ಯದಿ (active == completed)
                    {
                        ತಾರ್ಕಿಕ need_signal = !j->is_completed;
                        j->is_completed = ಸತ್ಯ;
                        j = ರಿಕ್ತ; j_ptr.release();
                        ಯದಿ (need_signal)
                        {
                            CV_LOG_VERBOSE(ರಿಕ್ತ, 5, "Thread: job finished => notifying the main thread");
                            pthread_mutex_lock(&thread_pool.mutex_notify);  // to avoid signal miss due pre-check condition
                            // empty
                            pthread_mutex_unlock(&thread_pool.mutex_notify);
                            pthread_cond_broadcast/*pthread_cond_signal*/(&thread_pool.cond_thread_task_complete);
                        }
                    }
                }
                ಅನ್ಯಥಾ
                {
                    CV_LOG_VERBOSE(ರಿಕ್ತ, 5, "Thread: no free job tasks");
                }
            }
        }
#ifdef CV_PROFILE_THREADS
        stat.threadFree = get_Tick_ಎಣಿಕೆ();
        stat.keepActive = allow_active_wait;
#endif
    }
}

ThreadPool::ThreadPool()
{
#ifdef CV_PROFILE_THREADS
    tickFreq = getTickFrequency();
#endif

    ಪೂರ್ಣಾಙ್ಕ res = 0;
    res |= pthread_mutex_init(&mutex, ರಿಕ್ತ);
    res |= pthread_mutex_init(&mutex_notify, ರಿಕ್ತ);
#ಯದಿ defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
    res |= pthread_cond_init(&cond_thread_wake, ರಿಕ್ತ);
#endif
    res |= pthread_cond_init(&cond_thread_task_complete, ರಿಕ್ತ);

    ಯದಿ (0 != res)
    {
        CV_LOG_FATAL(ರಿಕ್ತ, "Failed to initialize ThreadPool (pthreads)");
    }
    ಅಂಕೆ_threads = default_ಸಂಖ್ಯೆ_Of_Threads();
}

ತಾರ್ಕಿಕ ThreadPool::reconfigure_(unsigned new_threads_ಎಣಿಕೆ)
{
    ಯದಿ (new_threads_ಎಣಿಕೆ == threads.size())
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    ಯದಿ (new_threads_ಎಣಿಕೆ < threads.size())
    {
        CV_LOG_VERBOSE(ರಿಕ್ತ, 1, "MainThread: reduce worker pool: " << threads.size() << " => " << new_threads_ಎಣಿಕೆ);
        std::vector< Ptr<WorkerThread> > release_threads(threads.size() - new_threads_ಎಣಿಕೆ);
        ಪ್ರತಿ (size_t i = new_threads_ಎಣಿಕೆ; i < threads.size(); ++i)
        {
            pthread_mutex_lock(&threads[i]->mutex);  // to avoid signal miss due pre-check
            threads[i]->stop_thread = ಸತ್ಯ;
            threads[i]->has_wake_signal = ಸತ್ಯ;
#ಯದಿ !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
            pthread_mutex_unlock(&threads[i]->mutex);
            pthread_cond_broadcast/*pthread_cond_signal*/(&threads[i]->cond_thread_wake); // wake thread
#ಅನ್ಯಥಾ
            pthread_mutex_unlock(&threads[i]->mutex);
#endif
            std::swap(threads[i], release_threads[i - new_threads_ಎಣಿಕೆ]);
        }
#ಯದಿ defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
        CV_LOG_VERBOSE(ರಿಕ್ತ, 1, "MainThread: notify worker threads about termination...");
        pthread_cond_broadcast(&cond_thread_wake); // wake all threads
#endif
        threads.resize(new_threads_ಎಣಿಕೆ);
        release_threads.clear();  // calls thread_join which want to lock mutex
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }
    ಅನ್ಯಥಾ
    {
        CV_LOG_VERBOSE(ರಿಕ್ತ, 1, "MainThread: upgrade worker pool: " << threads.size() << " => " << new_threads_ಎಣಿಕೆ);
        ಪ್ರತಿ (size_t i = threads.size(); i < new_threads_ಎಣಿಕೆ; ++i)
        {
            threads.push_back(Ptr<WorkerThread>(ನೂತನ WorkerThread(*ಏತತ್, (unsigned)i))); // spawn more threads
        }
    }
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
}

ThreadPool::~ThreadPool()
{
    reconfigure(0);
    pthread_cond_destroy(&cond_thread_task_complete);
#ಯದಿ defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
    pthread_cond_destroy(&cond_thread_wake);
#endif
    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&mutex_notify);
}

ಶೂನ್ಯ ThreadPool::run(ಧ್ರುವ Range& range, ಧ್ರುವ ParallelLoopBody& body, ದ್ವಿಗುಣ nstripes)
{
    CV_LOG_VERBOSE(ರಿಕ್ತ, 1, "MainThread: new parallel job: num_threads=" << ಅಂಕೆ_threads << "   range=" << range.size() << "   nstripes=" << nstripes << "   job=" << (ಶೂನ್ಯ*)job);
#ifdef CV_PROFILE_THREADS
    jobSubmitTime = get_Tick_ಎಣಿಕೆ();
    threads_stat[0].reset();
    threads_stat[0].threadWait = jobSubmitTime;
    threads_stat[0].threadWake = jobSubmitTime;
#endif
    ಯದಿ (get_ಅಂಕೆ_Of_Threads() > 1 &&
        job == ರಿಕ್ತ &&
        (range.size() * nstripes >= 2 || (range.size() > 1 && nstripes <= 0))
    )
    {
        pthread_mutex_lock(&mutex);
        ಯದಿ (job != ರಿಕ್ತ)
        {
            pthread_mutex_unlock(&mutex);
            body(range);
            ಪ್ರತ್ಯಾಗಮ;
        }
        reconfigure_(ಅಂಕೆ_threads - 1);

        {
            CV_LOG_VERBOSE(ರಿಕ್ತ, 1, "MainThread: initialize parallel job: " << range.size());
            job = Ptr<ParallelJob>(ನೂತನ ParallelJob(*ಏತತ್, range, body, nstripes));
            pthread_mutex_unlock(&mutex);

            CV_LOG_VERBOSE(ರಿಕ್ತ, 5, "MainThread: wake worker threads...");
            size_t ಅಂಕೆ_threads_to_wake = std::min(static_cast<size_t>(range.size()), threads.size());
            ಪ್ರತಿ (size_t i = 0; i < ಅಂಕೆ_threads_to_wake; ++i)
            {
                ಯದಿ (job->current_task >= job->range.size())
                    ಭಙ್ಗ;
                WorkerThread& thread = *(threads[i].get());
                ಯದಿ (
#ಯದಿ defined(__clang__) && defined(__has_feature)
#ಯದಿ __has_feature(thread_sanitizer)
                        1 ||  // Robust workaround to avoid data race warning of `thread.job`
#endif
#endif
#ಯದಿ !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
                        thread.isActive ||
#endif
                        thread.has_wake_signal
                        || !thread.job.empty()  // #10881
                )
                {
                    pthread_mutex_lock(&thread.mutex);
                    thread.job = job;
#ಯದಿ !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
                    ತಾರ್ಕಿಕ isActive = thread.isActive;
#endif
                    thread.has_wake_signal = ಸತ್ಯ;
#ifdef CV_PROFILE_THREADS
                    threads_stat[i + 1].reset();
#endif
                    pthread_mutex_unlock(&thread.mutex);
#ಯದಿ !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
                    ಯದಿ (!isActive)
                    {
                        pthread_cond_broadcast/*pthread_cond_signal*/(&thread.cond_thread_wake); // wake thread
                    }
#endif
                }
                ಅನ್ಯಥಾ
                {
                    CV_Assert(thread.job.empty());
                    thread.job = job;
                    thread.has_wake_signal = ಸತ್ಯ;
#ifdef CV_PROFILE_THREADS
                    threads_stat[i + 1].reset();
#endif
#ಯದಿ !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
                    pthread_cond_broadcast/*pthread_cond_signal*/(&thread.cond_thread_wake); // wake thread
#endif
                }
            }
#ifdef CV_PROFILE_THREADS
            threads_stat[0].threadPing = get_Tick_ಎಣಿಕೆ();
#endif
#ಯದಿ defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
            pthread_cond_broadcast(&cond_thread_wake); // wake all threads
#endif
#ifdef CV_PROFILE_THREADS
            threads_stat[0].threadWake = get_Tick_ಎಣಿಕೆ();
#endif
            CV_LOG_VERBOSE(ರಿಕ್ತ, 5, "MainThread: wake worker threads... (done)");

            {
                ParallelJob& j = *(ಏತತ್->job);
#ifdef CV_PROFILE_THREADS
                threads_stat[0].threadExecuteStart = get_Tick_ಎಣಿಕೆ();
                threads_stat[0].executedTasks = j.execute(ಅಸತ್ಯ);
                threads_stat[0].threadExecuteStop = get_Tick_ಎಣಿಕೆ();
#ಅನ್ಯಥಾ
                j.execute(ಅಸತ್ಯ);
#endif
                CV_Assert(j.current_task >= j.range.size());
                CV_LOG_VERBOSE(ರಿಕ್ತ, 5, "MainThread: complete self-tasks: " << j.active_thread_ಎಣಿಕೆ << " " << j.completed_thread_ಎಣಿಕೆ);
                ಯದಿ (job->is_completed || j.active_thread_ಎಣಿಕೆ == 0)
                {
                    job->is_completed = ಸತ್ಯ;
                    CV_LOG_VERBOSE(ರಿಕ್ತ, 5, "MainThread: no WIP worker threads");
                }
                ಅನ್ಯಥಾ
                {
                    ಯದಿ (CV_MAIN_THREAD_ACTIVE_WAIT > 0)
                    {
                        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < CV_MAIN_THREAD_ACTIVE_WAIT; i++)  // don't spin too much in any case (inaccurate getTickCount())
                        {
                            ಯದಿ (job->is_completed)
                            {
                                CV_LOG_VERBOSE(ರಿಕ್ತ, 5, "MainThread: job finalize (active wait) " << j.active_thread_ಎಣಿಕೆ << " " << j.completed_thread_ಎಣಿಕೆ);
                                ಭಙ್ಗ;
                            }
                            ಯದಿ (CV_ACTIVE_WAIT_PAUSE_LIMIT > 0 && (i < CV_ACTIVE_WAIT_PAUSE_LIMIT || (i & 1)))
                                CV_PAUSE(16);
                            ಅನ್ಯಥಾ
                                CV_YIELD();
                        }
                    }
                    ಯದಿ (!job->is_completed)
                    {
                        CV_LOG_VERBOSE(ರಿಕ್ತ, 5, "MainThread: prepare wait " << j.active_thread_ಎಣಿಕೆ << " " << j.completed_thread_ಎಣಿಕೆ);
                        pthread_mutex_lock(&mutex_notify);
                        ಪ್ರತಿ (;;)
                        {
                            ಯದಿ (job->is_completed)
                            {
                                CV_LOG_VERBOSE(ರಿಕ್ತ, 5, "MainThread: job finalize (wait) " << j.active_thread_ಎಣಿಕೆ << " " << j.completed_thread_ಎಣಿಕೆ);
                                ಭಙ್ಗ;
                            }
                            CV_LOG_VERBOSE(ರಿಕ್ತ, 5, "MainThread: wait completion (sleep) ...");
                            pthread_cond_wait(&cond_thread_task_complete, &mutex_notify);
                            CV_LOG_VERBOSE(ರಿಕ್ತ, 5, "MainThread: wake");
                        }
                        pthread_mutex_unlock(&mutex_notify);
                    }
                }
            }
#ifdef CV_PROFILE_THREADS
            threads_stat[0].threadFree = get_Tick_ಎಣಿಕೆ();
            std::cout << "Job: sz=" << range.size() << " nstripes=" << nstripes << "    Time: " << (threads_stat[0].threadFree - jobSubmitTime) / tickFreq * 1e6 << " usec" << std::endl;
            ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < (ಪೂರ್ಣಾಙ್ಕ)threads.size() + 1; i++)
            {
                threads_stat[i].dump(i - 1, jobSubmitTime, tickFreq);
            }
#endif
            ಯದಿ (job)
            {
                pthread_mutex_lock(&mutex);
                CV_LOG_VERBOSE(ರಿಕ್ತ, 5, "MainThread: job release");
                CV_Assert(job->is_completed);
                job.release();
                pthread_mutex_unlock(&mutex);
            }
        }
    }
    ಅನ್ಯಥಾ
    {
        body(range);
    }
}

size_t ThreadPool::get_ಅಂಕೆ_Of_Threads()
{
    ಪ್ರತ್ಯಾಗಮ ಅಂಕೆ_threads;
}

ಶೂನ್ಯ ThreadPool::set_ಅಂಕೆ_Of_Threads(unsigned n)
{
    ಯದಿ (n != ಅಂಕೆ_threads)
    {
        ಅಂಕೆ_threads = n;
        ಯದಿ (n == 1)
           ಯದಿ (job == ರಿಕ್ತ) reconfigure(0);  // stop worker threads immediately
    }
}

size_t parallel_pthreads_get_threads_ಅಂಕೆ()
{
    ಪ್ರತ್ಯಾಗಮ ThreadPool::instance().get_ಅಂಕೆ_Of_Threads();
}

ಶೂನ್ಯ parallel_pthreads_set_threads_ಅಂಕೆ(ಪೂರ್ಣಾಙ್ಕ num)
{
    ಯದಿ(num < 0)
    {
        ThreadPool::instance().set_ಅಂಕೆ_Of_Threads(0);
    }
    ಅನ್ಯಥಾ
    {
        ThreadPool::instance().set_ಅಂಕೆ_Of_Threads(unsigned(num));
    }
}

ಶೂನ್ಯ parallel_for_pthreads(ಧ್ರುವ Range& range, ಧ್ರುವ ParallelLoopBody& body, ದ್ವಿಗುಣ nstripes)
{
    ThreadPool::instance().run(range, body, nstripes);
}

}

#endif
