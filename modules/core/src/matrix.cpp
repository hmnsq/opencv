// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#include "precomp.hpp"
#include "bufferpool.impl.hpp"

namespace cv {

std::string layoutToString(DataLayout layout)
{
    върни
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
    ако (size1.dims != size2.dims)
        върни false;
    ако (size1.layout != size2.layout &&
        size1.layout != DATA_LAYOUT_UNKNOWN &&
        size2.layout != DATA_LAYOUT_UNKNOWN)
        върни false;
    ако (size1.layout == DATA_LAYOUT_BLOCK &&
        size2.layout == DATA_LAYOUT_BLOCK &&
        size1.C != size2.C)
        върни false;
    за (цяло i = 0; i < size1.dims; i++) {
        ако (size1.p[i] != size2.p[i])
            върни false;
    }
    върни true;
}

bool operator != (const MatShape& size1, const MatShape& size2)
{
    върни !(size1 == size2);
}

/////////////////////////// MatShape ////////////////////////////////

MatShape MatShape::scalar()
{
    върни MatShape(0);
}

void MatShape::clear()
{
    dims = -1;
    layout = DATA_LAYOUT_UNKNOWN;
    C = 0;
    за (цяло i = 0; i < MAX_DIMS; i++)
        p[i] = 0;
}

void MatShape::resize(size_t newSize, цяло value)
{
    CV_Assert(newSize < (size_t)MAX_DIMS);
    цяло old_dims = std::max(dims, 0);
    dims = (цяло)newSize;
    за (цяло i = old_dims; i < dims; i++)
        p[i] = value;
}

void MatShape::reserve(size_t)
{
    // no op; maybe need to add a check for overflow, but we check it anyway in other operations
}

void MatShape::assign(size_t newSize, цяло value)
{
    CV_Assert(newSize < (size_t)MAX_DIMS);
    dims = (цяло)newSize;
    за (цяло i = 0; i < dims; i++)
        p[i] = value;
}

void MatShape::assign(цяло newSize, цяло value)
{
    assign((size_t)newSize, value);
}

void MatShape::assign(const цяло* begin, const цяло* end)
{
    assign_(begin, end);
}

void MatShape::assign_(const цяло* begin, const цяло* end)
{
    ptrdiff_t newSize = end - begin;
    CV_Assert(0 <= newSize && newSize < (ptrdiff_t)MAX_DIMS);
    dims = (цяло)newSize;
    за (цяло i = 0; i < dims; i++)
        p[i] = begin[i];
}

цяло* MatShape::begin() { върни p; }
const цяло* MatShape::begin() const { върни p; }
цяло* MatShape::end() { върни p + std::max(dims, 0); }
const цяло* MatShape::end() const { върни p + std::max(dims, 0); }
цяло& MatShape::back() { върни p[std::max(dims-1, 0)]; }
const цяло& MatShape::back() const { върни p[std::max(dims-1, 0)]; }

void MatShape::push_back(цяло value)
{
    CV_Assert(dims+1 < MAX_DIMS);
    dims = std::max(dims+1, 1);
    p[dims-1] = value;
}

void MatShape::emplace_back(цяло value)
{
    push_back(value);
}

void MatShape::insert(цяло* where, цяло value)
{
    цяло old_dims = std::max(dims, 0);
    CV_Assert(old_dims+1 < MAX_DIMS);
    ptrdiff_t ofs = where - p;
    CV_Assert(0 <= ofs && ofs <= old_dims);
    dims = old_dims+1;
    за (цяло i = old_dims-1; i >= (цяло)ofs; i--)
        p[i+1] = p[i];
    p[ofs] = value;
}

void MatShape::insert(цяло* where, size_t count, цяло value)
{
    цяло old_dims = std::max(dims, 0);
    CV_Assert((size_t)(old_dims+count) < (size_t)MAX_DIMS);
    ptrdiff_t ofs = where - p;
    CV_Assert(0 <= ofs && ofs <= old_dims);
    dims = (цяло)(old_dims+count);
    за (цяло i = old_dims-1; i >= (цяло)ofs; i--)
        p[i+count] = p[i];
    за (цяло i = 0; i < (цяло)count; i++)
        p[i+ofs] = value;
}

void MatShape::insert(цяло* where, цяло count, цяло value)
{
    insert(where, (size_t)count, value);
}

void MatShape::insert(цяло* where, const цяло* begin, const цяло* end)
{
    insert_(where, begin, end);
}

void MatShape::insert_(цяло* where, const цяло* begin, const цяло* end)
{
    цяло old_dims = std::max(dims, 0);
    ptrdiff_t delta = end - begin;
    CV_Assert(0 <= delta && old_dims+delta < MAX_DIMS);
    ptrdiff_t ofs = where - p;
    CV_Assert(0 <= ofs && ofs <= old_dims);
    dims = (цяло)(old_dims+delta);
    за (цяло i = old_dims-1; i >= (цяло)ofs; i--)
        p[i+delta] = p[i];
    за (цяло i = 0; i < (цяло)delta; i++)
        p[i+ofs] = begin[i];
}

void MatShape::erase(цяло* where)
{
    CV_Assert(dims > 0);
    ptrdiff_t ofs = where - p;
    CV_Assert(0 <= ofs && ofs <= dims);
    ако (ofs == dims)
        върни;
    dims--;
    за (цяло i = (цяло)ofs+1; i <= dims; i++)
        p[i-1] = p[i];
}

size_t MatShape::total() const
{
    size_t result = 1;
    ако (dims < 0)
        върни 0;
    за (цяло i = 0; i < dims; i++)
        result *= p[i];
    върни result;
}

std::vector<цяло> MatShape::vec() const
{
    ако (dims < 0)
        върни std::vector<цяло>(1, 0);
    върни std::vector<цяло>(p, p + dims);
}

std::string MatShape::str() const
{
    std::stringstream sstrm;
    ако (empty()) {
        sstrm << "<empty>";
    } иначе ако (dims == 0) {
        sstrm << "<scalar>";
    } иначе {
        sstrm << "[";
        за (цяло i = 0; i < dims; i++) {
            sstrm << (i > 0 ? " x " : "") << p[i];
        }
        sstrm << "]";
    }
    върни sstrm.str();
}

static void finalizeBlockLayout(MatShape& size, цяло C=0)
{
    ако (size.layout == DATA_LAYOUT_BLOCK) {
        CV_Assert(size.dims >= 4);
        цяло C0 = size.p[size.dims-1];
        CV_Assert(C0 > 1 && (C0 & (C0-1)) == 0);
        size.C = C > 0 ? C : size.p[1]*size.p[size.dims-1];
    } иначе {
        size.C = 0;
    }
    за (цяло i = std::max(size.dims, 0); i < MatShape::MAX_DIMS; i++)
        size.p[i] = 0;
    ако (size.dims == 0)
        size.p[0] = 1;
}

MatShape::MatShape()
{
    clear();
}

MatShape::MatShape(size_t dims_, const цяло* size_, DataLayout layout_, цяло C_)
{
    layout = layout_;
    CV_Assert(dims_ <= (size_t)MAX_DIMS);
    dims = (цяло)dims_;
    за (цяло i = 0; i < dims; i++) {
        p[i] = size_ ? size_[i] : 0;
    }
    finalizeBlockLayout(*this, C_);
}

MatShape::MatShape(size_t dims_, цяло value, DataLayout layout_)
{
    layout = layout_;
    CV_Assert(dims_ <= (size_t)MAX_DIMS);
    dims = (цяло)dims_;
    за (цяло i = 0; i < dims; i++) {
        p[i] = value;
    }
    finalizeBlockLayout(*this, 0);
}

MatShape::MatShape(std::initializer_list<цяло> shape)
{
    layout = DATA_LAYOUT_UNKNOWN;
    CV_Assert(shape.size() <= (size_t)MAX_DIMS);
    dims = (цяло)shape.size();
    auto it = shape.begin();
    за (цяло i = 0; i < dims; i++, ++it) {
        p[i] = *it;
    }
    finalizeBlockLayout(*this, 0);
}

MatShape::MatShape(цяло dims_, цяло value, DataLayout layout_)
{
    layout = layout_;
    CV_Assert(dims_ <= MAX_DIMS);
    dims = dims_;
    за (цяло i = 0; i < dims; i++) {
        p[i] = value;
    }
    finalizeBlockLayout(*this, 0);
}

MatShape::MatShape(const std::vector<цяло>& shape_, DataLayout layout_, цяло C_)
{
    layout = layout_;
    size_t shape_size = shape_.size();
    CV_Assert(shape_size < (size_t)MAX_DIMS);
    dims = (цяло)shape_size;
    за (цяло i = 0; i < dims; i++) {
        p[i] = shape_[i];
    }
    finalizeBlockLayout(*this, C_);
}

MatShape::MatShape(const цяло* begin, const цяло* end, DataLayout layout_, цяло C_)
{
    layout = layout_;
    ptrdiff_t shape_size = end - begin;
    CV_Assert(0 <= shape_size && shape_size < MAX_DIMS);
    dims = (цяло)shape_size;
    за (цяло i = 0; i < dims; i++) {
        p[i] = begin[i];
    }
    finalizeBlockLayout(*this, C_);
}

MatShape::MatShape(const MatShape& shape)
{
    dims = shape.dims;
    layout = shape.layout;
    C = shape.C;
    за (цяло i = 0; i < MAX_DIMS; i++)
        p[i] = shape.p[i];
}

MatShape& MatShape::operator = (const MatShape& shape)
{
    ако (this != &shape) {
        dims = shape.dims;
        layout = shape.layout;
        C = shape.C;
        за (цяло i = 0; i < MAX_DIMS; i++)
            p[i] = shape.p[i];
    }
    върни *this;
}

bool MatShape::hasSymbols() const
{
    за (цяло i = 0; i < dims; i++) {
        ако (p[i] < 0)
            върни true;
    }
    върни false;
}

цяло MatShape::channels() const
{
    CV_Assert(layout == DATA_LAYOUT_BLOCK || layout == DATA_LAYOUT_NCHW || layout == DATA_LAYOUT_NHWC);
    върни layout == DATA_LAYOUT_BLOCK ? C : p[layout == DATA_LAYOUT_NCHW ? 1 : dims-1];
}

MatShape MatShape::toLayout(DataLayout newLayout, цяло C0) const
{
    CV_Assert(layout == DATA_LAYOUT_BLOCK || layout == DATA_LAYOUT_NCHW || layout == DATA_LAYOUT_NHWC);
    CV_Assert(newLayout == DATA_LAYOUT_BLOCK || newLayout == DATA_LAYOUT_NCHW || newLayout == DATA_LAYOUT_NHWC);

    MatShape newsize = *this;
    newsize.layout = newLayout;

    ако (newLayout == DATA_LAYOUT_BLOCK) {
        // any => BLOCK
        CV_Assert_N(C0 > 1, (C0 & (C0-1)) == 0);
        цяло Corig = channels();
        newsize.C = Corig;

        ако (layout == DATA_LAYOUT_NHWC) {
            за (цяло i = 2; i < dims; i++) {
                newsize.p[i] = p[i-1];
            }
        }

        newsize.dims += layout != DATA_LAYOUT_BLOCK;
        newsize.p[1] = (Corig + C0 - 1)/C0;
        newsize.p[newsize.dims-1] = C0;
    } иначе ако (layout == DATA_LAYOUT_BLOCK) {
        // BLOCK => any (except for BLOCK, which is handled above)
        newsize.C = 0;
        ако (newLayout == DATA_LAYOUT_NHWC) {
            за (цяло i = 2; i < dims; i++) {
                newsize.p[i-1] = p[i];
            }
        }
        newsize.p[newLayout == DATA_LAYOUT_NCHW ? 1 : dims-2] = C;
        newsize.dims--;
    } иначе {
        CV_Assert_N(C0 <= 1);

        ако (newLayout == layout)
            върни newsize;

        // NHWC => NCHW
        ако (newLayout == DATA_LAYOUT_NCHW) {
            за (цяло i = 2; i < dims; i++)
                newsize.p[i] = p[i-1];
            newsize.p[1] = p[dims-1];
        } иначе {
            // NCHW => NHWC
            за (цяло i = 2; i < dims; i++)
                newsize.p[i-1] = p[i];
            newsize.p[dims-1] = p[1];
        }
    }

    върни newsize;
}

static цяло normalizeShapeAxis(цяло axis, цяло dims)
{
    CV_CheckGE(axis, -dims, "axis is out of range");
    CV_CheckLT(axis, dims, "axis is out of range");
    върни axis < 0 ? axis + dims : axis;
}

// The semantic NCHW form {N, C, spatial...} of a block ('NC1HWC0') shape, so that axes and
// element counts are the logical ones rather than the C0-padded ones.
static MatShape toSemantic(const MatShape& shape)
{
    CV_CheckGE(shape.dims, 3, "a block shape is {N, C1, spatial..., C0}");
    върни shape.toLayout(DATA_LAYOUT_NCHW);
}

// Blocks such a result again. Once N or C changes, elements cross the padded C0 lanes and
// the data needs a repack, which a shape operation cannot express.
static MatShape toBlockOf(MatShape result, const MatShape& src, const char* op)
{
    ако (result.dims < 2 || result.p[0] != src.p[0] || result.p[1] != src.C)
        CV_Error_(Error::StsNotImplemented,
                  ("%s: the block layout is kept only when the batch and the channel axes stay "
                   "unchanged; convert the shape with toLayout() first", op));
    CV_CheckLE(result.dims + 1, (цяло)MatShape::MAX_DIMS, "too many dimensions");
    result.layout = DATA_LAYOUT_NCHW;
    върни result.toLayout(DATA_LAYOUT_BLOCK, src.p[src.dims-1]);
}

MatShape MatShape::squeeze(const std::vector<цяло>& axes) const
{
    CV_Assert(dims >= 0);
    ако (layout == DATA_LAYOUT_BLOCK)
        върни toBlockOf(toSemantic(*this).squeeze(axes), *this, "squeeze");
    bool remove[MAX_DIMS] = {false};
    ако (axes.empty())
    {
        за (цяло i = 0; i < dims; i++)
            remove[i] = p[i] == 1;
    }
    за (цяло a : axes)
    {
        цяло i = normalizeShapeAxis(a, dims);
        CV_Check(a, !remove[i], "squeeze: duplicate axis");
        CV_Check(p[i], p[i] == 1, "squeeze: the size of a squeezed axis must be 1");
        remove[i] = true;
    }
    // a new number of axes: the layout of this shape does not apply
    MatShape result;
    result.dims = 0;
    за (цяло i = 0; i < dims; i++)
        ако (!remove[i])
            result.p[result.dims++] = p[i];
    върни result;
}

MatShape MatShape::unsqueeze(const std::vector<цяло>& axes) const
{
    CV_Assert(dims >= 0);
    ако (layout == DATA_LAYOUT_BLOCK)
        върни toBlockOf(toSemantic(*this).unsqueeze(axes), *this, "unsqueeze");
    const цяло outDims = dims + (цяло)axes.size();
    CV_CheckLE(outDims, (цяло)MAX_DIMS, "unsqueeze: too many dimensions");
    bool insert[MAX_DIMS] = {false};
    за (цяло a : axes)
    {
        цяло i = normalizeShapeAxis(a, outDims);
        CV_Check(a, !insert[i], "unsqueeze: duplicate axis");
        insert[i] = true;
    }
    // a new number of axes: the layout of this shape does not apply
    MatShape result;
    result.dims = outDims;
    за (цяло i = 0, j = 0; i < outDims; i++)
        result.p[i] = insert[i] ? 1 : p[j++];
    върни result;
}

MatShape MatShape::flatten(цяло startAxis, цяло endAxis) const
{
    CV_Assert(dims >= 0);
    ако (layout == DATA_LAYOUT_BLOCK)
        върни toBlockOf(toSemantic(*this).flatten(startAxis, endAxis), *this, "flatten");
    ако (dims == 0)
    {
        CV_Check(startAxis, startAxis == 0 || startAxis == -1, "flatten: axis is out of range");
        CV_Check(endAxis, endAxis == 0 || endAxis == -1, "flatten: axis is out of range");
        върни MatShape({1});
    }
    цяло start = normalizeShapeAxis(startAxis, dims), end = normalizeShapeAxis(endAxis, dims);
    CV_CheckLE(start, end, "flatten: startAxis must not follow endAxis");
    int64 merged = 1;
    за (цяло i = start; i <= end; i++)
        merged *= p[i];
    CV_CheckLE((double)merged, (double)INT_MAX, "flatten: the result is too big");
    // a new number of axes: the layout of this shape does not apply
    MatShape result;
    result.dims = 0;
    за (цяло i = 0; i < dims; i++)
    {
        ако (i == start)
            result.p[result.dims++] = (цяло)merged;
        иначе ако (i < start || i > end)
            result.p[result.dims++] = p[i];
    }
    върни result;
}

MatShape MatShape::reshape(const MatShape& newShape, bool allowZero) const
{
    CV_Assert(dims >= 0 && newShape.dims >= 0);
    ако (layout == DATA_LAYOUT_BLOCK)
        върни toBlockOf(toSemantic(*this).reshape(newShape, allowZero), *this, "reshape");
    // a new number of axes: neither the layout of this shape nor the one of the spec applies
    MatShape result;
    result.dims = newShape.dims;
    std::copy(newShape.p, newShape.p + newShape.dims, result.p);
    цяло inferred = -1;
    int64 known = 1;
    за (цяло i = 0; i < result.dims; i++)
    {
        цяло& sz = result.p[i];
        ако (sz == -1)
        {
            CV_Check(i, inferred < 0, "reshape: at most one axis may be -1");
            inferred = i;
            continue;
        }
        CV_Check(sz, sz >= 0, "reshape: sizes must be -1 or non-negative");
        ако (sz == 0 && !allowZero)
        {
            CV_Check(i, i < dims, "reshape: 0 refers to an axis the source does not have");
            sz = p[i];
        }
        known *= sz;
    }
    const int64 total_ = (int64)total();
    ако (inferred >= 0)
    {
        CV_Check((double)known, known != 0, "reshape: cannot infer the -1 axis next to a zero-size axis");
        CV_Check((double)total_, total_ % known == 0, "reshape: the number of elements does not match");
        CV_CheckLE((double)(total_/known), (double)INT_MAX, "reshape: the result is too big");
        result.p[inferred] = (цяло)(total_/known);
    }
    иначе ако (known != total_)
        CV_Error_(Error::StsBadArg, ("reshape: the number of elements does not match: %lld vs %lld",
                                     (long long)known, (long long)total_));
    върни result;
}

MatShape MatShape::expand(const MatShape& another) const
{
    ако (dims == 0)
        върни another;
    ако (another.dims == 0)
        върни *this;

    ако ((layout == DATA_LAYOUT_NCHW || layout == DATA_LAYOUT_NHWC) &&
        (another.layout == DATA_LAYOUT_NCHW || another.layout == DATA_LAYOUT_NHWC)) {
        CV_Assert(layout == another.layout);
    }
    // [TODO] support block layout
    CV_Assert(layout != DATA_LAYOUT_BLOCK && another.layout != DATA_LAYOUT_BLOCK);

    MatShape result;

    ако (dims < 0 || another.dims < 0)
        върни result;

    result = *this;
    result.dims = std::max(dims, another.dims);
    result.layout = layout == DATA_LAYOUT_UNKNOWN ? another.layout :
        layout == DATA_LAYOUT_ND && (another.layout == DATA_LAYOUT_NCHW ||
        another.layout == DATA_LAYOUT_NHWC) ? another.layout : layout;
    за (цяло i = result.dims-1; i >= 0; i--) {
        цяло i1 = i - (result.dims - dims);
        цяло i2 = i - (result.dims - another.dims);
        цяло sz1 = i1 < 0 ? 1 : p[i1];
        цяло sz2 = i2 < 0 ? 1 : another.p[i2];
        CV_Assert(sz1 == sz2 || sz1 == 1 || sz2 == 1);
        // [TODO] handle symbolic shapes
        result.p[i] = sz1 != 1 ? sz1 : sz2;
    }
    върни result;
}

/////////////////////////// MatAllocator ////////////////////////////

void MatAllocator::map(UMatData*, AccessFlag) const
{
}

void MatAllocator::unmap(UMatData* u) const
{
    ако(u->urefcount == 0 && u->refcount == 0)
    {
        deallocate(u);
    }
}

void MatAllocator::download(UMatData* u, void* dstptr,
         цяло dims, const size_t sz[],
         const size_t srcofs[], const size_t srcstep[],
         const size_t dststep[]) const
{
    ако(!u)
        върни;
    цяло isz[CV_MAX_DIM];
    uchar* srcptr = u->data;
    за( цяло i = 0; i < dims; i++ )
    {
        CV_Assert( sz[i] <= (size_t)INT_MAX );
        ако( sz[i] == 0 )
            върни;
        ако( srcofs )
            srcptr += srcofs[i]*(i <= dims-2 ? srcstep[i] : 1);
        isz[i] = (цяло)sz[i];
    }

    Mat src(dims, isz, CV_8U, srcptr, srcstep);
    Mat dst(dims, isz, CV_8U, dstptr, dststep);

    const Mat* arrays[] = { &src, &dst };
    uchar* ptrs[2];
    NAryMatIterator it(arrays, ptrs, 2);
    size_t planesz = it.size;

    за( size_t j = 0; j < it.nplanes; j++, ++it )
        memcpy(ptrs[1], ptrs[0], planesz);
}


void MatAllocator::upload(UMatData* u, const void* srcptr, цяло dims, const size_t sz[],
                    const size_t dstofs[], const size_t dststep[],
                    const size_t srcstep[]) const
{
    ако(!u)
        върни;
    цяло isz[CV_MAX_DIM];
    uchar* dstptr = u->data;
    за( цяло i = 0; i < dims; i++ )
    {
        CV_Assert( sz[i] <= (size_t)INT_MAX );
        ако( sz[i] == 0 )
            върни;
        ако( dstofs )
            dstptr += dstofs[i]*(i <= dims-2 ? dststep[i] : 1);
        isz[i] = (цяло)sz[i];
    }

    Mat src(dims, isz, CV_8U, (void*)srcptr, srcstep);
    Mat dst(dims, isz, CV_8U, dstptr, dststep);

    const Mat* arrays[] = { &src, &dst };
    uchar* ptrs[2];
    NAryMatIterator it(arrays, ptrs, 2);
    size_t planesz = it.size;

    за( size_t j = 0; j < it.nplanes; j++, ++it )
        memcpy(ptrs[1], ptrs[0], planesz);
}

void MatAllocator::copy(UMatData* usrc, UMatData* udst, цяло dims, const size_t sz[],
                  const size_t srcofs[], const size_t srcstep[],
                  const size_t dstofs[], const size_t dststep[], bool /*sync*/) const
{
    CV_INSTRUMENT_REGION();

    ако(!usrc || !udst)
        върни;
    цяло isz[CV_MAX_DIM];
    uchar* srcptr = usrc->data;
    uchar* dstptr = udst->data;
    за( цяло i = 0; i < dims; i++ )
    {
        CV_Assert( sz[i] <= (size_t)INT_MAX );
        ако( sz[i] == 0 )
            върни;
        ако( srcofs )
            srcptr += srcofs[i]*(i <= dims-2 ? srcstep[i] : 1);
        ако( dstofs )
            dstptr += dstofs[i]*(i <= dims-2 ? dststep[i] : 1);
        isz[i] = (цяло)sz[i];
    }

    Mat src(dims, isz, CV_8U, srcptr, srcstep);
    Mat dst(dims, isz, CV_8U, dstptr, dststep);

    const Mat* arrays[] = { &src, &dst };
    uchar* ptrs[2];
    NAryMatIterator it(arrays, ptrs, 2);
    size_t planesz = it.size;

    за( size_t j = 0; j < it.nplanes; j++, ++it )
        memcpy(ptrs[1], ptrs[0], planesz);
}

BufferPoolController* MatAllocator::getBufferPoolController(const char* id) const
{
    CV_UNUSED(id);
    static DummyBufferPoolController dummy;
    върни &dummy;
}

class StdMatAllocator CV_FINAL : public MatAllocator
{
public:
    UMatData* allocate(цяло dims, const цяло* sizes, цяло type,
                       void* data0, size_t* step, AccessFlag /*flags*/, UMatUsageFlags /*usageFlags*/) const CV_OVERRIDE
    {
        size_t total = CV_ELEM_SIZE(type);
        за( цяло i = dims-1; i >= 0; i-- )
        {
            ако( step )
            {
                ако( data0 && step[i] != CV_AUTOSTEP )
                {
                    CV_Assert(total <= step[i]);
                    total = step[i];
                }
                иначе
                    step[i] = total;
            }
            total *= sizes[i];
        }
        uchar* data = data0 ? (uchar*)data0 : (uchar*)fastMalloc(total);
        UMatData* u = new UMatData(this);
        u->data = u->origdata = data;
        u->size = total;
        ако(data0)
            u->flags |= UMatData::USER_ALLOCATED;

        върни u;
    }

