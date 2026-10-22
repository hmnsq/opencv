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
فراغ سيٽ_سائيز(UMat& m, صحيح _dims, ثابت صحيح* _sz, ثابت size_t* قدمن_2,
             منطقي auto_قدمن = خطأ);

فراغ updateContinuityFlag(UMat& m);
فراغ finalizeHdr(UMat& m);

UMat_معلومات::UMat_معلومات(ثابت MatAllocator* allocator)
{
    prevAllocator = currAllocator = allocator;
    urefcount = refcount = mapcount = 0;
    معلومات = origdata = 0;
    size = 0;
    flags = static_cast<UMat_معلومات::يادگيري_Flag>(0);
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
    flags = static_cast<UMat_معلومات::يادگيري_Flag>(0);
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
            showWarn = سچ;
        إذا (zero_Ref && zero_URef) // oops, we need to free resources
        {
            showWarn = !isAsyncCleanup;
            // simulate UMat::deallocate
            u->currAllocator->deallocate(u);
        }
#جيڪڏهن_وصف_نه_ٿيل NDEBUG
        إذا (showWarn)
        {
            ساكن صحيح warn_پيغام_showed = 0;
            إذا (warn_پيغام_showed++ < 100)
            {
                fflush(stdout);
                fprintf(stderr, "\n! OPENCV warning: getUMat()/getMat() call chain possible problem."
                                "\n!                 Base object is dead, while nested/derived object is still alive or processed."
                                "\n!                 Please check lifetime of UMat/Mat objects!\n");
                fflush(stderr);
            }
        }
#وإلا
        CV_اڻ_استعمال_ٿيل(showWarn);
#جيڪڏهن_پورو
        original_UMat_معلومات = عدم;
    }
}

#جيڪڏهن_وصف_نه_ٿيل OPENCV_DISABLE_THREAD_SUPPORT

// it should be a prime number for the best hash function
تعداد { UMAT_NLOCKS = 31 };
ساكن Mutex umatLocks[UMAT_NLOCKS];

ساكن size_t get_UMat_معلومات_تالو_اشاريو(ثابت UMat_معلومات* u)
{
    size_t idx = ((size_t)(فراغ*)u) % UMAT_NLOCKS;
    أرجع idx;
}

فراغ UMat_معلومات::lock()
{
    size_t idx = get_UMat_معلومات_تالو_اشاريو(هذا);
    //printf("%d lock(%d)\n", cv::utils::getThreadID(), (int)idx);
    umatLocks[idx].lock();
}

فراغ UMat_معلومات::unlock()
{
    size_t idx = get_UMat_معلومات_تالو_اشاريو(هذا);
    //printf("%d unlock(%d)\n", cv::utils::getThreadID(), (int)idx);
    umatLocks[idx].unlock();
}


// Do not allow several lock() calls with different UMatData objects.
بنية UMat_معلومات_Auto_Locker
{
    صحيح usage_ڳڻپ;
    UMat_معلومات* locked_objects[2];
    UMat_معلومات_Auto_Locker() : usage_ڳڻپ(0) { locked_objects[0] = عدم; locked_objects[1] = عدم; }

    فراغ lock(UMat_معلومات*& u1)
    {
        منطقي locked_1 = (u1 == locked_objects[0] || u1 == locked_objects[1]);
        إذا (locked_1)
        {
            u1 = عدم;
            أرجع;
        }
        CV_Assert(usage_ڳڻپ == 0);  // UMatDataAutoLock can't be used multiple times from the same thread
        usage_ڳڻپ = 1;
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
        CV_Assert(usage_ڳڻپ == 0);  // UMatDataAutoLock can't be used multiple times from the same thread
        usage_ڳڻپ = 1;
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
        CV_Assert(usage_ڳڻپ == 1);
        usage_ڳڻپ = 0;
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


UMat_معلومات_Auto_تالو::UMat_معلومات_Auto_تالو(UMat_معلومات* u) : u1(u), u2(عدم)
{
    get_UMat_معلومات_Auto_Locker().lock(u1);
}
UMat_معلومات_Auto_تالو::UMat_معلومات_Auto_تالو(UMat_معلومات* u1_, UMat_معلومات* u2_) : u1(u1_), u2(u2_)
{
    إذا (get_UMat_معلومات_تالو_اشاريو(u1) > get_UMat_معلومات_تالو_اشاريو(u2))
    {
        std::swap(u1, u2);
    }
    get_UMat_معلومات_Auto_Locker().lock(u1, u2);
}
UMat_معلومات_Auto_تالو::~UMat_معلومات_Auto_تالو()
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

UMat_معلومات_Auto_تالو::UMat_معلومات_Auto_تالو(UMat_معلومات* u) : u1(u), u2(عدم)
{
    // nothing in OPENCV_DISABLE_THREAD_SUPPORT mode
}
UMat_معلومات_Auto_تالو::UMat_معلومات_Auto_تالو(UMat_معلومات* u1_, UMat_معلومات* u2_) : u1(u1_), u2(u2_)
{
    // nothing in OPENCV_DISABLE_THREAD_SUPPORT mode
}
UMat_معلومات_Auto_تالو::~UMat_معلومات_Auto_تالو()
{
    // nothing in OPENCV_DISABLE_THREAD_SUPPORT mode
}

#جيڪڏهن_پورو  // OPENCV_DISABLE_THREAD_SUPPORT

//////////////////////////////// UMat ////////////////////////////////

UMat::UMat(UMatUsageFlags _usageFlags) CV_NOEXCEPT
: flags(MAGIC_VAL), dims(0), صفون(0), کانن_ڳڻپ(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{}

UMat::UMat(صحيح صفون_3, صحيح کانن_ڳڻپ_3, صحيح _type, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), صفون(0), کانن_ڳڻپ(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(صفون_3, کانن_ڳڻپ_3, _type);
}

UMat::UMat(صحيح صفون_3, صحيح کانن_ڳڻپ_3, صحيح _type, ثابت Scalar& _s, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), صفون(0), کانن_ڳڻپ(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(صفون_3, کانن_ڳڻپ_3, _type);
    *هذا = _s;
}

UMat::UMat(سائيز _sz, صحيح _type, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), صفون(0), کانن_ڳڻپ(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create( _sz.اوچائي, _sz.ويڪر, _type );
}

UMat::UMat(سائيز _sz, صحيح _type, ثابت Scalar& _s, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), صفون(0), کانن_ڳڻپ(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_sz.اوچائي, _sz.ويڪر, _type);
    *هذا = _s;
}

UMat::UMat(صحيح _dims, ثابت صحيح* _sz, صحيح _type, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), صفون(0), کانن_ڳڻپ(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_dims, _sz, _type);
}

UMat::UMat(صحيح _dims, ثابت صحيح* _sz, صحيح _type, ثابت Scalar& _s, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), صفون(0), کانن_ڳڻپ(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_dims, _sz, _type);
    *هذا = _s;
}

