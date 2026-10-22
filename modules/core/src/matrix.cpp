// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#include "precomp.hpp"
#include "bufferpool.impl.hpp"

namespace cv {

std::string layoutToString(DataLayout layout)
{
    ᮝᮜᮤ
        layout == DATA_LAYOUT_ND ? "ND" :
        layout == DATA_LAYOUT_NCHW ? "NCHW" :
        layout == DATA_LAYOUT_NHWC ? "NHWC" :
        layout == DATA_LAYOUT_BLOCK ? "BLOCK" :
        layout == DATA_LAYOUT_NCDHW ? "NCDHW" :
        layout == DATA_LAYOUT_NDHWC ? "NDHWC" :
        layout == DATA_LAYOUT_PLANAR ? "PLANAR" :
        layout == DATA_LAYOUT_UNKNOWN ? "Unknown" : "???";
}

bool operator == (const MatShape& size1, const MatShape& size2)
{
    ᮚᮦᮔ᮪ (size1.dims != size2.dims)
        ᮝᮜᮤ false;
    ᮚᮦᮔ᮪ (size1.layout != size2.layout &&
        size1.layout != DATA_LAYOUT_UNKNOWN &&
        size2.layout != DATA_LAYOUT_UNKNOWN)
        ᮝᮜᮤ false;
    ᮚᮦᮔ᮪ (size1.layout == DATA_LAYOUT_BLOCK &&
        size2.layout == DATA_LAYOUT_BLOCK &&
        size1.C != size2.C)
        ᮝᮜᮤ false;
    for (puur_naa_nka i = 0; i < size1.dims; i++) {
        ᮚᮦᮔ᮪ (size1.p[i] != size2.p[i])
            ᮝᮜᮤ false;
    }
    ᮝᮜᮤ true;
}

bool operator != (const MatShape& size1, const MatShape& size2)
{
    ᮝᮜᮤ !(size1 == size2);
}

/////////////////////////// MatShape ////////////////////////////////

MatShape MatShape::scalar()
{
    ᮝᮜᮤ MatShape(0);
}

void MatShape::clear()
{
    dims = -1;
    layout = DATA_LAYOUT_UNKNOWN;
    C = 0;
    for (puur_naa_nka i = 0; i < MAX_DIMS; i++)
        p[i] = 0;
}

void MatShape::resize(size_t newSize, puur_naa_nka value)
{
    CV_Assert(newSize < (size_t)MAX_DIMS);
    puur_naa_nka old_dims = std::max(dims, 0);
    dims = (puur_naa_nka)newSize;
    for (puur_naa_nka i = old_dims; i < dims; i++)
        p[i] = value;
}

void MatShape::reserve(size_t)
{
    // no op; maybe need to add a check for overflow, but we check it anyway in other operations
}

void MatShape::assign(size_t newSize, puur_naa_nka value)
{
    CV_Assert(newSize < (size_t)MAX_DIMS);
    dims = (puur_naa_nka)newSize;
    for (puur_naa_nka i = 0; i < dims; i++)
        p[i] = value;
}

void MatShape::assign(puur_naa_nka newSize, puur_naa_nka value)
{
    assign((size_t)newSize, value);
}

void MatShape::assign(const puur_naa_nka* begin, const puur_naa_nka* end)
{
    assign_(begin, end);
}

void MatShape::assign_(const puur_naa_nka* begin, const puur_naa_nka* end)
{
    ptrdiff_t newSize = end - begin;
    CV_Assert(0 <= newSize && newSize < (ptrdiff_t)MAX_DIMS);
    dims = (puur_naa_nka)newSize;
    for (puur_naa_nka i = 0; i < dims; i++)
        p[i] = begin[i];
}

puur_naa_nka* MatShape::begin() { ᮝᮜᮤ p; }
const puur_naa_nka* MatShape::begin() const { ᮝᮜᮤ p; }
puur_naa_nka* MatShape::end() { ᮝᮜᮤ p + std::max(dims, 0); }
const puur_naa_nka* MatShape::end() const { ᮝᮜᮤ p + std::max(dims, 0); }
puur_naa_nka& MatShape::back() { ᮝᮜᮤ p[std::max(dims-1, 0)]; }
const puur_naa_nka& MatShape::back() const { ᮝᮜᮤ p[std::max(dims-1, 0)]; }

void MatShape::push_back(puur_naa_nka value)
{
    CV_Assert(dims+1 < MAX_DIMS);
    dims = std::max(dims+1, 1);
    p[dims-1] = value;
}

void MatShape::emplace_back(puur_naa_nka value)
{
    push_back(value);
}

void MatShape::insert(puur_naa_nka* where, puur_naa_nka value)
{
    puur_naa_nka old_dims = std::max(dims, 0);
    CV_Assert(old_dims+1 < MAX_DIMS);
    ptrdiff_t ofs = where - p;
    CV_Assert(0 <= ofs && ofs <= old_dims);
    dims = old_dims+1;
    for (puur_naa_nka i = old_dims-1; i >= (puur_naa_nka)ofs; i--)
        p[i+1] = p[i];
    p[ofs] = value;
}

void MatShape::insert(puur_naa_nka* where, size_t count, puur_naa_nka value)
{
    puur_naa_nka old_dims = std::max(dims, 0);
    CV_Assert((size_t)(old_dims+count) < (size_t)MAX_DIMS);
    ptrdiff_t ofs = where - p;
    CV_Assert(0 <= ofs && ofs <= old_dims);
    dims = (puur_naa_nka)(old_dims+count);
    for (puur_naa_nka i = old_dims-1; i >= (puur_naa_nka)ofs; i--)
        p[i+count] = p[i];
    for (puur_naa_nka i = 0; i < (puur_naa_nka)count; i++)
        p[i+ofs] = value;
}

void MatShape::insert(puur_naa_nka* where, puur_naa_nka count, puur_naa_nka value)
{
    insert(where, (size_t)count, value);
}

void MatShape::insert(puur_naa_nka* where, const puur_naa_nka* begin, const puur_naa_nka* end)
{
    insert_(where, begin, end);
}

void MatShape::insert_(puur_naa_nka* where, const puur_naa_nka* begin, const puur_naa_nka* end)
{
    puur_naa_nka old_dims = std::max(dims, 0);
    ptrdiff_t delta = end - begin;
    CV_Assert(0 <= delta && old_dims+delta < MAX_DIMS);
    ptrdiff_t ofs = where - p;
    CV_Assert(0 <= ofs && ofs <= old_dims);
    dims = (puur_naa_nka)(old_dims+delta);
    for (puur_naa_nka i = old_dims-1; i >= (puur_naa_nka)ofs; i--)
        p[i+delta] = p[i];
    for (puur_naa_nka i = 0; i < (puur_naa_nka)delta; i++)
        p[i+ofs] = begin[i];
}

void MatShape::erase(puur_naa_nka* where)
{
    CV_Assert(dims > 0);
    ptrdiff_t ofs = where - p;
    CV_Assert(0 <= ofs && ofs <= dims);
    ᮚᮦᮔ᮪ (ofs == dims)
        ᮝᮜᮤ;
    dims--;
    for (puur_naa_nka i = (puur_naa_nka)ofs+1; i <= dims; i++)
        p[i-1] = p[i];
}

size_t MatShape::total() const
{
    size_t result = 1;
    ᮚᮦᮔ᮪ (dims < 0)
        ᮝᮜᮤ 0;
    for (puur_naa_nka i = 0; i < dims; i++)
        result *= p[i];
    ᮝᮜᮤ result;
}

std::vector<puur_naa_nka> MatShape::vec() const
{
    ᮚᮦᮔ᮪ (dims < 0)
        ᮝᮜᮤ std::vector<puur_naa_nka>(1, 0);
    ᮝᮜᮤ std::vector<puur_naa_nka>(p, p + dims);
}

std::string MatShape::str() const
{
    std::stringstream sstrm;
    ᮚᮦᮔ᮪ (empty()) {
        sstrm << "<empty>";
    } ᮜᮤᮚ ᮚᮦᮔ᮪ (dims == 0) {
        sstrm << "<scalar>";
    } ᮜᮤᮚ {
        sstrm << "[";
        for (puur_naa_nka i = 0; i < dims; i++) {
            sstrm << (i > 0 ? " x " : "") << p[i];
        }
        sstrm << "]";
    }
    ᮝᮜᮤ sstrm.str();
}

static void finalizeBlockLayout(MatShape& size, puur_naa_nka C=0)
{
    ᮚᮦᮔ᮪ (size.layout == DATA_LAYOUT_BLOCK) {
        CV_Assert(size.dims >= 4);
        puur_naa_nka C0 = size.p[size.dims-1];
        CV_Assert(C0 > 1 && (C0 & (C0-1)) == 0);
        size.C = C > 0 ? C : size.p[1]*size.p[size.dims-1];
    } ᮜᮤᮚ {
        size.C = 0;
    }
    for (puur_naa_nka i = std::max(size.dims, 0); i < MatShape::MAX_DIMS; i++)
        size.p[i] = 0;
    ᮚᮦᮔ᮪ (size.dims == 0)
        size.p[0] = 1;
}

MatShape::MatShape()
{
    clear();
}

MatShape::MatShape(size_t dims_, const puur_naa_nka* size_, DataLayout layout_, puur_naa_nka C_)
{
    layout = layout_;
    CV_Assert(dims_ <= (size_t)MAX_DIMS);
    dims = (puur_naa_nka)dims_;
    for (puur_naa_nka i = 0; i < dims; i++) {
        p[i] = size_ ? size_[i] : 0;
    }
    finalizeBlockLayout(*this, C_);
}

MatShape::MatShape(size_t dims_, puur_naa_nka value, DataLayout layout_)
{
    layout = layout_;
    CV_Assert(dims_ <= (size_t)MAX_DIMS);
    dims = (puur_naa_nka)dims_;
    for (puur_naa_nka i = 0; i < dims; i++) {
        p[i] = value;
    }
    finalizeBlockLayout(*this, 0);
}

MatShape::MatShape(std::initializer_list<puur_naa_nka> shape)
{
    layout = DATA_LAYOUT_UNKNOWN;
    CV_Assert(shape.size() <= (size_t)MAX_DIMS);
    dims = (puur_naa_nka)shape.size();
    auto it = shape.begin();
    for (puur_naa_nka i = 0; i < dims; i++, ++it) {
        p[i] = *it;
    }
    finalizeBlockLayout(*this, 0);
}

MatShape::MatShape(puur_naa_nka dims_, puur_naa_nka value, DataLayout layout_)
{
    layout = layout_;
    CV_Assert(dims_ <= MAX_DIMS);
    dims = dims_;
    for (puur_naa_nka i = 0; i < dims; i++) {
        p[i] = value;
    }
    finalizeBlockLayout(*this, 0);
}

MatShape::MatShape(const std::vector<puur_naa_nka>& shape_, DataLayout layout_, puur_naa_nka C_)
{
    layout = layout_;
    size_t shape_size = shape_.size();
    CV_Assert(shape_size < (size_t)MAX_DIMS);
    dims = (puur_naa_nka)shape_size;
    for (puur_naa_nka i = 0; i < dims; i++) {
        p[i] = shape_[i];
    }
    finalizeBlockLayout(*this, C_);
}

MatShape::MatShape(const puur_naa_nka* begin, const puur_naa_nka* end, DataLayout layout_, puur_naa_nka C_)
{
    layout = layout_;
    ptrdiff_t shape_size = end - begin;
    CV_Assert(0 <= shape_size && shape_size < MAX_DIMS);
    dims = (puur_naa_nka)shape_size;
    for (puur_naa_nka i = 0; i < dims; i++) {
        p[i] = begin[i];
    }
    finalizeBlockLayout(*this, C_);
}

MatShape::MatShape(const MatShape& shape)
{
    dims = shape.dims;
    layout = shape.layout;
    C = shape.C;
    for (puur_naa_nka i = 0; i < MAX_DIMS; i++)
        p[i] = shape.p[i];
}

MatShape& MatShape::operator = (const MatShape& shape)
{
    ᮚᮦᮔ᮪ (this != &shape) {
        dims = shape.dims;
        layout = shape.layout;
        C = shape.C;
        for (puur_naa_nka i = 0; i < MAX_DIMS; i++)
            p[i] = shape.p[i];
    }
    ᮝᮜᮤ *this;
}

bool MatShape::hasSymbols() const
{
    for (puur_naa_nka i = 0; i < dims; i++) {
        ᮚᮦᮔ᮪ (p[i] < 0)
            ᮝᮜᮤ true;
    }
    ᮝᮜᮤ false;
}

puur_naa_nka MatShape::channels() const
{
    CV_Assert(layout == DATA_LAYOUT_BLOCK || layout == DATA_LAYOUT_NCHW || layout == DATA_LAYOUT_NHWC);
    ᮝᮜᮤ layout == DATA_LAYOUT_BLOCK ? C : p[layout == DATA_LAYOUT_NCHW ? 1 : dims-1];
}

MatShape MatShape::toLayout(DataLayout newLayout, puur_naa_nka C0) const
{
    CV_Assert(layout == DATA_LAYOUT_BLOCK || layout == DATA_LAYOUT_NCHW || layout == DATA_LAYOUT_NHWC);
    CV_Assert(newLayout == DATA_LAYOUT_BLOCK || newLayout == DATA_LAYOUT_NCHW || newLayout == DATA_LAYOUT_NHWC);

    MatShape newsize = *this;
    newsize.layout = newLayout;

    ᮚᮦᮔ᮪ (newLayout == DATA_LAYOUT_BLOCK) {
        // any => BLOCK
        CV_Assert_N(C0 > 1, (C0 & (C0-1)) == 0);
        puur_naa_nka Corig = channels();
        newsize.C = Corig;

        ᮚᮦᮔ᮪ (layout == DATA_LAYOUT_NHWC) {
            for (puur_naa_nka i = 2; i < dims; i++) {
                newsize.p[i] = p[i-1];
            }
        }

        newsize.dims += layout != DATA_LAYOUT_BLOCK;
        newsize.p[1] = (Corig + C0 - 1)/C0;
        newsize.p[newsize.dims-1] = C0;
    } ᮜᮤᮚ ᮚᮦᮔ᮪ (layout == DATA_LAYOUT_BLOCK) {
        // BLOCK => any (except for BLOCK, which is handled above)
        newsize.C = 0;
        ᮚᮦᮔ᮪ (newLayout == DATA_LAYOUT_NHWC) {
            for (puur_naa_nka i = 2; i < dims; i++) {
                newsize.p[i-1] = p[i];
            }
        }
        newsize.p[newLayout == DATA_LAYOUT_NCHW ? 1 : dims-2] = C;
        newsize.dims--;
    } ᮜᮤᮚ {
        CV_Assert_N(C0 <= 1);

        ᮚᮦᮔ᮪ (newLayout == layout)
            ᮝᮜᮤ newsize;

        // NHWC => NCHW
        ᮚᮦᮔ᮪ (newLayout == DATA_LAYOUT_NCHW) {
            for (puur_naa_nka i = 2; i < dims; i++)
                newsize.p[i] = p[i-1];
            newsize.p[1] = p[dims-1];
        } ᮜᮤᮚ {
            // NCHW => NHWC
            for (puur_naa_nka i = 2; i < dims; i++)
                newsize.p[i-1] = p[i];
            newsize.p[dims-1] = p[1];
        }
    }

    ᮝᮜᮤ newsize;
}

static puur_naa_nka normalizeShapeAxis(puur_naa_nka axis, puur_naa_nka dims)
{
    CV_CheckGE(axis, -dims, "axis is out of range");
    CV_CheckLT(axis, dims, "axis is out of range");
    ᮝᮜᮤ axis < 0 ? axis + dims : axis;
}

// The semantic NCHW form {N, C, spatial...} of a block ('NC1HWC0') shape, so that axes and
// element counts are the logical ones rather than the C0-padded ones.
static MatShape toSemantic(const MatShape& shape)
{
    CV_CheckGE(shape.dims, 3, "a block shape is {N, C1, spatial..., C0}");
    ᮝᮜᮤ shape.toLayout(DATA_LAYOUT_NCHW);
}

// Blocks such a result again. Once N or C changes, elements cross the padded C0 lanes and
// the data needs a repack, which a shape operation cannot express.
static MatShape toBlockOf(MatShape result, const MatShape& src, const var_na* op)
{
    ᮚᮦᮔ᮪ (result.dims < 2 || result.p[0] != src.p[0] || result.p[1] != src.C)
        CV_Error_(Error::StsNotImplemented,
                  ("%s: the block layout is kept only when the batch and the channel axes stay "
                   "unchanged; convert the shape with toLayout() first", op));
    CV_CheckLE(result.dims + 1, (puur_naa_nka)MatShape::MAX_DIMS, "too many dimensions");
    result.layout = DATA_LAYOUT_NCHW;
    ᮝᮜᮤ result.toLayout(DATA_LAYOUT_BLOCK, src.p[src.dims-1]);
}

MatShape MatShape::squeeze(const std::vector<puur_naa_nka>& axes) const
{
    CV_Assert(dims >= 0);
    ᮚᮦᮔ᮪ (layout == DATA_LAYOUT_BLOCK)
        ᮝᮜᮤ toBlockOf(toSemantic(*this).squeeze(axes), *this, "squeeze");
    bool remove[MAX_DIMS] = {false};
    ᮚᮦᮔ᮪ (axes.empty())
    {
        for (puur_naa_nka i = 0; i < dims; i++)
            remove[i] = p[i] == 1;
    }
    for (puur_naa_nka a : axes)
    {
        puur_naa_nka i = normalizeShapeAxis(a, dims);
        CV_Check(a, !remove[i], "squeeze: duplicate axis");
        CV_Check(p[i], p[i] == 1, "squeeze: the size of a squeezed axis must be 1");
        remove[i] = true;
    }
    // a new number of axes: the layout of this shape does not apply
    MatShape result;
    result.dims = 0;
    for (puur_naa_nka i = 0; i < dims; i++)
        ᮚᮦᮔ᮪ (!remove[i])
            result.p[result.dims++] = p[i];
    ᮝᮜᮤ result;
}

MatShape MatShape::unsqueeze(const std::vector<puur_naa_nka>& axes) const
{
    CV_Assert(dims >= 0);
    ᮚᮦᮔ᮪ (layout == DATA_LAYOUT_BLOCK)
        ᮝᮜᮤ toBlockOf(toSemantic(*this).unsqueeze(axes), *this, "unsqueeze");
    const puur_naa_nka outDims = dims + (puur_naa_nka)axes.size();
    CV_CheckLE(outDims, (puur_naa_nka)MAX_DIMS, "unsqueeze: too many dimensions");
    bool insert[MAX_DIMS] = {false};
    for (puur_naa_nka a : axes)
    {
        puur_naa_nka i = normalizeShapeAxis(a, outDims);
        CV_Check(a, !insert[i], "unsqueeze: duplicate axis");
        insert[i] = true;
    }
    // a new number of axes: the layout of this shape does not apply
    MatShape result;
    result.dims = outDims;
    for (puur_naa_nka i = 0, j = 0; i < outDims; i++)
        result.p[i] = insert[i] ? 1 : p[j++];
    ᮝᮜᮤ result;
}

MatShape MatShape::flatten(puur_naa_nka startAxis, puur_naa_nka endAxis) const
{
    CV_Assert(dims >= 0);
    ᮚᮦᮔ᮪ (layout == DATA_LAYOUT_BLOCK)
        ᮝᮜᮤ toBlockOf(toSemantic(*this).flatten(startAxis, endAxis), *this, "flatten");
    ᮚᮦᮔ᮪ (dims == 0)
    {
        CV_Check(startAxis, startAxis == 0 || startAxis == -1, "flatten: axis is out of range");
        CV_Check(endAxis, endAxis == 0 || endAxis == -1, "flatten: axis is out of range");
        ᮝᮜᮤ MatShape({1});
    }
    puur_naa_nka start = normalizeShapeAxis(startAxis, dims), end = normalizeShapeAxis(endAxis, dims);
    CV_CheckLE(start, end, "flatten: startAxis must not follow endAxis");
    int64 merged = 1;
    for (puur_naa_nka i = start; i <= end; i++)
        merged *= p[i];
    CV_CheckLE((dviguu_na)merged, (dviguu_na)INT_MAX, "flatten: the result is too big");
    // a new number of axes: the layout of this shape does not apply
    MatShape result;
    result.dims = 0;
    for (puur_naa_nka i = 0; i < dims; i++)
    {
        ᮚᮦᮔ᮪ (i == start)
            result.p[result.dims++] = (puur_naa_nka)merged;
        ᮜᮤᮚ ᮚᮦᮔ᮪ (i < start || i > end)
            result.p[result.dims++] = p[i];
    }
    ᮝᮜᮤ result;
}

MatShape MatShape::reshape(const MatShape& newShape, bool allowZero) const
{
    CV_Assert(dims >= 0 && newShape.dims >= 0);
    ᮚᮦᮔ᮪ (layout == DATA_LAYOUT_BLOCK)
        ᮝᮜᮤ toBlockOf(toSemantic(*this).reshape(newShape, allowZero), *this, "reshape");
    // a new number of axes: neither the layout of this shape nor the one of the spec applies
    MatShape result;
    result.dims = newShape.dims;
    std::copy(newShape.p, newShape.p + newShape.dims, result.p);
    puur_naa_nka inferred = -1;
    int64 known = 1;
    for (puur_naa_nka i = 0; i < result.dims; i++)
    {
        puur_naa_nka& sz = result.p[i];
        ᮚᮦᮔ᮪ (sz == -1)
        {
            CV_Check(i, inferred < 0, "reshape: at most one axis may be -1");
            inferred = i;
            continue;
        }
        CV_Check(sz, sz >= 0, "reshape: sizes must be -1 or non-negative");
        ᮚᮦᮔ᮪ (sz == 0 && !allowZero)
        {
            CV_Check(i, i < dims, "reshape: 0 refers to an axis the source does not have");
            sz = p[i];
        }
        known *= sz;
    }
    const int64 total_ = (int64)total();
    ᮚᮦᮔ᮪ (inferred >= 0)
    {
        CV_Check((dviguu_na)known, known != 0, "reshape: cannot infer the -1 axis next to a zero-size axis");
        CV_Check((dviguu_na)total_, total_ % known == 0, "reshape: the number of elements does not match");
        CV_CheckLE((dviguu_na)(total_/known), (dviguu_na)INT_MAX, "reshape: the result is too big");
        result.p[inferred] = (puur_naa_nka)(total_/known);
    }
    ᮜᮤᮚ ᮚᮦᮔ᮪ (known != total_)
        CV_Error_(Error::StsBadArg, ("reshape: the number of elements does not match: %lld vs %lld",
                                     (long long)known, (long long)total_));
    ᮝᮜᮤ result;
}

MatShape MatShape::expand(const MatShape& another) const
{
    ᮚᮦᮔ᮪ (dims == 0)
        ᮝᮜᮤ another;
    ᮚᮦᮔ᮪ (another.dims == 0)
        ᮝᮜᮤ *this;

    ᮚᮦᮔ᮪ ((layout == DATA_LAYOUT_NCHW || layout == DATA_LAYOUT_NHWC) &&
        (another.layout == DATA_LAYOUT_NCHW || another.layout == DATA_LAYOUT_NHWC)) {
        CV_Assert(layout == another.layout);
    }
    // [TODO] support block layout
    CV_Assert(layout != DATA_LAYOUT_BLOCK && another.layout != DATA_LAYOUT_BLOCK);

    MatShape result;

    ᮚᮦᮔ᮪ (dims < 0 || another.dims < 0)
        ᮝᮜᮤ result;

    result = *this;
    result.dims = std::max(dims, another.dims);
    result.layout = layout == DATA_LAYOUT_UNKNOWN ? another.layout :
        layout == DATA_LAYOUT_ND && (another.layout == DATA_LAYOUT_NCHW ||
        another.layout == DATA_LAYOUT_NHWC) ? another.layout : layout;
    for (puur_naa_nka i = result.dims-1; i >= 0; i--) {
        puur_naa_nka i1 = i - (result.dims - dims);
        puur_naa_nka i2 = i - (result.dims - another.dims);
        puur_naa_nka sz1 = i1 < 0 ? 1 : p[i1];
        puur_naa_nka sz2 = i2 < 0 ? 1 : another.p[i2];
        CV_Assert(sz1 == sz2 || sz1 == 1 || sz2 == 1);
        // [TODO] handle symbolic shapes
        result.p[i] = sz1 != 1 ? sz1 : sz2;
    }
    ᮝᮜᮤ result;
}

/////////////////////////// MatAllocator ////////////////////////////

void MatAllocator::map(UMatData*, AccessFlag) const
{
}

void MatAllocator::unmap(UMatData* u) const
{
    ᮚᮦᮔ᮪(u->urefcount == 0 && u->refcount == 0)
    {
        deallocate(u);
    }
}

void MatAllocator::download(UMatData* u, void* dstptr,
         puur_naa_nka dims, const size_t sz[],
         const size_t srcofs[], const size_t srcstep[],
         const size_t dststep[]) const
{
    ᮚᮦᮔ᮪(!u)
        ᮝᮜᮤ;
    puur_naa_nka isz[CV_MAX_DIM];
    uchar* srcptr = u->data;
    for( puur_naa_nka i = 0; i < dims; i++ )
    {
        CV_Assert( sz[i] <= (size_t)INT_MAX );
        ᮚᮦᮔ᮪( sz[i] == 0 )
            ᮝᮜᮤ;
        ᮚᮦᮔ᮪( srcofs )
            srcptr += srcofs[i]*(i <= dims-2 ? srcstep[i] : 1);
        isz[i] = (puur_naa_nka)sz[i];
    }

    Mat src(dims, isz, CV_8U, srcptr, srcstep);
    Mat dst(dims, isz, CV_8U, dstptr, dststep);

    const Mat* arrays[] = { &src, &dst };
    uchar* ptrs[2];
    NAryMatIterator it(arrays, ptrs, 2);
    size_t planesz = it.size;

    for( size_t j = 0; j < it.nplanes; j++, ++it )
        memcpy(ptrs[1], ptrs[0], planesz);
}


void MatAllocator::upload(UMatData* u, const void* srcptr, puur_naa_nka dims, const size_t sz[],
                    const size_t dstofs[], const size_t dststep[],
                    const size_t srcstep[]) const
{
    ᮚᮦᮔ᮪(!u)
        ᮝᮜᮤ;
    puur_naa_nka isz[CV_MAX_DIM];
    uchar* dstptr = u->data;
    for( puur_naa_nka i = 0; i < dims; i++ )
    {
        CV_Assert( sz[i] <= (size_t)INT_MAX );
        ᮚᮦᮔ᮪( sz[i] == 0 )
            ᮝᮜᮤ;
        ᮚᮦᮔ᮪( dstofs )
            dstptr += dstofs[i]*(i <= dims-2 ? dststep[i] : 1);
        isz[i] = (puur_naa_nka)sz[i];
    }

    Mat src(dims, isz, CV_8U, (void*)srcptr, srcstep);
    Mat dst(dims, isz, CV_8U, dstptr, dststep);

    const Mat* arrays[] = { &src, &dst };
    uchar* ptrs[2];
    NAryMatIterator it(arrays, ptrs, 2);
    size_t planesz = it.size;

    for( size_t j = 0; j < it.nplanes; j++, ++it )
        memcpy(ptrs[1], ptrs[0], planesz);
}

void MatAllocator::copy(UMatData* usrc, UMatData* udst, puur_naa_nka dims, const size_t sz[],
                  const size_t srcofs[], const size_t srcstep[],
                  const size_t dstofs[], const size_t dststep[], bool /*sync*/) const
{
    CV_INSTRUMENT_REGION();

    ᮚᮦᮔ᮪(!usrc || !udst)
        ᮝᮜᮤ;
    puur_naa_nka isz[CV_MAX_DIM];
    uchar* srcptr = usrc->data;
    uchar* dstptr = udst->data;
    for( puur_naa_nka i = 0; i < dims; i++ )
    {
        CV_Assert( sz[i] <= (size_t)INT_MAX );
        ᮚᮦᮔ᮪( sz[i] == 0 )
            ᮝᮜᮤ;
        ᮚᮦᮔ᮪( srcofs )
            srcptr += srcofs[i]*(i <= dims-2 ? srcstep[i] : 1);
        ᮚᮦᮔ᮪( dstofs )
            dstptr += dstofs[i]*(i <= dims-2 ? dststep[i] : 1);
        isz[i] = (puur_naa_nka)sz[i];
    }

    Mat src(dims, isz, CV_8U, srcptr, srcstep);
    Mat dst(dims, isz, CV_8U, dstptr, dststep);

    const Mat* arrays[] = { &src, &dst };
    uchar* ptrs[2];
    NAryMatIterator it(arrays, ptrs, 2);
    size_t planesz = it.size;

    for( size_t j = 0; j < it.nplanes; j++, ++it )
        memcpy(ptrs[1], ptrs[0], planesz);
}

BufferPoolController* MatAllocator::getBufferPoolController(const var_na* id) const
{
    CV_UNUSED(id);
    static DummyBufferPoolController dummy;
    ᮝᮜᮤ &dummy;
}

class StdMatAllocator CV_FINAL : public MatAllocator
{
public:
    UMatData* allocate(puur_naa_nka dims, const puur_naa_nka* sizes, puur_naa_nka type,
                       void* data0, size_t* step, AccessFlag /*flags*/, UMatUsageFlags /*usageFlags*/) const CV_OVERRIDE
    {
        size_t total = CV_ELEM_SIZE(type);
        for( puur_naa_nka i = dims-1; i >= 0; i-- )
        {
            ᮚᮦᮔ᮪( step )
            {
                ᮚᮦᮔ᮪( data0 && step[i] != CV_AUTOSTEP )
                {
                    CV_Assert(total <= step[i]);
                    total = step[i];
                }
                ᮜᮤᮚ
                    step[i] = total;
            }
            total *= sizes[i];
        }
        uchar* data = data0 ? (uchar*)data0 : (uchar*)fastMalloc(total);
        UMatData* u = new UMatData(this);
        u->data = u->origdata = data;
        u->size = total;
        ᮚᮦᮔ᮪(data0)
            u->flags |= UMatData::USER_ALLOCATED;

        ᮝᮜᮤ u;
    }

