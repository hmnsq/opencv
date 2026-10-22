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

#అన్తర్భావ "precomp.hpp"
#అన్తర్భావ "opencl_kernels_core.hpp"
#అన్తర్భావ "umatrix.hpp"

#అన్తర్భావ <opencv2/core/utils/tls.hpp>

///////////////////////////////// UMat implementation ///////////////////////////////

నామాకాశ cv {

// forward decls, implementation is below in this file
శూన్య సమితి_కొలత(UMat& m, పూర్ణాంకం _dims, స్థిరరాశి పూర్ణాంకం* _sz, స్థిరరాశి size_t* అడుగులు_2,
             తార్కిక auto_అడుగులు = అబద్ధం);

శూన్య updateContinuityFlag(UMat& m);
శూన్య finalizeHdr(UMat& m);

UMat_దత్తాంశం::UMat_దత్తాంశం(స్థిరరాశి MatAllocator* allocator)
{
    prevAllocator = currAllocator = allocator;
    urefcount = refcount = mapcount = 0;
    దత్తాంశం = origdata = 0;
    size = 0;
    flags = static_cast<UMat_దత్తాంశం::జ్ఞాపకం_Flag>(0);
    handle = 0;
    userdata = 0;
    allocatorFlags_ = 0;
    original_UMat_దత్తాంశం = ఖాళీ;
}

UMat_దత్తాంశం::~UMat_దత్తాంశం()
{
    prevAllocator = currAllocator = 0;
    urefcount = refcount = 0;
    CV_Assert(mapcount == 0);
    దత్తాంశం = origdata = 0;
    size = 0;
    తార్కిక isAsyncCleanup = !!(flags & UMat_దత్తాంశం::ASYNC_CLEANUP);
    flags = static_cast<UMat_దత్తాంశం::జ్ఞాపకం_Flag>(0);
    handle = 0;
    userdata = 0;
    allocatorFlags_ = 0;
    ఒకవేళ (original_UMat_దత్తాంశం)
    {
        తార్కిక showWarn = అబద్ధం;
        UMat_దత్తాంశం* u = original_UMat_దత్తాంశం;
        తార్కిక zero_Ref = CV_XADD(&(u->refcount), -1) == 1;
        ఒకవేళ (zero_Ref)
        {
            // simulate Mat::deallocate
            ఒకవేళ (u->mapcount != 0)
            {
                (u->currAllocator ? u->currAllocator : /* TODO allocator ? allocator :*/ Mat::getDefaultAllocator())->unmap(u);
            }
            లేకపోతే
            {
                // we don't do "map", so we can't do "unmap"
            }
        }
        తార్కిక zero_URef = CV_XADD(&(u->urefcount), -1) == 1;
        ఒకవేళ (zero_Ref && !zero_URef)
            showWarn = నిజం;
        ఒకవేళ (zero_Ref && zero_URef) // oops, we need to free resources
        {
            showWarn = !isAsyncCleanup;
            // simulate UMat::deallocate
            u->currAllocator->deallocate(u);
        }
#నిర్వచితం_కాకపోతే NDEBUG
        ఒకవేళ (showWarn)
        {
            స్థిర పూర్ణాంకం warn_సందేశం_showed = 0;
            ఒకవేళ (warn_సందేశం_showed++ < 100)
            {
                fflush(stdout);
                fprintf(stderr, "\n! OPENCV warning: getUMat()/getMat() call chain possible problem."
                                "\n!                 Base object is dead, while nested/derived object is still alive or processed."
                                "\n!                 Please check lifetime of UMat/Mat objects!\n");
                fflush(stderr);
            }
        }
#లేకపోతే
        CV_వాడని(showWarn);
#అయితే_ముగింపు
        original_UMat_దత్తాంశం = ఖాళీ;
    }
}

#నిర్వచితం_కాకపోతే OPENCV_DISABLE_THREAD_SUPPORT

// it should be a prime number for the best hash function
గణన { UMAT_NLOCKS = 31 };
స్థిర Mutex umatLocks[UMAT_NLOCKS];

స్థిర size_t get_UMat_దత్తాంశం_తాళం_సూచీ(స్థిరరాశి UMat_దత్తాంశం* u)
{
    size_t idx = ((size_t)(శూన్య*)u) % UMAT_NLOCKS;
    తిరిగి idx;
}

శూన్య UMat_దత్తాంశం::lock()
{
    size_t idx = get_UMat_దత్తాంశం_తాళం_సూచీ(ఇది);
    //printf("%d lock(%d)\n", cv::utils::getThreadID(), (int)idx);
    umatLocks[idx].lock();
}

శూన్య UMat_దత్తాంశం::unlock()
{
    size_t idx = get_UMat_దత్తాంశం_తాళం_సూచీ(ఇది);
    //printf("%d unlock(%d)\n", cv::utils::getThreadID(), (int)idx);
    umatLocks[idx].unlock();
}


// Do not allow several lock() calls with different UMatData objects.
నిర్మాణం UMat_దత్తాంశం_Auto_Locker
{
    పూర్ణాంకం usage_లెక్క;
    UMat_దత్తాంశం* locked_objects[2];
    UMat_దత్తాంశం_Auto_Locker() : usage_లెక్క(0) { locked_objects[0] = ఖాళీ; locked_objects[1] = ఖాళీ; }

    శూన్య lock(UMat_దత్తాంశం*& u1)
    {
        తార్కిక locked_1 = (u1 == locked_objects[0] || u1 == locked_objects[1]);
        ఒకవేళ (locked_1)
        {
            u1 = ఖాళీ;
            తిరిగి;
        }
        CV_Assert(usage_లెక్క == 0);  // UMatDataAutoLock can't be used multiple times from the same thread
        usage_లెక్క = 1;
        locked_objects[0] = u1;
        u1->lock();
    }
    శూన్య lock(UMat_దత్తాంశం*& u1, UMat_దత్తాంశం*& u2)
    {
        తార్కిక locked_1 = (u1 == locked_objects[0] || u1 == locked_objects[1]);
        తార్కిక locked_2 = (u2 == locked_objects[0] || u2 == locked_objects[1]);
        ఒకవేళ (locked_1)
            u1 = ఖాళీ;
        ఒకవేళ (locked_2)
            u2 = ఖాళీ;
        ఒకవేళ (locked_1 && locked_2)
            తిరిగి;
        CV_Assert(usage_లెక్క == 0);  // UMatDataAutoLock can't be used multiple times from the same thread
        usage_లెక్క = 1;
        locked_objects[0] = u1;
        locked_objects[1] = u2;
        ఒకవేళ (u1)
            u1->lock();
        ఒకవేళ (u2)
            u2->lock();
    }
    శూన్య release(UMat_దత్తాంశం* u1, UMat_దత్తాంశం* u2)
    {
        ఒకవేళ (u1 == ఖాళీ && u2 == ఖాళీ)
            తిరిగి;
        CV_Assert(usage_లెక్క == 1);
        usage_లెక్క = 0;
        ఒకవేళ (u1)
            u1->unlock();
        ఒకవేళ (u2)
            u2->unlock();
        locked_objects[0] = ఖాళీ; locked_objects[1] = ఖాళీ;
    }
};
స్థిర TLSData<UMat_దత్తాంశం_Auto_Locker>& get_UMat_దత్తాంశం_Auto_Locker_TLS()
{
    CV_SINGLETON_LAZY_INIT_REF(TLSData<UMat_దత్తాంశం_Auto_Locker>, కొత్త TLSData<UMat_దత్తాంశం_Auto_Locker>());
}
స్థిర UMat_దత్తాంశం_Auto_Locker& get_UMat_దత్తాంశం_Auto_Locker() { తిరిగి get_UMat_దత్తాంశం_Auto_Locker_TLS().getRef(); }


UMat_దత్తాంశం_Auto_తాళం::UMat_దత్తాంశం_Auto_తాళం(UMat_దత్తాంశం* u) : u1(u), u2(ఖాళీ)
{
    get_UMat_దత్తాంశం_Auto_Locker().lock(u1);
}
UMat_దత్తాంశం_Auto_తాళం::UMat_దత్తాంశం_Auto_తాళం(UMat_దత్తాంశం* u1_, UMat_దత్తాంశం* u2_) : u1(u1_), u2(u2_)
{
    ఒకవేళ (get_UMat_దత్తాంశం_తాళం_సూచీ(u1) > get_UMat_దత్తాంశం_తాళం_సూచీ(u2))
    {
        std::swap(u1, u2);
    }
    get_UMat_దత్తాంశం_Auto_Locker().lock(u1, u2);
}
UMat_దత్తాంశం_Auto_తాళం::~UMat_దత్తాంశం_Auto_తాళం()
{
    get_UMat_దత్తాంశం_Auto_Locker().release(u1, u2);
}

#లేకపోతే

శూన్య UMat_దత్తాంశం::lock()
{
    // nothing in OPENCV_DISABLE_THREAD_SUPPORT mode
}

శూన్య UMat_దత్తాంశం::unlock()
{
    // nothing in OPENCV_DISABLE_THREAD_SUPPORT mode
}

UMat_దత్తాంశం_Auto_తాళం::UMat_దత్తాంశం_Auto_తాళం(UMat_దత్తాంశం* u) : u1(u), u2(ఖాళీ)
{
    // nothing in OPENCV_DISABLE_THREAD_SUPPORT mode
}
UMat_దత్తాంశం_Auto_తాళం::UMat_దత్తాంశం_Auto_తాళం(UMat_దత్తాంశం* u1_, UMat_దత్తాంశం* u2_) : u1(u1_), u2(u2_)
{
    // nothing in OPENCV_DISABLE_THREAD_SUPPORT mode
}
UMat_దత్తాంశం_Auto_తాళం::~UMat_దత్తాంశం_Auto_తాళం()
{
    // nothing in OPENCV_DISABLE_THREAD_SUPPORT mode
}

#అయితే_ముగింపు  // OPENCV_DISABLE_THREAD_SUPPORT

//////////////////////////////// UMat ////////////////////////////////

UMat::UMat(UMatUsageFlags _usageFlags) CV_NOEXCEPT
: flags(MAGIC_VAL), dims(0), అడ్డువరుసలు(0), నిలువుల_సంఖ్య(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{}

UMat::UMat(పూర్ణాంకం అడ్డువరుసలు_3, పూర్ణాంకం నిలువుల_సంఖ్య_3, పూర్ణాంకం _type, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), అడ్డువరుసలు(0), నిలువుల_సంఖ్య(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(అడ్డువరుసలు_3, నిలువుల_సంఖ్య_3, _type);
}

UMat::UMat(పూర్ణాంకం అడ్డువరుసలు_3, పూర్ణాంకం నిలువుల_సంఖ్య_3, పూర్ణాంకం _type, స్థిరరాశి Scalar& _s, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), అడ్డువరుసలు(0), నిలువుల_సంఖ్య(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(అడ్డువరుసలు_3, నిలువుల_సంఖ్య_3, _type);
    *ఇది = _s;
}

UMat::UMat(కొలత _sz, పూర్ణాంకం _type, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), అడ్డువరుసలు(0), నిలువుల_సంఖ్య(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create( _sz.ఎత్తు, _sz.వెడల్పు, _type );
}

UMat::UMat(కొలత _sz, పూర్ణాంకం _type, స్థిరరాశి Scalar& _s, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), అడ్డువరుసలు(0), నిలువుల_సంఖ్య(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_sz.ఎత్తు, _sz.వెడల్పు, _type);
    *ఇది = _s;
}

UMat::UMat(పూర్ణాంకం _dims, స్థిరరాశి పూర్ణాంకం* _sz, పూర్ణాంకం _type, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), అడ్డువరుసలు(0), నిలువుల_సంఖ్య(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_dims, _sz, _type);
}

