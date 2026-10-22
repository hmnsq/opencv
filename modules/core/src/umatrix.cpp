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

#समावेश "precomp.hpp"
#समावेश "opencl_kernels_core.hpp"
#समावेश "umatrix.hpp"

#समावेश <opencv2/core/utils/tls.hpp>

///////////////////////////////// UMat implementation ///////////////////////////////

namespace cv {

// forward decls, implementation is below in this file
व्योम set_आकार(UMat& m, पूर्णांक _dims, स्थिर पूर्णांक* _sz, स्थिर माप_प्रकार* _steps,
             bool autoSteps = false);

व्योम updateContinuityFlag(UMat& m);
व्योम finalizeHdr(UMat& m);

UMat_तथ्य::UMat_तथ्य(स्थिर MatAllocator* allocator)
{
    पूर्व_Allocator = currAllocator = allocator;
    urefcount = refcount = mapcount = 0;
    तथ्य = origdata = 0;
    size = 0;
    flags = static_cast<UMat_तथ्य::MemoryFlag>(0);
    handle = 0;
    userdata = 0;
    allocatorFlags_ = 0;
    original_UMat_तथ्य = शून्य;
}

UMat_तथ्य::~UMat_तथ्य()
{
    पूर्व_Allocator = currAllocator = 0;
    urefcount = refcount = 0;
    CV_Assert(mapcount == 0);
    तथ्य = origdata = 0;
    size = 0;
    bool isAsyncCleanup = !!(flags & UMat_तथ्य::ASYNC_CLEANUP);
    flags = static_cast<UMat_तथ्य::MemoryFlag>(0);
    handle = 0;
    userdata = 0;
    allocatorFlags_ = 0;
    अगर (original_UMat_तथ्य)
    {
        bool showWarn = false;
        UMat_तथ्य* u = original_UMat_तथ्य;
        bool zero_Ref = CV_XADD(&(u->refcount), -1) == 1;
        अगर (zero_Ref)
        {
            // simulate Mat::deallocate
            अगर (u->mapcount != 0)
            {
                (u->currAllocator ? u->currAllocator : /* TODO allocator ? allocator :*/ Mat::getDefaultAllocator())->unmap(u);
            }
            अन्यथा
            {
                // we don't do "map", so we can't do "unmap"
            }
        }
        bool zero_URef = CV_XADD(&(u->urefcount), -1) == 1;
        अगर (zero_Ref && !zero_URef)
            showWarn = true;
        अगर (zero_Ref && zero_URef) // oops, we need to free resources
        {
            showWarn = !isAsyncCleanup;
            // simulate UMat::deallocate
            u->currAllocator->deallocate(u);
        }
#अगर_अघोषित न_संशोधन
        अगर (showWarn)
        {
            अटल पूर्णांक warn_message_showed = 0;
            अगर (warn_message_showed++ < 100)
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
#पूर्ण_अगर
        original_UMat_तथ्य = शून्य;
    }
}

#अगर_अघोषित OPENCV_DISABLE_THREAD_SUPPORT

// it should be a prime number for the best hash function
क्रमागत { UMAT_NLOCKS = 31 };
अटल Mutex umatLocks[UMAT_NLOCKS];

अटल माप_प्रकार get_UMat_तथ्य_Lock_सूचकांक(स्थिर UMat_तथ्य* u)
{
    माप_प्रकार idx = ((माप_प्रकार)(व्योम*)u) % UMAT_NLOCKS;
    वापस idx;
}

व्योम UMat_तथ्य::lock()
{
    माप_प्रकार idx = get_UMat_तथ्य_Lock_सूचकांक(यह);
    //printf("%d lock(%d)\n", cv::utils::getThreadID(), (int)idx);
    umatLocks[idx].lock();
}

व्योम UMat_तथ्य::unlock()
{
    माप_प्रकार idx = get_UMat_तथ्य_Lock_सूचकांक(यह);
    //printf("%d unlock(%d)\n", cv::utils::getThreadID(), (int)idx);
    umatLocks[idx].unlock();
}


// Do not allow several lock() calls with different UMatData objects.
काष्ठा UMat_तथ्य_Auto_Locker
{
    पूर्णांक usage_गिनती;
    UMat_तथ्य* locked_objects[2];
    UMat_तथ्य_Auto_Locker() : usage_गिनती(0) { locked_objects[0] = शून्य; locked_objects[1] = शून्य; }

    व्योम lock(UMat_तथ्य*& u1)
    {
        bool locked_1 = (u1 == locked_objects[0] || u1 == locked_objects[1]);
        अगर (locked_1)
        {
            u1 = शून्य;
            वापस;
        }
        CV_Assert(usage_गिनती == 0);  // UMatDataAutoLock can't be used multiple times from the same thread
        usage_गिनती = 1;
        locked_objects[0] = u1;
        u1->lock();
    }
    व्योम lock(UMat_तथ्य*& u1, UMat_तथ्य*& u2)
    {
        bool locked_1 = (u1 == locked_objects[0] || u1 == locked_objects[1]);
        bool locked_2 = (u2 == locked_objects[0] || u2 == locked_objects[1]);
        अगर (locked_1)
            u1 = शून्य;
        अगर (locked_2)
            u2 = शून्य;
        अगर (locked_1 && locked_2)
            वापस;
        CV_Assert(usage_गिनती == 0);  // UMatDataAutoLock can't be used multiple times from the same thread
        usage_गिनती = 1;
        locked_objects[0] = u1;
        locked_objects[1] = u2;
        अगर (u1)
            u1->lock();
        अगर (u2)
            u2->lock();
    }
    व्योम release(UMat_तथ्य* u1, UMat_तथ्य* u2)
    {
        अगर (u1 == शून्य && u2 == शून्य)
            वापस;
        CV_Assert(usage_गिनती == 1);
        usage_गिनती = 0;
        अगर (u1)
            u1->unlock();
        अगर (u2)
            u2->unlock();
        locked_objects[0] = शून्य; locked_objects[1] = शून्य;
    }
};
अटल TLSData<UMat_तथ्य_Auto_Locker>& get_UMat_तथ्य_Auto_Locker_TLS()
{
    CV_SINGLETON_LAZY_INIT_REF(TLSData<UMat_तथ्य_Auto_Locker>, नया TLSData<UMat_तथ्य_Auto_Locker>());
}
अटल UMat_तथ्य_Auto_Locker& get_UMat_तथ्य_Auto_Locker() { वापस get_UMat_तथ्य_Auto_Locker_TLS().getRef(); }


UMat_तथ्य_Auto_Lock::UMat_तथ्य_Auto_Lock(UMat_तथ्य* u) : u1(u), u2(शून्य)
{
    get_UMat_तथ्य_Auto_Locker().lock(u1);
}
UMat_तथ्य_Auto_Lock::UMat_तथ्य_Auto_Lock(UMat_तथ्य* u1_, UMat_तथ्य* u2_) : u1(u1_), u2(u2_)
{
    अगर (get_UMat_तथ्य_Lock_सूचकांक(u1) > get_UMat_तथ्य_Lock_सूचकांक(u2))
    {
        std::swap(u1, u2);
    }
    get_UMat_तथ्य_Auto_Locker().lock(u1, u2);
}
UMat_तथ्य_Auto_Lock::~UMat_तथ्य_Auto_Lock()
{
    get_UMat_तथ्य_Auto_Locker().release(u1, u2);
}

#अन्यथा

व्योम UMat_तथ्य::lock()
{
    // nothing in OPENCV_DISABLE_THREAD_SUPPORT mode
}

व्योम UMat_तथ्य::unlock()
{
    // nothing in OPENCV_DISABLE_THREAD_SUPPORT mode
}

UMat_तथ्य_Auto_Lock::UMat_तथ्य_Auto_Lock(UMat_तथ्य* u) : u1(u), u2(शून्य)
{
    // nothing in OPENCV_DISABLE_THREAD_SUPPORT mode
}
UMat_तथ्य_Auto_Lock::UMat_तथ्य_Auto_Lock(UMat_तथ्य* u1_, UMat_तथ्य* u2_) : u1(u1_), u2(u2_)
{
    // nothing in OPENCV_DISABLE_THREAD_SUPPORT mode
}
UMat_तथ्य_Auto_Lock::~UMat_तथ्य_Auto_Lock()
{
    // nothing in OPENCV_DISABLE_THREAD_SUPPORT mode
}

#पूर्ण_अगर  // OPENCV_DISABLE_THREAD_SUPPORT

//////////////////////////////// UMat ////////////////////////////////

UMat::UMat(UMatUsageFlags _usageFlags) CV_NOEXCEPT
: flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{}

UMat::UMat(पूर्णांक _rows, पूर्णांक _cols, पूर्णांक _type, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_rows, _cols, _type);
}

UMat::UMat(पूर्णांक _rows, पूर्णांक _cols, पूर्णांक _type, स्थिर Scalar& _s, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_rows, _cols, _type);
    *यह = _s;
}

