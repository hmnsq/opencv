// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ضمن "precomp.hpp"

#ضمن "parallel_impl.hpp"

#ifdef HAVE_PTHREADS_PF
#ضمن <pthread.h>

#ضمن <opencv2/core/utils/configuration.private.hpp>

#ضمن <opencv2/core/utils/logger.defines.hpp>
//#undef CV_LOG_STRIP_LEVEL
//#define CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_VERBOSE + 1
#ضمن <opencv2/core/utils/logger.hpp>

#ضمن <opencv2/core/utils/trace.private.hpp>

//#define CV_PROFILE_THREADS 64
//#define getTickCount getCPUTickCount  // use this if getTickCount() calls are expensive (and getCPUTickCount() is accurate)

//#define CV_USE_GLOBAL_WORKERS_COND_VAR  // not effective on many-core systems (10+)

#ضمن <atomic>

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
# إذا defined __GNUC__ && (defined __i386__ || defined __x86_64__)
#   include <x86intrin.h> /* for __rdtsc */
#   إذا !defined(__SSE2__)
      ساكن مضمن فراغ cv_non_sse_mm_pause() { __asm__ __volatile__ ("rep; nop"); }
#     define _mm_pause cv_non_sse_mm_pause
#   endif
// With Skylake CPUs and above, _mm_pause takes 140 cycles so no need for a loop.
#   define CV_PAUSE(v) افعل { (فراغ)v; _mm_pause(); } طالما (0)
# elif defined __GNUC__ && defined __aarch64__
#   define CV_PAUSE(v) افعل { لكل (صحيح __delay = (v); __delay > 0; --__delay) { asm متطاير("yield" ::: "memory"); } } طالما (0)
# elif defined __GNUC__ && defined __arm__
#   define CV_PAUSE(v) افعل { لكل (صحيح __delay = (v); __delay > 0; --__delay) { asm متطاير("" ::: "memory"); } } طالما (0)
# elif defined __GNUC__ && defined __mips__ && __mips_isa_rev >= 2
#   define CV_PAUSE(v) افعل { لكل (صحيح __delay = (v); __delay > 0; --__delay) { asm متطاير("pause" ::: "memory"); } } طالما (0)
# elif defined __APPLE__ && defined __POWERPC__
#   define CV_PAUSE(v) افعل { لكل (صحيح __delay = (v); __delay > 0; --__delay) { asm متطاير("or r27,r27,r27" ::: "memory"); } } طالما (0)
# elif defined __GNUC__ && defined __PPC64__
#   define CV_PAUSE(v) افعل { لكل (صحيح __delay = (v); __delay > 0; --__delay) { asm متطاير("or 27,27,27" ::: "memory"); } } طالما (0)
# elif defined __GNUC__ && defined __riscv
// PAUSE HINT is not part of RISC-V ISA yet, but is under discussion now. For details see:
// https://github.com/riscv/riscv-isa-manual/pull/398
// https://github.com/riscv/riscv-isa-manual/issues/43
// #   define CV_PAUSE(v) do { for (int __delay = (v); __delay > 0; --__delay) { asm volatile("pause"); } } while (0)
#   define CV_PAUSE(v) افعل { لكل (صحيح __delay = (v); __delay > 0; --__delay) { asm متطاير("nop"); } } طالما (0)
# elif defined __GNUC__ && defined __loongarch__
#   define CV_PAUSE(v) افعل { لكل (صحيح __delay = (v); __delay > 0; --__delay) { asm متطاير("nop"); } } طالما (0)
# وإلا
#   warning "Can't detect 'pause' (CPU-yield) instruction on the target platform. Specify CV_PAUSE() definition via compiler flags."
#   define CV_PAUSE(...) افعل { /* no-op: works, but not effective */ } طالما (0)
# endif
#endif // CV_PAUSE


