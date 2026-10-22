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

#അന്തര്ഭാവ "precomp.hpp"
#അന്തര്ഭാവ "opencl_kernels_core.hpp"
#അന്തര്ഭാവ "umatrix.hpp"

#അന്തര്ഭാവ <opencv2/core/utils/tls.hpp>

///////////////////////////////// UMat implementation ///////////////////////////////

നാമാകാശ cv {

// forward decls, implementation is below in this file
ശൂന്യം setSize(UMat& m, പൂർണ്ണസംഖ്യ _dims, സ്ഥിരം പൂർണ്ണസംഖ്യ* _sz, സ്ഥിരം size_t* _steps,
             തർക്കം autoSteps = അസത്യം);

ശൂന്യം updateContinuityFlag(UMat& m);
ശൂന്യം finalizeHdr(UMat& m);

UMatData::UMatData(സ്ഥിരം MatAllocator* allocator)
{
    prevAllocator = currAllocator = allocator;
    urefcount = refcount = mapcount = 0;
    data = origdata = 0;
    size = 0;
    flags = static_cast<UMatData::MemoryFlag>(0);
    handle = 0;
    userdata = 0;
    allocatorFlags_ = 0;
    originalUMatData = ശൂന്യ;
}

UMatData::~UMatData()
{
    prevAllocator = currAllocator = 0;
    urefcount = refcount = 0;
    CV_Assert(mapcount == 0);
    data = origdata = 0;
    size = 0;
    തർക്കം isAsyncCleanup = !!(flags & UMatData::ASYNC_CLEANUP);
    flags = static_cast<UMatData::MemoryFlag>(0);
    handle = 0;
    userdata = 0;
    allocatorFlags_ = 0;
    എങ്കിൽ (originalUMatData)
    {
        തർക്കം showWarn = അസത്യം;
        UMatData* u = originalUMatData;
        തർക്കം zero_Ref = CV_XADD(&(u->refcount), -1) == 1;
        എങ്കിൽ (zero_Ref)
        {
            // simulate Mat::deallocate
            എങ്കിൽ (u->mapcount != 0)
            {
                (u->currAllocator ? u->currAllocator : /* TODO allocator ? allocator :*/ Mat::getDefaultAllocator())->unmap(u);
            }
            അല്ലെങ്കിൽ
            {
                // we don't do "map", so we can't do "unmap"
            }
        }
        തർക്കം zero_URef = CV_XADD(&(u->urefcount), -1) == 1;
        എങ്കിൽ (zero_Ref && !zero_URef)
            showWarn = സത്യം;
        എങ്കിൽ (zero_Ref && zero_URef) // oops, we need to free resources
        {
            showWarn = !isAsyncCleanup;
            // simulate UMat::deallocate
            u->currAllocator->deallocate(u);
        }
#ifndef NDEBUG
        എങ്കിൽ (showWarn)
        {
            സ്ഥിതം പൂർണ്ണസംഖ്യ warn_message_showed = 0;
            എങ്കിൽ (warn_message_showed++ < 100)
            {
                fflush(stdout);
                fprintf(stderr, "\n! OPENCV warning: getUMat()/getMat() call chain possible problem."
                                "\n!                 Base object is dead, while nested/derived object is still alive or processed."
                                "\n!                 Please check lifetime of UMat/Mat objects!\n");
                fflush(stderr);
            }
        }
#അല്ലെങ്കിൽ
        CV_UNUSED(showWarn);
#endif
        originalUMatData = ശൂന്യ;
    }
}

#ifndef OPENCV_DISABLE_THREAD_SUPPORT

// it should be a prime number for the best hash function
എണ്ണൽ { UMAT_NLOCKS = 31 };
സ്ഥിതം Mutex umatLocks[UMAT_NLOCKS];

സ്ഥിതം size_t getUMatDataLockIndex(സ്ഥിരം UMatData* u)
{
    size_t idx = ((size_t)(ശൂന്യം*)u) % UMAT_NLOCKS;
    തിരികെ idx;
}

ശൂന്യം UMatData::lock()
{
    size_t idx = getUMatDataLockIndex(ഇത്);
    //printf("%d lock(%d)\n", cv::utils::getThreadID(), (int)idx);
    umatLocks[idx].lock();
}

ശൂന്യം UMatData::unlock()
{
    size_t idx = getUMatDataLockIndex(ഇത്);
    //printf("%d unlock(%d)\n", cv::utils::getThreadID(), (int)idx);
    umatLocks[idx].unlock();
}


// Do not allow several lock() calls with different UMatData objects.
ഘടന UMatDataAutoLocker
{
    പൂർണ്ണസംഖ്യ usage_എണ്ണം;
    UMatData* locked_objects[2];
    UMatDataAutoLocker() : usage_എണ്ണം(0) { locked_objects[0] = ശൂന്യ; locked_objects[1] = ശൂന്യ; }

    ശൂന്യം lock(UMatData*& u1)
    {
        തർക്കം locked_1 = (u1 == locked_objects[0] || u1 == locked_objects[1]);
        എങ്കിൽ (locked_1)
        {
            u1 = ശൂന്യ;
            തിരികെ;
        }
        CV_Assert(usage_എണ്ണം == 0);  // UMatDataAutoLock can't be used multiple times from the same thread
        usage_എണ്ണം = 1;
        locked_objects[0] = u1;
        u1->lock();
    }
    ശൂന്യം lock(UMatData*& u1, UMatData*& u2)
    {
        തർക്കം locked_1 = (u1 == locked_objects[0] || u1 == locked_objects[1]);
        തർക്കം locked_2 = (u2 == locked_objects[0] || u2 == locked_objects[1]);
        എങ്കിൽ (locked_1)
            u1 = ശൂന്യ;
        എങ്കിൽ (locked_2)
            u2 = ശൂന്യ;
        എങ്കിൽ (locked_1 && locked_2)
            തിരികെ;
        CV_Assert(usage_എണ്ണം == 0);  // UMatDataAutoLock can't be used multiple times from the same thread
        usage_എണ്ണം = 1;
        locked_objects[0] = u1;
        locked_objects[1] = u2;
        എങ്കിൽ (u1)
            u1->lock();
        എങ്കിൽ (u2)
            u2->lock();
    }
    ശൂന്യം release(UMatData* u1, UMatData* u2)
    {
        എങ്കിൽ (u1 == ശൂന്യ && u2 == ശൂന്യ)
            തിരികെ;
        CV_Assert(usage_എണ്ണം == 1);
        usage_എണ്ണം = 0;
        എങ്കിൽ (u1)
            u1->unlock();
        എങ്കിൽ (u2)
            u2->unlock();
        locked_objects[0] = ശൂന്യ; locked_objects[1] = ശൂന്യ;
    }
};
സ്ഥിതം TLSData<UMatDataAutoLocker>& getUMatDataAutoLockerTLS()
{
    CV_SINGLETON_LAZY_INIT_REF(TLSData<UMatDataAutoLocker>, പുതിയ TLSData<UMatDataAutoLocker>());
}
സ്ഥിതം UMatDataAutoLocker& getUMatDataAutoLocker() { തിരികെ getUMatDataAutoLockerTLS().getRef(); }


UMatDataAutoLock::UMatDataAutoLock(UMatData* u) : u1(u), u2(ശൂന്യ)
{
    getUMatDataAutoLocker().lock(u1);
}
UMatDataAutoLock::UMatDataAutoLock(UMatData* u1_, UMatData* u2_) : u1(u1_), u2(u2_)
{
    എങ്കിൽ (getUMatDataLockIndex(u1) > getUMatDataLockIndex(u2))
    {
        std::swap(u1, u2);
    }
    getUMatDataAutoLocker().lock(u1, u2);
}
UMatDataAutoLock::~UMatDataAutoLock()
{
    getUMatDataAutoLocker().release(u1, u2);
}

#അല്ലെങ്കിൽ

ശൂന്യം UMatData::lock()
{
    // nothing in OPENCV_DISABLE_THREAD_SUPPORT mode
}

ശൂന്യം UMatData::unlock()
{
    // nothing in OPENCV_DISABLE_THREAD_SUPPORT mode
}

UMatDataAutoLock::UMatDataAutoLock(UMatData* u) : u1(u), u2(ശൂന്യ)
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

UMat::UMat(പൂർണ്ണസംഖ്യ _rows, പൂർണ്ണസംഖ്യ _cols, പൂർണ്ണസംഖ്യ _type, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_rows, _cols, _type);
}

