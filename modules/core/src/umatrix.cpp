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

#شامل "precomp.hpp"
#شامل "opencl_kernels_core.hpp"
#شامل "umatrix.hpp"

#شامل <opencv2/core/utils/tls.hpp>

///////////////////////////////// UMat implementation ///////////////////////////////

نامخلا cv {

// forward decls, implementation is below in this file
خالی سیٹ_سائز(UMat& m, صحیح _dims, ثابت صحیح* _sz, ثابت size_t* اقدام_2,
             منطقی auto_اقدام = جھوٹ);

خالی updateContinuityFlag(UMat& m);
خالی finalizeHdr(UMat& m);

UMat_ڈیٹا::UMat_ڈیٹا(ثابت MatAllocator* allocator)
{
    پچھلا_Allocator = currAllocator = allocator;
    urefcount = refcount = mapcount = 0;
    ڈیٹا = origdata = 0;
    size = 0;
    flags = static_cast<UMat_ڈیٹا::حافظہ_Flag>(0);
    handle = 0;
    userdata = 0;
    allocatorFlags_ = 0;
    original_UMat_ڈیٹا = تہی;
}

UMat_ڈیٹا::~UMat_ڈیٹا()
{
    پچھلا_Allocator = currAllocator = 0;
    urefcount = refcount = 0;
    CV_Assert(mapcount == 0);
    ڈیٹا = origdata = 0;
    size = 0;
    منطقی isAsyncCleanup = !!(flags & UMat_ڈیٹا::ASYNC_CLEANUP);
    flags = static_cast<UMat_ڈیٹا::حافظہ_Flag>(0);
    handle = 0;
    userdata = 0;
    allocatorFlags_ = 0;
    اگر (original_UMat_ڈیٹا)
    {
        منطقی showWarn = جھوٹ;
        UMat_ڈیٹا* u = original_UMat_ڈیٹا;
        منطقی zero_Ref = CV_XADD(&(u->refcount), -1) == 1;
        اگر (zero_Ref)
        {
            // simulate Mat::deallocate
            اگر (u->mapcount != 0)
            {
                (u->currAllocator ? u->currAllocator : /* TODO allocator ? allocator :*/ Mat::getDefaultAllocator())->unmap(u);
            }
            ورنہ
            {
                // we don't do "map", so we can't do "unmap"
            }
        }
        منطقی zero_URef = CV_XADD(&(u->urefcount), -1) == 1;
        اگر (zero_Ref && !zero_URef)
            showWarn = سچ;
        اگر (zero_Ref && zero_URef) // oops, we need to free resources
        {
            showWarn = !isAsyncCleanup;
            // simulate UMat::deallocate
            u->currAllocator->deallocate(u);
        }
#اگر_غیر_تعریف_شدہ NDEBUG
        اگر (showWarn)
        {
            ساکن صحیح warn_پیغام_showed = 0;
            اگر (warn_پیغام_showed++ < 100)
            {
                fflush(stdout);
                fprintf(stderr, "\n! OPENCV warning: getUMat()/getMat() call chain possible problem."
                                "\n!                 Base object is dead, while nested/derived object is still alive or processed."
                                "\n!                 Please check lifetime of UMat/Mat objects!\n");
                fflush(stderr);
            }
        }
#ورنہ
        CV_غیر_مستعمل(showWarn);
#اگر_ختم
        original_UMat_ڈیٹا = تہی;
    }
}

#اگر_غیر_تعریف_شدہ OPENCV_DISABLE_THREAD_SUPPORT

// it should be a prime number for the best hash function
شمار { UMAT_NLOCKS = 31 };
ساکن Mutex umatLocks[UMAT_NLOCKS];

ساکن size_t get_UMat_ڈیٹا_تالا_اشاریہ(ثابت UMat_ڈیٹا* u)
{
    size_t idx = ((size_t)(خالی*)u) % UMAT_NLOCKS;
    واپس idx;
}

خالی UMat_ڈیٹا::lock()
{
    size_t idx = get_UMat_ڈیٹا_تالا_اشاریہ(یہ);
    //printf("%d lock(%d)\n", cv::utils::getThreadID(), (int)idx);
    umatLocks[idx].lock();
}

خالی UMat_ڈیٹا::unlock()
{
    size_t idx = get_UMat_ڈیٹا_تالا_اشاریہ(یہ);
    //printf("%d unlock(%d)\n", cv::utils::getThreadID(), (int)idx);
    umatLocks[idx].unlock();
}


// Do not allow several lock() calls with different UMatData objects.
ساخت UMat_ڈیٹا_Auto_Locker
{
    صحیح usage_گنتی;
    UMat_ڈیٹا* locked_objects[2];
    UMat_ڈیٹا_Auto_Locker() : usage_گنتی(0) { locked_objects[0] = تہی; locked_objects[1] = تہی; }

    خالی lock(UMat_ڈیٹا*& u1)
    {
        منطقی locked_1 = (u1 == locked_objects[0] || u1 == locked_objects[1]);
        اگر (locked_1)
        {
            u1 = تہی;
            واپس;
        }
        CV_Assert(usage_گنتی == 0);  // UMatDataAutoLock can't be used multiple times from the same thread
        usage_گنتی = 1;
        locked_objects[0] = u1;
        u1->lock();
    }
    خالی lock(UMat_ڈیٹا*& u1, UMat_ڈیٹا*& u2)
    {
        منطقی locked_1 = (u1 == locked_objects[0] || u1 == locked_objects[1]);
        منطقی locked_2 = (u2 == locked_objects[0] || u2 == locked_objects[1]);
        اگر (locked_1)
            u1 = تہی;
        اگر (locked_2)
            u2 = تہی;
        اگر (locked_1 && locked_2)
            واپس;
        CV_Assert(usage_گنتی == 0);  // UMatDataAutoLock can't be used multiple times from the same thread
        usage_گنتی = 1;
        locked_objects[0] = u1;
        locked_objects[1] = u2;
        اگر (u1)
            u1->lock();
        اگر (u2)
            u2->lock();
    }
    خالی release(UMat_ڈیٹا* u1, UMat_ڈیٹا* u2)
    {
        اگر (u1 == تہی && u2 == تہی)
            واپس;
        CV_Assert(usage_گنتی == 1);
        usage_گنتی = 0;
        اگر (u1)
            u1->unlock();
        اگر (u2)
            u2->unlock();
        locked_objects[0] = تہی; locked_objects[1] = تہی;
    }
};
ساکن TLSData<UMat_ڈیٹا_Auto_Locker>& get_UMat_ڈیٹا_Auto_Locker_TLS()
{
    CV_SINGLETON_LAZY_INIT_REF(TLSData<UMat_ڈیٹا_Auto_Locker>, نیا TLSData<UMat_ڈیٹا_Auto_Locker>());
}
ساکن UMat_ڈیٹا_Auto_Locker& get_UMat_ڈیٹا_Auto_Locker() { واپس get_UMat_ڈیٹا_Auto_Locker_TLS().getRef(); }


UMat_ڈیٹا_Auto_تالا::UMat_ڈیٹا_Auto_تالا(UMat_ڈیٹا* u) : u1(u), u2(تہی)
{
    get_UMat_ڈیٹا_Auto_Locker().lock(u1);
}
UMat_ڈیٹا_Auto_تالا::UMat_ڈیٹا_Auto_تالا(UMat_ڈیٹا* u1_, UMat_ڈیٹا* u2_) : u1(u1_), u2(u2_)
{
    اگر (get_UMat_ڈیٹا_تالا_اشاریہ(u1) > get_UMat_ڈیٹا_تالا_اشاریہ(u2))
    {
        std::swap(u1, u2);
    }
    get_UMat_ڈیٹا_Auto_Locker().lock(u1, u2);
}
UMat_ڈیٹا_Auto_تالا::~UMat_ڈیٹا_Auto_تالا()
{
    get_UMat_ڈیٹا_Auto_Locker().release(u1, u2);
}

#ورنہ

خالی UMat_ڈیٹا::lock()
{
    // nothing in OPENCV_DISABLE_THREAD_SUPPORT mode
}

خالی UMat_ڈیٹا::unlock()
{
    // nothing in OPENCV_DISABLE_THREAD_SUPPORT mode
}

UMat_ڈیٹا_Auto_تالا::UMat_ڈیٹا_Auto_تالا(UMat_ڈیٹا* u) : u1(u), u2(تہی)
{
    // nothing in OPENCV_DISABLE_THREAD_SUPPORT mode
}
UMat_ڈیٹا_Auto_تالا::UMat_ڈیٹا_Auto_تالا(UMat_ڈیٹا* u1_, UMat_ڈیٹا* u2_) : u1(u1_), u2(u2_)
{
    // nothing in OPENCV_DISABLE_THREAD_SUPPORT mode
}
UMat_ڈیٹا_Auto_تالا::~UMat_ڈیٹا_Auto_تالا()
{
    // nothing in OPENCV_DISABLE_THREAD_SUPPORT mode
}

#اگر_ختم  // OPENCV_DISABLE_THREAD_SUPPORT

//////////////////////////////// UMat ////////////////////////////////

UMat::UMat(UMatUsageFlags _usageFlags) CV_NOEXCEPT
: flags(MAGIC_VAL), dims(0), صفیں(0), ستون_تعداد(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{}

UMat::UMat(صحیح صفیں_3, صحیح ستون_تعداد_3, صحیح _type, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), صفیں(0), ستون_تعداد(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(صفیں_3, ستون_تعداد_3, _type);
}

UMat::UMat(صحیح صفیں_3, صحیح ستون_تعداد_3, صحیح _type, ثابت Scalar& _s, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), صفیں(0), ستون_تعداد(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(صفیں_3, ستون_تعداد_3, _type);
    *یہ = _s;
}

UMat::UMat(سائز _sz, صحیح _type, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), صفیں(0), ستون_تعداد(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create( _sz.اونچائی, _sz.چوڑائی, _type );
}

UMat::UMat(سائز _sz, صحیح _type, ثابت Scalar& _s, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), صفیں(0), ستون_تعداد(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_sz.اونچائی, _sz.چوڑائی, _type);
    *یہ = _s;
}

UMat::UMat(صحیح _dims, ثابت صحیح* _sz, صحیح _type, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), صفیں(0), ستون_تعداد(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_dims, _sz, _type);
}

