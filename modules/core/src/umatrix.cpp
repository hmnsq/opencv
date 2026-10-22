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

#অন্তর্ভাৱ "precomp.hpp"
#অন্তর্ভাৱ "opencl_kernels_core.hpp"
#অন্তর্ভাৱ "umatrix.hpp"

#অন্তর্ভাৱ <opencv2/core/utils/tls.hpp>

///////////////////////////////// UMat implementation ///////////////////////////////

নামাকাশ cv {

// forward decls, implementation is below in this file
শূন্য setSize(UMat& m, পূর্ণাঙ্ক _dims, ধ্রুৱ পূর্ণাঙ্ক* _sz, ধ্রুৱ size_t* _steps,
             তার্কিক autoSteps = মিছা);

শূন্য updateContinuityFlag(UMat& m);
শূন্য finalizeHdr(UMat& m);

UMatData::UMatData(ধ্রুৱ MatAllocator* allocator)
{
    prevAllocator = currAllocator = allocator;
    urefcount = refcount = mapcount = 0;
    data = origdata = 0;
    size = 0;
    flags = static_cast<UMatData::MemoryFlag>(0);
    handle = 0;
    userdata = 0;
    allocatorFlags_ = 0;
    originalUMatData = রিক্ত;
}

UMatData::~UMatData()
{
    prevAllocator = currAllocator = 0;
    urefcount = refcount = 0;
    CV_Assert(mapcount == 0);
    data = origdata = 0;
    size = 0;
    তার্কিক isAsyncCleanup = !!(flags & UMatData::ASYNC_CLEANUP);
    flags = static_cast<UMatData::MemoryFlag>(0);
    handle = 0;
    userdata = 0;
    allocatorFlags_ = 0;
    যদি (originalUMatData)
    {
        তার্কিক showWarn = মিছা;
        UMatData* u = originalUMatData;
        তার্কিক zero_Ref = CV_XADD(&(u->refcount), -1) == 1;
        যদি (zero_Ref)
        {
            // simulate Mat::deallocate
            যদি (u->mapcount != 0)
            {
                (u->currAllocator ? u->currAllocator : /* TODO allocator ? allocator :*/ Mat::getDefaultAllocator())->unmap(u);
            }
            অন্যথা
            {
                // we don't do "map", so we can't do "unmap"
            }
        }
        তার্কিক zero_URef = CV_XADD(&(u->urefcount), -1) == 1;
        যদি (zero_Ref && !zero_URef)
            showWarn = সঁচা;
        যদি (zero_Ref && zero_URef) // oops, we need to free resources
        {
            showWarn = !isAsyncCleanup;
            // simulate UMat::deallocate
            u->currAllocator->deallocate(u);
        }
#ifndef NDEBUG
        যদি (showWarn)
        {
            স্থির পূর্ণাঙ্ক warn_message_showed = 0;
            যদি (warn_message_showed++ < 100)
            {
                fflush(stdout);
                fprintf(stderr, "\n! OPENCV warning: getUMat()/getMat() call chain possible problem."
                                "\n!                 Base object is dead, while nested/derived object is still alive or processed."
                                "\n!                 Please check lifetime of UMat/Mat objects!\n");
                fflush(stderr);
            }
        }
#অন্যথা
        CV_UNUSED(showWarn);
#endif
        originalUMatData = রিক্ত;
    }
}

#ifndef OPENCV_DISABLE_THREAD_SUPPORT

// it should be a prime number for the best hash function
গণনা { UMAT_NLOCKS = 31 };
স্থির Mutex umatLocks[UMAT_NLOCKS];

স্থির size_t getUMatDataLockIndex(ধ্রুৱ UMatData* u)
{
    size_t idx = ((size_t)(শূন্য*)u) % UMAT_NLOCKS;
    ঘূৰাই idx;
}

শূন্য UMatData::lock()
{
    size_t idx = getUMatDataLockIndex(এতত্);
    //printf("%d lock(%d)\n", cv::utils::getThreadID(), (int)idx);
    umatLocks[idx].lock();
}

শূন্য UMatData::unlock()
{
    size_t idx = getUMatDataLockIndex(এতত্);
    //printf("%d unlock(%d)\n", cv::utils::getThreadID(), (int)idx);
    umatLocks[idx].unlock();
}


// Do not allow several lock() calls with different UMatData objects.
সংরচনা UMatDataAutoLocker
{
    পূর্ণাঙ্ক usage_গুনতি;
    UMatData* locked_objects[2];
    UMatDataAutoLocker() : usage_গুনতি(0) { locked_objects[0] = রিক্ত; locked_objects[1] = রিক্ত; }

    শূন্য lock(UMatData*& u1)
    {
        তার্কিক locked_1 = (u1 == locked_objects[0] || u1 == locked_objects[1]);
        যদি (locked_1)
        {
            u1 = রিক্ত;
            ঘূৰাই;
        }
        CV_Assert(usage_গুনতি == 0);  // UMatDataAutoLock can't be used multiple times from the same thread
        usage_গুনতি = 1;
        locked_objects[0] = u1;
        u1->lock();
    }
    শূন্য lock(UMatData*& u1, UMatData*& u2)
    {
        তার্কিক locked_1 = (u1 == locked_objects[0] || u1 == locked_objects[1]);
        তার্কিক locked_2 = (u2 == locked_objects[0] || u2 == locked_objects[1]);
        যদি (locked_1)
            u1 = রিক্ত;
        যদি (locked_2)
            u2 = রিক্ত;
        যদি (locked_1 && locked_2)
            ঘূৰাই;
        CV_Assert(usage_গুনতি == 0);  // UMatDataAutoLock can't be used multiple times from the same thread
        usage_গুনতি = 1;
        locked_objects[0] = u1;
        locked_objects[1] = u2;
        যদি (u1)
            u1->lock();
        যদি (u2)
            u2->lock();
    }
    শূন্য release(UMatData* u1, UMatData* u2)
    {
        যদি (u1 == রিক্ত && u2 == রিক্ত)
            ঘূৰাই;
        CV_Assert(usage_গুনতি == 1);
        usage_গুনতি = 0;
        যদি (u1)
            u1->unlock();
        যদি (u2)
            u2->unlock();
        locked_objects[0] = রিক্ত; locked_objects[1] = রিক্ত;
    }
};
স্থির TLSData<UMatDataAutoLocker>& getUMatDataAutoLockerTLS()
{
    CV_SINGLETON_LAZY_INIT_REF(TLSData<UMatDataAutoLocker>, নূতন TLSData<UMatDataAutoLocker>());
}
স্থির UMatDataAutoLocker& getUMatDataAutoLocker() { ঘূৰাই getUMatDataAutoLockerTLS().getRef(); }


UMatDataAutoLock::UMatDataAutoLock(UMatData* u) : u1(u), u2(রিক্ত)
{
    getUMatDataAutoLocker().lock(u1);
}
UMatDataAutoLock::UMatDataAutoLock(UMatData* u1_, UMatData* u2_) : u1(u1_), u2(u2_)
{
    যদি (getUMatDataLockIndex(u1) > getUMatDataLockIndex(u2))
    {
        std::swap(u1, u2);
    }
    getUMatDataAutoLocker().lock(u1, u2);
}
UMatDataAutoLock::~UMatDataAutoLock()
{
    getUMatDataAutoLocker().release(u1, u2);
}

#অন্যথা

শূন্য UMatData::lock()
{
    // nothing in OPENCV_DISABLE_THREAD_SUPPORT mode
}

শূন্য UMatData::unlock()
{
    // nothing in OPENCV_DISABLE_THREAD_SUPPORT mode
}

UMatDataAutoLock::UMatDataAutoLock(UMatData* u) : u1(u), u2(রিক্ত)
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

UMat::UMat(পূর্ণাঙ্ক _rows, পূর্ণাঙ্ক _cols, পূর্ণাঙ্ক _type, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_rows, _cols, _type);
}

