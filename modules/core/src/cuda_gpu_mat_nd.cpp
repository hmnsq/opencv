// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#شامل "precomp.hpp"

استعمال نامخلا cv;
استعمال نامخلا cv::cuda;

GpuMatND::~GpuMatND() = طےشدہ;

GpuMatND::GpuMatND(ثابت MatShape& _shape, صحیح _type, خالی* ڈیٹا_3, قدم_Array قدم_3) :
    flags(0), dims(0), ڈیٹا(static_cast<uchar*>(ڈیٹا_3)), offset(0)
{
    CV_Assert(قدم_3.empty() || _shape.size() == قدم_3.size() + 1 ||
              (_shape.size() == قدم_3.size() && قدم_3.back() == (size_t)CV_ELEM_سائز(_type)));

    سیٹ_Fields(_shape, _type, std::move(قدم_3));
}

GpuMatND GpuMatND::عامل()(ثابت std::vector<Range>& ranges) ثابت
{
    CV_Assert(dims == (صحیح)ranges.size());

    برائے (صحیح i = 0; i < dims; ++i)
    {
        Range r = ranges[i];
        CV_Assert(r == Range::all() || (0 <= r.آغاز && r.آغاز < r.end && r.end <= size[i]));
    }

    GpuMatND ret = *یہ;

    برائے (صحیح i = 0; i < dims; ++i)
    {
        Range r = ranges[i];
        اگر (r != Range::all() && r != Range(0, ret.size[i]))
        {
            ret.offset += r.آغاز * ret.قدم[i];
            ret.size[i] = r.size();
            ret.flags |= Mat::SUBMATRIX_FLAG;
        }
    }

    ret.flags = cv::updateContinuityFlag(ret.flags, dims, ret.size.ڈیٹا(), ret.قدم.ڈیٹا());

    واپس ret;
}

GpuMat GpuMatND::create_Gpu_Mat_سرخی(اشاریہ_Array idx, Range صف_Range_2, Range ستون_Range_2) ثابت
{
    CV_Assert((صحیح)idx.size() == dims - 2);

    std::vector<Range> ranges;
    برائے (صحیح i : idx)
        ranges.emplace_back(i, i+1);
    ranges.push_back(صف_Range_2);
    ranges.push_back(ستون_Range_2);

    واپس (*یہ)(ranges).create_Gpu_Mat_سرخی();
}

GpuMat GpuMatND::create_Gpu_Mat_سرخی() ثابت
{
    خودکار Effectively2D = [](GpuMatND m)
    {
        برائے (صحیح i = 0; i < m.dims - 2; ++i)
            اگر (m.size[i] > 1)
                واپس جھوٹ;
        واپس سچ;
    };
    CV_Assert(Effectively2D(*یہ));
    صحیح صفیں_2, ستون_تعداد_2 = dims >= 1 ? size[dims - 1] : 1;
    size_t قدم_2;
    اگر (dims >= 2)
    {
        صفیں_2 = size[dims - 2];
        قدم_2 = قدم[dims - 2];
    }
    ورنہ
    {
        صفیں_2 = 1;
        قدم_2 = ستون_تعداد_2 * elem_سائز();
    }

    واپس GpuMat(صفیں_2, ستون_تعداد_2, type(), getDevicePtr(), قدم_2);
}

GpuMat GpuMatND::عامل()(اشاریہ_Array idx, Range صف_Range_2, Range ستون_Range_2) ثابت
{
    واپس create_Gpu_Mat_سرخی(idx, صف_Range_2, ستون_Range_2).clone();
}

GpuMatND::عامل GpuMat() ثابت
{
    واپس create_Gpu_Mat_سرخی().clone();
}

خالی GpuMatND::سیٹ_Fields(MatShape سائز_4, صحیح _type, قدم_Array قدم_3)
{
    _type &= Mat::TYPE_نقاب;

    flags = Mat::MAGIC_VAL + _type;
    dims = static_cast<صحیح>(سائز_4.size());
    size = std::move(سائز_4);

    اگر (قدم_3.empty())
    {
        قدم = قدم_Array(dims);

        قدم.back() = elem_سائز();
        برائے (صحیح _i = dims - 2; _i >= 0; --_i)
        {
            ثابت size_t i = _i;
            قدم[i] = قدم[i+1] * size[i+1];
        }

        flags |= Mat::CONTINUOUS_FLAG;
    }
    ورنہ
    {
        قدم = std::move(قدم_3);
        اگر (قدم.size() < size.size())
          قدم.push_back(elem_سائز());

        flags = cv::updateContinuityFlag(flags, dims, size.ڈیٹا(), قدم.ڈیٹا());
    }

    CV_Assert(size.size() == قدم.size());
    CV_Assert(قدم.back() == elem_سائز());
}

#اگر_غیر_تعریف_شدہ HAVE_CUDA

Gpu_ڈیٹا::Gpu_ڈیٹا(ثابت size_t سائز_4)
    : ڈیٹا(خالی_اشاریہ), size(0)
{
    CV_غیر_مستعمل(سائز_4);
    throw_no_cuda();
}

Gpu_ڈیٹا::~Gpu_ڈیٹا()
{
}

خالی GpuMatND::create(ثابت MatShape& _shape, صحیح _type)
{
    CV_غیر_مستعمل(_shape);
    CV_غیر_مستعمل(_type);
    throw_no_cuda();
}

خالی GpuMatND::release()
{
    throw_no_cuda();
}

GpuMatND GpuMatND::clone() ثابت
{
    throw_no_cuda();
}

GpuMatND GpuMatND::clone(دھارا& stream) ثابت
{
    CV_غیر_مستعمل(stream);
    throw_no_cuda();
}

خالی GpuMatND::upload(داخلہ_Array src)
{
    CV_غیر_مستعمل(src);
    throw_no_cuda();
}

خالی GpuMatND::upload(داخلہ_Array src, دھارا& stream)
{
    CV_غیر_مستعمل(src);
    CV_غیر_مستعمل(stream);
    throw_no_cuda();
}

خالی GpuMatND::download(OutputArray dst) ثابت
{
    CV_غیر_مستعمل(dst);
    throw_no_cuda();
}

خالی GpuMatND::download(OutputArray dst, دھارا& stream) ثابت
{
    CV_غیر_مستعمل(dst);
    CV_غیر_مستعمل(stream);
    throw_no_cuda();
}

خالی GpuMatND::fit(ثابت MatShape& _shape, صحیح _type)
{
    خودکار elements_nonzero = [](ثابت MatShape& v)
    {
        واپس std::all_of(v.begin(), v.end(), [](صحیح u){ واپس u > 0; });
    };
    CV_Assert(!_shape.empty());
    CV_Assert(elements_nonzero(_shape));

    _type &= Mat::TYPE_نقاب;

    ثابت size_t esz = (size_t)CV_ELEM_سائز(_type);
    size_t step0 = esz;
    برائے (صحیح i = (صحیح)_shape.size() - 2; i >= 0; --i)
        step0 *= (size_t)_shape[(size_t)i + 1];
    ثابت size_t required_بائٹیں = (size_t)_shape[0] * step0;

    ثابت منطقی canReuse = !empty() && !external() && isContinuous() && !isSubmatrix() && offset == 0;
    ثابت size_t old_استعداد = canReuse ? کل_Mem_سائز() : 0;

    اگر (!canReuse || required_بائٹیں > old_استعداد)
    {
        create(_shape, _type);
        واپس;
    }

    سیٹ_Fields(_shape, _type);
}

#اگر_ختم
