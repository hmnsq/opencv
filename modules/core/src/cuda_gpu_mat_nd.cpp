// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#include "precomp.hpp"

using namespace cv;
using namespace cv::cuda;

GpuMatND::~GpuMatND() = default;

GpuMatND::GpuMatND(const MatShape& _shape, ចំនួនគត់ _type, void* _data, StepArray _step) :
    flags(0), dims(0), data(static_cast<uchar*>(_data)), offset(0)
{
    CV_Assert(_step.empty() || _shape.size() == _step.size() + 1 ||
              (_shape.size() == _step.size() && _step.back() == (size_t)CV_ELEM_SIZE(_type)));

    setFields(_shape, _type, std::move(_step));
}

GpuMatND GpuMatND::operator()(const std::vector<Range>& ranges) const
{
    CV_Assert(dims == (ចំនួនគត់)ranges.size());

    សម្រាប់ (ចំនួនគត់ i = 0; i < dims; ++i)
    {
        Range r = ranges[i];
        CV_Assert(r == Range::all() || (0 <= r.start && r.start < r.end && r.end <= size[i]));
    }

    GpuMatND ret = *this;

    សម្រាប់ (ចំនួនគត់ i = 0; i < dims; ++i)
    {
        Range r = ranges[i];
        ប្រសិនបើ (r != Range::all() && r != Range(0, ret.size[i]))
        {
            ret.offset += r.start * ret.step[i];
            ret.size[i] = r.size();
            ret.flags |= Mat::SUBMATRIX_FLAG;
        }
    }

    ret.flags = cv::updateContinuityFlag(ret.flags, dims, ret.size.data(), ret.step.data());

    ត្រឡប់ ret;
}

GpuMat GpuMatND::createGpuMatHeader(IndexArray idx, Range rowRange, Range colRange) const
{
    CV_Assert((ចំនួនគត់)idx.size() == dims - 2);

    std::vector<Range> ranges;
    សម្រាប់ (ចំនួនគត់ i : idx)
        ranges.emplace_back(i, i+1);
    ranges.push_back(rowRange);
    ranges.push_back(colRange);

    ត្រឡប់ (*this)(ranges).createGpuMatHeader();
}

GpuMat GpuMatND::createGpuMatHeader() const
{
    auto Effectively2D = [](GpuMatND m)
    {
        សម្រាប់ (ចំនួនគត់ i = 0; i < m.dims - 2; ++i)
            ប្រសិនបើ (m.size[i] > 1)
                ត្រឡប់ false;
        ត្រឡប់ true;
    };
    CV_Assert(Effectively2D(*this));
    ចំនួនគត់ rows_, cols_ = dims >= 1 ? size[dims - 1] : 1;
    size_t step_;
    ប្រសិនបើ (dims >= 2)
    {
        rows_ = size[dims - 2];
        step_ = step[dims - 2];
    }
    បើមិនដូច្នេះ
    {
        rows_ = 1;
        step_ = cols_ * elemSize();
    }

    ត្រឡប់ GpuMat(rows_, cols_, type(), getDevicePtr(), step_);
}

GpuMat GpuMatND::operator()(IndexArray idx, Range rowRange, Range colRange) const
{
    ត្រឡប់ createGpuMatHeader(idx, rowRange, colRange).clone();
}

GpuMatND::operator GpuMat() const
{
    ត្រឡប់ createGpuMatHeader().clone();
}

void GpuMatND::setFields(MatShape _size, ចំនួនគត់ _type, StepArray _step)
{
    _type &= Mat::TYPE_MASK;

    flags = Mat::MAGIC_VAL + _type;
    dims = static_cast<ចំនួនគត់>(_size.size());
    size = std::move(_size);

    ប្រសិនបើ (_step.empty())
    {
        step = StepArray(dims);

        step.back() = elemSize();
        សម្រាប់ (ចំនួនគត់ _i = dims - 2; _i >= 0; --_i)
        {
            const size_t i = _i;
            step[i] = step[i+1] * size[i+1];
        }

        flags |= Mat::CONTINUOUS_FLAG;
    }
    បើមិនដូច្នេះ
    {
        step = std::move(_step);
        ប្រសិនបើ (step.size() < size.size())
          step.push_back(elemSize());

        flags = cv::updateContinuityFlag(flags, dims, size.data(), step.data());
    }

    CV_Assert(size.size() == step.size());
    CV_Assert(step.back() == elemSize());
}

#ifndef HAVE_CUDA

GpuData::GpuData(const size_t _size)
    : data(nullptr), size(0)
{
    CV_UNUSED(_size);
    throw_no_cuda();
}

GpuData::~GpuData()
{
}

void GpuMatND::create(const MatShape& _shape, ចំនួនគត់ _type)
{
    CV_UNUSED(_shape);
    CV_UNUSED(_type);
    throw_no_cuda();
}

void GpuMatND::release()
{
    throw_no_cuda();
}

GpuMatND GpuMatND::clone() const
{
    throw_no_cuda();
}

GpuMatND GpuMatND::clone(Stream& stream) const
{
    CV_UNUSED(stream);
    throw_no_cuda();
}

void GpuMatND::upload(InputArray src)
{
    CV_UNUSED(src);
    throw_no_cuda();
}

void GpuMatND::upload(InputArray src, Stream& stream)
{
    CV_UNUSED(src);
    CV_UNUSED(stream);
    throw_no_cuda();
}

void GpuMatND::download(OutputArray dst) const
{
    CV_UNUSED(dst);
    throw_no_cuda();
}

void GpuMatND::download(OutputArray dst, Stream& stream) const
{
    CV_UNUSED(dst);
    CV_UNUSED(stream);
    throw_no_cuda();
}

void GpuMatND::fit(const MatShape& _shape, ចំនួនគត់ _type)
{
    auto elements_nonzero = [](const MatShape& v)
    {
        ត្រឡប់ std::all_of(v.begin(), v.end(), [](ចំនួនគត់ u){ ត្រឡប់ u > 0; });
    };
    CV_Assert(!_shape.empty());
    CV_Assert(elements_nonzero(_shape));

    _type &= Mat::TYPE_MASK;

    const size_t esz = (size_t)CV_ELEM_SIZE(_type);
    size_t step0 = esz;
    សម្រាប់ (ចំនួនគត់ i = (ចំនួនគត់)_shape.size() - 2; i >= 0; --i)
        step0 *= (size_t)_shape[(size_t)i + 1];
    const size_t requiredBytes = (size_t)_shape[0] * step0;

    const bool canReuse = !empty() && !external() && isContinuous() && !isSubmatrix() && offset == 0;
    const size_t oldCapacity = canReuse ? totalMemSize() : 0;

    ប្រសិនបើ (!canReuse || requiredBytes > oldCapacity)
    {
        create(_shape, _type);
        ត្រឡប់;
    }

    setFields(_shape, _type);
}

#endif
