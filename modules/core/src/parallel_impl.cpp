// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#شامل "precomp.hpp"

#شامل "parallel_impl.hpp"

#ifdef HAVE_PTHREADS_PF
#شامل <pthread.h>

#شامل <opencv2/core/utils/configuration.private.hpp>

#شامل <opencv2/core/utils/logger.defines.hpp>
//#undef CV_LOG_STRIP_LEVEL
//#define CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_VERBOSE + 1
#شامل <opencv2/core/utils/logger.hpp>

#شامل <opencv2/core/utils/trace.private.hpp>

//#define CV_PROFILE_THREADS 64
//#define getTickCount getCPUTickCount  // use this if getTickCount() calls are expensive (and getCPUTickCount() is accurate)

//#define CV_USE_GLOBAL_WORKERS_COND_VAR  // not effective on many-core systems (10+)

#شامل <atomic>

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
# اگر defined __GNUC__ && (defined __i386__ || defined __x86_64__)
#   include <x86intrin.h> /* for __rdtsc */
#   اگر !defined(__SSE2__)
      ایستا درونخطی تهی cv_non_sse_mm_pause() { __asm__ __volatile__ ("rep; nop"); }
#     define _mm_pause cv_non_sse_mm_pause
#   endif
// With Skylake CPUs and above, _mm_pause takes 140 cycles so no need for a loop.
#   define CV_PAUSE(v) انجامبده { (تهی)v; _mm_pause(); } وقتی (0)
# elif defined __GNUC__ && defined __aarch64__
#   define CV_PAUSE(v) انجامبده { برای (صحیح __delay = (v); __delay > 0; --__delay) { asm فرار("yield" ::: "memory"); } } وقتی (0)
# elif defined __GNUC__ && defined __arm__
#   define CV_PAUSE(v) انجامبده { برای (صحیح __delay = (v); __delay > 0; --__delay) { asm فرار("" ::: "memory"); } } وقتی (0)
# elif defined __GNUC__ && defined __mips__ && __mips_isa_rev >= 2
#   define CV_PAUSE(v) انجامبده { برای (صحیح __delay = (v); __delay > 0; --__delay) { asm فرار("pause" ::: "memory"); } } وقتی (0)
# elif defined __APPLE__ && defined __POWERPC__
#   define CV_PAUSE(v) انجامبده { برای (صحیح __delay = (v); __delay > 0; --__delay) { asm فرار("or r27,r27,r27" ::: "memory"); } } وقتی (0)
# elif defined __GNUC__ && defined __PPC64__
#   define CV_PAUSE(v) انجامبده { برای (صحیح __delay = (v); __delay > 0; --__delay) { asm فرار("or 27,27,27" ::: "memory"); } } وقتی (0)
# elif defined __GNUC__ && defined __riscv
// PAUSE HINT is not part of RISC-V ISA yet, but is under discussion now. For details see:
// https://github.com/riscv/riscv-isa-manual/pull/398
// https://github.com/riscv/riscv-isa-manual/issues/43
// #   define CV_PAUSE(v) do { for (int __delay = (v); __delay > 0; --__delay) { asm volatile("pause"); } } while (0)
#   define CV_PAUSE(v) انجامبده { برای (صحیح __delay = (v); __delay > 0; --__delay) { asm فرار("nop"); } } وقتی (0)
# elif defined __GNUC__ && defined __loongarch__
#   define CV_PAUSE(v) انجامبده { برای (صحیح __delay = (v); __delay > 0; --__delay) { asm فرار("nop"); } } وقتی (0)
# وگرنه
#   warning "Can't detect 'pause' (CPU-yield) instruction on the target platform. Specify CV_PAUSE() definition via compiler flags."
#   define CV_PAUSE(...) انجامبده { /* no-op: works, but not effective */ } وقتی (0)
# endif
#endif // CV_PAUSE


