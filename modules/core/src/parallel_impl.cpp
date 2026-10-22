// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#समावेश "precomp.hpp"

#समावेश "parallel_impl.hpp"

#अगर_घोषित HAVE_PTHREADS_PF
#समावेश <pthread.h>

#समावेश <opencv2/core/utils/configuration.private.hpp>

#समावेश <opencv2/core/utils/logger.defines.hpp>
//#undef CV_LOG_STRIP_LEVEL
//#define CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_VERBOSE + 1
#समावेश <opencv2/core/utils/logger.hpp>

#समावेश <opencv2/core/utils/trace.private.hpp>

//#define CV_PROFILE_THREADS 64
//#define getTickCount getCPUTickCount  // use this if getTickCount() calls are expensive (and getCPUTickCount() is accurate)

//#define CV_USE_GLOBAL_WORKERS_COND_VAR  // not effective on many-core systems (10+)

#समावेश <atomic>

// Spin lock's OS-level yield
#अगर_घोषित DECLARE_CV_YIELD
DECLARE_CV_YIELD
#पूर्ण_अगर
#अगर_अघोषित CV_YIELD
# include <thread>
# define CV_YIELD() std::this_thread::yield()
#पूर्ण_अगर // CV_YIELD

// Spin lock's CPU-level yield (required for Hyper-Threading)
#अगर_घोषित DECLARE_CV_PAUSE
DECLARE_CV_PAUSE
#पूर्ण_अगर
#अगर_अघोषित CV_PAUSE
# अगर defined __GNUC__ && (defined __i386__ || defined __x86_64__)
#   include <x86intrin.h> /* for __rdtsc */
#   अगर !defined(__SSE2__)
      अटल अंतरभूत व्योम cv_non_sse_mm_pause() { __asm__ __volatile__ ("rep; nop"); }
#     define _mm_pause cv_non_sse_mm_pause
#   endif
// With Skylake CPUs and above, _mm_pause takes 140 cycles so no need for a loop.
#   define CV_PAUSE(v) करो { (व्योम)v; _mm_pause(); } जबतक (0)
# elif defined __GNUC__ && defined __aarch64__
#   define CV_PAUSE(v) करो { क्रम (पूर्णांक __delay = (v); __delay > 0; --__delay) { यंत्र अस्थिर("yield" ::: "memory"); } } जबतक (0)
# elif defined __GNUC__ && defined __arm__
#   define CV_PAUSE(v) करो { क्रम (पूर्णांक __delay = (v); __delay > 0; --__delay) { यंत्र अस्थिर("" ::: "memory"); } } जबतक (0)
# elif defined __GNUC__ && defined __mips__ && __mips_isa_rev >= 2
#   define CV_PAUSE(v) करो { क्रम (पूर्णांक __delay = (v); __delay > 0; --__delay) { यंत्र अस्थिर("pause" ::: "memory"); } } जबतक (0)
# elif defined __APPLE__ && defined __POWERPC__
#   define CV_PAUSE(v) करो { क्रम (पूर्णांक __delay = (v); __delay > 0; --__delay) { यंत्र अस्थिर("or r27,r27,r27" ::: "memory"); } } जबतक (0)
# elif defined __GNUC__ && defined __PPC64__
#   define CV_PAUSE(v) करो { क्रम (पूर्णांक __delay = (v); __delay > 0; --__delay) { यंत्र अस्थिर("or 27,27,27" ::: "memory"); } } जबतक (0)
# elif defined __GNUC__ && defined __riscv
// PAUSE HINT is not part of RISC-V ISA yet, but is under discussion now. For details see:
// https://github.com/riscv/riscv-isa-manual/pull/398
// https://github.com/riscv/riscv-isa-manual/issues/43
// #   define CV_PAUSE(v) do { for (int __delay = (v); __delay > 0; --__delay) { asm volatile("pause"); } } while (0)
#   define CV_PAUSE(v) करो { क्रम (पूर्णांक __delay = (v); __delay > 0; --__delay) { यंत्र अस्थिर("nop"); } } जबतक (0)
# elif defined __GNUC__ && defined __loongarch__
#   define CV_PAUSE(v) करो { क्रम (पूर्णांक __delay = (v); __delay > 0; --__delay) { यंत्र अस्थिर("nop"); } } जबतक (0)
# अन्यथा
#   warning "Can't detect 'pause' (CPU-yield) instruction on the target platform. Specify CV_PAUSE() definition via compiler flags."
#   define CV_PAUSE(...) करो { /* no-op: works, but not effective */ } जबतक (0)
# endif
#पूर्ण_अगर // CV_PAUSE


