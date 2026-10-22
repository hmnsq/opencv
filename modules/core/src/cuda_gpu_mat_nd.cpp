// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#অন্তর্ভাৱ "precomp.hpp"

using নামাকাশ cv;
using নামাকাশ cv::cuda;

GpuMatND::~GpuMatND() = স্ৱভাৱ;

GpuMatND::GpuMatND(ধ্রুৱ MatShape& _shape, পূর্ণাঙ্ক _type, শূন্য* _data, StepArray _step) :
    flags(0), dims(0), data(static_cast<uchar*>(_data)), offset(0)
{
    CV_Assert(_step.empty() || _shape.size() == _step.size() + 1 ||
              (_shape.size() == _step.size() && _step.back() == (size_t)CV_ELEM_SIZE(_type)));

    setFields(_shape, _type, std::move(_step));
}

GpuMatND GpuMatND::operator()(ধ্রুৱ std::vector<Range>& ranges) ধ্রুৱ
{
    CV_Assert(dims == (পূর্ণাঙ্ক)ranges.size());

    বাবে (পূর্ণাঙ্ক i = 0; i < dims; ++i)
    {
        Range r = ranges[i];
        CV_Assert(r == Range::all() || (0 <= r.start && r.start < r.end && r.end <= size[i]));
    }

    GpuMatND ret = *এতত্;

    বাবে (পূর্ণাঙ্ক i = 0; i < dims; ++i)
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

    ঘূৰাই ret;
}

GpuMat GpuMatND::createGpuMatHeader(IndexArray idx, Range rowRange, Range colRange) ধ্রুৱ
{
    CV_Assert((পূর্ণাঙ্ক)idx.size() == dims - 2);

    std::vector<Range> ranges;
    বাবে (পূর্ণাঙ্ক i : idx)
        ranges.emplace_back(i, i+1);
    ranges.push_back(rowRange);
    ranges.push_back(colRange);

    ঘূৰাই (*এতত্)(ranges).createGpuMatHeader();
}

GpuMat GpuMatND::createGpuMatHeader() ধ্রুৱ
{
    auto Effectively2D = [](GpuMatND m)
    {
        বাবে (পূর্ণাঙ্ক i = 0; i < m.dims - 2; ++i)
            যদি (m.size[i] > 1)
                ঘূৰাই মিছা;
        ঘূৰাই সঁচা;
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

    ঘূৰাই GpuMat(rows_, cols_, type(), getDevicePtr(), step_);
}

GpuMat GpuMatND::operator()(IndexArray idx, Range rowRange, Range colRange) ধ্রুৱ
{
    ঘূৰাই createGpuMatHeader(idx, rowRange, colRange).clone();
}

GpuMatND::operator GpuMat() ধ্রুৱ
{
    ঘূৰাই createGpuMatHeader().clone();
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
        বাবে (পূর্ণাঙ্ক _i = dims - 2; _i >= 0; --_i)
        {
            ধ্রুৱ size_t i = _i;
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

GpuData::GpuData(ধ্রুৱ size_t _size)
    : data(nullptr), size(0)
{
    CV_UNUSED(_size);
    throw_no_cuda();
}

GpuData::~GpuData()
{
}

শূন্য GpuMatND::create(ধ্রুৱ MatShape& _shape, পূর্ণাঙ্ক _type)
{
    CV_UNUSED(_shape);
    CV_UNUSED(_type);
    throw_no_cuda();
}

শূন্য GpuMatND::release()
{
    throw_no_cuda();
}

GpuMatND GpuMatND::clone() ধ্রুৱ
{
    throw_no_cuda();
}

GpuMatND GpuMatND::clone(Stream& stream) ধ্রুৱ
{
    CV_UNUSED(stream);
    throw_no_cuda();
}

শূন্য GpuMatND::upload(প্ৰবেশ_Array src)
{
    CV_UNUSED(src);
    throw_no_cuda();
}

শূন্য GpuMatND::upload(প্ৰবেশ_Array src, Stream& stream)
{
    CV_UNUSED(src);
    CV_UNUSED(stream);
    throw_no_cuda();
}

শূন্য GpuMatND::download(OutputArray dst) ধ্রুৱ
{
    CV_UNUSED(dst);
    throw_no_cuda();
}

শূন্য GpuMatND::download(OutputArray dst, Stream& stream) ধ্রুৱ
{
    CV_UNUSED(dst);
    CV_UNUSED(stream);
    throw_no_cuda();
}

শূন্য GpuMatND::fit(ধ্রুৱ MatShape& _shape, পূর্ণাঙ্ক _type)
{
    auto elements_nonzero = [](ধ্রুৱ MatShape& v)
    {
        ঘূৰাই std::all_of(v.begin(), v.end(), [](পূর্ণাঙ্ক u){ ঘূৰাই u > 0; });
    };
    CV_Assert(!_shape.empty());
    CV_Assert(elements_nonzero(_shape));

    _type &= Mat::TYPE_MASK;

    ধ্রুৱ size_t esz = (size_t)CV_ELEM_SIZE(_type);
    size_t step0 = esz;
    বাবে (পূর্ণাঙ্ক i = (পূর্ণাঙ্ক)_shape.size() - 2; i >= 0; --i)
        step0 *= (size_t)_shape[(size_t)i + 1];
    ধ্রুৱ size_t requiredBytes = (size_t)_shape[0] * step0;

    ধ্রুৱ তার্কিক canReuse = !empty() && !external() && isContinuous() && !isSubmatrix() && offset == 0;
    ধ্রুৱ size_t oldCapacity = canReuse ? totalMemSize() : 0;

    যদি (!canReuse || requiredBytes > oldCapacity)
    {
        create(_shape, _type);
        ঘূৰাই;
    }

    setFields(_shape, _type);
}

#endif