UMat::UMat(പൂർണ്ണസംഖ്യ _rows, പൂർണ്ണസംഖ്യ _cols, പൂർണ്ണസംഖ്യ _type, സ്ഥിരം Scalar& _s, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_rows, _cols, _type);
    *ഇത് = _s;
}

UMat::UMat(Size _sz, പൂർണ്ണസംഖ്യ _type, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create( _sz.height, _sz.width, _type );
}

UMat::UMat(Size _sz, പൂർണ്ണസംഖ്യ _type, സ്ഥിരം Scalar& _s, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_sz.height, _sz.width, _type);
    *ഇത് = _s;
}

UMat::UMat(പൂർണ്ണസംഖ്യ _dims, സ്ഥിരം പൂർണ്ണസംഖ്യ* _sz, പൂർണ്ണസംഖ്യ _type, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_dims, _sz, _type);
}

UMat::UMat(പൂർണ്ണസംഖ്യ _dims, സ്ഥിരം പൂർണ്ണസംഖ്യ* _sz, പൂർണ്ണസംഖ്യ _type, സ്ഥിരം Scalar& _s, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_dims, _sz, _type);
    *ഇത് = _s;
}

UMat::UMat(സ്ഥിരം MatShape& _shape, പൂർണ്ണസംഖ്യ _type, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_shape.dims, _shape.p, _type);
}

UMat::UMat(സ്ഥിരം MatShape& _shape, പൂർണ്ണസംഖ്യ _type, സ്ഥിരം Scalar& _s, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_shape.dims, _shape.p, _type);
    *ഇത് = _s;
}

UMat::UMat(സ്ഥിരം UMat& m)
: flags(m.flags), dims(m.dims), rows(m.rows), cols(m.cols), allocator(m.allocator),
  usageFlags(m.usageFlags), u(m.u), offset(m.offset), size(m.size), step(m.step)
{
    addref();
}

UMat& UMat::operator=(സ്ഥിരം UMat& m)
{
    എങ്കിൽ( ഇത് != &m )
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
    തിരികെ *ഇത്;
}

UMat UMat::clone() സ്ഥിരം
{
    UMat m;
    copyTo(m);
    തിരികെ m;
}

ശൂന്യം UMat::assignTo(UMat& m, പൂർണ്ണസംഖ്യ _type) സ്ഥിരം
{
    എങ്കിൽ( _type < 0 )
        m = *ഇത്;
    അല്ലെങ്കിൽ
        convertTo(m, _type);
}

ശൂന്യം UMat::create(പൂർണ്ണസംഖ്യ _rows, പൂർണ്ണസംഖ്യ _cols, പൂർണ്ണസംഖ്യ _type, UMatUsageFlags _usageFlags)
{
    പൂർണ്ണസംഖ്യ sz[] = {_rows, _cols};
    create(2, sz, _type, _usageFlags);
}

ശൂന്യം UMat::create(Size _sz, പൂർണ്ണസംഖ്യ _type, UMatUsageFlags _usageFlags)
{
    create(_sz.height, _sz.width, _type, _usageFlags);
}

ശൂന്യം UMat::createSameSize(നിവേശം_Array arr, പൂർണ്ണസംഖ്യ _type, UMatUsageFlags _usageFlags)
{
    create(arr.shape(), _type, _usageFlags);
}

ശൂന്യം UMat::addref()
{
    എങ്കിൽ( u )
        CV_XADD(&(u->urefcount), 1);
}

ശൂന്യം UMat::release()
{
    എങ്കിൽ( u && CV_XADD(&(u->urefcount), -1) == 1 )
        deallocate();
    u = 0;
    size.clear();
    dims = cols = rows = 0;
}

തർക്കം UMat::empty() സ്ഥിരം
{
    തിരികെ u == 0 || total() == 0;
}

size_t UMat::total() സ്ഥിരം
{
    എങ്കിൽ( dims <= 2 )
        തിരികെ (size_t)rows * cols;
    size_t p = 1;
    വേണ്ടി( പൂർണ്ണസംഖ്യ i = 0; i < dims; i++ )
        p *= size[i];
    തിരികെ p;
}

MatShape UMat::shape() സ്ഥിരം
{
    തിരികെ size;
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
    m.allocator = ശൂന്യ;
    m.u = ശൂന്യ;
    m.offset = 0;
}

UMat& UMat::operator=(UMat&& m)
{
    എങ്കിൽ (ഇത് == &m)
      തിരികെ *ഇത്;
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
    m.allocator = ശൂന്യ;
    m.u = ശൂന്യ;
    m.offset = 0;
    തിരികെ *ഇത്;
}


MatAllocator* UMat::getStdAllocator()
{
#ifdef HAVE_OPENCL
    എങ്കിൽ (ocl::useOpenCL())
        തിരികെ ocl::getOpenCLAllocator();
#endif
    തിരികെ Mat::getDefaultAllocator();
}

ശൂന്യം swap( UMat& a, UMat& b )
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