UMat::UMat(صحیح _dims, ثابت صحیح* _sz, صحیح _type, ثابت Scalar& _s, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), صفیں(0), ستون_تعداد(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_dims, _sz, _type);
    *یہ = _s;
}

UMat::UMat(ثابت MatShape& _shape, صحیح _type, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), صفیں(0), ستون_تعداد(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_shape.dims, _shape.p, _type);
}

UMat::UMat(ثابت MatShape& _shape, صحیح _type, ثابت Scalar& _s, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), صفیں(0), ستون_تعداد(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_shape.dims, _shape.p, _type);
    *یہ = _s;
}

UMat::UMat(ثابت UMat& m)
: flags(m.flags), dims(m.dims), صفیں(m.صفیں), ستون_تعداد(m.ستون_تعداد), allocator(m.allocator),
  usageFlags(m.usageFlags), u(m.u), offset(m.offset), size(m.size), قدم(m.قدم)
{
    addref();
}

UMat& UMat::عامل=(ثابت UMat& m)
{
    اگر( یہ != &m )
    {
        const_cast<UMat&>(m).addref();
        release();
        flags = m.flags;
        dims = m.dims;
        صفیں = m.صفیں;
        ستون_تعداد = m.ستون_تعداد;
        size = m.size;
        قدم = m.قدم;
        allocator = m.allocator;
        usageFlags = m.usageFlags;
        u = m.u;
        offset = m.offset;
    }
    واپس *یہ;
}

UMat UMat::clone() ثابت
{
    UMat m;
    copy_مقصد(m);
    واپس m;
}

خالی UMat::تفویض_مقصد(UMat& m, صحیح _type) ثابت
{
    اگر( _type < 0 )
        m = *یہ;
    ورنہ
        convert_مقصد(m, _type);
}

خالی UMat::create(صحیح صفیں_3, صحیح ستون_تعداد_3, صحیح _type, UMatUsageFlags _usageFlags)
{
    صحیح sz[] = {صفیں_3, ستون_تعداد_3};
    create(2, sz, _type, _usageFlags);
}

خالی UMat::create(سائز _sz, صحیح _type, UMatUsageFlags _usageFlags)
{
    create(_sz.اونچائی, _sz.چوڑائی, _type, _usageFlags);
}

خالی UMat::create_Same_سائز(داخلہ_Array arr, صحیح _type, UMatUsageFlags _usageFlags)
{
    create(arr.shape(), _type, _usageFlags);
}

خالی UMat::addref()
{
    اگر( u )
        CV_XADD(&(u->urefcount), 1);
}

خالی UMat::release()
{
    اگر( u && CV_XADD(&(u->urefcount), -1) == 1 )
        deallocate();
    u = 0;
    size.clear();
    dims = ستون_تعداد = صفیں = 0;
}

منطقی UMat::empty() ثابت
{
    واپس u == 0 || کل() == 0;
}

size_t UMat::کل() ثابت
{
    اگر( dims <= 2 )
        واپس (size_t)صفیں * ستون_تعداد;
    size_t p = 1;
    برائے( صحیح i = 0; i < dims; i++ )
        p *= size[i];
    واپس p;
}

MatShape UMat::shape() ثابت
{
    واپس size;
}

UMat::UMat(UMat&& m)
: flags(m.flags), dims(m.dims), صفیں(m.صفیں), ستون_تعداد(m.ستون_تعداد), allocator(m.allocator),
  usageFlags(m.usageFlags), u(m.u), offset(m.offset)
{
    size = m.size;
    قدم = m.قدم;
    m.flags = MAGIC_VAL;
    m.usageFlags = USAGE_DEFAULT;
    m.dims = m.صفیں = m.ستون_تعداد = 0;
    m.size.clear();
    m.allocator = تہی;
    m.u = تہی;
    m.offset = 0;
}

