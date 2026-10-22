// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ਅਨ੍ਤਰ੍ਭਾਵ "precomp.hpp"

ਵਰਤੋਂ ਨਾਮਾਕਾਸ਼ cv;
ਵਰਤੋਂ ਨਾਮਾਕਾਸ਼ cv::cuda;

GpuMatND::~GpuMatND() = ਮੂਲ;

GpuMatND::GpuMatND(ਅਚਲ MatShape& _shape, ਪੂਰਨ _type, ਖਾਲੀ* ਅੰਕੜੇ_3, ਕਦਮ_Array ਕਦਮ_3) :
    flags(0), dims(0), ਅੰਕੜੇ(static_cast<uchar*>(ਅੰਕੜੇ_3)), offset(0)
{
    CV_Assert(ਕਦਮ_3.empty() || _shape.size() == ਕਦਮ_3.size() + 1 ||
              (_shape.size() == ਕਦਮ_3.size() && ਕਦਮ_3.back() == (size_t)CV_ELEM_ਮਾਪ(_type)));

    ਸੈੱਟ_Fields(_shape, _type, std::move(ਕਦਮ_3));
}

GpuMatND GpuMatND::ਸੰਚਾਲਕ()(ਅਚਲ std::vector<Range>& ranges) ਅਚਲ
{
    CV_Assert(dims == (ਪੂਰਨ)ranges.size());

    ਲਈ (ਪੂਰਨ i = 0; i < dims; ++i)
    {
        Range r = ranges[i];
        CV_Assert(r == Range::all() || (0 <= r.ਸ਼ੁਰੂ && r.ਸ਼ੁਰੂ < r.end && r.end <= size[i]));
    }

    GpuMatND ret = *ਇਹ;

    ਲਈ (ਪੂਰਨ i = 0; i < dims; ++i)
    {
        Range r = ranges[i];
        ਜੇ (r != Range::all() && r != Range(0, ret.size[i]))
        {
            ret.offset += r.ਸ਼ੁਰੂ * ret.ਕਦਮ[i];
            ret.size[i] = r.size();
            ret.flags |= Mat::SUBMATRIX_FLAG;
        }
    }

    ret.flags = cv::updateContinuityFlag(ret.flags, dims, ret.size.ਅੰਕੜੇ(), ret.ਕਦਮ.ਅੰਕੜੇ());

    ਵਾਪਸ ret;
}

GpuMat GpuMatND::create_Gpu_Mat_ਸਿਰਲੇਖ(ਸੂਚਕਾਂਕ_Array idx, Range ਪੰਕਤੀ_Range_2, Range ਖਾਨਾ_Range_2) ਅਚਲ
{
    CV_Assert((ਪੂਰਨ)idx.size() == dims - 2);

    std::vector<Range> ranges;
    ਲਈ (ਪੂਰਨ i : idx)
        ranges.emplace_back(i, i+1);
    ranges.push_back(ਪੰਕਤੀ_Range_2);
    ranges.push_back(ਖਾਨਾ_Range_2);

    ਵਾਪਸ (*ਇਹ)(ranges).create_Gpu_Mat_ਸਿਰਲੇਖ();
}

GpuMat GpuMatND::create_Gpu_Mat_ਸਿਰਲੇਖ() ਅਚਲ
{
    ਸਵੈਚਾਲਿਤ Effectively2D = [](GpuMatND m)
    {
        ਲਈ (ਪੂਰਨ i = 0; i < m.dims - 2; ++i)
            ਜੇ (m.size[i] > 1)
                ਵਾਪਸ ਝੂਠ;
        ਵਾਪਸ ਸੱਚ;
    };
    CV_Assert(Effectively2D(*ਇਹ));
    ਪੂਰਨ ਪੰਕਤੀਆਂ_2, ਖਾਨੇ_ਗਿਣਤੀ_2 = dims >= 1 ? size[dims - 1] : 1;
    size_t ਕਦਮ_2;
    ਜੇ (dims >= 2)
    {
        ਪੰਕਤੀਆਂ_2 = size[dims - 2];
        ਕਦਮ_2 = ਕਦਮ[dims - 2];
    }
    ਨਹੀਂਤਾਂ
    {
        ਪੰਕਤੀਆਂ_2 = 1;
        ਕਦਮ_2 = ਖਾਨੇ_ਗਿਣਤੀ_2 * elem_ਮਾਪ();
    }

    ਵਾਪਸ GpuMat(ਪੰਕਤੀਆਂ_2, ਖਾਨੇ_ਗਿਣਤੀ_2, type(), getDevicePtr(), ਕਦਮ_2);
}

GpuMat GpuMatND::ਸੰਚਾਲਕ()(ਸੂਚਕਾਂਕ_Array idx, Range ਪੰਕਤੀ_Range_2, Range ਖਾਨਾ_Range_2) ਅਚਲ
{
    ਵਾਪਸ create_Gpu_Mat_ਸਿਰਲੇਖ(idx, ਪੰਕਤੀ_Range_2, ਖਾਨਾ_Range_2).clone();
}

GpuMatND::ਸੰਚਾਲਕ GpuMat() ਅਚਲ
{
    ਵਾਪਸ create_Gpu_Mat_ਸਿਰਲੇਖ().clone();
}

