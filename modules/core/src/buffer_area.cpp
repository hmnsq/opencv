// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#شامل "opencv2/core/utils/buffer_area.private.hpp"
#شامل "opencv2/core/utils/configuration.private.hpp"

#ifndef OPENCV_ENABLE_MEMORY_SANITIZER
ایستا منطقی CV_BUFFER_AREA_OVERRIDE_SAFE_MODE =
    cv::utils::getConfigurationParameterBool("OPENCV_BUFFER_AREA_ALWAYS_SAFE", نادرست);
#endif

فضاینام cv { فضاینام utils {

//==================================================================================================

رده BufferArea::Block
{
خصوصی:
    درونخطی size_t reserve_شمار() ثابت
    {
        بازگشت alignment / type_size - 1;
    }
عمومی:
    Block(تهی **ptr_, ushort type_size_, size_t شمار, ushort alignment_)
        : ptr(ptr_), raw_mem(0), شمار_2(شمار), type_size(type_size_), alignment(alignment_)
    {
        CV_Assert(ptr && *ptr == پوچ);
    }
    تهی cleanup() ثابت
    {
        CV_DbgAssert(ptr);
        اگر (raw_mem)
            fastFree(raw_mem);
    }
    size_t get_Byte_شمار() ثابت
    {
        بازگشت type_size * (شمار_2 + reserve_شمار());
    }
    تهی real_allocate()
    {
        CV_Assert(ptr && *ptr == پوچ);
        ثابت size_t allocated_شمار = شمار_2 + reserve_شمار();
        raw_mem = fastMalloc(type_size * allocated_شمار);
        اگر (alignment != type_size)
        {
            *ptr = alignPtr(raw_mem, alignment);
            CV_Assert(reinterpret_cast<size_t>(*ptr) % alignment == 0);
            CV_Assert(static_cast<uchar*>(*ptr) + type_size * شمار_2 <= static_cast<uchar*>(raw_mem) + type_size * allocated_شمار);
        }
        وگرنه
        {
            *ptr = raw_mem;
        }
    }
#ifndef OPENCV_ENABLE_MEMORY_SANITIZER
    تهی * fast_allocate(تهی * buf) ثابت
    {
        CV_Assert(ptr && *ptr == پوچ);
        buf = alignPtr(buf, alignment);
        CV_Assert(reinterpret_cast<size_t>(buf) % alignment == 0);
        *ptr = buf;
        بازگشت static_cast<تهی*>(static_cast<uchar*>(*ptr) + type_size * شمار_2);
    }
#endif
    منطقی operator==(تهی **other) ثابت
    {
        CV_Assert(ptr && other);
        بازگشت *ptr == *other;
    }
    تهی zeroFill() ثابت
    {
        CV_Assert(ptr && *ptr);
        memset(static_cast<uchar*>(*ptr), 0, شمار_2 * type_size);
    }
خصوصی:
    تهی **ptr;
    تهی * raw_mem;
    size_t شمار_2;
    ushort type_size;
    ushort alignment;
};

//==================================================================================================

#ifndef OPENCV_ENABLE_MEMORY_SANITIZER
BufferArea::BufferArea(منطقی safe_) :
    oneBuf(0),
    totalSize(0),
    safe(safe_ || CV_BUFFER_AREA_OVERRIDE_SAFE_MODE)
{
    // nothing
}
#وگرنه
BufferArea::BufferArea(منطقی safe_)
{
    CV_UNUSED(safe_);
}
#endif

BufferArea::~BufferArea()
{
    release();
}

تهی BufferArea::allocate_(تهی **ptr, ushort type_size, size_t شمار_2, ushort alignment)
{
    blocks.push_back(Block(ptr, type_size, شمار_2, alignment));
#ifndef OPENCV_ENABLE_MEMORY_SANITIZER
    اگر (!safe)
    {
        totalSize += blocks.back().get_Byte_شمار();
    }
    وگرنه
#endif
    {
        blocks.back().real_allocate();
    }
}

تهی BufferArea::zeroFill_(تهی **ptr)
{
    برای(std::vector<Block>::const_iterator i = blocks.begin(); i != blocks.end(); ++i)
    {
        اگر (*i == ptr)
        {
            i->zeroFill();
            بشکن;
        }
    }
}

تهی BufferArea::zeroFill()
{
    برای(std::vector<Block>::const_iterator i = blocks.begin(); i != blocks.end(); ++i)
    {
        i->zeroFill();
    }
}

تهی BufferArea::commit()
{
#ifndef OPENCV_ENABLE_MEMORY_SANITIZER
    اگر (!safe)
    {
        CV_Assert(totalSize > 0);
        CV_Assert(oneBuf == پوچ);
        CV_Assert(!blocks.empty());
        oneBuf = fastMalloc(totalSize);
        تهی * ptr = oneBuf;
        برای(std::vector<Block>::const_iterator i = blocks.begin(); i != blocks.end(); ++i)
        {
            ptr = i->fast_allocate(ptr);
        }
    }
#endif
}

تهی BufferArea::release()
{
    برای(std::vector<Block>::const_iterator i = blocks.begin(); i != blocks.end(); ++i)
    {
        i->cleanup();
    }
    blocks.clear();
#ifndef OPENCV_ENABLE_MEMORY_SANITIZER
    اگر (oneBuf)
    {
        fastFree(oneBuf);
        oneBuf = 0;
    }
#endif
}

//==================================================================================================

}} // cv::utils::
