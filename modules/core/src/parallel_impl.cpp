// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ଅନ୍ତର୍ଭାଵ "precomp.hpp"

#ଅନ୍ତର୍ଭାଵ "parallel_impl.hpp"

#ifdef HAVE_PTHREADS_PF
#ଅନ୍ତର୍ଭାଵ <pthread.h>

#ଅନ୍ତର୍ଭାଵ <opencv2/core/utils/configuration.private.hpp>

#ଅନ୍ତର୍ଭାଵ <opencv2/core/utils/logger.defines.hpp>
//#undef CV_LOG_STRIP_LEVEL
//#define CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_VERBOSE + 1
#ଅନ୍ତର୍ଭାଵ <opencv2/core/utils/logger.hpp>

#ଅନ୍ତର୍ଭାଵ <opencv2/core/utils/trace.private.hpp>

//#define CV_PROFILE_THREADS 64
//#define getTickCount getCPUTickCount  // use this if getTickCount() calls are expensive (and getCPUTickCount() is accurate)

//#define CV_USE_GLOBAL_WORKERS_COND_VAR  // not effective on many-core systems (10+)

#ଅନ୍ତର୍ଭାଵ <atomic>

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
# ଯଦି defined __GNUC__ && (defined __i386__ || defined __x86_64__)
#   include <x86intrin.h> /* for __rdtsc */
#   ଯଦି !defined(__SSE2__)
      ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ cv_non_sse_mm_pause() { __asm__ __volatile__ ("rep; nop"); }
#     define _mm_pause cv_non_sse_mm_pause
#   endif
// With Skylake CPUs and above, _mm_pause takes 140 cycles so no need for a loop.
#   define CV_PAUSE(v) କର { (ଶୂନ୍ୟ)v; _mm_pause(); } ଯେତେବେଳେ (0)
# elif defined __GNUC__ && defined __aarch64__
#   define CV_PAUSE(v) କର { ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା __delay = (v); __delay > 0; --__delay) { asm ଅସ୍ଥିର("yield" ::: "memory"); } } ଯେତେବେଳେ (0)
# elif defined __GNUC__ && defined __arm__
#   define CV_PAUSE(v) କର { ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା __delay = (v); __delay > 0; --__delay) { asm ଅସ୍ଥିର("" ::: "memory"); } } ଯେତେବେଳେ (0)
# elif defined __GNUC__ && defined __mips__ && __mips_isa_rev >= 2
#   define CV_PAUSE(v) କର { ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା __delay = (v); __delay > 0; --__delay) { asm ଅସ୍ଥିର("pause" ::: "memory"); } } ଯେତେବେଳେ (0)
# elif defined __APPLE__ && defined __POWERPC__
#   define CV_PAUSE(v) କର { ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା __delay = (v); __delay > 0; --__delay) { asm ଅସ୍ଥିର("or r27,r27,r27" ::: "memory"); } } ଯେତେବେଳେ (0)
# elif defined __GNUC__ && defined __PPC64__
#   define CV_PAUSE(v) କର { ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା __delay = (v); __delay > 0; --__delay) { asm ଅସ୍ଥିର("or 27,27,27" ::: "memory"); } } ଯେତେବେଳେ (0)
# elif defined __GNUC__ && defined __riscv
// PAUSE HINT is not part of RISC-V ISA yet, but is under discussion now. For details see:
// https://github.com/riscv/riscv-isa-manual/pull/398
// https://github.com/riscv/riscv-isa-manual/issues/43
// #   define CV_PAUSE(v) do { for (int __delay = (v); __delay > 0; --__delay) { asm volatile("pause"); } } while (0)
#   define CV_PAUSE(v) କର { ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା __delay = (v); __delay > 0; --__delay) { asm ଅସ୍ଥିର("nop"); } } ଯେତେବେଳେ (0)
# elif defined __GNUC__ && defined __loongarch__
#   define CV_PAUSE(v) କର { ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା __delay = (v); __delay > 0; --__delay) { asm ଅସ୍ଥିର("nop"); } } ଯେତେବେଳେ (0)
# ଅନ୍ୟଥା
#   warning "Can't detect 'pause' (CPU-yield) instruction on the target platform. Specify CV_PAUSE() definition via compiler flags."
#   define CV_PAUSE(...) କର { /* no-op: works, but not effective */ } ଯେତେବେଳେ (0)
# endif
#endif // CV_PAUSE


