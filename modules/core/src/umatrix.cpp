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

#அந்தர்பாவ "precomp.hpp"
#அந்தர்பாவ "opencl_kernels_core.hpp"
#அந்தர்பாவ "umatrix.hpp"

#அந்தர்பாவ <opencv2/core/utils/tls.hpp>

///////////////////////////////// UMat implementation ///////////////////////////////

நாமாகாஶ cv {

// forward decls, implementation is below in this file
வெறுமை setSize(UMat& m, முழு _dims, மாறிலி முழு* _sz, மாறிலி size_t* _steps,
             தருக்க autoSteps = பொய்);

வெறுமை updateContinuityFlag(UMat& m);
வெறுமை finalizeHdr(UMat& m);

UMatData::UMatData(மாறிலி MatAllocator* allocator)
{
    prevAllocator = currAllocator = allocator;
    urefcount = refcount = mapcount = 0;
    data = origdata = 0;
    size = 0;
    flags = static_cast<UMatData::MemoryFlag>(0);
    handle = 0;
    userdata = 0;
    allocatorFlags_ = 0;
    originalUMatData = வெற்று;
}

UMatData::~UMatData()
{
    prevAllocator = currAllocator = 0;
    urefcount = refcount = 0;
    CV_Assert(mapcount == 0);
    data = origdata = 0;
    size = 0;
    தருக்க isAsyncCleanup = !!(flags & UMatData::ASYNC_CLEANUP);
    flags = static_cast<UMatData::MemoryFlag>(0);
    handle = 0;
    userdata = 0;
    allocatorFlags_ = 0;
    எனில் (originalUMatData)
    {
        தருக்க showWarn = பொய்;
        UMatData* u = originalUMatData;
        தருக்க zero_Ref = CV_XADD(&(u->refcount), -1) == 1;
        எனில் (zero_Ref)
        {
            // simulate Mat::deallocate
            எனில் (u->mapcount != 0)
            {
                (u->currAllocator ? u->currAllocator : /* TODO allocator ? allocator :*/ Mat::getDefaultAllocator())->unmap(u);
            }
            இல்லையெனில்
            {
                // we don't do "map", so we can't do "unmap"
            }
        }
        தருக்க zero_URef = CV_XADD(&(u->urefcount), -1) == 1;
        எனில் (zero_Ref && !zero_URef)
            showWarn = உண்மை;
        எனில் (zero_Ref && zero_URef) // oops, we need to free resources
        {
            showWarn = !isAsyncCleanup;
            // simulate UMat::deallocate
            u->currAllocator->deallocate(u);
        }
#ifndef NDEBUG
        எனில் (showWarn)
        {
            நிலையான முழு warn_message_showed = 0;
            எனில் (warn_message_showed++ < 100)
            {
                fflush(stdout);
                fprintf(stderr, "\n! OPENCV warning: getUMat()/getMat() call chain possible problem."
                                "\n!                 Base object is dead, while nested/derived object is still alive or processed."
                                "\n!                 Please check lifetime of UMat/Mat objects!\n");
                fflush(stderr);
            }
        }
#இல்லையெனில்
        CV_UNUSED(showWarn);
#endif
        originalUMatData = வெற்று;
    }
}

#ifndef OPENCV_DISABLE_THREAD_SUPPORT

// it should be a prime number for the best hash function
எண்ணிக்கை { UMAT_NLOCKS = 31 };
நிலையான Mutex umatLocks[UMAT_NLOCKS];

நிலையான size_t getUMatDataLockIndex(மாறிலி UMatData* u)
{
    size_t idx = ((size_t)(வெறுமை*)u) % UMAT_NLOCKS;
    திரும்பு idx;
}

வெறுமை UMatData::lock()
{
    size_t idx = getUMatDataLockIndex(இது);
    //printf("%d lock(%d)\n", cv::utils::getThreadID(), (int)idx);
    umatLocks[idx].lock();
}

வெறுமை UMatData::unlock()
{
    size_t idx = getUMatDataLockIndex(இது);
    //printf("%d unlock(%d)\n", cv::utils::getThreadID(), (int)idx);
    umatLocks[idx].unlock();
}


// Do not allow several lock() calls with different UMatData objects.
கட்டமைப்பு UMatDataAutoLocker
{
    முழு usage_மொத்தம்;
    UMatData* locked_objects[2];
    UMatDataAutoLocker() : usage_மொத்தம்(0) { locked_objects[0] = வெற்று; locked_objects[1] = வெற்று; }

    வெறுமை lock(UMatData*& u1)
    {
        தருக்க locked_1 = (u1 == locked_objects[0] || u1 == locked_objects[1]);
        எனில் (locked_1)
        {
            u1 = வெற்று;
            திரும்பு;
        }
        CV_Assert(usage_மொத்தம் == 0);  // UMatDataAutoLock can't be used multiple times from the same thread
        usage_மொத்தம் = 1;
        locked_objects[0] = u1;
        u1->lock();
    }
    வெறுமை lock(UMatData*& u1, UMatData*& u2)
    {
        தருக்க locked_1 = (u1 == locked_objects[0] || u1 == locked_objects[1]);
        தருக்க locked_2 = (u2 == locked_objects[0] || u2 == locked_objects[1]);
        எனில் (locked_1)
            u1 = வெற்று;
        எனில் (locked_2)
            u2 = வெற்று;
        எனில் (locked_1 && locked_2)
            திரும்பு;
        CV_Assert(usage_மொத்தம் == 0);  // UMatDataAutoLock can't be used multiple times from the same thread
        usage_மொத்தம் = 1;
        locked_objects[0] = u1;
        locked_objects[1] = u2;
        எனில் (u1)
            u1->lock();
        எனில் (u2)
            u2->lock();
    }
    வெறுமை release(UMatData* u1, UMatData* u2)
    {
        எனில் (u1 == வெற்று && u2 == வெற்று)
            திரும்பு;
        CV_Assert(usage_மொத்தம் == 1);
        usage_மொத்தம் = 0;
        எனில் (u1)
            u1->unlock();
        எனில் (u2)
            u2->unlock();
        locked_objects[0] = வெற்று; locked_objects[1] = வெற்று;
    }
};
நிலையான TLSData<UMatDataAutoLocker>& getUMatDataAutoLockerTLS()
{
    CV_SINGLETON_LAZY_INIT_REF(TLSData<UMatDataAutoLocker>, புதிய TLSData<UMatDataAutoLocker>());
}
நிலையான UMatDataAutoLocker& getUMatDataAutoLocker() { திரும்பு getUMatDataAutoLockerTLS().getRef(); }


UMatDataAutoLock::UMatDataAutoLock(UMatData* u) : u1(u), u2(வெற்று)
{
    getUMatDataAutoLocker().lock(u1);
}
UMatDataAutoLock::UMatDataAutoLock(UMatData* u1_, UMatData* u2_) : u1(u1_), u2(u2_)
{
    எனில் (getUMatDataLockIndex(u1) > getUMatDataLockIndex(u2))
    {
        std::swap(u1, u2);
    }
    getUMatDataAutoLocker().lock(u1, u2);
}
UMatDataAutoLock::~UMatDataAutoLock()
{
    getUMatDataAutoLocker().release(u1, u2);
}

#இல்லையெனில்

வெறுமை UMatData::lock()
{
    // nothing in OPENCV_DISABLE_THREAD_SUPPORT mode
}

வெறுமை UMatData::unlock()
{
    // nothing in OPENCV_DISABLE_THREAD_SUPPORT mode
}

UMatDataAutoLock::UMatDataAutoLock(UMatData* u) : u1(u), u2(வெற்று)
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

UMat::UMat(முழு _rows, முழு _cols, முழு _type, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_rows, _cols, _type);
}

