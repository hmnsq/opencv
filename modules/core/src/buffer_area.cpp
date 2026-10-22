// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ਅਨ੍ਤਰ੍ਭਾਵ "opencv2/core/utils/buffer_area.private.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "opencv2/core/utils/configuration.private.hpp"

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OPENCV_ENABLE_ਸਮ੍ਰਿਤੀ_SANITIZER
ਸਥਿਰ ਤਾਰਕਿਕ CV_ਬਫਰ_AREA_OVERRIDE_ਸੁਰੱਖਿਅਤ_ਬਹੁਲਕ =
    cv::utils::getConfigurationParameterBool("OPENCV_BUFFER_AREA_ALWAYS_SAFE", ਝੂਠ);
#ਜੇ_ਅੰਤ

ਨਾਮਾਕਾਸ਼ cv { ਨਾਮਾਕਾਸ਼ utils {

//==================================================================================================

ਵਰਗ ਬਫਰ_Area::ਖੰਡ
{
ਨਿਜੀ:
    ਸਤਰਅੰਦਰ size_t reserve_ਗਿਣਤੀ() ਅਚਲ
    {
        ਵਾਪਸ alignment / type_ਮਾਪ - 1;
    }
ਜਨਤਕ:
    ਖੰਡ(ਖਾਲੀ **ptr_, ushort type_ਮਾਪ_2, size_t ਗਿਣਤੀ_2, ushort alignment_)
        : ptr(ptr_), raw_mem(0), ਗਿਣਤੀ_3(ਗਿਣਤੀ_2), type_ਮਾਪ(type_ਮਾਪ_2), alignment(alignment_)
    {
        CV_Assert(ptr && *ptr == ਨਿਰਾ);
    }
    ਖਾਲੀ cleanup() ਅਚਲ
    {
        CV_DbgAssert(ptr);
        ਜੇ (raw_mem)
            fastFree(raw_mem);
    }
    size_t get_Byte_ਗਿਣਤੀ() ਅਚਲ
    {
        ਵਾਪਸ type_ਮਾਪ * (ਗਿਣਤੀ_3 + reserve_ਗਿਣਤੀ());
    }
    ਖਾਲੀ real_allocate()
    {
        CV_Assert(ptr && *ptr == ਨਿਰਾ);
        ਅਚਲ size_t allocated_ਗਿਣਤੀ = ਗਿਣਤੀ_3 + reserve_ਗਿਣਤੀ();
        raw_mem = fastMalloc(type_ਮਾਪ * allocated_ਗਿਣਤੀ);
        ਜੇ (alignment != type_ਮਾਪ)
        {
            *ptr = alignPtr(raw_mem, alignment);
            CV_Assert(reinterpret_cast<size_t>(*ptr) % alignment == 0);
            CV_Assert(static_cast<uchar*>(*ptr) + type_ਮਾਪ * ਗਿਣਤੀ_3 <= static_cast<uchar*>(raw_mem) + type_ਮਾਪ * allocated_ਗਿਣਤੀ);
        }
        ਨਹੀਂਤਾਂ
        {
            *ptr = raw_mem;
        }
    }
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OPENCV_ENABLE_ਸਮ੍ਰਿਤੀ_SANITIZER
    ਖਾਲੀ * fast_allocate(ਖਾਲੀ * buf) ਅਚਲ
    {
        CV_Assert(ptr && *ptr == ਨਿਰਾ);
        buf = alignPtr(buf, alignment);
        CV_Assert(reinterpret_cast<size_t>(buf) % alignment == 0);
        *ptr = buf;
        ਵਾਪਸ static_cast<ਖਾਲੀ*>(static_cast<uchar*>(*ptr) + type_ਮਾਪ * ਗਿਣਤੀ_3);
    }
#ਜੇ_ਅੰਤ
    ਤਾਰਕਿਕ ਸੰਚਾਲਕ==(ਖਾਲੀ **other) ਅਚਲ
    {
        CV_Assert(ptr && other);
        ਵਾਪਸ *ptr == *other;
    }
    ਖਾਲੀ zeroFill() ਅਚਲ
    {
        CV_Assert(ptr && *ptr);
        memset(static_cast<uchar*>(*ptr), 0, ਗਿਣਤੀ_3 * type_ਮਾਪ);
    }
ਨਿਜੀ:
    ਖਾਲੀ **ptr;
    ਖਾਲੀ * raw_mem;
    size_t ਗਿਣਤੀ_3;
    ushort type_ਮਾਪ;
    ushort alignment;
};

//==================================================================================================

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OPENCV_ENABLE_ਸਮ੍ਰਿਤੀ_SANITIZER
ਬਫਰ_Area::ਬਫਰ_Area(ਤਾਰਕਿਕ ਸੁਰੱਖਿਅਤ_2) :
    oneBuf(0),
    ਕੁੱਲ_ਮਾਪ(0),
    ਸੁਰੱਖਿਅਤ_3(ਸੁਰੱਖਿਅਤ_2 || CV_ਬਫਰ_AREA_OVERRIDE_ਸੁਰੱਖਿਅਤ_ਬਹੁਲਕ)
{
    // nothing
}
#ਨਹੀਂਤਾਂ
ਬਫਰ_Area::ਬਫਰ_Area(ਤਾਰਕਿਕ ਸੁਰੱਖਿਅਤ_2)
{
    CV_ਅਣਵਰਤਿਆ(ਸੁਰੱਖਿਅਤ_2);
}
#ਜੇ_ਅੰਤ

ਬਫਰ_Area::~ਬਫਰ_Area()
{
    release();
}

ਖਾਲੀ ਬਫਰ_Area::allocate_(ਖਾਲੀ **ptr, ushort type_ਮਾਪ, size_t ਗਿਣਤੀ_3, ushort alignment)
{
    ਖੰਡਾਂ.push_back(ਖੰਡ(ptr, type_ਮਾਪ, ਗਿਣਤੀ_3, alignment));
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OPENCV_ENABLE_ਸਮ੍ਰਿਤੀ_SANITIZER
    ਜੇ (!ਸੁਰੱਖਿਅਤ_3)
    {
        ਕੁੱਲ_ਮਾਪ += ਖੰਡਾਂ.back().get_Byte_ਗਿਣਤੀ();
    }
    ਨਹੀਂਤਾਂ
#ਜੇ_ਅੰਤ
    {
        ਖੰਡਾਂ.back().real_allocate();
    }
}

ਖਾਲੀ ਬਫਰ_Area::zeroFill_(ਖਾਲੀ **ptr)
{
    ਲਈ(std::vector<ਖੰਡ>::const_iterator i = ਖੰਡਾਂ.begin(); i != ਖੰਡਾਂ.end(); ++i)
    {
        ਜੇ (*i == ptr)
        {
            i->zeroFill();
            ਰੋਕੋ;
        }
    }
}

ਖਾਲੀ ਬਫਰ_Area::zeroFill()
{
    ਲਈ(std::vector<ਖੰਡ>::const_iterator i = ਖੰਡਾਂ.begin(); i != ਖੰਡਾਂ.end(); ++i)
    {
        i->zeroFill();
    }
}

ਖਾਲੀ ਬਫਰ_Area::commit()
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OPENCV_ENABLE_ਸਮ੍ਰਿਤੀ_SANITIZER
    ਜੇ (!ਸੁਰੱਖਿਅਤ_3)
    {
        CV_Assert(ਕੁੱਲ_ਮਾਪ > 0);
        CV_Assert(oneBuf == ਨਿਰਾ);
        CV_Assert(!ਖੰਡਾਂ.empty());
        oneBuf = fastMalloc(ਕੁੱਲ_ਮਾਪ);
        ਖਾਲੀ * ptr = oneBuf;
        ਲਈ(std::vector<ਖੰਡ>::const_iterator i = ਖੰਡਾਂ.begin(); i != ਖੰਡਾਂ.end(); ++i)
        {
            ptr = i->fast_allocate(ptr);
        }
    }
#ਜੇ_ਅੰਤ
}

ਖਾਲੀ ਬਫਰ_Area::release()
{
    ਲਈ(std::vector<ਖੰਡ>::const_iterator i = ਖੰਡਾਂ.begin(); i != ਖੰਡਾਂ.end(); ++i)
    {
        i->cleanup();
    }
    ਖੰਡਾਂ.clear();
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OPENCV_ENABLE_ਸਮ੍ਰਿਤੀ_SANITIZER
    ਜੇ (oneBuf)
    {
        fastFree(oneBuf);
        oneBuf = 0;
    }
#ਜੇ_ਅੰਤ
}

//==================================================================================================

}} // cv::utils::
