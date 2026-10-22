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
शून्य समुच्चयः_आकार(UMat& m, पूर्णाङ्क _dims, ध्रुव पूर्णाङ्क* _sz, ध्रुव size_t* चरणाः_2,
             तार्किक auto_चरणाः = असत्य);

शून्य updateContinuityFlag(UMat& m);
शून्य finalizeHdr(UMat& m);

UMat_दत्त::UMat_दत्त(ध्रुव MatAllocator* allocator)
{
    पूर्व_Allocator = currAllocator = allocator;
    urefcount = refcount = mapcount = 0;
    दत्त = origdata = 0;
    size = 0;
    flags = static_cast<UMat_दत्त::स्मृतिः_Flag>(0);
    handle = 0;
    userdata = 0;
    allocatorFlags_ = 0;
    original_UMat_दत्त = रिक्त;
}

UMat_दत्त::~UMat_दत्त()
{
    पूर्व_Allocator = currAllocator = 0;
    urefcount = refcount = 0;
    CV_Assert(mapcount == 0);
    दत्त = origdata = 0;
    size = 0;
    तार्किक isAsyncCleanup = !!(flags & UMat_दत्त::ASYNC_CLEANUP);
    flags = static_cast<UMat_दत्त::स्मृतिः_Flag>(0);
    handle = 0;
    userdata = 0;
    allocatorFlags_ = 0;
    यदि (original_UMat_दत्त)
    {
        तार्किक showWarn = असत्य;
        UMat_दत्त* u = original_UMat_दत्त;
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
#यदि_अपरिभाषितम् NDEBUG
        यदि (showWarn)
        {
            स्थिर पूर्णाङ्क warn_सन्देशः_showed = 0;
            यदि (warn_सन्देशः_showed++ < 100)
            {
                fflush(stdout);
                fprintf(stderr, "\n! OPENCV warning: getUMat()/getMat() call chain possible problem."
                                "\n!                 Base object is dead, while nested/derived object is still alive or processed."
                                "\n!                 Please check lifetime of UMat/Mat objects!\n");
                fflush(stderr);
            }
        }
#अन्यथा
        CV_अप्रयुक्तम्(showWarn);
#यदि_अन्तः
        original_UMat_दत्त = रिक्त;
    }
}

#यदि_अपरिभाषितम् OPENCV_DISABLE_THREAD_SUPPORT

// it should be a prime number for the best hash function
गणना { UMAT_NLOCKS = 31 };
स्थिर Mutex umatLocks[UMAT_NLOCKS];

स्थिर size_t get_UMat_दत्त_तालकम्_सूचकांक(ध्रुव UMat_दत्त* u)
{
    size_t idx = ((size_t)(शून्य*)u) % UMAT_NLOCKS;
    प्रत्यागम idx;
}

शून्य UMat_दत्त::lock()
{
    size_t idx = get_UMat_दत्त_तालकम्_सूचकांक(एतत्);
    //printf("%d lock(%d)\n", cv::utils::getThreadID(), (int)idx);
    umatLocks[idx].lock();
}

शून्य UMat_दत्त::unlock()
{
    size_t idx = get_UMat_दत्त_तालकम्_सूचकांक(एतत्);
    //printf("%d unlock(%d)\n", cv::utils::getThreadID(), (int)idx);
    umatLocks[idx].unlock();
}


// Do not allow several lock() calls with different UMatData objects.
संरचना UMat_दत्त_Auto_Locker
{
    पूर्णाङ्क usage_गणनम्;
    UMat_दत्त* locked_objects[2];
    UMat_दत्त_Auto_Locker() : usage_गणनम्(0) { locked_objects[0] = रिक्त; locked_objects[1] = रिक्त; }

    शून्य lock(UMat_दत्त*& u1)
    {
        तार्किक locked_1 = (u1 == locked_objects[0] || u1 == locked_objects[1]);
        यदि (locked_1)
        {
            u1 = रिक्त;
            प्रत्यागम;
        }
        CV_Assert(usage_गणनम् == 0);  // UMatDataAutoLock can't be used multiple times from the same thread
        usage_गणनम् = 1;
        locked_objects[0] = u1;
        u1->lock();
    }
    शून्य lock(UMat_दत्त*& u1, UMat_दत्त*& u2)
    {
        तार्किक locked_1 = (u1 == locked_objects[0] || u1 == locked_objects[1]);
        तार्किक locked_2 = (u2 == locked_objects[0] || u2 == locked_objects[1]);
        यदि (locked_1)
            u1 = रिक्त;
        यदि (locked_2)
            u2 = रिक्त;
        यदि (locked_1 && locked_2)
            प्रत्यागम;
        CV_Assert(usage_गणनम् == 0);  // UMatDataAutoLock can't be used multiple times from the same thread
        usage_गणनम् = 1;
        locked_objects[0] = u1;
        locked_objects[1] = u2;
        यदि (u1)
            u1->lock();
        यदि (u2)
            u2->lock();
    }
    शून्य release(UMat_दत्त* u1, UMat_दत्त* u2)
    {
        यदि (u1 == रिक्त && u2 == रिक्त)
            प्रत्यागम;
        CV_Assert(usage_गणनम् == 1);
        usage_गणनम् = 0;
        यदि (u1)
            u1->unlock();
        यदि (u2)
            u2->unlock();
        locked_objects[0] = रिक्त; locked_objects[1] = रिक्त;
    }
};
स्थिर TLSData<UMat_दत्त_Auto_Locker>& get_UMat_दत्त_Auto_Locker_TLS()
{
    CV_SINGLETON_LAZY_INIT_REF(TLSData<UMat_दत्त_Auto_Locker>, नूतन TLSData<UMat_दत्त_Auto_Locker>());
}
स्थिर UMat_दत्त_Auto_Locker& get_UMat_दत्त_Auto_Locker() { प्रत्यागम get_UMat_दत्त_Auto_Locker_TLS().getRef(); }