UMat::UMat(முழு _rows, முழு _cols, முழு _type, மாறிலி Scalar& _s, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_rows, _cols, _type);
    *இது = _s;
}

UMat::UMat(Size _sz, முழு _type, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create( _sz.height, _sz.width, _type );
}

UMat::UMat(Size _sz, முழு _type, மாறிலி Scalar& _s, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_sz.height, _sz.width, _type);
    *இது = _s;
}

UMat::UMat(முழு _dims, மாறிலி முழு* _sz, முழு _type, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_dims, _sz, _type);
}

UMat::UMat(முழு _dims, மாறிலி முழு* _sz, முழு _type, மாறிலி Scalar& _s, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_dims, _sz, _type);
    *இது = _s;
}

UMat::UMat(மாறிலி MatShape& _shape, முழு _type, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_shape.dims, _shape.p, _type);
}

UMat::UMat(மாறிலி MatShape& _shape, முழு _type, மாறிலி Scalar& _s, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_shape.dims, _shape.p, _type);
    *இது = _s;
}

UMat::UMat(மாறிலி UMat& m)
: flags(m.flags), dims(m.dims), rows(m.rows), cols(m.cols), allocator(m.allocator),
  usageFlags(m.usageFlags), u(m.u), offset(m.offset), size(m.size), step(m.step)
{
    addref();
}

UMat& UMat::operator=(மாறிலி UMat& m)
{
    எனில்( இது != &m )
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
    திரும்பு *இது;
}

UMat UMat::clone() மாறிலி
{
    UMat m;
    copyTo(m);
    திரும்பு m;
}

வெறுமை UMat::assignTo(UMat& m, முழு _type) மாறிலி
{
    எனில்( _type < 0 )
        m = *இது;
    இல்லையெனில்
        convertTo(m, _type);
}

வெறுமை UMat::create(முழு _rows, முழு _cols, முழு _type, UMatUsageFlags _usageFlags)
{
    முழு sz[] = {_rows, _cols};
    create(2, sz, _type, _usageFlags);
}

வெறுமை UMat::create(Size _sz, முழு _type, UMatUsageFlags _usageFlags)
{
    create(_sz.height, _sz.width, _type, _usageFlags);
}

வெறுமை UMat::createSameSize(உள்ளீடு_Array arr, முழு _type, UMatUsageFlags _usageFlags)
{
    create(arr.shape(), _type, _usageFlags);
}

வெறுமை UMat::addref()
{
    எனில்( u )
        CV_XADD(&(u->urefcount), 1);
}

வெறுமை UMat::release()
{
    எனில்( u && CV_XADD(&(u->urefcount), -1) == 1 )
        deallocate();
    u = 0;
    size.clear();
    dims = cols = rows = 0;
}

தருக்க UMat::empty() மாறிலி
{
    திரும்பு u == 0 || total() == 0;
}

size_t UMat::total() மாறிலி
{
    எனில்( dims <= 2 )
        திரும்பு (size_t)rows * cols;
    size_t p = 1;
    க்கு( முழு i = 0; i < dims; i++ )
        p *= size[i];
    திரும்பு p;
}

MatShape UMat::shape() மாறிலி
{
    திரும்பு size;
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
    m.allocator = வெற்று;
    m.u = வெற்று;
    m.offset = 0;
}

UMat& UMat::operator=(UMat&& m)
{
    எனில் (இது == &m)
      திரும்பு *இது;
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
    m.allocator = வெற்று;
    m.u = வெற்று;
    m.offset = 0;
    திரும்பு *இது;
}


MatAllocator* UMat::getStdAllocator()
{
#ifdef HAVE_OPENCL
    எனில் (ocl::useOpenCL())
        திரும்பு ocl::getOpenCLAllocator();
#endif
    திரும்பு Mat::getDefaultAllocator();
}

வெறுமை swap( UMat& a, UMat& b )
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


