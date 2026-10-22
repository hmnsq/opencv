// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#অন্তর্ভাব "precomp.hpp"

#অন্তর্ভাব "parallel_impl.hpp"

#ifdef HAVE_PTHREADS_PF
#অন্তর্ভাব <pthread.h>

#অন্তর্ভাব <opencv2/core/utils/configuration.private.hpp>

#অন্তর্ভাব <opencv2/core/utils/logger.defines.hpp>
//#undef CV_LOG_STRIP_LEVEL
//#define CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_VERBOSE + 1
#অন্তর্ভাব <opencv2/core/utils/logger.hpp>

#অন্তর্ভাব <opencv2/core/utils/trace.private.hpp>

//#define CV_PROFILE_THREADS 64
//#define getTickCount getCPUTickCount  // use this if getTickCount() calls are expensive (and getCPUTickCount() is accurate)

//#define CV_USE_GLOBAL_WORKERS_COND_VAR  // not effective on many-core systems (10+)

#অন্তর্ভাব <atomic>

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
# যদি defined __GNUC__ && (defined __i386__ || defined __x86_64__)
#   include <x86intrin.h> /* for __rdtsc */
#   যদি !defined(__SSE2__)
      স্থির অন্তঃরেখ শূন্য cv_non_sse_mm_pause() { __asm__ __volatile__ ("rep; nop"); }
#     define _mm_pause cv_non_sse_mm_pause
#   endif
// With Skylake CPUs and above, _mm_pause takes 140 cycles so no need for a loop.
#   define CV_PAUSE(v) করো { (শূন্য)v; _mm_pause(); } যখন (0)
# elif defined __GNUC__ && defined __aarch64__
#   define CV_PAUSE(v) করো { জন্য (পূর্ণসংখ্যা __delay = (v); __delay > 0; --__delay) { asm অস্থির("yield" ::: "memory"); } } যখন (0)
# elif defined __GNUC__ && defined __arm__
#   define CV_PAUSE(v) করো { জন্য (পূর্ণসংখ্যা __delay = (v); __delay > 0; --__delay) { asm অস্থির("" ::: "memory"); } } যখন (0)
# elif defined __GNUC__ && defined __mips__ && __mips_isa_rev >= 2
#   define CV_PAUSE(v) করো { জন্য (পূর্ণসংখ্যা __delay = (v); __delay > 0; --__delay) { asm অস্থির("pause" ::: "memory"); } } যখন (0)
# elif defined __APPLE__ && defined __POWERPC__
#   define CV_PAUSE(v) করো { জন্য (পূর্ণসংখ্যা __delay = (v); __delay > 0; --__delay) { asm অস্থির("or r27,r27,r27" ::: "memory"); } } যখন (0)
# elif defined __GNUC__ && defined __PPC64__
#   define CV_PAUSE(v) করো { জন্য (পূর্ণসংখ্যা __delay = (v); __delay > 0; --__delay) { asm অস্থির("or 27,27,27" ::: "memory"); } } যখন (0)
# elif defined __GNUC__ && defined __riscv
// PAUSE HINT is not part of RISC-V ISA yet, but is under discussion now. For details see:
// https://github.com/riscv/riscv-isa-manual/pull/398
// https://github.com/riscv/riscv-isa-manual/issues/43
// #   define CV_PAUSE(v) do { for (int __delay = (v); __delay > 0; --__delay) { asm volatile("pause"); } } while (0)
#   define CV_PAUSE(v) করো { জন্য (পূর্ণসংখ্যা __delay = (v); __delay > 0; --__delay) { asm অস্থির("nop"); } } যখন (0)
# elif defined __GNUC__ && defined __loongarch__
#   define CV_PAUSE(v) করো { জন্য (পূর্ণসংখ্যা __delay = (v); __delay > 0; --__delay) { asm অস্থির("nop"); } } যখন (0)
# অন্যথা
#   warning "Can't detect 'pause' (CPU-yield) instruction on the target platform. Specify CV_PAUSE() definition via compiler flags."
#   define CV_PAUSE(...) করো { /* no-op: works, but not effective */ } যখন (0)
# endif
#endif // CV_PAUSE