ନାମାକାଶ cv
{

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା CV_ACTIVE_WAIT_PAUSE_LIMIT = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)utils::getConfigurationParameterSizeT("OPENCV_THREAD_POOL_ACTIVE_WAIT_PAUSE_LIMIT", 16);  // iterations
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା CV_WORKER_ACTIVE_WAIT = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)utils::getConfigurationParameterSizeT("OPENCV_THREAD_POOL_ACTIVE_WAIT_WORKER", 2000);  // iterations
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା CV_MAIN_THREAD_ACTIVE_WAIT = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)utils::getConfigurationParameterSizeT("OPENCV_THREAD_POOL_ACTIVE_WAIT_MAIN", 10000); // iterations

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା CV_WORKER_ACTIVE_WAIT_THREADS_LIMIT = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)utils::getConfigurationParameterSizeT("OPENCV_THREAD_POOL_ACTIVE_WAIT_THREADS_LIMIT", 0); // number of real cores

ଶ୍ରେଣୀ WorkerThread;
ଶ୍ରେଣୀ ParallelJob;

ଶ୍ରେଣୀ ThreadPool
{
ସର୍ବସାଧାରଣ:
    ସ୍ଥିର ThreadPool& instance()
    {
        CV_SINGLETON_LAZY_INIT_REF(ThreadPool, ନୂତନ ThreadPool())
    }

    ସ୍ଥିର ଶୂନ୍ୟ stop()
    {
        ThreadPool& manager = instance();
        manager.reconfigure(0);
    }

    ଶୂନ୍ୟ reconfigure(unsigned new_threads_ଗଣନା)
    {
        ଯଦି (new_threads_ଗଣନା == threads.size())
            ଫେରନ୍ତୁ;
        pthread_mutex_lock(&mutex);
        reconfigure_(new_threads_ଗଣନା);
        pthread_mutex_unlock(&mutex);
    }
    ତାର୍କିକ reconfigure_(unsigned new_threads_ଗଣନା); // internal implementation

    ଶୂନ୍ୟ run(ଅଚଳ Range& range, ଅଚଳ ParallelLoopBody& body, ଦ୍ୱିଗୁଣ nstripes);

    size_t get_ଅଙ୍କ_Of_Threads();

    ଶୂନ୍ୟ set_ଅଙ୍କ_Of_Threads(unsigned n);

    ThreadPool();

    ~ThreadPool();

    unsigned ଅଙ୍କ_threads;

    pthread_mutex_t mutex;  // guards fields (job/threads) from non-worker threads (concurrent parallel_for calls)
#ଯଦି defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
    pthread_cond_t cond_thread_wake;
#endif

    pthread_mutex_t mutex_notify;
    pthread_cond_t cond_thread_task_complete;

    std::vector< Ptr<WorkerThread> > threads;

    Ptr<ParallelJob> job;

#ifdef CV_PROFILE_THREADS
    ଦ୍ୱିଗୁଣ tickFreq;
    int64 jobSubmitTime;
    ସଂରଚନା ThreadStatistics
    {
        ThreadStatistics() : threadWait(0)
        {
            reset();
        }
        ଶୂନ୍ୟ reset()
        {
            threadWake = 0;
            threadExecuteStart = 0;
            threadExecuteStop = 0;
            executedTasks = 0;
            keepActive = ମିଥ୍ୟା;
            threadPing = get_Tick_ଗଣନା();
        }
        int64 threadWait; // don't reset by default
        int64 threadPing; // don't reset by default
        int64 threadWake;
        int64 threadExecuteStart;
        int64 threadExecuteStop;
        int64 threadFree;
        unsigned executedTasks;
        ତାର୍କିକ keepActive;

        int64 dummy_[8]; // separate cache lines

        ଶୂନ୍ୟ dump(ପୂର୍ଣ୍ଣସଂଖ୍ୟା id, int64 baseTime, ଦ୍ୱିଗୁଣ tickFreq)
        {
            ଯଦି (id < 0)
                std::cout << "Main: ";
            ଅନ୍ୟଥା
                printf("T%03d: ", id + 2);
            printf("wait=% 10.1f   ping=% 6.1f",
                    threadWait > 0 ? (threadWait - baseTime) / tickFreq * 1e6 : -0.0,
                    threadPing > 0 ? (threadPing - baseTime) / tickFreq * 1e6 : -0.0);
            ଯଦି (threadWake > 0)
                printf("   wake=% 6.1f",
                    (threadWake > 0 ? (threadWake - baseTime) / tickFreq * 1e6 : -0.0));
            ଯଦି (threadExecuteStart > 0)
            {
                printf("   exec=% 6.1f - % 6.1f   tasksDone=%5u   free=% 6.1f",
                    (threadExecuteStart > 0 ? (threadExecuteStart - baseTime) / tickFreq * 1e6 : -0.0),
                    (threadExecuteStop > 0 ? (threadExecuteStop - baseTime) / tickFreq * 1e6 : -0.0),
                    executedTasks,
                    (threadFree > 0 ? (threadFree - baseTime) / tickFreq * 1e6 : -0.0));
                ଯଦି (id >= 0)
                    printf(" active=%s\n", keepActive ? "true" : "false");
                ଅନ୍ୟଥା
                    printf("\n");
            }
            ଅନ୍ୟଥା
                printf("   ------------------------------------------------------------------------------\n");
        }
    };
    ThreadStatistics threads_stat[CV_PROFILE_THREADS]; // 0 - main thread, 1..N - worker threads
#endif

};

