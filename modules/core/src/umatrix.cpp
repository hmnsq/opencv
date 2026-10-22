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

#ਅਨ੍ਤਰ੍ਭਾਵ "precomp.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "opencl_kernels_core.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "umatrix.hpp"

#ਅਨ੍ਤਰ੍ਭਾਵ <opencv2/core/utils/tls.hpp>

///////////////////////////////// UMat implementation ///////////////////////////////

ਨਾਮਾਕਾਸ਼ cv {

// forward decls, implementation is below in this file
ਖਾਲੀ ਸੈੱਟ_ਮਾਪ(UMat& m, ਪੂਰਨ _dims, ਅਚਲ ਪੂਰਨ* _sz, ਅਚਲ size_t* ਕਦਮਾਂ_2,
             ਤਾਰਕਿਕ auto_ਕਦਮਾਂ = ਝੂਠ);

ਖਾਲੀ updateContinuityFlag(UMat& m);
ਖਾਲੀ finalizeHdr(UMat& m);

UMat_ਅੰਕੜੇ::UMat_ਅੰਕੜੇ(ਅਚਲ MatAllocator* allocator)
{
    prevAllocator = currAllocator = allocator;
    urefcount = refcount = mapcount = 0;
    ਅੰਕੜੇ = origdata = 0;
    size = 0;
    flags = static_cast<UMat_ਅੰਕੜੇ::ਸਮ੍ਰਿਤੀ_Flag>(0);
    handle = 0;
    userdata = 0;
    allocatorFlags_ = 0;
    original_UMat_ਅੰਕੜੇ = ਨਿਰਾ;
}

UMat_ਅੰਕੜੇ::~UMat_ਅੰਕੜੇ()
{
    prevAllocator = currAllocator = 0;
    urefcount = refcount = 0;
    CV_Assert(mapcount == 0);
    ਅੰਕੜੇ = origdata = 0;
    size = 0;
    ਤਾਰਕਿਕ isAsyncCleanup = !!(flags & UMat_ਅੰਕੜੇ::ASYNC_CLEANUP);
    flags = static_cast<UMat_ਅੰਕੜੇ::ਸਮ੍ਰਿਤੀ_Flag>(0);
    handle = 0;
    userdata = 0;
    allocatorFlags_ = 0;
    ਜੇ (original_UMat_ਅੰਕੜੇ)
    {
        ਤਾਰਕਿਕ showWarn = ਝੂਠ;
        UMat_ਅੰਕੜੇ* u = original_UMat_ਅੰਕੜੇ;
        ਤਾਰਕਿਕ zero_Ref = CV_XADD(&(u->refcount), -1) == 1;
        ਜੇ (zero_Ref)
        {
            // simulate Mat::deallocate
            ਜੇ (u->mapcount != 0)
            {
                (u->currAllocator ? u->currAllocator : /* TODO allocator ? allocator :*/ Mat::getDefaultAllocator())->unmap(u);
            }
            ਨਹੀਂਤਾਂ
            {
                // we don't do "map", so we can't do "unmap"
            }
        }
        ਤਾਰਕਿਕ zero_URef = CV_XADD(&(u->urefcount), -1) == 1;
        ਜੇ (zero_Ref && !zero_URef)
            showWarn = ਸੱਚ;
        ਜੇ (zero_Ref && zero_URef) // oops, we need to free resources
        {
            showWarn = !isAsyncCleanup;
            // simulate UMat::deallocate
            u->currAllocator->deallocate(u);
        }
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ NDEBUG
        ਜੇ (showWarn)
        {
            ਸਥਿਰ ਪੂਰਨ warn_ਸੁਨੇਹਾ_showed = 0;
            ਜੇ (warn_ਸੁਨੇਹਾ_showed++ < 100)
            {
                fflush(stdout);
                fprintf(stderr, "\n! OPENCV warning: getUMat()/getMat() call chain possible problem."
                                "\n!                 Base object is dead, while nested/derived object is still alive or processed."
                                "\n!                 Please check lifetime of UMat/Mat objects!\n");
                fflush(stderr);
            }
        }
#ਨਹੀਂਤਾਂ
        CV_ਅਣਵਰਤਿਆ(showWarn);
#ਜੇ_ਅੰਤ
        original_UMat_ਅੰਕੜੇ = ਨਿਰਾ;
    }
}

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OPENCV_DISABLE_THREAD_SUPPORT

// it should be a prime number for the best hash function
ਗਿਣਤੀ { UMAT_NLOCKS = 31 };
ਸਥਿਰ Mutex umatLocks[UMAT_NLOCKS];

ਸਥਿਰ size_t get_UMat_ਅੰਕੜੇ_ਤਾਲਾ_ਸੂਚਕਾਂਕ(ਅਚਲ UMat_ਅੰਕੜੇ* u)
{
    size_t idx = ((size_t)(ਖਾਲੀ*)u) % UMAT_NLOCKS;
    ਵਾਪਸ idx;
}

ਖਾਲੀ UMat_ਅੰਕੜੇ::lock()
{
    size_t idx = get_UMat_ਅੰਕੜੇ_ਤਾਲਾ_ਸੂਚਕਾਂਕ(ਇਹ);
    //printf("%d lock(%d)\n", cv::utils::getThreadID(), (int)idx);
    umatLocks[idx].lock();
}

ਖਾਲੀ UMat_ਅੰਕੜੇ::unlock()
{
    size_t idx = get_UMat_ਅੰਕੜੇ_ਤਾਲਾ_ਸੂਚਕਾਂਕ(ਇਹ);
    //printf("%d unlock(%d)\n", cv::utils::getThreadID(), (int)idx);
    umatLocks[idx].unlock();
}


// Do not allow several lock() calls with different UMatData objects.
ਬਣਤਰ UMat_ਅੰਕੜੇ_Auto_Locker
{
    ਪੂਰਨ usage_ਗਿਣਤੀ;
    UMat_ਅੰਕੜੇ* locked_objects[2];
    UMat_ਅੰਕੜੇ_Auto_Locker() : usage_ਗਿਣਤੀ(0) { locked_objects[0] = ਨਿਰਾ; locked_objects[1] = ਨਿਰਾ; }

    ਖਾਲੀ lock(UMat_ਅੰਕੜੇ*& u1)
    {
        ਤਾਰਕਿਕ locked_1 = (u1 == locked_objects[0] || u1 == locked_objects[1]);
        ਜੇ (locked_1)
        {
            u1 = ਨਿਰਾ;
            ਵਾਪਸ;
        }
        CV_Assert(usage_ਗਿਣਤੀ == 0);  // UMatDataAutoLock can't be used multiple times from the same thread
        usage_ਗਿਣਤੀ = 1;
        locked_objects[0] = u1;
        u1->lock();
    }
    ਖਾਲੀ lock(UMat_ਅੰਕੜੇ*& u1, UMat_ਅੰਕੜੇ*& u2)
    {
        ਤਾਰਕਿਕ locked_1 = (u1 == locked_objects[0] || u1 == locked_objects[1]);
        ਤਾਰਕਿਕ locked_2 = (u2 == locked_objects[0] || u2 == locked_objects[1]);
        ਜੇ (locked_1)
            u1 = ਨਿਰਾ;
        ਜੇ (locked_2)
            u2 = ਨਿਰਾ;
        ਜੇ (locked_1 && locked_2)
            ਵਾਪਸ;
        CV_Assert(usage_ਗਿਣਤੀ == 0);  // UMatDataAutoLock can't be used multiple times from the same thread
        usage_ਗਿਣਤੀ = 1;
        locked_objects[0] = u1;
        locked_objects[1] = u2;
        ਜੇ (u1)
            u1->lock();
        ਜੇ (u2)
            u2->lock();
    }
    ਖਾਲੀ release(UMat_ਅੰਕੜੇ* u1, UMat_ਅੰਕੜੇ* u2)
    {
        ਜੇ (u1 == ਨਿਰਾ && u2 == ਨਿਰਾ)
            ਵਾਪਸ;
        CV_Assert(usage_ਗਿਣਤੀ == 1);
        usage_ਗਿਣਤੀ = 0;
        ਜੇ (u1)
            u1->unlock();
        ਜੇ (u2)
            u2->unlock();
        locked_objects[0] = ਨਿਰਾ; locked_objects[1] = ਨਿਰਾ;
    }
};
ਸਥਿਰ TLSData<UMat_ਅੰਕੜੇ_Auto_Locker>& get_UMat_ਅੰਕੜੇ_Auto_Locker_TLS()
{
    CV_SINGLETON_LAZY_INIT_REF(TLSData<UMat_ਅੰਕੜੇ_Auto_Locker>, ਨਵਾਂ TLSData<UMat_ਅੰਕੜੇ_Auto_Locker>());
}
ਸਥਿਰ UMat_ਅੰਕੜੇ_Auto_Locker& get_UMat_ਅੰਕੜੇ_Auto_Locker() { ਵਾਪਸ get_UMat_ਅੰਕੜੇ_Auto_Locker_TLS().getRef(); }


UMat_ਅੰਕੜੇ_Auto_ਤਾਲਾ::UMat_ਅੰਕੜੇ_Auto_ਤਾਲਾ(UMat_ਅੰਕੜੇ* u) : u1(u), u2(ਨਿਰਾ)
{
    get_UMat_ਅੰਕੜੇ_Auto_Locker().lock(u1);
}
UMat_ਅੰਕੜੇ_Auto_ਤਾਲਾ::UMat_ਅੰਕੜੇ_Auto_ਤਾਲਾ(UMat_ਅੰਕੜੇ* u1_, UMat_ਅੰਕੜੇ* u2_) : u1(u1_), u2(u2_)
{
    ਜੇ (get_UMat_ਅੰਕੜੇ_ਤਾਲਾ_ਸੂਚਕਾਂਕ(u1) > get_UMat_ਅੰਕੜੇ_ਤਾਲਾ_ਸੂਚਕਾਂਕ(u2))
    {
        std::swap(u1, u2);
    }
    get_UMat_ਅੰਕੜੇ_Auto_Locker().lock(u1, u2);
}
UMat_ਅੰਕੜੇ_Auto_ਤਾਲਾ::~UMat_ਅੰਕੜੇ_Auto_ਤਾਲਾ()
{
    get_UMat_ਅੰਕੜੇ_Auto_Locker().release(u1, u2);
}

#ਨਹੀਂਤਾਂ

ਖਾਲੀ UMat_ਅੰਕੜੇ::lock()
{
    // nothing in OPENCV_DISABLE_THREAD_SUPPORT mode
}

ਖਾਲੀ UMat_ਅੰਕੜੇ::unlock()
{
    // nothing in OPENCV_DISABLE_THREAD_SUPPORT mode
}

UMat_ਅੰਕੜੇ_Auto_ਤਾਲਾ::UMat_ਅੰਕੜੇ_Auto_ਤਾਲਾ(UMat_ਅੰਕੜੇ* u) : u1(u), u2(ਨਿਰਾ)
{
    // nothing in OPENCV_DISABLE_THREAD_SUPPORT mode
}
UMat_ਅੰਕੜੇ_Auto_ਤਾਲਾ::UMat_ਅੰਕੜੇ_Auto_ਤਾਲਾ(UMat_ਅੰਕੜੇ* u1_, UMat_ਅੰਕੜੇ* u2_) : u1(u1_), u2(u2_)
{
    // nothing in OPENCV_DISABLE_THREAD_SUPPORT mode
}
UMat_ਅੰਕੜੇ_Auto_ਤਾਲਾ::~UMat_ਅੰਕੜੇ_Auto_ਤਾਲਾ()
{
    // nothing in OPENCV_DISABLE_THREAD_SUPPORT mode
}

#ਜੇ_ਅੰਤ  // OPENCV_DISABLE_THREAD_SUPPORT

//////////////////////////////// UMat ////////////////////////////////

UMat::UMat(UMatUsageFlags _usageFlags) CV_NOEXCEPT
: flags(MAGIC_VAL), dims(0), ਪੰਕਤੀਆਂ(0), ਖਾਨੇ_ਗਿਣਤੀ(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{}

UMat::UMat(ਪੂਰਨ ਪੰਕਤੀਆਂ_3, ਪੂਰਨ ਖਾਨੇ_ਗਿਣਤੀ_3, ਪੂਰਨ _type, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), ਪੰਕਤੀਆਂ(0), ਖਾਨੇ_ਗਿਣਤੀ(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(ਪੰਕਤੀਆਂ_3, ਖਾਨੇ_ਗਿਣਤੀ_3, _type);
}

UMat::UMat(ਪੂਰਨ ਪੰਕਤੀਆਂ_3, ਪੂਰਨ ਖਾਨੇ_ਗਿਣਤੀ_3, ਪੂਰਨ _type, ਅਚਲ Scalar& _s, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), ਪੰਕਤੀਆਂ(0), ਖਾਨੇ_ਗਿਣਤੀ(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(ਪੰਕਤੀਆਂ_3, ਖਾਨੇ_ਗਿਣਤੀ_3, _type);
    *ਇਹ = _s;
}

UMat::UMat(ਮਾਪ _sz, ਪੂਰਨ _type, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), ਪੰਕਤੀਆਂ(0), ਖਾਨੇ_ਗਿਣਤੀ(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create( _sz.ਉਚਾਈ, _sz.ਚੌੜਾਈ, _type );
}

UMat::UMat(ਮਾਪ _sz, ਪੂਰਨ _type, ਅਚਲ Scalar& _s, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), ਪੰਕਤੀਆਂ(0), ਖਾਨੇ_ਗਿਣਤੀ(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_sz.ਉਚਾਈ, _sz.ਚੌੜਾਈ, _type);
    *ਇਹ = _s;
}

UMat::UMat(ਪੂਰਨ _dims, ਅਚਲ ਪੂਰਨ* _sz, ਪੂਰਨ _type, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), ਪੰਕਤੀਆਂ(0), ਖਾਨੇ_ਗਿਣਤੀ(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_dims, _sz, _type);
}

