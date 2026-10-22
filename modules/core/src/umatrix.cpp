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

#अन्तर्भाव "precomp.hpp"
#अन्तर्भाव "opencl_kernels_core.hpp"
#अन्तर्भाव "umatrix.hpp"

#अन्तर्भाव <opencv2/core/utils/tls.hpp>

///////////////////////////////// UMat implementation ///////////////////////////////

नामाकाश cv {

// forward decls, implementation is below in this file
शून्य setSize(UMat& m, पूर्णाङ्क _dims, ध्रुव पूर्णाङ्क* _sz, ध्रुव size_t* _steps,
             तार्किक autoSteps = असत्य);

शून्य updateContinuityFlag(UMat& m);
शून्य finalizeHdr(UMat& m);

UMatData::UMatData(ध्रुव MatAllocator* allocator)
{
    prevAllocator = currAllocator = allocator;
    urefcount = refcount = mapcount = 0;
    data = origdata = 0;
    size = 0;
    flags = static_cast<UMatData::MemoryFlag>(0);
    handle = 0;
    userdata = 0;
    allocatorFlags_ = 0;
    originalUMatData = रिक्त;
}

UMatData::~UMatData()
{
    prevAllocator = currAllocator = 0;
    urefcount = refcount = 0;
    CV_Assert(mapcount == 0);
    data = origdata = 0;
    size = 0;
    तार्किक isAsyncCleanup = !!(flags & UMatData::ASYNC_CLEANUP);
    flags = static_cast<UMatData::MemoryFlag>(0);
    handle = 0;
    userdata = 0;
    allocatorFlags_ = 0;
    यदि (originalUMatData)
    {
        तार्किक showWarn = असत्य;
        UMatData* u = originalUMatData;
        तार्किक zero_Ref = CV_XADD(&(u->refcount), -1) == 1;
        यदि (zero_Ref)
        {
            // simulate Mat::deallocate
            यदि (u->mapcount != 0)
            {
                (u->currAllocator ? u->currAllocator : /* TODO allocator ? allocator :*/ Mat::getDefaultAllocator())->unmap(u);
            }
            अन्यथा
            {
                // we don't do "map", so we can't do "unmap"
            }
        }
        तार्किक zero_URef = CV_XADD(&(u->urefcount), -1) == 1;
        यदि (zero_Ref && !zero_URef)
            showWarn = सत्य;
        यदि (zero_Ref && zero_URef) // oops, we need to free resources
        {
            showWarn = !isAsyncCleanup;
            // simulate UMat::deallocate
            u->currAllocator->deallocate(u);
        }
#ifndef NDEBUG
        यदि (showWarn)
        {
            स्थिर पूर्णाङ्क warn_message_showed = 0;
            यदि (warn_message_showed++ < 100)
            {
                fflush(stdout);
                fprintf(stderr, "\n! OPENCV warning: getUMat()/getMat() call chain possible problem."
                                "\n!                 Base object is dead, while nested/derived object is still alive or processed."
                                "\n!                 Please check lifetime of UMat/Mat objects!\n");
                fflush(stderr);
            }
        }
#अन्यथा
        CV_UNUSED(showWarn);
#endif
        originalUMatData = रिक्त;
    }
}

#ifndef OPENCV_DISABLE_THREAD_SUPPORT

// it should be a prime number for the best hash function
गणना { UMAT_NLOCKS = 31 };
स्थिर Mutex umatLocks[UMAT_NLOCKS];

स्थिर size_t getUMatDataLockIndex(ध्रुव UMatData* u)
{
    size_t idx = ((size_t)(शून्य*)u) % UMAT_NLOCKS;
    प्रत्यागम idx;
}

शून्य UMatData::lock()
{
    size_t idx = getUMatDataLockIndex(एतत्);
    //printf("%d lock(%d)\n", cv::utils::getThreadID(), (int)idx);
    umatLocks[idx].lock();
}

शून्य UMatData::unlock()
{
    size_t idx = getUMatDataLockIndex(एतत्);
    //printf("%d unlock(%d)\n", cv::utils::getThreadID(), (int)idx);
    umatLocks[idx].unlock();
}


// Do not allow several lock() calls with different UMatData objects.
संरचना UMatDataAutoLocker
{
    पूर्णाङ्क usage_मोजणी;
    UMatData* locked_objects[2];
    UMatDataAutoLocker() : usage_मोजणी(0) { locked_objects[0] = रिक्त; locked_objects[1] = रिक्त; }

    शून्य lock(UMatData*& u1)
    {
        तार्किक locked_1 = (u1 == locked_objects[0] || u1 == locked_objects[1]);
        यदि (locked_1)
        {
            u1 = रिक्त;
            प्रत्यागम;
        }
        CV_Assert(usage_मोजणी == 0);  // UMatDataAutoLock can't be used multiple times from the same thread
        usage_मोजणी = 1;
        locked_objects[0] = u1;
        u1->lock();
    }
    शून्य lock(UMatData*& u1, UMatData*& u2)
    {
        तार्किक locked_1 = (u1 == locked_objects[0] || u1 == locked_objects[1]);
        तार्किक locked_2 = (u2 == locked_objects[0] || u2 == locked_objects[1]);
        यदि (locked_1)
            u1 = रिक्त;
        यदि (locked_2)
            u2 = रिक्त;
        यदि (locked_1 && locked_2)
            प्रत्यागम;
        CV_Assert(usage_मोजणी == 0);  // UMatDataAutoLock can't be used multiple times from the same thread
        usage_मोजणी = 1;
        locked_objects[0] = u1;
        locked_objects[1] = u2;
        यदि (u1)
            u1->lock();
        यदि (u2)
            u2->lock();
    }
    शून्य release(UMatData* u1, UMatData* u2)
    {
        यदि (u1 == रिक्त && u2 == रिक्त)
            प्रत्यागम;
        CV_Assert(usage_मोजणी == 1);
        usage_मोजणी = 0;
        यदि (u1)
            u1->unlock();
        यदि (u2)
            u2->unlock();
        locked_objects[0] = रिक्त; locked_objects[1] = रिक्त;
    }
};
स्थिर TLSData<UMatDataAutoLocker>& getUMatDataAutoLockerTLS()
{
    CV_SINGLETON_LAZY_INIT_REF(TLSData<UMatDataAutoLocker>, नूतन TLSData<UMatDataAutoLocker>());
}
स्थिर UMatDataAutoLocker& getUMatDataAutoLocker() { प्रत्यागम getUMatDataAutoLockerTLS().getRef(); }


UMatDataAutoLock::UMatDataAutoLock(UMatData* u) : u1(u), u2(रिक्त)
{
    getUMatDataAutoLocker().lock(u1);
}
UMatDataAutoLock::UMatDataAutoLock(UMatData* u1_, UMatData* u2_) : u1(u1_), u2(u2_)
{
    यदि (getUMatDataLockIndex(u1) > getUMatDataLockIndex(u2))
    {
        std::swap(u1, u2);
    }
    getUMatDataAutoLocker().lock(u1, u2);
}
UMatDataAutoLock::~UMatDataAutoLock()
{
    getUMatDataAutoLocker().release(u1, u2);
}

#अन्यथा

शून्य UMatData::lock()
{
    // nothing in OPENCV_DISABLE_THREAD_SUPPORT mode
}

शून्य UMatData::unlock()
{
    // nothing in OPENCV_DISABLE_THREAD_SUPPORT mode
}

UMatDataAutoLock::UMatDataAutoLock(UMatData* u) : u1(u), u2(रिक्त)
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

UMat::UMat(पूर्णाङ्क _rows, पूर्णाङ्क _cols, पूर्णाङ्क _type, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_rows, _cols, _type);
}

