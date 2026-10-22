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

#include "precomp.hpp"
#include "opencl_kernels_core.hpp"
#include "umatrix.hpp"

#include <opencv2/core/utils/tls.hpp>

///////////////////////////////// UMat implementation ///////////////////////////////

namespace cv {

// forward decls, implementation is below in this file
void setSize(UMat& m, ចំនួនគត់ _dims, const ចំនួនគត់* _sz, const size_t* _steps,
             bool autoSteps = false);

void updateContinuityFlag(UMat& m);
void finalizeHdr(UMat& m);

UMatData::UMatData(const MatAllocator* allocator)
{
    prevAllocator = currAllocator = allocator;
    urefcount = refcount = mapcount = 0;
    data = origdata = 0;
    size = 0;
    flags = static_cast<UMatData::MemoryFlag>(0);
    handle = 0;
    userdata = 0;
    allocatorFlags_ = 0;
    originalUMatData = NULL;
}

UMatData::~UMatData()
{
    prevAllocator = currAllocator = 0;
    urefcount = refcount = 0;
    CV_Assert(mapcount == 0);
    data = origdata = 0;
    size = 0;
    bool isAsyncCleanup = !!(flags & UMatData::ASYNC_CLEANUP);
    flags = static_cast<UMatData::MemoryFlag>(0);
    handle = 0;
    userdata = 0;
    allocatorFlags_ = 0;
    ប្រសិនបើ (originalUMatData)
    {
        bool showWarn = false;
        UMatData* u = originalUMatData;
        bool zero_Ref = CV_XADD(&(u->refcount), -1) == 1;
        ប្រសិនបើ (zero_Ref)
        {
            // simulate Mat::deallocate
            ប្រសិនបើ (u->mapcount != 0)
            {
                (u->currAllocator ? u->currAllocator : /* TODO allocator ? allocator :*/ Mat::getDefaultAllocator())->unmap(u);
            }
            បើមិនដូច្នេះ
            {
                // we don't do "map", so we can't do "unmap"
            }
        }
        bool zero_URef = CV_XADD(&(u->urefcount), -1) == 1;
        ប្រសិនបើ (zero_Ref && !zero_URef)
            showWarn = true;
        ប្រសិនបើ (zero_Ref && zero_URef) // oops, we need to free resources
        {
            showWarn = !isAsyncCleanup;
            // simulate UMat::deallocate
            u->currAllocator->deallocate(u);
        }
#ifndef NDEBUG
        ប្រសិនបើ (showWarn)
        {
            static ចំនួនគត់ warn_message_showed = 0;
            ប្រសិនបើ (warn_message_showed++ < 100)
            {
                fflush(stdout);
                fprintf(stderr, "\n! OPENCV warning: getUMat()/getMat() call chain possible problem."
                                "\n!                 Base object is dead, while nested/derived object is still alive or processed."
                                "\n!                 Please check lifetime of UMat/Mat objects!\n");
                fflush(stderr);
            }
        }
#បើមិនដូច្នេះ
        CV_UNUSED(showWarn);
#endif
        originalUMatData = NULL;
    }
}

#ifndef OPENCV_DISABLE_THREAD_SUPPORT

// it should be a prime number for the best hash function
ga_nanaa { UMAT_NLOCKS = 31 };
static Mutex umatLocks[UMAT_NLOCKS];

static size_t getUMatDataLockIndex(const UMatData* u)
{
    size_t idx = ((size_t)(void*)u) % UMAT_NLOCKS;
    ត្រឡប់ idx;
}

void UMatData::lock()
{
    size_t idx = getUMatDataLockIndex(this);
    //printf("%d lock(%d)\n", cv::utils::getThreadID(), (int)idx);
    umatLocks[idx].lock();
}

void UMatData::unlock()
{
    size_t idx = getUMatDataLockIndex(this);
    //printf("%d unlock(%d)\n", cv::utils::getThreadID(), (int)idx);
    umatLocks[idx].unlock();
}


// Do not allow several lock() calls with different UMatData objects.
sa_nracanaa UMatDataAutoLocker
{
    ចំនួនគត់ usage_count;
    UMatData* locked_objects[2];
    UMatDataAutoLocker() : usage_count(0) { locked_objects[0] = NULL; locked_objects[1] = NULL; }

    void lock(UMatData*& u1)
    {
        bool locked_1 = (u1 == locked_objects[0] || u1 == locked_objects[1]);
        ប្រសិនបើ (locked_1)
        {
            u1 = NULL;
            ត្រឡប់;
        }
        CV_Assert(usage_count == 0);  // UMatDataAutoLock can't be used multiple times from the same thread
        usage_count = 1;
        locked_objects[0] = u1;
        u1->lock();
    }
    void lock(UMatData*& u1, UMatData*& u2)
    {
        bool locked_1 = (u1 == locked_objects[0] || u1 == locked_objects[1]);
        bool locked_2 = (u2 == locked_objects[0] || u2 == locked_objects[1]);
        ប្រសិនបើ (locked_1)
            u1 = NULL;
        ប្រសិនបើ (locked_2)
            u2 = NULL;
        ប្រសិនបើ (locked_1 && locked_2)
            ត្រឡប់;
        CV_Assert(usage_count == 0);  // UMatDataAutoLock can't be used multiple times from the same thread
        usage_count = 1;
        locked_objects[0] = u1;
        locked_objects[1] = u2;
        ប្រសិនបើ (u1)
            u1->lock();
        ប្រសិនបើ (u2)
            u2->lock();
    }
    void release(UMatData* u1, UMatData* u2)
    {
        ប្រសិនបើ (u1 == NULL && u2 == NULL)
            ត្រឡប់;
        CV_Assert(usage_count == 1);
        usage_count = 0;
        ប្រសិនបើ (u1)
            u1->unlock();
        ប្រសិនបើ (u2)
            u2->unlock();
        locked_objects[0] = NULL; locked_objects[1] = NULL;
    }
};
static TLSData<UMatDataAutoLocker>& getUMatDataAutoLockerTLS()
{
    CV_SINGLETON_LAZY_INIT_REF(TLSData<UMatDataAutoLocker>, new TLSData<UMatDataAutoLocker>());
}
static UMatDataAutoLocker& getUMatDataAutoLocker() { ត្រឡប់ getUMatDataAutoLockerTLS().getRef(); }


UMatDataAutoLock::UMatDataAutoLock(UMatData* u) : u1(u), u2(NULL)
{
    getUMatDataAutoLocker().lock(u1);
}
UMatDataAutoLock::UMatDataAutoLock(UMatData* u1_, UMatData* u2_) : u1(u1_), u2(u2_)
{
    ប្រសិនបើ (getUMatDataLockIndex(u1) > getUMatDataLockIndex(u2))
    {
        std::swap(u1, u2);
    }
    getUMatDataAutoLocker().lock(u1, u2);
}
UMatDataAutoLock::~UMatDataAutoLock()
{
    getUMatDataAutoLocker().release(u1, u2);
}

#បើមិនដូច្នេះ

void UMatData::lock()
{
    // nothing in OPENCV_DISABLE_THREAD_SUPPORT mode
}

void UMatData::unlock()
{
    // nothing in OPENCV_DISABLE_THREAD_SUPPORT mode
}

UMatDataAutoLock::UMatDataAutoLock(UMatData* u) : u1(u), u2(NULL)
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

UMat::UMat(ចំនួនគត់ _rows, ចំនួនគត់ _cols, ចំនួនគត់ _type, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_rows, _cols, _type);
}