UMat& UMat::عامل=(UMat&& m)
{
    اگر (یہ == &m)
      واپس *یہ;
    release();
    flags = m.flags; dims = m.dims; صفیں = m.صفیں; ستون_تعداد = m.ستون_تعداد;
    allocator = m.allocator; usageFlags = m.usageFlags;
    u = m.u;
    offset = m.offset;
    size = m.size;
    قدم = m.قدم;
    m.flags = MAGIC_VAL;
    m.usageFlags = USAGE_DEFAULT;
    m.dims = m.صفیں = m.ستون_تعداد = 0;
    m.allocator = تہی;
    m.u = تہی;
    m.offset = 0;
    واپس *یہ;
}


MatAllocator* UMat::getStdAllocator()
{
#اگر_تعریف_شدہ HAVE_OPENCL
    اگر (ocl::useOpenCL())
        واپس ocl::getOpenCLAllocator();
#اگر_ختم
    واپس Mat::getDefaultAllocator();
}

خالی swap( UMat& a, UMat& b )
{
    std::swap(a.flags, b.flags);
    std::swap(a.dims, b.dims);
    std::swap(a.صفیں, b.صفیں);
    std::swap(a.ستون_تعداد, b.ستون_تعداد);
    std::swap(a.allocator, b.allocator);
    std::swap(a.u, b.u);
    std::swap(a.offset, b.offset);

    std::swap(a.size, b.size);
    std::swap(a.قدم, b.قدم);
}


خالی سیٹ_سائز( UMat& m, صحیح _dims, ثابت صحیح* _sz,
              ثابت size_t* اقدام_2, منطقی auto_اقدام )
{
    CV_Assert( 0 <= _dims && _dims <= CV_MAX_DIM && _dims <= MatShape::MAX_DIMS);

    m.dims = _dims;
    m.size = MatShape(_dims, _sz);
    m.قدم[std::max(_dims-1, 0)] = CV_ELEM_سائز(m.flags);
    برائے (صحیح i = _dims-2; i >= 0; i--) {
        size_t autostep = m.size[i+1]*m.قدم[i+1];
        اگر (اقدام_2) {
            m.قدم[i] = اقدام_2[i];
            //CV_Assert(m.step[i] >= autostep);
        } ورنہ اگر (auto_اقدام) {
            m.قدم[i] = autostep;
        } ورنہ {
            m.قدم[i] = 0;
        }
    }

    اگر( _dims <= 2 )
    {
        m.ستون_تعداد = _dims == 0 ? 1 : _sz ? _sz[_dims > 1] : 0;
        m.صفیں = _dims < 2 ? 1 : _sz ? _sz[0] : 0;
    } ورنہ {
        m.ستون_تعداد = m.صفیں = -1;
    }
}


خالی UMat::updateContinuityFlag()
{
    flags = cv::updateContinuityFlag(flags, dims, size.p, قدم.p);
}


خالی finalizeHdr(UMat& m)
{
    m.updateContinuityFlag();
    صحیح d = m.dims;
    اگر( d > 2 )
        m.صفیں = m.ستون_تعداد = -1;
}


UMat Mat::getUMat(AccessFlag accessFlags, UMatUsageFlags usageFlags) ثابت
{
    UMat hdr;
    اگر(!ڈیٹا)
        واپس hdr;
    اگر (ڈیٹا != datastart)
    {
        سائز پورا_سائز;
        نقطہ ofs;
        locateROI(پورا_سائز, ofs);
        سائز sz(ستون_تعداد, صفیں);
        اگر (ofs.x != 0 || ofs.y != 0)
        {
            Mat src = *یہ;
            صحیح dtop = ofs.y;
            صحیح dbottom = پورا_سائز.اونچائی - src.صفیں - ofs.y;
            صحیح dleft = ofs.x;
            صحیح dright = پورا_سائز.چوڑائی - src.ستون_تعداد - ofs.x;
            src.adjustROI(dtop, dbottom, dleft, dright);
            واپس src.getUMat(accessFlags, usageFlags)(cv::Rect(ofs.x, ofs.y, sz.چوڑائی, sz.اونچائی));
        }
    }
    CV_Assert(ڈیٹا == datastart);

    accessFlags |= ACCESS_RW;
    UMat_ڈیٹا* new_u = تہی;
    Mat_قدم new_قدم = قدم;
    {
        MatAllocator *a = allocator, *a0 = getDefaultAllocator();
        اگر(!a)
            a = a0;
        new_u = a->allocate(dims, size.p, type(), ڈیٹا, new_قدم.p, accessFlags, usageFlags);
        new_u->original_UMat_ڈیٹا = u;
    }
    منطقی allocated = جھوٹ;
    کوشش
    {
        allocated = UMat::getStdAllocator()->allocate(new_u, accessFlags, usageFlags);
    }
    پکڑو (ثابت cv::Exception& e)
    {
        fprintf(stderr, "Exception: %s\n", e.what());
    }
    اگر (!allocated)
    {
        allocated = getDefaultAllocator()->allocate(new_u, accessFlags, usageFlags);
        CV_Assert(allocated);
    }
    اگر (u != تہی)
    {
#اگر_تعریف_شدہ HAVE_OPENCL
        اگر (ocl::useOpenCL() && new_u->currAllocator == ocl::getOpenCLAllocator())
        {
            CV_Assert(new_u->عارضی_UMat());
        }
#اگر_ختم
        CV_XADD(&(u->refcount), 1);
        CV_XADD(&(u->urefcount), 1);
    }
    کوشش
    {
        hdr.flags = flags;
        hdr.usageFlags = usageFlags;
        سیٹ_سائز(hdr, dims, size.p, new_قدم.p);
        finalizeHdr(hdr);
        hdr.u = new_u;
        hdr.offset = 0; //data - datastart;
        hdr.addref();
        واپس hdr;
    }
    پکڑو(...)
    {
        اگر (u != تہی)
        {
            CV_XADD(&(u->refcount), -1);
            CV_XADD(&(u->urefcount), -1);
        }
        new_u->currAllocator->deallocate(new_u);
        پھینکو;
    }

}

