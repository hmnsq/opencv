// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#అన్తర్భావ "precomp.hpp"
#అన్తర్భావ "bufferpool.impl.hpp"

నామాకాశ cv {

std::string layout_గమ్యం_String(దత్తాంశం_Layout layout)
{
    తిరిగి
        layout == దత్తాంశం_LAYOUT_ND ? "ND" :
        layout == దత్తాంశం_LAYOUT_NCHW ? "NCHW" :
        layout == దత్తాంశం_LAYOUT_NHWC ? "NHWC" :
        layout == దత్తాంశం_LAYOUT_ఖండం ? "BLOCK" :
        layout == దత్తాంశం_LAYOUT_NCDHW ? "NCDHW" :
        layout == దత్తాంశం_LAYOUT_NDHWC ? "NDHWC" :
        layout == దత్తాంశం_LAYOUT_PLANAR ? "PLANAR" :
        layout == దత్తాంశం_LAYOUT_UNKNOWN ? "Unknown" : "???";
}

తార్కిక క్రియాకారకం == (స్థిరరాశి MatShape& size1, స్థిరరాశి MatShape& size2)
{
    ఒకవేళ (size1.dims != size2.dims)
        తిరిగి అబద్ధం;
    ఒకవేళ (size1.layout != size2.layout &&
        size1.layout != దత్తాంశం_LAYOUT_UNKNOWN &&
        size2.layout != దత్తాంశం_LAYOUT_UNKNOWN)
        తిరిగి అబద్ధం;
    ఒకవేళ (size1.layout == దత్తాంశం_LAYOUT_ఖండం &&
        size2.layout == దత్తాంశం_LAYOUT_ఖండం &&
        size1.C != size2.C)
        తిరిగి అబద్ధం;
    కొరకు (పూర్ణాంకం i = 0; i < size1.dims; i++) {
        ఒకవేళ (size1.p[i] != size2.p[i])
            తిరిగి అబద్ధం;
    }
    తిరిగి నిజం;
}

తార్కిక క్రియాకారకం != (స్థిరరాశి MatShape& size1, స్థిరరాశి MatShape& size2)
{
    తిరిగి !(size1 == size2);
}

/////////////////////////// MatShape ////////////////////////////////

MatShape MatShape::scalar()
{
    తిరిగి MatShape(0);
}

శూన్య MatShape::clear()
{
    dims = -1;
    layout = దత్తాంశం_LAYOUT_UNKNOWN;
    C = 0;
    కొరకు (పూర్ణాంకం i = 0; i < MAX_DIMS; i++)
        p[i] = 0;
}

శూన్య MatShape::resize(size_t new_కొలత, పూర్ణాంకం value)
{
    CV_Assert(new_కొలత < (size_t)MAX_DIMS);
    పూర్ణాంకం old_dims = std::max(dims, 0);
    dims = (పూర్ణాంకం)new_కొలత;
    కొరకు (పూర్ణాంకం i = old_dims; i < dims; i++)
        p[i] = value;
}

శూన్య MatShape::reserve(size_t)
{
    // no op; maybe need to add a check for overflow, but we check it anyway in other operations
}

శూన్య MatShape::assign(size_t new_కొలత, పూర్ణాంకం value)
{
    CV_Assert(new_కొలత < (size_t)MAX_DIMS);
    dims = (పూర్ణాంకం)new_కొలత;
    కొరకు (పూర్ణాంకం i = 0; i < dims; i++)
        p[i] = value;
}

శూన్య MatShape::assign(పూర్ణాంకం new_కొలత, పూర్ణాంకం value)
{
    assign((size_t)new_కొలత, value);
}

శూన్య MatShape::assign(స్థిరరాశి పూర్ణాంకం* begin, స్థిరరాశి పూర్ణాంకం* end)
{
    కేటాయించు(begin, end);
}

శూన్య MatShape::కేటాయించు(స్థిరరాశి పూర్ణాంకం* begin, స్థిరరాశి పూర్ణాంకం* end)
{
    ptrdiff_t new_కొలత = end - begin;
    CV_Assert(0 <= new_కొలత && new_కొలత < (ptrdiff_t)MAX_DIMS);
    dims = (పూర్ణాంకం)new_కొలత;
    కొరకు (పూర్ణాంకం i = 0; i < dims; i++)
        p[i] = begin[i];
}

పూర్ణాంకం* MatShape::begin() { తిరిగి p; }
స్థిరరాశి పూర్ణాంకం* MatShape::begin() స్థిరరాశి { తిరిగి p; }
పూర్ణాంకం* MatShape::end() { తిరిగి p + std::max(dims, 0); }
స్థిరరాశి పూర్ణాంకం* MatShape::end() స్థిరరాశి { తిరిగి p + std::max(dims, 0); }
పూర్ణాంకం& MatShape::back() { తిరిగి p[std::max(dims-1, 0)]; }
స్థిరరాశి పూర్ణాంకం& MatShape::back() స్థిరరాశి { తిరిగి p[std::max(dims-1, 0)]; }

శూన్య MatShape::push_back(పూర్ణాంకం value)
{
    CV_Assert(dims+1 < MAX_DIMS);
    dims = std::max(dims+1, 1);
    p[dims-1] = value;
}

శూన్య MatShape::emplace_back(పూర్ణాంకం value)
{
    push_back(value);
}

శూన్య MatShape::insert(పూర్ణాంకం* where, పూర్ణాంకం value)
{
    పూర్ణాంకం old_dims = std::max(dims, 0);
    CV_Assert(old_dims+1 < MAX_DIMS);
    ptrdiff_t ofs = where - p;
    CV_Assert(0 <= ofs && ofs <= old_dims);
    dims = old_dims+1;
    కొరకు (పూర్ణాంకం i = old_dims-1; i >= (పూర్ణాంకం)ofs; i--)
        p[i+1] = p[i];
    p[ofs] = value;
}

శూన్య MatShape::insert(పూర్ణాంకం* where, size_t లెక్క_2, పూర్ణాంకం value)
{
    పూర్ణాంకం old_dims = std::max(dims, 0);
    CV_Assert((size_t)(old_dims+లెక్క_2) < (size_t)MAX_DIMS);
    ptrdiff_t ofs = where - p;
    CV_Assert(0 <= ofs && ofs <= old_dims);
    dims = (పూర్ణాంకం)(old_dims+లెక్క_2);
    కొరకు (పూర్ణాంకం i = old_dims-1; i >= (పూర్ణాంకం)ofs; i--)
        p[i+లెక్క_2] = p[i];
    కొరకు (పూర్ణాంకం i = 0; i < (పూర్ణాంకం)లెక్క_2; i++)
        p[i+ofs] = value;
}

శూన్య MatShape::insert(పూర్ణాంకం* where, పూర్ణాంకం లెక్క_2, పూర్ణాంకం value)
{
    insert(where, (size_t)లెక్క_2, value);
}

శూన్య MatShape::insert(పూర్ణాంకం* where, స్థిరరాశి పూర్ణాంకం* begin, స్థిరరాశి పూర్ణాంకం* end)
{
    చొప్పించు(where, begin, end);
}

శూన్య MatShape::చొప్పించు(పూర్ణాంకం* where, స్థిరరాశి పూర్ణాంకం* begin, స్థిరరాశి పూర్ణాంకం* end)
{
    పూర్ణాంకం old_dims = std::max(dims, 0);
    ptrdiff_t delta = end - begin;
    CV_Assert(0 <= delta && old_dims+delta < MAX_DIMS);
    ptrdiff_t ofs = where - p;
    CV_Assert(0 <= ofs && ofs <= old_dims);
    dims = (పూర్ణాంకం)(old_dims+delta);
    కొరకు (పూర్ణాంకం i = old_dims-1; i >= (పూర్ణాంకం)ofs; i--)
        p[i+delta] = p[i];
    కొరకు (పూర్ణాంకం i = 0; i < (పూర్ణాంకం)delta; i++)
        p[i+ofs] = begin[i];
}

శూన్య MatShape::erase(పూర్ణాంకం* where)
{
    CV_Assert(dims > 0);
    ptrdiff_t ofs = where - p;
    CV_Assert(0 <= ofs && ofs <= dims);
    ఒకవేళ (ofs == dims)
        తిరిగి;
    dims--;
    కొరకు (పూర్ణాంకం i = (పూర్ణాంకం)ofs+1; i <= dims; i++)
        p[i-1] = p[i];
}

size_t MatShape::మొత్తం() స్థిరరాశి
{
    size_t ఫలితం = 1;
    ఒకవేళ (dims < 0)
        తిరిగి 0;
    కొరకు (పూర్ణాంకం i = 0; i < dims; i++)
        ఫలితం *= p[i];
    తిరిగి ఫలితం;
}

std::vector<పూర్ణాంకం> MatShape::vec() స్థిరరాశి
{
    ఒకవేళ (dims < 0)
        తిరిగి std::vector<పూర్ణాంకం>(1, 0);
    తిరిగి std::vector<పూర్ణాంకం>(p, p + dims);
}

std::string MatShape::str() స్థిరరాశి
{
    std::stringstream sstrm;
    ఒకవేళ (empty()) {
        sstrm << "<empty>";
    } లేకపోతే ఒకవేళ (dims == 0) {
        sstrm << "<scalar>";
    } లేకపోతే {
        sstrm << "[";
        కొరకు (పూర్ణాంకం i = 0; i < dims; i++) {
            sstrm << (i > 0 ? " x " : "") << p[i];
        }
        sstrm << "]";
    }
    తిరిగి sstrm.str();
}

స్థిర శూన్య finalize_ఖండం_Layout(MatShape& size, పూర్ణాంకం C=0)
{
    ఒకవేళ (size.layout == దత్తాంశం_LAYOUT_ఖండం) {
        CV_Assert(size.dims >= 4);
        పూర్ణాంకం C0 = size.p[size.dims-1];
        CV_Assert(C0 > 1 && (C0 & (C0-1)) == 0);
        size.C = C > 0 ? C : size.p[1]*size.p[size.dims-1];
    } లేకపోతే {
        size.C = 0;
    }
    కొరకు (పూర్ణాంకం i = std::max(size.dims, 0); i < MatShape::MAX_DIMS; i++)
        size.p[i] = 0;
    ఒకవేళ (size.dims == 0)
        size.p[0] = 1;
}

MatShape::MatShape()
{
    clear();
}

MatShape::MatShape(size_t dims_, స్థిరరాశి పూర్ణాంకం* కొలత_3, దత్తాంశం_Layout layout_, పూర్ణాంకం C_)
{
    layout = layout_;
    CV_Assert(dims_ <= (size_t)MAX_DIMS);
    dims = (పూర్ణాంకం)dims_;
    కొరకు (పూర్ణాంకం i = 0; i < dims; i++) {
        p[i] = కొలత_3 ? కొలత_3[i] : 0;
    }
    finalize_ఖండం_Layout(*ఇది, C_);
}

MatShape::MatShape(size_t dims_, పూర్ణాంకం value, దత్తాంశం_Layout layout_)
{
    layout = layout_;
    CV_Assert(dims_ <= (size_t)MAX_DIMS);
    dims = (పూర్ణాంకం)dims_;
    కొరకు (పూర్ణాంకం i = 0; i < dims; i++) {
        p[i] = value;
    }
    finalize_ఖండం_Layout(*ఇది, 0);
}

MatShape::MatShape(std::initializer_list<పూర్ణాంకం> shape)
{
    layout = దత్తాంశం_LAYOUT_UNKNOWN;
    CV_Assert(shape.size() <= (size_t)MAX_DIMS);
    dims = (పూర్ణాంకం)shape.size();
    స్వయంచాలక it = shape.begin();
    కొరకు (పూర్ణాంకం i = 0; i < dims; i++, ++it) {
        p[i] = *it;
    }
    finalize_ఖండం_Layout(*ఇది, 0);
}

MatShape::MatShape(పూర్ణాంకం dims_, పూర్ణాంకం value, దత్తాంశం_Layout layout_)
{
    layout = layout_;
    CV_Assert(dims_ <= MAX_DIMS);
    dims = dims_;
    కొరకు (పూర్ణాంకం i = 0; i < dims; i++) {
        p[i] = value;
    }
    finalize_ఖండం_Layout(*ఇది, 0);
}

MatShape::MatShape(స్థిరరాశి std::vector<పూర్ణాంకం>& shape_, దత్తాంశం_Layout layout_, పూర్ణాంకం C_)
{
    layout = layout_;
    size_t shape_కొలత = shape_.size();
    CV_Assert(shape_కొలత < (size_t)MAX_DIMS);
    dims = (పూర్ణాంకం)shape_కొలత;
    కొరకు (పూర్ణాంకం i = 0; i < dims; i++) {
        p[i] = shape_[i];
    }
    finalize_ఖండం_Layout(*ఇది, C_);
}

MatShape::MatShape(స్థిరరాశి పూర్ణాంకం* begin, స్థిరరాశి పూర్ణాంకం* end, దత్తాంశం_Layout layout_, పూర్ణాంకం C_)
{
    layout = layout_;
    ptrdiff_t shape_కొలత = end - begin;
    CV_Assert(0 <= shape_కొలత && shape_కొలత < MAX_DIMS);
    dims = (పూర్ణాంకం)shape_కొలత;
    కొరకు (పూర్ణాంకం i = 0; i < dims; i++) {
        p[i] = begin[i];
    }
    finalize_ఖండం_Layout(*ఇది, C_);
}

MatShape::MatShape(స్థిరరాశి MatShape& shape)
{
    dims = shape.dims;
    layout = shape.layout;
    C = shape.C;
    కొరకు (పూర్ణాంకం i = 0; i < MAX_DIMS; i++)
        p[i] = shape.p[i];
}

MatShape& MatShape::క్రియాకారకం = (స్థిరరాశి MatShape& shape)
{
    ఒకవేళ (ఇది != &shape) {
        dims = shape.dims;
        layout = shape.layout;
        C = shape.C;
        కొరకు (పూర్ణాంకం i = 0; i < MAX_DIMS; i++)
            p[i] = shape.p[i];
    }
    తిరిగి *ఇది;
}

తార్కిక MatShape::hasSymbols() స్థిరరాశి
{
    కొరకు (పూర్ణాంకం i = 0; i < dims; i++) {
        ఒకవేళ (p[i] < 0)
            తిరిగి నిజం;
    }
    తిరిగి అబద్ధం;
}

పూర్ణాంకం MatShape::channels() స్థిరరాశి
{
    CV_Assert(layout == దత్తాంశం_LAYOUT_ఖండం || layout == దత్తాంశం_LAYOUT_NCHW || layout == దత్తాంశం_LAYOUT_NHWC);
    తిరిగి layout == దత్తాంశం_LAYOUT_ఖండం ? C : p[layout == దత్తాంశం_LAYOUT_NCHW ? 1 : dims-1];
}

MatShape MatShape::గమ్యం_Layout(దత్తాంశం_Layout newLayout, పూర్ణాంకం C0) స్థిరరాశి
{
    CV_Assert(layout == దత్తాంశం_LAYOUT_ఖండం || layout == దత్తాంశం_LAYOUT_NCHW || layout == దత్తాంశం_LAYOUT_NHWC);
    CV_Assert(newLayout == దత్తాంశం_LAYOUT_ఖండం || newLayout == దత్తాంశం_LAYOUT_NCHW || newLayout == దత్తాంశం_LAYOUT_NHWC);

    MatShape newsize = *ఇది;
    newsize.layout = newLayout;

    ఒకవేళ (newLayout == దత్తాంశం_LAYOUT_ఖండం) {
        // any => BLOCK
        CV_Assert_N(C0 > 1, (C0 & (C0-1)) == 0);
        పూర్ణాంకం Corig = channels();
        newsize.C = Corig;

        ఒకవేళ (layout == దత్తాంశం_LAYOUT_NHWC) {
            కొరకు (పూర్ణాంకం i = 2; i < dims; i++) {
                newsize.p[i] = p[i-1];
            }
        }

        newsize.dims += layout != దత్తాంశం_LAYOUT_ఖండం;
        newsize.p[1] = (Corig + C0 - 1)/C0;
        newsize.p[newsize.dims-1] = C0;
    } లేకపోతే ఒకవేళ (layout == దత్తాంశం_LAYOUT_ఖండం) {
        // BLOCK => any (except for BLOCK, which is handled above)
        newsize.C = 0;
        ఒకవేళ (newLayout == దత్తాంశం_LAYOUT_NHWC) {
            కొరకు (పూర్ణాంకం i = 2; i < dims; i++) {
                newsize.p[i-1] = p[i];
            }
        }
        newsize.p[newLayout == దత్తాంశం_LAYOUT_NCHW ? 1 : dims-2] = C;
        newsize.dims--;
    } లేకపోతే {
        CV_Assert_N(C0 <= 1);

        ఒకవేళ (newLayout == layout)
            తిరిగి newsize;

        // NHWC => NCHW
        ఒకవేళ (newLayout == దత్తాంశం_LAYOUT_NCHW) {
            కొరకు (పూర్ణాంకం i = 2; i < dims; i++)
                newsize.p[i] = p[i-1];
            newsize.p[1] = p[dims-1];
        } లేకపోతే {
            // NCHW => NHWC
            కొరకు (పూర్ణాంకం i = 2; i < dims; i++)
                newsize.p[i-1] = p[i];
            newsize.p[dims-1] = p[1];
        }
    }

    తిరిగి newsize;
}

స్థిర పూర్ణాంకం normalizeShapeAxis(పూర్ణాంకం axis, పూర్ణాంకం dims)
{
    CV_CheckGE(axis, -dims, "axis is out of range");
    CV_CheckLT(axis, dims, "axis is out of range");
    తిరిగి axis < 0 ? axis + dims : axis;
}

// The semantic NCHW form {N, C, spatial...} of a block ('NC1HWC0') shape, so that axes and
// element counts are the logical ones rather than the C0-padded ones.
స్థిర MatShape గమ్యం_Semantic(స్థిరరాశి MatShape& shape)
{
    CV_CheckGE(shape.dims, 3, "a block shape is {N, C1, spatial..., C0}");
    తిరిగి shape.గమ్యం_Layout(దత్తాంశం_LAYOUT_NCHW);
}

// Blocks such a result again. Once N or C changes, elements cross the padded C0 lanes and
// the data needs a repack, which a shape operation cannot express.
స్థిర MatShape గమ్యం_ఖండం_యొక్క(MatShape ఫలితం, స్థిరరాశి MatShape& src, స్థిరరాశి అక్షరం* op)
{
    ఒకవేళ (ఫలితం.dims < 2 || ఫలితం.p[0] != src.p[0] || ఫలితం.p[1] != src.C)
        CV_Error_(Error::StsNotImplemented,
                  ("%s: the block layout is kept only when the batch and the channel axes stay "
                   "unchanged; convert the shape with toLayout() first", op));
    CV_CheckLE(ఫలితం.dims + 1, (పూర్ణాంకం)MatShape::MAX_DIMS, "too many dimensions");
    ఫలితం.layout = దత్తాంశం_LAYOUT_NCHW;
    తిరిగి ఫలితం.గమ్యం_Layout(దత్తాంశం_LAYOUT_ఖండం, src.p[src.dims-1]);
}

MatShape MatShape::squeeze(స్థిరరాశి std::vector<పూర్ణాంకం>& axes) స్థిరరాశి
{
    CV_Assert(dims >= 0);
    ఒకవేళ (layout == దత్తాంశం_LAYOUT_ఖండం)
        తిరిగి గమ్యం_ఖండం_యొక్క(గమ్యం_Semantic(*ఇది).squeeze(axes), *ఇది, "squeeze");
    తార్కిక remove[MAX_DIMS] = {అబద్ధం};
    ఒకవేళ (axes.empty())
    {
        కొరకు (పూర్ణాంకం i = 0; i < dims; i++)
            remove[i] = p[i] == 1;
    }
    కొరకు (పూర్ణాంకం a : axes)
    {
        పూర్ణాంకం i = normalizeShapeAxis(a, dims);
        CV_Check(a, !remove[i], "squeeze: duplicate axis");
        CV_Check(p[i], p[i] == 1, "squeeze: the size of a squeezed axis must be 1");
        remove[i] = నిజం;
    }
    // a new number of axes: the layout of this shape does not apply
    MatShape ఫలితం;
    ఫలితం.dims = 0;
    కొరకు (పూర్ణాంకం i = 0; i < dims; i++)
        ఒకవేళ (!remove[i])
            ఫలితం.p[ఫలితం.dims++] = p[i];
    తిరిగి ఫలితం;
}

MatShape MatShape::unsqueeze(స్థిరరాశి std::vector<పూర్ణాంకం>& axes) స్థిరరాశి
{
    CV_Assert(dims >= 0);
    ఒకవేళ (layout == దత్తాంశం_LAYOUT_ఖండం)
        తిరిగి గమ్యం_ఖండం_యొక్క(గమ్యం_Semantic(*ఇది).unsqueeze(axes), *ఇది, "unsqueeze");
    స్థిరరాశి పూర్ణాంకం బహిర్గతం_Dims = dims + (పూర్ణాంకం)axes.size();
    CV_CheckLE(బహిర్గతం_Dims, (పూర్ణాంకం)MAX_DIMS, "unsqueeze: too many dimensions");
    తార్కిక insert[MAX_DIMS] = {అబద్ధం};
    కొరకు (పూర్ణాంకం a : axes)
    {
        పూర్ణాంకం i = normalizeShapeAxis(a, బహిర్గతం_Dims);
        CV_Check(a, !insert[i], "unsqueeze: duplicate axis");
        insert[i] = నిజం;
    }
    // a new number of axes: the layout of this shape does not apply
    MatShape ఫలితం;
    ఫలితం.dims = బహిర్గతం_Dims;
    కొరకు (పూర్ణాంకం i = 0, j = 0; i < బహిర్గతం_Dims; i++)
        ఫలితం.p[i] = insert[i] ? 1 : p[j++];
    తిరిగి ఫలితం;
}

MatShape MatShape::flatten(పూర్ణాంకం ప్రారంభం_Axis, పూర్ణాంకం చివర_Axis) స్థిరరాశి
{
    CV_Assert(dims >= 0);
    ఒకవేళ (layout == దత్తాంశం_LAYOUT_ఖండం)
        తిరిగి గమ్యం_ఖండం_యొక్క(గమ్యం_Semantic(*ఇది).flatten(ప్రారంభం_Axis, చివర_Axis), *ఇది, "flatten");
    ఒకవేళ (dims == 0)
    {
        CV_Check(ప్రారంభం_Axis, ప్రారంభం_Axis == 0 || ప్రారంభం_Axis == -1, "flatten: axis is out of range");
        CV_Check(చివర_Axis, చివర_Axis == 0 || చివర_Axis == -1, "flatten: axis is out of range");
        తిరిగి MatShape({1});
    }
    పూర్ణాంకం ప్రారంభం = normalizeShapeAxis(ప్రారంభం_Axis, dims), end = normalizeShapeAxis(చివర_Axis, dims);
    CV_CheckLE(ప్రారంభం, end, "flatten: startAxis must not follow endAxis");
    int64 merged = 1;
    కొరకు (పూర్ణాంకం i = ప్రారంభం; i <= end; i++)
        merged *= p[i];
    CV_CheckLE((ద్విగుణ)merged, (ద్విగుణ)గరిష్ఠ_పూర్ణాంకం, "flatten: the result is too big");
    // a new number of axes: the layout of this shape does not apply
    MatShape ఫలితం;
    ఫలితం.dims = 0;
    కొరకు (పూర్ణాంకం i = 0; i < dims; i++)
    {
        ఒకవేళ (i == ప్రారంభం)
            ఫలితం.p[ఫలితం.dims++] = (పూర్ణాంకం)merged;
        లేకపోతే ఒకవేళ (i < ప్రారంభం || i > end)
            ఫలితం.p[ఫలితం.dims++] = p[i];
    }
    తిరిగి ఫలితం;
}

MatShape MatShape::reshape(స్థిరరాశి MatShape& newShape, తార్కిక allowZero) స్థిరరాశి
{
    CV_Assert(dims >= 0 && newShape.dims >= 0);
    ఒకవేళ (layout == దత్తాంశం_LAYOUT_ఖండం)
        తిరిగి గమ్యం_ఖండం_యొక్క(గమ్యం_Semantic(*ఇది).reshape(newShape, allowZero), *ఇది, "reshape");
    // a new number of axes: neither the layout of this shape nor the one of the spec applies
    MatShape ఫలితం;
    ఫలితం.dims = newShape.dims;
    std::copy(newShape.p, newShape.p + newShape.dims, ఫలితం.p);
    పూర్ణాంకం inferred = -1;
    int64 తెలిసిన = 1;
    కొరకు (పూర్ణాంకం i = 0; i < ఫలితం.dims; i++)
    {
        పూర్ణాంకం& sz = ఫలితం.p[i];
        ఒకవేళ (sz == -1)
        {
            CV_Check(i, inferred < 0, "reshape: at most one axis may be -1");
            inferred = i;
            కొనసాగు;
        }
        CV_Check(sz, sz >= 0, "reshape: sizes must be -1 or non-negative");
        ఒకవేళ (sz == 0 && !allowZero)
        {
            CV_Check(i, i < dims, "reshape: 0 refers to an axis the source does not have");
            sz = p[i];
        }
        తెలిసిన *= sz;
    }
    స్థిరరాశి int64 మొత్తం_2 = (int64)మొత్తం();
    ఒకవేళ (inferred >= 0)
    {
        CV_Check((ద్విగుణ)తెలిసిన, తెలిసిన != 0, "reshape: cannot infer the -1 axis next to a zero-size axis");
        CV_Check((ద్విగుణ)మొత్తం_2, మొత్తం_2 % తెలిసిన == 0, "reshape: the number of elements does not match");
        CV_CheckLE((ద్విగుణ)(మొత్తం_2/తెలిసిన), (ద్విగుణ)గరిష్ఠ_పూర్ణాంకం, "reshape: the result is too big");
        ఫలితం.p[inferred] = (పూర్ణాంకం)(మొత్తం_2/తెలిసిన);
    }
    లేకపోతే ఒకవేళ (తెలిసిన != మొత్తం_2)
        CV_Error_(Error::StsBadArg, ("reshape: the number of elements does not match: %lld vs %lld",
                                     (దీర్ఘ దీర్ఘ)తెలిసిన, (దీర్ఘ దీర్ఘ)మొత్తం_2));
    తిరిగి ఫలితం;
}

MatShape MatShape::expand(స్థిరరాశి MatShape& another) స్థిరరాశి
{
    ఒకవేళ (dims == 0)
        తిరిగి another;
    ఒకవేళ (another.dims == 0)
        తిరిగి *ఇది;

    ఒకవేళ ((layout == దత్తాంశం_LAYOUT_NCHW || layout == దత్తాంశం_LAYOUT_NHWC) &&
        (another.layout == దత్తాంశం_LAYOUT_NCHW || another.layout == దత్తాంశం_LAYOUT_NHWC)) {
        CV_Assert(layout == another.layout);
    }
    // [TODO] support block layout
    CV_Assert(layout != దత్తాంశం_LAYOUT_ఖండం && another.layout != దత్తాంశం_LAYOUT_ఖండం);

    MatShape ఫలితం;

    ఒకవేళ (dims < 0 || another.dims < 0)
        తిరిగి ఫలితం;

    ఫలితం = *ఇది;
    ఫలితం.dims = std::max(dims, another.dims);
    ఫలితం.layout = layout == దత్తాంశం_LAYOUT_UNKNOWN ? another.layout :
        layout == దత్తాంశం_LAYOUT_ND && (another.layout == దత్తాంశం_LAYOUT_NCHW ||
        another.layout == దత్తాంశం_LAYOUT_NHWC) ? another.layout : layout;
    కొరకు (పూర్ణాంకం i = ఫలితం.dims-1; i >= 0; i--) {
        పూర్ణాంకం i1 = i - (ఫలితం.dims - dims);
        పూర్ణాంకం i2 = i - (ఫలితం.dims - another.dims);
        పూర్ణాంకం sz1 = i1 < 0 ? 1 : p[i1];
        పూర్ణాంకం sz2 = i2 < 0 ? 1 : another.p[i2];
        CV_Assert(sz1 == sz2 || sz1 == 1 || sz2 == 1);
        // [TODO] handle symbolic shapes
        ఫలితం.p[i] = sz1 != 1 ? sz1 : sz2;
    }
    తిరిగి ఫలితం;
}

/////////////////////////// MatAllocator ////////////////////////////

శూన్య MatAllocator::map(UMat_దత్తాంశం*, AccessFlag) స్థిరరాశి
{
}

శూన్య MatAllocator::unmap(UMat_దత్తాంశం* u) స్థిరరాశి
{
    ఒకవేళ(u->urefcount == 0 && u->refcount == 0)
    {
        deallocate(u);
    }
}

శూన్య MatAllocator::download(UMat_దత్తాంశం* u, శూన్య* dstptr,
         పూర్ణాంకం dims, స్థిరరాశి size_t sz[],
         స్థిరరాశి size_t srcofs[], స్థిరరాశి size_t srcstep[],
         స్థిరరాశి size_t dststep[]) స్థిరరాశి
{
    ఒకవేళ(!u)
        తిరిగి;
    పూర్ణాంకం isz[CV_MAX_DIM];
    uchar* srcptr = u->దత్తాంశం;
    కొరకు( పూర్ణాంకం i = 0; i < dims; i++ )
    {
        CV_Assert( sz[i] <= (size_t)గరిష్ఠ_పూర్ణాంకం );
        ఒకవేళ( sz[i] == 0 )
            తిరిగి;
        ఒకవేళ( srcofs )
            srcptr += srcofs[i]*(i <= dims-2 ? srcstep[i] : 1);
        isz[i] = (పూర్ణాంకం)sz[i];
    }

    Mat src(dims, isz, CV_8U, srcptr, srcstep);
    Mat dst(dims, isz, CV_8U, dstptr, dststep);

    స్థిరరాశి Mat* arrays[] = { &src, &dst };
    uchar* ptrs[2];
    NAryMatIterator it(arrays, ptrs, 2);
    size_t planesz = it.size;

    కొరకు( size_t j = 0; j < it.nplanes; j++, ++it )
        జ్ఞాపకం_నకలు(ptrs[1], ptrs[0], planesz);
}


శూన్య MatAllocator::upload(UMat_దత్తాంశం* u, స్థిరరాశి శూన్య* srcptr, పూర్ణాంకం dims, స్థిరరాశి size_t sz[],
                    స్థిరరాశి size_t dstofs[], స్థిరరాశి size_t dststep[],
                    స్థిరరాశి size_t srcstep[]) స్థిరరాశి
{
    ఒకవేళ(!u)
        తిరిగి;
    పూర్ణాంకం isz[CV_MAX_DIM];
    uchar* dstptr = u->దత్తాంశం;
    కొరకు( పూర్ణాంకం i = 0; i < dims; i++ )
    {
        CV_Assert( sz[i] <= (size_t)గరిష్ఠ_పూర్ణాంకం );
        ఒకవేళ( sz[i] == 0 )
            తిరిగి;
        ఒకవేళ( dstofs )
            dstptr += dstofs[i]*(i <= dims-2 ? dststep[i] : 1);
        isz[i] = (పూర్ణాంకం)sz[i];
    }

    Mat src(dims, isz, CV_8U, (శూన్య*)srcptr, srcstep);
    Mat dst(dims, isz, CV_8U, dstptr, dststep);

    స్థిరరాశి Mat* arrays[] = { &src, &dst };
    uchar* ptrs[2];
    NAryMatIterator it(arrays, ptrs, 2);
    size_t planesz = it.size;

    కొరకు( size_t j = 0; j < it.nplanes; j++, ++it )
        జ్ఞాపకం_నకలు(ptrs[1], ptrs[0], planesz);
}

శూన్య MatAllocator::copy(UMat_దత్తాంశం* usrc, UMat_దత్తాంశం* udst, పూర్ణాంకం dims, స్థిరరాశి size_t sz[],
                  స్థిరరాశి size_t srcofs[], స్థిరరాశి size_t srcstep[],
                  స్థిరరాశి size_t dstofs[], స్థిరరాశి size_t dststep[], తార్కిక /*sync*/) స్థిరరాశి
{
    CV_INSTRUMENT_REGION();

    ఒకవేళ(!usrc || !udst)
        తిరిగి;
    పూర్ణాంకం isz[CV_MAX_DIM];
    uchar* srcptr = usrc->దత్తాంశం;
    uchar* dstptr = udst->దత్తాంశం;
    కొరకు( పూర్ణాంకం i = 0; i < dims; i++ )
    {
        CV_Assert( sz[i] <= (size_t)గరిష్ఠ_పూర్ణాంకం );
        ఒకవేళ( sz[i] == 0 )
            తిరిగి;
        ఒకవేళ( srcofs )
            srcptr += srcofs[i]*(i <= dims-2 ? srcstep[i] : 1);
        ఒకవేళ( dstofs )
            dstptr += dstofs[i]*(i <= dims-2 ? dststep[i] : 1);
        isz[i] = (పూర్ణాంకం)sz[i];
    }

    Mat src(dims, isz, CV_8U, srcptr, srcstep);
    Mat dst(dims, isz, CV_8U, dstptr, dststep);

    స్థిరరాశి Mat* arrays[] = { &src, &dst };
    uchar* ptrs[2];
    NAryMatIterator it(arrays, ptrs, 2);
    size_t planesz = it.size;

    కొరకు( size_t j = 0; j < it.nplanes; j++, ++it )
        జ్ఞాపకం_నకలు(ptrs[1], ptrs[0], planesz);
}

బఫరు_Pool_Controller* MatAllocator::get_బఫరు_Pool_Controller(స్థిరరాశి అక్షరం* id) స్థిరరాశి
{
    CV_వాడని(id);
    స్థిర Dummy_బఫరు_Pool_Controller dummy;
    తిరిగి &dummy;
}

తరగతి StdMatAllocator CV_FINAL : బహిరంగ MatAllocator
{
బహిరంగ:
    UMat_దత్తాంశం* allocate(పూర్ణాంకం dims, స్థిరరాశి పూర్ణాంకం* sizes, పూర్ణాంకం type,
                       శూన్య* data0, size_t* అడుగు, AccessFlag /*flags*/, UMatUsageFlags /*usageFlags*/) స్థిరరాశి CV_OVERRIDE
    {
        size_t మొత్తం = CV_ELEM_కొలత(type);
        కొరకు( పూర్ణాంకం i = dims-1; i >= 0; i-- )
        {
            ఒకవేళ( అడుగు )
            {
                ఒకవేళ( data0 && అడుగు[i] != CV_AUTOSTEP )
                {
                    CV_Assert(మొత్తం <= అడుగు[i]);
                    మొత్తం = అడుగు[i];
                }
                లేకపోతే
                    అడుగు[i] = మొత్తం;
            }
            మొత్తం *= sizes[i];
        }
        uchar* దత్తాంశం = data0 ? (uchar*)data0 : (uchar*)fastMalloc(మొత్తం);
        UMat_దత్తాంశం* u = కొత్త UMat_దత్తాంశం(ఇది);
        u->దత్తాంశం = u->origdata = దత్తాంశం;
        u->size = మొత్తం;
        ఒకవేళ(data0)
            u->flags |= UMat_దత్తాంశం::USER_ALLOCATED;

        తిరిగి u;
    }

    తార్కిక allocate(UMat_దత్తాంశం* u, AccessFlag /*accessFlags*/, UMatUsageFlags /*usageFlags*/) స్థిరరాశి CV_OVERRIDE
    {
        ఒకవేళ(!u) తిరిగి అబద్ధం;
        తిరిగి నిజం;
    }

    శూన్య deallocate(UMat_దత్తాంశం* u) స్థిరరాశి CV_OVERRIDE
    {
        ఒకవేళ(!u)
            తిరిగి;

        CV_Assert(u->urefcount == 0);
        CV_Assert(u->refcount == 0);
        ఒకవేళ( !(u->flags & UMat_దత్తాంశం::USER_ALLOCATED) )
        {
            fastFree(u->origdata);
            u->origdata = 0;
        }
        తొలగించు u;
    }
};

స్థిర
MatAllocator*& getDefaultAllocatorMatRef()
{
    స్థిర MatAllocator* g_matAllocator = Mat::getStdAllocator();
    తిరిగి g_matAllocator;
}

MatAllocator* Mat::getDefaultAllocator()
{
    తిరిగి getDefaultAllocatorMatRef();
}

శూన్య Mat::సమితి_Default_Allocator(MatAllocator* allocator)
{
    getDefaultAllocatorMatRef() = allocator;
}

MatAllocator* Mat::getStdAllocator()
{
    CV_SINGLETON_LAZY_INIT(MatAllocator, కొత్త StdMatAllocator())
}

//==================================================================================================

శూన్య సమితి_కొలత( Mat& m, పూర్ణాంకం _dims, స్థిరరాశి పూర్ణాంకం* _sz, స్థిరరాశి size_t* అడుగులు_2, తార్కిక auto_అడుగులు)
{
    CV_Assert( 0 <= _dims && _dims <= CV_MAX_DIM && _dims <= MatShape::MAX_DIMS);

    m.dims = _dims;
    m.size = MatShape(_dims, _sz);
    m.అడుగు[std::max(_dims-1, 0)] = CV_ELEM_కొలత(m.flags);
    కొరకు (పూర్ణాంకం i = _dims-2; i >= 0; i--) {
        size_t autostep = m.size[i+1]*m.అడుగు[i+1];
        ఒకవేళ (అడుగులు_2) {
            m.అడుగు[i] = అడుగులు_2[i];
            //CV_Assert(m.step[i] >= autostep);
        } లేకపోతే ఒకవేళ (auto_అడుగులు) {
            m.అడుగు[i] = autostep;
        } లేకపోతే {
            m.అడుగు[i] = 0;
        }
    }

    ఒకవేళ( _dims <= 2 )
    {
        m.నిలువుల_సంఖ్య = _dims == 0 ? 1 : _sz ? _sz[_dims > 1] : 0;
        m.అడ్డువరుసలు = _dims < 2 ? 1 : _sz ? _sz[0] : 0;
    } లేకపోతే {
        m.నిలువుల_సంఖ్య = m.అడ్డువరుసలు = -1;
    }
}

పూర్ణాంకం updateContinuityFlag(పూర్ణాంకం flags, పూర్ణాంకం dims, స్థిరరాశి పూర్ణాంకం* size, స్థిరరాశి size_t* అడుగు)
{
    పూర్ణాంకం i, j;
    ఒకవేళ (dims <= 1)
        తిరిగి flags | Mat::CONTINUOUS_FLAG;
    కొరకు( i = 0; i < dims; i++ )
    {
        ఒకవేళ( size[i] > 1 )
            ఆపు;
    }

    uint64 t = (uint64)size[std::min(i, dims-1)]*CV_MAT_CN(flags);
    కొరకు( j = dims-1; j > i; j-- )
    {
        t *= size[j];
        ఒకవేళ( అడుగు[j]*size[j] < అడుగు[j-1] )
            ఆపు;
    }

    ఒకవేళ( j <= i && t == (uint64)(పూర్ణాంకం)t )
        తిరిగి flags | Mat::CONTINUOUS_FLAG;
    తిరిగి flags & ~Mat::CONTINUOUS_FLAG;
}

శూన్య Mat::updateContinuityFlag()
{
    flags = cv::updateContinuityFlag(flags, dims, size.p, అడుగు.p);
}

శూన్య finalizeHdr(Mat& m)
{
    m.updateContinuityFlag();
    పూర్ణాంకం d = m.dims;
    ఒకవేళ( d > 2 )
        m.అడ్డువరుసలు = m.నిలువుల_సంఖ్య = -1;
    ఒకవేళ(m.u)
        m.datastart = m.దత్తాంశం = m.u->దత్తాంశం;
    ఒకవేళ( m.దత్తాంశం && d > 0 )
    {
        m.datalimit = m.datastart + m.size[0]*m.అడుగు[0];
        ఒకవేళ( m.size[0] > 0 )
        {
            పూర్ణాంకం lastdim = d > 0 ? d - 1 : 0;
            m.dataend = m.ptr() + m.size[lastdim]*m.అడుగు[lastdim];
            కొరకు( పూర్ణాంకం i = 0; i < d-1; i++ )
                m.dataend += (m.size[i] - 1)*m.అడుగు[i];
        }
        లేకపోతే
            m.dataend = m.datalimit;
    }
    లేకపోతే
        m.dataend = m.datalimit = m.దత్తాంశం;
}

//======================================= Mat ======================================================

Mat::Mat() CV_NOEXCEPT
    : flags(MAGIC_VAL), dims(0), అడ్డువరుసలు(0), నిలువుల_సంఖ్య(0), దత్తాంశం(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{}

Mat::Mat(పూర్ణాంకం అడ్డువరుసలు_3, పూర్ణాంకం నిలువుల_సంఖ్య_3, పూర్ణాంకం _type)
    : flags(MAGIC_VAL), dims(0), అడ్డువరుసలు(0), నిలువుల_సంఖ్య(0), దత్తాంశం(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(అడ్డువరుసలు_3, నిలువుల_సంఖ్య_3, _type);
}

Mat::Mat(పూర్ణాంకం అడ్డువరుసలు_3, పూర్ణాంకం నిలువుల_సంఖ్య_3, పూర్ణాంకం _type, స్థిరరాశి Scalar& _s)
    : flags(MAGIC_VAL), dims(0), అడ్డువరుసలు(0), నిలువుల_సంఖ్య(0), దత్తాంశం(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(అడ్డువరుసలు_3, నిలువుల_సంఖ్య_3, _type);
    *ఇది = _s;
}

Mat::Mat(కొలత _sz, పూర్ణాంకం _type)
    : flags(MAGIC_VAL), dims(0), అడ్డువరుసలు(0), నిలువుల_సంఖ్య(0), దత్తాంశం(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create( _sz.ఎత్తు, _sz.వెడల్పు, _type );
}

Mat::Mat(కొలత _sz, పూర్ణాంకం _type, స్థిరరాశి Scalar& _s)
    : flags(MAGIC_VAL), dims(0), అడ్డువరుసలు(0), నిలువుల_సంఖ్య(0), దత్తాంశం(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_sz.ఎత్తు, _sz.వెడల్పు, _type);
    *ఇది = _s;
}

Mat::Mat(పూర్ణాంకం _dims, స్థిరరాశి పూర్ణాంకం* _sz, పూర్ణాంకం _type)
    : flags(MAGIC_VAL), dims(0), అడ్డువరుసలు(0), నిలువుల_సంఖ్య(0), దత్తాంశం(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_dims, _sz, _type);
}

