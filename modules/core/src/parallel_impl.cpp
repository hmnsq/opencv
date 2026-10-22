// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ضمن "precomp.hpp"

#ضمن "parallel_impl.hpp"

#جيڪڏهن_وصف_ٿيل HAVE_PTHREADS_PF
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
#جيڪڏهن_وصف_ٿيل DECLARE_CV_YIELD
DECLARE_CV_YIELD
#جيڪڏهن_پورو
#جيڪڏهن_وصف_نه_ٿيل CV_YIELD
# include <thread>
# define CV_YIELD() std::this_thread::yield()
#جيڪڏهن_پورو // CV_YIELD

// Spin lock's CPU-level yield (required for Hyper-Threading)
#جيڪڏهن_وصف_ٿيل DECLARE_CV_PAUSE
DECLARE_CV_PAUSE
#جيڪڏهن_پورو
#جيڪڏهن_وصف_نه_ٿيل CV_PAUSE
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
#جيڪڏهن_پورو // CV_PAUSE


نطاق cv
{

ساكن صحيح CV_سرگرم_WAIT_PAUSE_حد_بندي = (صحيح)utils::get_Configuration_Parameter_سائيز_T("OPENCV_THREAD_POOL_ACTIVE_WAIT_PAUSE_LIMIT", 16);  // iterations
ساكن صحيح CV_ڪم_ڪندڙ_سرگرم_WAIT = (صحيح)utils::get_Configuration_Parameter_سائيز_T("OPENCV_THREAD_POOL_ACTIVE_WAIT_WORKER", 2000);  // iterations
ساكن صحيح CV_MAIN_THREAD_سرگرم_WAIT = (صحيح)utils::get_Configuration_Parameter_سائيز_T("OPENCV_THREAD_POOL_ACTIVE_WAIT_MAIN", 10000); // iterations

ساكن صحيح CV_ڪم_ڪندڙ_سرگرم_WAIT_ڌاڳا_حد_بندي = (صحيح)utils::get_Configuration_Parameter_سائيز_T("OPENCV_THREAD_POOL_ACTIVE_WAIT_THREADS_LIMIT", 0); // number of real cores

صنف ڪم_ڪندڙ_Thread;
صنف Parallel_ڪم;

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

    فراغ reconfigure(بنانشان new_ڌاڳا_ڳڻپ)
    {
        إذا (new_ڌاڳا_ڳڻپ == ڌاڳا.size())
            أرجع;
        pthread_mutex_lock(&mutex);
        reconfigure_(new_ڌاڳا_ڳڻپ);
        pthread_mutex_unlock(&mutex);
    }
    منطقي reconfigure_(بنانشان new_ڌاڳا_ڳڻپ); // internal implementation

    فراغ لاڳيتو(ثابت Range& range, ثابت ParallelLoopBody& body, مزدوج nstripes);

    size_t get_انگ_جو_ڌاڳا();

    فراغ سيٽ_انگ_جو_ڌاڳا(بنانشان n);

    ThreadPool();

    ~ThreadPool();

    بنانشان انگ_ڌاڳا_2;

    pthread_mutex_t mutex;  // guards fields (job/threads) from non-worker threads (concurrent parallel_for calls)
#إذا defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
    pthread_cond_t cond_thread_wake;
#جيڪڏهن_پورو

    pthread_mutex_t mutex_notify;
    pthread_cond_t cond_thread_task_complete;

    std::vector< Ptr<ڪم_ڪندڙ_Thread> > ڌاڳا;

    Ptr<Parallel_ڪم> ڪم;

#جيڪڏهن_وصف_ٿيل CV_PROFILE_ڌاڳا
    مزدوج tickFreq;
    int64 ڪم_Submit_وقت;
    بنية ThreadStatistics
    {
        ThreadStatistics() : threadWait(0)
        {
            reset();
        }
        فراغ reset()
        {
            threadWake = 0;
            thread_Execute_شروعات = 0;
            threadExecuteStop = 0;
            executedTasks = 0;
            keep_سرگرم = خطأ;
            threadPing = get_Tick_ڳڻپ();
        }
        int64 threadWait; // don't reset by default
        int64 threadPing; // don't reset by default
        int64 threadWake;
        int64 thread_Execute_شروعات;
        int64 threadExecuteStop;
        int64 threadFree;
        بنانشان executedTasks;
        منطقي keep_سرگرم;

        int64 dummy_[8]; // separate cache lines

        فراغ dump(صحيح id, int64 بنياد_وقت, مزدوج tickFreq)
        {
            إذا (id < 0)
                std::ٻاهر << "Main: ";
            وإلا
                لکو("T%03d: ", id + 2);
            لکو("wait=% 10.1f   ping=% 6.1f",
                    threadWait > 0 ? (threadWait - بنياد_وقت) / tickFreq * 1e6 : -0.0,
                    threadPing > 0 ? (threadPing - بنياد_وقت) / tickFreq * 1e6 : -0.0);
            إذا (threadWake > 0)
                لکو("   wake=% 6.1f",
                    (threadWake > 0 ? (threadWake - بنياد_وقت) / tickFreq * 1e6 : -0.0));
            إذا (thread_Execute_شروعات > 0)
            {
                لکو("   exec=% 6.1f - % 6.1f   tasksDone=%5u   free=% 6.1f",
                    (thread_Execute_شروعات > 0 ? (thread_Execute_شروعات - بنياد_وقت) / tickFreq * 1e6 : -0.0),
                    (threadExecuteStop > 0 ? (threadExecuteStop - بنياد_وقت) / tickFreq * 1e6 : -0.0),
                    executedTasks,
                    (threadFree > 0 ? (threadFree - بنياد_وقت) / tickFreq * 1e6 : -0.0));
                إذا (id >= 0)
                    لکو(" active=%s\n", keep_سرگرم ? "true" : "false");
                وإلا
                    لکو("\n");
            }
            وإلا
                لکو("   ------------------------------------------------------------------------------\n");
        }
    };
    ThreadStatistics ڌاڳا_stat[CV_PROFILE_ڌاڳا]; // 0 - main thread, 1..N - worker threads
#جيڪڏهن_پورو

};

