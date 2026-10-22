/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                           License Agreement
//                For Open Source Computer Vision Library
//
// Copyright (C) 2014, Itseez Inc., all rights reserved.
// Third party copyrights are property of their respective owners.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//   * Redistribution's of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//   * Redistribution's in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
//   * The name of the copyright holders may not be used to endorse or promote products
//     derived from this software without specific prior written permission.
//
// This software is provided by the copyright holders and contributors "as is" and
// any express or implied warranties, including, but not limited to, the implied
// warranties of merchantability and fitness for a particular purpose are disclaimed.
// In no event shall the Intel Corporation or contributors be liable for any direct,
// indirect, incidental, special, exemplary, or consequential damages
// (including, but not limited to, procurement of substitute goods or services;
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//
//M*/

#ضمن "precomp.hpp"
#ضمن "opencl_kernels_core.hpp"
#ضمن "umatrix.hpp"

#ضمن <opencv2/core/utils/tls.hpp>

///////////////////////////////// UMat implementation ///////////////////////////////

نطاق cv {

// forward decls, implementation is below in this file
فراغ سېټ_اندازه(UMat& m, صحيح _dims, ثابت صحيح* _sz, ثابت size_t* ګامونه_2,
             منطقي auto_ګامونه = خطأ);

فراغ updateContinuityFlag(UMat& m);
فراغ finalizeHdr(UMat& m);

UMat_معلومات::UMat_معلومات(ثابت MatAllocator* allocator)
{
    prevAllocator = currAllocator = allocator;
    urefcount = refcount = mapcount = 0;
    معلومات = origdata = 0;
    size = 0;
    flags = static_cast<UMat_معلومات::حافظه_Flag>(0);
    handle = 0;
    userdata = 0;
    allocatorFlags_ = 0;
    original_UMat_معلومات = عدم;
}

UMat_معلومات::~UMat_معلومات()
{
    prevAllocator = currAllocator = 0;
    urefcount = refcount = 0;
    CV_Assert(mapcount == 0);
    معلومات = origdata = 0;
    size = 0;
    منطقي isAsyncCleanup = !!(flags & UMat_معلومات::ASYNC_CLEANUP);
    flags = static_cast<UMat_معلومات::حافظه_Flag>(0);
    handle = 0;
    userdata = 0;
    allocatorFlags_ = 0;
    إذا (original_UMat_معلومات)
    {
        منطقي showWarn = خطأ;
        UMat_معلومات* u = original_UMat_معلومات;
        منطقي zero_Ref = CV_XADD(&(u->refcount), -1) == 1;
        إذا (zero_Ref)
        {
            // simulate Mat::deallocate
            إذا (u->mapcount != 0)
            {
                (u->currAllocator ? u->currAllocator : /* TODO allocator ? allocator :*/ Mat::getDefaultAllocator())->unmap(u);
            }
            وإلا
            {
                // we don't do "map", so we can't do "unmap"
            }
        }
        منطقي zero_URef = CV_XADD(&(u->urefcount), -1) == 1;
        إذا (zero_Ref && !zero_URef)
            showWarn = ریښتیا;
        إذا (zero_Ref && zero_URef) // oops, we need to free resources
        {
            showWarn = !isAsyncCleanup;
            // simulate UMat::deallocate
            u->currAllocator->deallocate(u);
        }
#که_نه_تعریف_شوی NDEBUG
        إذا (showWarn)
        {
            ساكن صحيح warn_پیغام_showed = 0;
            إذا (warn_پیغام_showed++ < 100)
            {
                fflush(stdout);
                fprintf(stderr, "\n! OPENCV warning: getUMat()/getMat() call chain possible problem."
                                "\n!                 Base object is dead, while nested/derived object is still alive or processed."
                                "\n!                 Please check lifetime of UMat/Mat objects!\n");
                fflush(stderr);
            }
        }
#وإلا
        CV_ناکارول_شوی(showWarn);
#د_که_پای
        original_UMat_معلومات = عدم;
    }
}

#که_نه_تعریف_شوی OPENCV_DISABLE_THREAD_SUPPORT

// it should be a prime number for the best hash function
تعداد { UMAT_NLOCKS = 31 };
ساكن Mutex umatLocks[UMAT_NLOCKS];

ساكن size_t get_UMat_معلومات_قلف_شاخص(ثابت UMat_معلومات* u)
{
    size_t idx = ((size_t)(فراغ*)u) % UMAT_NLOCKS;
    أرجع idx;
}

فراغ UMat_معلومات::lock()
{
    size_t idx = get_UMat_معلومات_قلف_شاخص(هذا);
    //printf("%d lock(%d)\n", cv::utils::getThreadID(), (int)idx);
    umatLocks[idx].lock();
}

فراغ UMat_معلومات::unlock()
{
    size_t idx = get_UMat_معلومات_قلف_شاخص(هذا);
    //printf("%d unlock(%d)\n", cv::utils::getThreadID(), (int)idx);
    umatLocks[idx].unlock();
}


// Do not allow several lock() calls with different UMatData objects.
بنية UMat_معلومات_Auto_Locker
{
    صحيح usage_شمېر;
    UMat_معلومات* locked_objects[2];
    UMat_معلومات_Auto_Locker() : usage_شمېر(0) { locked_objects[0] = عدم; locked_objects[1] = عدم; }

    فراغ lock(UMat_معلومات*& u1)
    {
        منطقي locked_1 = (u1 == locked_objects[0] || u1 == locked_objects[1]);
        إذا (locked_1)
        {
            u1 = عدم;
            أرجع;
        }
        CV_Assert(usage_شمېر == 0);  // UMatDataAutoLock can't be used multiple times from the same thread
        usage_شمېر = 1;
        locked_objects[0] = u1;
        u1->lock();
    }
    فراغ lock(UMat_معلومات*& u1, UMat_معلومات*& u2)
    {
        منطقي locked_1 = (u1 == locked_objects[0] || u1 == locked_objects[1]);
        منطقي locked_2 = (u2 == locked_objects[0] || u2 == locked_objects[1]);
        إذا (locked_1)
            u1 = عدم;
        إذا (locked_2)
            u2 = عدم;
        إذا (locked_1 && locked_2)
            أرجع;
        CV_Assert(usage_شمېر == 0);  // UMatDataAutoLock can't be used multiple times from the same thread
        usage_شمېر = 1;
        locked_objects[0] = u1;
        locked_objects[1] = u2;
        إذا (u1)
            u1->lock();
        إذا (u2)
            u2->lock();
    }
    فراغ release(UMat_معلومات* u1, UMat_معلومات* u2)
    {
        إذا (u1 == عدم && u2 == عدم)
            أرجع;
        CV_Assert(usage_شمېر == 1);
        usage_شمېر = 0;
        إذا (u1)
            u1->unlock();
        إذا (u2)
            u2->unlock();
        locked_objects[0] = عدم; locked_objects[1] = عدم;
    }
};
ساكن TLSData<UMat_معلومات_Auto_Locker>& get_UMat_معلومات_Auto_Locker_TLS()
{
    CV_SINGLETON_LAZY_INIT_REF(TLSData<UMat_معلومات_Auto_Locker>, جديد TLSData<UMat_معلومات_Auto_Locker>());
}
ساكن UMat_معلومات_Auto_Locker& get_UMat_معلومات_Auto_Locker() { أرجع get_UMat_معلومات_Auto_Locker_TLS().getRef(); }


UMat_معلومات_Auto_قلف::UMat_معلومات_Auto_قلف(UMat_معلومات* u) : u1(u), u2(عدم)
{
    get_UMat_معلومات_Auto_Locker().lock(u1);
}
UMat_معلومات_Auto_قلف::UMat_معلومات_Auto_قلف(UMat_معلومات* u1_, UMat_معلومات* u2_) : u1(u1_), u2(u2_)
{
    إذا (get_UMat_معلومات_قلف_شاخص(u1) > get_UMat_معلومات_قلف_شاخص(u2))
    {
        std::swap(u1, u2);
    }
    get_UMat_معلومات_Auto_Locker().lock(u1, u2);
}
UMat_معلومات_Auto_قلف::~UMat_معلومات_Auto_قلف()
{
    get_UMat_معلومات_Auto_Locker().release(u1, u2);
}

#وإلا

فراغ UMat_معلومات::lock()
{
    // nothing in OPENCV_DISABLE_THREAD_SUPPORT mode
}

فراغ UMat_معلومات::unlock()
{
    // nothing in OPENCV_DISABLE_THREAD_SUPPORT mode
}

UMat_معلومات_Auto_قلف::UMat_معلومات_Auto_قلف(UMat_معلومات* u) : u1(u), u2(عدم)
{
    // nothing in OPENCV_DISABLE_THREAD_SUPPORT mode
}
UMat_معلومات_Auto_قلف::UMat_معلومات_Auto_قلف(UMat_معلومات* u1_, UMat_معلومات* u2_) : u1(u1_), u2(u2_)
{
    // nothing in OPENCV_DISABLE_THREAD_SUPPORT mode
}
UMat_معلومات_Auto_قلف::~UMat_معلومات_Auto_قلف()
{
    // nothing in OPENCV_DISABLE_THREAD_SUPPORT mode
}

#د_که_پای  // OPENCV_DISABLE_THREAD_SUPPORT

//////////////////////////////// UMat ////////////////////////////////

UMat::UMat(UMatUsageFlags _usageFlags) CV_NOEXCEPT
: flags(MAGIC_VAL), dims(0), لیکې(0), ستون_شمېر(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{}

UMat::UMat(صحيح لیکې_3, صحيح ستون_شمېر_3, صحيح _type, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), لیکې(0), ستون_شمېر(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(لیکې_3, ستون_شمېر_3, _type);
}

UMat::UMat(صحيح لیکې_3, صحيح ستون_شمېر_3, صحيح _type, ثابت Scalar& _s, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), لیکې(0), ستون_شمېر(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(لیکې_3, ستون_شمېر_3, _type);
    *هذا = _s;
}

UMat::UMat(اندازه _sz, صحيح _type, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), لیکې(0), ستون_شمېر(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create( _sz.لوړوالی, _sz.پلنوالی, _type );
}

UMat::UMat(اندازه _sz, صحيح _type, ثابت Scalar& _s, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), لیکې(0), ستون_شمېر(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_sz.لوړوالی, _sz.پلنوالی, _type);
    *هذا = _s;
}

UMat::UMat(صحيح _dims, ثابت صحيح* _sz, صحيح _type, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), لیکې(0), ستون_شمېر(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_dims, _sz, _type);
}

