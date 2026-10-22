// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#அந்தர்பாவ "precomp.hpp"
#அந்தர்பாவ "bufferpool.impl.hpp"

நாமாகாஶ cv {

std::string layoutToString(DataLayout layout)
{
    திரும்பு
        layout == DATA_LAYOUT_ND ? "ND" :
        layout == DATA_LAYOUT_NCHW ? "NCHW" :
        layout == DATA_LAYOUT_NHWC ? "NHWC" :
        layout == DATA_LAYOUT_BLOCK ? "BLOCK" :
        layout == DATA_LAYOUT_NCDHW ? "NCDHW" :
        layout == DATA_LAYOUT_NDHWC ? "NDHWC" :
        layout == DATA_LAYOUT_PLANAR ? "PLANAR" :
        layout == DATA_LAYOUT_UNKNOWN ? "Unknown" : "???";
}

தருக்க operator == (மாறிலி MatShape& size1, மாறிலி MatShape& size2)
{
    எனில் (size1.dims != size2.dims)
        திரும்பு பொய்;
    எனில் (size1.layout != size2.layout &&
        size1.layout != DATA_LAYOUT_UNKNOWN &&
        size2.layout != DATA_LAYOUT_UNKNOWN)
        திரும்பு பொய்;
    எனில் (size1.layout == DATA_LAYOUT_BLOCK &&
        size2.layout == DATA_LAYOUT_BLOCK &&
        size1.C != size2.C)
        திரும்பு பொய்;
    க்கு (முழு i = 0; i < size1.dims; i++) {
        எனில் (size1.p[i] != size2.p[i])
            திரும்பு பொய்;
    }
    திரும்பு உண்மை;
}

தருக்க operator != (மாறிலி MatShape& size1, மாறிலி MatShape& size2)
{
    திரும்பு !(size1 == size2);
}

/////////////////////////// MatShape ////////////////////////////////

MatShape MatShape::scalar()
{
    திரும்பு MatShape(0);
}

வெறுமை MatShape::clear()
{
    dims = -1;
    layout = DATA_LAYOUT_UNKNOWN;
    C = 0;
    க்கு (முழு i = 0; i < MAX_DIMS; i++)
        p[i] = 0;
}

வெறுமை MatShape::resize(size_t newSize, முழு value)
{
    CV_Assert(newSize < (size_t)MAX_DIMS);
    முழு old_dims = std::max(dims, 0);
    dims = (முழு)newSize;
    க்கு (முழு i = old_dims; i < dims; i++)
        p[i] = value;
}

வெறுமை MatShape::reserve(size_t)
{
    // no op; maybe need to add a check for overflow, but we check it anyway in other operations
}

வெறுமை MatShape::assign(size_t newSize, முழு value)
{
    CV_Assert(newSize < (size_t)MAX_DIMS);
    dims = (முழு)newSize;
    க்கு (முழு i = 0; i < dims; i++)
        p[i] = value;
}

வெறுமை MatShape::assign(முழு newSize, முழு value)
{
    assign((size_t)newSize, value);
}

வெறுமை MatShape::assign(மாறிலி முழு* begin, மாறிலி முழு* end)
{
    assign_(begin, end);
}

வெறுமை MatShape::assign_(மாறிலி முழு* begin, மாறிலி முழு* end)
{
    ptrdiff_t newSize = end - begin;
    CV_Assert(0 <= newSize && newSize < (ptrdiff_t)MAX_DIMS);
    dims = (முழு)newSize;
    க்கு (முழு i = 0; i < dims; i++)
        p[i] = begin[i];
}

முழு* MatShape::begin() { திரும்பு p; }
மாறிலி முழு* MatShape::begin() மாறிலி { திரும்பு p; }
முழு* MatShape::end() { திரும்பு p + std::max(dims, 0); }
மாறிலி முழு* MatShape::end() மாறிலி { திரும்பு p + std::max(dims, 0); }
முழு& MatShape::back() { திரும்பு p[std::max(dims-1, 0)]; }
மாறிலி முழு& MatShape::back() மாறிலி { திரும்பு p[std::max(dims-1, 0)]; }

வெறுமை MatShape::push_back(முழு value)
{
    CV_Assert(dims+1 < MAX_DIMS);
    dims = std::max(dims+1, 1);
    p[dims-1] = value;
}

வெறுமை MatShape::emplace_back(முழு value)
{
    push_back(value);
}

வெறுமை MatShape::insert(முழு* where, முழு value)
{
    முழு old_dims = std::max(dims, 0);
    CV_Assert(old_dims+1 < MAX_DIMS);
    ptrdiff_t ofs = where - p;
    CV_Assert(0 <= ofs && ofs <= old_dims);
    dims = old_dims+1;
    க்கு (முழு i = old_dims-1; i >= (முழு)ofs; i--)
        p[i+1] = p[i];
    p[ofs] = value;
}

வெறுமை MatShape::insert(முழு* where, size_t மொத்தம்_2, முழு value)
{
    முழு old_dims = std::max(dims, 0);
    CV_Assert((size_t)(old_dims+மொத்தம்_2) < (size_t)MAX_DIMS);
    ptrdiff_t ofs = where - p;
    CV_Assert(0 <= ofs && ofs <= old_dims);
    dims = (முழு)(old_dims+மொத்தம்_2);
    க்கு (முழு i = old_dims-1; i >= (முழு)ofs; i--)
        p[i+மொத்தம்_2] = p[i];
    க்கு (முழு i = 0; i < (முழு)மொத்தம்_2; i++)
        p[i+ofs] = value;
}

வெறுமை MatShape::insert(முழு* where, முழு மொத்தம்_2, முழு value)
{
    insert(where, (size_t)மொத்தம்_2, value);
}

வெறுமை MatShape::insert(முழு* where, மாறிலி முழு* begin, மாறிலி முழு* end)
{
    செருகு(where, begin, end);
}

வெறுமை MatShape::செருகு(முழு* where, மாறிலி முழு* begin, மாறிலி முழு* end)
{
    முழு old_dims = std::max(dims, 0);
    ptrdiff_t delta = end - begin;
    CV_Assert(0 <= delta && old_dims+delta < MAX_DIMS);
    ptrdiff_t ofs = where - p;
    CV_Assert(0 <= ofs && ofs <= old_dims);
    dims = (முழு)(old_dims+delta);
    க்கு (முழு i = old_dims-1; i >= (முழு)ofs; i--)
        p[i+delta] = p[i];
    க்கு (முழு i = 0; i < (முழு)delta; i++)
        p[i+ofs] = begin[i];
}

வெறுமை MatShape::erase(முழு* where)
{
    CV_Assert(dims > 0);
    ptrdiff_t ofs = where - p;
    CV_Assert(0 <= ofs && ofs <= dims);
    எனில் (ofs == dims)
        திரும்பு;
    dims--;
    க்கு (முழு i = (முழு)ofs+1; i <= dims; i++)
        p[i-1] = p[i];
}

size_t MatShape::total() மாறிலி
{
    size_t result = 1;
    எனில் (dims < 0)
        திரும்பு 0;
    க்கு (முழு i = 0; i < dims; i++)
        result *= p[i];
    திரும்பு result;
}

std::vector<முழு> MatShape::vec() மாறிலி
{
    எனில் (dims < 0)
        திரும்பு std::vector<முழு>(1, 0);
    திரும்பு std::vector<முழு>(p, p + dims);
}

std::string MatShape::str() மாறிலி
{
    std::stringstream sstrm;
    எனில் (empty()) {
        sstrm << "<empty>";
    } இல்லையெனில் எனில் (dims == 0) {
        sstrm << "<scalar>";
    } இல்லையெனில் {
        sstrm << "[";
        க்கு (முழு i = 0; i < dims; i++) {
            sstrm << (i > 0 ? " x " : "") << p[i];
        }
        sstrm << "]";
    }
    திரும்பு sstrm.str();
}

நிலையான வெறுமை finalizeBlockLayout(MatShape& size, முழு C=0)
{
    எனில் (size.layout == DATA_LAYOUT_BLOCK) {
        CV_Assert(size.dims >= 4);
        முழு C0 = size.p[size.dims-1];
        CV_Assert(C0 > 1 && (C0 & (C0-1)) == 0);
        size.C = C > 0 ? C : size.p[1]*size.p[size.dims-1];
    } இல்லையெனில் {
        size.C = 0;
    }
    க்கு (முழு i = std::max(size.dims, 0); i < MatShape::MAX_DIMS; i++)
        size.p[i] = 0;
    எனில் (size.dims == 0)
        size.p[0] = 1;
}

MatShape::MatShape()
{
    clear();
}

MatShape::MatShape(size_t dims_, மாறிலி முழு* size_, DataLayout layout_, முழு C_)
{
    layout = layout_;
    CV_Assert(dims_ <= (size_t)MAX_DIMS);
    dims = (முழு)dims_;
    க்கு (முழு i = 0; i < dims; i++) {
        p[i] = size_ ? size_[i] : 0;
    }
    finalizeBlockLayout(*இது, C_);
}

MatShape::MatShape(size_t dims_, முழு value, DataLayout layout_)
{
    layout = layout_;
    CV_Assert(dims_ <= (size_t)MAX_DIMS);
    dims = (முழு)dims_;
    க்கு (முழு i = 0; i < dims; i++) {
        p[i] = value;
    }
    finalizeBlockLayout(*இது, 0);
}

MatShape::MatShape(std::initializer_list<முழு> shape)
{
    layout = DATA_LAYOUT_UNKNOWN;
    CV_Assert(shape.size() <= (size_t)MAX_DIMS);
    dims = (முழு)shape.size();
    auto it = shape.begin();
    க்கு (முழு i = 0; i < dims; i++, ++it) {
        p[i] = *it;
    }
    finalizeBlockLayout(*இது, 0);
}

MatShape::MatShape(முழு dims_, முழு value, DataLayout layout_)
{
    layout = layout_;
    CV_Assert(dims_ <= MAX_DIMS);
    dims = dims_;
    க்கு (முழு i = 0; i < dims; i++) {
        p[i] = value;
    }
    finalizeBlockLayout(*இது, 0);
}

MatShape::MatShape(மாறிலி std::vector<முழு>& shape_, DataLayout layout_, முழு C_)
{
    layout = layout_;
    size_t shape_size = shape_.size();
    CV_Assert(shape_size < (size_t)MAX_DIMS);
    dims = (முழு)shape_size;
    க்கு (முழு i = 0; i < dims; i++) {
        p[i] = shape_[i];
    }
    finalizeBlockLayout(*இது, C_);
}

MatShape::MatShape(மாறிலி முழு* begin, மாறிலி முழு* end, DataLayout layout_, முழு C_)
{
    layout = layout_;
    ptrdiff_t shape_size = end - begin;
    CV_Assert(0 <= shape_size && shape_size < MAX_DIMS);
    dims = (முழு)shape_size;
    க்கு (முழு i = 0; i < dims; i++) {
        p[i] = begin[i];
    }
    finalizeBlockLayout(*இது, C_);
}

MatShape::MatShape(மாறிலி MatShape& shape)
{
    dims = shape.dims;
    layout = shape.layout;
    C = shape.C;
    க்கு (முழு i = 0; i < MAX_DIMS; i++)
        p[i] = shape.p[i];
}

MatShape& MatShape::operator = (மாறிலி MatShape& shape)
{
    எனில் (இது != &shape) {
        dims = shape.dims;
        layout = shape.layout;
        C = shape.C;
        க்கு (முழு i = 0; i < MAX_DIMS; i++)
            p[i] = shape.p[i];
    }
    திரும்பு *இது;
}

தருக்க MatShape::hasSymbols() மாறிலி
{
    க்கு (முழு i = 0; i < dims; i++) {
        எனில் (p[i] < 0)
            திரும்பு உண்மை;
    }
    திரும்பு பொய்;
}

முழு MatShape::channels() மாறிலி
{
    CV_Assert(layout == DATA_LAYOUT_BLOCK || layout == DATA_LAYOUT_NCHW || layout == DATA_LAYOUT_NHWC);
    திரும்பு layout == DATA_LAYOUT_BLOCK ? C : p[layout == DATA_LAYOUT_NCHW ? 1 : dims-1];
}

MatShape MatShape::toLayout(DataLayout newLayout, முழு C0) மாறிலி
{
    CV_Assert(layout == DATA_LAYOUT_BLOCK || layout == DATA_LAYOUT_NCHW || layout == DATA_LAYOUT_NHWC);
    CV_Assert(newLayout == DATA_LAYOUT_BLOCK || newLayout == DATA_LAYOUT_NCHW || newLayout == DATA_LAYOUT_NHWC);

    MatShape newsize = *இது;
    newsize.layout = newLayout;

    எனில் (newLayout == DATA_LAYOUT_BLOCK) {
        // any => BLOCK
        CV_Assert_N(C0 > 1, (C0 & (C0-1)) == 0);
        முழு Corig = channels();
        newsize.C = Corig;

        எனில் (layout == DATA_LAYOUT_NHWC) {
            க்கு (முழு i = 2; i < dims; i++) {
                newsize.p[i] = p[i-1];
            }
        }

        newsize.dims += layout != DATA_LAYOUT_BLOCK;
        newsize.p[1] = (Corig + C0 - 1)/C0;
        newsize.p[newsize.dims-1] = C0;
    } இல்லையெனில் எனில் (layout == DATA_LAYOUT_BLOCK) {
        // BLOCK => any (except for BLOCK, which is handled above)
        newsize.C = 0;
        எனில் (newLayout == DATA_LAYOUT_NHWC) {
            க்கு (முழு i = 2; i < dims; i++) {
                newsize.p[i-1] = p[i];
            }
        }
        newsize.p[newLayout == DATA_LAYOUT_NCHW ? 1 : dims-2] = C;
        newsize.dims--;
    } இல்லையெனில் {
        CV_Assert_N(C0 <= 1);

        எனில் (newLayout == layout)
            திரும்பு newsize;

        // NHWC => NCHW
        எனில் (newLayout == DATA_LAYOUT_NCHW) {
            க்கு (முழு i = 2; i < dims; i++)
                newsize.p[i] = p[i-1];
            newsize.p[1] = p[dims-1];
        } இல்லையெனில் {
            // NCHW => NHWC
            க்கு (முழு i = 2; i < dims; i++)
                newsize.p[i-1] = p[i];
            newsize.p[dims-1] = p[1];
        }
    }

    திரும்பு newsize;
}

நிலையான முழு normalizeShapeAxis(முழு axis, முழு dims)
{
    CV_CheckGE(axis, -dims, "axis is out of range");
    CV_CheckLT(axis, dims, "axis is out of range");
    திரும்பு axis < 0 ? axis + dims : axis;
}

// The semantic NCHW form {N, C, spatial...} of a block ('NC1HWC0') shape, so that axes and
// element counts are the logical ones rather than the C0-padded ones.
நிலையான MatShape toSemantic(மாறிலி MatShape& shape)
{
    CV_CheckGE(shape.dims, 3, "a block shape is {N, C1, spatial..., C0}");
    திரும்பு shape.toLayout(DATA_LAYOUT_NCHW);
}

// Blocks such a result again. Once N or C changes, elements cross the padded C0 lanes and
// the data needs a repack, which a shape operation cannot express.
நிலையான MatShape toBlockOf(MatShape result, மாறிலி MatShape& src, மாறிலி எழுத்து* op)
{
    எனில் (result.dims < 2 || result.p[0] != src.p[0] || result.p[1] != src.C)
        CV_Error_(Error::StsNotImplemented,
                  ("%s: the block layout is kept only when the batch and the channel axes stay "
                   "unchanged; convert the shape with toLayout() first", op));
    CV_CheckLE(result.dims + 1, (முழு)MatShape::MAX_DIMS, "too many dimensions");
    result.layout = DATA_LAYOUT_NCHW;
    திரும்பு result.toLayout(DATA_LAYOUT_BLOCK, src.p[src.dims-1]);
}

MatShape MatShape::squeeze(மாறிலி std::vector<முழு>& axes) மாறிலி
{
    CV_Assert(dims >= 0);
    எனில் (layout == DATA_LAYOUT_BLOCK)
        திரும்பு toBlockOf(toSemantic(*இது).squeeze(axes), *இது, "squeeze");
    தருக்க remove[MAX_DIMS] = {பொய்};
    எனில் (axes.empty())
    {
        க்கு (முழு i = 0; i < dims; i++)
            remove[i] = p[i] == 1;
    }
    க்கு (முழு a : axes)
    {
        முழு i = normalizeShapeAxis(a, dims);
        CV_Check(a, !remove[i], "squeeze: duplicate axis");
        CV_Check(p[i], p[i] == 1, "squeeze: the size of a squeezed axis must be 1");
        remove[i] = உண்மை;
    }
    // a new number of axes: the layout of this shape does not apply
    MatShape result;
    result.dims = 0;
    க்கு (முழு i = 0; i < dims; i++)
        எனில் (!remove[i])
            result.p[result.dims++] = p[i];
    திரும்பு result;
}

MatShape MatShape::unsqueeze(மாறிலி std::vector<முழு>& axes) மாறிலி
{
    CV_Assert(dims >= 0);
    எனில் (layout == DATA_LAYOUT_BLOCK)
        திரும்பு toBlockOf(toSemantic(*இது).unsqueeze(axes), *இது, "unsqueeze");
    மாறிலி முழு outDims = dims + (முழு)axes.size();
    CV_CheckLE(outDims, (முழு)MAX_DIMS, "unsqueeze: too many dimensions");
    தருக்க insert[MAX_DIMS] = {பொய்};
    க்கு (முழு a : axes)
    {
        முழு i = normalizeShapeAxis(a, outDims);
        CV_Check(a, !insert[i], "unsqueeze: duplicate axis");
        insert[i] = உண்மை;
    }
    // a new number of axes: the layout of this shape does not apply
    MatShape result;
    result.dims = outDims;
    க்கு (முழு i = 0, j = 0; i < outDims; i++)
        result.p[i] = insert[i] ? 1 : p[j++];
    திரும்பு result;
}

MatShape MatShape::flatten(முழு startAxis, முழு endAxis) மாறிலி
{
    CV_Assert(dims >= 0);
    எனில் (layout == DATA_LAYOUT_BLOCK)
        திரும்பு toBlockOf(toSemantic(*இது).flatten(startAxis, endAxis), *இது, "flatten");
    எனில் (dims == 0)
    {
        CV_Check(startAxis, startAxis == 0 || startAxis == -1, "flatten: axis is out of range");
        CV_Check(endAxis, endAxis == 0 || endAxis == -1, "flatten: axis is out of range");
        திரும்பு MatShape({1});
    }
    முழு start = normalizeShapeAxis(startAxis, dims), end = normalizeShapeAxis(endAxis, dims);
    CV_CheckLE(start, end, "flatten: startAxis must not follow endAxis");
    int64 merged = 1;
    க்கு (முழு i = start; i <= end; i++)
        merged *= p[i];
    CV_CheckLE((இரட்டை)merged, (இரட்டை)INT_MAX, "flatten: the result is too big");
    // a new number of axes: the layout of this shape does not apply
    MatShape result;
    result.dims = 0;
    க்கு (முழு i = 0; i < dims; i++)
    {
        எனில் (i == start)
            result.p[result.dims++] = (முழு)merged;
        இல்லையெனில் எனில் (i < start || i > end)
            result.p[result.dims++] = p[i];
    }
    திரும்பு result;
}

MatShape MatShape::reshape(மாறிலி MatShape& newShape, தருக்க allowZero) மாறிலி
{
    CV_Assert(dims >= 0 && newShape.dims >= 0);
    எனில் (layout == DATA_LAYOUT_BLOCK)
        திரும்பு toBlockOf(toSemantic(*இது).reshape(newShape, allowZero), *இது, "reshape");
    // a new number of axes: neither the layout of this shape nor the one of the spec applies
    MatShape result;
    result.dims = newShape.dims;
    std::copy(newShape.p, newShape.p + newShape.dims, result.p);
    முழு inferred = -1;
    int64 known = 1;
    க்கு (முழு i = 0; i < result.dims; i++)
    {
        முழு& sz = result.p[i];
        எனில் (sz == -1)
        {
            CV_Check(i, inferred < 0, "reshape: at most one axis may be -1");
            inferred = i;
            தொடர்;
        }
        CV_Check(sz, sz >= 0, "reshape: sizes must be -1 or non-negative");
        எனில் (sz == 0 && !allowZero)
        {
            CV_Check(i, i < dims, "reshape: 0 refers to an axis the source does not have");
            sz = p[i];
        }
        known *= sz;
    }
    மாறிலி int64 total_ = (int64)total();
    எனில் (inferred >= 0)
    {
        CV_Check((இரட்டை)known, known != 0, "reshape: cannot infer the -1 axis next to a zero-size axis");
        CV_Check((இரட்டை)total_, total_ % known == 0, "reshape: the number of elements does not match");
        CV_CheckLE((இரட்டை)(total_/known), (இரட்டை)INT_MAX, "reshape: the result is too big");
        result.p[inferred] = (முழு)(total_/known);
    }
    இல்லையெனில் எனில் (known != total_)
        CV_Error_(Error::StsBadArg, ("reshape: the number of elements does not match: %lld vs %lld",
                                     (long long)known, (long long)total_));
    திரும்பு result;
}

MatShape MatShape::expand(மாறிலி MatShape& another) மாறிலி
{
    எனில் (dims == 0)
        திரும்பு another;
    எனில் (another.dims == 0)
        திரும்பு *இது;

    எனில் ((layout == DATA_LAYOUT_NCHW || layout == DATA_LAYOUT_NHWC) &&
        (another.layout == DATA_LAYOUT_NCHW || another.layout == DATA_LAYOUT_NHWC)) {
        CV_Assert(layout == another.layout);
    }
    // [TODO] support block layout
    CV_Assert(layout != DATA_LAYOUT_BLOCK && another.layout != DATA_LAYOUT_BLOCK);

    MatShape result;

    எனில் (dims < 0 || another.dims < 0)
        திரும்பு result;

    result = *இது;
    result.dims = std::max(dims, another.dims);
    result.layout = layout == DATA_LAYOUT_UNKNOWN ? another.layout :
        layout == DATA_LAYOUT_ND && (another.layout == DATA_LAYOUT_NCHW ||
        another.layout == DATA_LAYOUT_NHWC) ? another.layout : layout;
    க்கு (முழு i = result.dims-1; i >= 0; i--) {
        முழு i1 = i - (result.dims - dims);
        முழு i2 = i - (result.dims - another.dims);
        முழு sz1 = i1 < 0 ? 1 : p[i1];
        முழு sz2 = i2 < 0 ? 1 : another.p[i2];
        CV_Assert(sz1 == sz2 || sz1 == 1 || sz2 == 1);
        // [TODO] handle symbolic shapes
        result.p[i] = sz1 != 1 ? sz1 : sz2;
    }
    திரும்பு result;
}

/////////////////////////// MatAllocator ////////////////////////////

வெறுமை MatAllocator::map(UMatData*, AccessFlag) மாறிலி
{
}

வெறுமை MatAllocator::unmap(UMatData* u) மாறிலி
{
    எனில்(u->urefcount == 0 && u->refcount == 0)
    {
        deallocate(u);
    }
}

வெறுமை MatAllocator::download(UMatData* u, வெறுமை* dstptr,
         முழு dims, மாறிலி size_t sz[],
         மாறிலி size_t srcofs[], மாறிலி size_t srcstep[],
         மாறிலி size_t dststep[]) மாறிலி
{
    எனில்(!u)
        திரும்பு;
    முழு isz[CV_MAX_DIM];
    uchar* srcptr = u->data;
    க்கு( முழு i = 0; i < dims; i++ )
    {
        CV_Assert( sz[i] <= (size_t)INT_MAX );
        எனில்( sz[i] == 0 )
            திரும்பு;
        எனில்( srcofs )
            srcptr += srcofs[i]*(i <= dims-2 ? srcstep[i] : 1);
        isz[i] = (முழு)sz[i];
    }

    Mat src(dims, isz, CV_8U, srcptr, srcstep);
    Mat dst(dims, isz, CV_8U, dstptr, dststep);

    மாறிலி Mat* arrays[] = { &src, &dst };
    uchar* ptrs[2];
    NAryMatIterator it(arrays, ptrs, 2);
    size_t planesz = it.size;

    க்கு( size_t j = 0; j < it.nplanes; j++, ++it )
        memcpy(ptrs[1], ptrs[0], planesz);
}


வெறுமை MatAllocator::upload(UMatData* u, மாறிலி வெறுமை* srcptr, முழு dims, மாறிலி size_t sz[],
                    மாறிலி size_t dstofs[], மாறிலி size_t dststep[],
                    மாறிலி size_t srcstep[]) மாறிலி
{
    எனில்(!u)
        திரும்பு;
    முழு isz[CV_MAX_DIM];
    uchar* dstptr = u->data;
    க்கு( முழு i = 0; i < dims; i++ )
    {
        CV_Assert( sz[i] <= (size_t)INT_MAX );
        எனில்( sz[i] == 0 )
            திரும்பு;
        எனில்( dstofs )
            dstptr += dstofs[i]*(i <= dims-2 ? dststep[i] : 1);
        isz[i] = (முழு)sz[i];
    }

    Mat src(dims, isz, CV_8U, (வெறுமை*)srcptr, srcstep);
    Mat dst(dims, isz, CV_8U, dstptr, dststep);

    மாறிலி Mat* arrays[] = { &src, &dst };
    uchar* ptrs[2];
    NAryMatIterator it(arrays, ptrs, 2);
    size_t planesz = it.size;

    க்கு( size_t j = 0; j < it.nplanes; j++, ++it )
        memcpy(ptrs[1], ptrs[0], planesz);
}

வெறுமை MatAllocator::copy(UMatData* usrc, UMatData* udst, முழு dims, மாறிலி size_t sz[],
                  மாறிலி size_t srcofs[], மாறிலி size_t srcstep[],
                  மாறிலி size_t dstofs[], மாறிலி size_t dststep[], தருக்க /*sync*/) மாறிலி
{
    CV_INSTRUMENT_REGION();

    எனில்(!usrc || !udst)
        திரும்பு;
    முழு isz[CV_MAX_DIM];
    uchar* srcptr = usrc->data;
    uchar* dstptr = udst->data;
    க்கு( முழு i = 0; i < dims; i++ )
    {
        CV_Assert( sz[i] <= (size_t)INT_MAX );
        எனில்( sz[i] == 0 )
            திரும்பு;
        எனில்( srcofs )
            srcptr += srcofs[i]*(i <= dims-2 ? srcstep[i] : 1);
        எனில்( dstofs )
            dstptr += dstofs[i]*(i <= dims-2 ? dststep[i] : 1);
        isz[i] = (முழு)sz[i];
    }

    Mat src(dims, isz, CV_8U, srcptr, srcstep);
    Mat dst(dims, isz, CV_8U, dstptr, dststep);

    மாறிலி Mat* arrays[] = { &src, &dst };
    uchar* ptrs[2];
    NAryMatIterator it(arrays, ptrs, 2);
    size_t planesz = it.size;

    க்கு( size_t j = 0; j < it.nplanes; j++, ++it )
        memcpy(ptrs[1], ptrs[0], planesz);
}

BufferPoolController* MatAllocator::getBufferPoolController(மாறிலி எழுத்து* id) மாறிலி
{
    CV_UNUSED(id);
    நிலையான DummyBufferPoolController dummy;
    திரும்பு &dummy;
}

வகுப்பு StdMatAllocator CV_FINAL : பொது MatAllocator
{
பொது:
    UMatData* allocate(முழு dims, மாறிலி முழு* sizes, முழு type,
                       வெறுமை* data0, size_t* step, AccessFlag /*flags*/, UMatUsageFlags /*usageFlags*/) மாறிலி CV_OVERRIDE
    {
        size_t total = CV_ELEM_SIZE(type);
        க்கு( முழு i = dims-1; i >= 0; i-- )
        {
            எனில்( step )
            {
                எனில்( data0 && step[i] != CV_AUTOSTEP )
                {
                    CV_Assert(total <= step[i]);
                    total = step[i];
                }
                இல்லையெனில்
                    step[i] = total;
            }
            total *= sizes[i];
        }
        uchar* data = data0 ? (uchar*)data0 : (uchar*)fastMalloc(total);
        UMatData* u = புதிய UMatData(இது);
        u->data = u->origdata = data;
        u->size = total;
        எனில்(data0)
            u->flags |= UMatData::USER_ALLOCATED;

        திரும்பு u;
    }

    தருக்க allocate(UMatData* u, AccessFlag /*accessFlags*/, UMatUsageFlags /*usageFlags*/) மாறிலி CV_OVERRIDE
    {
        எனில்(!u) திரும்பு பொய்;
        திரும்பு உண்மை;
    }

    வெறுமை deallocate(UMatData* u) மாறிலி CV_OVERRIDE
    {
        எனில்(!u)
            திரும்பு;

        CV_Assert(u->urefcount == 0);
        CV_Assert(u->refcount == 0);
        எனில்( !(u->flags & UMatData::USER_ALLOCATED) )
        {
            fastFree(u->origdata);
            u->origdata = 0;
        }
        delete u;
    }
};

நிலையான
MatAllocator*& getDefaultAllocatorMatRef()
{
    நிலையான MatAllocator* g_matAllocator = Mat::getStdAllocator();
    திரும்பு g_matAllocator;
}

MatAllocator* Mat::getDefaultAllocator()
{
    திரும்பு getDefaultAllocatorMatRef();
}

வெறுமை Mat::setDefaultAllocator(MatAllocator* allocator)
{
    getDefaultAllocatorMatRef() = allocator;
}

MatAllocator* Mat::getStdAllocator()
{
    CV_SINGLETON_LAZY_INIT(MatAllocator, புதிய StdMatAllocator())
}

//==================================================================================================

வெறுமை setSize( Mat& m, முழு _dims, மாறிலி முழு* _sz, மாறிலி size_t* _steps, தருக்க autoSteps)
{
    CV_Assert( 0 <= _dims && _dims <= CV_MAX_DIM && _dims <= MatShape::MAX_DIMS);

    m.dims = _dims;
    m.size = MatShape(_dims, _sz);
    m.step[std::max(_dims-1, 0)] = CV_ELEM_SIZE(m.flags);
    க்கு (முழு i = _dims-2; i >= 0; i--) {
        size_t autostep = m.size[i+1]*m.step[i+1];
        எனில் (_steps) {
            m.step[i] = _steps[i];
            //CV_Assert(m.step[i] >= autostep);
        } இல்லையெனில் எனில் (autoSteps) {
            m.step[i] = autostep;
        } இல்லையெனில் {
            m.step[i] = 0;
        }
    }

    எனில்( _dims <= 2 )
    {
        m.cols = _dims == 0 ? 1 : _sz ? _sz[_dims > 1] : 0;
        m.rows = _dims < 2 ? 1 : _sz ? _sz[0] : 0;
    } இல்லையெனில் {
        m.cols = m.rows = -1;
    }
}

முழு updateContinuityFlag(முழு flags, முழு dims, மாறிலி முழு* size, மாறிலி size_t* step)
{
    முழு i, j;
    எனில் (dims <= 1)
        திரும்பு flags | Mat::CONTINUOUS_FLAG;
    க்கு( i = 0; i < dims; i++ )
    {
        எனில்( size[i] > 1 )
            நிறுத்து;
    }

    uint64 t = (uint64)size[std::min(i, dims-1)]*CV_MAT_CN(flags);
    க்கு( j = dims-1; j > i; j-- )
    {
        t *= size[j];
        எனில்( step[j]*size[j] < step[j-1] )
            நிறுத்து;
    }

    எனில்( j <= i && t == (uint64)(முழு)t )
        திரும்பு flags | Mat::CONTINUOUS_FLAG;
    திரும்பு flags & ~Mat::CONTINUOUS_FLAG;
}

வெறுமை Mat::updateContinuityFlag()
{
    flags = cv::updateContinuityFlag(flags, dims, size.p, step.p);
}

வெறுமை finalizeHdr(Mat& m)
{
    m.updateContinuityFlag();
    முழு d = m.dims;
    எனில்( d > 2 )
        m.rows = m.cols = -1;
    எனில்(m.u)
        m.datastart = m.data = m.u->data;
    எனில்( m.data && d > 0 )
    {
        m.datalimit = m.datastart + m.size[0]*m.step[0];
        எனில்( m.size[0] > 0 )
        {
            முழு lastdim = d > 0 ? d - 1 : 0;
            m.dataend = m.ptr() + m.size[lastdim]*m.step[lastdim];
            க்கு( முழு i = 0; i < d-1; i++ )
                m.dataend += (m.size[i] - 1)*m.step[i];
        }
        இல்லையெனில்
            m.dataend = m.datalimit;
    }
    இல்லையெனில்
        m.dataend = m.datalimit = m.data;
}

//======================================= Mat ======================================================

Mat::Mat() CV_NOEXCEPT
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{}

Mat::Mat(முழு _rows, முழு _cols, முழு _type)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_rows, _cols, _type);
}