UMat::UMat(ចំនួនគត់ _rows, ចំនួនគត់ _cols, ចំនួនគត់ _type, const Scalar& _s, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_rows, _cols, _type);
    *this = _s;
}

UMat::UMat(Size _sz, ចំនួនគត់ _type, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create( _sz.height, _sz.width, _type );
}

UMat::UMat(Size _sz, ចំនួនគត់ _type, const Scalar& _s, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_sz.height, _sz.width, _type);
    *this = _s;
}

UMat::UMat(ចំនួនគត់ _dims, const ចំនួនគត់* _sz, ចំនួនគត់ _type, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_dims, _sz, _type);
}

UMat::UMat(ចំនួនគត់ _dims, const ចំនួនគត់* _sz, ចំនួនគត់ _type, const Scalar& _s, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_dims, _sz, _type);
    *this = _s;
}

UMat::UMat(const MatShape& _shape, ចំនួនគត់ _type, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_shape.dims, _shape.p, _type);
}

UMat::UMat(const MatShape& _shape, ចំនួនគត់ _type, const Scalar& _s, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_shape.dims, _shape.p, _type);
    *this = _s;
}

UMat::UMat(const UMat& m)
: flags(m.flags), dims(m.dims), rows(m.rows), cols(m.cols), allocator(m.allocator),
  usageFlags(m.usageFlags), u(m.u), offset(m.offset), size(m.size), step(m.step)
{
    addref();
}

UMat& UMat::operator=(const UMat& m)
{
    ប្រសិនបើ( this != &m )
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
    ត្រឡប់ *this;
}

UMat UMat::clone() const
{
    UMat m;
    copyTo(m);
    ត្រឡប់ m;
}

void UMat::assignTo(UMat& m, ចំនួនគត់ _type) const
{
    ប្រសិនបើ( _type < 0 )
        m = *this;
    បើមិនដូច្នេះ
        convertTo(m, _type);
}

void UMat::create(ចំនួនគត់ _rows, ចំនួនគត់ _cols, ចំនួនគត់ _type, UMatUsageFlags _usageFlags)
{
    ចំនួនគត់ sz[] = {_rows, _cols};
    create(2, sz, _type, _usageFlags);
}

void UMat::create(Size _sz, ចំនួនគត់ _type, UMatUsageFlags _usageFlags)
{
    create(_sz.height, _sz.width, _type, _usageFlags);
}

void UMat::createSameSize(InputArray arr, ចំនួនគត់ _type, UMatUsageFlags _usageFlags)
{
    create(arr.shape(), _type, _usageFlags);
}

void UMat::addref()
{
    ប្រសិនបើ( u )
        CV_XADD(&(u->urefcount), 1);
}

void UMat::release()
{
    ប្រសិនបើ( u && CV_XADD(&(u->urefcount), -1) == 1 )
        deallocate();
    u = 0;
    size.clear();
    dims = cols = rows = 0;
}

bool UMat::empty() const
{
    ត្រឡប់ u == 0 || total() == 0;
}

size_t UMat::total() const
{
    ប្រសិនបើ( dims <= 2 )
        ត្រឡប់ (size_t)rows * cols;
    size_t p = 1;
    សម្រាប់( ចំនួនគត់ i = 0; i < dims; i++ )
        p *= size[i];
    ត្រឡប់ p;
}

MatShape UMat::shape() const
{
    ត្រឡប់ size;
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
    m.allocator = NULL;
    m.u = NULL;
    m.offset = 0;
}

UMat& UMat::operator=(UMat&& m)
{
    ប្រសិនបើ (this == &m)
      ត្រឡប់ *this;
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
    m.allocator = NULL;
    m.u = NULL;
    m.offset = 0;
    ត្រឡប់ *this;
}


MatAllocator* UMat::getStdAllocator()
{
#ifdef HAVE_OPENCL
    ប្រសិនបើ (ocl::useOpenCL())
        ត្រឡប់ ocl::getOpenCLAllocator();
#endif
    ត្រឡប់ Mat::getDefaultAllocator();
}

void swap( UMat& a, UMat& b )
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


void setSize( UMat& m, ចំនួនគត់ _dims, const ចំនួនគត់* _sz,
              const size_t* _steps, bool autoSteps )
{
    CV_Assert( 0 <= _dims && _dims <= CV_MAX_DIM && _dims <= MatShape::MAX_DIMS);

    m.dims = _dims;
    m.size = MatShape(_dims, _sz);
    m.step[std::max(_dims-1, 0)] = CV_ELEM_SIZE(m.flags);
    សម្រាប់ (ចំនួនគត់ i = _dims-2; i >= 0; i--) {
        size_t autostep = m.size[i+1]*m.step[i+1];
        ប្រសិនបើ (_steps) {
            m.step[i] = _steps[i];
            //CV_Assert(m.step[i] >= autostep);
        } បើមិនដូច្នេះ ប្រសិនបើ (autoSteps) {
            m.step[i] = autostep;
        } បើមិនដូច្នេះ {
            m.step[i] = 0;
        }
    }

    ប្រសិនបើ( _dims <= 2 )
    {
        m.cols = _dims == 0 ? 1 : _sz ? _sz[_dims > 1] : 0;
        m.rows = _dims < 2 ? 1 : _sz ? _sz[0] : 0;
    } បើមិនដូច្នេះ {
        m.cols = m.rows = -1;
    }
}


