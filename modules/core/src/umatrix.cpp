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

#ଅନ୍ତର୍ଭାଵ "precomp.hpp"
#ଅନ୍ତର୍ଭାଵ "opencl_kernels_core.hpp"
#ଅନ୍ତର୍ଭାଵ "umatrix.hpp"

#ଅନ୍ତର୍ଭାଵ <opencv2/core/utils/tls.hpp>

///////////////////////////////// UMat implementation ///////////////////////////////

ନାମାକାଶ cv {

// forward decls, implementation is below in this file
ଶୂନ୍ୟ setSize(UMat& m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା _dims, ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା* _sz, ଅଚଳ size_t* _steps,
             ତାର୍କିକ autoSteps = ମିଥ୍ୟା);

ଶୂନ୍ୟ updateContinuityFlag(UMat& m);
ଶୂନ୍ୟ finalizeHdr(UMat& m);

UMatData::UMatData(ଅଚଳ MatAllocator* allocator)
{
    prevAllocator = currAllocator = allocator;
    urefcount = refcount = mapcount = 0;
    data = origdata = 0;
    size = 0;
    flags = static_cast<UMatData::MemoryFlag>(0);
    handle = 0;
    userdata = 0;
    allocatorFlags_ = 0;
    originalUMatData = ଖାଲି;
}

UMatData::~UMatData()
{
    prevAllocator = currAllocator = 0;
    urefcount = refcount = 0;
    CV_Assert(mapcount == 0);
    data = origdata = 0;
    size = 0;
    ତାର୍କିକ isAsyncCleanup = !!(flags & UMatData::ASYNC_CLEANUP);
    flags = static_cast<UMatData::MemoryFlag>(0);
    handle = 0;
    userdata = 0;
    allocatorFlags_ = 0;
    ଯଦି (originalUMatData)
    {
        ତାର୍କିକ showWarn = ମିଥ୍ୟା;
        UMatData* u = originalUMatData;
        ତାର୍କିକ zero_Ref = CV_XADD(&(u->refcount), -1) == 1;
        ଯଦି (zero_Ref)
        {
            // simulate Mat::deallocate
            ଯଦି (u->mapcount != 0)
            {
                (u->currAllocator ? u->currAllocator : /* TODO allocator ? allocator :*/ Mat::getDefaultAllocator())->unmap(u);
            }
            ଅନ୍ୟଥା
            {
                // we don't do "map", so we can't do "unmap"
            }
        }
        ତାର୍କିକ zero_URef = CV_XADD(&(u->urefcount), -1) == 1;
        ଯଦି (zero_Ref && !zero_URef)
            showWarn = ସତ୍ୟ;
        ଯଦି (zero_Ref && zero_URef) // oops, we need to free resources
        {
            showWarn = !isAsyncCleanup;
            // simulate UMat::deallocate
            u->currAllocator->deallocate(u);
        }
#ifndef NDEBUG
        ଯଦି (showWarn)
        {
            ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା warn_message_showed = 0;
            ଯଦି (warn_message_showed++ < 100)
            {
                fflush(stdout);
                fprintf(stderr, "\n! OPENCV warning: getUMat()/getMat() call chain possible problem."
                                "\n!                 Base object is dead, while nested/derived object is still alive or processed."
                                "\n!                 Please check lifetime of UMat/Mat objects!\n");
                fflush(stderr);
            }
        }
#ଅନ୍ୟଥା
        CV_UNUSED(showWarn);
#endif
        originalUMatData = ଖାଲି;
    }
}

#ifndef OPENCV_DISABLE_THREAD_SUPPORT

// it should be a prime number for the best hash function
ଗଣନା { UMAT_NLOCKS = 31 };
ସ୍ଥିର Mutex umatLocks[UMAT_NLOCKS];

ସ୍ଥିର size_t getUMatDataLockIndex(ଅଚଳ UMatData* u)
{
    size_t idx = ((size_t)(ଶୂନ୍ୟ*)u) % UMAT_NLOCKS;
    ଫେରନ୍ତୁ idx;
}

ଶୂନ୍ୟ UMatData::lock()
{
    size_t idx = getUMatDataLockIndex(ଏହି);
    //printf("%d lock(%d)\n", cv::utils::getThreadID(), (int)idx);
    umatLocks[idx].lock();
}

ଶୂନ୍ୟ UMatData::unlock()
{
    size_t idx = getUMatDataLockIndex(ଏହି);
    //printf("%d unlock(%d)\n", cv::utils::getThreadID(), (int)idx);
    umatLocks[idx].unlock();
}


// Do not allow several lock() calls with different UMatData objects.
ସଂରଚନା UMatDataAutoLocker
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା usage_ଗଣନା;
    UMatData* locked_objects[2];
    UMatDataAutoLocker() : usage_ଗଣନା(0) { locked_objects[0] = ଖାଲି; locked_objects[1] = ଖାଲି; }

    ଶୂନ୍ୟ lock(UMatData*& u1)
    {
        ତାର୍କିକ locked_1 = (u1 == locked_objects[0] || u1 == locked_objects[1]);
        ଯଦି (locked_1)
        {
            u1 = ଖାଲି;
            ଫେରନ୍ତୁ;
        }
        CV_Assert(usage_ଗଣନା == 0);  // UMatDataAutoLock can't be used multiple times from the same thread
        usage_ଗଣନା = 1;
        locked_objects[0] = u1;
        u1->lock();
    }
    ଶୂନ୍ୟ lock(UMatData*& u1, UMatData*& u2)
    {
        ତାର୍କିକ locked_1 = (u1 == locked_objects[0] || u1 == locked_objects[1]);
        ତାର୍କିକ locked_2 = (u2 == locked_objects[0] || u2 == locked_objects[1]);
        ଯଦି (locked_1)
            u1 = ଖାଲି;
        ଯଦି (locked_2)
            u2 = ଖାଲି;
        ଯଦି (locked_1 && locked_2)
            ଫେରନ୍ତୁ;
        CV_Assert(usage_ଗଣନା == 0);  // UMatDataAutoLock can't be used multiple times from the same thread
        usage_ଗଣନା = 1;
        locked_objects[0] = u1;
        locked_objects[1] = u2;
        ଯଦି (u1)
            u1->lock();
        ଯଦି (u2)
            u2->lock();
    }
    ଶୂନ୍ୟ release(UMatData* u1, UMatData* u2)
    {
        ଯଦି (u1 == ଖାଲି && u2 == ଖାଲି)
            ଫେରନ୍ତୁ;
        CV_Assert(usage_ଗଣନା == 1);
        usage_ଗଣନା = 0;
        ଯଦି (u1)
            u1->unlock();
        ଯଦି (u2)
            u2->unlock();
        locked_objects[0] = ଖାଲି; locked_objects[1] = ଖାଲି;
    }
};
ସ୍ଥିର TLSData<UMatDataAutoLocker>& getUMatDataAutoLockerTLS()
{
    CV_SINGLETON_LAZY_INIT_REF(TLSData<UMatDataAutoLocker>, ନୂତନ TLSData<UMatDataAutoLocker>());
}
ସ୍ଥିର UMatDataAutoLocker& getUMatDataAutoLocker() { ଫେରନ୍ତୁ getUMatDataAutoLockerTLS().getRef(); }


