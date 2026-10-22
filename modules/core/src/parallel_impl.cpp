// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#అన్తర్భావ "precomp.hpp"

#అన్తర్భావ "parallel_impl.hpp"

#నిర్వచితమైతే HAVE_PTHREADS_PF
#అన్తర్భావ <pthread.h>

#అన్తర్భావ <opencv2/core/utils/configuration.private.hpp>

#అన్తర్భావ <opencv2/core/utils/logger.defines.hpp>
//#undef CV_LOG_STRIP_LEVEL
//#define CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_VERBOSE + 1
#అన్తర్భావ <opencv2/core/utils/logger.hpp>

#అన్తర్భావ <opencv2/core/utils/trace.private.hpp>

//#define CV_PROFILE_THREADS 64
//#define getTickCount getCPUTickCount  // use this if getTickCount() calls are expensive (and getCPUTickCount() is accurate)

//#define CV_USE_GLOBAL_WORKERS_COND_VAR  // not effective on many-core systems (10+)

#అన్తర్భావ <atomic>

// Spin lock's OS-level yield
#నిర్వచితమైతే DECLARE_CV_YIELD
DECLARE_CV_YIELD
#అయితే_ముగింపు
#నిర్వచితం_కాకపోతే CV_YIELD
# include <thread>
# define CV_YIELD() std::this_thread::yield()
#అయితే_ముగింపు // CV_YIELD

// Spin lock's CPU-level yield (required for Hyper-Threading)
#నిర్వచితమైతే DECLARE_CV_PAUSE
DECLARE_CV_PAUSE
#అయితే_ముగింపు
#నిర్వచితం_కాకపోతే CV_PAUSE
# ఒకవేళ defined __GNUC__ && (defined __i386__ || defined __x86_64__)
#   include <x86intrin.h> /* for __rdtsc */
#   ఒకవేళ !defined(__SSE2__)
      స్థిర అంతఃవరుస శూన్య cv_non_sse_mm_pause() { __asm__ __volatile__ ("rep; nop"); }
#     define _mm_pause cv_non_sse_mm_pause
#   endif
// With Skylake CPUs and above, _mm_pause takes 140 cycles so no need for a loop.
#   define CV_PAUSE(v) చేయి { (శూన్య)v; _mm_pause(); } అయితే (0)
# elif defined __GNUC__ && defined __aarch64__
#   define CV_PAUSE(v) చేయి { కొరకు (పూర్ణాంకం __delay = (v); __delay > 0; --__delay) { asm అస్థిర("yield" ::: "memory"); } } అయితే (0)
# elif defined __GNUC__ && defined __arm__
#   define CV_PAUSE(v) చేయి { కొరకు (పూర్ణాంకం __delay = (v); __delay > 0; --__delay) { asm అస్థిర("" ::: "memory"); } } అయితే (0)
# elif defined __GNUC__ && defined __mips__ && __mips_isa_rev >= 2
#   define CV_PAUSE(v) చేయి { కొరకు (పూర్ణాంకం __delay = (v); __delay > 0; --__delay) { asm అస్థిర("pause" ::: "memory"); } } అయితే (0)
# elif defined __APPLE__ && defined __POWERPC__
#   define CV_PAUSE(v) చేయి { కొరకు (పూర్ణాంకం __delay = (v); __delay > 0; --__delay) { asm అస్థిర("or r27,r27,r27" ::: "memory"); } } అయితే (0)
# elif defined __GNUC__ && defined __PPC64__
#   define CV_PAUSE(v) చేయి { కొరకు (పూర్ణాంకం __delay = (v); __delay > 0; --__delay) { asm అస్థిర("or 27,27,27" ::: "memory"); } } అయితే (0)
# elif defined __GNUC__ && defined __riscv
// PAUSE HINT is not part of RISC-V ISA yet, but is under discussion now. For details see:
// https://github.com/riscv/riscv-isa-manual/pull/398
// https://github.com/riscv/riscv-isa-manual/issues/43
// #   define CV_PAUSE(v) do { for (int __delay = (v); __delay > 0; --__delay) { asm volatile("pause"); } } while (0)
#   define CV_PAUSE(v) చేయి { కొరకు (పూర్ణాంకం __delay = (v); __delay > 0; --__delay) { asm అస్థిర("nop"); } } అయితే (0)
# elif defined __GNUC__ && defined __loongarch__
#   define CV_PAUSE(v) చేయి { కొరకు (పూర్ణాంకం __delay = (v); __delay > 0; --__delay) { asm అస్థిర("nop"); } } అయితే (0)
# లేకపోతే
#   warning "Can't detect 'pause' (CPU-yield) instruction on the target platform. Specify CV_PAUSE() definition via compiler flags."
#   define CV_PAUSE(...) చేయి { /* no-op: works, but not effective */ } అయితే (0)
# endif
#అయితే_ముగింపు // CV_PAUSE


