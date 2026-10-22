// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ضمن "precomp.hpp"

#ضمن "parallel_impl.hpp"

#که_تعریف_شوی HAVE_PTHREADS_PF
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
#که_تعریف_شوی DECLARE_CV_YIELD
DECLARE_CV_YIELD
#د_که_پای
#که_نه_تعریف_شوی CV_YIELD
# include <thread>
# define CV_YIELD() std::this_thread::yield()
#د_که_پای // CV_YIELD

// Spin lock's CPU-level yield (required for Hyper-Threading)
#که_تعریف_شوی DECLARE_CV_PAUSE
DECLARE_CV_PAUSE
#د_که_پای
#که_نه_تعریف_شوی CV_PAUSE
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
#د_که_پای // CV_PAUSE


نطاق cv
{

ساكن صحيح CV_فعال_WAIT_PAUSE_حد = (صحيح)utils::get_Configuration_Parameter_اندازه_T("OPENCV_THREAD_POOL_ACTIVE_WAIT_PAUSE_LIMIT", 16);  // iterations
ساكن صحيح CV_کارګر_فعال_WAIT = (صحيح)utils::get_Configuration_Parameter_اندازه_T("OPENCV_THREAD_POOL_ACTIVE_WAIT_WORKER", 2000);  // iterations
ساكن صحيح CV_MAIN_THREAD_فعال_WAIT = (صحيح)utils::get_Configuration_Parameter_اندازه_T("OPENCV_THREAD_POOL_ACTIVE_WAIT_MAIN", 10000); // iterations

ساكن صحيح CV_کارګر_فعال_WAIT_تارونه_حد = (صحيح)utils::get_Configuration_Parameter_اندازه_T("OPENCV_THREAD_POOL_ACTIVE_WAIT_THREADS_LIMIT", 0); // number of real cores

صنف کارګر_Thread;
صنف Parallel_دنده;

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

    فراغ reconfigure(بینښه new_تارونه_شمېر)
    {
        إذا (new_تارونه_شمېر == تارونه.size())
            أرجع;
        pthread_mutex_lock(&mutex);
        reconfigure_(new_تارونه_شمېر);
        pthread_mutex_unlock(&mutex);
    }
    منطقي reconfigure_(بینښه new_تارونه_شمېر); // internal implementation

    فراغ پرلپسې(ثابت Range& range, ثابت ParallelLoopBody& body, مزدوج nstripes);

    size_t get_عدد_د_تارونه();

    فراغ سېټ_عدد_د_تارونه(بینښه n);

    ThreadPool();

    ~ThreadPool();

    بینښه عدد_تارونه_2;

    pthread_mutex_t mutex;  // guards fields (job/threads) from non-worker threads (concurrent parallel_for calls)
#إذا defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
    pthread_cond_t cond_thread_wake;
#د_که_پای

    pthread_mutex_t mutex_notify;
    pthread_cond_t cond_thread_task_complete;

    std::vector< Ptr<کارګر_Thread> > تارونه;

    Ptr<Parallel_دنده> دنده;

#که_تعریف_شوی CV_PROFILE_تارونه
    مزدوج tickFreq;
    int64 دنده_Submit_وخت;
    بنية ThreadStatistics
    {
        ThreadStatistics() : threadWait(0)
        {
            reset();
        }
        فراغ reset()
        {
            threadWake = 0;
            thread_Execute_پیل = 0;
            threadExecuteStop = 0;
            executedTasks = 0;
            keep_فعال = خطأ;
            threadPing = get_Tick_شمېر();
        }
        int64 threadWait; // don't reset by default
        int64 threadPing; // don't reset by default
        int64 threadWake;
        int64 thread_Execute_پیل;
        int64 threadExecuteStop;
        int64 threadFree;
        بینښه executedTasks;
        منطقي keep_فعال;

        int64 dummy_[8]; // separate cache lines

        فراغ dump(صحيح id, int64 بنسټ_وخت, مزدوج tickFreq)
        {
            إذا (id < 0)
                std::وتنه << "Main: ";
            وإلا
                چاپ_کړه("T%03d: ", id + 2);
            چاپ_کړه("wait=% 10.1f   ping=% 6.1f",
                    threadWait > 0 ? (threadWait - بنسټ_وخت) / tickFreq * 1e6 : -0.0,
                    threadPing > 0 ? (threadPing - بنسټ_وخت) / tickFreq * 1e6 : -0.0);
            إذا (threadWake > 0)
                چاپ_کړه("   wake=% 6.1f",
                    (threadWake > 0 ? (threadWake - بنسټ_وخت) / tickFreq * 1e6 : -0.0));
            إذا (thread_Execute_پیل > 0)
            {
                چاپ_کړه("   exec=% 6.1f - % 6.1f   tasksDone=%5u   free=% 6.1f",
                    (thread_Execute_پیل > 0 ? (thread_Execute_پیل - بنسټ_وخت) / tickFreq * 1e6 : -0.0),
                    (threadExecuteStop > 0 ? (threadExecuteStop - بنسټ_وخت) / tickFreq * 1e6 : -0.0),
                    executedTasks,
                    (threadFree > 0 ? (threadFree - بنسټ_وخت) / tickFreq * 1e6 : -0.0));
                إذا (id >= 0)
                    چاپ_کړه(" active=%s\n", keep_فعال ? "true" : "false");
                وإلا
                    چاپ_کړه("\n");
            }
            وإلا
                چاپ_کړه("   ------------------------------------------------------------------------------\n");
        }
    };
    ThreadStatistics تارونه_stat[CV_PROFILE_تارونه]; // 0 - main thread, 1..N - worker threads
#د_که_پای

};

