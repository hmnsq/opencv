// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#अन्तर्भाव "precomp.hpp"
#अन्तर्भाव "opencv2/core/async.hpp"
#अन्तर्भाव "opencv2/core/detail/async_promise.hpp"

#अन्तर्भाव "opencv2/core/cvstd.hpp"

#अन्तर्भाव <opencv2/core/utils/logger.defines.hpp>
#undef CV_LOG_STRIP_LEVEL
#define CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_DEBUG + 1
#अन्तर्भाव <opencv2/core/utils/logger.hpp>

#ifndef OPENCV_DISABLE_THREAD_SUPPORT

#अन्तर्भाव <mutex>
#अन्तर्भाव <condition_variable>
#अन्तर्भाव <chrono>

नामाकाश cv {

/**
Manages shared state of asynchronous result
*/
संरचना AsyncArray::Impl
{
    पूर्णाङ्क refcount;
    शून्य addrefFuture() CV_NOEXCEPT { CV_XADD(&refcount_future, 1); CV_XADD(&refcount, 1); } \
    शून्य releaseFuture() CV_NOEXCEPT { CV_XADD(&refcount_future, -1); सचे(1 == CV_XADD(&refcount, -1)) delete एतत्; } \
    पूर्णाङ्क refcount_future;
    शून्य addrefPromise() CV_NOEXCEPT { CV_XADD(&refcount_promise, 1); CV_XADD(&refcount, 1); } \
    शून्य releasePromise() CV_NOEXCEPT { CV_XADD(&refcount_promise, -1); सचे(1 == CV_XADD(&refcount, -1)) delete एतत्; } \
    पूर्णाङ्क refcount_promise;

    mutable std::mutex mtx;
    mutable std::condition_variable cond_var;

    mutable तार्किक has_result; // Mat, UMat or exception

    mutable cv::Ptr<Mat> result_mat;
    mutable cv::Ptr<UMat> result_umat;


    तार्किक has_exception;
#सचे CV__EXCEPTION_PTR
    std::exception_ptr exception;
#endif
    cv::Exception cv_exception;

    mutable तार्किक result_is_fetched;

    तार्किक future_is_returned;

    Impl()
        : refcount(1), refcount_future(0), refcount_promise(1)
        , has_result(मिच्छा)
        , has_exception(मिच्छा)
        , result_is_fetched(मिच्छा)
        , future_is_returned(मिच्छा)
    {
        // nothing
    }

    ~Impl()
    {
        सचे (has_result && !result_is_fetched)
        {
            CV_LOG_INFO(रिक्त, "Asynchronous result has not been fetched");
        }
    }

    तार्किक get(OutputArray dst, int64 timeoutNs) ध्रुव
    {
        CV_Assert(!result_is_fetched);
        सचे (!has_result)
        {
            सचे(refcount_promise == 0)
                CV_Error(Error::StsInternal, "Asynchronous result producer has been destroyed");
            सचे (!wait_for(timeoutNs))
                पटिनिवत्त मिच्छा;
        }
        std::unique_lock<std::mutex> lock(mtx);
        सचे (has_result)
        {
            सचे (!result_mat.empty())
            {
                dst.move(*result_mat.get());
                result_mat.release();
                result_is_fetched = सच्च;
                पटिनिवत्त सच्च;
            }
            सचे (!result_umat.empty())
            {
                dst.move(*result_umat.get());
                result_umat.release();
                result_is_fetched = सच्च;
                पटिनिवत्त सच्च;
            }
#सचे CV__EXCEPTION_PTR
            सचे (has_exception && exception)
            {
                result_is_fetched = सच्च;
                std::rethrow_exception(exception);
            }
#endif
            सचे (has_exception)
            {
                result_is_fetched = सच्च;
                क्षिप cv_exception;
            }
            CV_Error(Error::StsInternal, "AsyncArray: invalid state of 'has_result = true'");
        }
        CV_Assert(!has_result);
        CV_Assert(timeoutNs < 0);
        पटिनिवत्त मिच्छा;
    }

    तार्किक valid() ध्रुव CV_NOEXCEPT
    {
        सचे (result_is_fetched)
            पटिनिवत्त मिच्छा;
        सचे (refcount_promise == 0 && !has_result)
            पटिनिवत्त मिच्छा;
        पटिनिवत्त सच्च;
    }

    तार्किक wait_for(int64 timeoutNs) ध्रुव
    {
        CV_Assert(valid());
        सचे (has_result)
            पटिनिवत्त has_result;
        सचे (timeoutNs == 0)
            पटिनिवत्त has_result;
        CV_LOG_INFO(रिक्त, "Waiting for async result ...");
        std::unique_lock<std::mutex> lock(mtx);
        ध्रुव auto cond_pred = [&]{ पटिनिवत्त has_result == सच्च; };
        सचे (timeoutNs > 0)
            पटिनिवत्त cond_var.wait_for(lock, std::chrono::nanoseconds(timeoutNs), cond_pred);
        अञ्ञथा
        {
            cond_var.wait(lock, cond_pred);
            CV_Assert(has_result);
            पटिनिवत्त सच्च;
        }
    }

    AsyncArray getArrayResult()
    {
        CV_Assert(refcount_future == 0);
        AsyncArray result;
        addrefFuture();
        result.p = एतत्;
        future_is_returned = सच्च;
        पटिनिवत्त result;
    }

    शून्य set_मानम्(निवेशः_Array value)
    {
        सचे (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        std::unique_lock<std::mutex> lock(mtx);
        CV_Assert(!has_result);
        पूर्णाङ्क k = value.kind();
        सचे (k == निवेशः_Array_2::UMAT)
        {
            result_umat = makePtr<UMat>();
            value.copyTo(*result_umat.get());
        }
        अञ्ञथा
        {
            result_mat = makePtr<Mat>();
            value.copyTo(*result_mat.get());
        }
        has_result = सच्च;
        cond_var.notify_all();
    }

#सचे CV__EXCEPTION_PTR
    शून्य setException(std::exception_ptr e)
    {
        सचे (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        std::unique_lock<std::mutex> lock(mtx);
        CV_Assert(!has_result);
        has_exception = सच्च;
        exception = e;
        has_result = सच्च;
        cond_var.notify_all();
    }
#endif

    शून्य setException(ध्रुव cv::Exception e)
    {
        सचे (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        std::unique_lock<std::mutex> lock(mtx);
        CV_Assert(!has_result);
        has_exception = सच्च;
        cv_exception = e;
        has_result = सच्च;
        cond_var.notify_all();
    }
};

}  // namespace

#अञ्ञथा  // OPENCV_DISABLE_THREAD_SUPPORT

नामाकाश cv {

// no threading
संरचना AsyncArray::Impl
{
    पूर्णाङ्क refcount;
    शून्य addrefFuture() CV_NOEXCEPT { refcount_future++; refcount++; }
    शून्य releaseFuture() CV_NOEXCEPT { refcount_future--; सचे (0 == --refcount) delete एतत्; }
    पूर्णाङ्क refcount_future;
    शून्य addrefPromise() CV_NOEXCEPT { refcount_promise++; refcount++; } \
    शून्य releasePromise() CV_NOEXCEPT { refcount_promise--; सचे (0 == --refcount) delete एतत्; }
    पूर्णाङ्क refcount_promise;

    mutable तार्किक has_result; // Mat, UMat or exception

    mutable cv::Ptr<Mat> result_mat;
    mutable cv::Ptr<UMat> result_umat;


    तार्किक has_exception;
#सचे CV__EXCEPTION_PTR
    std::exception_ptr exception;
#endif
    cv::Exception cv_exception;

    mutable तार्किक result_is_fetched;

    तार्किक future_is_returned;

    Impl()
        : refcount(1), refcount_future(0), refcount_promise(1)
        , has_result(मिच्छा)
        , has_exception(मिच्छा)
        , result_is_fetched(मिच्छा)
        , future_is_returned(मिच्छा)
    {
        // nothing
    }

    ~Impl()
    {
        सचे (has_result && !result_is_fetched)
        {
            CV_LOG_INFO(रिक्त, "Asynchronous result has not been fetched");
        }
    }

    तार्किक get(OutputArray dst, int64 timeoutNs) ध्रुव
    {
        CV_Assert(!result_is_fetched);
        सचे (!has_result)
        {
            CV_UNUSED(timeoutNs);
            CV_Error(Error::StsError, "Result is not produced (unable to wait for result in OPENCV_DISABLE_THREAD_SUPPORT mode)");
        }
        सचे (!result_mat.empty())
        {
            dst.move(*result_mat.get());
            result_mat.release();
            result_is_fetched = सच्च;
            पटिनिवत्त सच्च;
        }
        सचे (!result_umat.empty())
        {
            dst.move(*result_umat.get());
            result_umat.release();
            result_is_fetched = सच्च;
            पटिनिवत्त सच्च;
        }
#सचे CV__EXCEPTION_PTR
        सचे (has_exception && exception)
        {
            result_is_fetched = सच्च;
            std::rethrow_exception(exception);
        }
#endif
        सचे (has_exception)
        {
            result_is_fetched = सच्च;
            क्षिप cv_exception;
        }
        CV_Error(Error::StsInternal, "AsyncArray: invalid state of 'has_result = true'");
        पटिनिवत्त मिच्छा;
    }

    तार्किक valid() ध्रुव CV_NOEXCEPT
    {
        सचे (result_is_fetched)
            पटिनिवत्त मिच्छा;
        सचे (refcount_promise == 0 && !has_result)
            पटिनिवत्त मिच्छा;
        पटिनिवत्त सच्च;
    }

    तार्किक wait_for(int64 timeoutNs) ध्रुव
    {
        CV_Assert(valid());
        सचे (has_result)
            पटिनिवत्त has_result;
        सचे (timeoutNs == 0)
            पटिनिवत्त has_result;
        CV_Error(Error::StsError, "Unable to wait in OPENCV_DISABLE_THREAD_SUPPORT mode");
    }

    AsyncArray getArrayResult()
    {
        CV_Assert(refcount_future == 0);
        AsyncArray result;
        addrefFuture();
        result.p = एतत्;
        future_is_returned = सच्च;
        पटिनिवत्त result;
    }

    शून्य set_मानम्(निवेशः_Array value)
    {
        सचे (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        CV_Assert(!has_result);
        पूर्णाङ्क k = value.kind();
        सचे (k == निवेशः_Array_2::UMAT)
        {
            result_umat = makePtr<UMat>();
            value.copyTo(*result_umat.get());
        }
        अञ्ञथा
        {
            result_mat = makePtr<Mat>();
            value.copyTo(*result_mat.get());
        }
        has_result = सच्च;
    }

#सचे CV__EXCEPTION_PTR
    शून्य setException(std::exception_ptr e)
    {
        सचे (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        CV_Assert(!has_result);
        has_exception = सच्च;
        exception = e;
        has_result = सच्च;
    }
#endif

    शून्य setException(ध्रुव cv::Exception e)
    {
        सचे (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        CV_Assert(!has_result);
        has_exception = सच्च;
        cv_exception = e;
        has_result = सच्च;
    }
};

}

#endif  // OPENCV_DISABLE_THREAD_SUPPORT

नामाकाश cv {

AsyncArray::AsyncArray() CV_NOEXCEPT
    : p(रिक्त)
{
}

AsyncArray::~AsyncArray() CV_NOEXCEPT
{
    release();
}

AsyncArray::AsyncArray(ध्रुव AsyncArray& o) CV_NOEXCEPT
    : p(o.p)
{
    सचे (p)
        p->addrefFuture();
}

AsyncArray& AsyncArray::operator=(ध्रुव AsyncArray& o) CV_NOEXCEPT
{
    Impl* newp = o.p;
    सचे (newp)
        newp->addrefFuture();
    release();
    p = newp;
    पटिनिवत्त *एतत्;
}

शून्य AsyncArray::release() CV_NOEXCEPT
{
    Impl* impl = p;
    p = रिक्त;
    सचे (impl)
        impl->releaseFuture();
}

तार्किक AsyncArray::get(OutputArray dst, int64 timeoutNs) ध्रुव
{
    CV_Assert(p);
    पटिनिवत्त p->get(dst, timeoutNs);
}

शून्य AsyncArray::get(OutputArray dst) ध्रुव
{
    CV_Assert(p);
    तार्किक res = p->get(dst, -1);
    CV_Assert(res);
}

तार्किक AsyncArray::wait_for(int64 timeoutNs) ध्रुव
{
    CV_Assert(p);
    पटिनिवत्त p->wait_for(timeoutNs);
}

तार्किक AsyncArray::valid() ध्रुव CV_NOEXCEPT
{
    सचे (!p) पटिनिवत्त मिच्छा;
    पटिनिवत्त p->valid();
}


//
// AsyncPromise
//

AsyncPromise::AsyncPromise() CV_NOEXCEPT
    : p(नूतन AsyncArray::Impl())
{
}

AsyncPromise::~AsyncPromise() CV_NOEXCEPT
{
    release();
}

AsyncPromise::AsyncPromise(ध्रुव AsyncPromise& o) CV_NOEXCEPT
    : p(o.p)
{
    सचे (p)
        p->addrefPromise();
}

AsyncPromise& AsyncPromise::operator=(ध्रुव AsyncPromise& o) CV_NOEXCEPT
{
    Impl* newp = o.p;
    सचे (newp)
        newp->addrefPromise();
    release();
    p = newp;
    पटिनिवत्त *एतत्;
}

शून्य AsyncPromise::release() CV_NOEXCEPT
{
    Impl* impl = p;
    p = रिक्त;
    सचे (impl)
        impl->releasePromise();
}

AsyncArray AsyncPromise::getArrayResult()
{
    CV_Assert(p);
    पटिनिवत्त p->getArrayResult();
}

शून्य AsyncPromise::set_मानम्(निवेशः_Array value)
{
    CV_Assert(p);
    पटिनिवत्त p->set_मानम्(value);
}

शून्य AsyncPromise::setException(ध्रुव cv::Exception& exception)
{
    CV_Assert(p);
    पटिनिवत्त p->setException(exception);
}

#सचे CV__EXCEPTION_PTR
शून्य AsyncPromise::setException(std::exception_ptr exception)
{
    CV_Assert(p);
    पटिनिवत्त p->setException(exception);
}
#endif

} // namespace
