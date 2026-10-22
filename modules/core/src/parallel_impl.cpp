// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ਅਨ੍ਤਰ੍ਭਾਵ "precomp.hpp"

#ਅਨ੍ਤਰ੍ਭਾਵ "parallel_impl.hpp"

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_PTHREADS_PF
#ਅਨ੍ਤਰ੍ਭਾਵ <pthread.h>

#ਅਨ੍ਤਰ੍ਭਾਵ <opencv2/core/utils/configuration.private.hpp>

#ਅਨ੍ਤਰ੍ਭਾਵ <opencv2/core/utils/logger.defines.hpp>
//#undef CV_LOG_STRIP_LEVEL
//#define CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_VERBOSE + 1
#ਅਨ੍ਤਰ੍ਭਾਵ <opencv2/core/utils/logger.hpp>

#ਅਨ੍ਤਰ੍ਭਾਵ <opencv2/core/utils/trace.private.hpp>

//#define CV_PROFILE_THREADS 64
//#define getTickCount getCPUTickCount  // use this if getTickCount() calls are expensive (and getCPUTickCount() is accurate)

//#define CV_USE_GLOBAL_WORKERS_COND_VAR  // not effective on many-core systems (10+)

#ਅਨ੍ਤਰ੍ਭਾਵ <atomic>

// Spin lock's OS-level yield
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ DECLARE_CV_YIELD
DECLARE_CV_YIELD
#ਜੇ_ਅੰਤ
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ CV_YIELD
# include <thread>
# define CV_YIELD() std::this_thread::yield()
#ਜੇ_ਅੰਤ // CV_YIELD

// Spin lock's CPU-level yield (required for Hyper-Threading)
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ DECLARE_CV_PAUSE
DECLARE_CV_PAUSE
#ਜੇ_ਅੰਤ
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ CV_PAUSE
# ਜੇ defined __GNUC__ && (defined __i386__ || defined __x86_64__)
#   include <x86intrin.h> /* for __rdtsc */
#   ਜੇ !defined(__SSE2__)
      ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ cv_non_sse_mm_pause() { __asm__ __volatile__ ("rep; nop"); }
#     define _mm_pause cv_non_sse_mm_pause
#   endif
// With Skylake CPUs and above, _mm_pause takes 140 cycles so no need for a loop.
#   define CV_PAUSE(v) ਕਰੋ { (ਖਾਲੀ)v; _mm_pause(); } ਜਦੋਂ (0)
# elif defined __GNUC__ && defined __aarch64__
#   define CV_PAUSE(v) ਕਰੋ { ਲਈ (ਪੂਰਨ __delay = (v); __delay > 0; --__delay) { asm ਅਸਥਿਰ("yield" ::: "memory"); } } ਜਦੋਂ (0)
# elif defined __GNUC__ && defined __arm__
#   define CV_PAUSE(v) ਕਰੋ { ਲਈ (ਪੂਰਨ __delay = (v); __delay > 0; --__delay) { asm ਅਸਥਿਰ("" ::: "memory"); } } ਜਦੋਂ (0)
# elif defined __GNUC__ && defined __mips__ && __mips_isa_rev >= 2
#   define CV_PAUSE(v) ਕਰੋ { ਲਈ (ਪੂਰਨ __delay = (v); __delay > 0; --__delay) { asm ਅਸਥਿਰ("pause" ::: "memory"); } } ਜਦੋਂ (0)
# elif defined __APPLE__ && defined __POWERPC__
#   define CV_PAUSE(v) ਕਰੋ { ਲਈ (ਪੂਰਨ __delay = (v); __delay > 0; --__delay) { asm ਅਸਥਿਰ("or r27,r27,r27" ::: "memory"); } } ਜਦੋਂ (0)
# elif defined __GNUC__ && defined __PPC64__
#   define CV_PAUSE(v) ਕਰੋ { ਲਈ (ਪੂਰਨ __delay = (v); __delay > 0; --__delay) { asm ਅਸਥਿਰ("or 27,27,27" ::: "memory"); } } ਜਦੋਂ (0)
# elif defined __GNUC__ && defined __riscv
// PAUSE HINT is not part of RISC-V ISA yet, but is under discussion now. For details see:
// https://github.com/riscv/riscv-isa-manual/pull/398
// https://github.com/riscv/riscv-isa-manual/issues/43
// #   define CV_PAUSE(v) do { for (int __delay = (v); __delay > 0; --__delay) { asm volatile("pause"); } } while (0)
#   define CV_PAUSE(v) ਕਰੋ { ਲਈ (ਪੂਰਨ __delay = (v); __delay > 0; --__delay) { asm ਅਸਥਿਰ("nop"); } } ਜਦੋਂ (0)
# elif defined __GNUC__ && defined __loongarch__
#   define CV_PAUSE(v) ਕਰੋ { ਲਈ (ਪੂਰਨ __delay = (v); __delay > 0; --__delay) { asm ਅਸਥਿਰ("nop"); } } ਜਦੋਂ (0)
# ਨਹੀਂਤਾਂ
#   warning "Can't detect 'pause' (CPU-yield) instruction on the target platform. Specify CV_PAUSE() definition via compiler flags."
#   define CV_PAUSE(...) ਕਰੋ { /* no-op: works, but not effective */ } ਜਦੋਂ (0)
# endif
#ਜੇ_ਅੰਤ // CV_PAUSE


