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
فراغ setSize(UMat& m, صحيح _dims, ثابت صحيح* _sz, ثابت size_t* _steps,
             منطقي autoSteps = خطأ);

فراغ updateContinuityFlag(UMat& m);
فراغ finalizeHdr(UMat& m);

UMatData::UMatData(ثابت MatAllocator* allocator)
{
    prevAllocator = currAllocator = allocator;
    urefcount = refcount = mapcount = 0;
    data = origdata = 0;
    size = 0;
    flags = static_cast<UMatData::MemoryFlag>(0);
    handle = 0;
    userdata = 0;
    allocatorFlags_ = 0;
    originalUMatData = عدم;
}

UMatData::~UMatData()
{
    prevAllocator = currAllocator = 0;
    urefcount = refcount = 0;
    CV_Assert(mapcount == 0);
    data = origdata = 0;
    size = 0;
    منطقي isAsyncCleanup = !!(flags & UMatData::ASYNC_CLEANUP);
    flags = static_cast<UMatData::MemoryFlag>(0);
    handle = 0;
    userdata = 0;
    allocatorFlags_ = 0;
    𐤀𐤌 (originalUMatData)
    {
        منطقي showWarn = خطأ;
        UMatData* u = originalUMatData;
        منطقي zero_Ref = CV_XADD(&(u->refcount), -1) == 1;
        𐤀𐤌 (zero_Ref)
        {
            // simulate Mat::deallocate
            𐤀𐤌 (u->mapcount != 0)
            {
                (u->currAllocator ? u->currAllocator : /* TODO allocator ? allocator :*/ Mat::getDefaultAllocator())->unmap(u);
            }
            وإلا
            {
                // we don't do "map", so we can't do "unmap"
            }
        }
        منطقي zero_URef = CV_XADD(&(u->urefcount), -1) == 1;
        𐤀𐤌 (zero_Ref && !zero_URef)
            showWarn = true;
        𐤀𐤌 (zero_Ref && zero_URef) // oops, we need to free resources
        {
            showWarn = !isAsyncCleanup;
            // simulate UMat::deallocate
            u->currAllocator->deallocate(u);
        }
#ifndef NDEBUG
        𐤀𐤌 (showWarn)
        {
            ساكن صحيح warn_message_showed = 0;
            𐤀𐤌 (warn_message_showed++ < 100)
            {
                fflush(stdout);
                fprintf(stderr, "\n! OPENCV warning: getUMat()/getMat() call chain possible problem."
                                "\n!                 Base object is dead, while nested/derived object is still alive or processed."
                                "\n!                 Please check lifetime of UMat/Mat objects!\n");
                fflush(stderr);
            }
        }
#وإلا
        CV_UNUSED(showWarn);
#endif
        originalUMatData = عدم;
    }
}

#ifndef OPENCV_DISABLE_THREAD_SUPPORT

// it should be a prime number for the best hash function
تعداد { UMAT_NLOCKS = 31 };
ساكن Mutex umatLocks[UMAT_NLOCKS];

ساكن size_t getUMatDataLockIndex(ثابت UMatData* u)
{
    size_t idx = ((size_t)(فراغ*)u) % UMAT_NLOCKS;
    أرجع idx;
}

فراغ UMatData::lock()
{
    size_t idx = getUMatDataLockIndex(هذا);
    //printf("%d lock(%d)\n", cv::utils::getThreadID(), (int)idx);
    umatLocks[idx].lock();
}

فراغ UMatData::unlock()
{
    size_t idx = getUMatDataLockIndex(هذا);
    //printf("%d unlock(%d)\n", cv::utils::getThreadID(), (int)idx);
    umatLocks[idx].unlock();
}


// Do not allow several lock() calls with different UMatData objects.
بنية UMatDataAutoLocker
{
    صحيح usage_count;
    UMatData* locked_objects[2];
    UMatDataAutoLocker() : usage_count(0) { locked_objects[0] = عدم; locked_objects[1] = عدم; }

    فراغ lock(UMatData*& u1)
    {
        منطقي locked_1 = (u1 == locked_objects[0] || u1 == locked_objects[1]);
        𐤀𐤌 (locked_1)
        {
            u1 = عدم;
            أرجع;
        }
        CV_Assert(usage_count == 0);  // UMatDataAutoLock can't be used multiple times from the same thread
        usage_count = 1;
        locked_objects[0] = u1;
        u1->lock();
    }
    فراغ lock(UMatData*& u1, UMatData*& u2)
    {
        منطقي locked_1 = (u1 == locked_objects[0] || u1 == locked_objects[1]);
        منطقي locked_2 = (u2 == locked_objects[0] || u2 == locked_objects[1]);
        𐤀𐤌 (locked_1)
            u1 = عدم;
        𐤀𐤌 (locked_2)
            u2 = عدم;
        𐤀𐤌 (locked_1 && locked_2)
            أرجع;
        CV_Assert(usage_count == 0);  // UMatDataAutoLock can't be used multiple times from the same thread
        usage_count = 1;
        locked_objects[0] = u1;
        locked_objects[1] = u2;
        𐤀𐤌 (u1)
            u1->lock();
        𐤀𐤌 (u2)
            u2->lock();
    }
    فراغ release(UMatData* u1, UMatData* u2)
    {
        𐤀𐤌 (u1 == عدم && u2 == عدم)
            أرجع;
        CV_Assert(usage_count == 1);
        usage_count = 0;
        𐤀𐤌 (u1)
            u1->unlock();
        𐤀𐤌 (u2)
            u2->unlock();
        locked_objects[0] = عدم; locked_objects[1] = عدم;
    }
};
ساكن TLSData<UMatDataAutoLocker>& getUMatDataAutoLockerTLS()
{
    CV_SINGLETON_LAZY_INIT_REF(TLSData<UMatDataAutoLocker>, جديد TLSData<UMatDataAutoLocker>());
}
ساكن UMatDataAutoLocker& getUMatDataAutoLocker() { أرجع getUMatDataAutoLockerTLS().getRef(); }