ശൂന്യം setSize( UMat& m, പൂർണ്ണസംഖ്യ _dims, സ്ഥിരം പൂർണ്ണസംഖ്യ* _sz,
              സ്ഥിരം size_t* _steps, തർക്കം autoSteps )
{
    CV_Assert( 0 <= _dims && _dims <= CV_MAX_DIM && _dims <= MatShape::MAX_DIMS);

    m.dims = _dims;
    m.size = MatShape(_dims, _sz);
    m.step[std::max(_dims-1, 0)] = CV_ELEM_SIZE(m.flags);
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = _dims-2; i >= 0; i--) {
        size_t autostep = m.size[i+1]*m.step[i+1];
        എങ്കിൽ (_steps) {
            m.step[i] = _steps[i];
            //CV_Assert(m.step[i] >= autostep);
        } അല്ലെങ്കിൽ എങ്കിൽ (autoSteps) {
            m.step[i] = autostep;
        } അല്ലെങ്കിൽ {
            m.step[i] = 0;
        }
    }

    എങ്കിൽ( _dims <= 2 )
    {
        m.cols = _dims == 0 ? 1 : _sz ? _sz[_dims > 1] : 0;
        m.rows = _dims < 2 ? 1 : _sz ? _sz[0] : 0;
    } അല്ലെങ്കിൽ {
        m.cols = m.rows = -1;
    }
}


ശൂന്യം UMat::updateContinuityFlag()
{
    flags = cv::updateContinuityFlag(flags, dims, size.p, step.p);
}


ശൂന്യം finalizeHdr(UMat& m)
{
    m.updateContinuityFlag();
    പൂർണ്ണസംഖ്യ d = m.dims;
    എങ്കിൽ( d > 2 )
        m.rows = m.cols = -1;
}


UMat Mat::getUMat(AccessFlag accessFlags, UMatUsageFlags usageFlags) സ്ഥിരം
{
    UMat hdr;
    എങ്കിൽ(!data)
        തിരികെ hdr;
    എങ്കിൽ (data != datastart)
    {
        Size wholeSize;
        Point ofs;
        locateROI(wholeSize, ofs);
        Size sz(cols, rows);
        എങ്കിൽ (ofs.x != 0 || ofs.y != 0)
        {
            Mat src = *ഇത്;
            പൂർണ്ണസംഖ്യ dtop = ofs.y;
            പൂർണ്ണസംഖ്യ dbottom = wholeSize.height - src.rows - ofs.y;
            പൂർണ്ണസംഖ്യ dleft = ofs.x;
            പൂർണ്ണസംഖ്യ dright = wholeSize.width - src.cols - ofs.x;
            src.adjustROI(dtop, dbottom, dleft, dright);
            തിരികെ src.getUMat(accessFlags, usageFlags)(cv::Rect(ofs.x, ofs.y, sz.width, sz.height));
        }
    }
    CV_Assert(data == datastart);

    accessFlags |= ACCESS_RW;
    UMatData* new_u = ശൂന്യ;
    MatStep new_step = step;
    {
        MatAllocator *a = allocator, *a0 = getDefaultAllocator();
        എങ്കിൽ(!a)
            a = a0;
        new_u = a->allocate(dims, size.p, type(), data, new_step.p, accessFlags, usageFlags);
        new_u->originalUMatData = u;
    }
    തർക്കം allocated = അസത്യം;
    ശ്രമിക്കുക
    {
        allocated = UMat::getStdAllocator()->allocate(new_u, accessFlags, usageFlags);
    }
    പിടിക്കുക (സ്ഥിരം cv::Exception& e)
    {
        fprintf(stderr, "Exception: %s\n", e.what());
    }
    എങ്കിൽ (!allocated)
    {
        allocated = getDefaultAllocator()->allocate(new_u, accessFlags, usageFlags);
        CV_Assert(allocated);
    }
    എങ്കിൽ (u != ശൂന്യ)
    {
#ifdef HAVE_OPENCL
        എങ്കിൽ (ocl::useOpenCL() && new_u->currAllocator == ocl::getOpenCLAllocator())
        {
            CV_Assert(new_u->tempUMat());
        }
#endif
        CV_XADD(&(u->refcount), 1);
        CV_XADD(&(u->urefcount), 1);
    }
    ശ്രമിക്കുക
    {
        hdr.flags = flags;
        hdr.usageFlags = usageFlags;
        setSize(hdr, dims, size.p, new_step.p);
        finalizeHdr(hdr);
        hdr.u = new_u;
        hdr.offset = 0; //data - datastart;
        hdr.addref();
        തിരികെ hdr;
    }
    പിടിക്കുക(...)
    {
        എങ്കിൽ (u != ശൂന്യ)
        {
            CV_XADD(&(u->refcount), -1);
            CV_XADD(&(u->urefcount), -1);
        }
        new_u->currAllocator->deallocate(new_u);
        എറിയുക;
    }

}

ശൂന്യം UMat::create(പൂർണ്ണസംഖ്യ d0, സ്ഥിരം പൂർണ്ണസംഖ്യ* _sizes, പൂർണ്ണസംഖ്യ _type, UMatUsageFlags _usageFlags)
{
    പൂർണ്ണസംഖ്യ sz1 = 1, d = d0;
    പൂർണ്ണസംഖ്യ i;
    എങ്കിൽ (d == 0) {
        d = 1;
        _sizes = (സ്ഥിരം പൂർണ്ണസംഖ്യ*)&sz1;
    }
    CV_Assert(0 <= d && d <= CV_MAX_DIM && _sizes);
    _type = CV_MAT_TYPE(_type);

    // if param value is USAGE_DEFAULT by implicit default param value -or- explicit value
    // ...then don't change the existing usageFlags
    // it is not possible to change usage from non-default to USAGE_DEFAULT through create()
    // ...instead must construct UMat()
    എങ്കിൽ (_usageFlags == cv::USAGE_DEFAULT)
    {
        _usageFlags = usageFlags;
    }

    എങ്കിൽ( u && d == dims && _type == type() && _usageFlags == usageFlags )
    {
        വേണ്ടി( i = 0; i < d; i++ )
            എങ്കിൽ( size[i] != _sizes[i] )
                നിർത്തുക;
        എങ്കിൽ( i == d )
            തിരികെ;
    }

    പൂർണ്ണസംഖ്യ _sizes_backup[CV_MAX_DIM]; // #5991
    എങ്കിൽ (_sizes == (ഇത്->size.p))
    {
        വേണ്ടി(i = 0; i < d; i++ )
            _sizes_backup[i] = _sizes[i];
        _sizes = _sizes_backup;
    }

    release();
    usageFlags = _usageFlags;
    എങ്കിൽ( d == 0 )
        തിരികെ;
    flags = (_type & CV_MAT_TYPE_MASK) | MAGIC_VAL;
    setSize(*ഇത്, d, _sizes, 0, സത്യം);
    offset = 0;

    എങ്കിൽ( total() > 0 )
    {
        MatAllocator *a = allocator, *a0 = getStdAllocator();
        എങ്കിൽ (!a)
        {
            a = a0;
            a0 = Mat::getDefaultAllocator();
        }
        ശ്രമിക്കുക
        {
            u = a->allocate(dims, size.p, _type, 0, step.p, ACCESS_RW /* ignored */, usageFlags);
            CV_Assert(u != 0);
        }
        പിടിക്കുക(...)
        {
            എങ്കിൽ(a != a0)
                u = a0->allocate(dims, size.p, _type, 0, step.p, ACCESS_RW /* ignored */, usageFlags);
            CV_Assert(u != 0);
        }
        CV_Assert( step[dims-1] == (size_t)CV_ELEM_SIZE(flags) );
    }

    finalizeHdr(*ഇത്);
    addref();
    size.dims = dims = d0;
}

