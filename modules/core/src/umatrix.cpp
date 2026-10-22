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

#ಅನ್ತರ್ಭಾವ "precomp.hpp"
#ಅನ್ತರ್ಭಾವ "opencl_kernels_core.hpp"
#ಅನ್ತರ್ಭಾವ "umatrix.hpp"

#ಅನ್ತರ್ಭಾವ <opencv2/core/utils/tls.hpp>

///////////////////////////////// UMat implementation ///////////////////////////////

ನಾಮಾಕಾಶ cv {

// forward decls, implementation is below in this file
ಶೂನ್ಯ ಗಣ_ಗಾತ್ರ(UMat& m, ಪೂರ್ಣಾಂಕ _dims, ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* _sz, ಸ್ಥಿರಾಂಕ size_t* ಹೆಜ್ಜೆಗಳು_2,
             ತಾರ್ಕಿಕ auto_ಹೆಜ್ಜೆಗಳು = ಸುಳ್ಳು);

ಶೂನ್ಯ updateContinuityFlag(UMat& m);
ಶೂನ್ಯ finalizeHdr(UMat& m);

UMat_ದತ್ತಾಂಶ::UMat_ದತ್ತಾಂಶ(ಸ್ಥಿರಾಂಕ MatAllocator* allocator)
{
    prevAllocator = currAllocator = allocator;
    urefcount = refcount = mapcount = 0;
    ದತ್ತಾಂಶ = origdata = 0;
    size = 0;
    flags = static_cast<UMat_ದತ್ತಾಂಶ::ಸ್ಮರಣೆ_Flag>(0);
    handle = 0;
    userdata = 0;
    allocatorFlags_ = 0;
    original_UMat_ದತ್ತಾಂಶ = ಖಾಲಿ;
}

UMat_ದತ್ತಾಂಶ::~UMat_ದತ್ತಾಂಶ()
{
    prevAllocator = currAllocator = 0;
    urefcount = refcount = 0;
    CV_Assert(mapcount == 0);
    ದತ್ತಾಂಶ = origdata = 0;
    size = 0;
    ತಾರ್ಕಿಕ isAsyncCleanup = !!(flags & UMat_ದತ್ತಾಂಶ::ASYNC_CLEANUP);
    flags = static_cast<UMat_ದತ್ತಾಂಶ::ಸ್ಮರಣೆ_Flag>(0);
    handle = 0;
    userdata = 0;
    allocatorFlags_ = 0;
    ಆದರೆ (original_UMat_ದತ್ತಾಂಶ)
    {
        ತಾರ್ಕಿಕ showWarn = ಸುಳ್ಳು;
        UMat_ದತ್ತಾಂಶ* u = original_UMat_ದತ್ತಾಂಶ;
        ತಾರ್ಕಿಕ zero_Ref = CV_XADD(&(u->refcount), -1) == 1;
        ಆದರೆ (zero_Ref)
        {
            // simulate Mat::deallocate
            ಆದರೆ (u->mapcount != 0)
            {
                (u->currAllocator ? u->currAllocator : /* TODO allocator ? allocator :*/ Mat::getDefaultAllocator())->unmap(u);
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                // we don't do "map", so we can't do "unmap"
            }
        }
        ತಾರ್ಕಿಕ zero_URef = CV_XADD(&(u->urefcount), -1) == 1;
        ಆದರೆ (zero_Ref && !zero_URef)
            showWarn = ನಿಜ;
        ಆದರೆ (zero_Ref && zero_URef) // oops, we need to free resources
        {
            showWarn = !isAsyncCleanup;
            // simulate UMat::deallocate
            u->currAllocator->deallocate(u);
        }
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ NDEBUG
        ಆದರೆ (showWarn)
        {
            ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ warn_ಸಂದೇಶ_showed = 0;
            ಆದರೆ (warn_ಸಂದೇಶ_showed++ < 100)
            {
                fflush(stdout);
                fprintf(stderr, "\n! OPENCV warning: getUMat()/getMat() call chain possible problem."
                                "\n!                 Base object is dead, while nested/derived object is still alive or processed."
                                "\n!                 Please check lifetime of UMat/Mat objects!\n");
                fflush(stderr);
            }
        }
#ಇಲ್ಲದಿದ್ದರೆ
        CV_ಬಳಸದ(showWarn);
#ಆದರೆ_ಅಂತ್ಯ
        original_UMat_ದತ್ತಾಂಶ = ಖಾಲಿ;
    }
}

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ OPENCV_DISABLE_THREAD_SUPPORT

// it should be a prime number for the best hash function
ಎಣಿಕೆ { UMAT_NLOCKS = 31 };
ಸ್ಥಿರ Mutex umatLocks[UMAT_NLOCKS];

ಸ್ಥಿರ size_t get_UMat_ದತ್ತಾಂಶ_ಬೀಗ_ಸೂಚ್ಯಂಕ(ಸ್ಥಿರಾಂಕ UMat_ದತ್ತಾಂಶ* u)
{
    size_t idx = ((size_t)(ಶೂನ್ಯ*)u) % UMAT_NLOCKS;
    ಹಿಂತಿರುಗಿ idx;
}

ಶೂನ್ಯ UMat_ದತ್ತಾಂಶ::lock()
{
    size_t idx = get_UMat_ದತ್ತಾಂಶ_ಬೀಗ_ಸೂಚ್ಯಂಕ(ಇದು);
    //printf("%d lock(%d)\n", cv::utils::getThreadID(), (int)idx);
    umatLocks[idx].lock();
}

ಶೂನ್ಯ UMat_ದತ್ತಾಂಶ::unlock()
{
    size_t idx = get_UMat_ದತ್ತಾಂಶ_ಬೀಗ_ಸೂಚ್ಯಂಕ(ಇದು);
    //printf("%d unlock(%d)\n", cv::utils::getThreadID(), (int)idx);
    umatLocks[idx].unlock();
}


// Do not allow several lock() calls with different UMatData objects.
ರಚನೆ UMat_ದತ್ತಾಂಶ_Auto_Locker
{
    ಪೂರ್ಣಾಂಕ usage_ಎಣಿಕೆ;
    UMat_ದತ್ತಾಂಶ* locked_objects[2];
    UMat_ದತ್ತಾಂಶ_Auto_Locker() : usage_ಎಣಿಕೆ(0) { locked_objects[0] = ಖಾಲಿ; locked_objects[1] = ಖಾಲಿ; }

    ಶೂನ್ಯ lock(UMat_ದತ್ತಾಂಶ*& u1)
    {
        ತಾರ್ಕಿಕ locked_1 = (u1 == locked_objects[0] || u1 == locked_objects[1]);
        ಆದರೆ (locked_1)
        {
            u1 = ಖಾಲಿ;
            ಹಿಂತಿರುಗಿ;
        }
        CV_Assert(usage_ಎಣಿಕೆ == 0);  // UMatDataAutoLock can't be used multiple times from the same thread
        usage_ಎಣಿಕೆ = 1;
        locked_objects[0] = u1;
        u1->lock();
    }
    ಶೂನ್ಯ lock(UMat_ದತ್ತಾಂಶ*& u1, UMat_ದತ್ತಾಂಶ*& u2)
    {
        ತಾರ್ಕಿಕ locked_1 = (u1 == locked_objects[0] || u1 == locked_objects[1]);
        ತಾರ್ಕಿಕ locked_2 = (u2 == locked_objects[0] || u2 == locked_objects[1]);
        ಆದರೆ (locked_1)
            u1 = ಖಾಲಿ;
        ಆದರೆ (locked_2)
            u2 = ಖಾಲಿ;
        ಆದರೆ (locked_1 && locked_2)
            ಹಿಂತಿರುಗಿ;
        CV_Assert(usage_ಎಣಿಕೆ == 0);  // UMatDataAutoLock can't be used multiple times from the same thread
        usage_ಎಣಿಕೆ = 1;
        locked_objects[0] = u1;
        locked_objects[1] = u2;
        ಆದರೆ (u1)
            u1->lock();
        ಆದರೆ (u2)
            u2->lock();
    }
    ಶೂನ್ಯ release(UMat_ದತ್ತಾಂಶ* u1, UMat_ದತ್ತಾಂಶ* u2)
    {
        ಆದರೆ (u1 == ಖಾಲಿ && u2 == ಖಾಲಿ)
            ಹಿಂತಿರುಗಿ;
        CV_Assert(usage_ಎಣಿಕೆ == 1);
        usage_ಎಣಿಕೆ = 0;
        ಆದರೆ (u1)
            u1->unlock();
        ಆದರೆ (u2)
            u2->unlock();
        locked_objects[0] = ಖಾಲಿ; locked_objects[1] = ಖಾಲಿ;
    }
};
ಸ್ಥಿರ TLSData<UMat_ದತ್ತಾಂಶ_Auto_Locker>& get_UMat_ದತ್ತಾಂಶ_Auto_Locker_TLS()
{
    CV_SINGLETON_LAZY_INIT_REF(TLSData<UMat_ದತ್ತಾಂಶ_Auto_Locker>, ಹೊಸ TLSData<UMat_ದತ್ತಾಂಶ_Auto_Locker>());
}
ಸ್ಥಿರ UMat_ದತ್ತಾಂಶ_Auto_Locker& get_UMat_ದತ್ತಾಂಶ_Auto_Locker() { ಹಿಂತಿರುಗಿ get_UMat_ದತ್ತಾಂಶ_Auto_Locker_TLS().getRef(); }


UMat_ದತ್ತಾಂಶ_Auto_ಬೀಗ::UMat_ದತ್ತಾಂಶ_Auto_ಬೀಗ(UMat_ದತ್ತಾಂಶ* u) : u1(u), u2(ಖಾಲಿ)
{
    get_UMat_ದತ್ತಾಂಶ_Auto_Locker().lock(u1);
}
UMat_ದತ್ತಾಂಶ_Auto_ಬೀಗ::UMat_ದತ್ತಾಂಶ_Auto_ಬೀಗ(UMat_ದತ್ತಾಂಶ* u1_, UMat_ದತ್ತಾಂಶ* u2_) : u1(u1_), u2(u2_)
{
    ಆದರೆ (get_UMat_ದತ್ತಾಂಶ_ಬೀಗ_ಸೂಚ್ಯಂಕ(u1) > get_UMat_ದತ್ತಾಂಶ_ಬೀಗ_ಸೂಚ್ಯಂಕ(u2))
    {
        std::swap(u1, u2);
    }
    get_UMat_ದತ್ತಾಂಶ_Auto_Locker().lock(u1, u2);
}
UMat_ದತ್ತಾಂಶ_Auto_ಬೀಗ::~UMat_ದತ್ತಾಂಶ_Auto_ಬೀಗ()
{
    get_UMat_ದತ್ತಾಂಶ_Auto_Locker().release(u1, u2);
}

#ಇಲ್ಲದಿದ್ದರೆ

ಶೂನ್ಯ UMat_ದತ್ತಾಂಶ::lock()
{
    // nothing in OPENCV_DISABLE_THREAD_SUPPORT mode
}

ಶೂನ್ಯ UMat_ದತ್ತಾಂಶ::unlock()
{
    // nothing in OPENCV_DISABLE_THREAD_SUPPORT mode
}

UMat_ದತ್ತಾಂಶ_Auto_ಬೀಗ::UMat_ದತ್ತಾಂಶ_Auto_ಬೀಗ(UMat_ದತ್ತಾಂಶ* u) : u1(u), u2(ಖಾಲಿ)
{
    // nothing in OPENCV_DISABLE_THREAD_SUPPORT mode
}
UMat_ದತ್ತಾಂಶ_Auto_ಬೀಗ::UMat_ದತ್ತಾಂಶ_Auto_ಬೀಗ(UMat_ದತ್ತಾಂಶ* u1_, UMat_ದತ್ತಾಂಶ* u2_) : u1(u1_), u2(u2_)
{
    // nothing in OPENCV_DISABLE_THREAD_SUPPORT mode
}
UMat_ದತ್ತಾಂಶ_Auto_ಬೀಗ::~UMat_ದತ್ತಾಂಶ_Auto_ಬೀಗ()
{
    // nothing in OPENCV_DISABLE_THREAD_SUPPORT mode
}

#ಆದರೆ_ಅಂತ್ಯ  // OPENCV_DISABLE_THREAD_SUPPORT

//////////////////////////////// UMat ////////////////////////////////

UMat::UMat(UMatUsageFlags _usageFlags) CV_NOEXCEPT
: flags(MAGIC_VAL), dims(0), ಅಡ್ಡಸಾಲುಗಳು(0), ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{}

UMat::UMat(ಪೂರ್ಣಾಂಕ ಅಡ್ಡಸಾಲುಗಳು_3, ಪೂರ್ಣಾಂಕ ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_3, ಪೂರ್ಣಾಂಕ _type, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), ಅಡ್ಡಸಾಲುಗಳು(0), ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(ಅಡ್ಡಸಾಲುಗಳು_3, ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_3, _type);
}