UMat::UMat(ਪੂਰਨ _dims, ਅਚਲ ਪੂਰਨ* _sz, ਪੂਰਨ _type, ਅਚਲ Scalar& _s, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), ਪੰਕਤੀਆਂ(0), ਖਾਨੇ_ਗਿਣਤੀ(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_dims, _sz, _type);
    *ਇਹ = _s;
}

UMat::UMat(ਅਚਲ MatShape& _shape, ਪੂਰਨ _type, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), ਪੰਕਤੀਆਂ(0), ਖਾਨੇ_ਗਿਣਤੀ(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_shape.dims, _shape.p, _type);
}

UMat::UMat(ਅਚਲ MatShape& _shape, ਪੂਰਨ _type, ਅਚਲ Scalar& _s, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), ਪੰਕਤੀਆਂ(0), ਖਾਨੇ_ਗਿਣਤੀ(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_shape.dims, _shape.p, _type);
    *ਇਹ = _s;
}

UMat::UMat(ਅਚਲ UMat& m)
: flags(m.flags), dims(m.dims), ਪੰਕਤੀਆਂ(m.ਪੰਕਤੀਆਂ), ਖਾਨੇ_ਗਿਣਤੀ(m.ਖਾਨੇ_ਗਿਣਤੀ), allocator(m.allocator),
  usageFlags(m.usageFlags), u(m.u), offset(m.offset), size(m.size), ਕਦਮ(m.ਕਦਮ)
{
    addref();
}

UMat& UMat::ਸੰਚਾਲਕ=(ਅਚਲ UMat& m)
{
    ਜੇ( ਇਹ != &m )
    {
        const_cast<UMat&>(m).addref();
        release();
        flags = m.flags;
        dims = m.dims;
        ਪੰਕਤੀਆਂ = m.ਪੰਕਤੀਆਂ;
        ਖਾਨੇ_ਗਿਣਤੀ = m.ਖਾਨੇ_ਗਿਣਤੀ;
        size = m.size;
        ਕਦਮ = m.ਕਦਮ;
        allocator = m.allocator;
        usageFlags = m.usageFlags;
        u = m.u;
        offset = m.offset;
    }
    ਵਾਪਸ *ਇਹ;
}

UMat UMat::clone() ਅਚਲ
{
    UMat m;
    copy_ਮੰਜ਼ਿਲ(m);
    ਵਾਪਸ m;
}

ਖਾਲੀ UMat::ਸੌਂਪੋ_ਮੰਜ਼ਿਲ(UMat& m, ਪੂਰਨ _type) ਅਚਲ
{
    ਜੇ( _type < 0 )
        m = *ਇਹ;
    ਨਹੀਂਤਾਂ
        convert_ਮੰਜ਼ਿਲ(m, _type);
}

ਖਾਲੀ UMat::create(ਪੂਰਨ ਪੰਕਤੀਆਂ_3, ਪੂਰਨ ਖਾਨੇ_ਗਿਣਤੀ_3, ਪੂਰਨ _type, UMatUsageFlags _usageFlags)
{
    ਪੂਰਨ sz[] = {ਪੰਕਤੀਆਂ_3, ਖਾਨੇ_ਗਿਣਤੀ_3};
    create(2, sz, _type, _usageFlags);
}

ਖਾਲੀ UMat::create(ਮਾਪ _sz, ਪੂਰਨ _type, UMatUsageFlags _usageFlags)
{
    create(_sz.ਉਚਾਈ, _sz.ਚੌੜਾਈ, _type, _usageFlags);
}

ਖਾਲੀ UMat::create_Same_ਮਾਪ(ਨਿਵੇਸ਼_Array arr, ਪੂਰਨ _type, UMatUsageFlags _usageFlags)
{
    create(arr.shape(), _type, _usageFlags);
}

ਖਾਲੀ UMat::addref()
{
    ਜੇ( u )
        CV_XADD(&(u->urefcount), 1);
}

ਖਾਲੀ UMat::release()
{
    ਜੇ( u && CV_XADD(&(u->urefcount), -1) == 1 )
        deallocate();
    u = 0;
    size.clear();
    dims = ਖਾਨੇ_ਗਿਣਤੀ = ਪੰਕਤੀਆਂ = 0;
}

ਤਾਰਕਿਕ UMat::empty() ਅਚਲ
{
    ਵਾਪਸ u == 0 || ਕੁੱਲ() == 0;
}

size_t UMat::ਕੁੱਲ() ਅਚਲ
{
    ਜੇ( dims <= 2 )
        ਵਾਪਸ (size_t)ਪੰਕਤੀਆਂ * ਖਾਨੇ_ਗਿਣਤੀ;
    size_t p = 1;
    ਲਈ( ਪੂਰਨ i = 0; i < dims; i++ )
        p *= size[i];
    ਵਾਪਸ p;
}

MatShape UMat::shape() ਅਚਲ
{
    ਵਾਪਸ size;
}

