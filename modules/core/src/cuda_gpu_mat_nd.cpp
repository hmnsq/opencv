// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ಅನ್ತರ್ಭಾವ "precomp.hpp"

ಬಳಸಿ ನಾಮಾಕಾಶ cv;
ಬಳಸಿ ನಾಮಾಕಾಶ cv::cuda;

GpuMatND::~GpuMatND() = ಪೂರ್ವನಿಯೋಜಿತ;

GpuMatND::GpuMatND(ಸ್ಥಿರಾಂಕ MatShape& _shape, ಪೂರ್ಣಾಂಕ _type, ಶೂನ್ಯ* ದತ್ತಾಂಶ_3, ಹೆಜ್ಜೆ_Array ಹೆಜ್ಜೆ_3) :
    flags(0), dims(0), ದತ್ತಾಂಶ(static_cast<uchar*>(ದತ್ತಾಂಶ_3)), offset(0)
{
    CV_Assert(ಹೆಜ್ಜೆ_3.empty() || _shape.size() == ಹೆಜ್ಜೆ_3.size() + 1 ||
              (_shape.size() == ಹೆಜ್ಜೆ_3.size() && ಹೆಜ್ಜೆ_3.back() == (size_t)CV_ELEM_ಗಾತ್ರ(_type)));

    ಗಣ_Fields(_shape, _type, std::move(ಹೆಜ್ಜೆ_3));
}

GpuMatND GpuMatND::ಕ್ರಿಯಾಕಾರಕ()(ಸ್ಥಿರಾಂಕ std::vector<Range>& ranges) ಸ್ಥಿರಾಂಕ
{
    CV_Assert(dims == (ಪೂರ್ಣಾಂಕ)ranges.size());

    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < dims; ++i)
    {
        Range r = ranges[i];
        CV_Assert(r == Range::all() || (0 <= r.ಆರಂಭ && r.ಆರಂಭ < r.end && r.end <= size[i]));
    }

    GpuMatND ret = *ಇದು;

    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < dims; ++i)
    {
        Range r = ranges[i];
        ಆದರೆ (r != Range::all() && r != Range(0, ret.size[i]))
        {
            ret.offset += r.ಆರಂಭ * ret.ಹೆಜ್ಜೆ[i];
            ret.size[i] = r.size();
            ret.flags |= Mat::SUBMATRIX_FLAG;
        }
    }

    ret.flags = cv::updateContinuityFlag(ret.flags, dims, ret.size.ದತ್ತಾಂಶ(), ret.ಹೆಜ್ಜೆ.ದತ್ತಾಂಶ());

    ಹಿಂತಿರುಗಿ ret;
}

GpuMat GpuMatND::create_Gpu_Mat_ಶೀರ್ಷಿಕೆ(ಸೂಚ್ಯಂಕ_Array idx, Range ಅಡ್ಡಸಾಲು_Range_2, Range ಲಂಬಸಾಲು_Range_2) ಸ್ಥಿರಾಂಕ
{
    CV_Assert((ಪೂರ್ಣಾಂಕ)idx.size() == dims - 2);

    std::vector<Range> ranges;
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i : idx)
        ranges.emplace_back(i, i+1);
    ranges.push_back(ಅಡ್ಡಸಾಲು_Range_2);
    ranges.push_back(ಲಂಬಸಾಲು_Range_2);

    ಹಿಂತಿರುಗಿ (*ಇದು)(ranges).create_Gpu_Mat_ಶೀರ್ಷಿಕೆ();
}