Mat::Mat(முழு _rows, முழு _cols, முழு _type, மாறிலி Scalar& _s)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_rows, _cols, _type);
    *இது = _s;
}

Mat::Mat(Size _sz, முழு _type)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create( _sz.height, _sz.width, _type );
}

Mat::Mat(Size _sz, முழு _type, மாறிலி Scalar& _s)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_sz.height, _sz.width, _type);
    *இது = _s;
}

Mat::Mat(முழு _dims, மாறிலி முழு* _sz, முழு _type)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_dims, _sz, _type);
}

Mat::Mat(முழு _dims, மாறிலி முழு* _sz, முழு _type, மாறிலி Scalar& _s)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_dims, _sz, _type);
    *இது = _s;
}

Mat::Mat(மாறிலி std::vector<முழு>& _sz, முழு _type)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_sz, _type);
}

Mat::Mat(மாறிலி std::vector<முழு>& _sz, முழு _type, மாறிலி Scalar& _s)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_sz, _type);
    *இது = _s;
}

Mat::Mat(மாறிலி MatShape& _shape, முழு _type)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_shape, _type);
}

Mat::Mat(std::initializer_list<முழு> _shape, முழு _type)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_shape, _type);
}

Mat::Mat(மாறிலி MatShape& _shape, முழு _type, மாறிலி Scalar& _s)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_shape, _type);
    *இது = _s;
}

