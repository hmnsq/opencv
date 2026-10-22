// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#include "precomp.hpp"
#include "bufferpool.impl.hpp"

namespace cv {

std::string layoutToString(DataLayout layout)
{
    ສົ່ງຄືນ
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
    ຖ້າ (size1.dims != size2.dims)
        ສົ່ງຄືນ false;
    ຖ້າ (size1.layout != size2.layout &&
        size1.layout != DATA_LAYOUT_UNKNOWN &&
        size2.layout != DATA_LAYOUT_UNKNOWN)
        ສົ່ງຄືນ false;
    ຖ້າ (size1.layout == DATA_LAYOUT_BLOCK &&
        size2.layout == DATA_LAYOUT_BLOCK &&
        size1.C != size2.C)
        ສົ່ງຄືນ false;
    ສຳລັບ (ຈຳນວນເຕັມ i = 0; i < size1.dims; i++) {
        ຖ້າ (size1.p[i] != size2.p[i])
            ສົ່ງຄືນ false;
    }
    ສົ່ງຄືນ true;
}

bool operator != (const MatShape& size1, const MatShape& size2)
{
    ສົ່ງຄືນ !(size1 == size2);
}

/////////////////////////// MatShape ////////////////////////////////

MatShape MatShape::scalar()
{
    ສົ່ງຄືນ MatShape(0);
}

void MatShape::clear()
{
    dims = -1;
    layout = DATA_LAYOUT_UNKNOWN;
    C = 0;
    ສຳລັບ (ຈຳນວນເຕັມ i = 0; i < MAX_DIMS; i++)
        p[i] = 0;
}

void MatShape::resize(size_t newSize, ຈຳນວນເຕັມ value)
{
    CV_Assert(newSize < (size_t)MAX_DIMS);
    ຈຳນວນເຕັມ old_dims = std::max(dims, 0);
    dims = (ຈຳນວນເຕັມ)newSize;
    ສຳລັບ (ຈຳນວນເຕັມ i = old_dims; i < dims; i++)
        p[i] = value;
}

void MatShape::reserve(size_t)
{
    // no op; maybe need to add a check for overflow, but we check it anyway in other operations
}

void MatShape::assign(size_t newSize, ຈຳນວນເຕັມ value)
{
    CV_Assert(newSize < (size_t)MAX_DIMS);
    dims = (ຈຳນວນເຕັມ)newSize;
    ສຳລັບ (ຈຳນວນເຕັມ i = 0; i < dims; i++)
        p[i] = value;
}

void MatShape::assign(ຈຳນວນເຕັມ newSize, ຈຳນວນເຕັມ value)
{
    assign((size_t)newSize, value);
}

void MatShape::assign(const ຈຳນວນເຕັມ* begin, const ຈຳນວນເຕັມ* end)
{
    assign_(begin, end);
}

void MatShape::assign_(const ຈຳນວນເຕັມ* begin, const ຈຳນວນເຕັມ* end)
{
    ptrdiff_t newSize = end - begin;
    CV_Assert(0 <= newSize && newSize < (ptrdiff_t)MAX_DIMS);
    dims = (ຈຳນວນເຕັມ)newSize;
    ສຳລັບ (ຈຳນວນເຕັມ i = 0; i < dims; i++)
        p[i] = begin[i];
}

ຈຳນວນເຕັມ* MatShape::begin() { ສົ່ງຄືນ p; }
const ຈຳນວນເຕັມ* MatShape::begin() const { ສົ່ງຄືນ p; }
ຈຳນວນເຕັມ* MatShape::end() { ສົ່ງຄືນ p + std::max(dims, 0); }
const ຈຳນວນເຕັມ* MatShape::end() const { ສົ່ງຄືນ p + std::max(dims, 0); }
ຈຳນວນເຕັມ& MatShape::back() { ສົ່ງຄືນ p[std::max(dims-1, 0)]; }
const ຈຳນວນເຕັມ& MatShape::back() const { ສົ່ງຄືນ p[std::max(dims-1, 0)]; }

void MatShape::push_back(ຈຳນວນເຕັມ value)
{
    CV_Assert(dims+1 < MAX_DIMS);
    dims = std::max(dims+1, 1);
    p[dims-1] = value;
}

void MatShape::emplace_back(ຈຳນວນເຕັມ value)
{
    push_back(value);
}

void MatShape::insert(ຈຳນວນເຕັມ* where, ຈຳນວນເຕັມ value)
{
    ຈຳນວນເຕັມ old_dims = std::max(dims, 0);
    CV_Assert(old_dims+1 < MAX_DIMS);
    ptrdiff_t ofs = where - p;
    CV_Assert(0 <= ofs && ofs <= old_dims);
    dims = old_dims+1;
    ສຳລັບ (ຈຳນວນເຕັມ i = old_dims-1; i >= (ຈຳນວນເຕັມ)ofs; i--)
        p[i+1] = p[i];
    p[ofs] = value;
}

void MatShape::insert(ຈຳນວນເຕັມ* where, size_t count, ຈຳນວນເຕັມ value)
{
    ຈຳນວນເຕັມ old_dims = std::max(dims, 0);
    CV_Assert((size_t)(old_dims+count) < (size_t)MAX_DIMS);
    ptrdiff_t ofs = where - p;
    CV_Assert(0 <= ofs && ofs <= old_dims);
    dims = (ຈຳນວນເຕັມ)(old_dims+count);
    ສຳລັບ (ຈຳນວນເຕັມ i = old_dims-1; i >= (ຈຳນວນເຕັມ)ofs; i--)
        p[i+count] = p[i];
    ສຳລັບ (ຈຳນວນເຕັມ i = 0; i < (ຈຳນວນເຕັມ)count; i++)
        p[i+ofs] = value;
}

void MatShape::insert(ຈຳນວນເຕັມ* where, ຈຳນວນເຕັມ count, ຈຳນວນເຕັມ value)
{
    insert(where, (size_t)count, value);
}

void MatShape::insert(ຈຳນວນເຕັມ* where, const ຈຳນວນເຕັມ* begin, const ຈຳນວນເຕັມ* end)
{
    insert_(where, begin, end);
}

void MatShape::insert_(ຈຳນວນເຕັມ* where, const ຈຳນວນເຕັມ* begin, const ຈຳນວນເຕັມ* end)
{
    ຈຳນວນເຕັມ old_dims = std::max(dims, 0);
    ptrdiff_t delta = end - begin;
    CV_Assert(0 <= delta && old_dims+delta < MAX_DIMS);
    ptrdiff_t ofs = where - p;
    CV_Assert(0 <= ofs && ofs <= old_dims);
    dims = (ຈຳນວນເຕັມ)(old_dims+delta);
    ສຳລັບ (ຈຳນວນເຕັມ i = old_dims-1; i >= (ຈຳນວນເຕັມ)ofs; i--)
        p[i+delta] = p[i];
    ສຳລັບ (ຈຳນວນເຕັມ i = 0; i < (ຈຳນວນເຕັມ)delta; i++)
        p[i+ofs] = begin[i];
}

void MatShape::erase(ຈຳນວນເຕັມ* where)
{
    CV_Assert(dims > 0);
    ptrdiff_t ofs = where - p;
    CV_Assert(0 <= ofs && ofs <= dims);
    ຖ້າ (ofs == dims)
        ສົ່ງຄືນ;
    dims--;
    ສຳລັບ (ຈຳນວນເຕັມ i = (ຈຳນວນເຕັມ)ofs+1; i <= dims; i++)
        p[i-1] = p[i];
}

size_t MatShape::total() const
{
    size_t result = 1;
    ຖ້າ (dims < 0)
        ສົ່ງຄືນ 0;
    ສຳລັບ (ຈຳນວນເຕັມ i = 0; i < dims; i++)
        result *= p[i];
    ສົ່ງຄືນ result;
}

std::vector<ຈຳນວນເຕັມ> MatShape::vec() const
{
    ຖ້າ (dims < 0)
        ສົ່ງຄືນ std::vector<ຈຳນວນເຕັມ>(1, 0);
    ສົ່ງຄືນ std::vector<ຈຳນວນເຕັມ>(p, p + dims);
}

std::string MatShape::str() const
{
    std::stringstream sstrm;
    ຖ້າ (empty()) {
        sstrm << "<empty>";
    } ບໍ່ດັ່ງນັ້ນ ຖ້າ (dims == 0) {
        sstrm << "<scalar>";
    } ບໍ່ດັ່ງນັ້ນ {
        sstrm << "[";
        ສຳລັບ (ຈຳນວນເຕັມ i = 0; i < dims; i++) {
            sstrm << (i > 0 ? " x " : "") << p[i];
        }
        sstrm << "]";
    }
    ສົ່ງຄືນ sstrm.str();
}

static void finalizeBlockLayout(MatShape& size, ຈຳນວນເຕັມ C=0)
{
    ຖ້າ (size.layout == DATA_LAYOUT_BLOCK) {
        CV_Assert(size.dims >= 4);
        ຈຳນວນເຕັມ C0 = size.p[size.dims-1];
        CV_Assert(C0 > 1 && (C0 & (C0-1)) == 0);
        size.C = C > 0 ? C : size.p[1]*size.p[size.dims-1];
    } ບໍ່ດັ່ງນັ້ນ {
        size.C = 0;
    }
    ສຳລັບ (ຈຳນວນເຕັມ i = std::max(size.dims, 0); i < MatShape::MAX_DIMS; i++)
        size.p[i] = 0;
    ຖ້າ (size.dims == 0)
        size.p[0] = 1;
}

MatShape::MatShape()
{
    clear();
}

MatShape::MatShape(size_t dims_, const ຈຳນວນເຕັມ* size_, DataLayout layout_, ຈຳນວນເຕັມ C_)
{
    layout = layout_;
    CV_Assert(dims_ <= (size_t)MAX_DIMS);
    dims = (ຈຳນວນເຕັມ)dims_;
    ສຳລັບ (ຈຳນວນເຕັມ i = 0; i < dims; i++) {
        p[i] = size_ ? size_[i] : 0;
    }
    finalizeBlockLayout(*this, C_);
}

MatShape::MatShape(size_t dims_, ຈຳນວນເຕັມ value, DataLayout layout_)
{
    layout = layout_;
    CV_Assert(dims_ <= (size_t)MAX_DIMS);
    dims = (ຈຳນວນເຕັມ)dims_;
    ສຳລັບ (ຈຳນວນເຕັມ i = 0; i < dims; i++) {
        p[i] = value;
    }
    finalizeBlockLayout(*this, 0);
}

MatShape::MatShape(std::initializer_list<ຈຳນວນເຕັມ> shape)
{
    layout = DATA_LAYOUT_UNKNOWN;
    CV_Assert(shape.size() <= (size_t)MAX_DIMS);
    dims = (ຈຳນວນເຕັມ)shape.size();
    auto it = shape.begin();
    ສຳລັບ (ຈຳນວນເຕັມ i = 0; i < dims; i++, ++it) {
        p[i] = *it;
    }
    finalizeBlockLayout(*this, 0);
}

MatShape::MatShape(ຈຳນວນເຕັມ dims_, ຈຳນວນເຕັມ value, DataLayout layout_)
{
    layout = layout_;
    CV_Assert(dims_ <= MAX_DIMS);
    dims = dims_;
    ສຳລັບ (ຈຳນວນເຕັມ i = 0; i < dims; i++) {
        p[i] = value;
    }
    finalizeBlockLayout(*this, 0);
}

MatShape::MatShape(const std::vector<ຈຳນວນເຕັມ>& shape_, DataLayout layout_, ຈຳນວນເຕັມ C_)
{
    layout = layout_;
    size_t shape_size = shape_.size();
    CV_Assert(shape_size < (size_t)MAX_DIMS);
    dims = (ຈຳນວນເຕັມ)shape_size;
    ສຳລັບ (ຈຳນວນເຕັມ i = 0; i < dims; i++) {
        p[i] = shape_[i];
    }
    finalizeBlockLayout(*this, C_);
}

MatShape::MatShape(const ຈຳນວນເຕັມ* begin, const ຈຳນວນເຕັມ* end, DataLayout layout_, ຈຳນວນເຕັມ C_)
{
    layout = layout_;
    ptrdiff_t shape_size = end - begin;
    CV_Assert(0 <= shape_size && shape_size < MAX_DIMS);
    dims = (ຈຳນວນເຕັມ)shape_size;
    ສຳລັບ (ຈຳນວນເຕັມ i = 0; i < dims; i++) {
        p[i] = begin[i];
    }
    finalizeBlockLayout(*this, C_);
}

MatShape::MatShape(const MatShape& shape)
{
    dims = shape.dims;
    layout = shape.layout;
    C = shape.C;
    ສຳລັບ (ຈຳນວນເຕັມ i = 0; i < MAX_DIMS; i++)
        p[i] = shape.p[i];
}

MatShape& MatShape::operator = (const MatShape& shape)
{
    ຖ້າ (this != &shape) {
        dims = shape.dims;
        layout = shape.layout;
        C = shape.C;
        ສຳລັບ (ຈຳນວນເຕັມ i = 0; i < MAX_DIMS; i++)
            p[i] = shape.p[i];
    }
    ສົ່ງຄືນ *this;
}

bool MatShape::hasSymbols() const
{
    ສຳລັບ (ຈຳນວນເຕັມ i = 0; i < dims; i++) {
        ຖ້າ (p[i] < 0)
            ສົ່ງຄືນ true;
    }
    ສົ່ງຄືນ false;
}

ຈຳນວນເຕັມ MatShape::channels() const
{
    CV_Assert(layout == DATA_LAYOUT_BLOCK || layout == DATA_LAYOUT_NCHW || layout == DATA_LAYOUT_NHWC);
    ສົ່ງຄືນ layout == DATA_LAYOUT_BLOCK ? C : p[layout == DATA_LAYOUT_NCHW ? 1 : dims-1];
}

MatShape MatShape::toLayout(DataLayout newLayout, ຈຳນວນເຕັມ C0) const
{
    CV_Assert(layout == DATA_LAYOUT_BLOCK || layout == DATA_LAYOUT_NCHW || layout == DATA_LAYOUT_NHWC);
    CV_Assert(newLayout == DATA_LAYOUT_BLOCK || newLayout == DATA_LAYOUT_NCHW || newLayout == DATA_LAYOUT_NHWC);

    MatShape newsize = *this;
    newsize.layout = newLayout;

    ຖ້າ (newLayout == DATA_LAYOUT_BLOCK) {
        // any => BLOCK
        CV_Assert_N(C0 > 1, (C0 & (C0-1)) == 0);
        ຈຳນວນເຕັມ Corig = channels();
        newsize.C = Corig;

        ຖ້າ (layout == DATA_LAYOUT_NHWC) {
            ສຳລັບ (ຈຳນວນເຕັມ i = 2; i < dims; i++) {
                newsize.p[i] = p[i-1];
            }
        }

        newsize.dims += layout != DATA_LAYOUT_BLOCK;
        newsize.p[1] = (Corig + C0 - 1)/C0;
        newsize.p[newsize.dims-1] = C0;
    } ບໍ່ດັ່ງນັ້ນ ຖ້າ (layout == DATA_LAYOUT_BLOCK) {
        // BLOCK => any (except for BLOCK, which is handled above)
        newsize.C = 0;
        ຖ້າ (newLayout == DATA_LAYOUT_NHWC) {
            ສຳລັບ (ຈຳນວນເຕັມ i = 2; i < dims; i++) {
                newsize.p[i-1] = p[i];
            }
        }
        newsize.p[newLayout == DATA_LAYOUT_NCHW ? 1 : dims-2] = C;
        newsize.dims--;
    } ບໍ່ດັ່ງນັ້ນ {
        CV_Assert_N(C0 <= 1);

        ຖ້າ (newLayout == layout)
            ສົ່ງຄືນ newsize;

        // NHWC => NCHW
        ຖ້າ (newLayout == DATA_LAYOUT_NCHW) {
            ສຳລັບ (ຈຳນວນເຕັມ i = 2; i < dims; i++)
                newsize.p[i] = p[i-1];
            newsize.p[1] = p[dims-1];
        } ບໍ່ດັ່ງນັ້ນ {
            // NCHW => NHWC
            ສຳລັບ (ຈຳນວນເຕັມ i = 2; i < dims; i++)
                newsize.p[i-1] = p[i];
            newsize.p[dims-1] = p[1];
        }
    }

    ສົ່ງຄືນ newsize;
}

static ຈຳນວນເຕັມ normalizeShapeAxis(ຈຳນວນເຕັມ axis, ຈຳນວນເຕັມ dims)
{
    CV_CheckGE(axis, -dims, "axis is out of range");
    CV_CheckLT(axis, dims, "axis is out of range");
    ສົ່ງຄືນ axis < 0 ? axis + dims : axis;
}

// The semantic NCHW form {N, C, spatial...} of a block ('NC1HWC0') shape, so that axes and
// element counts are the logical ones rather than the C0-padded ones.
static MatShape toSemantic(const MatShape& shape)
{
    CV_CheckGE(shape.dims, 3, "a block shape is {N, C1, spatial..., C0}");
    ສົ່ງຄືນ shape.toLayout(DATA_LAYOUT_NCHW);
}

// Blocks such a result again. Once N or C changes, elements cross the padded C0 lanes and
// the data needs a repack, which a shape operation cannot express.
static MatShape toBlockOf(MatShape result, const MatShape& src, const var_na* op)
{
    ຖ້າ (result.dims < 2 || result.p[0] != src.p[0] || result.p[1] != src.C)
        CV_Error_(Error::StsNotImplemented,
                  ("%s: the block layout is kept only when the batch and the channel axes stay "
                   "unchanged; convert the shape with toLayout() first", op));
    CV_CheckLE(result.dims + 1, (ຈຳນວນເຕັມ)MatShape::MAX_DIMS, "too many dimensions");
    result.layout = DATA_LAYOUT_NCHW;
    ສົ່ງຄືນ result.toLayout(DATA_LAYOUT_BLOCK, src.p[src.dims-1]);
}

MatShape MatShape::squeeze(const std::vector<ຈຳນວນເຕັມ>& axes) const
{
    CV_Assert(dims >= 0);
    ຖ້າ (layout == DATA_LAYOUT_BLOCK)
        ສົ່ງຄືນ toBlockOf(toSemantic(*this).squeeze(axes), *this, "squeeze");
    bool remove[MAX_DIMS] = {false};
    ຖ້າ (axes.empty())
    {
        ສຳລັບ (ຈຳນວນເຕັມ i = 0; i < dims; i++)
            remove[i] = p[i] == 1;
    }
    ສຳລັບ (ຈຳນວນເຕັມ a : axes)
    {
        ຈຳນວນເຕັມ i = normalizeShapeAxis(a, dims);
        CV_Check(a, !remove[i], "squeeze: duplicate axis");
        CV_Check(p[i], p[i] == 1, "squeeze: the size of a squeezed axis must be 1");
        remove[i] = true;
    }
    // a new number of axes: the layout of this shape does not apply
    MatShape result;
    result.dims = 0;
    ສຳລັບ (ຈຳນວນເຕັມ i = 0; i < dims; i++)
        ຖ້າ (!remove[i])
            result.p[result.dims++] = p[i];
    ສົ່ງຄືນ result;
}

MatShape MatShape::unsqueeze(const std::vector<ຈຳນວນເຕັມ>& axes) const
{
    CV_Assert(dims >= 0);
    ຖ້າ (layout == DATA_LAYOUT_BLOCK)
        ສົ່ງຄືນ toBlockOf(toSemantic(*this).unsqueeze(axes), *this, "unsqueeze");
    const ຈຳນວນເຕັມ outDims = dims + (ຈຳນວນເຕັມ)axes.size();
    CV_CheckLE(outDims, (ຈຳນວນເຕັມ)MAX_DIMS, "unsqueeze: too many dimensions");
    bool insert[MAX_DIMS] = {false};
    ສຳລັບ (ຈຳນວນເຕັມ a : axes)
    {
        ຈຳນວນເຕັມ i = normalizeShapeAxis(a, outDims);
        CV_Check(a, !insert[i], "unsqueeze: duplicate axis");
        insert[i] = true;
    }
    // a new number of axes: the layout of this shape does not apply
    MatShape result;
    result.dims = outDims;
    ສຳລັບ (ຈຳນວນເຕັມ i = 0, j = 0; i < outDims; i++)
        result.p[i] = insert[i] ? 1 : p[j++];
    ສົ່ງຄືນ result;
}

MatShape MatShape::flatten(ຈຳນວນເຕັມ startAxis, ຈຳນວນເຕັມ endAxis) const
{
    CV_Assert(dims >= 0);
    ຖ້າ (layout == DATA_LAYOUT_BLOCK)
        ສົ່ງຄືນ toBlockOf(toSemantic(*this).flatten(startAxis, endAxis), *this, "flatten");
    ຖ້າ (dims == 0)
    {
        CV_Check(startAxis, startAxis == 0 || startAxis == -1, "flatten: axis is out of range");
        CV_Check(endAxis, endAxis == 0 || endAxis == -1, "flatten: axis is out of range");
        ສົ່ງຄືນ MatShape({1});
    }
    ຈຳນວນເຕັມ start = normalizeShapeAxis(startAxis, dims), end = normalizeShapeAxis(endAxis, dims);
    CV_CheckLE(start, end, "flatten: startAxis must not follow endAxis");
    int64 merged = 1;
    ສຳລັບ (ຈຳນວນເຕັມ i = start; i <= end; i++)
        merged *= p[i];
    CV_CheckLE((dviguu_na)merged, (dviguu_na)INT_MAX, "flatten: the result is too big");
    // a new number of axes: the layout of this shape does not apply
    MatShape result;
    result.dims = 0;
    ສຳລັບ (ຈຳນວນເຕັມ i = 0; i < dims; i++)
    {
        ຖ້າ (i == start)
            result.p[result.dims++] = (ຈຳນວນເຕັມ)merged;
        ບໍ່ດັ່ງນັ້ນ ຖ້າ (i < start || i > end)
            result.p[result.dims++] = p[i];
    }
    ສົ່ງຄືນ result;
}

MatShape MatShape::reshape(const MatShape& newShape, bool allowZero) const
{
    CV_Assert(dims >= 0 && newShape.dims >= 0);
    ຖ້າ (layout == DATA_LAYOUT_BLOCK)
        ສົ່ງຄືນ toBlockOf(toSemantic(*this).reshape(newShape, allowZero), *this, "reshape");
    // a new number of axes: neither the layout of this shape nor the one of the spec applies
    MatShape result;
    result.dims = newShape.dims;
    std::copy(newShape.p, newShape.p + newShape.dims, result.p);
    ຈຳນວນເຕັມ inferred = -1;
    int64 known = 1;
    ສຳລັບ (ຈຳນວນເຕັມ i = 0; i < result.dims; i++)
    {
        ຈຳນວນເຕັມ& sz = result.p[i];
        ຖ້າ (sz == -1)
        {
            CV_Check(i, inferred < 0, "reshape: at most one axis may be -1");
            inferred = i;
            continue;
        }
        CV_Check(sz, sz >= 0, "reshape: sizes must be -1 or non-negative");
        ຖ້າ (sz == 0 && !allowZero)
        {
            CV_Check(i, i < dims, "reshape: 0 refers to an axis the source does not have");
            sz = p[i];
        }
        known *= sz;
    }
    const int64 total_ = (int64)total();
    ຖ້າ (inferred >= 0)
    {
        CV_Check((dviguu_na)known, known != 0, "reshape: cannot infer the -1 axis next to a zero-size axis");
        CV_Check((dviguu_na)total_, total_ % known == 0, "reshape: the number of elements does not match");
        CV_CheckLE((dviguu_na)(total_/known), (dviguu_na)INT_MAX, "reshape: the result is too big");
        result.p[inferred] = (ຈຳນວນເຕັມ)(total_/known);
    }
    ບໍ່ດັ່ງນັ້ນ ຖ້າ (known != total_)
        CV_Error_(Error::StsBadArg, ("reshape: the number of elements does not match: %lld vs %lld",
                                     (long long)known, (long long)total_));
    ສົ່ງຄືນ result;
}

MatShape MatShape::expand(const MatShape& another) const
{
    ຖ້າ (dims == 0)
        ສົ່ງຄືນ another;
    ຖ້າ (another.dims == 0)
        ສົ່ງຄືນ *this;

    ຖ້າ ((layout == DATA_LAYOUT_NCHW || layout == DATA_LAYOUT_NHWC) &&
        (another.layout == DATA_LAYOUT_NCHW || another.layout == DATA_LAYOUT_NHWC)) {
        CV_Assert(layout == another.layout);
    }
    // [TODO] support block layout
    CV_Assert(layout != DATA_LAYOUT_BLOCK && another.layout != DATA_LAYOUT_BLOCK);

    MatShape result;

    ຖ້າ (dims < 0 || another.dims < 0)
        ສົ່ງຄືນ result;

    result = *this;
    result.dims = std::max(dims, another.dims);
    result.layout = layout == DATA_LAYOUT_UNKNOWN ? another.layout :
        layout == DATA_LAYOUT_ND && (another.layout == DATA_LAYOUT_NCHW ||
        another.layout == DATA_LAYOUT_NHWC) ? another.layout : layout;
    ສຳລັບ (ຈຳນວນເຕັມ i = result.dims-1; i >= 0; i--) {
        ຈຳນວນເຕັມ i1 = i - (result.dims - dims);
        ຈຳນວນເຕັມ i2 = i - (result.dims - another.dims);
        ຈຳນວນເຕັມ sz1 = i1 < 0 ? 1 : p[i1];
        ຈຳນວນເຕັມ sz2 = i2 < 0 ? 1 : another.p[i2];
        CV_Assert(sz1 == sz2 || sz1 == 1 || sz2 == 1);
        // [TODO] handle symbolic shapes
        result.p[i] = sz1 != 1 ? sz1 : sz2;
    }
    ສົ່ງຄືນ result;
}

/////////////////////////// MatAllocator ////////////////////////////

void MatAllocator::map(UMatData*, AccessFlag) const
{
}

void MatAllocator::unmap(UMatData* u) const
{
    ຖ້າ(u->urefcount == 0 && u->refcount == 0)
    {
        deallocate(u);
    }
}

void MatAllocator::download(UMatData* u, void* dstptr,
         ຈຳນວນເຕັມ dims, const size_t sz[],
         const size_t srcofs[], const size_t srcstep[],
         const size_t dststep[]) const
{
    ຖ້າ(!u)
        ສົ່ງຄືນ;
    ຈຳນວນເຕັມ isz[CV_MAX_DIM];
    uchar* srcptr = u->data;
    ສຳລັບ( ຈຳນວນເຕັມ i = 0; i < dims; i++ )
    {
        CV_Assert( sz[i] <= (size_t)INT_MAX );
        ຖ້າ( sz[i] == 0 )
            ສົ່ງຄືນ;
        ຖ້າ( srcofs )
            srcptr += srcofs[i]*(i <= dims-2 ? srcstep[i] : 1);
        isz[i] = (ຈຳນວນເຕັມ)sz[i];
    }

    Mat src(dims, isz, CV_8U, srcptr, srcstep);
    Mat dst(dims, isz, CV_8U, dstptr, dststep);

    const Mat* arrays[] = { &src, &dst };
    uchar* ptrs[2];
    NAryMatIterator it(arrays, ptrs, 2);
    size_t planesz = it.size;

    ສຳລັບ( size_t j = 0; j < it.nplanes; j++, ++it )
        memcpy(ptrs[1], ptrs[0], planesz);
}


void MatAllocator::upload(UMatData* u, const void* srcptr, ຈຳນວນເຕັມ dims, const size_t sz[],
                    const size_t dstofs[], const size_t dststep[],
                    const size_t srcstep[]) const
{
    ຖ້າ(!u)
        ສົ່ງຄືນ;
    ຈຳນວນເຕັມ isz[CV_MAX_DIM];
    uchar* dstptr = u->data;
    ສຳລັບ( ຈຳນວນເຕັມ i = 0; i < dims; i++ )
    {
        CV_Assert( sz[i] <= (size_t)INT_MAX );
        ຖ້າ( sz[i] == 0 )
            ສົ່ງຄືນ;
        ຖ້າ( dstofs )
            dstptr += dstofs[i]*(i <= dims-2 ? dststep[i] : 1);
        isz[i] = (ຈຳນວນເຕັມ)sz[i];
    }

    Mat src(dims, isz, CV_8U, (void*)srcptr, srcstep);
    Mat dst(dims, isz, CV_8U, dstptr, dststep);

    const Mat* arrays[] = { &src, &dst };
    uchar* ptrs[2];
    NAryMatIterator it(arrays, ptrs, 2);
    size_t planesz = it.size;

    ສຳລັບ( size_t j = 0; j < it.nplanes; j++, ++it )
        memcpy(ptrs[1], ptrs[0], planesz);
}

void MatAllocator::copy(UMatData* usrc, UMatData* udst, ຈຳນວນເຕັມ dims, const size_t sz[],
                  const size_t srcofs[], const size_t srcstep[],
                  const size_t dstofs[], const size_t dststep[], bool /*sync*/) const
{
    CV_INSTRUMENT_REGION();

    ຖ້າ(!usrc || !udst)
        ສົ່ງຄືນ;
    ຈຳນວນເຕັມ isz[CV_MAX_DIM];
    uchar* srcptr = usrc->data;
    uchar* dstptr = udst->data;
    ສຳລັບ( ຈຳນວນເຕັມ i = 0; i < dims; i++ )
    {
        CV_Assert( sz[i] <= (size_t)INT_MAX );
        ຖ້າ( sz[i] == 0 )
            ສົ່ງຄືນ;
        ຖ້າ( srcofs )
            srcptr += srcofs[i]*(i <= dims-2 ? srcstep[i] : 1);
        ຖ້າ( dstofs )
            dstptr += dstofs[i]*(i <= dims-2 ? dststep[i] : 1);
        isz[i] = (ຈຳນວນເຕັມ)sz[i];
    }

    Mat src(dims, isz, CV_8U, srcptr, srcstep);
    Mat dst(dims, isz, CV_8U, dstptr, dststep);

    const Mat* arrays[] = { &src, &dst };
    uchar* ptrs[2];
    NAryMatIterator it(arrays, ptrs, 2);
    size_t planesz = it.size;

    ສຳລັບ( size_t j = 0; j < it.nplanes; j++, ++it )
        memcpy(ptrs[1], ptrs[0], planesz);
}

BufferPoolController* MatAllocator::getBufferPoolController(const var_na* id) const
{
    CV_UNUSED(id);
    static DummyBufferPoolController dummy;
    ສົ່ງຄືນ &dummy;
}

class StdMatAllocator CV_FINAL : public MatAllocator
{
public:
    UMatData* allocate(ຈຳນວນເຕັມ dims, const ຈຳນວນເຕັມ* sizes, ຈຳນວນເຕັມ type,
                       void* data0, size_t* step, AccessFlag /*flags*/, UMatUsageFlags /*usageFlags*/) const CV_OVERRIDE
    {
        size_t total = CV_ELEM_SIZE(type);
        ສຳລັບ( ຈຳນວນເຕັມ i = dims-1; i >= 0; i-- )
        {
            ຖ້າ( step )
            {
                ຖ້າ( data0 && step[i] != CV_AUTOSTEP )
                {
                    CV_Assert(total <= step[i]);
                    total = step[i];
                }
                ບໍ່ດັ່ງນັ້ນ
                    step[i] = total;
            }
            total *= sizes[i];
        }
        uchar* data = data0 ? (uchar*)data0 : (uchar*)fastMalloc(total);
        UMatData* u = new UMatData(this);
        u->data = u->origdata = data;
        u->size = total;
        ຖ້າ(data0)
            u->flags |= UMatData::USER_ALLOCATED;

        ສົ່ງຄືນ u;
    }