ଶ୍ରେଣୀ WorkerThread
{
ସର୍ବସାଧାରଣ:
    ThreadPool& thread_pool;
    ଅଚଳ unsigned id;
    pthread_t posix_thread;
    ତାର୍କିକ is_created;

    std::atomic<ତାର୍କିକ> stop_thread;

    std::atomic<ତାର୍କିକ> has_wake_signal;

    Ptr<ParallelJob> job;

    pthread_mutex_t mutex;
#ଯଦି !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
    ଅସ୍ଥିର ତାର୍କିକ isActive;
    pthread_cond_t cond_thread_wake;
#endif

    WorkerThread(ThreadPool& thread_pool_, unsigned id_) :
        thread_pool(thread_pool_),
        id(id_),
        posix_thread(0),
        is_created(ମିଥ୍ୟା),
        stop_thread(ମିଥ୍ୟା),
        has_wake_signal(ମିଥ୍ୟା)
#ଯଦି !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
        , isActive(ସତ୍ୟ)
#endif
    {
        CV_LOG_VERBOSE(ଖାଲି, 1, "MainThread: initializing new worker: " << id);
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା res = pthread_mutex_init(&mutex, ଖାଲି);
        ଯଦି (res != 0)
        {
            CV_LOG_ERROR(ଖାଲି, id << ": Can't create thread mutex: res = " << res);
            ଫେରନ୍ତୁ;
        }
#ଯଦି !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
        res = pthread_cond_init(&cond_thread_wake, ଖାଲି);
        ଯଦି (res != 0)
        {
            CV_LOG_ERROR(ଖାଲି, id << ": Can't create thread condition variable: res = " << res);
            ଫେରନ୍ତୁ;
        }
#endif
        res = pthread_create(&posix_thread, ଖାଲି, thread_loop_wrapper, (ଶୂନ୍ୟ*)ଏହି);
        ଯଦି (res != 0)
        {
            CV_LOG_ERROR(ଖାଲି, id << ": Can't spawn new thread: res = " << res);
        }
        ଅନ୍ୟଥା
        {
            is_created = ସତ୍ୟ;
        }
    }

    ~WorkerThread()
    {
        CV_LOG_VERBOSE(ଖାଲି, 1, "MainThread: destroy worker thread: " << id);
        ଯଦି (is_created)
        {
            ଯଦି (!stop_thread)
            {
                pthread_mutex_lock(&mutex);  // to avoid signal miss due pre-check
                stop_thread = ସତ୍ୟ;
                pthread_mutex_unlock(&mutex);
#ଯଦି defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
                pthread_cond_broadcast(&thread_pool.cond_thread_wake);
#ଅନ୍ୟଥା
                pthread_cond_signal(&cond_thread_wake);
#endif
            }
            pthread_join(posix_thread, ଖାଲି);
        }
#ଯଦି !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
        pthread_cond_destroy(&cond_thread_wake);
#endif
        pthread_mutex_destroy(&mutex);
    }

    ଶୂନ୍ୟ thread_body();
    ସ୍ଥିର ଶୂନ୍ୟ* thread_loop_wrapper(ଶୂନ୍ୟ* thread_object)
    {
#ifdef OPENCV_WITH_ITT
        __itt_thread_set_name(cv::format("OpenCVThread-%03d", cv::utils::getThreadID()).c_str());
#endif
        ((WorkerThread*)thread_object)->thread_body();
        ଫେରନ୍ତୁ 0;
    }
};