নামাকাশ cv
{

স্থির পূর্ণসংখ্যা CV_ACTIVE_WAIT_PAUSE_LIMIT = (পূর্ণসংখ্যা)utils::get_Configuration_Parameter_আকার_T("OPENCV_THREAD_POOL_ACTIVE_WAIT_PAUSE_LIMIT", 16);  // iterations
স্থির পূর্ণসংখ্যা CV_WORKER_ACTIVE_WAIT = (পূর্ণসংখ্যা)utils::get_Configuration_Parameter_আকার_T("OPENCV_THREAD_POOL_ACTIVE_WAIT_WORKER", 2000);  // iterations
স্থির পূর্ণসংখ্যা CV_MAIN_THREAD_ACTIVE_WAIT = (পূর্ণসংখ্যা)utils::get_Configuration_Parameter_আকার_T("OPENCV_THREAD_POOL_ACTIVE_WAIT_MAIN", 10000); // iterations

স্থির পূর্ণসংখ্যা CV_WORKER_ACTIVE_WAIT_THREADS_LIMIT = (পূর্ণসংখ্যা)utils::get_Configuration_Parameter_আকার_T("OPENCV_THREAD_POOL_ACTIVE_WAIT_THREADS_LIMIT", 0); // number of real cores

শ্রেণী WorkerThread;
শ্রেণী ParallelJob;

শ্রেণী ThreadPool
{
সর্বজনীন:
    স্থির ThreadPool& instance()
    {
        CV_SINGLETON_LAZY_INIT_REF(ThreadPool, নতুন ThreadPool())
    }

    স্থির শূন্য stop()
    {
        ThreadPool& manager = instance();
        manager.reconfigure(0);
    }

    শূন্য reconfigure(unsigned new_threads_গুনতি)
    {
        যদি (new_threads_গুনতি == threads.size())
            ফেরত;
        pthread_mutex_lock(&mutex);
        reconfigure_(new_threads_গুনতি);
        pthread_mutex_unlock(&mutex);
    }
    যৌক্তিক reconfigure_(unsigned new_threads_গুনতি); // internal implementation

    শূন্য run(ধ্রুবক Range& range, ধ্রুবক ParallelLoopBody& body, দ্বিগুণ nstripes);

    size_t get_অঙ্ক_Of_Threads();

    শূন্য set_অঙ্ক_Of_Threads(unsigned n);

    ThreadPool();

    ~ThreadPool();

    unsigned অঙ্ক_threads;

    pthread_mutex_t mutex;  // guards fields (job/threads) from non-worker threads (concurrent parallel_for calls)
#যদি defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
    pthread_cond_t cond_thread_wake;
#endif

    pthread_mutex_t mutex_notify;
    pthread_cond_t cond_thread_task_complete;

    std::vector< Ptr<WorkerThread> > threads;

    Ptr<ParallelJob> job;

#ifdef CV_PROFILE_THREADS
    দ্বিগুণ tickFreq;
    int64 jobSubmitTime;
    কাঠামো ThreadStatistics
    {
        ThreadStatistics() : threadWait(0)
        {
            reset();
        }
        শূন্য reset()
        {
            threadWake = 0;
            threadExecuteStart = 0;
            threadExecuteStop = 0;
            executedTasks = 0;
            keepActive = মিথ্যা;
            threadPing = get_Tick_গুনতি();
        }
        int64 threadWait; // don't reset by default
        int64 threadPing; // don't reset by default
        int64 threadWake;
        int64 threadExecuteStart;
        int64 threadExecuteStop;
        int64 threadFree;
        unsigned executedTasks;
        যৌক্তিক keepActive;

        int64 dummy_[8]; // separate cache lines

        শূন্য dump(পূর্ণসংখ্যা id, int64 baseTime, দ্বিগুণ tickFreq)
        {
            যদি (id < 0)
                std::cout << "Main: ";
            অন্যথা
                printf("T%03d: ", id + 2);
            printf("wait=% 10.1f   ping=% 6.1f",
                    threadWait > 0 ? (threadWait - baseTime) / tickFreq * 1e6 : -0.0,
                    threadPing > 0 ? (threadPing - baseTime) / tickFreq * 1e6 : -0.0);
            যদি (threadWake > 0)
                printf("   wake=% 6.1f",
                    (threadWake > 0 ? (threadWake - baseTime) / tickFreq * 1e6 : -0.0));
            যদি (threadExecuteStart > 0)
            {
                printf("   exec=% 6.1f - % 6.1f   tasksDone=%5u   free=% 6.1f",
                    (threadExecuteStart > 0 ? (threadExecuteStart - baseTime) / tickFreq * 1e6 : -0.0),
                    (threadExecuteStop > 0 ? (threadExecuteStop - baseTime) / tickFreq * 1e6 : -0.0),
                    executedTasks,
                    (threadFree > 0 ? (threadFree - baseTime) / tickFreq * 1e6 : -0.0));
                যদি (id >= 0)
                    printf(" active=%s\n", keepActive ? "true" : "false");
                অন্যথা
                    printf("\n");
            }
            অন্যথা
                printf("   ------------------------------------------------------------------------------\n");
        }
    };
    ThreadStatistics threads_stat[CV_PROFILE_THREADS]; // 0 - main thread, 1..N - worker threads
#endif

};

শ্রেণী WorkerThread
{
সর্বজনীন:
    ThreadPool& thread_pool;
    ধ্রুবক unsigned id;
    pthread_t posix_thread;
    যৌক্তিক is_created;

    std::atomic<যৌক্তিক> stop_thread;

    std::atomic<যৌক্তিক> has_wake_signal;

    Ptr<ParallelJob> job;

    pthread_mutex_t mutex;
#যদি !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
    অস্থির যৌক্তিক isActive;
    pthread_cond_t cond_thread_wake;
#endif

    WorkerThread(ThreadPool& thread_pool_, unsigned id_) :
        thread_pool(thread_pool_),
        id(id_),
        posix_thread(0),
        is_created(মিথ্যা),
        stop_thread(মিথ্যা),
        has_wake_signal(মিথ্যা)
#যদি !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
        , isActive(সত্য)
#endif
    {
        CV_LOG_VERBOSE(ফাঁকা, 1, "MainThread: initializing new worker: " << id);
        পূর্ণসংখ্যা res = pthread_mutex_init(&mutex, ফাঁকা);
        যদি (res != 0)
        {
            CV_LOG_ERROR(ফাঁকা, id << ": Can't create thread mutex: res = " << res);
            ফেরত;
        }
#যদি !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
        res = pthread_cond_init(&cond_thread_wake, ফাঁকা);
        যদি (res != 0)
        {
            CV_LOG_ERROR(ফাঁকা, id << ": Can't create thread condition variable: res = " << res);
            ফেরত;
        }
#endif
        res = pthread_create(&posix_thread, ফাঁকা, thread_loop_wrapper, (শূন্য*)এই);
        যদি (res != 0)
        {
            CV_LOG_ERROR(ফাঁকা, id << ": Can't spawn new thread: res = " << res);
        }
        অন্যথা
        {
            is_created = সত্য;
        }
    }

    ~WorkerThread()
    {
        CV_LOG_VERBOSE(ফাঁকা, 1, "MainThread: destroy worker thread: " << id);
        যদি (is_created)
        {
            যদি (!stop_thread)
            {
                pthread_mutex_lock(&mutex);  // to avoid signal miss due pre-check
                stop_thread = সত্য;
                pthread_mutex_unlock(&mutex);
#যদি defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
                pthread_cond_broadcast(&thread_pool.cond_thread_wake);
#অন্যথা
                pthread_cond_signal(&cond_thread_wake);
#endif
            }
            pthread_join(posix_thread, ফাঁকা);
        }
#যদি !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
        pthread_cond_destroy(&cond_thread_wake);
#endif
        pthread_mutex_destroy(&mutex);
    }

    শূন্য thread_body();
    স্থির শূন্য* thread_loop_wrapper(শূন্য* thread_object)
    {
#ifdef OPENCV_WITH_ITT
        __itt_thread_set_name(cv::format("OpenCVThread-%03d", cv::utils::getThreadID()).c_str());
#endif
        ((WorkerThread*)thread_object)->thread_body();
        ফেরত 0;
    }
};

শ্রেণী ParallelJob
{
সর্বজনীন:
    ParallelJob(ধ্রুবক ThreadPool& thread_pool_, ধ্রুবক Range& range_, ধ্রুবক ParallelLoopBody& body_, পূর্ণসংখ্যা nstripes_) :
        thread_pool(thread_pool_),
        body(body_),
        range(range_),
        nstripes((unsigned)nstripes_),
        is_completed(মিথ্যা)
    {
        CV_LOG_VERBOSE(ফাঁকা, 5, "ParallelJob::ParallelJob(" << (শূন্য*)এই << ")");
        current_task.store(0, std::memory_order_relaxed);
        active_thread_গুনতি.store(0, std::memory_order_relaxed);
        completed_thread_গুনতি.store(0, std::memory_order_relaxed);
        dummy0_[0] = 0, dummy1_[0] = 0, dummy2_[0] = 0; // compiler warning
    }

    ~ParallelJob()
    {
        CV_LOG_VERBOSE(ফাঁকা, 5, "ParallelJob::~ParallelJob(" << (শূন্য*)এই << ")");
    }

    unsigned execute(যৌক্তিক is_worker_thread)
    {
        unsigned executed_tasks = 0;
        ধ্রুবক পূর্ণসংখ্যা task_গুনতি = range.size();
        ধ্রুবক পূর্ণসংখ্যা remaining_multiplier = std::min(nstripes,
                std::max(
                        std::min(100u, thread_pool.অঙ্ক_threads * 4),
                        thread_pool.অঙ্ক_threads * 2
                ));  // experimental value
        জন্য (;;)
        {
            পূর্ণসংখ্যা chunk_আকার = std::max(1, (task_গুনতি - current_task) / remaining_multiplier);
            পূর্ণসংখ্যা id = current_task.fetch_add(chunk_আকার, std::memory_order_seq_cst);
            যদি (id >= task_গুনতি)
                বিরতি; // no more free tasks

            executed_tasks += chunk_আকার;
            পূর্ণসংখ্যা start_id = id;
            পূর্ণসংখ্যা end_id = std::min(task_গুনতি, id + chunk_আকার);
            CV_LOG_VERBOSE(ফাঁকা, 9, "Thread: job " << start_id << "-" << end_id);

            //TODO: if (not pending exception)
            {
                body.operator()(Range(range.start + start_id, range.start + end_id));
            }
            যদি (is_worker_thread && is_completed)
            {
                CV_LOG_ERROR(ফাঁকা, "\t\t\t\tBUG! Job: " << (শূন্য*)এই << " " << id << " " << active_thread_গুনতি << " " << completed_thread_গুনতি);
                CV_Assert(!is_completed); // TODO Dbg this
            }
        }
        ফেরত executed_tasks;
    }

    ধ্রুবক ThreadPool& thread_pool;
    ধ্রুবক ParallelLoopBody& body;
    ধ্রুবক Range range;
    ধ্রুবক unsigned nstripes;

    std::atomic<পূর্ণসংখ্যা> current_task;  // next free part of job
    int64 dummy0_[8];  // avoid cache-line reusing for the same atomics

    std::atomic<পূর্ণসংখ্যা> active_thread_গুনতি;  // number of threads worked on this job
    int64 dummy1_[8];  // avoid cache-line reusing for the same atomics

    std::atomic<পূর্ণসংখ্যা> completed_thread_গুনতি;  // number of threads completed any activities on this job
    int64 dummy2_[8];  // avoid cache-line reusing for the same atomics

    std::atomic<যৌক্তিক> is_completed;

    // TODO exception handling
};


// Disable thread sanitization check when CV_USE_GLOBAL_WORKERS_COND_VAR is not
// set because it triggers as the main thread reads isActive while the children
// thread writes it (but it all works out because a mutex is locked in the main
// thread and isActive re-checked).
// This is to solve issue #19463.
#যদি !defined(CV_USE_GLOBAL_WORKERS_COND_VAR) && defined(__clang__) && defined(__has_feature)
#যদি __has_feature(thread_sanitizer)
__attribute__((no_sanitize("thread")))
#endif
#endif
শূন্য WorkerThread::thread_body()
{
    (শূন্য)cv::utils::getThreadID(); // notify OpenCV about new thread
    CV_LOG_VERBOSE(ফাঁকা, 5, "Thread: new thread: " << id);

    যৌক্তিক allow_active_wait = সত্য;

#ifdef CV_PROFILE_THREADS
    ThreadPool::ThreadStatistics& stat = thread_pool.threads_stat[id + 1];
#endif

    যখন (!stop_thread)
    {
        CV_LOG_VERBOSE(ফাঁকা, 5, "Thread: ... loop iteration: allow_active_wait=" << allow_active_wait << "   has_wake_signal=" << has_wake_signal);
        যদি (allow_active_wait && CV_WORKER_ACTIVE_WAIT > 0)
        {
            allow_active_wait = মিথ্যা;
            জন্য (পূর্ণসংখ্যা i = 0; i < CV_WORKER_ACTIVE_WAIT; i++)
            {
                যদি (has_wake_signal)
                    বিরতি;
                যদি (CV_ACTIVE_WAIT_PAUSE_LIMIT > 0 && (i < CV_ACTIVE_WAIT_PAUSE_LIMIT || (i & 1)))
                    CV_PAUSE(16);
                অন্যথা
                    CV_YIELD();
            }
        }
        pthread_mutex_lock(&mutex);
#ifdef CV_PROFILE_THREADS
        stat.threadWait = get_Tick_গুনতি();
#endif
        যখন (!has_wake_signal) // to handle spurious wakeups
        {
            //CV_LOG_VERBOSE(NULL, 5, "Thread: wait (sleep) ...");
#যদি defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
            pthread_cond_wait(&thread_pool.cond_thread_wake, &mutex);
#অন্যথা
            isActive = মিথ্যা;
            pthread_cond_wait(&cond_thread_wake, &mutex);
            isActive = সত্য;
#endif
            CV_LOG_VERBOSE(ফাঁকা, 5, "Thread: wake ... (has_wake_signal=" << has_wake_signal << " stop_thread=" << stop_thread << ")")
        }
#ifdef CV_PROFILE_THREADS
        stat.threadWake = get_Tick_গুনতি();
#endif

        CV_LOG_VERBOSE(ফাঁকা, 5, "Thread: checking for new job");
        যদি (CV_WORKER_ACTIVE_WAIT_THREADS_LIMIT == 0)
            allow_active_wait = সত্য;
        Ptr<ParallelJob> j_ptr; swap(j_ptr, job);
        has_wake_signal = মিথ্যা;
        pthread_mutex_unlock(&mutex);

        যদি (!stop_thread)
        {
            ParallelJob* j = j_ptr;
            যদি (j)
            {
                CV_LOG_VERBOSE(ফাঁকা, 5, "Thread: job size=" << j->range.size() << " done=" << j->current_task);
                যদি (j->current_task < j->range.size())
                {
                    পূর্ণসংখ্যা other = j->active_thread_গুনতি.fetch_add(1, std::memory_order_seq_cst);
                    CV_LOG_VERBOSE(ফাঁকা, 5, "Thread: processing new job (with " << other << " other threads)"); CV_UNUSED(other);
#ifdef CV_PROFILE_THREADS
                    stat.threadExecuteStart = get_Tick_গুনতি();
                    stat.executedTasks = j->execute(সত্য);
                    stat.threadExecuteStop = get_Tick_গুনতি();
#অন্যথা
                    j->execute(সত্য);
#endif
                    পূর্ণসংখ্যা completed = j->completed_thread_গুনতি.fetch_add(1, std::memory_order_seq_cst) + 1;
                    পূর্ণসংখ্যা active = j->active_thread_গুনতি.load(std::memory_order_acquire);
                    যদি (CV_WORKER_ACTIVE_WAIT_THREADS_LIMIT > 0)
                    {
                        allow_active_wait = সত্য;
                        যদি (active >= CV_WORKER_ACTIVE_WAIT_THREADS_LIMIT && (id & 1) == 0) // turn off a half of threads
                            allow_active_wait = মিথ্যা;
                    }
                    CV_LOG_VERBOSE(ফাঁকা, 5, "Thread: completed job processing: " << active << " " << completed);
                    যদি (active == completed)
                    {
                        যৌক্তিক need_signal = !j->is_completed;
                        j->is_completed = সত্য;
                        j = ফাঁকা; j_ptr.release();
                        যদি (need_signal)
                        {
                            CV_LOG_VERBOSE(ফাঁকা, 5, "Thread: job finished => notifying the main thread");
                            pthread_mutex_lock(&thread_pool.mutex_notify);  // to avoid signal miss due pre-check condition
                            // empty
                            pthread_mutex_unlock(&thread_pool.mutex_notify);
                            pthread_cond_broadcast/*pthread_cond_signal*/(&thread_pool.cond_thread_task_complete);
                        }
                    }
                }
                অন্যথা
                {
                    CV_LOG_VERBOSE(ফাঁকা, 5, "Thread: no free job tasks");
                }
            }
        }
#ifdef CV_PROFILE_THREADS
        stat.threadFree = get_Tick_গুনতি();
        stat.keepActive = allow_active_wait;
#endif
    }
}

ThreadPool::ThreadPool()
{
#ifdef CV_PROFILE_THREADS
    tickFreq = getTickFrequency();
#endif

    পূর্ণসংখ্যা res = 0;
    res |= pthread_mutex_init(&mutex, ফাঁকা);
    res |= pthread_mutex_init(&mutex_notify, ফাঁকা);
#যদি defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
    res |= pthread_cond_init(&cond_thread_wake, ফাঁকা);
#endif
    res |= pthread_cond_init(&cond_thread_task_complete, ফাঁকা);

    যদি (0 != res)
    {
        CV_LOG_FATAL(ফাঁকা, "Failed to initialize ThreadPool (pthreads)");
    }
    অঙ্ক_threads = default_সংখ্যা_Of_Threads();
}

যৌক্তিক ThreadPool::reconfigure_(unsigned new_threads_গুনতি)
{
    যদি (new_threads_গুনতি == threads.size())
        ফেরত মিথ্যা;

    যদি (new_threads_গুনতি < threads.size())
    {
        CV_LOG_VERBOSE(ফাঁকা, 1, "MainThread: reduce worker pool: " << threads.size() << " => " << new_threads_গুনতি);
        std::vector< Ptr<WorkerThread> > release_threads(threads.size() - new_threads_গুনতি);
        জন্য (size_t i = new_threads_গুনতি; i < threads.size(); ++i)
        {
            pthread_mutex_lock(&threads[i]->mutex);  // to avoid signal miss due pre-check
            threads[i]->stop_thread = সত্য;
            threads[i]->has_wake_signal = সত্য;
#যদি !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
            pthread_mutex_unlock(&threads[i]->mutex);
            pthread_cond_broadcast/*pthread_cond_signal*/(&threads[i]->cond_thread_wake); // wake thread
#অন্যথা
            pthread_mutex_unlock(&threads[i]->mutex);
#endif
            std::swap(threads[i], release_threads[i - new_threads_গুনতি]);
        }
#যদি defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
        CV_LOG_VERBOSE(ফাঁকা, 1, "MainThread: notify worker threads about termination...");
        pthread_cond_broadcast(&cond_thread_wake); // wake all threads
#endif
        threads.resize(new_threads_গুনতি);
        release_threads.clear();  // calls thread_join which want to lock mutex
        ফেরত মিথ্যা;
    }
    অন্যথা
    {
        CV_LOG_VERBOSE(ফাঁকা, 1, "MainThread: upgrade worker pool: " << threads.size() << " => " << new_threads_গুনতি);
        জন্য (size_t i = threads.size(); i < new_threads_গুনতি; ++i)
        {
            threads.push_back(Ptr<WorkerThread>(নতুন WorkerThread(*এই, (unsigned)i))); // spawn more threads
        }
    }
    ফেরত মিথ্যা;
}

ThreadPool::~ThreadPool()
{
    reconfigure(0);
    pthread_cond_destroy(&cond_thread_task_complete);
#যদি defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
    pthread_cond_destroy(&cond_thread_wake);
#endif
    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&mutex_notify);
}

