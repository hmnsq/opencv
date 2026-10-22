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
शून्य setSize(UMat& m, पूर्णाङ्क _dims, अचर पूर्णाङ्क* _sz, अचर माप_प्रकार* _steps,
             तार्किक autoSteps = असत्य);

शून्य updateContinuityFlag(UMat& m);
शून्य finalizeHdr(UMat& m);

UMatData::UMatData(अचर MatAllocator* allocator)
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
#ifndef न_संशोधन
        यदि (showWarn)
        {
            स्थिर पूर्णाङ्क warn_message_showed = 0;
            यदि (warn_message_showed++ < 100)
            {
                ख_साफ(मानक_निकास);
                ख_लिखो(मानक_त्रुटि, "\n! OPENCV warning: getUMat()/getMat() call chain possible problem."
                                "\n!                 Base object is dead, while nested/derived object is still alive or processed."
                                "\n!                 Please check lifetime of UMat/Mat objects!\n");
                ख_साफ(मानक_त्रुटि);
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

स्थिर माप_प्रकार getUMatDataLockIndex(अचर UMatData* u)
{
    माप_प्रकार idx = ((माप_प्रकार)(शून्य*)u) % UMAT_NLOCKS;
    फर्काऊ idx;
}

शून्य UMatData::lock()
{
    माप_प्रकार idx = getUMatDataLockIndex(एतत्);
    //printf("%d lock(%d)\n", cv::utils::getThreadID(), (int)idx);
    umatLocks[idx].lock();
}

शून्य UMatData::unlock()
{
    माप_प्रकार idx = getUMatDataLockIndex(एतत्);
    //printf("%d unlock(%d)\n", cv::utils::getThreadID(), (int)idx);
    umatLocks[idx].unlock();
}


// Do not allow several lock() calls with different UMatData objects.
संरचना UMatDataAutoLocker
{
    पूर्णाङ्क usage_गन्ती;
    UMatData* locked_objects[2];
    UMatDataAutoLocker() : usage_गन्ती(0) { locked_objects[0] = रिक्त; locked_objects[1] = रिक्त; }

    शून्य lock(UMatData*& u1)
    {
        तार्किक locked_1 = (u1 == locked_objects[0] || u1 == locked_objects[1]);
        यदि (locked_1)
        {
            u1 = रिक्त;
            फर्काऊ;
        }
        CV_Assert(usage_गन्ती == 0);  // UMatDataAutoLock can't be used multiple times from the same thread
        usage_गन्ती = 1;
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
            फर्काऊ;
        CV_Assert(usage_गन्ती == 0);  // UMatDataAutoLock can't be used multiple times from the same thread
        usage_गन्ती = 1;
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
            फर्काऊ;
        CV_Assert(usage_गन्ती == 1);
        usage_गन्ती = 0;
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
स्थिर UMatDataAutoLocker& getUMatDataAutoLocker() { फर्काऊ getUMatDataAutoLockerTLS().getRef(); }


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

UMat::UMat(पूर्णाङ्क _rows, पूर्णाङ्क _cols, पूर्णाङ्क _type, अचर Scalar& _s, UMatUsageFlags _usageFlags)
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

UMat::UMat(Size _sz, पूर्णाङ्क _type, अचर Scalar& _s, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_sz.height, _sz.width, _type);
    *एतत् = _s;
}

UMat::UMat(पूर्णाङ्क _dims, अचर पूर्णाङ्क* _sz, पूर्णाङ्क _type, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_dims, _sz, _type);
}

UMat::UMat(पूर्णाङ्क _dims, अचर पूर्णाङ्क* _sz, पूर्णाङ्क _type, अचर Scalar& _s, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_dims, _sz, _type);
    *एतत् = _s;
}

UMat::UMat(अचर MatShape& _shape, पूर्णाङ्क _type, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_shape.dims, _shape.p, _type);
}

UMat::UMat(अचर MatShape& _shape, पूर्णाङ्क _type, अचर Scalar& _s, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_shape.dims, _shape.p, _type);
    *एतत् = _s;
}

UMat::UMat(अचर UMat& m)
: flags(m.flags), dims(m.dims), rows(m.rows), cols(m.cols), allocator(m.allocator),
  usageFlags(m.usageFlags), u(m.u), offset(m.offset), size(m.size), step(m.step)
{
    addref();
}