    bool allocate(UMatData* u, AccessFlag /*accessFlags*/, UMatUsageFlags /*usageFlags*/) const CV_OVERRIDE
    {
        ако(!u) върни false;
        върни true;
    }

    void deallocate(UMatData* u) const CV_OVERRIDE
    {
        ако(!u)
            върни;

        CV_Assert(u->urefcount == 0);
        CV_Assert(u->refcount == 0);
        ако( !(u->flags & UMatData::USER_ALLOCATED) )
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
    върни g_matAllocator;
}

MatAllocator* Mat::getDefaultAllocator()
{
    върни getDefaultAllocatorMatRef();
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

void setSize( Mat& m, цяло _dims, const цяло* _sz, const size_t* _steps, bool autoSteps)
{
    CV_Assert( 0 <= _dims && _dims <= CV_MAX_DIM && _dims <= MatShape::MAX_DIMS);

    m.dims = _dims;
    m.size = MatShape(_dims, _sz);
    m.step[std::max(_dims-1, 0)] = CV_ELEM_SIZE(m.flags);
    за (цяло i = _dims-2; i >= 0; i--) {
        size_t autostep = m.size[i+1]*m.step[i+1];
        ако (_steps) {
            m.step[i] = _steps[i];
            //CV_Assert(m.step[i] >= autostep);
        } иначе ако (autoSteps) {
            m.step[i] = autostep;
        } иначе {
            m.step[i] = 0;
        }
    }

    ако( _dims <= 2 )
    {
        m.cols = _dims == 0 ? 1 : _sz ? _sz[_dims > 1] : 0;
        m.rows = _dims < 2 ? 1 : _sz ? _sz[0] : 0;
    } иначе {
        m.cols = m.rows = -1;
    }
}

цяло updateContinuityFlag(цяло flags, цяло dims, const цяло* size, const size_t* step)
{
    цяло i, j;
    ако (dims <= 1)
        върни flags | Mat::CONTINUOUS_FLAG;
    за( i = 0; i < dims; i++ )
    {
        ако( size[i] > 1 )
            break;
    }

    uint64 t = (uint64)size[std::min(i, dims-1)]*CV_MAT_CN(flags);
    за( j = dims-1; j > i; j-- )
    {
        t *= size[j];
        ако( step[j]*size[j] < step[j-1] )
            break;
    }

    ако( j <= i && t == (uint64)(цяло)t )
        върни flags | Mat::CONTINUOUS_FLAG;
    върни flags & ~Mat::CONTINUOUS_FLAG;
}

void Mat::updateContinuityFlag()
{
    flags = cv::updateContinuityFlag(flags, dims, size.p, step.p);
}

void finalizeHdr(Mat& m)
{
    m.updateContinuityFlag();
    цяло d = m.dims;
    ако( d > 2 )
        m.rows = m.cols = -1;
    ако(m.u)
        m.datastart = m.data = m.u->data;
    ако( m.data && d > 0 )
    {
        m.datalimit = m.datastart + m.size[0]*m.step[0];
        ако( m.size[0] > 0 )
        {
            цяло lastdim = d > 0 ? d - 1 : 0;
            m.dataend = m.ptr() + m.size[lastdim]*m.step[lastdim];
            за( цяло i = 0; i < d-1; i++ )
                m.dataend += (m.size[i] - 1)*m.step[i];
        }
        иначе
            m.dataend = m.datalimit;
    }
    иначе
        m.dataend = m.datalimit = m.data;
}

//======================================= Mat ======================================================

Mat::Mat() CV_NOEXCEPT
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{}

Mat::Mat(цяло _rows, цяло _cols, цяло _type)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_rows, _cols, _type);
}

Mat::Mat(цяло _rows, цяло _cols, цяло _type, const Scalar& _s)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_rows, _cols, _type);
    *this = _s;
}