నామాకాశ cv
{

స్థిర పూర్ణాంకం CV_చురుకైన_WAIT_PAUSE_హద్దు = (పూర్ణాంకం)utils::get_Configuration_Parameter_కొలత_T("OPENCV_THREAD_POOL_ACTIVE_WAIT_PAUSE_LIMIT", 16);  // iterations
స్థిర పూర్ణాంకం CV_కార్మికుడు_చురుకైన_WAIT = (పూర్ణాంకం)utils::get_Configuration_Parameter_కొలత_T("OPENCV_THREAD_POOL_ACTIVE_WAIT_WORKER", 2000);  // iterations
స్థిర పూర్ణాంకం CV_MAIN_THREAD_చురుకైన_WAIT = (పూర్ణాంకం)utils::get_Configuration_Parameter_కొలత_T("OPENCV_THREAD_POOL_ACTIVE_WAIT_MAIN", 10000); // iterations

స్థిర పూర్ణాంకం CV_కార్మికుడు_చురుకైన_WAIT_దారాలు_హద్దు = (పూర్ణాంకం)utils::get_Configuration_Parameter_కొలత_T("OPENCV_THREAD_POOL_ACTIVE_WAIT_THREADS_LIMIT", 0); // number of real cores

తరగతి కార్మికుడు_Thread;
తరగతి Parallel_పని;

తరగతి ThreadPool
{
బహిరంగ:
    స్థిర ThreadPool& instance()
    {
        CV_SINGLETON_LAZY_INIT_REF(ThreadPool, కొత్త ThreadPool())
    }

    స్థిర శూన్య stop()
    {
        ThreadPool& manager = instance();
        manager.reconfigure(0);
    }

    శూన్య reconfigure(గుర్తులేని new_దారాలు_లెక్క)
    {
        ఒకవేళ (new_దారాలు_లెక్క == దారాలు.size())
            తిరిగి;
        pthread_mutex_lock(&mutex);
        reconfigure_(new_దారాలు_లెక్క);
        pthread_mutex_unlock(&mutex);
    }
    తార్కిక reconfigure_(గుర్తులేని new_దారాలు_లెక్క); // internal implementation

    శూన్య నిరంతరం(స్థిరరాశి Range& range, స్థిరరాశి ParallelLoopBody& body, ద్విగుణ nstripes);

    size_t get_అంకె_యొక్క_దారాలు();

    శూన్య సమితి_అంకె_యొక్క_దారాలు(గుర్తులేని n);

    ThreadPool();

    ~ThreadPool();

    గుర్తులేని అంకె_దారాలు_2;

    pthread_mutex_t mutex;  // guards fields (job/threads) from non-worker threads (concurrent parallel_for calls)
#ఒకవేళ defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
    pthread_cond_t cond_thread_wake;
#అయితే_ముగింపు

    pthread_mutex_t mutex_notify;
    pthread_cond_t cond_thread_task_complete;

    std::vector< Ptr<కార్మికుడు_Thread> > దారాలు;

    Ptr<Parallel_పని> పని;

#నిర్వచితమైతే CV_PROFILE_దారాలు
    ద్విగుణ tickFreq;
    int64 పని_Submit_సమయం;
    నిర్మాణం ThreadStatistics
    {
        ThreadStatistics() : threadWait(0)
        {
            reset();
        }
        శూన్య reset()
        {
            threadWake = 0;
            thread_Execute_ప్రారంభం = 0;
            threadExecuteStop = 0;
            executedTasks = 0;
            keep_చురుకైన = అబద్ధం;
            threadPing = get_Tick_లెక్క();
        }
        int64 threadWait; // don't reset by default
        int64 threadPing; // don't reset by default
        int64 threadWake;
        int64 thread_Execute_ప్రారంభం;
        int64 threadExecuteStop;
        int64 threadFree;
        గుర్తులేని executedTasks;
        తార్కిక keep_చురుకైన;

        int64 dummy_[8]; // separate cache lines

        శూన్య dump(పూర్ణాంకం id, int64 పునాది_సమయం, ద్విగుణ tickFreq)
        {
            ఒకవేళ (id < 0)
                std::బయటకు << "Main: ";
            లేకపోతే
                రాయి("T%03d: ", id + 2);
            రాయి("wait=% 10.1f   ping=% 6.1f",
                    threadWait > 0 ? (threadWait - పునాది_సమయం) / tickFreq * 1e6 : -0.0,
                    threadPing > 0 ? (threadPing - పునాది_సమయం) / tickFreq * 1e6 : -0.0);
            ఒకవేళ (threadWake > 0)
                రాయి("   wake=% 6.1f",
                    (threadWake > 0 ? (threadWake - పునాది_సమయం) / tickFreq * 1e6 : -0.0));
            ఒకవేళ (thread_Execute_ప్రారంభం > 0)
            {
                రాయి("   exec=% 6.1f - % 6.1f   tasksDone=%5u   free=% 6.1f",
                    (thread_Execute_ప్రారంభం > 0 ? (thread_Execute_ప్రారంభం - పునాది_సమయం) / tickFreq * 1e6 : -0.0),
                    (threadExecuteStop > 0 ? (threadExecuteStop - పునాది_సమయం) / tickFreq * 1e6 : -0.0),
                    executedTasks,
                    (threadFree > 0 ? (threadFree - పునాది_సమయం) / tickFreq * 1e6 : -0.0));
                ఒకవేళ (id >= 0)
                    రాయి(" active=%s\n", keep_చురుకైన ? "true" : "false");
                లేకపోతే
                    రాయి("\n");
            }
            లేకపోతే
                రాయి("   ------------------------------------------------------------------------------\n");
        }
    };
    ThreadStatistics దారాలు_stat[CV_PROFILE_దారాలు]; // 0 - main thread, 1..N - worker threads
#అయితే_ముగింపు

};

తరగతి కార్మికుడు_Thread
{
బహిరంగ:
    ThreadPool& thread_pool;
    స్థిరరాశి గుర్తులేని id;
    pthread_t posix_thread;
    తార్కిక is_created;

    std::atomic<తార్కిక> stop_thread;

    std::atomic<తార్కిక> has_wake_signal;

    Ptr<Parallel_పని> పని;

    pthread_mutex_t mutex;
#ఒకవేళ !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
    అస్థిర తార్కిక is_చురుకైన_2;
    pthread_cond_t cond_thread_wake;
#అయితే_ముగింపు

    కార్మికుడు_Thread(ThreadPool& thread_pool_, గుర్తులేని id_) :
        thread_pool(thread_pool_),
        id(id_),
        posix_thread(0),
        is_created(అబద్ధం),
        stop_thread(అబద్ధం),
        has_wake_signal(అబద్ధం)
#ఒకవేళ !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
        , is_చురుకైన_2(నిజం)
#అయితే_ముగింపు
    {
        CV_LOG_VERBOSE(ఖాళీ, 1, "MainThread: initializing new worker: " << id);
        పూర్ణాంకం res = pthread_mutex_init(&mutex, ఖాళీ);
        ఒకవేళ (res != 0)
        {
            CV_LOG_ERROR(ఖాళీ, id << ": Can't create thread mutex: res = " << res);
            తిరిగి;
        }
#ఒకవేళ !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
        res = pthread_cond_init(&cond_thread_wake, ఖాళీ);
        ఒకవేళ (res != 0)
        {
            CV_LOG_ERROR(ఖాళీ, id << ": Can't create thread condition variable: res = " << res);
            తిరిగి;
        }
#అయితే_ముగింపు
        res = pthread_create(&posix_thread, ఖాళీ, thread_loop_wrapper, (శూన్య*)ఇది);
        ఒకవేళ (res != 0)
        {
            CV_LOG_ERROR(ఖాళీ, id << ": Can't spawn new thread: res = " << res);
        }
        లేకపోతే
        {
            is_created = నిజం;
        }
    }

    ~కార్మికుడు_Thread()
    {
        CV_LOG_VERBOSE(ఖాళీ, 1, "MainThread: destroy worker thread: " << id);
        ఒకవేళ (is_created)
        {
            ఒకవేళ (!stop_thread)
            {
                pthread_mutex_lock(&mutex);  // to avoid signal miss due pre-check
                stop_thread = నిజం;
                pthread_mutex_unlock(&mutex);
#ఒకవేళ defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
                pthread_cond_broadcast(&thread_pool.cond_thread_wake);
#లేకపోతే
                pthread_cond_signal(&cond_thread_wake);
#అయితే_ముగింపు
            }
            pthread_join(posix_thread, ఖాళీ);
        }
#ఒకవేళ !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
        pthread_cond_destroy(&cond_thread_wake);
#అయితే_ముగింపు
        pthread_mutex_destroy(&mutex);
    }

    శూన్య thread_body();
    స్థిర శూన్య* thread_loop_wrapper(శూన్య* thread_object)
    {
#నిర్వచితమైతే OPENCV_WITH_ITT
        __itt_thread_set_name(cv::format("OpenCVThread-%03d", cv::utils::getThreadID()).c_str());
#అయితే_ముగింపు
        ((కార్మికుడు_Thread*)thread_object)->thread_body();
        తిరిగి 0;
    }
};

తరగతి Parallel_పని
{
బహిరంగ:
    Parallel_పని(స్థిరరాశి ThreadPool& thread_pool_, స్థిరరాశి Range& range_, స్థిరరాశి ParallelLoopBody& body_, పూర్ణాంకం nstripes_) :
        thread_pool(thread_pool_),
        body(body_),
        range(range_),
        nstripes((గుర్తులేని)nstripes_),
        is_completed(అబద్ధం)
    {
        CV_LOG_VERBOSE(ఖాళీ, 5, "ParallelJob::ParallelJob(" << (శూన్య*)ఇది << ")");
        current_task.store(0, std::memory_order_relaxed);
        చురుకైన_thread_లెక్క.store(0, std::memory_order_relaxed);
        completed_thread_లెక్క.store(0, std::memory_order_relaxed);
        dummy0_[0] = 0, dummy1_[0] = 0, dummy2_[0] = 0; // compiler warning
    }

    ~Parallel_పని()
    {
        CV_LOG_VERBOSE(ఖాళీ, 5, "ParallelJob::~ParallelJob(" << (శూన్య*)ఇది << ")");
    }

    గుర్తులేని execute(తార్కిక is_కార్మికుడు_thread)
    {
        గుర్తులేని executed_tasks = 0;
        స్థిరరాశి పూర్ణాంకం task_లెక్క = range.size();
        స్థిరరాశి పూర్ణాంకం మిగిలిన_multiplier = std::min(nstripes,
                std::max(
                        std::min(100u, thread_pool.అంకె_దారాలు_2 * 4),
                        thread_pool.అంకె_దారాలు_2 * 2
                ));  // experimental value
        కొరకు (;;)
        {
            పూర్ణాంకం chunk_కొలత = std::max(1, (task_లెక్క - current_task) / మిగిలిన_multiplier);
            పూర్ణాంకం id = current_task.fetch_add(chunk_కొలత, std::memory_order_seq_cst);
            ఒకవేళ (id >= task_లెక్క)
                ఆపు; // no more free tasks

            executed_tasks += chunk_కొలత;
            పూర్ణాంకం ప్రారంభం_id = id;
            పూర్ణాంకం చివర_id = std::min(task_లెక్క, id + chunk_కొలత);
            CV_LOG_VERBOSE(ఖాళీ, 9, "Thread: job " << ప్రారంభం_id << "-" << చివర_id);

            //TODO: if (not pending exception)
            {
                body.క్రియాకారకం()(Range(range.ప్రారంభం + ప్రారంభం_id, range.ప్రారంభం + చివర_id));
            }
            ఒకవేళ (is_కార్మికుడు_thread && is_completed)
            {
                CV_LOG_ERROR(ఖాళీ, "\t\t\t\tBUG! Job: " << (శూన్య*)ఇది << " " << id << " " << చురుకైన_thread_లెక్క << " " << completed_thread_లెక్క);
                CV_Assert(!is_completed); // TODO Dbg this
            }
        }
        తిరిగి executed_tasks;
    }

    స్థిరరాశి ThreadPool& thread_pool;
    స్థిరరాశి ParallelLoopBody& body;
    స్థిరరాశి Range range;
    స్థిరరాశి గుర్తులేని nstripes;

    std::atomic<పూర్ణాంకం> current_task;  // next free part of job
    int64 dummy0_[8];  // avoid cache-line reusing for the same atomics

    std::atomic<పూర్ణాంకం> చురుకైన_thread_లెక్క;  // number of threads worked on this job
    int64 dummy1_[8];  // avoid cache-line reusing for the same atomics

    std::atomic<పూర్ణాంకం> completed_thread_లెక్క;  // number of threads completed any activities on this job
    int64 dummy2_[8];  // avoid cache-line reusing for the same atomics

    std::atomic<తార్కిక> is_completed;

    // TODO exception handling
};


// Disable thread sanitization check when CV_USE_GLOBAL_WORKERS_COND_VAR is not
// set because it triggers as the main thread reads isActive while the children
// thread writes it (but it all works out because a mutex is locked in the main
// thread and isActive re-checked).
// This is to solve issue #19463.
#ఒకవేళ !defined(CV_USE_GLOBAL_WORKERS_COND_VAR) && defined(__clang__) && defined(__has_feature)
#ఒకవేళ __has_feature(thread_sanitizer)
__attribute__((no_sanitize("thread")))
#అయితే_ముగింపు
#అయితే_ముగింపు
శూన్య కార్మికుడు_Thread::thread_body()
{
    (శూన్య)cv::utils::getThreadID(); // notify OpenCV about new thread
    CV_LOG_VERBOSE(ఖాళీ, 5, "Thread: new thread: " << id);

    తార్కిక allow_చురుకైన_wait = నిజం;

#నిర్వచితమైతే CV_PROFILE_దారాలు
    ThreadPool::ThreadStatistics& stat = thread_pool.దారాలు_stat[id + 1];
#అయితే_ముగింపు

    అయితే (!stop_thread)
    {
        CV_LOG_VERBOSE(ఖాళీ, 5, "Thread: ... loop iteration: allow_active_wait=" << allow_చురుకైన_wait << "   has_wake_signal=" << has_wake_signal);
        ఒకవేళ (allow_చురుకైన_wait && CV_కార్మికుడు_చురుకైన_WAIT > 0)
        {
            allow_చురుకైన_wait = అబద్ధం;
            కొరకు (పూర్ణాంకం i = 0; i < CV_కార్మికుడు_చురుకైన_WAIT; i++)
            {
                ఒకవేళ (has_wake_signal)
                    ఆపు;
                ఒకవేళ (CV_చురుకైన_WAIT_PAUSE_హద్దు > 0 && (i < CV_చురుకైన_WAIT_PAUSE_హద్దు || (i & 1)))
                    CV_PAUSE(16);
                లేకపోతే
                    CV_YIELD();
            }
        }
        pthread_mutex_lock(&mutex);
#నిర్వచితమైతే CV_PROFILE_దారాలు
        stat.threadWait = get_Tick_లెక్క();
#అయితే_ముగింపు
        అయితే (!has_wake_signal) // to handle spurious wakeups
        {
            //CV_LOG_VERBOSE(NULL, 5, "Thread: wait (sleep) ...");
#ఒకవేళ defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
            pthread_cond_wait(&thread_pool.cond_thread_wake, &mutex);
#లేకపోతే
            is_చురుకైన_2 = అబద్ధం;
            pthread_cond_wait(&cond_thread_wake, &mutex);
            is_చురుకైన_2 = నిజం;
#అయితే_ముగింపు
            CV_LOG_VERBOSE(ఖాళీ, 5, "Thread: wake ... (has_wake_signal=" << has_wake_signal << " stop_thread=" << stop_thread << ")")
        }
#నిర్వచితమైతే CV_PROFILE_దారాలు
        stat.threadWake = get_Tick_లెక్క();
#అయితే_ముగింపు

        CV_LOG_VERBOSE(ఖాళీ, 5, "Thread: checking for new job");
        ఒకవేళ (CV_కార్మికుడు_చురుకైన_WAIT_దారాలు_హద్దు == 0)
            allow_చురుకైన_wait = నిజం;
        Ptr<Parallel_పని> j_ptr; swap(j_ptr, పని);
        has_wake_signal = అబద్ధం;
        pthread_mutex_unlock(&mutex);

        ఒకవేళ (!stop_thread)
        {
            Parallel_పని* j = j_ptr;
            ఒకవేళ (j)
            {
                CV_LOG_VERBOSE(ఖాళీ, 5, "Thread: job size=" << j->range.size() << " done=" << j->current_task);
                ఒకవేళ (j->current_task < j->range.size())
                {
                    పూర్ణాంకం other = j->చురుకైన_thread_లెక్క.fetch_add(1, std::memory_order_seq_cst);
                    CV_LOG_VERBOSE(ఖాళీ, 5, "Thread: processing new job (with " << other << " other threads)"); CV_వాడని(other);
#నిర్వచితమైతే CV_PROFILE_దారాలు
                    stat.thread_Execute_ప్రారంభం = get_Tick_లెక్క();
                    stat.executedTasks = j->execute(నిజం);
                    stat.threadExecuteStop = get_Tick_లెక్క();
#లేకపోతే
                    j->execute(నిజం);
#అయితే_ముగింపు
                    పూర్ణాంకం completed = j->completed_thread_లెక్క.fetch_add(1, std::memory_order_seq_cst) + 1;
                    పూర్ణాంకం చురుకైన = j->చురుకైన_thread_లెక్క.load(std::memory_order_acquire);
                    ఒకవేళ (CV_కార్మికుడు_చురుకైన_WAIT_దారాలు_హద్దు > 0)
                    {
                        allow_చురుకైన_wait = నిజం;
                        ఒకవేళ (చురుకైన >= CV_కార్మికుడు_చురుకైన_WAIT_దారాలు_హద్దు && (id & 1) == 0) // turn off a half of threads
                            allow_చురుకైన_wait = అబద్ధం;
                    }
                    CV_LOG_VERBOSE(ఖాళీ, 5, "Thread: completed job processing: " << చురుకైన << " " << completed);
                    ఒకవేళ (చురుకైన == completed)
                    {
                        తార్కిక need_signal = !j->is_completed;
                        j->is_completed = నిజం;
                        j = ఖాళీ; j_ptr.release();
                        ఒకవేళ (need_signal)
                        {
                            CV_LOG_VERBOSE(ఖాళీ, 5, "Thread: job finished => notifying the main thread");
                            pthread_mutex_lock(&thread_pool.mutex_notify);  // to avoid signal miss due pre-check condition
                            //ఖాళీలు
                            pthread_mutex_unlock(&thread_pool.mutex_notify);
                            pthread_cond_broadcast/*pthread_cond_signal*/(&thread_pool.cond_thread_task_complete);
                        }
                    }
                }
                లేకపోతే
                {
                    CV_LOG_VERBOSE(ఖాళీ, 5, "Thread: no free job tasks");
                }
            }
        }
#నిర్వచితమైతే CV_PROFILE_దారాలు
        stat.threadFree = get_Tick_లెక్క();
        stat.keep_చురుకైన = allow_చురుకైన_wait;
#అయితే_ముగింపు
    }
}

ThreadPool::ThreadPool()
{
#నిర్వచితమైతే CV_PROFILE_దారాలు
    tickFreq = getTickFrequency();
#అయితే_ముగింపు

    పూర్ణాంకం res = 0;
    res |= pthread_mutex_init(&mutex, ఖాళీ);
    res |= pthread_mutex_init(&mutex_notify, ఖాళీ);
#ఒకవేళ defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
    res |= pthread_cond_init(&cond_thread_wake, ఖాళీ);
#అయితే_ముగింపు
    res |= pthread_cond_init(&cond_thread_task_complete, ఖాళీ);

    ఒకవేళ (0 != res)
    {
        CV_LOG_FATAL(ఖాళీ, "Failed to initialize ThreadPool (pthreads)");
    }
    అంకె_దారాలు_2 = default_సంఖ్య_యొక్క_దారాలు();
}

తార్కిక ThreadPool::reconfigure_(గుర్తులేని new_దారాలు_లెక్క)
{
    ఒకవేళ (new_దారాలు_లెక్క == దారాలు.size())
        తిరిగి అబద్ధం;

    ఒకవేళ (new_దారాలు_లెక్క < దారాలు.size())
    {
        CV_LOG_VERBOSE(ఖాళీ, 1, "MainThread: reduce worker pool: " << దారాలు.size() << " => " << new_దారాలు_లెక్క);
        std::vector< Ptr<కార్మికుడు_Thread> > release_దారాలు(దారాలు.size() - new_దారాలు_లెక్క);
        కొరకు (size_t i = new_దారాలు_లెక్క; i < దారాలు.size(); ++i)
        {
            pthread_mutex_lock(&దారాలు[i]->mutex);  // to avoid signal miss due pre-check
            దారాలు[i]->stop_thread = నిజం;
            దారాలు[i]->has_wake_signal = నిజం;
#ఒకవేళ !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
            pthread_mutex_unlock(&దారాలు[i]->mutex);
            pthread_cond_broadcast/*pthread_cond_signal*/(&దారాలు[i]->cond_thread_wake); // wake thread
#లేకపోతే
            pthread_mutex_unlock(&దారాలు[i]->mutex);
#అయితే_ముగింపు
            std::swap(దారాలు[i], release_దారాలు[i - new_దారాలు_లెక్క]);
        }
#ఒకవేళ defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
        CV_LOG_VERBOSE(ఖాళీ, 1, "MainThread: notify worker threads about termination...");
        pthread_cond_broadcast(&cond_thread_wake); // wake all threads
#అయితే_ముగింపు
        దారాలు.resize(new_దారాలు_లెక్క);
        release_దారాలు.clear();  // calls thread_join which want to lock mutex
        తిరిగి అబద్ధం;
    }
    లేకపోతే
    {
        CV_LOG_VERBOSE(ఖాళీ, 1, "MainThread: upgrade worker pool: " << దారాలు.size() << " => " << new_దారాలు_లెక్క);
        కొరకు (size_t i = దారాలు.size(); i < new_దారాలు_లెక్క; ++i)
        {
            దారాలు.push_back(Ptr<కార్మికుడు_Thread>(కొత్త కార్మికుడు_Thread(*ఇది, (గుర్తులేని)i))); // spawn more threads
        }
    }
    తిరిగి అబద్ధం;
}

ThreadPool::~ThreadPool()
{
    reconfigure(0);
    pthread_cond_destroy(&cond_thread_task_complete);
#ఒకవేళ defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
    pthread_cond_destroy(&cond_thread_wake);
#అయితే_ముగింపు
    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&mutex_notify);
}

