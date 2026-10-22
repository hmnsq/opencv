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
    पूर्णांक refcount;
    शून्य addrefFuture() CV_NOEXCEPT { CV_XADD(&refcount_future, 1); CV_XADD(&refcount, 1); } \
    शून्य releaseFuture() CV_NOEXCEPT { CV_XADD(&refcount_future, -1); जर(1 == CV_XADD(&refcount, -1)) delete हे; } \
    पूर्णांक refcount_future;
    शून्य addrefPromise() CV_NOEXCEPT { CV_XADD(&refcount_promise, 1); CV_XADD(&refcount, 1); } \
    शून्य releasePromise() CV_NOEXCEPT { CV_XADD(&refcount_promise, -1); जर(1 == CV_XADD(&refcount, -1)) delete हे; } \
    पूर्णांक refcount_promise;

    mutable std::mutex mtx;
    mutable std::condition_variable cond_var;

    mutable तार्किक has_result; // Mat, UMat or exception

    mutable cv::Ptr<Mat> result_mat;
    mutable cv::Ptr<UMat> result_umat;


    तार्किक has_exception;
#जर CV__EXCEPTION_PTR
    std::exception_ptr exception;
#endif
    cv::Exception cv_exception;

    mutable तार्किक result_is_fetched;

    तार्किक future_is_returned;

    Impl()
        : refcount(1), refcount_future(0), refcount_promise(1)
        , has_result(असत्य)
        , has_exception(असत्य)
        , result_is_fetched(असत्य)
        , future_is_returned(असत्य)
    {
        // nothing
    }

    ~Impl()
    {
        जर (has_result && !result_is_fetched)
        {
            CV_LOG_INFO(रिक्त, "Asynchronous result has not been fetched");
        }
    }

    तार्किक get(OutputArray dst, int64 timeoutNs) स्थिरांक
    {
        CV_Assert(!result_is_fetched);
        जर (!has_result)
        {
            जर(refcount_promise == 0)
                CV_Error(Error::StsInternal, "Asynchronous result producer has been destroyed");
            जर (!wait_for(timeoutNs))
                परत असत्य;
        }
        std::unique_lock<std::mutex> lock(mtx);
        जर (has_result)
        {
            जर (!result_mat.empty())
            {
                dst.move(*result_mat.get());
                result_mat.release();
                result_is_fetched = सत्य;
                परत सत्य;
            }
            जर (!result_umat.empty())
            {
                dst.move(*result_umat.get());
                result_umat.release();
                result_is_fetched = सत्य;
                परत सत्य;
            }
#जर CV__EXCEPTION_PTR
            जर (has_exception && exception)
            {
                result_is_fetched = सत्य;
                std::rethrow_exception(exception);
            }
#endif
            जर (has_exception)
            {
                result_is_fetched = सत्य;
                फेका cv_exception;
            }
            CV_Error(Error::StsInternal, "AsyncArray: invalid state of 'has_result = true'");
        }
        CV_Assert(!has_result);
        CV_Assert(timeoutNs < 0);
        परत असत्य;
    }

    तार्किक valid() स्थिरांक CV_NOEXCEPT
    {
        जर (result_is_fetched)
            परत असत्य;
        जर (refcount_promise == 0 && !has_result)
            परत असत्य;
        परत सत्य;
    }

    तार्किक wait_for(int64 timeoutNs) स्थिरांक
    {
        CV_Assert(valid());
        जर (has_result)
            परत has_result;
        जर (timeoutNs == 0)
            परत has_result;
        CV_LOG_INFO(रिक्त, "Waiting for async result ...");
        std::unique_lock<std::mutex> lock(mtx);
        स्थिरांक स्वतः cond_pred = [&]{ परत has_result == सत्य; };
        जर (timeoutNs > 0)
            परत cond_var.wait_for(lock, std::chrono::nanoseconds(timeoutNs), cond_pred);
        अन्यथा
        {
            cond_var.रुको(lock, cond_pred);
            CV_Assert(has_result);
            परत सत्य;
        }
    }

    AsyncArray getArrayResult()
    {
        CV_Assert(refcount_future == 0);
        AsyncArray result;
        addrefFuture();
        result.p = हे;
        future_is_returned = सत्य;
        परत result;
    }

    शून्य set_मूल्य(आदान_Array value)
    {
        जर (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        std::unique_lock<std::mutex> lock(mtx);
        CV_Assert(!has_result);
        पूर्णांक k = value.kind();
        जर (k == आदान_Array_2::UMAT)
        {
            result_umat = makePtr<UMat>();
            value.copyTo(*result_umat.get());
        }
        अन्यथा
        {
            result_mat = makePtr<Mat>();
            value.copyTo(*result_mat.get());
        }
        has_result = सत्य;
        cond_var.notify_all();
    }

#जर CV__EXCEPTION_PTR
    शून्य setException(std::exception_ptr e)
    {
        जर (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        std::unique_lock<std::mutex> lock(mtx);
        CV_Assert(!has_result);
        has_exception = सत्य;
        exception = e;
        has_result = सत्य;
        cond_var.notify_all();
    }
#endif

    शून्य setException(स्थिरांक cv::Exception e)
    {
        जर (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        std::unique_lock<std::mutex> lock(mtx);
        CV_Assert(!has_result);
        has_exception = सत्य;
        cv_exception = e;
        has_result = सत्य;
        cond_var.notify_all();
    }
};

}  // namespace

#अन्यथा  // OPENCV_DISABLE_THREAD_SUPPORT

नामाकाश cv {

// no threading
संरचना AsyncArray::Impl
{
    पूर्णांक refcount;
    शून्य addrefFuture() CV_NOEXCEPT { refcount_future++; refcount++; }
    शून्य releaseFuture() CV_NOEXCEPT { refcount_future--; जर (0 == --refcount) delete हे; }
    पूर्णांक refcount_future;
    शून्य addrefPromise() CV_NOEXCEPT { refcount_promise++; refcount++; } \
    शून्य releasePromise() CV_NOEXCEPT { refcount_promise--; जर (0 == --refcount) delete हे; }
    पूर्णांक refcount_promise;

    mutable तार्किक has_result; // Mat, UMat or exception

    mutable cv::Ptr<Mat> result_mat;
    mutable cv::Ptr<UMat> result_umat;


    तार्किक has_exception;
#जर CV__EXCEPTION_PTR
    std::exception_ptr exception;
#endif
    cv::Exception cv_exception;

    mutable तार्किक result_is_fetched;

    तार्किक future_is_returned;

    Impl()
        : refcount(1), refcount_future(0), refcount_promise(1)
        , has_result(असत्य)
        , has_exception(असत्य)
        , result_is_fetched(असत्य)
        , future_is_returned(असत्य)
    {
        // nothing
    }

    ~Impl()
    {
        जर (has_result && !result_is_fetched)
        {
            CV_LOG_INFO(रिक्त, "Asynchronous result has not been fetched");
        }
    }

    तार्किक get(OutputArray dst, int64 timeoutNs) स्थिरांक
    {
        CV_Assert(!result_is_fetched);
        जर (!has_result)
        {
            CV_UNUSED(timeoutNs);
            CV_Error(Error::StsError, "Result is not produced (unable to wait for result in OPENCV_DISABLE_THREAD_SUPPORT mode)");
        }
        जर (!result_mat.empty())
        {
            dst.move(*result_mat.get());
            result_mat.release();
            result_is_fetched = सत्य;
            परत सत्य;
        }
        जर (!result_umat.empty())
        {
            dst.move(*result_umat.get());
            result_umat.release();
            result_is_fetched = सत्य;
            परत सत्य;
        }
#जर CV__EXCEPTION_PTR
        जर (has_exception && exception)
        {
            result_is_fetched = सत्य;
            std::rethrow_exception(exception);
        }
#endif
        जर (has_exception)
        {
            result_is_fetched = सत्य;
            फेका cv_exception;
        }
        CV_Error(Error::StsInternal, "AsyncArray: invalid state of 'has_result = true'");
        परत असत्य;
    }

    तार्किक valid() स्थिरांक CV_NOEXCEPT
    {
        जर (result_is_fetched)
            परत असत्य;
        जर (refcount_promise == 0 && !has_result)
            परत असत्य;
        परत सत्य;
    }

    तार्किक wait_for(int64 timeoutNs) स्थिरांक
    {
        CV_Assert(valid());
        जर (has_result)
            परत has_result;
        जर (timeoutNs == 0)
            परत has_result;
        CV_Error(Error::StsError, "Unable to wait in OPENCV_DISABLE_THREAD_SUPPORT mode");
    }

    AsyncArray getArrayResult()
    {
        CV_Assert(refcount_future == 0);
        AsyncArray result;
        addrefFuture();
        result.p = हे;
        future_is_returned = सत्य;
        परत result;
    }

    शून्य set_मूल्य(आदान_Array value)
    {
        जर (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        CV_Assert(!has_result);
        पूर्णांक k = value.kind();
        जर (k == आदान_Array_2::UMAT)
        {
            result_umat = makePtr<UMat>();
            value.copyTo(*result_umat.get());
        }
        अन्यथा
        {
            result_mat = makePtr<Mat>();
            value.copyTo(*result_mat.get());
        }
        has_result = सत्य;
    }

#जर CV__EXCEPTION_PTR
    शून्य setException(std::exception_ptr e)
    {
        जर (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        CV_Assert(!has_result);
        has_exception = सत्य;
        exception = e;
        has_result = सत्य;
    }
#endif

    शून्य setException(स्थिरांक cv::Exception e)
    {
        जर (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        CV_Assert(!has_result);
        has_exception = सत्य;
        cv_exception = e;
        has_result = सत्य;
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

AsyncArray::AsyncArray(स्थिरांक AsyncArray& o) CV_NOEXCEPT
    : p(o.p)
{
    जर (p)
        p->addrefFuture();
}

AsyncArray& AsyncArray::चालक=(स्थिरांक AsyncArray& o) CV_NOEXCEPT
{
    Impl* newp = o.p;
    जर (newp)
        newp->addrefFuture();
    release();
    p = newp;
    परत *हे;
}

शून्य AsyncArray::release() CV_NOEXCEPT
{
    Impl* impl = p;
    p = रिक्त;
    जर (impl)
        impl->releaseFuture();
}

तार्किक AsyncArray::get(OutputArray dst, int64 timeoutNs) स्थिरांक
{
    CV_Assert(p);
    परत p->get(dst, timeoutNs);
}

शून्य AsyncArray::get(OutputArray dst) स्थिरांक
{
    CV_Assert(p);
    तार्किक res = p->get(dst, -1);
    CV_Assert(res);
}

तार्किक AsyncArray::wait_for(int64 timeoutNs) स्थिरांक
{
    CV_Assert(p);
    परत p->wait_for(timeoutNs);
}

तार्किक AsyncArray::valid() स्थिरांक CV_NOEXCEPT
{
    जर (!p) परत असत्य;
    परत p->valid();
}


//
// AsyncPromise
//

AsyncPromise::AsyncPromise() CV_NOEXCEPT
    : p(नवीन AsyncArray::Impl())
{
}

AsyncPromise::~AsyncPromise() CV_NOEXCEPT
{
    release();
}

AsyncPromise::AsyncPromise(स्थिरांक AsyncPromise& o) CV_NOEXCEPT
    : p(o.p)
{
    जर (p)
        p->addrefPromise();
}

AsyncPromise& AsyncPromise::चालक=(स्थिरांक AsyncPromise& o) CV_NOEXCEPT
{
    Impl* newp = o.p;
    जर (newp)
        newp->addrefPromise();
    release();
    p = newp;
    परत *हे;
}

शून्य AsyncPromise::release() CV_NOEXCEPT
{
    Impl* impl = p;
    p = रिक्त;
    जर (impl)
        impl->releasePromise();
}

AsyncArray AsyncPromise::getArrayResult()
{
    CV_Assert(p);
    परत p->getArrayResult();
}

शून्य AsyncPromise::set_मूल्य(आदान_Array value)
{
    CV_Assert(p);
    परत p->set_मूल्य(value);
}

शून्य AsyncPromise::setException(स्थिरांक cv::Exception& exception)
{
    CV_Assert(p);
    परत p->setException(exception);
}

#जर CV__EXCEPTION_PTR
शून्य AsyncPromise::setException(std::exception_ptr exception)
{
    CV_Assert(p);
    परत p->setException(exception);
}
#endif

} // namespace