ਨਾਮਾਕਾਸ਼ cv
{

ਸਥਿਰ ਪੂਰਨ CV_ਸਰਗਰਮ_WAIT_PAUSE_ਹੱਦਬੰਦੀ = (ਪੂਰਨ)utils::get_Configuration_Parameter_ਮਾਪ_T("OPENCV_THREAD_POOL_ACTIVE_WAIT_PAUSE_LIMIT", 16);  // iterations
ਸਥਿਰ ਪੂਰਨ CV_ਕਾਮਾ_ਸਰਗਰਮ_WAIT = (ਪੂਰਨ)utils::get_Configuration_Parameter_ਮਾਪ_T("OPENCV_THREAD_POOL_ACTIVE_WAIT_WORKER", 2000);  // iterations
ਸਥਿਰ ਪੂਰਨ CV_MAIN_THREAD_ਸਰਗਰਮ_WAIT = (ਪੂਰਨ)utils::get_Configuration_Parameter_ਮਾਪ_T("OPENCV_THREAD_POOL_ACTIVE_WAIT_MAIN", 10000); // iterations

ਸਥਿਰ ਪੂਰਨ CV_ਕਾਮਾ_ਸਰਗਰਮ_WAIT_ਧਾਗੇ_ਹੱਦਬੰਦੀ = (ਪੂਰਨ)utils::get_Configuration_Parameter_ਮਾਪ_T("OPENCV_THREAD_POOL_ACTIVE_WAIT_THREADS_LIMIT", 0); // number of real cores

ਵਰਗ ਕਾਮਾ_Thread;
ਵਰਗ Parallel_ਕੰਮ;

ਵਰਗ ThreadPool
{
ਜਨਤਕ:
    ਸਥਿਰ ThreadPool& instance()
    {
        CV_SINGLETON_LAZY_INIT_REF(ThreadPool, ਨਵਾਂ ThreadPool())
    }

    ਸਥਿਰ ਖਾਲੀ stop()
    {
        ThreadPool& manager = instance();
        manager.reconfigure(0);
    }

    ਖਾਲੀ reconfigure(ਬਿਨਾਨਿਸ਼ਾਨੀ new_ਧਾਗੇ_ਗਿਣਤੀ)
    {
        ਜੇ (new_ਧਾਗੇ_ਗਿਣਤੀ == ਧਾਗੇ.size())
            ਵਾਪਸ;
        pthread_mutex_lock(&mutex);
        reconfigure_(new_ਧਾਗੇ_ਗਿਣਤੀ);
        pthread_mutex_unlock(&mutex);
    }
    ਤਾਰਕਿਕ reconfigure_(ਬਿਨਾਨਿਸ਼ਾਨੀ new_ਧਾਗੇ_ਗਿਣਤੀ); // internal implementation

    ਖਾਲੀ ਲੜੀਵਾਰ(ਅਚਲ Range& range, ਅਚਲ ParallelLoopBody& body, ਦੁੱਗਣਾ nstripes);

    size_t get_ਅੰਕ_ਦਾ_ਧਾਗੇ();

    ਖਾਲੀ ਸੈੱਟ_ਅੰਕ_ਦਾ_ਧਾਗੇ(ਬਿਨਾਨਿਸ਼ਾਨੀ n);

    ThreadPool();

    ~ThreadPool();

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_ਧਾਗੇ_2;

    pthread_mutex_t mutex;  // guards fields (job/threads) from non-worker threads (concurrent parallel_for calls)
#ਜੇ defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
    pthread_cond_t cond_thread_wake;
#ਜੇ_ਅੰਤ

    pthread_mutex_t mutex_notify;
    pthread_cond_t cond_thread_task_complete;

    std::vector< Ptr<ਕਾਮਾ_Thread> > ਧਾਗੇ;

    Ptr<Parallel_ਕੰਮ> ਕੰਮ;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CV_PROFILE_ਧਾਗੇ
    ਦੁੱਗਣਾ tickFreq;
    int64 ਕੰਮ_Submit_ਸਮਾਂ;
    ਬਣਤਰ ThreadStatistics
    {
        ThreadStatistics() : threadWait(0)
        {
            reset();
        }
        ਖਾਲੀ reset()
        {
            threadWake = 0;
            thread_Execute_ਸ਼ੁਰੂ = 0;
            threadExecuteStop = 0;
            executedTasks = 0;
            keep_ਸਰਗਰਮ = ਝੂਠ;
            threadPing = get_Tick_ਗਿਣਤੀ();
        }
        int64 threadWait; // don't reset by default
        int64 threadPing; // don't reset by default
        int64 threadWake;
        int64 thread_Execute_ਸ਼ੁਰੂ;
        int64 threadExecuteStop;
        int64 threadFree;
        ਬਿਨਾਨਿਸ਼ਾਨੀ executedTasks;
        ਤਾਰਕਿਕ keep_ਸਰਗਰਮ;

        int64 dummy_[8]; // separate cache lines

        ਖਾਲੀ dump(ਪੂਰਨ id, int64 ਅਧਾਰ_ਸਮਾਂ, ਦੁੱਗਣਾ tickFreq)
        {
            ਜੇ (id < 0)
                std::ਬਾਹਰ << "Main: ";
            ਨਹੀਂਤਾਂ
                ਲਿਖੋ("T%03d: ", id + 2);
            ਲਿਖੋ("wait=% 10.1f   ping=% 6.1f",
                    threadWait > 0 ? (threadWait - ਅਧਾਰ_ਸਮਾਂ) / tickFreq * 1e6 : -0.0,
                    threadPing > 0 ? (threadPing - ਅਧਾਰ_ਸਮਾਂ) / tickFreq * 1e6 : -0.0);
            ਜੇ (threadWake > 0)
                ਲਿਖੋ("   wake=% 6.1f",
                    (threadWake > 0 ? (threadWake - ਅਧਾਰ_ਸਮਾਂ) / tickFreq * 1e6 : -0.0));
            ਜੇ (thread_Execute_ਸ਼ੁਰੂ > 0)
            {
                ਲਿਖੋ("   exec=% 6.1f - % 6.1f   tasksDone=%5u   free=% 6.1f",
                    (thread_Execute_ਸ਼ੁਰੂ > 0 ? (thread_Execute_ਸ਼ੁਰੂ - ਅਧਾਰ_ਸਮਾਂ) / tickFreq * 1e6 : -0.0),
                    (threadExecuteStop > 0 ? (threadExecuteStop - ਅਧਾਰ_ਸਮਾਂ) / tickFreq * 1e6 : -0.0),
                    executedTasks,
                    (threadFree > 0 ? (threadFree - ਅਧਾਰ_ਸਮਾਂ) / tickFreq * 1e6 : -0.0));
                ਜੇ (id >= 0)
                    ਲਿਖੋ(" active=%s\n", keep_ਸਰਗਰਮ ? "true" : "false");
                ਨਹੀਂਤਾਂ
                    ਲਿਖੋ("\n");
            }
            ਨਹੀਂਤਾਂ
                ਲਿਖੋ("   ------------------------------------------------------------------------------\n");
        }
    };
    ThreadStatistics ਧਾਗੇ_stat[CV_PROFILE_ਧਾਗੇ]; // 0 - main thread, 1..N - worker threads
#ਜੇ_ਅੰਤ

};

ਵਰਗ ਕਾਮਾ_Thread
{
ਜਨਤਕ:
    ThreadPool& thread_pool;
    ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ id;
    pthread_t posix_thread;
    ਤਾਰਕਿਕ is_created;

    std::atomic<ਤਾਰਕਿਕ> stop_thread;

    std::atomic<ਤਾਰਕਿਕ> has_wake_signal;

    Ptr<Parallel_ਕੰਮ> ਕੰਮ;

    pthread_mutex_t mutex;
#ਜੇ !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
    ਅਸਥਿਰ ਤਾਰਕਿਕ is_ਸਰਗਰਮ_2;
    pthread_cond_t cond_thread_wake;
#ਜੇ_ਅੰਤ

    ਕਾਮਾ_Thread(ThreadPool& thread_pool_, ਬਿਨਾਨਿਸ਼ਾਨੀ id_) :
        thread_pool(thread_pool_),
        id(id_),
        posix_thread(0),
        is_created(ਝੂਠ),
        stop_thread(ਝੂਠ),
        has_wake_signal(ਝੂਠ)
#ਜੇ !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
        , is_ਸਰਗਰਮ_2(ਸੱਚ)
#ਜੇ_ਅੰਤ
    {
        CV_LOG_VERBOSE(ਨਿਰਾ, 1, "MainThread: initializing new worker: " << id);
        ਪੂਰਨ res = pthread_mutex_init(&mutex, ਨਿਰਾ);
        ਜੇ (res != 0)
        {
            CV_LOG_ERROR(ਨਿਰਾ, id << ": Can't create thread mutex: res = " << res);
            ਵਾਪਸ;
        }
#ਜੇ !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
        res = pthread_cond_init(&cond_thread_wake, ਨਿਰਾ);
        ਜੇ (res != 0)
        {
            CV_LOG_ERROR(ਨਿਰਾ, id << ": Can't create thread condition variable: res = " << res);
            ਵਾਪਸ;
        }
#ਜੇ_ਅੰਤ
        res = pthread_create(&posix_thread, ਨਿਰਾ, thread_loop_wrapper, (ਖਾਲੀ*)ਇਹ);
        ਜੇ (res != 0)
        {
            CV_LOG_ERROR(ਨਿਰਾ, id << ": Can't spawn new thread: res = " << res);
        }
        ਨਹੀਂਤਾਂ
        {
            is_created = ਸੱਚ;
        }
    }

    ~ਕਾਮਾ_Thread()
    {
        CV_LOG_VERBOSE(ਨਿਰਾ, 1, "MainThread: destroy worker thread: " << id);
        ਜੇ (is_created)
        {
            ਜੇ (!stop_thread)
            {
                pthread_mutex_lock(&mutex);  // to avoid signal miss due pre-check
                stop_thread = ਸੱਚ;
                pthread_mutex_unlock(&mutex);
#ਜੇ defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
                pthread_cond_broadcast(&thread_pool.cond_thread_wake);
#ਨਹੀਂਤਾਂ
                pthread_cond_signal(&cond_thread_wake);
#ਜੇ_ਅੰਤ
            }
            pthread_join(posix_thread, ਨਿਰਾ);
        }
#ਜੇ !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
        pthread_cond_destroy(&cond_thread_wake);
#ਜੇ_ਅੰਤ
        pthread_mutex_destroy(&mutex);
    }

    ਖਾਲੀ thread_body();
    ਸਥਿਰ ਖਾਲੀ* thread_loop_wrapper(ਖਾਲੀ* thread_object)
    {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ OPENCV_WITH_ITT
        __itt_thread_set_name(cv::format("OpenCVThread-%03d", cv::utils::getThreadID()).c_str());
#ਜੇ_ਅੰਤ
        ((ਕਾਮਾ_Thread*)thread_object)->thread_body();
        ਵਾਪਸ 0;
    }
};

ਵਰਗ Parallel_ਕੰਮ
{
ਜਨਤਕ:
    Parallel_ਕੰਮ(ਅਚਲ ThreadPool& thread_pool_, ਅਚਲ Range& range_, ਅਚਲ ParallelLoopBody& body_, ਪੂਰਨ nstripes_) :
        thread_pool(thread_pool_),
        body(body_),
        range(range_),
        nstripes((ਬਿਨਾਨਿਸ਼ਾਨੀ)nstripes_),
        is_completed(ਝੂਠ)
    {
        CV_LOG_VERBOSE(ਨਿਰਾ, 5, "ParallelJob::ParallelJob(" << (ਖਾਲੀ*)ਇਹ << ")");
        current_task.store(0, std::memory_order_relaxed);
        ਸਰਗਰਮ_thread_ਗਿਣਤੀ.store(0, std::memory_order_relaxed);
        completed_thread_ਗਿਣਤੀ.store(0, std::memory_order_relaxed);
        dummy0_[0] = 0, dummy1_[0] = 0, dummy2_[0] = 0; // compiler warning
    }

    ~Parallel_ਕੰਮ()
    {
        CV_LOG_VERBOSE(ਨਿਰਾ, 5, "ParallelJob::~ParallelJob(" << (ਖਾਲੀ*)ਇਹ << ")");
    }

    ਬਿਨਾਨਿਸ਼ਾਨੀ execute(ਤਾਰਕਿਕ is_ਕਾਮਾ_thread)
    {
        ਬਿਨਾਨਿਸ਼ਾਨੀ executed_tasks = 0;
        ਅਚਲ ਪੂਰਨ task_ਗਿਣਤੀ = range.size();
        ਅਚਲ ਪੂਰਨ ਬਚਿਆ_multiplier = std::min(nstripes,
                std::max(
                        std::min(100u, thread_pool.ਅੰਕ_ਧਾਗੇ_2 * 4),
                        thread_pool.ਅੰਕ_ਧਾਗੇ_2 * 2
                ));  // experimental value
        ਲਈ (;;)
        {
            ਪੂਰਨ chunk_ਮਾਪ = std::max(1, (task_ਗਿਣਤੀ - current_task) / ਬਚਿਆ_multiplier);
            ਪੂਰਨ id = current_task.fetch_add(chunk_ਮਾਪ, std::memory_order_seq_cst);
            ਜੇ (id >= task_ਗਿਣਤੀ)
                ਰੋਕੋ; // no more free tasks

            executed_tasks += chunk_ਮਾਪ;
            ਪੂਰਨ ਸ਼ੁਰੂ_id = id;
            ਪੂਰਨ ਸਿਰਾ_id = std::min(task_ਗਿਣਤੀ, id + chunk_ਮਾਪ);
            CV_LOG_VERBOSE(ਨਿਰਾ, 9, "Thread: job " << ਸ਼ੁਰੂ_id << "-" << ਸਿਰਾ_id);

            //TODO: if (not pending exception)
            {
                body.ਸੰਚਾਲਕ()(Range(range.ਸ਼ੁਰੂ + ਸ਼ੁਰੂ_id, range.ਸ਼ੁਰੂ + ਸਿਰਾ_id));
            }
            ਜੇ (is_ਕਾਮਾ_thread && is_completed)
            {
                CV_LOG_ERROR(ਨਿਰਾ, "\t\t\t\tBUG! Job: " << (ਖਾਲੀ*)ਇਹ << " " << id << " " << ਸਰਗਰਮ_thread_ਗਿਣਤੀ << " " << completed_thread_ਗਿਣਤੀ);
                CV_Assert(!is_completed); // TODO Dbg this
            }
        }
        ਵਾਪਸ executed_tasks;
    }

    ਅਚਲ ThreadPool& thread_pool;
    ਅਚਲ ParallelLoopBody& body;
    ਅਚਲ Range range;
    ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ nstripes;

    std::atomic<ਪੂਰਨ> current_task;  // next free part of job
    int64 dummy0_[8];  // avoid cache-line reusing for the same atomics

    std::atomic<ਪੂਰਨ> ਸਰਗਰਮ_thread_ਗਿਣਤੀ;  // number of threads worked on this job
    int64 dummy1_[8];  // avoid cache-line reusing for the same atomics

    std::atomic<ਪੂਰਨ> completed_thread_ਗਿਣਤੀ;  // number of threads completed any activities on this job
    int64 dummy2_[8];  // avoid cache-line reusing for the same atomics

    std::atomic<ਤਾਰਕਿਕ> is_completed;

    // TODO exception handling
};


// Disable thread sanitization check when CV_USE_GLOBAL_WORKERS_COND_VAR is not
// set because it triggers as the main thread reads isActive while the children
// thread writes it (but it all works out because a mutex is locked in the main
// thread and isActive re-checked).
// This is to solve issue #19463.
#ਜੇ !defined(CV_USE_GLOBAL_WORKERS_COND_VAR) && defined(__clang__) && defined(__has_feature)
#ਜੇ __has_feature(thread_sanitizer)
__attribute__((no_sanitize("thread")))
#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ
ਖਾਲੀ ਕਾਮਾ_Thread::thread_body()
{
    (ਖਾਲੀ)cv::utils::getThreadID(); // notify OpenCV about new thread
    CV_LOG_VERBOSE(ਨਿਰਾ, 5, "Thread: new thread: " << id);

    ਤਾਰਕਿਕ allow_ਸਰਗਰਮ_wait = ਸੱਚ;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CV_PROFILE_ਧਾਗੇ
    ThreadPool::ThreadStatistics& stat = thread_pool.ਧਾਗੇ_stat[id + 1];
#ਜੇ_ਅੰਤ

    ਜਦੋਂ (!stop_thread)
    {
        CV_LOG_VERBOSE(ਨਿਰਾ, 5, "Thread: ... loop iteration: allow_active_wait=" << allow_ਸਰਗਰਮ_wait << "   has_wake_signal=" << has_wake_signal);
        ਜੇ (allow_ਸਰਗਰਮ_wait && CV_ਕਾਮਾ_ਸਰਗਰਮ_WAIT > 0)
        {
            allow_ਸਰਗਰਮ_wait = ਝੂਠ;
            ਲਈ (ਪੂਰਨ i = 0; i < CV_ਕਾਮਾ_ਸਰਗਰਮ_WAIT; i++)
            {
                ਜੇ (has_wake_signal)
                    ਰੋਕੋ;
                ਜੇ (CV_ਸਰਗਰਮ_WAIT_PAUSE_ਹੱਦਬੰਦੀ > 0 && (i < CV_ਸਰਗਰਮ_WAIT_PAUSE_ਹੱਦਬੰਦੀ || (i & 1)))
                    CV_PAUSE(16);
                ਨਹੀਂਤਾਂ
                    CV_YIELD();
            }
        }
        pthread_mutex_lock(&mutex);
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CV_PROFILE_ਧਾਗੇ
        stat.threadWait = get_Tick_ਗਿਣਤੀ();
#ਜੇ_ਅੰਤ
        ਜਦੋਂ (!has_wake_signal) // to handle spurious wakeups
        {
            //CV_LOG_VERBOSE(NULL, 5, "Thread: wait (sleep) ...");
#ਜੇ defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
            pthread_cond_wait(&thread_pool.cond_thread_wake, &mutex);
#ਨਹੀਂਤਾਂ
            is_ਸਰਗਰਮ_2 = ਝੂਠ;
            pthread_cond_wait(&cond_thread_wake, &mutex);
            is_ਸਰਗਰਮ_2 = ਸੱਚ;
#ਜੇ_ਅੰਤ
            CV_LOG_VERBOSE(ਨਿਰਾ, 5, "Thread: wake ... (has_wake_signal=" << has_wake_signal << " stop_thread=" << stop_thread << ")")
        }
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CV_PROFILE_ਧਾਗੇ
        stat.threadWake = get_Tick_ਗਿਣਤੀ();
#ਜੇ_ਅੰਤ

        CV_LOG_VERBOSE(ਨਿਰਾ, 5, "Thread: checking for new job");
        ਜੇ (CV_ਕਾਮਾ_ਸਰਗਰਮ_WAIT_ਧਾਗੇ_ਹੱਦਬੰਦੀ == 0)
            allow_ਸਰਗਰਮ_wait = ਸੱਚ;
        Ptr<Parallel_ਕੰਮ> j_ptr; swap(j_ptr, ਕੰਮ);
        has_wake_signal = ਝੂਠ;
        pthread_mutex_unlock(&mutex);

        ਜੇ (!stop_thread)
        {
            Parallel_ਕੰਮ* j = j_ptr;
            ਜੇ (j)
            {
                CV_LOG_VERBOSE(ਨਿਰਾ, 5, "Thread: job size=" << j->range.size() << " done=" << j->current_task);
                ਜੇ (j->current_task < j->range.size())
                {
                    ਪੂਰਨ other = j->ਸਰਗਰਮ_thread_ਗਿਣਤੀ.fetch_add(1, std::memory_order_seq_cst);
                    CV_LOG_VERBOSE(ਨਿਰਾ, 5, "Thread: processing new job (with " << other << " other threads)"); CV_ਅਣਵਰਤਿਆ(other);
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CV_PROFILE_ਧਾਗੇ
                    stat.thread_Execute_ਸ਼ੁਰੂ = get_Tick_ਗਿਣਤੀ();
                    stat.executedTasks = j->execute(ਸੱਚ);
                    stat.threadExecuteStop = get_Tick_ਗਿਣਤੀ();
#ਨਹੀਂਤਾਂ
                    j->execute(ਸੱਚ);
#ਜੇ_ਅੰਤ
                    ਪੂਰਨ completed = j->completed_thread_ਗਿਣਤੀ.fetch_add(1, std::memory_order_seq_cst) + 1;
                    ਪੂਰਨ ਸਰਗਰਮ = j->ਸਰਗਰਮ_thread_ਗਿਣਤੀ.load(std::memory_order_acquire);
                    ਜੇ (CV_ਕਾਮਾ_ਸਰਗਰਮ_WAIT_ਧਾਗੇ_ਹੱਦਬੰਦੀ > 0)
                    {
                        allow_ਸਰਗਰਮ_wait = ਸੱਚ;
                        ਜੇ (ਸਰਗਰਮ >= CV_ਕਾਮਾ_ਸਰਗਰਮ_WAIT_ਧਾਗੇ_ਹੱਦਬੰਦੀ && (id & 1) == 0) // turn off a half of threads
                            allow_ਸਰਗਰਮ_wait = ਝੂਠ;
                    }
                    CV_LOG_VERBOSE(ਨਿਰਾ, 5, "Thread: completed job processing: " << ਸਰਗਰਮ << " " << completed);
                    ਜੇ (ਸਰਗਰਮ == completed)
                    {
                        ਤਾਰਕਿਕ need_signal = !j->is_completed;
                        j->is_completed = ਸੱਚ;
                        j = ਨਿਰਾ; j_ptr.release();
                        ਜੇ (need_signal)
                        {
                            CV_LOG_VERBOSE(ਨਿਰਾ, 5, "Thread: job finished => notifying the main thread");
                            pthread_mutex_lock(&thread_pool.mutex_notify);  // to avoid signal miss due pre-check condition
                            //ਖਾਲੀ_ਥਾਵਾਂ
                            pthread_mutex_unlock(&thread_pool.mutex_notify);
                            pthread_cond_broadcast/*pthread_cond_signal*/(&thread_pool.cond_thread_task_complete);
                        }
                    }
                }
                ਨਹੀਂਤਾਂ
                {
                    CV_LOG_VERBOSE(ਨਿਰਾ, 5, "Thread: no free job tasks");
                }
            }
        }
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CV_PROFILE_ਧਾਗੇ
        stat.threadFree = get_Tick_ਗਿਣਤੀ();
        stat.keep_ਸਰਗਰਮ = allow_ਸਰਗਰਮ_wait;
#ਜੇ_ਅੰਤ
    }
}

ThreadPool::ThreadPool()
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CV_PROFILE_ਧਾਗੇ
    tickFreq = getTickFrequency();
#ਜੇ_ਅੰਤ

    ਪੂਰਨ res = 0;
    res |= pthread_mutex_init(&mutex, ਨਿਰਾ);
    res |= pthread_mutex_init(&mutex_notify, ਨਿਰਾ);
#ਜੇ defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
    res |= pthread_cond_init(&cond_thread_wake, ਨਿਰਾ);
#ਜੇ_ਅੰਤ
    res |= pthread_cond_init(&cond_thread_task_complete, ਨਿਰਾ);

    ਜੇ (0 != res)
    {
        CV_LOG_FATAL(ਨਿਰਾ, "Failed to initialize ThreadPool (pthreads)");
    }
    ਅੰਕ_ਧਾਗੇ_2 = default_ਸੰਖਿਆ_ਦਾ_ਧਾਗੇ();
}

