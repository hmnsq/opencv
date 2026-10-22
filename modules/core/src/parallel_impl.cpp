// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#അന്തര്ഭാവ "precomp.hpp"

#അന്തര്ഭാവ "parallel_impl.hpp"

#ifdef HAVE_PTHREADS_PF
#അന്തര്ഭാവ <pthread.h>

#അന്തര്ഭാവ <opencv2/core/utils/configuration.private.hpp>

#അന്തര്ഭാവ <opencv2/core/utils/logger.defines.hpp>
//#undef CV_LOG_STRIP_LEVEL
//#define CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_VERBOSE + 1
#അന്തര്ഭാവ <opencv2/core/utils/logger.hpp>

#അന്തര്ഭാവ <opencv2/core/utils/trace.private.hpp>

//#define CV_PROFILE_THREADS 64
//#define getTickCount getCPUTickCount  // use this if getTickCount() calls are expensive (and getCPUTickCount() is accurate)

//#define CV_USE_GLOBAL_WORKERS_COND_VAR  // not effective on many-core systems (10+)

#അന്തര്ഭാവ <atomic>

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
# എങ്കിൽ defined __GNUC__ && (defined __i386__ || defined __x86_64__)
#   include <x86intrin.h> /* for __rdtsc */
#   എങ്കിൽ !defined(__SSE2__)
      സ്ഥിതം വരിയിൽ ശൂന്യം cv_non_sse_mm_pause() { __asm__ __volatile__ ("rep; nop"); }
#     define _mm_pause cv_non_sse_mm_pause
#   endif
// With Skylake CPUs and above, _mm_pause takes 140 cycles so no need for a loop.
#   define CV_PAUSE(v) ചെയ്യുക { (ശൂന്യം)v; _mm_pause(); } ആകുമ്പോൾ (0)
# elif defined __GNUC__ && defined __aarch64__
#   define CV_PAUSE(v) ചെയ്യുക { വേണ്ടി (പൂർണ്ണസംഖ്യ __delay = (v); __delay > 0; --__delay) { asm അസ്ഥിര("yield" ::: "memory"); } } ആകുമ്പോൾ (0)
# elif defined __GNUC__ && defined __arm__
#   define CV_PAUSE(v) ചെയ്യുക { വേണ്ടി (പൂർണ്ണസംഖ്യ __delay = (v); __delay > 0; --__delay) { asm അസ്ഥിര("" ::: "memory"); } } ആകുമ്പോൾ (0)
# elif defined __GNUC__ && defined __mips__ && __mips_isa_rev >= 2
#   define CV_PAUSE(v) ചെയ്യുക { വേണ്ടി (പൂർണ്ണസംഖ്യ __delay = (v); __delay > 0; --__delay) { asm അസ്ഥിര("pause" ::: "memory"); } } ആകുമ്പോൾ (0)
# elif defined __APPLE__ && defined __POWERPC__
#   define CV_PAUSE(v) ചെയ്യുക { വേണ്ടി (പൂർണ്ണസംഖ്യ __delay = (v); __delay > 0; --__delay) { asm അസ്ഥിര("or r27,r27,r27" ::: "memory"); } } ആകുമ്പോൾ (0)
# elif defined __GNUC__ && defined __PPC64__
#   define CV_PAUSE(v) ചെയ്യുക { വേണ്ടി (പൂർണ്ണസംഖ്യ __delay = (v); __delay > 0; --__delay) { asm അസ്ഥിര("or 27,27,27" ::: "memory"); } } ആകുമ്പോൾ (0)
# elif defined __GNUC__ && defined __riscv
// PAUSE HINT is not part of RISC-V ISA yet, but is under discussion now. For details see:
// https://github.com/riscv/riscv-isa-manual/pull/398
// https://github.com/riscv/riscv-isa-manual/issues/43
// #   define CV_PAUSE(v) do { for (int __delay = (v); __delay > 0; --__delay) { asm volatile("pause"); } } while (0)
#   define CV_PAUSE(v) ചെയ്യുക { വേണ്ടി (പൂർണ്ണസംഖ്യ __delay = (v); __delay > 0; --__delay) { asm അസ്ഥിര("nop"); } } ആകുമ്പോൾ (0)
# elif defined __GNUC__ && defined __loongarch__
#   define CV_PAUSE(v) ചെയ്യുക { വേണ്ടി (പൂർണ്ണസംഖ്യ __delay = (v); __delay > 0; --__delay) { asm അസ്ഥിര("nop"); } } ആകുമ്പോൾ (0)
# അല്ലെങ്കിൽ
#   warning "Can't detect 'pause' (CPU-yield) instruction on the target platform. Specify CV_PAUSE() definition via compiler flags."
#   define CV_PAUSE(...) ചെയ്യുക { /* no-op: works, but not effective */ } ആകുമ്പോൾ (0)
# endif
#endif // CV_PAUSE


