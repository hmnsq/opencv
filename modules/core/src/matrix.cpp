// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#અન્તર્ભાવ "precomp.hpp"
#અન્તર્ભાવ "bufferpool.impl.hpp"

નામાકાશ cv {

std::string layoutToString(DataLayout layout)
{
    પાછા
        layout == DATA_LAYOUT_ND ? "ND" :
        layout == DATA_LAYOUT_NCHW ? "NCHW" :
        layout == DATA_LAYOUT_NHWC ? "NHWC" :
        layout == DATA_LAYOUT_BLOCK ? "BLOCK" :
        layout == DATA_LAYOUT_NCDHW ? "NCDHW" :
        layout == DATA_LAYOUT_NDHWC ? "NDHWC" :
        layout == DATA_LAYOUT_PLANAR ? "PLANAR" :
        layout == DATA_LAYOUT_UNKNOWN ? "Unknown" : "???";
}

તાર્કિક operator == (અચલ MatShape& size1, અચલ MatShape& size2)
{
    જો (size1.dims != size2.dims)
        પાછા ખોટું;
    જો (size1.layout != size2.layout &&
        size1.layout != DATA_LAYOUT_UNKNOWN &&
        size2.layout != DATA_LAYOUT_UNKNOWN)
        પાછા ખોટું;
    જો (size1.layout == DATA_LAYOUT_BLOCK &&
        size2.layout == DATA_LAYOUT_BLOCK &&
        size1.C != size2.C)
        પાછા ખોટું;
    માટે (પૂર્ણાંક i = 0; i < size1.dims; i++) {
        જો (size1.p[i] != size2.p[i])
            પાછા ખોટું;
    }
    પાછા સાચું;
}

તાર્કિક operator != (અચલ MatShape& size1, અચલ MatShape& size2)
{
    પાછા !(size1 == size2);
}

/////////////////////////// MatShape ////////////////////////////////

MatShape MatShape::scalar()
{
    પાછા MatShape(0);
}

શૂન્ય MatShape::clear()
{
    dims = -1;
    layout = DATA_LAYOUT_UNKNOWN;
    C = 0;
    માટે (પૂર્ણાંક i = 0; i < MAX_DIMS; i++)
        p[i] = 0;
}

શૂન્ય MatShape::resize(size_t newSize, પૂર્ણાંક value)
{
    CV_Assert(newSize < (size_t)MAX_DIMS);
    પૂર્ણાંક old_dims = std::max(dims, 0);
    dims = (પૂર્ણાંક)newSize;
    માટે (પૂર્ણાંક i = old_dims; i < dims; i++)
        p[i] = value;
}

શૂન્ય MatShape::reserve(size_t)
{
    // no op; maybe need to add a check for overflow, but we check it anyway in other operations
}

શૂન્ય MatShape::assign(size_t newSize, પૂર્ણાંક value)
{
    CV_Assert(newSize < (size_t)MAX_DIMS);
    dims = (પૂર્ણાંક)newSize;
    માટે (પૂર્ણાંક i = 0; i < dims; i++)
        p[i] = value;
}

શૂન્ય MatShape::assign(પૂર્ણાંક newSize, પૂર્ણાંક value)
{
    assign((size_t)newSize, value);
}

શૂન્ય MatShape::assign(અચલ પૂર્ણાંક* begin, અચલ પૂર્ણાંક* end)
{
    assign_(begin, end);
}

શૂન્ય MatShape::assign_(અચલ પૂર્ણાંક* begin, અચલ પૂર્ણાંક* end)
{
    ptrdiff_t newSize = end - begin;
    CV_Assert(0 <= newSize && newSize < (ptrdiff_t)MAX_DIMS);
    dims = (પૂર્ણાંક)newSize;
    માટે (પૂર્ણાંક i = 0; i < dims; i++)
        p[i] = begin[i];
}

પૂર્ણાંક* MatShape::begin() { પાછા p; }
અચલ પૂર્ણાંક* MatShape::begin() અચલ { પાછા p; }
પૂર્ણાંક* MatShape::end() { પાછા p + std::max(dims, 0); }
અચલ પૂર્ણાંક* MatShape::end() અચલ { પાછા p + std::max(dims, 0); }
પૂર્ણાંક& MatShape::back() { પાછા p[std::max(dims-1, 0)]; }
અચલ પૂર્ણાંક& MatShape::back() અચલ { પાછા p[std::max(dims-1, 0)]; }

શૂન્ય MatShape::push_back(પૂર્ણાંક value)
{
    CV_Assert(dims+1 < MAX_DIMS);
    dims = std::max(dims+1, 1);
    p[dims-1] = value;
}

શૂન્ય MatShape::emplace_back(પૂર્ણાંક value)
{
    push_back(value);
}

શૂન્ય MatShape::insert(પૂર્ણાંક* where, પૂર્ણાંક value)
{
    પૂર્ણાંક old_dims = std::max(dims, 0);
    CV_Assert(old_dims+1 < MAX_DIMS);
    ptrdiff_t ofs = where - p;
    CV_Assert(0 <= ofs && ofs <= old_dims);
    dims = old_dims+1;
    માટે (પૂર્ણાંક i = old_dims-1; i >= (પૂર્ણાંક)ofs; i--)
        p[i+1] = p[i];
    p[ofs] = value;
}

શૂન્ય MatShape::insert(પૂર્ણાંક* where, size_t ગણતરી_2, પૂર્ણાંક value)
{
    પૂર્ણાંક old_dims = std::max(dims, 0);
    CV_Assert((size_t)(old_dims+ગણતરી_2) < (size_t)MAX_DIMS);
    ptrdiff_t ofs = where - p;
    CV_Assert(0 <= ofs && ofs <= old_dims);
    dims = (પૂર્ણાંક)(old_dims+ગણતરી_2);
    માટે (પૂર્ણાંક i = old_dims-1; i >= (પૂર્ણાંક)ofs; i--)
        p[i+ગણતરી_2] = p[i];
    માટે (પૂર્ણાંક i = 0; i < (પૂર્ણાંક)ગણતરી_2; i++)
        p[i+ofs] = value;
}

શૂન્ય MatShape::insert(પૂર્ણાંક* where, પૂર્ણાંક ગણતરી_2, પૂર્ણાંક value)
{
    insert(where, (size_t)ગણતરી_2, value);
}

શૂન્ય MatShape::insert(પૂર્ણાંક* where, અચલ પૂર્ણાંક* begin, અચલ પૂર્ણાંક* end)
{
    ઉમેરો(where, begin, end);
}

શૂન્ય MatShape::ઉમેરો(પૂર્ણાંક* where, અચલ પૂર્ણાંક* begin, અચલ પૂર્ણાંક* end)
{
    પૂર્ણાંક old_dims = std::max(dims, 0);
    ptrdiff_t delta = end - begin;
    CV_Assert(0 <= delta && old_dims+delta < MAX_DIMS);
    ptrdiff_t ofs = where - p;
    CV_Assert(0 <= ofs && ofs <= old_dims);
    dims = (પૂર્ણાંક)(old_dims+delta);
    માટે (પૂર્ણાંક i = old_dims-1; i >= (પૂર્ણાંક)ofs; i--)
        p[i+delta] = p[i];
    માટે (પૂર્ણાંક i = 0; i < (પૂર્ણાંક)delta; i++)
        p[i+ofs] = begin[i];
}

શૂન્ય MatShape::erase(પૂર્ણાંક* where)
{
    CV_Assert(dims > 0);
    ptrdiff_t ofs = where - p;
    CV_Assert(0 <= ofs && ofs <= dims);
    જો (ofs == dims)
        પાછા;
    dims--;
    માટે (પૂર્ણાંક i = (પૂર્ણાંક)ofs+1; i <= dims; i++)
        p[i-1] = p[i];
}

size_t MatShape::total() અચલ
{
    size_t result = 1;
    જો (dims < 0)
        પાછા 0;
    માટે (પૂર્ણાંક i = 0; i < dims; i++)
        result *= p[i];
    પાછા result;
}

std::vector<પૂર્ણાંક> MatShape::vec() અચલ
{
    જો (dims < 0)
        પાછા std::vector<પૂર્ણાંક>(1, 0);
    પાછા std::vector<પૂર્ણાંક>(p, p + dims);
}

std::string MatShape::str() અચલ
{
    std::stringstream sstrm;
    જો (empty()) {
        sstrm << "<empty>";
    } નહીંતર જો (dims == 0) {
        sstrm << "<scalar>";
    } નહીંતર {
        sstrm << "[";
        માટે (પૂર્ણાંક i = 0; i < dims; i++) {
            sstrm << (i > 0 ? " x " : "") << p[i];
        }
        sstrm << "]";
    }
    પાછા sstrm.str();
}

સ્થિર શૂન્ય finalizeBlockLayout(MatShape& size, પૂર્ણાંક C=0)
{
    જો (size.layout == DATA_LAYOUT_BLOCK) {
        CV_Assert(size.dims >= 4);
        પૂર્ણાંક C0 = size.p[size.dims-1];
        CV_Assert(C0 > 1 && (C0 & (C0-1)) == 0);
        size.C = C > 0 ? C : size.p[1]*size.p[size.dims-1];
    } નહીંતર {
        size.C = 0;
    }
    માટે (પૂર્ણાંક i = std::max(size.dims, 0); i < MatShape::MAX_DIMS; i++)
        size.p[i] = 0;
    જો (size.dims == 0)
        size.p[0] = 1;
}

MatShape::MatShape()
{
    clear();
}

MatShape::MatShape(size_t dims_, અચલ પૂર્ણાંક* size_, DataLayout layout_, પૂર્ણાંક C_)
{
    layout = layout_;
    CV_Assert(dims_ <= (size_t)MAX_DIMS);
    dims = (પૂર્ણાંક)dims_;
    માટે (પૂર્ણાંક i = 0; i < dims; i++) {
        p[i] = size_ ? size_[i] : 0;
    }
    finalizeBlockLayout(*આ, C_);
}

MatShape::MatShape(size_t dims_, પૂર્ણાંક value, DataLayout layout_)
{
    layout = layout_;
    CV_Assert(dims_ <= (size_t)MAX_DIMS);
    dims = (પૂર્ણાંક)dims_;
    માટે (પૂર્ણાંક i = 0; i < dims; i++) {
        p[i] = value;
    }
    finalizeBlockLayout(*આ, 0);
}

MatShape::MatShape(std::initializer_list<પૂર્ણાંક> shape)
{
    layout = DATA_LAYOUT_UNKNOWN;
    CV_Assert(shape.size() <= (size_t)MAX_DIMS);
    dims = (પૂર્ણાંક)shape.size();
    auto it = shape.begin();
    માટે (પૂર્ણાંક i = 0; i < dims; i++, ++it) {
        p[i] = *it;
    }
    finalizeBlockLayout(*આ, 0);
}

MatShape::MatShape(પૂર્ણાંક dims_, પૂર્ણાંક value, DataLayout layout_)
{
    layout = layout_;
    CV_Assert(dims_ <= MAX_DIMS);
    dims = dims_;
    માટે (પૂર્ણાંક i = 0; i < dims; i++) {
        p[i] = value;
    }
    finalizeBlockLayout(*આ, 0);
}

MatShape::MatShape(અચલ std::vector<પૂર્ણાંક>& shape_, DataLayout layout_, પૂર્ણાંક C_)
{
    layout = layout_;
    size_t shape_size = shape_.size();
    CV_Assert(shape_size < (size_t)MAX_DIMS);
    dims = (પૂર્ણાંક)shape_size;
    માટે (પૂર્ણાંક i = 0; i < dims; i++) {
        p[i] = shape_[i];
    }
    finalizeBlockLayout(*આ, C_);
}

MatShape::MatShape(અચલ પૂર્ણાંક* begin, અચલ પૂર્ણાંક* end, DataLayout layout_, પૂર્ણાંક C_)
{
    layout = layout_;
    ptrdiff_t shape_size = end - begin;
    CV_Assert(0 <= shape_size && shape_size < MAX_DIMS);
    dims = (પૂર્ણાંક)shape_size;
    માટે (પૂર્ણાંક i = 0; i < dims; i++) {
        p[i] = begin[i];
    }
    finalizeBlockLayout(*આ, C_);
}

MatShape::MatShape(અચલ MatShape& shape)
{
    dims = shape.dims;
    layout = shape.layout;
    C = shape.C;
    માટે (પૂર્ણાંક i = 0; i < MAX_DIMS; i++)
        p[i] = shape.p[i];
}

MatShape& MatShape::operator = (અચલ MatShape& shape)
{
    જો (આ != &shape) {
        dims = shape.dims;
        layout = shape.layout;
        C = shape.C;
        માટે (પૂર્ણાંક i = 0; i < MAX_DIMS; i++)
            p[i] = shape.p[i];
    }
    પાછા *આ;
}

તાર્કિક MatShape::hasSymbols() અચલ
{
    માટે (પૂર્ણાંક i = 0; i < dims; i++) {
        જો (p[i] < 0)
            પાછા સાચું;
    }
    પાછા ખોટું;
}

પૂર્ણાંક MatShape::channels() અચલ
{
    CV_Assert(layout == DATA_LAYOUT_BLOCK || layout == DATA_LAYOUT_NCHW || layout == DATA_LAYOUT_NHWC);
    પાછા layout == DATA_LAYOUT_BLOCK ? C : p[layout == DATA_LAYOUT_NCHW ? 1 : dims-1];
}

MatShape MatShape::toLayout(DataLayout newLayout, પૂર્ણાંક C0) અચલ
{
    CV_Assert(layout == DATA_LAYOUT_BLOCK || layout == DATA_LAYOUT_NCHW || layout == DATA_LAYOUT_NHWC);
    CV_Assert(newLayout == DATA_LAYOUT_BLOCK || newLayout == DATA_LAYOUT_NCHW || newLayout == DATA_LAYOUT_NHWC);

    MatShape newsize = *આ;
    newsize.layout = newLayout;

    જો (newLayout == DATA_LAYOUT_BLOCK) {
        // any => BLOCK
        CV_Assert_N(C0 > 1, (C0 & (C0-1)) == 0);
        પૂર્ણાંક Corig = channels();
        newsize.C = Corig;

        જો (layout == DATA_LAYOUT_NHWC) {
            માટે (પૂર્ણાંક i = 2; i < dims; i++) {
                newsize.p[i] = p[i-1];
            }
        }

        newsize.dims += layout != DATA_LAYOUT_BLOCK;
        newsize.p[1] = (Corig + C0 - 1)/C0;
        newsize.p[newsize.dims-1] = C0;
    } નહીંતર જો (layout == DATA_LAYOUT_BLOCK) {
        // BLOCK => any (except for BLOCK, which is handled above)
        newsize.C = 0;
        જો (newLayout == DATA_LAYOUT_NHWC) {
            માટે (પૂર્ણાંક i = 2; i < dims; i++) {
                newsize.p[i-1] = p[i];
            }
        }
        newsize.p[newLayout == DATA_LAYOUT_NCHW ? 1 : dims-2] = C;
        newsize.dims--;
    } નહીંતર {
        CV_Assert_N(C0 <= 1);

        જો (newLayout == layout)
            પાછા newsize;

        // NHWC => NCHW
        જો (newLayout == DATA_LAYOUT_NCHW) {
            માટે (પૂર્ણાંક i = 2; i < dims; i++)
                newsize.p[i] = p[i-1];
            newsize.p[1] = p[dims-1];
        } નહીંતર {
            // NCHW => NHWC
            માટે (પૂર્ણાંક i = 2; i < dims; i++)
                newsize.p[i-1] = p[i];
            newsize.p[dims-1] = p[1];
        }
    }

    પાછા newsize;
}

સ્થિર પૂર્ણાંક normalizeShapeAxis(પૂર્ણાંક axis, પૂર્ણાંક dims)
{
    CV_CheckGE(axis, -dims, "axis is out of range");
    CV_CheckLT(axis, dims, "axis is out of range");
    પાછા axis < 0 ? axis + dims : axis;
}

// The semantic NCHW form {N, C, spatial...} of a block ('NC1HWC0') shape, so that axes and
// element counts are the logical ones rather than the C0-padded ones.
સ્થિર MatShape toSemantic(અચલ MatShape& shape)
{
    CV_CheckGE(shape.dims, 3, "a block shape is {N, C1, spatial..., C0}");
    પાછા shape.toLayout(DATA_LAYOUT_NCHW);
}

// Blocks such a result again. Once N or C changes, elements cross the padded C0 lanes and
// the data needs a repack, which a shape operation cannot express.
સ્થિર MatShape toBlockOf(MatShape result, અચલ MatShape& src, અચલ અક્ષર* op)
{
    જો (result.dims < 2 || result.p[0] != src.p[0] || result.p[1] != src.C)
        CV_Error_(Error::StsNotImplemented,
                  ("%s: the block layout is kept only when the batch and the channel axes stay "
                   "unchanged; convert the shape with toLayout() first", op));
    CV_CheckLE(result.dims + 1, (પૂર્ણાંક)MatShape::MAX_DIMS, "too many dimensions");
    result.layout = DATA_LAYOUT_NCHW;
    પાછા result.toLayout(DATA_LAYOUT_BLOCK, src.p[src.dims-1]);
}

MatShape MatShape::squeeze(અચલ std::vector<પૂર્ણાંક>& axes) અચલ
{
    CV_Assert(dims >= 0);
    જો (layout == DATA_LAYOUT_BLOCK)
        પાછા toBlockOf(toSemantic(*આ).squeeze(axes), *આ, "squeeze");
    તાર્કિક remove[MAX_DIMS] = {ખોટું};
    જો (axes.empty())
    {
        માટે (પૂર્ણાંક i = 0; i < dims; i++)
            remove[i] = p[i] == 1;
    }
    માટે (પૂર્ણાંક a : axes)
    {
        પૂર્ણાંક i = normalizeShapeAxis(a, dims);
        CV_Check(a, !remove[i], "squeeze: duplicate axis");
        CV_Check(p[i], p[i] == 1, "squeeze: the size of a squeezed axis must be 1");
        remove[i] = સાચું;
    }
    // a new number of axes: the layout of this shape does not apply
    MatShape result;
    result.dims = 0;
    માટે (પૂર્ણાંક i = 0; i < dims; i++)
        જો (!remove[i])
            result.p[result.dims++] = p[i];
    પાછા result;
}

MatShape MatShape::unsqueeze(અચલ std::vector<પૂર્ણાંક>& axes) અચલ
{
    CV_Assert(dims >= 0);
    જો (layout == DATA_LAYOUT_BLOCK)
        પાછા toBlockOf(toSemantic(*આ).unsqueeze(axes), *આ, "unsqueeze");
    અચલ પૂર્ણાંક outDims = dims + (પૂર્ણાંક)axes.size();
    CV_CheckLE(outDims, (પૂર્ણાંક)MAX_DIMS, "unsqueeze: too many dimensions");
    તાર્કિક insert[MAX_DIMS] = {ખોટું};
    માટે (પૂર્ણાંક a : axes)
    {
        પૂર્ણાંક i = normalizeShapeAxis(a, outDims);
        CV_Check(a, !insert[i], "unsqueeze: duplicate axis");
        insert[i] = સાચું;
    }
    // a new number of axes: the layout of this shape does not apply
    MatShape result;
    result.dims = outDims;
    માટે (પૂર્ણાંક i = 0, j = 0; i < outDims; i++)
        result.p[i] = insert[i] ? 1 : p[j++];
    પાછા result;
}

MatShape MatShape::flatten(પૂર્ણાંક startAxis, પૂર્ણાંક endAxis) અચલ
{
    CV_Assert(dims >= 0);
    જો (layout == DATA_LAYOUT_BLOCK)
        પાછા toBlockOf(toSemantic(*આ).flatten(startAxis, endAxis), *આ, "flatten");
    જો (dims == 0)
    {
        CV_Check(startAxis, startAxis == 0 || startAxis == -1, "flatten: axis is out of range");
        CV_Check(endAxis, endAxis == 0 || endAxis == -1, "flatten: axis is out of range");
        પાછા MatShape({1});
    }
    પૂર્ણાંક start = normalizeShapeAxis(startAxis, dims), end = normalizeShapeAxis(endAxis, dims);
    CV_CheckLE(start, end, "flatten: startAxis must not follow endAxis");
    int64 merged = 1;
    માટે (પૂર્ણાંક i = start; i <= end; i++)
        merged *= p[i];
    CV_CheckLE((દ્વિગુણ)merged, (દ્વિગુણ)INT_MAX, "flatten: the result is too big");
    // a new number of axes: the layout of this shape does not apply
    MatShape result;
    result.dims = 0;
    માટે (પૂર્ણાંક i = 0; i < dims; i++)
    {
        જો (i == start)
            result.p[result.dims++] = (પૂર્ણાંક)merged;
        નહીંતર જો (i < start || i > end)
            result.p[result.dims++] = p[i];
    }
    પાછા result;
}

MatShape MatShape::reshape(અચલ MatShape& newShape, તાર્કિક allowZero) અચલ
{
    CV_Assert(dims >= 0 && newShape.dims >= 0);
    જો (layout == DATA_LAYOUT_BLOCK)
        પાછા toBlockOf(toSemantic(*આ).reshape(newShape, allowZero), *આ, "reshape");
    // a new number of axes: neither the layout of this shape nor the one of the spec applies
    MatShape result;
    result.dims = newShape.dims;
    std::copy(newShape.p, newShape.p + newShape.dims, result.p);
    પૂર્ણાંક inferred = -1;
    int64 known = 1;
    માટે (પૂર્ણાંક i = 0; i < result.dims; i++)
    {
        પૂર્ણાંક& sz = result.p[i];
        જો (sz == -1)
        {
            CV_Check(i, inferred < 0, "reshape: at most one axis may be -1");
            inferred = i;
            ચાલુ;
        }
        CV_Check(sz, sz >= 0, "reshape: sizes must be -1 or non-negative");
        જો (sz == 0 && !allowZero)
        {
            CV_Check(i, i < dims, "reshape: 0 refers to an axis the source does not have");
            sz = p[i];
        }
        known *= sz;
    }
    અચલ int64 total_ = (int64)total();
    જો (inferred >= 0)
    {
        CV_Check((દ્વિગુણ)known, known != 0, "reshape: cannot infer the -1 axis next to a zero-size axis");
        CV_Check((દ્વિગુણ)total_, total_ % known == 0, "reshape: the number of elements does not match");
        CV_CheckLE((દ્વિગુણ)(total_/known), (દ્વિગુણ)INT_MAX, "reshape: the result is too big");
        result.p[inferred] = (પૂર્ણાંક)(total_/known);
    }
    નહીંતર જો (known != total_)
        CV_Error_(Error::StsBadArg, ("reshape: the number of elements does not match: %lld vs %lld",
                                     (long long)known, (long long)total_));
    પાછા result;
}

MatShape MatShape::expand(અચલ MatShape& another) અચલ
{
    જો (dims == 0)
        પાછા another;
    જો (another.dims == 0)
        પાછા *આ;

    જો ((layout == DATA_LAYOUT_NCHW || layout == DATA_LAYOUT_NHWC) &&
        (another.layout == DATA_LAYOUT_NCHW || another.layout == DATA_LAYOUT_NHWC)) {
        CV_Assert(layout == another.layout);
    }
    // [TODO] support block layout
    CV_Assert(layout != DATA_LAYOUT_BLOCK && another.layout != DATA_LAYOUT_BLOCK);

    MatShape result;

    જો (dims < 0 || another.dims < 0)
        પાછા result;

    result = *આ;
    result.dims = std::max(dims, another.dims);
    result.layout = layout == DATA_LAYOUT_UNKNOWN ? another.layout :
        layout == DATA_LAYOUT_ND && (another.layout == DATA_LAYOUT_NCHW ||
        another.layout == DATA_LAYOUT_NHWC) ? another.layout : layout;
    માટે (પૂર્ણાંક i = result.dims-1; i >= 0; i--) {
        પૂર્ણાંક i1 = i - (result.dims - dims);
        પૂર્ણાંક i2 = i - (result.dims - another.dims);
        પૂર્ણાંક sz1 = i1 < 0 ? 1 : p[i1];
        પૂર્ણાંક sz2 = i2 < 0 ? 1 : another.p[i2];
        CV_Assert(sz1 == sz2 || sz1 == 1 || sz2 == 1);
        // [TODO] handle symbolic shapes
        result.p[i] = sz1 != 1 ? sz1 : sz2;
    }
    પાછા result;
}

/////////////////////////// MatAllocator ////////////////////////////

શૂન્ય MatAllocator::map(UMatData*, AccessFlag) અચલ
{
}

શૂન્ય MatAllocator::unmap(UMatData* u) અચલ
{
    જો(u->urefcount == 0 && u->refcount == 0)
    {
        deallocate(u);
    }
}

શૂન્ય MatAllocator::download(UMatData* u, શૂન્ય* dstptr,
         પૂર્ણાંક dims, અચલ size_t sz[],
         અચલ size_t srcofs[], અચલ size_t srcstep[],
         અચલ size_t dststep[]) અચલ
{
    જો(!u)
        પાછા;
    પૂર્ણાંક isz[CV_MAX_DIM];
    uchar* srcptr = u->data;
    માટે( પૂર્ણાંક i = 0; i < dims; i++ )
    {
        CV_Assert( sz[i] <= (size_t)INT_MAX );
        જો( sz[i] == 0 )
            પાછા;
        જો( srcofs )
            srcptr += srcofs[i]*(i <= dims-2 ? srcstep[i] : 1);
        isz[i] = (પૂર્ણાંક)sz[i];
    }

    Mat src(dims, isz, CV_8U, srcptr, srcstep);
    Mat dst(dims, isz, CV_8U, dstptr, dststep);

    અચલ Mat* arrays[] = { &src, &dst };
    uchar* ptrs[2];
    NAryMatIterator it(arrays, ptrs, 2);
    size_t planesz = it.size;

    માટે( size_t j = 0; j < it.nplanes; j++, ++it )
        memcpy(ptrs[1], ptrs[0], planesz);
}


શૂન્ય MatAllocator::upload(UMatData* u, અચલ શૂન્ય* srcptr, પૂર્ણાંક dims, અચલ size_t sz[],
                    અચલ size_t dstofs[], અચલ size_t dststep[],
                    અચલ size_t srcstep[]) અચલ
{
    જો(!u)
        પાછા;
    પૂર્ણાંક isz[CV_MAX_DIM];
    uchar* dstptr = u->data;
    માટે( પૂર્ણાંક i = 0; i < dims; i++ )
    {
        CV_Assert( sz[i] <= (size_t)INT_MAX );
        જો( sz[i] == 0 )
            પાછા;
        જો( dstofs )
            dstptr += dstofs[i]*(i <= dims-2 ? dststep[i] : 1);
        isz[i] = (પૂર્ણાંક)sz[i];
    }

    Mat src(dims, isz, CV_8U, (શૂન્ય*)srcptr, srcstep);
    Mat dst(dims, isz, CV_8U, dstptr, dststep);

    અચલ Mat* arrays[] = { &src, &dst };
    uchar* ptrs[2];
    NAryMatIterator it(arrays, ptrs, 2);
    size_t planesz = it.size;

    માટે( size_t j = 0; j < it.nplanes; j++, ++it )
        memcpy(ptrs[1], ptrs[0], planesz);
}

શૂન્ય MatAllocator::copy(UMatData* usrc, UMatData* udst, પૂર્ણાંક dims, અચલ size_t sz[],
                  અચલ size_t srcofs[], અચલ size_t srcstep[],
                  અચલ size_t dstofs[], અચલ size_t dststep[], તાર્કિક /*sync*/) અચલ
{
    CV_INSTRUMENT_REGION();

    જો(!usrc || !udst)
        પાછા;
    પૂર્ણાંક isz[CV_MAX_DIM];
    uchar* srcptr = usrc->data;
    uchar* dstptr = udst->data;
    માટે( પૂર્ણાંક i = 0; i < dims; i++ )
    {
        CV_Assert( sz[i] <= (size_t)INT_MAX );
        જો( sz[i] == 0 )
            પાછા;
        જો( srcofs )
            srcptr += srcofs[i]*(i <= dims-2 ? srcstep[i] : 1);
        જો( dstofs )
            dstptr += dstofs[i]*(i <= dims-2 ? dststep[i] : 1);
        isz[i] = (પૂર્ણાંક)sz[i];
    }

    Mat src(dims, isz, CV_8U, srcptr, srcstep);
    Mat dst(dims, isz, CV_8U, dstptr, dststep);

    અચલ Mat* arrays[] = { &src, &dst };
    uchar* ptrs[2];
    NAryMatIterator it(arrays, ptrs, 2);
    size_t planesz = it.size;

    માટે( size_t j = 0; j < it.nplanes; j++, ++it )
        memcpy(ptrs[1], ptrs[0], planesz);
}

BufferPoolController* MatAllocator::getBufferPoolController(અચલ અક્ષર* id) અચલ
{
    CV_UNUSED(id);
    સ્થિર DummyBufferPoolController dummy;
    પાછા &dummy;
}

વર્ગ StdMatAllocator CV_FINAL : જાહેર MatAllocator
{
જાહેર:
    UMatData* allocate(પૂર્ણાંક dims, અચલ પૂર્ણાંક* sizes, પૂર્ણાંક type,
                       શૂન્ય* data0, size_t* step, AccessFlag /*flags*/, UMatUsageFlags /*usageFlags*/) અચલ CV_OVERRIDE
    {
        size_t total = CV_ELEM_SIZE(type);
        માટે( પૂર્ણાંક i = dims-1; i >= 0; i-- )
        {
            જો( step )
            {
                જો( data0 && step[i] != CV_AUTOSTEP )
                {
                    CV_Assert(total <= step[i]);
                    total = step[i];
                }
                નહીંતર
                    step[i] = total;
            }
            total *= sizes[i];
        }
        uchar* data = data0 ? (uchar*)data0 : (uchar*)fastMalloc(total);
        UMatData* u = નવું UMatData(આ);
        u->data = u->origdata = data;
        u->size = total;
        જો(data0)
            u->flags |= UMatData::USER_ALLOCATED;

        પાછા u;
    }

    તાર્કિક allocate(UMatData* u, AccessFlag /*accessFlags*/, UMatUsageFlags /*usageFlags*/) અચલ CV_OVERRIDE
    {
        જો(!u) પાછા ખોટું;
        પાછા સાચું;
    }

    શૂન્ય deallocate(UMatData* u) અચલ CV_OVERRIDE
    {
        જો(!u)
            પાછા;

        CV_Assert(u->urefcount == 0);
        CV_Assert(u->refcount == 0);
        જો( !(u->flags & UMatData::USER_ALLOCATED) )
        {
            fastFree(u->origdata);
            u->origdata = 0;
        }
        delete u;
    }
};

સ્થિર
MatAllocator*& getDefaultAllocatorMatRef()
{
    સ્થિર MatAllocator* g_matAllocator = Mat::getStdAllocator();
    પાછા g_matAllocator;
}

MatAllocator* Mat::getDefaultAllocator()
{
    પાછા getDefaultAllocatorMatRef();
}

શૂન્ય Mat::setDefaultAllocator(MatAllocator* allocator)
{
    getDefaultAllocatorMatRef() = allocator;
}

MatAllocator* Mat::getStdAllocator()
{
    CV_SINGLETON_LAZY_INIT(MatAllocator, નવું StdMatAllocator())
}

//==================================================================================================

શૂન્ય setSize( Mat& m, પૂર્ણાંક _dims, અચલ પૂર્ણાંક* _sz, અચલ size_t* _steps, તાર્કિક autoSteps)
{
    CV_Assert( 0 <= _dims && _dims <= CV_MAX_DIM && _dims <= MatShape::MAX_DIMS);

    m.dims = _dims;
    m.size = MatShape(_dims, _sz);
    m.step[std::max(_dims-1, 0)] = CV_ELEM_SIZE(m.flags);
    માટે (પૂર્ણાંક i = _dims-2; i >= 0; i--) {
        size_t autostep = m.size[i+1]*m.step[i+1];
        જો (_steps) {
            m.step[i] = _steps[i];
            //CV_Assert(m.step[i] >= autostep);
        } નહીંતર જો (autoSteps) {
            m.step[i] = autostep;
        } નહીંતર {
            m.step[i] = 0;
        }
    }

    જો( _dims <= 2 )
    {
        m.cols = _dims == 0 ? 1 : _sz ? _sz[_dims > 1] : 0;
        m.rows = _dims < 2 ? 1 : _sz ? _sz[0] : 0;
    } નહીંતર {
        m.cols = m.rows = -1;
    }
}

પૂર્ણાંક updateContinuityFlag(પૂર્ણાંક flags, પૂર્ણાંક dims, અચલ પૂર્ણાંક* size, અચલ size_t* step)
{
    પૂર્ણાંક i, j;
    જો (dims <= 1)
        પાછા flags | Mat::CONTINUOUS_FLAG;
    માટે( i = 0; i < dims; i++ )
    {
        જો( size[i] > 1 )
            તોડો;
    }

    uint64 t = (uint64)size[std::min(i, dims-1)]*CV_MAT_CN(flags);
    માટે( j = dims-1; j > i; j-- )
    {
        t *= size[j];
        જો( step[j]*size[j] < step[j-1] )
            તોડો;
    }

    જો( j <= i && t == (uint64)(પૂર્ણાંક)t )
        પાછા flags | Mat::CONTINUOUS_FLAG;
    પાછા flags & ~Mat::CONTINUOUS_FLAG;
}

શૂન્ય Mat::updateContinuityFlag()
{
    flags = cv::updateContinuityFlag(flags, dims, size.p, step.p);
}

શૂન્ય finalizeHdr(Mat& m)
{
    m.updateContinuityFlag();
    પૂર્ણાંક d = m.dims;
    જો( d > 2 )
        m.rows = m.cols = -1;
    જો(m.u)
        m.datastart = m.data = m.u->data;
    જો( m.data && d > 0 )
    {
        m.datalimit = m.datastart + m.size[0]*m.step[0];
        જો( m.size[0] > 0 )
        {
            પૂર્ણાંક lastdim = d > 0 ? d - 1 : 0;
            m.dataend = m.ptr() + m.size[lastdim]*m.step[lastdim];
            માટે( પૂર્ણાંક i = 0; i < d-1; i++ )
                m.dataend += (m.size[i] - 1)*m.step[i];
        }
        નહીંતર
            m.dataend = m.datalimit;
    }
    નહીંતર
        m.dataend = m.datalimit = m.data;
}

//======================================= Mat ======================================================

Mat::Mat() CV_NOEXCEPT
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{}

Mat::Mat(પૂર્ણાંક _rows, પૂર્ણાંક _cols, પૂર્ણાંક _type)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_rows, _cols, _type);
}