UMat::UMat(صحيح _dims, ثابت صحيح* _sz, صحيح _type, ثابت Scalar& _s, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), لیکې(0), ستون_شمېر(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_dims, _sz, _type);
    *هذا = _s;
}

UMat::UMat(ثابت MatShape& _shape, صحيح _type, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), لیکې(0), ستون_شمېر(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_shape.dims, _shape.p, _type);
}

UMat::UMat(ثابت MatShape& _shape, صحيح _type, ثابت Scalar& _s, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), لیکې(0), ستون_شمېر(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_shape.dims, _shape.p, _type);
    *هذا = _s;
}

UMat::UMat(ثابت UMat& m)
: flags(m.flags), dims(m.dims), لیکې(m.لیکې), ستون_شمېر(m.ستون_شمېر), allocator(m.allocator),
  usageFlags(m.usageFlags), u(m.u), offset(m.offset), size(m.size), ګام(m.ګام)
{
    addref();
}

UMat& UMat::چلوونکی=(ثابت UMat& m)
{
    إذا( هذا != &m )
    {
        const_cast<UMat&>(m).addref();
        release();
        flags = m.flags;
        dims = m.dims;
        لیکې = m.لیکې;
        ستون_شمېر = m.ستون_شمېر;
        size = m.size;
        ګام = m.ګام;
        allocator = m.allocator;
        usageFlags = m.usageFlags;
        u = m.u;
        offset = m.offset;
    }
    أرجع *هذا;
}

UMat UMat::clone() ثابت
{
    UMat m;
    copy_منزل(m);
    أرجع m;
}

فراغ UMat::ټاکل_منزل(UMat& m, صحيح _type) ثابت
{
    إذا( _type < 0 )
        m = *هذا;
    وإلا
        convert_منزل(m, _type);
}

فراغ UMat::create(صحيح لیکې_3, صحيح ستون_شمېر_3, صحيح _type, UMatUsageFlags _usageFlags)
{
    صحيح sz[] = {لیکې_3, ستون_شمېر_3};
    create(2, sz, _type, _usageFlags);
}

فراغ UMat::create(اندازه _sz, صحيح _type, UMatUsageFlags _usageFlags)
{
    create(_sz.لوړوالی, _sz.پلنوالی, _type, _usageFlags);
}

فراغ UMat::create_Same_اندازه(ننوت_Array arr, صحيح _type, UMatUsageFlags _usageFlags)
{
    create(arr.shape(), _type, _usageFlags);
}

فراغ UMat::addref()
{
    إذا( u )
        CV_XADD(&(u->urefcount), 1);
}

فراغ UMat::release()
{
    إذا( u && CV_XADD(&(u->urefcount), -1) == 1 )
        deallocate();
    u = 0;
    size.clear();
    dims = ستون_شمېر = لیکې = 0;
}

منطقي UMat::empty() ثابت
{
    أرجع u == 0 || ټول() == 0;
}

size_t UMat::ټول() ثابت
{
    إذا( dims <= 2 )
        أرجع (size_t)لیکې * ستون_شمېر;
    size_t p = 1;
    لكل( صحيح i = 0; i < dims; i++ )
        p *= size[i];
    أرجع p;
}

MatShape UMat::shape() ثابت
{
    أرجع size;
}

