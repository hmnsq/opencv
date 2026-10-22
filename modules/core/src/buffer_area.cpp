// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#अन्तर्भाव "opencv2/core/utils/buffer_area.private.hpp"
#अन्तर्भाव "opencv2/core/utils/configuration.private.hpp"

#यदि_अपरिभाषितम् OPENCV_ENABLE_स्मृतिः_SANITIZER
स्थिर तार्किक CV_बफर_AREA_OVERRIDE_SAFE_बहुलकम् =
    cv::utils::getConfigurationParameterBool("OPENCV_BUFFER_AREA_ALWAYS_SAFE", असत्य);
#यदि_अन्तः

नामाकाश cv { नामाकाश utils {

//==================================================================================================

वर्ग बफर_Area::खण्डम्
{
गुप्त:
    अन्तःस्थ size_t reserve_गणनम्() ध्रुव
    {
        प्रत्यागम alignment / type_आकार - 1;
    }
सार्वजनिक:
    खण्डम्(शून्य **ptr_, ushort type_आकार_2, size_t गणनम्, ushort alignment_)
        : ptr(ptr_), raw_mem(0), गणनम्_2(गणनम्), type_आकार(type_आकार_2), alignment(alignment_)
    {
        CV_Assert(ptr && *ptr == रिक्त);
    }
    शून्य cleanup() ध्रुव
    {
        CV_DbgAssert(ptr);
        यदि (raw_mem)
            fastFree(raw_mem);
    }
    size_t get_Byte_गणनम्() ध्रुव
    {
        प्रत्यागम type_आकार * (गणनम्_2 + reserve_गणनम्());
    }
    शून्य real_allocate()
    {
        CV_Assert(ptr && *ptr == रिक्त);
        ध्रुव size_t allocated_गणनम् = गणनम्_2 + reserve_गणनम्();
        raw_mem = fastMalloc(type_आकार * allocated_गणनम्);
        यदि (alignment != type_आकार)
        {
            *ptr = alignPtr(raw_mem, alignment);
            CV_Assert(reinterpret_cast<size_t>(*ptr) % alignment == 0);
            CV_Assert(static_cast<uchar*>(*ptr) + type_आकार * गणनम्_2 <= static_cast<uchar*>(raw_mem) + type_आकार * allocated_गणनम्);
        }
        अन्यथा
        {
            *ptr = raw_mem;
        }
    }
#यदि_अपरिभाषितम् OPENCV_ENABLE_स्मृतिः_SANITIZER
    शून्य * fast_allocate(शून्य * buf) ध्रुव
    {
        CV_Assert(ptr && *ptr == रिक्त);
        buf = alignPtr(buf, alignment);
        CV_Assert(reinterpret_cast<size_t>(buf) % alignment == 0);
        *ptr = buf;
        प्रत्यागम static_cast<शून्य*>(static_cast<uchar*>(*ptr) + type_आकार * गणनम्_2);
    }
#यदि_अन्तः
    तार्किक संक्रियकः==(शून्य **other) ध्रुव
    {
        CV_Assert(ptr && other);
        प्रत्यागम *ptr == *other;
    }
    शून्य zeroFill() ध्रुव
    {
        CV_Assert(ptr && *ptr);
        स्मृतिं_पूरय(static_cast<uchar*>(*ptr), 0, गणनम्_2 * type_आकार);
    }
गुप्त:
    शून्य **ptr;
    शून्य * raw_mem;
    size_t गणनम्_2;
    ushort type_आकार;
    ushort alignment;
};

//==================================================================================================

#यदि_अपरिभाषितम् OPENCV_ENABLE_स्मृतिः_SANITIZER
बफर_Area::बफर_Area(तार्किक safe_) :
    oneBuf(0),
    योग_आकार(0),
    safe(safe_ || CV_बफर_AREA_OVERRIDE_SAFE_बहुलकम्)
{
    // nothing
}
#अन्यथा
बफर_Area::बफर_Area(तार्किक safe_)
{
    CV_अप्रयुक्तम्(safe_);
}
#यदि_अन्तः

बफर_Area::~बफर_Area()
{
    release();
}

शून्य बफर_Area::allocate_(शून्य **ptr, ushort type_आकार, size_t गणनम्_2, ushort alignment)
{
    खण्डानि.push_back(खण्डम्(ptr, type_आकार, गणनम्_2, alignment));
#यदि_अपरिभाषितम् OPENCV_ENABLE_स्मृतिः_SANITIZER
    यदि (!safe)
    {
        योग_आकार += खण्डानि.back().get_Byte_गणनम्();
    }
    अन्यथा
#यदि_अन्तः
    {
        खण्डानि.back().real_allocate();
    }
}

शून्य बफर_Area::zeroFill_(शून्य **ptr)
{
    प्रति(std::vector<खण्डम्>::const_iterator i = खण्डानि.begin(); i != खण्डानि.end(); ++i)
    {
        यदि (*i == ptr)
        {
            i->zeroFill();
            भङ्ग;
        }
    }
}

शून्य बफर_Area::zeroFill()
{
    प्रति(std::vector<खण्डम्>::const_iterator i = खण्डानि.begin(); i != खण्डानि.end(); ++i)
    {
        i->zeroFill();
    }
}

शून्य बफर_Area::commit()
{
#यदि_अपरिभाषितम् OPENCV_ENABLE_स्मृतिः_SANITIZER
    यदि (!safe)
    {
        CV_Assert(योग_आकार > 0);
        CV_Assert(oneBuf == रिक्त);
        CV_Assert(!खण्डानि.empty());
        oneBuf = fastMalloc(योग_आकार);
        शून्य * ptr = oneBuf;
        प्रति(std::vector<खण्डम्>::const_iterator i = खण्डानि.begin(); i != खण्डानि.end(); ++i)
        {
            ptr = i->fast_allocate(ptr);
        }
    }
#यदि_अन्तः
}

शून्य बफर_Area::release()
{
    प्रति(std::vector<खण्डम्>::const_iterator i = खण्डानि.begin(); i != खण्डानि.end(); ++i)
    {
        i->cleanup();
    }
    खण्डानि.clear();
#यदि_अपरिभाषितम् OPENCV_ENABLE_स्मृतिः_SANITIZER
    यदि (oneBuf)
    {
        fastFree(oneBuf);
        oneBuf = 0;
    }
#यदि_अन्तः
}

//==================================================================================================

}} // cv::utils::