void UMat::updateContinuityFlag()
{
    flags = cv::updateContinuityFlag(flags, dims, size.p, step.p);
}


void finalizeHdr(UMat& m)
{
    m.updateContinuityFlag();
    ចំនួនគត់ d = m.dims;
    ប្រសិនបើ( d > 2 )
        m.rows = m.cols = -1;
}


UMat Mat::getUMat(AccessFlag accessFlags, UMatUsageFlags usageFlags) const
{
    UMat hdr;
    ប្រសិនបើ(!data)
        ត្រឡប់ hdr;
    ប្រសិនបើ (data != datastart)
    {
        Size wholeSize;
        Point ofs;
        locateROI(wholeSize, ofs);
        Size sz(cols, rows);
        ប្រសិនបើ (ofs.x != 0 || ofs.y != 0)
        {
            Mat src = *this;
            ចំនួនគត់ dtop = ofs.y;
            ចំនួនគត់ dbottom = wholeSize.height - src.rows - ofs.y;
            ចំនួនគត់ dleft = ofs.x;
            ចំនួនគត់ dright = wholeSize.width - src.cols - ofs.x;
            src.adjustROI(dtop, dbottom, dleft, dright);
            ត្រឡប់ src.getUMat(accessFlags, usageFlags)(cv::Rect(ofs.x, ofs.y, sz.width, sz.height));
        }
    }
    CV_Assert(data == datastart);

    accessFlags |= ACCESS_RW;
    UMatData* new_u = NULL;
    MatStep new_step = step;
    {
        MatAllocator *a = allocator, *a0 = getDefaultAllocator();
        ប្រសិនបើ(!a)
            a = a0;
        new_u = a->allocate(dims, size.p, type(), data, new_step.p, accessFlags, usageFlags);
        new_u->originalUMatData = u;
    }
    bool allocated = false;
    try
    {
        allocated = UMat::getStdAllocator()->allocate(new_u, accessFlags, usageFlags);
    }
    gr_haa_na (const cv::Exception& e)
    {
        fprintf(stderr, "Exception: %s\n", e.what());
    }
    ប្រសិនបើ (!allocated)
    {
        allocated = getDefaultAllocator()->allocate(new_u, accessFlags, usageFlags);
        CV_Assert(allocated);
    }
    ប្រសិនបើ (u != NULL)
    {
#ifdef HAVE_OPENCL
        ប្រសិនបើ (ocl::useOpenCL() && new_u->currAllocator == ocl::getOpenCLAllocator())
        {
            CV_Assert(new_u->tempUMat());
        }
#endif
        CV_XADD(&(u->refcount), 1);
        CV_XADD(&(u->urefcount), 1);
    }
    try
    {
        hdr.flags = flags;
        hdr.usageFlags = usageFlags;
        setSize(hdr, dims, size.p, new_step.p);
        finalizeHdr(hdr);
        hdr.u = new_u;
        hdr.offset = 0; //data - datastart;
        hdr.addref();
        ត្រឡប់ hdr;
    }
    gr_haa_na(...)
    {
        ប្រសិនបើ (u != NULL)
        {
            CV_XADD(&(u->refcount), -1);
            CV_XADD(&(u->urefcount), -1);
        }
        new_u->currAllocator->deallocate(new_u);
        k_sipa;
    }

}

void UMat::create(ចំនួនគត់ d0, const ចំនួនគត់* _sizes, ចំនួនគត់ _type, UMatUsageFlags _usageFlags)
{
    ចំនួនគត់ sz1 = 1, d = d0;
    ចំនួនគត់ i;
    ប្រសិនបើ (d == 0) {
        d = 1;
        _sizes = (const ចំនួនគត់*)&sz1;
    }
    CV_Assert(0 <= d && d <= CV_MAX_DIM && _sizes);
    _type = CV_MAT_TYPE(_type);

    // if param value is USAGE_DEFAULT by implicit default param value -or- explicit value
    // ...then don't change the existing usageFlags
    // it is not possible to change usage from non-default to USAGE_DEFAULT through create()
    // ...instead must construct UMat()
    ប្រសិនបើ (_usageFlags == cv::USAGE_DEFAULT)
    {
        _usageFlags = usageFlags;
    }

    ប្រសិនបើ( u && d == dims && _type == type() && _usageFlags == usageFlags )
    {
        សម្រាប់( i = 0; i < d; i++ )
            ប្រសិនបើ( size[i] != _sizes[i] )
                bha_nga;
        ប្រសិនបើ( i == d )
            ត្រឡប់;
    }

    ចំនួនគត់ _sizes_backup[CV_MAX_DIM]; // #5991
    ប្រសិនបើ (_sizes == (this->size.p))
    {
        សម្រាប់(i = 0; i < d; i++ )
            _sizes_backup[i] = _sizes[i];
        _sizes = _sizes_backup;
    }

    release();
    usageFlags = _usageFlags;
    ប្រសិនបើ( d == 0 )
        ត្រឡប់;
    flags = (_type & CV_MAT_TYPE_MASK) | MAGIC_VAL;
    setSize(*this, d, _sizes, 0, true);
    offset = 0;

    ប្រសិនបើ( total() > 0 )
    {
        MatAllocator *a = allocator, *a0 = getStdAllocator();
        ប្រសិនបើ (!a)
        {
            a = a0;
            a0 = Mat::getDefaultAllocator();
        }
        try
        {
            u = a->allocate(dims, size.p, _type, 0, step.p, ACCESS_RW /* ignored */, usageFlags);
            CV_Assert(u != 0);
        }
        gr_haa_na(...)
        {
            ប្រសិនបើ(a != a0)
                u = a0->allocate(dims, size.p, _type, 0, step.p, ACCESS_RW /* ignored */, usageFlags);
            CV_Assert(u != 0);
        }
        CV_Assert( step[dims-1] == (size_t)CV_ELEM_SIZE(flags) );
    }

    finalizeHdr(*this);
    addref();
    size.dims = dims = d0;
}

void UMat::create(const std::vector<ចំនួនគត់>& _sizes, ចំនួនគត់ _type, UMatUsageFlags _usageFlags)
{
    create((ចំនួនគត់)_sizes.size(), _sizes.data(), _type, _usageFlags);
}