GpuMat GpuMatND::create_Gpu_Mat_ಶೀರ್ಷಿಕೆ() ಸ್ಥಿರಾಂಕ
{
    ಸ್ವಯಂಚಾಲಿತ Effectively2D = [](GpuMatND m)
    {
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < m.dims - 2; ++i)
            ಆದರೆ (m.size[i] > 1)
                ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
        ಹಿಂತಿರುಗಿ ನಿಜ;
    };
    CV_Assert(Effectively2D(*ಇದು));
    ಪೂರ್ಣಾಂಕ ಅಡ್ಡಸಾಲುಗಳು_2, ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_2 = dims >= 1 ? size[dims - 1] : 1;
    size_t ಹೆಜ್ಜೆ_2;
    ಆದರೆ (dims >= 2)
    {
        ಅಡ್ಡಸಾಲುಗಳು_2 = size[dims - 2];
        ಹೆಜ್ಜೆ_2 = ಹೆಜ್ಜೆ[dims - 2];
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        ಅಡ್ಡಸಾಲುಗಳು_2 = 1;
        ಹೆಜ್ಜೆ_2 = ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_2 * elem_ಗಾತ್ರ();
    }

    ಹಿಂತಿರುಗಿ GpuMat(ಅಡ್ಡಸಾಲುಗಳು_2, ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_2, type(), getDevicePtr(), ಹೆಜ್ಜೆ_2);
}

GpuMat GpuMatND::ಕ್ರಿಯಾಕಾರಕ()(ಸೂಚ್ಯಂಕ_Array idx, Range ಅಡ್ಡಸಾಲು_Range_2, Range ಲಂಬಸಾಲು_Range_2) ಸ್ಥಿರಾಂಕ
{
    ಹಿಂತಿರುಗಿ create_Gpu_Mat_ಶೀರ್ಷಿಕೆ(idx, ಅಡ್ಡಸಾಲು_Range_2, ಲಂಬಸಾಲು_Range_2).clone();
}

GpuMatND::ಕ್ರಿಯಾಕಾರಕ GpuMat() ಸ್ಥಿರಾಂಕ
{
    ಹಿಂತಿರುಗಿ create_Gpu_Mat_ಶೀರ್ಷಿಕೆ().clone();
}

ಶೂನ್ಯ GpuMatND::ಗಣ_Fields(MatShape ಗಾತ್ರ_5, ಪೂರ್ಣಾಂಕ _type, ಹೆಜ್ಜೆ_Array ಹೆಜ್ಜೆ_3)
{
    _type &= Mat::TYPE_ಮುಖವಾಡ;

    flags = Mat::MAGIC_VAL + _type;
    dims = static_cast<ಪೂರ್ಣಾಂಕ>(ಗಾತ್ರ_5.size());
    size = std::move(ಗಾತ್ರ_5);

    ಆದರೆ (ಹೆಜ್ಜೆ_3.empty())
    {
        ಹೆಜ್ಜೆ = ಹೆಜ್ಜೆ_Array(dims);

        ಹೆಜ್ಜೆ.back() = elem_ಗಾತ್ರ();
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ _i = dims - 2; _i >= 0; --_i)
        {
            ಸ್ಥಿರಾಂಕ size_t i = _i;
            ಹೆಜ್ಜೆ[i] = ಹೆಜ್ಜೆ[i+1] * size[i+1];
        }

        flags |= Mat::CONTINUOUS_FLAG;
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        ಹೆಜ್ಜೆ = std::move(ಹೆಜ್ಜೆ_3);
        ಆದರೆ (ಹೆಜ್ಜೆ.size() < size.size())
          ಹೆಜ್ಜೆ.push_back(elem_ಗಾತ್ರ());

        flags = cv::updateContinuityFlag(flags, dims, size.ದತ್ತಾಂಶ(), ಹೆಜ್ಜೆ.ದತ್ತಾಂಶ());
    }

    CV_Assert(size.size() == ಹೆಜ್ಜೆ.size());
    CV_Assert(ಹೆಜ್ಜೆ.back() == elem_ಗಾತ್ರ());
}

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_CUDA

