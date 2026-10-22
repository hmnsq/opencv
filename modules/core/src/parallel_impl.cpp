// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ضمن "precomp.hpp"

#ضمن "parallel_impl.hpp"

#اگر_تعریف_شده HAVE_PTHREADS_PF
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
#اگر_تعریف_شده DECLARE_CV_YIELD
DECLARE_CV_YIELD
#پایان_اگر
#اگر_تعریف_نشده CV_YIELD
# include <thread>
# define CV_YIELD() std::this_thread::yield()
#پایان_اگر // CV_YIELD

// Spin lock's CPU-level yield (required for Hyper-Threading)
#اگر_تعریف_شده DECLARE_CV_PAUSE
DECLARE_CV_PAUSE
#پایان_اگر
#اگر_تعریف_نشده CV_PAUSE
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
#پایان_اگر // CV_PAUSE


نطاق cv
{

ساكن صحيح CV_فعال_WAIT_PAUSE_حد = (صحيح)utils::get_Configuration_Parameter_اندازه_T("OPENCV_THREAD_POOL_ACTIVE_WAIT_PAUSE_LIMIT", 16);  // iterations
ساكن صحيح CV_کارگر_فعال_WAIT = (صحيح)utils::get_Configuration_Parameter_اندازه_T("OPENCV_THREAD_POOL_ACTIVE_WAIT_WORKER", 2000);  // iterations
ساكن صحيح CV_MAIN_THREAD_فعال_WAIT = (صحيح)utils::get_Configuration_Parameter_اندازه_T("OPENCV_THREAD_POOL_ACTIVE_WAIT_MAIN", 10000); // iterations

ساكن صحيح CV_کارگر_فعال_WAIT_نخها_حد = (صحيح)utils::get_Configuration_Parameter_اندازه_T("OPENCV_THREAD_POOL_ACTIVE_WAIT_THREADS_LIMIT", 0); // number of real cores

صنف کارگر_Thread;
صنف Parallel_کار;

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

    فراغ reconfigure(بیعلامت new_نخها_شمار)
    {
        إذا (new_نخها_شمار == نخها.size())
            أرجع;
        pthread_mutex_lock(&mutex);
        reconfigure_(new_نخها_شمار);
        pthread_mutex_unlock(&mutex);
    }
    منطقي reconfigure_(بیعلامت new_نخها_شمار); // internal implementation

    فراغ پیاپی(ثابت Range& range, ثابت ParallelLoopBody& body, مزدوج nstripes);

    size_t get_رقم_از_نخها();

    فراغ مجموعه_رقم_از_نخها(بیعلامت n);

    ThreadPool();

    ~ThreadPool();

    بیعلامت رقم_نخها_2;

    pthread_mutex_t mutex;  // guards fields (job/threads) from non-worker threads (concurrent parallel_for calls)
#إذا defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
    pthread_cond_t cond_thread_wake;
#پایان_اگر

    pthread_mutex_t mutex_notify;
    pthread_cond_t cond_thread_task_complete;

    std::vector< Ptr<کارگر_Thread> > نخها;

    Ptr<Parallel_کار> کار;

#اگر_تعریف_شده CV_PROFILE_نخها
    مزدوج tickFreq;
    int64 کار_Submit_زمان;
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
            keep_فعال = خطأ;
            threadPing = get_Tick_شمار();
        }
        int64 threadWait; // don't reset by default
        int64 threadPing; // don't reset by default
        int64 threadWake;
        int64 thread_Execute_شروع;
        int64 threadExecuteStop;
        int64 threadFree;
        بیعلامت executedTasks;
        منطقي keep_فعال;

        int64 dummy_[8]; // separate cache lines

        فراغ dump(صحيح id, int64 پایه_زمان, مزدوج tickFreq)
        {
            إذا (id < 0)
                std::خروجی << "Main: ";
            وإلا
                چاپ_کن("T%03d: ", id + 2);
            چاپ_کن("wait=% 10.1f   ping=% 6.1f",
                    threadWait > 0 ? (threadWait - پایه_زمان) / tickFreq * 1e6 : -0.0,
                    threadPing > 0 ? (threadPing - پایه_زمان) / tickFreq * 1e6 : -0.0);
            إذا (threadWake > 0)
                چاپ_کن("   wake=% 6.1f",
                    (threadWake > 0 ? (threadWake - پایه_زمان) / tickFreq * 1e6 : -0.0));
            إذا (thread_Execute_شروع > 0)
            {
                چاپ_کن("   exec=% 6.1f - % 6.1f   tasksDone=%5u   free=% 6.1f",
                    (thread_Execute_شروع > 0 ? (thread_Execute_شروع - پایه_زمان) / tickFreq * 1e6 : -0.0),
                    (threadExecuteStop > 0 ? (threadExecuteStop - پایه_زمان) / tickFreq * 1e6 : -0.0),
                    executedTasks,
                    (threadFree > 0 ? (threadFree - پایه_زمان) / tickFreq * 1e6 : -0.0));
                إذا (id >= 0)
                    چاپ_کن(" active=%s\n", keep_فعال ? "true" : "false");
                وإلا
                    چاپ_کن("\n");
            }
            وإلا
                چاپ_کن("   ------------------------------------------------------------------------------\n");
        }
    };
    ThreadStatistics نخها_stat[CV_PROFILE_نخها]; // 0 - main thread, 1..N - worker threads
