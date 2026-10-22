// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#অন্তর্ভাব "precomp.hpp"
#অন্তর্ভাব "opencv2/core/async.hpp"
#অন্তর্ভাব "opencv2/core/detail/async_promise.hpp"

#অন্তর্ভাব "opencv2/core/cvstd.hpp"

#অন্তর্ভাব <opencv2/core/utils/logger.defines.hpp>
#undef CV_LOG_STRIP_LEVEL
#define CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_DEBUG + 1
#অন্তর্ভাব <opencv2/core/utils/logger.hpp>

#ifndef OPENCV_DISABLE_THREAD_SUPPORT

#অন্তর্ভাব <mutex>
#অন্তর্ভাব <condition_variable>
#অন্তর্ভাব <chrono>

নামাকাশ cv {

/**
Manages shared state of asynchronous result
*/
কাঠামো AsyncArray::Impl
{
    পূর্ণসংখ্যা refcount;
    শূন্য addrefFuture() CV_NOEXCEPT { CV_XADD(&refcount_future, 1); CV_XADD(&refcount, 1); } \
    শূন্য releaseFuture() CV_NOEXCEPT { CV_XADD(&refcount_future, -1); যদি(1 == CV_XADD(&refcount, -1)) delete এই; } \
    পূর্ণসংখ্যা refcount_future;
    শূন্য addrefPromise() CV_NOEXCEPT { CV_XADD(&refcount_promise, 1); CV_XADD(&refcount, 1); } \
    শূন্য releasePromise() CV_NOEXCEPT { CV_XADD(&refcount_promise, -1); যদি(1 == CV_XADD(&refcount, -1)) delete এই; } \
    পূর্ণসংখ্যা refcount_promise;

    mutable std::mutex mtx;
    mutable std::condition_variable cond_var;

    mutable যৌক্তিক has_ফলাফল; // Mat, UMat or exception

    mutable cv::Ptr<Mat> ফলাফল_mat;
    mutable cv::Ptr<UMat> ফলাফল_umat;


    যৌক্তিক has_exception;
#যদি CV__EXCEPTION_PTR
    std::exception_ptr exception;
#endif
    cv::Exception cv_exception;

    mutable যৌক্তিক ফলাফল_is_fetched;

    যৌক্তিক future_is_returned;

    Impl()
        : refcount(1), refcount_future(0), refcount_promise(1)
        , has_ফলাফল(মিথ্যা)
        , has_exception(মিথ্যা)
        , ফলাফল_is_fetched(মিথ্যা)
        , future_is_returned(মিথ্যা)
    {
        // nothing
    }

    ~Impl()
    {
        যদি (has_ফলাফল && !ফলাফল_is_fetched)
        {
            CV_LOG_INFO(ফাঁকা, "Asynchronous result has not been fetched");
        }
    }

    যৌক্তিক get(OutputArray dst, int64 timeoutNs) ধ্রুবক
    {
        CV_Assert(!ফলাফল_is_fetched);
        যদি (!has_ফলাফল)
        {
            যদি(refcount_promise == 0)
                CV_Error(Error::StsInternal, "Asynchronous result producer has been destroyed");
            যদি (!wait_for(timeoutNs))
                ফেরত মিথ্যা;
        }
        std::unique_lock<std::mutex> lock(mtx);
        যদি (has_ফলাফল)
        {
            যদি (!ফলাফল_mat.empty())
            {
                dst.move(*ফলাফল_mat.get());
                ফলাফল_mat.release();
                ফলাফল_is_fetched = সত্য;
                ফেরত সত্য;
            }
            যদি (!ফলাফল_umat.empty())
            {
                dst.move(*ফলাফল_umat.get());
                ফলাফল_umat.release();
                ফলাফল_is_fetched = সত্য;
                ফেরত সত্য;
            }
#যদি CV__EXCEPTION_PTR
            যদি (has_exception && exception)
            {
                ফলাফল_is_fetched = সত্য;
                std::rethrow_exception(exception);
            }
#endif
            যদি (has_exception)
            {
                ফলাফল_is_fetched = সত্য;
                ছোড়ো cv_exception;
            }
            CV_Error(Error::StsInternal, "AsyncArray: invalid state of 'has_result = true'");
        }
        CV_Assert(!has_ফলাফল);
        CV_Assert(timeoutNs < 0);
        ফেরত মিথ্যা;
    }

    যৌক্তিক valid() ধ্রুবক CV_NOEXCEPT
    {
        যদি (ফলাফল_is_fetched)
            ফেরত মিথ্যা;
        যদি (refcount_promise == 0 && !has_ফলাফল)
            ফেরত মিথ্যা;
        ফেরত সত্য;
    }

    যৌক্তিক wait_for(int64 timeoutNs) ধ্রুবক
    {
        CV_Assert(valid());
        যদি (has_ফলাফল)
            ফেরত has_ফলাফল;
        যদি (timeoutNs == 0)
            ফেরত has_ফলাফল;
        CV_LOG_INFO(ফাঁকা, "Waiting for async result ...");
        std::unique_lock<std::mutex> lock(mtx);
        ধ্রুবক auto cond_pred = [&]{ ফেরত has_ফলাফল == সত্য; };
        যদি (timeoutNs > 0)
            ফেরত cond_var.wait_for(lock, std::chrono::nanoseconds(timeoutNs), cond_pred);
        অন্যথা
        {
            cond_var.wait(lock, cond_pred);
            CV_Assert(has_ফলাফল);
            ফেরত সত্য;
        }
    }

    AsyncArray get_Array_ফলাফল()
    {
        CV_Assert(refcount_future == 0);
        AsyncArray ফলাফল;
        addrefFuture();
        ফলাফল.p = এই;
        future_is_returned = সত্য;
        ফেরত ফলাফল;
    }

    শূন্য set_মান(প্রবেশ_Array value)
    {
        যদি (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        std::unique_lock<std::mutex> lock(mtx);
        CV_Assert(!has_ফলাফল);
        পূর্ণসংখ্যা k = value.kind();
        যদি (k == প্রবেশ_Array_2::UMAT)
        {
            ফলাফল_umat = makePtr<UMat>();
            value.copyTo(*ফলাফল_umat.get());
        }
        অন্যথা
        {
            ফলাফল_mat = makePtr<Mat>();
            value.copyTo(*ফলাফল_mat.get());
        }
        has_ফলাফল = সত্য;
        cond_var.notify_all();
    }

#যদি CV__EXCEPTION_PTR
    শূন্য setException(std::exception_ptr e)
    {
        যদি (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        std::unique_lock<std::mutex> lock(mtx);
        CV_Assert(!has_ফলাফল);
        has_exception = সত্য;
        exception = e;
        has_ফলাফল = সত্য;
        cond_var.notify_all();
    }
#endif

    শূন্য setException(ধ্রুবক cv::Exception e)
    {
        যদি (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        std::unique_lock<std::mutex> lock(mtx);
        CV_Assert(!has_ফলাফল);
        has_exception = সত্য;
        cv_exception = e;
        has_ফলাফল = সত্য;
        cond_var.notify_all();
    }
};

}  // namespace

#অন্যথা  // OPENCV_DISABLE_THREAD_SUPPORT

নামাকাশ cv {

// no threading
কাঠামো AsyncArray::Impl
{
    পূর্ণসংখ্যা refcount;
    শূন্য addrefFuture() CV_NOEXCEPT { refcount_future++; refcount++; }
    শূন্য releaseFuture() CV_NOEXCEPT { refcount_future--; যদি (0 == --refcount) delete এই; }
    পূর্ণসংখ্যা refcount_future;
    শূন্য addrefPromise() CV_NOEXCEPT { refcount_promise++; refcount++; } \
    শূন্য releasePromise() CV_NOEXCEPT { refcount_promise--; যদি (0 == --refcount) delete এই; }
    পূর্ণসংখ্যা refcount_promise;

    mutable যৌক্তিক has_ফলাফল; // Mat, UMat or exception

    mutable cv::Ptr<Mat> ফলাফল_mat;
    mutable cv::Ptr<UMat> ফলাফল_umat;


    যৌক্তিক has_exception;
#যদি CV__EXCEPTION_PTR
    std::exception_ptr exception;
#endif
    cv::Exception cv_exception;

    mutable যৌক্তিক ফলাফল_is_fetched;

    যৌক্তিক future_is_returned;

    Impl()
        : refcount(1), refcount_future(0), refcount_promise(1)
        , has_ফলাফল(মিথ্যা)
        , has_exception(মিথ্যা)
        , ফলাফল_is_fetched(মিথ্যা)
        , future_is_returned(মিথ্যা)
    {
        // nothing
    }

    ~Impl()
    {
        যদি (has_ফলাফল && !ফলাফল_is_fetched)
        {
            CV_LOG_INFO(ফাঁকা, "Asynchronous result has not been fetched");
        }
    }

    যৌক্তিক get(OutputArray dst, int64 timeoutNs) ধ্রুবক
    {
        CV_Assert(!ফলাফল_is_fetched);
        যদি (!has_ফলাফল)
        {
            CV_UNUSED(timeoutNs);
            CV_Error(Error::StsError, "Result is not produced (unable to wait for result in OPENCV_DISABLE_THREAD_SUPPORT mode)");
        }
        যদি (!ফলাফল_mat.empty())
        {
            dst.move(*ফলাফল_mat.get());
            ফলাফল_mat.release();
            ফলাফল_is_fetched = সত্য;
            ফেরত সত্য;
        }
        যদি (!ফলাফল_umat.empty())
        {
            dst.move(*ফলাফল_umat.get());
            ফলাফল_umat.release();
            ফলাফল_is_fetched = সত্য;
            ফেরত সত্য;
        }
#যদি CV__EXCEPTION_PTR
        যদি (has_exception && exception)
        {
            ফলাফল_is_fetched = সত্য;
            std::rethrow_exception(exception);
        }
#endif
        যদি (has_exception)
        {
            ফলাফল_is_fetched = সত্য;
            ছোড়ো cv_exception;
        }
        CV_Error(Error::StsInternal, "AsyncArray: invalid state of 'has_result = true'");
        ফেরত মিথ্যা;
    }

    যৌক্তিক valid() ধ্রুবক CV_NOEXCEPT
    {
        যদি (ফলাফল_is_fetched)
            ফেরত মিথ্যা;
        যদি (refcount_promise == 0 && !has_ফলাফল)
            ফেরত মিথ্যা;
        ফেরত সত্য;
    }

    যৌক্তিক wait_for(int64 timeoutNs) ধ্রুবক
    {
        CV_Assert(valid());
        যদি (has_ফলাফল)
            ফেরত has_ফলাফল;
        যদি (timeoutNs == 0)
            ফেরত has_ফলাফল;
        CV_Error(Error::StsError, "Unable to wait in OPENCV_DISABLE_THREAD_SUPPORT mode");
    }

    AsyncArray get_Array_ফলাফল()
    {
        CV_Assert(refcount_future == 0);
        AsyncArray ফলাফল;
        addrefFuture();
        ফলাফল.p = এই;
        future_is_returned = সত্য;
        ফেরত ফলাফল;
    }

    শূন্য set_মান(প্রবেশ_Array value)
    {
        যদি (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        CV_Assert(!has_ফলাফল);
        পূর্ণসংখ্যা k = value.kind();
        যদি (k == প্রবেশ_Array_2::UMAT)
        {
            ফলাফল_umat = makePtr<UMat>();
            value.copyTo(*ফলাফল_umat.get());
        }
        অন্যথা
        {
            ফলাফল_mat = makePtr<Mat>();
            value.copyTo(*ফলাফল_mat.get());
        }
        has_ফলাফল = সত্য;
    }

#যদি CV__EXCEPTION_PTR
    শূন্য setException(std::exception_ptr e)
    {
        যদি (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        CV_Assert(!has_ফলাফল);
        has_exception = সত্য;
        exception = e;
        has_ফলাফল = সত্য;
    }
#endif

    শূন্য setException(ধ্রুবক cv::Exception e)
    {
        যদি (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        CV_Assert(!has_ফলাফল);
        has_exception = সত্য;
        cv_exception = e;
        has_ফলাফল = সত্য;
    }
};

}

#endif  // OPENCV_DISABLE_THREAD_SUPPORT

নামাকাশ cv {

AsyncArray::AsyncArray() CV_NOEXCEPT
    : p(ফাঁকা)
{
}

AsyncArray::~AsyncArray() CV_NOEXCEPT
{
    release();
}

AsyncArray::AsyncArray(ধ্রুবক AsyncArray& o) CV_NOEXCEPT
    : p(o.p)
{
    যদি (p)
        p->addrefFuture();
}

AsyncArray& AsyncArray::operator=(ধ্রুবক AsyncArray& o) CV_NOEXCEPT
{
    Impl* newp = o.p;
    যদি (newp)
        newp->addrefFuture();
    release();
    p = newp;
    ফেরত *এই;
}

শূন্য AsyncArray::release() CV_NOEXCEPT
{
    Impl* impl = p;
    p = ফাঁকা;
    যদি (impl)
        impl->releaseFuture();
}

যৌক্তিক AsyncArray::get(OutputArray dst, int64 timeoutNs) ধ্রুবক
{
    CV_Assert(p);
    ফেরত p->get(dst, timeoutNs);
}

শূন্য AsyncArray::get(OutputArray dst) ধ্রুবক
{
    CV_Assert(p);
    যৌক্তিক res = p->get(dst, -1);
    CV_Assert(res);
}

যৌক্তিক AsyncArray::wait_for(int64 timeoutNs) ধ্রুবক
{
    CV_Assert(p);
    ফেরত p->wait_for(timeoutNs);
}

যৌক্তিক AsyncArray::valid() ধ্রুবক CV_NOEXCEPT
{
    যদি (!p) ফেরত মিথ্যা;
    ফেরত p->valid();
}


//
// AsyncPromise
//

AsyncPromise::AsyncPromise() CV_NOEXCEPT
    : p(নতুন AsyncArray::Impl())
{
}

AsyncPromise::~AsyncPromise() CV_NOEXCEPT
{
    release();
}

AsyncPromise::AsyncPromise(ধ্রুবক AsyncPromise& o) CV_NOEXCEPT
    : p(o.p)
{
    যদি (p)
        p->addrefPromise();
}

AsyncPromise& AsyncPromise::operator=(ধ্রুবক AsyncPromise& o) CV_NOEXCEPT
{
    Impl* newp = o.p;
    যদি (newp)
        newp->addrefPromise();
    release();
    p = newp;
    ফেরত *এই;
}

শূন্য AsyncPromise::release() CV_NOEXCEPT
{
    Impl* impl = p;
    p = ফাঁকা;
    যদি (impl)
        impl->releasePromise();
}

AsyncArray AsyncPromise::get_Array_ফলাফল()
{
    CV_Assert(p);
    ফেরত p->get_Array_ফলাফল();
}

শূন্য AsyncPromise::set_মান(প্রবেশ_Array value)
{
    CV_Assert(p);
    ফেরত p->set_মান(value);
}

শূন্য AsyncPromise::setException(ধ্রুবক cv::Exception& exception)
{
    CV_Assert(p);
    ফেরত p->setException(exception);
}

#যদি CV__EXCEPTION_PTR
শূন্য AsyncPromise::setException(std::exception_ptr exception)
{
    CV_Assert(p);
    ফেরত p->setException(exception);
}
#endif

} // namespace