UMat::UMat(UMat&& m)
: flags(m.flags), dims(m.dims), ਪੰਕਤੀਆਂ(m.ਪੰਕਤੀਆਂ), ਖਾਨੇ_ਗਿਣਤੀ(m.ਖਾਨੇ_ਗਿਣਤੀ), allocator(m.allocator),
  usageFlags(m.usageFlags), u(m.u), offset(m.offset)
{
    size = m.size;
    ਕਦਮ = m.ਕਦਮ;
    m.flags = MAGIC_VAL;
    m.usageFlags = USAGE_DEFAULT;
    m.dims = m.ਪੰਕਤੀਆਂ = m.ਖਾਨੇ_ਗਿਣਤੀ = 0;
    m.size.clear();
    m.allocator = ਨਿਰਾ;
    m.u = ਨਿਰਾ;
    m.offset = 0;
}

UMat& UMat::ਸੰਚਾਲਕ=(UMat&& m)
{
    ਜੇ (ਇਹ == &m)
      ਵਾਪਸ *ਇਹ;
    release();
    flags = m.flags; dims = m.dims; ਪੰਕਤੀਆਂ = m.ਪੰਕਤੀਆਂ; ਖਾਨੇ_ਗਿਣਤੀ = m.ਖਾਨੇ_ਗਿਣਤੀ;
    allocator = m.allocator; usageFlags = m.usageFlags;
    u = m.u;
    offset = m.offset;
    size = m.size;
    ਕਦਮ = m.ਕਦਮ;
    m.flags = MAGIC_VAL;
    m.usageFlags = USAGE_DEFAULT;
    m.dims = m.ਪੰਕਤੀਆਂ = m.ਖਾਨੇ_ਗਿਣਤੀ = 0;
    m.allocator = ਨਿਰਾ;
    m.u = ਨਿਰਾ;
    m.offset = 0;
    ਵਾਪਸ *ਇਹ;
}


MatAllocator* UMat::getStdAllocator()
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_OPENCL
    ਜੇ (ocl::useOpenCL())
        ਵਾਪਸ ocl::getOpenCLAllocator();
#ਜੇ_ਅੰਤ
    ਵਾਪਸ Mat::getDefaultAllocator();
}

ਖਾਲੀ swap( UMat& a, UMat& b )
{
    std::swap(a.flags, b.flags);
    std::swap(a.dims, b.dims);
    std::swap(a.ਪੰਕਤੀਆਂ, b.ਪੰਕਤੀਆਂ);
    std::swap(a.ਖਾਨੇ_ਗਿਣਤੀ, b.ਖਾਨੇ_ਗਿਣਤੀ);
    std::swap(a.allocator, b.allocator);
    std::swap(a.u, b.u);
    std::swap(a.offset, b.offset);

    std::swap(a.size, b.size);
    std::swap(a.ਕਦਮ, b.ਕਦਮ);
}


ਖਾਲੀ ਸੈੱਟ_ਮਾਪ( UMat& m, ਪੂਰਨ _dims, ਅਚਲ ਪੂਰਨ* _sz,
              ਅਚਲ size_t* ਕਦਮਾਂ_2, ਤਾਰਕਿਕ auto_ਕਦਮਾਂ )
{
    CV_Assert( 0 <= _dims && _dims <= CV_MAX_DIM && _dims <= MatShape::MAX_DIMS);

    m.dims = _dims;
    m.size = MatShape(_dims, _sz);
    m.ਕਦਮ[std::max(_dims-1, 0)] = CV_ELEM_ਮਾਪ(m.flags);
    ਲਈ (ਪੂਰਨ i = _dims-2; i >= 0; i--) {
        size_t autostep = m.size[i+1]*m.ਕਦਮ[i+1];
        ਜੇ (ਕਦਮਾਂ_2) {
            m.ਕਦਮ[i] = ਕਦਮਾਂ_2[i];
            //CV_Assert(m.step[i] >= autostep);
        } ਨਹੀਂਤਾਂ ਜੇ (auto_ਕਦਮਾਂ) {
            m.ਕਦਮ[i] = autostep;
        } ਨਹੀਂਤਾਂ {
            m.ਕਦਮ[i] = 0;
        }
    }

    ਜੇ( _dims <= 2 )
    {
        m.ਖਾਨੇ_ਗਿਣਤੀ = _dims == 0 ? 1 : _sz ? _sz[_dims > 1] : 0;
        m.ਪੰਕਤੀਆਂ = _dims < 2 ? 1 : _sz ? _sz[0] : 0;
    } ਨਹੀਂਤਾਂ {
        m.ਖਾਨੇ_ਗਿਣਤੀ = m.ਪੰਕਤੀਆਂ = -1;
    }
}


ਖਾਲੀ UMat::updateContinuityFlag()
{
    flags = cv::updateContinuityFlag(flags, dims, size.p, ਕਦਮ.p);
}


ਖਾਲੀ finalizeHdr(UMat& m)
{
    m.updateContinuityFlag();
    ਪੂਰਨ d = m.dims;
    ਜੇ( d > 2 )
        m.ਪੰਕਤੀਆਂ = m.ਖਾਨੇ_ਗਿਣਤੀ = -1;
}


UMat Mat::getUMat(AccessFlag accessFlags, UMatUsageFlags usageFlags) ਅਚਲ
{
    UMat hdr;
    ਜੇ(!ਅੰਕੜੇ)
        ਵਾਪਸ hdr;
    ਜੇ (ਅੰਕੜੇ != datastart)
    {
        ਮਾਪ ਸਾਰਾ_ਮਾਪ;
        ਬਿੰਦੂ ofs;
        locateROI(ਸਾਰਾ_ਮਾਪ, ofs);
        ਮਾਪ sz(ਖਾਨੇ_ਗਿਣਤੀ, ਪੰਕਤੀਆਂ);
        ਜੇ (ofs.x != 0 || ofs.y != 0)
        {
            Mat src = *ਇਹ;
            ਪੂਰਨ dtop = ofs.y;
            ਪੂਰਨ dbottom = ਸਾਰਾ_ਮਾਪ.ਉਚਾਈ - src.ਪੰਕਤੀਆਂ - ofs.y;
            ਪੂਰਨ dleft = ofs.x;
            ਪੂਰਨ dright = ਸਾਰਾ_ਮਾਪ.ਚੌੜਾਈ - src.ਖਾਨੇ_ਗਿਣਤੀ - ofs.x;
            src.adjustROI(dtop, dbottom, dleft, dright);
            ਵਾਪਸ src.getUMat(accessFlags, usageFlags)(cv::Rect(ofs.x, ofs.y, sz.ਚੌੜਾਈ, sz.ਉਚਾਈ));
        }
    }
    CV_Assert(ਅੰਕੜੇ == datastart);

    accessFlags |= ACCESS_RW;
    UMat_ਅੰਕੜੇ* new_u = ਨਿਰਾ;
    Mat_ਕਦਮ new_ਕਦਮ = ਕਦਮ;
    {
        MatAllocator *a = allocator, *a0 = getDefaultAllocator();
        ਜੇ(!a)
            a = a0;
        new_u = a->allocate(dims, size.p, type(), ਅੰਕੜੇ, new_ਕਦਮ.p, accessFlags, usageFlags);
        new_u->original_UMat_ਅੰਕੜੇ = u;
    }
    ਤਾਰਕਿਕ allocated = ਝੂਠ;
    ਕੋਸ਼ਿਸ਼
    {
        allocated = UMat::getStdAllocator()->allocate(new_u, accessFlags, usageFlags);
    }
    ਫੜੋ (ਅਚਲ cv::Exception& e)
    {
        fprintf(stderr, "Exception: %s\n", e.what());
    }
    ਜੇ (!allocated)
    {
        allocated = getDefaultAllocator()->allocate(new_u, accessFlags, usageFlags);
        CV_Assert(allocated);
    }
    ਜੇ (u != ਨਿਰਾ)
    {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_OPENCL
        ਜੇ (ocl::useOpenCL() && new_u->currAllocator == ocl::getOpenCLAllocator())
        {
            CV_Assert(new_u->ਆਰਜ਼ੀ_UMat());
        }
#ਜੇ_ਅੰਤ
        CV_XADD(&(u->refcount), 1);
        CV_XADD(&(u->urefcount), 1);
    }
    ਕੋਸ਼ਿਸ਼
    {
        hdr.flags = flags;
        hdr.usageFlags = usageFlags;
        ਸੈੱਟ_ਮਾਪ(hdr, dims, size.p, new_ਕਦਮ.p);
        finalizeHdr(hdr);
        hdr.u = new_u;
        hdr.offset = 0; //data - datastart;
        hdr.addref();
        ਵਾਪਸ hdr;
    }
    ਫੜੋ(...)
    {
        ਜੇ (u != ਨਿਰਾ)
        {
            CV_XADD(&(u->refcount), -1);
            CV_XADD(&(u->urefcount), -1);
        }
        new_u->currAllocator->deallocate(new_u);
        ਸੁੱਟੋ;
    }

}

