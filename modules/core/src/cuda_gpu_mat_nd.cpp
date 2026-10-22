// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#অন্তর্ভাব "precomp.hpp"

using নামাকাশ cv;
using নামাকাশ cv::cuda;

GpuMatND::~GpuMatND() = স্বভাব;

GpuMatND::GpuMatND(ধ্রুব MatShape& _shape, পূর্ণাঙ্ক _type, শূন্য* _data, StepArray _step) :
    flags(0), dims(0), data(static_cast<uchar*>(_data)), offset(0)
{
    CV_Assert(_step.empty() || _shape.size() == _step.size() + 1 ||
              (_shape.size() == _step.size() && _step.back() == (size_t)CV_ELEM_SIZE(_type)));

    setFields(_shape, _type, std::move(_step));
}

GpuMatND GpuMatND::operator()(ধ্রুব std::vector<Range>& ranges) ধ্রুব
{
    CV_Assert(dims == (পূর্ণাঙ্ক)ranges.size());

    প্রতি (পূর্ণাঙ্ক i = 0; i < dims; ++i)
    {
        Range r = ranges[i];
        CV_Assert(r == Range::all() || (0 <= r.start && r.start < r.end && r.end <= size[i]));
    }

    GpuMatND ret = *এতত্;

    প্রতি (পূর্ণাঙ্ক i = 0; i < dims; ++i)
    {
        Range r = ranges[i];
        যদি (r != Range::all() && r != Range(0, ret.size[i]))
        {
            ret.offset += r.start * ret.step[i];
            ret.size[i] = r.size();
            ret.flags |= Mat::SUBMATRIX_FLAG;
        }
    }

    ret.flags = cv::updateContinuityFlag(ret.flags, dims, ret.size.data(), ret.step.data());

    প্রত্যাগম ret;
}

GpuMat GpuMatND::createGpuMatHeader(IndexArray idx, Range rowRange, Range colRange) ধ্রুব
{
    CV_Assert((পূর্ণাঙ্ক)idx.size() == dims - 2);

    std::vector<Range> ranges;
    প্রতি (পূর্ণাঙ্ক i : idx)
        ranges.emplace_back(i, i+1);
    ranges.push_back(rowRange);
    ranges.push_back(colRange);

    প্রত্যাগম (*এতত্)(ranges).createGpuMatHeader();
}

GpuMat GpuMatND::createGpuMatHeader() ধ্রুব
{
    auto Effectively2D = [](GpuMatND m)
    {
        প্রতি (পূর্ণাঙ্ক i = 0; i < m.dims - 2; ++i)
            যদি (m.size[i] > 1)
                প্রত্যাগম অসত্য;
        প্রত্যাগম সত্য;
    };
    CV_Assert(Effectively2D(*এতত্));
    পূর্ণাঙ্ক rows_, cols_ = dims >= 1 ? size[dims - 1] : 1;
    size_t step_;
    যদি (dims >= 2)
    {
        rows_ = size[dims - 2];
        step_ = step[dims - 2];
    }
    অন্যথা
    {
        rows_ = 1;
        step_ = cols_ * elemSize();
    }

    প্রত্যাগম GpuMat(rows_, cols_, type(), getDevicePtr(), step_);
}

GpuMat GpuMatND::operator()(IndexArray idx, Range rowRange, Range colRange) ধ্রুব
{
    প্রত্যাগম createGpuMatHeader(idx, rowRange, colRange).clone();
}

GpuMatND::operator GpuMat() ধ্রুব
{
    প্রত্যাগম createGpuMatHeader().clone();
}

শূন্য GpuMatND::setFields(MatShape _size, পূর্ণাঙ্ক _type, StepArray _step)
{
    _type &= Mat::TYPE_MASK;

    flags = Mat::MAGIC_VAL + _type;
    dims = static_cast<পূর্ণাঙ্ক>(_size.size());
    size = std::move(_size);

    যদি (_step.empty())
    {
        step = StepArray(dims);

        step.back() = elemSize();
        প্রতি (পূর্ণাঙ্ক _i = dims - 2; _i >= 0; --_i)
        {
            ধ্রুব size_t i = _i;
            step[i] = step[i+1] * size[i+1];
        }

        flags |= Mat::CONTINUOUS_FLAG;
    }
    অন্যথা
    {
        step = std::move(_step);
        যদি (step.size() < size.size())
          step.push_back(elemSize());

        flags = cv::updateContinuityFlag(flags, dims, size.data(), step.data());
    }

    CV_Assert(size.size() == step.size());
    CV_Assert(step.back() == elemSize());
}

#ifndef HAVE_CUDA

GpuData::GpuData(ধ্রুব size_t _size)
    : data(nullptr), size(0)
{
    CV_UNUSED(_size);
    throw_no_cuda();
}

GpuData::~GpuData()
{
}

শূন্য GpuMatND::create(ধ্রুব MatShape& _shape, পূর্ণাঙ্ক _type)
{
    CV_UNUSED(_shape);
    CV_UNUSED(_type);
    throw_no_cuda();
}

শূন্য GpuMatND::release()
{
    throw_no_cuda();
}

GpuMatND GpuMatND::clone() ধ্রুব
{
    throw_no_cuda();
}

GpuMatND GpuMatND::clone(Stream& stream) ধ্রুব
{
    CV_UNUSED(stream);
    throw_no_cuda();
}

শূন্য GpuMatND::upload(প্রবেশ_Array src)
{
    CV_UNUSED(src);
    throw_no_cuda();
}

শূন্য GpuMatND::upload(প্রবেশ_Array src, Stream& stream)
{
    CV_UNUSED(src);
    CV_UNUSED(stream);
    throw_no_cuda();
}

শূন্য GpuMatND::download(OutputArray dst) ধ্রুব
{
    CV_UNUSED(dst);
    throw_no_cuda();
}

শূন্য GpuMatND::download(OutputArray dst, Stream& stream) ধ্রুব
{
    CV_UNUSED(dst);
    CV_UNUSED(stream);
    throw_no_cuda();
}

শূন্য GpuMatND::fit(ধ্রুব MatShape& _shape, পূর্ণাঙ্ক _type)
{
    auto elements_nonzero = [](ধ্রুব MatShape& v)
    {
        প্রত্যাগম std::all_of(v.begin(), v.end(), [](পূর্ণাঙ্ক u){ প্রত্যাগম u > 0; });
    };
    CV_Assert(!_shape.empty());
    CV_Assert(elements_nonzero(_shape));

    _type &= Mat::TYPE_MASK;

    ধ্রুব size_t esz = (size_t)CV_ELEM_SIZE(_type);
    size_t step0 = esz;
    প্রতি (পূর্ণাঙ্ক i = (পূর্ণাঙ্ক)_shape.size() - 2; i >= 0; --i)
        step0 *= (size_t)_shape[(size_t)i + 1];
    ধ্রুব size_t requiredBytes = (size_t)_shape[0] * step0;

    ধ্রুব তার্কিক canReuse = !empty() && !external() && isContinuous() && !isSubmatrix() && offset == 0;
    ধ্রুব size_t oldCapacity = canReuse ? totalMemSize() : 0;

    যদি (!canReuse || requiredBytes > oldCapacity)
    {
        create(_shape, _type);
        প্রত্যাগম;
    }

    setFields(_shape, _type);
}

#endif