خالی UMat::create(صحیح d0, ثابت صحیح* _sizes, صحیح _type, UMatUsageFlags _usageFlags)
{
    صحیح sz1 = 1, d = d0;
    صحیح i;
    اگر (d == 0) {
        d = 1;
        _sizes = (ثابت صحیح*)&sz1;
    }
    CV_Assert(0 <= d && d <= CV_MAX_DIM && _sizes);
    _type = CV_MAT_TYPE(_type);

    // if param value is USAGE_DEFAULT by implicit default param value -or- explicit value
    // ...then don't change the existing usageFlags
    // it is not possible to change usage from non-default to USAGE_DEFAULT through create()
    // ...instead must construct UMat()
    اگر (_usageFlags == cv::USAGE_DEFAULT)
    {
        _usageFlags = usageFlags;
    }

    اگر( u && d == dims && _type == type() && _usageFlags == usageFlags )
    {
        برائے( i = 0; i < d; i++ )
            اگر( size[i] != _sizes[i] )
                توڑو;
        اگر( i == d )
            واپس;
    }

    صحیح _sizes_backup[CV_MAX_DIM]; // #5991
    اگر (_sizes == (یہ->size.p))
    {
        برائے(i = 0; i < d; i++ )
            _sizes_backup[i] = _sizes[i];
        _sizes = _sizes_backup;
    }

    release();
    usageFlags = _usageFlags;
    اگر( d == 0 )
        واپس;
    flags = (_type & CV_MAT_TYPE_نقاب) | MAGIC_VAL;
    سیٹ_سائز(*یہ, d, _sizes, 0, سچ);
    offset = 0;

    اگر( کل() > 0 )
    {
        MatAllocator *a = allocator, *a0 = getStdAllocator();
        اگر (!a)
        {
            a = a0;
            a0 = Mat::getDefaultAllocator();
        }
        کوشش
        {
            u = a->allocate(dims, size.p, _type, 0, قدم.p, ACCESS_RW /* ignored */, usageFlags);
            CV_Assert(u != 0);
        }
        پکڑو(...)
        {
            اگر(a != a0)
                u = a0->allocate(dims, size.p, _type, 0, قدم.p, ACCESS_RW /* ignored */, usageFlags);
            CV_Assert(u != 0);
        }
        CV_Assert( قدم[dims-1] == (size_t)CV_ELEM_سائز(flags) );
    }

    finalizeHdr(*یہ);
    addref();
    size.dims = dims = d0;
}

خالی UMat::create(ثابت std::vector<صحیح>& _sizes, صحیح _type, UMatUsageFlags _usageFlags)
{
    create((صحیح)_sizes.size(), _sizes.ڈیٹا(), _type, _usageFlags);
}

خالی UMat::create(ثابت MatShape& _shape, صحیح _type, UMatUsageFlags _usageFlags)
{
    اگر (_shape.dims < 0) {
        release();
    } ورنہ {
        create(_shape.dims, _shape.p, _type, _usageFlags);
        size.layout = _shape.layout;
        size.C = _shape.C;
    }
}

خالی UMat::fit(صحیح _dims, ثابت صحیح* _sizes, صحیح _type, UMatUsageFlags _usageFlags)
{
    اگر (_usageFlags == cv::USAGE_DEFAULT)
        _usageFlags = usageFlags;
    size_t old_کل_بائٹیں = u ? u->size : 0;
    size_t esz = CV_ELEM_سائز(_type), new_کل = _dims >= 0;
    برائے (صحیح i = 0; i < _dims; i++)
        new_کل *= _sizes[i];
    size_t new_کل_بائٹیں = new_کل*esz;
    اگر (new_کل_بائٹیں > 0 &&
        (!isContinuous() ||
         new_کل_بائٹیں > old_کل_بائٹیں ||
         offset != 0 ||
         _usageFlags != usageFlags)) {
        create(_dims, _sizes, _type, _usageFlags);
    } ورنہ {
        flags = (flags & ~Mat::TYPE_نقاب) | CV_MAT_TYPE(_type);
        صحیح dummy_سائز = 0;
        سیٹ_سائز(*یہ, (_dims >= 0 ? _dims : 1), (_dims >= 0 ? _sizes : &dummy_سائز), خالی_اشاریہ, سچ);
        finalizeHdr(*یہ);
    }
}

خالی UMat::fit(ثابت std::vector<صحیح>& _shape, صحیح _type, UMatUsageFlags _usageFlags)
{
    fit((صحیح)_shape.size(), _shape.ڈیٹا(), _type, _usageFlags);
}

خالی UMat::fit(ثابت MatShape& _shape, صحیح _type, UMatUsageFlags _usageFlags)
{
    fit(_shape.dims, _shape.p, _type, _usageFlags);
    size.layout = _shape.layout;
    size.C = _shape.C;
}

خالی UMat::fit(صحیح صفیں_3, صحیح ستون_تعداد_3, صحیح _type, UMatUsageFlags _usageFlags)
{
    _type &= TYPE_نقاب;
    صحیح sz[] = {صفیں_3, ستون_تعداد_3};
    fit(2, sz, _type, _usageFlags);
}

خالی UMat::fit(سائز _sz, صحیح _type, UMatUsageFlags _usageFlags)
{
    fit(_sz.اونچائی, _sz.چوڑائی, _type, _usageFlags);
}

خالی UMat::fit_Same_سائز(داخلہ_Array m, صحیح _type, UMatUsageFlags _usageFlags)
{
    fit(m.shape(), _type, _usageFlags);
}

خالی UMat::copy_سائز(ثابت UMat& m)
{
    dims = m.dims;
    ستون_تعداد = m.ستون_تعداد;
    صفیں = m.صفیں;
    size = m.size;
    قدم = m.قدم;
}

UMat::~UMat()
{
    release();
}

خالی UMat::deallocate()
{
    UMat_ڈیٹا* u_ = u;
    u = تہی;
    u_->currAllocator->deallocate(u_);
}


UMat::UMat(ثابت UMat& m, ثابت Range& صف_Range_3, ثابت Range& ستون_Range_3)
    : flags(MAGIC_VAL), dims(0), صفیں(0), ستون_تعداد(0), allocator(0), usageFlags(USAGE_DEFAULT), u(0), offset(0)
{
    CV_Assert( m.dims >= 2 && m.dims <= MatShape::MAX_DIMS );
    اگر( m.dims > 2 )
    {
        Range rs[MatShape::MAX_DIMS];
        rs[0] = صف_Range_3;
        rs[1] = ستون_Range_3;
        برائے( صحیح i = 2; i < m.dims; i++ )
            rs[i] = Range::all();
        *یہ = m(rs);
        واپس;
    }

    *یہ = m;
    اگر( صف_Range_3 != Range::all() && صف_Range_3 != Range(0,صفیں) )
    {
        CV_Assert( 0 <= صف_Range_3.آغاز &&
                  صف_Range_3.آغاز <= صف_Range_3.end &&
                  صف_Range_3.end <= m.صفیں );
        size[0] = صفیں = صف_Range_3.size();
        offset += قدم*صف_Range_3.آغاز;
        flags |= SUBMATRIX_FLAG;
    }

    اگر( ستون_Range_3 != Range::all() && ستون_Range_3 != Range(0,ستون_تعداد) )
    {
        CV_Assert( 0 <= ستون_Range_3.آغاز &&
                  ستون_Range_3.آغاز <= ستون_Range_3.end &&
                  ستون_Range_3.end <= m.ستون_تعداد );
        size[1] = ستون_تعداد = ستون_Range_3.size();
        offset += ستون_Range_3.آغاز*elem_سائز();
        flags |= SUBMATRIX_FLAG;
    }

    updateContinuityFlag();

    اگر( صفیں <= 0 || ستون_تعداد <= 0 )
    {
        release();
        صفیں = ستون_تعداد = 0;
    }
}