Mat::Mat(Size _sz, цяло _type)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create( _sz.height, _sz.width, _type );
}

Mat::Mat(Size _sz, цяло _type, const Scalar& _s)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_sz.height, _sz.width, _type);
    *this = _s;
}

Mat::Mat(цяло _dims, const цяло* _sz, цяло _type)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_dims, _sz, _type);
}

Mat::Mat(цяло _dims, const цяло* _sz, цяло _type, const Scalar& _s)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_dims, _sz, _type);
    *this = _s;
}

Mat::Mat(const std::vector<цяло>& _sz, цяло _type)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_sz, _type);
}

Mat::Mat(const std::vector<цяло>& _sz, цяло _type, const Scalar& _s)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_sz, _type);
    *this = _s;
}

Mat::Mat(const MatShape& _shape, цяло _type)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_shape, _type);
}

Mat::Mat(std::initializer_list<цяло> _shape, цяло _type)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_shape, _type);
}

Mat::Mat(const MatShape& _shape, цяло _type, const Scalar& _s)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_shape, _type);
    *this = _s;
}

Mat::Mat(std::initializer_list<цяло> _shape, цяло _type, const Scalar& _s)
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
    ако( u )
        CV_XADD(&u->refcount, 1);
}

Mat::Mat(цяло _rows, цяло _cols, цяло _type, void* _data, size_t _step)
    : flags(MAGIC_VAL + (_type & TYPE_MASK)), dims(2), rows(_rows), cols(_cols),
      data((uchar*)_data), datastart((uchar*)_data), dataend(0), datalimit(0),
      allocator(0), u(0), size(2)
{
    CV_Assert(total() == 0 || data != NULL);

    size_t esz = CV_ELEM_SIZE(_type), esz1 = CV_ELEM_SIZE1(_type);
    size_t minstep = cols * esz;
    ако( _step == AUTO_STEP )
    {
        _step = minstep;
    }
    иначе
    {
        CV_Assert( _step >= minstep );
        ако (_step % esz1 != 0)
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

Mat::Mat(Size _sz, цяло _type, void* _data, size_t _step)
    : flags(MAGIC_VAL + (_type & TYPE_MASK)), dims(2), rows(_sz.height), cols(_sz.width),
      data((uchar*)_data), datastart((uchar*)_data), dataend(0), datalimit(0),
      allocator(0), u(0), size(2)
{
    CV_Assert(total() == 0 || data != NULL);

    size_t esz = CV_ELEM_SIZE(_type), esz1 = CV_ELEM_SIZE1(_type);
    size_t minstep = cols*esz;
    ако( _step == AUTO_STEP || (_step < minstep && rows == 1))
    {
        _step = minstep;
    }
    иначе
    {
        CV_CheckGE(_step, minstep, "");

        ако (_step % esz1 != 0)
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
    ако( this != &m )
    {
        ако( m.u )
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
    върни *this;
}

Mat Mat::clone() const
{
    Mat m;
    copyTo(m);
    върни m;
}

void Mat::assignTo( Mat& m, цяло _type ) const
{
    ако( _type < 0 )
        m = *this;
    иначе
        convertTo(m, _type);
}

void Mat::create(цяло _rows, цяло _cols, цяло _type)
{
    _type &= TYPE_MASK;
    ако( dims <= 2 && rows == _rows && cols == _cols && type() == _type && data )
        върни;
    цяло sz[] = {_rows, _cols};
    create(2, sz, _type);
}

void Mat::create(Size _sz, цяло _type)
{
    create(_sz.height, _sz.width, _type);
}

void Mat::createSameSize(InputArray m, цяло type)
{
    MatShape msize = m.shape();
    create(msize, type);
}

void Mat::fit(цяло _dims, const цяло* _sizes, цяло _type)
{
    size_t oldTotalBytes = u ? u->size : 0;
    size_t esz = CV_ELEM_SIZE(_type), newTotal = _dims >= 0;
    за (цяло i = 0; i < _dims; i++)
        newTotal *= _sizes[i];
    size_t newTotalBytes = newTotal*esz;
    ако (newTotalBytes > 0 && (!isContinuous() ||
                              newTotalBytes > oldTotalBytes ||
                              data != datastart)) {
        create(_dims, _sizes, _type);
    } иначе {
        flags = (flags & ~Mat::TYPE_MASK) | CV_MAT_TYPE(_type);
        цяло _dummy_size = 0;
        setSize(*this, (_dims >= 0 ? _dims : 1), (_dims >= 0 ? _sizes : &_dummy_size), nullptr, true);
        finalizeHdr(*this);
    }
}

void Mat::fit(const std::vector<цяло>& _shape, цяло _type)
{
    fit((цяло)_shape.size(), _shape.data(), _type);
}

void Mat::fit(const MatShape& _shape, цяло _type)
{
    fit(_shape.dims, _shape.p, _type);
    size.layout = _shape.layout;
    size.C = _shape.C;
}

void Mat::fit(std::initializer_list<цяло> _shape, цяло _type)
{
    цяло new_shape[MatShape::MAX_DIMS];
    цяло new_ndims = (цяло)_shape.size();
    CV_Assert(new_ndims <= MatShape::MAX_DIMS);
    auto it = _shape.begin();
    за (цяло i = 0; i < new_ndims; i++, ++it)
        new_shape[i] = *it;
    fit(new_ndims, new_shape, _type);
}

void Mat::fit(цяло _rows, цяло _cols, цяло _type)
{
    _type &= TYPE_MASK;
    цяло sz[] = {_rows, _cols};
    fit(2, sz, _type);
}

void Mat::fit(Size _sz, цяло _type)
{
    fit(_sz.height, _sz.width, _type);
}

void Mat::fitSameSize(InputArray m, цяло _type)
{
    fit(m.shape(), _type);
}

void Mat::addref()
{
    ако( u )
        CV_XADD(&u->refcount, 1);
}

void Mat::release()
{
    ако( u && CV_XADD(&u->refcount, -1) == 1 )
        deallocate();
    u = NULL;
    datastart = dataend = datalimit = data = 0;
    за(цяло i = 0; i < dims; i++)
        size.p[i] = 0;
    flags = (flags & CV_MAT_TYPE_MASK) | MAGIC_VAL;
    dims = rows = cols = 0;
    size.clear();
}

size_t Mat::step1(цяло i) const
{
    върни step.p[i] / elemSize1();
}

bool Mat::empty() const
{
    върни data == 0 || total() == 0;
}

size_t Mat::total() const
{
    ако( dims <= 2 )
        върни (size_t)rows * cols;
    size_t p = 1;
    за( цяло i = 0; i < dims; i++ )
        p *= size[i];
    върни p;
}

size_t Mat::total(цяло startDim, цяло endDim) const
{
    CV_Assert( 0 <= startDim && startDim <= endDim);
    size_t p = 1;
    цяло endDim_ = endDim <= dims ? endDim : dims;
    за( цяло i = startDim; i < endDim_; i++ )
        p *= size[i];
    върни p;
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
    ако (this == &m)
      върни *this;

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
    върни *this;
}


void Mat::create(цяло d0, const цяло* _sizes, цяло _type)
{
    цяло sz1 = 1, d = d0;
    цяло i;
    ако (d == 0) {
        d = 1;
        _sizes = (const цяло*)&sz1;
    }
    CV_Assert(0 <= d && d <= CV_MAX_DIM && _sizes);
    _type = CV_MAT_TYPE(_type);

    ако( data && d == dims && _type == type() )
    {
        за( i = 0; i < d; i++ )
            ако( size[i] != _sizes[i] )
                break;
        ако( i == d )
            върни;
    }

    цяло _sizes_backup[CV_MAX_DIM]; // #5991
    ако (_sizes == (this->size.p))
    {
        за(i = 0; i < d; i++ )
            _sizes_backup[i] = _sizes[i];
        _sizes = _sizes_backup;
    }

    release();
    ако( d == 0 )
        върни;
    flags = (_type & CV_MAT_TYPE_MASK) | MAGIC_VAL;
    setSize(*this, d, _sizes, 0, true);

    ако( total() > 0 )
    {
        MatAllocator *a = allocator, *a0 = getDefaultAllocator();
        ако(!a)
            a = a0;
        try
        {
            u = a->allocate(dims, size.p, _type, 0, step.p, ACCESS_RW /* ignored */, USAGE_DEFAULT);
            CV_Assert(u != 0);
            allocator = a;
        }
        catch (...)
        {
            ако (a == a0)
                throw;
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

void Mat::create(const std::vector<цяло>& _sizes, цяло _type)
{
    create((цяло)_sizes.size(), _sizes.data(), _type);
}

void Mat::create(const MatShape& _shape, цяло _type)
{
    ако (_shape.dims < 0) {
        release();
        върни;
    }
    create(_shape.dims, _shape.p, _type);
    size.layout = _shape.layout;
    size.C = _shape.C;
}

void Mat::create(std::initializer_list<цяло> _shape, цяло _type)
{
    цяло new_shape[MatShape::MAX_DIMS];
    цяло new_ndims = (цяло)_shape.size();
    CV_Assert(new_ndims <= MatShape::MAX_DIMS);
    auto it = _shape.begin();
    за (цяло i = 0; i < new_ndims; i++, ++it)
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
    ако(u)
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
    ако( m.dims > 2 )
    {
        Range rs[MatShape::MAX_DIMS];
        rs[0] = _rowRange;
        rs[1] = _colRange;
        за( цяло i = 2; i < m.dims; i++ )
            rs[i] = Range::all();
        *this = m(rs);
        върни;
    }

    *this = m;
    try
    {
        ако( _rowRange != Range::all() && _rowRange != Range(0,rows) )
        {
            CV_Assert( 0 <= _rowRange.start && _rowRange.start <= _rowRange.end
                       && _rowRange.end <= m.rows );
            size[0] = rows = _rowRange.size();
            data += step*_rowRange.start;
            flags |= SUBMATRIX_FLAG;
        }

        ако( _colRange != Range::all() && _colRange != Range(0,cols) )
        {
            CV_Assert( 0 <= _colRange.start && _colRange.start <= _colRange.end
                       && _colRange.end <= m.cols );
            size[dims > 1] = cols = _colRange.size();
            data += _colRange.start*elemSize();
            flags |= SUBMATRIX_FLAG;
        }
    }
    catch(...)
    {
        release();
        throw;
    }

    updateContinuityFlag();

    ако( rows <= 0 || cols <= 0 )
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
    ако( roi.width < m.cols || roi.height < m.rows )
        flags |= SUBMATRIX_FLAG;

    size[0] = rows;
    size[1] = cols;
    step[0] = m.step[0];
    step[1] = esz;
    updateContinuityFlag();

    addref();
    ако( rows <= 0 || cols <= 0 )
    {
        rows = cols = 0;
        release();
    }
}


Mat::Mat(цяло _dims, const цяло* _sizes, цяло _type, void* _data, const size_t* _steps)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    flags |= CV_MAT_TYPE(_type);
    datastart = data = (uchar*)_data;
    ако (_dims > 0 || _data != nullptr)
        setSize(*this, _dims, _sizes, _steps, true);
    finalizeHdr(*this);
}


Mat::Mat(const std::vector<цяло>& _sizes, цяло _type, void* _data, const size_t* _steps)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    flags |= CV_MAT_TYPE(_type);
    datastart = data = (uchar*)_data;
    цяло _dims = (цяло)_sizes.size();
    ако (_dims > 0 || _data != nullptr)
        setSize(*this, _dims, _sizes.data(), _steps, true);
    finalizeHdr(*this);
}

Mat::Mat(const MatShape& _shape, цяло _type, void* _data, const size_t* _steps)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    flags |= CV_MAT_TYPE(_type);
    datastart = data = (uchar*)_data;
    ако (_shape.dims >= 0) {
        setSize(*this, _shape.dims, _shape.p, _steps, true);
    }
    иначе {
        CV_Assert(!data);
    }
    finalizeHdr(*this);
}

Mat::Mat(std::initializer_list<цяло> _shape, цяло _type, void* _data, const size_t* _steps)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    цяло new_shape[MatShape::MAX_DIMS];
    цяло _dims = (цяло)_shape.size();
    CV_Assert(_dims <= MatShape::MAX_DIMS);
    auto it = _shape.begin();
    за (цяло i = 0; i < _dims; i++, ++it)
        new_shape[i] = *it;

    flags |= CV_MAT_TYPE(_type);
    datastart = data = (uchar*)_data;
    ако (_dims > 0 || _data != nullptr)
        setSize(*this, _dims, new_shape, _steps, true);
    finalizeHdr(*this);
}

Mat::Mat(const Mat& m, const Range* ranges)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    цяло d = m.dims;

    CV_Assert(ranges);
    за( цяло i = 0; i < d; i++ )
    {
        Range r = ranges[i];
        CV_Assert( r == Range::all() || (0 <= r.start && r.start < r.end && r.end <= m.size[i]) );
    }
    *this = m;
    за( цяло i = 0; i < d; i++ )
    {
        Range r = ranges[i];
        ако( r != Range::all() && r != Range(0, size.p[i]))
        {
            size.p[i] = r.end - r.start;
            data += r.start*step.p[i];
            flags |= SUBMATRIX_FLAG;
        }
    }

    ако (d <= 2) {
        rows = d == 2 ? size[0] : 1;
        cols = d <= 0 ? (d >= 0) : size[d > 1];
    }
    updateContinuityFlag();
}

Mat::Mat(const Mat& m, const std::vector<Range>& ranges)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
    datalimit(0), allocator(0), u(0)
{
    цяло d = m.dims;

    CV_Assert((цяло)ranges.size() == d);
    за (цяло i = 0; i < d; i++)
    {
        Range r = ranges[i];
        CV_Assert(r == Range::all() || (0 <= r.start && r.start < r.end && r.end <= m.size[i]));
    }
    *this = m;
    за (цяло i = 0; i < d; i++)
    {
        Range r = ranges[i];
        ако (r != Range::all() && r != Range(0, size.p[i]))
        {
            size.p[i] = r.end - r.start;
            data += r.start*step.p[i];
            flags |= SUBMATRIX_FLAG;
        }
    }

    ако (d <= 2) {
        rows = d == 2 ? size[0] : 1;
        cols = d <= 0 ? (d >= 0) : size[d > 1];
    }
    updateContinuityFlag();
}


Mat Mat::diag(цяло d) const
{
    CV_Assert( dims == 2 );
    Mat m = *this;
    size_t esz = elemSize();
    цяло len;

    ако( d >= 0 )
    {
        len = std::min(cols - d, rows);
        m.data += esz*d;
    }
    иначе
    {
        len = std::min(rows + d, cols);
        m.data -= step[0]*d;
    }
    CV_DbgAssert( len > 0 );

    m.size[0] = m.rows = len;
    m.size[1] = m.cols = 1;
    m.step[0] += (len > 1 ? esz : 0);

    m.updateContinuityFlag();

    ако( size() != Size(1,1) )
        m.flags |= SUBMATRIX_FLAG;

    върни m;
}


void Mat::pop_back(size_t nelems)
{
    CV_Assert( nelems <= (size_t)size.p[0] );
    CV_Assert( dims >= 1 );

    ако( isSubmatrix() )
        *this = rowRange(0, size.p[0] - (цяло)nelems);
    иначе
    {
        size.p[0] -= (цяло)nelems;
        ако (dims == 2)
            rows = size.p[0];
        иначе ако (dims == 1)
            cols = size.p[0];
        dataend -= nelems*step.p[0];
    }
}


void Mat::push_back_(const void* elem)
{
    size_t r = size.p[0];
    ако( isSubmatrix() || dataend + step.p[0] > datalimit )
        reserve( std::max(r + 1, (r*3+1)/2) );

    size_t esz = elemSize();
    memcpy(data + r*step.p[0], elem, esz);
    size.p[0] = цяло(r + 1);
    ако (dims == 2)
        rows = size.p[0];
    иначе ако (dims == 1)
        cols = size.p[0];
    dataend += step.p[0];
    uint64 tsz = size.p[0];
    за( цяло i = 1; i < dims; i++ )
        tsz *= size.p[i];
    ако( esz < step.p[0] || tsz != (uint64)(цяло)tsz )
        flags &= ~CONTINUOUS_FLAG;
}


void Mat::reserve(size_t nelems)
{
    const size_t MIN_SIZE = 64;

    CV_Assert( (цяло)nelems >= 0 );
    ако( !isSubmatrix() && step.p[0] != 0 && data + step.p[0]*nelems <= datalimit )
        върни;

    цяло r = size.p[0];
    ако( (size_t)r >= nelems)
        върни;

    MatShape newsize = size;
    size_t esz = elemSize();
    newsize.dims = std::max(newsize.dims, 1);
    ако (dims > 1 && r == 0 && step.p[0] == 0) {
        step.p[0] = size.p[1] * (dims == 2 ? esz : step.p[1]);
    }
    newsize.p[0] = (цяло)nelems;
    size_t newbytes = newsize.total() * esz;
    ако (newbytes < MIN_SIZE) {
        newsize.p[0] = 1;
        newsize.p[0] = цяло(MIN_SIZE / newsize.total() / esz);
    }

    Mat m(newsize, type());
    ако( r > 0)
    {
        Mat mpart = m.rowRange(0, r);
        copyTo(mpart);
    }

    *this = m;
    size.p[0] = r;

    ако (dims == 2)
        rows = size.p[0];
    иначе ако (dims == 1)
        cols = size.p[0];
    dataend = data + step.p[0]*r;
}


void Mat::reserveBuffer(size_t nbytes)
{
    size_t esz = 1;
    цяло mtype = CV_8UC1;
    ако (!empty())
    {
        ако (!isSubmatrix() && data + nbytes <= dataend)//Should it be datalimit?
            върни;
        esz = elemSize();
        mtype = type();
    }

    size_t nelems = (nbytes - 1) / esz + 1;

#ако SIZE_MAX > UINT_MAX
    CV_Assert(nelems <= size_t(INT_MAX)*size_t(INT_MAX));
    цяло newrows = nelems > size_t(INT_MAX) ? nelems > 0x400*size_t(INT_MAX) ? nelems > 0x100000 * size_t(INT_MAX) ? nelems > 0x40000000 * size_t(INT_MAX) ?
                  size_t(INT_MAX) : 0x40000000 : 0x100000 : 0x400 : 1;
#иначе
    цяло newrows = nelems > size_t(INT_MAX) ? 2 : 1;
#endif
    цяло newcols = (цяло)((nelems - 1) / newrows + 1);

    create(newrows, newcols, mtype);
}


void Mat::resize(size_t nelems)
{
    цяло r = size.p[0];
    ако( r == (цяло)nelems )
        върни;
    CV_Assert( (цяло)nelems >= 0 );

    ако( isSubmatrix() || data + step.p[0]*nelems > datalimit )
        reserve(nelems);

    size.p[0] = (цяло)nelems;
    ако (dims == 2)
        rows = size.p[0];
    иначе ако (dims == 1)
        cols = size.p[0];
    dataend += (size.p[0] - r)*(int64_t)step.p[0];

    //updateContinuityFlag(*this);
}


void Mat::resize(size_t nelems, const Scalar& s)
{
    цяло saveRows = size.p[0];
    resize(nelems);

    ако( size.p[0] > saveRows )
    {
        Mat part = rowRange(saveRows, size.p[0]);
        part = s;
    }
}

void Mat::push_back(const Mat& elems)
{
    size_t r = size.p[0];
    size_t delta = elems.size.p[0];
    ако( delta == 0 )
        върни;
    ако( this == &elems )
    {
        Mat tmp = elems;
        push_back(tmp);
        върни;
    }
    ако( !data )
    {
        *this = elems.clone();
        върни;
    }
    CV_Assert(dims > 0);

    size.p[0] = elems.size.p[0];
    bool eq = size == elems.size;
    size.p[0] = цяло(r);
    ако( !eq )
        CV_Error(cv::Error::StsUnmatchedSizes, "Pushed vector length is not equal to matrix row length");
    ако( type() != elems.type() )
        CV_Error(cv::Error::StsUnmatchedFormats, "Pushed vector type is not the same as matrix type");
    size_t esz = elemSize();
    size_t minstep = dims <= 1 ? esz : size.p[1] * (dims == 2 ? esz : step.p[1]);
    size_t step0 = step.p[0];
    ако (step0 < minstep) {
        ако (size.p[0] <= 1)
            step.p[0] = step0 = minstep;
    }

    ако( isSubmatrix() || dataend + step0*delta > datalimit )
        reserve( std::max(r + delta, (r*3+1)/2) );

    size.p[0] += цяло(delta);
    ако (dims == 2)
        rows = size.p[0];
    иначе ако (dims == 1)
        cols = size.p[0];
    dataend += step0*delta;

    //updateContinuityFlag(*this);

    ако( isContinuous() && elems.isContinuous() )
        memcpy(data + r*step0, elems.data, elems.total()*elems.elemSize());
    иначе
    {
        Mat part = rowRange(цяло(r), цяло(r + delta));
        elems.copyTo(part);
    }
}


void Mat::locateROI( Size& wholeSize, Point& ofs ) const
{
    CV_Assert( dims <= 2 && step[0] > 0 );
    size_t esz = elemSize(), minstep;
    ptrdiff_t delta1 = data - datastart, delta2 = dataend - datastart;

    ако( delta1 == 0 )
        ofs.x = ofs.y = 0;
    иначе
    {
        ofs.y = (цяло)(delta1/step[0]);
        ofs.x = (цяло)((delta1 - step[0]*ofs.y)/esz);
        CV_DbgAssert( data == datastart + ofs.y*step[0] + ofs.x*esz );
    }
    minstep = (ofs.x + cols)*esz;
    wholeSize.height = (цяло)((delta2 - minstep)/step[0] + 1);
    wholeSize.height = std::max(wholeSize.height, ofs.y + rows);
    wholeSize.width = (цяло)((delta2 - step*(wholeSize.height-1))/esz);
    wholeSize.width = std::max(wholeSize.width, ofs.x + cols);
}

Mat& Mat::adjustROI( цяло dtop, цяло dbottom, цяло dleft, цяло dright )
{
    CV_Assert( dims <= 2 && step[0] > 0 );
    Size wholeSize; Point ofs;
    size_t esz = elemSize();
    locateROI( wholeSize, ofs );
    цяло row1 = std::min(std::max(ofs.y - dtop, 0), wholeSize.height), row2 = std::max(0, std::min(ofs.y + rows + dbottom, wholeSize.height));
    цяло col1 = std::min(std::max(ofs.x - dleft, 0), wholeSize.width), col2 = std::max(0, std::min(ofs.x + cols + dright, wholeSize.width));
    ако(row1 > row2)
        std::swap(row1, row2);
    ако(col1 > col2)
        std::swap(col1, col2);

    ако (dims == 1) {
        data += (col1 - ofs.x)*(std::ptrdiff_t)esz;
        cols = col2 - col1;
        size.p[0] = cols;
    } иначе {
        data += (row1 - ofs.y)*(std::ptrdiff_t)step + (col1 - ofs.x)*(std::ptrdiff_t)esz;
        rows = row2 - row1; cols = col2 - col1;
        size.p[0] = rows; size.p[1] = cols;
        updateContinuityFlag();
    }
    върни *this;
}

Mat Mat::reshape(цяло new_cn, цяло new_rows) const
{
    цяло cn = channels();
    Mat hdr = *this;

    ако( new_cn == 0 )
        new_cn = cn;

    ако( dims > 2 )
    {
        ако( new_rows == 0 )
        {
            // special case: just change the number of channnels; retain the same shape,
            // except for the last, innermost dimension
            CV_Assert(size[dims-1]*cn % new_cn == 0);
            hdr.flags = (hdr.flags & ~CV_MAT_CN_MASK) | ((new_cn-1) << CV_CN_SHIFT);
            hdr.step[dims-1] = CV_ELEM_SIZE(hdr.flags);
            hdr.size[dims-1] = hdr.size[dims-1]*cn / new_cn;
            върни hdr;
        }
        CV_Assert( new_rows > 0 );
        цяло sz[] = { new_rows, (цяло)(total()*cn/new_rows) };
        върни reshape(new_cn, 2, sz);
    }

    цяло total_width = cols * cn;

    ако( (new_cn > total_width || total_width % new_cn != 0) && new_rows == 0 )
        new_rows = rows * total_width / new_cn;

    ако( new_rows != 0 && new_rows != rows )
    {
        цяло total_size = total_width * rows;
        ако( !isContinuous() )
            CV_Error( cv::Error::BadStep,
            "The matrix is not continuous, thus its number of rows can not be changed" );

        total_width = total_size / new_rows;

        ако( total_width * new_rows != total_size )
            CV_Error( cv::Error::StsBadArg, "The total number of matrix elements "
                                    "is not divisible by the new number of rows" );

        hdr.size[0] = hdr.rows = new_rows;
        hdr.step[0] = total_width * elemSize1();
    } иначе {
        hdr.size[0] = hdr.rows = rows;
        ако (dims <= 1)
            hdr.step[0] = cols * CV_ELEM_SIZE(flags);
    }

    цяло new_width = total_width / new_cn;

    ако( new_width * new_cn != total_width )
        CV_Error( cv::Error::BadNumChannels,
        "The total width is not divisible by the new number of channels" );

    hdr.size.dims = hdr.dims = 2;
    hdr.size[1] = hdr.cols = new_width;
    hdr.flags = (hdr.flags & ~CV_MAT_CN_MASK) | ((new_cn-1) << CV_CN_SHIFT);
    hdr.step[1] = CV_ELEM_SIZE(hdr.flags);
    върни hdr;
}

Mat Mat::reshape(цяло _cn, цяло _newndims, const цяло* _newsz) const
{
    ако(_newndims == dims)
    {
        ако(_newsz == 0)
            върни reshape(_cn);
        ако(_newndims == 2)
            върни reshape(_cn, _newsz[0]);
    }

    ако (isContinuous())
    {
        CV_Assert(_cn >= 0 && _newndims >= 0 && _newndims <= CV_MAX_DIM && (_newndims == 0 || _newsz != 0));

        ако (_cn == 0)
            _cn = this->channels();
        иначе
            CV_Assert(_cn <= CV_CN_MAX);

        size_t total_elem1_ref = this->total() * this->channels();
        size_t total_elem1 = _cn;

        AutoBuffer<цяло, 4> newsz_buf( (size_t)_newndims );

        цяло m1_idx = -1;

        за (цяло i = 0; i < _newndims; i++)
        {
            ако (_newsz[i] >= 0) {
                ако (_newsz[i] == 0 && i < dims)
                    newsz_buf[i] = size.p[i];
                иначе
                    newsz_buf[i] = _newsz[i];
                total_elem1 *= (size_t)newsz_buf[i];
            } иначе {
                ако (m1_idx >= 0)
                    CV_Error(cv::Error::StsBadSize, "More than one '-1' occured in the new shape");
                m1_idx = i;
            }
        }

        ако (m1_idx >= 0) {
            ако (total_elem1 == 0) {
                CV_Assert(total_elem1_ref == 0);
                total_elem1 = 1;
            }
            CV_Assert(total_elem1_ref % total_elem1 == 0);
            newsz_buf[m1_idx] = (цяло)(total_elem1_ref / total_elem1);
        } иначе ако (total_elem1 != total_elem1_ref) {
            CV_Error(cv::Error::StsUnmatchedSizes, "Requested and source matrices have different count of elements");
        }

        Mat hdr = *this;
        hdr.flags = (hdr.flags & ~CV_MAT_CN_MASK) | ((_cn-1) << CV_CN_SHIFT);
        setSize(hdr, _newndims, newsz_buf.data(), NULL, true);

        върни hdr;
    }

    CV_Error(cv::Error::StsNotImplemented, "Reshaping of n-dimensional non-continuous matrices is not supported yet");
    // TBD
}

Mat Mat::reshape(цяло _cn, const std::vector<цяло>& _newshape) const
{
    цяло newdims = (цяло)_newshape.size();
    ако(newdims == 0 && empty())
    {
        върни *this;
    }

    върни reshape(_cn, newdims, newdims > 0 ? &_newshape[0] : 0);
}

Mat Mat::reshape(цяло _cn, const MatShape& _newshape) const
{
    ако (_newshape.dims < 0) {
        цяло newshape[] = {0};
        върни reshape(_cn, 1, newshape);
    }
    върни reshape(_cn, _newshape.dims, _newshape.p);
}

Mat Mat::reshape(цяло _cn, std::initializer_list<цяло> newshape_) const
{
    цяло newshape[MatShape::MAX_DIMS];
    size_t i, newshape_dims = newshape_.size();
    CV_Assert(newshape_dims <= (size_t)MatShape::MAX_DIMS);
    auto it = newshape_.begin();
    за (i = 0; i < newshape_dims; i++, ++it)
        newshape[i] = *it;
    върни reshape(_cn, (цяло)newshape_dims, newshape);
}

Mat Mat::reinterpret(цяло type) const
{
    type = CV_MAT_TYPE(type);
    CV_Assert(CV_ELEM_SIZE(this->type()) == CV_ELEM_SIZE(type));
    Mat m = *this;
    m.flags = (m.flags & ~CV_MAT_TYPE_MASK) | type;
    m.updateContinuityFlag();
    върни m;
}

Mat Mat::diag(const Mat& d)
{
    CV_Assert( d.cols == 1 || d.rows == 1 );
    цяло len = d.rows + d.cols - 1;
    Mat m(len, len, d.type(), Scalar(0));
    Mat md = m.diag();
    ако( d.cols == 1 )
        d.copyTo(md);
    иначе
        transpose(d, md);
    върни m;
}

цяло Mat::checkVector(цяло _elemChannels, цяло _depth, bool _requireContinuous) const
{
    върни data && (depth() == _depth || _depth <= 0) &&
        (isContinuous() || !_requireContinuous) &&
        ((dims <= 2 && (((rows == 1 || cols == 1) && channels() == _elemChannels) ||
                        (cols == _elemChannels && channels() == 1))) ||
        (dims == 3 && channels() == 1 && size.p[2] == _elemChannels && (size.p[0] == 1 || size.p[1] == 1) &&
         (isContinuous() || step.p[1] == step.p[2]*size.p[2])))
    ? (цяло)(total()*channels()/_elemChannels) : -1;
}


static inline Size getContinuousSize_(цяло flags, цяло cols, цяло rows, цяло widthScale)
{
    int64 sz = (int64)cols * rows * widthScale;
    bool has_int_overflow = sz >= INT_MAX;
    bool isContiguous = (flags & Mat::CONTINUOUS_FLAG) != 0;
    върни (isContiguous && !has_int_overflow)
            ? Size((цяло)sz, 1)
            : Size(cols * widthScale, rows);
}

Size getContinuousSize2D(Mat& m1, цяло widthScale)
{
    CV_CheckLE(m1.dims, 2, "");
    върни getContinuousSize_(m1.flags,
                              m1.cols, m1.rows, widthScale);
}
Size getContinuousSize2D(Mat& m1, Mat& m2, цяло widthScale)
{
    CV_CheckLE(m1.dims, 2, "");
    CV_CheckLE(m2.dims, 2, "");
    const Size sz1 = m1.size();
    ако (sz1 != m2.size())  // reshape all matrixes to the same size (#4159)
    {
        size_t total_sz = m1.total();
        CV_CheckEQ(total_sz, m2.total(), "");
        bool is_m1_vector = m1.cols == 1 || m1.rows == 1;
        bool is_m2_vector = m2.cols == 1 || m2.rows == 1;
        CV_Assert(is_m1_vector); CV_Assert(is_m2_vector);
        цяло total = (цяло)total_sz;  // vector-column
        bool isContiguous = ((m1.flags & m2.flags) & Mat::CONTINUOUS_FLAG) != 0;
        bool has_int_overflow = ((int64)total_sz * widthScale) >= INT_MAX;
        ако (isContiguous && !has_int_overflow)
            total = 1; // vector-row
        m1 = m1.reshape(0, total);
        m2 = m2.reshape(0, total);
        CV_Assert(m1.cols == m2.cols && m1.rows == m2.rows);
        върни Size(m1.cols * widthScale, m1.rows);
    }
    върни getContinuousSize_(m1.flags & m2.flags,
                              m1.cols, m1.rows, widthScale);
}

Size getContinuousSize2D(Mat& m1, Mat& m2, Mat& m3, цяло widthScale)
{
    CV_CheckLE(m1.dims, 2, "");
    CV_CheckLE(m2.dims, 2, "");
    CV_CheckLE(m3.dims, 2, "");
    const Size sz1 = m1.size();
    ако (sz1 != m2.size() || sz1 != m3.size())  // reshape all matrixes to the same size (#4159)
    {
        size_t total_sz = m1.total();
        CV_CheckEQ(total_sz, m2.total(), "");
        CV_CheckEQ(total_sz, m3.total(), "");
        bool is_m1_vector = m1.cols == 1 || m1.rows == 1;
        bool is_m2_vector = m2.cols == 1 || m2.rows == 1;
        bool is_m3_vector = m3.cols == 1 || m3.rows == 1;
        CV_Assert(is_m1_vector); CV_Assert(is_m2_vector); CV_Assert(is_m3_vector);
        цяло total = (цяло)total_sz;  // vector-column
        bool isContiguous = ((m1.flags & m2.flags & m3.flags) & Mat::CONTINUOUS_FLAG) != 0;
        bool has_int_overflow = ((int64)total_sz * widthScale) >= INT_MAX;
        ако (isContiguous && !has_int_overflow)
            total = 1; // vector-row
        m1 = m1.reshape(0, total);
        m2 = m2.reshape(0, total);
        m3 = m3.reshape(0, total);
        CV_Assert(m1.cols == m2.cols && m1.rows == m2.rows && m1.cols == m3.cols && m1.rows == m3.rows);
        върни Size(m1.cols * widthScale, m1.rows);
    }
    върни getContinuousSize_(m1.flags & m2.flags & m3.flags,
                              m1.cols, m1.rows, widthScale);
}

} // cv::