Mat::Mat(પૂર્ણાંક _rows, પૂર્ણાંક _cols, પૂર્ણાંક _type, અચલ Scalar& _s)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_rows, _cols, _type);
    *આ = _s;
}

Mat::Mat(Size _sz, પૂર્ણાંક _type)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create( _sz.height, _sz.width, _type );
}

Mat::Mat(Size _sz, પૂર્ણાંક _type, અચલ Scalar& _s)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_sz.height, _sz.width, _type);
    *આ = _s;
}

Mat::Mat(પૂર્ણાંક _dims, અચલ પૂર્ણાંક* _sz, પૂર્ણાંક _type)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_dims, _sz, _type);
}

Mat::Mat(પૂર્ણાંક _dims, અચલ પૂર્ણાંક* _sz, પૂર્ણાંક _type, અચલ Scalar& _s)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_dims, _sz, _type);
    *આ = _s;
}

Mat::Mat(અચલ std::vector<પૂર્ણાંક>& _sz, પૂર્ણાંક _type)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_sz, _type);
}

Mat::Mat(અચલ std::vector<પૂર્ણાંક>& _sz, પૂર્ણાંક _type, અચલ Scalar& _s)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_sz, _type);
    *આ = _s;
}

Mat::Mat(અચલ MatShape& _shape, પૂર્ણાંક _type)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_shape, _type);
}