ଶ୍ରେଣୀ ParallelJob
{
ସର୍ବସାଧାରଣ:
    ParallelJob(ଅଚଳ ThreadPool& thread_pool_, ଅଚଳ Range& range_, ଅଚଳ ParallelLoopBody& body_, ପୂର୍ଣ୍ଣସଂଖ୍ୟା nstripes_) :
        thread_pool(thread_pool_),
        body(body_),
        range(range_),
        nstripes((unsigned)nstripes_),
        is_completed(ମିଥ୍ୟା)
    {
        CV_LOG_VERBOSE(ଖାଲି, 5, "ParallelJob::ParallelJob(" << (ଶୂନ୍ୟ*)ଏହି << ")");
        current_task.store(0, std::memory_order_relaxed);
        active_thread_ଗଣନା.store(0, std::memory_order_relaxed);
        completed_thread_ଗଣନା.store(0, std::memory_order_relaxed);
        dummy0_[0] = 0, dummy1_[0] = 0, dummy2_[0] = 0; // compiler warning
    }

    ~ParallelJob()
    {
        CV_LOG_VERBOSE(ଖାଲି, 5, "ParallelJob::~ParallelJob(" << (ଶୂନ୍ୟ*)ଏହି << ")");
    }

    unsigned execute(ତାର୍କିକ is_worker_thread)
    {
        unsigned executed_tasks = 0;
        ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା task_ଗଣନା = range.size();
        ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା remaining_multiplier = std::min(nstripes,
                std::max(
                        std::min(100u, thread_pool.ଅଙ୍କ_threads * 4),
                        thread_pool.ଅଙ୍କ_threads * 2
                ));  // experimental value
        ପାଇଁ (;;)
        {
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା chunk_size = std::max(1, (task_ଗଣନା - current_task) / remaining_multiplier);
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା id = current_task.fetch_add(chunk_size, std::memory_order_seq_cst);
            ଯଦି (id >= task_ଗଣନା)
                ଭାଙ୍ଗ; // no more free tasks

            executed_tasks += chunk_size;
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା start_id = id;
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା end_id = std::min(task_ଗଣନା, id + chunk_size);
            CV_LOG_VERBOSE(ଖାଲି, 9, "Thread: job " << start_id << "-" << end_id);

            //TODO: if (not pending exception)
            {
                body.operator()(Range(range.start + start_id, range.start + end_id));
            }
            ଯଦି (is_worker_thread && is_completed)
            {
                CV_LOG_ERROR(ଖାଲି, "\t\t\t\tBUG! Job: " << (ଶୂନ୍ୟ*)ଏହି << " " << id << " " << active_thread_ଗଣନା << " " << completed_thread_ଗଣନା);
                CV_Assert(!is_completed); // TODO Dbg this
            }
        }
        ଫେରନ୍ତୁ executed_tasks;
    }

    ଅଚଳ ThreadPool& thread_pool;
    ଅଚଳ ParallelLoopBody& body;
    ଅଚଳ Range range;
    ଅଚଳ unsigned nstripes;

    std::atomic<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> current_task;  // next free part of job
    int64 dummy0_[8];  // avoid cache-line reusing for the same atomics

    std::atomic<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> active_thread_ଗଣନା;  // number of threads worked on this job
    int64 dummy1_[8];  // avoid cache-line reusing for the same atomics

    std::atomic<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> completed_thread_ଗଣନା;  // number of threads completed any activities on this job
    int64 dummy2_[8];  // avoid cache-line reusing for the same atomics

    std::atomic<ତାର୍କିକ> is_completed;

    // TODO exception handling
};