UMat::UMat(ثابت UMat& m, ثابت Rect& roi)
    : flags(m.flags), dims(2), صفیں(roi.اونچائی), ستون_تعداد(roi.چوڑائی),
    allocator(m.allocator), usageFlags(m.usageFlags), u(m.u),
    offset(m.offset + roi.y*m.قدم[0]), size(2)
{
    CV_Assert( m.dims <= 2 );

    size_t esz = CV_ELEM_سائز(flags);
    offset += roi.x*esz;
    CV_Assert( 0 <= roi.x && 0 <= roi.چوڑائی && roi.x + roi.چوڑائی <= m.ستون_تعداد &&
              0 <= roi.y && 0 <= roi.اونچائی && roi.y + roi.اونچائی <= m.صفیں );
    اگر( roi.چوڑائی < m.ستون_تعداد || roi.اونچائی < m.صفیں )
        flags |= SUBMATRIX_FLAG;

    size[0] = صفیں;
    size[1] = ستون_تعداد;
    قدم[0] = m.dims == 2 ? m.قدم[0] : ستون_تعداد*esz;
    قدم[1] = esz;

    updateContinuityFlag();

    addref();
    اگر( صفیں <= 0 || ستون_تعداد <= 0 )
    {
        صفیں = ستون_تعداد = 0;
        release();
    }
}


UMat::UMat(ثابت UMat& m, ثابت Range* ranges)
    : flags(MAGIC_VAL), dims(0), صفیں(0), ستون_تعداد(0), allocator(0), usageFlags(USAGE_DEFAULT), u(0), offset(0)
{
    صحیح i, d = m.dims;

    CV_Assert(ranges);
    برائے( i = 0; i < d; i++ )
    {
        Range r = ranges[i];
        CV_Assert( r == Range::all() || (0 <= r.آغاز && r.آغاز < r.end && r.end <= m.size[i]) );
    }
    *یہ = m;
    برائے( i = 0; i < d; i++ )
    {
        Range r = ranges[i];
        اگر( r != Range::all() && r != Range(0, size.p[i]))
        {
            size.p[i] = r.end - r.آغاز;
            offset += r.آغاز*قدم.p[i];
            flags |= SUBMATRIX_FLAG;
        }
    }

    اگر (d <= 2) {
        صفیں = d == 2 ? size[0] : 1;
        ستون_تعداد = d <= 0 ? (d >= 0) : size[d > 1];
    }
    updateContinuityFlag();
}

UMat::UMat(ثابت UMat& m, ثابت std::vector<Range>& ranges)
    : flags(MAGIC_VAL), dims(0), صفیں(0), ستون_تعداد(0), allocator(0), usageFlags(USAGE_DEFAULT), u(0), offset(0)
{
    صحیح i, d = m.dims;

    CV_Assert((صحیح)ranges.size() == d);
    برائے (i = 0; i < d; i++)
    {
        Range r = ranges[i];
        CV_Assert(r == Range::all() || (0 <= r.آغاز && r.آغاز < r.end && r.end <= m.size[i]));
    }
    *یہ = m;
    برائے (i = 0; i < d; i++)
    {
        Range r = ranges[i];
        اگر (r != Range::all() && r != Range(0, size.p[i]))
        {
            size.p[i] = r.end - r.آغاز;
            offset += r.آغاز*قدم.p[i];
            flags |= SUBMATRIX_FLAG;
        }
    }

    اگر (d <= 2) {
        صفیں = d == 2 ? size[0] : 1;
        ستون_تعداد = d <= 0 ? (d >= 0) : size[d > 1];
    }
    updateContinuityFlag();
}

UMat UMat::diag(صحیح d) ثابت
{
    CV_Assert( dims <= 2 );
    UMat m = *یہ;
    size_t esz = elem_سائز();
    صحیح len;

    اگر( d >= 0 )
    {
        len = std::min(ستون_تعداد - d, صفیں);
        m.offset += esz*d;
    }
    ورنہ
    {
        len = std::min(صفیں + d, ستون_تعداد);
        m.offset -= قدم[0]*d;
    }
    CV_DbgAssert( len > 0 );

    m.size[0] = m.صفیں = len;
    m.size[1] = m.ستون_تعداد = 1;
    m.قدم[0] += (len > 1 ? esz : 0);

    m.updateContinuityFlag();

    اگر( size() != سائز(1,1) )
        m.flags |= SUBMATRIX_FLAG;

    واپس m;
}

خالی UMat::locateROI( سائز& پورا_سائز, نقطہ& ofs ) ثابت
{
    CV_Assert( dims <= 2 && قدم[0] > 0 );
    size_t esz = elem_سائز(), minstep;
    ptrdiff_t delta1 = (ptrdiff_t)offset, delta2 = (ptrdiff_t)u->size;

    اگر( delta1 == 0 )
        ofs.x = ofs.y = 0;
    ورنہ
    {
        ofs.y = (صحیح)(delta1/قدم[0]);
        ofs.x = (صحیح)((delta1 - قدم[0]*ofs.y)/esz);
        CV_DbgAssert( offset == (size_t)(ofs.y*قدم[0] + ofs.x*esz) );
    }
    minstep = (ofs.x + ستون_تعداد)*esz;
    پورا_سائز.اونچائی = (صحیح)((delta2 - minstep)/قدم[0] + 1);
    پورا_سائز.اونچائی = std::max(پورا_سائز.اونچائی, ofs.y + صفیں);
    پورا_سائز.چوڑائی = (صحیح)((delta2 - قدم*(پورا_سائز.اونچائی-1))/esz);
    پورا_سائز.چوڑائی = std::max(پورا_سائز.چوڑائی, ofs.x + ستون_تعداد);
}


UMat& UMat::adjustROI( صحیح dtop, صحیح dbottom, صحیح dleft, صحیح dright )
{
    CV_Assert( dims == 2 && قدم[0] > 0 );
    سائز پورا_سائز; نقطہ ofs;
    size_t esz = elem_سائز();
    locateROI( پورا_سائز, ofs );
    صحیح row1 = std::min(std::max(ofs.y - dtop, 0), پورا_سائز.اونچائی), row2 = std::max(0, std::min(ofs.y + صفیں + dbottom, پورا_سائز.اونچائی));
    صحیح col1 = std::min(std::max(ofs.x - dleft, 0), پورا_سائز.چوڑائی), col2 = std::max(0, std::min(ofs.x + ستون_تعداد + dright, پورا_سائز.چوڑائی));
    اگر(row1 > row2)
        std::swap(row1, row2);
    اگر(col1 > col2)
        std::swap(col1, col2);

    offset += (row1 - ofs.y)*قدم + (col1 - ofs.x)*esz;
    صفیں = row2 - row1; ستون_تعداد = col2 - col1;
    size.p[0] = صفیں; size.p[1] = ستون_تعداد;
    updateContinuityFlag();
    واپس *یہ;
}


