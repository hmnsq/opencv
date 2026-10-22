// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#અન્તર્ભાવ "precomp.hpp"

#અન્તર્ભાવ "parallel_impl.hpp"

#ifdef HAVE_PTHREADS_PF
#અન્તર્ભાવ <pthread.h>

#અન્તર્ભાવ <opencv2/core/utils/configuration.private.hpp>

#અન્તર્ભાવ <opencv2/core/utils/logger.defines.hpp>
//#undef CV_LOG_STRIP_LEVEL
//#define CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_VERBOSE + 1
#અન્તર્ભાવ <opencv2/core/utils/logger.hpp>

#અન્તર્ભાવ <opencv2/core/utils/trace.private.hpp>

//#define CV_PROFILE_THREADS 64
//#define getTickCount getCPUTickCount  // use this if getTickCount() calls are expensive (and getCPUTickCount() is accurate)

//#define CV_USE_GLOBAL_WORKERS_COND_VAR  // not effective on many-core systems (10+)

#અન્તર્ભાવ <atomic>

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
# જો defined __GNUC__ && (defined __i386__ || defined __x86_64__)
#   include <x86intrin.h> /* for __rdtsc */
#   જો !defined(__SSE2__)
      સ્થિર અંતઃરેખ શૂન્ય cv_non_sse_mm_pause() { __asm__ __volatile__ ("rep; nop"); }
#     define _mm_pause cv_non_sse_mm_pause
#   endif
// With Skylake CPUs and above, _mm_pause takes 140 cycles so no need for a loop.
#   define CV_PAUSE(v) કરો { (શૂન્ય)v; _mm_pause(); } જ્યારે (0)
# elif defined __GNUC__ && defined __aarch64__
#   define CV_PAUSE(v) કરો { માટે (પૂર્ણાંક __delay = (v); __delay > 0; --__delay) { asm અસ્થિર("yield" ::: "memory"); } } જ્યારે (0)
# elif defined __GNUC__ && defined __arm__
#   define CV_PAUSE(v) કરો { માટે (પૂર્ણાંક __delay = (v); __delay > 0; --__delay) { asm અસ્થિર("" ::: "memory"); } } જ્યારે (0)
# elif defined __GNUC__ && defined __mips__ && __mips_isa_rev >= 2
#   define CV_PAUSE(v) કરો { માટે (પૂર્ણાંક __delay = (v); __delay > 0; --__delay) { asm અસ્થિર("pause" ::: "memory"); } } જ્યારે (0)
# elif defined __APPLE__ && defined __POWERPC__
#   define CV_PAUSE(v) કરો { માટે (પૂર્ણાંક __delay = (v); __delay > 0; --__delay) { asm અસ્થિર("or r27,r27,r27" ::: "memory"); } } જ્યારે (0)
# elif defined __GNUC__ && defined __PPC64__
#   define CV_PAUSE(v) કરો { માટે (પૂર્ણાંક __delay = (v); __delay > 0; --__delay) { asm અસ્થિર("or 27,27,27" ::: "memory"); } } જ્યારે (0)
# elif defined __GNUC__ && defined __riscv
// PAUSE HINT is not part of RISC-V ISA yet, but is under discussion now. For details see:
// https://github.com/riscv/riscv-isa-manual/pull/398
// https://github.com/riscv/riscv-isa-manual/issues/43
// #   define CV_PAUSE(v) do { for (int __delay = (v); __delay > 0; --__delay) { asm volatile("pause"); } } while (0)
#   define CV_PAUSE(v) કરો { માટે (પૂર્ણાંક __delay = (v); __delay > 0; --__delay) { asm અસ્થિર("nop"); } } જ્યારે (0)
# elif defined __GNUC__ && defined __loongarch__
#   define CV_PAUSE(v) કરો { માટે (પૂર્ણાંક __delay = (v); __delay > 0; --__delay) { asm અસ્થિર("nop"); } } જ્યારે (0)
# નહીંતર
#   warning "Can't detect 'pause' (CPU-yield) instruction on the target platform. Specify CV_PAUSE() definition via compiler flags."
#   define CV_PAUSE(...) કરો { /* no-op: works, but not effective */ } જ્યારે (0)
# endif
#endif // CV_PAUSE