#پایان_اگر

};

صنف کارگر_Thread
{
عام:
    ThreadPool& thread_pool;
    ثابت بیعلامت id;
    pthread_t posix_thread;
    منطقي is_created;

    std::atomic<منطقي> stop_thread;

    std::atomic<منطقي> has_wake_signal;

    Ptr<Parallel_کار> کار;

    pthread_mutex_t mutex;
#إذا !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
    متطاير منطقي is_فعال_2;
    pthread_cond_t cond_thread_wake;
#پایان_اگر

    کارگر_Thread(ThreadPool& thread_pool_, بیعلامت id_) :
        thread_pool(thread_pool_),
        id(id_),
        posix_thread(0),
        is_created(خطأ),
        stop_thread(خطأ),
        has_wake_signal(خطأ)
#إذا !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
        , is_فعال_2(درست)
#پایان_اگر
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
#پایان_اگر
        res = pthread_create(&posix_thread, عدم, thread_loop_wrapper, (فراغ*)هذا);
        إذا (res != 0)
        {
            CV_LOG_ERROR(عدم, id << ": Can't spawn new thread: res = " << res);
        }
        وإلا
        {
            is_created = درست;
        }
    }

    ~کارگر_Thread()
    {
        CV_LOG_VERBOSE(عدم, 1, "MainThread: destroy worker thread: " << id);
        إذا (is_created)
        {
            إذا (!stop_thread)
            {
                pthread_mutex_lock(&mutex);  // to avoid signal miss due pre-check
                stop_thread = درست;
                pthread_mutex_unlock(&mutex);
#إذا defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
                pthread_cond_broadcast(&thread_pool.cond_thread_wake);
#وإلا
                pthread_cond_signal(&cond_thread_wake);
#پایان_اگر
            }
            pthread_join(posix_thread, عدم);
        }
#إذا !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
        pthread_cond_destroy(&cond_thread_wake);
#پایان_اگر
        pthread_mutex_destroy(&mutex);
    }

    فراغ thread_body();
    ساكن فراغ* thread_loop_wrapper(فراغ* thread_object)
    {
#اگر_تعریف_شده OPENCV_WITH_ITT
        __itt_thread_set_name(cv::format("OpenCVThread-%03d", cv::utils::getThreadID()).c_str());
#پایان_اگر
        ((کارگر_Thread*)thread_object)->thread_body();
        أرجع 0;
    }
};