வெறுமை setSize( UMat& m, முழு _dims, மாறிலி முழு* _sz,
              மாறிலி size_t* _steps, தருக்க autoSteps )
{
    CV_Assert( 0 <= _dims && _dims <= CV_MAX_DIM && _dims <= MatShape::MAX_DIMS);

    m.dims = _dims;
    m.size = MatShape(_dims, _sz);
    m.step[std::max(_dims-1, 0)] = CV_ELEM_SIZE(m.flags);
    க்கு (முழு i = _dims-2; i >= 0; i--) {
        size_t autostep = m.size[i+1]*m.step[i+1];
        எனில் (_steps) {
            m.step[i] = _steps[i];
            //CV_Assert(m.step[i] >= autostep);
        } இல்லையெனில் எனில் (autoSteps) {
            m.step[i] = autostep;
        } இல்லையெனில் {
            m.step[i] = 0;
        }
    }

    எனில்( _dims <= 2 )
    {
        m.cols = _dims == 0 ? 1 : _sz ? _sz[_dims > 1] : 0;
        m.rows = _dims < 2 ? 1 : _sz ? _sz[0] : 0;
    } இல்லையெனில் {
        m.cols = m.rows = -1;
    }
}


வெறுமை UMat::updateContinuityFlag()
{
    flags = cv::updateContinuityFlag(flags, dims, size.p, step.p);
}


வெறுமை finalizeHdr(UMat& m)
{
    m.updateContinuityFlag();
    முழு d = m.dims;
    எனில்( d > 2 )
        m.rows = m.cols = -1;
}


UMat Mat::getUMat(AccessFlag accessFlags, UMatUsageFlags usageFlags) மாறிலி
{
    UMat hdr;
    எனில்(!data)
        திரும்பு hdr;
    எனில் (data != datastart)
    {
        Size wholeSize;
        Point ofs;
        locateROI(wholeSize, ofs);
        Size sz(cols, rows);
        எனில் (ofs.x != 0 || ofs.y != 0)
        {
            Mat src = *இது;
            முழு dtop = ofs.y;
            முழு dbottom = wholeSize.height - src.rows - ofs.y;
            முழு dleft = ofs.x;
            முழு dright = wholeSize.width - src.cols - ofs.x;
            src.adjustROI(dtop, dbottom, dleft, dright);
            திரும்பு src.getUMat(accessFlags, usageFlags)(cv::Rect(ofs.x, ofs.y, sz.width, sz.height));
        }
    }
    CV_Assert(data == datastart);

    accessFlags |= ACCESS_RW;
    UMatData* new_u = வெற்று;
    MatStep new_step = step;
    {
        MatAllocator *a = allocator, *a0 = getDefaultAllocator();
        எனில்(!a)
            a = a0;
        new_u = a->allocate(dims, size.p, type(), data, new_step.p, accessFlags, usageFlags);
        new_u->originalUMatData = u;
    }
    தருக்க allocated = பொய்;
    முயல்
    {
        allocated = UMat::getStdAllocator()->allocate(new_u, accessFlags, usageFlags);
    }
    பிடி (மாறிலி cv::Exception& e)
    {
        fprintf(stderr, "Exception: %s\n", e.what());
    }
    எனில் (!allocated)
    {
        allocated = getDefaultAllocator()->allocate(new_u, accessFlags, usageFlags);
        CV_Assert(allocated);
    }
    எனில் (u != வெற்று)
    {
#ifdef HAVE_OPENCL
        எனில் (ocl::useOpenCL() && new_u->currAllocator == ocl::getOpenCLAllocator())
        {
            CV_Assert(new_u->tempUMat());
        }
#endif
        CV_XADD(&(u->refcount), 1);
        CV_XADD(&(u->urefcount), 1);
    }
    முயல்
    {
        hdr.flags = flags;
        hdr.usageFlags = usageFlags;
        setSize(hdr, dims, size.p, new_step.p);
        finalizeHdr(hdr);
        hdr.u = new_u;
        hdr.offset = 0; //data - datastart;
        hdr.addref();
        திரும்பு hdr;
    }
    பிடி(...)
    {
        எனில் (u != வெற்று)
        {
            CV_XADD(&(u->refcount), -1);
            CV_XADD(&(u->urefcount), -1);
        }
        new_u->currAllocator->deallocate(new_u);
        எறி;
    }

}

வெறுமை UMat::create(முழு d0, மாறிலி முழு* _sizes, முழு _type, UMatUsageFlags _usageFlags)
{
    முழு sz1 = 1, d = d0;
    முழு i;
    எனில் (d == 0) {
        d = 1;
        _sizes = (மாறிலி முழு*)&sz1;
    }
    CV_Assert(0 <= d && d <= CV_MAX_DIM && _sizes);
    _type = CV_MAT_TYPE(_type);

    // if param value is USAGE_DEFAULT by implicit default param value -or- explicit value
    // ...then don't change the existing usageFlags
    // it is not possible to change usage from non-default to USAGE_DEFAULT through create()
    // ...instead must construct UMat()
    எனில் (_usageFlags == cv::USAGE_DEFAULT)
    {
        _usageFlags = usageFlags;
    }

    எனில்( u && d == dims && _type == type() && _usageFlags == usageFlags )
    {
        க்கு( i = 0; i < d; i++ )
            எனில்( size[i] != _sizes[i] )
                நிறுத்து;
        எனில்( i == d )
            திரும்பு;
    }

    முழு _sizes_backup[CV_MAX_DIM]; // #5991
    எனில் (_sizes == (இது->size.p))
    {
        க்கு(i = 0; i < d; i++ )
            _sizes_backup[i] = _sizes[i];
        _sizes = _sizes_backup;
    }

    release();
    usageFlags = _usageFlags;
    எனில்( d == 0 )
        திரும்பு;
    flags = (_type & CV_MAT_TYPE_MASK) | MAGIC_VAL;
    setSize(*இது, d, _sizes, 0, உண்மை);
    offset = 0;

    எனில்( total() > 0 )
    {
        MatAllocator *a = allocator, *a0 = getStdAllocator();
        எனில் (!a)
        {
            a = a0;
            a0 = Mat::getDefaultAllocator();
        }
        முயல்
        {
            u = a->allocate(dims, size.p, _type, 0, step.p, ACCESS_RW /* ignored */, usageFlags);
            CV_Assert(u != 0);
        }
        பிடி(...)
        {
            எனில்(a != a0)
                u = a0->allocate(dims, size.p, _type, 0, step.p, ACCESS_RW /* ignored */, usageFlags);
            CV_Assert(u != 0);
        }
        CV_Assert( step[dims-1] == (size_t)CV_ELEM_SIZE(flags) );
    }

    finalizeHdr(*இது);
    addref();
    size.dims = dims = d0;
}