Mat::Mat(std::initializer_list<પૂર્ણાંક> _shape, પૂર્ણાંક _type)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_shape, _type);
}

Mat::Mat(અચલ MatShape& _shape, પૂર્ણાંક _type, અચલ Scalar& _s)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_shape, _type);
    *આ = _s;
}

Mat::Mat(std::initializer_list<પૂર્ણાંક> _shape, પૂર્ણાંક _type, અચલ Scalar& _s)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_shape, _type);
    *આ = _s;
}

Mat::Mat(અચલ Mat& m)
    : flags(m.flags), dims(m.dims), rows(m.rows), cols(m.cols), data(m.data),
      datastart(m.datastart), dataend(m.dataend), datalimit(m.datalimit), allocator(m.allocator),
      u(m.u), size(m.size), step(m.step)
{
    જો( u )
        CV_XADD(&u->refcount, 1);
}

Mat::Mat(પૂર્ણાંક _rows, પૂર્ણાંક _cols, પૂર્ણાંક _type, શૂન્ય* _data, size_t _step)
    : flags(MAGIC_VAL + (_type & TYPE_MASK)), dims(2), rows(_rows), cols(_cols),
      data((uchar*)_data), datastart((uchar*)_data), dataend(0), datalimit(0),
      allocator(0), u(0), size(2)
{
    CV_Assert(total() == 0 || data != ખાલી);

    size_t esz = CV_ELEM_SIZE(_type), esz1 = CV_ELEM_SIZE1(_type);
    size_t minstep = cols * esz;
    જો( _step == AUTO_STEP )
    {
        _step = minstep;
    }
    નહીંતર
    {
        CV_Assert( _step >= minstep );
        જો (_step % esz1 != 0)
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

Mat::Mat(Size _sz, પૂર્ણાંક _type, શૂન્ય* _data, size_t _step)
    : flags(MAGIC_VAL + (_type & TYPE_MASK)), dims(2), rows(_sz.height), cols(_sz.width),
      data((uchar*)_data), datastart((uchar*)_data), dataend(0), datalimit(0),
      allocator(0), u(0), size(2)
{
    CV_Assert(total() == 0 || data != ખાલી);

    size_t esz = CV_ELEM_SIZE(_type), esz1 = CV_ELEM_SIZE1(_type);
    size_t minstep = cols*esz;
    જો( _step == AUTO_STEP || (_step < minstep && rows == 1))
    {
        _step = minstep;
    }
    નહીંતર
    {
        CV_CheckGE(_step, minstep, "");

        જો (_step % esz1 != 0)
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

Mat& Mat::operator = (અચલ Mat& m)
{
    જો( આ != &m )
    {
        જો( m.u )
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
    પાછા *આ;
}

Mat Mat::clone() અચલ
{
    Mat m;
    copyTo(m);
    પાછા m;
}

શૂન્ય Mat::assignTo( Mat& m, પૂર્ણાંક _type ) અચલ
{
    જો( _type < 0 )
        m = *આ;
    નહીંતર
        convertTo(m, _type);
}

શૂન્ય Mat::create(પૂર્ણાંક _rows, પૂર્ણાંક _cols, પૂર્ણાંક _type)
{
    _type &= TYPE_MASK;
    જો( dims <= 2 && rows == _rows && cols == _cols && type() == _type && data )
        પાછા;
    પૂર્ણાંક sz[] = {_rows, _cols};
    create(2, sz, _type);
}

શૂન્ય Mat::create(Size _sz, પૂર્ણાંક _type)
{
    create(_sz.height, _sz.width, _type);
}

શૂન્ય Mat::createSameSize(આગત_Array m, પૂર્ણાંક type)
{
    MatShape msize = m.shape();
    create(msize, type);
}

શૂન્ય Mat::fit(પૂર્ણાંક _dims, અચલ પૂર્ણાંક* _sizes, પૂર્ણાંક _type)
{
    size_t oldTotalBytes = u ? u->size : 0;
    size_t esz = CV_ELEM_SIZE(_type), newTotal = _dims >= 0;
    માટે (પૂર્ણાંક i = 0; i < _dims; i++)
        newTotal *= _sizes[i];
    size_t newTotalBytes = newTotal*esz;
    જો (newTotalBytes > 0 && (!isContinuous() ||
                              newTotalBytes > oldTotalBytes ||
                              data != datastart)) {
        create(_dims, _sizes, _type);
    } નહીંતર {
        flags = (flags & ~Mat::TYPE_MASK) | CV_MAT_TYPE(_type);
        પૂર્ણાંક _dummy_size = 0;
        setSize(*આ, (_dims >= 0 ? _dims : 1), (_dims >= 0 ? _sizes : &_dummy_size), nullptr, સાચું);
        finalizeHdr(*આ);
    }
}

શૂન્ય Mat::fit(અચલ std::vector<પૂર્ણાંક>& _shape, પૂર્ણાંક _type)
{
    fit((પૂર્ણાંક)_shape.size(), _shape.data(), _type);
}

શૂન્ય Mat::fit(અચલ MatShape& _shape, પૂર્ણાંક _type)
{
    fit(_shape.dims, _shape.p, _type);
    size.layout = _shape.layout;
    size.C = _shape.C;
}

શૂન્ય Mat::fit(std::initializer_list<પૂર્ણાંક> _shape, પૂર્ણાંક _type)
{
    પૂર્ણાંક new_shape[MatShape::MAX_DIMS];
    પૂર્ણાંક new_ndims = (પૂર્ણાંક)_shape.size();
    CV_Assert(new_ndims <= MatShape::MAX_DIMS);
    auto it = _shape.begin();
    માટે (પૂર્ણાંક i = 0; i < new_ndims; i++, ++it)
        new_shape[i] = *it;
    fit(new_ndims, new_shape, _type);
}

શૂન્ય Mat::fit(પૂર્ણાંક _rows, પૂર્ણાંક _cols, પૂર્ણાંક _type)
{
    _type &= TYPE_MASK;
    પૂર્ણાંક sz[] = {_rows, _cols};
    fit(2, sz, _type);
}

શૂન્ય Mat::fit(Size _sz, પૂર્ણાંક _type)
{
    fit(_sz.height, _sz.width, _type);
}

શૂન્ય Mat::fitSameSize(આગત_Array m, પૂર્ણાંક _type)
{
    fit(m.shape(), _type);
}

શૂન્ય Mat::addref()
{
    જો( u )
        CV_XADD(&u->refcount, 1);
}

શૂન્ય Mat::release()
{
    જો( u && CV_XADD(&u->refcount, -1) == 1 )
        deallocate();
    u = ખાલી;
    datastart = dataend = datalimit = data = 0;
    માટે(પૂર્ણાંક i = 0; i < dims; i++)
        size.p[i] = 0;
    flags = (flags & CV_MAT_TYPE_MASK) | MAGIC_VAL;
    dims = rows = cols = 0;
    size.clear();
}

size_t Mat::step1(પૂર્ણાંક i) અચલ
{
    પાછા step.p[i] / elemSize1();
}

તાર્કિક Mat::empty() અચલ
{
    પાછા data == 0 || total() == 0;
}

size_t Mat::total() અચલ
{
    જો( dims <= 2 )
        પાછા (size_t)rows * cols;
    size_t p = 1;
    માટે( પૂર્ણાંક i = 0; i < dims; i++ )
        p *= size[i];
    પાછા p;
}

size_t Mat::total(પૂર્ણાંક startDim, પૂર્ણાંક endDim) અચલ
{
    CV_Assert( 0 <= startDim && startDim <= endDim);
    size_t p = 1;
    પૂર્ણાંક endDim_ = endDim <= dims ? endDim : dims;
    માટે( પૂર્ણાંક i = startDim; i < endDim_; i++ )
        p *= size[i];
    પાછા p;
}

Mat::Mat(Mat&& m) CV_NOEXCEPT
    : flags(m.flags), dims(m.dims), rows(m.rows), cols(m.cols), data(m.data),
      datastart(m.datastart), dataend(m.dataend), datalimit(m.datalimit), allocator(m.allocator),
      u(m.u), size(m.size), step(m.step)
{
    m.flags = MAGIC_VAL; m.dims = m.rows = m.cols = 0;
    m.size.clear(); m.step.clear();
    m.data = ખાલી; m.datastart = ખાલી; m.dataend = ખાલી; m.datalimit = ખાલી;
    m.allocator = ખાલી;
    m.u = ખાલી;
}


Mat& Mat::operator = (Mat&& m)
{
    જો (આ == &m)
      પાછા *આ;

    release();
    flags = m.flags; dims = m.dims; rows = m.rows; cols = m.cols; data = m.data;
    size = m.size; step = m.step;
    datastart = m.datastart; dataend = m.dataend; datalimit = m.datalimit; allocator = m.allocator;
    u = m.u;
    m.flags = MAGIC_VAL; m.dims = m.rows = m.cols = 0;
    m.size.clear(); m.step.clear();
    m.data = ખાલી; m.datastart = ખાલી; m.dataend = ખાલી; m.datalimit = ખાલી;
    m.allocator = ખાલી;
    m.u = ખાલી;
    પાછા *આ;
}


શૂન્ય Mat::create(પૂર્ણાંક d0, અચલ પૂર્ણાંક* _sizes, પૂર્ણાંક _type)
{
    પૂર્ણાંક sz1 = 1, d = d0;
    પૂર્ણાંક i;
    જો (d == 0) {
        d = 1;
        _sizes = (અચલ પૂર્ણાંક*)&sz1;
    }
    CV_Assert(0 <= d && d <= CV_MAX_DIM && _sizes);
    _type = CV_MAT_TYPE(_type);

    જો( data && d == dims && _type == type() )
    {
        માટે( i = 0; i < d; i++ )
            જો( size[i] != _sizes[i] )
                તોડો;
        જો( i == d )
            પાછા;
    }

    પૂર્ણાંક _sizes_backup[CV_MAX_DIM]; // #5991
    જો (_sizes == (આ->size.p))
    {
        માટે(i = 0; i < d; i++ )
            _sizes_backup[i] = _sizes[i];
        _sizes = _sizes_backup;
    }

    release();
    જો( d == 0 )
        પાછા;
    flags = (_type & CV_MAT_TYPE_MASK) | MAGIC_VAL;
    setSize(*આ, d, _sizes, 0, સાચું);

    જો( total() > 0 )
    {
        MatAllocator *a = allocator, *a0 = getDefaultAllocator();
        જો(!a)
            a = a0;
        પ્રયાસ
        {
            u = a->allocate(dims, size.p, _type, 0, step.p, ACCESS_RW /* ignored */, USAGE_DEFAULT);
            CV_Assert(u != 0);
            allocator = a;
        }
        પકડો (...)
        {
            જો (a == a0)
                ફેંકો;
            u = a0->allocate(dims, size.p, _type, 0, step.p, ACCESS_RW /* ignored */, USAGE_DEFAULT);
            CV_Assert(u != 0);
            allocator = a0;
        }
        CV_Assert( step[dims-1] == (size_t)CV_ELEM_SIZE(flags) );
    }

    addref();
    finalizeHdr(*આ);
    size.dims = dims = d0;
}

શૂન્ય Mat::create(અચલ std::vector<પૂર્ણાંક>& _sizes, પૂર્ણાંક _type)
{
    create((પૂર્ણાંક)_sizes.size(), _sizes.data(), _type);
}

શૂન્ય Mat::create(અચલ MatShape& _shape, પૂર્ણાંક _type)
{
    જો (_shape.dims < 0) {
        release();
        પાછા;
    }
    create(_shape.dims, _shape.p, _type);
    size.layout = _shape.layout;
    size.C = _shape.C;
}

શૂન્ય Mat::create(std::initializer_list<પૂર્ણાંક> _shape, પૂર્ણાંક _type)
{
    પૂર્ણાંક new_shape[MatShape::MAX_DIMS];
    પૂર્ણાંક new_ndims = (પૂર્ણાંક)_shape.size();
    CV_Assert(new_ndims <= MatShape::MAX_DIMS);
    auto it = _shape.begin();
    માટે (પૂર્ણાંક i = 0; i < new_ndims; i++, ++it)
        new_shape[i] = *it;

    create(new_ndims, new_shape, _type);
}

શૂન્ય Mat::copySize(અચલ Mat& m)
{
    dims = m.dims;
    cols = m.cols;
    rows = m.rows;
    size = m.size;
    step = m.step;
}

શૂન્ય Mat::deallocate()
{
    જો(u)
    {
        UMatData* u_ = u;
        u = ખાલી;
        (u_->currAllocator ? u_->currAllocator : allocator ? allocator : getDefaultAllocator())->unmap(u_);
    }
}

Mat::Mat(અચલ Mat& m, અચલ Range& _rowRange, અચલ Range& _colRange)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    CV_Assert(m.dims <= MatShape::MAX_DIMS);
    CV_Assert(m.dims >= 2 ||
              (m.dims == 1 && (_rowRange == Range::all() ||
                               _rowRange == Range(0, m.rows))));
    જો( m.dims > 2 )
    {
        Range rs[MatShape::MAX_DIMS];
        rs[0] = _rowRange;
        rs[1] = _colRange;
        માટે( પૂર્ણાંક i = 2; i < m.dims; i++ )
            rs[i] = Range::all();
        *આ = m(rs);
        પાછા;
    }

    *આ = m;
    પ્રયાસ
    {
        જો( _rowRange != Range::all() && _rowRange != Range(0,rows) )
        {
            CV_Assert( 0 <= _rowRange.start && _rowRange.start <= _rowRange.end
                       && _rowRange.end <= m.rows );
            size[0] = rows = _rowRange.size();
            data += step*_rowRange.start;
            flags |= SUBMATRIX_FLAG;
        }

        જો( _colRange != Range::all() && _colRange != Range(0,cols) )
        {
            CV_Assert( 0 <= _colRange.start && _colRange.start <= _colRange.end
                       && _colRange.end <= m.cols );
            size[dims > 1] = cols = _colRange.size();
            data += _colRange.start*elemSize();
            flags |= SUBMATRIX_FLAG;
        }
    }
    પકડો(...)
    {
        release();
        ફેંકો;
    }

    updateContinuityFlag();

    જો( rows <= 0 || cols <= 0 )
    {
        release();
        rows = cols = 0;
    }
}


Mat::Mat(અચલ Mat& m, અચલ Rect& roi)
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
    જો( roi.width < m.cols || roi.height < m.rows )
        flags |= SUBMATRIX_FLAG;

    size[0] = rows;
    size[1] = cols;
    step[0] = m.step[0];
    step[1] = esz;
    updateContinuityFlag();

    addref();
    જો( rows <= 0 || cols <= 0 )
    {
        rows = cols = 0;
        release();
    }
}


Mat::Mat(પૂર્ણાંક _dims, અચલ પૂર્ણાંક* _sizes, પૂર્ણાંક _type, શૂન્ય* _data, અચલ size_t* _steps)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    flags |= CV_MAT_TYPE(_type);
    datastart = data = (uchar*)_data;
    જો (_dims > 0 || _data != nullptr)
        setSize(*આ, _dims, _sizes, _steps, સાચું);
    finalizeHdr(*આ);
}


Mat::Mat(અચલ std::vector<પૂર્ણાંક>& _sizes, પૂર્ણાંક _type, શૂન્ય* _data, અચલ size_t* _steps)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    flags |= CV_MAT_TYPE(_type);
    datastart = data = (uchar*)_data;
    પૂર્ણાંક _dims = (પૂર્ણાંક)_sizes.size();
    જો (_dims > 0 || _data != nullptr)
        setSize(*આ, _dims, _sizes.data(), _steps, સાચું);
    finalizeHdr(*આ);
}

Mat::Mat(અચલ MatShape& _shape, પૂર્ણાંક _type, શૂન્ય* _data, અચલ size_t* _steps)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    flags |= CV_MAT_TYPE(_type);
    datastart = data = (uchar*)_data;
    જો (_shape.dims >= 0) {
        setSize(*આ, _shape.dims, _shape.p, _steps, સાચું);
    }
    નહીંતર {
        CV_Assert(!data);
    }
    finalizeHdr(*આ);
}