UMat::UMat(పూర్ణాంకం _dims, స్థిరరాశి పూర్ణాంకం* _sz, పూర్ణాంకం _type, స్థిరరాశి Scalar& _s, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), అడ్డువరుసలు(0), నిలువుల_సంఖ్య(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_dims, _sz, _type);
    *ఇది = _s;
}

UMat::UMat(స్థిరరాశి MatShape& _shape, పూర్ణాంకం _type, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), అడ్డువరుసలు(0), నిలువుల_సంఖ్య(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_shape.dims, _shape.p, _type);
}

UMat::UMat(స్థిరరాశి MatShape& _shape, పూర్ణాంకం _type, స్థిరరాశి Scalar& _s, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), అడ్డువరుసలు(0), నిలువుల_సంఖ్య(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0)
{
    create(_shape.dims, _shape.p, _type);
    *ఇది = _s;
}

UMat::UMat(స్థిరరాశి UMat& m)
: flags(m.flags), dims(m.dims), అడ్డువరుసలు(m.అడ్డువరుసలు), నిలువుల_సంఖ్య(m.నిలువుల_సంఖ్య), allocator(m.allocator),
  usageFlags(m.usageFlags), u(m.u), offset(m.offset), size(m.size), అడుగు(m.అడుగు)
{
    addref();
}

UMat& UMat::క్రియాకారకం=(స్థిరరాశి UMat& m)
{
    ఒకవేళ( ఇది != &m )
    {
        const_cast<UMat&>(m).addref();
        release();
        flags = m.flags;
        dims = m.dims;
        అడ్డువరుసలు = m.అడ్డువరుసలు;
        నిలువుల_సంఖ్య = m.నిలువుల_సంఖ్య;
        size = m.size;
        అడుగు = m.అడుగు;
        allocator = m.allocator;
        usageFlags = m.usageFlags;
        u = m.u;
        offset = m.offset;
    }
    తిరిగి *ఇది;
}

UMat UMat::clone() స్థిరరాశి
{
    UMat m;
    copy_గమ్యం(m);
    తిరిగి m;
}

శూన్య UMat::కేటాయించు_గమ్యం(UMat& m, పూర్ణాంకం _type) స్థిరరాశి
{
    ఒకవేళ( _type < 0 )
        m = *ఇది;
    లేకపోతే
        convert_గమ్యం(m, _type);
}

శూన్య UMat::create(పూర్ణాంకం అడ్డువరుసలు_3, పూర్ణాంకం నిలువుల_సంఖ్య_3, పూర్ణాంకం _type, UMatUsageFlags _usageFlags)
{
    పూర్ణాంకం sz[] = {అడ్డువరుసలు_3, నిలువుల_సంఖ్య_3};
    create(2, sz, _type, _usageFlags);
}

శూన్య UMat::create(కొలత _sz, పూర్ణాంకం _type, UMatUsageFlags _usageFlags)
{
    create(_sz.ఎత్తు, _sz.వెడల్పు, _type, _usageFlags);
}

శూన్య UMat::create_Same_కొలత(ఆదానం_Array arr, పూర్ణాంకం _type, UMatUsageFlags _usageFlags)
{
    create(arr.shape(), _type, _usageFlags);
}

శూన్య UMat::addref()
{
    ఒకవేళ( u )
        CV_XADD(&(u->urefcount), 1);
}

శూన్య UMat::release()
{
    ఒకవేళ( u && CV_XADD(&(u->urefcount), -1) == 1 )
        deallocate();
    u = 0;
    size.clear();
    dims = నిలువుల_సంఖ్య = అడ్డువరుసలు = 0;
}

తార్కిక UMat::empty() స్థిరరాశి
{
    తిరిగి u == 0 || మొత్తం() == 0;
}

size_t UMat::మొత్తం() స్థిరరాశి
{
    ఒకవేళ( dims <= 2 )
        తిరిగి (size_t)అడ్డువరుసలు * నిలువుల_సంఖ్య;
    size_t p = 1;
    కొరకు( పూర్ణాంకం i = 0; i < dims; i++ )
        p *= size[i];
    తిరిగి p;
}

MatShape UMat::shape() స్థిరరాశి
{
    తిరిగి size;
}

UMat::UMat(UMat&& m)
: flags(m.flags), dims(m.dims), అడ్డువరుసలు(m.అడ్డువరుసలు), నిలువుల_సంఖ్య(m.నిలువుల_సంఖ్య), allocator(m.allocator),
  usageFlags(m.usageFlags), u(m.u), offset(m.offset)
{
    size = m.size;
    అడుగు = m.అడుగు;
    m.flags = MAGIC_VAL;
    m.usageFlags = USAGE_DEFAULT;
    m.dims = m.అడ్డువరుసలు = m.నిలువుల_సంఖ్య = 0;
    m.size.clear();
    m.allocator = ఖాళీ;
    m.u = ఖాళీ;
    m.offset = 0;
}

