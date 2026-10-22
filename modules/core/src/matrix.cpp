// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#അന്തര്ഭാവ "precomp.hpp"
#അന്തര്ഭാവ "bufferpool.impl.hpp"

നാമാകാശ cv {

std::string layoutToString(DataLayout layout)
{
    തിരികെ
        layout == DATA_LAYOUT_ND ? "ND" :
        layout == DATA_LAYOUT_NCHW ? "NCHW" :
        layout == DATA_LAYOUT_NHWC ? "NHWC" :
        layout == DATA_LAYOUT_BLOCK ? "BLOCK" :
        layout == DATA_LAYOUT_NCDHW ? "NCDHW" :
        layout == DATA_LAYOUT_NDHWC ? "NDHWC" :
        layout == DATA_LAYOUT_PLANAR ? "PLANAR" :
        layout == DATA_LAYOUT_UNKNOWN ? "Unknown" : "???";
}

തർക്കം operator == (സ്ഥിരം MatShape& size1, സ്ഥിരം MatShape& size2)
{
    എങ്കിൽ (size1.dims != size2.dims)
        തിരികെ അസത്യം;
    എങ്കിൽ (size1.layout != size2.layout &&
        size1.layout != DATA_LAYOUT_UNKNOWN &&
        size2.layout != DATA_LAYOUT_UNKNOWN)
        തിരികെ അസത്യം;
    എങ്കിൽ (size1.layout == DATA_LAYOUT_BLOCK &&
        size2.layout == DATA_LAYOUT_BLOCK &&
        size1.C != size2.C)
        തിരികെ അസത്യം;
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < size1.dims; i++) {
        എങ്കിൽ (size1.p[i] != size2.p[i])
            തിരികെ അസത്യം;
    }
    തിരികെ സത്യം;
}

തർക്കം operator != (സ്ഥിരം MatShape& size1, സ്ഥിരം MatShape& size2)
{
    തിരികെ !(size1 == size2);
}

/////////////////////////// MatShape ////////////////////////////////

MatShape MatShape::scalar()
{
    തിരികെ MatShape(0);
}

ശൂന്യം MatShape::clear()
{
    dims = -1;
    layout = DATA_LAYOUT_UNKNOWN;
    C = 0;
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < MAX_DIMS; i++)
        p[i] = 0;
}

ശൂന്യം MatShape::resize(size_t newSize, പൂർണ്ണസംഖ്യ value)
{
    CV_Assert(newSize < (size_t)MAX_DIMS);
    പൂർണ്ണസംഖ്യ old_dims = std::max(dims, 0);
    dims = (പൂർണ്ണസംഖ്യ)newSize;
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = old_dims; i < dims; i++)
        p[i] = value;
}

ശൂന്യം MatShape::reserve(size_t)
{
    // no op; maybe need to add a check for overflow, but we check it anyway in other operations
}

ശൂന്യം MatShape::assign(size_t newSize, പൂർണ്ണസംഖ്യ value)
{
    CV_Assert(newSize < (size_t)MAX_DIMS);
    dims = (പൂർണ്ണസംഖ്യ)newSize;
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < dims; i++)
        p[i] = value;
}

ശൂന്യം MatShape::assign(പൂർണ്ണസംഖ്യ newSize, പൂർണ്ണസംഖ്യ value)
{
    assign((size_t)newSize, value);
}

ശൂന്യം MatShape::assign(സ്ഥിരം പൂർണ്ണസംഖ്യ* begin, സ്ഥിരം പൂർണ്ണസംഖ്യ* end)
{
    assign_(begin, end);
}

ശൂന്യം MatShape::assign_(സ്ഥിരം പൂർണ്ണസംഖ്യ* begin, സ്ഥിരം പൂർണ്ണസംഖ്യ* end)
{
    ptrdiff_t newSize = end - begin;
    CV_Assert(0 <= newSize && newSize < (ptrdiff_t)MAX_DIMS);
    dims = (പൂർണ്ണസംഖ്യ)newSize;
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < dims; i++)
        p[i] = begin[i];
}

പൂർണ്ണസംഖ്യ* MatShape::begin() { തിരികെ p; }
സ്ഥിരം പൂർണ്ണസംഖ്യ* MatShape::begin() സ്ഥിരം { തിരികെ p; }
പൂർണ്ണസംഖ്യ* MatShape::end() { തിരികെ p + std::max(dims, 0); }
സ്ഥിരം പൂർണ്ണസംഖ്യ* MatShape::end() സ്ഥിരം { തിരികെ p + std::max(dims, 0); }
പൂർണ്ണസംഖ്യ& MatShape::back() { തിരികെ p[std::max(dims-1, 0)]; }
സ്ഥിരം പൂർണ്ണസംഖ്യ& MatShape::back() സ്ഥിരം { തിരികെ p[std::max(dims-1, 0)]; }

ശൂന്യം MatShape::push_back(പൂർണ്ണസംഖ്യ value)
{
    CV_Assert(dims+1 < MAX_DIMS);
    dims = std::max(dims+1, 1);
    p[dims-1] = value;
}

ശൂന്യം MatShape::emplace_back(പൂർണ്ണസംഖ്യ value)
{
    push_back(value);
}

ശൂന്യം MatShape::insert(പൂർണ്ണസംഖ്യ* where, പൂർണ്ണസംഖ്യ value)
{
    പൂർണ്ണസംഖ്യ old_dims = std::max(dims, 0);
    CV_Assert(old_dims+1 < MAX_DIMS);
    ptrdiff_t ofs = where - p;
    CV_Assert(0 <= ofs && ofs <= old_dims);
    dims = old_dims+1;
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = old_dims-1; i >= (പൂർണ്ണസംഖ്യ)ofs; i--)
        p[i+1] = p[i];
    p[ofs] = value;
}

ശൂന്യം MatShape::insert(പൂർണ്ണസംഖ്യ* where, size_t എണ്ണം_2, പൂർണ്ണസംഖ്യ value)
{
    പൂർണ്ണസംഖ്യ old_dims = std::max(dims, 0);
    CV_Assert((size_t)(old_dims+എണ്ണം_2) < (size_t)MAX_DIMS);
    ptrdiff_t ofs = where - p;
    CV_Assert(0 <= ofs && ofs <= old_dims);
    dims = (പൂർണ്ണസംഖ്യ)(old_dims+എണ്ണം_2);
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = old_dims-1; i >= (പൂർണ്ണസംഖ്യ)ofs; i--)
        p[i+എണ്ണം_2] = p[i];
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < (പൂർണ്ണസംഖ്യ)എണ്ണം_2; i++)
        p[i+ofs] = value;
}

ശൂന്യം MatShape::insert(പൂർണ്ണസംഖ്യ* where, പൂർണ്ണസംഖ്യ എണ്ണം_2, പൂർണ്ണസംഖ്യ value)
{
    insert(where, (size_t)എണ്ണം_2, value);
}

ശൂന്യം MatShape::insert(പൂർണ്ണസംഖ്യ* where, സ്ഥിരം പൂർണ്ണസംഖ്യ* begin, സ്ഥിരം പൂർണ്ണസംഖ്യ* end)
{
    തിരുകുക(where, begin, end);
}

ശൂന്യം MatShape::തിരുകുക(പൂർണ്ണസംഖ്യ* where, സ്ഥിരം പൂർണ്ണസംഖ്യ* begin, സ്ഥിരം പൂർണ്ണസംഖ്യ* end)
{
    പൂർണ്ണസംഖ്യ old_dims = std::max(dims, 0);
    ptrdiff_t delta = end - begin;
    CV_Assert(0 <= delta && old_dims+delta < MAX_DIMS);
    ptrdiff_t ofs = where - p;
    CV_Assert(0 <= ofs && ofs <= old_dims);
    dims = (പൂർണ്ണസംഖ്യ)(old_dims+delta);
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = old_dims-1; i >= (പൂർണ്ണസംഖ്യ)ofs; i--)
        p[i+delta] = p[i];
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < (പൂർണ്ണസംഖ്യ)delta; i++)
        p[i+ofs] = begin[i];
}

ശൂന്യം MatShape::erase(പൂർണ്ണസംഖ്യ* where)
{
    CV_Assert(dims > 0);
    ptrdiff_t ofs = where - p;
    CV_Assert(0 <= ofs && ofs <= dims);
    എങ്കിൽ (ofs == dims)
        തിരികെ;
    dims--;
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = (പൂർണ്ണസംഖ്യ)ofs+1; i <= dims; i++)
        p[i-1] = p[i];
}

size_t MatShape::total() സ്ഥിരം
{
    size_t result = 1;
    എങ്കിൽ (dims < 0)
        തിരികെ 0;
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < dims; i++)
        result *= p[i];
    തിരികെ result;
}

std::vector<പൂർണ്ണസംഖ്യ> MatShape::vec() സ്ഥിരം
{
    എങ്കിൽ (dims < 0)
        തിരികെ std::vector<പൂർണ്ണസംഖ്യ>(1, 0);
    തിരികെ std::vector<പൂർണ്ണസംഖ്യ>(p, p + dims);
}

std::string MatShape::str() സ്ഥിരം
{
    std::stringstream sstrm;
    എങ്കിൽ (empty()) {
        sstrm << "<empty>";
    } അല്ലെങ്കിൽ എങ്കിൽ (dims == 0) {
        sstrm << "<scalar>";
    } അല്ലെങ്കിൽ {
        sstrm << "[";
        വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < dims; i++) {
            sstrm << (i > 0 ? " x " : "") << p[i];
        }
        sstrm << "]";
    }
    തിരികെ sstrm.str();
}

സ്ഥിതം ശൂന്യം finalizeBlockLayout(MatShape& size, പൂർണ്ണസംഖ്യ C=0)
{
    എങ്കിൽ (size.layout == DATA_LAYOUT_BLOCK) {
        CV_Assert(size.dims >= 4);
        പൂർണ്ണസംഖ്യ C0 = size.p[size.dims-1];
        CV_Assert(C0 > 1 && (C0 & (C0-1)) == 0);
        size.C = C > 0 ? C : size.p[1]*size.p[size.dims-1];
    } അല്ലെങ്കിൽ {
        size.C = 0;
    }
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = std::max(size.dims, 0); i < MatShape::MAX_DIMS; i++)
        size.p[i] = 0;
    എങ്കിൽ (size.dims == 0)
        size.p[0] = 1;
}

MatShape::MatShape()
{
    clear();
}

MatShape::MatShape(size_t dims_, സ്ഥിരം പൂർണ്ണസംഖ്യ* size_, DataLayout layout_, പൂർണ്ണസംഖ്യ C_)
{
    layout = layout_;
    CV_Assert(dims_ <= (size_t)MAX_DIMS);
    dims = (പൂർണ്ണസംഖ്യ)dims_;
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < dims; i++) {
        p[i] = size_ ? size_[i] : 0;
    }
    finalizeBlockLayout(*ഇത്, C_);
}

MatShape::MatShape(size_t dims_, പൂർണ്ണസംഖ്യ value, DataLayout layout_)
{
    layout = layout_;
    CV_Assert(dims_ <= (size_t)MAX_DIMS);
    dims = (പൂർണ്ണസംഖ്യ)dims_;
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < dims; i++) {
        p[i] = value;
    }
    finalizeBlockLayout(*ഇത്, 0);
}