Mat::Mat(std::initializer_list<પૂર્ણાંક> _shape, પૂર્ણાંક _type, શૂન્ય* _data, અચલ size_t* _steps)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    પૂર્ણાંક new_shape[MatShape::MAX_DIMS];
    પૂર્ણાંક _dims = (પૂર્ણાંક)_shape.size();
    CV_Assert(_dims <= MatShape::MAX_DIMS);
    auto it = _shape.begin();
    માટે (પૂર્ણાંક i = 0; i < _dims; i++, ++it)
        new_shape[i] = *it;

    flags |= CV_MAT_TYPE(_type);
    datastart = data = (uchar*)_data;
    જો (_dims > 0 || _data != nullptr)
        setSize(*આ, _dims, new_shape, _steps, સાચું);
    finalizeHdr(*આ);
}

Mat::Mat(અચલ Mat& m, અચલ Range* ranges)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    પૂર્ણાંક d = m.dims;

    CV_Assert(ranges);
    માટે( પૂર્ણાંક i = 0; i < d; i++ )
    {
        Range r = ranges[i];
        CV_Assert( r == Range::all() || (0 <= r.start && r.start < r.end && r.end <= m.size[i]) );
    }
    *આ = m;
    માટે( પૂર્ણાંક i = 0; i < d; i++ )
    {
        Range r = ranges[i];
        જો( r != Range::all() && r != Range(0, size.p[i]))
        {
            size.p[i] = r.end - r.start;
            data += r.start*step.p[i];
            flags |= SUBMATRIX_FLAG;
        }
    }

    જો (d <= 2) {
        rows = d == 2 ? size[0] : 1;
        cols = d <= 0 ? (d >= 0) : size[d > 1];
    }
    updateContinuityFlag();
}