namespace cv
{

अटल पूर्णांक CV_ACTIVE_WAIT_PAUSE_LIMIT = (पूर्णांक)utils::get_Configuration_Parameter_आकार_T("OPENCV_THREAD_POOL_ACTIVE_WAIT_PAUSE_LIMIT", 16);  // iterations
अटल पूर्णांक CV_WORKER_ACTIVE_WAIT = (पूर्णांक)utils::get_Configuration_Parameter_आकार_T("OPENCV_THREAD_POOL_ACTIVE_WAIT_WORKER", 2000);  // iterations
अटल पूर्णांक CV_MAIN_THREAD_ACTIVE_WAIT = (पूर्णांक)utils::get_Configuration_Parameter_आकार_T("OPENCV_THREAD_POOL_ACTIVE_WAIT_MAIN", 10000); // iterations

अटल पूर्णांक CV_WORKER_ACTIVE_WAIT_THREADS_LIMIT = (पूर्णांक)utils::get_Configuration_Parameter_आकार_T("OPENCV_THREAD_POOL_ACTIVE_WAIT_THREADS_LIMIT", 0); // number of real cores

श्रेणी WorkerThread;
श्रेणी ParallelJob;

श्रेणी ThreadPool
{
खुला:
    अटल ThreadPool& instance()
    {
        CV_SINGLETON_LAZY_INIT_REF(ThreadPool, नया ThreadPool())
    }

    अटल व्योम stop()
    {
        ThreadPool& manager = instance();
        manager.reconfigure(0);
    }

    व्योम reconfigure(अचिन्हित new_threads_गिनती)
    {
        अगर (new_threads_गिनती == threads.size())
            वापस;
        pthread_mutex_lock(&mutex);
        reconfigure_(new_threads_गिनती);
        pthread_mutex_unlock(&mutex);
    }
    bool reconfigure_(अचिन्हित new_threads_गिनती); // internal implementation

    व्योम run(स्थिर Range& range, स्थिर ParallelLoopBody& body, द्विगुन nstripes);

    माप_प्रकार get_अंक_Of_Threads();

    व्योम set_अंक_Of_Threads(अचिन्हित n);

    ThreadPool();

    ~ThreadPool();

    अचिन्हित अंक_threads;

    pthread_mutex_t mutex;  // guards fields (job/threads) from non-worker threads (concurrent parallel_for calls)
#अगर defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
    pthread_cond_t cond_thread_wake;
#पूर्ण_अगर

    pthread_mutex_t mutex_notify;
    pthread_cond_t cond_thread_task_complete;

    std::vector< Ptr<WorkerThread> > threads;

    Ptr<ParallelJob> job;

#अगर_घोषित CV_PROFILE_THREADS
    द्विगुन tickFreq;
    int64 jobSubmitTime;
    काष्ठा ThreadStatistics
    {
        ThreadStatistics() : threadWait(0)
        {
            reset();
        }
        व्योम reset()
        {
            threadWake = 0;
            threadExecuteStart = 0;
            threadExecuteStop = 0;
            executedTasks = 0;
            keepActive = false;
            threadPing = get_Tick_गिनती();
        }
        int64 threadWait; // don't reset by default
        int64 threadPing; // don't reset by default
        int64 threadWake;
        int64 threadExecuteStart;
        int64 threadExecuteStop;
        int64 threadFree;
        अचिन्हित executedTasks;
        bool keepActive;

        int64 dummy_[8]; // separate cache lines

        व्योम dump(पूर्णांक id, int64 baseTime, द्विगुन tickFreq)
        {
            अगर (id < 0)
                std::अ_बाहर << "Main: ";
            अन्यथा
                म_लिखो("T%03d: ", id + 2);
            म_लिखो("wait=% 10.1f   ping=% 6.1f",
                    threadWait > 0 ? (threadWait - baseTime) / tickFreq * 1e6 : -0.0,
                    threadPing > 0 ? (threadPing - baseTime) / tickFreq * 1e6 : -0.0);
            अगर (threadWake > 0)
                म_लिखो("   wake=% 6.1f",
                    (threadWake > 0 ? (threadWake - baseTime) / tickFreq * 1e6 : -0.0));
            अगर (threadExecuteStart > 0)
            {
                म_लिखो("   exec=% 6.1f - % 6.1f   tasksDone=%5u   free=% 6.1f",
                    (threadExecuteStart > 0 ? (threadExecuteStart - baseTime) / tickFreq * 1e6 : -0.0),
                    (threadExecuteStop > 0 ? (threadExecuteStop - baseTime) / tickFreq * 1e6 : -0.0),
                    executedTasks,
                    (threadFree > 0 ? (threadFree - baseTime) / tickFreq * 1e6 : -0.0));
                अगर (id >= 0)
                    म_लिखो(" active=%s\n", keepActive ? "true" : "false");
                अन्यथा
                    म_लिखो("\n");
            }
            अन्यथा
                म_लिखो("   ------------------------------------------------------------------------------\n");
        }
    };
    ThreadStatistics threads_stat[CV_PROFILE_THREADS]; // 0 - main thread, 1..N - worker threads
#पूर्ण_अगर

};

श्रेणी WorkerThread
{
खुला:
    ThreadPool& thread_pool;
    स्थिर अचिन्हित id;
    pthread_t posix_thread;
    bool is_created;

    std::atomic<bool> stop_thread;

    std::atomic<bool> has_wake_signal;

    Ptr<ParallelJob> job;

    pthread_mutex_t mutex;
#अगर !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
    अस्थिर bool isActive;
    pthread_cond_t cond_thread_wake;
#पूर्ण_अगर

    WorkerThread(ThreadPool& thread_pool_, अचिन्हित id_) :
        thread_pool(thread_pool_),
        id(id_),
        posix_thread(0),
        is_created(false),
        stop_thread(false),
        has_wake_signal(false)
#अगर !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
        , isActive(true)
#पूर्ण_अगर
    {
        CV_LOG_VERBOSE(शून्य, 1, "MainThread: initializing new worker: " << id);
        पूर्णांक res = pthread_mutex_init(&mutex, शून्य);
        अगर (res != 0)
        {
            CV_LOG_ERROR(शून्य, id << ": Can't create thread mutex: res = " << res);
            वापस;
        }
#अगर !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
        res = pthread_cond_init(&cond_thread_wake, शून्य);
        अगर (res != 0)
        {
            CV_LOG_ERROR(शून्य, id << ": Can't create thread condition variable: res = " << res);
            वापस;
        }
#पूर्ण_अगर
        res = pthread_create(&posix_thread, शून्य, thread_loop_wrapper, (व्योम*)यह);
        अगर (res != 0)
        {
            CV_LOG_ERROR(शून्य, id << ": Can't spawn new thread: res = " << res);
        }
        अन्यथा
        {
            is_created = true;
        }
    }

    ~WorkerThread()
    {
        CV_LOG_VERBOSE(शून्य, 1, "MainThread: destroy worker thread: " << id);
        अगर (is_created)
        {
            अगर (!stop_thread)
            {
                pthread_mutex_lock(&mutex);  // to avoid signal miss due pre-check
                stop_thread = true;
                pthread_mutex_unlock(&mutex);
#अगर defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
                pthread_cond_broadcast(&thread_pool.cond_thread_wake);
#अन्यथा
                pthread_cond_signal(&cond_thread_wake);
#पूर्ण_अगर
            }
            pthread_join(posix_thread, शून्य);
        }
#अगर !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
        pthread_cond_destroy(&cond_thread_wake);
#पूर्ण_अगर
        pthread_mutex_destroy(&mutex);
    }

    व्योम thread_body();
    अटल व्योम* thread_loop_wrapper(व्योम* thread_object)
    {
#अगर_घोषित OPENCV_WITH_ITT
        __itt_thread_set_name(cv::format("OpenCVThread-%03d", cv::utils::getThreadID()).c_str());
#पूर्ण_अगर
        ((WorkerThread*)thread_object)->thread_body();
        वापस 0;
    }
};

श्रेणी ParallelJob
{
खुला:
    ParallelJob(स्थिर ThreadPool& thread_pool_, स्थिर Range& range_, स्थिर ParallelLoopBody& body_, पूर्णांक nstripes_) :
        thread_pool(thread_pool_),
        body(body_),
        range(range_),
        nstripes((अचिन्हित)nstripes_),
        is_completed(false)
    {
        CV_LOG_VERBOSE(शून्य, 5, "ParallelJob::ParallelJob(" << (व्योम*)यह << ")");
        current_task.store(0, std::memory_order_relaxed);
        active_thread_गिनती.store(0, std::memory_order_relaxed);
        completed_thread_गिनती.store(0, std::memory_order_relaxed);
        dummy0_[0] = 0, dummy1_[0] = 0, dummy2_[0] = 0; // compiler warning
    }

    ~ParallelJob()
    {
        CV_LOG_VERBOSE(शून्य, 5, "ParallelJob::~ParallelJob(" << (व्योम*)यह << ")");
    }

    अचिन्हित execute(bool is_worker_thread)
    {
        अचिन्हित executed_tasks = 0;
        स्थिर पूर्णांक task_गिनती = range.size();
        स्थिर पूर्णांक remaining_multiplier = std::min(nstripes,
                std::max(
                        std::min(100u, thread_pool.अंक_threads * 4),
                        thread_pool.अंक_threads * 2
                ));  // experimental value
        क्रम (;;)
        {
            पूर्णांक chunk_आकार = std::max(1, (task_गिनती - current_task) / remaining_multiplier);
            पूर्णांक id = current_task.fetch_add(chunk_आकार, std::memory_order_seq_cst);
            अगर (id >= task_गिनती)
                अवरोध; // no more free tasks

            executed_tasks += chunk_आकार;
            पूर्णांक start_id = id;
            पूर्णांक end_id = std::min(task_गिनती, id + chunk_आकार);
            CV_LOG_VERBOSE(शून्य, 9, "Thread: job " << start_id << "-" << end_id);

            //TODO: if (not pending exception)
            {
                body.चालक()(Range(range.start + start_id, range.start + end_id));
            }
            अगर (is_worker_thread && is_completed)
            {
                CV_LOG_ERROR(शून्य, "\t\t\t\tBUG! Job: " << (व्योम*)यह << " " << id << " " << active_thread_गिनती << " " << completed_thread_गिनती);
                CV_Assert(!is_completed); // TODO Dbg this
            }
        }
        वापस executed_tasks;
    }

    स्थिर ThreadPool& thread_pool;
    स्थिर ParallelLoopBody& body;
    स्थिर Range range;
    स्थिर अचिन्हित nstripes;

    std::atomic<पूर्णांक> current_task;  // next free part of job
    int64 dummy0_[8];  // avoid cache-line reusing for the same atomics

    std::atomic<पूर्णांक> active_thread_गिनती;  // number of threads worked on this job
    int64 dummy1_[8];  // avoid cache-line reusing for the same atomics

    std::atomic<पूर्णांक> completed_thread_गिनती;  // number of threads completed any activities on this job
    int64 dummy2_[8];  // avoid cache-line reusing for the same atomics

    std::atomic<bool> is_completed;

    // TODO exception handling
};


// Disable thread sanitization check when CV_USE_GLOBAL_WORKERS_COND_VAR is not
// set because it triggers as the main thread reads isActive while the children
// thread writes it (but it all works out because a mutex is locked in the main
// thread and isActive re-checked).
// This is to solve issue #19463.
#अगर !defined(CV_USE_GLOBAL_WORKERS_COND_VAR) && defined(__clang__) && defined(__has_feature)
#अगर __has_feature(thread_sanitizer)
__attribute__((no_sanitize("thread")))
#पूर्ण_अगर
#पूर्ण_अगर
व्योम WorkerThread::thread_body()
{
    (व्योम)cv::utils::getThreadID(); // notify OpenCV about new thread
    CV_LOG_VERBOSE(शून्य, 5, "Thread: new thread: " << id);

    bool allow_active_wait = true;

#अगर_घोषित CV_PROFILE_THREADS
    ThreadPool::ThreadStatistics& stat = thread_pool.threads_stat[id + 1];
#पूर्ण_अगर

    जबतक (!stop_thread)
    {
        CV_LOG_VERBOSE(शून्य, 5, "Thread: ... loop iteration: allow_active_wait=" << allow_active_wait << "   has_wake_signal=" << has_wake_signal);
        अगर (allow_active_wait && CV_WORKER_ACTIVE_WAIT > 0)
        {
            allow_active_wait = false;
            क्रम (पूर्णांक i = 0; i < CV_WORKER_ACTIVE_WAIT; i++)
            {
                अगर (has_wake_signal)
                    अवरोध;
                अगर (CV_ACTIVE_WAIT_PAUSE_LIMIT > 0 && (i < CV_ACTIVE_WAIT_PAUSE_LIMIT || (i & 1)))
                    CV_PAUSE(16);
                अन्यथा
                    CV_YIELD();
            }
        }
        pthread_mutex_lock(&mutex);
#अगर_घोषित CV_PROFILE_THREADS
        stat.threadWait = get_Tick_गिनती();
#पूर्ण_अगर
        जबतक (!has_wake_signal) // to handle spurious wakeups
        {
            //CV_LOG_VERBOSE(NULL, 5, "Thread: wait (sleep) ...");
#अगर defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
            pthread_cond_wait(&thread_pool.cond_thread_wake, &mutex);
#अन्यथा
            isActive = false;
            pthread_cond_wait(&cond_thread_wake, &mutex);
            isActive = true;
#पूर्ण_अगर
            CV_LOG_VERBOSE(शून्य, 5, "Thread: wake ... (has_wake_signal=" << has_wake_signal << " stop_thread=" << stop_thread << ")")
        }
#अगर_घोषित CV_PROFILE_THREADS
        stat.threadWake = get_Tick_गिनती();
#पूर्ण_अगर

        CV_LOG_VERBOSE(शून्य, 5, "Thread: checking for new job");
        अगर (CV_WORKER_ACTIVE_WAIT_THREADS_LIMIT == 0)
            allow_active_wait = true;
        Ptr<ParallelJob> j_ptr; swap(j_ptr, job);
        has_wake_signal = false;
        pthread_mutex_unlock(&mutex);

        अगर (!stop_thread)
        {
            ParallelJob* j = j_ptr;
            अगर (j)
            {
                CV_LOG_VERBOSE(शून्य, 5, "Thread: job size=" << j->range.size() << " done=" << j->current_task);
                अगर (j->current_task < j->range.size())
                {
                    पूर्णांक other = j->active_thread_गिनती.fetch_add(1, std::memory_order_seq_cst);
                    CV_LOG_VERBOSE(शून्य, 5, "Thread: processing new job (with " << other << " other threads)"); CV_UNUSED(other);
#अगर_घोषित CV_PROFILE_THREADS
                    stat.threadExecuteStart = get_Tick_गिनती();
                    stat.executedTasks = j->execute(true);
                    stat.threadExecuteStop = get_Tick_गिनती();
#अन्यथा
                    j->execute(true);
#पूर्ण_अगर
                    पूर्णांक completed = j->completed_thread_गिनती.fetch_add(1, std::memory_order_seq_cst) + 1;
                    पूर्णांक active = j->active_thread_गिनती.load(std::memory_order_acquire);
                    अगर (CV_WORKER_ACTIVE_WAIT_THREADS_LIMIT > 0)
                    {
                        allow_active_wait = true;
                        अगर (active >= CV_WORKER_ACTIVE_WAIT_THREADS_LIMIT && (id & 1) == 0) // turn off a half of threads
                            allow_active_wait = false;
                    }
                    CV_LOG_VERBOSE(शून्य, 5, "Thread: completed job processing: " << active << " " << completed);
                    अगर (active == completed)
                    {
                        bool need_signal = !j->is_completed;
                        j->is_completed = true;
                        j = शून्य; j_ptr.release();
                        अगर (need_signal)
                        {
                            CV_LOG_VERBOSE(शून्य, 5, "Thread: job finished => notifying the main thread");
                            pthread_mutex_lock(&thread_pool.mutex_notify);  // to avoid signal miss due pre-check condition
                            // empty
                            pthread_mutex_unlock(&thread_pool.mutex_notify);
                            pthread_cond_broadcast/*pthread_cond_signal*/(&thread_pool.cond_thread_task_complete);
                        }
                    }
                }
                अन्यथा
                {
                    CV_LOG_VERBOSE(शून्य, 5, "Thread: no free job tasks");
                }
            }
        }
#अगर_घोषित CV_PROFILE_THREADS
        stat.threadFree = get_Tick_गिनती();
        stat.keepActive = allow_active_wait;
#पूर्ण_अगर
    }
}

ThreadPool::ThreadPool()
{
#अगर_घोषित CV_PROFILE_THREADS
    tickFreq = getTickFrequency();
#पूर्ण_अगर

    पूर्णांक res = 0;
    res |= pthread_mutex_init(&mutex, शून्य);
    res |= pthread_mutex_init(&mutex_notify, शून्य);
#अगर defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
    res |= pthread_cond_init(&cond_thread_wake, शून्य);
#पूर्ण_अगर
    res |= pthread_cond_init(&cond_thread_task_complete, शून्य);

    अगर (0 != res)
    {
        CV_LOG_FATAL(शून्य, "Failed to initialize ThreadPool (pthreads)");
    }
    अंक_threads = default_संख्या_Of_Threads();
}

bool ThreadPool::reconfigure_(अचिन्हित new_threads_गिनती)
{
    अगर (new_threads_गिनती == threads.size())
        वापस false;

    अगर (new_threads_गिनती < threads.size())
    {
        CV_LOG_VERBOSE(शून्य, 1, "MainThread: reduce worker pool: " << threads.size() << " => " << new_threads_गिनती);
        std::vector< Ptr<WorkerThread> > release_threads(threads.size() - new_threads_गिनती);
        क्रम (माप_प्रकार i = new_threads_गिनती; i < threads.size(); ++i)
        {
            pthread_mutex_lock(&threads[i]->mutex);  // to avoid signal miss due pre-check
            threads[i]->stop_thread = true;
            threads[i]->has_wake_signal = true;
#अगर !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
            pthread_mutex_unlock(&threads[i]->mutex);
            pthread_cond_broadcast/*pthread_cond_signal*/(&threads[i]->cond_thread_wake); // wake thread
#अन्यथा
            pthread_mutex_unlock(&threads[i]->mutex);
#पूर्ण_अगर
            std::swap(threads[i], release_threads[i - new_threads_गिनती]);
        }
#अगर defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
        CV_LOG_VERBOSE(शून्य, 1, "MainThread: notify worker threads about termination...");
        pthread_cond_broadcast(&cond_thread_wake); // wake all threads
#पूर्ण_अगर
        threads.resize(new_threads_गिनती);
        release_threads.सफा();  // calls thread_join which want to lock mutex
        वापस false;
    }
    अन्यथा
    {
        CV_LOG_VERBOSE(शून्य, 1, "MainThread: upgrade worker pool: " << threads.size() << " => " << new_threads_गिनती);
        क्रम (माप_प्रकार i = threads.size(); i < new_threads_गिनती; ++i)
        {
            threads.push_back(Ptr<WorkerThread>(नया WorkerThread(*यह, (अचिन्हित)i))); // spawn more threads
        }
    }
    वापस false;
}

ThreadPool::~ThreadPool()
{
    reconfigure(0);
    pthread_cond_destroy(&cond_thread_task_complete);
#अगर defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
    pthread_cond_destroy(&cond_thread_wake);
#पूर्ण_अगर
    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&mutex_notify);
}

