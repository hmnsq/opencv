// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#समावेश "precomp.hpp"

using namespace cv;
using namespace cv::cuda;

GpuMatND::~GpuMatND() = शेष;

GpuMatND::GpuMatND(स्थिर MatShape& _shape, पूर्णांक _type, व्योम* तथ्य_3, StepArray _step) :
    flags(0), dims(0), तथ्य(static_cast<uchar*>(तथ्य_3)), offset(0)
{
    CV_Assert(_step.empty() || _shape.size() == _step.size() + 1 ||
              (_shape.size() == _step.size() && _step.back() == (माप_प्रकार)CV_ELEM_आकार(_type)));

    setFields(_shape, _type, std::move(_step));
}

GpuMatND GpuMatND::चालक()(स्थिर std::vector<Range>& ranges) स्थिर
{
    CV_Assert(dims == (पूर्णांक)ranges.size());

    क्रम (पूर्णांक i = 0; i < dims; ++i)
    {
        Range r = ranges[i];
        CV_Assert(r == Range::all() || (0 <= r.start && r.start < r.end && r.end <= size[i]));
    }

    GpuMatND ret = *यह;

    क्रम (पूर्णांक i = 0; i < dims; ++i)
    {
        Range r = ranges[i];
        अगर (r != Range::all() && r != Range(0, ret.size[i]))
        {
            ret.offset += r.start * ret.step[i];
            ret.size[i] = r.size();
            ret.flags |= Mat::SUBMATRIX_FLAG;
        }
    }

    ret.flags = cv::updateContinuityFlag(ret.flags, dims, ret.size.तथ्य(), ret.step.तथ्य());

    वापस ret;
}

GpuMat GpuMatND::createGpuMatHeader(सूचकांक_Array idx, Range rowRange, Range colRange) स्थिर
{
    CV_Assert((पूर्णांक)idx.size() == dims - 2);

    std::vector<Range> ranges;
    क्रम (पूर्णांक i : idx)
        ranges.emplace_back(i, i+1);
    ranges.push_back(rowRange);
    ranges.push_back(colRange);

    वापस (*यह)(ranges).createGpuMatHeader();
}

GpuMat GpuMatND::createGpuMatHeader() स्थिर
{
    स्वतः Effectively2D = [](GpuMatND m)
    {
        क्रम (पूर्णांक i = 0; i < m.dims - 2; ++i)
            अगर (m.size[i] > 1)
                वापस false;
        वापस true;
    };
    CV_Assert(Effectively2D(*यह));
    पूर्णांक rows_, cols_ = dims >= 1 ? size[dims - 1] : 1;
    माप_प्रकार step_;
    अगर (dims >= 2)
    {
        rows_ = size[dims - 2];
        step_ = step[dims - 2];
    }
    अन्यथा
    {
        rows_ = 1;
        step_ = cols_ * elem_आकार();
    }

    वापस GpuMat(rows_, cols_, type(), getDevicePtr(), step_);
}

GpuMat GpuMatND::चालक()(सूचकांक_Array idx, Range rowRange, Range colRange) स्थिर
{
    वापस createGpuMatHeader(idx, rowRange, colRange).clone();
}

GpuMatND::चालक GpuMat() स्थिर
{
    वापस createGpuMatHeader().clone();
}

व्योम GpuMatND::setFields(MatShape आकार_4, पूर्णांक _type, StepArray _step)
{
    _type &= Mat::TYPE_MASK;

    flags = Mat::MAGIC_VAL + _type;
    dims = static_cast<पूर्णांक>(आकार_4.size());
    size = std::move(आकार_4);

    अगर (_step.empty())
    {
        step = StepArray(dims);

        step.back() = elem_आकार();
        क्रम (पूर्णांक _i = dims - 2; _i >= 0; --_i)
        {
            स्थिर माप_प्रकार i = _i;
            step[i] = step[i+1] * size[i+1];
        }

        flags |= Mat::CONTINUOUS_FLAG;
    }
    अन्यथा
    {
        step = std::move(_step);
        अगर (step.size() < size.size())
          step.push_back(elem_आकार());

        flags = cv::updateContinuityFlag(flags, dims, size.तथ्य(), step.तथ्य());
    }

    CV_Assert(size.size() == step.size());
    CV_Assert(step.back() == elem_आकार());
}

#अगर_अघोषित HAVE_CUDA

Gpu_तथ्य::Gpu_तथ्य(स्थिर माप_प्रकार आकार_4)
    : तथ्य(nullptr), size(0)
{
    CV_UNUSED(आकार_4);
    throw_no_cuda();
}

Gpu_तथ्य::~Gpu_तथ्य()
{
}

व्योम GpuMatND::create(स्थिर MatShape& _shape, पूर्णांक _type)
{
    CV_UNUSED(_shape);
    CV_UNUSED(_type);
    throw_no_cuda();
}

व्योम GpuMatND::release()
{
    throw_no_cuda();
}

GpuMatND GpuMatND::clone() स्थिर
{
    throw_no_cuda();
}

GpuMatND GpuMatND::clone(Stream& stream) स्थिर
{
    CV_UNUSED(stream);
    throw_no_cuda();
}

व्योम GpuMatND::upload(निवेश_Array src)
{
    CV_UNUSED(src);
    throw_no_cuda();
}

व्योम GpuMatND::upload(निवेश_Array src, Stream& stream)
{
    CV_UNUSED(src);
    CV_UNUSED(stream);
    throw_no_cuda();
}

व्योम GpuMatND::download(OutputArray dst) स्थिर
{
    CV_UNUSED(dst);
    throw_no_cuda();
}

व्योम GpuMatND::download(OutputArray dst, Stream& stream) स्थिर
{
    CV_UNUSED(dst);
    CV_UNUSED(stream);
    throw_no_cuda();
}

व्योम GpuMatND::fit(स्थिर MatShape& _shape, पूर्णांक _type)
{
    स्वतः elements_nonzero = [](स्थिर MatShape& v)
    {
        वापस std::all_of(v.begin(), v.end(), [](पूर्णांक u){ वापस u > 0; });
    };
    CV_Assert(!_shape.empty());
    CV_Assert(elements_nonzero(_shape));

    _type &= Mat::TYPE_MASK;

    स्थिर माप_प्रकार esz = (माप_प्रकार)CV_ELEM_आकार(_type);
    माप_प्रकार step0 = esz;
    क्रम (पूर्णांक i = (पूर्णांक)_shape.size() - 2; i >= 0; --i)
        step0 *= (माप_प्रकार)_shape[(माप_प्रकार)i + 1];
    स्थिर माप_प्रकार requiredBytes = (माप_प्रकार)_shape[0] * step0;

    स्थिर bool canReuse = !empty() && !external() && isContinuous() && !isSubmatrix() && offset == 0;
    स्थिर माप_प्रकार oldCapacity = canReuse ? योग_Mem_आकार() : 0;

    अगर (!canReuse || requiredBytes > oldCapacity)
    {
        create(_shape, _type);
        वापस;
    }

    setFields(_shape, _type);
}

#पूर्ण_अगर