UMatDataAutoLock::UMatDataAutoLock(UMatData* u) : u1(u), u2(ଖାଲି)
{
    getUMatDataAutoLocker().lock(u1);
}
UMatDataAutoLock::UMatDataAutoLock(UMatData* u1_, UMatData* u2_) : u1(u1_), u2(u2_)
{
    ଯଦି (getUMatDataLockIndex(u1) > getUMatDataLockIndex(u2))
    {
        std::swap(u1, u2);
    }
    getUMatDataAutoLocker().lock(u1, u2);
}
UMatDataAutoLock::~UMatDataAutoLock()
{
    getUMatDataAutoLocker().release(u1, u2);
}

#ଅନ୍ୟଥା

ଶୂନ୍ୟ UMatData::lock()
{
    // nothing in OPENCV_DISABLE_THREAD_SUPPORT mode
}

ଶୂନ୍ୟ UMatData::unlock()
{
    // nothing in OPENCV_DISABLE_THREAD_SUPPORT mode
}

UMatDataAutoLock::UMatDataAutoLock(UMatData* u) : u1(u), u2(ଖାଲି)
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

UMat::UMat(ପୂର୍ଣ୍ଣସଂଖ୍ୟା _rows, ପୂର୍ଣ୍ଣସଂଖ୍ୟା _cols, ପୂର୍ଣ୍ଣସଂଖ୍ୟା _type, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_rows, _cols, _type);
}

UMat::UMat(ପୂର୍ଣ୍ଣସଂଖ୍ୟା _rows, ପୂର୍ଣ୍ଣସଂଖ୍ୟା _cols, ପୂର୍ଣ୍ଣସଂଖ୍ୟା _type, ଅଚଳ Scalar& _s, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_rows, _cols, _type);
    *ଏହି = _s;
}

UMat::UMat(Size _sz, ପୂର୍ଣ୍ଣସଂଖ୍ୟା _type, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create( _sz.height, _sz.width, _type );
}

UMat::UMat(Size _sz, ପୂର୍ଣ୍ଣସଂଖ୍ୟା _type, ଅଚଳ Scalar& _s, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_sz.height, _sz.width, _type);
    *ଏହି = _s;
}

UMat::UMat(ପୂର୍ଣ୍ଣସଂଖ୍ୟା _dims, ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା* _sz, ପୂର୍ଣ୍ଣସଂଖ୍ୟା _type, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_dims, _sz, _type);
}

UMat::UMat(ପୂର୍ଣ୍ଣସଂଖ୍ୟା _dims, ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା* _sz, ପୂର୍ଣ୍ଣସଂଖ୍ୟା _type, ଅଚଳ Scalar& _s, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_dims, _sz, _type);
    *ଏହି = _s;
}

UMat::UMat(ଅଚଳ MatShape& _shape, ପୂର୍ଣ୍ଣସଂଖ୍ୟା _type, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_shape.dims, _shape.p, _type);
}

UMat::UMat(ଅଚଳ MatShape& _shape, ପୂର୍ଣ୍ଣସଂଖ୍ୟା _type, ଅଚଳ Scalar& _s, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_shape.dims, _shape.p, _type);
    *ଏହି = _s;
}

UMat::UMat(ଅଚଳ UMat& m)
: flags(m.flags), dims(m.dims), rows(m.rows), cols(m.cols), allocator(m.allocator),
  usageFlags(m.usageFlags), u(m.u), offset(m.offset), size(m.size), step(m.step)
{
    addref();
}

UMat& UMat::operator=(ଅଚଳ UMat& m)
{
    ଯଦି( ଏହି != &m )
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
    ଫେରନ୍ତୁ *ଏହି;
}

UMat UMat::clone() ଅଚଳ
{
    UMat m;
    copyTo(m);
    ଫେରନ୍ତୁ m;
}

ଶୂନ୍ୟ UMat::assignTo(UMat& m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା _type) ଅଚଳ
{
    ଯଦି( _type < 0 )
        m = *ଏହି;
    ଅନ୍ୟଥା
        convertTo(m, _type);
}

ଶୂନ୍ୟ UMat::create(ପୂର୍ଣ୍ଣସଂଖ୍ୟା _rows, ପୂର୍ଣ୍ଣସଂଖ୍ୟା _cols, ପୂର୍ଣ୍ଣସଂଖ୍ୟା _type, UMatUsageFlags _usageFlags)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା sz[] = {_rows, _cols};
    create(2, sz, _type, _usageFlags);
}

ଶୂନ୍ୟ UMat::create(Size _sz, ପୂର୍ଣ୍ଣସଂଖ୍ୟା _type, UMatUsageFlags _usageFlags)
{
    create(_sz.height, _sz.width, _type, _usageFlags);
}

ଶୂନ୍ୟ UMat::createSameSize(ନିବେଶ_Array arr, ପୂର୍ଣ୍ଣସଂଖ୍ୟା _type, UMatUsageFlags _usageFlags)
{
    create(arr.shape(), _type, _usageFlags);
}

ଶୂନ୍ୟ UMat::addref()
{
    ଯଦି( u )
        CV_XADD(&(u->urefcount), 1);
}

ଶୂନ୍ୟ UMat::release()
{
    ଯଦି( u && CV_XADD(&(u->urefcount), -1) == 1 )
        deallocate();
    u = 0;
    size.clear();
    dims = cols = rows = 0;
}

ତାର୍କିକ UMat::empty() ଅଚଳ
{
    ଫେରନ୍ତୁ u == 0 || total() == 0;
}

size_t UMat::total() ଅଚଳ
{
    ଯଦି( dims <= 2 )
        ଫେରନ୍ତୁ (size_t)rows * cols;
    size_t p = 1;
    ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < dims; i++ )
        p *= size[i];
    ଫେରନ୍ତୁ p;
}

MatShape UMat::shape() ଅଚଳ
{
    ଫେରନ୍ତୁ size;
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
    m.allocator = ଖାଲି;
    m.u = ଖାଲି;
    m.offset = 0;
}

UMat& UMat::operator=(UMat&& m)
{
    ଯଦି (ଏହି == &m)
      ଫେରନ୍ତୁ *ଏହି;
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
    m.allocator = ଖାଲି;
    m.u = ଖାଲି;
    m.offset = 0;
    ଫେରନ୍ତୁ *ଏହି;
}


MatAllocator* UMat::getStdAllocator()
{
#ifdef HAVE_OPENCL
    ଯଦି (ocl::useOpenCL())
        ଫେରନ୍ତୁ ocl::getOpenCLAllocator();
#endif
    ଫେରନ୍ତୁ Mat::getDefaultAllocator();
}

ଶୂନ୍ୟ swap( UMat& a, UMat& b )
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


