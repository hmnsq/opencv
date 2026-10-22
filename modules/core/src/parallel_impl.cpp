// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#அந்தர்பாவ "precomp.hpp"

#அந்தர்பாவ "parallel_impl.hpp"

#ifdef HAVE_PTHREADS_PF
#அந்தர்பாவ <pthread.h>

#அந்தர்பாவ <opencv2/core/utils/configuration.private.hpp>

#அந்தர்பாவ <opencv2/core/utils/logger.defines.hpp>
//#undef CV_LOG_STRIP_LEVEL
//#define CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_VERBOSE + 1
#அந்தர்பாவ <opencv2/core/utils/logger.hpp>

#அந்தர்பாவ <opencv2/core/utils/trace.private.hpp>

//#define CV_PROFILE_THREADS 64
//#define getTickCount getCPUTickCount  // use this if getTickCount() calls are expensive (and getCPUTickCount() is accurate)

//#define CV_USE_GLOBAL_WORKERS_COND_VAR  // not effective on many-core systems (10+)

#அந்தர்பாவ <atomic>

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
# எனில் defined __GNUC__ && (defined __i386__ || defined __x86_64__)
#   include <x86intrin.h> /* for __rdtsc */
#   எனில் !defined(__SSE2__)
      நிலையான உள்வரி வெறுமை cv_non_sse_mm_pause() { __asm__ __volatile__ ("rep; nop"); }
#     define _mm_pause cv_non_sse_mm_pause
#   endif
// With Skylake CPUs and above, _mm_pause takes 140 cycles so no need for a loop.
#   define CV_PAUSE(v) செய் { (வெறுமை)v; _mm_pause(); } போது (0)
# elif defined __GNUC__ && defined __aarch64__
#   define CV_PAUSE(v) செய் { க்கு (முழு __delay = (v); __delay > 0; --__delay) { asm நிலையற்ற("yield" ::: "memory"); } } போது (0)
# elif defined __GNUC__ && defined __arm__
#   define CV_PAUSE(v) செய் { க்கு (முழு __delay = (v); __delay > 0; --__delay) { asm நிலையற்ற("" ::: "memory"); } } போது (0)
# elif defined __GNUC__ && defined __mips__ && __mips_isa_rev >= 2
#   define CV_PAUSE(v) செய் { க்கு (முழு __delay = (v); __delay > 0; --__delay) { asm நிலையற்ற("pause" ::: "memory"); } } போது (0)
# elif defined __APPLE__ && defined __POWERPC__
#   define CV_PAUSE(v) செய் { க்கு (முழு __delay = (v); __delay > 0; --__delay) { asm நிலையற்ற("or r27,r27,r27" ::: "memory"); } } போது (0)
# elif defined __GNUC__ && defined __PPC64__
#   define CV_PAUSE(v) செய் { க்கு (முழு __delay = (v); __delay > 0; --__delay) { asm நிலையற்ற("or 27,27,27" ::: "memory"); } } போது (0)
# elif defined __GNUC__ && defined __riscv
// PAUSE HINT is not part of RISC-V ISA yet, but is under discussion now. For details see:
// https://github.com/riscv/riscv-isa-manual/pull/398
// https://github.com/riscv/riscv-isa-manual/issues/43
// #   define CV_PAUSE(v) do { for (int __delay = (v); __delay > 0; --__delay) { asm volatile("pause"); } } while (0)
#   define CV_PAUSE(v) செய் { க்கு (முழு __delay = (v); __delay > 0; --__delay) { asm நிலையற்ற("nop"); } } போது (0)
# elif defined __GNUC__ && defined __loongarch__
#   define CV_PAUSE(v) செய் { க்கு (முழு __delay = (v); __delay > 0; --__delay) { asm நிலையற்ற("nop"); } } போது (0)
# இல்லையெனில்
#   warning "Can't detect 'pause' (CPU-yield) instruction on the target platform. Specify CV_PAUSE() definition via compiler flags."
#   define CV_PAUSE(...) செய் { /* no-op: works, but not effective */ } போது (0)
# endif
#endif // CV_PAUSE


