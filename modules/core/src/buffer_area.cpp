// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#अन्तर्भाव "opencv2/core/utils/buffer_area.private.hpp"
#अन्तर्भाव "opencv2/core/utils/configuration.private.hpp"

#ifndef OPENCV_ENABLE_MEMORY_SANITIZER
स्थिर तार्किक CV_BUFFER_AREA_OVERRIDE_SAFE_MODE =
    cv::utils::getConfigurationParameterBool("OPENCV_BUFFER_AREA_ALWAYS_SAFE", असत्य);
#endif

नामाकाश cv { नामाकाश utils {

//==================================================================================================

वर्ग BufferArea::Block
{
खाजगी:
    ओळीत माप_प्रकार reserve_मोजणी() स्थिरांक
    {
        परत alignment / type_size - 1;
    }
सार्वजनिक:
    Block(शून्य **ptr_, ushort type_size_, माप_प्रकार मोजणी, ushort alignment_)
        : ptr(ptr_), raw_mem(0), मोजणी_2(मोजणी), type_size(type_size_), alignment(alignment_)
    {
        CV_Assert(ptr && *ptr == रिक्त);
    }
    शून्य cleanup() स्थिरांक
    {
        CV_DbgAssert(ptr);
        जर (raw_mem)
            fastFree(raw_mem);
    }
    माप_प्रकार get_Byte_मोजणी() स्थिरांक
    {
        परत type_size * (मोजणी_2 + reserve_मोजणी());
    }
    शून्य real_allocate()
    {
        CV_Assert(ptr && *ptr == रिक्त);
        स्थिरांक माप_प्रकार allocated_मोजणी = मोजणी_2 + reserve_मोजणी();
        raw_mem = fastMalloc(type_size * allocated_मोजणी);
        जर (alignment != type_size)
        {
            *ptr = alignPtr(raw_mem, alignment);
            CV_Assert(reinterpret_cast<माप_प्रकार>(*ptr) % alignment == 0);
            CV_Assert(static_cast<uchar*>(*ptr) + type_size * मोजणी_2 <= static_cast<uchar*>(raw_mem) + type_size * allocated_मोजणी);
        }
        अन्यथा
        {
            *ptr = raw_mem;
        }
    }
#ifndef OPENCV_ENABLE_MEMORY_SANITIZER
    शून्य * fast_allocate(शून्य * buf) स्थिरांक
    {
        CV_Assert(ptr && *ptr == रिक्त);
        buf = alignPtr(buf, alignment);
        CV_Assert(reinterpret_cast<माप_प्रकार>(buf) % alignment == 0);
        *ptr = buf;
        परत static_cast<शून्य*>(static_cast<uchar*>(*ptr) + type_size * मोजणी_2);
    }
#endif
    तार्किक चालक==(शून्य **other) स्थिरांक
    {
        CV_Assert(ptr && other);
        परत *ptr == *other;
    }
    शून्य zeroFill() स्थिरांक
    {
        CV_Assert(ptr && *ptr);
        स_रखो(static_cast<uchar*>(*ptr), 0, मोजणी_2 * type_size);
    }
खाजगी:
    शून्य **ptr;
    शून्य * raw_mem;
    माप_प्रकार मोजणी_2;
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
#अन्यथा
BufferArea::BufferArea(तार्किक safe_)
{
    CV_UNUSED(safe_);
}
#endif

BufferArea::~BufferArea()
{
    release();
}

शून्य BufferArea::allocate_(शून्य **ptr, ushort type_size, माप_प्रकार मोजणी_2, ushort alignment)
{
    blocks.push_back(Block(ptr, type_size, मोजणी_2, alignment));
#ifndef OPENCV_ENABLE_MEMORY_SANITIZER
    जर (!safe)
    {
        totalSize += blocks.back().get_Byte_मोजणी();
    }
    अन्यथा
#endif
    {
        blocks.back().real_allocate();
    }
}

शून्य BufferArea::zeroFill_(शून्य **ptr)
{
    साठी(std::vector<Block>::const_iterator i = blocks.begin(); i != blocks.end(); ++i)
    {
        जर (*i == ptr)
        {
            i->zeroFill();
            थांबा;
        }
    }
}

शून्य BufferArea::zeroFill()
{
    साठी(std::vector<Block>::const_iterator i = blocks.begin(); i != blocks.end(); ++i)
    {
        i->zeroFill();
    }
}

शून्य BufferArea::commit()
{
#ifndef OPENCV_ENABLE_MEMORY_SANITIZER
    जर (!safe)
    {
        CV_Assert(totalSize > 0);
        CV_Assert(oneBuf == रिक्त);
        CV_Assert(!blocks.empty());
        oneBuf = fastMalloc(totalSize);
        शून्य * ptr = oneBuf;
        साठी(std::vector<Block>::const_iterator i = blocks.begin(); i != blocks.end(); ++i)
        {
            ptr = i->fast_allocate(ptr);
        }
    }
#endif
}

शून्य BufferArea::release()
{
    साठी(std::vector<Block>::const_iterator i = blocks.begin(); i != blocks.end(); ++i)
    {
        i->cleanup();
    }
    blocks.clear();
#ifndef OPENCV_ENABLE_MEMORY_SANITIZER
    जर (oneBuf)
    {
        fastFree(oneBuf);
        oneBuf = 0;
    }
#endif
}

//==================================================================================================

}} // cv::utils::
