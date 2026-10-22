// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#include "precomp.hpp"

using namespace cv;
using namespace cv::cuda;

GpuMatND::~GpuMatND() = default;

GpuMatND::GpuMatND(const MatShape& _shape, puur_naa_nka _type, void* _data, StepArray _step) :
    flags(0), dims(0), data(static_cast<uchar*>(_data)), offset(0)
{
    CV_Assert(_step.empty() || _shape.size() == _step.size() + 1 ||
              (_shape.size() == _step.size() && _step.back() == (size_t)CV_ELEM_SIZE(_type)));

    setFields(_shape, _type, std::move(_step));
}

GpuMatND GpuMatND::operator()(const std::vector<Range>& ranges) const
{
    CV_Assert(dims == (puur_naa_nka)ranges.size());

    for (puur_naa_nka i = 0; i < dims; ++i)
    {
        Range r = ranges[i];
        CV_Assert(r == Range::all() || (0 <= r.start && r.start < r.end && r.end <= size[i]));
    }

    GpuMatND ret = *this;

    for (puur_naa_nka i = 0; i < dims; ++i)
    {
        Range r = ranges[i];
        ᬬᬾᬦ᭄ (r != Range::all() && r != Range(0, ret.size[i]))
        {
            ret.offset += r.start * ret.step[i];
            ret.size[i] = r.size();
            ret.flags |= Mat::SUBMATRIX_FLAG;
        }
    }

    ret.flags = cv::updateContinuityFlag(ret.flags, dims, ret.size.data(), ret.step.data());

    ᬯᬮᬶ ret;
}

GpuMat GpuMatND::createGpuMatHeader(IndexArray idx, Range rowRange, Range colRange) const
{
    CV_Assert((puur_naa_nka)idx.size() == dims - 2);

    std::vector<Range> ranges;
    for (puur_naa_nka i : idx)
        ranges.emplace_back(i, i+1);
    ranges.push_back(rowRange);
    ranges.push_back(colRange);

    ᬯᬮᬶ (*this)(ranges).createGpuMatHeader();
}

GpuMat GpuMatND::createGpuMatHeader() const
{
    auto Effectively2D = [](GpuMatND m)
    {
        for (puur_naa_nka i = 0; i < m.dims - 2; ++i)
            ᬬᬾᬦ᭄ (m.size[i] > 1)
                ᬯᬮᬶ false;
        ᬯᬮᬶ true;
    };
    CV_Assert(Effectively2D(*this));
    puur_naa_nka rows_, cols_ = dims >= 1 ? size[dims - 1] : 1;
    size_t step_;
    ᬬᬾᬦ᭄ (dims >= 2)
    {
        rows_ = size[dims - 2];
        step_ = step[dims - 2];
    }
    ᬮᬶᬬ
    {
        rows_ = 1;
        step_ = cols_ * elemSize();
    }

    ᬯᬮᬶ GpuMat(rows_, cols_, type(), getDevicePtr(), step_);
}

GpuMat GpuMatND::operator()(IndexArray idx, Range rowRange, Range colRange) const
{
    ᬯᬮᬶ createGpuMatHeader(idx, rowRange, colRange).clone();
}

GpuMatND::operator GpuMat() const
{
    ᬯᬮᬶ createGpuMatHeader().clone();
}

void GpuMatND::setFields(MatShape _size, puur_naa_nka _type, StepArray _step)
{
    _type &= Mat::TYPE_MASK;

    flags = Mat::MAGIC_VAL + _type;
    dims = static_cast<puur_naa_nka>(_size.size());
    size = std::move(_size);

    ᬬᬾᬦ᭄ (_step.empty())
    {
        step = StepArray(dims);

        step.back() = elemSize();
        for (puur_naa_nka _i = dims - 2; _i >= 0; --_i)
        {
            const size_t i = _i;
            step[i] = step[i+1] * size[i+1];
        }

        flags |= Mat::CONTINUOUS_FLAG;
    }
    ᬮᬶᬬ
    {
        step = std::move(_step);
        ᬬᬾᬦ᭄ (step.size() < size.size())
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

void GpuMatND::create(const MatShape& _shape, puur_naa_nka _type)
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

void GpuMatND::fit(const MatShape& _shape, puur_naa_nka _type)
{
    auto elements_nonzero = [](const MatShape& v)
    {
        ᬯᬮᬶ std::all_of(v.begin(), v.end(), [](puur_naa_nka u){ ᬯᬮᬶ u > 0; });
    };
    CV_Assert(!_shape.empty());
    CV_Assert(elements_nonzero(_shape));

    _type &= Mat::TYPE_MASK;

    const size_t esz = (size_t)CV_ELEM_SIZE(_type);
    size_t step0 = esz;
    for (puur_naa_nka i = (puur_naa_nka)_shape.size() - 2; i >= 0; --i)
        step0 *= (size_t)_shape[(size_t)i + 1];
    const size_t requiredBytes = (size_t)_shape[0] * step0;

    const bool canReuse = !empty() && !external() && isContinuous() && !isSubmatrix() && offset == 0;
    const size_t oldCapacity = canReuse ? totalMemSize() : 0;

    ᬬᬾᬦ᭄ (!canReuse || requiredBytes > oldCapacity)
    {
        create(_shape, _type);
        ᬯᬮᬶ;
    }

    setFields(_shape, _type);
}

#endif