శూన్య ThreadPool::నిరంతరం(స్థిరరాశి Range& range, స్థిరరాశి ParallelLoopBody& body, ద్విగుణ nstripes)
{
    CV_LOG_VERBOSE(ఖాళీ, 1, "MainThread: new parallel job: num_threads=" << అంకె_దారాలు_2 << "   range=" << range.size() << "   nstripes=" << nstripes << "   job=" << (శూన్య*)పని);
#నిర్వచితమైతే CV_PROFILE_దారాలు
    పని_Submit_సమయం = get_Tick_లెక్క();
    దారాలు_stat[0].reset();
    దారాలు_stat[0].threadWait = పని_Submit_సమయం;
    దారాలు_stat[0].threadWake = పని_Submit_సమయం;
#అయితే_ముగింపు
    ఒకవేళ (get_అంకె_యొక్క_దారాలు() > 1 &&
        పని == ఖాళీ &&
        (range.size() * nstripes >= 2 || (range.size() > 1 && nstripes <= 0))
    )
    {
        pthread_mutex_lock(&mutex);
        ఒకవేళ (పని != ఖాళీ)
        {
            pthread_mutex_unlock(&mutex);
            body(range);
            తిరిగి;
        }
        reconfigure_(అంకె_దారాలు_2 - 1);

        {
            CV_LOG_VERBOSE(ఖాళీ, 1, "MainThread: initialize parallel job: " << range.size());
            పని = Ptr<Parallel_పని>(కొత్త Parallel_పని(*ఇది, range, body, nstripes));
            pthread_mutex_unlock(&mutex);

            CV_LOG_VERBOSE(ఖాళీ, 5, "MainThread: wake worker threads...");
            size_t అంకె_దారాలు_గమ్యం_wake = std::min(static_cast<size_t>(range.size()), దారాలు.size());
            కొరకు (size_t i = 0; i < అంకె_దారాలు_గమ్యం_wake; ++i)
            {
                ఒకవేళ (పని->current_task >= పని->range.size())
                    ఆపు;
                కార్మికుడు_Thread& thread = *(దారాలు[i].get());
                ఒకవేళ (
#ఒకవేళ defined(__clang__) && defined(__has_feature)
#ఒకవేళ __has_feature(thread_sanitizer)
                        1 ||  // Robust workaround to avoid data race warning of `thread.job`
#అయితే_ముగింపు
#అయితే_ముగింపు
#ఒకవేళ !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
                        thread.is_చురుకైన_2 ||
#అయితే_ముగింపు
                        thread.has_wake_signal
                        || !thread.పని.empty()  // #10881
                )
                {
                    pthread_mutex_lock(&thread.mutex);
                    thread.పని = పని;
#ఒకవేళ !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
                    తార్కిక is_చురుకైన_2 = thread.is_చురుకైన_2;
#అయితే_ముగింపు
                    thread.has_wake_signal = నిజం;
#నిర్వచితమైతే CV_PROFILE_దారాలు
                    దారాలు_stat[i + 1].reset();
#అయితే_ముగింపు
                    pthread_mutex_unlock(&thread.mutex);
#ఒకవేళ !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
                    ఒకవేళ (!is_చురుకైన_2)
                    {
                        pthread_cond_broadcast/*pthread_cond_signal*/(&thread.cond_thread_wake); // wake thread
                    }
#అయితే_ముగింపు
                }
                లేకపోతే
                {
                    CV_Assert(thread.పని.empty());
                    thread.పని = పని;
                    thread.has_wake_signal = నిజం;
#నిర్వచితమైతే CV_PROFILE_దారాలు
                    దారాలు_stat[i + 1].reset();
#అయితే_ముగింపు
#ఒకవేళ !defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
                    pthread_cond_broadcast/*pthread_cond_signal*/(&thread.cond_thread_wake); // wake thread
#అయితే_ముగింపు
                }
            }
#నిర్వచితమైతే CV_PROFILE_దారాలు
            దారాలు_stat[0].threadPing = get_Tick_లెక్క();
#అయితే_ముగింపు
#ఒకవేళ defined(CV_USE_GLOBAL_WORKERS_COND_VAR)
            pthread_cond_broadcast(&cond_thread_wake); // wake all threads
#అయితే_ముగింపు
#నిర్వచితమైతే CV_PROFILE_దారాలు
            దారాలు_stat[0].threadWake = get_Tick_లెక్క();
#అయితే_ముగింపు
            CV_LOG_VERBOSE(ఖాళీ, 5, "MainThread: wake worker threads... (done)");

            {
                Parallel_పని& j = *(ఇది->పని);
#నిర్వచితమైతే CV_PROFILE_దారాలు
                దారాలు_stat[0].thread_Execute_ప్రారంభం = get_Tick_లెక్క();
                దారాలు_stat[0].executedTasks = j.execute(అబద్ధం);
                దారాలు_stat[0].threadExecuteStop = get_Tick_లెక్క();
#లేకపోతే
                j.execute(అబద్ధం);
#అయితే_ముగింపు
                CV_Assert(j.current_task >= j.range.size());
                CV_LOG_VERBOSE(ఖాళీ, 5, "MainThread: complete self-tasks: " << j.చురుకైన_thread_లెక్క << " " << j.completed_thread_లెక్క);
                ఒకవేళ (పని->is_completed || j.చురుకైన_thread_లెక్క == 0)
                {
                    పని->is_completed = నిజం;
                    CV_LOG_VERBOSE(ఖాళీ, 5, "MainThread: no WIP worker threads");
                }
                లేకపోతే
                {
                    ఒకవేళ (CV_MAIN_THREAD_చురుకైన_WAIT > 0)
                    {
                        కొరకు (పూర్ణాంకం i = 0; i < CV_MAIN_THREAD_చురుకైన_WAIT; i++)  // don't spin too much in any case (inaccurate getTickCount())
                        {
                            ఒకవేళ (పని->is_completed)
                            {
                                CV_LOG_VERBOSE(ఖాళీ, 5, "MainThread: job finalize (active wait) " << j.చురుకైన_thread_లెక్క << " " << j.completed_thread_లెక్క);
                                ఆపు;
                            }
                            ఒకవేళ (CV_చురుకైన_WAIT_PAUSE_హద్దు > 0 && (i < CV_చురుకైన_WAIT_PAUSE_హద్దు || (i & 1)))
                                CV_PAUSE(16);
                            లేకపోతే
                                CV_YIELD();
                        }
                    }
                    ఒకవేళ (!పని->is_completed)
                    {
                        CV_LOG_VERBOSE(ఖాళీ, 5, "MainThread: prepare wait " << j.చురుకైన_thread_లెక్క << " " << j.completed_thread_లెక్క);
                        pthread_mutex_lock(&mutex_notify);
                        కొరకు (;;)
                        {
                            ఒకవేళ (పని->is_completed)
                            {
                                CV_LOG_VERBOSE(ఖాళీ, 5, "MainThread: job finalize (wait) " << j.చురుకైన_thread_లెక్క << " " << j.completed_thread_లెక్క);
                                ఆపు;
                            }
                            CV_LOG_VERBOSE(ఖాళీ, 5, "MainThread: wait completion (sleep) ...");
                            pthread_cond_wait(&cond_thread_task_complete, &mutex_notify);
                            CV_LOG_VERBOSE(ఖాళీ, 5, "MainThread: wake");
                        }
                        pthread_mutex_unlock(&mutex_notify);
                    }
                }
            }
#నిర్వచితమైతే CV_PROFILE_దారాలు
            దారాలు_stat[0].threadFree = get_Tick_లెక్క();
            std::బయటకు << "Job: sz=" << range.size() << " nstripes=" << nstripes << "    Time: " << (దారాలు_stat[0].threadFree - పని_Submit_సమయం) / tickFreq * 1e6 << " usec" << std::పంక్తి_ముగింపు;
            కొరకు (పూర్ణాంకం i = 0; i < (పూర్ణాంకం)దారాలు.size() + 1; i++)
            {
                దారాలు_stat[i].dump(i - 1, పని_Submit_సమయం, tickFreq);
            }
#అయితే_ముగింపు
            ఒకవేళ (పని)
            {
                pthread_mutex_lock(&mutex);
                CV_LOG_VERBOSE(ఖాళీ, 5, "MainThread: job release");
                CV_Assert(పని->is_completed);
                పని.release();
                pthread_mutex_unlock(&mutex);
            }
        }
    }
    లేకపోతే
    {
        body(range);
    }
}