UMat& UMat::క్రియాకారకం=(UMat&& m)
{
    ఒకవేళ (ఇది == &m)
      తిరిగి *ఇది;
    release();
    flags = m.flags; dims = m.dims; అడ్డువరుసలు = m.అడ్డువరుసలు; నిలువుల_సంఖ్య = m.నిలువుల_సంఖ్య;
    allocator = m.allocator; usageFlags = m.usageFlags;
    u = m.u;
    offset = m.offset;
    size = m.size;
    అడుగు = m.అడుగు;
    m.flags = MAGIC_VAL;
    m.usageFlags = USAGE_DEFAULT;
    m.dims = m.అడ్డువరుసలు = m.నిలువుల_సంఖ్య = 0;
    m.allocator = ఖాళీ;
    m.u = ఖాళీ;
    m.offset = 0;
    తిరిగి *ఇది;
}


MatAllocator* UMat::getStdAllocator()
{
#నిర్వచితమైతే HAVE_OPENCL
    ఒకవేళ (ocl::useOpenCL())
        తిరిగి ocl::getOpenCLAllocator();
#అయితే_ముగింపు
    తిరిగి Mat::getDefaultAllocator();
}

శూన్య swap( UMat& a, UMat& b )
{
    std::swap(a.flags, b.flags);
    std::swap(a.dims, b.dims);
    std::swap(a.అడ్డువరుసలు, b.అడ్డువరుసలు);
    std::swap(a.నిలువుల_సంఖ్య, b.నిలువుల_సంఖ్య);
    std::swap(a.allocator, b.allocator);
    std::swap(a.u, b.u);
    std::swap(a.offset, b.offset);

    std::swap(a.size, b.size);
    std::swap(a.అడుగు, b.అడుగు);
}


శూన్య సమితి_కొలత( UMat& m, పూర్ణాంకం _dims, స్థిరరాశి పూర్ణాంకం* _sz,
              స్థిరరాశి size_t* అడుగులు_2, తార్కిక auto_అడుగులు )
{
    CV_Assert( 0 <= _dims && _dims <= CV_MAX_DIM && _dims <= MatShape::MAX_DIMS);

    m.dims = _dims;
    m.size = MatShape(_dims, _sz);
    m.అడుగు[std::max(_dims-1, 0)] = CV_ELEM_కొలత(m.flags);
    కొరకు (పూర్ణాంకం i = _dims-2; i >= 0; i--) {
        size_t autostep = m.size[i+1]*m.అడుగు[i+1];
        ఒకవేళ (అడుగులు_2) {
            m.అడుగు[i] = అడుగులు_2[i];
            //CV_Assert(m.step[i] >= autostep);
        } లేకపోతే ఒకవేళ (auto_అడుగులు) {
            m.అడుగు[i] = autostep;
        } లేకపోతే {
            m.అడుగు[i] = 0;
        }
    }

    ఒకవేళ( _dims <= 2 )
    {
        m.నిలువుల_సంఖ్య = _dims == 0 ? 1 : _sz ? _sz[_dims > 1] : 0;
        m.అడ్డువరుసలు = _dims < 2 ? 1 : _sz ? _sz[0] : 0;
    } లేకపోతే {
        m.నిలువుల_సంఖ్య = m.అడ్డువరుసలు = -1;
    }
}


శూన్య UMat::updateContinuityFlag()
{
    flags = cv::updateContinuityFlag(flags, dims, size.p, అడుగు.p);
}


శూన్య finalizeHdr(UMat& m)
{
    m.updateContinuityFlag();
    పూర్ణాంకం d = m.dims;
    ఒకవేళ( d > 2 )
        m.అడ్డువరుసలు = m.నిలువుల_సంఖ్య = -1;
}


UMat Mat::getUMat(AccessFlag accessFlags, UMatUsageFlags usageFlags) స్థిరరాశి
{
    UMat hdr;
    ఒకవేళ(!దత్తాంశం)
        తిరిగి hdr;
    ఒకవేళ (దత్తాంశం != datastart)
    {
        కొలత సంపూర్ణ_కొలత;
        బిందువు ofs;
        locateROI(సంపూర్ణ_కొలత, ofs);
        కొలత sz(నిలువుల_సంఖ్య, అడ్డువరుసలు);
        ఒకవేళ (ofs.x != 0 || ofs.y != 0)
        {
            Mat src = *ఇది;
            పూర్ణాంకం dtop = ofs.y;
            పూర్ణాంకం dbottom = సంపూర్ణ_కొలత.ఎత్తు - src.అడ్డువరుసలు - ofs.y;
            పూర్ణాంకం dleft = ofs.x;
            పూర్ణాంకం dright = సంపూర్ణ_కొలత.వెడల్పు - src.నిలువుల_సంఖ్య - ofs.x;
            src.adjustROI(dtop, dbottom, dleft, dright);
            తిరిగి src.getUMat(accessFlags, usageFlags)(cv::Rect(ofs.x, ofs.y, sz.వెడల్పు, sz.ఎత్తు));
        }
    }
    CV_Assert(దత్తాంశం == datastart);

    accessFlags |= ACCESS_RW;
    UMat_దత్తాంశం* new_u = ఖాళీ;
    Mat_అడుగు new_అడుగు = అడుగు;
    {
        MatAllocator *a = allocator, *a0 = getDefaultAllocator();
        ఒకవేళ(!a)
            a = a0;
        new_u = a->allocate(dims, size.p, type(), దత్తాంశం, new_అడుగు.p, accessFlags, usageFlags);
        new_u->original_UMat_దత్తాంశం = u;
    }
    తార్కిక allocated = అబద్ధం;
    ప్రయత్నించు
    {
        allocated = UMat::getStdAllocator()->allocate(new_u, accessFlags, usageFlags);
    }
    పట్టుకో (స్థిరరాశి cv::Exception& e)
    {
        fprintf(stderr, "Exception: %s\n", e.what());
    }
    ఒకవేళ (!allocated)
    {
        allocated = getDefaultAllocator()->allocate(new_u, accessFlags, usageFlags);
        CV_Assert(allocated);
    }
    ఒకవేళ (u != ఖాళీ)
    {
#నిర్వచితమైతే HAVE_OPENCL
        ఒకవేళ (ocl::useOpenCL() && new_u->currAllocator == ocl::getOpenCLAllocator())
        {
            CV_Assert(new_u->తాత్కాలిక_UMat());
        }
#అయితే_ముగింపు
        CV_XADD(&(u->refcount), 1);
        CV_XADD(&(u->urefcount), 1);
    }
    ప్రయత్నించు
    {
        hdr.flags = flags;
        hdr.usageFlags = usageFlags;
        సమితి_కొలత(hdr, dims, size.p, new_అడుగు.p);
        finalizeHdr(hdr);
        hdr.u = new_u;
        hdr.offset = 0; //data - datastart;
        hdr.addref();
        తిరిగి hdr;
    }
    పట్టుకో(...)
    {
        ఒకవేళ (u != ఖాళీ)
        {
            CV_XADD(&(u->refcount), -1);
            CV_XADD(&(u->urefcount), -1);
        }
        new_u->currAllocator->deallocate(new_u);
        విసురు;
    }

}

