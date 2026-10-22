// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ضمن "opencv2/core/utils/buffer_area.private.hpp"
#ضمن "opencv2/core/utils/configuration.private.hpp"

#جے_بنا_تعریف OPENCV_ENABLE_حافظہ_SANITIZER
ساكن منطقي CV_بفر_AREA_OVERRIDE_محفوظ_منوال =
    cv::utils::getConfigurationParameterBool("OPENCV_BUFFER_AREA_ALWAYS_SAFE", خطأ);
#جے_انت

نطاق cv { نطاق utils {

//==================================================================================================

صنف بفر_Area::بلاک
{
خاص:
    مضمن size_t reserve_گنتی() ثابت
    {
        أرجع alignment / type_ناپ - 1;
    }
عام:
    بلاک(فراغ **ptr_, ushort type_ناپ_2, size_t گنتی, ushort alignment_)
        : ptr(ptr_), raw_mem(0), گنتی_2(گنتی), type_ناپ(type_ناپ_2), alignment(alignment_)
    {
        CV_Assert(ptr && *ptr == عدم);
    }
    فراغ cleanup() ثابت
    {
        CV_DbgAssert(ptr);
        إذا (raw_mem)
            fastFree(raw_mem);
    }
    size_t get_Byte_گنتی() ثابت
    {
        أرجع type_ناپ * (گنتی_2 + reserve_گنتی());
    }
    فراغ real_allocate()
    {
        CV_Assert(ptr && *ptr == عدم);
        ثابت size_t allocated_گنتی = گنتی_2 + reserve_گنتی();
        raw_mem = fastMalloc(type_ناپ * allocated_گنتی);
        إذا (alignment != type_ناپ)
        {
            *ptr = alignPtr(raw_mem, alignment);
            CV_Assert(reinterpret_cast<size_t>(*ptr) % alignment == 0);
            CV_Assert(static_cast<uchar*>(*ptr) + type_ناپ * گنتی_2 <= static_cast<uchar*>(raw_mem) + type_ناپ * allocated_گنتی);
        }
        وإلا
        {
            *ptr = raw_mem;
        }
    }
#جے_بنا_تعریف OPENCV_ENABLE_حافظہ_SANITIZER
    فراغ * fast_allocate(فراغ * buf) ثابت
    {
        CV_Assert(ptr && *ptr == عدم);
        buf = alignPtr(buf, alignment);
        CV_Assert(reinterpret_cast<size_t>(buf) % alignment == 0);
        *ptr = buf;
        أرجع static_cast<فراغ*>(static_cast<uchar*>(*ptr) + type_ناپ * گنتی_2);
    }
#جے_انت
    منطقي عامل==(فراغ **other) ثابت
    {
        CV_Assert(ptr && other);
        أرجع *ptr == *other;
    }
    فراغ zeroFill() ثابت
    {
        CV_Assert(ptr && *ptr);
        memset(static_cast<uchar*>(*ptr), 0, گنتی_2 * type_ناپ);
    }
خاص:
    فراغ **ptr;
    فراغ * raw_mem;
    size_t گنتی_2;
    ushort type_ناپ;
    ushort alignment;
};

//==================================================================================================

#جے_بنا_تعریف OPENCV_ENABLE_حافظہ_SANITIZER
بفر_Area::بفر_Area(منطقي محفوظ) :
    oneBuf(0),
    کل_ناپ(0),
    محفوظ_2(محفوظ || CV_بفر_AREA_OVERRIDE_محفوظ_منوال)
{
    // nothing
}
#وإلا
بفر_Area::بفر_Area(منطقي محفوظ)
{
    CV_بن_ورتیا(محفوظ);
}
#جے_انت

بفر_Area::~بفر_Area()
{
    release();
}

فراغ بفر_Area::allocate_(فراغ **ptr, ushort type_ناپ, size_t گنتی_2, ushort alignment)
{
    بلاکاں.push_back(بلاک(ptr, type_ناپ, گنتی_2, alignment));
#جے_بنا_تعریف OPENCV_ENABLE_حافظہ_SANITIZER
    إذا (!محفوظ_2)
    {
        کل_ناپ += بلاکاں.back().get_Byte_گنتی();
    }
    وإلا
#جے_انت
    {
        بلاکاں.back().real_allocate();
    }
}

فراغ بفر_Area::zeroFill_(فراغ **ptr)
{
    لكل(std::vector<بلاک>::const_iterator i = بلاکاں.begin(); i != بلاکاں.end(); ++i)
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
    لكل(std::vector<بلاک>::const_iterator i = بلاکاں.begin(); i != بلاکاں.end(); ++i)
    {
        i->zeroFill();
    }
}

فراغ بفر_Area::commit()
{
#جے_بنا_تعریف OPENCV_ENABLE_حافظہ_SANITIZER
    إذا (!محفوظ_2)
    {
        CV_Assert(کل_ناپ > 0);
        CV_Assert(oneBuf == عدم);
        CV_Assert(!بلاکاں.empty());
        oneBuf = fastMalloc(کل_ناپ);
        فراغ * ptr = oneBuf;
        لكل(std::vector<بلاک>::const_iterator i = بلاکاں.begin(); i != بلاکاں.end(); ++i)
        {
            ptr = i->fast_allocate(ptr);
        }
    }
#جے_انت
}

فراغ بفر_Area::release()
{
    لكل(std::vector<بلاک>::const_iterator i = بلاکاں.begin(); i != بلاکاں.end(); ++i)
    {
        i->cleanup();
    }
    بلاکاں.clear();
#جے_بنا_تعریف OPENCV_ENABLE_حافظہ_SANITIZER
    إذا (oneBuf)
    {
        fastFree(oneBuf);
        oneBuf = 0;
    }
#جے_انت
}

//==================================================================================================

}} // cv::utils::