// Disable thread sanitization check when CV_USE_GLOBAL_WORKERS_COND_VAR is not
// set because it triggers as the main thread reads isActive while the children
// thread writes it (but it all works out because a mutex is locked in the main
// thread and isActive re-checked).
// This is to solve issue #19463.
#ଯଦି !defined(CV_USE_GLOBAL_WORKERS_COND_VAR) && defined(__clang__) && defined(__has_feature)
#ଯଦି __has_feature(thread_sanitizer)
__attribute__((no_sanitize("thread")))
#endif
#endif
ଶୂନ୍ୟ WorkerThread::thread_body()
{
    (ଶୂନ୍ୟ)cv::utils::getThreadID(); // notify OpenCV about new thread
    CV_LOG_VERBOSE(ଖାଲି, 5, "Thread: new thread: " << id);

    ତାର୍କିକ allow_active_wait = ସତ୍ୟ;

#ifdef CV_PROFILE_THREADS
    ThreadPool::ThreadStatistics& stat = thread_pool.threads_stat[id + 1];
#endif

    ଯେତେବେଳେ (!stop_thread)
    {
        CV_LOG_VERBOSE(ଖାଲି, 5, "Thread: ... loop iteration: allow_active_wait=" << allow_active_wait << "   has_wake_signal=" << has_wake_signal);
        ଯଦି (allow_active_wait && CV_WORKER_ACTIVE_WAIT > 0)
        {
            allow_active_wait = ମିଥ୍ୟା;
            ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < CV_WORKER_ACTIVE_WAIT; i++)
            {
                ଯଦି (has_wake_signal)
                    ଭାଙ୍ଗ;
                ଯଦି (CV_ACTIVE_WAIT_PAUSE_LIMIT > 0 && (i < CV_ACTIVE_WAIT_PAUSE_LIMIT || (i & 1)))
                    CV_PAUSE(16);
                ଅନ୍ୟଥା
                    CV_YIELD();
            }
        }
        pthread_mutex_lock(&mutex);
#ifdef CV_PROFILE_THREADS
        stat.threadWait = get_Tick_ଗଣନା();
#endif
        ଯେତେବେଳେ (!has_wake_signal) // to handle spurious wakeups
        {
            //CV_LOG_VERBOSE(NULL, 5, "Thread: wait (sleep) ...");
#ଯଦି defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
            pthread_cond_wait(&thread_pool.cond_thread_wake, &mutex);
#ଅନ୍ୟଥା
            isActive = ମିଥ୍ୟା;
            pthread_cond_wait(&cond_thread_wake, &mutex);
            isActive = ସତ୍ୟ;
#endif
            CV_LOG_VERBOSE(ଖାଲି, 5, "Thread: wake ... (has_wake_signal=" << has_wake_signal << " stop_thread=" << stop_thread << ")")
        }
#ifdef CV_PROFILE_THREADS
        stat.threadWake = get_Tick_ଗଣନା();
#endif

        CV_LOG_VERBOSE(ଖାଲି, 5, "Thread: checking for new job");
        ଯଦି (CV_WORKER_ACTIVE_WAIT_THREADS_LIMIT == 0)
            allow_active_wait = ସତ୍ୟ;
        Ptr<ParallelJob> j_ptr; swap(j_ptr, job);
        has_wake_signal = ମିଥ୍ୟା;
        pthread_mutex_unlock(&mutex);

        ଯଦି (!stop_thread)
        {
            ParallelJob* j = j_ptr;
            ଯଦି (j)
            {
                CV_LOG_VERBOSE(ଖାଲି, 5, "Thread: job size=" << j->range.size() << " done=" << j->current_task);
                ଯଦି (j->current_task < j->range.size())
                {
                    ପୂର୍ଣ୍ଣସଂଖ୍ୟା other = j->active_thread_ଗଣନା.fetch_add(1, std::memory_order_seq_cst);
                    CV_LOG_VERBOSE(ଖାଲି, 5, "Thread: processing new job (with " << other << " other threads)"); CV_UNUSED(other);
#ifdef CV_PROFILE_THREADS
                    stat.threadExecuteStart = get_Tick_ଗଣନା();
                    stat.executedTasks = j->execute(ସତ୍ୟ);
                    stat.threadExecuteStop = get_Tick_ଗଣନା();
#ଅନ୍ୟଥା
                    j->execute(ସତ୍ୟ);
#endif
                    ପୂର୍ଣ୍ଣସଂଖ୍ୟା completed = j->completed_thread_ଗଣନା.fetch_add(1, std::memory_order_seq_cst) + 1;
                    ପୂର୍ଣ୍ଣସଂଖ୍ୟା active = j->active_thread_ଗଣନା.load(std::memory_order_acquire);
                    ଯଦି (CV_WORKER_ACTIVE_WAIT_THREADS_LIMIT > 0)
                    {
                        allow_active_wait = ସତ୍ୟ;
                        ଯଦି (active >= CV_WORKER_ACTIVE_WAIT_THREADS_LIMIT && (id & 1) == 0) // turn off a half of threads
                            allow_active_wait = ମିଥ୍ୟା;
                    }
                    CV_LOG_VERBOSE(ଖାଲି, 5, "Thread: completed job processing: " << active << " " << completed);
                    ଯଦି (active == completed)
                    {
                        ତାର୍କିକ need_signal = !j->is_completed;
                        j->is_completed = ସତ୍ୟ;
                        j = ଖାଲି; j_ptr.release();
                        ଯଦି (need_signal)
                        {
                            CV_LOG_VERBOSE(ଖାଲି, 5, "Thread: job finished => notifying the main thread");
                            pthread_mutex_lock(&thread_pool.mutex_notify);  // to avoid signal miss due pre-check condition
                            // empty
                            pthread_mutex_unlock(&thread_pool.mutex_notify);
                            pthread_cond_broadcast/*pthread_cond_signal*/(&thread_pool.cond_thread_task_complete);
                        }
                    }
                }
                ଅନ୍ୟଥା
                {
                    CV_LOG_VERBOSE(ଖାଲି, 5, "Thread: no free job tasks");
                }
            }
        }
#ifdef CV_PROFILE_THREADS
        stat.threadFree = get_Tick_ଗଣନା();
        stat.keepActive = allow_active_wait;
#endif
    }
}

