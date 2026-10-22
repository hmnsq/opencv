// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#അന്തര്ഭാവ "opencv2/core/utils/buffer_area.private.hpp"
#അന്തര്ഭാവ "opencv2/core/utils/configuration.private.hpp"

#ifndef OPENCV_ENABLE_MEMORY_SANITIZER
സ്ഥിതം തർക്കം CV_BUFFER_AREA_OVERRIDE_SAFE_MODE =
    cv::utils::getConfigurationParameterBool("OPENCV_BUFFER_AREA_ALWAYS_SAFE", അസത്യം);
#endif

നാമാകാശ cv { നാമാകാശ utils {

//==================================================================================================

വർഗ്ഗം BufferArea::Block
{
സ്വകാര്യ:
    വരിയിൽ size_t reserve_എണ്ണം() സ്ഥിരം
    {
        തിരികെ alignment / type_size - 1;
    }
പൊതു:
    Block(ശൂന്യം **ptr_, ushort type_size_, size_t എണ്ണം, ushort alignment_)
        : ptr(ptr_), raw_mem(0), എണ്ണം_2(എണ്ണം), type_size(type_size_), alignment(alignment_)
    {
        CV_Assert(ptr && *ptr == ശൂന്യ);
    }
    ശൂന്യം cleanup() സ്ഥിരം
    {
        CV_DbgAssert(ptr);
        എങ്കിൽ (raw_mem)
            fastFree(raw_mem);
    }
    size_t get_Byte_എണ്ണം() സ്ഥിരം
    {
        തിരികെ type_size * (എണ്ണം_2 + reserve_എണ്ണം());
    }
    ശൂന്യം real_allocate()
    {
        CV_Assert(ptr && *ptr == ശൂന്യ);
        സ്ഥിരം size_t allocated_എണ്ണം = എണ്ണം_2 + reserve_എണ്ണം();
        raw_mem = fastMalloc(type_size * allocated_എണ്ണം);
        എങ്കിൽ (alignment != type_size)
        {
            *ptr = alignPtr(raw_mem, alignment);
            CV_Assert(reinterpret_cast<size_t>(*ptr) % alignment == 0);
            CV_Assert(static_cast<uchar*>(*ptr) + type_size * എണ്ണം_2 <= static_cast<uchar*>(raw_mem) + type_size * allocated_എണ്ണം);
        }
        അല്ലെങ്കിൽ
        {
            *ptr = raw_mem;
        }
    }
#ifndef OPENCV_ENABLE_MEMORY_SANITIZER
    ശൂന്യം * fast_allocate(ശൂന്യം * buf) സ്ഥിരം
    {
        CV_Assert(ptr && *ptr == ശൂന്യ);
        buf = alignPtr(buf, alignment);
        CV_Assert(reinterpret_cast<size_t>(buf) % alignment == 0);
        *ptr = buf;
        തിരികെ static_cast<ശൂന്യം*>(static_cast<uchar*>(*ptr) + type_size * എണ്ണം_2);
    }
#endif
    തർക്കം operator==(ശൂന്യം **other) സ്ഥിരം
    {
        CV_Assert(ptr && other);
        തിരികെ *ptr == *other;
    }
    ശൂന്യം zeroFill() സ്ഥിരം
    {
        CV_Assert(ptr && *ptr);
        memset(static_cast<uchar*>(*ptr), 0, എണ്ണം_2 * type_size);
    }
സ്വകാര്യ:
    ശൂന്യം **ptr;
    ശൂന്യം * raw_mem;
    size_t എണ്ണം_2;
    ushort type_size;
    ushort alignment;
};

//==================================================================================================

#ifndef OPENCV_ENABLE_MEMORY_SANITIZER
BufferArea::BufferArea(തർക്കം safe_) :
    oneBuf(0),
    totalSize(0),
    safe(safe_ || CV_BUFFER_AREA_OVERRIDE_SAFE_MODE)
{
    // nothing
}
#അല്ലെങ്കിൽ
BufferArea::BufferArea(തർക്കം safe_)
{
    CV_UNUSED(safe_);
}
#endif

BufferArea::~BufferArea()
{
    release();
}

ശൂന്യം BufferArea::allocate_(ശൂന്യം **ptr, ushort type_size, size_t എണ്ണം_2, ushort alignment)
{
    blocks.push_back(Block(ptr, type_size, എണ്ണം_2, alignment));
#ifndef OPENCV_ENABLE_MEMORY_SANITIZER
    എങ്കിൽ (!safe)
    {
        totalSize += blocks.back().get_Byte_എണ്ണം();
    }
    അല്ലെങ്കിൽ
#endif
    {
        blocks.back().real_allocate();
    }
}

ശൂന്യം BufferArea::zeroFill_(ശൂന്യം **ptr)
{
    വേണ്ടി(std::vector<Block>::const_iterator i = blocks.begin(); i != blocks.end(); ++i)
    {
        എങ്കിൽ (*i == ptr)
        {
            i->zeroFill();
            നിർത്തുക;
        }
    }
}

ശൂന്യം BufferArea::zeroFill()
{
    വേണ്ടി(std::vector<Block>::const_iterator i = blocks.begin(); i != blocks.end(); ++i)
    {
        i->zeroFill();
    }
}

ശൂന്യം BufferArea::commit()
{
#ifndef OPENCV_ENABLE_MEMORY_SANITIZER
    എങ്കിൽ (!safe)
    {
        CV_Assert(totalSize > 0);
        CV_Assert(oneBuf == ശൂന്യ);
        CV_Assert(!blocks.empty());
        oneBuf = fastMalloc(totalSize);
        ശൂന്യം * ptr = oneBuf;
        വേണ്ടി(std::vector<Block>::const_iterator i = blocks.begin(); i != blocks.end(); ++i)
        {
            ptr = i->fast_allocate(ptr);
        }
    }
#endif
}

ശൂന്യം BufferArea::release()
{
    വേണ്ടി(std::vector<Block>::const_iterator i = blocks.begin(); i != blocks.end(); ++i)
    {
        i->cleanup();
    }
    blocks.clear();
#ifndef OPENCV_ENABLE_MEMORY_SANITIZER
    എങ്കിൽ (oneBuf)
    {
        fastFree(oneBuf);
        oneBuf = 0;
    }
#endif
}

//==================================================================================================

}} // cv::utils::
