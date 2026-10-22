// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#अन्तर्भाव "precomp.hpp"

using नामाकाश cv;
using नामाकाश cv::cuda;

GpuMatND::~GpuMatND() = पूर्वनिर्धारित;

GpuMatND::GpuMatND(स्थिरांक MatShape& _shape, पूर्णांक _type, शून्य* _data, StepArray _step) :
    flags(0), dims(0), data(static_cast<uchar*>(_data)), offset(0)
{
    CV_Assert(_step.empty() || _shape.size() == _step.size() + 1 ||
              (_shape.size() == _step.size() && _step.back() == (माप_प्रकार)CV_ELEM_SIZE(_type)));

    setFields(_shape, _type, std::move(_step));
}

GpuMatND GpuMatND::चालक()(स्थिरांक std::vector<Range>& ranges) स्थिरांक
{
    CV_Assert(dims == (पूर्णांक)ranges.size());

    साठी (पूर्णांक i = 0; i < dims; ++i)
    {
        Range r = ranges[i];
        CV_Assert(r == Range::all() || (0 <= r.start && r.start < r.end && r.end <= size[i]));
    }

    GpuMatND ret = *हे;

    साठी (पूर्णांक i = 0; i < dims; ++i)
    {
        Range r = ranges[i];
        जर (r != Range::all() && r != Range(0, ret.size[i]))
        {
            ret.offset += r.start * ret.step[i];
            ret.size[i] = r.size();
            ret.flags |= Mat::SUBMATRIX_FLAG;
        }
    }

    ret.flags = cv::updateContinuityFlag(ret.flags, dims, ret.size.data(), ret.step.data());

    परत ret;
}

GpuMat GpuMatND::createGpuMatHeader(IndexArray idx, Range rowRange, Range colRange) स्थिरांक
{
    CV_Assert((पूर्णांक)idx.size() == dims - 2);

    std::vector<Range> ranges;
    साठी (पूर्णांक i : idx)
        ranges.emplace_back(i, i+1);
    ranges.push_back(rowRange);
    ranges.push_back(colRange);

    परत (*हे)(ranges).createGpuMatHeader();
}

GpuMat GpuMatND::createGpuMatHeader() स्थिरांक
{
    स्वतः Effectively2D = [](GpuMatND m)
    {
        साठी (पूर्णांक i = 0; i < m.dims - 2; ++i)
            जर (m.size[i] > 1)
                परत असत्य;
        परत सत्य;
    };
    CV_Assert(Effectively2D(*हे));
    पूर्णांक rows_, cols_ = dims >= 1 ? size[dims - 1] : 1;
    माप_प्रकार step_;
    जर (dims >= 2)
    {
        rows_ = size[dims - 2];
        step_ = step[dims - 2];
    }
    अन्यथा
    {
        rows_ = 1;
        step_ = cols_ * elemSize();
    }

    परत GpuMat(rows_, cols_, type(), getDevicePtr(), step_);
}

GpuMat GpuMatND::चालक()(IndexArray idx, Range rowRange, Range colRange) स्थिरांक
{
    परत createGpuMatHeader(idx, rowRange, colRange).clone();
}

GpuMatND::चालक GpuMat() स्थिरांक
{
    परत createGpuMatHeader().clone();
}

शून्य GpuMatND::setFields(MatShape _size, पूर्णांक _type, StepArray _step)
{
    _type &= Mat::TYPE_MASK;

    flags = Mat::MAGIC_VAL + _type;
    dims = static_cast<पूर्णांक>(_size.size());
    size = std::move(_size);

    जर (_step.empty())
    {
        step = StepArray(dims);

        step.back() = elemSize();
        साठी (पूर्णांक _i = dims - 2; _i >= 0; --_i)
        {
            स्थिरांक माप_प्रकार i = _i;
            step[i] = step[i+1] * size[i+1];
        }

        flags |= Mat::CONTINUOUS_FLAG;
    }
    अन्यथा
    {
        step = std::move(_step);
        जर (step.size() < size.size())
          step.push_back(elemSize());

        flags = cv::updateContinuityFlag(flags, dims, size.data(), step.data());
    }

    CV_Assert(size.size() == step.size());
    CV_Assert(step.back() == elemSize());
}

#ifndef HAVE_CUDA

GpuData::GpuData(स्थिरांक माप_प्रकार _size)
    : data(nullptr), size(0)
{
    CV_UNUSED(_size);
    throw_no_cuda();
}

GpuData::~GpuData()
{
}

शून्य GpuMatND::create(स्थिरांक MatShape& _shape, पूर्णांक _type)
{
    CV_UNUSED(_shape);
    CV_UNUSED(_type);
    throw_no_cuda();
}

शून्य GpuMatND::release()
{
    throw_no_cuda();
}

GpuMatND GpuMatND::clone() स्थिरांक
{
    throw_no_cuda();
}

GpuMatND GpuMatND::clone(Stream& stream) स्थिरांक
{
    CV_UNUSED(stream);
    throw_no_cuda();
}

शून्य GpuMatND::upload(आदान_Array src)
{
    CV_UNUSED(src);
    throw_no_cuda();
}

शून्य GpuMatND::upload(आदान_Array src, Stream& stream)
{
    CV_UNUSED(src);
    CV_UNUSED(stream);
    throw_no_cuda();
}

शून्य GpuMatND::download(OutputArray dst) स्थिरांक
{
    CV_UNUSED(dst);
    throw_no_cuda();
}

शून्य GpuMatND::download(OutputArray dst, Stream& stream) स्थिरांक
{
    CV_UNUSED(dst);
    CV_UNUSED(stream);
    throw_no_cuda();
}

शून्य GpuMatND::fit(स्थिरांक MatShape& _shape, पूर्णांक _type)
{
    स्वतः elements_nonzero = [](स्थिरांक MatShape& v)
    {
        परत std::all_of(v.begin(), v.end(), [](पूर्णांक u){ परत u > 0; });
    };
    CV_Assert(!_shape.empty());
    CV_Assert(elements_nonzero(_shape));

    _type &= Mat::TYPE_MASK;

    स्थिरांक माप_प्रकार esz = (माप_प्रकार)CV_ELEM_SIZE(_type);
    माप_प्रकार step0 = esz;
    साठी (पूर्णांक i = (पूर्णांक)_shape.size() - 2; i >= 0; --i)
        step0 *= (माप_प्रकार)_shape[(माप_प्रकार)i + 1];
    स्थिरांक माप_प्रकार requiredBytes = (माप_प्रकार)_shape[0] * step0;

    स्थिरांक तार्किक canReuse = !empty() && !external() && isContinuous() && !isSubmatrix() && offset == 0;
    स्थिरांक माप_प्रकार oldCapacity = canReuse ? totalMemSize() : 0;

    जर (!canReuse || requiredBytes > oldCapacity)
    {
        create(_shape, _type);
        परत;
    }

    setFields(_shape, _type);
}

#endif