UMat::UMat(UMat&& m)
: flags(m.flags), dims(m.dims), لیکې(m.لیکې), ستون_شمېر(m.ستون_شمېر), allocator(m.allocator),
  usageFlags(m.usageFlags), u(m.u), offset(m.offset)
{
    size = m.size;
    ګام = m.ګام;
    m.flags = MAGIC_VAL;
    m.usageFlags = USAGE_DEFAULT;
    m.dims = m.لیکې = m.ستون_شمېر = 0;
    m.size.clear();
    m.allocator = عدم;
    m.u = عدم;
    m.offset = 0;
}

UMat& UMat::چلوونکی=(UMat&& m)
{
    إذا (هذا == &m)
      أرجع *هذا;
    release();
    flags = m.flags; dims = m.dims; لیکې = m.لیکې; ستون_شمېر = m.ستون_شمېر;
    allocator = m.allocator; usageFlags = m.usageFlags;
    u = m.u;
    offset = m.offset;
    size = m.size;
    ګام = m.ګام;
    m.flags = MAGIC_VAL;
    m.usageFlags = USAGE_DEFAULT;
    m.dims = m.لیکې = m.ستون_شمېر = 0;
    m.allocator = عدم;
    m.u = عدم;
    m.offset = 0;
    أرجع *هذا;
}


MatAllocator* UMat::getStdAllocator()
{
#که_تعریف_شوی HAVE_OPENCL
    إذا (ocl::useOpenCL())
        أرجع ocl::getOpenCLAllocator();
#د_که_پای
    أرجع Mat::getDefaultAllocator();
}

فراغ swap( UMat& a, UMat& b )
{
    std::swap(a.flags, b.flags);
    std::swap(a.dims, b.dims);
    std::swap(a.لیکې, b.لیکې);
    std::swap(a.ستون_شمېر, b.ستون_شمېر);
    std::swap(a.allocator, b.allocator);
    std::swap(a.u, b.u);
    std::swap(a.offset, b.offset);

    std::swap(a.size, b.size);
    std::swap(a.ګام, b.ګام);
}


فراغ سېټ_اندازه( UMat& m, صحيح _dims, ثابت صحيح* _sz,
              ثابت size_t* ګامونه_2, منطقي auto_ګامونه )
{
    CV_Assert( 0 <= _dims && _dims <= CV_MAX_DIM && _dims <= MatShape::MAX_DIMS);

    m.dims = _dims;
    m.size = MatShape(_dims, _sz);
    m.ګام[std::max(_dims-1, 0)] = CV_ELEM_اندازه(m.flags);
    لكل (صحيح i = _dims-2; i >= 0; i--) {
        size_t autostep = m.size[i+1]*m.ګام[i+1];
        إذا (ګامونه_2) {
            m.ګام[i] = ګامونه_2[i];
            //CV_Assert(m.step[i] >= autostep);
        } وإلا إذا (auto_ګامونه) {
            m.ګام[i] = autostep;
        } وإلا {
            m.ګام[i] = 0;
        }
    }

    إذا( _dims <= 2 )
    {
        m.ستون_شمېر = _dims == 0 ? 1 : _sz ? _sz[_dims > 1] : 0;
        m.لیکې = _dims < 2 ? 1 : _sz ? _sz[0] : 0;
    } وإلا {
        m.ستون_شمېر = m.لیکې = -1;
    }
}


فراغ UMat::updateContinuityFlag()
{
    flags = cv::updateContinuityFlag(flags, dims, size.p, ګام.p);
}


فراغ finalizeHdr(UMat& m)
{
    m.updateContinuityFlag();
    صحيح d = m.dims;
    إذا( d > 2 )
        m.لیکې = m.ستون_شمېر = -1;
}


UMat Mat::getUMat(AccessFlag accessFlags, UMatUsageFlags usageFlags) ثابت
{
    UMat hdr;
    إذا(!معلومات)
        أرجع hdr;
    إذا (معلومات != datastart)
    {
        اندازه ټولیز_اندازه;
        ټکی ofs;
        locateROI(ټولیز_اندازه, ofs);
        اندازه sz(ستون_شمېر, لیکې);
        إذا (ofs.x != 0 || ofs.y != 0)
        {
            Mat src = *هذا;
            صحيح dtop = ofs.y;
            صحيح dbottom = ټولیز_اندازه.لوړوالی - src.لیکې - ofs.y;
            صحيح dleft = ofs.x;
            صحيح dright = ټولیز_اندازه.پلنوالی - src.ستون_شمېر - ofs.x;
            src.adjustROI(dtop, dbottom, dleft, dright);
            أرجع src.getUMat(accessFlags, usageFlags)(cv::Rect(ofs.x, ofs.y, sz.پلنوالی, sz.لوړوالی));
        }
    }
    CV_Assert(معلومات == datastart);

    accessFlags |= ACCESS_RW;
    UMat_معلومات* new_u = عدم;
    Mat_ګام new_ګام = ګام;
    {
        MatAllocator *a = allocator, *a0 = getDefaultAllocator();
        إذا(!a)
            a = a0;
        new_u = a->allocate(dims, size.p, type(), معلومات, new_ګام.p, accessFlags, usageFlags);
        new_u->original_UMat_معلومات = u;
    }
    منطقي allocated = خطأ;
    حاول
    {
        allocated = UMat::getStdAllocator()->allocate(new_u, accessFlags, usageFlags);
    }
    امسك (ثابت cv::Exception& e)
    {
        fprintf(stderr, "Exception: %s\n", e.what());
    }
    إذا (!allocated)
    {
        allocated = getDefaultAllocator()->allocate(new_u, accessFlags, usageFlags);
        CV_Assert(allocated);
    }
    إذا (u != عدم)
    {
#که_تعریف_شوی HAVE_OPENCL
        إذا (ocl::useOpenCL() && new_u->currAllocator == ocl::getOpenCLAllocator())
        {
            CV_Assert(new_u->لنډمهاله_UMat());
        }
#د_که_پای
        CV_XADD(&(u->refcount), 1);
        CV_XADD(&(u->urefcount), 1);
    }
    حاول
    {
        hdr.flags = flags;
        hdr.usageFlags = usageFlags;
        سېټ_اندازه(hdr, dims, size.p, new_ګام.p);
        finalizeHdr(hdr);
        hdr.u = new_u;
        hdr.offset = 0; //data - datastart;
        hdr.addref();
        أرجع hdr;
    }
    امسك(...)
    {
        إذا (u != عدم)
        {
            CV_XADD(&(u->refcount), -1);
            CV_XADD(&(u->urefcount), -1);
        }
        new_u->currAllocator->deallocate(new_u);
        ارم;
    }

}