ThreadPool::ThreadPool()
{
#ifdef CV_PROFILE_THREADS
    tickFreq = getTickFrequency();
#endif

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା res = 0;
    res |= pthread_mutex_init(&mutex, ଖାଲି);
    res |= pthread_mutex_init(&mutex_notify, ଖାଲି);
#ଯଦି defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
    res |= pthread_cond_init(&cond_thread_wake, ଖାଲି);
#endif
    res |= pthread_cond_init(&cond_thread_task_complete, ଖାଲି);

    ଯଦି (0 != res)
    {
        CV_LOG_FATAL(ଖାଲି, "Failed to initialize ThreadPool (pthreads)");
    }
    ଅଙ୍କ_threads = default_ସଂଖ୍ୟା_Of_Threads();
}

ତାର୍କିକ ThreadPool::reconfigure_(unsigned new_threads_ଗଣନା)
{
    ଯଦି (new_threads_ଗଣନା == threads.size())
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

    ଯଦି (new_threads_ଗଣନା < threads.size())
    {
        CV_LOG_VERBOSE(ଖାଲି, 1, "MainThread: reduce worker pool: " << threads.size() << " => " << new_threads_ଗଣନା);
        std::vector< Ptr<WorkerThread> > release_threads(threads.size() - new_threads_ଗଣନା);
        ପାଇଁ (size_t i = new_threads_ଗଣନା; i < threads.size(); ++i)
        {
            pthread_mutex_lock(&threads[i]->mutex);  // to avoid signal miss due pre-check
            threads[i]->stop_thread = ସତ୍ୟ;
            threads[i]->has_wake_signal = ସତ୍ୟ;
#ଯଦି !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
            pthread_mutex_unlock(&threads[i]->mutex);
            pthread_cond_broadcast/*pthread_cond_signal*/(&threads[i]->cond_thread_wake); // wake thread
#ଅନ୍ୟଥା
            pthread_mutex_unlock(&threads[i]->mutex);
#endif
            std::swap(threads[i], release_threads[i - new_threads_ଗଣନା]);
        }
#ଯଦି defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
        CV_LOG_VERBOSE(ଖାଲି, 1, "MainThread: notify worker threads about termination...");
        pthread_cond_broadcast(&cond_thread_wake); // wake all threads
#endif
        threads.resize(new_threads_ଗଣନା);
        release_threads.clear();  // calls thread_join which want to lock mutex
        ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }
    ଅନ୍ୟଥା
    {
        CV_LOG_VERBOSE(ଖାଲି, 1, "MainThread: upgrade worker pool: " << threads.size() << " => " << new_threads_ଗଣନା);
        ପାଇଁ (size_t i = threads.size(); i < new_threads_ଗଣନା; ++i)
        {
            threads.push_back(Ptr<WorkerThread>(ନୂତନ WorkerThread(*ଏହି, (unsigned)i))); // spawn more threads
        }
    }
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
}

ThreadPool::~ThreadPool()
{
    reconfigure(0);
    pthread_cond_destroy(&cond_thread_task_complete);
#ଯଦି defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
    pthread_cond_destroy(&cond_thread_wake);
#endif
    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&mutex_notify);
}