نطاق cv
{

ساكن صحيح CV_ACTIVE_WAIT_PAUSE_LIMIT = (صحيح)utils::getConfigurationParameterSizeT("OPENCV_THREAD_POOL_ACTIVE_WAIT_PAUSE_LIMIT", 16);  // iterations
ساكن صحيح CV_WORKER_ACTIVE_WAIT = (صحيح)utils::getConfigurationParameterSizeT("OPENCV_THREAD_POOL_ACTIVE_WAIT_WORKER", 2000);  // iterations
ساكن صحيح CV_MAIN_THREAD_ACTIVE_WAIT = (صحيح)utils::getConfigurationParameterSizeT("OPENCV_THREAD_POOL_ACTIVE_WAIT_MAIN", 10000); // iterations

ساكن صحيح CV_WORKER_ACTIVE_WAIT_THREADS_LIMIT = (صحيح)utils::getConfigurationParameterSizeT("OPENCV_THREAD_POOL_ACTIVE_WAIT_THREADS_LIMIT", 0); // number of real cores

صنف WorkerThread;
صنف ParallelJob;

صنف ThreadPool
{
عام:
    ساكن ThreadPool& instance()
    {
        CV_SINGLETON_LAZY_INIT_REF(ThreadPool, جديد ThreadPool())
    }

    ساكن فراغ stop()
    {
        ThreadPool& manager = instance();
        manager.reconfigure(0);
    }

    فراغ reconfigure(unsigned new_threads_سانى)
    {
        إذا (new_threads_سانى == threads.size())
            أرجع;
        pthread_mutex_lock(&mutex);
        reconfigure_(new_threads_سانى);
        pthread_mutex_unlock(&mutex);
    }
    منطقي reconfigure_(unsigned new_threads_سانى); // internal implementation

    فراغ run(ثابت Range& range, ثابت ParallelLoopBody& body, مزدوج nstripes);

    size_t get_رەقەم_Of_Threads();

    فراغ set_رەقەم_Of_Threads(unsigned n);

    ThreadPool();

    ~ThreadPool();

    unsigned رەقەم_threads;

    pthread_mutex_t mutex;  // guards fields (job/threads) from non-worker threads (concurrent parallel_for calls)
#إذا defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
    pthread_cond_t cond_thread_wake;
#endif

    pthread_mutex_t mutex_notify;
    pthread_cond_t cond_thread_task_complete;

    std::vector< Ptr<WorkerThread> > threads;

    Ptr<ParallelJob> job;

#ifdef CV_PROFILE_THREADS
    مزدوج tickFreq;
    int64 jobSubmitTime;
    بنية ThreadStatistics
    {
        ThreadStatistics() : threadWait(0)
        {
            reset();
        }
        فراغ reset()
        {
            threadWake = 0;
            threadExecuteStart = 0;
            threadExecuteStop = 0;
            executedTasks = 0;
            keepActive = خطأ;
            threadPing = get_Tick_سانى();
        }
        int64 threadWait; // don't reset by default
        int64 threadPing; // don't reset by default
        int64 threadWake;
        int64 threadExecuteStart;
        int64 threadExecuteStop;
        int64 threadFree;
        unsigned executedTasks;
        منطقي keepActive;

        int64 dummy_[8]; // separate cache lines

        فراغ dump(صحيح id, int64 baseTime, مزدوج tickFreq)
        {
            إذا (id < 0)
                std::cout << "Main: ";
            وإلا
                printf("T%03d: ", id + 2);
            printf("wait=% 10.1f   ping=% 6.1f",
                    threadWait > 0 ? (threadWait - baseTime) / tickFreq * 1e6 : -0.0,
                    threadPing > 0 ? (threadPing - baseTime) / tickFreq * 1e6 : -0.0);
            إذا (threadWake > 0)
                printf("   wake=% 6.1f",
                    (threadWake > 0 ? (threadWake - baseTime) / tickFreq * 1e6 : -0.0));
            إذا (threadExecuteStart > 0)
            {
                printf("   exec=% 6.1f - % 6.1f   tasksDone=%5u   free=% 6.1f",
                    (threadExecuteStart > 0 ? (threadExecuteStart - baseTime) / tickFreq * 1e6 : -0.0),
                    (threadExecuteStop > 0 ? (threadExecuteStop - baseTime) / tickFreq * 1e6 : -0.0),
                    executedTasks,
                    (threadFree > 0 ? (threadFree - baseTime) / tickFreq * 1e6 : -0.0));
                إذا (id >= 0)
                    printf(" active=%s\n", keepActive ? "true" : "false");
                وإلا
                    printf("\n");
            }
            وإلا
                printf("   ------------------------------------------------------------------------------\n");
        }
    };
    ThreadStatistics threads_stat[CV_PROFILE_THREADS]; // 0 - main thread, 1..N - worker threads
#endif

};

صنف WorkerThread
{
عام:
    ThreadPool& thread_pool;
    ثابت unsigned id;
    pthread_t posix_thread;
    منطقي is_created;

    std::atomic<منطقي> stop_thread;

    std::atomic<منطقي> has_wake_signal;

    Ptr<ParallelJob> job;

    pthread_mutex_t mutex;
#إذا !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
    متطاير منطقي isActive;
    pthread_cond_t cond_thread_wake;
#endif

    WorkerThread(ThreadPool& thread_pool_, unsigned id_) :
        thread_pool(thread_pool_),
        id(id_),
        posix_thread(0),
        is_created(خطأ),
        stop_thread(خطأ),
        has_wake_signal(خطأ)
#إذا !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
        , isActive(true)
#endif
    {
        CV_LOG_VERBOSE(عدم, 1, "MainThread: initializing new worker: " << id);
        صحيح res = pthread_mutex_init(&mutex, عدم);
        إذا (res != 0)
        {
            CV_LOG_ERROR(عدم, id << ": Can't create thread mutex: res = " << res);
            أرجع;
        }
#إذا !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
        res = pthread_cond_init(&cond_thread_wake, عدم);
        إذا (res != 0)
        {
            CV_LOG_ERROR(عدم, id << ": Can't create thread condition variable: res = " << res);
            أرجع;
        }
#endif
        res = pthread_create(&posix_thread, عدم, thread_loop_wrapper, (فراغ*)هذا);
        إذا (res != 0)
        {
            CV_LOG_ERROR(عدم, id << ": Can't spawn new thread: res = " << res);
        }
        وإلا
        {
            is_created = true;
        }
    }

    ~WorkerThread()
    {
        CV_LOG_VERBOSE(عدم, 1, "MainThread: destroy worker thread: " << id);
        إذا (is_created)
        {
            إذا (!stop_thread)
            {
                pthread_mutex_lock(&mutex);  // to avoid signal miss due pre-check
                stop_thread = true;
                pthread_mutex_unlock(&mutex);
#إذا defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
                pthread_cond_broadcast(&thread_pool.cond_thread_wake);
#وإلا
                pthread_cond_signal(&cond_thread_wake);
#endif
            }
            pthread_join(posix_thread, عدم);
        }
#إذا !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
        pthread_cond_destroy(&cond_thread_wake);
#endif
        pthread_mutex_destroy(&mutex);
    }

    فراغ thread_body();
    ساكن فراغ* thread_loop_wrapper(فراغ* thread_object)
    {
#ifdef OPENCV_WITH_ITT
        __itt_thread_set_name(cv::format("OpenCVThread-%03d", cv::utils::getThreadID()).c_str());
#endif
        ((WorkerThread*)thread_object)->thread_body();
        أرجع 0;
    }
};

صنف ParallelJob
{
عام:
    ParallelJob(ثابت ThreadPool& thread_pool_, ثابت Range& range_, ثابت ParallelLoopBody& body_, صحيح nstripes_) :
        thread_pool(thread_pool_),
        body(body_),
        range(range_),
        nstripes((unsigned)nstripes_),
        is_completed(خطأ)
    {
        CV_LOG_VERBOSE(عدم, 5, "ParallelJob::ParallelJob(" << (فراغ*)هذا << ")");
        current_task.store(0, std::memory_order_relaxed);
        active_thread_سانى.store(0, std::memory_order_relaxed);
        completed_thread_سانى.store(0, std::memory_order_relaxed);
        dummy0_[0] = 0, dummy1_[0] = 0, dummy2_[0] = 0; // compiler warning
    }

    ~ParallelJob()
    {
        CV_LOG_VERBOSE(عدم, 5, "ParallelJob::~ParallelJob(" << (فراغ*)هذا << ")");
    }

    unsigned execute(منطقي is_worker_thread)
    {
        unsigned executed_tasks = 0;
        ثابت صحيح task_سانى = range.size();
        ثابت صحيح remaining_multiplier = std::min(nstripes,
                std::max(
                        std::min(100u, thread_pool.رەقەم_threads * 4),
                        thread_pool.رەقەم_threads * 2
                ));  // experimental value
        لكل (;;)
        {
            صحيح chunk_size = std::max(1, (task_سانى - current_task) / remaining_multiplier);
            صحيح id = current_task.fetch_add(chunk_size, std::memory_order_seq_cst);
            إذا (id >= task_سانى)
                اكسر; // no more free tasks

            executed_tasks += chunk_size;
            صحيح start_id = id;
            صحيح end_id = std::min(task_سانى, id + chunk_size);
            CV_LOG_VERBOSE(عدم, 9, "Thread: job " << start_id << "-" << end_id);

            //TODO: if (not pending exception)
            {
                body.operator()(Range(range.start + start_id, range.start + end_id));
            }
            إذا (is_worker_thread && is_completed)
            {
                CV_LOG_ERROR(عدم, "\t\t\t\tBUG! Job: " << (فراغ*)هذا << " " << id << " " << active_thread_سانى << " " << completed_thread_سانى);
                CV_Assert(!is_completed); // TODO Dbg this
            }
        }
        أرجع executed_tasks;
    }

    ثابت ThreadPool& thread_pool;
    ثابت ParallelLoopBody& body;
    ثابت Range range;
    ثابت unsigned nstripes;

    std::atomic<صحيح> current_task;  // next free part of job
    int64 dummy0_[8];  // avoid cache-line reusing for the same atomics

    std::atomic<صحيح> active_thread_سانى;  // number of threads worked on this job
    int64 dummy1_[8];  // avoid cache-line reusing for the same atomics

    std::atomic<صحيح> completed_thread_سانى;  // number of threads completed any activities on this job
    int64 dummy2_[8];  // avoid cache-line reusing for the same atomics

    std::atomic<منطقي> is_completed;

    // TODO exception handling
};


// Disable thread sanitization check when CV_USE_GLOBAL_WORKERS_COND_VAR is not
// set because it triggers as the main thread reads isActive while the children
// thread writes it (but it all works out because a mutex is locked in the main
// thread and isActive re-checked).
// This is to solve issue #19463.
#إذا !defined(CV_USE_GLOBAL_WORKERS_COND_VAR) && defined(__clang__) && defined(__has_feature)
#إذا __has_feature(thread_sanitizer)
__attribute__((no_sanitize("thread")))
#endif
#endif
فراغ WorkerThread::thread_body()
{
    (فراغ)cv::utils::getThreadID(); // notify OpenCV about new thread
    CV_LOG_VERBOSE(عدم, 5, "Thread: new thread: " << id);

    منطقي allow_active_wait = true;

#ifdef CV_PROFILE_THREADS
    ThreadPool::ThreadStatistics& stat = thread_pool.threads_stat[id + 1];
#endif

    طالما (!stop_thread)
    {
        CV_LOG_VERBOSE(عدم, 5, "Thread: ... loop iteration: allow_active_wait=" << allow_active_wait << "   has_wake_signal=" << has_wake_signal);
        إذا (allow_active_wait && CV_WORKER_ACTIVE_WAIT > 0)
        {
            allow_active_wait = خطأ;
            لكل (صحيح i = 0; i < CV_WORKER_ACTIVE_WAIT; i++)
            {
                إذا (has_wake_signal)
                    اكسر;
                إذا (CV_ACTIVE_WAIT_PAUSE_LIMIT > 0 && (i < CV_ACTIVE_WAIT_PAUSE_LIMIT || (i & 1)))
                    CV_PAUSE(16);
                وإلا
                    CV_YIELD();
            }
        }
        pthread_mutex_lock(&mutex);
#ifdef CV_PROFILE_THREADS
        stat.threadWait = get_Tick_سانى();
#endif
        طالما (!has_wake_signal) // to handle spurious wakeups
        {
            //CV_LOG_VERBOSE(NULL, 5, "Thread: wait (sleep) ...");
#إذا defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
            pthread_cond_wait(&thread_pool.cond_thread_wake, &mutex);
#وإلا
            isActive = خطأ;
            pthread_cond_wait(&cond_thread_wake, &mutex);
            isActive = true;
#endif
            CV_LOG_VERBOSE(عدم, 5, "Thread: wake ... (has_wake_signal=" << has_wake_signal << " stop_thread=" << stop_thread << ")")
        }
#ifdef CV_PROFILE_THREADS
        stat.threadWake = get_Tick_سانى();
#endif

        CV_LOG_VERBOSE(عدم, 5, "Thread: checking for new job");
        إذا (CV_WORKER_ACTIVE_WAIT_THREADS_LIMIT == 0)
            allow_active_wait = true;
        Ptr<ParallelJob> j_ptr; swap(j_ptr, job);
        has_wake_signal = خطأ;
        pthread_mutex_unlock(&mutex);

        إذا (!stop_thread)
        {
            ParallelJob* j = j_ptr;
            إذا (j)
            {
                CV_LOG_VERBOSE(عدم, 5, "Thread: job size=" << j->range.size() << " done=" << j->current_task);
                إذا (j->current_task < j->range.size())
                {
                    صحيح other = j->active_thread_سانى.fetch_add(1, std::memory_order_seq_cst);
                    CV_LOG_VERBOSE(عدم, 5, "Thread: processing new job (with " << other << " other threads)"); CV_UNUSED(other);
#ifdef CV_PROFILE_THREADS
                    stat.threadExecuteStart = get_Tick_سانى();
                    stat.executedTasks = j->execute(true);
                    stat.threadExecuteStop = get_Tick_سانى();
#وإلا
                    j->execute(true);
#endif
                    صحيح completed = j->completed_thread_سانى.fetch_add(1, std::memory_order_seq_cst) + 1;
                    صحيح active = j->active_thread_سانى.load(std::memory_order_acquire);
                    إذا (CV_WORKER_ACTIVE_WAIT_THREADS_LIMIT > 0)
                    {
                        allow_active_wait = true;
                        إذا (active >= CV_WORKER_ACTIVE_WAIT_THREADS_LIMIT && (id & 1) == 0) // turn off a half of threads
                            allow_active_wait = خطأ;
                    }
                    CV_LOG_VERBOSE(عدم, 5, "Thread: completed job processing: " << active << " " << completed);
                    إذا (active == completed)
                    {
                        منطقي need_signal = !j->is_completed;
                        j->is_completed = true;
                        j = عدم; j_ptr.release();
                        إذا (need_signal)
                        {
                            CV_LOG_VERBOSE(عدم, 5, "Thread: job finished => notifying the main thread");
                            pthread_mutex_lock(&thread_pool.mutex_notify);  // to avoid signal miss due pre-check condition
                            // empty
                            pthread_mutex_unlock(&thread_pool.mutex_notify);
                            pthread_cond_broadcast/*pthread_cond_signal*/(&thread_pool.cond_thread_task_complete);
                        }
                    }
                }
                وإلا
                {
                    CV_LOG_VERBOSE(عدم, 5, "Thread: no free job tasks");
                }
            }
        }
#ifdef CV_PROFILE_THREADS
        stat.threadFree = get_Tick_سانى();
        stat.keepActive = allow_active_wait;
#endif
    }
}

ThreadPool::ThreadPool()
{
#ifdef CV_PROFILE_THREADS
    tickFreq = getTickFrequency();
#endif

    صحيح res = 0;
    res |= pthread_mutex_init(&mutex, عدم);
    res |= pthread_mutex_init(&mutex_notify, عدم);
#إذا defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
    res |= pthread_cond_init(&cond_thread_wake, عدم);
#endif
    res |= pthread_cond_init(&cond_thread_task_complete, عدم);

    إذا (0 != res)
    {
        CV_LOG_FATAL(عدم, "Failed to initialize ThreadPool (pthreads)");
    }
    رەقەم_threads = default_سان_Of_Threads();
}

منطقي ThreadPool::reconfigure_(unsigned new_threads_سانى)
{
    إذا (new_threads_سانى == threads.size())
        أرجع خطأ;

    إذا (new_threads_سانى < threads.size())
    {
        CV_LOG_VERBOSE(عدم, 1, "MainThread: reduce worker pool: " << threads.size() << " => " << new_threads_سانى);
        std::vector< Ptr<WorkerThread> > release_threads(threads.size() - new_threads_سانى);
        لكل (size_t i = new_threads_سانى; i < threads.size(); ++i)
        {
            pthread_mutex_lock(&threads[i]->mutex);  // to avoid signal miss due pre-check
            threads[i]->stop_thread = true;
            threads[i]->has_wake_signal = true;
#إذا !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
            pthread_mutex_unlock(&threads[i]->mutex);
            pthread_cond_broadcast/*pthread_cond_signal*/(&threads[i]->cond_thread_wake); // wake thread
#وإلا
            pthread_mutex_unlock(&threads[i]->mutex);
#endif
            std::swap(threads[i], release_threads[i - new_threads_سانى]);
        }
#إذا defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
        CV_LOG_VERBOSE(عدم, 1, "MainThread: notify worker threads about termination...");
        pthread_cond_broadcast(&cond_thread_wake); // wake all threads
#endif
        threads.resize(new_threads_سانى);
        release_threads.clear();  // calls thread_join which want to lock mutex
        أرجع خطأ;
    }
    وإلا
    {
        CV_LOG_VERBOSE(عدم, 1, "MainThread: upgrade worker pool: " << threads.size() << " => " << new_threads_سانى);
        لكل (size_t i = threads.size(); i < new_threads_سانى; ++i)
        {
            threads.push_back(Ptr<WorkerThread>(جديد WorkerThread(*هذا, (unsigned)i))); // spawn more threads
        }
    }
    أرجع خطأ;
}

ThreadPool::~ThreadPool()
{
    reconfigure(0);
    pthread_cond_destroy(&cond_thread_task_complete);
#إذا defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
    pthread_cond_destroy(&cond_thread_wake);
#endif
    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&mutex_notify);
}

