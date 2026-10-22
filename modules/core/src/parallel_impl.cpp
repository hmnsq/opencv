// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ضمن "precomp.hpp"

#ضمن "parallel_impl.hpp"

#جے_تعریف_شدہ HAVE_PTHREADS_PF
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
#جے_تعریف_شدہ DECLARE_CV_YIELD
DECLARE_CV_YIELD
#جے_انت
#جے_بنا_تعریف CV_YIELD
# include <thread>
# define CV_YIELD() std::this_thread::yield()
#جے_انت // CV_YIELD

// Spin lock's CPU-level yield (required for Hyper-Threading)
#جے_تعریف_شدہ DECLARE_CV_PAUSE
DECLARE_CV_PAUSE
#جے_انت
#جے_بنا_تعریف CV_PAUSE
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
#جے_انت // CV_PAUSE


نطاق cv
{

ساكن صحيح CV_سرگرم_WAIT_PAUSE_حد_بندی = (صحيح)utils::get_Configuration_Parameter_ناپ_T("OPENCV_THREAD_POOL_ACTIVE_WAIT_PAUSE_LIMIT", 16);  // iterations
ساكن صحيح CV_کامہ_سرگرم_WAIT = (صحيح)utils::get_Configuration_Parameter_ناپ_T("OPENCV_THREAD_POOL_ACTIVE_WAIT_WORKER", 2000);  // iterations
ساكن صحيح CV_MAIN_THREAD_سرگرم_WAIT = (صحيح)utils::get_Configuration_Parameter_ناپ_T("OPENCV_THREAD_POOL_ACTIVE_WAIT_MAIN", 10000); // iterations

ساكن صحيح CV_کامہ_سرگرم_WAIT_دھاگے_حد_بندی = (صحيح)utils::get_Configuration_Parameter_ناپ_T("OPENCV_THREAD_POOL_ACTIVE_WAIT_THREADS_LIMIT", 0); // number of real cores

صنف کامہ_Thread;
صنف Parallel_کم;

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

    فراغ reconfigure(بنانشانی new_دھاگے_گنتی)
    {
        إذا (new_دھاگے_گنتی == دھاگے.size())
            أرجع;
        pthread_mutex_lock(&mutex);
        reconfigure_(new_دھاگے_گنتی);
        pthread_mutex_unlock(&mutex);
    }
    منطقي reconfigure_(بنانشانی new_دھاگے_گنتی); // internal implementation

    فراغ لگاتار(ثابت Range& range, ثابت ParallelLoopBody& body, مزدوج nstripes);

    size_t get_عدد_دا_دھاگے();

    فراغ سیٹ_عدد_دا_دھاگے(بنانشانی n);

    ThreadPool();

    ~ThreadPool();

    بنانشانی عدد_دھاگے_2;

    pthread_mutex_t mutex;  // guards fields (job/threads) from non-worker threads (concurrent parallel_for calls)
#إذا defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
    pthread_cond_t cond_thread_wake;
#جے_انت

    pthread_mutex_t mutex_notify;
    pthread_cond_t cond_thread_task_complete;

    std::vector< Ptr<کامہ_Thread> > دھاگے;

    Ptr<Parallel_کم> کم_2;

#جے_تعریف_شدہ CV_PROFILE_دھاگے
    مزدوج tickFreq;
    int64 کم_Submit_ویلا;
    بنية ThreadStatistics
    {
        ThreadStatistics() : threadWait(0)
        {
            reset();
        }
        فراغ reset()
        {
            threadWake = 0;
            thread_Execute_شروع = 0;
            threadExecuteStop = 0;
            executedTasks = 0;
            keep_سرگرم = خطأ;
            threadPing = get_Tick_گنتی();
        }
        int64 threadWait; // don't reset by default
        int64 threadPing; // don't reset by default
        int64 threadWake;
        int64 thread_Execute_شروع;
        int64 threadExecuteStop;
        int64 threadFree;
        بنانشانی executedTasks;
        منطقي keep_سرگرم;

        int64 dummy_[8]; // separate cache lines

        فراغ dump(صحيح id, int64 ادھار_ویلا, مزدوج tickFreq)
        {
            إذا (id < 0)
                std::باہر << "Main: ";
            وإلا
                لکھو("T%03d: ", id + 2);
            لکھو("wait=% 10.1f   ping=% 6.1f",
                    threadWait > 0 ? (threadWait - ادھار_ویلا) / tickFreq * 1e6 : -0.0,
                    threadPing > 0 ? (threadPing - ادھار_ویلا) / tickFreq * 1e6 : -0.0);
            إذا (threadWake > 0)
                لکھو("   wake=% 6.1f",
                    (threadWake > 0 ? (threadWake - ادھار_ویلا) / tickFreq * 1e6 : -0.0));
            إذا (thread_Execute_شروع > 0)
            {
                لکھو("   exec=% 6.1f - % 6.1f   tasksDone=%5u   free=% 6.1f",
                    (thread_Execute_شروع > 0 ? (thread_Execute_شروع - ادھار_ویلا) / tickFreq * 1e6 : -0.0),
                    (threadExecuteStop > 0 ? (threadExecuteStop - ادھار_ویلا) / tickFreq * 1e6 : -0.0),
                    executedTasks,
                    (threadFree > 0 ? (threadFree - ادھار_ویلا) / tickFreq * 1e6 : -0.0));
                إذا (id >= 0)
                    لکھو(" active=%s\n", keep_سرگرم ? "true" : "false");
                وإلا
                    لکھو("\n");
            }
            وإلا
                لکھو("   ------------------------------------------------------------------------------\n");
        }
    };
    ThreadStatistics دھاگے_stat[CV_PROFILE_دھاگے]; // 0 - main thread, 1..N - worker threads
#جے_انت

};

