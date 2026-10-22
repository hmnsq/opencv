// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#অন্তর্ভাব "precomp.hpp"

using নামাকাশ cv;
using নামাকাশ cv::cuda;

GpuMatND::~GpuMatND() = পূর্বনির্ধারিত;

GpuMatND::GpuMatND(ধ্রুবক MatShape& _shape, পূর্ণসংখ্যা _type, শূন্য* তথ্য_3, StepArray _step) :
    flags(0), dims(0), তথ্য(static_cast<uchar*>(তথ্য_3)), offset(0)
{
    CV_Assert(_step.empty() || _shape.size() == _step.size() + 1 ||
              (_shape.size() == _step.size() && _step.back() == (size_t)CV_ELEM_আকার(_type)));

    setFields(_shape, _type, std::move(_step));
}

GpuMatND GpuMatND::operator()(ধ্রুবক std::vector<Range>& ranges) ধ্রুবক
{
    CV_Assert(dims == (পূর্ণসংখ্যা)ranges.size());

    জন্য (পূর্ণসংখ্যা i = 0; i < dims; ++i)
    {
        Range r = ranges[i];
        CV_Assert(r == Range::all() || (0 <= r.start && r.start < r.end && r.end <= size[i]));
    }

    GpuMatND ret = *এই;

    জন্য (পূর্ণসংখ্যা i = 0; i < dims; ++i)
    {
        Range r = ranges[i];
        যদি (r != Range::all() && r != Range(0, ret.size[i]))
        {
            ret.offset += r.start * ret.step[i];
            ret.size[i] = r.size();
            ret.flags |= Mat::SUBMATRIX_FLAG;
        }
    }

    ret.flags = cv::updateContinuityFlag(ret.flags, dims, ret.size.তথ্য(), ret.step.তথ্য());

    ফেরত ret;
}

GpuMat GpuMatND::createGpuMatHeader(সূচক_Array idx, Range rowRange, Range colRange) ধ্রুবক
{
    CV_Assert((পূর্ণসংখ্যা)idx.size() == dims - 2);

    std::vector<Range> ranges;
    জন্য (পূর্ণসংখ্যা i : idx)
        ranges.emplace_back(i, i+1);
    ranges.push_back(rowRange);
    ranges.push_back(colRange);

    ফেরত (*এই)(ranges).createGpuMatHeader();
}

GpuMat GpuMatND::createGpuMatHeader() ধ্রুবক
{
    auto Effectively2D = [](GpuMatND m)
    {
        জন্য (পূর্ণসংখ্যা i = 0; i < m.dims - 2; ++i)
            যদি (m.size[i] > 1)
                ফেরত মিথ্যা;
        ফেরত সত্য;
    };
    CV_Assert(Effectively2D(*এই));
    পূর্ণসংখ্যা rows_, cols_ = dims >= 1 ? size[dims - 1] : 1;
    size_t step_;
    যদি (dims >= 2)
    {
        rows_ = size[dims - 2];
        step_ = step[dims - 2];
    }
    অন্যথা
    {
        rows_ = 1;
        step_ = cols_ * elem_আকার();
    }

    ফেরত GpuMat(rows_, cols_, type(), getDevicePtr(), step_);
}

GpuMat GpuMatND::operator()(সূচক_Array idx, Range rowRange, Range colRange) ধ্রুবক
{
    ফেরত createGpuMatHeader(idx, rowRange, colRange).clone();
}

GpuMatND::operator GpuMat() ধ্রুবক
{
    ফেরত createGpuMatHeader().clone();
}

শূন্য GpuMatND::setFields(MatShape আকার_5, পূর্ণসংখ্যা _type, StepArray _step)
{
    _type &= Mat::TYPE_MASK;

    flags = Mat::MAGIC_VAL + _type;
    dims = static_cast<পূর্ণসংখ্যা>(আকার_5.size());
    size = std::move(আকার_5);

    যদি (_step.empty())
    {
        step = StepArray(dims);

        step.back() = elem_আকার();
        জন্য (পূর্ণসংখ্যা _i = dims - 2; _i >= 0; --_i)
        {
            ধ্রুবক size_t i = _i;
            step[i] = step[i+1] * size[i+1];
        }

        flags |= Mat::CONTINUOUS_FLAG;
    }
    অন্যথা
    {
        step = std::move(_step);
        যদি (step.size() < size.size())
          step.push_back(elem_আকার());

        flags = cv::updateContinuityFlag(flags, dims, size.তথ্য(), step.তথ্য());
    }

    CV_Assert(size.size() == step.size());
    CV_Assert(step.back() == elem_আকার());
}

#ifndef HAVE_CUDA

Gpu_তথ্য::Gpu_তথ্য(ধ্রুবক size_t আকার_5)
    : তথ্য(nullptr), size(0)
{
    CV_UNUSED(আকার_5);
    throw_no_cuda();
}

Gpu_তথ্য::~Gpu_তথ্য()
{
}

শূন্য GpuMatND::create(ধ্রুবক MatShape& _shape, পূর্ণসংখ্যা _type)
{
    CV_UNUSED(_shape);
    CV_UNUSED(_type);
    throw_no_cuda();
}

শূন্য GpuMatND::release()
{
    throw_no_cuda();
}

GpuMatND GpuMatND::clone() ধ্রুবক
{
    throw_no_cuda();
}

GpuMatND GpuMatND::clone(Stream& stream) ধ্রুবক
{
    CV_UNUSED(stream);
    throw_no_cuda();
}

শূন্য GpuMatND::upload(প্রবেশ_Array src)
{
    CV_UNUSED(src);
    throw_no_cuda();
}

শূন্য GpuMatND::upload(প্রবেশ_Array src, Stream& stream)
{
    CV_UNUSED(src);
    CV_UNUSED(stream);
    throw_no_cuda();
}

শূন্য GpuMatND::download(OutputArray dst) ধ্রুবক
{
    CV_UNUSED(dst);
    throw_no_cuda();
}

শূন্য GpuMatND::download(OutputArray dst, Stream& stream) ধ্রুবক
{
    CV_UNUSED(dst);
    CV_UNUSED(stream);
    throw_no_cuda();
}

শূন্য GpuMatND::fit(ধ্রুবক MatShape& _shape, পূর্ণসংখ্যা _type)
{
    auto elements_nonzero = [](ধ্রুবক MatShape& v)
    {
        ফেরত std::all_of(v.begin(), v.end(), [](পূর্ণসংখ্যা u){ ফেরত u > 0; });
    };
    CV_Assert(!_shape.empty());
    CV_Assert(elements_nonzero(_shape));

    _type &= Mat::TYPE_MASK;

    ধ্রুবক size_t esz = (size_t)CV_ELEM_আকার(_type);
    size_t step0 = esz;
    জন্য (পূর্ণসংখ্যা i = (পূর্ণসংখ্যা)_shape.size() - 2; i >= 0; --i)
        step0 *= (size_t)_shape[(size_t)i + 1];
    ধ্রুবক size_t requiredBytes = (size_t)_shape[0] * step0;

    ধ্রুবক যৌক্তিক canReuse = !empty() && !external() && isContinuous() && !isSubmatrix() && offset == 0;
    ধ্রুবক size_t oldCapacity = canReuse ? যোগ_Mem_আকার() : 0;

    যদি (!canReuse || requiredBytes > oldCapacity)
    {
        create(_shape, _type);
        ফেরত;
    }

    setFields(_shape, _type);
}

#endif