فراغ UMat::create(صحيح d0, ثابت صحيح* _sizes, صحيح _type, UMatUsageFlags _usageFlags)
{
    صحيح sz1 = 1, d = d0;
    صحيح i;
    إذا (d == 0) {
        d = 1;
        _sizes = (ثابت صحيح*)&sz1;
    }
    CV_Assert(0 <= d && d <= CV_MAX_DIM && _sizes);
    _type = CV_MAT_TYPE(_type);

    // if param value is USAGE_DEFAULT by implicit default param value -or- explicit value
    // ...then don't change the existing usageFlags
    // it is not possible to change usage from non-default to USAGE_DEFAULT through create()
    // ...instead must construct UMat()
    إذا (_usageFlags == cv::USAGE_DEFAULT)
    {
        _usageFlags = usageFlags;
    }

    إذا( u && d == dims && _type == type() && _usageFlags == usageFlags )
    {
        لكل( i = 0; i < d; i++ )
            إذا( size[i] != _sizes[i] )
                اكسر;
        إذا( i == d )
            أرجع;
    }

    صحيح _sizes_backup[CV_MAX_DIM]; // #5991
    إذا (_sizes == (هذا->size.p))
    {
        لكل(i = 0; i < d; i++ )
            _sizes_backup[i] = _sizes[i];
        _sizes = _sizes_backup;
    }

    release();
    usageFlags = _usageFlags;
    إذا( d == 0 )
        أرجع;
    flags = (_type & CV_MAT_TYPE_ماسک) | MAGIC_VAL;
    سېټ_اندازه(*هذا, d, _sizes, 0, ریښتیا);
    offset = 0;

    إذا( ټول() > 0 )
    {
        MatAllocator *a = allocator, *a0 = getStdAllocator();
        إذا (!a)
        {
            a = a0;
            a0 = Mat::getDefaultAllocator();
        }
        حاول
        {
            u = a->allocate(dims, size.p, _type, 0, ګام.p, ACCESS_RW /* ignored */, usageFlags);
            CV_Assert(u != 0);
        }
        امسك(...)
        {
            إذا(a != a0)
                u = a0->allocate(dims, size.p, _type, 0, ګام.p, ACCESS_RW /* ignored */, usageFlags);
            CV_Assert(u != 0);
        }
        CV_Assert( ګام[dims-1] == (size_t)CV_ELEM_اندازه(flags) );
    }

    finalizeHdr(*هذا);
    addref();
    size.dims = dims = d0;
}

فراغ UMat::create(ثابت std::vector<صحيح>& _sizes, صحيح _type, UMatUsageFlags _usageFlags)
{
    create((صحيح)_sizes.size(), _sizes.معلومات(), _type, _usageFlags);
}

فراغ UMat::create(ثابت MatShape& _shape, صحيح _type, UMatUsageFlags _usageFlags)
{
    إذا (_shape.dims < 0) {
        release();
    } وإلا {
        create(_shape.dims, _shape.p, _type, _usageFlags);
        size.layout = _shape.layout;
        size.C = _shape.C;
    }
}

فراغ UMat::fit(صحيح _dims, ثابت صحيح* _sizes, صحيح _type, UMatUsageFlags _usageFlags)
{
    إذا (_usageFlags == cv::USAGE_DEFAULT)
        _usageFlags = usageFlags;
    size_t old_ټول_بایټونه = u ? u->size : 0;
    size_t esz = CV_ELEM_اندازه(_type), new_ټول = _dims >= 0;
    لكل (صحيح i = 0; i < _dims; i++)
        new_ټول *= _sizes[i];
    size_t new_ټول_بایټونه = new_ټول*esz;
    إذا (new_ټول_بایټونه > 0 &&
        (!isContinuous() ||
         new_ټول_بایټونه > old_ټول_بایټونه ||
         offset != 0 ||
         _usageFlags != usageFlags)) {
        create(_dims, _sizes, _type, _usageFlags);
    } وإلا {
        flags = (flags & ~Mat::TYPE_ماسک) | CV_MAT_TYPE(_type);
        صحيح dummy_اندازه = 0;
        سېټ_اندازه(*هذا, (_dims >= 0 ? _dims : 1), (_dims >= 0 ? _sizes : &dummy_اندازه), تش_اشاره, ریښتیا);
        finalizeHdr(*هذا);
    }
}

فراغ UMat::fit(ثابت std::vector<صحيح>& _shape, صحيح _type, UMatUsageFlags _usageFlags)
{
    fit((صحيح)_shape.size(), _shape.معلومات(), _type, _usageFlags);
}

فراغ UMat::fit(ثابت MatShape& _shape, صحيح _type, UMatUsageFlags _usageFlags)
{
    fit(_shape.dims, _shape.p, _type, _usageFlags);
    size.layout = _shape.layout;
    size.C = _shape.C;
}

فراغ UMat::fit(صحيح لیکې_3, صحيح ستون_شمېر_3, صحيح _type, UMatUsageFlags _usageFlags)
{
    _type &= TYPE_ماسک;
    صحيح sz[] = {لیکې_3, ستون_شمېر_3};
    fit(2, sz, _type, _usageFlags);
}

فراغ UMat::fit(اندازه _sz, صحيح _type, UMatUsageFlags _usageFlags)
{
    fit(_sz.لوړوالی, _sz.پلنوالی, _type, _usageFlags);
}

فراغ UMat::fit_Same_اندازه(ننوت_Array m, صحيح _type, UMatUsageFlags _usageFlags)
{
    fit(m.shape(), _type, _usageFlags);
}

فراغ UMat::copy_اندازه(ثابت UMat& m)
{
    dims = m.dims;
    ستون_شمېر = m.ستون_شمېر;
    لیکې = m.لیکې;
    size = m.size;
    ګام = m.ګام;
}

UMat::~UMat()
{
    release();
}

فراغ UMat::deallocate()
{
    UMat_معلومات* u_ = u;
    u = عدم;
    u_->currAllocator->deallocate(u_);
}


UMat::UMat(ثابت UMat& m, ثابت Range& لیکه_Range_3, ثابت Range& ستون_Range_3)
    : flags(MAGIC_VAL), dims(0), لیکې(0), ستون_شمېر(0), allocator(0), usageFlags(USAGE_DEFAULT), u(0), offset(0)
{
    CV_Assert( m.dims >= 2 && m.dims <= MatShape::MAX_DIMS );
    إذا( m.dims > 2 )
    {
        Range rs[MatShape::MAX_DIMS];
        rs[0] = لیکه_Range_3;
        rs[1] = ستون_Range_3;
        لكل( صحيح i = 2; i < m.dims; i++ )
            rs[i] = Range::all();
        *هذا = m(rs);
        أرجع;
    }

    *هذا = m;
    إذا( لیکه_Range_3 != Range::all() && لیکه_Range_3 != Range(0,لیکې) )
    {
        CV_Assert( 0 <= لیکه_Range_3.پیل &&
                  لیکه_Range_3.پیل <= لیکه_Range_3.end &&
                  لیکه_Range_3.end <= m.لیکې );
        size[0] = لیکې = لیکه_Range_3.size();
        offset += ګام*لیکه_Range_3.پیل;
        flags |= SUBMATRIX_FLAG;
    }

    إذا( ستون_Range_3 != Range::all() && ستون_Range_3 != Range(0,ستون_شمېر) )
    {
        CV_Assert( 0 <= ستون_Range_3.پیل &&
                  ستون_Range_3.پیل <= ستون_Range_3.end &&
                  ستون_Range_3.end <= m.ستون_شمېر );
        size[1] = ستون_شمېر = ستون_Range_3.size();
        offset += ستون_Range_3.پیل*elem_اندازه();
        flags |= SUBMATRIX_FLAG;
    }

    updateContinuityFlag();

    إذا( لیکې <= 0 || ستون_شمېر <= 0 )
    {
        release();
        لیکې = ستون_شمېر = 0;
    }
}