Mat::Mat(std::initializer_list<முழு> _shape, முழு _type, மாறிலி Scalar& _s)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_shape, _type);
    *இது = _s;
}

Mat::Mat(மாறிலி Mat& m)
    : flags(m.flags), dims(m.dims), rows(m.rows), cols(m.cols), data(m.data),
      datastart(m.datastart), dataend(m.dataend), datalimit(m.datalimit), allocator(m.allocator),
      u(m.u), size(m.size), step(m.step)
{
    எனில்( u )
        CV_XADD(&u->refcount, 1);
}

Mat::Mat(முழு _rows, முழு _cols, முழு _type, வெறுமை* _data, size_t _step)
    : flags(MAGIC_VAL + (_type & TYPE_MASK)), dims(2), rows(_rows), cols(_cols),
      data((uchar*)_data), datastart((uchar*)_data), dataend(0), datalimit(0),
      allocator(0), u(0), size(2)
{
    CV_Assert(total() == 0 || data != வெற்று);

    size_t esz = CV_ELEM_SIZE(_type), esz1 = CV_ELEM_SIZE1(_type);
    size_t minstep = cols * esz;
    எனில்( _step == AUTO_STEP )
    {
        _step = minstep;
    }
    இல்லையெனில்
    {
        CV_Assert( _step >= minstep );
        எனில் (_step % esz1 != 0)
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

Mat::Mat(Size _sz, முழு _type, வெறுமை* _data, size_t _step)
    : flags(MAGIC_VAL + (_type & TYPE_MASK)), dims(2), rows(_sz.height), cols(_sz.width),
      data((uchar*)_data), datastart((uchar*)_data), dataend(0), datalimit(0),
      allocator(0), u(0), size(2)
{
    CV_Assert(total() == 0 || data != வெற்று);

    size_t esz = CV_ELEM_SIZE(_type), esz1 = CV_ELEM_SIZE1(_type);
    size_t minstep = cols*esz;
    எனில்( _step == AUTO_STEP || (_step < minstep && rows == 1))
    {
        _step = minstep;
    }
    இல்லையெனில்
    {
        CV_CheckGE(_step, minstep, "");

        எனில் (_step % esz1 != 0)
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

Mat& Mat::operator = (மாறிலி Mat& m)
{
    எனில்( இது != &m )
    {
        எனில்( m.u )
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
    திரும்பு *இது;
}

Mat Mat::clone() மாறிலி
{
    Mat m;
    copyTo(m);
    திரும்பு m;
}

வெறுமை Mat::assignTo( Mat& m, முழு _type ) மாறிலி
{
    எனில்( _type < 0 )
        m = *இது;
    இல்லையெனில்
        convertTo(m, _type);
}

வெறுமை Mat::create(முழு _rows, முழு _cols, முழு _type)
{
    _type &= TYPE_MASK;
    எனில்( dims <= 2 && rows == _rows && cols == _cols && type() == _type && data )
        திரும்பு;
    முழு sz[] = {_rows, _cols};
    create(2, sz, _type);
}

வெறுமை Mat::create(Size _sz, முழு _type)
{
    create(_sz.height, _sz.width, _type);
}

வெறுமை Mat::createSameSize(உள்ளீடு_Array m, முழு type)
{
    MatShape msize = m.shape();
    create(msize, type);
}

வெறுமை Mat::fit(முழு _dims, மாறிலி முழு* _sizes, முழு _type)
{
    size_t oldTotalBytes = u ? u->size : 0;
    size_t esz = CV_ELEM_SIZE(_type), newTotal = _dims >= 0;
    க்கு (முழு i = 0; i < _dims; i++)
        newTotal *= _sizes[i];
    size_t newTotalBytes = newTotal*esz;
    எனில் (newTotalBytes > 0 && (!isContinuous() ||
                              newTotalBytes > oldTotalBytes ||
                              data != datastart)) {
        create(_dims, _sizes, _type);
    } இல்லையெனில் {
        flags = (flags & ~Mat::TYPE_MASK) | CV_MAT_TYPE(_type);
        முழு _dummy_size = 0;
        setSize(*இது, (_dims >= 0 ? _dims : 1), (_dims >= 0 ? _sizes : &_dummy_size), nullptr, உண்மை);
        finalizeHdr(*இது);
    }
}

வெறுமை Mat::fit(மாறிலி std::vector<முழு>& _shape, முழு _type)
{
    fit((முழு)_shape.size(), _shape.data(), _type);
}

வெறுமை Mat::fit(மாறிலி MatShape& _shape, முழு _type)
{
    fit(_shape.dims, _shape.p, _type);
    size.layout = _shape.layout;
    size.C = _shape.C;
}

வெறுமை Mat::fit(std::initializer_list<முழு> _shape, முழு _type)
{
    முழு new_shape[MatShape::MAX_DIMS];
    முழு new_ndims = (முழு)_shape.size();
    CV_Assert(new_ndims <= MatShape::MAX_DIMS);
    auto it = _shape.begin();
    க்கு (முழு i = 0; i < new_ndims; i++, ++it)
        new_shape[i] = *it;
    fit(new_ndims, new_shape, _type);
}

வெறுமை Mat::fit(முழு _rows, முழு _cols, முழு _type)
{
    _type &= TYPE_MASK;
    முழு sz[] = {_rows, _cols};
    fit(2, sz, _type);
}

வெறுமை Mat::fit(Size _sz, முழு _type)
{
    fit(_sz.height, _sz.width, _type);
}

வெறுமை Mat::fitSameSize(உள்ளீடு_Array m, முழு _type)
{
    fit(m.shape(), _type);
}

வெறுமை Mat::addref()
{
    எனில்( u )
        CV_XADD(&u->refcount, 1);
}

வெறுமை Mat::release()
{
    எனில்( u && CV_XADD(&u->refcount, -1) == 1 )
        deallocate();
    u = வெற்று;
    datastart = dataend = datalimit = data = 0;
    க்கு(முழு i = 0; i < dims; i++)
        size.p[i] = 0;
    flags = (flags & CV_MAT_TYPE_MASK) | MAGIC_VAL;
    dims = rows = cols = 0;
    size.clear();
}

size_t Mat::step1(முழு i) மாறிலி
{
    திரும்பு step.p[i] / elemSize1();
}

தருக்க Mat::empty() மாறிலி
{
    திரும்பு data == 0 || total() == 0;
}

size_t Mat::total() மாறிலி
{
    எனில்( dims <= 2 )
        திரும்பு (size_t)rows * cols;
    size_t p = 1;
    க்கு( முழு i = 0; i < dims; i++ )
        p *= size[i];
    திரும்பு p;
}

size_t Mat::total(முழு startDim, முழு endDim) மாறிலி
{
    CV_Assert( 0 <= startDim && startDim <= endDim);
    size_t p = 1;
    முழு endDim_ = endDim <= dims ? endDim : dims;
    க்கு( முழு i = startDim; i < endDim_; i++ )
        p *= size[i];
    திரும்பு p;
}

Mat::Mat(Mat&& m) CV_NOEXCEPT
    : flags(m.flags), dims(m.dims), rows(m.rows), cols(m.cols), data(m.data),
      datastart(m.datastart), dataend(m.dataend), datalimit(m.datalimit), allocator(m.allocator),
      u(m.u), size(m.size), step(m.step)
{
    m.flags = MAGIC_VAL; m.dims = m.rows = m.cols = 0;
    m.size.clear(); m.step.clear();
    m.data = வெற்று; m.datastart = வெற்று; m.dataend = வெற்று; m.datalimit = வெற்று;
    m.allocator = வெற்று;
    m.u = வெற்று;
}


Mat& Mat::operator = (Mat&& m)
{
    எனில் (இது == &m)
      திரும்பு *இது;

    release();
    flags = m.flags; dims = m.dims; rows = m.rows; cols = m.cols; data = m.data;
    size = m.size; step = m.step;
    datastart = m.datastart; dataend = m.dataend; datalimit = m.datalimit; allocator = m.allocator;
    u = m.u;
    m.flags = MAGIC_VAL; m.dims = m.rows = m.cols = 0;
    m.size.clear(); m.step.clear();
    m.data = வெற்று; m.datastart = வெற்று; m.dataend = வெற்று; m.datalimit = வெற்று;
    m.allocator = வெற்று;
    m.u = வெற்று;
    திரும்பு *இது;
}


வெறுமை Mat::create(முழு d0, மாறிலி முழு* _sizes, முழு _type)
{
    முழு sz1 = 1, d = d0;
    முழு i;
    எனில் (d == 0) {
        d = 1;
        _sizes = (மாறிலி முழு*)&sz1;
    }
    CV_Assert(0 <= d && d <= CV_MAX_DIM && _sizes);
    _type = CV_MAT_TYPE(_type);

    எனில்( data && d == dims && _type == type() )
    {
        க்கு( i = 0; i < d; i++ )
            எனில்( size[i] != _sizes[i] )
                நிறுத்து;
        எனில்( i == d )
            திரும்பு;
    }

    முழு _sizes_backup[CV_MAX_DIM]; // #5991
    எனில் (_sizes == (இது->size.p))
    {
        க்கு(i = 0; i < d; i++ )
            _sizes_backup[i] = _sizes[i];
        _sizes = _sizes_backup;
    }

    release();
    எனில்( d == 0 )
        திரும்பு;
    flags = (_type & CV_MAT_TYPE_MASK) | MAGIC_VAL;
    setSize(*இது, d, _sizes, 0, உண்மை);

    எனில்( total() > 0 )
    {
        MatAllocator *a = allocator, *a0 = getDefaultAllocator();
        எனில்(!a)
            a = a0;
        முயல்
        {
            u = a->allocate(dims, size.p, _type, 0, step.p, ACCESS_RW /* ignored */, USAGE_DEFAULT);
            CV_Assert(u != 0);
            allocator = a;
        }
        பிடி (...)
        {
            எனில் (a == a0)
                எறி;
            u = a0->allocate(dims, size.p, _type, 0, step.p, ACCESS_RW /* ignored */, USAGE_DEFAULT);
            CV_Assert(u != 0);
            allocator = a0;
        }
        CV_Assert( step[dims-1] == (size_t)CV_ELEM_SIZE(flags) );
    }

    addref();
    finalizeHdr(*இது);
    size.dims = dims = d0;
}

வெறுமை Mat::create(மாறிலி std::vector<முழு>& _sizes, முழு _type)
{
    create((முழு)_sizes.size(), _sizes.data(), _type);
}

வெறுமை Mat::create(மாறிலி MatShape& _shape, முழு _type)
{
    எனில் (_shape.dims < 0) {
        release();
        திரும்பு;
    }
    create(_shape.dims, _shape.p, _type);
    size.layout = _shape.layout;
    size.C = _shape.C;
}

வெறுமை Mat::create(std::initializer_list<முழு> _shape, முழு _type)
{
    முழு new_shape[MatShape::MAX_DIMS];
    முழு new_ndims = (முழு)_shape.size();
    CV_Assert(new_ndims <= MatShape::MAX_DIMS);
    auto it = _shape.begin();
    க்கு (முழு i = 0; i < new_ndims; i++, ++it)
        new_shape[i] = *it;

    create(new_ndims, new_shape, _type);
}

வெறுமை Mat::copySize(மாறிலி Mat& m)
{
    dims = m.dims;
    cols = m.cols;
    rows = m.rows;
    size = m.size;
    step = m.step;
}

வெறுமை Mat::deallocate()
{
    எனில்(u)
    {
        UMatData* u_ = u;
        u = வெற்று;
        (u_->currAllocator ? u_->currAllocator : allocator ? allocator : getDefaultAllocator())->unmap(u_);
    }
}

Mat::Mat(மாறிலி Mat& m, மாறிலி Range& _rowRange, மாறிலி Range& _colRange)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    CV_Assert(m.dims <= MatShape::MAX_DIMS);
    CV_Assert(m.dims >= 2 ||
              (m.dims == 1 && (_rowRange == Range::all() ||
                               _rowRange == Range(0, m.rows))));
    எனில்( m.dims > 2 )
    {
        Range rs[MatShape::MAX_DIMS];
        rs[0] = _rowRange;
        rs[1] = _colRange;
        க்கு( முழு i = 2; i < m.dims; i++ )
            rs[i] = Range::all();
        *இது = m(rs);
        திரும்பு;
    }

    *இது = m;
    முயல்
    {
        எனில்( _rowRange != Range::all() && _rowRange != Range(0,rows) )
        {
            CV_Assert( 0 <= _rowRange.start && _rowRange.start <= _rowRange.end
                       && _rowRange.end <= m.rows );
            size[0] = rows = _rowRange.size();
            data += step*_rowRange.start;
            flags |= SUBMATRIX_FLAG;
        }

        எனில்( _colRange != Range::all() && _colRange != Range(0,cols) )
        {
            CV_Assert( 0 <= _colRange.start && _colRange.start <= _colRange.end
                       && _colRange.end <= m.cols );
            size[dims > 1] = cols = _colRange.size();
            data += _colRange.start*elemSize();
            flags |= SUBMATRIX_FLAG;
        }
    }
    பிடி(...)
    {
        release();
        எறி;
    }

    updateContinuityFlag();

    எனில்( rows <= 0 || cols <= 0 )
    {
        release();
        rows = cols = 0;
    }
}


Mat::Mat(மாறிலி Mat& m, மாறிலி Rect& roi)
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
    எனில்( roi.width < m.cols || roi.height < m.rows )
        flags |= SUBMATRIX_FLAG;

    size[0] = rows;
    size[1] = cols;
    step[0] = m.step[0];
    step[1] = esz;
    updateContinuityFlag();

    addref();
    எனில்( rows <= 0 || cols <= 0 )
    {
        rows = cols = 0;
        release();
    }
}


Mat::Mat(முழு _dims, மாறிலி முழு* _sizes, முழு _type, வெறுமை* _data, மாறிலி size_t* _steps)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    flags |= CV_MAT_TYPE(_type);
    datastart = data = (uchar*)_data;
    எனில் (_dims > 0 || _data != nullptr)
        setSize(*இது, _dims, _sizes, _steps, உண்மை);
    finalizeHdr(*இது);
}


Mat::Mat(மாறிலி std::vector<முழு>& _sizes, முழு _type, வெறுமை* _data, மாறிலி size_t* _steps)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    flags |= CV_MAT_TYPE(_type);
    datastart = data = (uchar*)_data;
    முழு _dims = (முழு)_sizes.size();
    எனில் (_dims > 0 || _data != nullptr)
        setSize(*இது, _dims, _sizes.data(), _steps, உண்மை);
    finalizeHdr(*இது);
}

Mat::Mat(மாறிலி MatShape& _shape, முழு _type, வெறுமை* _data, மாறிலி size_t* _steps)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    flags |= CV_MAT_TYPE(_type);
    datastart = data = (uchar*)_data;
    எனில் (_shape.dims >= 0) {
        setSize(*இது, _shape.dims, _shape.p, _steps, உண்மை);
    }
    இல்லையெனில் {
        CV_Assert(!data);
    }
    finalizeHdr(*இது);
}