MatShape::MatShape(std::initializer_list<പൂർണ്ണസംഖ്യ> shape)
{
    layout = DATA_LAYOUT_UNKNOWN;
    CV_Assert(shape.size() <= (size_t)MAX_DIMS);
    dims = (പൂർണ്ണസംഖ്യ)shape.size();
    auto it = shape.begin();
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < dims; i++, ++it) {
        p[i] = *it;
    }
    finalizeBlockLayout(*ഇത്, 0);
}

MatShape::MatShape(പൂർണ്ണസംഖ്യ dims_, പൂർണ്ണസംഖ്യ value, DataLayout layout_)
{
    layout = layout_;
    CV_Assert(dims_ <= MAX_DIMS);
    dims = dims_;
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < dims; i++) {
        p[i] = value;
    }
    finalizeBlockLayout(*ഇത്, 0);
}

MatShape::MatShape(സ്ഥിരം std::vector<പൂർണ്ണസംഖ്യ>& shape_, DataLayout layout_, പൂർണ്ണസംഖ്യ C_)
{
    layout = layout_;
    size_t shape_size = shape_.size();
    CV_Assert(shape_size < (size_t)MAX_DIMS);
    dims = (പൂർണ്ണസംഖ്യ)shape_size;
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < dims; i++) {
        p[i] = shape_[i];
    }
    finalizeBlockLayout(*ഇത്, C_);
}

MatShape::MatShape(സ്ഥിരം പൂർണ്ണസംഖ്യ* begin, സ്ഥിരം പൂർണ്ണസംഖ്യ* end, DataLayout layout_, പൂർണ്ണസംഖ്യ C_)
{
    layout = layout_;
    ptrdiff_t shape_size = end - begin;
    CV_Assert(0 <= shape_size && shape_size < MAX_DIMS);
    dims = (പൂർണ്ണസംഖ്യ)shape_size;
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < dims; i++) {
        p[i] = begin[i];
    }
    finalizeBlockLayout(*ഇത്, C_);
}

MatShape::MatShape(സ്ഥിരം MatShape& shape)
{
    dims = shape.dims;
    layout = shape.layout;
    C = shape.C;
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < MAX_DIMS; i++)
        p[i] = shape.p[i];
}

MatShape& MatShape::operator = (സ്ഥിരം MatShape& shape)
{
    എങ്കിൽ (ഇത് != &shape) {
        dims = shape.dims;
        layout = shape.layout;
        C = shape.C;
        വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < MAX_DIMS; i++)
            p[i] = shape.p[i];
    }
    തിരികെ *ഇത്;
}

തർക്കം MatShape::hasSymbols() സ്ഥിരം
{
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < dims; i++) {
        എങ്കിൽ (p[i] < 0)
            തിരികെ സത്യം;
    }
    തിരികെ അസത്യം;
}

പൂർണ്ണസംഖ്യ MatShape::channels() സ്ഥിരം
{
    CV_Assert(layout == DATA_LAYOUT_BLOCK || layout == DATA_LAYOUT_NCHW || layout == DATA_LAYOUT_NHWC);
    തിരികെ layout == DATA_LAYOUT_BLOCK ? C : p[layout == DATA_LAYOUT_NCHW ? 1 : dims-1];
}

MatShape MatShape::toLayout(DataLayout newLayout, പൂർണ്ണസംഖ്യ C0) സ്ഥിരം
{
    CV_Assert(layout == DATA_LAYOUT_BLOCK || layout == DATA_LAYOUT_NCHW || layout == DATA_LAYOUT_NHWC);
    CV_Assert(newLayout == DATA_LAYOUT_BLOCK || newLayout == DATA_LAYOUT_NCHW || newLayout == DATA_LAYOUT_NHWC);

    MatShape newsize = *ഇത്;
    newsize.layout = newLayout;

    എങ്കിൽ (newLayout == DATA_LAYOUT_BLOCK) {
        // any => BLOCK
        CV_Assert_N(C0 > 1, (C0 & (C0-1)) == 0);
        പൂർണ്ണസംഖ്യ Corig = channels();
        newsize.C = Corig;

        എങ്കിൽ (layout == DATA_LAYOUT_NHWC) {
            വേണ്ടി (പൂർണ്ണസംഖ്യ i = 2; i < dims; i++) {
                newsize.p[i] = p[i-1];
            }
        }

        newsize.dims += layout != DATA_LAYOUT_BLOCK;
        newsize.p[1] = (Corig + C0 - 1)/C0;
        newsize.p[newsize.dims-1] = C0;
    } അല്ലെങ്കിൽ എങ്കിൽ (layout == DATA_LAYOUT_BLOCK) {
        // BLOCK => any (except for BLOCK, which is handled above)
        newsize.C = 0;
        എങ്കിൽ (newLayout == DATA_LAYOUT_NHWC) {
            വേണ്ടി (പൂർണ്ണസംഖ്യ i = 2; i < dims; i++) {
                newsize.p[i-1] = p[i];
            }
        }
        newsize.p[newLayout == DATA_LAYOUT_NCHW ? 1 : dims-2] = C;
        newsize.dims--;
    } അല്ലെങ്കിൽ {
        CV_Assert_N(C0 <= 1);

        എങ്കിൽ (newLayout == layout)
            തിരികെ newsize;

        // NHWC => NCHW
        എങ്കിൽ (newLayout == DATA_LAYOUT_NCHW) {
            വേണ്ടി (പൂർണ്ണസംഖ്യ i = 2; i < dims; i++)
                newsize.p[i] = p[i-1];
            newsize.p[1] = p[dims-1];
        } അല്ലെങ്കിൽ {
            // NCHW => NHWC
            വേണ്ടി (പൂർണ്ണസംഖ്യ i = 2; i < dims; i++)
                newsize.p[i-1] = p[i];
            newsize.p[dims-1] = p[1];
        }
    }

    തിരികെ newsize;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ normalizeShapeAxis(പൂർണ്ണസംഖ്യ axis, പൂർണ്ണസംഖ്യ dims)
{
    CV_CheckGE(axis, -dims, "axis is out of range");
    CV_CheckLT(axis, dims, "axis is out of range");
    തിരികെ axis < 0 ? axis + dims : axis;
}

// The semantic NCHW form {N, C, spatial...} of a block ('NC1HWC0') shape, so that axes and
// element counts are the logical ones rather than the C0-padded ones.
സ്ഥിതം MatShape toSemantic(സ്ഥിരം MatShape& shape)
{
    CV_CheckGE(shape.dims, 3, "a block shape is {N, C1, spatial..., C0}");
    തിരികെ shape.toLayout(DATA_LAYOUT_NCHW);
}

// Blocks such a result again. Once N or C changes, elements cross the padded C0 lanes and
// the data needs a repack, which a shape operation cannot express.
സ്ഥിതം MatShape toBlockOf(MatShape result, സ്ഥിരം MatShape& src, സ്ഥിരം അക്ഷരം* op)
{
    എങ്കിൽ (result.dims < 2 || result.p[0] != src.p[0] || result.p[1] != src.C)
        CV_Error_(Error::StsNotImplemented,
                  ("%s: the block layout is kept only when the batch and the channel axes stay "
                   "unchanged; convert the shape with toLayout() first", op));
    CV_CheckLE(result.dims + 1, (പൂർണ്ണസംഖ്യ)MatShape::MAX_DIMS, "too many dimensions");
    result.layout = DATA_LAYOUT_NCHW;
    തിരികെ result.toLayout(DATA_LAYOUT_BLOCK, src.p[src.dims-1]);
}

MatShape MatShape::squeeze(സ്ഥിരം std::vector<പൂർണ്ണസംഖ്യ>& axes) സ്ഥിരം
{
    CV_Assert(dims >= 0);
    എങ്കിൽ (layout == DATA_LAYOUT_BLOCK)
        തിരികെ toBlockOf(toSemantic(*ഇത്).squeeze(axes), *ഇത്, "squeeze");
    തർക്കം remove[MAX_DIMS] = {അസത്യം};
    എങ്കിൽ (axes.empty())
    {
        വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < dims; i++)
            remove[i] = p[i] == 1;
    }
    വേണ്ടി (പൂർണ്ണസംഖ്യ a : axes)
    {
        പൂർണ്ണസംഖ്യ i = normalizeShapeAxis(a, dims);
        CV_Check(a, !remove[i], "squeeze: duplicate axis");
        CV_Check(p[i], p[i] == 1, "squeeze: the size of a squeezed axis must be 1");
        remove[i] = സത്യം;
    }
    // a new number of axes: the layout of this shape does not apply
    MatShape result;
    result.dims = 0;
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < dims; i++)
        എങ്കിൽ (!remove[i])
            result.p[result.dims++] = p[i];
    തിരികെ result;
}

MatShape MatShape::unsqueeze(സ്ഥിരം std::vector<പൂർണ്ണസംഖ്യ>& axes) സ്ഥിരം
{
    CV_Assert(dims >= 0);
    എങ്കിൽ (layout == DATA_LAYOUT_BLOCK)
        തിരികെ toBlockOf(toSemantic(*ഇത്).unsqueeze(axes), *ഇത്, "unsqueeze");
    സ്ഥിരം പൂർണ്ണസംഖ്യ outDims = dims + (പൂർണ്ണസംഖ്യ)axes.size();
    CV_CheckLE(outDims, (പൂർണ്ണസംഖ്യ)MAX_DIMS, "unsqueeze: too many dimensions");
    തർക്കം insert[MAX_DIMS] = {അസത്യം};
    വേണ്ടി (പൂർണ്ണസംഖ്യ a : axes)
    {
        പൂർണ്ണസംഖ്യ i = normalizeShapeAxis(a, outDims);
        CV_Check(a, !insert[i], "unsqueeze: duplicate axis");
        insert[i] = സത്യം;
    }
    // a new number of axes: the layout of this shape does not apply
    MatShape result;
    result.dims = outDims;
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0, j = 0; i < outDims; i++)
        result.p[i] = insert[i] ? 1 : p[j++];
    തിരികെ result;
}

MatShape MatShape::flatten(പൂർണ്ണസംഖ്യ startAxis, പൂർണ്ണസംഖ്യ endAxis) സ്ഥിരം
{
    CV_Assert(dims >= 0);
    എങ്കിൽ (layout == DATA_LAYOUT_BLOCK)
        തിരികെ toBlockOf(toSemantic(*ഇത്).flatten(startAxis, endAxis), *ഇത്, "flatten");
    എങ്കിൽ (dims == 0)
    {
        CV_Check(startAxis, startAxis == 0 || startAxis == -1, "flatten: axis is out of range");
        CV_Check(endAxis, endAxis == 0 || endAxis == -1, "flatten: axis is out of range");
        തിരികെ MatShape({1});
    }
    പൂർണ്ണസംഖ്യ start = normalizeShapeAxis(startAxis, dims), end = normalizeShapeAxis(endAxis, dims);
    CV_CheckLE(start, end, "flatten: startAxis must not follow endAxis");
    int64 merged = 1;
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = start; i <= end; i++)
        merged *= p[i];
    CV_CheckLE((ഇരട്ട)merged, (ഇരട്ട)INT_MAX, "flatten: the result is too big");
    // a new number of axes: the layout of this shape does not apply
    MatShape result;
    result.dims = 0;
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < dims; i++)
    {
        എങ്കിൽ (i == start)
            result.p[result.dims++] = (പൂർണ്ണസംഖ്യ)merged;
        അല്ലെങ്കിൽ എങ്കിൽ (i < start || i > end)
            result.p[result.dims++] = p[i];
    }
    തിരികെ result;
}