Mat::Mat(અચલ Mat& m, અચલ std::vector<Range>& ranges)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
    datalimit(0), allocator(0), u(0)
{
    પૂર્ણાંક d = m.dims;

    CV_Assert((પૂર્ણાંક)ranges.size() == d);
    માટે (પૂર્ણાંક i = 0; i < d; i++)
    {
        Range r = ranges[i];
        CV_Assert(r == Range::all() || (0 <= r.start && r.start < r.end && r.end <= m.size[i]));
    }
    *આ = m;
    માટે (પૂર્ણાંક i = 0; i < d; i++)
    {
        Range r = ranges[i];
        જો (r != Range::all() && r != Range(0, size.p[i]))
        {
            size.p[i] = r.end - r.start;
            data += r.start*step.p[i];
            flags |= SUBMATRIX_FLAG;
        }
    }

    જો (d <= 2) {
        rows = d == 2 ? size[0] : 1;
        cols = d <= 0 ? (d >= 0) : size[d > 1];
    }
    updateContinuityFlag();
}


Mat Mat::diag(પૂર્ણાંક d) અચલ
{
    CV_Assert( dims == 2 );
    Mat m = *આ;
    size_t esz = elemSize();
    પૂર્ણાંક len;

    જો( d >= 0 )
    {
        len = std::min(cols - d, rows);
        m.data += esz*d;
    }
    નહીંતર
    {
        len = std::min(rows + d, cols);
        m.data -= step[0]*d;
    }
    CV_DbgAssert( len > 0 );

    m.size[0] = m.rows = len;
    m.size[1] = m.cols = 1;
    m.step[0] += (len > 1 ? esz : 0);

    m.updateContinuityFlag();

    જો( size() != Size(1,1) )
        m.flags |= SUBMATRIX_FLAG;

    પાછા m;
}