व्योम ThreadPool::run(स्थिर Range& range, स्थिर ParallelLoopBody& body, द्विगुन nstripes)
{
    CV_LOG_VERBOSE(शून्य, 1, "MainThread: new parallel job: num_threads=" << अंक_threads << "   range=" << range.size() << "   nstripes=" << nstripes << "   job=" << (व्योम*)job);
#अगर_घोषित CV_PROFILE_THREADS
    jobSubmitTime = get_Tick_गिनती();
    threads_stat[0].reset();
    threads_stat[0].threadWait = jobSubmitTime;
    threads_stat[0].threadWake = jobSubmitTime;
#पूर्ण_अगर
    अगर (get_अंक_Of_Threads() > 1 &&
        job == शून्य &&
        (range.size() * nstripes >= 2 || (range.size() > 1 && nstripes <= 0))
    )
    {
        pthread_mutex_lock(&mutex);
        अगर (job != शून्य)
        {
            pthread_mutex_unlock(&mutex);
            body(range);
            वापस;
        }
        reconfigure_(अंक_threads - 1);

        {
            CV_LOG_VERBOSE(शून्य, 1, "MainThread: initialize parallel job: " << range.size());
            job = Ptr<ParallelJob>(नया ParallelJob(*यह, range, body, nstripes));
            pthread_mutex_unlock(&mutex);

            CV_LOG_VERBOSE(शून्य, 5, "MainThread: wake worker threads...");
            माप_प्रकार अंक_threads_to_wake = std::min(static_cast<माप_प्रकार>(range.size()), threads.size());
            क्रम (माप_प्रकार i = 0; i < अंक_threads_to_wake; ++i)
            {
                अगर (job->current_task >= job->range.size())
                    अवरोध;
                WorkerThread& thread = *(threads[i].लो());
                अगर (
#अगर defined(__clang__) && defined(__has_feature)
#अगर __has_feature(thread_sanitizer)
                        1 ||  // Robust workaround to avoid data race warning of `thread.job`
#पूर्ण_अगर
#पूर्ण_अगर
#अगर !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
                        thread.isActive ||
#पूर्ण_अगर
                        thread.has_wake_signal
                        || !thread.job.empty()  // #10881
                )
                {
                    pthread_mutex_lock(&thread.mutex);
                    thread.job = job;
#अगर !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
                    bool isActive = thread.isActive;
#पूर्ण_अगर
                    thread.has_wake_signal = true;
#अगर_घोषित CV_PROFILE_THREADS
                    threads_stat[i + 1].reset();
#पूर्ण_अगर
                    pthread_mutex_unlock(&thread.mutex);
#अगर !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
                    अगर (!isActive)
                    {
                        pthread_cond_broadcast/*pthread_cond_signal*/(&thread.cond_thread_wake); // wake thread
                    }
#पूर्ण_अगर
                }
                अन्यथा
                {
                    CV_Assert(thread.job.empty());
                    thread.job = job;
                    thread.has_wake_signal = true;
#अगर_घोषित CV_PROFILE_THREADS
                    threads_stat[i + 1].reset();
#पूर्ण_अगर
#अगर !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
                    pthread_cond_broadcast/*pthread_cond_signal*/(&thread.cond_thread_wake); // wake thread
#पूर्ण_अगर
                }
            }
#अगर_घोषित CV_PROFILE_THREADS
            threads_stat[0].threadPing = get_Tick_गिनती();
#पूर्ण_अगर
#अगर defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
            pthread_cond_broadcast(&cond_thread_wake); // wake all threads
#पूर्ण_अगर
#अगर_घोषित CV_PROFILE_THREADS
            threads_stat[0].threadWake = get_Tick_गिनती();
#पूर्ण_अगर
            CV_LOG_VERBOSE(शून्य, 5, "MainThread: wake worker threads... (done)");

            {
                ParallelJob& j = *(यह->job);
#अगर_घोषित CV_PROFILE_THREADS
                threads_stat[0].threadExecuteStart = get_Tick_गिनती();
                threads_stat[0].executedTasks = j.execute(false);
                threads_stat[0].threadExecuteStop = get_Tick_गिनती();
#अन्यथा
                j.execute(false);
#पूर्ण_अगर
                CV_Assert(j.current_task >= j.range.size());
                CV_LOG_VERBOSE(शून्य, 5, "MainThread: complete self-tasks: " << j.active_thread_गिनती << " " << j.completed_thread_गिनती);
                अगर (job->is_completed || j.active_thread_गिनती == 0)
                {
                    job->is_completed = true;
                    CV_LOG_VERBOSE(शून्य, 5, "MainThread: no WIP worker threads");
                }
                अन्यथा
                {
                    अगर (CV_MAIN_THREAD_ACTIVE_WAIT > 0)
                    {
                        क्रम (पूर्णांक i = 0; i < CV_MAIN_THREAD_ACTIVE_WAIT; i++)  // don't spin too much in any case (inaccurate getTickCount())
                        {
                            अगर (job->is_completed)
                            {
                                CV_LOG_VERBOSE(शून्य, 5, "MainThread: job finalize (active wait) " << j.active_thread_गिनती << " " << j.completed_thread_गिनती);
                                अवरोध;
                            }
                            अगर (CV_ACTIVE_WAIT_PAUSE_LIMIT > 0 && (i < CV_ACTIVE_WAIT_PAUSE_LIMIT || (i & 1)))
                                CV_PAUSE(16);
                            अन्यथा
                                CV_YIELD();
                        }
                    }
                    अगर (!job->is_completed)
                    {
                        CV_LOG_VERBOSE(शून्य, 5, "MainThread: prepare wait " << j.active_thread_गिनती << " " << j.completed_thread_गिनती);
                        pthread_mutex_lock(&mutex_notify);
                        क्रम (;;)
                        {
                            अगर (job->is_completed)
                            {
                                CV_LOG_VERBOSE(शून्य, 5, "MainThread: job finalize (wait) " << j.active_thread_गिनती << " " << j.completed_thread_गिनती);
                                अवरोध;
                            }
                            CV_LOG_VERBOSE(शून्य, 5, "MainThread: wait completion (sleep) ...");
                            pthread_cond_wait(&cond_thread_task_complete, &mutex_notify);
                            CV_LOG_VERBOSE(शून्य, 5, "MainThread: wake");
                        }
                        pthread_mutex_unlock(&mutex_notify);
                    }
                }
            }
#अगर_घोषित CV_PROFILE_THREADS
            threads_stat[0].threadFree = get_Tick_गिनती();
            std::अ_बाहर << "Job: sz=" << range.size() << " nstripes=" << nstripes << "    Time: " << (threads_stat[0].threadFree - jobSubmitTime) / tickFreq * 1e6 << " usec" << std::प_पूर्ण;
            क्रम (पूर्णांक i = 0; i < (पूर्णांक)threads.size() + 1; i++)
            {
                threads_stat[i].dump(i - 1, jobSubmitTime, tickFreq);
            }
#पूर्ण_अगर
            अगर (job)
            {
                pthread_mutex_lock(&mutex);
                CV_LOG_VERBOSE(शून्य, 5, "MainThread: job release");
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

माप_प्रकार ThreadPool::get_अंक_Of_Threads()
{
    वापस अंक_threads;
}

व्योम ThreadPool::set_अंक_Of_Threads(अचिन्हित n)
{
    अगर (n != अंक_threads)
    {
        अंक_threads = n;
        अगर (n == 1)
           अगर (job == शून्य) reconfigure(0);  // stop worker threads immediately
    }
}

माप_प्रकार parallel_pthreads_get_threads_अंक()
{
    वापस ThreadPool::instance().get_अंक_Of_Threads();
}

व्योम parallel_pthreads_set_threads_अंक(पूर्णांक num)
{
    अगर(num < 0)
    {
        ThreadPool::instance().set_अंक_Of_Threads(0);
    }
    अन्यथा
    {
        ThreadPool::instance().set_अंक_Of_Threads(अचिन्हित(num));
    }
}

व्योम parallel_for_pthreads(स्थिर Range& range, स्थिर ParallelLoopBody& body, द्विगुन nstripes)
{
    ThreadPool::instance().run(range, body, nstripes);
}

}

#पूर्ण_अगर