ଶୂନ୍ୟ setSize( UMat& m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା _dims, ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା* _sz,
              ଅଚଳ size_t* _steps, ତାର୍କିକ autoSteps )
{
    CV_Assert( 0 <= _dims && _dims <= CV_MAX_DIM && _dims <= MatShape::MAX_DIMS);

    m.dims = _dims;
    m.size = MatShape(_dims, _sz);
    m.step[std::max(_dims-1, 0)] = CV_ELEM_SIZE(m.flags);
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = _dims-2; i >= 0; i--) {
        size_t autostep = m.size[i+1]*m.step[i+1];
        ଯଦି (_steps) {
            m.step[i] = _steps[i];
            //CV_Assert(m.step[i] >= autostep);
        } ଅନ୍ୟଥା ଯଦି (autoSteps) {
            m.step[i] = autostep;
        } ଅନ୍ୟଥା {
            m.step[i] = 0;
        }
    }

    ଯଦି( _dims <= 2 )
    {
        m.cols = _dims == 0 ? 1 : _sz ? _sz[_dims > 1] : 0;
        m.rows = _dims < 2 ? 1 : _sz ? _sz[0] : 0;
    } ଅନ୍ୟଥା {
        m.cols = m.rows = -1;
    }
}


ଶୂନ୍ୟ UMat::updateContinuityFlag()
{
    flags = cv::updateContinuityFlag(flags, dims, size.p, step.p);
}


ଶୂନ୍ୟ finalizeHdr(UMat& m)
{
    m.updateContinuityFlag();
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା d = m.dims;
    ଯଦି( d > 2 )
        m.rows = m.cols = -1;
}


UMat Mat::getUMat(AccessFlag accessFlags, UMatUsageFlags usageFlags) ଅଚଳ
{
    UMat hdr;
    ଯଦି(!data)
        ଫେରନ୍ତୁ hdr;
    ଯଦି (data != datastart)
    {
        Size wholeSize;
        Point ofs;
        locateROI(wholeSize, ofs);
        Size sz(cols, rows);
        ଯଦି (ofs.x != 0 || ofs.y != 0)
        {
            Mat src = *ଏହି;
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା dtop = ofs.y;
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା dbottom = wholeSize.height - src.rows - ofs.y;
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା dleft = ofs.x;
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା dright = wholeSize.width - src.cols - ofs.x;
            src.adjustROI(dtop, dbottom, dleft, dright);
            ଫେରନ୍ତୁ src.getUMat(accessFlags, usageFlags)(cv::Rect(ofs.x, ofs.y, sz.width, sz.height));
        }
    }
    CV_Assert(data == datastart);

    accessFlags |= ACCESS_RW;
    UMatData* new_u = ଖାଲି;
    MatStep new_step = step;
    {
        MatAllocator *a = allocator, *a0 = getDefaultAllocator();
        ଯଦି(!a)
            a = a0;
        new_u = a->allocate(dims, size.p, type(), data, new_step.p, accessFlags, usageFlags);
        new_u->originalUMatData = u;
    }
    ତାର୍କିକ allocated = ମିଥ୍ୟା;
    ଚେଷ୍ଟା
    {
        allocated = UMat::getStdAllocator()->allocate(new_u, accessFlags, usageFlags);
    }
    ଧର (ଅଚଳ cv::Exception& e)
    {
        fprintf(stderr, "Exception: %s\n", e.what());
    }
    ଯଦି (!allocated)
    {
        allocated = getDefaultAllocator()->allocate(new_u, accessFlags, usageFlags);
        CV_Assert(allocated);
    }
    ଯଦି (u != ଖାଲି)
    {
#ifdef HAVE_OPENCL
        ଯଦି (ocl::useOpenCL() && new_u->currAllocator == ocl::getOpenCLAllocator())
        {
            CV_Assert(new_u->tempUMat());
        }
#endif
        CV_XADD(&(u->refcount), 1);
        CV_XADD(&(u->urefcount), 1);
    }
    ଚେଷ୍ଟା
    {
        hdr.flags = flags;
        hdr.usageFlags = usageFlags;
        setSize(hdr, dims, size.p, new_step.p);
        finalizeHdr(hdr);
        hdr.u = new_u;
        hdr.offset = 0; //data - datastart;
        hdr.addref();
        ଫେରନ୍ତୁ hdr;
    }
    ଧର(...)
    {
        ଯଦି (u != ଖାଲି)
        {
            CV_XADD(&(u->refcount), -1);
            CV_XADD(&(u->urefcount), -1);
        }
        new_u->currAllocator->deallocate(new_u);
        ଫିଙ୍ଗ;
    }

}

ଶୂନ୍ୟ UMat::create(ପୂର୍ଣ୍ଣସଂଖ୍ୟା d0, ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା* _sizes, ପୂର୍ଣ୍ଣସଂଖ୍ୟା _type, UMatUsageFlags _usageFlags)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା sz1 = 1, d = d0;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ଯଦି (d == 0) {
        d = 1;
        _sizes = (ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା*)&sz1;
    }
    CV_Assert(0 <= d && d <= CV_MAX_DIM && _sizes);
    _type = CV_MAT_TYPE(_type);

    // if param value is USAGE_DEFAULT by implicit default param value -or- explicit value
    // ...then don't change the existing usageFlags
    // it is not possible to change usage from non-default to USAGE_DEFAULT through create()
    // ...instead must construct UMat()
    ଯଦି (_usageFlags == cv::USAGE_DEFAULT)
    {
        _usageFlags = usageFlags;
    }

    ଯଦି( u && d == dims && _type == type() && _usageFlags == usageFlags )
    {
        ପାଇଁ( i = 0; i < d; i++ )
            ଯଦି( size[i] != _sizes[i] )
                ଭାଙ୍ଗ;
        ଯଦି( i == d )
            ଫେରନ୍ତୁ;
    }

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା _sizes_backup[CV_MAX_DIM]; // #5991
    ଯଦି (_sizes == (ଏହି->size.p))
    {
        ପାଇଁ(i = 0; i < d; i++ )
            _sizes_backup[i] = _sizes[i];
        _sizes = _sizes_backup;
    }

    release();
    usageFlags = _usageFlags;
    ଯଦି( d == 0 )
        ଫେରନ୍ତୁ;
    flags = (_type & CV_MAT_TYPE_MASK) | MAGIC_VAL;
    setSize(*ଏହି, d, _sizes, 0, ସତ୍ୟ);
    offset = 0;

    ଯଦି( total() > 0 )
    {
        MatAllocator *a = allocator, *a0 = getStdAllocator();
        ଯଦି (!a)
        {
            a = a0;
            a0 = Mat::getDefaultAllocator();
        }
        ଚେଷ୍ଟା
        {
            u = a->allocate(dims, size.p, _type, 0, step.p, ACCESS_RW /* ignored */, usageFlags);
            CV_Assert(u != 0);
        }
        ଧର(...)
        {
            ଯଦି(a != a0)
                u = a0->allocate(dims, size.p, _type, 0, step.p, ACCESS_RW /* ignored */, usageFlags);
            CV_Assert(u != 0);
        }
        CV_Assert( step[dims-1] == (size_t)CV_ELEM_SIZE(flags) );
    }

    finalizeHdr(*ଏହି);
    addref();
    size.dims = dims = d0;
}

