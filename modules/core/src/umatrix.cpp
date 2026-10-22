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
ಶೂನ್ಯ setSize(UMat& m, ಪೂರ್ಣಾಙ್ಕ _dims, ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ* _sz, ಧ್ರುವ size_t* _steps,
             ತಾರ್ಕಿಕ autoSteps = ಅಸತ್ಯ);

ಶೂನ್ಯ updateContinuityFlag(UMat& m);
ಶೂನ್ಯ finalizeHdr(UMat& m);

UMatData::UMatData(ಧ್ರುವ MatAllocator* allocator)
{
    prevAllocator = currAllocator = allocator;
    urefcount = refcount = mapcount = 0;
    data = origdata = 0;
    size = 0;
    flags = static_cast<UMatData::MemoryFlag>(0);
    handle = 0;
    userdata = 0;
    allocatorFlags_ = 0;
    originalUMatData = ರಿಕ್ತ;
}

UMatData::~UMatData()
{
    prevAllocator = currAllocator = 0;
    urefcount = refcount = 0;
    CV_Assert(mapcount == 0);
    data = origdata = 0;
    size = 0;
    ತಾರ್ಕಿಕ isAsyncCleanup = !!(flags & UMatData::ASYNC_CLEANUP);
    flags = static_cast<UMatData::MemoryFlag>(0);
    handle = 0;
    userdata = 0;
    allocatorFlags_ = 0;
    ಯದಿ (originalUMatData)
    {
        ತಾರ್ಕಿಕ showWarn = ಅಸತ್ಯ;
        UMatData* u = originalUMatData;
        ತಾರ್ಕಿಕ zero_Ref = CV_XADD(&(u->refcount), -1) == 1;
        ಯದಿ (zero_Ref)
        {
            // simulate Mat::deallocate
            ಯದಿ (u->mapcount != 0)
            {
                (u->currAllocator ? u->currAllocator : /* TODO allocator ? allocator :*/ Mat::getDefaultAllocator())->unmap(u);
            }
            ಅನ್ಯಥಾ
            {
                // we don't do "map", so we can't do "unmap"
            }
        }
        ತಾರ್ಕಿಕ zero_URef = CV_XADD(&(u->urefcount), -1) == 1;
        ಯದಿ (zero_Ref && !zero_URef)
            showWarn = ಸತ್ಯ;
        ಯದಿ (zero_Ref && zero_URef) // oops, we need to free resources
        {
            showWarn = !isAsyncCleanup;
            // simulate UMat::deallocate
            u->currAllocator->deallocate(u);
        }
#ifndef NDEBUG
        ಯದಿ (showWarn)
        {
            ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ warn_message_showed = 0;
            ಯದಿ (warn_message_showed++ < 100)
            {
                fflush(stdout);
                fprintf(stderr, "\n! OPENCV warning: getUMat()/getMat() call chain possible problem."
                                "\n!                 Base object is dead, while nested/derived object is still alive or processed."
                                "\n!                 Please check lifetime of UMat/Mat objects!\n");
                fflush(stderr);
            }
        }
#ಅನ್ಯಥಾ
        CV_UNUSED(showWarn);
#endif
        originalUMatData = ರಿಕ್ತ;
    }
}

#ifndef OPENCV_DISABLE_THREAD_SUPPORT

// it should be a prime number for the best hash function
ಗಣನಾ { UMAT_NLOCKS = 31 };
ಸ್ಥಿರ Mutex umatLocks[UMAT_NLOCKS];

ಸ್ಥಿರ size_t getUMatDataLockIndex(ಧ್ರುವ UMatData* u)
{
    size_t idx = ((size_t)(ಶೂನ್ಯ*)u) % UMAT_NLOCKS;
    ಪ್ರತ್ಯಾಗಮ idx;
}

ಶೂನ್ಯ UMatData::lock()
{
    size_t idx = getUMatDataLockIndex(ಏತತ್);
    //printf("%d lock(%d)\n", cv::utils::getThreadID(), (int)idx);
    umatLocks[idx].lock();
}

ಶೂನ್ಯ UMatData::unlock()
{
    size_t idx = getUMatDataLockIndex(ಏತತ್);
    //printf("%d unlock(%d)\n", cv::utils::getThreadID(), (int)idx);
    umatLocks[idx].unlock();
}


// Do not allow several lock() calls with different UMatData objects.
ಸಂರಚನಾ UMatDataAutoLocker
{
    ಪೂರ್ಣಾಙ್ಕ usage_ಎಣಿಕೆ;
    UMatData* locked_objects[2];
    UMatDataAutoLocker() : usage_ಎಣಿಕೆ(0) { locked_objects[0] = ರಿಕ್ತ; locked_objects[1] = ರಿಕ್ತ; }

    ಶೂನ್ಯ lock(UMatData*& u1)
    {
        ತಾರ್ಕಿಕ locked_1 = (u1 == locked_objects[0] || u1 == locked_objects[1]);
        ಯದಿ (locked_1)
        {
            u1 = ರಿಕ್ತ;
            ಪ್ರತ್ಯಾಗಮ;
        }
        CV_Assert(usage_ಎಣಿಕೆ == 0);  // UMatDataAutoLock can't be used multiple times from the same thread
        usage_ಎಣಿಕೆ = 1;
        locked_objects[0] = u1;
        u1->lock();
    }
    ಶೂನ್ಯ lock(UMatData*& u1, UMatData*& u2)
    {
        ತಾರ್ಕಿಕ locked_1 = (u1 == locked_objects[0] || u1 == locked_objects[1]);
        ತಾರ್ಕಿಕ locked_2 = (u2 == locked_objects[0] || u2 == locked_objects[1]);
        ಯದಿ (locked_1)
            u1 = ರಿಕ್ತ;
        ಯದಿ (locked_2)
            u2 = ರಿಕ್ತ;
        ಯದಿ (locked_1 && locked_2)
            ಪ್ರತ್ಯಾಗಮ;
        CV_Assert(usage_ಎಣಿಕೆ == 0);  // UMatDataAutoLock can't be used multiple times from the same thread
        usage_ಎಣಿಕೆ = 1;
        locked_objects[0] = u1;
        locked_objects[1] = u2;
        ಯದಿ (u1)
            u1->lock();
        ಯದಿ (u2)
            u2->lock();
    }
    ಶೂನ್ಯ release(UMatData* u1, UMatData* u2)
    {
        ಯದಿ (u1 == ರಿಕ್ತ && u2 == ರಿಕ್ತ)
            ಪ್ರತ್ಯಾಗಮ;
        CV_Assert(usage_ಎಣಿಕೆ == 1);
        usage_ಎಣಿಕೆ = 0;
        ಯದಿ (u1)
            u1->unlock();
        ಯದಿ (u2)
            u2->unlock();
        locked_objects[0] = ರಿಕ್ತ; locked_objects[1] = ರಿಕ್ತ;
    }
};
ಸ್ಥಿರ TLSData<UMatDataAutoLocker>& getUMatDataAutoLockerTLS()
{
    CV_SINGLETON_LAZY_INIT_REF(TLSData<UMatDataAutoLocker>, ನೂತನ TLSData<UMatDataAutoLocker>());
}
ಸ್ಥಿರ UMatDataAutoLocker& getUMatDataAutoLocker() { ಪ್ರತ್ಯಾಗಮ getUMatDataAutoLockerTLS().getRef(); }


UMatDataAutoLock::UMatDataAutoLock(UMatData* u) : u1(u), u2(ರಿಕ್ತ)
{
    getUMatDataAutoLocker().lock(u1);
}
UMatDataAutoLock::UMatDataAutoLock(UMatData* u1_, UMatData* u2_) : u1(u1_), u2(u2_)
{
    ಯದಿ (getUMatDataLockIndex(u1) > getUMatDataLockIndex(u2))
    {
        std::swap(u1, u2);
    }
    getUMatDataAutoLocker().lock(u1, u2);
}
UMatDataAutoLock::~UMatDataAutoLock()
{
    getUMatDataAutoLocker().release(u1, u2);
}