UMat::UMat(आकार _sz, पूर्णांक _type, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create( _sz.height, _sz.width, _type );
}

UMat::UMat(आकार _sz, पूर्णांक _type, स्थिर Scalar& _s, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_sz.height, _sz.width, _type);
    *यह = _s;
}

UMat::UMat(पूर्णांक _dims, स्थिर पूर्णांक* _sz, पूर्णांक _type, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_dims, _sz, _type);
}

UMat::UMat(पूर्णांक _dims, स्थिर पूर्णांक* _sz, पूर्णांक _type, स्थिर Scalar& _s, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_dims, _sz, _type);
    *यह = _s;
}

UMat::UMat(स्थिर MatShape& _shape, पूर्णांक _type, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_shape.dims, _shape.p, _type);
}

UMat::UMat(स्थिर MatShape& _shape, पूर्णांक _type, स्थिर Scalar& _s, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_shape.dims, _shape.p, _type);
    *यह = _s;
}

UMat::UMat(स्थिर UMat& m)
: flags(m.flags), dims(m.dims), rows(m.rows), cols(m.cols), allocator(m.allocator),
  usageFlags(m.usageFlags), u(m.u), offset(m.offset), size(m.size), step(m.step)
{
    addref();
}

UMat& UMat::चालक=(स्थिर UMat& m)
{
    अगर( यह != &m )
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
    वापस *यह;
}

UMat UMat::clone() स्थिर
{
    UMat m;
    copyTo(m);
    वापस m;
}

व्योम UMat::assignTo(UMat& m, पूर्णांक _type) स्थिर
{
    अगर( _type < 0 )
        m = *यह;
    अन्यथा
        convertTo(m, _type);
}

व्योम UMat::create(पूर्णांक _rows, पूर्णांक _cols, पूर्णांक _type, UMatUsageFlags _usageFlags)
{
    पूर्णांक sz[] = {_rows, _cols};
    create(2, sz, _type, _usageFlags);
}

व्योम UMat::create(आकार _sz, पूर्णांक _type, UMatUsageFlags _usageFlags)
{
    create(_sz.height, _sz.width, _type, _usageFlags);
}

व्योम UMat::create_Same_आकार(निवेश_Array arr, पूर्णांक _type, UMatUsageFlags _usageFlags)
{
    create(arr.shape(), _type, _usageFlags);
}

व्योम UMat::addref()
{
    अगर( u )
        CV_XADD(&(u->urefcount), 1);
}

व्योम UMat::release()
{
    अगर( u && CV_XADD(&(u->urefcount), -1) == 1 )
        deallocate();
    u = 0;
    size.सफा();
    dims = cols = rows = 0;
}

bool UMat::empty() स्थिर
{
    वापस u == 0 || योग() == 0;
}

माप_प्रकार UMat::योग() स्थिर
{
    अगर( dims <= 2 )
        वापस (माप_प्रकार)rows * cols;
    माप_प्रकार p = 1;
    क्रम( पूर्णांक i = 0; i < dims; i++ )
        p *= size[i];
    वापस p;
}

MatShape UMat::shape() स्थिर
{
    वापस size;
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
    m.size.सफा();
    m.allocator = शून्य;
    m.u = शून्य;
    m.offset = 0;
}

UMat& UMat::चालक=(UMat&& m)
{
    अगर (यह == &m)
      वापस *यह;
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
    m.allocator = शून्य;
    m.u = शून्य;
    m.offset = 0;
    वापस *यह;
}


MatAllocator* UMat::getStdAllocator()
{
#अगर_घोषित HAVE_OPENCL
    अगर (ocl::useOpenCL())
        वापस ocl::getOpenCLAllocator();
#पूर्ण_अगर
    वापस Mat::getDefaultAllocator();
}

