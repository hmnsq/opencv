// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#समावेश "opencv2/core/utils/buffer_area.private.hpp"
#समावेश "opencv2/core/utils/configuration.private.hpp"

#अगर_अघोषित OPENCV_ENABLE_MEMORY_SANITIZER
अटल bool CV_बफर_AREA_OVERRIDE_SAFE_MODE =
    cv::utils::getConfigurationParameterBool("OPENCV_BUFFER_AREA_ALWAYS_SAFE", false);
#पूर्ण_अगर

namespace cv { namespace utils {

//==================================================================================================

श्रेणी बफर_Area::Block
{
निजी:
    अंतरभूत माप_प्रकार reserve_गिनती() स्थिर
    {
        वापस alignment / type_आकार - 1;
    }
खुला:
    Block(व्योम **ptr_, ushort type_आकार_2, माप_प्रकार गिनती, ushort alignment_)
        : ptr(ptr_), raw_mem(0), गिनती_2(गिनती), type_आकार(type_आकार_2), alignment(alignment_)
    {
        CV_Assert(ptr && *ptr == शून्य);
    }
    व्योम cleanup() स्थिर
    {
        CV_DbgAssert(ptr);
        अगर (raw_mem)
            fastFree(raw_mem);
    }
    माप_प्रकार get_Byte_गिनती() स्थिर
    {
        वापस type_आकार * (गिनती_2 + reserve_गिनती());
    }
    व्योम real_allocate()
    {
        CV_Assert(ptr && *ptr == शून्य);
        स्थिर माप_प्रकार allocated_गिनती = गिनती_2 + reserve_गिनती();
        raw_mem = fastMalloc(type_आकार * allocated_गिनती);
        अगर (alignment != type_आकार)
        {
            *ptr = alignPtr(raw_mem, alignment);
            CV_Assert(reinterpret_cast<माप_प्रकार>(*ptr) % alignment == 0);
            CV_Assert(static_cast<uchar*>(*ptr) + type_आकार * गिनती_2 <= static_cast<uchar*>(raw_mem) + type_आकार * allocated_गिनती);
        }
        अन्यथा
        {
            *ptr = raw_mem;
        }
    }
#अगर_अघोषित OPENCV_ENABLE_MEMORY_SANITIZER
    व्योम * fast_allocate(व्योम * buf) स्थिर
    {
        CV_Assert(ptr && *ptr == शून्य);
        buf = alignPtr(buf, alignment);
        CV_Assert(reinterpret_cast<माप_प्रकार>(buf) % alignment == 0);
        *ptr = buf;
        वापस static_cast<व्योम*>(static_cast<uchar*>(*ptr) + type_आकार * गिनती_2);
    }
#पूर्ण_अगर
    bool चालक==(व्योम **other) स्थिर
    {
        CV_Assert(ptr && other);
        वापस *ptr == *other;
    }
    व्योम zeroFill() स्थिर
    {
        CV_Assert(ptr && *ptr);
        स_रखो(static_cast<uchar*>(*ptr), 0, गिनती_2 * type_आकार);
    }
निजी:
    व्योम **ptr;
    व्योम * raw_mem;
    माप_प्रकार गिनती_2;
    ushort type_आकार;
    ushort alignment;
};

//==================================================================================================

#अगर_अघोषित OPENCV_ENABLE_MEMORY_SANITIZER
बफर_Area::बफर_Area(bool safe_) :
    oneBuf(0),
    योग_आकार(0),
    safe(safe_ || CV_बफर_AREA_OVERRIDE_SAFE_MODE)
{
    // nothing
}
#अन्यथा
बफर_Area::बफर_Area(bool safe_)
{
    CV_UNUSED(safe_);
}
#पूर्ण_अगर

बफर_Area::~बफर_Area()
{
    release();
}

व्योम बफर_Area::allocate_(व्योम **ptr, ushort type_आकार, माप_प्रकार गिनती_2, ushort alignment)
{
    blocks.push_back(Block(ptr, type_आकार, गिनती_2, alignment));
#अगर_अघोषित OPENCV_ENABLE_MEMORY_SANITIZER
    अगर (!safe)
    {
        योग_आकार += blocks.back().get_Byte_गिनती();
    }
    अन्यथा
#पूर्ण_अगर
    {
        blocks.back().real_allocate();
    }
}

व्योम बफर_Area::zeroFill_(व्योम **ptr)
{
    क्रम(std::vector<Block>::const_iterator i = blocks.begin(); i != blocks.end(); ++i)
    {
        अगर (*i == ptr)
        {
            i->zeroFill();
            अवरोध;
        }
    }
}

व्योम बफर_Area::zeroFill()
{
    क्रम(std::vector<Block>::const_iterator i = blocks.begin(); i != blocks.end(); ++i)
    {
        i->zeroFill();
    }
}

व्योम बफर_Area::commit()
{
#अगर_अघोषित OPENCV_ENABLE_MEMORY_SANITIZER
    अगर (!safe)
    {
        CV_Assert(योग_आकार > 0);
        CV_Assert(oneBuf == शून्य);
        CV_Assert(!blocks.empty());
        oneBuf = fastMalloc(योग_आकार);
        व्योम * ptr = oneBuf;
        क्रम(std::vector<Block>::const_iterator i = blocks.begin(); i != blocks.end(); ++i)
        {
            ptr = i->fast_allocate(ptr);
        }
    }
#पूर्ण_अगर
}

व्योम बफर_Area::release()
{
    क्रम(std::vector<Block>::const_iterator i = blocks.begin(); i != blocks.end(); ++i)
    {
        i->cleanup();
    }
    blocks.सफा();
#अगर_अघोषित OPENCV_ENABLE_MEMORY_SANITIZER
    अगर (oneBuf)
    {
        fastFree(oneBuf);
        oneBuf = 0;
    }
#पूर्ण_अगर
}

//==================================================================================================

}} // cv::utils::