UMat::UMat(पूर्णाङ्क _rows, पूर्णाङ्क _cols, पूर्णाङ्क _type, ध्रुव Scalar& _s, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_rows, _cols, _type);
    *एतत् = _s;
}

UMat::UMat(Size _sz, पूर्णाङ्क _type, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create( _sz.height, _sz.width, _type );
}

UMat::UMat(Size _sz, पूर्णाङ्क _type, ध्रुव Scalar& _s, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_sz.height, _sz.width, _type);
    *एतत् = _s;
}

UMat::UMat(पूर्णाङ्क _dims, ध्रुव पूर्णाङ्क* _sz, पूर्णाङ्क _type, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_dims, _sz, _type);
}

UMat::UMat(पूर्णाङ्क _dims, ध्रुव पूर्णाङ्क* _sz, पूर्णाङ्क _type, ध्रुव Scalar& _s, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_dims, _sz, _type);
    *एतत् = _s;
}

UMat::UMat(ध्रुव MatShape& _shape, पूर्णाङ्क _type, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_shape.dims, _shape.p, _type);
}

UMat::UMat(ध्रुव MatShape& _shape, पूर्णाङ्क _type, ध्रुव Scalar& _s, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_shape.dims, _shape.p, _type);
    *एतत् = _s;
}

UMat::UMat(ध्रुव UMat& m)
: flags(m.flags), dims(m.dims), rows(m.rows), cols(m.cols), allocator(m.allocator),
  usageFlags(m.usageFlags), u(m.u), offset(m.offset), size(m.size), step(m.step)
{
    addref();
}

UMat& UMat::operator=(ध्रुव UMat& m)
{
    यदि( एतत् != &m )
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
    प्रत्यागम *एतत्;
}

UMat UMat::clone() ध्रुव
{
    UMat m;
    copyTo(m);
    प्रत्यागम m;
}

शून्य UMat::assignTo(UMat& m, पूर्णाङ्क _type) ध्रुव
{
    यदि( _type < 0 )
        m = *एतत्;
    अन्यथा
        convertTo(m, _type);
}

शून्य UMat::create(पूर्णाङ्क _rows, पूर्णाङ्क _cols, पूर्णाङ्क _type, UMatUsageFlags _usageFlags)
{
    पूर्णाङ्क sz[] = {_rows, _cols};
    create(2, sz, _type, _usageFlags);
}

शून्य UMat::create(Size _sz, पूर्णाङ्क _type, UMatUsageFlags _usageFlags)
{
    create(_sz.height, _sz.width, _type, _usageFlags);
}

शून्य UMat::createSameSize(आदान_Array arr, पूर्णाङ्क _type, UMatUsageFlags _usageFlags)
{
    create(arr.shape(), _type, _usageFlags);
}

शून्य UMat::addref()
{
    यदि( u )
        CV_XADD(&(u->urefcount), 1);
}

शून्य UMat::release()
{
    यदि( u && CV_XADD(&(u->urefcount), -1) == 1 )
        deallocate();
    u = 0;
    size.clear();
    dims = cols = rows = 0;
}

तार्किक UMat::empty() ध्रुव
{
    प्रत्यागम u == 0 || total() == 0;
}

size_t UMat::total() ध्रुव
{
    यदि( dims <= 2 )
        प्रत्यागम (size_t)rows * cols;
    size_t p = 1;
    प्रति( पूर्णाङ्क i = 0; i < dims; i++ )
        p *= size[i];
    प्रत्यागम p;
}

MatShape UMat::shape() ध्रुव
{
    प्रत्यागम size;
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
    m.allocator = रिक्त;
    m.u = रिक्त;
    m.offset = 0;
}

UMat& UMat::operator=(UMat&& m)
{
    यदि (एतत् == &m)
      प्रत्यागम *एतत्;
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
    m.allocator = रिक्त;
    m.u = रिक्त;
    m.offset = 0;
    प्रत्यागम *एतत्;
}


MatAllocator* UMat::getStdAllocator()
{
#ifdef HAVE_OPENCL
    यदि (ocl::useOpenCL())
        प्रत्यागम ocl::getOpenCLAllocator();
#endif
    प्रत्यागम Mat::getDefaultAllocator();
}

शून्य swap( UMat& a, UMat& b )
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


शून्य setSize( UMat& m, पूर्णाङ्क _dims, ध्रुव पूर्णाङ्क* _sz,
              ध्रुव size_t* _steps, तार्किक autoSteps )
{
    CV_Assert( 0 <= _dims && _dims <= CV_MAX_DIM && _dims <= MatShape::MAX_DIMS);

    m.dims = _dims;
    m.size = MatShape(_dims, _sz);
    m.step[std::max(_dims-1, 0)] = CV_ELEM_SIZE(m.flags);
    प्रति (पूर्णाङ्क i = _dims-2; i >= 0; i--) {
        size_t autostep = m.size[i+1]*m.step[i+1];
        यदि (_steps) {
            m.step[i] = _steps[i];
            //CV_Assert(m.step[i] >= autostep);
        } अन्यथा यदि (autoSteps) {
            m.step[i] = autostep;
        } अन्यथा {
            m.step[i] = 0;
        }
    }

    यदि( _dims <= 2 )
    {
        m.cols = _dims == 0 ? 1 : _sz ? _sz[_dims > 1] : 0;
        m.rows = _dims < 2 ? 1 : _sz ? _sz[0] : 0;
    } अन्यथा {
        m.cols = m.rows = -1;
    }
}


