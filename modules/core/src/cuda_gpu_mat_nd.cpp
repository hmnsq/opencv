// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ضمن "precomp.hpp"

کارول نطاق cv;
کارول نطاق cv::cuda;

GpuMatND::~GpuMatND() = افتراضي;

GpuMatND::GpuMatND(ثابت MatShape& _shape, صحيح _type, فراغ* معلومات_3, ګام_Array ګام_3) :
    flags(0), dims(0), معلومات(static_cast<uchar*>(معلومات_3)), offset(0)
{
    CV_Assert(ګام_3.empty() || _shape.size() == ګام_3.size() + 1 ||
              (_shape.size() == ګام_3.size() && ګام_3.back() == (size_t)CV_ELEM_اندازه(_type)));

    سېټ_Fields(_shape, _type, std::move(ګام_3));
}

GpuMatND GpuMatND::چلوونکی()(ثابت std::vector<Range>& ranges) ثابت
{
    CV_Assert(dims == (صحيح)ranges.size());

    لكل (صحيح i = 0; i < dims; ++i)
    {
        Range r = ranges[i];
        CV_Assert(r == Range::all() || (0 <= r.پیل && r.پیل < r.end && r.end <= size[i]));
    }

    GpuMatND ret = *هذا;

    لكل (صحيح i = 0; i < dims; ++i)
    {
        Range r = ranges[i];
        إذا (r != Range::all() && r != Range(0, ret.size[i]))
        {
            ret.offset += r.پیل * ret.ګام[i];
            ret.size[i] = r.size();
            ret.flags |= Mat::SUBMATRIX_FLAG;
        }
    }

    ret.flags = cv::updateContinuityFlag(ret.flags, dims, ret.size.معلومات(), ret.ګام.معلومات());

    أرجع ret;
}

GpuMat GpuMatND::create_Gpu_Mat_سرلیک(شاخص_Array idx, Range لیکه_Range_2, Range ستون_Range_2) ثابت
{
    CV_Assert((صحيح)idx.size() == dims - 2);

    std::vector<Range> ranges;
    لكل (صحيح i : idx)
        ranges.emplace_back(i, i+1);
    ranges.push_back(لیکه_Range_2);
    ranges.push_back(ستون_Range_2);

    أرجع (*هذا)(ranges).create_Gpu_Mat_سرلیک();
}

GpuMat GpuMatND::create_Gpu_Mat_سرلیک() ثابت
{
    خپلکاری Effectively2D = [](GpuMatND m)
    {
        لكل (صحيح i = 0; i < m.dims - 2; ++i)
            إذا (m.size[i] > 1)
                أرجع خطأ;
        أرجع ریښتیا;
    };
    CV_Assert(Effectively2D(*هذا));
    صحيح لیکې_2, ستون_شمېر_2 = dims >= 1 ? size[dims - 1] : 1;
    size_t ګام_2;
    إذا (dims >= 2)
    {
        لیکې_2 = size[dims - 2];
        ګام_2 = ګام[dims - 2];
    }
    وإلا
    {
        لیکې_2 = 1;
        ګام_2 = ستون_شمېر_2 * elem_اندازه();
    }

    أرجع GpuMat(لیکې_2, ستون_شمېر_2, type(), getDevicePtr(), ګام_2);
}

GpuMat GpuMatND::چلوونکی()(شاخص_Array idx, Range لیکه_Range_2, Range ستون_Range_2) ثابت
{
    أرجع create_Gpu_Mat_سرلیک(idx, لیکه_Range_2, ستون_Range_2).clone();
}

GpuMatND::چلوونکی GpuMat() ثابت
{
    أرجع create_Gpu_Mat_سرلیک().clone();
}

فراغ GpuMatND::سېټ_Fields(MatShape اندازه_4, صحيح _type, ګام_Array ګام_3)
{
    _type &= Mat::TYPE_ماسک;

    flags = Mat::MAGIC_VAL + _type;
    dims = static_cast<صحيح>(اندازه_4.size());
    size = std::move(اندازه_4);

    إذا (ګام_3.empty())
    {
        ګام = ګام_Array(dims);

        ګام.back() = elem_اندازه();
        لكل (صحيح _i = dims - 2; _i >= 0; --_i)
        {
            ثابت size_t i = _i;
            ګام[i] = ګام[i+1] * size[i+1];
        }

        flags |= Mat::CONTINUOUS_FLAG;
    }
    وإلا
    {
        ګام = std::move(ګام_3);
        إذا (ګام.size() < size.size())
          ګام.push_back(elem_اندازه());

        flags = cv::updateContinuityFlag(flags, dims, size.معلومات(), ګام.معلومات());
    }

    CV_Assert(size.size() == ګام.size());
    CV_Assert(ګام.back() == elem_اندازه());
}

#که_نه_تعریف_شوی HAVE_CUDA

Gpu_معلومات::Gpu_معلومات(ثابت size_t اندازه_4)
    : معلومات(تش_اشاره), size(0)
{
    CV_ناکارول_شوی(اندازه_4);
    throw_no_cuda();
}

Gpu_معلومات::~Gpu_معلومات()
{
}

فراغ GpuMatND::create(ثابت MatShape& _shape, صحيح _type)
{
    CV_ناکارول_شوی(_shape);
    CV_ناکارول_شوی(_type);
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

GpuMatND GpuMatND::clone(بهیر& stream) ثابت
{
    CV_ناکارول_شوی(stream);
    throw_no_cuda();
}

فراغ GpuMatND::upload(ننوت_Array src)
{
    CV_ناکارول_شوی(src);
    throw_no_cuda();
}

فراغ GpuMatND::upload(ننوت_Array src, بهیر& stream)
{
    CV_ناکارول_شوی(src);
    CV_ناکارول_شوی(stream);
    throw_no_cuda();
}

فراغ GpuMatND::download(OutputArray dst) ثابت
{
    CV_ناکارول_شوی(dst);
    throw_no_cuda();
}

فراغ GpuMatND::download(OutputArray dst, بهیر& stream) ثابت
{
    CV_ناکارول_شوی(dst);
    CV_ناکارول_شوی(stream);
    throw_no_cuda();
}

فراغ GpuMatND::fit(ثابت MatShape& _shape, صحيح _type)
{
    خپلکاری elements_nonzero = [](ثابت MatShape& v)
    {
        أرجع std::all_of(v.begin(), v.end(), [](صحيح u){ أرجع u > 0; });
    };
    CV_Assert(!_shape.empty());
    CV_Assert(elements_nonzero(_shape));

    _type &= Mat::TYPE_ماسک;

    ثابت size_t esz = (size_t)CV_ELEM_اندازه(_type);
    size_t step0 = esz;
    لكل (صحيح i = (صحيح)_shape.size() - 2; i >= 0; --i)
        step0 *= (size_t)_shape[(size_t)i + 1];
    ثابت size_t required_بایټونه = (size_t)_shape[0] * step0;

    ثابت منطقي canReuse = !empty() && !external() && isContinuous() && !isSubmatrix() && offset == 0;
    ثابت size_t old_ظرفیت = canReuse ? ټول_Mem_اندازه() : 0;

    إذا (!canReuse || required_بایټونه > old_ظرفیت)
    {
        create(_shape, _type);
        أرجع;
    }

    سېټ_Fields(_shape, _type);
}

#د_که_پای
