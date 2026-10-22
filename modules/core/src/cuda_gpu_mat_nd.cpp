// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#అన్తర్భావ "precomp.hpp"

ఉపయోగించి నామాకాశ cv;
ఉపయోగించి నామాకాశ cv::cuda;

GpuMatND::~GpuMatND() = అప్రమేయ;

GpuMatND::GpuMatND(స్థిరరాశి MatShape& _shape, పూర్ణాంకం _type, శూన్య* దత్తాంశం_3, అడుగు_Array అడుగు_3) :
    flags(0), dims(0), దత్తాంశం(static_cast<uchar*>(దత్తాంశం_3)), offset(0)
{
    CV_Assert(అడుగు_3.empty() || _shape.size() == అడుగు_3.size() + 1 ||
              (_shape.size() == అడుగు_3.size() && అడుగు_3.back() == (size_t)CV_ELEM_కొలత(_type)));

    సమితి_Fields(_shape, _type, std::move(అడుగు_3));
}

GpuMatND GpuMatND::క్రియాకారకం()(స్థిరరాశి std::vector<Range>& ranges) స్థిరరాశి
{
    CV_Assert(dims == (పూర్ణాంకం)ranges.size());

    కొరకు (పూర్ణాంకం i = 0; i < dims; ++i)
    {
        Range r = ranges[i];
        CV_Assert(r == Range::all() || (0 <= r.ప్రారంభం && r.ప్రారంభం < r.end && r.end <= size[i]));
    }

    GpuMatND ret = *ఇది;

    కొరకు (పూర్ణాంకం i = 0; i < dims; ++i)
    {
        Range r = ranges[i];
        ఒకవేళ (r != Range::all() && r != Range(0, ret.size[i]))
        {
            ret.offset += r.ప్రారంభం * ret.అడుగు[i];
            ret.size[i] = r.size();
            ret.flags |= Mat::SUBMATRIX_FLAG;
        }
    }

    ret.flags = cv::updateContinuityFlag(ret.flags, dims, ret.size.దత్తాంశం(), ret.అడుగు.దత్తాంశం());

    తిరిగి ret;
}

GpuMat GpuMatND::create_Gpu_Mat_శీర్షిక(సూచీ_Array idx, Range అడ్డువరుస_Range_2, Range నిలువు_Range_2) స్థిరరాశి
{
    CV_Assert((పూర్ణాంకం)idx.size() == dims - 2);

    std::vector<Range> ranges;
    కొరకు (పూర్ణాంకం i : idx)
        ranges.emplace_back(i, i+1);
    ranges.push_back(అడ్డువరుస_Range_2);
    ranges.push_back(నిలువు_Range_2);

    తిరిగి (*ఇది)(ranges).create_Gpu_Mat_శీర్షిక();
}

GpuMat GpuMatND::create_Gpu_Mat_శీర్షిక() స్థిరరాశి
{
    స్వయంచాలక Effectively2D = [](GpuMatND m)
    {
        కొరకు (పూర్ణాంకం i = 0; i < m.dims - 2; ++i)
            ఒకవేళ (m.size[i] > 1)
                తిరిగి అబద్ధం;
        తిరిగి నిజం;
    };
    CV_Assert(Effectively2D(*ఇది));
    పూర్ణాంకం అడ్డువరుసలు_2, నిలువుల_సంఖ్య_2 = dims >= 1 ? size[dims - 1] : 1;
    size_t అడుగు_2;
    ఒకవేళ (dims >= 2)
    {
        అడ్డువరుసలు_2 = size[dims - 2];
        అడుగు_2 = అడుగు[dims - 2];
    }
    లేకపోతే
    {
        అడ్డువరుసలు_2 = 1;
        అడుగు_2 = నిలువుల_సంఖ్య_2 * elem_కొలత();
    }

    తిరిగి GpuMat(అడ్డువరుసలు_2, నిలువుల_సంఖ్య_2, type(), getDevicePtr(), అడుగు_2);
}

GpuMat GpuMatND::క్రియాకారకం()(సూచీ_Array idx, Range అడ్డువరుస_Range_2, Range నిలువు_Range_2) స్థిరరాశి
{
    తిరిగి create_Gpu_Mat_శీర్షిక(idx, అడ్డువరుస_Range_2, నిలువు_Range_2).clone();
}

GpuMatND::క్రియాకారకం GpuMat() స్థిరరాశి
{
    తిరిగి create_Gpu_Mat_శీర్షిక().clone();
}

