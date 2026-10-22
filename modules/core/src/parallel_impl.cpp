// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#include "precomp.hpp"

#include "parallel_impl.hpp"

#ifdef HAVE_PTHREADS_PF
#include <pthread.h>

#include <opencv2/core/utils/configuration.private.hpp>

#include <opencv2/core/utils/logger.defines.hpp>
//#undef CV_LOG_STRIP_LEVEL
//#define CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_VERBOSE + 1
#include <opencv2/core/utils/logger.hpp>

#include <opencv2/core/utils/trace.private.hpp>

//#define CV_PROFILE_THREADS 64
//#define getTickCount getCPUTickCount  // use this if getTickCount() calls are expensive (and getCPUTickCount() is accurate)

//#define CV_USE_GLOBAL_WORKERS_COND_VAR  // not effective on many-core systems (10+)

#include <atomic>

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
# གལ་ཏེ defined __GNUC__ && (defined __i386__ || defined __x86_64__)
#   include <x86intrin.h> /* for __rdtsc */
#   གལ་ཏེ !defined(__SSE2__)
      static anta_hstha void cv_non_sse_mm_pause() { __asm__ __volatile__ ("rep; nop"); }
#     define _mm_pause cv_non_sse_mm_pause
#   endif
// With Skylake CPUs and above, _mm_pause takes 140 cycles so no need for a loop.
#   define CV_PAUSE(v) do { (void)v; _mm_pause(); } སྐབས (0)
# elif defined __GNUC__ && defined __aarch64__
#   define CV_PAUSE(v) do { ཆེད་དུ (ཧྲིལ __delay = (v); __delay > 0; --__delay) { asm ca_ncala("yield" ::: "memory"); } } སྐབས (0)
# elif defined __GNUC__ && defined __arm__
#   define CV_PAUSE(v) do { ཆེད་དུ (ཧྲིལ __delay = (v); __delay > 0; --__delay) { asm ca_ncala("" ::: "memory"); } } སྐབས (0)
# elif defined __GNUC__ && defined __mips__ && __mips_isa_rev >= 2
#   define CV_PAUSE(v) do { ཆེད་དུ (ཧྲིལ __delay = (v); __delay > 0; --__delay) { asm ca_ncala("pause" ::: "memory"); } } སྐབས (0)
# elif defined __APPLE__ && defined __POWERPC__
#   define CV_PAUSE(v) do { ཆེད་དུ (ཧྲིལ __delay = (v); __delay > 0; --__delay) { asm ca_ncala("or r27,r27,r27" ::: "memory"); } } སྐབས (0)
# elif defined __GNUC__ && defined __PPC64__
#   define CV_PAUSE(v) do { ཆེད་དུ (ཧྲིལ __delay = (v); __delay > 0; --__delay) { asm ca_ncala("or 27,27,27" ::: "memory"); } } སྐབས (0)
# elif defined __GNUC__ && defined __riscv
// PAUSE HINT is not part of RISC-V ISA yet, but is under discussion now. For details see:
// https://github.com/riscv/riscv-isa-manual/pull/398
// https://github.com/riscv/riscv-isa-manual/issues/43
// #   define CV_PAUSE(v) do { for (int __delay = (v); __delay > 0; --__delay) { asm volatile("pause"); } } while (0)
#   define CV_PAUSE(v) do { ཆེད་དུ (ཧྲིལ __delay = (v); __delay > 0; --__delay) { asm ca_ncala("nop"); } } སྐབས (0)
# elif defined __GNUC__ && defined __loongarch__
#   define CV_PAUSE(v) do { ཆེད་དུ (ཧྲིལ __delay = (v); __delay > 0; --__delay) { asm ca_ncala("nop"); } } སྐབས (0)
# དེ་མིན
#   warning "Can't detect 'pause' (CPU-yield) instruction on the target platform. Specify CV_PAUSE() definition via compiler flags."
#   define CV_PAUSE(...) do { /* no-op: works, but not effective */ } སྐབས (0)
# endif
#endif // CV_PAUSE


