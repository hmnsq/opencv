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
void setSize(UMat& m, ကိန်းပြည့် _dims, const ကိန်းပြည့်* _sz, const size_t* _steps,
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
    အကယ်၍ (originalUMatData)
    {
        bool showWarn = false;
        UMatData* u = originalUMatData;
        bool zero_Ref = CV_XADD(&(u->refcount), -1) == 1;
        အကယ်၍ (zero_Ref)
        {
            // simulate Mat::deallocate
            အကယ်၍ (u->mapcount != 0)
            {
                (u->currAllocator ? u->currAllocator : /* TODO allocator ? allocator :*/ Mat::getDefaultAllocator())->unmap(u);
            }
            မဟုတ်ပါက
            {
                // we don't do "map", so we can't do "unmap"
            }
        }
        bool zero_URef = CV_XADD(&(u->urefcount), -1) == 1;
        အကယ်၍ (zero_Ref && !zero_URef)
            showWarn = true;
        အကယ်၍ (zero_Ref && zero_URef) // oops, we need to free resources
        {
            showWarn = !isAsyncCleanup;
            // simulate UMat::deallocate
            u->currAllocator->deallocate(u);
        }
#ifndef NDEBUG
        အကယ်၍ (showWarn)
        {
            static ကိန်းပြည့် warn_message_showed = 0;
            အကယ်၍ (warn_message_showed++ < 100)
            {
                fflush(stdout);
                fprintf(stderr, "\n! OPENCV warning: getUMat()/getMat() call chain possible problem."
                                "\n!                 Base object is dead, while nested/derived object is still alive or processed."
                                "\n!                 Please check lifetime of UMat/Mat objects!\n");
                fflush(stderr);
            }
        }
#မဟုတ်ပါက
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
    ပြန်ပေး idx;
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
    ကိန်းပြည့် usage_count;
    UMatData* locked_objects[2];
    UMatDataAutoLocker() : usage_count(0) { locked_objects[0] = NULL; locked_objects[1] = NULL; }

    void lock(UMatData*& u1)
    {
        bool locked_1 = (u1 == locked_objects[0] || u1 == locked_objects[1]);
        အကယ်၍ (locked_1)
        {
            u1 = NULL;
            ပြန်ပေး;
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
        အကယ်၍ (locked_1)
            u1 = NULL;
        အကယ်၍ (locked_2)
            u2 = NULL;
        အကယ်၍ (locked_1 && locked_2)
            ပြန်ပေး;
        CV_Assert(usage_count == 0);  // UMatDataAutoLock can't be used multiple times from the same thread
        usage_count = 1;
        locked_objects[0] = u1;
        locked_objects[1] = u2;
        အကယ်၍ (u1)
            u1->lock();
        အကယ်၍ (u2)
            u2->lock();
    }
    void release(UMatData* u1, UMatData* u2)
    {
        အကယ်၍ (u1 == NULL && u2 == NULL)
            ပြန်ပေး;
        CV_Assert(usage_count == 1);
        usage_count = 0;
        အကယ်၍ (u1)
            u1->unlock();
        အကယ်၍ (u2)
            u2->unlock();
        locked_objects[0] = NULL; locked_objects[1] = NULL;
    }
};
static TLSData<UMatDataAutoLocker>& getUMatDataAutoLockerTLS()
{
    CV_SINGLETON_LAZY_INIT_REF(TLSData<UMatDataAutoLocker>, new TLSData<UMatDataAutoLocker>());
}
static UMatDataAutoLocker& getUMatDataAutoLocker() { ပြန်ပေး getUMatDataAutoLockerTLS().getRef(); }


UMatDataAutoLock::UMatDataAutoLock(UMatData* u) : u1(u), u2(NULL)
{
    getUMatDataAutoLocker().lock(u1);
}
UMatDataAutoLock::UMatDataAutoLock(UMatData* u1_, UMatData* u2_) : u1(u1_), u2(u2_)
{
    အကယ်၍ (getUMatDataLockIndex(u1) > getUMatDataLockIndex(u2))
    {
        std::swap(u1, u2);
    }
    getUMatDataAutoLocker().lock(u1, u2);
}
UMatDataAutoLock::~UMatDataAutoLock()
{
    getUMatDataAutoLocker().release(u1, u2);
}

#မဟုတ်ပါက

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

UMat::UMat(ကိန်းပြည့် _rows, ကိန်းပြည့် _cols, ကိန်းပြည့် _type, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_rows, _cols, _type);
}

UMat::UMat(ကိန်းပြည့် _rows, ကိန်းပြည့် _cols, ကိန်းပြည့် _type, const Scalar& _s, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_rows, _cols, _type);
    *this = _s;
}

UMat::UMat(Size _sz, ကိန်းပြည့် _type, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create( _sz.height, _sz.width, _type );
}

UMat::UMat(Size _sz, ကိန်းပြည့် _type, const Scalar& _s, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_sz.height, _sz.width, _type);
    *this = _s;
}

UMat::UMat(ကိန်းပြည့် _dims, const ကိန်းပြည့်* _sz, ကိန်းပြည့် _type, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_dims, _sz, _type);
}

UMat::UMat(ကိန်းပြည့် _dims, const ကိန်းပြည့်* _sz, ကိန်းပြည့် _type, const Scalar& _s, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_dims, _sz, _type);
    *this = _s;
}

UMat::UMat(const MatShape& _shape, ကိန်းပြည့် _type, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_shape.dims, _shape.p, _type);
}