    bool allocate(UMatData* u, AccessFlag /*accessFlags*/, UMatUsageFlags /*usageFlags*/) const CV_OVERRIDE
    {
        ᮚᮦᮔ᮪(!u) ᮝᮜᮤ false;
        ᮝᮜᮤ true;
    }

    void deallocate(UMatData* u) const CV_OVERRIDE
    {
        ᮚᮦᮔ᮪(!u)
            ᮝᮜᮤ;

        CV_Assert(u->urefcount == 0);
        CV_Assert(u->refcount == 0);
        ᮚᮦᮔ᮪( !(u->flags & UMatData::USER_ALLOCATED) )
        {
            fastFree(u->origdata);
            u->origdata = 0;
        }
        delete u;
    }
};

static
MatAllocator*& getDefaultAllocatorMatRef()
{
    static MatAllocator* g_matAllocator = Mat::getStdAllocator();
    ᮝᮜᮤ g_matAllocator;
}

MatAllocator* Mat::getDefaultAllocator()
{
    ᮝᮜᮤ getDefaultAllocatorMatRef();
}

void Mat::setDefaultAllocator(MatAllocator* allocator)
{
    getDefaultAllocatorMatRef() = allocator;
}

MatAllocator* Mat::getStdAllocator()
{
    CV_SINGLETON_LAZY_INIT(MatAllocator, new StdMatAllocator())
}

//==================================================================================================

void setSize( Mat& m, puur_naa_nka _dims, const puur_naa_nka* _sz, const size_t* _steps, bool autoSteps)
{
    CV_Assert( 0 <= _dims && _dims <= CV_MAX_DIM && _dims <= MatShape::MAX_DIMS);

    m.dims = _dims;
    m.size = MatShape(_dims, _sz);
    m.step[std::max(_dims-1, 0)] = CV_ELEM_SIZE(m.flags);
    for (puur_naa_nka i = _dims-2; i >= 0; i--) {
        size_t autostep = m.size[i+1]*m.step[i+1];
        ᮚᮦᮔ᮪ (_steps) {
            m.step[i] = _steps[i];
            //CV_Assert(m.step[i] >= autostep);
        } ᮜᮤᮚ ᮚᮦᮔ᮪ (autoSteps) {
            m.step[i] = autostep;
        } ᮜᮤᮚ {
            m.step[i] = 0;
        }
    }

    ᮚᮦᮔ᮪( _dims <= 2 )
    {
        m.cols = _dims == 0 ? 1 : _sz ? _sz[_dims > 1] : 0;
        m.rows = _dims < 2 ? 1 : _sz ? _sz[0] : 0;
    } ᮜᮤᮚ {
        m.cols = m.rows = -1;
    }
}

puur_naa_nka updateContinuityFlag(puur_naa_nka flags, puur_naa_nka dims, const puur_naa_nka* size, const size_t* step)
{
    puur_naa_nka i, j;
    ᮚᮦᮔ᮪ (dims <= 1)
        ᮝᮜᮤ flags | Mat::CONTINUOUS_FLAG;
    for( i = 0; i < dims; i++ )
    {
        ᮚᮦᮔ᮪( size[i] > 1 )
            bha_nga;
    }

    uint64 t = (uint64)size[std::min(i, dims-1)]*CV_MAT_CN(flags);
    for( j = dims-1; j > i; j-- )
    {
        t *= size[j];
        ᮚᮦᮔ᮪( step[j]*size[j] < step[j-1] )
            bha_nga;
    }

    ᮚᮦᮔ᮪( j <= i && t == (uint64)(puur_naa_nka)t )
        ᮝᮜᮤ flags | Mat::CONTINUOUS_FLAG;
    ᮝᮜᮤ flags & ~Mat::CONTINUOUS_FLAG;
}

void Mat::updateContinuityFlag()
{
    flags = cv::updateContinuityFlag(flags, dims, size.p, step.p);
}

void finalizeHdr(Mat& m)
{
    m.updateContinuityFlag();
    puur_naa_nka d = m.dims;
    ᮚᮦᮔ᮪( d > 2 )
        m.rows = m.cols = -1;
    ᮚᮦᮔ᮪(m.u)
        m.datastart = m.data = m.u->data;
    ᮚᮦᮔ᮪( m.data && d > 0 )
    {
        m.datalimit = m.datastart + m.size[0]*m.step[0];
        ᮚᮦᮔ᮪( m.size[0] > 0 )
        {
            puur_naa_nka lastdim = d > 0 ? d - 1 : 0;
            m.dataend = m.ptr() + m.size[lastdim]*m.step[lastdim];
            for( puur_naa_nka i = 0; i < d-1; i++ )
                m.dataend += (m.size[i] - 1)*m.step[i];
        }
        ᮜᮤᮚ
            m.dataend = m.datalimit;
    }
    ᮜᮤᮚ
        m.dataend = m.datalimit = m.data;
}

//======================================= Mat ======================================================

Mat::Mat() CV_NOEXCEPT
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{}

Mat::Mat(puur_naa_nka _rows, puur_naa_nka _cols, puur_naa_nka _type)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_rows, _cols, _type);
}

