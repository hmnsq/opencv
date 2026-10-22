// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#अन्तर्भाव "precomp.hpp"

#अन्तर्भाव "parallel_impl.hpp"

#यदि_परिभाषितम् HAVE_PTHREADS_PF
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
#यदि_परिभाषितम् DECLARE_CV_YIELD
DECLARE_CV_YIELD
#यदि_अन्तः
#यदि_अपरिभाषितम् CV_YIELD
# include <thread>
# define CV_YIELD() std::this_thread::yield()
#यदि_अन्तः // CV_YIELD

// Spin lock's CPU-level yield (required for Hyper-Threading)
#यदि_परिभाषितम् DECLARE_CV_PAUSE
DECLARE_CV_PAUSE
#यदि_अन्तः
#यदि_अपरिभाषितम् CV_PAUSE
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
#यदि_अन्तः // CV_PAUSE


नामाकाश cv
{

स्थिर पूर्णाङ्क CV_सक्रिय_WAIT_PAUSE_सीमा = (पूर्णाङ्क)utils::get_Configuration_Parameter_आकार_T("OPENCV_THREAD_POOL_ACTIVE_WAIT_PAUSE_LIMIT", 16);  // iterations
स्थिर पूर्णाङ्क CV_कार्यकर्ता_सक्रिय_WAIT = (पूर्णाङ्क)utils::get_Configuration_Parameter_आकार_T("OPENCV_THREAD_POOL_ACTIVE_WAIT_WORKER", 2000);  // iterations
स्थिर पूर्णाङ्क CV_MAIN_THREAD_सक्रिय_WAIT = (पूर्णाङ्क)utils::get_Configuration_Parameter_आकार_T("OPENCV_THREAD_POOL_ACTIVE_WAIT_MAIN", 10000); // iterations

स्थिर पूर्णाङ्क CV_कार्यकर्ता_सक्रिय_WAIT_तन्तवः_सीमा = (पूर्णाङ्क)utils::get_Configuration_Parameter_आकार_T("OPENCV_THREAD_POOL_ACTIVE_WAIT_THREADS_LIMIT", 0); // number of real cores

वर्ग कार्यकर्ता_Thread;
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

    शून्य reconfigure(अचिह्नित new_तन्तवः_गणनम्)
    {
        यदि (new_तन्तवः_गणनम् == तन्तवः.size())
            प्रत्यागम;
        pthread_mutex_lock(&mutex);
        reconfigure_(new_तन्तवः_गणनम्);
        pthread_mutex_unlock(&mutex);
    }
    तार्किक reconfigure_(अचिह्नित new_तन्तवः_गणनम्); // internal implementation

    शून्य सततम्(ध्रुव Range& range, ध्रुव ParallelLoopBody& body, द्विगुण nstripes);

    size_t get_अङ्कः_सम्बन्धि_तन्तवः();

    शून्य समुच्चयः_अङ्कः_सम्बन्धि_तन्तवः(अचिह्नित n);

    ThreadPool();

    ~ThreadPool();

    अचिह्नित अङ्कः_तन्तवः_2;

    pthread_mutex_t mutex;  // guards fields (job/threads) from non-worker threads (concurrent parallel_for calls)
#यदि defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
    pthread_cond_t cond_thread_wake;
#यदि_अन्तः

    pthread_mutex_t mutex_notify;
    pthread_cond_t cond_thread_task_complete;

    std::vector< Ptr<कार्यकर्ता_Thread> > तन्तवः;

    Ptr<ParallelJob> job;

#यदि_परिभाषितम् CV_PROFILE_तन्तवः
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
            keep_सक्रिय = असत्य;
            threadPing = get_Tick_गणनम्();
        }
        int64 threadWait; // don't reset by default
        int64 threadPing; // don't reset by default
        int64 threadWake;
        int64 threadExecuteStart;
        int64 threadExecuteStop;
        int64 threadFree;
        अचिह्नित executedTasks;
        तार्किक keep_सक्रिय;

        int64 dummy_[8]; // separate cache lines

        शून्य dump(पूर्णाङ्क id, int64 आधारः_Time, द्विगुण tickFreq)
        {
            यदि (id < 0)
                std::निर्गमः << "Main: ";
            अन्यथा
                प्रारूप_लिख("T%03d: ", id + 2);
            प्रारूप_लिख("wait=% 10.1f   ping=% 6.1f",
                    threadWait > 0 ? (threadWait - आधारः_Time) / tickFreq * 1e6 : -0.0,
                    threadPing > 0 ? (threadPing - आधारः_Time) / tickFreq * 1e6 : -0.0);
            यदि (threadWake > 0)
                प्रारूप_लिख("   wake=% 6.1f",
                    (threadWake > 0 ? (threadWake - आधारः_Time) / tickFreq * 1e6 : -0.0));
            यदि (threadExecuteStart > 0)
            {
                प्रारूप_लिख("   exec=% 6.1f - % 6.1f   tasksDone=%5u   free=% 6.1f",
                    (threadExecuteStart > 0 ? (threadExecuteStart - आधारः_Time) / tickFreq * 1e6 : -0.0),
                    (threadExecuteStop > 0 ? (threadExecuteStop - आधारः_Time) / tickFreq * 1e6 : -0.0),
                    executedTasks,
                    (threadFree > 0 ? (threadFree - आधारः_Time) / tickFreq * 1e6 : -0.0));
                यदि (id >= 0)
                    प्रारूप_लिख(" active=%s\n", keep_सक्रिय ? "true" : "false");
                अन्यथा
                    प्रारूप_लिख("\n");
            }
            अन्यथा
                प्रारूप_लिख("   ------------------------------------------------------------------------------\n");
        }
    };
    ThreadStatistics तन्तवः_stat[CV_PROFILE_तन्तवः]; // 0 - main thread, 1..N - worker threads