UMat_दत्त_Auto_तालकम्::UMat_दत्त_Auto_तालकम्(UMat_दत्त* u) : u1(u), u2(रिक्त)
{
    get_UMat_दत्त_Auto_Locker().lock(u1);
}
UMat_दत्त_Auto_तालकम्::UMat_दत्त_Auto_तालकम्(UMat_दत्त* u1_, UMat_दत्त* u2_) : u1(u1_), u2(u2_)
{
    यदि (get_UMat_दत्त_तालकम्_सूचकांक(u1) > get_UMat_दत्त_तालकम्_सूचकांक(u2))
    {
        std::swap(u1, u2);
    }
    get_UMat_दत्त_Auto_Locker().lock(u1, u2);
}
UMat_दत्त_Auto_तालकम्::~UMat_दत्त_Auto_तालकम्()
{
    get_UMat_दत्त_Auto_Locker().release(u1, u2);
}

#अन्यथा

शून्य UMat_दत्त::lock()
{
    // nothing in OPENCV_DISABLE_THREAD_SUPPORT mode
}

शून्य UMat_दत्त::unlock()
{
    // nothing in OPENCV_DISABLE_THREAD_SUPPORT mode
}

UMat_दत्त_Auto_तालकम्::UMat_दत्त_Auto_तालकम्(UMat_दत्त* u) : u1(u), u2(रिक्त)
{
    // nothing in OPENCV_DISABLE_THREAD_SUPPORT mode
}
UMat_दत्त_Auto_तालकम्::UMat_दत्त_Auto_तालकम्(UMat_दत्त* u1_, UMat_दत्त* u2_) : u1(u1_), u2(u2_)
{
    // nothing in OPENCV_DISABLE_THREAD_SUPPORT mode
}
UMat_दत्त_Auto_तालकम्::~UMat_दत्त_Auto_तालकम्()
{
    // nothing in OPENCV_DISABLE_THREAD_SUPPORT mode
}

#यदि_अन्तः  // OPENCV_DISABLE_THREAD_SUPPORT

//////////////////////////////// UMat ////////////////////////////////

UMat::UMat(UMatUsageFlags _usageFlags) CV_NOEXCEPT
: flags(MAGIC_VAL), dims(0), पङ्क्तिकाः(0), स्तम्भसङ्ख्या(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{}

UMat::UMat(पूर्णाङ्क पङ्क्तिकाः_3, पूर्णाङ्क स्तम्भसङ्ख्या_3, पूर्णाङ्क _type, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), पङ्क्तिकाः(0), स्तम्भसङ्ख्या(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(पङ्क्तिकाः_3, स्तम्भसङ्ख्या_3, _type);
}

UMat::UMat(पूर्णाङ्क पङ्क्तिकाः_3, पूर्णाङ्क स्तम्भसङ्ख्या_3, पूर्णाङ्क _type, ध्रुव Scalar& _s, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), पङ्क्तिकाः(0), स्तम्भसङ्ख्या(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(पङ्क्तिकाः_3, स्तम्भसङ्ख्या_3, _type);
    *एतत् = _s;
}

UMat::UMat(आकार _sz, पूर्णाङ्क _type, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), पङ्क्तिकाः(0), स्तम्भसङ्ख्या(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create( _sz.height, _sz.विस्तारः, _type );
}

UMat::UMat(आकार _sz, पूर्णाङ्क _type, ध्रुव Scalar& _s, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), पङ्क्तिकाः(0), स्तम्भसङ्ख्या(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_sz.height, _sz.विस्तारः, _type);
    *एतत् = _s;
}

UMat::UMat(पूर्णाङ्क _dims, ध्रुव पूर्णाङ्क* _sz, पूर्णाङ्क _type, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), पङ्क्तिकाः(0), स्तम्भसङ्ख्या(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_dims, _sz, _type);
}

UMat::UMat(पूर्णाङ्क _dims, ध्रुव पूर्णाङ्क* _sz, पूर्णाङ्क _type, ध्रुव Scalar& _s, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), पङ्क्तिकाः(0), स्तम्भसङ्ख्या(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_dims, _sz, _type);
    *एतत् = _s;
}

UMat::UMat(ध्रुव MatShape& _shape, पूर्णाङ्क _type, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), पङ्क्तिकाः(0), स्तम्भसङ्ख्या(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_shape.dims, _shape.p, _type);
}

UMat::UMat(ध्रुव MatShape& _shape, पूर्णाङ्क _type, ध्रुव Scalar& _s, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), पङ्क्तिकाः(0), स्तम्भसङ्ख्या(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_shape.dims, _shape.p, _type);
    *एतत् = _s;
}

UMat::UMat(ध्रुव UMat& m)
: flags(m.flags), dims(m.dims), पङ्क्तिकाः(m.पङ्क्तिकाः), स्तम्भसङ्ख्या(m.स्तम्भसङ्ख्या), allocator(m.allocator),
  usageFlags(m.usageFlags), u(m.u), offset(m.offset), size(m.size), चरणः(m.चरणः)
{
    addref();
}

UMat& UMat::संक्रियकः=(ध्रुव UMat& m)
{
    यदि( एतत् != &m )
    {
        const_cast<UMat&>(m).addref();
        release();
        flags = m.flags;
        dims = m.dims;
        पङ्क्तिकाः = m.पङ्क्तिकाः;
        स्तम्भसङ्ख्या = m.स्तम्भसङ्ख्या;
        size = m.size;
        चरणः = m.चरणः;
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
    copy_गन्तव्यम्(m);
    प्रत्यागम m;
}

शून्य UMat::नियोजय_गन्तव्यम्(UMat& m, पूर्णाङ्क _type) ध्रुव
{
    यदि( _type < 0 )
        m = *एतत्;
    अन्यथा
        convert_गन्तव्यम्(m, _type);
}

शून्य UMat::create(पूर्णाङ्क पङ्क्तिकाः_3, पूर्णाङ्क स्तम्भसङ्ख्या_3, पूर्णाङ्क _type, UMatUsageFlags _usageFlags)
{
    पूर्णाङ्क sz[] = {पङ्क्तिकाः_3, स्तम्भसङ्ख्या_3};
    create(2, sz, _type, _usageFlags);
}

शून्य UMat::create(आकार _sz, पूर्णाङ्क _type, UMatUsageFlags _usageFlags)
{
    create(_sz.height, _sz.विस्तारः, _type, _usageFlags);
}

शून्य UMat::create_Same_आकार(निवेशः_Array arr, पूर्णाङ्क _type, UMatUsageFlags _usageFlags)
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
    dims = स्तम्भसङ्ख्या = पङ्क्तिकाः = 0;
}