UMat::UMat(const MatShape& _shape, ကိန်းပြည့် _type, const Scalar& _s, UMatUsageFlags _usageFlags)
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
    အကယ်၍( this != &m )
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
    ပြန်ပေး *this;
}

UMat UMat::clone() const
{
    UMat m;
    copyTo(m);
    ပြန်ပေး m;
}

void UMat::assignTo(UMat& m, ကိန်းပြည့် _type) const
{
    အကယ်၍( _type < 0 )
        m = *this;
    မဟုတ်ပါက
        convertTo(m, _type);
}

void UMat::create(ကိန်းပြည့် _rows, ကိန်းပြည့် _cols, ကိန်းပြည့် _type, UMatUsageFlags _usageFlags)
{
    ကိန်းပြည့် sz[] = {_rows, _cols};
    create(2, sz, _type, _usageFlags);
}

void UMat::create(Size _sz, ကိန်းပြည့် _type, UMatUsageFlags _usageFlags)
{
    create(_sz.height, _sz.width, _type, _usageFlags);
}

void UMat::createSameSize(InputArray arr, ကိန်းပြည့် _type, UMatUsageFlags _usageFlags)
{
    create(arr.shape(), _type, _usageFlags);
}

void UMat::addref()
{
    အကယ်၍( u )
        CV_XADD(&(u->urefcount), 1);
}

void UMat::release()
{
    အကယ်၍( u && CV_XADD(&(u->urefcount), -1) == 1 )
        deallocate();
    u = 0;
    size.clear();
    dims = cols = rows = 0;
}

bool UMat::empty() const
{
    ပြန်ပေး u == 0 || total() == 0;
}

size_t UMat::total() const
{
    အကယ်၍( dims <= 2 )
        ပြန်ပေး (size_t)rows * cols;
    size_t p = 1;
    အတွက်( ကိန်းပြည့် i = 0; i < dims; i++ )
        p *= size[i];
    ပြန်ပေး p;
}

MatShape UMat::shape() const
{
    ပြန်ပေး size;
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
    အကယ်၍ (this == &m)
      ပြန်ပေး *this;
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
    ပြန်ပေး *this;
}