UMat::UMat(ಪೂರ್ಣಾಂಕ ಅಡ್ಡಸಾಲುಗಳು_3, ಪೂರ್ಣಾಂಕ ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_3, ಪೂರ್ಣಾಂಕ _type, ಸ್ಥಿರಾಂಕ Scalar& _s, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), ಅಡ್ಡಸಾಲುಗಳು(0), ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(ಅಡ್ಡಸಾಲುಗಳು_3, ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_3, _type);
    *ಇದು = _s;
}

UMat::UMat(ಗಾತ್ರ_2 _sz, ಪೂರ್ಣಾಂಕ _type, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), ಅಡ್ಡಸಾಲುಗಳು(0), ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create( _sz.ಎತ್ತರ, _sz.ಅಗಲ, _type );
}

UMat::UMat(ಗಾತ್ರ_2 _sz, ಪೂರ್ಣಾಂಕ _type, ಸ್ಥಿರಾಂಕ Scalar& _s, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), ಅಡ್ಡಸಾಲುಗಳು(0), ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_sz.ಎತ್ತರ, _sz.ಅಗಲ, _type);
    *ಇದು = _s;
}

UMat::UMat(ಪೂರ್ಣಾಂಕ _dims, ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* _sz, ಪೂರ್ಣಾಂಕ _type, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), ಅಡ್ಡಸಾಲುಗಳು(0), ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_dims, _sz, _type);
}

UMat::UMat(ಪೂರ್ಣಾಂಕ _dims, ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* _sz, ಪೂರ್ಣಾಂಕ _type, ಸ್ಥಿರಾಂಕ Scalar& _s, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), ಅಡ್ಡಸಾಲುಗಳು(0), ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_dims, _sz, _type);
    *ಇದು = _s;
}

UMat::UMat(ಸ್ಥಿರಾಂಕ MatShape& _shape, ಪೂರ್ಣಾಂಕ _type, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), ಅಡ್ಡಸಾಲುಗಳು(0), ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_shape.dims, _shape.p, _type);
}

UMat::UMat(ಸ್ಥಿರಾಂಕ MatShape& _shape, ಪೂರ್ಣಾಂಕ _type, ಸ್ಥಿರಾಂಕ Scalar& _s, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), ಅಡ್ಡಸಾಲುಗಳು(0), ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_shape.dims, _shape.p, _type);
    *ಇದು = _s;
}

UMat::UMat(ಸ್ಥಿರಾಂಕ UMat& m)
: flags(m.flags), dims(m.dims), ಅಡ್ಡಸಾಲುಗಳು(m.ಅಡ್ಡಸಾಲುಗಳು), ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ(m.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ), allocator(m.allocator),
  usageFlags(m.usageFlags), u(m.u), offset(m.offset), size(m.size), ಹೆಜ್ಜೆ(m.ಹೆಜ್ಜೆ)
{
    addref();
}

UMat& UMat::ಕ್ರಿಯಾಕಾರಕ=(ಸ್ಥಿರಾಂಕ UMat& m)
{
    ಆದರೆ( ಇದು != &m )
    {
        const_cast<UMat&>(m).addref();
        release();
        flags = m.flags;
        dims = m.dims;
        ಅಡ್ಡಸಾಲುಗಳು = m.ಅಡ್ಡಸಾಲುಗಳು;
        ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = m.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ;
        size = m.size;
        ಹೆಜ್ಜೆ = m.ಹೆಜ್ಜೆ;
        allocator = m.allocator;
        usageFlags = m.usageFlags;
        u = m.u;
        offset = m.offset;
    }
    ಹಿಂತಿರುಗಿ *ಇದು;
}

UMat UMat::clone() ಸ್ಥಿರಾಂಕ
{
    UMat m;
    copy_ಗಮ್ಯ(m);
    ಹಿಂತಿರುಗಿ m;
}

ಶೂನ್ಯ UMat::ನಿಯೋಜಿಸು_ಗಮ್ಯ(UMat& m, ಪೂರ್ಣಾಂಕ _type) ಸ್ಥಿರಾಂಕ
{
    ಆದರೆ( _type < 0 )
        m = *ಇದು;
    ಇಲ್ಲದಿದ್ದರೆ
        convert_ಗಮ್ಯ(m, _type);
}

ಶೂನ್ಯ UMat::create(ಪೂರ್ಣಾಂಕ ಅಡ್ಡಸಾಲುಗಳು_3, ಪೂರ್ಣಾಂಕ ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_3, ಪೂರ್ಣಾಂಕ _type, UMatUsageFlags _usageFlags)
{
    ಪೂರ್ಣಾಂಕ sz[] = {ಅಡ್ಡಸಾಲುಗಳು_3, ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_3};
    create(2, sz, _type, _usageFlags);
}

ಶೂನ್ಯ UMat::create(ಗಾತ್ರ_2 _sz, ಪೂರ್ಣಾಂಕ _type, UMatUsageFlags _usageFlags)
{
    create(_sz.ಎತ್ತರ, _sz.ಅಗಲ, _type, _usageFlags);
}

ಶೂನ್ಯ UMat::create_Same_ಗಾತ್ರ(ಒಳಹರಿವು_Array arr, ಪೂರ್ಣಾಂಕ _type, UMatUsageFlags _usageFlags)
{
    create(arr.shape(), _type, _usageFlags);
}

ಶೂನ್ಯ UMat::addref()
{
    ಆದರೆ( u )
        CV_XADD(&(u->urefcount), 1);
}

ಶೂನ್ಯ UMat::release()
{
    ಆದರೆ( u && CV_XADD(&(u->urefcount), -1) == 1 )
        deallocate();
    u = 0;
    size.clear();
    dims = ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = ಅಡ್ಡಸಾಲುಗಳು = 0;
}

ತಾರ್ಕಿಕ UMat::empty() ಸ್ಥಿರಾಂಕ
{
    ಹಿಂತಿರುಗಿ u == 0 || ಒಟ್ಟು() == 0;
}

size_t UMat::ಒಟ್ಟು() ಸ್ಥಿರಾಂಕ
{
    ಆದರೆ( dims <= 2 )
        ಹಿಂತಿರುಗಿ (size_t)ಅಡ್ಡಸಾಲುಗಳು * ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ;
    size_t p = 1;
    ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = 0; i < dims; i++ )
        p *= size[i];
    ಹಿಂತಿರುಗಿ p;
}

MatShape UMat::shape() ಸ್ಥಿರಾಂಕ
{
    ಹಿಂತಿರುಗಿ size;
}

UMat::UMat(UMat&& m)
: flags(m.flags), dims(m.dims), ಅಡ್ಡಸಾಲುಗಳು(m.ಅಡ್ಡಸಾಲುಗಳು), ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ(m.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ), allocator(m.allocator),
  usageFlags(m.usageFlags), u(m.u), offset(m.offset)
{
    size = m.size;
    ಹೆಜ್ಜೆ = m.ಹೆಜ್ಜೆ;
    m.flags = MAGIC_VAL;
    m.usageFlags = USAGE_DEFAULT;
    m.dims = m.ಅಡ್ಡಸಾಲುಗಳು = m.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = 0;
    m.size.clear();
    m.allocator = ಖಾಲಿ;
    m.u = ಖಾಲಿ;
    m.offset = 0;
}

