// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ଅନ୍ତର୍ଭାଵ "opencv2/core/utils/buffer_area.private.hpp"
#ଅନ୍ତର୍ଭାଵ "opencv2/core/utils/configuration.private.hpp"

#ifndef OPENCV_ENABLE_MEMORY_SANITIZER
ସ୍ଥିର ତାର୍କିକ CV_BUFFER_AREA_OVERRIDE_SAFE_MODE =
    cv::utils::getConfigurationParameterBool("OPENCV_BUFFER_AREA_ALWAYS_SAFE", ମିଥ୍ୟା);
#endif

ନାମାକାଶ cv { ନାମାକାଶ utils {

//==================================================================================================

ଶ୍ରେଣୀ BufferArea::Block
{
ବ୍ୟକ୍ତିଗତ:
    ଧାଡ଼ିଭିତରେ size_t reserve_ଗଣନା() ଅଚଳ
    {
        ଫେରନ୍ତୁ alignment / type_size - 1;
    }
ସର୍ବସାଧାରଣ:
    Block(ଶୂନ୍ୟ **ptr_, ushort type_size_, size_t ଗଣନା_2, ushort alignment_)
        : ptr(ptr_), raw_mem(0), ଗଣନା_3(ଗଣନା_2), type_size(type_size_), alignment(alignment_)
    {
        CV_Assert(ptr && *ptr == ଖାଲି);
    }
    ଶୂନ୍ୟ cleanup() ଅଚଳ
    {
        CV_DbgAssert(ptr);
        ଯଦି (raw_mem)
            fastFree(raw_mem);
    }
    size_t get_Byte_ଗଣନା() ଅଚଳ
    {
        ଫେରନ୍ତୁ type_size * (ଗଣନା_3 + reserve_ଗଣନା());
    }
    ଶୂନ୍ୟ real_allocate()
    {
        CV_Assert(ptr && *ptr == ଖାଲି);
        ଅଚଳ size_t allocated_ଗଣନା = ଗଣନା_3 + reserve_ଗଣନା();
        raw_mem = fastMalloc(type_size * allocated_ଗଣନା);
        ଯଦି (alignment != type_size)
        {
            *ptr = alignPtr(raw_mem, alignment);
            CV_Assert(reinterpret_cast<size_t>(*ptr) % alignment == 0);
            CV_Assert(static_cast<uchar*>(*ptr) + type_size * ଗଣନା_3 <= static_cast<uchar*>(raw_mem) + type_size * allocated_ଗଣନା);
        }
        ଅନ୍ୟଥା
        {
            *ptr = raw_mem;
        }
    }
#ifndef OPENCV_ENABLE_MEMORY_SANITIZER
    ଶୂନ୍ୟ * fast_allocate(ଶୂନ୍ୟ * buf) ଅଚଳ
    {
        CV_Assert(ptr && *ptr == ଖାଲି);
        buf = alignPtr(buf, alignment);
        CV_Assert(reinterpret_cast<size_t>(buf) % alignment == 0);
        *ptr = buf;
        ଫେରନ୍ତୁ static_cast<ଶୂନ୍ୟ*>(static_cast<uchar*>(*ptr) + type_size * ଗଣନା_3);
    }
#endif
    ତାର୍କିକ operator==(ଶୂନ୍ୟ **other) ଅଚଳ
    {
        CV_Assert(ptr && other);
        ଫେରନ୍ତୁ *ptr == *other;
    }
    ଶୂନ୍ୟ zeroFill() ଅଚଳ
    {
        CV_Assert(ptr && *ptr);
        memset(static_cast<uchar*>(*ptr), 0, ଗଣନା_3 * type_size);
    }
ବ୍ୟକ୍ତିଗତ:
    ଶୂନ୍ୟ **ptr;
    ଶୂନ୍ୟ * raw_mem;
    size_t ଗଣନା_3;
    ushort type_size;
    ushort alignment;
};

//==================================================================================================

#ifndef OPENCV_ENABLE_MEMORY_SANITIZER
BufferArea::BufferArea(ତାର୍କିକ safe_) :
    oneBuf(0),
    totalSize(0),
    safe(safe_ || CV_BUFFER_AREA_OVERRIDE_SAFE_MODE)
{
    // nothing
}
#ଅନ୍ୟଥା
BufferArea::BufferArea(ତାର୍କିକ safe_)
{
    CV_UNUSED(safe_);
}
#endif

BufferArea::~BufferArea()
{
    release();
}

ଶୂନ୍ୟ BufferArea::allocate_(ଶୂନ୍ୟ **ptr, ushort type_size, size_t ଗଣନା_3, ushort alignment)
{
    blocks.push_back(Block(ptr, type_size, ଗଣନା_3, alignment));
#ifndef OPENCV_ENABLE_MEMORY_SANITIZER
    ଯଦି (!safe)
    {
        totalSize += blocks.back().get_Byte_ଗଣନା();
    }
    ଅନ୍ୟଥା
#endif
    {
        blocks.back().real_allocate();
    }
}

ଶୂନ୍ୟ BufferArea::zeroFill_(ଶୂନ୍ୟ **ptr)
{
    ପାଇଁ(std::vector<Block>::const_iterator i = blocks.begin(); i != blocks.end(); ++i)
    {
        ଯଦି (*i == ptr)
        {
            i->zeroFill();
            ଭାଙ୍ଗ;
        }
    }
}

ଶୂନ୍ୟ BufferArea::zeroFill()
{
    ପାଇଁ(std::vector<Block>::const_iterator i = blocks.begin(); i != blocks.end(); ++i)
    {
        i->zeroFill();
    }
}

ଶୂନ୍ୟ BufferArea::commit()
{
#ifndef OPENCV_ENABLE_MEMORY_SANITIZER
    ଯଦି (!safe)
    {
        CV_Assert(totalSize > 0);
        CV_Assert(oneBuf == ଖାଲି);
        CV_Assert(!blocks.empty());
        oneBuf = fastMalloc(totalSize);
        ଶୂନ୍ୟ * ptr = oneBuf;
        ପାଇଁ(std::vector<Block>::const_iterator i = blocks.begin(); i != blocks.end(); ++i)
        {
            ptr = i->fast_allocate(ptr);
        }
    }
#endif
}

ଶୂନ୍ୟ BufferArea::release()
{
    ପାଇଁ(std::vector<Block>::const_iterator i = blocks.begin(); i != blocks.end(); ++i)
    {
        i->cleanup();
    }
    blocks.clear();
#ifndef OPENCV_ENABLE_MEMORY_SANITIZER
    ଯଦି (oneBuf)
    {
        fastFree(oneBuf);
        oneBuf = 0;
    }
#endif
}

//==================================================================================================

}} // cv::utils::
