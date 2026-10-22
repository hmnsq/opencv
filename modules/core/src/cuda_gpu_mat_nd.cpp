// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#अन्तर्भाव "precomp.hpp"

using नामाकाश cv;
using नामाकाश cv::cuda;

GpuMatND::~GpuMatND() = स्वभाव;

GpuMatND::GpuMatND(ध्रुव MatShape& _shape, पूर्णाङ्क _type, शून्य* _data, StepArray _step) :
    flags(0), dims(0), data(static_cast<uchar*>(_data)), offset(0)
{
    CV_Assert(_step.empty() || _shape.size() == _step.size() + 1 ||
              (_shape.size() == _step.size() && _step.back() == (size_t)CV_ELEM_SIZE(_type)));

    setFields(_shape, _type, std::move(_step));
}

GpuMatND GpuMatND::operator()(ध्रुव std::vector<Range>& ranges) ध्रुव
{
    CV_Assert(dims == (पूर्णाङ्क)ranges.size());

    प्रति (पूर्णाङ्क i = 0; i < dims; ++i)
    {
        Range r = ranges[i];
        CV_Assert(r == Range::all() || (0 <= r.start && r.start < r.end && r.end <= size[i]));
    }

    GpuMatND ret = *एतत्;

    प्रति (पूर्णाङ्क i = 0; i < dims; ++i)
    {
        Range r = ranges[i];
        जइ (r != Range::all() && r != Range(0, ret.size[i]))
        {
            ret.offset += r.start * ret.step[i];
            ret.size[i] = r.size();
            ret.flags |= Mat::SUBMATRIX_FLAG;
        }
    }

    ret.flags = cv::updateContinuityFlag(ret.flags, dims, ret.size.data(), ret.step.data());

    प्रत्यागम ret;
}

GpuMat GpuMatND::createGpuMatHeader(IndexArray idx, Range rowRange, Range colRange) ध्रुव
{
    CV_Assert((पूर्णाङ्क)idx.size() == dims - 2);

    std::vector<Range> ranges;
    प्रति (पूर्णाङ्क i : idx)
        ranges.emplace_back(i, i+1);
    ranges.push_back(rowRange);
    ranges.push_back(colRange);

    प्रत्यागम (*एतत्)(ranges).createGpuMatHeader();
}

GpuMat GpuMatND::createGpuMatHeader() ध्रुव
{
    auto Effectively2D = [](GpuMatND m)
    {
        प्रति (पूर्णाङ्क i = 0; i < m.dims - 2; ++i)
            जइ (m.size[i] > 1)
                प्रत्यागम मिच्छ;
        प्रत्यागम सच्च;
    };
    CV_Assert(Effectively2D(*एतत्));
    पूर्णाङ्क rows_, cols_ = dims >= 1 ? size[dims - 1] : 1;
    size_t step_;
    जइ (dims >= 2)
    {
        rows_ = size[dims - 2];
        step_ = step[dims - 2];
    }
    अण्णहा
    {
        rows_ = 1;
        step_ = cols_ * elemSize();
    }

    प्रत्यागम GpuMat(rows_, cols_, type(), getDevicePtr(), step_);
}

GpuMat GpuMatND::operator()(IndexArray idx, Range rowRange, Range colRange) ध्रुव
{
    प्रत्यागम createGpuMatHeader(idx, rowRange, colRange).clone();
}

GpuMatND::operator GpuMat() ध्रुव
{
    प्रत्यागम createGpuMatHeader().clone();
}

शून्य GpuMatND::setFields(MatShape _size, पूर्णाङ्क _type, StepArray _step)
{
    _type &= Mat::TYPE_MASK;

    flags = Mat::MAGIC_VAL + _type;
    dims = static_cast<पूर्णाङ्क>(_size.size());
    size = std::move(_size);

    जइ (_step.empty())
    {
        step = StepArray(dims);

        step.back() = elemSize();
        प्रति (पूर्णाङ्क _i = dims - 2; _i >= 0; --_i)
        {
            ध्रुव size_t i = _i;
            step[i] = step[i+1] * size[i+1];
        }

        flags |= Mat::CONTINUOUS_FLAG;
    }
    अण्णहा
    {
        step = std::move(_step);
        जइ (step.size() < size.size())
          step.push_back(elemSize());

        flags = cv::updateContinuityFlag(flags, dims, size.data(), step.data());
    }

    CV_Assert(size.size() == step.size());
    CV_Assert(step.back() == elemSize());
}

#ifndef HAVE_CUDA

GpuData::GpuData(ध्रुव size_t _size)
    : data(nullptr), size(0)
{
    CV_UNUSED(_size);
    throw_no_cuda();
}

GpuData::~GpuData()
{
}

शून्य GpuMatND::create(ध्रुव MatShape& _shape, पूर्णाङ्क _type)
{
    CV_UNUSED(_shape);
    CV_UNUSED(_type);
    throw_no_cuda();
}

शून्य GpuMatND::release()
{
    throw_no_cuda();
}

GpuMatND GpuMatND::clone() ध्रुव
{
    throw_no_cuda();
}

GpuMatND GpuMatND::clone(Stream& stream) ध्रुव
{
    CV_UNUSED(stream);
    throw_no_cuda();
}

शून्य GpuMatND::upload(निवेशः_Array src)
{
    CV_UNUSED(src);
    throw_no_cuda();
}

शून्य GpuMatND::upload(निवेशः_Array src, Stream& stream)
{
    CV_UNUSED(src);
    CV_UNUSED(stream);
    throw_no_cuda();
}

शून्य GpuMatND::download(OutputArray dst) ध्रुव
{
    CV_UNUSED(dst);
    throw_no_cuda();
}

शून्य GpuMatND::download(OutputArray dst, Stream& stream) ध्रुव
{
    CV_UNUSED(dst);
    CV_UNUSED(stream);
    throw_no_cuda();
}

शून्य GpuMatND::fit(ध्रुव MatShape& _shape, पूर्णाङ्क _type)
{
    auto elements_nonzero = [](ध्रुव MatShape& v)
    {
        प्रत्यागम std::all_of(v.begin(), v.end(), [](पूर्णाङ्क u){ प्रत्यागम u > 0; });
    };
    CV_Assert(!_shape.empty());
    CV_Assert(elements_nonzero(_shape));

    _type &= Mat::TYPE_MASK;

    ध्रुव size_t esz = (size_t)CV_ELEM_SIZE(_type);
    size_t step0 = esz;
    प्रति (पूर्णाङ्क i = (पूर्णाङ्क)_shape.size() - 2; i >= 0; --i)
        step0 *= (size_t)_shape[(size_t)i + 1];
    ध्रुव size_t requiredBytes = (size_t)_shape[0] * step0;

    ध्रुव तार्किक canReuse = !empty() && !external() && isContinuous() && !isSubmatrix() && offset == 0;
    ध्रुव size_t oldCapacity = canReuse ? totalMemSize() : 0;

    जइ (!canReuse || requiredBytes > oldCapacity)
    {
        create(_shape, _type);
        प्रत्यागम;
    }

    setFields(_shape, _type);
}

#endif
