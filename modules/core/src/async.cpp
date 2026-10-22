// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ضمن "precomp.hpp"
#ضمن "opencv2/core/async.hpp"
#ضمن "opencv2/core/detail/async_promise.hpp"

#ضمن "opencv2/core/cvstd.hpp"

#ضمن <opencv2/core/utils/logger.defines.hpp>
#لغو_تعریف CV_LOG_STRIP_LEVEL
#تعریف_کن CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_DEBUG + 1
#ضمن <opencv2/core/utils/logger.hpp>

#اگر_تعریف_نشده OPENCV_DISABLE_THREAD_SUPPORT

#ضمن <mutex>
#ضمن <condition_variable>
#ضمن <chrono>

نطاق cv {

/**
Manages shared state of asynchronous result
*/
بنية AsyncArray::Impl
{
    صحيح refcount;
    فراغ addrefFuture() CV_NOEXCEPT { CV_XADD(&refcount_future, 1); CV_XADD(&refcount, 1); } \
    فراغ releaseFuture() CV_NOEXCEPT { CV_XADD(&refcount_future, -1); إذا(1 == CV_XADD(&refcount, -1)) حذف هذا; } \
    صحيح refcount_future;
    فراغ addrefPromise() CV_NOEXCEPT { CV_XADD(&refcount_promise, 1); CV_XADD(&refcount, 1); } \
    فراغ releasePromise() CV_NOEXCEPT { CV_XADD(&refcount_promise, -1); إذا(1 == CV_XADD(&refcount, -1)) حذف هذا; } \
    صحيح refcount_promise;

    تغییرپذیر std::mutex mtx;
    تغییرپذیر std::condition_variable cond_var;

    تغییرپذیر منطقي has_نتیجه; // Mat, UMat or exception

    تغییرپذیر cv::Ptr<Mat> نتیجه_mat;
    تغییرپذیر cv::Ptr<UMat> نتیجه_umat;


    منطقي has_exception;
#إذا CV__EXCEPTION_PTR
    std::exception_ptr exception;
#پایان_اگر
    cv::Exception cv_exception;

    تغییرپذیر منطقي نتیجه_is_fetched;

    منطقي future_is_returned;

    Impl()
        : refcount(1), refcount_future(0), refcount_promise(1)
        , has_نتیجه(خطأ)
        , has_exception(خطأ)
        , نتیجه_is_fetched(خطأ)
        , future_is_returned(خطأ)
    {
        // nothing
    }

    ~Impl()
    {
        إذا (has_نتیجه && !نتیجه_is_fetched)
        {
            CV_LOG_INFO(عدم, "Asynchronous result has not been fetched");
        }
    }

    منطقي get(OutputArray dst, int64 timeoutNs) ثابت
    {
        CV_Assert(!نتیجه_is_fetched);
        إذا (!has_نتیجه)
        {
            إذا(refcount_promise == 0)
                CV_Error(Error::Sts_داخلی, "Asynchronous result producer has been destroyed");
            إذا (!wait_for(timeoutNs))
                أرجع خطأ;
        }
        std::unique_lock<std::mutex> lock(mtx);
        إذا (has_نتیجه)
        {
            إذا (!نتیجه_mat.empty())
            {
                dst.move(*نتیجه_mat.get());
                نتیجه_mat.release();
                نتیجه_is_fetched = درست;
                أرجع درست;
            }
            إذا (!نتیجه_umat.empty())
            {
                dst.move(*نتیجه_umat.get());
                نتیجه_umat.release();
                نتیجه_is_fetched = درست;
                أرجع درست;
            }
#إذا CV__EXCEPTION_PTR
            إذا (has_exception && exception)
            {
                نتیجه_is_fetched = درست;
                std::rethrow_exception(exception);
            }
#پایان_اگر
            إذا (has_exception)
            {
                نتیجه_is_fetched = درست;
                ارم cv_exception;
            }
            CV_Error(Error::Sts_داخلی, "AsyncArray: invalid state of 'has_result = true'");
        }
        CV_Assert(!has_نتیجه);
        CV_Assert(timeoutNs < 0);
        أرجع خطأ;
    }

    منطقي valid() ثابت CV_NOEXCEPT
    {
        إذا (نتیجه_is_fetched)
            أرجع خطأ;
        إذا (refcount_promise == 0 && !has_نتیجه)
            أرجع خطأ;
        أرجع درست;
    }

    منطقي wait_for(int64 timeoutNs) ثابت
    {
        CV_Assert(valid());
        إذا (has_نتیجه)
            أرجع has_نتیجه;
        إذا (timeoutNs == 0)
            أرجع has_نتیجه;
        CV_LOG_INFO(عدم, "Waiting for async result ...");
        std::unique_lock<std::mutex> lock(mtx);
        ثابت خودکار cond_pred = [&]{ أرجع has_نتیجه == درست; };
        إذا (timeoutNs > 0)
            أرجع cond_var.wait_for(lock, std::chrono::nanoseconds(timeoutNs), cond_pred);
        وإلا
        {
            cond_var.wait(lock, cond_pred);
            CV_Assert(has_نتیجه);
            أرجع درست;
        }
    }

    AsyncArray get_Array_نتیجه()
    {
        CV_Assert(refcount_future == 0);
        AsyncArray نتیجه;
        addrefFuture();
        نتیجه.p = هذا;
        future_is_returned = درست;
        أرجع نتیجه;
    }

    فراغ مجموعه_مقدار(درون_داد_Array value)
    {
        إذا (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        std::unique_lock<std::mutex> lock(mtx);
        CV_Assert(!has_نتیجه);
        صحيح k = value.نوع();
        إذا (k == درون_داد_Array_2::UMAT)
        {
            نتیجه_umat = makePtr<UMat>();
            value.copy_مقصد(*نتیجه_umat.get());
        }
        وإلا
        {
            نتیجه_mat = makePtr<Mat>();
            value.copy_مقصد(*نتیجه_mat.get());
        }
        has_نتیجه = درست;
        cond_var.notify_all();
    }

#إذا CV__EXCEPTION_PTR
    فراغ مجموعه_Exception(std::exception_ptr e)
    {
        إذا (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        std::unique_lock<std::mutex> lock(mtx);
        CV_Assert(!has_نتیجه);
        has_exception = درست;
        exception = e;
        has_نتیجه = درست;
        cond_var.notify_all();
    }
#پایان_اگر

    فراغ مجموعه_Exception(ثابت cv::Exception e)
    {
        إذا (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        std::unique_lock<std::mutex> lock(mtx);
        CV_Assert(!has_نتیجه);
        has_exception = درست;
        cv_exception = e;
        has_نتیجه = درست;
        cond_var.notify_all();
    }
};

}  // namespace

#وإلا  // OPENCV_DISABLE_THREAD_SUPPORT

نطاق cv {

// no threading
بنية AsyncArray::Impl
{
    صحيح refcount;
    فراغ addrefFuture() CV_NOEXCEPT { refcount_future++; refcount++; }
    فراغ releaseFuture() CV_NOEXCEPT { refcount_future--; إذا (0 == --refcount) حذف هذا; }
    صحيح refcount_future;
    فراغ addrefPromise() CV_NOEXCEPT { refcount_promise++; refcount++; } \
    فراغ releasePromise() CV_NOEXCEPT { refcount_promise--; إذا (0 == --refcount) حذف هذا; }
    صحيح refcount_promise;

    تغییرپذیر منطقي has_نتیجه; // Mat, UMat or exception

    تغییرپذیر cv::Ptr<Mat> نتیجه_mat;
    تغییرپذیر cv::Ptr<UMat> نتیجه_umat;


    منطقي has_exception;
#إذا CV__EXCEPTION_PTR
    std::exception_ptr exception;
#پایان_اگر
    cv::Exception cv_exception;

    تغییرپذیر منطقي نتیجه_is_fetched;

    منطقي future_is_returned;

    Impl()
        : refcount(1), refcount_future(0), refcount_promise(1)
        , has_نتیجه(خطأ)
        , has_exception(خطأ)
        , نتیجه_is_fetched(خطأ)
        , future_is_returned(خطأ)
    {
        // nothing
    }

    ~Impl()
    {
        إذا (has_نتیجه && !نتیجه_is_fetched)
        {
            CV_LOG_INFO(عدم, "Asynchronous result has not been fetched");
        }
    }

    منطقي get(OutputArray dst, int64 timeoutNs) ثابت
    {
        CV_Assert(!نتیجه_is_fetched);
        إذا (!has_نتیجه)
        {
            CV_استفادهنشده(timeoutNs);
            CV_Error(Error::StsError, "Result is not produced (unable to wait for result in OPENCV_DISABLE_THREAD_SUPPORT mode)");
        }
        إذا (!نتیجه_mat.empty())
        {
            dst.move(*نتیجه_mat.get());
            نتیجه_mat.release();
            نتیجه_is_fetched = درست;
            أرجع درست;
        }
        إذا (!نتیجه_umat.empty())
        {
            dst.move(*نتیجه_umat.get());
            نتیجه_umat.release();
            نتیجه_is_fetched = درست;
            أرجع درست;
        }
#إذا CV__EXCEPTION_PTR
        إذا (has_exception && exception)
        {
            نتیجه_is_fetched = درست;
            std::rethrow_exception(exception);
        }
#پایان_اگر
        إذا (has_exception)
        {
            نتیجه_is_fetched = درست;
            ارم cv_exception;
        }
        CV_Error(Error::Sts_داخلی, "AsyncArray: invalid state of 'has_result = true'");
        أرجع خطأ;
    }

    منطقي valid() ثابت CV_NOEXCEPT
    {
        إذا (نتیجه_is_fetched)
            أرجع خطأ;
        إذا (refcount_promise == 0 && !has_نتیجه)
            أرجع خطأ;
        أرجع درست;
    }

    منطقي wait_for(int64 timeoutNs) ثابت
    {
        CV_Assert(valid());
        إذا (has_نتیجه)
            أرجع has_نتیجه;
        إذا (timeoutNs == 0)
            أرجع has_نتیجه;
        CV_Error(Error::StsError, "Unable to wait in OPENCV_DISABLE_THREAD_SUPPORT mode");
    }

    AsyncArray get_Array_نتیجه()
    {
        CV_Assert(refcount_future == 0);
        AsyncArray نتیجه;
        addrefFuture();
        نتیجه.p = هذا;
        future_is_returned = درست;
        أرجع نتیجه;
    }

    فراغ مجموعه_مقدار(درون_داد_Array value)
    {
        إذا (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        CV_Assert(!has_نتیجه);
        صحيح k = value.نوع();
        إذا (k == درون_داد_Array_2::UMAT)
        {
            نتیجه_umat = makePtr<UMat>();
            value.copy_مقصد(*نتیجه_umat.get());
        }
        وإلا
        {
            نتیجه_mat = makePtr<Mat>();
            value.copy_مقصد(*نتیجه_mat.get());
        }
        has_نتیجه = درست;
    }

#إذا CV__EXCEPTION_PTR
    فراغ مجموعه_Exception(std::exception_ptr e)
    {
        إذا (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        CV_Assert(!has_نتیجه);
        has_exception = درست;
        exception = e;
        has_نتیجه = درست;
    }
#پایان_اگر

    فراغ مجموعه_Exception(ثابت cv::Exception e)
    {
        إذا (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        CV_Assert(!has_نتیجه);
        has_exception = درست;
        cv_exception = e;
        has_نتیجه = درست;
    }
};

}

#پایان_اگر  // OPENCV_DISABLE_THREAD_SUPPORT

نطاق cv {

AsyncArray::AsyncArray() CV_NOEXCEPT
    : p(عدم)
{
}

AsyncArray::~AsyncArray() CV_NOEXCEPT
{
    release();
}

AsyncArray::AsyncArray(ثابت AsyncArray& o) CV_NOEXCEPT
    : p(o.p)
{
    إذا (p)
        p->addrefFuture();
}

AsyncArray& AsyncArray::عملگر=(ثابت AsyncArray& o) CV_NOEXCEPT
{
    Impl* newp = o.p;
    إذا (newp)
        newp->addrefFuture();
    release();
    p = newp;
    أرجع *هذا;
}

فراغ AsyncArray::release() CV_NOEXCEPT
{
    Impl* impl = p;
    p = عدم;
    إذا (impl)
        impl->releaseFuture();
}

منطقي AsyncArray::get(OutputArray dst, int64 timeoutNs) ثابت
{
    CV_Assert(p);
    أرجع p->get(dst, timeoutNs);
}

فراغ AsyncArray::get(OutputArray dst) ثابت
{
    CV_Assert(p);
    منطقي res = p->get(dst, -1);
    CV_Assert(res);
}

منطقي AsyncArray::wait_for(int64 timeoutNs) ثابت
{
    CV_Assert(p);
    أرجع p->wait_for(timeoutNs);
}

منطقي AsyncArray::valid() ثابت CV_NOEXCEPT
{
    إذا (!p) أرجع خطأ;
    أرجع p->valid();
}


//
// AsyncPromise
//

AsyncPromise::AsyncPromise() CV_NOEXCEPT
    : p(جديد AsyncArray::Impl())
{
}

AsyncPromise::~AsyncPromise() CV_NOEXCEPT
{
    release();
}

AsyncPromise::AsyncPromise(ثابت AsyncPromise& o) CV_NOEXCEPT
    : p(o.p)
{
    إذا (p)
        p->addrefPromise();
}

AsyncPromise& AsyncPromise::عملگر=(ثابت AsyncPromise& o) CV_NOEXCEPT
{
    Impl* newp = o.p;
    إذا (newp)
        newp->addrefPromise();
    release();
    p = newp;
    أرجع *هذا;
}

فراغ AsyncPromise::release() CV_NOEXCEPT
{
    Impl* impl = p;
    p = عدم;
    إذا (impl)
        impl->releasePromise();
}

AsyncArray AsyncPromise::get_Array_نتیجه()
{
    CV_Assert(p);
    أرجع p->get_Array_نتیجه();
}

فراغ AsyncPromise::مجموعه_مقدار(درون_داد_Array value)
{
    CV_Assert(p);
    أرجع p->مجموعه_مقدار(value);
}

فراغ AsyncPromise::مجموعه_Exception(ثابت cv::Exception& exception)
{
    CV_Assert(p);
    أرجع p->مجموعه_Exception(exception);
}

#إذا CV__EXCEPTION_PTR
فراغ AsyncPromise::مجموعه_Exception(std::exception_ptr exception)
{
    CV_Assert(p);
    أرجع p->مجموعه_Exception(exception);
}
#پایان_اگر

} // namespace