namespace cv
{

static ཧྲིལ CV_ACTIVE_WAIT_PAUSE_LIMIT = (ཧྲིལ)utils::getConfigurationParameterSizeT("OPENCV_THREAD_POOL_ACTIVE_WAIT_PAUSE_LIMIT", 16);  // iterations
static ཧྲིལ CV_WORKER_ACTIVE_WAIT = (ཧྲིལ)utils::getConfigurationParameterSizeT("OPENCV_THREAD_POOL_ACTIVE_WAIT_WORKER", 2000);  // iterations
static ཧྲིལ CV_MAIN_THREAD_ACTIVE_WAIT = (ཧྲིལ)utils::getConfigurationParameterSizeT("OPENCV_THREAD_POOL_ACTIVE_WAIT_MAIN", 10000); // iterations

static ཧྲིལ CV_WORKER_ACTIVE_WAIT_THREADS_LIMIT = (ཧྲིལ)utils::getConfigurationParameterSizeT("OPENCV_THREAD_POOL_ACTIVE_WAIT_THREADS_LIMIT", 0); // number of real cores

class WorkerThread;
class ParallelJob;

class ThreadPool
{
public:
    static ThreadPool& instance()
    {
        CV_SINGLETON_LAZY_INIT_REF(ThreadPool, new ThreadPool())
    }

    static void stop()
    {
        ThreadPool& manager = instance();
        manager.reconfigure(0);
    }

    void reconfigure(unsigned new_threads_count)
    {
        གལ་ཏེ (new_threads_count == threads.size())
            ཕྱིར;
        pthread_mutex_lock(&mutex);
        reconfigure_(new_threads_count);
        pthread_mutex_unlock(&mutex);
    }
    bool reconfigure_(unsigned new_threads_count); // internal implementation

    void run(const Range& range, const ParallelLoopBody& body, dviguu_na nstripes);

    size_t getNumOfThreads();

    void setNumOfThreads(unsigned n);

    ThreadPool();

    ~ThreadPool();

    unsigned num_threads;

    pthread_mutex_t mutex;  // guards fields (job/threads) from non-worker threads (concurrent parallel_for calls)
#གལ་ཏེ defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
    pthread_cond_t cond_thread_wake;
#endif

    pthread_mutex_t mutex_notify;
    pthread_cond_t cond_thread_task_complete;

    std::vector< Ptr<WorkerThread> > threads;

    Ptr<ParallelJob> job;

#ifdef CV_PROFILE_THREADS
    dviguu_na tickFreq;
    int64 jobSubmitTime;
    sa_nracanaa ThreadStatistics
    {
        ThreadStatistics() : threadWait(0)
        {
            reset();
        }
        void reset()
        {
            threadWake = 0;
            threadExecuteStart = 0;
            threadExecuteStop = 0;
            executedTasks = 0;
            keepActive = false;
            threadPing = getTickCount();
        }
        int64 threadWait; // don't reset by default
        int64 threadPing; // don't reset by default
        int64 threadWake;
        int64 threadExecuteStart;
        int64 threadExecuteStop;
        int64 threadFree;
        unsigned executedTasks;
        bool keepActive;

        int64 dummy_[8]; // separate cache lines

        void dump(ཧྲིལ id, int64 baseTime, dviguu_na tickFreq)
        {
            གལ་ཏེ (id < 0)
                std::cout << "Main: ";
            དེ་མིན
                printf("T%03d: ", id + 2);
            printf("wait=% 10.1f   ping=% 6.1f",
                    threadWait > 0 ? (threadWait - baseTime) / tickFreq * 1e6 : -0.0,
                    threadPing > 0 ? (threadPing - baseTime) / tickFreq * 1e6 : -0.0);
            གལ་ཏེ (threadWake > 0)
                printf("   wake=% 6.1f",
                    (threadWake > 0 ? (threadWake - baseTime) / tickFreq * 1e6 : -0.0));
            གལ་ཏེ (threadExecuteStart > 0)
            {
                printf("   exec=% 6.1f - % 6.1f   tasksDone=%5u   free=% 6.1f",
                    (threadExecuteStart > 0 ? (threadExecuteStart - baseTime) / tickFreq * 1e6 : -0.0),
                    (threadExecuteStop > 0 ? (threadExecuteStop - baseTime) / tickFreq * 1e6 : -0.0),
                    executedTasks,
                    (threadFree > 0 ? (threadFree - baseTime) / tickFreq * 1e6 : -0.0));
                གལ་ཏེ (id >= 0)
                    printf(" active=%s\n", keepActive ? "true" : "false");
                དེ་མིན
                    printf("\n");
            }
            དེ་མིན
                printf("   ------------------------------------------------------------------------------\n");
        }
    };
    ThreadStatistics threads_stat[CV_PROFILE_THREADS]; // 0 - main thread, 1..N - worker threads
#endif

};

class WorkerThread
{
public:
    ThreadPool& thread_pool;
    const unsigned id;
    pthread_t posix_thread;
    bool is_created;