فراغ ThreadPool::run(ثابت Range& range, ثابت ParallelLoopBody& body, مزدوج nstripes)
{
    CV_LOG_VERBOSE(عدم, 1, "MainThread: new parallel job: num_threads=" << رەقەم_threads << "   range=" << range.size() << "   nstripes=" << nstripes << "   job=" << (فراغ*)job);
#ifdef CV_PROFILE_THREADS
    jobSubmitTime = get_Tick_سانى();
    threads_stat[0].reset();
    threads_stat[0].threadWait = jobSubmitTime;
    threads_stat[0].threadWake = jobSubmitTime;
#endif
    إذا (get_رەقەم_Of_Threads() > 1 &&
        job == عدم &&
        (range.size() * nstripes >= 2 || (range.size() > 1 && nstripes <= 0))
    )
    {
        pthread_mutex_lock(&mutex);
        إذا (job != عدم)
        {
            pthread_mutex_unlock(&mutex);
            body(range);
            أرجع;
        }
        reconfigure_(رەقەم_threads - 1);

        {
            CV_LOG_VERBOSE(عدم, 1, "MainThread: initialize parallel job: " << range.size());
            job = Ptr<ParallelJob>(جديد ParallelJob(*هذا, range, body, nstripes));
            pthread_mutex_unlock(&mutex);

            CV_LOG_VERBOSE(عدم, 5, "MainThread: wake worker threads...");
            size_t رەقەم_threads_to_wake = std::min(static_cast<size_t>(range.size()), threads.size());
            لكل (size_t i = 0; i < رەقەم_threads_to_wake; ++i)
            {
                إذا (job->current_task >= job->range.size())
                    اكسر;
                WorkerThread& thread = *(threads[i].get());
                إذا (
#إذا defined(__clang__) && defined(__has_feature)
#إذا __has_feature(thread_sanitizer)
                        1 ||  // Robust workaround to avoid data race warning of `thread.job`
#endif
#endif
#إذا !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
                        thread.isActive ||
#endif
                        thread.has_wake_signal
                        || !thread.job.empty()  // #10881
                )
                {
                    pthread_mutex_lock(&thread.mutex);
                    thread.job = job;
#إذا !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
                    منطقي isActive = thread.isActive;
#endif
                    thread.has_wake_signal = true;
#ifdef CV_PROFILE_THREADS
                    threads_stat[i + 1].reset();
#endif
                    pthread_mutex_unlock(&thread.mutex);
#إذا !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
                    إذا (!isActive)
                    {
                        pthread_cond_broadcast/*pthread_cond_signal*/(&thread.cond_thread_wake); // wake thread
                    }
#endif
                }
                وإلا
                {
                    CV_Assert(thread.job.empty());
                    thread.job = job;
                    thread.has_wake_signal = true;
#ifdef CV_PROFILE_THREADS
                    threads_stat[i + 1].reset();
#endif
#إذا !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
                    pthread_cond_broadcast/*pthread_cond_signal*/(&thread.cond_thread_wake); // wake thread
#endif
                }
            }
#ifdef CV_PROFILE_THREADS
            threads_stat[0].threadPing = get_Tick_سانى();
#endif
#إذا defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
            pthread_cond_broadcast(&cond_thread_wake); // wake all threads
#endif
#ifdef CV_PROFILE_THREADS
            threads_stat[0].threadWake = get_Tick_سانى();
#endif
            CV_LOG_VERBOSE(عدم, 5, "MainThread: wake worker threads... (done)");

            {
                ParallelJob& j = *(هذا->job);
#ifdef CV_PROFILE_THREADS
                threads_stat[0].threadExecuteStart = get_Tick_سانى();
                threads_stat[0].executedTasks = j.execute(خطأ);
                threads_stat[0].threadExecuteStop = get_Tick_سانى();
#وإلا
                j.execute(خطأ);
#endif
                CV_Assert(j.current_task >= j.range.size());
                CV_LOG_VERBOSE(عدم, 5, "MainThread: complete self-tasks: " << j.active_thread_سانى << " " << j.completed_thread_سانى);
                إذا (job->is_completed || j.active_thread_سانى == 0)
                {
                    job->is_completed = true;
                    CV_LOG_VERBOSE(عدم, 5, "MainThread: no WIP worker threads");
                }
                وإلا
                {
                    إذا (CV_MAIN_THREAD_ACTIVE_WAIT > 0)
                    {
                        لكل (صحيح i = 0; i < CV_MAIN_THREAD_ACTIVE_WAIT; i++)  // don't spin too much in any case (inaccurate getTickCount())
                        {
                            إذا (job->is_completed)
                            {
                                CV_LOG_VERBOSE(عدم, 5, "MainThread: job finalize (active wait) " << j.active_thread_سانى << " " << j.completed_thread_سانى);
                                اكسر;
                            }
                            إذا (CV_ACTIVE_WAIT_PAUSE_LIMIT > 0 && (i < CV_ACTIVE_WAIT_PAUSE_LIMIT || (i & 1)))
                                CV_PAUSE(16);
                            وإلا
                                CV_YIELD();
                        }
                    }
                    إذا (!job->is_completed)
                    {
                        CV_LOG_VERBOSE(عدم, 5, "MainThread: prepare wait " << j.active_thread_سانى << " " << j.completed_thread_سانى);
                        pthread_mutex_lock(&mutex_notify);
                        لكل (;;)
                        {
                            إذا (job->is_completed)
                            {
                                CV_LOG_VERBOSE(عدم, 5, "MainThread: job finalize (wait) " << j.active_thread_سانى << " " << j.completed_thread_سانى);
                                اكسر;
                            }
                            CV_LOG_VERBOSE(عدم, 5, "MainThread: wait completion (sleep) ...");
                            pthread_cond_wait(&cond_thread_task_complete, &mutex_notify);
                            CV_LOG_VERBOSE(عدم, 5, "MainThread: wake");
                        }
                        pthread_mutex_unlock(&mutex_notify);
                    }
                }
            }
#ifdef CV_PROFILE_THREADS
            threads_stat[0].threadFree = get_Tick_سانى();
            std::cout << "Job: sz=" << range.size() << " nstripes=" << nstripes << "    Time: " << (threads_stat[0].threadFree - jobSubmitTime) / tickFreq * 1e6 << " usec" << std::endl;
            لكل (صحيح i = 0; i < (صحيح)threads.size() + 1; i++)
            {
                threads_stat[i].dump(i - 1, jobSubmitTime, tickFreq);
            }
#endif
            إذا (job)
            {
                pthread_mutex_lock(&mutex);
                CV_LOG_VERBOSE(عدم, 5, "MainThread: job release");
                CV_Assert(job->is_completed);
                job.release();
                pthread_mutex_unlock(&mutex);
            }
        }
    }
    وإلا
    {
        body(range);
    }
}

size_t ThreadPool::get_رەقەم_Of_Threads()
{
    أرجع رەقەم_threads;
}

فراغ ThreadPool::set_رەقەم_Of_Threads(unsigned n)
{
    إذا (n != رەقەم_threads)
    {
        رەقەم_threads = n;
        إذا (n == 1)
           إذا (job == عدم) reconfigure(0);  // stop worker threads immediately
    }
}

size_t parallel_pthreads_get_threads_رەقەم()
{
    أرجع ThreadPool::instance().get_رەقەم_Of_Threads();
}

فراغ parallel_pthreads_set_threads_رەقەم(صحيح num)
{
    إذا(num < 0)
    {
        ThreadPool::instance().set_رەقەم_Of_Threads(0);
    }
    وإلا
    {
        ThreadPool::instance().set_رەقەم_Of_Threads(unsigned(num));
    }
}

فراغ parallel_for_pthreads(ثابت Range& range, ثابت ParallelLoopBody& body, مزدوج nstripes)
{
    ThreadPool::instance().run(range, body, nstripes);
}

}

#endif