size_t ThreadPool::get_అంకె_యొక్క_దారాలు()
{
    తిరిగి అంకె_దారాలు_2;
}

శూన్య ThreadPool::సమితి_అంకె_యొక్క_దారాలు(గుర్తులేని n)
{
    ఒకవేళ (n != అంకె_దారాలు_2)
    {
        అంకె_దారాలు_2 = n;
        ఒకవేళ (n == 1)
           ఒకవేళ (పని == ఖాళీ) reconfigure(0);  // stop worker threads immediately
    }
}

size_t parallel_pthreads_get_దారాలు_అంకె()
{
    తిరిగి ThreadPool::instance().get_అంకె_యొక్క_దారాలు();
}

శూన్య parallel_pthreads_సమితి_దారాలు_అంకె(పూర్ణాంకం num)
{
    ఒకవేళ(num < 0)
    {
        ThreadPool::instance().సమితి_అంకె_యొక్క_దారాలు(0);
    }
    లేకపోతే
    {
        ThreadPool::instance().సమితి_అంకె_యొక్క_దారాలు(గుర్తులేని(num));
    }
}

శూన్య parallel_for_pthreads(స్థిరరాశి Range& range, స్థిరరాశి ParallelLoopBody& body, ద్విగుణ nstripes)
{
    ThreadPool::instance().నిరంతరం(range, body, nstripes);
}

}

#అయితే_ముగింపు