UMat& UMat::ಕ್ರಿಯಾಕಾರಕ=(UMat&& m)
{
    ಆದರೆ (ಇದು == &m)
      ಹಿಂತಿರುಗಿ *ಇದು;
    release();
    flags = m.flags; dims = m.dims; ಅಡ್ಡಸಾಲುಗಳು = m.ಅಡ್ಡಸಾಲುಗಳು; ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = m.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ;
    allocator = m.allocator; usageFlags = m.usageFlags;
    u = m.u;
    offset = m.offset;
    size = m.size;
    ಹೆಜ್ಜೆ = m.ಹೆಜ್ಜೆ;
    m.flags = MAGIC_VAL;
    m.usageFlags = USAGE_DEFAULT;
    m.dims = m.ಅಡ್ಡಸಾಲುಗಳು = m.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = 0;
    m.allocator = ಖಾಲಿ;
    m.u = ಖಾಲಿ;
    m.offset = 0;
    ಹಿಂತಿರುಗಿ *ಇದು;
}


MatAllocator* UMat::getStdAllocator()
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_OPENCL
    ಆದರೆ (ocl::useOpenCL())
        ಹಿಂತಿರುಗಿ ocl::getOpenCLAllocator();
#ಆದರೆ_ಅಂತ್ಯ
    ಹಿಂತಿರುಗಿ Mat::getDefaultAllocator();
}

ಶೂನ್ಯ swap( UMat& a, UMat& b )
{
    std::swap(a.flags, b.flags);
    std::swap(a.dims, b.dims);
    std::swap(a.ಅಡ್ಡಸಾಲುಗಳು, b.ಅಡ್ಡಸಾಲುಗಳು);
    std::swap(a.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, b.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ);
    std::swap(a.allocator, b.allocator);
    std::swap(a.u, b.u);
    std::swap(a.offset, b.offset);

    std::swap(a.size, b.size);
    std::swap(a.ಹೆಜ್ಜೆ, b.ಹೆಜ್ಜೆ);
}


ಶೂನ್ಯ ಗಣ_ಗಾತ್ರ( UMat& m, ಪೂರ್ಣಾಂಕ _dims, ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* _sz,
              ಸ್ಥಿರಾಂಕ size_t* ಹೆಜ್ಜೆಗಳು_2, ತಾರ್ಕಿಕ auto_ಹೆಜ್ಜೆಗಳು )
{
    CV_Assert( 0 <= _dims && _dims <= CV_MAX_DIM && _dims <= MatShape::MAX_DIMS);

    m.dims = _dims;
    m.size = MatShape(_dims, _sz);
    m.ಹೆಜ್ಜೆ[std::max(_dims-1, 0)] = CV_ELEM_ಗಾತ್ರ(m.flags);
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = _dims-2; i >= 0; i--) {
        size_t autostep = m.size[i+1]*m.ಹೆಜ್ಜೆ[i+1];
        ಆದರೆ (ಹೆಜ್ಜೆಗಳು_2) {
            m.ಹೆಜ್ಜೆ[i] = ಹೆಜ್ಜೆಗಳು_2[i];
            //CV_Assert(m.step[i] >= autostep);
        } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (auto_ಹೆಜ್ಜೆಗಳು) {
            m.ಹೆಜ್ಜೆ[i] = autostep;
        } ಇಲ್ಲದಿದ್ದರೆ {
            m.ಹೆಜ್ಜೆ[i] = 0;
        }
    }

    ಆದರೆ( _dims <= 2 )
    {
        m.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = _dims == 0 ? 1 : _sz ? _sz[_dims > 1] : 0;
        m.ಅಡ್ಡಸಾಲುಗಳು = _dims < 2 ? 1 : _sz ? _sz[0] : 0;
    } ಇಲ್ಲದಿದ್ದರೆ {
        m.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = m.ಅಡ್ಡಸಾಲುಗಳು = -1;
    }
}


ಶೂನ್ಯ UMat::updateContinuityFlag()
{
    flags = cv::updateContinuityFlag(flags, dims, size.p, ಹೆಜ್ಜೆ.p);
}


ಶೂನ್ಯ finalizeHdr(UMat& m)
{
    m.updateContinuityFlag();
    ಪೂರ್ಣಾಂಕ d = m.dims;
    ಆದರೆ( d > 2 )
        m.ಅಡ್ಡಸಾಲುಗಳು = m.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = -1;
}


UMat Mat::getUMat(AccessFlag accessFlags, UMatUsageFlags usageFlags) ಸ್ಥಿರಾಂಕ
{
    UMat hdr;
    ಆದರೆ(!ದತ್ತಾಂಶ)
        ಹಿಂತಿರುಗಿ hdr;
    ಆದರೆ (ದತ್ತಾಂಶ != datastart)
    {
        ಗಾತ್ರ_2 ಸಂಪೂರ್ಣ_ಗಾತ್ರ;
        ಬಿಂದು ofs;
        locateROI(ಸಂಪೂರ್ಣ_ಗಾತ್ರ, ofs);
        ಗಾತ್ರ_2 sz(ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, ಅಡ್ಡಸಾಲುಗಳು);
        ಆದರೆ (ofs.x != 0 || ofs.y != 0)
        {
            Mat src = *ಇದು;
            ಪೂರ್ಣಾಂಕ dtop = ofs.y;
            ಪೂರ್ಣಾಂಕ dbottom = ಸಂಪೂರ್ಣ_ಗಾತ್ರ.ಎತ್ತರ - src.ಅಡ್ಡಸಾಲುಗಳು - ofs.y;
            ಪೂರ್ಣಾಂಕ dleft = ofs.x;
            ಪೂರ್ಣಾಂಕ dright = ಸಂಪೂರ್ಣ_ಗಾತ್ರ.ಅಗಲ - src.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ - ofs.x;
            src.adjustROI(dtop, dbottom, dleft, dright);
            ಹಿಂತಿರುಗಿ src.getUMat(accessFlags, usageFlags)(cv::Rect(ofs.x, ofs.y, sz.ಅಗಲ, sz.ಎತ್ತರ));
        }
    }
    CV_Assert(ದತ್ತಾಂಶ == datastart);

    accessFlags |= ACCESS_RW;
    UMat_ದತ್ತಾಂಶ* new_u = ಖಾಲಿ;
    Mat_ಹೆಜ್ಜೆ new_ಹೆಜ್ಜೆ = ಹೆಜ್ಜೆ;
    {
        MatAllocator *a = allocator, *a0 = getDefaultAllocator();
        ಆದರೆ(!a)
            a = a0;
        new_u = a->allocate(dims, size.p, type(), ದತ್ತಾಂಶ, new_ಹೆಜ್ಜೆ.p, accessFlags, usageFlags);
        new_u->original_UMat_ದತ್ತಾಂಶ = u;
    }
    ತಾರ್ಕಿಕ allocated = ಸುಳ್ಳು;
    ಪ್ರಯತ್ನಿಸು
    {
        allocated = UMat::getStdAllocator()->allocate(new_u, accessFlags, usageFlags);
    }
    ಹಿಡಿ (ಸ್ಥಿರಾಂಕ cv::Exception& e)
    {
        fprintf(stderr, "Exception: %s\n", e.what());
    }
    ಆದರೆ (!allocated)
    {
        allocated = getDefaultAllocator()->allocate(new_u, accessFlags, usageFlags);
        CV_Assert(allocated);
    }
    ಆದರೆ (u != ಖಾಲಿ)
    {
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_OPENCL
        ಆದರೆ (ocl::useOpenCL() && new_u->currAllocator == ocl::getOpenCLAllocator())
        {
            CV_Assert(new_u->ತಾತ್ಕಾಲಿಕ_UMat());
        }
#ಆದರೆ_ಅಂತ್ಯ
        CV_XADD(&(u->refcount), 1);
        CV_XADD(&(u->urefcount), 1);
    }
    ಪ್ರಯತ್ನಿಸು
    {
        hdr.flags = flags;
        hdr.usageFlags = usageFlags;
        ಗಣ_ಗಾತ್ರ(hdr, dims, size.p, new_ಹೆಜ್ಜೆ.p);
        finalizeHdr(hdr);
        hdr.u = new_u;
        hdr.offset = 0; //data - datastart;
        hdr.addref();
        ಹಿಂತಿರುಗಿ hdr;
    }
    ಹಿಡಿ(...)
    {
        ಆದರೆ (u != ಖಾಲಿ)
        {
            CV_XADD(&(u->refcount), -1);
            CV_XADD(&(u->urefcount), -1);
        }
        new_u->currAllocator->deallocate(new_u);
        ಎಸೆ;
    }

}