فضاینام cv
{

ایستا صحیح CV_ACTIVE_WAIT_PAUSE_LIMIT = (صحیح)utils::getConfigurationParameterSizeT("OPENCV_THREAD_POOL_ACTIVE_WAIT_PAUSE_LIMIT", 16);  // iterations
ایستا صحیح CV_WORKER_ACTIVE_WAIT = (صحیح)utils::getConfigurationParameterSizeT("OPENCV_THREAD_POOL_ACTIVE_WAIT_WORKER", 2000);  // iterations
ایستا صحیح CV_MAIN_THREAD_ACTIVE_WAIT = (صحیح)utils::getConfigurationParameterSizeT("OPENCV_THREAD_POOL_ACTIVE_WAIT_MAIN", 10000); // iterations

ایستا صحیح CV_WORKER_ACTIVE_WAIT_THREADS_LIMIT = (صحیح)utils::getConfigurationParameterSizeT("OPENCV_THREAD_POOL_ACTIVE_WAIT_THREADS_LIMIT", 0); // number of real cores

رده WorkerThread;
رده ParallelJob;

رده ThreadPool
{
عمومی:
    ایستا ThreadPool& instance()
    {
        CV_SINGLETON_LAZY_INIT_REF(ThreadPool, جدید ThreadPool())
    }

    ایستا تهی stop()
    {
        ThreadPool& manager = instance();
        manager.reconfigure(0);
    }

    تهی reconfigure(unsigned new_threads_شمار)
    {
        اگر (new_threads_شمار == threads.size())
            بازگشت;
        pthread_mutex_lock(&mutex);
        reconfigure_(new_threads_شمار);
        pthread_mutex_unlock(&mutex);
    }
    منطقی reconfigure_(unsigned new_threads_شمار); // internal implementation

    تهی run(ثابت Range& range, ثابت ParallelLoopBody& body, دوگانه nstripes);

    size_t get_رقم_Of_Threads();

    تهی set_رقم_Of_Threads(unsigned n);

    ThreadPool();

    ~ThreadPool();

    unsigned رقم_threads;

    pthread_mutex_t mutex;  // guards fields (job/threads) from non-worker threads (concurrent parallel_for calls)
#اگر defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
    pthread_cond_t cond_thread_wake;
#endif

    pthread_mutex_t mutex_notify;
    pthread_cond_t cond_thread_task_complete;

    std::vector< Ptr<WorkerThread> > threads;

    Ptr<ParallelJob> job;

#ifdef CV_PROFILE_THREADS
    دوگانه tickFreq;
    int64 jobSubmitTime;
    ساختار ThreadStatistics
    {
        ThreadStatistics() : threadWait(0)
        {
            reset();
        }
        تهی reset()
        {
            threadWake = 0;
            threadExecuteStart = 0;
            threadExecuteStop = 0;
            executedTasks = 0;
            keepActive = نادرست;
            threadPing = get_Tick_شمار();
        }
        int64 threadWait; // don't reset by default
        int64 threadPing; // don't reset by default
        int64 threadWake;
        int64 threadExecuteStart;
        int64 threadExecuteStop;
        int64 threadFree;
        unsigned executedTasks;
        منطقی keepActive;

        int64 dummy_[8]; // separate cache lines

        تهی dump(صحیح id, int64 baseTime, دوگانه tickFreq)
        {
            اگر (id < 0)
                std::cout << "Main: ";
            وگرنه
                printf("T%03d: ", id + 2);
            printf("wait=% 10.1f   ping=% 6.1f",
                    threadWait > 0 ? (threadWait - baseTime) / tickFreq * 1e6 : -0.0,
                    threadPing > 0 ? (threadPing - baseTime) / tickFreq * 1e6 : -0.0);
            اگر (threadWake > 0)
                printf("   wake=% 6.1f",
                    (threadWake > 0 ? (threadWake - baseTime) / tickFreq * 1e6 : -0.0));
            اگر (threadExecuteStart > 0)
            {
                printf("   exec=% 6.1f - % 6.1f   tasksDone=%5u   free=% 6.1f",
                    (threadExecuteStart > 0 ? (threadExecuteStart - baseTime) / tickFreq * 1e6 : -0.0),
                    (threadExecuteStop > 0 ? (threadExecuteStop - baseTime) / tickFreq * 1e6 : -0.0),
                    executedTasks,
                    (threadFree > 0 ? (threadFree - baseTime) / tickFreq * 1e6 : -0.0));
                اگر (id >= 0)
                    printf(" active=%s\n", keepActive ? "true" : "false");
                وگرنه
                    printf("\n");
            }
            وگرنه
                printf("   ------------------------------------------------------------------------------\n");
        }
    };
    ThreadStatistics threads_stat[CV_PROFILE_THREADS]; // 0 - main thread, 1..N - worker threads
#endif

};

رده WorkerThread
{
عمومی:
    ThreadPool& thread_pool;
    ثابت unsigned id;
    pthread_t posix_thread;
    منطقی is_created;

    std::atomic<منطقی> stop_thread;

    std::atomic<منطقی> has_wake_signal;

    Ptr<ParallelJob> job;

    pthread_mutex_t mutex;
#اگر !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
    فرار منطقی isActive;
    pthread_cond_t cond_thread_wake;
#endif

    WorkerThread(ThreadPool& thread_pool_, unsigned id_) :
        thread_pool(thread_pool_),
        id(id_),
        posix_thread(0),
        is_created(نادرست),
        stop_thread(نادرست),
        has_wake_signal(نادرست)
#اگر !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
        , isActive(درست)
#endif
    {
        CV_LOG_VERBOSE(پوچ, 1, "MainThread: initializing new worker: " << id);
        صحیح res = pthread_mutex_init(&mutex, پوچ);
        اگر (res != 0)
        {
            CV_LOG_ERROR(پوچ, id << ": Can't create thread mutex: res = " << res);
            بازگشت;
        }
#اگر !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
        res = pthread_cond_init(&cond_thread_wake, پوچ);
        اگر (res != 0)
        {
            CV_LOG_ERROR(پوچ, id << ": Can't create thread condition variable: res = " << res);
            بازگشت;
        }
#endif
        res = pthread_create(&posix_thread, پوچ, thread_loop_wrapper, (تهی*)این);
        اگر (res != 0)
        {
            CV_LOG_ERROR(پوچ, id << ": Can't spawn new thread: res = " << res);
        }
        وگرنه
        {
            is_created = درست;
        }
    }

    ~WorkerThread()
    {
        CV_LOG_VERBOSE(پوچ, 1, "MainThread: destroy worker thread: " << id);
        اگر (is_created)
        {
            اگر (!stop_thread)
            {
                pthread_mutex_lock(&mutex);  // to avoid signal miss due pre-check
                stop_thread = درست;
                pthread_mutex_unlock(&mutex);
#اگر defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
                pthread_cond_broadcast(&thread_pool.cond_thread_wake);
#وگرنه
                pthread_cond_signal(&cond_thread_wake);
#endif
            }
            pthread_join(posix_thread, پوچ);
        }
#اگر !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
        pthread_cond_destroy(&cond_thread_wake);
#endif
        pthread_mutex_destroy(&mutex);
    }

    تهی thread_body();
    ایستا تهی* thread_loop_wrapper(تهی* thread_object)
    {
#ifdef OPENCV_WITH_ITT
        __itt_thread_set_name(cv::format("OpenCVThread-%03d", cv::utils::getThreadID()).c_str());
#endif
        ((WorkerThread*)thread_object)->thread_body();
        بازگشت 0;
    }
};

رده ParallelJob
{
عمومی:
    ParallelJob(ثابت ThreadPool& thread_pool_, ثابت Range& range_, ثابت ParallelLoopBody& body_, صحیح nstripes_) :
        thread_pool(thread_pool_),
        body(body_),
        range(range_),
        nstripes((unsigned)nstripes_),
        is_completed(نادرست)
    {
        CV_LOG_VERBOSE(پوچ, 5, "ParallelJob::ParallelJob(" << (تهی*)این << ")");
        current_task.store(0, std::memory_order_relaxed);
        active_thread_شمار.store(0, std::memory_order_relaxed);
        completed_thread_شمار.store(0, std::memory_order_relaxed);
        dummy0_[0] = 0, dummy1_[0] = 0, dummy2_[0] = 0; // compiler warning
    }

    ~ParallelJob()
    {
        CV_LOG_VERBOSE(پوچ, 5, "ParallelJob::~ParallelJob(" << (تهی*)این << ")");
    }

    unsigned execute(منطقی is_worker_thread)
    {
        unsigned executed_tasks = 0;
        ثابت صحیح task_شمار = range.size();
        ثابت صحیح remaining_multiplier = std::min(nstripes,
                std::max(
                        std::min(100u, thread_pool.رقم_threads * 4),
                        thread_pool.رقم_threads * 2
                ));  // experimental value
        برای (;;)
        {
            صحیح chunk_size = std::max(1, (task_شمار - current_task) / remaining_multiplier);
            صحیح id = current_task.fetch_add(chunk_size, std::memory_order_seq_cst);
            اگر (id >= task_شمار)
                بشکن; // no more free tasks

            executed_tasks += chunk_size;
            صحیح start_id = id;
            صحیح end_id = std::min(task_شمار, id + chunk_size);
            CV_LOG_VERBOSE(پوچ, 9, "Thread: job " << start_id << "-" << end_id);

            //TODO: if (not pending exception)
            {
                body.operator()(Range(range.start + start_id, range.start + end_id));
            }
            اگر (is_worker_thread && is_completed)
            {
                CV_LOG_ERROR(پوچ, "\t\t\t\tBUG! Job: " << (تهی*)این << " " << id << " " << active_thread_شمار << " " << completed_thread_شمار);
                CV_Assert(!is_completed); // TODO Dbg this
            }
        }
        بازگشت executed_tasks;
    }

    ثابت ThreadPool& thread_pool;
    ثابت ParallelLoopBody& body;
    ثابت Range range;
    ثابت unsigned nstripes;

    std::atomic<صحیح> current_task;  // next free part of job
    int64 dummy0_[8];  // avoid cache-line reusing for the same atomics

    std::atomic<صحیح> active_thread_شمار;  // number of threads worked on this job
    int64 dummy1_[8];  // avoid cache-line reusing for the same atomics

    std::atomic<صحیح> completed_thread_شمار;  // number of threads completed any activities on this job
    int64 dummy2_[8];  // avoid cache-line reusing for the same atomics

    std::atomic<منطقی> is_completed;

    // TODO exception handling
};


// Disable thread sanitization check when CV_USE_GLOBAL_WORKERS_COND_VAR is not
// set because it triggers as the main thread reads isActive while the children
// thread writes it (but it all works out because a mutex is locked in the main
// thread and isActive re-checked).
// This is to solve issue #19463.
#اگر !defined(CV_USE_GLOBAL_WORKERS_COND_VAR) && defined(__clang__) && defined(__has_feature)
#اگر __has_feature(thread_sanitizer)
__attribute__((no_sanitize("thread")))
#endif
#endif
تهی WorkerThread::thread_body()
{
    (تهی)cv::utils::getThreadID(); // notify OpenCV about new thread
    CV_LOG_VERBOSE(پوچ, 5, "Thread: new thread: " << id);

    منطقی allow_active_wait = درست;

#ifdef CV_PROFILE_THREADS
    ThreadPool::ThreadStatistics& stat = thread_pool.threads_stat[id + 1];
#endif

    وقتی (!stop_thread)
    {
        CV_LOG_VERBOSE(پوچ, 5, "Thread: ... loop iteration: allow_active_wait=" << allow_active_wait << "   has_wake_signal=" << has_wake_signal);
        اگر (allow_active_wait && CV_WORKER_ACTIVE_WAIT > 0)
        {
            allow_active_wait = نادرست;
            برای (صحیح i = 0; i < CV_WORKER_ACTIVE_WAIT; i++)
            {
                اگر (has_wake_signal)
                    بشکن;
                اگر (CV_ACTIVE_WAIT_PAUSE_LIMIT > 0 && (i < CV_ACTIVE_WAIT_PAUSE_LIMIT || (i & 1)))
                    CV_PAUSE(16);
                وگرنه
                    CV_YIELD();
            }
        }
        pthread_mutex_lock(&mutex);
#ifdef CV_PROFILE_THREADS
        stat.threadWait = get_Tick_شمار();
#endif
        وقتی (!has_wake_signal) // to handle spurious wakeups
        {
            //CV_LOG_VERBOSE(NULL, 5, "Thread: wait (sleep) ...");
#اگر defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
            pthread_cond_wait(&thread_pool.cond_thread_wake, &mutex);
#وگرنه
            isActive = نادرست;
            pthread_cond_wait(&cond_thread_wake, &mutex);
            isActive = درست;
#endif
            CV_LOG_VERBOSE(پوچ, 5, "Thread: wake ... (has_wake_signal=" << has_wake_signal << " stop_thread=" << stop_thread << ")")
        }
#ifdef CV_PROFILE_THREADS
        stat.threadWake = get_Tick_شمار();
#endif

        CV_LOG_VERBOSE(پوچ, 5, "Thread: checking for new job");
        اگر (CV_WORKER_ACTIVE_WAIT_THREADS_LIMIT == 0)
            allow_active_wait = درست;
        Ptr<ParallelJob> j_ptr; swap(j_ptr, job);
        has_wake_signal = نادرست;
        pthread_mutex_unlock(&mutex);

        اگر (!stop_thread)
        {
            ParallelJob* j = j_ptr;
            اگر (j)
            {
                CV_LOG_VERBOSE(پوچ, 5, "Thread: job size=" << j->range.size() << " done=" << j->current_task);
                اگر (j->current_task < j->range.size())
                {
                    صحیح other = j->active_thread_شمار.fetch_add(1, std::memory_order_seq_cst);
                    CV_LOG_VERBOSE(پوچ, 5, "Thread: processing new job (with " << other << " other threads)"); CV_UNUSED(other);
#ifdef CV_PROFILE_THREADS
                    stat.threadExecuteStart = get_Tick_شمار();
                    stat.executedTasks = j->execute(درست);
                    stat.threadExecuteStop = get_Tick_شمار();
#وگرنه
                    j->execute(درست);
#endif
                    صحیح completed = j->completed_thread_شمار.fetch_add(1, std::memory_order_seq_cst) + 1;
                    صحیح active = j->active_thread_شمار.load(std::memory_order_acquire);
                    اگر (CV_WORKER_ACTIVE_WAIT_THREADS_LIMIT > 0)
                    {
                        allow_active_wait = درست;
                        اگر (active >= CV_WORKER_ACTIVE_WAIT_THREADS_LIMIT && (id & 1) == 0) // turn off a half of threads
                            allow_active_wait = نادرست;
                    }
                    CV_LOG_VERBOSE(پوچ, 5, "Thread: completed job processing: " << active << " " << completed);
                    اگر (active == completed)
                    {
                        منطقی need_signal = !j->is_completed;
                        j->is_completed = درست;
                        j = پوچ; j_ptr.release();
                        اگر (need_signal)
                        {
                            CV_LOG_VERBOSE(پوچ, 5, "Thread: job finished => notifying the main thread");
                            pthread_mutex_lock(&thread_pool.mutex_notify);  // to avoid signal miss due pre-check condition
                            // empty
                            pthread_mutex_unlock(&thread_pool.mutex_notify);
                            pthread_cond_broadcast/*pthread_cond_signal*/(&thread_pool.cond_thread_task_complete);
                        }
                    }
                }
                وگرنه
                {
                    CV_LOG_VERBOSE(پوچ, 5, "Thread: no free job tasks");
                }
            }
        }
#ifdef CV_PROFILE_THREADS
        stat.threadFree = get_Tick_شمار();
        stat.keepActive = allow_active_wait;
#endif
    }
}

ThreadPool::ThreadPool()
{
#ifdef CV_PROFILE_THREADS
    tickFreq = getTickFrequency();
#endif

    صحیح res = 0;
    res |= pthread_mutex_init(&mutex, پوچ);
    res |= pthread_mutex_init(&mutex_notify, پوچ);
#اگر defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
    res |= pthread_cond_init(&cond_thread_wake, پوچ);
#endif
    res |= pthread_cond_init(&cond_thread_task_complete, پوچ);

    اگر (0 != res)
    {
        CV_LOG_FATAL(پوچ, "Failed to initialize ThreadPool (pthreads)");
    }
    رقم_threads = default_عدد_Of_Threads();
}

منطقی ThreadPool::reconfigure_(unsigned new_threads_شمار)
{
    اگر (new_threads_شمار == threads.size())
        بازگشت نادرست;

    اگر (new_threads_شمار < threads.size())
    {
        CV_LOG_VERBOSE(پوچ, 1, "MainThread: reduce worker pool: " << threads.size() << " => " << new_threads_شمار);
        std::vector< Ptr<WorkerThread> > release_threads(threads.size() - new_threads_شمار);
        برای (size_t i = new_threads_شمار; i < threads.size(); ++i)
        {
            pthread_mutex_lock(&threads[i]->mutex);  // to avoid signal miss due pre-check
            threads[i]->stop_thread = درست;
            threads[i]->has_wake_signal = درست;
#اگر !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
            pthread_mutex_unlock(&threads[i]->mutex);
            pthread_cond_broadcast/*pthread_cond_signal*/(&threads[i]->cond_thread_wake); // wake thread
#وگرنه
            pthread_mutex_unlock(&threads[i]->mutex);
#endif
            std::swap(threads[i], release_threads[i - new_threads_شمار]);
        }
#اگر defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
        CV_LOG_VERBOSE(پوچ, 1, "MainThread: notify worker threads about termination...");
        pthread_cond_broadcast(&cond_thread_wake); // wake all threads
#endif
        threads.resize(new_threads_شمار);
        release_threads.clear();  // calls thread_join which want to lock mutex
        بازگشت نادرست;
    }
    وگرنه
    {
        CV_LOG_VERBOSE(پوچ, 1, "MainThread: upgrade worker pool: " << threads.size() << " => " << new_threads_شمار);
        برای (size_t i = threads.size(); i < new_threads_شمار; ++i)
        {
            threads.push_back(Ptr<WorkerThread>(جدید WorkerThread(*این, (unsigned)i))); // spawn more threads
        }
    }
    بازگشت نادرست;
}

ThreadPool::~ThreadPool()
{
    reconfigure(0);
    pthread_cond_destroy(&cond_thread_task_complete);
#اگر defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
    pthread_cond_destroy(&cond_thread_wake);
#endif
    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&mutex_notify);
}