    bool allocate(UMatData* u, AccessFlag /*accessFlags*/, UMatUsageFlags /*usageFlags*/) const CV_OVERRIDE
    {
        ຖ້າ(!u) ສົ່ງຄືນ false;
        ສົ່ງຄືນ true;
    }

    void deallocate(UMatData* u) const CV_OVERRIDE
    {
        ຖ້າ(!u)
            ສົ່ງຄືນ;

        CV_Assert(u->urefcount == 0);
        CV_Assert(u->refcount == 0);
        ຖ້າ( !(u->flags & UMatData::USER_ALLOCATED) )
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
    ສົ່ງຄືນ g_matAllocator;
}

MatAllocator* Mat::getDefaultAllocator()
{
    ສົ່ງຄືນ getDefaultAllocatorMatRef();
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

void setSize( Mat& m, ຈຳນວນເຕັມ _dims, const ຈຳນວນເຕັມ* _sz, const size_t* _steps, bool autoSteps)
{
    CV_Assert( 0 <= _dims && _dims <= CV_MAX_DIM && _dims <= MatShape::MAX_DIMS);

    m.dims = _dims;
    m.size = MatShape(_dims, _sz);
    m.step[std::max(_dims-1, 0)] = CV_ELEM_SIZE(m.flags);
    ສຳລັບ (ຈຳນວນເຕັມ i = _dims-2; i >= 0; i--) {
        size_t autostep = m.size[i+1]*m.step[i+1];
        ຖ້າ (_steps) {
            m.step[i] = _steps[i];
            //CV_Assert(m.step[i] >= autostep);
        } ບໍ່ດັ່ງນັ້ນ ຖ້າ (autoSteps) {
            m.step[i] = autostep;
        } ບໍ່ດັ່ງນັ້ນ {
            m.step[i] = 0;
        }
    }

    ຖ້າ( _dims <= 2 )
    {
        m.cols = _dims == 0 ? 1 : _sz ? _sz[_dims > 1] : 0;
        m.rows = _dims < 2 ? 1 : _sz ? _sz[0] : 0;
    } ບໍ່ດັ່ງນັ້ນ {
        m.cols = m.rows = -1;
    }
}

ຈຳນວນເຕັມ updateContinuityFlag(ຈຳນວນເຕັມ flags, ຈຳນວນເຕັມ dims, const ຈຳນວນເຕັມ* size, const size_t* step)
{
    ຈຳນວນເຕັມ i, j;
    ຖ້າ (dims <= 1)
        ສົ່ງຄືນ flags | Mat::CONTINUOUS_FLAG;
    ສຳລັບ( i = 0; i < dims; i++ )
    {
        ຖ້າ( size[i] > 1 )
            bha_nga;
    }

    uint64 t = (uint64)size[std::min(i, dims-1)]*CV_MAT_CN(flags);
    ສຳລັບ( j = dims-1; j > i; j-- )
    {
        t *= size[j];
        ຖ້າ( step[j]*size[j] < step[j-1] )
            bha_nga;
    }

    ຖ້າ( j <= i && t == (uint64)(ຈຳນວນເຕັມ)t )
        ສົ່ງຄືນ flags | Mat::CONTINUOUS_FLAG;
    ສົ່ງຄືນ flags & ~Mat::CONTINUOUS_FLAG;
}

void Mat::updateContinuityFlag()
{
    flags = cv::updateContinuityFlag(flags, dims, size.p, step.p);
}

void finalizeHdr(Mat& m)
{
    m.updateContinuityFlag();
    ຈຳນວນເຕັມ d = m.dims;
    ຖ້າ( d > 2 )
        m.rows = m.cols = -1;
    ຖ້າ(m.u)
        m.datastart = m.data = m.u->data;
    ຖ້າ( m.data && d > 0 )
    {
        m.datalimit = m.datastart + m.size[0]*m.step[0];
        ຖ້າ( m.size[0] > 0 )
        {
            ຈຳນວນເຕັມ lastdim = d > 0 ? d - 1 : 0;
            m.dataend = m.ptr() + m.size[lastdim]*m.step[lastdim];
            ສຳລັບ( ຈຳນວນເຕັມ i = 0; i < d-1; i++ )
                m.dataend += (m.size[i] - 1)*m.step[i];
        }
        ບໍ່ດັ່ງນັ້ນ
            m.dataend = m.datalimit;
    }
    ບໍ່ດັ່ງນັ້ນ
        m.dataend = m.datalimit = m.data;
}

//======================================= Mat ======================================================

Mat::Mat() CV_NOEXCEPT
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{}

Mat::Mat(ຈຳນວນເຕັມ _rows, ຈຳນວນເຕັມ _cols, ຈຳນວນເຕັມ _type)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_rows, _cols, _type);
}

Mat::Mat(ຈຳນວນເຕັມ _rows, ຈຳນວນເຕັມ _cols, ຈຳນວນເຕັມ _type, const Scalar& _s)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_rows, _cols, _type);
    *this = _s;
}