void UMat::create(const MatShape& _shape, ចំនួនគត់ _type, UMatUsageFlags _usageFlags)
{
    ប្រសិនបើ (_shape.dims < 0) {
        release();
    } បើមិនដូច្នេះ {
        create(_shape.dims, _shape.p, _type, _usageFlags);
        size.layout = _shape.layout;
        size.C = _shape.C;
    }
}

void UMat::fit(ចំនួនគត់ _dims, const ចំនួនគត់* _sizes, ចំនួនគត់ _type, UMatUsageFlags _usageFlags)
{
    ប្រសិនបើ (_usageFlags == cv::USAGE_DEFAULT)
        _usageFlags = usageFlags;
    size_t oldTotalBytes = u ? u->size : 0;
    size_t esz = CV_ELEM_SIZE(_type), newTotal = _dims >= 0;
    សម្រាប់ (ចំនួនគត់ i = 0; i < _dims; i++)
        newTotal *= _sizes[i];
    size_t newTotalBytes = newTotal*esz;
    ប្រសិនបើ (newTotalBytes > 0 &&
        (!isContinuous() ||
         newTotalBytes > oldTotalBytes ||
         offset != 0 ||
         _usageFlags != usageFlags)) {
        create(_dims, _sizes, _type, _usageFlags);
    } បើមិនដូច្នេះ {
        flags = (flags & ~Mat::TYPE_MASK) | CV_MAT_TYPE(_type);
        ចំនួនគត់ _dummy_size = 0;
        setSize(*this, (_dims >= 0 ? _dims : 1), (_dims >= 0 ? _sizes : &_dummy_size), nullptr, true);
        finalizeHdr(*this);
    }
}

void UMat::fit(const std::vector<ចំនួនគត់>& _shape, ចំនួនគត់ _type, UMatUsageFlags _usageFlags)
{
    fit((ចំនួនគត់)_shape.size(), _shape.data(), _type, _usageFlags);
}

void UMat::fit(const MatShape& _shape, ចំនួនគត់ _type, UMatUsageFlags _usageFlags)
{
    fit(_shape.dims, _shape.p, _type, _usageFlags);
    size.layout = _shape.layout;
    size.C = _shape.C;
}

void UMat::fit(ចំនួនគត់ _rows, ចំនួនគត់ _cols, ចំនួនគត់ _type, UMatUsageFlags _usageFlags)
{
    _type &= TYPE_MASK;
    ចំនួនគត់ sz[] = {_rows, _cols};
    fit(2, sz, _type, _usageFlags);
}

void UMat::fit(Size _sz, ចំនួនគត់ _type, UMatUsageFlags _usageFlags)
{
    fit(_sz.height, _sz.width, _type, _usageFlags);
}

void UMat::fitSameSize(InputArray m, ចំនួនគត់ _type, UMatUsageFlags _usageFlags)
{
    fit(m.shape(), _type, _usageFlags);
}

void UMat::copySize(const UMat& m)
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

void UMat::deallocate()
{
    UMatData* u_ = u;
    u = NULL;
    u_->currAllocator->deallocate(u_);
}


UMat::UMat(const UMat& m, const Range& _rowRange, const Range& _colRange)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(USAGE_DEFAULT), u(0), offset(0)
{
    CV_Assert( m.dims >= 2 && m.dims <= MatShape::MAX_DIMS );
    ប្រសិនបើ( m.dims > 2 )
    {
        Range rs[MatShape::MAX_DIMS];
        rs[0] = _rowRange;
        rs[1] = _colRange;
        សម្រាប់( ចំនួនគត់ i = 2; i < m.dims; i++ )
            rs[i] = Range::all();
        *this = m(rs);
        ត្រឡប់;
    }

    *this = m;
    ប្រសិនបើ( _rowRange != Range::all() && _rowRange != Range(0,rows) )
    {
        CV_Assert( 0 <= _rowRange.start &&
                  _rowRange.start <= _rowRange.end &&
                  _rowRange.end <= m.rows );
        size[0] = rows = _rowRange.size();
        offset += step*_rowRange.start;
        flags |= SUBMATRIX_FLAG;
    }

    ប្រសិនបើ( _colRange != Range::all() && _colRange != Range(0,cols) )
    {
        CV_Assert( 0 <= _colRange.start &&
                  _colRange.start <= _colRange.end &&
                  _colRange.end <= m.cols );
        size[1] = cols = _colRange.size();
        offset += _colRange.start*elemSize();
        flags |= SUBMATRIX_FLAG;
    }

    updateContinuityFlag();

    ប្រសិនបើ( rows <= 0 || cols <= 0 )
    {
        release();
        rows = cols = 0;
    }
}


UMat::UMat(const UMat& m, const Rect& roi)
    : flags(m.flags), dims(2), rows(roi.height), cols(roi.width),
    allocator(m.allocator), usageFlags(m.usageFlags), u(m.u),
    offset(m.offset + roi.y*m.step[0]), size(2)
{
    CV_Assert( m.dims <= 2 );

    size_t esz = CV_ELEM_SIZE(flags);
    offset += roi.x*esz;
    CV_Assert( 0 <= roi.x && 0 <= roi.width && roi.x + roi.width <= m.cols &&
              0 <= roi.y && 0 <= roi.height && roi.y + roi.height <= m.rows );
    ប្រសិនបើ( roi.width < m.cols || roi.height < m.rows )
        flags |= SUBMATRIX_FLAG;

    size[0] = rows;
    size[1] = cols;
    step[0] = m.dims == 2 ? m.step[0] : cols*esz;
    step[1] = esz;

    updateContinuityFlag();

    addref();
    ប្រសិនបើ( rows <= 0 || cols <= 0 )
    {
        rows = cols = 0;
        release();
    }
}


UMat::UMat(const UMat& m, const Range* ranges)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(USAGE_DEFAULT), u(0), offset(0)
{
    ចំនួនគត់ i, d = m.dims;

    CV_Assert(ranges);
    សម្រាប់( i = 0; i < d; i++ )
    {
        Range r = ranges[i];
        CV_Assert( r == Range::all() || (0 <= r.start && r.start < r.end && r.end <= m.size[i]) );
    }
    *this = m;
    សម្រាប់( i = 0; i < d; i++ )
    {
        Range r = ranges[i];
        ប្រសិនបើ( r != Range::all() && r != Range(0, size.p[i]))
        {
            size.p[i] = r.end - r.start;
            offset += r.start*step.p[i];
            flags |= SUBMATRIX_FLAG;
        }
    }

    ប្រសិនបើ (d <= 2) {
        rows = d == 2 ? size[0] : 1;
        cols = d <= 0 ? (d >= 0) : size[d > 1];
    }
    updateContinuityFlag();
}