#ಅನ್ಯಥಾ

ಶೂನ್ಯ UMatData::lock()
{
    // nothing in OPENCV_DISABLE_THREAD_SUPPORT mode
}

ಶೂನ್ಯ UMatData::unlock()
{
    // nothing in OPENCV_DISABLE_THREAD_SUPPORT mode
}

UMatDataAutoLock::UMatDataAutoLock(UMatData* u) : u1(u), u2(ರಿಕ್ತ)
{
    // nothing in OPENCV_DISABLE_THREAD_SUPPORT mode
}
UMatDataAutoLock::UMatDataAutoLock(UMatData* u1_, UMatData* u2_) : u1(u1_), u2(u2_)
{
    // nothing in OPENCV_DISABLE_THREAD_SUPPORT mode
}
UMatDataAutoLock::~UMatDataAutoLock()
{
    // nothing in OPENCV_DISABLE_THREAD_SUPPORT mode
}

#endif  // OPENCV_DISABLE_THREAD_SUPPORT

//////////////////////////////// UMat ////////////////////////////////

UMat::UMat(UMatUsageFlags _usageFlags) CV_NOEXCEPT
: flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{}

UMat::UMat(ಪೂರ್ಣಾಙ್ಕ _rows, ಪೂರ್ಣಾಙ್ಕ _cols, ಪೂರ್ಣಾಙ್ಕ _type, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_rows, _cols, _type);
}

UMat::UMat(ಪೂರ್ಣಾಙ್ಕ _rows, ಪೂರ್ಣಾಙ್ಕ _cols, ಪೂರ್ಣಾಙ್ಕ _type, ಧ್ರುವ Scalar& _s, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_rows, _cols, _type);
    *ಏತತ್ = _s;
}

UMat::UMat(Size _sz, ಪೂರ್ಣಾಙ್ಕ _type, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create( _sz.height, _sz.width, _type );
}

UMat::UMat(Size _sz, ಪೂರ್ಣಾಙ್ಕ _type, ಧ್ರುವ Scalar& _s, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_sz.height, _sz.width, _type);
    *ಏತತ್ = _s;
}

UMat::UMat(ಪೂರ್ಣಾಙ್ಕ _dims, ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ* _sz, ಪೂರ್ಣಾಙ್ಕ _type, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_dims, _sz, _type);
}

UMat::UMat(ಪೂರ್ಣಾಙ್ಕ _dims, ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ* _sz, ಪೂರ್ಣಾಙ್ಕ _type, ಧ್ರುವ Scalar& _s, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_dims, _sz, _type);
    *ಏತತ್ = _s;
}

UMat::UMat(ಧ್ರುವ MatShape& _shape, ಪೂರ್ಣಾಙ್ಕ _type, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_shape.dims, _shape.p, _type);
}

UMat::UMat(ಧ್ರುವ MatShape& _shape, ಪೂರ್ಣಾಙ್ಕ _type, ಧ್ರುವ Scalar& _s, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_shape.dims, _shape.p, _type);
    *ಏತತ್ = _s;
}

UMat::UMat(ಧ್ರುವ UMat& m)
: flags(m.flags), dims(m.dims), rows(m.rows), cols(m.cols), allocator(m.allocator),
  usageFlags(m.usageFlags), u(m.u), offset(m.offset), size(m.size), step(m.step)
{
    addref();
}

UMat& UMat::operator=(ಧ್ರುವ UMat& m)
{
    ಯದಿ( ಏತತ್ != &m )
    {
        const_cast<UMat&>(m).addref();
        release();
        flags = m.flags;
        dims = m.dims;
        rows = m.rows;
        cols = m.cols;
        size = m.size;
        step = m.step;
        allocator = m.allocator;
        usageFlags = m.usageFlags;
        u = m.u;
        offset = m.offset;
    }
    ಪ್ರತ್ಯಾಗಮ *ಏತತ್;
}

UMat UMat::clone() ಧ್ರುವ
{
    UMat m;
    copyTo(m);
    ಪ್ರತ್ಯಾಗಮ m;
}

ಶೂನ್ಯ UMat::assignTo(UMat& m, ಪೂರ್ಣಾಙ್ಕ _type) ಧ್ರುವ
{
    ಯದಿ( _type < 0 )
        m = *ಏತತ್;
    ಅನ್ಯಥಾ
        convertTo(m, _type);
}

ಶೂನ್ಯ UMat::create(ಪೂರ್ಣಾಙ್ಕ _rows, ಪೂರ್ಣಾಙ್ಕ _cols, ಪೂರ್ಣಾಙ್ಕ _type, UMatUsageFlags _usageFlags)
{
    ಪೂರ್ಣಾಙ್ಕ sz[] = {_rows, _cols};
    create(2, sz, _type, _usageFlags);
}

ಶೂನ್ಯ UMat::create(Size _sz, ಪೂರ್ಣಾಙ್ಕ _type, UMatUsageFlags _usageFlags)
{
    create(_sz.height, _sz.width, _type, _usageFlags);
}

ಶೂನ್ಯ UMat::createSameSize(ಒಳಹರಿವು_Array arr, ಪೂರ್ಣಾಙ್ಕ _type, UMatUsageFlags _usageFlags)
{
    create(arr.shape(), _type, _usageFlags);
}

ಶೂನ್ಯ UMat::addref()
{
    ಯದಿ( u )
        CV_XADD(&(u->urefcount), 1);
}

ಶೂನ್ಯ UMat::release()
{
    ಯದಿ( u && CV_XADD(&(u->urefcount), -1) == 1 )
        deallocate();
    u = 0;
    size.clear();
    dims = cols = rows = 0;
}

ತಾರ್ಕಿಕ UMat::empty() ಧ್ರುವ
{
    ಪ್ರತ್ಯಾಗಮ u == 0 || total() == 0;
}

size_t UMat::total() ಧ್ರುವ
{
    ಯದಿ( dims <= 2 )
        ಪ್ರತ್ಯಾಗಮ (size_t)rows * cols;
    size_t p = 1;
    ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ i = 0; i < dims; i++ )
        p *= size[i];
    ಪ್ರತ್ಯಾಗಮ p;
}

MatShape UMat::shape() ಧ್ರುವ
{
    ಪ್ರತ್ಯಾಗಮ size;
}

UMat::UMat(UMat&& m)
: flags(m.flags), dims(m.dims), rows(m.rows), cols(m.cols), allocator(m.allocator),
  usageFlags(m.usageFlags), u(m.u), offset(m.offset)
{
    size = m.size;
    step = m.step;
    m.flags = MAGIC_VAL;
    m.usageFlags = USAGE_DEFAULT;
    m.dims = m.rows = m.cols = 0;
    m.size.clear();
    m.allocator = ರಿಕ್ತ;
    m.u = ರಿಕ್ತ;
    m.offset = 0;
}

UMat& UMat::operator=(UMat&& m)
{
    ಯದಿ (ಏತತ್ == &m)
      ಪ್ರತ್ಯಾಗಮ *ಏತತ್;
    release();
    flags = m.flags; dims = m.dims; rows = m.rows; cols = m.cols;
    allocator = m.allocator; usageFlags = m.usageFlags;
    u = m.u;
    offset = m.offset;
    size = m.size;
    step = m.step;
    m.flags = MAGIC_VAL;
    m.usageFlags = USAGE_DEFAULT;
    m.dims = m.rows = m.cols = 0;
    m.allocator = ರಿಕ್ತ;
    m.u = ರಿಕ್ತ;
    m.offset = 0;
    ಪ್ರತ್ಯಾಗಮ *ಏತತ್;
}


