// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#অন্তর্ভাব "opencv2/core/utils/buffer_area.private.hpp"
#অন্তর্ভাব "opencv2/core/utils/configuration.private.hpp"

#ifndef OPENCV_ENABLE_MEMORY_SANITIZER
স্থির যৌক্তিক CV_বাফার_AREA_OVERRIDE_SAFE_MODE =
    cv::utils::getConfigurationParameterBool("OPENCV_BUFFER_AREA_ALWAYS_SAFE", মিথ্যা);
#endif

নামাকাশ cv { নামাকাশ utils {

//==================================================================================================

শ্রেণী বাফার_Area::Block
{
ব্যক্তিগত:
    অন্তঃরেখ size_t reserve_গুনতি() ধ্রুবক
    {
        ফেরত alignment / type_আকার - 1;
    }
সর্বজনীন:
    Block(শূন্য **ptr_, ushort type_আকার_2, size_t গুনতি, ushort alignment_)
        : ptr(ptr_), raw_mem(0), গুনতি_2(গুনতি), type_আকার(type_আকার_2), alignment(alignment_)
    {
        CV_Assert(ptr && *ptr == ফাঁকা);
    }
    শূন্য cleanup() ধ্রুবক
    {
        CV_DbgAssert(ptr);
        যদি (raw_mem)
            fastFree(raw_mem);
    }
    size_t get_Byte_গুনতি() ধ্রুবক
    {
        ফেরত type_আকার * (গুনতি_2 + reserve_গুনতি());
    }
    শূন্য real_allocate()
    {
        CV_Assert(ptr && *ptr == ফাঁকা);
        ধ্রুবক size_t allocated_গুনতি = গুনতি_2 + reserve_গুনতি();
        raw_mem = fastMalloc(type_আকার * allocated_গুনতি);
        যদি (alignment != type_আকার)
        {
            *ptr = alignPtr(raw_mem, alignment);
            CV_Assert(reinterpret_cast<size_t>(*ptr) % alignment == 0);
            CV_Assert(static_cast<uchar*>(*ptr) + type_আকার * গুনতি_2 <= static_cast<uchar*>(raw_mem) + type_আকার * allocated_গুনতি);
        }
        অন্যথা
        {
            *ptr = raw_mem;
        }
    }
#ifndef OPENCV_ENABLE_MEMORY_SANITIZER
    শূন্য * fast_allocate(শূন্য * buf) ধ্রুবক
    {
        CV_Assert(ptr && *ptr == ফাঁকা);
        buf = alignPtr(buf, alignment);
        CV_Assert(reinterpret_cast<size_t>(buf) % alignment == 0);
        *ptr = buf;
        ফেরত static_cast<শূন্য*>(static_cast<uchar*>(*ptr) + type_আকার * গুনতি_2);
    }
#endif
    যৌক্তিক operator==(শূন্য **other) ধ্রুবক
    {
        CV_Assert(ptr && other);
        ফেরত *ptr == *other;
    }
    শূন্য zeroFill() ধ্রুবক
    {
        CV_Assert(ptr && *ptr);
        memset(static_cast<uchar*>(*ptr), 0, গুনতি_2 * type_আকার);
    }
ব্যক্তিগত:
    শূন্য **ptr;
    শূন্য * raw_mem;
    size_t গুনতি_2;
    ushort type_আকার;
    ushort alignment;
};

//==================================================================================================

#ifndef OPENCV_ENABLE_MEMORY_SANITIZER
বাফার_Area::বাফার_Area(যৌক্তিক safe_) :
    oneBuf(0),
    যোগ_আকার(0),
    safe(safe_ || CV_বাফার_AREA_OVERRIDE_SAFE_MODE)
{
    // nothing
}
#অন্যথা
বাফার_Area::বাফার_Area(যৌক্তিক safe_)
{
    CV_UNUSED(safe_);
}
#endif

বাফার_Area::~বাফার_Area()
{
    release();
}

শূন্য বাফার_Area::allocate_(শূন্য **ptr, ushort type_আকার, size_t গুনতি_2, ushort alignment)
{
    blocks.push_back(Block(ptr, type_আকার, গুনতি_2, alignment));
#ifndef OPENCV_ENABLE_MEMORY_SANITIZER
    যদি (!safe)
    {
        যোগ_আকার += blocks.back().get_Byte_গুনতি();
    }
    অন্যথা
#endif
    {
        blocks.back().real_allocate();
    }
}

শূন্য বাফার_Area::zeroFill_(শূন্য **ptr)
{
    জন্য(std::vector<Block>::const_iterator i = blocks.begin(); i != blocks.end(); ++i)
    {
        যদি (*i == ptr)
        {
            i->zeroFill();
            বিরতি;
        }
    }
}

শূন্য বাফার_Area::zeroFill()
{
    জন্য(std::vector<Block>::const_iterator i = blocks.begin(); i != blocks.end(); ++i)
    {
        i->zeroFill();
    }
}

শূন্য বাফার_Area::commit()
{
#ifndef OPENCV_ENABLE_MEMORY_SANITIZER
    যদি (!safe)
    {
        CV_Assert(যোগ_আকার > 0);
        CV_Assert(oneBuf == ফাঁকা);
        CV_Assert(!blocks.empty());
        oneBuf = fastMalloc(যোগ_আকার);
        শূন্য * ptr = oneBuf;
        জন্য(std::vector<Block>::const_iterator i = blocks.begin(); i != blocks.end(); ++i)
        {
            ptr = i->fast_allocate(ptr);
        }
    }
#endif
}

শূন্য বাফার_Area::release()
{
    জন্য(std::vector<Block>::const_iterator i = blocks.begin(); i != blocks.end(); ++i)
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
