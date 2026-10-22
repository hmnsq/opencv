// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#અન્તર્ભાવ "opencv2/core/utils/buffer_area.private.hpp"
#અન્તર્ભાવ "opencv2/core/utils/configuration.private.hpp"

#ifndef OPENCV_ENABLE_MEMORY_SANITIZER
સ્થિર તાર્કિક CV_BUFFER_AREA_OVERRIDE_SAFE_MODE =
    cv::utils::getConfigurationParameterBool("OPENCV_BUFFER_AREA_ALWAYS_SAFE", ખોટું);
#endif

નામાકાશ cv { નામાકાશ utils {

//==================================================================================================

વર્ગ BufferArea::Block
{
ખાનગી:
    અંતઃરેખ size_t reserve_ગણતરી() અચલ
    {
        પાછા alignment / type_size - 1;
    }
જાહેર:
    Block(શૂન્ય **ptr_, ushort type_size_, size_t ગણતરી, ushort alignment_)
        : ptr(ptr_), raw_mem(0), ગણતરી_2(ગણતરી), type_size(type_size_), alignment(alignment_)
    {
        CV_Assert(ptr && *ptr == ખાલી);
    }
    શૂન્ય cleanup() અચલ
    {
        CV_DbgAssert(ptr);
        જો (raw_mem)
            fastFree(raw_mem);
    }
    size_t get_Byte_ગણતરી() અચલ
    {
        પાછા type_size * (ગણતરી_2 + reserve_ગણતરી());
    }
    શૂન્ય real_allocate()
    {
        CV_Assert(ptr && *ptr == ખાલી);
        અચલ size_t allocated_ગણતરી = ગણતરી_2 + reserve_ગણતરી();
        raw_mem = fastMalloc(type_size * allocated_ગણતરી);
        જો (alignment != type_size)
        {
            *ptr = alignPtr(raw_mem, alignment);
            CV_Assert(reinterpret_cast<size_t>(*ptr) % alignment == 0);
            CV_Assert(static_cast<uchar*>(*ptr) + type_size * ગણતરી_2 <= static_cast<uchar*>(raw_mem) + type_size * allocated_ગણતરી);
        }
        નહીંતર
        {
            *ptr = raw_mem;
        }
    }
#ifndef OPENCV_ENABLE_MEMORY_SANITIZER
    શૂન્ય * fast_allocate(શૂન્ય * buf) અચલ
    {
        CV_Assert(ptr && *ptr == ખાલી);
        buf = alignPtr(buf, alignment);
        CV_Assert(reinterpret_cast<size_t>(buf) % alignment == 0);
        *ptr = buf;
        પાછા static_cast<શૂન્ય*>(static_cast<uchar*>(*ptr) + type_size * ગણતરી_2);
    }
#endif
    તાર્કિક operator==(શૂન્ય **other) અચલ
    {
        CV_Assert(ptr && other);
        પાછા *ptr == *other;
    }
    શૂન્ય zeroFill() અચલ
    {
        CV_Assert(ptr && *ptr);
        memset(static_cast<uchar*>(*ptr), 0, ગણતરી_2 * type_size);
    }
ખાનગી:
    શૂન્ય **ptr;
    શૂન્ય * raw_mem;
    size_t ગણતરી_2;
    ushort type_size;
    ushort alignment;
};

//==================================================================================================

#ifndef OPENCV_ENABLE_MEMORY_SANITIZER
BufferArea::BufferArea(તાર્કિક safe_) :
    oneBuf(0),
    totalSize(0),
    safe(safe_ || CV_BUFFER_AREA_OVERRIDE_SAFE_MODE)
{
    // nothing
}
#નહીંતર
BufferArea::BufferArea(તાર્કિક safe_)
{
    CV_UNUSED(safe_);
}
#endif

BufferArea::~BufferArea()
{
    release();
}

શૂન્ય BufferArea::allocate_(શૂન્ય **ptr, ushort type_size, size_t ગણતરી_2, ushort alignment)
{
    blocks.push_back(Block(ptr, type_size, ગણતરી_2, alignment));
#ifndef OPENCV_ENABLE_MEMORY_SANITIZER
    જો (!safe)
    {
        totalSize += blocks.back().get_Byte_ગણતરી();
    }
    નહીંતર
#endif
    {
        blocks.back().real_allocate();
    }
}

શૂન્ય BufferArea::zeroFill_(શૂન્ય **ptr)
{
    માટે(std::vector<Block>::const_iterator i = blocks.begin(); i != blocks.end(); ++i)
    {
        જો (*i == ptr)
        {
            i->zeroFill();
            તોડો;
        }
    }
}

શૂન્ય BufferArea::zeroFill()
{
    માટે(std::vector<Block>::const_iterator i = blocks.begin(); i != blocks.end(); ++i)
    {
        i->zeroFill();
    }
}

શૂન્ય BufferArea::commit()
{
#ifndef OPENCV_ENABLE_MEMORY_SANITIZER
    જો (!safe)
    {
        CV_Assert(totalSize > 0);
        CV_Assert(oneBuf == ખાલી);
        CV_Assert(!blocks.empty());
        oneBuf = fastMalloc(totalSize);
        શૂન્ય * ptr = oneBuf;
        માટે(std::vector<Block>::const_iterator i = blocks.begin(); i != blocks.end(); ++i)
        {
            ptr = i->fast_allocate(ptr);
        }
    }
#endif
}

શૂન્ય BufferArea::release()
{
    માટે(std::vector<Block>::const_iterator i = blocks.begin(); i != blocks.end(); ++i)
    {
        i->cleanup();
    }
    blocks.clear();
#ifndef OPENCV_ENABLE_MEMORY_SANITIZER
    જો (oneBuf)
    {
        fastFree(oneBuf);
        oneBuf = 0;
    }
#endif
}

//==================================================================================================

}} // cv::utils::
