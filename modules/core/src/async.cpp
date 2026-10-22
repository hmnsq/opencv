// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#അന്തര്ഭാവ "precomp.hpp"
#അന്തര്ഭാവ "opencv2/core/async.hpp"
#അന്തര്ഭാവ "opencv2/core/detail/async_promise.hpp"

#അന്തര്ഭാവ "opencv2/core/cvstd.hpp"

#അന്തര്ഭാവ <opencv2/core/utils/logger.defines.hpp>
#undef CV_LOG_STRIP_LEVEL
#define CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_DEBUG + 1
#അന്തര്ഭാവ <opencv2/core/utils/logger.hpp>

#ifndef OPENCV_DISABLE_THREAD_SUPPORT

#അന്തര്ഭാവ <mutex>
#അന്തര്ഭാവ <condition_variable>
#അന്തര്ഭാവ <chrono>

നാമാകാശ cv {

/**
Manages shared state of asynchronous result
*/
ഘടന AsyncArray::Impl
{
    പൂർണ്ണസംഖ്യ refcount;
    ശൂന്യം addrefFuture() CV_NOEXCEPT { CV_XADD(&refcount_future, 1); CV_XADD(&refcount, 1); } \
    ശൂന്യം releaseFuture() CV_NOEXCEPT { CV_XADD(&refcount_future, -1); എങ്കിൽ(1 == CV_XADD(&refcount, -1)) delete ഇത്; } \
    പൂർണ്ണസംഖ്യ refcount_future;
    ശൂന്യം addrefPromise() CV_NOEXCEPT { CV_XADD(&refcount_promise, 1); CV_XADD(&refcount, 1); } \
    ശൂന്യം releasePromise() CV_NOEXCEPT { CV_XADD(&refcount_promise, -1); എങ്കിൽ(1 == CV_XADD(&refcount, -1)) delete ഇത്; } \
    പൂർണ്ണസംഖ്യ refcount_promise;

    mutable std::mutex mtx;
    mutable std::condition_variable cond_var;

    mutable തർക്കം has_result; // Mat, UMat or exception

    mutable cv::Ptr<Mat> result_mat;
    mutable cv::Ptr<UMat> result_umat;


    തർക്കം has_exception;
#എങ്കിൽ CV__EXCEPTION_PTR
    std::exception_ptr exception;
#endif
    cv::Exception cv_exception;

    mutable തർക്കം result_is_fetched;

    തർക്കം future_is_returned;

    Impl()
        : refcount(1), refcount_future(0), refcount_promise(1)
        , has_result(അസത്യം)
        , has_exception(അസത്യം)
        , result_is_fetched(അസത്യം)
        , future_is_returned(അസത്യം)
    {
        // nothing
    }

    ~Impl()
    {
        എങ്കിൽ (has_result && !result_is_fetched)
        {
            CV_LOG_INFO(ശൂന്യ, "Asynchronous result has not been fetched");
        }
    }

    തർക്കം get(OutputArray dst, int64 timeoutNs) സ്ഥിരം
    {
        CV_Assert(!result_is_fetched);
        എങ്കിൽ (!has_result)
        {
            എങ്കിൽ(refcount_promise == 0)
                CV_Error(Error::StsInternal, "Asynchronous result producer has been destroyed");
            എങ്കിൽ (!wait_for(timeoutNs))
                തിരികെ അസത്യം;
        }
        std::unique_lock<std::mutex> lock(mtx);
        എങ്കിൽ (has_result)
        {
            എങ്കിൽ (!result_mat.empty())
            {
                dst.move(*result_mat.get());
                result_mat.release();
                result_is_fetched = സത്യം;
                തിരികെ സത്യം;
            }
            എങ്കിൽ (!result_umat.empty())
            {
                dst.move(*result_umat.get());
                result_umat.release();
                result_is_fetched = സത്യം;
                തിരികെ സത്യം;
            }
#എങ്കിൽ CV__EXCEPTION_PTR
            എങ്കിൽ (has_exception && exception)
            {
                result_is_fetched = സത്യം;
                std::rethrow_exception(exception);
            }
#endif
            എങ്കിൽ (has_exception)
            {
                result_is_fetched = സത്യം;
                എറിയുക cv_exception;
            }
            CV_Error(Error::StsInternal, "AsyncArray: invalid state of 'has_result = true'");
        }
        CV_Assert(!has_result);
        CV_Assert(timeoutNs < 0);
        തിരികെ അസത്യം;
    }

    തർക്കം valid() സ്ഥിരം CV_NOEXCEPT
    {
        എങ്കിൽ (result_is_fetched)
            തിരികെ അസത്യം;
        എങ്കിൽ (refcount_promise == 0 && !has_result)
            തിരികെ അസത്യം;
        തിരികെ സത്യം;
    }

    തർക്കം wait_for(int64 timeoutNs) സ്ഥിരം
    {
        CV_Assert(valid());
        എങ്കിൽ (has_result)
            തിരികെ has_result;
        എങ്കിൽ (timeoutNs == 0)
            തിരികെ has_result;
        CV_LOG_INFO(ശൂന്യ, "Waiting for async result ...");
        std::unique_lock<std::mutex> lock(mtx);
        സ്ഥിരം auto cond_pred = [&]{ തിരികെ has_result == സത്യം; };
        എങ്കിൽ (timeoutNs > 0)
            തിരികെ cond_var.wait_for(lock, std::chrono::nanoseconds(timeoutNs), cond_pred);
        അല്ലെങ്കിൽ
        {
            cond_var.wait(lock, cond_pred);
            CV_Assert(has_result);
            തിരികെ സത്യം;
        }
    }

    AsyncArray getArrayResult()
    {
        CV_Assert(refcount_future == 0);
        AsyncArray result;
        addrefFuture();
        result.p = ഇത്;
        future_is_returned = സത്യം;
        തിരികെ result;
    }

    ശൂന്യം set_മൂല്യം(നിവേശം_Array value)
    {
        എങ്കിൽ (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        std::unique_lock<std::mutex> lock(mtx);
        CV_Assert(!has_result);
        പൂർണ്ണസംഖ്യ k = value.kind();
        എങ്കിൽ (k == നിവേശം_Array_2::UMAT)
        {
            result_umat = makePtr<UMat>();
            value.copyTo(*result_umat.get());
        }
        അല്ലെങ്കിൽ
        {
            result_mat = makePtr<Mat>();
            value.copyTo(*result_mat.get());
        }
        has_result = സത്യം;
        cond_var.notify_all();
    }

#എങ്കിൽ CV__EXCEPTION_PTR
    ശൂന്യം setException(std::exception_ptr e)
    {
        എങ്കിൽ (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        std::unique_lock<std::mutex> lock(mtx);
        CV_Assert(!has_result);
        has_exception = സത്യം;
        exception = e;
        has_result = സത്യം;
        cond_var.notify_all();
    }
#endif

    ശൂന്യം setException(സ്ഥിരം cv::Exception e)
    {
        എങ്കിൽ (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        std::unique_lock<std::mutex> lock(mtx);
        CV_Assert(!has_result);
        has_exception = സത്യം;
        cv_exception = e;
        has_result = സത്യം;
        cond_var.notify_all();
    }
};

}  // namespace

#അല്ലെങ്കിൽ  // OPENCV_DISABLE_THREAD_SUPPORT

നാമാകാശ cv {

// no threading
ഘടന AsyncArray::Impl
{
    പൂർണ്ണസംഖ്യ refcount;
    ശൂന്യം addrefFuture() CV_NOEXCEPT { refcount_future++; refcount++; }
    ശൂന്യം releaseFuture() CV_NOEXCEPT { refcount_future--; എങ്കിൽ (0 == --refcount) delete ഇത്; }
    പൂർണ്ണസംഖ്യ refcount_future;
    ശൂന്യം addrefPromise() CV_NOEXCEPT { refcount_promise++; refcount++; } \
    ശൂന്യം releasePromise() CV_NOEXCEPT { refcount_promise--; എങ്കിൽ (0 == --refcount) delete ഇത്; }
    പൂർണ്ണസംഖ്യ refcount_promise;

    mutable തർക്കം has_result; // Mat, UMat or exception

    mutable cv::Ptr<Mat> result_mat;
    mutable cv::Ptr<UMat> result_umat;


    തർക്കം has_exception;
#എങ്കിൽ CV__EXCEPTION_PTR
    std::exception_ptr exception;
#endif
    cv::Exception cv_exception;

    mutable തർക്കം result_is_fetched;

    തർക്കം future_is_returned;

    Impl()
        : refcount(1), refcount_future(0), refcount_promise(1)
        , has_result(അസത്യം)
        , has_exception(അസത്യം)
        , result_is_fetched(അസത്യം)
        , future_is_returned(അസത്യം)
    {
        // nothing
    }

    ~Impl()
    {
        എങ്കിൽ (has_result && !result_is_fetched)
        {
            CV_LOG_INFO(ശൂന്യ, "Asynchronous result has not been fetched");
        }
    }

    തർക്കം get(OutputArray dst, int64 timeoutNs) സ്ഥിരം
    {
        CV_Assert(!result_is_fetched);
        എങ്കിൽ (!has_result)
        {
            CV_UNUSED(timeoutNs);
            CV_Error(Error::StsError, "Result is not produced (unable to wait for result in OPENCV_DISABLE_THREAD_SUPPORT mode)");
        }
        എങ്കിൽ (!result_mat.empty())
        {
            dst.move(*result_mat.get());
            result_mat.release();
            result_is_fetched = സത്യം;
            തിരികെ സത്യം;
        }
        എങ്കിൽ (!result_umat.empty())
        {
            dst.move(*result_umat.get());
            result_umat.release();
            result_is_fetched = സത്യം;
            തിരികെ സത്യം;
        }
#എങ്കിൽ CV__EXCEPTION_PTR
        എങ്കിൽ (has_exception && exception)
        {
            result_is_fetched = സത്യം;
            std::rethrow_exception(exception);
        }
#endif
        എങ്കിൽ (has_exception)
        {
            result_is_fetched = സത്യം;
            എറിയുക cv_exception;
        }
        CV_Error(Error::StsInternal, "AsyncArray: invalid state of 'has_result = true'");
        തിരികെ അസത്യം;
    }

    തർക്കം valid() സ്ഥിരം CV_NOEXCEPT
    {
        എങ്കിൽ (result_is_fetched)
            തിരികെ അസത്യം;
        എങ്കിൽ (refcount_promise == 0 && !has_result)
            തിരികെ അസത്യം;
        തിരികെ സത്യം;
    }

    തർക്കം wait_for(int64 timeoutNs) സ്ഥിരം
    {
        CV_Assert(valid());
        എങ്കിൽ (has_result)
            തിരികെ has_result;
        എങ്കിൽ (timeoutNs == 0)
            തിരികെ has_result;
        CV_Error(Error::StsError, "Unable to wait in OPENCV_DISABLE_THREAD_SUPPORT mode");
    }

    AsyncArray getArrayResult()
    {
        CV_Assert(refcount_future == 0);
        AsyncArray result;
        addrefFuture();
        result.p = ഇത്;
        future_is_returned = സത്യം;
        തിരികെ result;
    }

    ശൂന്യം set_മൂല്യം(നിവേശം_Array value)
    {
        എങ്കിൽ (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        CV_Assert(!has_result);
        പൂർണ്ണസംഖ്യ k = value.kind();
        എങ്കിൽ (k == നിവേശം_Array_2::UMAT)
        {
            result_umat = makePtr<UMat>();
            value.copyTo(*result_umat.get());
        }
        അല്ലെങ്കിൽ
        {
            result_mat = makePtr<Mat>();
            value.copyTo(*result_mat.get());
        }
        has_result = സത്യം;
    }

#എങ്കിൽ CV__EXCEPTION_PTR
    ശൂന്യം setException(std::exception_ptr e)
    {
        എങ്കിൽ (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        CV_Assert(!has_result);
        has_exception = സത്യം;
        exception = e;
        has_result = സത്യം;
    }
#endif

    ശൂന്യം setException(സ്ഥിരം cv::Exception e)
    {
        എങ്കിൽ (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        CV_Assert(!has_result);
        has_exception = സത്യം;
        cv_exception = e;
        has_result = സത്യം;
    }
};

}

#endif  // OPENCV_DISABLE_THREAD_SUPPORT

നാമാകാശ cv {

AsyncArray::AsyncArray() CV_NOEXCEPT
    : p(ശൂന്യ)
{
}

AsyncArray::~AsyncArray() CV_NOEXCEPT
{
    release();
}

AsyncArray::AsyncArray(സ്ഥിരം AsyncArray& o) CV_NOEXCEPT
    : p(o.p)
{
    എങ്കിൽ (p)
        p->addrefFuture();
}

AsyncArray& AsyncArray::operator=(സ്ഥിരം AsyncArray& o) CV_NOEXCEPT
{
    Impl* newp = o.p;
    എങ്കിൽ (newp)
        newp->addrefFuture();
    release();
    p = newp;
    തിരികെ *ഇത്;
}

ശൂന്യം AsyncArray::release() CV_NOEXCEPT
{
    Impl* impl = p;
    p = ശൂന്യ;
    എങ്കിൽ (impl)
        impl->releaseFuture();
}

തർക്കം AsyncArray::get(OutputArray dst, int64 timeoutNs) സ്ഥിരം
{
    CV_Assert(p);
    തിരികെ p->get(dst, timeoutNs);
}

ശൂന്യം AsyncArray::get(OutputArray dst) സ്ഥിരം
{
    CV_Assert(p);
    തർക്കം res = p->get(dst, -1);
    CV_Assert(res);
}

തർക്കം AsyncArray::wait_for(int64 timeoutNs) സ്ഥിരം
{
    CV_Assert(p);
    തിരികെ p->wait_for(timeoutNs);
}

തർക്കം AsyncArray::valid() സ്ഥിരം CV_NOEXCEPT
{
    എങ്കിൽ (!p) തിരികെ അസത്യം;
    തിരികെ p->valid();
}


//
// AsyncPromise
//

AsyncPromise::AsyncPromise() CV_NOEXCEPT
    : p(പുതിയ AsyncArray::Impl())
{
}

AsyncPromise::~AsyncPromise() CV_NOEXCEPT
{
    release();
}

AsyncPromise::AsyncPromise(സ്ഥിരം AsyncPromise& o) CV_NOEXCEPT
    : p(o.p)
{
    എങ്കിൽ (p)
        p->addrefPromise();
}

AsyncPromise& AsyncPromise::operator=(സ്ഥിരം AsyncPromise& o) CV_NOEXCEPT
{
    Impl* newp = o.p;
    എങ്കിൽ (newp)
        newp->addrefPromise();
    release();
    p = newp;
    തിരികെ *ഇത്;
}

ശൂന്യം AsyncPromise::release() CV_NOEXCEPT
{
    Impl* impl = p;
    p = ശൂന്യ;
    എങ്കിൽ (impl)
        impl->releasePromise();
}

AsyncArray AsyncPromise::getArrayResult()
{
    CV_Assert(p);
    തിരികെ p->getArrayResult();
}

ശൂന്യം AsyncPromise::set_മൂല്യം(നിവേശം_Array value)
{
    CV_Assert(p);
    തിരികെ p->set_മൂല്യം(value);
}

ശൂന്യം AsyncPromise::setException(സ്ഥിരം cv::Exception& exception)
{
    CV_Assert(p);
    തിരികെ p->setException(exception);
}

#എങ്കിൽ CV__EXCEPTION_PTR
ശൂന്യം AsyncPromise::setException(std::exception_ptr exception)
{
    CV_Assert(p);
    തിരികെ p->setException(exception);
}
#endif

} // namespace
