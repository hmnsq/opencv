// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ضمن "precomp.hpp"
#ضمن "opencv2/core/async.hpp"
#ضمن "opencv2/core/detail/async_promise.hpp"

#ضمن "opencv2/core/cvstd.hpp"

#ضمن <opencv2/core/utils/logger.defines.hpp>
#وصف_رد CV_LOG_STRIP_LEVEL
#وصف_ڪريو CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_DEBUG + 1
#ضمن <opencv2/core/utils/logger.hpp>

#جيڪڏهن_وصف_نه_ٿيل OPENCV_DISABLE_THREAD_SUPPORT

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
    فراغ releaseFuture() CV_NOEXCEPT { CV_XADD(&refcount_future, -1); إذا(1 == CV_XADD(&refcount, -1)) ميٽيو هذا; } \
    صحيح refcount_future;
    فراغ addrefPromise() CV_NOEXCEPT { CV_XADD(&refcount_promise, 1); CV_XADD(&refcount, 1); } \
    فراغ releasePromise() CV_NOEXCEPT { CV_XADD(&refcount_promise, -1); إذا(1 == CV_XADD(&refcount, -1)) ميٽيو هذا; } \
    صحيح refcount_promise;

    بدلجندڙ std::mutex mtx;
    بدلجندڙ std::condition_variable cond_var;

    بدلجندڙ منطقي has_نتيجو; // Mat, UMat or exception

    بدلجندڙ cv::Ptr<Mat> نتيجو_mat;
    بدلجندڙ cv::Ptr<UMat> نتيجو_umat;


    منطقي has_exception;
#إذا CV__EXCEPTION_PTR
    std::exception_ptr exception;
