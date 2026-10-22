// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ಅನ್ತರ್ಭಾವ "precomp.hpp"

using ನಾಮಾಕಾಶ cv;
using ನಾಮಾಕಾಶ cv::cuda;

GpuMatND::~GpuMatND() = ಸ್ವಭಾವ;

GpuMatND::GpuMatND(ಧ್ರುವ MatShape& _shape, ಪೂರ್ಣಾಙ್ಕ _type, ಶೂನ್ಯ* _data, StepArray _step) :
    flags(0), dims(0), data(static_cast<uchar*>(_data)), offset(0)
{
    CV_Assert(_step.empty() || _shape.size() == _step.size() + 1 ||
              (_shape.size() == _step.size() && _step.back() == (size_t)CV_ELEM_SIZE(_type)));

    setFields(_shape, _type, std::move(_step));
}

GpuMatND GpuMatND::operator()(ಧ್ರುವ std::vector<Range>& ranges) ಧ್ರುವ
{
    CV_Assert(dims == (ಪೂರ್ಣಾಙ್ಕ)ranges.size());

    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < dims; ++i)
    {
        Range r = ranges[i];
        CV_Assert(r == Range::all() || (0 <= r.start && r.start < r.end && r.end <= size[i]));
    }

    GpuMatND ret = *ಏತತ್;

    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < dims; ++i)
    {
        Range r = ranges[i];
        ಯದಿ (r != Range::all() && r != Range(0, ret.size[i]))
        {
            ret.offset += r.start * ret.step[i];
            ret.size[i] = r.size();
            ret.flags |= Mat::SUBMATRIX_FLAG;
        }
    }

    ret.flags = cv::updateContinuityFlag(ret.flags, dims, ret.size.data(), ret.step.data());

    ಪ್ರತ್ಯಾಗಮ ret;
}

GpuMat GpuMatND::createGpuMatHeader(IndexArray idx, Range rowRange, Range colRange) ಧ್ರುವ
{
    CV_Assert((ಪೂರ್ಣಾಙ್ಕ)idx.size() == dims - 2);

    std::vector<Range> ranges;
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i : idx)
        ranges.emplace_back(i, i+1);
    ranges.push_back(rowRange);
    ranges.push_back(colRange);

    ಪ್ರತ್ಯಾಗಮ (*ಏತತ್)(ranges).createGpuMatHeader();
}

GpuMat GpuMatND::createGpuMatHeader() ಧ್ರುವ
{
    auto Effectively2D = [](GpuMatND m)
    {
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < m.dims - 2; ++i)
            ಯದಿ (m.size[i] > 1)
                ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
        ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    };
    CV_Assert(Effectively2D(*ಏತತ್));
    ಪೂರ್ಣಾಙ್ಕ rows_, cols_ = dims >= 1 ? size[dims - 1] : 1;
    size_t step_;
    ಯದಿ (dims >= 2)
    {
        rows_ = size[dims - 2];
        step_ = step[dims - 2];
    }
    ಅನ್ಯಥಾ
    {
        rows_ = 1;
        step_ = cols_ * elemSize();
    }

    ಪ್ರತ್ಯಾಗಮ GpuMat(rows_, cols_, type(), getDevicePtr(), step_);
}

GpuMat GpuMatND::operator()(IndexArray idx, Range rowRange, Range colRange) ಧ್ರುವ
{
    ಪ್ರತ್ಯಾಗಮ createGpuMatHeader(idx, rowRange, colRange).clone();
}

GpuMatND::operator GpuMat() ಧ್ರುವ
{
    ಪ್ರತ್ಯಾಗಮ createGpuMatHeader().clone();
}