வெறுமை UMat::create(மாறிலி std::vector<முழு>& _sizes, முழு _type, UMatUsageFlags _usageFlags)
{
    create((முழு)_sizes.size(), _sizes.data(), _type, _usageFlags);
}

வெறுமை UMat::create(மாறிலி MatShape& _shape, முழு _type, UMatUsageFlags _usageFlags)
{
    எனில் (_shape.dims < 0) {
        release();
    } இல்லையெனில் {
        create(_shape.dims, _shape.p, _type, _usageFlags);
        size.layout = _shape.layout;
        size.C = _shape.C;
    }
}

வெறுமை UMat::fit(முழு _dims, மாறிலி முழு* _sizes, முழு _type, UMatUsageFlags _usageFlags)
{
    எனில் (_usageFlags == cv::USAGE_DEFAULT)
        _usageFlags = usageFlags;
    size_t oldTotalBytes = u ? u->size : 0;
    size_t esz = CV_ELEM_SIZE(_type), newTotal = _dims >= 0;
    க்கு (முழு i = 0; i < _dims; i++)
        newTotal *= _sizes[i];
    size_t newTotalBytes = newTotal*esz;
    எனில் (newTotalBytes > 0 &&
        (!isContinuous() ||
         newTotalBytes > oldTotalBytes ||
         offset != 0 ||
         _usageFlags != usageFlags)) {
        create(_dims, _sizes, _type, _usageFlags);
    } இல்லையெனில் {
        flags = (flags & ~Mat::TYPE_MASK) | CV_MAT_TYPE(_type);
        முழு _dummy_size = 0;
        setSize(*இது, (_dims >= 0 ? _dims : 1), (_dims >= 0 ? _sizes : &_dummy_size), nullptr, உண்மை);
        finalizeHdr(*இது);
    }
}

வெறுமை UMat::fit(மாறிலி std::vector<முழு>& _shape, முழு _type, UMatUsageFlags _usageFlags)
{
    fit((முழு)_shape.size(), _shape.data(), _type, _usageFlags);
}

வெறுமை UMat::fit(மாறிலி MatShape& _shape, முழு _type, UMatUsageFlags _usageFlags)
{
    fit(_shape.dims, _shape.p, _type, _usageFlags);
    size.layout = _shape.layout;
    size.C = _shape.C;
}

வெறுமை UMat::fit(முழு _rows, முழு _cols, முழு _type, UMatUsageFlags _usageFlags)
{
    _type &= TYPE_MASK;
    முழு sz[] = {_rows, _cols};
    fit(2, sz, _type, _usageFlags);
}

வெறுமை UMat::fit(Size _sz, முழு _type, UMatUsageFlags _usageFlags)
{
    fit(_sz.height, _sz.width, _type, _usageFlags);
}

வெறுமை UMat::fitSameSize(உள்ளீடு_Array m, முழு _type, UMatUsageFlags _usageFlags)
{
    fit(m.shape(), _type, _usageFlags);
}

வெறுமை UMat::copySize(மாறிலி UMat& m)
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

வெறுமை UMat::deallocate()
{
    UMatData* u_ = u;
    u = வெற்று;
    u_->currAllocator->deallocate(u_);
}


UMat::UMat(மாறிலி UMat& m, மாறிலி Range& _rowRange, மாறிலி Range& _colRange)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(USAGE_DEFAULT), u(0), offset(0)
{
    CV_Assert( m.dims >= 2 && m.dims <= MatShape::MAX_DIMS );
    எனில்( m.dims > 2 )
    {
        Range rs[MatShape::MAX_DIMS];
        rs[0] = _rowRange;
        rs[1] = _colRange;
        க்கு( முழு i = 2; i < m.dims; i++ )
            rs[i] = Range::all();
        *இது = m(rs);
        திரும்பு;
    }

    *இது = m;
    எனில்( _rowRange != Range::all() && _rowRange != Range(0,rows) )
    {
        CV_Assert( 0 <= _rowRange.start &&
                  _rowRange.start <= _rowRange.end &&
                  _rowRange.end <= m.rows );
        size[0] = rows = _rowRange.size();
        offset += step*_rowRange.start;
        flags |= SUBMATRIX_FLAG;
    }

    எனில்( _colRange != Range::all() && _colRange != Range(0,cols) )
    {
        CV_Assert( 0 <= _colRange.start &&
                  _colRange.start <= _colRange.end &&
                  _colRange.end <= m.cols );
        size[1] = cols = _colRange.size();
        offset += _colRange.start*elemSize();
        flags |= SUBMATRIX_FLAG;
    }

    updateContinuityFlag();

    எனில்( rows <= 0 || cols <= 0 )
    {
        release();
        rows = cols = 0;
    }
}


UMat::UMat(மாறிலி UMat& m, மாறிலி Rect& roi)
    : flags(m.flags), dims(2), rows(roi.height), cols(roi.width),
    allocator(m.allocator), usageFlags(m.usageFlags), u(m.u),
    offset(m.offset + roi.y*m.step[0]), size(2)
{
    CV_Assert( m.dims <= 2 );

    size_t esz = CV_ELEM_SIZE(flags);
    offset += roi.x*esz;
    CV_Assert( 0 <= roi.x && 0 <= roi.width && roi.x + roi.width <= m.cols &&
              0 <= roi.y && 0 <= roi.height && roi.y + roi.height <= m.rows );
    எனில்( roi.width < m.cols || roi.height < m.rows )
        flags |= SUBMATRIX_FLAG;

    size[0] = rows;
    size[1] = cols;
    step[0] = m.dims == 2 ? m.step[0] : cols*esz;
    step[1] = esz;

    updateContinuityFlag();

    addref();
    எனில்( rows <= 0 || cols <= 0 )
    {
        rows = cols = 0;
        release();
    }
}