UMat::UMat(ثابت MatShape& _shape, صحيح _type, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), صفون(0), کانن_ڳڻپ(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_shape.dims, _shape.p, _type);
}

UMat::UMat(ثابت MatShape& _shape, صحيح _type, ثابت Scalar& _s, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), صفون(0), کانن_ڳڻپ(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_shape.dims, _shape.p, _type);
    *هذا = _s;
}

UMat::UMat(ثابت UMat& m)
: flags(m.flags), dims(m.dims), صفون(m.صفون), کانن_ڳڻپ(m.کانن_ڳڻپ), allocator(m.allocator),
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
        صفون = m.صفون;
        کانن_ڳڻپ = m.کانن_ڳڻپ;
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

فراغ UMat::سونپيو_منزل(UMat& m, صحيح _type) ثابت
{
    إذا( _type < 0 )
        m = *هذا;
    وإلا
        convert_منزل(m, _type);
}

فراغ UMat::create(صحيح صفون_3, صحيح کانن_ڳڻپ_3, صحيح _type, UMatUsageFlags _usageFlags)
{
    صحيح sz[] = {صفون_3, کانن_ڳڻپ_3};
    create(2, sz, _type, _usageFlags);
}

فراغ UMat::create(سائيز _sz, صحيح _type, UMatUsageFlags _usageFlags)
{
    create(_sz.اوچائي, _sz.ويڪر, _type, _usageFlags);
}

فراغ UMat::create_Same_سائيز(داخلا_Array arr, صحيح _type, UMatUsageFlags _usageFlags)
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
    dims = کانن_ڳڻپ = صفون = 0;
}

منطقي UMat::empty() ثابت
{
    أرجع u == 0 || ڪل() == 0;
}