శూన్య UMat::create(పూర్ణాంకం d0, స్థిరరాశి పూర్ణాంకం* _sizes, పూర్ణాంకం _type, UMatUsageFlags _usageFlags)
{
    పూర్ణాంకం sz1 = 1, d = d0;
    పూర్ణాంకం i;
    ఒకవేళ (d == 0) {
        d = 1;
        _sizes = (స్థిరరాశి పూర్ణాంకం*)&sz1;
    }
    CV_Assert(0 <= d && d <= CV_MAX_DIM && _sizes);
    _type = CV_MAT_TYPE(_type);

    // if param value is USAGE_DEFAULT by implicit default param value -or- explicit value
    // ...then don't change the existing usageFlags
    // it is not possible to change usage from non-default to USAGE_DEFAULT through create()
    // ...instead must construct UMat()
    ఒకవేళ (_usageFlags == cv::USAGE_DEFAULT)
    {
        _usageFlags = usageFlags;
    }

    ఒకవేళ( u && d == dims && _type == type() && _usageFlags == usageFlags )
    {
        కొరకు( i = 0; i < d; i++ )
            ఒకవేళ( size[i] != _sizes[i] )
                ఆపు;
        ఒకవేళ( i == d )
            తిరిగి;
    }

    పూర్ణాంకం _sizes_backup[CV_MAX_DIM]; // #5991
    ఒకవేళ (_sizes == (ఇది->size.p))
    {
        కొరకు(i = 0; i < d; i++ )
            _sizes_backup[i] = _sizes[i];
        _sizes = _sizes_backup;
    }

    release();
    usageFlags = _usageFlags;
    ఒకవేళ( d == 0 )
        తిరిగి;
    flags = (_type & CV_MAT_TYPE_ముసుగు) | MAGIC_VAL;
    సమితి_కొలత(*ఇది, d, _sizes, 0, నిజం);
    offset = 0;

    ఒకవేళ( మొత్తం() > 0 )
    {
        MatAllocator *a = allocator, *a0 = getStdAllocator();
        ఒకవేళ (!a)
        {
            a = a0;
            a0 = Mat::getDefaultAllocator();
        }
        ప్రయత్నించు
        {
            u = a->allocate(dims, size.p, _type, 0, అడుగు.p, ACCESS_RW /* ignored */, usageFlags);
            CV_Assert(u != 0);
        }
        పట్టుకో(...)
        {
            ఒకవేళ(a != a0)
                u = a0->allocate(dims, size.p, _type, 0, అడుగు.p, ACCESS_RW /* ignored */, usageFlags);
            CV_Assert(u != 0);
        }
        CV_Assert( అడుగు[dims-1] == (size_t)CV_ELEM_కొలత(flags) );
    }

    finalizeHdr(*ఇది);
    addref();
    size.dims = dims = d0;
}

శూన్య UMat::create(స్థిరరాశి std::vector<పూర్ణాంకం>& _sizes, పూర్ణాంకం _type, UMatUsageFlags _usageFlags)
{
    create((పూర్ణాంకం)_sizes.size(), _sizes.దత్తాంశం(), _type, _usageFlags);
}

శూన్య UMat::create(స్థిరరాశి MatShape& _shape, పూర్ణాంకం _type, UMatUsageFlags _usageFlags)
{
    ఒకవేళ (_shape.dims < 0) {
        release();
    } లేకపోతే {
        create(_shape.dims, _shape.p, _type, _usageFlags);
        size.layout = _shape.layout;
        size.C = _shape.C;
    }
}

శూన్య UMat::fit(పూర్ణాంకం _dims, స్థిరరాశి పూర్ణాంకం* _sizes, పూర్ణాంకం _type, UMatUsageFlags _usageFlags)
{
    ఒకవేళ (_usageFlags == cv::USAGE_DEFAULT)
        _usageFlags = usageFlags;
    size_t old_మొత్తం_బైట్లు = u ? u->size : 0;
    size_t esz = CV_ELEM_కొలత(_type), new_మొత్తం = _dims >= 0;
    కొరకు (పూర్ణాంకం i = 0; i < _dims; i++)
        new_మొత్తం *= _sizes[i];
    size_t new_మొత్తం_బైట్లు = new_మొత్తం*esz;
    ఒకవేళ (new_మొత్తం_బైట్లు > 0 &&
        (!isContinuous() ||
         new_మొత్తం_బైట్లు > old_మొత్తం_బైట్లు ||
         offset != 0 ||
         _usageFlags != usageFlags)) {
        create(_dims, _sizes, _type, _usageFlags);
    } లేకపోతే {
        flags = (flags & ~Mat::TYPE_ముసుగు) | CV_MAT_TYPE(_type);
        పూర్ణాంకం dummy_కొలత = 0;
        సమితి_కొలత(*ఇది, (_dims >= 0 ? _dims : 1), (_dims >= 0 ? _sizes : &dummy_కొలత), శూన్యసూచిక, నిజం);
        finalizeHdr(*ఇది);
    }
}

శూన్య UMat::fit(స్థిరరాశి std::vector<పూర్ణాంకం>& _shape, పూర్ణాంకం _type, UMatUsageFlags _usageFlags)
{
    fit((పూర్ణాంకం)_shape.size(), _shape.దత్తాంశం(), _type, _usageFlags);
}

శూన్య UMat::fit(స్థిరరాశి MatShape& _shape, పూర్ణాంకం _type, UMatUsageFlags _usageFlags)
{
    fit(_shape.dims, _shape.p, _type, _usageFlags);
    size.layout = _shape.layout;
    size.C = _shape.C;
}

శూన్య UMat::fit(పూర్ణాంకం అడ్డువరుసలు_3, పూర్ణాంకం నిలువుల_సంఖ్య_3, పూర్ణాంకం _type, UMatUsageFlags _usageFlags)
{
    _type &= TYPE_ముసుగు;
    పూర్ణాంకం sz[] = {అడ్డువరుసలు_3, నిలువుల_సంఖ్య_3};
    fit(2, sz, _type, _usageFlags);
}

శూన్య UMat::fit(కొలత _sz, పూర్ణాంకం _type, UMatUsageFlags _usageFlags)
{
    fit(_sz.ఎత్తు, _sz.వెడల్పు, _type, _usageFlags);
}

శూన్య UMat::fit_Same_కొలత(ఆదానం_Array m, పూర్ణాంకం _type, UMatUsageFlags _usageFlags)
{
    fit(m.shape(), _type, _usageFlags);
}

శూన్య UMat::copy_కొలత(స్థిరరాశి UMat& m)
{
    dims = m.dims;
    నిలువుల_సంఖ్య = m.నిలువుల_సంఖ్య;
    అడ్డువరుసలు = m.అడ్డువరుసలు;
    size = m.size;
    అడుగు = m.అడుగు;
}

UMat::~UMat()
{
    release();
}

శూన్య UMat::deallocate()
{
    UMat_దత్తాంశం* u_ = u;
    u = ఖాళీ;
    u_->currAllocator->deallocate(u_);
}


UMat::UMat(స్థిరరాశి UMat& m, స్థిరరాశి Range& అడ్డువరుస_Range_3, స్థిరరాశి Range& నిలువు_Range_3)
    : flags(MAGIC_VAL), dims(0), అడ్డువరుసలు(0), నిలువుల_సంఖ్య(0), allocator(0), usageFlags(USAGE_DEFAULT), u(0), offset(0)
{
    CV_Assert( m.dims >= 2 && m.dims <= MatShape::MAX_DIMS );
    ఒకవేళ( m.dims > 2 )
    {
        Range rs[MatShape::MAX_DIMS];
        rs[0] = అడ్డువరుస_Range_3;
        rs[1] = నిలువు_Range_3;
        కొరకు( పూర్ణాంకం i = 2; i < m.dims; i++ )
            rs[i] = Range::all();
        *ఇది = m(rs);
        తిరిగి;
    }

    *ఇది = m;
    ఒకవేళ( అడ్డువరుస_Range_3 != Range::all() && అడ్డువరుస_Range_3 != Range(0,అడ్డువరుసలు) )
    {
        CV_Assert( 0 <= అడ్డువరుస_Range_3.ప్రారంభం &&
                  అడ్డువరుస_Range_3.ప్రారంభం <= అడ్డువరుస_Range_3.end &&
                  అడ్డువరుస_Range_3.end <= m.అడ్డువరుసలు );
        size[0] = అడ్డువరుసలు = అడ్డువరుస_Range_3.size();
        offset += అడుగు*అడ్డువరుస_Range_3.ప్రారంభం;
        flags |= SUBMATRIX_FLAG;
    }

    ఒకవేళ( నిలువు_Range_3 != Range::all() && నిలువు_Range_3 != Range(0,నిలువుల_సంఖ్య) )
    {
        CV_Assert( 0 <= నిలువు_Range_3.ప్రారంభం &&
                  నిలువు_Range_3.ప్రారంభం <= నిలువు_Range_3.end &&
                  నిలువు_Range_3.end <= m.నిలువుల_సంఖ్య );
        size[1] = నిలువుల_సంఖ్య = నిలువు_Range_3.size();
        offset += నిలువు_Range_3.ప్రారంభం*elem_కొలత();
        flags |= SUBMATRIX_FLAG;
    }

    updateContinuityFlag();

    ఒకవేళ( అడ్డువరుసలు <= 0 || నిలువుల_సంఖ్య <= 0 )
    {
        release();
        అడ్డువరుసలు = నిలువుల_సంఖ్య = 0;
    }
}


