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

فضاینام cv {

// forward decls, implementation is below in this file
تهی setSize(UMat& m, صحیح _dims, ثابت صحیح* _sz, ثابت size_t* _steps,
             منطقی autoSteps = نادرست);

تهی updateContinuityFlag(UMat& m);
تهی finalizeHdr(UMat& m);

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
    originalUMatData = پوچ;
}

UMatData::~UMatData()
{
    prevAllocator = currAllocator = 0;
    urefcount = refcount = 0;
    CV_Assert(mapcount == 0);
    data = origdata = 0;
    size = 0;
    منطقی isAsyncCleanup = !!(flags & UMatData::ASYNC_CLEANUP);
    flags = static_cast<UMatData::MemoryFlag>(0);
    handle = 0;
    userdata = 0;
    allocatorFlags_ = 0;
    اگر (originalUMatData)
    {
        منطقی showWarn = نادرست;
        UMatData* u = originalUMatData;
        منطقی zero_Ref = CV_XADD(&(u->refcount), -1) == 1;
        اگر (zero_Ref)
        {
            // simulate Mat::deallocate
            اگر (u->mapcount != 0)
            {
                (u->currAllocator ? u->currAllocator : /* TODO allocator ? allocator :*/ Mat::getDefaultAllocator())->unmap(u);
            }
            وگرنه
            {
                // we don't do "map", so we can't do "unmap"
            }
        }
        منطقی zero_URef = CV_XADD(&(u->urefcount), -1) == 1;
        اگر (zero_Ref && !zero_URef)
            showWarn = درست;
        اگر (zero_Ref && zero_URef) // oops, we need to free resources
        {
            showWarn = !isAsyncCleanup;
            // simulate UMat::deallocate
            u->currAllocator->deallocate(u);
        }
#ifndef NDEBUG
        اگر (showWarn)
        {
            ایستا صحیح warn_message_showed = 0;
            اگر (warn_message_showed++ < 100)
            {
                fflush(stdout);
                fprintf(stderr, "\n! OPENCV warning: getUMat()/getMat() call chain possible problem."
                                "\n!                 Base object is dead, while nested/derived object is still alive or processed."
                                "\n!                 Please check lifetime of UMat/Mat objects!\n");
                fflush(stderr);
            }
        }
#وگرنه
        CV_UNUSED(showWarn);
#endif
        originalUMatData = پوچ;
    }
}

#ifndef OPENCV_DISABLE_THREAD_SUPPORT

// it should be a prime number for the best hash function
شمارش { UMAT_NLOCKS = 31 };
ایستا Mutex umatLocks[UMAT_NLOCKS];

ایستا size_t getUMatDataLockIndex(ثابت UMatData* u)
{
    size_t idx = ((size_t)(تهی*)u) % UMAT_NLOCKS;
    بازگشت idx;
}

تهی UMatData::lock()
{
    size_t idx = getUMatDataLockIndex(این);
    //printf("%d lock(%d)\n", cv::utils::getThreadID(), (int)idx);
    umatLocks[idx].lock();
}

تهی UMatData::unlock()
{
    size_t idx = getUMatDataLockIndex(این);
    //printf("%d unlock(%d)\n", cv::utils::getThreadID(), (int)idx);
    umatLocks[idx].unlock();
}


// Do not allow several lock() calls with different UMatData objects.
ساختار UMatDataAutoLocker
{
    صحیح usage_شمار;
    UMatData* locked_objects[2];
    UMatDataAutoLocker() : usage_شمار(0) { locked_objects[0] = پوچ; locked_objects[1] = پوچ; }

    تهی lock(UMatData*& u1)
    {
        منطقی locked_1 = (u1 == locked_objects[0] || u1 == locked_objects[1]);
        اگر (locked_1)
        {
            u1 = پوچ;
            بازگشت;
        }
        CV_Assert(usage_شمار == 0);  // UMatDataAutoLock can't be used multiple times from the same thread
        usage_شمار = 1;
        locked_objects[0] = u1;
        u1->lock();
    }
    تهی lock(UMatData*& u1, UMatData*& u2)
    {
        منطقی locked_1 = (u1 == locked_objects[0] || u1 == locked_objects[1]);
        منطقی locked_2 = (u2 == locked_objects[0] || u2 == locked_objects[1]);
        اگر (locked_1)
            u1 = پوچ;
        اگر (locked_2)
            u2 = پوچ;
        اگر (locked_1 && locked_2)
            بازگشت;
        CV_Assert(usage_شمار == 0);  // UMatDataAutoLock can't be used multiple times from the same thread
        usage_شمار = 1;
        locked_objects[0] = u1;
        locked_objects[1] = u2;
        اگر (u1)
            u1->lock();
        اگر (u2)
            u2->lock();
    }
    تهی release(UMatData* u1, UMatData* u2)
    {
        اگر (u1 == پوچ && u2 == پوچ)
            بازگشت;
        CV_Assert(usage_شمار == 1);
        usage_شمار = 0;
        اگر (u1)
            u1->unlock();
        اگر (u2)
            u2->unlock();
        locked_objects[0] = پوچ; locked_objects[1] = پوچ;
    }
};
ایستا TLSData<UMatDataAutoLocker>& getUMatDataAutoLockerTLS()
{
    CV_SINGLETON_LAZY_INIT_REF(TLSData<UMatDataAutoLocker>, جدید TLSData<UMatDataAutoLocker>());
}
ایستا UMatDataAutoLocker& getUMatDataAutoLocker() { بازگشت getUMatDataAutoLockerTLS().getRef(); }


