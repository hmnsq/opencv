// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#شامل "precomp.hpp"

#شامل "parallel_impl.hpp"

#اگر_تعریف_شدہ HAVE_PTHREADS_PF
#شامل <pthread.h>

#شامل <opencv2/core/utils/configuration.private.hpp>

#شامل <opencv2/core/utils/logger.defines.hpp>
//#undef CV_LOG_STRIP_LEVEL
//#define CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_VERBOSE + 1
#شامل <opencv2/core/utils/logger.hpp>

#شامل <opencv2/core/utils/trace.private.hpp>

//#define CV_PROFILE_THREADS 64
//#define getTickCount getCPUTickCount  // use this if getTickCount() calls are expensive (and getCPUTickCount() is accurate)

//#define CV_USE_GLOBAL_WORKERS_COND_VAR  // not effective on many-core systems (10+)

#شامل <atomic>

// Spin lock's OS-level yield
#اگر_تعریف_شدہ DECLARE_CV_YIELD
DECLARE_CV_YIELD
#اگر_ختم
#اگر_غیر_تعریف_شدہ CV_YIELD
# include <thread>
# define CV_YIELD() std::this_thread::yield()
#اگر_ختم // CV_YIELD

// Spin lock's CPU-level yield (required for Hyper-Threading)
#اگر_تعریف_شدہ DECLARE_CV_PAUSE
DECLARE_CV_PAUSE
#اگر_ختم
#اگر_غیر_تعریف_شدہ CV_PAUSE
# اگر defined __GNUC__ && (defined __i386__ || defined __x86_64__)
#   include <x86intrin.h> /* for __rdtsc */
#   اگر !defined(__SSE2__)
      ساکن اندرونی خالی cv_non_sse_mm_pause() { __asm__ __volatile__ ("rep; nop"); }
#     define _mm_pause cv_non_sse_mm_pause
#   endif
// With Skylake CPUs and above, _mm_pause takes 140 cycles so no need for a loop.
#   define CV_PAUSE(v) کرو { (خالی)v; _mm_pause(); } جب (0)
# elif defined __GNUC__ && defined __aarch64__
#   define CV_PAUSE(v) کرو { برائے (صحیح __delay = (v); __delay > 0; --__delay) { asm متلون("yield" ::: "memory"); } } جب (0)
# elif defined __GNUC__ && defined __arm__
#   define CV_PAUSE(v) کرو { برائے (صحیح __delay = (v); __delay > 0; --__delay) { asm متلون("" ::: "memory"); } } جب (0)
# elif defined __GNUC__ && defined __mips__ && __mips_isa_rev >= 2
#   define CV_PAUSE(v) کرو { برائے (صحیح __delay = (v); __delay > 0; --__delay) { asm متلون("pause" ::: "memory"); } } جب (0)
# elif defined __APPLE__ && defined __POWERPC__
#   define CV_PAUSE(v) کرو { برائے (صحیح __delay = (v); __delay > 0; --__delay) { asm متلون("or r27,r27,r27" ::: "memory"); } } جب (0)
# elif defined __GNUC__ && defined __PPC64__
#   define CV_PAUSE(v) کرو { برائے (صحیح __delay = (v); __delay > 0; --__delay) { asm متلون("or 27,27,27" ::: "memory"); } } جب (0)
# elif defined __GNUC__ && defined __riscv
// PAUSE HINT is not part of RISC-V ISA yet, but is under discussion now. For details see:
// https://github.com/riscv/riscv-isa-manual/pull/398
// https://github.com/riscv/riscv-isa-manual/issues/43
// #   define CV_PAUSE(v) do { for (int __delay = (v); __delay > 0; --__delay) { asm volatile("pause"); } } while (0)
#   define CV_PAUSE(v) کرو { برائے (صحیح __delay = (v); __delay > 0; --__delay) { asm متلون("nop"); } } جب (0)
# elif defined __GNUC__ && defined __loongarch__
#   define CV_PAUSE(v) کرو { برائے (صحیح __delay = (v); __delay > 0; --__delay) { asm متلون("nop"); } } جب (0)
# ورنہ
#   warning "Can't detect 'pause' (CPU-yield) instruction on the target platform. Specify CV_PAUSE() definition via compiler flags."
#   define CV_PAUSE(...) کرو { /* no-op: works, but not effective */ } جب (0)
# endif
#اگر_ختم // CV_PAUSE