MatShape MatShape::reshape(സ്ഥിരം MatShape& newShape, തർക്കം allowZero) സ്ഥിരം
{
    CV_Assert(dims >= 0 && newShape.dims >= 0);
    എങ്കിൽ (layout == DATA_LAYOUT_BLOCK)
        തിരികെ toBlockOf(toSemantic(*ഇത്).reshape(newShape, allowZero), *ഇത്, "reshape");
    // a new number of axes: neither the layout of this shape nor the one of the spec applies
    MatShape result;
    result.dims = newShape.dims;
    std::copy(newShape.p, newShape.p + newShape.dims, result.p);
    പൂർണ്ണസംഖ്യ inferred = -1;
    int64 known = 1;
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < result.dims; i++)
    {
        പൂർണ്ണസംഖ്യ& sz = result.p[i];
        എങ്കിൽ (sz == -1)
        {
            CV_Check(i, inferred < 0, "reshape: at most one axis may be -1");
            inferred = i;
            തുടരുക;
        }
        CV_Check(sz, sz >= 0, "reshape: sizes must be -1 or non-negative");
        എങ്കിൽ (sz == 0 && !allowZero)
        {
            CV_Check(i, i < dims, "reshape: 0 refers to an axis the source does not have");
            sz = p[i];
        }
        known *= sz;
    }
    സ്ഥിരം int64 total_ = (int64)total();
    എങ്കിൽ (inferred >= 0)
    {
        CV_Check((ഇരട്ട)known, known != 0, "reshape: cannot infer the -1 axis next to a zero-size axis");
        CV_Check((ഇരട്ട)total_, total_ % known == 0, "reshape: the number of elements does not match");
        CV_CheckLE((ഇരട്ട)(total_/known), (ഇരട്ട)INT_MAX, "reshape: the result is too big");
        result.p[inferred] = (പൂർണ്ണസംഖ്യ)(total_/known);
    }
    അല്ലെങ്കിൽ എങ്കിൽ (known != total_)
        CV_Error_(Error::StsBadArg, ("reshape: the number of elements does not match: %lld vs %lld",
                                     (long long)known, (long long)total_));
    തിരികെ result;
}

MatShape MatShape::expand(സ്ഥിരം MatShape& another) സ്ഥിരം
{
    എങ്കിൽ (dims == 0)
        തിരികെ another;
    എങ്കിൽ (another.dims == 0)
        തിരികെ *ഇത്;

    എങ്കിൽ ((layout == DATA_LAYOUT_NCHW || layout == DATA_LAYOUT_NHWC) &&
        (another.layout == DATA_LAYOUT_NCHW || another.layout == DATA_LAYOUT_NHWC)) {
        CV_Assert(layout == another.layout);
    }
    // [TODO] support block layout
    CV_Assert(layout != DATA_LAYOUT_BLOCK && another.layout != DATA_LAYOUT_BLOCK);

    MatShape result;

    എങ്കിൽ (dims < 0 || another.dims < 0)
        തിരികെ result;

    result = *ഇത്;
    result.dims = std::max(dims, another.dims);
    result.layout = layout == DATA_LAYOUT_UNKNOWN ? another.layout :
        layout == DATA_LAYOUT_ND && (another.layout == DATA_LAYOUT_NCHW ||
        another.layout == DATA_LAYOUT_NHWC) ? another.layout : layout;
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = result.dims-1; i >= 0; i--) {
        പൂർണ്ണസംഖ്യ i1 = i - (result.dims - dims);
        പൂർണ്ണസംഖ്യ i2 = i - (result.dims - another.dims);
        പൂർണ്ണസംഖ്യ sz1 = i1 < 0 ? 1 : p[i1];
        പൂർണ്ണസംഖ്യ sz2 = i2 < 0 ? 1 : another.p[i2];
        CV_Assert(sz1 == sz2 || sz1 == 1 || sz2 == 1);
        // [TODO] handle symbolic shapes
        result.p[i] = sz1 != 1 ? sz1 : sz2;
    }
    തിരികെ result;
}

/////////////////////////// MatAllocator ////////////////////////////

ശൂന്യം MatAllocator::map(UMatData*, AccessFlag) സ്ഥിരം
{
}

ശൂന്യം MatAllocator::unmap(UMatData* u) സ്ഥിരം
{
    എങ്കിൽ(u->urefcount == 0 && u->refcount == 0)
    {
        deallocate(u);
    }
}

ശൂന്യം MatAllocator::download(UMatData* u, ശൂന്യം* dstptr,
         പൂർണ്ണസംഖ്യ dims, സ്ഥിരം size_t sz[],
         സ്ഥിരം size_t srcofs[], സ്ഥിരം size_t srcstep[],
         സ്ഥിരം size_t dststep[]) സ്ഥിരം
{
    എങ്കിൽ(!u)
        തിരികെ;
    പൂർണ്ണസംഖ്യ isz[CV_MAX_DIM];
    uchar* srcptr = u->data;
    വേണ്ടി( പൂർണ്ണസംഖ്യ i = 0; i < dims; i++ )
    {
        CV_Assert( sz[i] <= (size_t)INT_MAX );
        എങ്കിൽ( sz[i] == 0 )
            തിരികെ;
        എങ്കിൽ( srcofs )
            srcptr += srcofs[i]*(i <= dims-2 ? srcstep[i] : 1);
        isz[i] = (പൂർണ്ണസംഖ്യ)sz[i];
    }

    Mat src(dims, isz, CV_8U, srcptr, srcstep);
    Mat dst(dims, isz, CV_8U, dstptr, dststep);

    സ്ഥിരം Mat* arrays[] = { &src, &dst };
    uchar* ptrs[2];
    NAryMatIterator it(arrays, ptrs, 2);
    size_t planesz = it.size;

    വേണ്ടി( size_t j = 0; j < it.nplanes; j++, ++it )
        memcpy(ptrs[1], ptrs[0], planesz);
}


ശൂന്യം MatAllocator::upload(UMatData* u, സ്ഥിരം ശൂന്യം* srcptr, പൂർണ്ണസംഖ്യ dims, സ്ഥിരം size_t sz[],
                    സ്ഥിരം size_t dstofs[], സ്ഥിരം size_t dststep[],
                    സ്ഥിരം size_t srcstep[]) സ്ഥിരം
{
    എങ്കിൽ(!u)
        തിരികെ;
    പൂർണ്ണസംഖ്യ isz[CV_MAX_DIM];
    uchar* dstptr = u->data;
    വേണ്ടി( പൂർണ്ണസംഖ്യ i = 0; i < dims; i++ )
    {
        CV_Assert( sz[i] <= (size_t)INT_MAX );
        എങ്കിൽ( sz[i] == 0 )
            തിരികെ;
        എങ്കിൽ( dstofs )
            dstptr += dstofs[i]*(i <= dims-2 ? dststep[i] : 1);
        isz[i] = (പൂർണ്ണസംഖ്യ)sz[i];
    }

    Mat src(dims, isz, CV_8U, (ശൂന്യം*)srcptr, srcstep);
    Mat dst(dims, isz, CV_8U, dstptr, dststep);

    സ്ഥിരം Mat* arrays[] = { &src, &dst };
    uchar* ptrs[2];
    NAryMatIterator it(arrays, ptrs, 2);
    size_t planesz = it.size;

    വേണ്ടി( size_t j = 0; j < it.nplanes; j++, ++it )
        memcpy(ptrs[1], ptrs[0], planesz);
}

ശൂന്യം MatAllocator::copy(UMatData* usrc, UMatData* udst, പൂർണ്ണസംഖ്യ dims, സ്ഥിരം size_t sz[],
                  സ്ഥിരം size_t srcofs[], സ്ഥിരം size_t srcstep[],
                  സ്ഥിരം size_t dstofs[], സ്ഥിരം size_t dststep[], തർക്കം /*sync*/) സ്ഥിരം
{
    CV_INSTRUMENT_REGION();

    എങ്കിൽ(!usrc || !udst)
        തിരികെ;
    പൂർണ്ണസംഖ്യ isz[CV_MAX_DIM];
    uchar* srcptr = usrc->data;
    uchar* dstptr = udst->data;
    വേണ്ടി( പൂർണ്ണസംഖ്യ i = 0; i < dims; i++ )
    {
        CV_Assert( sz[i] <= (size_t)INT_MAX );
        എങ്കിൽ( sz[i] == 0 )
            തിരികെ;
        എങ്കിൽ( srcofs )
            srcptr += srcofs[i]*(i <= dims-2 ? srcstep[i] : 1);
        എങ്കിൽ( dstofs )
            dstptr += dstofs[i]*(i <= dims-2 ? dststep[i] : 1);
        isz[i] = (പൂർണ്ണസംഖ്യ)sz[i];
    }

    Mat src(dims, isz, CV_8U, srcptr, srcstep);
    Mat dst(dims, isz, CV_8U, dstptr, dststep);

    സ്ഥിരം Mat* arrays[] = { &src, &dst };
    uchar* ptrs[2];
    NAryMatIterator it(arrays, ptrs, 2);
    size_t planesz = it.size;

    വേണ്ടി( size_t j = 0; j < it.nplanes; j++, ++it )
        memcpy(ptrs[1], ptrs[0], planesz);
}

BufferPoolController* MatAllocator::getBufferPoolController(സ്ഥിരം അക്ഷരം* id) സ്ഥിരം
{
    CV_UNUSED(id);
    സ്ഥിതം DummyBufferPoolController dummy;
    തിരികെ &dummy;
}

വർഗ്ഗം StdMatAllocator CV_FINAL : പൊതു MatAllocator
{
പൊതു:
    UMatData* allocate(പൂർണ്ണസംഖ്യ dims, സ്ഥിരം പൂർണ്ണസംഖ്യ* sizes, പൂർണ്ണസംഖ്യ type,
                       ശൂന്യം* data0, size_t* step, AccessFlag /*flags*/, UMatUsageFlags /*usageFlags*/) സ്ഥിരം CV_OVERRIDE
    {
        size_t total = CV_ELEM_SIZE(type);
        വേണ്ടി( പൂർണ്ണസംഖ്യ i = dims-1; i >= 0; i-- )
        {
            എങ്കിൽ( step )
            {
                എങ്കിൽ( data0 && step[i] != CV_AUTOSTEP )
                {
                    CV_Assert(total <= step[i]);
                    total = step[i];
                }
                അല്ലെങ്കിൽ
                    step[i] = total;
            }
            total *= sizes[i];
        }
        uchar* data = data0 ? (uchar*)data0 : (uchar*)fastMalloc(total);
        UMatData* u = പുതിയ UMatData(ഇത്);
        u->data = u->origdata = data;
        u->size = total;
        എങ്കിൽ(data0)
            u->flags |= UMatData::USER_ALLOCATED;

        തിരികെ u;
    }

    തർക്കം allocate(UMatData* u, AccessFlag /*accessFlags*/, UMatUsageFlags /*usageFlags*/) സ്ഥിരം CV_OVERRIDE
    {
        എങ്കിൽ(!u) തിരികെ അസത്യം;
        തിരികെ സത്യം;
    }

    ശൂന്യം deallocate(UMatData* u) സ്ഥിരം CV_OVERRIDE
    {
        എങ്കിൽ(!u)
            തിരികെ;

        CV_Assert(u->urefcount == 0);
        CV_Assert(u->refcount == 0);
        എങ്കിൽ( !(u->flags & UMatData::USER_ALLOCATED) )
        {
            fastFree(u->origdata);
            u->origdata = 0;
        }
        delete u;
    }
};