UMatDataAutoLock::UMatDataAutoLock(UMatData* u) : u1(u), u2(پوچ)
{
    getUMatDataAutoLocker().lock(u1);
}
UMatDataAutoLock::UMatDataAutoLock(UMatData* u1_, UMatData* u2_) : u1(u1_), u2(u2_)
{
    اگر (getUMatDataLockIndex(u1) > getUMatDataLockIndex(u2))
    {
        std::swap(u1, u2);
    }
    getUMatDataAutoLocker().lock(u1, u2);
}
UMatDataAutoLock::~UMatDataAutoLock()
{
    getUMatDataAutoLocker().release(u1, u2);
}

#وگرنه

تهی UMatData::lock()
{
    // nothing in OPENCV_DISABLE_THREAD_SUPPORT mode
}

تهی UMatData::unlock()
{
    // nothing in OPENCV_DISABLE_THREAD_SUPPORT mode
}

UMatDataAutoLock::UMatDataAutoLock(UMatData* u) : u1(u), u2(پوچ)
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

UMat::UMat(صحیح _rows, صحیح _cols, صحیح _type, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_rows, _cols, _type);
}

UMat::UMat(صحیح _rows, صحیح _cols, صحیح _type, ثابت Scalar& _s, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_rows, _cols, _type);
    *این = _s;
}

UMat::UMat(Size _sz, صحیح _type, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create( _sz.height, _sz.width, _type );
}

UMat::UMat(Size _sz, صحیح _type, ثابت Scalar& _s, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_sz.height, _sz.width, _type);
    *این = _s;
}

UMat::UMat(صحیح _dims, ثابت صحیح* _sz, صحیح _type, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_dims, _sz, _type);
}

UMat::UMat(صحیح _dims, ثابت صحیح* _sz, صحیح _type, ثابت Scalar& _s, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_dims, _sz, _type);
    *این = _s;
}

UMat::UMat(ثابت MatShape& _shape, صحیح _type, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_shape.dims, _shape.p, _type);
}

UMat::UMat(ثابت MatShape& _shape, صحیح _type, ثابت Scalar& _s, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_shape.dims, _shape.p, _type);
    *این = _s;
}

UMat::UMat(ثابت UMat& m)
: flags(m.flags), dims(m.dims), rows(m.rows), cols(m.cols), allocator(m.allocator),
  usageFlags(m.usageFlags), u(m.u), offset(m.offset), size(m.size), step(m.step)
{
    addref();
}

UMat& UMat::operator=(ثابت UMat& m)
{
    اگر( این != &m )
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
    بازگشت *این;
}

UMat UMat::clone() ثابت
{
    UMat m;
    copyTo(m);
    بازگشت m;
}

تهی UMat::assignTo(UMat& m, صحیح _type) ثابت
{
    اگر( _type < 0 )
        m = *این;
    وگرنه
        convertTo(m, _type);
}

تهی UMat::create(صحیح _rows, صحیح _cols, صحیح _type, UMatUsageFlags _usageFlags)
{
    صحیح sz[] = {_rows, _cols};
    create(2, sz, _type, _usageFlags);
}

تهی UMat::create(Size _sz, صحیح _type, UMatUsageFlags _usageFlags)
{
    create(_sz.height, _sz.width, _type, _usageFlags);
}

تهی UMat::createSameSize(درون_داد_Array arr, صحیح _type, UMatUsageFlags _usageFlags)
{
    create(arr.shape(), _type, _usageFlags);
}

تهی UMat::addref()
{
    اگر( u )
        CV_XADD(&(u->urefcount), 1);
}

تهی UMat::release()
{
    اگر( u && CV_XADD(&(u->urefcount), -1) == 1 )
        deallocate();
    u = 0;
    size.clear();
    dims = cols = rows = 0;
}

منطقی UMat::empty() ثابت
{
    بازگشت u == 0 || total() == 0;
}

size_t UMat::total() ثابت
{
    اگر( dims <= 2 )
        بازگشت (size_t)rows * cols;
    size_t p = 1;
    برای( صحیح i = 0; i < dims; i++ )
        p *= size[i];
    بازگشت p;
}

MatShape UMat::shape() ثابت
{
    بازگشت size;
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
    m.allocator = پوچ;
    m.u = پوچ;
    m.offset = 0;
}

UMat& UMat::operator=(UMat&& m)
{
    اگر (این == &m)
      بازگشت *این;
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
    m.allocator = پوچ;
    m.u = پوچ;
    m.offset = 0;
    بازگشت *این;
}


MatAllocator* UMat::getStdAllocator()
{
#ifdef HAVE_OPENCL
    اگر (ocl::useOpenCL())
        بازگشت ocl::getOpenCLAllocator();
#endif
    بازگشت Mat::getDefaultAllocator();
}

تهی swap( UMat& a, UMat& b )
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


تهی setSize( UMat& m, صحیح _dims, ثابت صحیح* _sz,
              ثابت size_t* _steps, منطقی autoSteps )
{
    CV_Assert( 0 <= _dims && _dims <= CV_MAX_DIM && _dims <= MatShape::MAX_DIMS);

    m.dims = _dims;
    m.size = MatShape(_dims, _sz);
    m.step[std::max(_dims-1, 0)] = CV_ELEM_SIZE(m.flags);
    برای (صحیح i = _dims-2; i >= 0; i--) {
        size_t autostep = m.size[i+1]*m.step[i+1];
        اگر (_steps) {
            m.step[i] = _steps[i];
            //CV_Assert(m.step[i] >= autostep);
        } وگرنه اگر (autoSteps) {
            m.step[i] = autostep;
        } وگرنه {
            m.step[i] = 0;
        }
    }

    اگر( _dims <= 2 )
    {
        m.cols = _dims == 0 ? 1 : _sz ? _sz[_dims > 1] : 0;
        m.rows = _dims < 2 ? 1 : _sz ? _sz[0] : 0;
    } وگرنه {
        m.cols = m.rows = -1;
    }
}