صنف ڪم_ڪندڙ_Thread
{
عام:
    ThreadPool& thread_pool;
    ثابت بنانشان id;
    pthread_t posix_thread;
    منطقي is_created;

    std::atomic<منطقي> stop_thread;

    std::atomic<منطقي> has_wake_signal;

    Ptr<Parallel_ڪم> ڪم;

    pthread_mutex_t mutex;
#إذا !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
    متطاير منطقي is_سرگرم_2;
    pthread_cond_t cond_thread_wake;
#جيڪڏهن_پورو

    ڪم_ڪندڙ_Thread(ThreadPool& thread_pool_, بنانشان id_) :
        thread_pool(thread_pool_),
        id(id_),
        posix_thread(0),
        is_created(خطأ),
        stop_thread(خطأ),
        has_wake_signal(خطأ)
#إذا !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
        , is_سرگرم_2(سچ)
#جيڪڏهن_پورو
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
#جيڪڏهن_پورو
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

    ~ڪم_ڪندڙ_Thread()
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
#جيڪڏهن_پورو
            }
            pthread_join(posix_thread, عدم);
        }
#إذا !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
        pthread_cond_destroy(&cond_thread_wake);
#جيڪڏهن_پورو
        pthread_mutex_destroy(&mutex);
    }

    فراغ thread_body();
    ساكن فراغ* thread_loop_wrapper(فراغ* thread_object)
    {
#جيڪڏهن_وصف_ٿيل OPENCV_WITH_ITT
        __itt_thread_set_name(cv::format("OpenCVThread-%03d", cv::utils::getThreadID()).c_str());
#جيڪڏهن_پورو
        ((ڪم_ڪندڙ_Thread*)thread_object)->thread_body();
        أرجع 0;
    }
};