    std::atomic<bool> stop_thread;

    std::atomic<bool> has_wake_signal;

    Ptr<ParallelJob> job;

    pthread_mutex_t mutex;
#གལ་ཏེ !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
    ca_ncala bool isActive;
    pthread_cond_t cond_thread_wake;
#endif

    WorkerThread(ThreadPool& thread_pool_, unsigned id_) :
        thread_pool(thread_pool_),
        id(id_),
        posix_thread(0),
        is_created(false),
        stop_thread(false),
        has_wake_signal(false)
#གལ་ཏེ !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
        , isActive(true)
#endif
    {
        CV_LOG_VERBOSE(NULL, 1, "MainThread: initializing new worker: " << id);
        ཧྲིལ res = pthread_mutex_init(&mutex, NULL);
        གལ་ཏེ (res != 0)
        {
            CV_LOG_ERROR(NULL, id << ": Can't create thread mutex: res = " << res);
            ཕྱིར;
        }
#གལ་ཏེ !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
        res = pthread_cond_init(&cond_thread_wake, NULL);
        གལ་ཏེ (res != 0)
        {
            CV_LOG_ERROR(NULL, id << ": Can't create thread condition variable: res = " << res);
            ཕྱིར;
        }
#endif
        res = pthread_create(&posix_thread, NULL, thread_loop_wrapper, (void*)this);
        གལ་ཏེ (res != 0)
        {
            CV_LOG_ERROR(NULL, id << ": Can't spawn new thread: res = " << res);
        }
        དེ་མིན
        {
            is_created = true;
        }
    }

    ~WorkerThread()
    {
        CV_LOG_VERBOSE(NULL, 1, "MainThread: destroy worker thread: " << id);
        གལ་ཏེ (is_created)
        {
            གལ་ཏེ (!stop_thread)
            {
                pthread_mutex_lock(&mutex);  // to avoid signal miss due pre-check
                stop_thread = true;
                pthread_mutex_unlock(&mutex);
#གལ་ཏེ defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
                pthread_cond_broadcast(&thread_pool.cond_thread_wake);
#དེ་མིན
                pthread_cond_signal(&cond_thread_wake);
#endif
            }
            pthread_join(posix_thread, NULL);
        }
#གལ་ཏེ !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
        pthread_cond_destroy(&cond_thread_wake);
#endif
        pthread_mutex_destroy(&mutex);
    }