Mat::Mat(పూర్ణాంకం _dims, స్థిరరాశి పూర్ణాంకం* _sz, పూర్ణాంకం _type, స్థిరరాశి Scalar& _s)
    : flags(MAGIC_VAL), dims(0), అడ్డువరుసలు(0), నిలువుల_సంఖ్య(0), దత్తాంశం(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_dims, _sz, _type);
    *ఇది = _s;
}

Mat::Mat(స్థిరరాశి std::vector<పూర్ణాంకం>& _sz, పూర్ణాంకం _type)
    : flags(MAGIC_VAL), dims(0), అడ్డువరుసలు(0), నిలువుల_సంఖ్య(0), దత్తాంశం(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_sz, _type);
}

Mat::Mat(స్థిరరాశి std::vector<పూర్ణాంకం>& _sz, పూర్ణాంకం _type, స్థిరరాశి Scalar& _s)
    : flags(MAGIC_VAL), dims(0), అడ్డువరుసలు(0), నిలువుల_సంఖ్య(0), దత్తాంశం(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_sz, _type);
    *ఇది = _s;
}

Mat::Mat(స్థిరరాశి MatShape& _shape, పూర్ణాంకం _type)
    : flags(MAGIC_VAL), dims(0), అడ్డువరుసలు(0), నిలువుల_సంఖ్య(0), దత్తాంశం(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_shape, _type);
}