ശൂന്യം UMat::create(സ്ഥിരം std::vector<പൂർണ്ണസംഖ്യ>& _sizes, പൂർണ്ണസംഖ്യ _type, UMatUsageFlags _usageFlags)
{
    create((പൂർണ്ണസംഖ്യ)_sizes.size(), _sizes.data(), _type, _usageFlags);
}

ശൂന്യം UMat::create(സ്ഥിരം MatShape& _shape, പൂർണ്ണസംഖ്യ _type, UMatUsageFlags _usageFlags)
{
    എങ്കിൽ (_shape.dims < 0) {
        release();
    } അല്ലെങ്കിൽ {
        create(_shape.dims, _shape.p, _type, _usageFlags);
        size.layout = _shape.layout;
        size.C = _shape.C;
    }
}

ശൂന്യം UMat::fit(പൂർണ്ണസംഖ്യ _dims, സ്ഥിരം പൂർണ്ണസംഖ്യ* _sizes, പൂർണ്ണസംഖ്യ _type, UMatUsageFlags _usageFlags)
{
    എങ്കിൽ (_usageFlags == cv::USAGE_DEFAULT)
        _usageFlags = usageFlags;
    size_t oldTotalBytes = u ? u->size : 0;
    size_t esz = CV_ELEM_SIZE(_type), newTotal = _dims >= 0;
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < _dims; i++)
        newTotal *= _sizes[i];
    size_t newTotalBytes = newTotal*esz;
    എങ്കിൽ (newTotalBytes > 0 &&
        (!isContinuous() ||
         newTotalBytes > oldTotalBytes ||
         offset != 0 ||
         _usageFlags != usageFlags)) {
        create(_dims, _sizes, _type, _usageFlags);
    } അല്ലെങ്കിൽ {
        flags = (flags & ~Mat::TYPE_MASK) | CV_MAT_TYPE(_type);
        പൂർണ്ണസംഖ്യ _dummy_size = 0;
        setSize(*ഇത്, (_dims >= 0 ? _dims : 1), (_dims >= 0 ? _sizes : &_dummy_size), nullptr, സത്യം);
        finalizeHdr(*ഇത്);
    }
}

ശൂന്യം UMat::fit(സ്ഥിരം std::vector<പൂർണ്ണസംഖ്യ>& _shape, പൂർണ്ണസംഖ്യ _type, UMatUsageFlags _usageFlags)
{
    fit((പൂർണ്ണസംഖ്യ)_shape.size(), _shape.data(), _type, _usageFlags);
}

ശൂന്യം UMat::fit(സ്ഥിരം MatShape& _shape, പൂർണ്ണസംഖ്യ _type, UMatUsageFlags _usageFlags)
{
    fit(_shape.dims, _shape.p, _type, _usageFlags);
    size.layout = _shape.layout;
    size.C = _shape.C;
}

ശൂന്യം UMat::fit(പൂർണ്ണസംഖ്യ _rows, പൂർണ്ണസംഖ്യ _cols, പൂർണ്ണസംഖ്യ _type, UMatUsageFlags _usageFlags)
{
    _type &= TYPE_MASK;
    പൂർണ്ണസംഖ്യ sz[] = {_rows, _cols};
    fit(2, sz, _type, _usageFlags);
}

ശൂന്യം UMat::fit(Size _sz, പൂർണ്ണസംഖ്യ _type, UMatUsageFlags _usageFlags)
{
    fit(_sz.height, _sz.width, _type, _usageFlags);
}

ശൂന്യം UMat::fitSameSize(നിവേശം_Array m, പൂർണ്ണസംഖ്യ _type, UMatUsageFlags _usageFlags)
{
    fit(m.shape(), _type, _usageFlags);
}

ശൂന്യം UMat::copySize(സ്ഥിരം UMat& m)
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

ശൂന്യം UMat::deallocate()
{
    UMatData* u_ = u;
    u = ശൂന്യ;
    u_->currAllocator->deallocate(u_);
}


UMat::UMat(സ്ഥിരം UMat& m, സ്ഥിരം Range& _rowRange, സ്ഥിരം Range& _colRange)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(USAGE_DEFAULT), u(0), offset(0)
{
    CV_Assert( m.dims >= 2 && m.dims <= MatShape::MAX_DIMS );
    എങ്കിൽ( m.dims > 2 )
    {
        Range rs[MatShape::MAX_DIMS];
        rs[0] = _rowRange;
        rs[1] = _colRange;
        വേണ്ടി( പൂർണ്ണസംഖ്യ i = 2; i < m.dims; i++ )
            rs[i] = Range::all();
        *ഇത് = m(rs);
        തിരികെ;
    }

    *ഇത് = m;
    എങ്കിൽ( _rowRange != Range::all() && _rowRange != Range(0,rows) )
    {
        CV_Assert( 0 <= _rowRange.start &&
                  _rowRange.start <= _rowRange.end &&
                  _rowRange.end <= m.rows );
        size[0] = rows = _rowRange.size();
        offset += step*_rowRange.start;
        flags |= SUBMATRIX_FLAG;
    }

    എങ്കിൽ( _colRange != Range::all() && _colRange != Range(0,cols) )
    {
        CV_Assert( 0 <= _colRange.start &&
                  _colRange.start <= _colRange.end &&
                  _colRange.end <= m.cols );
        size[1] = cols = _colRange.size();
        offset += _colRange.start*elemSize();
        flags |= SUBMATRIX_FLAG;
    }

    updateContinuityFlag();

    എങ്കിൽ( rows <= 0 || cols <= 0 )
    {
        release();
        rows = cols = 0;
    }
}


UMat::UMat(സ്ഥിരം UMat& m, സ്ഥിരം Rect& roi)
    : flags(m.flags), dims(2), rows(roi.height), cols(roi.width),
    allocator(m.allocator), usageFlags(m.usageFlags), u(m.u),
    offset(m.offset + roi.y*m.step[0]), size(2)
{
    CV_Assert( m.dims <= 2 );

    size_t esz = CV_ELEM_SIZE(flags);
    offset += roi.x*esz;
    CV_Assert( 0 <= roi.x && 0 <= roi.width && roi.x + roi.width <= m.cols &&
              0 <= roi.y && 0 <= roi.height && roi.y + roi.height <= m.rows );
    എങ്കിൽ( roi.width < m.cols || roi.height < m.rows )
        flags |= SUBMATRIX_FLAG;

    size[0] = rows;
    size[1] = cols;
    step[0] = m.dims == 2 ? m.step[0] : cols*esz;
    step[1] = esz;

    updateContinuityFlag();

    addref();
    എങ്കിൽ( rows <= 0 || cols <= 0 )
    {
        rows = cols = 0;
        release();
    }
}