ਖਾਲੀ UMat::create(ਪੂਰਨ d0, ਅਚਲ ਪੂਰਨ* _sizes, ਪੂਰਨ _type, UMatUsageFlags _usageFlags)
{
    ਪੂਰਨ sz1 = 1, d = d0;
    ਪੂਰਨ i;
    ਜੇ (d == 0) {
        d = 1;
        _sizes = (ਅਚਲ ਪੂਰਨ*)&sz1;
    }
    CV_Assert(0 <= d && d <= CV_MAX_DIM && _sizes);
    _type = CV_MAT_TYPE(_type);

    // if param value is USAGE_DEFAULT by implicit default param value -or- explicit value
    // ...then don't change the existing usageFlags
    // it is not possible to change usage from non-default to USAGE_DEFAULT through create()
    // ...instead must construct UMat()
    ਜੇ (_usageFlags == cv::USAGE_DEFAULT)
    {
        _usageFlags = usageFlags;
    }

    ਜੇ( u && d == dims && _type == type() && _usageFlags == usageFlags )
    {
        ਲਈ( i = 0; i < d; i++ )
            ਜੇ( size[i] != _sizes[i] )
                ਰੋਕੋ;
        ਜੇ( i == d )
            ਵਾਪਸ;
    }

    ਪੂਰਨ _sizes_backup[CV_MAX_DIM]; // #5991
    ਜੇ (_sizes == (ਇਹ->size.p))
    {
        ਲਈ(i = 0; i < d; i++ )
            _sizes_backup[i] = _sizes[i];
        _sizes = _sizes_backup;
    }

    release();
    usageFlags = _usageFlags;
    ਜੇ( d == 0 )
        ਵਾਪਸ;
    flags = (_type & CV_MAT_TYPE_ਨਕਾਬ) | MAGIC_VAL;
    ਸੈੱਟ_ਮਾਪ(*ਇਹ, d, _sizes, 0, ਸੱਚ);
    offset = 0;

    ਜੇ( ਕੁੱਲ() > 0 )
    {
        MatAllocator *a = allocator, *a0 = getStdAllocator();
        ਜੇ (!a)
        {
            a = a0;
            a0 = Mat::getDefaultAllocator();
        }
        ਕੋਸ਼ਿਸ਼
        {
            u = a->allocate(dims, size.p, _type, 0, ਕਦਮ.p, ACCESS_RW /* ignored */, usageFlags);
            CV_Assert(u != 0);
        }
        ਫੜੋ(...)
        {
            ਜੇ(a != a0)
                u = a0->allocate(dims, size.p, _type, 0, ਕਦਮ.p, ACCESS_RW /* ignored */, usageFlags);
            CV_Assert(u != 0);
        }
        CV_Assert( ਕਦਮ[dims-1] == (size_t)CV_ELEM_ਮਾਪ(flags) );
    }

    finalizeHdr(*ਇਹ);
    addref();
    size.dims = dims = d0;
}

ਖਾਲੀ UMat::create(ਅਚਲ std::vector<ਪੂਰਨ>& _sizes, ਪੂਰਨ _type, UMatUsageFlags _usageFlags)
{
    create((ਪੂਰਨ)_sizes.size(), _sizes.ਅੰਕੜੇ(), _type, _usageFlags);
}

ਖਾਲੀ UMat::create(ਅਚਲ MatShape& _shape, ਪੂਰਨ _type, UMatUsageFlags _usageFlags)
{
    ਜੇ (_shape.dims < 0) {
        release();
    } ਨਹੀਂਤਾਂ {
        create(_shape.dims, _shape.p, _type, _usageFlags);
        size.layout = _shape.layout;
        size.C = _shape.C;
    }
}

ਖਾਲੀ UMat::fit(ਪੂਰਨ _dims, ਅਚਲ ਪੂਰਨ* _sizes, ਪੂਰਨ _type, UMatUsageFlags _usageFlags)
{
    ਜੇ (_usageFlags == cv::USAGE_DEFAULT)
        _usageFlags = usageFlags;
    size_t old_ਕੁੱਲ_ਬਾਈਟਾਂ = u ? u->size : 0;
    size_t esz = CV_ELEM_ਮਾਪ(_type), new_ਕੁੱਲ = _dims >= 0;
    ਲਈ (ਪੂਰਨ i = 0; i < _dims; i++)
        new_ਕੁੱਲ *= _sizes[i];
    size_t new_ਕੁੱਲ_ਬਾਈਟਾਂ = new_ਕੁੱਲ*esz;
    ਜੇ (new_ਕੁੱਲ_ਬਾਈਟਾਂ > 0 &&
        (!isContinuous() ||
         new_ਕੁੱਲ_ਬਾਈਟਾਂ > old_ਕੁੱਲ_ਬਾਈਟਾਂ ||
         offset != 0 ||
         _usageFlags != usageFlags)) {
        create(_dims, _sizes, _type, _usageFlags);
    } ਨਹੀਂਤਾਂ {
        flags = (flags & ~Mat::TYPE_ਨਕਾਬ) | CV_MAT_TYPE(_type);
        ਪੂਰਨ dummy_ਮਾਪ = 0;
        ਸੈੱਟ_ਮਾਪ(*ਇਹ, (_dims >= 0 ? _dims : 1), (_dims >= 0 ? _sizes : &dummy_ਮਾਪ), ਖਾਲੀ_ਸੂਚਕ, ਸੱਚ);
        finalizeHdr(*ਇਹ);
    }
}

ਖਾਲੀ UMat::fit(ਅਚਲ std::vector<ਪੂਰਨ>& _shape, ਪੂਰਨ _type, UMatUsageFlags _usageFlags)
{
    fit((ਪੂਰਨ)_shape.size(), _shape.ਅੰਕੜੇ(), _type, _usageFlags);
}

ਖਾਲੀ UMat::fit(ਅਚਲ MatShape& _shape, ਪੂਰਨ _type, UMatUsageFlags _usageFlags)
{
    fit(_shape.dims, _shape.p, _type, _usageFlags);
    size.layout = _shape.layout;
    size.C = _shape.C;
}

ਖਾਲੀ UMat::fit(ਪੂਰਨ ਪੰਕਤੀਆਂ_3, ਪੂਰਨ ਖਾਨੇ_ਗਿਣਤੀ_3, ਪੂਰਨ _type, UMatUsageFlags _usageFlags)
{
    _type &= TYPE_ਨਕਾਬ;
    ਪੂਰਨ sz[] = {ਪੰਕਤੀਆਂ_3, ਖਾਨੇ_ਗਿਣਤੀ_3};
    fit(2, sz, _type, _usageFlags);
}

ਖਾਲੀ UMat::fit(ਮਾਪ _sz, ਪੂਰਨ _type, UMatUsageFlags _usageFlags)
{
    fit(_sz.ਉਚਾਈ, _sz.ਚੌੜਾਈ, _type, _usageFlags);
}

ਖਾਲੀ UMat::fit_Same_ਮਾਪ(ਨਿਵੇਸ਼_Array m, ਪੂਰਨ _type, UMatUsageFlags _usageFlags)
{
    fit(m.shape(), _type, _usageFlags);
}

ਖਾਲੀ UMat::copy_ਮਾਪ(ਅਚਲ UMat& m)
{
    dims = m.dims;
    ਖਾਨੇ_ਗਿਣਤੀ = m.ਖਾਨੇ_ਗਿਣਤੀ;
    ਪੰਕਤੀਆਂ = m.ਪੰਕਤੀਆਂ;
    size = m.size;
    ਕਦਮ = m.ਕਦਮ;
}

UMat::~UMat()
{
    release();
}

ਖਾਲੀ UMat::deallocate()
{
    UMat_ਅੰਕੜੇ* u_ = u;
    u = ਨਿਰਾ;
    u_->currAllocator->deallocate(u_);
}


UMat::UMat(ਅਚਲ UMat& m, ਅਚਲ Range& ਪੰਕਤੀ_Range_3, ਅਚਲ Range& ਖਾਨਾ_Range_3)
    : flags(MAGIC_VAL), dims(0), ਪੰਕਤੀਆਂ(0), ਖਾਨੇ_ਗਿਣਤੀ(0), allocator(0), usageFlags(USAGE_DEFAULT), u(0), offset(0)
{
    CV_Assert( m.dims >= 2 && m.dims <= MatShape::MAX_DIMS );
    ਜੇ( m.dims > 2 )
    {
        Range rs[MatShape::MAX_DIMS];
        rs[0] = ਪੰਕਤੀ_Range_3;
        rs[1] = ਖਾਨਾ_Range_3;
        ਲਈ( ਪੂਰਨ i = 2; i < m.dims; i++ )
            rs[i] = Range::all();
        *ਇਹ = m(rs);
        ਵਾਪਸ;
    }

    *ਇਹ = m;
    ਜੇ( ਪੰਕਤੀ_Range_3 != Range::all() && ਪੰਕਤੀ_Range_3 != Range(0,ਪੰਕਤੀਆਂ) )
    {
        CV_Assert( 0 <= ਪੰਕਤੀ_Range_3.ਸ਼ੁਰੂ &&
                  ਪੰਕਤੀ_Range_3.ਸ਼ੁਰੂ <= ਪੰਕਤੀ_Range_3.end &&
                  ਪੰਕਤੀ_Range_3.end <= m.ਪੰਕਤੀਆਂ );
        size[0] = ਪੰਕਤੀਆਂ = ਪੰਕਤੀ_Range_3.size();
        offset += ਕਦਮ*ਪੰਕਤੀ_Range_3.ਸ਼ੁਰੂ;
        flags |= SUBMATRIX_FLAG;
    }

    ਜੇ( ਖਾਨਾ_Range_3 != Range::all() && ਖਾਨਾ_Range_3 != Range(0,ਖਾਨੇ_ਗਿਣਤੀ) )
    {
        CV_Assert( 0 <= ਖਾਨਾ_Range_3.ਸ਼ੁਰੂ &&
                  ਖਾਨਾ_Range_3.ਸ਼ੁਰੂ <= ਖਾਨਾ_Range_3.end &&
                  ਖਾਨਾ_Range_3.end <= m.ਖਾਨੇ_ਗਿਣਤੀ );
        size[1] = ਖਾਨੇ_ਗਿਣਤੀ = ਖਾਨਾ_Range_3.size();
        offset += ਖਾਨਾ_Range_3.ਸ਼ੁਰੂ*elem_ਮਾਪ();
        flags |= SUBMATRIX_FLAG;
    }

    updateContinuityFlag();

    ਜੇ( ਪੰਕਤੀਆਂ <= 0 || ਖਾਨੇ_ਗਿਣਤੀ <= 0 )
    {
        release();
        ਪੰਕਤੀਆਂ = ਖਾਨੇ_ਗਿਣਤੀ = 0;
    }
}