ଶୂନ୍ୟ UMat::create(ଅଚଳ std::vector<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>& _sizes, ପୂର୍ଣ୍ଣସଂଖ୍ୟା _type, UMatUsageFlags _usageFlags)
{
    create((ପୂର୍ଣ୍ଣସଂଖ୍ୟା)_sizes.size(), _sizes.data(), _type, _usageFlags);
}

ଶୂନ୍ୟ UMat::create(ଅଚଳ MatShape& _shape, ପୂର୍ଣ୍ଣସଂଖ୍ୟା _type, UMatUsageFlags _usageFlags)
{
    ଯଦି (_shape.dims < 0) {
        release();
    } ଅନ୍ୟଥା {
        create(_shape.dims, _shape.p, _type, _usageFlags);
        size.layout = _shape.layout;
        size.C = _shape.C;
    }
}

ଶୂନ୍ୟ UMat::fit(ପୂର୍ଣ୍ଣସଂଖ୍ୟା _dims, ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା* _sizes, ପୂର୍ଣ୍ଣସଂଖ୍ୟା _type, UMatUsageFlags _usageFlags)
{
    ଯଦି (_usageFlags == cv::USAGE_DEFAULT)
        _usageFlags = usageFlags;
    size_t oldTotalBytes = u ? u->size : 0;
    size_t esz = CV_ELEM_SIZE(_type), newTotal = _dims >= 0;
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < _dims; i++)
        newTotal *= _sizes[i];
    size_t newTotalBytes = newTotal*esz;
    ଯଦି (newTotalBytes > 0 &&
        (!isContinuous() ||
         newTotalBytes > oldTotalBytes ||
         offset != 0 ||
         _usageFlags != usageFlags)) {
        create(_dims, _sizes, _type, _usageFlags);
    } ଅନ୍ୟଥା {
        flags = (flags & ~Mat::TYPE_MASK) | CV_MAT_TYPE(_type);
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା _dummy_size = 0;
        setSize(*ଏହି, (_dims >= 0 ? _dims : 1), (_dims >= 0 ? _sizes : &_dummy_size), nullptr, ସତ୍ୟ);
        finalizeHdr(*ଏହି);
    }
}

ଶୂନ୍ୟ UMat::fit(ଅଚଳ std::vector<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>& _shape, ପୂର୍ଣ୍ଣସଂଖ୍ୟା _type, UMatUsageFlags _usageFlags)
{
    fit((ପୂର୍ଣ୍ଣସଂଖ୍ୟା)_shape.size(), _shape.data(), _type, _usageFlags);
}

ଶୂନ୍ୟ UMat::fit(ଅଚଳ MatShape& _shape, ପୂର୍ଣ୍ଣସଂଖ୍ୟା _type, UMatUsageFlags _usageFlags)
{
    fit(_shape.dims, _shape.p, _type, _usageFlags);
    size.layout = _shape.layout;
    size.C = _shape.C;
}

ଶୂନ୍ୟ UMat::fit(ପୂର୍ଣ୍ଣସଂଖ୍ୟା _rows, ପୂର୍ଣ୍ଣସଂଖ୍ୟା _cols, ପୂର୍ଣ୍ଣସଂଖ୍ୟା _type, UMatUsageFlags _usageFlags)
{
    _type &= TYPE_MASK;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା sz[] = {_rows, _cols};
    fit(2, sz, _type, _usageFlags);
}

ଶୂନ୍ୟ UMat::fit(Size _sz, ପୂର୍ଣ୍ଣସଂଖ୍ୟା _type, UMatUsageFlags _usageFlags)
{
    fit(_sz.height, _sz.width, _type, _usageFlags);
}

ଶୂନ୍ୟ UMat::fitSameSize(ନିବେଶ_Array m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା _type, UMatUsageFlags _usageFlags)
{
    fit(m.shape(), _type, _usageFlags);
}

ଶୂନ୍ୟ UMat::copySize(ଅଚଳ UMat& m)
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

ଶୂନ୍ୟ UMat::deallocate()
{
    UMatData* u_ = u;
    u = ଖାଲି;
    u_->currAllocator->deallocate(u_);
}


UMat::UMat(ଅଚଳ UMat& m, ଅଚଳ Range& _rowRange, ଅଚଳ Range& _colRange)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(USAGE_DEFAULT), u(0), offset(0)
{
    CV_Assert( m.dims >= 2 && m.dims <= MatShape::MAX_DIMS );
    ଯଦି( m.dims > 2 )
    {
        Range rs[MatShape::MAX_DIMS];
        rs[0] = _rowRange;
        rs[1] = _colRange;
        ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 2; i < m.dims; i++ )
            rs[i] = Range::all();
        *ଏହି = m(rs);
        ଫେରନ୍ତୁ;
    }

    *ଏହି = m;
    ଯଦି( _rowRange != Range::all() && _rowRange != Range(0,rows) )
    {
        CV_Assert( 0 <= _rowRange.start &&
                  _rowRange.start <= _rowRange.end &&
                  _rowRange.end <= m.rows );
        size[0] = rows = _rowRange.size();
        offset += step*_rowRange.start;
        flags |= SUBMATRIX_FLAG;
    }

    ଯଦି( _colRange != Range::all() && _colRange != Range(0,cols) )
    {
        CV_Assert( 0 <= _colRange.start &&
                  _colRange.start <= _colRange.end &&
                  _colRange.end <= m.cols );
        size[1] = cols = _colRange.size();
        offset += _colRange.start*elemSize();
        flags |= SUBMATRIX_FLAG;
    }

    updateContinuityFlag();

    ଯଦି( rows <= 0 || cols <= 0 )
    {
        release();
        rows = cols = 0;
    }
}


UMat::UMat(ଅଚଳ UMat& m, ଅଚଳ Rect& roi)
    : flags(m.flags), dims(2), rows(roi.height), cols(roi.width),
    allocator(m.allocator), usageFlags(m.usageFlags), u(m.u),
    offset(m.offset + roi.y*m.step[0]), size(2)
{
    CV_Assert( m.dims <= 2 );

    size_t esz = CV_ELEM_SIZE(flags);
    offset += roi.x*esz;
    CV_Assert( 0 <= roi.x && 0 <= roi.width && roi.x + roi.width <= m.cols &&
              0 <= roi.y && 0 <= roi.height && roi.y + roi.height <= m.rows );
    ଯଦି( roi.width < m.cols || roi.height < m.rows )
        flags |= SUBMATRIX_FLAG;

    size[0] = rows;
    size[1] = cols;
    step[0] = m.dims == 2 ? m.step[0] : cols*esz;
    step[1] = esz;

    updateContinuityFlag();

    addref();
    ଯଦି( rows <= 0 || cols <= 0 )
    {
        rows = cols = 0;
        release();
    }
}