तार्किक UMat::empty() ध्रुव
{
    प्रत्यागम u == 0 || योग() == 0;
}

size_t UMat::योग() ध्रुव
{
    यदि( dims <= 2 )
        प्रत्यागम (size_t)पङ्क्तिकाः * स्तम्भसङ्ख्या;
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
: flags(m.flags), dims(m.dims), पङ्क्तिकाः(m.पङ्क्तिकाः), स्तम्भसङ्ख्या(m.स्तम्भसङ्ख्या), allocator(m.allocator),
  usageFlags(m.usageFlags), u(m.u), offset(m.offset)
{
    size = m.size;
    चरणः = m.चरणः;
    m.flags = MAGIC_VAL;
    m.usageFlags = USAGE_DEFAULT;
    m.dims = m.पङ्क्तिकाः = m.स्तम्भसङ्ख्या = 0;
    m.size.clear();
    m.allocator = रिक्त;
    m.u = रिक्त;
    m.offset = 0;
}

UMat& UMat::संक्रियकः=(UMat&& m)
{
    यदि (एतत् == &m)
      प्रत्यागम *एतत्;
    release();
    flags = m.flags; dims = m.dims; पङ्क्तिकाः = m.पङ्क्तिकाः; स्तम्भसङ्ख्या = m.स्तम्भसङ्ख्या;
    allocator = m.allocator; usageFlags = m.usageFlags;
    u = m.u;
    offset = m.offset;
    size = m.size;
    चरणः = m.चरणः;
    m.flags = MAGIC_VAL;
    m.usageFlags = USAGE_DEFAULT;
    m.dims = m.पङ्क्तिकाः = m.स्तम्भसङ्ख्या = 0;
    m.allocator = रिक्त;
    m.u = रिक्त;
    m.offset = 0;
    प्रत्यागम *एतत्;
}


MatAllocator* UMat::getStdAllocator()
{
#यदि_परिभाषितम् HAVE_OPENCL
    यदि (ocl::useOpenCL())
        प्रत्यागम ocl::getOpenCLAllocator();
#यदि_अन्तः
    प्रत्यागम Mat::getDefaultAllocator();
}

शून्य swap( UMat& a, UMat& b )
{
    std::swap(a.flags, b.flags);
    std::swap(a.dims, b.dims);
    std::swap(a.पङ्क्तिकाः, b.पङ्क्तिकाः);
    std::swap(a.स्तम्भसङ्ख्या, b.स्तम्भसङ्ख्या);
    std::swap(a.allocator, b.allocator);
    std::swap(a.u, b.u);
    std::swap(a.offset, b.offset);

    std::swap(a.size, b.size);
    std::swap(a.चरणः, b.चरणः);
}


शून्य समुच्चयः_आकार( UMat& m, पूर्णाङ्क _dims, ध्रुव पूर्णाङ्क* _sz,
              ध्रुव size_t* चरणाः_2, तार्किक auto_चरणाः )
{
    CV_Assert( 0 <= _dims && _dims <= CV_MAX_DIM && _dims <= MatShape::MAX_DIMS);

    m.dims = _dims;
    m.size = MatShape(_dims, _sz);
    m.चरणः[std::max(_dims-1, 0)] = CV_ELEM_आकार(m.flags);
    प्रति (पूर्णाङ्क i = _dims-2; i >= 0; i--) {
        size_t autostep = m.size[i+1]*m.चरणः[i+1];
        यदि (चरणाः_2) {
            m.चरणः[i] = चरणाः_2[i];
            //CV_Assert(m.step[i] >= autostep);
        } अन्यथा यदि (auto_चरणाः) {
            m.चरणः[i] = autostep;
        } अन्यथा {
            m.चरणः[i] = 0;
        }
    }

    यदि( _dims <= 2 )
    {
        m.स्तम्भसङ्ख्या = _dims == 0 ? 1 : _sz ? _sz[_dims > 1] : 0;
        m.पङ्क्तिकाः = _dims < 2 ? 1 : _sz ? _sz[0] : 0;
    } अन्यथा {
        m.स्तम्भसङ्ख्या = m.पङ्क्तिकाः = -1;
    }
}


शून्य UMat::updateContinuityFlag()
{
    flags = cv::updateContinuityFlag(flags, dims, size.p, चरणः.p);
}


शून्य finalizeHdr(UMat& m)
{
    m.updateContinuityFlag();
    पूर्णाङ्क d = m.dims;
    यदि( d > 2 )
        m.पङ्क्तिकाः = m.स्तम्भसङ्ख्या = -1;
}


UMat Mat::getUMat(AccessFlag accessFlags, UMatUsageFlags usageFlags) ध्रुव
{
    UMat hdr;
    यदि(!दत्त)
        प्रत्यागम hdr;
    यदि (दत्त != datastart)
    {
        आकार सम्पूर्णम्_आकार;
        Point ofs;
        locateROI(सम्पूर्णम्_आकार, ofs);
        आकार sz(स्तम्भसङ्ख्या, पङ्क्तिकाः);
        यदि (ofs.x != 0 || ofs.y != 0)
        {
            Mat src = *एतत्;
            पूर्णाङ्क dtop = ofs.y;
            पूर्णाङ्क dbottom = सम्पूर्णम्_आकार.height - src.पङ्क्तिकाः - ofs.y;
            पूर्णाङ्क dleft = ofs.x;
            पूर्णाङ्क dright = सम्पूर्णम्_आकार.विस्तारः - src.स्तम्भसङ्ख्या - ofs.x;
            src.adjustROI(dtop, dbottom, dleft, dright);
            प्रत्यागम src.getUMat(accessFlags, usageFlags)(cv::Rect(ofs.x, ofs.y, sz.विस्तारः, sz.height));
        }
    }
    CV_Assert(दत्त == datastart);

    accessFlags |= ACCESS_RW;
    UMat_दत्त* new_u = रिक्त;
    Mat_चरणः new_चरणः = चरणः;
    {
        MatAllocator *a = allocator, *a0 = getDefaultAllocator();
        यदि(!a)
            a = a0;
        new_u = a->allocate(dims, size.p, type(), दत्त, new_चरणः.p, accessFlags, usageFlags);
        new_u->original_UMat_दत्त = u;
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
#यदि_परिभाषितम् HAVE_OPENCL
        यदि (ocl::useOpenCL() && new_u->currAllocator == ocl::getOpenCLAllocator())
        {
            CV_Assert(new_u->अस्थायी_UMat());
        }
#यदि_अन्तः
        CV_XADD(&(u->refcount), 1);
        CV_XADD(&(u->urefcount), 1);
    }
    यत्न
    {
        hdr.flags = flags;
        hdr.usageFlags = usageFlags;
        समुच्चयः_आकार(hdr, dims, size.p, new_चरणः.p);
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
    flags = (_type & CV_MAT_TYPE_आवरणम्) | MAGIC_VAL;
    समुच्चयः_आकार(*एतत्, d, _sizes, 0, सत्य);
    offset = 0;

    यदि( योग() > 0 )
    {
        MatAllocator *a = allocator, *a0 = getStdAllocator();
        यदि (!a)
        {
            a = a0;
            a0 = Mat::getDefaultAllocator();
        }
        यत्न
        {
            u = a->allocate(dims, size.p, _type, 0, चरणः.p, ACCESS_RW /* ignored */, usageFlags);
            CV_Assert(u != 0);
        }
        गृहाण(...)
        {
            यदि(a != a0)
                u = a0->allocate(dims, size.p, _type, 0, चरणः.p, ACCESS_RW /* ignored */, usageFlags);
            CV_Assert(u != 0);
        }
        CV_Assert( चरणः[dims-1] == (size_t)CV_ELEM_आकार(flags) );
    }

    finalizeHdr(*एतत्);
    addref();
    size.dims = dims = d0;
}

शून्य UMat::create(ध्रुव std::vector<पूर्णाङ्क>& _sizes, पूर्णाङ्क _type, UMatUsageFlags _usageFlags)
{
    create((पूर्णाङ्क)_sizes.size(), _sizes.दत्त(), _type, _usageFlags);
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
    size_t old_योग_अष्टकानि = u ? u->size : 0;
    size_t esz = CV_ELEM_आकार(_type), new_योग = _dims >= 0;
    प्रति (पूर्णाङ्क i = 0; i < _dims; i++)
        new_योग *= _sizes[i];
    size_t new_योग_अष्टकानि = new_योग*esz;
    यदि (new_योग_अष्टकानि > 0 &&
        (!isContinuous() ||
         new_योग_अष्टकानि > old_योग_अष्टकानि ||
         offset != 0 ||
         _usageFlags != usageFlags)) {
        create(_dims, _sizes, _type, _usageFlags);
    } अन्यथा {
        flags = (flags & ~Mat::TYPE_आवरणम्) | CV_MAT_TYPE(_type);
        पूर्णाङ्क dummy_आकार = 0;
        समुच्चयः_आकार(*एतत्, (_dims >= 0 ? _dims : 1), (_dims >= 0 ? _sizes : &dummy_आकार), शून्यसूचकः, सत्य);
        finalizeHdr(*एतत्);
    }
}

शून्य UMat::fit(ध्रुव std::vector<पूर्णाङ्क>& _shape, पूर्णाङ्क _type, UMatUsageFlags _usageFlags)
{
    fit((पूर्णाङ्क)_shape.size(), _shape.दत्त(), _type, _usageFlags);
}

शून्य UMat::fit(ध्रुव MatShape& _shape, पूर्णाङ्क _type, UMatUsageFlags _usageFlags)
{
    fit(_shape.dims, _shape.p, _type, _usageFlags);
    size.layout = _shape.layout;
    size.C = _shape.C;
}

शून्य UMat::fit(पूर्णाङ्क पङ्क्तिकाः_3, पूर्णाङ्क स्तम्भसङ्ख्या_3, पूर्णाङ्क _type, UMatUsageFlags _usageFlags)
{
    _type &= TYPE_आवरणम्;
    पूर्णाङ्क sz[] = {पङ्क्तिकाः_3, स्तम्भसङ्ख्या_3};
    fit(2, sz, _type, _usageFlags);
}

शून्य UMat::fit(आकार _sz, पूर्णाङ्क _type, UMatUsageFlags _usageFlags)
{
    fit(_sz.height, _sz.विस्तारः, _type, _usageFlags);
}

शून्य UMat::fit_Same_आकार(निवेशः_Array m, पूर्णाङ्क _type, UMatUsageFlags _usageFlags)
{
    fit(m.shape(), _type, _usageFlags);
}

शून्य UMat::copy_आकार(ध्रुव UMat& m)
{
    dims = m.dims;
    स्तम्भसङ्ख्या = m.स्तम्भसङ्ख्या;
    पङ्क्तिकाः = m.पङ्क्तिकाः;
    size = m.size;
    चरणः = m.चरणः;
}

UMat::~UMat()
{
    release();
}

शून्य UMat::deallocate()
{
    UMat_दत्त* u_ = u;
    u = रिक्त;
    u_->currAllocator->deallocate(u_);
}


UMat::UMat(ध्रुव UMat& m, ध्रुव Range& _rowRange, ध्रुव Range& _colRange)
    : flags(MAGIC_VAL), dims(0), पङ्क्तिकाः(0), स्तम्भसङ्ख्या(0), allocator(0), usageFlags(USAGE_DEFAULT), u(0), offset(0)
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
    यदि( _rowRange != Range::all() && _rowRange != Range(0,पङ्क्तिकाः) )
    {
        CV_Assert( 0 <= _rowRange.start &&
                  _rowRange.start <= _rowRange.end &&
                  _rowRange.end <= m.पङ्क्तिकाः );
        size[0] = पङ्क्तिकाः = _rowRange.size();
        offset += चरणः*_rowRange.start;
        flags |= SUBMATRIX_FLAG;
    }

    यदि( _colRange != Range::all() && _colRange != Range(0,स्तम्भसङ्ख्या) )
    {
        CV_Assert( 0 <= _colRange.start &&
                  _colRange.start <= _colRange.end &&
                  _colRange.end <= m.स्तम्भसङ्ख्या );
        size[1] = स्तम्भसङ्ख्या = _colRange.size();
        offset += _colRange.start*elem_आकार();
        flags |= SUBMATRIX_FLAG;
    }

    updateContinuityFlag();

    यदि( पङ्क्तिकाः <= 0 || स्तम्भसङ्ख्या <= 0 )
    {
        release();
        पङ्क्तिकाः = स्तम्भसङ्ख्या = 0;
    }
}


UMat::UMat(ध्रुव UMat& m, ध्रुव Rect& roi)
    : flags(m.flags), dims(2), पङ्क्तिकाः(roi.height), स्तम्भसङ्ख्या(roi.विस्तारः),
    allocator(m.allocator), usageFlags(m.usageFlags), u(m.u),
    offset(m.offset + roi.y*m.चरणः[0]), size(2)
{
    CV_Assert( m.dims <= 2 );

    size_t esz = CV_ELEM_आकार(flags);
    offset += roi.x*esz;
    CV_Assert( 0 <= roi.x && 0 <= roi.विस्तारः && roi.x + roi.विस्तारः <= m.स्तम्भसङ्ख्या &&
              0 <= roi.y && 0 <= roi.height && roi.y + roi.height <= m.पङ्क्तिकाः );
    यदि( roi.विस्तारः < m.स्तम्भसङ्ख्या || roi.height < m.पङ्क्तिकाः )
        flags |= SUBMATRIX_FLAG;

    size[0] = पङ्क्तिकाः;
    size[1] = स्तम्भसङ्ख्या;
    चरणः[0] = m.dims == 2 ? m.चरणः[0] : स्तम्भसङ्ख्या*esz;
    चरणः[1] = esz;

    updateContinuityFlag();

    addref();
    यदि( पङ्क्तिकाः <= 0 || स्तम्भसङ्ख्या <= 0 )
    {
        पङ्क्तिकाः = स्तम्भसङ्ख्या = 0;
        release();
    }
}


UMat::UMat(ध्रुव UMat& m, ध्रुव Range* ranges)
    : flags(MAGIC_VAL), dims(0), पङ्क्तिकाः(0), स्तम्भसङ्ख्या(0), allocator(0), usageFlags(USAGE_DEFAULT), u(0), offset(0)
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
            offset += r.start*चरणः.p[i];
            flags |= SUBMATRIX_FLAG;
        }
    }

    यदि (d <= 2) {
        पङ्क्तिकाः = d == 2 ? size[0] : 1;
        स्तम्भसङ्ख्या = d <= 0 ? (d >= 0) : size[d > 1];
    }
    updateContinuityFlag();
}