Mat::Mat(std::initializer_list<పూర్ణాంకం> _shape, పూర్ణాంకం _type)
    : flags(MAGIC_VAL), dims(0), అడ్డువరుసలు(0), నిలువుల_సంఖ్య(0), దత్తాంశం(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_shape, _type);
}

Mat::Mat(స్థిరరాశి MatShape& _shape, పూర్ణాంకం _type, స్థిరరాశి Scalar& _s)
    : flags(MAGIC_VAL), dims(0), అడ్డువరుసలు(0), నిలువుల_సంఖ్య(0), దత్తాంశం(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_shape, _type);
    *ఇది = _s;
}

Mat::Mat(std::initializer_list<పూర్ణాంకం> _shape, పూర్ణాంకం _type, స్థిరరాశి Scalar& _s)
    : flags(MAGIC_VAL), dims(0), అడ్డువరుసలు(0), నిలువుల_సంఖ్య(0), దత్తాంశం(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_shape, _type);
    *ఇది = _s;
}

Mat::Mat(స్థిరరాశి Mat& m)
    : flags(m.flags), dims(m.dims), అడ్డువరుసలు(m.అడ్డువరుసలు), నిలువుల_సంఖ్య(m.నిలువుల_సంఖ్య), దత్తాంశం(m.దత్తాంశం),
      datastart(m.datastart), dataend(m.dataend), datalimit(m.datalimit), allocator(m.allocator),
      u(m.u), size(m.size), అడుగు(m.అడుగు)
{
    ఒకవేళ( u )
        CV_XADD(&u->refcount, 1);
}

