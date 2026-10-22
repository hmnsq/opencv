// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ضمن "precomp.hpp"

استفاده نطاق cv;
استفاده نطاق cv::cuda;

GpuMatND::~GpuMatND() = افتراضي;

GpuMatND::GpuMatND(ثابت MatShape& _shape, صحيح _type, فراغ* داده_3, گام_Array گام_3) :
    flags(0), dims(0), داده(static_cast<uchar*>(داده_3)), offset(0)
{
    CV_Assert(گام_3.empty() || _shape.size() == گام_3.size() + 1 ||
              (_shape.size() == گام_3.size() && گام_3.back() == (size_t)CV_ELEM_اندازه(_type)));

    مجموعه_Fields(_shape, _type, std::move(گام_3));
}

GpuMatND GpuMatND::عملگر()(ثابت std::vector<Range>& ranges) ثابت
{
    CV_Assert(dims == (صحيح)ranges.size());

    لكل (صحيح i = 0; i < dims; ++i)
    {
        Range r = ranges[i];
        CV_Assert(r == Range::all() || (0 <= r.شروع && r.شروع < r.end && r.end <= size[i]));
    }

    GpuMatND ret = *هذا;

    لكل (صحيح i = 0; i < dims; ++i)
    {
        Range r = ranges[i];
        إذا (r != Range::all() && r != Range(0, ret.size[i]))
        {
            ret.offset += r.شروع * ret.گام[i];
            ret.size[i] = r.size();
            ret.flags |= Mat::SUBMATRIX_FLAG;
        }
    }

    ret.flags = cv::updateContinuityFlag(ret.flags, dims, ret.size.داده(), ret.گام.داده());

    أرجع ret;
}

GpuMat GpuMatND::create_Gpu_Mat_سرآیند(شاخص_Array idx, Range ردیف_Range_2, Range ستون_Range_2) ثابت
{
    CV_Assert((صحيح)idx.size() == dims - 2);

    std::vector<Range> ranges;
    لكل (صحيح i : idx)
        ranges.emplace_back(i, i+1);
    ranges.push_back(ردیف_Range_2);
    ranges.push_back(ستون_Range_2);

    أرجع (*هذا)(ranges).create_Gpu_Mat_سرآیند();
}

GpuMat GpuMatND::create_Gpu_Mat_سرآیند() ثابت
{
    خودکار Effectively2D = [](GpuMatND m)
    {
        لكل (صحيح i = 0; i < m.dims - 2; ++i)
            إذا (m.size[i] > 1)
                أرجع خطأ;
        أرجع درست;
    };
    CV_Assert(Effectively2D(*هذا));
    صحيح ردیفها_2, تعداد_ستون_2 = dims >= 1 ? size[dims - 1] : 1;
    size_t گام_2;
    إذا (dims >= 2)
    {
        ردیفها_2 = size[dims - 2];
        گام_2 = گام[dims - 2];
    }
    وإلا
    {
        ردیفها_2 = 1;
        گام_2 = تعداد_ستون_2 * elem_اندازه();
    }

    أرجع GpuMat(ردیفها_2, تعداد_ستون_2, type(), getDevicePtr(), گام_2);
}

GpuMat GpuMatND::عملگر()(شاخص_Array idx, Range ردیف_Range_2, Range ستون_Range_2) ثابت
{
    أرجع create_Gpu_Mat_سرآیند(idx, ردیف_Range_2, ستون_Range_2).clone();
}

GpuMatND::عملگر GpuMat() ثابت
{
    أرجع create_Gpu_Mat_سرآیند().clone();
}

فراغ GpuMatND::مجموعه_Fields(MatShape اندازه_4, صحيح _type, گام_Array گام_3)
{
    _type &= Mat::TYPE_ماسک;

    flags = Mat::MAGIC_VAL + _type;
    dims = static_cast<صحيح>(اندازه_4.size());
    size = std::move(اندازه_4);

    إذا (گام_3.empty())
    {
        گام = گام_Array(dims);

        گام.back() = elem_اندازه();
        لكل (صحيح _i = dims - 2; _i >= 0; --_i)
        {
            ثابت size_t i = _i;
            گام[i] = گام[i+1] * size[i+1];
        }

        flags |= Mat::CONTINUOUS_FLAG;
    }
    وإلا
    {
        گام = std::move(گام_3);
        إذا (گام.size() < size.size())
          گام.push_back(elem_اندازه());

        flags = cv::updateContinuityFlag(flags, dims, size.داده(), گام.داده());
    }

    CV_Assert(size.size() == گام.size());
    CV_Assert(گام.back() == elem_اندازه());
}

#اگر_تعریف_نشده HAVE_CUDA

Gpu_داده::Gpu_داده(ثابت size_t اندازه_4)
    : داده(اشاره_تهی), size(0)
{
    CV_استفادهنشده(اندازه_4);
    throw_no_cuda();
}

Gpu_داده::~Gpu_داده()
{
}

فراغ GpuMatND::create(ثابت MatShape& _shape, صحيح _type)
{
    CV_استفادهنشده(_shape);
    CV_استفادهنشده(_type);
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

GpuMatND GpuMatND::clone(جریان& stream) ثابت
{
    CV_استفادهنشده(stream);
    throw_no_cuda();
}

فراغ GpuMatND::upload(درون_داد_Array src)
{
    CV_استفادهنشده(src);
    throw_no_cuda();
}

فراغ GpuMatND::upload(درون_داد_Array src, جریان& stream)
{
    CV_استفادهنشده(src);
    CV_استفادهنشده(stream);
    throw_no_cuda();
}

فراغ GpuMatND::download(OutputArray dst) ثابت
{
    CV_استفادهنشده(dst);
    throw_no_cuda();
}

فراغ GpuMatND::download(OutputArray dst, جریان& stream) ثابت
{
    CV_استفادهنشده(dst);
    CV_استفادهنشده(stream);
    throw_no_cuda();
}

فراغ GpuMatND::fit(ثابت MatShape& _shape, صحيح _type)
{
    خودکار elements_nonzero = [](ثابت MatShape& v)
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
    ثابت size_t required_بایتها = (size_t)_shape[0] * step0;

    ثابت منطقي canReuse = !empty() && !external() && isContinuous() && !isSubmatrix() && offset == 0;
    ثابت size_t old_ظرفیت = canReuse ? مجموع_Mem_اندازه() : 0;

    إذا (!canReuse || required_بایتها > old_ظرفیت)
    {
        create(_shape, _type);
        أرجع;
    }

    مجموعه_Fields(_shape, _type);
}

#پایان_اگر