व्योम swap( UMat& a, UMat& b )
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


व्योम set_आकार( UMat& m, पूर्णांक _dims, स्थिर पूर्णांक* _sz,
              स्थिर माप_प्रकार* _steps, bool autoSteps )
{
    CV_Assert( 0 <= _dims && _dims <= CV_MAX_DIM && _dims <= MatShape::MAX_DIMS);

    m.dims = _dims;
    m.size = MatShape(_dims, _sz);
    m.step[std::max(_dims-1, 0)] = CV_ELEM_आकार(m.flags);
    क्रम (पूर्णांक i = _dims-2; i >= 0; i--) {
        माप_प्रकार autostep = m.size[i+1]*m.step[i+1];
        अगर (_steps) {
            m.step[i] = _steps[i];
            //CV_Assert(m.step[i] >= autostep);
        } अन्यथा अगर (autoSteps) {
            m.step[i] = autostep;
        } अन्यथा {
            m.step[i] = 0;
        }
    }

    अगर( _dims <= 2 )
    {
        m.cols = _dims == 0 ? 1 : _sz ? _sz[_dims > 1] : 0;
        m.rows = _dims < 2 ? 1 : _sz ? _sz[0] : 0;
    } अन्यथा {
        m.cols = m.rows = -1;
    }
}


व्योम UMat::updateContinuityFlag()
{
    flags = cv::updateContinuityFlag(flags, dims, size.p, step.p);
}


व्योम finalizeHdr(UMat& m)
{
    m.updateContinuityFlag();
    पूर्णांक d = m.dims;
    अगर( d > 2 )
        m.rows = m.cols = -1;
}


UMat Mat::getUMat(AccessFlag accessFlags, UMatUsageFlags usageFlags) स्थिर
{
    UMat hdr;
    अगर(!तथ्य)
        वापस hdr;
    अगर (तथ्य != datastart)
    {
        आकार whole_आकार;
        Point ofs;
        locateROI(whole_आकार, ofs);
        आकार sz(cols, rows);
        अगर (ofs.x != 0 || ofs.y != 0)
        {
            Mat src = *यह;
            पूर्णांक dtop = ofs.y;
            पूर्णांक dbottom = whole_आकार.height - src.rows - ofs.y;
            पूर्णांक dleft = ofs.x;
            पूर्णांक dright = whole_आकार.width - src.cols - ofs.x;
            src.adjustROI(dtop, dbottom, dleft, dright);
            वापस src.getUMat(accessFlags, usageFlags)(cv::Rect(ofs.x, ofs.y, sz.width, sz.height));
        }
    }
    CV_Assert(तथ्य == datastart);

    accessFlags |= ACCESS_RW;
    UMat_तथ्य* new_u = शून्य;
    MatStep new_step = step;
    {
        MatAllocator *a = allocator, *a0 = getDefaultAllocator();
        अगर(!a)
            a = a0;
        new_u = a->allocate(dims, size.p, type(), तथ्य, new_step.p, accessFlags, usageFlags);
        new_u->original_UMat_तथ्य = u;
    }
    bool allocated = false;
    प्रयास
    {
        allocated = UMat::getStdAllocator()->allocate(new_u, accessFlags, usageFlags);
    }
    पकड़ो (स्थिर cv::Exception& e)
    {
        ख_लिखो(मानक_त्रुटि, "Exception: %s\n", e.what());
    }
    अगर (!allocated)
    {
        allocated = getDefaultAllocator()->allocate(new_u, accessFlags, usageFlags);
        CV_Assert(allocated);
    }
    अगर (u != शून्य)
    {
#अगर_घोषित HAVE_OPENCL
        अगर (ocl::useOpenCL() && new_u->currAllocator == ocl::getOpenCLAllocator())
        {
            CV_Assert(new_u->अस्थायी_UMat());
        }
#पूर्ण_अगर
        CV_XADD(&(u->refcount), 1);
        CV_XADD(&(u->urefcount), 1);
    }
    प्रयास
    {
        hdr.flags = flags;
        hdr.usageFlags = usageFlags;
        set_आकार(hdr, dims, size.p, new_step.p);
        finalizeHdr(hdr);
        hdr.u = new_u;
        hdr.offset = 0; //data - datastart;
        hdr.addref();
        वापस hdr;
    }
    पकड़ो(...)
    {
        अगर (u != शून्य)
        {
            CV_XADD(&(u->refcount), -1);
            CV_XADD(&(u->urefcount), -1);
        }
        new_u->currAllocator->deallocate(new_u);
        फेंको;
    }

}

व्योम UMat::create(पूर्णांक d0, स्थिर पूर्णांक* _sizes, पूर्णांक _type, UMatUsageFlags _usageFlags)
{
    पूर्णांक sz1 = 1, d = d0;
    पूर्णांक i;
    अगर (d == 0) {
        d = 1;
        _sizes = (स्थिर पूर्णांक*)&sz1;
    }
    CV_Assert(0 <= d && d <= CV_MAX_DIM && _sizes);
    _type = CV_MAT_TYPE(_type);

    // if param value is USAGE_DEFAULT by implicit default param value -or- explicit value
    // ...then don't change the existing usageFlags
    // it is not possible to change usage from non-default to USAGE_DEFAULT through create()
    // ...instead must construct UMat()
    अगर (_usageFlags == cv::USAGE_DEFAULT)
    {
        _usageFlags = usageFlags;
    }

    अगर( u && d == dims && _type == type() && _usageFlags == usageFlags )
    {
        क्रम( i = 0; i < d; i++ )
            अगर( size[i] != _sizes[i] )
                अवरोध;
        अगर( i == d )
            वापस;
    }

    पूर्णांक _sizes_backup[CV_MAX_DIM]; // #5991
    अगर (_sizes == (यह->size.p))
    {
        क्रम(i = 0; i < d; i++ )
            _sizes_backup[i] = _sizes[i];
        _sizes = _sizes_backup;
    }

    release();
    usageFlags = _usageFlags;
    अगर( d == 0 )
        वापस;
    flags = (_type & CV_MAT_TYPE_MASK) | MAGIC_VAL;
    set_आकार(*यह, d, _sizes, 0, true);
    offset = 0;

    अगर( योग() > 0 )
    {
        MatAllocator *a = allocator, *a0 = getStdAllocator();
        अगर (!a)
        {
            a = a0;
            a0 = Mat::getDefaultAllocator();
        }
        प्रयास
        {
            u = a->allocate(dims, size.p, _type, 0, step.p, ACCESS_RW /* ignored */, usageFlags);
            CV_Assert(u != 0);
        }
        पकड़ो(...)
        {
            अगर(a != a0)
                u = a0->allocate(dims, size.p, _type, 0, step.p, ACCESS_RW /* ignored */, usageFlags);
            CV_Assert(u != 0);
        }
        CV_Assert( step[dims-1] == (माप_प्रकार)CV_ELEM_आकार(flags) );
    }

    finalizeHdr(*यह);
    addref();
    size.dims = dims = d0;
}