UMat::UMat(const UMat& m, const std::vector<Range>& ranges)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(USAGE_DEFAULT), u(0), offset(0)
{
    ចំនួនគត់ i, d = m.dims;

    CV_Assert((ចំនួនគត់)ranges.size() == d);
    សម្រាប់ (i = 0; i < d; i++)
    {
        Range r = ranges[i];
        CV_Assert(r == Range::all() || (0 <= r.start && r.start < r.end && r.end <= m.size[i]));
    }
    *this = m;
    សម្រាប់ (i = 0; i < d; i++)
    {
        Range r = ranges[i];
        ប្រសិនបើ (r != Range::all() && r != Range(0, size.p[i]))
        {
            size.p[i] = r.end - r.start;
            offset += r.start*step.p[i];
            flags |= SUBMATRIX_FLAG;
        }
    }

    ប្រសិនបើ (d <= 2) {
        rows = d == 2 ? size[0] : 1;
        cols = d <= 0 ? (d >= 0) : size[d > 1];
    }
    updateContinuityFlag();
}

UMat UMat::diag(ចំនួនគត់ d) const
{
    CV_Assert( dims <= 2 );
    UMat m = *this;
    size_t esz = elemSize();
    ចំនួនគត់ len;

    ប្រសិនបើ( d >= 0 )
    {
        len = std::min(cols - d, rows);
        m.offset += esz*d;
    }
    បើមិនដូច្នេះ
    {
        len = std::min(rows + d, cols);
        m.offset -= step[0]*d;
    }
    CV_DbgAssert( len > 0 );

    m.size[0] = m.rows = len;
    m.size[1] = m.cols = 1;
    m.step[0] += (len > 1 ? esz : 0);

    m.updateContinuityFlag();

    ប្រសិនបើ( size() != Size(1,1) )
        m.flags |= SUBMATRIX_FLAG;

    ត្រឡប់ m;
}

void UMat::locateROI( Size& wholeSize, Point& ofs ) const
{
    CV_Assert( dims <= 2 && step[0] > 0 );
    size_t esz = elemSize(), minstep;
    ptrdiff_t delta1 = (ptrdiff_t)offset, delta2 = (ptrdiff_t)u->size;

    ប្រសិនបើ( delta1 == 0 )
        ofs.x = ofs.y = 0;
    បើមិនដូច្នេះ
    {
        ofs.y = (ចំនួនគត់)(delta1/step[0]);
        ofs.x = (ចំនួនគត់)((delta1 - step[0]*ofs.y)/esz);
        CV_DbgAssert( offset == (size_t)(ofs.y*step[0] + ofs.x*esz) );
    }
    minstep = (ofs.x + cols)*esz;
    wholeSize.height = (ចំនួនគត់)((delta2 - minstep)/step[0] + 1);
    wholeSize.height = std::max(wholeSize.height, ofs.y + rows);
    wholeSize.width = (ចំនួនគត់)((delta2 - step*(wholeSize.height-1))/esz);
    wholeSize.width = std::max(wholeSize.width, ofs.x + cols);
}


UMat& UMat::adjustROI( ចំនួនគត់ dtop, ចំនួនគត់ dbottom, ចំនួនគត់ dleft, ចំនួនគត់ dright )
{
    CV_Assert( dims == 2 && step[0] > 0 );
    Size wholeSize; Point ofs;
    size_t esz = elemSize();
    locateROI( wholeSize, ofs );
    ចំនួនគត់ row1 = std::min(std::max(ofs.y - dtop, 0), wholeSize.height), row2 = std::max(0, std::min(ofs.y + rows + dbottom, wholeSize.height));
    ចំនួនគត់ col1 = std::min(std::max(ofs.x - dleft, 0), wholeSize.width), col2 = std::max(0, std::min(ofs.x + cols + dright, wholeSize.width));
    ប្រសិនបើ(row1 > row2)
        std::swap(row1, row2);
    ប្រសិនបើ(col1 > col2)
        std::swap(col1, col2);

    offset += (row1 - ofs.y)*step + (col1 - ofs.x)*esz;
    rows = row2 - row1; cols = col2 - col1;
    size.p[0] = rows; size.p[1] = cols;
    updateContinuityFlag();
    ត្រឡប់ *this;
}


UMat UMat::reshape(ចំនួនគត់ new_cn, ចំនួនគត់ new_rows) const
{
    ចំនួនគត់ cn = channels();
    UMat hdr = *this;

    ប្រសិនបើ( dims > 2 && new_rows == 0 && new_cn != 0 && size[dims-1]*cn % new_cn == 0 )
    {
        hdr.flags = (hdr.flags & ~CV_MAT_CN_MASK) | ((new_cn-1) << CV_CN_SHIFT);
        hdr.step[dims-1] = CV_ELEM_SIZE(hdr.flags);
        hdr.size[dims-1] = hdr.size[dims-1]*cn / new_cn;
        ត្រឡប់ hdr;
    }

    CV_Assert( dims <= 2 );

    ប្រសិនបើ( new_cn == 0 )
        new_cn = cn;

    ចំនួនគត់ total_width = cols * cn;

    ប្រសិនបើ( (new_cn > total_width || total_width % new_cn != 0) && new_rows == 0 )
        new_rows = rows * total_width / new_cn;

    ប្រសិនបើ( new_rows != 0 && new_rows != rows )
    {
        ចំនួនគត់ total_size = total_width * rows;
        ប្រសិនបើ( !isContinuous() )
            CV_Error( cv::Error::BadStep,
            "The matrix is not continuous, thus its number of rows can not be changed" );

        total_width = total_size / new_rows;

        ប្រសិនបើ( total_width * new_rows != total_size )
            CV_Error( cv::Error::StsBadArg, "The total number of matrix elements "
                                    "is not divisible by the new number of rows" );

        hdr.size[0] = hdr.rows = new_rows;
        hdr.step[0] = total_width * elemSize1();
    } បើមិនដូច្នេះ {
        hdr.size[0] = hdr.rows = rows;
        ប្រសិនបើ (dims <= 1)
            hdr.step[0] = cols * CV_ELEM_SIZE(flags);
    }

    ចំនួនគត់ new_width = total_width / new_cn;

    ប្រសិនបើ( new_width * new_cn != total_width )
        CV_Error( cv::Error::BadNumChannels,
        "The total width is not divisible by the new number of channels" );

    hdr.size.dims = hdr.dims = 2;
    hdr.size[1] = hdr.cols = new_width;
    hdr.flags = (hdr.flags & ~CV_MAT_CN_MASK) | ((new_cn-1) << CV_CN_SHIFT);
    hdr.step[1] = CV_ELEM_SIZE(hdr.flags);
    ត្រឡប់ hdr;
}

