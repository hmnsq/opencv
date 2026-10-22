// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#அந்தர்பாவ "opencv2/core/utils/buffer_area.private.hpp"
#அந்தர்பாவ "opencv2/core/utils/configuration.private.hpp"

#ifndef OPENCV_ENABLE_MEMORY_SANITIZER
நிலையான தருக்க CV_BUFFER_AREA_OVERRIDE_SAFE_MODE =
    cv::utils::getConfigurationParameterBool("OPENCV_BUFFER_AREA_ALWAYS_SAFE", பொய்);
#endif

நாமாகாஶ cv { நாமாகாஶ utils {

//==================================================================================================

வகுப்பு BufferArea::Block
{
தனிப்பட்ட:
    உள்வரி size_t reserve_மொத்தம்() மாறிலி
    {
        திரும்பு alignment / type_size - 1;
    }
பொது:
    Block(வெறுமை **ptr_, ushort type_size_, size_t மொத்தம், ushort alignment_)
        : ptr(ptr_), raw_mem(0), மொத்தம்_2(மொத்தம்), type_size(type_size_), alignment(alignment_)
    {
        CV_Assert(ptr && *ptr == வெற்று);
    }
    வெறுமை cleanup() மாறிலி
    {
        CV_DbgAssert(ptr);
        எனில் (raw_mem)
            fastFree(raw_mem);
    }
    size_t get_Byte_மொத்தம்() மாறிலி
    {
        திரும்பு type_size * (மொத்தம்_2 + reserve_மொத்தம்());
    }
    வெறுமை real_allocate()
    {
        CV_Assert(ptr && *ptr == வெற்று);
        மாறிலி size_t allocated_மொத்தம் = மொத்தம்_2 + reserve_மொத்தம்();
        raw_mem = fastMalloc(type_size * allocated_மொத்தம்);
        எனில் (alignment != type_size)
        {
            *ptr = alignPtr(raw_mem, alignment);
            CV_Assert(reinterpret_cast<size_t>(*ptr) % alignment == 0);
            CV_Assert(static_cast<uchar*>(*ptr) + type_size * மொத்தம்_2 <= static_cast<uchar*>(raw_mem) + type_size * allocated_மொத்தம்);
        }
        இல்லையெனில்
        {
            *ptr = raw_mem;
        }
    }
#ifndef OPENCV_ENABLE_MEMORY_SANITIZER
    வெறுமை * fast_allocate(வெறுமை * buf) மாறிலி
    {
        CV_Assert(ptr && *ptr == வெற்று);
        buf = alignPtr(buf, alignment);
        CV_Assert(reinterpret_cast<size_t>(buf) % alignment == 0);
        *ptr = buf;
        திரும்பு static_cast<வெறுமை*>(static_cast<uchar*>(*ptr) + type_size * மொத்தம்_2);
    }
#endif
    தருக்க operator==(வெறுமை **other) மாறிலி
    {
        CV_Assert(ptr && other);
        திரும்பு *ptr == *other;
    }
    வெறுமை zeroFill() மாறிலி
    {
        CV_Assert(ptr && *ptr);
        memset(static_cast<uchar*>(*ptr), 0, மொத்தம்_2 * type_size);
    }
தனிப்பட்ட:
    வெறுமை **ptr;
    வெறுமை * raw_mem;
    size_t மொத்தம்_2;
    ushort type_size;
    ushort alignment;
};

//==================================================================================================

#ifndef OPENCV_ENABLE_MEMORY_SANITIZER
BufferArea::BufferArea(தருக்க safe_) :
    oneBuf(0),
    totalSize(0),
    safe(safe_ || CV_BUFFER_AREA_OVERRIDE_SAFE_MODE)
{
    // nothing
}
#இல்லையெனில்
BufferArea::BufferArea(தருக்க safe_)
{
    CV_UNUSED(safe_);
}
#endif

BufferArea::~BufferArea()
{
    release();
}

வெறுமை BufferArea::allocate_(வெறுமை **ptr, ushort type_size, size_t மொத்தம்_2, ushort alignment)
{
    blocks.push_back(Block(ptr, type_size, மொத்தம்_2, alignment));
#ifndef OPENCV_ENABLE_MEMORY_SANITIZER
    எனில் (!safe)
    {
        totalSize += blocks.back().get_Byte_மொத்தம்();
    }
    இல்லையெனில்
#endif
    {
        blocks.back().real_allocate();
    }
}

வெறுமை BufferArea::zeroFill_(வெறுமை **ptr)
{
    க்கு(std::vector<Block>::const_iterator i = blocks.begin(); i != blocks.end(); ++i)
    {
        எனில் (*i == ptr)
        {
            i->zeroFill();
            நிறுத்து;
        }
    }
}

வெறுமை BufferArea::zeroFill()
{
    க்கு(std::vector<Block>::const_iterator i = blocks.begin(); i != blocks.end(); ++i)
    {
        i->zeroFill();
    }
}

வெறுமை BufferArea::commit()
{
#ifndef OPENCV_ENABLE_MEMORY_SANITIZER
    எனில் (!safe)
    {
        CV_Assert(totalSize > 0);
        CV_Assert(oneBuf == வெற்று);
        CV_Assert(!blocks.empty());
        oneBuf = fastMalloc(totalSize);
        வெறுமை * ptr = oneBuf;
        க்கு(std::vector<Block>::const_iterator i = blocks.begin(); i != blocks.end(); ++i)
        {
            ptr = i->fast_allocate(ptr);
        }
    }
#endif
}

வெறுமை BufferArea::release()
{
    க்கு(std::vector<Block>::const_iterator i = blocks.begin(); i != blocks.end(); ++i)
    {
        i->cleanup();
    }
    blocks.clear();
#ifndef OPENCV_ENABLE_MEMORY_SANITIZER
    எனில் (oneBuf)
    {
        fastFree(oneBuf);
        oneBuf = 0;
    }
#endif
}

//==================================================================================================

}} // cv::utils::
