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
فراغ سیٹ_ناپ(UMat& m, صحيح _dims, ثابت صحيح* _sz, ثابت size_t* قدماں_2,
             منطقي auto_قدماں = خطأ);

فراغ updateContinuityFlag(UMat& m);
فراغ finalizeHdr(UMat& m);

UMat_انکڑے::UMat_انکڑے(ثابت MatAllocator* allocator)
{
    prevAllocator = currAllocator = allocator;
    urefcount = refcount = mapcount = 0;
    انکڑے = origdata = 0;
    size = 0;
    flags = static_cast<UMat_انکڑے::حافظہ_Flag>(0);
    handle = 0;
    userdata = 0;
    allocatorFlags_ = 0;
    original_UMat_انکڑے = عدم;
}

UMat_انکڑے::~UMat_انکڑے()
{
    prevAllocator = currAllocator = 0;
    urefcount = refcount = 0;
    CV_Assert(mapcount == 0);
    انکڑے = origdata = 0;
    size = 0;
    منطقي isAsyncCleanup = !!(flags & UMat_انکڑے::ASYNC_CLEANUP);
    flags = static_cast<UMat_انکڑے::حافظہ_Flag>(0);
    handle = 0;
    userdata = 0;
    allocatorFlags_ = 0;
    إذا (original_UMat_انکڑے)
    {
        منطقي showWarn = خطأ;
        UMat_انکڑے* u = original_UMat_انکڑے;
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
            showWarn = سچ;
        إذا (zero_Ref && zero_URef) // oops, we need to free resources
        {
            showWarn = !isAsyncCleanup;
            // simulate UMat::deallocate
            u->currAllocator->deallocate(u);
        }
#جے_بنا_تعریف NDEBUG
        إذا (showWarn)
        {
            ساكن صحيح warn_سنیہا_showed = 0;
            إذا (warn_سنیہا_showed++ < 100)
            {
                fflush(stdout);
                fprintf(stderr, "\n! OPENCV warning: getUMat()/getMat() call chain possible problem."
                                "\n!                 Base object is dead, while nested/derived object is still alive or processed."
                                "\n!                 Please check lifetime of UMat/Mat objects!\n");
                fflush(stderr);
            }
        }
#وإلا
        CV_بن_ورتیا(showWarn);
#جے_انت
        original_UMat_انکڑے = عدم;
    }
}

#جے_بنا_تعریف OPENCV_DISABLE_THREAD_SUPPORT

// it should be a prime number for the best hash function
تعداد { UMAT_NLOCKS = 31 };
ساكن Mutex umatLocks[UMAT_NLOCKS];

ساكن size_t get_UMat_انکڑے_جندرا_اشاریہ(ثابت UMat_انکڑے* u)
{
    size_t idx = ((size_t)(فراغ*)u) % UMAT_NLOCKS;
    أرجع idx;
}

فراغ UMat_انکڑے::lock()
{
    size_t idx = get_UMat_انکڑے_جندرا_اشاریہ(هذا);
    //printf("%d lock(%d)\n", cv::utils::getThreadID(), (int)idx);
    umatLocks[idx].lock();
}

فراغ UMat_انکڑے::unlock()
{
    size_t idx = get_UMat_انکڑے_جندرا_اشاریہ(هذا);
    //printf("%d unlock(%d)\n", cv::utils::getThreadID(), (int)idx);
    umatLocks[idx].unlock();
}


// Do not allow several lock() calls with different UMatData objects.
بنية UMat_انکڑے_Auto_Locker
{
    صحيح usage_گنتی;
    UMat_انکڑے* locked_objects[2];
    UMat_انکڑے_Auto_Locker() : usage_گنتی(0) { locked_objects[0] = عدم; locked_objects[1] = عدم; }

    فراغ lock(UMat_انکڑے*& u1)
    {
        منطقي locked_1 = (u1 == locked_objects[0] || u1 == locked_objects[1]);
        إذا (locked_1)
        {
            u1 = عدم;
            أرجع;
        }
        CV_Assert(usage_گنتی == 0);  // UMatDataAutoLock can't be used multiple times from the same thread
        usage_گنتی = 1;
        locked_objects[0] = u1;
        u1->lock();
    }
    فراغ lock(UMat_انکڑے*& u1, UMat_انکڑے*& u2)
    {
        منطقي locked_1 = (u1 == locked_objects[0] || u1 == locked_objects[1]);
        منطقي locked_2 = (u2 == locked_objects[0] || u2 == locked_objects[1]);
        إذا (locked_1)
            u1 = عدم;
        إذا (locked_2)
            u2 = عدم;
        إذا (locked_1 && locked_2)
            أرجع;
        CV_Assert(usage_گنتی == 0);  // UMatDataAutoLock can't be used multiple times from the same thread
        usage_گنتی = 1;
        locked_objects[0] = u1;
        locked_objects[1] = u2;
        إذا (u1)
            u1->lock();
        إذا (u2)
            u2->lock();
    }
    فراغ release(UMat_انکڑے* u1, UMat_انکڑے* u2)
    {
        إذا (u1 == عدم && u2 == عدم)
            أرجع;
        CV_Assert(usage_گنتی == 1);
        usage_گنتی = 0;
        إذا (u1)
            u1->unlock();
        إذا (u2)
            u2->unlock();
        locked_objects[0] = عدم; locked_objects[1] = عدم;
    }
};
ساكن TLSData<UMat_انکڑے_Auto_Locker>& get_UMat_انکڑے_Auto_Locker_TLS()
{
    CV_SINGLETON_LAZY_INIT_REF(TLSData<UMat_انکڑے_Auto_Locker>, جديد TLSData<UMat_انکڑے_Auto_Locker>());
}
ساكن UMat_انکڑے_Auto_Locker& get_UMat_انکڑے_Auto_Locker() { أرجع get_UMat_انکڑے_Auto_Locker_TLS().getRef(); }