#ifdef HAVE_OPENCL
namespace {
static bool ocl_setDiag(const UMat& d, UMat& m, ចំនួនគត់ len)
{
    ចំនួនគត់ cn = d.channels();
    ចំនួនគត់ depth = d.depth();

    ប្រសិនបើ (depth == CV_64F && !ocl::Device::getDefault().doubleFPConfig())
        ត្រឡប់ false;

    String opts = format("-D SET_DIAG -D T1=%s -D cn=%d -D IS_ROW_VECTOR=%d",
                         ocl::memopTypeToStr(depth),
                         cn,
                         (d.rows == 1) ? 1 : 0);

    ocl::Kernel k("setDiag", ocl::core::copyset_oclsrc, opts);
    ប្រសិនបើ (k.empty())
        ត្រឡប់ false;

    k.args(ocl::KernelArg::WriteOnly(m),
           ocl::KernelArg::ReadOnlyNoSize(d),
           len);

    size_t globalsize[2] = { (size_t)len, (size_t)len };
    ត្រឡប់ k.run(2, globalsize, NULL, false);
}
}
#endif

UMat UMat::diag(const UMat& d, UMatUsageFlags usageFlags)
{
    CV_INSTRUMENT_REGION();

    CV_Assert(d.cols == 1 || d.rows == 1);
    ចំនួនគត់ len = d.rows + d.cols - 1;

#ifdef HAVE_OPENCL
    ប្រសិនបើ (ocl::useOpenCL())
    {
        UMat m(len, len, d.type(), usageFlags);
        ប្រសិនបើ (ocl_setDiag(d, m, len))
        {
            CV_IMPL_ADD(CV_IMPL_OCL);
            ត្រឡប់ m;
        }
    }
#endif

    UMat m(len, len, d.type(), Scalar(0), usageFlags);
    UMat md = m.diag();
    ប្រសិនបើ( d.cols == 1 )
        d.copyTo(md);
    បើមិនដូច្នេះ
        transpose(d, md);
    ត្រឡប់ m;
}

ចំនួនគត់ UMat::checkVector(ចំនួនគត់ _elemChannels, ចំនួនគត់ _depth, bool _requireContinuous) const
{
    ត្រឡប់ (depth() == _depth || _depth <= 0) &&
        (isContinuous() || !_requireContinuous) &&
        ((dims <= 2 && (((rows == 1 || cols == 1) && channels() == _elemChannels) ||
                        (cols == _elemChannels && channels() == 1))) ||
        (dims == 3 && channels() == 1 && size.p[2] == _elemChannels && (size.p[0] == 1 || size.p[1] == 1) &&
         (isContinuous() || step.p[1] == step.p[2]*size.p[2])))
    ? (ចំនួនគត់)(total()*channels()/_elemChannels) : -1;
}

UMat UMat::reshape(ចំនួនគត់ _cn, ចំនួនគត់ _newndims, const ចំនួនគត់* _newsz) const
{
    ប្រសិនបើ(_newndims == dims)
    {
        ប្រសិនបើ(_newsz == 0)
            ត្រឡប់ reshape(_cn);
        ប្រសិនបើ(_newndims == 2)
            ត្រឡប់ reshape(_cn, _newsz[0]);
    }

    ប្រសិនបើ (isContinuous())
    {
        CV_Assert(_cn >= 0 && _newndims >= 0 && _newndims <= CV_MAX_DIM && (_newndims == 0 || _newsz != 0));

        ប្រសិនបើ (_cn == 0)
            _cn = this->channels();
        បើមិនដូច្នេះ
            CV_Assert(_cn <= CV_CN_MAX);

        size_t total_elem1_ref = this->total() * this->channels();
        size_t total_elem1 = _cn;

        AutoBuffer<ចំនួនគត់, 4> newsz_buf( (size_t)std::max(_newndims, 1) );

        ចំនួនគត់ m1_idx = -1;

        សម្រាប់ (ចំនួនគត់ i = 0; i < _newndims; i++)
        {
            ប្រសិនបើ (_newsz[i] >= 0) {
                ប្រសិនបើ (_newsz[i] == 0 && i < dims)
                    newsz_buf[i] = size.p[i];
                បើមិនដូច្នេះ
                    newsz_buf[i] = _newsz[i];
                total_elem1 *= (size_t)newsz_buf[i];
            } បើមិនដូច្នេះ {
                ប្រសិនបើ (m1_idx >= 0)
                    CV_Error(cv::Error::StsBadSize, "More than one '-1' occured in the new shape");
                m1_idx = i;
            }
        }

        ប្រសិនបើ (m1_idx >= 0) {
            ប្រសិនបើ (total_elem1 == 0) {
                CV_Assert(total_elem1_ref == 0);
                total_elem1 = 1;
            }
            CV_Assert(total_elem1_ref % total_elem1 == 0);
            newsz_buf[m1_idx] = (ចំនួនគត់)(total_elem1_ref / total_elem1);
        } បើមិនដូច្នេះ ប្រសិនបើ (total_elem1 != total_elem1_ref) {
            CV_Error(cv::Error::StsUnmatchedSizes, "Requested and source matrices have different count of elements");
        }

        UMat hdr = *this;
        hdr.flags = (hdr.flags & ~CV_MAT_CN_MASK) | ((_cn-1) << CV_CN_SHIFT);
        setSize(hdr, _newndims, newsz_buf.data(), NULL, true);

        ត្រឡប់ hdr;
    }

    CV_Error(cv::Error::StsNotImplemented, "Reshaping of n-dimensional non-continuous matrices is not supported yet");
}

UMat UMat::reshape(ចំនួនគត់ _cn, const MatShape& _newshape) const
{
    ប្រសិនបើ (_newshape.dims < 0) {
        ចំនួនគត់ newshape[] = {0};
        ត្រឡប់ reshape(_cn, 1, newshape);
    }
    ត្រឡប់ reshape(_cn, _newshape.dims, _newshape.p);
}

