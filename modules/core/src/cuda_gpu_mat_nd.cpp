// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ضمن "precomp.hpp"

using نطاق cv;
using نطاق cv::cuda;

GpuMatND::~GpuMatND() = افتراضي;

GpuMatND::GpuMatND(ثابت MatShape& _shape, صحيح _type, فراغ* _data, StepArray _step) :
    flags(0), dims(0), data(static_cast<uchar*>(_data)), offset(0)
{
    CV_Assert(_step.empty() || _shape.size() == _step.size() + 1 ||
              (_shape.size() == _step.size() && _step.back() == (size_t)CV_ELEM_SIZE(_type)));

    setFields(_shape, _type, std::move(_step));
}

GpuMatND GpuMatND::operator()(ثابت std::vector<Range>& ranges) ثابت
{
    CV_Assert(dims == (صحيح)ranges.size());

    لكل (صحيح i = 0; i < dims; ++i)
    {
        Range r = ranges[i];
        CV_Assert(r == Range::all() || (0 <= r.start && r.start < r.end && r.end <= size[i]));
    }

    GpuMatND ret = *هذا;

    لكل (صحيح i = 0; i < dims; ++i)
    {
        Range r = ranges[i];
        إذا (r != Range::all() && r != Range(0, ret.size[i]))
        {
            ret.offset += r.start * ret.step[i];
            ret.size[i] = r.size();
            ret.flags |= Mat::SUBMATRIX_FLAG;
        }
    }

    ret.flags = cv::updateContinuityFlag(ret.flags, dims, ret.size.data(), ret.step.data());

    أرجع ret;
}

GpuMat GpuMatND::createGpuMatHeader(IndexArray idx, Range rowRange, Range colRange) ثابت
{
    CV_Assert((صحيح)idx.size() == dims - 2);

    std::vector<Range> ranges;
    لكل (صحيح i : idx)
        ranges.emplace_back(i, i+1);
    ranges.push_back(rowRange);
    ranges.push_back(colRange);

    أرجع (*هذا)(ranges).createGpuMatHeader();
}

GpuMat GpuMatND::createGpuMatHeader() ثابت
{
    auto Effectively2D = [](GpuMatND m)
    {
        لكل (صحيح i = 0; i < m.dims - 2; ++i)
            إذا (m.size[i] > 1)
                أرجع خطأ;
        أرجع true;
    };
    CV_Assert(Effectively2D(*هذا));
    صحيح rows_, cols_ = dims >= 1 ? size[dims - 1] : 1;
    size_t step_;
    إذا (dims >= 2)
    {
        rows_ = size[dims - 2];
        step_ = step[dims - 2];
    }
    وإلا
    {
        rows_ = 1;
        step_ = cols_ * elemSize();
    }

    أرجع GpuMat(rows_, cols_, type(), getDevicePtr(), step_);
}

GpuMat GpuMatND::operator()(IndexArray idx, Range rowRange, Range colRange) ثابت
{
    أرجع createGpuMatHeader(idx, rowRange, colRange).clone();
}

GpuMatND::operator GpuMat() ثابت
{
    أرجع createGpuMatHeader().clone();
}

فراغ GpuMatND::setFields(MatShape _size, صحيح _type, StepArray _step)
{
    _type &= Mat::TYPE_MASK;

    flags = Mat::MAGIC_VAL + _type;
    dims = static_cast<صحيح>(_size.size());
    size = std::move(_size);

    إذا (_step.empty())
    {
        step = StepArray(dims);

        step.back() = elemSize();
        لكل (صحيح _i = dims - 2; _i >= 0; --_i)
        {
            ثابت size_t i = _i;
            step[i] = step[i+1] * size[i+1];
        }

        flags |= Mat::CONTINUOUS_FLAG;
    }
    وإلا
    {
        step = std::move(_step);
        إذا (step.size() < size.size())
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

فراغ GpuMatND::create(ثابت MatShape& _shape, صحيح _type)
{
    CV_UNUSED(_shape);
    CV_UNUSED(_type);
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

GpuMatND GpuMatND::clone(Stream& stream) ثابت
{
    CV_UNUSED(stream);
    throw_no_cuda();
}

فراغ GpuMatND::upload(كىرگۈزۈش_Array src)
{
    CV_UNUSED(src);
    throw_no_cuda();
}

فراغ GpuMatND::upload(كىرگۈزۈش_Array src, Stream& stream)
{
    CV_UNUSED(src);
    CV_UNUSED(stream);
    throw_no_cuda();
}

فراغ GpuMatND::download(OutputArray dst) ثابت
{
    CV_UNUSED(dst);
    throw_no_cuda();
}

فراغ GpuMatND::download(OutputArray dst, Stream& stream) ثابت
{
    CV_UNUSED(dst);
    CV_UNUSED(stream);
    throw_no_cuda();
}

فراغ GpuMatND::fit(ثابت MatShape& _shape, صحيح _type)
{
    auto elements_nonzero = [](ثابت MatShape& v)
    {
        أرجع std::all_of(v.begin(), v.end(), [](صحيح u){ أرجع u > 0; });
    };
    CV_Assert(!_shape.empty());
    CV_Assert(elements_nonzero(_shape));

    _type &= Mat::TYPE_MASK;

    ثابت size_t esz = (size_t)CV_ELEM_SIZE(_type);
    size_t step0 = esz;
    لكل (صحيح i = (صحيح)_shape.size() - 2; i >= 0; --i)
        step0 *= (size_t)_shape[(size_t)i + 1];
    ثابت size_t requiredBytes = (size_t)_shape[0] * step0;

    ثابت منطقي canReuse = !empty() && !external() && isContinuous() && !isSubmatrix() && offset == 0;
    ثابت size_t oldCapacity = canReuse ? totalMemSize() : 0;

    إذا (!canReuse || requiredBytes > oldCapacity)
    {
        create(_shape, _type);
        أرجع;
    }

    setFields(_shape, _type);
}

#endif