UMat::UMat(ਅਚਲ UMat& m, ਅਚਲ Rect& roi)
    : flags(m.flags), dims(2), ਪੰਕਤੀਆਂ(roi.ਉਚਾਈ), ਖਾਨੇ_ਗਿਣਤੀ(roi.ਚੌੜਾਈ),
    allocator(m.allocator), usageFlags(m.usageFlags), u(m.u),
    offset(m.offset + roi.y*m.ਕਦਮ[0]), size(2)
{
    CV_Assert( m.dims <= 2 );

    size_t esz = CV_ELEM_ਮਾਪ(flags);
    offset += roi.x*esz;
    CV_Assert( 0 <= roi.x && 0 <= roi.ਚੌੜਾਈ && roi.x + roi.ਚੌੜਾਈ <= m.ਖਾਨੇ_ਗਿਣਤੀ &&
              0 <= roi.y && 0 <= roi.ਉਚਾਈ && roi.y + roi.ਉਚਾਈ <= m.ਪੰਕਤੀਆਂ );
    ਜੇ( roi.ਚੌੜਾਈ < m.ਖਾਨੇ_ਗਿਣਤੀ || roi.ਉਚਾਈ < m.ਪੰਕਤੀਆਂ )
        flags |= SUBMATRIX_FLAG;

    size[0] = ਪੰਕਤੀਆਂ;
    size[1] = ਖਾਨੇ_ਗਿਣਤੀ;
    ਕਦਮ[0] = m.dims == 2 ? m.ਕਦਮ[0] : ਖਾਨੇ_ਗਿਣਤੀ*esz;
    ਕਦਮ[1] = esz;

    updateContinuityFlag();

    addref();
    ਜੇ( ਪੰਕਤੀਆਂ <= 0 || ਖਾਨੇ_ਗਿਣਤੀ <= 0 )
    {
        ਪੰਕਤੀਆਂ = ਖਾਨੇ_ਗਿਣਤੀ = 0;
        release();
    }
}


UMat::UMat(ਅਚਲ UMat& m, ਅਚਲ Range* ranges)
    : flags(MAGIC_VAL), dims(0), ਪੰਕਤੀਆਂ(0), ਖਾਨੇ_ਗਿਣਤੀ(0), allocator(0), usageFlags(USAGE_DEFAULT), u(0), offset(0)
{
    ਪੂਰਨ i, d = m.dims;

    CV_Assert(ranges);
    ਲਈ( i = 0; i < d; i++ )
    {
        Range r = ranges[i];
        CV_Assert( r == Range::all() || (0 <= r.ਸ਼ੁਰੂ && r.ਸ਼ੁਰੂ < r.end && r.end <= m.size[i]) );
    }
    *ਇਹ = m;
    ਲਈ( i = 0; i < d; i++ )
    {
        Range r = ranges[i];
        ਜੇ( r != Range::all() && r != Range(0, size.p[i]))
        {
            size.p[i] = r.end - r.ਸ਼ੁਰੂ;
            offset += r.ਸ਼ੁਰੂ*ਕਦਮ.p[i];
            flags |= SUBMATRIX_FLAG;
        }
    }

    ਜੇ (d <= 2) {
        ਪੰਕਤੀਆਂ = d == 2 ? size[0] : 1;
        ਖਾਨੇ_ਗਿਣਤੀ = d <= 0 ? (d >= 0) : size[d > 1];
    }
    updateContinuityFlag();
}

UMat::UMat(ਅਚਲ UMat& m, ਅਚਲ std::vector<Range>& ranges)
    : flags(MAGIC_VAL), dims(0), ਪੰਕਤੀਆਂ(0), ਖਾਨੇ_ਗਿਣਤੀ(0), allocator(0), usageFlags(USAGE_DEFAULT), u(0), offset(0)
{
    ਪੂਰਨ i, d = m.dims;

    CV_Assert((ਪੂਰਨ)ranges.size() == d);
    ਲਈ (i = 0; i < d; i++)
    {
        Range r = ranges[i];
        CV_Assert(r == Range::all() || (0 <= r.ਸ਼ੁਰੂ && r.ਸ਼ੁਰੂ < r.end && r.end <= m.size[i]));
    }
    *ਇਹ = m;
    ਲਈ (i = 0; i < d; i++)
    {
        Range r = ranges[i];
        ਜੇ (r != Range::all() && r != Range(0, size.p[i]))
        {
            size.p[i] = r.end - r.ਸ਼ੁਰੂ;
            offset += r.ਸ਼ੁਰੂ*ਕਦਮ.p[i];
            flags |= SUBMATRIX_FLAG;
        }
    }

    ਜੇ (d <= 2) {
        ਪੰਕਤੀਆਂ = d == 2 ? size[0] : 1;
        ਖਾਨੇ_ਗਿਣਤੀ = d <= 0 ? (d >= 0) : size[d > 1];
    }
    updateContinuityFlag();
}

UMat UMat::diag(ਪੂਰਨ d) ਅਚਲ
{
    CV_Assert( dims <= 2 );
    UMat m = *ਇਹ;
    size_t esz = elem_ਮਾਪ();
    ਪੂਰਨ len;

    ਜੇ( d >= 0 )
    {
        len = std::min(ਖਾਨੇ_ਗਿਣਤੀ - d, ਪੰਕਤੀਆਂ);
        m.offset += esz*d;
    }
    ਨਹੀਂਤਾਂ
    {
        len = std::min(ਪੰਕਤੀਆਂ + d, ਖਾਨੇ_ਗਿਣਤੀ);
        m.offset -= ਕਦਮ[0]*d;
    }
    CV_DbgAssert( len > 0 );

    m.size[0] = m.ਪੰਕਤੀਆਂ = len;
    m.size[1] = m.ਖਾਨੇ_ਗਿਣਤੀ = 1;
    m.ਕਦਮ[0] += (len > 1 ? esz : 0);

    m.updateContinuityFlag();

    ਜੇ( size() != ਮਾਪ(1,1) )
        m.flags |= SUBMATRIX_FLAG;

    ਵਾਪਸ m;
}

ਖਾਲੀ UMat::locateROI( ਮਾਪ& ਸਾਰਾ_ਮਾਪ, ਬਿੰਦੂ& ofs ) ਅਚਲ
{
    CV_Assert( dims <= 2 && ਕਦਮ[0] > 0 );
    size_t esz = elem_ਮਾਪ(), minstep;
    ptrdiff_t delta1 = (ptrdiff_t)offset, delta2 = (ptrdiff_t)u->size;

    ਜੇ( delta1 == 0 )
        ofs.x = ofs.y = 0;
    ਨਹੀਂਤਾਂ
    {
        ofs.y = (ਪੂਰਨ)(delta1/ਕਦਮ[0]);
        ofs.x = (ਪੂਰਨ)((delta1 - ਕਦਮ[0]*ofs.y)/esz);
        CV_DbgAssert( offset == (size_t)(ofs.y*ਕਦਮ[0] + ofs.x*esz) );
    }
    minstep = (ofs.x + ਖਾਨੇ_ਗਿਣਤੀ)*esz;
    ਸਾਰਾ_ਮਾਪ.ਉਚਾਈ = (ਪੂਰਨ)((delta2 - minstep)/ਕਦਮ[0] + 1);
    ਸਾਰਾ_ਮਾਪ.ਉਚਾਈ = std::max(ਸਾਰਾ_ਮਾਪ.ਉਚਾਈ, ofs.y + ਪੰਕਤੀਆਂ);
    ਸਾਰਾ_ਮਾਪ.ਚੌੜਾਈ = (ਪੂਰਨ)((delta2 - ਕਦਮ*(ਸਾਰਾ_ਮਾਪ.ਉਚਾਈ-1))/esz);
    ਸਾਰਾ_ਮਾਪ.ਚੌੜਾਈ = std::max(ਸਾਰਾ_ਮਾਪ.ਚੌੜਾਈ, ofs.x + ਖਾਨੇ_ਗਿਣਤੀ);
}


UMat& UMat::adjustROI( ਪੂਰਨ dtop, ਪੂਰਨ dbottom, ਪੂਰਨ dleft, ਪੂਰਨ dright )
{
    CV_Assert( dims == 2 && ਕਦਮ[0] > 0 );
    ਮਾਪ ਸਾਰਾ_ਮਾਪ; ਬਿੰਦੂ ofs;
    size_t esz = elem_ਮਾਪ();
    locateROI( ਸਾਰਾ_ਮਾਪ, ofs );
    ਪੂਰਨ row1 = std::min(std::max(ofs.y - dtop, 0), ਸਾਰਾ_ਮਾਪ.ਉਚਾਈ), row2 = std::max(0, std::min(ofs.y + ਪੰਕਤੀਆਂ + dbottom, ਸਾਰਾ_ਮਾਪ.ਉਚਾਈ));
    ਪੂਰਨ col1 = std::min(std::max(ofs.x - dleft, 0), ਸਾਰਾ_ਮਾਪ.ਚੌੜਾਈ), col2 = std::max(0, std::min(ofs.x + ਖਾਨੇ_ਗਿਣਤੀ + dright, ਸਾਰਾ_ਮਾਪ.ਚੌੜਾਈ));
    ਜੇ(row1 > row2)
        std::swap(row1, row2);
    ਜੇ(col1 > col2)
        std::swap(col1, col2);

    offset += (row1 - ofs.y)*ਕਦਮ + (col1 - ofs.x)*esz;
    ਪੰਕਤੀਆਂ = row2 - row1; ਖਾਨੇ_ਗਿਣਤੀ = col2 - col1;
    size.p[0] = ਪੰਕਤੀਆਂ; size.p[1] = ਖਾਨੇ_ਗਿਣਤੀ;
    updateContinuityFlag();
    ਵਾਪਸ *ਇਹ;
}


