// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#অন্তর্ভাব "opencv2/core/utils/buffer_area.private.hpp"
#অন্তর্ভাব "opencv2/core/utils/configuration.private.hpp"

#ifndef OPENCV_ENABLE_MEMORY_SANITIZER
স্থির তার্কিক CV_BUFFER_AREA_OVERRIDE_SAFE_MODE =
    cv::utils::getConfigurationParameterBool("OPENCV_BUFFER_AREA_ALWAYS_SAFE", অসত্য);
#endif

নামাকাশ cv { নামাকাশ utils {

//==================================================================================================

বর্গ BufferArea::Block
{
গুপ্ত:
    অন্তঃস্থ size_t reserve_গুনতি() ধ্রুব
    {
        প্রত্যাগম alignment / type_size - 1;
    }
সার্বজনিক:
    Block(শূন্য **ptr_, ushort type_size_, size_t গুনতি, ushort alignment_)
        : ptr(ptr_), raw_mem(0), গুনতি_2(গুনতি), type_size(type_size_), alignment(alignment_)
    {
        CV_Assert(ptr && *ptr == রিক্ত);
    }
    শূন্য cleanup() ধ্রুব
    {
        CV_DbgAssert(ptr);
        যদি (raw_mem)
            fastFree(raw_mem);
    }
    size_t get_Byte_গুনতি() ধ্রুব
    {
        প্রত্যাগম type_size * (গুনতি_2 + reserve_গুনতি());
    }
    শূন্য real_allocate()
    {
        CV_Assert(ptr && *ptr == রিক্ত);
        ধ্রুব size_t allocated_গুনতি = গুনতি_2 + reserve_গুনতি();
        raw_mem = fastMalloc(type_size * allocated_গুনতি);
        যদি (alignment != type_size)
        {
            *ptr = alignPtr(raw_mem, alignment);
            CV_Assert(reinterpret_cast<size_t>(*ptr) % alignment == 0);
            CV_Assert(static_cast<uchar*>(*ptr) + type_size * গুনতি_2 <= static_cast<uchar*>(raw_mem) + type_size * allocated_গুনতি);
        }
        অন্যথা
        {
            *ptr = raw_mem;
        }
    }
#ifndef OPENCV_ENABLE_MEMORY_SANITIZER
    শূন্য * fast_allocate(শূন্য * buf) ধ্রুব
    {
        CV_Assert(ptr && *ptr == রিক্ত);
        buf = alignPtr(buf, alignment);
        CV_Assert(reinterpret_cast<size_t>(buf) % alignment == 0);
        *ptr = buf;
        প্রত্যাগম static_cast<শূন্য*>(static_cast<uchar*>(*ptr) + type_size * গুনতি_2);
    }
#endif
    তার্কিক operator==(শূন্য **other) ধ্রুব
    {
        CV_Assert(ptr && other);
        প্রত্যাগম *ptr == *other;
    }
    শূন্য zeroFill() ধ্রুব
    {
        CV_Assert(ptr && *ptr);
        memset(static_cast<uchar*>(*ptr), 0, গুনতি_2 * type_size);
    }
গুপ্ত:
    শূন্য **ptr;
    শূন্য * raw_mem;
    size_t গুনতি_2;
    ushort type_size;
    ushort alignment;
};

//==================================================================================================

#ifndef OPENCV_ENABLE_MEMORY_SANITIZER
BufferArea::BufferArea(তার্কিক safe_) :
    oneBuf(0),
    totalSize(0),
    safe(safe_ || CV_BUFFER_AREA_OVERRIDE_SAFE_MODE)
{
    // nothing
}
#অন্যথা
BufferArea::BufferArea(তার্কিক safe_)
{
    CV_UNUSED(safe_);
}
#endif

BufferArea::~BufferArea()
{
    release();
}

শূন্য BufferArea::allocate_(শূন্য **ptr, ushort type_size, size_t গুনতি_2, ushort alignment)
{
    blocks.push_back(Block(ptr, type_size, গুনতি_2, alignment));
#ifndef OPENCV_ENABLE_MEMORY_SANITIZER
    যদি (!safe)
    {
        totalSize += blocks.back().get_Byte_গুনতি();
    }
    অন্যথা
#endif
    {
        blocks.back().real_allocate();
    }
}

শূন্য BufferArea::zeroFill_(শূন্য **ptr)
{
    প্রতি(std::vector<Block>::const_iterator i = blocks.begin(); i != blocks.end(); ++i)
    {
        যদি (*i == ptr)
        {
            i->zeroFill();
            ভঙ্গ;
        }
    }
}

শূন্য BufferArea::zeroFill()
{
    প্রতি(std::vector<Block>::const_iterator i = blocks.begin(); i != blocks.end(); ++i)
    {
        i->zeroFill();
    }
}

শূন্য BufferArea::commit()
{
#ifndef OPENCV_ENABLE_MEMORY_SANITIZER
    যদি (!safe)
    {
        CV_Assert(totalSize > 0);
        CV_Assert(oneBuf == রিক্ত);
        CV_Assert(!blocks.empty());
        oneBuf = fastMalloc(totalSize);
        শূন্য * ptr = oneBuf;
        প্রতি(std::vector<Block>::const_iterator i = blocks.begin(); i != blocks.end(); ++i)
        {
            ptr = i->fast_allocate(ptr);
        }
    }
#endif
}

শূন্য BufferArea::release()
{
    প্রতি(std::vector<Block>::const_iterator i = blocks.begin(); i != blocks.end(); ++i)
    {
        i->cleanup();
    }
    blocks.clear();
#ifndef OPENCV_ENABLE_MEMORY_SANITIZER
    যদি (oneBuf)
    {
        fastFree(oneBuf);
        oneBuf = 0;
    }
#endif
}

//==================================================================================================

}} // cv::utils::