تهی UMat::updateContinuityFlag()
{
    flags = cv::updateContinuityFlag(flags, dims, size.p, step.p);
}


تهی finalizeHdr(UMat& m)
{
    m.updateContinuityFlag();
    صحیح d = m.dims;
    اگر( d > 2 )
        m.rows = m.cols = -1;
}


UMat Mat::getUMat(AccessFlag accessFlags, UMatUsageFlags usageFlags) ثابت
{
    UMat hdr;
    اگر(!data)
        بازگشت hdr;
    اگر (data != datastart)
    {
        Size wholeSize;
        Point ofs;
        locateROI(wholeSize, ofs);
        Size sz(cols, rows);
        اگر (ofs.x != 0 || ofs.y != 0)
        {
            Mat src = *این;
            صحیح dtop = ofs.y;
            صحیح dbottom = wholeSize.height - src.rows - ofs.y;
            صحیح dleft = ofs.x;
            صحیح dright = wholeSize.width - src.cols - ofs.x;
            src.adjustROI(dtop, dbottom, dleft, dright);
            بازگشت src.getUMat(accessFlags, usageFlags)(cv::Rect(ofs.x, ofs.y, sz.width, sz.height));
        }
    }
    CV_Assert(data == datastart);

    accessFlags |= ACCESS_RW;
    UMatData* new_u = پوچ;
    MatStep new_step = step;
    {
        MatAllocator *a = allocator, *a0 = getDefaultAllocator();
        اگر(!a)
            a = a0;
        new_u = a->allocate(dims, size.p, type(), data, new_step.p, accessFlags, usageFlags);
        new_u->originalUMatData = u;
    }
    منطقی allocated = نادرست;
    تلاش
    {
        allocated = UMat::getStdAllocator()->allocate(new_u, accessFlags, usageFlags);
    }
    بگیر (ثابت cv::Exception& e)
    {
        fprintf(stderr, "Exception: %s\n", e.what());
    }
    اگر (!allocated)
    {
        allocated = getDefaultAllocator()->allocate(new_u, accessFlags, usageFlags);
        CV_Assert(allocated);
    }
    اگر (u != پوچ)
    {
#ifdef HAVE_OPENCL
        اگر (ocl::useOpenCL() && new_u->currAllocator == ocl::getOpenCLAllocator())
        {
            CV_Assert(new_u->tempUMat());
        }
#endif
        CV_XADD(&(u->refcount), 1);
        CV_XADD(&(u->urefcount), 1);
    }
    تلاش
    {
        hdr.flags = flags;
        hdr.usageFlags = usageFlags;
        setSize(hdr, dims, size.p, new_step.p);
        finalizeHdr(hdr);
        hdr.u = new_u;
        hdr.offset = 0; //data - datastart;
        hdr.addref();
        بازگشت hdr;
    }
    بگیر(...)
    {
        اگر (u != پوچ)
        {
            CV_XADD(&(u->refcount), -1);
            CV_XADD(&(u->urefcount), -1);
        }
        new_u->currAllocator->deallocate(new_u);
        پرتابکن;
    }

}

تهی UMat::create(صحیح d0, ثابت صحیح* _sizes, صحیح _type, UMatUsageFlags _usageFlags)
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
        برای( i = 0; i < d; i++ )
            اگر( size[i] != _sizes[i] )
                بشکن;
        اگر( i == d )
            بازگشت;
    }

    صحیح _sizes_backup[CV_MAX_DIM]; // #5991
    اگر (_sizes == (این->size.p))
    {
        برای(i = 0; i < d; i++ )
            _sizes_backup[i] = _sizes[i];
        _sizes = _sizes_backup;
    }

    release();
    usageFlags = _usageFlags;
    اگر( d == 0 )
        بازگشت;
    flags = (_type & CV_MAT_TYPE_MASK) | MAGIC_VAL;
    setSize(*این, d, _sizes, 0, درست);
    offset = 0;

    اگر( total() > 0 )
    {
        MatAllocator *a = allocator, *a0 = getStdAllocator();
        اگر (!a)
        {
            a = a0;
            a0 = Mat::getDefaultAllocator();
        }
        تلاش
        {
            u = a->allocate(dims, size.p, _type, 0, step.p, ACCESS_RW /* ignored */, usageFlags);
            CV_Assert(u != 0);
        }
        بگیر(...)
        {
            اگر(a != a0)
                u = a0->allocate(dims, size.p, _type, 0, step.p, ACCESS_RW /* ignored */, usageFlags);
            CV_Assert(u != 0);
        }
        CV_Assert( step[dims-1] == (size_t)CV_ELEM_SIZE(flags) );
    }

    finalizeHdr(*این);
    addref();
    size.dims = dims = d0;
}

تهی UMat::create(ثابت std::vector<صحیح>& _sizes, صحیح _type, UMatUsageFlags _usageFlags)
{
    create((صحیح)_sizes.size(), _sizes.data(), _type, _usageFlags);
}

تهی UMat::create(ثابت MatShape& _shape, صحیح _type, UMatUsageFlags _usageFlags)
{
    اگر (_shape.dims < 0) {
        release();
    } وگرنه {
        create(_shape.dims, _shape.p, _type, _usageFlags);
        size.layout = _shape.layout;
        size.C = _shape.C;
    }
}

