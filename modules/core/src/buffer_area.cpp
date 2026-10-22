// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ضمن "opencv2/core/utils/buffer_area.private.hpp"
#ضمن "opencv2/core/utils/configuration.private.hpp"

#اگر_تعریف_نشده OPENCV_ENABLE_حافظه_SANITIZER
ساكن منطقي CV_میانگیر_AREA_OVERRIDE_امن_نما =
    cv::utils::getConfigurationParameterBool("OPENCV_BUFFER_AREA_ALWAYS_SAFE", خطأ);
#پایان_اگر

نطاق cv { نطاق utils {

//==================================================================================================

صنف میانگیر_Area::بلوک
{
خاص:
    مضمن size_t reserve_شمار() ثابت
    {
        أرجع alignment / type_اندازه - 1;
    }
عام:
    بلوک(فراغ **ptr_, ushort type_اندازه_2, size_t شمار, ushort alignment_)
        : ptr(ptr_), raw_mem(0), شمار_2(شمار), type_اندازه(type_اندازه_2), alignment(alignment_)
    {
        CV_Assert(ptr && *ptr == عدم);
    }
    فراغ cleanup() ثابت
    {
        CV_DbgAssert(ptr);
        إذا (raw_mem)
            fastFree(raw_mem);
    }
    size_t get_Byte_شمار() ثابت
    {
        أرجع type_اندازه * (شمار_2 + reserve_شمار());
    }
    فراغ real_allocate()
    {
        CV_Assert(ptr && *ptr == عدم);
        ثابت size_t allocated_شمار = شمار_2 + reserve_شمار();
        raw_mem = fastMalloc(type_اندازه * allocated_شمار);
        إذا (alignment != type_اندازه)
        {
            *ptr = alignPtr(raw_mem, alignment);
            CV_Assert(reinterpret_cast<size_t>(*ptr) % alignment == 0);
            CV_Assert(static_cast<uchar*>(*ptr) + type_اندازه * شمار_2 <= static_cast<uchar*>(raw_mem) + type_اندازه * allocated_شمار);
        }
        وإلا
        {
            *ptr = raw_mem;
        }
    }
#اگر_تعریف_نشده OPENCV_ENABLE_حافظه_SANITIZER
    فراغ * fast_allocate(فراغ * buf) ثابت
    {
        CV_Assert(ptr && *ptr == عدم);
        buf = alignPtr(buf, alignment);
        CV_Assert(reinterpret_cast<size_t>(buf) % alignment == 0);
        *ptr = buf;
        أرجع static_cast<فراغ*>(static_cast<uchar*>(*ptr) + type_اندازه * شمار_2);
    }
#پایان_اگر
    منطقي عملگر==(فراغ **other) ثابت
    {
        CV_Assert(ptr && other);
        أرجع *ptr == *other;
    }
    فراغ zeroFill() ثابت
    {
        CV_Assert(ptr && *ptr);
        memset(static_cast<uchar*>(*ptr), 0, شمار_2 * type_اندازه);
    }
خاص:
    فراغ **ptr;
    فراغ * raw_mem;
    size_t شمار_2;
    ushort type_اندازه;
    ushort alignment;
};

//==================================================================================================

#اگر_تعریف_نشده OPENCV_ENABLE_حافظه_SANITIZER
میانگیر_Area::میانگیر_Area(منطقي امن) :
    oneBuf(0),
    مجموع_اندازه(0),
    امن_2(امن || CV_میانگیر_AREA_OVERRIDE_امن_نما)
{
    // nothing
}
#وإلا
میانگیر_Area::میانگیر_Area(منطقي امن)
{
    CV_استفادهنشده(امن);
}
#پایان_اگر

میانگیر_Area::~میانگیر_Area()
{
    release();
}

فراغ میانگیر_Area::allocate_(فراغ **ptr, ushort type_اندازه, size_t شمار_2, ushort alignment)
{
    بلوکها.push_back(بلوک(ptr, type_اندازه, شمار_2, alignment));
#اگر_تعریف_نشده OPENCV_ENABLE_حافظه_SANITIZER
    إذا (!امن_2)
    {
        مجموع_اندازه += بلوکها.back().get_Byte_شمار();
    }
    وإلا
#پایان_اگر
    {
        بلوکها.back().real_allocate();
    }
}

فراغ میانگیر_Area::zeroFill_(فراغ **ptr)
{
    لكل(std::vector<بلوک>::const_iterator i = بلوکها.begin(); i != بلوکها.end(); ++i)
    {
        إذا (*i == ptr)
        {
            i->zeroFill();
            اكسر;
        }
    }
}

فراغ میانگیر_Area::zeroFill()
{
    لكل(std::vector<بلوک>::const_iterator i = بلوکها.begin(); i != بلوکها.end(); ++i)
    {
        i->zeroFill();
    }
}

فراغ میانگیر_Area::commit()
{
#اگر_تعریف_نشده OPENCV_ENABLE_حافظه_SANITIZER
    إذا (!امن_2)
    {
        CV_Assert(مجموع_اندازه > 0);
        CV_Assert(oneBuf == عدم);
        CV_Assert(!بلوکها.empty());
        oneBuf = fastMalloc(مجموع_اندازه);
        فراغ * ptr = oneBuf;
        لكل(std::vector<بلوک>::const_iterator i = بلوکها.begin(); i != بلوکها.end(); ++i)
        {
            ptr = i->fast_allocate(ptr);
        }
    }
#پایان_اگر
}

فراغ میانگیر_Area::release()
{
    لكل(std::vector<بلوک>::const_iterator i = بلوکها.begin(); i != بلوکها.end(); ++i)
    {
        i->cleanup();
    }
    بلوکها.clear();
#اگر_تعریف_نشده OPENCV_ENABLE_حافظه_SANITIZER
    إذا (oneBuf)
    {
        fastFree(oneBuf);
        oneBuf = 0;
    }
#پایان_اگر
}

//==================================================================================================

}} // cv::utils::