MatAllocator* UMat::getStdAllocator()
{
#ifdef HAVE_OPENCL
    ಯದಿ (ocl::useOpenCL())
        ಪ್ರತ್ಯಾಗಮ ocl::getOpenCLAllocator();
#endif
    ಪ್ರತ್ಯಾಗಮ Mat::getDefaultAllocator();
}

ಶೂನ್ಯ swap( UMat& a, UMat& b )
{
    std::swap(a.flags, b.flags);
    std::swap(a.dims, b.dims);
    std::swap(a.rows, b.rows);
    std::swap(a.cols, b.cols);
    std::swap(a.allocator, b.allocator);
    std::swap(a.u, b.u);
    std::swap(a.offset, b.offset);

    std::swap(a.size, b.size);
    std::swap(a.step, b.step);
}


ಶೂನ್ಯ setSize( UMat& m, ಪೂರ್ಣಾಙ್ಕ _dims, ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ* _sz,
              ಧ್ರುವ size_t* _steps, ತಾರ್ಕಿಕ autoSteps )
{
    CV_Assert( 0 <= _dims && _dims <= CV_MAX_DIM && _dims <= MatShape::MAX_DIMS);

    m.dims = _dims;
    m.size = MatShape(_dims, _sz);
    m.step[std::max(_dims-1, 0)] = CV_ELEM_SIZE(m.flags);
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = _dims-2; i >= 0; i--) {
        size_t autostep = m.size[i+1]*m.step[i+1];
        ಯದಿ (_steps) {
            m.step[i] = _steps[i];
            //CV_Assert(m.step[i] >= autostep);
        } ಅನ್ಯಥಾ ಯದಿ (autoSteps) {
            m.step[i] = autostep;
        } ಅನ್ಯಥಾ {
            m.step[i] = 0;
        }
    }

    ಯದಿ( _dims <= 2 )
    {
        m.cols = _dims == 0 ? 1 : _sz ? _sz[_dims > 1] : 0;
        m.rows = _dims < 2 ? 1 : _sz ? _sz[0] : 0;
    } ಅನ್ಯಥಾ {
        m.cols = m.rows = -1;
    }
}


ಶೂನ್ಯ UMat::updateContinuityFlag()
{
    flags = cv::updateContinuityFlag(flags, dims, size.p, step.p);
}


ಶೂನ್ಯ finalizeHdr(UMat& m)
{
    m.updateContinuityFlag();
    ಪೂರ್ಣಾಙ್ಕ d = m.dims;
    ಯದಿ( d > 2 )
        m.rows = m.cols = -1;
}


UMat Mat::getUMat(AccessFlag accessFlags, UMatUsageFlags usageFlags) ಧ್ರುವ
{
    UMat hdr;
    ಯದಿ(!data)
        ಪ್ರತ್ಯಾಗಮ hdr;
    ಯದಿ (data != datastart)
    {
        Size wholeSize;
        Point ofs;
        locateROI(wholeSize, ofs);
        Size sz(cols, rows);
        ಯದಿ (ofs.x != 0 || ofs.y != 0)
        {
            Mat src = *ಏತತ್;
            ಪೂರ್ಣಾಙ್ಕ dtop = ofs.y;
            ಪೂರ್ಣಾಙ್ಕ dbottom = wholeSize.height - src.rows - ofs.y;
            ಪೂರ್ಣಾಙ್ಕ dleft = ofs.x;
            ಪೂರ್ಣಾಙ್ಕ dright = wholeSize.width - src.cols - ofs.x;
            src.adjustROI(dtop, dbottom, dleft, dright);
            ಪ್ರತ್ಯಾಗಮ src.getUMat(accessFlags, usageFlags)(cv::Rect(ofs.x, ofs.y, sz.width, sz.height));
        }
    }
    CV_Assert(data == datastart);

    accessFlags |= ACCESS_RW;
    UMatData* new_u = ರಿಕ್ತ;
    MatStep new_step = step;
    {
        MatAllocator *a = allocator, *a0 = getDefaultAllocator();
        ಯದಿ(!a)
            a = a0;
        new_u = a->allocate(dims, size.p, type(), data, new_step.p, accessFlags, usageFlags);
        new_u->originalUMatData = u;
    }
    ತಾರ್ಕಿಕ allocated = ಅಸತ್ಯ;
    ಯತ್ನ
    {
        allocated = UMat::getStdAllocator()->allocate(new_u, accessFlags, usageFlags);
    }
    ಗೃಹಾಣ (ಧ್ರುವ cv::Exception& e)
    {
        fprintf(stderr, "Exception: %s\n", e.what());
    }
    ಯದಿ (!allocated)
    {
        allocated = getDefaultAllocator()->allocate(new_u, accessFlags, usageFlags);
        CV_Assert(allocated);
    }
    ಯದಿ (u != ರಿಕ್ತ)
    {
#ifdef HAVE_OPENCL
        ಯದಿ (ocl::useOpenCL() && new_u->currAllocator == ocl::getOpenCLAllocator())
        {
            CV_Assert(new_u->tempUMat());
        }
#endif
        CV_XADD(&(u->refcount), 1);
        CV_XADD(&(u->urefcount), 1);
    }
    ಯತ್ನ
    {
        hdr.flags = flags;
        hdr.usageFlags = usageFlags;
        setSize(hdr, dims, size.p, new_step.p);
        finalizeHdr(hdr);
        hdr.u = new_u;
        hdr.offset = 0; //data - datastart;
        hdr.addref();
        ಪ್ರತ್ಯಾಗಮ hdr;
    }
    ಗೃಹಾಣ(...)
    {
        ಯದಿ (u != ರಿಕ್ತ)
        {
            CV_XADD(&(u->refcount), -1);
            CV_XADD(&(u->urefcount), -1);
        }
        new_u->currAllocator->deallocate(new_u);
        ಕ್ಷಿಪ;
    }

}

ಶೂನ್ಯ UMat::create(ಪೂರ್ಣಾಙ್ಕ d0, ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ* _sizes, ಪೂರ್ಣಾಙ್ಕ _type, UMatUsageFlags _usageFlags)
{
    ಪೂರ್ಣಾಙ್ಕ sz1 = 1, d = d0;
    ಪೂರ್ಣಾಙ್ಕ i;
    ಯದಿ (d == 0) {
        d = 1;
        _sizes = (ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ*)&sz1;
    }
    CV_Assert(0 <= d && d <= CV_MAX_DIM && _sizes);
    _type = CV_MAT_TYPE(_type);

    // if param value is USAGE_DEFAULT by implicit default param value -or- explicit value
    // ...then don't change the existing usageFlags
    // it is not possible to change usage from non-default to USAGE_DEFAULT through create()
    // ...instead must construct UMat()
    ಯದಿ (_usageFlags == cv::USAGE_DEFAULT)
    {
        _usageFlags = usageFlags;
    }

    ಯದಿ( u && d == dims && _type == type() && _usageFlags == usageFlags )
    {
        ಪ್ರತಿ( i = 0; i < d; i++ )
            ಯದಿ( size[i] != _sizes[i] )
                ಭಙ್ಗ;
        ಯದಿ( i == d )
            ಪ್ರತ್ಯಾಗಮ;
    }

    ಪೂರ್ಣಾಙ್ಕ _sizes_backup[CV_MAX_DIM]; // #5991
    ಯದಿ (_sizes == (ಏತತ್->size.p))
    {
        ಪ್ರತಿ(i = 0; i < d; i++ )
            _sizes_backup[i] = _sizes[i];
        _sizes = _sizes_backup;
    }

    release();
    usageFlags = _usageFlags;
    ಯದಿ( d == 0 )
        ಪ್ರತ್ಯಾಗಮ;
    flags = (_type & CV_MAT_TYPE_MASK) | MAGIC_VAL;
    setSize(*ಏತತ್, d, _sizes, 0, ಸತ್ಯ);
    offset = 0;

    ಯದಿ( total() > 0 )
    {
        MatAllocator *a = allocator, *a0 = getStdAllocator();
        ಯದಿ (!a)
        {
            a = a0;
            a0 = Mat::getDefaultAllocator();
        }
        ಯತ್ನ
        {
            u = a->allocate(dims, size.p, _type, 0, step.p, ACCESS_RW /* ignored */, usageFlags);
            CV_Assert(u != 0);
        }
        ಗೃಹಾಣ(...)
        {
            ಯದಿ(a != a0)
                u = a0->allocate(dims, size.p, _type, 0, step.p, ACCESS_RW /* ignored */, usageFlags);
            CV_Assert(u != 0);
        }
        CV_Assert( step[dims-1] == (size_t)CV_ELEM_SIZE(flags) );
    }

    finalizeHdr(*ಏತತ್);
    addref();
    size.dims = dims = d0;
}

