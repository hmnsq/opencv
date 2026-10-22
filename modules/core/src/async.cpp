// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#अन्तर्भाव "precomp.hpp"
#अन्तर्भाव "opencv2/core/async.hpp"
#अन्तर्भाव "opencv2/core/detail/async_promise.hpp"

#अन्तर्भाव "opencv2/core/cvstd.hpp"

#अन्तर्भाव <opencv2/core/utils/logger.defines.hpp>
#अपरिभाषय CV_LOG_STRIP_LEVEL
#परिभाषय CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_DEBUG + 1
#अन्तर्भाव <opencv2/core/utils/logger.hpp>

#यदि_अपरिभाषितम् OPENCV_DISABLE_THREAD_SUPPORT

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
    शून्य releaseFuture() CV_NOEXCEPT { CV_XADD(&refcount_future, -1); यदि(1 == CV_XADD(&refcount, -1)) विलोपय एतत्; } \
    पूर्णाङ्क refcount_future;
    शून्य addrefPromise() CV_NOEXCEPT { CV_XADD(&refcount_promise, 1); CV_XADD(&refcount, 1); } \
    शून्य releasePromise() CV_NOEXCEPT { CV_XADD(&refcount_promise, -1); यदि(1 == CV_XADD(&refcount, -1)) विलोपय एतत्; } \
    पूर्णाङ्क refcount_promise;

    परिवर्तनीय std::mutex mtx;
    परिवर्तनीय std::condition_variable cond_var;

    परिवर्तनीय तार्किक has_परिणाम; // Mat, UMat or exception

    परिवर्तनीय cv::Ptr<Mat> परिणाम_mat;
    परिवर्तनीय cv::Ptr<UMat> परिणाम_umat;


    तार्किक has_exception;
#यदि CV__EXCEPTION_PTR
    std::exception_ptr exception;