UMat::UMat(స్థిరరాశి UMat& m, స్థిరరాశి Rect& roi)
    : flags(m.flags), dims(2), అడ్డువరుసలు(roi.ఎత్తు), నిలువుల_సంఖ్య(roi.వెడల్పు),
    allocator(m.allocator), usageFlags(m.usageFlags), u(m.u),
    offset(m.offset + roi.y*m.అడుగు[0]), size(2)
{
    CV_Assert( m.dims <= 2 );

    size_t esz = CV_ELEM_కొలత(flags);
    offset += roi.x*esz;
    CV_Assert( 0 <= roi.x && 0 <= roi.వెడల్పు && roi.x + roi.వెడల్పు <= m.నిలువుల_సంఖ్య &&
              0 <= roi.y && 0 <= roi.ఎత్తు && roi.y + roi.ఎత్తు <= m.అడ్డువరుసలు );
    ఒకవేళ( roi.వెడల్పు < m.నిలువుల_సంఖ్య || roi.ఎత్తు < m.అడ్డువరుసలు )
        flags |= SUBMATRIX_FLAG;

    size[0] = అడ్డువరుసలు;
    size[1] = నిలువుల_సంఖ్య;
    అడుగు[0] = m.dims == 2 ? m.అడుగు[0] : నిలువుల_సంఖ్య*esz;
    అడుగు[1] = esz;

    updateContinuityFlag();

    addref();
    ఒకవేళ( అడ్డువరుసలు <= 0 || నిలువుల_సంఖ్య <= 0 )
    {
        అడ్డువరుసలు = నిలువుల_సంఖ్య = 0;
        release();
    }
}


UMat::UMat(స్థిరరాశి UMat& m, స్థిరరాశి Range* ranges)
    : flags(MAGIC_VAL), dims(0), అడ్డువరుసలు(0), నిలువుల_సంఖ్య(0), allocator(0), usageFlags(USAGE_DEFAULT), u(0), offset(0)
{
    పూర్ణాంకం i, d = m.dims;

    CV_Assert(ranges);
    కొరకు( i = 0; i < d; i++ )
    {
        Range r = ranges[i];
        CV_Assert( r == Range::all() || (0 <= r.ప్రారంభం && r.ప్రారంభం < r.end && r.end <= m.size[i]) );
    }
    *ఇది = m;
    కొరకు( i = 0; i < d; i++ )
    {
        Range r = ranges[i];
        ఒకవేళ( r != Range::all() && r != Range(0, size.p[i]))
        {
            size.p[i] = r.end - r.ప్రారంభం;
            offset += r.ప్రారంభం*అడుగు.p[i];
            flags |= SUBMATRIX_FLAG;
        }
    }

    ఒకవేళ (d <= 2) {
        అడ్డువరుసలు = d == 2 ? size[0] : 1;
        నిలువుల_సంఖ్య = d <= 0 ? (d >= 0) : size[d > 1];
    }
    updateContinuityFlag();
}

UMat::UMat(స్థిరరాశి UMat& m, స్థిరరాశి std::vector<Range>& ranges)
    : flags(MAGIC_VAL), dims(0), అడ్డువరుసలు(0), నిలువుల_సంఖ్య(0), allocator(0), usageFlags(USAGE_DEFAULT), u(0), offset(0)
{
    పూర్ణాంకం i, d = m.dims;

    CV_Assert((పూర్ణాంకం)ranges.size() == d);
    కొరకు (i = 0; i < d; i++)
    {
        Range r = ranges[i];
        CV_Assert(r == Range::all() || (0 <= r.ప్రారంభం && r.ప్రారంభం < r.end && r.end <= m.size[i]));
    }
    *ఇది = m;
    కొరకు (i = 0; i < d; i++)
    {
        Range r = ranges[i];
        ఒకవేళ (r != Range::all() && r != Range(0, size.p[i]))
        {
            size.p[i] = r.end - r.ప్రారంభం;
            offset += r.ప్రారంభం*అడుగు.p[i];
            flags |= SUBMATRIX_FLAG;
        }
    }

    ఒకవేళ (d <= 2) {
        అడ్డువరుసలు = d == 2 ? size[0] : 1;
        నిలువుల_సంఖ్య = d <= 0 ? (d >= 0) : size[d > 1];
    }
    updateContinuityFlag();
}

UMat UMat::diag(పూర్ణాంకం d) స్థిరరాశి
{
    CV_Assert( dims <= 2 );
    UMat m = *ఇది;
    size_t esz = elem_కొలత();
    పూర్ణాంకం len;

    ఒకవేళ( d >= 0 )
    {
        len = std::min(నిలువుల_సంఖ్య - d, అడ్డువరుసలు);
        m.offset += esz*d;
    }
    లేకపోతే
    {
        len = std::min(అడ్డువరుసలు + d, నిలువుల_సంఖ్య);
        m.offset -= అడుగు[0]*d;
    }
    CV_DbgAssert( len > 0 );

    m.size[0] = m.అడ్డువరుసలు = len;
    m.size[1] = m.నిలువుల_సంఖ్య = 1;
    m.అడుగు[0] += (len > 1 ? esz : 0);

    m.updateContinuityFlag();

    ఒకవేళ( size() != కొలత(1,1) )
        m.flags |= SUBMATRIX_FLAG;

    తిరిగి m;
}

శూన్య UMat::locateROI( కొలత& సంపూర్ణ_కొలత, బిందువు& ofs ) స్థిరరాశి
{
    CV_Assert( dims <= 2 && అడుగు[0] > 0 );
    size_t esz = elem_కొలత(), minstep;
    ptrdiff_t delta1 = (ptrdiff_t)offset, delta2 = (ptrdiff_t)u->size;

    ఒకవేళ( delta1 == 0 )
        ofs.x = ofs.y = 0;
    లేకపోతే
    {
        ofs.y = (పూర్ణాంకం)(delta1/అడుగు[0]);
        ofs.x = (పూర్ణాంకం)((delta1 - అడుగు[0]*ofs.y)/esz);
        CV_DbgAssert( offset == (size_t)(ofs.y*అడుగు[0] + ofs.x*esz) );
    }
    minstep = (ofs.x + నిలువుల_సంఖ్య)*esz;
    సంపూర్ణ_కొలత.ఎత్తు = (పూర్ణాంకం)((delta2 - minstep)/అడుగు[0] + 1);
    సంపూర్ణ_కొలత.ఎత్తు = std::max(సంపూర్ణ_కొలత.ఎత్తు, ofs.y + అడ్డువరుసలు);
    సంపూర్ణ_కొలత.వెడల్పు = (పూర్ణాంకం)((delta2 - అడుగు*(సంపూర్ణ_కొలత.ఎత్తు-1))/esz);
    సంపూర్ణ_కొలత.వెడల్పు = std::max(సంపూర్ణ_కొలత.వెడల్పు, ofs.x + నిలువుల_సంఖ్య);
}


UMat& UMat::adjustROI( పూర్ణాంకం dtop, పూర్ణాంకం dbottom, పూర్ణాంకం dleft, పూర్ణాంకం dright )
{
    CV_Assert( dims == 2 && అడుగు[0] > 0 );
    కొలత సంపూర్ణ_కొలత; బిందువు ofs;
    size_t esz = elem_కొలత();
    locateROI( సంపూర్ణ_కొలత, ofs );
    పూర్ణాంకం row1 = std::min(std::max(ofs.y - dtop, 0), సంపూర్ణ_కొలత.ఎత్తు), row2 = std::max(0, std::min(ofs.y + అడ్డువరుసలు + dbottom, సంపూర్ణ_కొలత.ఎత్తు));
    పూర్ణాంకం col1 = std::min(std::max(ofs.x - dleft, 0), సంపూర్ణ_కొలత.వెడల్పు), col2 = std::max(0, std::min(ofs.x + నిలువుల_సంఖ్య + dright, సంపూర్ణ_కొలత.వెడల్పు));
    ఒకవేళ(row1 > row2)
        std::swap(row1, row2);
    ఒకవేళ(col1 > col2)
        std::swap(col1, col2);

    offset += (row1 - ofs.y)*అడుగు + (col1 - ofs.x)*esz;
    అడ్డువరుసలు = row2 - row1; నిలువుల_సంఖ్య = col2 - col1;
    size.p[0] = అడ్డువరుసలు; size.p[1] = నిలువుల_సంఖ్య;
    updateContinuityFlag();
    తిరిగి *ఇది;
}