സ്ഥിതം
MatAllocator*& getDefaultAllocatorMatRef()
{
    സ്ഥിതം MatAllocator* g_matAllocator = Mat::getStdAllocator();
    തിരികെ g_matAllocator;
}

MatAllocator* Mat::getDefaultAllocator()
{
    തിരികെ getDefaultAllocatorMatRef();
}

ശൂന്യം Mat::setDefaultAllocator(MatAllocator* allocator)
{
    getDefaultAllocatorMatRef() = allocator;
}

MatAllocator* Mat::getStdAllocator()
{
    CV_SINGLETON_LAZY_INIT(MatAllocator, പുതിയ StdMatAllocator())
}

//==================================================================================================

ശൂന്യം setSize( Mat& m, പൂർണ്ണസംഖ്യ _dims, സ്ഥിരം പൂർണ്ണസംഖ്യ* _sz, സ്ഥിരം size_t* _steps, തർക്കം autoSteps)
{
    CV_Assert( 0 <= _dims && _dims <= CV_MAX_DIM && _dims <= MatShape::MAX_DIMS);

    m.dims = _dims;
    m.size = MatShape(_dims, _sz);
    m.step[std::max(_dims-1, 0)] = CV_ELEM_SIZE(m.flags);
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = _dims-2; i >= 0; i--) {
        size_t autostep = m.size[i+1]*m.step[i+1];
        എങ്കിൽ (_steps) {
            m.step[i] = _steps[i];
            //CV_Assert(m.step[i] >= autostep);
        } അല്ലെങ്കിൽ എങ്കിൽ (autoSteps) {
            m.step[i] = autostep;
        } അല്ലെങ്കിൽ {
            m.step[i] = 0;
        }
    }

    എങ്കിൽ( _dims <= 2 )
    {
        m.cols = _dims == 0 ? 1 : _sz ? _sz[_dims > 1] : 0;
        m.rows = _dims < 2 ? 1 : _sz ? _sz[0] : 0;
    } അല്ലെങ്കിൽ {
        m.cols = m.rows = -1;
    }
}

പൂർണ്ണസംഖ്യ updateContinuityFlag(പൂർണ്ണസംഖ്യ flags, പൂർണ്ണസംഖ്യ dims, സ്ഥിരം പൂർണ്ണസംഖ്യ* size, സ്ഥിരം size_t* step)
{
    പൂർണ്ണസംഖ്യ i, j;
    എങ്കിൽ (dims <= 1)
        തിരികെ flags | Mat::CONTINUOUS_FLAG;
    വേണ്ടി( i = 0; i < dims; i++ )
    {
        എങ്കിൽ( size[i] > 1 )
            നിർത്തുക;
    }

    uint64 t = (uint64)size[std::min(i, dims-1)]*CV_MAT_CN(flags);
    വേണ്ടി( j = dims-1; j > i; j-- )
    {
        t *= size[j];
        എങ്കിൽ( step[j]*size[j] < step[j-1] )
            നിർത്തുക;
    }

    എങ്കിൽ( j <= i && t == (uint64)(പൂർണ്ണസംഖ്യ)t )
        തിരികെ flags | Mat::CONTINUOUS_FLAG;
    തിരികെ flags & ~Mat::CONTINUOUS_FLAG;
}

ശൂന്യം Mat::updateContinuityFlag()
{
    flags = cv::updateContinuityFlag(flags, dims, size.p, step.p);
}

ശൂന്യം finalizeHdr(Mat& m)
{
    m.updateContinuityFlag();
    പൂർണ്ണസംഖ്യ d = m.dims;
    എങ്കിൽ( d > 2 )
        m.rows = m.cols = -1;
    എങ്കിൽ(m.u)
        m.datastart = m.data = m.u->data;
    എങ്കിൽ( m.data && d > 0 )
    {
        m.datalimit = m.datastart + m.size[0]*m.step[0];
        എങ്കിൽ( m.size[0] > 0 )
        {
            പൂർണ്ണസംഖ്യ lastdim = d > 0 ? d - 1 : 0;
            m.dataend = m.ptr() + m.size[lastdim]*m.step[lastdim];
            വേണ്ടി( പൂർണ്ണസംഖ്യ i = 0; i < d-1; i++ )
                m.dataend += (m.size[i] - 1)*m.step[i];
        }
        അല്ലെങ്കിൽ
            m.dataend = m.datalimit;
    }
    അല്ലെങ്കിൽ
        m.dataend = m.datalimit = m.data;
}

//======================================= Mat ======================================================

Mat::Mat() CV_NOEXCEPT
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{}

Mat::Mat(പൂർണ്ണസംഖ്യ _rows, പൂർണ്ണസംഖ്യ _cols, പൂർണ്ണസംഖ്യ _type)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_rows, _cols, _type);
}

Mat::Mat(പൂർണ്ണസംഖ്യ _rows, പൂർണ്ണസംഖ്യ _cols, പൂർണ്ണസംഖ്യ _type, സ്ഥിരം Scalar& _s)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_rows, _cols, _type);
    *ഇത് = _s;
}

Mat::Mat(Size _sz, പൂർണ്ണസംഖ്യ _type)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create( _sz.height, _sz.width, _type );
}

Mat::Mat(Size _sz, പൂർണ്ണസംഖ്യ _type, സ്ഥിരം Scalar& _s)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_sz.height, _sz.width, _type);
    *ഇത് = _s;
}

Mat::Mat(പൂർണ്ണസംഖ്യ _dims, സ്ഥിരം പൂർണ്ണസംഖ്യ* _sz, പൂർണ്ണസംഖ്യ _type)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_dims, _sz, _type);
}

Mat::Mat(പൂർണ്ണസംഖ്യ _dims, സ്ഥിരം പൂർണ്ണസംഖ്യ* _sz, പൂർണ്ണസംഖ്യ _type, സ്ഥിരം Scalar& _s)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_dims, _sz, _type);
    *ഇത് = _s;
}

Mat::Mat(സ്ഥിരം std::vector<പൂർണ്ണസംഖ്യ>& _sz, പൂർണ്ണസംഖ്യ _type)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_sz, _type);
}

Mat::Mat(സ്ഥിരം std::vector<പൂർണ്ണസംഖ്യ>& _sz, പൂർണ്ണസംഖ്യ _type, സ്ഥിരം Scalar& _s)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_sz, _type);
    *ഇത് = _s;
}

Mat::Mat(സ്ഥിരം MatShape& _shape, പൂർണ്ണസംഖ്യ _type)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_shape, _type);
}

Mat::Mat(std::initializer_list<പൂർണ്ണസംഖ്യ> _shape, പൂർണ്ണസംഖ്യ _type)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_shape, _type);
}

Mat::Mat(സ്ഥിരം MatShape& _shape, പൂർണ്ണസംഖ്യ _type, സ്ഥിരം Scalar& _s)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_shape, _type);
    *ഇത് = _s;
}

Mat::Mat(std::initializer_list<പൂർണ്ണസംഖ്യ> _shape, പൂർണ്ണസംഖ്യ _type, സ്ഥിരം Scalar& _s)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_shape, _type);
    *ഇത് = _s;
}

Mat::Mat(സ്ഥിരം Mat& m)
    : flags(m.flags), dims(m.dims), rows(m.rows), cols(m.cols), data(m.data),
      datastart(m.datastart), dataend(m.dataend), datalimit(m.datalimit), allocator(m.allocator),
      u(m.u), size(m.size), step(m.step)
{
    എങ്കിൽ( u )
        CV_XADD(&u->refcount, 1);
}

Mat::Mat(പൂർണ്ണസംഖ്യ _rows, പൂർണ്ണസംഖ്യ _cols, പൂർണ്ണസംഖ്യ _type, ശൂന്യം* _data, size_t _step)
    : flags(MAGIC_VAL + (_type & TYPE_MASK)), dims(2), rows(_rows), cols(_cols),
      data((uchar*)_data), datastart((uchar*)_data), dataend(0), datalimit(0),
      allocator(0), u(0), size(2)
{
    CV_Assert(total() == 0 || data != ശൂന്യ);

    size_t esz = CV_ELEM_SIZE(_type), esz1 = CV_ELEM_SIZE1(_type);
    size_t minstep = cols * esz;
    എങ്കിൽ( _step == AUTO_STEP )
    {
        _step = minstep;
    }
    അല്ലെങ്കിൽ
    {
        CV_Assert( _step >= minstep );
        എങ്കിൽ (_step % esz1 != 0)
        {
            CV_Error(Error::BadStep, "Step must be a multiple of esz1");
        }
    }
    size[0] = rows;
    size[1] = cols;
    step[0] = _step;
    step[1] = esz;
    datalimit = datastart + _step * rows;
    dataend = datalimit - _step + minstep;
    updateContinuityFlag();
}

Mat::Mat(Size _sz, പൂർണ്ണസംഖ്യ _type, ശൂന്യം* _data, size_t _step)
    : flags(MAGIC_VAL + (_type & TYPE_MASK)), dims(2), rows(_sz.height), cols(_sz.width),
      data((uchar*)_data), datastart((uchar*)_data), dataend(0), datalimit(0),
      allocator(0), u(0), size(2)
{
    CV_Assert(total() == 0 || data != ശൂന്യ);

    size_t esz = CV_ELEM_SIZE(_type), esz1 = CV_ELEM_SIZE1(_type);
    size_t minstep = cols*esz;
    എങ്കിൽ( _step == AUTO_STEP || (_step < minstep && rows == 1))
    {
        _step = minstep;
    }
    അല്ലെങ്കിൽ
    {
        CV_CheckGE(_step, minstep, "");

        എങ്കിൽ (_step % esz1 != 0)
        {
            CV_Error(Error::BadStep, "Step must be a multiple of esz1");
        }
    }
    size[0] = rows;
    size[1] = cols;
    step[0] = _step;
    step[1] = esz;
    datalimit = datastart + _step*rows;
    dataend = datalimit - _step + minstep;
    updateContinuityFlag();
}


Mat::~Mat()
{
    release();
}

Mat& Mat::operator = (സ്ഥിരം Mat& m)
{
    എങ്കിൽ( ഇത് != &m )
    {
        എങ്കിൽ( m.u )
            CV_XADD(&m.u->refcount, 1);
        release();
        flags = m.flags;
        dims = m.dims;
        rows = m.rows;
        cols = m.cols;
        size = m.size;
        step = m.step;
        data = m.data;
        datastart = m.datastart;
        dataend = m.dataend;
        datalimit = m.datalimit;
        allocator = m.allocator;
        u = m.u;
    }
    തിരികെ *ഇത്;
}