UMat::UMat(സ്ഥിരം UMat& m, സ്ഥിരം Range* ranges)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(USAGE_DEFAULT), u(0), offset(0)
{
    പൂർണ്ണസംഖ്യ i, d = m.dims;

    CV_Assert(ranges);
    വേണ്ടി( i = 0; i < d; i++ )
    {
        Range r = ranges[i];
        CV_Assert( r == Range::all() || (0 <= r.start && r.start < r.end && r.end <= m.size[i]) );
    }
    *ഇത് = m;
    വേണ്ടി( i = 0; i < d; i++ )
    {
        Range r = ranges[i];
        എങ്കിൽ( r != Range::all() && r != Range(0, size.p[i]))
        {
            size.p[i] = r.end - r.start;
            offset += r.start*step.p[i];
            flags |= SUBMATRIX_FLAG;
        }
    }

    എങ്കിൽ (d <= 2) {
        rows = d == 2 ? size[0] : 1;
        cols = d <= 0 ? (d >= 0) : size[d > 1];
    }
    updateContinuityFlag();
}

UMat::UMat(സ്ഥിരം UMat& m, സ്ഥിരം std::vector<Range>& ranges)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(USAGE_DEFAULT), u(0), offset(0)
{
    പൂർണ്ണസംഖ്യ i, d = m.dims;

    CV_Assert((പൂർണ്ണസംഖ്യ)ranges.size() == d);
    വേണ്ടി (i = 0; i < d; i++)
    {
        Range r = ranges[i];
        CV_Assert(r == Range::all() || (0 <= r.start && r.start < r.end && r.end <= m.size[i]));
    }
    *ഇത് = m;
    വേണ്ടി (i = 0; i < d; i++)
    {
        Range r = ranges[i];
        എങ്കിൽ (r != Range::all() && r != Range(0, size.p[i]))
        {
            size.p[i] = r.end - r.start;
            offset += r.start*step.p[i];
            flags |= SUBMATRIX_FLAG;
        }
    }

    എങ്കിൽ (d <= 2) {
        rows = d == 2 ? size[0] : 1;
        cols = d <= 0 ? (d >= 0) : size[d > 1];
    }
    updateContinuityFlag();
}

UMat UMat::diag(പൂർണ്ണസംഖ്യ d) സ്ഥിരം
{
    CV_Assert( dims <= 2 );
    UMat m = *ഇത്;
    size_t esz = elemSize();
    പൂർണ്ണസംഖ്യ len;

    എങ്കിൽ( d >= 0 )
    {
        len = std::min(cols - d, rows);
        m.offset += esz*d;
    }
    അല്ലെങ്കിൽ
    {
        len = std::min(rows + d, cols);
        m.offset -= step[0]*d;
    }
    CV_DbgAssert( len > 0 );

    m.size[0] = m.rows = len;
    m.size[1] = m.cols = 1;
    m.step[0] += (len > 1 ? esz : 0);

    m.updateContinuityFlag();

    എങ്കിൽ( size() != Size(1,1) )
        m.flags |= SUBMATRIX_FLAG;

    തിരികെ m;
}

ശൂന്യം UMat::locateROI( Size& wholeSize, Point& ofs ) സ്ഥിരം
{
    CV_Assert( dims <= 2 && step[0] > 0 );
    size_t esz = elemSize(), minstep;
    ptrdiff_t delta1 = (ptrdiff_t)offset, delta2 = (ptrdiff_t)u->size;

    എങ്കിൽ( delta1 == 0 )
        ofs.x = ofs.y = 0;
    അല്ലെങ്കിൽ
    {
        ofs.y = (പൂർണ്ണസംഖ്യ)(delta1/step[0]);
        ofs.x = (പൂർണ്ണസംഖ്യ)((delta1 - step[0]*ofs.y)/esz);
        CV_DbgAssert( offset == (size_t)(ofs.y*step[0] + ofs.x*esz) );
    }
    minstep = (ofs.x + cols)*esz;
    wholeSize.height = (പൂർണ്ണസംഖ്യ)((delta2 - minstep)/step[0] + 1);
    wholeSize.height = std::max(wholeSize.height, ofs.y + rows);
    wholeSize.width = (പൂർണ്ണസംഖ്യ)((delta2 - step*(wholeSize.height-1))/esz);
    wholeSize.width = std::max(wholeSize.width, ofs.x + cols);
}


UMat& UMat::adjustROI( പൂർണ്ണസംഖ്യ dtop, പൂർണ്ണസംഖ്യ dbottom, പൂർണ്ണസംഖ്യ dleft, പൂർണ്ണസംഖ്യ dright )
{
    CV_Assert( dims == 2 && step[0] > 0 );
    Size wholeSize; Point ofs;
    size_t esz = elemSize();
    locateROI( wholeSize, ofs );
    പൂർണ്ണസംഖ്യ row1 = std::min(std::max(ofs.y - dtop, 0), wholeSize.height), row2 = std::max(0, std::min(ofs.y + rows + dbottom, wholeSize.height));
    പൂർണ്ണസംഖ്യ col1 = std::min(std::max(ofs.x - dleft, 0), wholeSize.width), col2 = std::max(0, std::min(ofs.x + cols + dright, wholeSize.width));
    എങ്കിൽ(row1 > row2)
        std::swap(row1, row2);
    എങ്കിൽ(col1 > col2)
        std::swap(col1, col2);

    offset += (row1 - ofs.y)*step + (col1 - ofs.x)*esz;
    rows = row2 - row1; cols = col2 - col1;
    size.p[0] = rows; size.p[1] = cols;
    updateContinuityFlag();
    തിരികെ *ഇത്;
}


UMat UMat::reshape(പൂർണ്ണസംഖ്യ new_cn, പൂർണ്ണസംഖ്യ new_rows) സ്ഥിരം
{
    പൂർണ്ണസംഖ്യ cn = channels();
    UMat hdr = *ഇത്;

    എങ്കിൽ( dims > 2 && new_rows == 0 && new_cn != 0 && size[dims-1]*cn % new_cn == 0 )
    {
        hdr.flags = (hdr.flags & ~CV_MAT_CN_MASK) | ((new_cn-1) << CV_CN_SHIFT);
        hdr.step[dims-1] = CV_ELEM_SIZE(hdr.flags);
        hdr.size[dims-1] = hdr.size[dims-1]*cn / new_cn;
        തിരികെ hdr;
    }

    CV_Assert( dims <= 2 );

    എങ്കിൽ( new_cn == 0 )
        new_cn = cn;

    പൂർണ്ണസംഖ്യ total_width = cols * cn;

    എങ്കിൽ( (new_cn > total_width || total_width % new_cn != 0) && new_rows == 0 )
        new_rows = rows * total_width / new_cn;

    എങ്കിൽ( new_rows != 0 && new_rows != rows )
    {
        പൂർണ്ണസംഖ്യ total_size = total_width * rows;
        എങ്കിൽ( !isContinuous() )
            CV_Error( cv::Error::BadStep,
            "The matrix is not continuous, thus its number of rows can not be changed" );

        total_width = total_size / new_rows;

        എങ്കിൽ( total_width * new_rows != total_size )
            CV_Error( cv::Error::StsBadArg, "The total number of matrix elements "
                                    "is not divisible by the new number of rows" );

        hdr.size[0] = hdr.rows = new_rows;
        hdr.step[0] = total_width * elemSize1();
    } അല്ലെങ്കിൽ {
        hdr.size[0] = hdr.rows = rows;
        എങ്കിൽ (dims <= 1)
            hdr.step[0] = cols * CV_ELEM_SIZE(flags);
    }

    പൂർണ്ണസംഖ്യ new_width = total_width / new_cn;

    എങ്കിൽ( new_width * new_cn != total_width )
        CV_Error( cv::Error::Bad_അക്കം_Channels,
        "The total width is not divisible by the new number of channels" );

    hdr.size.dims = hdr.dims = 2;
    hdr.size[1] = hdr.cols = new_width;
    hdr.flags = (hdr.flags & ~CV_MAT_CN_MASK) | ((new_cn-1) << CV_CN_SHIFT);
    hdr.step[1] = CV_ELEM_SIZE(hdr.flags);
    തിരികെ hdr;
}