size_t UMat::ڪل() ثابت
{
    إذا( dims <= 2 )
        أرجع (size_t)صفون * کانن_ڳڻپ;
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
: flags(m.flags), dims(m.dims), صفون(m.صفون), کانن_ڳڻپ(m.کانن_ڳڻپ), allocator(m.allocator),
  usageFlags(m.usageFlags), u(m.u), offset(m.offset)
{
    size = m.size;
    قدم = m.قدم;
    m.flags = MAGIC_VAL;
    m.usageFlags = USAGE_DEFAULT;
    m.dims = m.صفون = m.کانن_ڳڻپ = 0;
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
    flags = m.flags; dims = m.dims; صفون = m.صفون; کانن_ڳڻپ = m.کانن_ڳڻپ;
    allocator = m.allocator; usageFlags = m.usageFlags;
    u = m.u;
    offset = m.offset;
    size = m.size;
    قدم = m.قدم;
    m.flags = MAGIC_VAL;
    m.usageFlags = USAGE_DEFAULT;
    m.dims = m.صفون = m.کانن_ڳڻپ = 0;
    m.allocator = عدم;
    m.u = عدم;
    m.offset = 0;
    أرجع *هذا;
}


MatAllocator* UMat::getStdAllocator()
{
#جيڪڏهن_وصف_ٿيل HAVE_OPENCL
    إذا (ocl::useOpenCL())
        أرجع ocl::getOpenCLAllocator();
#جيڪڏهن_پورو
    أرجع Mat::getDefaultAllocator();
}

فراغ swap( UMat& a, UMat& b )
{
    std::swap(a.flags, b.flags);
    std::swap(a.dims, b.dims);
    std::swap(a.صفون, b.صفون);
    std::swap(a.کانن_ڳڻپ, b.کانن_ڳڻپ);
    std::swap(a.allocator, b.allocator);
    std::swap(a.u, b.u);
    std::swap(a.offset, b.offset);

    std::swap(a.size, b.size);
    std::swap(a.قدم, b.قدم);
}


فراغ سيٽ_سائيز( UMat& m, صحيح _dims, ثابت صحيح* _sz,
              ثابت size_t* قدمن_2, منطقي auto_قدمن )
{
    CV_Assert( 0 <= _dims && _dims <= CV_MAX_DIM && _dims <= MatShape::MAX_DIMS);

    m.dims = _dims;
    m.size = MatShape(_dims, _sz);
    m.قدم[std::max(_dims-1, 0)] = CV_ELEM_سائيز(m.flags);
    لكل (صحيح i = _dims-2; i >= 0; i--) {
        size_t autostep = m.size[i+1]*m.قدم[i+1];
        إذا (قدمن_2) {
            m.قدم[i] = قدمن_2[i];
            //CV_Assert(m.step[i] >= autostep);
        } وإلا إذا (auto_قدمن) {
            m.قدم[i] = autostep;
        } وإلا {
            m.قدم[i] = 0;
        }
    }

    إذا( _dims <= 2 )
    {
        m.کانن_ڳڻپ = _dims == 0 ? 1 : _sz ? _sz[_dims > 1] : 0;
        m.صفون = _dims < 2 ? 1 : _sz ? _sz[0] : 0;
    } وإلا {
        m.کانن_ڳڻپ = m.صفون = -1;
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
        m.صفون = m.کانن_ڳڻپ = -1;
}


UMat Mat::getUMat(AccessFlag accessFlags, UMatUsageFlags usageFlags) ثابت
{
    UMat hdr;
    إذا(!معلومات)
        أرجع hdr;
    إذا (معلومات != datastart)
    {
        سائيز سڄو_سائيز;
        نقطو ofs;
        locateROI(سڄو_سائيز, ofs);
        سائيز sz(کانن_ڳڻپ, صفون);
        إذا (ofs.x != 0 || ofs.y != 0)
        {
            Mat src = *هذا;
            صحيح dtop = ofs.y;
            صحيح dbottom = سڄو_سائيز.اوچائي - src.صفون - ofs.y;
            صحيح dleft = ofs.x;
            صحيح dright = سڄو_سائيز.ويڪر - src.کانن_ڳڻپ - ofs.x;
            src.adjustROI(dtop, dbottom, dleft, dright);
            أرجع src.getUMat(accessFlags, usageFlags)(cv::Rect(ofs.x, ofs.y, sz.ويڪر, sz.اوچائي));
        }
    }
    CV_Assert(معلومات == datastart);

    accessFlags |= ACCESS_RW;
    UMat_معلومات* new_u = عدم;
    Mat_قدم new_قدم = قدم;
    {
        MatAllocator *a = allocator, *a0 = getDefaultAllocator();
        إذا(!a)
            a = a0;
        new_u = a->allocate(dims, size.p, type(), معلومات, new_قدم.p, accessFlags, usageFlags);
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
#جيڪڏهن_وصف_ٿيل HAVE_OPENCL
        إذا (ocl::useOpenCL() && new_u->currAllocator == ocl::getOpenCLAllocator())
        {
            CV_Assert(new_u->عارضي_UMat());
        }
#جيڪڏهن_پورو
        CV_XADD(&(u->refcount), 1);
        CV_XADD(&(u->urefcount), 1);
    }
    حاول
    {
        hdr.flags = flags;
        hdr.usageFlags = usageFlags;
        سيٽ_سائيز(hdr, dims, size.p, new_قدم.p);
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
    سيٽ_سائيز(*هذا, d, _sizes, 0, سچ);
    offset = 0;

    إذا( ڪل() > 0 )
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
        CV_Assert( قدم[dims-1] == (size_t)CV_ELEM_سائيز(flags) );
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
    size_t old_ڪل_بائيٽون = u ? u->size : 0;
    size_t esz = CV_ELEM_سائيز(_type), new_ڪل = _dims >= 0;
    لكل (صحيح i = 0; i < _dims; i++)
        new_ڪل *= _sizes[i];
    size_t new_ڪل_بائيٽون = new_ڪل*esz;
    إذا (new_ڪل_بائيٽون > 0 &&
        (!isContinuous() ||
         new_ڪل_بائيٽون > old_ڪل_بائيٽون ||
         offset != 0 ||
         _usageFlags != usageFlags)) {
        create(_dims, _sizes, _type, _usageFlags);
    } وإلا {
        flags = (flags & ~Mat::TYPE_نقاب) | CV_MAT_TYPE(_type);
        صحيح dummy_سائيز = 0;
        سيٽ_سائيز(*هذا, (_dims >= 0 ? _dims : 1), (_dims >= 0 ? _sizes : &dummy_سائيز), خالي_اشارو, سچ);
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

فراغ UMat::fit(صحيح صفون_3, صحيح کانن_ڳڻپ_3, صحيح _type, UMatUsageFlags _usageFlags)
{
    _type &= TYPE_نقاب;
    صحيح sz[] = {صفون_3, کانن_ڳڻپ_3};
    fit(2, sz, _type, _usageFlags);
}

فراغ UMat::fit(سائيز _sz, صحيح _type, UMatUsageFlags _usageFlags)
{
    fit(_sz.اوچائي, _sz.ويڪر, _type, _usageFlags);
}

فراغ UMat::fit_Same_سائيز(داخلا_Array m, صحيح _type, UMatUsageFlags _usageFlags)
{
    fit(m.shape(), _type, _usageFlags);
}

فراغ UMat::copy_سائيز(ثابت UMat& m)
{
    dims = m.dims;
    کانن_ڳڻپ = m.کانن_ڳڻپ;
    صفون = m.صفون;
    size = m.size;
    قدم = m.قدم;
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


UMat::UMat(ثابت UMat& m, ثابت Range& صف_Range_3, ثابت Range& کانو_Range_3)
    : flags(MAGIC_VAL), dims(0), صفون(0), کانن_ڳڻپ(0), allocator(0), usageFlags(USAGE_DEFAULT), u(0), offset(0)
{
    CV_Assert( m.dims >= 2 && m.dims <= MatShape::MAX_DIMS );
    إذا( m.dims > 2 )
    {
        Range rs[MatShape::MAX_DIMS];
        rs[0] = صف_Range_3;
        rs[1] = کانو_Range_3;
        لكل( صحيح i = 2; i < m.dims; i++ )
            rs[i] = Range::all();
        *هذا = m(rs);
        أرجع;
    }

    *هذا = m;
    إذا( صف_Range_3 != Range::all() && صف_Range_3 != Range(0,صفون) )
    {
        CV_Assert( 0 <= صف_Range_3.شروعات &&
                  صف_Range_3.شروعات <= صف_Range_3.end &&
                  صف_Range_3.end <= m.صفون );
        size[0] = صفون = صف_Range_3.size();
        offset += قدم*صف_Range_3.شروعات;
        flags |= SUBMATRIX_FLAG;
    }

    إذا( کانو_Range_3 != Range::all() && کانو_Range_3 != Range(0,کانن_ڳڻپ) )
    {
        CV_Assert( 0 <= کانو_Range_3.شروعات &&
                  کانو_Range_3.شروعات <= کانو_Range_3.end &&
                  کانو_Range_3.end <= m.کانن_ڳڻپ );
        size[1] = کانن_ڳڻپ = کانو_Range_3.size();
        offset += کانو_Range_3.شروعات*elem_سائيز();
        flags |= SUBMATRIX_FLAG;
    }

    updateContinuityFlag();

    إذا( صفون <= 0 || کانن_ڳڻپ <= 0 )
    {
        release();
        صفون = کانن_ڳڻپ = 0;
    }
}


UMat::UMat(ثابت UMat& m, ثابت Rect& roi)
    : flags(m.flags), dims(2), صفون(roi.اوچائي), کانن_ڳڻپ(roi.ويڪر),
    allocator(m.allocator), usageFlags(m.usageFlags), u(m.u),
    offset(m.offset + roi.y*m.قدم[0]), size(2)
{
    CV_Assert( m.dims <= 2 );

    size_t esz = CV_ELEM_سائيز(flags);
    offset += roi.x*esz;
    CV_Assert( 0 <= roi.x && 0 <= roi.ويڪر && roi.x + roi.ويڪر <= m.کانن_ڳڻپ &&
              0 <= roi.y && 0 <= roi.اوچائي && roi.y + roi.اوچائي <= m.صفون );
    إذا( roi.ويڪر < m.کانن_ڳڻپ || roi.اوچائي < m.صفون )
        flags |= SUBMATRIX_FLAG;

    size[0] = صفون;
    size[1] = کانن_ڳڻپ;
    قدم[0] = m.dims == 2 ? m.قدم[0] : کانن_ڳڻپ*esz;
    قدم[1] = esz;

    updateContinuityFlag();

    addref();
    إذا( صفون <= 0 || کانن_ڳڻپ <= 0 )
    {
        صفون = کانن_ڳڻپ = 0;
        release();
    }
}


UMat::UMat(ثابت UMat& m, ثابت Range* ranges)
    : flags(MAGIC_VAL), dims(0), صفون(0), کانن_ڳڻپ(0), allocator(0), usageFlags(USAGE_DEFAULT), u(0), offset(0)
{
    صحيح i, d = m.dims;

    CV_Assert(ranges);
    لكل( i = 0; i < d; i++ )
    {
        Range r = ranges[i];
        CV_Assert( r == Range::all() || (0 <= r.شروعات && r.شروعات < r.end && r.end <= m.size[i]) );
    }
    *هذا = m;
    لكل( i = 0; i < d; i++ )
    {
        Range r = ranges[i];
        إذا( r != Range::all() && r != Range(0, size.p[i]))
        {
            size.p[i] = r.end - r.شروعات;
            offset += r.شروعات*قدم.p[i];
            flags |= SUBMATRIX_FLAG;
        }
    }

    إذا (d <= 2) {
        صفون = d == 2 ? size[0] : 1;
        کانن_ڳڻپ = d <= 0 ? (d >= 0) : size[d > 1];
    }
    updateContinuityFlag();
}

UMat::UMat(ثابت UMat& m, ثابت std::vector<Range>& ranges)
    : flags(MAGIC_VAL), dims(0), صفون(0), کانن_ڳڻپ(0), allocator(0), usageFlags(USAGE_DEFAULT), u(0), offset(0)
{
    صحيح i, d = m.dims;

    CV_Assert((صحيح)ranges.size() == d);
    لكل (i = 0; i < d; i++)
    {
        Range r = ranges[i];
        CV_Assert(r == Range::all() || (0 <= r.شروعات && r.شروعات < r.end && r.end <= m.size[i]));
    }
    *هذا = m;
    لكل (i = 0; i < d; i++)
    {
        Range r = ranges[i];
        إذا (r != Range::all() && r != Range(0, size.p[i]))
        {
            size.p[i] = r.end - r.شروعات;
            offset += r.شروعات*قدم.p[i];
            flags |= SUBMATRIX_FLAG;
        }
    }

    إذا (d <= 2) {
        صفون = d == 2 ? size[0] : 1;
        کانن_ڳڻپ = d <= 0 ? (d >= 0) : size[d > 1];
    }
    updateContinuityFlag();
}

UMat UMat::diag(صحيح d) ثابت
{
    CV_Assert( dims <= 2 );
    UMat m = *هذا;
    size_t esz = elem_سائيز();
    صحيح len;

    إذا( d >= 0 )
    {
        len = std::min(کانن_ڳڻپ - d, صفون);
        m.offset += esz*d;
    }
    وإلا
    {
        len = std::min(صفون + d, کانن_ڳڻپ);
        m.offset -= قدم[0]*d;
    }
    CV_DbgAssert( len > 0 );

    m.size[0] = m.صفون = len;
    m.size[1] = m.کانن_ڳڻپ = 1;
    m.قدم[0] += (len > 1 ? esz : 0);

    m.updateContinuityFlag();

    إذا( size() != سائيز(1,1) )
        m.flags |= SUBMATRIX_FLAG;

    أرجع m;
}

فراغ UMat::locateROI( سائيز& سڄو_سائيز, نقطو& ofs ) ثابت
{
    CV_Assert( dims <= 2 && قدم[0] > 0 );
    size_t esz = elem_سائيز(), minstep;
    ptrdiff_t delta1 = (ptrdiff_t)offset, delta2 = (ptrdiff_t)u->size;

    إذا( delta1 == 0 )
        ofs.x = ofs.y = 0;
    وإلا
    {
        ofs.y = (صحيح)(delta1/قدم[0]);
        ofs.x = (صحيح)((delta1 - قدم[0]*ofs.y)/esz);
        CV_DbgAssert( offset == (size_t)(ofs.y*قدم[0] + ofs.x*esz) );
    }
    minstep = (ofs.x + کانن_ڳڻپ)*esz;
    سڄو_سائيز.اوچائي = (صحيح)((delta2 - minstep)/قدم[0] + 1);
    سڄو_سائيز.اوچائي = std::max(سڄو_سائيز.اوچائي, ofs.y + صفون);
    سڄو_سائيز.ويڪر = (صحيح)((delta2 - قدم*(سڄو_سائيز.اوچائي-1))/esz);
    سڄو_سائيز.ويڪر = std::max(سڄو_سائيز.ويڪر, ofs.x + کانن_ڳڻپ);
}


UMat& UMat::adjustROI( صحيح dtop, صحيح dbottom, صحيح dleft, صحيح dright )
{
    CV_Assert( dims == 2 && قدم[0] > 0 );
    سائيز سڄو_سائيز; نقطو ofs;
    size_t esz = elem_سائيز();
    locateROI( سڄو_سائيز, ofs );
    صحيح row1 = std::min(std::max(ofs.y - dtop, 0), سڄو_سائيز.اوچائي), row2 = std::max(0, std::min(ofs.y + صفون + dbottom, سڄو_سائيز.اوچائي));
    صحيح col1 = std::min(std::max(ofs.x - dleft, 0), سڄو_سائيز.ويڪر), col2 = std::max(0, std::min(ofs.x + کانن_ڳڻپ + dright, سڄو_سائيز.ويڪر));
    إذا(row1 > row2)
        std::swap(row1, row2);
    إذا(col1 > col2)
        std::swap(col1, col2);

    offset += (row1 - ofs.y)*قدم + (col1 - ofs.x)*esz;
    صفون = row2 - row1; کانن_ڳڻپ = col2 - col1;
    size.p[0] = صفون; size.p[1] = کانن_ڳڻپ;
    updateContinuityFlag();
    أرجع *هذا;
}


UMat UMat::reshape(صحيح new_cn, صحيح new_صفون) ثابت
{
    صحيح cn = channels();
    UMat hdr = *هذا;

    إذا( dims > 2 && new_صفون == 0 && new_cn != 0 && size[dims-1]*cn % new_cn == 0 )
    {
        hdr.flags = (hdr.flags & ~CV_MAT_CN_نقاب) | ((new_cn-1) << CV_CN_سرڪايو);
        hdr.قدم[dims-1] = CV_ELEM_سائيز(hdr.flags);
        hdr.size[dims-1] = hdr.size[dims-1]*cn / new_cn;
        أرجع hdr;
    }

    CV_Assert( dims <= 2 );

    إذا( new_cn == 0 )
        new_cn = cn;

    صحيح ڪل_ويڪر = کانن_ڳڻپ * cn;

    إذا( (new_cn > ڪل_ويڪر || ڪل_ويڪر % new_cn != 0) && new_صفون == 0 )
        new_صفون = صفون * ڪل_ويڪر / new_cn;

    إذا( new_صفون != 0 && new_صفون != صفون )
    {
        صحيح ڪل_سائيز_2 = ڪل_ويڪر * صفون;
        إذا( !isContinuous() )
            CV_Error( cv::Error::Bad_قدم,
            "The matrix is not continuous, thus its number of rows can not be changed" );

        ڪل_ويڪر = ڪل_سائيز_2 / new_صفون;

        إذا( ڪل_ويڪر * new_صفون != ڪل_سائيز_2 )
            CV_Error( cv::Error::StsBadArg, "The total number of matrix elements "
                                    "is not divisible by the new number of rows" );

        hdr.size[0] = hdr.صفون = new_صفون;
        hdr.قدم[0] = ڪل_ويڪر * elemSize1();
    } وإلا {
        hdr.size[0] = hdr.صفون = صفون;
        إذا (dims <= 1)
            hdr.قدم[0] = کانن_ڳڻپ * CV_ELEM_سائيز(flags);
    }

    صحيح new_ويڪر = ڪل_ويڪر / new_cn;

    إذا( new_ويڪر * new_cn != ڪل_ويڪر )
        CV_Error( cv::Error::Bad_انگ_Channels,
        "The total width is not divisible by the new number of channels" );

    hdr.size.dims = hdr.dims = 2;
    hdr.size[1] = hdr.کانن_ڳڻپ = new_ويڪر;
    hdr.flags = (hdr.flags & ~CV_MAT_CN_نقاب) | ((new_cn-1) << CV_CN_سرڪايو);
    hdr.قدم[1] = CV_ELEM_سائيز(hdr.flags);
    أرجع hdr;
}

#جيڪڏهن_وصف_ٿيل HAVE_OPENCL
نطاق {
ساكن منطقي ocl_سيٽ_Diag(ثابت UMat& d, UMat& m, صحيح len)
{
    صحيح cn = d.channels();
    صحيح اونهائي = d.اونهائي();

    إذا (اونهائي == CV_64F && !ocl::Device::getDefault().doubleFPConfig())
        أرجع خطأ;

    String opts = format("-D SET_DIAG -D T1=%s -D cn=%d -D IS_ROW_VECTOR=%d",
                         ocl::memop_Type_منزل_Str(اونهائي),
                         cn,
                         (d.صفون == 1) ? 1 : 0);

    ocl::Kernel k("setDiag", ocl::core::copyset_oclsrc, opts);
    إذا (k.empty())
        أرجع خطأ;

    k.دليلون(ocl::KernelArg::WriteOnly(m),
           ocl::KernelArg::Read_Only_No_سائيز(d),
           len);

    size_t globalsize[2] = { (size_t)len, (size_t)len };
    أرجع k.لاڳيتو(2, globalsize, عدم, خطأ);
}
}
#جيڪڏهن_پورو

UMat UMat::diag(ثابت UMat& d, UMatUsageFlags usageFlags)
{
    CV_INSTRUMENT_REGION();

    CV_Assert(d.کانن_ڳڻپ == 1 || d.صفون == 1);
    صحيح len = d.صفون + d.کانن_ڳڻپ - 1;

#جيڪڏهن_وصف_ٿيل HAVE_OPENCL
    إذا (ocl::useOpenCL())
    {
        UMat m(len, len, d.type(), usageFlags);
        إذا (ocl_سيٽ_Diag(d, m, len))
        {
            CV_IMPL_جوڙيو(CV_IMPL_OCL);
            أرجع m;
        }
    }
#جيڪڏهن_پورو

    UMat m(len, len, d.type(), Scalar(0), usageFlags);
    UMat md = m.diag();
    إذا( d.کانن_ڳڻپ == 1 )
        d.copy_منزل(md);
    وإلا
        transpose(d, md);
    أرجع m;
}

صحيح UMat::check_ويڪٽر(صحيح _elemChannels, صحيح اونهائي_2, منطقي _requireContinuous) ثابت
{
    أرجع (اونهائي() == اونهائي_2 || اونهائي_2 <= 0) &&
        (isContinuous() || !_requireContinuous) &&
        ((dims <= 2 && (((صفون == 1 || کانن_ڳڻپ == 1) && channels() == _elemChannels) ||
                        (کانن_ڳڻپ == _elemChannels && channels() == 1))) ||
        (dims == 3 && channels() == 1 && size.p[2] == _elemChannels && (size.p[0] == 1 || size.p[1] == 1) &&
         (isContinuous() || قدم.p[1] == قدم.p[2]*size.p[2])))
    ? (صحيح)(ڪل()*channels()/_elemChannels) : -1;
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

        size_t ڪل_elem1_ref = هذا->ڪل() * هذا->channels();
        size_t ڪل_elem1 = _cn;

        Auto_بفر<صحيح, 4> newsz_buf( (size_t)std::max(_newndims, 1) );

        صحيح m1_idx = -1;

        لكل (صحيح i = 0; i < _newndims; i++)
        {
            إذا (_newsz[i] >= 0) {
                إذا (_newsz[i] == 0 && i < dims)
                    newsz_buf[i] = size.p[i];
                وإلا
                    newsz_buf[i] = _newsz[i];
                ڪل_elem1 *= (size_t)newsz_buf[i];
            } وإلا {
                إذا (m1_idx >= 0)
                    CV_Error(cv::Error::Sts_Bad_سائيز, "More than one '-1' occured in the new shape");
                m1_idx = i;
            }
        }

        إذا (m1_idx >= 0) {
            إذا (ڪل_elem1 == 0) {
                CV_Assert(ڪل_elem1_ref == 0);
                ڪل_elem1 = 1;
            }
            CV_Assert(ڪل_elem1_ref % ڪل_elem1 == 0);
            newsz_buf[m1_idx] = (صحيح)(ڪل_elem1_ref / ڪل_elem1);
        } وإلا إذا (ڪل_elem1 != ڪل_elem1_ref) {
            CV_Error(cv::Error::StsUnmatchedSizes, "Requested and source matrices have different count of elements");
        }

        UMat hdr = *هذا;
        hdr.flags = (hdr.flags & ~CV_MAT_CN_نقاب) | ((_cn-1) << CV_CN_سرڪايو);
        سيٽ_سائيز(hdr, _newndims, newsz_buf.معلومات(), عدم, سچ);

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
    UMat_معلومات_Auto_تالو autolock(u);
    حاول
    {
        إذا(CV_XADD(&u->refcount, 1) == 0)
            u->currAllocator->map(u, accessFlags);
        إذا (u->معلومات != 0)
        {
            Mat hdr(dims, size.p, type(), u->معلومات + offset, قدم.p);
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

#جيڪڏهن_وصف_ٿيل HAVE_CUDA
    إذا (_dst.isGpuMat())
    {
        _dst.getGpuMat().upload(*هذا);
        أرجع;
    }
#جيڪڏهن_پورو

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
    size_t esz = CV_ELEM_سائيز(stype);
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

فراغ UMat::copy_منزل(OutputArray _dst, داخلا_Array نقاب_2) ثابت
{
    CV_INSTRUMENT_REGION();

    إذا( نقاب_2.empty() )
    {
        copy_منزل(_dst);
        أرجع;
    }
#جيڪڏهن_وصف_ٿيل HAVE_OPENCL
    صحيح cn = channels(), mtype = نقاب_2.type(), mdepth = CV_MAT_اونهائي(mtype), mcn = CV_MAT_CN(mtype);
    CV_Assert( (mdepth == CV_8U || mdepth == CV_8S || mdepth == CV_Bool) && (mcn == 1 || mcn == cn) );

    إذا (ocl::useOpenCL() && _dst.isUMat() && dims <= 2)
    {
        UMat_معلومات * prevu = _dst.getUMat().u;
        _dst.create( size, type() );

        UMat dst = _dst.getUMat();

        منطقي haveDstUninit = خطأ;
        إذا( prevu != dst.u ) // do not leave dst uninitialized
            haveDstUninit = سچ;

        String opts = format("-D COPY_TO_MASK -D T1=%s -D scn=%d -D mcn=%d%s",
                             ocl::memop_Type_منزل_Str(اونهائي()), cn, mcn,
                             haveDstUninit ? " -D HAVE_DST_UNINIT" : "");

        ocl::Kernel k("copyToMask", ocl::core::copyset_oclsrc, opts);
        إذا (!k.empty())
        {
            k.دليلون(ocl::KernelArg::Read_Only_No_سائيز(*هذا),
                   ocl::KernelArg::Read_Only_No_سائيز(نقاب_2.getUMat()),
                   haveDstUninit ? ocl::KernelArg::WriteOnly(dst) :
                                   ocl::KernelArg::ReadWrite(dst));

            size_t globalsize[2] = { (size_t)کانن_ڳڻپ, (size_t)صفون };
            إذا (k.لاڳيتو(2, globalsize, عدم, خطأ))
            {
                CV_IMPL_جوڙيو(CV_IMPL_OCL);
                أرجع;
            }
        }
    }
#جيڪڏهن_پورو
    Mat src = getMat(ACCESS_READ);
    src.copy_منزل(_dst, نقاب_2);
}


//
// void UMat::convertTo moved to convert.dispatch.cpp
//

UMat& UMat::سيٽ_منزل(داخلا_Array قدر, داخلا_Array نقاب_2)
{
    CV_INSTRUMENT_REGION();

    منطقي have_نقاب = !نقاب_2.empty();
#جيڪڏهن_وصف_ٿيل HAVE_OPENCL
    صحيح tp = type(), cn = CV_MAT_CN(tp), d = CV_MAT_اونهائي(tp);

    إذا( dims <= 2 && cn <= 4 && CV_MAT_اونهائي(tp) < CV_64F && ocl::useOpenCL() )
    {
        Mat value = قدر.getMat();
        CV_Assert( checkScalar(value, type(), قدر.قسم(), داخلا_Array_2::UMAT) );
        صحيح kercn = have_نقاب || cn == 3 ? cn : std::max(cn, ocl::predict_Optimal_ويڪٽر_ويڪر(*هذا)),
                kertp = CV_MAKE_TYPE(d, kercn);

        مزدوج buf[16] = { 0, 0, 0, 0, 0, 0, 0, 0,
                           0, 0, 0, 0, 0, 0, 0, 0 };
        convertAndUnrollScalar(value, tp, (uchar *)buf, kercn / cn);

        صحيح scalarcn = kercn == 3 ? 4 : kercn, صفون_في_WI = ocl::Device::getDefault().isIntel() ? 4 : 1;
        String opts = format("-D dstT=%s -D rowsPerWI=%d -D dstST=%s -D dstT1=%s -D cn=%d",
                             ocl::memop_Type_منزل_Str(kertp), صفون_في_WI,
                             ocl::memop_Type_منزل_Str(CV_MAKETYPE(d, scalarcn)),
                             ocl::memop_Type_منزل_Str(d), kercn);

        ocl::Kernel سيٽ_K(have_نقاب ? "setMask" : "set", ocl::core::copyset_oclsrc, opts);
        إذا( !سيٽ_K.empty() )
        {
            ocl::KernelArg scalararg(ocl::KernelArg::CONSTANT, 0, 0, 0, buf, CV_ELEM_سائيز(d) * scalarcn);
            UMat نقاب;

            إذا( have_نقاب )
            {
                نقاب = نقاب_2.getUMat();
                CV_Assert( نقاب.size() == size() && (نقاب.type() == CV_8U || نقاب.type() == CV_8S || نقاب.type() == CV_Bool) );
                ocl::KernelArg maskarg = ocl::KernelArg::Read_Only_No_سائيز(نقاب),
                        dstarg = ocl::KernelArg::ReadWrite(*هذا);
                سيٽ_K.دليلون(maskarg, dstarg, scalararg);
            }
            وإلا
            {
                ocl::KernelArg dstarg = ocl::KernelArg::WriteOnly(*هذا, cn, kercn);
                سيٽ_K.دليلون(dstarg, scalararg);
            }

            size_t globalsize[] = { (size_t)کانن_ڳڻپ * cn / kercn, ((size_t)صفون + صفون_في_WI - 1) / صفون_في_WI };
            إذا( سيٽ_K.لاڳيتو(2, globalsize, عدم, خطأ) )
            {
                CV_IMPL_جوڙيو(CV_IMPL_OCL);
                أرجع *هذا;
            }
        }
    }
#جيڪڏهن_پورو
    Mat m = getMat(have_نقاب ? ACCESS_RW : ACCESS_WRITE);
    m.سيٽ_منزل(قدر, نقاب_2);
    أرجع *هذا;
}

UMat& UMat::عامل = (ثابت Scalar& s)
{
    سيٽ_منزل(s);
    أرجع *هذا;
}

UMat UMat::t() ثابت
{
    UMat m;
    transpose(*هذا, m);
    أرجع m;
}

UMat UMat::zeros(صحيح صفون, صحيح کانن_ڳڻپ, صحيح type, UMatUsageFlags usageFlags)
{
    أرجع UMat(صفون, کانن_ڳڻپ, type, Scalar::all(0), usageFlags);
}

UMat UMat::zeros(سائيز size, صحيح type, UMatUsageFlags usageFlags)
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

UMat UMat::ones(صحيح صفون, صحيح کانن_ڳڻپ, صحيح type, UMatUsageFlags usageFlags)
{
    أرجع UMat(صفون, کانن_ڳڻپ, type, Scalar(1), usageFlags);
}

UMat UMat::ones(سائيز size, صحيح type, UMatUsageFlags usageFlags)
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