UMatDataAutoLock::UMatDataAutoLock(UMatData* u) : u1(u), u2(عدم)
{
    getUMatDataAutoLocker().lock(u1);
}
UMatDataAutoLock::UMatDataAutoLock(UMatData* u1_, UMatData* u2_) : u1(u1_), u2(u2_)
{
    𐤀𐤌 (getUMatDataLockIndex(u1) > getUMatDataLockIndex(u2))
    {
        std::swap(u1, u2);
    }
    getUMatDataAutoLocker().lock(u1, u2);
}
UMatDataAutoLock::~UMatDataAutoLock()
{
    getUMatDataAutoLocker().release(u1, u2);
}

#وإلا

فراغ UMatData::lock()
{
    // nothing in OPENCV_DISABLE_THREAD_SUPPORT mode
}

فراغ UMatData::unlock()
{
    // nothing in OPENCV_DISABLE_THREAD_SUPPORT mode
}

UMatDataAutoLock::UMatDataAutoLock(UMatData* u) : u1(u), u2(عدم)
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

UMat::UMat(صحيح _rows, صحيح _cols, صحيح _type, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_rows, _cols, _type);
}

UMat::UMat(صحيح _rows, صحيح _cols, صحيح _type, ثابت Scalar& _s, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_rows, _cols, _type);
    *هذا = _s;
}

UMat::UMat(Size _sz, صحيح _type, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create( _sz.height, _sz.width, _type );
}

UMat::UMat(Size _sz, صحيح _type, ثابت Scalar& _s, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_sz.height, _sz.width, _type);
    *هذا = _s;
}

UMat::UMat(صحيح _dims, ثابت صحيح* _sz, صحيح _type, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_dims, _sz, _type);
}

UMat::UMat(صحيح _dims, ثابت صحيح* _sz, صحيح _type, ثابت Scalar& _s, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_dims, _sz, _type);
    *هذا = _s;
}

UMat::UMat(ثابت MatShape& _shape, صحيح _type, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_shape.dims, _shape.p, _type);
}

UMat::UMat(ثابت MatShape& _shape, صحيح _type, ثابت Scalar& _s, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_shape.dims, _shape.p, _type);
    *هذا = _s;
}

UMat::UMat(ثابت UMat& m)
: flags(m.flags), dims(m.dims), rows(m.rows), cols(m.cols), allocator(m.allocator),
  usageFlags(m.usageFlags), u(m.u), offset(m.offset), size(m.size), step(m.step)
{
    addref();
}

UMat& UMat::operator=(ثابت UMat& m)
{
    𐤀𐤌( هذا != &m )
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
    أرجع *هذا;
}

UMat UMat::clone() ثابت
{
    UMat m;
    copyTo(m);
    أرجع m;
}

فراغ UMat::assignTo(UMat& m, صحيح _type) ثابت
{
    𐤀𐤌( _type < 0 )
        m = *هذا;
    وإلا
        convertTo(m, _type);
}

فراغ UMat::create(صحيح _rows, صحيح _cols, صحيح _type, UMatUsageFlags _usageFlags)
{
    صحيح sz[] = {_rows, _cols};
    create(2, sz, _type, _usageFlags);
}

فراغ UMat::create(Size _sz, صحيح _type, UMatUsageFlags _usageFlags)
{
    create(_sz.height, _sz.width, _type, _usageFlags);
}

فراغ UMat::createSameSize(InputArray arr, صحيح _type, UMatUsageFlags _usageFlags)
{
    create(arr.shape(), _type, _usageFlags);
}

فراغ UMat::addref()
{
    𐤀𐤌( u )
        CV_XADD(&(u->urefcount), 1);
}

فراغ UMat::release()
{
    𐤀𐤌( u && CV_XADD(&(u->urefcount), -1) == 1 )
        deallocate();
    u = 0;
    size.clear();
    dims = cols = rows = 0;
}

منطقي UMat::empty() ثابت
{
    أرجع u == 0 || total() == 0;
}