#ifdef HAVE_OPENCL
നാമാകാശ {
സ്ഥിതം തർക്കം ocl_setDiag(സ്ഥിരം UMat& d, UMat& m, പൂർണ്ണസംഖ്യ len)
{
    പൂർണ്ണസംഖ്യ cn = d.channels();
    പൂർണ്ണസംഖ്യ depth = d.depth();

    എങ്കിൽ (depth == CV_64F && !ocl::Device::getDefault().doubleFPConfig())
        തിരികെ അസത്യം;

    String opts = format("-D SET_DIAG -D T1=%s -D cn=%d -D IS_ROW_VECTOR=%d",
                         ocl::memopTypeToStr(depth),
                         cn,
                         (d.rows == 1) ? 1 : 0);

    ocl::Kernel k("setDiag", ocl::core::copyset_oclsrc, opts);
    എങ്കിൽ (k.empty())
        തിരികെ അസത്യം;

    k.args(ocl::KernelArg::WriteOnly(m),
           ocl::KernelArg::ReadOnlyNoSize(d),
           len);

    size_t globalsize[2] = { (size_t)len, (size_t)len };
    തിരികെ k.run(2, globalsize, ശൂന്യ, അസത്യം);
}
}
#endif

UMat UMat::diag(സ്ഥിരം UMat& d, UMatUsageFlags usageFlags)
{
    CV_INSTRUMENT_REGION();

    CV_Assert(d.cols == 1 || d.rows == 1);
    പൂർണ്ണസംഖ്യ len = d.rows + d.cols - 1;

#ifdef HAVE_OPENCL
    എങ്കിൽ (ocl::useOpenCL())
    {
        UMat m(len, len, d.type(), usageFlags);
        എങ്കിൽ (ocl_setDiag(d, m, len))
        {
            CV_IMPL_ADD(CV_IMPL_OCL);
            തിരികെ m;
        }
    }
#endif

    UMat m(len, len, d.type(), Scalar(0), usageFlags);
    UMat md = m.diag();
    എങ്കിൽ( d.cols == 1 )
        d.copyTo(md);
    അല്ലെങ്കിൽ
        transpose(d, md);
    തിരികെ m;
}

പൂർണ്ണസംഖ്യ UMat::checkVector(പൂർണ്ണസംഖ്യ _elemChannels, പൂർണ്ണസംഖ്യ _depth, തർക്കം _requireContinuous) സ്ഥിരം
{
    തിരികെ (depth() == _depth || _depth <= 0) &&
        (isContinuous() || !_requireContinuous) &&
        ((dims <= 2 && (((rows == 1 || cols == 1) && channels() == _elemChannels) ||
                        (cols == _elemChannels && channels() == 1))) ||
        (dims == 3 && channels() == 1 && size.p[2] == _elemChannels && (size.p[0] == 1 || size.p[1] == 1) &&
         (isContinuous() || step.p[1] == step.p[2]*size.p[2])))
    ? (പൂർണ്ണസംഖ്യ)(total()*channels()/_elemChannels) : -1;
}

UMat UMat::reshape(പൂർണ്ണസംഖ്യ _cn, പൂർണ്ണസംഖ്യ _newndims, സ്ഥിരം പൂർണ്ണസംഖ്യ* _newsz) സ്ഥിരം
{
    എങ്കിൽ(_newndims == dims)
    {
        എങ്കിൽ(_newsz == 0)
            തിരികെ reshape(_cn);
        എങ്കിൽ(_newndims == 2)
            തിരികെ reshape(_cn, _newsz[0]);
    }

    എങ്കിൽ (isContinuous())
    {
        CV_Assert(_cn >= 0 && _newndims >= 0 && _newndims <= CV_MAX_DIM && (_newndims == 0 || _newsz != 0));

        എങ്കിൽ (_cn == 0)
            _cn = ഇത്->channels();
        അല്ലെങ്കിൽ
            CV_Assert(_cn <= CV_CN_MAX);

        size_t total_elem1_ref = ഇത്->total() * ഇത്->channels();
        size_t total_elem1 = _cn;

        AutoBuffer<പൂർണ്ണസംഖ്യ, 4> newsz_buf( (size_t)std::max(_newndims, 1) );

        പൂർണ്ണസംഖ്യ m1_idx = -1;

        വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < _newndims; i++)
        {
            എങ്കിൽ (_newsz[i] >= 0) {
                എങ്കിൽ (_newsz[i] == 0 && i < dims)
                    newsz_buf[i] = size.p[i];
                അല്ലെങ്കിൽ
                    newsz_buf[i] = _newsz[i];
                total_elem1 *= (size_t)newsz_buf[i];
            } അല്ലെങ്കിൽ {
                എങ്കിൽ (m1_idx >= 0)
                    CV_Error(cv::Error::StsBadSize, "More than one '-1' occured in the new shape");
                m1_idx = i;
            }
        }

        എങ്കിൽ (m1_idx >= 0) {
            എങ്കിൽ (total_elem1 == 0) {
                CV_Assert(total_elem1_ref == 0);
                total_elem1 = 1;
            }
            CV_Assert(total_elem1_ref % total_elem1 == 0);
            newsz_buf[m1_idx] = (പൂർണ്ണസംഖ്യ)(total_elem1_ref / total_elem1);
        } അല്ലെങ്കിൽ എങ്കിൽ (total_elem1 != total_elem1_ref) {
            CV_Error(cv::Error::StsUnmatchedSizes, "Requested and source matrices have different count of elements");
        }

        UMat hdr = *ഇത്;
        hdr.flags = (hdr.flags & ~CV_MAT_CN_MASK) | ((_cn-1) << CV_CN_SHIFT);
        setSize(hdr, _newndims, newsz_buf.data(), ശൂന്യ, സത്യം);

        തിരികെ hdr;
    }

    CV_Error(cv::Error::StsNotImplemented, "Reshaping of n-dimensional non-continuous matrices is not supported yet");
}