व्योम UMat::create(स्थिर std::vector<पूर्णांक>& _sizes, पूर्णांक _type, UMatUsageFlags _usageFlags)
{
    create((पूर्णांक)_sizes.size(), _sizes.तथ्य(), _type, _usageFlags);
}

व्योम UMat::create(स्थिर MatShape& _shape, पूर्णांक _type, UMatUsageFlags _usageFlags)
{
    अगर (_shape.dims < 0) {
        release();
    } अन्यथा {
        create(_shape.dims, _shape.p, _type, _usageFlags);
        size.layout = _shape.layout;
        size.C = _shape.C;
    }
}

व्योम UMat::fit(पूर्णांक _dims, स्थिर पूर्णांक* _sizes, पूर्णांक _type, UMatUsageFlags _usageFlags)
{
    अगर (_usageFlags == cv::USAGE_DEFAULT)
        _usageFlags = usageFlags;
    माप_प्रकार old_योग_Bytes = u ? u->size : 0;
    माप_प्रकार esz = CV_ELEM_आकार(_type), new_योग = _dims >= 0;
    क्रम (पूर्णांक i = 0; i < _dims; i++)
        new_योग *= _sizes[i];
    माप_प्रकार new_योग_Bytes = new_योग*esz;
    अगर (new_योग_Bytes > 0 &&
        (!isContinuous() ||
         new_योग_Bytes > old_योग_Bytes ||
         offset != 0 ||
         _usageFlags != usageFlags)) {
        create(_dims, _sizes, _type, _usageFlags);
    } अन्यथा {
        flags = (flags & ~Mat::TYPE_MASK) | CV_MAT_TYPE(_type);
        पूर्णांक dummy_आकार = 0;
        set_आकार(*यह, (_dims >= 0 ? _dims : 1), (_dims >= 0 ? _sizes : &dummy_आकार), nullptr, true);
        finalizeHdr(*यह);
    }
}

व्योम UMat::fit(स्थिर std::vector<पूर्णांक>& _shape, पूर्णांक _type, UMatUsageFlags _usageFlags)
{
    fit((पूर्णांक)_shape.size(), _shape.तथ्य(), _type, _usageFlags);
}

व्योम UMat::fit(स्थिर MatShape& _shape, पूर्णांक _type, UMatUsageFlags _usageFlags)
{
    fit(_shape.dims, _shape.p, _type, _usageFlags);
    size.layout = _shape.layout;
    size.C = _shape.C;
}

व्योम UMat::fit(पूर्णांक _rows, पूर्णांक _cols, पूर्णांक _type, UMatUsageFlags _usageFlags)
{
    _type &= TYPE_MASK;
    पूर्णांक sz[] = {_rows, _cols};
    fit(2, sz, _type, _usageFlags);
}

व्योम UMat::fit(आकार _sz, पूर्णांक _type, UMatUsageFlags _usageFlags)
{
    fit(_sz.height, _sz.width, _type, _usageFlags);
}

व्योम UMat::fit_Same_आकार(निवेश_Array m, पूर्णांक _type, UMatUsageFlags _usageFlags)
{
    fit(m.shape(), _type, _usageFlags);
}

व्योम UMat::copy_आकार(स्थिर UMat& m)
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

व्योम UMat::deallocate()
{
    UMat_तथ्य* u_ = u;
    u = शून्य;
    u_->currAllocator->deallocate(u_);
}


UMat::UMat(स्थिर UMat& m, स्थिर Range& _rowRange, स्थिर Range& _colRange)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(USAGE_DEFAULT), u(0), offset(0)
{
    CV_Assert( m.dims >= 2 && m.dims <= MatShape::MAX_DIMS );
    अगर( m.dims > 2 )
    {
        Range rs[MatShape::MAX_DIMS];
        rs[0] = _rowRange;
        rs[1] = _colRange;
        क्रम( पूर्णांक i = 2; i < m.dims; i++ )
            rs[i] = Range::all();
        *यह = m(rs);
        वापस;
    }

    *यह = m;
    अगर( _rowRange != Range::all() && _rowRange != Range(0,rows) )
    {
        CV_Assert( 0 <= _rowRange.start &&
                  _rowRange.start <= _rowRange.end &&
                  _rowRange.end <= m.rows );
        size[0] = rows = _rowRange.size();
        offset += step*_rowRange.start;
        flags |= SUBMATRIX_FLAG;
    }

    अगर( _colRange != Range::all() && _colRange != Range(0,cols) )
    {
        CV_Assert( 0 <= _colRange.start &&
                  _colRange.start <= _colRange.end &&
                  _colRange.end <= m.cols );
        size[1] = cols = _colRange.size();
        offset += _colRange.start*elem_आकार();
        flags |= SUBMATRIX_FLAG;
    }

    updateContinuityFlag();

    अगर( rows <= 0 || cols <= 0 )
    {
        release();
        rows = cols = 0;
    }
}


UMat::UMat(स्थिर UMat& m, स्थिर Rect& roi)
    : flags(m.flags), dims(2), rows(roi.height), cols(roi.width),
    allocator(m.allocator), usageFlags(m.usageFlags), u(m.u),
    offset(m.offset + roi.y*m.step[0]), size(2)
{
    CV_Assert( m.dims <= 2 );

    माप_प्रकार esz = CV_ELEM_आकार(flags);
    offset += roi.x*esz;
    CV_Assert( 0 <= roi.x && 0 <= roi.width && roi.x + roi.width <= m.cols &&
              0 <= roi.y && 0 <= roi.height && roi.y + roi.height <= m.rows );
    अगर( roi.width < m.cols || roi.height < m.rows )
        flags |= SUBMATRIX_FLAG;

    size[0] = rows;
    size[1] = cols;
    step[0] = m.dims == 2 ? m.step[0] : cols*esz;
    step[1] = esz;

    updateContinuityFlag();

    addref();
    अगर( rows <= 0 || cols <= 0 )
    {
        rows = cols = 0;
        release();
    }
}