ਤਾਰਕਿਕ ThreadPool::reconfigure_(ਬਿਨਾਨਿਸ਼ਾਨੀ new_ਧਾਗੇ_ਗਿਣਤੀ)
{
    ਜੇ (new_ਧਾਗੇ_ਗਿਣਤੀ == ਧਾਗੇ.size())
        ਵਾਪਸ ਝੂਠ;

    ਜੇ (new_ਧਾਗੇ_ਗਿਣਤੀ < ਧਾਗੇ.size())
    {
        CV_LOG_VERBOSE(ਨਿਰਾ, 1, "MainThread: reduce worker pool: " << ਧਾਗੇ.size() << " => " << new_ਧਾਗੇ_ਗਿਣਤੀ);
        std::vector< Ptr<ਕਾਮਾ_Thread> > release_ਧਾਗੇ(ਧਾਗੇ.size() - new_ਧਾਗੇ_ਗਿਣਤੀ);
        ਲਈ (size_t i = new_ਧਾਗੇ_ਗਿਣਤੀ; i < ਧਾਗੇ.size(); ++i)
        {
            pthread_mutex_lock(&ਧਾਗੇ[i]->mutex);  // to avoid signal miss due pre-check
            ਧਾਗੇ[i]->stop_thread = ਸੱਚ;
            ਧਾਗੇ[i]->has_wake_signal = ਸੱਚ;
#ਜੇ !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
            pthread_mutex_unlock(&ਧਾਗੇ[i]->mutex);
            pthread_cond_broadcast/*pthread_cond_signal*/(&ਧਾਗੇ[i]->cond_thread_wake); // wake thread
#ਨਹੀਂਤਾਂ
            pthread_mutex_unlock(&ਧਾਗੇ[i]->mutex);
#ਜੇ_ਅੰਤ
            std::swap(ਧਾਗੇ[i], release_ਧਾਗੇ[i - new_ਧਾਗੇ_ਗਿਣਤੀ]);
        }
#ਜੇ defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
        CV_LOG_VERBOSE(ਨਿਰਾ, 1, "MainThread: notify worker threads about termination...");
        pthread_cond_broadcast(&cond_thread_wake); // wake all threads
#ਜੇ_ਅੰਤ
        ਧਾਗੇ.resize(new_ਧਾਗੇ_ਗਿਣਤੀ);
        release_ਧਾਗੇ.clear();  // calls thread_join which want to lock mutex
        ਵਾਪਸ ਝੂਠ;
    }
    ਨਹੀਂਤਾਂ
    {
        CV_LOG_VERBOSE(ਨਿਰਾ, 1, "MainThread: upgrade worker pool: " << ਧਾਗੇ.size() << " => " << new_ਧਾਗੇ_ਗਿਣਤੀ);
        ਲਈ (size_t i = ਧਾਗੇ.size(); i < new_ਧਾਗੇ_ਗਿਣਤੀ; ++i)
        {
            ਧਾਗੇ.push_back(Ptr<ਕਾਮਾ_Thread>(ਨਵਾਂ ਕਾਮਾ_Thread(*ਇਹ, (ਬਿਨਾਨਿਸ਼ਾਨੀ)i))); // spawn more threads
        }
    }
    ਵਾਪਸ ਝੂਠ;
}