UMat_انکڑے_Auto_جندرا::UMat_انکڑے_Auto_جندرا(UMat_انکڑے* u) : u1(u), u2(عدم)
{
    get_UMat_انکڑے_Auto_Locker().lock(u1);
}
UMat_انکڑے_Auto_جندرا::UMat_انکڑے_Auto_جندرا(UMat_انکڑے* u1_, UMat_انکڑے* u2_) : u1(u1_), u2(u2_)
{
    إذا (get_UMat_انکڑے_جندرا_اشاریہ(u1) > get_UMat_انکڑے_جندرا_اشاریہ(u2))
    {
        std::swap(u1, u2);
    }
    get_UMat_انکڑے_Auto_Locker().lock(u1, u2);
}
UMat_انکڑے_Auto_جندرا::~UMat_انکڑے_Auto_جندرا()
{
    get_UMat_انکڑے_Auto_Locker().release(u1, u2);
}

#وإلا

فراغ UMat_انکڑے::lock()
{
    // nothing in OPENCV_DISABLE_THREAD_SUPPORT mode
}

فراغ UMat_انکڑے::unlock()
{
    // nothing in OPENCV_DISABLE_THREAD_SUPPORT mode
}

UMat_انکڑے_Auto_جندرا::UMat_انکڑے_Auto_جندرا(UMat_انکڑے* u) : u1(u), u2(عدم)
{
    // nothing in OPENCV_DISABLE_THREAD_SUPPORT mode
}
UMat_انکڑے_Auto_جندرا::UMat_انکڑے_Auto_جندرا(UMat_انکڑے* u1_, UMat_انکڑے* u2_) : u1(u1_), u2(u2_)
{
    // nothing in OPENCV_DISABLE_THREAD_SUPPORT mode
}
UMat_انکڑے_Auto_جندرا::~UMat_انکڑے_Auto_جندرا()
{
    // nothing in OPENCV_DISABLE_THREAD_SUPPORT mode
}

#جے_انت  // OPENCV_DISABLE_THREAD_SUPPORT

//////////////////////////////// UMat ////////////////////////////////

UMat::UMat(UMatUsageFlags _usageFlags) CV_NOEXCEPT
: flags(MAGIC_VAL), dims(0), صفاں(0), خانے_گنتی(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{}

UMat::UMat(صحيح صفاں_3, صحيح خانے_گنتی_3, صحيح _type, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), صفاں(0), خانے_گنتی(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(صفاں_3, خانے_گنتی_3, _type);
}

UMat::UMat(صحيح صفاں_3, صحيح خانے_گنتی_3, صحيح _type, ثابت Scalar& _s, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), صفاں(0), خانے_گنتی(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(صفاں_3, خانے_گنتی_3, _type);
    *هذا = _s;
}

UMat::UMat(ناپ _sz, صحيح _type, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), صفاں(0), خانے_گنتی(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create( _sz.اچائی, _sz.چوڑائی, _type );
}

UMat::UMat(ناپ _sz, صحيح _type, ثابت Scalar& _s, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), صفاں(0), خانے_گنتی(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_sz.اچائی, _sz.چوڑائی, _type);
    *هذا = _s;
}

UMat::UMat(صحيح _dims, ثابت صحيح* _sz, صحيح _type, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), صفاں(0), خانے_گنتی(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_dims, _sz, _type);
}

UMat::UMat(صحيح _dims, ثابت صحيح* _sz, صحيح _type, ثابت Scalar& _s, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), صفاں(0), خانے_گنتی(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_dims, _sz, _type);
    *هذا = _s;
}

UMat::UMat(ثابت MatShape& _shape, صحيح _type, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), صفاں(0), خانے_گنتی(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_shape.dims, _shape.p, _type);
}

UMat::UMat(ثابت MatShape& _shape, صحيح _type, ثابت Scalar& _s, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), صفاں(0), خانے_گنتی(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_shape.dims, _shape.p, _type);
    *هذا = _s;
}

UMat::UMat(ثابت UMat& m)
: flags(m.flags), dims(m.dims), صفاں(m.صفاں), خانے_گنتی(m.خانے_گنتی), allocator(m.allocator),
  usageFlags(m.usageFlags), u(m.u), offset(m.offset), size(m.size), قدم(m.قدم)
{
    addref();
}