UMat::UMat(পূর্ণাঙ্ক _rows, পূর্ণাঙ্ক _cols, পূর্ণাঙ্ক _type, ধ্রুৱ Scalar& _s, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_rows, _cols, _type);
    *এতত্ = _s;
}

UMat::UMat(Size _sz, পূর্ণাঙ্ক _type, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create( _sz.height, _sz.width, _type );
}

UMat::UMat(Size _sz, পূর্ণাঙ্ক _type, ধ্রুৱ Scalar& _s, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_sz.height, _sz.width, _type);
    *এতত্ = _s;
}

UMat::UMat(পূর্ণাঙ্ক _dims, ধ্রুৱ পূর্ণাঙ্ক* _sz, পূর্ণাঙ্ক _type, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_dims, _sz, _type);
}

UMat::UMat(পূর্ণাঙ্ক _dims, ধ্রুৱ পূর্ণাঙ্ক* _sz, পূর্ণাঙ্ক _type, ধ্রুৱ Scalar& _s, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_dims, _sz, _type);
    *এতত্ = _s;
}

UMat::UMat(ধ্রুৱ MatShape& _shape, পূর্ণাঙ্ক _type, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_shape.dims, _shape.p, _type);
}

UMat::UMat(ধ্রুৱ MatShape& _shape, পূর্ণাঙ্ক _type, ধ্রুৱ Scalar& _s, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_shape.dims, _shape.p, _type);
    *এতত্ = _s;
}

UMat::UMat(ধ্রুৱ UMat& m)
: flags(m.flags), dims(m.dims), rows(m.rows), cols(m.cols), allocator(m.allocator),
  usageFlags(m.usageFlags), u(m.u), offset(m.offset), size(m.size), step(m.step)
{
    addref();
}

UMat& UMat::operator=(ধ্রুৱ UMat& m)
{
    যদি( এতত্ != &m )
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
    ঘূৰাই *এতত্;
}

UMat UMat::clone() ধ্রুৱ
{
    UMat m;
    copyTo(m);
    ঘূৰাই m;
}

শূন্য UMat::assignTo(UMat& m, পূর্ণাঙ্ক _type) ধ্রুৱ
{
    যদি( _type < 0 )
        m = *এতত্;
    অন্যথা
        convertTo(m, _type);
}

শূন্য UMat::create(পূর্ণাঙ্ক _rows, পূর্ণাঙ্ক _cols, পূর্ণাঙ্ক _type, UMatUsageFlags _usageFlags)
{
    পূর্ণাঙ্ক sz[] = {_rows, _cols};
    create(2, sz, _type, _usageFlags);
}

শূন্য UMat::create(Size _sz, পূর্ণাঙ্ক _type, UMatUsageFlags _usageFlags)
{
    create(_sz.height, _sz.width, _type, _usageFlags);
}

শূন্য UMat::createSameSize(প্ৰবেশ_Array arr, পূর্ণাঙ্ক _type, UMatUsageFlags _usageFlags)
{
    create(arr.shape(), _type, _usageFlags);
}

শূন্য UMat::addref()
{
    যদি( u )
        CV_XADD(&(u->urefcount), 1);
}

শূন্য UMat::release()
{
    যদি( u && CV_XADD(&(u->urefcount), -1) == 1 )
        deallocate();
    u = 0;
    size.clear();
    dims = cols = rows = 0;
}

তার্কিক UMat::empty() ধ্রুৱ
{
    ঘূৰাই u == 0 || total() == 0;
}

size_t UMat::total() ধ্রুৱ
{
    যদি( dims <= 2 )
        ঘূৰাই (size_t)rows * cols;
    size_t p = 1;
    বাবে( পূর্ণাঙ্ক i = 0; i < dims; i++ )
        p *= size[i];
    ঘূৰাই p;
}

MatShape UMat::shape() ধ্রুৱ
{
    ঘূৰাই size;
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
    m.allocator = রিক্ত;
    m.u = রিক্ত;
    m.offset = 0;
}

UMat& UMat::operator=(UMat&& m)
{
    যদি (এতত্ == &m)
      ঘূৰাই *এতত্;
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
    m.allocator = রিক্ত;
    m.u = রিক্ত;
    m.offset = 0;
    ঘূৰাই *এতত্;
}


MatAllocator* UMat::getStdAllocator()
{
#ifdef HAVE_OPENCL
    যদি (ocl::useOpenCL())
        ঘূৰাই ocl::getOpenCLAllocator();
#endif
    ঘূৰাই Mat::getDefaultAllocator();
}

শূন্য swap( UMat& a, UMat& b )
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