Mat::Mat(పూర్ణాంకం అడ్డువరుసలు_3, పూర్ణాంకం నిలువుల_సంఖ్య_3, పూర్ణాంకం _type, శూన్య* దత్తాంశం_3, size_t అడుగు_3)
    : flags(MAGIC_VAL + (_type & TYPE_ముసుగు)), dims(2), అడ్డువరుసలు(అడ్డువరుసలు_3), నిలువుల_సంఖ్య(నిలువుల_సంఖ్య_3),
      దత్తాంశం((uchar*)దత్తాంశం_3), datastart((uchar*)దత్తాంశం_3), dataend(0), datalimit(0),
      allocator(0), u(0), size(2)
{
    CV_Assert(మొత్తం() == 0 || దత్తాంశం != ఖాళీ);

    size_t esz = CV_ELEM_కొలత(_type), esz1 = CV_ELEM_SIZE1(_type);
    size_t minstep = నిలువుల_సంఖ్య * esz;
    ఒకవేళ( అడుగు_3 == AUTO_అడుగు )
    {
        అడుగు_3 = minstep;
    }
    లేకపోతే
    {
        CV_Assert( అడుగు_3 >= minstep );
        ఒకవేళ (అడుగు_3 % esz1 != 0)
        {
            CV_Error(Error::Bad_అడుగు, "Step must be a multiple of esz1");
        }
    }
    size[0] = అడ్డువరుసలు;
    size[1] = నిలువుల_సంఖ్య;
    అడుగు[0] = అడుగు_3;
    అడుగు[1] = esz;
    datalimit = datastart + అడుగు_3 * అడ్డువరుసలు;
    dataend = datalimit - అడుగు_3 + minstep;
    updateContinuityFlag();
}

Mat::Mat(కొలత _sz, పూర్ణాంకం _type, శూన్య* దత్తాంశం_3, size_t అడుగు_3)
    : flags(MAGIC_VAL + (_type & TYPE_ముసుగు)), dims(2), అడ్డువరుసలు(_sz.ఎత్తు), నిలువుల_సంఖ్య(_sz.వెడల్పు),
      దత్తాంశం((uchar*)దత్తాంశం_3), datastart((uchar*)దత్తాంశం_3), dataend(0), datalimit(0),
      allocator(0), u(0), size(2)
{
    CV_Assert(మొత్తం() == 0 || దత్తాంశం != ఖాళీ);

    size_t esz = CV_ELEM_కొలత(_type), esz1 = CV_ELEM_SIZE1(_type);
    size_t minstep = నిలువుల_సంఖ్య*esz;
    ఒకవేళ( అడుగు_3 == AUTO_అడుగు || (అడుగు_3 < minstep && అడ్డువరుసలు == 1))
    {
        అడుగు_3 = minstep;
    }
    లేకపోతే
    {
        CV_CheckGE(అడుగు_3, minstep, "");

        ఒకవేళ (అడుగు_3 % esz1 != 0)
        {
            CV_Error(Error::Bad_అడుగు, "Step must be a multiple of esz1");
        }
    }
    size[0] = అడ్డువరుసలు;
    size[1] = నిలువుల_సంఖ్య;
    అడుగు[0] = అడుగు_3;
    అడుగు[1] = esz;
    datalimit = datastart + అడుగు_3*అడ్డువరుసలు;
    dataend = datalimit - అడుగు_3 + minstep;
    updateContinuityFlag();
}


Mat::~Mat()
{
    release();
}

Mat& Mat::క్రియాకారకం = (స్థిరరాశి Mat& m)
{
    ఒకవేళ( ఇది != &m )
    {
        ఒకవేళ( m.u )
            CV_XADD(&m.u->refcount, 1);
        release();
        flags = m.flags;
        dims = m.dims;
        అడ్డువరుసలు = m.అడ్డువరుసలు;
        నిలువుల_సంఖ్య = m.నిలువుల_సంఖ్య;
        size = m.size;
        అడుగు = m.అడుగు;
        దత్తాంశం = m.దత్తాంశం;
        datastart = m.datastart;
        dataend = m.dataend;
        datalimit = m.datalimit;
        allocator = m.allocator;
        u = m.u;
    }
    తిరిగి *ఇది;
}

Mat Mat::clone() స్థిరరాశి
{
    Mat m;
    copy_గమ్యం(m);
    తిరిగి m;
}

శూన్య Mat::కేటాయించు_గమ్యం( Mat& m, పూర్ణాంకం _type ) స్థిరరాశి
{
    ఒకవేళ( _type < 0 )
        m = *ఇది;
    లేకపోతే
        convert_గమ్యం(m, _type);
}

శూన్య Mat::create(పూర్ణాంకం అడ్డువరుసలు_3, పూర్ణాంకం నిలువుల_సంఖ్య_3, పూర్ణాంకం _type)
{
    _type &= TYPE_ముసుగు;
    ఒకవేళ( dims <= 2 && అడ్డువరుసలు == అడ్డువరుసలు_3 && నిలువుల_సంఖ్య == నిలువుల_సంఖ్య_3 && type() == _type && దత్తాంశం )
        తిరిగి;
    పూర్ణాంకం sz[] = {అడ్డువరుసలు_3, నిలువుల_సంఖ్య_3};
    create(2, sz, _type);
}