ThreadPool::~ThreadPool()
{
    reconfigure(0);
    pthread_cond_destroy(&cond_thread_task_complete);
#ਜੇ defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
    pthread_cond_destroy(&cond_thread_wake);
#ਜੇ_ਅੰਤ
    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&mutex_notify);
}

ਖਾਲੀ ThreadPool::ਲੜੀਵਾਰ(ਅਚਲ Range& range, ਅਚਲ ParallelLoopBody& body, ਦੁੱਗਣਾ nstripes)
{
    CV_LOG_VERBOSE(ਨਿਰਾ, 1, "MainThread: new parallel job: num_threads=" << ਅੰਕ_ਧਾਗੇ_2 << "   range=" << range.size() << "   nstripes=" << nstripes << "   job=" << (ਖਾਲੀ*)ਕੰਮ);
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CV_PROFILE_ਧਾਗੇ
    ਕੰਮ_Submit_ਸਮਾਂ = get_Tick_ਗਿਣਤੀ();
    ਧਾਗੇ_stat[0].reset();
    ਧਾਗੇ_stat[0].threadWait = ਕੰਮ_Submit_ਸਮਾਂ;
    ਧਾਗੇ_stat[0].threadWake = ਕੰਮ_Submit_ਸਮਾਂ;
#ਜੇ_ਅੰਤ
    ਜੇ (get_ਅੰਕ_ਦਾ_ਧਾਗੇ() > 1 &&
        ਕੰਮ == ਨਿਰਾ &&
        (range.size() * nstripes >= 2 || (range.size() > 1 && nstripes <= 0))
    )
    {
        pthread_mutex_lock(&mutex);
        ਜੇ (ਕੰਮ != ਨਿਰਾ)
        {
            pthread_mutex_unlock(&mutex);
            body(range);
            ਵਾਪਸ;
        }
        reconfigure_(ਅੰਕ_ਧਾਗੇ_2 - 1);

        {
            CV_LOG_VERBOSE(ਨਿਰਾ, 1, "MainThread: initialize parallel job: " << range.size());
            ਕੰਮ = Ptr<Parallel_ਕੰਮ>(ਨਵਾਂ Parallel_ਕੰਮ(*ਇਹ, range, body, nstripes));
            pthread_mutex_unlock(&mutex);

            CV_LOG_VERBOSE(ਨਿਰਾ, 5, "MainThread: wake worker threads...");
            size_t ਅੰਕ_ਧਾਗੇ_ਮੰਜ਼ਿਲ_wake = std::min(static_cast<size_t>(range.size()), ਧਾਗੇ.size());
            ਲਈ (size_t i = 0; i < ਅੰਕ_ਧਾਗੇ_ਮੰਜ਼ਿਲ_wake; ++i)
            {
                ਜੇ (ਕੰਮ->current_task >= ਕੰਮ->range.size())
                    ਰੋਕੋ;
                ਕਾਮਾ_Thread& thread = *(ਧਾਗੇ[i].get());
                ਜੇ (
#ਜੇ defined(__clang__) && defined(__has_feature)
#ਜੇ __has_feature(thread_sanitizer)
                        1 ||  // Robust workaround to avoid data race warning of `thread.job`
#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ
#ਜੇ !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
                        thread.is_ਸਰਗਰਮ_2 ||
#ਜੇ_ਅੰਤ
                        thread.has_wake_signal
                        || !thread.ਕੰਮ.empty()  // #10881
                )
                {
                    pthread_mutex_lock(&thread.mutex);
                    thread.ਕੰਮ = ਕੰਮ;
#ਜੇ !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
                    ਤਾਰਕਿਕ is_ਸਰਗਰਮ_2 = thread.is_ਸਰਗਰਮ_2;
#ਜੇ_ਅੰਤ
                    thread.has_wake_signal = ਸੱਚ;
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CV_PROFILE_ਧਾਗੇ
                    ਧਾਗੇ_stat[i + 1].reset();
#ਜੇ_ਅੰਤ
                    pthread_mutex_unlock(&thread.mutex);
#ਜੇ !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
                    ਜੇ (!is_ਸਰਗਰਮ_2)
                    {
                        pthread_cond_broadcast/*pthread_cond_signal*/(&thread.cond_thread_wake); // wake thread
                    }
#ਜੇ_ਅੰਤ
                }
                ਨਹੀਂਤਾਂ
                {
                    CV_Assert(thread.ਕੰਮ.empty());
                    thread.ਕੰਮ = ਕੰਮ;
                    thread.has_wake_signal = ਸੱਚ;
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CV_PROFILE_ਧਾਗੇ
                    ਧਾਗੇ_stat[i + 1].reset();
#ਜੇ_ਅੰਤ
#ਜੇ !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
                    pthread_cond_broadcast/*pthread_cond_signal*/(&thread.cond_thread_wake); // wake thread
#ਜੇ_ਅੰਤ
                }
            }
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CV_PROFILE_ਧਾਗੇ
            ਧਾਗੇ_stat[0].threadPing = get_Tick_ਗਿਣਤੀ();
#ਜੇ_ਅੰਤ
#ਜੇ defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
            pthread_cond_broadcast(&cond_thread_wake); // wake all threads
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CV_PROFILE_ਧਾਗੇ
            ਧਾਗੇ_stat[0].threadWake = get_Tick_ਗਿਣਤੀ();
#ਜੇ_ਅੰਤ
            CV_LOG_VERBOSE(ਨਿਰਾ, 5, "MainThread: wake worker threads... (done)");

            {
                Parallel_ਕੰਮ& j = *(ਇਹ->ਕੰਮ);
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CV_PROFILE_ਧਾਗੇ
                ਧਾਗੇ_stat[0].thread_Execute_ਸ਼ੁਰੂ = get_Tick_ਗਿਣਤੀ();
                ਧਾਗੇ_stat[0].executedTasks = j.execute(ਝੂਠ);
                ਧਾਗੇ_stat[0].threadExecuteStop = get_Tick_ਗਿਣਤੀ();
#ਨਹੀਂਤਾਂ
                j.execute(ਝੂਠ);
#ਜੇ_ਅੰਤ
                CV_Assert(j.current_task >= j.range.size());
                CV_LOG_VERBOSE(ਨਿਰਾ, 5, "MainThread: complete self-tasks: " << j.ਸਰਗਰਮ_thread_ਗਿਣਤੀ << " " << j.completed_thread_ਗਿਣਤੀ);
                ਜੇ (ਕੰਮ->is_completed || j.ਸਰਗਰਮ_thread_ਗਿਣਤੀ == 0)
                {
                    ਕੰਮ->is_completed = ਸੱਚ;
                    CV_LOG_VERBOSE(ਨਿਰਾ, 5, "MainThread: no WIP worker threads");
                }
                ਨਹੀਂਤਾਂ
                {
                    ਜੇ (CV_MAIN_THREAD_ਸਰਗਰਮ_WAIT > 0)
                    {
                        ਲਈ (ਪੂਰਨ i = 0; i < CV_MAIN_THREAD_ਸਰਗਰਮ_WAIT; i++)  // don't spin too much in any case (inaccurate getTickCount())
                        {
                            ਜੇ (ਕੰਮ->is_completed)
                            {
                                CV_LOG_VERBOSE(ਨਿਰਾ, 5, "MainThread: job finalize (active wait) " << j.ਸਰਗਰਮ_thread_ਗਿਣਤੀ << " " << j.completed_thread_ਗਿਣਤੀ);
                                ਰੋਕੋ;
                            }
                            ਜੇ (CV_ਸਰਗਰਮ_WAIT_PAUSE_ਹੱਦਬੰਦੀ > 0 && (i < CV_ਸਰਗਰਮ_WAIT_PAUSE_ਹੱਦਬੰਦੀ || (i & 1)))
                                CV_PAUSE(16);
                            ਨਹੀਂਤਾਂ
                                CV_YIELD();
                        }
                    }
                    ਜੇ (!ਕੰਮ->is_completed)
                    {
                        CV_LOG_VERBOSE(ਨਿਰਾ, 5, "MainThread: prepare wait " << j.ਸਰਗਰਮ_thread_ਗਿਣਤੀ << " " << j.completed_thread_ਗਿਣਤੀ);
                        pthread_mutex_lock(&mutex_notify);
                        ਲਈ (;;)
                        {
                            ਜੇ (ਕੰਮ->is_completed)
                            {
                                CV_LOG_VERBOSE(ਨਿਰਾ, 5, "MainThread: job finalize (wait) " << j.ਸਰਗਰਮ_thread_ਗਿਣਤੀ << " " << j.completed_thread_ਗਿਣਤੀ);
                                ਰੋਕੋ;
                            }
                            CV_LOG_VERBOSE(ਨਿਰਾ, 5, "MainThread: wait completion (sleep) ...");
                            pthread_cond_wait(&cond_thread_task_complete, &mutex_notify);
                            CV_LOG_VERBOSE(ਨਿਰਾ, 5, "MainThread: wake");
                        }
                        pthread_mutex_unlock(&mutex_notify);
                    }
                }
            }
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CV_PROFILE_ਧਾਗੇ
            ਧਾਗੇ_stat[0].threadFree = get_Tick_ਗਿਣਤੀ();
            std::ਬਾਹਰ << "Job: sz=" << range.size() << " nstripes=" << nstripes << "    Time: " << (ਧਾਗੇ_stat[0].threadFree - ਕੰਮ_Submit_ਸਮਾਂ) / tickFreq * 1e6 << " usec" << std::ਸਤਰ_ਅੰਤ;
            ਲਈ (ਪੂਰਨ i = 0; i < (ਪੂਰਨ)ਧਾਗੇ.size() + 1; i++)
            {
                ਧਾਗੇ_stat[i].dump(i - 1, ਕੰਮ_Submit_ਸਮਾਂ, tickFreq);
            }
#ਜੇ_ਅੰਤ
            ਜੇ (ਕੰਮ)
            {
                pthread_mutex_lock(&mutex);
                CV_LOG_VERBOSE(ਨਿਰਾ, 5, "MainThread: job release");
                CV_Assert(ਕੰਮ->is_completed);
                ਕੰਮ.release();
                pthread_mutex_unlock(&mutex);
            }
        }
    }
    ਨਹੀਂਤਾਂ
    {
        body(range);
    }
}