UMat UMat::reshape(صحیح new_cn, صحیح new_صفیں) ثابت
{
    صحیح cn = channels();
    UMat hdr = *یہ;

    اگر( dims > 2 && new_صفیں == 0 && new_cn != 0 && size[dims-1]*cn % new_cn == 0 )
    {
        hdr.flags = (hdr.flags & ~CV_MAT_CN_نقاب) | ((new_cn-1) << CV_CN_سرکاؤ);
        hdr.قدم[dims-1] = CV_ELEM_سائز(hdr.flags);
        hdr.size[dims-1] = hdr.size[dims-1]*cn / new_cn;
        واپس hdr;
    }

    CV_Assert( dims <= 2 );

    اگر( new_cn == 0 )
        new_cn = cn;

    صحیح کل_چوڑائی = ستون_تعداد * cn;

    اگر( (new_cn > کل_چوڑائی || کل_چوڑائی % new_cn != 0) && new_صفیں == 0 )
        new_صفیں = صفیں * کل_چوڑائی / new_cn;

    اگر( new_صفیں != 0 && new_صفیں != صفیں )
    {
        صحیح کل_سائز_2 = کل_چوڑائی * صفیں;
        اگر( !isContinuous() )
            CV_Error( cv::Error::Bad_قدم,
            "The matrix is not continuous, thus its number of rows can not be changed" );

        کل_چوڑائی = کل_سائز_2 / new_صفیں;

        اگر( کل_چوڑائی * new_صفیں != کل_سائز_2 )
            CV_Error( cv::Error::StsBadArg, "The total number of matrix elements "
                                    "is not divisible by the new number of rows" );

        hdr.size[0] = hdr.صفیں = new_صفیں;
        hdr.قدم[0] = کل_چوڑائی * elemSize1();
    } ورنہ {
        hdr.size[0] = hdr.صفیں = صفیں;
        اگر (dims <= 1)
            hdr.قدم[0] = ستون_تعداد * CV_ELEM_سائز(flags);
    }

    صحیح new_چوڑائی = کل_چوڑائی / new_cn;

    اگر( new_چوڑائی * new_cn != کل_چوڑائی )
        CV_Error( cv::Error::Bad_عدد_Channels,
        "The total width is not divisible by the new number of channels" );

    hdr.size.dims = hdr.dims = 2;
    hdr.size[1] = hdr.ستون_تعداد = new_چوڑائی;
    hdr.flags = (hdr.flags & ~CV_MAT_CN_نقاب) | ((new_cn-1) << CV_CN_سرکاؤ);
    hdr.قدم[1] = CV_ELEM_سائز(hdr.flags);
    واپس hdr;
}

#اگر_تعریف_شدہ HAVE_OPENCL
نامخلا {
ساکن منطقی ocl_سیٹ_Diag(ثابت UMat& d, UMat& m, صحیح len)
{
    صحیح cn = d.channels();
    صحیح گہرائی = d.گہرائی();

    اگر (گہرائی == CV_64F && !ocl::Device::getDefault().doubleFPConfig())
        واپس جھوٹ;

    String opts = format("-D SET_DIAG -D T1=%s -D cn=%d -D IS_ROW_VECTOR=%d",
                         ocl::memop_Type_مقصد_Str(گہرائی),
                         cn,
                         (d.صفیں == 1) ? 1 : 0);

    ocl::Kernel k("setDiag", ocl::core::copyset_oclsrc, opts);
    اگر (k.empty())
        واپس جھوٹ;

    k.دلائل(ocl::KernelArg::WriteOnly(m),
           ocl::KernelArg::Read_Only_No_سائز(d),
           len);

    size_t globalsize[2] = { (size_t)len, (size_t)len };
    واپس k.تسلسل(2, globalsize, تہی, جھوٹ);
}
}
#اگر_ختم

UMat UMat::diag(ثابت UMat& d, UMatUsageFlags usageFlags)
{
    CV_INSTRUMENT_REGION();

    CV_Assert(d.ستون_تعداد == 1 || d.صفیں == 1);
    صحیح len = d.صفیں + d.ستون_تعداد - 1;

#اگر_تعریف_شدہ HAVE_OPENCL
    اگر (ocl::useOpenCL())
    {
        UMat m(len, len, d.type(), usageFlags);
        اگر (ocl_سیٹ_Diag(d, m, len))
        {
            CV_IMPL_جوڑو(CV_IMPL_OCL);
            واپس m;
        }
    }
#اگر_ختم

    UMat m(len, len, d.type(), Scalar(0), usageFlags);
    UMat md = m.diag();
    اگر( d.ستون_تعداد == 1 )
        d.copy_مقصد(md);
    ورنہ
        transpose(d, md);
    واپس m;
}

صحیح UMat::check_سمتیہ(صحیح _elemChannels, صحیح گہرائی_2, منطقی _requireContinuous) ثابت
{
    واپس (گہرائی() == گہرائی_2 || گہرائی_2 <= 0) &&
        (isContinuous() || !_requireContinuous) &&
        ((dims <= 2 && (((صفیں == 1 || ستون_تعداد == 1) && channels() == _elemChannels) ||
                        (ستون_تعداد == _elemChannels && channels() == 1))) ||
        (dims == 3 && channels() == 1 && size.p[2] == _elemChannels && (size.p[0] == 1 || size.p[1] == 1) &&
         (isContinuous() || قدم.p[1] == قدم.p[2]*size.p[2])))
    ? (صحیح)(کل()*channels()/_elemChannels) : -1;
}

UMat UMat::reshape(صحیح _cn, صحیح _newndims, ثابت صحیح* _newsz) ثابت
{
    اگر(_newndims == dims)
    {
        اگر(_newsz == 0)
            واپس reshape(_cn);
        اگر(_newndims == 2)
            واپس reshape(_cn, _newsz[0]);
    }

    اگر (isContinuous())
    {
        CV_Assert(_cn >= 0 && _newndims >= 0 && _newndims <= CV_MAX_DIM && (_newndims == 0 || _newsz != 0));

        اگر (_cn == 0)
            _cn = یہ->channels();
        ورنہ
            CV_Assert(_cn <= CV_CN_MAX);

        size_t کل_elem1_ref = یہ->کل() * یہ->channels();
        size_t کل_elem1 = _cn;

        Auto_بفر<صحیح, 4> newsz_buf( (size_t)std::max(_newndims, 1) );

        صحیح m1_idx = -1;

        برائے (صحیح i = 0; i < _newndims; i++)
        {
            اگر (_newsz[i] >= 0) {
                اگر (_newsz[i] == 0 && i < dims)
                    newsz_buf[i] = size.p[i];
                ورنہ
                    newsz_buf[i] = _newsz[i];
                کل_elem1 *= (size_t)newsz_buf[i];
            } ورنہ {
                اگر (m1_idx >= 0)
                    CV_Error(cv::Error::Sts_Bad_سائز, "More than one '-1' occured in the new shape");
                m1_idx = i;
            }
        }

        اگر (m1_idx >= 0) {
            اگر (کل_elem1 == 0) {
                CV_Assert(کل_elem1_ref == 0);
                کل_elem1 = 1;
            }
            CV_Assert(کل_elem1_ref % کل_elem1 == 0);
            newsz_buf[m1_idx] = (صحیح)(کل_elem1_ref / کل_elem1);
        } ورنہ اگر (کل_elem1 != کل_elem1_ref) {
            CV_Error(cv::Error::StsUnmatchedSizes, "Requested and source matrices have different count of elements");
        }

        UMat hdr = *یہ;
        hdr.flags = (hdr.flags & ~CV_MAT_CN_نقاب) | ((_cn-1) << CV_CN_سرکاؤ);
        سیٹ_سائز(hdr, _newndims, newsz_buf.ڈیٹا(), تہی, سچ);

        واپس hdr;
    }

    CV_Error(cv::Error::StsNotImplemented, "Reshaping of n-dimensional non-continuous matrices is not supported yet");
}