शून्य UMat::updateContinuityFlag()
{
    flags = cv::updateContinuityFlag(flags, dims, size.p, step.p);
}


शून्य finalizeHdr(UMat& m)
{
    m.updateContinuityFlag();
    पूर्णाङ्क d = m.dims;
    यदि( d > 2 )
        m.rows = m.cols = -1;
}


UMat Mat::getUMat(AccessFlag accessFlags, UMatUsageFlags usageFlags) ध्रुव
{
    UMat hdr;
    यदि(!data)
        प्रत्यागम hdr;
    यदि (data != datastart)
    {
        Size wholeSize;
        Point ofs;
        locateROI(wholeSize, ofs);
        Size sz(cols, rows);
        यदि (ofs.x != 0 || ofs.y != 0)
        {
            Mat src = *एतत्;
            पूर्णाङ्क dtop = ofs.y;
            पूर्णाङ्क dbottom = wholeSize.height - src.rows - ofs.y;
            पूर्णाङ्क dleft = ofs.x;
            पूर्णाङ्क dright = wholeSize.width - src.cols - ofs.x;
            src.adjustROI(dtop, dbottom, dleft, dright);
            प्रत्यागम src.getUMat(accessFlags, usageFlags)(cv::Rect(ofs.x, ofs.y, sz.width, sz.height));
        }
    }
    CV_Assert(data == datastart);

    accessFlags |= ACCESS_RW;
    UMatData* new_u = रिक्त;
    MatStep new_step = step;
    {
        MatAllocator *a = allocator, *a0 = getDefaultAllocator();
        यदि(!a)
            a = a0;
        new_u = a->allocate(dims, size.p, type(), data, new_step.p, accessFlags, usageFlags);
        new_u->originalUMatData = u;
    }
    तार्किक allocated = असत्य;
    यत्न
    {
        allocated = UMat::getStdAllocator()->allocate(new_u, accessFlags, usageFlags);
    }
    गृहाण (ध्रुव cv::Exception& e)
    {
        fprintf(stderr, "Exception: %s\n", e.what());
    }
    यदि (!allocated)
    {
        allocated = getDefaultAllocator()->allocate(new_u, accessFlags, usageFlags);
        CV_Assert(allocated);
    }
    यदि (u != रिक्त)
    {
#ifdef HAVE_OPENCL
        यदि (ocl::useOpenCL() && new_u->currAllocator == ocl::getOpenCLAllocator())
        {
            CV_Assert(new_u->tempUMat());
        }
#endif
        CV_XADD(&(u->refcount), 1);
        CV_XADD(&(u->urefcount), 1);
    }
    यत्न
    {
        hdr.flags = flags;
        hdr.usageFlags = usageFlags;
        setSize(hdr, dims, size.p, new_step.p);
        finalizeHdr(hdr);
        hdr.u = new_u;
        hdr.offset = 0; //data - datastart;
        hdr.addref();
        प्रत्यागम hdr;
    }
    गृहाण(...)
    {
        यदि (u != रिक्त)
        {
            CV_XADD(&(u->refcount), -1);
            CV_XADD(&(u->urefcount), -1);
        }
        new_u->currAllocator->deallocate(new_u);
        क्षिप;
    }

}

शून्य UMat::create(पूर्णाङ्क d0, ध्रुव पूर्णाङ्क* _sizes, पूर्णाङ्क _type, UMatUsageFlags _usageFlags)
{
    पूर्णाङ्क sz1 = 1, d = d0;
    पूर्णाङ्क i;
    यदि (d == 0) {
        d = 1;
        _sizes = (ध्रुव पूर्णाङ्क*)&sz1;
    }
    CV_Assert(0 <= d && d <= CV_MAX_DIM && _sizes);
    _type = CV_MAT_TYPE(_type);

    // if param value is USAGE_DEFAULT by implicit default param value -or- explicit value
    // ...then don't change the existing usageFlags
    // it is not possible to change usage from non-default to USAGE_DEFAULT through create()
    // ...instead must construct UMat()
    यदि (_usageFlags == cv::USAGE_DEFAULT)
    {
        _usageFlags = usageFlags;
    }

    यदि( u && d == dims && _type == type() && _usageFlags == usageFlags )
    {
        प्रति( i = 0; i < d; i++ )
            यदि( size[i] != _sizes[i] )
                भङ्ग;
        यदि( i == d )
            प्रत्यागम;
    }

    पूर्णाङ्क _sizes_backup[CV_MAX_DIM]; // #5991
    यदि (_sizes == (एतत्->size.p))
    {
        प्रति(i = 0; i < d; i++ )
            _sizes_backup[i] = _sizes[i];
        _sizes = _sizes_backup;
    }

    release();
    usageFlags = _usageFlags;
    यदि( d == 0 )
        प्रत्यागम;
    flags = (_type & CV_MAT_TYPE_MASK) | MAGIC_VAL;
    setSize(*एतत्, d, _sizes, 0, सत्य);
    offset = 0;

    यदि( total() > 0 )
    {
        MatAllocator *a = allocator, *a0 = getStdAllocator();
        यदि (!a)
        {
            a = a0;
            a0 = Mat::getDefaultAllocator();
        }
        यत्न
        {
            u = a->allocate(dims, size.p, _type, 0, step.p, ACCESS_RW /* ignored */, usageFlags);
            CV_Assert(u != 0);
        }
        गृहाण(...)
        {
            यदि(a != a0)
                u = a0->allocate(dims, size.p, _type, 0, step.p, ACCESS_RW /* ignored */, usageFlags);
            CV_Assert(u != 0);
        }
        CV_Assert( step[dims-1] == (size_t)CV_ELEM_SIZE(flags) );
    }

    finalizeHdr(*एतत्);
    addref();
    size.dims = dims = d0;
}