size_t ThreadPool::get_ਅੰਕ_ਦਾ_ਧਾਗੇ()
{
    ਵਾਪਸ ਅੰਕ_ਧਾਗੇ_2;
}

ਖਾਲੀ ThreadPool::ਸੈੱਟ_ਅੰਕ_ਦਾ_ਧਾਗੇ(ਬਿਨਾਨਿਸ਼ਾਨੀ n)
{
    ਜੇ (n != ਅੰਕ_ਧਾਗੇ_2)
    {
        ਅੰਕ_ਧਾਗੇ_2 = n;
        ਜੇ (n == 1)
           ਜੇ (ਕੰਮ == ਨਿਰਾ) reconfigure(0);  // stop worker threads immediately
    }
}

size_t parallel_pthreads_get_ਧਾਗੇ_ਅੰਕ()
{
    ਵਾਪਸ ThreadPool::instance().get_ਅੰਕ_ਦਾ_ਧਾਗੇ();
}

ਖਾਲੀ parallel_pthreads_ਸੈੱਟ_ਧਾਗੇ_ਅੰਕ(ਪੂਰਨ num)
{
    ਜੇ(num < 0)
    {
        ThreadPool::instance().ਸੈੱਟ_ਅੰਕ_ਦਾ_ਧਾਗੇ(0);
    }
    ਨਹੀਂਤਾਂ
    {
        ThreadPool::instance().ਸੈੱਟ_ਅੰਕ_ਦਾ_ਧਾਗੇ(ਬਿਨਾਨਿਸ਼ਾਨੀ(num));
    }
}

ਖਾਲੀ parallel_for_pthreads(ਅਚਲ Range& range, ਅਚਲ ParallelLoopBody& body, ਦੁੱਗਣਾ nstripes)
{
    ThreadPool::instance().ਲੜੀਵਾਰ(range, body, nstripes);
}

}

#ਜੇ_ਅੰਤ
