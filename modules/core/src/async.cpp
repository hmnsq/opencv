// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ضمن "precomp.hpp"
#ضمن "opencv2/core/async.hpp"
#ضمن "opencv2/core/detail/async_promise.hpp"

#ضمن "opencv2/core/cvstd.hpp"

#ضمن <opencv2/core/utils/logger.defines.hpp>
#تعریف_لغوه CV_LOG_STRIP_LEVEL
#تعریف_کړه CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_DEBUG + 1
#ضمن <opencv2/core/utils/logger.hpp>

#که_نه_تعریف_شوی OPENCV_DISABLE_THREAD_SUPPORT

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
    فراغ releaseFuture() CV_NOEXCEPT { CV_XADD(&refcount_future, -1); إذا(1 == CV_XADD(&refcount, -1)) ړنګول هذا; } \
    صحيح refcount_future;
    فراغ addrefPromise() CV_NOEXCEPT { CV_XADD(&refcount_promise, 1); CV_XADD(&refcount, 1); } \
    فراغ releasePromise() CV_NOEXCEPT { CV_XADD(&refcount_promise, -1); إذا(1 == CV_XADD(&refcount, -1)) ړنګول هذا; } \
    صحيح refcount_promise;

    بدلیدونکی std::mutex mtx;
    بدلیدونکی std::condition_variable cond_var;

    بدلیدونکی منطقي has_پایله; // Mat, UMat or exception

    بدلیدونکی cv::Ptr<Mat> پایله_mat;
    بدلیدونکی cv::Ptr<UMat> پایله_umat;


    منطقي has_exception;
#إذا CV__EXCEPTION_PTR
    std::exception_ptr exception;