শূন্য setSize( UMat& m, পূর্ণাঙ্ক _dims, ধ্রুৱ পূর্ণাঙ্ক* _sz,
              ধ্রুৱ size_t* _steps, তার্কিক autoSteps )
{
    CV_Assert( 0 <= _dims && _dims <= CV_MAX_DIM && _dims <= MatShape::MAX_DIMS);

    m.dims = _dims;
    m.size = MatShape(_dims, _sz);
    m.step[std::max(_dims-1, 0)] = CV_ELEM_SIZE(m.flags);
    বাবে (পূর্ণাঙ্ক i = _dims-2; i >= 0; i--) {
        size_t autostep = m.size[i+1]*m.step[i+1];
        যদি (_steps) {
            m.step[i] = _steps[i];
            //CV_Assert(m.step[i] >= autostep);
        } অন্যথা যদি (autoSteps) {
            m.step[i] = autostep;
        } অন্যথা {
            m.step[i] = 0;
        }
    }

    যদি( _dims <= 2 )
    {
        m.cols = _dims == 0 ? 1 : _sz ? _sz[_dims > 1] : 0;
        m.rows = _dims < 2 ? 1 : _sz ? _sz[0] : 0;
    } অন্যথা {
        m.cols = m.rows = -1;
    }
}


শূন্য UMat::updateContinuityFlag()
{
    flags = cv::updateContinuityFlag(flags, dims, size.p, step.p);
}


শূন্য finalizeHdr(UMat& m)
{
    m.updateContinuityFlag();
    পূর্ণাঙ্ক d = m.dims;
    যদি( d > 2 )
        m.rows = m.cols = -1;
}


UMat Mat::getUMat(AccessFlag accessFlags, UMatUsageFlags usageFlags) ধ্রুৱ
{
    UMat hdr;
    যদি(!data)
        ঘূৰাই hdr;
    যদি (data != datastart)
    {
        Size wholeSize;
        Point ofs;
        locateROI(wholeSize, ofs);
        Size sz(cols, rows);
        যদি (ofs.x != 0 || ofs.y != 0)
        {
            Mat src = *এতত্;
            পূর্ণাঙ্ক dtop = ofs.y;
            পূর্ণাঙ্ক dbottom = wholeSize.height - src.rows - ofs.y;
            পূর্ণাঙ্ক dleft = ofs.x;
            পূর্ণাঙ্ক dright = wholeSize.width - src.cols - ofs.x;
            src.adjustROI(dtop, dbottom, dleft, dright);
            ঘূৰাই src.getUMat(accessFlags, usageFlags)(cv::Rect(ofs.x, ofs.y, sz.width, sz.height));
        }
    }
    CV_Assert(data == datastart);

    accessFlags |= ACCESS_RW;
    UMatData* new_u = রিক্ত;
    MatStep new_step = step;
    {
        MatAllocator *a = allocator, *a0 = getDefaultAllocator();
        যদি(!a)
            a = a0;
        new_u = a->allocate(dims, size.p, type(), data, new_step.p, accessFlags, usageFlags);
        new_u->originalUMatData = u;
    }
    তার্কিক allocated = মিছা;
    যত্ন
    {
        allocated = UMat::getStdAllocator()->allocate(new_u, accessFlags, usageFlags);
    }
    গৃহাণ (ধ্রুৱ cv::Exception& e)
    {
        fprintf(stderr, "Exception: %s\n", e.what());
    }
    যদি (!allocated)
    {
        allocated = getDefaultAllocator()->allocate(new_u, accessFlags, usageFlags);
        CV_Assert(allocated);
    }
    যদি (u != রিক্ত)
    {
#ifdef HAVE_OPENCL
        যদি (ocl::useOpenCL() && new_u->currAllocator == ocl::getOpenCLAllocator())
        {
            CV_Assert(new_u->tempUMat());
        }
#endif
        CV_XADD(&(u->refcount), 1);
        CV_XADD(&(u->urefcount), 1);
    }
    যত্ন
    {
        hdr.flags = flags;
        hdr.usageFlags = usageFlags;
        setSize(hdr, dims, size.p, new_step.p);
        finalizeHdr(hdr);
        hdr.u = new_u;
        hdr.offset = 0; //data - datastart;
        hdr.addref();
        ঘূৰাই hdr;
    }
    গৃহাণ(...)
    {
        যদি (u != রিক্ত)
        {
            CV_XADD(&(u->refcount), -1);
            CV_XADD(&(u->urefcount), -1);
        }
        new_u->currAllocator->deallocate(new_u);
        ক্ষিপ;
    }

}

শূন্য UMat::create(পূর্ণাঙ্ক d0, ধ্রুৱ পূর্ণাঙ্ক* _sizes, পূর্ণাঙ্ক _type, UMatUsageFlags _usageFlags)
{
    পূর্ণাঙ্ক sz1 = 1, d = d0;
    পূর্ণাঙ্ক i;
    যদি (d == 0) {
        d = 1;
        _sizes = (ধ্রুৱ পূর্ণাঙ্ক*)&sz1;
    }
    CV_Assert(0 <= d && d <= CV_MAX_DIM && _sizes);
    _type = CV_MAT_TYPE(_type);

    // if param value is USAGE_DEFAULT by implicit default param value -or- explicit value
    // ...then don't change the existing usageFlags
    // it is not possible to change usage from non-default to USAGE_DEFAULT through create()
    // ...instead must construct UMat()
    যদি (_usageFlags == cv::USAGE_DEFAULT)
    {
        _usageFlags = usageFlags;
    }

    যদি( u && d == dims && _type == type() && _usageFlags == usageFlags )
    {
        বাবে( i = 0; i < d; i++ )
            যদি( size[i] != _sizes[i] )
                ভঙ্গ;
        যদি( i == d )
            ঘূৰাই;
    }

    পূর্ণাঙ্ক _sizes_backup[CV_MAX_DIM]; // #5991
    যদি (_sizes == (এতত্->size.p))
    {
        বাবে(i = 0; i < d; i++ )
            _sizes_backup[i] = _sizes[i];
        _sizes = _sizes_backup;
    }

    release();
    usageFlags = _usageFlags;
    যদি( d == 0 )
        ঘূৰাই;
    flags = (_type & CV_MAT_TYPE_MASK) | MAGIC_VAL;
    setSize(*এতত্, d, _sizes, 0, সঁচা);
    offset = 0;

    যদি( total() > 0 )
    {
        MatAllocator *a = allocator, *a0 = getStdAllocator();
        যদি (!a)
        {
            a = a0;
            a0 = Mat::getDefaultAllocator();
        }
        যত্ন
        {
            u = a->allocate(dims, size.p, _type, 0, step.p, ACCESS_RW /* ignored */, usageFlags);
            CV_Assert(u != 0);
        }
        গৃহাণ(...)
        {
            যদি(a != a0)
                u = a0->allocate(dims, size.p, _type, 0, step.p, ACCESS_RW /* ignored */, usageFlags);
            CV_Assert(u != 0);
        }
        CV_Assert( step[dims-1] == (size_t)CV_ELEM_SIZE(flags) );
    }

    finalizeHdr(*এতত্);
    addref();
    size.dims = dims = d0;
}