ಶೂನ್ಯ UMat::create(ಪೂರ್ಣಾಂಕ d0, ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* _sizes, ಪೂರ್ಣಾಂಕ _type, UMatUsageFlags _usageFlags)
{
    ಪೂರ್ಣಾಂಕ sz1 = 1, d = d0;
    ಪೂರ್ಣಾಂಕ i;
    ಆದರೆ (d == 0) {
        d = 1;
        _sizes = (ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ*)&sz1;
    }
    CV_Assert(0 <= d && d <= CV_MAX_DIM && _sizes);
    _type = CV_MAT_TYPE(_type);

    // if param value is USAGE_DEFAULT by implicit default param value -or- explicit value
    // ...then don't change the existing usageFlags
    // it is not possible to change usage from non-default to USAGE_DEFAULT through create()
    // ...instead must construct UMat()
    ಆದರೆ (_usageFlags == cv::USAGE_DEFAULT)
    {
        _usageFlags = usageFlags;
    }

    ಆದರೆ( u && d == dims && _type == type() && _usageFlags == usageFlags )
    {
        ಗಾಗಿ( i = 0; i < d; i++ )
            ಆದರೆ( size[i] != _sizes[i] )
                ನಿಲ್ಲಿಸು;
        ಆದರೆ( i == d )
            ಹಿಂತಿರುಗಿ;
    }

    ಪೂರ್ಣಾಂಕ _sizes_backup[CV_MAX_DIM]; // #5991
    ಆದರೆ (_sizes == (ಇದು->size.p))
    {
        ಗಾಗಿ(i = 0; i < d; i++ )
            _sizes_backup[i] = _sizes[i];
        _sizes = _sizes_backup;
    }

    release();
    usageFlags = _usageFlags;
    ಆದರೆ( d == 0 )
        ಹಿಂತಿರುಗಿ;
    flags = (_type & CV_MAT_TYPE_ಮುಖವಾಡ) | MAGIC_VAL;
    ಗಣ_ಗಾತ್ರ(*ಇದು, d, _sizes, 0, ನಿಜ);
    offset = 0;

    ಆದರೆ( ಒಟ್ಟು() > 0 )
    {
        MatAllocator *a = allocator, *a0 = getStdAllocator();
        ಆದರೆ (!a)
        {
            a = a0;
            a0 = Mat::getDefaultAllocator();
        }
        ಪ್ರಯತ್ನಿಸು
        {
            u = a->allocate(dims, size.p, _type, 0, ಹೆಜ್ಜೆ.p, ACCESS_RW /* ignored */, usageFlags);
            CV_Assert(u != 0);
        }
        ಹಿಡಿ(...)
        {
            ಆದರೆ(a != a0)
                u = a0->allocate(dims, size.p, _type, 0, ಹೆಜ್ಜೆ.p, ACCESS_RW /* ignored */, usageFlags);
            CV_Assert(u != 0);
        }
        CV_Assert( ಹೆಜ್ಜೆ[dims-1] == (size_t)CV_ELEM_ಗಾತ್ರ(flags) );
    }

    finalizeHdr(*ಇದು);
    addref();
    size.dims = dims = d0;
}

ಶೂನ್ಯ UMat::create(ಸ್ಥಿರಾಂಕ std::vector<ಪೂರ್ಣಾಂಕ>& _sizes, ಪೂರ್ಣಾಂಕ _type, UMatUsageFlags _usageFlags)
{
    create((ಪೂರ್ಣಾಂಕ)_sizes.size(), _sizes.ದತ್ತಾಂಶ(), _type, _usageFlags);
}

ಶೂನ್ಯ UMat::create(ಸ್ಥಿರಾಂಕ MatShape& _shape, ಪೂರ್ಣಾಂಕ _type, UMatUsageFlags _usageFlags)
{
    ಆದರೆ (_shape.dims < 0) {
        release();
    } ಇಲ್ಲದಿದ್ದರೆ {
        create(_shape.dims, _shape.p, _type, _usageFlags);
        size.layout = _shape.layout;
        size.C = _shape.C;
    }
}

ಶೂನ್ಯ UMat::fit(ಪೂರ್ಣಾಂಕ _dims, ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* _sizes, ಪೂರ್ಣಾಂಕ _type, UMatUsageFlags _usageFlags)
{
    ಆದರೆ (_usageFlags == cv::USAGE_DEFAULT)
        _usageFlags = usageFlags;
    size_t old_ಒಟ್ಟು_ಬೈಟುಗಳು = u ? u->size : 0;
    size_t esz = CV_ELEM_ಗಾತ್ರ(_type), new_ಒಟ್ಟು = _dims >= 0;
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < _dims; i++)
        new_ಒಟ್ಟು *= _sizes[i];
    size_t new_ಒಟ್ಟು_ಬೈಟುಗಳು = new_ಒಟ್ಟು*esz;
    ಆದರೆ (new_ಒಟ್ಟು_ಬೈಟುಗಳು > 0 &&
        (!isContinuous() ||
         new_ಒಟ್ಟು_ಬೈಟುಗಳು > old_ಒಟ್ಟು_ಬೈಟುಗಳು ||
         offset != 0 ||
         _usageFlags != usageFlags)) {
        create(_dims, _sizes, _type, _usageFlags);
    } ಇಲ್ಲದಿದ್ದರೆ {
        flags = (flags & ~Mat::TYPE_ಮುಖವಾಡ) | CV_MAT_TYPE(_type);
        ಪೂರ್ಣಾಂಕ dummy_ಗಾತ್ರ = 0;
        ಗಣ_ಗಾತ್ರ(*ಇದು, (_dims >= 0 ? _dims : 1), (_dims >= 0 ? _sizes : &dummy_ಗಾತ್ರ), ಶೂನ್ಯಸೂಚಕ, ನಿಜ);
        finalizeHdr(*ಇದು);
    }
}

ಶೂನ್ಯ UMat::fit(ಸ್ಥಿರಾಂಕ std::vector<ಪೂರ್ಣಾಂಕ>& _shape, ಪೂರ್ಣಾಂಕ _type, UMatUsageFlags _usageFlags)
{
    fit((ಪೂರ್ಣಾಂಕ)_shape.size(), _shape.ದತ್ತಾಂಶ(), _type, _usageFlags);
}

ಶೂನ್ಯ UMat::fit(ಸ್ಥಿರಾಂಕ MatShape& _shape, ಪೂರ್ಣಾಂಕ _type, UMatUsageFlags _usageFlags)
{
    fit(_shape.dims, _shape.p, _type, _usageFlags);
    size.layout = _shape.layout;
    size.C = _shape.C;
}

ಶೂನ್ಯ UMat::fit(ಪೂರ್ಣಾಂಕ ಅಡ್ಡಸಾಲುಗಳು_3, ಪೂರ್ಣಾಂಕ ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_3, ಪೂರ್ಣಾಂಕ _type, UMatUsageFlags _usageFlags)
{
    _type &= TYPE_ಮುಖವಾಡ;
    ಪೂರ್ಣಾಂಕ sz[] = {ಅಡ್ಡಸಾಲುಗಳು_3, ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_3};
    fit(2, sz, _type, _usageFlags);
}

ಶೂನ್ಯ UMat::fit(ಗಾತ್ರ_2 _sz, ಪೂರ್ಣಾಂಕ _type, UMatUsageFlags _usageFlags)
{
    fit(_sz.ಎತ್ತರ, _sz.ಅಗಲ, _type, _usageFlags);
}

ಶೂನ್ಯ UMat::fit_Same_ಗಾತ್ರ(ಒಳಹರಿವು_Array m, ಪೂರ್ಣಾಂಕ _type, UMatUsageFlags _usageFlags)
{
    fit(m.shape(), _type, _usageFlags);
}

ಶೂನ್ಯ UMat::copy_ಗಾತ್ರ(ಸ್ಥಿರಾಂಕ UMat& m)
{
    dims = m.dims;
    ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = m.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ;
    ಅಡ್ಡಸಾಲುಗಳು = m.ಅಡ್ಡಸಾಲುಗಳು;
    size = m.size;
    ಹೆಜ್ಜೆ = m.ಹೆಜ್ಜೆ;
}

UMat::~UMat()
{
    release();
}

ಶೂನ್ಯ UMat::deallocate()
{
    UMat_ದತ್ತಾಂಶ* u_ = u;
    u = ಖಾಲಿ;
    u_->currAllocator->deallocate(u_);
}


UMat::UMat(ಸ್ಥಿರಾಂಕ UMat& m, ಸ್ಥಿರಾಂಕ Range& ಅಡ್ಡಸಾಲು_Range_3, ಸ್ಥಿರಾಂಕ Range& ಲಂಬಸಾಲು_Range_3)
    : flags(MAGIC_VAL), dims(0), ಅಡ್ಡಸಾಲುಗಳು(0), ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ(0), allocator(0), usageFlags(USAGE_DEFAULT), u(0), offset(0)
{
    CV_Assert( m.dims >= 2 && m.dims <= MatShape::MAX_DIMS );
    ಆದರೆ( m.dims > 2 )
    {
        Range rs[MatShape::MAX_DIMS];
        rs[0] = ಅಡ್ಡಸಾಲು_Range_3;
        rs[1] = ಲಂಬಸಾಲು_Range_3;
        ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = 2; i < m.dims; i++ )
            rs[i] = Range::all();
        *ಇದು = m(rs);
        ಹಿಂತಿರುಗಿ;
    }

    *ಇದು = m;
    ಆದರೆ( ಅಡ್ಡಸಾಲು_Range_3 != Range::all() && ಅಡ್ಡಸಾಲು_Range_3 != Range(0,ಅಡ್ಡಸಾಲುಗಳು) )
    {
        CV_Assert( 0 <= ಅಡ್ಡಸಾಲು_Range_3.ಆರಂಭ &&
                  ಅಡ್ಡಸಾಲು_Range_3.ಆರಂಭ <= ಅಡ್ಡಸಾಲು_Range_3.end &&
                  ಅಡ್ಡಸಾಲು_Range_3.end <= m.ಅಡ್ಡಸಾಲುಗಳು );
        size[0] = ಅಡ್ಡಸಾಲುಗಳು = ಅಡ್ಡಸಾಲು_Range_3.size();
        offset += ಹೆಜ್ಜೆ*ಅಡ್ಡಸಾಲು_Range_3.ಆರಂಭ;
        flags |= SUBMATRIX_FLAG;
    }

    ಆದರೆ( ಲಂಬಸಾಲು_Range_3 != Range::all() && ಲಂಬಸಾಲು_Range_3 != Range(0,ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ) )
    {
        CV_Assert( 0 <= ಲಂಬಸಾಲು_Range_3.ಆರಂಭ &&
                  ಲಂಬಸಾಲು_Range_3.ಆರಂಭ <= ಲಂಬಸಾಲು_Range_3.end &&
                  ಲಂಬಸಾಲು_Range_3.end <= m.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ );
        size[1] = ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = ಲಂಬಸಾಲು_Range_3.size();
        offset += ಲಂಬಸಾಲು_Range_3.ಆರಂಭ*elem_ಗಾತ್ರ();
        flags |= SUBMATRIX_FLAG;
    }

    updateContinuityFlag();

    ಆದರೆ( ಅಡ್ಡಸಾಲುಗಳು <= 0 || ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ <= 0 )
    {
        release();
        ಅಡ್ಡಸಾಲುಗಳು = ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = 0;
    }
}