శూన్య Mat::create(కొలత _sz, పూర్ణాంకం _type)
{
    create(_sz.ఎత్తు, _sz.వెడల్పు, _type);
}

శూన్య Mat::create_Same_కొలత(ఆదానం_Array m, పూర్ణాంకం type)
{
    MatShape msize = m.shape();
    create(msize, type);
}

శూన్య Mat::fit(పూర్ణాంకం _dims, స్థిరరాశి పూర్ణాంకం* _sizes, పూర్ణాంకం _type)
{
    size_t old_మొత్తం_బైట్లు = u ? u->size : 0;
    size_t esz = CV_ELEM_కొలత(_type), new_మొత్తం = _dims >= 0;
    కొరకు (పూర్ణాంకం i = 0; i < _dims; i++)
        new_మొత్తం *= _sizes[i];
    size_t new_మొత్తం_బైట్లు = new_మొత్తం*esz;
    ఒకవేళ (new_మొత్తం_బైట్లు > 0 && (!isContinuous() ||
                              new_మొత్తం_బైట్లు > old_మొత్తం_బైట్లు ||
                              దత్తాంశం != datastart)) {
        create(_dims, _sizes, _type);
    } లేకపోతే {
        flags = (flags & ~Mat::TYPE_ముసుగు) | CV_MAT_TYPE(_type);
        పూర్ణాంకం dummy_కొలత = 0;
        సమితి_కొలత(*ఇది, (_dims >= 0 ? _dims : 1), (_dims >= 0 ? _sizes : &dummy_కొలత), శూన్యసూచిక, నిజం);
        finalizeHdr(*ఇది);
    }
}

శూన్య Mat::fit(స్థిరరాశి std::vector<పూర్ణాంకం>& _shape, పూర్ణాంకం _type)
{
    fit((పూర్ణాంకం)_shape.size(), _shape.దత్తాంశం(), _type);
}

శూన్య Mat::fit(స్థిరరాశి MatShape& _shape, పూర్ణాంకం _type)
{
    fit(_shape.dims, _shape.p, _type);
    size.layout = _shape.layout;
    size.C = _shape.C;
}

శూన్య Mat::fit(std::initializer_list<పూర్ణాంకం> _shape, పూర్ణాంకం _type)
{
    పూర్ణాంకం new_shape[MatShape::MAX_DIMS];
    పూర్ణాంకం new_ndims = (పూర్ణాంకం)_shape.size();
    CV_Assert(new_ndims <= MatShape::MAX_DIMS);
    స్వయంచాలక it = _shape.begin();
    కొరకు (పూర్ణాంకం i = 0; i < new_ndims; i++, ++it)
        new_shape[i] = *it;
    fit(new_ndims, new_shape, _type);
}

శూన్య Mat::fit(పూర్ణాంకం అడ్డువరుసలు_3, పూర్ణాంకం నిలువుల_సంఖ్య_3, పూర్ణాంకం _type)
{
    _type &= TYPE_ముసుగు;
    పూర్ణాంకం sz[] = {అడ్డువరుసలు_3, నిలువుల_సంఖ్య_3};
    fit(2, sz, _type);
}

శూన్య Mat::fit(కొలత _sz, పూర్ణాంకం _type)
{
    fit(_sz.ఎత్తు, _sz.వెడల్పు, _type);
}

శూన్య Mat::fit_Same_కొలత(ఆదానం_Array m, పూర్ణాంకం _type)
{
    fit(m.shape(), _type);
}

శూన్య Mat::addref()
{
    ఒకవేళ( u )
        CV_XADD(&u->refcount, 1);
}

శూన్య Mat::release()
{
    ఒకవేళ( u && CV_XADD(&u->refcount, -1) == 1 )
        deallocate();
    u = ఖాళీ;
    datastart = dataend = datalimit = దత్తాంశం = 0;
    కొరకు(పూర్ణాంకం i = 0; i < dims; i++)
        size.p[i] = 0;
    flags = (flags & CV_MAT_TYPE_ముసుగు) | MAGIC_VAL;
    dims = అడ్డువరుసలు = నిలువుల_సంఖ్య = 0;
    size.clear();
}

size_t Mat::step1(పూర్ణాంకం i) స్థిరరాశి
{
    తిరిగి అడుగు.p[i] / elemSize1();
}

తార్కిక Mat::empty() స్థిరరాశి
{
    తిరిగి దత్తాంశం == 0 || మొత్తం() == 0;
}

size_t Mat::మొత్తం() స్థిరరాశి
{
    ఒకవేళ( dims <= 2 )
        తిరిగి (size_t)అడ్డువరుసలు * నిలువుల_సంఖ్య;
    size_t p = 1;
    కొరకు( పూర్ణాంకం i = 0; i < dims; i++ )
        p *= size[i];
    తిరిగి p;
}

size_t Mat::మొత్తం(పూర్ణాంకం ప్రారంభం_Dim, పూర్ణాంకం చివర_Dim) స్థిరరాశి
{
    CV_Assert( 0 <= ప్రారంభం_Dim && ప్రారంభం_Dim <= చివర_Dim);
    size_t p = 1;
    పూర్ణాంకం చివర_Dim_2 = చివర_Dim <= dims ? చివర_Dim : dims;
    కొరకు( పూర్ణాంకం i = ప్రారంభం_Dim; i < చివర_Dim_2; i++ )
        p *= size[i];
    తిరిగి p;
}

Mat::Mat(Mat&& m) CV_NOEXCEPT
    : flags(m.flags), dims(m.dims), అడ్డువరుసలు(m.అడ్డువరుసలు), నిలువుల_సంఖ్య(m.నిలువుల_సంఖ్య), దత్తాంశం(m.దత్తాంశం),
      datastart(m.datastart), dataend(m.dataend), datalimit(m.datalimit), allocator(m.allocator),
      u(m.u), size(m.size), అడుగు(m.అడుగు)
{
    m.flags = MAGIC_VAL; m.dims = m.అడ్డువరుసలు = m.నిలువుల_సంఖ్య = 0;
    m.size.clear(); m.అడుగు.clear();
    m.దత్తాంశం = ఖాళీ; m.datastart = ఖాళీ; m.dataend = ఖాళీ; m.datalimit = ఖాళీ;
    m.allocator = ఖాళీ;
    m.u = ఖాళీ;
}


Mat& Mat::క్రియాకారకం = (Mat&& m)
{
    ఒకవేళ (ఇది == &m)
      తిరిగి *ఇది;

    release();
    flags = m.flags; dims = m.dims; అడ్డువరుసలు = m.అడ్డువరుసలు; నిలువుల_సంఖ్య = m.నిలువుల_సంఖ్య; దత్తాంశం = m.దత్తాంశం;
    size = m.size; అడుగు = m.అడుగు;
    datastart = m.datastart; dataend = m.dataend; datalimit = m.datalimit; allocator = m.allocator;
    u = m.u;
    m.flags = MAGIC_VAL; m.dims = m.అడ్డువరుసలు = m.నిలువుల_సంఖ్య = 0;
    m.size.clear(); m.అడుగు.clear();
    m.దత్తాంశం = ఖాళీ; m.datastart = ఖాళీ; m.dataend = ఖాళీ; m.datalimit = ఖాళీ;
    m.allocator = ఖాళీ;
    m.u = ఖాళీ;
    తిరిగి *ఇది;
}


శూన్య Mat::create(పూర్ణాంకం d0, స్థిరరాశి పూర్ణాంకం* _sizes, పూర్ణాంకం _type)
{
    పూర్ణాంకం sz1 = 1, d = d0;
    పూర్ణాంకం i;
    ఒకవేళ (d == 0) {
        d = 1;
        _sizes = (స్థిరరాశి పూర్ణాంకం*)&sz1;
    }
    CV_Assert(0 <= d && d <= CV_MAX_DIM && _sizes);
    _type = CV_MAT_TYPE(_type);

    ఒకవేళ( దత్తాంశం && d == dims && _type == type() )
    {
        కొరకు( i = 0; i < d; i++ )
            ఒకవేళ( size[i] != _sizes[i] )
                ఆపు;
        ఒకవేళ( i == d )
            తిరిగి;
    }

    పూర్ణాంకం _sizes_backup[CV_MAX_DIM]; // #5991
    ఒకవేళ (_sizes == (ఇది->size.p))
    {
        కొరకు(i = 0; i < d; i++ )
            _sizes_backup[i] = _sizes[i];
        _sizes = _sizes_backup;
    }

    release();
    ఒకవేళ( d == 0 )
        తిరిగి;
    flags = (_type & CV_MAT_TYPE_ముసుగు) | MAGIC_VAL;
    సమితి_కొలత(*ఇది, d, _sizes, 0, నిజం);

    ఒకవేళ( మొత్తం() > 0 )
    {
        MatAllocator *a = allocator, *a0 = getDefaultAllocator();
        ఒకవేళ(!a)
            a = a0;
        ప్రయత్నించు
        {
            u = a->allocate(dims, size.p, _type, 0, అడుగు.p, ACCESS_RW /* ignored */, USAGE_DEFAULT);
            CV_Assert(u != 0);
            allocator = a;
        }
        పట్టుకో (...)
        {
            ఒకవేళ (a == a0)
                విసురు;
            u = a0->allocate(dims, size.p, _type, 0, అడుగు.p, ACCESS_RW /* ignored */, USAGE_DEFAULT);
            CV_Assert(u != 0);
            allocator = a0;
        }
        CV_Assert( అడుగు[dims-1] == (size_t)CV_ELEM_కొలత(flags) );
    }

    addref();
    finalizeHdr(*ఇది);
    size.dims = dims = d0;
}

శూన్య Mat::create(స్థిరరాశి std::vector<పూర్ణాంకం>& _sizes, పూర్ణాంకం _type)
{
    create((పూర్ణాంకం)_sizes.size(), _sizes.దత్తాంశం(), _type);
}

శూన్య Mat::create(స్థిరరాశి MatShape& _shape, పూర్ణాంకం _type)
{
    ఒకవేళ (_shape.dims < 0) {
        release();
        తిరిగి;
    }
    create(_shape.dims, _shape.p, _type);
    size.layout = _shape.layout;
    size.C = _shape.C;
}

శూన్య Mat::create(std::initializer_list<పూర్ణాంకం> _shape, పూర్ణాంకం _type)
{
    పూర్ణాంకం new_shape[MatShape::MAX_DIMS];
    పూర్ణాంకం new_ndims = (పూర్ణాంకం)_shape.size();
    CV_Assert(new_ndims <= MatShape::MAX_DIMS);
    స్వయంచాలక it = _shape.begin();
    కొరకు (పూర్ణాంకం i = 0; i < new_ndims; i++, ++it)
        new_shape[i] = *it;

    create(new_ndims, new_shape, _type);
}

శూన్య Mat::copy_కొలత(స్థిరరాశి Mat& m)
{
    dims = m.dims;
    నిలువుల_సంఖ్య = m.నిలువుల_సంఖ్య;
    అడ్డువరుసలు = m.అడ్డువరుసలు;
    size = m.size;
    అడుగు = m.అడుగు;
}

శూన్య Mat::deallocate()
{
    ఒకవేళ(u)
    {
        UMat_దత్తాంశం* u_ = u;
        u = ఖాళీ;
        (u_->currAllocator ? u_->currAllocator : allocator ? allocator : getDefaultAllocator())->unmap(u_);
    }
}

Mat::Mat(స్థిరరాశి Mat& m, స్థిరరాశి Range& అడ్డువరుస_Range_3, స్థిరరాశి Range& నిలువు_Range_3)
    : flags(MAGIC_VAL), dims(0), అడ్డువరుసలు(0), నిలువుల_సంఖ్య(0), దత్తాంశం(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    CV_Assert(m.dims <= MatShape::MAX_DIMS);
    CV_Assert(m.dims >= 2 ||
              (m.dims == 1 && (అడ్డువరుస_Range_3 == Range::all() ||
                               అడ్డువరుస_Range_3 == Range(0, m.అడ్డువరుసలు))));
    ఒకవేళ( m.dims > 2 )
    {
        Range rs[MatShape::MAX_DIMS];
        rs[0] = అడ్డువరుస_Range_3;
        rs[1] = నిలువు_Range_3;
        కొరకు( పూర్ణాంకం i = 2; i < m.dims; i++ )
            rs[i] = Range::all();
        *ఇది = m(rs);
        తిరిగి;
    }

    *ఇది = m;
    ప్రయత్నించు
    {
        ఒకవేళ( అడ్డువరుస_Range_3 != Range::all() && అడ్డువరుస_Range_3 != Range(0,అడ్డువరుసలు) )
        {
            CV_Assert( 0 <= అడ్డువరుస_Range_3.ప్రారంభం && అడ్డువరుస_Range_3.ప్రారంభం <= అడ్డువరుస_Range_3.end
                       && అడ్డువరుస_Range_3.end <= m.అడ్డువరుసలు );
            size[0] = అడ్డువరుసలు = అడ్డువరుస_Range_3.size();
            దత్తాంశం += అడుగు*అడ్డువరుస_Range_3.ప్రారంభం;
            flags |= SUBMATRIX_FLAG;
        }

        ఒకవేళ( నిలువు_Range_3 != Range::all() && నిలువు_Range_3 != Range(0,నిలువుల_సంఖ్య) )
        {
            CV_Assert( 0 <= నిలువు_Range_3.ప్రారంభం && నిలువు_Range_3.ప్రారంభం <= నిలువు_Range_3.end
                       && నిలువు_Range_3.end <= m.నిలువుల_సంఖ్య );
            size[dims > 1] = నిలువుల_సంఖ్య = నిలువు_Range_3.size();
            దత్తాంశం += నిలువు_Range_3.ప్రారంభం*elem_కొలత();
            flags |= SUBMATRIX_FLAG;
        }
    }
    పట్టుకో(...)
    {
        release();
        విసురు;
    }

    updateContinuityFlag();

    ఒకవేళ( అడ్డువరుసలు <= 0 || నిలువుల_సంఖ్య <= 0 )
    {
        release();
        అడ్డువరుసలు = నిలువుల_సంఖ్య = 0;
    }
}


