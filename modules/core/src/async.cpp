// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ଅନ୍ତର୍ଭାଵ "precomp.hpp"
#ଅନ୍ତର୍ଭାଵ "opencv2/core/async.hpp"
#ଅନ୍ତର୍ଭାଵ "opencv2/core/detail/async_promise.hpp"

#ଅନ୍ତର୍ଭାଵ "opencv2/core/cvstd.hpp"

#ଅନ୍ତର୍ଭାଵ <opencv2/core/utils/logger.defines.hpp>
#undef CV_LOG_STRIP_LEVEL
#define CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_DEBUG + 1
#ଅନ୍ତର୍ଭାଵ <opencv2/core/utils/logger.hpp>

#ifndef OPENCV_DISABLE_THREAD_SUPPORT

#ଅନ୍ତର୍ଭାଵ <mutex>
#ଅନ୍ତର୍ଭାଵ <condition_variable>
#ଅନ୍ତର୍ଭାଵ <chrono>

ନାମାକାଶ cv {

/**
Manages shared state of asynchronous result
*/
ସଂରଚନା AsyncArray::Impl
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା refcount;
    ଶୂନ୍ୟ addrefFuture() CV_NOEXCEPT { CV_XADD(&refcount_future, 1); CV_XADD(&refcount, 1); } \
    ଶୂନ୍ୟ releaseFuture() CV_NOEXCEPT { CV_XADD(&refcount_future, -1); ଯଦି(1 == CV_XADD(&refcount, -1)) delete ଏହି; } \
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା refcount_future;
    ଶୂନ୍ୟ addrefPromise() CV_NOEXCEPT { CV_XADD(&refcount_promise, 1); CV_XADD(&refcount, 1); } \
    ଶୂନ୍ୟ releasePromise() CV_NOEXCEPT { CV_XADD(&refcount_promise, -1); ଯଦି(1 == CV_XADD(&refcount, -1)) delete ଏହି; } \
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା refcount_promise;

    mutable std::mutex mtx;
    mutable std::condition_variable cond_var;

    mutable ତାର୍କିକ has_result; // Mat, UMat or exception

    mutable cv::Ptr<Mat> result_mat;
    mutable cv::Ptr<UMat> result_umat;


    ତାର୍କିକ has_exception;
#ଯଦି CV__EXCEPTION_PTR
    std::exception_ptr exception;
#endif
    cv::Exception cv_exception;

    mutable ତାର୍କିକ result_is_fetched;

    ତାର୍କିକ future_is_returned;

    Impl()
        : refcount(1), refcount_future(0), refcount_promise(1)
        , has_result(ମିଥ୍ୟା)
        , has_exception(ମିଥ୍ୟା)
        , result_is_fetched(ମିଥ୍ୟା)
        , future_is_returned(ମିଥ୍ୟା)
    {
        // nothing
    }

    ~Impl()
    {
        ଯଦି (has_result && !result_is_fetched)
        {
            CV_LOG_INFO(ଖାଲି, "Asynchronous result has not been fetched");
        }
    }

    ତାର୍କିକ get(OutputArray dst, int64 timeoutNs) ଅଚଳ
    {
        CV_Assert(!result_is_fetched);
        ଯଦି (!has_result)
        {
            ଯଦି(refcount_promise == 0)
                CV_Error(Error::StsInternal, "Asynchronous result producer has been destroyed");
            ଯଦି (!wait_for(timeoutNs))
                ଫେରନ୍ତୁ ମିଥ୍ୟା;
        }
        std::unique_lock<std::mutex> lock(mtx);
        ଯଦି (has_result)
        {
            ଯଦି (!result_mat.empty())
            {
                dst.move(*result_mat.get());
                result_mat.release();
                result_is_fetched = ସତ୍ୟ;
                ଫେରନ୍ତୁ ସତ୍ୟ;
            }
            ଯଦି (!result_umat.empty())
            {
                dst.move(*result_umat.get());
                result_umat.release();
                result_is_fetched = ସତ୍ୟ;
                ଫେରନ୍ତୁ ସତ୍ୟ;
            }
#ଯଦି CV__EXCEPTION_PTR
            ଯଦି (has_exception && exception)
            {
                result_is_fetched = ସତ୍ୟ;
                std::rethrow_exception(exception);
            }
#endif
            ଯଦି (has_exception)
            {
                result_is_fetched = ସତ୍ୟ;
                ଫିଙ୍ଗ cv_exception;
            }
            CV_Error(Error::StsInternal, "AsyncArray: invalid state of 'has_result = true'");
        }
        CV_Assert(!has_result);
        CV_Assert(timeoutNs < 0);
        ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }

    ତାର୍କିକ valid() ଅଚଳ CV_NOEXCEPT
    {
        ଯଦି (result_is_fetched)
            ଫେରନ୍ତୁ ମିଥ୍ୟା;
        ଯଦି (refcount_promise == 0 && !has_result)
            ଫେରନ୍ତୁ ମିଥ୍ୟା;
        ଫେରନ୍ତୁ ସତ୍ୟ;
    }

    ତାର୍କିକ wait_for(int64 timeoutNs) ଅଚଳ
    {
        CV_Assert(valid());
        ଯଦି (has_result)
            ଫେରନ୍ତୁ has_result;
        ଯଦି (timeoutNs == 0)
            ଫେରନ୍ତୁ has_result;
        CV_LOG_INFO(ଖାଲି, "Waiting for async result ...");
        std::unique_lock<std::mutex> lock(mtx);
        ଅଚଳ auto cond_pred = [&]{ ଫେରନ୍ତୁ has_result == ସତ୍ୟ; };
        ଯଦି (timeoutNs > 0)
            ଫେରନ୍ତୁ cond_var.wait_for(lock, std::chrono::nanoseconds(timeoutNs), cond_pred);
        ଅନ୍ୟଥା
        {
            cond_var.wait(lock, cond_pred);
            CV_Assert(has_result);
            ଫେରନ୍ତୁ ସତ୍ୟ;
        }
    }

    AsyncArray getArrayResult()
    {
        CV_Assert(refcount_future == 0);
        AsyncArray result;
        addrefFuture();
        result.p = ଏହି;
        future_is_returned = ସତ୍ୟ;
        ଫେରନ୍ତୁ result;
    }

    ଶୂନ୍ୟ set_ମୂଲ୍ୟ(ନିବେଶ_Array value)
    {
        ଯଦି (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        std::unique_lock<std::mutex> lock(mtx);
        CV_Assert(!has_result);
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = value.kind();
        ଯଦି (k == ନିବେଶ_Array_2::UMAT)
        {
            result_umat = makePtr<UMat>();
            value.copyTo(*result_umat.get());
        }
        ଅନ୍ୟଥା
        {
            result_mat = makePtr<Mat>();
            value.copyTo(*result_mat.get());
        }
        has_result = ସତ୍ୟ;
        cond_var.notify_all();
    }

#ଯଦି CV__EXCEPTION_PTR
    ଶୂନ୍ୟ setException(std::exception_ptr e)
    {
        ଯଦି (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        std::unique_lock<std::mutex> lock(mtx);
        CV_Assert(!has_result);
        has_exception = ସତ୍ୟ;
        exception = e;
        has_result = ସତ୍ୟ;
        cond_var.notify_all();
    }
#endif

    ଶୂନ୍ୟ setException(ଅଚଳ cv::Exception e)
    {
        ଯଦି (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        std::unique_lock<std::mutex> lock(mtx);
        CV_Assert(!has_result);
        has_exception = ସତ୍ୟ;
        cv_exception = e;
        has_result = ସତ୍ୟ;
        cond_var.notify_all();
    }
};

}  // namespace

#ଅନ୍ୟଥା  // OPENCV_DISABLE_THREAD_SUPPORT

ନାମାକାଶ cv {

// no threading
ସଂରଚନା AsyncArray::Impl
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା refcount;
    ଶୂନ୍ୟ addrefFuture() CV_NOEXCEPT { refcount_future++; refcount++; }
    ଶୂନ୍ୟ releaseFuture() CV_NOEXCEPT { refcount_future--; ଯଦି (0 == --refcount) delete ଏହି; }
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା refcount_future;
    ଶୂନ୍ୟ addrefPromise() CV_NOEXCEPT { refcount_promise++; refcount++; } \
    ଶୂନ୍ୟ releasePromise() CV_NOEXCEPT { refcount_promise--; ଯଦି (0 == --refcount) delete ଏହି; }
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା refcount_promise;

    mutable ତାର୍କିକ has_result; // Mat, UMat or exception

    mutable cv::Ptr<Mat> result_mat;
    mutable cv::Ptr<UMat> result_umat;


    ତାର୍କିକ has_exception;
#ଯଦି CV__EXCEPTION_PTR
    std::exception_ptr exception;
#endif
    cv::Exception cv_exception;

    mutable ତାର୍କିକ result_is_fetched;

    ତାର୍କିକ future_is_returned;

    Impl()
        : refcount(1), refcount_future(0), refcount_promise(1)
        , has_result(ମିଥ୍ୟା)
        , has_exception(ମିଥ୍ୟା)
        , result_is_fetched(ମିଥ୍ୟା)
        , future_is_returned(ମିଥ୍ୟା)
    {
        // nothing
    }

    ~Impl()
    {
        ଯଦି (has_result && !result_is_fetched)
        {
            CV_LOG_INFO(ଖାଲି, "Asynchronous result has not been fetched");
        }
    }

    ତାର୍କିକ get(OutputArray dst, int64 timeoutNs) ଅଚଳ
    {
        CV_Assert(!result_is_fetched);
        ଯଦି (!has_result)
        {
            CV_UNUSED(timeoutNs);
            CV_Error(Error::StsError, "Result is not produced (unable to wait for result in OPENCV_DISABLE_THREAD_SUPPORT mode)");
        }
        ଯଦି (!result_mat.empty())
        {
            dst.move(*result_mat.get());
            result_mat.release();
            result_is_fetched = ସତ୍ୟ;
            ଫେରନ୍ତୁ ସତ୍ୟ;
        }
        ଯଦି (!result_umat.empty())
        {
            dst.move(*result_umat.get());
            result_umat.release();
            result_is_fetched = ସତ୍ୟ;
            ଫେରନ୍ତୁ ସତ୍ୟ;
        }
#ଯଦି CV__EXCEPTION_PTR
        ଯଦି (has_exception && exception)
        {
            result_is_fetched = ସତ୍ୟ;
            std::rethrow_exception(exception);
        }
#endif
        ଯଦି (has_exception)
        {
            result_is_fetched = ସତ୍ୟ;
            ଫିଙ୍ଗ cv_exception;
        }
        CV_Error(Error::StsInternal, "AsyncArray: invalid state of 'has_result = true'");
        ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }

    ତାର୍କିକ valid() ଅଚଳ CV_NOEXCEPT
    {
        ଯଦି (result_is_fetched)
            ଫେରନ୍ତୁ ମିଥ୍ୟା;
        ଯଦି (refcount_promise == 0 && !has_result)
            ଫେରନ୍ତୁ ମିଥ୍ୟା;
        ଫେରନ୍ତୁ ସତ୍ୟ;
    }

    ତାର୍କିକ wait_for(int64 timeoutNs) ଅଚଳ
    {
        CV_Assert(valid());
        ଯଦି (has_result)
            ଫେରନ୍ତୁ has_result;
        ଯଦି (timeoutNs == 0)
            ଫେରନ୍ତୁ has_result;
        CV_Error(Error::StsError, "Unable to wait in OPENCV_DISABLE_THREAD_SUPPORT mode");
    }

    AsyncArray getArrayResult()
    {
        CV_Assert(refcount_future == 0);
        AsyncArray result;
        addrefFuture();
        result.p = ଏହି;
        future_is_returned = ସତ୍ୟ;
        ଫେରନ୍ତୁ result;
    }

    ଶୂନ୍ୟ set_ମୂଲ୍ୟ(ନିବେଶ_Array value)
    {
        ଯଦି (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        CV_Assert(!has_result);
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = value.kind();
        ଯଦି (k == ନିବେଶ_Array_2::UMAT)
        {
            result_umat = makePtr<UMat>();
            value.copyTo(*result_umat.get());
        }
        ଅନ୍ୟଥା
        {
            result_mat = makePtr<Mat>();
            value.copyTo(*result_mat.get());
        }
        has_result = ସତ୍ୟ;
    }

#ଯଦି CV__EXCEPTION_PTR
    ଶୂନ୍ୟ setException(std::exception_ptr e)
    {
        ଯଦି (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        CV_Assert(!has_result);
        has_exception = ସତ୍ୟ;
        exception = e;
        has_result = ସତ୍ୟ;
    }
#endif

    ଶୂନ୍ୟ setException(ଅଚଳ cv::Exception e)
    {
        ଯଦି (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        CV_Assert(!has_result);
        has_exception = ସତ୍ୟ;
        cv_exception = e;
        has_result = ସତ୍ୟ;
    }
};

}

#endif  // OPENCV_DISABLE_THREAD_SUPPORT

ନାମାକାଶ cv {

AsyncArray::AsyncArray() CV_NOEXCEPT
    : p(ଖାଲି)
{
}

AsyncArray::~AsyncArray() CV_NOEXCEPT
{
    release();
}

AsyncArray::AsyncArray(ଅଚଳ AsyncArray& o) CV_NOEXCEPT
    : p(o.p)
{
    ଯଦି (p)
        p->addrefFuture();
}

AsyncArray& AsyncArray::operator=(ଅଚଳ AsyncArray& o) CV_NOEXCEPT
{
    Impl* newp = o.p;
    ଯଦି (newp)
        newp->addrefFuture();
    release();
    p = newp;
    ଫେରନ୍ତୁ *ଏହି;
}

ଶୂନ୍ୟ AsyncArray::release() CV_NOEXCEPT
{
    Impl* impl = p;
    p = ଖାଲି;
    ଯଦି (impl)
        impl->releaseFuture();
}

ତାର୍କିକ AsyncArray::get(OutputArray dst, int64 timeoutNs) ଅଚଳ
{
    CV_Assert(p);
    ଫେରନ୍ତୁ p->get(dst, timeoutNs);
}

ଶୂନ୍ୟ AsyncArray::get(OutputArray dst) ଅଚଳ
{
    CV_Assert(p);
    ତାର୍କିକ res = p->get(dst, -1);
    CV_Assert(res);
}

ତାର୍କିକ AsyncArray::wait_for(int64 timeoutNs) ଅଚଳ
{
    CV_Assert(p);
    ଫେରନ୍ତୁ p->wait_for(timeoutNs);
}

ତାର୍କିକ AsyncArray::valid() ଅଚଳ CV_NOEXCEPT
{
    ଯଦି (!p) ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ଫେରନ୍ତୁ p->valid();
}


//
// AsyncPromise
//

AsyncPromise::AsyncPromise() CV_NOEXCEPT
    : p(ନୂତନ AsyncArray::Impl())
{
}

AsyncPromise::~AsyncPromise() CV_NOEXCEPT
{
    release();
}

AsyncPromise::AsyncPromise(ଅଚଳ AsyncPromise& o) CV_NOEXCEPT
    : p(o.p)
{
    ଯଦି (p)
        p->addrefPromise();
}

AsyncPromise& AsyncPromise::operator=(ଅଚଳ AsyncPromise& o) CV_NOEXCEPT
{
    Impl* newp = o.p;
    ଯଦି (newp)
        newp->addrefPromise();
    release();
    p = newp;
    ଫେରନ୍ତୁ *ଏହି;
}

ଶୂନ୍ୟ AsyncPromise::release() CV_NOEXCEPT
{
    Impl* impl = p;
    p = ଖାଲି;
    ଯଦି (impl)
        impl->releasePromise();
}

AsyncArray AsyncPromise::getArrayResult()
{
    CV_Assert(p);
    ଫେରନ୍ତୁ p->getArrayResult();
}

ଶୂନ୍ୟ AsyncPromise::set_ମୂଲ୍ୟ(ନିବେଶ_Array value)
{
    CV_Assert(p);
    ଫେରନ୍ତୁ p->set_ମୂଲ୍ୟ(value);
}

ଶୂନ୍ୟ AsyncPromise::setException(ଅଚଳ cv::Exception& exception)
{
    CV_Assert(p);
    ଫେରନ୍ତୁ p->setException(exception);
}

#ଯଦି CV__EXCEPTION_PTR
ଶୂନ୍ୟ AsyncPromise::setException(std::exception_ptr exception)
{
    CV_Assert(p);
    ଫେରନ୍ତୁ p->setException(exception);
}
#endif

} // namespace