UMat UMat::reshape(ਪੂਰਨ new_cn, ਪੂਰਨ new_ਪੰਕਤੀਆਂ) ਅਚਲ
{
    ਪੂਰਨ cn = channels();
    UMat hdr = *ਇਹ;

    ਜੇ( dims > 2 && new_ਪੰਕਤੀਆਂ == 0 && new_cn != 0 && size[dims-1]*cn % new_cn == 0 )
    {
        hdr.flags = (hdr.flags & ~CV_MAT_CN_ਨਕਾਬ) | ((new_cn-1) << CV_CN_ਖਿਸਕਾਓ);
        hdr.ਕਦਮ[dims-1] = CV_ELEM_ਮਾਪ(hdr.flags);
        hdr.size[dims-1] = hdr.size[dims-1]*cn / new_cn;
        ਵਾਪਸ hdr;
    }

    CV_Assert( dims <= 2 );

    ਜੇ( new_cn == 0 )
        new_cn = cn;

    ਪੂਰਨ ਕੁੱਲ_ਚੌੜਾਈ = ਖਾਨੇ_ਗਿਣਤੀ * cn;

    ਜੇ( (new_cn > ਕੁੱਲ_ਚੌੜਾਈ || ਕੁੱਲ_ਚੌੜਾਈ % new_cn != 0) && new_ਪੰਕਤੀਆਂ == 0 )
        new_ਪੰਕਤੀਆਂ = ਪੰਕਤੀਆਂ * ਕੁੱਲ_ਚੌੜਾਈ / new_cn;

    ਜੇ( new_ਪੰਕਤੀਆਂ != 0 && new_ਪੰਕਤੀਆਂ != ਪੰਕਤੀਆਂ )
    {
        ਪੂਰਨ ਕੁੱਲ_ਮਾਪ_2 = ਕੁੱਲ_ਚੌੜਾਈ * ਪੰਕਤੀਆਂ;
        ਜੇ( !isContinuous() )
            CV_Error( cv::Error::Bad_ਕਦਮ,
            "The matrix is not continuous, thus its number of rows can not be changed" );

        ਕੁੱਲ_ਚੌੜਾਈ = ਕੁੱਲ_ਮਾਪ_2 / new_ਪੰਕਤੀਆਂ;

        ਜੇ( ਕੁੱਲ_ਚੌੜਾਈ * new_ਪੰਕਤੀਆਂ != ਕੁੱਲ_ਮਾਪ_2 )
            CV_Error( cv::Error::StsBadArg, "The total number of matrix elements "
                                    "is not divisible by the new number of rows" );

        hdr.size[0] = hdr.ਪੰਕਤੀਆਂ = new_ਪੰਕਤੀਆਂ;
        hdr.ਕਦਮ[0] = ਕੁੱਲ_ਚੌੜਾਈ * elemSize1();
    } ਨਹੀਂਤਾਂ {
        hdr.size[0] = hdr.ਪੰਕਤੀਆਂ = ਪੰਕਤੀਆਂ;
        ਜੇ (dims <= 1)
            hdr.ਕਦਮ[0] = ਖਾਨੇ_ਗਿਣਤੀ * CV_ELEM_ਮਾਪ(flags);
    }

    ਪੂਰਨ new_ਚੌੜਾਈ = ਕੁੱਲ_ਚੌੜਾਈ / new_cn;

    ਜੇ( new_ਚੌੜਾਈ * new_cn != ਕੁੱਲ_ਚੌੜਾਈ )
        CV_Error( cv::Error::Bad_ਅੰਕ_Channels,
        "The total width is not divisible by the new number of channels" );

    hdr.size.dims = hdr.dims = 2;
    hdr.size[1] = hdr.ਖਾਨੇ_ਗਿਣਤੀ = new_ਚੌੜਾਈ;
    hdr.flags = (hdr.flags & ~CV_MAT_CN_ਨਕਾਬ) | ((new_cn-1) << CV_CN_ਖਿਸਕਾਓ);
    hdr.ਕਦਮ[1] = CV_ELEM_ਮਾਪ(hdr.flags);
    ਵਾਪਸ hdr;
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_OPENCL
ਨਾਮਾਕਾਸ਼ {
ਸਥਿਰ ਤਾਰਕਿਕ ocl_ਸੈੱਟ_Diag(ਅਚਲ UMat& d, UMat& m, ਪੂਰਨ len)
{
    ਪੂਰਨ cn = d.channels();
    ਪੂਰਨ ਡੂੰਘਾਈ = d.ਡੂੰਘਾਈ();

    ਜੇ (ਡੂੰਘਾਈ == CV_64F && !ocl::Device::getDefault().doubleFPConfig())
        ਵਾਪਸ ਝੂਠ;

    String opts = format("-D SET_DIAG -D T1=%s -D cn=%d -D IS_ROW_VECTOR=%d",
                         ocl::memop_Type_ਮੰਜ਼ਿਲ_Str(ਡੂੰਘਾਈ),
                         cn,
                         (d.ਪੰਕਤੀਆਂ == 1) ? 1 : 0);

    ocl::Kernel k("setDiag", ocl::core::copyset_oclsrc, opts);
    ਜੇ (k.empty())
        ਵਾਪਸ ਝੂਠ;

    k.ਦਲੀਲਾਂ(ocl::KernelArg::WriteOnly(m),
           ocl::KernelArg::Read_Only_No_ਮਾਪ(d),
           len);

    size_t globalsize[2] = { (size_t)len, (size_t)len };
    ਵਾਪਸ k.ਲੜੀਵਾਰ(2, globalsize, ਨਿਰਾ, ਝੂਠ);
}
}
#ਜੇ_ਅੰਤ

UMat UMat::diag(ਅਚਲ UMat& d, UMatUsageFlags usageFlags)
{
    CV_INSTRUMENT_REGION();

    CV_Assert(d.ਖਾਨੇ_ਗਿਣਤੀ == 1 || d.ਪੰਕਤੀਆਂ == 1);
    ਪੂਰਨ len = d.ਪੰਕਤੀਆਂ + d.ਖਾਨੇ_ਗਿਣਤੀ - 1;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_OPENCL
    ਜੇ (ocl::useOpenCL())
    {
        UMat m(len, len, d.type(), usageFlags);
        ਜੇ (ocl_ਸੈੱਟ_Diag(d, m, len))
        {
            CV_IMPL_ਜੋੜੋ(CV_IMPL_OCL);
            ਵਾਪਸ m;
        }
    }
#ਜੇ_ਅੰਤ

    UMat m(len, len, d.type(), Scalar(0), usageFlags);
    UMat md = m.diag();
    ਜੇ( d.ਖਾਨੇ_ਗਿਣਤੀ == 1 )
        d.copy_ਮੰਜ਼ਿਲ(md);
    ਨਹੀਂਤਾਂ
        transpose(d, md);
    ਵਾਪਸ m;
}

ਪੂਰਨ UMat::check_ਸਦਿਸ਼(ਪੂਰਨ _elemChannels, ਪੂਰਨ ਡੂੰਘਾਈ_2, ਤਾਰਕਿਕ _requireContinuous) ਅਚਲ
{
    ਵਾਪਸ (ਡੂੰਘਾਈ() == ਡੂੰਘਾਈ_2 || ਡੂੰਘਾਈ_2 <= 0) &&
        (isContinuous() || !_requireContinuous) &&
        ((dims <= 2 && (((ਪੰਕਤੀਆਂ == 1 || ਖਾਨੇ_ਗਿਣਤੀ == 1) && channels() == _elemChannels) ||
                        (ਖਾਨੇ_ਗਿਣਤੀ == _elemChannels && channels() == 1))) ||
        (dims == 3 && channels() == 1 && size.p[2] == _elemChannels && (size.p[0] == 1 || size.p[1] == 1) &&
         (isContinuous() || ਕਦਮ.p[1] == ਕਦਮ.p[2]*size.p[2])))
    ? (ਪੂਰਨ)(ਕੁੱਲ()*channels()/_elemChannels) : -1;
}

UMat UMat::reshape(ਪੂਰਨ _cn, ਪੂਰਨ _newndims, ਅਚਲ ਪੂਰਨ* _newsz) ਅਚਲ
{
    ਜੇ(_newndims == dims)
    {
        ਜੇ(_newsz == 0)
            ਵਾਪਸ reshape(_cn);
        ਜੇ(_newndims == 2)
            ਵਾਪਸ reshape(_cn, _newsz[0]);
    }

    ਜੇ (isContinuous())
    {
        CV_Assert(_cn >= 0 && _newndims >= 0 && _newndims <= CV_MAX_DIM && (_newndims == 0 || _newsz != 0));

        ਜੇ (_cn == 0)
            _cn = ਇਹ->channels();
        ਨਹੀਂਤਾਂ
            CV_Assert(_cn <= CV_CN_MAX);

        size_t ਕੁੱਲ_elem1_ref = ਇਹ->ਕੁੱਲ() * ਇਹ->channels();
        size_t ਕੁੱਲ_elem1 = _cn;

        Auto_ਬਫਰ<ਪੂਰਨ, 4> newsz_buf( (size_t)std::max(_newndims, 1) );

        ਪੂਰਨ m1_idx = -1;

        ਲਈ (ਪੂਰਨ i = 0; i < _newndims; i++)
        {
            ਜੇ (_newsz[i] >= 0) {
                ਜੇ (_newsz[i] == 0 && i < dims)
                    newsz_buf[i] = size.p[i];
                ਨਹੀਂਤਾਂ
                    newsz_buf[i] = _newsz[i];
                ਕੁੱਲ_elem1 *= (size_t)newsz_buf[i];
            } ਨਹੀਂਤਾਂ {
                ਜੇ (m1_idx >= 0)
                    CV_Error(cv::Error::Sts_Bad_ਮਾਪ, "More than one '-1' occured in the new shape");
                m1_idx = i;
            }
        }

        ਜੇ (m1_idx >= 0) {
            ਜੇ (ਕੁੱਲ_elem1 == 0) {
                CV_Assert(ਕੁੱਲ_elem1_ref == 0);
                ਕੁੱਲ_elem1 = 1;
            }
            CV_Assert(ਕੁੱਲ_elem1_ref % ਕੁੱਲ_elem1 == 0);
            newsz_buf[m1_idx] = (ਪੂਰਨ)(ਕੁੱਲ_elem1_ref / ਕੁੱਲ_elem1);
        } ਨਹੀਂਤਾਂ ਜੇ (ਕੁੱਲ_elem1 != ਕੁੱਲ_elem1_ref) {
            CV_Error(cv::Error::StsUnmatchedSizes, "Requested and source matrices have different count of elements");
        }

        UMat hdr = *ਇਹ;
        hdr.flags = (hdr.flags & ~CV_MAT_CN_ਨਕਾਬ) | ((_cn-1) << CV_CN_ਖਿਸਕਾਓ);
        ਸੈੱਟ_ਮਾਪ(hdr, _newndims, newsz_buf.ਅੰਕੜੇ(), ਨਿਰਾ, ਸੱਚ);

        ਵਾਪਸ hdr;
    }

    CV_Error(cv::Error::StsNotImplemented, "Reshaping of n-dimensional non-continuous matrices is not supported yet");
}

UMat UMat::reshape(ਪੂਰਨ _cn, ਅਚਲ MatShape& _newshape) ਅਚਲ
{
    ਜੇ (_newshape.dims < 0) {
        ਪੂਰਨ newshape[] = {0};
        ਵਾਪਸ reshape(_cn, 1, newshape);
    }
    ਵਾਪਸ reshape(_cn, _newshape.dims, _newshape.p);
}

Mat UMat::getMat(AccessFlag accessFlags) ਅਚਲ
{
    ਜੇ(!u)
        ਵਾਪਸ Mat();
    // TODO Support ACCESS_READ (ACCESS_WRITE) without unnecessary data transfers
    accessFlags |= ACCESS_RW;
    UMat_ਅੰਕੜੇ_Auto_ਤਾਲਾ autolock(u);
    ਕੋਸ਼ਿਸ਼
    {
        ਜੇ(CV_XADD(&u->refcount, 1) == 0)
            u->currAllocator->map(u, accessFlags);
        ਜੇ (u->ਅੰਕੜੇ != 0)
        {
            Mat hdr(dims, size.p, type(), u->ਅੰਕੜੇ + offset, ਕਦਮ.p);
            hdr.flags = flags;
            hdr.u = u;
            hdr.datastart = u->ਅੰਕੜੇ;
            hdr.ਅੰਕੜੇ = u->ਅੰਕੜੇ + offset;
            hdr.datalimit = hdr.dataend = u->ਅੰਕੜੇ + u->size;
            ਵਾਪਸ hdr;
        }
    }
    ਫੜੋ(...)
    {
        CV_XADD(&u->refcount, -1);
        ਸੁੱਟੋ;
    }
    CV_XADD(&u->refcount, -1);
    CV_Assert(u->ਅੰਕੜੇ != 0 && "Error mapping of UMat to host memory.");
    ਵਾਪਸ Mat();
}

ਖਾਲੀ* UMat::handle(AccessFlag accessFlags) ਅਚਲ
{
    ਜੇ( !u )
        ਵਾਪਸ 0;

    CV_Assert(u->refcount == 0);
    CV_Assert(!u->deviceCopyObsolete() || u->copyOnMap());
    ਜੇ (u->deviceCopyObsolete())
    {
        u->currAllocator->unmap(u);
    }

    ਜੇ (!!(accessFlags & ACCESS_WRITE))
        u->markHostCopyObsolete(ਸੱਚ);

    ਵਾਪਸ u->handle;
}

ਖਾਲੀ UMat::ndoffset(size_t* ofs) ਅਚਲ
{
    // offset = step[0]*ofs[0] + step[1]*ofs[1] + step[2]*ofs[2] + ...;
    size_t val = offset;
    ਲਈ( ਪੂਰਨ i = 0; i < dims; i++ )
    {
        size_t s = ਕਦਮ.p[i];
        ofs[i] = val / s;
        val -= ofs[i]*s;
    }
}

ਖਾਲੀ UMat::copy_ਮੰਜ਼ਿਲ(OutputArray _dst) ਅਚਲ
{
    CV_INSTRUMENT_REGION();

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
    ਜੇ (_dst.isGpuMat())
    {
        _dst.getGpuMat().upload(*ਇਹ);
        ਵਾਪਸ;
    }
#ਜੇ_ਅੰਤ

    ਪੂਰਨ stype = type();
    ਪੂਰਨ dtype = _dst.type();
    ਜੇ( _dst.fixedType() && dtype != stype )
    {
        CV_Assert( CV_MAT_CN(stype) == CV_MAT_CN(dtype) );
        convert_ਮੰਜ਼ਿਲ( _dst, dtype );
        ਵਾਪਸ;
    }

    ਜੇ( dims == 0 && empty() )
    {
        _dst.release();
        ਖਾਲੀ* obj = _dst.getObj();
        ਜੇ (_dst.isMat())
            reinterpret_cast<Mat*>(obj)->flags = Mat::MAGIC_VAL | Mat::CONTINUOUS_FLAG | stype;
        ਨਹੀਂਤਾਂ ਜੇ (_dst.isUMat())
            reinterpret_cast<UMat*>(obj)->flags = UMat::MAGIC_VAL | UMat::CONTINUOUS_FLAG | stype;
        ਨਹੀਂਤਾਂ ਜੇ (_dst.isGpuMat())
            reinterpret_cast<cuda::GpuMat*>(obj)->flags = stype;
        ਵਾਪਸ;
    }

    _dst.create( dims, size.p, stype );
    ਜੇ (empty())
        ਵਾਪਸ;

    size_t sz[CV_MAX_DIM] = {1}, srcofs[CV_MAX_DIM]={0}, dstofs[CV_MAX_DIM]={0};
    size_t esz = CV_ELEM_ਮਾਪ(stype);
    ਪੂਰਨ i, d = std::max(dims, 1);
    ਲਈ( i = 0; i < d; i++ )
        sz[i] = size.p[i];
    sz[d-1] *= esz;
    ndoffset(srcofs);
    srcofs[d-1] *= esz;

    ਜੇ( _dst.isUMat() )
    {
        UMat dst = _dst.getUMat();
        CV_Assert(dst.u);
        ਜੇ( u == dst.u && dst.offset == offset )
            ਵਾਪਸ;

        ਜੇ (u->currAllocator == dst.u->currAllocator)
        {
            dst.ndoffset(dstofs);
            dstofs[dims-1] *= esz;
            u->currAllocator->copy(u, dst.u, d, sz, srcofs, ਕਦਮ.p, dstofs, dst.ਕਦਮ.p, ਝੂਠ);
            ਵਾਪਸ;
        }

        // different allocators: never map a device destination, upload into it instead
        // (through a host Mat when both sides are device memory)
        MatAllocator* hostAllocator = Mat::getDefaultAllocator();
        ਜੇ (dst.u->currAllocator != hostAllocator)
        {
            ਜੇ (u->currAllocator == hostAllocator)
                getMat(ACCESS_READ).copy_ਮੰਜ਼ਿਲ(_dst);
            ਨਹੀਂਤਾਂ
            {
                Mat tmp;
                copy_ਮੰਜ਼ਿਲ(tmp);
                tmp.copy_ਮੰਜ਼ਿਲ(_dst);
            }
            ਵਾਪਸ;
        }
    }

    Mat dst = _dst.getMat();
    u->currAllocator->download(u, dst.ptr(), d, sz, srcofs, ਕਦਮ.p, dst.ਕਦਮ.p);
}

ਖਾਲੀ UMat::copy_ਮੰਜ਼ਿਲ(OutputArray _dst, ਨਿਵੇਸ਼_Array ਨਕਾਬ_2) ਅਚਲ
{
    CV_INSTRUMENT_REGION();

    ਜੇ( ਨਕਾਬ_2.empty() )
    {
        copy_ਮੰਜ਼ਿਲ(_dst);
        ਵਾਪਸ;
    }
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_OPENCL
    ਪੂਰਨ cn = channels(), mtype = ਨਕਾਬ_2.type(), mdepth = CV_MAT_ਡੂੰਘਾਈ(mtype), mcn = CV_MAT_CN(mtype);
    CV_Assert( (mdepth == CV_8U || mdepth == CV_8S || mdepth == CV_Bool) && (mcn == 1 || mcn == cn) );

    ਜੇ (ocl::useOpenCL() && _dst.isUMat() && dims <= 2)
    {
        UMat_ਅੰਕੜੇ * prevu = _dst.getUMat().u;
        _dst.create( size, type() );

        UMat dst = _dst.getUMat();

        ਤਾਰਕਿਕ haveDstUninit = ਝੂਠ;
        ਜੇ( prevu != dst.u ) // do not leave dst uninitialized
            haveDstUninit = ਸੱਚ;

        String opts = format("-D COPY_TO_MASK -D T1=%s -D scn=%d -D mcn=%d%s",
                             ocl::memop_Type_ਮੰਜ਼ਿਲ_Str(ਡੂੰਘਾਈ()), cn, mcn,
                             haveDstUninit ? " -D HAVE_DST_UNINIT" : "");

        ocl::Kernel k("copyToMask", ocl::core::copyset_oclsrc, opts);
        ਜੇ (!k.empty())
        {
            k.ਦਲੀਲਾਂ(ocl::KernelArg::Read_Only_No_ਮਾਪ(*ਇਹ),
                   ocl::KernelArg::Read_Only_No_ਮਾਪ(ਨਕਾਬ_2.getUMat()),
                   haveDstUninit ? ocl::KernelArg::WriteOnly(dst) :
                                   ocl::KernelArg::ReadWrite(dst));

            size_t globalsize[2] = { (size_t)ਖਾਨੇ_ਗਿਣਤੀ, (size_t)ਪੰਕਤੀਆਂ };
            ਜੇ (k.ਲੜੀਵਾਰ(2, globalsize, ਨਿਰਾ, ਝੂਠ))
            {
                CV_IMPL_ਜੋੜੋ(CV_IMPL_OCL);
                ਵਾਪਸ;
            }
        }
    }
#ਜੇ_ਅੰਤ
    Mat src = getMat(ACCESS_READ);
    src.copy_ਮੰਜ਼ਿਲ(_dst, ਨਕਾਬ_2);
}


//
// void UMat::convertTo moved to convert.dispatch.cpp
//

UMat& UMat::ਸੈੱਟ_ਮੰਜ਼ਿਲ(ਨਿਵੇਸ਼_Array ਮੁੱਲ, ਨਿਵੇਸ਼_Array ਨਕਾਬ_2)
{
    CV_INSTRUMENT_REGION();

    ਤਾਰਕਿਕ have_ਨਕਾਬ = !ਨਕਾਬ_2.empty();
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_OPENCL
    ਪੂਰਨ tp = type(), cn = CV_MAT_CN(tp), d = CV_MAT_ਡੂੰਘਾਈ(tp);

    ਜੇ( dims <= 2 && cn <= 4 && CV_MAT_ਡੂੰਘਾਈ(tp) < CV_64F && ocl::useOpenCL() )
    {
        Mat value = ਮੁੱਲ.getMat();
        CV_Assert( checkScalar(value, type(), ਮੁੱਲ.ਕਿਸਮ(), ਨਿਵੇਸ਼_Array_2::UMAT) );
        ਪੂਰਨ kercn = have_ਨਕਾਬ || cn == 3 ? cn : std::max(cn, ocl::predict_Optimal_ਸਦਿਸ਼_ਚੌੜਾਈ(*ਇਹ)),
                kertp = CV_MAKE_TYPE(d, kercn);

        ਦੁੱਗਣਾ buf[16] = { 0, 0, 0, 0, 0, 0, 0, 0,
                           0, 0, 0, 0, 0, 0, 0, 0 };
        convertAndUnrollScalar(value, tp, (uchar *)buf, kercn / cn);

        ਪੂਰਨ scalarcn = kercn == 3 ? 4 : kercn, ਪੰਕਤੀਆਂ_ਪ੍ਰਤੀ_WI = ocl::Device::getDefault().isIntel() ? 4 : 1;
        String opts = format("-D dstT=%s -D rowsPerWI=%d -D dstST=%s -D dstT1=%s -D cn=%d",
                             ocl::memop_Type_ਮੰਜ਼ਿਲ_Str(kertp), ਪੰਕਤੀਆਂ_ਪ੍ਰਤੀ_WI,
                             ocl::memop_Type_ਮੰਜ਼ਿਲ_Str(CV_MAKETYPE(d, scalarcn)),
                             ocl::memop_Type_ਮੰਜ਼ਿਲ_Str(d), kercn);

        ocl::Kernel ਸੈੱਟ_K(have_ਨਕਾਬ ? "setMask" : "set", ocl::core::copyset_oclsrc, opts);
        ਜੇ( !ਸੈੱਟ_K.empty() )
        {
            ocl::KernelArg scalararg(ocl::KernelArg::CONSTANT, 0, 0, 0, buf, CV_ELEM_ਮਾਪ(d) * scalarcn);
            UMat ਨਕਾਬ;

            ਜੇ( have_ਨਕਾਬ )
            {
                ਨਕਾਬ = ਨਕਾਬ_2.getUMat();
                CV_Assert( ਨਕਾਬ.size() == size() && (ਨਕਾਬ.type() == CV_8U || ਨਕਾਬ.type() == CV_8S || ਨਕਾਬ.type() == CV_Bool) );
                ocl::KernelArg maskarg = ocl::KernelArg::Read_Only_No_ਮਾਪ(ਨਕਾਬ),
                        dstarg = ocl::KernelArg::ReadWrite(*ਇਹ);
                ਸੈੱਟ_K.ਦਲੀਲਾਂ(maskarg, dstarg, scalararg);
            }
            ਨਹੀਂਤਾਂ
            {
                ocl::KernelArg dstarg = ocl::KernelArg::WriteOnly(*ਇਹ, cn, kercn);
                ਸੈੱਟ_K.ਦਲੀਲਾਂ(dstarg, scalararg);
            }

            size_t globalsize[] = { (size_t)ਖਾਨੇ_ਗਿਣਤੀ * cn / kercn, ((size_t)ਪੰਕਤੀਆਂ + ਪੰਕਤੀਆਂ_ਪ੍ਰਤੀ_WI - 1) / ਪੰਕਤੀਆਂ_ਪ੍ਰਤੀ_WI };
            ਜੇ( ਸੈੱਟ_K.ਲੜੀਵਾਰ(2, globalsize, ਨਿਰਾ, ਝੂਠ) )
            {
                CV_IMPL_ਜੋੜੋ(CV_IMPL_OCL);
                ਵਾਪਸ *ਇਹ;
            }
        }
    }
#ਜੇ_ਅੰਤ
    Mat m = getMat(have_ਨਕਾਬ ? ACCESS_RW : ACCESS_WRITE);
    m.ਸੈੱਟ_ਮੰਜ਼ਿਲ(ਮੁੱਲ, ਨਕਾਬ_2);
    ਵਾਪਸ *ਇਹ;
}

UMat& UMat::ਸੰਚਾਲਕ = (ਅਚਲ Scalar& s)
{
    ਸੈੱਟ_ਮੰਜ਼ਿਲ(s);
    ਵਾਪਸ *ਇਹ;
}

UMat UMat::t() ਅਚਲ
{
    UMat m;
    transpose(*ਇਹ, m);
    ਵਾਪਸ m;
}

UMat UMat::zeros(ਪੂਰਨ ਪੰਕਤੀਆਂ, ਪੂਰਨ ਖਾਨੇ_ਗਿਣਤੀ, ਪੂਰਨ type, UMatUsageFlags usageFlags)
{
    ਵਾਪਸ UMat(ਪੰਕਤੀਆਂ, ਖਾਨੇ_ਗਿਣਤੀ, type, Scalar::all(0), usageFlags);
}

UMat UMat::zeros(ਮਾਪ size, ਪੂਰਨ type, UMatUsageFlags usageFlags)
{
    ਵਾਪਸ UMat(size, type, Scalar::all(0), usageFlags);
}

UMat UMat::zeros(ਪੂਰਨ ndims, ਅਚਲ ਪੂਰਨ* sz, ਪੂਰਨ type, UMatUsageFlags usageFlags)
{
    ਵਾਪਸ UMat(ndims, sz, type, Scalar::all(0), usageFlags);
}

UMat UMat::zeros(ਅਚਲ MatShape& shape, ਪੂਰਨ type, UMatUsageFlags usageFlags)
{
    ਵਾਪਸ UMat(shape.dims, shape.p, type, Scalar::all(0), usageFlags);
}

UMat UMat::ones(ਪੂਰਨ ਪੰਕਤੀਆਂ, ਪੂਰਨ ਖਾਨੇ_ਗਿਣਤੀ, ਪੂਰਨ type, UMatUsageFlags usageFlags)
{
    ਵਾਪਸ UMat(ਪੰਕਤੀਆਂ, ਖਾਨੇ_ਗਿਣਤੀ, type, Scalar(1), usageFlags);
}

UMat UMat::ones(ਮਾਪ size, ਪੂਰਨ type, UMatUsageFlags usageFlags)
{
    ਵਾਪਸ UMat(size, type, Scalar(1), usageFlags);
}

UMat UMat::ones(ਪੂਰਨ ndims, ਅਚਲ ਪੂਰਨ* sz, ਪੂਰਨ type, UMatUsageFlags usageFlags)
{
    ਵਾਪਸ UMat(ndims, sz, type, Scalar(1), usageFlags);
}

UMat UMat::ones(ਅਚਲ MatShape& shape, ਪੂਰਨ type, UMatUsageFlags usageFlags)
{
    ਵਾਪਸ UMat(shape.dims, shape.p, type, Scalar(1), usageFlags);
}

}

/* End of file. */