শূন্য UMat::create(ধ্রুৱ std::vector<পূর্ণাঙ্ক>& _sizes, পূর্ণাঙ্ক _type, UMatUsageFlags _usageFlags)
{
    create((পূর্ণাঙ্ক)_sizes.size(), _sizes.data(), _type, _usageFlags);
}

শূন্য UMat::create(ধ্রুৱ MatShape& _shape, পূর্ণাঙ্ক _type, UMatUsageFlags _usageFlags)
{
    যদি (_shape.dims < 0) {
        release();
    } অন্যথা {
        create(_shape.dims, _shape.p, _type, _usageFlags);
        size.layout = _shape.layout;
        size.C = _shape.C;
    }
}

শূন্য UMat::fit(পূর্ণাঙ্ক _dims, ধ্রুৱ পূর্ণাঙ্ক* _sizes, পূর্ণাঙ্ক _type, UMatUsageFlags _usageFlags)
{
    যদি (_usageFlags == cv::USAGE_DEFAULT)
        _usageFlags = usageFlags;
    size_t oldTotalBytes = u ? u->size : 0;
    size_t esz = CV_ELEM_SIZE(_type), newTotal = _dims >= 0;
    বাবে (পূর্ণাঙ্ক i = 0; i < _dims; i++)
        newTotal *= _sizes[i];
    size_t newTotalBytes = newTotal*esz;
    যদি (newTotalBytes > 0 &&
        (!isContinuous() ||
         newTotalBytes > oldTotalBytes ||
         offset != 0 ||
         _usageFlags != usageFlags)) {
        create(_dims, _sizes, _type, _usageFlags);
    } অন্যথা {
        flags = (flags & ~Mat::TYPE_MASK) | CV_MAT_TYPE(_type);
        পূর্ণাঙ্ক _dummy_size = 0;
        setSize(*এতত্, (_dims >= 0 ? _dims : 1), (_dims >= 0 ? _sizes : &_dummy_size), nullptr, সঁচা);
        finalizeHdr(*এতত্);
    }
}

শূন্য UMat::fit(ধ্রুৱ std::vector<পূর্ণাঙ্ক>& _shape, পূর্ণাঙ্ক _type, UMatUsageFlags _usageFlags)
{
    fit((পূর্ণাঙ্ক)_shape.size(), _shape.data(), _type, _usageFlags);
}

শূন্য UMat::fit(ধ্রুৱ MatShape& _shape, পূর্ণাঙ্ক _type, UMatUsageFlags _usageFlags)
{
    fit(_shape.dims, _shape.p, _type, _usageFlags);
    size.layout = _shape.layout;
    size.C = _shape.C;
}

শূন্য UMat::fit(পূর্ণাঙ্ক _rows, পূর্ণাঙ্ক _cols, পূর্ণাঙ্ক _type, UMatUsageFlags _usageFlags)
{
    _type &= TYPE_MASK;
    পূর্ণাঙ্ক sz[] = {_rows, _cols};
    fit(2, sz, _type, _usageFlags);
}

শূন্য UMat::fit(Size _sz, পূর্ণাঙ্ক _type, UMatUsageFlags _usageFlags)
{
    fit(_sz.height, _sz.width, _type, _usageFlags);
}

শূন্য UMat::fitSameSize(প্ৰবেশ_Array m, পূর্ণাঙ্ক _type, UMatUsageFlags _usageFlags)
{
    fit(m.shape(), _type, _usageFlags);
}

শূন্য UMat::copySize(ধ্রুৱ UMat& m)
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

শূন্য UMat::deallocate()
{
    UMatData* u_ = u;
    u = রিক্ত;
    u_->currAllocator->deallocate(u_);
}


UMat::UMat(ধ্রুৱ UMat& m, ধ্রুৱ Range& _rowRange, ধ্রুৱ Range& _colRange)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(USAGE_DEFAULT), u(0), offset(0)
{
    CV_Assert( m.dims >= 2 && m.dims <= MatShape::MAX_DIMS );
    যদি( m.dims > 2 )
    {
        Range rs[MatShape::MAX_DIMS];
        rs[0] = _rowRange;
        rs[1] = _colRange;
        বাবে( পূর্ণাঙ্ক i = 2; i < m.dims; i++ )
            rs[i] = Range::all();
        *এতত্ = m(rs);
        ঘূৰাই;
    }

    *এতত্ = m;
    যদি( _rowRange != Range::all() && _rowRange != Range(0,rows) )
    {
        CV_Assert( 0 <= _rowRange.start &&
                  _rowRange.start <= _rowRange.end &&
                  _rowRange.end <= m.rows );
        size[0] = rows = _rowRange.size();
        offset += step*_rowRange.start;
        flags |= SUBMATRIX_FLAG;
    }

    যদি( _colRange != Range::all() && _colRange != Range(0,cols) )
    {
        CV_Assert( 0 <= _colRange.start &&
                  _colRange.start <= _colRange.end &&
                  _colRange.end <= m.cols );
        size[1] = cols = _colRange.size();
        offset += _colRange.start*elemSize();
        flags |= SUBMATRIX_FLAG;
    }

    updateContinuityFlag();

    যদি( rows <= 0 || cols <= 0 )
    {
        release();
        rows = cols = 0;
    }
}


UMat::UMat(ধ্রুৱ UMat& m, ধ্রুৱ Rect& roi)
    : flags(m.flags), dims(2), rows(roi.height), cols(roi.width),
    allocator(m.allocator), usageFlags(m.usageFlags), u(m.u),
    offset(m.offset + roi.y*m.step[0]), size(2)
{
    CV_Assert( m.dims <= 2 );

    size_t esz = CV_ELEM_SIZE(flags);
    offset += roi.x*esz;
    CV_Assert( 0 <= roi.x && 0 <= roi.width && roi.x + roi.width <= m.cols &&
              0 <= roi.y && 0 <= roi.height && roi.y + roi.height <= m.rows );
    যদি( roi.width < m.cols || roi.height < m.rows )
        flags |= SUBMATRIX_FLAG;

    size[0] = rows;
    size[1] = cols;
    step[0] = m.dims == 2 ? m.step[0] : cols*esz;
    step[1] = esz;

    updateContinuityFlag();

    addref();
    যদি( rows <= 0 || cols <= 0 )
    {
        rows = cols = 0;
        release();
    }
}


