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

#অন্তর্ভাব "precomp.hpp"
#অন্তর্ভাব "opencl_kernels_core.hpp"
#অন্তর্ভাব "umatrix.hpp"

#অন্তর্ভাব <opencv2/core/utils/tls.hpp>

///////////////////////////////// UMat implementation ///////////////////////////////

নামাকাশ cv {

// forward decls, implementation is below in this file
শূন্য set_আকার(UMat& m, পূর্ণসংখ্যা _dims, ধ্রুবক পূর্ণসংখ্যা* _sz, ধ্রুবক size_t* _steps,
             যৌক্তিক autoSteps = মিথ্যা);

শূন্য updateContinuityFlag(UMat& m);
শূন্য finalizeHdr(UMat& m);

UMat_তথ্য::UMat_তথ্য(ধ্রুবক MatAllocator* allocator)
{
    পূর্ব_Allocator = currAllocator = allocator;
    urefcount = refcount = mapcount = 0;
    তথ্য = origdata = 0;
    size = 0;
    flags = static_cast<UMat_তথ্য::MemoryFlag>(0);
    handle = 0;
    userdata = 0;
    allocatorFlags_ = 0;
    original_UMat_তথ্য = ফাঁকা;
}

UMat_তথ্য::~UMat_তথ্য()
{
    পূর্ব_Allocator = currAllocator = 0;
    urefcount = refcount = 0;
    CV_Assert(mapcount == 0);
    তথ্য = origdata = 0;
    size = 0;
    যৌক্তিক isAsyncCleanup = !!(flags & UMat_তথ্য::ASYNC_CLEANUP);
    flags = static_cast<UMat_তথ্য::MemoryFlag>(0);
    handle = 0;
    userdata = 0;
    allocatorFlags_ = 0;
    যদি (original_UMat_তথ্য)
    {
        যৌক্তিক showWarn = মিথ্যা;
        UMat_তথ্য* u = original_UMat_তথ্য;
        যৌক্তিক zero_Ref = CV_XADD(&(u->refcount), -1) == 1;
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
        যৌক্তিক zero_URef = CV_XADD(&(u->urefcount), -1) == 1;
        যদি (zero_Ref && !zero_URef)
            showWarn = সত্য;
        যদি (zero_Ref && zero_URef) // oops, we need to free resources
        {
            showWarn = !isAsyncCleanup;
            // simulate UMat::deallocate
            u->currAllocator->deallocate(u);
        }
#ifndef NDEBUG
        যদি (showWarn)
        {
            স্থির পূর্ণসংখ্যা warn_message_showed = 0;
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
        original_UMat_তথ্য = ফাঁকা;
    }
}

#ifndef OPENCV_DISABLE_THREAD_SUPPORT

// it should be a prime number for the best hash function
গণনা { UMAT_NLOCKS = 31 };
স্থির Mutex umatLocks[UMAT_NLOCKS];

স্থির size_t get_UMat_তথ্য_Lock_সূচক(ধ্রুবক UMat_তথ্য* u)
{
    size_t idx = ((size_t)(শূন্য*)u) % UMAT_NLOCKS;
    ফেরত idx;
}

শূন্য UMat_তথ্য::lock()
{
    size_t idx = get_UMat_তথ্য_Lock_সূচক(এই);
    //printf("%d lock(%d)\n", cv::utils::getThreadID(), (int)idx);
    umatLocks[idx].lock();
}

শূন্য UMat_তথ্য::unlock()
{
    size_t idx = get_UMat_তথ্য_Lock_সূচক(এই);
    //printf("%d unlock(%d)\n", cv::utils::getThreadID(), (int)idx);
    umatLocks[idx].unlock();
}


// Do not allow several lock() calls with different UMatData objects.
কাঠামো UMat_তথ্য_Auto_Locker
{
    পূর্ণসংখ্যা usage_গুনতি;
    UMat_তথ্য* locked_objects[2];
    UMat_তথ্য_Auto_Locker() : usage_গুনতি(0) { locked_objects[0] = ফাঁকা; locked_objects[1] = ফাঁকা; }

    শূন্য lock(UMat_তথ্য*& u1)
    {
        যৌক্তিক locked_1 = (u1 == locked_objects[0] || u1 == locked_objects[1]);
        যদি (locked_1)
        {
            u1 = ফাঁকা;
            ফেরত;
        }
        CV_Assert(usage_গুনতি == 0);  // UMatDataAutoLock can't be used multiple times from the same thread
        usage_গুনতি = 1;
        locked_objects[0] = u1;
        u1->lock();
    }
    শূন্য lock(UMat_তথ্য*& u1, UMat_তথ্য*& u2)
    {
        যৌক্তিক locked_1 = (u1 == locked_objects[0] || u1 == locked_objects[1]);
        যৌক্তিক locked_2 = (u2 == locked_objects[0] || u2 == locked_objects[1]);
        যদি (locked_1)
            u1 = ফাঁকা;
        যদি (locked_2)
            u2 = ফাঁকা;
        যদি (locked_1 && locked_2)
            ফেরত;
        CV_Assert(usage_গুনতি == 0);  // UMatDataAutoLock can't be used multiple times from the same thread
        usage_গুনতি = 1;
        locked_objects[0] = u1;
        locked_objects[1] = u2;
        যদি (u1)
            u1->lock();
        যদি (u2)
            u2->lock();
    }
    শূন্য release(UMat_তথ্য* u1, UMat_তথ্য* u2)
    {
        যদি (u1 == ফাঁকা && u2 == ফাঁকা)
            ফেরত;
        CV_Assert(usage_গুনতি == 1);
        usage_গুনতি = 0;
        যদি (u1)
            u1->unlock();
        যদি (u2)
            u2->unlock();
        locked_objects[0] = ফাঁকা; locked_objects[1] = ফাঁকা;
    }
};
স্থির TLSData<UMat_তথ্য_Auto_Locker>& get_UMat_তথ্য_Auto_Locker_TLS()
{
    CV_SINGLETON_LAZY_INIT_REF(TLSData<UMat_তথ্য_Auto_Locker>, নতুন TLSData<UMat_তথ্য_Auto_Locker>());
}
স্থির UMat_তথ্য_Auto_Locker& get_UMat_তথ্য_Auto_Locker() { ফেরত get_UMat_তথ্য_Auto_Locker_TLS().getRef(); }


UMat_তথ্য_Auto_Lock::UMat_তথ্য_Auto_Lock(UMat_তথ্য* u) : u1(u), u2(ফাঁকা)
{
    get_UMat_তথ্য_Auto_Locker().lock(u1);
}
UMat_তথ্য_Auto_Lock::UMat_তথ্য_Auto_Lock(UMat_তথ্য* u1_, UMat_তথ্য* u2_) : u1(u1_), u2(u2_)
{
    যদি (get_UMat_তথ্য_Lock_সূচক(u1) > get_UMat_তথ্য_Lock_সূচক(u2))
    {
        std::swap(u1, u2);
    }
    get_UMat_তথ্য_Auto_Locker().lock(u1, u2);
}
UMat_তথ্য_Auto_Lock::~UMat_তথ্য_Auto_Lock()
{
    get_UMat_তথ্য_Auto_Locker().release(u1, u2);
}

#অন্যথা

শূন্য UMat_তথ্য::lock()
{
    // nothing in OPENCV_DISABLE_THREAD_SUPPORT mode
}

শূন্য UMat_তথ্য::unlock()
{
    // nothing in OPENCV_DISABLE_THREAD_SUPPORT mode
}

UMat_তথ্য_Auto_Lock::UMat_তথ্য_Auto_Lock(UMat_তথ্য* u) : u1(u), u2(ফাঁকা)
{
    // nothing in OPENCV_DISABLE_THREAD_SUPPORT mode
}
UMat_তথ্য_Auto_Lock::UMat_তথ্য_Auto_Lock(UMat_তথ্য* u1_, UMat_তথ্য* u2_) : u1(u1_), u2(u2_)
{
    // nothing in OPENCV_DISABLE_THREAD_SUPPORT mode
}
UMat_তথ্য_Auto_Lock::~UMat_তথ্য_Auto_Lock()
{
    // nothing in OPENCV_DISABLE_THREAD_SUPPORT mode
}

#endif  // OPENCV_DISABLE_THREAD_SUPPORT

//////////////////////////////// UMat ////////////////////////////////

UMat::UMat(UMatUsageFlags _usageFlags) CV_NOEXCEPT
: flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{}

UMat::UMat(পূর্ণসংখ্যা _rows, পূর্ণসংখ্যা _cols, পূর্ণসংখ্যা _type, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_rows, _cols, _type);
}

UMat::UMat(পূর্ণসংখ্যা _rows, পূর্ণসংখ্যা _cols, পূর্ণসংখ্যা _type, ধ্রুবক Scalar& _s, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_rows, _cols, _type);
    *এই = _s;
}