UMat& UMat::عامل=(ثابت UMat& m)
{
    إذا( هذا != &m )
    {
        const_cast<UMat&>(m).addref();
        release();
        flags = m.flags;
        dims = m.dims;
        صفاں = m.صفاں;
        خانے_گنتی = m.خانے_گنتی;
        size = m.size;
        قدم = m.قدم;
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

فراغ UMat::سونپو_منزل(UMat& m, صحيح _type) ثابت
{
    إذا( _type < 0 )
        m = *هذا;
    وإلا
        convert_منزل(m, _type);
}

فراغ UMat::create(صحيح صفاں_3, صحيح خانے_گنتی_3, صحيح _type, UMatUsageFlags _usageFlags)
{
    صحيح sz[] = {صفاں_3, خانے_گنتی_3};
    create(2, sz, _type, _usageFlags);
}

فراغ UMat::create(ناپ _sz, صحيح _type, UMatUsageFlags _usageFlags)
{
    create(_sz.اچائی, _sz.چوڑائی, _type, _usageFlags);
}

فراغ UMat::create_Same_ناپ(داخلہ_Array arr, صحيح _type, UMatUsageFlags _usageFlags)
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
    dims = خانے_گنتی = صفاں = 0;
}

منطقي UMat::empty() ثابت
{
    أرجع u == 0 || کل() == 0;
}

size_t UMat::کل() ثابت
{
    إذا( dims <= 2 )
        أرجع (size_t)صفاں * خانے_گنتی;
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
: flags(m.flags), dims(m.dims), صفاں(m.صفاں), خانے_گنتی(m.خانے_گنتی), allocator(m.allocator),
  usageFlags(m.usageFlags), u(m.u), offset(m.offset)
{
    size = m.size;
    قدم = m.قدم;
    m.flags = MAGIC_VAL;
    m.usageFlags = USAGE_DEFAULT;
    m.dims = m.صفاں = m.خانے_گنتی = 0;
    m.size.clear();
    m.allocator = عدم;
    m.u = عدم;
    m.offset = 0;
}

UMat& UMat::عامل=(UMat&& m)
{
    إذا (هذا == &m)
      أرجع *هذا;
    release();
    flags = m.flags; dims = m.dims; صفاں = m.صفاں; خانے_گنتی = m.خانے_گنتی;
    allocator = m.allocator; usageFlags = m.usageFlags;
    u = m.u;
    offset = m.offset;
    size = m.size;
    قدم = m.قدم;
    m.flags = MAGIC_VAL;
    m.usageFlags = USAGE_DEFAULT;
    m.dims = m.صفاں = m.خانے_گنتی = 0;
    m.allocator = عدم;
    m.u = عدم;
    m.offset = 0;
    أرجع *هذا;
}


MatAllocator* UMat::getStdAllocator()
{
#جے_تعریف_شدہ HAVE_OPENCL
    إذا (ocl::useOpenCL())
        أرجع ocl::getOpenCLAllocator();
#جے_انت
    أرجع Mat::getDefaultAllocator();
}

فراغ swap( UMat& a, UMat& b )
{
    std::swap(a.flags, b.flags);
    std::swap(a.dims, b.dims);
    std::swap(a.صفاں, b.صفاں);
    std::swap(a.خانے_گنتی, b.خانے_گنتی);
    std::swap(a.allocator, b.allocator);
    std::swap(a.u, b.u);
    std::swap(a.offset, b.offset);

    std::swap(a.size, b.size);
    std::swap(a.قدم, b.قدم);
}


فراغ سیٹ_ناپ( UMat& m, صحيح _dims, ثابت صحيح* _sz,
              ثابت size_t* قدماں_2, منطقي auto_قدماں )
{
    CV_Assert( 0 <= _dims && _dims <= CV_MAX_DIM && _dims <= MatShape::MAX_DIMS);

    m.dims = _dims;
    m.size = MatShape(_dims, _sz);
    m.قدم[std::max(_dims-1, 0)] = CV_ELEM_ناپ(m.flags);
    لكل (صحيح i = _dims-2; i >= 0; i--) {
        size_t autostep = m.size[i+1]*m.قدم[i+1];
        إذا (قدماں_2) {
            m.قدم[i] = قدماں_2[i];
            //CV_Assert(m.step[i] >= autostep);
        } وإلا إذا (auto_قدماں) {
            m.قدم[i] = autostep;
        } وإلا {
            m.قدم[i] = 0;
        }
    }

    إذا( _dims <= 2 )
    {
        m.خانے_گنتی = _dims == 0 ? 1 : _sz ? _sz[_dims > 1] : 0;
        m.صفاں = _dims < 2 ? 1 : _sz ? _sz[0] : 0;
    } وإلا {
        m.خانے_گنتی = m.صفاں = -1;
    }
}


فراغ UMat::updateContinuityFlag()
{
    flags = cv::updateContinuityFlag(flags, dims, size.p, قدم.p);
}


فراغ finalizeHdr(UMat& m)
{
    m.updateContinuityFlag();
    صحيح d = m.dims;
    إذا( d > 2 )
        m.صفاں = m.خانے_گنتی = -1;
}


UMat Mat::getUMat(AccessFlag accessFlags, UMatUsageFlags usageFlags) ثابت
{
    UMat hdr;
    إذا(!انکڑے)
        أرجع hdr;
    إذا (انکڑے != datastart)
    {
        ناپ سارا_ناپ;
        نقطہ ofs;
        locateROI(سارا_ناپ, ofs);
        ناپ sz(خانے_گنتی, صفاں);
        إذا (ofs.x != 0 || ofs.y != 0)
        {
            Mat src = *هذا;
            صحيح dtop = ofs.y;
            صحيح dbottom = سارا_ناپ.اچائی - src.صفاں - ofs.y;
            صحيح dleft = ofs.x;
            صحيح dright = سارا_ناپ.چوڑائی - src.خانے_گنتی - ofs.x;
            src.adjustROI(dtop, dbottom, dleft, dright);
            أرجع src.getUMat(accessFlags, usageFlags)(cv::Rect(ofs.x, ofs.y, sz.چوڑائی, sz.اچائی));
        }
    }
    CV_Assert(انکڑے == datastart);

    accessFlags |= ACCESS_RW;
    UMat_انکڑے* new_u = عدم;
    Mat_قدم new_قدم = قدم;
    {
        MatAllocator *a = allocator, *a0 = getDefaultAllocator();
        إذا(!a)
            a = a0;
        new_u = a->allocate(dims, size.p, type(), انکڑے, new_قدم.p, accessFlags, usageFlags);
        new_u->original_UMat_انکڑے = u;
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
#جے_تعریف_شدہ HAVE_OPENCL
        إذا (ocl::useOpenCL() && new_u->currAllocator == ocl::getOpenCLAllocator())
        {
            CV_Assert(new_u->عارضی_UMat());
        }
#جے_انت
        CV_XADD(&(u->refcount), 1);
        CV_XADD(&(u->urefcount), 1);
    }
    حاول
    {
        hdr.flags = flags;
        hdr.usageFlags = usageFlags;
        سیٹ_ناپ(hdr, dims, size.p, new_قدم.p);
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
    flags = (_type & CV_MAT_TYPE_نقاب) | MAGIC_VAL;
    سیٹ_ناپ(*هذا, d, _sizes, 0, سچ);
    offset = 0;

    إذا( کل() > 0 )
    {
        MatAllocator *a = allocator, *a0 = getStdAllocator();
        إذا (!a)
        {
            a = a0;
            a0 = Mat::getDefaultAllocator();
        }
        حاول
        {
            u = a->allocate(dims, size.p, _type, 0, قدم.p, ACCESS_RW /* ignored */, usageFlags);
            CV_Assert(u != 0);
        }
        امسك(...)
        {
            إذا(a != a0)
                u = a0->allocate(dims, size.p, _type, 0, قدم.p, ACCESS_RW /* ignored */, usageFlags);
            CV_Assert(u != 0);
        }
        CV_Assert( قدم[dims-1] == (size_t)CV_ELEM_ناپ(flags) );
    }

    finalizeHdr(*هذا);
    addref();
    size.dims = dims = d0;
}

فراغ UMat::create(ثابت std::vector<صحيح>& _sizes, صحيح _type, UMatUsageFlags _usageFlags)
{
    create((صحيح)_sizes.size(), _sizes.انکڑے(), _type, _usageFlags);
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
    size_t old_کل_بائٹاں = u ? u->size : 0;
    size_t esz = CV_ELEM_ناپ(_type), new_کل = _dims >= 0;
    لكل (صحيح i = 0; i < _dims; i++)
        new_کل *= _sizes[i];
    size_t new_کل_بائٹاں = new_کل*esz;
    إذا (new_کل_بائٹاں > 0 &&
        (!isContinuous() ||
         new_کل_بائٹاں > old_کل_بائٹاں ||
         offset != 0 ||
         _usageFlags != usageFlags)) {
        create(_dims, _sizes, _type, _usageFlags);
    } وإلا {
        flags = (flags & ~Mat::TYPE_نقاب) | CV_MAT_TYPE(_type);
        صحيح dummy_ناپ = 0;
        سیٹ_ناپ(*هذا, (_dims >= 0 ? _dims : 1), (_dims >= 0 ? _sizes : &dummy_ناپ), خالی_سوچک, سچ);
        finalizeHdr(*هذا);
    }
}

فراغ UMat::fit(ثابت std::vector<صحيح>& _shape, صحيح _type, UMatUsageFlags _usageFlags)
{
    fit((صحيح)_shape.size(), _shape.انکڑے(), _type, _usageFlags);
}

فراغ UMat::fit(ثابت MatShape& _shape, صحيح _type, UMatUsageFlags _usageFlags)
{
    fit(_shape.dims, _shape.p, _type, _usageFlags);
    size.layout = _shape.layout;
    size.C = _shape.C;
}

فراغ UMat::fit(صحيح صفاں_3, صحيح خانے_گنتی_3, صحيح _type, UMatUsageFlags _usageFlags)
{
    _type &= TYPE_نقاب;
    صحيح sz[] = {صفاں_3, خانے_گنتی_3};
    fit(2, sz, _type, _usageFlags);
}

فراغ UMat::fit(ناپ _sz, صحيح _type, UMatUsageFlags _usageFlags)
{
    fit(_sz.اچائی, _sz.چوڑائی, _type, _usageFlags);
}

فراغ UMat::fit_Same_ناپ(داخلہ_Array m, صحيح _type, UMatUsageFlags _usageFlags)
{
    fit(m.shape(), _type, _usageFlags);
}

فراغ UMat::copy_ناپ(ثابت UMat& m)
{
    dims = m.dims;
    خانے_گنتی = m.خانے_گنتی;
    صفاں = m.صفاں;
    size = m.size;
    قدم = m.قدم;
}

UMat::~UMat()
{
    release();
}

فراغ UMat::deallocate()
{
    UMat_انکڑے* u_ = u;
    u = عدم;
    u_->currAllocator->deallocate(u_);
}


UMat::UMat(ثابت UMat& m, ثابت Range& صف_Range_3, ثابت Range& خانہ_Range_3)
    : flags(MAGIC_VAL), dims(0), صفاں(0), خانے_گنتی(0), allocator(0), usageFlags(USAGE_DEFAULT), u(0), offset(0)
{
    CV_Assert( m.dims >= 2 && m.dims <= MatShape::MAX_DIMS );
    إذا( m.dims > 2 )
    {
        Range rs[MatShape::MAX_DIMS];
        rs[0] = صف_Range_3;
        rs[1] = خانہ_Range_3;
        لكل( صحيح i = 2; i < m.dims; i++ )
            rs[i] = Range::all();
        *هذا = m(rs);
        أرجع;
    }

    *هذا = m;
    إذا( صف_Range_3 != Range::all() && صف_Range_3 != Range(0,صفاں) )
    {
        CV_Assert( 0 <= صف_Range_3.شروع &&
                  صف_Range_3.شروع <= صف_Range_3.end &&
                  صف_Range_3.end <= m.صفاں );
        size[0] = صفاں = صف_Range_3.size();
        offset += قدم*صف_Range_3.شروع;
        flags |= SUBMATRIX_FLAG;
    }

    إذا( خانہ_Range_3 != Range::all() && خانہ_Range_3 != Range(0,خانے_گنتی) )
    {
        CV_Assert( 0 <= خانہ_Range_3.شروع &&
                  خانہ_Range_3.شروع <= خانہ_Range_3.end &&
                  خانہ_Range_3.end <= m.خانے_گنتی );
        size[1] = خانے_گنتی = خانہ_Range_3.size();
        offset += خانہ_Range_3.شروع*elem_ناپ();
        flags |= SUBMATRIX_FLAG;
    }

    updateContinuityFlag();

    إذا( صفاں <= 0 || خانے_گنتی <= 0 )
    {
        release();
        صفاں = خانے_گنتی = 0;
    }
}


UMat::UMat(ثابت UMat& m, ثابت Rect& roi)
    : flags(m.flags), dims(2), صفاں(roi.اچائی), خانے_گنتی(roi.چوڑائی),
    allocator(m.allocator), usageFlags(m.usageFlags), u(m.u),
    offset(m.offset + roi.y*m.قدم[0]), size(2)
{
    CV_Assert( m.dims <= 2 );

    size_t esz = CV_ELEM_ناپ(flags);
    offset += roi.x*esz;
    CV_Assert( 0 <= roi.x && 0 <= roi.چوڑائی && roi.x + roi.چوڑائی <= m.خانے_گنتی &&
              0 <= roi.y && 0 <= roi.اچائی && roi.y + roi.اچائی <= m.صفاں );
    إذا( roi.چوڑائی < m.خانے_گنتی || roi.اچائی < m.صفاں )
        flags |= SUBMATRIX_FLAG;

    size[0] = صفاں;
    size[1] = خانے_گنتی;
    قدم[0] = m.dims == 2 ? m.قدم[0] : خانے_گنتی*esz;
    قدم[1] = esz;

    updateContinuityFlag();

    addref();
    إذا( صفاں <= 0 || خانے_گنتی <= 0 )
    {
        صفاں = خانے_گنتی = 0;
        release();
    }
}


UMat::UMat(ثابت UMat& m, ثابت Range* ranges)
    : flags(MAGIC_VAL), dims(0), صفاں(0), خانے_گنتی(0), allocator(0), usageFlags(USAGE_DEFAULT), u(0), offset(0)
{
    صحيح i, d = m.dims;

    CV_Assert(ranges);
    لكل( i = 0; i < d; i++ )
    {
        Range r = ranges[i];
        CV_Assert( r == Range::all() || (0 <= r.شروع && r.شروع < r.end && r.end <= m.size[i]) );
    }
    *هذا = m;
    لكل( i = 0; i < d; i++ )
    {
        Range r = ranges[i];
        إذا( r != Range::all() && r != Range(0, size.p[i]))
        {
            size.p[i] = r.end - r.شروع;
            offset += r.شروع*قدم.p[i];
            flags |= SUBMATRIX_FLAG;
        }
    }

    إذا (d <= 2) {
        صفاں = d == 2 ? size[0] : 1;
        خانے_گنتی = d <= 0 ? (d >= 0) : size[d > 1];
    }
    updateContinuityFlag();
}

UMat::UMat(ثابت UMat& m, ثابت std::vector<Range>& ranges)
    : flags(MAGIC_VAL), dims(0), صفاں(0), خانے_گنتی(0), allocator(0), usageFlags(USAGE_DEFAULT), u(0), offset(0)
{
    صحيح i, d = m.dims;

    CV_Assert((صحيح)ranges.size() == d);
    لكل (i = 0; i < d; i++)
    {
        Range r = ranges[i];
        CV_Assert(r == Range::all() || (0 <= r.شروع && r.شروع < r.end && r.end <= m.size[i]));
    }
    *هذا = m;
    لكل (i = 0; i < d; i++)
    {
        Range r = ranges[i];
        إذا (r != Range::all() && r != Range(0, size.p[i]))
        {
            size.p[i] = r.end - r.شروع;
            offset += r.شروع*قدم.p[i];
            flags |= SUBMATRIX_FLAG;
        }
    }

    إذا (d <= 2) {
        صفاں = d == 2 ? size[0] : 1;
        خانے_گنتی = d <= 0 ? (d >= 0) : size[d > 1];
    }
    updateContinuityFlag();
}

UMat UMat::diag(صحيح d) ثابت
{
    CV_Assert( dims <= 2 );
    UMat m = *هذا;
    size_t esz = elem_ناپ();
    صحيح len;

    إذا( d >= 0 )
    {
        len = std::min(خانے_گنتی - d, صفاں);
        m.offset += esz*d;
    }
    وإلا
    {
        len = std::min(صفاں + d, خانے_گنتی);
        m.offset -= قدم[0]*d;
    }
    CV_DbgAssert( len > 0 );

    m.size[0] = m.صفاں = len;
    m.size[1] = m.خانے_گنتی = 1;
    m.قدم[0] += (len > 1 ? esz : 0);

    m.updateContinuityFlag();

    إذا( size() != ناپ(1,1) )
        m.flags |= SUBMATRIX_FLAG;

    أرجع m;
}

فراغ UMat::locateROI( ناپ& سارا_ناپ, نقطہ& ofs ) ثابت
{
    CV_Assert( dims <= 2 && قدم[0] > 0 );
    size_t esz = elem_ناپ(), minstep;
    ptrdiff_t delta1 = (ptrdiff_t)offset, delta2 = (ptrdiff_t)u->size;

    إذا( delta1 == 0 )
        ofs.x = ofs.y = 0;
    وإلا
    {
        ofs.y = (صحيح)(delta1/قدم[0]);
        ofs.x = (صحيح)((delta1 - قدم[0]*ofs.y)/esz);
        CV_DbgAssert( offset == (size_t)(ofs.y*قدم[0] + ofs.x*esz) );
    }
    minstep = (ofs.x + خانے_گنتی)*esz;
    سارا_ناپ.اچائی = (صحيح)((delta2 - minstep)/قدم[0] + 1);
    سارا_ناپ.اچائی = std::max(سارا_ناپ.اچائی, ofs.y + صفاں);
    سارا_ناپ.چوڑائی = (صحيح)((delta2 - قدم*(سارا_ناپ.اچائی-1))/esz);
    سارا_ناپ.چوڑائی = std::max(سارا_ناپ.چوڑائی, ofs.x + خانے_گنتی);
}


UMat& UMat::adjustROI( صحيح dtop, صحيح dbottom, صحيح dleft, صحيح dright )
{
    CV_Assert( dims == 2 && قدم[0] > 0 );
    ناپ سارا_ناپ; نقطہ ofs;
    size_t esz = elem_ناپ();
    locateROI( سارا_ناپ, ofs );
    صحيح row1 = std::min(std::max(ofs.y - dtop, 0), سارا_ناپ.اچائی), row2 = std::max(0, std::min(ofs.y + صفاں + dbottom, سارا_ناپ.اچائی));
    صحيح col1 = std::min(std::max(ofs.x - dleft, 0), سارا_ناپ.چوڑائی), col2 = std::max(0, std::min(ofs.x + خانے_گنتی + dright, سارا_ناپ.چوڑائی));
    إذا(row1 > row2)
        std::swap(row1, row2);
    إذا(col1 > col2)
        std::swap(col1, col2);

    offset += (row1 - ofs.y)*قدم + (col1 - ofs.x)*esz;
    صفاں = row2 - row1; خانے_گنتی = col2 - col1;
    size.p[0] = صفاں; size.p[1] = خانے_گنتی;
    updateContinuityFlag();
    أرجع *هذا;
}


UMat UMat::reshape(صحيح new_cn, صحيح new_صفاں) ثابت
{
    صحيح cn = channels();
    UMat hdr = *هذا;

    إذا( dims > 2 && new_صفاں == 0 && new_cn != 0 && size[dims-1]*cn % new_cn == 0 )
    {
        hdr.flags = (hdr.flags & ~CV_MAT_CN_نقاب) | ((new_cn-1) << CV_CN_کھسکاؤ);
        hdr.قدم[dims-1] = CV_ELEM_ناپ(hdr.flags);
        hdr.size[dims-1] = hdr.size[dims-1]*cn / new_cn;
        أرجع hdr;
    }

    CV_Assert( dims <= 2 );

    إذا( new_cn == 0 )
        new_cn = cn;

    صحيح کل_چوڑائی = خانے_گنتی * cn;

    إذا( (new_cn > کل_چوڑائی || کل_چوڑائی % new_cn != 0) && new_صفاں == 0 )
        new_صفاں = صفاں * کل_چوڑائی / new_cn;

    إذا( new_صفاں != 0 && new_صفاں != صفاں )
    {
        صحيح کل_ناپ_2 = کل_چوڑائی * صفاں;
        إذا( !isContinuous() )
            CV_Error( cv::Error::Bad_قدم,
            "The matrix is not continuous, thus its number of rows can not be changed" );

        کل_چوڑائی = کل_ناپ_2 / new_صفاں;

        إذا( کل_چوڑائی * new_صفاں != کل_ناپ_2 )
            CV_Error( cv::Error::StsBadArg, "The total number of matrix elements "
                                    "is not divisible by the new number of rows" );

        hdr.size[0] = hdr.صفاں = new_صفاں;
        hdr.قدم[0] = کل_چوڑائی * elemSize1();
    } وإلا {
        hdr.size[0] = hdr.صفاں = صفاں;
        إذا (dims <= 1)
            hdr.قدم[0] = خانے_گنتی * CV_ELEM_ناپ(flags);
    }

    صحيح new_چوڑائی = کل_چوڑائی / new_cn;

    إذا( new_چوڑائی * new_cn != کل_چوڑائی )
        CV_Error( cv::Error::Bad_عدد_Channels,
        "The total width is not divisible by the new number of channels" );

    hdr.size.dims = hdr.dims = 2;
    hdr.size[1] = hdr.خانے_گنتی = new_چوڑائی;
    hdr.flags = (hdr.flags & ~CV_MAT_CN_نقاب) | ((new_cn-1) << CV_CN_کھسکاؤ);
    hdr.قدم[1] = CV_ELEM_ناپ(hdr.flags);
    أرجع hdr;
}

#جے_تعریف_شدہ HAVE_OPENCL
نطاق {
ساكن منطقي ocl_سیٹ_Diag(ثابت UMat& d, UMat& m, صحيح len)
{
    صحيح cn = d.channels();
    صحيح ڈونگھائی = d.ڈونگھائی();

    إذا (ڈونگھائی == CV_64F && !ocl::Device::getDefault().doubleFPConfig())
        أرجع خطأ;

    String opts = format("-D SET_DIAG -D T1=%s -D cn=%d -D IS_ROW_VECTOR=%d",
                         ocl::memop_Type_منزل_Str(ڈونگھائی),
                         cn,
                         (d.صفاں == 1) ? 1 : 0);

    ocl::Kernel k("setDiag", ocl::core::copyset_oclsrc, opts);
    إذا (k.empty())
        أرجع خطأ;

    k.دلیلاں(ocl::KernelArg::WriteOnly(m),
           ocl::KernelArg::Read_Only_No_ناپ(d),
           len);

    size_t globalsize[2] = { (size_t)len, (size_t)len };
    أرجع k.لگاتار(2, globalsize, عدم, خطأ);
}
}
#جے_انت

UMat UMat::diag(ثابت UMat& d, UMatUsageFlags usageFlags)
{
    CV_INSTRUMENT_REGION();

    CV_Assert(d.خانے_گنتی == 1 || d.صفاں == 1);
    صحيح len = d.صفاں + d.خانے_گنتی - 1;

#جے_تعریف_شدہ HAVE_OPENCL
    إذا (ocl::useOpenCL())
    {
        UMat m(len, len, d.type(), usageFlags);
        إذا (ocl_سیٹ_Diag(d, m, len))
        {
            CV_IMPL_جوڑو(CV_IMPL_OCL);
            أرجع m;
        }
    }
#جے_انت

    UMat m(len, len, d.type(), Scalar(0), usageFlags);
    UMat md = m.diag();
    إذا( d.خانے_گنتی == 1 )
        d.copy_منزل(md);
    وإلا
        transpose(d, md);
    أرجع m;
}

صحيح UMat::check_سمتیہ(صحيح _elemChannels, صحيح ڈونگھائی_2, منطقي _requireContinuous) ثابت
{
    أرجع (ڈونگھائی() == ڈونگھائی_2 || ڈونگھائی_2 <= 0) &&
        (isContinuous() || !_requireContinuous) &&
        ((dims <= 2 && (((صفاں == 1 || خانے_گنتی == 1) && channels() == _elemChannels) ||
                        (خانے_گنتی == _elemChannels && channels() == 1))) ||
        (dims == 3 && channels() == 1 && size.p[2] == _elemChannels && (size.p[0] == 1 || size.p[1] == 1) &&
         (isContinuous() || قدم.p[1] == قدم.p[2]*size.p[2])))
    ? (صحيح)(کل()*channels()/_elemChannels) : -1;
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

        size_t کل_elem1_ref = هذا->کل() * هذا->channels();
        size_t کل_elem1 = _cn;

        Auto_بفر<صحيح, 4> newsz_buf( (size_t)std::max(_newndims, 1) );

        صحيح m1_idx = -1;

        لكل (صحيح i = 0; i < _newndims; i++)
        {
            إذا (_newsz[i] >= 0) {
                إذا (_newsz[i] == 0 && i < dims)
                    newsz_buf[i] = size.p[i];
                وإلا
                    newsz_buf[i] = _newsz[i];
                کل_elem1 *= (size_t)newsz_buf[i];
            } وإلا {
                إذا (m1_idx >= 0)
                    CV_Error(cv::Error::Sts_Bad_ناپ, "More than one '-1' occured in the new shape");
                m1_idx = i;
            }
        }

        إذا (m1_idx >= 0) {
            إذا (کل_elem1 == 0) {
                CV_Assert(کل_elem1_ref == 0);
                کل_elem1 = 1;
            }
            CV_Assert(کل_elem1_ref % کل_elem1 == 0);
            newsz_buf[m1_idx] = (صحيح)(کل_elem1_ref / کل_elem1);
        } وإلا إذا (کل_elem1 != کل_elem1_ref) {
            CV_Error(cv::Error::StsUnmatchedSizes, "Requested and source matrices have different count of elements");
        }

        UMat hdr = *هذا;
        hdr.flags = (hdr.flags & ~CV_MAT_CN_نقاب) | ((_cn-1) << CV_CN_کھسکاؤ);
        سیٹ_ناپ(hdr, _newndims, newsz_buf.انکڑے(), عدم, سچ);

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
    UMat_انکڑے_Auto_جندرا autolock(u);
    حاول
    {
        إذا(CV_XADD(&u->refcount, 1) == 0)
            u->currAllocator->map(u, accessFlags);
        إذا (u->انکڑے != 0)
        {
            Mat hdr(dims, size.p, type(), u->انکڑے + offset, قدم.p);
            hdr.flags = flags;
            hdr.u = u;
            hdr.datastart = u->انکڑے;
            hdr.انکڑے = u->انکڑے + offset;
            hdr.datalimit = hdr.dataend = u->انکڑے + u->size;
            أرجع hdr;
        }
    }
    امسك(...)
    {
        CV_XADD(&u->refcount, -1);
        ارم;
    }
    CV_XADD(&u->refcount, -1);
    CV_Assert(u->انکڑے != 0 && "Error mapping of UMat to host memory.");
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
        u->markHostCopyObsolete(سچ);

    أرجع u->handle;
}

فراغ UMat::ndoffset(size_t* ofs) ثابت
{
    // offset = step[0]*ofs[0] + step[1]*ofs[1] + step[2]*ofs[2] + ...;
    size_t val = offset;
    لكل( صحيح i = 0; i < dims; i++ )
    {
        size_t s = قدم.p[i];
        ofs[i] = val / s;
        val -= ofs[i]*s;
    }
}

فراغ UMat::copy_منزل(OutputArray _dst) ثابت
{
    CV_INSTRUMENT_REGION();

#جے_تعریف_شدہ HAVE_CUDA
    إذا (_dst.isGpuMat())
    {
        _dst.getGpuMat().upload(*هذا);
        أرجع;
    }
#جے_انت

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
    size_t esz = CV_ELEM_ناپ(stype);
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
            u->currAllocator->copy(u, dst.u, d, sz, srcofs, قدم.p, dstofs, dst.قدم.p, خطأ);
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
    u->currAllocator->download(u, dst.ptr(), d, sz, srcofs, قدم.p, dst.قدم.p);
}

فراغ UMat::copy_منزل(OutputArray _dst, داخلہ_Array نقاب_2) ثابت
{
    CV_INSTRUMENT_REGION();

    إذا( نقاب_2.empty() )
    {
        copy_منزل(_dst);
        أرجع;
    }
#جے_تعریف_شدہ HAVE_OPENCL
    صحيح cn = channels(), mtype = نقاب_2.type(), mdepth = CV_MAT_ڈونگھائی(mtype), mcn = CV_MAT_CN(mtype);
    CV_Assert( (mdepth == CV_8U || mdepth == CV_8S || mdepth == CV_Bool) && (mcn == 1 || mcn == cn) );

    إذا (ocl::useOpenCL() && _dst.isUMat() && dims <= 2)
    {
        UMat_انکڑے * prevu = _dst.getUMat().u;
        _dst.create( size, type() );

        UMat dst = _dst.getUMat();

        منطقي haveDstUninit = خطأ;
        إذا( prevu != dst.u ) // do not leave dst uninitialized
            haveDstUninit = سچ;

        String opts = format("-D COPY_TO_MASK -D T1=%s -D scn=%d -D mcn=%d%s",
                             ocl::memop_Type_منزل_Str(ڈونگھائی()), cn, mcn,
                             haveDstUninit ? " -D HAVE_DST_UNINIT" : "");

        ocl::Kernel k("copyToMask", ocl::core::copyset_oclsrc, opts);
        إذا (!k.empty())
        {
            k.دلیلاں(ocl::KernelArg::Read_Only_No_ناپ(*هذا),
                   ocl::KernelArg::Read_Only_No_ناپ(نقاب_2.getUMat()),
                   haveDstUninit ? ocl::KernelArg::WriteOnly(dst) :
                                   ocl::KernelArg::ReadWrite(dst));

            size_t globalsize[2] = { (size_t)خانے_گنتی, (size_t)صفاں };
            إذا (k.لگاتار(2, globalsize, عدم, خطأ))
            {
                CV_IMPL_جوڑو(CV_IMPL_OCL);
                أرجع;
            }
        }
    }
#جے_انت
    Mat src = getMat(ACCESS_READ);
    src.copy_منزل(_dst, نقاب_2);
}


//
// void UMat::convertTo moved to convert.dispatch.cpp
//

UMat& UMat::سیٹ_منزل(داخلہ_Array قدر, داخلہ_Array نقاب_2)
{
    CV_INSTRUMENT_REGION();

    منطقي have_نقاب = !نقاب_2.empty();
#جے_تعریف_شدہ HAVE_OPENCL
    صحيح tp = type(), cn = CV_MAT_CN(tp), d = CV_MAT_ڈونگھائی(tp);

    إذا( dims <= 2 && cn <= 4 && CV_MAT_ڈونگھائی(tp) < CV_64F && ocl::useOpenCL() )
    {
        Mat value = قدر.getMat();
        CV_Assert( checkScalar(value, type(), قدر.قسم(), داخلہ_Array_2::UMAT) );
        صحيح kercn = have_نقاب || cn == 3 ? cn : std::max(cn, ocl::predict_Optimal_سمتیہ_چوڑائی(*هذا)),
                kertp = CV_MAKE_TYPE(d, kercn);

        مزدوج buf[16] = { 0, 0, 0, 0, 0, 0, 0, 0,
                           0, 0, 0, 0, 0, 0, 0, 0 };
        convertAndUnrollScalar(value, tp, (uchar *)buf, kercn / cn);

        صحيح scalarcn = kercn == 3 ? 4 : kercn, صفاں_فی_WI = ocl::Device::getDefault().isIntel() ? 4 : 1;
        String opts = format("-D dstT=%s -D rowsPerWI=%d -D dstST=%s -D dstT1=%s -D cn=%d",
                             ocl::memop_Type_منزل_Str(kertp), صفاں_فی_WI,
                             ocl::memop_Type_منزل_Str(CV_MAKETYPE(d, scalarcn)),
                             ocl::memop_Type_منزل_Str(d), kercn);

        ocl::Kernel سیٹ_K(have_نقاب ? "setMask" : "set", ocl::core::copyset_oclsrc, opts);
        إذا( !سیٹ_K.empty() )
        {
            ocl::KernelArg scalararg(ocl::KernelArg::CONSTANT, 0, 0, 0, buf, CV_ELEM_ناپ(d) * scalarcn);
            UMat نقاب;

            إذا( have_نقاب )
            {
                نقاب = نقاب_2.getUMat();
                CV_Assert( نقاب.size() == size() && (نقاب.type() == CV_8U || نقاب.type() == CV_8S || نقاب.type() == CV_Bool) );
                ocl::KernelArg maskarg = ocl::KernelArg::Read_Only_No_ناپ(نقاب),
                        dstarg = ocl::KernelArg::ReadWrite(*هذا);
                سیٹ_K.دلیلاں(maskarg, dstarg, scalararg);
            }
            وإلا
            {
                ocl::KernelArg dstarg = ocl::KernelArg::WriteOnly(*هذا, cn, kercn);
                سیٹ_K.دلیلاں(dstarg, scalararg);
            }

            size_t globalsize[] = { (size_t)خانے_گنتی * cn / kercn, ((size_t)صفاں + صفاں_فی_WI - 1) / صفاں_فی_WI };
            إذا( سیٹ_K.لگاتار(2, globalsize, عدم, خطأ) )
            {
                CV_IMPL_جوڑو(CV_IMPL_OCL);
                أرجع *هذا;
            }
        }
    }
#جے_انت
    Mat m = getMat(have_نقاب ? ACCESS_RW : ACCESS_WRITE);
    m.سیٹ_منزل(قدر, نقاب_2);
    أرجع *هذا;
}

UMat& UMat::عامل = (ثابت Scalar& s)
{
    سیٹ_منزل(s);
    أرجع *هذا;
}

UMat UMat::t() ثابت
{
    UMat m;
    transpose(*هذا, m);
    أرجع m;
}

UMat UMat::zeros(صحيح صفاں, صحيح خانے_گنتی, صحيح type, UMatUsageFlags usageFlags)
{
    أرجع UMat(صفاں, خانے_گنتی, type, Scalar::all(0), usageFlags);
}

UMat UMat::zeros(ناپ size, صحيح type, UMatUsageFlags usageFlags)
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

UMat UMat::ones(صحيح صفاں, صحيح خانے_گنتی, صحيح type, UMatUsageFlags usageFlags)
{
    أرجع UMat(صفاں, خانے_گنتی, type, Scalar(1), usageFlags);
}

UMat UMat::ones(ناپ size, صحيح type, UMatUsageFlags usageFlags)
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