صنف کامہ_Thread
{
عام:
    ThreadPool& thread_pool;
    ثابت بنانشانی id;
    pthread_t posix_thread;
    منطقي is_created;

    std::atomic<منطقي> stop_thread;

    std::atomic<منطقي> has_wake_signal;

    Ptr<Parallel_کم> کم_2;

    pthread_mutex_t mutex;
#إذا !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
    متطاير منطقي is_سرگرم_2;
    pthread_cond_t cond_thread_wake;
#جے_انت

    کامہ_Thread(ThreadPool& thread_pool_, بنانشانی id_) :
        thread_pool(thread_pool_),
        id(id_),
        posix_thread(0),
        is_created(خطأ),
        stop_thread(خطأ),
        has_wake_signal(خطأ)
#إذا !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
        , is_سرگرم_2(سچ)
#جے_انت
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
#جے_انت
        res = pthread_create(&posix_thread, عدم, thread_loop_wrapper, (فراغ*)هذا);
        إذا (res != 0)
        {
            CV_LOG_ERROR(عدم, id << ": Can't spawn new thread: res = " << res);
        }
        وإلا
        {
            is_created = سچ;
        }
    }

    ~کامہ_Thread()
    {
        CV_LOG_VERBOSE(عدم, 1, "MainThread: destroy worker thread: " << id);
        إذا (is_created)
        {
            إذا (!stop_thread)
            {
                pthread_mutex_lock(&mutex);  // to avoid signal miss due pre-check
                stop_thread = سچ;
                pthread_mutex_unlock(&mutex);
#إذا defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
                pthread_cond_broadcast(&thread_pool.cond_thread_wake);
#وإلا
                pthread_cond_signal(&cond_thread_wake);
#جے_انت
            }
            pthread_join(posix_thread, عدم);
        }
#إذا !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
        pthread_cond_destroy(&cond_thread_wake);
#جے_انت
        pthread_mutex_destroy(&mutex);
    }

    فراغ thread_body();
    ساكن فراغ* thread_loop_wrapper(فراغ* thread_object)
    {
#جے_تعریف_شدہ OPENCV_WITH_ITT
        __itt_thread_set_name(cv::format("OpenCVThread-%03d", cv::utils::getThreadID()).c_str());
#جے_انت
        ((کامہ_Thread*)thread_object)->thread_body();
        أرجع 0;
    }
};