تهی UMat::fit(صحیح _dims, ثابت صحیح* _sizes, صحیح _type, UMatUsageFlags _usageFlags)
{
    اگر (_usageFlags == cv::USAGE_DEFAULT)
        _usageFlags = usageFlags;
    size_t oldTotalBytes = u ? u->size : 0;
    size_t esz = CV_ELEM_SIZE(_type), newTotal = _dims >= 0;
    برای (صحیح i = 0; i < _dims; i++)
        newTotal *= _sizes[i];
    size_t newTotalBytes = newTotal*esz;
    اگر (newTotalBytes > 0 &&
        (!isContinuous() ||
         newTotalBytes > oldTotalBytes ||
         offset != 0 ||
         _usageFlags != usageFlags)) {
        create(_dims, _sizes, _type, _usageFlags);
    } وگرنه {
        flags = (flags & ~Mat::TYPE_MASK) | CV_MAT_TYPE(_type);
        صحیح _dummy_size = 0;
        setSize(*این, (_dims >= 0 ? _dims : 1), (_dims >= 0 ? _sizes : &_dummy_size), nullptr, درست);
        finalizeHdr(*این);
    }
}

تهی UMat::fit(ثابت std::vector<صحیح>& _shape, صحیح _type, UMatUsageFlags _usageFlags)
{
    fit((صحیح)_shape.size(), _shape.data(), _type, _usageFlags);
}

تهی UMat::fit(ثابت MatShape& _shape, صحیح _type, UMatUsageFlags _usageFlags)
{
    fit(_shape.dims, _shape.p, _type, _usageFlags);
    size.layout = _shape.layout;
    size.C = _shape.C;
}

تهی UMat::fit(صحیح _rows, صحیح _cols, صحیح _type, UMatUsageFlags _usageFlags)
{
    _type &= TYPE_MASK;
    صحیح sz[] = {_rows, _cols};
    fit(2, sz, _type, _usageFlags);
}

تهی UMat::fit(Size _sz, صحیح _type, UMatUsageFlags _usageFlags)
{
    fit(_sz.height, _sz.width, _type, _usageFlags);
}

تهی UMat::fitSameSize(درون_داد_Array m, صحیح _type, UMatUsageFlags _usageFlags)
{
    fit(m.shape(), _type, _usageFlags);
}

تهی UMat::copySize(ثابت UMat& m)
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

تهی UMat::deallocate()
{
    UMatData* u_ = u;
    u = پوچ;
    u_->currAllocator->deallocate(u_);
}


UMat::UMat(ثابت UMat& m, ثابت Range& _rowRange, ثابت Range& _colRange)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(USAGE_DEFAULT), u(0), offset(0)
{
    CV_Assert( m.dims >= 2 && m.dims <= MatShape::MAX_DIMS );
    اگر( m.dims > 2 )
    {
        Range rs[MatShape::MAX_DIMS];
        rs[0] = _rowRange;
        rs[1] = _colRange;
        برای( صحیح i = 2; i < m.dims; i++ )
            rs[i] = Range::all();
        *این = m(rs);
        بازگشت;
    }

    *این = m;
    اگر( _rowRange != Range::all() && _rowRange != Range(0,rows) )
    {
        CV_Assert( 0 <= _rowRange.start &&
                  _rowRange.start <= _rowRange.end &&
                  _rowRange.end <= m.rows );
        size[0] = rows = _rowRange.size();
        offset += step*_rowRange.start;
        flags |= SUBMATRIX_FLAG;
    }

    اگر( _colRange != Range::all() && _colRange != Range(0,cols) )
    {
        CV_Assert( 0 <= _colRange.start &&
                  _colRange.start <= _colRange.end &&
                  _colRange.end <= m.cols );
        size[1] = cols = _colRange.size();
        offset += _colRange.start*elemSize();
        flags |= SUBMATRIX_FLAG;
    }

    updateContinuityFlag();

    اگر( rows <= 0 || cols <= 0 )
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
    اگر( roi.width < m.cols || roi.height < m.rows )
        flags |= SUBMATRIX_FLAG;

    size[0] = rows;
    size[1] = cols;
    step[0] = m.dims == 2 ? m.step[0] : cols*esz;
    step[1] = esz;

    updateContinuityFlag();

    addref();
    اگر( rows <= 0 || cols <= 0 )
    {
        rows = cols = 0;
        release();
    }
}


UMat::UMat(ثابت UMat& m, ثابت Range* ranges)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(USAGE_DEFAULT), u(0), offset(0)
{
    صحیح i, d = m.dims;

    CV_Assert(ranges);
    برای( i = 0; i < d; i++ )
    {
        Range r = ranges[i];
        CV_Assert( r == Range::all() || (0 <= r.start && r.start < r.end && r.end <= m.size[i]) );
    }
    *این = m;
    برای( i = 0; i < d; i++ )
    {
        Range r = ranges[i];
        اگر( r != Range::all() && r != Range(0, size.p[i]))
        {
            size.p[i] = r.end - r.start;
            offset += r.start*step.p[i];
            flags |= SUBMATRIX_FLAG;
        }
    }

    اگر (d <= 2) {
        rows = d == 2 ? size[0] : 1;
        cols = d <= 0 ? (d >= 0) : size[d > 1];
    }
    updateContinuityFlag();
}

UMat::UMat(ثابت UMat& m, ثابت std::vector<Range>& ranges)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(USAGE_DEFAULT), u(0), offset(0)
{
    صحیح i, d = m.dims;

    CV_Assert((صحیح)ranges.size() == d);
    برای (i = 0; i < d; i++)
    {
        Range r = ranges[i];
        CV_Assert(r == Range::all() || (0 <= r.start && r.start < r.end && r.end <= m.size[i]));
    }
    *این = m;
    برای (i = 0; i < d; i++)
    {
        Range r = ranges[i];
        اگر (r != Range::all() && r != Range(0, size.p[i]))
        {
            size.p[i] = r.end - r.start;
            offset += r.start*step.p[i];
            flags |= SUBMATRIX_FLAG;
        }
    }

    اگر (d <= 2) {
        rows = d == 2 ? size[0] : 1;
        cols = d <= 0 ? (d >= 0) : size[d > 1];
    }
    updateContinuityFlag();
}