Mat UMat::getMat(AccessFlag accessFlags) const
{
    ប្រសិនបើ(!u)
        ត្រឡប់ Mat();
    // TODO Support ACCESS_READ (ACCESS_WRITE) without unnecessary data transfers
    accessFlags |= ACCESS_RW;
    UMatDataAutoLock autolock(u);
    try
    {
        ប្រសិនបើ(CV_XADD(&u->refcount, 1) == 0)
            u->currAllocator->map(u, accessFlags);
        ប្រសិនបើ (u->data != 0)
        {
            Mat hdr(dims, size.p, type(), u->data + offset, step.p);
            hdr.flags = flags;
            hdr.u = u;
            hdr.datastart = u->data;
            hdr.data = u->data + offset;
            hdr.datalimit = hdr.dataend = u->data + u->size;
            ត្រឡប់ hdr;
        }
    }
    gr_haa_na(...)
    {
        CV_XADD(&u->refcount, -1);
        k_sipa;
    }
    CV_XADD(&u->refcount, -1);
    CV_Assert(u->data != 0 && "Error mapping of UMat to host memory.");
    ត្រឡប់ Mat();
}

void* UMat::handle(AccessFlag accessFlags) const
{
    ប្រសិនបើ( !u )
        ត្រឡប់ 0;

    CV_Assert(u->refcount == 0);
    CV_Assert(!u->deviceCopyObsolete() || u->copyOnMap());
    ប្រសិនបើ (u->deviceCopyObsolete())
    {
        u->currAllocator->unmap(u);
    }

    ប្រសិនបើ (!!(accessFlags & ACCESS_WRITE))
        u->markHostCopyObsolete(true);

    ត្រឡប់ u->handle;
}

void UMat::ndoffset(size_t* ofs) const
{
    // offset = step[0]*ofs[0] + step[1]*ofs[1] + step[2]*ofs[2] + ...;
    size_t val = offset;
    សម្រាប់( ចំនួនគត់ i = 0; i < dims; i++ )
    {
        size_t s = step.p[i];
        ofs[i] = val / s;
        val -= ofs[i]*s;
    }
}

void UMat::copyTo(OutputArray _dst) const
{
    CV_INSTRUMENT_REGION();

#ifdef HAVE_CUDA
    ប្រសិនបើ (_dst.isGpuMat())
    {
        _dst.getGpuMat().upload(*this);
        ត្រឡប់;
    }
#endif

    ចំនួនគត់ stype = type();
    ចំនួនគត់ dtype = _dst.type();
    ប្រសិនបើ( _dst.fixedType() && dtype != stype )
    {
        CV_Assert( CV_MAT_CN(stype) == CV_MAT_CN(dtype) );
        convertTo( _dst, dtype );
        ត្រឡប់;
    }

    ប្រសិនបើ( dims == 0 && empty() )
    {
        _dst.release();
        void* obj = _dst.getObj();
        ប្រសិនបើ (_dst.isMat())
            reinterpret_cast<Mat*>(obj)->flags = Mat::MAGIC_VAL | Mat::CONTINUOUS_FLAG | stype;
        បើមិនដូច្នេះ ប្រសិនបើ (_dst.isUMat())
            reinterpret_cast<UMat*>(obj)->flags = UMat::MAGIC_VAL | UMat::CONTINUOUS_FLAG | stype;
        បើមិនដូច្នេះ ប្រសិនបើ (_dst.isGpuMat())
            reinterpret_cast<cuda::GpuMat*>(obj)->flags = stype;
        ត្រឡប់;
    }

    _dst.create( dims, size.p, stype );
    ប្រសិនបើ (empty())
        ត្រឡប់;

    size_t sz[CV_MAX_DIM] = {1}, srcofs[CV_MAX_DIM]={0}, dstofs[CV_MAX_DIM]={0};
    size_t esz = CV_ELEM_SIZE(stype);
    ចំនួនគត់ i, d = std::max(dims, 1);
    សម្រាប់( i = 0; i < d; i++ )
        sz[i] = size.p[i];
    sz[d-1] *= esz;
    ndoffset(srcofs);
    srcofs[d-1] *= esz;

    ប្រសិនបើ( _dst.isUMat() )
    {
        UMat dst = _dst.getUMat();
        CV_Assert(dst.u);
        ប្រសិនបើ( u == dst.u && dst.offset == offset )
            ត្រឡប់;

        ប្រសិនបើ (u->currAllocator == dst.u->currAllocator)
        {
            dst.ndoffset(dstofs);
            dstofs[dims-1] *= esz;
            u->currAllocator->copy(u, dst.u, d, sz, srcofs, step.p, dstofs, dst.step.p, false);
            ត្រឡប់;
        }

        // different allocators: never map a device destination, upload into it instead
        // (through a host Mat when both sides are device memory)
        MatAllocator* hostAllocator = Mat::getDefaultAllocator();
        ប្រសិនបើ (dst.u->currAllocator != hostAllocator)
        {
            ប្រសិនបើ (u->currAllocator == hostAllocator)
                getMat(ACCESS_READ).copyTo(_dst);
            បើមិនដូច្នេះ
            {
                Mat tmp;
                copyTo(tmp);
                tmp.copyTo(_dst);
            }
            ត្រឡប់;
        }
    }

    Mat dst = _dst.getMat();
    u->currAllocator->download(u, dst.ptr(), d, sz, srcofs, step.p, dst.step.p);
}

