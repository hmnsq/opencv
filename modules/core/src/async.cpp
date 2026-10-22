// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#అన్తర్భావ "precomp.hpp"
#అన్తర్భావ "opencv2/core/async.hpp"
#అన్తర్భావ "opencv2/core/detail/async_promise.hpp"

#అన్తర్భావ "opencv2/core/cvstd.hpp"

#అన్తర్భావ <opencv2/core/utils/logger.defines.hpp>
#నిర్వచనం_రద్దు CV_LOG_STRIP_LEVEL
#నిర్వచించు CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_DEBUG + 1
#అన్తర్భావ <opencv2/core/utils/logger.hpp>

#నిర్వచితం_కాకపోతే OPENCV_DISABLE_THREAD_SUPPORT

#అన్తర్భావ <mutex>
#అన్తర్భావ <condition_variable>
#అన్తర్భావ <chrono>

నామాకాశ cv {

/**
Manages shared state of asynchronous result
*/
నిర్మాణం AsyncArray::Impl
{
    పూర్ణాంకం refcount;
    శూన్య addrefFuture() CV_NOEXCEPT { CV_XADD(&refcount_future, 1); CV_XADD(&refcount, 1); } \
    శూన్య releaseFuture() CV_NOEXCEPT { CV_XADD(&refcount_future, -1); ఒకవేళ(1 == CV_XADD(&refcount, -1)) తొలగించు ఇది; } \
    పూర్ణాంకం refcount_future;
    శూన్య addrefPromise() CV_NOEXCEPT { CV_XADD(&refcount_promise, 1); CV_XADD(&refcount, 1); } \
    శూన్య releasePromise() CV_NOEXCEPT { CV_XADD(&refcount_promise, -1); ఒకవేళ(1 == CV_XADD(&refcount, -1)) తొలగించు ఇది; } \
    పూర్ణాంకం refcount_promise;

    మార్చదగిన std::mutex mtx;
    మార్చదగిన std::condition_variable cond_var;

    మార్చదగిన తార్కిక has_ఫలితం; // Mat, UMat or exception

    మార్చదగిన cv::Ptr<Mat> ఫలితం_mat;
    మార్చదగిన cv::Ptr<UMat> ఫలితం_umat;


    తార్కిక has_exception;
#ఒకవేళ CV__EXCEPTION_PTR
    std::exception_ptr exception;
#అయితే_ముగింపు
    cv::Exception cv_exception;

    మార్చదగిన తార్కిక ఫలితం_is_fetched;

    తార్కిక future_is_returned;

    Impl()
        : refcount(1), refcount_future(0), refcount_promise(1)
        , has_ఫలితం(అబద్ధం)
        , has_exception(అబద్ధం)
        , ఫలితం_is_fetched(అబద్ధం)
        , future_is_returned(అబద్ధం)
    {
        // nothing
    }

    ~Impl()
    {
        ఒకవేళ (has_ఫలితం && !ఫలితం_is_fetched)
        {
            CV_LOG_INFO(ఖాళీ, "Asynchronous result has not been fetched");
        }
    }

    తార్కిక get(OutputArray dst, int64 timeoutNs) స్థిరరాశి
    {
        CV_Assert(!ఫలితం_is_fetched);
        ఒకవేళ (!has_ఫలితం)
        {
            ఒకవేళ(refcount_promise == 0)
                CV_Error(Error::Sts_అంతర్గత, "Asynchronous result producer has been destroyed");
            ఒకవేళ (!wait_for(timeoutNs))
                తిరిగి అబద్ధం;
        }
        std::unique_lock<std::mutex> lock(mtx);
        ఒకవేళ (has_ఫలితం)
        {
            ఒకవేళ (!ఫలితం_mat.empty())
            {
                dst.move(*ఫలితం_mat.get());
                ఫలితం_mat.release();
                ఫలితం_is_fetched = నిజం;
                తిరిగి నిజం;
            }
            ఒకవేళ (!ఫలితం_umat.empty())
            {
                dst.move(*ఫలితం_umat.get());
                ఫలితం_umat.release();
                ఫలితం_is_fetched = నిజం;
                తిరిగి నిజం;
            }
#ఒకవేళ CV__EXCEPTION_PTR
            ఒకవేళ (has_exception && exception)
            {
                ఫలితం_is_fetched = నిజం;
                std::rethrow_exception(exception);
            }
#అయితే_ముగింపు
            ఒకవేళ (has_exception)
            {
                ఫలితం_is_fetched = నిజం;
                విసురు cv_exception;
            }
            CV_Error(Error::Sts_అంతర్గత, "AsyncArray: invalid state of 'has_result = true'");
        }
        CV_Assert(!has_ఫలితం);
        CV_Assert(timeoutNs < 0);
        తిరిగి అబద్ధం;
    }

    తార్కిక valid() స్థిరరాశి CV_NOEXCEPT
    {
        ఒకవేళ (ఫలితం_is_fetched)
            తిరిగి అబద్ధం;
        ఒకవేళ (refcount_promise == 0 && !has_ఫలితం)
            తిరిగి అబద్ధం;
        తిరిగి నిజం;
    }

    తార్కిక wait_for(int64 timeoutNs) స్థిరరాశి
    {
        CV_Assert(valid());
        ఒకవేళ (has_ఫలితం)
            తిరిగి has_ఫలితం;
        ఒకవేళ (timeoutNs == 0)
            తిరిగి has_ఫలితం;
        CV_LOG_INFO(ఖాళీ, "Waiting for async result ...");
        std::unique_lock<std::mutex> lock(mtx);
        స్థిరరాశి స్వయంచాలక cond_pred = [&]{ తిరిగి has_ఫలితం == నిజం; };
        ఒకవేళ (timeoutNs > 0)
            తిరిగి cond_var.wait_for(lock, std::chrono::nanoseconds(timeoutNs), cond_pred);
        లేకపోతే
        {
            cond_var.wait(lock, cond_pred);
            CV_Assert(has_ఫలితం);
            తిరిగి నిజం;
        }
    }

    AsyncArray get_Array_ఫలితం()
    {
        CV_Assert(refcount_future == 0);
        AsyncArray ఫలితం;
        addrefFuture();
        ఫలితం.p = ఇది;
        future_is_returned = నిజం;
        తిరిగి ఫలితం;
    }

    శూన్య సమితి_విలువ(ఆదానం_Array value)
    {
        ఒకవేళ (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        std::unique_lock<std::mutex> lock(mtx);
        CV_Assert(!has_ఫలితం);
        పూర్ణాంకం k = value.రకం();
        ఒకవేళ (k == ఆదానం_Array_2::UMAT)
        {
            ఫలితం_umat = makePtr<UMat>();
            value.copy_గమ్యం(*ఫలితం_umat.get());
        }
        లేకపోతే
        {
            ఫలితం_mat = makePtr<Mat>();
            value.copy_గమ్యం(*ఫలితం_mat.get());
        }
        has_ఫలితం = నిజం;
        cond_var.notify_all();
    }

#ఒకవేళ CV__EXCEPTION_PTR
    శూన్య సమితి_Exception(std::exception_ptr e)
    {
        ఒకవేళ (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        std::unique_lock<std::mutex> lock(mtx);
        CV_Assert(!has_ఫలితం);
        has_exception = నిజం;
        exception = e;
        has_ఫలితం = నిజం;
        cond_var.notify_all();
    }
#అయితే_ముగింపు

    శూన్య సమితి_Exception(స్థిరరాశి cv::Exception e)
    {
        ఒకవేళ (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        std::unique_lock<std::mutex> lock(mtx);
        CV_Assert(!has_ఫలితం);
        has_exception = నిజం;
        cv_exception = e;
        has_ఫలితం = నిజం;
        cond_var.notify_all();
    }
};

}  // namespace

#లేకపోతే  // OPENCV_DISABLE_THREAD_SUPPORT

నామాకాశ cv {

// no threading
నిర్మాణం AsyncArray::Impl
{
    పూర్ణాంకం refcount;
    శూన్య addrefFuture() CV_NOEXCEPT { refcount_future++; refcount++; }
    శూన్య releaseFuture() CV_NOEXCEPT { refcount_future--; ఒకవేళ (0 == --refcount) తొలగించు ఇది; }
    పూర్ణాంకం refcount_future;
    శూన్య addrefPromise() CV_NOEXCEPT { refcount_promise++; refcount++; } \
    శూన్య releasePromise() CV_NOEXCEPT { refcount_promise--; ఒకవేళ (0 == --refcount) తొలగించు ఇది; }
    పూర్ణాంకం refcount_promise;

    మార్చదగిన తార్కిక has_ఫలితం; // Mat, UMat or exception

    మార్చదగిన cv::Ptr<Mat> ఫలితం_mat;
    మార్చదగిన cv::Ptr<UMat> ఫలితం_umat;


    తార్కిక has_exception;
#ఒకవేళ CV__EXCEPTION_PTR
    std::exception_ptr exception;
#అయితే_ముగింపు
    cv::Exception cv_exception;

    మార్చదగిన తార్కిక ఫలితం_is_fetched;

    తార్కిక future_is_returned;

    Impl()
        : refcount(1), refcount_future(0), refcount_promise(1)
        , has_ఫలితం(అబద్ధం)
        , has_exception(అబద్ధం)
        , ఫలితం_is_fetched(అబద్ధం)
        , future_is_returned(అబద్ధం)
    {
        // nothing
    }

    ~Impl()
    {
        ఒకవేళ (has_ఫలితం && !ఫలితం_is_fetched)
        {
            CV_LOG_INFO(ఖాళీ, "Asynchronous result has not been fetched");
        }
    }

    తార్కిక get(OutputArray dst, int64 timeoutNs) స్థిరరాశి
    {
        CV_Assert(!ఫలితం_is_fetched);
        ఒకవేళ (!has_ఫలితం)
        {
            CV_వాడని(timeoutNs);
            CV_Error(Error::StsError, "Result is not produced (unable to wait for result in OPENCV_DISABLE_THREAD_SUPPORT mode)");
        }
        ఒకవేళ (!ఫలితం_mat.empty())
        {
            dst.move(*ఫలితం_mat.get());
            ఫలితం_mat.release();
            ఫలితం_is_fetched = నిజం;
            తిరిగి నిజం;
        }
        ఒకవేళ (!ఫలితం_umat.empty())
        {
            dst.move(*ఫలితం_umat.get());
            ఫలితం_umat.release();
            ఫలితం_is_fetched = నిజం;
            తిరిగి నిజం;
        }
#ఒకవేళ CV__EXCEPTION_PTR
        ఒకవేళ (has_exception && exception)
        {
            ఫలితం_is_fetched = నిజం;
            std::rethrow_exception(exception);
        }
#అయితే_ముగింపు
        ఒకవేళ (has_exception)
        {
            ఫలితం_is_fetched = నిజం;
            విసురు cv_exception;
        }
        CV_Error(Error::Sts_అంతర్గత, "AsyncArray: invalid state of 'has_result = true'");
        తిరిగి అబద్ధం;
    }

    తార్కిక valid() స్థిరరాశి CV_NOEXCEPT
    {
        ఒకవేళ (ఫలితం_is_fetched)
            తిరిగి అబద్ధం;
        ఒకవేళ (refcount_promise == 0 && !has_ఫలితం)
            తిరిగి అబద్ధం;
        తిరిగి నిజం;
    }

    తార్కిక wait_for(int64 timeoutNs) స్థిరరాశి
    {
        CV_Assert(valid());
        ఒకవేళ (has_ఫలితం)
            తిరిగి has_ఫలితం;
        ఒకవేళ (timeoutNs == 0)
            తిరిగి has_ఫలితం;
        CV_Error(Error::StsError, "Unable to wait in OPENCV_DISABLE_THREAD_SUPPORT mode");
    }

    AsyncArray get_Array_ఫలితం()
    {
        CV_Assert(refcount_future == 0);
        AsyncArray ఫలితం;
        addrefFuture();
        ఫలితం.p = ఇది;
        future_is_returned = నిజం;
        తిరిగి ఫలితం;
    }

    శూన్య సమితి_విలువ(ఆదానం_Array value)
    {
        ఒకవేళ (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        CV_Assert(!has_ఫలితం);
        పూర్ణాంకం k = value.రకం();
        ఒకవేళ (k == ఆదానం_Array_2::UMAT)
        {
            ఫలితం_umat = makePtr<UMat>();
            value.copy_గమ్యం(*ఫలితం_umat.get());
        }
        లేకపోతే
        {
            ఫలితం_mat = makePtr<Mat>();
            value.copy_గమ్యం(*ఫలితం_mat.get());
        }
        has_ఫలితం = నిజం;
    }

#ఒకవేళ CV__EXCEPTION_PTR
    శూన్య సమితి_Exception(std::exception_ptr e)
    {
        ఒకవేళ (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        CV_Assert(!has_ఫలితం);
        has_exception = నిజం;
        exception = e;
        has_ఫలితం = నిజం;
    }
#అయితే_ముగింపు

    శూన్య సమితి_Exception(స్థిరరాశి cv::Exception e)
    {
        ఒకవేళ (future_is_returned && refcount_future == 0)
            CV_Error(Error::StsError, "Associated AsyncArray has been destroyed");
        CV_Assert(!has_ఫలితం);
        has_exception = నిజం;
        cv_exception = e;
        has_ఫలితం = నిజం;
    }
};

}

#అయితే_ముగింపు  // OPENCV_DISABLE_THREAD_SUPPORT

నామాకాశ cv {

AsyncArray::AsyncArray() CV_NOEXCEPT
    : p(ఖాళీ)
{
}

AsyncArray::~AsyncArray() CV_NOEXCEPT
{
    release();
}

AsyncArray::AsyncArray(స్థిరరాశి AsyncArray& o) CV_NOEXCEPT
    : p(o.p)
{
    ఒకవేళ (p)
        p->addrefFuture();
}

AsyncArray& AsyncArray::క్రియాకారకం=(స్థిరరాశి AsyncArray& o) CV_NOEXCEPT
{
    Impl* newp = o.p;
    ఒకవేళ (newp)
        newp->addrefFuture();
    release();
    p = newp;
    తిరిగి *ఇది;
}

శూన్య AsyncArray::release() CV_NOEXCEPT
{
    Impl* impl = p;
    p = ఖాళీ;
    ఒకవేళ (impl)
        impl->releaseFuture();
}

తార్కిక AsyncArray::get(OutputArray dst, int64 timeoutNs) స్థిరరాశి
{
    CV_Assert(p);
    తిరిగి p->get(dst, timeoutNs);
}

శూన్య AsyncArray::get(OutputArray dst) స్థిరరాశి
{
    CV_Assert(p);
    తార్కిక res = p->get(dst, -1);
    CV_Assert(res);
}

తార్కిక AsyncArray::wait_for(int64 timeoutNs) స్థిరరాశి
{
    CV_Assert(p);
    తిరిగి p->wait_for(timeoutNs);
}

తార్కిక AsyncArray::valid() స్థిరరాశి CV_NOEXCEPT
{
    ఒకవేళ (!p) తిరిగి అబద్ధం;
    తిరిగి p->valid();
}


//
// AsyncPromise
//

AsyncPromise::AsyncPromise() CV_NOEXCEPT
    : p(కొత్త AsyncArray::Impl())
{
}

AsyncPromise::~AsyncPromise() CV_NOEXCEPT
{
    release();
}

AsyncPromise::AsyncPromise(స్థిరరాశి AsyncPromise& o) CV_NOEXCEPT
    : p(o.p)
{
    ఒకవేళ (p)
        p->addrefPromise();
}

AsyncPromise& AsyncPromise::క్రియాకారకం=(స్థిరరాశి AsyncPromise& o) CV_NOEXCEPT
{
    Impl* newp = o.p;
    ఒకవేళ (newp)
        newp->addrefPromise();
    release();
    p = newp;
    తిరిగి *ఇది;
}

శూన్య AsyncPromise::release() CV_NOEXCEPT
{
    Impl* impl = p;
    p = ఖాళీ;
    ఒకవేళ (impl)
        impl->releasePromise();
}

AsyncArray AsyncPromise::get_Array_ఫలితం()
{
    CV_Assert(p);
    తిరిగి p->get_Array_ఫలితం();
}

శూన్య AsyncPromise::సమితి_విలువ(ఆదానం_Array value)
{
    CV_Assert(p);
    తిరిగి p->సమితి_విలువ(value);
}

శూన్య AsyncPromise::సమితి_Exception(స్థిరరాశి cv::Exception& exception)
{
    CV_Assert(p);
    తిరిగి p->సమితి_Exception(exception);
}

#ఒకవేళ CV__EXCEPTION_PTR
శూన్య AsyncPromise::సమితి_Exception(std::exception_ptr exception)
{
    CV_Assert(p);
    తిరిగి p->సమితి_Exception(exception);
}
#అయితే_ముగింపు

} // namespace