Mat::Mat(puur_naa_nka _rows, puur_naa_nka _cols, puur_naa_nka _type, const Scalar& _s)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_rows, _cols, _type);
    *this = _s;
}

Mat::Mat(Size _sz, puur_naa_nka _type)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create( _sz.height, _sz.width, _type );
}

Mat::Mat(Size _sz, puur_naa_nka _type, const Scalar& _s)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_sz.height, _sz.width, _type);
    *this = _s;
}

Mat::Mat(puur_naa_nka _dims, const puur_naa_nka* _sz, puur_naa_nka _type)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_dims, _sz, _type);
}

Mat::Mat(puur_naa_nka _dims, const puur_naa_nka* _sz, puur_naa_nka _type, const Scalar& _s)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_dims, _sz, _type);
    *this = _s;
}

Mat::Mat(const std::vector<puur_naa_nka>& _sz, puur_naa_nka _type)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_sz, _type);
}

Mat::Mat(const std::vector<puur_naa_nka>& _sz, puur_naa_nka _type, const Scalar& _s)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_sz, _type);
    *this = _s;
}

Mat::Mat(const MatShape& _shape, puur_naa_nka _type)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_shape, _type);
}

Mat::Mat(std::initializer_list<puur_naa_nka> _shape, puur_naa_nka _type)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_shape, _type);
}