UMat::UMat(আকার_2 _sz, পূর্ণসংখ্যা _type, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create( _sz.height, _sz.width, _type );
}

UMat::UMat(আকার_2 _sz, পূর্ণসংখ্যা _type, ধ্রুবক Scalar& _s, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_sz.height, _sz.width, _type);
    *এই = _s;
}

UMat::UMat(পূর্ণসংখ্যা _dims, ধ্রুবক পূর্ণসংখ্যা* _sz, পূর্ণসংখ্যা _type, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_dims, _sz, _type);
}

UMat::UMat(পূর্ণসংখ্যা _dims, ধ্রুবক পূর্ণসংখ্যা* _sz, পূর্ণসংখ্যা _type, ধ্রুবক Scalar& _s, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_dims, _sz, _type);
    *এই = _s;
}

UMat::UMat(ধ্রুবক MatShape& _shape, পূর্ণসংখ্যা _type, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_shape.dims, _shape.p, _type);
}

UMat::UMat(ধ্রুবক MatShape& _shape, পূর্ণসংখ্যা _type, ধ্রুবক Scalar& _s, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_shape.dims, _shape.p, _type);
    *এই = _s;
}

UMat::UMat(ধ্রুবক UMat& m)
: flags(m.flags), dims(m.dims), rows(m.rows), cols(m.cols), allocator(m.allocator),
  usageFlags(m.usageFlags), u(m.u), offset(m.offset), size(m.size), step(m.step)
{
    addref();
}

UMat& UMat::operator=(ধ্রুবক UMat& m)
{
    যদি( এই != &m )
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
    ফেরত *এই;
}

UMat UMat::clone() ধ্রুবক
{
    UMat m;
    copyTo(m);
    ফেরত m;
}

শূন্য UMat::assignTo(UMat& m, পূর্ণসংখ্যা _type) ধ্রুবক
{
    যদি( _type < 0 )
        m = *এই;
    অন্যথা
        convertTo(m, _type);
}

শূন্য UMat::create(পূর্ণসংখ্যা _rows, পূর্ণসংখ্যা _cols, পূর্ণসংখ্যা _type, UMatUsageFlags _usageFlags)
{
    পূর্ণসংখ্যা sz[] = {_rows, _cols};
    create(2, sz, _type, _usageFlags);
}

শূন্য UMat::create(আকার_2 _sz, পূর্ণসংখ্যা _type, UMatUsageFlags _usageFlags)
{
    create(_sz.height, _sz.width, _type, _usageFlags);
}

শূন্য UMat::create_Same_আকার(প্রবেশ_Array arr, পূর্ণসংখ্যা _type, UMatUsageFlags _usageFlags)
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

যৌক্তিক UMat::empty() ধ্রুবক
{
    ফেরত u == 0 || যোগ() == 0;
}

size_t UMat::যোগ() ধ্রুবক
{
    যদি( dims <= 2 )
        ফেরত (size_t)rows * cols;
    size_t p = 1;
    জন্য( পূর্ণসংখ্যা i = 0; i < dims; i++ )
        p *= size[i];
    ফেরত p;
}