MatAllocator* UMat::getStdAllocator()
{
#ifdef HAVE_OPENCL
    အကယ်၍ (ocl::useOpenCL())
        ပြန်ပေး ocl::getOpenCLAllocator();
#endif
    ပြန်ပေး Mat::getDefaultAllocator();
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


void setSize( UMat& m, ကိန်းပြည့် _dims, const ကိန်းပြည့်* _sz,
              const size_t* _steps, bool autoSteps )
{
    CV_Assert( 0 <= _dims && _dims <= CV_MAX_DIM && _dims <= MatShape::MAX_DIMS);

    m.dims = _dims;
    m.size = MatShape(_dims, _sz);
    m.step[std::max(_dims-1, 0)] = CV_ELEM_SIZE(m.flags);
    အတွက် (ကိန်းပြည့် i = _dims-2; i >= 0; i--) {
        size_t autostep = m.size[i+1]*m.step[i+1];
        အကယ်၍ (_steps) {
            m.step[i] = _steps[i];
            //CV_Assert(m.step[i] >= autostep);
        } မဟုတ်ပါက အကယ်၍ (autoSteps) {
            m.step[i] = autostep;
        } မဟုတ်ပါက {
            m.step[i] = 0;
        }
    }

    အကယ်၍( _dims <= 2 )
    {
        m.cols = _dims == 0 ? 1 : _sz ? _sz[_dims > 1] : 0;
        m.rows = _dims < 2 ? 1 : _sz ? _sz[0] : 0;
    } မဟုတ်ပါက {
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
    ကိန်းပြည့် d = m.dims;
    အကယ်၍( d > 2 )
        m.rows = m.cols = -1;
}


UMat Mat::getUMat(AccessFlag accessFlags, UMatUsageFlags usageFlags) const
{
    UMat hdr;
    အကယ်၍(!data)
        ပြန်ပေး hdr;
    အကယ်၍ (data != datastart)
    {
        Size wholeSize;
        Point ofs;
        locateROI(wholeSize, ofs);
        Size sz(cols, rows);
        အကယ်၍ (ofs.x != 0 || ofs.y != 0)
        {
            Mat src = *this;
            ကိန်းပြည့် dtop = ofs.y;
            ကိန်းပြည့် dbottom = wholeSize.height - src.rows - ofs.y;
            ကိန်းပြည့် dleft = ofs.x;
            ကိန်းပြည့် dright = wholeSize.width - src.cols - ofs.x;
            src.adjustROI(dtop, dbottom, dleft, dright);
            ပြန်ပေး src.getUMat(accessFlags, usageFlags)(cv::Rect(ofs.x, ofs.y, sz.width, sz.height));
        }
    }
    CV_Assert(data == datastart);

    accessFlags |= ACCESS_RW;
    UMatData* new_u = NULL;
    MatStep new_step = step;
    {
        MatAllocator *a = allocator, *a0 = getDefaultAllocator();
        အကယ်၍(!a)
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
    အကယ်၍ (!allocated)
    {
        allocated = getDefaultAllocator()->allocate(new_u, accessFlags, usageFlags);
        CV_Assert(allocated);
    }
    အကယ်၍ (u != NULL)
    {
#ifdef HAVE_OPENCL
        အကယ်၍ (ocl::useOpenCL() && new_u->currAllocator == ocl::getOpenCLAllocator())
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
        ပြန်ပေး hdr;
    }
    gr_haa_na(...)
    {
        အကယ်၍ (u != NULL)
        {
            CV_XADD(&(u->refcount), -1);
            CV_XADD(&(u->urefcount), -1);
        }
        new_u->currAllocator->deallocate(new_u);
        k_sipa;
    }

}

void UMat::create(ကိန်းပြည့် d0, const ကိန်းပြည့်* _sizes, ကိန်းပြည့် _type, UMatUsageFlags _usageFlags)
{
    ကိန်းပြည့် sz1 = 1, d = d0;
    ကိန်းပြည့် i;
    အကယ်၍ (d == 0) {
        d = 1;
        _sizes = (const ကိန်းပြည့်*)&sz1;
    }
    CV_Assert(0 <= d && d <= CV_MAX_DIM && _sizes);
    _type = CV_MAT_TYPE(_type);

    // if param value is USAGE_DEFAULT by implicit default param value -or- explicit value
    // ...then don't change the existing usageFlags
    // it is not possible to change usage from non-default to USAGE_DEFAULT through create()
    // ...instead must construct UMat()
    အကယ်၍ (_usageFlags == cv::USAGE_DEFAULT)
    {
        _usageFlags = usageFlags;
    }

    အကယ်၍( u && d == dims && _type == type() && _usageFlags == usageFlags )
    {
        အတွက်( i = 0; i < d; i++ )
            အကယ်၍( size[i] != _sizes[i] )
                bha_nga;
        အကယ်၍( i == d )
            ပြန်ပေး;
    }

    ကိန်းပြည့် _sizes_backup[CV_MAX_DIM]; // #5991
    အကယ်၍ (_sizes == (this->size.p))
    {
        အတွက်(i = 0; i < d; i++ )
            _sizes_backup[i] = _sizes[i];
        _sizes = _sizes_backup;
    }

    release();
    usageFlags = _usageFlags;
    အကယ်၍( d == 0 )
        ပြန်ပေး;
    flags = (_type & CV_MAT_TYPE_MASK) | MAGIC_VAL;
    setSize(*this, d, _sizes, 0, true);
    offset = 0;

    အကယ်၍( total() > 0 )
    {
        MatAllocator *a = allocator, *a0 = getStdAllocator();
        အကယ်၍ (!a)
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
            အကယ်၍(a != a0)
                u = a0->allocate(dims, size.p, _type, 0, step.p, ACCESS_RW /* ignored */, usageFlags);
            CV_Assert(u != 0);
        }
        CV_Assert( step[dims-1] == (size_t)CV_ELEM_SIZE(flags) );
    }

    finalizeHdr(*this);
    addref();
    size.dims = dims = d0;
}

void UMat::create(const std::vector<ကိန်းပြည့်>& _sizes, ကိန်းပြည့် _type, UMatUsageFlags _usageFlags)
{
    create((ကိန်းပြည့်)_sizes.size(), _sizes.data(), _type, _usageFlags);
}

void UMat::create(const MatShape& _shape, ကိန်းပြည့် _type, UMatUsageFlags _usageFlags)
{
    အကယ်၍ (_shape.dims < 0) {
        release();
    } မဟုတ်ပါက {
        create(_shape.dims, _shape.p, _type, _usageFlags);
        size.layout = _shape.layout;
        size.C = _shape.C;
    }
}

void UMat::fit(ကိန်းပြည့် _dims, const ကိန်းပြည့်* _sizes, ကိန်းပြည့် _type, UMatUsageFlags _usageFlags)
{
    အကယ်၍ (_usageFlags == cv::USAGE_DEFAULT)
        _usageFlags = usageFlags;
    size_t oldTotalBytes = u ? u->size : 0;
    size_t esz = CV_ELEM_SIZE(_type), newTotal = _dims >= 0;
    အတွက် (ကိန်းပြည့် i = 0; i < _dims; i++)
        newTotal *= _sizes[i];
    size_t newTotalBytes = newTotal*esz;
    အကယ်၍ (newTotalBytes > 0 &&
        (!isContinuous() ||
         newTotalBytes > oldTotalBytes ||
         offset != 0 ||
         _usageFlags != usageFlags)) {
        create(_dims, _sizes, _type, _usageFlags);
    } မဟုတ်ပါက {
        flags = (flags & ~Mat::TYPE_MASK) | CV_MAT_TYPE(_type);
        ကိန်းပြည့် _dummy_size = 0;
        setSize(*this, (_dims >= 0 ? _dims : 1), (_dims >= 0 ? _sizes : &_dummy_size), nullptr, true);
        finalizeHdr(*this);
    }
}

void UMat::fit(const std::vector<ကိန်းပြည့်>& _shape, ကိန်းပြည့် _type, UMatUsageFlags _usageFlags)
{
    fit((ကိန်းပြည့်)_shape.size(), _shape.data(), _type, _usageFlags);
}

void UMat::fit(const MatShape& _shape, ကိန်းပြည့် _type, UMatUsageFlags _usageFlags)
{
    fit(_shape.dims, _shape.p, _type, _usageFlags);
    size.layout = _shape.layout;
    size.C = _shape.C;
}

void UMat::fit(ကိန်းပြည့် _rows, ကိန်းပြည့် _cols, ကိန်းပြည့် _type, UMatUsageFlags _usageFlags)
{
    _type &= TYPE_MASK;
    ကိန်းပြည့် sz[] = {_rows, _cols};
    fit(2, sz, _type, _usageFlags);
}

void UMat::fit(Size _sz, ကိန်းပြည့် _type, UMatUsageFlags _usageFlags)
{
    fit(_sz.height, _sz.width, _type, _usageFlags);
}

void UMat::fitSameSize(InputArray m, ကိန်းပြည့် _type, UMatUsageFlags _usageFlags)
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
    အကယ်၍( m.dims > 2 )
    {
        Range rs[MatShape::MAX_DIMS];
        rs[0] = _rowRange;
        rs[1] = _colRange;
        အတွက်( ကိန်းပြည့် i = 2; i < m.dims; i++ )
            rs[i] = Range::all();
        *this = m(rs);
        ပြန်ပေး;
    }

    *this = m;
    အကယ်၍( _rowRange != Range::all() && _rowRange != Range(0,rows) )
    {
        CV_Assert( 0 <= _rowRange.start &&
                  _rowRange.start <= _rowRange.end &&
                  _rowRange.end <= m.rows );
        size[0] = rows = _rowRange.size();
        offset += step*_rowRange.start;
        flags |= SUBMATRIX_FLAG;
    }

    အကယ်၍( _colRange != Range::all() && _colRange != Range(0,cols) )
    {
        CV_Assert( 0 <= _colRange.start &&
                  _colRange.start <= _colRange.end &&
                  _colRange.end <= m.cols );
        size[1] = cols = _colRange.size();
        offset += _colRange.start*elemSize();
        flags |= SUBMATRIX_FLAG;
    }

    updateContinuityFlag();

    အကယ်၍( rows <= 0 || cols <= 0 )
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
    အကယ်၍( roi.width < m.cols || roi.height < m.rows )
        flags |= SUBMATRIX_FLAG;

    size[0] = rows;
    size[1] = cols;
    step[0] = m.dims == 2 ? m.step[0] : cols*esz;
    step[1] = esz;

    updateContinuityFlag();

    addref();
    အကယ်၍( rows <= 0 || cols <= 0 )
    {
        rows = cols = 0;
        release();
    }
}


UMat::UMat(const UMat& m, const Range* ranges)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(USAGE_DEFAULT), u(0), offset(0)
{
    ကိန်းပြည့် i, d = m.dims;

    CV_Assert(ranges);
    အတွက်( i = 0; i < d; i++ )
    {
        Range r = ranges[i];
        CV_Assert( r == Range::all() || (0 <= r.start && r.start < r.end && r.end <= m.size[i]) );
    }
    *this = m;
    အတွက်( i = 0; i < d; i++ )
    {
        Range r = ranges[i];
        အကယ်၍( r != Range::all() && r != Range(0, size.p[i]))
        {
            size.p[i] = r.end - r.start;
            offset += r.start*step.p[i];
            flags |= SUBMATRIX_FLAG;
        }
    }

    အကယ်၍ (d <= 2) {
        rows = d == 2 ? size[0] : 1;
        cols = d <= 0 ? (d >= 0) : size[d > 1];
    }
    updateContinuityFlag();
}

UMat::UMat(const UMat& m, const std::vector<Range>& ranges)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(USAGE_DEFAULT), u(0), offset(0)
{
    ကိန်းပြည့် i, d = m.dims;

    CV_Assert((ကိန်းပြည့်)ranges.size() == d);
    အတွက် (i = 0; i < d; i++)
    {
        Range r = ranges[i];
        CV_Assert(r == Range::all() || (0 <= r.start && r.start < r.end && r.end <= m.size[i]));
    }
    *this = m;
    အတွက် (i = 0; i < d; i++)
    {
        Range r = ranges[i];
        အကယ်၍ (r != Range::all() && r != Range(0, size.p[i]))
        {
            size.p[i] = r.end - r.start;
            offset += r.start*step.p[i];
            flags |= SUBMATRIX_FLAG;
        }
    }

    အကယ်၍ (d <= 2) {
        rows = d == 2 ? size[0] : 1;
        cols = d <= 0 ? (d >= 0) : size[d > 1];
    }
    updateContinuityFlag();
}

UMat UMat::diag(ကိန်းပြည့် d) const
{
    CV_Assert( dims <= 2 );
    UMat m = *this;
    size_t esz = elemSize();
    ကိန်းပြည့် len;

    အကယ်၍( d >= 0 )
    {
        len = std::min(cols - d, rows);
        m.offset += esz*d;
    }
    မဟုတ်ပါက
    {
        len = std::min(rows + d, cols);
        m.offset -= step[0]*d;
    }
    CV_DbgAssert( len > 0 );

    m.size[0] = m.rows = len;
    m.size[1] = m.cols = 1;
    m.step[0] += (len > 1 ? esz : 0);

    m.updateContinuityFlag();

    အကယ်၍( size() != Size(1,1) )
        m.flags |= SUBMATRIX_FLAG;

    ပြန်ပေး m;
}

void UMat::locateROI( Size& wholeSize, Point& ofs ) const
{
    CV_Assert( dims <= 2 && step[0] > 0 );
    size_t esz = elemSize(), minstep;
    ptrdiff_t delta1 = (ptrdiff_t)offset, delta2 = (ptrdiff_t)u->size;

    အကယ်၍( delta1 == 0 )
        ofs.x = ofs.y = 0;
    မဟုတ်ပါက
    {
        ofs.y = (ကိန်းပြည့်)(delta1/step[0]);
        ofs.x = (ကိန်းပြည့်)((delta1 - step[0]*ofs.y)/esz);
        CV_DbgAssert( offset == (size_t)(ofs.y*step[0] + ofs.x*esz) );
    }
    minstep = (ofs.x + cols)*esz;
    wholeSize.height = (ကိန်းပြည့်)((delta2 - minstep)/step[0] + 1);
    wholeSize.height = std::max(wholeSize.height, ofs.y + rows);
    wholeSize.width = (ကိန်းပြည့်)((delta2 - step*(wholeSize.height-1))/esz);
    wholeSize.width = std::max(wholeSize.width, ofs.x + cols);
}


UMat& UMat::adjustROI( ကိန်းပြည့် dtop, ကိန်းပြည့် dbottom, ကိန်းပြည့် dleft, ကိန်းပြည့် dright )
{
    CV_Assert( dims == 2 && step[0] > 0 );
    Size wholeSize; Point ofs;
    size_t esz = elemSize();
    locateROI( wholeSize, ofs );
    ကိန်းပြည့် row1 = std::min(std::max(ofs.y - dtop, 0), wholeSize.height), row2 = std::max(0, std::min(ofs.y + rows + dbottom, wholeSize.height));
    ကိန်းပြည့် col1 = std::min(std::max(ofs.x - dleft, 0), wholeSize.width), col2 = std::max(0, std::min(ofs.x + cols + dright, wholeSize.width));
    အကယ်၍(row1 > row2)
        std::swap(row1, row2);
    အကယ်၍(col1 > col2)
        std::swap(col1, col2);

    offset += (row1 - ofs.y)*step + (col1 - ofs.x)*esz;
    rows = row2 - row1; cols = col2 - col1;
    size.p[0] = rows; size.p[1] = cols;
    updateContinuityFlag();
    ပြန်ပေး *this;
}


UMat UMat::reshape(ကိန်းပြည့် new_cn, ကိန်းပြည့် new_rows) const
{
    ကိန်းပြည့် cn = channels();
    UMat hdr = *this;

    အကယ်၍( dims > 2 && new_rows == 0 && new_cn != 0 && size[dims-1]*cn % new_cn == 0 )
    {
        hdr.flags = (hdr.flags & ~CV_MAT_CN_MASK) | ((new_cn-1) << CV_CN_SHIFT);
        hdr.step[dims-1] = CV_ELEM_SIZE(hdr.flags);
        hdr.size[dims-1] = hdr.size[dims-1]*cn / new_cn;
        ပြန်ပေး hdr;
    }

    CV_Assert( dims <= 2 );

    အကယ်၍( new_cn == 0 )
        new_cn = cn;

    ကိန်းပြည့် total_width = cols * cn;

    အကယ်၍( (new_cn > total_width || total_width % new_cn != 0) && new_rows == 0 )
        new_rows = rows * total_width / new_cn;

    အကယ်၍( new_rows != 0 && new_rows != rows )
    {
        ကိန်းပြည့် total_size = total_width * rows;
        အကယ်၍( !isContinuous() )
            CV_Error( cv::Error::BadStep,
            "The matrix is not continuous, thus its number of rows can not be changed" );

        total_width = total_size / new_rows;

        အကယ်၍( total_width * new_rows != total_size )
            CV_Error( cv::Error::StsBadArg, "The total number of matrix elements "
                                    "is not divisible by the new number of rows" );

        hdr.size[0] = hdr.rows = new_rows;
        hdr.step[0] = total_width * elemSize1();
    } မဟုတ်ပါက {
        hdr.size[0] = hdr.rows = rows;
        အကယ်၍ (dims <= 1)
            hdr.step[0] = cols * CV_ELEM_SIZE(flags);
    }

    ကိန်းပြည့် new_width = total_width / new_cn;

    အကယ်၍( new_width * new_cn != total_width )
        CV_Error( cv::Error::BadNumChannels,
        "The total width is not divisible by the new number of channels" );

    hdr.size.dims = hdr.dims = 2;
    hdr.size[1] = hdr.cols = new_width;
    hdr.flags = (hdr.flags & ~CV_MAT_CN_MASK) | ((new_cn-1) << CV_CN_SHIFT);
    hdr.step[1] = CV_ELEM_SIZE(hdr.flags);
    ပြန်ပေး hdr;
}

#ifdef HAVE_OPENCL
namespace {
static bool ocl_setDiag(const UMat& d, UMat& m, ကိန်းပြည့် len)
{
    ကိန်းပြည့် cn = d.channels();
    ကိန်းပြည့် depth = d.depth();

    အကယ်၍ (depth == CV_64F && !ocl::Device::getDefault().doubleFPConfig())
        ပြန်ပေး false;

    String opts = format("-D SET_DIAG -D T1=%s -D cn=%d -D IS_ROW_VECTOR=%d",
                         ocl::memopTypeToStr(depth),
                         cn,
                         (d.rows == 1) ? 1 : 0);

    ocl::Kernel k("setDiag", ocl::core::copyset_oclsrc, opts);
    အကယ်၍ (k.empty())
        ပြန်ပေး false;

    k.args(ocl::KernelArg::WriteOnly(m),
           ocl::KernelArg::ReadOnlyNoSize(d),
           len);

    size_t globalsize[2] = { (size_t)len, (size_t)len };
    ပြန်ပေး k.run(2, globalsize, NULL, false);
}
}
#endif

UMat UMat::diag(const UMat& d, UMatUsageFlags usageFlags)
{
    CV_INSTRUMENT_REGION();

    CV_Assert(d.cols == 1 || d.rows == 1);
    ကိန်းပြည့် len = d.rows + d.cols - 1;

#ifdef HAVE_OPENCL
    အကယ်၍ (ocl::useOpenCL())
    {
        UMat m(len, len, d.type(), usageFlags);
        အကယ်၍ (ocl_setDiag(d, m, len))
        {
            CV_IMPL_ADD(CV_IMPL_OCL);
            ပြန်ပေး m;
        }
    }
#endif

    UMat m(len, len, d.type(), Scalar(0), usageFlags);
    UMat md = m.diag();
    အကယ်၍( d.cols == 1 )
        d.copyTo(md);
    မဟုတ်ပါက
        transpose(d, md);
    ပြန်ပေး m;
}

ကိန်းပြည့် UMat::checkVector(ကိန်းပြည့် _elemChannels, ကိန်းပြည့် _depth, bool _requireContinuous) const
{
    ပြန်ပေး (depth() == _depth || _depth <= 0) &&
        (isContinuous() || !_requireContinuous) &&
        ((dims <= 2 && (((rows == 1 || cols == 1) && channels() == _elemChannels) ||
                        (cols == _elemChannels && channels() == 1))) ||
        (dims == 3 && channels() == 1 && size.p[2] == _elemChannels && (size.p[0] == 1 || size.p[1] == 1) &&
         (isContinuous() || step.p[1] == step.p[2]*size.p[2])))
    ? (ကိန်းပြည့်)(total()*channels()/_elemChannels) : -1;
}

UMat UMat::reshape(ကိန်းပြည့် _cn, ကိန်းပြည့် _newndims, const ကိန်းပြည့်* _newsz) const
{
    အကယ်၍(_newndims == dims)
    {
        အကယ်၍(_newsz == 0)
            ပြန်ပေး reshape(_cn);
        အကယ်၍(_newndims == 2)
            ပြန်ပေး reshape(_cn, _newsz[0]);
    }

    အကယ်၍ (isContinuous())
    {
        CV_Assert(_cn >= 0 && _newndims >= 0 && _newndims <= CV_MAX_DIM && (_newndims == 0 || _newsz != 0));

        အကယ်၍ (_cn == 0)
            _cn = this->channels();
        မဟုတ်ပါက
            CV_Assert(_cn <= CV_CN_MAX);

        size_t total_elem1_ref = this->total() * this->channels();
        size_t total_elem1 = _cn;

        AutoBuffer<ကိန်းပြည့်, 4> newsz_buf( (size_t)std::max(_newndims, 1) );

        ကိန်းပြည့် m1_idx = -1;

        အတွက် (ကိန်းပြည့် i = 0; i < _newndims; i++)
        {
            အကယ်၍ (_newsz[i] >= 0) {
                အကယ်၍ (_newsz[i] == 0 && i < dims)
                    newsz_buf[i] = size.p[i];
                မဟုတ်ပါက
                    newsz_buf[i] = _newsz[i];
                total_elem1 *= (size_t)newsz_buf[i];
            } မဟုတ်ပါက {
                အကယ်၍ (m1_idx >= 0)
                    CV_Error(cv::Error::StsBadSize, "More than one '-1' occured in the new shape");
                m1_idx = i;
            }
        }

        အကယ်၍ (m1_idx >= 0) {
            အကယ်၍ (total_elem1 == 0) {
                CV_Assert(total_elem1_ref == 0);
                total_elem1 = 1;
            }
            CV_Assert(total_elem1_ref % total_elem1 == 0);
            newsz_buf[m1_idx] = (ကိန်းပြည့်)(total_elem1_ref / total_elem1);
        } မဟုတ်ပါက အကယ်၍ (total_elem1 != total_elem1_ref) {
            CV_Error(cv::Error::StsUnmatchedSizes, "Requested and source matrices have different count of elements");
        }

        UMat hdr = *this;
        hdr.flags = (hdr.flags & ~CV_MAT_CN_MASK) | ((_cn-1) << CV_CN_SHIFT);
        setSize(hdr, _newndims, newsz_buf.data(), NULL, true);

        ပြန်ပေး hdr;
    }

    CV_Error(cv::Error::StsNotImplemented, "Reshaping of n-dimensional non-continuous matrices is not supported yet");
}

UMat UMat::reshape(ကိန်းပြည့် _cn, const MatShape& _newshape) const
{
    အကယ်၍ (_newshape.dims < 0) {
        ကိန်းပြည့် newshape[] = {0};
        ပြန်ပေး reshape(_cn, 1, newshape);
    }
    ပြန်ပေး reshape(_cn, _newshape.dims, _newshape.p);
}

Mat UMat::getMat(AccessFlag accessFlags) const
{
    အကယ်၍(!u)
        ပြန်ပေး Mat();
    // TODO Support ACCESS_READ (ACCESS_WRITE) without unnecessary data transfers
    accessFlags |= ACCESS_RW;
    UMatDataAutoLock autolock(u);
    try
    {
        အကယ်၍(CV_XADD(&u->refcount, 1) == 0)
            u->currAllocator->map(u, accessFlags);
        အကယ်၍ (u->data != 0)
        {
            Mat hdr(dims, size.p, type(), u->data + offset, step.p);
            hdr.flags = flags;
            hdr.u = u;
            hdr.datastart = u->data;
            hdr.data = u->data + offset;
            hdr.datalimit = hdr.dataend = u->data + u->size;
            ပြန်ပေး hdr;
        }
    }
    gr_haa_na(...)
    {
        CV_XADD(&u->refcount, -1);
        k_sipa;
    }
    CV_XADD(&u->refcount, -1);
    CV_Assert(u->data != 0 && "Error mapping of UMat to host memory.");
    ပြန်ပေး Mat();
}

void* UMat::handle(AccessFlag accessFlags) const
{
    အကယ်၍( !u )
        ပြန်ပေး 0;

    CV_Assert(u->refcount == 0);
    CV_Assert(!u->deviceCopyObsolete() || u->copyOnMap());
    အကယ်၍ (u->deviceCopyObsolete())
    {
        u->currAllocator->unmap(u);
    }

    အကယ်၍ (!!(accessFlags & ACCESS_WRITE))
        u->markHostCopyObsolete(true);

    ပြန်ပေး u->handle;
}

void UMat::ndoffset(size_t* ofs) const
{
    // offset = step[0]*ofs[0] + step[1]*ofs[1] + step[2]*ofs[2] + ...;
    size_t val = offset;
    အတွက်( ကိန်းပြည့် i = 0; i < dims; i++ )
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
    အကယ်၍ (_dst.isGpuMat())
    {
        _dst.getGpuMat().upload(*this);
        ပြန်ပေး;
    }
#endif

    ကိန်းပြည့် stype = type();
    ကိန်းပြည့် dtype = _dst.type();
    အကယ်၍( _dst.fixedType() && dtype != stype )
    {
        CV_Assert( CV_MAT_CN(stype) == CV_MAT_CN(dtype) );
        convertTo( _dst, dtype );
        ပြန်ပေး;
    }

    အကယ်၍( dims == 0 && empty() )
    {
        _dst.release();
        void* obj = _dst.getObj();
        အကယ်၍ (_dst.isMat())
            reinterpret_cast<Mat*>(obj)->flags = Mat::MAGIC_VAL | Mat::CONTINUOUS_FLAG | stype;
        မဟုတ်ပါက အကယ်၍ (_dst.isUMat())
            reinterpret_cast<UMat*>(obj)->flags = UMat::MAGIC_VAL | UMat::CONTINUOUS_FLAG | stype;
        မဟုတ်ပါက အကယ်၍ (_dst.isGpuMat())
            reinterpret_cast<cuda::GpuMat*>(obj)->flags = stype;
        ပြန်ပေး;
    }

    _dst.create( dims, size.p, stype );
    အကယ်၍ (empty())
        ပြန်ပေး;

    size_t sz[CV_MAX_DIM] = {1}, srcofs[CV_MAX_DIM]={0}, dstofs[CV_MAX_DIM]={0};
    size_t esz = CV_ELEM_SIZE(stype);
    ကိန်းပြည့် i, d = std::max(dims, 1);
    အတွက်( i = 0; i < d; i++ )
        sz[i] = size.p[i];
    sz[d-1] *= esz;
    ndoffset(srcofs);
    srcofs[d-1] *= esz;

    အကယ်၍( _dst.isUMat() )
    {
        UMat dst = _dst.getUMat();
        CV_Assert(dst.u);
        အကယ်၍( u == dst.u && dst.offset == offset )
            ပြန်ပေး;

        အကယ်၍ (u->currAllocator == dst.u->currAllocator)
        {
            dst.ndoffset(dstofs);
            dstofs[dims-1] *= esz;
            u->currAllocator->copy(u, dst.u, d, sz, srcofs, step.p, dstofs, dst.step.p, false);
            ပြန်ပေး;
        }

        // different allocators: never map a device destination, upload into it instead
        // (through a host Mat when both sides are device memory)
        MatAllocator* hostAllocator = Mat::getDefaultAllocator();
        အကယ်၍ (dst.u->currAllocator != hostAllocator)
        {
            အကယ်၍ (u->currAllocator == hostAllocator)
                getMat(ACCESS_READ).copyTo(_dst);
            မဟုတ်ပါက
            {
                Mat tmp;
                copyTo(tmp);
                tmp.copyTo(_dst);
            }
            ပြန်ပေး;
        }
    }

    Mat dst = _dst.getMat();
    u->currAllocator->download(u, dst.ptr(), d, sz, srcofs, step.p, dst.step.p);
}

void UMat::copyTo(OutputArray _dst, InputArray _mask) const
{
    CV_INSTRUMENT_REGION();

    အကယ်၍( _mask.empty() )
    {
        copyTo(_dst);
        ပြန်ပေး;
    }
#ifdef HAVE_OPENCL
    ကိန်းပြည့် cn = channels(), mtype = _mask.type(), mdepth = CV_MAT_DEPTH(mtype), mcn = CV_MAT_CN(mtype);
    CV_Assert( (mdepth == CV_8U || mdepth == CV_8S || mdepth == CV_Bool) && (mcn == 1 || mcn == cn) );

    အကယ်၍ (ocl::useOpenCL() && _dst.isUMat() && dims <= 2)
    {
        UMatData * prevu = _dst.getUMat().u;
        _dst.create( size, type() );

        UMat dst = _dst.getUMat();

        bool haveDstUninit = false;
        အကယ်၍( prevu != dst.u ) // do not leave dst uninitialized
            haveDstUninit = true;

        String opts = format("-D COPY_TO_MASK -D T1=%s -D scn=%d -D mcn=%d%s",
                             ocl::memopTypeToStr(depth()), cn, mcn,
                             haveDstUninit ? " -D HAVE_DST_UNINIT" : "");

        ocl::Kernel k("copyToMask", ocl::core::copyset_oclsrc, opts);
        အကယ်၍ (!k.empty())
        {
            k.args(ocl::KernelArg::ReadOnlyNoSize(*this),
                   ocl::KernelArg::ReadOnlyNoSize(_mask.getUMat()),
                   haveDstUninit ? ocl::KernelArg::WriteOnly(dst) :
                                   ocl::KernelArg::ReadWrite(dst));

            size_t globalsize[2] = { (size_t)cols, (size_t)rows };
            အကယ်၍ (k.run(2, globalsize, NULL, false))
            {
                CV_IMPL_ADD(CV_IMPL_OCL);
                ပြန်ပေး;
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
    ကိန်းပြည့် tp = type(), cn = CV_MAT_CN(tp), d = CV_MAT_DEPTH(tp);

    အကယ်၍( dims <= 2 && cn <= 4 && CV_MAT_DEPTH(tp) < CV_64F && ocl::useOpenCL() )
    {
        Mat value = _value.getMat();
        CV_Assert( checkScalar(value, type(), _value.kind(), _InputArray::UMAT) );
        ကိန်းပြည့် kercn = haveMask || cn == 3 ? cn : std::max(cn, ocl::predictOptimalVectorWidth(*this)),
                kertp = CV_MAKE_TYPE(d, kercn);

        dviguu_na buf[16] = { 0, 0, 0, 0, 0, 0, 0, 0,
                           0, 0, 0, 0, 0, 0, 0, 0 };
        convertAndUnrollScalar(value, tp, (uchar *)buf, kercn / cn);

        ကိန်းပြည့် scalarcn = kercn == 3 ? 4 : kercn, rowsPerWI = ocl::Device::getDefault().isIntel() ? 4 : 1;
        String opts = format("-D dstT=%s -D rowsPerWI=%d -D dstST=%s -D dstT1=%s -D cn=%d",
                             ocl::memopTypeToStr(kertp), rowsPerWI,
                             ocl::memopTypeToStr(CV_MAKETYPE(d, scalarcn)),
                             ocl::memopTypeToStr(d), kercn);

        ocl::Kernel setK(haveMask ? "setMask" : "set", ocl::core::copyset_oclsrc, opts);
        အကယ်၍( !setK.empty() )
        {
            ocl::KernelArg scalararg(ocl::KernelArg::CONSTANT, 0, 0, 0, buf, CV_ELEM_SIZE(d) * scalarcn);
            UMat mask;

            အကယ်၍( haveMask )
            {
                mask = _mask.getUMat();
                CV_Assert( mask.size() == size() && (mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool) );
                ocl::KernelArg maskarg = ocl::KernelArg::ReadOnlyNoSize(mask),
                        dstarg = ocl::KernelArg::ReadWrite(*this);
                setK.args(maskarg, dstarg, scalararg);
            }
            မဟုတ်ပါက
            {
                ocl::KernelArg dstarg = ocl::KernelArg::WriteOnly(*this, cn, kercn);
                setK.args(dstarg, scalararg);
            }

            size_t globalsize[] = { (size_t)cols * cn / kercn, ((size_t)rows + rowsPerWI - 1) / rowsPerWI };
            အကယ်၍( setK.run(2, globalsize, NULL, false) )
            {
                CV_IMPL_ADD(CV_IMPL_OCL);
                ပြန်ပေး *this;
            }
        }
    }
#endif
    Mat m = getMat(haveMask ? ACCESS_RW : ACCESS_WRITE);
    m.setTo(_value, _mask);
    ပြန်ပေး *this;
}

UMat& UMat::operator = (const Scalar& s)
{
    setTo(s);
    ပြန်ပေး *this;
}

UMat UMat::t() const
{
    UMat m;
    transpose(*this, m);
    ပြန်ပေး m;
}

UMat UMat::zeros(ကိန်းပြည့် rows, ကိန်းပြည့် cols, ကိန်းပြည့် type, UMatUsageFlags usageFlags)
{
    ပြန်ပေး UMat(rows, cols, type, Scalar::all(0), usageFlags);
}

UMat UMat::zeros(Size size, ကိန်းပြည့် type, UMatUsageFlags usageFlags)
{
    ပြန်ပေး UMat(size, type, Scalar::all(0), usageFlags);
}

UMat UMat::zeros(ကိန်းပြည့် ndims, const ကိန်းပြည့်* sz, ကိန်းပြည့် type, UMatUsageFlags usageFlags)
{
    ပြန်ပေး UMat(ndims, sz, type, Scalar::all(0), usageFlags);
}

UMat UMat::zeros(const MatShape& shape, ကိန်းပြည့် type, UMatUsageFlags usageFlags)
{
    ပြန်ပေး UMat(shape.dims, shape.p, type, Scalar::all(0), usageFlags);
}

UMat UMat::ones(ကိန်းပြည့် rows, ကိန်းပြည့် cols, ကိန်းပြည့် type, UMatUsageFlags usageFlags)
{
    ပြန်ပေး UMat(rows, cols, type, Scalar(1), usageFlags);
}

UMat UMat::ones(Size size, ကိန်းပြည့် type, UMatUsageFlags usageFlags)
{
    ပြန်ပေး UMat(size, type, Scalar(1), usageFlags);
}

UMat UMat::ones(ကိန်းပြည့် ndims, const ကိန်းပြည့်* sz, ကိန်းပြည့် type, UMatUsageFlags usageFlags)
{
    ပြန်ပေး UMat(ndims, sz, type, Scalar(1), usageFlags);
}

UMat UMat::ones(const MatShape& shape, ကိန်းပြည့် type, UMatUsageFlags usageFlags)
{
    ပြန်ပေး UMat(shape.dims, shape.p, type, Scalar(1), usageFlags);
}

}

/* End of file. */