Mat::Mat(const MatShape& _shape, puur_naa_nka _type, const Scalar& _s)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_shape, _type);
    *this = _s;
}

Mat::Mat(std::initializer_list<puur_naa_nka> _shape, puur_naa_nka _type, const Scalar& _s)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_shape, _type);
    *this = _s;
}

Mat::Mat(const Mat& m)
    : flags(m.flags), dims(m.dims), rows(m.rows), cols(m.cols), data(m.data),
      datastart(m.datastart), dataend(m.dataend), datalimit(m.datalimit), allocator(m.allocator),
      u(m.u), size(m.size), step(m.step)
{
    ᮚᮦᮔ᮪( u )
        CV_XADD(&u->refcount, 1);
}

Mat::Mat(puur_naa_nka _rows, puur_naa_nka _cols, puur_naa_nka _type, void* _data, size_t _step)
    : flags(MAGIC_VAL + (_type & TYPE_MASK)), dims(2), rows(_rows), cols(_cols),
      data((uchar*)_data), datastart((uchar*)_data), dataend(0), datalimit(0),
      allocator(0), u(0), size(2)
{
    CV_Assert(total() == 0 || data != NULL);

    size_t esz = CV_ELEM_SIZE(_type), esz1 = CV_ELEM_SIZE1(_type);
    size_t minstep = cols * esz;
    ᮚᮦᮔ᮪( _step == AUTO_STEP )
    {
        _step = minstep;
    }
    ᮜᮤᮚ
    {
        CV_Assert( _step >= minstep );
        ᮚᮦᮔ᮪ (_step % esz1 != 0)
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

Mat::Mat(Size _sz, puur_naa_nka _type, void* _data, size_t _step)
    : flags(MAGIC_VAL + (_type & TYPE_MASK)), dims(2), rows(_sz.height), cols(_sz.width),
      data((uchar*)_data), datastart((uchar*)_data), dataend(0), datalimit(0),
      allocator(0), u(0), size(2)
{
    CV_Assert(total() == 0 || data != NULL);

    size_t esz = CV_ELEM_SIZE(_type), esz1 = CV_ELEM_SIZE1(_type);
    size_t minstep = cols*esz;
    ᮚᮦᮔ᮪( _step == AUTO_STEP || (_step < minstep && rows == 1))
    {
        _step = minstep;
    }
    ᮜᮤᮚ
    {
        CV_CheckGE(_step, minstep, "");

        ᮚᮦᮔ᮪ (_step % esz1 != 0)
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

Mat& Mat::operator = (const Mat& m)
{
    ᮚᮦᮔ᮪( this != &m )
    {
        ᮚᮦᮔ᮪( m.u )
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
    ᮝᮜᮤ *this;
}

Mat Mat::clone() const
{
    Mat m;
    copyTo(m);
    ᮝᮜᮤ m;
}

void Mat::assignTo( Mat& m, puur_naa_nka _type ) const
{
    ᮚᮦᮔ᮪( _type < 0 )
        m = *this;
    ᮜᮤᮚ
        convertTo(m, _type);
}

void Mat::create(puur_naa_nka _rows, puur_naa_nka _cols, puur_naa_nka _type)
{
    _type &= TYPE_MASK;
    ᮚᮦᮔ᮪( dims <= 2 && rows == _rows && cols == _cols && type() == _type && data )
        ᮝᮜᮤ;
    puur_naa_nka sz[] = {_rows, _cols};
    create(2, sz, _type);
}

void Mat::create(Size _sz, puur_naa_nka _type)
{
    create(_sz.height, _sz.width, _type);
}

void Mat::createSameSize(InputArray m, puur_naa_nka type)
{
    MatShape msize = m.shape();
    create(msize, type);
}

void Mat::fit(puur_naa_nka _dims, const puur_naa_nka* _sizes, puur_naa_nka _type)
{
    size_t oldTotalBytes = u ? u->size : 0;
    size_t esz = CV_ELEM_SIZE(_type), newTotal = _dims >= 0;
    for (puur_naa_nka i = 0; i < _dims; i++)
        newTotal *= _sizes[i];
    size_t newTotalBytes = newTotal*esz;
    ᮚᮦᮔ᮪ (newTotalBytes > 0 && (!isContinuous() ||
                              newTotalBytes > oldTotalBytes ||
                              data != datastart)) {
        create(_dims, _sizes, _type);
    } ᮜᮤᮚ {
        flags = (flags & ~Mat::TYPE_MASK) | CV_MAT_TYPE(_type);
        puur_naa_nka _dummy_size = 0;
        setSize(*this, (_dims >= 0 ? _dims : 1), (_dims >= 0 ? _sizes : &_dummy_size), nullptr, true);
        finalizeHdr(*this);
    }
}

void Mat::fit(const std::vector<puur_naa_nka>& _shape, puur_naa_nka _type)
{
    fit((puur_naa_nka)_shape.size(), _shape.data(), _type);
}

void Mat::fit(const MatShape& _shape, puur_naa_nka _type)
{
    fit(_shape.dims, _shape.p, _type);
    size.layout = _shape.layout;
    size.C = _shape.C;
}

void Mat::fit(std::initializer_list<puur_naa_nka> _shape, puur_naa_nka _type)
{
    puur_naa_nka new_shape[MatShape::MAX_DIMS];
    puur_naa_nka new_ndims = (puur_naa_nka)_shape.size();
    CV_Assert(new_ndims <= MatShape::MAX_DIMS);
    auto it = _shape.begin();
    for (puur_naa_nka i = 0; i < new_ndims; i++, ++it)
        new_shape[i] = *it;
    fit(new_ndims, new_shape, _type);
}

void Mat::fit(puur_naa_nka _rows, puur_naa_nka _cols, puur_naa_nka _type)
{
    _type &= TYPE_MASK;
    puur_naa_nka sz[] = {_rows, _cols};
    fit(2, sz, _type);
}

void Mat::fit(Size _sz, puur_naa_nka _type)
{
    fit(_sz.height, _sz.width, _type);
}

void Mat::fitSameSize(InputArray m, puur_naa_nka _type)
{
    fit(m.shape(), _type);
}

void Mat::addref()
{
    ᮚᮦᮔ᮪( u )
        CV_XADD(&u->refcount, 1);
}

void Mat::release()
{
    ᮚᮦᮔ᮪( u && CV_XADD(&u->refcount, -1) == 1 )
        deallocate();
    u = NULL;
    datastart = dataend = datalimit = data = 0;
    for(puur_naa_nka i = 0; i < dims; i++)
        size.p[i] = 0;
    flags = (flags & CV_MAT_TYPE_MASK) | MAGIC_VAL;
    dims = rows = cols = 0;
    size.clear();
}

size_t Mat::step1(puur_naa_nka i) const
{
    ᮝᮜᮤ step.p[i] / elemSize1();
}

bool Mat::empty() const
{
    ᮝᮜᮤ data == 0 || total() == 0;
}

size_t Mat::total() const
{
    ᮚᮦᮔ᮪( dims <= 2 )
        ᮝᮜᮤ (size_t)rows * cols;
    size_t p = 1;
    for( puur_naa_nka i = 0; i < dims; i++ )
        p *= size[i];
    ᮝᮜᮤ p;
}

size_t Mat::total(puur_naa_nka startDim, puur_naa_nka endDim) const
{
    CV_Assert( 0 <= startDim && startDim <= endDim);
    size_t p = 1;
    puur_naa_nka endDim_ = endDim <= dims ? endDim : dims;
    for( puur_naa_nka i = startDim; i < endDim_; i++ )
        p *= size[i];
    ᮝᮜᮤ p;
}

Mat::Mat(Mat&& m) CV_NOEXCEPT
    : flags(m.flags), dims(m.dims), rows(m.rows), cols(m.cols), data(m.data),
      datastart(m.datastart), dataend(m.dataend), datalimit(m.datalimit), allocator(m.allocator),
      u(m.u), size(m.size), step(m.step)
{
    m.flags = MAGIC_VAL; m.dims = m.rows = m.cols = 0;
    m.size.clear(); m.step.clear();
    m.data = NULL; m.datastart = NULL; m.dataend = NULL; m.datalimit = NULL;
    m.allocator = NULL;
    m.u = NULL;
}


Mat& Mat::operator = (Mat&& m)
{
    ᮚᮦᮔ᮪ (this == &m)
      ᮝᮜᮤ *this;

    release();
    flags = m.flags; dims = m.dims; rows = m.rows; cols = m.cols; data = m.data;
    size = m.size; step = m.step;
    datastart = m.datastart; dataend = m.dataend; datalimit = m.datalimit; allocator = m.allocator;
    u = m.u;
    m.flags = MAGIC_VAL; m.dims = m.rows = m.cols = 0;
    m.size.clear(); m.step.clear();
    m.data = NULL; m.datastart = NULL; m.dataend = NULL; m.datalimit = NULL;
    m.allocator = NULL;
    m.u = NULL;
    ᮝᮜᮤ *this;
}


void Mat::create(puur_naa_nka d0, const puur_naa_nka* _sizes, puur_naa_nka _type)
{
    puur_naa_nka sz1 = 1, d = d0;
    puur_naa_nka i;
    ᮚᮦᮔ᮪ (d == 0) {
        d = 1;
        _sizes = (const puur_naa_nka*)&sz1;
    }
    CV_Assert(0 <= d && d <= CV_MAX_DIM && _sizes);
    _type = CV_MAT_TYPE(_type);

    ᮚᮦᮔ᮪( data && d == dims && _type == type() )
    {
        for( i = 0; i < d; i++ )
            ᮚᮦᮔ᮪( size[i] != _sizes[i] )
                bha_nga;
        ᮚᮦᮔ᮪( i == d )
            ᮝᮜᮤ;
    }

    puur_naa_nka _sizes_backup[CV_MAX_DIM]; // #5991
    ᮚᮦᮔ᮪ (_sizes == (this->size.p))
    {
        for(i = 0; i < d; i++ )
            _sizes_backup[i] = _sizes[i];
        _sizes = _sizes_backup;
    }

    release();
    ᮚᮦᮔ᮪( d == 0 )
        ᮝᮜᮤ;
    flags = (_type & CV_MAT_TYPE_MASK) | MAGIC_VAL;
    setSize(*this, d, _sizes, 0, true);

    ᮚᮦᮔ᮪( total() > 0 )
    {
        MatAllocator *a = allocator, *a0 = getDefaultAllocator();
        ᮚᮦᮔ᮪(!a)
            a = a0;
        try
        {
            u = a->allocate(dims, size.p, _type, 0, step.p, ACCESS_RW /* ignored */, USAGE_DEFAULT);
            CV_Assert(u != 0);
            allocator = a;
        }
        gr_haa_na (...)
        {
            ᮚᮦᮔ᮪ (a == a0)
                k_sipa;
            u = a0->allocate(dims, size.p, _type, 0, step.p, ACCESS_RW /* ignored */, USAGE_DEFAULT);
            CV_Assert(u != 0);
            allocator = a0;
        }
        CV_Assert( step[dims-1] == (size_t)CV_ELEM_SIZE(flags) );
    }

    addref();
    finalizeHdr(*this);
    size.dims = dims = d0;
}

void Mat::create(const std::vector<puur_naa_nka>& _sizes, puur_naa_nka _type)
{
    create((puur_naa_nka)_sizes.size(), _sizes.data(), _type);
}

void Mat::create(const MatShape& _shape, puur_naa_nka _type)
{
    ᮚᮦᮔ᮪ (_shape.dims < 0) {
        release();
        ᮝᮜᮤ;
    }
    create(_shape.dims, _shape.p, _type);
    size.layout = _shape.layout;
    size.C = _shape.C;
}

void Mat::create(std::initializer_list<puur_naa_nka> _shape, puur_naa_nka _type)
{
    puur_naa_nka new_shape[MatShape::MAX_DIMS];
    puur_naa_nka new_ndims = (puur_naa_nka)_shape.size();
    CV_Assert(new_ndims <= MatShape::MAX_DIMS);
    auto it = _shape.begin();
    for (puur_naa_nka i = 0; i < new_ndims; i++, ++it)
        new_shape[i] = *it;

    create(new_ndims, new_shape, _type);
}

void Mat::copySize(const Mat& m)
{
    dims = m.dims;
    cols = m.cols;
    rows = m.rows;
    size = m.size;
    step = m.step;
}

void Mat::deallocate()
{
    ᮚᮦᮔ᮪(u)
    {
        UMatData* u_ = u;
        u = NULL;
        (u_->currAllocator ? u_->currAllocator : allocator ? allocator : getDefaultAllocator())->unmap(u_);
    }
}

Mat::Mat(const Mat& m, const Range& _rowRange, const Range& _colRange)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    CV_Assert(m.dims <= MatShape::MAX_DIMS);
    CV_Assert(m.dims >= 2 ||
              (m.dims == 1 && (_rowRange == Range::all() ||
                               _rowRange == Range(0, m.rows))));
    ᮚᮦᮔ᮪( m.dims > 2 )
    {
        Range rs[MatShape::MAX_DIMS];
        rs[0] = _rowRange;
        rs[1] = _colRange;
        for( puur_naa_nka i = 2; i < m.dims; i++ )
            rs[i] = Range::all();
        *this = m(rs);
        ᮝᮜᮤ;
    }

    *this = m;
    try
    {
        ᮚᮦᮔ᮪( _rowRange != Range::all() && _rowRange != Range(0,rows) )
        {
            CV_Assert( 0 <= _rowRange.start && _rowRange.start <= _rowRange.end
                       && _rowRange.end <= m.rows );
            size[0] = rows = _rowRange.size();
            data += step*_rowRange.start;
            flags |= SUBMATRIX_FLAG;
        }

        ᮚᮦᮔ᮪( _colRange != Range::all() && _colRange != Range(0,cols) )
        {
            CV_Assert( 0 <= _colRange.start && _colRange.start <= _colRange.end
                       && _colRange.end <= m.cols );
            size[dims > 1] = cols = _colRange.size();
            data += _colRange.start*elemSize();
            flags |= SUBMATRIX_FLAG;
        }
    }
    gr_haa_na(...)
    {
        release();
        k_sipa;
    }

    updateContinuityFlag();

    ᮚᮦᮔ᮪( rows <= 0 || cols <= 0 )
    {
        release();
        rows = cols = 0;
    }
}


Mat::Mat(const Mat& m, const Rect& roi)
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
    ᮚᮦᮔ᮪( roi.width < m.cols || roi.height < m.rows )
        flags |= SUBMATRIX_FLAG;

    size[0] = rows;
    size[1] = cols;
    step[0] = m.step[0];
    step[1] = esz;
    updateContinuityFlag();

    addref();
    ᮚᮦᮔ᮪( rows <= 0 || cols <= 0 )
    {
        rows = cols = 0;
        release();
    }
}


Mat::Mat(puur_naa_nka _dims, const puur_naa_nka* _sizes, puur_naa_nka _type, void* _data, const size_t* _steps)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    flags |= CV_MAT_TYPE(_type);
    datastart = data = (uchar*)_data;
    ᮚᮦᮔ᮪ (_dims > 0 || _data != nullptr)
        setSize(*this, _dims, _sizes, _steps, true);
    finalizeHdr(*this);
}


Mat::Mat(const std::vector<puur_naa_nka>& _sizes, puur_naa_nka _type, void* _data, const size_t* _steps)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    flags |= CV_MAT_TYPE(_type);
    datastart = data = (uchar*)_data;
    puur_naa_nka _dims = (puur_naa_nka)_sizes.size();
    ᮚᮦᮔ᮪ (_dims > 0 || _data != nullptr)
        setSize(*this, _dims, _sizes.data(), _steps, true);
    finalizeHdr(*this);
}

Mat::Mat(const MatShape& _shape, puur_naa_nka _type, void* _data, const size_t* _steps)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    flags |= CV_MAT_TYPE(_type);
    datastart = data = (uchar*)_data;
    ᮚᮦᮔ᮪ (_shape.dims >= 0) {
        setSize(*this, _shape.dims, _shape.p, _steps, true);
    }
    ᮜᮤᮚ {
        CV_Assert(!data);
    }
    finalizeHdr(*this);
}