નામાકાશ cv
{

સ્થિર પૂર્ણાંક CV_ACTIVE_WAIT_PAUSE_LIMIT = (પૂર્ણાંક)utils::getConfigurationParameterSizeT("OPENCV_THREAD_POOL_ACTIVE_WAIT_PAUSE_LIMIT", 16);  // iterations
સ્થિર પૂર્ણાંક CV_WORKER_ACTIVE_WAIT = (પૂર્ણાંક)utils::getConfigurationParameterSizeT("OPENCV_THREAD_POOL_ACTIVE_WAIT_WORKER", 2000);  // iterations
સ્થિર પૂર્ણાંક CV_MAIN_THREAD_ACTIVE_WAIT = (પૂર્ણાંક)utils::getConfigurationParameterSizeT("OPENCV_THREAD_POOL_ACTIVE_WAIT_MAIN", 10000); // iterations

સ્થિર પૂર્ણાંક CV_WORKER_ACTIVE_WAIT_THREADS_LIMIT = (પૂર્ણાંક)utils::getConfigurationParameterSizeT("OPENCV_THREAD_POOL_ACTIVE_WAIT_THREADS_LIMIT", 0); // number of real cores

વર્ગ WorkerThread;
વર્ગ ParallelJob;

વર્ગ ThreadPool
{
જાહેર:
    સ્થિર ThreadPool& instance()
    {
        CV_SINGLETON_LAZY_INIT_REF(ThreadPool, નવું ThreadPool())
    }

    સ્થિર શૂન્ય stop()
    {
        ThreadPool& manager = instance();
        manager.reconfigure(0);
    }

    શૂન્ય reconfigure(unsigned new_threads_ગણતરી)
    {
        જો (new_threads_ગણતરી == threads.size())
            પાછા;
        pthread_mutex_lock(&mutex);
        reconfigure_(new_threads_ગણતરી);
        pthread_mutex_unlock(&mutex);
    }
    તાર્કિક reconfigure_(unsigned new_threads_ગણતરી); // internal implementation

    શૂન્ય run(અચલ Range& range, અચલ ParallelLoopBody& body, દ્વિગુણ nstripes);

    size_t get_અંક_Of_Threads();

    શૂન્ય set_અંક_Of_Threads(unsigned n);

    ThreadPool();

    ~ThreadPool();

    unsigned અંક_threads;

    pthread_mutex_t mutex;  // guards fields (job/threads) from non-worker threads (concurrent parallel_for calls)
#જો defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
    pthread_cond_t cond_thread_wake;
#endif

    pthread_mutex_t mutex_notify;
    pthread_cond_t cond_thread_task_complete;

    std::vector< Ptr<WorkerThread> > threads;

    Ptr<ParallelJob> job;

#ifdef CV_PROFILE_THREADS
    દ્વિગુણ tickFreq;
    int64 jobSubmitTime;
    સંરચના ThreadStatistics
    {
        ThreadStatistics() : threadWait(0)
        {
            reset();
        }
        શૂન્ય reset()
        {
            threadWake = 0;
            threadExecuteStart = 0;
            threadExecuteStop = 0;
            executedTasks = 0;
            keepActive = ખોટું;
            threadPing = get_Tick_ગણતરી();
        }
        int64 threadWait; // don't reset by default
        int64 threadPing; // don't reset by default
        int64 threadWake;
        int64 threadExecuteStart;
        int64 threadExecuteStop;
        int64 threadFree;
        unsigned executedTasks;
        તાર્કિક keepActive;

        int64 dummy_[8]; // separate cache lines

        શૂન્ય dump(પૂર્ણાંક id, int64 baseTime, દ્વિગુણ tickFreq)
        {
            જો (id < 0)
                std::cout << "Main: ";
            નહીંતર
                printf("T%03d: ", id + 2);
            printf("wait=% 10.1f   ping=% 6.1f",
                    threadWait > 0 ? (threadWait - baseTime) / tickFreq * 1e6 : -0.0,
                    threadPing > 0 ? (threadPing - baseTime) / tickFreq * 1e6 : -0.0);
            જો (threadWake > 0)
                printf("   wake=% 6.1f",
                    (threadWake > 0 ? (threadWake - baseTime) / tickFreq * 1e6 : -0.0));
            જો (threadExecuteStart > 0)
            {
                printf("   exec=% 6.1f - % 6.1f   tasksDone=%5u   free=% 6.1f",
                    (threadExecuteStart > 0 ? (threadExecuteStart - baseTime) / tickFreq * 1e6 : -0.0),
                    (threadExecuteStop > 0 ? (threadExecuteStop - baseTime) / tickFreq * 1e6 : -0.0),
                    executedTasks,
                    (threadFree > 0 ? (threadFree - baseTime) / tickFreq * 1e6 : -0.0));
                જો (id >= 0)
                    printf(" active=%s\n", keepActive ? "true" : "false");
                નહીંતર
                    printf("\n");
            }
            નહીંતર
                printf("   ------------------------------------------------------------------------------\n");
        }
    };
    ThreadStatistics threads_stat[CV_PROFILE_THREADS]; // 0 - main thread, 1..N - worker threads
#endif

};

વર્ગ WorkerThread
{
જાહેર:
    ThreadPool& thread_pool;
    અચલ unsigned id;
    pthread_t posix_thread;
    તાર્કિક is_created;

    std::atomic<તાર્કિક> stop_thread;

    std::atomic<તાર્કિક> has_wake_signal;

    Ptr<ParallelJob> job;

    pthread_mutex_t mutex;
#જો !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
    અસ્થિર તાર્કિક isActive;
    pthread_cond_t cond_thread_wake;
#endif

    WorkerThread(ThreadPool& thread_pool_, unsigned id_) :
        thread_pool(thread_pool_),
        id(id_),
        posix_thread(0),
        is_created(ખોટું),
        stop_thread(ખોટું),
        has_wake_signal(ખોટું)
#જો !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
        , isActive(સાચું)
#endif
    {
        CV_LOG_VERBOSE(ખાલી, 1, "MainThread: initializing new worker: " << id);
        પૂર્ણાંક res = pthread_mutex_init(&mutex, ખાલી);
        જો (res != 0)
        {
            CV_LOG_ERROR(ખાલી, id << ": Can't create thread mutex: res = " << res);
            પાછા;
        }
#જો !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
        res = pthread_cond_init(&cond_thread_wake, ખાલી);
        જો (res != 0)
        {
            CV_LOG_ERROR(ખાલી, id << ": Can't create thread condition variable: res = " << res);
            પાછા;
        }
#endif
        res = pthread_create(&posix_thread, ખાલી, thread_loop_wrapper, (શૂન્ય*)આ);
        જો (res != 0)
        {
            CV_LOG_ERROR(ખાલી, id << ": Can't spawn new thread: res = " << res);
        }
        નહીંતર
        {
            is_created = સાચું;
        }
    }

    ~WorkerThread()
    {
        CV_LOG_VERBOSE(ખાલી, 1, "MainThread: destroy worker thread: " << id);
        જો (is_created)
        {
            જો (!stop_thread)
            {
                pthread_mutex_lock(&mutex);  // to avoid signal miss due pre-check
                stop_thread = સાચું;
                pthread_mutex_unlock(&mutex);
#જો defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
                pthread_cond_broadcast(&thread_pool.cond_thread_wake);
#નહીંતર
                pthread_cond_signal(&cond_thread_wake);
#endif
            }
            pthread_join(posix_thread, ખાલી);
        }
#જો !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
        pthread_cond_destroy(&cond_thread_wake);
#endif
        pthread_mutex_destroy(&mutex);
    }

    શૂન્ય thread_body();
    સ્થિર શૂન્ય* thread_loop_wrapper(શૂન્ય* thread_object)
    {
#ifdef OPENCV_WITH_ITT
        __itt_thread_set_name(cv::format("OpenCVThread-%03d", cv::utils::getThreadID()).c_str());
#endif
        ((WorkerThread*)thread_object)->thread_body();
        પાછા 0;
    }
};

વર્ગ ParallelJob
{
જાહેર:
    ParallelJob(અચલ ThreadPool& thread_pool_, અચલ Range& range_, અચલ ParallelLoopBody& body_, પૂર્ણાંક nstripes_) :
        thread_pool(thread_pool_),
        body(body_),
        range(range_),
        nstripes((unsigned)nstripes_),
        is_completed(ખોટું)
    {
        CV_LOG_VERBOSE(ખાલી, 5, "ParallelJob::ParallelJob(" << (શૂન્ય*)આ << ")");
        current_task.store(0, std::memory_order_relaxed);
        active_thread_ગણતરી.store(0, std::memory_order_relaxed);
        completed_thread_ગણતરી.store(0, std::memory_order_relaxed);
        dummy0_[0] = 0, dummy1_[0] = 0, dummy2_[0] = 0; // compiler warning
    }

    ~ParallelJob()
    {
        CV_LOG_VERBOSE(ખાલી, 5, "ParallelJob::~ParallelJob(" << (શૂન્ય*)આ << ")");
    }

    unsigned execute(તાર્કિક is_worker_thread)
    {
        unsigned executed_tasks = 0;
        અચલ પૂર્ણાંક task_ગણતરી = range.size();
        અચલ પૂર્ણાંક remaining_multiplier = std::min(nstripes,
                std::max(
                        std::min(100u, thread_pool.અંક_threads * 4),
                        thread_pool.અંક_threads * 2
                ));  // experimental value
        માટે (;;)
        {
            પૂર્ણાંક chunk_size = std::max(1, (task_ગણતરી - current_task) / remaining_multiplier);
            પૂર્ણાંક id = current_task.fetch_add(chunk_size, std::memory_order_seq_cst);
            જો (id >= task_ગણતરી)
                તોડો; // no more free tasks

            executed_tasks += chunk_size;
            પૂર્ણાંક start_id = id;
            પૂર્ણાંક end_id = std::min(task_ગણતરી, id + chunk_size);
            CV_LOG_VERBOSE(ખાલી, 9, "Thread: job " << start_id << "-" << end_id);

            //TODO: if (not pending exception)
            {
                body.operator()(Range(range.start + start_id, range.start + end_id));
            }
            જો (is_worker_thread && is_completed)
            {
                CV_LOG_ERROR(ખાલી, "\t\t\t\tBUG! Job: " << (શૂન્ય*)આ << " " << id << " " << active_thread_ગણતરી << " " << completed_thread_ગણતરી);
                CV_Assert(!is_completed); // TODO Dbg this
            }
        }
        પાછા executed_tasks;
    }

    અચલ ThreadPool& thread_pool;
    અચલ ParallelLoopBody& body;
    અચલ Range range;
    અચલ unsigned nstripes;

    std::atomic<પૂર્ણાંક> current_task;  // next free part of job
    int64 dummy0_[8];  // avoid cache-line reusing for the same atomics

    std::atomic<પૂર્ણાંક> active_thread_ગણતરી;  // number of threads worked on this job
    int64 dummy1_[8];  // avoid cache-line reusing for the same atomics

    std::atomic<પૂર્ણાંક> completed_thread_ગણતરી;  // number of threads completed any activities on this job
    int64 dummy2_[8];  // avoid cache-line reusing for the same atomics

    std::atomic<તાર્કિક> is_completed;

    // TODO exception handling
};


// Disable thread sanitization check when CV_USE_GLOBAL_WORKERS_COND_VAR is not
// set because it triggers as the main thread reads isActive while the children
// thread writes it (but it all works out because a mutex is locked in the main
// thread and isActive re-checked).
// This is to solve issue #19463.
#જો !defined(CV_USE_GLOBAL_WORKERS_COND_VAR) && defined(__clang__) && defined(__has_feature)
#જો __has_feature(thread_sanitizer)
__attribute__((no_sanitize("thread")))
#endif
#endif
શૂન્ય WorkerThread::thread_body()
{
    (શૂન્ય)cv::utils::getThreadID(); // notify OpenCV about new thread
    CV_LOG_VERBOSE(ખાલી, 5, "Thread: new thread: " << id);

    તાર્કિક allow_active_wait = સાચું;

#ifdef CV_PROFILE_THREADS
    ThreadPool::ThreadStatistics& stat = thread_pool.threads_stat[id + 1];
#endif

    જ્યારે (!stop_thread)
    {
        CV_LOG_VERBOSE(ખાલી, 5, "Thread: ... loop iteration: allow_active_wait=" << allow_active_wait << "   has_wake_signal=" << has_wake_signal);
        જો (allow_active_wait && CV_WORKER_ACTIVE_WAIT > 0)
        {
            allow_active_wait = ખોટું;
            માટે (પૂર્ણાંક i = 0; i < CV_WORKER_ACTIVE_WAIT; i++)
            {
                જો (has_wake_signal)
                    તોડો;
                જો (CV_ACTIVE_WAIT_PAUSE_LIMIT > 0 && (i < CV_ACTIVE_WAIT_PAUSE_LIMIT || (i & 1)))
                    CV_PAUSE(16);
                નહીંતર
                    CV_YIELD();
            }
        }
        pthread_mutex_lock(&mutex);
#ifdef CV_PROFILE_THREADS
        stat.threadWait = get_Tick_ગણતરી();
#endif
        જ્યારે (!has_wake_signal) // to handle spurious wakeups
        {
            //CV_LOG_VERBOSE(NULL, 5, "Thread: wait (sleep) ...");
#જો defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
            pthread_cond_wait(&thread_pool.cond_thread_wake, &mutex);
#નહીંતર
            isActive = ખોટું;
            pthread_cond_wait(&cond_thread_wake, &mutex);
            isActive = સાચું;
#endif
            CV_LOG_VERBOSE(ખાલી, 5, "Thread: wake ... (has_wake_signal=" << has_wake_signal << " stop_thread=" << stop_thread << ")")
        }
#ifdef CV_PROFILE_THREADS
        stat.threadWake = get_Tick_ગણતરી();
#endif

        CV_LOG_VERBOSE(ખાલી, 5, "Thread: checking for new job");
        જો (CV_WORKER_ACTIVE_WAIT_THREADS_LIMIT == 0)
            allow_active_wait = સાચું;
        Ptr<ParallelJob> j_ptr; swap(j_ptr, job);
        has_wake_signal = ખોટું;
        pthread_mutex_unlock(&mutex);

        જો (!stop_thread)
        {
            ParallelJob* j = j_ptr;
            જો (j)
            {
                CV_LOG_VERBOSE(ખાલી, 5, "Thread: job size=" << j->range.size() << " done=" << j->current_task);
                જો (j->current_task < j->range.size())
                {
                    પૂર્ણાંક other = j->active_thread_ગણતરી.fetch_add(1, std::memory_order_seq_cst);
                    CV_LOG_VERBOSE(ખાલી, 5, "Thread: processing new job (with " << other << " other threads)"); CV_UNUSED(other);
#ifdef CV_PROFILE_THREADS
                    stat.threadExecuteStart = get_Tick_ગણતરી();
                    stat.executedTasks = j->execute(સાચું);
                    stat.threadExecuteStop = get_Tick_ગણતરી();
#નહીંતર
                    j->execute(સાચું);
#endif
                    પૂર્ણાંક completed = j->completed_thread_ગણતરી.fetch_add(1, std::memory_order_seq_cst) + 1;
                    પૂર્ણાંક active = j->active_thread_ગણતરી.load(std::memory_order_acquire);
                    જો (CV_WORKER_ACTIVE_WAIT_THREADS_LIMIT > 0)
                    {
                        allow_active_wait = સાચું;
                        જો (active >= CV_WORKER_ACTIVE_WAIT_THREADS_LIMIT && (id & 1) == 0) // turn off a half of threads
                            allow_active_wait = ખોટું;
                    }
                    CV_LOG_VERBOSE(ખાલી, 5, "Thread: completed job processing: " << active << " " << completed);
                    જો (active == completed)
                    {
                        તાર્કિક need_signal = !j->is_completed;
                        j->is_completed = સાચું;
                        j = ખાલી; j_ptr.release();
                        જો (need_signal)
                        {
                            CV_LOG_VERBOSE(ખાલી, 5, "Thread: job finished => notifying the main thread");
                            pthread_mutex_lock(&thread_pool.mutex_notify);  // to avoid signal miss due pre-check condition
                            // empty
                            pthread_mutex_unlock(&thread_pool.mutex_notify);
                            pthread_cond_broadcast/*pthread_cond_signal*/(&thread_pool.cond_thread_task_complete);
                        }
                    }
                }
                નહીંતર
                {
                    CV_LOG_VERBOSE(ખાલી, 5, "Thread: no free job tasks");
                }
            }
        }
#ifdef CV_PROFILE_THREADS
        stat.threadFree = get_Tick_ગણતરી();
        stat.keepActive = allow_active_wait;
#endif
    }
}

ThreadPool::ThreadPool()
{
#ifdef CV_PROFILE_THREADS
    tickFreq = getTickFrequency();
#endif

    પૂર્ણાંક res = 0;
    res |= pthread_mutex_init(&mutex, ખાલી);
    res |= pthread_mutex_init(&mutex_notify, ખાલી);
#જો defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
    res |= pthread_cond_init(&cond_thread_wake, ખાલી);
#endif
    res |= pthread_cond_init(&cond_thread_task_complete, ખાલી);

    જો (0 != res)
    {
        CV_LOG_FATAL(ખાલી, "Failed to initialize ThreadPool (pthreads)");
    }
    અંક_threads = default_સંખ્યા_Of_Threads();
}

તાર્કિક ThreadPool::reconfigure_(unsigned new_threads_ગણતરી)
{
    જો (new_threads_ગણતરી == threads.size())
        પાછા ખોટું;

    જો (new_threads_ગણતરી < threads.size())
    {
        CV_LOG_VERBOSE(ખાલી, 1, "MainThread: reduce worker pool: " << threads.size() << " => " << new_threads_ગણતરી);
        std::vector< Ptr<WorkerThread> > release_threads(threads.size() - new_threads_ગણતરી);
        માટે (size_t i = new_threads_ગણતરી; i < threads.size(); ++i)
        {
            pthread_mutex_lock(&threads[i]->mutex);  // to avoid signal miss due pre-check
            threads[i]->stop_thread = સાચું;
            threads[i]->has_wake_signal = સાચું;
#જો !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
            pthread_mutex_unlock(&threads[i]->mutex);
            pthread_cond_broadcast/*pthread_cond_signal*/(&threads[i]->cond_thread_wake); // wake thread
#નહીંતર
            pthread_mutex_unlock(&threads[i]->mutex);
#endif
            std::swap(threads[i], release_threads[i - new_threads_ગણતરી]);
        }
#જો defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
        CV_LOG_VERBOSE(ખાલી, 1, "MainThread: notify worker threads about termination...");
        pthread_cond_broadcast(&cond_thread_wake); // wake all threads
#endif
        threads.resize(new_threads_ગણતરી);
        release_threads.clear();  // calls thread_join which want to lock mutex
        પાછા ખોટું;
    }
    નહીંતર
    {
        CV_LOG_VERBOSE(ખાલી, 1, "MainThread: upgrade worker pool: " << threads.size() << " => " << new_threads_ગણતરી);
        માટે (size_t i = threads.size(); i < new_threads_ગણતરી; ++i)
        {
            threads.push_back(Ptr<WorkerThread>(નવું WorkerThread(*આ, (unsigned)i))); // spawn more threads
        }
    }
    પાછા ખોટું;
}

ThreadPool::~ThreadPool()
{
    reconfigure(0);
    pthread_cond_destroy(&cond_thread_task_complete);
#જો defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
    pthread_cond_destroy(&cond_thread_wake);
#endif
    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&mutex_notify);
}