UMat UMat::reshape(పూర్ణాంకం new_cn, పూర్ణాంకం new_అడ్డువరుసలు) స్థిరరాశి
{
    పూర్ణాంకం cn = channels();
    UMat hdr = *ఇది;

    ఒకవేళ( dims > 2 && new_అడ్డువరుసలు == 0 && new_cn != 0 && size[dims-1]*cn % new_cn == 0 )
    {
        hdr.flags = (hdr.flags & ~CV_MAT_CN_ముసుగు) | ((new_cn-1) << CV_CN_జరుపు);
        hdr.అడుగు[dims-1] = CV_ELEM_కొలత(hdr.flags);
        hdr.size[dims-1] = hdr.size[dims-1]*cn / new_cn;
        తిరిగి hdr;
    }

    CV_Assert( dims <= 2 );

    ఒకవేళ( new_cn == 0 )
        new_cn = cn;

    పూర్ణాంకం మొత్తం_వెడల్పు = నిలువుల_సంఖ్య * cn;

    ఒకవేళ( (new_cn > మొత్తం_వెడల్పు || మొత్తం_వెడల్పు % new_cn != 0) && new_అడ్డువరుసలు == 0 )
        new_అడ్డువరుసలు = అడ్డువరుసలు * మొత్తం_వెడల్పు / new_cn;

    ఒకవేళ( new_అడ్డువరుసలు != 0 && new_అడ్డువరుసలు != అడ్డువరుసలు )
    {
        పూర్ణాంకం మొత్తం_కొలత_2 = మొత్తం_వెడల్పు * అడ్డువరుసలు;
        ఒకవేళ( !isContinuous() )
            CV_Error( cv::Error::Bad_అడుగు,
            "The matrix is not continuous, thus its number of rows can not be changed" );

        మొత్తం_వెడల్పు = మొత్తం_కొలత_2 / new_అడ్డువరుసలు;

        ఒకవేళ( మొత్తం_వెడల్పు * new_అడ్డువరుసలు != మొత్తం_కొలత_2 )
            CV_Error( cv::Error::StsBadArg, "The total number of matrix elements "
                                    "is not divisible by the new number of rows" );

        hdr.size[0] = hdr.అడ్డువరుసలు = new_అడ్డువరుసలు;
        hdr.అడుగు[0] = మొత్తం_వెడల్పు * elemSize1();
    } లేకపోతే {
        hdr.size[0] = hdr.అడ్డువరుసలు = అడ్డువరుసలు;
        ఒకవేళ (dims <= 1)
            hdr.అడుగు[0] = నిలువుల_సంఖ్య * CV_ELEM_కొలత(flags);
    }

    పూర్ణాంకం new_వెడల్పు = మొత్తం_వెడల్పు / new_cn;

    ఒకవేళ( new_వెడల్పు * new_cn != మొత్తం_వెడల్పు )
        CV_Error( cv::Error::Bad_అంకె_Channels,
        "The total width is not divisible by the new number of channels" );

    hdr.size.dims = hdr.dims = 2;
    hdr.size[1] = hdr.నిలువుల_సంఖ్య = new_వెడల్పు;
    hdr.flags = (hdr.flags & ~CV_MAT_CN_ముసుగు) | ((new_cn-1) << CV_CN_జరుపు);
    hdr.అడుగు[1] = CV_ELEM_కొలత(hdr.flags);
    తిరిగి hdr;
}

#నిర్వచితమైతే HAVE_OPENCL
నామాకాశ {
స్థిర తార్కిక ocl_సమితి_Diag(స్థిరరాశి UMat& d, UMat& m, పూర్ణాంకం len)
{
    పూర్ణాంకం cn = d.channels();
    పూర్ణాంకం లోతు = d.లోతు();

    ఒకవేళ (లోతు == CV_64F && !ocl::Device::getDefault().doubleFPConfig())
        తిరిగి అబద్ధం;

    String opts = format("-D SET_DIAG -D T1=%s -D cn=%d -D IS_ROW_VECTOR=%d",
                         ocl::memop_Type_గమ్యం_Str(లోతు),
                         cn,
                         (d.అడ్డువరుసలు == 1) ? 1 : 0);

    ocl::Kernel k("setDiag", ocl::core::copyset_oclsrc, opts);
    ఒకవేళ (k.empty())
        తిరిగి అబద్ధం;

    k.వాదనలు(ocl::KernelArg::WriteOnly(m),
           ocl::KernelArg::Read_Only_No_కొలత(d),
           len);

    size_t globalsize[2] = { (size_t)len, (size_t)len };
    తిరిగి k.నిరంతరం(2, globalsize, ఖాళీ, అబద్ధం);
}
}
#అయితే_ముగింపు

UMat UMat::diag(స్థిరరాశి UMat& d, UMatUsageFlags usageFlags)
{
    CV_INSTRUMENT_REGION();

    CV_Assert(d.నిలువుల_సంఖ్య == 1 || d.అడ్డువరుసలు == 1);
    పూర్ణాంకం len = d.అడ్డువరుసలు + d.నిలువుల_సంఖ్య - 1;

#నిర్వచితమైతే HAVE_OPENCL
    ఒకవేళ (ocl::useOpenCL())
    {
        UMat m(len, len, d.type(), usageFlags);
        ఒకవేళ (ocl_సమితి_Diag(d, m, len))
        {
            CV_IMPL_కూడు(CV_IMPL_OCL);
            తిరిగి m;
        }
    }
#అయితే_ముగింపు

    UMat m(len, len, d.type(), Scalar(0), usageFlags);
    UMat md = m.diag();
    ఒకవేళ( d.నిలువుల_సంఖ్య == 1 )
        d.copy_గమ్యం(md);
    లేకపోతే
        transpose(d, md);
    తిరిగి m;
}

పూర్ణాంకం UMat::check_సదిశ(పూర్ణాంకం _elemChannels, పూర్ణాంకం లోతు_2, తార్కిక _requireContinuous) స్థిరరాశి
{
    తిరిగి (లోతు() == లోతు_2 || లోతు_2 <= 0) &&
        (isContinuous() || !_requireContinuous) &&
        ((dims <= 2 && (((అడ్డువరుసలు == 1 || నిలువుల_సంఖ్య == 1) && channels() == _elemChannels) ||
                        (నిలువుల_సంఖ్య == _elemChannels && channels() == 1))) ||
        (dims == 3 && channels() == 1 && size.p[2] == _elemChannels && (size.p[0] == 1 || size.p[1] == 1) &&
         (isContinuous() || అడుగు.p[1] == అడుగు.p[2]*size.p[2])))
    ? (పూర్ణాంకం)(మొత్తం()*channels()/_elemChannels) : -1;
}