UMat UMat::reshape(صحیح _cn, ثابت MatShape& _newshape) ثابت
{
    اگر (_newshape.dims < 0) {
        صحیح newshape[] = {0};
        واپس reshape(_cn, 1, newshape);
    }
    واپس reshape(_cn, _newshape.dims, _newshape.p);
}

Mat UMat::getMat(AccessFlag accessFlags) ثابت
{
    اگر(!u)
        واپس Mat();
    // TODO Support ACCESS_READ (ACCESS_WRITE) without unnecessary data transfers
    accessFlags |= ACCESS_RW;
    UMat_ڈیٹا_Auto_تالا autolock(u);
    کوشش
    {
        اگر(CV_XADD(&u->refcount, 1) == 0)
            u->currAllocator->map(u, accessFlags);
        اگر (u->ڈیٹا != 0)
        {
            Mat hdr(dims, size.p, type(), u->ڈیٹا + offset, قدم.p);
            hdr.flags = flags;
            hdr.u = u;
            hdr.datastart = u->ڈیٹا;
            hdr.ڈیٹا = u->ڈیٹا + offset;
            hdr.datalimit = hdr.dataend = u->ڈیٹا + u->size;
            واپس hdr;
        }
    }
    پکڑو(...)
    {
        CV_XADD(&u->refcount, -1);
        پھینکو;
    }
    CV_XADD(&u->refcount, -1);
    CV_Assert(u->ڈیٹا != 0 && "Error mapping of UMat to host memory.");
    واپس Mat();
}

خالی* UMat::handle(AccessFlag accessFlags) ثابت
{
    اگر( !u )
        واپس 0;

    CV_Assert(u->refcount == 0);
    CV_Assert(!u->deviceCopyObsolete() || u->copyOnMap());
    اگر (u->deviceCopyObsolete())
    {
        u->currAllocator->unmap(u);
    }

    اگر (!!(accessFlags & ACCESS_WRITE))
        u->markHostCopyObsolete(سچ);

    واپس u->handle;
}

خالی UMat::ndoffset(size_t* ofs) ثابت
{
    // offset = step[0]*ofs[0] + step[1]*ofs[1] + step[2]*ofs[2] + ...;
    size_t val = offset;
    برائے( صحیح i = 0; i < dims; i++ )
    {
        size_t s = قدم.p[i];
        ofs[i] = val / s;
        val -= ofs[i]*s;
    }
}

خالی UMat::copy_مقصد(OutputArray _dst) ثابت
{
    CV_INSTRUMENT_REGION();

#اگر_تعریف_شدہ HAVE_CUDA
    اگر (_dst.isGpuMat())
    {
        _dst.getGpuMat().upload(*یہ);
        واپس;
    }
#اگر_ختم

    صحیح stype = type();
    صحیح dtype = _dst.type();
    اگر( _dst.fixedType() && dtype != stype )
    {
        CV_Assert( CV_MAT_CN(stype) == CV_MAT_CN(dtype) );
        convert_مقصد( _dst, dtype );
        واپس;
    }

    اگر( dims == 0 && empty() )
    {
        _dst.release();
        خالی* obj = _dst.getObj();
        اگر (_dst.isMat())
            reinterpret_cast<Mat*>(obj)->flags = Mat::MAGIC_VAL | Mat::CONTINUOUS_FLAG | stype;
        ورنہ اگر (_dst.isUMat())
            reinterpret_cast<UMat*>(obj)->flags = UMat::MAGIC_VAL | UMat::CONTINUOUS_FLAG | stype;
        ورنہ اگر (_dst.isGpuMat())
            reinterpret_cast<cuda::GpuMat*>(obj)->flags = stype;
        واپس;
    }

    _dst.create( dims, size.p, stype );
    اگر (empty())
        واپس;

    size_t sz[CV_MAX_DIM] = {1}, srcofs[CV_MAX_DIM]={0}, dstofs[CV_MAX_DIM]={0};
    size_t esz = CV_ELEM_سائز(stype);
    صحیح i, d = std::max(dims, 1);
    برائے( i = 0; i < d; i++ )
        sz[i] = size.p[i];
    sz[d-1] *= esz;
    ndoffset(srcofs);
    srcofs[d-1] *= esz;

    اگر( _dst.isUMat() )
    {
        UMat dst = _dst.getUMat();
        CV_Assert(dst.u);
        اگر( u == dst.u && dst.offset == offset )
            واپس;

        اگر (u->currAllocator == dst.u->currAllocator)
        {
            dst.ndoffset(dstofs);
            dstofs[dims-1] *= esz;
            u->currAllocator->copy(u, dst.u, d, sz, srcofs, قدم.p, dstofs, dst.قدم.p, جھوٹ);
            واپس;
        }

        // different allocators: never map a device destination, upload into it instead
        // (through a host Mat when both sides are device memory)
        MatAllocator* hostAllocator = Mat::getDefaultAllocator();
        اگر (dst.u->currAllocator != hostAllocator)
        {
            اگر (u->currAllocator == hostAllocator)
                getMat(ACCESS_READ).copy_مقصد(_dst);
            ورنہ
            {
                Mat tmp;
                copy_مقصد(tmp);
                tmp.copy_مقصد(_dst);
            }
            واپس;
        }
    }

    Mat dst = _dst.getMat();
    u->currAllocator->download(u, dst.ptr(), d, sz, srcofs, قدم.p, dst.قدم.p);
}

