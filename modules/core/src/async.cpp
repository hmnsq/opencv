// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ಅನ್ತರ್ಭಾವ "precomp.hpp"
#ಅನ್ತರ್ಭಾವ "opencv2/core/async.hpp"
#ಅನ್ತರ್ಭಾವ "opencv2/core/detail/async_promise.hpp"

#ಅನ್ತರ್ಭಾವ "opencv2/core/cvstd.hpp"

#ಅನ್ತರ್ಭಾವ <opencv2/core/utils/logger.defines.hpp>
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು CV_LOG_STRIP_LEVEL
#ವ್ಯಾಖ್ಯಾನಿಸು CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_DEBUG + 1
#ಅನ್ತರ್ಭಾವ <opencv2/core/utils/logger.hpp>

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ OPENCV_DISABLE_THREAD_SUPPORT

#ಅನ್ತರ್ಭಾವ <mutex>
#ಅನ್ತರ್ಭಾವ <condition_variable>
#ಅನ್ತರ್ಭಾವ <chrono>

ನಾಮಾಕಾಶ cv {

/**
Manages shared state of asynchronous result
*/
ರಚನೆ AsyncArray::Impl
{
    ಪೂರ್ಣಾಂಕ refcount;
    ಶೂನ್ಯ addrefFuture() CV_NOEXCEPT { CV_XADD(&refcount_future, 1); CV_XADD(&refcount, 1); } \
    ಶೂನ್ಯ releaseFuture() CV_NOEXCEPT { CV_XADD(&refcount_future, -1); ಆದರೆ(1 == CV_XADD(&refcount, -1)) ಅಳಿಸು ಇದು; } \
    ಪೂರ್ಣಾಂಕ refcount_future;
    ಶೂನ್ಯ addrefPromise() CV_NOEXCEPT { CV_XADD(&refcount_promise, 1); CV_XADD(&refcount, 1); } \
    ಶೂನ್ಯ releasePromise() CV_NOEXCEPT { CV_XADD(&refcount_promise, -1); ಆದರೆ(1 == CV_XADD(&refcount, -1)) ಅಳಿಸು ಇದು; } \
    ಪೂರ್ಣಾಂಕ refcount_promise;

    ಬದಲಾಯಿಸಬಹುದಾದ std::mutex mtx;
    ಬದಲಾಯಿಸಬಹುದಾದ std::condition_variable cond_var;

    ಬದಲಾಯಿಸಬಹುದಾದ ತಾರ್ಕಿಕ has_ಫಲಿತಾಂಶ; // Mat, UMat or exception

    ಬದಲಾಯಿಸಬಹುದಾದ cv::Ptr<Mat> ಫಲಿತಾಂಶ_mat;
    ಬದಲಾಯಿಸಬಹುದಾದ cv::Ptr<UMat> ಫಲಿತಾಂಶ_umat;


    ತಾರ್ಕಿಕ has_exception;
#ಆದರೆ CV__EXCEPTION_PTR
    std::exception_ptr exception;
#ಆದರೆ_ಅಂತ್ಯ
    cv::Exception cv_exception;

    ಬದಲಾಯಿಸಬಹುದಾದ ತಾರ್ಕಿಕ ಫಲಿತಾಂಶ_is_fetched;

    ತಾರ್ಕಿಕ future_is_returned;

    Impl()
        : refcount(1), refcount_future(0), refcount_promise(1)
        , has_ಫಲಿತಾಂಶ(ಸುಳ್ಳು)
        , has_exception(ಸುಳ್ಳು)
        , ಫಲಿತಾಂಶ_is_fetched(ಸುಳ್ಳು)
        , future_is_returned(ಸುಳ್ಳು)
    {
        // nothing
    }

    ~Impl()
    {
        ಆದರೆ (has_ಫಲಿತಾಂಶ && !ಫಲಿತಾಂಶ_is_fetched)
        {
            CV_LOG_INFO(ಖಾಲಿ, "Asynchronous result has not been fetched");
        }
    }

    ತಾರ್ಕಿಕ get(OutputArray dst, int64 timeoutNs) ಸ್ಥಿರಾಂಕ
    {
        CV_Assert(!ಫಲಿತಾಂಶ_is_fetched);
        ಆದರೆ (!has_ಫಲಿತಾಂಶ)
        {
            ಆದರೆ(refcount_promise == 0)
                CV_Error(Error::Sts_ಆಂತರಿಕ, "Asynchronous result producer has been destroyed");
            ಆದರೆ (!wait_for(timeoutNs))
                ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
        }
        std::unique_lock<std::mutex> lock(mtx);
        ಆದರೆ (has_ಫಲಿತಾಂಶ)
        {
            ಆದರೆ (!ಫಲಿತಾಂಶ_mat.empty())
            {
                dst.move(*ಫಲಿತಾಂಶ_mat.get());
                ಫಲಿತಾಂಶ_mat.release();
                ಫಲಿತಾಂಶ_is_fetched = ನಿಜ;
                ಹಿಂತಿರುಗಿ ನಿಜ;
            }
            ಆದರೆ (!ಫಲಿತಾಂಶ_umat.empty())
            {
                dst.move(*ಫಲಿತಾಂಶ_umat.get());
                ಫಲಿತಾಂಶ_umat.release();
                ಫಲಿತಾಂಶ_is_fetched = ನಿಜ;
                ಹಿಂತಿರುಗಿ ನಿಜ;
            }
#ಆದರೆ CV__EXCEPTION_PTR
            ಆದರೆ (has_exception && exception)
            {
                ಫಲಿತಾಂಶ_is_fetched = ನಿಜ;
                std::rethrow_exception(exception);
            }
#ಆದರೆ_ಅಂತ್ಯ
            ಆದರೆ (has_exception)
            {
                ಫಲಿತಾಂಶ_is_fetched = ನಿಜ;
                ಎಸೆ cv_exception;
            }
            CV_Error(Error::Sts_ಆಂತರಿಕ, "AsyncArray: invalid state of 'has_result = true'");
        }
        CV_Assert(!has_ಫಲಿತಾಂಶ);
        CV_Assert(timeoutNs < 0);
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
    }

    ತಾರ್ಕಿಕ valid() ಸ್ಥಿರಾಂಕ CV_NOEXCEPT
    {
        ಆದರೆ (ಫಲಿತಾಂಶ_is_fetched)
            ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
        ಆದರೆ (refcount_promise == 0 && !has_ಫಲಿತಾಂಶ)
            ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
        ಹಿಂತಿರುಗಿ ನಿಜ;
    }

    ತಾರ್ಕಿಕ wait_for(int64 timeoutNs) ಸ್ಥಿರಾಂಕ
    {
        CV_Assert(valid());
        ಆದರೆ (has_ಫಲಿತಾಂಶ)
            ಹಿಂತಿರುಗಿ has_ಫಲಿತಾಂಶ;
        ಆದರೆ (timeoutNs == 0)
            ಹಿಂತಿರುಗಿ has_ಫಲಿತಾಂಶ;
        CV_LOG_INFO(ಖಾಲಿ, "Waiting for async result ...");
        std::unique_lock<std::mutex> lock(mtx);
        ಸ್ಥಿರಾಂಕ ಸ್ವಯಂಚಾಲಿತ cond_pred = [&]{ ಹಿಂತಿರುಗಿ has_ಫಲಿತಾಂಶ == ನಿಜ; };
        ಆದರೆ (timeoutNs > 0)
            ಹಿಂತಿರುಗಿ cond_var.wait_for(lock, std::chrono::nanoseconds(timeoutNs), cond_pred);
        ಇಲ್ಲದಿದ್ದರೆ
        {
            cond_var.wait(lock, cond_pred);
            CV_Assert(has_ಫಲಿತಾಂಶ);
            ಹಿಂತಿರುಗಿ ನಿಜ;
        }
    }

    AsyncArray get_Array_ಫಲಿತಾಂಶ()
    {
        CV_Assert(refcount_future == 0);
        AsyncArray ಫಲಿತಾಂಶ;
        addrefFuture();
        ಫಲಿತಾಂಶ.p = ಇದು;
        future_is_returned = ನಿಜ;
        ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
    }

    ಶೂನ್ಯ ಗಣ_ಮೌಲ್ಯ(ಒಳಹರಿವು_Array value)
    {
        ಆದರೆ (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        std::unique_lock<std::mutex> lock(mtx);
        CV_Assert(!has_ಫಲಿತಾಂಶ);
        ಪೂರ್ಣಾಂಕ k = value.ಬಗೆ();
        ಆದರೆ (k == ಒಳಹರಿವು_Array_2::UMAT)
        {
            ಫಲಿತಾಂಶ_umat = makePtr<UMat>();
            value.copy_ಗಮ್ಯ(*ಫಲಿತಾಂಶ_umat.get());
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            ಫಲಿತಾಂಶ_mat = makePtr<Mat>();
            value.copy_ಗಮ್ಯ(*ಫಲಿತಾಂಶ_mat.get());
        }
        has_ಫಲಿತಾಂಶ = ನಿಜ;
        cond_var.notify_all();
    }

#ಆದರೆ CV__EXCEPTION_PTR
    ಶೂನ್ಯ ಗಣ_Exception(std::exception_ptr e)
    {
        ಆದರೆ (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        std::unique_lock<std::mutex> lock(mtx);
        CV_Assert(!has_ಫಲಿತಾಂಶ);
        has_exception = ನಿಜ;
        exception = e;
        has_ಫಲಿತಾಂಶ = ನಿಜ;
        cond_var.notify_all();
    }
#ಆದರೆ_ಅಂತ್ಯ

    ಶೂನ್ಯ ಗಣ_Exception(ಸ್ಥಿರಾಂಕ cv::Exception e)
    {
        ಆದರೆ (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        std::unique_lock<std::mutex> lock(mtx);
        CV_Assert(!has_ಫಲಿತಾಂಶ);
        has_exception = ನಿಜ;
        cv_exception = e;
        has_ಫಲಿತಾಂಶ = ನಿಜ;
        cond_var.notify_all();
    }
};

}  // namespace

#ಇಲ್ಲದಿದ್ದರೆ  // OPENCV_DISABLE_THREAD_SUPPORT

ನಾಮಾಕಾಶ cv {

// no threading
ರಚನೆ AsyncArray::Impl
{
    ಪೂರ್ಣಾಂಕ refcount;
    ಶೂನ್ಯ addrefFuture() CV_NOEXCEPT { refcount_future++; refcount++; }
    ಶೂನ್ಯ releaseFuture() CV_NOEXCEPT { refcount_future--; ಆದರೆ (0 == --refcount) ಅಳಿಸು ಇದು; }
    ಪೂರ್ಣಾಂಕ refcount_future;
    ಶೂನ್ಯ addrefPromise() CV_NOEXCEPT { refcount_promise++; refcount++; } \
    ಶೂನ್ಯ releasePromise() CV_NOEXCEPT { refcount_promise--; ಆದರೆ (0 == --refcount) ಅಳಿಸು ಇದು; }
    ಪೂರ್ಣಾಂಕ refcount_promise;

    ಬದಲಾಯಿಸಬಹುದಾದ ತಾರ್ಕಿಕ has_ಫಲಿತಾಂಶ; // Mat, UMat or exception

    ಬದಲಾಯಿಸಬಹುದಾದ cv::Ptr<Mat> ಫಲಿತಾಂಶ_mat;
    ಬದಲಾಯಿಸಬಹುದಾದ cv::Ptr<UMat> ಫಲಿತಾಂಶ_umat;


    ತಾರ್ಕಿಕ has_exception;
#ಆದರೆ CV__EXCEPTION_PTR
    std::exception_ptr exception;
#ಆದರೆ_ಅಂತ್ಯ
    cv::Exception cv_exception;

    ಬದಲಾಯಿಸಬಹುದಾದ ತಾರ್ಕಿಕ ಫಲಿತಾಂಶ_is_fetched;

    ತಾರ್ಕಿಕ future_is_returned;

    Impl()
        : refcount(1), refcount_future(0), refcount_promise(1)
        , has_ಫಲಿತಾಂಶ(ಸುಳ್ಳು)
        , has_exception(ಸುಳ್ಳು)
        , ಫಲಿತಾಂಶ_is_fetched(ಸುಳ್ಳು)
        , future_is_returned(ಸುಳ್ಳು)
    {
        // nothing
    }

    ~Impl()
    {
        ಆದರೆ (has_ಫಲಿತಾಂಶ && !ಫಲಿತಾಂಶ_is_fetched)
        {
            CV_LOG_INFO(ಖಾಲಿ, "Asynchronous result has not been fetched");
        }
    }

    ತಾರ್ಕಿಕ get(OutputArray dst, int64 timeoutNs) ಸ್ಥಿರಾಂಕ
    {
        CV_Assert(!ಫಲಿತಾಂಶ_is_fetched);
        ಆದರೆ (!has_ಫಲಿತಾಂಶ)
        {
            CV_ಬಳಸದ(timeoutNs);
            CV_Error(Error::StsError, "Result is not produced (unable to wait for result in OPENCV_DISABLE_THREAD_SUPPORT mode)");
        }
        ಆದರೆ (!ಫಲಿತಾಂಶ_mat.empty())
        {
            dst.move(*ಫಲಿತಾಂಶ_mat.get());
            ಫಲಿತಾಂಶ_mat.release();
            ಫಲಿತಾಂಶ_is_fetched = ನಿಜ;
            ಹಿಂತಿರುಗಿ ನಿಜ;
        }
        ಆದರೆ (!ಫಲಿತಾಂಶ_umat.empty())
        {
            dst.move(*ಫಲಿತಾಂಶ_umat.get());
            ಫಲಿತಾಂಶ_umat.release();
            ಫಲಿತಾಂಶ_is_fetched = ನಿಜ;
            ಹಿಂತಿರುಗಿ ನಿಜ;
        }
#ಆದರೆ CV__EXCEPTION_PTR
        ಆದರೆ (has_exception && exception)
        {
            ಫಲಿತಾಂಶ_is_fetched = ನಿಜ;
            std::rethrow_exception(exception);
        }
#ಆದರೆ_ಅಂತ್ಯ
        ಆದರೆ (has_exception)
        {
            ಫಲಿತಾಂಶ_is_fetched = ನಿಜ;
            ಎಸೆ cv_exception;
        }
        CV_Error(Error::Sts_ಆಂತರಿಕ, "AsyncArray: invalid state of 'has_result = true'");
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
    }

    ತಾರ್ಕಿಕ valid() ಸ್ಥಿರಾಂಕ CV_NOEXCEPT
    {
        ಆದರೆ (ಫಲಿತಾಂಶ_is_fetched)
            ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
        ಆದರೆ (refcount_promise == 0 && !has_ಫಲಿತಾಂಶ)
            ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
        ಹಿಂತಿರುಗಿ ನಿಜ;
    }

    ತಾರ್ಕಿಕ wait_for(int64 timeoutNs) ಸ್ಥಿರಾಂಕ
    {
        CV_Assert(valid());
        ಆದರೆ (has_ಫಲಿತಾಂಶ)
            ಹಿಂತಿರುಗಿ has_ಫಲಿತಾಂಶ;
        ಆದರೆ (timeoutNs == 0)
            ಹಿಂತಿರುಗಿ has_ಫಲಿತಾಂಶ;
        CV_Error(Error::StsError, "Unable to wait in OPENCV_DISABLE_THREAD_SUPPORT mode");
    }

    AsyncArray get_Array_ಫಲಿತಾಂಶ()
    {
        CV_Assert(refcount_future == 0);
        AsyncArray ಫಲಿತಾಂಶ;
        addrefFuture();
        ಫಲಿತಾಂಶ.p = ಇದು;
        future_is_returned = ನಿಜ;
        ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
    }

    ಶೂನ್ಯ ಗಣ_ಮೌಲ್ಯ(ಒಳಹರಿವು_Array value)
    {
        ಆದರೆ (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        CV_Assert(!has_ಫಲಿತಾಂಶ);
        ಪೂರ್ಣಾಂಕ k = value.ಬಗೆ();
        ಆದರೆ (k == ಒಳಹರಿವು_Array_2::UMAT)
        {
            ಫಲಿತಾಂಶ_umat = makePtr<UMat>();
            value.copy_ಗಮ್ಯ(*ಫಲಿತಾಂಶ_umat.get());
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            ಫಲಿತಾಂಶ_mat = makePtr<Mat>();
            value.copy_ಗಮ್ಯ(*ಫಲಿತಾಂಶ_mat.get());
        }
        has_ಫಲಿತಾಂಶ = ನಿಜ;
    }

#ಆದರೆ CV__EXCEPTION_PTR
    ಶೂನ್ಯ ಗಣ_Exception(std::exception_ptr e)
    {
        ಆದರೆ (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        CV_Assert(!has_ಫಲಿತಾಂಶ);
        has_exception = ನಿಜ;
        exception = e;
        has_ಫಲಿತಾಂಶ = ನಿಜ;
    }
#ಆದರೆ_ಅಂತ್ಯ

    ಶೂನ್ಯ ಗಣ_Exception(ಸ್ಥಿರಾಂಕ cv::Exception e)
    {
        ಆದರೆ (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        CV_Assert(!has_ಫಲಿತಾಂಶ);
        has_exception = ನಿಜ;
        cv_exception = e;
        has_ಫಲಿತಾಂಶ = ನಿಜ;
    }
};

}

#ಆದರೆ_ಅಂತ್ಯ  // OPENCV_DISABLE_THREAD_SUPPORT

ನಾಮಾಕಾಶ cv {

AsyncArray::AsyncArray() CV_NOEXCEPT
    : p(ಖಾಲಿ)
{
}

AsyncArray::~AsyncArray() CV_NOEXCEPT
{
    release();
}

AsyncArray::AsyncArray(ಸ್ಥಿರಾಂಕ AsyncArray& o) CV_NOEXCEPT
    : p(o.p)
{
    ಆದರೆ (p)
        p->addrefFuture();
}

AsyncArray& AsyncArray::ಕ್ರಿಯಾಕಾರಕ=(ಸ್ಥಿರಾಂಕ AsyncArray& o) CV_NOEXCEPT
{
    Impl* newp = o.p;
    ಆದರೆ (newp)
        newp->addrefFuture();
    release();
    p = newp;
    ಹಿಂತಿರುಗಿ *ಇದು;
}

ಶೂನ್ಯ AsyncArray::release() CV_NOEXCEPT
{
    Impl* impl = p;
    p = ಖಾಲಿ;
    ಆದರೆ (impl)
        impl->releaseFuture();
}

ತಾರ್ಕಿಕ AsyncArray::get(OutputArray dst, int64 timeoutNs) ಸ್ಥಿರಾಂಕ
{
    CV_Assert(p);
    ಹಿಂತಿರುಗಿ p->get(dst, timeoutNs);
}

ಶೂನ್ಯ AsyncArray::get(OutputArray dst) ಸ್ಥಿರಾಂಕ
{
    CV_Assert(p);
    ತಾರ್ಕಿಕ res = p->get(dst, -1);
    CV_Assert(res);
}

ತಾರ್ಕಿಕ AsyncArray::wait_for(int64 timeoutNs) ಸ್ಥಿರಾಂಕ
{
    CV_Assert(p);
    ಹಿಂತಿರುಗಿ p->wait_for(timeoutNs);
}

ತಾರ್ಕಿಕ AsyncArray::valid() ಸ್ಥಿರಾಂಕ CV_NOEXCEPT
{
    ಆದರೆ (!p) ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
    ಹಿಂತಿರುಗಿ p->valid();
}


//
// AsyncPromise
//

AsyncPromise::AsyncPromise() CV_NOEXCEPT
    : p(ಹೊಸ AsyncArray::Impl())
{
}

AsyncPromise::~AsyncPromise() CV_NOEXCEPT
{
    release();
}

AsyncPromise::AsyncPromise(ಸ್ಥಿರಾಂಕ AsyncPromise& o) CV_NOEXCEPT
    : p(o.p)
{
    ಆದರೆ (p)
        p->addrefPromise();
}

AsyncPromise& AsyncPromise::ಕ್ರಿಯಾಕಾರಕ=(ಸ್ಥಿರಾಂಕ AsyncPromise& o) CV_NOEXCEPT
{
    Impl* newp = o.p;
    ಆದರೆ (newp)
        newp->addrefPromise();
    release();
    p = newp;
    ಹಿಂತಿರುಗಿ *ಇದು;
}

ಶೂನ್ಯ AsyncPromise::release() CV_NOEXCEPT
{
    Impl* impl = p;
    p = ಖಾಲಿ;
    ಆದರೆ (impl)
        impl->releasePromise();
}

AsyncArray AsyncPromise::get_Array_ಫಲಿತಾಂಶ()
{
    CV_Assert(p);
    ಹಿಂತಿರುಗಿ p->get_Array_ಫಲಿತಾಂಶ();
}

ಶೂನ್ಯ AsyncPromise::ಗಣ_ಮೌಲ್ಯ(ಒಳಹರಿವು_Array value)
{
    CV_Assert(p);
    ಹಿಂತಿರುಗಿ p->ಗಣ_ಮೌಲ್ಯ(value);
}

ಶೂನ್ಯ AsyncPromise::ಗಣ_Exception(ಸ್ಥಿರಾಂಕ cv::Exception& exception)
{
    CV_Assert(p);
    ಹಿಂತಿರುಗಿ p->ಗಣ_Exception(exception);
}

#ಆದರೆ CV__EXCEPTION_PTR
ಶೂನ್ಯ AsyncPromise::ಗಣ_Exception(std::exception_ptr exception)
{
    CV_Assert(p);
    ಹಿಂತಿರುಗಿ p->ಗಣ_Exception(exception);
}
#ಆದರೆ_ಅಂತ್ಯ

} // namespace