UMat::UMat(ثابت UMat& m, ثابت Rect& roi)
    : flags(m.flags), dims(2), لیکې(roi.لوړوالی), ستون_شمېر(roi.پلنوالی),
    allocator(m.allocator), usageFlags(m.usageFlags), u(m.u),
    offset(m.offset + roi.y*m.ګام[0]), size(2)
{
    CV_Assert( m.dims <= 2 );

    size_t esz = CV_ELEM_اندازه(flags);
    offset += roi.x*esz;
    CV_Assert( 0 <= roi.x && 0 <= roi.پلنوالی && roi.x + roi.پلنوالی <= m.ستون_شمېر &&
              0 <= roi.y && 0 <= roi.لوړوالی && roi.y + roi.لوړوالی <= m.لیکې );
    إذا( roi.پلنوالی < m.ستون_شمېر || roi.لوړوالی < m.لیکې )
        flags |= SUBMATRIX_FLAG;

    size[0] = لیکې;
    size[1] = ستون_شمېر;
    ګام[0] = m.dims == 2 ? m.ګام[0] : ستون_شمېر*esz;
    ګام[1] = esz;

    updateContinuityFlag();

    addref();
    إذا( لیکې <= 0 || ستون_شمېر <= 0 )
    {
        لیکې = ستون_شمېر = 0;
        release();
    }
}


UMat::UMat(ثابت UMat& m, ثابت Range* ranges)
    : flags(MAGIC_VAL), dims(0), لیکې(0), ستون_شمېر(0), allocator(0), usageFlags(USAGE_DEFAULT), u(0), offset(0)
{
    صحيح i, d = m.dims;

    CV_Assert(ranges);
    لكل( i = 0; i < d; i++ )
    {
        Range r = ranges[i];
        CV_Assert( r == Range::all() || (0 <= r.پیل && r.پیل < r.end && r.end <= m.size[i]) );
    }
    *هذا = m;
    لكل( i = 0; i < d; i++ )
    {
        Range r = ranges[i];
        إذا( r != Range::all() && r != Range(0, size.p[i]))
        {
            size.p[i] = r.end - r.پیل;
            offset += r.پیل*ګام.p[i];
            flags |= SUBMATRIX_FLAG;
        }
    }

    إذا (d <= 2) {
        لیکې = d == 2 ? size[0] : 1;
        ستون_شمېر = d <= 0 ? (d >= 0) : size[d > 1];
    }
    updateContinuityFlag();
}

UMat::UMat(ثابت UMat& m, ثابت std::vector<Range>& ranges)
    : flags(MAGIC_VAL), dims(0), لیکې(0), ستون_شمېر(0), allocator(0), usageFlags(USAGE_DEFAULT), u(0), offset(0)
{
    صحيح i, d = m.dims;

    CV_Assert((صحيح)ranges.size() == d);
    لكل (i = 0; i < d; i++)
    {
        Range r = ranges[i];
        CV_Assert(r == Range::all() || (0 <= r.پیل && r.پیل < r.end && r.end <= m.size[i]));
    }
    *هذا = m;
    لكل (i = 0; i < d; i++)
    {
        Range r = ranges[i];
        إذا (r != Range::all() && r != Range(0, size.p[i]))
        {
            size.p[i] = r.end - r.پیل;
            offset += r.پیل*ګام.p[i];
            flags |= SUBMATRIX_FLAG;
        }
    }

    إذا (d <= 2) {
        لیکې = d == 2 ? size[0] : 1;
        ستون_شمېر = d <= 0 ? (d >= 0) : size[d > 1];
    }
    updateContinuityFlag();
}

UMat UMat::diag(صحيح d) ثابت
{
    CV_Assert( dims <= 2 );
    UMat m = *هذا;
    size_t esz = elem_اندازه();
    صحيح len;

    إذا( d >= 0 )
    {
        len = std::min(ستون_شمېر - d, لیکې);
        m.offset += esz*d;
    }
    وإلا
    {
        len = std::min(لیکې + d, ستون_شمېر);
        m.offset -= ګام[0]*d;
    }
    CV_DbgAssert( len > 0 );

    m.size[0] = m.لیکې = len;
    m.size[1] = m.ستون_شمېر = 1;
    m.ګام[0] += (len > 1 ? esz : 0);

    m.updateContinuityFlag();

    إذا( size() != اندازه(1,1) )
        m.flags |= SUBMATRIX_FLAG;

    أرجع m;
}

فراغ UMat::locateROI( اندازه& ټولیز_اندازه, ټکی& ofs ) ثابت
{
    CV_Assert( dims <= 2 && ګام[0] > 0 );
    size_t esz = elem_اندازه(), minstep;
    ptrdiff_t delta1 = (ptrdiff_t)offset, delta2 = (ptrdiff_t)u->size;

    إذا( delta1 == 0 )
        ofs.x = ofs.y = 0;
    وإلا
    {
        ofs.y = (صحيح)(delta1/ګام[0]);
        ofs.x = (صحيح)((delta1 - ګام[0]*ofs.y)/esz);
        CV_DbgAssert( offset == (size_t)(ofs.y*ګام[0] + ofs.x*esz) );
    }
    minstep = (ofs.x + ستون_شمېر)*esz;
    ټولیز_اندازه.لوړوالی = (صحيح)((delta2 - minstep)/ګام[0] + 1);
    ټولیز_اندازه.لوړوالی = std::max(ټولیز_اندازه.لوړوالی, ofs.y + لیکې);
    ټولیز_اندازه.پلنوالی = (صحيح)((delta2 - ګام*(ټولیز_اندازه.لوړوالی-1))/esz);
    ټولیز_اندازه.پلنوالی = std::max(ټولیز_اندازه.پلنوالی, ofs.x + ستون_شمېر);
}


UMat& UMat::adjustROI( صحيح dtop, صحيح dbottom, صحيح dleft, صحيح dright )
{
    CV_Assert( dims == 2 && ګام[0] > 0 );
    اندازه ټولیز_اندازه; ټکی ofs;
    size_t esz = elem_اندازه();
    locateROI( ټولیز_اندازه, ofs );
    صحيح row1 = std::min(std::max(ofs.y - dtop, 0), ټولیز_اندازه.لوړوالی), row2 = std::max(0, std::min(ofs.y + لیکې + dbottom, ټولیز_اندازه.لوړوالی));
    صحيح col1 = std::min(std::max(ofs.x - dleft, 0), ټولیز_اندازه.پلنوالی), col2 = std::max(0, std::min(ofs.x + ستون_شمېر + dright, ټولیز_اندازه.پلنوالی));
    إذا(row1 > row2)
        std::swap(row1, row2);
    إذا(col1 > col2)
        std::swap(col1, col2);

    offset += (row1 - ofs.y)*ګام + (col1 - ofs.x)*esz;
    لیکې = row2 - row1; ستون_شمېر = col2 - col1;
    size.p[0] = لیکې; size.p[1] = ستون_شمېر;
    updateContinuityFlag();
    أرجع *هذا;
}


