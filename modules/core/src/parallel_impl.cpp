// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#अन्तर्भाव "precomp.hpp"

#अन्तर्भाव "parallel_impl.hpp"

#ifdef HAVE_PTHREADS_PF
#अन्तर्भाव <pthread.h>

#अन्तर्भाव <opencv2/core/utils/configuration.private.hpp>

#अन्तर्भाव <opencv2/core/utils/logger.defines.hpp>
//#undef CV_LOG_STRIP_LEVEL
//#define CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_VERBOSE + 1
#अन्तर्भाव <opencv2/core/utils/logger.hpp>

#अन्तर्भाव <opencv2/core/utils/trace.private.hpp>

//#define CV_PROFILE_THREADS 64
//#define getTickCount getCPUTickCount  // use this if getTickCount() calls are expensive (and getCPUTickCount() is accurate)

//#define CV_USE_GLOBAL_WORKERS_COND_VAR  // not effective on many-core systems (10+)

#अन्तर्भाव <atomic>

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
# यदि defined __GNUC__ && (defined __i386__ || defined __x86_64__)
#   include <x86intrin.h> /* for __rdtsc */
#   यदि !defined(__SSE2__)
      स्थिर अन्तःस्थ शून्य cv_non_sse_mm_pause() { __asm__ __volatile__ ("rep; nop"); }
#     define _mm_pause cv_non_sse_mm_pause
#   endif
// With Skylake CPUs and above, _mm_pause takes 140 cycles so no need for a loop.
#   define CV_PAUSE(v) कुरु { (शून्य)v; _mm_pause(); } यावत् (0)
# elif defined __GNUC__ && defined __aarch64__
#   define CV_PAUSE(v) कुरु { प्रति (पूर्णाङ्क __delay = (v); __delay > 0; --__delay) { asm चञ्चल("yield" ::: "memory"); } } यावत् (0)
# elif defined __GNUC__ && defined __arm__
#   define CV_PAUSE(v) कुरु { प्रति (पूर्णाङ्क __delay = (v); __delay > 0; --__delay) { asm चञ्चल("" ::: "memory"); } } यावत् (0)
# elif defined __GNUC__ && defined __mips__ && __mips_isa_rev >= 2
#   define CV_PAUSE(v) कुरु { प्रति (पूर्णाङ्क __delay = (v); __delay > 0; --__delay) { asm चञ्चल("pause" ::: "memory"); } } यावत् (0)
# elif defined __APPLE__ && defined __POWERPC__
#   define CV_PAUSE(v) कुरु { प्रति (पूर्णाङ्क __delay = (v); __delay > 0; --__delay) { asm चञ्चल("or r27,r27,r27" ::: "memory"); } } यावत् (0)
# elif defined __GNUC__ && defined __PPC64__
#   define CV_PAUSE(v) कुरु { प्रति (पूर्णाङ्क __delay = (v); __delay > 0; --__delay) { asm चञ्चल("or 27,27,27" ::: "memory"); } } यावत् (0)
# elif defined __GNUC__ && defined __riscv
// PAUSE HINT is not part of RISC-V ISA yet, but is under discussion now. For details see:
// https://github.com/riscv/riscv-isa-manual/pull/398
// https://github.com/riscv/riscv-isa-manual/issues/43
// #   define CV_PAUSE(v) do { for (int __delay = (v); __delay > 0; --__delay) { asm volatile("pause"); } } while (0)
#   define CV_PAUSE(v) कुरु { प्रति (पूर्णाङ्क __delay = (v); __delay > 0; --__delay) { asm चञ्चल("nop"); } } यावत् (0)
# elif defined __GNUC__ && defined __loongarch__
#   define CV_PAUSE(v) कुरु { प्रति (पूर्णाङ्क __delay = (v); __delay > 0; --__delay) { asm चञ्चल("nop"); } } यावत् (0)
# अन्यथा
#   warning "Can't detect 'pause' (CPU-yield) instruction on the target platform. Specify CV_PAUSE() definition via compiler flags."
#   define CV_PAUSE(...) कुरु { /* no-op: works, but not effective */ } यावत् (0)
# endif
#endif // CV_PAUSE