خالی UMat::copy_مقصد(OutputArray _dst, داخلہ_Array نقاب_2) ثابت
{
    CV_INSTRUMENT_REGION();

    اگر( نقاب_2.empty() )
    {
        copy_مقصد(_dst);
        واپس;
    }
#اگر_تعریف_شدہ HAVE_OPENCL
    صحیح cn = channels(), mtype = نقاب_2.type(), mdepth = CV_MAT_گہرائی(mtype), mcn = CV_MAT_CN(mtype);
    CV_Assert( (mdepth == CV_8U || mdepth == CV_8S || mdepth == CV_Bool) && (mcn == 1 || mcn == cn) );

    اگر (ocl::useOpenCL() && _dst.isUMat() && dims <= 2)
    {
        UMat_ڈیٹا * prevu = _dst.getUMat().u;
        _dst.create( size, type() );

        UMat dst = _dst.getUMat();

        منطقی haveDstUninit = جھوٹ;
        اگر( prevu != dst.u ) // do not leave dst uninitialized
            haveDstUninit = سچ;

        String opts = format("-D COPY_TO_MASK -D T1=%s -D scn=%d -D mcn=%d%s",
                             ocl::memop_Type_مقصد_Str(گہرائی()), cn, mcn,
                             haveDstUninit ? " -D HAVE_DST_UNINIT" : "");

        ocl::Kernel k("copyToMask", ocl::core::copyset_oclsrc, opts);
        اگر (!k.empty())
        {
            k.دلائل(ocl::KernelArg::Read_Only_No_سائز(*یہ),
                   ocl::KernelArg::Read_Only_No_سائز(نقاب_2.getUMat()),
                   haveDstUninit ? ocl::KernelArg::WriteOnly(dst) :
                                   ocl::KernelArg::ReadWrite(dst));

            size_t globalsize[2] = { (size_t)ستون_تعداد, (size_t)صفیں };
            اگر (k.تسلسل(2, globalsize, تہی, جھوٹ))
            {
                CV_IMPL_جوڑو(CV_IMPL_OCL);
                واپس;
            }
        }
    }
#اگر_ختم
    Mat src = getMat(ACCESS_READ);
    src.copy_مقصد(_dst, نقاب_2);
}


//
// void UMat::convertTo moved to convert.dispatch.cpp
//

UMat& UMat::سیٹ_مقصد(داخلہ_Array قدر, داخلہ_Array نقاب_2)
{
    CV_INSTRUMENT_REGION();

    منطقی have_نقاب = !نقاب_2.empty();
#اگر_تعریف_شدہ HAVE_OPENCL
    صحیح tp = type(), cn = CV_MAT_CN(tp), d = CV_MAT_گہرائی(tp);

    اگر( dims <= 2 && cn <= 4 && CV_MAT_گہرائی(tp) < CV_64F && ocl::useOpenCL() )
    {
        Mat value = قدر.getMat();
        CV_Assert( checkScalar(value, type(), قدر.قسم(), داخلہ_Array_2::UMAT) );
        صحیح kercn = have_نقاب || cn == 3 ? cn : std::max(cn, ocl::predict_Optimal_سمتیہ_چوڑائی(*یہ)),
                kertp = CV_MAKE_TYPE(d, kercn);

        دوہرا buf[16] = { 0, 0, 0, 0, 0, 0, 0, 0,
                           0, 0, 0, 0, 0, 0, 0, 0 };
        convertAndUnrollScalar(value, tp, (uchar *)buf, kercn / cn);

        صحیح scalarcn = kercn == 3 ? 4 : kercn, صفیں_فی_WI = ocl::Device::getDefault().isIntel() ? 4 : 1;
        String opts = format("-D dstT=%s -D rowsPerWI=%d -D dstST=%s -D dstT1=%s -D cn=%d",
                             ocl::memop_Type_مقصد_Str(kertp), صفیں_فی_WI,
                             ocl::memop_Type_مقصد_Str(CV_MAKETYPE(d, scalarcn)),
                             ocl::memop_Type_مقصد_Str(d), kercn);

        ocl::Kernel سیٹ_K(have_نقاب ? "setMask" : "set", ocl::core::copyset_oclsrc, opts);
        اگر( !سیٹ_K.empty() )
        {
            ocl::KernelArg scalararg(ocl::KernelArg::CONSTANT, 0, 0, 0, buf, CV_ELEM_سائز(d) * scalarcn);
            UMat نقاب;

            اگر( have_نقاب )
            {
                نقاب = نقاب_2.getUMat();
                CV_Assert( نقاب.size() == size() && (نقاب.type() == CV_8U || نقاب.type() == CV_8S || نقاب.type() == CV_Bool) );
                ocl::KernelArg maskarg = ocl::KernelArg::Read_Only_No_سائز(نقاب),
                        dstarg = ocl::KernelArg::ReadWrite(*یہ);
                سیٹ_K.دلائل(maskarg, dstarg, scalararg);
            }
            ورنہ
            {
                ocl::KernelArg dstarg = ocl::KernelArg::WriteOnly(*یہ, cn, kercn);
                سیٹ_K.دلائل(dstarg, scalararg);
            }

            size_t globalsize[] = { (size_t)ستون_تعداد * cn / kercn, ((size_t)صفیں + صفیں_فی_WI - 1) / صفیں_فی_WI };
            اگر( سیٹ_K.تسلسل(2, globalsize, تہی, جھوٹ) )
            {
                CV_IMPL_جوڑو(CV_IMPL_OCL);
                واپس *یہ;
            }
        }
    }
#اگر_ختم
    Mat m = getMat(have_نقاب ? ACCESS_RW : ACCESS_WRITE);
    m.سیٹ_مقصد(قدر, نقاب_2);
    واپس *یہ;
}

UMat& UMat::عامل = (ثابت Scalar& s)
{
    سیٹ_مقصد(s);
    واپس *یہ;
}

UMat UMat::t() ثابت
{
    UMat m;
    transpose(*یہ, m);
    واپس m;
}

UMat UMat::zeros(صحیح صفیں, صحیح ستون_تعداد, صحیح type, UMatUsageFlags usageFlags)
{
    واپس UMat(صفیں, ستون_تعداد, type, Scalar::all(0), usageFlags);
}

UMat UMat::zeros(سائز size, صحیح type, UMatUsageFlags usageFlags)
{
    واپس UMat(size, type, Scalar::all(0), usageFlags);
}

UMat UMat::zeros(صحیح ndims, ثابت صحیح* sz, صحیح type, UMatUsageFlags usageFlags)
{
    واپس UMat(ndims, sz, type, Scalar::all(0), usageFlags);
}

UMat UMat::zeros(ثابت MatShape& shape, صحیح type, UMatUsageFlags usageFlags)
{
    واپس UMat(shape.dims, shape.p, type, Scalar::all(0), usageFlags);
}

UMat UMat::ones(صحیح صفیں, صحیح ستون_تعداد, صحیح type, UMatUsageFlags usageFlags)
{
    واپس UMat(صفیں, ستون_تعداد, type, Scalar(1), usageFlags);
}

UMat UMat::ones(سائز size, صحیح type, UMatUsageFlags usageFlags)
{
    واپس UMat(size, type, Scalar(1), usageFlags);
}

UMat UMat::ones(صحیح ndims, ثابت صحیح* sz, صحیح type, UMatUsageFlags usageFlags)
{
    واپس UMat(ndims, sz, type, Scalar(1), usageFlags);
}

UMat UMat::ones(ثابت MatShape& shape, صحیح type, UMatUsageFlags usageFlags)
{
    واپس UMat(shape.dims, shape.p, type, Scalar(1), usageFlags);
}

}

/* End of file. */