Mat::Mat(std::initializer_list<முழு> _shape, முழு _type, வெறுமை* _data, மாறிலி size_t* _steps)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    முழு new_shape[MatShape::MAX_DIMS];
    முழு _dims = (முழு)_shape.size();
    CV_Assert(_dims <= MatShape::MAX_DIMS);
    auto it = _shape.begin();
    க்கு (முழு i = 0; i < _dims; i++, ++it)
        new_shape[i] = *it;

    flags |= CV_MAT_TYPE(_type);
    datastart = data = (uchar*)_data;
    எனில் (_dims > 0 || _data != nullptr)
        setSize(*இது, _dims, new_shape, _steps, உண்மை);
    finalizeHdr(*இது);
}

Mat::Mat(மாறிலி Mat& m, மாறிலி Range* ranges)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    முழு d = m.dims;

    CV_Assert(ranges);
    க்கு( முழு i = 0; i < d; i++ )
    {
        Range r = ranges[i];
        CV_Assert( r == Range::all() || (0 <= r.start && r.start < r.end && r.end <= m.size[i]) );
    }
    *இது = m;
    க்கு( முழு i = 0; i < d; i++ )
    {
        Range r = ranges[i];
        எனில்( r != Range::all() && r != Range(0, size.p[i]))
        {
            size.p[i] = r.end - r.start;
            data += r.start*step.p[i];
            flags |= SUBMATRIX_FLAG;
        }
    }

    எனில் (d <= 2) {
        rows = d == 2 ? size[0] : 1;
        cols = d <= 0 ? (d >= 0) : size[d > 1];
    }
    updateContinuityFlag();
}