UMat::UMat(ଅଚଳ UMat& m, ଅଚଳ Range* ranges)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(USAGE_DEFAULT), u(0), offset(0)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, d = m.dims;

    CV_Assert(ranges);
    ପାଇଁ( i = 0; i < d; i++ )
    {
        Range r = ranges[i];
        CV_Assert( r == Range::all() || (0 <= r.start && r.start < r.end && r.end <= m.size[i]) );
    }
    *ଏହି = m;
    ପାଇଁ( i = 0; i < d; i++ )
    {
        Range r = ranges[i];
        ଯଦି( r != Range::all() && r != Range(0, size.p[i]))
        {
            size.p[i] = r.end - r.start;
            offset += r.start*step.p[i];
            flags |= SUBMATRIX_FLAG;
        }
    }

    ଯଦି (d <= 2) {
        rows = d == 2 ? size[0] : 1;
        cols = d <= 0 ? (d >= 0) : size[d > 1];
    }
    updateContinuityFlag();
}

UMat::UMat(ଅଚଳ UMat& m, ଅଚଳ std::vector<Range>& ranges)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(USAGE_DEFAULT), u(0), offset(0)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, d = m.dims;

    CV_Assert((ପୂର୍ଣ୍ଣସଂଖ୍ୟା)ranges.size() == d);
    ପାଇଁ (i = 0; i < d; i++)
    {
        Range r = ranges[i];
        CV_Assert(r == Range::all() || (0 <= r.start && r.start < r.end && r.end <= m.size[i]));
    }
    *ଏହି = m;
    ପାଇଁ (i = 0; i < d; i++)
    {
        Range r = ranges[i];
        ଯଦି (r != Range::all() && r != Range(0, size.p[i]))
        {
            size.p[i] = r.end - r.start;
            offset += r.start*step.p[i];
            flags |= SUBMATRIX_FLAG;
        }
    }

    ଯଦି (d <= 2) {
        rows = d == 2 ? size[0] : 1;
        cols = d <= 0 ? (d >= 0) : size[d > 1];
    }
    updateContinuityFlag();
}

UMat UMat::diag(ପୂର୍ଣ୍ଣସଂଖ୍ୟା d) ଅଚଳ
{
    CV_Assert( dims <= 2 );
    UMat m = *ଏହି;
    size_t esz = elemSize();
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା len;

    ଯଦି( d >= 0 )
    {
        len = std::min(cols - d, rows);
        m.offset += esz*d;
    }
    ଅନ୍ୟଥା
    {
        len = std::min(rows + d, cols);
        m.offset -= step[0]*d;
    }
    CV_DbgAssert( len > 0 );

    m.size[0] = m.rows = len;
    m.size[1] = m.cols = 1;
    m.step[0] += (len > 1 ? esz : 0);

    m.updateContinuityFlag();

    ଯଦି( size() != Size(1,1) )
        m.flags |= SUBMATRIX_FLAG;

    ଫେରନ୍ତୁ m;
}

ଶୂନ୍ୟ UMat::locateROI( Size& wholeSize, Point& ofs ) ଅଚଳ
{
    CV_Assert( dims <= 2 && step[0] > 0 );
    size_t esz = elemSize(), minstep;
    ptrdiff_t delta1 = (ptrdiff_t)offset, delta2 = (ptrdiff_t)u->size;

    ଯଦି( delta1 == 0 )
        ofs.x = ofs.y = 0;
    ଅନ୍ୟଥା
    {
        ofs.y = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(delta1/step[0]);
        ofs.x = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)((delta1 - step[0]*ofs.y)/esz);
        CV_DbgAssert( offset == (size_t)(ofs.y*step[0] + ofs.x*esz) );
    }
    minstep = (ofs.x + cols)*esz;
    wholeSize.height = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)((delta2 - minstep)/step[0] + 1);
    wholeSize.height = std::max(wholeSize.height, ofs.y + rows);
    wholeSize.width = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)((delta2 - step*(wholeSize.height-1))/esz);
    wholeSize.width = std::max(wholeSize.width, ofs.x + cols);
}


UMat& UMat::adjustROI( ପୂର୍ଣ୍ଣସଂଖ୍ୟା dtop, ପୂର୍ଣ୍ଣସଂଖ୍ୟା dbottom, ପୂର୍ଣ୍ଣସଂଖ୍ୟା dleft, ପୂର୍ଣ୍ଣସଂଖ୍ୟା dright )
{
    CV_Assert( dims == 2 && step[0] > 0 );
    Size wholeSize; Point ofs;
    size_t esz = elemSize();
    locateROI( wholeSize, ofs );
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା row1 = std::min(std::max(ofs.y - dtop, 0), wholeSize.height), row2 = std::max(0, std::min(ofs.y + rows + dbottom, wholeSize.height));
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା col1 = std::min(std::max(ofs.x - dleft, 0), wholeSize.width), col2 = std::max(0, std::min(ofs.x + cols + dright, wholeSize.width));
    ଯଦି(row1 > row2)
        std::swap(row1, row2);
    ଯଦି(col1 > col2)
        std::swap(col1, col2);

    offset += (row1 - ofs.y)*step + (col1 - ofs.x)*esz;
    rows = row2 - row1; cols = col2 - col1;
    size.p[0] = rows; size.p[1] = cols;
    updateContinuityFlag();
    ଫେରନ୍ତୁ *ଏହି;
}


UMat UMat::reshape(ପୂର୍ଣ୍ଣସଂଖ୍ୟା new_cn, ପୂର୍ଣ୍ଣସଂଖ୍ୟା new_rows) ଅଚଳ
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn = channels();
    UMat hdr = *ଏହି;

    ଯଦି( dims > 2 && new_rows == 0 && new_cn != 0 && size[dims-1]*cn % new_cn == 0 )
    {
        hdr.flags = (hdr.flags & ~CV_MAT_CN_MASK) | ((new_cn-1) << CV_CN_SHIFT);
        hdr.step[dims-1] = CV_ELEM_SIZE(hdr.flags);
        hdr.size[dims-1] = hdr.size[dims-1]*cn / new_cn;
        ଫେରନ୍ତୁ hdr;
    }

    CV_Assert( dims <= 2 );

    ଯଦି( new_cn == 0 )
        new_cn = cn;

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା total_width = cols * cn;

    ଯଦି( (new_cn > total_width || total_width % new_cn != 0) && new_rows == 0 )
        new_rows = rows * total_width / new_cn;

    ଯଦି( new_rows != 0 && new_rows != rows )
    {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା total_size = total_width * rows;
        ଯଦି( !isContinuous() )
            CV_Error( cv::Error::BadStep,
            "The matrix is not continuous, thus its number of rows can not be changed" );

        total_width = total_size / new_rows;

        ଯଦି( total_width * new_rows != total_size )
            CV_Error( cv::Error::StsBadArg, "The total number of matrix elements "
                                    "is not divisible by the new number of rows" );

        hdr.size[0] = hdr.rows = new_rows;
        hdr.step[0] = total_width * elemSize1();
    } ଅନ୍ୟଥା {
        hdr.size[0] = hdr.rows = rows;
        ଯଦି (dims <= 1)
            hdr.step[0] = cols * CV_ELEM_SIZE(flags);
    }

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା new_width = total_width / new_cn;

    ଯଦି( new_width * new_cn != total_width )
        CV_Error( cv::Error::Bad_ଅଙ୍କ_Channels,
        "The total width is not divisible by the new number of channels" );

    hdr.size.dims = hdr.dims = 2;
    hdr.size[1] = hdr.cols = new_width;
    hdr.flags = (hdr.flags & ~CV_MAT_CN_MASK) | ((new_cn-1) << CV_CN_SHIFT);
    hdr.step[1] = CV_ELEM_SIZE(hdr.flags);
    ଫେରନ୍ତୁ hdr;
}