UMat UMat::diag(صحیح d) ثابت
{
    CV_Assert( dims <= 2 );
    UMat m = *این;
    size_t esz = elemSize();
    صحیح len;

    اگر( d >= 0 )
    {
        len = std::min(cols - d, rows);
        m.offset += esz*d;
    }
    وگرنه
    {
        len = std::min(rows + d, cols);
        m.offset -= step[0]*d;
    }
    CV_DbgAssert( len > 0 );

    m.size[0] = m.rows = len;
    m.size[1] = m.cols = 1;
    m.step[0] += (len > 1 ? esz : 0);

    m.updateContinuityFlag();

    اگر( size() != Size(1,1) )
        m.flags |= SUBMATRIX_FLAG;

    بازگشت m;
}

تهی UMat::locateROI( Size& wholeSize, Point& ofs ) ثابت
{
    CV_Assert( dims <= 2 && step[0] > 0 );
    size_t esz = elemSize(), minstep;
    ptrdiff_t delta1 = (ptrdiff_t)offset, delta2 = (ptrdiff_t)u->size;

    اگر( delta1 == 0 )
        ofs.x = ofs.y = 0;
    وگرنه
    {
        ofs.y = (صحیح)(delta1/step[0]);
        ofs.x = (صحیح)((delta1 - step[0]*ofs.y)/esz);
        CV_DbgAssert( offset == (size_t)(ofs.y*step[0] + ofs.x*esz) );
    }
    minstep = (ofs.x + cols)*esz;
    wholeSize.height = (صحیح)((delta2 - minstep)/step[0] + 1);
    wholeSize.height = std::max(wholeSize.height, ofs.y + rows);
    wholeSize.width = (صحیح)((delta2 - step*(wholeSize.height-1))/esz);
    wholeSize.width = std::max(wholeSize.width, ofs.x + cols);
}


UMat& UMat::adjustROI( صحیح dtop, صحیح dbottom, صحیح dleft, صحیح dright )
{
    CV_Assert( dims == 2 && step[0] > 0 );
    Size wholeSize; Point ofs;
    size_t esz = elemSize();
    locateROI( wholeSize, ofs );
    صحیح row1 = std::min(std::max(ofs.y - dtop, 0), wholeSize.height), row2 = std::max(0, std::min(ofs.y + rows + dbottom, wholeSize.height));
    صحیح col1 = std::min(std::max(ofs.x - dleft, 0), wholeSize.width), col2 = std::max(0, std::min(ofs.x + cols + dright, wholeSize.width));
    اگر(row1 > row2)
        std::swap(row1, row2);
    اگر(col1 > col2)
        std::swap(col1, col2);

    offset += (row1 - ofs.y)*step + (col1 - ofs.x)*esz;
    rows = row2 - row1; cols = col2 - col1;
    size.p[0] = rows; size.p[1] = cols;
    updateContinuityFlag();
    بازگشت *این;
}


UMat UMat::reshape(صحیح new_cn, صحیح new_rows) ثابت
{
    صحیح cn = channels();
    UMat hdr = *این;

    اگر( dims > 2 && new_rows == 0 && new_cn != 0 && size[dims-1]*cn % new_cn == 0 )
    {
        hdr.flags = (hdr.flags & ~CV_MAT_CN_MASK) | ((new_cn-1) << CV_CN_SHIFT);
        hdr.step[dims-1] = CV_ELEM_SIZE(hdr.flags);
        hdr.size[dims-1] = hdr.size[dims-1]*cn / new_cn;
        بازگشت hdr;
    }

    CV_Assert( dims <= 2 );

    اگر( new_cn == 0 )
        new_cn = cn;

    صحیح total_width = cols * cn;

    اگر( (new_cn > total_width || total_width % new_cn != 0) && new_rows == 0 )
        new_rows = rows * total_width / new_cn;

    اگر( new_rows != 0 && new_rows != rows )
    {
        صحیح total_size = total_width * rows;
        اگر( !isContinuous() )
            CV_Error( cv::Error::BadStep,
            "The matrix is not continuous, thus its number of rows can not be changed" );

        total_width = total_size / new_rows;

        اگر( total_width * new_rows != total_size )
            CV_Error( cv::Error::StsBadArg, "The total number of matrix elements "
                                    "is not divisible by the new number of rows" );

        hdr.size[0] = hdr.rows = new_rows;
        hdr.step[0] = total_width * elemSize1();
    } وگرنه {
        hdr.size[0] = hdr.rows = rows;
        اگر (dims <= 1)
            hdr.step[0] = cols * CV_ELEM_SIZE(flags);
    }

    صحیح new_width = total_width / new_cn;

    اگر( new_width * new_cn != total_width )
        CV_Error( cv::Error::Bad_رقم_Channels,
        "The total width is not divisible by the new number of channels" );

    hdr.size.dims = hdr.dims = 2;
    hdr.size[1] = hdr.cols = new_width;
    hdr.flags = (hdr.flags & ~CV_MAT_CN_MASK) | ((new_cn-1) << CV_CN_SHIFT);
    hdr.step[1] = CV_ELEM_SIZE(hdr.flags);
    بازگشت hdr;
}

