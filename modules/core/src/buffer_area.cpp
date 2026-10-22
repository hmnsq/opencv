// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#అన్తర్భావ "opencv2/core/utils/buffer_area.private.hpp"
#అన్తర్భావ "opencv2/core/utils/configuration.private.hpp"

#నిర్వచితం_కాకపోతే OPENCV_ENABLE_జ్ఞాపకం_SANITIZER
స్థిర తార్కిక CV_బఫరు_AREA_OVERRIDE_సురక్షిత_బహుళకం =
    cv::utils::getConfigurationParameterBool("OPENCV_BUFFER_AREA_ALWAYS_SAFE", అబద్ధం);
#అయితే_ముగింపు

నామాకాశ cv { నామాకాశ utils {

//==================================================================================================

తరగతి బఫరు_Area::ఖండం
{
అంతరంగ:
    అంతఃవరుస size_t reserve_లెక్క() స్థిరరాశి
    {
        తిరిగి alignment / type_కొలత - 1;
    }
బహిరంగ:
    ఖండం(శూన్య **ptr_, ushort type_కొలత_2, size_t లెక్క, ushort alignment_)
        : ptr(ptr_), raw_mem(0), లెక్క_2(లెక్క), type_కొలత(type_కొలత_2), alignment(alignment_)
    {
        CV_Assert(ptr && *ptr == ఖాళీ);
    }
    శూన్య cleanup() స్థిరరాశి
    {
        CV_DbgAssert(ptr);
        ఒకవేళ (raw_mem)
            fastFree(raw_mem);
    }
    size_t get_Byte_లెక్క() స్థిరరాశి
    {
        తిరిగి type_కొలత * (లెక్క_2 + reserve_లెక్క());
    }
    శూన్య real_allocate()
    {
        CV_Assert(ptr && *ptr == ఖాళీ);
        స్థిరరాశి size_t allocated_లెక్క = లెక్క_2 + reserve_లెక్క();
        raw_mem = fastMalloc(type_కొలత * allocated_లెక్క);
        ఒకవేళ (alignment != type_కొలత)
        {
            *ptr = alignPtr(raw_mem, alignment);
            CV_Assert(reinterpret_cast<size_t>(*ptr) % alignment == 0);
            CV_Assert(static_cast<uchar*>(*ptr) + type_కొలత * లెక్క_2 <= static_cast<uchar*>(raw_mem) + type_కొలత * allocated_లెక్క);
        }
        లేకపోతే
        {
            *ptr = raw_mem;
        }
    }
#నిర్వచితం_కాకపోతే OPENCV_ENABLE_జ్ఞాపకం_SANITIZER
    శూన్య * fast_allocate(శూన్య * buf) స్థిరరాశి
    {
        CV_Assert(ptr && *ptr == ఖాళీ);
        buf = alignPtr(buf, alignment);
        CV_Assert(reinterpret_cast<size_t>(buf) % alignment == 0);
        *ptr = buf;
        తిరిగి static_cast<శూన్య*>(static_cast<uchar*>(*ptr) + type_కొలత * లెక్క_2);
    }
#అయితే_ముగింపు
    తార్కిక క్రియాకారకం==(శూన్య **other) స్థిరరాశి
    {
        CV_Assert(ptr && other);
        తిరిగి *ptr == *other;
    }
    శూన్య zeroFill() స్థిరరాశి
    {
        CV_Assert(ptr && *ptr);
        memset(static_cast<uchar*>(*ptr), 0, లెక్క_2 * type_కొలత);
    }
అంతరంగ:
    శూన్య **ptr;
    శూన్య * raw_mem;
    size_t లెక్క_2;
    ushort type_కొలత;
    ushort alignment;
};

//==================================================================================================

#నిర్వచితం_కాకపోతే OPENCV_ENABLE_జ్ఞాపకం_SANITIZER
బఫరు_Area::బఫరు_Area(తార్కిక సురక్షిత) :
    oneBuf(0),
    మొత్తం_కొలత(0),
    సురక్షిత_2(సురక్షిత || CV_బఫరు_AREA_OVERRIDE_సురక్షిత_బహుళకం)
{
    // nothing
}
#లేకపోతే
బఫరు_Area::బఫరు_Area(తార్కిక సురక్షిత)
{
    CV_వాడని(సురక్షిత);
}
#అయితే_ముగింపు

బఫరు_Area::~బఫరు_Area()
{
    release();
}

శూన్య బఫరు_Area::allocate_(శూన్య **ptr, ushort type_కొలత, size_t లెక్క_2, ushort alignment)
{
    ఖండాలు.push_back(ఖండం(ptr, type_కొలత, లెక్క_2, alignment));
#నిర్వచితం_కాకపోతే OPENCV_ENABLE_జ్ఞాపకం_SANITIZER
    ఒకవేళ (!సురక్షిత_2)
    {
        మొత్తం_కొలత += ఖండాలు.back().get_Byte_లెక్క();
    }
    లేకపోతే
#అయితే_ముగింపు
    {
        ఖండాలు.back().real_allocate();
    }
}

శూన్య బఫరు_Area::zeroFill_(శూన్య **ptr)
{
    కొరకు(std::vector<ఖండం>::const_iterator i = ఖండాలు.begin(); i != ఖండాలు.end(); ++i)
    {
        ఒకవేళ (*i == ptr)
        {
            i->zeroFill();
            ఆపు;
        }
    }
}

శూన్య బఫరు_Area::zeroFill()
{
    కొరకు(std::vector<ఖండం>::const_iterator i = ఖండాలు.begin(); i != ఖండాలు.end(); ++i)
    {
        i->zeroFill();
    }
}

శూన్య బఫరు_Area::commit()
{
#నిర్వచితం_కాకపోతే OPENCV_ENABLE_జ్ఞాపకం_SANITIZER
    ఒకవేళ (!సురక్షిత_2)
    {
        CV_Assert(మొత్తం_కొలత > 0);
        CV_Assert(oneBuf == ఖాళీ);
        CV_Assert(!ఖండాలు.empty());
        oneBuf = fastMalloc(మొత్తం_కొలత);
        శూన్య * ptr = oneBuf;
        కొరకు(std::vector<ఖండం>::const_iterator i = ఖండాలు.begin(); i != ఖండాలు.end(); ++i)
        {
            ptr = i->fast_allocate(ptr);
        }
    }
#అయితే_ముగింపు
}

శూన్య బఫరు_Area::release()
{
    కొరకు(std::vector<ఖండం>::const_iterator i = ఖండాలు.begin(); i != ఖండాలు.end(); ++i)
    {
        i->cleanup();
    }
    ఖండాలు.clear();
#నిర్వచితం_కాకపోతే OPENCV_ENABLE_జ్ఞాపకం_SANITIZER
    ఒకవేళ (oneBuf)
    {
        fastFree(oneBuf);
        oneBuf = 0;
    }
#అయితే_ముగింపు
}

//==================================================================================================

}} // cv::utils::