શૂન્ય Mat::pop_back(size_t nelems)
{
    CV_Assert( nelems <= (size_t)size.p[0] );
    CV_Assert( dims >= 1 );

    જો( isSubmatrix() )
        *આ = rowRange(0, size.p[0] - (પૂર્ણાંક)nelems);
    નહીંતર
    {
        size.p[0] -= (પૂર્ણાંક)nelems;
        જો (dims == 2)
            rows = size.p[0];
        નહીંતર જો (dims == 1)
            cols = size.p[0];
        dataend -= nelems*step.p[0];
    }
}


શૂન્ય Mat::ધકેલો_back(અચલ શૂન્ય* elem)
{
    size_t r = size.p[0];
    જો( isSubmatrix() || dataend + step.p[0] > datalimit )
        reserve( std::max(r + 1, (r*3+1)/2) );

    size_t esz = elemSize();
    memcpy(data + r*step.p[0], elem, esz);
    size.p[0] = પૂર્ણાંક(r + 1);
    જો (dims == 2)
        rows = size.p[0];
    નહીંતર જો (dims == 1)
        cols = size.p[0];
    dataend += step.p[0];
    uint64 tsz = size.p[0];
    માટે( પૂર્ણાંક i = 1; i < dims; i++ )
        tsz *= size.p[i];
    જો( esz < step.p[0] || tsz != (uint64)(પૂર્ણાંક)tsz )
        flags &= ~CONTINUOUS_FLAG;
}