#ifdef HAVE_OPENCL
فضاینام {
ایستا منطقی ocl_setDiag(ثابت UMat& d, UMat& m, صحیح len)
{
    صحیح cn = d.channels();
    صحیح depth = d.depth();

    اگر (depth == CV_64F && !ocl::Device::getDefault().doubleFPConfig())
        بازگشت نادرست;

    String opts = format("-D SET_DIAG -D T1=%s -D cn=%d -D IS_ROW_VECTOR=%d",
                         ocl::memopTypeToStr(depth),
                         cn,
                         (d.rows == 1) ? 1 : 0);

    ocl::Kernel k("setDiag", ocl::core::copyset_oclsrc, opts);
    اگر (k.empty())
        بازگشت نادرست;

    k.args(ocl::KernelArg::WriteOnly(m),
           ocl::KernelArg::ReadOnlyNoSize(d),
           len);

    size_t globalsize[2] = { (size_t)len, (size_t)len };
    بازگشت k.run(2, globalsize, پوچ, نادرست);
}
}
#endif

UMat UMat::diag(ثابت UMat& d, UMatUsageFlags usageFlags)
{
    CV_INSTRUMENT_REGION();

    CV_Assert(d.cols == 1 || d.rows == 1);
    صحیح len = d.rows + d.cols - 1;

#ifdef HAVE_OPENCL
    اگر (ocl::useOpenCL())
    {
        UMat m(len, len, d.type(), usageFlags);
        اگر (ocl_setDiag(d, m, len))
        {
            CV_IMPL_ADD(CV_IMPL_OCL);
            بازگشت m;
        }
    }
#endif

    UMat m(len, len, d.type(), Scalar(0), usageFlags);
    UMat md = m.diag();
    اگر( d.cols == 1 )
        d.copyTo(md);
    وگرنه
        transpose(d, md);
    بازگشت m;
}

صحیح UMat::checkVector(صحیح _elemChannels, صحیح _depth, منطقی _requireContinuous) ثابت
{
    بازگشت (depth() == _depth || _depth <= 0) &&
        (isContinuous() || !_requireContinuous) &&
        ((dims <= 2 && (((rows == 1 || cols == 1) && channels() == _elemChannels) ||
                        (cols == _elemChannels && channels() == 1))) ||
        (dims == 3 && channels() == 1 && size.p[2] == _elemChannels && (size.p[0] == 1 || size.p[1] == 1) &&
         (isContinuous() || step.p[1] == step.p[2]*size.p[2])))
    ? (صحیح)(total()*channels()/_elemChannels) : -1;
}

UMat UMat::reshape(صحیح _cn, صحیح _newndims, ثابت صحیح* _newsz) ثابت
{
    اگر(_newndims == dims)
    {
        اگر(_newsz == 0)
            بازگشت reshape(_cn);
        اگر(_newndims == 2)
            بازگشت reshape(_cn, _newsz[0]);
    }

    اگر (isContinuous())
    {
        CV_Assert(_cn >= 0 && _newndims >= 0 && _newndims <= CV_MAX_DIM && (_newndims == 0 || _newsz != 0));

        اگر (_cn == 0)
            _cn = این->channels();
        وگرنه
            CV_Assert(_cn <= CV_CN_MAX);

        size_t total_elem1_ref = این->total() * این->channels();
        size_t total_elem1 = _cn;

        AutoBuffer<صحیح, 4> newsz_buf( (size_t)std::max(_newndims, 1) );

        صحیح m1_idx = -1;

        برای (صحیح i = 0; i < _newndims; i++)
        {
            اگر (_newsz[i] >= 0) {
                اگر (_newsz[i] == 0 && i < dims)
                    newsz_buf[i] = size.p[i];
                وگرنه
                    newsz_buf[i] = _newsz[i];
                total_elem1 *= (size_t)newsz_buf[i];
            } وگرنه {
                اگر (m1_idx >= 0)
                    CV_Error(cv::Error::StsBadSize, "More than one '-1' occured in the new shape");
                m1_idx = i;
            }
        }

        اگر (m1_idx >= 0) {
            اگر (total_elem1 == 0) {
                CV_Assert(total_elem1_ref == 0);
                total_elem1 = 1;
            }
            CV_Assert(total_elem1_ref % total_elem1 == 0);
            newsz_buf[m1_idx] = (صحیح)(total_elem1_ref / total_elem1);
        } وگرنه اگر (total_elem1 != total_elem1_ref) {
            CV_Error(cv::Error::StsUnmatchedSizes, "Requested and source matrices have different count of elements");
        }

        UMat hdr = *این;
        hdr.flags = (hdr.flags & ~CV_MAT_CN_MASK) | ((_cn-1) << CV_CN_SHIFT);
        setSize(hdr, _newndims, newsz_buf.data(), پوچ, درست);

        بازگشت hdr;
    }

    CV_Error(cv::Error::StsNotImplemented, "Reshaping of n-dimensional non-continuous matrices is not supported yet");
}

UMat UMat::reshape(صحیح _cn, ثابت MatShape& _newshape) ثابت
{
    اگر (_newshape.dims < 0) {
        صحیح newshape[] = {0};
        بازگشت reshape(_cn, 1, newshape);
    }
    بازگشت reshape(_cn, _newshape.dims, _newshape.p);
}