நாமாகாஶ cv
{

நிலையான முழு CV_ACTIVE_WAIT_PAUSE_LIMIT = (முழு)utils::getConfigurationParameterSizeT("OPENCV_THREAD_POOL_ACTIVE_WAIT_PAUSE_LIMIT", 16);  // iterations
நிலையான முழு CV_WORKER_ACTIVE_WAIT = (முழு)utils::getConfigurationParameterSizeT("OPENCV_THREAD_POOL_ACTIVE_WAIT_WORKER", 2000);  // iterations
நிலையான முழு CV_MAIN_THREAD_ACTIVE_WAIT = (முழு)utils::getConfigurationParameterSizeT("OPENCV_THREAD_POOL_ACTIVE_WAIT_MAIN", 10000); // iterations

நிலையான முழு CV_WORKER_ACTIVE_WAIT_THREADS_LIMIT = (முழு)utils::getConfigurationParameterSizeT("OPENCV_THREAD_POOL_ACTIVE_WAIT_THREADS_LIMIT", 0); // number of real cores

வகுப்பு WorkerThread;
வகுப்பு ParallelJob;

வகுப்பு ThreadPool
{
பொது:
    நிலையான ThreadPool& instance()
    {
        CV_SINGLETON_LAZY_INIT_REF(ThreadPool, புதிய ThreadPool())
    }

    நிலையான வெறுமை stop()
    {
        ThreadPool& manager = instance();
        manager.reconfigure(0);
    }

    வெறுமை reconfigure(unsigned new_threads_மொத்தம்)
    {
        எனில் (new_threads_மொத்தம் == threads.size())
            திரும்பு;
        pthread_mutex_lock(&mutex);
        reconfigure_(new_threads_மொத்தம்);
        pthread_mutex_unlock(&mutex);
    }
    தருக்க reconfigure_(unsigned new_threads_மொத்தம்); // internal implementation

    வெறுமை run(மாறிலி Range& range, மாறிலி ParallelLoopBody& body, இரட்டை nstripes);

    size_t get_இலக்கம்_Of_Threads();

    வெறுமை set_இலக்கம்_Of_Threads(unsigned n);

    ThreadPool();

    ~ThreadPool();

    unsigned இலக்கம்_threads;

    pthread_mutex_t mutex;  // guards fields (job/threads) from non-worker threads (concurrent parallel_for calls)
#எனில் defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
    pthread_cond_t cond_thread_wake;
#endif

    pthread_mutex_t mutex_notify;
    pthread_cond_t cond_thread_task_complete;

    std::vector< Ptr<WorkerThread> > threads;

    Ptr<ParallelJob> job;

#ifdef CV_PROFILE_THREADS
    இரட்டை tickFreq;
    int64 jobSubmitTime;
    கட்டமைப்பு ThreadStatistics
    {
        ThreadStatistics() : threadWait(0)
        {
            reset();
        }
        வெறுமை reset()
        {
            threadWake = 0;
            threadExecuteStart = 0;
            threadExecuteStop = 0;
            executedTasks = 0;
            keepActive = பொய்;
            threadPing = get_Tick_மொத்தம்();
        }
        int64 threadWait; // don't reset by default
        int64 threadPing; // don't reset by default
        int64 threadWake;
        int64 threadExecuteStart;
        int64 threadExecuteStop;
        int64 threadFree;
        unsigned executedTasks;
        தருக்க keepActive;

        int64 dummy_[8]; // separate cache lines

        வெறுமை dump(முழு id, int64 baseTime, இரட்டை tickFreq)
        {
            எனில் (id < 0)
                std::cout << "Main: ";
            இல்லையெனில்
                printf("T%03d: ", id + 2);
            printf("wait=% 10.1f   ping=% 6.1f",
                    threadWait > 0 ? (threadWait - baseTime) / tickFreq * 1e6 : -0.0,
                    threadPing > 0 ? (threadPing - baseTime) / tickFreq * 1e6 : -0.0);
            எனில் (threadWake > 0)
                printf("   wake=% 6.1f",
                    (threadWake > 0 ? (threadWake - baseTime) / tickFreq * 1e6 : -0.0));
            எனில் (threadExecuteStart > 0)
            {
                printf("   exec=% 6.1f - % 6.1f   tasksDone=%5u   free=% 6.1f",
                    (threadExecuteStart > 0 ? (threadExecuteStart - baseTime) / tickFreq * 1e6 : -0.0),
                    (threadExecuteStop > 0 ? (threadExecuteStop - baseTime) / tickFreq * 1e6 : -0.0),
                    executedTasks,
                    (threadFree > 0 ? (threadFree - baseTime) / tickFreq * 1e6 : -0.0));
                எனில் (id >= 0)
                    printf(" active=%s\n", keepActive ? "true" : "false");
                இல்லையெனில்
                    printf("\n");
            }
            இல்லையெனில்
                printf("   ------------------------------------------------------------------------------\n");
        }
    };
    ThreadStatistics threads_stat[CV_PROFILE_THREADS]; // 0 - main thread, 1..N - worker threads
#endif

};

வகுப்பு WorkerThread
{
பொது:
    ThreadPool& thread_pool;
    மாறிலி unsigned id;
    pthread_t posix_thread;
    தருக்க is_created;

    std::atomic<தருக்க> stop_thread;

    std::atomic<தருக்க> has_wake_signal;

    Ptr<ParallelJob> job;

    pthread_mutex_t mutex;
#எனில் !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
    நிலையற்ற தருக்க isActive;
    pthread_cond_t cond_thread_wake;
#endif

    WorkerThread(ThreadPool& thread_pool_, unsigned id_) :
        thread_pool(thread_pool_),
        id(id_),
        posix_thread(0),
        is_created(பொய்),
        stop_thread(பொய்),
        has_wake_signal(பொய்)
#எனில் !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
        , isActive(உண்மை)
#endif
    {
        CV_LOG_VERBOSE(வெற்று, 1, "MainThread: initializing new worker: " << id);
        முழு res = pthread_mutex_init(&mutex, வெற்று);
        எனில் (res != 0)
        {
            CV_LOG_ERROR(வெற்று, id << ": Can't create thread mutex: res = " << res);
            திரும்பு;
        }
#எனில் !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
        res = pthread_cond_init(&cond_thread_wake, வெற்று);
        எனில் (res != 0)
        {
            CV_LOG_ERROR(வெற்று, id << ": Can't create thread condition variable: res = " << res);
            திரும்பு;
        }
#endif
        res = pthread_create(&posix_thread, வெற்று, thread_loop_wrapper, (வெறுமை*)இது);
        எனில் (res != 0)
        {
            CV_LOG_ERROR(வெற்று, id << ": Can't spawn new thread: res = " << res);
        }
        இல்லையெனில்
        {
            is_created = உண்மை;
        }
    }

    ~WorkerThread()
    {
        CV_LOG_VERBOSE(வெற்று, 1, "MainThread: destroy worker thread: " << id);
        எனில் (is_created)
        {
            எனில் (!stop_thread)
            {
                pthread_mutex_lock(&mutex);  // to avoid signal miss due pre-check
                stop_thread = உண்மை;
                pthread_mutex_unlock(&mutex);
#எனில் defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
                pthread_cond_broadcast(&thread_pool.cond_thread_wake);
#இல்லையெனில்
                pthread_cond_signal(&cond_thread_wake);
#endif
            }
            pthread_join(posix_thread, வெற்று);
        }
#எனில் !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
        pthread_cond_destroy(&cond_thread_wake);
#endif
        pthread_mutex_destroy(&mutex);
    }

    வெறுமை thread_body();
    நிலையான வெறுமை* thread_loop_wrapper(வெறுமை* thread_object)
    {
#ifdef OPENCV_WITH_ITT
        __itt_thread_set_name(cv::format("OpenCVThread-%03d", cv::utils::getThreadID()).c_str());
#endif
        ((WorkerThread*)thread_object)->thread_body();
        திரும்பு 0;
    }
};

வகுப்பு ParallelJob
{
பொது:
    ParallelJob(மாறிலி ThreadPool& thread_pool_, மாறிலி Range& range_, மாறிலி ParallelLoopBody& body_, முழு nstripes_) :
        thread_pool(thread_pool_),
        body(body_),
        range(range_),
        nstripes((unsigned)nstripes_),
        is_completed(பொய்)
    {
        CV_LOG_VERBOSE(வெற்று, 5, "ParallelJob::ParallelJob(" << (வெறுமை*)இது << ")");
        current_task.store(0, std::memory_order_relaxed);
        active_thread_மொத்தம்.store(0, std::memory_order_relaxed);
        completed_thread_மொத்தம்.store(0, std::memory_order_relaxed);
        dummy0_[0] = 0, dummy1_[0] = 0, dummy2_[0] = 0; // compiler warning
    }

    ~ParallelJob()
    {
        CV_LOG_VERBOSE(வெற்று, 5, "ParallelJob::~ParallelJob(" << (வெறுமை*)இது << ")");
    }

    unsigned execute(தருக்க is_worker_thread)
    {
        unsigned executed_tasks = 0;
        மாறிலி முழு task_மொத்தம் = range.size();
        மாறிலி முழு remaining_multiplier = std::min(nstripes,
                std::max(
                        std::min(100u, thread_pool.இலக்கம்_threads * 4),
                        thread_pool.இலக்கம்_threads * 2
                ));  // experimental value
        க்கு (;;)
        {
            முழு chunk_size = std::max(1, (task_மொத்தம் - current_task) / remaining_multiplier);
            முழு id = current_task.fetch_add(chunk_size, std::memory_order_seq_cst);
            எனில் (id >= task_மொத்தம்)
                நிறுத்து; // no more free tasks

            executed_tasks += chunk_size;
            முழு start_id = id;
            முழு end_id = std::min(task_மொத்தம், id + chunk_size);
            CV_LOG_VERBOSE(வெற்று, 9, "Thread: job " << start_id << "-" << end_id);

            //TODO: if (not pending exception)
            {
                body.operator()(Range(range.start + start_id, range.start + end_id));
            }
            எனில் (is_worker_thread && is_completed)
            {
                CV_LOG_ERROR(வெற்று, "\t\t\t\tBUG! Job: " << (வெறுமை*)இது << " " << id << " " << active_thread_மொத்தம் << " " << completed_thread_மொத்தம்);
                CV_Assert(!is_completed); // TODO Dbg this
            }
        }
        திரும்பு executed_tasks;
    }

    மாறிலி ThreadPool& thread_pool;
    மாறிலி ParallelLoopBody& body;
    மாறிலி Range range;
    மாறிலி unsigned nstripes;

    std::atomic<முழு> current_task;  // next free part of job
    int64 dummy0_[8];  // avoid cache-line reusing for the same atomics

    std::atomic<முழு> active_thread_மொத்தம்;  // number of threads worked on this job
    int64 dummy1_[8];  // avoid cache-line reusing for the same atomics

    std::atomic<முழு> completed_thread_மொத்தம்;  // number of threads completed any activities on this job
    int64 dummy2_[8];  // avoid cache-line reusing for the same atomics

    std::atomic<தருக்க> is_completed;

    // TODO exception handling
};


// Disable thread sanitization check when CV_USE_GLOBAL_WORKERS_COND_VAR is not
// set because it triggers as the main thread reads isActive while the children
// thread writes it (but it all works out because a mutex is locked in the main
// thread and isActive re-checked).
// This is to solve issue #19463.
#எனில் !defined(CV_USE_GLOBAL_WORKERS_COND_VAR) && defined(__clang__) && defined(__has_feature)
#எனில் __has_feature(thread_sanitizer)
__attribute__((no_sanitize("thread")))
#endif
#endif
வெறுமை WorkerThread::thread_body()
{
    (வெறுமை)cv::utils::getThreadID(); // notify OpenCV about new thread
    CV_LOG_VERBOSE(வெற்று, 5, "Thread: new thread: " << id);

    தருக்க allow_active_wait = உண்மை;

#ifdef CV_PROFILE_THREADS
    ThreadPool::ThreadStatistics& stat = thread_pool.threads_stat[id + 1];
#endif

    போது (!stop_thread)
    {
        CV_LOG_VERBOSE(வெற்று, 5, "Thread: ... loop iteration: allow_active_wait=" << allow_active_wait << "   has_wake_signal=" << has_wake_signal);
        எனில் (allow_active_wait && CV_WORKER_ACTIVE_WAIT > 0)
        {
            allow_active_wait = பொய்;
            க்கு (முழு i = 0; i < CV_WORKER_ACTIVE_WAIT; i++)
            {
                எனில் (has_wake_signal)
                    நிறுத்து;
                எனில் (CV_ACTIVE_WAIT_PAUSE_LIMIT > 0 && (i < CV_ACTIVE_WAIT_PAUSE_LIMIT || (i & 1)))
                    CV_PAUSE(16);
                இல்லையெனில்
                    CV_YIELD();
            }
        }
        pthread_mutex_lock(&mutex);
#ifdef CV_PROFILE_THREADS
        stat.threadWait = get_Tick_மொத்தம்();
#endif
        போது (!has_wake_signal) // to handle spurious wakeups
        {
            //CV_LOG_VERBOSE(NULL, 5, "Thread: wait (sleep) ...");
#எனில் defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
            pthread_cond_wait(&thread_pool.cond_thread_wake, &mutex);
#இல்லையெனில்
            isActive = பொய்;
            pthread_cond_wait(&cond_thread_wake, &mutex);
            isActive = உண்மை;
#endif
            CV_LOG_VERBOSE(வெற்று, 5, "Thread: wake ... (has_wake_signal=" << has_wake_signal << " stop_thread=" << stop_thread << ")")
        }
#ifdef CV_PROFILE_THREADS
        stat.threadWake = get_Tick_மொத்தம்();
#endif

        CV_LOG_VERBOSE(வெற்று, 5, "Thread: checking for new job");
        எனில் (CV_WORKER_ACTIVE_WAIT_THREADS_LIMIT == 0)
            allow_active_wait = உண்மை;
        Ptr<ParallelJob> j_ptr; swap(j_ptr, job);
        has_wake_signal = பொய்;
        pthread_mutex_unlock(&mutex);

        எனில் (!stop_thread)
        {
            ParallelJob* j = j_ptr;
            எனில் (j)
            {
                CV_LOG_VERBOSE(வெற்று, 5, "Thread: job size=" << j->range.size() << " done=" << j->current_task);
                எனில் (j->current_task < j->range.size())
                {
                    முழு other = j->active_thread_மொத்தம்.fetch_add(1, std::memory_order_seq_cst);
                    CV_LOG_VERBOSE(வெற்று, 5, "Thread: processing new job (with " << other << " other threads)"); CV_UNUSED(other);
#ifdef CV_PROFILE_THREADS
                    stat.threadExecuteStart = get_Tick_மொத்தம்();
                    stat.executedTasks = j->execute(உண்மை);
                    stat.threadExecuteStop = get_Tick_மொத்தம்();
#இல்லையெனில்
                    j->execute(உண்மை);
#endif
                    முழு completed = j->completed_thread_மொத்தம்.fetch_add(1, std::memory_order_seq_cst) + 1;
                    முழு active = j->active_thread_மொத்தம்.load(std::memory_order_acquire);
                    எனில் (CV_WORKER_ACTIVE_WAIT_THREADS_LIMIT > 0)
                    {
                        allow_active_wait = உண்மை;
                        எனில் (active >= CV_WORKER_ACTIVE_WAIT_THREADS_LIMIT && (id & 1) == 0) // turn off a half of threads
                            allow_active_wait = பொய்;
                    }
                    CV_LOG_VERBOSE(வெற்று, 5, "Thread: completed job processing: " << active << " " << completed);
                    எனில் (active == completed)
                    {
                        தருக்க need_signal = !j->is_completed;
                        j->is_completed = உண்மை;
                        j = வெற்று; j_ptr.release();
                        எனில் (need_signal)
                        {
                            CV_LOG_VERBOSE(வெற்று, 5, "Thread: job finished => notifying the main thread");
                            pthread_mutex_lock(&thread_pool.mutex_notify);  // to avoid signal miss due pre-check condition
                            // empty
                            pthread_mutex_unlock(&thread_pool.mutex_notify);
                            pthread_cond_broadcast/*pthread_cond_signal*/(&thread_pool.cond_thread_task_complete);
                        }
                    }
                }
                இல்லையெனில்
                {
                    CV_LOG_VERBOSE(வெற்று, 5, "Thread: no free job tasks");
                }
            }
        }
#ifdef CV_PROFILE_THREADS
        stat.threadFree = get_Tick_மொத்தம்();
        stat.keepActive = allow_active_wait;
#endif
    }
}

ThreadPool::ThreadPool()
{
#ifdef CV_PROFILE_THREADS
    tickFreq = getTickFrequency();
#endif

    முழு res = 0;
    res |= pthread_mutex_init(&mutex, வெற்று);
    res |= pthread_mutex_init(&mutex_notify, வெற்று);
#எனில் defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
    res |= pthread_cond_init(&cond_thread_wake, வெற்று);
#endif
    res |= pthread_cond_init(&cond_thread_task_complete, வெற்று);

    எனில் (0 != res)
    {
        CV_LOG_FATAL(வெற்று, "Failed to initialize ThreadPool (pthreads)");
    }
    இலக்கம்_threads = default_எண்_Of_Threads();
}

தருக்க ThreadPool::reconfigure_(unsigned new_threads_மொத்தம்)
{
    எனில் (new_threads_மொத்தம் == threads.size())
        திரும்பு பொய்;

    எனில் (new_threads_மொத்தம் < threads.size())
    {
        CV_LOG_VERBOSE(வெற்று, 1, "MainThread: reduce worker pool: " << threads.size() << " => " << new_threads_மொத்தம்);
        std::vector< Ptr<WorkerThread> > release_threads(threads.size() - new_threads_மொத்தம்);
        க்கு (size_t i = new_threads_மொத்தம்; i < threads.size(); ++i)
        {
            pthread_mutex_lock(&threads[i]->mutex);  // to avoid signal miss due pre-check
            threads[i]->stop_thread = உண்மை;
            threads[i]->has_wake_signal = உண்மை;
#எனில் !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
            pthread_mutex_unlock(&threads[i]->mutex);
            pthread_cond_broadcast/*pthread_cond_signal*/(&threads[i]->cond_thread_wake); // wake thread
#இல்லையெனில்
            pthread_mutex_unlock(&threads[i]->mutex);
#endif
            std::swap(threads[i], release_threads[i - new_threads_மொத்தம்]);
        }
#எனில் defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
        CV_LOG_VERBOSE(வெற்று, 1, "MainThread: notify worker threads about termination...");
        pthread_cond_broadcast(&cond_thread_wake); // wake all threads
#endif
        threads.resize(new_threads_மொத்தம்);
        release_threads.clear();  // calls thread_join which want to lock mutex
        திரும்பு பொய்;
    }
    இல்லையெனில்
    {
        CV_LOG_VERBOSE(வெற்று, 1, "MainThread: upgrade worker pool: " << threads.size() << " => " << new_threads_மொத்தம்);
        க்கு (size_t i = threads.size(); i < new_threads_மொத்தம்; ++i)
        {
            threads.push_back(Ptr<WorkerThread>(புதிய WorkerThread(*இது, (unsigned)i))); // spawn more threads
        }
    }
    திரும்பு பொய்;
}

ThreadPool::~ThreadPool()
{
    reconfigure(0);
    pthread_cond_destroy(&cond_thread_task_complete);
#எனில் defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
    pthread_cond_destroy(&cond_thread_wake);
#endif
    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&mutex_notify);
}