शून्य UMat::create(ध्रुव std::vector<पूर्णाङ्क>& _sizes, पूर्णाङ्क _type, UMatUsageFlags _usageFlags)
{
    create((पूर्णाङ्क)_sizes.size(), _sizes.data(), _type, _usageFlags);
}

शून्य UMat::create(ध्रुव MatShape& _shape, पूर्णाङ्क _type, UMatUsageFlags _usageFlags)
{
    यदि (_shape.dims < 0) {
        release();
    } अन्यथा {
        create(_shape.dims, _shape.p, _type, _usageFlags);
        size.layout = _shape.layout;
        size.C = _shape.C;
    }
}

शून्य UMat::fit(पूर्णाङ्क _dims, ध्रुव पूर्णाङ्क* _sizes, पूर्णाङ्क _type, UMatUsageFlags _usageFlags)
{
    यदि (_usageFlags == cv::USAGE_DEFAULT)
        _usageFlags = usageFlags;
    size_t oldTotalBytes = u ? u->size : 0;
    size_t esz = CV_ELEM_SIZE(_type), newTotal = _dims >= 0;
    प्रति (पूर्णाङ्क i = 0; i < _dims; i++)
        newTotal *= _sizes[i];
    size_t newTotalBytes = newTotal*esz;
    यदि (newTotalBytes > 0 &&
        (!isContinuous() ||
         newTotalBytes > oldTotalBytes ||
         offset != 0 ||
         _usageFlags != usageFlags)) {
        create(_dims, _sizes, _type, _usageFlags);
    } अन्यथा {
        flags = (flags & ~Mat::TYPE_MASK) | CV_MAT_TYPE(_type);
        पूर्णाङ्क _dummy_size = 0;
        setSize(*एतत्, (_dims >= 0 ? _dims : 1), (_dims >= 0 ? _sizes : &_dummy_size), nullptr, सत्य);
        finalizeHdr(*एतत्);
    }
}

शून्य UMat::fit(ध्रुव std::vector<पूर्णाङ्क>& _shape, पूर्णाङ्क _type, UMatUsageFlags _usageFlags)
{
    fit((पूर्णाङ्क)_shape.size(), _shape.data(), _type, _usageFlags);
}

शून्य UMat::fit(ध्रुव MatShape& _shape, पूर्णाङ्क _type, UMatUsageFlags _usageFlags)
{
    fit(_shape.dims, _shape.p, _type, _usageFlags);
    size.layout = _shape.layout;
    size.C = _shape.C;
}

शून्य UMat::fit(पूर्णाङ्क _rows, पूर्णाङ्क _cols, पूर्णाङ्क _type, UMatUsageFlags _usageFlags)
{
    _type &= TYPE_MASK;
    पूर्णाङ्क sz[] = {_rows, _cols};
    fit(2, sz, _type, _usageFlags);
}

शून्य UMat::fit(Size _sz, पूर्णाङ्क _type, UMatUsageFlags _usageFlags)
{
    fit(_sz.height, _sz.width, _type, _usageFlags);
}

शून्य UMat::fitSameSize(आदान_Array m, पूर्णाङ्क _type, UMatUsageFlags _usageFlags)
{
    fit(m.shape(), _type, _usageFlags);
}

शून्य UMat::copySize(ध्रुव UMat& m)
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

शून्य UMat::deallocate()
{
    UMatData* u_ = u;
    u = रिक्त;
    u_->currAllocator->deallocate(u_);
}


UMat::UMat(ध्रुव UMat& m, ध्रुव Range& _rowRange, ध्रुव Range& _colRange)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(USAGE_DEFAULT), u(0), offset(0)
{
    CV_Assert( m.dims >= 2 && m.dims <= MatShape::MAX_DIMS );
    यदि( m.dims > 2 )
    {
        Range rs[MatShape::MAX_DIMS];
        rs[0] = _rowRange;
        rs[1] = _colRange;
        प्रति( पूर्णाङ्क i = 2; i < m.dims; i++ )
            rs[i] = Range::all();
        *एतत् = m(rs);
        प्रत्यागम;
    }

    *एतत् = m;
    यदि( _rowRange != Range::all() && _rowRange != Range(0,rows) )
    {
        CV_Assert( 0 <= _rowRange.start &&
                  _rowRange.start <= _rowRange.end &&
                  _rowRange.end <= m.rows );
        size[0] = rows = _rowRange.size();
        offset += step*_rowRange.start;
        flags |= SUBMATRIX_FLAG;
    }

    यदि( _colRange != Range::all() && _colRange != Range(0,cols) )
    {
        CV_Assert( 0 <= _colRange.start &&
                  _colRange.start <= _colRange.end &&
                  _colRange.end <= m.cols );
        size[1] = cols = _colRange.size();
        offset += _colRange.start*elemSize();
        flags |= SUBMATRIX_FLAG;
    }

    updateContinuityFlag();

    यदि( rows <= 0 || cols <= 0 )
    {
        release();
        rows = cols = 0;
    }
}


UMat::UMat(ध्रुव UMat& m, ध्रुव Rect& roi)
    : flags(m.flags), dims(2), rows(roi.height), cols(roi.width),
    allocator(m.allocator), usageFlags(m.usageFlags), u(m.u),
    offset(m.offset + roi.y*m.step[0]), size(2)
{
    CV_Assert( m.dims <= 2 );

    size_t esz = CV_ELEM_SIZE(flags);
    offset += roi.x*esz;
    CV_Assert( 0 <= roi.x && 0 <= roi.width && roi.x + roi.width <= m.cols &&
              0 <= roi.y && 0 <= roi.height && roi.y + roi.height <= m.rows );
    यदि( roi.width < m.cols || roi.height < m.rows )
        flags |= SUBMATRIX_FLAG;

    size[0] = rows;
    size[1] = cols;
    step[0] = m.dims == 2 ? m.step[0] : cols*esz;
    step[1] = esz;

    updateContinuityFlag();

    addref();
    यदि( rows <= 0 || cols <= 0 )
    {
        rows = cols = 0;
        release();
    }
}