Mat UMat::getMat(AccessFlag accessFlags) ثابت
{
    اگر(!u)
        بازگشت Mat();
    // TODO Support ACCESS_READ (ACCESS_WRITE) without unnecessary data transfers
    accessFlags |= ACCESS_RW;
    UMatDataAutoLock autolock(u);
    تلاش
    {
        اگر(CV_XADD(&u->refcount, 1) == 0)
            u->currAllocator->map(u, accessFlags);
        اگر (u->data != 0)
        {
            Mat hdr(dims, size.p, type(), u->data + offset, step.p);
            hdr.flags = flags;
            hdr.u = u;
            hdr.datastart = u->data;
            hdr.data = u->data + offset;
            hdr.datalimit = hdr.dataend = u->data + u->size;
            بازگشت hdr;
        }
    }
    بگیر(...)
    {
        CV_XADD(&u->refcount, -1);
        پرتابکن;
    }
    CV_XADD(&u->refcount, -1);
    CV_Assert(u->data != 0 && "Error mapping of UMat to host memory.");
    بازگشت Mat();
}

تهی* UMat::handle(AccessFlag accessFlags) ثابت
{
    اگر( !u )
        بازگشت 0;

    CV_Assert(u->refcount == 0);
    CV_Assert(!u->deviceCopyObsolete() || u->copyOnMap());
    اگر (u->deviceCopyObsolete())
    {
        u->currAllocator->unmap(u);
    }

    اگر (!!(accessFlags & ACCESS_WRITE))
        u->markHostCopyObsolete(درست);

    بازگشت u->handle;
}

تهی UMat::ndoffset(size_t* ofs) ثابت
{
    // offset = step[0]*ofs[0] + step[1]*ofs[1] + step[2]*ofs[2] + ...;
    size_t val = offset;
    برای( صحیح i = 0; i < dims; i++ )
    {
        size_t s = step.p[i];
        ofs[i] = val / s;
        val -= ofs[i]*s;
    }
}

تهی UMat::copyTo(OutputArray _dst) ثابت
{
    CV_INSTRUMENT_REGION();

#ifdef HAVE_CUDA
    اگر (_dst.isGpuMat())
    {
        _dst.getGpuMat().upload(*این);
        بازگشت;
    }
#endif

    صحیح stype = type();
    صحیح dtype = _dst.type();
    اگر( _dst.fixedType() && dtype != stype )
    {
        CV_Assert( CV_MAT_CN(stype) == CV_MAT_CN(dtype) );
        convertTo( _dst, dtype );
        بازگشت;
    }

    اگر( dims == 0 && empty() )
    {
        _dst.release();
        تهی* obj = _dst.getObj();
        اگر (_dst.isMat())
            reinterpret_cast<Mat*>(obj)->flags = Mat::MAGIC_VAL | Mat::CONTINUOUS_FLAG | stype;
        وگرنه اگر (_dst.isUMat())
            reinterpret_cast<UMat*>(obj)->flags = UMat::MAGIC_VAL | UMat::CONTINUOUS_FLAG | stype;
        وگرنه اگر (_dst.isGpuMat())
            reinterpret_cast<cuda::GpuMat*>(obj)->flags = stype;
        بازگشت;
    }

    _dst.create( dims, size.p, stype );
    اگر (empty())
        بازگشت;

    size_t sz[CV_MAX_DIM] = {1}, srcofs[CV_MAX_DIM]={0}, dstofs[CV_MAX_DIM]={0};
    size_t esz = CV_ELEM_SIZE(stype);
    صحیح i, d = std::max(dims, 1);
    برای( i = 0; i < d; i++ )
        sz[i] = size.p[i];
    sz[d-1] *= esz;
    ndoffset(srcofs);
    srcofs[d-1] *= esz;

    اگر( _dst.isUMat() )
    {
        UMat dst = _dst.getUMat();
        CV_Assert(dst.u);
        اگر( u == dst.u && dst.offset == offset )
            بازگشت;

        اگر (u->currAllocator == dst.u->currAllocator)
        {
            dst.ndoffset(dstofs);
            dstofs[dims-1] *= esz;
            u->currAllocator->copy(u, dst.u, d, sz, srcofs, step.p, dstofs, dst.step.p, نادرست);
            بازگشت;
        }

        // different allocators: never map a device destination, upload into it instead
        // (through a host Mat when both sides are device memory)
        MatAllocator* hostAllocator = Mat::getDefaultAllocator();
        اگر (dst.u->currAllocator != hostAllocator)
        {
            اگر (u->currAllocator == hostAllocator)
                getMat(ACCESS_READ).copyTo(_dst);
            وگرنه
            {
                Mat tmp;
                copyTo(tmp);
                tmp.copyTo(_dst);
            }
            بازگشت;
        }
    }

    Mat dst = _dst.getMat();
    u->currAllocator->download(u, dst.ptr(), d, sz, srcofs, step.p, dst.step.p);
}