Mat::Mat(மாறிலி Mat& m, மாறிலி std::vector<Range>& ranges)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
    datalimit(0), allocator(0), u(0)
{
    முழு d = m.dims;

    CV_Assert((முழு)ranges.size() == d);
    க்கு (முழு i = 0; i < d; i++)
    {
        Range r = ranges[i];
        CV_Assert(r == Range::all() || (0 <= r.start && r.start < r.end && r.end <= m.size[i]));
    }
    *இது = m;
    க்கு (முழு i = 0; i < d; i++)
    {
        Range r = ranges[i];
        எனில் (r != Range::all() && r != Range(0, size.p[i]))
        {
            size.p[i] = r.end - r.start;
            data += r.start*step.p[i];
            flags |= SUBMATRIX_FLAG;
        }
    }

    எனில் (d <= 2) {
        rows = d == 2 ? size[0] : 1;
        cols = d <= 0 ? (d >= 0) : size[d > 1];
    }
    updateContinuityFlag();
}


Mat Mat::diag(முழு d) மாறிலி
{
    CV_Assert( dims == 2 );
    Mat m = *இது;
    size_t esz = elemSize();
    முழு len;

    எனில்( d >= 0 )
    {
        len = std::min(cols - d, rows);
        m.data += esz*d;
    }
    இல்லையெனில்
    {
        len = std::min(rows + d, cols);
        m.data -= step[0]*d;
    }
    CV_DbgAssert( len > 0 );

    m.size[0] = m.rows = len;
    m.size[1] = m.cols = 1;
    m.step[0] += (len > 1 ? esz : 0);

    m.updateContinuityFlag();

    எனில்( size() != Size(1,1) )
        m.flags |= SUBMATRIX_FLAG;

    திரும்பு m;
}