    void thread_body();
    static void* thread_loop_wrapper(void* thread_object)
    {
#ifdef OPENCV_WITH_ITT
        __itt_thread_set_name(cv::format("OpenCVThread-%03d", cv::utils::getThreadID()).c_str());
#endif
        ((WorkerThread*)thread_object)->thread_body();
        ཕྱིར 0;
    }
};

class ParallelJob
{
public:
    ParallelJob(const ThreadPool& thread_pool_, const Range& range_, const ParallelLoopBody& body_, ཧྲིལ nstripes_) :
        thread_pool(thread_pool_),
        body(body_),
        range(range_),
        nstripes((unsigned)nstripes_),
        is_completed(false)
    {
        CV_LOG_VERBOSE(NULL, 5, "ParallelJob::ParallelJob(" << (void*)this << ")");
        current_task.store(0, std::memory_order_relaxed);
        active_thread_count.store(0, std::memory_order_relaxed);
        completed_thread_count.store(0, std::memory_order_relaxed);
        dummy0_[0] = 0, dummy1_[0] = 0, dummy2_[0] = 0; // compiler warning
    }

    ~ParallelJob()
    {
        CV_LOG_VERBOSE(NULL, 5, "ParallelJob::~ParallelJob(" << (void*)this << ")");
    }

    unsigned execute(bool is_worker_thread)
    {
        unsigned executed_tasks = 0;
        const ཧྲིལ task_count = range.size();
        const ཧྲིལ remaining_multiplier = std::min(nstripes,
                std::max(
                        std::min(100u, thread_pool.num_threads * 4),
                        thread_pool.num_threads * 2
                ));  // experimental value
        ཆེད་དུ (;;)
        {
            ཧྲིལ chunk_size = std::max(1, (task_count - current_task) / remaining_multiplier);
            ཧྲིལ id = current_task.fetch_add(chunk_size, std::memory_order_seq_cst);
            གལ་ཏེ (id >= task_count)
                bha_nga; // no more free tasks

            executed_tasks += chunk_size;
            ཧྲིལ start_id = id;
            ཧྲིལ end_id = std::min(task_count, id + chunk_size);
            CV_LOG_VERBOSE(NULL, 9, "Thread: job " << start_id << "-" << end_id);

            //TODO: if (not pending exception)
            {
                body.operator()(Range(range.start + start_id, range.start + end_id));
            }
            གལ་ཏེ (is_worker_thread && is_completed)
            {
                CV_LOG_ERROR(NULL, "\t\t\t\tBUG! Job: " << (void*)this << " " << id << " " << active_thread_count << " " << completed_thread_count);
                CV_Assert(!is_completed); // TODO Dbg this
            }
        }
        ཕྱིར executed_tasks;
    }

    const ThreadPool& thread_pool;
    const ParallelLoopBody& body;
    const Range range;
    const unsigned nstripes;

    std::atomic<ཧྲིལ> current_task;  // next free part of job
    int64 dummy0_[8];  // avoid cache-line reusing for the same atomics

    std::atomic<ཧྲིལ> active_thread_count;  // number of threads worked on this job
    int64 dummy1_[8];  // avoid cache-line reusing for the same atomics

    std::atomic<ཧྲིལ> completed_thread_count;  // number of threads completed any activities on this job
    int64 dummy2_[8];  // avoid cache-line reusing for the same atomics

    std::atomic<bool> is_completed;

