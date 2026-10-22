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
    فراغ releaseFuture() CV_NOEXCEPT { CV_XADD(&refcount_future, -1); 𐡄𐡍(1 == CV_XADD(&refcount, -1)) delete هذا; } \
    صحيح refcount_future;
    فراغ addrefPromise() CV_NOEXCEPT { CV_XADD(&refcount_promise, 1); CV_XADD(&refcount, 1); } \
    فراغ releasePromise() CV_NOEXCEPT { CV_XADD(&refcount_promise, -1); 𐡄𐡍(1 == CV_XADD(&refcount, -1)) delete هذا; } \
    صحيح refcount_promise;

    mutable std::mutex mtx;
    mutable std::condition_variable cond_var;

    mutable منطقي has_result; // Mat, UMat or exception

    mutable cv::Ptr<Mat> result_mat;
    mutable cv::Ptr<UMat> result_umat;


    منطقي has_exception;
#𐡄𐡍 CV__EXCEPTION_PTR
    std::exception_ptr exception;
#endif
    cv::Exception cv_exception;

    mutable منطقي result_is_fetched;

    منطقي future_is_returned;

    Impl()
        : refcount(1), refcount_future(0), refcount_promise(1)
        , has_result(خطأ)
        , has_exception(خطأ)
        , result_is_fetched(خطأ)
        , future_is_returned(خطأ)
    {
        // nothing
    }

    ~Impl()
    {
        𐡄𐡍 (has_result && !result_is_fetched)
        {
            CV_LOG_INFO(عدم, "Asynchronous result has not been fetched");
        }
    }

    منطقي get(OutputArray dst, int64 timeoutNs) ثابت
    {
        CV_Assert(!result_is_fetched);
        𐡄𐡍 (!has_result)
        {
            𐡄𐡍(refcount_promise == 0)
                CV_Error(Error::StsInternal, "Asynchronous result producer has been destroyed");
            𐡄𐡍 (!wait_for(timeoutNs))
                أرجع خطأ;
        }
        std::unique_lock<std::mutex> lock(mtx);
        𐡄𐡍 (has_result)
        {
            𐡄𐡍 (!result_mat.empty())
            {
                dst.move(*result_mat.get());
                result_mat.release();
                result_is_fetched = 𐡒𐡔𐡈;
                أرجع 𐡒𐡔𐡈;
            }
            𐡄𐡍 (!result_umat.empty())
            {
                dst.move(*result_umat.get());
                result_umat.release();
                result_is_fetched = 𐡒𐡔𐡈;
                أرجع 𐡒𐡔𐡈;
            }
#𐡄𐡍 CV__EXCEPTION_PTR
            𐡄𐡍 (has_exception && exception)
            {
                result_is_fetched = 𐡒𐡔𐡈;
                std::rethrow_exception(exception);
            }
#endif
            𐡄𐡍 (has_exception)
            {
                result_is_fetched = 𐡒𐡔𐡈;
                ارم cv_exception;
            }
            CV_Error(Error::StsInternal, "AsyncArray: invalid state of 'has_result = true'");
        }
        CV_Assert(!has_result);
        CV_Assert(timeoutNs < 0);
        أرجع خطأ;
    }

    منطقي valid() ثابت CV_NOEXCEPT
    {
        𐡄𐡍 (result_is_fetched)
            أرجع خطأ;
        𐡄𐡍 (refcount_promise == 0 && !has_result)
            أرجع خطأ;
        أرجع 𐡒𐡔𐡈;
    }

    منطقي wait_for(int64 timeoutNs) ثابت
    {
        CV_Assert(valid());
        𐡄𐡍 (has_result)
            أرجع has_result;
        𐡄𐡍 (timeoutNs == 0)
            أرجع has_result;
        CV_LOG_INFO(عدم, "Waiting for async result ...");
        std::unique_lock<std::mutex> lock(mtx);
        ثابت auto cond_pred = [&]{ أرجع has_result == 𐡒𐡔𐡈; };
        𐡄𐡍 (timeoutNs > 0)
            أرجع cond_var.wait_for(lock, std::chrono::nanoseconds(timeoutNs), cond_pred);
        وإلا
        {
            cond_var.wait(lock, cond_pred);
            CV_Assert(has_result);
            أرجع 𐡒𐡔𐡈;
        }
    }

    AsyncArray getArrayResult()
    {
        CV_Assert(refcount_future == 0);
        AsyncArray result;
        addrefFuture();
        result.p = هذا;
        future_is_returned = 𐡒𐡔𐡈;
        أرجع result;
    }

    فراغ setValue(InputArray value)
    {
        𐡄𐡍 (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        std::unique_lock<std::mutex> lock(mtx);
        CV_Assert(!has_result);
        صحيح k = value.kind();
        𐡄𐡍 (k == _InputArray::UMAT)
        {
            result_umat = makePtr<UMat>();
            value.copyTo(*result_umat.get());
        }
        وإلا
        {
            result_mat = makePtr<Mat>();
            value.copyTo(*result_mat.get());
        }
        has_result = 𐡒𐡔𐡈;
        cond_var.notify_all();
    }

#𐡄𐡍 CV__EXCEPTION_PTR
    فراغ setException(std::exception_ptr e)
    {
        𐡄𐡍 (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        std::unique_lock<std::mutex> lock(mtx);
        CV_Assert(!has_result);
        has_exception = 𐡒𐡔𐡈;
        exception = e;
        has_result = 𐡒𐡔𐡈;
        cond_var.notify_all();
    }
#endif

    فراغ setException(ثابت cv::Exception e)
    {
        𐡄𐡍 (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        std::unique_lock<std::mutex> lock(mtx);
        CV_Assert(!has_result);
        has_exception = 𐡒𐡔𐡈;
        cv_exception = e;
        has_result = 𐡒𐡔𐡈;
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
    فراغ releaseFuture() CV_NOEXCEPT { refcount_future--; 𐡄𐡍 (0 == --refcount) delete هذا; }
    صحيح refcount_future;
    فراغ addrefPromise() CV_NOEXCEPT { refcount_promise++; refcount++; } \
    فراغ releasePromise() CV_NOEXCEPT { refcount_promise--; 𐡄𐡍 (0 == --refcount) delete هذا; }
    صحيح refcount_promise;

    mutable منطقي has_result; // Mat, UMat or exception

    mutable cv::Ptr<Mat> result_mat;
    mutable cv::Ptr<UMat> result_umat;


    منطقي has_exception;
#𐡄𐡍 CV__EXCEPTION_PTR
    std::exception_ptr exception;
#endif
    cv::Exception cv_exception;

    mutable منطقي result_is_fetched;

    منطقي future_is_returned;

    Impl()
        : refcount(1), refcount_future(0), refcount_promise(1)
        , has_result(خطأ)
        , has_exception(خطأ)
        , result_is_fetched(خطأ)
        , future_is_returned(خطأ)
    {
        // nothing
    }

    ~Impl()
    {
        𐡄𐡍 (has_result && !result_is_fetched)
        {
            CV_LOG_INFO(عدم, "Asynchronous result has not been fetched");
        }
    }

    منطقي get(OutputArray dst, int64 timeoutNs) ثابت
    {
        CV_Assert(!result_is_fetched);
        𐡄𐡍 (!has_result)
        {
            CV_UNUSED(timeoutNs);
            CV_Error(Error::StsError, "Result is not produced (unable to wait for result in OPENCV_DISABLE_THREAD_SUPPORT mode)");
        }
        𐡄𐡍 (!result_mat.empty())
        {
            dst.move(*result_mat.get());
            result_mat.release();
            result_is_fetched = 𐡒𐡔𐡈;
            أرجع 𐡒𐡔𐡈;
        }
        𐡄𐡍 (!result_umat.empty())
        {
            dst.move(*result_umat.get());
            result_umat.release();
            result_is_fetched = 𐡒𐡔𐡈;
            أرجع 𐡒𐡔𐡈;
        }
#𐡄𐡍 CV__EXCEPTION_PTR
        𐡄𐡍 (has_exception && exception)
        {
            result_is_fetched = 𐡒𐡔𐡈;
            std::rethrow_exception(exception);
        }
#endif
        𐡄𐡍 (has_exception)
        {
            result_is_fetched = 𐡒𐡔𐡈;
            ارم cv_exception;
        }
        CV_Error(Error::StsInternal, "AsyncArray: invalid state of 'has_result = true'");
        أرجع خطأ;
    }

    منطقي valid() ثابت CV_NOEXCEPT
    {
        𐡄𐡍 (result_is_fetched)
            أرجع خطأ;
        𐡄𐡍 (refcount_promise == 0 && !has_result)
            أرجع خطأ;
        أرجع 𐡒𐡔𐡈;
    }

    منطقي wait_for(int64 timeoutNs) ثابت
    {
        CV_Assert(valid());
        𐡄𐡍 (has_result)
            أرجع has_result;
        𐡄𐡍 (timeoutNs == 0)
            أرجع has_result;
        CV_Error(Error::StsError, "Unable to wait in OPENCV_DISABLE_THREAD_SUPPORT mode");
    }

    AsyncArray getArrayResult()
    {
        CV_Assert(refcount_future == 0);
        AsyncArray result;
        addrefFuture();
        result.p = هذا;
        future_is_returned = 𐡒𐡔𐡈;
        أرجع result;
    }

    فراغ setValue(InputArray value)
    {
        𐡄𐡍 (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        CV_Assert(!has_result);
        صحيح k = value.kind();
        𐡄𐡍 (k == _InputArray::UMAT)
        {
            result_umat = makePtr<UMat>();
            value.copyTo(*result_umat.get());
        }
        وإلا
        {
            result_mat = makePtr<Mat>();
            value.copyTo(*result_mat.get());
        }
        has_result = 𐡒𐡔𐡈;
    }

#𐡄𐡍 CV__EXCEPTION_PTR
    فراغ setException(std::exception_ptr e)
    {
        𐡄𐡍 (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        CV_Assert(!has_result);
        has_exception = 𐡒𐡔𐡈;
        exception = e;
        has_result = 𐡒𐡔𐡈;
    }
#endif

    فراغ setException(ثابت cv::Exception e)
    {
        𐡄𐡍 (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        CV_Assert(!has_result);
        has_exception = 𐡒𐡔𐡈;
        cv_exception = e;
        has_result = 𐡒𐡔𐡈;
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
    𐡄𐡍 (p)
        p->addrefFuture();
}

AsyncArray& AsyncArray::operator=(ثابت AsyncArray& o) CV_NOEXCEPT
{
    Impl* newp = o.p;
    𐡄𐡍 (newp)
        newp->addrefFuture();
    release();
    p = newp;
    أرجع *هذا;
}

فراغ AsyncArray::release() CV_NOEXCEPT
{
    Impl* impl = p;
    p = عدم;
    𐡄𐡍 (impl)
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
    𐡄𐡍 (!p) أرجع خطأ;
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
    𐡄𐡍 (p)
        p->addrefPromise();
}

AsyncPromise& AsyncPromise::operator=(ثابت AsyncPromise& o) CV_NOEXCEPT
{
    Impl* newp = o.p;
    𐡄𐡍 (newp)
        newp->addrefPromise();
    release();
    p = newp;
    أرجع *هذا;
}

فراغ AsyncPromise::release() CV_NOEXCEPT
{
    Impl* impl = p;
    p = عدم;
    𐡄𐡍 (impl)
        impl->releasePromise();
}

AsyncArray AsyncPromise::getArrayResult()
{
    CV_Assert(p);
    أرجع p->getArrayResult();
}

فراغ AsyncPromise::setValue(InputArray value)
{
    CV_Assert(p);
    أرجع p->setValue(value);
}

فراغ AsyncPromise::setException(ثابت cv::Exception& exception)
{
    CV_Assert(p);
    أرجع p->setException(exception);
}

#𐡄𐡍 CV__EXCEPTION_PTR
فراغ AsyncPromise::setException(std::exception_ptr exception)
{
    CV_Assert(p);
    أرجع p->setException(exception);
}
#endif

} // namespace
