// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#અન્તર્ભાવ "precomp.hpp"
#અન્તર્ભાવ "opencv2/core/async.hpp"
#અન્તર્ભાવ "opencv2/core/detail/async_promise.hpp"

#અન્તર્ભાવ "opencv2/core/cvstd.hpp"

#અન્તર્ભાવ <opencv2/core/utils/logger.defines.hpp>
#undef CV_LOG_STRIP_LEVEL
#define CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_DEBUG + 1
#અન્તર્ભાવ <opencv2/core/utils/logger.hpp>

#ifndef OPENCV_DISABLE_THREAD_SUPPORT

#અન્તર્ભાવ <mutex>
#અન્તર્ભાવ <condition_variable>
#અન્તર્ભાવ <chrono>

નામાકાશ cv {

/**
Manages shared state of asynchronous result
*/
સંરચના AsyncArray::Impl
{
    પૂર્ણાંક refcount;
    શૂન્ય addrefFuture() CV_NOEXCEPT { CV_XADD(&refcount_future, 1); CV_XADD(&refcount, 1); } \
    શૂન્ય releaseFuture() CV_NOEXCEPT { CV_XADD(&refcount_future, -1); જો(1 == CV_XADD(&refcount, -1)) delete આ; } \
    પૂર્ણાંક refcount_future;
    શૂન્ય addrefPromise() CV_NOEXCEPT { CV_XADD(&refcount_promise, 1); CV_XADD(&refcount, 1); } \
    શૂન્ય releasePromise() CV_NOEXCEPT { CV_XADD(&refcount_promise, -1); જો(1 == CV_XADD(&refcount, -1)) delete આ; } \
    પૂર્ણાંક refcount_promise;

    mutable std::mutex mtx;
    mutable std::condition_variable cond_var;

    mutable તાર્કિક has_result; // Mat, UMat or exception

    mutable cv::Ptr<Mat> result_mat;
    mutable cv::Ptr<UMat> result_umat;


    તાર્કિક has_exception;
#જો CV__EXCEPTION_PTR
    std::exception_ptr exception;
#endif
    cv::Exception cv_exception;

    mutable તાર્કિક result_is_fetched;

    તાર્કિક future_is_returned;

    Impl()
        : refcount(1), refcount_future(0), refcount_promise(1)
        , has_result(ખોટું)
        , has_exception(ખોટું)
        , result_is_fetched(ખોટું)
        , future_is_returned(ખોટું)
    {
        // nothing
    }

    ~Impl()
    {
        જો (has_result && !result_is_fetched)
        {
            CV_LOG_INFO(ખાલી, "Asynchronous result has not been fetched");
        }
    }

    તાર્કિક get(OutputArray dst, int64 timeoutNs) અચલ
    {
        CV_Assert(!result_is_fetched);
        જો (!has_result)
        {
            જો(refcount_promise == 0)
                CV_Error(Error::StsInternal, "Asynchronous result producer has been destroyed");
            જો (!wait_for(timeoutNs))
                પાછા ખોટું;
        }
        std::unique_lock<std::mutex> lock(mtx);
        જો (has_result)
        {
            જો (!result_mat.empty())
            {
                dst.move(*result_mat.get());
                result_mat.release();
                result_is_fetched = સાચું;
                પાછા સાચું;
            }
            જો (!result_umat.empty())
            {
                dst.move(*result_umat.get());
                result_umat.release();
                result_is_fetched = સાચું;
                પાછા સાચું;
            }
#જો CV__EXCEPTION_PTR
            જો (has_exception && exception)
            {
                result_is_fetched = સાચું;
                std::rethrow_exception(exception);
            }
#endif
            જો (has_exception)
            {
                result_is_fetched = સાચું;
                ફેંકો cv_exception;
            }
            CV_Error(Error::StsInternal, "AsyncArray: invalid state of 'has_result = true'");
        }
        CV_Assert(!has_result);
        CV_Assert(timeoutNs < 0);
        પાછા ખોટું;
    }

    તાર્કિક valid() અચલ CV_NOEXCEPT
    {
        જો (result_is_fetched)
            પાછા ખોટું;
        જો (refcount_promise == 0 && !has_result)
            પાછા ખોટું;
        પાછા સાચું;
    }

    તાર્કિક wait_for(int64 timeoutNs) અચલ
    {
        CV_Assert(valid());
        જો (has_result)
            પાછા has_result;
        જો (timeoutNs == 0)
            પાછા has_result;
        CV_LOG_INFO(ખાલી, "Waiting for async result ...");
        std::unique_lock<std::mutex> lock(mtx);
        અચલ auto cond_pred = [&]{ પાછા has_result == સાચું; };
        જો (timeoutNs > 0)
            પાછા cond_var.wait_for(lock, std::chrono::nanoseconds(timeoutNs), cond_pred);
        નહીંતર
        {
            cond_var.wait(lock, cond_pred);
            CV_Assert(has_result);
            પાછા સાચું;
        }
    }

    AsyncArray getArrayResult()
    {
        CV_Assert(refcount_future == 0);
        AsyncArray result;
        addrefFuture();
        result.p = આ;
        future_is_returned = સાચું;
        પાછા result;
    }

    શૂન્ય set_કિંમત(આગત_Array value)
    {
        જો (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        std::unique_lock<std::mutex> lock(mtx);
        CV_Assert(!has_result);
        પૂર્ણાંક k = value.kind();
        જો (k == આગત_Array_2::UMAT)
        {
            result_umat = makePtr<UMat>();
            value.copyTo(*result_umat.get());
        }
        નહીંતર
        {
            result_mat = makePtr<Mat>();
            value.copyTo(*result_mat.get());
        }
        has_result = સાચું;
        cond_var.notify_all();
    }

#જો CV__EXCEPTION_PTR
    શૂન્ય setException(std::exception_ptr e)
    {
        જો (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        std::unique_lock<std::mutex> lock(mtx);
        CV_Assert(!has_result);
        has_exception = સાચું;
        exception = e;
        has_result = સાચું;
        cond_var.notify_all();
    }
#endif

    શૂન્ય setException(અચલ cv::Exception e)
    {
        જો (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        std::unique_lock<std::mutex> lock(mtx);
        CV_Assert(!has_result);
        has_exception = સાચું;
        cv_exception = e;
        has_result = સાચું;
        cond_var.notify_all();
    }
};

}  // namespace

#નહીંતર  // OPENCV_DISABLE_THREAD_SUPPORT

નામાકાશ cv {

// no threading
સંરચના AsyncArray::Impl
{
    પૂર્ણાંક refcount;
    શૂન્ય addrefFuture() CV_NOEXCEPT { refcount_future++; refcount++; }
    શૂન્ય releaseFuture() CV_NOEXCEPT { refcount_future--; જો (0 == --refcount) delete આ; }
    પૂર્ણાંક refcount_future;
    શૂન્ય addrefPromise() CV_NOEXCEPT { refcount_promise++; refcount++; } \
    શૂન્ય releasePromise() CV_NOEXCEPT { refcount_promise--; જો (0 == --refcount) delete આ; }
    પૂર્ણાંક refcount_promise;

    mutable તાર્કિક has_result; // Mat, UMat or exception

    mutable cv::Ptr<Mat> result_mat;
    mutable cv::Ptr<UMat> result_umat;


    તાર્કિક has_exception;
#જો CV__EXCEPTION_PTR
    std::exception_ptr exception;
#endif
    cv::Exception cv_exception;

    mutable તાર્કિક result_is_fetched;

    તાર્કિક future_is_returned;

    Impl()
        : refcount(1), refcount_future(0), refcount_promise(1)
        , has_result(ખોટું)
        , has_exception(ખોટું)
        , result_is_fetched(ખોટું)
        , future_is_returned(ખોટું)
    {
        // nothing
    }

    ~Impl()
    {
        જો (has_result && !result_is_fetched)
        {
            CV_LOG_INFO(ખાલી, "Asynchronous result has not been fetched");
        }
    }

    તાર્કિક get(OutputArray dst, int64 timeoutNs) અચલ
    {
        CV_Assert(!result_is_fetched);
        જો (!has_result)
        {
            CV_UNUSED(timeoutNs);
            CV_Error(Error::StsError, "Result is not produced (unable to wait for result in OPENCV_DISABLE_THREAD_SUPPORT mode)");
        }
        જો (!result_mat.empty())
        {
            dst.move(*result_mat.get());
            result_mat.release();
            result_is_fetched = સાચું;
            પાછા સાચું;
        }
        જો (!result_umat.empty())
        {
            dst.move(*result_umat.get());
            result_umat.release();
            result_is_fetched = સાચું;
            પાછા સાચું;
        }
#જો CV__EXCEPTION_PTR
        જો (has_exception && exception)
        {
            result_is_fetched = સાચું;
            std::rethrow_exception(exception);
        }
#endif
        જો (has_exception)
        {
            result_is_fetched = સાચું;
            ફેંકો cv_exception;
        }
        CV_Error(Error::StsInternal, "AsyncArray: invalid state of 'has_result = true'");
        પાછા ખોટું;
    }

    તાર્કિક valid() અચલ CV_NOEXCEPT
    {
        જો (result_is_fetched)
            પાછા ખોટું;
        જો (refcount_promise == 0 && !has_result)
            પાછા ખોટું;
        પાછા સાચું;
    }

    તાર્કિક wait_for(int64 timeoutNs) અચલ
    {
        CV_Assert(valid());
        જો (has_result)
            પાછા has_result;
        જો (timeoutNs == 0)
            પાછા has_result;
        CV_Error(Error::StsError, "Unable to wait in OPENCV_DISABLE_THREAD_SUPPORT mode");
    }

    AsyncArray getArrayResult()
    {
        CV_Assert(refcount_future == 0);
        AsyncArray result;
        addrefFuture();
        result.p = આ;
        future_is_returned = સાચું;
        પાછા result;
    }

    શૂન્ય set_કિંમત(આગત_Array value)
    {
        જો (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        CV_Assert(!has_result);
        પૂર્ણાંક k = value.kind();
        જો (k == આગત_Array_2::UMAT)
        {
            result_umat = makePtr<UMat>();
            value.copyTo(*result_umat.get());
        }
        નહીંતર
        {
            result_mat = makePtr<Mat>();
            value.copyTo(*result_mat.get());
        }
        has_result = સાચું;
    }

#જો CV__EXCEPTION_PTR
    શૂન્ય setException(std::exception_ptr e)
    {
        જો (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        CV_Assert(!has_result);
        has_exception = સાચું;
        exception = e;
        has_result = સાચું;
    }
#endif

    શૂન્ય setException(અચલ cv::Exception e)
    {
        જો (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        CV_Assert(!has_result);
        has_exception = સાચું;
        cv_exception = e;
        has_result = સાચું;
    }
};

}

#endif  // OPENCV_DISABLE_THREAD_SUPPORT

નામાકાશ cv {

AsyncArray::AsyncArray() CV_NOEXCEPT
    : p(ખાલી)
{
}

AsyncArray::~AsyncArray() CV_NOEXCEPT
{
    release();
}

AsyncArray::AsyncArray(અચલ AsyncArray& o) CV_NOEXCEPT
    : p(o.p)
{
    જો (p)
        p->addrefFuture();
}

AsyncArray& AsyncArray::operator=(અચલ AsyncArray& o) CV_NOEXCEPT
{
    Impl* newp = o.p;
    જો (newp)
        newp->addrefFuture();
    release();
    p = newp;
    પાછા *આ;
}

શૂન્ય AsyncArray::release() CV_NOEXCEPT
{
    Impl* impl = p;
    p = ખાલી;
    જો (impl)
        impl->releaseFuture();
}

તાર્કિક AsyncArray::get(OutputArray dst, int64 timeoutNs) અચલ
{
    CV_Assert(p);
    પાછા p->get(dst, timeoutNs);
}

શૂન્ય AsyncArray::get(OutputArray dst) અચલ
{
    CV_Assert(p);
    તાર્કિક res = p->get(dst, -1);
    CV_Assert(res);
}

તાર્કિક AsyncArray::wait_for(int64 timeoutNs) અચલ
{
    CV_Assert(p);
    પાછા p->wait_for(timeoutNs);
}

તાર્કિક AsyncArray::valid() અચલ CV_NOEXCEPT
{
    જો (!p) પાછા ખોટું;
    પાછા p->valid();
}


//
// AsyncPromise
//

AsyncPromise::AsyncPromise() CV_NOEXCEPT
    : p(નવું AsyncArray::Impl())
{
}

AsyncPromise::~AsyncPromise() CV_NOEXCEPT
{
    release();
}

AsyncPromise::AsyncPromise(અચલ AsyncPromise& o) CV_NOEXCEPT
    : p(o.p)
{
    જો (p)
        p->addrefPromise();
}

AsyncPromise& AsyncPromise::operator=(અચલ AsyncPromise& o) CV_NOEXCEPT
{
    Impl* newp = o.p;
    જો (newp)
        newp->addrefPromise();
    release();
    p = newp;
    પાછા *આ;
}

શૂન્ય AsyncPromise::release() CV_NOEXCEPT
{
    Impl* impl = p;
    p = ખાલી;
    જો (impl)
        impl->releasePromise();
}

AsyncArray AsyncPromise::getArrayResult()
{
    CV_Assert(p);
    પાછા p->getArrayResult();
}

શૂન્ય AsyncPromise::set_કિંમત(આગત_Array value)
{
    CV_Assert(p);
    પાછા p->set_કિંમત(value);
}

શૂન્ય AsyncPromise::setException(અચલ cv::Exception& exception)
{
    CV_Assert(p);
    પાછા p->setException(exception);
}

#જો CV__EXCEPTION_PTR
શૂન્ય AsyncPromise::setException(std::exception_ptr exception)
{
    CV_Assert(p);
    પાછા p->setException(exception);
}
#endif

} // namespace