صنف کارګر_Thread
{
عام:
    ThreadPool& thread_pool;
    ثابت بینښه id;
    pthread_t posix_thread;
    منطقي is_created;

    std::atomic<منطقي> stop_thread;

    std::atomic<منطقي> has_wake_signal;

    Ptr<Parallel_دنده> دنده;

    pthread_mutex_t mutex;
#إذا !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
    متطاير منطقي is_فعال_2;
    pthread_cond_t cond_thread_wake;
#د_که_پای

    کارګر_Thread(ThreadPool& thread_pool_, بینښه id_) :
        thread_pool(thread_pool_),
        id(id_),
        posix_thread(0),
        is_created(خطأ),
        stop_thread(خطأ),
        has_wake_signal(خطأ)
#إذا !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
        , is_فعال_2(ریښتیا)
#د_که_پای
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
#د_که_پای
        res = pthread_create(&posix_thread, عدم, thread_loop_wrapper, (فراغ*)هذا);
        إذا (res != 0)
        {
            CV_LOG_ERROR(عدم, id << ": Can't spawn new thread: res = " << res);
        }
        وإلا
        {
            is_created = ریښتیا;
        }
    }

    ~کارګر_Thread()
    {
        CV_LOG_VERBOSE(عدم, 1, "MainThread: destroy worker thread: " << id);
        إذا (is_created)
        {
            إذا (!stop_thread)
            {
                pthread_mutex_lock(&mutex);  // to avoid signal miss due pre-check
                stop_thread = ریښتیا;
                pthread_mutex_unlock(&mutex);
#إذا defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
                pthread_cond_broadcast(&thread_pool.cond_thread_wake);
#وإلا
                pthread_cond_signal(&cond_thread_wake);
#د_که_پای
            }
            pthread_join(posix_thread, عدم);
        }
#إذا !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
        pthread_cond_destroy(&cond_thread_wake);
#د_که_پای
        pthread_mutex_destroy(&mutex);
    }

    فراغ thread_body();
    ساكن فراغ* thread_loop_wrapper(فراغ* thread_object)
    {
#که_تعریف_شوی OPENCV_WITH_ITT
        __itt_thread_set_name(cv::format("OpenCVThread-%03d", cv::utils::getThreadID()).c_str());
#د_که_پای
        ((کارګر_Thread*)thread_object)->thread_body();
        أرجع 0;
    }
};