வெறுமை ThreadPool::run(மாறிலி Range& range, மாறிலி ParallelLoopBody& body, இரட்டை nstripes)
{
    CV_LOG_VERBOSE(வெற்று, 1, "MainThread: new parallel job: num_threads=" << இலக்கம்_threads << "   range=" << range.size() << "   nstripes=" << nstripes << "   job=" << (வெறுமை*)job);
#ifdef CV_PROFILE_THREADS
    jobSubmitTime = get_Tick_மொத்தம்();
    threads_stat[0].reset();
    threads_stat[0].threadWait = jobSubmitTime;
    threads_stat[0].threadWake = jobSubmitTime;
#endif
    எனில் (get_இலக்கம்_Of_Threads() > 1 &&
        job == வெற்று &&
        (range.size() * nstripes >= 2 || (range.size() > 1 && nstripes <= 0))
    )
    {
        pthread_mutex_lock(&mutex);
        எனில் (job != வெற்று)
        {
            pthread_mutex_unlock(&mutex);
            body(range);
            திரும்பு;
        }
        reconfigure_(இலக்கம்_threads - 1);

        {
            CV_LOG_VERBOSE(வெற்று, 1, "MainThread: initialize parallel job: " << range.size());
            job = Ptr<ParallelJob>(புதிய ParallelJob(*இது, range, body, nstripes));
            pthread_mutex_unlock(&mutex);

            CV_LOG_VERBOSE(வெற்று, 5, "MainThread: wake worker threads...");
            size_t இலக்கம்_threads_to_wake = std::min(static_cast<size_t>(range.size()), threads.size());
            க்கு (size_t i = 0; i < இலக்கம்_threads_to_wake; ++i)
            {
                எனில் (job->current_task >= job->range.size())
                    நிறுத்து;
                WorkerThread& thread = *(threads[i].get());
                எனில் (
#எனில் defined(__clang__) && defined(__has_feature)
#எனில் __has_feature(thread_sanitizer)
                        1 ||  // Robust workaround to avoid data race warning of `thread.job`
#endif
#endif
#எனில் !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
                        thread.isActive ||
#endif
                        thread.has_wake_signal
                        || !thread.job.empty()  // #10881
                )
                {
                    pthread_mutex_lock(&thread.mutex);
                    thread.job = job;
#எனில் !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
                    தருக்க isActive = thread.isActive;
#endif
                    thread.has_wake_signal = உண்மை;
#ifdef CV_PROFILE_THREADS
                    threads_stat[i + 1].reset();
#endif
                    pthread_mutex_unlock(&thread.mutex);
#எனில் !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
                    எனில் (!isActive)
                    {
                        pthread_cond_broadcast/*pthread_cond_signal*/(&thread.cond_thread_wake); // wake thread
                    }
#endif
                }
                இல்லையெனில்
                {
                    CV_Assert(thread.job.empty());
                    thread.job = job;
                    thread.has_wake_signal = உண்மை;
#ifdef CV_PROFILE_THREADS
                    threads_stat[i + 1].reset();
#endif
#எனில் !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
                    pthread_cond_broadcast/*pthread_cond_signal*/(&thread.cond_thread_wake); // wake thread
#endif
                }
            }
#ifdef CV_PROFILE_THREADS
            threads_stat[0].threadPing = get_Tick_மொத்தம்();
#endif
#எனில் defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
            pthread_cond_broadcast(&cond_thread_wake); // wake all threads
#endif
#ifdef CV_PROFILE_THREADS
            threads_stat[0].threadWake = get_Tick_மொத்தம்();
#endif
            CV_LOG_VERBOSE(வெற்று, 5, "MainThread: wake worker threads... (done)");

            {
                ParallelJob& j = *(இது->job);
#ifdef CV_PROFILE_THREADS
                threads_stat[0].threadExecuteStart = get_Tick_மொத்தம்();
                threads_stat[0].executedTasks = j.execute(பொய்);
                threads_stat[0].threadExecuteStop = get_Tick_மொத்தம்();
#இல்லையெனில்
                j.execute(பொய்);
#endif
                CV_Assert(j.current_task >= j.range.size());
                CV_LOG_VERBOSE(வெற்று, 5, "MainThread: complete self-tasks: " << j.active_thread_மொத்தம் << " " << j.completed_thread_மொத்தம்);
                எனில் (job->is_completed || j.active_thread_மொத்தம் == 0)
                {
                    job->is_completed = உண்மை;
                    CV_LOG_VERBOSE(வெற்று, 5, "MainThread: no WIP worker threads");
                }
                இல்லையெனில்
                {
                    எனில் (CV_MAIN_THREAD_ACTIVE_WAIT > 0)
                    {
                        க்கு (முழு i = 0; i < CV_MAIN_THREAD_ACTIVE_WAIT; i++)  // don't spin too much in any case (inaccurate getTickCount())
                        {
                            எனில் (job->is_completed)
                            {
                                CV_LOG_VERBOSE(வெற்று, 5, "MainThread: job finalize (active wait) " << j.active_thread_மொத்தம் << " " << j.completed_thread_மொத்தம்);
                                நிறுத்து;
                            }
                            எனில் (CV_ACTIVE_WAIT_PAUSE_LIMIT > 0 && (i < CV_ACTIVE_WAIT_PAUSE_LIMIT || (i & 1)))
                                CV_PAUSE(16);
                            இல்லையெனில்
                                CV_YIELD();
                        }
                    }
                    எனில் (!job->is_completed)
                    {
                        CV_LOG_VERBOSE(வெற்று, 5, "MainThread: prepare wait " << j.active_thread_மொத்தம் << " " << j.completed_thread_மொத்தம்);
                        pthread_mutex_lock(&mutex_notify);
                        க்கு (;;)
                        {
                            எனில் (job->is_completed)
                            {
                                CV_LOG_VERBOSE(வெற்று, 5, "MainThread: job finalize (wait) " << j.active_thread_மொத்தம் << " " << j.completed_thread_மொத்தம்);
                                நிறுத்து;
                            }
                            CV_LOG_VERBOSE(வெற்று, 5, "MainThread: wait completion (sleep) ...");
                            pthread_cond_wait(&cond_thread_task_complete, &mutex_notify);
                            CV_LOG_VERBOSE(வெற்று, 5, "MainThread: wake");
                        }
                        pthread_mutex_unlock(&mutex_notify);
                    }
                }
            }
#ifdef CV_PROFILE_THREADS
            threads_stat[0].threadFree = get_Tick_மொத்தம்();
            std::cout << "Job: sz=" << range.size() << " nstripes=" << nstripes << "    Time: " << (threads_stat[0].threadFree - jobSubmitTime) / tickFreq * 1e6 << " usec" << std::endl;
            க்கு (முழு i = 0; i < (முழு)threads.size() + 1; i++)
            {
                threads_stat[i].dump(i - 1, jobSubmitTime, tickFreq);
            }
#endif
            எனில் (job)
            {
                pthread_mutex_lock(&mutex);
                CV_LOG_VERBOSE(வெற்று, 5, "MainThread: job release");
                CV_Assert(job->is_completed);
                job.release();
                pthread_mutex_unlock(&mutex);
            }
        }
    }
    இல்லையெனில்
    {
        body(range);
    }
}

size_t ThreadPool::get_இலக்கம்_Of_Threads()
{
    திரும்பு இலக்கம்_threads;
}

வெறுமை ThreadPool::set_இலக்கம்_Of_Threads(unsigned n)
{
    எனில் (n != இலக்கம்_threads)
    {
        இலக்கம்_threads = n;
        எனில் (n == 1)
           எனில் (job == வெற்று) reconfigure(0);  // stop worker threads immediately
    }
}

size_t parallel_pthreads_get_threads_இலக்கம்()
{
    திரும்பு ThreadPool::instance().get_இலக்கம்_Of_Threads();
}

வெறுமை parallel_pthreads_set_threads_இலக்கம்(முழு num)
{
    எனில்(num < 0)
    {
        ThreadPool::instance().set_இலக்கம்_Of_Threads(0);
    }
    இல்லையெனில்
    {
        ThreadPool::instance().set_இலக்கம்_Of_Threads(unsigned(num));
    }
}

வெறுமை parallel_for_pthreads(மாறிலி Range& range, மாறிலி ParallelLoopBody& body, இரட்டை nstripes)
{
    ThreadPool::instance().run(range, body, nstripes);
}

}

#endif
