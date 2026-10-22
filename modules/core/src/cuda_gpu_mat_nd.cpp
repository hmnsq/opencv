// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ضمن "precomp.hpp"

استعمال نطاق cv;
استعمال نطاق cv::cuda;

GpuMatND::~GpuMatND() = افتراضي;

GpuMatND::GpuMatND(ثابت MatShape& _shape, صحيح _type, فراغ* معلومات_3, قدم_Array قدم_3) :
    flags(0), dims(0), معلومات(static_cast<uchar*>(معلومات_3)), offset(0)
{
    CV_Assert(قدم_3.empty() || _shape.size() == قدم_3.size() + 1 ||
              (_shape.size() == قدم_3.size() && قدم_3.back() == (size_t)CV_ELEM_سائيز(_type)));

    سيٽ_Fields(_shape, _type, std::move(قدم_3));
}

GpuMatND GpuMatND::عامل()(ثابت std::vector<Range>& ranges) ثابت
{
    CV_Assert(dims == (صحيح)ranges.size());

    لكل (صحيح i = 0; i < dims; ++i)
    {
        Range r = ranges[i];
        CV_Assert(r == Range::all() || (0 <= r.شروعات && r.شروعات < r.end && r.end <= size[i]));
    }

    GpuMatND ret = *هذا;

    لكل (صحيح i = 0; i < dims; ++i)
    {
        Range r = ranges[i];
        إذا (r != Range::all() && r != Range(0, ret.size[i]))
        {
            ret.offset += r.شروعات * ret.قدم[i];
            ret.size[i] = r.size();
            ret.flags |= Mat::SUBMATRIX_FLAG;
        }
    }

    ret.flags = cv::updateContinuityFlag(ret.flags, dims, ret.size.معلومات(), ret.قدم.معلومات());

    أرجع ret;
}

GpuMat GpuMatND::create_Gpu_Mat_سرو(اشاريو_Array idx, Range صف_Range_2, Range کانو_Range_2) ثابت
{
    CV_Assert((صحيح)idx.size() == dims - 2);

    std::vector<Range> ranges;
    لكل (صحيح i : idx)
        ranges.emplace_back(i, i+1);
    ranges.push_back(صف_Range_2);
    ranges.push_back(کانو_Range_2);

    أرجع (*هذا)(ranges).create_Gpu_Mat_سرو();
}

GpuMat GpuMatND::create_Gpu_Mat_سرو() ثابت
{
    خودڪار Effectively2D = [](GpuMatND m)
    {
        لكل (صحيح i = 0; i < m.dims - 2; ++i)
            إذا (m.size[i] > 1)
                أرجع خطأ;
        أرجع سچ;
    };
    CV_Assert(Effectively2D(*هذا));
    صحيح صفون_2, کانن_ڳڻپ_2 = dims >= 1 ? size[dims - 1] : 1;
    size_t قدم_2;
    إذا (dims >= 2)
    {
        صفون_2 = size[dims - 2];
        قدم_2 = قدم[dims - 2];
    }
    وإلا
    {
        صفون_2 = 1;
        قدم_2 = کانن_ڳڻپ_2 * elem_سائيز();
    }

    أرجع GpuMat(صفون_2, کانن_ڳڻپ_2, type(), getDevicePtr(), قدم_2);
}

GpuMat GpuMatND::عامل()(اشاريو_Array idx, Range صف_Range_2, Range کانو_Range_2) ثابت
{
    أرجع create_Gpu_Mat_سرو(idx, صف_Range_2, کانو_Range_2).clone();
}

GpuMatND::عامل GpuMat() ثابت
{
    أرجع create_Gpu_Mat_سرو().clone();
}