UMat::UMat(ಸ್ಥಿರಾಂಕ UMat& m, ಸ್ಥಿರಾಂಕ Rect& roi)
    : flags(m.flags), dims(2), ಅಡ್ಡಸಾಲುಗಳು(roi.ಎತ್ತರ), ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ(roi.ಅಗಲ),
    allocator(m.allocator), usageFlags(m.usageFlags), u(m.u),
    offset(m.offset + roi.y*m.ಹೆಜ್ಜೆ[0]), size(2)
{
    CV_Assert( m.dims <= 2 );

    size_t esz = CV_ELEM_ಗಾತ್ರ(flags);
    offset += roi.x*esz;
    CV_Assert( 0 <= roi.x && 0 <= roi.ಅಗಲ && roi.x + roi.ಅಗಲ <= m.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ &&
              0 <= roi.y && 0 <= roi.ಎತ್ತರ && roi.y + roi.ಎತ್ತರ <= m.ಅಡ್ಡಸಾಲುಗಳು );
    ಆದರೆ( roi.ಅಗಲ < m.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ || roi.ಎತ್ತರ < m.ಅಡ್ಡಸಾಲುಗಳು )
        flags |= SUBMATRIX_FLAG;

    size[0] = ಅಡ್ಡಸಾಲುಗಳು;
    size[1] = ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ;
    ಹೆಜ್ಜೆ[0] = m.dims == 2 ? m.ಹೆಜ್ಜೆ[0] : ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ*esz;
    ಹೆಜ್ಜೆ[1] = esz;

    updateContinuityFlag();

    addref();
    ಆದರೆ( ಅಡ್ಡಸಾಲುಗಳು <= 0 || ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ <= 0 )
    {
        ಅಡ್ಡಸಾಲುಗಳು = ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = 0;
        release();
    }
}


UMat::UMat(ಸ್ಥಿರಾಂಕ UMat& m, ಸ್ಥಿರಾಂಕ Range* ranges)
    : flags(MAGIC_VAL), dims(0), ಅಡ್ಡಸಾಲುಗಳು(0), ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ(0), allocator(0), usageFlags(USAGE_DEFAULT), u(0), offset(0)
{
    ಪೂರ್ಣಾಂಕ i, d = m.dims;

    CV_Assert(ranges);
    ಗಾಗಿ( i = 0; i < d; i++ )
    {
        Range r = ranges[i];
        CV_Assert( r == Range::all() || (0 <= r.ಆರಂಭ && r.ಆರಂಭ < r.end && r.end <= m.size[i]) );
    }
    *ಇದು = m;
    ಗಾಗಿ( i = 0; i < d; i++ )
    {
        Range r = ranges[i];
        ಆದರೆ( r != Range::all() && r != Range(0, size.p[i]))
        {
            size.p[i] = r.end - r.ಆರಂಭ;
            offset += r.ಆರಂಭ*ಹೆಜ್ಜೆ.p[i];
            flags |= SUBMATRIX_FLAG;
        }
    }

    ಆದರೆ (d <= 2) {
        ಅಡ್ಡಸಾಲುಗಳು = d == 2 ? size[0] : 1;
        ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = d <= 0 ? (d >= 0) : size[d > 1];
    }
    updateContinuityFlag();
}

UMat::UMat(ಸ್ಥಿರಾಂಕ UMat& m, ಸ್ಥಿರಾಂಕ std::vector<Range>& ranges)
    : flags(MAGIC_VAL), dims(0), ಅಡ್ಡಸಾಲುಗಳು(0), ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ(0), allocator(0), usageFlags(USAGE_DEFAULT), u(0), offset(0)
{
    ಪೂರ್ಣಾಂಕ i, d = m.dims;

    CV_Assert((ಪೂರ್ಣಾಂಕ)ranges.size() == d);
    ಗಾಗಿ (i = 0; i < d; i++)
    {
        Range r = ranges[i];
        CV_Assert(r == Range::all() || (0 <= r.ಆರಂಭ && r.ಆರಂಭ < r.end && r.end <= m.size[i]));
    }
    *ಇದು = m;
    ಗಾಗಿ (i = 0; i < d; i++)
    {
        Range r = ranges[i];
        ಆದರೆ (r != Range::all() && r != Range(0, size.p[i]))
        {
            size.p[i] = r.end - r.ಆರಂಭ;
            offset += r.ಆರಂಭ*ಹೆಜ್ಜೆ.p[i];
            flags |= SUBMATRIX_FLAG;
        }
    }

    ಆದರೆ (d <= 2) {
        ಅಡ್ಡಸಾಲುಗಳು = d == 2 ? size[0] : 1;
        ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = d <= 0 ? (d >= 0) : size[d > 1];
    }
    updateContinuityFlag();
}

UMat UMat::diag(ಪೂರ್ಣಾಂಕ d) ಸ್ಥಿರಾಂಕ
{
    CV_Assert( dims <= 2 );
    UMat m = *ಇದು;
    size_t esz = elem_ಗಾತ್ರ();
    ಪೂರ್ಣಾಂಕ len;

    ಆದರೆ( d >= 0 )
    {
        len = std::min(ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ - d, ಅಡ್ಡಸಾಲುಗಳು);
        m.offset += esz*d;
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        len = std::min(ಅಡ್ಡಸಾಲುಗಳು + d, ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ);
        m.offset -= ಹೆಜ್ಜೆ[0]*d;
    }
    CV_DbgAssert( len > 0 );

    m.size[0] = m.ಅಡ್ಡಸಾಲುಗಳು = len;
    m.size[1] = m.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = 1;
    m.ಹೆಜ್ಜೆ[0] += (len > 1 ? esz : 0);

    m.updateContinuityFlag();

    ಆದರೆ( size() != ಗಾತ್ರ_2(1,1) )
        m.flags |= SUBMATRIX_FLAG;

    ಹಿಂತಿರುಗಿ m;
}

ಶೂನ್ಯ UMat::locateROI( ಗಾತ್ರ_2& ಸಂಪೂರ್ಣ_ಗಾತ್ರ, ಬಿಂದು& ofs ) ಸ್ಥಿರಾಂಕ
{
    CV_Assert( dims <= 2 && ಹೆಜ್ಜೆ[0] > 0 );
    size_t esz = elem_ಗಾತ್ರ(), minstep;
    ptrdiff_t delta1 = (ptrdiff_t)offset, delta2 = (ptrdiff_t)u->size;

    ಆದರೆ( delta1 == 0 )
        ofs.x = ofs.y = 0;
    ಇಲ್ಲದಿದ್ದರೆ
    {
        ofs.y = (ಪೂರ್ಣಾಂಕ)(delta1/ಹೆಜ್ಜೆ[0]);
        ofs.x = (ಪೂರ್ಣಾಂಕ)((delta1 - ಹೆಜ್ಜೆ[0]*ofs.y)/esz);
        CV_DbgAssert( offset == (size_t)(ofs.y*ಹೆಜ್ಜೆ[0] + ofs.x*esz) );
    }
    minstep = (ofs.x + ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ)*esz;
    ಸಂಪೂರ್ಣ_ಗಾತ್ರ.ಎತ್ತರ = (ಪೂರ್ಣಾಂಕ)((delta2 - minstep)/ಹೆಜ್ಜೆ[0] + 1);
    ಸಂಪೂರ್ಣ_ಗಾತ್ರ.ಎತ್ತರ = std::max(ಸಂಪೂರ್ಣ_ಗಾತ್ರ.ಎತ್ತರ, ofs.y + ಅಡ್ಡಸಾಲುಗಳು);
    ಸಂಪೂರ್ಣ_ಗಾತ್ರ.ಅಗಲ = (ಪೂರ್ಣಾಂಕ)((delta2 - ಹೆಜ್ಜೆ*(ಸಂಪೂರ್ಣ_ಗಾತ್ರ.ಎತ್ತರ-1))/esz);
    ಸಂಪೂರ್ಣ_ಗಾತ್ರ.ಅಗಲ = std::max(ಸಂಪೂರ್ಣ_ಗಾತ್ರ.ಅಗಲ, ofs.x + ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ);
}


UMat& UMat::adjustROI( ಪೂರ್ಣಾಂಕ dtop, ಪೂರ್ಣಾಂಕ dbottom, ಪೂರ್ಣಾಂಕ dleft, ಪೂರ್ಣಾಂಕ dright )
{
    CV_Assert( dims == 2 && ಹೆಜ್ಜೆ[0] > 0 );
    ಗಾತ್ರ_2 ಸಂಪೂರ್ಣ_ಗಾತ್ರ; ಬಿಂದು ofs;
    size_t esz = elem_ಗಾತ್ರ();
    locateROI( ಸಂಪೂರ್ಣ_ಗಾತ್ರ, ofs );
    ಪೂರ್ಣಾಂಕ row1 = std::min(std::max(ofs.y - dtop, 0), ಸಂಪೂರ್ಣ_ಗಾತ್ರ.ಎತ್ತರ), row2 = std::max(0, std::min(ofs.y + ಅಡ್ಡಸಾಲುಗಳು + dbottom, ಸಂಪೂರ್ಣ_ಗಾತ್ರ.ಎತ್ತರ));
    ಪೂರ್ಣಾಂಕ col1 = std::min(std::max(ofs.x - dleft, 0), ಸಂಪೂರ್ಣ_ಗಾತ್ರ.ಅಗಲ), col2 = std::max(0, std::min(ofs.x + ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ + dright, ಸಂಪೂರ್ಣ_ಗಾತ್ರ.ಅಗಲ));
    ಆದರೆ(row1 > row2)
        std::swap(row1, row2);
    ಆದರೆ(col1 > col2)
        std::swap(col1, col2);

    offset += (row1 - ofs.y)*ಹೆಜ್ಜೆ + (col1 - ofs.x)*esz;
    ಅಡ್ಡಸಾಲುಗಳು = row2 - row1; ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = col2 - col1;
    size.p[0] = ಅಡ್ಡಸಾಲುಗಳು; size.p[1] = ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ;
    updateContinuityFlag();
    ಹಿಂತಿರುಗಿ *ಇದು;
}