UMat::UMat(स्थिर UMat& m, स्थिर Range* ranges)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(USAGE_DEFAULT), u(0), offset(0)
{
    पूर्णांक i, d = m.dims;

    CV_Assert(ranges);
    क्रम( i = 0; i < d; i++ )
    {
        Range r = ranges[i];
        CV_Assert( r == Range::all() || (0 <= r.start && r.start < r.end && r.end <= m.size[i]) );
    }
    *यह = m;
    क्रम( i = 0; i < d; i++ )
    {
        Range r = ranges[i];
        अगर( r != Range::all() && r != Range(0, size.p[i]))
        {
            size.p[i] = r.end - r.start;
            offset += r.start*step.p[i];
            flags |= SUBMATRIX_FLAG;
        }
    }

    अगर (d <= 2) {
        rows = d == 2 ? size[0] : 1;
        cols = d <= 0 ? (d >= 0) : size[d > 1];
    }
    updateContinuityFlag();
}

UMat::UMat(स्थिर UMat& m, स्थिर std::vector<Range>& ranges)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(USAGE_DEFAULT), u(0), offset(0)
{
    पूर्णांक i, d = m.dims;

    CV_Assert((पूर्णांक)ranges.size() == d);
    क्रम (i = 0; i < d; i++)
    {
        Range r = ranges[i];
        CV_Assert(r == Range::all() || (0 <= r.start && r.start < r.end && r.end <= m.size[i]));
    }
    *यह = m;
    क्रम (i = 0; i < d; i++)
    {
        Range r = ranges[i];
        अगर (r != Range::all() && r != Range(0, size.p[i]))
        {
            size.p[i] = r.end - r.start;
            offset += r.start*step.p[i];
            flags |= SUBMATRIX_FLAG;
        }
    }

    अगर (d <= 2) {
        rows = d == 2 ? size[0] : 1;
        cols = d <= 0 ? (d >= 0) : size[d > 1];
    }
    updateContinuityFlag();
}

UMat UMat::diag(पूर्णांक d) स्थिर
{
    CV_Assert( dims <= 2 );
    UMat m = *यह;
    माप_प्रकार esz = elem_आकार();
    पूर्णांक len;

    अगर( d >= 0 )
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

    अगर( size() != आकार(1,1) )
        m.flags |= SUBMATRIX_FLAG;

    वापस m;
}

व्योम UMat::locateROI( आकार& whole_आकार, Point& ofs ) स्थिर
{
    CV_Assert( dims <= 2 && step[0] > 0 );
    माप_प्रकार esz = elem_आकार(), minstep;
    सूचक_भेद_प्रकार delta1 = (सूचक_भेद_प्रकार)offset, delta2 = (सूचक_भेद_प्रकार)u->size;

    अगर( delta1 == 0 )
        ofs.x = ofs.y = 0;
    अन्यथा
    {
        ofs.y = (पूर्णांक)(delta1/step[0]);
        ofs.x = (पूर्णांक)((delta1 - step[0]*ofs.y)/esz);
        CV_DbgAssert( offset == (माप_प्रकार)(ofs.y*step[0] + ofs.x*esz) );
    }
    minstep = (ofs.x + cols)*esz;
    whole_आकार.height = (पूर्णांक)((delta2 - minstep)/step[0] + 1);
    whole_आकार.height = std::max(whole_आकार.height, ofs.y + rows);
    whole_आकार.width = (पूर्णांक)((delta2 - step*(whole_आकार.height-1))/esz);
    whole_आकार.width = std::max(whole_आकार.width, ofs.x + cols);
}


UMat& UMat::adjustROI( पूर्णांक dtop, पूर्णांक dbottom, पूर्णांक dleft, पूर्णांक dright )
{
    CV_Assert( dims == 2 && step[0] > 0 );
    आकार whole_आकार; Point ofs;
    माप_प्रकार esz = elem_आकार();
    locateROI( whole_आकार, ofs );
    पूर्णांक row1 = std::min(std::max(ofs.y - dtop, 0), whole_आकार.height), row2 = std::max(0, std::min(ofs.y + rows + dbottom, whole_आकार.height));
    पूर्णांक col1 = std::min(std::max(ofs.x - dleft, 0), whole_आकार.width), col2 = std::max(0, std::min(ofs.x + cols + dright, whole_आकार.width));
    अगर(row1 > row2)
        std::swap(row1, row2);
    अगर(col1 > col2)
        std::swap(col1, col2);

    offset += (row1 - ofs.y)*step + (col1 - ofs.x)*esz;
    rows = row2 - row1; cols = col2 - col1;
    size.p[0] = rows; size.p[1] = cols;
    updateContinuityFlag();
    वापस *यह;
}


UMat UMat::reshape(पूर्णांक new_cn, पूर्णांक new_rows) स्थिर
{
    पूर्णांक cn = channels();
    UMat hdr = *यह;

    अगर( dims > 2 && new_rows == 0 && new_cn != 0 && size[dims-1]*cn % new_cn == 0 )
    {
        hdr.flags = (hdr.flags & ~CV_MAT_CN_MASK) | ((new_cn-1) << CV_CN_SHIFT);
        hdr.step[dims-1] = CV_ELEM_आकार(hdr.flags);
        hdr.size[dims-1] = hdr.size[dims-1]*cn / new_cn;
        वापस hdr;
    }

    CV_Assert( dims <= 2 );

    अगर( new_cn == 0 )
        new_cn = cn;

    पूर्णांक योग_width = cols * cn;

    अगर( (new_cn > योग_width || योग_width % new_cn != 0) && new_rows == 0 )
        new_rows = rows * योग_width / new_cn;

    अगर( new_rows != 0 && new_rows != rows )
    {
        पूर्णांक योग_आकार_2 = योग_width * rows;
        अगर( !isContinuous() )
            CV_Error( cv::Error::BadStep,
            "The matrix is not continuous, thus its number of rows can not be changed" );

        योग_width = योग_आकार_2 / new_rows;

        अगर( योग_width * new_rows != योग_आकार_2 )
            CV_Error( cv::Error::StsBadArg, "The total number of matrix elements "
                                    "is not divisible by the new number of rows" );

        hdr.size[0] = hdr.rows = new_rows;
        hdr.step[0] = योग_width * elemSize1();
    } अन्यथा {
        hdr.size[0] = hdr.rows = rows;
        अगर (dims <= 1)
            hdr.step[0] = cols * CV_ELEM_आकार(flags);
    }

    पूर्णांक new_width = योग_width / new_cn;

    अगर( new_width * new_cn != योग_width )
        CV_Error( cv::Error::Bad_अंक_Channels,
        "The total width is not divisible by the new number of channels" );

    hdr.size.dims = hdr.dims = 2;
    hdr.size[1] = hdr.cols = new_width;
    hdr.flags = (hdr.flags & ~CV_MAT_CN_MASK) | ((new_cn-1) << CV_CN_SHIFT);
    hdr.step[1] = CV_ELEM_आकार(hdr.flags);
    वापस hdr;
}