صنف Parallel_کم
{
عام:
    Parallel_کم(ثابت ThreadPool& thread_pool_, ثابت Range& range_, ثابت ParallelLoopBody& body_, صحيح nstripes_) :
        thread_pool(thread_pool_),
        body(body_),
        range(range_),
        nstripes((بنانشانی)nstripes_),
        is_completed(خطأ)
    {
        CV_LOG_VERBOSE(عدم, 5, "ParallelJob::ParallelJob(" << (فراغ*)هذا << ")");
        current_task.store(0, std::memory_order_relaxed);
        سرگرم_thread_گنتی.store(0, std::memory_order_relaxed);
        completed_thread_گنتی.store(0, std::memory_order_relaxed);
        dummy0_[0] = 0, dummy1_[0] = 0, dummy2_[0] = 0; // compiler warning
    }

    ~Parallel_کم()
    {
        CV_LOG_VERBOSE(عدم, 5, "ParallelJob::~ParallelJob(" << (فراغ*)هذا << ")");
    }

    بنانشانی execute(منطقي is_کامہ_thread)
    {
        بنانشانی executed_tasks = 0;
        ثابت صحيح task_گنتی = range.size();
        ثابت صحيح بچیا_multiplier = std::min(nstripes,
                std::max(
                        std::min(100u, thread_pool.عدد_دھاگے_2 * 4),
                        thread_pool.عدد_دھاگے_2 * 2
                ));  // experimental value
        لكل (;;)
        {
            صحيح chunk_ناپ = std::max(1, (task_گنتی - current_task) / بچیا_multiplier);
            صحيح id = current_task.fetch_add(chunk_ناپ, std::memory_order_seq_cst);
            إذا (id >= task_گنتی)
                اكسر; // no more free tasks

            executed_tasks += chunk_ناپ;
            صحيح شروع_id = id;
            صحيح سرا_id = std::min(task_گنتی, id + chunk_ناپ);
            CV_LOG_VERBOSE(عدم, 9, "Thread: job " << شروع_id << "-" << سرا_id);

            //TODO: if (not pending exception)
            {
                body.عامل()(Range(range.شروع + شروع_id, range.شروع + سرا_id));
            }
            إذا (is_کامہ_thread && is_completed)
            {
                CV_LOG_ERROR(عدم, "\t\t\t\tBUG! Job: " << (فراغ*)هذا << " " << id << " " << سرگرم_thread_گنتی << " " << completed_thread_گنتی);
                CV_Assert(!is_completed); // TODO Dbg this
            }
        }
        أرجع executed_tasks;
    }

    ثابت ThreadPool& thread_pool;
    ثابت ParallelLoopBody& body;
    ثابت Range range;
    ثابت بنانشانی nstripes;

    std::atomic<صحيح> current_task;  // next free part of job
    int64 dummy0_[8];  // avoid cache-line reusing for the same atomics

    std::atomic<صحيح> سرگرم_thread_گنتی;  // number of threads worked on this job
    int64 dummy1_[8];  // avoid cache-line reusing for the same atomics

    std::atomic<صحيح> completed_thread_گنتی;  // number of threads completed any activities on this job
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
#جے_انت
#جے_انت
فراغ کامہ_Thread::thread_body()
{
    (فراغ)cv::utils::getThreadID(); // notify OpenCV about new thread
    CV_LOG_VERBOSE(عدم, 5, "Thread: new thread: " << id);

    منطقي allow_سرگرم_wait = سچ;

#جے_تعریف_شدہ CV_PROFILE_دھاگے
    ThreadPool::ThreadStatistics& stat = thread_pool.دھاگے_stat[id + 1];
#جے_انت

    طالما (!stop_thread)
    {
        CV_LOG_VERBOSE(عدم, 5, "Thread: ... loop iteration: allow_active_wait=" << allow_سرگرم_wait << "   has_wake_signal=" << has_wake_signal);
        إذا (allow_سرگرم_wait && CV_کامہ_سرگرم_WAIT > 0)
        {
            allow_سرگرم_wait = خطأ;
            لكل (صحيح i = 0; i < CV_کامہ_سرگرم_WAIT; i++)
            {
                إذا (has_wake_signal)
                    اكسر;
                إذا (CV_سرگرم_WAIT_PAUSE_حد_بندی > 0 && (i < CV_سرگرم_WAIT_PAUSE_حد_بندی || (i & 1)))
                    CV_PAUSE(16);
                وإلا
                    CV_YIELD();
            }
        }
        pthread_mutex_lock(&mutex);
#جے_تعریف_شدہ CV_PROFILE_دھاگے
        stat.threadWait = get_Tick_گنتی();
#جے_انت
        طالما (!has_wake_signal) // to handle spurious wakeups
        {
            //CV_LOG_VERBOSE(NULL, 5, "Thread: wait (sleep) ...");
#إذا defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
            pthread_cond_wait(&thread_pool.cond_thread_wake, &mutex);
#وإلا
            is_سرگرم_2 = خطأ;
            pthread_cond_wait(&cond_thread_wake, &mutex);
            is_سرگرم_2 = سچ;
#جے_انت
            CV_LOG_VERBOSE(عدم, 5, "Thread: wake ... (has_wake_signal=" << has_wake_signal << " stop_thread=" << stop_thread << ")")
        }
#جے_تعریف_شدہ CV_PROFILE_دھاگے
        stat.threadWake = get_Tick_گنتی();
#جے_انت

        CV_LOG_VERBOSE(عدم, 5, "Thread: checking for new job");
        إذا (CV_کامہ_سرگرم_WAIT_دھاگے_حد_بندی == 0)
            allow_سرگرم_wait = سچ;
        Ptr<Parallel_کم> j_ptr; swap(j_ptr, کم_2);
        has_wake_signal = خطأ;
        pthread_mutex_unlock(&mutex);

        إذا (!stop_thread)
        {
            Parallel_کم* j = j_ptr;
            إذا (j)
            {
                CV_LOG_VERBOSE(عدم, 5, "Thread: job size=" << j->range.size() << " done=" << j->current_task);
                إذا (j->current_task < j->range.size())
                {
                    صحيح other = j->سرگرم_thread_گنتی.fetch_add(1, std::memory_order_seq_cst);
                    CV_LOG_VERBOSE(عدم, 5, "Thread: processing new job (with " << other << " other threads)"); CV_بن_ورتیا(other);
#جے_تعریف_شدہ CV_PROFILE_دھاگے
                    stat.thread_Execute_شروع = get_Tick_گنتی();
                    stat.executedTasks = j->execute(سچ);
                    stat.threadExecuteStop = get_Tick_گنتی();
#وإلا
                    j->execute(سچ);
#جے_انت
                    صحيح completed = j->completed_thread_گنتی.fetch_add(1, std::memory_order_seq_cst) + 1;
                    صحيح سرگرم = j->سرگرم_thread_گنتی.load(std::memory_order_acquire);
                    إذا (CV_کامہ_سرگرم_WAIT_دھاگے_حد_بندی > 0)
                    {
                        allow_سرگرم_wait = سچ;
                        إذا (سرگرم >= CV_کامہ_سرگرم_WAIT_دھاگے_حد_بندی && (id & 1) == 0) // turn off a half of threads
                            allow_سرگرم_wait = خطأ;
                    }
                    CV_LOG_VERBOSE(عدم, 5, "Thread: completed job processing: " << سرگرم << " " << completed);
                    إذا (سرگرم == completed)
                    {
                        منطقي need_signal = !j->is_completed;
                        j->is_completed = سچ;
                        j = عدم; j_ptr.release();
                        إذا (need_signal)
                        {
                            CV_LOG_VERBOSE(عدم, 5, "Thread: job finished => notifying the main thread");
                            pthread_mutex_lock(&thread_pool.mutex_notify);  // to avoid signal miss due pre-check condition
                            //خالی_تھاواں
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
#جے_تعریف_شدہ CV_PROFILE_دھاگے
        stat.threadFree = get_Tick_گنتی();
        stat.keep_سرگرم = allow_سرگرم_wait;
#جے_انت
    }
}

ThreadPool::ThreadPool()
{
#جے_تعریف_شدہ CV_PROFILE_دھاگے
    tickFreq = getTickFrequency();
#جے_انت

    صحيح res = 0;
    res |= pthread_mutex_init(&mutex, عدم);
    res |= pthread_mutex_init(&mutex_notify, عدم);
#إذا defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
    res |= pthread_cond_init(&cond_thread_wake, عدم);
#جے_انت
    res |= pthread_cond_init(&cond_thread_task_complete, عدم);

    إذا (0 != res)
    {
        CV_LOG_FATAL(عدم, "Failed to initialize ThreadPool (pthreads)");
    }
    عدد_دھاگے_2 = default_نمبر_دا_دھاگے();
}

منطقي ThreadPool::reconfigure_(بنانشانی new_دھاگے_گنتی)
{
    إذا (new_دھاگے_گنتی == دھاگے.size())
        أرجع خطأ;

    إذا (new_دھاگے_گنتی < دھاگے.size())
    {
        CV_LOG_VERBOSE(عدم, 1, "MainThread: reduce worker pool: " << دھاگے.size() << " => " << new_دھاگے_گنتی);
        std::vector< Ptr<کامہ_Thread> > release_دھاگے(دھاگے.size() - new_دھاگے_گنتی);
        لكل (size_t i = new_دھاگے_گنتی; i < دھاگے.size(); ++i)
        {
            pthread_mutex_lock(&دھاگے[i]->mutex);  // to avoid signal miss due pre-check
            دھاگے[i]->stop_thread = سچ;
            دھاگے[i]->has_wake_signal = سچ;
#إذا !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
            pthread_mutex_unlock(&دھاگے[i]->mutex);
            pthread_cond_broadcast/*pthread_cond_signal*/(&دھاگے[i]->cond_thread_wake); // wake thread
#وإلا
            pthread_mutex_unlock(&دھاگے[i]->mutex);
#جے_انت
            std::swap(دھاگے[i], release_دھاگے[i - new_دھاگے_گنتی]);
        }
#إذا defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
        CV_LOG_VERBOSE(عدم, 1, "MainThread: notify worker threads about termination...");
        pthread_cond_broadcast(&cond_thread_wake); // wake all threads
#جے_انت
        دھاگے.resize(new_دھاگے_گنتی);
        release_دھاگے.clear();  // calls thread_join which want to lock mutex
        أرجع خطأ;
    }
    وإلا
    {
        CV_LOG_VERBOSE(عدم, 1, "MainThread: upgrade worker pool: " << دھاگے.size() << " => " << new_دھاگے_گنتی);
        لكل (size_t i = دھاگے.size(); i < new_دھاگے_گنتی; ++i)
        {
            دھاگے.push_back(Ptr<کامہ_Thread>(جديد کامہ_Thread(*هذا, (بنانشانی)i))); // spawn more threads
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
#جے_انت
    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&mutex_notify);
}

فراغ ThreadPool::لگاتار(ثابت Range& range, ثابت ParallelLoopBody& body, مزدوج nstripes)
{
    CV_LOG_VERBOSE(عدم, 1, "MainThread: new parallel job: num_threads=" << عدد_دھاگے_2 << "   range=" << range.size() << "   nstripes=" << nstripes << "   job=" << (فراغ*)کم_2);
#جے_تعریف_شدہ CV_PROFILE_دھاگے
    کم_Submit_ویلا = get_Tick_گنتی();
    دھاگے_stat[0].reset();
    دھاگے_stat[0].threadWait = کم_Submit_ویلا;
    دھاگے_stat[0].threadWake = کم_Submit_ویلا;
#جے_انت
    إذا (get_عدد_دا_دھاگے() > 1 &&
        کم_2 == عدم &&
        (range.size() * nstripes >= 2 || (range.size() > 1 && nstripes <= 0))
    )
    {
        pthread_mutex_lock(&mutex);
        إذا (کم_2 != عدم)
        {
            pthread_mutex_unlock(&mutex);
            body(range);
            أرجع;
        }
        reconfigure_(عدد_دھاگے_2 - 1);

        {
            CV_LOG_VERBOSE(عدم, 1, "MainThread: initialize parallel job: " << range.size());
            کم_2 = Ptr<Parallel_کم>(جديد Parallel_کم(*هذا, range, body, nstripes));
            pthread_mutex_unlock(&mutex);

            CV_LOG_VERBOSE(عدم, 5, "MainThread: wake worker threads...");
            size_t عدد_دھاگے_منزل_wake = std::min(static_cast<size_t>(range.size()), دھاگے.size());
            لكل (size_t i = 0; i < عدد_دھاگے_منزل_wake; ++i)
            {
                إذا (کم_2->current_task >= کم_2->range.size())
                    اكسر;
                کامہ_Thread& thread = *(دھاگے[i].get());
                إذا (
#إذا defined(__clang__) && defined(__has_feature)
#إذا __has_feature(thread_sanitizer)
                        1 ||  // Robust workaround to avoid data race warning of `thread.job`
#جے_انت
#جے_انت
#إذا !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
                        thread.is_سرگرم_2 ||
#جے_انت
                        thread.has_wake_signal
                        || !thread.کم_2.empty()  // #10881
                )
                {
                    pthread_mutex_lock(&thread.mutex);
                    thread.کم_2 = کم_2;
#إذا !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
                    منطقي is_سرگرم_2 = thread.is_سرگرم_2;
#جے_انت
                    thread.has_wake_signal = سچ;
#جے_تعریف_شدہ CV_PROFILE_دھاگے
                    دھاگے_stat[i + 1].reset();
#جے_انت
                    pthread_mutex_unlock(&thread.mutex);
#إذا !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
                    إذا (!is_سرگرم_2)
                    {
                        pthread_cond_broadcast/*pthread_cond_signal*/(&thread.cond_thread_wake); // wake thread
                    }
#جے_انت
                }
                وإلا
                {
                    CV_Assert(thread.کم_2.empty());
                    thread.کم_2 = کم_2;
                    thread.has_wake_signal = سچ;
#جے_تعریف_شدہ CV_PROFILE_دھاگے
                    دھاگے_stat[i + 1].reset();
#جے_انت
#إذا !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
                    pthread_cond_broadcast/*pthread_cond_signal*/(&thread.cond_thread_wake); // wake thread
#جے_انت
                }
            }
#جے_تعریف_شدہ CV_PROFILE_دھاگے
            دھاگے_stat[0].threadPing = get_Tick_گنتی();
#جے_انت
#إذا defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
            pthread_cond_broadcast(&cond_thread_wake); // wake all threads
#جے_انت
#جے_تعریف_شدہ CV_PROFILE_دھاگے
            دھاگے_stat[0].threadWake = get_Tick_گنتی();
#جے_انت
            CV_LOG_VERBOSE(عدم, 5, "MainThread: wake worker threads... (done)");

            {
                Parallel_کم& j = *(هذا->کم_2);
#جے_تعریف_شدہ CV_PROFILE_دھاگے
                دھاگے_stat[0].thread_Execute_شروع = get_Tick_گنتی();
                دھاگے_stat[0].executedTasks = j.execute(خطأ);
                دھاگے_stat[0].threadExecuteStop = get_Tick_گنتی();
#وإلا
                j.execute(خطأ);
#جے_انت
                CV_Assert(j.current_task >= j.range.size());
                CV_LOG_VERBOSE(عدم, 5, "MainThread: complete self-tasks: " << j.سرگرم_thread_گنتی << " " << j.completed_thread_گنتی);
                إذا (کم_2->is_completed || j.سرگرم_thread_گنتی == 0)
                {
                    کم_2->is_completed = سچ;
                    CV_LOG_VERBOSE(عدم, 5, "MainThread: no WIP worker threads");
                }
                وإلا
                {
                    إذا (CV_MAIN_THREAD_سرگرم_WAIT > 0)
                    {
                        لكل (صحيح i = 0; i < CV_MAIN_THREAD_سرگرم_WAIT; i++)  // don't spin too much in any case (inaccurate getTickCount())
                        {
                            إذا (کم_2->is_completed)
                            {
                                CV_LOG_VERBOSE(عدم, 5, "MainThread: job finalize (active wait) " << j.سرگرم_thread_گنتی << " " << j.completed_thread_گنتی);
                                اكسر;
                            }
                            إذا (CV_سرگرم_WAIT_PAUSE_حد_بندی > 0 && (i < CV_سرگرم_WAIT_PAUSE_حد_بندی || (i & 1)))
                                CV_PAUSE(16);
                            وإلا
                                CV_YIELD();
                        }
                    }
                    إذا (!کم_2->is_completed)
                    {
                        CV_LOG_VERBOSE(عدم, 5, "MainThread: prepare wait " << j.سرگرم_thread_گنتی << " " << j.completed_thread_گنتی);
                        pthread_mutex_lock(&mutex_notify);
                        لكل (;;)
                        {
                            إذا (کم_2->is_completed)
                            {
                                CV_LOG_VERBOSE(عدم, 5, "MainThread: job finalize (wait) " << j.سرگرم_thread_گنتی << " " << j.completed_thread_گنتی);
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
#جے_تعریف_شدہ CV_PROFILE_دھاگے
            دھاگے_stat[0].threadFree = get_Tick_گنتی();
            std::باہر << "Job: sz=" << range.size() << " nstripes=" << nstripes << "    Time: " << (دھاگے_stat[0].threadFree - کم_Submit_ویلا) / tickFreq * 1e6 << " usec" << std::سطر_انت;
            لكل (صحيح i = 0; i < (صحيح)دھاگے.size() + 1; i++)
            {
                دھاگے_stat[i].dump(i - 1, کم_Submit_ویلا, tickFreq);
            }
#جے_انت
            إذا (کم_2)
            {
                pthread_mutex_lock(&mutex);
                CV_LOG_VERBOSE(عدم, 5, "MainThread: job release");
                CV_Assert(کم_2->is_completed);
                کم_2.release();
                pthread_mutex_unlock(&mutex);
            }
        }
    }
    وإلا
    {
        body(range);
    }
}

size_t ThreadPool::get_عدد_دا_دھاگے()
{
    أرجع عدد_دھاگے_2;
}

فراغ ThreadPool::سیٹ_عدد_دا_دھاگے(بنانشانی n)
{
    إذا (n != عدد_دھاگے_2)
    {
        عدد_دھاگے_2 = n;
        إذا (n == 1)
           إذا (کم_2 == عدم) reconfigure(0);  // stop worker threads immediately
    }
}

size_t parallel_pthreads_get_دھاگے_عدد()
{
    أرجع ThreadPool::instance().get_عدد_دا_دھاگے();
}

فراغ parallel_pthreads_سیٹ_دھاگے_عدد(صحيح num)
{
    إذا(num < 0)
    {
        ThreadPool::instance().سیٹ_عدد_دا_دھاگے(0);
    }
    وإلا
    {
        ThreadPool::instance().سیٹ_عدد_دا_دھاگے(بنانشانی(num));
    }
}

فراغ parallel_for_pthreads(ثابت Range& range, ثابت ParallelLoopBody& body, مزدوج nstripes)
{
    ThreadPool::instance().لگاتار(range, body, nstripes);
}

}

#جے_انت