ಶೂನ್ಯ UMat::create(ಧ್ರುವ std::vector<ಪೂರ್ಣಾಙ್ಕ>& _sizes, ಪೂರ್ಣಾಙ್ಕ _type, UMatUsageFlags _usageFlags)
{
    create((ಪೂರ್ಣಾಙ್ಕ)_sizes.size(), _sizes.data(), _type, _usageFlags);
}

ಶೂನ್ಯ UMat::create(ಧ್ರುವ MatShape& _shape, ಪೂರ್ಣಾಙ್ಕ _type, UMatUsageFlags _usageFlags)
{
    ಯದಿ (_shape.dims < 0) {
        release();
    } ಅನ್ಯಥಾ {
        create(_shape.dims, _shape.p, _type, _usageFlags);
        size.layout = _shape.layout;
        size.C = _shape.C;
    }
}

ಶೂನ್ಯ UMat::fit(ಪೂರ್ಣಾಙ್ಕ _dims, ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ* _sizes, ಪೂರ್ಣಾಙ್ಕ _type, UMatUsageFlags _usageFlags)
{
    ಯದಿ (_usageFlags == cv::USAGE_DEFAULT)
        _usageFlags = usageFlags;
    size_t oldTotalBytes = u ? u->size : 0;
    size_t esz = CV_ELEM_SIZE(_type), newTotal = _dims >= 0;
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < _dims; i++)
        newTotal *= _sizes[i];
    size_t newTotalBytes = newTotal*esz;
    ಯದಿ (newTotalBytes > 0 &&
        (!isContinuous() ||
         newTotalBytes > oldTotalBytes ||
         offset != 0 ||
         _usageFlags != usageFlags)) {
        create(_dims, _sizes, _type, _usageFlags);
    } ಅನ್ಯಥಾ {
        flags = (flags & ~Mat::TYPE_MASK) | CV_MAT_TYPE(_type);
        ಪೂರ್ಣಾಙ್ಕ _dummy_size = 0;
        setSize(*ಏತತ್, (_dims >= 0 ? _dims : 1), (_dims >= 0 ? _sizes : &_dummy_size), nullptr, ಸತ್ಯ);
        finalizeHdr(*ಏತತ್);
    }
}

ಶೂನ್ಯ UMat::fit(ಧ್ರುವ std::vector<ಪೂರ್ಣಾಙ್ಕ>& _shape, ಪೂರ್ಣಾಙ್ಕ _type, UMatUsageFlags _usageFlags)
{
    fit((ಪೂರ್ಣಾಙ್ಕ)_shape.size(), _shape.data(), _type, _usageFlags);
}

ಶೂನ್ಯ UMat::fit(ಧ್ರುವ MatShape& _shape, ಪೂರ್ಣಾಙ್ಕ _type, UMatUsageFlags _usageFlags)
{
    fit(_shape.dims, _shape.p, _type, _usageFlags);
    size.layout = _shape.layout;
    size.C = _shape.C;
}

ಶೂನ್ಯ UMat::fit(ಪೂರ್ಣಾಙ್ಕ _rows, ಪೂರ್ಣಾಙ್ಕ _cols, ಪೂರ್ಣಾಙ್ಕ _type, UMatUsageFlags _usageFlags)
{
    _type &= TYPE_MASK;
    ಪೂರ್ಣಾಙ್ಕ sz[] = {_rows, _cols};
    fit(2, sz, _type, _usageFlags);
}

ಶೂನ್ಯ UMat::fit(Size _sz, ಪೂರ್ಣಾಙ್ಕ _type, UMatUsageFlags _usageFlags)
{
    fit(_sz.height, _sz.width, _type, _usageFlags);
}

ಶೂನ್ಯ UMat::fitSameSize(ಒಳಹರಿವು_Array m, ಪೂರ್ಣಾಙ್ಕ _type, UMatUsageFlags _usageFlags)
{
    fit(m.shape(), _type, _usageFlags);
}

ಶೂನ್ಯ UMat::copySize(ಧ್ರುವ UMat& m)
{
    dims = m.dims;
    cols = m.cols;
    rows = m.rows;
    size = m.size;
    step = m.step;
}

UMat::~UMat()
{
    release();
}

ಶೂನ್ಯ UMat::deallocate()
{
    UMatData* u_ = u;
    u = ರಿಕ್ತ;
    u_->currAllocator->deallocate(u_);
}


UMat::UMat(ಧ್ರುವ UMat& m, ಧ್ರುವ Range& _rowRange, ಧ್ರುವ Range& _colRange)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(USAGE_DEFAULT), u(0), offset(0)
{
    CV_Assert( m.dims >= 2 && m.dims <= MatShape::MAX_DIMS );
    ಯದಿ( m.dims > 2 )
    {
        Range rs[MatShape::MAX_DIMS];
        rs[0] = _rowRange;
        rs[1] = _colRange;
        ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ i = 2; i < m.dims; i++ )
            rs[i] = Range::all();
        *ಏತತ್ = m(rs);
        ಪ್ರತ್ಯಾಗಮ;
    }

    *ಏತತ್ = m;
    ಯದಿ( _rowRange != Range::all() && _rowRange != Range(0,rows) )
    {
        CV_Assert( 0 <= _rowRange.start &&
                  _rowRange.start <= _rowRange.end &&
                  _rowRange.end <= m.rows );
        size[0] = rows = _rowRange.size();
        offset += step*_rowRange.start;
        flags |= SUBMATRIX_FLAG;
    }

    ಯದಿ( _colRange != Range::all() && _colRange != Range(0,cols) )
    {
        CV_Assert( 0 <= _colRange.start &&
                  _colRange.start <= _colRange.end &&
                  _colRange.end <= m.cols );
        size[1] = cols = _colRange.size();
        offset += _colRange.start*elemSize();
        flags |= SUBMATRIX_FLAG;
    }

    updateContinuityFlag();

    ಯದಿ( rows <= 0 || cols <= 0 )
    {
        release();
        rows = cols = 0;
    }
}