നാമാകാശ cv
{

സ്ഥിതം പൂർണ്ണസംഖ്യ CV_ACTIVE_WAIT_PAUSE_LIMIT = (പൂർണ്ണസംഖ്യ)utils::getConfigurationParameterSizeT("OPENCV_THREAD_POOL_ACTIVE_WAIT_PAUSE_LIMIT", 16);  // iterations
സ്ഥിതം പൂർണ്ണസംഖ്യ CV_WORKER_ACTIVE_WAIT = (പൂർണ്ണസംഖ്യ)utils::getConfigurationParameterSizeT("OPENCV_THREAD_POOL_ACTIVE_WAIT_WORKER", 2000);  // iterations
സ്ഥിതം പൂർണ്ണസംഖ്യ CV_MAIN_THREAD_ACTIVE_WAIT = (പൂർണ്ണസംഖ്യ)utils::getConfigurationParameterSizeT("OPENCV_THREAD_POOL_ACTIVE_WAIT_MAIN", 10000); // iterations

സ്ഥിതം പൂർണ്ണസംഖ്യ CV_WORKER_ACTIVE_WAIT_THREADS_LIMIT = (പൂർണ്ണസംഖ്യ)utils::getConfigurationParameterSizeT("OPENCV_THREAD_POOL_ACTIVE_WAIT_THREADS_LIMIT", 0); // number of real cores

വർഗ്ഗം WorkerThread;
വർഗ്ഗം ParallelJob;

വർഗ്ഗം ThreadPool
{
പൊതു:
    സ്ഥിതം ThreadPool& instance()
    {
        CV_SINGLETON_LAZY_INIT_REF(ThreadPool, പുതിയ ThreadPool())
    }

    സ്ഥിതം ശൂന്യം stop()
    {
        ThreadPool& manager = instance();
        manager.reconfigure(0);
    }

    ശൂന്യം reconfigure(unsigned new_threads_എണ്ണം)
    {
        എങ്കിൽ (new_threads_എണ്ണം == threads.size())
            തിരികെ;
        pthread_mutex_lock(&mutex);
        reconfigure_(new_threads_എണ്ണം);
        pthread_mutex_unlock(&mutex);
    }
    തർക്കം reconfigure_(unsigned new_threads_എണ്ണം); // internal implementation

    ശൂന്യം run(സ്ഥിരം Range& range, സ്ഥിരം ParallelLoopBody& body, ഇരട്ട nstripes);

    size_t get_അക്കം_Of_Threads();

    ശൂന്യം set_അക്കം_Of_Threads(unsigned n);

    ThreadPool();

    ~ThreadPool();

    unsigned അക്കം_threads;

    pthread_mutex_t mutex;  // guards fields (job/threads) from non-worker threads (concurrent parallel_for calls)
#എങ്കിൽ defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
    pthread_cond_t cond_thread_wake;
#endif

    pthread_mutex_t mutex_notify;
    pthread_cond_t cond_thread_task_complete;

    std::vector< Ptr<WorkerThread> > threads;

    Ptr<ParallelJob> job;

#ifdef CV_PROFILE_THREADS
    ഇരട്ട tickFreq;
    int64 jobSubmitTime;
    ഘടന ThreadStatistics
    {
        ThreadStatistics() : threadWait(0)
        {
            reset();
        }
        ശൂന്യം reset()
        {
            threadWake = 0;
            threadExecuteStart = 0;
            threadExecuteStop = 0;
            executedTasks = 0;
            keepActive = അസത്യം;
            threadPing = get_Tick_എണ്ണം();
        }
        int64 threadWait; // don't reset by default
        int64 threadPing; // don't reset by default
        int64 threadWake;
        int64 threadExecuteStart;
        int64 threadExecuteStop;
        int64 threadFree;
        unsigned executedTasks;
        തർക്കം keepActive;

        int64 dummy_[8]; // separate cache lines

        ശൂന്യം dump(പൂർണ്ണസംഖ്യ id, int64 baseTime, ഇരട്ട tickFreq)
        {
            എങ്കിൽ (id < 0)
                std::cout << "Main: ";
            അല്ലെങ്കിൽ
                printf("T%03d: ", id + 2);
            printf("wait=% 10.1f   ping=% 6.1f",
                    threadWait > 0 ? (threadWait - baseTime) / tickFreq * 1e6 : -0.0,
                    threadPing > 0 ? (threadPing - baseTime) / tickFreq * 1e6 : -0.0);
            എങ്കിൽ (threadWake > 0)
                printf("   wake=% 6.1f",
                    (threadWake > 0 ? (threadWake - baseTime) / tickFreq * 1e6 : -0.0));
            എങ്കിൽ (threadExecuteStart > 0)
            {
                printf("   exec=% 6.1f - % 6.1f   tasksDone=%5u   free=% 6.1f",
                    (threadExecuteStart > 0 ? (threadExecuteStart - baseTime) / tickFreq * 1e6 : -0.0),
                    (threadExecuteStop > 0 ? (threadExecuteStop - baseTime) / tickFreq * 1e6 : -0.0),
                    executedTasks,
                    (threadFree > 0 ? (threadFree - baseTime) / tickFreq * 1e6 : -0.0));
                എങ്കിൽ (id >= 0)
                    printf(" active=%s\n", keepActive ? "true" : "false");
                അല്ലെങ്കിൽ
                    printf("\n");
            }
            അല്ലെങ്കിൽ
                printf("   ------------------------------------------------------------------------------\n");
        }
    };
    ThreadStatistics threads_stat[CV_PROFILE_THREADS]; // 0 - main thread, 1..N - worker threads
#endif

};

വർഗ്ഗം WorkerThread
{
പൊതു:
    ThreadPool& thread_pool;
    സ്ഥിരം unsigned id;
    pthread_t posix_thread;
    തർക്കം is_created;

    std::atomic<തർക്കം> stop_thread;

    std::atomic<തർക്കം> has_wake_signal;

    Ptr<ParallelJob> job;

    pthread_mutex_t mutex;
#എങ്കിൽ !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
    അസ്ഥിര തർക്കം isActive;
    pthread_cond_t cond_thread_wake;
#endif

    WorkerThread(ThreadPool& thread_pool_, unsigned id_) :
        thread_pool(thread_pool_),
        id(id_),
        posix_thread(0),
        is_created(അസത്യം),
        stop_thread(അസത്യം),
        has_wake_signal(അസത്യം)
#എങ്കിൽ !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
        , isActive(സത്യം)
#endif
    {
        CV_LOG_VERBOSE(ശൂന്യ, 1, "MainThread: initializing new worker: " << id);
        പൂർണ്ണസംഖ്യ res = pthread_mutex_init(&mutex, ശൂന്യ);
        എങ്കിൽ (res != 0)
        {
            CV_LOG_ERROR(ശൂന്യ, id << ": Can't create thread mutex: res = " << res);
            തിരികെ;
        }
#എങ്കിൽ !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
        res = pthread_cond_init(&cond_thread_wake, ശൂന്യ);
        എങ്കിൽ (res != 0)
        {
            CV_LOG_ERROR(ശൂന്യ, id << ": Can't create thread condition variable: res = " << res);
            തിരികെ;
        }
#endif
        res = pthread_create(&posix_thread, ശൂന്യ, thread_loop_wrapper, (ശൂന്യം*)ഇത്);
        എങ്കിൽ (res != 0)
        {
            CV_LOG_ERROR(ശൂന്യ, id << ": Can't spawn new thread: res = " << res);
        }
        അല്ലെങ്കിൽ
        {
            is_created = സത്യം;
        }
    }

    ~WorkerThread()
    {
        CV_LOG_VERBOSE(ശൂന്യ, 1, "MainThread: destroy worker thread: " << id);
        എങ്കിൽ (is_created)
        {
            എങ്കിൽ (!stop_thread)
            {
                pthread_mutex_lock(&mutex);  // to avoid signal miss due pre-check
                stop_thread = സത്യം;
                pthread_mutex_unlock(&mutex);
#എങ്കിൽ defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
                pthread_cond_broadcast(&thread_pool.cond_thread_wake);
#അല്ലെങ്കിൽ
                pthread_cond_signal(&cond_thread_wake);
#endif
            }
            pthread_join(posix_thread, ശൂന്യ);
        }
#എങ്കിൽ !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
        pthread_cond_destroy(&cond_thread_wake);
#endif
        pthread_mutex_destroy(&mutex);
    }

    ശൂന്യം thread_body();
    സ്ഥിതം ശൂന്യം* thread_loop_wrapper(ശൂന്യം* thread_object)
    {
#ifdef OPENCV_WITH_ITT
        __itt_thread_set_name(cv::format("OpenCVThread-%03d", cv::utils::getThreadID()).c_str());
#endif
        ((WorkerThread*)thread_object)->thread_body();
        തിരികെ 0;
    }
};

വർഗ്ഗം ParallelJob
{
പൊതു:
    ParallelJob(സ്ഥിരം ThreadPool& thread_pool_, സ്ഥിരം Range& range_, സ്ഥിരം ParallelLoopBody& body_, പൂർണ്ണസംഖ്യ nstripes_) :
        thread_pool(thread_pool_),
        body(body_),
        range(range_),
        nstripes((unsigned)nstripes_),
        is_completed(അസത്യം)
    {
        CV_LOG_VERBOSE(ശൂന്യ, 5, "ParallelJob::ParallelJob(" << (ശൂന്യം*)ഇത് << ")");
        current_task.store(0, std::memory_order_relaxed);
        active_thread_എണ്ണം.store(0, std::memory_order_relaxed);
        completed_thread_എണ്ണം.store(0, std::memory_order_relaxed);
        dummy0_[0] = 0, dummy1_[0] = 0, dummy2_[0] = 0; // compiler warning
    }

    ~ParallelJob()
    {
        CV_LOG_VERBOSE(ശൂന്യ, 5, "ParallelJob::~ParallelJob(" << (ശൂന്യം*)ഇത് << ")");
    }

    unsigned execute(തർക്കം is_worker_thread)
    {
        unsigned executed_tasks = 0;
        സ്ഥിരം പൂർണ്ണസംഖ്യ task_എണ്ണം = range.size();
        സ്ഥിരം പൂർണ്ണസംഖ്യ remaining_multiplier = std::min(nstripes,
                std::max(
                        std::min(100u, thread_pool.അക്കം_threads * 4),
                        thread_pool.അക്കം_threads * 2
                ));  // experimental value
        വേണ്ടി (;;)
        {
            പൂർണ്ണസംഖ്യ chunk_size = std::max(1, (task_എണ്ണം - current_task) / remaining_multiplier);
            പൂർണ്ണസംഖ്യ id = current_task.fetch_add(chunk_size, std::memory_order_seq_cst);
            എങ്കിൽ (id >= task_എണ്ണം)
                നിർത്തുക; // no more free tasks

            executed_tasks += chunk_size;
            പൂർണ്ണസംഖ്യ start_id = id;
            പൂർണ്ണസംഖ്യ end_id = std::min(task_എണ്ണം, id + chunk_size);
            CV_LOG_VERBOSE(ശൂന്യ, 9, "Thread: job " << start_id << "-" << end_id);

            //TODO: if (not pending exception)
            {
                body.operator()(Range(range.start + start_id, range.start + end_id));
            }
            എങ്കിൽ (is_worker_thread && is_completed)
            {
                CV_LOG_ERROR(ശൂന്യ, "\t\t\t\tBUG! Job: " << (ശൂന്യം*)ഇത് << " " << id << " " << active_thread_എണ്ണം << " " << completed_thread_എണ്ണം);
                CV_Assert(!is_completed); // TODO Dbg this
            }
        }
        തിരികെ executed_tasks;
    }

    സ്ഥിരം ThreadPool& thread_pool;
    സ്ഥിരം ParallelLoopBody& body;
    സ്ഥിരം Range range;
    സ്ഥിരം unsigned nstripes;

    std::atomic<പൂർണ്ണസംഖ്യ> current_task;  // next free part of job
    int64 dummy0_[8];  // avoid cache-line reusing for the same atomics

    std::atomic<പൂർണ്ണസംഖ്യ> active_thread_എണ്ണം;  // number of threads worked on this job
    int64 dummy1_[8];  // avoid cache-line reusing for the same atomics

    std::atomic<പൂർണ്ണസംഖ്യ> completed_thread_എണ്ണം;  // number of threads completed any activities on this job
    int64 dummy2_[8];  // avoid cache-line reusing for the same atomics

    std::atomic<തർക്കം> is_completed;

    // TODO exception handling
};


// Disable thread sanitization check when CV_USE_GLOBAL_WORKERS_COND_VAR is not
// set because it triggers as the main thread reads isActive while the children
// thread writes it (but it all works out because a mutex is locked in the main
// thread and isActive re-checked).
// This is to solve issue #19463.
#എങ്കിൽ !defined(CV_USE_GLOBAL_WORKERS_COND_VAR) && defined(__clang__) && defined(__has_feature)
#എങ്കിൽ __has_feature(thread_sanitizer)
__attribute__((no_sanitize("thread")))
#endif
#endif
ശൂന്യം WorkerThread::thread_body()
{
    (ശൂന്യം)cv::utils::getThreadID(); // notify OpenCV about new thread
    CV_LOG_VERBOSE(ശൂന്യ, 5, "Thread: new thread: " << id);

    തർക്കം allow_active_wait = സത്യം;

#ifdef CV_PROFILE_THREADS
    ThreadPool::ThreadStatistics& stat = thread_pool.threads_stat[id + 1];
#endif

    ആകുമ്പോൾ (!stop_thread)
    {
        CV_LOG_VERBOSE(ശൂന്യ, 5, "Thread: ... loop iteration: allow_active_wait=" << allow_active_wait << "   has_wake_signal=" << has_wake_signal);
        എങ്കിൽ (allow_active_wait && CV_WORKER_ACTIVE_WAIT > 0)
        {
            allow_active_wait = അസത്യം;
            വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < CV_WORKER_ACTIVE_WAIT; i++)
            {
                എങ്കിൽ (has_wake_signal)
                    നിർത്തുക;
                എങ്കിൽ (CV_ACTIVE_WAIT_PAUSE_LIMIT > 0 && (i < CV_ACTIVE_WAIT_PAUSE_LIMIT || (i & 1)))
                    CV_PAUSE(16);
                അല്ലെങ്കിൽ
                    CV_YIELD();
            }
        }
        pthread_mutex_lock(&mutex);
#ifdef CV_PROFILE_THREADS
        stat.threadWait = get_Tick_എണ്ണം();
#endif
        ആകുമ്പോൾ (!has_wake_signal) // to handle spurious wakeups
        {
            //CV_LOG_VERBOSE(NULL, 5, "Thread: wait (sleep) ...");
#എങ്കിൽ defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
            pthread_cond_wait(&thread_pool.cond_thread_wake, &mutex);
#അല്ലെങ്കിൽ
            isActive = അസത്യം;
            pthread_cond_wait(&cond_thread_wake, &mutex);
            isActive = സത്യം;
#endif
            CV_LOG_VERBOSE(ശൂന്യ, 5, "Thread: wake ... (has_wake_signal=" << has_wake_signal << " stop_thread=" << stop_thread << ")")
        }
#ifdef CV_PROFILE_THREADS
        stat.threadWake = get_Tick_എണ്ണം();
#endif

        CV_LOG_VERBOSE(ശൂന്യ, 5, "Thread: checking for new job");
        എങ്കിൽ (CV_WORKER_ACTIVE_WAIT_THREADS_LIMIT == 0)
            allow_active_wait = സത്യം;
        Ptr<ParallelJob> j_ptr; swap(j_ptr, job);
        has_wake_signal = അസത്യം;
        pthread_mutex_unlock(&mutex);

        എങ്കിൽ (!stop_thread)
        {
            ParallelJob* j = j_ptr;
            എങ്കിൽ (j)
            {
                CV_LOG_VERBOSE(ശൂന്യ, 5, "Thread: job size=" << j->range.size() << " done=" << j->current_task);
                എങ്കിൽ (j->current_task < j->range.size())
                {
                    പൂർണ്ണസംഖ്യ other = j->active_thread_എണ്ണം.fetch_add(1, std::memory_order_seq_cst);
                    CV_LOG_VERBOSE(ശൂന്യ, 5, "Thread: processing new job (with " << other << " other threads)"); CV_UNUSED(other);
#ifdef CV_PROFILE_THREADS
                    stat.threadExecuteStart = get_Tick_എണ്ണം();
                    stat.executedTasks = j->execute(സത്യം);
                    stat.threadExecuteStop = get_Tick_എണ്ണം();
#അല്ലെങ്കിൽ
                    j->execute(സത്യം);
#endif
                    പൂർണ്ണസംഖ്യ completed = j->completed_thread_എണ്ണം.fetch_add(1, std::memory_order_seq_cst) + 1;
                    പൂർണ്ണസംഖ്യ active = j->active_thread_എണ്ണം.load(std::memory_order_acquire);
                    എങ്കിൽ (CV_WORKER_ACTIVE_WAIT_THREADS_LIMIT > 0)
                    {
                        allow_active_wait = സത്യം;
                        എങ്കിൽ (active >= CV_WORKER_ACTIVE_WAIT_THREADS_LIMIT && (id & 1) == 0) // turn off a half of threads
                            allow_active_wait = അസത്യം;
                    }
                    CV_LOG_VERBOSE(ശൂന്യ, 5, "Thread: completed job processing: " << active << " " << completed);
                    എങ്കിൽ (active == completed)
                    {
                        തർക്കം need_signal = !j->is_completed;
                        j->is_completed = സത്യം;
                        j = ശൂന്യ; j_ptr.release();
                        എങ്കിൽ (need_signal)
                        {
                            CV_LOG_VERBOSE(ശൂന്യ, 5, "Thread: job finished => notifying the main thread");
                            pthread_mutex_lock(&thread_pool.mutex_notify);  // to avoid signal miss due pre-check condition
                            // empty
                            pthread_mutex_unlock(&thread_pool.mutex_notify);
                            pthread_cond_broadcast/*pthread_cond_signal*/(&thread_pool.cond_thread_task_complete);
                        }
                    }
                }
                അല്ലെങ്കിൽ
                {
                    CV_LOG_VERBOSE(ശൂന്യ, 5, "Thread: no free job tasks");
                }
            }
        }
#ifdef CV_PROFILE_THREADS
        stat.threadFree = get_Tick_എണ്ണം();
        stat.keepActive = allow_active_wait;
#endif
    }
}