#यदि_अन्तः
    cv::Exception cv_exception;

    परिवर्तनीय तार्किक परिणाम_is_fetched;

    तार्किक future_is_returned;

    Impl()
        : refcount(1), refcount_future(0), refcount_promise(1)
        , has_परिणाम(असत्य)
        , has_exception(असत्य)
        , परिणाम_is_fetched(असत्य)
        , future_is_returned(असत्य)
    {
        // nothing
    }

    ~Impl()
    {
        यदि (has_परिणाम && !परिणाम_is_fetched)
        {
            CV_LOG_INFO(रिक्त, "Asynchronous result has not been fetched");
        }
    }

    तार्किक get(OutputArray dst, int64 timeoutNs) ध्रुव
    {
        CV_Assert(!परिणाम_is_fetched);
        यदि (!has_परिणाम)
        {
            यदि(refcount_promise == 0)
                CV_Error(Error::Sts_आन्तरिकम्, "Asynchronous result producer has been destroyed");
            यदि (!wait_for(timeoutNs))
                प्रत्यागम असत्य;
        }
        std::unique_lock<std::mutex> lock(mtx);
        यदि (has_परिणाम)
        {
            यदि (!परिणाम_mat.empty())
            {
                dst.move(*परिणाम_mat.get());
                परिणाम_mat.release();
                परिणाम_is_fetched = सत्य;
                प्रत्यागम सत्य;
            }
            यदि (!परिणाम_umat.empty())
            {
                dst.move(*परिणाम_umat.get());
                परिणाम_umat.release();
                परिणाम_is_fetched = सत्य;
                प्रत्यागम सत्य;
            }
#यदि CV__EXCEPTION_PTR
            यदि (has_exception && exception)
            {
                परिणाम_is_fetched = सत्य;
                std::rethrow_exception(exception);
            }
#यदि_अन्तः
            यदि (has_exception)
            {
                परिणाम_is_fetched = सत्य;
                क्षिप cv_exception;
            }
            CV_Error(Error::Sts_आन्तरिकम्, "AsyncArray: invalid state of 'has_result = true'");
        }
        CV_Assert(!has_परिणाम);
        CV_Assert(timeoutNs < 0);
        प्रत्यागम असत्य;
    }

    तार्किक valid() ध्रुव CV_NOEXCEPT
    {
        यदि (परिणाम_is_fetched)
            प्रत्यागम असत्य;
        यदि (refcount_promise == 0 && !has_परिणाम)
            प्रत्यागम असत्य;
        प्रत्यागम सत्य;
    }

    तार्किक wait_for(int64 timeoutNs) ध्रुव
    {
        CV_Assert(valid());
        यदि (has_परिणाम)
            प्रत्यागम has_परिणाम;
        यदि (timeoutNs == 0)
            प्रत्यागम has_परिणाम;
        CV_LOG_INFO(रिक्त, "Waiting for async result ...");
        std::unique_lock<std::mutex> lock(mtx);
        ध्रुव स्वचालित cond_pred = [&]{ प्रत्यागम has_परिणाम == सत्य; };
        यदि (timeoutNs > 0)
            प्रत्यागम cond_var.wait_for(lock, std::chrono::nanoseconds(timeoutNs), cond_pred);
        अन्यथा
        {
            cond_var.प्रतीक्षस्व(lock, cond_pred);
            CV_Assert(has_परिणाम);
            प्रत्यागम सत्य;
        }
    }

    AsyncArray get_Array_परिणाम()
    {
        CV_Assert(refcount_future == 0);
        AsyncArray परिणाम;
        addrefFuture();
        परिणाम.p = एतत्;
        future_is_returned = सत्य;
        प्रत्यागम परिणाम;
    }

    शून्य समुच्चयः_मानम्(निवेशः_Array value)
    {
        यदि (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        std::unique_lock<std::mutex> lock(mtx);
        CV_Assert(!has_परिणाम);
        पूर्णाङ्क k = value.विधा();
        यदि (k == निवेशः_Array_2::UMAT)
        {
            परिणाम_umat = makePtr<UMat>();
            value.copy_गन्तव्यम्(*परिणाम_umat.get());
        }
        अन्यथा
        {
            परिणाम_mat = makePtr<Mat>();
            value.copy_गन्तव्यम्(*परिणाम_mat.get());
        }
        has_परिणाम = सत्य;
        cond_var.notify_all();
    }

#यदि CV__EXCEPTION_PTR
    शून्य समुच्चयः_Exception(std::exception_ptr e)
    {
        यदि (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        std::unique_lock<std::mutex> lock(mtx);
        CV_Assert(!has_परिणाम);
        has_exception = सत्य;
        exception = e;
        has_परिणाम = सत्य;
        cond_var.notify_all();
    }
#यदि_अन्तः

    शून्य समुच्चयः_Exception(ध्रुव cv::Exception e)
    {
        यदि (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        std::unique_lock<std::mutex> lock(mtx);
        CV_Assert(!has_परिणाम);
        has_exception = सत्य;
        cv_exception = e;
        has_परिणाम = सत्य;
        cond_var.notify_all();
    }
};

}  // namespace

#अन्यथा  // OPENCV_DISABLE_THREAD_SUPPORT

नामाकाश cv {

// no threading
संरचना AsyncArray::Impl
{
    पूर्णाङ्क refcount;
    शून्य addrefFuture() CV_NOEXCEPT { refcount_future++; refcount++; }
    शून्य releaseFuture() CV_NOEXCEPT { refcount_future--; यदि (0 == --refcount) विलोपय एतत्; }
    पूर्णाङ्क refcount_future;
    शून्य addrefPromise() CV_NOEXCEPT { refcount_promise++; refcount++; } \
    शून्य releasePromise() CV_NOEXCEPT { refcount_promise--; यदि (0 == --refcount) विलोपय एतत्; }
    पूर्णाङ्क refcount_promise;

    परिवर्तनीय तार्किक has_परिणाम; // Mat, UMat or exception

    परिवर्तनीय cv::Ptr<Mat> परिणाम_mat;
    परिवर्तनीय cv::Ptr<UMat> परिणाम_umat;


    तार्किक has_exception;
#यदि CV__EXCEPTION_PTR
    std::exception_ptr exception;
#यदि_अन्तः
    cv::Exception cv_exception;

    परिवर्तनीय तार्किक परिणाम_is_fetched;

    तार्किक future_is_returned;

    Impl()
        : refcount(1), refcount_future(0), refcount_promise(1)
        , has_परिणाम(असत्य)
        , has_exception(असत्य)
        , परिणाम_is_fetched(असत्य)
        , future_is_returned(असत्य)
    {
        // nothing
    }

    ~Impl()
    {
        यदि (has_परिणाम && !परिणाम_is_fetched)
        {
            CV_LOG_INFO(रिक्त, "Asynchronous result has not been fetched");
        }
    }

    तार्किक get(OutputArray dst, int64 timeoutNs) ध्रुव
    {
        CV_Assert(!परिणाम_is_fetched);
        यदि (!has_परिणाम)
        {
            CV_अप्रयुक्तम्(timeoutNs);
            CV_Error(Error::StsError, "Result is not produced (unable to wait for result in OPENCV_DISABLE_THREAD_SUPPORT mode)");
        }
        यदि (!परिणाम_mat.empty())
        {
            dst.move(*परिणाम_mat.get());
            परिणाम_mat.release();
            परिणाम_is_fetched = सत्य;
            प्रत्यागम सत्य;
        }
        यदि (!परिणाम_umat.empty())
        {
            dst.move(*परिणाम_umat.get());
            परिणाम_umat.release();
            परिणाम_is_fetched = सत्य;
            प्रत्यागम सत्य;
        }
#यदि CV__EXCEPTION_PTR
        यदि (has_exception && exception)
        {
            परिणाम_is_fetched = सत्य;
            std::rethrow_exception(exception);
        }
#यदि_अन्तः
        यदि (has_exception)
        {
            परिणाम_is_fetched = सत्य;
            क्षिप cv_exception;
        }
        CV_Error(Error::Sts_आन्तरिकम्, "AsyncArray: invalid state of 'has_result = true'");
        प्रत्यागम असत्य;
    }

    तार्किक valid() ध्रुव CV_NOEXCEPT
    {
        यदि (परिणाम_is_fetched)
            प्रत्यागम असत्य;
        यदि (refcount_promise == 0 && !has_परिणाम)
            प्रत्यागम असत्य;
        प्रत्यागम सत्य;
    }

    तार्किक wait_for(int64 timeoutNs) ध्रुव
    {
        CV_Assert(valid());
        यदि (has_परिणाम)
            प्रत्यागम has_परिणाम;
        यदि (timeoutNs == 0)
            प्रत्यागम has_परिणाम;
        CV_Error(Error::StsError, "Unable to wait in OPENCV_DISABLE_THREAD_SUPPORT mode");
    }

    AsyncArray get_Array_परिणाम()
    {
        CV_Assert(refcount_future == 0);
        AsyncArray परिणाम;
        addrefFuture();
        परिणाम.p = एतत्;
        future_is_returned = सत्य;
        प्रत्यागम परिणाम;
    }

    शून्य समुच्चयः_मानम्(निवेशः_Array value)
    {
        यदि (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        CV_Assert(!has_परिणाम);
        पूर्णाङ्क k = value.विधा();
        यदि (k == निवेशः_Array_2::UMAT)
        {
            परिणाम_umat = makePtr<UMat>();
            value.copy_गन्तव्यम्(*परिणाम_umat.get());
        }
        अन्यथा
        {
            परिणाम_mat = makePtr<Mat>();
            value.copy_गन्तव्यम्(*परिणाम_mat.get());
        }
        has_परिणाम = सत्य;
    }

#यदि CV__EXCEPTION_PTR
    शून्य समुच्चयः_Exception(std::exception_ptr e)
    {
        यदि (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        CV_Assert(!has_परिणाम);
        has_exception = सत्य;
        exception = e;
        has_परिणाम = सत्य;
    }
#यदि_अन्तः

    शून्य समुच्चयः_Exception(ध्रुव cv::Exception e)
    {
        यदि (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        CV_Assert(!has_परिणाम);
        has_exception = सत्य;
        cv_exception = e;
        has_परिणाम = सत्य;
    }
};

}

#यदि_अन्तः  // OPENCV_DISABLE_THREAD_SUPPORT

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
    यदि (p)
        p->addrefFuture();
}

AsyncArray& AsyncArray::संक्रियकः=(ध्रुव AsyncArray& o) CV_NOEXCEPT
{
    Impl* newp = o.p;
    यदि (newp)
        newp->addrefFuture();
    release();
    p = newp;
    प्रत्यागम *एतत्;
}

शून्य AsyncArray::release() CV_NOEXCEPT
{
    Impl* impl = p;
    p = रिक्त;
    यदि (impl)
        impl->releaseFuture();
}

तार्किक AsyncArray::get(OutputArray dst, int64 timeoutNs) ध्रुव
{
    CV_Assert(p);
    प्रत्यागम p->get(dst, timeoutNs);
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
    प्रत्यागम p->wait_for(timeoutNs);
}

तार्किक AsyncArray::valid() ध्रुव CV_NOEXCEPT
{
    यदि (!p) प्रत्यागम असत्य;
    प्रत्यागम p->valid();
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
    यदि (p)
        p->addrefPromise();
}

AsyncPromise& AsyncPromise::संक्रियकः=(ध्रुव AsyncPromise& o) CV_NOEXCEPT
{
    Impl* newp = o.p;
    यदि (newp)
        newp->addrefPromise();
    release();
    p = newp;
    प्रत्यागम *एतत्;
}

शून्य AsyncPromise::release() CV_NOEXCEPT
{
    Impl* impl = p;
    p = रिक्त;
    यदि (impl)
        impl->releasePromise();
}

AsyncArray AsyncPromise::get_Array_परिणाम()
{
    CV_Assert(p);
    प्रत्यागम p->get_Array_परिणाम();
}

शून्य AsyncPromise::समुच्चयः_मानम्(निवेशः_Array value)
{
    CV_Assert(p);
    प्रत्यागम p->समुच्चयः_मानम्(value);
}

शून्य AsyncPromise::समुच्चयः_Exception(ध्रुव cv::Exception& exception)
{
    CV_Assert(p);
    प्रत्यागम p->समुच्चयः_Exception(exception);
}

#यदि CV__EXCEPTION_PTR
शून्य AsyncPromise::समुच्चयः_Exception(std::exception_ptr exception)
{
    CV_Assert(p);
    प्रत्यागम p->समुच्चयः_Exception(exception);
}
#यदि_अन्तः

} // namespace