UMat::UMat(மாறிலி UMat& m, மாறிலி Range* ranges)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(USAGE_DEFAULT), u(0), offset(0)
{
    முழு i, d = m.dims;

    CV_Assert(ranges);
    க்கு( i = 0; i < d; i++ )
    {
        Range r = ranges[i];
        CV_Assert( r == Range::all() || (0 <= r.start && r.start < r.end && r.end <= m.size[i]) );
    }
    *இது = m;
    க்கு( i = 0; i < d; i++ )
    {
        Range r = ranges[i];
        எனில்( r != Range::all() && r != Range(0, size.p[i]))
        {
            size.p[i] = r.end - r.start;
            offset += r.start*step.p[i];
            flags |= SUBMATRIX_FLAG;
        }
    }

    எனில் (d <= 2) {
        rows = d == 2 ? size[0] : 1;
        cols = d <= 0 ? (d >= 0) : size[d > 1];
    }
    updateContinuityFlag();
}

UMat::UMat(மாறிலி UMat& m, மாறிலி std::vector<Range>& ranges)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(USAGE_DEFAULT), u(0), offset(0)
{
    முழு i, d = m.dims;

    CV_Assert((முழு)ranges.size() == d);
    க்கு (i = 0; i < d; i++)
    {
        Range r = ranges[i];
        CV_Assert(r == Range::all() || (0 <= r.start && r.start < r.end && r.end <= m.size[i]));
    }
    *இது = m;
    க்கு (i = 0; i < d; i++)
    {
        Range r = ranges[i];
        எனில் (r != Range::all() && r != Range(0, size.p[i]))
        {
            size.p[i] = r.end - r.start;
            offset += r.start*step.p[i];
            flags |= SUBMATRIX_FLAG;
        }
    }

    எனில் (d <= 2) {
        rows = d == 2 ? size[0] : 1;
        cols = d <= 0 ? (d >= 0) : size[d > 1];
    }
    updateContinuityFlag();
}

UMat UMat::diag(முழு d) மாறிலி
{
    CV_Assert( dims <= 2 );
    UMat m = *இது;
    size_t esz = elemSize();
    முழு len;

    எனில்( d >= 0 )
    {
        len = std::min(cols - d, rows);
        m.offset += esz*d;
    }
    இல்லையெனில்
    {
        len = std::min(rows + d, cols);
        m.offset -= step[0]*d;
    }
    CV_DbgAssert( len > 0 );

    m.size[0] = m.rows = len;
    m.size[1] = m.cols = 1;
    m.step[0] += (len > 1 ? esz : 0);

    m.updateContinuityFlag();

    எனில்( size() != Size(1,1) )
        m.flags |= SUBMATRIX_FLAG;

    திரும்பு m;
}

வெறுமை UMat::locateROI( Size& wholeSize, Point& ofs ) மாறிலி
{
    CV_Assert( dims <= 2 && step[0] > 0 );
    size_t esz = elemSize(), minstep;
    ptrdiff_t delta1 = (ptrdiff_t)offset, delta2 = (ptrdiff_t)u->size;

    எனில்( delta1 == 0 )
        ofs.x = ofs.y = 0;
    இல்லையெனில்
    {
        ofs.y = (முழு)(delta1/step[0]);
        ofs.x = (முழு)((delta1 - step[0]*ofs.y)/esz);
        CV_DbgAssert( offset == (size_t)(ofs.y*step[0] + ofs.x*esz) );
    }
    minstep = (ofs.x + cols)*esz;
    wholeSize.height = (முழு)((delta2 - minstep)/step[0] + 1);
    wholeSize.height = std::max(wholeSize.height, ofs.y + rows);
    wholeSize.width = (முழு)((delta2 - step*(wholeSize.height-1))/esz);
    wholeSize.width = std::max(wholeSize.width, ofs.x + cols);
}


UMat& UMat::adjustROI( முழு dtop, முழு dbottom, முழு dleft, முழு dright )
{
    CV_Assert( dims == 2 && step[0] > 0 );
    Size wholeSize; Point ofs;
    size_t esz = elemSize();
    locateROI( wholeSize, ofs );
    முழு row1 = std::min(std::max(ofs.y - dtop, 0), wholeSize.height), row2 = std::max(0, std::min(ofs.y + rows + dbottom, wholeSize.height));
    முழு col1 = std::min(std::max(ofs.x - dleft, 0), wholeSize.width), col2 = std::max(0, std::min(ofs.x + cols + dright, wholeSize.width));
    எனில்(row1 > row2)
        std::swap(row1, row2);
    எனில்(col1 > col2)
        std::swap(col1, col2);

    offset += (row1 - ofs.y)*step + (col1 - ofs.x)*esz;
    rows = row2 - row1; cols = col2 - col1;
    size.p[0] = rows; size.p[1] = cols;
    updateContinuityFlag();
    திரும்பு *இது;
}


UMat UMat::reshape(முழு new_cn, முழு new_rows) மாறிலி
{
    முழு cn = channels();
    UMat hdr = *இது;

    எனில்( dims > 2 && new_rows == 0 && new_cn != 0 && size[dims-1]*cn % new_cn == 0 )
    {
        hdr.flags = (hdr.flags & ~CV_MAT_CN_MASK) | ((new_cn-1) << CV_CN_SHIFT);
        hdr.step[dims-1] = CV_ELEM_SIZE(hdr.flags);
        hdr.size[dims-1] = hdr.size[dims-1]*cn / new_cn;
        திரும்பு hdr;
    }

    CV_Assert( dims <= 2 );

    எனில்( new_cn == 0 )
        new_cn = cn;

    முழு total_width = cols * cn;

    எனில்( (new_cn > total_width || total_width % new_cn != 0) && new_rows == 0 )
        new_rows = rows * total_width / new_cn;

    எனில்( new_rows != 0 && new_rows != rows )
    {
        முழு total_size = total_width * rows;
        எனில்( !isContinuous() )
            CV_Error( cv::Error::BadStep,
            "The matrix is not continuous, thus its number of rows can not be changed" );

        total_width = total_size / new_rows;

        எனில்( total_width * new_rows != total_size )
            CV_Error( cv::Error::StsBadArg, "The total number of matrix elements "
                                    "is not divisible by the new number of rows" );

        hdr.size[0] = hdr.rows = new_rows;
        hdr.step[0] = total_width * elemSize1();
    } இல்லையெனில் {
        hdr.size[0] = hdr.rows = rows;
        எனில் (dims <= 1)
            hdr.step[0] = cols * CV_ELEM_SIZE(flags);
    }

    முழு new_width = total_width / new_cn;

    எனில்( new_width * new_cn != total_width )
        CV_Error( cv::Error::Bad_இலக்கம்_Channels,
        "The total width is not divisible by the new number of channels" );

    hdr.size.dims = hdr.dims = 2;
    hdr.size[1] = hdr.cols = new_width;
    hdr.flags = (hdr.flags & ~CV_MAT_CN_MASK) | ((new_cn-1) << CV_CN_SHIFT);
    hdr.step[1] = CV_ELEM_SIZE(hdr.flags);
    திரும்பு hdr;
}