ThreadPool::ThreadPool()
{
#ifdef CV_PROFILE_THREADS
    tickFreq = getTickFrequency();
#endif

    പൂർണ്ണസംഖ്യ res = 0;
    res |= pthread_mutex_init(&mutex, ശൂന്യ);
    res |= pthread_mutex_init(&mutex_notify, ശൂന്യ);
#എങ്കിൽ defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
    res |= pthread_cond_init(&cond_thread_wake, ശൂന്യ);
#endif
    res |= pthread_cond_init(&cond_thread_task_complete, ശൂന്യ);

    എങ്കിൽ (0 != res)
    {
        CV_LOG_FATAL(ശൂന്യ, "Failed to initialize ThreadPool (pthreads)");
    }
    അക്കം_threads = default_സംഖ്യ_Of_Threads();
}

തർക്കം ThreadPool::reconfigure_(unsigned new_threads_എണ്ണം)
{
    എങ്കിൽ (new_threads_എണ്ണം == threads.size())
        തിരികെ അസത്യം;

    എങ്കിൽ (new_threads_എണ്ണം < threads.size())
    {
        CV_LOG_VERBOSE(ശൂന്യ, 1, "MainThread: reduce worker pool: " << threads.size() << " => " << new_threads_എണ്ണം);
        std::vector< Ptr<WorkerThread> > release_threads(threads.size() - new_threads_എണ്ണം);
        വേണ്ടി (size_t i = new_threads_എണ്ണം; i < threads.size(); ++i)
        {
            pthread_mutex_lock(&threads[i]->mutex);  // to avoid signal miss due pre-check
            threads[i]->stop_thread = സത്യം;
            threads[i]->has_wake_signal = സത്യം;
#എങ്കിൽ !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
            pthread_mutex_unlock(&threads[i]->mutex);
            pthread_cond_broadcast/*pthread_cond_signal*/(&threads[i]->cond_thread_wake); // wake thread
#അല്ലെങ്കിൽ
            pthread_mutex_unlock(&threads[i]->mutex);
#endif
            std::swap(threads[i], release_threads[i - new_threads_എണ്ണം]);
        }
#എങ്കിൽ defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
        CV_LOG_VERBOSE(ശൂന്യ, 1, "MainThread: notify worker threads about termination...");
        pthread_cond_broadcast(&cond_thread_wake); // wake all threads
#endif
        threads.resize(new_threads_എണ്ണം);
        release_threads.clear();  // calls thread_join which want to lock mutex
        തിരികെ അസത്യം;
    }
    അല്ലെങ്കിൽ
    {
        CV_LOG_VERBOSE(ശൂന്യ, 1, "MainThread: upgrade worker pool: " << threads.size() << " => " << new_threads_എണ്ണം);
        വേണ്ടി (size_t i = threads.size(); i < new_threads_എണ്ണം; ++i)
        {
            threads.push_back(Ptr<WorkerThread>(പുതിയ WorkerThread(*ഇത്, (unsigned)i))); // spawn more threads
        }
    }
    തിരികെ അസത്യം;
}