MatShape UMat::shape() ধ্রুবক
{
    ফেরত size;
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
    m.allocator = ফাঁকা;
    m.u = ফাঁকা;
    m.offset = 0;
}

UMat& UMat::operator=(UMat&& m)
{
    যদি (এই == &m)
      ফেরত *এই;
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
    m.allocator = ফাঁকা;
    m.u = ফাঁকা;
    m.offset = 0;
    ফেরত *এই;
}


MatAllocator* UMat::getStdAllocator()
{
#ifdef HAVE_OPENCL
    যদি (ocl::useOpenCL())
        ফেরত ocl::getOpenCLAllocator();
#endif
    ফেরত Mat::getDefaultAllocator();
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


শূন্য set_আকার( UMat& m, পূর্ণসংখ্যা _dims, ধ্রুবক পূর্ণসংখ্যা* _sz,
              ধ্রুবক size_t* _steps, যৌক্তিক autoSteps )
{
    CV_Assert( 0 <= _dims && _dims <= CV_MAX_DIM && _dims <= MatShape::MAX_DIMS);

    m.dims = _dims;
    m.size = MatShape(_dims, _sz);
    m.step[std::max(_dims-1, 0)] = CV_ELEM_আকার(m.flags);
    জন্য (পূর্ণসংখ্যা i = _dims-2; i >= 0; i--) {
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
    পূর্ণসংখ্যা d = m.dims;
    যদি( d > 2 )
        m.rows = m.cols = -1;
}


UMat Mat::getUMat(AccessFlag accessFlags, UMatUsageFlags usageFlags) ধ্রুবক
{
    UMat hdr;
    যদি(!তথ্য)
        ফেরত hdr;
    যদি (তথ্য != datastart)
    {
        আকার_2 whole_আকার;
        Point ofs;
        locateROI(whole_আকার, ofs);
        আকার_2 sz(cols, rows);
        যদি (ofs.x != 0 || ofs.y != 0)
        {
            Mat src = *এই;
            পূর্ণসংখ্যা dtop = ofs.y;
            পূর্ণসংখ্যা dbottom = whole_আকার.height - src.rows - ofs.y;
            পূর্ণসংখ্যা dleft = ofs.x;
            পূর্ণসংখ্যা dright = whole_আকার.width - src.cols - ofs.x;
            src.adjustROI(dtop, dbottom, dleft, dright);
            ফেরত src.getUMat(accessFlags, usageFlags)(cv::Rect(ofs.x, ofs.y, sz.width, sz.height));
        }
    }
    CV_Assert(তথ্য == datastart);

    accessFlags |= ACCESS_RW;
    UMat_তথ্য* new_u = ফাঁকা;
    MatStep new_step = step;
    {
        MatAllocator *a = allocator, *a0 = getDefaultAllocator();
        যদি(!a)
            a = a0;
        new_u = a->allocate(dims, size.p, type(), তথ্য, new_step.p, accessFlags, usageFlags);
        new_u->original_UMat_তথ্য = u;
    }
    যৌক্তিক allocated = মিথ্যা;
    চেষ্টা
    {
        allocated = UMat::getStdAllocator()->allocate(new_u, accessFlags, usageFlags);
    }
    ধরো (ধ্রুবক cv::Exception& e)
    {
        fprintf(stderr, "Exception: %s\n", e.what());
    }
    যদি (!allocated)
    {
        allocated = getDefaultAllocator()->allocate(new_u, accessFlags, usageFlags);
        CV_Assert(allocated);
    }
    যদি (u != ফাঁকা)
    {
#ifdef HAVE_OPENCL
        যদি (ocl::useOpenCL() && new_u->currAllocator == ocl::getOpenCLAllocator())
        {
            CV_Assert(new_u->অস্থায়ী_UMat());
        }
#endif
        CV_XADD(&(u->refcount), 1);
        CV_XADD(&(u->urefcount), 1);
    }
    চেষ্টা
    {
        hdr.flags = flags;
        hdr.usageFlags = usageFlags;
        set_আকার(hdr, dims, size.p, new_step.p);
        finalizeHdr(hdr);
        hdr.u = new_u;
        hdr.offset = 0; //data - datastart;
        hdr.addref();
        ফেরত hdr;
    }
    ধরো(...)
    {
        যদি (u != ফাঁকা)
        {
            CV_XADD(&(u->refcount), -1);
            CV_XADD(&(u->urefcount), -1);
        }
        new_u->currAllocator->deallocate(new_u);
        ছোড়ো;
    }

}

শূন্য UMat::create(পূর্ণসংখ্যা d0, ধ্রুবক পূর্ণসংখ্যা* _sizes, পূর্ণসংখ্যা _type, UMatUsageFlags _usageFlags)
{
    পূর্ণসংখ্যা sz1 = 1, d = d0;
    পূর্ণসংখ্যা i;
    যদি (d == 0) {
        d = 1;
        _sizes = (ধ্রুবক পূর্ণসংখ্যা*)&sz1;
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
        জন্য( i = 0; i < d; i++ )
            যদি( size[i] != _sizes[i] )
                বিরতি;
        যদি( i == d )
            ফেরত;
    }

    পূর্ণসংখ্যা _sizes_backup[CV_MAX_DIM]; // #5991
    যদি (_sizes == (এই->size.p))
    {
        জন্য(i = 0; i < d; i++ )
            _sizes_backup[i] = _sizes[i];
        _sizes = _sizes_backup;
    }

    release();
    usageFlags = _usageFlags;
    যদি( d == 0 )
        ফেরত;
    flags = (_type & CV_MAT_TYPE_MASK) | MAGIC_VAL;
    set_আকার(*এই, d, _sizes, 0, সত্য);
    offset = 0;

    যদি( যোগ() > 0 )
    {
        MatAllocator *a = allocator, *a0 = getStdAllocator();
        যদি (!a)
        {
            a = a0;
            a0 = Mat::getDefaultAllocator();
        }
        চেষ্টা
        {
            u = a->allocate(dims, size.p, _type, 0, step.p, ACCESS_RW /* ignored */, usageFlags);
            CV_Assert(u != 0);
        }
        ধরো(...)
        {
            যদি(a != a0)
                u = a0->allocate(dims, size.p, _type, 0, step.p, ACCESS_RW /* ignored */, usageFlags);
            CV_Assert(u != 0);
        }
        CV_Assert( step[dims-1] == (size_t)CV_ELEM_আকার(flags) );
    }

    finalizeHdr(*এই);
    addref();
    size.dims = dims = d0;
}

শূন্য UMat::create(ধ্রুবক std::vector<পূর্ণসংখ্যা>& _sizes, পূর্ণসংখ্যা _type, UMatUsageFlags _usageFlags)
{
    create((পূর্ণসংখ্যা)_sizes.size(), _sizes.তথ্য(), _type, _usageFlags);
}

শূন্য UMat::create(ধ্রুবক MatShape& _shape, পূর্ণসংখ্যা _type, UMatUsageFlags _usageFlags)
{
    যদি (_shape.dims < 0) {
        release();
    } অন্যথা {
        create(_shape.dims, _shape.p, _type, _usageFlags);
        size.layout = _shape.layout;
        size.C = _shape.C;
    }
}

শূন্য UMat::fit(পূর্ণসংখ্যা _dims, ধ্রুবক পূর্ণসংখ্যা* _sizes, পূর্ণসংখ্যা _type, UMatUsageFlags _usageFlags)
{
    যদি (_usageFlags == cv::USAGE_DEFAULT)
        _usageFlags = usageFlags;
    size_t old_যোগ_Bytes = u ? u->size : 0;
    size_t esz = CV_ELEM_আকার(_type), new_যোগ = _dims >= 0;
    জন্য (পূর্ণসংখ্যা i = 0; i < _dims; i++)
        new_যোগ *= _sizes[i];
    size_t new_যোগ_Bytes = new_যোগ*esz;
    যদি (new_যোগ_Bytes > 0 &&
        (!isContinuous() ||
         new_যোগ_Bytes > old_যোগ_Bytes ||
         offset != 0 ||
         _usageFlags != usageFlags)) {
        create(_dims, _sizes, _type, _usageFlags);
    } অন্যথা {
        flags = (flags & ~Mat::TYPE_MASK) | CV_MAT_TYPE(_type);
        পূর্ণসংখ্যা dummy_আকার = 0;
        set_আকার(*এই, (_dims >= 0 ? _dims : 1), (_dims >= 0 ? _sizes : &dummy_আকার), nullptr, সত্য);
        finalizeHdr(*এই);
    }
}

শূন্য UMat::fit(ধ্রুবক std::vector<পূর্ণসংখ্যা>& _shape, পূর্ণসংখ্যা _type, UMatUsageFlags _usageFlags)
{
    fit((পূর্ণসংখ্যা)_shape.size(), _shape.তথ্য(), _type, _usageFlags);
}

শূন্য UMat::fit(ধ্রুবক MatShape& _shape, পূর্ণসংখ্যা _type, UMatUsageFlags _usageFlags)
{
    fit(_shape.dims, _shape.p, _type, _usageFlags);
    size.layout = _shape.layout;
    size.C = _shape.C;
}

শূন্য UMat::fit(পূর্ণসংখ্যা _rows, পূর্ণসংখ্যা _cols, পূর্ণসংখ্যা _type, UMatUsageFlags _usageFlags)
{
    _type &= TYPE_MASK;
    পূর্ণসংখ্যা sz[] = {_rows, _cols};
    fit(2, sz, _type, _usageFlags);
}

শূন্য UMat::fit(আকার_2 _sz, পূর্ণসংখ্যা _type, UMatUsageFlags _usageFlags)
{
    fit(_sz.height, _sz.width, _type, _usageFlags);
}

শূন্য UMat::fit_Same_আকার(প্রবেশ_Array m, পূর্ণসংখ্যা _type, UMatUsageFlags _usageFlags)
{
    fit(m.shape(), _type, _usageFlags);
}

শূন্য UMat::copy_আকার(ধ্রুবক UMat& m)
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
    UMat_তথ্য* u_ = u;
    u = ফাঁকা;
    u_->currAllocator->deallocate(u_);
}


UMat::UMat(ধ্রুবক UMat& m, ধ্রুবক Range& _rowRange, ধ্রুবক Range& _colRange)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(USAGE_DEFAULT), u(0), offset(0)
{
    CV_Assert( m.dims >= 2 && m.dims <= MatShape::MAX_DIMS );
    যদি( m.dims > 2 )
    {
        Range rs[MatShape::MAX_DIMS];
        rs[0] = _rowRange;
        rs[1] = _colRange;
        জন্য( পূর্ণসংখ্যা i = 2; i < m.dims; i++ )
            rs[i] = Range::all();
        *এই = m(rs);
        ফেরত;
    }

    *এই = m;
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
        offset += _colRange.start*elem_আকার();
        flags |= SUBMATRIX_FLAG;
    }

    updateContinuityFlag();

    যদি( rows <= 0 || cols <= 0 )
    {
        release();
        rows = cols = 0;
    }
}