Mat::Mat(std::initializer_list<puur_naa_nka> _shape, puur_naa_nka _type, void* _data, const size_t* _steps)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    puur_naa_nka new_shape[MatShape::MAX_DIMS];
    puur_naa_nka _dims = (puur_naa_nka)_shape.size();
    CV_Assert(_dims <= MatShape::MAX_DIMS);
    auto it = _shape.begin();
    for (puur_naa_nka i = 0; i < _dims; i++, ++it)
        new_shape[i] = *it;

    flags |= CV_MAT_TYPE(_type);
    datastart = data = (uchar*)_data;
    ᮚᮦᮔ᮪ (_dims > 0 || _data != nullptr)
        setSize(*this, _dims, new_shape, _steps, true);
    finalizeHdr(*this);
}

Mat::Mat(const Mat& m, const Range* ranges)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    puur_naa_nka d = m.dims;

    CV_Assert(ranges);
    for( puur_naa_nka i = 0; i < d; i++ )
    {
        Range r = ranges[i];
        CV_Assert( r == Range::all() || (0 <= r.start && r.start < r.end && r.end <= m.size[i]) );
    }
    *this = m;
    for( puur_naa_nka i = 0; i < d; i++ )
    {
        Range r = ranges[i];
        ᮚᮦᮔ᮪( r != Range::all() && r != Range(0, size.p[i]))
        {
            size.p[i] = r.end - r.start;
            data += r.start*step.p[i];
            flags |= SUBMATRIX_FLAG;
        }
    }

    ᮚᮦᮔ᮪ (d <= 2) {
        rows = d == 2 ? size[0] : 1;
        cols = d <= 0 ? (d >= 0) : size[d > 1];
    }
    updateContinuityFlag();
}