Mat Mat::clone() സ്ഥിരം
{
    Mat m;
    copyTo(m);
    തിരികെ m;
}

ശൂന്യം Mat::assignTo( Mat& m, പൂർണ്ണസംഖ്യ _type ) സ്ഥിരം
{
    എങ്കിൽ( _type < 0 )
        m = *ഇത്;
    അല്ലെങ്കിൽ
        convertTo(m, _type);
}

ശൂന്യം Mat::create(പൂർണ്ണസംഖ്യ _rows, പൂർണ്ണസംഖ്യ _cols, പൂർണ്ണസംഖ്യ _type)
{
    _type &= TYPE_MASK;
    എങ്കിൽ( dims <= 2 && rows == _rows && cols == _cols && type() == _type && data )
        തിരികെ;
    പൂർണ്ണസംഖ്യ sz[] = {_rows, _cols};
    create(2, sz, _type);
}

ശൂന്യം Mat::create(Size _sz, പൂർണ്ണസംഖ്യ _type)
{
    create(_sz.height, _sz.width, _type);
}

ശൂന്യം Mat::createSameSize(നിവേശം_Array m, പൂർണ്ണസംഖ്യ type)
{
    MatShape msize = m.shape();
    create(msize, type);
}

ശൂന്യം Mat::fit(പൂർണ്ണസംഖ്യ _dims, സ്ഥിരം പൂർണ്ണസംഖ്യ* _sizes, പൂർണ്ണസംഖ്യ _type)
{
    size_t oldTotalBytes = u ? u->size : 0;
    size_t esz = CV_ELEM_SIZE(_type), newTotal = _dims >= 0;
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < _dims; i++)
        newTotal *= _sizes[i];
    size_t newTotalBytes = newTotal*esz;
    എങ്കിൽ (newTotalBytes > 0 && (!isContinuous() ||
                              newTotalBytes > oldTotalBytes ||
                              data != datastart)) {
        create(_dims, _sizes, _type);
    } അല്ലെങ്കിൽ {
        flags = (flags & ~Mat::TYPE_MASK) | CV_MAT_TYPE(_type);
        പൂർണ്ണസംഖ്യ _dummy_size = 0;
        setSize(*ഇത്, (_dims >= 0 ? _dims : 1), (_dims >= 0 ? _sizes : &_dummy_size), nullptr, സത്യം);
        finalizeHdr(*ഇത്);
    }
}

ശൂന്യം Mat::fit(സ്ഥിരം std::vector<പൂർണ്ണസംഖ്യ>& _shape, പൂർണ്ണസംഖ്യ _type)
{
    fit((പൂർണ്ണസംഖ്യ)_shape.size(), _shape.data(), _type);
}

ശൂന്യം Mat::fit(സ്ഥിരം MatShape& _shape, പൂർണ്ണസംഖ്യ _type)
{
    fit(_shape.dims, _shape.p, _type);
    size.layout = _shape.layout;
    size.C = _shape.C;
}

ശൂന്യം Mat::fit(std::initializer_list<പൂർണ്ണസംഖ്യ> _shape, പൂർണ്ണസംഖ്യ _type)
{
    പൂർണ്ണസംഖ്യ new_shape[MatShape::MAX_DIMS];
    പൂർണ്ണസംഖ്യ new_ndims = (പൂർണ്ണസംഖ്യ)_shape.size();
    CV_Assert(new_ndims <= MatShape::MAX_DIMS);
    auto it = _shape.begin();
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < new_ndims; i++, ++it)
        new_shape[i] = *it;
    fit(new_ndims, new_shape, _type);
}

ശൂന്യം Mat::fit(പൂർണ്ണസംഖ്യ _rows, പൂർണ്ണസംഖ്യ _cols, പൂർണ്ണസംഖ്യ _type)
{
    _type &= TYPE_MASK;
    പൂർണ്ണസംഖ്യ sz[] = {_rows, _cols};
    fit(2, sz, _type);
}

ശൂന്യം Mat::fit(Size _sz, പൂർണ്ണസംഖ്യ _type)
{
    fit(_sz.height, _sz.width, _type);
}

ശൂന്യം Mat::fitSameSize(നിവേശം_Array m, പൂർണ്ണസംഖ്യ _type)
{
    fit(m.shape(), _type);
}

ശൂന്യം Mat::addref()
{
    എങ്കിൽ( u )
        CV_XADD(&u->refcount, 1);
}

ശൂന്യം Mat::release()
{
    എങ്കിൽ( u && CV_XADD(&u->refcount, -1) == 1 )
        deallocate();
    u = ശൂന്യ;
    datastart = dataend = datalimit = data = 0;
    വേണ്ടി(പൂർണ്ണസംഖ്യ i = 0; i < dims; i++)
        size.p[i] = 0;
    flags = (flags & CV_MAT_TYPE_MASK) | MAGIC_VAL;
    dims = rows = cols = 0;
    size.clear();
}

size_t Mat::step1(പൂർണ്ണസംഖ്യ i) സ്ഥിരം
{
    തിരികെ step.p[i] / elemSize1();
}

തർക്കം Mat::empty() സ്ഥിരം
{
    തിരികെ data == 0 || total() == 0;
}

size_t Mat::total() സ്ഥിരം
{
    എങ്കിൽ( dims <= 2 )
        തിരികെ (size_t)rows * cols;
    size_t p = 1;
    വേണ്ടി( പൂർണ്ണസംഖ്യ i = 0; i < dims; i++ )
        p *= size[i];
    തിരികെ p;
}

size_t Mat::total(പൂർണ്ണസംഖ്യ startDim, പൂർണ്ണസംഖ്യ endDim) സ്ഥിരം
{
    CV_Assert( 0 <= startDim && startDim <= endDim);
    size_t p = 1;
    പൂർണ്ണസംഖ്യ endDim_ = endDim <= dims ? endDim : dims;
    വേണ്ടി( പൂർണ്ണസംഖ്യ i = startDim; i < endDim_; i++ )
        p *= size[i];
    തിരികെ p;
}

Mat::Mat(Mat&& m) CV_NOEXCEPT
    : flags(m.flags), dims(m.dims), rows(m.rows), cols(m.cols), data(m.data),
      datastart(m.datastart), dataend(m.dataend), datalimit(m.datalimit), allocator(m.allocator),
      u(m.u), size(m.size), step(m.step)
{
    m.flags = MAGIC_VAL; m.dims = m.rows = m.cols = 0;
    m.size.clear(); m.step.clear();
    m.data = ശൂന്യ; m.datastart = ശൂന്യ; m.dataend = ശൂന്യ; m.datalimit = ശൂന്യ;
    m.allocator = ശൂന്യ;
    m.u = ശൂന്യ;
}


Mat& Mat::operator = (Mat&& m)
{
    എങ്കിൽ (ഇത് == &m)
      തിരികെ *ഇത്;

    release();
    flags = m.flags; dims = m.dims; rows = m.rows; cols = m.cols; data = m.data;
    size = m.size; step = m.step;
    datastart = m.datastart; dataend = m.dataend; datalimit = m.datalimit; allocator = m.allocator;
    u = m.u;
    m.flags = MAGIC_VAL; m.dims = m.rows = m.cols = 0;
    m.size.clear(); m.step.clear();
    m.data = ശൂന്യ; m.datastart = ശൂന്യ; m.dataend = ശൂന്യ; m.datalimit = ശൂന്യ;
    m.allocator = ശൂന്യ;
    m.u = ശൂന്യ;
    തിരികെ *ഇത്;
}


ശൂന്യം Mat::create(പൂർണ്ണസംഖ്യ d0, സ്ഥിരം പൂർണ്ണസംഖ്യ* _sizes, പൂർണ്ണസംഖ്യ _type)
{
    പൂർണ്ണസംഖ്യ sz1 = 1, d = d0;
    പൂർണ്ണസംഖ്യ i;
    എങ്കിൽ (d == 0) {
        d = 1;
        _sizes = (സ്ഥിരം പൂർണ്ണസംഖ്യ*)&sz1;
    }
    CV_Assert(0 <= d && d <= CV_MAX_DIM && _sizes);
    _type = CV_MAT_TYPE(_type);

    എങ്കിൽ( data && d == dims && _type == type() )
    {
        വേണ്ടി( i = 0; i < d; i++ )
            എങ്കിൽ( size[i] != _sizes[i] )
                നിർത്തുക;
        എങ്കിൽ( i == d )
            തിരികെ;
    }

    പൂർണ്ണസംഖ്യ _sizes_backup[CV_MAX_DIM]; // #5991
    എങ്കിൽ (_sizes == (ഇത്->size.p))
    {
        വേണ്ടി(i = 0; i < d; i++ )
            _sizes_backup[i] = _sizes[i];
        _sizes = _sizes_backup;
    }

    release();
    എങ്കിൽ( d == 0 )
        തിരികെ;
    flags = (_type & CV_MAT_TYPE_MASK) | MAGIC_VAL;
    setSize(*ഇത്, d, _sizes, 0, സത്യം);

    എങ്കിൽ( total() > 0 )
    {
        MatAllocator *a = allocator, *a0 = getDefaultAllocator();
        എങ്കിൽ(!a)
            a = a0;
        ശ്രമിക്കുക
        {
            u = a->allocate(dims, size.p, _type, 0, step.p, ACCESS_RW /* ignored */, USAGE_DEFAULT);
            CV_Assert(u != 0);
            allocator = a;
        }
        പിടിക്കുക (...)
        {
            എങ്കിൽ (a == a0)
                എറിയുക;
            u = a0->allocate(dims, size.p, _type, 0, step.p, ACCESS_RW /* ignored */, USAGE_DEFAULT);
            CV_Assert(u != 0);
            allocator = a0;
        }
        CV_Assert( step[dims-1] == (size_t)CV_ELEM_SIZE(flags) );
    }

    addref();
    finalizeHdr(*ഇത്);
    size.dims = dims = d0;
}

ശൂന്യം Mat::create(സ്ഥിരം std::vector<പൂർണ്ണസംഖ്യ>& _sizes, പൂർണ്ണസംഖ്യ _type)
{
    create((പൂർണ്ണസംഖ്യ)_sizes.size(), _sizes.data(), _type);
}

ശൂന്യം Mat::create(സ്ഥിരം MatShape& _shape, പൂർണ്ണസംഖ്യ _type)
{
    എങ്കിൽ (_shape.dims < 0) {
        release();
        തിരികെ;
    }
    create(_shape.dims, _shape.p, _type);
    size.layout = _shape.layout;
    size.C = _shape.C;
}

ശൂന്യം Mat::create(std::initializer_list<പൂർണ്ണസംഖ്യ> _shape, പൂർണ്ണസംഖ്യ _type)
{
    പൂർണ്ണസംഖ്യ new_shape[MatShape::MAX_DIMS];
    പൂർണ്ണസംഖ്യ new_ndims = (പൂർണ്ണസംഖ്യ)_shape.size();
    CV_Assert(new_ndims <= MatShape::MAX_DIMS);
    auto it = _shape.begin();
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < new_ndims; i++, ++it)
        new_shape[i] = *it;

    create(new_ndims, new_shape, _type);
}

