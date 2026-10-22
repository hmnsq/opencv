// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#شامل "precomp.hpp"
#شامل "opencv2/core/async.hpp"
#شامل "opencv2/core/detail/async_promise.hpp"

#شامل "opencv2/core/cvstd.hpp"

#شامل <opencv2/core/utils/logger.defines.hpp>
#تعریف_منسوخ CV_LOG_STRIP_LEVEL
#تعریف_کرو CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_DEBUG + 1
#شامل <opencv2/core/utils/logger.hpp>

#اگر_غیر_تعریف_شدہ OPENCV_DISABLE_THREAD_SUPPORT

#شامل <mutex>
#شامل <condition_variable>
#شامل <chrono>

نامخلا cv {

/**
Manages shared state of asynchronous result
*/
ساخت AsyncArray::Impl
{
    صحیح refcount;
    خالی addrefFuture() CV_NOEXCEPT { CV_XADD(&refcount_future, 1); CV_XADD(&refcount, 1); } \
    خالی releaseFuture() CV_NOEXCEPT { CV_XADD(&refcount_future, -1); اگر(1 == CV_XADD(&refcount, -1)) حذف یہ; } \
    صحیح refcount_future;
    خالی addrefPromise() CV_NOEXCEPT { CV_XADD(&refcount_promise, 1); CV_XADD(&refcount, 1); } \
    خالی releasePromise() CV_NOEXCEPT { CV_XADD(&refcount_promise, -1); اگر(1 == CV_XADD(&refcount, -1)) حذف یہ; } \
    صحیح refcount_promise;

    تغیرپذیر std::mutex mtx;
    تغیرپذیر std::condition_variable cond_var;

    تغیرپذیر منطقی has_نتیجہ; // Mat, UMat or exception

    تغیرپذیر cv::Ptr<Mat> نتیجہ_mat;
    تغیرپذیر cv::Ptr<UMat> نتیجہ_umat;


    منطقی has_exception;
#اگر CV__EXCEPTION_PTR
    std::exception_ptr exception;
#اگر_ختم
    cv::Exception cv_exception;

    تغیرپذیر منطقی نتیجہ_is_fetched;

    منطقی future_is_returned;

    Impl()
        : refcount(1), refcount_future(0), refcount_promise(1)
        , has_نتیجہ(جھوٹ)
        , has_exception(جھوٹ)
        , نتیجہ_is_fetched(جھوٹ)
        , future_is_returned(جھوٹ)
    {
        // nothing
    }

    ~Impl()
    {
        اگر (has_نتیجہ && !نتیجہ_is_fetched)
        {
            CV_LOG_INFO(تہی, "Asynchronous result has not been fetched");
        }
    }

    منطقی get(OutputArray dst, int64 timeoutNs) ثابت
    {
        CV_Assert(!نتیجہ_is_fetched);
        اگر (!has_نتیجہ)
        {
            اگر(refcount_promise == 0)
                CV_Error(Error::Sts_اندرونی, "Asynchronous result producer has been destroyed");
            اگر (!wait_for(timeoutNs))
                واپس جھوٹ;
        }
        std::unique_lock<std::mutex> lock(mtx);
        اگر (has_نتیجہ)
        {
            اگر (!نتیجہ_mat.empty())
            {
                dst.move(*نتیجہ_mat.get());
                نتیجہ_mat.release();
                نتیجہ_is_fetched = سچ;
                واپس سچ;
            }
            اگر (!نتیجہ_umat.empty())
            {
                dst.move(*نتیجہ_umat.get());
                نتیجہ_umat.release();
                نتیجہ_is_fetched = سچ;
                واپس سچ;
            }
#اگر CV__EXCEPTION_PTR
            اگر (has_exception && exception)
            {
                نتیجہ_is_fetched = سچ;
                std::rethrow_exception(exception);
            }
#اگر_ختم
            اگر (has_exception)
            {
                نتیجہ_is_fetched = سچ;
                پھینکو cv_exception;
            }
            CV_Error(Error::Sts_اندرونی, "AsyncArray: invalid state of 'has_result = true'");
        }
        CV_Assert(!has_نتیجہ);
        CV_Assert(timeoutNs < 0);
        واپس جھوٹ;
    }

    منطقی valid() ثابت CV_NOEXCEPT
    {
        اگر (نتیجہ_is_fetched)
            واپس جھوٹ;
        اگر (refcount_promise == 0 && !has_نتیجہ)
            واپس جھوٹ;
        واپس سچ;
    }

    منطقی wait_for(int64 timeoutNs) ثابت
    {
        CV_Assert(valid());
        اگر (has_نتیجہ)
            واپس has_نتیجہ;
        اگر (timeoutNs == 0)
            واپس has_نتیجہ;
        CV_LOG_INFO(تہی, "Waiting for async result ...");
        std::unique_lock<std::mutex> lock(mtx);
        ثابت خودکار cond_pred = [&]{ واپس has_نتیجہ == سچ; };
        اگر (timeoutNs > 0)
            واپس cond_var.wait_for(lock, std::chrono::nanoseconds(timeoutNs), cond_pred);
        ورنہ
        {
            cond_var.wait(lock, cond_pred);
            CV_Assert(has_نتیجہ);
            واپس سچ;
        }
    }

    AsyncArray get_Array_نتیجہ()
    {
        CV_Assert(refcount_future == 0);
        AsyncArray نتیجہ;
        addrefFuture();
        نتیجہ.p = یہ;
        future_is_returned = سچ;
        واپس نتیجہ;
    }

    خالی سیٹ_قدر(داخلہ_Array value)
    {
        اگر (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        std::unique_lock<std::mutex> lock(mtx);
        CV_Assert(!has_نتیجہ);
        صحیح k = value.قسم();
        اگر (k == داخلہ_Array_2::UMAT)
        {
            نتیجہ_umat = makePtr<UMat>();
            value.copy_مقصد(*نتیجہ_umat.get());
        }
        ورنہ
        {
            نتیجہ_mat = makePtr<Mat>();
            value.copy_مقصد(*نتیجہ_mat.get());
        }
        has_نتیجہ = سچ;
        cond_var.notify_all();
    }

#اگر CV__EXCEPTION_PTR
    خالی سیٹ_Exception(std::exception_ptr e)
    {
        اگر (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        std::unique_lock<std::mutex> lock(mtx);
        CV_Assert(!has_نتیجہ);
        has_exception = سچ;
        exception = e;
        has_نتیجہ = سچ;
        cond_var.notify_all();
    }
#اگر_ختم

    خالی سیٹ_Exception(ثابت cv::Exception e)
    {
        اگر (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        std::unique_lock<std::mutex> lock(mtx);
        CV_Assert(!has_نتیجہ);
        has_exception = سچ;
        cv_exception = e;
        has_نتیجہ = سچ;
        cond_var.notify_all();
    }
};

}  // namespace

#ورنہ  // OPENCV_DISABLE_THREAD_SUPPORT

نامخلا cv {

// no threading
ساخت AsyncArray::Impl
{
    صحیح refcount;
    خالی addrefFuture() CV_NOEXCEPT { refcount_future++; refcount++; }
    خالی releaseFuture() CV_NOEXCEPT { refcount_future--; اگر (0 == --refcount) حذف یہ; }
    صحیح refcount_future;
    خالی addrefPromise() CV_NOEXCEPT { refcount_promise++; refcount++; } \
    خالی releasePromise() CV_NOEXCEPT { refcount_promise--; اگر (0 == --refcount) حذف یہ; }
    صحیح refcount_promise;

    تغیرپذیر منطقی has_نتیجہ; // Mat, UMat or exception

    تغیرپذیر cv::Ptr<Mat> نتیجہ_mat;
    تغیرپذیر cv::Ptr<UMat> نتیجہ_umat;


    منطقی has_exception;
#اگر CV__EXCEPTION_PTR
    std::exception_ptr exception;
#اگر_ختم
    cv::Exception cv_exception;

    تغیرپذیر منطقی نتیجہ_is_fetched;

    منطقی future_is_returned;

    Impl()
        : refcount(1), refcount_future(0), refcount_promise(1)
        , has_نتیجہ(جھوٹ)
        , has_exception(جھوٹ)
        , نتیجہ_is_fetched(جھوٹ)
        , future_is_returned(جھوٹ)
    {
        // nothing
    }

    ~Impl()
    {
        اگر (has_نتیجہ && !نتیجہ_is_fetched)
        {
            CV_LOG_INFO(تہی, "Asynchronous result has not been fetched");
        }
    }

    منطقی get(OutputArray dst, int64 timeoutNs) ثابت
    {
        CV_Assert(!نتیجہ_is_fetched);
        اگر (!has_نتیجہ)
        {
            CV_غیر_مستعمل(timeoutNs);
            CV_Error(Error::StsError, "Result is not produced (unable to wait for result in OPENCV_DISABLE_THREAD_SUPPORT mode)");
        }
        اگر (!نتیجہ_mat.empty())
        {
            dst.move(*نتیجہ_mat.get());
            نتیجہ_mat.release();
            نتیجہ_is_fetched = سچ;
            واپس سچ;
        }
        اگر (!نتیجہ_umat.empty())
        {
            dst.move(*نتیجہ_umat.get());
            نتیجہ_umat.release();
            نتیجہ_is_fetched = سچ;
            واپس سچ;
        }
#اگر CV__EXCEPTION_PTR
        اگر (has_exception && exception)
        {
            نتیجہ_is_fetched = سچ;
            std::rethrow_exception(exception);
        }
#اگر_ختم
        اگر (has_exception)
        {
            نتیجہ_is_fetched = سچ;
            پھینکو cv_exception;
        }
        CV_Error(Error::Sts_اندرونی, "AsyncArray: invalid state of 'has_result = true'");
        واپس جھوٹ;
    }

    منطقی valid() ثابت CV_NOEXCEPT
    {
        اگر (نتیجہ_is_fetched)
            واپس جھوٹ;
        اگر (refcount_promise == 0 && !has_نتیجہ)
            واپس جھوٹ;
        واپس سچ;
    }

    منطقی wait_for(int64 timeoutNs) ثابت
    {
        CV_Assert(valid());
        اگر (has_نتیجہ)
            واپس has_نتیجہ;
        اگر (timeoutNs == 0)
            واپس has_نتیجہ;
        CV_Error(Error::StsError, "Unable to wait in OPENCV_DISABLE_THREAD_SUPPORT mode");
    }

    AsyncArray get_Array_نتیجہ()
    {
        CV_Assert(refcount_future == 0);
        AsyncArray نتیجہ;
        addrefFuture();
        نتیجہ.p = یہ;
        future_is_returned = سچ;
        واپس نتیجہ;
    }

    خالی سیٹ_قدر(داخلہ_Array value)
    {
        اگر (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        CV_Assert(!has_نتیجہ);
        صحیح k = value.قسم();
        اگر (k == داخلہ_Array_2::UMAT)
        {
            نتیجہ_umat = makePtr<UMat>();
            value.copy_مقصد(*نتیجہ_umat.get());
        }
        ورنہ
        {
            نتیجہ_mat = makePtr<Mat>();
            value.copy_مقصد(*نتیجہ_mat.get());
        }
        has_نتیجہ = سچ;
    }

#اگر CV__EXCEPTION_PTR
    خالی سیٹ_Exception(std::exception_ptr e)
    {
        اگر (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        CV_Assert(!has_نتیجہ);
        has_exception = سچ;
        exception = e;
        has_نتیجہ = سچ;
    }
#اگر_ختم

    خالی سیٹ_Exception(ثابت cv::Exception e)
    {
        اگر (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        CV_Assert(!has_نتیجہ);
        has_exception = سچ;
        cv_exception = e;
        has_نتیجہ = سچ;
    }
};

}

#اگر_ختم  // OPENCV_DISABLE_THREAD_SUPPORT

نامخلا cv {

AsyncArray::AsyncArray() CV_NOEXCEPT
    : p(تہی)
{
}

AsyncArray::~AsyncArray() CV_NOEXCEPT
{
    release();
}

AsyncArray::AsyncArray(ثابت AsyncArray& o) CV_NOEXCEPT
    : p(o.p)
{
    اگر (p)
        p->addrefFuture();
}

AsyncArray& AsyncArray::عامل=(ثابت AsyncArray& o) CV_NOEXCEPT
{
    Impl* newp = o.p;
    اگر (newp)
        newp->addrefFuture();
    release();
    p = newp;
    واپس *یہ;
}

خالی AsyncArray::release() CV_NOEXCEPT
{
    Impl* impl = p;
    p = تہی;
    اگر (impl)
        impl->releaseFuture();
}

منطقی AsyncArray::get(OutputArray dst, int64 timeoutNs) ثابت
{
    CV_Assert(p);
    واپس p->get(dst, timeoutNs);
}

خالی AsyncArray::get(OutputArray dst) ثابت
{
    CV_Assert(p);
    منطقی res = p->get(dst, -1);
    CV_Assert(res);
}

منطقی AsyncArray::wait_for(int64 timeoutNs) ثابت
{
    CV_Assert(p);
    واپس p->wait_for(timeoutNs);
}

منطقی AsyncArray::valid() ثابت CV_NOEXCEPT
{
    اگر (!p) واپس جھوٹ;
    واپس p->valid();
}


//
// AsyncPromise
//

AsyncPromise::AsyncPromise() CV_NOEXCEPT
    : p(نیا AsyncArray::Impl())
{
}

AsyncPromise::~AsyncPromise() CV_NOEXCEPT
{
    release();
}

AsyncPromise::AsyncPromise(ثابت AsyncPromise& o) CV_NOEXCEPT
    : p(o.p)
{
    اگر (p)
        p->addrefPromise();
}

AsyncPromise& AsyncPromise::عامل=(ثابت AsyncPromise& o) CV_NOEXCEPT
{
    Impl* newp = o.p;
    اگر (newp)
        newp->addrefPromise();
    release();
    p = newp;
    واپس *یہ;
}

خالی AsyncPromise::release() CV_NOEXCEPT
{
    Impl* impl = p;
    p = تہی;
    اگر (impl)
        impl->releasePromise();
}

AsyncArray AsyncPromise::get_Array_نتیجہ()
{
    CV_Assert(p);
    واپس p->get_Array_نتیجہ();
}

خالی AsyncPromise::سیٹ_قدر(داخلہ_Array value)
{
    CV_Assert(p);
    واپس p->سیٹ_قدر(value);
}

خالی AsyncPromise::سیٹ_Exception(ثابت cv::Exception& exception)
{
    CV_Assert(p);
    واپس p->سیٹ_Exception(exception);
}

#اگر CV__EXCEPTION_PTR
خالی AsyncPromise::سیٹ_Exception(std::exception_ptr exception)
{
    CV_Assert(p);
    واپس p->سیٹ_Exception(exception);
}
#اگر_ختم

} // namespace