શૂન્ય Mat::reserve(size_t nelems)
{
    અચલ size_t MIN_SIZE = 64;

    CV_Assert( (પૂર્ણાંક)nelems >= 0 );
    જો( !isSubmatrix() && step.p[0] != 0 && data + step.p[0]*nelems <= datalimit )
        પાછા;

    પૂર્ણાંક r = size.p[0];
    જો( (size_t)r >= nelems)
        પાછા;

    MatShape newsize = size;
    size_t esz = elemSize();
    newsize.dims = std::max(newsize.dims, 1);
    જો (dims > 1 && r == 0 && step.p[0] == 0) {
        step.p[0] = size.p[1] * (dims == 2 ? esz : step.p[1]);
    }
    newsize.p[0] = (પૂર્ણાંક)nelems;
    size_t newbytes = newsize.total() * esz;
    જો (newbytes < MIN_SIZE) {
        newsize.p[0] = 1;
        newsize.p[0] = પૂર્ણાંક(MIN_SIZE / newsize.total() / esz);
    }

    Mat m(newsize, type());
    જો( r > 0)
    {
        Mat mpart = m.rowRange(0, r);
        copyTo(mpart);
    }

    *આ = m;
    size.p[0] = r;

    જો (dims == 2)
        rows = size.p[0];
    નહીંતર જો (dims == 1)
        cols = size.p[0];
    dataend = data + step.p[0]*r;
}


શૂન્ય Mat::reserveBuffer(size_t nbytes)
{
    size_t esz = 1;
    પૂર્ણાંક mtype = CV_8UC1;
    જો (!empty())
    {
        જો (!isSubmatrix() && data + nbytes <= dataend)//Should it be datalimit?
            પાછા;
        esz = elemSize();
        mtype = type();
    }

    size_t nelems = (nbytes - 1) / esz + 1;

#જો SIZE_MAX > UINT_MAX
    CV_Assert(nelems <= size_t(INT_MAX)*size_t(INT_MAX));
    પૂર્ણાંક newrows = nelems > size_t(INT_MAX) ? nelems > 0x400*size_t(INT_MAX) ? nelems > 0x100000 * size_t(INT_MAX) ? nelems > 0x40000000 * size_t(INT_MAX) ?
                  size_t(INT_MAX) : 0x40000000 : 0x100000 : 0x400 : 1;
#નહીંતર
    પૂર્ણાંક newrows = nelems > size_t(INT_MAX) ? 2 : 1;
#endif
    પૂર્ણાંક newcols = (પૂર્ણાંક)((nelems - 1) / newrows + 1);

    create(newrows, newcols, mtype);
}


શૂન્ય Mat::resize(size_t nelems)
{
    પૂર્ણાંક r = size.p[0];
    જો( r == (પૂર્ણાંક)nelems )
        પાછા;
    CV_Assert( (પૂર્ણાંક)nelems >= 0 );

    જો( isSubmatrix() || data + step.p[0]*nelems > datalimit )
        reserve(nelems);

    size.p[0] = (પૂર્ણાંક)nelems;
    જો (dims == 2)
        rows = size.p[0];
    નહીંતર જો (dims == 1)
        cols = size.p[0];
    dataend += (size.p[0] - r)*(int64_t)step.p[0];

    //updateContinuityFlag(*this);
}


શૂન્ય Mat::resize(size_t nelems, અચલ Scalar& s)
{
    પૂર્ણાંક saveRows = size.p[0];
    resize(nelems);

    જો( size.p[0] > saveRows )
    {
        Mat part = rowRange(saveRows, size.p[0]);
        part = s;
    }
}

શૂન્ય Mat::push_back(અચલ Mat& elems)
{
    size_t r = size.p[0];
    size_t delta = elems.size.p[0];
    જો( delta == 0 )
        પાછા;
    જો( આ == &elems )
    {
        Mat tmp = elems;
        push_back(tmp);
        પાછા;
    }
    જો( !data )
    {
        *આ = elems.clone();
        પાછા;
    }
    CV_Assert(dims > 0);

    size.p[0] = elems.size.p[0];
    તાર્કિક eq = size == elems.size;
    size.p[0] = પૂર્ણાંક(r);
    જો( !eq )
        CV_Error(cv::Error::StsUnmatchedSizes, "Pushed vector length is not equal to matrix row length");
    જો( type() != elems.type() )
        CV_Error(cv::Error::StsUnmatchedFormats, "Pushed vector type is not the same as matrix type");
    size_t esz = elemSize();
    size_t minstep = dims <= 1 ? esz : size.p[1] * (dims == 2 ? esz : step.p[1]);
    size_t step0 = step.p[0];
    જો (step0 < minstep) {
        જો (size.p[0] <= 1)
            step.p[0] = step0 = minstep;
    }

    જો( isSubmatrix() || dataend + step0*delta > datalimit )
        reserve( std::max(r + delta, (r*3+1)/2) );

    size.p[0] += પૂર્ણાંક(delta);
    જો (dims == 2)
        rows = size.p[0];
    નહીંતર જો (dims == 1)
        cols = size.p[0];
    dataend += step0*delta;

    //updateContinuityFlag(*this);

    જો( isContinuous() && elems.isContinuous() )
        memcpy(data + r*step0, elems.data, elems.total()*elems.elemSize());
    નહીંતર
    {
        Mat part = rowRange(પૂર્ણાંક(r), પૂર્ણાંક(r + delta));
        elems.copyTo(part);
    }
}


શૂન્ય Mat::locateROI( Size& wholeSize, Point& ofs ) અચલ
{
    CV_Assert( dims <= 2 && step[0] > 0 );
    size_t esz = elemSize(), minstep;
    ptrdiff_t delta1 = data - datastart, delta2 = dataend - datastart;

    જો( delta1 == 0 )
        ofs.x = ofs.y = 0;
    નહીંતર
    {
        ofs.y = (પૂર્ણાંક)(delta1/step[0]);
        ofs.x = (પૂર્ણાંક)((delta1 - step[0]*ofs.y)/esz);
        CV_DbgAssert( data == datastart + ofs.y*step[0] + ofs.x*esz );
    }
    minstep = (ofs.x + cols)*esz;
    wholeSize.height = (પૂર્ણાંક)((delta2 - minstep)/step[0] + 1);
    wholeSize.height = std::max(wholeSize.height, ofs.y + rows);
    wholeSize.width = (પૂર્ણાંક)((delta2 - step*(wholeSize.height-1))/esz);
    wholeSize.width = std::max(wholeSize.width, ofs.x + cols);
}

Mat& Mat::adjustROI( પૂર્ણાંક dtop, પૂર્ણાંક dbottom, પૂર્ણાંક dleft, પૂર્ણાંક dright )
{
    CV_Assert( dims <= 2 && step[0] > 0 );
    Size wholeSize; Point ofs;
    size_t esz = elemSize();
    locateROI( wholeSize, ofs );
    પૂર્ણાંક row1 = std::min(std::max(ofs.y - dtop, 0), wholeSize.height), row2 = std::max(0, std::min(ofs.y + rows + dbottom, wholeSize.height));
    પૂર્ણાંક col1 = std::min(std::max(ofs.x - dleft, 0), wholeSize.width), col2 = std::max(0, std::min(ofs.x + cols + dright, wholeSize.width));
    જો(row1 > row2)
        std::swap(row1, row2);
    જો(col1 > col2)
        std::swap(col1, col2);

    જો (dims == 1) {
        data += (col1 - ofs.x)*(std::ptrdiff_t)esz;
        cols = col2 - col1;
        size.p[0] = cols;
    } નહીંતર {
        data += (row1 - ofs.y)*(std::ptrdiff_t)step + (col1 - ofs.x)*(std::ptrdiff_t)esz;
        rows = row2 - row1; cols = col2 - col1;
        size.p[0] = rows; size.p[1] = cols;
        updateContinuityFlag();
    }
    પાછા *આ;
}

Mat Mat::reshape(પૂર્ણાંક new_cn, પૂર્ણાંક new_rows) અચલ
{
    પૂર્ણાંક cn = channels();
    Mat hdr = *આ;

    જો( new_cn == 0 )
        new_cn = cn;

    જો( dims > 2 )
    {
        જો( new_rows == 0 )
        {
            // special case: just change the number of channnels; retain the same shape,
            // except for the last, innermost dimension
            CV_Assert(size[dims-1]*cn % new_cn == 0);
            hdr.flags = (hdr.flags & ~CV_MAT_CN_MASK) | ((new_cn-1) << CV_CN_SHIFT);
            hdr.step[dims-1] = CV_ELEM_SIZE(hdr.flags);
            hdr.size[dims-1] = hdr.size[dims-1]*cn / new_cn;
            પાછા hdr;
        }
        CV_Assert( new_rows > 0 );
        પૂર્ણાંક sz[] = { new_rows, (પૂર્ણાંક)(total()*cn/new_rows) };
        પાછા reshape(new_cn, 2, sz);
    }

    પૂર્ણાંક total_width = cols * cn;

    જો( (new_cn > total_width || total_width % new_cn != 0) && new_rows == 0 )
        new_rows = rows * total_width / new_cn;

    જો( new_rows != 0 && new_rows != rows )
    {
        પૂર્ણાંક total_size = total_width * rows;
        જો( !isContinuous() )
            CV_Error( cv::Error::BadStep,
            "The matrix is not continuous, thus its number of rows can not be changed" );

        total_width = total_size / new_rows;

        જો( total_width * new_rows != total_size )
            CV_Error( cv::Error::StsBadArg, "The total number of matrix elements "
                                    "is not divisible by the new number of rows" );

        hdr.size[0] = hdr.rows = new_rows;
        hdr.step[0] = total_width * elemSize1();
    } નહીંતર {
        hdr.size[0] = hdr.rows = rows;
        જો (dims <= 1)
            hdr.step[0] = cols * CV_ELEM_SIZE(flags);
    }

    પૂર્ણાંક new_width = total_width / new_cn;

    જો( new_width * new_cn != total_width )
        CV_Error( cv::Error::Bad_અંક_Channels,
        "The total width is not divisible by the new number of channels" );

    hdr.size.dims = hdr.dims = 2;
    hdr.size[1] = hdr.cols = new_width;
    hdr.flags = (hdr.flags & ~CV_MAT_CN_MASK) | ((new_cn-1) << CV_CN_SHIFT);
    hdr.step[1] = CV_ELEM_SIZE(hdr.flags);
    પાછા hdr;
}

