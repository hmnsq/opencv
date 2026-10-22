// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#अन्तर्भाव "opencv2/core/utils/buffer_area.private.hpp"
#अन्तर्भाव "opencv2/core/utils/configuration.private.hpp"

#ifndef OPENCV_ENABLE_MEMORY_SANITIZER
स्थिर तार्किक CV_BUFFER_AREA_OVERRIDE_SAFE_MODE =
    cv::utils::getConfigurationParameterBool("OPENCV_BUFFER_AREA_ALWAYS_SAFE", मिच्छा);
#endif

नामाकाश cv { नामाकाश utils {

//==================================================================================================

वग्ग BufferArea::Block
{
गुप्त:
    अन्तःस्थ size_t reserve_गणनम्() ध्रुव
    {
        पटिनिवत्त alignment / type_size - 1;
    }
सार्वजनिक:
    Block(शून्य **ptr_, ushort type_size_, size_t गणनम्, ushort alignment_)
        : ptr(ptr_), raw_mem(0), गणनम्_2(गणनम्), type_size(type_size_), alignment(alignment_)
    {
        CV_Assert(ptr && *ptr == रिक्त);
    }
    शून्य cleanup() ध्रुव
    {
        CV_DbgAssert(ptr);
        सचे (raw_mem)
            fastFree(raw_mem);
    }
    size_t get_Byte_गणनम्() ध्रुव
    {
        पटिनिवत्त type_size * (गणनम्_2 + reserve_गणनम्());
    }
    शून्य real_allocate()
    {
        CV_Assert(ptr && *ptr == रिक्त);
        ध्रुव size_t allocated_गणनम् = गणनम्_2 + reserve_गणनम्();
        raw_mem = fastMalloc(type_size * allocated_गणनम्);
        सचे (alignment != type_size)
        {
            *ptr = alignPtr(raw_mem, alignment);
            CV_Assert(reinterpret_cast<size_t>(*ptr) % alignment == 0);
            CV_Assert(static_cast<uchar*>(*ptr) + type_size * गणनम्_2 <= static_cast<uchar*>(raw_mem) + type_size * allocated_गणनम्);
        }
        अञ्ञथा
        {
            *ptr = raw_mem;
        }
    }
#ifndef OPENCV_ENABLE_MEMORY_SANITIZER
    शून्य * fast_allocate(शून्य * buf) ध्रुव
    {
        CV_Assert(ptr && *ptr == रिक्त);
        buf = alignPtr(buf, alignment);
        CV_Assert(reinterpret_cast<size_t>(buf) % alignment == 0);
        *ptr = buf;
        पटिनिवत्त static_cast<शून्य*>(static_cast<uchar*>(*ptr) + type_size * गणनम्_2);
    }
#endif
    तार्किक operator==(शून्य **other) ध्रुव
    {
        CV_Assert(ptr && other);
        पटिनिवत्त *ptr == *other;
    }
    शून्य zeroFill() ध्रुव
    {
        CV_Assert(ptr && *ptr);
        memset(static_cast<uchar*>(*ptr), 0, गणनम्_2 * type_size);
    }
गुप्त:
    शून्य **ptr;
    शून्य * raw_mem;
    size_t गणनम्_2;
    ushort type_size;
    ushort alignment;
};

//==================================================================================================

#ifndef OPENCV_ENABLE_MEMORY_SANITIZER
BufferArea::BufferArea(तार्किक safe_) :
    oneBuf(0),
    totalSize(0),
    safe(safe_ || CV_BUFFER_AREA_OVERRIDE_SAFE_MODE)
{
    // nothing
}
#अञ्ञथा
BufferArea::BufferArea(तार्किक safe_)
{
    CV_UNUSED(safe_);
}
#endif

BufferArea::~BufferArea()
{
    release();
}

शून्य BufferArea::allocate_(शून्य **ptr, ushort type_size, size_t गणनम्_2, ushort alignment)
{
    blocks.push_back(Block(ptr, type_size, गणनम्_2, alignment));
#ifndef OPENCV_ENABLE_MEMORY_SANITIZER
    सचे (!safe)
    {
        totalSize += blocks.back().get_Byte_गणनम्();
    }
    अञ्ञथा
#endif
    {
        blocks.back().real_allocate();
    }
}

शून्य BufferArea::zeroFill_(शून्य **ptr)
{
    अत्थाय(std::vector<Block>::const_iterator i = blocks.begin(); i != blocks.end(); ++i)
    {
        सचे (*i == ptr)
        {
            i->zeroFill();
            भङ्ग;
        }
    }
}

शून्य BufferArea::zeroFill()
{
    अत्थाय(std::vector<Block>::const_iterator i = blocks.begin(); i != blocks.end(); ++i)
    {
        i->zeroFill();
    }
}

शून्य BufferArea::commit()
{
#ifndef OPENCV_ENABLE_MEMORY_SANITIZER
    सचे (!safe)
    {
        CV_Assert(totalSize > 0);
        CV_Assert(oneBuf == रिक्त);
        CV_Assert(!blocks.empty());
        oneBuf = fastMalloc(totalSize);
        शून्य * ptr = oneBuf;
        अत्थाय(std::vector<Block>::const_iterator i = blocks.begin(); i != blocks.end(); ++i)
        {
            ptr = i->fast_allocate(ptr);
        }
    }
#endif
}

शून्य BufferArea::release()
{
    अत्थाय(std::vector<Block>::const_iterator i = blocks.begin(); i != blocks.end(); ++i)
    {
        i->cleanup();
    }
    blocks.clear();
#ifndef OPENCV_ENABLE_MEMORY_SANITIZER
    सचे (oneBuf)
    {
        fastFree(oneBuf);
        oneBuf = 0;
    }
#endif
}

//==================================================================================================

}} // cv::utils::
