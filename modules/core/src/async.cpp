// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#समावेश "precomp.hpp"
#समावेश "opencv2/core/async.hpp"
#समावेश "opencv2/core/detail/async_promise.hpp"

#समावेश "opencv2/core/cvstd.hpp"

#समावेश <opencv2/core/utils/logger.defines.hpp>
#अघोषित CV_LOG_STRIP_LEVEL
#घोषणा CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_DEBUG + 1
#समावेश <opencv2/core/utils/logger.hpp>

#अगर_अघोषित OPENCV_DISABLE_THREAD_SUPPORT

#समावेश <mutex>
#समावेश <condition_variable>
#समावेश <chrono>

namespace cv {

/**
Manages shared state of asynchronous result
*/
काष्ठा AsyncArray::Impl
{
    पूर्णांक refcount;
    व्योम addrefFuture() CV_NOEXCEPT { CV_XADD(&refcount_future, 1); CV_XADD(&refcount, 1); } \
    व्योम releaseFuture() CV_NOEXCEPT { CV_XADD(&refcount_future, -1); अगर(1 == CV_XADD(&refcount, -1)) मिटाओ यह; } \
    पूर्णांक refcount_future;
    व्योम addrefPromise() CV_NOEXCEPT { CV_XADD(&refcount_promise, 1); CV_XADD(&refcount, 1); } \
    व्योम releasePromise() CV_NOEXCEPT { CV_XADD(&refcount_promise, -1); अगर(1 == CV_XADD(&refcount, -1)) मिटाओ यह; } \
    पूर्णांक refcount_promise;

    mutable std::mutex mtx;
    mutable std::condition_variable cond_var;

    mutable bool has_परिणाम; // Mat, UMat or exception

    mutable cv::Ptr<Mat> परिणाम_mat;
    mutable cv::Ptr<UMat> परिणाम_umat;


    bool has_exception;
#अगर CV__EXCEPTION_PTR
    std::exception_ptr exception;
#पूर्ण_अगर
    cv::Exception cv_exception;

    mutable bool परिणाम_is_fetched;

    bool future_is_returned;

    Impl()
        : refcount(1), refcount_future(0), refcount_promise(1)
        , has_परिणाम(false)
        , has_exception(false)
        , परिणाम_is_fetched(false)
        , future_is_returned(false)
    {
        // nothing
    }

    ~Impl()
    {
        अगर (has_परिणाम && !परिणाम_is_fetched)
        {
            CV_LOG_INFO(शून्य, "Asynchronous result has not been fetched");
        }
    }

    bool लो(OutputArray dst, int64 timeoutNs) स्थिर
    {
        CV_Assert(!परिणाम_is_fetched);
        अगर (!has_परिणाम)
        {
            अगर(refcount_promise == 0)
                CV_Error(Error::StsInternal, "Asynchronous result producer has been destroyed");
            अगर (!wait_for(timeoutNs))
                वापस false;
        }
        std::unique_lock<std::mutex> lock(mtx);
        अगर (has_परिणाम)
        {
            अगर (!परिणाम_mat.empty())
            {
                dst.move(*परिणाम_mat.लो());
                परिणाम_mat.release();
                परिणाम_is_fetched = true;
                वापस true;
            }
            अगर (!परिणाम_umat.empty())
            {
                dst.move(*परिणाम_umat.लो());
                परिणाम_umat.release();
                परिणाम_is_fetched = true;
                वापस true;
            }
#अगर CV__EXCEPTION_PTR
            अगर (has_exception && exception)
            {
                परिणाम_is_fetched = true;
                std::rethrow_exception(exception);
            }
#पूर्ण_अगर
            अगर (has_exception)
            {
                परिणाम_is_fetched = true;
                फेंको cv_exception;
            }
            CV_Error(Error::StsInternal, "AsyncArray: invalid state of 'has_result = true'");
        }
        CV_Assert(!has_परिणाम);
        CV_Assert(timeoutNs < 0);
        वापस false;
    }

    bool valid() स्थिर CV_NOEXCEPT
    {
        अगर (परिणाम_is_fetched)
            वापस false;
        अगर (refcount_promise == 0 && !has_परिणाम)
            वापस false;
        वापस true;
    }

    bool wait_for(int64 timeoutNs) स्थिर
    {
        CV_Assert(valid());
        अगर (has_परिणाम)
            वापस has_परिणाम;
        अगर (timeoutNs == 0)
            वापस has_परिणाम;
        CV_LOG_INFO(शून्य, "Waiting for async result ...");
        std::unique_lock<std::mutex> lock(mtx);
        स्थिर स्वतः cond_pred = [&]{ वापस has_परिणाम == true; };
        अगर (timeoutNs > 0)
            वापस cond_var.wait_for(lock, std::chrono::nanoseconds(timeoutNs), cond_pred);
        अन्यथा
        {
            cond_var.रुको(lock, cond_pred);
            CV_Assert(has_परिणाम);
            वापस true;
        }
    }

    AsyncArray get_Array_परिणाम()
    {
        CV_Assert(refcount_future == 0);
        AsyncArray परिणाम;
        addrefFuture();
        परिणाम.p = यह;
        future_is_returned = true;
        वापस परिणाम;
    }

    व्योम set_मूल्य(निवेश_Array value)
    {
        अगर (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        std::unique_lock<std::mutex> lock(mtx);
        CV_Assert(!has_परिणाम);
        पूर्णांक k = value.kind();
        अगर (k == निवेश_Array_2::UMAT)
        {
            परिणाम_umat = makePtr<UMat>();
            value.copyTo(*परिणाम_umat.लो());
        }
        अन्यथा
        {
            परिणाम_mat = makePtr<Mat>();
            value.copyTo(*परिणाम_mat.लो());
        }
        has_परिणाम = true;
        cond_var.notify_all();
    }

#अगर CV__EXCEPTION_PTR
    व्योम setException(std::exception_ptr e)
    {
        अगर (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        std::unique_lock<std::mutex> lock(mtx);
        CV_Assert(!has_परिणाम);
        has_exception = true;
        exception = e;
        has_परिणाम = true;
        cond_var.notify_all();
    }
#पूर्ण_अगर

    व्योम setException(स्थिर cv::Exception e)
    {
        अगर (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        std::unique_lock<std::mutex> lock(mtx);
        CV_Assert(!has_परिणाम);
        has_exception = true;
        cv_exception = e;
        has_परिणाम = true;
        cond_var.notify_all();
    }
};

}  // namespace

#अन्यथा  // OPENCV_DISABLE_THREAD_SUPPORT

namespace cv {

// no threading
काष्ठा AsyncArray::Impl
{
    पूर्णांक refcount;
    व्योम addrefFuture() CV_NOEXCEPT { refcount_future++; refcount++; }
    व्योम releaseFuture() CV_NOEXCEPT { refcount_future--; अगर (0 == --refcount) मिटाओ यह; }
    पूर्णांक refcount_future;
    व्योम addrefPromise() CV_NOEXCEPT { refcount_promise++; refcount++; } \
    व्योम releasePromise() CV_NOEXCEPT { refcount_promise--; अगर (0 == --refcount) मिटाओ यह; }
    पूर्णांक refcount_promise;

    mutable bool has_परिणाम; // Mat, UMat or exception

    mutable cv::Ptr<Mat> परिणाम_mat;
    mutable cv::Ptr<UMat> परिणाम_umat;


    bool has_exception;
#अगर CV__EXCEPTION_PTR
    std::exception_ptr exception;
#पूर्ण_अगर
    cv::Exception cv_exception;

    mutable bool परिणाम_is_fetched;

    bool future_is_returned;

    Impl()
        : refcount(1), refcount_future(0), refcount_promise(1)
        , has_परिणाम(false)
        , has_exception(false)
        , परिणाम_is_fetched(false)
        , future_is_returned(false)
    {
        // nothing
    }

    ~Impl()
    {
        अगर (has_परिणाम && !परिणाम_is_fetched)
        {
            CV_LOG_INFO(शून्य, "Asynchronous result has not been fetched");
        }
    }

    bool लो(OutputArray dst, int64 timeoutNs) स्थिर
    {
        CV_Assert(!परिणाम_is_fetched);
        अगर (!has_परिणाम)
        {
            CV_UNUSED(timeoutNs);
            CV_Error(Error::StsError, "Result is not produced (unable to wait for result in OPENCV_DISABLE_THREAD_SUPPORT mode)");
        }
        अगर (!परिणाम_mat.empty())
        {
            dst.move(*परिणाम_mat.लो());
            परिणाम_mat.release();
            परिणाम_is_fetched = true;
            वापस true;
        }
        अगर (!परिणाम_umat.empty())
        {
            dst.move(*परिणाम_umat.लो());
            परिणाम_umat.release();
            परिणाम_is_fetched = true;
            वापस true;
        }
#अगर CV__EXCEPTION_PTR
        अगर (has_exception && exception)
        {
            परिणाम_is_fetched = true;
            std::rethrow_exception(exception);
        }
#पूर्ण_अगर
        अगर (has_exception)
        {
            परिणाम_is_fetched = true;
            फेंको cv_exception;
        }
        CV_Error(Error::StsInternal, "AsyncArray: invalid state of 'has_result = true'");
        वापस false;
    }

    bool valid() स्थिर CV_NOEXCEPT
    {
        अगर (परिणाम_is_fetched)
            वापस false;
        अगर (refcount_promise == 0 && !has_परिणाम)
            वापस false;
        वापस true;
    }

    bool wait_for(int64 timeoutNs) स्थिर
    {
        CV_Assert(valid());
        अगर (has_परिणाम)
            वापस has_परिणाम;
        अगर (timeoutNs == 0)
            वापस has_परिणाम;
        CV_Error(Error::StsError, "Unable to wait in OPENCV_DISABLE_THREAD_SUPPORT mode");
    }

    AsyncArray get_Array_परिणाम()
    {
        CV_Assert(refcount_future == 0);
        AsyncArray परिणाम;
        addrefFuture();
        परिणाम.p = यह;
        future_is_returned = true;
        वापस परिणाम;
    }

    व्योम set_मूल्य(निवेश_Array value)
    {
        अगर (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        CV_Assert(!has_परिणाम);
        पूर्णांक k = value.kind();
        अगर (k == निवेश_Array_2::UMAT)
        {
            परिणाम_umat = makePtr<UMat>();
            value.copyTo(*परिणाम_umat.लो());
        }
        अन्यथा
        {
            परिणाम_mat = makePtr<Mat>();
            value.copyTo(*परिणाम_mat.लो());
        }
        has_परिणाम = true;
    }

#अगर CV__EXCEPTION_PTR
    व्योम setException(std::exception_ptr e)
    {
        अगर (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        CV_Assert(!has_परिणाम);
        has_exception = true;
        exception = e;
        has_परिणाम = true;
    }
#पूर्ण_अगर

    व्योम setException(स्थिर cv::Exception e)
    {
        अगर (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        CV_Assert(!has_परिणाम);
        has_exception = true;
        cv_exception = e;
        has_परिणाम = true;
    }
};

}

#पूर्ण_अगर  // OPENCV_DISABLE_THREAD_SUPPORT

namespace cv {

AsyncArray::AsyncArray() CV_NOEXCEPT
    : p(शून्य)
{
}

AsyncArray::~AsyncArray() CV_NOEXCEPT
{
    release();
}

AsyncArray::AsyncArray(स्थिर AsyncArray& o) CV_NOEXCEPT
    : p(o.p)
{
    अगर (p)
        p->addrefFuture();
}

AsyncArray& AsyncArray::चालक=(स्थिर AsyncArray& o) CV_NOEXCEPT
{
    Impl* newp = o.p;
    अगर (newp)
        newp->addrefFuture();
    release();
    p = newp;
    वापस *यह;
}

व्योम AsyncArray::release() CV_NOEXCEPT
{
    Impl* impl = p;
    p = शून्य;
    अगर (impl)
        impl->releaseFuture();
}

bool AsyncArray::लो(OutputArray dst, int64 timeoutNs) स्थिर
{
    CV_Assert(p);
    वापस p->लो(dst, timeoutNs);
}

व्योम AsyncArray::लो(OutputArray dst) स्थिर
{
    CV_Assert(p);
    bool res = p->लो(dst, -1);
    CV_Assert(res);
}

bool AsyncArray::wait_for(int64 timeoutNs) स्थिर
{
    CV_Assert(p);
    वापस p->wait_for(timeoutNs);
}

bool AsyncArray::valid() स्थिर CV_NOEXCEPT
{
    अगर (!p) वापस false;
    वापस p->valid();
}


//
// AsyncPromise
//

AsyncPromise::AsyncPromise() CV_NOEXCEPT
    : p(नया AsyncArray::Impl())
{
}

AsyncPromise::~AsyncPromise() CV_NOEXCEPT
{
    release();
}

AsyncPromise::AsyncPromise(स्थिर AsyncPromise& o) CV_NOEXCEPT
    : p(o.p)
{
    अगर (p)
        p->addrefPromise();
}

AsyncPromise& AsyncPromise::चालक=(स्थिर AsyncPromise& o) CV_NOEXCEPT
{
    Impl* newp = o.p;
    अगर (newp)
        newp->addrefPromise();
    release();
    p = newp;
    वापस *यह;
}

व्योम AsyncPromise::release() CV_NOEXCEPT
{
    Impl* impl = p;
    p = शून्य;
    अगर (impl)
        impl->releasePromise();
}

AsyncArray AsyncPromise::get_Array_परिणाम()
{
    CV_Assert(p);
    वापस p->get_Array_परिणाम();
}

व्योम AsyncPromise::set_मूल्य(निवेश_Array value)
{
    CV_Assert(p);
    वापस p->set_मूल्य(value);
}

व्योम AsyncPromise::setException(स्थिर cv::Exception& exception)
{
    CV_Assert(p);
    वापस p->setException(exception);
}

#अगर CV__EXCEPTION_PTR
व्योम AsyncPromise::setException(std::exception_ptr exception)
{
    CV_Assert(p);
    वापस p->setException(exception);
}
#पूर्ण_अगर

} // namespace