UMat::UMat(ध्रुव UMat& m, ध्रुव Range* ranges)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(USAGE_DEFAULT), u(0), offset(0)
{
    पूर्णाङ्क i, d = m.dims;

    CV_Assert(ranges);
    प्रति( i = 0; i < d; i++ )
    {
        Range r = ranges[i];
        CV_Assert( r == Range::all() || (0 <= r.start && r.start < r.end && r.end <= m.size[i]) );
    }
    *एतत् = m;
    प्रति( i = 0; i < d; i++ )
    {
        Range r = ranges[i];
        यदि( r != Range::all() && r != Range(0, size.p[i]))
        {
            size.p[i] = r.end - r.start;
            offset += r.start*step.p[i];
            flags |= SUBMATRIX_FLAG;
        }
    }

    यदि (d <= 2) {
        rows = d == 2 ? size[0] : 1;
        cols = d <= 0 ? (d >= 0) : size[d > 1];
    }
    updateContinuityFlag();
}

UMat::UMat(ध्रुव UMat& m, ध्रुव std::vector<Range>& ranges)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(USAGE_DEFAULT), u(0), offset(0)
{
    पूर्णाङ्क i, d = m.dims;

    CV_Assert((पूर्णाङ्क)ranges.size() == d);
    प्रति (i = 0; i < d; i++)
    {
        Range r = ranges[i];
        CV_Assert(r == Range::all() || (0 <= r.start && r.start < r.end && r.end <= m.size[i]));
    }
    *एतत् = m;
    प्रति (i = 0; i < d; i++)
    {
        Range r = ranges[i];
        यदि (r != Range::all() && r != Range(0, size.p[i]))
        {
            size.p[i] = r.end - r.start;
            offset += r.start*step.p[i];
            flags |= SUBMATRIX_FLAG;
        }
    }

    यदि (d <= 2) {
        rows = d == 2 ? size[0] : 1;
        cols = d <= 0 ? (d >= 0) : size[d > 1];
    }
    updateContinuityFlag();
}

UMat UMat::diag(पूर्णाङ्क d) ध्रुव
{
    CV_Assert( dims <= 2 );
    UMat m = *एतत्;
    size_t esz = elemSize();
    पूर्णाङ्क len;

    यदि( d >= 0 )
    {
        len = std::min(cols - d, rows);
        m.offset += esz*d;
    }
    अन्यथा
    {
        len = std::min(rows + d, cols);
        m.offset -= step[0]*d;
    }
    CV_DbgAssert( len > 0 );

    m.size[0] = m.rows = len;
    m.size[1] = m.cols = 1;
    m.step[0] += (len > 1 ? esz : 0);

    m.updateContinuityFlag();

    यदि( size() != Size(1,1) )
        m.flags |= SUBMATRIX_FLAG;

    प्रत्यागम m;
}

शून्य UMat::locateROI( Size& wholeSize, Point& ofs ) ध्रुव
{
    CV_Assert( dims <= 2 && step[0] > 0 );
    size_t esz = elemSize(), minstep;
    ptrdiff_t delta1 = (ptrdiff_t)offset, delta2 = (ptrdiff_t)u->size;

    यदि( delta1 == 0 )
        ofs.x = ofs.y = 0;
    अन्यथा
    {
        ofs.y = (पूर्णाङ्क)(delta1/step[0]);
        ofs.x = (पूर्णाङ्क)((delta1 - step[0]*ofs.y)/esz);
        CV_DbgAssert( offset == (size_t)(ofs.y*step[0] + ofs.x*esz) );
    }
    minstep = (ofs.x + cols)*esz;
    wholeSize.height = (पूर्णाङ्क)((delta2 - minstep)/step[0] + 1);
    wholeSize.height = std::max(wholeSize.height, ofs.y + rows);
    wholeSize.width = (पूर्णाङ्क)((delta2 - step*(wholeSize.height-1))/esz);
    wholeSize.width = std::max(wholeSize.width, ofs.x + cols);
}


UMat& UMat::adjustROI( पूर्णाङ्क dtop, पूर्णाङ्क dbottom, पूर्णाङ्क dleft, पूर्णाङ्क dright )
{
    CV_Assert( dims == 2 && step[0] > 0 );
    Size wholeSize; Point ofs;
    size_t esz = elemSize();
    locateROI( wholeSize, ofs );
    पूर्णाङ्क row1 = std::min(std::max(ofs.y - dtop, 0), wholeSize.height), row2 = std::max(0, std::min(ofs.y + rows + dbottom, wholeSize.height));
    पूर्णाङ्क col1 = std::min(std::max(ofs.x - dleft, 0), wholeSize.width), col2 = std::max(0, std::min(ofs.x + cols + dright, wholeSize.width));
    यदि(row1 > row2)
        std::swap(row1, row2);
    यदि(col1 > col2)
        std::swap(col1, col2);

    offset += (row1 - ofs.y)*step + (col1 - ofs.x)*esz;
    rows = row2 - row1; cols = col2 - col1;
    size.p[0] = rows; size.p[1] = cols;
    updateContinuityFlag();
    प्रत्यागम *एतत्;
}


UMat UMat::reshape(पूर्णाङ्क new_cn, पूर्णाङ्क new_rows) ध्रुव
{
    पूर्णाङ्क cn = channels();
    UMat hdr = *एतत्;

    यदि( dims > 2 && new_rows == 0 && new_cn != 0 && size[dims-1]*cn % new_cn == 0 )
    {
        hdr.flags = (hdr.flags & ~CV_MAT_CN_MASK) | ((new_cn-1) << CV_CN_SHIFT);
        hdr.step[dims-1] = CV_ELEM_SIZE(hdr.flags);
        hdr.size[dims-1] = hdr.size[dims-1]*cn / new_cn;
        प्रत्यागम hdr;
    }

    CV_Assert( dims <= 2 );

    यदि( new_cn == 0 )
        new_cn = cn;

    पूर्णाङ्क total_width = cols * cn;

    यदि( (new_cn > total_width || total_width % new_cn != 0) && new_rows == 0 )
        new_rows = rows * total_width / new_cn;

    यदि( new_rows != 0 && new_rows != rows )
    {
        पूर्णाङ्क total_size = total_width * rows;
        यदि( !isContinuous() )
            CV_Error( cv::Error::BadStep,
            "The matrix is not continuous, thus its number of rows can not be changed" );

        total_width = total_size / new_rows;

        यदि( total_width * new_rows != total_size )
            CV_Error( cv::Error::StsBadArg, "The total number of matrix elements "
                                    "is not divisible by the new number of rows" );

        hdr.size[0] = hdr.rows = new_rows;
        hdr.step[0] = total_width * elemSize1();
    } अन्यथा {
        hdr.size[0] = hdr.rows = rows;
        यदि (dims <= 1)
            hdr.step[0] = cols * CV_ELEM_SIZE(flags);
    }

    पूर्णाङ्क new_width = total_width / new_cn;

    यदि( new_width * new_cn != total_width )
        CV_Error( cv::Error::Bad_अंक_Channels,
        "The total width is not divisible by the new number of channels" );

    hdr.size.dims = hdr.dims = 2;
    hdr.size[1] = hdr.cols = new_width;
    hdr.flags = (hdr.flags & ~CV_MAT_CN_MASK) | ((new_cn-1) << CV_CN_SHIFT);
    hdr.step[1] = CV_ELEM_SIZE(hdr.flags);
    प्रत्यागम hdr;
}