UMat UMat::reshape(പൂർണ്ണസംഖ്യ _cn, സ്ഥിരം MatShape& _newshape) സ്ഥിരം
{
    എങ്കിൽ (_newshape.dims < 0) {
        പൂർണ്ണസംഖ്യ newshape[] = {0};
        തിരികെ reshape(_cn, 1, newshape);
    }
    തിരികെ reshape(_cn, _newshape.dims, _newshape.p);
}

Mat UMat::getMat(AccessFlag accessFlags) സ്ഥിരം
{
    എങ്കിൽ(!u)
        തിരികെ Mat();
    // TODO Support ACCESS_READ (ACCESS_WRITE) without unnecessary data transfers
    accessFlags |= ACCESS_RW;
    UMatDataAutoLock autolock(u);
    ശ്രമിക്കുക
    {
        എങ്കിൽ(CV_XADD(&u->refcount, 1) == 0)
            u->currAllocator->map(u, accessFlags);
        എങ്കിൽ (u->data != 0)
        {
            Mat hdr(dims, size.p, type(), u->data + offset, step.p);
            hdr.flags = flags;
            hdr.u = u;
            hdr.datastart = u->data;
            hdr.data = u->data + offset;
            hdr.datalimit = hdr.dataend = u->data + u->size;
            തിരികെ hdr;
        }
    }
    പിടിക്കുക(...)
    {
        CV_XADD(&u->refcount, -1);
        എറിയുക;
    }
    CV_XADD(&u->refcount, -1);
    CV_Assert(u->data != 0 && "Error mapping of UMat to host memory.");
    തിരികെ Mat();
}

ശൂന്യം* UMat::handle(AccessFlag accessFlags) സ്ഥിരം
{
    എങ്കിൽ( !u )
        തിരികെ 0;

    CV_Assert(u->refcount == 0);
    CV_Assert(!u->deviceCopyObsolete() || u->copyOnMap());
    എങ്കിൽ (u->deviceCopyObsolete())
    {
        u->currAllocator->unmap(u);
    }

    എങ്കിൽ (!!(accessFlags & ACCESS_WRITE))
        u->markHostCopyObsolete(സത്യം);

    തിരികെ u->handle;
}

ശൂന്യം UMat::ndoffset(size_t* ofs) സ്ഥിരം
{
    // offset = step[0]*ofs[0] + step[1]*ofs[1] + step[2]*ofs[2] + ...;
    size_t val = offset;
    വേണ്ടി( പൂർണ്ണസംഖ്യ i = 0; i < dims; i++ )
    {
        size_t s = step.p[i];
        ofs[i] = val / s;
        val -= ofs[i]*s;
    }
}

ശൂന്യം UMat::copyTo(OutputArray _dst) സ്ഥിരം
{
    CV_INSTRUMENT_REGION();

#ifdef HAVE_CUDA
    എങ്കിൽ (_dst.isGpuMat())
    {
        _dst.getGpuMat().upload(*ഇത്);
        തിരികെ;
    }
#endif

    പൂർണ്ണസംഖ്യ stype = type();
    പൂർണ്ണസംഖ്യ dtype = _dst.type();
    എങ്കിൽ( _dst.fixedType() && dtype != stype )
    {
        CV_Assert( CV_MAT_CN(stype) == CV_MAT_CN(dtype) );
        convertTo( _dst, dtype );
        തിരികെ;
    }

    എങ്കിൽ( dims == 0 && empty() )
    {
        _dst.release();
        ശൂന്യം* obj = _dst.getObj();
        എങ്കിൽ (_dst.isMat())
            reinterpret_cast<Mat*>(obj)->flags = Mat::MAGIC_VAL | Mat::CONTINUOUS_FLAG | stype;
        അല്ലെങ്കിൽ എങ്കിൽ (_dst.isUMat())
            reinterpret_cast<UMat*>(obj)->flags = UMat::MAGIC_VAL | UMat::CONTINUOUS_FLAG | stype;
        അല്ലെങ്കിൽ എങ്കിൽ (_dst.isGpuMat())
            reinterpret_cast<cuda::GpuMat*>(obj)->flags = stype;
        തിരികെ;
    }

    _dst.create( dims, size.p, stype );
    എങ്കിൽ (empty())
        തിരികെ;

    size_t sz[CV_MAX_DIM] = {1}, srcofs[CV_MAX_DIM]={0}, dstofs[CV_MAX_DIM]={0};
    size_t esz = CV_ELEM_SIZE(stype);
    പൂർണ്ണസംഖ്യ i, d = std::max(dims, 1);
    വേണ്ടി( i = 0; i < d; i++ )
        sz[i] = size.p[i];
    sz[d-1] *= esz;
    ndoffset(srcofs);
    srcofs[d-1] *= esz;

    എങ്കിൽ( _dst.isUMat() )
    {
        UMat dst = _dst.getUMat();
        CV_Assert(dst.u);
        എങ്കിൽ( u == dst.u && dst.offset == offset )
            തിരികെ;

        എങ്കിൽ (u->currAllocator == dst.u->currAllocator)
        {
            dst.ndoffset(dstofs);
            dstofs[dims-1] *= esz;
            u->currAllocator->copy(u, dst.u, d, sz, srcofs, step.p, dstofs, dst.step.p, അസത്യം);
            തിരികെ;
        }

        // different allocators: never map a device destination, upload into it instead
        // (through a host Mat when both sides are device memory)
        MatAllocator* hostAllocator = Mat::getDefaultAllocator();
        എങ്കിൽ (dst.u->currAllocator != hostAllocator)
        {
            എങ്കിൽ (u->currAllocator == hostAllocator)
                getMat(ACCESS_READ).copyTo(_dst);
            അല്ലെങ്കിൽ
            {
                Mat tmp;
                copyTo(tmp);
                tmp.copyTo(_dst);
            }
            തിരികെ;
        }
    }

    Mat dst = _dst.getMat();
    u->currAllocator->download(u, dst.ptr(), d, sz, srcofs, step.p, dst.step.p);
}