UMat UMat::reshape(صحيح new_cn, صحيح new_لیکې) ثابت
{
    صحيح cn = channels();
    UMat hdr = *هذا;

    إذا( dims > 2 && new_لیکې == 0 && new_cn != 0 && size[dims-1]*cn % new_cn == 0 )
    {
        hdr.flags = (hdr.flags & ~CV_MAT_CN_ماسک) | ((new_cn-1) << CV_CN_ښويدنه);
        hdr.ګام[dims-1] = CV_ELEM_اندازه(hdr.flags);
        hdr.size[dims-1] = hdr.size[dims-1]*cn / new_cn;
        أرجع hdr;
    }

    CV_Assert( dims <= 2 );

    إذا( new_cn == 0 )
        new_cn = cn;

    صحيح ټول_پلنوالی = ستون_شمېر * cn;

    إذا( (new_cn > ټول_پلنوالی || ټول_پلنوالی % new_cn != 0) && new_لیکې == 0 )
        new_لیکې = لیکې * ټول_پلنوالی / new_cn;

    إذا( new_لیکې != 0 && new_لیکې != لیکې )
    {
        صحيح ټول_اندازه_2 = ټول_پلنوالی * لیکې;
        إذا( !isContinuous() )
            CV_Error( cv::Error::Bad_ګام,
            "The matrix is not continuous, thus its number of rows can not be changed" );

        ټول_پلنوالی = ټول_اندازه_2 / new_لیکې;

        إذا( ټول_پلنوالی * new_لیکې != ټول_اندازه_2 )
            CV_Error( cv::Error::StsBadArg, "The total number of matrix elements "
                                    "is not divisible by the new number of rows" );

        hdr.size[0] = hdr.لیکې = new_لیکې;
        hdr.ګام[0] = ټول_پلنوالی * elemSize1();
    } وإلا {
        hdr.size[0] = hdr.لیکې = لیکې;
        إذا (dims <= 1)
            hdr.ګام[0] = ستون_شمېر * CV_ELEM_اندازه(flags);
    }

    صحيح new_پلنوالی = ټول_پلنوالی / new_cn;

    إذا( new_پلنوالی * new_cn != ټول_پلنوالی )
        CV_Error( cv::Error::Bad_عدد_Channels,
        "The total width is not divisible by the new number of channels" );

    hdr.size.dims = hdr.dims = 2;
    hdr.size[1] = hdr.ستون_شمېر = new_پلنوالی;
    hdr.flags = (hdr.flags & ~CV_MAT_CN_ماسک) | ((new_cn-1) << CV_CN_ښويدنه);
    hdr.ګام[1] = CV_ELEM_اندازه(hdr.flags);
    أرجع hdr;
}

#که_تعریف_شوی HAVE_OPENCL
نطاق {
ساكن منطقي ocl_سېټ_Diag(ثابت UMat& d, UMat& m, صحيح len)
{
    صحيح cn = d.channels();
    صحيح ژوروالی = d.ژوروالی();

    إذا (ژوروالی == CV_64F && !ocl::Device::getDefault().doubleFPConfig())
        أرجع خطأ;

    String opts = format("-D SET_DIAG -D T1=%s -D cn=%d -D IS_ROW_VECTOR=%d",
                         ocl::memop_Type_منزل_Str(ژوروالی),
                         cn,
                         (d.لیکې == 1) ? 1 : 0);

    ocl::Kernel k("setDiag", ocl::core::copyset_oclsrc, opts);
    إذا (k.empty())
        أرجع خطأ;

    k.دلیلونه(ocl::KernelArg::WriteOnly(m),
           ocl::KernelArg::Read_Only_No_اندازه(d),
           len);

    size_t globalsize[2] = { (size_t)len, (size_t)len };
    أرجع k.پرلپسې(2, globalsize, عدم, خطأ);
}
}
#د_که_پای

UMat UMat::diag(ثابت UMat& d, UMatUsageFlags usageFlags)
{
    CV_INSTRUMENT_REGION();

    CV_Assert(d.ستون_شمېر == 1 || d.لیکې == 1);
    صحيح len = d.لیکې + d.ستون_شمېر - 1;

#که_تعریف_شوی HAVE_OPENCL
    إذا (ocl::useOpenCL())
    {
        UMat m(len, len, d.type(), usageFlags);
        إذا (ocl_سېټ_Diag(d, m, len))
        {
            CV_IMPL_جمع_کړه(CV_IMPL_OCL);
            أرجع m;
        }
    }
#د_که_پای

    UMat m(len, len, d.type(), Scalar(0), usageFlags);
    UMat md = m.diag();
    إذا( d.ستون_شمېر == 1 )
        d.copy_منزل(md);
    وإلا
        transpose(d, md);
    أرجع m;
}

صحيح UMat::check_ویکټور(صحيح _elemChannels, صحيح ژوروالی_2, منطقي _requireContinuous) ثابت
{
    أرجع (ژوروالی() == ژوروالی_2 || ژوروالی_2 <= 0) &&
        (isContinuous() || !_requireContinuous) &&
        ((dims <= 2 && (((لیکې == 1 || ستون_شمېر == 1) && channels() == _elemChannels) ||
                        (ستون_شمېر == _elemChannels && channels() == 1))) ||
        (dims == 3 && channels() == 1 && size.p[2] == _elemChannels && (size.p[0] == 1 || size.p[1] == 1) &&
         (isContinuous() || ګام.p[1] == ګام.p[2]*size.p[2])))
    ? (صحيح)(ټول()*channels()/_elemChannels) : -1;
}

UMat UMat::reshape(صحيح _cn, صحيح _newndims, ثابت صحيح* _newsz) ثابت
{
    إذا(_newndims == dims)
    {
        إذا(_newsz == 0)
            أرجع reshape(_cn);
        إذا(_newndims == 2)
            أرجع reshape(_cn, _newsz[0]);
    }

    إذا (isContinuous())
    {
        CV_Assert(_cn >= 0 && _newndims >= 0 && _newndims <= CV_MAX_DIM && (_newndims == 0 || _newsz != 0));

        إذا (_cn == 0)
            _cn = هذا->channels();
        وإلا
            CV_Assert(_cn <= CV_CN_MAX);

        size_t ټول_elem1_ref = هذا->ټول() * هذا->channels();
        size_t ټول_elem1 = _cn;

        Auto_بفر<صحيح, 4> newsz_buf( (size_t)std::max(_newndims, 1) );

        صحيح m1_idx = -1;

        لكل (صحيح i = 0; i < _newndims; i++)
        {
            إذا (_newsz[i] >= 0) {
                إذا (_newsz[i] == 0 && i < dims)
                    newsz_buf[i] = size.p[i];
                وإلا
                    newsz_buf[i] = _newsz[i];
                ټول_elem1 *= (size_t)newsz_buf[i];
            } وإلا {
                إذا (m1_idx >= 0)
                    CV_Error(cv::Error::Sts_Bad_اندازه, "More than one '-1' occured in the new shape");
                m1_idx = i;
            }
        }

        إذا (m1_idx >= 0) {
            إذا (ټول_elem1 == 0) {
                CV_Assert(ټول_elem1_ref == 0);
                ټول_elem1 = 1;
            }
            CV_Assert(ټول_elem1_ref % ټول_elem1 == 0);
            newsz_buf[m1_idx] = (صحيح)(ټول_elem1_ref / ټول_elem1);
        } وإلا إذا (ټول_elem1 != ټول_elem1_ref) {
            CV_Error(cv::Error::StsUnmatchedSizes, "Requested and source matrices have different count of elements");
        }

        UMat hdr = *هذا;
        hdr.flags = (hdr.flags & ~CV_MAT_CN_ماسک) | ((_cn-1) << CV_CN_ښويدنه);
        سېټ_اندازه(hdr, _newndims, newsz_buf.معلومات(), عدم, ریښتیا);

        أرجع hdr;
    }

    CV_Error(cv::Error::StsNotImplemented, "Reshaping of n-dimensional non-continuous matrices is not supported yet");
}