UMat& UMat::चालक=(अचर UMat& m)
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
    फर्काऊ *एतत्;
}

UMat UMat::clone() अचर
{
    UMat m;
    copyTo(m);
    फर्काऊ m;
}

शून्य UMat::assignTo(UMat& m, पूर्णाङ्क _type) अचर
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

शून्य UMat::createSameSize(आगत_Array arr, पूर्णाङ्क _type, UMatUsageFlags _usageFlags)
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

तार्किक UMat::empty() अचर
{
    फर्काऊ u == 0 || total() == 0;
}

माप_प्रकार UMat::total() अचर
{
    यदि( dims <= 2 )
        फर्काऊ (माप_प्रकार)rows * cols;
    माप_प्रकार p = 1;
    लागि( पूर्णाङ्क i = 0; i < dims; i++ )
        p *= size[i];
    फर्काऊ p;
}

MatShape UMat::shape() अचर
{
    फर्काऊ size;
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

UMat& UMat::चालक=(UMat&& m)
{
    यदि (एतत् == &m)
      फर्काऊ *एतत्;
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
    फर्काऊ *एतत्;
}


MatAllocator* UMat::getStdAllocator()
{
#ifdef HAVE_OPENCL
    यदि (ocl::useOpenCL())
        फर्काऊ ocl::getOpenCLAllocator();
#endif
    फर्काऊ Mat::getDefaultAllocator();
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


शून्य setSize( UMat& m, पूर्णाङ्क _dims, अचर पूर्णाङ्क* _sz,
              अचर माप_प्रकार* _steps, तार्किक autoSteps )
{
    CV_Assert( 0 <= _dims && _dims <= CV_MAX_DIM && _dims <= MatShape::MAX_DIMS);

    m.dims = _dims;
    m.size = MatShape(_dims, _sz);
    m.step[std::max(_dims-1, 0)] = CV_ELEM_SIZE(m.flags);
    लागि (पूर्णाङ्क i = _dims-2; i >= 0; i--) {
        माप_प्रकार autostep = m.size[i+1]*m.step[i+1];
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


UMat Mat::getUMat(AccessFlag accessFlags, UMatUsageFlags usageFlags) अचर
{
    UMat hdr;
    यदि(!data)
        फर्काऊ hdr;
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
            फर्काऊ src.getUMat(accessFlags, usageFlags)(cv::Rect(ofs.x, ofs.y, sz.width, sz.height));
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
    गृहाण (अचर cv::Exception& e)
    {
        ख_लिखो(मानक_त्रुटि, "Exception: %s\n", e.what());
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
        फर्काऊ hdr;
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

शून्य UMat::create(पूर्णाङ्क d0, अचर पूर्णाङ्क* _sizes, पूर्णाङ्क _type, UMatUsageFlags _usageFlags)
{
    पूर्णाङ्क sz1 = 1, d = d0;
    पूर्णाङ्क i;
    यदि (d == 0) {
        d = 1;
        _sizes = (अचर पूर्णाङ्क*)&sz1;
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
        लागि( i = 0; i < d; i++ )
            यदि( size[i] != _sizes[i] )
                रोक;
        यदि( i == d )
            फर्काऊ;
    }

    पूर्णाङ्क _sizes_backup[CV_MAX_DIM]; // #5991
    यदि (_sizes == (एतत्->size.p))
    {
        लागि(i = 0; i < d; i++ )
            _sizes_backup[i] = _sizes[i];
        _sizes = _sizes_backup;
    }

    release();
    usageFlags = _usageFlags;
    यदि( d == 0 )
        फर्काऊ;
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
        CV_Assert( step[dims-1] == (माप_प्रकार)CV_ELEM_SIZE(flags) );
    }

    finalizeHdr(*एतत्);
    addref();
    size.dims = dims = d0;
}

शून्य UMat::create(अचर std::vector<पूर्णाङ्क>& _sizes, पूर्णाङ्क _type, UMatUsageFlags _usageFlags)
{
    create((पूर्णाङ्क)_sizes.size(), _sizes.data(), _type, _usageFlags);
}

शून्य UMat::create(अचर MatShape& _shape, पूर्णाङ्क _type, UMatUsageFlags _usageFlags)
{
    यदि (_shape.dims < 0) {
        release();
    } अन्यथा {
        create(_shape.dims, _shape.p, _type, _usageFlags);
        size.layout = _shape.layout;
        size.C = _shape.C;
    }
}

शून्य UMat::fit(पूर्णाङ्क _dims, अचर पूर्णाङ्क* _sizes, पूर्णाङ्क _type, UMatUsageFlags _usageFlags)
{
    यदि (_usageFlags == cv::USAGE_DEFAULT)
        _usageFlags = usageFlags;
    माप_प्रकार oldTotalBytes = u ? u->size : 0;
    माप_प्रकार esz = CV_ELEM_SIZE(_type), newTotal = _dims >= 0;
    लागि (पूर्णाङ्क i = 0; i < _dims; i++)
        newTotal *= _sizes[i];
    माप_प्रकार newTotalBytes = newTotal*esz;
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

शून्य UMat::fit(अचर std::vector<पूर्णाङ्क>& _shape, पूर्णाङ्क _type, UMatUsageFlags _usageFlags)
{
    fit((पूर्णाङ्क)_shape.size(), _shape.data(), _type, _usageFlags);
}

शून्य UMat::fit(अचर MatShape& _shape, पूर्णाङ्क _type, UMatUsageFlags _usageFlags)
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

शून्य UMat::fitSameSize(आगत_Array m, पूर्णाङ्क _type, UMatUsageFlags _usageFlags)
{
    fit(m.shape(), _type, _usageFlags);
}

शून्य UMat::copySize(अचर UMat& m)
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


UMat::UMat(अचर UMat& m, अचर Range& _rowRange, अचर Range& _colRange)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(USAGE_DEFAULT), u(0), offset(0)
{
    CV_Assert( m.dims >= 2 && m.dims <= MatShape::MAX_DIMS );
    यदि( m.dims > 2 )
    {
        Range rs[MatShape::MAX_DIMS];
        rs[0] = _rowRange;
        rs[1] = _colRange;
        लागि( पूर्णाङ्क i = 2; i < m.dims; i++ )
            rs[i] = Range::all();
        *एतत् = m(rs);
        फर्काऊ;
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


UMat::UMat(अचर UMat& m, अचर Rect& roi)
    : flags(m.flags), dims(2), rows(roi.height), cols(roi.width),
    allocator(m.allocator), usageFlags(m.usageFlags), u(m.u),
    offset(m.offset + roi.y*m.step[0]), size(2)
{
    CV_Assert( m.dims <= 2 );

    माप_प्रकार esz = CV_ELEM_SIZE(flags);
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


UMat::UMat(अचर UMat& m, अचर Range* ranges)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(USAGE_DEFAULT), u(0), offset(0)
{
    पूर्णाङ्क i, d = m.dims;

    CV_Assert(ranges);
    लागि( i = 0; i < d; i++ )
    {
        Range r = ranges[i];
        CV_Assert( r == Range::all() || (0 <= r.start && r.start < r.end && r.end <= m.size[i]) );
    }
    *एतत् = m;
    लागि( i = 0; i < d; i++ )
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

UMat::UMat(अचर UMat& m, अचर std::vector<Range>& ranges)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(USAGE_DEFAULT), u(0), offset(0)
{
    पूर्णाङ्क i, d = m.dims;

    CV_Assert((पूर्णाङ्क)ranges.size() == d);
    लागि (i = 0; i < d; i++)
    {
        Range r = ranges[i];
        CV_Assert(r == Range::all() || (0 <= r.start && r.start < r.end && r.end <= m.size[i]));
    }
    *एतत् = m;
    लागि (i = 0; i < d; i++)
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

UMat UMat::diag(पूर्णाङ्क d) अचर
{
    CV_Assert( dims <= 2 );
    UMat m = *एतत्;
    माप_प्रकार esz = elemSize();
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

    फर्काऊ m;
}

शून्य UMat::locateROI( Size& wholeSize, Point& ofs ) अचर
{
    CV_Assert( dims <= 2 && step[0] > 0 );
    माप_प्रकार esz = elemSize(), minstep;
    सूचक_भेद_प्रकार delta1 = (सूचक_भेद_प्रकार)offset, delta2 = (सूचक_भेद_प्रकार)u->size;

    यदि( delta1 == 0 )
        ofs.x = ofs.y = 0;
    अन्यथा
    {
        ofs.y = (पूर्णाङ्क)(delta1/step[0]);
        ofs.x = (पूर्णाङ्क)((delta1 - step[0]*ofs.y)/esz);
        CV_DbgAssert( offset == (माप_प्रकार)(ofs.y*step[0] + ofs.x*esz) );
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
    माप_प्रकार esz = elemSize();
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
    फर्काऊ *एतत्;
}


UMat UMat::reshape(पूर्णाङ्क new_cn, पूर्णाङ्क new_rows) अचर
{
    पूर्णाङ्क cn = channels();
    UMat hdr = *एतत्;

    यदि( dims > 2 && new_rows == 0 && new_cn != 0 && size[dims-1]*cn % new_cn == 0 )
    {
        hdr.flags = (hdr.flags & ~CV_MAT_CN_MASK) | ((new_cn-1) << CV_CN_SHIFT);
        hdr.step[dims-1] = CV_ELEM_SIZE(hdr.flags);
        hdr.size[dims-1] = hdr.size[dims-1]*cn / new_cn;
        फर्काऊ hdr;
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
        CV_Error( cv::Error::Bad_अङ्क_Channels,
        "The total width is not divisible by the new number of channels" );

    hdr.size.dims = hdr.dims = 2;
    hdr.size[1] = hdr.cols = new_width;
    hdr.flags = (hdr.flags & ~CV_MAT_CN_MASK) | ((new_cn-1) << CV_CN_SHIFT);
    hdr.step[1] = CV_ELEM_SIZE(hdr.flags);
    फर्काऊ hdr;
}

#ifdef HAVE_OPENCL
नामाकाश {
स्थिर तार्किक ocl_setDiag(अचर UMat& d, UMat& m, पूर्णाङ्क len)
{
    पूर्णाङ्क cn = d.channels();
    पूर्णाङ्क depth = d.depth();

    यदि (depth == CV_64F && !ocl::Device::getDefault().doubleFPConfig())
        फर्काऊ असत्य;

    String opts = format("-D SET_DIAG -D T1=%s -D cn=%d -D IS_ROW_VECTOR=%d",
                         ocl::memopTypeToStr(depth),
                         cn,
                         (d.rows == 1) ? 1 : 0);

    ocl::Kernel k("setDiag", ocl::core::copyset_oclsrc, opts);
    यदि (k.empty())
        फर्काऊ असत्य;

    k.args(ocl::KernelArg::WriteOnly(m),
           ocl::KernelArg::ReadOnlyNoSize(d),
           len);

    माप_प्रकार globalsize[2] = { (माप_प्रकार)len, (माप_प्रकार)len };
    फर्काऊ k.run(2, globalsize, रिक्त, असत्य);
}
}
#endif

UMat UMat::diag(अचर UMat& d, UMatUsageFlags usageFlags)
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
            फर्काऊ m;
        }
    }
#endif

    UMat m(len, len, d.type(), Scalar(0), usageFlags);
    UMat md = m.diag();
    यदि( d.cols == 1 )
        d.copyTo(md);
    अन्यथा
        transpose(d, md);
    फर्काऊ m;
}

पूर्णाङ्क UMat::checkVector(पूर्णाङ्क _elemChannels, पूर्णाङ्क _depth, तार्किक _requireContinuous) अचर
{
    फर्काऊ (depth() == _depth || _depth <= 0) &&
        (isContinuous() || !_requireContinuous) &&
        ((dims <= 2 && (((rows == 1 || cols == 1) && channels() == _elemChannels) ||
                        (cols == _elemChannels && channels() == 1))) ||
        (dims == 3 && channels() == 1 && size.p[2] == _elemChannels && (size.p[0] == 1 || size.p[1] == 1) &&
         (isContinuous() || step.p[1] == step.p[2]*size.p[2])))
    ? (पूर्णाङ्क)(total()*channels()/_elemChannels) : -1;
}

UMat UMat::reshape(पूर्णाङ्क _cn, पूर्णाङ्क _newndims, अचर पूर्णाङ्क* _newsz) अचर
{
    यदि(_newndims == dims)
    {
        यदि(_newsz == 0)
            फर्काऊ reshape(_cn);
        यदि(_newndims == 2)
            फर्काऊ reshape(_cn, _newsz[0]);
    }

    यदि (isContinuous())
    {
        CV_Assert(_cn >= 0 && _newndims >= 0 && _newndims <= CV_MAX_DIM && (_newndims == 0 || _newsz != 0));

        यदि (_cn == 0)
            _cn = एतत्->channels();
        अन्यथा
            CV_Assert(_cn <= CV_CN_MAX);

        माप_प्रकार total_elem1_ref = एतत्->total() * एतत्->channels();
        माप_प्रकार total_elem1 = _cn;

        AutoBuffer<पूर्णाङ्क, 4> newsz_buf( (माप_प्रकार)std::max(_newndims, 1) );

        पूर्णाङ्क m1_idx = -1;

        लागि (पूर्णाङ्क i = 0; i < _newndims; i++)
        {
            यदि (_newsz[i] >= 0) {
                यदि (_newsz[i] == 0 && i < dims)
                    newsz_buf[i] = size.p[i];
                अन्यथा
                    newsz_buf[i] = _newsz[i];
                total_elem1 *= (माप_प्रकार)newsz_buf[i];
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

        फर्काऊ hdr;
    }

    CV_Error(cv::Error::StsNotImplemented, "Reshaping of n-dimensional non-continuous matrices is not supported yet");
}

UMat UMat::reshape(पूर्णाङ्क _cn, अचर MatShape& _newshape) अचर
{
    यदि (_newshape.dims < 0) {
        पूर्णाङ्क newshape[] = {0};
        फर्काऊ reshape(_cn, 1, newshape);
    }
    फर्काऊ reshape(_cn, _newshape.dims, _newshape.p);
}

Mat UMat::getMat(AccessFlag accessFlags) अचर
{
    यदि(!u)
        फर्काऊ Mat();
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
            फर्काऊ hdr;
        }
    }
    गृहाण(...)
    {
        CV_XADD(&u->refcount, -1);
        क्षिप;
    }
    CV_XADD(&u->refcount, -1);
    CV_Assert(u->data != 0 && "Error mapping of UMat to host memory.");
    फर्काऊ Mat();
}

शून्य* UMat::handle(AccessFlag accessFlags) अचर
{
    यदि( !u )
        फर्काऊ 0;

    CV_Assert(u->refcount == 0);
    CV_Assert(!u->deviceCopyObsolete() || u->copyOnMap());
    यदि (u->deviceCopyObsolete())
    {
        u->currAllocator->unmap(u);
    }

    यदि (!!(accessFlags & ACCESS_WRITE))
        u->markHostCopyObsolete(सत्य);

    फर्काऊ u->handle;
}

शून्य UMat::ndoffset(माप_प्रकार* ofs) अचर
{
    // offset = step[0]*ofs[0] + step[1]*ofs[1] + step[2]*ofs[2] + ...;
    माप_प्रकार val = offset;
    लागि( पूर्णाङ्क i = 0; i < dims; i++ )
    {
        माप_प्रकार s = step.p[i];
        ofs[i] = val / s;
        val -= ofs[i]*s;
    }
}

शून्य UMat::copyTo(OutputArray _dst) अचर
{
    CV_INSTRUMENT_REGION();

#ifdef HAVE_CUDA
    यदि (_dst.isGpuMat())
    {
        _dst.getGpuMat().upload(*एतत्);
        फर्काऊ;
    }
#endif

    पूर्णाङ्क stype = type();
    पूर्णाङ्क dtype = _dst.type();
    यदि( _dst.fixedType() && dtype != stype )
    {
        CV_Assert( CV_MAT_CN(stype) == CV_MAT_CN(dtype) );
        convertTo( _dst, dtype );
        फर्काऊ;
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
        फर्काऊ;
    }

    _dst.create( dims, size.p, stype );
    यदि (empty())
        फर्काऊ;

    माप_प्रकार sz[CV_MAX_DIM] = {1}, srcofs[CV_MAX_DIM]={0}, dstofs[CV_MAX_DIM]={0};
    माप_प्रकार esz = CV_ELEM_SIZE(stype);
    पूर्णाङ्क i, d = std::max(dims, 1);
    लागि( i = 0; i < d; i++ )
        sz[i] = size.p[i];
    sz[d-1] *= esz;
    ndoffset(srcofs);
    srcofs[d-1] *= esz;

    यदि( _dst.isUMat() )
    {
        UMat dst = _dst.getUMat();
        CV_Assert(dst.u);
        यदि( u == dst.u && dst.offset == offset )
            फर्काऊ;

        यदि (u->currAllocator == dst.u->currAllocator)
        {
            dst.ndoffset(dstofs);
            dstofs[dims-1] *= esz;
            u->currAllocator->copy(u, dst.u, d, sz, srcofs, step.p, dstofs, dst.step.p, असत्य);
            फर्काऊ;
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
            फर्काऊ;
        }
    }

    Mat dst = _dst.getMat();
    u->currAllocator->download(u, dst.ptr(), d, sz, srcofs, step.p, dst.step.p);
}

शून्य UMat::copyTo(OutputArray _dst, आगत_Array _mask) अचर
{
    CV_INSTRUMENT_REGION();

    यदि( _mask.empty() )
    {
        copyTo(_dst);
        फर्काऊ;
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

            माप_प्रकार globalsize[2] = { (माप_प्रकार)cols, (माप_प्रकार)rows };
            यदि (k.run(2, globalsize, रिक्त, असत्य))
            {
                CV_IMPL_ADD(CV_IMPL_OCL);
                फर्काऊ;
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

UMat& UMat::setTo(आगत_Array मान, आगत_Array _mask)
{
    CV_INSTRUMENT_REGION();

    तार्किक haveMask = !_mask.empty();
#ifdef HAVE_OPENCL
    पूर्णाङ्क tp = type(), cn = CV_MAT_CN(tp), d = CV_MAT_DEPTH(tp);

    यदि( dims <= 2 && cn <= 4 && CV_MAT_DEPTH(tp) < CV_64F && ocl::useOpenCL() )
    {
        Mat value = मान.getMat();
        CV_Assert( checkScalar(value, type(), मान.kind(), आगत_Array_2::UMAT) );
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

            माप_प्रकार globalsize[] = { (माप_प्रकार)cols * cn / kercn, ((माप_प्रकार)rows + rowsPerWI - 1) / rowsPerWI };
            यदि( setK.run(2, globalsize, रिक्त, असत्य) )
            {
                CV_IMPL_ADD(CV_IMPL_OCL);
                फर्काऊ *एतत्;
            }
        }
    }
#endif
    Mat m = getMat(haveMask ? ACCESS_RW : ACCESS_WRITE);
    m.setTo(मान, _mask);
    फर्काऊ *एतत्;
}

UMat& UMat::चालक = (अचर Scalar& s)
{
    setTo(s);
    फर्काऊ *एतत्;
}

UMat UMat::t() अचर
{
    UMat m;
    transpose(*एतत्, m);
    फर्काऊ m;
}

UMat UMat::zeros(पूर्णाङ्क rows, पूर्णाङ्क cols, पूर्णाङ्क type, UMatUsageFlags usageFlags)
{
    फर्काऊ UMat(rows, cols, type, Scalar::all(0), usageFlags);
}

UMat UMat::zeros(Size size, पूर्णाङ्क type, UMatUsageFlags usageFlags)
{
    फर्काऊ UMat(size, type, Scalar::all(0), usageFlags);
}

UMat UMat::zeros(पूर्णाङ्क ndims, अचर पूर्णाङ्क* sz, पूर्णाङ्क type, UMatUsageFlags usageFlags)
{
    फर्काऊ UMat(ndims, sz, type, Scalar::all(0), usageFlags);
}

UMat UMat::zeros(अचर MatShape& shape, पूर्णाङ्क type, UMatUsageFlags usageFlags)
{
    फर्काऊ UMat(shape.dims, shape.p, type, Scalar::all(0), usageFlags);
}

UMat UMat::ones(पूर्णाङ्क rows, पूर्णाङ्क cols, पूर्णाङ्क type, UMatUsageFlags usageFlags)
{
    फर्काऊ UMat(rows, cols, type, Scalar(1), usageFlags);
}

UMat UMat::ones(Size size, पूर्णाङ्क type, UMatUsageFlags usageFlags)
{
    फर्काऊ UMat(size, type, Scalar(1), usageFlags);
}

UMat UMat::ones(पूर्णाङ्क ndims, अचर पूर्णाङ्क* sz, पूर्णाङ्क type, UMatUsageFlags usageFlags)
{
    फर्काऊ UMat(ndims, sz, type, Scalar(1), usageFlags);
}

UMat UMat::ones(अचर MatShape& shape, पूर्णाङ्क type, UMatUsageFlags usageFlags)
{
    फर्काऊ UMat(shape.dims, shape.p, type, Scalar(1), usageFlags);
}

}

/* End of file. */