#अगर_घोषित HAVE_OPENCL
namespace {
अटल bool ocl_setDiag(स्थिर UMat& d, UMat& m, पूर्णांक len)
{
    पूर्णांक cn = d.channels();
    पूर्णांक depth = d.depth();

    अगर (depth == CV_64F && !ocl::Device::getDefault().doubleFPConfig())
        वापस false;

    String opts = format("-D SET_DIAG -D T1=%s -D cn=%d -D IS_ROW_VECTOR=%d",
                         ocl::memopTypeToStr(depth),
                         cn,
                         (d.rows == 1) ? 1 : 0);

    ocl::Kernel k("setDiag", ocl::core::copyset_oclsrc, opts);
    अगर (k.empty())
        वापस false;

    k.args(ocl::KernelArg::WriteOnly(m),
           ocl::KernelArg::Read_Only_No_आकार(d),
           len);

    माप_प्रकार globalsize[2] = { (माप_प्रकार)len, (माप_प्रकार)len };
    वापस k.run(2, globalsize, शून्य, false);
}
}
#पूर्ण_अगर

UMat UMat::diag(स्थिर UMat& d, UMatUsageFlags usageFlags)
{
    CV_INSTRUMENT_REGION();

    CV_Assert(d.cols == 1 || d.rows == 1);
    पूर्णांक len = d.rows + d.cols - 1;

#अगर_घोषित HAVE_OPENCL
    अगर (ocl::useOpenCL())
    {
        UMat m(len, len, d.type(), usageFlags);
        अगर (ocl_setDiag(d, m, len))
        {
            CV_IMPL_ADD(CV_IMPL_OCL);
            वापस m;
        }
    }
#पूर्ण_अगर

    UMat m(len, len, d.type(), Scalar(0), usageFlags);
    UMat md = m.diag();
    अगर( d.cols == 1 )
        d.copyTo(md);
    अन्यथा
        transpose(d, md);
    वापस m;
}

पूर्णांक UMat::checkVector(पूर्णांक _elemChannels, पूर्णांक _depth, bool _requireContinuous) स्थिर
{
    वापस (depth() == _depth || _depth <= 0) &&
        (isContinuous() || !_requireContinuous) &&
        ((dims <= 2 && (((rows == 1 || cols == 1) && channels() == _elemChannels) ||
                        (cols == _elemChannels && channels() == 1))) ||
        (dims == 3 && channels() == 1 && size.p[2] == _elemChannels && (size.p[0] == 1 || size.p[1] == 1) &&
         (isContinuous() || step.p[1] == step.p[2]*size.p[2])))
    ? (पूर्णांक)(योग()*channels()/_elemChannels) : -1;
}

UMat UMat::reshape(पूर्णांक _cn, पूर्णांक _newndims, स्थिर पूर्णांक* _newsz) स्थिर
{
    अगर(_newndims == dims)
    {
        अगर(_newsz == 0)
            वापस reshape(_cn);
        अगर(_newndims == 2)
            वापस reshape(_cn, _newsz[0]);
    }

    अगर (isContinuous())
    {
        CV_Assert(_cn >= 0 && _newndims >= 0 && _newndims <= CV_MAX_DIM && (_newndims == 0 || _newsz != 0));

        अगर (_cn == 0)
            _cn = यह->channels();
        अन्यथा
            CV_Assert(_cn <= CV_CN_MAX);

        माप_प्रकार योग_elem1_ref = यह->योग() * यह->channels();
        माप_प्रकार योग_elem1 = _cn;

        Auto_बफर<पूर्णांक, 4> newsz_buf( (माप_प्रकार)std::max(_newndims, 1) );

        पूर्णांक m1_idx = -1;

        क्रम (पूर्णांक i = 0; i < _newndims; i++)
        {
            अगर (_newsz[i] >= 0) {
                अगर (_newsz[i] == 0 && i < dims)
                    newsz_buf[i] = size.p[i];
                अन्यथा
                    newsz_buf[i] = _newsz[i];
                योग_elem1 *= (माप_प्रकार)newsz_buf[i];
            } अन्यथा {
                अगर (m1_idx >= 0)
                    CV_Error(cv::Error::Sts_Bad_आकार, "More than one '-1' occured in the new shape");
                m1_idx = i;
            }
        }

        अगर (m1_idx >= 0) {
            अगर (योग_elem1 == 0) {
                CV_Assert(योग_elem1_ref == 0);
                योग_elem1 = 1;
            }
            CV_Assert(योग_elem1_ref % योग_elem1 == 0);
            newsz_buf[m1_idx] = (पूर्णांक)(योग_elem1_ref / योग_elem1);
        } अन्यथा अगर (योग_elem1 != योग_elem1_ref) {
            CV_Error(cv::Error::StsUnmatchedSizes, "Requested and source matrices have different count of elements");
        }

        UMat hdr = *यह;
        hdr.flags = (hdr.flags & ~CV_MAT_CN_MASK) | ((_cn-1) << CV_CN_SHIFT);
        set_आकार(hdr, _newndims, newsz_buf.तथ्य(), शून्य, true);

        वापस hdr;
    }

    CV_Error(cv::Error::StsNotImplemented, "Reshaping of n-dimensional non-continuous matrices is not supported yet");
}