نامخلا cv
{

ساکن صحیح CV_فعال_WAIT_PAUSE_حد_بندی = (صحیح)utils::get_Configuration_Parameter_سائز_T("OPENCV_THREAD_POOL_ACTIVE_WAIT_PAUSE_LIMIT", 16);  // iterations
ساکن صحیح CV_کارکن_فعال_WAIT = (صحیح)utils::get_Configuration_Parameter_سائز_T("OPENCV_THREAD_POOL_ACTIVE_WAIT_WORKER", 2000);  // iterations
ساکن صحیح CV_MAIN_THREAD_فعال_WAIT = (صحیح)utils::get_Configuration_Parameter_سائز_T("OPENCV_THREAD_POOL_ACTIVE_WAIT_MAIN", 10000); // iterations

ساکن صحیح CV_کارکن_فعال_WAIT_دھاگے_حد_بندی = (صحیح)utils::get_Configuration_Parameter_سائز_T("OPENCV_THREAD_POOL_ACTIVE_WAIT_THREADS_LIMIT", 0); // number of real cores

جماعت کارکن_Thread;
جماعت Parallel_کام;

جماعت ThreadPool
{
عوامی:
    ساکن ThreadPool& instance()
    {
        CV_SINGLETON_LAZY_INIT_REF(ThreadPool, نیا ThreadPool())
    }

    ساکن خالی stop()
    {
        ThreadPool& manager = instance();
        manager.reconfigure(0);
    }

    خالی reconfigure(غیرعلامتی new_دھاگے_گنتی)
    {
        اگر (new_دھاگے_گنتی == دھاگے.size())
            واپس;
        pthread_mutex_lock(&mutex);
        reconfigure_(new_دھاگے_گنتی);
        pthread_mutex_unlock(&mutex);
    }
    منطقی reconfigure_(غیرعلامتی new_دھاگے_گنتی); // internal implementation

    خالی تسلسل(ثابت Range& range, ثابت ParallelLoopBody& body, دوہرا nstripes);

    size_t get_عدد_کا_دھاگے();

    خالی سیٹ_عدد_کا_دھاگے(غیرعلامتی n);

    ThreadPool();

    ~ThreadPool();

    غیرعلامتی عدد_دھاگے_2;

    pthread_mutex_t mutex;  // guards fields (job/threads) from non-worker threads (concurrent parallel_for calls)
#اگر defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
    pthread_cond_t cond_thread_wake;
#اگر_ختم

    pthread_mutex_t mutex_notify;
    pthread_cond_t cond_thread_task_complete;

    std::vector< Ptr<کارکن_Thread> > دھاگے;

    Ptr<Parallel_کام> کام;

#اگر_تعریف_شدہ CV_PROFILE_دھاگے
    دوہرا tickFreq;
    int64 کام_Submit_وقت;
    ساخت ThreadStatistics
    {
        ThreadStatistics() : threadWait(0)
        {
            reset();
        }
        خالی reset()
        {
            threadWake = 0;
            thread_Execute_آغاز = 0;
            threadExecuteStop = 0;
            executedTasks = 0;
            keep_فعال = جھوٹ;
            threadPing = get_Tick_گنتی();
        }
        int64 threadWait; // don't reset by default
        int64 threadPing; // don't reset by default
        int64 threadWake;
        int64 thread_Execute_آغاز;
        int64 threadExecuteStop;
        int64 threadFree;
        غیرعلامتی executedTasks;
        منطقی keep_فعال;

        int64 dummy_[8]; // separate cache lines

        خالی dump(صحیح id, int64 بنیاد_وقت, دوہرا tickFreq)
        {
            اگر (id < 0)
                std::خارج << "Main: ";
            ورنہ
                لکھو("T%03d: ", id + 2);
            لکھو("wait=% 10.1f   ping=% 6.1f",
                    threadWait > 0 ? (threadWait - بنیاد_وقت) / tickFreq * 1e6 : -0.0,
                    threadPing > 0 ? (threadPing - بنیاد_وقت) / tickFreq * 1e6 : -0.0);
            اگر (threadWake > 0)
                لکھو("   wake=% 6.1f",
                    (threadWake > 0 ? (threadWake - بنیاد_وقت) / tickFreq * 1e6 : -0.0));
            اگر (thread_Execute_آغاز > 0)
            {
                لکھو("   exec=% 6.1f - % 6.1f   tasksDone=%5u   free=% 6.1f",
                    (thread_Execute_آغاز > 0 ? (thread_Execute_آغاز - بنیاد_وقت) / tickFreq * 1e6 : -0.0),
                    (threadExecuteStop > 0 ? (threadExecuteStop - بنیاد_وقت) / tickFreq * 1e6 : -0.0),
                    executedTasks,
                    (threadFree > 0 ? (threadFree - بنیاد_وقت) / tickFreq * 1e6 : -0.0));
                اگر (id >= 0)
                    لکھو(" active=%s\n", keep_فعال ? "true" : "false");
                ورنہ
                    لکھو("\n");
            }
            ورنہ
                لکھو("   ------------------------------------------------------------------------------\n");
        }
    };
    ThreadStatistics دھاگے_stat[CV_PROFILE_دھاگے]; // 0 - main thread, 1..N - worker threads
#اگر_ختم

};

جماعت کارکن_Thread
{
عوامی:
    ThreadPool& thread_pool;
    ثابت غیرعلامتی id;
    pthread_t posix_thread;
    منطقی is_created;

    std::atomic<منطقی> stop_thread;

    std::atomic<منطقی> has_wake_signal;

    Ptr<Parallel_کام> کام;

    pthread_mutex_t mutex;
#اگر !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
    متلون منطقی is_فعال_2;
    pthread_cond_t cond_thread_wake;
#اگر_ختم

    کارکن_Thread(ThreadPool& thread_pool_, غیرعلامتی id_) :
        thread_pool(thread_pool_),
        id(id_),
        posix_thread(0),
        is_created(جھوٹ),
        stop_thread(جھوٹ),
        has_wake_signal(جھوٹ)
#اگر !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
        , is_فعال_2(سچ)
#اگر_ختم
    {
        CV_LOG_VERBOSE(تہی, 1, "MainThread: initializing new worker: " << id);
        صحیح res = pthread_mutex_init(&mutex, تہی);
        اگر (res != 0)
        {
            CV_LOG_ERROR(تہی, id << ": Can't create thread mutex: res = " << res);
            واپس;
        }
#اگر !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
        res = pthread_cond_init(&cond_thread_wake, تہی);
        اگر (res != 0)
        {
            CV_LOG_ERROR(تہی, id << ": Can't create thread condition variable: res = " << res);
            واپس;
        }
#اگر_ختم
        res = pthread_create(&posix_thread, تہی, thread_loop_wrapper, (خالی*)یہ);
        اگر (res != 0)
        {
            CV_LOG_ERROR(تہی, id << ": Can't spawn new thread: res = " << res);
        }
        ورنہ
        {
            is_created = سچ;
        }
    }

    ~کارکن_Thread()
    {
        CV_LOG_VERBOSE(تہی, 1, "MainThread: destroy worker thread: " << id);
        اگر (is_created)
        {
            اگر (!stop_thread)
            {
                pthread_mutex_lock(&mutex);  // to avoid signal miss due pre-check
                stop_thread = سچ;
                pthread_mutex_unlock(&mutex);
#اگر defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
                pthread_cond_broadcast(&thread_pool.cond_thread_wake);
#ورنہ
                pthread_cond_signal(&cond_thread_wake);
#اگر_ختم
            }
            pthread_join(posix_thread, تہی);
        }
#اگر !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
        pthread_cond_destroy(&cond_thread_wake);
#اگر_ختم
        pthread_mutex_destroy(&mutex);
    }

    خالی thread_body();
    ساکن خالی* thread_loop_wrapper(خالی* thread_object)
    {
#اگر_تعریف_شدہ OPENCV_WITH_ITT
        __itt_thread_set_name(cv::format("OpenCVThread-%03d", cv::utils::getThreadID()).c_str());
#اگر_ختم
        ((کارکن_Thread*)thread_object)->thread_body();
        واپس 0;
    }
};

جماعت Parallel_کام
{
عوامی:
    Parallel_کام(ثابت ThreadPool& thread_pool_, ثابت Range& range_, ثابت ParallelLoopBody& body_, صحیح nstripes_) :
        thread_pool(thread_pool_),
        body(body_),
        range(range_),
        nstripes((غیرعلامتی)nstripes_),
        is_completed(جھوٹ)
    {
        CV_LOG_VERBOSE(تہی, 5, "ParallelJob::ParallelJob(" << (خالی*)یہ << ")");
        current_task.store(0, std::memory_order_relaxed);
        فعال_thread_گنتی.store(0, std::memory_order_relaxed);
        completed_thread_گنتی.store(0, std::memory_order_relaxed);
        dummy0_[0] = 0, dummy1_[0] = 0, dummy2_[0] = 0; // compiler warning
    }

    ~Parallel_کام()
    {
        CV_LOG_VERBOSE(تہی, 5, "ParallelJob::~ParallelJob(" << (خالی*)یہ << ")");
    }

    غیرعلامتی execute(منطقی is_کارکن_thread)
    {
        غیرعلامتی executed_tasks = 0;
        ثابت صحیح task_گنتی = range.size();
        ثابت صحیح بقایا_multiplier = std::min(nstripes,
                std::max(
                        std::min(100u, thread_pool.عدد_دھاگے_2 * 4),
                        thread_pool.عدد_دھاگے_2 * 2
                ));  // experimental value
        برائے (;;)
        {
            صحیح chunk_سائز = std::max(1, (task_گنتی - current_task) / بقایا_multiplier);
            صحیح id = current_task.fetch_add(chunk_سائز, std::memory_order_seq_cst);
            اگر (id >= task_گنتی)
                توڑو; // no more free tasks

            executed_tasks += chunk_سائز;
            صحیح آغاز_id = id;
            صحیح انتہا_id = std::min(task_گنتی, id + chunk_سائز);
            CV_LOG_VERBOSE(تہی, 9, "Thread: job " << آغاز_id << "-" << انتہا_id);

            //TODO: if (not pending exception)
            {
                body.عامل()(Range(range.آغاز + آغاز_id, range.آغاز + انتہا_id));
            }
            اگر (is_کارکن_thread && is_completed)
            {
                CV_LOG_ERROR(تہی, "\t\t\t\tBUG! Job: " << (خالی*)یہ << " " << id << " " << فعال_thread_گنتی << " " << completed_thread_گنتی);
                CV_Assert(!is_completed); // TODO Dbg this
            }
        }
        واپس executed_tasks;
    }

    ثابت ThreadPool& thread_pool;
    ثابت ParallelLoopBody& body;
    ثابت Range range;
    ثابت غیرعلامتی nstripes;

    std::atomic<صحیح> current_task;  // next free part of job
    int64 dummy0_[8];  // avoid cache-line reusing for the same atomics

    std::atomic<صحیح> فعال_thread_گنتی;  // number of threads worked on this job
    int64 dummy1_[8];  // avoid cache-line reusing for the same atomics

    std::atomic<صحیح> completed_thread_گنتی;  // number of threads completed any activities on this job
    int64 dummy2_[8];  // avoid cache-line reusing for the same atomics

    std::atomic<منطقی> is_completed;

    // TODO exception handling
};


// Disable thread sanitization check when CV_USE_GLOBAL_WORKERS_COND_VAR is not
// set because it triggers as the main thread reads isActive while the children
// thread writes it (but it all works out because a mutex is locked in the main
// thread and isActive re-checked).
// This is to solve issue #19463.
#اگر !defined(CV_USE_GLOBAL_WORKERS_COND_VAR) && defined(__clang__) && defined(__has_feature)
#اگر __has_feature(thread_sanitizer)
__attribute__((no_sanitize("thread")))
#اگر_ختم
#اگر_ختم
خالی کارکن_Thread::thread_body()
{
    (خالی)cv::utils::getThreadID(); // notify OpenCV about new thread
    CV_LOG_VERBOSE(تہی, 5, "Thread: new thread: " << id);

    منطقی allow_فعال_wait = سچ;

#اگر_تعریف_شدہ CV_PROFILE_دھاگے
    ThreadPool::ThreadStatistics& stat = thread_pool.دھاگے_stat[id + 1];
#اگر_ختم

    جب (!stop_thread)
    {
        CV_LOG_VERBOSE(تہی, 5, "Thread: ... loop iteration: allow_active_wait=" << allow_فعال_wait << "   has_wake_signal=" << has_wake_signal);
        اگر (allow_فعال_wait && CV_کارکن_فعال_WAIT > 0)
        {
            allow_فعال_wait = جھوٹ;
            برائے (صحیح i = 0; i < CV_کارکن_فعال_WAIT; i++)
            {
                اگر (has_wake_signal)
                    توڑو;
                اگر (CV_فعال_WAIT_PAUSE_حد_بندی > 0 && (i < CV_فعال_WAIT_PAUSE_حد_بندی || (i & 1)))
                    CV_PAUSE(16);
                ورنہ
                    CV_YIELD();
            }
        }
        pthread_mutex_lock(&mutex);
#اگر_تعریف_شدہ CV_PROFILE_دھاگے
        stat.threadWait = get_Tick_گنتی();
#اگر_ختم
        جب (!has_wake_signal) // to handle spurious wakeups
        {
            //CV_LOG_VERBOSE(NULL, 5, "Thread: wait (sleep) ...");
#اگر defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
            pthread_cond_wait(&thread_pool.cond_thread_wake, &mutex);
#ورنہ
            is_فعال_2 = جھوٹ;
            pthread_cond_wait(&cond_thread_wake, &mutex);
            is_فعال_2 = سچ;
#اگر_ختم
            CV_LOG_VERBOSE(تہی, 5, "Thread: wake ... (has_wake_signal=" << has_wake_signal << " stop_thread=" << stop_thread << ")")
        }
#اگر_تعریف_شدہ CV_PROFILE_دھاگے
        stat.threadWake = get_Tick_گنتی();
#اگر_ختم

        CV_LOG_VERBOSE(تہی, 5, "Thread: checking for new job");
        اگر (CV_کارکن_فعال_WAIT_دھاگے_حد_بندی == 0)
            allow_فعال_wait = سچ;
        Ptr<Parallel_کام> j_ptr; swap(j_ptr, کام);
        has_wake_signal = جھوٹ;
        pthread_mutex_unlock(&mutex);

        اگر (!stop_thread)
        {
            Parallel_کام* j = j_ptr;
            اگر (j)
            {
                CV_LOG_VERBOSE(تہی, 5, "Thread: job size=" << j->range.size() << " done=" << j->current_task);
                اگر (j->current_task < j->range.size())
                {
                    صحیح other = j->فعال_thread_گنتی.fetch_add(1, std::memory_order_seq_cst);
                    CV_LOG_VERBOSE(تہی, 5, "Thread: processing new job (with " << other << " other threads)"); CV_غیر_مستعمل(other);
#اگر_تعریف_شدہ CV_PROFILE_دھاگے
                    stat.thread_Execute_آغاز = get_Tick_گنتی();
                    stat.executedTasks = j->execute(سچ);
                    stat.threadExecuteStop = get_Tick_گنتی();
#ورنہ
                    j->execute(سچ);
#اگر_ختم
                    صحیح completed = j->completed_thread_گنتی.fetch_add(1, std::memory_order_seq_cst) + 1;
                    صحیح فعال = j->فعال_thread_گنتی.load(std::memory_order_acquire);
                    اگر (CV_کارکن_فعال_WAIT_دھاگے_حد_بندی > 0)
                    {
                        allow_فعال_wait = سچ;
                        اگر (فعال >= CV_کارکن_فعال_WAIT_دھاگے_حد_بندی && (id & 1) == 0) // turn off a half of threads
                            allow_فعال_wait = جھوٹ;
                    }
                    CV_LOG_VERBOSE(تہی, 5, "Thread: completed job processing: " << فعال << " " << completed);
                    اگر (فعال == completed)
                    {
                        منطقی need_signal = !j->is_completed;
                        j->is_completed = سچ;
                        j = تہی; j_ptr.release();
                        اگر (need_signal)
                        {
                            CV_LOG_VERBOSE(تہی, 5, "Thread: job finished => notifying the main thread");
                            pthread_mutex_lock(&thread_pool.mutex_notify);  // to avoid signal miss due pre-check condition
                            //خالی_خانے
                            pthread_mutex_unlock(&thread_pool.mutex_notify);
                            pthread_cond_broadcast/*pthread_cond_signal*/(&thread_pool.cond_thread_task_complete);
                        }
                    }
                }
                ورنہ
                {
                    CV_LOG_VERBOSE(تہی, 5, "Thread: no free job tasks");
                }
            }
        }
#اگر_تعریف_شدہ CV_PROFILE_دھاگے
        stat.threadFree = get_Tick_گنتی();
        stat.keep_فعال = allow_فعال_wait;
#اگر_ختم
    }
}

ThreadPool::ThreadPool()
{
#اگر_تعریف_شدہ CV_PROFILE_دھاگے
    tickFreq = getTickFrequency();
#اگر_ختم

    صحیح res = 0;
    res |= pthread_mutex_init(&mutex, تہی);
    res |= pthread_mutex_init(&mutex_notify, تہی);
#اگر defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
    res |= pthread_cond_init(&cond_thread_wake, تہی);
#اگر_ختم
    res |= pthread_cond_init(&cond_thread_task_complete, تہی);

    اگر (0 != res)
    {
        CV_LOG_FATAL(تہی, "Failed to initialize ThreadPool (pthreads)");
    }
    عدد_دھاگے_2 = default_نمبر_کا_دھاگے();
}

منطقی ThreadPool::reconfigure_(غیرعلامتی new_دھاگے_گنتی)
{
    اگر (new_دھاگے_گنتی == دھاگے.size())
        واپس جھوٹ;

    اگر (new_دھاگے_گنتی < دھاگے.size())
    {
        CV_LOG_VERBOSE(تہی, 1, "MainThread: reduce worker pool: " << دھاگے.size() << " => " << new_دھاگے_گنتی);
        std::vector< Ptr<کارکن_Thread> > release_دھاگے(دھاگے.size() - new_دھاگے_گنتی);
        برائے (size_t i = new_دھاگے_گنتی; i < دھاگے.size(); ++i)
        {
            pthread_mutex_lock(&دھاگے[i]->mutex);  // to avoid signal miss due pre-check
            دھاگے[i]->stop_thread = سچ;
            دھاگے[i]->has_wake_signal = سچ;
#اگر !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
            pthread_mutex_unlock(&دھاگے[i]->mutex);
            pthread_cond_broadcast/*pthread_cond_signal*/(&دھاگے[i]->cond_thread_wake); // wake thread
#ورنہ
            pthread_mutex_unlock(&دھاگے[i]->mutex);
#اگر_ختم
            std::swap(دھاگے[i], release_دھاگے[i - new_دھاگے_گنتی]);
        }
#اگر defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
        CV_LOG_VERBOSE(تہی, 1, "MainThread: notify worker threads about termination...");
        pthread_cond_broadcast(&cond_thread_wake); // wake all threads
#اگر_ختم
        دھاگے.resize(new_دھاگے_گنتی);
        release_دھاگے.clear();  // calls thread_join which want to lock mutex
        واپس جھوٹ;
    }
    ورنہ
    {
        CV_LOG_VERBOSE(تہی, 1, "MainThread: upgrade worker pool: " << دھاگے.size() << " => " << new_دھاگے_گنتی);
        برائے (size_t i = دھاگے.size(); i < new_دھاگے_گنتی; ++i)
        {
            دھاگے.push_back(Ptr<کارکن_Thread>(نیا کارکن_Thread(*یہ, (غیرعلامتی)i))); // spawn more threads
        }
    }
    واپس جھوٹ;
}

ThreadPool::~ThreadPool()
{
    reconfigure(0);
    pthread_cond_destroy(&cond_thread_task_complete);
#اگر defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
    pthread_cond_destroy(&cond_thread_wake);
#اگر_ختم
    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&mutex_notify);
}