नामाकाश cv
{

स्थिर पूर्णाङ्क CV_ACTIVE_WAIT_PAUSE_LIMIT = (पूर्णाङ्क)utils::getConfigurationParameterSizeT("OPENCV_THREAD_POOL_ACTIVE_WAIT_PAUSE_LIMIT", 16);  // iterations
स्थिर पूर्णाङ्क CV_WORKER_ACTIVE_WAIT = (पूर्णाङ्क)utils::getConfigurationParameterSizeT("OPENCV_THREAD_POOL_ACTIVE_WAIT_WORKER", 2000);  // iterations
स्थिर पूर्णाङ्क CV_MAIN_THREAD_ACTIVE_WAIT = (पूर्णाङ्क)utils::getConfigurationParameterSizeT("OPENCV_THREAD_POOL_ACTIVE_WAIT_MAIN", 10000); // iterations

स्थिर पूर्णाङ्क CV_WORKER_ACTIVE_WAIT_THREADS_LIMIT = (पूर्णाङ्क)utils::getConfigurationParameterSizeT("OPENCV_THREAD_POOL_ACTIVE_WAIT_THREADS_LIMIT", 0); // number of real cores

वर्ग WorkerThread;
वर्ग ParallelJob;

वर्ग ThreadPool
{
सार्वजनिक:
    स्थिर ThreadPool& instance()
    {
        CV_SINGLETON_LAZY_INIT_REF(ThreadPool, नूतन ThreadPool())
    }

    स्थिर शून्य stop()
    {
        ThreadPool& manager = instance();
        manager.reconfigure(0);
    }

    शून्य reconfigure(unsigned new_threads_मोजणी)
    {
        यदि (new_threads_मोजणी == threads.size())
            प्रत्यागम;
        pthread_mutex_lock(&mutex);
        reconfigure_(new_threads_मोजणी);
        pthread_mutex_unlock(&mutex);
    }
    तार्किक reconfigure_(unsigned new_threads_मोजणी); // internal implementation

    शून्य run(ध्रुव Range& range, ध्रुव ParallelLoopBody& body, द्विगुण nstripes);

    size_t get_अंक_Of_Threads();

    शून्य set_अंक_Of_Threads(unsigned n);

    ThreadPool();

    ~ThreadPool();

    unsigned अंक_threads;

    pthread_mutex_t mutex;  // guards fields (job/threads) from non-worker threads (concurrent parallel_for calls)
#यदि defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
    pthread_cond_t cond_thread_wake;
#endif

    pthread_mutex_t mutex_notify;
    pthread_cond_t cond_thread_task_complete;

    std::vector< Ptr<WorkerThread> > threads;

    Ptr<ParallelJob> job;

#ifdef CV_PROFILE_THREADS
    द्विगुण tickFreq;
    int64 jobSubmitTime;
    संरचना ThreadStatistics
    {
        ThreadStatistics() : threadWait(0)
        {
            reset();
        }
        शून्य reset()
        {
            threadWake = 0;
            threadExecuteStart = 0;
            threadExecuteStop = 0;
            executedTasks = 0;
            keepActive = असत्य;
            threadPing = get_Tick_मोजणी();
        }
        int64 threadWait; // don't reset by default
        int64 threadPing; // don't reset by default
        int64 threadWake;
        int64 threadExecuteStart;
        int64 threadExecuteStop;
        int64 threadFree;
        unsigned executedTasks;
        तार्किक keepActive;

        int64 dummy_[8]; // separate cache lines

        शून्य dump(पूर्णाङ्क id, int64 baseTime, द्विगुण tickFreq)
        {
            यदि (id < 0)
                std::cout << "Main: ";
            अन्यथा
                printf("T%03d: ", id + 2);
            printf("wait=% 10.1f   ping=% 6.1f",
                    threadWait > 0 ? (threadWait - baseTime) / tickFreq * 1e6 : -0.0,
                    threadPing > 0 ? (threadPing - baseTime) / tickFreq * 1e6 : -0.0);
            यदि (threadWake > 0)
                printf("   wake=% 6.1f",
                    (threadWake > 0 ? (threadWake - baseTime) / tickFreq * 1e6 : -0.0));
            यदि (threadExecuteStart > 0)
            {
                printf("   exec=% 6.1f - % 6.1f   tasksDone=%5u   free=% 6.1f",
                    (threadExecuteStart > 0 ? (threadExecuteStart - baseTime) / tickFreq * 1e6 : -0.0),
                    (threadExecuteStop > 0 ? (threadExecuteStop - baseTime) / tickFreq * 1e6 : -0.0),
                    executedTasks,
                    (threadFree > 0 ? (threadFree - baseTime) / tickFreq * 1e6 : -0.0));
                यदि (id >= 0)
                    printf(" active=%s\n", keepActive ? "true" : "false");
                अन्यथा
                    printf("\n");
            }
            अन्यथा
                printf("   ------------------------------------------------------------------------------\n");
        }
    };
    ThreadStatistics threads_stat[CV_PROFILE_THREADS]; // 0 - main thread, 1..N - worker threads
#endif

};

वर्ग WorkerThread
{
सार्वजनिक:
    ThreadPool& thread_pool;
    ध्रुव unsigned id;
    pthread_t posix_thread;
    तार्किक is_created;

    std::atomic<तार्किक> stop_thread;

    std::atomic<तार्किक> has_wake_signal;

    Ptr<ParallelJob> job;

    pthread_mutex_t mutex;
#यदि !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
    चञ्चल तार्किक isActive;
    pthread_cond_t cond_thread_wake;
#endif

    WorkerThread(ThreadPool& thread_pool_, unsigned id_) :
        thread_pool(thread_pool_),
        id(id_),
        posix_thread(0),
        is_created(असत्य),
        stop_thread(असत्य),
        has_wake_signal(असत्य)
#यदि !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
        , isActive(सत्य)
#endif
    {
        CV_LOG_VERBOSE(रिक्त, 1, "MainThread: initializing new worker: " << id);
        पूर्णाङ्क res = pthread_mutex_init(&mutex, रिक्त);
        यदि (res != 0)
        {
            CV_LOG_ERROR(रिक्त, id << ": Can't create thread mutex: res = " << res);
            प्रत्यागम;
        }
#यदि !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
        res = pthread_cond_init(&cond_thread_wake, रिक्त);
        यदि (res != 0)
        {
            CV_LOG_ERROR(रिक्त, id << ": Can't create thread condition variable: res = " << res);
            प्रत्यागम;
        }
#endif
        res = pthread_create(&posix_thread, रिक्त, thread_loop_wrapper, (शून्य*)एतत्);
        यदि (res != 0)
        {
            CV_LOG_ERROR(रिक्त, id << ": Can't spawn new thread: res = " << res);
        }
        अन्यथा
        {
            is_created = सत्य;
        }
    }

    ~WorkerThread()
    {
        CV_LOG_VERBOSE(रिक्त, 1, "MainThread: destroy worker thread: " << id);
        यदि (is_created)
        {
            यदि (!stop_thread)
            {
                pthread_mutex_lock(&mutex);  // to avoid signal miss due pre-check
                stop_thread = सत्य;
                pthread_mutex_unlock(&mutex);
#यदि defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
                pthread_cond_broadcast(&thread_pool.cond_thread_wake);
#अन्यथा
                pthread_cond_signal(&cond_thread_wake);
#endif
            }
            pthread_join(posix_thread, रिक्त);
        }
#यदि !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
        pthread_cond_destroy(&cond_thread_wake);
#endif
        pthread_mutex_destroy(&mutex);
    }

    शून्य thread_body();
    स्थिर शून्य* thread_loop_wrapper(शून्य* thread_object)
    {
#ifdef OPENCV_WITH_ITT
        __itt_thread_set_name(cv::format("OpenCVThread-%03d", cv::utils::getThreadID()).c_str());
#endif
        ((WorkerThread*)thread_object)->thread_body();
        प्रत्यागम 0;
    }
};

वर्ग ParallelJob
{
सार्वजनिक:
    ParallelJob(ध्रुव ThreadPool& thread_pool_, ध्रुव Range& range_, ध्रुव ParallelLoopBody& body_, पूर्णाङ्क nstripes_) :
        thread_pool(thread_pool_),
        body(body_),
        range(range_),
        nstripes((unsigned)nstripes_),
        is_completed(असत्य)
    {
        CV_LOG_VERBOSE(रिक्त, 5, "ParallelJob::ParallelJob(" << (शून्य*)एतत् << ")");
        current_task.store(0, std::memory_order_relaxed);
        active_thread_मोजणी.store(0, std::memory_order_relaxed);
        completed_thread_मोजणी.store(0, std::memory_order_relaxed);
        dummy0_[0] = 0, dummy1_[0] = 0, dummy2_[0] = 0; // compiler warning
    }

    ~ParallelJob()
    {
        CV_LOG_VERBOSE(रिक्त, 5, "ParallelJob::~ParallelJob(" << (शून्य*)एतत् << ")");
    }

    unsigned execute(तार्किक is_worker_thread)
    {
        unsigned executed_tasks = 0;
        ध्रुव पूर्णाङ्क task_मोजणी = range.size();
        ध्रुव पूर्णाङ्क remaining_multiplier = std::min(nstripes,
                std::max(
                        std::min(100u, thread_pool.अंक_threads * 4),
                        thread_pool.अंक_threads * 2
                ));  // experimental value
        प्रति (;;)
        {
            पूर्णाङ्क chunk_size = std::max(1, (task_मोजणी - current_task) / remaining_multiplier);
            पूर्णाङ्क id = current_task.fetch_add(chunk_size, std::memory_order_seq_cst);
            यदि (id >= task_मोजणी)
                भङ्ग; // no more free tasks

            executed_tasks += chunk_size;
            पूर्णाङ्क start_id = id;
            पूर्णाङ्क end_id = std::min(task_मोजणी, id + chunk_size);
            CV_LOG_VERBOSE(रिक्त, 9, "Thread: job " << start_id << "-" << end_id);

            //TODO: if (not pending exception)
            {
                body.operator()(Range(range.start + start_id, range.start + end_id));
            }
            यदि (is_worker_thread && is_completed)
            {
                CV_LOG_ERROR(रिक्त, "\t\t\t\tBUG! Job: " << (शून्य*)एतत् << " " << id << " " << active_thread_मोजणी << " " << completed_thread_मोजणी);
                CV_Assert(!is_completed); // TODO Dbg this
            }
        }
        प्रत्यागम executed_tasks;
    }

    ध्रुव ThreadPool& thread_pool;
    ध्रुव ParallelLoopBody& body;
    ध्रुव Range range;
    ध्रुव unsigned nstripes;

    std::atomic<पूर्णाङ्क> current_task;  // next free part of job
    int64 dummy0_[8];  // avoid cache-line reusing for the same atomics

    std::atomic<पूर्णाङ्क> active_thread_मोजणी;  // number of threads worked on this job
    int64 dummy1_[8];  // avoid cache-line reusing for the same atomics

    std::atomic<पूर्णाङ्क> completed_thread_मोजणी;  // number of threads completed any activities on this job
    int64 dummy2_[8];  // avoid cache-line reusing for the same atomics

    std::atomic<तार्किक> is_completed;

    // TODO exception handling
};


// Disable thread sanitization check when CV_USE_GLOBAL_WORKERS_COND_VAR is not
// set because it triggers as the main thread reads isActive while the children
// thread writes it (but it all works out because a mutex is locked in the main
// thread and isActive re-checked).
// This is to solve issue #19463.
#यदि !defined(CV_USE_GLOBAL_WORKERS_COND_VAR) && defined(__clang__) && defined(__has_feature)
#यदि __has_feature(thread_sanitizer)
__attribute__((no_sanitize("thread")))
#endif
#endif
शून्य WorkerThread::thread_body()
{
    (शून्य)cv::utils::getThreadID(); // notify OpenCV about new thread
    CV_LOG_VERBOSE(रिक्त, 5, "Thread: new thread: " << id);

    तार्किक allow_active_wait = सत्य;

#ifdef CV_PROFILE_THREADS
    ThreadPool::ThreadStatistics& stat = thread_pool.threads_stat[id + 1];
#endif

    यावत् (!stop_thread)
    {
        CV_LOG_VERBOSE(रिक्त, 5, "Thread: ... loop iteration: allow_active_wait=" << allow_active_wait << "   has_wake_signal=" << has_wake_signal);
        यदि (allow_active_wait && CV_WORKER_ACTIVE_WAIT > 0)
        {
            allow_active_wait = असत्य;
            प्रति (पूर्णाङ्क i = 0; i < CV_WORKER_ACTIVE_WAIT; i++)
            {
                यदि (has_wake_signal)
                    भङ्ग;
                यदि (CV_ACTIVE_WAIT_PAUSE_LIMIT > 0 && (i < CV_ACTIVE_WAIT_PAUSE_LIMIT || (i & 1)))
                    CV_PAUSE(16);
                अन्यथा
                    CV_YIELD();
            }
        }
        pthread_mutex_lock(&mutex);
#ifdef CV_PROFILE_THREADS
        stat.threadWait = get_Tick_मोजणी();
#endif
        यावत् (!has_wake_signal) // to handle spurious wakeups
        {
            //CV_LOG_VERBOSE(NULL, 5, "Thread: wait (sleep) ...");
#यदि defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
            pthread_cond_wait(&thread_pool.cond_thread_wake, &mutex);
#अन्यथा
            isActive = असत्य;
            pthread_cond_wait(&cond_thread_wake, &mutex);
            isActive = सत्य;
#endif
            CV_LOG_VERBOSE(रिक्त, 5, "Thread: wake ... (has_wake_signal=" << has_wake_signal << " stop_thread=" << stop_thread << ")")
        }
#ifdef CV_PROFILE_THREADS
        stat.threadWake = get_Tick_मोजणी();
#endif

        CV_LOG_VERBOSE(रिक्त, 5, "Thread: checking for new job");
        यदि (CV_WORKER_ACTIVE_WAIT_THREADS_LIMIT == 0)
            allow_active_wait = सत्य;
        Ptr<ParallelJob> j_ptr; swap(j_ptr, job);
        has_wake_signal = असत्य;
        pthread_mutex_unlock(&mutex);

        यदि (!stop_thread)
        {
            ParallelJob* j = j_ptr;
            यदि (j)
            {
                CV_LOG_VERBOSE(रिक्त, 5, "Thread: job size=" << j->range.size() << " done=" << j->current_task);
                यदि (j->current_task < j->range.size())
                {
                    पूर्णाङ्क other = j->active_thread_मोजणी.fetch_add(1, std::memory_order_seq_cst);
                    CV_LOG_VERBOSE(रिक्त, 5, "Thread: processing new job (with " << other << " other threads)"); CV_UNUSED(other);
#ifdef CV_PROFILE_THREADS
                    stat.threadExecuteStart = get_Tick_मोजणी();
                    stat.executedTasks = j->execute(सत्य);
                    stat.threadExecuteStop = get_Tick_मोजणी();
#अन्यथा
                    j->execute(सत्य);
#endif
                    पूर्णाङ्क completed = j->completed_thread_मोजणी.fetch_add(1, std::memory_order_seq_cst) + 1;
                    पूर्णाङ्क active = j->active_thread_मोजणी.load(std::memory_order_acquire);
                    यदि (CV_WORKER_ACTIVE_WAIT_THREADS_LIMIT > 0)
                    {
                        allow_active_wait = सत्य;
                        यदि (active >= CV_WORKER_ACTIVE_WAIT_THREADS_LIMIT && (id & 1) == 0) // turn off a half of threads
                            allow_active_wait = असत्य;
                    }
                    CV_LOG_VERBOSE(रिक्त, 5, "Thread: completed job processing: " << active << " " << completed);
                    यदि (active == completed)
                    {
                        तार्किक need_signal = !j->is_completed;
                        j->is_completed = सत्य;
                        j = रिक्त; j_ptr.release();
                        यदि (need_signal)
                        {
                            CV_LOG_VERBOSE(रिक्त, 5, "Thread: job finished => notifying the main thread");
                            pthread_mutex_lock(&thread_pool.mutex_notify);  // to avoid signal miss due pre-check condition
                            // empty
                            pthread_mutex_unlock(&thread_pool.mutex_notify);
                            pthread_cond_broadcast/*pthread_cond_signal*/(&thread_pool.cond_thread_task_complete);
                        }
                    }
                }
                अन्यथा
                {
                    CV_LOG_VERBOSE(रिक्त, 5, "Thread: no free job tasks");
                }
            }
        }
#ifdef CV_PROFILE_THREADS
        stat.threadFree = get_Tick_मोजणी();
        stat.keepActive = allow_active_wait;
#endif
    }
}

ThreadPool::ThreadPool()
{
#ifdef CV_PROFILE_THREADS
    tickFreq = getTickFrequency();
#endif

    पूर्णाङ्क res = 0;
    res |= pthread_mutex_init(&mutex, रिक्त);
    res |= pthread_mutex_init(&mutex_notify, रिक्त);
#यदि defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
    res |= pthread_cond_init(&cond_thread_wake, रिक्त);
#endif
    res |= pthread_cond_init(&cond_thread_task_complete, रिक्त);

    यदि (0 != res)
    {
        CV_LOG_FATAL(रिक्त, "Failed to initialize ThreadPool (pthreads)");
    }
    अंक_threads = default_संख्या_Of_Threads();
}

तार्किक ThreadPool::reconfigure_(unsigned new_threads_मोजणी)
{
    यदि (new_threads_मोजणी == threads.size())
        प्रत्यागम असत्य;

    यदि (new_threads_मोजणी < threads.size())
    {
        CV_LOG_VERBOSE(रिक्त, 1, "MainThread: reduce worker pool: " << threads.size() << " => " << new_threads_मोजणी);
        std::vector< Ptr<WorkerThread> > release_threads(threads.size() - new_threads_मोजणी);
        प्रति (size_t i = new_threads_मोजणी; i < threads.size(); ++i)
        {
            pthread_mutex_lock(&threads[i]->mutex);  // to avoid signal miss due pre-check
            threads[i]->stop_thread = सत्य;
            threads[i]->has_wake_signal = सत्य;
#यदि !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
            pthread_mutex_unlock(&threads[i]->mutex);
            pthread_cond_broadcast/*pthread_cond_signal*/(&threads[i]->cond_thread_wake); // wake thread
#अन्यथा
            pthread_mutex_unlock(&threads[i]->mutex);
#endif
            std::swap(threads[i], release_threads[i - new_threads_मोजणी]);
        }
#यदि defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
        CV_LOG_VERBOSE(रिक्त, 1, "MainThread: notify worker threads about termination...");
        pthread_cond_broadcast(&cond_thread_wake); // wake all threads
#endif
        threads.resize(new_threads_मोजणी);
        release_threads.clear();  // calls thread_join which want to lock mutex
        प्रत्यागम असत्य;
    }
    अन्यथा
    {
        CV_LOG_VERBOSE(रिक्त, 1, "MainThread: upgrade worker pool: " << threads.size() << " => " << new_threads_मोजणी);
        प्रति (size_t i = threads.size(); i < new_threads_मोजणी; ++i)
        {
            threads.push_back(Ptr<WorkerThread>(नूतन WorkerThread(*एतत्, (unsigned)i))); // spawn more threads
        }
    }
    प्रत्यागम असत्य;
}

ThreadPool::~ThreadPool()
{
    reconfigure(0);
    pthread_cond_destroy(&cond_thread_task_complete);
#यदि defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
    pthread_cond_destroy(&cond_thread_wake);
#endif
    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&mutex_notify);
}