ଶୂନ୍ୟ ThreadPool::run(ଅଚଳ Range& range, ଅଚଳ ParallelLoopBody& body, ଦ୍ୱିଗୁଣ nstripes)
{
    CV_LOG_VERBOSE(ଖାଲି, 1, "MainThread: new parallel job: num_threads=" << ଅଙ୍କ_threads << "   range=" << range.size() << "   nstripes=" << nstripes << "   job=" << (ଶୂନ୍ୟ*)job);
#ifdef CV_PROFILE_THREADS
    jobSubmitTime = get_Tick_ଗଣନା();
    threads_stat[0].reset();
    threads_stat[0].threadWait = jobSubmitTime;
    threads_stat[0].threadWake = jobSubmitTime;
#endif
    ଯଦି (get_ଅଙ୍କ_Of_Threads() > 1 &&
        job == ଖାଲି &&
        (range.size() * nstripes >= 2 || (range.size() > 1 && nstripes <= 0))
    )
    {
        pthread_mutex_lock(&mutex);
        ଯଦି (job != ଖାଲି)
        {
            pthread_mutex_unlock(&mutex);
            body(range);
            ଫେରନ୍ତୁ;
        }
        reconfigure_(ଅଙ୍କ_threads - 1);

        {
            CV_LOG_VERBOSE(ଖାଲି, 1, "MainThread: initialize parallel job: " << range.size());
            job = Ptr<ParallelJob>(ନୂତନ ParallelJob(*ଏହି, range, body, nstripes));
            pthread_mutex_unlock(&mutex);

            CV_LOG_VERBOSE(ଖାଲି, 5, "MainThread: wake worker threads...");
            size_t ଅଙ୍କ_threads_to_wake = std::min(static_cast<size_t>(range.size()), threads.size());
            ପାଇଁ (size_t i = 0; i < ଅଙ୍କ_threads_to_wake; ++i)
            {
                ଯଦି (job->current_task >= job->range.size())
                    ଭାଙ୍ଗ;
                WorkerThread& thread = *(threads[i].get());
                ଯଦି (
#ଯଦି defined(__clang__) && defined(__has_feature)
#ଯଦି __has_feature(thread_sanitizer)
                        1 ||  // Robust workaround to avoid data race warning of `thread.job`
#endif
#endif
#ଯଦି !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
                        thread.isActive ||
#endif
                        thread.has_wake_signal
                        || !thread.job.empty()  // #10881
                )
                {
                    pthread_mutex_lock(&thread.mutex);
                    thread.job = job;
#ଯଦି !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
                    ତାର୍କିକ isActive = thread.isActive;
#endif
                    thread.has_wake_signal = ସତ୍ୟ;
#ifdef CV_PROFILE_THREADS
                    threads_stat[i + 1].reset();
#endif
                    pthread_mutex_unlock(&thread.mutex);
#ଯଦି !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
                    ଯଦି (!isActive)
                    {
                        pthread_cond_broadcast/*pthread_cond_signal*/(&thread.cond_thread_wake); // wake thread
                    }
#endif
                }
                ଅନ୍ୟଥା
                {
                    CV_Assert(thread.job.empty());
                    thread.job = job;
                    thread.has_wake_signal = ସତ୍ୟ;
#ifdef CV_PROFILE_THREADS
                    threads_stat[i + 1].reset();
#endif
#ଯଦି !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
                    pthread_cond_broadcast/*pthread_cond_signal*/(&thread.cond_thread_wake); // wake thread
#endif
                }
            }
#ifdef CV_PROFILE_THREADS
            threads_stat[0].threadPing = get_Tick_ଗଣନା();
#endif
#ଯଦି defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
            pthread_cond_broadcast(&cond_thread_wake); // wake all threads
#endif
#ifdef CV_PROFILE_THREADS
            threads_stat[0].threadWake = get_Tick_ଗଣନା();
#endif
            CV_LOG_VERBOSE(ଖାଲି, 5, "MainThread: wake worker threads... (done)");

            {
                ParallelJob& j = *(ଏହି->job);
#ifdef CV_PROFILE_THREADS
                threads_stat[0].threadExecuteStart = get_Tick_ଗଣନା();
                threads_stat[0].executedTasks = j.execute(ମିଥ୍ୟା);
                threads_stat[0].threadExecuteStop = get_Tick_ଗଣନା();
#ଅନ୍ୟଥା
                j.execute(ମିଥ୍ୟା);
#endif
                CV_Assert(j.current_task >= j.range.size());
                CV_LOG_VERBOSE(ଖାଲି, 5, "MainThread: complete self-tasks: " << j.active_thread_ଗଣନା << " " << j.completed_thread_ଗଣନା);
                ଯଦି (job->is_completed || j.active_thread_ଗଣନା == 0)
                {
                    job->is_completed = ସତ୍ୟ;
                    CV_LOG_VERBOSE(ଖାଲି, 5, "MainThread: no WIP worker threads");
                }
                ଅନ୍ୟଥା
                {
                    ଯଦି (CV_MAIN_THREAD_ACTIVE_WAIT > 0)
                    {
                        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < CV_MAIN_THREAD_ACTIVE_WAIT; i++)  // don't spin too much in any case (inaccurate getTickCount())
                        {
                            ଯଦି (job->is_completed)
                            {
                                CV_LOG_VERBOSE(ଖାଲି, 5, "MainThread: job finalize (active wait) " << j.active_thread_ଗଣନା << " " << j.completed_thread_ଗଣନା);
                                ଭାଙ୍ଗ;
                            }
                            ଯଦି (CV_ACTIVE_WAIT_PAUSE_LIMIT > 0 && (i < CV_ACTIVE_WAIT_PAUSE_LIMIT || (i & 1)))
                                CV_PAUSE(16);
                            ଅନ୍ୟଥା
                                CV_YIELD();
                        }
                    }
                    ଯଦି (!job->is_completed)
                    {
                        CV_LOG_VERBOSE(ଖାଲି, 5, "MainThread: prepare wait " << j.active_thread_ଗଣନା << " " << j.completed_thread_ଗଣନା);
                        pthread_mutex_lock(&mutex_notify);
                        ପାଇଁ (;;)
                        {
                            ଯଦି (job->is_completed)
                            {
                                CV_LOG_VERBOSE(ଖାଲି, 5, "MainThread: job finalize (wait) " << j.active_thread_ଗଣନା << " " << j.completed_thread_ଗଣନା);
                                ଭାଙ୍ଗ;
                            }
                            CV_LOG_VERBOSE(ଖାଲି, 5, "MainThread: wait completion (sleep) ...");
                            pthread_cond_wait(&cond_thread_task_complete, &mutex_notify);
                            CV_LOG_VERBOSE(ଖାଲି, 5, "MainThread: wake");
                        }
                        pthread_mutex_unlock(&mutex_notify);
                    }
                }
            }
#ifdef CV_PROFILE_THREADS
            threads_stat[0].threadFree = get_Tick_ଗଣନା();
            std::cout << "Job: sz=" << range.size() << " nstripes=" << nstripes << "    Time: " << (threads_stat[0].threadFree - jobSubmitTime) / tickFreq * 1e6 << " usec" << std::endl;
            ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)threads.size() + 1; i++)
            {
                threads_stat[i].dump(i - 1, jobSubmitTime, tickFreq);
            }