Mat::Mat(స్థిరరాశి Mat& m, స్థిరరాశి Rect& roi)
    : flags(m.flags), dims(2), అడ్డువరుసలు(roi.ఎత్తు), నిలువుల_సంఖ్య(roi.వెడల్పు),
    దత్తాంశం(m.దత్తాంశం + roi.y*m.అడుగు[0]),
    datastart(m.datastart), dataend(m.dataend), datalimit(m.datalimit),
    allocator(m.allocator), u(m.u), size(2)
{
    CV_Assert( m.dims <= 2 );

    size_t esz = CV_ELEM_కొలత(flags);
    దత్తాంశం += roi.x*esz;
    CV_Assert( 0 <= roi.x && 0 <= roi.వెడల్పు && roi.x + roi.వెడల్పు <= m.నిలువుల_సంఖ్య &&
              0 <= roi.y && 0 <= roi.ఎత్తు && roi.y + roi.ఎత్తు <= m.అడ్డువరుసలు );
    ఒకవేళ( roi.వెడల్పు < m.నిలువుల_సంఖ్య || roi.ఎత్తు < m.అడ్డువరుసలు )
        flags |= SUBMATRIX_FLAG;

    size[0] = అడ్డువరుసలు;
    size[1] = నిలువుల_సంఖ్య;
    అడుగు[0] = m.అడుగు[0];
    అడుగు[1] = esz;
    updateContinuityFlag();

    addref();
    ఒకవేళ( అడ్డువరుసలు <= 0 || నిలువుల_సంఖ్య <= 0 )
    {
        అడ్డువరుసలు = నిలువుల_సంఖ్య = 0;
        release();
    }
}


Mat::Mat(పూర్ణాంకం _dims, స్థిరరాశి పూర్ణాంకం* _sizes, పూర్ణాంకం _type, శూన్య* దత్తాంశం_3, స్థిరరాశి size_t* అడుగులు_2)
    : flags(MAGIC_VAL), dims(0), అడ్డువరుసలు(0), నిలువుల_సంఖ్య(0), దత్తాంశం(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    flags |= CV_MAT_TYPE(_type);
    datastart = దత్తాంశం = (uchar*)దత్తాంశం_3;
    ఒకవేళ (_dims > 0 || దత్తాంశం_3 != శూన్యసూచిక)
        సమితి_కొలత(*ఇది, _dims, _sizes, అడుగులు_2, నిజం);
    finalizeHdr(*ఇది);
}


Mat::Mat(స్థిరరాశి std::vector<పూర్ణాంకం>& _sizes, పూర్ణాంకం _type, శూన్య* దత్తాంశం_3, స్థిరరాశి size_t* అడుగులు_2)
    : flags(MAGIC_VAL), dims(0), అడ్డువరుసలు(0), నిలువుల_సంఖ్య(0), దత్తాంశం(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    flags |= CV_MAT_TYPE(_type);
    datastart = దత్తాంశం = (uchar*)దత్తాంశం_3;
    పూర్ణాంకం _dims = (పూర్ణాంకం)_sizes.size();
    ఒకవేళ (_dims > 0 || దత్తాంశం_3 != శూన్యసూచిక)
        సమితి_కొలత(*ఇది, _dims, _sizes.దత్తాంశం(), అడుగులు_2, నిజం);
    finalizeHdr(*ఇది);
}

Mat::Mat(స్థిరరాశి MatShape& _shape, పూర్ణాంకం _type, శూన్య* దత్తాంశం_3, స్థిరరాశి size_t* అడుగులు_2)
    : flags(MAGIC_VAL), dims(0), అడ్డువరుసలు(0), నిలువుల_సంఖ్య(0), దత్తాంశం(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    flags |= CV_MAT_TYPE(_type);
    datastart = దత్తాంశం = (uchar*)దత్తాంశం_3;
    ఒకవేళ (_shape.dims >= 0) {
        సమితి_కొలత(*ఇది, _shape.dims, _shape.p, అడుగులు_2, నిజం);
    }
    లేకపోతే {
        CV_Assert(!దత్తాంశం);
    }
    finalizeHdr(*ఇది);
}

Mat::Mat(std::initializer_list<పూర్ణాంకం> _shape, పూర్ణాంకం _type, శూన్య* దత్తాంశం_3, స్థిరరాశి size_t* అడుగులు_2)
    : flags(MAGIC_VAL), dims(0), అడ్డువరుసలు(0), నిలువుల_సంఖ్య(0), దత్తాంశం(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    పూర్ణాంకం new_shape[MatShape::MAX_DIMS];
    పూర్ణాంకం _dims = (పూర్ణాంకం)_shape.size();
    CV_Assert(_dims <= MatShape::MAX_DIMS);
    స్వయంచాలక it = _shape.begin();
    కొరకు (పూర్ణాంకం i = 0; i < _dims; i++, ++it)
        new_shape[i] = *it;

    flags |= CV_MAT_TYPE(_type);
    datastart = దత్తాంశం = (uchar*)దత్తాంశం_3;
    ఒకవేళ (_dims > 0 || దత్తాంశం_3 != శూన్యసూచిక)
        సమితి_కొలత(*ఇది, _dims, new_shape, అడుగులు_2, నిజం);
    finalizeHdr(*ఇది);
}

Mat::Mat(స్థిరరాశి Mat& m, స్థిరరాశి Range* ranges)
    : flags(MAGIC_VAL), dims(0), అడ్డువరుసలు(0), నిలువుల_సంఖ్య(0), దత్తాంశం(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    పూర్ణాంకం d = m.dims;

    CV_Assert(ranges);
    కొరకు( పూర్ణాంకం i = 0; i < d; i++ )
    {
        Range r = ranges[i];
        CV_Assert( r == Range::all() || (0 <= r.ప్రారంభం && r.ప్రారంభం < r.end && r.end <= m.size[i]) );
    }
    *ఇది = m;
    కొరకు( పూర్ణాంకం i = 0; i < d; i++ )
    {
        Range r = ranges[i];
        ఒకవేళ( r != Range::all() && r != Range(0, size.p[i]))
        {
            size.p[i] = r.end - r.ప్రారంభం;
            దత్తాంశం += r.ప్రారంభం*అడుగు.p[i];
            flags |= SUBMATRIX_FLAG;
        }
    }

    ఒకవేళ (d <= 2) {
        అడ్డువరుసలు = d == 2 ? size[0] : 1;
        నిలువుల_సంఖ్య = d <= 0 ? (d >= 0) : size[d > 1];
    }
    updateContinuityFlag();
}

Mat::Mat(స్థిరరాశి Mat& m, స్థిరరాశి std::vector<Range>& ranges)
    : flags(MAGIC_VAL), dims(0), అడ్డువరుసలు(0), నిలువుల_సంఖ్య(0), దత్తాంశం(0), datastart(0), dataend(0),
    datalimit(0), allocator(0), u(0)
{
    పూర్ణాంకం d = m.dims;

    CV_Assert((పూర్ణాంకం)ranges.size() == d);
    కొరకు (పూర్ణాంకం i = 0; i < d; i++)
    {
        Range r = ranges[i];
        CV_Assert(r == Range::all() || (0 <= r.ప్రారంభం && r.ప్రారంభం < r.end && r.end <= m.size[i]));
    }
    *ఇది = m;
    కొరకు (పూర్ణాంకం i = 0; i < d; i++)
    {
        Range r = ranges[i];
        ఒకవేళ (r != Range::all() && r != Range(0, size.p[i]))
        {
            size.p[i] = r.end - r.ప్రారంభం;
            దత్తాంశం += r.ప్రారంభం*అడుగు.p[i];
            flags |= SUBMATRIX_FLAG;
        }
    }

    ఒకవేళ (d <= 2) {
        అడ్డువరుసలు = d == 2 ? size[0] : 1;
        నిలువుల_సంఖ్య = d <= 0 ? (d >= 0) : size[d > 1];
    }
    updateContinuityFlag();
}


Mat Mat::diag(పూర్ణాంకం d) స్థిరరాశి
{
    CV_Assert( dims == 2 );
    Mat m = *ఇది;
    size_t esz = elem_కొలత();
    పూర్ణాంకం len;

    ఒకవేళ( d >= 0 )
    {
        len = std::min(నిలువుల_సంఖ్య - d, అడ్డువరుసలు);
        m.దత్తాంశం += esz*d;
    }
    లేకపోతే
    {
        len = std::min(అడ్డువరుసలు + d, నిలువుల_సంఖ్య);
        m.దత్తాంశం -= అడుగు[0]*d;
    }
    CV_DbgAssert( len > 0 );

    m.size[0] = m.అడ్డువరుసలు = len;
    m.size[1] = m.నిలువుల_సంఖ్య = 1;
    m.అడుగు[0] += (len > 1 ? esz : 0);

    m.updateContinuityFlag();

    ఒకవేళ( size() != కొలత(1,1) )
        m.flags |= SUBMATRIX_FLAG;

    తిరిగి m;
}


శూన్య Mat::pop_back(size_t nelems)
{
    CV_Assert( nelems <= (size_t)size.p[0] );
    CV_Assert( dims >= 1 );

    ఒకవేళ( isSubmatrix() )
        *ఇది = అడ్డువరుస_Range_2(0, size.p[0] - (పూర్ణాంకం)nelems);
    లేకపోతే
    {
        size.p[0] -= (పూర్ణాంకం)nelems;
        ఒకవేళ (dims == 2)
            అడ్డువరుసలు = size.p[0];
        లేకపోతే ఒకవేళ (dims == 1)
            నిలువుల_సంఖ్య = size.p[0];
        dataend -= nelems*అడుగు.p[0];
    }
}


శూన్య Mat::నెట్టు_వెనుక(స్థిరరాశి శూన్య* elem)
{
    size_t r = size.p[0];
    ఒకవేళ( isSubmatrix() || dataend + అడుగు.p[0] > datalimit )
        reserve( std::max(r + 1, (r*3+1)/2) );

    size_t esz = elem_కొలత();
    జ్ఞాపకం_నకలు(దత్తాంశం + r*అడుగు.p[0], elem, esz);
    size.p[0] = పూర్ణాంకం(r + 1);
    ఒకవేళ (dims == 2)
        అడ్డువరుసలు = size.p[0];
    లేకపోతే ఒకవేళ (dims == 1)
        నిలువుల_సంఖ్య = size.p[0];
    dataend += అడుగు.p[0];
    uint64 tsz = size.p[0];
    కొరకు( పూర్ణాంకం i = 1; i < dims; i++ )
        tsz *= size.p[i];
    ఒకవేళ( esz < అడుగు.p[0] || tsz != (uint64)(పూర్ణాంకం)tsz )
        flags &= ~CONTINUOUS_FLAG;
}


