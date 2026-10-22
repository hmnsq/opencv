// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#அந்தர்பாவ "precomp.hpp"
#அந்தர்பாவ "opencv2/core/async.hpp"
#அந்தர்பாவ "opencv2/core/detail/async_promise.hpp"

#அந்தர்பாவ "opencv2/core/cvstd.hpp"

#அந்தர்பாவ <opencv2/core/utils/logger.defines.hpp>
#undef CV_LOG_STRIP_LEVEL
#define CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_DEBUG + 1
#அந்தர்பாவ <opencv2/core/utils/logger.hpp>

#ifndef OPENCV_DISABLE_THREAD_SUPPORT

#அந்தர்பாவ <mutex>
#அந்தர்பாவ <condition_variable>
#அந்தர்பாவ <chrono>

நாமாகாஶ cv {

/**
Manages shared state of asynchronous result
*/
கட்டமைப்பு AsyncArray::Impl
{
    முழு refcount;
    வெறுமை addrefFuture() CV_NOEXCEPT { CV_XADD(&refcount_future, 1); CV_XADD(&refcount, 1); } \
    வெறுமை releaseFuture() CV_NOEXCEPT { CV_XADD(&refcount_future, -1); எனில்(1 == CV_XADD(&refcount, -1)) delete இது; } \
    முழு refcount_future;
    வெறுமை addrefPromise() CV_NOEXCEPT { CV_XADD(&refcount_promise, 1); CV_XADD(&refcount, 1); } \
    வெறுமை releasePromise() CV_NOEXCEPT { CV_XADD(&refcount_promise, -1); எனில்(1 == CV_XADD(&refcount, -1)) delete இது; } \
    முழு refcount_promise;

    mutable std::mutex mtx;
    mutable std::condition_variable cond_var;

    mutable தருக்க has_result; // Mat, UMat or exception

    mutable cv::Ptr<Mat> result_mat;
    mutable cv::Ptr<UMat> result_umat;


    தருக்க has_exception;
#எனில் CV__EXCEPTION_PTR
    std::exception_ptr exception;
#endif
    cv::Exception cv_exception;

    mutable தருக்க result_is_fetched;

    தருக்க future_is_returned;

    Impl()
        : refcount(1), refcount_future(0), refcount_promise(1)
        , has_result(பொய்)
        , has_exception(பொய்)
        , result_is_fetched(பொய்)
        , future_is_returned(பொய்)
    {
        // nothing
    }

    ~Impl()
    {
        எனில் (has_result && !result_is_fetched)
        {
            CV_LOG_INFO(வெற்று, "Asynchronous result has not been fetched");
        }
    }

    தருக்க get(OutputArray dst, int64 timeoutNs) மாறிலி
    {
        CV_Assert(!result_is_fetched);
        எனில் (!has_result)
        {
            எனில்(refcount_promise == 0)
                CV_Error(Error::StsInternal, "Asynchronous result producer has been destroyed");
            எனில் (!wait_for(timeoutNs))
                திரும்பு பொய்;
        }
        std::unique_lock<std::mutex> lock(mtx);
        எனில் (has_result)
        {
            எனில் (!result_mat.empty())
            {
                dst.move(*result_mat.get());
                result_mat.release();
                result_is_fetched = உண்மை;
                திரும்பு உண்மை;
            }
            எனில் (!result_umat.empty())
            {
                dst.move(*result_umat.get());
                result_umat.release();
                result_is_fetched = உண்மை;
                திரும்பு உண்மை;
            }
#எனில் CV__EXCEPTION_PTR
            எனில் (has_exception && exception)
            {
                result_is_fetched = உண்மை;
                std::rethrow_exception(exception);
            }
#endif
            எனில் (has_exception)
            {
                result_is_fetched = உண்மை;
                எறி cv_exception;
            }
            CV_Error(Error::StsInternal, "AsyncArray: invalid state of 'has_result = true'");
        }
        CV_Assert(!has_result);
        CV_Assert(timeoutNs < 0);
        திரும்பு பொய்;
    }

    தருக்க valid() மாறிலி CV_NOEXCEPT
    {
        எனில் (result_is_fetched)
            திரும்பு பொய்;
        எனில் (refcount_promise == 0 && !has_result)
            திரும்பு பொய்;
        திரும்பு உண்மை;
    }

    தருக்க wait_for(int64 timeoutNs) மாறிலி
    {
        CV_Assert(valid());
        எனில் (has_result)
            திரும்பு has_result;
        எனில் (timeoutNs == 0)
            திரும்பு has_result;
        CV_LOG_INFO(வெற்று, "Waiting for async result ...");
        std::unique_lock<std::mutex> lock(mtx);
        மாறிலி auto cond_pred = [&]{ திரும்பு has_result == உண்மை; };
        எனில் (timeoutNs > 0)
            திரும்பு cond_var.wait_for(lock, std::chrono::nanoseconds(timeoutNs), cond_pred);
        இல்லையெனில்
        {
            cond_var.wait(lock, cond_pred);
            CV_Assert(has_result);
            திரும்பு உண்மை;
        }
    }

    AsyncArray getArrayResult()
    {
        CV_Assert(refcount_future == 0);
        AsyncArray result;
        addrefFuture();
        result.p = இது;
        future_is_returned = உண்மை;
        திரும்பு result;
    }

    வெறுமை set_மதிப்பு(உள்ளீடு_Array value)
    {
        எனில் (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        std::unique_lock<std::mutex> lock(mtx);
        CV_Assert(!has_result);
        முழு k = value.kind();
        எனில் (k == உள்ளீடு_Array_2::UMAT)
        {
            result_umat = makePtr<UMat>();
            value.copyTo(*result_umat.get());
        }
        இல்லையெனில்
        {
            result_mat = makePtr<Mat>();
            value.copyTo(*result_mat.get());
        }
        has_result = உண்மை;
        cond_var.notify_all();
    }

#எனில் CV__EXCEPTION_PTR
    வெறுமை setException(std::exception_ptr e)
    {
        எனில் (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        std::unique_lock<std::mutex> lock(mtx);
        CV_Assert(!has_result);
        has_exception = உண்மை;
        exception = e;
        has_result = உண்மை;
        cond_var.notify_all();
    }
#endif

    வெறுமை setException(மாறிலி cv::Exception e)
    {
        எனில் (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        std::unique_lock<std::mutex> lock(mtx);
        CV_Assert(!has_result);
        has_exception = உண்மை;
        cv_exception = e;
        has_result = உண்மை;
        cond_var.notify_all();
    }
};

}  // namespace

#இல்லையெனில்  // OPENCV_DISABLE_THREAD_SUPPORT

நாமாகாஶ cv {

// no threading
கட்டமைப்பு AsyncArray::Impl
{
    முழு refcount;
    வெறுமை addrefFuture() CV_NOEXCEPT { refcount_future++; refcount++; }
    வெறுமை releaseFuture() CV_NOEXCEPT { refcount_future--; எனில் (0 == --refcount) delete இது; }
    முழு refcount_future;
    வெறுமை addrefPromise() CV_NOEXCEPT { refcount_promise++; refcount++; } \
    வெறுமை releasePromise() CV_NOEXCEPT { refcount_promise--; எனில் (0 == --refcount) delete இது; }
    முழு refcount_promise;

    mutable தருக்க has_result; // Mat, UMat or exception

    mutable cv::Ptr<Mat> result_mat;
    mutable cv::Ptr<UMat> result_umat;


    தருக்க has_exception;
#எனில் CV__EXCEPTION_PTR
    std::exception_ptr exception;
#endif
    cv::Exception cv_exception;

    mutable தருக்க result_is_fetched;

    தருக்க future_is_returned;

    Impl()
        : refcount(1), refcount_future(0), refcount_promise(1)
        , has_result(பொய்)
        , has_exception(பொய்)
        , result_is_fetched(பொய்)
        , future_is_returned(பொய்)
    {
        // nothing
    }

    ~Impl()
    {
        எனில் (has_result && !result_is_fetched)
        {
            CV_LOG_INFO(வெற்று, "Asynchronous result has not been fetched");
        }
    }

    தருக்க get(OutputArray dst, int64 timeoutNs) மாறிலி
    {
        CV_Assert(!result_is_fetched);
        எனில் (!has_result)
        {
            CV_UNUSED(timeoutNs);
            CV_Error(Error::StsError, "Result is not produced (unable to wait for result in OPENCV_DISABLE_THREAD_SUPPORT mode)");
        }
        எனில் (!result_mat.empty())
        {
            dst.move(*result_mat.get());
            result_mat.release();
            result_is_fetched = உண்மை;
            திரும்பு உண்மை;
        }
        எனில் (!result_umat.empty())
        {
            dst.move(*result_umat.get());
            result_umat.release();
            result_is_fetched = உண்மை;
            திரும்பு உண்மை;
        }
#எனில் CV__EXCEPTION_PTR
        எனில் (has_exception && exception)
        {
            result_is_fetched = உண்மை;
            std::rethrow_exception(exception);
        }
#endif
        எனில் (has_exception)
        {
            result_is_fetched = உண்மை;
            எறி cv_exception;
        }
        CV_Error(Error::StsInternal, "AsyncArray: invalid state of 'has_result = true'");
        திரும்பு பொய்;
    }

    தருக்க valid() மாறிலி CV_NOEXCEPT
    {
        எனில் (result_is_fetched)
            திரும்பு பொய்;
        எனில் (refcount_promise == 0 && !has_result)
            திரும்பு பொய்;
        திரும்பு உண்மை;
    }

    தருக்க wait_for(int64 timeoutNs) மாறிலி
    {
        CV_Assert(valid());
        எனில் (has_result)
            திரும்பு has_result;
        எனில் (timeoutNs == 0)
            திரும்பு has_result;
        CV_Error(Error::StsError, "Unable to wait in OPENCV_DISABLE_THREAD_SUPPORT mode");
    }

    AsyncArray getArrayResult()
    {
        CV_Assert(refcount_future == 0);
        AsyncArray result;
        addrefFuture();
        result.p = இது;
        future_is_returned = உண்மை;
        திரும்பு result;
    }

    வெறுமை set_மதிப்பு(உள்ளீடு_Array value)
    {
        எனில் (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        CV_Assert(!has_result);
        முழு k = value.kind();
        எனில் (k == உள்ளீடு_Array_2::UMAT)
        {
            result_umat = makePtr<UMat>();
            value.copyTo(*result_umat.get());
        }
        இல்லையெனில்
        {
            result_mat = makePtr<Mat>();
            value.copyTo(*result_mat.get());
        }
        has_result = உண்மை;
    }

#எனில் CV__EXCEPTION_PTR
    வெறுமை setException(std::exception_ptr e)
    {
        எனில் (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        CV_Assert(!has_result);
        has_exception = உண்மை;
        exception = e;
        has_result = உண்மை;
    }
#endif

    வெறுமை setException(மாறிலி cv::Exception e)
    {
        எனில் (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        CV_Assert(!has_result);
        has_exception = உண்மை;
        cv_exception = e;
        has_result = உண்மை;
    }
};

}

#endif  // OPENCV_DISABLE_THREAD_SUPPORT

நாமாகாஶ cv {

AsyncArray::AsyncArray() CV_NOEXCEPT
    : p(வெற்று)
{
}

AsyncArray::~AsyncArray() CV_NOEXCEPT
{
    release();
}

AsyncArray::AsyncArray(மாறிலி AsyncArray& o) CV_NOEXCEPT
    : p(o.p)
{
    எனில் (p)
        p->addrefFuture();
}

AsyncArray& AsyncArray::operator=(மாறிலி AsyncArray& o) CV_NOEXCEPT
{
    Impl* newp = o.p;
    எனில் (newp)
        newp->addrefFuture();
    release();
    p = newp;
    திரும்பு *இது;
}

வெறுமை AsyncArray::release() CV_NOEXCEPT
{
    Impl* impl = p;
    p = வெற்று;
    எனில் (impl)
        impl->releaseFuture();
}

தருக்க AsyncArray::get(OutputArray dst, int64 timeoutNs) மாறிலி
{
    CV_Assert(p);
    திரும்பு p->get(dst, timeoutNs);
}

வெறுமை AsyncArray::get(OutputArray dst) மாறிலி
{
    CV_Assert(p);
    தருக்க res = p->get(dst, -1);
    CV_Assert(res);
}

தருக்க AsyncArray::wait_for(int64 timeoutNs) மாறிலி
{
    CV_Assert(p);
    திரும்பு p->wait_for(timeoutNs);
}

தருக்க AsyncArray::valid() மாறிலி CV_NOEXCEPT
{
    எனில் (!p) திரும்பு பொய்;
    திரும்பு p->valid();
}


//
// AsyncPromise
//

AsyncPromise::AsyncPromise() CV_NOEXCEPT
    : p(புதிய AsyncArray::Impl())
{
}

AsyncPromise::~AsyncPromise() CV_NOEXCEPT
{
    release();
}

AsyncPromise::AsyncPromise(மாறிலி AsyncPromise& o) CV_NOEXCEPT
    : p(o.p)
{
    எனில் (p)
        p->addrefPromise();
}

AsyncPromise& AsyncPromise::operator=(மாறிலி AsyncPromise& o) CV_NOEXCEPT
{
    Impl* newp = o.p;
    எனில் (newp)
        newp->addrefPromise();
    release();
    p = newp;
    திரும்பு *இது;
}

வெறுமை AsyncPromise::release() CV_NOEXCEPT
{
    Impl* impl = p;
    p = வெற்று;
    எனில் (impl)
        impl->releasePromise();
}

AsyncArray AsyncPromise::getArrayResult()
{
    CV_Assert(p);
    திரும்பு p->getArrayResult();
}

வெறுமை AsyncPromise::set_மதிப்பு(உள்ளீடு_Array value)
{
    CV_Assert(p);
    திரும்பு p->set_மதிப்பு(value);
}

வெறுமை AsyncPromise::setException(மாறிலி cv::Exception& exception)
{
    CV_Assert(p);
    திரும்பு p->setException(exception);
}

#எனில் CV__EXCEPTION_PTR
வெறுமை AsyncPromise::setException(std::exception_ptr exception)
{
    CV_Assert(p);
    திரும்பு p->setException(exception);
}
#endif

} // namespace
