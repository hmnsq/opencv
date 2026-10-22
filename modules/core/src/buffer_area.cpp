// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#شامل "opencv2/core/utils/buffer_area.private.hpp"
#شامل "opencv2/core/utils/configuration.private.hpp"

#اگر_غیر_تعریف_شدہ OPENCV_ENABLE_حافظہ_SANITIZER
ساکن منطقی CV_بفر_AREA_OVERRIDE_محفوظ_منوال =
    cv::utils::getConfigurationParameterBool("OPENCV_BUFFER_AREA_ALWAYS_SAFE", جھوٹ);
#اگر_ختم

نامخلا cv { نامخلا utils {

//==================================================================================================

جماعت بفر_Area::بلاک
{
نجی:
    اندرونی size_t reserve_گنتی() ثابت
    {
        واپس alignment / type_سائز - 1;
    }
عوامی:
    بلاک(خالی **ptr_, ushort type_سائز_2, size_t گنتی, ushort alignment_)
        : ptr(ptr_), raw_mem(0), گنتی_2(گنتی), type_سائز(type_سائز_2), alignment(alignment_)
    {
        CV_Assert(ptr && *ptr == تہی);
    }
    خالی cleanup() ثابت
    {
        CV_DbgAssert(ptr);
        اگر (raw_mem)
            fastFree(raw_mem);
    }
    size_t get_Byte_گنتی() ثابت
    {
        واپس type_سائز * (گنتی_2 + reserve_گنتی());
    }
    خالی real_allocate()
    {
        CV_Assert(ptr && *ptr == تہی);
        ثابت size_t allocated_گنتی = گنتی_2 + reserve_گنتی();
        raw_mem = fastMalloc(type_سائز * allocated_گنتی);
        اگر (alignment != type_سائز)
        {
            *ptr = alignPtr(raw_mem, alignment);
            CV_Assert(reinterpret_cast<size_t>(*ptr) % alignment == 0);
            CV_Assert(static_cast<uchar*>(*ptr) + type_سائز * گنتی_2 <= static_cast<uchar*>(raw_mem) + type_سائز * allocated_گنتی);
        }
        ورنہ
        {
            *ptr = raw_mem;
        }
    }
#اگر_غیر_تعریف_شدہ OPENCV_ENABLE_حافظہ_SANITIZER
    خالی * fast_allocate(خالی * buf) ثابت
    {
        CV_Assert(ptr && *ptr == تہی);
        buf = alignPtr(buf, alignment);
        CV_Assert(reinterpret_cast<size_t>(buf) % alignment == 0);
        *ptr = buf;
        واپس static_cast<خالی*>(static_cast<uchar*>(*ptr) + type_سائز * گنتی_2);
    }
#اگر_ختم
    منطقی عامل==(خالی **other) ثابت
    {
        CV_Assert(ptr && other);
        واپس *ptr == *other;
    }
    خالی zeroFill() ثابت
    {
        CV_Assert(ptr && *ptr);
        memset(static_cast<uchar*>(*ptr), 0, گنتی_2 * type_سائز);
    }
نجی:
    خالی **ptr;
    خالی * raw_mem;
    size_t گنتی_2;
    ushort type_سائز;
    ushort alignment;
};

//==================================================================================================

#اگر_غیر_تعریف_شدہ OPENCV_ENABLE_حافظہ_SANITIZER
بفر_Area::بفر_Area(منطقی محفوظ_2) :
    oneBuf(0),
    کل_سائز(0),
    محفوظ_3(محفوظ_2 || CV_بفر_AREA_OVERRIDE_محفوظ_منوال)
{
    // nothing
}
#ورنہ
بفر_Area::بفر_Area(منطقی محفوظ_2)
{
    CV_غیر_مستعمل(محفوظ_2);
}
#اگر_ختم

بفر_Area::~بفر_Area()
{
    release();
}

خالی بفر_Area::allocate_(خالی **ptr, ushort type_سائز, size_t گنتی_2, ushort alignment)
{
    بلاکیں.push_back(بلاک(ptr, type_سائز, گنتی_2, alignment));
#اگر_غیر_تعریف_شدہ OPENCV_ENABLE_حافظہ_SANITIZER
    اگر (!محفوظ_3)
    {
        کل_سائز += بلاکیں.back().get_Byte_گنتی();
    }
    ورنہ
#اگر_ختم
    {
        بلاکیں.back().real_allocate();
    }
}

خالی بفر_Area::zeroFill_(خالی **ptr)
{
    برائے(std::vector<بلاک>::const_iterator i = بلاکیں.begin(); i != بلاکیں.end(); ++i)
    {
        اگر (*i == ptr)
        {
            i->zeroFill();
            توڑو;
        }
    }
}

خالی بفر_Area::zeroFill()
{
    برائے(std::vector<بلاک>::const_iterator i = بلاکیں.begin(); i != بلاکیں.end(); ++i)
    {
        i->zeroFill();
    }
}

خالی بفر_Area::commit()
{
#اگر_غیر_تعریف_شدہ OPENCV_ENABLE_حافظہ_SANITIZER
    اگر (!محفوظ_3)
    {
        CV_Assert(کل_سائز > 0);
        CV_Assert(oneBuf == تہی);
        CV_Assert(!بلاکیں.empty());
        oneBuf = fastMalloc(کل_سائز);
        خالی * ptr = oneBuf;
        برائے(std::vector<بلاک>::const_iterator i = بلاکیں.begin(); i != بلاکیں.end(); ++i)
        {
            ptr = i->fast_allocate(ptr);
        }
    }
#اگر_ختم
}

خالی بفر_Area::release()
{
    برائے(std::vector<بلاک>::const_iterator i = بلاکیں.begin(); i != بلاکیں.end(); ++i)
    {
        i->cleanup();
    }
    بلاکیں.clear();
#اگر_غیر_تعریف_شدہ OPENCV_ENABLE_حافظہ_SANITIZER
    اگر (oneBuf)
    {
        fastFree(oneBuf);
        oneBuf = 0;
    }
#اگر_ختم
}

//==================================================================================================

}} // cv::utils::