size_t UMat::total() ثابت
{
    𐤀𐤌( dims <= 2 )
        أرجع (size_t)rows * cols;
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
: flags(m.flags), dims(m.dims), rows(m.rows), cols(m.cols), allocator(m.allocator),
  usageFlags(m.usageFlags), u(m.u), offset(m.offset)
{
    size = m.size;
    step = m.step;
    m.flags = MAGIC_VAL;
    m.usageFlags = USAGE_DEFAULT;
    m.dims = m.rows = m.cols = 0;
    m.size.clear();
    m.allocator = عدم;
    m.u = عدم;
    m.offset = 0;
}

UMat& UMat::operator=(UMat&& m)
{
    𐤀𐤌 (هذا == &m)
      أرجع *هذا;
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
    m.allocator = عدم;
    m.u = عدم;
    m.offset = 0;
    أرجع *هذا;
}


MatAllocator* UMat::getStdAllocator()
{
#ifdef HAVE_OPENCL
    𐤀𐤌 (ocl::useOpenCL())
        أرجع ocl::getOpenCLAllocator();
#endif
    أرجع Mat::getDefaultAllocator();
}

فراغ swap( UMat& a, UMat& b )
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


فراغ setSize( UMat& m, صحيح _dims, ثابت صحيح* _sz,
              ثابت size_t* _steps, منطقي autoSteps )
{
    CV_Assert( 0 <= _dims && _dims <= CV_MAX_DIM && _dims <= MatShape::MAX_DIMS);

    m.dims = _dims;
    m.size = MatShape(_dims, _sz);
    m.step[std::max(_dims-1, 0)] = CV_ELEM_SIZE(m.flags);
    لكل (صحيح i = _dims-2; i >= 0; i--) {
        size_t autostep = m.size[i+1]*m.step[i+1];
        𐤀𐤌 (_steps) {
            m.step[i] = _steps[i];
            //CV_Assert(m.step[i] >= autostep);
        } وإلا 𐤀𐤌 (autoSteps) {
            m.step[i] = autostep;
        } وإلا {
            m.step[i] = 0;
        }
    }

    𐤀𐤌( _dims <= 2 )
    {
        m.cols = _dims == 0 ? 1 : _sz ? _sz[_dims > 1] : 0;
        m.rows = _dims < 2 ? 1 : _sz ? _sz[0] : 0;
    } وإلا {
        m.cols = m.rows = -1;
    }
}


فراغ UMat::updateContinuityFlag()
{
    flags = cv::updateContinuityFlag(flags, dims, size.p, step.p);
}


فراغ finalizeHdr(UMat& m)
{
    m.updateContinuityFlag();
    صحيح d = m.dims;
    𐤀𐤌( d > 2 )
        m.rows = m.cols = -1;
}


UMat Mat::getUMat(AccessFlag accessFlags, UMatUsageFlags usageFlags) ثابت
{
    UMat hdr;
    𐤀𐤌(!data)
        أرجع hdr;
    𐤀𐤌 (data != datastart)
    {
        Size wholeSize;
        Point ofs;
        locateROI(wholeSize, ofs);
        Size sz(cols, rows);
        𐤀𐤌 (ofs.x != 0 || ofs.y != 0)
        {
            Mat src = *هذا;
            صحيح dtop = ofs.y;
            صحيح dbottom = wholeSize.height - src.rows - ofs.y;
            صحيح dleft = ofs.x;
            صحيح dright = wholeSize.width - src.cols - ofs.x;
            src.adjustROI(dtop, dbottom, dleft, dright);
            أرجع src.getUMat(accessFlags, usageFlags)(cv::Rect(ofs.x, ofs.y, sz.width, sz.height));
        }
    }
    CV_Assert(data == datastart);

    accessFlags |= ACCESS_RW;
    UMatData* new_u = عدم;
    MatStep new_step = step;
    {
        MatAllocator *a = allocator, *a0 = getDefaultAllocator();
        𐤀𐤌(!a)
            a = a0;
        new_u = a->allocate(dims, size.p, type(), data, new_step.p, accessFlags, usageFlags);
        new_u->originalUMatData = u;
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
    𐤀𐤌 (!allocated)
    {
        allocated = getDefaultAllocator()->allocate(new_u, accessFlags, usageFlags);
        CV_Assert(allocated);
    }
    𐤀𐤌 (u != عدم)
    {
#ifdef HAVE_OPENCL
        𐤀𐤌 (ocl::useOpenCL() && new_u->currAllocator == ocl::getOpenCLAllocator())
        {
            CV_Assert(new_u->tempUMat());
        }
#endif
        CV_XADD(&(u->refcount), 1);
        CV_XADD(&(u->urefcount), 1);
    }
    حاول
    {
        hdr.flags = flags;
        hdr.usageFlags = usageFlags;
        setSize(hdr, dims, size.p, new_step.p);
        finalizeHdr(hdr);
        hdr.u = new_u;
        hdr.offset = 0; //data - datastart;
        hdr.addref();
        أرجع hdr;
    }
    امسك(...)
    {
        𐤀𐤌 (u != عدم)
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
    𐤀𐤌 (d == 0) {
        d = 1;
        _sizes = (ثابت صحيح*)&sz1;
    }
    CV_Assert(0 <= d && d <= CV_MAX_DIM && _sizes);
    _type = CV_MAT_TYPE(_type);

    // if param value is USAGE_DEFAULT by implicit default param value -or- explicit value
    // ...then don't change the existing usageFlags
    // it is not possible to change usage from non-default to USAGE_DEFAULT through create()
    // ...instead must construct UMat()
    𐤀𐤌 (_usageFlags == cv::USAGE_DEFAULT)
    {
        _usageFlags = usageFlags;
    }

    𐤀𐤌( u && d == dims && _type == type() && _usageFlags == usageFlags )
    {
        لكل( i = 0; i < d; i++ )
            𐤀𐤌( size[i] != _sizes[i] )
                اكسر;
        𐤀𐤌( i == d )
            أرجع;
    }

    صحيح _sizes_backup[CV_MAX_DIM]; // #5991
    𐤀𐤌 (_sizes == (هذا->size.p))
    {
        لكل(i = 0; i < d; i++ )
            _sizes_backup[i] = _sizes[i];
        _sizes = _sizes_backup;
    }

    release();
    usageFlags = _usageFlags;
    𐤀𐤌( d == 0 )
        أرجع;
    flags = (_type & CV_MAT_TYPE_MASK) | MAGIC_VAL;
    setSize(*هذا, d, _sizes, 0, true);
    offset = 0;

    𐤀𐤌( total() > 0 )
    {
        MatAllocator *a = allocator, *a0 = getStdAllocator();
        𐤀𐤌 (!a)
        {
            a = a0;
            a0 = Mat::getDefaultAllocator();
        }
        حاول
        {
            u = a->allocate(dims, size.p, _type, 0, step.p, ACCESS_RW /* ignored */, usageFlags);
            CV_Assert(u != 0);
        }
        امسك(...)
        {
            𐤀𐤌(a != a0)
                u = a0->allocate(dims, size.p, _type, 0, step.p, ACCESS_RW /* ignored */, usageFlags);
            CV_Assert(u != 0);
        }
        CV_Assert( step[dims-1] == (size_t)CV_ELEM_SIZE(flags) );
    }

    finalizeHdr(*هذا);
    addref();
    size.dims = dims = d0;
}

فراغ UMat::create(ثابت std::vector<صحيح>& _sizes, صحيح _type, UMatUsageFlags _usageFlags)
{
    create((صحيح)_sizes.size(), _sizes.data(), _type, _usageFlags);
}

فراغ UMat::create(ثابت MatShape& _shape, صحيح _type, UMatUsageFlags _usageFlags)
{
    𐤀𐤌 (_shape.dims < 0) {
        release();
    } وإلا {
        create(_shape.dims, _shape.p, _type, _usageFlags);
        size.layout = _shape.layout;
        size.C = _shape.C;
    }
}

فراغ UMat::fit(صحيح _dims, ثابت صحيح* _sizes, صحيح _type, UMatUsageFlags _usageFlags)
{
    𐤀𐤌 (_usageFlags == cv::USAGE_DEFAULT)
        _usageFlags = usageFlags;
    size_t oldTotalBytes = u ? u->size : 0;
    size_t esz = CV_ELEM_SIZE(_type), newTotal = _dims >= 0;
    لكل (صحيح i = 0; i < _dims; i++)
        newTotal *= _sizes[i];
    size_t newTotalBytes = newTotal*esz;
    𐤀𐤌 (newTotalBytes > 0 &&
        (!isContinuous() ||
         newTotalBytes > oldTotalBytes ||
         offset != 0 ||
         _usageFlags != usageFlags)) {
        create(_dims, _sizes, _type, _usageFlags);
    } وإلا {
        flags = (flags & ~Mat::TYPE_MASK) | CV_MAT_TYPE(_type);
        صحيح _dummy_size = 0;
        setSize(*هذا, (_dims >= 0 ? _dims : 1), (_dims >= 0 ? _sizes : &_dummy_size), nullptr, true);
        finalizeHdr(*هذا);
    }
}

فراغ UMat::fit(ثابت std::vector<صحيح>& _shape, صحيح _type, UMatUsageFlags _usageFlags)
{
    fit((صحيح)_shape.size(), _shape.data(), _type, _usageFlags);
}

فراغ UMat::fit(ثابت MatShape& _shape, صحيح _type, UMatUsageFlags _usageFlags)
{
    fit(_shape.dims, _shape.p, _type, _usageFlags);
    size.layout = _shape.layout;
    size.C = _shape.C;
}

فراغ UMat::fit(صحيح _rows, صحيح _cols, صحيح _type, UMatUsageFlags _usageFlags)
{
    _type &= TYPE_MASK;
    صحيح sz[] = {_rows, _cols};
    fit(2, sz, _type, _usageFlags);
}

فراغ UMat::fit(Size _sz, صحيح _type, UMatUsageFlags _usageFlags)
{
    fit(_sz.height, _sz.width, _type, _usageFlags);
}

فراغ UMat::fitSameSize(InputArray m, صحيح _type, UMatUsageFlags _usageFlags)
{
    fit(m.shape(), _type, _usageFlags);
}

فراغ UMat::copySize(ثابت UMat& m)
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

فراغ UMat::deallocate()
{
    UMatData* u_ = u;
    u = عدم;
    u_->currAllocator->deallocate(u_);
}


UMat::UMat(ثابت UMat& m, ثابت Range& _rowRange, ثابت Range& _colRange)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(USAGE_DEFAULT), u(0), offset(0)
{
    CV_Assert( m.dims >= 2 && m.dims <= MatShape::MAX_DIMS );
    𐤀𐤌( m.dims > 2 )
    {
        Range rs[MatShape::MAX_DIMS];
        rs[0] = _rowRange;
        rs[1] = _colRange;
        لكل( صحيح i = 2; i < m.dims; i++ )
            rs[i] = Range::all();
        *هذا = m(rs);
        أرجع;
    }

    *هذا = m;
    𐤀𐤌( _rowRange != Range::all() && _rowRange != Range(0,rows) )
    {
        CV_Assert( 0 <= _rowRange.start &&
                  _rowRange.start <= _rowRange.end &&
                  _rowRange.end <= m.rows );
        size[0] = rows = _rowRange.size();
        offset += step*_rowRange.start;
        flags |= SUBMATRIX_FLAG;
    }

    𐤀𐤌( _colRange != Range::all() && _colRange != Range(0,cols) )
    {
        CV_Assert( 0 <= _colRange.start &&
                  _colRange.start <= _colRange.end &&
                  _colRange.end <= m.cols );
        size[1] = cols = _colRange.size();
        offset += _colRange.start*elemSize();
        flags |= SUBMATRIX_FLAG;
    }

    updateContinuityFlag();

    𐤀𐤌( rows <= 0 || cols <= 0 )
    {
        release();
        rows = cols = 0;
    }
}


UMat::UMat(ثابت UMat& m, ثابت Rect& roi)
    : flags(m.flags), dims(2), rows(roi.height), cols(roi.width),
    allocator(m.allocator), usageFlags(m.usageFlags), u(m.u),
    offset(m.offset + roi.y*m.step[0]), size(2)
{
    CV_Assert( m.dims <= 2 );

    size_t esz = CV_ELEM_SIZE(flags);
    offset += roi.x*esz;
    CV_Assert( 0 <= roi.x && 0 <= roi.width && roi.x + roi.width <= m.cols &&
              0 <= roi.y && 0 <= roi.height && roi.y + roi.height <= m.rows );
    𐤀𐤌( roi.width < m.cols || roi.height < m.rows )
        flags |= SUBMATRIX_FLAG;

    size[0] = rows;
    size[1] = cols;
    step[0] = m.dims == 2 ? m.step[0] : cols*esz;
    step[1] = esz;

    updateContinuityFlag();

    addref();
    𐤀𐤌( rows <= 0 || cols <= 0 )
    {
        rows = cols = 0;
        release();
    }
}


UMat::UMat(ثابت UMat& m, ثابت Range* ranges)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(USAGE_DEFAULT), u(0), offset(0)
{
    صحيح i, d = m.dims;

    CV_Assert(ranges);
    لكل( i = 0; i < d; i++ )
    {
        Range r = ranges[i];
        CV_Assert( r == Range::all() || (0 <= r.start && r.start < r.end && r.end <= m.size[i]) );
    }
    *هذا = m;
    لكل( i = 0; i < d; i++ )
    {
        Range r = ranges[i];
        𐤀𐤌( r != Range::all() && r != Range(0, size.p[i]))
        {
            size.p[i] = r.end - r.start;
            offset += r.start*step.p[i];
            flags |= SUBMATRIX_FLAG;
        }
    }

    𐤀𐤌 (d <= 2) {
        rows = d == 2 ? size[0] : 1;
        cols = d <= 0 ? (d >= 0) : size[d > 1];
    }
    updateContinuityFlag();
}

UMat::UMat(ثابت UMat& m, ثابت std::vector<Range>& ranges)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(USAGE_DEFAULT), u(0), offset(0)
{
    صحيح i, d = m.dims;

    CV_Assert((صحيح)ranges.size() == d);
    لكل (i = 0; i < d; i++)
    {
        Range r = ranges[i];
        CV_Assert(r == Range::all() || (0 <= r.start && r.start < r.end && r.end <= m.size[i]));
    }
    *هذا = m;
    لكل (i = 0; i < d; i++)
    {
        Range r = ranges[i];
        𐤀𐤌 (r != Range::all() && r != Range(0, size.p[i]))
        {
            size.p[i] = r.end - r.start;
            offset += r.start*step.p[i];
            flags |= SUBMATRIX_FLAG;
        }
    }

    𐤀𐤌 (d <= 2) {
        rows = d == 2 ? size[0] : 1;
        cols = d <= 0 ? (d >= 0) : size[d > 1];
    }
    updateContinuityFlag();
}

UMat UMat::diag(صحيح d) ثابت
{
    CV_Assert( dims <= 2 );
    UMat m = *هذا;
    size_t esz = elemSize();
    صحيح len;

    𐤀𐤌( d >= 0 )
    {
        len = std::min(cols - d, rows);
        m.offset += esz*d;
    }
    وإلا
    {
        len = std::min(rows + d, cols);
        m.offset -= step[0]*d;
    }
    CV_DbgAssert( len > 0 );

    m.size[0] = m.rows = len;
    m.size[1] = m.cols = 1;
    m.step[0] += (len > 1 ? esz : 0);

    m.updateContinuityFlag();

    𐤀𐤌( size() != Size(1,1) )
        m.flags |= SUBMATRIX_FLAG;

    أرجع m;
}

فراغ UMat::locateROI( Size& wholeSize, Point& ofs ) ثابت
{
    CV_Assert( dims <= 2 && step[0] > 0 );
    size_t esz = elemSize(), minstep;
    ptrdiff_t delta1 = (ptrdiff_t)offset, delta2 = (ptrdiff_t)u->size;

    𐤀𐤌( delta1 == 0 )
        ofs.x = ofs.y = 0;
    وإلا
    {
        ofs.y = (صحيح)(delta1/step[0]);
        ofs.x = (صحيح)((delta1 - step[0]*ofs.y)/esz);
        CV_DbgAssert( offset == (size_t)(ofs.y*step[0] + ofs.x*esz) );
    }
    minstep = (ofs.x + cols)*esz;
    wholeSize.height = (صحيح)((delta2 - minstep)/step[0] + 1);
    wholeSize.height = std::max(wholeSize.height, ofs.y + rows);
    wholeSize.width = (صحيح)((delta2 - step*(wholeSize.height-1))/esz);
    wholeSize.width = std::max(wholeSize.width, ofs.x + cols);
}


UMat& UMat::adjustROI( صحيح dtop, صحيح dbottom, صحيح dleft, صحيح dright )
{
    CV_Assert( dims == 2 && step[0] > 0 );
    Size wholeSize; Point ofs;
    size_t esz = elemSize();
    locateROI( wholeSize, ofs );
    صحيح row1 = std::min(std::max(ofs.y - dtop, 0), wholeSize.height), row2 = std::max(0, std::min(ofs.y + rows + dbottom, wholeSize.height));
    صحيح col1 = std::min(std::max(ofs.x - dleft, 0), wholeSize.width), col2 = std::max(0, std::min(ofs.x + cols + dright, wholeSize.width));
    𐤀𐤌(row1 > row2)
        std::swap(row1, row2);
    𐤀𐤌(col1 > col2)
        std::swap(col1, col2);

    offset += (row1 - ofs.y)*step + (col1 - ofs.x)*esz;
    rows = row2 - row1; cols = col2 - col1;
    size.p[0] = rows; size.p[1] = cols;
    updateContinuityFlag();
    أرجع *هذا;
}


UMat UMat::reshape(صحيح new_cn, صحيح new_rows) ثابت
{
    صحيح cn = channels();
    UMat hdr = *هذا;

    𐤀𐤌( dims > 2 && new_rows == 0 && new_cn != 0 && size[dims-1]*cn % new_cn == 0 )
    {
        hdr.flags = (hdr.flags & ~CV_MAT_CN_MASK) | ((new_cn-1) << CV_CN_SHIFT);
        hdr.step[dims-1] = CV_ELEM_SIZE(hdr.flags);
        hdr.size[dims-1] = hdr.size[dims-1]*cn / new_cn;
        أرجع hdr;
    }

    CV_Assert( dims <= 2 );

    𐤀𐤌( new_cn == 0 )
        new_cn = cn;

    صحيح total_width = cols * cn;

    𐤀𐤌( (new_cn > total_width || total_width % new_cn != 0) && new_rows == 0 )
        new_rows = rows * total_width / new_cn;

    𐤀𐤌( new_rows != 0 && new_rows != rows )
    {
        صحيح total_size = total_width * rows;
        𐤀𐤌( !isContinuous() )
            CV_Error( cv::Error::BadStep,
            "The matrix is not continuous, thus its number of rows can not be changed" );

        total_width = total_size / new_rows;

        𐤀𐤌( total_width * new_rows != total_size )
            CV_Error( cv::Error::StsBadArg, "The total number of matrix elements "
                                    "is not divisible by the new number of rows" );

        hdr.size[0] = hdr.rows = new_rows;
        hdr.step[0] = total_width * elemSize1();
    } وإلا {
        hdr.size[0] = hdr.rows = rows;
        𐤀𐤌 (dims <= 1)
            hdr.step[0] = cols * CV_ELEM_SIZE(flags);
    }

    صحيح new_width = total_width / new_cn;

    𐤀𐤌( new_width * new_cn != total_width )
        CV_Error( cv::Error::BadNumChannels,
        "The total width is not divisible by the new number of channels" );

    hdr.size.dims = hdr.dims = 2;
    hdr.size[1] = hdr.cols = new_width;
    hdr.flags = (hdr.flags & ~CV_MAT_CN_MASK) | ((new_cn-1) << CV_CN_SHIFT);
    hdr.step[1] = CV_ELEM_SIZE(hdr.flags);
    أرجع hdr;
}

#ifdef HAVE_OPENCL
نطاق {
ساكن منطقي ocl_setDiag(ثابت UMat& d, UMat& m, صحيح len)
{
    صحيح cn = d.channels();
    صحيح depth = d.depth();

    𐤀𐤌 (depth == CV_64F && !ocl::Device::getDefault().doubleFPConfig())
        أرجع خطأ;

    String opts = format("-D SET_DIAG -D T1=%s -D cn=%d -D IS_ROW_VECTOR=%d",
                         ocl::memopTypeToStr(depth),
                         cn,
                         (d.rows == 1) ? 1 : 0);

    ocl::Kernel k("setDiag", ocl::core::copyset_oclsrc, opts);
    𐤀𐤌 (k.empty())
        أرجع خطأ;

    k.args(ocl::KernelArg::WriteOnly(m),
           ocl::KernelArg::ReadOnlyNoSize(d),
           len);

    size_t globalsize[2] = { (size_t)len, (size_t)len };
    أرجع k.run(2, globalsize, عدم, خطأ);
}
}
#endif

UMat UMat::diag(ثابت UMat& d, UMatUsageFlags usageFlags)
{
    CV_INSTRUMENT_REGION();

    CV_Assert(d.cols == 1 || d.rows == 1);
    صحيح len = d.rows + d.cols - 1;

#ifdef HAVE_OPENCL
    𐤀𐤌 (ocl::useOpenCL())
    {
        UMat m(len, len, d.type(), usageFlags);
        𐤀𐤌 (ocl_setDiag(d, m, len))
        {
            CV_IMPL_ADD(CV_IMPL_OCL);
            أرجع m;
        }
    }
#endif

    UMat m(len, len, d.type(), Scalar(0), usageFlags);
    UMat md = m.diag();
    𐤀𐤌( d.cols == 1 )
        d.copyTo(md);
    وإلا
        transpose(d, md);
    أرجع m;
}

صحيح UMat::checkVector(صحيح _elemChannels, صحيح _depth, منطقي _requireContinuous) ثابت
{
    أرجع (depth() == _depth || _depth <= 0) &&
        (isContinuous() || !_requireContinuous) &&
        ((dims <= 2 && (((rows == 1 || cols == 1) && channels() == _elemChannels) ||
                        (cols == _elemChannels && channels() == 1))) ||
        (dims == 3 && channels() == 1 && size.p[2] == _elemChannels && (size.p[0] == 1 || size.p[1] == 1) &&
         (isContinuous() || step.p[1] == step.p[2]*size.p[2])))
    ? (صحيح)(total()*channels()/_elemChannels) : -1;
}

UMat UMat::reshape(صحيح _cn, صحيح _newndims, ثابت صحيح* _newsz) ثابت
{
    𐤀𐤌(_newndims == dims)
    {
        𐤀𐤌(_newsz == 0)
            أرجع reshape(_cn);
        𐤀𐤌(_newndims == 2)
            أرجع reshape(_cn, _newsz[0]);
    }

    𐤀𐤌 (isContinuous())
    {
        CV_Assert(_cn >= 0 && _newndims >= 0 && _newndims <= CV_MAX_DIM && (_newndims == 0 || _newsz != 0));

        𐤀𐤌 (_cn == 0)
            _cn = هذا->channels();
        وإلا
            CV_Assert(_cn <= CV_CN_MAX);

        size_t total_elem1_ref = هذا->total() * هذا->channels();
        size_t total_elem1 = _cn;

        AutoBuffer<صحيح, 4> newsz_buf( (size_t)std::max(_newndims, 1) );

        صحيح m1_idx = -1;

        لكل (صحيح i = 0; i < _newndims; i++)
        {
            𐤀𐤌 (_newsz[i] >= 0) {
                𐤀𐤌 (_newsz[i] == 0 && i < dims)
                    newsz_buf[i] = size.p[i];
                وإلا
                    newsz_buf[i] = _newsz[i];
                total_elem1 *= (size_t)newsz_buf[i];
            } وإلا {
                𐤀𐤌 (m1_idx >= 0)
                    CV_Error(cv::Error::StsBadSize, "More than one '-1' occured in the new shape");
                m1_idx = i;
            }
        }

        𐤀𐤌 (m1_idx >= 0) {
            𐤀𐤌 (total_elem1 == 0) {
                CV_Assert(total_elem1_ref == 0);
                total_elem1 = 1;
            }
            CV_Assert(total_elem1_ref % total_elem1 == 0);
            newsz_buf[m1_idx] = (صحيح)(total_elem1_ref / total_elem1);
        } وإلا 𐤀𐤌 (total_elem1 != total_elem1_ref) {
            CV_Error(cv::Error::StsUnmatchedSizes, "Requested and source matrices have different count of elements");
        }

        UMat hdr = *هذا;
        hdr.flags = (hdr.flags & ~CV_MAT_CN_MASK) | ((_cn-1) << CV_CN_SHIFT);
        setSize(hdr, _newndims, newsz_buf.data(), عدم, true);

        أرجع hdr;
    }

    CV_Error(cv::Error::StsNotImplemented, "Reshaping of n-dimensional non-continuous matrices is not supported yet");
}

UMat UMat::reshape(صحيح _cn, ثابت MatShape& _newshape) ثابت
{
    𐤀𐤌 (_newshape.dims < 0) {
        صحيح newshape[] = {0};
        أرجع reshape(_cn, 1, newshape);
    }
    أرجع reshape(_cn, _newshape.dims, _newshape.p);
}

Mat UMat::getMat(AccessFlag accessFlags) ثابت
{
    𐤀𐤌(!u)
        أرجع Mat();
    // TODO Support ACCESS_READ (ACCESS_WRITE) without unnecessary data transfers
    accessFlags |= ACCESS_RW;
    UMatDataAutoLock autolock(u);
    حاول
    {
        𐤀𐤌(CV_XADD(&u->refcount, 1) == 0)
            u->currAllocator->map(u, accessFlags);
        𐤀𐤌 (u->data != 0)
        {
            Mat hdr(dims, size.p, type(), u->data + offset, step.p);
            hdr.flags = flags;
            hdr.u = u;
            hdr.datastart = u->data;
            hdr.data = u->data + offset;
            hdr.datalimit = hdr.dataend = u->data + u->size;
            أرجع hdr;
        }
    }
    امسك(...)
    {
        CV_XADD(&u->refcount, -1);
        ارم;
    }
    CV_XADD(&u->refcount, -1);
    CV_Assert(u->data != 0 && "Error mapping of UMat to host memory.");
    أرجع Mat();
}

فراغ* UMat::handle(AccessFlag accessFlags) ثابت
{
    𐤀𐤌( !u )
        أرجع 0;

    CV_Assert(u->refcount == 0);
    CV_Assert(!u->deviceCopyObsolete() || u->copyOnMap());
    𐤀𐤌 (u->deviceCopyObsolete())
    {
        u->currAllocator->unmap(u);
    }

    𐤀𐤌 (!!(accessFlags & ACCESS_WRITE))
        u->markHostCopyObsolete(true);

    أرجع u->handle;
}

فراغ UMat::ndoffset(size_t* ofs) ثابت
{
    // offset = step[0]*ofs[0] + step[1]*ofs[1] + step[2]*ofs[2] + ...;
    size_t val = offset;
    لكل( صحيح i = 0; i < dims; i++ )
    {
        size_t s = step.p[i];
        ofs[i] = val / s;
        val -= ofs[i]*s;
    }
}

فراغ UMat::copyTo(OutputArray _dst) ثابت
{
    CV_INSTRUMENT_REGION();

#ifdef HAVE_CUDA
    𐤀𐤌 (_dst.isGpuMat())
    {
        _dst.getGpuMat().upload(*هذا);
        أرجع;
    }
#endif

    صحيح stype = type();
    صحيح dtype = _dst.type();
    𐤀𐤌( _dst.fixedType() && dtype != stype )
    {
        CV_Assert( CV_MAT_CN(stype) == CV_MAT_CN(dtype) );
        convertTo( _dst, dtype );
        أرجع;
    }

    𐤀𐤌( dims == 0 && empty() )
    {
        _dst.release();
        فراغ* obj = _dst.getObj();
        𐤀𐤌 (_dst.isMat())
            reinterpret_cast<Mat*>(obj)->flags = Mat::MAGIC_VAL | Mat::CONTINUOUS_FLAG | stype;
        وإلا 𐤀𐤌 (_dst.isUMat())
            reinterpret_cast<UMat*>(obj)->flags = UMat::MAGIC_VAL | UMat::CONTINUOUS_FLAG | stype;
        وإلا 𐤀𐤌 (_dst.isGpuMat())
            reinterpret_cast<cuda::GpuMat*>(obj)->flags = stype;
        أرجع;
    }

    _dst.create( dims, size.p, stype );
    𐤀𐤌 (empty())
        أرجع;

    size_t sz[CV_MAX_DIM] = {1}, srcofs[CV_MAX_DIM]={0}, dstofs[CV_MAX_DIM]={0};
    size_t esz = CV_ELEM_SIZE(stype);
    صحيح i, d = std::max(dims, 1);
    لكل( i = 0; i < d; i++ )
        sz[i] = size.p[i];
    sz[d-1] *= esz;
    ndoffset(srcofs);
    srcofs[d-1] *= esz;

    𐤀𐤌( _dst.isUMat() )
    {
        UMat dst = _dst.getUMat();
        CV_Assert(dst.u);
        𐤀𐤌( u == dst.u && dst.offset == offset )
            أرجع;

        𐤀𐤌 (u->currAllocator == dst.u->currAllocator)
        {
            dst.ndoffset(dstofs);
            dstofs[dims-1] *= esz;
            u->currAllocator->copy(u, dst.u, d, sz, srcofs, step.p, dstofs, dst.step.p, خطأ);
            أرجع;
        }

        // different allocators: never map a device destination, upload into it instead
        // (through a host Mat when both sides are device memory)
        MatAllocator* hostAllocator = Mat::getDefaultAllocator();
        𐤀𐤌 (dst.u->currAllocator != hostAllocator)
        {
            𐤀𐤌 (u->currAllocator == hostAllocator)
                getMat(ACCESS_READ).copyTo(_dst);
            وإلا
            {
                Mat tmp;
                copyTo(tmp);
                tmp.copyTo(_dst);
            }
            أرجع;
        }
    }

    Mat dst = _dst.getMat();
    u->currAllocator->download(u, dst.ptr(), d, sz, srcofs, step.p, dst.step.p);
}

فراغ UMat::copyTo(OutputArray _dst, InputArray _mask) ثابت
{
    CV_INSTRUMENT_REGION();

    𐤀𐤌( _mask.empty() )
    {
        copyTo(_dst);
        أرجع;
    }
#ifdef HAVE_OPENCL
    صحيح cn = channels(), mtype = _mask.type(), mdepth = CV_MAT_DEPTH(mtype), mcn = CV_MAT_CN(mtype);
    CV_Assert( (mdepth == CV_8U || mdepth == CV_8S || mdepth == CV_Bool) && (mcn == 1 || mcn == cn) );

    𐤀𐤌 (ocl::useOpenCL() && _dst.isUMat() && dims <= 2)
    {
        UMatData * prevu = _dst.getUMat().u;
        _dst.create( size, type() );

        UMat dst = _dst.getUMat();

        منطقي haveDstUninit = خطأ;
        𐤀𐤌( prevu != dst.u ) // do not leave dst uninitialized
            haveDstUninit = true;

        String opts = format("-D COPY_TO_MASK -D T1=%s -D scn=%d -D mcn=%d%s",
                             ocl::memopTypeToStr(depth()), cn, mcn,
                             haveDstUninit ? " -D HAVE_DST_UNINIT" : "");

        ocl::Kernel k("copyToMask", ocl::core::copyset_oclsrc, opts);
        𐤀𐤌 (!k.empty())
        {
            k.args(ocl::KernelArg::ReadOnlyNoSize(*هذا),
                   ocl::KernelArg::ReadOnlyNoSize(_mask.getUMat()),
                   haveDstUninit ? ocl::KernelArg::WriteOnly(dst) :
                                   ocl::KernelArg::ReadWrite(dst));

            size_t globalsize[2] = { (size_t)cols, (size_t)rows };
            𐤀𐤌 (k.run(2, globalsize, عدم, خطأ))
            {
                CV_IMPL_ADD(CV_IMPL_OCL);
                أرجع;
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

    منطقي haveMask = !_mask.empty();
#ifdef HAVE_OPENCL
    صحيح tp = type(), cn = CV_MAT_CN(tp), d = CV_MAT_DEPTH(tp);

    𐤀𐤌( dims <= 2 && cn <= 4 && CV_MAT_DEPTH(tp) < CV_64F && ocl::useOpenCL() )
    {
        Mat value = _value.getMat();
        CV_Assert( checkScalar(value, type(), _value.kind(), _InputArray::UMAT) );
        صحيح kercn = haveMask || cn == 3 ? cn : std::max(cn, ocl::predictOptimalVectorWidth(*هذا)),
                kertp = CV_MAKE_TYPE(d, kercn);

        مزدوج buf[16] = { 0, 0, 0, 0, 0, 0, 0, 0,
                           0, 0, 0, 0, 0, 0, 0, 0 };
        convertAndUnrollScalar(value, tp, (uchar *)buf, kercn / cn);

        صحيح scalarcn = kercn == 3 ? 4 : kercn, rowsPerWI = ocl::Device::getDefault().isIntel() ? 4 : 1;
        String opts = format("-D dstT=%s -D rowsPerWI=%d -D dstST=%s -D dstT1=%s -D cn=%d",
                             ocl::memopTypeToStr(kertp), rowsPerWI,
                             ocl::memopTypeToStr(CV_MAKETYPE(d, scalarcn)),
                             ocl::memopTypeToStr(d), kercn);

        ocl::Kernel setK(haveMask ? "setMask" : "set", ocl::core::copyset_oclsrc, opts);
        𐤀𐤌( !setK.empty() )
        {
            ocl::KernelArg scalararg(ocl::KernelArg::CONSTANT, 0, 0, 0, buf, CV_ELEM_SIZE(d) * scalarcn);
            UMat mask;

            𐤀𐤌( haveMask )
            {
                mask = _mask.getUMat();
                CV_Assert( mask.size() == size() && (mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool) );
                ocl::KernelArg maskarg = ocl::KernelArg::ReadOnlyNoSize(mask),
                        dstarg = ocl::KernelArg::ReadWrite(*هذا);
                setK.args(maskarg, dstarg, scalararg);
            }
            وإلا
            {
                ocl::KernelArg dstarg = ocl::KernelArg::WriteOnly(*هذا, cn, kercn);
                setK.args(dstarg, scalararg);
            }

            size_t globalsize[] = { (size_t)cols * cn / kercn, ((size_t)rows + rowsPerWI - 1) / rowsPerWI };
            𐤀𐤌( setK.run(2, globalsize, عدم, خطأ) )
            {
                CV_IMPL_ADD(CV_IMPL_OCL);
                أرجع *هذا;
            }
        }
    }
#endif
    Mat m = getMat(haveMask ? ACCESS_RW : ACCESS_WRITE);
    m.setTo(_value, _mask);
    أرجع *هذا;
}

UMat& UMat::operator = (ثابت Scalar& s)
{
    setTo(s);
    أرجع *هذا;
}

UMat UMat::t() ثابت
{
    UMat m;
    transpose(*هذا, m);
    أرجع m;
}

UMat UMat::zeros(صحيح rows, صحيح cols, صحيح type, UMatUsageFlags usageFlags)
{
    أرجع UMat(rows, cols, type, Scalar::all(0), usageFlags);
}

UMat UMat::zeros(Size size, صحيح type, UMatUsageFlags usageFlags)
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

UMat UMat::ones(صحيح rows, صحيح cols, صحيح type, UMatUsageFlags usageFlags)
{
    أرجع UMat(rows, cols, type, Scalar(1), usageFlags);
}

UMat UMat::ones(Size size, صحيح type, UMatUsageFlags usageFlags)
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