UMat::UMat(ধ্রুৱ UMat& m, ধ্রুৱ Range* ranges)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(USAGE_DEFAULT), u(0), offset(0)
{
    পূর্ণাঙ্ক i, d = m.dims;

    CV_Assert(ranges);
    বাবে( i = 0; i < d; i++ )
    {
        Range r = ranges[i];
        CV_Assert( r == Range::all() || (0 <= r.start && r.start < r.end && r.end <= m.size[i]) );
    }
    *এতত্ = m;
    বাবে( i = 0; i < d; i++ )
    {
        Range r = ranges[i];
        যদি( r != Range::all() && r != Range(0, size.p[i]))
        {
            size.p[i] = r.end - r.start;
            offset += r.start*step.p[i];
            flags |= SUBMATRIX_FLAG;
        }
    }

    যদি (d <= 2) {
        rows = d == 2 ? size[0] : 1;
        cols = d <= 0 ? (d >= 0) : size[d > 1];
    }
    updateContinuityFlag();
}

UMat::UMat(ধ্রুৱ UMat& m, ধ্রুৱ std::vector<Range>& ranges)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(USAGE_DEFAULT), u(0), offset(0)
{
    পূর্ণাঙ্ক i, d = m.dims;

    CV_Assert((পূর্ণাঙ্ক)ranges.size() == d);
    বাবে (i = 0; i < d; i++)
    {
        Range r = ranges[i];
        CV_Assert(r == Range::all() || (0 <= r.start && r.start < r.end && r.end <= m.size[i]));
    }
    *এতত্ = m;
    বাবে (i = 0; i < d; i++)
    {
        Range r = ranges[i];
        যদি (r != Range::all() && r != Range(0, size.p[i]))
        {
            size.p[i] = r.end - r.start;
            offset += r.start*step.p[i];
            flags |= SUBMATRIX_FLAG;
        }
    }

    যদি (d <= 2) {
        rows = d == 2 ? size[0] : 1;
        cols = d <= 0 ? (d >= 0) : size[d > 1];
    }
    updateContinuityFlag();
}

UMat UMat::diag(পূর্ণাঙ্ক d) ধ্রুৱ
{
    CV_Assert( dims <= 2 );
    UMat m = *এতত্;
    size_t esz = elemSize();
    পূর্ণাঙ্ক len;

    যদি( d >= 0 )
    {
        len = std::min(cols - d, rows);
        m.offset += esz*d;
    }
    অন্যথা
    {
        len = std::min(rows + d, cols);
        m.offset -= step[0]*d;
    }
    CV_DbgAssert( len > 0 );

    m.size[0] = m.rows = len;
    m.size[1] = m.cols = 1;
    m.step[0] += (len > 1 ? esz : 0);

    m.updateContinuityFlag();

    যদি( size() != Size(1,1) )
        m.flags |= SUBMATRIX_FLAG;

    ঘূৰাই m;
}

শূন্য UMat::locateROI( Size& wholeSize, Point& ofs ) ধ্রুৱ
{
    CV_Assert( dims <= 2 && step[0] > 0 );
    size_t esz = elemSize(), minstep;
    ptrdiff_t delta1 = (ptrdiff_t)offset, delta2 = (ptrdiff_t)u->size;

    যদি( delta1 == 0 )
        ofs.x = ofs.y = 0;
    অন্যথা
    {
        ofs.y = (পূর্ণাঙ্ক)(delta1/step[0]);
        ofs.x = (পূর্ণাঙ্ক)((delta1 - step[0]*ofs.y)/esz);
        CV_DbgAssert( offset == (size_t)(ofs.y*step[0] + ofs.x*esz) );
    }
    minstep = (ofs.x + cols)*esz;
    wholeSize.height = (পূর্ণাঙ্ক)((delta2 - minstep)/step[0] + 1);
    wholeSize.height = std::max(wholeSize.height, ofs.y + rows);
    wholeSize.width = (পূর্ণাঙ্ক)((delta2 - step*(wholeSize.height-1))/esz);
    wholeSize.width = std::max(wholeSize.width, ofs.x + cols);
}


UMat& UMat::adjustROI( পূর্ণাঙ্ক dtop, পূর্ণাঙ্ক dbottom, পূর্ণাঙ্ক dleft, পূর্ণাঙ্ক dright )
{
    CV_Assert( dims == 2 && step[0] > 0 );
    Size wholeSize; Point ofs;
    size_t esz = elemSize();
    locateROI( wholeSize, ofs );
    পূর্ণাঙ্ক row1 = std::min(std::max(ofs.y - dtop, 0), wholeSize.height), row2 = std::max(0, std::min(ofs.y + rows + dbottom, wholeSize.height));
    পূর্ণাঙ্ক col1 = std::min(std::max(ofs.x - dleft, 0), wholeSize.width), col2 = std::max(0, std::min(ofs.x + cols + dright, wholeSize.width));
    যদি(row1 > row2)
        std::swap(row1, row2);
    যদি(col1 > col2)
        std::swap(col1, col2);

    offset += (row1 - ofs.y)*step + (col1 - ofs.x)*esz;
    rows = row2 - row1; cols = col2 - col1;
    size.p[0] = rows; size.p[1] = cols;
    updateContinuityFlag();
    ঘূৰাই *এতত্;
}


UMat UMat::reshape(পূর্ণাঙ্ক new_cn, পূর্ণাঙ্ক new_rows) ধ্রুৱ
{
    পূর্ণাঙ্ক cn = channels();
    UMat hdr = *এতত্;

    যদি( dims > 2 && new_rows == 0 && new_cn != 0 && size[dims-1]*cn % new_cn == 0 )
    {
        hdr.flags = (hdr.flags & ~CV_MAT_CN_MASK) | ((new_cn-1) << CV_CN_SHIFT);
        hdr.step[dims-1] = CV_ELEM_SIZE(hdr.flags);
        hdr.size[dims-1] = hdr.size[dims-1]*cn / new_cn;
        ঘূৰাই hdr;
    }

    CV_Assert( dims <= 2 );

    যদি( new_cn == 0 )
        new_cn = cn;

    পূর্ণাঙ্ক total_width = cols * cn;

    যদি( (new_cn > total_width || total_width % new_cn != 0) && new_rows == 0 )
        new_rows = rows * total_width / new_cn;

    যদি( new_rows != 0 && new_rows != rows )
    {
        পূর্ণাঙ্ক total_size = total_width * rows;
        যদি( !isContinuous() )
            CV_Error( cv::Error::BadStep,
            "The matrix is not continuous, thus its number of rows can not be changed" );

        total_width = total_size / new_rows;

        যদি( total_width * new_rows != total_size )
            CV_Error( cv::Error::StsBadArg, "The total number of matrix elements "
                                    "is not divisible by the new number of rows" );

        hdr.size[0] = hdr.rows = new_rows;
        hdr.step[0] = total_width * elemSize1();
    } অন্যথা {
        hdr.size[0] = hdr.rows = rows;
        যদি (dims <= 1)
            hdr.step[0] = cols * CV_ELEM_SIZE(flags);
    }

    পূর্ণাঙ্ক new_width = total_width / new_cn;

    যদি( new_width * new_cn != total_width )
        CV_Error( cv::Error::Bad_অঙ্ক_Channels,
        "The total width is not divisible by the new number of channels" );

    hdr.size.dims = hdr.dims = 2;
    hdr.size[1] = hdr.cols = new_width;
    hdr.flags = (hdr.flags & ~CV_MAT_CN_MASK) | ((new_cn-1) << CV_CN_SHIFT);
    hdr.step[1] = CV_ELEM_SIZE(hdr.flags);
    ঘূৰাই hdr;
}