#ifdef HAVE_OPENCL
நாமாகாஶ {
நிலையான தருக்க ocl_setDiag(மாறிலி UMat& d, UMat& m, முழு len)
{
    முழு cn = d.channels();
    முழு depth = d.depth();

    எனில் (depth == CV_64F && !ocl::Device::getDefault().doubleFPConfig())
        திரும்பு பொய்;

    String opts = format("-D SET_DIAG -D T1=%s -D cn=%d -D IS_ROW_VECTOR=%d",
                         ocl::memopTypeToStr(depth),
                         cn,
                         (d.rows == 1) ? 1 : 0);

    ocl::Kernel k("setDiag", ocl::core::copyset_oclsrc, opts);
    எனில் (k.empty())
        திரும்பு பொய்;

    k.args(ocl::KernelArg::WriteOnly(m),
           ocl::KernelArg::ReadOnlyNoSize(d),
           len);

    size_t globalsize[2] = { (size_t)len, (size_t)len };
    திரும்பு k.run(2, globalsize, வெற்று, பொய்);
}
}
#endif

UMat UMat::diag(மாறிலி UMat& d, UMatUsageFlags usageFlags)
{
    CV_INSTRUMENT_REGION();

    CV_Assert(d.cols == 1 || d.rows == 1);
    முழு len = d.rows + d.cols - 1;

#ifdef HAVE_OPENCL
    எனில் (ocl::useOpenCL())
    {
        UMat m(len, len, d.type(), usageFlags);
        எனில் (ocl_setDiag(d, m, len))
        {
            CV_IMPL_ADD(CV_IMPL_OCL);
            திரும்பு m;
        }
    }
#endif

    UMat m(len, len, d.type(), Scalar(0), usageFlags);
    UMat md = m.diag();
    எனில்( d.cols == 1 )
        d.copyTo(md);
    இல்லையெனில்
        transpose(d, md);
    திரும்பு m;
}

முழு UMat::checkVector(முழு _elemChannels, முழு _depth, தருக்க _requireContinuous) மாறிலி
{
    திரும்பு (depth() == _depth || _depth <= 0) &&
        (isContinuous() || !_requireContinuous) &&
        ((dims <= 2 && (((rows == 1 || cols == 1) && channels() == _elemChannels) ||
                        (cols == _elemChannels && channels() == 1))) ||
        (dims == 3 && channels() == 1 && size.p[2] == _elemChannels && (size.p[0] == 1 || size.p[1] == 1) &&
         (isContinuous() || step.p[1] == step.p[2]*size.p[2])))
    ? (முழு)(total()*channels()/_elemChannels) : -1;
}

UMat UMat::reshape(முழு _cn, முழு _newndims, மாறிலி முழு* _newsz) மாறிலி
{
    எனில்(_newndims == dims)
    {
        எனில்(_newsz == 0)
            திரும்பு reshape(_cn);
        எனில்(_newndims == 2)
            திரும்பு reshape(_cn, _newsz[0]);
    }

    எனில் (isContinuous())
    {
        CV_Assert(_cn >= 0 && _newndims >= 0 && _newndims <= CV_MAX_DIM && (_newndims == 0 || _newsz != 0));

        எனில் (_cn == 0)
            _cn = இது->channels();
        இல்லையெனில்
            CV_Assert(_cn <= CV_CN_MAX);

        size_t total_elem1_ref = இது->total() * இது->channels();
        size_t total_elem1 = _cn;

        AutoBuffer<முழு, 4> newsz_buf( (size_t)std::max(_newndims, 1) );

        முழு m1_idx = -1;

        க்கு (முழு i = 0; i < _newndims; i++)
        {
            எனில் (_newsz[i] >= 0) {
                எனில் (_newsz[i] == 0 && i < dims)
                    newsz_buf[i] = size.p[i];
                இல்லையெனில்
                    newsz_buf[i] = _newsz[i];
                total_elem1 *= (size_t)newsz_buf[i];
            } இல்லையெனில் {
                எனில் (m1_idx >= 0)
                    CV_Error(cv::Error::StsBadSize, "More than one '-1' occured in the new shape");
                m1_idx = i;
            }
        }

        எனில் (m1_idx >= 0) {
            எனில் (total_elem1 == 0) {
                CV_Assert(total_elem1_ref == 0);
                total_elem1 = 1;
            }
            CV_Assert(total_elem1_ref % total_elem1 == 0);
            newsz_buf[m1_idx] = (முழு)(total_elem1_ref / total_elem1);
        } இல்லையெனில் எனில் (total_elem1 != total_elem1_ref) {
            CV_Error(cv::Error::StsUnmatchedSizes, "Requested and source matrices have different count of elements");
        }

        UMat hdr = *இது;
        hdr.flags = (hdr.flags & ~CV_MAT_CN_MASK) | ((_cn-1) << CV_CN_SHIFT);
        setSize(hdr, _newndims, newsz_buf.data(), வெற்று, உண்மை);

        திரும்பு hdr;
    }

    CV_Error(cv::Error::StsNotImplemented, "Reshaping of n-dimensional non-continuous matrices is not supported yet");
}

UMat UMat::reshape(முழு _cn, மாறிலி MatShape& _newshape) மாறிலி
{
    எனில் (_newshape.dims < 0) {
        முழு newshape[] = {0};
        திரும்பு reshape(_cn, 1, newshape);
    }
    திரும்பு reshape(_cn, _newshape.dims, _newshape.p);
}

