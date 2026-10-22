// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#अन्तर्भाव "precomp.hpp"

उपयुज्य नामाकाश cv;
उपयुज्य नामाकाश cv::cuda;

GpuMatND::~GpuMatND() = स्वभाव;

GpuMatND::GpuMatND(ध्रुव MatShape& _shape, पूर्णाङ्क _type, शून्य* दत्त_3, चरणः_Array चरणः_3) :
    flags(0), dims(0), दत्त(static_cast<uchar*>(दत्त_3)), offset(0)
{
    CV_Assert(चरणः_3.empty() || _shape.size() == चरणः_3.size() + 1 ||
              (_shape.size() == चरणः_3.size() && चरणः_3.back() == (size_t)CV_ELEM_आकार(_type)));

    समुच्चयः_Fields(_shape, _type, std::move(चरणः_3));
}

GpuMatND GpuMatND::संक्रियकः()(ध्रुव std::vector<Range>& ranges) ध्रुव
{
    CV_Assert(dims == (पूर्णाङ्क)ranges.size());

    प्रति (पूर्णाङ्क i = 0; i < dims; ++i)
    {
        Range r = ranges[i];
        CV_Assert(r == Range::all() || (0 <= r.start && r.start < r.end && r.end <= size[i]));
    }

    GpuMatND ret = *एतत्;

    प्रति (पूर्णाङ्क i = 0; i < dims; ++i)
    {
        Range r = ranges[i];
        यदि (r != Range::all() && r != Range(0, ret.size[i]))
        {
            ret.offset += r.start * ret.चरणः[i];
            ret.size[i] = r.size();
            ret.flags |= Mat::SUBMATRIX_FLAG;
        }
    }

    ret.flags = cv::updateContinuityFlag(ret.flags, dims, ret.size.दत्त(), ret.चरणः.दत्त());

    प्रत्यागम ret;
}

GpuMat GpuMatND::create_Gpu_Mat_शीर्षकम्(सूचकांक_Array idx, Range rowRange, Range colRange) ध्रुव
{
    CV_Assert((पूर्णाङ्क)idx.size() == dims - 2);

    std::vector<Range> ranges;
    प्रति (पूर्णाङ्क i : idx)
        ranges.emplace_back(i, i+1);
    ranges.push_back(rowRange);
    ranges.push_back(colRange);

    प्रत्यागम (*एतत्)(ranges).create_Gpu_Mat_शीर्षकम्();
}

GpuMat GpuMatND::create_Gpu_Mat_शीर्षकम्() ध्रुव
{
    स्वचालित Effectively2D = [](GpuMatND m)
    {
        प्रति (पूर्णाङ्क i = 0; i < m.dims - 2; ++i)
            यदि (m.size[i] > 1)
                प्रत्यागम असत्य;
        प्रत्यागम सत्य;
    };
    CV_Assert(Effectively2D(*एतत्));
    पूर्णाङ्क पङ्क्तिकाः_2, स्तम्भसङ्ख्या_2 = dims >= 1 ? size[dims - 1] : 1;
    size_t चरणः_2;
    यदि (dims >= 2)
    {
        पङ्क्तिकाः_2 = size[dims - 2];
        चरणः_2 = चरणः[dims - 2];
    }
    अन्यथा
    {
        पङ्क्तिकाः_2 = 1;
        चरणः_2 = स्तम्भसङ्ख्या_2 * elem_आकार();
    }

    प्रत्यागम GpuMat(पङ्क्तिकाः_2, स्तम्भसङ्ख्या_2, type(), getDevicePtr(), चरणः_2);
}

GpuMat GpuMatND::संक्रियकः()(सूचकांक_Array idx, Range rowRange, Range colRange) ध्रुव
{
    प्रत्यागम create_Gpu_Mat_शीर्षकम्(idx, rowRange, colRange).clone();
}

GpuMatND::संक्रियकः GpuMat() ध्रुव
{
    प्रत्यागम create_Gpu_Mat_शीर्षकम्().clone();
}