UMat UMat::reshape(पूर्णांक _cn, स्थिर MatShape& _newshape) स्थिर
{
    अगर (_newshape.dims < 0) {
        पूर्णांक newshape[] = {0};
        वापस reshape(_cn, 1, newshape);
    }
    वापस reshape(_cn, _newshape.dims, _newshape.p);
}

Mat UMat::getMat(AccessFlag accessFlags) स्थिर
{
    अगर(!u)
        वापस Mat();
    // TODO Support ACCESS_READ (ACCESS_WRITE) without unnecessary data transfers
    accessFlags |= ACCESS_RW;
    UMat_तथ्य_Auto_Lock autolock(u);
    प्रयास
    {
        अगर(CV_XADD(&u->refcount, 1) == 0)
            u->currAllocator->map(u, accessFlags);
        अगर (u->तथ्य != 0)
        {
            Mat hdr(dims, size.p, type(), u->तथ्य + offset, step.p);
            hdr.flags = flags;
            hdr.u = u;
            hdr.datastart = u->तथ्य;
            hdr.तथ्य = u->तथ्य + offset;
            hdr.datalimit = hdr.dataend = u->तथ्य + u->size;
            वापस hdr;
        }
    }
    पकड़ो(...)
    {
        CV_XADD(&u->refcount, -1);
        फेंको;
    }
    CV_XADD(&u->refcount, -1);
    CV_Assert(u->तथ्य != 0 && "Error mapping of UMat to host memory.");
    वापस Mat();
}

व्योम* UMat::handle(AccessFlag accessFlags) स्थिर
{
    अगर( !u )
        वापस 0;

    CV_Assert(u->refcount == 0);
    CV_Assert(!u->deviceCopyObsolete() || u->copyOnMap());
    अगर (u->deviceCopyObsolete())
    {
        u->currAllocator->unmap(u);
    }

    अगर (!!(accessFlags & ACCESS_WRITE))
        u->markHostCopyObsolete(true);

    वापस u->handle;
}

व्योम UMat::ndoffset(माप_प्रकार* ofs) स्थिर
{
    // offset = step[0]*ofs[0] + step[1]*ofs[1] + step[2]*ofs[2] + ...;
    माप_प्रकार val = offset;
    क्रम( पूर्णांक i = 0; i < dims; i++ )
    {
        माप_प्रकार s = step.p[i];
        ofs[i] = val / s;
        val -= ofs[i]*s;
    }
}

व्योम UMat::copyTo(OutputArray _dst) स्थिर
{
    CV_INSTRUMENT_REGION();

#अगर_घोषित HAVE_CUDA
    अगर (_dst.isGpuMat())
    {
        _dst.getGpuMat().upload(*यह);
        वापस;
    }
#पूर्ण_अगर

    पूर्णांक stype = type();
    पूर्णांक dtype = _dst.type();
    अगर( _dst.fixedType() && dtype != stype )
    {
        CV_Assert( CV_MAT_CN(stype) == CV_MAT_CN(dtype) );
        convertTo( _dst, dtype );
        वापस;
    }

    अगर( dims == 0 && empty() )
    {
        _dst.release();
        व्योम* obj = _dst.getObj();
        अगर (_dst.isMat())
            reinterpret_cast<Mat*>(obj)->flags = Mat::MAGIC_VAL | Mat::CONTINUOUS_FLAG | stype;
        अन्यथा अगर (_dst.isUMat())
            reinterpret_cast<UMat*>(obj)->flags = UMat::MAGIC_VAL | UMat::CONTINUOUS_FLAG | stype;
        अन्यथा अगर (_dst.isGpuMat())
            reinterpret_cast<cuda::GpuMat*>(obj)->flags = stype;
        वापस;
    }

    _dst.create( dims, size.p, stype );
    अगर (empty())
        वापस;

    माप_प्रकार sz[CV_MAX_DIM] = {1}, srcofs[CV_MAX_DIM]={0}, dstofs[CV_MAX_DIM]={0};
    माप_प्रकार esz = CV_ELEM_आकार(stype);
    पूर्णांक i, d = std::max(dims, 1);
    क्रम( i = 0; i < d; i++ )
        sz[i] = size.p[i];
    sz[d-1] *= esz;
    ndoffset(srcofs);
    srcofs[d-1] *= esz;

    अगर( _dst.isUMat() )
    {
        UMat dst = _dst.getUMat();
        CV_Assert(dst.u);
        अगर( u == dst.u && dst.offset == offset )
            वापस;

        अगर (u->currAllocator == dst.u->currAllocator)
        {
            dst.ndoffset(dstofs);
            dstofs[dims-1] *= esz;
            u->currAllocator->copy(u, dst.u, d, sz, srcofs, step.p, dstofs, dst.step.p, false);
            वापस;
        }

        // different allocators: never map a device destination, upload into it instead
        // (through a host Mat when both sides are device memory)
        MatAllocator* hostAllocator = Mat::getDefaultAllocator();
        अगर (dst.u->currAllocator != hostAllocator)
        {
            अगर (u->currAllocator == hostAllocator)
                getMat(ACCESS_READ).copyTo(_dst);
            अन्यथा
            {
                Mat tmp;
                copyTo(tmp);
                tmp.copyTo(_dst);
            }
            वापस;
        }
    }

    Mat dst = _dst.getMat();
    u->currAllocator->download(u, dst.ptr(), d, sz, srcofs, step.p, dst.step.p);
}