ശൂന്യം Mat::copySize(സ്ഥിരം Mat& m)
{
    dims = m.dims;
    cols = m.cols;
    rows = m.rows;
    size = m.size;
    step = m.step;
}

ശൂന്യം Mat::deallocate()
{
    എങ്കിൽ(u)
    {
        UMatData* u_ = u;
        u = ശൂന്യ;
        (u_->currAllocator ? u_->currAllocator : allocator ? allocator : getDefaultAllocator())->unmap(u_);
    }
}

Mat::Mat(സ്ഥിരം Mat& m, സ്ഥിരം Range& _rowRange, സ്ഥിരം Range& _colRange)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    CV_Assert(m.dims <= MatShape::MAX_DIMS);
    CV_Assert(m.dims >= 2 ||
              (m.dims == 1 && (_rowRange == Range::all() ||
                               _rowRange == Range(0, m.rows))));
    എങ്കിൽ( m.dims > 2 )
    {
        Range rs[MatShape::MAX_DIMS];
        rs[0] = _rowRange;
        rs[1] = _colRange;
        വേണ്ടി( പൂർണ്ണസംഖ്യ i = 2; i < m.dims; i++ )
            rs[i] = Range::all();
        *ഇത് = m(rs);
        തിരികെ;
    }

    *ഇത് = m;
    ശ്രമിക്കുക
    {
        എങ്കിൽ( _rowRange != Range::all() && _rowRange != Range(0,rows) )
        {
            CV_Assert( 0 <= _rowRange.start && _rowRange.start <= _rowRange.end
                       && _rowRange.end <= m.rows );
            size[0] = rows = _rowRange.size();
            data += step*_rowRange.start;
            flags |= SUBMATRIX_FLAG;
        }

        എങ്കിൽ( _colRange != Range::all() && _colRange != Range(0,cols) )
        {
            CV_Assert( 0 <= _colRange.start && _colRange.start <= _colRange.end
                       && _colRange.end <= m.cols );
            size[dims > 1] = cols = _colRange.size();
            data += _colRange.start*elemSize();
            flags |= SUBMATRIX_FLAG;
        }
    }
    പിടിക്കുക(...)
    {
        release();
        എറിയുക;
    }

    updateContinuityFlag();

    എങ്കിൽ( rows <= 0 || cols <= 0 )
    {
        release();
        rows = cols = 0;
    }
}


Mat::Mat(സ്ഥിരം Mat& m, സ്ഥിരം Rect& roi)
    : flags(m.flags), dims(2), rows(roi.height), cols(roi.width),
    data(m.data + roi.y*m.step[0]),
    datastart(m.datastart), dataend(m.dataend), datalimit(m.datalimit),
    allocator(m.allocator), u(m.u), size(2)
{
    CV_Assert( m.dims <= 2 );

    size_t esz = CV_ELEM_SIZE(flags);
    data += roi.x*esz;
    CV_Assert( 0 <= roi.x && 0 <= roi.width && roi.x + roi.width <= m.cols &&
              0 <= roi.y && 0 <= roi.height && roi.y + roi.height <= m.rows );
    എങ്കിൽ( roi.width < m.cols || roi.height < m.rows )
        flags |= SUBMATRIX_FLAG;

    size[0] = rows;
    size[1] = cols;
    step[0] = m.step[0];
    step[1] = esz;
    updateContinuityFlag();

    addref();
    എങ്കിൽ( rows <= 0 || cols <= 0 )
    {
        rows = cols = 0;
        release();
    }
}


Mat::Mat(പൂർണ്ണസംഖ്യ _dims, സ്ഥിരം പൂർണ്ണസംഖ്യ* _sizes, പൂർണ്ണസംഖ്യ _type, ശൂന്യം* _data, സ്ഥിരം size_t* _steps)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    flags |= CV_MAT_TYPE(_type);
    datastart = data = (uchar*)_data;
    എങ്കിൽ (_dims > 0 || _data != nullptr)
        setSize(*ഇത്, _dims, _sizes, _steps, സത്യം);
    finalizeHdr(*ഇത്);
}


Mat::Mat(സ്ഥിരം std::vector<പൂർണ്ണസംഖ്യ>& _sizes, പൂർണ്ണസംഖ്യ _type, ശൂന്യം* _data, സ്ഥിരം size_t* _steps)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    flags |= CV_MAT_TYPE(_type);
    datastart = data = (uchar*)_data;
    പൂർണ്ണസംഖ്യ _dims = (പൂർണ്ണസംഖ്യ)_sizes.size();
    എങ്കിൽ (_dims > 0 || _data != nullptr)
        setSize(*ഇത്, _dims, _sizes.data(), _steps, സത്യം);
    finalizeHdr(*ഇത്);
}

Mat::Mat(സ്ഥിരം MatShape& _shape, പൂർണ്ണസംഖ്യ _type, ശൂന്യം* _data, സ്ഥിരം size_t* _steps)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    flags |= CV_MAT_TYPE(_type);
    datastart = data = (uchar*)_data;
    എങ്കിൽ (_shape.dims >= 0) {
        setSize(*ഇത്, _shape.dims, _shape.p, _steps, സത്യം);
    }
    അല്ലെങ്കിൽ {
        CV_Assert(!data);
    }
    finalizeHdr(*ഇത്);
}

Mat::Mat(std::initializer_list<പൂർണ്ണസംഖ്യ> _shape, പൂർണ്ണസംഖ്യ _type, ശൂന്യം* _data, സ്ഥിരം size_t* _steps)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    പൂർണ്ണസംഖ്യ new_shape[MatShape::MAX_DIMS];
    പൂർണ്ണസംഖ്യ _dims = (പൂർണ്ണസംഖ്യ)_shape.size();
    CV_Assert(_dims <= MatShape::MAX_DIMS);
    auto it = _shape.begin();
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < _dims; i++, ++it)
        new_shape[i] = *it;

    flags |= CV_MAT_TYPE(_type);
    datastart = data = (uchar*)_data;
    എങ്കിൽ (_dims > 0 || _data != nullptr)
        setSize(*ഇത്, _dims, new_shape, _steps, സത്യം);
    finalizeHdr(*ഇത്);
}

Mat::Mat(സ്ഥിരം Mat& m, സ്ഥിരം Range* ranges)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    പൂർണ്ണസംഖ്യ d = m.dims;

    CV_Assert(ranges);
    വേണ്ടി( പൂർണ്ണസംഖ്യ i = 0; i < d; i++ )
    {
        Range r = ranges[i];
        CV_Assert( r == Range::all() || (0 <= r.start && r.start < r.end && r.end <= m.size[i]) );
    }
    *ഇത് = m;
    വേണ്ടി( പൂർണ്ണസംഖ്യ i = 0; i < d; i++ )
    {
        Range r = ranges[i];
        എങ്കിൽ( r != Range::all() && r != Range(0, size.p[i]))
        {
            size.p[i] = r.end - r.start;
            data += r.start*step.p[i];
            flags |= SUBMATRIX_FLAG;
        }
    }

    എങ്കിൽ (d <= 2) {
        rows = d == 2 ? size[0] : 1;
        cols = d <= 0 ? (d >= 0) : size[d > 1];
    }
    updateContinuityFlag();
}

Mat::Mat(സ്ഥിരം Mat& m, സ്ഥിരം std::vector<Range>& ranges)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
    datalimit(0), allocator(0), u(0)
{
    പൂർണ്ണസംഖ്യ d = m.dims;

    CV_Assert((പൂർണ്ണസംഖ്യ)ranges.size() == d);
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < d; i++)
    {
        Range r = ranges[i];
        CV_Assert(r == Range::all() || (0 <= r.start && r.start < r.end && r.end <= m.size[i]));
    }
    *ഇത് = m;
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < d; i++)
    {
        Range r = ranges[i];
        എങ്കിൽ (r != Range::all() && r != Range(0, size.p[i]))
        {
            size.p[i] = r.end - r.start;
            data += r.start*step.p[i];
            flags |= SUBMATRIX_FLAG;
        }
    }

    എങ്കിൽ (d <= 2) {
        rows = d == 2 ? size[0] : 1;
        cols = d <= 0 ? (d >= 0) : size[d > 1];
    }
    updateContinuityFlag();
}


Mat Mat::diag(പൂർണ്ണസംഖ്യ d) സ്ഥിരം
{
    CV_Assert( dims == 2 );
    Mat m = *ഇത്;
    size_t esz = elemSize();
    പൂർണ്ണസംഖ്യ len;

    എങ്കിൽ( d >= 0 )
    {
        len = std::min(cols - d, rows);
        m.data += esz*d;
    }
    അല്ലെങ്കിൽ
    {
        len = std::min(rows + d, cols);
        m.data -= step[0]*d;
    }
    CV_DbgAssert( len > 0 );

    m.size[0] = m.rows = len;
    m.size[1] = m.cols = 1;
    m.step[0] += (len > 1 ? esz : 0);

    m.updateContinuityFlag();

    എങ്കിൽ( size() != Size(1,1) )
        m.flags |= SUBMATRIX_FLAG;

    തിരികെ m;
}


ശൂന്യം Mat::pop_back(size_t nelems)
{
    CV_Assert( nelems <= (size_t)size.p[0] );
    CV_Assert( dims >= 1 );

    എങ്കിൽ( isSubmatrix() )
        *ഇത് = rowRange(0, size.p[0] - (പൂർണ്ണസംഖ്യ)nelems);
    അല്ലെങ്കിൽ
    {
        size.p[0] -= (പൂർണ്ണസംഖ്യ)nelems;
        എങ്കിൽ (dims == 2)
            rows = size.p[0];
        അല്ലെങ്കിൽ എങ്കിൽ (dims == 1)
            cols = size.p[0];
        dataend -= nelems*step.p[0];
    }
}


ശൂന്യം Mat::തള്ളുക_back(സ്ഥിരം ശൂന്യം* elem)
{
    size_t r = size.p[0];
    എങ്കിൽ( isSubmatrix() || dataend + step.p[0] > datalimit )
        reserve( std::max(r + 1, (r*3+1)/2) );

    size_t esz = elemSize();
    memcpy(data + r*step.p[0], elem, esz);
    size.p[0] = പൂർണ്ണസംഖ്യ(r + 1);
    എങ്കിൽ (dims == 2)
        rows = size.p[0];
    അല്ലെങ്കിൽ എങ്കിൽ (dims == 1)
        cols = size.p[0];
    dataend += step.p[0];
    uint64 tsz = size.p[0];
    വേണ്ടി( പൂർണ്ണസംഖ്യ i = 1; i < dims; i++ )
        tsz *= size.p[i];
    എങ്കിൽ( esz < step.p[0] || tsz != (uint64)(പൂർണ്ണസംഖ്യ)tsz )
        flags &= ~CONTINUOUS_FLAG;
}