void UMat::copyTo(OutputArray _dst, InputArray _mask) const
{
    CV_INSTRUMENT_REGION();

    ប្រសិនបើ( _mask.empty() )
    {
        copyTo(_dst);
        ត្រឡប់;
    }
#ifdef HAVE_OPENCL
    ចំនួនគត់ cn = channels(), mtype = _mask.type(), mdepth = CV_MAT_DEPTH(mtype), mcn = CV_MAT_CN(mtype);
    CV_Assert( (mdepth == CV_8U || mdepth == CV_8S || mdepth == CV_Bool) && (mcn == 1 || mcn == cn) );

    ប្រសិនបើ (ocl::useOpenCL() && _dst.isUMat() && dims <= 2)
    {
        UMatData * prevu = _dst.getUMat().u;
        _dst.create( size, type() );

        UMat dst = _dst.getUMat();

        bool haveDstUninit = false;
        ប្រសិនបើ( prevu != dst.u ) // do not leave dst uninitialized
            haveDstUninit = true;

        String opts = format("-D COPY_TO_MASK -D T1=%s -D scn=%d -D mcn=%d%s",
                             ocl::memopTypeToStr(depth()), cn, mcn,
                             haveDstUninit ? " -D HAVE_DST_UNINIT" : "");

        ocl::Kernel k("copyToMask", ocl::core::copyset_oclsrc, opts);
        ប្រសិនបើ (!k.empty())
        {
            k.args(ocl::KernelArg::ReadOnlyNoSize(*this),
                   ocl::KernelArg::ReadOnlyNoSize(_mask.getUMat()),
                   haveDstUninit ? ocl::KernelArg::WriteOnly(dst) :
                                   ocl::KernelArg::ReadWrite(dst));

            size_t globalsize[2] = { (size_t)cols, (size_t)rows };
            ប្រសិនបើ (k.run(2, globalsize, NULL, false))
            {
                CV_IMPL_ADD(CV_IMPL_OCL);
                ត្រឡប់;
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

UMat& UMat::setTo(InputArray _value, InputArray _mask)
{
    CV_INSTRUMENT_REGION();

    bool haveMask = !_mask.empty();
#ifdef HAVE_OPENCL
    ចំនួនគត់ tp = type(), cn = CV_MAT_CN(tp), d = CV_MAT_DEPTH(tp);

    ប្រសិនបើ( dims <= 2 && cn <= 4 && CV_MAT_DEPTH(tp) < CV_64F && ocl::useOpenCL() )
    {
        Mat value = _value.getMat();
        CV_Assert( checkScalar(value, type(), _value.kind(), _InputArray::UMAT) );
        ចំនួនគត់ kercn = haveMask || cn == 3 ? cn : std::max(cn, ocl::predictOptimalVectorWidth(*this)),
                kertp = CV_MAKE_TYPE(d, kercn);

        dviguu_na buf[16] = { 0, 0, 0, 0, 0, 0, 0, 0,
                           0, 0, 0, 0, 0, 0, 0, 0 };
        convertAndUnrollScalar(value, tp, (uchar *)buf, kercn / cn);

        ចំនួនគត់ scalarcn = kercn == 3 ? 4 : kercn, rowsPerWI = ocl::Device::getDefault().isIntel() ? 4 : 1;
        String opts = format("-D dstT=%s -D rowsPerWI=%d -D dstST=%s -D dstT1=%s -D cn=%d",
                             ocl::memopTypeToStr(kertp), rowsPerWI,
                             ocl::memopTypeToStr(CV_MAKETYPE(d, scalarcn)),
                             ocl::memopTypeToStr(d), kercn);

        ocl::Kernel setK(haveMask ? "setMask" : "set", ocl::core::copyset_oclsrc, opts);
        ប្រសិនបើ( !setK.empty() )
        {
            ocl::KernelArg scalararg(ocl::KernelArg::CONSTANT, 0, 0, 0, buf, CV_ELEM_SIZE(d) * scalarcn);
            UMat mask;

            ប្រសិនបើ( haveMask )
            {
                mask = _mask.getUMat();
                CV_Assert( mask.size() == size() && (mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool) );
                ocl::KernelArg maskarg = ocl::KernelArg::ReadOnlyNoSize(mask),
                        dstarg = ocl::KernelArg::ReadWrite(*this);
                setK.args(maskarg, dstarg, scalararg);
            }
            បើមិនដូច្នេះ
            {
                ocl::KernelArg dstarg = ocl::KernelArg::WriteOnly(*this, cn, kercn);
                setK.args(dstarg, scalararg);
            }

            size_t globalsize[] = { (size_t)cols * cn / kercn, ((size_t)rows + rowsPerWI - 1) / rowsPerWI };
            ប្រសិនបើ( setK.run(2, globalsize, NULL, false) )
            {
                CV_IMPL_ADD(CV_IMPL_OCL);
                ត្រឡប់ *this;
            }
        }
    }
#endif
    Mat m = getMat(haveMask ? ACCESS_RW : ACCESS_WRITE);
    m.setTo(_value, _mask);
    ត្រឡប់ *this;
}

UMat& UMat::operator = (const Scalar& s)
{
    setTo(s);
    ត្រឡប់ *this;
}

UMat UMat::t() const
{
    UMat m;
    transpose(*this, m);
    ត្រឡប់ m;
}

UMat UMat::zeros(ចំនួនគត់ rows, ចំនួនគត់ cols, ចំនួនគត់ type, UMatUsageFlags usageFlags)
{
    ត្រឡប់ UMat(rows, cols, type, Scalar::all(0), usageFlags);
}

UMat UMat::zeros(Size size, ចំនួនគត់ type, UMatUsageFlags usageFlags)
{
    ត្រឡប់ UMat(size, type, Scalar::all(0), usageFlags);
}

UMat UMat::zeros(ចំនួនគត់ ndims, const ចំនួនគត់* sz, ចំនួនគត់ type, UMatUsageFlags usageFlags)
{
    ត្រឡប់ UMat(ndims, sz, type, Scalar::all(0), usageFlags);
}

UMat UMat::zeros(const MatShape& shape, ចំនួនគត់ type, UMatUsageFlags usageFlags)
{
    ត្រឡប់ UMat(shape.dims, shape.p, type, Scalar::all(0), usageFlags);
}

UMat UMat::ones(ចំនួនគត់ rows, ចំនួនគត់ cols, ចំនួនគត់ type, UMatUsageFlags usageFlags)
{
    ត្រឡប់ UMat(rows, cols, type, Scalar(1), usageFlags);
}

UMat UMat::ones(Size size, ចំនួនគត់ type, UMatUsageFlags usageFlags)
{
    ត្រឡប់ UMat(size, type, Scalar(1), usageFlags);
}

UMat UMat::ones(ចំនួនគត់ ndims, const ចំនួនគត់* sz, ចំនួនគត់ type, UMatUsageFlags usageFlags)
{
    ត្រឡប់ UMat(ndims, sz, type, Scalar(1), usageFlags);
}

UMat UMat::ones(const MatShape& shape, ចំនួនគត់ type, UMatUsageFlags usageFlags)
{
    ត្រឡប់ UMat(shape.dims, shape.p, type, Scalar(1), usageFlags);
}

}

/* End of file. */