UMat::UMat(ধ্রুবক UMat& m, ধ্রুবক Rect& roi)
    : flags(m.flags), dims(2), rows(roi.height), cols(roi.width),
    allocator(m.allocator), usageFlags(m.usageFlags), u(m.u),
    offset(m.offset + roi.y*m.step[0]), size(2)
{
    CV_Assert( m.dims <= 2 );

    size_t esz = CV_ELEM_আকার(flags);
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


UMat::UMat(ধ্রুবক UMat& m, ধ্রুবক Range* ranges)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(USAGE_DEFAULT), u(0), offset(0)
{
    পূর্ণসংখ্যা i, d = m.dims;

    CV_Assert(ranges);
    জন্য( i = 0; i < d; i++ )
    {
        Range r = ranges[i];
        CV_Assert( r == Range::all() || (0 <= r.start && r.start < r.end && r.end <= m.size[i]) );
    }
    *এই = m;
    জন্য( i = 0; i < d; i++ )
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

UMat::UMat(ধ্রুবক UMat& m, ধ্রুবক std::vector<Range>& ranges)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(USAGE_DEFAULT), u(0), offset(0)
{
    পূর্ণসংখ্যা i, d = m.dims;

    CV_Assert((পূর্ণসংখ্যা)ranges.size() == d);
    জন্য (i = 0; i < d; i++)
    {
        Range r = ranges[i];
        CV_Assert(r == Range::all() || (0 <= r.start && r.start < r.end && r.end <= m.size[i]));
    }
    *এই = m;
    জন্য (i = 0; i < d; i++)
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

UMat UMat::diag(পূর্ণসংখ্যা d) ধ্রুবক
{
    CV_Assert( dims <= 2 );
    UMat m = *এই;
    size_t esz = elem_আকার();
    পূর্ণসংখ্যা len;

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

    যদি( size() != আকার_2(1,1) )
        m.flags |= SUBMATRIX_FLAG;

    ফেরত m;
}

শূন্য UMat::locateROI( আকার_2& whole_আকার, Point& ofs ) ধ্রুবক
{
    CV_Assert( dims <= 2 && step[0] > 0 );
    size_t esz = elem_আকার(), minstep;
    ptrdiff_t delta1 = (ptrdiff_t)offset, delta2 = (ptrdiff_t)u->size;

    যদি( delta1 == 0 )
        ofs.x = ofs.y = 0;
    অন্যথা
    {
        ofs.y = (পূর্ণসংখ্যা)(delta1/step[0]);
        ofs.x = (পূর্ণসংখ্যা)((delta1 - step[0]*ofs.y)/esz);
        CV_DbgAssert( offset == (size_t)(ofs.y*step[0] + ofs.x*esz) );
    }
    minstep = (ofs.x + cols)*esz;
    whole_আকার.height = (পূর্ণসংখ্যা)((delta2 - minstep)/step[0] + 1);
    whole_আকার.height = std::max(whole_আকার.height, ofs.y + rows);
    whole_আকার.width = (পূর্ণসংখ্যা)((delta2 - step*(whole_আকার.height-1))/esz);
    whole_আকার.width = std::max(whole_আকার.width, ofs.x + cols);
}


UMat& UMat::adjustROI( পূর্ণসংখ্যা dtop, পূর্ণসংখ্যা dbottom, পূর্ণসংখ্যা dleft, পূর্ণসংখ্যা dright )
{
    CV_Assert( dims == 2 && step[0] > 0 );
    আকার_2 whole_আকার; Point ofs;
    size_t esz = elem_আকার();
    locateROI( whole_আকার, ofs );
    পূর্ণসংখ্যা row1 = std::min(std::max(ofs.y - dtop, 0), whole_আকার.height), row2 = std::max(0, std::min(ofs.y + rows + dbottom, whole_আকার.height));
    পূর্ণসংখ্যা col1 = std::min(std::max(ofs.x - dleft, 0), whole_আকার.width), col2 = std::max(0, std::min(ofs.x + cols + dright, whole_আকার.width));
    যদি(row1 > row2)
        std::swap(row1, row2);
    যদি(col1 > col2)
        std::swap(col1, col2);

    offset += (row1 - ofs.y)*step + (col1 - ofs.x)*esz;
    rows = row2 - row1; cols = col2 - col1;
    size.p[0] = rows; size.p[1] = cols;
    updateContinuityFlag();
    ফেরত *এই;
}


UMat UMat::reshape(পূর্ণসংখ্যা new_cn, পূর্ণসংখ্যা new_rows) ধ্রুবক
{
    পূর্ণসংখ্যা cn = channels();
    UMat hdr = *এই;

    যদি( dims > 2 && new_rows == 0 && new_cn != 0 && size[dims-1]*cn % new_cn == 0 )
    {
        hdr.flags = (hdr.flags & ~CV_MAT_CN_MASK) | ((new_cn-1) << CV_CN_SHIFT);
        hdr.step[dims-1] = CV_ELEM_আকার(hdr.flags);
        hdr.size[dims-1] = hdr.size[dims-1]*cn / new_cn;
        ফেরত hdr;
    }

    CV_Assert( dims <= 2 );

    যদি( new_cn == 0 )
        new_cn = cn;

    পূর্ণসংখ্যা যোগ_width = cols * cn;

    যদি( (new_cn > যোগ_width || যোগ_width % new_cn != 0) && new_rows == 0 )
        new_rows = rows * যোগ_width / new_cn;

    যদি( new_rows != 0 && new_rows != rows )
    {
        পূর্ণসংখ্যা যোগ_আকার_2 = যোগ_width * rows;
        যদি( !isContinuous() )
            CV_Error( cv::Error::BadStep,
            "The matrix is not continuous, thus its number of rows can not be changed" );

        যোগ_width = যোগ_আকার_2 / new_rows;

        যদি( যোগ_width * new_rows != যোগ_আকার_2 )
            CV_Error( cv::Error::StsBadArg, "The total number of matrix elements "
                                    "is not divisible by the new number of rows" );

        hdr.size[0] = hdr.rows = new_rows;
        hdr.step[0] = যোগ_width * elemSize1();
    } অন্যথা {
        hdr.size[0] = hdr.rows = rows;
        যদি (dims <= 1)
            hdr.step[0] = cols * CV_ELEM_আকার(flags);
    }

    পূর্ণসংখ্যা new_width = যোগ_width / new_cn;

    যদি( new_width * new_cn != যোগ_width )
        CV_Error( cv::Error::Bad_অঙ্ক_Channels,
        "The total width is not divisible by the new number of channels" );

    hdr.size.dims = hdr.dims = 2;
    hdr.size[1] = hdr.cols = new_width;
    hdr.flags = (hdr.flags & ~CV_MAT_CN_MASK) | ((new_cn-1) << CV_CN_SHIFT);
    hdr.step[1] = CV_ELEM_আকার(hdr.flags);
    ফেরত hdr;
}

#ifdef HAVE_OPENCL
নামাকাশ {
স্থির যৌক্তিক ocl_setDiag(ধ্রুবক UMat& d, UMat& m, পূর্ণসংখ্যা len)
{
    পূর্ণসংখ্যা cn = d.channels();
    পূর্ণসংখ্যা depth = d.depth();

    যদি (depth == CV_64F && !ocl::Device::getDefault().doubleFPConfig())
        ফেরত মিথ্যা;

    String opts = format("-D SET_DIAG -D T1=%s -D cn=%d -D IS_ROW_VECTOR=%d",
                         ocl::memopTypeToStr(depth),
                         cn,
                         (d.rows == 1) ? 1 : 0);

    ocl::Kernel k("setDiag", ocl::core::copyset_oclsrc, opts);
    যদি (k.empty())
        ফেরত মিথ্যা;

    k.args(ocl::KernelArg::WriteOnly(m),
           ocl::KernelArg::Read_Only_No_আকার(d),
           len);

    size_t globalsize[2] = { (size_t)len, (size_t)len };
    ফেরত k.run(2, globalsize, ফাঁকা, মিথ্যা);
}
}
#endif

UMat UMat::diag(ধ্রুবক UMat& d, UMatUsageFlags usageFlags)
{
    CV_INSTRUMENT_REGION();

    CV_Assert(d.cols == 1 || d.rows == 1);
    পূর্ণসংখ্যা len = d.rows + d.cols - 1;

#ifdef HAVE_OPENCL
    যদি (ocl::useOpenCL())
    {
        UMat m(len, len, d.type(), usageFlags);
        যদি (ocl_setDiag(d, m, len))
        {
            CV_IMPL_ADD(CV_IMPL_OCL);
            ফেরত m;
        }
    }
#endif

    UMat m(len, len, d.type(), Scalar(0), usageFlags);
    UMat md = m.diag();
    যদি( d.cols == 1 )
        d.copyTo(md);
    অন্যথা
        transpose(d, md);
    ফেরত m;
}

পূর্ণসংখ্যা UMat::checkVector(পূর্ণসংখ্যা _elemChannels, পূর্ণসংখ্যা _depth, যৌক্তিক _requireContinuous) ধ্রুবক
{
    ফেরত (depth() == _depth || _depth <= 0) &&
        (isContinuous() || !_requireContinuous) &&
        ((dims <= 2 && (((rows == 1 || cols == 1) && channels() == _elemChannels) ||
                        (cols == _elemChannels && channels() == 1))) ||
        (dims == 3 && channels() == 1 && size.p[2] == _elemChannels && (size.p[0] == 1 || size.p[1] == 1) &&
         (isContinuous() || step.p[1] == step.p[2]*size.p[2])))
    ? (পূর্ণসংখ্যা)(যোগ()*channels()/_elemChannels) : -1;
}

UMat UMat::reshape(পূর্ণসংখ্যা _cn, পূর্ণসংখ্যা _newndims, ধ্রুবক পূর্ণসংখ্যা* _newsz) ধ্রুবক
{
    যদি(_newndims == dims)
    {
        যদি(_newsz == 0)
            ফেরত reshape(_cn);
        যদি(_newndims == 2)
            ফেরত reshape(_cn, _newsz[0]);
    }

    যদি (isContinuous())
    {
        CV_Assert(_cn >= 0 && _newndims >= 0 && _newndims <= CV_MAX_DIM && (_newndims == 0 || _newsz != 0));

        যদি (_cn == 0)
            _cn = এই->channels();
        অন্যথা
            CV_Assert(_cn <= CV_CN_MAX);

        size_t যোগ_elem1_ref = এই->যোগ() * এই->channels();
        size_t যোগ_elem1 = _cn;

        Auto_বাফার<পূর্ণসংখ্যা, 4> newsz_buf( (size_t)std::max(_newndims, 1) );

        পূর্ণসংখ্যা m1_idx = -1;

        জন্য (পূর্ণসংখ্যা i = 0; i < _newndims; i++)
        {
            যদি (_newsz[i] >= 0) {
                যদি (_newsz[i] == 0 && i < dims)
                    newsz_buf[i] = size.p[i];
                অন্যথা
                    newsz_buf[i] = _newsz[i];
                যোগ_elem1 *= (size_t)newsz_buf[i];
            } অন্যথা {
                যদি (m1_idx >= 0)
                    CV_Error(cv::Error::Sts_Bad_আকার, "More than one '-1' occured in the new shape");
                m1_idx = i;
            }
        }

        যদি (m1_idx >= 0) {
            যদি (যোগ_elem1 == 0) {
                CV_Assert(যোগ_elem1_ref == 0);
                যোগ_elem1 = 1;
            }
            CV_Assert(যোগ_elem1_ref % যোগ_elem1 == 0);
            newsz_buf[m1_idx] = (পূর্ণসংখ্যা)(যোগ_elem1_ref / যোগ_elem1);
        } অন্যথা যদি (যোগ_elem1 != যোগ_elem1_ref) {
            CV_Error(cv::Error::StsUnmatchedSizes, "Requested and source matrices have different count of elements");
        }

        UMat hdr = *এই;
        hdr.flags = (hdr.flags & ~CV_MAT_CN_MASK) | ((_cn-1) << CV_CN_SHIFT);
        set_আকার(hdr, _newndims, newsz_buf.তথ্য(), ফাঁকা, সত্য);

        ফেরত hdr;
    }

    CV_Error(cv::Error::StsNotImplemented, "Reshaping of n-dimensional non-continuous matrices is not supported yet");
}

UMat UMat::reshape(পূর্ণসংখ্যা _cn, ধ্রুবক MatShape& _newshape) ধ্রুবক
{
    যদি (_newshape.dims < 0) {
        পূর্ণসংখ্যা newshape[] = {0};
        ফেরত reshape(_cn, 1, newshape);
    }
    ফেরত reshape(_cn, _newshape.dims, _newshape.p);
}

Mat UMat::getMat(AccessFlag accessFlags) ধ্রুবক
{
    যদি(!u)
        ফেরত Mat();
    // TODO Support ACCESS_READ (ACCESS_WRITE) without unnecessary data transfers
    accessFlags |= ACCESS_RW;
    UMat_তথ্য_Auto_Lock autolock(u);
    চেষ্টা
    {
        যদি(CV_XADD(&u->refcount, 1) == 0)
            u->currAllocator->map(u, accessFlags);
        যদি (u->তথ্য != 0)
        {
            Mat hdr(dims, size.p, type(), u->তথ্য + offset, step.p);
            hdr.flags = flags;
            hdr.u = u;
            hdr.datastart = u->তথ্য;
            hdr.তথ্য = u->তথ্য + offset;
            hdr.datalimit = hdr.dataend = u->তথ্য + u->size;
            ফেরত hdr;
        }
    }
    ধরো(...)
    {
        CV_XADD(&u->refcount, -1);
        ছোড়ো;
    }
    CV_XADD(&u->refcount, -1);
    CV_Assert(u->তথ্য != 0 && "Error mapping of UMat to host memory.");
    ফেরত Mat();
}

শূন্য* UMat::handle(AccessFlag accessFlags) ধ্রুবক
{
    যদি( !u )
        ফেরত 0;

    CV_Assert(u->refcount == 0);
    CV_Assert(!u->deviceCopyObsolete() || u->copyOnMap());
    যদি (u->deviceCopyObsolete())
    {
        u->currAllocator->unmap(u);
    }

    যদি (!!(accessFlags & ACCESS_WRITE))
        u->markHostCopyObsolete(সত্য);

    ফেরত u->handle;
}

শূন্য UMat::ndoffset(size_t* ofs) ধ্রুবক
{
    // offset = step[0]*ofs[0] + step[1]*ofs[1] + step[2]*ofs[2] + ...;
    size_t val = offset;
    জন্য( পূর্ণসংখ্যা i = 0; i < dims; i++ )
    {
        size_t s = step.p[i];
        ofs[i] = val / s;
        val -= ofs[i]*s;
    }
}

শূন্য UMat::copyTo(OutputArray _dst) ধ্রুবক
{
    CV_INSTRUMENT_REGION();

#ifdef HAVE_CUDA
    যদি (_dst.isGpuMat())
    {
        _dst.getGpuMat().upload(*এই);
        ফেরত;
    }
#endif

    পূর্ণসংখ্যা stype = type();
    পূর্ণসংখ্যা dtype = _dst.type();
    যদি( _dst.fixedType() && dtype != stype )
    {
        CV_Assert( CV_MAT_CN(stype) == CV_MAT_CN(dtype) );
        convertTo( _dst, dtype );
        ফেরত;
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
        ফেরত;
    }

    _dst.create( dims, size.p, stype );
    যদি (empty())
        ফেরত;

    size_t sz[CV_MAX_DIM] = {1}, srcofs[CV_MAX_DIM]={0}, dstofs[CV_MAX_DIM]={0};
    size_t esz = CV_ELEM_আকার(stype);
    পূর্ণসংখ্যা i, d = std::max(dims, 1);
    জন্য( i = 0; i < d; i++ )
        sz[i] = size.p[i];
    sz[d-1] *= esz;
    ndoffset(srcofs);
    srcofs[d-1] *= esz;

    যদি( _dst.isUMat() )
    {
        UMat dst = _dst.getUMat();
        CV_Assert(dst.u);
        যদি( u == dst.u && dst.offset == offset )
            ফেরত;

        যদি (u->currAllocator == dst.u->currAllocator)
        {
            dst.ndoffset(dstofs);
            dstofs[dims-1] *= esz;
            u->currAllocator->copy(u, dst.u, d, sz, srcofs, step.p, dstofs, dst.step.p, মিথ্যা);
            ফেরত;
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
            ফেরত;
        }
    }

    Mat dst = _dst.getMat();
    u->currAllocator->download(u, dst.ptr(), d, sz, srcofs, step.p, dst.step.p);
}

শূন্য UMat::copyTo(OutputArray _dst, প্রবেশ_Array _mask) ধ্রুবক
{
    CV_INSTRUMENT_REGION();

    যদি( _mask.empty() )
    {
        copyTo(_dst);
        ফেরত;
    }
#ifdef HAVE_OPENCL
    পূর্ণসংখ্যা cn = channels(), mtype = _mask.type(), mdepth = CV_MAT_DEPTH(mtype), mcn = CV_MAT_CN(mtype);
    CV_Assert( (mdepth == CV_8U || mdepth == CV_8S || mdepth == CV_Bool) && (mcn == 1 || mcn == cn) );

    যদি (ocl::useOpenCL() && _dst.isUMat() && dims <= 2)
    {
        UMat_তথ্য * prevu = _dst.getUMat().u;
        _dst.create( size, type() );

        UMat dst = _dst.getUMat();

        যৌক্তিক haveDstUninit = মিথ্যা;
        যদি( prevu != dst.u ) // do not leave dst uninitialized
            haveDstUninit = সত্য;

        String opts = format("-D COPY_TO_MASK -D T1=%s -D scn=%d -D mcn=%d%s",
                             ocl::memopTypeToStr(depth()), cn, mcn,
                             haveDstUninit ? " -D HAVE_DST_UNINIT" : "");

        ocl::Kernel k("copyToMask", ocl::core::copyset_oclsrc, opts);
        যদি (!k.empty())
        {
            k.args(ocl::KernelArg::Read_Only_No_আকার(*এই),
                   ocl::KernelArg::Read_Only_No_আকার(_mask.getUMat()),
                   haveDstUninit ? ocl::KernelArg::WriteOnly(dst) :
                                   ocl::KernelArg::ReadWrite(dst));

            size_t globalsize[2] = { (size_t)cols, (size_t)rows };
            যদি (k.run(2, globalsize, ফাঁকা, মিথ্যা))
            {
                CV_IMPL_ADD(CV_IMPL_OCL);
                ফেরত;
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

UMat& UMat::setTo(প্রবেশ_Array মান, প্রবেশ_Array _mask)
{
    CV_INSTRUMENT_REGION();

    যৌক্তিক haveMask = !_mask.empty();
#ifdef HAVE_OPENCL
    পূর্ণসংখ্যা tp = type(), cn = CV_MAT_CN(tp), d = CV_MAT_DEPTH(tp);

    যদি( dims <= 2 && cn <= 4 && CV_MAT_DEPTH(tp) < CV_64F && ocl::useOpenCL() )
    {
        Mat value = মান.getMat();
        CV_Assert( checkScalar(value, type(), মান.kind(), প্রবেশ_Array_2::UMAT) );
        পূর্ণসংখ্যা kercn = haveMask || cn == 3 ? cn : std::max(cn, ocl::predictOptimalVectorWidth(*এই)),
                kertp = CV_MAKE_TYPE(d, kercn);

        দ্বিগুণ buf[16] = { 0, 0, 0, 0, 0, 0, 0, 0,
                           0, 0, 0, 0, 0, 0, 0, 0 };
        convertAndUnrollScalar(value, tp, (uchar *)buf, kercn / cn);

        পূর্ণসংখ্যা scalarcn = kercn == 3 ? 4 : kercn, rowsPerWI = ocl::Device::getDefault().isIntel() ? 4 : 1;
        String opts = format("-D dstT=%s -D rowsPerWI=%d -D dstST=%s -D dstT1=%s -D cn=%d",
                             ocl::memopTypeToStr(kertp), rowsPerWI,
                             ocl::memopTypeToStr(CV_MAKETYPE(d, scalarcn)),
                             ocl::memopTypeToStr(d), kercn);

        ocl::Kernel setK(haveMask ? "setMask" : "set", ocl::core::copyset_oclsrc, opts);
        যদি( !setK.empty() )
        {
            ocl::KernelArg scalararg(ocl::KernelArg::CONSTANT, 0, 0, 0, buf, CV_ELEM_আকার(d) * scalarcn);
            UMat mask;

            যদি( haveMask )
            {
                mask = _mask.getUMat();
                CV_Assert( mask.size() == size() && (mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool) );
                ocl::KernelArg maskarg = ocl::KernelArg::Read_Only_No_আকার(mask),
                        dstarg = ocl::KernelArg::ReadWrite(*এই);
                setK.args(maskarg, dstarg, scalararg);
            }
            অন্যথা
            {
                ocl::KernelArg dstarg = ocl::KernelArg::WriteOnly(*এই, cn, kercn);
                setK.args(dstarg, scalararg);
            }

            size_t globalsize[] = { (size_t)cols * cn / kercn, ((size_t)rows + rowsPerWI - 1) / rowsPerWI };
            যদি( setK.run(2, globalsize, ফাঁকা, মিথ্যা) )
            {
                CV_IMPL_ADD(CV_IMPL_OCL);
                ফেরত *এই;
            }
        }
    }
#endif
    Mat m = getMat(haveMask ? ACCESS_RW : ACCESS_WRITE);
    m.setTo(মান, _mask);
    ফেরত *এই;
}

UMat& UMat::operator = (ধ্রুবক Scalar& s)
{
    setTo(s);
    ফেরত *এই;
}

UMat UMat::t() ধ্রুবক
{
    UMat m;
    transpose(*এই, m);
    ফেরত m;
}

UMat UMat::zeros(পূর্ণসংখ্যা rows, পূর্ণসংখ্যা cols, পূর্ণসংখ্যা type, UMatUsageFlags usageFlags)
{
    ফেরত UMat(rows, cols, type, Scalar::all(0), usageFlags);
}

UMat UMat::zeros(আকার_2 size, পূর্ণসংখ্যা type, UMatUsageFlags usageFlags)
{
    ফেরত UMat(size, type, Scalar::all(0), usageFlags);
}

UMat UMat::zeros(পূর্ণসংখ্যা ndims, ধ্রুবক পূর্ণসংখ্যা* sz, পূর্ণসংখ্যা type, UMatUsageFlags usageFlags)
{
    ফেরত UMat(ndims, sz, type, Scalar::all(0), usageFlags);
}

UMat UMat::zeros(ধ্রুবক MatShape& shape, পূর্ণসংখ্যা type, UMatUsageFlags usageFlags)
{
    ফেরত UMat(shape.dims, shape.p, type, Scalar::all(0), usageFlags);
}

UMat UMat::ones(পূর্ণসংখ্যা rows, পূর্ণসংখ্যা cols, পূর্ণসংখ্যা type, UMatUsageFlags usageFlags)
{
    ফেরত UMat(rows, cols, type, Scalar(1), usageFlags);
}

UMat UMat::ones(আকার_2 size, পূর্ণসংখ্যা type, UMatUsageFlags usageFlags)
{
    ফেরত UMat(size, type, Scalar(1), usageFlags);
}

UMat UMat::ones(পূর্ণসংখ্যা ndims, ধ্রুবক পূর্ণসংখ্যা* sz, পূর্ণসংখ্যা type, UMatUsageFlags usageFlags)
{
    ফেরত UMat(ndims, sz, type, Scalar(1), usageFlags);
}

UMat UMat::ones(ধ্রুবক MatShape& shape, পূর্ণসংখ্যা type, UMatUsageFlags usageFlags)
{
    ফেরত UMat(shape.dims, shape.p, type, Scalar(1), usageFlags);
}

}

/* End of file. */