ശൂന്യം Mat::reserve(size_t nelems)
{
    സ്ഥിരം size_t MIN_SIZE = 64;

    CV_Assert( (പൂർണ്ണസംഖ്യ)nelems >= 0 );
    എങ്കിൽ( !isSubmatrix() && step.p[0] != 0 && data + step.p[0]*nelems <= datalimit )
        തിരികെ;

    പൂർണ്ണസംഖ്യ r = size.p[0];
    എങ്കിൽ( (size_t)r >= nelems)
        തിരികെ;

    MatShape newsize = size;
    size_t esz = elemSize();
    newsize.dims = std::max(newsize.dims, 1);
    എങ്കിൽ (dims > 1 && r == 0 && step.p[0] == 0) {
        step.p[0] = size.p[1] * (dims == 2 ? esz : step.p[1]);
    }
    newsize.p[0] = (പൂർണ്ണസംഖ്യ)nelems;
    size_t newbytes = newsize.total() * esz;
    എങ്കിൽ (newbytes < MIN_SIZE) {
        newsize.p[0] = 1;
        newsize.p[0] = പൂർണ്ണസംഖ്യ(MIN_SIZE / newsize.total() / esz);
    }

    Mat m(newsize, type());
    എങ്കിൽ( r > 0)
    {
        Mat mpart = m.rowRange(0, r);
        copyTo(mpart);
    }

    *ഇത് = m;
    size.p[0] = r;

    എങ്കിൽ (dims == 2)
        rows = size.p[0];
    അല്ലെങ്കിൽ എങ്കിൽ (dims == 1)
        cols = size.p[0];
    dataend = data + step.p[0]*r;
}


ശൂന്യം Mat::reserveBuffer(size_t nbytes)
{
    size_t esz = 1;
    പൂർണ്ണസംഖ്യ mtype = CV_8UC1;
    എങ്കിൽ (!empty())
    {
        എങ്കിൽ (!isSubmatrix() && data + nbytes <= dataend)//Should it be datalimit?
            തിരികെ;
        esz = elemSize();
        mtype = type();
    }

    size_t nelems = (nbytes - 1) / esz + 1;

#എങ്കിൽ SIZE_MAX > UINT_MAX
    CV_Assert(nelems <= size_t(INT_MAX)*size_t(INT_MAX));
    പൂർണ്ണസംഖ്യ newrows = nelems > size_t(INT_MAX) ? nelems > 0x400*size_t(INT_MAX) ? nelems > 0x100000 * size_t(INT_MAX) ? nelems > 0x40000000 * size_t(INT_MAX) ?
                  size_t(INT_MAX) : 0x40000000 : 0x100000 : 0x400 : 1;
#അല്ലെങ്കിൽ
    പൂർണ്ണസംഖ്യ newrows = nelems > size_t(INT_MAX) ? 2 : 1;
#endif
    പൂർണ്ണസംഖ്യ newcols = (പൂർണ്ണസംഖ്യ)((nelems - 1) / newrows + 1);

    create(newrows, newcols, mtype);
}


ശൂന്യം Mat::resize(size_t nelems)
{
    പൂർണ്ണസംഖ്യ r = size.p[0];
    എങ്കിൽ( r == (പൂർണ്ണസംഖ്യ)nelems )
        തിരികെ;
    CV_Assert( (പൂർണ്ണസംഖ്യ)nelems >= 0 );

    എങ്കിൽ( isSubmatrix() || data + step.p[0]*nelems > datalimit )
        reserve(nelems);

    size.p[0] = (പൂർണ്ണസംഖ്യ)nelems;
    എങ്കിൽ (dims == 2)
        rows = size.p[0];
    അല്ലെങ്കിൽ എങ്കിൽ (dims == 1)
        cols = size.p[0];
    dataend += (size.p[0] - r)*(int64_t)step.p[0];

    //updateContinuityFlag(*this);
}


ശൂന്യം Mat::resize(size_t nelems, സ്ഥിരം Scalar& s)
{
    പൂർണ്ണസംഖ്യ saveRows = size.p[0];
    resize(nelems);

    എങ്കിൽ( size.p[0] > saveRows )
    {
        Mat part = rowRange(saveRows, size.p[0]);
        part = s;
    }
}

ശൂന്യം Mat::push_back(സ്ഥിരം Mat& elems)
{
    size_t r = size.p[0];
    size_t delta = elems.size.p[0];
    എങ്കിൽ( delta == 0 )
        തിരികെ;
    എങ്കിൽ( ഇത് == &elems )
    {
        Mat tmp = elems;
        push_back(tmp);
        തിരികെ;
    }
    എങ്കിൽ( !data )
    {
        *ഇത് = elems.clone();
        തിരികെ;
    }
    CV_Assert(dims > 0);

    size.p[0] = elems.size.p[0];
    തർക്കം eq = size == elems.size;
    size.p[0] = പൂർണ്ണസംഖ്യ(r);
    എങ്കിൽ( !eq )
        CV_Error(cv::Error::StsUnmatchedSizes, "Pushed vector length is not equal to matrix row length");
    എങ്കിൽ( type() != elems.type() )
        CV_Error(cv::Error::StsUnmatchedFormats, "Pushed vector type is not the same as matrix type");
    size_t esz = elemSize();
    size_t minstep = dims <= 1 ? esz : size.p[1] * (dims == 2 ? esz : step.p[1]);
    size_t step0 = step.p[0];
    എങ്കിൽ (step0 < minstep) {
        എങ്കിൽ (size.p[0] <= 1)
            step.p[0] = step0 = minstep;
    }

    എങ്കിൽ( isSubmatrix() || dataend + step0*delta > datalimit )
        reserve( std::max(r + delta, (r*3+1)/2) );

    size.p[0] += പൂർണ്ണസംഖ്യ(delta);
    എങ്കിൽ (dims == 2)
        rows = size.p[0];
    അല്ലെങ്കിൽ എങ്കിൽ (dims == 1)
        cols = size.p[0];
    dataend += step0*delta;

    //updateContinuityFlag(*this);

    എങ്കിൽ( isContinuous() && elems.isContinuous() )
        memcpy(data + r*step0, elems.data, elems.total()*elems.elemSize());
    അല്ലെങ്കിൽ
    {
        Mat part = rowRange(പൂർണ്ണസംഖ്യ(r), പൂർണ്ണസംഖ്യ(r + delta));
        elems.copyTo(part);
    }
}


ശൂന്യം Mat::locateROI( Size& wholeSize, Point& ofs ) സ്ഥിരം
{
    CV_Assert( dims <= 2 && step[0] > 0 );
    size_t esz = elemSize(), minstep;
    ptrdiff_t delta1 = data - datastart, delta2 = dataend - datastart;

    എങ്കിൽ( delta1 == 0 )
        ofs.x = ofs.y = 0;
    അല്ലെങ്കിൽ
    {
        ofs.y = (പൂർണ്ണസംഖ്യ)(delta1/step[0]);
        ofs.x = (പൂർണ്ണസംഖ്യ)((delta1 - step[0]*ofs.y)/esz);
        CV_DbgAssert( data == datastart + ofs.y*step[0] + ofs.x*esz );
    }
    minstep = (ofs.x + cols)*esz;
    wholeSize.height = (പൂർണ്ണസംഖ്യ)((delta2 - minstep)/step[0] + 1);
    wholeSize.height = std::max(wholeSize.height, ofs.y + rows);
    wholeSize.width = (പൂർണ്ണസംഖ്യ)((delta2 - step*(wholeSize.height-1))/esz);
    wholeSize.width = std::max(wholeSize.width, ofs.x + cols);
}

Mat& Mat::adjustROI( പൂർണ്ണസംഖ്യ dtop, പൂർണ്ണസംഖ്യ dbottom, പൂർണ്ണസംഖ്യ dleft, പൂർണ്ണസംഖ്യ dright )
{
    CV_Assert( dims <= 2 && step[0] > 0 );
    Size wholeSize; Point ofs;
    size_t esz = elemSize();
    locateROI( wholeSize, ofs );
    പൂർണ്ണസംഖ്യ row1 = std::min(std::max(ofs.y - dtop, 0), wholeSize.height), row2 = std::max(0, std::min(ofs.y + rows + dbottom, wholeSize.height));
    പൂർണ്ണസംഖ്യ col1 = std::min(std::max(ofs.x - dleft, 0), wholeSize.width), col2 = std::max(0, std::min(ofs.x + cols + dright, wholeSize.width));
    എങ്കിൽ(row1 > row2)
        std::swap(row1, row2);
    എങ്കിൽ(col1 > col2)
        std::swap(col1, col2);

    എങ്കിൽ (dims == 1) {
        data += (col1 - ofs.x)*(std::ptrdiff_t)esz;
        cols = col2 - col1;
        size.p[0] = cols;
    } അല്ലെങ്കിൽ {
        data += (row1 - ofs.y)*(std::ptrdiff_t)step + (col1 - ofs.x)*(std::ptrdiff_t)esz;
        rows = row2 - row1; cols = col2 - col1;
        size.p[0] = rows; size.p[1] = cols;
        updateContinuityFlag();
    }
    തിരികെ *ഇത്;
}

Mat Mat::reshape(പൂർണ്ണസംഖ്യ new_cn, പൂർണ്ണസംഖ്യ new_rows) സ്ഥിരം
{
    പൂർണ്ണസംഖ്യ cn = channels();
    Mat hdr = *ഇത്;

    എങ്കിൽ( new_cn == 0 )
        new_cn = cn;

    എങ്കിൽ( dims > 2 )
    {
        എങ്കിൽ( new_rows == 0 )
        {
            // special case: just change the number of channnels; retain the same shape,
            // except for the last, innermost dimension
            CV_Assert(size[dims-1]*cn % new_cn == 0);
            hdr.flags = (hdr.flags & ~CV_MAT_CN_MASK) | ((new_cn-1) << CV_CN_SHIFT);
            hdr.step[dims-1] = CV_ELEM_SIZE(hdr.flags);
            hdr.size[dims-1] = hdr.size[dims-1]*cn / new_cn;
            തിരികെ hdr;
        }
        CV_Assert( new_rows > 0 );
        പൂർണ്ണസംഖ്യ sz[] = { new_rows, (പൂർണ്ണസംഖ്യ)(total()*cn/new_rows) };
        തിരികെ reshape(new_cn, 2, sz);
    }

    പൂർണ്ണസംഖ്യ total_width = cols * cn;

    എങ്കിൽ( (new_cn > total_width || total_width % new_cn != 0) && new_rows == 0 )
        new_rows = rows * total_width / new_cn;

    എങ്കിൽ( new_rows != 0 && new_rows != rows )
    {
        പൂർണ്ണസംഖ്യ total_size = total_width * rows;
        എങ്കിൽ( !isContinuous() )
            CV_Error( cv::Error::BadStep,
            "The matrix is not continuous, thus its number of rows can not be changed" );

        total_width = total_size / new_rows;

        എങ്കിൽ( total_width * new_rows != total_size )
            CV_Error( cv::Error::StsBadArg, "The total number of matrix elements "
                                    "is not divisible by the new number of rows" );

        hdr.size[0] = hdr.rows = new_rows;
        hdr.step[0] = total_width * elemSize1();
    } അല്ലെങ്കിൽ {
        hdr.size[0] = hdr.rows = rows;
        എങ്കിൽ (dims <= 1)
            hdr.step[0] = cols * CV_ELEM_SIZE(flags);
    }

    പൂർണ്ണസംഖ്യ new_width = total_width / new_cn;

    എങ്കിൽ( new_width * new_cn != total_width )
        CV_Error( cv::Error::Bad_അക്കം_Channels,
        "The total width is not divisible by the new number of channels" );

    hdr.size.dims = hdr.dims = 2;
    hdr.size[1] = hdr.cols = new_width;
    hdr.flags = (hdr.flags & ~CV_MAT_CN_MASK) | ((new_cn-1) << CV_CN_SHIFT);
    hdr.step[1] = CV_ELEM_SIZE(hdr.flags);
    തിരികെ hdr;
}

