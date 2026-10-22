// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#شامل "precomp.hpp"
#شامل "opencv2/core/async.hpp"
#شامل "opencv2/core/detail/async_promise.hpp"

#شامل "opencv2/core/cvstd.hpp"

#شامل <opencv2/core/utils/logger.defines.hpp>
#undef CV_LOG_STRIP_LEVEL
#define CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_DEBUG + 1
#شامل <opencv2/core/utils/logger.hpp>

#ifndef OPENCV_DISABLE_THREAD_SUPPORT

#شامل <mutex>
#شامل <condition_variable>
#شامل <chrono>

فضاینام cv {

/**
Manages shared state of asynchronous result
*/
ساختار AsyncArray::Impl
{
    صحیح refcount;
    تهی addrefFuture() CV_NOEXCEPT { CV_XADD(&refcount_future, 1); CV_XADD(&refcount, 1); } \
    تهی releaseFuture() CV_NOEXCEPT { CV_XADD(&refcount_future, -1); اگر(1 == CV_XADD(&refcount, -1)) delete این; } \
    صحیح refcount_future;
    تهی addrefPromise() CV_NOEXCEPT { CV_XADD(&refcount_promise, 1); CV_XADD(&refcount, 1); } \
    تهی releasePromise() CV_NOEXCEPT { CV_XADD(&refcount_promise, -1); اگر(1 == CV_XADD(&refcount, -1)) delete این; } \
    صحیح refcount_promise;

    mutable std::mutex mtx;
    mutable std::condition_variable cond_var;

    mutable منطقی has_result; // Mat, UMat or exception

    mutable cv::Ptr<Mat> result_mat;
    mutable cv::Ptr<UMat> result_umat;


    منطقی has_exception;
#اگر CV__EXCEPTION_PTR
    std::exception_ptr exception;
#endif
    cv::Exception cv_exception;

    mutable منطقی result_is_fetched;

    منطقی future_is_returned;

    Impl()
        : refcount(1), refcount_future(0), refcount_promise(1)
        , has_result(نادرست)
        , has_exception(نادرست)
        , result_is_fetched(نادرست)
        , future_is_returned(نادرست)
    {
        // nothing
    }

    ~Impl()
    {
        اگر (has_result && !result_is_fetched)
        {
            CV_LOG_INFO(پوچ, "Asynchronous result has not been fetched");
        }
    }

    منطقی get(OutputArray dst, int64 timeoutNs) ثابت
    {
        CV_Assert(!result_is_fetched);
        اگر (!has_result)
        {
            اگر(refcount_promise == 0)
                CV_Error(Error::StsInternal, "Asynchronous result producer has been destroyed");
            اگر (!wait_for(timeoutNs))
                بازگشت نادرست;
        }
        std::unique_lock<std::mutex> lock(mtx);
        اگر (has_result)
        {
            اگر (!result_mat.empty())
            {
                dst.move(*result_mat.get());
                result_mat.release();
                result_is_fetched = درست;
                بازگشت درست;
            }
            اگر (!result_umat.empty())
            {
                dst.move(*result_umat.get());
                result_umat.release();
                result_is_fetched = درست;
                بازگشت درست;
            }
#اگر CV__EXCEPTION_PTR
            اگر (has_exception && exception)
            {
                result_is_fetched = درست;
                std::rethrow_exception(exception);
            }
#endif
            اگر (has_exception)
            {
                result_is_fetched = درست;
                پرتابکن cv_exception;
            }
            CV_Error(Error::StsInternal, "AsyncArray: invalid state of 'has_result = true'");
        }
        CV_Assert(!has_result);
        CV_Assert(timeoutNs < 0);
        بازگشت نادرست;
    }

    منطقی valid() ثابت CV_NOEXCEPT
    {
        اگر (result_is_fetched)
            بازگشت نادرست;
        اگر (refcount_promise == 0 && !has_result)
            بازگشت نادرست;
        بازگشت درست;
    }

    منطقی wait_for(int64 timeoutNs) ثابت
    {
        CV_Assert(valid());
        اگر (has_result)
            بازگشت has_result;
        اگر (timeoutNs == 0)
            بازگشت has_result;
        CV_LOG_INFO(پوچ, "Waiting for async result ...");
        std::unique_lock<std::mutex> lock(mtx);
        ثابت auto cond_pred = [&]{ بازگشت has_result == درست; };
        اگر (timeoutNs > 0)
            بازگشت cond_var.wait_for(lock, std::chrono::nanoseconds(timeoutNs), cond_pred);
        وگرنه
        {
            cond_var.wait(lock, cond_pred);
            CV_Assert(has_result);
            بازگشت درست;
        }
    }

    AsyncArray getArrayResult()
    {
        CV_Assert(refcount_future == 0);
        AsyncArray result;
        addrefFuture();
        result.p = این;
        future_is_returned = درست;
        بازگشت result;
    }

    تهی set_مقدار(درون_داد_Array value)
    {
        اگر (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        std::unique_lock<std::mutex> lock(mtx);
        CV_Assert(!has_result);
        صحیح k = value.kind();
        اگر (k == درون_داد_Array_2::UMAT)
        {
            result_umat = makePtr<UMat>();
            value.copyTo(*result_umat.get());
        }
        وگرنه
        {
            result_mat = makePtr<Mat>();
            value.copyTo(*result_mat.get());
        }
        has_result = درست;
        cond_var.notify_all();
    }

#اگر CV__EXCEPTION_PTR
    تهی setException(std::exception_ptr e)
    {
        اگر (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        std::unique_lock<std::mutex> lock(mtx);
        CV_Assert(!has_result);
        has_exception = درست;
        exception = e;
        has_result = درست;
        cond_var.notify_all();
    }
#endif

    تهی setException(ثابت cv::Exception e)
    {
        اگر (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        std::unique_lock<std::mutex> lock(mtx);
        CV_Assert(!has_result);
        has_exception = درست;
        cv_exception = e;
        has_result = درست;
        cond_var.notify_all();
    }
};

}  // namespace

#وگرنه  // OPENCV_DISABLE_THREAD_SUPPORT

فضاینام cv {

// no threading
ساختار AsyncArray::Impl
{
    صحیح refcount;
    تهی addrefFuture() CV_NOEXCEPT { refcount_future++; refcount++; }
    تهی releaseFuture() CV_NOEXCEPT { refcount_future--; اگر (0 == --refcount) delete این; }
    صحیح refcount_future;
    تهی addrefPromise() CV_NOEXCEPT { refcount_promise++; refcount++; } \
    تهی releasePromise() CV_NOEXCEPT { refcount_promise--; اگر (0 == --refcount) delete این; }
    صحیح refcount_promise;

    mutable منطقی has_result; // Mat, UMat or exception

    mutable cv::Ptr<Mat> result_mat;
    mutable cv::Ptr<UMat> result_umat;


    منطقی has_exception;
#اگر CV__EXCEPTION_PTR
    std::exception_ptr exception;
#endif
    cv::Exception cv_exception;

    mutable منطقی result_is_fetched;

    منطقی future_is_returned;

    Impl()
        : refcount(1), refcount_future(0), refcount_promise(1)
        , has_result(نادرست)
        , has_exception(نادرست)
        , result_is_fetched(نادرست)
        , future_is_returned(نادرست)
    {
        // nothing
    }

    ~Impl()
    {
        اگر (has_result && !result_is_fetched)
        {
            CV_LOG_INFO(پوچ, "Asynchronous result has not been fetched");
        }
    }

    منطقی get(OutputArray dst, int64 timeoutNs) ثابت
    {
        CV_Assert(!result_is_fetched);
        اگر (!has_result)
        {
            CV_UNUSED(timeoutNs);
            CV_Error(Error::StsError, "Result is not produced (unable to wait for result in OPENCV_DISABLE_THREAD_SUPPORT mode)");
        }
        اگر (!result_mat.empty())
        {
            dst.move(*result_mat.get());
            result_mat.release();
            result_is_fetched = درست;
            بازگشت درست;
        }
        اگر (!result_umat.empty())
        {
            dst.move(*result_umat.get());
            result_umat.release();
            result_is_fetched = درست;
            بازگشت درست;
        }
#اگر CV__EXCEPTION_PTR
        اگر (has_exception && exception)
        {
            result_is_fetched = درست;
            std::rethrow_exception(exception);
        }
#endif
        اگر (has_exception)
        {
            result_is_fetched = درست;
            پرتابکن cv_exception;
        }
        CV_Error(Error::StsInternal, "AsyncArray: invalid state of 'has_result = true'");
        بازگشت نادرست;
    }

    منطقی valid() ثابت CV_NOEXCEPT
    {
        اگر (result_is_fetched)
            بازگشت نادرست;
        اگر (refcount_promise == 0 && !has_result)
            بازگشت نادرست;
        بازگشت درست;
    }

    منطقی wait_for(int64 timeoutNs) ثابت
    {
        CV_Assert(valid());
        اگر (has_result)
            بازگشت has_result;
        اگر (timeoutNs == 0)
            بازگشت has_result;
        CV_Error(Error::StsError, "Unable to wait in OPENCV_DISABLE_THREAD_SUPPORT mode");
    }

    AsyncArray getArrayResult()
    {
        CV_Assert(refcount_future == 0);
        AsyncArray result;
        addrefFuture();
        result.p = این;
        future_is_returned = درست;
        بازگشت result;
    }

    تهی set_مقدار(درون_داد_Array value)
    {
        اگر (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        CV_Assert(!has_result);
        صحیح k = value.kind();
        اگر (k == درون_داد_Array_2::UMAT)
        {
            result_umat = makePtr<UMat>();
            value.copyTo(*result_umat.get());
        }
        وگرنه
        {
            result_mat = makePtr<Mat>();
            value.copyTo(*result_mat.get());
        }
        has_result = درست;
    }

#اگر CV__EXCEPTION_PTR
    تهی setException(std::exception_ptr e)
    {
        اگر (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        CV_Assert(!has_result);
        has_exception = درست;
        exception = e;
        has_result = درست;
    }
#endif

    تهی setException(ثابت cv::Exception e)
    {
        اگر (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        CV_Assert(!has_result);
        has_exception = درست;
        cv_exception = e;
        has_result = درست;
    }
};

}

#endif  // OPENCV_DISABLE_THREAD_SUPPORT

فضاینام cv {

AsyncArray::AsyncArray() CV_NOEXCEPT
    : p(پوچ)
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

AsyncArray& AsyncArray::operator=(ثابت AsyncArray& o) CV_NOEXCEPT
{
    Impl* newp = o.p;
    اگر (newp)
        newp->addrefFuture();
    release();
    p = newp;
    بازگشت *این;
}

تهی AsyncArray::release() CV_NOEXCEPT
{
    Impl* impl = p;
    p = پوچ;
    اگر (impl)
        impl->releaseFuture();
}

منطقی AsyncArray::get(OutputArray dst, int64 timeoutNs) ثابت
{
    CV_Assert(p);
    بازگشت p->get(dst, timeoutNs);
}

تهی AsyncArray::get(OutputArray dst) ثابت
{
    CV_Assert(p);
    منطقی res = p->get(dst, -1);
    CV_Assert(res);
}

منطقی AsyncArray::wait_for(int64 timeoutNs) ثابت
{
    CV_Assert(p);
    بازگشت p->wait_for(timeoutNs);
}

منطقی AsyncArray::valid() ثابت CV_NOEXCEPT
{
    اگر (!p) بازگشت نادرست;
    بازگشت p->valid();
}


//
// AsyncPromise
//

AsyncPromise::AsyncPromise() CV_NOEXCEPT
    : p(جدید AsyncArray::Impl())
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

AsyncPromise& AsyncPromise::operator=(ثابت AsyncPromise& o) CV_NOEXCEPT
{
    Impl* newp = o.p;
    اگر (newp)
        newp->addrefPromise();
    release();
    p = newp;
    بازگشت *این;
}

تهی AsyncPromise::release() CV_NOEXCEPT
{
    Impl* impl = p;
    p = پوچ;
    اگر (impl)
        impl->releasePromise();
}

AsyncArray AsyncPromise::getArrayResult()
{
    CV_Assert(p);
    بازگشت p->getArrayResult();
}

تهی AsyncPromise::set_مقدار(درون_داد_Array value)
{
    CV_Assert(p);
    بازگشت p->set_مقدار(value);
}

تهی AsyncPromise::setException(ثابت cv::Exception& exception)
{
    CV_Assert(p);
    بازگشت p->setException(exception);
}

#اگر CV__EXCEPTION_PTR
تهی AsyncPromise::setException(std::exception_ptr exception)
{
    CV_Assert(p);
    بازگشت p->setException(exception);
}
#endif

} // namespace