تهی ThreadPool::run(ثابت Range& range, ثابت ParallelLoopBody& body, دوگانه nstripes)
{
    CV_LOG_VERBOSE(پوچ, 1, "MainThread: new parallel job: num_threads=" << رقم_threads << "   range=" << range.size() << "   nstripes=" << nstripes << "   job=" << (تهی*)job);
#ifdef CV_PROFILE_THREADS
    jobSubmitTime = get_Tick_شمار();
    threads_stat[0].reset();
    threads_stat[0].threadWait = jobSubmitTime;
    threads_stat[0].threadWake = jobSubmitTime;
#endif
    اگر (get_رقم_Of_Threads() > 1 &&
        job == پوچ &&
        (range.size() * nstripes >= 2 || (range.size() > 1 && nstripes <= 0))
    )
    {
        pthread_mutex_lock(&mutex);
        اگر (job != پوچ)
        {
            pthread_mutex_unlock(&mutex);
            body(range);
            بازگشت;
        }
        reconfigure_(رقم_threads - 1);

        {
            CV_LOG_VERBOSE(پوچ, 1, "MainThread: initialize parallel job: " << range.size());
            job = Ptr<ParallelJob>(جدید ParallelJob(*این, range, body, nstripes));
            pthread_mutex_unlock(&mutex);

            CV_LOG_VERBOSE(پوچ, 5, "MainThread: wake worker threads...");
            size_t رقم_threads_to_wake = std::min(static_cast<size_t>(range.size()), threads.size());
            برای (size_t i = 0; i < رقم_threads_to_wake; ++i)
            {
                اگر (job->current_task >= job->range.size())
                    بشکن;
                WorkerThread& thread = *(threads[i].get());
                اگر (
#اگر defined(__clang__) && defined(__has_feature)
#اگر __has_feature(thread_sanitizer)
                        1 ||  // Robust workaround to avoid data race warning of `thread.job`
#endif
#endif
#اگر !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
                        thread.isActive ||
#endif
                        thread.has_wake_signal
                        || !thread.job.empty()  // #10881
                )
                {
                    pthread_mutex_lock(&thread.mutex);
                    thread.job = job;
#اگر !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
                    منطقی isActive = thread.isActive;
#endif
                    thread.has_wake_signal = درست;
#ifdef CV_PROFILE_THREADS
                    threads_stat[i + 1].reset();
#endif
                    pthread_mutex_unlock(&thread.mutex);
#اگر !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
                    اگر (!isActive)
                    {
                        pthread_cond_broadcast/*pthread_cond_signal*/(&thread.cond_thread_wake); // wake thread
                    }
#endif
                }
                وگرنه
                {
                    CV_Assert(thread.job.empty());
                    thread.job = job;
                    thread.has_wake_signal = درست;
#ifdef CV_PROFILE_THREADS
                    threads_stat[i + 1].reset();
#endif
#اگر !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
                    pthread_cond_broadcast/*pthread_cond_signal*/(&thread.cond_thread_wake); // wake thread
#endif
                }
            }
#ifdef CV_PROFILE_THREADS
            threads_stat[0].threadPing = get_Tick_شمار();
#endif
#اگر defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
            pthread_cond_broadcast(&cond_thread_wake); // wake all threads
#endif
#ifdef CV_PROFILE_THREADS
            threads_stat[0].threadWake = get_Tick_شمار();
#endif
            CV_LOG_VERBOSE(پوچ, 5, "MainThread: wake worker threads... (done)");

            {
                ParallelJob& j = *(این->job);
#ifdef CV_PROFILE_THREADS
                threads_stat[0].threadExecuteStart = get_Tick_شمار();
                threads_stat[0].executedTasks = j.execute(نادرست);
                threads_stat[0].threadExecuteStop = get_Tick_شمار();
#وگرنه
                j.execute(نادرست);
#endif
                CV_Assert(j.current_task >= j.range.size());
                CV_LOG_VERBOSE(پوچ, 5, "MainThread: complete self-tasks: " << j.active_thread_شمار << " " << j.completed_thread_شمار);
                اگر (job->is_completed || j.active_thread_شمار == 0)
                {
                    job->is_completed = درست;
                    CV_LOG_VERBOSE(پوچ, 5, "MainThread: no WIP worker threads");
                }
                وگرنه
                {
                    اگر (CV_MAIN_THREAD_ACTIVE_WAIT > 0)
                    {
                        برای (صحیح i = 0; i < CV_MAIN_THREAD_ACTIVE_WAIT; i++)  // don't spin too much in any case (inaccurate getTickCount())
                        {
                            اگر (job->is_completed)
                            {
                                CV_LOG_VERBOSE(پوچ, 5, "MainThread: job finalize (active wait) " << j.active_thread_شمار << " " << j.completed_thread_شمار);
                                بشکن;
                            }
                            اگر (CV_ACTIVE_WAIT_PAUSE_LIMIT > 0 && (i < CV_ACTIVE_WAIT_PAUSE_LIMIT || (i & 1)))
                                CV_PAUSE(16);
                            وگرنه
                                CV_YIELD();
                        }
                    }
                    اگر (!job->is_completed)
                    {
                        CV_LOG_VERBOSE(پوچ, 5, "MainThread: prepare wait " << j.active_thread_شمار << " " << j.completed_thread_شمار);
                        pthread_mutex_lock(&mutex_notify);
                        برای (;;)
                        {
                            اگر (job->is_completed)
                            {
                                CV_LOG_VERBOSE(پوچ, 5, "MainThread: job finalize (wait) " << j.active_thread_شمار << " " << j.completed_thread_شمار);
                                بشکن;
                            }
                            CV_LOG_VERBOSE(پوچ, 5, "MainThread: wait completion (sleep) ...");
                            pthread_cond_wait(&cond_thread_task_complete, &mutex_notify);
                            CV_LOG_VERBOSE(پوچ, 5, "MainThread: wake");
                        }
                        pthread_mutex_unlock(&mutex_notify);
                    }
                }
            }
#ifdef CV_PROFILE_THREADS
            threads_stat[0].threadFree = get_Tick_شمار();
            std::cout << "Job: sz=" << range.size() << " nstripes=" << nstripes << "    Time: " << (threads_stat[0].threadFree - jobSubmitTime) / tickFreq * 1e6 << " usec" << std::endl;
            برای (صحیح i = 0; i < (صحیح)threads.size() + 1; i++)
            {
                threads_stat[i].dump(i - 1, jobSubmitTime, tickFreq);
            }
#endif
            اگر (job)
            {
                pthread_mutex_lock(&mutex);
                CV_LOG_VERBOSE(پوچ, 5, "MainThread: job release");
                CV_Assert(job->is_completed);
                job.release();
                pthread_mutex_unlock(&mutex);
            }
        }
    }
    وگرنه
    {
        body(range);
    }
}

size_t ThreadPool::get_رقم_Of_Threads()
{
    بازگشت رقم_threads;
}

تهی ThreadPool::set_رقم_Of_Threads(unsigned n)
{
    اگر (n != رقم_threads)
    {
        رقم_threads = n;
        اگر (n == 1)
           اگر (job == پوچ) reconfigure(0);  // stop worker threads immediately
    }
}

size_t parallel_pthreads_get_threads_رقم()
{
    بازگشت ThreadPool::instance().get_رقم_Of_Threads();
}

تهی parallel_pthreads_set_threads_رقم(صحیح num)
{
    اگر(num < 0)
    {
        ThreadPool::instance().set_رقم_Of_Threads(0);
    }
    وگرنه
    {
        ThreadPool::instance().set_رقم_Of_Threads(unsigned(num));
    }
}

تهی parallel_for_pthreads(ثابت Range& range, ثابت ParallelLoopBody& body, دوگانه nstripes)
{
    ThreadPool::instance().run(range, body, nstripes);
}

}

#endif