UMat::UMat(ಧ್ರುವ UMat& m, ಧ್ರುವ Rect& roi)
    : flags(m.flags), dims(2), rows(roi.height), cols(roi.width),
    allocator(m.allocator), usageFlags(m.usageFlags), u(m.u),
    offset(m.offset + roi.y*m.step[0]), size(2)
{
    CV_Assert( m.dims <= 2 );

    size_t esz = CV_ELEM_SIZE(flags);
    offset += roi.x*esz;
    CV_Assert( 0 <= roi.x && 0 <= roi.width && roi.x + roi.width <= m.cols &&
              0 <= roi.y && 0 <= roi.height && roi.y + roi.height <= m.rows );
    ಯದಿ( roi.width < m.cols || roi.height < m.rows )
        flags |= SUBMATRIX_FLAG;

    size[0] = rows;
    size[1] = cols;
    step[0] = m.dims == 2 ? m.step[0] : cols*esz;
    step[1] = esz;

    updateContinuityFlag();

    addref();
    ಯದಿ( rows <= 0 || cols <= 0 )
    {
        rows = cols = 0;
        release();
    }
}


UMat::UMat(ಧ್ರುವ UMat& m, ಧ್ರುವ Range* ranges)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(USAGE_DEFAULT), u(0), offset(0)
{
    ಪೂರ್ಣಾಙ್ಕ i, d = m.dims;

    CV_Assert(ranges);
    ಪ್ರತಿ( i = 0; i < d; i++ )
    {
        Range r = ranges[i];
        CV_Assert( r == Range::all() || (0 <= r.start && r.start < r.end && r.end <= m.size[i]) );
    }
    *ಏತತ್ = m;
    ಪ್ರತಿ( i = 0; i < d; i++ )
    {
        Range r = ranges[i];
        ಯದಿ( r != Range::all() && r != Range(0, size.p[i]))
        {
            size.p[i] = r.end - r.start;
            offset += r.start*step.p[i];
            flags |= SUBMATRIX_FLAG;
        }
    }

    ಯದಿ (d <= 2) {
        rows = d == 2 ? size[0] : 1;
        cols = d <= 0 ? (d >= 0) : size[d > 1];
    }
    updateContinuityFlag();
}

UMat::UMat(ಧ್ರುವ UMat& m, ಧ್ರುವ std::vector<Range>& ranges)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(USAGE_DEFAULT), u(0), offset(0)
{
    ಪೂರ್ಣಾಙ್ಕ i, d = m.dims;

    CV_Assert((ಪೂರ್ಣಾಙ್ಕ)ranges.size() == d);
    ಪ್ರತಿ (i = 0; i < d; i++)
    {
        Range r = ranges[i];
        CV_Assert(r == Range::all() || (0 <= r.start && r.start < r.end && r.end <= m.size[i]));
    }
    *ಏತತ್ = m;
    ಪ್ರತಿ (i = 0; i < d; i++)
    {
        Range r = ranges[i];
        ಯದಿ (r != Range::all() && r != Range(0, size.p[i]))
        {
            size.p[i] = r.end - r.start;
            offset += r.start*step.p[i];
            flags |= SUBMATRIX_FLAG;
        }
    }

    ಯದಿ (d <= 2) {
        rows = d == 2 ? size[0] : 1;
        cols = d <= 0 ? (d >= 0) : size[d > 1];
    }
    updateContinuityFlag();
}

UMat UMat::diag(ಪೂರ್ಣಾಙ್ಕ d) ಧ್ರುವ
{
    CV_Assert( dims <= 2 );
    UMat m = *ಏತತ್;
    size_t esz = elemSize();
    ಪೂರ್ಣಾಙ್ಕ len;

    ಯದಿ( d >= 0 )
    {
        len = std::min(cols - d, rows);
        m.offset += esz*d;
    }
    ಅನ್ಯಥಾ
    {
        len = std::min(rows + d, cols);
        m.offset -= step[0]*d;
    }
    CV_DbgAssert( len > 0 );

    m.size[0] = m.rows = len;
    m.size[1] = m.cols = 1;
    m.step[0] += (len > 1 ? esz : 0);

    m.updateContinuityFlag();

    ಯದಿ( size() != Size(1,1) )
        m.flags |= SUBMATRIX_FLAG;

    ಪ್ರತ್ಯಾಗಮ m;
}

ಶೂನ್ಯ UMat::locateROI( Size& wholeSize, Point& ofs ) ಧ್ರುವ
{
    CV_Assert( dims <= 2 && step[0] > 0 );
    size_t esz = elemSize(), minstep;
    ptrdiff_t delta1 = (ptrdiff_t)offset, delta2 = (ptrdiff_t)u->size;

    ಯದಿ( delta1 == 0 )
        ofs.x = ofs.y = 0;
    ಅನ್ಯಥಾ
    {
        ofs.y = (ಪೂರ್ಣಾಙ್ಕ)(delta1/step[0]);
        ofs.x = (ಪೂರ್ಣಾಙ್ಕ)((delta1 - step[0]*ofs.y)/esz);
        CV_DbgAssert( offset == (size_t)(ofs.y*step[0] + ofs.x*esz) );
    }
    minstep = (ofs.x + cols)*esz;
    wholeSize.height = (ಪೂರ್ಣಾಙ್ಕ)((delta2 - minstep)/step[0] + 1);
    wholeSize.height = std::max(wholeSize.height, ofs.y + rows);
    wholeSize.width = (ಪೂರ್ಣಾಙ್ಕ)((delta2 - step*(wholeSize.height-1))/esz);
    wholeSize.width = std::max(wholeSize.width, ofs.x + cols);
}


UMat& UMat::adjustROI( ಪೂರ್ಣಾಙ್ಕ dtop, ಪೂರ್ಣಾಙ್ಕ dbottom, ಪೂರ್ಣಾಙ್ಕ dleft, ಪೂರ್ಣಾಙ್ಕ dright )
{
    CV_Assert( dims == 2 && step[0] > 0 );
    Size wholeSize; Point ofs;
    size_t esz = elemSize();
    locateROI( wholeSize, ofs );
    ಪೂರ್ಣಾಙ್ಕ row1 = std::min(std::max(ofs.y - dtop, 0), wholeSize.height), row2 = std::max(0, std::min(ofs.y + rows + dbottom, wholeSize.height));
    ಪೂರ್ಣಾಙ್ಕ col1 = std::min(std::max(ofs.x - dleft, 0), wholeSize.width), col2 = std::max(0, std::min(ofs.x + cols + dright, wholeSize.width));
    ಯದಿ(row1 > row2)
        std::swap(row1, row2);
    ಯದಿ(col1 > col2)
        std::swap(col1, col2);

    offset += (row1 - ofs.y)*step + (col1 - ofs.x)*esz;
    rows = row2 - row1; cols = col2 - col1;
    size.p[0] = rows; size.p[1] = cols;
    updateContinuityFlag();
    ಪ್ರತ್ಯಾಗಮ *ಏತತ್;
}