#ifdef HAVE_OPENCL
নামাকাশ {
স্থির তার্কিক ocl_setDiag(ধ্রুৱ UMat& d, UMat& m, পূর্ণাঙ্ক len)
{
    পূর্ণাঙ্ক cn = d.channels();
    পূর্ণাঙ্ক depth = d.depth();

    যদি (depth == CV_64F && !ocl::Device::getDefault().doubleFPConfig())
        ঘূৰাই মিছা;

    String opts = format("-D SET_DIAG -D T1=%s -D cn=%d -D IS_ROW_VECTOR=%d",
                         ocl::memopTypeToStr(depth),
                         cn,
                         (d.rows == 1) ? 1 : 0);

    ocl::Kernel k("setDiag", ocl::core::copyset_oclsrc, opts);
    যদি (k.empty())
        ঘূৰাই মিছা;

    k.args(ocl::KernelArg::WriteOnly(m),
           ocl::KernelArg::ReadOnlyNoSize(d),
           len);

    size_t globalsize[2] = { (size_t)len, (size_t)len };
    ঘূৰাই k.run(2, globalsize, রিক্ত, মিছা);
}
}
#endif

UMat UMat::diag(ধ্রুৱ UMat& d, UMatUsageFlags usageFlags)
{
    CV_INSTRUMENT_REGION();

    CV_Assert(d.cols == 1 || d.rows == 1);
    পূর্ণাঙ্ক len = d.rows + d.cols - 1;

#ifdef HAVE_OPENCL
    যদি (ocl::useOpenCL())
    {
        UMat m(len, len, d.type(), usageFlags);
        যদি (ocl_setDiag(d, m, len))
        {
            CV_IMPL_ADD(CV_IMPL_OCL);
            ঘূৰাই m;
        }
    }
#endif

    UMat m(len, len, d.type(), Scalar(0), usageFlags);
    UMat md = m.diag();
    যদি( d.cols == 1 )
        d.copyTo(md);
    অন্যথা
        transpose(d, md);
    ঘূৰাই m;
}

পূর্ণাঙ্ক UMat::checkVector(পূর্ণাঙ্ক _elemChannels, পূর্ণাঙ্ক _depth, তার্কিক _requireContinuous) ধ্রুৱ
{
    ঘূৰাই (depth() == _depth || _depth <= 0) &&
        (isContinuous() || !_requireContinuous) &&
        ((dims <= 2 && (((rows == 1 || cols == 1) && channels() == _elemChannels) ||
                        (cols == _elemChannels && channels() == 1))) ||
        (dims == 3 && channels() == 1 && size.p[2] == _elemChannels && (size.p[0] == 1 || size.p[1] == 1) &&
         (isContinuous() || step.p[1] == step.p[2]*size.p[2])))
    ? (পূর্ণাঙ্ক)(total()*channels()/_elemChannels) : -1;
}

UMat UMat::reshape(পূর্ণাঙ্ক _cn, পূর্ণাঙ্ক _newndims, ধ্রুৱ পূর্ণাঙ্ক* _newsz) ধ্রুৱ
{
    যদি(_newndims == dims)
    {
        যদি(_newsz == 0)
            ঘূৰাই reshape(_cn);
        যদি(_newndims == 2)
            ঘূৰাই reshape(_cn, _newsz[0]);
    }

    যদি (isContinuous())
    {
        CV_Assert(_cn >= 0 && _newndims >= 0 && _newndims <= CV_MAX_DIM && (_newndims == 0 || _newsz != 0));

        যদি (_cn == 0)
            _cn = এতত্->channels();
        অন্যথা
            CV_Assert(_cn <= CV_CN_MAX);

        size_t total_elem1_ref = এতত্->total() * এতত্->channels();
        size_t total_elem1 = _cn;

        AutoBuffer<পূর্ণাঙ্ক, 4> newsz_buf( (size_t)std::max(_newndims, 1) );

        পূর্ণাঙ্ক m1_idx = -1;

        বাবে (পূর্ণাঙ্ক i = 0; i < _newndims; i++)
        {
            যদি (_newsz[i] >= 0) {
                যদি (_newsz[i] == 0 && i < dims)
                    newsz_buf[i] = size.p[i];
                অন্যথা
                    newsz_buf[i] = _newsz[i];
                total_elem1 *= (size_t)newsz_buf[i];
            } অন্যথা {
                যদি (m1_idx >= 0)
                    CV_Error(cv::Error::StsBadSize, "More than one '-1' occured in the new shape");
                m1_idx = i;
            }
        }

        যদি (m1_idx >= 0) {
            যদি (total_elem1 == 0) {
                CV_Assert(total_elem1_ref == 0);
                total_elem1 = 1;
            }
            CV_Assert(total_elem1_ref % total_elem1 == 0);
            newsz_buf[m1_idx] = (পূর্ণাঙ্ক)(total_elem1_ref / total_elem1);
        } অন্যথা যদি (total_elem1 != total_elem1_ref) {
            CV_Error(cv::Error::StsUnmatchedSizes, "Requested and source matrices have different count of elements");
        }

        UMat hdr = *এতত্;
        hdr.flags = (hdr.flags & ~CV_MAT_CN_MASK) | ((_cn-1) << CV_CN_SHIFT);
        setSize(hdr, _newndims, newsz_buf.data(), রিক্ত, সঁচা);

        ঘূৰাই hdr;
    }

    CV_Error(cv::Error::StsNotImplemented, "Reshaping of n-dimensional non-continuous matrices is not supported yet");
}