#यदि_अन्तः

};

वर्ग कार्यकर्ता_Thread
{
सार्वजनिक:
    ThreadPool& thread_pool;
    ध्रुव अचिह्नित id;
    pthread_t posix_thread;
    तार्किक is_created;

    std::atomic<तार्किक> stop_thread;

    std::atomic<तार्किक> has_wake_signal;

    Ptr<ParallelJob> job;

    pthread_mutex_t mutex;
#यदि !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
    चञ्चल तार्किक is_सक्रिय_2;
    pthread_cond_t cond_thread_wake;
#यदि_अन्तः

    कार्यकर्ता_Thread(ThreadPool& thread_pool_, अचिह्नित id_) :
        thread_pool(thread_pool_),
        id(id_),
        posix_thread(0),
        is_created(असत्य),
        stop_thread(असत्य),
        has_wake_signal(असत्य)
#यदि !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
        , is_सक्रिय_2(सत्य)
#यदि_अन्तः
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
#यदि_अन्तः
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

    ~कार्यकर्ता_Thread()
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
#यदि_अन्तः
            }
            pthread_join(posix_thread, रिक्त);
        }
#यदि !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
        pthread_cond_destroy(&cond_thread_wake);
#यदि_अन्तः
        pthread_mutex_destroy(&mutex);
    }

    शून्य thread_body();
    स्थिर शून्य* thread_loop_wrapper(शून्य* thread_object)
    {
#यदि_परिभाषितम् OPENCV_WITH_ITT
        __itt_thread_set_name(cv::format("OpenCVThread-%03d", cv::utils::getThreadID()).c_str());
#यदि_अन्तः
        ((कार्यकर्ता_Thread*)thread_object)->thread_body();
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
        nstripes((अचिह्नित)nstripes_),
        is_completed(असत्य)
    {
        CV_LOG_VERBOSE(रिक्त, 5, "ParallelJob::ParallelJob(" << (शून्य*)एतत् << ")");
        current_task.store(0, std::memory_order_relaxed);
        सक्रिय_thread_गणनम्.store(0, std::memory_order_relaxed);
        completed_thread_गणनम्.store(0, std::memory_order_relaxed);
        dummy0_[0] = 0, dummy1_[0] = 0, dummy2_[0] = 0; // compiler warning
    }

    ~ParallelJob()
    {
        CV_LOG_VERBOSE(रिक्त, 5, "ParallelJob::~ParallelJob(" << (शून्य*)एतत् << ")");
    }

    अचिह्नित execute(तार्किक is_कार्यकर्ता_thread)
    {
        अचिह्नित executed_tasks = 0;
        ध्रुव पूर्णाङ्क task_गणनम् = range.size();
        ध्रुव पूर्णाङ्क remaining_multiplier = std::min(nstripes,
                std::max(
                        std::min(100u, thread_pool.अङ्कः_तन्तवः_2 * 4),
                        thread_pool.अङ्कः_तन्तवः_2 * 2
                ));  // experimental value
        प्रति (;;)
        {
            पूर्णाङ्क chunk_आकार = std::max(1, (task_गणनम् - current_task) / remaining_multiplier);
            पूर्णाङ्क id = current_task.fetch_add(chunk_आकार, std::memory_order_seq_cst);
            यदि (id >= task_गणनम्)
                भङ्ग; // no more free tasks

            executed_tasks += chunk_आकार;
            पूर्णाङ्क start_id = id;
            पूर्णाङ्क अन्तः_id = std::min(task_गणनम्, id + chunk_आकार);
            CV_LOG_VERBOSE(रिक्त, 9, "Thread: job " << start_id << "-" << अन्तः_id);

            //TODO: if (not pending exception)
            {
                body.संक्रियकः()(Range(range.start + start_id, range.start + अन्तः_id));
            }
            यदि (is_कार्यकर्ता_thread && is_completed)
            {
                CV_LOG_ERROR(रिक्त, "\t\t\t\tBUG! Job: " << (शून्य*)एतत् << " " << id << " " << सक्रिय_thread_गणनम् << " " << completed_thread_गणनम्);
                CV_Assert(!is_completed); // TODO Dbg this
            }
        }
        प्रत्यागम executed_tasks;
    }

    ध्रुव ThreadPool& thread_pool;
    ध्रुव ParallelLoopBody& body;
    ध्रुव Range range;
    ध्रुव अचिह्नित nstripes;

    std::atomic<पूर्णाङ्क> current_task;  // next free part of job
    int64 dummy0_[8];  // avoid cache-line reusing for the same atomics

    std::atomic<पूर्णाङ्क> सक्रिय_thread_गणनम्;  // number of threads worked on this job
    int64 dummy1_[8];  // avoid cache-line reusing for the same atomics

    std::atomic<पूर्णाङ्क> completed_thread_गणनम्;  // number of threads completed any activities on this job
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
#यदि_अन्तः
#यदि_अन्तः
शून्य कार्यकर्ता_Thread::thread_body()
{
    (शून्य)cv::utils::getThreadID(); // notify OpenCV about new thread
    CV_LOG_VERBOSE(रिक्त, 5, "Thread: new thread: " << id);

    तार्किक allow_सक्रिय_wait = सत्य;

#यदि_परिभाषितम् CV_PROFILE_तन्तवः
    ThreadPool::ThreadStatistics& stat = thread_pool.तन्तवः_stat[id + 1];
#यदि_अन्तः

    यावत् (!stop_thread)
    {
        CV_LOG_VERBOSE(रिक्त, 5, "Thread: ... loop iteration: allow_active_wait=" << allow_सक्रिय_wait << "   has_wake_signal=" << has_wake_signal);
        यदि (allow_सक्रिय_wait && CV_कार्यकर्ता_सक्रिय_WAIT > 0)
        {
            allow_सक्रिय_wait = असत्य;
            प्रति (पूर्णाङ्क i = 0; i < CV_कार्यकर्ता_सक्रिय_WAIT; i++)
            {
                यदि (has_wake_signal)
                    भङ्ग;
                यदि (CV_सक्रिय_WAIT_PAUSE_सीमा > 0 && (i < CV_सक्रिय_WAIT_PAUSE_सीमा || (i & 1)))
                    CV_PAUSE(16);
                अन्यथा
                    CV_YIELD();
            }
        }
        pthread_mutex_lock(&mutex);
#यदि_परिभाषितम् CV_PROFILE_तन्तवः
        stat.threadWait = get_Tick_गणनम्();
#यदि_अन्तः
        यावत् (!has_wake_signal) // to handle spurious wakeups
        {
            //CV_LOG_VERBOSE(NULL, 5, "Thread: wait (sleep) ...");
#यदि defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
            pthread_cond_wait(&thread_pool.cond_thread_wake, &mutex);
#अन्यथा
            is_सक्रिय_2 = असत्य;
            pthread_cond_wait(&cond_thread_wake, &mutex);
            is_सक्रिय_2 = सत्य;
#यदि_अन्तः
            CV_LOG_VERBOSE(रिक्त, 5, "Thread: wake ... (has_wake_signal=" << has_wake_signal << " stop_thread=" << stop_thread << ")")
        }
#यदि_परिभाषितम् CV_PROFILE_तन्तवः
        stat.threadWake = get_Tick_गणनम्();
#यदि_अन्तः

        CV_LOG_VERBOSE(रिक्त, 5, "Thread: checking for new job");
        यदि (CV_कार्यकर्ता_सक्रिय_WAIT_तन्तवः_सीमा == 0)
            allow_सक्रिय_wait = सत्य;
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
                    पूर्णाङ्क other = j->सक्रिय_thread_गणनम्.fetch_add(1, std::memory_order_seq_cst);
                    CV_LOG_VERBOSE(रिक्त, 5, "Thread: processing new job (with " << other << " other threads)"); CV_अप्रयुक्तम्(other);
#यदि_परिभाषितम् CV_PROFILE_तन्तवः
                    stat.threadExecuteStart = get_Tick_गणनम्();
                    stat.executedTasks = j->execute(सत्य);
                    stat.threadExecuteStop = get_Tick_गणनम्();
#अन्यथा
                    j->execute(सत्य);
#यदि_अन्तः
                    पूर्णाङ्क completed = j->completed_thread_गणनम्.fetch_add(1, std::memory_order_seq_cst) + 1;
                    पूर्णाङ्क सक्रिय = j->सक्रिय_thread_गणनम्.load(std::memory_order_acquire);
                    यदि (CV_कार्यकर्ता_सक्रिय_WAIT_तन्तवः_सीमा > 0)
                    {
                        allow_सक्रिय_wait = सत्य;
                        यदि (सक्रिय >= CV_कार्यकर्ता_सक्रिय_WAIT_तन्तवः_सीमा && (id & 1) == 0) // turn off a half of threads
                            allow_सक्रिय_wait = असत्य;
                    }
                    CV_LOG_VERBOSE(रिक्त, 5, "Thread: completed job processing: " << सक्रिय << " " << completed);
                    यदि (सक्रिय == completed)
                    {
                        तार्किक need_signal = !j->is_completed;
                        j->is_completed = सत्य;
                        j = रिक्त; j_ptr.release();
                        यदि (need_signal)
                        {
                            CV_LOG_VERBOSE(रिक्त, 5, "Thread: job finished => notifying the main thread");
                            pthread_mutex_lock(&thread_pool.mutex_notify);  // to avoid signal miss due pre-check condition
                            //रिक्तानि
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
#यदि_परिभाषितम् CV_PROFILE_तन्तवः
        stat.threadFree = get_Tick_गणनम्();
        stat.keep_सक्रिय = allow_सक्रिय_wait;
#यदि_अन्तः
    }
}

ThreadPool::ThreadPool()
{
#यदि_परिभाषितम् CV_PROFILE_तन्तवः
    tickFreq = getTickFrequency();
#यदि_अन्तः

    पूर्णाङ्क res = 0;
    res |= pthread_mutex_init(&mutex, रिक्त);
    res |= pthread_mutex_init(&mutex_notify, रिक्त);
#यदि defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
    res |= pthread_cond_init(&cond_thread_wake, रिक्त);
#यदि_अन्तः
    res |= pthread_cond_init(&cond_thread_task_complete, रिक्त);

    यदि (0 != res)
    {
        CV_LOG_FATAL(रिक्त, "Failed to initialize ThreadPool (pthreads)");
    }
    अङ्कः_तन्तवः_2 = default_सङ्ख्या_सम्बन्धि_तन्तवः();
}

तार्किक ThreadPool::reconfigure_(अचिह्नित new_तन्तवः_गणनम्)
{
    यदि (new_तन्तवः_गणनम् == तन्तवः.size())
        प्रत्यागम असत्य;

    यदि (new_तन्तवः_गणनम् < तन्तवः.size())
    {
        CV_LOG_VERBOSE(रिक्त, 1, "MainThread: reduce worker pool: " << तन्तवः.size() << " => " << new_तन्तवः_गणनम्);
        std::vector< Ptr<कार्यकर्ता_Thread> > release_तन्तवः(तन्तवः.size() - new_तन्तवः_गणनम्);
        प्रति (size_t i = new_तन्तवः_गणनम्; i < तन्तवः.size(); ++i)
        {
            pthread_mutex_lock(&तन्तवः[i]->mutex);  // to avoid signal miss due pre-check
            तन्तवः[i]->stop_thread = सत्य;
            तन्तवः[i]->has_wake_signal = सत्य;
#यदि !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
            pthread_mutex_unlock(&तन्तवः[i]->mutex);
            pthread_cond_broadcast/*pthread_cond_signal*/(&तन्तवः[i]->cond_thread_wake); // wake thread
#अन्यथा
            pthread_mutex_unlock(&तन्तवः[i]->mutex);
#यदि_अन्तः
            std::swap(तन्तवः[i], release_तन्तवः[i - new_तन्तवः_गणनम्]);
        }
#यदि defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
        CV_LOG_VERBOSE(रिक्त, 1, "MainThread: notify worker threads about termination...");
        pthread_cond_broadcast(&cond_thread_wake); // wake all threads
#यदि_अन्तः
        तन्तवः.resize(new_तन्तवः_गणनम्);
        release_तन्तवः.clear();  // calls thread_join which want to lock mutex
        प्रत्यागम असत्य;
    }
    अन्यथा
    {
        CV_LOG_VERBOSE(रिक्त, 1, "MainThread: upgrade worker pool: " << तन्तवः.size() << " => " << new_तन्तवः_गणनम्);
        प्रति (size_t i = तन्तवः.size(); i < new_तन्तवः_गणनम्; ++i)
        {
            तन्तवः.push_back(Ptr<कार्यकर्ता_Thread>(नूतन कार्यकर्ता_Thread(*एतत्, (अचिह्नित)i))); // spawn more threads
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
#यदि_अन्तः
    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&mutex_notify);
}

शून्य ThreadPool::सततम्(ध्रुव Range& range, ध्रुव ParallelLoopBody& body, द्विगुण nstripes)
{
    CV_LOG_VERBOSE(रिक्त, 1, "MainThread: new parallel job: num_threads=" << अङ्कः_तन्तवः_2 << "   range=" << range.size() << "   nstripes=" << nstripes << "   job=" << (शून्य*)job);
#यदि_परिभाषितम् CV_PROFILE_तन्तवः
    jobSubmitTime = get_Tick_गणनम्();
    तन्तवः_stat[0].reset();
    तन्तवः_stat[0].threadWait = jobSubmitTime;
    तन्तवः_stat[0].threadWake = jobSubmitTime;
#यदि_अन्तः
    यदि (get_अङ्कः_सम्बन्धि_तन्तवः() > 1 &&
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
        reconfigure_(अङ्कः_तन्तवः_2 - 1);

        {
            CV_LOG_VERBOSE(रिक्त, 1, "MainThread: initialize parallel job: " << range.size());
            job = Ptr<ParallelJob>(नूतन ParallelJob(*एतत्, range, body, nstripes));
            pthread_mutex_unlock(&mutex);

            CV_LOG_VERBOSE(रिक्त, 5, "MainThread: wake worker threads...");
            size_t अङ्कः_तन्तवः_गन्तव्यम्_wake = std::min(static_cast<size_t>(range.size()), तन्तवः.size());
            प्रति (size_t i = 0; i < अङ्कः_तन्तवः_गन्तव्यम्_wake; ++i)
            {
                यदि (job->current_task >= job->range.size())
                    भङ्ग;
                कार्यकर्ता_Thread& thread = *(तन्तवः[i].get());
                यदि (
#यदि defined(__clang__) && defined(__has_feature)
#यदि __has_feature(thread_sanitizer)
                        1 ||  // Robust workaround to avoid data race warning of `thread.job`
#यदि_अन्तः
#यदि_अन्तः
#यदि !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
                        thread.is_सक्रिय_2 ||
#यदि_अन्तः
                        thread.has_wake_signal
                        || !thread.job.empty()  // #10881
                )
                {
                    pthread_mutex_lock(&thread.mutex);
                    thread.job = job;
#यदि !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
                    तार्किक is_सक्रिय_2 = thread.is_सक्रिय_2;
#यदि_अन्तः
                    thread.has_wake_signal = सत्य;
#यदि_परिभाषितम् CV_PROFILE_तन्तवः
                    तन्तवः_stat[i + 1].reset();
#यदि_अन्तः
                    pthread_mutex_unlock(&thread.mutex);
#यदि !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
                    यदि (!is_सक्रिय_2)
                    {
                        pthread_cond_broadcast/*pthread_cond_signal*/(&thread.cond_thread_wake); // wake thread
                    }
#यदि_अन्तः
                }
                अन्यथा
                {
                    CV_Assert(thread.job.empty());
                    thread.job = job;
                    thread.has_wake_signal = सत्य;
#यदि_परिभाषितम् CV_PROFILE_तन्तवः
                    तन्तवः_stat[i + 1].reset();
#यदि_अन्तः
#यदि !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
                    pthread_cond_broadcast/*pthread_cond_signal*/(&thread.cond_thread_wake); // wake thread
#यदि_अन्तः
                }
            }
#यदि_परिभाषितम् CV_PROFILE_तन्तवः
            तन्तवः_stat[0].threadPing = get_Tick_गणनम्();
#यदि_अन्तः
#यदि defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
            pthread_cond_broadcast(&cond_thread_wake); // wake all threads
#यदि_अन्तः
#यदि_परिभाषितम् CV_PROFILE_तन्तवः
            तन्तवः_stat[0].threadWake = get_Tick_गणनम्();
#यदि_अन्तः
            CV_LOG_VERBOSE(रिक्त, 5, "MainThread: wake worker threads... (done)");

            {
                ParallelJob& j = *(एतत्->job);
#यदि_परिभाषितम् CV_PROFILE_तन्तवः
                तन्तवः_stat[0].threadExecuteStart = get_Tick_गणनम्();
                तन्तवः_stat[0].executedTasks = j.execute(असत्य);
                तन्तवः_stat[0].threadExecuteStop = get_Tick_गणनम्();
#अन्यथा
                j.execute(असत्य);
#यदि_अन्तः
                CV_Assert(j.current_task >= j.range.size());
                CV_LOG_VERBOSE(रिक्त, 5, "MainThread: complete self-tasks: " << j.सक्रिय_thread_गणनम् << " " << j.completed_thread_गणनम्);
                यदि (job->is_completed || j.सक्रिय_thread_गणनम् == 0)
                {
                    job->is_completed = सत्य;
                    CV_LOG_VERBOSE(रिक्त, 5, "MainThread: no WIP worker threads");
                }
                अन्यथा
                {
                    यदि (CV_MAIN_THREAD_सक्रिय_WAIT > 0)
                    {
                        प्रति (पूर्णाङ्क i = 0; i < CV_MAIN_THREAD_सक्रिय_WAIT; i++)  // don't spin too much in any case (inaccurate getTickCount())
                        {
                            यदि (job->is_completed)
                            {
                                CV_LOG_VERBOSE(रिक्त, 5, "MainThread: job finalize (active wait) " << j.सक्रिय_thread_गणनम् << " " << j.completed_thread_गणनम्);
                                भङ्ग;
                            }
                            यदि (CV_सक्रिय_WAIT_PAUSE_सीमा > 0 && (i < CV_सक्रिय_WAIT_PAUSE_सीमा || (i & 1)))
                                CV_PAUSE(16);
                            अन्यथा
                                CV_YIELD();
                        }
                    }
                    यदि (!job->is_completed)
                    {
                        CV_LOG_VERBOSE(रिक्त, 5, "MainThread: prepare wait " << j.सक्रिय_thread_गणनम् << " " << j.completed_thread_गणनम्);
                        pthread_mutex_lock(&mutex_notify);
                        प्रति (;;)
                        {
                            यदि (job->is_completed)
                            {
                                CV_LOG_VERBOSE(रिक्त, 5, "MainThread: job finalize (wait) " << j.सक्रिय_thread_गणनम् << " " << j.completed_thread_गणनम्);
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
#यदि_परिभाषितम् CV_PROFILE_तन्तवः
            तन्तवः_stat[0].threadFree = get_Tick_गणनम्();
            std::निर्गमः << "Job: sz=" << range.size() << " nstripes=" << nstripes << "    Time: " << (तन्तवः_stat[0].threadFree - jobSubmitTime) / tickFreq * 1e6 << " usec" << std::पङ्क्त्यन्तः;
            प्रति (पूर्णाङ्क i = 0; i < (पूर्णाङ्क)तन्तवः.size() + 1; i++)
            {
                तन्तवः_stat[i].dump(i - 1, jobSubmitTime, tickFreq);
            }
#यदि_अन्तः
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

size_t ThreadPool::get_अङ्कः_सम्बन्धि_तन्तवः()
{
    प्रत्यागम अङ्कः_तन्तवः_2;
}

शून्य ThreadPool::समुच्चयः_अङ्कः_सम्बन्धि_तन्तवः(अचिह्नित n)
{
    यदि (n != अङ्कः_तन्तवः_2)
    {
        अङ्कः_तन्तवः_2 = n;
        यदि (n == 1)
           यदि (job == रिक्त) reconfigure(0);  // stop worker threads immediately
    }
}

size_t parallel_pthreads_get_तन्तवः_अङ्कः()
{
    प्रत्यागम ThreadPool::instance().get_अङ्कः_सम्बन्धि_तन्तवः();
}

शून्य parallel_pthreads_समुच्चयः_तन्तवः_अङ्कः(पूर्णाङ्क num)
{
    यदि(num < 0)
    {
        ThreadPool::instance().समुच्चयः_अङ्कः_सम्बन्धि_तन्तवः(0);
    }
    अन्यथा
    {
        ThreadPool::instance().समुच्चयः_अङ्कः_सम्बन्धि_तन्तवः(अचिह्नित(num));
    }
}

शून्य parallel_for_pthreads(ध्रुव Range& range, ध्रुव ParallelLoopBody& body, द्विगुण nstripes)
{
    ThreadPool::instance().सततम्(range, body, nstripes);
}

}

#यदि_अन्तः