خالی ThreadPool::تسلسل(ثابت Range& range, ثابت ParallelLoopBody& body, دوہرا nstripes)
{
    CV_LOG_VERBOSE(تہی, 1, "MainThread: new parallel job: num_threads=" << عدد_دھاگے_2 << "   range=" << range.size() << "   nstripes=" << nstripes << "   job=" << (خالی*)کام);
#اگر_تعریف_شدہ CV_PROFILE_دھاگے
    کام_Submit_وقت = get_Tick_گنتی();
    دھاگے_stat[0].reset();
    دھاگے_stat[0].threadWait = کام_Submit_وقت;
    دھاگے_stat[0].threadWake = کام_Submit_وقت;
#اگر_ختم
    اگر (get_عدد_کا_دھاگے() > 1 &&
        کام == تہی &&
        (range.size() * nstripes >= 2 || (range.size() > 1 && nstripes <= 0))
    )
    {
        pthread_mutex_lock(&mutex);
        اگر (کام != تہی)
        {
            pthread_mutex_unlock(&mutex);
            body(range);
            واپس;
        }
        reconfigure_(عدد_دھاگے_2 - 1);

        {
            CV_LOG_VERBOSE(تہی, 1, "MainThread: initialize parallel job: " << range.size());
            کام = Ptr<Parallel_کام>(نیا Parallel_کام(*یہ, range, body, nstripes));
            pthread_mutex_unlock(&mutex);

            CV_LOG_VERBOSE(تہی, 5, "MainThread: wake worker threads...");
            size_t عدد_دھاگے_مقصد_wake = std::min(static_cast<size_t>(range.size()), دھاگے.size());
            برائے (size_t i = 0; i < عدد_دھاگے_مقصد_wake; ++i)
            {
                اگر (کام->current_task >= کام->range.size())
                    توڑو;
                کارکن_Thread& thread = *(دھاگے[i].get());
                اگر (
#اگر defined(__clang__) && defined(__has_feature)
#اگر __has_feature(thread_sanitizer)
                        1 ||  // Robust workaround to avoid data race warning of `thread.job`
#اگر_ختم
#اگر_ختم
#اگر !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
                        thread.is_فعال_2 ||
#اگر_ختم
                        thread.has_wake_signal
                        || !thread.کام.empty()  // #10881
                )
                {
                    pthread_mutex_lock(&thread.mutex);
                    thread.کام = کام;
#اگر !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
                    منطقی is_فعال_2 = thread.is_فعال_2;
#اگر_ختم
                    thread.has_wake_signal = سچ;
#اگر_تعریف_شدہ CV_PROFILE_دھاگے
                    دھاگے_stat[i + 1].reset();
#اگر_ختم
                    pthread_mutex_unlock(&thread.mutex);
#اگر !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
                    اگر (!is_فعال_2)
                    {
                        pthread_cond_broadcast/*pthread_cond_signal*/(&thread.cond_thread_wake); // wake thread
                    }
#اگر_ختم
                }
                ورنہ
                {
                    CV_Assert(thread.کام.empty());
                    thread.کام = کام;
                    thread.has_wake_signal = سچ;
#اگر_تعریف_شدہ CV_PROFILE_دھاگے
                    دھاگے_stat[i + 1].reset();
#اگر_ختم
#اگر !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
                    pthread_cond_broadcast/*pthread_cond_signal*/(&thread.cond_thread_wake); // wake thread
#اگر_ختم
                }
            }
#اگر_تعریف_شدہ CV_PROFILE_دھاگے
            دھاگے_stat[0].threadPing = get_Tick_گنتی();
#اگر_ختم
#اگر defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
            pthread_cond_broadcast(&cond_thread_wake); // wake all threads
#اگر_ختم
#اگر_تعریف_شدہ CV_PROFILE_دھاگے
            دھاگے_stat[0].threadWake = get_Tick_گنتی();
#اگر_ختم
            CV_LOG_VERBOSE(تہی, 5, "MainThread: wake worker threads... (done)");

            {
                Parallel_کام& j = *(یہ->کام);
#اگر_تعریف_شدہ CV_PROFILE_دھاگے
                دھاگے_stat[0].thread_Execute_آغاز = get_Tick_گنتی();
                دھاگے_stat[0].executedTasks = j.execute(جھوٹ);
                دھاگے_stat[0].threadExecuteStop = get_Tick_گنتی();
#ورنہ
                j.execute(جھوٹ);
#اگر_ختم
                CV_Assert(j.current_task >= j.range.size());
                CV_LOG_VERBOSE(تہی, 5, "MainThread: complete self-tasks: " << j.فعال_thread_گنتی << " " << j.completed_thread_گنتی);
                اگر (کام->is_completed || j.فعال_thread_گنتی == 0)
                {
                    کام->is_completed = سچ;
                    CV_LOG_VERBOSE(تہی, 5, "MainThread: no WIP worker threads");
                }
                ورنہ
                {
                    اگر (CV_MAIN_THREAD_فعال_WAIT > 0)
                    {
                        برائے (صحیح i = 0; i < CV_MAIN_THREAD_فعال_WAIT; i++)  // don't spin too much in any case (inaccurate getTickCount())
                        {
                            اگر (کام->is_completed)
                            {
                                CV_LOG_VERBOSE(تہی, 5, "MainThread: job finalize (active wait) " << j.فعال_thread_گنتی << " " << j.completed_thread_گنتی);
                                توڑو;
                            }
                            اگر (CV_فعال_WAIT_PAUSE_حد_بندی > 0 && (i < CV_فعال_WAIT_PAUSE_حد_بندی || (i & 1)))
                                CV_PAUSE(16);
                            ورنہ
                                CV_YIELD();
                        }
                    }
                    اگر (!کام->is_completed)
                    {
                        CV_LOG_VERBOSE(تہی, 5, "MainThread: prepare wait " << j.فعال_thread_گنتی << " " << j.completed_thread_گنتی);
                        pthread_mutex_lock(&mutex_notify);
                        برائے (;;)
                        {
                            اگر (کام->is_completed)
                            {
                                CV_LOG_VERBOSE(تہی, 5, "MainThread: job finalize (wait) " << j.فعال_thread_گنتی << " " << j.completed_thread_گنتی);
                                توڑو;
                            }
                            CV_LOG_VERBOSE(تہی, 5, "MainThread: wait completion (sleep) ...");
                            pthread_cond_wait(&cond_thread_task_complete, &mutex_notify);
                            CV_LOG_VERBOSE(تہی, 5, "MainThread: wake");
                        }
                        pthread_mutex_unlock(&mutex_notify);
                    }
                }
            }
#اگر_تعریف_شدہ CV_PROFILE_دھاگے
            دھاگے_stat[0].threadFree = get_Tick_گنتی();
            std::خارج << "Job: sz=" << range.size() << " nstripes=" << nstripes << "    Time: " << (دھاگے_stat[0].threadFree - کام_Submit_وقت) / tickFreq * 1e6 << " usec" << std::سطر_ختم;
            برائے (صحیح i = 0; i < (صحیح)دھاگے.size() + 1; i++)
            {
                دھاگے_stat[i].dump(i - 1, کام_Submit_وقت, tickFreq);
            }
#اگر_ختم
            اگر (کام)
            {
                pthread_mutex_lock(&mutex);
                CV_LOG_VERBOSE(تہی, 5, "MainThread: job release");
                CV_Assert(کام->is_completed);
                کام.release();
                pthread_mutex_unlock(&mutex);
            }
        }
    }
    ورنہ
    {
        body(range);
    }
}

size_t ThreadPool::get_عدد_کا_دھاگے()
{
    واپس عدد_دھاگے_2;
}

خالی ThreadPool::سیٹ_عدد_کا_دھاگے(غیرعلامتی n)
{
    اگر (n != عدد_دھاگے_2)
    {
        عدد_دھاگے_2 = n;
        اگر (n == 1)
           اگر (کام == تہی) reconfigure(0);  // stop worker threads immediately
    }
}

size_t parallel_pthreads_get_دھاگے_عدد()
{
    واپس ThreadPool::instance().get_عدد_کا_دھاگے();
}

خالی parallel_pthreads_سیٹ_دھاگے_عدد(صحیح num)
{
    اگر(num < 0)
    {
        ThreadPool::instance().سیٹ_عدد_کا_دھاگے(0);
    }
    ورنہ
    {
        ThreadPool::instance().سیٹ_عدد_کا_دھاگے(غیرعلامتی(num));
    }
}

خالی parallel_for_pthreads(ثابت Range& range, ثابت ParallelLoopBody& body, دوہرا nstripes)
{
    ThreadPool::instance().تسلسل(range, body, nstripes);
}

}

#اگر_ختم