تهی UMat::copyTo(OutputArray _dst, درون_داد_Array _mask) ثابت
{
    CV_INSTRUMENT_REGION();

    اگر( _mask.empty() )
    {
        copyTo(_dst);
        بازگشت;
    }
#ifdef HAVE_OPENCL
    صحیح cn = channels(), mtype = _mask.type(), mdepth = CV_MAT_DEPTH(mtype), mcn = CV_MAT_CN(mtype);
    CV_Assert( (mdepth == CV_8U || mdepth == CV_8S || mdepth == CV_Bool) && (mcn == 1 || mcn == cn) );

    اگر (ocl::useOpenCL() && _dst.isUMat() && dims <= 2)
    {
        UMatData * prevu = _dst.getUMat().u;
        _dst.create( size, type() );

        UMat dst = _dst.getUMat();

        منطقی haveDstUninit = نادرست;
        اگر( prevu != dst.u ) // do not leave dst uninitialized
            haveDstUninit = درست;

        String opts = format("-D COPY_TO_MASK -D T1=%s -D scn=%d -D mcn=%d%s",
                             ocl::memopTypeToStr(depth()), cn, mcn,
                             haveDstUninit ? " -D HAVE_DST_UNINIT" : "");

        ocl::Kernel k("copyToMask", ocl::core::copyset_oclsrc, opts);
        اگر (!k.empty())
        {
            k.args(ocl::KernelArg::ReadOnlyNoSize(*این),
                   ocl::KernelArg::ReadOnlyNoSize(_mask.getUMat()),
                   haveDstUninit ? ocl::KernelArg::WriteOnly(dst) :
                                   ocl::KernelArg::ReadWrite(dst));

            size_t globalsize[2] = { (size_t)cols, (size_t)rows };
            اگر (k.run(2, globalsize, پوچ, نادرست))
            {
                CV_IMPL_ADD(CV_IMPL_OCL);
                بازگشت;
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

UMat& UMat::setTo(درون_داد_Array مقدار, درون_داد_Array _mask)
{
    CV_INSTRUMENT_REGION();

    منطقی haveMask = !_mask.empty();
#ifdef HAVE_OPENCL
    صحیح tp = type(), cn = CV_MAT_CN(tp), d = CV_MAT_DEPTH(tp);

    اگر( dims <= 2 && cn <= 4 && CV_MAT_DEPTH(tp) < CV_64F && ocl::useOpenCL() )
    {
        Mat value = مقدار.getMat();
        CV_Assert( checkScalar(value, type(), مقدار.kind(), درون_داد_Array_2::UMAT) );
        صحیح kercn = haveMask || cn == 3 ? cn : std::max(cn, ocl::predictOptimalVectorWidth(*این)),
                kertp = CV_MAKE_TYPE(d, kercn);

        دوگانه buf[16] = { 0, 0, 0, 0, 0, 0, 0, 0,
                           0, 0, 0, 0, 0, 0, 0, 0 };
        convertAndUnrollScalar(value, tp, (uchar *)buf, kercn / cn);

        صحیح scalarcn = kercn == 3 ? 4 : kercn, rowsPerWI = ocl::Device::getDefault().isIntel() ? 4 : 1;
        String opts = format("-D dstT=%s -D rowsPerWI=%d -D dstST=%s -D dstT1=%s -D cn=%d",
                             ocl::memopTypeToStr(kertp), rowsPerWI,
                             ocl::memopTypeToStr(CV_MAKETYPE(d, scalarcn)),
                             ocl::memopTypeToStr(d), kercn);

        ocl::Kernel setK(haveMask ? "setMask" : "set", ocl::core::copyset_oclsrc, opts);
        اگر( !setK.empty() )
        {
            ocl::KernelArg scalararg(ocl::KernelArg::CONSTANT, 0, 0, 0, buf, CV_ELEM_SIZE(d) * scalarcn);
            UMat mask;

            اگر( haveMask )
            {
                mask = _mask.getUMat();
                CV_Assert( mask.size() == size() && (mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool) );
                ocl::KernelArg maskarg = ocl::KernelArg::ReadOnlyNoSize(mask),
                        dstarg = ocl::KernelArg::ReadWrite(*این);
                setK.args(maskarg, dstarg, scalararg);
            }
            وگرنه
            {
                ocl::KernelArg dstarg = ocl::KernelArg::WriteOnly(*این, cn, kercn);
                setK.args(dstarg, scalararg);
            }

            size_t globalsize[] = { (size_t)cols * cn / kercn, ((size_t)rows + rowsPerWI - 1) / rowsPerWI };
            اگر( setK.run(2, globalsize, پوچ, نادرست) )
            {
                CV_IMPL_ADD(CV_IMPL_OCL);
                بازگشت *این;
            }
        }
    }
#endif
    Mat m = getMat(haveMask ? ACCESS_RW : ACCESS_WRITE);
    m.setTo(مقدار, _mask);
    بازگشت *این;
}

UMat& UMat::operator = (ثابت Scalar& s)
{
    setTo(s);
    بازگشت *این;
}

UMat UMat::t() ثابت
{
    UMat m;
    transpose(*این, m);
    بازگشت m;
}

UMat UMat::zeros(صحیح rows, صحیح cols, صحیح type, UMatUsageFlags usageFlags)
{
    بازگشت UMat(rows, cols, type, Scalar::all(0), usageFlags);
}

UMat UMat::zeros(Size size, صحیح type, UMatUsageFlags usageFlags)
{
    بازگشت UMat(size, type, Scalar::all(0), usageFlags);
}

UMat UMat::zeros(صحیح ndims, ثابت صحیح* sz, صحیح type, UMatUsageFlags usageFlags)
{
    بازگشت UMat(ndims, sz, type, Scalar::all(0), usageFlags);
}

UMat UMat::zeros(ثابت MatShape& shape, صحیح type, UMatUsageFlags usageFlags)
{
    بازگشت UMat(shape.dims, shape.p, type, Scalar::all(0), usageFlags);
}

UMat UMat::ones(صحیح rows, صحیح cols, صحیح type, UMatUsageFlags usageFlags)
{
    بازگشت UMat(rows, cols, type, Scalar(1), usageFlags);
}

UMat UMat::ones(Size size, صحیح type, UMatUsageFlags usageFlags)
{
    بازگشت UMat(size, type, Scalar(1), usageFlags);
}

UMat UMat::ones(صحیح ndims, ثابت صحیح* sz, صحیح type, UMatUsageFlags usageFlags)
{
    بازگشت UMat(ndims, sz, type, Scalar(1), usageFlags);
}

UMat UMat::ones(ثابت MatShape& shape, صحیح type, UMatUsageFlags usageFlags)
{
    بازگشت UMat(shape.dims, shape.p, type, Scalar(1), usageFlags);
}

}

/* End of file. */