UMat UMat::reshape(ಪೂರ್ಣಾಙ್ಕ new_cn, ಪೂರ್ಣಾಙ್ಕ new_rows) ಧ್ರುವ
{
    ಪೂರ್ಣಾಙ್ಕ cn = channels();
    UMat hdr = *ಏತತ್;

    ಯದಿ( dims > 2 && new_rows == 0 && new_cn != 0 && size[dims-1]*cn % new_cn == 0 )
    {
        hdr.flags = (hdr.flags & ~CV_MAT_CN_MASK) | ((new_cn-1) << CV_CN_SHIFT);
        hdr.step[dims-1] = CV_ELEM_SIZE(hdr.flags);
        hdr.size[dims-1] = hdr.size[dims-1]*cn / new_cn;
        ಪ್ರತ್ಯಾಗಮ hdr;
    }

    CV_Assert( dims <= 2 );

    ಯದಿ( new_cn == 0 )
        new_cn = cn;

    ಪೂರ್ಣಾಙ್ಕ total_width = cols * cn;

    ಯದಿ( (new_cn > total_width || total_width % new_cn != 0) && new_rows == 0 )
        new_rows = rows * total_width / new_cn;

    ಯದಿ( new_rows != 0 && new_rows != rows )
    {
        ಪೂರ್ಣಾಙ್ಕ total_size = total_width * rows;
        ಯದಿ( !isContinuous() )
            CV_Error( cv::Error::BadStep,
            "The matrix is not continuous, thus its number of rows can not be changed" );

        total_width = total_size / new_rows;

        ಯದಿ( total_width * new_rows != total_size )
            CV_Error( cv::Error::StsBadArg, "The total number of matrix elements "
                                    "is not divisible by the new number of rows" );

        hdr.size[0] = hdr.rows = new_rows;
        hdr.step[0] = total_width * elemSize1();
    } ಅನ್ಯಥಾ {
        hdr.size[0] = hdr.rows = rows;
        ಯದಿ (dims <= 1)
            hdr.step[0] = cols * CV_ELEM_SIZE(flags);
    }

    ಪೂರ್ಣಾಙ್ಕ new_width = total_width / new_cn;

    ಯದಿ( new_width * new_cn != total_width )
        CV_Error( cv::Error::Bad_ಅಂಕೆ_Channels,
        "The total width is not divisible by the new number of channels" );

    hdr.size.dims = hdr.dims = 2;
    hdr.size[1] = hdr.cols = new_width;
    hdr.flags = (hdr.flags & ~CV_MAT_CN_MASK) | ((new_cn-1) << CV_CN_SHIFT);
    hdr.step[1] = CV_ELEM_SIZE(hdr.flags);
    ಪ್ರತ್ಯಾಗಮ hdr;
}

#ifdef HAVE_OPENCL
ನಾಮಾಕಾಶ {
ಸ್ಥಿರ ತಾರ್ಕಿಕ ocl_setDiag(ಧ್ರುವ UMat& d, UMat& m, ಪೂರ್ಣಾಙ್ಕ len)
{
    ಪೂರ್ಣಾಙ್ಕ cn = d.channels();
    ಪೂರ್ಣಾಙ್ಕ depth = d.depth();

    ಯದಿ (depth == CV_64F && !ocl::Device::getDefault().doubleFPConfig())
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    String opts = format("-D SET_DIAG -D T1=%s -D cn=%d -D IS_ROW_VECTOR=%d",
                         ocl::memopTypeToStr(depth),
                         cn,
                         (d.rows == 1) ? 1 : 0);

    ocl::Kernel k("setDiag", ocl::core::copyset_oclsrc, opts);
    ಯದಿ (k.empty())
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    k.args(ocl::KernelArg::WriteOnly(m),
           ocl::KernelArg::ReadOnlyNoSize(d),
           len);

    size_t globalsize[2] = { (size_t)len, (size_t)len };
    ಪ್ರತ್ಯಾಗಮ k.run(2, globalsize, ರಿಕ್ತ, ಅಸತ್ಯ);
}
}
#endif

UMat UMat::diag(ಧ್ರುವ UMat& d, UMatUsageFlags usageFlags)
{
    CV_INSTRUMENT_REGION();

    CV_Assert(d.cols == 1 || d.rows == 1);
    ಪೂರ್ಣಾಙ್ಕ len = d.rows + d.cols - 1;

#ifdef HAVE_OPENCL
    ಯದಿ (ocl::useOpenCL())
    {
        UMat m(len, len, d.type(), usageFlags);
        ಯದಿ (ocl_setDiag(d, m, len))
        {
            CV_IMPL_ADD(CV_IMPL_OCL);
            ಪ್ರತ್ಯಾಗಮ m;
        }
    }
#endif

    UMat m(len, len, d.type(), Scalar(0), usageFlags);
    UMat md = m.diag();
    ಯದಿ( d.cols == 1 )
        d.copyTo(md);
    ಅನ್ಯಥಾ
        transpose(d, md);
    ಪ್ರತ್ಯಾಗಮ m;
}

ಪೂರ್ಣಾಙ್ಕ UMat::checkVector(ಪೂರ್ಣಾಙ್ಕ _elemChannels, ಪೂರ್ಣಾಙ್ಕ _depth, ತಾರ್ಕಿಕ _requireContinuous) ಧ್ರುವ
{
    ಪ್ರತ್ಯಾಗಮ (depth() == _depth || _depth <= 0) &&
        (isContinuous() || !_requireContinuous) &&
        ((dims <= 2 && (((rows == 1 || cols == 1) && channels() == _elemChannels) ||
                        (cols == _elemChannels && channels() == 1))) ||
        (dims == 3 && channels() == 1 && size.p[2] == _elemChannels && (size.p[0] == 1 || size.p[1] == 1) &&
         (isContinuous() || step.p[1] == step.p[2]*size.p[2])))
    ? (ಪೂರ್ಣಾಙ್ಕ)(total()*channels()/_elemChannels) : -1;
}

UMat UMat::reshape(ಪೂರ್ಣಾಙ್ಕ _cn, ಪೂರ್ಣಾಙ್ಕ _newndims, ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ* _newsz) ಧ್ರುವ
{
    ಯದಿ(_newndims == dims)
    {
        ಯದಿ(_newsz == 0)
            ಪ್ರತ್ಯಾಗಮ reshape(_cn);
        ಯದಿ(_newndims == 2)
            ಪ್ರತ್ಯಾಗಮ reshape(_cn, _newsz[0]);
    }

    ಯದಿ (isContinuous())
    {
        CV_Assert(_cn >= 0 && _newndims >= 0 && _newndims <= CV_MAX_DIM && (_newndims == 0 || _newsz != 0));

        ಯದಿ (_cn == 0)
            _cn = ಏತತ್->channels();
        ಅನ್ಯಥಾ
            CV_Assert(_cn <= CV_CN_MAX);

        size_t total_elem1_ref = ಏತತ್->total() * ಏತತ್->channels();
        size_t total_elem1 = _cn;

        AutoBuffer<ಪೂರ್ಣಾಙ್ಕ, 4> newsz_buf( (size_t)std::max(_newndims, 1) );

        ಪೂರ್ಣಾಙ್ಕ m1_idx = -1;

        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < _newndims; i++)
        {
            ಯದಿ (_newsz[i] >= 0) {
                ಯದಿ (_newsz[i] == 0 && i < dims)
                    newsz_buf[i] = size.p[i];
                ಅನ್ಯಥಾ
                    newsz_buf[i] = _newsz[i];
                total_elem1 *= (size_t)newsz_buf[i];
            } ಅನ್ಯಥಾ {
                ಯದಿ (m1_idx >= 0)
                    CV_Error(cv::Error::StsBadSize, "More than one '-1' occured in the new shape");
                m1_idx = i;
            }
        }

        ಯದಿ (m1_idx >= 0) {
            ಯದಿ (total_elem1 == 0) {
                CV_Assert(total_elem1_ref == 0);
                total_elem1 = 1;
            }
            CV_Assert(total_elem1_ref % total_elem1 == 0);
            newsz_buf[m1_idx] = (ಪೂರ್ಣಾಙ್ಕ)(total_elem1_ref / total_elem1);
        } ಅನ್ಯಥಾ ಯದಿ (total_elem1 != total_elem1_ref) {
            CV_Error(cv::Error::StsUnmatchedSizes, "Requested and source matrices have different count of elements");
        }

        UMat hdr = *ಏತತ್;
        hdr.flags = (hdr.flags & ~CV_MAT_CN_MASK) | ((_cn-1) << CV_CN_SHIFT);
        setSize(hdr, _newndims, newsz_buf.data(), ರಿಕ್ತ, ಸತ್ಯ);

        ಪ್ರತ್ಯಾಗಮ hdr;
    }

    CV_Error(cv::Error::StsNotImplemented, "Reshaping of n-dimensional non-continuous matrices is not supported yet");
}