व्योम UMat::copyTo(OutputArray _dst, निवेश_Array _mask) स्थिर
{
    CV_INSTRUMENT_REGION();

    अगर( _mask.empty() )
    {
        copyTo(_dst);
        वापस;
    }
#अगर_घोषित HAVE_OPENCL
    पूर्णांक cn = channels(), mtype = _mask.type(), mdepth = CV_MAT_DEPTH(mtype), mcn = CV_MAT_CN(mtype);
    CV_Assert( (mdepth == CV_8U || mdepth == CV_8S || mdepth == CV_Bool) && (mcn == 1 || mcn == cn) );

    अगर (ocl::useOpenCL() && _dst.isUMat() && dims <= 2)
    {
        UMat_तथ्य * prevu = _dst.getUMat().u;
        _dst.create( size, type() );

        UMat dst = _dst.getUMat();

        bool haveDstUninit = false;
        अगर( prevu != dst.u ) // do not leave dst uninitialized
            haveDstUninit = true;

        String opts = format("-D COPY_TO_MASK -D T1=%s -D scn=%d -D mcn=%d%s",
                             ocl::memopTypeToStr(depth()), cn, mcn,
                             haveDstUninit ? " -D HAVE_DST_UNINIT" : "");

        ocl::Kernel k("copyToMask", ocl::core::copyset_oclsrc, opts);
        अगर (!k.empty())
        {
            k.args(ocl::KernelArg::Read_Only_No_आकार(*यह),
                   ocl::KernelArg::Read_Only_No_आकार(_mask.getUMat()),
                   haveDstUninit ? ocl::KernelArg::WriteOnly(dst) :
                                   ocl::KernelArg::ReadWrite(dst));

            माप_प्रकार globalsize[2] = { (माप_प्रकार)cols, (माप_प्रकार)rows };
            अगर (k.run(2, globalsize, शून्य, false))
            {
                CV_IMPL_ADD(CV_IMPL_OCL);
                वापस;
            }
        }
    }
#पूर्ण_अगर
    Mat src = getMat(ACCESS_READ);
    src.copyTo(_dst, _mask);
}


//
// void UMat::convertTo moved to convert.dispatch.cpp
//

UMat& UMat::setTo(निवेश_Array मूल्य, निवेश_Array _mask)
{
    CV_INSTRUMENT_REGION();

    bool haveMask = !_mask.empty();
#अगर_घोषित HAVE_OPENCL
    पूर्णांक tp = type(), cn = CV_MAT_CN(tp), d = CV_MAT_DEPTH(tp);

    अगर( dims <= 2 && cn <= 4 && CV_MAT_DEPTH(tp) < CV_64F && ocl::useOpenCL() )
    {
        Mat value = मूल्य.getMat();
        CV_Assert( checkScalar(value, type(), मूल्य.kind(), निवेश_Array_2::UMAT) );
        पूर्णांक kercn = haveMask || cn == 3 ? cn : std::max(cn, ocl::predictOptimalVectorWidth(*यह)),
                kertp = CV_MAKE_TYPE(d, kercn);

        द्विगुन buf[16] = { 0, 0, 0, 0, 0, 0, 0, 0,
                           0, 0, 0, 0, 0, 0, 0, 0 };
        convertAndUnrollScalar(value, tp, (uchar *)buf, kercn / cn);

        पूर्णांक scalarcn = kercn == 3 ? 4 : kercn, rowsPerWI = ocl::Device::getDefault().isIntel() ? 4 : 1;
        String opts = format("-D dstT=%s -D rowsPerWI=%d -D dstST=%s -D dstT1=%s -D cn=%d",
                             ocl::memopTypeToStr(kertp), rowsPerWI,
                             ocl::memopTypeToStr(CV_MAKETYPE(d, scalarcn)),
                             ocl::memopTypeToStr(d), kercn);

        ocl::Kernel setK(haveMask ? "setMask" : "set", ocl::core::copyset_oclsrc, opts);
        अगर( !setK.empty() )
        {
            ocl::KernelArg scalararg(ocl::KernelArg::CONSTANT, 0, 0, 0, buf, CV_ELEM_आकार(d) * scalarcn);
            UMat mask;

            अगर( haveMask )
            {
                mask = _mask.getUMat();
                CV_Assert( mask.size() == size() && (mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool) );
                ocl::KernelArg maskarg = ocl::KernelArg::Read_Only_No_आकार(mask),
                        dstarg = ocl::KernelArg::ReadWrite(*यह);
                setK.args(maskarg, dstarg, scalararg);
            }
            अन्यथा
            {
                ocl::KernelArg dstarg = ocl::KernelArg::WriteOnly(*यह, cn, kercn);
                setK.args(dstarg, scalararg);
            }

            माप_प्रकार globalsize[] = { (माप_प्रकार)cols * cn / kercn, ((माप_प्रकार)rows + rowsPerWI - 1) / rowsPerWI };
            अगर( setK.run(2, globalsize, शून्य, false) )
            {
                CV_IMPL_ADD(CV_IMPL_OCL);
                वापस *यह;
            }
        }
    }
#पूर्ण_अगर
    Mat m = getMat(haveMask ? ACCESS_RW : ACCESS_WRITE);
    m.setTo(मूल्य, _mask);
    वापस *यह;
}

UMat& UMat::चालक = (स्थिर Scalar& s)
{
    setTo(s);
    वापस *यह;
}

UMat UMat::t() स्थिर
{
    UMat m;
    transpose(*यह, m);
    वापस m;
}

UMat UMat::zeros(पूर्णांक rows, पूर्णांक cols, पूर्णांक type, UMatUsageFlags usageFlags)
{
    वापस UMat(rows, cols, type, Scalar::all(0), usageFlags);
}

UMat UMat::zeros(आकार size, पूर्णांक type, UMatUsageFlags usageFlags)
{
    वापस UMat(size, type, Scalar::all(0), usageFlags);
}

UMat UMat::zeros(पूर्णांक ndims, स्थिर पूर्णांक* sz, पूर्णांक type, UMatUsageFlags usageFlags)
{
    वापस UMat(ndims, sz, type, Scalar::all(0), usageFlags);
}

UMat UMat::zeros(स्थिर MatShape& shape, पूर्णांक type, UMatUsageFlags usageFlags)
{
    वापस UMat(shape.dims, shape.p, type, Scalar::all(0), usageFlags);
}

UMat UMat::ones(पूर्णांक rows, पूर्णांक cols, पूर्णांक type, UMatUsageFlags usageFlags)
{
    वापस UMat(rows, cols, type, Scalar(1), usageFlags);
}

UMat UMat::ones(आकार size, पूर्णांक type, UMatUsageFlags usageFlags)
{
    वापस UMat(size, type, Scalar(1), usageFlags);
}

UMat UMat::ones(पूर्णांक ndims, स्थिर पूर्णांक* sz, पूर्णांक type, UMatUsageFlags usageFlags)
{
    वापस UMat(ndims, sz, type, Scalar(1), usageFlags);
}

UMat UMat::ones(स्थिर MatShape& shape, पूर्णांक type, UMatUsageFlags usageFlags)
{
    वापस UMat(shape.dims, shape.p, type, Scalar(1), usageFlags);
}

}

/* End of file. */