ThreadPool::~ThreadPool()
{
    reconfigure(0);
    pthread_cond_destroy(&cond_thread_task_complete);
#എങ്കിൽ defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
    pthread_cond_destroy(&cond_thread_wake);
#endif
    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&mutex_notify);
}

ശൂന്യം ThreadPool::run(സ്ഥിരം Range& range, സ്ഥിരം ParallelLoopBody& body, ഇരട്ട nstripes)
{
    CV_LOG_VERBOSE(ശൂന്യ, 1, "MainThread: new parallel job: num_threads=" << അക്കം_threads << "   range=" << range.size() << "   nstripes=" << nstripes << "   job=" << (ശൂന്യം*)job);
#ifdef CV_PROFILE_THREADS
    jobSubmitTime = get_Tick_എണ്ണം();
    threads_stat[0].reset();
    threads_stat[0].threadWait = jobSubmitTime;
    threads_stat[0].threadWake = jobSubmitTime;
#endif
    എങ്കിൽ (get_അക്കം_Of_Threads() > 1 &&
        job == ശൂന്യ &&
        (range.size() * nstripes >= 2 || (range.size() > 1 && nstripes <= 0))
    )
    {
        pthread_mutex_lock(&mutex);
        എങ്കിൽ (job != ശൂന്യ)
        {
            pthread_mutex_unlock(&mutex);
            body(range);
            തിരികെ;
        }
        reconfigure_(അക്കം_threads - 1);

        {
            CV_LOG_VERBOSE(ശൂന്യ, 1, "MainThread: initialize parallel job: " << range.size());
            job = Ptr<ParallelJob>(പുതിയ ParallelJob(*ഇത്, range, body, nstripes));
            pthread_mutex_unlock(&mutex);

            CV_LOG_VERBOSE(ശൂന്യ, 5, "MainThread: wake worker threads...");
            size_t അക്കം_threads_to_wake = std::min(static_cast<size_t>(range.size()), threads.size());
            വേണ്ടി (size_t i = 0; i < അക്കം_threads_to_wake; ++i)
            {
                എങ്കിൽ (job->current_task >= job->range.size())
                    നിർത്തുക;
                WorkerThread& thread = *(threads[i].get());
                എങ്കിൽ (
#എങ്കിൽ defined(__clang__) && defined(__has_feature)
#എങ്കിൽ __has_feature(thread_sanitizer)
                        1 ||  // Robust workaround to avoid data race warning of `thread.job`
#endif
#endif
#എങ്കിൽ !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
                        thread.isActive ||
#endif
                        thread.has_wake_signal
                        || !thread.job.empty()  // #10881
                )
                {
                    pthread_mutex_lock(&thread.mutex);
                    thread.job = job;
#എങ്കിൽ !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
                    തർക്കം isActive = thread.isActive;
#endif
                    thread.has_wake_signal = സത്യം;
#ifdef CV_PROFILE_THREADS
                    threads_stat[i + 1].reset();
#endif
                    pthread_mutex_unlock(&thread.mutex);
#എങ്കിൽ !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
                    എങ്കിൽ (!isActive)
                    {
                        pthread_cond_broadcast/*pthread_cond_signal*/(&thread.cond_thread_wake); // wake thread
                    }
#endif
                }
                അല്ലെങ്കിൽ
                {
                    CV_Assert(thread.job.empty());
                    thread.job = job;
                    thread.has_wake_signal = സത്യം;
#ifdef CV_PROFILE_THREADS
                    threads_stat[i + 1].reset();
#endif
#എങ്കിൽ !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
                    pthread_cond_broadcast/*pthread_cond_signal*/(&thread.cond_thread_wake); // wake thread
#endif
                }
            }
#ifdef CV_PROFILE_THREADS
            threads_stat[0].threadPing = get_Tick_എണ്ണം();
#endif
#എങ്കിൽ defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
            pthread_cond_broadcast(&cond_thread_wake); // wake all threads
#endif
#ifdef CV_PROFILE_THREADS
            threads_stat[0].threadWake = get_Tick_എണ്ണം();
#endif
            CV_LOG_VERBOSE(ശൂന്യ, 5, "MainThread: wake worker threads... (done)");

            {
                ParallelJob& j = *(ഇത്->job);
#ifdef CV_PROFILE_THREADS
                threads_stat[0].threadExecuteStart = get_Tick_എണ്ണം();
                threads_stat[0].executedTasks = j.execute(അസത്യം);
                threads_stat[0].threadExecuteStop = get_Tick_എണ്ണം();
#അല്ലെങ്കിൽ
                j.execute(അസത്യം);
#endif
                CV_Assert(j.current_task >= j.range.size());
                CV_LOG_VERBOSE(ശൂന്യ, 5, "MainThread: complete self-tasks: " << j.active_thread_എണ്ണം << " " << j.completed_thread_എണ്ണം);
                എങ്കിൽ (job->is_completed || j.active_thread_എണ്ണം == 0)
                {
                    job->is_completed = സത്യം;
                    CV_LOG_VERBOSE(ശൂന്യ, 5, "MainThread: no WIP worker threads");
                }
                അല്ലെങ്കിൽ
                {
                    എങ്കിൽ (CV_MAIN_THREAD_ACTIVE_WAIT > 0)
                    {
                        വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < CV_MAIN_THREAD_ACTIVE_WAIT; i++)  // don't spin too much in any case (inaccurate getTickCount())
                        {
                            എങ്കിൽ (job->is_completed)
                            {
                                CV_LOG_VERBOSE(ശൂന്യ, 5, "MainThread: job finalize (active wait) " << j.active_thread_എണ്ണം << " " << j.completed_thread_എണ്ണം);
                                നിർത്തുക;
                            }
                            എങ്കിൽ (CV_ACTIVE_WAIT_PAUSE_LIMIT > 0 && (i < CV_ACTIVE_WAIT_PAUSE_LIMIT || (i & 1)))
                                CV_PAUSE(16);
                            അല്ലെങ്കിൽ
                                CV_YIELD();
                        }
                    }
                    എങ്കിൽ (!job->is_completed)
                    {
                        CV_LOG_VERBOSE(ശൂന്യ, 5, "MainThread: prepare wait " << j.active_thread_എണ്ണം << " " << j.completed_thread_എണ്ണം);
                        pthread_mutex_lock(&mutex_notify);
                        വേണ്ടി (;;)
                        {
                            എങ്കിൽ (job->is_completed)
                            {
                                CV_LOG_VERBOSE(ശൂന്യ, 5, "MainThread: job finalize (wait) " << j.active_thread_എണ്ണം << " " << j.completed_thread_എണ്ണം);
                                നിർത്തുക;
                            }
                            CV_LOG_VERBOSE(ശൂന്യ, 5, "MainThread: wait completion (sleep) ...");
                            pthread_cond_wait(&cond_thread_task_complete, &mutex_notify);
                            CV_LOG_VERBOSE(ശൂന്യ, 5, "MainThread: wake");
                        }
                        pthread_mutex_unlock(&mutex_notify);
                    }
                }
            }
#ifdef CV_PROFILE_THREADS
            threads_stat[0].threadFree = get_Tick_എണ്ണം();
            std::cout << "Job: sz=" << range.size() << " nstripes=" << nstripes << "    Time: " << (threads_stat[0].threadFree - jobSubmitTime) / tickFreq * 1e6 << " usec" << std::endl;
            വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < (പൂർണ്ണസംഖ്യ)threads.size() + 1; i++)
            {
                threads_stat[i].dump(i - 1, jobSubmitTime, tickFreq);
            }
#endif
            എങ്കിൽ (job)
            {
                pthread_mutex_lock(&mutex);
                CV_LOG_VERBOSE(ശൂന്യ, 5, "MainThread: job release");
                CV_Assert(job->is_completed);
                job.release();
                pthread_mutex_unlock(&mutex);
            }
        }
    }
    അല്ലെങ്കിൽ
    {
        body(range);
    }
}

size_t ThreadPool::get_അക്കം_Of_Threads()
{
    തിരികെ അക്കം_threads;
}

ശൂന്യം ThreadPool::set_അക്കം_Of_Threads(unsigned n)
{
    എങ്കിൽ (n != അക്കം_threads)
    {
        അക്കം_threads = n;
        എങ്കിൽ (n == 1)
           എങ്കിൽ (job == ശൂന്യ) reconfigure(0);  // stop worker threads immediately
    }
}

size_t parallel_pthreads_get_threads_അക്കം()
{
    തിരികെ ThreadPool::instance().get_അക്കം_Of_Threads();
}

ശൂന്യം parallel_pthreads_set_threads_അക്കം(പൂർണ്ണസംഖ്യ num)
{
    എങ്കിൽ(num < 0)
    {
        ThreadPool::instance().set_അക്കം_Of_Threads(0);
    }
    അല്ലെങ്കിൽ
    {
        ThreadPool::instance().set_അക്കം_Of_Threads(unsigned(num));
    }
}

ശൂന്യം parallel_for_pthreads(സ്ഥിരം Range& range, സ്ഥിരം ParallelLoopBody& body, ഇരട്ട nstripes)
{
    ThreadPool::instance().run(range, body, nstripes);
}

}

#endif