UMat UMat::reshape(ಪೂರ್ಣಾಂಕ new_cn, ಪೂರ್ಣಾಂಕ new_ಅಡ್ಡಸಾಲುಗಳು) ಸ್ಥಿರಾಂಕ
{
    ಪೂರ್ಣಾಂಕ cn = channels();
    UMat hdr = *ಇದು;

    ಆದರೆ( dims > 2 && new_ಅಡ್ಡಸಾಲುಗಳು == 0 && new_cn != 0 && size[dims-1]*cn % new_cn == 0 )
    {
        hdr.flags = (hdr.flags & ~CV_MAT_CN_ಮುಖವಾಡ) | ((new_cn-1) << CV_CN_ಸರಿಸು);
        hdr.ಹೆಜ್ಜೆ[dims-1] = CV_ELEM_ಗಾತ್ರ(hdr.flags);
        hdr.size[dims-1] = hdr.size[dims-1]*cn / new_cn;
        ಹಿಂತಿರುಗಿ hdr;
    }

    CV_Assert( dims <= 2 );

    ಆದರೆ( new_cn == 0 )
        new_cn = cn;

    ಪೂರ್ಣಾಂಕ ಒಟ್ಟು_ಅಗಲ = ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ * cn;

    ಆದರೆ( (new_cn > ಒಟ್ಟು_ಅಗಲ || ಒಟ್ಟು_ಅಗಲ % new_cn != 0) && new_ಅಡ್ಡಸಾಲುಗಳು == 0 )
        new_ಅಡ್ಡಸಾಲುಗಳು = ಅಡ್ಡಸಾಲುಗಳು * ಒಟ್ಟು_ಅಗಲ / new_cn;

    ಆದರೆ( new_ಅಡ್ಡಸಾಲುಗಳು != 0 && new_ಅಡ್ಡಸಾಲುಗಳು != ಅಡ್ಡಸಾಲುಗಳು )
    {
        ಪೂರ್ಣಾಂಕ ಒಟ್ಟು_ಗಾತ್ರ_2 = ಒಟ್ಟು_ಅಗಲ * ಅಡ್ಡಸಾಲುಗಳು;
        ಆದರೆ( !isContinuous() )
            CV_Error( cv::Error::Bad_ಹೆಜ್ಜೆ,
            "The matrix is not continuous, thus its number of rows can not be changed" );

        ಒಟ್ಟು_ಅಗಲ = ಒಟ್ಟು_ಗಾತ್ರ_2 / new_ಅಡ್ಡಸಾಲುಗಳು;

        ಆದರೆ( ಒಟ್ಟು_ಅಗಲ * new_ಅಡ್ಡಸಾಲುಗಳು != ಒಟ್ಟು_ಗಾತ್ರ_2 )
            CV_Error( cv::Error::StsBadArg, "The total number of matrix elements "
                                    "is not divisible by the new number of rows" );

        hdr.size[0] = hdr.ಅಡ್ಡಸಾಲುಗಳು = new_ಅಡ್ಡಸಾಲುಗಳು;
        hdr.ಹೆಜ್ಜೆ[0] = ಒಟ್ಟು_ಅಗಲ * elemSize1();
    } ಇಲ್ಲದಿದ್ದರೆ {
        hdr.size[0] = hdr.ಅಡ್ಡಸಾಲುಗಳು = ಅಡ್ಡಸಾಲುಗಳು;
        ಆದರೆ (dims <= 1)
            hdr.ಹೆಜ್ಜೆ[0] = ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ * CV_ELEM_ಗಾತ್ರ(flags);
    }

    ಪೂರ್ಣಾಂಕ new_ಅಗಲ = ಒಟ್ಟು_ಅಗಲ / new_cn;

    ಆದರೆ( new_ಅಗಲ * new_cn != ಒಟ್ಟು_ಅಗಲ )
        CV_Error( cv::Error::Bad_ಅಂಕೆ_Channels,
        "The total width is not divisible by the new number of channels" );

    hdr.size.dims = hdr.dims = 2;
    hdr.size[1] = hdr.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = new_ಅಗಲ;
    hdr.flags = (hdr.flags & ~CV_MAT_CN_ಮುಖವಾಡ) | ((new_cn-1) << CV_CN_ಸರಿಸು);
    hdr.ಹೆಜ್ಜೆ[1] = CV_ELEM_ಗಾತ್ರ(hdr.flags);
    ಹಿಂತಿರುಗಿ hdr;
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_OPENCL
ನಾಮಾಕಾಶ {
ಸ್ಥಿರ ತಾರ್ಕಿಕ ocl_ಗಣ_Diag(ಸ್ಥಿರಾಂಕ UMat& d, UMat& m, ಪೂರ್ಣಾಂಕ len)
{
    ಪೂರ್ಣಾಂಕ cn = d.channels();
    ಪೂರ್ಣಾಂಕ ಆಳ = d.ಆಳ();

    ಆದರೆ (ಆಳ == CV_64F && !ocl::Device::getDefault().doubleFPConfig())
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

    String opts = format("-D SET_DIAG -D T1=%s -D cn=%d -D IS_ROW_VECTOR=%d",
                         ocl::memop_Type_ಗಮ್ಯ_Str(ಆಳ),
                         cn,
                         (d.ಅಡ್ಡಸಾಲುಗಳು == 1) ? 1 : 0);

    ocl::Kernel k("setDiag", ocl::core::copyset_oclsrc, opts);
    ಆದರೆ (k.empty())
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

    k.ವಾದಗಳು(ocl::KernelArg::WriteOnly(m),
           ocl::KernelArg::Read_Only_No_ಗಾತ್ರ(d),
           len);

    size_t globalsize[2] = { (size_t)len, (size_t)len };
    ಹಿಂತಿರುಗಿ k.ಸತತ(2, globalsize, ಖಾಲಿ, ಸುಳ್ಳು);
}
}
#ಆದರೆ_ಅಂತ್ಯ

UMat UMat::diag(ಸ್ಥಿರಾಂಕ UMat& d, UMatUsageFlags usageFlags)
{
    CV_INSTRUMENT_REGION();

    CV_Assert(d.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ == 1 || d.ಅಡ್ಡಸಾಲುಗಳು == 1);
    ಪೂರ್ಣಾಂಕ len = d.ಅಡ್ಡಸಾಲುಗಳು + d.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ - 1;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_OPENCL
    ಆದರೆ (ocl::useOpenCL())
    {
        UMat m(len, len, d.type(), usageFlags);
        ಆದರೆ (ocl_ಗಣ_Diag(d, m, len))
        {
            CV_IMPL_ಕೂಡಿಸು(CV_IMPL_OCL);
            ಹಿಂತಿರುಗಿ m;
        }
    }
#ಆದರೆ_ಅಂತ್ಯ

    UMat m(len, len, d.type(), Scalar(0), usageFlags);
    UMat md = m.diag();
    ಆದರೆ( d.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ == 1 )
        d.copy_ಗಮ್ಯ(md);
    ಇಲ್ಲದಿದ್ದರೆ
        transpose(d, md);
    ಹಿಂತಿರುಗಿ m;
}

ಪೂರ್ಣಾಂಕ UMat::check_ಸದಿಶ(ಪೂರ್ಣಾಂಕ _elemChannels, ಪೂರ್ಣಾಂಕ ಆಳ_2, ತಾರ್ಕಿಕ _requireContinuous) ಸ್ಥಿರಾಂಕ
{
    ಹಿಂತಿರುಗಿ (ಆಳ() == ಆಳ_2 || ಆಳ_2 <= 0) &&
        (isContinuous() || !_requireContinuous) &&
        ((dims <= 2 && (((ಅಡ್ಡಸಾಲುಗಳು == 1 || ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ == 1) && channels() == _elemChannels) ||
                        (ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ == _elemChannels && channels() == 1))) ||
        (dims == 3 && channels() == 1 && size.p[2] == _elemChannels && (size.p[0] == 1 || size.p[1] == 1) &&
         (isContinuous() || ಹೆಜ್ಜೆ.p[1] == ಹೆಜ್ಜೆ.p[2]*size.p[2])))
    ? (ಪೂರ್ಣಾಂಕ)(ಒಟ್ಟು()*channels()/_elemChannels) : -1;
}

UMat UMat::reshape(ಪೂರ್ಣಾಂಕ _cn, ಪೂರ್ಣಾಂಕ _newndims, ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* _newsz) ಸ್ಥಿರಾಂಕ
{
    ಆದರೆ(_newndims == dims)
    {
        ಆದರೆ(_newsz == 0)
            ಹಿಂತಿರುಗಿ reshape(_cn);
        ಆದರೆ(_newndims == 2)
            ಹಿಂತಿರುಗಿ reshape(_cn, _newsz[0]);
    }

    ಆದರೆ (isContinuous())
    {
        CV_Assert(_cn >= 0 && _newndims >= 0 && _newndims <= CV_MAX_DIM && (_newndims == 0 || _newsz != 0));

        ಆದರೆ (_cn == 0)
            _cn = ಇದು->channels();
        ಇಲ್ಲದಿದ್ದರೆ
            CV_Assert(_cn <= CV_CN_MAX);

        size_t ಒಟ್ಟು_elem1_ref = ಇದು->ಒಟ್ಟು() * ಇದು->channels();
        size_t ಒಟ್ಟು_elem1 = _cn;

        Auto_ಬಫರ್<ಪೂರ್ಣಾಂಕ, 4> newsz_buf( (size_t)std::max(_newndims, 1) );

        ಪೂರ್ಣಾಂಕ m1_idx = -1;

        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < _newndims; i++)
        {
            ಆದರೆ (_newsz[i] >= 0) {
                ಆದರೆ (_newsz[i] == 0 && i < dims)
                    newsz_buf[i] = size.p[i];
                ಇಲ್ಲದಿದ್ದರೆ
                    newsz_buf[i] = _newsz[i];
                ಒಟ್ಟು_elem1 *= (size_t)newsz_buf[i];
            } ಇಲ್ಲದಿದ್ದರೆ {
                ಆದರೆ (m1_idx >= 0)
                    CV_Error(cv::Error::Sts_Bad_ಗಾತ್ರ, "More than one '-1' occured in the new shape");
                m1_idx = i;
            }
        }

        ಆದರೆ (m1_idx >= 0) {
            ಆದರೆ (ಒಟ್ಟು_elem1 == 0) {
                CV_Assert(ಒಟ್ಟು_elem1_ref == 0);
                ಒಟ್ಟು_elem1 = 1;
            }
            CV_Assert(ಒಟ್ಟು_elem1_ref % ಒಟ್ಟು_elem1 == 0);
            newsz_buf[m1_idx] = (ಪೂರ್ಣಾಂಕ)(ಒಟ್ಟು_elem1_ref / ಒಟ್ಟು_elem1);
        } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಒಟ್ಟು_elem1 != ಒಟ್ಟು_elem1_ref) {
            CV_Error(cv::Error::StsUnmatchedSizes, "Requested and source matrices have different count of elements");
        }

        UMat hdr = *ಇದು;
        hdr.flags = (hdr.flags & ~CV_MAT_CN_ಮುಖವಾಡ) | ((_cn-1) << CV_CN_ಸರಿಸು);
        ಗಣ_ಗಾತ್ರ(hdr, _newndims, newsz_buf.ದತ್ತಾಂಶ(), ಖಾಲಿ, ನಿಜ);

        ಹಿಂತಿರುಗಿ hdr;
    }

    CV_Error(cv::Error::StsNotImplemented, "Reshaping of n-dimensional non-continuous matrices is not supported yet");
}