UMat::UMat(ध्रुव UMat& m, ध्रुव std::vector<Range>& ranges)
    : flags(MAGIC_VAL), dims(0), पङ्क्तिकाः(0), स्तम्भसङ्ख्या(0), allocator(0), usageFlags(USAGE_DEFAULT), u(0), offset(0)
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
            offset += r.start*चरणः.p[i];
            flags |= SUBMATRIX_FLAG;
        }
    }

    यदि (d <= 2) {
        पङ्क्तिकाः = d == 2 ? size[0] : 1;
        स्तम्भसङ्ख्या = d <= 0 ? (d >= 0) : size[d > 1];
    }
    updateContinuityFlag();
}

UMat UMat::diag(पूर्णाङ्क d) ध्रुव
{
    CV_Assert( dims <= 2 );
    UMat m = *एतत्;
    size_t esz = elem_आकार();
    पूर्णाङ्क len;

    यदि( d >= 0 )
    {
        len = std::min(स्तम्भसङ्ख्या - d, पङ्क्तिकाः);
        m.offset += esz*d;
    }
    अन्यथा
    {
        len = std::min(पङ्क्तिकाः + d, स्तम्भसङ्ख्या);
        m.offset -= चरणः[0]*d;
    }
    CV_DbgAssert( len > 0 );

    m.size[0] = m.पङ्क्तिकाः = len;
    m.size[1] = m.स्तम्भसङ्ख्या = 1;
    m.चरणः[0] += (len > 1 ? esz : 0);

    m.updateContinuityFlag();

    यदि( size() != आकार(1,1) )
        m.flags |= SUBMATRIX_FLAG;

    प्रत्यागम m;
}