صنف Parallel_دنده
{
عام:
    Parallel_دنده(ثابت ThreadPool& thread_pool_, ثابت Range& range_, ثابت ParallelLoopBody& body_, صحيح nstripes_) :
        thread_pool(thread_pool_),
        body(body_),
        range(range_),
        nstripes((بینښه)nstripes_),
        is_completed(خطأ)
    {
        CV_LOG_VERBOSE(عدم, 5, "ParallelJob::ParallelJob(" << (فراغ*)هذا << ")");
        current_task.store(0, std::memory_order_relaxed);
        فعال_thread_شمېر.store(0, std::memory_order_relaxed);
        completed_thread_شمېر.store(0, std::memory_order_relaxed);
        dummy0_[0] = 0, dummy1_[0] = 0, dummy2_[0] = 0; // compiler warning
    }

    ~Parallel_دنده()
    {
        CV_LOG_VERBOSE(عدم, 5, "ParallelJob::~ParallelJob(" << (فراغ*)هذا << ")");
    }

    بینښه execute(منطقي is_کارګر_thread)
    {
        بینښه executed_tasks = 0;
        ثابت صحيح task_شمېر = range.size();
        ثابت صحيح پاتې_شوی_multiplier = std::min(nstripes,
                std::max(
                        std::min(100u, thread_pool.عدد_تارونه_2 * 4),
                        thread_pool.عدد_تارونه_2 * 2
                ));  // experimental value
        لكل (;;)
        {
            صحيح chunk_اندازه = std::max(1, (task_شمېر - current_task) / پاتې_شوی_multiplier);
            صحيح id = current_task.fetch_add(chunk_اندازه, std::memory_order_seq_cst);
            إذا (id >= task_شمېر)
                اكسر; // no more free tasks

            executed_tasks += chunk_اندازه;
            صحيح پیل_id = id;
            صحيح پای_ټکی_id = std::min(task_شمېر, id + chunk_اندازه);
            CV_LOG_VERBOSE(عدم, 9, "Thread: job " << پیل_id << "-" << پای_ټکی_id);

            //TODO: if (not pending exception)
            {
                body.چلوونکی()(Range(range.پیل + پیل_id, range.پیل + پای_ټکی_id));
            }
            إذا (is_کارګر_thread && is_completed)
            {
                CV_LOG_ERROR(عدم, "\t\t\t\tBUG! Job: " << (فراغ*)هذا << " " << id << " " << فعال_thread_شمېر << " " << completed_thread_شمېر);
                CV_Assert(!is_completed); // TODO Dbg this
            }
        }
        أرجع executed_tasks;
    }

    ثابت ThreadPool& thread_pool;
    ثابت ParallelLoopBody& body;
    ثابت Range range;
    ثابت بینښه nstripes;

    std::atomic<صحيح> current_task;  // next free part of job
    int64 dummy0_[8];  // avoid cache-line reusing for the same atomics

    std::atomic<صحيح> فعال_thread_شمېر;  // number of threads worked on this job
    int64 dummy1_[8];  // avoid cache-line reusing for the same atomics

    std::atomic<صحيح> completed_thread_شمېر;  // number of threads completed any activities on this job
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
#د_که_پای
#د_که_پای
فراغ کارګر_Thread::thread_body()
{
    (فراغ)cv::utils::getThreadID(); // notify OpenCV about new thread
    CV_LOG_VERBOSE(عدم, 5, "Thread: new thread: " << id);

    منطقي allow_فعال_wait = ریښتیا;

#که_تعریف_شوی CV_PROFILE_تارونه
    ThreadPool::ThreadStatistics& stat = thread_pool.تارونه_stat[id + 1];
#د_که_پای

    طالما (!stop_thread)
    {
        CV_LOG_VERBOSE(عدم, 5, "Thread: ... loop iteration: allow_active_wait=" << allow_فعال_wait << "   has_wake_signal=" << has_wake_signal);
        إذا (allow_فعال_wait && CV_کارګر_فعال_WAIT > 0)
        {
            allow_فعال_wait = خطأ;
            لكل (صحيح i = 0; i < CV_کارګر_فعال_WAIT; i++)
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
#که_تعریف_شوی CV_PROFILE_تارونه
        stat.threadWait = get_Tick_شمېر();
#د_که_پای
        طالما (!has_wake_signal) // to handle spurious wakeups
        {
            //CV_LOG_VERBOSE(NULL, 5, "Thread: wait (sleep) ...");
#إذا defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
            pthread_cond_wait(&thread_pool.cond_thread_wake, &mutex);
#وإلا
            is_فعال_2 = خطأ;
            pthread_cond_wait(&cond_thread_wake, &mutex);
            is_فعال_2 = ریښتیا;
#د_که_پای
            CV_LOG_VERBOSE(عدم, 5, "Thread: wake ... (has_wake_signal=" << has_wake_signal << " stop_thread=" << stop_thread << ")")
        }
#که_تعریف_شوی CV_PROFILE_تارونه
        stat.threadWake = get_Tick_شمېر();
#د_که_پای

        CV_LOG_VERBOSE(عدم, 5, "Thread: checking for new job");
        إذا (CV_کارګر_فعال_WAIT_تارونه_حد == 0)
            allow_فعال_wait = ریښتیا;
        Ptr<Parallel_دنده> j_ptr; swap(j_ptr, دنده);
        has_wake_signal = خطأ;
        pthread_mutex_unlock(&mutex);

        إذا (!stop_thread)
        {
            Parallel_دنده* j = j_ptr;
            إذا (j)
            {
                CV_LOG_VERBOSE(عدم, 5, "Thread: job size=" << j->range.size() << " done=" << j->current_task);
                إذا (j->current_task < j->range.size())
                {
                    صحيح other = j->فعال_thread_شمېر.fetch_add(1, std::memory_order_seq_cst);
                    CV_LOG_VERBOSE(عدم, 5, "Thread: processing new job (with " << other << " other threads)"); CV_ناکارول_شوی(other);
#که_تعریف_شوی CV_PROFILE_تارونه
                    stat.thread_Execute_پیل = get_Tick_شمېر();
                    stat.executedTasks = j->execute(ریښتیا);
                    stat.threadExecuteStop = get_Tick_شمېر();
#وإلا
                    j->execute(ریښتیا);
#د_که_پای
                    صحيح completed = j->completed_thread_شمېر.fetch_add(1, std::memory_order_seq_cst) + 1;
                    صحيح فعال = j->فعال_thread_شمېر.load(std::memory_order_acquire);
                    إذا (CV_کارګر_فعال_WAIT_تارونه_حد > 0)
                    {
                        allow_فعال_wait = ریښتیا;
                        إذا (فعال >= CV_کارګر_فعال_WAIT_تارونه_حد && (id & 1) == 0) // turn off a half of threads
                            allow_فعال_wait = خطأ;
                    }
                    CV_LOG_VERBOSE(عدم, 5, "Thread: completed job processing: " << فعال << " " << completed);
                    إذا (فعال == completed)
                    {
                        منطقي need_signal = !j->is_completed;
                        j->is_completed = ریښتیا;
                        j = عدم; j_ptr.release();
                        إذا (need_signal)
                        {
                            CV_LOG_VERBOSE(عدم, 5, "Thread: job finished => notifying the main thread");
                            pthread_mutex_lock(&thread_pool.mutex_notify);  // to avoid signal miss due pre-check condition
                            //تشه
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
#که_تعریف_شوی CV_PROFILE_تارونه
        stat.threadFree = get_Tick_شمېر();
        stat.keep_فعال = allow_فعال_wait;
#د_که_پای
    }
}

ThreadPool::ThreadPool()
{
#که_تعریف_شوی CV_PROFILE_تارونه
    tickFreq = getTickFrequency();
#د_که_پای

    صحيح res = 0;
    res |= pthread_mutex_init(&mutex, عدم);
    res |= pthread_mutex_init(&mutex_notify, عدم);
#إذا defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
    res |= pthread_cond_init(&cond_thread_wake, عدم);
#د_که_پای
    res |= pthread_cond_init(&cond_thread_task_complete, عدم);

    إذا (0 != res)
    {
        CV_LOG_FATAL(عدم, "Failed to initialize ThreadPool (pthreads)");
    }
    عدد_تارونه_2 = default_شمېره_د_تارونه();
}

منطقي ThreadPool::reconfigure_(بینښه new_تارونه_شمېر)
{
    إذا (new_تارونه_شمېر == تارونه.size())
        أرجع خطأ;

    إذا (new_تارونه_شمېر < تارونه.size())
    {
        CV_LOG_VERBOSE(عدم, 1, "MainThread: reduce worker pool: " << تارونه.size() << " => " << new_تارونه_شمېر);
        std::vector< Ptr<کارګر_Thread> > release_تارونه(تارونه.size() - new_تارونه_شمېر);
        لكل (size_t i = new_تارونه_شمېر; i < تارونه.size(); ++i)
        {
            pthread_mutex_lock(&تارونه[i]->mutex);  // to avoid signal miss due pre-check
            تارونه[i]->stop_thread = ریښتیا;
            تارونه[i]->has_wake_signal = ریښتیا;
#إذا !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
            pthread_mutex_unlock(&تارونه[i]->mutex);
            pthread_cond_broadcast/*pthread_cond_signal*/(&تارونه[i]->cond_thread_wake); // wake thread
#وإلا
            pthread_mutex_unlock(&تارونه[i]->mutex);
#د_که_پای
            std::swap(تارونه[i], release_تارونه[i - new_تارونه_شمېر]);
        }
#إذا defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
        CV_LOG_VERBOSE(عدم, 1, "MainThread: notify worker threads about termination...");
        pthread_cond_broadcast(&cond_thread_wake); // wake all threads
#د_که_پای
        تارونه.resize(new_تارونه_شمېر);
        release_تارونه.clear();  // calls thread_join which want to lock mutex
        أرجع خطأ;
    }
    وإلا
    {
        CV_LOG_VERBOSE(عدم, 1, "MainThread: upgrade worker pool: " << تارونه.size() << " => " << new_تارونه_شمېر);
        لكل (size_t i = تارونه.size(); i < new_تارونه_شمېر; ++i)
        {
            تارونه.push_back(Ptr<کارګر_Thread>(جديد کارګر_Thread(*هذا, (بینښه)i))); // spawn more threads
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
#د_که_پای
    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&mutex_notify);
}

فراغ ThreadPool::پرلپسې(ثابت Range& range, ثابت ParallelLoopBody& body, مزدوج nstripes)
{
    CV_LOG_VERBOSE(عدم, 1, "MainThread: new parallel job: num_threads=" << عدد_تارونه_2 << "   range=" << range.size() << "   nstripes=" << nstripes << "   job=" << (فراغ*)دنده);
#که_تعریف_شوی CV_PROFILE_تارونه
    دنده_Submit_وخت = get_Tick_شمېر();
    تارونه_stat[0].reset();
    تارونه_stat[0].threadWait = دنده_Submit_وخت;
    تارونه_stat[0].threadWake = دنده_Submit_وخت;
#د_که_پای
    إذا (get_عدد_د_تارونه() > 1 &&
        دنده == عدم &&
        (range.size() * nstripes >= 2 || (range.size() > 1 && nstripes <= 0))
    )
    {
        pthread_mutex_lock(&mutex);
        إذا (دنده != عدم)
        {
            pthread_mutex_unlock(&mutex);
            body(range);
            أرجع;
        }
        reconfigure_(عدد_تارونه_2 - 1);

        {
            CV_LOG_VERBOSE(عدم, 1, "MainThread: initialize parallel job: " << range.size());
            دنده = Ptr<Parallel_دنده>(جديد Parallel_دنده(*هذا, range, body, nstripes));
            pthread_mutex_unlock(&mutex);

            CV_LOG_VERBOSE(عدم, 5, "MainThread: wake worker threads...");
            size_t عدد_تارونه_منزل_wake = std::min(static_cast<size_t>(range.size()), تارونه.size());
            لكل (size_t i = 0; i < عدد_تارونه_منزل_wake; ++i)
            {
                إذا (دنده->current_task >= دنده->range.size())
                    اكسر;
                کارګر_Thread& thread = *(تارونه[i].get());
                إذا (
#إذا defined(__clang__) && defined(__has_feature)
#إذا __has_feature(thread_sanitizer)
                        1 ||  // Robust workaround to avoid data race warning of `thread.job`
#د_که_پای
#د_که_پای
#إذا !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
                        thread.is_فعال_2 ||
#د_که_پای
                        thread.has_wake_signal
                        || !thread.دنده.empty()  // #10881
                )
                {
                    pthread_mutex_lock(&thread.mutex);
                    thread.دنده = دنده;
#إذا !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
                    منطقي is_فعال_2 = thread.is_فعال_2;
#د_که_پای
                    thread.has_wake_signal = ریښتیا;
#که_تعریف_شوی CV_PROFILE_تارونه
                    تارونه_stat[i + 1].reset();
#د_که_پای
                    pthread_mutex_unlock(&thread.mutex);
#إذا !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
                    إذا (!is_فعال_2)
                    {
                        pthread_cond_broadcast/*pthread_cond_signal*/(&thread.cond_thread_wake); // wake thread
                    }
#د_که_پای
                }
                وإلا
                {
                    CV_Assert(thread.دنده.empty());
                    thread.دنده = دنده;
                    thread.has_wake_signal = ریښتیا;
#که_تعریف_شوی CV_PROFILE_تارونه
                    تارونه_stat[i + 1].reset();
#د_که_پای
#إذا !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
                    pthread_cond_broadcast/*pthread_cond_signal*/(&thread.cond_thread_wake); // wake thread
#د_که_پای
                }
            }
#که_تعریف_شوی CV_PROFILE_تارونه
            تارونه_stat[0].threadPing = get_Tick_شمېر();
#د_که_پای
#إذا defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
            pthread_cond_broadcast(&cond_thread_wake); // wake all threads
#د_که_پای
#که_تعریف_شوی CV_PROFILE_تارونه
            تارونه_stat[0].threadWake = get_Tick_شمېر();
#د_که_پای
            CV_LOG_VERBOSE(عدم, 5, "MainThread: wake worker threads... (done)");

            {
                Parallel_دنده& j = *(هذا->دنده);
#که_تعریف_شوی CV_PROFILE_تارونه
                تارونه_stat[0].thread_Execute_پیل = get_Tick_شمېر();
                تارونه_stat[0].executedTasks = j.execute(خطأ);
                تارونه_stat[0].threadExecuteStop = get_Tick_شمېر();
#وإلا
                j.execute(خطأ);
#د_که_پای
                CV_Assert(j.current_task >= j.range.size());
                CV_LOG_VERBOSE(عدم, 5, "MainThread: complete self-tasks: " << j.فعال_thread_شمېر << " " << j.completed_thread_شمېر);
                إذا (دنده->is_completed || j.فعال_thread_شمېر == 0)
                {
                    دنده->is_completed = ریښتیا;
                    CV_LOG_VERBOSE(عدم, 5, "MainThread: no WIP worker threads");
                }
                وإلا
                {
                    إذا (CV_MAIN_THREAD_فعال_WAIT > 0)
                    {
                        لكل (صحيح i = 0; i < CV_MAIN_THREAD_فعال_WAIT; i++)  // don't spin too much in any case (inaccurate getTickCount())
                        {
                            إذا (دنده->is_completed)
                            {
                                CV_LOG_VERBOSE(عدم, 5, "MainThread: job finalize (active wait) " << j.فعال_thread_شمېر << " " << j.completed_thread_شمېر);
                                اكسر;
                            }
                            إذا (CV_فعال_WAIT_PAUSE_حد > 0 && (i < CV_فعال_WAIT_PAUSE_حد || (i & 1)))
                                CV_PAUSE(16);
                            وإلا
                                CV_YIELD();
                        }
                    }
                    إذا (!دنده->is_completed)
                    {
                        CV_LOG_VERBOSE(عدم, 5, "MainThread: prepare wait " << j.فعال_thread_شمېر << " " << j.completed_thread_شمېر);
                        pthread_mutex_lock(&mutex_notify);
                        لكل (;;)
                        {
                            إذا (دنده->is_completed)
                            {
                                CV_LOG_VERBOSE(عدم, 5, "MainThread: job finalize (wait) " << j.فعال_thread_شمېر << " " << j.completed_thread_شمېر);
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
#که_تعریف_شوی CV_PROFILE_تارونه
            تارونه_stat[0].threadFree = get_Tick_شمېر();
            std::وتنه << "Job: sz=" << range.size() << " nstripes=" << nstripes << "    Time: " << (تارونه_stat[0].threadFree - دنده_Submit_وخت) / tickFreq * 1e6 << " usec" << std::کرښه_پای;
            لكل (صحيح i = 0; i < (صحيح)تارونه.size() + 1; i++)
            {
                تارونه_stat[i].dump(i - 1, دنده_Submit_وخت, tickFreq);
            }
#د_که_پای
            إذا (دنده)
            {
                pthread_mutex_lock(&mutex);
                CV_LOG_VERBOSE(عدم, 5, "MainThread: job release");
                CV_Assert(دنده->is_completed);
                دنده.release();
                pthread_mutex_unlock(&mutex);
            }
        }
    }
    وإلا
    {
        body(range);
    }
}

size_t ThreadPool::get_عدد_د_تارونه()
{
    أرجع عدد_تارونه_2;
}

فراغ ThreadPool::سېټ_عدد_د_تارونه(بینښه n)
{
    إذا (n != عدد_تارونه_2)
    {
        عدد_تارونه_2 = n;
        إذا (n == 1)
           إذا (دنده == عدم) reconfigure(0);  // stop worker threads immediately
    }
}

size_t parallel_pthreads_get_تارونه_عدد()
{
    أرجع ThreadPool::instance().get_عدد_د_تارونه();
}

فراغ parallel_pthreads_سېټ_تارونه_عدد(صحيح num)
{
    إذا(num < 0)
    {
        ThreadPool::instance().سېټ_عدد_د_تارونه(0);
    }
    وإلا
    {
        ThreadPool::instance().سېټ_عدد_د_تارونه(بینښه(num));
    }
}

فراغ parallel_for_pthreads(ثابت Range& range, ثابت ParallelLoopBody& body, مزدوج nstripes)
{
    ThreadPool::instance().پرلپسې(range, body, nstripes);
}

}

#د_که_پای