शून्य ThreadPool::run(ध्रुव Range& range, ध्रुव ParallelLoopBody& body, द्विगुण nstripes)
{
    CV_LOG_VERBOSE(रिक्त, 1, "MainThread: new parallel job: num_threads=" << अंक_threads << "   range=" << range.size() << "   nstripes=" << nstripes << "   job=" << (शून्य*)job);
#ifdef CV_PROFILE_THREADS
    jobSubmitTime = get_Tick_मोजणी();
    threads_stat[0].reset();
    threads_stat[0].threadWait = jobSubmitTime;
    threads_stat[0].threadWake = jobSubmitTime;
#endif
    यदि (get_अंक_Of_Threads() > 1 &&
        job == रिक्त &&
        (range.size() * nstripes >= 2 || (range.size() > 1 && nstripes <= 0))
    )
    {
        pthread_mutex_lock(&mutex);
        यदि (job != रिक्त)
        {
            pthread_mutex_unlock(&mutex);
            body(range);
            प्रत्यागम;
        }
        reconfigure_(अंक_threads - 1);

        {
            CV_LOG_VERBOSE(रिक्त, 1, "MainThread: initialize parallel job: " << range.size());
            job = Ptr<ParallelJob>(नूतन ParallelJob(*एतत्, range, body, nstripes));
            pthread_mutex_unlock(&mutex);

            CV_LOG_VERBOSE(रिक्त, 5, "MainThread: wake worker threads...");
            size_t अंक_threads_to_wake = std::min(static_cast<size_t>(range.size()), threads.size());
            प्रति (size_t i = 0; i < अंक_threads_to_wake; ++i)
            {
                यदि (job->current_task >= job->range.size())
                    भङ्ग;
                WorkerThread& thread = *(threads[i].get());
                यदि (
#यदि defined(__clang__) && defined(__has_feature)
#यदि __has_feature(thread_sanitizer)
                        1 ||  // Robust workaround to avoid data race warning of `thread.job`
#endif
#endif
#यदि !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
                        thread.isActive ||
#endif
                        thread.has_wake_signal
                        || !thread.job.empty()  // #10881
                )
                {
                    pthread_mutex_lock(&thread.mutex);
                    thread.job = job;
#यदि !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
                    तार्किक isActive = thread.isActive;
#endif
                    thread.has_wake_signal = सत्य;
#ifdef CV_PROFILE_THREADS
                    threads_stat[i + 1].reset();
#endif
                    pthread_mutex_unlock(&thread.mutex);
#यदि !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
                    यदि (!isActive)
                    {
                        pthread_cond_broadcast/*pthread_cond_signal*/(&thread.cond_thread_wake); // wake thread
                    }
#endif
                }
                अन्यथा
                {
                    CV_Assert(thread.job.empty());
                    thread.job = job;
                    thread.has_wake_signal = सत्य;
#ifdef CV_PROFILE_THREADS
                    threads_stat[i + 1].reset();
#endif
#यदि !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
                    pthread_cond_broadcast/*pthread_cond_signal*/(&thread.cond_thread_wake); // wake thread
#endif
                }
            }
#ifdef CV_PROFILE_THREADS
            threads_stat[0].threadPing = get_Tick_मोजणी();
#endif
#यदि defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
            pthread_cond_broadcast(&cond_thread_wake); // wake all threads
#endif
#ifdef CV_PROFILE_THREADS
            threads_stat[0].threadWake = get_Tick_मोजणी();
#endif
            CV_LOG_VERBOSE(रिक्त, 5, "MainThread: wake worker threads... (done)");

            {
                ParallelJob& j = *(एतत्->job);
#ifdef CV_PROFILE_THREADS
                threads_stat[0].threadExecuteStart = get_Tick_मोजणी();
                threads_stat[0].executedTasks = j.execute(असत्य);
                threads_stat[0].threadExecuteStop = get_Tick_मोजणी();
#अन्यथा
                j.execute(असत्य);
#endif
                CV_Assert(j.current_task >= j.range.size());
                CV_LOG_VERBOSE(रिक्त, 5, "MainThread: complete self-tasks: " << j.active_thread_मोजणी << " " << j.completed_thread_मोजणी);
                यदि (job->is_completed || j.active_thread_मोजणी == 0)
                {
                    job->is_completed = सत्य;
                    CV_LOG_VERBOSE(रिक्त, 5, "MainThread: no WIP worker threads");
                }
                अन्यथा
                {
                    यदि (CV_MAIN_THREAD_ACTIVE_WAIT > 0)
                    {
                        प्रति (पूर्णाङ्क i = 0; i < CV_MAIN_THREAD_ACTIVE_WAIT; i++)  // don't spin too much in any case (inaccurate getTickCount())
                        {
                            यदि (job->is_completed)
                            {
                                CV_LOG_VERBOSE(रिक्त, 5, "MainThread: job finalize (active wait) " << j.active_thread_मोजणी << " " << j.completed_thread_मोजणी);
                                भङ्ग;
                            }
                            यदि (CV_ACTIVE_WAIT_PAUSE_LIMIT > 0 && (i < CV_ACTIVE_WAIT_PAUSE_LIMIT || (i & 1)))
                                CV_PAUSE(16);
                            अन्यथा
                                CV_YIELD();
                        }
                    }
                    यदि (!job->is_completed)
                    {
                        CV_LOG_VERBOSE(रिक्त, 5, "MainThread: prepare wait " << j.active_thread_मोजणी << " " << j.completed_thread_मोजणी);
                        pthread_mutex_lock(&mutex_notify);
                        प्रति (;;)
                        {
                            यदि (job->is_completed)
                            {
                                CV_LOG_VERBOSE(रिक्त, 5, "MainThread: job finalize (wait) " << j.active_thread_मोजणी << " " << j.completed_thread_मोजणी);
                                भङ्ग;
                            }
                            CV_LOG_VERBOSE(रिक्त, 5, "MainThread: wait completion (sleep) ...");
                            pthread_cond_wait(&cond_thread_task_complete, &mutex_notify);
                            CV_LOG_VERBOSE(रिक्त, 5, "MainThread: wake");
                        }
                        pthread_mutex_unlock(&mutex_notify);
                    }
                }
            }
#ifdef CV_PROFILE_THREADS
            threads_stat[0].threadFree = get_Tick_मोजणी();
            std::cout << "Job: sz=" << range.size() << " nstripes=" << nstripes << "    Time: " << (threads_stat[0].threadFree - jobSubmitTime) / tickFreq * 1e6 << " usec" << std::endl;
            प्रति (पूर्णाङ्क i = 0; i < (पूर्णाङ्क)threads.size() + 1; i++)
            {
                threads_stat[i].dump(i - 1, jobSubmitTime, tickFreq);
            }
#endif
            यदि (job)
            {
                pthread_mutex_lock(&mutex);
                CV_LOG_VERBOSE(रिक्त, 5, "MainThread: job release");
                CV_Assert(job->is_completed);
                job.release();
                pthread_mutex_unlock(&mutex);
            }
        }
    }
    अन्यथा
    {
        body(range);
    }
}

size_t ThreadPool::get_अंक_Of_Threads()
{
    प्रत्यागम अंक_threads;
}

शून्य ThreadPool::set_अंक_Of_Threads(unsigned n)
{
    यदि (n != अंक_threads)
    {
        अंक_threads = n;
        यदि (n == 1)
           यदि (job == रिक्त) reconfigure(0);  // stop worker threads immediately
    }
}

size_t parallel_pthreads_get_threads_अंक()
{
    प्रत्यागम ThreadPool::instance().get_अंक_Of_Threads();
}

शून्य parallel_pthreads_set_threads_अंक(पूर्णाङ्क num)
{
    यदि(num < 0)
    {
        ThreadPool::instance().set_अंक_Of_Threads(0);
    }
    अन्यथा
    {
        ThreadPool::instance().set_अंक_Of_Threads(unsigned(num));
    }
}

शून्य parallel_for_pthreads(ध्रुव Range& range, ध्रुव ParallelLoopBody& body, द्विगुण nstripes)
{
    ThreadPool::instance().run(range, body, nstripes);
}

}

#endif