શૂન્ય ThreadPool::run(અચલ Range& range, અચલ ParallelLoopBody& body, દ્વિગુણ nstripes)
{
    CV_LOG_VERBOSE(ખાલી, 1, "MainThread: new parallel job: num_threads=" << અંક_threads << "   range=" << range.size() << "   nstripes=" << nstripes << "   job=" << (શૂન્ય*)job);
#ifdef CV_PROFILE_THREADS
    jobSubmitTime = get_Tick_ગણતરી();
    threads_stat[0].reset();
    threads_stat[0].threadWait = jobSubmitTime;
    threads_stat[0].threadWake = jobSubmitTime;
#endif
    જો (get_અંક_Of_Threads() > 1 &&
        job == ખાલી &&
        (range.size() * nstripes >= 2 || (range.size() > 1 && nstripes <= 0))
    )
    {
        pthread_mutex_lock(&mutex);
        જો (job != ખાલી)
        {
            pthread_mutex_unlock(&mutex);
            body(range);
            પાછા;
        }
        reconfigure_(અંક_threads - 1);

        {
            CV_LOG_VERBOSE(ખાલી, 1, "MainThread: initialize parallel job: " << range.size());
            job = Ptr<ParallelJob>(નવું ParallelJob(*આ, range, body, nstripes));
            pthread_mutex_unlock(&mutex);

            CV_LOG_VERBOSE(ખાલી, 5, "MainThread: wake worker threads...");
            size_t અંક_threads_to_wake = std::min(static_cast<size_t>(range.size()), threads.size());
            માટે (size_t i = 0; i < અંક_threads_to_wake; ++i)
            {
                જો (job->current_task >= job->range.size())
                    તોડો;
                WorkerThread& thread = *(threads[i].get());
                જો (
#જો defined(__clang__) && defined(__has_feature)
#જો __has_feature(thread_sanitizer)
                        1 ||  // Robust workaround to avoid data race warning of `thread.job`
#endif
#endif
#જો !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
                        thread.isActive ||
#endif
                        thread.has_wake_signal
                        || !thread.job.empty()  // #10881
                )
                {
                    pthread_mutex_lock(&thread.mutex);
                    thread.job = job;
#જો !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
                    તાર્કિક isActive = thread.isActive;
#endif
                    thread.has_wake_signal = સાચું;
#ifdef CV_PROFILE_THREADS
                    threads_stat[i + 1].reset();
#endif
                    pthread_mutex_unlock(&thread.mutex);
#જો !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
                    જો (!isActive)
                    {
                        pthread_cond_broadcast/*pthread_cond_signal*/(&thread.cond_thread_wake); // wake thread
                    }
#endif
                }
                નહીંતર
                {
                    CV_Assert(thread.job.empty());
                    thread.job = job;
                    thread.has_wake_signal = સાચું;
#ifdef CV_PROFILE_THREADS
                    threads_stat[i + 1].reset();
#endif
#જો !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
                    pthread_cond_broadcast/*pthread_cond_signal*/(&thread.cond_thread_wake); // wake thread
#endif
                }
            }
#ifdef CV_PROFILE_THREADS
            threads_stat[0].threadPing = get_Tick_ગણતરી();
#endif
#જો defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
            pthread_cond_broadcast(&cond_thread_wake); // wake all threads
#endif
#ifdef CV_PROFILE_THREADS
            threads_stat[0].threadWake = get_Tick_ગણતરી();
#endif
            CV_LOG_VERBOSE(ખાલી, 5, "MainThread: wake worker threads... (done)");

            {
                ParallelJob& j = *(આ->job);
#ifdef CV_PROFILE_THREADS
                threads_stat[0].threadExecuteStart = get_Tick_ગણતરી();
                threads_stat[0].executedTasks = j.execute(ખોટું);
                threads_stat[0].threadExecuteStop = get_Tick_ગણતરી();
#નહીંતર
                j.execute(ખોટું);
#endif
                CV_Assert(j.current_task >= j.range.size());
                CV_LOG_VERBOSE(ખાલી, 5, "MainThread: complete self-tasks: " << j.active_thread_ગણતરી << " " << j.completed_thread_ગણતરી);
                જો (job->is_completed || j.active_thread_ગણતરી == 0)
                {
                    job->is_completed = સાચું;
                    CV_LOG_VERBOSE(ખાલી, 5, "MainThread: no WIP worker threads");
                }
                નહીંતર
                {
                    જો (CV_MAIN_THREAD_ACTIVE_WAIT > 0)
                    {
                        માટે (પૂર્ણાંક i = 0; i < CV_MAIN_THREAD_ACTIVE_WAIT; i++)  // don't spin too much in any case (inaccurate getTickCount())
                        {
                            જો (job->is_completed)
                            {
                                CV_LOG_VERBOSE(ખાલી, 5, "MainThread: job finalize (active wait) " << j.active_thread_ગણતરી << " " << j.completed_thread_ગણતરી);
                                તોડો;
                            }
                            જો (CV_ACTIVE_WAIT_PAUSE_LIMIT > 0 && (i < CV_ACTIVE_WAIT_PAUSE_LIMIT || (i & 1)))
                                CV_PAUSE(16);
                            નહીંતર
                                CV_YIELD();
                        }
                    }
                    જો (!job->is_completed)
                    {
                        CV_LOG_VERBOSE(ખાલી, 5, "MainThread: prepare wait " << j.active_thread_ગણતરી << " " << j.completed_thread_ગણતરી);
                        pthread_mutex_lock(&mutex_notify);
                        માટે (;;)
                        {
                            જો (job->is_completed)
                            {
                                CV_LOG_VERBOSE(ખાલી, 5, "MainThread: job finalize (wait) " << j.active_thread_ગણતરી << " " << j.completed_thread_ગણતરી);
                                તોડો;
                            }
                            CV_LOG_VERBOSE(ખાલી, 5, "MainThread: wait completion (sleep) ...");
                            pthread_cond_wait(&cond_thread_task_complete, &mutex_notify);
                            CV_LOG_VERBOSE(ખાલી, 5, "MainThread: wake");
                        }
                        pthread_mutex_unlock(&mutex_notify);
                    }
                }
            }
#ifdef CV_PROFILE_THREADS
            threads_stat[0].threadFree = get_Tick_ગણતરી();
            std::cout << "Job: sz=" << range.size() << " nstripes=" << nstripes << "    Time: " << (threads_stat[0].threadFree - jobSubmitTime) / tickFreq * 1e6 << " usec" << std::endl;
            માટે (પૂર્ણાંક i = 0; i < (પૂર્ણાંક)threads.size() + 1; i++)
            {
                threads_stat[i].dump(i - 1, jobSubmitTime, tickFreq);
            }
#endif
            જો (job)
            {
                pthread_mutex_lock(&mutex);
                CV_LOG_VERBOSE(ખાલી, 5, "MainThread: job release");
                CV_Assert(job->is_completed);
                job.release();
                pthread_mutex_unlock(&mutex);
            }
        }
    }
    નહીંતર
    {
        body(range);
    }
}

size_t ThreadPool::get_અંક_Of_Threads()
{
    પાછા અંક_threads;
}

શૂન્ય ThreadPool::set_અંક_Of_Threads(unsigned n)
{
    જો (n != અંક_threads)
    {
        અંક_threads = n;
        જો (n == 1)
           જો (job == ખાલી) reconfigure(0);  // stop worker threads immediately
    }
}

size_t parallel_pthreads_get_threads_અંક()
{
    પાછા ThreadPool::instance().get_અંક_Of_Threads();
}

શૂન્ય parallel_pthreads_set_threads_અંક(પૂર્ણાંક num)
{
    જો(num < 0)
    {
        ThreadPool::instance().set_અંક_Of_Threads(0);
    }
    નહીંતર
    {
        ThreadPool::instance().set_અંક_Of_Threads(unsigned(num));
    }
}

શૂન્ય parallel_for_pthreads(અચલ Range& range, અચલ ParallelLoopBody& body, દ્વિગુણ nstripes)
{
    ThreadPool::instance().run(range, body, nstripes);
}

}

#endif