శూన్య GpuMatND::సమితి_Fields(MatShape కొలత_4, పూర్ణాంకం _type, అడుగు_Array అడుగు_3)
{
    _type &= Mat::TYPE_ముసుగు;

    flags = Mat::MAGIC_VAL + _type;
    dims = static_cast<పూర్ణాంకం>(కొలత_4.size());
    size = std::move(కొలత_4);

    ఒకవేళ (అడుగు_3.empty())
    {
        అడుగు = అడుగు_Array(dims);

        అడుగు.back() = elem_కొలత();
        కొరకు (పూర్ణాంకం _i = dims - 2; _i >= 0; --_i)
        {
            స్థిరరాశి size_t i = _i;
            అడుగు[i] = అడుగు[i+1] * size[i+1];
        }

        flags |= Mat::CONTINUOUS_FLAG;
    }
    లేకపోతే
    {
        అడుగు = std::move(అడుగు_3);
        ఒకవేళ (అడుగు.size() < size.size())
          అడుగు.push_back(elem_కొలత());

        flags = cv::updateContinuityFlag(flags, dims, size.దత్తాంశం(), అడుగు.దత్తాంశం());
    }

    CV_Assert(size.size() == అడుగు.size());
    CV_Assert(అడుగు.back() == elem_కొలత());
}

#నిర్వచితం_కాకపోతే HAVE_CUDA

Gpu_దత్తాంశం::Gpu_దత్తాంశం(స్థిరరాశి size_t కొలత_4)
    : దత్తాంశం(శూన్యసూచిక), size(0)
{
    CV_వాడని(కొలత_4);
    throw_no_cuda();
}

Gpu_దత్తాంశం::~Gpu_దత్తాంశం()
{
}

శూన్య GpuMatND::create(స్థిరరాశి MatShape& _shape, పూర్ణాంకం _type)
{
    CV_వాడని(_shape);
    CV_వాడని(_type);
    throw_no_cuda();
}

శూన్య GpuMatND::release()
{
    throw_no_cuda();
}

GpuMatND GpuMatND::clone() స్థిరరాశి
{
    throw_no_cuda();
}

GpuMatND GpuMatND::clone(ప్రవాహం& stream) స్థిరరాశి
{
    CV_వాడని(stream);
    throw_no_cuda();
}

శూన్య GpuMatND::upload(ఆదానం_Array src)
{
    CV_వాడని(src);
    throw_no_cuda();
}

శూన్య GpuMatND::upload(ఆదానం_Array src, ప్రవాహం& stream)
{
    CV_వాడని(src);
    CV_వాడని(stream);
    throw_no_cuda();
}

శూన్య GpuMatND::download(OutputArray dst) స్థిరరాశి
{
    CV_వాడని(dst);
    throw_no_cuda();
}

శూన్య GpuMatND::download(OutputArray dst, ప్రవాహం& stream) స్థిరరాశి
{
    CV_వాడని(dst);
    CV_వాడని(stream);
    throw_no_cuda();
}

శూన్య GpuMatND::fit(స్థిరరాశి MatShape& _shape, పూర్ణాంకం _type)
{
    స్వయంచాలక elements_nonzero = [](స్థిరరాశి MatShape& v)
    {
        తిరిగి std::all_of(v.begin(), v.end(), [](పూర్ణాంకం u){ తిరిగి u > 0; });
    };
    CV_Assert(!_shape.empty());
    CV_Assert(elements_nonzero(_shape));

    _type &= Mat::TYPE_ముసుగు;

    స్థిరరాశి size_t esz = (size_t)CV_ELEM_కొలత(_type);
    size_t step0 = esz;
    కొరకు (పూర్ణాంకం i = (పూర్ణాంకం)_shape.size() - 2; i >= 0; --i)
        step0 *= (size_t)_shape[(size_t)i + 1];
    స్థిరరాశి size_t required_బైట్లు = (size_t)_shape[0] * step0;

    స్థిరరాశి తార్కిక canReuse = !empty() && !external() && isContinuous() && !isSubmatrix() && offset == 0;
    స్థిరరాశి size_t old_సామర్థ్యం = canReuse ? మొత్తం_Mem_కొలత() : 0;

    ఒకవేళ (!canReuse || required_బైట్లు > old_సామర్థ్యం)
    {
        create(_shape, _type);
        తిరిగి;
    }

    సమితి_Fields(_shape, _type);
}

#అయితే_ముగింపు