#جيڪڏهن_پورو
    cv::Exception cv_exception;

    بدلجندڙ منطقي نتيجو_is_fetched;

    منطقي future_is_returned;

    Impl()
        : refcount(1), refcount_future(0), refcount_promise(1)
        , has_نتيجو(خطأ)
        , has_exception(خطأ)
        , نتيجو_is_fetched(خطأ)
        , future_is_returned(خطأ)
    {
        // nothing
    }

    ~Impl()
    {
        إذا (has_نتيجو && !نتيجو_is_fetched)
        {
            CV_LOG_INFO(عدم, "Asynchronous result has not been fetched");
        }
    }

    منطقي get(OutputArray dst, int64 timeoutNs) ثابت
    {
        CV_Assert(!نتيجو_is_fetched);
        إذا (!has_نتيجو)
        {
            إذا(refcount_promise == 0)
                CV_Error(Error::Sts_اندروني, "Asynchronous result producer has been destroyed");
            إذا (!wait_for(timeoutNs))
                أرجع خطأ;
        }
        std::unique_lock<std::mutex> lock(mtx);
        إذا (has_نتيجو)
        {
            إذا (!نتيجو_mat.empty())
            {
                dst.move(*نتيجو_mat.get());
                نتيجو_mat.release();
                نتيجو_is_fetched = سچ;
                أرجع سچ;
            }
            إذا (!نتيجو_umat.empty())
            {
                dst.move(*نتيجو_umat.get());
                نتيجو_umat.release();
                نتيجو_is_fetched = سچ;
                أرجع سچ;
            }
#إذا CV__EXCEPTION_PTR
            إذا (has_exception && exception)
            {
                نتيجو_is_fetched = سچ;
                std::rethrow_exception(exception);
            }
#جيڪڏهن_پورو
            إذا (has_exception)
            {
                نتيجو_is_fetched = سچ;
                ارم cv_exception;
            }
            CV_Error(Error::Sts_اندروني, "AsyncArray: invalid state of 'has_result = true'");
        }
        CV_Assert(!has_نتيجو);
        CV_Assert(timeoutNs < 0);
        أرجع خطأ;
    }

    منطقي valid() ثابت CV_NOEXCEPT
    {
        إذا (نتيجو_is_fetched)
            أرجع خطأ;
        إذا (refcount_promise == 0 && !has_نتيجو)
            أرجع خطأ;
        أرجع سچ;
    }

    منطقي wait_for(int64 timeoutNs) ثابت
    {
        CV_Assert(valid());
        إذا (has_نتيجو)
            أرجع has_نتيجو;
        إذا (timeoutNs == 0)
            أرجع has_نتيجو;
        CV_LOG_INFO(عدم, "Waiting for async result ...");
        std::unique_lock<std::mutex> lock(mtx);
        ثابت خودڪار cond_pred = [&]{ أرجع has_نتيجو == سچ; };
        إذا (timeoutNs > 0)
            أرجع cond_var.wait_for(lock, std::chrono::nanoseconds(timeoutNs), cond_pred);
        وإلا
        {
            cond_var.wait(lock, cond_pred);
            CV_Assert(has_نتيجو);
            أرجع سچ;
        }
    }

    AsyncArray get_Array_نتيجو()
    {
        CV_Assert(refcount_future == 0);
        AsyncArray نتيجو;
        addrefFuture();
        نتيجو.p = هذا;
        future_is_returned = سچ;
        أرجع نتيجو;
    }

    فراغ سيٽ_قدر(داخلا_Array value)
    {
        إذا (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        std::unique_lock<std::mutex> lock(mtx);
        CV_Assert(!has_نتيجو);
        صحيح k = value.قسم();
        إذا (k == داخلا_Array_2::UMAT)
        {
            نتيجو_umat = makePtr<UMat>();
            value.copy_منزل(*نتيجو_umat.get());
        }
        وإلا
        {
            نتيجو_mat = makePtr<Mat>();
            value.copy_منزل(*نتيجو_mat.get());
        }
        has_نتيجو = سچ;
        cond_var.notify_all();
    }

#إذا CV__EXCEPTION_PTR
    فراغ سيٽ_Exception(std::exception_ptr e)
    {
        إذا (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        std::unique_lock<std::mutex> lock(mtx);
        CV_Assert(!has_نتيجو);
        has_exception = سچ;
        exception = e;
        has_نتيجو = سچ;
        cond_var.notify_all();
    }
#جيڪڏهن_پورو

    فراغ سيٽ_Exception(ثابت cv::Exception e)
    {
        إذا (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        std::unique_lock<std::mutex> lock(mtx);
        CV_Assert(!has_نتيجو);
        has_exception = سچ;
        cv_exception = e;
        has_نتيجو = سچ;
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
    فراغ releaseFuture() CV_NOEXCEPT { refcount_future--; إذا (0 == --refcount) ميٽيو هذا; }
    صحيح refcount_future;
    فراغ addrefPromise() CV_NOEXCEPT { refcount_promise++; refcount++; } \
    فراغ releasePromise() CV_NOEXCEPT { refcount_promise--; إذا (0 == --refcount) ميٽيو هذا; }
    صحيح refcount_promise;

    بدلجندڙ منطقي has_نتيجو; // Mat, UMat or exception

    بدلجندڙ cv::Ptr<Mat> نتيجو_mat;
    بدلجندڙ cv::Ptr<UMat> نتيجو_umat;


    منطقي has_exception;
#إذا CV__EXCEPTION_PTR
    std::exception_ptr exception;
#جيڪڏهن_پورو
    cv::Exception cv_exception;

    بدلجندڙ منطقي نتيجو_is_fetched;

    منطقي future_is_returned;

    Impl()
        : refcount(1), refcount_future(0), refcount_promise(1)
        , has_نتيجو(خطأ)
        , has_exception(خطأ)
        , نتيجو_is_fetched(خطأ)
        , future_is_returned(خطأ)
    {
        // nothing
    }

    ~Impl()
    {
        إذا (has_نتيجو && !نتيجو_is_fetched)
        {
            CV_LOG_INFO(عدم, "Asynchronous result has not been fetched");
        }
    }

    منطقي get(OutputArray dst, int64 timeoutNs) ثابت
    {
        CV_Assert(!نتيجو_is_fetched);
        إذا (!has_نتيجو)
        {
            CV_اڻ_استعمال_ٿيل(timeoutNs);
            CV_Error(Error::StsError, "Result is not produced (unable to wait for result in OPENCV_DISABLE_THREAD_SUPPORT mode)");
        }
        إذا (!نتيجو_mat.empty())
        {
            dst.move(*نتيجو_mat.get());
            نتيجو_mat.release();
            نتيجو_is_fetched = سچ;
            أرجع سچ;
        }
        إذا (!نتيجو_umat.empty())
        {
            dst.move(*نتيجو_umat.get());
            نتيجو_umat.release();
            نتيجو_is_fetched = سچ;
            أرجع سچ;
        }
#إذا CV__EXCEPTION_PTR
        إذا (has_exception && exception)
        {
            نتيجو_is_fetched = سچ;
            std::rethrow_exception(exception);
        }
#جيڪڏهن_پورو
        إذا (has_exception)
        {
            نتيجو_is_fetched = سچ;
            ارم cv_exception;
        }
        CV_Error(Error::Sts_اندروني, "AsyncArray: invalid state of 'has_result = true'");
        أرجع خطأ;
    }

    منطقي valid() ثابت CV_NOEXCEPT
    {
        إذا (نتيجو_is_fetched)
            أرجع خطأ;
        إذا (refcount_promise == 0 && !has_نتيجو)
            أرجع خطأ;
        أرجع سچ;
    }

    منطقي wait_for(int64 timeoutNs) ثابت
    {
        CV_Assert(valid());
        إذا (has_نتيجو)
            أرجع has_نتيجو;
        إذا (timeoutNs == 0)
            أرجع has_نتيجو;
        CV_Error(Error::StsError, "Unable to wait in OPENCV_DISABLE_THREAD_SUPPORT mode");
    }

    AsyncArray get_Array_نتيجو()
    {
        CV_Assert(refcount_future == 0);
        AsyncArray نتيجو;
        addrefFuture();
        نتيجو.p = هذا;
        future_is_returned = سچ;
        أرجع نتيجو;
    }

    فراغ سيٽ_قدر(داخلا_Array value)
    {
        إذا (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        CV_Assert(!has_نتيجو);
        صحيح k = value.قسم();
        إذا (k == داخلا_Array_2::UMAT)
        {
            نتيجو_umat = makePtr<UMat>();
            value.copy_منزل(*نتيجو_umat.get());
        }
        وإلا
        {
            نتيجو_mat = makePtr<Mat>();
            value.copy_منزل(*نتيجو_mat.get());
        }
        has_نتيجو = سچ;
    }

#إذا CV__EXCEPTION_PTR
    فراغ سيٽ_Exception(std::exception_ptr e)
    {
        إذا (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        CV_Assert(!has_نتيجو);
        has_exception = سچ;
        exception = e;
        has_نتيجو = سچ;
    }
#جيڪڏهن_پورو

    فراغ سيٽ_Exception(ثابت cv::Exception e)
    {
        إذا (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        CV_Assert(!has_نتيجو);
        has_exception = سچ;
        cv_exception = e;
        has_نتيجو = سچ;
    }
};

}

#جيڪڏهن_پورو  // OPENCV_DISABLE_THREAD_SUPPORT

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

AsyncArray& AsyncArray::عامل=(ثابت AsyncArray& o) CV_NOEXCEPT
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

AsyncPromise& AsyncPromise::عامل=(ثابت AsyncPromise& o) CV_NOEXCEPT
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

AsyncArray AsyncPromise::get_Array_نتيجو()
{
    CV_Assert(p);
    أرجع p->get_Array_نتيجو();
}

فراغ AsyncPromise::سيٽ_قدر(داخلا_Array value)
{
    CV_Assert(p);
    أرجع p->سيٽ_قدر(value);
}

فراغ AsyncPromise::سيٽ_Exception(ثابت cv::Exception& exception)
{
    CV_Assert(p);
    أرجع p->سيٽ_Exception(exception);
}

#إذا CV__EXCEPTION_PTR
فراغ AsyncPromise::سيٽ_Exception(std::exception_ptr exception)
{
    CV_Assert(p);
    أرجع p->سيٽ_Exception(exception);
}
#جيڪڏهن_پورو

} // namespace
