// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ضمن "opencv2/core/utils/buffer_area.private.hpp"
#ضمن "opencv2/core/utils/configuration.private.hpp"

#که_نه_تعریف_شوی OPENCV_ENABLE_حافظه_SANITIZER
ساكن منطقي CV_بفر_AREA_OVERRIDE_خوندي_ډېرپېښ =
    cv::utils::getConfigurationParameterBool("OPENCV_BUFFER_AREA_ALWAYS_SAFE", خطأ);
#د_که_پای

نطاق cv { نطاق utils {

//==================================================================================================

صنف بفر_Area::بلاک
{
خاص:
    مضمن size_t reserve_شمېر() ثابت
    {
        أرجع alignment / type_اندازه - 1;
    }
عام:
    بلاک(فراغ **ptr_, ushort type_اندازه_2, size_t شمېر, ushort alignment_)
        : ptr(ptr_), raw_mem(0), شمېر_2(شمېر), type_اندازه(type_اندازه_2), alignment(alignment_)
    {
        CV_Assert(ptr && *ptr == عدم);
    }
    فراغ cleanup() ثابت
    {
        CV_DbgAssert(ptr);
        إذا (raw_mem)
            fastFree(raw_mem);
    }
    size_t get_Byte_شمېر() ثابت
    {
        أرجع type_اندازه * (شمېر_2 + reserve_شمېر());
    }
    فراغ real_allocate()
    {
        CV_Assert(ptr && *ptr == عدم);
        ثابت size_t allocated_شمېر = شمېر_2 + reserve_شمېر();
        raw_mem = fastMalloc(type_اندازه * allocated_شمېر);
        إذا (alignment != type_اندازه)
        {
            *ptr = alignPtr(raw_mem, alignment);
            CV_Assert(reinterpret_cast<size_t>(*ptr) % alignment == 0);
            CV_Assert(static_cast<uchar*>(*ptr) + type_اندازه * شمېر_2 <= static_cast<uchar*>(raw_mem) + type_اندازه * allocated_شمېر);
        }
        وإلا
        {
            *ptr = raw_mem;
        }
    }
#که_نه_تعریف_شوی OPENCV_ENABLE_حافظه_SANITIZER
    فراغ * fast_allocate(فراغ * buf) ثابت
    {
        CV_Assert(ptr && *ptr == عدم);
        buf = alignPtr(buf, alignment);
        CV_Assert(reinterpret_cast<size_t>(buf) % alignment == 0);
        *ptr = buf;
        أرجع static_cast<فراغ*>(static_cast<uchar*>(*ptr) + type_اندازه * شمېر_2);
    }
#د_که_پای
    منطقي چلوونکی==(فراغ **other) ثابت
    {
        CV_Assert(ptr && other);
        أرجع *ptr == *other;
    }
    فراغ zeroFill() ثابت
    {
        CV_Assert(ptr && *ptr);
        memset(static_cast<uchar*>(*ptr), 0, شمېر_2 * type_اندازه);
    }
خاص:
    فراغ **ptr;
    فراغ * raw_mem;
    size_t شمېر_2;
    ushort type_اندازه;
    ushort alignment;
};

//==================================================================================================

#که_نه_تعریف_شوی OPENCV_ENABLE_حافظه_SANITIZER
بفر_Area::بفر_Area(منطقي خوندي) :
    oneBuf(0),
    ټول_اندازه(0),
    خوندي_2(خوندي || CV_بفر_AREA_OVERRIDE_خوندي_ډېرپېښ)
{
    // nothing
}
#وإلا
بفر_Area::بفر_Area(منطقي خوندي)
{
    CV_ناکارول_شوی(خوندي);
}
#د_که_پای

بفر_Area::~بفر_Area()
{
    release();
}

فراغ بفر_Area::allocate_(فراغ **ptr, ushort type_اندازه, size_t شمېر_2, ushort alignment)
{
    بلاکونه.push_back(بلاک(ptr, type_اندازه, شمېر_2, alignment));
#که_نه_تعریف_شوی OPENCV_ENABLE_حافظه_SANITIZER
    إذا (!خوندي_2)
    {
        ټول_اندازه += بلاکونه.back().get_Byte_شمېر();
    }
    وإلا
#د_که_پای
    {
        بلاکونه.back().real_allocate();
    }
}

فراغ بفر_Area::zeroFill_(فراغ **ptr)
{
    لكل(std::vector<بلاک>::const_iterator i = بلاکونه.begin(); i != بلاکونه.end(); ++i)
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
    لكل(std::vector<بلاک>::const_iterator i = بلاکونه.begin(); i != بلاکونه.end(); ++i)
    {
        i->zeroFill();
    }
}

فراغ بفر_Area::commit()
{
#که_نه_تعریف_شوی OPENCV_ENABLE_حافظه_SANITIZER
    إذا (!خوندي_2)
    {
        CV_Assert(ټول_اندازه > 0);
        CV_Assert(oneBuf == عدم);
        CV_Assert(!بلاکونه.empty());
        oneBuf = fastMalloc(ټول_اندازه);
        فراغ * ptr = oneBuf;
        لكل(std::vector<بلاک>::const_iterator i = بلاکونه.begin(); i != بلاکونه.end(); ++i)
        {
            ptr = i->fast_allocate(ptr);
        }
    }
#د_که_پای
}

فراغ بفر_Area::release()
{
    لكل(std::vector<بلاک>::const_iterator i = بلاکونه.begin(); i != بلاکونه.end(); ++i)
    {
        i->cleanup();
    }
    بلاکونه.clear();
#که_نه_تعریف_شوی OPENCV_ENABLE_حافظه_SANITIZER
    إذا (oneBuf)
    {
        fastFree(oneBuf);
        oneBuf = 0;
    }
#د_که_پای
}

//==================================================================================================

}} // cv::utils::