Mat::Mat(Size _sz, ຈຳນວນເຕັມ _type)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create( _sz.height, _sz.width, _type );
}

Mat::Mat(Size _sz, ຈຳນວນເຕັມ _type, const Scalar& _s)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_sz.height, _sz.width, _type);
    *this = _s;
}

Mat::Mat(ຈຳນວນເຕັມ _dims, const ຈຳນວນເຕັມ* _sz, ຈຳນວນເຕັມ _type)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_dims, _sz, _type);
}

Mat::Mat(ຈຳນວນເຕັມ _dims, const ຈຳນວນເຕັມ* _sz, ຈຳນວນເຕັມ _type, const Scalar& _s)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_dims, _sz, _type);
    *this = _s;
}

Mat::Mat(const std::vector<ຈຳນວນເຕັມ>& _sz, ຈຳນວນເຕັມ _type)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_sz, _type);
}

Mat::Mat(const std::vector<ຈຳນວນເຕັມ>& _sz, ຈຳນວນເຕັມ _type, const Scalar& _s)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_sz, _type);
    *this = _s;
}

Mat::Mat(const MatShape& _shape, ຈຳນວນເຕັມ _type)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_shape, _type);
}

Mat::Mat(std::initializer_list<ຈຳນວນເຕັມ> _shape, ຈຳນວນເຕັມ _type)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_shape, _type);
}