#د_که_پای
    cv::Exception cv_exception;

    بدلیدونکی منطقي پایله_is_fetched;

    منطقي future_is_returned;

    Impl()
        : refcount(1), refcount_future(0), refcount_promise(1)
        , has_پایله(خطأ)
        , has_exception(خطأ)
        , پایله_is_fetched(خطأ)
        , future_is_returned(خطأ)
    {
        // nothing
    }

    ~Impl()
    {
        إذا (has_پایله && !پایله_is_fetched)
        {
            CV_LOG_INFO(عدم, "Asynchronous result has not been fetched");
        }
    }

    منطقي get(OutputArray dst, int64 timeoutNs) ثابت
    {
        CV_Assert(!پایله_is_fetched);
        إذا (!has_پایله)
        {
            إذا(refcount_promise == 0)
                CV_Error(Error::Sts_دننی, "Asynchronous result producer has been destroyed");
            إذا (!wait_for(timeoutNs))
                أرجع خطأ;
        }
        std::unique_lock<std::mutex> lock(mtx);
        إذا (has_پایله)
        {
            إذا (!پایله_mat.empty())
            {
                dst.move(*پایله_mat.get());
                پایله_mat.release();
                پایله_is_fetched = ریښتیا;
                أرجع ریښتیا;
            }
            إذا (!پایله_umat.empty())
            {
                dst.move(*پایله_umat.get());
                پایله_umat.release();
                پایله_is_fetched = ریښتیا;
                أرجع ریښتیا;
            }
#إذا CV__EXCEPTION_PTR
            إذا (has_exception && exception)
            {
                پایله_is_fetched = ریښتیا;
                std::rethrow_exception(exception);
            }
#د_که_پای
            إذا (has_exception)
            {
                پایله_is_fetched = ریښتیا;
                ارم cv_exception;
            }
            CV_Error(Error::Sts_دننی, "AsyncArray: invalid state of 'has_result = true'");
        }
        CV_Assert(!has_پایله);
        CV_Assert(timeoutNs < 0);
        أرجع خطأ;
    }

    منطقي valid() ثابت CV_NOEXCEPT
    {
        إذا (پایله_is_fetched)
            أرجع خطأ;
        إذا (refcount_promise == 0 && !has_پایله)
            أرجع خطأ;
        أرجع ریښتیا;
    }

    منطقي wait_for(int64 timeoutNs) ثابت
    {
        CV_Assert(valid());
        إذا (has_پایله)
            أرجع has_پایله;
        إذا (timeoutNs == 0)
            أرجع has_پایله;
        CV_LOG_INFO(عدم, "Waiting for async result ...");
        std::unique_lock<std::mutex> lock(mtx);
        ثابت خپلکاری cond_pred = [&]{ أرجع has_پایله == ریښتیا; };
        إذا (timeoutNs > 0)
            أرجع cond_var.wait_for(lock, std::chrono::nanoseconds(timeoutNs), cond_pred);
        وإلا
        {
            cond_var.wait(lock, cond_pred);
            CV_Assert(has_پایله);
            أرجع ریښتیا;
        }
    }

    AsyncArray get_Array_پایله()
    {
        CV_Assert(refcount_future == 0);
        AsyncArray پایله;
        addrefFuture();
        پایله.p = هذا;
        future_is_returned = ریښتیا;
        أرجع پایله;
    }

    فراغ سېټ_ارزښت(ننوت_Array value)
    {
        إذا (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        std::unique_lock<std::mutex> lock(mtx);
        CV_Assert(!has_پایله);
        صحيح k = value.ډول();
        إذا (k == ننوت_Array_2::UMAT)
        {
            پایله_umat = makePtr<UMat>();
            value.copy_منزل(*پایله_umat.get());
        }
        وإلا
        {
            پایله_mat = makePtr<Mat>();
            value.copy_منزل(*پایله_mat.get());
        }
        has_پایله = ریښتیا;
        cond_var.notify_all();
    }

#إذا CV__EXCEPTION_PTR
    فراغ سېټ_Exception(std::exception_ptr e)
    {
        إذا (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        std::unique_lock<std::mutex> lock(mtx);
        CV_Assert(!has_پایله);
        has_exception = ریښتیا;
        exception = e;
        has_پایله = ریښتیا;
        cond_var.notify_all();
    }
#د_که_پای

    فراغ سېټ_Exception(ثابت cv::Exception e)
    {
        إذا (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        std::unique_lock<std::mutex> lock(mtx);
        CV_Assert(!has_پایله);
        has_exception = ریښتیا;
        cv_exception = e;
        has_پایله = ریښتیا;
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
    فراغ releaseFuture() CV_NOEXCEPT { refcount_future--; إذا (0 == --refcount) ړنګول هذا; }
    صحيح refcount_future;
    فراغ addrefPromise() CV_NOEXCEPT { refcount_promise++; refcount++; } \
    فراغ releasePromise() CV_NOEXCEPT { refcount_promise--; إذا (0 == --refcount) ړنګول هذا; }
    صحيح refcount_promise;

    بدلیدونکی منطقي has_پایله; // Mat, UMat or exception

    بدلیدونکی cv::Ptr<Mat> پایله_mat;
    بدلیدونکی cv::Ptr<UMat> پایله_umat;


    منطقي has_exception;
#إذا CV__EXCEPTION_PTR
    std::exception_ptr exception;
#د_که_پای
    cv::Exception cv_exception;

    بدلیدونکی منطقي پایله_is_fetched;

    منطقي future_is_returned;

    Impl()
        : refcount(1), refcount_future(0), refcount_promise(1)
        , has_پایله(خطأ)
        , has_exception(خطأ)
        , پایله_is_fetched(خطأ)
        , future_is_returned(خطأ)
    {
        // nothing
    }

    ~Impl()
    {
        إذا (has_پایله && !پایله_is_fetched)
        {
            CV_LOG_INFO(عدم, "Asynchronous result has not been fetched");
        }
    }

    منطقي get(OutputArray dst, int64 timeoutNs) ثابت
    {
        CV_Assert(!پایله_is_fetched);
        إذا (!has_پایله)
        {
            CV_ناکارول_شوی(timeoutNs);
            CV_Error(Error::StsError, "Result is not produced (unable to wait for result in OPENCV_DISABLE_THREAD_SUPPORT mode)");
        }
        إذا (!پایله_mat.empty())
        {
            dst.move(*پایله_mat.get());
            پایله_mat.release();
            پایله_is_fetched = ریښتیا;
            أرجع ریښتیا;
        }
        إذا (!پایله_umat.empty())
        {
            dst.move(*پایله_umat.get());
            پایله_umat.release();
            پایله_is_fetched = ریښتیا;
            أرجع ریښتیا;
        }
#إذا CV__EXCEPTION_PTR
        إذا (has_exception && exception)
        {
            پایله_is_fetched = ریښتیا;
            std::rethrow_exception(exception);
        }
#د_که_پای
        إذا (has_exception)
        {
            پایله_is_fetched = ریښتیا;
            ارم cv_exception;
        }
        CV_Error(Error::Sts_دننی, "AsyncArray: invalid state of 'has_result = true'");
        أرجع خطأ;
    }

    منطقي valid() ثابت CV_NOEXCEPT
    {
        إذا (پایله_is_fetched)
            أرجع خطأ;
        إذا (refcount_promise == 0 && !has_پایله)
            أرجع خطأ;
        أرجع ریښتیا;
    }

    منطقي wait_for(int64 timeoutNs) ثابت
    {
        CV_Assert(valid());
        إذا (has_پایله)
            أرجع has_پایله;
        إذا (timeoutNs == 0)
            أرجع has_پایله;
        CV_Error(Error::StsError, "Unable to wait in OPENCV_DISABLE_THREAD_SUPPORT mode");
    }

    AsyncArray get_Array_پایله()
    {
        CV_Assert(refcount_future == 0);
        AsyncArray پایله;
        addrefFuture();
        پایله.p = هذا;
        future_is_returned = ریښتیا;
        أرجع پایله;
    }

    فراغ سېټ_ارزښت(ننوت_Array value)
    {
        إذا (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        CV_Assert(!has_پایله);
        صحيح k = value.ډول();
        إذا (k == ننوت_Array_2::UMAT)
        {
            پایله_umat = makePtr<UMat>();
            value.copy_منزل(*پایله_umat.get());
        }
        وإلا
        {
            پایله_mat = makePtr<Mat>();
            value.copy_منزل(*پایله_mat.get());
        }
        has_پایله = ریښتیا;
    }

#إذا CV__EXCEPTION_PTR
    فراغ سېټ_Exception(std::exception_ptr e)
    {
        إذا (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        CV_Assert(!has_پایله);
        has_exception = ریښتیا;
        exception = e;
        has_پایله = ریښتیا;
    }
#د_که_پای

    فراغ سېټ_Exception(ثابت cv::Exception e)
    {
        إذا (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        CV_Assert(!has_پایله);
        has_exception = ریښتیا;
        cv_exception = e;
        has_پایله = ریښتیا;
    }
};

}

#د_که_پای  // OPENCV_DISABLE_THREAD_SUPPORT

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
    إذا (p)
        p->addrefFuture();
}

AsyncArray& AsyncArray::چلوونکی=(ثابت AsyncArray& o) CV_NOEXCEPT
{
    Impl* newp = o.p;
    إذا (newp)
        newp->addrefFuture();
    release();
    p = newp;
    أرجع *هذا;
}

فراغ AsyncArray::release() CV_NOEXCEPT
{
    Impl* impl = p;
    p = عدم;
    إذا (impl)
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
    إذا (!p) أرجع خطأ;
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
    إذا (p)
        p->addrefPromise();
}

AsyncPromise& AsyncPromise::چلوونکی=(ثابت AsyncPromise& o) CV_NOEXCEPT
{
    Impl* newp = o.p;
    إذا (newp)
        newp->addrefPromise();
    release();
    p = newp;
    أرجع *هذا;
}

فراغ AsyncPromise::release() CV_NOEXCEPT
{
    Impl* impl = p;
    p = عدم;
    إذا (impl)
        impl->releasePromise();
}

AsyncArray AsyncPromise::get_Array_پایله()
{
    CV_Assert(p);
    أرجع p->get_Array_پایله();
}

فراغ AsyncPromise::سېټ_ارزښت(ننوت_Array value)
{
    CV_Assert(p);
    أرجع p->سېټ_ارزښت(value);
}

فراغ AsyncPromise::سېټ_Exception(ثابت cv::Exception& exception)
{
    CV_Assert(p);
    أرجع p->سېټ_Exception(exception);
}

#إذا CV__EXCEPTION_PTR
فراغ AsyncPromise::سېټ_Exception(std::exception_ptr exception)
{
    CV_Assert(p);
    أرجع p->سېټ_Exception(exception);
}
#د_که_پای

} // namespace