శూన్య Mat::reserve(size_t nelems)
{
    స్థిరరాశి size_t MIN_కొలత = 64;

    CV_Assert( (పూర్ణాంకం)nelems >= 0 );
    ఒకవేళ( !isSubmatrix() && అడుగు.p[0] != 0 && దత్తాంశం + అడుగు.p[0]*nelems <= datalimit )
        తిరిగి;

    పూర్ణాంకం r = size.p[0];
    ఒకవేళ( (size_t)r >= nelems)
        తిరిగి;

    MatShape newsize = size;
    size_t esz = elem_కొలత();
    newsize.dims = std::max(newsize.dims, 1);
    ఒకవేళ (dims > 1 && r == 0 && అడుగు.p[0] == 0) {
        అడుగు.p[0] = size.p[1] * (dims == 2 ? esz : అడుగు.p[1]);
    }
    newsize.p[0] = (పూర్ణాంకం)nelems;
    size_t newbytes = newsize.మొత్తం() * esz;
    ఒకవేళ (newbytes < MIN_కొలత) {
        newsize.p[0] = 1;
        newsize.p[0] = పూర్ణాంకం(MIN_కొలత / newsize.మొత్తం() / esz);
    }

    Mat m(newsize, type());
    ఒకవేళ( r > 0)
    {
        Mat mpart = m.అడ్డువరుస_Range_2(0, r);
        copy_గమ్యం(mpart);
    }

    *ఇది = m;
    size.p[0] = r;

    ఒకవేళ (dims == 2)
        అడ్డువరుసలు = size.p[0];
    లేకపోతే ఒకవేళ (dims == 1)
        నిలువుల_సంఖ్య = size.p[0];
    dataend = దత్తాంశం + అడుగు.p[0]*r;
}


శూన్య Mat::reserve_బఫరు(size_t nbytes)
{
    size_t esz = 1;
    పూర్ణాంకం mtype = CV_8UC1;
    ఒకవేళ (!empty())
    {
        ఒకవేళ (!isSubmatrix() && దత్తాంశం + nbytes <= dataend)//Should it be datalimit?
            తిరిగి;
        esz = elem_కొలత();
        mtype = type();
    }

    size_t nelems = (nbytes - 1) / esz + 1;

#ఒకవేళ SIZE_MAX > UINT_MAX
    CV_Assert(nelems <= size_t(గరిష్ఠ_పూర్ణాంకం)*size_t(గరిష్ఠ_పూర్ణాంకం));
    పూర్ణాంకం newrows = nelems > size_t(గరిష్ఠ_పూర్ణాంకం) ? nelems > 0x400*size_t(గరిష్ఠ_పూర్ణాంకం) ? nelems > 0x100000 * size_t(గరిష్ఠ_పూర్ణాంకం) ? nelems > 0x40000000 * size_t(గరిష్ఠ_పూర్ణాంకం) ?
                  size_t(గరిష్ఠ_పూర్ణాంకం) : 0x40000000 : 0x100000 : 0x400 : 1;
#లేకపోతే
    పూర్ణాంకం newrows = nelems > size_t(గరిష్ఠ_పూర్ణాంకం) ? 2 : 1;
#అయితే_ముగింపు
    పూర్ణాంకం newcols = (పూర్ణాంకం)((nelems - 1) / newrows + 1);

    create(newrows, newcols, mtype);
}


శూన్య Mat::resize(size_t nelems)
{
    పూర్ణాంకం r = size.p[0];
    ఒకవేళ( r == (పూర్ణాంకం)nelems )
        తిరిగి;
    CV_Assert( (పూర్ణాంకం)nelems >= 0 );

    ఒకవేళ( isSubmatrix() || దత్తాంశం + అడుగు.p[0]*nelems > datalimit )
        reserve(nelems);

    size.p[0] = (పూర్ణాంకం)nelems;
    ఒకవేళ (dims == 2)
        అడ్డువరుసలు = size.p[0];
    లేకపోతే ఒకవేళ (dims == 1)
        నిలువుల_సంఖ్య = size.p[0];
    dataend += (size.p[0] - r)*(int64_t)అడుగు.p[0];

    //updateContinuityFlag(*this);
}


శూన్య Mat::resize(size_t nelems, స్థిరరాశి Scalar& s)
{
    పూర్ణాంకం save_అడ్డువరుసలు = size.p[0];
    resize(nelems);

    ఒకవేళ( size.p[0] > save_అడ్డువరుసలు )
    {
        Mat part = అడ్డువరుస_Range_2(save_అడ్డువరుసలు, size.p[0]);
        part = s;
    }
}

శూన్య Mat::push_back(స్థిరరాశి Mat& elems)
{
    size_t r = size.p[0];
    size_t delta = elems.size.p[0];
    ఒకవేళ( delta == 0 )
        తిరిగి;
    ఒకవేళ( ఇది == &elems )
    {
        Mat tmp = elems;
        push_back(tmp);
        తిరిగి;
    }
    ఒకవేళ( !దత్తాంశం )
    {
        *ఇది = elems.clone();
        తిరిగి;
    }
    CV_Assert(dims > 0);

    size.p[0] = elems.size.p[0];
    తార్కిక eq = size == elems.size;
    size.p[0] = పూర్ణాంకం(r);
    ఒకవేళ( !eq )
        CV_Error(cv::Error::StsUnmatchedSizes, "Pushed vector length is not equal to matrix row length");
    ఒకవేళ( type() != elems.type() )
        CV_Error(cv::Error::StsUnmatchedFormats, "Pushed vector type is not the same as matrix type");
    size_t esz = elem_కొలత();
    size_t minstep = dims <= 1 ? esz : size.p[1] * (dims == 2 ? esz : అడుగు.p[1]);
    size_t step0 = అడుగు.p[0];
    ఒకవేళ (step0 < minstep) {
        ఒకవేళ (size.p[0] <= 1)
            అడుగు.p[0] = step0 = minstep;
    }

    ఒకవేళ( isSubmatrix() || dataend + step0*delta > datalimit )
        reserve( std::max(r + delta, (r*3+1)/2) );

    size.p[0] += పూర్ణాంకం(delta);
    ఒకవేళ (dims == 2)
        అడ్డువరుసలు = size.p[0];
    లేకపోతే ఒకవేళ (dims == 1)
        నిలువుల_సంఖ్య = size.p[0];
    dataend += step0*delta;

    //updateContinuityFlag(*this);

    ఒకవేళ( isContinuous() && elems.isContinuous() )
        జ్ఞాపకం_నకలు(దత్తాంశం + r*step0, elems.దత్తాంశం, elems.మొత్తం()*elems.elem_కొలత());
    లేకపోతే
    {
        Mat part = అడ్డువరుస_Range_2(పూర్ణాంకం(r), పూర్ణాంకం(r + delta));
        elems.copy_గమ్యం(part);
    }
}


శూన్య Mat::locateROI( కొలత& సంపూర్ణ_కొలత, బిందువు& ofs ) స్థిరరాశి
{
    CV_Assert( dims <= 2 && అడుగు[0] > 0 );
    size_t esz = elem_కొలత(), minstep;
    ptrdiff_t delta1 = దత్తాంశం - datastart, delta2 = dataend - datastart;

    ఒకవేళ( delta1 == 0 )
        ofs.x = ofs.y = 0;
    లేకపోతే
    {
        ofs.y = (పూర్ణాంకం)(delta1/అడుగు[0]);
        ofs.x = (పూర్ణాంకం)((delta1 - అడుగు[0]*ofs.y)/esz);
        CV_DbgAssert( దత్తాంశం == datastart + ofs.y*అడుగు[0] + ofs.x*esz );
    }
    minstep = (ofs.x + నిలువుల_సంఖ్య)*esz;
    సంపూర్ణ_కొలత.ఎత్తు = (పూర్ణాంకం)((delta2 - minstep)/అడుగు[0] + 1);
    సంపూర్ణ_కొలత.ఎత్తు = std::max(సంపూర్ణ_కొలత.ఎత్తు, ofs.y + అడ్డువరుసలు);
    సంపూర్ణ_కొలత.వెడల్పు = (పూర్ణాంకం)((delta2 - అడుగు*(సంపూర్ణ_కొలత.ఎత్తు-1))/esz);
    సంపూర్ణ_కొలత.వెడల్పు = std::max(సంపూర్ణ_కొలత.వెడల్పు, ofs.x + నిలువుల_సంఖ్య);
}

Mat& Mat::adjustROI( పూర్ణాంకం dtop, పూర్ణాంకం dbottom, పూర్ణాంకం dleft, పూర్ణాంకం dright )
{
    CV_Assert( dims <= 2 && అడుగు[0] > 0 );
    కొలత సంపూర్ణ_కొలత; బిందువు ofs;
    size_t esz = elem_కొలత();
    locateROI( సంపూర్ణ_కొలత, ofs );
    పూర్ణాంకం row1 = std::min(std::max(ofs.y - dtop, 0), సంపూర్ణ_కొలత.ఎత్తు), row2 = std::max(0, std::min(ofs.y + అడ్డువరుసలు + dbottom, సంపూర్ణ_కొలత.ఎత్తు));
    పూర్ణాంకం col1 = std::min(std::max(ofs.x - dleft, 0), సంపూర్ణ_కొలత.వెడల్పు), col2 = std::max(0, std::min(ofs.x + నిలువుల_సంఖ్య + dright, సంపూర్ణ_కొలత.వెడల్పు));
    ఒకవేళ(row1 > row2)
        std::swap(row1, row2);
    ఒకవేళ(col1 > col2)
        std::swap(col1, col2);

    ఒకవేళ (dims == 1) {
        దత్తాంశం += (col1 - ofs.x)*(std::ptrdiff_t)esz;
        నిలువుల_సంఖ్య = col2 - col1;
        size.p[0] = నిలువుల_సంఖ్య;
    } లేకపోతే {
        దత్తాంశం += (row1 - ofs.y)*(std::ptrdiff_t)అడుగు + (col1 - ofs.x)*(std::ptrdiff_t)esz;
        అడ్డువరుసలు = row2 - row1; నిలువుల_సంఖ్య = col2 - col1;
        size.p[0] = అడ్డువరుసలు; size.p[1] = నిలువుల_సంఖ్య;
        updateContinuityFlag();
    }
    తిరిగి *ఇది;
}

Mat Mat::reshape(పూర్ణాంకం new_cn, పూర్ణాంకం new_అడ్డువరుసలు) స్థిరరాశి
{
    పూర్ణాంకం cn = channels();
    Mat hdr = *ఇది;

    ఒకవేళ( new_cn == 0 )
        new_cn = cn;

    ఒకవేళ( dims > 2 )
    {
        ఒకవేళ( new_అడ్డువరుసలు == 0 )
        {
            // special case: just change the number of channnels; retain the same shape,
            // except for the last, innermost dimension
            CV_Assert(size[dims-1]*cn % new_cn == 0);
            hdr.flags = (hdr.flags & ~CV_MAT_CN_ముసుగు) | ((new_cn-1) << CV_CN_జరుపు);
            hdr.అడుగు[dims-1] = CV_ELEM_కొలత(hdr.flags);
            hdr.size[dims-1] = hdr.size[dims-1]*cn / new_cn;
            తిరిగి hdr;
        }
        CV_Assert( new_అడ్డువరుసలు > 0 );
        పూర్ణాంకం sz[] = { new_అడ్డువరుసలు, (పూర్ణాంకం)(మొత్తం()*cn/new_అడ్డువరుసలు) };
        తిరిగి reshape(new_cn, 2, sz);
    }

    పూర్ణాంకం మొత్తం_వెడల్పు = నిలువుల_సంఖ్య * cn;

    ఒకవేళ( (new_cn > మొత్తం_వెడల్పు || మొత్తం_వెడల్పు % new_cn != 0) && new_అడ్డువరుసలు == 0 )
        new_అడ్డువరుసలు = అడ్డువరుసలు * మొత్తం_వెడల్పు / new_cn;

    ఒకవేళ( new_అడ్డువరుసలు != 0 && new_అడ్డువరుసలు != అడ్డువరుసలు )
    {
        పూర్ణాంకం మొత్తం_కొలత_2 = మొత్తం_వెడల్పు * అడ్డువరుసలు;
        ఒకవేళ( !isContinuous() )
            CV_Error( cv::Error::Bad_అడుగు,
            "The matrix is not continuous, thus its number of rows can not be changed" );

        మొత్తం_వెడల్పు = మొత్తం_కొలత_2 / new_అడ్డువరుసలు;

        ఒకవేళ( మొత్తం_వెడల్పు * new_అడ్డువరుసలు != మొత్తం_కొలత_2 )
            CV_Error( cv::Error::StsBadArg, "The total number of matrix elements "
                                    "is not divisible by the new number of rows" );

        hdr.size[0] = hdr.అడ్డువరుసలు = new_అడ్డువరుసలు;
        hdr.అడుగు[0] = మొత్తం_వెడల్పు * elemSize1();
    } లేకపోతే {
        hdr.size[0] = hdr.అడ్డువరుసలు = అడ్డువరుసలు;
        ఒకవేళ (dims <= 1)
            hdr.అడుగు[0] = నిలువుల_సంఖ్య * CV_ELEM_కొలత(flags);
    }

    పూర్ణాంకం new_వెడల్పు = మొత్తం_వెడల్పు / new_cn;

    ఒకవేళ( new_వెడల్పు * new_cn != మొత్తం_వెడల్పు )
        CV_Error( cv::Error::Bad_అంకె_Channels,
        "The total width is not divisible by the new number of channels" );

    hdr.size.dims = hdr.dims = 2;
    hdr.size[1] = hdr.నిలువుల_సంఖ్య = new_వెడల్పు;
    hdr.flags = (hdr.flags & ~CV_MAT_CN_ముసుగు) | ((new_cn-1) << CV_CN_జరుపు);
    hdr.అడుగు[1] = CV_ELEM_కొలత(hdr.flags);
    తిరిగి hdr;
}

