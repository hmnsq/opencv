// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#અન્તર્ભાવ "precomp.hpp"

using નામાકાશ cv;
using નામાકાશ cv::cuda;

GpuMatND::~GpuMatND() = મૂળભૂત;

GpuMatND::GpuMatND(અચલ MatShape& _shape, પૂર્ણાંક _type, શૂન્ય* _data, StepArray _step) :
    flags(0), dims(0), data(static_cast<uchar*>(_data)), offset(0)
{
    CV_Assert(_step.empty() || _shape.size() == _step.size() + 1 ||
              (_shape.size() == _step.size() && _step.back() == (size_t)CV_ELEM_SIZE(_type)));

    setFields(_shape, _type, std::move(_step));
}

GpuMatND GpuMatND::operator()(અચલ std::vector<Range>& ranges) અચલ
{
    CV_Assert(dims == (પૂર્ણાંક)ranges.size());

    માટે (પૂર્ણાંક i = 0; i < dims; ++i)
    {
        Range r = ranges[i];
        CV_Assert(r == Range::all() || (0 <= r.start && r.start < r.end && r.end <= size[i]));
    }

    GpuMatND ret = *આ;

    માટે (પૂર્ણાંક i = 0; i < dims; ++i)
    {
        Range r = ranges[i];
        જો (r != Range::all() && r != Range(0, ret.size[i]))
        {
            ret.offset += r.start * ret.step[i];
            ret.size[i] = r.size();
            ret.flags |= Mat::SUBMATRIX_FLAG;
        }
    }

    ret.flags = cv::updateContinuityFlag(ret.flags, dims, ret.size.data(), ret.step.data());

    પાછા ret;
}

GpuMat GpuMatND::createGpuMatHeader(IndexArray idx, Range rowRange, Range colRange) અચલ
{
    CV_Assert((પૂર્ણાંક)idx.size() == dims - 2);

    std::vector<Range> ranges;
    માટે (પૂર્ણાંક i : idx)
        ranges.emplace_back(i, i+1);
    ranges.push_back(rowRange);
    ranges.push_back(colRange);

    પાછા (*આ)(ranges).createGpuMatHeader();
}

GpuMat GpuMatND::createGpuMatHeader() અચલ
{
    auto Effectively2D = [](GpuMatND m)
    {
        માટે (પૂર્ણાંક i = 0; i < m.dims - 2; ++i)
            જો (m.size[i] > 1)
                પાછા ખોટું;
        પાછા સાચું;
    };
    CV_Assert(Effectively2D(*આ));
    પૂર્ણાંક rows_, cols_ = dims >= 1 ? size[dims - 1] : 1;
    size_t step_;
    જો (dims >= 2)
    {
        rows_ = size[dims - 2];
        step_ = step[dims - 2];
    }
    નહીંતર
    {
        rows_ = 1;
        step_ = cols_ * elemSize();
    }

    પાછા GpuMat(rows_, cols_, type(), getDevicePtr(), step_);
}

GpuMat GpuMatND::operator()(IndexArray idx, Range rowRange, Range colRange) અચલ
{
    પાછા createGpuMatHeader(idx, rowRange, colRange).clone();
}

GpuMatND::operator GpuMat() અચલ
{
    પાછા createGpuMatHeader().clone();
}

શૂન્ય GpuMatND::setFields(MatShape _size, પૂર્ણાંક _type, StepArray _step)
{
    _type &= Mat::TYPE_MASK;

    flags = Mat::MAGIC_VAL + _type;
    dims = static_cast<પૂર્ણાંક>(_size.size());
    size = std::move(_size);

    જો (_step.empty())
    {
        step = StepArray(dims);

        step.back() = elemSize();
        માટે (પૂર્ણાંક _i = dims - 2; _i >= 0; --_i)
        {
            અચલ size_t i = _i;
            step[i] = step[i+1] * size[i+1];
        }

        flags |= Mat::CONTINUOUS_FLAG;
    }
    નહીંતર
    {
        step = std::move(_step);
        જો (step.size() < size.size())
          step.push_back(elemSize());

        flags = cv::updateContinuityFlag(flags, dims, size.data(), step.data());
    }

    CV_Assert(size.size() == step.size());
    CV_Assert(step.back() == elemSize());
}

#ifndef HAVE_CUDA

GpuData::GpuData(અચલ size_t _size)
    : data(nullptr), size(0)
{
    CV_UNUSED(_size);
    throw_no_cuda();
}

GpuData::~GpuData()
{
}

શૂન્ય GpuMatND::create(અચલ MatShape& _shape, પૂર્ણાંક _type)
{
    CV_UNUSED(_shape);
    CV_UNUSED(_type);
    throw_no_cuda();
}

શૂન્ય GpuMatND::release()
{
    throw_no_cuda();
}

GpuMatND GpuMatND::clone() અચલ
{
    throw_no_cuda();
}

GpuMatND GpuMatND::clone(Stream& stream) અચલ
{
    CV_UNUSED(stream);
    throw_no_cuda();
}

શૂન્ય GpuMatND::upload(આગત_Array src)
{
    CV_UNUSED(src);
    throw_no_cuda();
}

શૂન્ય GpuMatND::upload(આગત_Array src, Stream& stream)
{
    CV_UNUSED(src);
    CV_UNUSED(stream);
    throw_no_cuda();
}

શૂન્ય GpuMatND::download(OutputArray dst) અચલ
{
    CV_UNUSED(dst);
    throw_no_cuda();
}

શૂન્ય GpuMatND::download(OutputArray dst, Stream& stream) અચલ
{
    CV_UNUSED(dst);
    CV_UNUSED(stream);
    throw_no_cuda();
}

શૂન્ય GpuMatND::fit(અચલ MatShape& _shape, પૂર્ણાંક _type)
{
    auto elements_nonzero = [](અચલ MatShape& v)
    {
        પાછા std::all_of(v.begin(), v.end(), [](પૂર્ણાંક u){ પાછા u > 0; });
    };
    CV_Assert(!_shape.empty());
    CV_Assert(elements_nonzero(_shape));

    _type &= Mat::TYPE_MASK;

    અચલ size_t esz = (size_t)CV_ELEM_SIZE(_type);
    size_t step0 = esz;
    માટે (પૂર્ણાંક i = (પૂર્ણાંક)_shape.size() - 2; i >= 0; --i)
        step0 *= (size_t)_shape[(size_t)i + 1];
    અચલ size_t requiredBytes = (size_t)_shape[0] * step0;

    અચલ તાર્કિક canReuse = !empty() && !external() && isContinuous() && !isSubmatrix() && offset == 0;
    અચલ size_t oldCapacity = canReuse ? totalMemSize() : 0;

    જો (!canReuse || requiredBytes > oldCapacity)
    {
        create(_shape, _type);
        પાછા;
    }

    setFields(_shape, _type);
}

#endif