UMat UMat::reshape(ಪೂರ್ಣಾಂಕ _cn, ಸ್ಥಿರಾಂಕ MatShape& _newshape) ಸ್ಥಿರಾಂಕ
{
    ಆದರೆ (_newshape.dims < 0) {
        ಪೂರ್ಣಾಂಕ newshape[] = {0};
        ಹಿಂತಿರುಗಿ reshape(_cn, 1, newshape);
    }
    ಹಿಂತಿರುಗಿ reshape(_cn, _newshape.dims, _newshape.p);
}

Mat UMat::getMat(AccessFlag accessFlags) ಸ್ಥಿರಾಂಕ
{
    ಆದರೆ(!u)
        ಹಿಂತಿರುಗಿ Mat();
    // TODO Support ACCESS_READ (ACCESS_WRITE) without unnecessary data transfers
    accessFlags |= ACCESS_RW;
    UMat_ದತ್ತಾಂಶ_Auto_ಬೀಗ autolock(u);
    ಪ್ರಯತ್ನಿಸು
    {
        ಆದರೆ(CV_XADD(&u->refcount, 1) == 0)
            u->currAllocator->map(u, accessFlags);
        ಆದರೆ (u->ದತ್ತಾಂಶ != 0)
        {
            Mat hdr(dims, size.p, type(), u->ದತ್ತಾಂಶ + offset, ಹೆಜ್ಜೆ.p);
            hdr.flags = flags;
            hdr.u = u;
            hdr.datastart = u->ದತ್ತಾಂಶ;
            hdr.ದತ್ತಾಂಶ = u->ದತ್ತಾಂಶ + offset;
            hdr.datalimit = hdr.dataend = u->ದತ್ತಾಂಶ + u->size;
            ಹಿಂತಿರುಗಿ hdr;
        }
    }
    ಹಿಡಿ(...)
    {
        CV_XADD(&u->refcount, -1);
        ಎಸೆ;
    }
    CV_XADD(&u->refcount, -1);
    CV_Assert(u->ದತ್ತಾಂಶ != 0 && "Error mapping of UMat to host memory.");
    ಹಿಂತಿರುಗಿ Mat();
}

ಶೂನ್ಯ* UMat::handle(AccessFlag accessFlags) ಸ್ಥಿರಾಂಕ
{
    ಆದರೆ( !u )
        ಹಿಂತಿರುಗಿ 0;

    CV_Assert(u->refcount == 0);
    CV_Assert(!u->deviceCopyObsolete() || u->copyOnMap());
    ಆದರೆ (u->deviceCopyObsolete())
    {
        u->currAllocator->unmap(u);
    }

    ಆದರೆ (!!(accessFlags & ACCESS_WRITE))
        u->markHostCopyObsolete(ನಿಜ);

    ಹಿಂತಿರುಗಿ u->handle;
}

ಶೂನ್ಯ UMat::ndoffset(size_t* ofs) ಸ್ಥಿರಾಂಕ
{
    // offset = step[0]*ofs[0] + step[1]*ofs[1] + step[2]*ofs[2] + ...;
    size_t val = offset;
    ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = 0; i < dims; i++ )
    {
        size_t s = ಹೆಜ್ಜೆ.p[i];
        ofs[i] = val / s;
        val -= ofs[i]*s;
    }
}

ಶೂನ್ಯ UMat::copy_ಗಮ್ಯ(OutputArray _dst) ಸ್ಥಿರಾಂಕ
{
    CV_INSTRUMENT_REGION();

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_CUDA
    ಆದರೆ (_dst.isGpuMat())
    {
        _dst.getGpuMat().upload(*ಇದು);
        ಹಿಂತಿರುಗಿ;
    }
#ಆದರೆ_ಅಂತ್ಯ

    ಪೂರ್ಣಾಂಕ stype = type();
    ಪೂರ್ಣಾಂಕ dtype = _dst.type();
    ಆದರೆ( _dst.fixedType() && dtype != stype )
    {
        CV_Assert( CV_MAT_CN(stype) == CV_MAT_CN(dtype) );
        convert_ಗಮ್ಯ( _dst, dtype );
        ಹಿಂತಿರುಗಿ;
    }

    ಆದರೆ( dims == 0 && empty() )
    {
        _dst.release();
        ಶೂನ್ಯ* obj = _dst.getObj();
        ಆದರೆ (_dst.isMat())
            reinterpret_cast<Mat*>(obj)->flags = Mat::MAGIC_VAL | Mat::CONTINUOUS_FLAG | stype;
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (_dst.isUMat())
            reinterpret_cast<UMat*>(obj)->flags = UMat::MAGIC_VAL | UMat::CONTINUOUS_FLAG | stype;
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (_dst.isGpuMat())
            reinterpret_cast<cuda::GpuMat*>(obj)->flags = stype;
        ಹಿಂತಿರುಗಿ;
    }

    _dst.create( dims, size.p, stype );
    ಆದರೆ (empty())
        ಹಿಂತಿರುಗಿ;

    size_t sz[CV_MAX_DIM] = {1}, srcofs[CV_MAX_DIM]={0}, dstofs[CV_MAX_DIM]={0};
    size_t esz = CV_ELEM_ಗಾತ್ರ(stype);
    ಪೂರ್ಣಾಂಕ i, d = std::max(dims, 1);
    ಗಾಗಿ( i = 0; i < d; i++ )
        sz[i] = size.p[i];
    sz[d-1] *= esz;
    ndoffset(srcofs);
    srcofs[d-1] *= esz;

    ಆದರೆ( _dst.isUMat() )
    {
        UMat dst = _dst.getUMat();
        CV_Assert(dst.u);
        ಆದರೆ( u == dst.u && dst.offset == offset )
            ಹಿಂತಿರುಗಿ;

        ಆದರೆ (u->currAllocator == dst.u->currAllocator)
        {
            dst.ndoffset(dstofs);
            dstofs[dims-1] *= esz;
            u->currAllocator->copy(u, dst.u, d, sz, srcofs, ಹೆಜ್ಜೆ.p, dstofs, dst.ಹೆಜ್ಜೆ.p, ಸುಳ್ಳು);
            ಹಿಂತಿರುಗಿ;
        }

        // different allocators: never map a device destination, upload into it instead
        // (through a host Mat when both sides are device memory)
        MatAllocator* hostAllocator = Mat::getDefaultAllocator();
        ಆದರೆ (dst.u->currAllocator != hostAllocator)
        {
            ಆದರೆ (u->currAllocator == hostAllocator)
                getMat(ACCESS_READ).copy_ಗಮ್ಯ(_dst);
            ಇಲ್ಲದಿದ್ದರೆ
            {
                Mat tmp;
                copy_ಗಮ್ಯ(tmp);
                tmp.copy_ಗಮ್ಯ(_dst);
            }
            ಹಿಂತಿರುಗಿ;
        }
    }

    Mat dst = _dst.getMat();
    u->currAllocator->download(u, dst.ptr(), d, sz, srcofs, ಹೆಜ್ಜೆ.p, dst.ಹೆಜ್ಜೆ.p);
}