ശൂന്യം UMat::copyTo(OutputArray _dst, നിവേശം_Array _mask) സ്ഥിരം
{
    CV_INSTRUMENT_REGION();

    എങ്കിൽ( _mask.empty() )
    {
        copyTo(_dst);
        തിരികെ;
    }
#ifdef HAVE_OPENCL
    പൂർണ്ണസംഖ്യ cn = channels(), mtype = _mask.type(), mdepth = CV_MAT_DEPTH(mtype), mcn = CV_MAT_CN(mtype);
    CV_Assert( (mdepth == CV_8U || mdepth == CV_8S || mdepth == CV_Bool) && (mcn == 1 || mcn == cn) );

    എങ്കിൽ (ocl::useOpenCL() && _dst.isUMat() && dims <= 2)
    {
        UMatData * prevu = _dst.getUMat().u;
        _dst.create( size, type() );

        UMat dst = _dst.getUMat();

        തർക്കം haveDstUninit = അസത്യം;
        എങ്കിൽ( prevu != dst.u ) // do not leave dst uninitialized
            haveDstUninit = സത്യം;

        String opts = format("-D COPY_TO_MASK -D T1=%s -D scn=%d -D mcn=%d%s",
                             ocl::memopTypeToStr(depth()), cn, mcn,
                             haveDstUninit ? " -D HAVE_DST_UNINIT" : "");

        ocl::Kernel k("copyToMask", ocl::core::copyset_oclsrc, opts);
        എങ്കിൽ (!k.empty())
        {
            k.args(ocl::KernelArg::ReadOnlyNoSize(*ഇത്),
                   ocl::KernelArg::ReadOnlyNoSize(_mask.getUMat()),
                   haveDstUninit ? ocl::KernelArg::WriteOnly(dst) :
                                   ocl::KernelArg::ReadWrite(dst));

            size_t globalsize[2] = { (size_t)cols, (size_t)rows };
            എങ്കിൽ (k.run(2, globalsize, ശൂന്യ, അസത്യം))
            {
                CV_IMPL_ADD(CV_IMPL_OCL);
                തിരികെ;
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

UMat& UMat::setTo(നിവേശം_Array മൂല്യം, നിവേശം_Array _mask)
{
    CV_INSTRUMENT_REGION();

    തർക്കം haveMask = !_mask.empty();
#ifdef HAVE_OPENCL
    പൂർണ്ണസംഖ്യ tp = type(), cn = CV_MAT_CN(tp), d = CV_MAT_DEPTH(tp);

    എങ്കിൽ( dims <= 2 && cn <= 4 && CV_MAT_DEPTH(tp) < CV_64F && ocl::useOpenCL() )
    {
        Mat value = മൂല്യം.getMat();
        CV_Assert( checkScalar(value, type(), മൂല്യം.kind(), നിവേശം_Array_2::UMAT) );
        പൂർണ്ണസംഖ്യ kercn = haveMask || cn == 3 ? cn : std::max(cn, ocl::predictOptimalVectorWidth(*ഇത്)),
                kertp = CV_MAKE_TYPE(d, kercn);

        ഇരട്ട buf[16] = { 0, 0, 0, 0, 0, 0, 0, 0,
                           0, 0, 0, 0, 0, 0, 0, 0 };
        convertAndUnrollScalar(value, tp, (uchar *)buf, kercn / cn);

        പൂർണ്ണസംഖ്യ scalarcn = kercn == 3 ? 4 : kercn, rowsPerWI = ocl::Device::getDefault().isIntel() ? 4 : 1;
        String opts = format("-D dstT=%s -D rowsPerWI=%d -D dstST=%s -D dstT1=%s -D cn=%d",
                             ocl::memopTypeToStr(kertp), rowsPerWI,
                             ocl::memopTypeToStr(CV_MAKETYPE(d, scalarcn)),
                             ocl::memopTypeToStr(d), kercn);

        ocl::Kernel setK(haveMask ? "setMask" : "set", ocl::core::copyset_oclsrc, opts);
        എങ്കിൽ( !setK.empty() )
        {
            ocl::KernelArg scalararg(ocl::KernelArg::CONSTANT, 0, 0, 0, buf, CV_ELEM_SIZE(d) * scalarcn);
            UMat mask;

            എങ്കിൽ( haveMask )
            {
                mask = _mask.getUMat();
                CV_Assert( mask.size() == size() && (mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool) );
                ocl::KernelArg maskarg = ocl::KernelArg::ReadOnlyNoSize(mask),
                        dstarg = ocl::KernelArg::ReadWrite(*ഇത്);
                setK.args(maskarg, dstarg, scalararg);
            }
            അല്ലെങ്കിൽ
            {
                ocl::KernelArg dstarg = ocl::KernelArg::WriteOnly(*ഇത്, cn, kercn);
                setK.args(dstarg, scalararg);
            }

            size_t globalsize[] = { (size_t)cols * cn / kercn, ((size_t)rows + rowsPerWI - 1) / rowsPerWI };
            എങ്കിൽ( setK.run(2, globalsize, ശൂന്യ, അസത്യം) )
            {
                CV_IMPL_ADD(CV_IMPL_OCL);
                തിരികെ *ഇത്;
            }
        }
    }
#endif
    Mat m = getMat(haveMask ? ACCESS_RW : ACCESS_WRITE);
    m.setTo(മൂല്യം, _mask);
    തിരികെ *ഇത്;
}

UMat& UMat::operator = (സ്ഥിരം Scalar& s)
{
    setTo(s);
    തിരികെ *ഇത്;
}

UMat UMat::t() സ്ഥിരം
{
    UMat m;
    transpose(*ഇത്, m);
    തിരികെ m;
}

UMat UMat::zeros(പൂർണ്ണസംഖ്യ rows, പൂർണ്ണസംഖ്യ cols, പൂർണ്ണസംഖ്യ type, UMatUsageFlags usageFlags)
{
    തിരികെ UMat(rows, cols, type, Scalar::all(0), usageFlags);
}

UMat UMat::zeros(Size size, പൂർണ്ണസംഖ്യ type, UMatUsageFlags usageFlags)
{
    തിരികെ UMat(size, type, Scalar::all(0), usageFlags);
}

UMat UMat::zeros(പൂർണ്ണസംഖ്യ ndims, സ്ഥിരം പൂർണ്ണസംഖ്യ* sz, പൂർണ്ണസംഖ്യ type, UMatUsageFlags usageFlags)
{
    തിരികെ UMat(ndims, sz, type, Scalar::all(0), usageFlags);
}

UMat UMat::zeros(സ്ഥിരം MatShape& shape, പൂർണ്ണസംഖ്യ type, UMatUsageFlags usageFlags)
{
    തിരികെ UMat(shape.dims, shape.p, type, Scalar::all(0), usageFlags);
}

UMat UMat::ones(പൂർണ്ണസംഖ്യ rows, പൂർണ്ണസംഖ്യ cols, പൂർണ്ണസംഖ്യ type, UMatUsageFlags usageFlags)
{
    തിരികെ UMat(rows, cols, type, Scalar(1), usageFlags);
}

UMat UMat::ones(Size size, പൂർണ്ണസംഖ്യ type, UMatUsageFlags usageFlags)
{
    തിരികെ UMat(size, type, Scalar(1), usageFlags);
}

UMat UMat::ones(പൂർണ്ണസംഖ്യ ndims, സ്ഥിരം പൂർണ്ണസംഖ്യ* sz, പൂർണ്ണസംഖ്യ type, UMatUsageFlags usageFlags)
{
    തിരികെ UMat(ndims, sz, type, Scalar(1), usageFlags);
}

UMat UMat::ones(സ്ഥിരം MatShape& shape, പൂർണ്ണസംഖ്യ type, UMatUsageFlags usageFlags)
{
    തിരികെ UMat(shape.dims, shape.p, type, Scalar(1), usageFlags);
}

}

/* End of file. */
