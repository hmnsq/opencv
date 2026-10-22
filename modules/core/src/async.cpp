// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ਅਨ੍ਤਰ੍ਭਾਵ "precomp.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "opencv2/core/async.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "opencv2/core/detail/async_promise.hpp"

#ਅਨ੍ਤਰ੍ਭਾਵ "opencv2/core/cvstd.hpp"

#ਅਨ੍ਤਰ੍ਭਾਵ <opencv2/core/utils/logger.defines.hpp>
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ CV_LOG_STRIP_LEVEL
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_DEBUG + 1
#ਅਨ੍ਤਰ੍ਭਾਵ <opencv2/core/utils/logger.hpp>

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OPENCV_DISABLE_THREAD_SUPPORT

#ਅਨ੍ਤਰ੍ਭਾਵ <mutex>
#ਅਨ੍ਤਰ੍ਭਾਵ <condition_variable>
#ਅਨ੍ਤਰ੍ਭਾਵ <chrono>

ਨਾਮਾਕਾਸ਼ cv {

/**
Manages shared state of asynchronous result
*/
ਬਣਤਰ AsyncArray::Impl
{
    ਪੂਰਨ refcount;
    ਖਾਲੀ addrefFuture() CV_NOEXCEPT { CV_XADD(&refcount_future, 1); CV_XADD(&refcount, 1); } \
    ਖਾਲੀ releaseFuture() CV_NOEXCEPT { CV_XADD(&refcount_future, -1); ਜੇ(1 == CV_XADD(&refcount, -1)) ਮਿਟਾਓ ਇਹ; } \
    ਪੂਰਨ refcount_future;
    ਖਾਲੀ addrefPromise() CV_NOEXCEPT { CV_XADD(&refcount_promise, 1); CV_XADD(&refcount, 1); } \
    ਖਾਲੀ releasePromise() CV_NOEXCEPT { CV_XADD(&refcount_promise, -1); ਜੇ(1 == CV_XADD(&refcount, -1)) ਮਿਟਾਓ ਇਹ; } \
    ਪੂਰਨ refcount_promise;

    ਬਦਲਣਯੋਗ std::mutex mtx;
    ਬਦਲਣਯੋਗ std::condition_variable cond_var;

    ਬਦਲਣਯੋਗ ਤਾਰਕਿਕ has_ਨਤੀਜਾ; // Mat, UMat or exception

    ਬਦਲਣਯੋਗ cv::Ptr<Mat> ਨਤੀਜਾ_mat;
    ਬਦਲਣਯੋਗ cv::Ptr<UMat> ਨਤੀਜਾ_umat;


    ਤਾਰਕਿਕ has_exception;
#ਜੇ CV__EXCEPTION_PTR
    std::exception_ptr exception;
#ਜੇ_ਅੰਤ
    cv::Exception cv_exception;

    ਬਦਲਣਯੋਗ ਤਾਰਕਿਕ ਨਤੀਜਾ_is_fetched;

    ਤਾਰਕਿਕ future_is_returned;

    Impl()
        : refcount(1), refcount_future(0), refcount_promise(1)
        , has_ਨਤੀਜਾ(ਝੂਠ)
        , has_exception(ਝੂਠ)
        , ਨਤੀਜਾ_is_fetched(ਝੂਠ)
        , future_is_returned(ਝੂਠ)
    {
        // nothing
    }

    ~Impl()
    {
        ਜੇ (has_ਨਤੀਜਾ && !ਨਤੀਜਾ_is_fetched)
        {
            CV_LOG_INFO(ਨਿਰਾ, "Asynchronous result has not been fetched");
        }
    }

    ਤਾਰਕਿਕ get(OutputArray dst, int64 timeoutNs) ਅਚਲ
    {
        CV_Assert(!ਨਤੀਜਾ_is_fetched);
        ਜੇ (!has_ਨਤੀਜਾ)
        {
            ਜੇ(refcount_promise == 0)
                CV_Error(Error::Sts_ਅੰਦਰੂਨੀ, "Asynchronous result producer has been destroyed");
            ਜੇ (!wait_for(timeoutNs))
                ਵਾਪਸ ਝੂਠ;
        }
        std::unique_lock<std::mutex> lock(mtx);
        ਜੇ (has_ਨਤੀਜਾ)
        {
            ਜੇ (!ਨਤੀਜਾ_mat.empty())
            {
                dst.move(*ਨਤੀਜਾ_mat.get());
                ਨਤੀਜਾ_mat.release();
                ਨਤੀਜਾ_is_fetched = ਸੱਚ;
                ਵਾਪਸ ਸੱਚ;
            }
            ਜੇ (!ਨਤੀਜਾ_umat.empty())
            {
                dst.move(*ਨਤੀਜਾ_umat.get());
                ਨਤੀਜਾ_umat.release();
                ਨਤੀਜਾ_is_fetched = ਸੱਚ;
                ਵਾਪਸ ਸੱਚ;
            }
#ਜੇ CV__EXCEPTION_PTR
            ਜੇ (has_exception && exception)
            {
                ਨਤੀਜਾ_is_fetched = ਸੱਚ;
                std::rethrow_exception(exception);
            }
#ਜੇ_ਅੰਤ
            ਜੇ (has_exception)
            {
                ਨਤੀਜਾ_is_fetched = ਸੱਚ;
                ਸੁੱਟੋ cv_exception;
            }
            CV_Error(Error::Sts_ਅੰਦਰੂਨੀ, "AsyncArray: invalid state of 'has_result = true'");
        }
        CV_Assert(!has_ਨਤੀਜਾ);
        CV_Assert(timeoutNs < 0);
        ਵਾਪਸ ਝੂਠ;
    }

    ਤਾਰਕਿਕ valid() ਅਚਲ CV_NOEXCEPT
    {
        ਜੇ (ਨਤੀਜਾ_is_fetched)
            ਵਾਪਸ ਝੂਠ;
        ਜੇ (refcount_promise == 0 && !has_ਨਤੀਜਾ)
            ਵਾਪਸ ਝੂਠ;
        ਵਾਪਸ ਸੱਚ;
    }

    ਤਾਰਕਿਕ wait_for(int64 timeoutNs) ਅਚਲ
    {
        CV_Assert(valid());
        ਜੇ (has_ਨਤੀਜਾ)
            ਵਾਪਸ has_ਨਤੀਜਾ;
        ਜੇ (timeoutNs == 0)
            ਵਾਪਸ has_ਨਤੀਜਾ;
        CV_LOG_INFO(ਨਿਰਾ, "Waiting for async result ...");
        std::unique_lock<std::mutex> lock(mtx);
        ਅਚਲ ਸਵੈਚਾਲਿਤ cond_pred = [&]{ ਵਾਪਸ has_ਨਤੀਜਾ == ਸੱਚ; };
        ਜੇ (timeoutNs > 0)
            ਵਾਪਸ cond_var.wait_for(lock, std::chrono::nanoseconds(timeoutNs), cond_pred);
        ਨਹੀਂਤਾਂ
        {
            cond_var.wait(lock, cond_pred);
            CV_Assert(has_ਨਤੀਜਾ);
            ਵਾਪਸ ਸੱਚ;
        }
    }

    AsyncArray get_Array_ਨਤੀਜਾ()
    {
        CV_Assert(refcount_future == 0);
        AsyncArray ਨਤੀਜਾ;
        addrefFuture();
        ਨਤੀਜਾ.p = ਇਹ;
        future_is_returned = ਸੱਚ;
        ਵਾਪਸ ਨਤੀਜਾ;
    }

    ਖਾਲੀ ਸੈੱਟ_ਮੁੱਲ(ਨਿਵੇਸ਼_Array value)
    {
        ਜੇ (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        std::unique_lock<std::mutex> lock(mtx);
        CV_Assert(!has_ਨਤੀਜਾ);
        ਪੂਰਨ k = value.ਕਿਸਮ();
        ਜੇ (k == ਨਿਵੇਸ਼_Array_2::UMAT)
        {
            ਨਤੀਜਾ_umat = makePtr<UMat>();
            value.copy_ਮੰਜ਼ਿਲ(*ਨਤੀਜਾ_umat.get());
        }
        ਨਹੀਂਤਾਂ
        {
            ਨਤੀਜਾ_mat = makePtr<Mat>();
            value.copy_ਮੰਜ਼ਿਲ(*ਨਤੀਜਾ_mat.get());
        }
        has_ਨਤੀਜਾ = ਸੱਚ;
        cond_var.notify_all();
    }

#ਜੇ CV__EXCEPTION_PTR
    ਖਾਲੀ ਸੈੱਟ_Exception(std::exception_ptr e)
    {
        ਜੇ (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        std::unique_lock<std::mutex> lock(mtx);
        CV_Assert(!has_ਨਤੀਜਾ);
        has_exception = ਸੱਚ;
        exception = e;
        has_ਨਤੀਜਾ = ਸੱਚ;
        cond_var.notify_all();
    }
#ਜੇ_ਅੰਤ

    ਖਾਲੀ ਸੈੱਟ_Exception(ਅਚਲ cv::Exception e)
    {
        ਜੇ (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        std::unique_lock<std::mutex> lock(mtx);
        CV_Assert(!has_ਨਤੀਜਾ);
        has_exception = ਸੱਚ;
        cv_exception = e;
        has_ਨਤੀਜਾ = ਸੱਚ;
        cond_var.notify_all();
    }
};

}  // namespace

#ਨਹੀਂਤਾਂ  // OPENCV_DISABLE_THREAD_SUPPORT

ਨਾਮਾਕਾਸ਼ cv {

// no threading
ਬਣਤਰ AsyncArray::Impl
{
    ਪੂਰਨ refcount;
    ਖਾਲੀ addrefFuture() CV_NOEXCEPT { refcount_future++; refcount++; }
    ਖਾਲੀ releaseFuture() CV_NOEXCEPT { refcount_future--; ਜੇ (0 == --refcount) ਮਿਟਾਓ ਇਹ; }
    ਪੂਰਨ refcount_future;
    ਖਾਲੀ addrefPromise() CV_NOEXCEPT { refcount_promise++; refcount++; } \
    ਖਾਲੀ releasePromise() CV_NOEXCEPT { refcount_promise--; ਜੇ (0 == --refcount) ਮਿਟਾਓ ਇਹ; }
    ਪੂਰਨ refcount_promise;

    ਬਦਲਣਯੋਗ ਤਾਰਕਿਕ has_ਨਤੀਜਾ; // Mat, UMat or exception

    ਬਦਲਣਯੋਗ cv::Ptr<Mat> ਨਤੀਜਾ_mat;
    ਬਦਲਣਯੋਗ cv::Ptr<UMat> ਨਤੀਜਾ_umat;


    ਤਾਰਕਿਕ has_exception;
#ਜੇ CV__EXCEPTION_PTR
    std::exception_ptr exception;
#ਜੇ_ਅੰਤ
    cv::Exception cv_exception;

    ਬਦਲਣਯੋਗ ਤਾਰਕਿਕ ਨਤੀਜਾ_is_fetched;

    ਤਾਰਕਿਕ future_is_returned;

    Impl()
        : refcount(1), refcount_future(0), refcount_promise(1)
        , has_ਨਤੀਜਾ(ਝੂਠ)
        , has_exception(ਝੂਠ)
        , ਨਤੀਜਾ_is_fetched(ਝੂਠ)
        , future_is_returned(ਝੂਠ)
    {
        // nothing
    }

    ~Impl()
    {
        ਜੇ (has_ਨਤੀਜਾ && !ਨਤੀਜਾ_is_fetched)
        {
            CV_LOG_INFO(ਨਿਰਾ, "Asynchronous result has not been fetched");
        }
    }

    ਤਾਰਕਿਕ get(OutputArray dst, int64 timeoutNs) ਅਚਲ
    {
        CV_Assert(!ਨਤੀਜਾ_is_fetched);
        ਜੇ (!has_ਨਤੀਜਾ)
        {
            CV_ਅਣਵਰਤਿਆ(timeoutNs);
            CV_Error(Error::StsError, "Result is not produced (unable to wait for result in OPENCV_DISABLE_THREAD_SUPPORT mode)");
        }
        ਜੇ (!ਨਤੀਜਾ_mat.empty())
        {
            dst.move(*ਨਤੀਜਾ_mat.get());
            ਨਤੀਜਾ_mat.release();
            ਨਤੀਜਾ_is_fetched = ਸੱਚ;
            ਵਾਪਸ ਸੱਚ;
        }
        ਜੇ (!ਨਤੀਜਾ_umat.empty())
        {
            dst.move(*ਨਤੀਜਾ_umat.get());
            ਨਤੀਜਾ_umat.release();
            ਨਤੀਜਾ_is_fetched = ਸੱਚ;
            ਵਾਪਸ ਸੱਚ;
        }
#ਜੇ CV__EXCEPTION_PTR
        ਜੇ (has_exception && exception)
        {
            ਨਤੀਜਾ_is_fetched = ਸੱਚ;
            std::rethrow_exception(exception);
        }
#ਜੇ_ਅੰਤ
        ਜੇ (has_exception)
        {
            ਨਤੀਜਾ_is_fetched = ਸੱਚ;
            ਸੁੱਟੋ cv_exception;
        }
        CV_Error(Error::Sts_ਅੰਦਰੂਨੀ, "AsyncArray: invalid state of 'has_result = true'");
        ਵਾਪਸ ਝੂਠ;
    }

    ਤਾਰਕਿਕ valid() ਅਚਲ CV_NOEXCEPT
    {
        ਜੇ (ਨਤੀਜਾ_is_fetched)
            ਵਾਪਸ ਝੂਠ;
        ਜੇ (refcount_promise == 0 && !has_ਨਤੀਜਾ)
            ਵਾਪਸ ਝੂਠ;
        ਵਾਪਸ ਸੱਚ;
    }

    ਤਾਰਕਿਕ wait_for(int64 timeoutNs) ਅਚਲ
    {
        CV_Assert(valid());
        ਜੇ (has_ਨਤੀਜਾ)
            ਵਾਪਸ has_ਨਤੀਜਾ;
        ਜੇ (timeoutNs == 0)
            ਵਾਪਸ has_ਨਤੀਜਾ;
        CV_Error(Error::StsError, "Unable to wait in OPENCV_DISABLE_THREAD_SUPPORT mode");
    }

    AsyncArray get_Array_ਨਤੀਜਾ()
    {
        CV_Assert(refcount_future == 0);
        AsyncArray ਨਤੀਜਾ;
        addrefFuture();
        ਨਤੀਜਾ.p = ਇਹ;
        future_is_returned = ਸੱਚ;
        ਵਾਪਸ ਨਤੀਜਾ;
    }

    ਖਾਲੀ ਸੈੱਟ_ਮੁੱਲ(ਨਿਵੇਸ਼_Array value)
    {
        ਜੇ (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        CV_Assert(!has_ਨਤੀਜਾ);
        ਪੂਰਨ k = value.ਕਿਸਮ();
        ਜੇ (k == ਨਿਵੇਸ਼_Array_2::UMAT)
        {
            ਨਤੀਜਾ_umat = makePtr<UMat>();
            value.copy_ਮੰਜ਼ਿਲ(*ਨਤੀਜਾ_umat.get());
        }
        ਨਹੀਂਤਾਂ
        {
            ਨਤੀਜਾ_mat = makePtr<Mat>();
            value.copy_ਮੰਜ਼ਿਲ(*ਨਤੀਜਾ_mat.get());
        }
        has_ਨਤੀਜਾ = ਸੱਚ;
    }

#ਜੇ CV__EXCEPTION_PTR
    ਖਾਲੀ ਸੈੱਟ_Exception(std::exception_ptr e)
    {
        ਜੇ (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        CV_Assert(!has_ਨਤੀਜਾ);
        has_exception = ਸੱਚ;
        exception = e;
        has_ਨਤੀਜਾ = ਸੱਚ;
    }
#ਜੇ_ਅੰਤ

    ਖਾਲੀ ਸੈੱਟ_Exception(ਅਚਲ cv::Exception e)
    {
        ਜੇ (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        CV_Assert(!has_ਨਤੀਜਾ);
        has_exception = ਸੱਚ;
        cv_exception = e;
        has_ਨਤੀਜਾ = ਸੱਚ;
    }
};

}

#ਜੇ_ਅੰਤ  // OPENCV_DISABLE_THREAD_SUPPORT

ਨਾਮਾਕਾਸ਼ cv {

AsyncArray::AsyncArray() CV_NOEXCEPT
    : p(ਨਿਰਾ)
{
}

AsyncArray::~AsyncArray() CV_NOEXCEPT
{
    release();
}

AsyncArray::AsyncArray(ਅਚਲ AsyncArray& o) CV_NOEXCEPT
    : p(o.p)
{
    ਜੇ (p)
        p->addrefFuture();
}

AsyncArray& AsyncArray::ਸੰਚਾਲਕ=(ਅਚਲ AsyncArray& o) CV_NOEXCEPT
{
    Impl* newp = o.p;
    ਜੇ (newp)
        newp->addrefFuture();
    release();
    p = newp;
    ਵਾਪਸ *ਇਹ;
}

ਖਾਲੀ AsyncArray::release() CV_NOEXCEPT
{
    Impl* impl = p;
    p = ਨਿਰਾ;
    ਜੇ (impl)
        impl->releaseFuture();
}

ਤਾਰਕਿਕ AsyncArray::get(OutputArray dst, int64 timeoutNs) ਅਚਲ
{
    CV_Assert(p);
    ਵਾਪਸ p->get(dst, timeoutNs);
}

ਖਾਲੀ AsyncArray::get(OutputArray dst) ਅਚਲ
{
    CV_Assert(p);
    ਤਾਰਕਿਕ res = p->get(dst, -1);
    CV_Assert(res);
}

ਤਾਰਕਿਕ AsyncArray::wait_for(int64 timeoutNs) ਅਚਲ
{
    CV_Assert(p);
    ਵਾਪਸ p->wait_for(timeoutNs);
}

ਤਾਰਕਿਕ AsyncArray::valid() ਅਚਲ CV_NOEXCEPT
{
    ਜੇ (!p) ਵਾਪਸ ਝੂਠ;
    ਵਾਪਸ p->valid();
}


//
// AsyncPromise
//

AsyncPromise::AsyncPromise() CV_NOEXCEPT
    : p(ਨਵਾਂ AsyncArray::Impl())
{
}

AsyncPromise::~AsyncPromise() CV_NOEXCEPT
{
    release();
}

AsyncPromise::AsyncPromise(ਅਚਲ AsyncPromise& o) CV_NOEXCEPT
    : p(o.p)
{
    ਜੇ (p)
        p->addrefPromise();
}

AsyncPromise& AsyncPromise::ਸੰਚਾਲਕ=(ਅਚਲ AsyncPromise& o) CV_NOEXCEPT
{
    Impl* newp = o.p;
    ਜੇ (newp)
        newp->addrefPromise();
    release();
    p = newp;
    ਵਾਪਸ *ਇਹ;
}

ਖਾਲੀ AsyncPromise::release() CV_NOEXCEPT
{
    Impl* impl = p;
    p = ਨਿਰਾ;
    ਜੇ (impl)
        impl->releasePromise();
}

AsyncArray AsyncPromise::get_Array_ਨਤੀਜਾ()
{
    CV_Assert(p);
    ਵਾਪਸ p->get_Array_ਨਤੀਜਾ();
}

ਖਾਲੀ AsyncPromise::ਸੈੱਟ_ਮੁੱਲ(ਨਿਵੇਸ਼_Array value)
{
    CV_Assert(p);
    ਵਾਪਸ p->ਸੈੱਟ_ਮੁੱਲ(value);
}

ਖਾਲੀ AsyncPromise::ਸੈੱਟ_Exception(ਅਚਲ cv::Exception& exception)
{
    CV_Assert(p);
    ਵਾਪਸ p->ਸੈੱਟ_Exception(exception);
}

#ਜੇ CV__EXCEPTION_PTR
ਖਾਲੀ AsyncPromise::ਸੈੱਟ_Exception(std::exception_ptr exception)
{
    CV_Assert(p);
    ਵਾਪਸ p->ਸੈੱਟ_Exception(exception);
}
#ਜੇ_ਅੰਤ

} // namespace