UMat UMat::reshape(పూర్ణాంకం _cn, పూర్ణాంకం _newndims, స్థిరరాశి పూర్ణాంకం* _newsz) స్థిరరాశి
{
    ఒకవేళ(_newndims == dims)
    {
        ఒకవేళ(_newsz == 0)
            తిరిగి reshape(_cn);
        ఒకవేళ(_newndims == 2)
            తిరిగి reshape(_cn, _newsz[0]);
    }

    ఒకవేళ (isContinuous())
    {
        CV_Assert(_cn >= 0 && _newndims >= 0 && _newndims <= CV_MAX_DIM && (_newndims == 0 || _newsz != 0));

        ఒకవేళ (_cn == 0)
            _cn = ఇది->channels();
        లేకపోతే
            CV_Assert(_cn <= CV_CN_MAX);

        size_t మొత్తం_elem1_ref = ఇది->మొత్తం() * ఇది->channels();
        size_t మొత్తం_elem1 = _cn;

        Auto_బఫరు<పూర్ణాంకం, 4> newsz_buf( (size_t)std::max(_newndims, 1) );

        పూర్ణాంకం m1_idx = -1;

        కొరకు (పూర్ణాంకం i = 0; i < _newndims; i++)
        {
            ఒకవేళ (_newsz[i] >= 0) {
                ఒకవేళ (_newsz[i] == 0 && i < dims)
                    newsz_buf[i] = size.p[i];
                లేకపోతే
                    newsz_buf[i] = _newsz[i];
                మొత్తం_elem1 *= (size_t)newsz_buf[i];
            } లేకపోతే {
                ఒకవేళ (m1_idx >= 0)
                    CV_Error(cv::Error::Sts_Bad_కొలత, "More than one '-1' occured in the new shape");
                m1_idx = i;
            }
        }

        ఒకవేళ (m1_idx >= 0) {
            ఒకవేళ (మొత్తం_elem1 == 0) {
                CV_Assert(మొత్తం_elem1_ref == 0);
                మొత్తం_elem1 = 1;
            }
            CV_Assert(మొత్తం_elem1_ref % మొత్తం_elem1 == 0);
            newsz_buf[m1_idx] = (పూర్ణాంకం)(మొత్తం_elem1_ref / మొత్తం_elem1);
        } లేకపోతే ఒకవేళ (మొత్తం_elem1 != మొత్తం_elem1_ref) {
            CV_Error(cv::Error::StsUnmatchedSizes, "Requested and source matrices have different count of elements");
        }

        UMat hdr = *ఇది;
        hdr.flags = (hdr.flags & ~CV_MAT_CN_ముసుగు) | ((_cn-1) << CV_CN_జరుపు);
        సమితి_కొలత(hdr, _newndims, newsz_buf.దత్తాంశం(), ఖాళీ, నిజం);

        తిరిగి hdr;
    }

    CV_Error(cv::Error::StsNotImplemented, "Reshaping of n-dimensional non-continuous matrices is not supported yet");
}

UMat UMat::reshape(పూర్ణాంకం _cn, స్థిరరాశి MatShape& _newshape) స్థిరరాశి
{
    ఒకవేళ (_newshape.dims < 0) {
        పూర్ణాంకం newshape[] = {0};
        తిరిగి reshape(_cn, 1, newshape);
    }
    తిరిగి reshape(_cn, _newshape.dims, _newshape.p);
}

Mat UMat::getMat(AccessFlag accessFlags) స్థిరరాశి
{
    ఒకవేళ(!u)
        తిరిగి Mat();
    // TODO Support ACCESS_READ (ACCESS_WRITE) without unnecessary data transfers
    accessFlags |= ACCESS_RW;
    UMat_దత్తాంశం_Auto_తాళం autolock(u);
    ప్రయత్నించు
    {
        ఒకవేళ(CV_XADD(&u->refcount, 1) == 0)
            u->currAllocator->map(u, accessFlags);
        ఒకవేళ (u->దత్తాంశం != 0)
        {
            Mat hdr(dims, size.p, type(), u->దత్తాంశం + offset, అడుగు.p);
            hdr.flags = flags;
            hdr.u = u;
            hdr.datastart = u->దత్తాంశం;
            hdr.దత్తాంశం = u->దత్తాంశం + offset;
            hdr.datalimit = hdr.dataend = u->దత్తాంశం + u->size;
            తిరిగి hdr;
        }
    }
    పట్టుకో(...)
    {
        CV_XADD(&u->refcount, -1);
        విసురు;
    }
    CV_XADD(&u->refcount, -1);
    CV_Assert(u->దత్తాంశం != 0 && "Error mapping of UMat to host memory.");
    తిరిగి Mat();
}

శూన్య* UMat::handle(AccessFlag accessFlags) స్థిరరాశి
{
    ఒకవేళ( !u )
        తిరిగి 0;

    CV_Assert(u->refcount == 0);
    CV_Assert(!u->deviceCopyObsolete() || u->copyOnMap());
    ఒకవేళ (u->deviceCopyObsolete())
    {
        u->currAllocator->unmap(u);
    }

    ఒకవేళ (!!(accessFlags & ACCESS_WRITE))
        u->markHostCopyObsolete(నిజం);

    తిరిగి u->handle;
}

శూన్య UMat::ndoffset(size_t* ofs) స్థిరరాశి
{
    // offset = step[0]*ofs[0] + step[1]*ofs[1] + step[2]*ofs[2] + ...;
    size_t val = offset;
    కొరకు( పూర్ణాంకం i = 0; i < dims; i++ )
    {
        size_t s = అడుగు.p[i];
        ofs[i] = val / s;
        val -= ofs[i]*s;
    }
}

శూన్య UMat::copy_గమ్యం(OutputArray _dst) స్థిరరాశి
{
    CV_INSTRUMENT_REGION();

#నిర్వచితమైతే HAVE_CUDA
    ఒకవేళ (_dst.isGpuMat())
    {
        _dst.getGpuMat().upload(*ఇది);
        తిరిగి;
    }
#అయితే_ముగింపు

    పూర్ణాంకం stype = type();
    పూర్ణాంకం dtype = _dst.type();
    ఒకవేళ( _dst.fixedType() && dtype != stype )
    {
        CV_Assert( CV_MAT_CN(stype) == CV_MAT_CN(dtype) );
        convert_గమ్యం( _dst, dtype );
        తిరిగి;
    }

    ఒకవేళ( dims == 0 && empty() )
    {
        _dst.release();
        శూన్య* obj = _dst.getObj();
        ఒకవేళ (_dst.isMat())
            reinterpret_cast<Mat*>(obj)->flags = Mat::MAGIC_VAL | Mat::CONTINUOUS_FLAG | stype;
        లేకపోతే ఒకవేళ (_dst.isUMat())
            reinterpret_cast<UMat*>(obj)->flags = UMat::MAGIC_VAL | UMat::CONTINUOUS_FLAG | stype;
        లేకపోతే ఒకవేళ (_dst.isGpuMat())
            reinterpret_cast<cuda::GpuMat*>(obj)->flags = stype;
        తిరిగి;
    }

    _dst.create( dims, size.p, stype );
    ఒకవేళ (empty())
        తిరిగి;

    size_t sz[CV_MAX_DIM] = {1}, srcofs[CV_MAX_DIM]={0}, dstofs[CV_MAX_DIM]={0};
    size_t esz = CV_ELEM_కొలత(stype);
    పూర్ణాంకం i, d = std::max(dims, 1);
    కొరకు( i = 0; i < d; i++ )
        sz[i] = size.p[i];
    sz[d-1] *= esz;
    ndoffset(srcofs);
    srcofs[d-1] *= esz;

    ఒకవేళ( _dst.isUMat() )
    {
        UMat dst = _dst.getUMat();
        CV_Assert(dst.u);
        ఒకవేళ( u == dst.u && dst.offset == offset )
            తిరిగి;

        ఒకవేళ (u->currAllocator == dst.u->currAllocator)
        {
            dst.ndoffset(dstofs);
            dstofs[dims-1] *= esz;
            u->currAllocator->copy(u, dst.u, d, sz, srcofs, అడుగు.p, dstofs, dst.అడుగు.p, అబద్ధం);
            తిరిగి;
        }

        // different allocators: never map a device destination, upload into it instead
        // (through a host Mat when both sides are device memory)
        MatAllocator* hostAllocator = Mat::getDefaultAllocator();
        ఒకవేళ (dst.u->currAllocator != hostAllocator)
        {
            ఒకవేళ (u->currAllocator == hostAllocator)
                getMat(ACCESS_READ).copy_గమ్యం(_dst);
            లేకపోతే
            {
                Mat tmp;
                copy_గమ్యం(tmp);
                tmp.copy_గమ్యం(_dst);
            }
            తిరిగి;
        }
    }

    Mat dst = _dst.getMat();
    u->currAllocator->download(u, dst.ptr(), d, sz, srcofs, అడుగు.p, dst.అడుగు.p);
}