UMat UMat::reshape(ಪೂರ್ಣಾಙ್ಕ _cn, ಧ್ರುವ MatShape& _newshape) ಧ್ರುವ
{
    ಯದಿ (_newshape.dims < 0) {
        ಪೂರ್ಣಾಙ್ಕ newshape[] = {0};
        ಪ್ರತ್ಯಾಗಮ reshape(_cn, 1, newshape);
    }
    ಪ್ರತ್ಯಾಗಮ reshape(_cn, _newshape.dims, _newshape.p);
}

Mat UMat::getMat(AccessFlag accessFlags) ಧ್ರುವ
{
    ಯದಿ(!u)
        ಪ್ರತ್ಯಾಗಮ Mat();
    // TODO Support ACCESS_READ (ACCESS_WRITE) without unnecessary data transfers
    accessFlags |= ACCESS_RW;
    UMatDataAutoLock autolock(u);
    ಯತ್ನ
    {
        ಯದಿ(CV_XADD(&u->refcount, 1) == 0)
            u->currAllocator->map(u, accessFlags);
        ಯದಿ (u->data != 0)
        {
            Mat hdr(dims, size.p, type(), u->data + offset, step.p);
            hdr.flags = flags;
            hdr.u = u;
            hdr.datastart = u->data;
            hdr.data = u->data + offset;
            hdr.datalimit = hdr.dataend = u->data + u->size;
            ಪ್ರತ್ಯಾಗಮ hdr;
        }
    }
    ಗೃಹಾಣ(...)
    {
        CV_XADD(&u->refcount, -1);
        ಕ್ಷಿಪ;
    }
    CV_XADD(&u->refcount, -1);
    CV_Assert(u->data != 0 && "Error mapping of UMat to host memory.");
    ಪ್ರತ್ಯಾಗಮ Mat();
}

ಶೂನ್ಯ* UMat::handle(AccessFlag accessFlags) ಧ್ರುವ
{
    ಯದಿ( !u )
        ಪ್ರತ್ಯಾಗಮ 0;

    CV_Assert(u->refcount == 0);
    CV_Assert(!u->deviceCopyObsolete() || u->copyOnMap());
    ಯದಿ (u->deviceCopyObsolete())
    {
        u->currAllocator->unmap(u);
    }

    ಯದಿ (!!(accessFlags & ACCESS_WRITE))
        u->markHostCopyObsolete(ಸತ್ಯ);

    ಪ್ರತ್ಯಾಗಮ u->handle;
}

ಶೂನ್ಯ UMat::ndoffset(size_t* ofs) ಧ್ರುವ
{
    // offset = step[0]*ofs[0] + step[1]*ofs[1] + step[2]*ofs[2] + ...;
    size_t val = offset;
    ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ i = 0; i < dims; i++ )
    {
        size_t s = step.p[i];
        ofs[i] = val / s;
        val -= ofs[i]*s;
    }
}

ಶೂನ್ಯ UMat::copyTo(OutputArray _dst) ಧ್ರುವ
{
    CV_INSTRUMENT_REGION();

#ifdef HAVE_CUDA
    ಯದಿ (_dst.isGpuMat())
    {
        _dst.getGpuMat().upload(*ಏತತ್);
        ಪ್ರತ್ಯಾಗಮ;
    }
#endif

    ಪೂರ್ಣಾಙ್ಕ stype = type();
    ಪೂರ್ಣಾಙ್ಕ dtype = _dst.type();
    ಯದಿ( _dst.fixedType() && dtype != stype )
    {
        CV_Assert( CV_MAT_CN(stype) == CV_MAT_CN(dtype) );
        convertTo( _dst, dtype );
        ಪ್ರತ್ಯಾಗಮ;
    }

    ಯದಿ( dims == 0 && empty() )
    {
        _dst.release();
        ಶೂನ್ಯ* obj = _dst.getObj();
        ಯದಿ (_dst.isMat())
            reinterpret_cast<Mat*>(obj)->flags = Mat::MAGIC_VAL | Mat::CONTINUOUS_FLAG | stype;
        ಅನ್ಯಥಾ ಯದಿ (_dst.isUMat())
            reinterpret_cast<UMat*>(obj)->flags = UMat::MAGIC_VAL | UMat::CONTINUOUS_FLAG | stype;
        ಅನ್ಯಥಾ ಯದಿ (_dst.isGpuMat())
            reinterpret_cast<cuda::GpuMat*>(obj)->flags = stype;
        ಪ್ರತ್ಯಾಗಮ;
    }

    _dst.create( dims, size.p, stype );
    ಯದಿ (empty())
        ಪ್ರತ್ಯಾಗಮ;

    size_t sz[CV_MAX_DIM] = {1}, srcofs[CV_MAX_DIM]={0}, dstofs[CV_MAX_DIM]={0};
    size_t esz = CV_ELEM_SIZE(stype);
    ಪೂರ್ಣಾಙ್ಕ i, d = std::max(dims, 1);
    ಪ್ರತಿ( i = 0; i < d; i++ )
        sz[i] = size.p[i];
    sz[d-1] *= esz;
    ndoffset(srcofs);
    srcofs[d-1] *= esz;

    ಯದಿ( _dst.isUMat() )
    {
        UMat dst = _dst.getUMat();
        CV_Assert(dst.u);
        ಯದಿ( u == dst.u && dst.offset == offset )
            ಪ್ರತ್ಯಾಗಮ;

        ಯದಿ (u->currAllocator == dst.u->currAllocator)
        {
            dst.ndoffset(dstofs);
            dstofs[dims-1] *= esz;
            u->currAllocator->copy(u, dst.u, d, sz, srcofs, step.p, dstofs, dst.step.p, ಅಸತ್ಯ);
            ಪ್ರತ್ಯಾಗಮ;
        }

        // different allocators: never map a device destination, upload into it instead
        // (through a host Mat when both sides are device memory)
        MatAllocator* hostAllocator = Mat::getDefaultAllocator();
        ಯದಿ (dst.u->currAllocator != hostAllocator)
        {
            ಯದಿ (u->currAllocator == hostAllocator)
                getMat(ACCESS_READ).copyTo(_dst);
            ಅನ್ಯಥಾ
            {
                Mat tmp;
                copyTo(tmp);
                tmp.copyTo(_dst);
            }
            ಪ್ರತ್ಯಾಗಮ;
        }
    }

    Mat dst = _dst.getMat();
    u->currAllocator->download(u, dst.ptr(), d, sz, srcofs, step.p, dst.step.p);
}

ಶೂನ್ಯ UMat::copyTo(OutputArray _dst, ಒಳಹರಿವು_Array _mask) ಧ್ರುವ
{
    CV_INSTRUMENT_REGION();

    ಯದಿ( _mask.empty() )
    {
        copyTo(_dst);
        ಪ್ರತ್ಯಾಗಮ;
    }
#ifdef HAVE_OPENCL
    ಪೂರ್ಣಾಙ್ಕ cn = channels(), mtype = _mask.type(), mdepth = CV_MAT_DEPTH(mtype), mcn = CV_MAT_CN(mtype);
    CV_Assert( (mdepth == CV_8U || mdepth == CV_8S || mdepth == CV_Bool) && (mcn == 1 || mcn == cn) );

    ಯದಿ (ocl::useOpenCL() && _dst.isUMat() && dims <= 2)
    {
        UMatData * prevu = _dst.getUMat().u;
        _dst.create( size, type() );

        UMat dst = _dst.getUMat();

        ತಾರ್ಕಿಕ haveDstUninit = ಅಸತ್ಯ;
        ಯದಿ( prevu != dst.u ) // do not leave dst uninitialized
            haveDstUninit = ಸತ್ಯ;

        String opts = format("-D COPY_TO_MASK -D T1=%s -D scn=%d -D mcn=%d%s",
                             ocl::memopTypeToStr(depth()), cn, mcn,
                             haveDstUninit ? " -D HAVE_DST_UNINIT" : "");

        ocl::Kernel k("copyToMask", ocl::core::copyset_oclsrc, opts);
        ಯದಿ (!k.empty())
        {
            k.args(ocl::KernelArg::ReadOnlyNoSize(*ಏತತ್),
                   ocl::KernelArg::ReadOnlyNoSize(_mask.getUMat()),
                   haveDstUninit ? ocl::KernelArg::WriteOnly(dst) :
                                   ocl::KernelArg::ReadWrite(dst));

            size_t globalsize[2] = { (size_t)cols, (size_t)rows };
            ಯದಿ (k.run(2, globalsize, ರಿಕ್ತ, ಅಸತ್ಯ))
            {
                CV_IMPL_ADD(CV_IMPL_OCL);
                ಪ್ರತ್ಯಾಗಮ;
            }
        }
    }
#endif
    Mat src = getMat(ACCESS_READ);
    src.copyTo(_dst, _mask);
}


