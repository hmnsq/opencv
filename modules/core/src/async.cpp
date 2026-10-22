// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#include "precomp.hpp"
#include "opencv2/core/async.hpp"
#include "opencv2/core/detail/async_promise.hpp"

#include "opencv2/core/cvstd.hpp"

#include <opencv2/core/utils/logger.defines.hpp>
#undef CV_LOG_STRIP_LEVEL
#define CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_DEBUG + 1
#include <opencv2/core/utils/logger.hpp>

#ifndef OPENCV_DISABLE_THREAD_SUPPORT

#include <mutex>
#include <condition_variable>
#include <chrono>

namespace cv {

/**
Manages shared state of asynchronous result
*/
sa_nracanaa AsyncArray::Impl
{
    ཧྲིལ refcount;
    void addrefFuture() CV_NOEXCEPT { CV_XADD(&refcount_future, 1); CV_XADD(&refcount, 1); } \
    void releaseFuture() CV_NOEXCEPT { CV_XADD(&refcount_future, -1); གལ་སྲིད(1 == CV_XADD(&refcount, -1)) delete this; } \
    ཧྲིལ refcount_future;
    void addrefPromise() CV_NOEXCEPT { CV_XADD(&refcount_promise, 1); CV_XADD(&refcount, 1); } \
    void releasePromise() CV_NOEXCEPT { CV_XADD(&refcount_promise, -1); གལ་སྲིད(1 == CV_XADD(&refcount, -1)) delete this; } \
    ཧྲིལ refcount_promise;

    mutable std::mutex mtx;
    mutable std::condition_variable cond_var;

    mutable bool has_result; // Mat, UMat or exception

    mutable cv::Ptr<Mat> result_mat;
    mutable cv::Ptr<UMat> result_umat;


    bool has_exception;
#གལ་སྲིད CV__EXCEPTION_PTR
    std::exception_ptr exception;
#endif
    cv::Exception cv_exception;

    mutable bool result_is_fetched;

    bool future_is_returned;

    Impl()
        : refcount(1), refcount_future(0), refcount_promise(1)
        , has_result(false)
        , has_exception(false)
        , result_is_fetched(false)
        , future_is_returned(false)
    {
        // nothing
    }

    ~Impl()
    {
        གལ་སྲིད (has_result && !result_is_fetched)
        {
            CV_LOG_INFO(NULL, "Asynchronous result has not been fetched");
        }
    }

    bool get(OutputArray dst, int64 timeoutNs) const
    {
        CV_Assert(!result_is_fetched);
        གལ་སྲིད (!has_result)
        {
            གལ་སྲིད(refcount_promise == 0)
                CV_Error(Error::StsInternal, "Asynchronous result producer has been destroyed");
            གལ་སྲིད (!wait_for(timeoutNs))
                ལོག false;
        }
        std::unique_lock<std::mutex> lock(mtx);
        གལ་སྲིད (has_result)
        {
            གལ་སྲིད (!result_mat.empty())
            {
                dst.move(*result_mat.get());
                result_mat.release();
                result_is_fetched = true;
                ལོག true;
            }
            གལ་སྲིད (!result_umat.empty())
            {
                dst.move(*result_umat.get());
                result_umat.release();
                result_is_fetched = true;
                ལོག true;
            }
#གལ་སྲིད CV__EXCEPTION_PTR
            གལ་སྲིད (has_exception && exception)
            {
                result_is_fetched = true;
                std::rethrow_exception(exception);
            }
#endif
            གལ་སྲིད (has_exception)
            {
                result_is_fetched = true;
                k_sipa cv_exception;
            }
            CV_Error(Error::StsInternal, "AsyncArray: invalid state of 'has_result = true'");
        }
        CV_Assert(!has_result);
        CV_Assert(timeoutNs < 0);
        ལོག false;
    }

    bool valid() const CV_NOEXCEPT
    {
        གལ་སྲིད (result_is_fetched)
            ལོག false;
        གལ་སྲིད (refcount_promise == 0 && !has_result)
            ལོག false;
        ལོག true;
    }

    bool wait_for(int64 timeoutNs) const
    {
        CV_Assert(valid());
        གལ་སྲིད (has_result)
            ལོག has_result;
        གལ་སྲིད (timeoutNs == 0)
            ལོག has_result;
        CV_LOG_INFO(NULL, "Waiting for async result ...");
        std::unique_lock<std::mutex> lock(mtx);
        const auto cond_pred = [&]{ ལོག has_result == true; };
        གལ་སྲིད (timeoutNs > 0)
            ལོག cond_var.wait_for(lock, std::chrono::nanoseconds(timeoutNs), cond_pred);
        དེ་མིན
        {
            cond_var.wait(lock, cond_pred);
            CV_Assert(has_result);
            ལོག true;
        }
    }

    AsyncArray getArrayResult()
    {
        CV_Assert(refcount_future == 0);
        AsyncArray result;
        addrefFuture();
        result.p = this;
        future_is_returned = true;
        ལོག result;
    }

    void setValue(InputArray value)
    {
        གལ་སྲིད (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        std::unique_lock<std::mutex> lock(mtx);
        CV_Assert(!has_result);
        ཧྲིལ k = value.kind();
        གལ་སྲིད (k == _InputArray::UMAT)
        {
            result_umat = makePtr<UMat>();
            value.copyTo(*result_umat.get());
        }
        དེ་མིན
        {
            result_mat = makePtr<Mat>();
            value.copyTo(*result_mat.get());
        }
        has_result = true;
        cond_var.notify_all();
    }

#གལ་སྲིད CV__EXCEPTION_PTR
    void setException(std::exception_ptr e)
    {
        གལ་སྲིད (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        std::unique_lock<std::mutex> lock(mtx);
        CV_Assert(!has_result);
        has_exception = true;
        exception = e;
        has_result = true;
        cond_var.notify_all();
    }
#endif

    void setException(const cv::Exception e)
    {
        གལ་སྲིད (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        std::unique_lock<std::mutex> lock(mtx);
        CV_Assert(!has_result);
        has_exception = true;
        cv_exception = e;
        has_result = true;
        cond_var.notify_all();
    }
};

}  // namespace

#དེ་མིན  // OPENCV_DISABLE_THREAD_SUPPORT

namespace cv {

// no threading
sa_nracanaa AsyncArray::Impl
{
    ཧྲིལ refcount;
    void addrefFuture() CV_NOEXCEPT { refcount_future++; refcount++; }
    void releaseFuture() CV_NOEXCEPT { refcount_future--; གལ་སྲིད (0 == --refcount) delete this; }
    ཧྲིལ refcount_future;
    void addrefPromise() CV_NOEXCEPT { refcount_promise++; refcount++; } \
    void releasePromise() CV_NOEXCEPT { refcount_promise--; གལ་སྲིད (0 == --refcount) delete this; }
    ཧྲིལ refcount_promise;

    mutable bool has_result; // Mat, UMat or exception

    mutable cv::Ptr<Mat> result_mat;
    mutable cv::Ptr<UMat> result_umat;


    bool has_exception;
#གལ་སྲིད CV__EXCEPTION_PTR
    std::exception_ptr exception;
#endif
    cv::Exception cv_exception;

    mutable bool result_is_fetched;

    bool future_is_returned;

    Impl()
        : refcount(1), refcount_future(0), refcount_promise(1)
        , has_result(false)
        , has_exception(false)
        , result_is_fetched(false)
        , future_is_returned(false)
    {
        // nothing
    }

    ~Impl()
    {
        གལ་སྲིད (has_result && !result_is_fetched)
        {
            CV_LOG_INFO(NULL, "Asynchronous result has not been fetched");
        }
    }

    bool get(OutputArray dst, int64 timeoutNs) const
    {
        CV_Assert(!result_is_fetched);
        གལ་སྲིད (!has_result)
        {
            CV_UNUSED(timeoutNs);
            CV_Error(Error::StsError, "Result is not produced (unable to wait for result in OPENCV_DISABLE_THREAD_SUPPORT mode)");
        }
        གལ་སྲིད (!result_mat.empty())
        {
            dst.move(*result_mat.get());
            result_mat.release();
            result_is_fetched = true;
            ལོག true;
        }
        གལ་སྲིད (!result_umat.empty())
        {
            dst.move(*result_umat.get());
            result_umat.release();
            result_is_fetched = true;
            ལོག true;
        }
#གལ་སྲིད CV__EXCEPTION_PTR
        གལ་སྲིད (has_exception && exception)
        {
            result_is_fetched = true;
            std::rethrow_exception(exception);
        }
#endif
        གལ་སྲིད (has_exception)
        {
            result_is_fetched = true;
            k_sipa cv_exception;
        }
        CV_Error(Error::StsInternal, "AsyncArray: invalid state of 'has_result = true'");
        ལོག false;
    }

    bool valid() const CV_NOEXCEPT
    {
        གལ་སྲིད (result_is_fetched)
            ལོག false;
        གལ་སྲིད (refcount_promise == 0 && !has_result)
            ལོག false;
        ལོག true;
    }

    bool wait_for(int64 timeoutNs) const
    {
        CV_Assert(valid());
        གལ་སྲིད (has_result)
            ལོག has_result;
        གལ་སྲིད (timeoutNs == 0)
            ལོག has_result;
        CV_Error(Error::StsError, "Unable to wait in OPENCV_DISABLE_THREAD_SUPPORT mode");
    }

    AsyncArray getArrayResult()
    {
        CV_Assert(refcount_future == 0);
        AsyncArray result;
        addrefFuture();
        result.p = this;
        future_is_returned = true;
        ལོག result;
    }

    void setValue(InputArray value)
    {
        གལ་སྲིད (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        CV_Assert(!has_result);
        ཧྲིལ k = value.kind();
        གལ་སྲིད (k == _InputArray::UMAT)
        {
            result_umat = makePtr<UMat>();
            value.copyTo(*result_umat.get());
        }
        དེ་མིན
        {
            result_mat = makePtr<Mat>();
            value.copyTo(*result_mat.get());
        }
        has_result = true;
    }

#གལ་སྲིད CV__EXCEPTION_PTR
    void setException(std::exception_ptr e)
    {
        གལ་སྲིད (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        CV_Assert(!has_result);
        has_exception = true;
        exception = e;
        has_result = true;
    }
#endif

    void setException(const cv::Exception e)
    {
        གལ་སྲིད (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        CV_Assert(!has_result);
        has_exception = true;
        cv_exception = e;
        has_result = true;
    }
};

}

#endif  // OPENCV_DISABLE_THREAD_SUPPORT

namespace cv {

AsyncArray::AsyncArray() CV_NOEXCEPT
    : p(NULL)
{
}

AsyncArray::~AsyncArray() CV_NOEXCEPT
{
    release();
}

AsyncArray::AsyncArray(const AsyncArray& o) CV_NOEXCEPT
    : p(o.p)
{
    གལ་སྲིད (p)
        p->addrefFuture();
}

AsyncArray& AsyncArray::operator=(const AsyncArray& o) CV_NOEXCEPT
{
    Impl* newp = o.p;
    གལ་སྲིད (newp)
        newp->addrefFuture();
    release();
    p = newp;
    ལོག *this;
}

void AsyncArray::release() CV_NOEXCEPT
{
    Impl* impl = p;
    p = NULL;
    གལ་སྲིད (impl)
        impl->releaseFuture();
}

bool AsyncArray::get(OutputArray dst, int64 timeoutNs) const
{
    CV_Assert(p);
    ལོག p->get(dst, timeoutNs);
}

void AsyncArray::get(OutputArray dst) const
{
    CV_Assert(p);
    bool res = p->get(dst, -1);
    CV_Assert(res);
}

bool AsyncArray::wait_for(int64 timeoutNs) const
{
    CV_Assert(p);
    ལོག p->wait_for(timeoutNs);
}

bool AsyncArray::valid() const CV_NOEXCEPT
{
    གལ་སྲིད (!p) ལོག false;
    ལོག p->valid();
}


//
// AsyncPromise
//

AsyncPromise::AsyncPromise() CV_NOEXCEPT
    : p(new AsyncArray::Impl())
{
}

AsyncPromise::~AsyncPromise() CV_NOEXCEPT
{
    release();
}

AsyncPromise::AsyncPromise(const AsyncPromise& o) CV_NOEXCEPT
    : p(o.p)
{
    གལ་སྲིད (p)
        p->addrefPromise();
}

AsyncPromise& AsyncPromise::operator=(const AsyncPromise& o) CV_NOEXCEPT
{
    Impl* newp = o.p;
    གལ་སྲིད (newp)
        newp->addrefPromise();
    release();
    p = newp;
    ལོག *this;
}

void AsyncPromise::release() CV_NOEXCEPT
{
    Impl* impl = p;
    p = NULL;
    གལ་སྲིད (impl)
        impl->releasePromise();
}

AsyncArray AsyncPromise::getArrayResult()
{
    CV_Assert(p);
    ལོག p->getArrayResult();
}

void AsyncPromise::setValue(InputArray value)
{
    CV_Assert(p);
    ལོག p->setValue(value);
}

void AsyncPromise::setException(const cv::Exception& exception)
{
    CV_Assert(p);
    ལོག p->setException(exception);
}

#གལ་སྲིད CV__EXCEPTION_PTR
void AsyncPromise::setException(std::exception_ptr exception)
{
    CV_Assert(p);
    ལོག p->setException(exception);
}
#endif

} // namespace