வெறுமை Mat::pop_back(size_t nelems)
{
    CV_Assert( nelems <= (size_t)size.p[0] );
    CV_Assert( dims >= 1 );

    எனில்( isSubmatrix() )
        *இது = rowRange(0, size.p[0] - (முழு)nelems);
    இல்லையெனில்
    {
        size.p[0] -= (முழு)nelems;
        எனில் (dims == 2)
            rows = size.p[0];
        இல்லையெனில் எனில் (dims == 1)
            cols = size.p[0];
        dataend -= nelems*step.p[0];
    }
}


வெறுமை Mat::தள்ளு_back(மாறிலி வெறுமை* elem)
{
    size_t r = size.p[0];
    எனில்( isSubmatrix() || dataend + step.p[0] > datalimit )
        reserve( std::max(r + 1, (r*3+1)/2) );

    size_t esz = elemSize();
    memcpy(data + r*step.p[0], elem, esz);
    size.p[0] = முழு(r + 1);
    எனில் (dims == 2)
        rows = size.p[0];
    இல்லையெனில் எனில் (dims == 1)
        cols = size.p[0];
    dataend += step.p[0];
    uint64 tsz = size.p[0];
    க்கு( முழு i = 1; i < dims; i++ )
        tsz *= size.p[i];
    எனில்( esz < step.p[0] || tsz != (uint64)(முழு)tsz )
        flags &= ~CONTINUOUS_FLAG;
}


வெறுமை Mat::reserve(size_t nelems)
{
    மாறிலி size_t MIN_SIZE = 64;

    CV_Assert( (முழு)nelems >= 0 );
    எனில்( !isSubmatrix() && step.p[0] != 0 && data + step.p[0]*nelems <= datalimit )
        திரும்பு;

    முழு r = size.p[0];
    எனில்( (size_t)r >= nelems)
        திரும்பு;

    MatShape newsize = size;
    size_t esz = elemSize();
    newsize.dims = std::max(newsize.dims, 1);
    எனில் (dims > 1 && r == 0 && step.p[0] == 0) {
        step.p[0] = size.p[1] * (dims == 2 ? esz : step.p[1]);
    }
    newsize.p[0] = (முழு)nelems;
    size_t newbytes = newsize.total() * esz;
    எனில் (newbytes < MIN_SIZE) {
        newsize.p[0] = 1;
        newsize.p[0] = முழு(MIN_SIZE / newsize.total() / esz);
    }

    Mat m(newsize, type());
    எனில்( r > 0)
    {
        Mat mpart = m.rowRange(0, r);
        copyTo(mpart);
    }

    *இது = m;
    size.p[0] = r;

    எனில் (dims == 2)
        rows = size.p[0];
    இல்லையெனில் எனில் (dims == 1)
        cols = size.p[0];
    dataend = data + step.p[0]*r;
}


