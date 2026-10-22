// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#அந்தர்பாவ "precomp.hpp"

using நாமாகாஶ cv;
using நாமாகாஶ cv::cuda;

GpuMatND::~GpuMatND() = இயல்பு;

GpuMatND::GpuMatND(மாறிலி MatShape& _shape, முழு _type, வெறுமை* _data, StepArray _step) :
    flags(0), dims(0), data(static_cast<uchar*>(_data)), offset(0)
{
    CV_Assert(_step.empty() || _shape.size() == _step.size() + 1 ||
              (_shape.size() == _step.size() && _step.back() == (size_t)CV_ELEM_SIZE(_type)));

    setFields(_shape, _type, std::move(_step));
}

GpuMatND GpuMatND::operator()(மாறிலி std::vector<Range>& ranges) மாறிலி
{
    CV_Assert(dims == (முழு)ranges.size());

    க்கு (முழு i = 0; i < dims; ++i)
    {
        Range r = ranges[i];
        CV_Assert(r == Range::all() || (0 <= r.start && r.start < r.end && r.end <= size[i]));
    }

    GpuMatND ret = *இது;

    க்கு (முழு i = 0; i < dims; ++i)
    {
        Range r = ranges[i];
        எனில் (r != Range::all() && r != Range(0, ret.size[i]))
        {
            ret.offset += r.start * ret.step[i];
            ret.size[i] = r.size();
            ret.flags |= Mat::SUBMATRIX_FLAG;
        }
    }

    ret.flags = cv::updateContinuityFlag(ret.flags, dims, ret.size.data(), ret.step.data());

    திரும்பு ret;
}

GpuMat GpuMatND::createGpuMatHeader(IndexArray idx, Range rowRange, Range colRange) மாறிலி
{
    CV_Assert((முழு)idx.size() == dims - 2);

    std::vector<Range> ranges;
    க்கு (முழு i : idx)
        ranges.emplace_back(i, i+1);
    ranges.push_back(rowRange);
    ranges.push_back(colRange);

    திரும்பு (*இது)(ranges).createGpuMatHeader();
}

GpuMat GpuMatND::createGpuMatHeader() மாறிலி
{
    auto Effectively2D = [](GpuMatND m)
    {
        க்கு (முழு i = 0; i < m.dims - 2; ++i)
            எனில் (m.size[i] > 1)
                திரும்பு பொய்;
        திரும்பு உண்மை;
    };
    CV_Assert(Effectively2D(*இது));
    முழு rows_, cols_ = dims >= 1 ? size[dims - 1] : 1;
    size_t step_;
    எனில் (dims >= 2)
    {
        rows_ = size[dims - 2];
        step_ = step[dims - 2];
    }
    இல்லையெனில்
    {
        rows_ = 1;
        step_ = cols_ * elemSize();
    }

    திரும்பு GpuMat(rows_, cols_, type(), getDevicePtr(), step_);
}

GpuMat GpuMatND::operator()(IndexArray idx, Range rowRange, Range colRange) மாறிலி
{
    திரும்பு createGpuMatHeader(idx, rowRange, colRange).clone();
}

GpuMatND::operator GpuMat() மாறிலி
{
    திரும்பு createGpuMatHeader().clone();
}

வெறுமை GpuMatND::setFields(MatShape _size, முழு _type, StepArray _step)
{
    _type &= Mat::TYPE_MASK;

    flags = Mat::MAGIC_VAL + _type;
    dims = static_cast<முழு>(_size.size());
    size = std::move(_size);

    எனில் (_step.empty())
    {
        step = StepArray(dims);

        step.back() = elemSize();
        க்கு (முழு _i = dims - 2; _i >= 0; --_i)
        {
            மாறிலி size_t i = _i;
            step[i] = step[i+1] * size[i+1];
        }

        flags |= Mat::CONTINUOUS_FLAG;
    }
    இல்லையெனில்
    {
        step = std::move(_step);
        எனில் (step.size() < size.size())
          step.push_back(elemSize());

        flags = cv::updateContinuityFlag(flags, dims, size.data(), step.data());
    }

    CV_Assert(size.size() == step.size());
    CV_Assert(step.back() == elemSize());
}

#ifndef HAVE_CUDA

GpuData::GpuData(மாறிலி size_t _size)
    : data(nullptr), size(0)
{
    CV_UNUSED(_size);
    throw_no_cuda();
}

GpuData::~GpuData()
{
}

வெறுமை GpuMatND::create(மாறிலி MatShape& _shape, முழு _type)
{
    CV_UNUSED(_shape);
    CV_UNUSED(_type);
    throw_no_cuda();
}

வெறுமை GpuMatND::release()
{
    throw_no_cuda();
}

GpuMatND GpuMatND::clone() மாறிலி
{
    throw_no_cuda();
}

GpuMatND GpuMatND::clone(Stream& stream) மாறிலி
{
    CV_UNUSED(stream);
    throw_no_cuda();
}

வெறுமை GpuMatND::upload(உள்ளீடு_Array src)
{
    CV_UNUSED(src);
    throw_no_cuda();
}

வெறுமை GpuMatND::upload(உள்ளீடு_Array src, Stream& stream)
{
    CV_UNUSED(src);
    CV_UNUSED(stream);
    throw_no_cuda();
}

வெறுமை GpuMatND::download(OutputArray dst) மாறிலி
{
    CV_UNUSED(dst);
    throw_no_cuda();
}

வெறுமை GpuMatND::download(OutputArray dst, Stream& stream) மாறிலி
{
    CV_UNUSED(dst);
    CV_UNUSED(stream);
    throw_no_cuda();
}

வெறுமை GpuMatND::fit(மாறிலி MatShape& _shape, முழு _type)
{
    auto elements_nonzero = [](மாறிலி MatShape& v)
    {
        திரும்பு std::all_of(v.begin(), v.end(), [](முழு u){ திரும்பு u > 0; });
    };
    CV_Assert(!_shape.empty());
    CV_Assert(elements_nonzero(_shape));

    _type &= Mat::TYPE_MASK;

    மாறிலி size_t esz = (size_t)CV_ELEM_SIZE(_type);
    size_t step0 = esz;
    க்கு (முழு i = (முழு)_shape.size() - 2; i >= 0; --i)
        step0 *= (size_t)_shape[(size_t)i + 1];
    மாறிலி size_t requiredBytes = (size_t)_shape[0] * step0;

    மாறிலி தருக்க canReuse = !empty() && !external() && isContinuous() && !isSubmatrix() && offset == 0;
    மாறிலி size_t oldCapacity = canReuse ? totalMemSize() : 0;

    எனில் (!canReuse || requiredBytes > oldCapacity)
    {
        create(_shape, _type);
        திரும்பு;
    }

    setFields(_shape, _type);
}

#endif