ಶೂನ್ಯ GpuMatND::setFields(MatShape _size, ಪೂರ್ಣಾಙ್ಕ _type, StepArray _step)
{
    _type &= Mat::TYPE_MASK;

    flags = Mat::MAGIC_VAL + _type;
    dims = static_cast<ಪೂರ್ಣಾಙ್ಕ>(_size.size());
    size = std::move(_size);

    ಯದಿ (_step.empty())
    {
        step = StepArray(dims);

        step.back() = elemSize();
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ _i = dims - 2; _i >= 0; --_i)
        {
            ಧ್ರುವ size_t i = _i;
            step[i] = step[i+1] * size[i+1];
        }

        flags |= Mat::CONTINUOUS_FLAG;
    }
    ಅನ್ಯಥಾ
    {
        step = std::move(_step);
        ಯದಿ (step.size() < size.size())
          step.push_back(elemSize());

        flags = cv::updateContinuityFlag(flags, dims, size.data(), step.data());
    }

    CV_Assert(size.size() == step.size());
    CV_Assert(step.back() == elemSize());
}

#ifndef HAVE_CUDA

GpuData::GpuData(ಧ್ರುವ size_t _size)
    : data(nullptr), size(0)
{
    CV_UNUSED(_size);
    throw_no_cuda();
}

GpuData::~GpuData()
{
}

ಶೂನ್ಯ GpuMatND::create(ಧ್ರುವ MatShape& _shape, ಪೂರ್ಣಾಙ್ಕ _type)
{
    CV_UNUSED(_shape);
    CV_UNUSED(_type);
    throw_no_cuda();
}

ಶೂನ್ಯ GpuMatND::release()
{
    throw_no_cuda();
}

GpuMatND GpuMatND::clone() ಧ್ರುವ
{
    throw_no_cuda();
}

GpuMatND GpuMatND::clone(Stream& stream) ಧ್ರುವ
{
    CV_UNUSED(stream);
    throw_no_cuda();
}

ಶೂನ್ಯ GpuMatND::upload(ಒಳಹರಿವು_Array src)
{
    CV_UNUSED(src);
    throw_no_cuda();
}

ಶೂನ್ಯ GpuMatND::upload(ಒಳಹರಿವು_Array src, Stream& stream)
{
    CV_UNUSED(src);
    CV_UNUSED(stream);
    throw_no_cuda();
}

ಶೂನ್ಯ GpuMatND::download(OutputArray dst) ಧ್ರುವ
{
    CV_UNUSED(dst);
    throw_no_cuda();
}

ಶೂನ್ಯ GpuMatND::download(OutputArray dst, Stream& stream) ಧ್ರುವ
{
    CV_UNUSED(dst);
    CV_UNUSED(stream);
    throw_no_cuda();
}

ಶೂನ್ಯ GpuMatND::fit(ಧ್ರುವ MatShape& _shape, ಪೂರ್ಣಾಙ್ಕ _type)
{
    auto elements_nonzero = [](ಧ್ರುವ MatShape& v)
    {
        ಪ್ರತ್ಯಾಗಮ std::all_of(v.begin(), v.end(), [](ಪೂರ್ಣಾಙ್ಕ u){ ಪ್ರತ್ಯಾಗಮ u > 0; });
    };
    CV_Assert(!_shape.empty());
    CV_Assert(elements_nonzero(_shape));

    _type &= Mat::TYPE_MASK;

    ಧ್ರುವ size_t esz = (size_t)CV_ELEM_SIZE(_type);
    size_t step0 = esz;
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = (ಪೂರ್ಣಾಙ್ಕ)_shape.size() - 2; i >= 0; --i)
        step0 *= (size_t)_shape[(size_t)i + 1];
    ಧ್ರುವ size_t requiredBytes = (size_t)_shape[0] * step0;

    ಧ್ರುವ ತಾರ್ಕಿಕ canReuse = !empty() && !external() && isContinuous() && !isSubmatrix() && offset == 0;
    ಧ್ರುವ size_t oldCapacity = canReuse ? totalMemSize() : 0;

    ಯದಿ (!canReuse || requiredBytes > oldCapacity)
    {
        create(_shape, _type);
        ಪ್ರತ್ಯಾಗಮ;
    }

    setFields(_shape, _type);
}

#endif
