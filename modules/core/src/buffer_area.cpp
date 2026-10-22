// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ضمن "opencv2/core/utils/buffer_area.private.hpp"
#ضمن "opencv2/core/utils/configuration.private.hpp"

#ifndef OPENCV_ENABLE_MEMORY_SANITIZER
ساكن منطقي CV_BUFFER_AREA_OVERRIDE_SAFE_MODE =
    cv::utils::getConfigurationParameterBool("OPENCV_BUFFER_AREA_ALWAYS_SAFE", خطأ);
#endif

نطاق cv { نطاق utils {

//==================================================================================================

صنف BufferArea::Block
{
خاص:
    مضمن size_t reserve_عدد() ثابت
    {
        أرجع alignment / type_size - 1;
    }
عام:
    Block(فراغ **ptr_, ushort type_size_, size_t عدد, ushort alignment_)
        : ptr(ptr_), raw_mem(0), عدد_2(عدد), type_size(type_size_), alignment(alignment_)
    {
        CV_Assert(ptr && *ptr == عدم);
    }
    فراغ cleanup() ثابت
    {
        CV_DbgAssert(ptr);
        إذا (raw_mem)
            fastFree(raw_mem);
    }
    size_t get_Byte_عدد() ثابت
    {
        أرجع type_size * (عدد_2 + reserve_عدد());
    }
    فراغ real_allocate()
    {
        CV_Assert(ptr && *ptr == عدم);
        ثابت size_t allocated_عدد = عدد_2 + reserve_عدد();
        raw_mem = fastMalloc(type_size * allocated_عدد);
        إذا (alignment != type_size)
        {
            *ptr = alignPtr(raw_mem, alignment);
            CV_Assert(reinterpret_cast<size_t>(*ptr) % alignment == 0);
            CV_Assert(static_cast<uchar*>(*ptr) + type_size * عدد_2 <= static_cast<uchar*>(raw_mem) + type_size * allocated_عدد);
        }
        وإلا
        {
            *ptr = raw_mem;
        }
    }
#ifndef OPENCV_ENABLE_MEMORY_SANITIZER
    فراغ * fast_allocate(فراغ * buf) ثابت
    {
        CV_Assert(ptr && *ptr == عدم);
        buf = alignPtr(buf, alignment);
        CV_Assert(reinterpret_cast<size_t>(buf) % alignment == 0);
        *ptr = buf;
        أرجع static_cast<فراغ*>(static_cast<uchar*>(*ptr) + type_size * عدد_2);
    }
#endif
    منطقي operator==(فراغ **other) ثابت
    {
        CV_Assert(ptr && other);
        أرجع *ptr == *other;
    }
    فراغ zeroFill() ثابت
    {
        CV_Assert(ptr && *ptr);
        memset(static_cast<uchar*>(*ptr), 0, عدد_2 * type_size);
    }
خاص:
    فراغ **ptr;
    فراغ * raw_mem;
    size_t عدد_2;
    ushort type_size;
    ushort alignment;
};

//==================================================================================================

#ifndef OPENCV_ENABLE_MEMORY_SANITIZER
BufferArea::BufferArea(منطقي safe_) :
    oneBuf(0),
    totalSize(0),
    safe(safe_ || CV_BUFFER_AREA_OVERRIDE_SAFE_MODE)
{
    // nothing
}
#وإلا
BufferArea::BufferArea(منطقي safe_)
{
    CV_UNUSED(safe_);
}
#endif

BufferArea::~BufferArea()
{
    release();
}

فراغ BufferArea::allocate_(فراغ **ptr, ushort type_size, size_t عدد_2, ushort alignment)
{
    blocks.push_back(Block(ptr, type_size, عدد_2, alignment));
#ifndef OPENCV_ENABLE_MEMORY_SANITIZER
    إذا (!safe)
    {
        totalSize += blocks.back().get_Byte_عدد();
    }
    وإلا
#endif
    {
        blocks.back().real_allocate();
    }
}

فراغ BufferArea::zeroFill_(فراغ **ptr)
{
    لكل(std::vector<Block>::const_iterator i = blocks.begin(); i != blocks.end(); ++i)
    {
        إذا (*i == ptr)
        {
            i->zeroFill();
            اكسر;
        }
    }
}

فراغ BufferArea::zeroFill()
{
    لكل(std::vector<Block>::const_iterator i = blocks.begin(); i != blocks.end(); ++i)
    {
        i->zeroFill();
    }
}

فراغ BufferArea::commit()
{
#ifndef OPENCV_ENABLE_MEMORY_SANITIZER
    إذا (!safe)
    {
        CV_Assert(totalSize > 0);
        CV_Assert(oneBuf == عدم);
        CV_Assert(!blocks.empty());
        oneBuf = fastMalloc(totalSize);
        فراغ * ptr = oneBuf;
        لكل(std::vector<Block>::const_iterator i = blocks.begin(); i != blocks.end(); ++i)
        {
            ptr = i->fast_allocate(ptr);
        }
    }
#endif
}

فراغ BufferArea::release()
{
    لكل(std::vector<Block>::const_iterator i = blocks.begin(); i != blocks.end(); ++i)
    {
        i->cleanup();
    }
    blocks.clear();
#ifndef OPENCV_ENABLE_MEMORY_SANITIZER
    إذا (oneBuf)
    {
        fastFree(oneBuf);
        oneBuf = 0;
    }
#endif
}

//==================================================================================================

}} // cv::utils::