Gpu_ದತ್ತಾಂಶ::Gpu_ದತ್ತಾಂಶ(ಸ್ಥಿರಾಂಕ size_t ಗಾತ್ರ_5)
    : ದತ್ತಾಂಶ(ಶೂನ್ಯಸೂಚಕ), size(0)
{
    CV_ಬಳಸದ(ಗಾತ್ರ_5);
    throw_no_cuda();
}

Gpu_ದತ್ತಾಂಶ::~Gpu_ದತ್ತಾಂಶ()
{
}

ಶೂನ್ಯ GpuMatND::create(ಸ್ಥಿರಾಂಕ MatShape& _shape, ಪೂರ್ಣಾಂಕ _type)
{
    CV_ಬಳಸದ(_shape);
    CV_ಬಳಸದ(_type);
    throw_no_cuda();
}

ಶೂನ್ಯ GpuMatND::release()
{
    throw_no_cuda();
}

GpuMatND GpuMatND::clone() ಸ್ಥಿರಾಂಕ
{
    throw_no_cuda();
}

GpuMatND GpuMatND::clone(ಹರಿವು& stream) ಸ್ಥಿರಾಂಕ
{
    CV_ಬಳಸದ(stream);
    throw_no_cuda();
}

ಶೂನ್ಯ GpuMatND::upload(ಒಳಹರಿವು_Array src)
{
    CV_ಬಳಸದ(src);
    throw_no_cuda();
}

ಶೂನ್ಯ GpuMatND::upload(ಒಳಹರಿವು_Array src, ಹರಿವು& stream)
{
    CV_ಬಳಸದ(src);
    CV_ಬಳಸದ(stream);
    throw_no_cuda();
}

ಶೂನ್ಯ GpuMatND::download(OutputArray dst) ಸ್ಥಿರಾಂಕ
{
    CV_ಬಳಸದ(dst);
    throw_no_cuda();
}

ಶೂನ್ಯ GpuMatND::download(OutputArray dst, ಹರಿವು& stream) ಸ್ಥಿರಾಂಕ
{
    CV_ಬಳಸದ(dst);
    CV_ಬಳಸದ(stream);
    throw_no_cuda();
}

ಶೂನ್ಯ GpuMatND::fit(ಸ್ಥಿರಾಂಕ MatShape& _shape, ಪೂರ್ಣಾಂಕ _type)
{
    ಸ್ವಯಂಚಾಲಿತ elements_nonzero = [](ಸ್ಥಿರಾಂಕ MatShape& v)
    {
        ಹಿಂತಿರುಗಿ std::all_of(v.begin(), v.end(), [](ಪೂರ್ಣಾಂಕ u){ ಹಿಂತಿರುಗಿ u > 0; });
    };
    CV_Assert(!_shape.empty());
    CV_Assert(elements_nonzero(_shape));

    _type &= Mat::TYPE_ಮುಖವಾಡ;

    ಸ್ಥಿರಾಂಕ size_t esz = (size_t)CV_ELEM_ಗಾತ್ರ(_type);
    size_t step0 = esz;
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = (ಪೂರ್ಣಾಂಕ)_shape.size() - 2; i >= 0; --i)
        step0 *= (size_t)_shape[(size_t)i + 1];
    ಸ್ಥಿರಾಂಕ size_t required_ಬೈಟುಗಳು = (size_t)_shape[0] * step0;

    ಸ್ಥಿರಾಂಕ ತಾರ್ಕಿಕ canReuse = !empty() && !external() && isContinuous() && !isSubmatrix() && offset == 0;
    ಸ್ಥಿರಾಂಕ size_t old_ಸಾಮರ್ಥ್ಯ = canReuse ? ಒಟ್ಟು_Mem_ಗಾತ್ರ() : 0;

    ಆದರೆ (!canReuse || required_ಬೈಟುಗಳು > old_ಸಾಮರ್ಥ್ಯ)
    {
        create(_shape, _type);
        ಹಿಂತಿರುಗಿ;
    }

    ಗಣ_Fields(_shape, _type);
}

#ಆದರೆ_ಅಂತ್ಯ