Mat Mat::reshape(പൂർണ്ണസംഖ്യ _cn, പൂർണ്ണസംഖ്യ _newndims, സ്ഥിരം പൂർണ്ണസംഖ്യ* _newsz) സ്ഥിരം
{
    എങ്കിൽ(_newndims == dims)
    {
        എങ്കിൽ(_newsz == 0)
            തിരികെ reshape(_cn);
        എങ്കിൽ(_newndims == 2)
            തിരികെ reshape(_cn, _newsz[0]);
    }

    എങ്കിൽ (isContinuous())
    {
        CV_Assert(_cn >= 0 && _newndims >= 0 && _newndims <= CV_MAX_DIM && (_newndims == 0 || _newsz != 0));

        എങ്കിൽ (_cn == 0)
            _cn = ഇത്->channels();
        അല്ലെങ്കിൽ
            CV_Assert(_cn <= CV_CN_MAX);

        size_t total_elem1_ref = ഇത്->total() * ഇത്->channels();
        size_t total_elem1 = _cn;

        AutoBuffer<പൂർണ്ണസംഖ്യ, 4> newsz_buf( (size_t)_newndims );

        പൂർണ്ണസംഖ്യ m1_idx = -1;

        വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < _newndims; i++)
        {
            എങ്കിൽ (_newsz[i] >= 0) {
                എങ്കിൽ (_newsz[i] == 0 && i < dims)
                    newsz_buf[i] = size.p[i];
                അല്ലെങ്കിൽ
                    newsz_buf[i] = _newsz[i];
                total_elem1 *= (size_t)newsz_buf[i];
            } അല്ലെങ്കിൽ {
                എങ്കിൽ (m1_idx >= 0)
                    CV_Error(cv::Error::StsBadSize, "More than one '-1' occured in the new shape");
                m1_idx = i;
            }
        }

        എങ്കിൽ (m1_idx >= 0) {
            എങ്കിൽ (total_elem1 == 0) {
                CV_Assert(total_elem1_ref == 0);
                total_elem1 = 1;
            }
            CV_Assert(total_elem1_ref % total_elem1 == 0);
            newsz_buf[m1_idx] = (പൂർണ്ണസംഖ്യ)(total_elem1_ref / total_elem1);
        } അല്ലെങ്കിൽ എങ്കിൽ (total_elem1 != total_elem1_ref) {
            CV_Error(cv::Error::StsUnmatchedSizes, "Requested and source matrices have different count of elements");
        }

        Mat hdr = *ഇത്;
        hdr.flags = (hdr.flags & ~CV_MAT_CN_MASK) | ((_cn-1) << CV_CN_SHIFT);
        setSize(hdr, _newndims, newsz_buf.data(), ശൂന്യ, സത്യം);

        തിരികെ hdr;
    }

    CV_Error(cv::Error::StsNotImplemented, "Reshaping of n-dimensional non-continuous matrices is not supported yet");
    // TBD
}

Mat Mat::reshape(പൂർണ്ണസംഖ്യ _cn, സ്ഥിരം std::vector<പൂർണ്ണസംഖ്യ>& _newshape) സ്ഥിരം
{
    പൂർണ്ണസംഖ്യ newdims = (പൂർണ്ണസംഖ്യ)_newshape.size();
    എങ്കിൽ(newdims == 0 && empty())
    {
        തിരികെ *ഇത്;
    }

    തിരികെ reshape(_cn, newdims, newdims > 0 ? &_newshape[0] : 0);
}

Mat Mat::reshape(പൂർണ്ണസംഖ്യ _cn, സ്ഥിരം MatShape& _newshape) സ്ഥിരം
{
    എങ്കിൽ (_newshape.dims < 0) {
        പൂർണ്ണസംഖ്യ newshape[] = {0};
        തിരികെ reshape(_cn, 1, newshape);
    }
    തിരികെ reshape(_cn, _newshape.dims, _newshape.p);
}

Mat Mat::reshape(പൂർണ്ണസംഖ്യ _cn, std::initializer_list<പൂർണ്ണസംഖ്യ> newshape_) സ്ഥിരം
{
    പൂർണ്ണസംഖ്യ newshape[MatShape::MAX_DIMS];
    size_t i, newshape_dims = newshape_.size();
    CV_Assert(newshape_dims <= (size_t)MatShape::MAX_DIMS);
    auto it = newshape_.begin();
    വേണ്ടി (i = 0; i < newshape_dims; i++, ++it)
        newshape[i] = *it;
    തിരികെ reshape(_cn, (പൂർണ്ണസംഖ്യ)newshape_dims, newshape);
}

Mat Mat::reinterpret(പൂർണ്ണസംഖ്യ type) സ്ഥിരം
{
    type = CV_MAT_TYPE(type);
    CV_Assert(CV_ELEM_SIZE(ഇത്->type()) == CV_ELEM_SIZE(type));
    Mat m = *ഇത്;
    m.flags = (m.flags & ~CV_MAT_TYPE_MASK) | type;
    m.updateContinuityFlag();
    തിരികെ m;
}

Mat Mat::diag(സ്ഥിരം Mat& d)
{
    CV_Assert( d.cols == 1 || d.rows == 1 );
    പൂർണ്ണസംഖ്യ len = d.rows + d.cols - 1;
    Mat m(len, len, d.type(), Scalar(0));
    Mat md = m.diag();
    എങ്കിൽ( d.cols == 1 )
        d.copyTo(md);
    അല്ലെങ്കിൽ
        transpose(d, md);
    തിരികെ m;
}

പൂർണ്ണസംഖ്യ Mat::checkVector(പൂർണ്ണസംഖ്യ _elemChannels, പൂർണ്ണസംഖ്യ _depth, തർക്കം _requireContinuous) സ്ഥിരം
{
    തിരികെ data && (depth() == _depth || _depth <= 0) &&
        (isContinuous() || !_requireContinuous) &&
        ((dims <= 2 && (((rows == 1 || cols == 1) && channels() == _elemChannels) ||
                        (cols == _elemChannels && channels() == 1))) ||
        (dims == 3 && channels() == 1 && size.p[2] == _elemChannels && (size.p[0] == 1 || size.p[1] == 1) &&
         (isContinuous() || step.p[1] == step.p[2]*size.p[2])))
    ? (പൂർണ്ണസംഖ്യ)(total()*channels()/_elemChannels) : -1;
}


സ്ഥിതം വരിയിൽ Size getContinuousSize_(പൂർണ്ണസംഖ്യ flags, പൂർണ്ണസംഖ്യ cols, പൂർണ്ണസംഖ്യ rows, പൂർണ്ണസംഖ്യ widthScale)
{
    int64 sz = (int64)cols * rows * widthScale;
    തർക്കം has_int_overflow = sz >= INT_MAX;
    തർക്കം isContiguous = (flags & Mat::CONTINUOUS_FLAG) != 0;
    തിരികെ (isContiguous && !has_int_overflow)
            ? Size((പൂർണ്ണസംഖ്യ)sz, 1)
            : Size(cols * widthScale, rows);
}

Size getContinuousSize2D(Mat& m1, പൂർണ്ണസംഖ്യ widthScale)
{
    CV_CheckLE(m1.dims, 2, "");
    തിരികെ getContinuousSize_(m1.flags,
                              m1.cols, m1.rows, widthScale);
}
Size getContinuousSize2D(Mat& m1, Mat& m2, പൂർണ്ണസംഖ്യ widthScale)
{
    CV_CheckLE(m1.dims, 2, "");
    CV_CheckLE(m2.dims, 2, "");
    സ്ഥിരം Size sz1 = m1.size();
    എങ്കിൽ (sz1 != m2.size())  // reshape all matrixes to the same size (#4159)
    {
        size_t total_sz = m1.total();
        CV_CheckEQ(total_sz, m2.total(), "");
        തർക്കം is_m1_vector = m1.cols == 1 || m1.rows == 1;
        തർക്കം is_m2_vector = m2.cols == 1 || m2.rows == 1;
        CV_Assert(is_m1_vector); CV_Assert(is_m2_vector);
        പൂർണ്ണസംഖ്യ total = (പൂർണ്ണസംഖ്യ)total_sz;  // vector-column
        തർക്കം isContiguous = ((m1.flags & m2.flags) & Mat::CONTINUOUS_FLAG) != 0;
        തർക്കം has_int_overflow = ((int64)total_sz * widthScale) >= INT_MAX;
        എങ്കിൽ (isContiguous && !has_int_overflow)
            total = 1; // vector-row
        m1 = m1.reshape(0, total);
        m2 = m2.reshape(0, total);
        CV_Assert(m1.cols == m2.cols && m1.rows == m2.rows);
        തിരികെ Size(m1.cols * widthScale, m1.rows);
    }
    തിരികെ getContinuousSize_(m1.flags & m2.flags,
                              m1.cols, m1.rows, widthScale);
}

Size getContinuousSize2D(Mat& m1, Mat& m2, Mat& m3, പൂർണ്ണസംഖ്യ widthScale)
{
    CV_CheckLE(m1.dims, 2, "");
    CV_CheckLE(m2.dims, 2, "");
    CV_CheckLE(m3.dims, 2, "");
    സ്ഥിരം Size sz1 = m1.size();
    എങ്കിൽ (sz1 != m2.size() || sz1 != m3.size())  // reshape all matrixes to the same size (#4159)
    {
        size_t total_sz = m1.total();
        CV_CheckEQ(total_sz, m2.total(), "");
        CV_CheckEQ(total_sz, m3.total(), "");
        തർക്കം is_m1_vector = m1.cols == 1 || m1.rows == 1;
        തർക്കം is_m2_vector = m2.cols == 1 || m2.rows == 1;
        തർക്കം is_m3_vector = m3.cols == 1 || m3.rows == 1;
        CV_Assert(is_m1_vector); CV_Assert(is_m2_vector); CV_Assert(is_m3_vector);
        പൂർണ്ണസംഖ്യ total = (പൂർണ്ണസംഖ്യ)total_sz;  // vector-column
        തർക്കം isContiguous = ((m1.flags & m2.flags & m3.flags) & Mat::CONTINUOUS_FLAG) != 0;
        തർക്കം has_int_overflow = ((int64)total_sz * widthScale) >= INT_MAX;
        എങ്കിൽ (isContiguous && !has_int_overflow)
            total = 1; // vector-row
        m1 = m1.reshape(0, total);
        m2 = m2.reshape(0, total);
        m3 = m3.reshape(0, total);
        CV_Assert(m1.cols == m2.cols && m1.rows == m2.rows && m1.cols == m3.cols && m1.rows == m3.rows);
        തിരികെ Size(m1.cols * widthScale, m1.rows);
    }
    തിരികെ getContinuousSize_(m1.flags & m2.flags & m3.flags,
                              m1.cols, m1.rows, widthScale);
}

} // cv::
