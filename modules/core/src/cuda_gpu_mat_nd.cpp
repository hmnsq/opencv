// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#شامل "precomp.hpp"

using فضاینام cv;
using فضاینام cv::cuda;

GpuMatND::~GpuMatND() = پیشفرض;

GpuMatND::GpuMatND(ثابت MatShape& _shape, صحیح _type, تهی* _data, StepArray _step) :
    flags(0), dims(0), data(static_cast<uchar*>(_data)), offset(0)
{
    CV_Assert(_step.empty() || _shape.size() == _step.size() + 1 ||
              (_shape.size() == _step.size() && _step.back() == (size_t)CV_ELEM_SIZE(_type)));

    setFields(_shape, _type, std::move(_step));
}

GpuMatND GpuMatND::operator()(ثابت std::vector<Range>& ranges) ثابت
{
    CV_Assert(dims == (صحیح)ranges.size());

    برای (صحیح i = 0; i < dims; ++i)
    {
        Range r = ranges[i];
        CV_Assert(r == Range::all() || (0 <= r.start && r.start < r.end && r.end <= size[i]));
    }

    GpuMatND ret = *این;

    برای (صحیح i = 0; i < dims; ++i)
    {
        Range r = ranges[i];
        اگر (r != Range::all() && r != Range(0, ret.size[i]))
        {
            ret.offset += r.start * ret.step[i];
            ret.size[i] = r.size();
            ret.flags |= Mat::SUBMATRIX_FLAG;
        }
    }

    ret.flags = cv::updateContinuityFlag(ret.flags, dims, ret.size.data(), ret.step.data());

    بازگشت ret;
}

GpuMat GpuMatND::createGpuMatHeader(IndexArray idx, Range rowRange, Range colRange) ثابت
{
    CV_Assert((صحیح)idx.size() == dims - 2);

    std::vector<Range> ranges;
    برای (صحیح i : idx)
        ranges.emplace_back(i, i+1);
    ranges.push_back(rowRange);
    ranges.push_back(colRange);

    بازگشت (*این)(ranges).createGpuMatHeader();
}

GpuMat GpuMatND::createGpuMatHeader() ثابت
{
    auto Effectively2D = [](GpuMatND m)
    {
        برای (صحیح i = 0; i < m.dims - 2; ++i)
            اگر (m.size[i] > 1)
                بازگشت نادرست;
        بازگشت درست;
    };
    CV_Assert(Effectively2D(*این));
    صحیح rows_, cols_ = dims >= 1 ? size[dims - 1] : 1;
    size_t step_;
    اگر (dims >= 2)
    {
        rows_ = size[dims - 2];
        step_ = step[dims - 2];
    }
    وگرنه
    {
        rows_ = 1;
        step_ = cols_ * elemSize();
    }

    بازگشت GpuMat(rows_, cols_, type(), getDevicePtr(), step_);
}

GpuMat GpuMatND::operator()(IndexArray idx, Range rowRange, Range colRange) ثابت
{
    بازگشت createGpuMatHeader(idx, rowRange, colRange).clone();
}

GpuMatND::operator GpuMat() ثابت
{
    بازگشت createGpuMatHeader().clone();
}

تهی GpuMatND::setFields(MatShape _size, صحیح _type, StepArray _step)
{
    _type &= Mat::TYPE_MASK;

    flags = Mat::MAGIC_VAL + _type;
    dims = static_cast<صحیح>(_size.size());
    size = std::move(_size);

    اگر (_step.empty())
    {
        step = StepArray(dims);

        step.back() = elemSize();
        برای (صحیح _i = dims - 2; _i >= 0; --_i)
        {
            ثابت size_t i = _i;
            step[i] = step[i+1] * size[i+1];
        }

        flags |= Mat::CONTINUOUS_FLAG;
    }
    وگرنه
    {
        step = std::move(_step);
        اگر (step.size() < size.size())
          step.push_back(elemSize());

        flags = cv::updateContinuityFlag(flags, dims, size.data(), step.data());
    }

    CV_Assert(size.size() == step.size());
    CV_Assert(step.back() == elemSize());
}

#ifndef HAVE_CUDA

GpuData::GpuData(ثابت size_t _size)
    : data(nullptr), size(0)
{
    CV_UNUSED(_size);
    throw_no_cuda();
}

GpuData::~GpuData()
{
}

تهی GpuMatND::create(ثابت MatShape& _shape, صحیح _type)
{
    CV_UNUSED(_shape);
    CV_UNUSED(_type);
    throw_no_cuda();
}

تهی GpuMatND::release()
{
    throw_no_cuda();
}

GpuMatND GpuMatND::clone() ثابت
{
    throw_no_cuda();
}

GpuMatND GpuMatND::clone(Stream& stream) ثابت
{
    CV_UNUSED(stream);
    throw_no_cuda();
}

تهی GpuMatND::upload(درون_داد_Array src)
{
    CV_UNUSED(src);
    throw_no_cuda();
}

تهی GpuMatND::upload(درون_داد_Array src, Stream& stream)
{
    CV_UNUSED(src);
    CV_UNUSED(stream);
    throw_no_cuda();
}

تهی GpuMatND::download(OutputArray dst) ثابت
{
    CV_UNUSED(dst);
    throw_no_cuda();
}

تهی GpuMatND::download(OutputArray dst, Stream& stream) ثابت
{
    CV_UNUSED(dst);
    CV_UNUSED(stream);
    throw_no_cuda();
}

تهی GpuMatND::fit(ثابت MatShape& _shape, صحیح _type)
{
    auto elements_nonzero = [](ثابت MatShape& v)
    {
        بازگشت std::all_of(v.begin(), v.end(), [](صحیح u){ بازگشت u > 0; });
    };
    CV_Assert(!_shape.empty());
    CV_Assert(elements_nonzero(_shape));

    _type &= Mat::TYPE_MASK;

    ثابت size_t esz = (size_t)CV_ELEM_SIZE(_type);
    size_t step0 = esz;
    برای (صحیح i = (صحیح)_shape.size() - 2; i >= 0; --i)
        step0 *= (size_t)_shape[(size_t)i + 1];
    ثابت size_t requiredBytes = (size_t)_shape[0] * step0;

    ثابت منطقی canReuse = !empty() && !external() && isContinuous() && !isSubmatrix() && offset == 0;
    ثابت size_t oldCapacity = canReuse ? totalMemSize() : 0;

    اگر (!canReuse || requiredBytes > oldCapacity)
    {
        create(_shape, _type);
        بازگشت;
    }

    setFields(_shape, _type);
}

#endif