Mat UMat::getMat(AccessFlag accessFlags) மாறிலி
{
    எனில்(!u)
        திரும்பு Mat();
    // TODO Support ACCESS_READ (ACCESS_WRITE) without unnecessary data transfers
    accessFlags |= ACCESS_RW;
    UMatDataAutoLock autolock(u);
    முயல்
    {
        எனில்(CV_XADD(&u->refcount, 1) == 0)
            u->currAllocator->map(u, accessFlags);
        எனில் (u->data != 0)
        {
            Mat hdr(dims, size.p, type(), u->data + offset, step.p);
            hdr.flags = flags;
            hdr.u = u;
            hdr.datastart = u->data;
            hdr.data = u->data + offset;
            hdr.datalimit = hdr.dataend = u->data + u->size;
            திரும்பு hdr;
        }
    }
    பிடி(...)
    {
        CV_XADD(&u->refcount, -1);
        எறி;
    }
    CV_XADD(&u->refcount, -1);
    CV_Assert(u->data != 0 && "Error mapping of UMat to host memory.");
    திரும்பு Mat();
}

வெறுமை* UMat::handle(AccessFlag accessFlags) மாறிலி
{
    எனில்( !u )
        திரும்பு 0;

    CV_Assert(u->refcount == 0);
    CV_Assert(!u->deviceCopyObsolete() || u->copyOnMap());
    எனில் (u->deviceCopyObsolete())
    {
        u->currAllocator->unmap(u);
    }

    எனில் (!!(accessFlags & ACCESS_WRITE))
        u->markHostCopyObsolete(உண்மை);

    திரும்பு u->handle;
}

வெறுமை UMat::ndoffset(size_t* ofs) மாறிலி
{
    // offset = step[0]*ofs[0] + step[1]*ofs[1] + step[2]*ofs[2] + ...;
    size_t val = offset;
    க்கு( முழு i = 0; i < dims; i++ )
    {
        size_t s = step.p[i];
        ofs[i] = val / s;
        val -= ofs[i]*s;
    }
}

வெறுமை UMat::copyTo(OutputArray _dst) மாறிலி
{
    CV_INSTRUMENT_REGION();

#ifdef HAVE_CUDA
    எனில் (_dst.isGpuMat())
    {
        _dst.getGpuMat().upload(*இது);
        திரும்பு;
    }
#endif

    முழு stype = type();
    முழு dtype = _dst.type();
    எனில்( _dst.fixedType() && dtype != stype )
    {
        CV_Assert( CV_MAT_CN(stype) == CV_MAT_CN(dtype) );
        convertTo( _dst, dtype );
        திரும்பு;
    }

    எனில்( dims == 0 && empty() )
    {
        _dst.release();
        வெறுமை* obj = _dst.getObj();
        எனில் (_dst.isMat())
            reinterpret_cast<Mat*>(obj)->flags = Mat::MAGIC_VAL | Mat::CONTINUOUS_FLAG | stype;
        இல்லையெனில் எனில் (_dst.isUMat())
            reinterpret_cast<UMat*>(obj)->flags = UMat::MAGIC_VAL | UMat::CONTINUOUS_FLAG | stype;
        இல்லையெனில் எனில் (_dst.isGpuMat())
            reinterpret_cast<cuda::GpuMat*>(obj)->flags = stype;
        திரும்பு;
    }

    _dst.create( dims, size.p, stype );
    எனில் (empty())
        திரும்பு;

    size_t sz[CV_MAX_DIM] = {1}, srcofs[CV_MAX_DIM]={0}, dstofs[CV_MAX_DIM]={0};
    size_t esz = CV_ELEM_SIZE(stype);
    முழு i, d = std::max(dims, 1);
    க்கு( i = 0; i < d; i++ )
        sz[i] = size.p[i];
    sz[d-1] *= esz;
    ndoffset(srcofs);
    srcofs[d-1] *= esz;

    எனில்( _dst.isUMat() )
    {
        UMat dst = _dst.getUMat();
        CV_Assert(dst.u);
        எனில்( u == dst.u && dst.offset == offset )
            திரும்பு;

        எனில் (u->currAllocator == dst.u->currAllocator)
        {
            dst.ndoffset(dstofs);
            dstofs[dims-1] *= esz;
            u->currAllocator->copy(u, dst.u, d, sz, srcofs, step.p, dstofs, dst.step.p, பொய்);
            திரும்பு;
        }

        // different allocators: never map a device destination, upload into it instead
        // (through a host Mat when both sides are device memory)
        MatAllocator* hostAllocator = Mat::getDefaultAllocator();
        எனில் (dst.u->currAllocator != hostAllocator)
        {
            எனில் (u->currAllocator == hostAllocator)
                getMat(ACCESS_READ).copyTo(_dst);
            இல்லையெனில்
            {
                Mat tmp;
                copyTo(tmp);
                tmp.copyTo(_dst);
            }
            திரும்பு;
        }
    }

    Mat dst = _dst.getMat();
    u->currAllocator->download(u, dst.ptr(), d, sz, srcofs, step.p, dst.step.p);
}

