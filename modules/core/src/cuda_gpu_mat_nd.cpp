// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#അന്തര്ഭാവ "precomp.hpp"

using നാമാകാശ cv;
using നാമാകാശ cv::cuda;

GpuMatND::~GpuMatND() = സ്ഥിരസ്ഥിതി;

GpuMatND::GpuMatND(സ്ഥിരം MatShape& _shape, പൂർണ്ണസംഖ്യ _type, ശൂന്യം* _data, StepArray _step) :
    flags(0), dims(0), data(static_cast<uchar*>(_data)), offset(0)
{
    CV_Assert(_step.empty() || _shape.size() == _step.size() + 1 ||
              (_shape.size() == _step.size() && _step.back() == (size_t)CV_ELEM_SIZE(_type)));

    setFields(_shape, _type, std::move(_step));
}

GpuMatND GpuMatND::operator()(സ്ഥിരം std::vector<Range>& ranges) സ്ഥിരം
{
    CV_Assert(dims == (പൂർണ്ണസംഖ്യ)ranges.size());

    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < dims; ++i)
    {
        Range r = ranges[i];
        CV_Assert(r == Range::all() || (0 <= r.start && r.start < r.end && r.end <= size[i]));
    }

    GpuMatND ret = *ഇത്;

    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < dims; ++i)
    {
        Range r = ranges[i];
        എങ്കിൽ (r != Range::all() && r != Range(0, ret.size[i]))
        {
            ret.offset += r.start * ret.step[i];
            ret.size[i] = r.size();
            ret.flags |= Mat::SUBMATRIX_FLAG;
        }
    }

    ret.flags = cv::updateContinuityFlag(ret.flags, dims, ret.size.data(), ret.step.data());

    തിരികെ ret;
}

GpuMat GpuMatND::createGpuMatHeader(IndexArray idx, Range rowRange, Range colRange) സ്ഥിരം
{
    CV_Assert((പൂർണ്ണസംഖ്യ)idx.size() == dims - 2);

    std::vector<Range> ranges;
    വേണ്ടി (പൂർണ്ണസംഖ്യ i : idx)
        ranges.emplace_back(i, i+1);
    ranges.push_back(rowRange);
    ranges.push_back(colRange);

    തിരികെ (*ഇത്)(ranges).createGpuMatHeader();
}

GpuMat GpuMatND::createGpuMatHeader() സ്ഥിരം
{
    auto Effectively2D = [](GpuMatND m)
    {
        വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < m.dims - 2; ++i)
            എങ്കിൽ (m.size[i] > 1)
                തിരികെ അസത്യം;
        തിരികെ സത്യം;
    };
    CV_Assert(Effectively2D(*ഇത്));
    പൂർണ്ണസംഖ്യ rows_, cols_ = dims >= 1 ? size[dims - 1] : 1;
    size_t step_;
    എങ്കിൽ (dims >= 2)
    {
        rows_ = size[dims - 2];
        step_ = step[dims - 2];
    }
    അല്ലെങ്കിൽ
    {
        rows_ = 1;
        step_ = cols_ * elemSize();
    }

    തിരികെ GpuMat(rows_, cols_, type(), getDevicePtr(), step_);
}

GpuMat GpuMatND::operator()(IndexArray idx, Range rowRange, Range colRange) സ്ഥിരം
{
    തിരികെ createGpuMatHeader(idx, rowRange, colRange).clone();
}

GpuMatND::operator GpuMat() സ്ഥിരം
{
    തിരികെ createGpuMatHeader().clone();
}

ശൂന്യം GpuMatND::setFields(MatShape _size, പൂർണ്ണസംഖ്യ _type, StepArray _step)
{
    _type &= Mat::TYPE_MASK;

    flags = Mat::MAGIC_VAL + _type;
    dims = static_cast<പൂർണ്ണസംഖ്യ>(_size.size());
    size = std::move(_size);

    എങ്കിൽ (_step.empty())
    {
        step = StepArray(dims);

        step.back() = elemSize();
        വേണ്ടി (പൂർണ്ണസംഖ്യ _i = dims - 2; _i >= 0; --_i)
        {
            സ്ഥിരം size_t i = _i;
            step[i] = step[i+1] * size[i+1];
        }

        flags |= Mat::CONTINUOUS_FLAG;
    }
    അല്ലെങ്കിൽ
    {
        step = std::move(_step);
        എങ്കിൽ (step.size() < size.size())
          step.push_back(elemSize());

        flags = cv::updateContinuityFlag(flags, dims, size.data(), step.data());
    }

    CV_Assert(size.size() == step.size());
    CV_Assert(step.back() == elemSize());
}

#ifndef HAVE_CUDA

GpuData::GpuData(സ്ഥിരം size_t _size)
    : data(nullptr), size(0)
{
    CV_UNUSED(_size);
    throw_no_cuda();
}

GpuData::~GpuData()
{
}

ശൂന്യം GpuMatND::create(സ്ഥിരം MatShape& _shape, പൂർണ്ണസംഖ്യ _type)
{
    CV_UNUSED(_shape);
    CV_UNUSED(_type);
    throw_no_cuda();
}

ശൂന്യം GpuMatND::release()
{
    throw_no_cuda();
}

GpuMatND GpuMatND::clone() സ്ഥിരം
{
    throw_no_cuda();
}

GpuMatND GpuMatND::clone(Stream& stream) സ്ഥിരം
{
    CV_UNUSED(stream);
    throw_no_cuda();
}

ശൂന്യം GpuMatND::upload(നിവേശം_Array src)
{
    CV_UNUSED(src);
    throw_no_cuda();
}

ശൂന്യം GpuMatND::upload(നിവേശം_Array src, Stream& stream)
{
    CV_UNUSED(src);
    CV_UNUSED(stream);
    throw_no_cuda();
}

ശൂന്യം GpuMatND::download(OutputArray dst) സ്ഥിരം
{
    CV_UNUSED(dst);
    throw_no_cuda();
}

ശൂന്യം GpuMatND::download(OutputArray dst, Stream& stream) സ്ഥിരം
{
    CV_UNUSED(dst);
    CV_UNUSED(stream);
    throw_no_cuda();
}

ശൂന്യം GpuMatND::fit(സ്ഥിരം MatShape& _shape, പൂർണ്ണസംഖ്യ _type)
{
    auto elements_nonzero = [](സ്ഥിരം MatShape& v)
    {
        തിരികെ std::all_of(v.begin(), v.end(), [](പൂർണ്ണസംഖ്യ u){ തിരികെ u > 0; });
    };
    CV_Assert(!_shape.empty());
    CV_Assert(elements_nonzero(_shape));

    _type &= Mat::TYPE_MASK;

    സ്ഥിരം size_t esz = (size_t)CV_ELEM_SIZE(_type);
    size_t step0 = esz;
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = (പൂർണ്ണസംഖ്യ)_shape.size() - 2; i >= 0; --i)
        step0 *= (size_t)_shape[(size_t)i + 1];
    സ്ഥിരം size_t requiredBytes = (size_t)_shape[0] * step0;

    സ്ഥിരം തർക്കം canReuse = !empty() && !external() && isContinuous() && !isSubmatrix() && offset == 0;
    സ്ഥിരം size_t oldCapacity = canReuse ? totalMemSize() : 0;

    എങ്കിൽ (!canReuse || requiredBytes > oldCapacity)
    {
        create(_shape, _type);
        തിരികെ;
    }

    setFields(_shape, _type);
}

#endif