शून्य GpuMatND::समुच्चयः_Fields(MatShape आकार_4, पूर्णाङ्क _type, चरणः_Array चरणः_3)
{
    _type &= Mat::TYPE_आवरणम्;

    flags = Mat::MAGIC_VAL + _type;
    dims = static_cast<पूर्णाङ्क>(आकार_4.size());
    size = std::move(आकार_4);

    यदि (चरणः_3.empty())
    {
        चरणः = चरणः_Array(dims);

        चरणः.back() = elem_आकार();
        प्रति (पूर्णाङ्क _i = dims - 2; _i >= 0; --_i)
        {
            ध्रुव size_t i = _i;
            चरणः[i] = चरणः[i+1] * size[i+1];
        }

        flags |= Mat::CONTINUOUS_FLAG;
    }
    अन्यथा
    {
        चरणः = std::move(चरणः_3);
        यदि (चरणः.size() < size.size())
          चरणः.push_back(elem_आकार());

        flags = cv::updateContinuityFlag(flags, dims, size.दत्त(), चरणः.दत्त());
    }

    CV_Assert(size.size() == चरणः.size());
    CV_Assert(चरणः.back() == elem_आकार());
}

#यदि_अपरिभाषितम् HAVE_CUDA

Gpu_दत्त::Gpu_दत्त(ध्रुव size_t आकार_4)
    : दत्त(शून्यसूचकः), size(0)
{
    CV_अप्रयुक्तम्(आकार_4);
    throw_no_cuda();
}

Gpu_दत्त::~Gpu_दत्त()
{
}

शून्य GpuMatND::create(ध्रुव MatShape& _shape, पूर्णाङ्क _type)
{
    CV_अप्रयुक्तम्(_shape);
    CV_अप्रयुक्तम्(_type);
    throw_no_cuda();
}

शून्य GpuMatND::release()
{
    throw_no_cuda();
}

GpuMatND GpuMatND::clone() ध्रुव
{
    throw_no_cuda();
}

GpuMatND GpuMatND::clone(प्रवाहः& stream) ध्रुव
{
    CV_अप्रयुक्तम्(stream);
    throw_no_cuda();
}

शून्य GpuMatND::upload(निवेशः_Array src)
{
    CV_अप्रयुक्तम्(src);
    throw_no_cuda();
}

शून्य GpuMatND::upload(निवेशः_Array src, प्रवाहः& stream)
{
    CV_अप्रयुक्तम्(src);
    CV_अप्रयुक्तम्(stream);
    throw_no_cuda();
}

शून्य GpuMatND::download(OutputArray dst) ध्रुव
{
    CV_अप्रयुक्तम्(dst);
    throw_no_cuda();
}

शून्य GpuMatND::download(OutputArray dst, प्रवाहः& stream) ध्रुव
{
    CV_अप्रयुक्तम्(dst);
    CV_अप्रयुक्तम्(stream);
    throw_no_cuda();
}

शून्य GpuMatND::fit(ध्रुव MatShape& _shape, पूर्णाङ्क _type)
{
    स्वचालित elements_nonzero = [](ध्रुव MatShape& v)
    {
        प्रत्यागम std::all_of(v.begin(), v.end(), [](पूर्णाङ्क u){ प्रत्यागम u > 0; });
    };
    CV_Assert(!_shape.empty());
    CV_Assert(elements_nonzero(_shape));

    _type &= Mat::TYPE_आवरणम्;

    ध्रुव size_t esz = (size_t)CV_ELEM_आकार(_type);
    size_t step0 = esz;
    प्रति (पूर्णाङ्क i = (पूर्णाङ्क)_shape.size() - 2; i >= 0; --i)
        step0 *= (size_t)_shape[(size_t)i + 1];
    ध्रुव size_t required_अष्टकानि = (size_t)_shape[0] * step0;

    ध्रुव तार्किक canReuse = !empty() && !external() && isContinuous() && !isSubmatrix() && offset == 0;
    ध्रुव size_t old_क्षमता = canReuse ? योग_Mem_आकार() : 0;

    यदि (!canReuse || required_अष्टकानि > old_क्षमता)
    {
        create(_shape, _type);
        प्रत्यागम;
    }

    समुच्चयः_Fields(_shape, _type);
}

#यदि_अन्तः