UMat UMat::reshape(صحيح _cn, ثابت MatShape& _newshape) ثابت
{
    إذا (_newshape.dims < 0) {
        صحيح newshape[] = {0};
        أرجع reshape(_cn, 1, newshape);
    }
    أرجع reshape(_cn, _newshape.dims, _newshape.p);
}

Mat UMat::getMat(AccessFlag accessFlags) ثابت
{
    إذا(!u)
        أرجع Mat();
    // TODO Support ACCESS_READ (ACCESS_WRITE) without unnecessary data transfers
    accessFlags |= ACCESS_RW;
    UMat_معلومات_Auto_قلف autolock(u);
    حاول
    {
        إذا(CV_XADD(&u->refcount, 1) == 0)
            u->currAllocator->map(u, accessFlags);
        إذا (u->معلومات != 0)
        {
            Mat hdr(dims, size.p, type(), u->معلومات + offset, ګام.p);
            hdr.flags = flags;
            hdr.u = u;
            hdr.datastart = u->معلومات;
            hdr.معلومات = u->معلومات + offset;
            hdr.datalimit = hdr.dataend = u->معلومات + u->size;
            أرجع hdr;
        }
    }
    امسك(...)
    {
        CV_XADD(&u->refcount, -1);
        ارم;
    }
    CV_XADD(&u->refcount, -1);
    CV_Assert(u->معلومات != 0 && "Error mapping of UMat to host memory.");
    أرجع Mat();
}

فراغ* UMat::handle(AccessFlag accessFlags) ثابت
{
    إذا( !u )
        أرجع 0;

    CV_Assert(u->refcount == 0);
    CV_Assert(!u->deviceCopyObsolete() || u->copyOnMap());
    إذا (u->deviceCopyObsolete())
    {
        u->currAllocator->unmap(u);
    }

    إذا (!!(accessFlags & ACCESS_WRITE))
        u->markHostCopyObsolete(ریښتیا);

    أرجع u->handle;
}

فراغ UMat::ndoffset(size_t* ofs) ثابت
{
    // offset = step[0]*ofs[0] + step[1]*ofs[1] + step[2]*ofs[2] + ...;
    size_t val = offset;
    لكل( صحيح i = 0; i < dims; i++ )
    {
        size_t s = ګام.p[i];
        ofs[i] = val / s;
        val -= ofs[i]*s;
    }
}

فراغ UMat::copy_منزل(OutputArray _dst) ثابت
{
    CV_INSTRUMENT_REGION();

#که_تعریف_شوی HAVE_CUDA
    إذا (_dst.isGpuMat())
    {
        _dst.getGpuMat().upload(*هذا);
        أرجع;
    }
#د_که_پای

    صحيح stype = type();
    صحيح dtype = _dst.type();
    إذا( _dst.fixedType() && dtype != stype )
    {
        CV_Assert( CV_MAT_CN(stype) == CV_MAT_CN(dtype) );
        convert_منزل( _dst, dtype );
        أرجع;
    }

    إذا( dims == 0 && empty() )
    {
        _dst.release();
        فراغ* obj = _dst.getObj();
        إذا (_dst.isMat())
            reinterpret_cast<Mat*>(obj)->flags = Mat::MAGIC_VAL | Mat::CONTINUOUS_FLAG | stype;
        وإلا إذا (_dst.isUMat())
            reinterpret_cast<UMat*>(obj)->flags = UMat::MAGIC_VAL | UMat::CONTINUOUS_FLAG | stype;
        وإلا إذا (_dst.isGpuMat())
            reinterpret_cast<cuda::GpuMat*>(obj)->flags = stype;
        أرجع;
    }

    _dst.create( dims, size.p, stype );
    إذا (empty())
        أرجع;

    size_t sz[CV_MAX_DIM] = {1}, srcofs[CV_MAX_DIM]={0}, dstofs[CV_MAX_DIM]={0};
    size_t esz = CV_ELEM_اندازه(stype);
    صحيح i, d = std::max(dims, 1);
    لكل( i = 0; i < d; i++ )
        sz[i] = size.p[i];
    sz[d-1] *= esz;
    ndoffset(srcofs);
    srcofs[d-1] *= esz;

    إذا( _dst.isUMat() )
    {
        UMat dst = _dst.getUMat();
        CV_Assert(dst.u);
        إذا( u == dst.u && dst.offset == offset )
            أرجع;

        إذا (u->currAllocator == dst.u->currAllocator)
        {
            dst.ndoffset(dstofs);
            dstofs[dims-1] *= esz;
            u->currAllocator->copy(u, dst.u, d, sz, srcofs, ګام.p, dstofs, dst.ګام.p, خطأ);
            أرجع;
        }

        // different allocators: never map a device destination, upload into it instead
        // (through a host Mat when both sides are device memory)
        MatAllocator* hostAllocator = Mat::getDefaultAllocator();
        إذا (dst.u->currAllocator != hostAllocator)
        {
            إذا (u->currAllocator == hostAllocator)
                getMat(ACCESS_READ).copy_منزل(_dst);
            وإلا
            {
                Mat tmp;
                copy_منزل(tmp);
                tmp.copy_منزل(_dst);
            }
            أرجع;
        }
    }

    Mat dst = _dst.getMat();
    u->currAllocator->download(u, dst.ptr(), d, sz, srcofs, ګام.p, dst.ګام.p);
}

