// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ଅନ୍ତର୍ଭାଵ "precomp.hpp"

using ନାମାକାଶ cv;
using ନାମାକାଶ cv::cuda;

GpuMatND::~GpuMatND() = ପୂର୍ବନିର୍ଦ୍ଧାରିତ;

GpuMatND::GpuMatND(ଅଚଳ MatShape& _shape, ପୂର୍ଣ୍ଣସଂଖ୍ୟା _type, ଶୂନ୍ୟ* _data, StepArray _step) :
    flags(0), dims(0), data(static_cast<uchar*>(_data)), offset(0)
{
    CV_Assert(_step.empty() || _shape.size() == _step.size() + 1 ||
              (_shape.size() == _step.size() && _step.back() == (size_t)CV_ELEM_SIZE(_type)));

    setFields(_shape, _type, std::move(_step));
}

GpuMatND GpuMatND::operator()(ଅଚଳ std::vector<Range>& ranges) ଅଚଳ
{
    CV_Assert(dims == (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)ranges.size());

    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < dims; ++i)
    {
        Range r = ranges[i];
        CV_Assert(r == Range::all() || (0 <= r.start && r.start < r.end && r.end <= size[i]));
    }

    GpuMatND ret = *ଏହି;

    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < dims; ++i)
    {
        Range r = ranges[i];
        ଯଦି (r != Range::all() && r != Range(0, ret.size[i]))
        {
            ret.offset += r.start * ret.step[i];
            ret.size[i] = r.size();
            ret.flags |= Mat::SUBMATRIX_FLAG;
        }
    }

    ret.flags = cv::updateContinuityFlag(ret.flags, dims, ret.size.data(), ret.step.data());

    ଫେରନ୍ତୁ ret;
}

GpuMat GpuMatND::createGpuMatHeader(IndexArray idx, Range rowRange, Range colRange) ଅଚଳ
{
    CV_Assert((ପୂର୍ଣ୍ଣସଂଖ୍ୟା)idx.size() == dims - 2);

    std::vector<Range> ranges;
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i : idx)
        ranges.emplace_back(i, i+1);
    ranges.push_back(rowRange);
    ranges.push_back(colRange);

    ଫେରନ୍ତୁ (*ଏହି)(ranges).createGpuMatHeader();
}

GpuMat GpuMatND::createGpuMatHeader() ଅଚଳ
{
    auto Effectively2D = [](GpuMatND m)
    {
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < m.dims - 2; ++i)
            ଯଦି (m.size[i] > 1)
                ଫେରନ୍ତୁ ମିଥ୍ୟା;
        ଫେରନ୍ତୁ ସତ୍ୟ;
    };
    CV_Assert(Effectively2D(*ଏହି));
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା rows_, cols_ = dims >= 1 ? size[dims - 1] : 1;
    size_t step_;
    ଯଦି (dims >= 2)
    {
        rows_ = size[dims - 2];
        step_ = step[dims - 2];
    }
    ଅନ୍ୟଥା
    {
        rows_ = 1;
        step_ = cols_ * elemSize();
    }

    ଫେରନ୍ତୁ GpuMat(rows_, cols_, type(), getDevicePtr(), step_);
}

GpuMat GpuMatND::operator()(IndexArray idx, Range rowRange, Range colRange) ଅଚଳ
{
    ଫେରନ୍ତୁ createGpuMatHeader(idx, rowRange, colRange).clone();
}

GpuMatND::operator GpuMat() ଅଚଳ
{
    ଫେରନ୍ତୁ createGpuMatHeader().clone();
}

ଶୂନ୍ୟ GpuMatND::setFields(MatShape _size, ପୂର୍ଣ୍ଣସଂଖ୍ୟା _type, StepArray _step)
{
    _type &= Mat::TYPE_MASK;

    flags = Mat::MAGIC_VAL + _type;
    dims = static_cast<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(_size.size());
    size = std::move(_size);

    ଯଦି (_step.empty())
    {
        step = StepArray(dims);

        step.back() = elemSize();
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା _i = dims - 2; _i >= 0; --_i)
        {
            ଅଚଳ size_t i = _i;
            step[i] = step[i+1] * size[i+1];
        }

        flags |= Mat::CONTINUOUS_FLAG;
    }
    ଅନ୍ୟଥା
    {
        step = std::move(_step);
        ଯଦି (step.size() < size.size())
          step.push_back(elemSize());

        flags = cv::updateContinuityFlag(flags, dims, size.data(), step.data());
    }

    CV_Assert(size.size() == step.size());
    CV_Assert(step.back() == elemSize());
}

#ifndef HAVE_CUDA

GpuData::GpuData(ଅଚଳ size_t _size)
    : data(nullptr), size(0)
{
    CV_UNUSED(_size);
    throw_no_cuda();
}

GpuData::~GpuData()
{
}

ଶୂନ୍ୟ GpuMatND::create(ଅଚଳ MatShape& _shape, ପୂର୍ଣ୍ଣସଂଖ୍ୟା _type)
{
    CV_UNUSED(_shape);
    CV_UNUSED(_type);
    throw_no_cuda();
}

ଶୂନ୍ୟ GpuMatND::release()
{
    throw_no_cuda();
}

GpuMatND GpuMatND::clone() ଅଚଳ
{
    throw_no_cuda();
}

GpuMatND GpuMatND::clone(Stream& stream) ଅଚଳ
{
    CV_UNUSED(stream);
    throw_no_cuda();
}

ଶୂନ୍ୟ GpuMatND::upload(ନିବେଶ_Array src)
{
    CV_UNUSED(src);
    throw_no_cuda();
}

ଶୂନ୍ୟ GpuMatND::upload(ନିବେଶ_Array src, Stream& stream)
{
    CV_UNUSED(src);
    CV_UNUSED(stream);
    throw_no_cuda();
}

ଶୂନ୍ୟ GpuMatND::download(OutputArray dst) ଅଚଳ
{
    CV_UNUSED(dst);
    throw_no_cuda();
}

ଶୂନ୍ୟ GpuMatND::download(OutputArray dst, Stream& stream) ଅଚଳ
{
    CV_UNUSED(dst);
    CV_UNUSED(stream);
    throw_no_cuda();
}

ଶୂନ୍ୟ GpuMatND::fit(ଅଚଳ MatShape& _shape, ପୂର୍ଣ୍ଣସଂଖ୍ୟା _type)
{
    auto elements_nonzero = [](ଅଚଳ MatShape& v)
    {
        ଫେରନ୍ତୁ std::all_of(v.begin(), v.end(), [](ପୂର୍ଣ୍ଣସଂଖ୍ୟା u){ ଫେରନ୍ତୁ u > 0; });
    };
    CV_Assert(!_shape.empty());
    CV_Assert(elements_nonzero(_shape));

    _type &= Mat::TYPE_MASK;

    ଅଚଳ size_t esz = (size_t)CV_ELEM_SIZE(_type);
    size_t step0 = esz;
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)_shape.size() - 2; i >= 0; --i)
        step0 *= (size_t)_shape[(size_t)i + 1];
    ଅଚଳ size_t requiredBytes = (size_t)_shape[0] * step0;

    ଅଚଳ ତାର୍କିକ canReuse = !empty() && !external() && isContinuous() && !isSubmatrix() && offset == 0;
    ଅଚଳ size_t oldCapacity = canReuse ? totalMemSize() : 0;

    ଯଦି (!canReuse || requiredBytes > oldCapacity)
    {
        create(_shape, _type);
        ଫେରନ୍ତୁ;
    }

    setFields(_shape, _type);
}

#endif