//
// void UMat::convertTo moved to convert.dispatch.cpp
//

UMat& UMat::setTo(ಒಳಹರಿವು_Array ಮೌಲ್ಯ, ಒಳಹರಿವು_Array _mask)
{
    CV_INSTRUMENT_REGION();

    ತಾರ್ಕಿಕ haveMask = !_mask.empty();
#ifdef HAVE_OPENCL
    ಪೂರ್ಣಾಙ್ಕ tp = type(), cn = CV_MAT_CN(tp), d = CV_MAT_DEPTH(tp);

    ಯದಿ( dims <= 2 && cn <= 4 && CV_MAT_DEPTH(tp) < CV_64F && ocl::useOpenCL() )
    {
        Mat value = ಮೌಲ್ಯ.getMat();
        CV_Assert( checkScalar(value, type(), ಮೌಲ್ಯ.kind(), ಒಳಹರಿವು_Array_2::UMAT) );
        ಪೂರ್ಣಾಙ್ಕ kercn = haveMask || cn == 3 ? cn : std::max(cn, ocl::predictOptimalVectorWidth(*ಏತತ್)),
                kertp = CV_MAKE_TYPE(d, kercn);

        ದ್ವಿಗುಣ buf[16] = { 0, 0, 0, 0, 0, 0, 0, 0,
                           0, 0, 0, 0, 0, 0, 0, 0 };
        convertAndUnrollScalar(value, tp, (uchar *)buf, kercn / cn);

        ಪೂರ್ಣಾಙ್ಕ scalarcn = kercn == 3 ? 4 : kercn, rowsPerWI = ocl::Device::getDefault().isIntel() ? 4 : 1;
        String opts = format("-D dstT=%s -D rowsPerWI=%d -D dstST=%s -D dstT1=%s -D cn=%d",
                             ocl::memopTypeToStr(kertp), rowsPerWI,
                             ocl::memopTypeToStr(CV_MAKETYPE(d, scalarcn)),
                             ocl::memopTypeToStr(d), kercn);

        ocl::Kernel setK(haveMask ? "setMask" : "set", ocl::core::copyset_oclsrc, opts);
        ಯದಿ( !setK.empty() )
        {
            ocl::KernelArg scalararg(ocl::KernelArg::CONSTANT, 0, 0, 0, buf, CV_ELEM_SIZE(d) * scalarcn);
            UMat mask;

            ಯದಿ( haveMask )
            {
                mask = _mask.getUMat();
                CV_Assert( mask.size() == size() && (mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool) );
                ocl::KernelArg maskarg = ocl::KernelArg::ReadOnlyNoSize(mask),
                        dstarg = ocl::KernelArg::ReadWrite(*ಏತತ್);
                setK.args(maskarg, dstarg, scalararg);
            }
            ಅನ್ಯಥಾ
            {
                ocl::KernelArg dstarg = ocl::KernelArg::WriteOnly(*ಏತತ್, cn, kercn);
                setK.args(dstarg, scalararg);
            }

            size_t globalsize[] = { (size_t)cols * cn / kercn, ((size_t)rows + rowsPerWI - 1) / rowsPerWI };
            ಯದಿ( setK.run(2, globalsize, ರಿಕ್ತ, ಅಸತ್ಯ) )
            {
                CV_IMPL_ADD(CV_IMPL_OCL);
                ಪ್ರತ್ಯಾಗಮ *ಏತತ್;
            }
        }
    }
#endif
    Mat m = getMat(haveMask ? ACCESS_RW : ACCESS_WRITE);
    m.setTo(ಮೌಲ್ಯ, _mask);
    ಪ್ರತ್ಯಾಗಮ *ಏತತ್;
}

UMat& UMat::operator = (ಧ್ರುವ Scalar& s)
{
    setTo(s);
    ಪ್ರತ್ಯಾಗಮ *ಏತತ್;
}

UMat UMat::t() ಧ್ರುವ
{
    UMat m;
    transpose(*ಏತತ್, m);
    ಪ್ರತ್ಯಾಗಮ m;
}

UMat UMat::zeros(ಪೂರ್ಣಾಙ್ಕ rows, ಪೂರ್ಣಾಙ್ಕ cols, ಪೂರ್ಣಾಙ್ಕ type, UMatUsageFlags usageFlags)
{
    ಪ್ರತ್ಯಾಗಮ UMat(rows, cols, type, Scalar::all(0), usageFlags);
}

UMat UMat::zeros(Size size, ಪೂರ್ಣಾಙ್ಕ type, UMatUsageFlags usageFlags)
{
    ಪ್ರತ್ಯಾಗಮ UMat(size, type, Scalar::all(0), usageFlags);
}

UMat UMat::zeros(ಪೂರ್ಣಾಙ್ಕ ndims, ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ* sz, ಪೂರ್ಣಾಙ್ಕ type, UMatUsageFlags usageFlags)
{
    ಪ್ರತ್ಯಾಗಮ UMat(ndims, sz, type, Scalar::all(0), usageFlags);
}

UMat UMat::zeros(ಧ್ರುವ MatShape& shape, ಪೂರ್ಣಾಙ್ಕ type, UMatUsageFlags usageFlags)
{
    ಪ್ರತ್ಯಾಗಮ UMat(shape.dims, shape.p, type, Scalar::all(0), usageFlags);
}

UMat UMat::ones(ಪೂರ್ಣಾಙ್ಕ rows, ಪೂರ್ಣಾಙ್ಕ cols, ಪೂರ್ಣಾಙ್ಕ type, UMatUsageFlags usageFlags)
{
    ಪ್ರತ್ಯಾಗಮ UMat(rows, cols, type, Scalar(1), usageFlags);
}

UMat UMat::ones(Size size, ಪೂರ್ಣಾಙ್ಕ type, UMatUsageFlags usageFlags)
{
    ಪ್ರತ್ಯಾಗಮ UMat(size, type, Scalar(1), usageFlags);
}

UMat UMat::ones(ಪೂರ್ಣಾಙ್ಕ ndims, ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ* sz, ಪೂರ್ಣಾಙ್ಕ type, UMatUsageFlags usageFlags)
{
    ಪ್ರತ್ಯಾಗಮ UMat(ndims, sz, type, Scalar(1), usageFlags);
}

UMat UMat::ones(ಧ್ರುವ MatShape& shape, ಪೂರ್ಣಾಙ್ಕ type, UMatUsageFlags usageFlags)
{
    ಪ್ರತ್ಯಾಗಮ UMat(shape.dims, shape.p, type, Scalar(1), usageFlags);
}

}

/* End of file. */