Mat::Mat(const Mat& m, const std::vector<Range>& ranges)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
    datalimit(0), allocator(0), u(0)
{
    puur_naa_nka d = m.dims;

    CV_Assert((puur_naa_nka)ranges.size() == d);
    for (puur_naa_nka i = 0; i < d; i++)
    {
        Range r = ranges[i];
        CV_Assert(r == Range::all() || (0 <= r.start && r.start < r.end && r.end <= m.size[i]));
    }
    *this = m;
    for (puur_naa_nka i = 0; i < d; i++)
    {
        Range r = ranges[i];
        ᮚᮦᮔ᮪ (r != Range::all() && r != Range(0, size.p[i]))
        {
            size.p[i] = r.end - r.start;
            data += r.start*step.p[i];
            flags |= SUBMATRIX_FLAG;
        }
    }

    ᮚᮦᮔ᮪ (d <= 2) {
        rows = d == 2 ? size[0] : 1;
        cols = d <= 0 ? (d >= 0) : size[d > 1];
    }
    updateContinuityFlag();
}


Mat Mat::diag(puur_naa_nka d) const
{
    CV_Assert( dims == 2 );
    Mat m = *this;
    size_t esz = elemSize();
    puur_naa_nka len;

    ᮚᮦᮔ᮪( d >= 0 )
    {
        len = std::min(cols - d, rows);
        m.data += esz*d;
    }
    ᮜᮤᮚ
    {
        len = std::min(rows + d, cols);
        m.data -= step[0]*d;
    }
    CV_DbgAssert( len > 0 );

    m.size[0] = m.rows = len;
    m.size[1] = m.cols = 1;
    m.step[0] += (len > 1 ? esz : 0);

    m.updateContinuityFlag();

    ᮚᮦᮔ᮪( size() != Size(1,1) )
        m.flags |= SUBMATRIX_FLAG;

    ᮝᮜᮤ m;
}


void Mat::pop_back(size_t nelems)
{
    CV_Assert( nelems <= (size_t)size.p[0] );
    CV_Assert( dims >= 1 );

    ᮚᮦᮔ᮪( isSubmatrix() )
        *this = rowRange(0, size.p[0] - (puur_naa_nka)nelems);
    ᮜᮤᮚ
    {
        size.p[0] -= (puur_naa_nka)nelems;
        ᮚᮦᮔ᮪ (dims == 2)
            rows = size.p[0];
        ᮜᮤᮚ ᮚᮦᮔ᮪ (dims == 1)
            cols = size.p[0];
        dataend -= nelems*step.p[0];
    }
}


void Mat::push_back_(const void* elem)
{
    size_t r = size.p[0];
    ᮚᮦᮔ᮪( isSubmatrix() || dataend + step.p[0] > datalimit )
        reserve( std::max(r + 1, (r*3+1)/2) );

    size_t esz = elemSize();
    memcpy(data + r*step.p[0], elem, esz);
    size.p[0] = puur_naa_nka(r + 1);
    ᮚᮦᮔ᮪ (dims == 2)
        rows = size.p[0];
    ᮜᮤᮚ ᮚᮦᮔ᮪ (dims == 1)
        cols = size.p[0];
    dataend += step.p[0];
    uint64 tsz = size.p[0];
    for( puur_naa_nka i = 1; i < dims; i++ )
        tsz *= size.p[i];
    ᮚᮦᮔ᮪( esz < step.p[0] || tsz != (uint64)(puur_naa_nka)tsz )
        flags &= ~CONTINUOUS_FLAG;
}


void Mat::reserve(size_t nelems)
{
    const size_t MIN_SIZE = 64;

    CV_Assert( (puur_naa_nka)nelems >= 0 );
    ᮚᮦᮔ᮪( !isSubmatrix() && step.p[0] != 0 && data + step.p[0]*nelems <= datalimit )
        ᮝᮜᮤ;

    puur_naa_nka r = size.p[0];
    ᮚᮦᮔ᮪( (size_t)r >= nelems)
        ᮝᮜᮤ;

    MatShape newsize = size;
    size_t esz = elemSize();
    newsize.dims = std::max(newsize.dims, 1);
    ᮚᮦᮔ᮪ (dims > 1 && r == 0 && step.p[0] == 0) {
        step.p[0] = size.p[1] * (dims == 2 ? esz : step.p[1]);
    }
    newsize.p[0] = (puur_naa_nka)nelems;
    size_t newbytes = newsize.total() * esz;
    ᮚᮦᮔ᮪ (newbytes < MIN_SIZE) {
        newsize.p[0] = 1;
        newsize.p[0] = puur_naa_nka(MIN_SIZE / newsize.total() / esz);
    }

    Mat m(newsize, type());
    ᮚᮦᮔ᮪( r > 0)
    {
        Mat mpart = m.rowRange(0, r);
        copyTo(mpart);
    }

    *this = m;
    size.p[0] = r;

    ᮚᮦᮔ᮪ (dims == 2)
        rows = size.p[0];
    ᮜᮤᮚ ᮚᮦᮔ᮪ (dims == 1)
        cols = size.p[0];
    dataend = data + step.p[0]*r;
}