UMat UMat::reshape(পূর্ণাঙ্ক _cn, ধ্রুৱ MatShape& _newshape) ধ্রুৱ
{
    যদি (_newshape.dims < 0) {
        পূর্ণাঙ্ক newshape[] = {0};
        ঘূৰাই reshape(_cn, 1, newshape);
    }
    ঘূৰাই reshape(_cn, _newshape.dims, _newshape.p);
}

Mat UMat::getMat(AccessFlag accessFlags) ধ্রুৱ
{
    যদি(!u)
        ঘূৰাই Mat();
    // TODO Support ACCESS_READ (ACCESS_WRITE) without unnecessary data transfers
    accessFlags |= ACCESS_RW;
    UMatDataAutoLock autolock(u);
    যত্ন
    {
        যদি(CV_XADD(&u->refcount, 1) == 0)
            u->currAllocator->map(u, accessFlags);
        যদি (u->data != 0)
        {
            Mat hdr(dims, size.p, type(), u->data + offset, step.p);
            hdr.flags = flags;
            hdr.u = u;
            hdr.datastart = u->data;
            hdr.data = u->data + offset;
            hdr.datalimit = hdr.dataend = u->data + u->size;
            ঘূৰাই hdr;
        }
    }
    গৃহাণ(...)
    {
        CV_XADD(&u->refcount, -1);
        ক্ষিপ;
    }
    CV_XADD(&u->refcount, -1);
    CV_Assert(u->data != 0 && "Error mapping of UMat to host memory.");
    ঘূৰাই Mat();
}

শূন্য* UMat::handle(AccessFlag accessFlags) ধ্রুৱ
{
    যদি( !u )
        ঘূৰাই 0;

    CV_Assert(u->refcount == 0);
    CV_Assert(!u->deviceCopyObsolete() || u->copyOnMap());
    যদি (u->deviceCopyObsolete())
    {
        u->currAllocator->unmap(u);
    }

    যদি (!!(accessFlags & ACCESS_WRITE))
        u->markHostCopyObsolete(সঁচা);

    ঘূৰাই u->handle;
}

শূন্য UMat::ndoffset(size_t* ofs) ধ্রুৱ
{
    // offset = step[0]*ofs[0] + step[1]*ofs[1] + step[2]*ofs[2] + ...;
    size_t val = offset;
    বাবে( পূর্ণাঙ্ক i = 0; i < dims; i++ )
    {
        size_t s = step.p[i];
        ofs[i] = val / s;
        val -= ofs[i]*s;
    }
}

শূন্য UMat::copyTo(OutputArray _dst) ধ্রুৱ
{
    CV_INSTRUMENT_REGION();

#ifdef HAVE_CUDA
    যদি (_dst.isGpuMat())
    {
        _dst.getGpuMat().upload(*এতত্);
        ঘূৰাই;
    }
#endif

    পূর্ণাঙ্ক stype = type();
    পূর্ণাঙ্ক dtype = _dst.type();
    যদি( _dst.fixedType() && dtype != stype )
    {
        CV_Assert( CV_MAT_CN(stype) == CV_MAT_CN(dtype) );
        convertTo( _dst, dtype );
        ঘূৰাই;
    }

    যদি( dims == 0 && empty() )
    {
        _dst.release();
        শূন্য* obj = _dst.getObj();
        যদি (_dst.isMat())
            reinterpret_cast<Mat*>(obj)->flags = Mat::MAGIC_VAL | Mat::CONTINUOUS_FLAG | stype;
        অন্যথা যদি (_dst.isUMat())
            reinterpret_cast<UMat*>(obj)->flags = UMat::MAGIC_VAL | UMat::CONTINUOUS_FLAG | stype;
        অন্যথা যদি (_dst.isGpuMat())
            reinterpret_cast<cuda::GpuMat*>(obj)->flags = stype;
        ঘূৰাই;
    }

    _dst.create( dims, size.p, stype );
    যদি (empty())
        ঘূৰাই;

    size_t sz[CV_MAX_DIM] = {1}, srcofs[CV_MAX_DIM]={0}, dstofs[CV_MAX_DIM]={0};
    size_t esz = CV_ELEM_SIZE(stype);
    পূর্ণাঙ্ক i, d = std::max(dims, 1);
    বাবে( i = 0; i < d; i++ )
        sz[i] = size.p[i];
    sz[d-1] *= esz;
    ndoffset(srcofs);
    srcofs[d-1] *= esz;

    যদি( _dst.isUMat() )
    {
        UMat dst = _dst.getUMat();
        CV_Assert(dst.u);
        যদি( u == dst.u && dst.offset == offset )
            ঘূৰাই;

        যদি (u->currAllocator == dst.u->currAllocator)
        {
            dst.ndoffset(dstofs);
            dstofs[dims-1] *= esz;
            u->currAllocator->copy(u, dst.u, d, sz, srcofs, step.p, dstofs, dst.step.p, মিছা);
            ঘূৰাই;
        }

        // different allocators: never map a device destination, upload into it instead
        // (through a host Mat when both sides are device memory)
        MatAllocator* hostAllocator = Mat::getDefaultAllocator();
        যদি (dst.u->currAllocator != hostAllocator)
        {
            যদি (u->currAllocator == hostAllocator)
                getMat(ACCESS_READ).copyTo(_dst);
            অন্যথা
            {
                Mat tmp;
                copyTo(tmp);
                tmp.copyTo(_dst);
            }
            ঘূৰাই;
        }
    }

    Mat dst = _dst.getMat();
    u->currAllocator->download(u, dst.ptr(), d, sz, srcofs, step.p, dst.step.p);
}