#ifdef HAVE_OPENCL
ନାମାକାଶ {
ସ୍ଥିର ତାର୍କିକ ocl_setDiag(ଅଚଳ UMat& d, UMat& m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn = d.channels();
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth = d.depth();

    ଯଦି (depth == CV_64F && !ocl::Device::getDefault().doubleFPConfig())
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

    String opts = format("-D SET_DIAG -D T1=%s -D cn=%d -D IS_ROW_VECTOR=%d",
                         ocl::memopTypeToStr(depth),
                         cn,
                         (d.rows == 1) ? 1 : 0);

    ocl::Kernel k("setDiag", ocl::core::copyset_oclsrc, opts);
    ଯଦି (k.empty())
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

    k.args(ocl::KernelArg::WriteOnly(m),
           ocl::KernelArg::ReadOnlyNoSize(d),
           len);

    size_t globalsize[2] = { (size_t)len, (size_t)len };
    ଫେରନ୍ତୁ k.run(2, globalsize, ଖାଲି, ମିଥ୍ୟା);
}
}
#endif

UMat UMat::diag(ଅଚଳ UMat& d, UMatUsageFlags usageFlags)
{
    CV_INSTRUMENT_REGION();

    CV_Assert(d.cols == 1 || d.rows == 1);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା len = d.rows + d.cols - 1;

#ifdef HAVE_OPENCL
    ଯଦି (ocl::useOpenCL())
    {
        UMat m(len, len, d.type(), usageFlags);
        ଯଦି (ocl_setDiag(d, m, len))
        {
            CV_IMPL_ADD(CV_IMPL_OCL);
            ଫେରନ୍ତୁ m;
        }
    }
#endif

    UMat m(len, len, d.type(), Scalar(0), usageFlags);
    UMat md = m.diag();
    ଯଦି( d.cols == 1 )
        d.copyTo(md);
    ଅନ୍ୟଥା
        transpose(d, md);
    ଫେରନ୍ତୁ m;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା UMat::checkVector(ପୂର୍ଣ୍ଣସଂଖ୍ୟା _elemChannels, ପୂର୍ଣ୍ଣସଂଖ୍ୟା _depth, ତାର୍କିକ _requireContinuous) ଅଚଳ
{
    ଫେରନ୍ତୁ (depth() == _depth || _depth <= 0) &&
        (isContinuous() || !_requireContinuous) &&
        ((dims <= 2 && (((rows == 1 || cols == 1) && channels() == _elemChannels) ||
                        (cols == _elemChannels && channels() == 1))) ||
        (dims == 3 && channels() == 1 && size.p[2] == _elemChannels && (size.p[0] == 1 || size.p[1] == 1) &&
         (isContinuous() || step.p[1] == step.p[2]*size.p[2])))
    ? (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(total()*channels()/_elemChannels) : -1;
}

UMat UMat::reshape(ପୂର୍ଣ୍ଣସଂଖ୍ୟା _cn, ପୂର୍ଣ୍ଣସଂଖ୍ୟା _newndims, ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା* _newsz) ଅଚଳ
{
    ଯଦି(_newndims == dims)
    {
        ଯଦି(_newsz == 0)
            ଫେରନ୍ତୁ reshape(_cn);
        ଯଦି(_newndims == 2)
            ଫେରନ୍ତୁ reshape(_cn, _newsz[0]);
    }

    ଯଦି (isContinuous())
    {
        CV_Assert(_cn >= 0 && _newndims >= 0 && _newndims <= CV_MAX_DIM && (_newndims == 0 || _newsz != 0));

        ଯଦି (_cn == 0)
            _cn = ଏହି->channels();
        ଅନ୍ୟଥା
            CV_Assert(_cn <= CV_CN_MAX);

        size_t total_elem1_ref = ଏହି->total() * ଏହି->channels();
        size_t total_elem1 = _cn;

        AutoBuffer<ପୂର୍ଣ୍ଣସଂଖ୍ୟା, 4> newsz_buf( (size_t)std::max(_newndims, 1) );

        ପୂର୍ଣ୍ଣସଂଖ୍ୟା m1_idx = -1;

        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < _newndims; i++)
        {
            ଯଦି (_newsz[i] >= 0) {
                ଯଦି (_newsz[i] == 0 && i < dims)
                    newsz_buf[i] = size.p[i];
                ଅନ୍ୟଥା
                    newsz_buf[i] = _newsz[i];
                total_elem1 *= (size_t)newsz_buf[i];
            } ଅନ୍ୟଥା {
                ଯଦି (m1_idx >= 0)
                    CV_Error(cv::Error::StsBadSize, "More than one '-1' occured in the new shape");
                m1_idx = i;
            }
        }

        ଯଦି (m1_idx >= 0) {
            ଯଦି (total_elem1 == 0) {
                CV_Assert(total_elem1_ref == 0);
                total_elem1 = 1;
            }
            CV_Assert(total_elem1_ref % total_elem1 == 0);
            newsz_buf[m1_idx] = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(total_elem1_ref / total_elem1);
        } ଅନ୍ୟଥା ଯଦି (total_elem1 != total_elem1_ref) {
            CV_Error(cv::Error::StsUnmatchedSizes, "Requested and source matrices have different count of elements");
        }

        UMat hdr = *ଏହି;
        hdr.flags = (hdr.flags & ~CV_MAT_CN_MASK) | ((_cn-1) << CV_CN_SHIFT);
        setSize(hdr, _newndims, newsz_buf.data(), ଖାଲି, ସତ୍ୟ);

        ଫେରନ୍ତୁ hdr;
    }

    CV_Error(cv::Error::StsNotImplemented, "Reshaping of n-dimensional non-continuous matrices is not supported yet");
}

UMat UMat::reshape(ପୂର୍ଣ୍ଣସଂଖ୍ୟା _cn, ଅଚଳ MatShape& _newshape) ଅଚଳ
{
    ଯଦି (_newshape.dims < 0) {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା newshape[] = {0};
        ଫେରନ୍ତୁ reshape(_cn, 1, newshape);
    }
    ଫେରନ୍ତୁ reshape(_cn, _newshape.dims, _newshape.p);
}

Mat UMat::getMat(AccessFlag accessFlags) ଅଚଳ
{
    ଯଦି(!u)
        ଫେରନ୍ତୁ Mat();
    // TODO Support ACCESS_READ (ACCESS_WRITE) without unnecessary data transfers
    accessFlags |= ACCESS_RW;
    UMatDataAutoLock autolock(u);
    ଚେଷ୍ଟା
    {
        ଯଦି(CV_XADD(&u->refcount, 1) == 0)
            u->currAllocator->map(u, accessFlags);
        ଯଦି (u->data != 0)
        {
            Mat hdr(dims, size.p, type(), u->data + offset, step.p);
            hdr.flags = flags;
            hdr.u = u;
            hdr.datastart = u->data;
            hdr.data = u->data + offset;
            hdr.datalimit = hdr.dataend = u->data + u->size;
            ଫେରନ୍ତୁ hdr;
        }
    }
    ଧର(...)
    {
        CV_XADD(&u->refcount, -1);
        ଫିଙ୍ଗ;
    }
    CV_XADD(&u->refcount, -1);
    CV_Assert(u->data != 0 && "Error mapping of UMat to host memory.");
    ଫେରନ୍ତୁ Mat();
}

ଶୂନ୍ୟ* UMat::handle(AccessFlag accessFlags) ଅଚଳ
{
    ଯଦି( !u )
        ଫେରନ୍ତୁ 0;

    CV_Assert(u->refcount == 0);
    CV_Assert(!u->deviceCopyObsolete() || u->copyOnMap());
    ଯଦି (u->deviceCopyObsolete())
    {
        u->currAllocator->unmap(u);
    }

    ଯଦି (!!(accessFlags & ACCESS_WRITE))
        u->markHostCopyObsolete(ସତ୍ୟ);

    ଫେରନ୍ତୁ u->handle;
}

ଶୂନ୍ୟ UMat::ndoffset(size_t* ofs) ଅଚଳ
{
    // offset = step[0]*ofs[0] + step[1]*ofs[1] + step[2]*ofs[2] + ...;
    size_t val = offset;
    ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < dims; i++ )
    {
        size_t s = step.p[i];
        ofs[i] = val / s;
        val -= ofs[i]*s;
    }
}