శూన్య UMat::copy_గమ్యం(OutputArray _dst, ఆదానం_Array ముసుగు_2) స్థిరరాశి
{
    CV_INSTRUMENT_REGION();

    ఒకవేళ( ముసుగు_2.empty() )
    {
        copy_గమ్యం(_dst);
        తిరిగి;
    }
#నిర్వచితమైతే HAVE_OPENCL
    పూర్ణాంకం cn = channels(), mtype = ముసుగు_2.type(), mdepth = CV_MAT_లోతు(mtype), mcn = CV_MAT_CN(mtype);
    CV_Assert( (mdepth == CV_8U || mdepth == CV_8S || mdepth == CV_Bool) && (mcn == 1 || mcn == cn) );

    ఒకవేళ (ocl::useOpenCL() && _dst.isUMat() && dims <= 2)
    {
        UMat_దత్తాంశం * prevu = _dst.getUMat().u;
        _dst.create( size, type() );

        UMat dst = _dst.getUMat();

        తార్కిక haveDstUninit = అబద్ధం;
        ఒకవేళ( prevu != dst.u ) // do not leave dst uninitialized
            haveDstUninit = నిజం;

        String opts = format("-D COPY_TO_MASK -D T1=%s -D scn=%d -D mcn=%d%s",
                             ocl::memop_Type_గమ్యం_Str(లోతు()), cn, mcn,
                             haveDstUninit ? " -D HAVE_DST_UNINIT" : "");

        ocl::Kernel k("copyToMask", ocl::core::copyset_oclsrc, opts);
        ఒకవేళ (!k.empty())
        {
            k.వాదనలు(ocl::KernelArg::Read_Only_No_కొలత(*ఇది),
                   ocl::KernelArg::Read_Only_No_కొలత(ముసుగు_2.getUMat()),
                   haveDstUninit ? ocl::KernelArg::WriteOnly(dst) :
                                   ocl::KernelArg::ReadWrite(dst));

            size_t globalsize[2] = { (size_t)నిలువుల_సంఖ్య, (size_t)అడ్డువరుసలు };
            ఒకవేళ (k.నిరంతరం(2, globalsize, ఖాళీ, అబద్ధం))
            {
                CV_IMPL_కూడు(CV_IMPL_OCL);
                తిరిగి;
            }
        }
    }
#అయితే_ముగింపు
    Mat src = getMat(ACCESS_READ);
    src.copy_గమ్యం(_dst, ముసుగు_2);
}


//
// void UMat::convertTo moved to convert.dispatch.cpp
//

UMat& UMat::సమితి_గమ్యం(ఆదానం_Array విలువ, ఆదానం_Array ముసుగు_2)
{
    CV_INSTRUMENT_REGION();

    తార్కిక have_ముసుగు = !ముసుగు_2.empty();
#నిర్వచితమైతే HAVE_OPENCL
    పూర్ణాంకం tp = type(), cn = CV_MAT_CN(tp), d = CV_MAT_లోతు(tp);

    ఒకవేళ( dims <= 2 && cn <= 4 && CV_MAT_లోతు(tp) < CV_64F && ocl::useOpenCL() )
    {
        Mat value = విలువ.getMat();
        CV_Assert( checkScalar(value, type(), విలువ.రకం(), ఆదానం_Array_2::UMAT) );
        పూర్ణాంకం kercn = have_ముసుగు || cn == 3 ? cn : std::max(cn, ocl::predict_Optimal_సదిశ_వెడల్పు(*ఇది)),
                kertp = CV_MAKE_TYPE(d, kercn);

        ద్విగుణ buf[16] = { 0, 0, 0, 0, 0, 0, 0, 0,
                           0, 0, 0, 0, 0, 0, 0, 0 };
        convertAndUnrollScalar(value, tp, (uchar *)buf, kercn / cn);

        పూర్ణాంకం scalarcn = kercn == 3 ? 4 : kercn, అడ్డువరుసలు_చొప్పున_WI = ocl::Device::getDefault().isIntel() ? 4 : 1;
        String opts = format("-D dstT=%s -D rowsPerWI=%d -D dstST=%s -D dstT1=%s -D cn=%d",
                             ocl::memop_Type_గమ్యం_Str(kertp), అడ్డువరుసలు_చొప్పున_WI,
                             ocl::memop_Type_గమ్యం_Str(CV_MAKETYPE(d, scalarcn)),
                             ocl::memop_Type_గమ్యం_Str(d), kercn);

        ocl::Kernel సమితి_K(have_ముసుగు ? "setMask" : "set", ocl::core::copyset_oclsrc, opts);
        ఒకవేళ( !సమితి_K.empty() )
        {
            ocl::KernelArg scalararg(ocl::KernelArg::CONSTANT, 0, 0, 0, buf, CV_ELEM_కొలత(d) * scalarcn);
            UMat ముసుగు;

            ఒకవేళ( have_ముసుగు )
            {
                ముసుగు = ముసుగు_2.getUMat();
                CV_Assert( ముసుగు.size() == size() && (ముసుగు.type() == CV_8U || ముసుగు.type() == CV_8S || ముసుగు.type() == CV_Bool) );
                ocl::KernelArg maskarg = ocl::KernelArg::Read_Only_No_కొలత(ముసుగు),
                        dstarg = ocl::KernelArg::ReadWrite(*ఇది);
                సమితి_K.వాదనలు(maskarg, dstarg, scalararg);
            }
            లేకపోతే
            {
                ocl::KernelArg dstarg = ocl::KernelArg::WriteOnly(*ఇది, cn, kercn);
                సమితి_K.వాదనలు(dstarg, scalararg);
            }

            size_t globalsize[] = { (size_t)నిలువుల_సంఖ్య * cn / kercn, ((size_t)అడ్డువరుసలు + అడ్డువరుసలు_చొప్పున_WI - 1) / అడ్డువరుసలు_చొప్పున_WI };
            ఒకవేళ( సమితి_K.నిరంతరం(2, globalsize, ఖాళీ, అబద్ధం) )
            {
                CV_IMPL_కూడు(CV_IMPL_OCL);
                తిరిగి *ఇది;
            }
        }
    }
#అయితే_ముగింపు
    Mat m = getMat(have_ముసుగు ? ACCESS_RW : ACCESS_WRITE);
    m.సమితి_గమ్యం(విలువ, ముసుగు_2);
    తిరిగి *ఇది;
}

UMat& UMat::క్రియాకారకం = (స్థిరరాశి Scalar& s)
{
    సమితి_గమ్యం(s);
    తిరిగి *ఇది;
}

UMat UMat::t() స్థిరరాశి
{
    UMat m;
    transpose(*ఇది, m);
    తిరిగి m;
}

UMat UMat::zeros(పూర్ణాంకం అడ్డువరుసలు, పూర్ణాంకం నిలువుల_సంఖ్య, పూర్ణాంకం type, UMatUsageFlags usageFlags)
{
    తిరిగి UMat(అడ్డువరుసలు, నిలువుల_సంఖ్య, type, Scalar::all(0), usageFlags);
}

UMat UMat::zeros(కొలత size, పూర్ణాంకం type, UMatUsageFlags usageFlags)
{
    తిరిగి UMat(size, type, Scalar::all(0), usageFlags);
}

UMat UMat::zeros(పూర్ణాంకం ndims, స్థిరరాశి పూర్ణాంకం* sz, పూర్ణాంకం type, UMatUsageFlags usageFlags)
{
    తిరిగి UMat(ndims, sz, type, Scalar::all(0), usageFlags);
}

UMat UMat::zeros(స్థిరరాశి MatShape& shape, పూర్ణాంకం type, UMatUsageFlags usageFlags)
{
    తిరిగి UMat(shape.dims, shape.p, type, Scalar::all(0), usageFlags);
}

UMat UMat::ones(పూర్ణాంకం అడ్డువరుసలు, పూర్ణాంకం నిలువుల_సంఖ్య, పూర్ణాంకం type, UMatUsageFlags usageFlags)
{
    తిరిగి UMat(అడ్డువరుసలు, నిలువుల_సంఖ్య, type, Scalar(1), usageFlags);
}

UMat UMat::ones(కొలత size, పూర్ణాంకం type, UMatUsageFlags usageFlags)
{
    తిరిగి UMat(size, type, Scalar(1), usageFlags);
}

UMat UMat::ones(పూర్ణాంకం ndims, స్థిరరాశి పూర్ణాంకం* sz, పూర్ణాంకం type, UMatUsageFlags usageFlags)
{
    తిరిగి UMat(ndims, sz, type, Scalar(1), usageFlags);
}

UMat UMat::ones(స్థిరరాశి MatShape& shape, పూర్ణాంకం type, UMatUsageFlags usageFlags)
{
    తిరిగి UMat(shape.dims, shape.p, type, Scalar(1), usageFlags);
}

}

/* End of file. */