صنف Parallel_ڪم
{
عام:
    Parallel_ڪم(ثابت ThreadPool& thread_pool_, ثابت Range& range_, ثابت ParallelLoopBody& body_, صحيح nstripes_) :
        thread_pool(thread_pool_),
        body(body_),
        range(range_),
        nstripes((بنانشان)nstripes_),
        is_completed(خطأ)
    {
        CV_LOG_VERBOSE(عدم, 5, "ParallelJob::ParallelJob(" << (فراغ*)هذا << ")");
        current_task.store(0, std::memory_order_relaxed);
        سرگرم_thread_ڳڻپ.store(0, std::memory_order_relaxed);
        completed_thread_ڳڻپ.store(0, std::memory_order_relaxed);
        dummy0_[0] = 0, dummy1_[0] = 0, dummy2_[0] = 0; // compiler warning
    }

    ~Parallel_ڪم()
    {
        CV_LOG_VERBOSE(عدم, 5, "ParallelJob::~ParallelJob(" << (فراغ*)هذا << ")");
    }

    بنانشان execute(منطقي is_ڪم_ڪندڙ_thread)
    {
        بنانشان executed_tasks = 0;
        ثابت صحيح task_ڳڻپ = range.size();
        ثابت صحيح بچيل_multiplier = std::min(nstripes,
                std::max(
                        std::min(100u, thread_pool.انگ_ڌاڳا_2 * 4),
                        thread_pool.انگ_ڌاڳا_2 * 2
                ));  // experimental value
        لكل (;;)
        {
            صحيح chunk_سائيز = std::max(1, (task_ڳڻپ - current_task) / بچيل_multiplier);
            صحيح id = current_task.fetch_add(chunk_سائيز, std::memory_order_seq_cst);
            إذا (id >= task_ڳڻپ)
                اكسر; // no more free tasks

            executed_tasks += chunk_سائيز;
            صحيح شروعات_id = id;
            صحيح پڇاڙي_id = std::min(task_ڳڻپ, id + chunk_سائيز);
            CV_LOG_VERBOSE(عدم, 9, "Thread: job " << شروعات_id << "-" << پڇاڙي_id);

            //TODO: if (not pending exception)
            {
                body.عامل()(Range(range.شروعات + شروعات_id, range.شروعات + پڇاڙي_id));
            }
            إذا (is_ڪم_ڪندڙ_thread && is_completed)
            {
                CV_LOG_ERROR(عدم, "\t\t\t\tBUG! Job: " << (فراغ*)هذا << " " << id << " " << سرگرم_thread_ڳڻپ << " " << completed_thread_ڳڻپ);
                CV_Assert(!is_completed); // TODO Dbg this
            }
        }
        أرجع executed_tasks;
    }

    ثابت ThreadPool& thread_pool;
    ثابت ParallelLoopBody& body;
    ثابت Range range;
    ثابت بنانشان nstripes;

    std::atomic<صحيح> current_task;  // next free part of job
    int64 dummy0_[8];  // avoid cache-line reusing for the same atomics

    std::atomic<صحيح> سرگرم_thread_ڳڻپ;  // number of threads worked on this job
    int64 dummy1_[8];  // avoid cache-line reusing for the same atomics

    std::atomic<صحيح> completed_thread_ڳڻپ;  // number of threads completed any activities on this job
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
#جيڪڏهن_پورو
#جيڪڏهن_پورو
فراغ ڪم_ڪندڙ_Thread::thread_body()
{
    (فراغ)cv::utils::getThreadID(); // notify OpenCV about new thread
    CV_LOG_VERBOSE(عدم, 5, "Thread: new thread: " << id);

    منطقي allow_سرگرم_wait = سچ;

#جيڪڏهن_وصف_ٿيل CV_PROFILE_ڌاڳا
    ThreadPool::ThreadStatistics& stat = thread_pool.ڌاڳا_stat[id + 1];
#جيڪڏهن_پورو

    طالما (!stop_thread)
    {
        CV_LOG_VERBOSE(عدم, 5, "Thread: ... loop iteration: allow_active_wait=" << allow_سرگرم_wait << "   has_wake_signal=" << has_wake_signal);
        إذا (allow_سرگرم_wait && CV_ڪم_ڪندڙ_سرگرم_WAIT > 0)
        {
            allow_سرگرم_wait = خطأ;
            لكل (صحيح i = 0; i < CV_ڪم_ڪندڙ_سرگرم_WAIT; i++)
            {
                إذا (has_wake_signal)
                    اكسر;
                إذا (CV_سرگرم_WAIT_PAUSE_حد_بندي > 0 && (i < CV_سرگرم_WAIT_PAUSE_حد_بندي || (i & 1)))
                    CV_PAUSE(16);
                وإلا
                    CV_YIELD();
            }
        }
        pthread_mutex_lock(&mutex);
#جيڪڏهن_وصف_ٿيل CV_PROFILE_ڌاڳا
        stat.threadWait = get_Tick_ڳڻپ();
#جيڪڏهن_پورو
        طالما (!has_wake_signal) // to handle spurious wakeups
        {
            //CV_LOG_VERBOSE(NULL, 5, "Thread: wait (sleep) ...");
#إذا defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
            pthread_cond_wait(&thread_pool.cond_thread_wake, &mutex);
#وإلا
            is_سرگرم_2 = خطأ;
            pthread_cond_wait(&cond_thread_wake, &mutex);
            is_سرگرم_2 = سچ;
#جيڪڏهن_پورو
            CV_LOG_VERBOSE(عدم, 5, "Thread: wake ... (has_wake_signal=" << has_wake_signal << " stop_thread=" << stop_thread << ")")
        }
#جيڪڏهن_وصف_ٿيل CV_PROFILE_ڌاڳا
        stat.threadWake = get_Tick_ڳڻپ();
#جيڪڏهن_پورو

        CV_LOG_VERBOSE(عدم, 5, "Thread: checking for new job");
        إذا (CV_ڪم_ڪندڙ_سرگرم_WAIT_ڌاڳا_حد_بندي == 0)
            allow_سرگرم_wait = سچ;
        Ptr<Parallel_ڪم> j_ptr; swap(j_ptr, ڪم);
        has_wake_signal = خطأ;
        pthread_mutex_unlock(&mutex);

        إذا (!stop_thread)
        {
            Parallel_ڪم* j = j_ptr;
            إذا (j)
            {
                CV_LOG_VERBOSE(عدم, 5, "Thread: job size=" << j->range.size() << " done=" << j->current_task);
                إذا (j->current_task < j->range.size())
                {
                    صحيح other = j->سرگرم_thread_ڳڻپ.fetch_add(1, std::memory_order_seq_cst);
                    CV_LOG_VERBOSE(عدم, 5, "Thread: processing new job (with " << other << " other threads)"); CV_اڻ_استعمال_ٿيل(other);
#جيڪڏهن_وصف_ٿيل CV_PROFILE_ڌاڳا
                    stat.thread_Execute_شروعات = get_Tick_ڳڻپ();
                    stat.executedTasks = j->execute(سچ);
                    stat.threadExecuteStop = get_Tick_ڳڻپ();
#وإلا
                    j->execute(سچ);
#جيڪڏهن_پورو
                    صحيح completed = j->completed_thread_ڳڻپ.fetch_add(1, std::memory_order_seq_cst) + 1;
                    صحيح سرگرم = j->سرگرم_thread_ڳڻپ.load(std::memory_order_acquire);
                    إذا (CV_ڪم_ڪندڙ_سرگرم_WAIT_ڌاڳا_حد_بندي > 0)
                    {
                        allow_سرگرم_wait = سچ;
                        إذا (سرگرم >= CV_ڪم_ڪندڙ_سرگرم_WAIT_ڌاڳا_حد_بندي && (id & 1) == 0) // turn off a half of threads
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
                            //خالي_جايون
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
#جيڪڏهن_وصف_ٿيل CV_PROFILE_ڌاڳا
        stat.threadFree = get_Tick_ڳڻپ();
        stat.keep_سرگرم = allow_سرگرم_wait;
#جيڪڏهن_پورو
    }
}

ThreadPool::ThreadPool()
{
#جيڪڏهن_وصف_ٿيل CV_PROFILE_ڌاڳا
    tickFreq = getTickFrequency();
#جيڪڏهن_پورو

    صحيح res = 0;
    res |= pthread_mutex_init(&mutex, عدم);
    res |= pthread_mutex_init(&mutex_notify, عدم);
#إذا defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
    res |= pthread_cond_init(&cond_thread_wake, عدم);
#جيڪڏهن_پورو
    res |= pthread_cond_init(&cond_thread_task_complete, عدم);

    إذا (0 != res)
    {
        CV_LOG_FATAL(عدم, "Failed to initialize ThreadPool (pthreads)");
    }
    انگ_ڌاڳا_2 = default_عدد_جو_ڌاڳا();
}

منطقي ThreadPool::reconfigure_(بنانشان new_ڌاڳا_ڳڻپ)
{
    إذا (new_ڌاڳا_ڳڻپ == ڌاڳا.size())
        أرجع خطأ;

    إذا (new_ڌاڳا_ڳڻپ < ڌاڳا.size())
    {
        CV_LOG_VERBOSE(عدم, 1, "MainThread: reduce worker pool: " << ڌاڳا.size() << " => " << new_ڌاڳا_ڳڻپ);
        std::vector< Ptr<ڪم_ڪندڙ_Thread> > release_ڌاڳا(ڌاڳا.size() - new_ڌاڳا_ڳڻپ);
        لكل (size_t i = new_ڌاڳا_ڳڻپ; i < ڌاڳا.size(); ++i)
        {
            pthread_mutex_lock(&ڌاڳا[i]->mutex);  // to avoid signal miss due pre-check
            ڌاڳا[i]->stop_thread = سچ;
            ڌاڳا[i]->has_wake_signal = سچ;
#إذا !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
            pthread_mutex_unlock(&ڌاڳا[i]->mutex);
            pthread_cond_broadcast/*pthread_cond_signal*/(&ڌاڳا[i]->cond_thread_wake); // wake thread
#وإلا
            pthread_mutex_unlock(&ڌاڳا[i]->mutex);
#جيڪڏهن_پورو
            std::swap(ڌاڳا[i], release_ڌاڳا[i - new_ڌاڳا_ڳڻپ]);
        }
#إذا defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
        CV_LOG_VERBOSE(عدم, 1, "MainThread: notify worker threads about termination...");
        pthread_cond_broadcast(&cond_thread_wake); // wake all threads
#جيڪڏهن_پورو
        ڌاڳا.resize(new_ڌاڳا_ڳڻپ);
        release_ڌاڳا.clear();  // calls thread_join which want to lock mutex
        أرجع خطأ;
    }
    وإلا
    {
        CV_LOG_VERBOSE(عدم, 1, "MainThread: upgrade worker pool: " << ڌاڳا.size() << " => " << new_ڌاڳا_ڳڻپ);
        لكل (size_t i = ڌاڳا.size(); i < new_ڌاڳا_ڳڻپ; ++i)
        {
            ڌاڳا.push_back(Ptr<ڪم_ڪندڙ_Thread>(جديد ڪم_ڪندڙ_Thread(*هذا, (بنانشان)i))); // spawn more threads
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
#جيڪڏهن_پورو
    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&mutex_notify);
}

فراغ ThreadPool::لاڳيتو(ثابت Range& range, ثابت ParallelLoopBody& body, مزدوج nstripes)
{
    CV_LOG_VERBOSE(عدم, 1, "MainThread: new parallel job: num_threads=" << انگ_ڌاڳا_2 << "   range=" << range.size() << "   nstripes=" << nstripes << "   job=" << (فراغ*)ڪم);
#جيڪڏهن_وصف_ٿيل CV_PROFILE_ڌاڳا
    ڪم_Submit_وقت = get_Tick_ڳڻپ();
    ڌاڳا_stat[0].reset();
    ڌاڳا_stat[0].threadWait = ڪم_Submit_وقت;
    ڌاڳا_stat[0].threadWake = ڪم_Submit_وقت;
#جيڪڏهن_پورو
    إذا (get_انگ_جو_ڌاڳا() > 1 &&
        ڪم == عدم &&
        (range.size() * nstripes >= 2 || (range.size() > 1 && nstripes <= 0))
    )
    {
        pthread_mutex_lock(&mutex);
        إذا (ڪم != عدم)
        {
            pthread_mutex_unlock(&mutex);
            body(range);
            أرجع;
        }
        reconfigure_(انگ_ڌاڳا_2 - 1);

        {
            CV_LOG_VERBOSE(عدم, 1, "MainThread: initialize parallel job: " << range.size());
            ڪم = Ptr<Parallel_ڪم>(جديد Parallel_ڪم(*هذا, range, body, nstripes));
            pthread_mutex_unlock(&mutex);

            CV_LOG_VERBOSE(عدم, 5, "MainThread: wake worker threads...");
            size_t انگ_ڌاڳا_منزل_wake = std::min(static_cast<size_t>(range.size()), ڌاڳا.size());
            لكل (size_t i = 0; i < انگ_ڌاڳا_منزل_wake; ++i)
            {
                إذا (ڪم->current_task >= ڪم->range.size())
                    اكسر;
                ڪم_ڪندڙ_Thread& thread = *(ڌاڳا[i].get());
                إذا (
#إذا defined(__clang__) && defined(__has_feature)
#إذا __has_feature(thread_sanitizer)
                        1 ||  // Robust workaround to avoid data race warning of `thread.job`
#جيڪڏهن_پورو
#جيڪڏهن_پورو
#إذا !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
                        thread.is_سرگرم_2 ||
#جيڪڏهن_پورو
                        thread.has_wake_signal
                        || !thread.ڪم.empty()  // #10881
                )
                {
                    pthread_mutex_lock(&thread.mutex);
                    thread.ڪم = ڪم;
#إذا !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
                    منطقي is_سرگرم_2 = thread.is_سرگرم_2;
#جيڪڏهن_پورو
                    thread.has_wake_signal = سچ;
#جيڪڏهن_وصف_ٿيل CV_PROFILE_ڌاڳا
                    ڌاڳا_stat[i + 1].reset();
#جيڪڏهن_پورو
                    pthread_mutex_unlock(&thread.mutex);
#إذا !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
                    إذا (!is_سرگرم_2)
                    {
                        pthread_cond_broadcast/*pthread_cond_signal*/(&thread.cond_thread_wake); // wake thread
                    }
#جيڪڏهن_پورو
                }
                وإلا
                {
                    CV_Assert(thread.ڪم.empty());
                    thread.ڪم = ڪم;
                    thread.has_wake_signal = سچ;
#جيڪڏهن_وصف_ٿيل CV_PROFILE_ڌاڳا
                    ڌاڳا_stat[i + 1].reset();
#جيڪڏهن_پورو
#إذا !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
                    pthread_cond_broadcast/*pthread_cond_signal*/(&thread.cond_thread_wake); // wake thread
#جيڪڏهن_پورو
                }
            }
#جيڪڏهن_وصف_ٿيل CV_PROFILE_ڌاڳا
            ڌاڳا_stat[0].threadPing = get_Tick_ڳڻپ();
#جيڪڏهن_پورو
#إذا defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
            pthread_cond_broadcast(&cond_thread_wake); // wake all threads
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل CV_PROFILE_ڌاڳا
            ڌاڳا_stat[0].threadWake = get_Tick_ڳڻپ();
#جيڪڏهن_پورو
            CV_LOG_VERBOSE(عدم, 5, "MainThread: wake worker threads... (done)");

            {
                Parallel_ڪم& j = *(هذا->ڪم);
#جيڪڏهن_وصف_ٿيل CV_PROFILE_ڌاڳا
                ڌاڳا_stat[0].thread_Execute_شروعات = get_Tick_ڳڻپ();
                ڌاڳا_stat[0].executedTasks = j.execute(خطأ);
                ڌاڳا_stat[0].threadExecuteStop = get_Tick_ڳڻپ();
#وإلا
                j.execute(خطأ);
#جيڪڏهن_پورو
                CV_Assert(j.current_task >= j.range.size());
                CV_LOG_VERBOSE(عدم, 5, "MainThread: complete self-tasks: " << j.سرگرم_thread_ڳڻپ << " " << j.completed_thread_ڳڻپ);
                إذا (ڪم->is_completed || j.سرگرم_thread_ڳڻپ == 0)
                {
                    ڪم->is_completed = سچ;
                    CV_LOG_VERBOSE(عدم, 5, "MainThread: no WIP worker threads");
                }
                وإلا
                {
                    إذا (CV_MAIN_THREAD_سرگرم_WAIT > 0)
                    {
                        لكل (صحيح i = 0; i < CV_MAIN_THREAD_سرگرم_WAIT; i++)  // don't spin too much in any case (inaccurate getTickCount())
                        {
                            إذا (ڪم->is_completed)
                            {
                                CV_LOG_VERBOSE(عدم, 5, "MainThread: job finalize (active wait) " << j.سرگرم_thread_ڳڻپ << " " << j.completed_thread_ڳڻپ);
                                اكسر;
                            }
                            إذا (CV_سرگرم_WAIT_PAUSE_حد_بندي > 0 && (i < CV_سرگرم_WAIT_PAUSE_حد_بندي || (i & 1)))
                                CV_PAUSE(16);
                            وإلا
                                CV_YIELD();
                        }
                    }
                    إذا (!ڪم->is_completed)
                    {
                        CV_LOG_VERBOSE(عدم, 5, "MainThread: prepare wait " << j.سرگرم_thread_ڳڻپ << " " << j.completed_thread_ڳڻپ);
                        pthread_mutex_lock(&mutex_notify);
                        لكل (;;)
                        {
                            إذا (ڪم->is_completed)
                            {
                                CV_LOG_VERBOSE(عدم, 5, "MainThread: job finalize (wait) " << j.سرگرم_thread_ڳڻپ << " " << j.completed_thread_ڳڻپ);
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
#جيڪڏهن_وصف_ٿيل CV_PROFILE_ڌاڳا
            ڌاڳا_stat[0].threadFree = get_Tick_ڳڻپ();
            std::ٻاهر << "Job: sz=" << range.size() << " nstripes=" << nstripes << "    Time: " << (ڌاڳا_stat[0].threadFree - ڪم_Submit_وقت) / tickFreq * 1e6 << " usec" << std::سٽ_پڄاڻي;
            لكل (صحيح i = 0; i < (صحيح)ڌاڳا.size() + 1; i++)
            {
                ڌاڳا_stat[i].dump(i - 1, ڪم_Submit_وقت, tickFreq);
            }
#جيڪڏهن_پورو
            إذا (ڪم)
            {
                pthread_mutex_lock(&mutex);
                CV_LOG_VERBOSE(عدم, 5, "MainThread: job release");
                CV_Assert(ڪم->is_completed);
                ڪم.release();
                pthread_mutex_unlock(&mutex);
            }
        }
    }
    وإلا
    {
        body(range);
    }
}

size_t ThreadPool::get_انگ_جو_ڌاڳا()
{
    أرجع انگ_ڌاڳا_2;
}

فراغ ThreadPool::سيٽ_انگ_جو_ڌاڳا(بنانشان n)
{
    إذا (n != انگ_ڌاڳا_2)
    {
        انگ_ڌاڳا_2 = n;
        إذا (n == 1)
           إذا (ڪم == عدم) reconfigure(0);  // stop worker threads immediately
    }
}

size_t parallel_pthreads_get_ڌاڳا_انگ()
{
    أرجع ThreadPool::instance().get_انگ_جو_ڌاڳا();
}

فراغ parallel_pthreads_سيٽ_ڌاڳا_انگ(صحيح num)
{
    إذا(num < 0)
    {
        ThreadPool::instance().سيٽ_انگ_جو_ڌاڳا(0);
    }
    وإلا
    {
        ThreadPool::instance().سيٽ_انگ_جو_ڌاڳا(بنانشان(num));
    }
}

فراغ parallel_for_pthreads(ثابت Range& range, ثابت ParallelLoopBody& body, مزدوج nstripes)
{
    ThreadPool::instance().لاڳيتو(range, body, nstripes);
}

}

#جيڪڏهن_پورو