ଶୂନ୍ୟ UMat::copyTo(OutputArray _dst) ଅଚଳ
{
    CV_INSTRUMENT_REGION();

#ifdef HAVE_CUDA
    ଯଦି (_dst.isGpuMat())
    {
        _dst.getGpuMat().upload(*ଏହି);
        ଫେରନ୍ତୁ;
    }
#endif

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା stype = type();
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା dtype = _dst.type();
    ଯଦି( _dst.fixedType() && dtype != stype )
    {
        CV_Assert( CV_MAT_CN(stype) == CV_MAT_CN(dtype) );
        convertTo( _dst, dtype );
        ଫେରନ୍ତୁ;
    }

    ଯଦି( dims == 0 && empty() )
    {
        _dst.release();
        ଶୂନ୍ୟ* obj = _dst.getObj();
        ଯଦି (_dst.isMat())
            reinterpret_cast<Mat*>(obj)->flags = Mat::MAGIC_VAL | Mat::CONTINUOUS_FLAG | stype;
        ଅନ୍ୟଥା ଯଦି (_dst.isUMat())
            reinterpret_cast<UMat*>(obj)->flags = UMat::MAGIC_VAL | UMat::CONTINUOUS_FLAG | stype;
        ଅନ୍ୟଥା ଯଦି (_dst.isGpuMat())
            reinterpret_cast<cuda::GpuMat*>(obj)->flags = stype;
        ଫେରନ୍ତୁ;
    }

    _dst.create( dims, size.p, stype );
    ଯଦି (empty())
        ଫେରନ୍ତୁ;

    size_t sz[CV_MAX_DIM] = {1}, srcofs[CV_MAX_DIM]={0}, dstofs[CV_MAX_DIM]={0};
    size_t esz = CV_ELEM_SIZE(stype);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, d = std::max(dims, 1);
    ପାଇଁ( i = 0; i < d; i++ )
        sz[i] = size.p[i];
    sz[d-1] *= esz;
    ndoffset(srcofs);
    srcofs[d-1] *= esz;

    ଯଦି( _dst.isUMat() )
    {
        UMat dst = _dst.getUMat();
        CV_Assert(dst.u);
        ଯଦି( u == dst.u && dst.offset == offset )
            ଫେରନ୍ତୁ;

        ଯଦି (u->currAllocator == dst.u->currAllocator)
        {
            dst.ndoffset(dstofs);
            dstofs[dims-1] *= esz;
            u->currAllocator->copy(u, dst.u, d, sz, srcofs, step.p, dstofs, dst.step.p, ମିଥ୍ୟା);
            ଫେରନ୍ତୁ;
        }

        // different allocators: never map a device destination, upload into it instead
        // (through a host Mat when both sides are device memory)
        MatAllocator* hostAllocator = Mat::getDefaultAllocator();
        ଯଦି (dst.u->currAllocator != hostAllocator)
        {
            ଯଦି (u->currAllocator == hostAllocator)
                getMat(ACCESS_READ).copyTo(_dst);
            ଅନ୍ୟଥା
            {
                Mat tmp;
                copyTo(tmp);
                tmp.copyTo(_dst);
            }
            ଫେରନ୍ତୁ;
        }
    }

    Mat dst = _dst.getMat();
    u->currAllocator->download(u, dst.ptr(), d, sz, srcofs, step.p, dst.step.p);
}

