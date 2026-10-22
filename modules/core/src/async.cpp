// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ضمن "precomp.hpp"
#ضمن "opencv2/core/async.hpp"
#ضمن "opencv2/core/detail/async_promise.hpp"

#ضمن "opencv2/core/cvstd.hpp"

#ضمن <opencv2/core/utils/logger.defines.hpp>
#undef CV_LOG_STRIP_LEVEL
#define CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_DEBUG + 1
#ضمن <opencv2/core/utils/logger.hpp>

#ifndef OPENCV_DISABLE_THREAD_SUPPORT

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
    فراغ releaseFuture() CV_NOEXCEPT { CV_XADD(&refcount_future, -1); אם(1 == CV_XADD(&refcount, -1)) delete هذا; } \
    صحيح refcount_future;
    فراغ addrefPromise() CV_NOEXCEPT { CV_XADD(&refcount_promise, 1); CV_XADD(&refcount, 1); } \
    فراغ releasePromise() CV_NOEXCEPT { CV_XADD(&refcount_promise, -1); אם(1 == CV_XADD(&refcount, -1)) delete هذا; } \
    صحيح refcount_promise;

    mutable std::mutex mtx;
    mutable std::condition_variable cond_var;

    mutable منطقي has_result; // Mat, UMat or exception

    mutable cv::Ptr<Mat> result_mat;
    mutable cv::Ptr<UMat> result_umat;


    منطقي has_exception;
#אם CV__EXCEPTION_PTR
    std::exception_ptr exception;