Mat Mat::reshape(પૂર્ણાંક _cn, પૂર્ણાંક _newndims, અચલ પૂર્ણાંક* _newsz) અચલ
{
    જો(_newndims == dims)
    {
        જો(_newsz == 0)
            પાછા reshape(_cn);
        જો(_newndims == 2)
            પાછા reshape(_cn, _newsz[0]);
    }

    જો (isContinuous())
    {
        CV_Assert(_cn >= 0 && _newndims >= 0 && _newndims <= CV_MAX_DIM && (_newndims == 0 || _newsz != 0));

        જો (_cn == 0)
            _cn = આ->channels();
        નહીંતર
            CV_Assert(_cn <= CV_CN_MAX);

        size_t total_elem1_ref = આ->total() * આ->channels();
        size_t total_elem1 = _cn;

        AutoBuffer<પૂર્ણાંક, 4> newsz_buf( (size_t)_newndims );

        પૂર્ણાંક m1_idx = -1;

        માટે (પૂર્ણાંક i = 0; i < _newndims; i++)
        {
            જો (_newsz[i] >= 0) {
                જો (_newsz[i] == 0 && i < dims)
                    newsz_buf[i] = size.p[i];
                નહીંતર
                    newsz_buf[i] = _newsz[i];
                total_elem1 *= (size_t)newsz_buf[i];
            } નહીંતર {
                જો (m1_idx >= 0)
                    CV_Error(cv::Error::StsBadSize, "More than one '-1' occured in the new shape");
                m1_idx = i;
            }
        }

        જો (m1_idx >= 0) {
            જો (total_elem1 == 0) {
                CV_Assert(total_elem1_ref == 0);
                total_elem1 = 1;
            }
            CV_Assert(total_elem1_ref % total_elem1 == 0);
            newsz_buf[m1_idx] = (પૂર્ણાંક)(total_elem1_ref / total_elem1);
        } નહીંતર જો (total_elem1 != total_elem1_ref) {
            CV_Error(cv::Error::StsUnmatchedSizes, "Requested and source matrices have different count of elements");
        }

        Mat hdr = *આ;
        hdr.flags = (hdr.flags & ~CV_MAT_CN_MASK) | ((_cn-1) << CV_CN_SHIFT);
        setSize(hdr, _newndims, newsz_buf.data(), ખાલી, સાચું);

        પાછા hdr;
    }

    CV_Error(cv::Error::StsNotImplemented, "Reshaping of n-dimensional non-continuous matrices is not supported yet");
    // TBD
}

Mat Mat::reshape(પૂર્ણાંક _cn, અચલ std::vector<પૂર્ણાંક>& _newshape) અચલ
{
    પૂર્ણાંક newdims = (પૂર્ણાંક)_newshape.size();
    જો(newdims == 0 && empty())
    {
        પાછા *આ;
    }

    પાછા reshape(_cn, newdims, newdims > 0 ? &_newshape[0] : 0);
}

Mat Mat::reshape(પૂર્ણાંક _cn, અચલ MatShape& _newshape) અચલ
{
    જો (_newshape.dims < 0) {
        પૂર્ણાંક newshape[] = {0};
        પાછા reshape(_cn, 1, newshape);
    }
    પાછા reshape(_cn, _newshape.dims, _newshape.p);
}

Mat Mat::reshape(પૂર્ણાંક _cn, std::initializer_list<પૂર્ણાંક> newshape_) અચલ
{
    પૂર્ણાંક newshape[MatShape::MAX_DIMS];
    size_t i, newshape_dims = newshape_.size();
    CV_Assert(newshape_dims <= (size_t)MatShape::MAX_DIMS);
    auto it = newshape_.begin();
    માટે (i = 0; i < newshape_dims; i++, ++it)
        newshape[i] = *it;
    પાછા reshape(_cn, (પૂર્ણાંક)newshape_dims, newshape);
}

Mat Mat::reinterpret(પૂર્ણાંક type) અચલ
{
    type = CV_MAT_TYPE(type);
    CV_Assert(CV_ELEM_SIZE(આ->type()) == CV_ELEM_SIZE(type));
    Mat m = *આ;
    m.flags = (m.flags & ~CV_MAT_TYPE_MASK) | type;
    m.updateContinuityFlag();
    પાછા m;
}

Mat Mat::diag(અચલ Mat& d)
{
    CV_Assert( d.cols == 1 || d.rows == 1 );
    પૂર્ણાંક len = d.rows + d.cols - 1;
    Mat m(len, len, d.type(), Scalar(0));
    Mat md = m.diag();
    જો( d.cols == 1 )
        d.copyTo(md);
    નહીંતર
        transpose(d, md);
    પાછા m;
}

પૂર્ણાંક Mat::checkVector(પૂર્ણાંક _elemChannels, પૂર્ણાંક _depth, તાર્કિક _requireContinuous) અચલ
{
    પાછા data && (depth() == _depth || _depth <= 0) &&
        (isContinuous() || !_requireContinuous) &&
        ((dims <= 2 && (((rows == 1 || cols == 1) && channels() == _elemChannels) ||
                        (cols == _elemChannels && channels() == 1))) ||
        (dims == 3 && channels() == 1 && size.p[2] == _elemChannels && (size.p[0] == 1 || size.p[1] == 1) &&
         (isContinuous() || step.p[1] == step.p[2]*size.p[2])))
    ? (પૂર્ણાંક)(total()*channels()/_elemChannels) : -1;
}


સ્થિર અંતઃરેખ Size getContinuousSize_(પૂર્ણાંક flags, પૂર્ણાંક cols, પૂર્ણાંક rows, પૂર્ણાંક widthScale)
{
    int64 sz = (int64)cols * rows * widthScale;
    તાર્કિક has_int_overflow = sz >= INT_MAX;
    તાર્કિક isContiguous = (flags & Mat::CONTINUOUS_FLAG) != 0;
    પાછા (isContiguous && !has_int_overflow)
            ? Size((પૂર્ણાંક)sz, 1)
            : Size(cols * widthScale, rows);
}

Size getContinuousSize2D(Mat& m1, પૂર્ણાંક widthScale)
{
    CV_CheckLE(m1.dims, 2, "");
    પાછા getContinuousSize_(m1.flags,
                              m1.cols, m1.rows, widthScale);
}
Size getContinuousSize2D(Mat& m1, Mat& m2, પૂર્ણાંક widthScale)
{
    CV_CheckLE(m1.dims, 2, "");
    CV_CheckLE(m2.dims, 2, "");
    અચલ Size sz1 = m1.size();
    જો (sz1 != m2.size())  // reshape all matrixes to the same size (#4159)
    {
        size_t total_sz = m1.total();
        CV_CheckEQ(total_sz, m2.total(), "");
        તાર્કિક is_m1_vector = m1.cols == 1 || m1.rows == 1;
        તાર્કિક is_m2_vector = m2.cols == 1 || m2.rows == 1;
        CV_Assert(is_m1_vector); CV_Assert(is_m2_vector);
        પૂર્ણાંક total = (પૂર્ણાંક)total_sz;  // vector-column
        તાર્કિક isContiguous = ((m1.flags & m2.flags) & Mat::CONTINUOUS_FLAG) != 0;
        તાર્કિક has_int_overflow = ((int64)total_sz * widthScale) >= INT_MAX;
        જો (isContiguous && !has_int_overflow)
            total = 1; // vector-row
        m1 = m1.reshape(0, total);
        m2 = m2.reshape(0, total);
        CV_Assert(m1.cols == m2.cols && m1.rows == m2.rows);
        પાછા Size(m1.cols * widthScale, m1.rows);
    }
    પાછા getContinuousSize_(m1.flags & m2.flags,
                              m1.cols, m1.rows, widthScale);
}

Size getContinuousSize2D(Mat& m1, Mat& m2, Mat& m3, પૂર્ણાંક widthScale)
{
    CV_CheckLE(m1.dims, 2, "");
    CV_CheckLE(m2.dims, 2, "");
    CV_CheckLE(m3.dims, 2, "");
    અચલ Size sz1 = m1.size();
    જો (sz1 != m2.size() || sz1 != m3.size())  // reshape all matrixes to the same size (#4159)
    {
        size_t total_sz = m1.total();
        CV_CheckEQ(total_sz, m2.total(), "");
        CV_CheckEQ(total_sz, m3.total(), "");
        તાર્કિક is_m1_vector = m1.cols == 1 || m1.rows == 1;
        તાર્કિક is_m2_vector = m2.cols == 1 || m2.rows == 1;
        તાર્કિક is_m3_vector = m3.cols == 1 || m3.rows == 1;
        CV_Assert(is_m1_vector); CV_Assert(is_m2_vector); CV_Assert(is_m3_vector);
        પૂર્ણાંક total = (પૂર્ણાંક)total_sz;  // vector-column
        તાર્કિક isContiguous = ((m1.flags & m2.flags & m3.flags) & Mat::CONTINUOUS_FLAG) != 0;
        તાર્કિક has_int_overflow = ((int64)total_sz * widthScale) >= INT_MAX;
        જો (isContiguous && !has_int_overflow)
            total = 1; // vector-row
        m1 = m1.reshape(0, total);
        m2 = m2.reshape(0, total);
        m3 = m3.reshape(0, total);
        CV_Assert(m1.cols == m2.cols && m1.rows == m2.rows && m1.cols == m3.cols && m1.rows == m3.rows);
        પાછા Size(m1.cols * widthScale, m1.rows);
    }
    પાછા getContinuousSize_(m1.flags & m2.flags & m3.flags,
                              m1.cols, m1.rows, widthScale);
}

} // cv::