வெறுமை Mat::reserveBuffer(size_t nbytes)
{
    size_t esz = 1;
    முழு mtype = CV_8UC1;
    எனில் (!empty())
    {
        எனில் (!isSubmatrix() && data + nbytes <= dataend)//Should it be datalimit?
            திரும்பு;
        esz = elemSize();
        mtype = type();
    }

    size_t nelems = (nbytes - 1) / esz + 1;

#எனில் SIZE_MAX > UINT_MAX
    CV_Assert(nelems <= size_t(INT_MAX)*size_t(INT_MAX));
    முழு newrows = nelems > size_t(INT_MAX) ? nelems > 0x400*size_t(INT_MAX) ? nelems > 0x100000 * size_t(INT_MAX) ? nelems > 0x40000000 * size_t(INT_MAX) ?
                  size_t(INT_MAX) : 0x40000000 : 0x100000 : 0x400 : 1;
#இல்லையெனில்
    முழு newrows = nelems > size_t(INT_MAX) ? 2 : 1;
#endif
    முழு newcols = (முழு)((nelems - 1) / newrows + 1);

    create(newrows, newcols, mtype);
}


வெறுமை Mat::resize(size_t nelems)
{
    முழு r = size.p[0];
    எனில்( r == (முழு)nelems )
        திரும்பு;
    CV_Assert( (முழு)nelems >= 0 );

    எனில்( isSubmatrix() || data + step.p[0]*nelems > datalimit )
        reserve(nelems);

    size.p[0] = (முழு)nelems;
    எனில் (dims == 2)
        rows = size.p[0];
    இல்லையெனில் எனில் (dims == 1)
        cols = size.p[0];
    dataend += (size.p[0] - r)*(int64_t)step.p[0];

    //updateContinuityFlag(*this);
}


வெறுமை Mat::resize(size_t nelems, மாறிலி Scalar& s)
{
    முழு saveRows = size.p[0];
    resize(nelems);

    எனில்( size.p[0] > saveRows )
    {
        Mat part = rowRange(saveRows, size.p[0]);
        part = s;
    }
}

வெறுமை Mat::push_back(மாறிலி Mat& elems)
{
    size_t r = size.p[0];
    size_t delta = elems.size.p[0];
    எனில்( delta == 0 )
        திரும்பு;
    எனில்( இது == &elems )
    {
        Mat tmp = elems;
        push_back(tmp);
        திரும்பு;
    }
    எனில்( !data )
    {
        *இது = elems.clone();
        திரும்பு;
    }
    CV_Assert(dims > 0);

    size.p[0] = elems.size.p[0];
    தருக்க eq = size == elems.size;
    size.p[0] = முழு(r);
    எனில்( !eq )
        CV_Error(cv::Error::StsUnmatchedSizes, "Pushed vector length is not equal to matrix row length");
    எனில்( type() != elems.type() )
        CV_Error(cv::Error::StsUnmatchedFormats, "Pushed vector type is not the same as matrix type");
    size_t esz = elemSize();
    size_t minstep = dims <= 1 ? esz : size.p[1] * (dims == 2 ? esz : step.p[1]);
    size_t step0 = step.p[0];
    எனில் (step0 < minstep) {
        எனில் (size.p[0] <= 1)
            step.p[0] = step0 = minstep;
    }

    எனில்( isSubmatrix() || dataend + step0*delta > datalimit )
        reserve( std::max(r + delta, (r*3+1)/2) );

    size.p[0] += முழு(delta);
    எனில் (dims == 2)
        rows = size.p[0];
    இல்லையெனில் எனில் (dims == 1)
        cols = size.p[0];
    dataend += step0*delta;

    //updateContinuityFlag(*this);

    எனில்( isContinuous() && elems.isContinuous() )
        memcpy(data + r*step0, elems.data, elems.total()*elems.elemSize());
    இல்லையெனில்
    {
        Mat part = rowRange(முழு(r), முழு(r + delta));
        elems.copyTo(part);
    }
}


வெறுமை Mat::locateROI( Size& wholeSize, Point& ofs ) மாறிலி
{
    CV_Assert( dims <= 2 && step[0] > 0 );
    size_t esz = elemSize(), minstep;
    ptrdiff_t delta1 = data - datastart, delta2 = dataend - datastart;

    எனில்( delta1 == 0 )
        ofs.x = ofs.y = 0;
    இல்லையெனில்
    {
        ofs.y = (முழு)(delta1/step[0]);
        ofs.x = (முழு)((delta1 - step[0]*ofs.y)/esz);
        CV_DbgAssert( data == datastart + ofs.y*step[0] + ofs.x*esz );
    }
    minstep = (ofs.x + cols)*esz;
    wholeSize.height = (முழு)((delta2 - minstep)/step[0] + 1);
    wholeSize.height = std::max(wholeSize.height, ofs.y + rows);
    wholeSize.width = (முழு)((delta2 - step*(wholeSize.height-1))/esz);
    wholeSize.width = std::max(wholeSize.width, ofs.x + cols);
}

Mat& Mat::adjustROI( முழு dtop, முழு dbottom, முழு dleft, முழு dright )
{
    CV_Assert( dims <= 2 && step[0] > 0 );
    Size wholeSize; Point ofs;
    size_t esz = elemSize();
    locateROI( wholeSize, ofs );
    முழு row1 = std::min(std::max(ofs.y - dtop, 0), wholeSize.height), row2 = std::max(0, std::min(ofs.y + rows + dbottom, wholeSize.height));
    முழு col1 = std::min(std::max(ofs.x - dleft, 0), wholeSize.width), col2 = std::max(0, std::min(ofs.x + cols + dright, wholeSize.width));
    எனில்(row1 > row2)
        std::swap(row1, row2);
    எனில்(col1 > col2)
        std::swap(col1, col2);

    எனில் (dims == 1) {
        data += (col1 - ofs.x)*(std::ptrdiff_t)esz;
        cols = col2 - col1;
        size.p[0] = cols;
    } இல்லையெனில் {
        data += (row1 - ofs.y)*(std::ptrdiff_t)step + (col1 - ofs.x)*(std::ptrdiff_t)esz;
        rows = row2 - row1; cols = col2 - col1;
        size.p[0] = rows; size.p[1] = cols;
        updateContinuityFlag();
    }
    திரும்பு *இது;
}

Mat Mat::reshape(முழு new_cn, முழு new_rows) மாறிலி
{
    முழு cn = channels();
    Mat hdr = *இது;

    எனில்( new_cn == 0 )
        new_cn = cn;

    எனில்( dims > 2 )
    {
        எனில்( new_rows == 0 )
        {
            // special case: just change the number of channnels; retain the same shape,
            // except for the last, innermost dimension
            CV_Assert(size[dims-1]*cn % new_cn == 0);
            hdr.flags = (hdr.flags & ~CV_MAT_CN_MASK) | ((new_cn-1) << CV_CN_SHIFT);
            hdr.step[dims-1] = CV_ELEM_SIZE(hdr.flags);
            hdr.size[dims-1] = hdr.size[dims-1]*cn / new_cn;
            திரும்பு hdr;
        }
        CV_Assert( new_rows > 0 );
        முழு sz[] = { new_rows, (முழு)(total()*cn/new_rows) };
        திரும்பு reshape(new_cn, 2, sz);
    }

    முழு total_width = cols * cn;

    எனில்( (new_cn > total_width || total_width % new_cn != 0) && new_rows == 0 )
        new_rows = rows * total_width / new_cn;

    எனில்( new_rows != 0 && new_rows != rows )
    {
        முழு total_size = total_width * rows;
        எனில்( !isContinuous() )
            CV_Error( cv::Error::BadStep,
            "The matrix is not continuous, thus its number of rows can not be changed" );

        total_width = total_size / new_rows;

        எனில்( total_width * new_rows != total_size )
            CV_Error( cv::Error::StsBadArg, "The total number of matrix elements "
                                    "is not divisible by the new number of rows" );

        hdr.size[0] = hdr.rows = new_rows;
        hdr.step[0] = total_width * elemSize1();
    } இல்லையெனில் {
        hdr.size[0] = hdr.rows = rows;
        எனில் (dims <= 1)
            hdr.step[0] = cols * CV_ELEM_SIZE(flags);
    }

    முழு new_width = total_width / new_cn;

    எனில்( new_width * new_cn != total_width )
        CV_Error( cv::Error::Bad_இலக்கம்_Channels,
        "The total width is not divisible by the new number of channels" );

    hdr.size.dims = hdr.dims = 2;
    hdr.size[1] = hdr.cols = new_width;
    hdr.flags = (hdr.flags & ~CV_MAT_CN_MASK) | ((new_cn-1) << CV_CN_SHIFT);
    hdr.step[1] = CV_ELEM_SIZE(hdr.flags);
    திரும்பு hdr;
}