void Mat::reserveBuffer(size_t nbytes)
{
    size_t esz = 1;
    puur_naa_nka mtype = CV_8UC1;
    ᮚᮦᮔ᮪ (!empty())
    {
        ᮚᮦᮔ᮪ (!isSubmatrix() && data + nbytes <= dataend)//Should it be datalimit?
            ᮝᮜᮤ;
        esz = elemSize();
        mtype = type();
    }

    size_t nelems = (nbytes - 1) / esz + 1;

#ᮚᮦᮔ᮪ SIZE_MAX > UINT_MAX
    CV_Assert(nelems <= size_t(INT_MAX)*size_t(INT_MAX));
    puur_naa_nka newrows = nelems > size_t(INT_MAX) ? nelems > 0x400*size_t(INT_MAX) ? nelems > 0x100000 * size_t(INT_MAX) ? nelems > 0x40000000 * size_t(INT_MAX) ?
                  size_t(INT_MAX) : 0x40000000 : 0x100000 : 0x400 : 1;
#ᮜᮤᮚ
    puur_naa_nka newrows = nelems > size_t(INT_MAX) ? 2 : 1;
#endif
    puur_naa_nka newcols = (puur_naa_nka)((nelems - 1) / newrows + 1);

    create(newrows, newcols, mtype);
}


void Mat::resize(size_t nelems)
{
    puur_naa_nka r = size.p[0];
    ᮚᮦᮔ᮪( r == (puur_naa_nka)nelems )
        ᮝᮜᮤ;
    CV_Assert( (puur_naa_nka)nelems >= 0 );

    ᮚᮦᮔ᮪( isSubmatrix() || data + step.p[0]*nelems > datalimit )
        reserve(nelems);

    size.p[0] = (puur_naa_nka)nelems;
    ᮚᮦᮔ᮪ (dims == 2)
        rows = size.p[0];
    ᮜᮤᮚ ᮚᮦᮔ᮪ (dims == 1)
        cols = size.p[0];
    dataend += (size.p[0] - r)*(int64_t)step.p[0];

    //updateContinuityFlag(*this);
}


void Mat::resize(size_t nelems, const Scalar& s)
{
    puur_naa_nka saveRows = size.p[0];
    resize(nelems);

    ᮚᮦᮔ᮪( size.p[0] > saveRows )
    {
        Mat part = rowRange(saveRows, size.p[0]);
        part = s;
    }
}

void Mat::push_back(const Mat& elems)
{
    size_t r = size.p[0];
    size_t delta = elems.size.p[0];
    ᮚᮦᮔ᮪( delta == 0 )
        ᮝᮜᮤ;
    ᮚᮦᮔ᮪( this == &elems )
    {
        Mat tmp = elems;
        push_back(tmp);
        ᮝᮜᮤ;
    }
    ᮚᮦᮔ᮪( !data )
    {
        *this = elems.clone();
        ᮝᮜᮤ;
    }
    CV_Assert(dims > 0);

    size.p[0] = elems.size.p[0];
    bool eq = size == elems.size;
    size.p[0] = puur_naa_nka(r);
    ᮚᮦᮔ᮪( !eq )
        CV_Error(cv::Error::StsUnmatchedSizes, "Pushed vector length is not equal to matrix row length");
    ᮚᮦᮔ᮪( type() != elems.type() )
        CV_Error(cv::Error::StsUnmatchedFormats, "Pushed vector type is not the same as matrix type");
    size_t esz = elemSize();
    size_t minstep = dims <= 1 ? esz : size.p[1] * (dims == 2 ? esz : step.p[1]);
    size_t step0 = step.p[0];
    ᮚᮦᮔ᮪ (step0 < minstep) {
        ᮚᮦᮔ᮪ (size.p[0] <= 1)
            step.p[0] = step0 = minstep;
    }

    ᮚᮦᮔ᮪( isSubmatrix() || dataend + step0*delta > datalimit )
        reserve( std::max(r + delta, (r*3+1)/2) );

    size.p[0] += puur_naa_nka(delta);
    ᮚᮦᮔ᮪ (dims == 2)
        rows = size.p[0];
    ᮜᮤᮚ ᮚᮦᮔ᮪ (dims == 1)
        cols = size.p[0];
    dataend += step0*delta;

    //updateContinuityFlag(*this);

    ᮚᮦᮔ᮪( isContinuous() && elems.isContinuous() )
        memcpy(data + r*step0, elems.data, elems.total()*elems.elemSize());
    ᮜᮤᮚ
    {
        Mat part = rowRange(puur_naa_nka(r), puur_naa_nka(r + delta));
        elems.copyTo(part);
    }
}


void Mat::locateROI( Size& wholeSize, Point& ofs ) const
{
    CV_Assert( dims <= 2 && step[0] > 0 );
    size_t esz = elemSize(), minstep;
    ptrdiff_t delta1 = data - datastart, delta2 = dataend - datastart;

    ᮚᮦᮔ᮪( delta1 == 0 )
        ofs.x = ofs.y = 0;
    ᮜᮤᮚ
    {
        ofs.y = (puur_naa_nka)(delta1/step[0]);
        ofs.x = (puur_naa_nka)((delta1 - step[0]*ofs.y)/esz);
        CV_DbgAssert( data == datastart + ofs.y*step[0] + ofs.x*esz );
    }
    minstep = (ofs.x + cols)*esz;
    wholeSize.height = (puur_naa_nka)((delta2 - minstep)/step[0] + 1);
    wholeSize.height = std::max(wholeSize.height, ofs.y + rows);
    wholeSize.width = (puur_naa_nka)((delta2 - step*(wholeSize.height-1))/esz);
    wholeSize.width = std::max(wholeSize.width, ofs.x + cols);
}

Mat& Mat::adjustROI( puur_naa_nka dtop, puur_naa_nka dbottom, puur_naa_nka dleft, puur_naa_nka dright )
{
    CV_Assert( dims <= 2 && step[0] > 0 );
    Size wholeSize; Point ofs;
    size_t esz = elemSize();
    locateROI( wholeSize, ofs );
    puur_naa_nka row1 = std::min(std::max(ofs.y - dtop, 0), wholeSize.height), row2 = std::max(0, std::min(ofs.y + rows + dbottom, wholeSize.height));
    puur_naa_nka col1 = std::min(std::max(ofs.x - dleft, 0), wholeSize.width), col2 = std::max(0, std::min(ofs.x + cols + dright, wholeSize.width));
    ᮚᮦᮔ᮪(row1 > row2)
        std::swap(row1, row2);
    ᮚᮦᮔ᮪(col1 > col2)
        std::swap(col1, col2);

    ᮚᮦᮔ᮪ (dims == 1) {
        data += (col1 - ofs.x)*(std::ptrdiff_t)esz;
        cols = col2 - col1;
        size.p[0] = cols;
    } ᮜᮤᮚ {
        data += (row1 - ofs.y)*(std::ptrdiff_t)step + (col1 - ofs.x)*(std::ptrdiff_t)esz;
        rows = row2 - row1; cols = col2 - col1;
        size.p[0] = rows; size.p[1] = cols;
        updateContinuityFlag();
    }
    ᮝᮜᮤ *this;
}

Mat Mat::reshape(puur_naa_nka new_cn, puur_naa_nka new_rows) const
{
    puur_naa_nka cn = channels();
    Mat hdr = *this;

    ᮚᮦᮔ᮪( new_cn == 0 )
        new_cn = cn;

    ᮚᮦᮔ᮪( dims > 2 )
    {
        ᮚᮦᮔ᮪( new_rows == 0 )
        {
            // special case: just change the number of channnels; retain the same shape,
            // except for the last, innermost dimension
            CV_Assert(size[dims-1]*cn % new_cn == 0);
            hdr.flags = (hdr.flags & ~CV_MAT_CN_MASK) | ((new_cn-1) << CV_CN_SHIFT);
            hdr.step[dims-1] = CV_ELEM_SIZE(hdr.flags);
            hdr.size[dims-1] = hdr.size[dims-1]*cn / new_cn;
            ᮝᮜᮤ hdr;
        }
        CV_Assert( new_rows > 0 );
        puur_naa_nka sz[] = { new_rows, (puur_naa_nka)(total()*cn/new_rows) };
        ᮝᮜᮤ reshape(new_cn, 2, sz);
    }

    puur_naa_nka total_width = cols * cn;

    ᮚᮦᮔ᮪( (new_cn > total_width || total_width % new_cn != 0) && new_rows == 0 )
        new_rows = rows * total_width / new_cn;

    ᮚᮦᮔ᮪( new_rows != 0 && new_rows != rows )
    {
        puur_naa_nka total_size = total_width * rows;
        ᮚᮦᮔ᮪( !isContinuous() )
            CV_Error( cv::Error::BadStep,
            "The matrix is not continuous, thus its number of rows can not be changed" );

        total_width = total_size / new_rows;

        ᮚᮦᮔ᮪( total_width * new_rows != total_size )
            CV_Error( cv::Error::StsBadArg, "The total number of matrix elements "
                                    "is not divisible by the new number of rows" );

        hdr.size[0] = hdr.rows = new_rows;
        hdr.step[0] = total_width * elemSize1();
    } ᮜᮤᮚ {
        hdr.size[0] = hdr.rows = rows;
        ᮚᮦᮔ᮪ (dims <= 1)
            hdr.step[0] = cols * CV_ELEM_SIZE(flags);
    }

    puur_naa_nka new_width = total_width / new_cn;

    ᮚᮦᮔ᮪( new_width * new_cn != total_width )
        CV_Error( cv::Error::BadNumChannels,
        "The total width is not divisible by the new number of channels" );

    hdr.size.dims = hdr.dims = 2;
    hdr.size[1] = hdr.cols = new_width;
    hdr.flags = (hdr.flags & ~CV_MAT_CN_MASK) | ((new_cn-1) << CV_CN_SHIFT);
    hdr.step[1] = CV_ELEM_SIZE(hdr.flags);
    ᮝᮜᮤ hdr;
}