فراغ UMat::copy_منزل(OutputArray _dst, ننوت_Array ماسک_2) ثابت
{
    CV_INSTRUMENT_REGION();

    إذا( ماسک_2.empty() )
    {
        copy_منزل(_dst);
        أرجع;
    }
#که_تعریف_شوی HAVE_OPENCL
    صحيح cn = channels(), mtype = ماسک_2.type(), mdepth = CV_MAT_ژوروالی(mtype), mcn = CV_MAT_CN(mtype);
    CV_Assert( (mdepth == CV_8U || mdepth == CV_8S || mdepth == CV_Bool) && (mcn == 1 || mcn == cn) );

    إذا (ocl::useOpenCL() && _dst.isUMat() && dims <= 2)
    {
        UMat_معلومات * prevu = _dst.getUMat().u;
        _dst.create( size, type() );

        UMat dst = _dst.getUMat();

        منطقي haveDstUninit = خطأ;
        إذا( prevu != dst.u ) // do not leave dst uninitialized
            haveDstUninit = ریښتیا;

        String opts = format("-D COPY_TO_MASK -D T1=%s -D scn=%d -D mcn=%d%s",
                             ocl::memop_Type_منزل_Str(ژوروالی()), cn, mcn,
                             haveDstUninit ? " -D HAVE_DST_UNINIT" : "");

        ocl::Kernel k("copyToMask", ocl::core::copyset_oclsrc, opts);
        إذا (!k.empty())
        {
            k.دلیلونه(ocl::KernelArg::Read_Only_No_اندازه(*هذا),
                   ocl::KernelArg::Read_Only_No_اندازه(ماسک_2.getUMat()),
                   haveDstUninit ? ocl::KernelArg::WriteOnly(dst) :
                                   ocl::KernelArg::ReadWrite(dst));

            size_t globalsize[2] = { (size_t)ستون_شمېر, (size_t)لیکې };
            إذا (k.پرلپسې(2, globalsize, عدم, خطأ))
            {
                CV_IMPL_جمع_کړه(CV_IMPL_OCL);
                أرجع;
            }
        }
    }
#د_که_پای
    Mat src = getMat(ACCESS_READ);
    src.copy_منزل(_dst, ماسک_2);
}


//
// void UMat::convertTo moved to convert.dispatch.cpp
//

UMat& UMat::سېټ_منزل(ننوت_Array ارزښت, ننوت_Array ماسک_2)
{
    CV_INSTRUMENT_REGION();

    منطقي have_ماسک = !ماسک_2.empty();
#که_تعریف_شوی HAVE_OPENCL
    صحيح tp = type(), cn = CV_MAT_CN(tp), d = CV_MAT_ژوروالی(tp);

    إذا( dims <= 2 && cn <= 4 && CV_MAT_ژوروالی(tp) < CV_64F && ocl::useOpenCL() )
    {
        Mat value = ارزښت.getMat();
        CV_Assert( checkScalar(value, type(), ارزښت.ډول(), ننوت_Array_2::UMAT) );
        صحيح kercn = have_ماسک || cn == 3 ? cn : std::max(cn, ocl::predict_Optimal_ویکټور_پلنوالی(*هذا)),
                kertp = CV_MAKE_TYPE(d, kercn);

        مزدوج buf[16] = { 0, 0, 0, 0, 0, 0, 0, 0,
                           0, 0, 0, 0, 0, 0, 0, 0 };
        convertAndUnrollScalar(value, tp, (uchar *)buf, kercn / cn);

        صحيح scalarcn = kercn == 3 ? 4 : kercn, لیکې_فی_WI = ocl::Device::getDefault().isIntel() ? 4 : 1;
        String opts = format("-D dstT=%s -D rowsPerWI=%d -D dstST=%s -D dstT1=%s -D cn=%d",
                             ocl::memop_Type_منزل_Str(kertp), لیکې_فی_WI,
                             ocl::memop_Type_منزل_Str(CV_MAKETYPE(d, scalarcn)),
                             ocl::memop_Type_منزل_Str(d), kercn);

        ocl::Kernel سېټ_K(have_ماسک ? "setMask" : "set", ocl::core::copyset_oclsrc, opts);
        إذا( !سېټ_K.empty() )
        {
            ocl::KernelArg scalararg(ocl::KernelArg::CONSTANT, 0, 0, 0, buf, CV_ELEM_اندازه(d) * scalarcn);
            UMat ماسک;

            إذا( have_ماسک )
            {
                ماسک = ماسک_2.getUMat();
                CV_Assert( ماسک.size() == size() && (ماسک.type() == CV_8U || ماسک.type() == CV_8S || ماسک.type() == CV_Bool) );
                ocl::KernelArg maskarg = ocl::KernelArg::Read_Only_No_اندازه(ماسک),
                        dstarg = ocl::KernelArg::ReadWrite(*هذا);
                سېټ_K.دلیلونه(maskarg, dstarg, scalararg);
            }
            وإلا
            {
                ocl::KernelArg dstarg = ocl::KernelArg::WriteOnly(*هذا, cn, kercn);
                سېټ_K.دلیلونه(dstarg, scalararg);
            }

            size_t globalsize[] = { (size_t)ستون_شمېر * cn / kercn, ((size_t)لیکې + لیکې_فی_WI - 1) / لیکې_فی_WI };
            إذا( سېټ_K.پرلپسې(2, globalsize, عدم, خطأ) )
            {
                CV_IMPL_جمع_کړه(CV_IMPL_OCL);
                أرجع *هذا;
            }
        }
    }
#د_که_پای
    Mat m = getMat(have_ماسک ? ACCESS_RW : ACCESS_WRITE);
    m.سېټ_منزل(ارزښت, ماسک_2);
    أرجع *هذا;
}

UMat& UMat::چلوونکی = (ثابت Scalar& s)
{
    سېټ_منزل(s);
    أرجع *هذا;
}

UMat UMat::t() ثابت
{
    UMat m;
    transpose(*هذا, m);
    أرجع m;
}

UMat UMat::zeros(صحيح لیکې, صحيح ستون_شمېر, صحيح type, UMatUsageFlags usageFlags)
{
    أرجع UMat(لیکې, ستون_شمېر, type, Scalar::all(0), usageFlags);
}

UMat UMat::zeros(اندازه size, صحيح type, UMatUsageFlags usageFlags)
{
    أرجع UMat(size, type, Scalar::all(0), usageFlags);
}

UMat UMat::zeros(صحيح ndims, ثابت صحيح* sz, صحيح type, UMatUsageFlags usageFlags)
{
    أرجع UMat(ndims, sz, type, Scalar::all(0), usageFlags);
}

UMat UMat::zeros(ثابت MatShape& shape, صحيح type, UMatUsageFlags usageFlags)
{
    أرجع UMat(shape.dims, shape.p, type, Scalar::all(0), usageFlags);
}

UMat UMat::ones(صحيح لیکې, صحيح ستون_شمېر, صحيح type, UMatUsageFlags usageFlags)
{
    أرجع UMat(لیکې, ستون_شمېر, type, Scalar(1), usageFlags);
}

UMat UMat::ones(اندازه size, صحيح type, UMatUsageFlags usageFlags)
{
    أرجع UMat(size, type, Scalar(1), usageFlags);
}

UMat UMat::ones(صحيح ndims, ثابت صحيح* sz, صحيح type, UMatUsageFlags usageFlags)
{
    أرجع UMat(ndims, sz, type, Scalar(1), usageFlags);
}

UMat UMat::ones(ثابت MatShape& shape, صحيح type, UMatUsageFlags usageFlags)
{
    أرجع UMat(shape.dims, shape.p, type, Scalar(1), usageFlags);
}

}

/* End of file. */