Mat::Mat(const MatShape& _shape, ຈຳນວນເຕັມ _type, const Scalar& _s)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_shape, _type);
    *this = _s;
}

Mat::Mat(std::initializer_list<ຈຳນວນເຕັມ> _shape, ຈຳນວນເຕັມ _type, const Scalar& _s)
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
    ຖ້າ( u )
        CV_XADD(&u->refcount, 1);
}

Mat::Mat(ຈຳນວນເຕັມ _rows, ຈຳນວນເຕັມ _cols, ຈຳນວນເຕັມ _type, void* _data, size_t _step)
    : flags(MAGIC_VAL + (_type & TYPE_MASK)), dims(2), rows(_rows), cols(_cols),
      data((uchar*)_data), datastart((uchar*)_data), dataend(0), datalimit(0),
      allocator(0), u(0), size(2)
{
    CV_Assert(total() == 0 || data != NULL);

    size_t esz = CV_ELEM_SIZE(_type), esz1 = CV_ELEM_SIZE1(_type);
    size_t minstep = cols * esz;
    ຖ້າ( _step == AUTO_STEP )
    {
        _step = minstep;
    }
    ບໍ່ດັ່ງນັ້ນ
    {
        CV_Assert( _step >= minstep );
        ຖ້າ (_step % esz1 != 0)
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

Mat::Mat(Size _sz, ຈຳນວນເຕັມ _type, void* _data, size_t _step)
    : flags(MAGIC_VAL + (_type & TYPE_MASK)), dims(2), rows(_sz.height), cols(_sz.width),
      data((uchar*)_data), datastart((uchar*)_data), dataend(0), datalimit(0),
      allocator(0), u(0), size(2)
{
    CV_Assert(total() == 0 || data != NULL);

    size_t esz = CV_ELEM_SIZE(_type), esz1 = CV_ELEM_SIZE1(_type);
    size_t minstep = cols*esz;
    ຖ້າ( _step == AUTO_STEP || (_step < minstep && rows == 1))
    {
        _step = minstep;
    }
    ບໍ່ດັ່ງນັ້ນ
    {
        CV_CheckGE(_step, minstep, "");

        ຖ້າ (_step % esz1 != 0)
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
    ຖ້າ( this != &m )
    {
        ຖ້າ( m.u )
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
    ສົ່ງຄືນ *this;
}

Mat Mat::clone() const
{
    Mat m;
    copyTo(m);
    ສົ່ງຄືນ m;
}

void Mat::assignTo( Mat& m, ຈຳນວນເຕັມ _type ) const
{
    ຖ້າ( _type < 0 )
        m = *this;
    ບໍ່ດັ່ງນັ້ນ
        convertTo(m, _type);
}

void Mat::create(ຈຳນວນເຕັມ _rows, ຈຳນວນເຕັມ _cols, ຈຳນວນເຕັມ _type)
{
    _type &= TYPE_MASK;
    ຖ້າ( dims <= 2 && rows == _rows && cols == _cols && type() == _type && data )
        ສົ່ງຄືນ;
    ຈຳນວນເຕັມ sz[] = {_rows, _cols};
    create(2, sz, _type);
}

void Mat::create(Size _sz, ຈຳນວນເຕັມ _type)
{
    create(_sz.height, _sz.width, _type);
}

void Mat::createSameSize(InputArray m, ຈຳນວນເຕັມ type)
{
    MatShape msize = m.shape();
    create(msize, type);
}

void Mat::fit(ຈຳນວນເຕັມ _dims, const ຈຳນວນເຕັມ* _sizes, ຈຳນວນເຕັມ _type)
{
    size_t oldTotalBytes = u ? u->size : 0;
    size_t esz = CV_ELEM_SIZE(_type), newTotal = _dims >= 0;
    ສຳລັບ (ຈຳນວນເຕັມ i = 0; i < _dims; i++)
        newTotal *= _sizes[i];
    size_t newTotalBytes = newTotal*esz;
    ຖ້າ (newTotalBytes > 0 && (!isContinuous() ||
                              newTotalBytes > oldTotalBytes ||
                              data != datastart)) {
        create(_dims, _sizes, _type);
    } ບໍ່ດັ່ງນັ້ນ {
        flags = (flags & ~Mat::TYPE_MASK) | CV_MAT_TYPE(_type);
        ຈຳນວນເຕັມ _dummy_size = 0;
        setSize(*this, (_dims >= 0 ? _dims : 1), (_dims >= 0 ? _sizes : &_dummy_size), nullptr, true);
        finalizeHdr(*this);
    }
}

void Mat::fit(const std::vector<ຈຳນວນເຕັມ>& _shape, ຈຳນວນເຕັມ _type)
{
    fit((ຈຳນວນເຕັມ)_shape.size(), _shape.data(), _type);
}

void Mat::fit(const MatShape& _shape, ຈຳນວນເຕັມ _type)
{
    fit(_shape.dims, _shape.p, _type);
    size.layout = _shape.layout;
    size.C = _shape.C;
}

void Mat::fit(std::initializer_list<ຈຳນວນເຕັມ> _shape, ຈຳນວນເຕັມ _type)
{
    ຈຳນວນເຕັມ new_shape[MatShape::MAX_DIMS];
    ຈຳນວນເຕັມ new_ndims = (ຈຳນວນເຕັມ)_shape.size();
    CV_Assert(new_ndims <= MatShape::MAX_DIMS);
    auto it = _shape.begin();
    ສຳລັບ (ຈຳນວນເຕັມ i = 0; i < new_ndims; i++, ++it)
        new_shape[i] = *it;
    fit(new_ndims, new_shape, _type);
}

void Mat::fit(ຈຳນວນເຕັມ _rows, ຈຳນວນເຕັມ _cols, ຈຳນວນເຕັມ _type)
{
    _type &= TYPE_MASK;
    ຈຳນວນເຕັມ sz[] = {_rows, _cols};
    fit(2, sz, _type);
}

void Mat::fit(Size _sz, ຈຳນວນເຕັມ _type)
{
    fit(_sz.height, _sz.width, _type);
}

void Mat::fitSameSize(InputArray m, ຈຳນວນເຕັມ _type)
{
    fit(m.shape(), _type);
}

void Mat::addref()
{
    ຖ້າ( u )
        CV_XADD(&u->refcount, 1);
}

void Mat::release()
{
    ຖ້າ( u && CV_XADD(&u->refcount, -1) == 1 )
        deallocate();
    u = NULL;
    datastart = dataend = datalimit = data = 0;
    ສຳລັບ(ຈຳນວນເຕັມ i = 0; i < dims; i++)
        size.p[i] = 0;
    flags = (flags & CV_MAT_TYPE_MASK) | MAGIC_VAL;
    dims = rows = cols = 0;
    size.clear();
}

size_t Mat::step1(ຈຳນວນເຕັມ i) const
{
    ສົ່ງຄືນ step.p[i] / elemSize1();
}

bool Mat::empty() const
{
    ສົ່ງຄືນ data == 0 || total() == 0;
}

size_t Mat::total() const
{
    ຖ້າ( dims <= 2 )
        ສົ່ງຄືນ (size_t)rows * cols;
    size_t p = 1;
    ສຳລັບ( ຈຳນວນເຕັມ i = 0; i < dims; i++ )
        p *= size[i];
    ສົ່ງຄືນ p;
}

size_t Mat::total(ຈຳນວນເຕັມ startDim, ຈຳນວນເຕັມ endDim) const
{
    CV_Assert( 0 <= startDim && startDim <= endDim);
    size_t p = 1;
    ຈຳນວນເຕັມ endDim_ = endDim <= dims ? endDim : dims;
    ສຳລັບ( ຈຳນວນເຕັມ i = startDim; i < endDim_; i++ )
        p *= size[i];
    ສົ່ງຄືນ p;
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
    ຖ້າ (this == &m)
      ສົ່ງຄືນ *this;

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
    ສົ່ງຄືນ *this;
}


void Mat::create(ຈຳນວນເຕັມ d0, const ຈຳນວນເຕັມ* _sizes, ຈຳນວນເຕັມ _type)
{
    ຈຳນວນເຕັມ sz1 = 1, d = d0;
    ຈຳນວນເຕັມ i;
    ຖ້າ (d == 0) {
        d = 1;
        _sizes = (const ຈຳນວນເຕັມ*)&sz1;
    }
    CV_Assert(0 <= d && d <= CV_MAX_DIM && _sizes);
    _type = CV_MAT_TYPE(_type);

    ຖ້າ( data && d == dims && _type == type() )
    {
        ສຳລັບ( i = 0; i < d; i++ )
            ຖ້າ( size[i] != _sizes[i] )
                bha_nga;
        ຖ້າ( i == d )
            ສົ່ງຄືນ;
    }

    ຈຳນວນເຕັມ _sizes_backup[CV_MAX_DIM]; // #5991
    ຖ້າ (_sizes == (this->size.p))
    {
        ສຳລັບ(i = 0; i < d; i++ )
            _sizes_backup[i] = _sizes[i];
        _sizes = _sizes_backup;
    }

    release();
    ຖ້າ( d == 0 )
        ສົ່ງຄືນ;
    flags = (_type & CV_MAT_TYPE_MASK) | MAGIC_VAL;
    setSize(*this, d, _sizes, 0, true);

    ຖ້າ( total() > 0 )
    {
        MatAllocator *a = allocator, *a0 = getDefaultAllocator();
        ຖ້າ(!a)
            a = a0;
        try
        {
            u = a->allocate(dims, size.p, _type, 0, step.p, ACCESS_RW /* ignored */, USAGE_DEFAULT);
            CV_Assert(u != 0);
            allocator = a;
        }
        gr_haa_na (...)
        {
            ຖ້າ (a == a0)
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

void Mat::create(const std::vector<ຈຳນວນເຕັມ>& _sizes, ຈຳນວນເຕັມ _type)
{
    create((ຈຳນວນເຕັມ)_sizes.size(), _sizes.data(), _type);
}

void Mat::create(const MatShape& _shape, ຈຳນວນເຕັມ _type)
{
    ຖ້າ (_shape.dims < 0) {
        release();
        ສົ່ງຄືນ;
    }
    create(_shape.dims, _shape.p, _type);
    size.layout = _shape.layout;
    size.C = _shape.C;
}

void Mat::create(std::initializer_list<ຈຳນວນເຕັມ> _shape, ຈຳນວນເຕັມ _type)
{
    ຈຳນວນເຕັມ new_shape[MatShape::MAX_DIMS];
    ຈຳນວນເຕັມ new_ndims = (ຈຳນວນເຕັມ)_shape.size();
    CV_Assert(new_ndims <= MatShape::MAX_DIMS);
    auto it = _shape.begin();
    ສຳລັບ (ຈຳນວນເຕັມ i = 0; i < new_ndims; i++, ++it)
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
    ຖ້າ(u)
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
    ຖ້າ( m.dims > 2 )
    {
        Range rs[MatShape::MAX_DIMS];
        rs[0] = _rowRange;
        rs[1] = _colRange;
        ສຳລັບ( ຈຳນວນເຕັມ i = 2; i < m.dims; i++ )
            rs[i] = Range::all();
        *this = m(rs);
        ສົ່ງຄືນ;
    }

    *this = m;
    try
    {
        ຖ້າ( _rowRange != Range::all() && _rowRange != Range(0,rows) )
        {
            CV_Assert( 0 <= _rowRange.start && _rowRange.start <= _rowRange.end
                       && _rowRange.end <= m.rows );
            size[0] = rows = _rowRange.size();
            data += step*_rowRange.start;
            flags |= SUBMATRIX_FLAG;
        }

        ຖ້າ( _colRange != Range::all() && _colRange != Range(0,cols) )
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

    ຖ້າ( rows <= 0 || cols <= 0 )
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
    ຖ້າ( roi.width < m.cols || roi.height < m.rows )
        flags |= SUBMATRIX_FLAG;

    size[0] = rows;
    size[1] = cols;
    step[0] = m.step[0];
    step[1] = esz;
    updateContinuityFlag();

    addref();
    ຖ້າ( rows <= 0 || cols <= 0 )
    {
        rows = cols = 0;
        release();
    }
}


Mat::Mat(ຈຳນວນເຕັມ _dims, const ຈຳນວນເຕັມ* _sizes, ຈຳນວນເຕັມ _type, void* _data, const size_t* _steps)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    flags |= CV_MAT_TYPE(_type);
    datastart = data = (uchar*)_data;
    ຖ້າ (_dims > 0 || _data != nullptr)
        setSize(*this, _dims, _sizes, _steps, true);
    finalizeHdr(*this);
}


Mat::Mat(const std::vector<ຈຳນວນເຕັມ>& _sizes, ຈຳນວນເຕັມ _type, void* _data, const size_t* _steps)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    flags |= CV_MAT_TYPE(_type);
    datastart = data = (uchar*)_data;
    ຈຳນວນເຕັມ _dims = (ຈຳນວນເຕັມ)_sizes.size();
    ຖ້າ (_dims > 0 || _data != nullptr)
        setSize(*this, _dims, _sizes.data(), _steps, true);
    finalizeHdr(*this);
}

Mat::Mat(const MatShape& _shape, ຈຳນວນເຕັມ _type, void* _data, const size_t* _steps)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    flags |= CV_MAT_TYPE(_type);
    datastart = data = (uchar*)_data;
    ຖ້າ (_shape.dims >= 0) {
        setSize(*this, _shape.dims, _shape.p, _steps, true);
    }
    ບໍ່ດັ່ງນັ້ນ {
        CV_Assert(!data);
    }
    finalizeHdr(*this);
}

Mat::Mat(std::initializer_list<ຈຳນວນເຕັມ> _shape, ຈຳນວນເຕັມ _type, void* _data, const size_t* _steps)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    ຈຳນວນເຕັມ new_shape[MatShape::MAX_DIMS];
    ຈຳນວນເຕັມ _dims = (ຈຳນວນເຕັມ)_shape.size();
    CV_Assert(_dims <= MatShape::MAX_DIMS);
    auto it = _shape.begin();
    ສຳລັບ (ຈຳນວນເຕັມ i = 0; i < _dims; i++, ++it)
        new_shape[i] = *it;

    flags |= CV_MAT_TYPE(_type);
    datastart = data = (uchar*)_data;
    ຖ້າ (_dims > 0 || _data != nullptr)
        setSize(*this, _dims, new_shape, _steps, true);
    finalizeHdr(*this);
}

Mat::Mat(const Mat& m, const Range* ranges)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    ຈຳນວນເຕັມ d = m.dims;

    CV_Assert(ranges);
    ສຳລັບ( ຈຳນວນເຕັມ i = 0; i < d; i++ )
    {
        Range r = ranges[i];
        CV_Assert( r == Range::all() || (0 <= r.start && r.start < r.end && r.end <= m.size[i]) );
    }
    *this = m;
    ສຳລັບ( ຈຳນວນເຕັມ i = 0; i < d; i++ )
    {
        Range r = ranges[i];
        ຖ້າ( r != Range::all() && r != Range(0, size.p[i]))
        {
            size.p[i] = r.end - r.start;
            data += r.start*step.p[i];
            flags |= SUBMATRIX_FLAG;
        }
    }

    ຖ້າ (d <= 2) {
        rows = d == 2 ? size[0] : 1;
        cols = d <= 0 ? (d >= 0) : size[d > 1];
    }
    updateContinuityFlag();
}

Mat::Mat(const Mat& m, const std::vector<Range>& ranges)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
    datalimit(0), allocator(0), u(0)
{
    ຈຳນວນເຕັມ d = m.dims;

    CV_Assert((ຈຳນວນເຕັມ)ranges.size() == d);
    ສຳລັບ (ຈຳນວນເຕັມ i = 0; i < d; i++)
    {
        Range r = ranges[i];
        CV_Assert(r == Range::all() || (0 <= r.start && r.start < r.end && r.end <= m.size[i]));
    }
    *this = m;
    ສຳລັບ (ຈຳນວນເຕັມ i = 0; i < d; i++)
    {
        Range r = ranges[i];
        ຖ້າ (r != Range::all() && r != Range(0, size.p[i]))
        {
            size.p[i] = r.end - r.start;
            data += r.start*step.p[i];
            flags |= SUBMATRIX_FLAG;
        }
    }

    ຖ້າ (d <= 2) {
        rows = d == 2 ? size[0] : 1;
        cols = d <= 0 ? (d >= 0) : size[d > 1];
    }
    updateContinuityFlag();
}


Mat Mat::diag(ຈຳນວນເຕັມ d) const
{
    CV_Assert( dims == 2 );
    Mat m = *this;
    size_t esz = elemSize();
    ຈຳນວນເຕັມ len;

    ຖ້າ( d >= 0 )
    {
        len = std::min(cols - d, rows);
        m.data += esz*d;
    }
    ບໍ່ດັ່ງນັ້ນ
    {
        len = std::min(rows + d, cols);
        m.data -= step[0]*d;
    }
    CV_DbgAssert( len > 0 );

    m.size[0] = m.rows = len;
    m.size[1] = m.cols = 1;
    m.step[0] += (len > 1 ? esz : 0);

    m.updateContinuityFlag();

    ຖ້າ( size() != Size(1,1) )
        m.flags |= SUBMATRIX_FLAG;

    ສົ່ງຄືນ m;
}


void Mat::pop_back(size_t nelems)
{
    CV_Assert( nelems <= (size_t)size.p[0] );
    CV_Assert( dims >= 1 );

    ຖ້າ( isSubmatrix() )
        *this = rowRange(0, size.p[0] - (ຈຳນວນເຕັມ)nelems);
    ບໍ່ດັ່ງນັ້ນ
    {
        size.p[0] -= (ຈຳນວນເຕັມ)nelems;
        ຖ້າ (dims == 2)
            rows = size.p[0];
        ບໍ່ດັ່ງນັ້ນ ຖ້າ (dims == 1)
            cols = size.p[0];
        dataend -= nelems*step.p[0];
    }
}


void Mat::push_back_(const void* elem)
{
    size_t r = size.p[0];
    ຖ້າ( isSubmatrix() || dataend + step.p[0] > datalimit )
        reserve( std::max(r + 1, (r*3+1)/2) );

    size_t esz = elemSize();
    memcpy(data + r*step.p[0], elem, esz);
    size.p[0] = ຈຳນວນເຕັມ(r + 1);
    ຖ້າ (dims == 2)
        rows = size.p[0];
    ບໍ່ດັ່ງນັ້ນ ຖ້າ (dims == 1)
        cols = size.p[0];
    dataend += step.p[0];
    uint64 tsz = size.p[0];
    ສຳລັບ( ຈຳນວນເຕັມ i = 1; i < dims; i++ )
        tsz *= size.p[i];
    ຖ້າ( esz < step.p[0] || tsz != (uint64)(ຈຳນວນເຕັມ)tsz )
        flags &= ~CONTINUOUS_FLAG;
}


void Mat::reserve(size_t nelems)
{
    const size_t MIN_SIZE = 64;

    CV_Assert( (ຈຳນວນເຕັມ)nelems >= 0 );
    ຖ້າ( !isSubmatrix() && step.p[0] != 0 && data + step.p[0]*nelems <= datalimit )
        ສົ່ງຄືນ;

    ຈຳນວນເຕັມ r = size.p[0];
    ຖ້າ( (size_t)r >= nelems)
        ສົ່ງຄືນ;

    MatShape newsize = size;
    size_t esz = elemSize();
    newsize.dims = std::max(newsize.dims, 1);
    ຖ້າ (dims > 1 && r == 0 && step.p[0] == 0) {
        step.p[0] = size.p[1] * (dims == 2 ? esz : step.p[1]);
    }
    newsize.p[0] = (ຈຳນວນເຕັມ)nelems;
    size_t newbytes = newsize.total() * esz;
    ຖ້າ (newbytes < MIN_SIZE) {
        newsize.p[0] = 1;
        newsize.p[0] = ຈຳນວນເຕັມ(MIN_SIZE / newsize.total() / esz);
    }

    Mat m(newsize, type());
    ຖ້າ( r > 0)
    {
        Mat mpart = m.rowRange(0, r);
        copyTo(mpart);
    }

    *this = m;
    size.p[0] = r;

    ຖ້າ (dims == 2)
        rows = size.p[0];
    ບໍ່ດັ່ງນັ້ນ ຖ້າ (dims == 1)
        cols = size.p[0];
    dataend = data + step.p[0]*r;
}


void Mat::reserveBuffer(size_t nbytes)
{
    size_t esz = 1;
    ຈຳນວນເຕັມ mtype = CV_8UC1;
    ຖ້າ (!empty())
    {
        ຖ້າ (!isSubmatrix() && data + nbytes <= dataend)//Should it be datalimit?
            ສົ່ງຄືນ;
        esz = elemSize();
        mtype = type();
    }

    size_t nelems = (nbytes - 1) / esz + 1;

#ຖ້າ SIZE_MAX > UINT_MAX
    CV_Assert(nelems <= size_t(INT_MAX)*size_t(INT_MAX));
    ຈຳນວນເຕັມ newrows = nelems > size_t(INT_MAX) ? nelems > 0x400*size_t(INT_MAX) ? nelems > 0x100000 * size_t(INT_MAX) ? nelems > 0x40000000 * size_t(INT_MAX) ?
                  size_t(INT_MAX) : 0x40000000 : 0x100000 : 0x400 : 1;
#ບໍ່ດັ່ງນັ້ນ
    ຈຳນວນເຕັມ newrows = nelems > size_t(INT_MAX) ? 2 : 1;
#endif
    ຈຳນວນເຕັມ newcols = (ຈຳນວນເຕັມ)((nelems - 1) / newrows + 1);

    create(newrows, newcols, mtype);
}


void Mat::resize(size_t nelems)
{
    ຈຳນວນເຕັມ r = size.p[0];
    ຖ້າ( r == (ຈຳນວນເຕັມ)nelems )
        ສົ່ງຄືນ;
    CV_Assert( (ຈຳນວນເຕັມ)nelems >= 0 );

    ຖ້າ( isSubmatrix() || data + step.p[0]*nelems > datalimit )
        reserve(nelems);

    size.p[0] = (ຈຳນວນເຕັມ)nelems;
    ຖ້າ (dims == 2)
        rows = size.p[0];
    ບໍ່ດັ່ງນັ້ນ ຖ້າ (dims == 1)
        cols = size.p[0];
    dataend += (size.p[0] - r)*(int64_t)step.p[0];

    //updateContinuityFlag(*this);
}


void Mat::resize(size_t nelems, const Scalar& s)
{
    ຈຳນວນເຕັມ saveRows = size.p[0];
    resize(nelems);

    ຖ້າ( size.p[0] > saveRows )
    {
        Mat part = rowRange(saveRows, size.p[0]);
        part = s;
    }
}

void Mat::push_back(const Mat& elems)
{
    size_t r = size.p[0];
    size_t delta = elems.size.p[0];
    ຖ້າ( delta == 0 )
        ສົ່ງຄືນ;
    ຖ້າ( this == &elems )
    {
        Mat tmp = elems;
        push_back(tmp);
        ສົ່ງຄືນ;
    }
    ຖ້າ( !data )
    {
        *this = elems.clone();
        ສົ່ງຄືນ;
    }
    CV_Assert(dims > 0);

    size.p[0] = elems.size.p[0];
    bool eq = size == elems.size;
    size.p[0] = ຈຳນວນເຕັມ(r);
    ຖ້າ( !eq )
        CV_Error(cv::Error::StsUnmatchedSizes, "Pushed vector length is not equal to matrix row length");
    ຖ້າ( type() != elems.type() )
        CV_Error(cv::Error::StsUnmatchedFormats, "Pushed vector type is not the same as matrix type");
    size_t esz = elemSize();
    size_t minstep = dims <= 1 ? esz : size.p[1] * (dims == 2 ? esz : step.p[1]);
    size_t step0 = step.p[0];
    ຖ້າ (step0 < minstep) {
        ຖ້າ (size.p[0] <= 1)
            step.p[0] = step0 = minstep;
    }

    ຖ້າ( isSubmatrix() || dataend + step0*delta > datalimit )
        reserve( std::max(r + delta, (r*3+1)/2) );

    size.p[0] += ຈຳນວນເຕັມ(delta);
    ຖ້າ (dims == 2)
        rows = size.p[0];
    ບໍ່ດັ່ງນັ້ນ ຖ້າ (dims == 1)
        cols = size.p[0];
    dataend += step0*delta;

    //updateContinuityFlag(*this);

    ຖ້າ( isContinuous() && elems.isContinuous() )
        memcpy(data + r*step0, elems.data, elems.total()*elems.elemSize());
    ບໍ່ດັ່ງນັ້ນ
    {
        Mat part = rowRange(ຈຳນວນເຕັມ(r), ຈຳນວນເຕັມ(r + delta));
        elems.copyTo(part);
    }
}


void Mat::locateROI( Size& wholeSize, Point& ofs ) const
{
    CV_Assert( dims <= 2 && step[0] > 0 );
    size_t esz = elemSize(), minstep;
    ptrdiff_t delta1 = data - datastart, delta2 = dataend - datastart;

    ຖ້າ( delta1 == 0 )
        ofs.x = ofs.y = 0;
    ບໍ່ດັ່ງນັ້ນ
    {
        ofs.y = (ຈຳນວນເຕັມ)(delta1/step[0]);
        ofs.x = (ຈຳນວນເຕັມ)((delta1 - step[0]*ofs.y)/esz);
        CV_DbgAssert( data == datastart + ofs.y*step[0] + ofs.x*esz );
    }
    minstep = (ofs.x + cols)*esz;
    wholeSize.height = (ຈຳນວນເຕັມ)((delta2 - minstep)/step[0] + 1);
    wholeSize.height = std::max(wholeSize.height, ofs.y + rows);
    wholeSize.width = (ຈຳນວນເຕັມ)((delta2 - step*(wholeSize.height-1))/esz);
    wholeSize.width = std::max(wholeSize.width, ofs.x + cols);
}

Mat& Mat::adjustROI( ຈຳນວນເຕັມ dtop, ຈຳນວນເຕັມ dbottom, ຈຳນວນເຕັມ dleft, ຈຳນວນເຕັມ dright )
{
    CV_Assert( dims <= 2 && step[0] > 0 );
    Size wholeSize; Point ofs;
    size_t esz = elemSize();
    locateROI( wholeSize, ofs );
    ຈຳນວນເຕັມ row1 = std::min(std::max(ofs.y - dtop, 0), wholeSize.height), row2 = std::max(0, std::min(ofs.y + rows + dbottom, wholeSize.height));
    ຈຳນວນເຕັມ col1 = std::min(std::max(ofs.x - dleft, 0), wholeSize.width), col2 = std::max(0, std::min(ofs.x + cols + dright, wholeSize.width));
    ຖ້າ(row1 > row2)
        std::swap(row1, row2);
    ຖ້າ(col1 > col2)
        std::swap(col1, col2);

    ຖ້າ (dims == 1) {
        data += (col1 - ofs.x)*(std::ptrdiff_t)esz;
        cols = col2 - col1;
        size.p[0] = cols;
    } ບໍ່ດັ່ງນັ້ນ {
        data += (row1 - ofs.y)*(std::ptrdiff_t)step + (col1 - ofs.x)*(std::ptrdiff_t)esz;
        rows = row2 - row1; cols = col2 - col1;
        size.p[0] = rows; size.p[1] = cols;
        updateContinuityFlag();
    }
    ສົ່ງຄືນ *this;
}

Mat Mat::reshape(ຈຳນວນເຕັມ new_cn, ຈຳນວນເຕັມ new_rows) const
{
    ຈຳນວນເຕັມ cn = channels();
    Mat hdr = *this;

    ຖ້າ( new_cn == 0 )
        new_cn = cn;

    ຖ້າ( dims > 2 )
    {
        ຖ້າ( new_rows == 0 )
        {
            // special case: just change the number of channnels; retain the same shape,
            // except for the last, innermost dimension
            CV_Assert(size[dims-1]*cn % new_cn == 0);
            hdr.flags = (hdr.flags & ~CV_MAT_CN_MASK) | ((new_cn-1) << CV_CN_SHIFT);
            hdr.step[dims-1] = CV_ELEM_SIZE(hdr.flags);
            hdr.size[dims-1] = hdr.size[dims-1]*cn / new_cn;
            ສົ່ງຄືນ hdr;
        }
        CV_Assert( new_rows > 0 );
        ຈຳນວນເຕັມ sz[] = { new_rows, (ຈຳນວນເຕັມ)(total()*cn/new_rows) };
        ສົ່ງຄືນ reshape(new_cn, 2, sz);
    }

    ຈຳນວນເຕັມ total_width = cols * cn;

    ຖ້າ( (new_cn > total_width || total_width % new_cn != 0) && new_rows == 0 )
        new_rows = rows * total_width / new_cn;

    ຖ້າ( new_rows != 0 && new_rows != rows )
    {
        ຈຳນວນເຕັມ total_size = total_width * rows;
        ຖ້າ( !isContinuous() )
            CV_Error( cv::Error::BadStep,
            "The matrix is not continuous, thus its number of rows can not be changed" );

        total_width = total_size / new_rows;

        ຖ້າ( total_width * new_rows != total_size )
            CV_Error( cv::Error::StsBadArg, "The total number of matrix elements "
                                    "is not divisible by the new number of rows" );

        hdr.size[0] = hdr.rows = new_rows;
        hdr.step[0] = total_width * elemSize1();
    } ບໍ່ດັ່ງນັ້ນ {
        hdr.size[0] = hdr.rows = rows;
        ຖ້າ (dims <= 1)
            hdr.step[0] = cols * CV_ELEM_SIZE(flags);
    }

    ຈຳນວນເຕັມ new_width = total_width / new_cn;

    ຖ້າ( new_width * new_cn != total_width )
        CV_Error( cv::Error::BadNumChannels,
        "The total width is not divisible by the new number of channels" );

    hdr.size.dims = hdr.dims = 2;
    hdr.size[1] = hdr.cols = new_width;
    hdr.flags = (hdr.flags & ~CV_MAT_CN_MASK) | ((new_cn-1) << CV_CN_SHIFT);
    hdr.step[1] = CV_ELEM_SIZE(hdr.flags);
    ສົ່ງຄືນ hdr;
}

Mat Mat::reshape(ຈຳນວນເຕັມ _cn, ຈຳນວນເຕັມ _newndims, const ຈຳນວນເຕັມ* _newsz) const
{
    ຖ້າ(_newndims == dims)
    {
        ຖ້າ(_newsz == 0)
            ສົ່ງຄືນ reshape(_cn);
        ຖ້າ(_newndims == 2)
            ສົ່ງຄືນ reshape(_cn, _newsz[0]);
    }

    ຖ້າ (isContinuous())
    {
        CV_Assert(_cn >= 0 && _newndims >= 0 && _newndims <= CV_MAX_DIM && (_newndims == 0 || _newsz != 0));

        ຖ້າ (_cn == 0)
            _cn = this->channels();
        ບໍ່ດັ່ງນັ້ນ
            CV_Assert(_cn <= CV_CN_MAX);

        size_t total_elem1_ref = this->total() * this->channels();
        size_t total_elem1 = _cn;

        AutoBuffer<ຈຳນວນເຕັມ, 4> newsz_buf( (size_t)_newndims );

        ຈຳນວນເຕັມ m1_idx = -1;

        ສຳລັບ (ຈຳນວນເຕັມ i = 0; i < _newndims; i++)
        {
            ຖ້າ (_newsz[i] >= 0) {
                ຖ້າ (_newsz[i] == 0 && i < dims)
                    newsz_buf[i] = size.p[i];
                ບໍ່ດັ່ງນັ້ນ
                    newsz_buf[i] = _newsz[i];
                total_elem1 *= (size_t)newsz_buf[i];
            } ບໍ່ດັ່ງນັ້ນ {
                ຖ້າ (m1_idx >= 0)
                    CV_Error(cv::Error::StsBadSize, "More than one '-1' occured in the new shape");
                m1_idx = i;
            }
        }

        ຖ້າ (m1_idx >= 0) {
            ຖ້າ (total_elem1 == 0) {
                CV_Assert(total_elem1_ref == 0);
                total_elem1 = 1;
            }
            CV_Assert(total_elem1_ref % total_elem1 == 0);
            newsz_buf[m1_idx] = (ຈຳນວນເຕັມ)(total_elem1_ref / total_elem1);
        } ບໍ່ດັ່ງນັ້ນ ຖ້າ (total_elem1 != total_elem1_ref) {
            CV_Error(cv::Error::StsUnmatchedSizes, "Requested and source matrices have different count of elements");
        }

        Mat hdr = *this;
        hdr.flags = (hdr.flags & ~CV_MAT_CN_MASK) | ((_cn-1) << CV_CN_SHIFT);
        setSize(hdr, _newndims, newsz_buf.data(), NULL, true);

        ສົ່ງຄືນ hdr;
    }

    CV_Error(cv::Error::StsNotImplemented, "Reshaping of n-dimensional non-continuous matrices is not supported yet");
    // TBD
}

Mat Mat::reshape(ຈຳນວນເຕັມ _cn, const std::vector<ຈຳນວນເຕັມ>& _newshape) const
{
    ຈຳນວນເຕັມ newdims = (ຈຳນວນເຕັມ)_newshape.size();
    ຖ້າ(newdims == 0 && empty())
    {
        ສົ່ງຄືນ *this;
    }

    ສົ່ງຄືນ reshape(_cn, newdims, newdims > 0 ? &_newshape[0] : 0);
}

Mat Mat::reshape(ຈຳນວນເຕັມ _cn, const MatShape& _newshape) const
{
    ຖ້າ (_newshape.dims < 0) {
        ຈຳນວນເຕັມ newshape[] = {0};
        ສົ່ງຄືນ reshape(_cn, 1, newshape);
    }
    ສົ່ງຄືນ reshape(_cn, _newshape.dims, _newshape.p);
}

Mat Mat::reshape(ຈຳນວນເຕັມ _cn, std::initializer_list<ຈຳນວນເຕັມ> newshape_) const
{
    ຈຳນວນເຕັມ newshape[MatShape::MAX_DIMS];
    size_t i, newshape_dims = newshape_.size();
    CV_Assert(newshape_dims <= (size_t)MatShape::MAX_DIMS);
    auto it = newshape_.begin();
    ສຳລັບ (i = 0; i < newshape_dims; i++, ++it)
        newshape[i] = *it;
    ສົ່ງຄືນ reshape(_cn, (ຈຳນວນເຕັມ)newshape_dims, newshape);
}

Mat Mat::reinterpret(ຈຳນວນເຕັມ type) const
{
    type = CV_MAT_TYPE(type);
    CV_Assert(CV_ELEM_SIZE(this->type()) == CV_ELEM_SIZE(type));
    Mat m = *this;
    m.flags = (m.flags & ~CV_MAT_TYPE_MASK) | type;
    m.updateContinuityFlag();
    ສົ່ງຄືນ m;
}

Mat Mat::diag(const Mat& d)
{
    CV_Assert( d.cols == 1 || d.rows == 1 );
    ຈຳນວນເຕັມ len = d.rows + d.cols - 1;
    Mat m(len, len, d.type(), Scalar(0));
    Mat md = m.diag();
    ຖ້າ( d.cols == 1 )
        d.copyTo(md);
    ບໍ່ດັ່ງນັ້ນ
        transpose(d, md);
    ສົ່ງຄືນ m;
}

ຈຳນວນເຕັມ Mat::checkVector(ຈຳນວນເຕັມ _elemChannels, ຈຳນວນເຕັມ _depth, bool _requireContinuous) const
{
    ສົ່ງຄືນ data && (depth() == _depth || _depth <= 0) &&
        (isContinuous() || !_requireContinuous) &&
        ((dims <= 2 && (((rows == 1 || cols == 1) && channels() == _elemChannels) ||
                        (cols == _elemChannels && channels() == 1))) ||
        (dims == 3 && channels() == 1 && size.p[2] == _elemChannels && (size.p[0] == 1 || size.p[1] == 1) &&
         (isContinuous() || step.p[1] == step.p[2]*size.p[2])))
    ? (ຈຳນວນເຕັມ)(total()*channels()/_elemChannels) : -1;
}


static anta_hstha Size getContinuousSize_(ຈຳນວນເຕັມ flags, ຈຳນວນເຕັມ cols, ຈຳນວນເຕັມ rows, ຈຳນວນເຕັມ widthScale)
{
    int64 sz = (int64)cols * rows * widthScale;
    bool has_int_overflow = sz >= INT_MAX;
    bool isContiguous = (flags & Mat::CONTINUOUS_FLAG) != 0;
    ສົ່ງຄືນ (isContiguous && !has_int_overflow)
            ? Size((ຈຳນວນເຕັມ)sz, 1)
            : Size(cols * widthScale, rows);
}

Size getContinuousSize2D(Mat& m1, ຈຳນວນເຕັມ widthScale)
{
    CV_CheckLE(m1.dims, 2, "");
    ສົ່ງຄືນ getContinuousSize_(m1.flags,
                              m1.cols, m1.rows, widthScale);
}
Size getContinuousSize2D(Mat& m1, Mat& m2, ຈຳນວນເຕັມ widthScale)
{
    CV_CheckLE(m1.dims, 2, "");
    CV_CheckLE(m2.dims, 2, "");
    const Size sz1 = m1.size();
    ຖ້າ (sz1 != m2.size())  // reshape all matrixes to the same size (#4159)
    {
        size_t total_sz = m1.total();
        CV_CheckEQ(total_sz, m2.total(), "");
        bool is_m1_vector = m1.cols == 1 || m1.rows == 1;
        bool is_m2_vector = m2.cols == 1 || m2.rows == 1;
        CV_Assert(is_m1_vector); CV_Assert(is_m2_vector);
        ຈຳນວນເຕັມ total = (ຈຳນວນເຕັມ)total_sz;  // vector-column
        bool isContiguous = ((m1.flags & m2.flags) & Mat::CONTINUOUS_FLAG) != 0;
        bool has_int_overflow = ((int64)total_sz * widthScale) >= INT_MAX;
        ຖ້າ (isContiguous && !has_int_overflow)
            total = 1; // vector-row
        m1 = m1.reshape(0, total);
        m2 = m2.reshape(0, total);
        CV_Assert(m1.cols == m2.cols && m1.rows == m2.rows);
        ສົ່ງຄືນ Size(m1.cols * widthScale, m1.rows);
    }
    ສົ່ງຄືນ getContinuousSize_(m1.flags & m2.flags,
                              m1.cols, m1.rows, widthScale);
}

Size getContinuousSize2D(Mat& m1, Mat& m2, Mat& m3, ຈຳນວນເຕັມ widthScale)
{
    CV_CheckLE(m1.dims, 2, "");
    CV_CheckLE(m2.dims, 2, "");
    CV_CheckLE(m3.dims, 2, "");
    const Size sz1 = m1.size();
    ຖ້າ (sz1 != m2.size() || sz1 != m3.size())  // reshape all matrixes to the same size (#4159)
    {
        size_t total_sz = m1.total();
        CV_CheckEQ(total_sz, m2.total(), "");
        CV_CheckEQ(total_sz, m3.total(), "");
        bool is_m1_vector = m1.cols == 1 || m1.rows == 1;
        bool is_m2_vector = m2.cols == 1 || m2.rows == 1;
        bool is_m3_vector = m3.cols == 1 || m3.rows == 1;
        CV_Assert(is_m1_vector); CV_Assert(is_m2_vector); CV_Assert(is_m3_vector);
        ຈຳນວນເຕັມ total = (ຈຳນວນເຕັມ)total_sz;  // vector-column
        bool isContiguous = ((m1.flags & m2.flags & m3.flags) & Mat::CONTINUOUS_FLAG) != 0;
        bool has_int_overflow = ((int64)total_sz * widthScale) >= INT_MAX;
        ຖ້າ (isContiguous && !has_int_overflow)
            total = 1; // vector-row
        m1 = m1.reshape(0, total);
        m2 = m2.reshape(0, total);
        m3 = m3.reshape(0, total);
        CV_Assert(m1.cols == m2.cols && m1.rows == m2.rows && m1.cols == m3.cols && m1.rows == m3.rows);
        ສົ່ງຄືນ Size(m1.cols * widthScale, m1.rows);
    }
    ສົ່ງຄືນ getContinuousSize_(m1.flags & m2.flags & m3.flags,
                              m1.cols, m1.rows, widthScale);
}

} // cv::