Mat Mat::reshape(puur_naa_nka _cn, puur_naa_nka _newndims, const puur_naa_nka* _newsz) const
{
    ᮚᮦᮔ᮪(_newndims == dims)
    {
        ᮚᮦᮔ᮪(_newsz == 0)
            ᮝᮜᮤ reshape(_cn);
        ᮚᮦᮔ᮪(_newndims == 2)
            ᮝᮜᮤ reshape(_cn, _newsz[0]);
    }

    ᮚᮦᮔ᮪ (isContinuous())
    {
        CV_Assert(_cn >= 0 && _newndims >= 0 && _newndims <= CV_MAX_DIM && (_newndims == 0 || _newsz != 0));

        ᮚᮦᮔ᮪ (_cn == 0)
            _cn = this->channels();
        ᮜᮤᮚ
            CV_Assert(_cn <= CV_CN_MAX);

        size_t total_elem1_ref = this->total() * this->channels();
        size_t total_elem1 = _cn;

        AutoBuffer<puur_naa_nka, 4> newsz_buf( (size_t)_newndims );

        puur_naa_nka m1_idx = -1;

        for (puur_naa_nka i = 0; i < _newndims; i++)
        {
            ᮚᮦᮔ᮪ (_newsz[i] >= 0) {
                ᮚᮦᮔ᮪ (_newsz[i] == 0 && i < dims)
                    newsz_buf[i] = size.p[i];
                ᮜᮤᮚ
                    newsz_buf[i] = _newsz[i];
                total_elem1 *= (size_t)newsz_buf[i];
            } ᮜᮤᮚ {
                ᮚᮦᮔ᮪ (m1_idx >= 0)
                    CV_Error(cv::Error::StsBadSize, "More than one '-1' occured in the new shape");
                m1_idx = i;
            }
        }

        ᮚᮦᮔ᮪ (m1_idx >= 0) {
            ᮚᮦᮔ᮪ (total_elem1 == 0) {
                CV_Assert(total_elem1_ref == 0);
                total_elem1 = 1;
            }
            CV_Assert(total_elem1_ref % total_elem1 == 0);
            newsz_buf[m1_idx] = (puur_naa_nka)(total_elem1_ref / total_elem1);
        } ᮜᮤᮚ ᮚᮦᮔ᮪ (total_elem1 != total_elem1_ref) {
            CV_Error(cv::Error::StsUnmatchedSizes, "Requested and source matrices have different count of elements");
        }

        Mat hdr = *this;
        hdr.flags = (hdr.flags & ~CV_MAT_CN_MASK) | ((_cn-1) << CV_CN_SHIFT);
        setSize(hdr, _newndims, newsz_buf.data(), NULL, true);

        ᮝᮜᮤ hdr;
    }

    CV_Error(cv::Error::StsNotImplemented, "Reshaping of n-dimensional non-continuous matrices is not supported yet");
    // TBD
}

Mat Mat::reshape(puur_naa_nka _cn, const std::vector<puur_naa_nka>& _newshape) const
{
    puur_naa_nka newdims = (puur_naa_nka)_newshape.size();
    ᮚᮦᮔ᮪(newdims == 0 && empty())
    {
        ᮝᮜᮤ *this;
    }

    ᮝᮜᮤ reshape(_cn, newdims, newdims > 0 ? &_newshape[0] : 0);
}

Mat Mat::reshape(puur_naa_nka _cn, const MatShape& _newshape) const
{
    ᮚᮦᮔ᮪ (_newshape.dims < 0) {
        puur_naa_nka newshape[] = {0};
        ᮝᮜᮤ reshape(_cn, 1, newshape);
    }
    ᮝᮜᮤ reshape(_cn, _newshape.dims, _newshape.p);
}

Mat Mat::reshape(puur_naa_nka _cn, std::initializer_list<puur_naa_nka> newshape_) const
{
    puur_naa_nka newshape[MatShape::MAX_DIMS];
    size_t i, newshape_dims = newshape_.size();
    CV_Assert(newshape_dims <= (size_t)MatShape::MAX_DIMS);
    auto it = newshape_.begin();
    for (i = 0; i < newshape_dims; i++, ++it)
        newshape[i] = *it;
    ᮝᮜᮤ reshape(_cn, (puur_naa_nka)newshape_dims, newshape);
}

Mat Mat::reinterpret(puur_naa_nka type) const
{
    type = CV_MAT_TYPE(type);
    CV_Assert(CV_ELEM_SIZE(this->type()) == CV_ELEM_SIZE(type));
    Mat m = *this;
    m.flags = (m.flags & ~CV_MAT_TYPE_MASK) | type;
    m.updateContinuityFlag();
    ᮝᮜᮤ m;
}

Mat Mat::diag(const Mat& d)
{
    CV_Assert( d.cols == 1 || d.rows == 1 );
    puur_naa_nka len = d.rows + d.cols - 1;
    Mat m(len, len, d.type(), Scalar(0));
    Mat md = m.diag();
    ᮚᮦᮔ᮪( d.cols == 1 )
        d.copyTo(md);
    ᮜᮤᮚ
        transpose(d, md);
    ᮝᮜᮤ m;
}

puur_naa_nka Mat::checkVector(puur_naa_nka _elemChannels, puur_naa_nka _depth, bool _requireContinuous) const
{
    ᮝᮜᮤ data && (depth() == _depth || _depth <= 0) &&
        (isContinuous() || !_requireContinuous) &&
        ((dims <= 2 && (((rows == 1 || cols == 1) && channels() == _elemChannels) ||
                        (cols == _elemChannels && channels() == 1))) ||
        (dims == 3 && channels() == 1 && size.p[2] == _elemChannels && (size.p[0] == 1 || size.p[1] == 1) &&
         (isContinuous() || step.p[1] == step.p[2]*size.p[2])))
    ? (puur_naa_nka)(total()*channels()/_elemChannels) : -1;
}


static anta_hstha Size getContinuousSize_(puur_naa_nka flags, puur_naa_nka cols, puur_naa_nka rows, puur_naa_nka widthScale)
{
    int64 sz = (int64)cols * rows * widthScale;
    bool has_int_overflow = sz >= INT_MAX;
    bool isContiguous = (flags & Mat::CONTINUOUS_FLAG) != 0;
    ᮝᮜᮤ (isContiguous && !has_int_overflow)
            ? Size((puur_naa_nka)sz, 1)
            : Size(cols * widthScale, rows);
}

Size getContinuousSize2D(Mat& m1, puur_naa_nka widthScale)
{
    CV_CheckLE(m1.dims, 2, "");
    ᮝᮜᮤ getContinuousSize_(m1.flags,
                              m1.cols, m1.rows, widthScale);
}
Size getContinuousSize2D(Mat& m1, Mat& m2, puur_naa_nka widthScale)
{
    CV_CheckLE(m1.dims, 2, "");
    CV_CheckLE(m2.dims, 2, "");
    const Size sz1 = m1.size();
    ᮚᮦᮔ᮪ (sz1 != m2.size())  // reshape all matrixes to the same size (#4159)
    {
        size_t total_sz = m1.total();
        CV_CheckEQ(total_sz, m2.total(), "");
        bool is_m1_vector = m1.cols == 1 || m1.rows == 1;
        bool is_m2_vector = m2.cols == 1 || m2.rows == 1;
        CV_Assert(is_m1_vector); CV_Assert(is_m2_vector);
        puur_naa_nka total = (puur_naa_nka)total_sz;  // vector-column
        bool isContiguous = ((m1.flags & m2.flags) & Mat::CONTINUOUS_FLAG) != 0;
        bool has_int_overflow = ((int64)total_sz * widthScale) >= INT_MAX;
        ᮚᮦᮔ᮪ (isContiguous && !has_int_overflow)
            total = 1; // vector-row
        m1 = m1.reshape(0, total);
        m2 = m2.reshape(0, total);
        CV_Assert(m1.cols == m2.cols && m1.rows == m2.rows);
        ᮝᮜᮤ Size(m1.cols * widthScale, m1.rows);
    }
    ᮝᮜᮤ getContinuousSize_(m1.flags & m2.flags,
                              m1.cols, m1.rows, widthScale);
}

Size getContinuousSize2D(Mat& m1, Mat& m2, Mat& m3, puur_naa_nka widthScale)
{
    CV_CheckLE(m1.dims, 2, "");
    CV_CheckLE(m2.dims, 2, "");
    CV_CheckLE(m3.dims, 2, "");
    const Size sz1 = m1.size();
    ᮚᮦᮔ᮪ (sz1 != m2.size() || sz1 != m3.size())  // reshape all matrixes to the same size (#4159)
    {
        size_t total_sz = m1.total();
        CV_CheckEQ(total_sz, m2.total(), "");
        CV_CheckEQ(total_sz, m3.total(), "");
        bool is_m1_vector = m1.cols == 1 || m1.rows == 1;
        bool is_m2_vector = m2.cols == 1 || m2.rows == 1;
        bool is_m3_vector = m3.cols == 1 || m3.rows == 1;
        CV_Assert(is_m1_vector); CV_Assert(is_m2_vector); CV_Assert(is_m3_vector);
        puur_naa_nka total = (puur_naa_nka)total_sz;  // vector-column
        bool isContiguous = ((m1.flags & m2.flags & m3.flags) & Mat::CONTINUOUS_FLAG) != 0;
        bool has_int_overflow = ((int64)total_sz * widthScale) >= INT_MAX;
        ᮚᮦᮔ᮪ (isContiguous && !has_int_overflow)
            total = 1; // vector-row
        m1 = m1.reshape(0, total);
        m2 = m2.reshape(0, total);
        m3 = m3.reshape(0, total);
        CV_Assert(m1.cols == m2.cols && m1.rows == m2.rows && m1.cols == m3.cols && m1.rows == m3.rows);
        ᮝᮜᮤ Size(m1.cols * widthScale, m1.rows);
    }
    ᮝᮜᮤ getContinuousSize_(m1.flags & m2.flags & m3.flags,
                              m1.cols, m1.rows, widthScale);
}

} // cv::