शून्य UMat::locateROI( आकार& सम्पूर्णम्_आकार, Point& ofs ) ध्रुव
{
    CV_Assert( dims <= 2 && चरणः[0] > 0 );
    size_t esz = elem_आकार(), minstep;
    ptrdiff_t delta1 = (ptrdiff_t)offset, delta2 = (ptrdiff_t)u->size;

    यदि( delta1 == 0 )
        ofs.x = ofs.y = 0;
    अन्यथा
    {
        ofs.y = (पूर्णाङ्क)(delta1/चरणः[0]);
        ofs.x = (पूर्णाङ्क)((delta1 - चरणः[0]*ofs.y)/esz);
        CV_DbgAssert( offset == (size_t)(ofs.y*चरणः[0] + ofs.x*esz) );
    }
    minstep = (ofs.x + स्तम्भसङ्ख्या)*esz;
    सम्पूर्णम्_आकार.height = (पूर्णाङ्क)((delta2 - minstep)/चरणः[0] + 1);
    सम्पूर्णम्_आकार.height = std::max(सम्पूर्णम्_आकार.height, ofs.y + पङ्क्तिकाः);
    सम्पूर्णम्_आकार.विस्तारः = (पूर्णाङ्क)((delta2 - चरणः*(सम्पूर्णम्_आकार.height-1))/esz);
    सम्पूर्णम्_आकार.विस्तारः = std::max(सम्पूर्णम्_आकार.विस्तारः, ofs.x + स्तम्भसङ्ख्या);
}


UMat& UMat::adjustROI( पूर्णाङ्क dtop, पूर्णाङ्क dbottom, पूर्णाङ्क dleft, पूर्णाङ्क dright )
{
    CV_Assert( dims == 2 && चरणः[0] > 0 );
    आकार सम्पूर्णम्_आकार; Point ofs;
    size_t esz = elem_आकार();
    locateROI( सम्पूर्णम्_आकार, ofs );
    पूर्णाङ्क row1 = std::min(std::max(ofs.y - dtop, 0), सम्पूर्णम्_आकार.height), row2 = std::max(0, std::min(ofs.y + पङ्क्तिकाः + dbottom, सम्पूर्णम्_आकार.height));
    पूर्णाङ्क col1 = std::min(std::max(ofs.x - dleft, 0), सम्पूर्णम्_आकार.विस्तारः), col2 = std::max(0, std::min(ofs.x + स्तम्भसङ्ख्या + dright, सम्पूर्णम्_आकार.विस्तारः));
    यदि(row1 > row2)
        std::swap(row1, row2);
    यदि(col1 > col2)
        std::swap(col1, col2);

    offset += (row1 - ofs.y)*चरणः + (col1 - ofs.x)*esz;
    पङ्क्तिकाः = row2 - row1; स्तम्भसङ्ख्या = col2 - col1;
    size.p[0] = पङ्क्तिकाः; size.p[1] = स्तम्भसङ्ख्या;
    updateContinuityFlag();
    प्रत्यागम *एतत्;
}


