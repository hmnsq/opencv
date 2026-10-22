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
সংরচনা AsyncArray::Impl
{
    পূর্ণাঙ্ক refcount;
    শূন্য addrefFuture() CV_NOEXCEPT { CV_XADD(&refcount_future, 1); CV_XADD(&refcount, 1); } \
    শূন্য releaseFuture() CV_NOEXCEPT { CV_XADD(&refcount_future, -1); যদি(1 == CV_XADD(&refcount, -1)) delete এতত্; } \
    পূর্ণাঙ্ক refcount_future;
    শূন্য addrefPromise() CV_NOEXCEPT { CV_XADD(&refcount_promise, 1); CV_XADD(&refcount, 1); } \
    শূন্য releasePromise() CV_NOEXCEPT { CV_XADD(&refcount_promise, -1); যদি(1 == CV_XADD(&refcount, -1)) delete এতত্; } \
    পূর্ণাঙ্ক refcount_promise;

    mutable std::mutex mtx;
    mutable std::condition_variable cond_var;

    mutable তার্কিক has_result; // Mat, UMat or exception

    mutable cv::Ptr<Mat> result_mat;
    mutable cv::Ptr<UMat> result_umat;


    তার্কিক has_exception;
#যদি CV__EXCEPTION_PTR
    std::exception_ptr exception;
#endif
    cv::Exception cv_exception;

    mutable তার্কিক result_is_fetched;

    তার্কিক future_is_returned;

    Impl()
        : refcount(1), refcount_future(0), refcount_promise(1)
        , has_result(অসত্য)
        , has_exception(অসত্য)
        , result_is_fetched(অসত্য)
        , future_is_returned(অসত্য)
    {
        // nothing
    }

    ~Impl()
    {
        যদি (has_result && !result_is_fetched)
        {
            CV_LOG_INFO(রিক্ত, "Asynchronous result has not been fetched");
        }
    }

    তার্কিক get(OutputArray dst, int64 timeoutNs) ধ্রুব
    {
        CV_Assert(!result_is_fetched);
        যদি (!has_result)
        {
            যদি(refcount_promise == 0)
                CV_Error(Error::StsInternal, "Asynchronous result producer has been destroyed");
            যদি (!wait_for(timeoutNs))
                প্রত্যাগম অসত্য;
        }
        std::unique_lock<std::mutex> lock(mtx);
        যদি (has_result)
        {
            যদি (!result_mat.empty())
            {
                dst.move(*result_mat.get());
                result_mat.release();
                result_is_fetched = সত্য;
                প্রত্যাগম সত্য;
            }
            যদি (!result_umat.empty())
            {
                dst.move(*result_umat.get());
                result_umat.release();
                result_is_fetched = সত্য;
                প্রত্যাগম সত্য;
            }
#যদি CV__EXCEPTION_PTR
            যদি (has_exception && exception)
            {
                result_is_fetched = সত্য;
                std::rethrow_exception(exception);
            }
#endif
            যদি (has_exception)
            {
                result_is_fetched = সত্য;
                ক্ষিপ cv_exception;
            }
            CV_Error(Error::StsInternal, "AsyncArray: invalid state of 'has_result = true'");
        }
        CV_Assert(!has_result);
        CV_Assert(timeoutNs < 0);
        প্রত্যাগম অসত্য;
    }

    তার্কিক valid() ধ্রুব CV_NOEXCEPT
    {
        যদি (result_is_fetched)
            প্রত্যাগম অসত্য;
        যদি (refcount_promise == 0 && !has_result)
            প্রত্যাগম অসত্য;
        প্রত্যাগম সত্য;
    }

    তার্কিক wait_for(int64 timeoutNs) ধ্রুব
    {
        CV_Assert(valid());
        যদি (has_result)
            প্রত্যাগম has_result;
        যদি (timeoutNs == 0)
            প্রত্যাগম has_result;
        CV_LOG_INFO(রিক্ত, "Waiting for async result ...");
        std::unique_lock<std::mutex> lock(mtx);
        ধ্রুব auto cond_pred = [&]{ প্রত্যাগম has_result == সত্য; };
        যদি (timeoutNs > 0)
            প্রত্যাগম cond_var.wait_for(lock, std::chrono::nanoseconds(timeoutNs), cond_pred);
        অন্যথা
        {
            cond_var.wait(lock, cond_pred);
            CV_Assert(has_result);
            প্রত্যাগম সত্য;
        }
    }

    AsyncArray getArrayResult()
    {
        CV_Assert(refcount_future == 0);
        AsyncArray result;
        addrefFuture();
        result.p = এতত্;
        future_is_returned = সত্য;
        প্রত্যাগম result;
    }

    শূন্য set_মান(প্রবেশ_Array value)
    {
        যদি (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        std::unique_lock<std::mutex> lock(mtx);
        CV_Assert(!has_result);
        পূর্ণাঙ্ক k = value.kind();
        যদি (k == প্রবেশ_Array_2::UMAT)
        {
            result_umat = makePtr<UMat>();
            value.copyTo(*result_umat.get());
        }
        অন্যথা
        {
            result_mat = makePtr<Mat>();
            value.copyTo(*result_mat.get());
        }
        has_result = সত্য;
        cond_var.notify_all();
    }

#যদি CV__EXCEPTION_PTR
    শূন্য setException(std::exception_ptr e)
    {
        যদি (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        std::unique_lock<std::mutex> lock(mtx);
        CV_Assert(!has_result);
        has_exception = সত্য;
        exception = e;
        has_result = সত্য;
        cond_var.notify_all();
    }
#endif

    শূন্য setException(ধ্রুব cv::Exception e)
    {
        যদি (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        std::unique_lock<std::mutex> lock(mtx);
        CV_Assert(!has_result);
        has_exception = সত্য;
        cv_exception = e;
        has_result = সত্য;
        cond_var.notify_all();
    }
};

}  // namespace

#অন্যথা  // OPENCV_DISABLE_THREAD_SUPPORT

নামাকাশ cv {

// no threading
সংরচনা AsyncArray::Impl
{
    পূর্ণাঙ্ক refcount;
    শূন্য addrefFuture() CV_NOEXCEPT { refcount_future++; refcount++; }
    শূন্য releaseFuture() CV_NOEXCEPT { refcount_future--; যদি (0 == --refcount) delete এতত্; }
    পূর্ণাঙ্ক refcount_future;
    শূন্য addrefPromise() CV_NOEXCEPT { refcount_promise++; refcount++; } \
    শূন্য releasePromise() CV_NOEXCEPT { refcount_promise--; যদি (0 == --refcount) delete এতত্; }
    পূর্ণাঙ্ক refcount_promise;

    mutable তার্কিক has_result; // Mat, UMat or exception

    mutable cv::Ptr<Mat> result_mat;
    mutable cv::Ptr<UMat> result_umat;


    তার্কিক has_exception;
#যদি CV__EXCEPTION_PTR
    std::exception_ptr exception;
#endif
    cv::Exception cv_exception;

    mutable তার্কিক result_is_fetched;

    তার্কিক future_is_returned;

    Impl()
        : refcount(1), refcount_future(0), refcount_promise(1)
        , has_result(অসত্য)
        , has_exception(অসত্য)
        , result_is_fetched(অসত্য)
        , future_is_returned(অসত্য)
    {
        // nothing
    }

    ~Impl()
    {
        যদি (has_result && !result_is_fetched)
        {
            CV_LOG_INFO(রিক্ত, "Asynchronous result has not been fetched");
        }
    }

    তার্কিক get(OutputArray dst, int64 timeoutNs) ধ্রুব
    {
        CV_Assert(!result_is_fetched);
        যদি (!has_result)
        {
            CV_UNUSED(timeoutNs);
            CV_Error(Error::StsError, "Result is not produced (unable to wait for result in OPENCV_DISABLE_THREAD_SUPPORT mode)");
        }
        যদি (!result_mat.empty())
        {
            dst.move(*result_mat.get());
            result_mat.release();
            result_is_fetched = সত্য;
            প্রত্যাগম সত্য;
        }
        যদি (!result_umat.empty())
        {
            dst.move(*result_umat.get());
            result_umat.release();
            result_is_fetched = সত্য;
            প্রত্যাগম সত্য;
        }
#যদি CV__EXCEPTION_PTR
        যদি (has_exception && exception)
        {
            result_is_fetched = সত্য;
            std::rethrow_exception(exception);
        }
#endif
        যদি (has_exception)
        {
            result_is_fetched = সত্য;
            ক্ষিপ cv_exception;
        }
        CV_Error(Error::StsInternal, "AsyncArray: invalid state of 'has_result = true'");
        প্রত্যাগম অসত্য;
    }

    তার্কিক valid() ধ্রুব CV_NOEXCEPT
    {
        যদি (result_is_fetched)
            প্রত্যাগম অসত্য;
        যদি (refcount_promise == 0 && !has_result)
            প্রত্যাগম অসত্য;
        প্রত্যাগম সত্য;
    }

    তার্কিক wait_for(int64 timeoutNs) ধ্রুব
    {
        CV_Assert(valid());
        যদি (has_result)
            প্রত্যাগম has_result;
        যদি (timeoutNs == 0)
            প্রত্যাগম has_result;
        CV_Error(Error::StsError, "Unable to wait in OPENCV_DISABLE_THREAD_SUPPORT mode");
    }

    AsyncArray getArrayResult()
    {
        CV_Assert(refcount_future == 0);
        AsyncArray result;
        addrefFuture();
        result.p = এতত্;
        future_is_returned = সত্য;
        প্রত্যাগম result;
    }

    শূন্য set_মান(প্রবেশ_Array value)
    {
        যদি (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        CV_Assert(!has_result);
        পূর্ণাঙ্ক k = value.kind();
        যদি (k == প্রবেশ_Array_2::UMAT)
        {
            result_umat = makePtr<UMat>();
            value.copyTo(*result_umat.get());
        }
        অন্যথা
        {
            result_mat = makePtr<Mat>();
            value.copyTo(*result_mat.get());
        }
        has_result = সত্য;
    }

#যদি CV__EXCEPTION_PTR
    শূন্য setException(std::exception_ptr e)
    {
        যদি (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        CV_Assert(!has_result);
        has_exception = সত্য;
        exception = e;
        has_result = সত্য;
    }
#endif

    শূন্য setException(ধ্রুব cv::Exception e)
    {
        যদি (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        CV_Assert(!has_result);
        has_exception = সত্য;
        cv_exception = e;
        has_result = সত্য;
    }
};

}

#endif  // OPENCV_DISABLE_THREAD_SUPPORT

নামাকাশ cv {

AsyncArray::AsyncArray() CV_NOEXCEPT
    : p(রিক্ত)
{
}

AsyncArray::~AsyncArray() CV_NOEXCEPT
{
    release();
}

AsyncArray::AsyncArray(ধ্রুব AsyncArray& o) CV_NOEXCEPT
    : p(o.p)
{
    যদি (p)
        p->addrefFuture();
}

AsyncArray& AsyncArray::operator=(ধ্রুব AsyncArray& o) CV_NOEXCEPT
{
    Impl* newp = o.p;
    যদি (newp)
        newp->addrefFuture();
    release();
    p = newp;
    প্রত্যাগম *এতত্;
}

শূন্য AsyncArray::release() CV_NOEXCEPT
{
    Impl* impl = p;
    p = রিক্ত;
    যদি (impl)
        impl->releaseFuture();
}

তার্কিক AsyncArray::get(OutputArray dst, int64 timeoutNs) ধ্রুব
{
    CV_Assert(p);
    প্রত্যাগম p->get(dst, timeoutNs);
}

শূন্য AsyncArray::get(OutputArray dst) ধ্রুব
{
    CV_Assert(p);
    তার্কিক res = p->get(dst, -1);
    CV_Assert(res);
}

তার্কিক AsyncArray::wait_for(int64 timeoutNs) ধ্রুব
{
    CV_Assert(p);
    প্রত্যাগম p->wait_for(timeoutNs);
}

তার্কিক AsyncArray::valid() ধ্রুব CV_NOEXCEPT
{
    যদি (!p) প্রত্যাগম অসত্য;
    প্রত্যাগম p->valid();
}


//
// AsyncPromise
//

AsyncPromise::AsyncPromise() CV_NOEXCEPT
    : p(নূতন AsyncArray::Impl())
{
}

AsyncPromise::~AsyncPromise() CV_NOEXCEPT
{
    release();
}

AsyncPromise::AsyncPromise(ধ্রুব AsyncPromise& o) CV_NOEXCEPT
    : p(o.p)
{
    যদি (p)
        p->addrefPromise();
}

AsyncPromise& AsyncPromise::operator=(ধ্রুব AsyncPromise& o) CV_NOEXCEPT
{
    Impl* newp = o.p;
    যদি (newp)
        newp->addrefPromise();
    release();
    p = newp;
    প্রত্যাগম *এতত্;
}

শূন্য AsyncPromise::release() CV_NOEXCEPT
{
    Impl* impl = p;
    p = রিক্ত;
    যদি (impl)
        impl->releasePromise();
}

AsyncArray AsyncPromise::getArrayResult()
{
    CV_Assert(p);
    প্রত্যাগম p->getArrayResult();
}

শূন্য AsyncPromise::set_মান(প্রবেশ_Array value)
{
    CV_Assert(p);
    প্রত্যাগম p->set_মান(value);
}

শূন্য AsyncPromise::setException(ধ্রুব cv::Exception& exception)
{
    CV_Assert(p);
    প্রত্যাগম p->setException(exception);
}

#যদি CV__EXCEPTION_PTR
শূন্য AsyncPromise::setException(std::exception_ptr exception)
{
    CV_Assert(p);
    প্রত্যাগম p->setException(exception);
}
#endif

} // namespace