فراغ GpuMatND::سيٽ_Fields(MatShape سائيز_4, صحيح _type, قدم_Array قدم_3)
{
    _type &= Mat::TYPE_نقاب;

    flags = Mat::MAGIC_VAL + _type;
    dims = static_cast<صحيح>(سائيز_4.size());
    size = std::move(سائيز_4);

    إذا (قدم_3.empty())
    {
        قدم = قدم_Array(dims);

        قدم.back() = elem_سائيز();
        لكل (صحيح _i = dims - 2; _i >= 0; --_i)
        {
            ثابت size_t i = _i;
            قدم[i] = قدم[i+1] * size[i+1];
        }

        flags |= Mat::CONTINUOUS_FLAG;
    }
    وإلا
    {
        قدم = std::move(قدم_3);
        إذا (قدم.size() < size.size())
          قدم.push_back(elem_سائيز());

        flags = cv::updateContinuityFlag(flags, dims, size.معلومات(), قدم.معلومات());
    }

    CV_Assert(size.size() == قدم.size());
    CV_Assert(قدم.back() == elem_سائيز());
}

#جيڪڏهن_وصف_نه_ٿيل HAVE_CUDA

Gpu_معلومات::Gpu_معلومات(ثابت size_t سائيز_4)
    : معلومات(خالي_اشارو), size(0)
{
    CV_اڻ_استعمال_ٿيل(سائيز_4);
    throw_no_cuda();
}

Gpu_معلومات::~Gpu_معلومات()
{
}

فراغ GpuMatND::create(ثابت MatShape& _shape, صحيح _type)
{
    CV_اڻ_استعمال_ٿيل(_shape);
    CV_اڻ_استعمال_ٿيل(_type);
    throw_no_cuda();
}

فراغ GpuMatND::release()
{
    throw_no_cuda();
}

GpuMatND GpuMatND::clone() ثابت
{
    throw_no_cuda();
}

GpuMatND GpuMatND::clone(وهڪرو& stream) ثابت
{
    CV_اڻ_استعمال_ٿيل(stream);
    throw_no_cuda();
}

فراغ GpuMatND::upload(داخلا_Array src)
{
    CV_اڻ_استعمال_ٿيل(src);
    throw_no_cuda();
}

فراغ GpuMatND::upload(داخلا_Array src, وهڪرو& stream)
{
    CV_اڻ_استعمال_ٿيل(src);
    CV_اڻ_استعمال_ٿيل(stream);
    throw_no_cuda();
}

فراغ GpuMatND::download(OutputArray dst) ثابت
{
    CV_اڻ_استعمال_ٿيل(dst);
    throw_no_cuda();
}

فراغ GpuMatND::download(OutputArray dst, وهڪرو& stream) ثابت
{
    CV_اڻ_استعمال_ٿيل(dst);
    CV_اڻ_استعمال_ٿيل(stream);
    throw_no_cuda();
}

فراغ GpuMatND::fit(ثابت MatShape& _shape, صحيح _type)
{
    خودڪار elements_nonzero = [](ثابت MatShape& v)
    {
        أرجع std::all_of(v.begin(), v.end(), [](صحيح u){ أرجع u > 0; });
    };
    CV_Assert(!_shape.empty());
    CV_Assert(elements_nonzero(_shape));

    _type &= Mat::TYPE_نقاب;

    ثابت size_t esz = (size_t)CV_ELEM_سائيز(_type);
    size_t step0 = esz;
    لكل (صحيح i = (صحيح)_shape.size() - 2; i >= 0; --i)
        step0 *= (size_t)_shape[(size_t)i + 1];
    ثابت size_t required_بائيٽون = (size_t)_shape[0] * step0;

    ثابت منطقي canReuse = !empty() && !external() && isContinuous() && !isSubmatrix() && offset == 0;
    ثابت size_t old_صلاحيت = canReuse ? ڪل_Mem_سائيز() : 0;

    إذا (!canReuse || required_بائيٽون > old_صلاحيت)
    {
        create(_shape, _type);
        أرجع;
    }

    سيٽ_Fields(_shape, _type);
}

#جيڪڏهن_پورو