শূন্য UMat::copyTo(OutputArray _dst, প্ৰবেশ_Array _mask) ধ্রুৱ
{
    CV_INSTRUMENT_REGION();

    যদি( _mask.empty() )
    {
        copyTo(_dst);
        ঘূৰাই;
    }
#ifdef HAVE_OPENCL
    পূর্ণাঙ্ক cn = channels(), mtype = _mask.type(), mdepth = CV_MAT_DEPTH(mtype), mcn = CV_MAT_CN(mtype);
    CV_Assert( (mdepth == CV_8U || mdepth == CV_8S || mdepth == CV_Bool) && (mcn == 1 || mcn == cn) );

    যদি (ocl::useOpenCL() && _dst.isUMat() && dims <= 2)
    {
        UMatData * prevu = _dst.getUMat().u;
        _dst.create( size, type() );

        UMat dst = _dst.getUMat();

        তার্কিক haveDstUninit = মিছা;
        যদি( prevu != dst.u ) // do not leave dst uninitialized
            haveDstUninit = সঁচা;

        String opts = format("-D COPY_TO_MASK -D T1=%s -D scn=%d -D mcn=%d%s",
                             ocl::memopTypeToStr(depth()), cn, mcn,
                             haveDstUninit ? " -D HAVE_DST_UNINIT" : "");

        ocl::Kernel k("copyToMask", ocl::core::copyset_oclsrc, opts);
        যদি (!k.empty())
        {
            k.args(ocl::KernelArg::ReadOnlyNoSize(*এতত্),
                   ocl::KernelArg::ReadOnlyNoSize(_mask.getUMat()),
                   haveDstUninit ? ocl::KernelArg::WriteOnly(dst) :
                                   ocl::KernelArg::ReadWrite(dst));

            size_t globalsize[2] = { (size_t)cols, (size_t)rows };
            যদি (k.run(2, globalsize, রিক্ত, মিছা))
            {
                CV_IMPL_ADD(CV_IMPL_OCL);
                ঘূৰাই;
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

UMat& UMat::setTo(প্ৰবেশ_Array মান, প্ৰবেশ_Array _mask)
{
    CV_INSTRUMENT_REGION();

    তার্কিক haveMask = !_mask.empty();
#ifdef HAVE_OPENCL
    পূর্ণাঙ্ক tp = type(), cn = CV_MAT_CN(tp), d = CV_MAT_DEPTH(tp);

    যদি( dims <= 2 && cn <= 4 && CV_MAT_DEPTH(tp) < CV_64F && ocl::useOpenCL() )
    {
        Mat value = মান.getMat();
        CV_Assert( checkScalar(value, type(), মান.kind(), প্ৰবেশ_Array_2::UMAT) );
        পূর্ণাঙ্ক kercn = haveMask || cn == 3 ? cn : std::max(cn, ocl::predictOptimalVectorWidth(*এতত্)),
                kertp = CV_MAKE_TYPE(d, kercn);

        দ্ৱিগুণ buf[16] = { 0, 0, 0, 0, 0, 0, 0, 0,
                           0, 0, 0, 0, 0, 0, 0, 0 };
        convertAndUnrollScalar(value, tp, (uchar *)buf, kercn / cn);

        পূর্ণাঙ্ক scalarcn = kercn == 3 ? 4 : kercn, rowsPerWI = ocl::Device::getDefault().isIntel() ? 4 : 1;
        String opts = format("-D dstT=%s -D rowsPerWI=%d -D dstST=%s -D dstT1=%s -D cn=%d",
                             ocl::memopTypeToStr(kertp), rowsPerWI,
                             ocl::memopTypeToStr(CV_MAKETYPE(d, scalarcn)),
                             ocl::memopTypeToStr(d), kercn);

        ocl::Kernel setK(haveMask ? "setMask" : "set", ocl::core::copyset_oclsrc, opts);
        যদি( !setK.empty() )
        {
            ocl::KernelArg scalararg(ocl::KernelArg::CONSTANT, 0, 0, 0, buf, CV_ELEM_SIZE(d) * scalarcn);
            UMat mask;

            যদি( haveMask )
            {
                mask = _mask.getUMat();
                CV_Assert( mask.size() == size() && (mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool) );
                ocl::KernelArg maskarg = ocl::KernelArg::ReadOnlyNoSize(mask),
                        dstarg = ocl::KernelArg::ReadWrite(*এতত্);
                setK.args(maskarg, dstarg, scalararg);
            }
            অন্যথা
            {
                ocl::KernelArg dstarg = ocl::KernelArg::WriteOnly(*এতত্, cn, kercn);
                setK.args(dstarg, scalararg);
            }

            size_t globalsize[] = { (size_t)cols * cn / kercn, ((size_t)rows + rowsPerWI - 1) / rowsPerWI };
            যদি( setK.run(2, globalsize, রিক্ত, মিছা) )
            {
                CV_IMPL_ADD(CV_IMPL_OCL);
                ঘূৰাই *এতত্;
            }
        }
    }
#endif
    Mat m = getMat(haveMask ? ACCESS_RW : ACCESS_WRITE);
    m.setTo(মান, _mask);
    ঘূৰাই *এতত্;
}

UMat& UMat::operator = (ধ্রুৱ Scalar& s)
{
    setTo(s);
    ঘূৰাই *এতত্;
}

UMat UMat::t() ধ্রুৱ
{
    UMat m;
    transpose(*এতত্, m);
    ঘূৰাই m;
}

UMat UMat::zeros(পূর্ণাঙ্ক rows, পূর্ণাঙ্ক cols, পূর্ণাঙ্ক type, UMatUsageFlags usageFlags)
{
    ঘূৰাই UMat(rows, cols, type, Scalar::all(0), usageFlags);
}

UMat UMat::zeros(Size size, পূর্ণাঙ্ক type, UMatUsageFlags usageFlags)
{
    ঘূৰাই UMat(size, type, Scalar::all(0), usageFlags);
}

UMat UMat::zeros(পূর্ণাঙ্ক ndims, ধ্রুৱ পূর্ণাঙ্ক* sz, পূর্ণাঙ্ক type, UMatUsageFlags usageFlags)
{
    ঘূৰাই UMat(ndims, sz, type, Scalar::all(0), usageFlags);
}

UMat UMat::zeros(ধ্রুৱ MatShape& shape, পূর্ণাঙ্ক type, UMatUsageFlags usageFlags)
{
    ঘূৰাই UMat(shape.dims, shape.p, type, Scalar::all(0), usageFlags);
}

UMat UMat::ones(পূর্ণাঙ্ক rows, পূর্ণাঙ্ক cols, পূর্ণাঙ্ক type, UMatUsageFlags usageFlags)
{
    ঘূৰাই UMat(rows, cols, type, Scalar(1), usageFlags);
}

UMat UMat::ones(Size size, পূর্ণাঙ্ক type, UMatUsageFlags usageFlags)
{
    ঘূৰাই UMat(size, type, Scalar(1), usageFlags);
}

UMat UMat::ones(পূর্ণাঙ্ক ndims, ধ্রুৱ পূর্ণাঙ্ক* sz, পূর্ণাঙ্ক type, UMatUsageFlags usageFlags)
{
    ঘূৰাই UMat(ndims, sz, type, Scalar(1), usageFlags);
}

UMat UMat::ones(ধ্রুৱ MatShape& shape, পূর্ণাঙ্ক type, UMatUsageFlags usageFlags)
{
    ঘূৰাই UMat(shape.dims, shape.p, type, Scalar(1), usageFlags);
}

}

/* End of file. */