ਖਾਲੀ GpuMatND::ਸੈੱਟ_Fields(MatShape ਮਾਪ_4, ਪੂਰਨ _type, ਕਦਮ_Array ਕਦਮ_3)
{
    _type &= Mat::TYPE_ਨਕਾਬ;

    flags = Mat::MAGIC_VAL + _type;
    dims = static_cast<ਪੂਰਨ>(ਮਾਪ_4.size());
    size = std::move(ਮਾਪ_4);

    ਜੇ (ਕਦਮ_3.empty())
    {
        ਕਦਮ = ਕਦਮ_Array(dims);

        ਕਦਮ.back() = elem_ਮਾਪ();
        ਲਈ (ਪੂਰਨ _i = dims - 2; _i >= 0; --_i)
        {
            ਅਚਲ size_t i = _i;
            ਕਦਮ[i] = ਕਦਮ[i+1] * size[i+1];
        }

        flags |= Mat::CONTINUOUS_FLAG;
    }
    ਨਹੀਂਤਾਂ
    {
        ਕਦਮ = std::move(ਕਦਮ_3);
        ਜੇ (ਕਦਮ.size() < size.size())
          ਕਦਮ.push_back(elem_ਮਾਪ());

        flags = cv::updateContinuityFlag(flags, dims, size.ਅੰਕੜੇ(), ਕਦਮ.ਅੰਕੜੇ());
    }

    CV_Assert(size.size() == ਕਦਮ.size());
    CV_Assert(ਕਦਮ.back() == elem_ਮਾਪ());
}

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA

Gpu_ਅੰਕੜੇ::Gpu_ਅੰਕੜੇ(ਅਚਲ size_t ਮਾਪ_4)
    : ਅੰਕੜੇ(ਖਾਲੀ_ਸੂਚਕ), size(0)
{
    CV_ਅਣਵਰਤਿਆ(ਮਾਪ_4);
    throw_no_cuda();
}

Gpu_ਅੰਕੜੇ::~Gpu_ਅੰਕੜੇ()
{
}

ਖਾਲੀ GpuMatND::create(ਅਚਲ MatShape& _shape, ਪੂਰਨ _type)
{
    CV_ਅਣਵਰਤਿਆ(_shape);
    CV_ਅਣਵਰਤਿਆ(_type);
    throw_no_cuda();
}

ਖਾਲੀ GpuMatND::release()
{
    throw_no_cuda();
}

GpuMatND GpuMatND::clone() ਅਚਲ
{
    throw_no_cuda();
}

GpuMatND GpuMatND::clone(ਧਾਰਾ& stream) ਅਚਲ
{
    CV_ਅਣਵਰਤਿਆ(stream);
    throw_no_cuda();
}

ਖਾਲੀ GpuMatND::upload(ਨਿਵੇਸ਼_Array src)
{
    CV_ਅਣਵਰਤਿਆ(src);
    throw_no_cuda();
}

ਖਾਲੀ GpuMatND::upload(ਨਿਵੇਸ਼_Array src, ਧਾਰਾ& stream)
{
    CV_ਅਣਵਰਤਿਆ(src);
    CV_ਅਣਵਰਤਿਆ(stream);
    throw_no_cuda();
}

ਖਾਲੀ GpuMatND::download(OutputArray dst) ਅਚਲ
{
    CV_ਅਣਵਰਤਿਆ(dst);
    throw_no_cuda();
}

ਖਾਲੀ GpuMatND::download(OutputArray dst, ਧਾਰਾ& stream) ਅਚਲ
{
    CV_ਅਣਵਰਤਿਆ(dst);
    CV_ਅਣਵਰਤਿਆ(stream);
    throw_no_cuda();
}

ਖਾਲੀ GpuMatND::fit(ਅਚਲ MatShape& _shape, ਪੂਰਨ _type)
{
    ਸਵੈਚਾਲਿਤ elements_nonzero = [](ਅਚਲ MatShape& v)
    {
        ਵਾਪਸ std::all_of(v.begin(), v.end(), [](ਪੂਰਨ u){ ਵਾਪਸ u > 0; });
    };
    CV_Assert(!_shape.empty());
    CV_Assert(elements_nonzero(_shape));

    _type &= Mat::TYPE_ਨਕਾਬ;

    ਅਚਲ size_t esz = (size_t)CV_ELEM_ਮਾਪ(_type);
    size_t step0 = esz;
    ਲਈ (ਪੂਰਨ i = (ਪੂਰਨ)_shape.size() - 2; i >= 0; --i)
        step0 *= (size_t)_shape[(size_t)i + 1];
    ਅਚਲ size_t required_ਬਾਈਟਾਂ = (size_t)_shape[0] * step0;

    ਅਚਲ ਤਾਰਕਿਕ canReuse = !empty() && !external() && isContinuous() && !isSubmatrix() && offset == 0;
    ਅਚਲ size_t old_ਸਮਰੱਥਾ = canReuse ? ਕੁੱਲ_Mem_ਮਾਪ() : 0;

    ਜੇ (!canReuse || required_ਬਾਈਟਾਂ > old_ਸਮਰੱਥਾ)
    {
        create(_shape, _type);
        ਵਾਪਸ;
    }

    ਸੈੱਟ_Fields(_shape, _type);
}

#ਜੇ_ਅੰਤ