ಶೂನ್ಯ UMat::copy_ಗಮ್ಯ(OutputArray _dst, ಒಳಹರಿವು_Array ಮುಖವಾಡ_2) ಸ್ಥಿರಾಂಕ
{
    CV_INSTRUMENT_REGION();

    ಆದರೆ( ಮುಖವಾಡ_2.empty() )
    {
        copy_ಗಮ್ಯ(_dst);
        ಹಿಂತಿರುಗಿ;
    }
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_OPENCL
    ಪೂರ್ಣಾಂಕ cn = channels(), mtype = ಮುಖವಾಡ_2.type(), mdepth = CV_MAT_ಆಳ(mtype), mcn = CV_MAT_CN(mtype);
    CV_Assert( (mdepth == CV_8U || mdepth == CV_8S || mdepth == CV_Bool) && (mcn == 1 || mcn == cn) );

    ಆದರೆ (ocl::useOpenCL() && _dst.isUMat() && dims <= 2)
    {
        UMat_ದತ್ತಾಂಶ * prevu = _dst.getUMat().u;
        _dst.create( size, type() );

        UMat dst = _dst.getUMat();

        ತಾರ್ಕಿಕ haveDstUninit = ಸುಳ್ಳು;
        ಆದರೆ( prevu != dst.u ) // do not leave dst uninitialized
            haveDstUninit = ನಿಜ;

        String opts = format("-D COPY_TO_MASK -D T1=%s -D scn=%d -D mcn=%d%s",
                             ocl::memop_Type_ಗಮ್ಯ_Str(ಆಳ()), cn, mcn,
                             haveDstUninit ? " -D HAVE_DST_UNINIT" : "");

        ocl::Kernel k("copyToMask", ocl::core::copyset_oclsrc, opts);
        ಆದರೆ (!k.empty())
        {
            k.ವಾದಗಳು(ocl::KernelArg::Read_Only_No_ಗಾತ್ರ(*ಇದು),
                   ocl::KernelArg::Read_Only_No_ಗಾತ್ರ(ಮುಖವಾಡ_2.getUMat()),
                   haveDstUninit ? ocl::KernelArg::WriteOnly(dst) :
                                   ocl::KernelArg::ReadWrite(dst));

            size_t globalsize[2] = { (size_t)ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, (size_t)ಅಡ್ಡಸಾಲುಗಳು };
            ಆದರೆ (k.ಸತತ(2, globalsize, ಖಾಲಿ, ಸುಳ್ಳು))
            {
                CV_IMPL_ಕೂಡಿಸು(CV_IMPL_OCL);
                ಹಿಂತಿರುಗಿ;
            }
        }
    }
#ಆದರೆ_ಅಂತ್ಯ
    Mat src = getMat(ACCESS_READ);
    src.copy_ಗಮ್ಯ(_dst, ಮುಖವಾಡ_2);
}


//
// void UMat::convertTo moved to convert.dispatch.cpp
//

UMat& UMat::ಗಣ_ಗಮ್ಯ(ಒಳಹರಿವು_Array ಮೌಲ್ಯ, ಒಳಹರಿವು_Array ಮುಖವಾಡ_2)
{
    CV_INSTRUMENT_REGION();

    ತಾರ್ಕಿಕ have_ಮುಖವಾಡ = !ಮುಖವಾಡ_2.empty();
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_OPENCL
    ಪೂರ್ಣಾಂಕ tp = type(), cn = CV_MAT_CN(tp), d = CV_MAT_ಆಳ(tp);

    ಆದರೆ( dims <= 2 && cn <= 4 && CV_MAT_ಆಳ(tp) < CV_64F && ocl::useOpenCL() )
    {
        Mat value = ಮೌಲ್ಯ.getMat();
        CV_Assert( checkScalar(value, type(), ಮೌಲ್ಯ.ಬಗೆ(), ಒಳಹರಿವು_Array_2::UMAT) );
        ಪೂರ್ಣಾಂಕ kercn = have_ಮುಖವಾಡ || cn == 3 ? cn : std::max(cn, ocl::predict_Optimal_ಸದಿಶ_ಅಗಲ(*ಇದು)),
                kertp = CV_MAKE_TYPE(d, kercn);

        ದ್ವಿಗುಣ buf[16] = { 0, 0, 0, 0, 0, 0, 0, 0,
                           0, 0, 0, 0, 0, 0, 0, 0 };
        convertAndUnrollScalar(value, tp, (uchar *)buf, kercn / cn);

        ಪೂರ್ಣಾಂಕ scalarcn = kercn == 3 ? 4 : kercn, ಅಡ್ಡಸಾಲುಗಳು_ಪ್ರತಿ_WI = ocl::Device::getDefault().isIntel() ? 4 : 1;
        String opts = format("-D dstT=%s -D rowsPerWI=%d -D dstST=%s -D dstT1=%s -D cn=%d",
                             ocl::memop_Type_ಗಮ್ಯ_Str(kertp), ಅಡ್ಡಸಾಲುಗಳು_ಪ್ರತಿ_WI,
                             ocl::memop_Type_ಗಮ್ಯ_Str(CV_MAKETYPE(d, scalarcn)),
                             ocl::memop_Type_ಗಮ್ಯ_Str(d), kercn);

        ocl::Kernel ಗಣ_K(have_ಮುಖವಾಡ ? "setMask" : "set", ocl::core::copyset_oclsrc, opts);
        ಆದರೆ( !ಗಣ_K.empty() )
        {
            ocl::KernelArg scalararg(ocl::KernelArg::CONSTANT, 0, 0, 0, buf, CV_ELEM_ಗಾತ್ರ(d) * scalarcn);
            UMat ಮುಖವಾಡ;

            ಆದರೆ( have_ಮುಖವಾಡ )
            {
                ಮುಖವಾಡ = ಮುಖವಾಡ_2.getUMat();
                CV_Assert( ಮುಖವಾಡ.size() == size() && (ಮುಖವಾಡ.type() == CV_8U || ಮುಖವಾಡ.type() == CV_8S || ಮುಖವಾಡ.type() == CV_Bool) );
                ocl::KernelArg maskarg = ocl::KernelArg::Read_Only_No_ಗಾತ್ರ(ಮುಖವಾಡ),
                        dstarg = ocl::KernelArg::ReadWrite(*ಇದು);
                ಗಣ_K.ವಾದಗಳು(maskarg, dstarg, scalararg);
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                ocl::KernelArg dstarg = ocl::KernelArg::WriteOnly(*ಇದು, cn, kercn);
                ಗಣ_K.ವಾದಗಳು(dstarg, scalararg);
            }

            size_t globalsize[] = { (size_t)ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ * cn / kercn, ((size_t)ಅಡ್ಡಸಾಲುಗಳು + ಅಡ್ಡಸಾಲುಗಳು_ಪ್ರತಿ_WI - 1) / ಅಡ್ಡಸಾಲುಗಳು_ಪ್ರತಿ_WI };
            ಆದರೆ( ಗಣ_K.ಸತತ(2, globalsize, ಖಾಲಿ, ಸುಳ್ಳು) )
            {
                CV_IMPL_ಕೂಡಿಸು(CV_IMPL_OCL);
                ಹಿಂತಿರುಗಿ *ಇದು;
            }
        }
    }
#ಆದರೆ_ಅಂತ್ಯ
    Mat m = getMat(have_ಮುಖವಾಡ ? ACCESS_RW : ACCESS_WRITE);
    m.ಗಣ_ಗಮ್ಯ(ಮೌಲ್ಯ, ಮುಖವಾಡ_2);
    ಹಿಂತಿರುಗಿ *ಇದು;
}

UMat& UMat::ಕ್ರಿಯಾಕಾರಕ = (ಸ್ಥಿರಾಂಕ Scalar& s)
{
    ಗಣ_ಗಮ್ಯ(s);
    ಹಿಂತಿರುಗಿ *ಇದು;
}

UMat UMat::t() ಸ್ಥಿರಾಂಕ
{
    UMat m;
    transpose(*ಇದು, m);
    ಹಿಂತಿರುಗಿ m;
}

UMat UMat::zeros(ಪೂರ್ಣಾಂಕ ಅಡ್ಡಸಾಲುಗಳು, ಪೂರ್ಣಾಂಕ ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, ಪೂರ್ಣಾಂಕ type, UMatUsageFlags usageFlags)
{
    ಹಿಂತಿರುಗಿ UMat(ಅಡ್ಡಸಾಲುಗಳು, ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, type, Scalar::all(0), usageFlags);
}

UMat UMat::zeros(ಗಾತ್ರ_2 size, ಪೂರ್ಣಾಂಕ type, UMatUsageFlags usageFlags)
{
    ಹಿಂತಿರುಗಿ UMat(size, type, Scalar::all(0), usageFlags);
}

UMat UMat::zeros(ಪೂರ್ಣಾಂಕ ndims, ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* sz, ಪೂರ್ಣಾಂಕ type, UMatUsageFlags usageFlags)
{
    ಹಿಂತಿರುಗಿ UMat(ndims, sz, type, Scalar::all(0), usageFlags);
}

UMat UMat::zeros(ಸ್ಥಿರಾಂಕ MatShape& shape, ಪೂರ್ಣಾಂಕ type, UMatUsageFlags usageFlags)
{
    ಹಿಂತಿರುಗಿ UMat(shape.dims, shape.p, type, Scalar::all(0), usageFlags);
}

UMat UMat::ones(ಪೂರ್ಣಾಂಕ ಅಡ್ಡಸಾಲುಗಳು, ಪೂರ್ಣಾಂಕ ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, ಪೂರ್ಣಾಂಕ type, UMatUsageFlags usageFlags)
{
    ಹಿಂತಿರುಗಿ UMat(ಅಡ್ಡಸಾಲುಗಳು, ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, type, Scalar(1), usageFlags);
}

UMat UMat::ones(ಗಾತ್ರ_2 size, ಪೂರ್ಣಾಂಕ type, UMatUsageFlags usageFlags)
{
    ಹಿಂತಿರುಗಿ UMat(size, type, Scalar(1), usageFlags);
}

UMat UMat::ones(ಪೂರ್ಣಾಂಕ ndims, ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* sz, ಪೂರ್ಣಾಂಕ type, UMatUsageFlags usageFlags)
{
    ಹಿಂತಿರುಗಿ UMat(ndims, sz, type, Scalar(1), usageFlags);
}

UMat UMat::ones(ಸ್ಥಿರಾಂಕ MatShape& shape, ಪೂರ್ಣಾಂಕ type, UMatUsageFlags usageFlags)
{
    ಹಿಂತಿರುಗಿ UMat(shape.dims, shape.p, type, Scalar(1), usageFlags);
}

}

/* End of file. */