வெறுமை UMat::copyTo(OutputArray _dst, உள்ளீடு_Array _mask) மாறிலி
{
    CV_INSTRUMENT_REGION();

    எனில்( _mask.empty() )
    {
        copyTo(_dst);
        திரும்பு;
    }
#ifdef HAVE_OPENCL
    முழு cn = channels(), mtype = _mask.type(), mdepth = CV_MAT_DEPTH(mtype), mcn = CV_MAT_CN(mtype);
    CV_Assert( (mdepth == CV_8U || mdepth == CV_8S || mdepth == CV_Bool) && (mcn == 1 || mcn == cn) );

    எனில் (ocl::useOpenCL() && _dst.isUMat() && dims <= 2)
    {
        UMatData * prevu = _dst.getUMat().u;
        _dst.create( size, type() );

        UMat dst = _dst.getUMat();

        தருக்க haveDstUninit = பொய்;
        எனில்( prevu != dst.u ) // do not leave dst uninitialized
            haveDstUninit = உண்மை;

        String opts = format("-D COPY_TO_MASK -D T1=%s -D scn=%d -D mcn=%d%s",
                             ocl::memopTypeToStr(depth()), cn, mcn,
                             haveDstUninit ? " -D HAVE_DST_UNINIT" : "");

        ocl::Kernel k("copyToMask", ocl::core::copyset_oclsrc, opts);
        எனில் (!k.empty())
        {
            k.args(ocl::KernelArg::ReadOnlyNoSize(*இது),
                   ocl::KernelArg::ReadOnlyNoSize(_mask.getUMat()),
                   haveDstUninit ? ocl::KernelArg::WriteOnly(dst) :
                                   ocl::KernelArg::ReadWrite(dst));

            size_t globalsize[2] = { (size_t)cols, (size_t)rows };
            எனில் (k.run(2, globalsize, வெற்று, பொய்))
            {
                CV_IMPL_ADD(CV_IMPL_OCL);
                திரும்பு;
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

UMat& UMat::setTo(உள்ளீடு_Array மதிப்பு, உள்ளீடு_Array _mask)
{
    CV_INSTRUMENT_REGION();

    தருக்க haveMask = !_mask.empty();
#ifdef HAVE_OPENCL
    முழு tp = type(), cn = CV_MAT_CN(tp), d = CV_MAT_DEPTH(tp);

    எனில்( dims <= 2 && cn <= 4 && CV_MAT_DEPTH(tp) < CV_64F && ocl::useOpenCL() )
    {
        Mat value = மதிப்பு.getMat();
        CV_Assert( checkScalar(value, type(), மதிப்பு.kind(), உள்ளீடு_Array_2::UMAT) );
        முழு kercn = haveMask || cn == 3 ? cn : std::max(cn, ocl::predictOptimalVectorWidth(*இது)),
                kertp = CV_MAKE_TYPE(d, kercn);

        இரட்டை buf[16] = { 0, 0, 0, 0, 0, 0, 0, 0,
                           0, 0, 0, 0, 0, 0, 0, 0 };
        convertAndUnrollScalar(value, tp, (uchar *)buf, kercn / cn);

        முழு scalarcn = kercn == 3 ? 4 : kercn, rowsPerWI = ocl::Device::getDefault().isIntel() ? 4 : 1;
        String opts = format("-D dstT=%s -D rowsPerWI=%d -D dstST=%s -D dstT1=%s -D cn=%d",
                             ocl::memopTypeToStr(kertp), rowsPerWI,
                             ocl::memopTypeToStr(CV_MAKETYPE(d, scalarcn)),
                             ocl::memopTypeToStr(d), kercn);

        ocl::Kernel setK(haveMask ? "setMask" : "set", ocl::core::copyset_oclsrc, opts);
        எனில்( !setK.empty() )
        {
            ocl::KernelArg scalararg(ocl::KernelArg::CONSTANT, 0, 0, 0, buf, CV_ELEM_SIZE(d) * scalarcn);
            UMat mask;

            எனில்( haveMask )
            {
                mask = _mask.getUMat();
                CV_Assert( mask.size() == size() && (mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool) );
                ocl::KernelArg maskarg = ocl::KernelArg::ReadOnlyNoSize(mask),
                        dstarg = ocl::KernelArg::ReadWrite(*இது);
                setK.args(maskarg, dstarg, scalararg);
            }
            இல்லையெனில்
            {
                ocl::KernelArg dstarg = ocl::KernelArg::WriteOnly(*இது, cn, kercn);
                setK.args(dstarg, scalararg);
            }

            size_t globalsize[] = { (size_t)cols * cn / kercn, ((size_t)rows + rowsPerWI - 1) / rowsPerWI };
            எனில்( setK.run(2, globalsize, வெற்று, பொய்) )
            {
                CV_IMPL_ADD(CV_IMPL_OCL);
                திரும்பு *இது;
            }
        }
    }
#endif
    Mat m = getMat(haveMask ? ACCESS_RW : ACCESS_WRITE);
    m.setTo(மதிப்பு, _mask);
    திரும்பு *இது;
}

UMat& UMat::operator = (மாறிலி Scalar& s)
{
    setTo(s);
    திரும்பு *இது;
}

UMat UMat::t() மாறிலி
{
    UMat m;
    transpose(*இது, m);
    திரும்பு m;
}

UMat UMat::zeros(முழு rows, முழு cols, முழு type, UMatUsageFlags usageFlags)
{
    திரும்பு UMat(rows, cols, type, Scalar::all(0), usageFlags);
}

UMat UMat::zeros(Size size, முழு type, UMatUsageFlags usageFlags)
{
    திரும்பு UMat(size, type, Scalar::all(0), usageFlags);
}

UMat UMat::zeros(முழு ndims, மாறிலி முழு* sz, முழு type, UMatUsageFlags usageFlags)
{
    திரும்பு UMat(ndims, sz, type, Scalar::all(0), usageFlags);
}

UMat UMat::zeros(மாறிலி MatShape& shape, முழு type, UMatUsageFlags usageFlags)
{
    திரும்பு UMat(shape.dims, shape.p, type, Scalar::all(0), usageFlags);
}

UMat UMat::ones(முழு rows, முழு cols, முழு type, UMatUsageFlags usageFlags)
{
    திரும்பு UMat(rows, cols, type, Scalar(1), usageFlags);
}

UMat UMat::ones(Size size, முழு type, UMatUsageFlags usageFlags)
{
    திரும்பு UMat(size, type, Scalar(1), usageFlags);
}

UMat UMat::ones(முழு ndims, மாறிலி முழு* sz, முழு type, UMatUsageFlags usageFlags)
{
    திரும்பு UMat(ndims, sz, type, Scalar(1), usageFlags);
}

UMat UMat::ones(மாறிலி MatShape& shape, முழு type, UMatUsageFlags usageFlags)
{
    திரும்பு UMat(shape.dims, shape.p, type, Scalar(1), usageFlags);
}

}

/* End of file. */