#ifdef HAVE_OPENCL
नामाकाश {
स्थिर तार्किक ocl_setDiag(ध्रुव UMat& d, UMat& m, पूर्णाङ्क len)
{
    पूर्णाङ्क cn = d.channels();
    पूर्णाङ्क depth = d.depth();

    यदि (depth == CV_64F && !ocl::Device::getDefault().doubleFPConfig())
        प्रत्यागम असत्य;

    String opts = format("-D SET_DIAG -D T1=%s -D cn=%d -D IS_ROW_VECTOR=%d",
                         ocl::memopTypeToStr(depth),
                         cn,
                         (d.rows == 1) ? 1 : 0);

    ocl::Kernel k("setDiag", ocl::core::copyset_oclsrc, opts);
    यदि (k.empty())
        प्रत्यागम असत्य;

    k.args(ocl::KernelArg::WriteOnly(m),
           ocl::KernelArg::ReadOnlyNoSize(d),
           len);

    size_t globalsize[2] = { (size_t)len, (size_t)len };
    प्रत्यागम k.run(2, globalsize, रिक्त, असत्य);
}
}
#endif

UMat UMat::diag(ध्रुव UMat& d, UMatUsageFlags usageFlags)
{
    CV_INSTRUMENT_REGION();

    CV_Assert(d.cols == 1 || d.rows == 1);
    पूर्णाङ्क len = d.rows + d.cols - 1;

#ifdef HAVE_OPENCL
    यदि (ocl::useOpenCL())
    {
        UMat m(len, len, d.type(), usageFlags);
        यदि (ocl_setDiag(d, m, len))
        {
            CV_IMPL_ADD(CV_IMPL_OCL);
            प्रत्यागम m;
        }
    }
#endif

    UMat m(len, len, d.type(), Scalar(0), usageFlags);
    UMat md = m.diag();
    यदि( d.cols == 1 )
        d.copyTo(md);
    अन्यथा
        transpose(d, md);
    प्रत्यागम m;
}

पूर्णाङ्क UMat::checkVector(पूर्णाङ्क _elemChannels, पूर्णाङ्क _depth, तार्किक _requireContinuous) ध्रुव
{
    प्रत्यागम (depth() == _depth || _depth <= 0) &&
        (isContinuous() || !_requireContinuous) &&
        ((dims <= 2 && (((rows == 1 || cols == 1) && channels() == _elemChannels) ||
                        (cols == _elemChannels && channels() == 1))) ||
        (dims == 3 && channels() == 1 && size.p[2] == _elemChannels && (size.p[0] == 1 || size.p[1] == 1) &&
         (isContinuous() || step.p[1] == step.p[2]*size.p[2])))
    ? (पूर्णाङ्क)(total()*channels()/_elemChannels) : -1;
}

UMat UMat::reshape(पूर्णाङ्क _cn, पूर्णाङ्क _newndims, ध्रुव पूर्णाङ्क* _newsz) ध्रुव
{
    यदि(_newndims == dims)
    {
        यदि(_newsz == 0)
            प्रत्यागम reshape(_cn);
        यदि(_newndims == 2)
            प्रत्यागम reshape(_cn, _newsz[0]);
    }

    यदि (isContinuous())
    {
        CV_Assert(_cn >= 0 && _newndims >= 0 && _newndims <= CV_MAX_DIM && (_newndims == 0 || _newsz != 0));

        यदि (_cn == 0)
            _cn = एतत्->channels();
        अन्यथा
            CV_Assert(_cn <= CV_CN_MAX);

        size_t total_elem1_ref = एतत्->total() * एतत्->channels();
        size_t total_elem1 = _cn;

        AutoBuffer<पूर्णाङ्क, 4> newsz_buf( (size_t)std::max(_newndims, 1) );

        पूर्णाङ्क m1_idx = -1;

        प्रति (पूर्णाङ्क i = 0; i < _newndims; i++)
        {
            यदि (_newsz[i] >= 0) {
                यदि (_newsz[i] == 0 && i < dims)
                    newsz_buf[i] = size.p[i];
                अन्यथा
                    newsz_buf[i] = _newsz[i];
                total_elem1 *= (size_t)newsz_buf[i];
            } अन्यथा {
                यदि (m1_idx >= 0)
                    CV_Error(cv::Error::StsBadSize, "More than one '-1' occured in the new shape");
                m1_idx = i;
            }
        }

        यदि (m1_idx >= 0) {
            यदि (total_elem1 == 0) {
                CV_Assert(total_elem1_ref == 0);
                total_elem1 = 1;
            }
            CV_Assert(total_elem1_ref % total_elem1 == 0);
            newsz_buf[m1_idx] = (पूर्णाङ्क)(total_elem1_ref / total_elem1);
        } अन्यथा यदि (total_elem1 != total_elem1_ref) {
            CV_Error(cv::Error::StsUnmatchedSizes, "Requested and source matrices have different count of elements");
        }

        UMat hdr = *एतत्;
        hdr.flags = (hdr.flags & ~CV_MAT_CN_MASK) | ((_cn-1) << CV_CN_SHIFT);
        setSize(hdr, _newndims, newsz_buf.data(), रिक्त, सत्य);

        प्रत्यागम hdr;
    }

    CV_Error(cv::Error::StsNotImplemented, "Reshaping of n-dimensional non-continuous matrices is not supported yet");
}

