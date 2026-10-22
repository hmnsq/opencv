// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ضمن "opencv2/core/utils/buffer_area.private.hpp"
#ضمن "opencv2/core/utils/configuration.private.hpp"

#جيڪڏهن_وصف_نه_ٿيل OPENCV_ENABLE_يادگيري_SANITIZER
ساكن منطقي CV_بفر_AREA_OVERRIDE_محفوظ_گهڻائي =
    cv::utils::getConfigurationParameterBool("OPENCV_BUFFER_AREA_ALWAYS_SAFE", خطأ);
#جيڪڏهن_پورو

نطاق cv { نطاق utils {

//==================================================================================================

صنف بفر_Area::بلاڪ
{
خاص:
    مضمن size_t reserve_ڳڻپ() ثابت
    {
        أرجع alignment / type_سائيز - 1;
    }
عام:
    بلاڪ(فراغ **ptr_, ushort type_سائيز_2, size_t ڳڻپ, ushort alignment_)
        : ptr(ptr_), raw_mem(0), ڳڻپ_2(ڳڻپ), type_سائيز(type_سائيز_2), alignment(alignment_)
    {
        CV_Assert(ptr && *ptr == عدم);
    }
    فراغ cleanup() ثابت
    {
        CV_DbgAssert(ptr);
        إذا (raw_mem)
            fastFree(raw_mem);
    }
    size_t get_Byte_ڳڻپ() ثابت
    {
        أرجع type_سائيز * (ڳڻپ_2 + reserve_ڳڻپ());
    }
    فراغ real_allocate()
    {
        CV_Assert(ptr && *ptr == عدم);
        ثابت size_t allocated_ڳڻپ = ڳڻپ_2 + reserve_ڳڻپ();
        raw_mem = fastMalloc(type_سائيز * allocated_ڳڻپ);
        إذا (alignment != type_سائيز)
        {
            *ptr = alignPtr(raw_mem, alignment);
            CV_Assert(reinterpret_cast<size_t>(*ptr) % alignment == 0);
            CV_Assert(static_cast<uchar*>(*ptr) + type_سائيز * ڳڻپ_2 <= static_cast<uchar*>(raw_mem) + type_سائيز * allocated_ڳڻپ);
        }
        وإلا
        {
            *ptr = raw_mem;
        }
    }
#جيڪڏهن_وصف_نه_ٿيل OPENCV_ENABLE_يادگيري_SANITIZER
    فراغ * fast_allocate(فراغ * buf) ثابت
    {
        CV_Assert(ptr && *ptr == عدم);
        buf = alignPtr(buf, alignment);
        CV_Assert(reinterpret_cast<size_t>(buf) % alignment == 0);
        *ptr = buf;
        أرجع static_cast<فراغ*>(static_cast<uchar*>(*ptr) + type_سائيز * ڳڻپ_2);
    }
#جيڪڏهن_پورو
    منطقي عامل==(فراغ **other) ثابت
    {
        CV_Assert(ptr && other);
        أرجع *ptr == *other;
    }
    فراغ zeroFill() ثابت
    {
        CV_Assert(ptr && *ptr);
        memset(static_cast<uchar*>(*ptr), 0, ڳڻپ_2 * type_سائيز);
    }
خاص:
    فراغ **ptr;
    فراغ * raw_mem;
    size_t ڳڻپ_2;
    ushort type_سائيز;
    ushort alignment;
};

//==================================================================================================

#جيڪڏهن_وصف_نه_ٿيل OPENCV_ENABLE_يادگيري_SANITIZER
بفر_Area::بفر_Area(منطقي محفوظ) :
    oneBuf(0),
    ڪل_سائيز(0),
    محفوظ_2(محفوظ || CV_بفر_AREA_OVERRIDE_محفوظ_گهڻائي)
{
    // nothing
}
#وإلا
بفر_Area::بفر_Area(منطقي محفوظ)
{
    CV_اڻ_استعمال_ٿيل(محفوظ);
}
#جيڪڏهن_پورو

بفر_Area::~بفر_Area()
{
    release();
}

فراغ بفر_Area::allocate_(فراغ **ptr, ushort type_سائيز, size_t ڳڻپ_2, ushort alignment)
{
    بلاڪون.push_back(بلاڪ(ptr, type_سائيز, ڳڻپ_2, alignment));
#جيڪڏهن_وصف_نه_ٿيل OPENCV_ENABLE_يادگيري_SANITIZER
    إذا (!محفوظ_2)
    {
        ڪل_سائيز += بلاڪون.back().get_Byte_ڳڻپ();
    }
    وإلا
#جيڪڏهن_پورو
    {
        بلاڪون.back().real_allocate();
    }
}

فراغ بفر_Area::zeroFill_(فراغ **ptr)
{
    لكل(std::vector<بلاڪ>::const_iterator i = بلاڪون.begin(); i != بلاڪون.end(); ++i)
    {
        إذا (*i == ptr)
        {
            i->zeroFill();
            اكسر;
        }
    }
}

فراغ بفر_Area::zeroFill()
{
    لكل(std::vector<بلاڪ>::const_iterator i = بلاڪون.begin(); i != بلاڪون.end(); ++i)
    {
        i->zeroFill();
    }
}

فراغ بفر_Area::commit()
{
#جيڪڏهن_وصف_نه_ٿيل OPENCV_ENABLE_يادگيري_SANITIZER
    إذا (!محفوظ_2)
    {
        CV_Assert(ڪل_سائيز > 0);
        CV_Assert(oneBuf == عدم);
        CV_Assert(!بلاڪون.empty());
        oneBuf = fastMalloc(ڪل_سائيز);
        فراغ * ptr = oneBuf;
        لكل(std::vector<بلاڪ>::const_iterator i = بلاڪون.begin(); i != بلاڪون.end(); ++i)
        {
            ptr = i->fast_allocate(ptr);
        }
    }
#جيڪڏهن_پورو
}

فراغ بفر_Area::release()
{
    لكل(std::vector<بلاڪ>::const_iterator i = بلاڪون.begin(); i != بلاڪون.end(); ++i)
    {
        i->cleanup();
    }
    بلاڪون.clear();
#جيڪڏهن_وصف_نه_ٿيل OPENCV_ENABLE_يادگيري_SANITIZER
    إذا (oneBuf)
    {
        fastFree(oneBuf);
        oneBuf = 0;
    }
#جيڪڏهن_پورو
}

//==================================================================================================

}} // cv::utils::