UMat UMat::reshape(पूर्णाङ्क new_cn, पूर्णाङ्क new_पङ्क्तिकाः) ध्रुव
{
    पूर्णाङ्क cn = channels();
    UMat hdr = *एतत्;

    यदि( dims > 2 && new_पङ्क्तिकाः == 0 && new_cn != 0 && size[dims-1]*cn % new_cn == 0 )
    {
        hdr.flags = (hdr.flags & ~CV_MAT_CN_आवरणम्) | ((new_cn-1) << CV_CN_सरणम्);
        hdr.चरणः[dims-1] = CV_ELEM_आकार(hdr.flags);
        hdr.size[dims-1] = hdr.size[dims-1]*cn / new_cn;
        प्रत्यागम hdr;
    }

    CV_Assert( dims <= 2 );

    यदि( new_cn == 0 )
        new_cn = cn;

    पूर्णाङ्क योग_विस्तारः = स्तम्भसङ्ख्या * cn;

    यदि( (new_cn > योग_विस्तारः || योग_विस्तारः % new_cn != 0) && new_पङ्क्तिकाः == 0 )
        new_पङ्क्तिकाः = पङ्क्तिकाः * योग_विस्तारः / new_cn;

    यदि( new_पङ्क्तिकाः != 0 && new_पङ्क्तिकाः != पङ्क्तिकाः )
    {
        पूर्णाङ्क योग_आकार_2 = योग_विस्तारः * पङ्क्तिकाः;
        यदि( !isContinuous() )
            CV_Error( cv::Error::Bad_चरणः,
            "The matrix is not continuous, thus its number of rows can not be changed" );

        योग_विस्तारः = योग_आकार_2 / new_पङ्क्तिकाः;

        यदि( योग_विस्तारः * new_पङ्क्तिकाः != योग_आकार_2 )
            CV_Error( cv::Error::StsBadArg, "The total number of matrix elements "
                                    "is not divisible by the new number of rows" );

        hdr.size[0] = hdr.पङ्क्तिकाः = new_पङ्क्तिकाः;
        hdr.चरणः[0] = योग_विस्तारः * elemSize1();
    } अन्यथा {
        hdr.size[0] = hdr.पङ्क्तिकाः = पङ्क्तिकाः;
        यदि (dims <= 1)
            hdr.चरणः[0] = स्तम्भसङ्ख्या * CV_ELEM_आकार(flags);
    }

    पूर्णाङ्क new_विस्तारः = योग_विस्तारः / new_cn;

    यदि( new_विस्तारः * new_cn != योग_विस्तारः )
        CV_Error( cv::Error::Bad_अङ्कः_Channels,
        "The total width is not divisible by the new number of channels" );

    hdr.size.dims = hdr.dims = 2;
    hdr.size[1] = hdr.स्तम्भसङ्ख्या = new_विस्तारः;
    hdr.flags = (hdr.flags & ~CV_MAT_CN_आवरणम्) | ((new_cn-1) << CV_CN_सरणम्);
    hdr.चरणः[1] = CV_ELEM_आकार(hdr.flags);
    प्रत्यागम hdr;
}

#यदि_परिभाषितम् HAVE_OPENCL
नामाकाश {
स्थिर तार्किक ocl_समुच्चयः_Diag(ध्रुव UMat& d, UMat& m, पूर्णाङ्क len)
{
    पूर्णाङ्क cn = d.channels();
    पूर्णाङ्क गभीरता = d.गभीरता();

    यदि (गभीरता == CV_64F && !ocl::Device::getDefault().doubleFPConfig())
        प्रत्यागम असत्य;

    String opts = format("-D SET_DIAG -D T1=%s -D cn=%d -D IS_ROW_VECTOR=%d",
                         ocl::memop_Type_गन्तव्यम्_Str(गभीरता),
                         cn,
                         (d.पङ्क्तिकाः == 1) ? 1 : 0);

    ocl::Kernel k("setDiag", ocl::core::copyset_oclsrc, opts);
    यदि (k.empty())
        प्रत्यागम असत्य;

    k.तर्काः(ocl::KernelArg::WriteOnly(m),
           ocl::KernelArg::Read_Only_No_आकार(d),
           len);

    size_t globalsize[2] = { (size_t)len, (size_t)len };
    प्रत्यागम k.सततम्(2, globalsize, रिक्त, असत्य);
}
}
#यदि_अन्तः

UMat UMat::diag(ध्रुव UMat& d, UMatUsageFlags usageFlags)
{
    CV_INSTRUMENT_REGION();

    CV_Assert(d.स्तम्भसङ्ख्या == 1 || d.पङ्क्तिकाः == 1);
    पूर्णाङ्क len = d.पङ्क्तिकाः + d.स्तम्भसङ्ख्या - 1;

#यदि_परिभाषितम् HAVE_OPENCL
    यदि (ocl::useOpenCL())
    {
        UMat m(len, len, d.type(), usageFlags);
        यदि (ocl_समुच्चयः_Diag(d, m, len))
        {
            CV_IMPL_सङ्कलय(CV_IMPL_OCL);
            प्रत्यागम m;
        }
    }
#यदि_अन्तः

    UMat m(len, len, d.type(), Scalar(0), usageFlags);
    UMat md = m.diag();
    यदि( d.स्तम्भसङ्ख्या == 1 )
        d.copy_गन्तव्यम्(md);
    अन्यथा
        transpose(d, md);
    प्रत्यागम m;
}