UMat UMat::reshape(पूर्णाङ्क _cn, ध्रुव MatShape& _newshape) ध्रुव
{
    यदि (_newshape.dims < 0) {
        पूर्णाङ्क newshape[] = {0};
        प्रत्यागम reshape(_cn, 1, newshape);
    }
    प्रत्यागम reshape(_cn, _newshape.dims, _newshape.p);
}

Mat UMat::getMat(AccessFlag accessFlags) ध्रुव
{
    यदि(!u)
        प्रत्यागम Mat();
    // TODO Support ACCESS_READ (ACCESS_WRITE) without unnecessary data transfers
    accessFlags |= ACCESS_RW;
    UMatDataAutoLock autolock(u);
    यत्न
    {
        यदि(CV_XADD(&u->refcount, 1) == 0)
            u->currAllocator->map(u, accessFlags);
        यदि (u->data != 0)
        {
            Mat hdr(dims, size.p, type(), u->data + offset, step.p);
            hdr.flags = flags;
            hdr.u = u;
            hdr.datastart = u->data;
            hdr.data = u->data + offset;
            hdr.datalimit = hdr.dataend = u->data + u->size;
            प्रत्यागम hdr;
        }
    }
    गृहाण(...)
    {
        CV_XADD(&u->refcount, -1);
        क्षिप;
    }
    CV_XADD(&u->refcount, -1);
    CV_Assert(u->data != 0 && "Error mapping of UMat to host memory.");
    प्रत्यागम Mat();
}

शून्य* UMat::handle(AccessFlag accessFlags) ध्रुव
{
    यदि( !u )
        प्रत्यागम 0;

    CV_Assert(u->refcount == 0);
    CV_Assert(!u->deviceCopyObsolete() || u->copyOnMap());
    यदि (u->deviceCopyObsolete())
    {
        u->currAllocator->unmap(u);
    }

    यदि (!!(accessFlags & ACCESS_WRITE))
        u->markHostCopyObsolete(सत्य);

    प्रत्यागम u->handle;
}

शून्य UMat::ndoffset(size_t* ofs) ध्रुव
{
    // offset = step[0]*ofs[0] + step[1]*ofs[1] + step[2]*ofs[2] + ...;
    size_t val = offset;
    प्रति( पूर्णाङ्क i = 0; i < dims; i++ )
    {
        size_t s = step.p[i];
        ofs[i] = val / s;
        val -= ofs[i]*s;
    }
}

शून्य UMat::copyTo(OutputArray _dst) ध्रुव
{
    CV_INSTRUMENT_REGION();

#ifdef HAVE_CUDA
    यदि (_dst.isGpuMat())
    {
        _dst.getGpuMat().upload(*एतत्);
        प्रत्यागम;
    }
#endif

    पूर्णाङ्क stype = type();
    पूर्णाङ्क dtype = _dst.type();
    यदि( _dst.fixedType() && dtype != stype )
    {
        CV_Assert( CV_MAT_CN(stype) == CV_MAT_CN(dtype) );
        convertTo( _dst, dtype );
        प्रत्यागम;
    }

    यदि( dims == 0 && empty() )
    {
        _dst.release();
        शून्य* obj = _dst.getObj();
        यदि (_dst.isMat())
            reinterpret_cast<Mat*>(obj)->flags = Mat::MAGIC_VAL | Mat::CONTINUOUS_FLAG | stype;
        अन्यथा यदि (_dst.isUMat())
            reinterpret_cast<UMat*>(obj)->flags = UMat::MAGIC_VAL | UMat::CONTINUOUS_FLAG | stype;
        अन्यथा यदि (_dst.isGpuMat())
            reinterpret_cast<cuda::GpuMat*>(obj)->flags = stype;
        प्रत्यागम;
    }

    _dst.create( dims, size.p, stype );
    यदि (empty())
        प्रत्यागम;

    size_t sz[CV_MAX_DIM] = {1}, srcofs[CV_MAX_DIM]={0}, dstofs[CV_MAX_DIM]={0};
    size_t esz = CV_ELEM_SIZE(stype);
    पूर्णाङ्क i, d = std::max(dims, 1);
    प्रति( i = 0; i < d; i++ )
        sz[i] = size.p[i];
    sz[d-1] *= esz;
    ndoffset(srcofs);
    srcofs[d-1] *= esz;

    यदि( _dst.isUMat() )
    {
        UMat dst = _dst.getUMat();
        CV_Assert(dst.u);
        यदि( u == dst.u && dst.offset == offset )
            प्रत्यागम;

        यदि (u->currAllocator == dst.u->currAllocator)
        {
            dst.ndoffset(dstofs);
            dstofs[dims-1] *= esz;
            u->currAllocator->copy(u, dst.u, d, sz, srcofs, step.p, dstofs, dst.step.p, असत्य);
            प्रत्यागम;
        }

        // different allocators: never map a device destination, upload into it instead
        // (through a host Mat when both sides are device memory)
        MatAllocator* hostAllocator = Mat::getDefaultAllocator();
        यदि (dst.u->currAllocator != hostAllocator)
        {
            यदि (u->currAllocator == hostAllocator)
                getMat(ACCESS_READ).copyTo(_dst);
            अन्यथा
            {
                Mat tmp;
                copyTo(tmp);
                tmp.copyTo(_dst);
            }
            प्रत्यागम;
        }
    }

    Mat dst = _dst.getMat();
    u->currAllocator->download(u, dst.ptr(), d, sz, srcofs, step.p, dst.step.p);
}