Mat Mat::reshape(முழு _cn, முழு _newndims, மாறிலி முழு* _newsz) மாறிலி
{
    எனில்(_newndims == dims)
    {
        எனில்(_newsz == 0)
            திரும்பு reshape(_cn);
        எனில்(_newndims == 2)
            திரும்பு reshape(_cn, _newsz[0]);
    }

    எனில் (isContinuous())
    {
        CV_Assert(_cn >= 0 && _newndims >= 0 && _newndims <= CV_MAX_DIM && (_newndims == 0 || _newsz != 0));

        எனில் (_cn == 0)
            _cn = இது->channels();
        இல்லையெனில்
            CV_Assert(_cn <= CV_CN_MAX);

        size_t total_elem1_ref = இது->total() * இது->channels();
        size_t total_elem1 = _cn;

        AutoBuffer<முழு, 4> newsz_buf( (size_t)_newndims );

        முழு m1_idx = -1;

        க்கு (முழு i = 0; i < _newndims; i++)
        {
            எனில் (_newsz[i] >= 0) {
                எனில் (_newsz[i] == 0 && i < dims)
                    newsz_buf[i] = size.p[i];
                இல்லையெனில்
                    newsz_buf[i] = _newsz[i];
                total_elem1 *= (size_t)newsz_buf[i];
            } இல்லையெனில் {
                எனில் (m1_idx >= 0)
                    CV_Error(cv::Error::StsBadSize, "More than one '-1' occured in the new shape");
                m1_idx = i;
            }
        }

        எனில் (m1_idx >= 0) {
            எனில் (total_elem1 == 0) {
                CV_Assert(total_elem1_ref == 0);
                total_elem1 = 1;
            }
            CV_Assert(total_elem1_ref % total_elem1 == 0);
            newsz_buf[m1_idx] = (முழு)(total_elem1_ref / total_elem1);
        } இல்லையெனில் எனில் (total_elem1 != total_elem1_ref) {
            CV_Error(cv::Error::StsUnmatchedSizes, "Requested and source matrices have different count of elements");
        }

        Mat hdr = *இது;
        hdr.flags = (hdr.flags & ~CV_MAT_CN_MASK) | ((_cn-1) << CV_CN_SHIFT);
        setSize(hdr, _newndims, newsz_buf.data(), வெற்று, உண்மை);

        திரும்பு hdr;
    }

    CV_Error(cv::Error::StsNotImplemented, "Reshaping of n-dimensional non-continuous matrices is not supported yet");
    // TBD
}

Mat Mat::reshape(முழு _cn, மாறிலி std::vector<முழு>& _newshape) மாறிலி
{
    முழு newdims = (முழு)_newshape.size();
    எனில்(newdims == 0 && empty())
    {
        திரும்பு *இது;
    }

    திரும்பு reshape(_cn, newdims, newdims > 0 ? &_newshape[0] : 0);
}

Mat Mat::reshape(முழு _cn, மாறிலி MatShape& _newshape) மாறிலி
{
    எனில் (_newshape.dims < 0) {
        முழு newshape[] = {0};
        திரும்பு reshape(_cn, 1, newshape);
    }
    திரும்பு reshape(_cn, _newshape.dims, _newshape.p);
}

Mat Mat::reshape(முழு _cn, std::initializer_list<முழு> newshape_) மாறிலி
{
    முழு newshape[MatShape::MAX_DIMS];
    size_t i, newshape_dims = newshape_.size();
    CV_Assert(newshape_dims <= (size_t)MatShape::MAX_DIMS);
    auto it = newshape_.begin();
    க்கு (i = 0; i < newshape_dims; i++, ++it)
        newshape[i] = *it;
    திரும்பு reshape(_cn, (முழு)newshape_dims, newshape);
}

Mat Mat::reinterpret(முழு type) மாறிலி
{
    type = CV_MAT_TYPE(type);
    CV_Assert(CV_ELEM_SIZE(இது->type()) == CV_ELEM_SIZE(type));
    Mat m = *இது;
    m.flags = (m.flags & ~CV_MAT_TYPE_MASK) | type;
    m.updateContinuityFlag();
    திரும்பு m;
}

Mat Mat::diag(மாறிலி Mat& d)
{
    CV_Assert( d.cols == 1 || d.rows == 1 );
    முழு len = d.rows + d.cols - 1;
    Mat m(len, len, d.type(), Scalar(0));
    Mat md = m.diag();
    எனில்( d.cols == 1 )
        d.copyTo(md);
    இல்லையெனில்
        transpose(d, md);
    திரும்பு m;
}

முழு Mat::checkVector(முழு _elemChannels, முழு _depth, தருக்க _requireContinuous) மாறிலி
{
    திரும்பு data && (depth() == _depth || _depth <= 0) &&
        (isContinuous() || !_requireContinuous) &&
        ((dims <= 2 && (((rows == 1 || cols == 1) && channels() == _elemChannels) ||
                        (cols == _elemChannels && channels() == 1))) ||
        (dims == 3 && channels() == 1 && size.p[2] == _elemChannels && (size.p[0] == 1 || size.p[1] == 1) &&
         (isContinuous() || step.p[1] == step.p[2]*size.p[2])))
    ? (முழு)(total()*channels()/_elemChannels) : -1;
}


நிலையான உள்வரி Size getContinuousSize_(முழு flags, முழு cols, முழு rows, முழு widthScale)
{
    int64 sz = (int64)cols * rows * widthScale;
    தருக்க has_int_overflow = sz >= INT_MAX;
    தருக்க isContiguous = (flags & Mat::CONTINUOUS_FLAG) != 0;
    திரும்பு (isContiguous && !has_int_overflow)
            ? Size((முழு)sz, 1)
            : Size(cols * widthScale, rows);
}

Size getContinuousSize2D(Mat& m1, முழு widthScale)
{
    CV_CheckLE(m1.dims, 2, "");
    திரும்பு getContinuousSize_(m1.flags,
                              m1.cols, m1.rows, widthScale);
}
Size getContinuousSize2D(Mat& m1, Mat& m2, முழு widthScale)
{
    CV_CheckLE(m1.dims, 2, "");
    CV_CheckLE(m2.dims, 2, "");
    மாறிலி Size sz1 = m1.size();
    எனில் (sz1 != m2.size())  // reshape all matrixes to the same size (#4159)
    {
        size_t total_sz = m1.total();
        CV_CheckEQ(total_sz, m2.total(), "");
        தருக்க is_m1_vector = m1.cols == 1 || m1.rows == 1;
        தருக்க is_m2_vector = m2.cols == 1 || m2.rows == 1;
        CV_Assert(is_m1_vector); CV_Assert(is_m2_vector);
        முழு total = (முழு)total_sz;  // vector-column
        தருக்க isContiguous = ((m1.flags & m2.flags) & Mat::CONTINUOUS_FLAG) != 0;
        தருக்க has_int_overflow = ((int64)total_sz * widthScale) >= INT_MAX;
        எனில் (isContiguous && !has_int_overflow)
            total = 1; // vector-row
        m1 = m1.reshape(0, total);
        m2 = m2.reshape(0, total);
        CV_Assert(m1.cols == m2.cols && m1.rows == m2.rows);
        திரும்பு Size(m1.cols * widthScale, m1.rows);
    }
    திரும்பு getContinuousSize_(m1.flags & m2.flags,
                              m1.cols, m1.rows, widthScale);
}

Size getContinuousSize2D(Mat& m1, Mat& m2, Mat& m3, முழு widthScale)
{
    CV_CheckLE(m1.dims, 2, "");
    CV_CheckLE(m2.dims, 2, "");
    CV_CheckLE(m3.dims, 2, "");
    மாறிலி Size sz1 = m1.size();
    எனில் (sz1 != m2.size() || sz1 != m3.size())  // reshape all matrixes to the same size (#4159)
    {
        size_t total_sz = m1.total();
        CV_CheckEQ(total_sz, m2.total(), "");
        CV_CheckEQ(total_sz, m3.total(), "");
        தருக்க is_m1_vector = m1.cols == 1 || m1.rows == 1;
        தருக்க is_m2_vector = m2.cols == 1 || m2.rows == 1;
        தருக்க is_m3_vector = m3.cols == 1 || m3.rows == 1;
        CV_Assert(is_m1_vector); CV_Assert(is_m2_vector); CV_Assert(is_m3_vector);
        முழு total = (முழு)total_sz;  // vector-column
        தருக்க isContiguous = ((m1.flags & m2.flags & m3.flags) & Mat::CONTINUOUS_FLAG) != 0;
        தருக்க has_int_overflow = ((int64)total_sz * widthScale) >= INT_MAX;
        எனில் (isContiguous && !has_int_overflow)
            total = 1; // vector-row
        m1 = m1.reshape(0, total);
        m2 = m2.reshape(0, total);
        m3 = m3.reshape(0, total);
        CV_Assert(m1.cols == m2.cols && m1.rows == m2.rows && m1.cols == m3.cols && m1.rows == m3.rows);
        திரும்பு Size(m1.cols * widthScale, m1.rows);
    }
    திரும்பு getContinuousSize_(m1.flags & m2.flags & m3.flags,
                              m1.cols, m1.rows, widthScale);
}

} // cv::