पूर्णाङ्क UMat::check_सदिशम्(पूर्णाङ्क _elemChannels, पूर्णाङ्क गभीरता_2, तार्किक _requireContinuous) ध्रुव
{
    प्रत्यागम (गभीरता() == गभीरता_2 || गभीरता_2 <= 0) &&
        (isContinuous() || !_requireContinuous) &&
        ((dims <= 2 && (((पङ्क्तिकाः == 1 || स्तम्भसङ्ख्या == 1) && channels() == _elemChannels) ||
                        (स्तम्भसङ्ख्या == _elemChannels && channels() == 1))) ||
        (dims == 3 && channels() == 1 && size.p[2] == _elemChannels && (size.p[0] == 1 || size.p[1] == 1) &&
         (isContinuous() || चरणः.p[1] == चरणः.p[2]*size.p[2])))
    ? (पूर्णाङ्क)(योग()*channels()/_elemChannels) : -1;
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

        size_t योग_elem1_ref = एतत्->योग() * एतत्->channels();
        size_t योग_elem1 = _cn;

        Auto_बफर<पूर्णाङ्क, 4> newsz_buf( (size_t)std::max(_newndims, 1) );

        पूर्णाङ्क m1_idx = -1;

        प्रति (पूर्णाङ्क i = 0; i < _newndims; i++)
        {
            यदि (_newsz[i] >= 0) {
                यदि (_newsz[i] == 0 && i < dims)
                    newsz_buf[i] = size.p[i];
                अन्यथा
                    newsz_buf[i] = _newsz[i];
                योग_elem1 *= (size_t)newsz_buf[i];
            } अन्यथा {
                यदि (m1_idx >= 0)
                    CV_Error(cv::Error::Sts_Bad_आकार, "More than one '-1' occured in the new shape");
                m1_idx = i;
            }
        }

        यदि (m1_idx >= 0) {
            यदि (योग_elem1 == 0) {
                CV_Assert(योग_elem1_ref == 0);
                योग_elem1 = 1;
            }
            CV_Assert(योग_elem1_ref % योग_elem1 == 0);
            newsz_buf[m1_idx] = (पूर्णाङ्क)(योग_elem1_ref / योग_elem1);
        } अन्यथा यदि (योग_elem1 != योग_elem1_ref) {
            CV_Error(cv::Error::StsUnmatchedSizes, "Requested and source matrices have different count of elements");
        }

        UMat hdr = *एतत्;
        hdr.flags = (hdr.flags & ~CV_MAT_CN_आवरणम्) | ((_cn-1) << CV_CN_सरणम्);
        समुच्चयः_आकार(hdr, _newndims, newsz_buf.दत्त(), रिक्त, सत्य);

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
    UMat_दत्त_Auto_तालकम् autolock(u);
    यत्न
    {
        यदि(CV_XADD(&u->refcount, 1) == 0)
            u->currAllocator->map(u, accessFlags);
        यदि (u->दत्त != 0)
        {
            Mat hdr(dims, size.p, type(), u->दत्त + offset, चरणः.p);
            hdr.flags = flags;
            hdr.u = u;
            hdr.datastart = u->दत्त;
            hdr.दत्त = u->दत्त + offset;
            hdr.datalimit = hdr.dataend = u->दत्त + u->size;
            प्रत्यागम hdr;
        }
    }
    गृहाण(...)
    {
        CV_XADD(&u->refcount, -1);
        क्षिप;
    }
    CV_XADD(&u->refcount, -1);
    CV_Assert(u->दत्त != 0 && "Error mapping of UMat to host memory.");
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
        size_t s = चरणः.p[i];
        ofs[i] = val / s;
        val -= ofs[i]*s;
    }
}

शून्य UMat::copy_गन्तव्यम्(OutputArray _dst) ध्रुव
{
    CV_INSTRUMENT_REGION();

#यदि_परिभाषितम् HAVE_CUDA
    यदि (_dst.isGpuMat())
    {
        _dst.getGpuMat().upload(*एतत्);
        प्रत्यागम;
    }
#यदि_अन्तः

    पूर्णाङ्क stype = type();
    पूर्णाङ्क dtype = _dst.type();
    यदि( _dst.fixedType() && dtype != stype )
    {
        CV_Assert( CV_MAT_CN(stype) == CV_MAT_CN(dtype) );
        convert_गन्तव्यम्( _dst, dtype );
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
    size_t esz = CV_ELEM_आकार(stype);
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
            u->currAllocator->copy(u, dst.u, d, sz, srcofs, चरणः.p, dstofs, dst.चरणः.p, असत्य);
            प्रत्यागम;
        }

        // different allocators: never map a device destination, upload into it instead
        // (through a host Mat when both sides are device memory)
        MatAllocator* hostAllocator = Mat::getDefaultAllocator();
        यदि (dst.u->currAllocator != hostAllocator)
        {
            यदि (u->currAllocator == hostAllocator)
                getMat(ACCESS_READ).copy_गन्तव्यम्(_dst);
            अन्यथा
            {
                Mat tmp;
                copy_गन्तव्यम्(tmp);
                tmp.copy_गन्तव्यम्(_dst);
            }
            प्रत्यागम;
        }
    }

    Mat dst = _dst.getMat();
    u->currAllocator->download(u, dst.ptr(), d, sz, srcofs, चरणः.p, dst.चरणः.p);
}

शून्य UMat::copy_गन्तव्यम्(OutputArray _dst, निवेशः_Array आवरणम्_2) ध्रुव
{
    CV_INSTRUMENT_REGION();

    यदि( आवरणम्_2.empty() )
    {
        copy_गन्तव्यम्(_dst);
        प्रत्यागम;
    }
#यदि_परिभाषितम् HAVE_OPENCL
    पूर्णाङ्क cn = channels(), mtype = आवरणम्_2.type(), mdepth = CV_MAT_गभीरता(mtype), mcn = CV_MAT_CN(mtype);
    CV_Assert( (mdepth == CV_8U || mdepth == CV_8S || mdepth == CV_Bool) && (mcn == 1 || mcn == cn) );

    यदि (ocl::useOpenCL() && _dst.isUMat() && dims <= 2)
    {
        UMat_दत्त * prevu = _dst.getUMat().u;
        _dst.create( size, type() );

        UMat dst = _dst.getUMat();

        तार्किक haveDstUninit = असत्य;
        यदि( prevu != dst.u ) // do not leave dst uninitialized
            haveDstUninit = सत्य;

        String opts = format("-D COPY_TO_MASK -D T1=%s -D scn=%d -D mcn=%d%s",
                             ocl::memop_Type_गन्तव्यम्_Str(गभीरता()), cn, mcn,
                             haveDstUninit ? " -D HAVE_DST_UNINIT" : "");

        ocl::Kernel k("copyToMask", ocl::core::copyset_oclsrc, opts);
        यदि (!k.empty())
        {
            k.तर्काः(ocl::KernelArg::Read_Only_No_आकार(*एतत्),
                   ocl::KernelArg::Read_Only_No_आकार(आवरणम्_2.getUMat()),
                   haveDstUninit ? ocl::KernelArg::WriteOnly(dst) :
                                   ocl::KernelArg::ReadWrite(dst));

            size_t globalsize[2] = { (size_t)स्तम्भसङ्ख्या, (size_t)पङ्क्तिकाः };
            यदि (k.सततम्(2, globalsize, रिक्त, असत्य))
            {
                CV_IMPL_सङ्कलय(CV_IMPL_OCL);
                प्रत्यागम;
            }
        }
    }
#यदि_अन्तः
    Mat src = getMat(ACCESS_READ);
    src.copy_गन्तव्यम्(_dst, आवरणम्_2);
}