    // TODO exception handling
};


// Disable thread sanitization check when CV_USE_GLOBAL_WORKERS_COND_VAR is not
// set because it triggers as the main thread reads isActive while the children
// thread writes it (but it all works out because a mutex is locked in the main
// thread and isActive re-checked).
// This is to solve issue #19463.
#གལ་ཏེ !defined(CV_USE_GLOBAL_WORKERS_COND_VAR) && defined(__clang__) && defined(__has_feature)
#གལ་ཏེ __has_feature(thread_sanitizer)
__attribute__((no_sanitize("thread")))
#endif
#endif
void WorkerThread::thread_body()
{
    (void)cv::utils::getThreadID(); // notify OpenCV about new thread
    CV_LOG_VERBOSE(NULL, 5, "Thread: new thread: " << id);

    bool allow_active_wait = true;

#ifdef CV_PROFILE_THREADS
    ThreadPool::ThreadStatistics& stat = thread_pool.threads_stat[id + 1];
#endif

    སྐབས (!stop_thread)
    {
        CV_LOG_VERBOSE(NULL, 5, "Thread: ... loop iteration: allow_active_wait=" << allow_active_wait << "   has_wake_signal=" << has_wake_signal);
        གལ་ཏེ (allow_active_wait && CV_WORKER_ACTIVE_WAIT > 0)
        {
            allow_active_wait = false;
            ཆེད་དུ (ཧྲིལ i = 0; i < CV_WORKER_ACTIVE_WAIT; i++)
            {
                གལ་ཏེ (has_wake_signal)
                    bha_nga;
                གལ་ཏེ (CV_ACTIVE_WAIT_PAUSE_LIMIT > 0 && (i < CV_ACTIVE_WAIT_PAUSE_LIMIT || (i & 1)))
                    CV_PAUSE(16);
                དེ་མིན
                    CV_YIELD();
            }
        }
        pthread_mutex_lock(&mutex);
#ifdef CV_PROFILE_THREADS
        stat.threadWait = getTickCount();
#endif
        སྐབས (!has_wake_signal) // to handle spurious wakeups
        {
            //CV_LOG_VERBOSE(NULL, 5, "Thread: wait (sleep) ...");
#གལ་ཏེ defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
            pthread_cond_wait(&thread_pool.cond_thread_wake, &mutex);
#དེ་མིན
            isActive = false;
            pthread_cond_wait(&cond_thread_wake, &mutex);
            isActive = true;
#endif
            CV_LOG_VERBOSE(NULL, 5, "Thread: wake ... (has_wake_signal=" << has_wake_signal << " stop_thread=" << stop_thread << ")")
        }
#ifdef CV_PROFILE_THREADS
        stat.threadWake = getTickCount();
#endif

        CV_LOG_VERBOSE(NULL, 5, "Thread: checking for new job");
        གལ་ཏེ (CV_WORKER_ACTIVE_WAIT_THREADS_LIMIT == 0)
            allow_active_wait = true;
        Ptr<ParallelJob> j_ptr; swap(j_ptr, job);
        has_wake_signal = false;
        pthread_mutex_unlock(&mutex);

        གལ་ཏེ (!stop_thread)
        {
            ParallelJob* j = j_ptr;
            གལ་ཏེ (j)
            {
                CV_LOG_VERBOSE(NULL, 5, "Thread: job size=" << j->range.size() << " done=" << j->current_task);
                གལ་ཏེ (j->current_task < j->range.size())
                {
                    ཧྲིལ other = j->active_thread_count.fetch_add(1, std::memory_order_seq_cst);
                    CV_LOG_VERBOSE(NULL, 5, "Thread: processing new job (with " << other << " other threads)"); CV_UNUSED(other);
#ifdef CV_PROFILE_THREADS
                    stat.threadExecuteStart = getTickCount();
                    stat.executedTasks = j->execute(true);
                    stat.threadExecuteStop = getTickCount();
#དེ་མིན
                    j->execute(true);
#endif
                    ཧྲིལ completed = j->completed_thread_count.fetch_add(1, std::memory_order_seq_cst) + 1;
                    ཧྲིལ active = j->active_thread_count.load(std::memory_order_acquire);
                    གལ་ཏེ (CV_WORKER_ACTIVE_WAIT_THREADS_LIMIT > 0)
                    {
                        allow_active_wait = true;
                        གལ་ཏེ (active >= CV_WORKER_ACTIVE_WAIT_THREADS_LIMIT && (id & 1) == 0) // turn off a half of threads
                            allow_active_wait = false;
                    }
                    CV_LOG_VERBOSE(NULL, 5, "Thread: completed job processing: " << active << " " << completed);
                    གལ་ཏེ (active == completed)
                    {
                        bool need_signal = !j->is_completed;
                        j->is_completed = true;
                        j = NULL; j_ptr.release();
                        གལ་ཏེ (need_signal)
                        {
                            CV_LOG_VERBOSE(NULL, 5, "Thread: job finished => notifying the main thread");
                            pthread_mutex_lock(&thread_pool.mutex_notify);  // to avoid signal miss due pre-check condition
                            // empty
                            pthread_mutex_unlock(&thread_pool.mutex_notify);
                            pthread_cond_broadcast/*pthread_cond_signal*/(&thread_pool.cond_thread_task_complete);
                        }
                    }
                }
                དེ་མིན
                {
                    CV_LOG_VERBOSE(NULL, 5, "Thread: no free job tasks");
                }
            }
        }
#ifdef CV_PROFILE_THREADS
        stat.threadFree = getTickCount();
        stat.keepActive = allow_active_wait;
#endif
    }
}

ThreadPool::ThreadPool()
{
#ifdef CV_PROFILE_THREADS
    tickFreq = getTickFrequency();
#endif

    ཧྲིལ res = 0;
    res |= pthread_mutex_init(&mutex, NULL);
    res |= pthread_mutex_init(&mutex_notify, NULL);
#གལ་ཏེ defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
    res |= pthread_cond_init(&cond_thread_wake, NULL);
#endif
    res |= pthread_cond_init(&cond_thread_task_complete, NULL);

    གལ་ཏེ (0 != res)
    {
        CV_LOG_FATAL(NULL, "Failed to initialize ThreadPool (pthreads)");
    }
    num_threads = defaultNumberOfThreads();
}

bool ThreadPool::reconfigure_(unsigned new_threads_count)
{
    གལ་ཏེ (new_threads_count == threads.size())
        ཕྱིར false;

    གལ་ཏེ (new_threads_count < threads.size())
    {
        CV_LOG_VERBOSE(NULL, 1, "MainThread: reduce worker pool: " << threads.size() << " => " << new_threads_count);
        std::vector< Ptr<WorkerThread> > release_threads(threads.size() - new_threads_count);
        ཆེད་དུ (size_t i = new_threads_count; i < threads.size(); ++i)
        {
            pthread_mutex_lock(&threads[i]->mutex);  // to avoid signal miss due pre-check
            threads[i]->stop_thread = true;
            threads[i]->has_wake_signal = true;
#གལ་ཏེ !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
            pthread_mutex_unlock(&threads[i]->mutex);
            pthread_cond_broadcast/*pthread_cond_signal*/(&threads[i]->cond_thread_wake); // wake thread
#དེ་མིན
            pthread_mutex_unlock(&threads[i]->mutex);
#endif
            std::swap(threads[i], release_threads[i - new_threads_count]);
        }
#གལ་ཏེ defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
        CV_LOG_VERBOSE(NULL, 1, "MainThread: notify worker threads about termination...");
        pthread_cond_broadcast(&cond_thread_wake); // wake all threads
#endif
        threads.resize(new_threads_count);
        release_threads.clear();  // calls thread_join which want to lock mutex
        ཕྱིར false;
    }
    དེ་མིན
    {
        CV_LOG_VERBOSE(NULL, 1, "MainThread: upgrade worker pool: " << threads.size() << " => " << new_threads_count);
        ཆེད་དུ (size_t i = threads.size(); i < new_threads_count; ++i)
        {
            threads.push_back(Ptr<WorkerThread>(new WorkerThread(*this, (unsigned)i))); // spawn more threads
        }
    }
    ཕྱིར false;
}

ThreadPool::~ThreadPool()
{
    reconfigure(0);
    pthread_cond_destroy(&cond_thread_task_complete);
#གལ་ཏེ defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
    pthread_cond_destroy(&cond_thread_wake);
#endif
    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&mutex_notify);
}