Mat Mat::reshape(పూర్ణాంకం _cn, పూర్ణాంకం _newndims, స్థిరరాశి పూర్ణాంకం* _newsz) స్థిరరాశి
{
    ఒకవేళ(_newndims == dims)
    {
        ఒకవేళ(_newsz == 0)
            తిరిగి reshape(_cn);
        ఒకవేళ(_newndims == 2)
            తిరిగి reshape(_cn, _newsz[0]);
    }

    ఒకవేళ (isContinuous())
    {
        CV_Assert(_cn >= 0 && _newndims >= 0 && _newndims <= CV_MAX_DIM && (_newndims == 0 || _newsz != 0));

        ఒకవేళ (_cn == 0)
            _cn = ఇది->channels();
        లేకపోతే
            CV_Assert(_cn <= CV_CN_MAX);

        size_t మొత్తం_elem1_ref = ఇది->మొత్తం() * ఇది->channels();
        size_t మొత్తం_elem1 = _cn;

        Auto_బఫరు<పూర్ణాంకం, 4> newsz_buf( (size_t)_newndims );

        పూర్ణాంకం m1_idx = -1;

        కొరకు (పూర్ణాంకం i = 0; i < _newndims; i++)
        {
            ఒకవేళ (_newsz[i] >= 0) {
                ఒకవేళ (_newsz[i] == 0 && i < dims)
                    newsz_buf[i] = size.p[i];
                లేకపోతే
                    newsz_buf[i] = _newsz[i];
                మొత్తం_elem1 *= (size_t)newsz_buf[i];
            } లేకపోతే {
                ఒకవేళ (m1_idx >= 0)
                    CV_Error(cv::Error::Sts_Bad_కొలత, "More than one '-1' occured in the new shape");
                m1_idx = i;
            }
        }

        ఒకవేళ (m1_idx >= 0) {
            ఒకవేళ (మొత్తం_elem1 == 0) {
                CV_Assert(మొత్తం_elem1_ref == 0);
                మొత్తం_elem1 = 1;
            }
            CV_Assert(మొత్తం_elem1_ref % మొత్తం_elem1 == 0);
            newsz_buf[m1_idx] = (పూర్ణాంకం)(మొత్తం_elem1_ref / మొత్తం_elem1);
        } లేకపోతే ఒకవేళ (మొత్తం_elem1 != మొత్తం_elem1_ref) {
            CV_Error(cv::Error::StsUnmatchedSizes, "Requested and source matrices have different count of elements");
        }

        Mat hdr = *ఇది;
        hdr.flags = (hdr.flags & ~CV_MAT_CN_ముసుగు) | ((_cn-1) << CV_CN_జరుపు);
        సమితి_కొలత(hdr, _newndims, newsz_buf.దత్తాంశం(), ఖాళీ, నిజం);

        తిరిగి hdr;
    }

    CV_Error(cv::Error::StsNotImplemented, "Reshaping of n-dimensional non-continuous matrices is not supported yet");
    // TBD
}

Mat Mat::reshape(పూర్ణాంకం _cn, స్థిరరాశి std::vector<పూర్ణాంకం>& _newshape) స్థిరరాశి
{
    పూర్ణాంకం newdims = (పూర్ణాంకం)_newshape.size();
    ఒకవేళ(newdims == 0 && empty())
    {
        తిరిగి *ఇది;
    }

    తిరిగి reshape(_cn, newdims, newdims > 0 ? &_newshape[0] : 0);
}

Mat Mat::reshape(పూర్ణాంకం _cn, స్థిరరాశి MatShape& _newshape) స్థిరరాశి
{
    ఒకవేళ (_newshape.dims < 0) {
        పూర్ణాంకం newshape[] = {0};
        తిరిగి reshape(_cn, 1, newshape);
    }
    తిరిగి reshape(_cn, _newshape.dims, _newshape.p);
}

Mat Mat::reshape(పూర్ణాంకం _cn, std::initializer_list<పూర్ణాంకం> newshape_) స్థిరరాశి
{
    పూర్ణాంకం newshape[MatShape::MAX_DIMS];
    size_t i, newshape_dims = newshape_.size();
    CV_Assert(newshape_dims <= (size_t)MatShape::MAX_DIMS);
    స్వయంచాలక it = newshape_.begin();
    కొరకు (i = 0; i < newshape_dims; i++, ++it)
        newshape[i] = *it;
    తిరిగి reshape(_cn, (పూర్ణాంకం)newshape_dims, newshape);
}

Mat Mat::reinterpret(పూర్ణాంకం type) స్థిరరాశి
{
    type = CV_MAT_TYPE(type);
    CV_Assert(CV_ELEM_కొలత(ఇది->type()) == CV_ELEM_కొలత(type));
    Mat m = *ఇది;
    m.flags = (m.flags & ~CV_MAT_TYPE_ముసుగు) | type;
    m.updateContinuityFlag();
    తిరిగి m;
}

Mat Mat::diag(స్థిరరాశి Mat& d)
{
    CV_Assert( d.నిలువుల_సంఖ్య == 1 || d.అడ్డువరుసలు == 1 );
    పూర్ణాంకం len = d.అడ్డువరుసలు + d.నిలువుల_సంఖ్య - 1;
    Mat m(len, len, d.type(), Scalar(0));
    Mat md = m.diag();
    ఒకవేళ( d.నిలువుల_సంఖ్య == 1 )
        d.copy_గమ్యం(md);
    లేకపోతే
        transpose(d, md);
    తిరిగి m;
}

పూర్ణాంకం Mat::check_సదిశ(పూర్ణాంకం _elemChannels, పూర్ణాంకం లోతు_2, తార్కిక _requireContinuous) స్థిరరాశి
{
    తిరిగి దత్తాంశం && (లోతు() == లోతు_2 || లోతు_2 <= 0) &&
        (isContinuous() || !_requireContinuous) &&
        ((dims <= 2 && (((అడ్డువరుసలు == 1 || నిలువుల_సంఖ్య == 1) && channels() == _elemChannels) ||
                        (నిలువుల_సంఖ్య == _elemChannels && channels() == 1))) ||
        (dims == 3 && channels() == 1 && size.p[2] == _elemChannels && (size.p[0] == 1 || size.p[1] == 1) &&
         (isContinuous() || అడుగు.p[1] == అడుగు.p[2]*size.p[2])))
    ? (పూర్ణాంకం)(మొత్తం()*channels()/_elemChannels) : -1;
}


స్థిర అంతఃవరుస కొలత get_Continuous_కొలత(పూర్ణాంకం flags, పూర్ణాంకం నిలువుల_సంఖ్య, పూర్ణాంకం అడ్డువరుసలు, పూర్ణాంకం వెడల్పు_Scale)
{
    int64 sz = (int64)నిలువుల_సంఖ్య * అడ్డువరుసలు * వెడల్పు_Scale;
    తార్కిక has_int_అధికప్రవాహం = sz >= గరిష్ఠ_పూర్ణాంకం;
    తార్కిక isContiguous = (flags & Mat::CONTINUOUS_FLAG) != 0;
    తిరిగి (isContiguous && !has_int_అధికప్రవాహం)
            ? కొలత((పూర్ణాంకం)sz, 1)
            : కొలత(నిలువుల_సంఖ్య * వెడల్పు_Scale, అడ్డువరుసలు);
}

కొలత getContinuousSize2D(Mat& m1, పూర్ణాంకం వెడల్పు_Scale)
{
    CV_CheckLE(m1.dims, 2, "");
    తిరిగి get_Continuous_కొలత(m1.flags,
                              m1.నిలువుల_సంఖ్య, m1.అడ్డువరుసలు, వెడల్పు_Scale);
}
కొలత getContinuousSize2D(Mat& m1, Mat& m2, పూర్ణాంకం వెడల్పు_Scale)
{
    CV_CheckLE(m1.dims, 2, "");
    CV_CheckLE(m2.dims, 2, "");
    స్థిరరాశి కొలత sz1 = m1.size();
    ఒకవేళ (sz1 != m2.size())  // reshape all matrixes to the same size (#4159)
    {
        size_t మొత్తం_sz = m1.మొత్తం();
        CV_CheckEQ(మొత్తం_sz, m2.మొత్తం(), "");
        తార్కిక is_m1_సదిశ = m1.నిలువుల_సంఖ్య == 1 || m1.అడ్డువరుసలు == 1;
        తార్కిక is_m2_సదిశ = m2.నిలువుల_సంఖ్య == 1 || m2.అడ్డువరుసలు == 1;
        CV_Assert(is_m1_సదిశ); CV_Assert(is_m2_సదిశ);
        పూర్ణాంకం మొత్తం = (పూర్ణాంకం)మొత్తం_sz;  // vector-column
        తార్కిక isContiguous = ((m1.flags & m2.flags) & Mat::CONTINUOUS_FLAG) != 0;
        తార్కిక has_int_అధికప్రవాహం = ((int64)మొత్తం_sz * వెడల్పు_Scale) >= గరిష్ఠ_పూర్ణాంకం;
        ఒకవేళ (isContiguous && !has_int_అధికప్రవాహం)
            మొత్తం = 1; // vector-row
        m1 = m1.reshape(0, మొత్తం);
        m2 = m2.reshape(0, మొత్తం);
        CV_Assert(m1.నిలువుల_సంఖ్య == m2.నిలువుల_సంఖ్య && m1.అడ్డువరుసలు == m2.అడ్డువరుసలు);
        తిరిగి కొలత(m1.నిలువుల_సంఖ్య * వెడల్పు_Scale, m1.అడ్డువరుసలు);
    }
    తిరిగి get_Continuous_కొలత(m1.flags & m2.flags,
                              m1.నిలువుల_సంఖ్య, m1.అడ్డువరుసలు, వెడల్పు_Scale);
}

కొలత getContinuousSize2D(Mat& m1, Mat& m2, Mat& m3, పూర్ణాంకం వెడల్పు_Scale)
{
    CV_CheckLE(m1.dims, 2, "");
    CV_CheckLE(m2.dims, 2, "");
    CV_CheckLE(m3.dims, 2, "");
    స్థిరరాశి కొలత sz1 = m1.size();
    ఒకవేళ (sz1 != m2.size() || sz1 != m3.size())  // reshape all matrixes to the same size (#4159)
    {
        size_t మొత్తం_sz = m1.మొత్తం();
        CV_CheckEQ(మొత్తం_sz, m2.మొత్తం(), "");
        CV_CheckEQ(మొత్తం_sz, m3.మొత్తం(), "");
        తార్కిక is_m1_సదిశ = m1.నిలువుల_సంఖ్య == 1 || m1.అడ్డువరుసలు == 1;
        తార్కిక is_m2_సదిశ = m2.నిలువుల_సంఖ్య == 1 || m2.అడ్డువరుసలు == 1;
        తార్కిక is_m3_సదిశ = m3.నిలువుల_సంఖ్య == 1 || m3.అడ్డువరుసలు == 1;
        CV_Assert(is_m1_సదిశ); CV_Assert(is_m2_సదిశ); CV_Assert(is_m3_సదిశ);
        పూర్ణాంకం మొత్తం = (పూర్ణాంకం)మొత్తం_sz;  // vector-column
        తార్కిక isContiguous = ((m1.flags & m2.flags & m3.flags) & Mat::CONTINUOUS_FLAG) != 0;
        తార్కిక has_int_అధికప్రవాహం = ((int64)మొత్తం_sz * వెడల్పు_Scale) >= గరిష్ఠ_పూర్ణాంకం;
        ఒకవేళ (isContiguous && !has_int_అధికప్రవాహం)
            మొత్తం = 1; // vector-row
        m1 = m1.reshape(0, మొత్తం);
        m2 = m2.reshape(0, మొత్తం);
        m3 = m3.reshape(0, మొత్తం);
        CV_Assert(m1.నిలువుల_సంఖ్య == m2.నిలువుల_సంఖ్య && m1.అడ్డువరుసలు == m2.అడ్డువరుసలు && m1.నిలువుల_సంఖ్య == m3.నిలువుల_సంఖ్య && m1.అడ్డువరుసలు == m3.అడ్డువరుసలు);
        తిరిగి కొలత(m1.నిలువుల_సంఖ్య * వెడల్పు_Scale, m1.అడ్డువరుసలు);
    }
    తిరిగి get_Continuous_కొలత(m1.flags & m2.flags & m3.flags,
                              m1.నిలువుల_సంఖ్య, m1.అడ్డువరుసలు, వెడల్పు_Scale);
}

} // cv::
