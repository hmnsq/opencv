// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ಅನ್ತರ್ಭಾವ "precomp.hpp"
#ಅನ್ತರ್ಭಾವ "opencv2/core/async.hpp"
#ಅನ್ತರ್ಭಾವ "opencv2/core/detail/async_promise.hpp"

#ಅನ್ತರ್ಭಾವ "opencv2/core/cvstd.hpp"

#ಅನ್ತರ್ಭಾವ <opencv2/core/utils/logger.defines.hpp>
#undef CV_LOG_STRIP_LEVEL
#define CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_DEBUG + 1
#ಅನ್ತರ್ಭಾವ <opencv2/core/utils/logger.hpp>

#ifndef OPENCV_DISABLE_THREAD_SUPPORT

#ಅನ್ತರ್ಭಾವ <mutex>
#ಅನ್ತರ್ಭಾವ <condition_variable>
#ಅನ್ತರ್ಭಾವ <chrono>

ನಾಮಾಕಾಶ cv {

/**
Manages shared state of asynchronous result
*/
ಸಂರಚನಾ AsyncArray::Impl
{
    ಪೂರ್ಣಾಙ್ಕ refcount;
    ಶೂನ್ಯ addrefFuture() CV_NOEXCEPT { CV_XADD(&refcount_future, 1); CV_XADD(&refcount, 1); } \
    ಶೂನ್ಯ releaseFuture() CV_NOEXCEPT { CV_XADD(&refcount_future, -1); ಯದಿ(1 == CV_XADD(&refcount, -1)) delete ಏತತ್; } \
    ಪೂರ್ಣಾಙ್ಕ refcount_future;
    ಶೂನ್ಯ addrefPromise() CV_NOEXCEPT { CV_XADD(&refcount_promise, 1); CV_XADD(&refcount, 1); } \
    ಶೂನ್ಯ releasePromise() CV_NOEXCEPT { CV_XADD(&refcount_promise, -1); ಯದಿ(1 == CV_XADD(&refcount, -1)) delete ಏತತ್; } \
    ಪೂರ್ಣಾಙ್ಕ refcount_promise;

    mutable std::mutex mtx;
    mutable std::condition_variable cond_var;

    mutable ತಾರ್ಕಿಕ has_result; // Mat, UMat or exception

    mutable cv::Ptr<Mat> result_mat;
    mutable cv::Ptr<UMat> result_umat;


    ತಾರ್ಕಿಕ has_exception;
#ಯದಿ CV__EXCEPTION_PTR
    std::exception_ptr exception;
#endif
    cv::Exception cv_exception;

    mutable ತಾರ್ಕಿಕ result_is_fetched;

    ತಾರ್ಕಿಕ future_is_returned;

    Impl()
        : refcount(1), refcount_future(0), refcount_promise(1)
        , has_result(ಅಸತ್ಯ)
        , has_exception(ಅಸತ್ಯ)
        , result_is_fetched(ಅಸತ್ಯ)
        , future_is_returned(ಅಸತ್ಯ)
    {
        // nothing
    }

    ~Impl()
    {
        ಯದಿ (has_result && !result_is_fetched)
        {
            CV_LOG_INFO(ರಿಕ್ತ, "Asynchronous result has not been fetched");
        }
    }

    ತಾರ್ಕಿಕ get(OutputArray dst, int64 timeoutNs) ಧ್ರುವ
    {
        CV_Assert(!result_is_fetched);
        ಯದಿ (!has_result)
        {
            ಯದಿ(refcount_promise == 0)
                CV_Error(Error::StsInternal, "Asynchronous result producer has been destroyed");
            ಯದಿ (!wait_for(timeoutNs))
                ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
        }
        std::unique_lock<std::mutex> lock(mtx);
        ಯದಿ (has_result)
        {
            ಯದಿ (!result_mat.empty())
            {
                dst.move(*result_mat.get());
                result_mat.release();
                result_is_fetched = ಸತ್ಯ;
                ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
            }
            ಯದಿ (!result_umat.empty())
            {
                dst.move(*result_umat.get());
                result_umat.release();
                result_is_fetched = ಸತ್ಯ;
                ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
            }
#ಯದಿ CV__EXCEPTION_PTR
            ಯದಿ (has_exception && exception)
            {
                result_is_fetched = ಸತ್ಯ;
                std::rethrow_exception(exception);
            }
#endif
            ಯದಿ (has_exception)
            {
                result_is_fetched = ಸತ್ಯ;
                ಕ್ಷಿಪ cv_exception;
            }
            CV_Error(Error::StsInternal, "AsyncArray: invalid state of 'has_result = true'");
        }
        CV_Assert(!has_result);
        CV_Assert(timeoutNs < 0);
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }

    ತಾರ್ಕಿಕ valid() ಧ್ರುವ CV_NOEXCEPT
    {
        ಯದಿ (result_is_fetched)
            ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
        ಯದಿ (refcount_promise == 0 && !has_result)
            ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
        ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    }

    ತಾರ್ಕಿಕ wait_for(int64 timeoutNs) ಧ್ರುವ
    {
        CV_Assert(valid());
        ಯದಿ (has_result)
            ಪ್ರತ್ಯಾಗಮ has_result;
        ಯದಿ (timeoutNs == 0)
            ಪ್ರತ್ಯಾಗಮ has_result;
        CV_LOG_INFO(ರಿಕ್ತ, "Waiting for async result ...");
        std::unique_lock<std::mutex> lock(mtx);
        ಧ್ರುವ auto cond_pred = [&]{ ಪ್ರತ್ಯಾಗಮ has_result == ಸತ್ಯ; };
        ಯದಿ (timeoutNs > 0)
            ಪ್ರತ್ಯಾಗಮ cond_var.wait_for(lock, std::chrono::nanoseconds(timeoutNs), cond_pred);
        ಅನ್ಯಥಾ
        {
            cond_var.wait(lock, cond_pred);
            CV_Assert(has_result);
            ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
        }
    }

    AsyncArray getArrayResult()
    {
        CV_Assert(refcount_future == 0);
        AsyncArray result;
        addrefFuture();
        result.p = ಏತತ್;
        future_is_returned = ಸತ್ಯ;
        ಪ್ರತ್ಯಾಗಮ result;
    }

    ಶೂನ್ಯ set_ಮೌಲ್ಯ(ಒಳಹರಿವು_Array value)
    {
        ಯದಿ (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        std::unique_lock<std::mutex> lock(mtx);
        CV_Assert(!has_result);
        ಪೂರ್ಣಾಙ್ಕ k = value.kind();
        ಯದಿ (k == ಒಳಹರಿವು_Array_2::UMAT)
        {
            result_umat = makePtr<UMat>();
            value.copyTo(*result_umat.get());
        }
        ಅನ್ಯಥಾ
        {
            result_mat = makePtr<Mat>();
            value.copyTo(*result_mat.get());
        }
        has_result = ಸತ್ಯ;
        cond_var.notify_all();
    }

#ಯದಿ CV__EXCEPTION_PTR
    ಶೂನ್ಯ setException(std::exception_ptr e)
    {
        ಯದಿ (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        std::unique_lock<std::mutex> lock(mtx);
        CV_Assert(!has_result);
        has_exception = ಸತ್ಯ;
        exception = e;
        has_result = ಸತ್ಯ;
        cond_var.notify_all();
    }
#endif

    ಶೂನ್ಯ setException(ಧ್ರುವ cv::Exception e)
    {
        ಯದಿ (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        std::unique_lock<std::mutex> lock(mtx);
        CV_Assert(!has_result);
        has_exception = ಸತ್ಯ;
        cv_exception = e;
        has_result = ಸತ್ಯ;
        cond_var.notify_all();
    }
};

}  // namespace

#ಅನ್ಯಥಾ  // OPENCV_DISABLE_THREAD_SUPPORT

ನಾಮಾಕಾಶ cv {

// no threading
ಸಂರಚನಾ AsyncArray::Impl
{
    ಪೂರ್ಣಾಙ್ಕ refcount;
    ಶೂನ್ಯ addrefFuture() CV_NOEXCEPT { refcount_future++; refcount++; }
    ಶೂನ್ಯ releaseFuture() CV_NOEXCEPT { refcount_future--; ಯದಿ (0 == --refcount) delete ಏತತ್; }
    ಪೂರ್ಣಾಙ್ಕ refcount_future;
    ಶೂನ್ಯ addrefPromise() CV_NOEXCEPT { refcount_promise++; refcount++; } \
    ಶೂನ್ಯ releasePromise() CV_NOEXCEPT { refcount_promise--; ಯದಿ (0 == --refcount) delete ಏತತ್; }
    ಪೂರ್ಣಾಙ್ಕ refcount_promise;

    mutable ತಾರ್ಕಿಕ has_result; // Mat, UMat or exception

    mutable cv::Ptr<Mat> result_mat;
    mutable cv::Ptr<UMat> result_umat;


    ತಾರ್ಕಿಕ has_exception;
#ಯದಿ CV__EXCEPTION_PTR
    std::exception_ptr exception;
#endif
    cv::Exception cv_exception;

    mutable ತಾರ್ಕಿಕ result_is_fetched;

    ತಾರ್ಕಿಕ future_is_returned;

    Impl()
        : refcount(1), refcount_future(0), refcount_promise(1)
        , has_result(ಅಸತ್ಯ)
        , has_exception(ಅಸತ್ಯ)
        , result_is_fetched(ಅಸತ್ಯ)
        , future_is_returned(ಅಸತ್ಯ)
    {
        // nothing
    }

    ~Impl()
    {
        ಯದಿ (has_result && !result_is_fetched)
        {
            CV_LOG_INFO(ರಿಕ್ತ, "Asynchronous result has not been fetched");
        }
    }

    ತಾರ್ಕಿಕ get(OutputArray dst, int64 timeoutNs) ಧ್ರುವ
    {
        CV_Assert(!result_is_fetched);
        ಯದಿ (!has_result)
        {
            CV_UNUSED(timeoutNs);
            CV_Error(Error::StsError, "Result is not produced (unable to wait for result in OPENCV_DISABLE_THREAD_SUPPORT mode)");
        }
        ಯದಿ (!result_mat.empty())
        {
            dst.move(*result_mat.get());
            result_mat.release();
            result_is_fetched = ಸತ್ಯ;
            ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
        }
        ಯದಿ (!result_umat.empty())
        {
            dst.move(*result_umat.get());
            result_umat.release();
            result_is_fetched = ಸತ್ಯ;
            ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
        }
#ಯದಿ CV__EXCEPTION_PTR
        ಯದಿ (has_exception && exception)
        {
            result_is_fetched = ಸತ್ಯ;
            std::rethrow_exception(exception);
        }
#endif
        ಯದಿ (has_exception)
        {
            result_is_fetched = ಸತ್ಯ;
            ಕ್ಷಿಪ cv_exception;
        }
        CV_Error(Error::StsInternal, "AsyncArray: invalid state of 'has_result = true'");
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }

    ತಾರ್ಕಿಕ valid() ಧ್ರುವ CV_NOEXCEPT
    {
        ಯದಿ (result_is_fetched)
            ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
        ಯದಿ (refcount_promise == 0 && !has_result)
            ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
        ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    }

    ತಾರ್ಕಿಕ wait_for(int64 timeoutNs) ಧ್ರುವ
    {
        CV_Assert(valid());
        ಯದಿ (has_result)
            ಪ್ರತ್ಯಾಗಮ has_result;
        ಯದಿ (timeoutNs == 0)
            ಪ್ರತ್ಯಾಗಮ has_result;
        CV_Error(Error::StsError, "Unable to wait in OPENCV_DISABLE_THREAD_SUPPORT mode");
    }

    AsyncArray getArrayResult()
    {
        CV_Assert(refcount_future == 0);
        AsyncArray result;
        addrefFuture();
        result.p = ಏತತ್;
        future_is_returned = ಸತ್ಯ;
        ಪ್ರತ್ಯಾಗಮ result;
    }

    ಶೂನ್ಯ set_ಮೌಲ್ಯ(ಒಳಹರಿವು_Array value)
    {
        ಯದಿ (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        CV_Assert(!has_result);
        ಪೂರ್ಣಾಙ್ಕ k = value.kind();
        ಯದಿ (k == ಒಳಹರಿವು_Array_2::UMAT)
        {
            result_umat = makePtr<UMat>();
            value.copyTo(*result_umat.get());
        }
        ಅನ್ಯಥಾ
        {
            result_mat = makePtr<Mat>();
            value.copyTo(*result_mat.get());
        }
        has_result = ಸತ್ಯ;
    }

#ಯದಿ CV__EXCEPTION_PTR
    ಶೂನ್ಯ setException(std::exception_ptr e)
    {
        ಯದಿ (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        CV_Assert(!has_result);
        has_exception = ಸತ್ಯ;
        exception = e;
        has_result = ಸತ್ಯ;
    }
#endif

    ಶೂನ್ಯ setException(ಧ್ರುವ cv::Exception e)
    {
        ಯದಿ (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        CV_Assert(!has_result);
        has_exception = ಸತ್ಯ;
        cv_exception = e;
        has_result = ಸತ್ಯ;
    }
};

}

#endif  // OPENCV_DISABLE_THREAD_SUPPORT

ನಾಮಾಕಾಶ cv {

AsyncArray::AsyncArray() CV_NOEXCEPT
    : p(ರಿಕ್ತ)
{
}

AsyncArray::~AsyncArray() CV_NOEXCEPT
{
    release();
}

AsyncArray::AsyncArray(ಧ್ರುವ AsyncArray& o) CV_NOEXCEPT
    : p(o.p)
{
    ಯದಿ (p)
        p->addrefFuture();
}

AsyncArray& AsyncArray::operator=(ಧ್ರುವ AsyncArray& o) CV_NOEXCEPT
{
    Impl* newp = o.p;
    ಯದಿ (newp)
        newp->addrefFuture();
    release();
    p = newp;
    ಪ್ರತ್ಯಾಗಮ *ಏತತ್;
}

ಶೂನ್ಯ AsyncArray::release() CV_NOEXCEPT
{
    Impl* impl = p;
    p = ರಿಕ್ತ;
    ಯದಿ (impl)
        impl->releaseFuture();
}

ತಾರ್ಕಿಕ AsyncArray::get(OutputArray dst, int64 timeoutNs) ಧ್ರುವ
{
    CV_Assert(p);
    ಪ್ರತ್ಯಾಗಮ p->get(dst, timeoutNs);
}

ಶೂನ್ಯ AsyncArray::get(OutputArray dst) ಧ್ರುವ
{
    CV_Assert(p);
    ತಾರ್ಕಿಕ res = p->get(dst, -1);
    CV_Assert(res);
}

ತಾರ್ಕಿಕ AsyncArray::wait_for(int64 timeoutNs) ಧ್ರುವ
{
    CV_Assert(p);
    ಪ್ರತ್ಯಾಗಮ p->wait_for(timeoutNs);
}

ತಾರ್ಕಿಕ AsyncArray::valid() ಧ್ರುವ CV_NOEXCEPT
{
    ಯದಿ (!p) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    ಪ್ರತ್ಯಾಗಮ p->valid();
}


//
// AsyncPromise
//

AsyncPromise::AsyncPromise() CV_NOEXCEPT
    : p(ನೂತನ AsyncArray::Impl())
{
}

AsyncPromise::~AsyncPromise() CV_NOEXCEPT
{
    release();
}

AsyncPromise::AsyncPromise(ಧ್ರುವ AsyncPromise& o) CV_NOEXCEPT
    : p(o.p)
{
    ಯದಿ (p)
        p->addrefPromise();
}

AsyncPromise& AsyncPromise::operator=(ಧ್ರುವ AsyncPromise& o) CV_NOEXCEPT
{
    Impl* newp = o.p;
    ಯದಿ (newp)
        newp->addrefPromise();
    release();
    p = newp;
    ಪ್ರತ್ಯಾಗಮ *ಏತತ್;
}

ಶೂನ್ಯ AsyncPromise::release() CV_NOEXCEPT
{
    Impl* impl = p;
    p = ರಿಕ್ತ;
    ಯದಿ (impl)
        impl->releasePromise();
}

AsyncArray AsyncPromise::getArrayResult()
{
    CV_Assert(p);
    ಪ್ರತ್ಯಾಗಮ p->getArrayResult();
}

ಶೂನ್ಯ AsyncPromise::set_ಮೌಲ್ಯ(ಒಳಹರಿವು_Array value)
{
    CV_Assert(p);
    ಪ್ರತ್ಯಾಗಮ p->set_ಮೌಲ್ಯ(value);
}

ಶೂನ್ಯ AsyncPromise::setException(ಧ್ರುವ cv::Exception& exception)
{
    CV_Assert(p);
    ಪ್ರತ್ಯಾಗಮ p->setException(exception);
}

#ಯದಿ CV__EXCEPTION_PTR
ಶೂನ್ಯ AsyncPromise::setException(std::exception_ptr exception)
{
    CV_Assert(p);
    ಪ್ರತ್ಯಾಗಮ p->setException(exception);
}
#endif

} // namespace