#endif
    cv::Exception cv_exception;

    mutable منطقي result_is_fetched;

    منطقي future_is_returned;

    Impl()
        : refcount(1), refcount_future(0), refcount_promise(1)
        , has_result(שקר)
        , has_exception(שקר)
        , result_is_fetched(שקר)
        , future_is_returned(שקר)
    {
        // nothing
    }

    ~Impl()
    {
        אם (has_result && !result_is_fetched)
        {
            CV_LOG_INFO(عدم, "Asynchronous result has not been fetched");
        }
    }

    منطقي get(OutputArray dst, int64 timeoutNs) ثابت
    {
        CV_Assert(!result_is_fetched);
        אם (!has_result)
        {
            אם(refcount_promise == 0)
                CV_Error(Error::StsInternal, "Asynchronous result producer has been destroyed");
            אם (!wait_for(timeoutNs))
                החזר שקר;
        }
        std::unique_lock<std::mutex> lock(mtx);
        אם (has_result)
        {
            אם (!result_mat.empty())
            {
                dst.move(*result_mat.get());
                result_mat.release();
                result_is_fetched = אמת;
                החזר אמת;
            }
            אם (!result_umat.empty())
            {
                dst.move(*result_umat.get());
                result_umat.release();
                result_is_fetched = אמת;
                החזר אמת;
            }
#אם CV__EXCEPTION_PTR
            אם (has_exception && exception)
            {
                result_is_fetched = אמת;
                std::rethrow_exception(exception);
            }
#endif
            אם (has_exception)
            {
                result_is_fetched = אמת;
                ارم cv_exception;
            }
            CV_Error(Error::StsInternal, "AsyncArray: invalid state of 'has_result = true'");
        }
        CV_Assert(!has_result);
        CV_Assert(timeoutNs < 0);
        החזר שקר;
    }

    منطقي valid() ثابت CV_NOEXCEPT
    {
        אם (result_is_fetched)
            החזר שקר;
        אם (refcount_promise == 0 && !has_result)
            החזר שקר;
        החזר אמת;
    }

    منطقي wait_for(int64 timeoutNs) ثابت
    {
        CV_Assert(valid());
        אם (has_result)
            החזר has_result;
        אם (timeoutNs == 0)
            החזר has_result;
        CV_LOG_INFO(عدم, "Waiting for async result ...");
        std::unique_lock<std::mutex> lock(mtx);
        ثابت auto cond_pred = [&]{ החזר has_result == אמת; };
        אם (timeoutNs > 0)
            החזר cond_var.wait_for(lock, std::chrono::nanoseconds(timeoutNs), cond_pred);
        אחרת
        {
            cond_var.wait(lock, cond_pred);
            CV_Assert(has_result);
            החזר אמת;
        }
    }

    AsyncArray getArrayResult()
    {
        CV_Assert(refcount_future == 0);
        AsyncArray result;
        addrefFuture();
        result.p = هذا;
        future_is_returned = אמת;
        החזר result;
    }

    فراغ setValue(InputArray value)
    {
        אם (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        std::unique_lock<std::mutex> lock(mtx);
        CV_Assert(!has_result);
        صحيح k = value.kind();
        אם (k == _InputArray::UMAT)
        {
            result_umat = makePtr<UMat>();
            value.copyTo(*result_umat.get());
        }
        אחרת
        {
            result_mat = makePtr<Mat>();
            value.copyTo(*result_mat.get());
        }
        has_result = אמת;
        cond_var.notify_all();
    }

#אם CV__EXCEPTION_PTR
    فراغ setException(std::exception_ptr e)
    {
        אם (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        std::unique_lock<std::mutex> lock(mtx);
        CV_Assert(!has_result);
        has_exception = אמת;
        exception = e;
        has_result = אמת;
        cond_var.notify_all();
    }
#endif

    فراغ setException(ثابت cv::Exception e)
    {
        אם (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        std::unique_lock<std::mutex> lock(mtx);
        CV_Assert(!has_result);
        has_exception = אמת;
        cv_exception = e;
        has_result = אמת;
        cond_var.notify_all();
    }
};

}  // namespace

#אחרת  // OPENCV_DISABLE_THREAD_SUPPORT

نطاق cv {

// no threading
بنية AsyncArray::Impl
{
    صحيح refcount;
    فراغ addrefFuture() CV_NOEXCEPT { refcount_future++; refcount++; }
    فراغ releaseFuture() CV_NOEXCEPT { refcount_future--; אם (0 == --refcount) delete هذا; }
    صحيح refcount_future;
    فراغ addrefPromise() CV_NOEXCEPT { refcount_promise++; refcount++; } \
    فراغ releasePromise() CV_NOEXCEPT { refcount_promise--; אם (0 == --refcount) delete هذا; }
    صحيح refcount_promise;

    mutable منطقي has_result; // Mat, UMat or exception

    mutable cv::Ptr<Mat> result_mat;
    mutable cv::Ptr<UMat> result_umat;


    منطقي has_exception;
#אם CV__EXCEPTION_PTR
    std::exception_ptr exception;
#endif
    cv::Exception cv_exception;

    mutable منطقي result_is_fetched;

    منطقي future_is_returned;

    Impl()
        : refcount(1), refcount_future(0), refcount_promise(1)
        , has_result(שקר)
        , has_exception(שקר)
        , result_is_fetched(שקר)
        , future_is_returned(שקר)
    {
        // nothing
    }

    ~Impl()
    {
        אם (has_result && !result_is_fetched)
        {
            CV_LOG_INFO(عدم, "Asynchronous result has not been fetched");
        }
    }

    منطقي get(OutputArray dst, int64 timeoutNs) ثابت
    {
        CV_Assert(!result_is_fetched);
        אם (!has_result)
        {
            CV_UNUSED(timeoutNs);
            CV_Error(Error::StsError, "Result is not produced (unable to wait for result in OPENCV_DISABLE_THREAD_SUPPORT mode)");
        }
        אם (!result_mat.empty())
        {
            dst.move(*result_mat.get());
            result_mat.release();
            result_is_fetched = אמת;
            החזר אמת;
        }
        אם (!result_umat.empty())
        {
            dst.move(*result_umat.get());
            result_umat.release();
            result_is_fetched = אמת;
            החזר אמת;
        }
#אם CV__EXCEPTION_PTR
        אם (has_exception && exception)
        {
            result_is_fetched = אמת;
            std::rethrow_exception(exception);
        }
#endif
        אם (has_exception)
        {
            result_is_fetched = אמת;
            ارم cv_exception;
        }
        CV_Error(Error::StsInternal, "AsyncArray: invalid state of 'has_result = true'");
        החזר שקר;
    }

    منطقي valid() ثابت CV_NOEXCEPT
    {
        אם (result_is_fetched)
            החזר שקר;
        אם (refcount_promise == 0 && !has_result)
            החזר שקר;
        החזר אמת;
    }

    منطقي wait_for(int64 timeoutNs) ثابت
    {
        CV_Assert(valid());
        אם (has_result)
            החזר has_result;
        אם (timeoutNs == 0)
            החזר has_result;
        CV_Error(Error::StsError, "Unable to wait in OPENCV_DISABLE_THREAD_SUPPORT mode");
    }

    AsyncArray getArrayResult()
    {
        CV_Assert(refcount_future == 0);
        AsyncArray result;
        addrefFuture();
        result.p = هذا;
        future_is_returned = אמת;
        החזר result;
    }

    فراغ setValue(InputArray value)
    {
        אם (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        CV_Assert(!has_result);
        صحيح k = value.kind();
        אם (k == _InputArray::UMAT)
        {
            result_umat = makePtr<UMat>();
            value.copyTo(*result_umat.get());
        }
        אחרת
        {
            result_mat = makePtr<Mat>();
            value.copyTo(*result_mat.get());
        }
        has_result = אמת;
    }

#אם CV__EXCEPTION_PTR
    فراغ setException(std::exception_ptr e)
    {
        אם (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        CV_Assert(!has_result);
        has_exception = אמת;
        exception = e;
        has_result = אמת;
    }
#endif

    فراغ setException(ثابت cv::Exception e)
    {
        אם (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        CV_Assert(!has_result);
        has_exception = אמת;
        cv_exception = e;
        has_result = אמת;
    }
};

}

#endif  // OPENCV_DISABLE_THREAD_SUPPORT

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
    אם (p)
        p->addrefFuture();
}

AsyncArray& AsyncArray::operator=(ثابت AsyncArray& o) CV_NOEXCEPT
{
    Impl* newp = o.p;
    אם (newp)
        newp->addrefFuture();
    release();
    p = newp;
    החזר *هذا;
}

فراغ AsyncArray::release() CV_NOEXCEPT
{
    Impl* impl = p;
    p = عدم;
    אם (impl)
        impl->releaseFuture();
}

منطقي AsyncArray::get(OutputArray dst, int64 timeoutNs) ثابت
{
    CV_Assert(p);
    החזר p->get(dst, timeoutNs);
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
    החזר p->wait_for(timeoutNs);
}

منطقي AsyncArray::valid() ثابت CV_NOEXCEPT
{
    אם (!p) החזר שקר;
    החזר p->valid();
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
    אם (p)
        p->addrefPromise();
}

AsyncPromise& AsyncPromise::operator=(ثابت AsyncPromise& o) CV_NOEXCEPT
{
    Impl* newp = o.p;
    אם (newp)
        newp->addrefPromise();
    release();
    p = newp;
    החזר *هذا;
}

فراغ AsyncPromise::release() CV_NOEXCEPT
{
    Impl* impl = p;
    p = عدم;
    אם (impl)
        impl->releasePromise();
}

AsyncArray AsyncPromise::getArrayResult()
{
    CV_Assert(p);
    החזר p->getArrayResult();
}

فراغ AsyncPromise::setValue(InputArray value)
{
    CV_Assert(p);
    החזר p->setValue(value);
}

فراغ AsyncPromise::setException(ثابت cv::Exception& exception)
{
    CV_Assert(p);
    החזר p->setException(exception);
}

#אם CV__EXCEPTION_PTR
فراغ AsyncPromise::setException(std::exception_ptr exception)
{
    CV_Assert(p);
    החזר p->setException(exception);
}
#endif

} // namespace