#endif
            ଯଦି (job)
            {
                pthread_mutex_lock(&mutex);
                CV_LOG_VERBOSE(ଖାଲି, 5, "MainThread: job release");
                CV_Assert(job->is_completed);
                job.release();
                pthread_mutex_unlock(&mutex);
            }
        }
    }
    ଅନ୍ୟଥା
    {
        body(range);
    }
}

size_t ThreadPool::get_ଅଙ୍କ_Of_Threads()
{
    ଫେରନ୍ତୁ ଅଙ୍କ_threads;
}

ଶୂନ୍ୟ ThreadPool::set_ଅଙ୍କ_Of_Threads(unsigned n)
{
    ଯଦି (n != ଅଙ୍କ_threads)
    {
        ଅଙ୍କ_threads = n;
        ଯଦି (n == 1)
           ଯଦି (job == ଖାଲି) reconfigure(0);  // stop worker threads immediately
    }
}

size_t parallel_pthreads_get_threads_ଅଙ୍କ()
{
    ଫେରନ୍ତୁ ThreadPool::instance().get_ଅଙ୍କ_Of_Threads();
}

ଶୂନ୍ୟ parallel_pthreads_set_threads_ଅଙ୍କ(ପୂର୍ଣ୍ଣସଂଖ୍ୟା num)
{
    ଯଦି(num < 0)
    {
        ThreadPool::instance().set_ଅଙ୍କ_Of_Threads(0);
    }
    ଅନ୍ୟଥା
    {
        ThreadPool::instance().set_ଅଙ୍କ_Of_Threads(unsigned(num));
    }
}

ଶୂନ୍ୟ parallel_for_pthreads(ଅଚଳ Range& range, ଅଚଳ ParallelLoopBody& body, ଦ୍ୱିଗୁଣ nstripes)
{
    ThreadPool::instance().run(range, body, nstripes);
}

}

#endif