//
// void UMat::convertTo moved to convert.dispatch.cpp
//

UMat& UMat::समुच्चयः_गन्तव्यम्(निवेशः_Array मानम्, निवेशः_Array आवरणम्_2)
{
    CV_INSTRUMENT_REGION();

    तार्किक have_आवरणम् = !आवरणम्_2.empty();
#यदि_परिभाषितम् HAVE_OPENCL
    पूर्णाङ्क tp = type(), cn = CV_MAT_CN(tp), d = CV_MAT_गभीरता(tp);

    यदि( dims <= 2 && cn <= 4 && CV_MAT_गभीरता(tp) < CV_64F && ocl::useOpenCL() )
    {
        Mat value = मानम्.getMat();
        CV_Assert( checkScalar(value, type(), मानम्.विधा(), निवेशः_Array_2::UMAT) );
        पूर्णाङ्क kercn = have_आवरणम् || cn == 3 ? cn : std::max(cn, ocl::predict_Optimal_सदिशम्_विस्तारः(*एतत्)),
                kertp = CV_MAKE_TYPE(d, kercn);

        द्विगुण buf[16] = { 0, 0, 0, 0, 0, 0, 0, 0,
                           0, 0, 0, 0, 0, 0, 0, 0 };
        convertAndUnrollScalar(value, tp, (uchar *)buf, kercn / cn);

        पूर्णाङ्क scalarcn = kercn == 3 ? 4 : kercn, पङ्क्तिकाः_प्रत्येकम्_WI = ocl::Device::getDefault().isIntel() ? 4 : 1;
        String opts = format("-D dstT=%s -D rowsPerWI=%d -D dstST=%s -D dstT1=%s -D cn=%d",
                             ocl::memop_Type_गन्तव्यम्_Str(kertp), पङ्क्तिकाः_प्रत्येकम्_WI,
                             ocl::memop_Type_गन्तव्यम्_Str(CV_MAKETYPE(d, scalarcn)),
                             ocl::memop_Type_गन्तव्यम्_Str(d), kercn);

        ocl::Kernel समुच्चयः_K(have_आवरणम् ? "setMask" : "set", ocl::core::copyset_oclsrc, opts);
        यदि( !समुच्चयः_K.empty() )
        {
            ocl::KernelArg scalararg(ocl::KernelArg::CONSTANT, 0, 0, 0, buf, CV_ELEM_आकार(d) * scalarcn);
            UMat आवरणम्;

            यदि( have_आवरणम् )
            {
                आवरणम् = आवरणम्_2.getUMat();
                CV_Assert( आवरणम्.size() == size() && (आवरणम्.type() == CV_8U || आवरणम्.type() == CV_8S || आवरणम्.type() == CV_Bool) );
                ocl::KernelArg maskarg = ocl::KernelArg::Read_Only_No_आकार(आवरणम्),
                        dstarg = ocl::KernelArg::ReadWrite(*एतत्);
                समुच्चयः_K.तर्काः(maskarg, dstarg, scalararg);
            }
            अन्यथा
            {
                ocl::KernelArg dstarg = ocl::KernelArg::WriteOnly(*एतत्, cn, kercn);
                समुच्चयः_K.तर्काः(dstarg, scalararg);
            }

            size_t globalsize[] = { (size_t)स्तम्भसङ्ख्या * cn / kercn, ((size_t)पङ्क्तिकाः + पङ्क्तिकाः_प्रत्येकम्_WI - 1) / पङ्क्तिकाः_प्रत्येकम्_WI };
            यदि( समुच्चयः_K.सततम्(2, globalsize, रिक्त, असत्य) )
            {
                CV_IMPL_सङ्कलय(CV_IMPL_OCL);
                प्रत्यागम *एतत्;
            }
        }
    }
#यदि_अन्तः
    Mat m = getMat(have_आवरणम् ? ACCESS_RW : ACCESS_WRITE);
    m.समुच्चयः_गन्तव्यम्(मानम्, आवरणम्_2);
    प्रत्यागम *एतत्;
}

UMat& UMat::संक्रियकः = (ध्रुव Scalar& s)
{
    समुच्चयः_गन्तव्यम्(s);
    प्रत्यागम *एतत्;
}

UMat UMat::t() ध्रुव
{
    UMat m;
    transpose(*एतत्, m);
    प्रत्यागम m;
}

UMat UMat::zeros(पूर्णाङ्क पङ्क्तिकाः, पूर्णाङ्क स्तम्भसङ्ख्या, पूर्णाङ्क type, UMatUsageFlags usageFlags)
{
    प्रत्यागम UMat(पङ्क्तिकाः, स्तम्भसङ्ख्या, type, Scalar::all(0), usageFlags);
}

UMat UMat::zeros(आकार size, पूर्णाङ्क type, UMatUsageFlags usageFlags)
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

UMat UMat::ones(पूर्णाङ्क पङ्क्तिकाः, पूर्णाङ्क स्तम्भसङ्ख्या, पूर्णाङ्क type, UMatUsageFlags usageFlags)
{
    प्रत्यागम UMat(पङ्क्तिकाः, स्तम्भसङ्ख्या, type, Scalar(1), usageFlags);
}

UMat UMat::ones(आकार size, पूर्णाङ्क type, UMatUsageFlags usageFlags)
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