শূন্য ThreadPool::run(ধ্রুবক Range& range, ধ্রুবক ParallelLoopBody& body, দ্বিগুণ nstripes)
{
    CV_LOG_VERBOSE(ফাঁকা, 1, "MainThread: new parallel job: num_threads=" << অঙ্ক_threads << "   range=" << range.size() << "   nstripes=" << nstripes << "   job=" << (শূন্য*)job);
#ifdef CV_PROFILE_THREADS
    jobSubmitTime = get_Tick_গুনতি();
    threads_stat[0].reset();
    threads_stat[0].threadWait = jobSubmitTime;
    threads_stat[0].threadWake = jobSubmitTime;
#endif
    যদি (get_অঙ্ক_Of_Threads() > 1 &&
        job == ফাঁকা &&
        (range.size() * nstripes >= 2 || (range.size() > 1 && nstripes <= 0))
    )
    {
        pthread_mutex_lock(&mutex);
        যদি (job != ফাঁকা)
        {
            pthread_mutex_unlock(&mutex);
            body(range);
            ফেরত;
        }
        reconfigure_(অঙ্ক_threads - 1);

        {
            CV_LOG_VERBOSE(ফাঁকা, 1, "MainThread: initialize parallel job: " << range.size());
            job = Ptr<ParallelJob>(নতুন ParallelJob(*এই, range, body, nstripes));
            pthread_mutex_unlock(&mutex);

            CV_LOG_VERBOSE(ফাঁকা, 5, "MainThread: wake worker threads...");
            size_t অঙ্ক_threads_to_wake = std::min(static_cast<size_t>(range.size()), threads.size());
            জন্য (size_t i = 0; i < অঙ্ক_threads_to_wake; ++i)
            {
                যদি (job->current_task >= job->range.size())
                    বিরতি;
                WorkerThread& thread = *(threads[i].get());
                যদি (
#যদি defined(__clang__) && defined(__has_feature)
#যদি __has_feature(thread_sanitizer)
                        1 ||  // Robust workaround to avoid data race warning of `thread.job`
#endif
#endif
#যদি !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
                        thread.isActive ||
#endif
                        thread.has_wake_signal
                        || !thread.job.empty()  // #10881
                )
                {
                    pthread_mutex_lock(&thread.mutex);
                    thread.job = job;
#যদি !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
                    যৌক্তিক isActive = thread.isActive;
#endif
                    thread.has_wake_signal = সত্য;
#ifdef CV_PROFILE_THREADS
                    threads_stat[i + 1].reset();
#endif
                    pthread_mutex_unlock(&thread.mutex);
#যদি !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
                    যদি (!isActive)
                    {
                        pthread_cond_broadcast/*pthread_cond_signal*/(&thread.cond_thread_wake); // wake thread
                    }
#endif
                }
                অন্যথা
                {
                    CV_Assert(thread.job.empty());
                    thread.job = job;
                    thread.has_wake_signal = সত্য;
#ifdef CV_PROFILE_THREADS
                    threads_stat[i + 1].reset();
#endif
#যদি !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
                    pthread_cond_broadcast/*pthread_cond_signal*/(&thread.cond_thread_wake); // wake thread
#endif
                }
            }
#ifdef CV_PROFILE_THREADS
            threads_stat[0].threadPing = get_Tick_গুনতি();
#endif
#যদি defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
            pthread_cond_broadcast(&cond_thread_wake); // wake all threads
#endif
#ifdef CV_PROFILE_THREADS
            threads_stat[0].threadWake = get_Tick_গুনতি();
#endif
            CV_LOG_VERBOSE(ফাঁকা, 5, "MainThread: wake worker threads... (done)");

            {
                ParallelJob& j = *(এই->job);
#ifdef CV_PROFILE_THREADS
                threads_stat[0].threadExecuteStart = get_Tick_গুনতি();
                threads_stat[0].executedTasks = j.execute(মিথ্যা);
                threads_stat[0].threadExecuteStop = get_Tick_গুনতি();
#অন্যথা
                j.execute(মিথ্যা);
#endif
                CV_Assert(j.current_task >= j.range.size());
                CV_LOG_VERBOSE(ফাঁকা, 5, "MainThread: complete self-tasks: " << j.active_thread_গুনতি << " " << j.completed_thread_গুনতি);
                যদি (job->is_completed || j.active_thread_গুনতি == 0)
                {
                    job->is_completed = সত্য;
                    CV_LOG_VERBOSE(ফাঁকা, 5, "MainThread: no WIP worker threads");
                }
                অন্যথা
                {
                    যদি (CV_MAIN_THREAD_ACTIVE_WAIT > 0)
                    {
                        জন্য (পূর্ণসংখ্যা i = 0; i < CV_MAIN_THREAD_ACTIVE_WAIT; i++)  // don't spin too much in any case (inaccurate getTickCount())
                        {
                            যদি (job->is_completed)
                            {
                                CV_LOG_VERBOSE(ফাঁকা, 5, "MainThread: job finalize (active wait) " << j.active_thread_গুনতি << " " << j.completed_thread_গুনতি);
                                বিরতি;
                            }
                            যদি (CV_ACTIVE_WAIT_PAUSE_LIMIT > 0 && (i < CV_ACTIVE_WAIT_PAUSE_LIMIT || (i & 1)))
                                CV_PAUSE(16);
                            অন্যথা
                                CV_YIELD();
                        }
                    }
                    যদি (!job->is_completed)
                    {
                        CV_LOG_VERBOSE(ফাঁকা, 5, "MainThread: prepare wait " << j.active_thread_গুনতি << " " << j.completed_thread_গুনতি);
                        pthread_mutex_lock(&mutex_notify);
                        জন্য (;;)
                        {
                            যদি (job->is_completed)
                            {
                                CV_LOG_VERBOSE(ফাঁকা, 5, "MainThread: job finalize (wait) " << j.active_thread_গুনতি << " " << j.completed_thread_গুনতি);
                                বিরতি;
                            }
                            CV_LOG_VERBOSE(ফাঁকা, 5, "MainThread: wait completion (sleep) ...");
                            pthread_cond_wait(&cond_thread_task_complete, &mutex_notify);
                            CV_LOG_VERBOSE(ফাঁকা, 5, "MainThread: wake");
                        }
                        pthread_mutex_unlock(&mutex_notify);
                    }
                }
            }
#ifdef CV_PROFILE_THREADS
            threads_stat[0].threadFree = get_Tick_গুনতি();
            std::cout << "Job: sz=" << range.size() << " nstripes=" << nstripes << "    Time: " << (threads_stat[0].threadFree - jobSubmitTime) / tickFreq * 1e6 << " usec" << std::endl;
            জন্য (পূর্ণসংখ্যা i = 0; i < (পূর্ণসংখ্যা)threads.size() + 1; i++)
            {
                threads_stat[i].dump(i - 1, jobSubmitTime, tickFreq);
            }
#endif
            যদি (job)
            {
                pthread_mutex_lock(&mutex);
                CV_LOG_VERBOSE(ফাঁকা, 5, "MainThread: job release");
                CV_Assert(job->is_completed);
                job.release();
                pthread_mutex_unlock(&mutex);
            }
        }
    }
    অন্যথা
    {
        body(range);
    }
}

size_t ThreadPool::get_অঙ্ক_Of_Threads()
{
    ফেরত অঙ্ক_threads;
}

শূন্য ThreadPool::set_অঙ্ক_Of_Threads(unsigned n)
{
    যদি (n != অঙ্ক_threads)
    {
        অঙ্ক_threads = n;
        যদি (n == 1)
           যদি (job == ফাঁকা) reconfigure(0);  // stop worker threads immediately
    }
}

size_t parallel_pthreads_get_threads_অঙ্ক()
{
    ফেরত ThreadPool::instance().get_অঙ্ক_Of_Threads();
}

শূন্য parallel_pthreads_set_threads_অঙ্ক(পূর্ণসংখ্যা num)
{
    যদি(num < 0)
    {
        ThreadPool::instance().set_অঙ্ক_Of_Threads(0);
    }
    অন্যথা
    {
        ThreadPool::instance().set_অঙ্ক_Of_Threads(unsigned(num));
    }
}

শূন্য parallel_for_pthreads(ধ্রুবক Range& range, ধ্রুবক ParallelLoopBody& body, দ্বিগুণ nstripes)
{
    ThreadPool::instance().run(range, body, nstripes);
}

}

#endif
