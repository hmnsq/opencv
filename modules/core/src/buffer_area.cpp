// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ಅನ್ತರ್ಭಾವ "opencv2/core/utils/buffer_area.private.hpp"
#ಅನ್ತರ್ಭಾವ "opencv2/core/utils/configuration.private.hpp"

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ OPENCV_ENABLE_ಸ್ಮರಣೆ_SANITIZER
ಸ್ಥಿರ ತಾರ್ಕಿಕ CV_ಬಫರ್_AREA_OVERRIDE_ಸುರಕ್ಷಿತ_ಬಹುಲಕ =
    cv::utils::getConfigurationParameterBool("OPENCV_BUFFER_AREA_ALWAYS_SAFE", ಸುಳ್ಳು);
#ಆದರೆ_ಅಂತ್ಯ

ನಾಮಾಕಾಶ cv { ನಾಮಾಕಾಶ utils {

//==================================================================================================

ವರ್ಗ ಬಫರ್_Area::ಖಂಡ
{
ಖಾಸಗಿ:
    ಸಾಲೊಳಗೆ size_t reserve_ಎಣಿಕೆ() ಸ್ಥಿರಾಂಕ
    {
        ಹಿಂತಿರುಗಿ alignment / type_ಗಾತ್ರ - 1;
    }
ಸಾರ್ವಜನಿಕ:
    ಖಂಡ(ಶೂನ್ಯ **ptr_, ushort type_ಗಾತ್ರ_2, size_t ಎಣಿಕೆ_2, ushort alignment_)
        : ptr(ptr_), raw_mem(0), ಎಣಿಕೆ_3(ಎಣಿಕೆ_2), type_ಗಾತ್ರ(type_ಗಾತ್ರ_2), alignment(alignment_)
    {
        CV_Assert(ptr && *ptr == ಖಾಲಿ);
    }
    ಶೂನ್ಯ cleanup() ಸ್ಥಿರಾಂಕ
    {
        CV_DbgAssert(ptr);
        ಆದರೆ (raw_mem)
            fastFree(raw_mem);
    }
    size_t get_Byte_ಎಣಿಕೆ() ಸ್ಥಿರಾಂಕ
    {
        ಹಿಂತಿರುಗಿ type_ಗಾತ್ರ * (ಎಣಿಕೆ_3 + reserve_ಎಣಿಕೆ());
    }
    ಶೂನ್ಯ real_allocate()
    {
        CV_Assert(ptr && *ptr == ಖಾಲಿ);
        ಸ್ಥಿರಾಂಕ size_t allocated_ಎಣಿಕೆ = ಎಣಿಕೆ_3 + reserve_ಎಣಿಕೆ();
        raw_mem = fastMalloc(type_ಗಾತ್ರ * allocated_ಎಣಿಕೆ);
        ಆದರೆ (alignment != type_ಗಾತ್ರ)
        {
            *ptr = alignPtr(raw_mem, alignment);
            CV_Assert(reinterpret_cast<size_t>(*ptr) % alignment == 0);
            CV_Assert(static_cast<uchar*>(*ptr) + type_ಗಾತ್ರ * ಎಣಿಕೆ_3 <= static_cast<uchar*>(raw_mem) + type_ಗಾತ್ರ * allocated_ಎಣಿಕೆ);
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            *ptr = raw_mem;
        }
    }
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ OPENCV_ENABLE_ಸ್ಮರಣೆ_SANITIZER
    ಶೂನ್ಯ * fast_allocate(ಶೂನ್ಯ * buf) ಸ್ಥಿರಾಂಕ
    {
        CV_Assert(ptr && *ptr == ಖಾಲಿ);
        buf = alignPtr(buf, alignment);
        CV_Assert(reinterpret_cast<size_t>(buf) % alignment == 0);
        *ptr = buf;
        ಹಿಂತಿರುಗಿ static_cast<ಶೂನ್ಯ*>(static_cast<uchar*>(*ptr) + type_ಗಾತ್ರ * ಎಣಿಕೆ_3);
    }
#ಆದರೆ_ಅಂತ್ಯ
    ತಾರ್ಕಿಕ ಕ್ರಿಯಾಕಾರಕ==(ಶೂನ್ಯ **other) ಸ್ಥಿರಾಂಕ
    {
        CV_Assert(ptr && other);
        ಹಿಂತಿರುಗಿ *ptr == *other;
    }
    ಶೂನ್ಯ zeroFill() ಸ್ಥಿರಾಂಕ
    {
        CV_Assert(ptr && *ptr);
        memset(static_cast<uchar*>(*ptr), 0, ಎಣಿಕೆ_3 * type_ಗಾತ್ರ);
    }
ಖಾಸಗಿ:
    ಶೂನ್ಯ **ptr;
    ಶೂನ್ಯ * raw_mem;
    size_t ಎಣಿಕೆ_3;
    ushort type_ಗಾತ್ರ;
    ushort alignment;
};

//==================================================================================================

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ OPENCV_ENABLE_ಸ್ಮರಣೆ_SANITIZER
ಬಫರ್_Area::ಬಫರ್_Area(ತಾರ್ಕಿಕ ಸುರಕ್ಷಿತ) :
    oneBuf(0),
    ಒಟ್ಟು_ಗಾತ್ರ(0),
    ಸುರಕ್ಷಿತ_2(ಸುರಕ್ಷಿತ || CV_ಬಫರ್_AREA_OVERRIDE_ಸುರಕ್ಷಿತ_ಬಹುಲಕ)
{
    // nothing
}
#ಇಲ್ಲದಿದ್ದರೆ
ಬಫರ್_Area::ಬಫರ್_Area(ತಾರ್ಕಿಕ ಸುರಕ್ಷಿತ)
{
    CV_ಬಳಸದ(ಸುರಕ್ಷಿತ);
}
#ಆದರೆ_ಅಂತ್ಯ

ಬಫರ್_Area::~ಬಫರ್_Area()
{
    release();
}

ಶೂನ್ಯ ಬಫರ್_Area::allocate_(ಶೂನ್ಯ **ptr, ushort type_ಗಾತ್ರ, size_t ಎಣಿಕೆ_3, ushort alignment)
{
    ಖಂಡಗಳು.push_back(ಖಂಡ(ptr, type_ಗಾತ್ರ, ಎಣಿಕೆ_3, alignment));
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ OPENCV_ENABLE_ಸ್ಮರಣೆ_SANITIZER
    ಆದರೆ (!ಸುರಕ್ಷಿತ_2)
    {
        ಒಟ್ಟು_ಗಾತ್ರ += ಖಂಡಗಳು.back().get_Byte_ಎಣಿಕೆ();
    }
    ಇಲ್ಲದಿದ್ದರೆ
#ಆದರೆ_ಅಂತ್ಯ
    {
        ಖಂಡಗಳು.back().real_allocate();
    }
}

ಶೂನ್ಯ ಬಫರ್_Area::zeroFill_(ಶೂನ್ಯ **ptr)
{
    ಗಾಗಿ(std::vector<ಖಂಡ>::const_iterator i = ಖಂಡಗಳು.begin(); i != ಖಂಡಗಳು.end(); ++i)
    {
        ಆದರೆ (*i == ptr)
        {
            i->zeroFill();
            ನಿಲ್ಲಿಸು;
        }
    }
}

ಶೂನ್ಯ ಬಫರ್_Area::zeroFill()
{
    ಗಾಗಿ(std::vector<ಖಂಡ>::const_iterator i = ಖಂಡಗಳು.begin(); i != ಖಂಡಗಳು.end(); ++i)
    {
        i->zeroFill();
    }
}

ಶೂನ್ಯ ಬಫರ್_Area::commit()
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ OPENCV_ENABLE_ಸ್ಮರಣೆ_SANITIZER
    ಆದರೆ (!ಸುರಕ್ಷಿತ_2)
    {
        CV_Assert(ಒಟ್ಟು_ಗಾತ್ರ > 0);
        CV_Assert(oneBuf == ಖಾಲಿ);
        CV_Assert(!ಖಂಡಗಳು.empty());
        oneBuf = fastMalloc(ಒಟ್ಟು_ಗಾತ್ರ);
        ಶೂನ್ಯ * ptr = oneBuf;
        ಗಾಗಿ(std::vector<ಖಂಡ>::const_iterator i = ಖಂಡಗಳು.begin(); i != ಖಂಡಗಳು.end(); ++i)
        {
            ptr = i->fast_allocate(ptr);
        }
    }
#ಆದರೆ_ಅಂತ್ಯ
}

ಶೂನ್ಯ ಬಫರ್_Area::release()
{
    ಗಾಗಿ(std::vector<ಖಂಡ>::const_iterator i = ಖಂಡಗಳು.begin(); i != ಖಂಡಗಳು.end(); ++i)
    {
        i->cleanup();
    }
    ಖಂಡಗಳು.clear();
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ OPENCV_ENABLE_ಸ್ಮರಣೆ_SANITIZER
    ಆದರೆ (oneBuf)
    {
        fastFree(oneBuf);
        oneBuf = 0;
    }
#ಆದರೆ_ಅಂತ್ಯ
}

//==================================================================================================

}} // cv::utils::
