// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ಅನ್ತರ್ಭಾವ "opencv2/core/utils/buffer_area.private.hpp"
#ಅನ್ತರ್ಭಾವ "opencv2/core/utils/configuration.private.hpp"

#ifndef OPENCV_ENABLE_MEMORY_SANITIZER
ಸ್ಥಿರ ತಾರ್ಕಿಕ CV_BUFFER_AREA_OVERRIDE_SAFE_MODE =
    cv::utils::getConfigurationParameterBool("OPENCV_BUFFER_AREA_ALWAYS_SAFE", ಅಸತ್ಯ);
#endif

ನಾಮಾಕಾಶ cv { ನಾಮಾಕಾಶ utils {

//==================================================================================================

ವರ್ಗ BufferArea::Block
{
ಗುಪ್ತ:
    ಅನ್ತಃಸ್ಥ size_t reserve_ಎಣಿಕೆ() ಧ್ರುವ
    {
        ಪ್ರತ್ಯಾಗಮ alignment / type_size - 1;
    }
ಸಾರ್ವಜನಿಕ:
    Block(ಶೂನ್ಯ **ptr_, ushort type_size_, size_t ಎಣಿಕೆ, ushort alignment_)
        : ptr(ptr_), raw_mem(0), ಎಣಿಕೆ_2(ಎಣಿಕೆ), type_size(type_size_), alignment(alignment_)
    {
        CV_Assert(ptr && *ptr == ರಿಕ್ತ);
    }
    ಶೂನ್ಯ cleanup() ಧ್ರುವ
    {
        CV_DbgAssert(ptr);
        ಯದಿ (raw_mem)
            fastFree(raw_mem);
    }
    size_t get_Byte_ಎಣಿಕೆ() ಧ್ರುವ
    {
        ಪ್ರತ್ಯಾಗಮ type_size * (ಎಣಿಕೆ_2 + reserve_ಎಣಿಕೆ());
    }
    ಶೂನ್ಯ real_allocate()
    {
        CV_Assert(ptr && *ptr == ರಿಕ್ತ);
        ಧ್ರುವ size_t allocated_ಎಣಿಕೆ = ಎಣಿಕೆ_2 + reserve_ಎಣಿಕೆ();
        raw_mem = fastMalloc(type_size * allocated_ಎಣಿಕೆ);
        ಯದಿ (alignment != type_size)
        {
            *ptr = alignPtr(raw_mem, alignment);
            CV_Assert(reinterpret_cast<size_t>(*ptr) % alignment == 0);
            CV_Assert(static_cast<uchar*>(*ptr) + type_size * ಎಣಿಕೆ_2 <= static_cast<uchar*>(raw_mem) + type_size * allocated_ಎಣಿಕೆ);
        }
        ಅನ್ಯಥಾ
        {
            *ptr = raw_mem;
        }
    }
#ifndef OPENCV_ENABLE_MEMORY_SANITIZER
    ಶೂನ್ಯ * fast_allocate(ಶೂನ್ಯ * buf) ಧ್ರುವ
    {
        CV_Assert(ptr && *ptr == ರಿಕ್ತ);
        buf = alignPtr(buf, alignment);
        CV_Assert(reinterpret_cast<size_t>(buf) % alignment == 0);
        *ptr = buf;
        ಪ್ರತ್ಯಾಗಮ static_cast<ಶೂನ್ಯ*>(static_cast<uchar*>(*ptr) + type_size * ಎಣಿಕೆ_2);
    }
#endif
    ತಾರ್ಕಿಕ operator==(ಶೂನ್ಯ **other) ಧ್ರುವ
    {
        CV_Assert(ptr && other);
        ಪ್ರತ್ಯಾಗಮ *ptr == *other;
    }
    ಶೂನ್ಯ zeroFill() ಧ್ರುವ
    {
        CV_Assert(ptr && *ptr);
        memset(static_cast<uchar*>(*ptr), 0, ಎಣಿಕೆ_2 * type_size);
    }
ಗುಪ್ತ:
    ಶೂನ್ಯ **ptr;
    ಶೂನ್ಯ * raw_mem;
    size_t ಎಣಿಕೆ_2;
    ushort type_size;
    ushort alignment;
};

//==================================================================================================

#ifndef OPENCV_ENABLE_MEMORY_SANITIZER
BufferArea::BufferArea(ತಾರ್ಕಿಕ safe_) :
    oneBuf(0),
    totalSize(0),
    safe(safe_ || CV_BUFFER_AREA_OVERRIDE_SAFE_MODE)
{
    // nothing
}
#ಅನ್ಯಥಾ
BufferArea::BufferArea(ತಾರ್ಕಿಕ safe_)
{
    CV_UNUSED(safe_);
}
#endif

BufferArea::~BufferArea()
{
    release();
}

ಶೂನ್ಯ BufferArea::allocate_(ಶೂನ್ಯ **ptr, ushort type_size, size_t ಎಣಿಕೆ_2, ushort alignment)
{
    blocks.push_back(Block(ptr, type_size, ಎಣಿಕೆ_2, alignment));
#ifndef OPENCV_ENABLE_MEMORY_SANITIZER
    ಯದಿ (!safe)
    {
        totalSize += blocks.back().get_Byte_ಎಣಿಕೆ();
    }
    ಅನ್ಯಥಾ
#endif
    {
        blocks.back().real_allocate();
    }
}

ಶೂನ್ಯ BufferArea::zeroFill_(ಶೂನ್ಯ **ptr)
{
    ಪ್ರತಿ(std::vector<Block>::const_iterator i = blocks.begin(); i != blocks.end(); ++i)
    {
        ಯದಿ (*i == ptr)
        {
            i->zeroFill();
            ಭಙ್ಗ;
        }
    }
}

ಶೂನ್ಯ BufferArea::zeroFill()
{
    ಪ್ರತಿ(std::vector<Block>::const_iterator i = blocks.begin(); i != blocks.end(); ++i)
    {
        i->zeroFill();
    }
}

ಶೂನ್ಯ BufferArea::commit()
{
#ifndef OPENCV_ENABLE_MEMORY_SANITIZER
    ಯದಿ (!safe)
    {
        CV_Assert(totalSize > 0);
        CV_Assert(oneBuf == ರಿಕ್ತ);
        CV_Assert(!blocks.empty());
        oneBuf = fastMalloc(totalSize);
        ಶೂನ್ಯ * ptr = oneBuf;
        ಪ್ರತಿ(std::vector<Block>::const_iterator i = blocks.begin(); i != blocks.end(); ++i)
        {
            ptr = i->fast_allocate(ptr);
        }
    }
#endif
}

ಶೂನ್ಯ BufferArea::release()
{
    ಪ್ರತಿ(std::vector<Block>::const_iterator i = blocks.begin(); i != blocks.end(); ++i)
    {
        i->cleanup();
    }
    blocks.clear();
#ifndef OPENCV_ENABLE_MEMORY_SANITIZER
    ಯದಿ (oneBuf)
    {
        fastFree(oneBuf);
        oneBuf = 0;
    }
#endif
}

//==================================================================================================

}} // cv::utils::