ଶୂନ୍ୟ UMat::copyTo(OutputArray _dst, ନିବେଶ_Array _mask) ଅଚଳ
{
    CV_INSTRUMENT_REGION();

    ଯଦି( _mask.empty() )
    {
        copyTo(_dst);
        ଫେରନ୍ତୁ;
    }
#ifdef HAVE_OPENCL
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn = channels(), mtype = _mask.type(), mdepth = CV_MAT_DEPTH(mtype), mcn = CV_MAT_CN(mtype);
    CV_Assert( (mdepth == CV_8U || mdepth == CV_8S || mdepth == CV_Bool) && (mcn == 1 || mcn == cn) );

    ଯଦି (ocl::useOpenCL() && _dst.isUMat() && dims <= 2)
    {
        UMatData * prevu = _dst.getUMat().u;
        _dst.create( size, type() );

        UMat dst = _dst.getUMat();

        ତାର୍କିକ haveDstUninit = ମିଥ୍ୟା;
        ଯଦି( prevu != dst.u ) // do not leave dst uninitialized
            haveDstUninit = ସତ୍ୟ;

        String opts = format("-D COPY_TO_MASK -D T1=%s -D scn=%d -D mcn=%d%s",
                             ocl::memopTypeToStr(depth()), cn, mcn,
                             haveDstUninit ? " -D HAVE_DST_UNINIT" : "");

        ocl::Kernel k("copyToMask", ocl::core::copyset_oclsrc, opts);
        ଯଦି (!k.empty())
        {
            k.args(ocl::KernelArg::ReadOnlyNoSize(*ଏହି),
                   ocl::KernelArg::ReadOnlyNoSize(_mask.getUMat()),
                   haveDstUninit ? ocl::KernelArg::WriteOnly(dst) :
                                   ocl::KernelArg::ReadWrite(dst));

            size_t globalsize[2] = { (size_t)cols, (size_t)rows };
            ଯଦି (k.run(2, globalsize, ଖାଲି, ମିଥ୍ୟା))
            {
                CV_IMPL_ADD(CV_IMPL_OCL);
                ଫେରନ୍ତୁ;
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

UMat& UMat::setTo(ନିବେଶ_Array ମୂଲ୍ୟ, ନିବେଶ_Array _mask)
{
    CV_INSTRUMENT_REGION();

    ତାର୍କିକ haveMask = !_mask.empty();
#ifdef HAVE_OPENCL
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା tp = type(), cn = CV_MAT_CN(tp), d = CV_MAT_DEPTH(tp);

    ଯଦି( dims <= 2 && cn <= 4 && CV_MAT_DEPTH(tp) < CV_64F && ocl::useOpenCL() )
    {
        Mat value = ମୂଲ୍ୟ.getMat();
        CV_Assert( checkScalar(value, type(), ମୂଲ୍ୟ.kind(), ନିବେଶ_Array_2::UMAT) );
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା kercn = haveMask || cn == 3 ? cn : std::max(cn, ocl::predictOptimalVectorWidth(*ଏହି)),
                kertp = CV_MAKE_TYPE(d, kercn);

        ଦ୍ୱିଗୁଣ buf[16] = { 0, 0, 0, 0, 0, 0, 0, 0,
                           0, 0, 0, 0, 0, 0, 0, 0 };
        convertAndUnrollScalar(value, tp, (uchar *)buf, kercn / cn);

        ପୂର୍ଣ୍ଣସଂଖ୍ୟା scalarcn = kercn == 3 ? 4 : kercn, rowsPerWI = ocl::Device::getDefault().isIntel() ? 4 : 1;
        String opts = format("-D dstT=%s -D rowsPerWI=%d -D dstST=%s -D dstT1=%s -D cn=%d",
                             ocl::memopTypeToStr(kertp), rowsPerWI,
                             ocl::memopTypeToStr(CV_MAKETYPE(d, scalarcn)),
                             ocl::memopTypeToStr(d), kercn);

        ocl::Kernel setK(haveMask ? "setMask" : "set", ocl::core::copyset_oclsrc, opts);
        ଯଦି( !setK.empty() )
        {
            ocl::KernelArg scalararg(ocl::KernelArg::CONSTANT, 0, 0, 0, buf, CV_ELEM_SIZE(d) * scalarcn);
            UMat mask;

            ଯଦି( haveMask )
            {
                mask = _mask.getUMat();
                CV_Assert( mask.size() == size() && (mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool) );
                ocl::KernelArg maskarg = ocl::KernelArg::ReadOnlyNoSize(mask),
                        dstarg = ocl::KernelArg::ReadWrite(*ଏହି);
                setK.args(maskarg, dstarg, scalararg);
            }
            ଅନ୍ୟଥା
            {
                ocl::KernelArg dstarg = ocl::KernelArg::WriteOnly(*ଏହି, cn, kercn);
                setK.args(dstarg, scalararg);
            }

            size_t globalsize[] = { (size_t)cols * cn / kercn, ((size_t)rows + rowsPerWI - 1) / rowsPerWI };
            ଯଦି( setK.run(2, globalsize, ଖାଲି, ମିଥ୍ୟା) )
            {
                CV_IMPL_ADD(CV_IMPL_OCL);
                ଫେରନ୍ତୁ *ଏହି;
            }
        }
    }
#endif
    Mat m = getMat(haveMask ? ACCESS_RW : ACCESS_WRITE);
    m.setTo(ମୂଲ୍ୟ, _mask);
    ଫେରନ୍ତୁ *ଏହି;
}

UMat& UMat::operator = (ଅଚଳ Scalar& s)
{
    setTo(s);
    ଫେରନ୍ତୁ *ଏହି;
}

UMat UMat::t() ଅଚଳ
{
    UMat m;
    transpose(*ଏହି, m);
    ଫେରନ୍ତୁ m;
}

UMat UMat::zeros(ପୂର୍ଣ୍ଣସଂଖ୍ୟା rows, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cols, ପୂର୍ଣ୍ଣସଂଖ୍ୟା type, UMatUsageFlags usageFlags)
{
    ଫେରନ୍ତୁ UMat(rows, cols, type, Scalar::all(0), usageFlags);
}

UMat UMat::zeros(Size size, ପୂର୍ଣ୍ଣସଂଖ୍ୟା type, UMatUsageFlags usageFlags)
{
    ଫେରନ୍ତୁ UMat(size, type, Scalar::all(0), usageFlags);
}

UMat UMat::zeros(ପୂର୍ଣ୍ଣସଂଖ୍ୟା ndims, ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା* sz, ପୂର୍ଣ୍ଣସଂଖ୍ୟା type, UMatUsageFlags usageFlags)
{
    ଫେରନ୍ତୁ UMat(ndims, sz, type, Scalar::all(0), usageFlags);
}

UMat UMat::zeros(ଅଚଳ MatShape& shape, ପୂର୍ଣ୍ଣସଂଖ୍ୟା type, UMatUsageFlags usageFlags)
{
    ଫେରନ୍ତୁ UMat(shape.dims, shape.p, type, Scalar::all(0), usageFlags);
}

UMat UMat::ones(ପୂର୍ଣ୍ଣସଂଖ୍ୟା rows, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cols, ପୂର୍ଣ୍ଣସଂଖ୍ୟା type, UMatUsageFlags usageFlags)
{
    ଫେରନ୍ତୁ UMat(rows, cols, type, Scalar(1), usageFlags);
}

UMat UMat::ones(Size size, ପୂର୍ଣ୍ଣସଂଖ୍ୟା type, UMatUsageFlags usageFlags)
{
    ଫେରନ୍ତୁ UMat(size, type, Scalar(1), usageFlags);
}

UMat UMat::ones(ପୂର୍ଣ୍ଣସଂଖ୍ୟା ndims, ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା* sz, ପୂର୍ଣ୍ଣସଂଖ୍ୟା type, UMatUsageFlags usageFlags)
{
    ଫେରନ୍ତୁ UMat(ndims, sz, type, Scalar(1), usageFlags);
}

UMat UMat::ones(ଅଚଳ MatShape& shape, ପୂର୍ଣ୍ଣସଂଖ୍ୟା type, UMatUsageFlags usageFlags)
{
    ଫେରନ୍ତୁ UMat(shape.dims, shape.p, type, Scalar(1), usageFlags);
}

}

/* End of file. */