शून्य UMat::copyTo(OutputArray _dst, आदान_Array _mask) ध्रुव
{
    CV_INSTRUMENT_REGION();

    यदि( _mask.empty() )
    {
        copyTo(_dst);
        प्रत्यागम;
    }
#ifdef HAVE_OPENCL
    पूर्णाङ्क cn = channels(), mtype = _mask.type(), mdepth = CV_MAT_DEPTH(mtype), mcn = CV_MAT_CN(mtype);
    CV_Assert( (mdepth == CV_8U || mdepth == CV_8S || mdepth == CV_Bool) && (mcn == 1 || mcn == cn) );

    यदि (ocl::useOpenCL() && _dst.isUMat() && dims <= 2)
    {
        UMatData * prevu = _dst.getUMat().u;
        _dst.create( size, type() );

        UMat dst = _dst.getUMat();

        तार्किक haveDstUninit = असत्य;
        यदि( prevu != dst.u ) // do not leave dst uninitialized
            haveDstUninit = सत्य;

        String opts = format("-D COPY_TO_MASK -D T1=%s -D scn=%d -D mcn=%d%s",
                             ocl::memopTypeToStr(depth()), cn, mcn,
                             haveDstUninit ? " -D HAVE_DST_UNINIT" : "");

        ocl::Kernel k("copyToMask", ocl::core::copyset_oclsrc, opts);
        यदि (!k.empty())
        {
            k.args(ocl::KernelArg::ReadOnlyNoSize(*एतत्),
                   ocl::KernelArg::ReadOnlyNoSize(_mask.getUMat()),
                   haveDstUninit ? ocl::KernelArg::WriteOnly(dst) :
                                   ocl::KernelArg::ReadWrite(dst));

            size_t globalsize[2] = { (size_t)cols, (size_t)rows };
            यदि (k.run(2, globalsize, रिक्त, असत्य))
            {
                CV_IMPL_ADD(CV_IMPL_OCL);
                प्रत्यागम;
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

UMat& UMat::setTo(आदान_Array मूल्य, आदान_Array _mask)
{
    CV_INSTRUMENT_REGION();

    तार्किक haveMask = !_mask.empty();
#ifdef HAVE_OPENCL
    पूर्णाङ्क tp = type(), cn = CV_MAT_CN(tp), d = CV_MAT_DEPTH(tp);

    यदि( dims <= 2 && cn <= 4 && CV_MAT_DEPTH(tp) < CV_64F && ocl::useOpenCL() )
    {
        Mat value = मूल्य.getMat();
        CV_Assert( checkScalar(value, type(), मूल्य.kind(), आदान_Array_2::UMAT) );
        पूर्णाङ्क kercn = haveMask || cn == 3 ? cn : std::max(cn, ocl::predictOptimalVectorWidth(*एतत्)),
                kertp = CV_MAKE_TYPE(d, kercn);

        द्विगुण buf[16] = { 0, 0, 0, 0, 0, 0, 0, 0,
                           0, 0, 0, 0, 0, 0, 0, 0 };
        convertAndUnrollScalar(value, tp, (uchar *)buf, kercn / cn);

        पूर्णाङ्क scalarcn = kercn == 3 ? 4 : kercn, rowsPerWI = ocl::Device::getDefault().isIntel() ? 4 : 1;
        String opts = format("-D dstT=%s -D rowsPerWI=%d -D dstST=%s -D dstT1=%s -D cn=%d",
                             ocl::memopTypeToStr(kertp), rowsPerWI,
                             ocl::memopTypeToStr(CV_MAKETYPE(d, scalarcn)),
                             ocl::memopTypeToStr(d), kercn);

        ocl::Kernel setK(haveMask ? "setMask" : "set", ocl::core::copyset_oclsrc, opts);
        यदि( !setK.empty() )
        {
            ocl::KernelArg scalararg(ocl::KernelArg::CONSTANT, 0, 0, 0, buf, CV_ELEM_SIZE(d) * scalarcn);
            UMat mask;

            यदि( haveMask )
            {
                mask = _mask.getUMat();
                CV_Assert( mask.size() == size() && (mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool) );
                ocl::KernelArg maskarg = ocl::KernelArg::ReadOnlyNoSize(mask),
                        dstarg = ocl::KernelArg::ReadWrite(*एतत्);
                setK.args(maskarg, dstarg, scalararg);
            }
            अन्यथा
            {
                ocl::KernelArg dstarg = ocl::KernelArg::WriteOnly(*एतत्, cn, kercn);
                setK.args(dstarg, scalararg);
            }

            size_t globalsize[] = { (size_t)cols * cn / kercn, ((size_t)rows + rowsPerWI - 1) / rowsPerWI };
            यदि( setK.run(2, globalsize, रिक्त, असत्य) )
            {
                CV_IMPL_ADD(CV_IMPL_OCL);
                प्रत्यागम *एतत्;
            }
        }
    }
#endif
    Mat m = getMat(haveMask ? ACCESS_RW : ACCESS_WRITE);
    m.setTo(मूल्य, _mask);
    प्रत्यागम *एतत्;
}

UMat& UMat::operator = (ध्रुव Scalar& s)
{
    setTo(s);
    प्रत्यागम *एतत्;
}

UMat UMat::t() ध्रुव
{
    UMat m;
    transpose(*एतत्, m);
    प्रत्यागम m;
}

UMat UMat::zeros(पूर्णाङ्क rows, पूर्णाङ्क cols, पूर्णाङ्क type, UMatUsageFlags usageFlags)
{
    प्रत्यागम UMat(rows, cols, type, Scalar::all(0), usageFlags);
}

UMat UMat::zeros(Size size, पूर्णाङ्क type, UMatUsageFlags usageFlags)
{
    प्रत्यागम UMat(size, type, Scalar::all(0), usageFlags);
}

UMat UMat::zeros(पूर्णाङ्क ndims, ध्रुव पूर्णाङ्क* sz, पूर्णाङ्क type, UMatUsageFlags usageFlags)
{
    प्रत्यागम UMat(ndims, sz, type, Scalar::all(0), usageFlags);
}

UMat UMat::zeros(ध्रुव MatShape& shape, पूर्णाङ्क type, UMatUsageFlags usageFlags)
{
    प्रत्यागम UMat(shape.dims, shape.p, type, Scalar::all(0), usageFlags);
}

UMat UMat::ones(पूर्णाङ्क rows, पूर्णाङ्क cols, पूर्णाङ्क type, UMatUsageFlags usageFlags)
{
    प्रत्यागम UMat(rows, cols, type, Scalar(1), usageFlags);
}

UMat UMat::ones(Size size, पूर्णाङ्क type, UMatUsageFlags usageFlags)
{
    प्रत्यागम UMat(size, type, Scalar(1), usageFlags);
}

UMat UMat::ones(पूर्णाङ्क ndims, ध्रुव पूर्णाङ्क* sz, पूर्णाङ्क type, UMatUsageFlags usageFlags)
{
    प्रत्यागम UMat(ndims, sz, type, Scalar(1), usageFlags);
}

UMat UMat::ones(ध्रुव MatShape& shape, पूर्णाङ्क type, UMatUsageFlags usageFlags)
{
    प्रत्यागम UMat(shape.dims, shape.p, type, Scalar(1), usageFlags);
}

}

/* End of file. */