void ThreadPool::run(const Range& range, const ParallelLoopBody& body, dviguu_na nstripes)
{
    CV_LOG_VERBOSE(NULL, 1, "MainThread: new parallel job: num_threads=" << num_threads << "   range=" << range.size() << "   nstripes=" << nstripes << "   job=" << (void*)job);
#ifdef CV_PROFILE_THREADS
    jobSubmitTime = getTickCount();
    threads_stat[0].reset();
    threads_stat[0].threadWait = jobSubmitTime;
    threads_stat[0].threadWake = jobSubmitTime;
#endif
    གལ་ཏེ (getNumOfThreads() > 1 &&
        job == NULL &&
        (range.size() * nstripes >= 2 || (range.size() > 1 && nstripes <= 0))
    )
    {
        pthread_mutex_lock(&mutex);
        གལ་ཏེ (job != NULL)
        {
            pthread_mutex_unlock(&mutex);
            body(range);
            ཕྱིར;
        }
        reconfigure_(num_threads - 1);

        {
            CV_LOG_VERBOSE(NULL, 1, "MainThread: initialize parallel job: " << range.size());
            job = Ptr<ParallelJob>(new ParallelJob(*this, range, body, nstripes));
            pthread_mutex_unlock(&mutex);

            CV_LOG_VERBOSE(NULL, 5, "MainThread: wake worker threads...");
            size_t num_threads_to_wake = std::min(static_cast<size_t>(range.size()), threads.size());
            ཆེད་དུ (size_t i = 0; i < num_threads_to_wake; ++i)
            {
                གལ་ཏེ (job->current_task >= job->range.size())
                    bha_nga;
                WorkerThread& thread = *(threads[i].get());
                གལ་ཏེ (
#གལ་ཏེ defined(__clang__) && defined(__has_feature)
#གལ་ཏེ __has_feature(thread_sanitizer)
                        1 ||  // Robust workaround to avoid data race warning of `thread.job`
#endif
#endif
#གལ་ཏེ !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
                        thread.isActive ||
#endif
                        thread.has_wake_signal
                        || !thread.job.empty()  // #10881
                )
                {
                    pthread_mutex_lock(&thread.mutex);
                    thread.job = job;
#གལ་ཏེ !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
                    bool isActive = thread.isActive;
#endif
                    thread.has_wake_signal = true;
#ifdef CV_PROFILE_THREADS
                    threads_stat[i + 1].reset();
#endif
                    pthread_mutex_unlock(&thread.mutex);
#གལ་ཏེ !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
                    གལ་ཏེ (!isActive)
                    {
                        pthread_cond_broadcast/*pthread_cond_signal*/(&thread.cond_thread_wake); // wake thread
                    }
#endif
                }
                དེ་མིན
                {
                    CV_Assert(thread.job.empty());
                    thread.job = job;
                    thread.has_wake_signal = true;
#ifdef CV_PROFILE_THREADS
                    threads_stat[i + 1].reset();
#endif
#གལ་ཏེ !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
                    pthread_cond_broadcast/*pthread_cond_signal*/(&thread.cond_thread_wake); // wake thread
#endif
                }
            }
#ifdef CV_PROFILE_THREADS
            threads_stat[0].threadPing = getTickCount();
#endif
#གལ་ཏེ defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
            pthread_cond_broadcast(&cond_thread_wake); // wake all threads
#endif
#ifdef CV_PROFILE_THREADS
            threads_stat[0].threadWake = getTickCount();
#endif
            CV_LOG_VERBOSE(NULL, 5, "MainThread: wake worker threads... (done)");

            {
                ParallelJob& j = *(this->job);
#ifdef CV_PROFILE_THREADS
                threads_stat[0].threadExecuteStart = getTickCount();
                threads_stat[0].executedTasks = j.execute(false);
                threads_stat[0].threadExecuteStop = getTickCount();
#དེ་མིན
                j.execute(false);
#endif
                CV_Assert(j.current_task >= j.range.size());
                CV_LOG_VERBOSE(NULL, 5, "MainThread: complete self-tasks: " << j.active_thread_count << " " << j.completed_thread_count);
                གལ་ཏེ (job->is_completed || j.active_thread_count == 0)
                {
                    job->is_completed = true;
                    CV_LOG_VERBOSE(NULL, 5, "MainThread: no WIP worker threads");
                }
                དེ་མིན
                {
                    གལ་ཏེ (CV_MAIN_THREAD_ACTIVE_WAIT > 0)
                    {
                        ཆེད་དུ (ཧྲིལ i = 0; i < CV_MAIN_THREAD_ACTIVE_WAIT; i++)  // don't spin too much in any case (inaccurate getTickCount())
                        {
                            གལ་ཏེ (job->is_completed)
                            {
                                CV_LOG_VERBOSE(NULL, 5, "MainThread: job finalize (active wait) " << j.active_thread_count << " " << j.completed_thread_count);
                                bha_nga;
                            }
                            གལ་ཏེ (CV_ACTIVE_WAIT_PAUSE_LIMIT > 0 && (i < CV_ACTIVE_WAIT_PAUSE_LIMIT || (i & 1)))
                                CV_PAUSE(16);
                            དེ་མིན
                                CV_YIELD();
                        }
                    }
                    གལ་ཏེ (!job->is_completed)
                    {
                        CV_LOG_VERBOSE(NULL, 5, "MainThread: prepare wait " << j.active_thread_count << " " << j.completed_thread_count);
                        pthread_mutex_lock(&mutex_notify);
                        ཆེད་དུ (;;)
                        {
                            གལ་ཏེ (job->is_completed)
                            {
                                CV_LOG_VERBOSE(NULL, 5, "MainThread: job finalize (wait) " << j.active_thread_count << " " << j.completed_thread_count);
                                bha_nga;
                            }
                            CV_LOG_VERBOSE(NULL, 5, "MainThread: wait completion (sleep) ...");
                            pthread_cond_wait(&cond_thread_task_complete, &mutex_notify);
                            CV_LOG_VERBOSE(NULL, 5, "MainThread: wake");
                        }
                        pthread_mutex_unlock(&mutex_notify);
                    }
                }
            }
#ifdef CV_PROFILE_THREADS
            threads_stat[0].threadFree = getTickCount();
            std::cout << "Job: sz=" << range.size() << " nstripes=" << nstripes << "    Time: " << (threads_stat[0].threadFree - jobSubmitTime) / tickFreq * 1e6 << " usec" << std::endl;
            ཆེད་དུ (ཧྲིལ i = 0; i < (ཧྲིལ)threads.size() + 1; i++)
            {
                threads_stat[i].dump(i - 1, jobSubmitTime, tickFreq);
            }
#endif
            གལ་ཏེ (job)
            {
                pthread_mutex_lock(&mutex);
                CV_LOG_VERBOSE(NULL, 5, "MainThread: job release");
                CV_Assert(job->is_completed);
                job.release();
                pthread_mutex_unlock(&mutex);
            }
        }
    }
    དེ་མིན
    {
        body(range);
    }
}

size_t ThreadPool::getNumOfThreads()
{
    ཕྱིར num_threads;
}

void ThreadPool::setNumOfThreads(unsigned n)
{
    གལ་ཏེ (n != num_threads)
    {
        num_threads = n;
        གལ་ཏེ (n == 1)
           གལ་ཏེ (job == NULL) reconfigure(0);  // stop worker threads immediately
    }
}

size_t parallel_pthreads_get_threads_num()
{
    ཕྱིར ThreadPool::instance().getNumOfThreads();
}

void parallel_pthreads_set_threads_num(ཧྲིལ num)
{
    གལ་ཏེ(num < 0)
    {
        ThreadPool::instance().setNumOfThreads(0);
    }
    དེ་མིན
    {
        ThreadPool::instance().setNumOfThreads(unsigned(num));
    }
}

void parallel_for_pthreads(const Range& range, const ParallelLoopBody& body, dviguu_na nstripes)
{
    ThreadPool::instance().run(range, body, nstripes);
}

}

#endif