صنف Parallel_کار
{
عام:
    Parallel_کار(ثابت ThreadPool& thread_pool_, ثابت Range& range_, ثابت ParallelLoopBody& body_, صحيح nstripes_) :
        thread_pool(thread_pool_),
        body(body_),
        range(range_),
        nstripes((بیعلامت)nstripes_),
        is_completed(خطأ)
    {
        CV_LOG_VERBOSE(عدم, 5, "ParallelJob::ParallelJob(" << (فراغ*)هذا << ")");
        current_task.store(0, std::memory_order_relaxed);
        فعال_thread_شمار.store(0, std::memory_order_relaxed);
        completed_thread_شمار.store(0, std::memory_order_relaxed);
        dummy0_[0] = 0, dummy1_[0] = 0, dummy2_[0] = 0; // compiler warning
    }

    ~Parallel_کار()
    {
        CV_LOG_VERBOSE(عدم, 5, "ParallelJob::~ParallelJob(" << (فراغ*)هذا << ")");
    }

    بیعلامت execute(منطقي is_کارگر_thread)
    {
        بیعلامت executed_tasks = 0;
        ثابت صحيح task_شمار = range.size();
        ثابت صحيح مانده_multiplier = std::min(nstripes,
                std::max(
                        std::min(100u, thread_pool.رقم_نخها_2 * 4),
                        thread_pool.رقم_نخها_2 * 2
                ));  // experimental value
        لكل (;;)
        {
            صحيح chunk_اندازه = std::max(1, (task_شمار - current_task) / مانده_multiplier);
            صحيح id = current_task.fetch_add(chunk_اندازه, std::memory_order_seq_cst);
            إذا (id >= task_شمار)
                اكسر; // no more free tasks

            executed_tasks += chunk_اندازه;
            صحيح شروع_id = id;
            صحيح انتها_id = std::min(task_شمار, id + chunk_اندازه);
            CV_LOG_VERBOSE(عدم, 9, "Thread: job " << شروع_id << "-" << انتها_id);

            //TODO: if (not pending exception)
            {
                body.عملگر()(Range(range.شروع + شروع_id, range.شروع + انتها_id));
            }
            إذا (is_کارگر_thread && is_completed)
            {
                CV_LOG_ERROR(عدم, "\t\t\t\tBUG! Job: " << (فراغ*)هذا << " " << id << " " << فعال_thread_شمار << " " << completed_thread_شمار);
                CV_Assert(!is_completed); // TODO Dbg this
            }
        }
        أرجع executed_tasks;
    }

    ثابت ThreadPool& thread_pool;
    ثابت ParallelLoopBody& body;
    ثابت Range range;
    ثابت بیعلامت nstripes;

    std::atomic<صحيح> current_task;  // next free part of job
    int64 dummy0_[8];  // avoid cache-line reusing for the same atomics

    std::atomic<صحيح> فعال_thread_شمار;  // number of threads worked on this job
    int64 dummy1_[8];  // avoid cache-line reusing for the same atomics

    std::atomic<صحيح> completed_thread_شمار;  // number of threads completed any activities on this job
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
#پایان_اگر
#پایان_اگر
فراغ کارگر_Thread::thread_body()
{
    (فراغ)cv::utils::getThreadID(); // notify OpenCV about new thread
    CV_LOG_VERBOSE(عدم, 5, "Thread: new thread: " << id);

    منطقي allow_فعال_wait = درست;

#اگر_تعریف_شده CV_PROFILE_نخها
    ThreadPool::ThreadStatistics& stat = thread_pool.نخها_stat[id + 1];
#پایان_اگر

    طالما (!stop_thread)
    {
        CV_LOG_VERBOSE(عدم, 5, "Thread: ... loop iteration: allow_active_wait=" << allow_فعال_wait << "   has_wake_signal=" << has_wake_signal);
        إذا (allow_فعال_wait && CV_کارگر_فعال_WAIT > 0)
        {
            allow_فعال_wait = خطأ;
            لكل (صحيح i = 0; i < CV_کارگر_فعال_WAIT; i++)
            {
                إذا (has_wake_signal)
                    اكسر;
                إذا (CV_فعال_WAIT_PAUSE_حد > 0 && (i < CV_فعال_WAIT_PAUSE_حد || (i & 1)))
                    CV_PAUSE(16);
                وإلا
                    CV_YIELD();
            }
        }
        pthread_mutex_lock(&mutex);
#اگر_تعریف_شده CV_PROFILE_نخها
        stat.threadWait = get_Tick_شمار();
#پایان_اگر
        طالما (!has_wake_signal) // to handle spurious wakeups
        {
            //CV_LOG_VERBOSE(NULL, 5, "Thread: wait (sleep) ...");
#إذا defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
            pthread_cond_wait(&thread_pool.cond_thread_wake, &mutex);
#وإلا
            is_فعال_2 = خطأ;
            pthread_cond_wait(&cond_thread_wake, &mutex);
            is_فعال_2 = درست;
#پایان_اگر
            CV_LOG_VERBOSE(عدم, 5, "Thread: wake ... (has_wake_signal=" << has_wake_signal << " stop_thread=" << stop_thread << ")")
        }
#اگر_تعریف_شده CV_PROFILE_نخها
        stat.threadWake = get_Tick_شمار();
#پایان_اگر

        CV_LOG_VERBOSE(عدم, 5, "Thread: checking for new job");
        إذا (CV_کارگر_فعال_WAIT_نخها_حد == 0)
            allow_فعال_wait = درست;
        Ptr<Parallel_کار> j_ptr; swap(j_ptr, کار);
        has_wake_signal = خطأ;
        pthread_mutex_unlock(&mutex);

        إذا (!stop_thread)
        {
            Parallel_کار* j = j_ptr;
            إذا (j)
            {
                CV_LOG_VERBOSE(عدم, 5, "Thread: job size=" << j->range.size() << " done=" << j->current_task);
                إذا (j->current_task < j->range.size())
                {
                    صحيح other = j->فعال_thread_شمار.fetch_add(1, std::memory_order_seq_cst);
                    CV_LOG_VERBOSE(عدم, 5, "Thread: processing new job (with " << other << " other threads)"); CV_استفادهنشده(other);
#اگر_تعریف_شده CV_PROFILE_نخها
                    stat.thread_Execute_شروع = get_Tick_شمار();
                    stat.executedTasks = j->execute(درست);
                    stat.threadExecuteStop = get_Tick_شمار();
#وإلا
                    j->execute(درست);
#پایان_اگر
                    صحيح completed = j->completed_thread_شمار.fetch_add(1, std::memory_order_seq_cst) + 1;
                    صحيح فعال = j->فعال_thread_شمار.load(std::memory_order_acquire);
                    إذا (CV_کارگر_فعال_WAIT_نخها_حد > 0)
                    {
                        allow_فعال_wait = درست;
                        إذا (فعال >= CV_کارگر_فعال_WAIT_نخها_حد && (id & 1) == 0) // turn off a half of threads
                            allow_فعال_wait = خطأ;
                    }
                    CV_LOG_VERBOSE(عدم, 5, "Thread: completed job processing: " << فعال << " " << completed);
                    إذا (فعال == completed)
                    {
                        منطقي need_signal = !j->is_completed;
                        j->is_completed = درست;
                        j = عدم; j_ptr.release();
                        إذا (need_signal)
                        {
                            CV_LOG_VERBOSE(عدم, 5, "Thread: job finished => notifying the main thread");
                            pthread_mutex_lock(&thread_pool.mutex_notify);  // to avoid signal miss due pre-check condition
                            //خالی
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
#اگر_تعریف_شده CV_PROFILE_نخها
        stat.threadFree = get_Tick_شمار();
        stat.keep_فعال = allow_فعال_wait;
#پایان_اگر
    }
}

ThreadPool::ThreadPool()
{
#اگر_تعریف_شده CV_PROFILE_نخها
    tickFreq = getTickFrequency();
#پایان_اگر

    صحيح res = 0;
    res |= pthread_mutex_init(&mutex, عدم);
    res |= pthread_mutex_init(&mutex_notify, عدم);
#إذا defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
    res |= pthread_cond_init(&cond_thread_wake, عدم);
#پایان_اگر
    res |= pthread_cond_init(&cond_thread_task_complete, عدم);

    إذا (0 != res)
    {
        CV_LOG_FATAL(عدم, "Failed to initialize ThreadPool (pthreads)");
    }
    رقم_نخها_2 = default_عدد_از_نخها();
}

منطقي ThreadPool::reconfigure_(بیعلامت new_نخها_شمار)
{
    إذا (new_نخها_شمار == نخها.size())
        أرجع خطأ;

    إذا (new_نخها_شمار < نخها.size())
    {
        CV_LOG_VERBOSE(عدم, 1, "MainThread: reduce worker pool: " << نخها.size() << " => " << new_نخها_شمار);
        std::vector< Ptr<کارگر_Thread> > release_نخها(نخها.size() - new_نخها_شمار);
        لكل (size_t i = new_نخها_شمار; i < نخها.size(); ++i)
        {
            pthread_mutex_lock(&نخها[i]->mutex);  // to avoid signal miss due pre-check
            نخها[i]->stop_thread = درست;
            نخها[i]->has_wake_signal = درست;
#إذا !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
            pthread_mutex_unlock(&نخها[i]->mutex);
            pthread_cond_broadcast/*pthread_cond_signal*/(&نخها[i]->cond_thread_wake); // wake thread
#وإلا
            pthread_mutex_unlock(&نخها[i]->mutex);
#پایان_اگر
            std::swap(نخها[i], release_نخها[i - new_نخها_شمار]);
        }
#إذا defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
        CV_LOG_VERBOSE(عدم, 1, "MainThread: notify worker threads about termination...");
        pthread_cond_broadcast(&cond_thread_wake); // wake all threads
#پایان_اگر
        نخها.resize(new_نخها_شمار);
        release_نخها.clear();  // calls thread_join which want to lock mutex
        أرجع خطأ;
    }
    وإلا
    {
        CV_LOG_VERBOSE(عدم, 1, "MainThread: upgrade worker pool: " << نخها.size() << " => " << new_نخها_شمار);
        لكل (size_t i = نخها.size(); i < new_نخها_شمار; ++i)
        {
            نخها.push_back(Ptr<کارگر_Thread>(جديد کارگر_Thread(*هذا, (بیعلامت)i))); // spawn more threads
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
#پایان_اگر
    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&mutex_notify);
}

فراغ ThreadPool::پیاپی(ثابت Range& range, ثابت ParallelLoopBody& body, مزدوج nstripes)
{
    CV_LOG_VERBOSE(عدم, 1, "MainThread: new parallel job: num_threads=" << رقم_نخها_2 << "   range=" << range.size() << "   nstripes=" << nstripes << "   job=" << (فراغ*)کار);
#اگر_تعریف_شده CV_PROFILE_نخها
    کار_Submit_زمان = get_Tick_شمار();
    نخها_stat[0].reset();
    نخها_stat[0].threadWait = کار_Submit_زمان;
    نخها_stat[0].threadWake = کار_Submit_زمان;
#پایان_اگر
    إذا (get_رقم_از_نخها() > 1 &&
        کار == عدم &&
        (range.size() * nstripes >= 2 || (range.size() > 1 && nstripes <= 0))
    )
    {
        pthread_mutex_lock(&mutex);
        إذا (کار != عدم)
        {
            pthread_mutex_unlock(&mutex);
            body(range);
            أرجع;
        }
        reconfigure_(رقم_نخها_2 - 1);

        {
            CV_LOG_VERBOSE(عدم, 1, "MainThread: initialize parallel job: " << range.size());
            کار = Ptr<Parallel_کار>(جديد Parallel_کار(*هذا, range, body, nstripes));
            pthread_mutex_unlock(&mutex);

            CV_LOG_VERBOSE(عدم, 5, "MainThread: wake worker threads...");
            size_t رقم_نخها_مقصد_wake = std::min(static_cast<size_t>(range.size()), نخها.size());
            لكل (size_t i = 0; i < رقم_نخها_مقصد_wake; ++i)
            {
                إذا (کار->current_task >= کار->range.size())
                    اكسر;
                کارگر_Thread& thread = *(نخها[i].get());
                إذا (
#إذا defined(__clang__) && defined(__has_feature)
#إذا __has_feature(thread_sanitizer)
                        1 ||  // Robust workaround to avoid data race warning of `thread.job`
#پایان_اگر
#پایان_اگر
#إذا !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
                        thread.is_فعال_2 ||
#پایان_اگر
                        thread.has_wake_signal
                        || !thread.کار.empty()  // #10881
                )
                {
                    pthread_mutex_lock(&thread.mutex);
                    thread.کار = کار;
#إذا !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
                    منطقي is_فعال_2 = thread.is_فعال_2;
#پایان_اگر
                    thread.has_wake_signal = درست;
#اگر_تعریف_شده CV_PROFILE_نخها
                    نخها_stat[i + 1].reset();
#پایان_اگر
                    pthread_mutex_unlock(&thread.mutex);
#إذا !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
                    إذا (!is_فعال_2)
                    {
                        pthread_cond_broadcast/*pthread_cond_signal*/(&thread.cond_thread_wake); // wake thread
                    }
#پایان_اگر
                }
                وإلا
                {
                    CV_Assert(thread.کار.empty());
                    thread.کار = کار;
                    thread.has_wake_signal = درست;
#اگر_تعریف_شده CV_PROFILE_نخها
                    نخها_stat[i + 1].reset();
#پایان_اگر
#إذا !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
                    pthread_cond_broadcast/*pthread_cond_signal*/(&thread.cond_thread_wake); // wake thread
#پایان_اگر
                }
            }
#اگر_تعریف_شده CV_PROFILE_نخها
            نخها_stat[0].threadPing = get_Tick_شمار();
#پایان_اگر
#إذا defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
            pthread_cond_broadcast(&cond_thread_wake); // wake all threads
#پایان_اگر
#اگر_تعریف_شده CV_PROFILE_نخها
            نخها_stat[0].threadWake = get_Tick_شمار();
#پایان_اگر
            CV_LOG_VERBOSE(عدم, 5, "MainThread: wake worker threads... (done)");

            {
                Parallel_کار& j = *(هذا->کار);
#اگر_تعریف_شده CV_PROFILE_نخها
                نخها_stat[0].thread_Execute_شروع = get_Tick_شمار();
                نخها_stat[0].executedTasks = j.execute(خطأ);
                نخها_stat[0].threadExecuteStop = get_Tick_شمار();
#وإلا
                j.execute(خطأ);
#پایان_اگر
                CV_Assert(j.current_task >= j.range.size());
                CV_LOG_VERBOSE(عدم, 5, "MainThread: complete self-tasks: " << j.فعال_thread_شمار << " " << j.completed_thread_شمار);
                إذا (کار->is_completed || j.فعال_thread_شمار == 0)
                {
                    کار->is_completed = درست;
                    CV_LOG_VERBOSE(عدم, 5, "MainThread: no WIP worker threads");
                }
                وإلا
                {
                    إذا (CV_MAIN_THREAD_فعال_WAIT > 0)
                    {
                        لكل (صحيح i = 0; i < CV_MAIN_THREAD_فعال_WAIT; i++)  // don't spin too much in any case (inaccurate getTickCount())
                        {
                            إذا (کار->is_completed)
                            {
                                CV_LOG_VERBOSE(عدم, 5, "MainThread: job finalize (active wait) " << j.فعال_thread_شمار << " " << j.completed_thread_شمار);
                                اكسر;
                            }
                            إذا (CV_فعال_WAIT_PAUSE_حد > 0 && (i < CV_فعال_WAIT_PAUSE_حد || (i & 1)))
                                CV_PAUSE(16);
                            وإلا
                                CV_YIELD();
                        }
                    }
                    إذا (!کار->is_completed)
                    {
                        CV_LOG_VERBOSE(عدم, 5, "MainThread: prepare wait " << j.فعال_thread_شمار << " " << j.completed_thread_شمار);
                        pthread_mutex_lock(&mutex_notify);
                        لكل (;;)
                        {
                            إذا (کار->is_completed)
                            {
                                CV_LOG_VERBOSE(عدم, 5, "MainThread: job finalize (wait) " << j.فعال_thread_شمار << " " << j.completed_thread_شمار);
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
#اگر_تعریف_شده CV_PROFILE_نخها
            نخها_stat[0].threadFree = get_Tick_شمار();
            std::خروجی << "Job: sz=" << range.size() << " nstripes=" << nstripes << "    Time: " << (نخها_stat[0].threadFree - کار_Submit_زمان) / tickFreq * 1e6 << " usec" << std::پایان_سطر;
            لكل (صحيح i = 0; i < (صحيح)نخها.size() + 1; i++)
            {
                نخها_stat[i].dump(i - 1, کار_Submit_زمان, tickFreq);
            }
#پایان_اگر
            إذا (کار)
            {
                pthread_mutex_lock(&mutex);
                CV_LOG_VERBOSE(عدم, 5, "MainThread: job release");
                CV_Assert(کار->is_completed);
                کار.release();
                pthread_mutex_unlock(&mutex);
            }
        }
    }
    وإلا
    {
        body(range);
    }
}

size_t ThreadPool::get_رقم_از_نخها()
{
    أرجع رقم_نخها_2;
}

فراغ ThreadPool::مجموعه_رقم_از_نخها(بیعلامت n)
{
    إذا (n != رقم_نخها_2)
    {
        رقم_نخها_2 = n;
        إذا (n == 1)
           إذا (کار == عدم) reconfigure(0);  // stop worker threads immediately
    }
}

size_t parallel_pthreads_get_نخها_رقم()
{
    أرجع ThreadPool::instance().get_رقم_از_نخها();
}

فراغ parallel_pthreads_مجموعه_نخها_رقم(صحيح num)
{
    إذا(num < 0)
    {
        ThreadPool::instance().مجموعه_رقم_از_نخها(0);
    }
    وإلا
    {
        ThreadPool::instance().مجموعه_رقم_از_نخها(بیعلامت(num));
    }
}

فراغ parallel_for_pthreads(ثابت Range& range, ثابت ParallelLoopBody& body, مزدوج nstripes)
{
    ThreadPool::instance().پیاپی(range, body, nstripes);
}

}

#پایان_اگر
