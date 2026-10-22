// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#ضمن "precomp.hpp"
#ضمن "bufferpool.impl.hpp"

نطاق cv {

std::string layoutToString(DataLayout layout)
{
    החזר
        layout == DATA_LAYOUT_ND ? "ND" :
        layout == DATA_LAYOUT_NCHW ? "NCHW" :
        layout == DATA_LAYOUT_NHWC ? "NHWC" :
        layout == DATA_LAYOUT_BLOCK ? "BLOCK" :
        layout == DATA_LAYOUT_NCDHW ? "NCDHW" :
        layout == DATA_LAYOUT_NDHWC ? "NDHWC" :
        layout == DATA_LAYOUT_PLANAR ? "PLANAR" :
        layout == DATA_LAYOUT_UNKNOWN ? "Unknown" : "???";
}

منطقي operator == (ثابت MatShape& size1, ثابت MatShape& size2)
{
    אם (size1.dims != size2.dims)
        החזר שקר;
    אם (size1.layout != size2.layout &&
        size1.layout != DATA_LAYOUT_UNKNOWN &&
        size2.layout != DATA_LAYOUT_UNKNOWN)
        החזר שקר;
    אם (size1.layout == DATA_LAYOUT_BLOCK &&
        size2.layout == DATA_LAYOUT_BLOCK &&
        size1.C != size2.C)
        החזר שקר;
    עבור (صحيح i = 0; i < size1.dims; i++) {
        אם (size1.p[i] != size2.p[i])
            החזר שקר;
    }
    החזר אמת;
}

منطقي operator != (ثابت MatShape& size1, ثابت MatShape& size2)
{
    החזר !(size1 == size2);
}

/////////////////////////// MatShape ////////////////////////////////

MatShape MatShape::scalar()
{
    החזר MatShape(0);
}

فراغ MatShape::clear()
{
    dims = -1;
    layout = DATA_LAYOUT_UNKNOWN;
    C = 0;
    עבור (صحيح i = 0; i < MAX_DIMS; i++)
        p[i] = 0;
}

فراغ MatShape::resize(size_t newSize, صحيح value)
{
    CV_Assert(newSize < (size_t)MAX_DIMS);
    صحيح old_dims = std::max(dims, 0);
    dims = (صحيح)newSize;
    עבור (صحيح i = old_dims; i < dims; i++)
        p[i] = value;
}

فراغ MatShape::reserve(size_t)
{
    // no op; maybe need to add a check for overflow, but we check it anyway in other operations
}

فراغ MatShape::assign(size_t newSize, صحيح value)
{
    CV_Assert(newSize < (size_t)MAX_DIMS);
    dims = (صحيح)newSize;
    עבור (صحيح i = 0; i < dims; i++)
        p[i] = value;
}

فراغ MatShape::assign(صحيح newSize, صحيح value)
{
    assign((size_t)newSize, value);
}

فراغ MatShape::assign(ثابت صحيح* begin, ثابت صحيح* end)
{
    assign_(begin, end);
}

فراغ MatShape::assign_(ثابت صحيح* begin, ثابت صحيح* end)
{
    ptrdiff_t newSize = end - begin;
    CV_Assert(0 <= newSize && newSize < (ptrdiff_t)MAX_DIMS);
    dims = (صحيح)newSize;
    עבור (صحيح i = 0; i < dims; i++)
        p[i] = begin[i];
}

صحيح* MatShape::begin() { החזר p; }
ثابت صحيح* MatShape::begin() ثابت { החזר p; }
صحيح* MatShape::end() { החזר p + std::max(dims, 0); }
ثابت صحيح* MatShape::end() ثابت { החזר p + std::max(dims, 0); }
صحيح& MatShape::back() { החזר p[std::max(dims-1, 0)]; }
ثابت صحيح& MatShape::back() ثابت { החזר p[std::max(dims-1, 0)]; }

فراغ MatShape::push_back(صحيح value)
{
    CV_Assert(dims+1 < MAX_DIMS);
    dims = std::max(dims+1, 1);
    p[dims-1] = value;
}

فراغ MatShape::emplace_back(صحيح value)
{
    push_back(value);
}

فراغ MatShape::insert(صحيح* where, صحيح value)
{
    صحيح old_dims = std::max(dims, 0);
    CV_Assert(old_dims+1 < MAX_DIMS);
    ptrdiff_t ofs = where - p;
    CV_Assert(0 <= ofs && ofs <= old_dims);
    dims = old_dims+1;
    עבור (صحيح i = old_dims-1; i >= (صحيح)ofs; i--)
        p[i+1] = p[i];
    p[ofs] = value;
}

فراغ MatShape::insert(صحيح* where, size_t count, صحيح value)
{
    صحيح old_dims = std::max(dims, 0);
    CV_Assert((size_t)(old_dims+count) < (size_t)MAX_DIMS);
    ptrdiff_t ofs = where - p;
    CV_Assert(0 <= ofs && ofs <= old_dims);
    dims = (صحيح)(old_dims+count);
    עבור (صحيح i = old_dims-1; i >= (صحيح)ofs; i--)
        p[i+count] = p[i];
    עבור (صحيح i = 0; i < (صحيح)count; i++)
        p[i+ofs] = value;
}

فراغ MatShape::insert(صحيح* where, صحيح count, صحيح value)
{
    insert(where, (size_t)count, value);
}

فراغ MatShape::insert(صحيح* where, ثابت صحيح* begin, ثابت صحيح* end)
{
    insert_(where, begin, end);
}

فراغ MatShape::insert_(صحيح* where, ثابت صحيح* begin, ثابت صحيح* end)
{
    صحيح old_dims = std::max(dims, 0);
    ptrdiff_t delta = end - begin;
    CV_Assert(0 <= delta && old_dims+delta < MAX_DIMS);
    ptrdiff_t ofs = where - p;
    CV_Assert(0 <= ofs && ofs <= old_dims);
    dims = (صحيح)(old_dims+delta);
    עבור (صحيح i = old_dims-1; i >= (صحيح)ofs; i--)
        p[i+delta] = p[i];
    עבור (صحيح i = 0; i < (صحيح)delta; i++)
        p[i+ofs] = begin[i];
}

فراغ MatShape::erase(صحيح* where)
{
    CV_Assert(dims > 0);
    ptrdiff_t ofs = where - p;
    CV_Assert(0 <= ofs && ofs <= dims);
    אם (ofs == dims)
        החזר;
    dims--;
    עבור (صحيح i = (صحيح)ofs+1; i <= dims; i++)
        p[i-1] = p[i];
}

size_t MatShape::total() ثابت
{
    size_t result = 1;
    אם (dims < 0)
        החזר 0;
    עבור (صحيح i = 0; i < dims; i++)
        result *= p[i];
    החזר result;
}

std::vector<صحيح> MatShape::vec() ثابت
{
    אם (dims < 0)
        החזר std::vector<صحيح>(1, 0);
    החזר std::vector<صحيح>(p, p + dims);
}

std::string MatShape::str() ثابت
{
    std::stringstream sstrm;
    אם (empty()) {
        sstrm << "<empty>";
    } אחרת אם (dims == 0) {
        sstrm << "<scalar>";
    } אחרת {
        sstrm << "[";
        עבור (صحيح i = 0; i < dims; i++) {
            sstrm << (i > 0 ? " x " : "") << p[i];
        }
        sstrm << "]";
    }
    החזר sstrm.str();
}

ساكن فراغ finalizeBlockLayout(MatShape& size, صحيح C=0)
{
    אם (size.layout == DATA_LAYOUT_BLOCK) {
        CV_Assert(size.dims >= 4);
        صحيح C0 = size.p[size.dims-1];
        CV_Assert(C0 > 1 && (C0 & (C0-1)) == 0);
        size.C = C > 0 ? C : size.p[1]*size.p[size.dims-1];
    } אחרת {
        size.C = 0;
    }
    עבור (صحيح i = std::max(size.dims, 0); i < MatShape::MAX_DIMS; i++)
        size.p[i] = 0;
    אם (size.dims == 0)
        size.p[0] = 1;
}

MatShape::MatShape()
{
    clear();
}

MatShape::MatShape(size_t dims_, ثابت صحيح* size_, DataLayout layout_, صحيح C_)
{
    layout = layout_;
    CV_Assert(dims_ <= (size_t)MAX_DIMS);
    dims = (صحيح)dims_;
    עבור (صحيح i = 0; i < dims; i++) {
        p[i] = size_ ? size_[i] : 0;
    }
    finalizeBlockLayout(*هذا, C_);
}

MatShape::MatShape(size_t dims_, صحيح value, DataLayout layout_)
{
    layout = layout_;
    CV_Assert(dims_ <= (size_t)MAX_DIMS);
    dims = (صحيح)dims_;
    עבור (صحيح i = 0; i < dims; i++) {
        p[i] = value;
    }
    finalizeBlockLayout(*هذا, 0);
}

MatShape::MatShape(std::initializer_list<صحيح> shape)
{
    layout = DATA_LAYOUT_UNKNOWN;
    CV_Assert(shape.size() <= (size_t)MAX_DIMS);
    dims = (صحيح)shape.size();
    auto it = shape.begin();
    עבור (صحيح i = 0; i < dims; i++, ++it) {
        p[i] = *it;
    }
    finalizeBlockLayout(*هذا, 0);
}

MatShape::MatShape(صحيح dims_, صحيح value, DataLayout layout_)
{
    layout = layout_;
    CV_Assert(dims_ <= MAX_DIMS);
    dims = dims_;
    עבור (صحيح i = 0; i < dims; i++) {
        p[i] = value;
    }
    finalizeBlockLayout(*هذا, 0);
}

MatShape::MatShape(ثابت std::vector<صحيح>& shape_, DataLayout layout_, صحيح C_)
{
    layout = layout_;
    size_t shape_size = shape_.size();
    CV_Assert(shape_size < (size_t)MAX_DIMS);
    dims = (صحيح)shape_size;
    עבור (صحيح i = 0; i < dims; i++) {
        p[i] = shape_[i];
    }
    finalizeBlockLayout(*هذا, C_);
}

MatShape::MatShape(ثابت صحيح* begin, ثابت صحيح* end, DataLayout layout_, صحيح C_)
{
    layout = layout_;
    ptrdiff_t shape_size = end - begin;
    CV_Assert(0 <= shape_size && shape_size < MAX_DIMS);
    dims = (صحيح)shape_size;
    עבור (صحيح i = 0; i < dims; i++) {
        p[i] = begin[i];
    }
    finalizeBlockLayout(*هذا, C_);
}

MatShape::MatShape(ثابت MatShape& shape)
{
    dims = shape.dims;
    layout = shape.layout;
    C = shape.C;
    עבור (صحيح i = 0; i < MAX_DIMS; i++)
        p[i] = shape.p[i];
}

MatShape& MatShape::operator = (ثابت MatShape& shape)
{
    אם (هذا != &shape) {
        dims = shape.dims;
        layout = shape.layout;
        C = shape.C;
        עבור (صحيح i = 0; i < MAX_DIMS; i++)
            p[i] = shape.p[i];
    }
    החזר *هذا;
}

منطقي MatShape::hasSymbols() ثابت
{
    עבור (صحيح i = 0; i < dims; i++) {
        אם (p[i] < 0)
            החזר אמת;
    }
    החזר שקר;
}

صحيح MatShape::channels() ثابت
{
    CV_Assert(layout == DATA_LAYOUT_BLOCK || layout == DATA_LAYOUT_NCHW || layout == DATA_LAYOUT_NHWC);
    החזר layout == DATA_LAYOUT_BLOCK ? C : p[layout == DATA_LAYOUT_NCHW ? 1 : dims-1];
}

MatShape MatShape::toLayout(DataLayout newLayout, صحيح C0) ثابت
{
    CV_Assert(layout == DATA_LAYOUT_BLOCK || layout == DATA_LAYOUT_NCHW || layout == DATA_LAYOUT_NHWC);
    CV_Assert(newLayout == DATA_LAYOUT_BLOCK || newLayout == DATA_LAYOUT_NCHW || newLayout == DATA_LAYOUT_NHWC);

    MatShape newsize = *هذا;
    newsize.layout = newLayout;

    אם (newLayout == DATA_LAYOUT_BLOCK) {
        // any => BLOCK
        CV_Assert_N(C0 > 1, (C0 & (C0-1)) == 0);
        صحيح Corig = channels();
        newsize.C = Corig;

        אם (layout == DATA_LAYOUT_NHWC) {
            עבור (صحيح i = 2; i < dims; i++) {
                newsize.p[i] = p[i-1];
            }
        }

        newsize.dims += layout != DATA_LAYOUT_BLOCK;
        newsize.p[1] = (Corig + C0 - 1)/C0;
        newsize.p[newsize.dims-1] = C0;
    } אחרת אם (layout == DATA_LAYOUT_BLOCK) {
        // BLOCK => any (except for BLOCK, which is handled above)
        newsize.C = 0;
        אם (newLayout == DATA_LAYOUT_NHWC) {
            עבור (صحيح i = 2; i < dims; i++) {
                newsize.p[i-1] = p[i];
            }
        }
        newsize.p[newLayout == DATA_LAYOUT_NCHW ? 1 : dims-2] = C;
        newsize.dims--;
    } אחרת {
        CV_Assert_N(C0 <= 1);

        אם (newLayout == layout)
            החזר newsize;

        // NHWC => NCHW
        אם (newLayout == DATA_LAYOUT_NCHW) {
            עבור (صحيح i = 2; i < dims; i++)
                newsize.p[i] = p[i-1];
            newsize.p[1] = p[dims-1];
        } אחרת {
            // NCHW => NHWC
            עבור (صحيح i = 2; i < dims; i++)
                newsize.p[i-1] = p[i];
            newsize.p[dims-1] = p[1];
        }
    }

    החזר newsize;
}

ساكن صحيح normalizeShapeAxis(صحيح axis, صحيح dims)
{
    CV_CheckGE(axis, -dims, "axis is out of range");
    CV_CheckLT(axis, dims, "axis is out of range");
    החזר axis < 0 ? axis + dims : axis;
}

// The semantic NCHW form {N, C, spatial...} of a block ('NC1HWC0') shape, so that axes and
// element counts are the logical ones rather than the C0-padded ones.
ساكن MatShape toSemantic(ثابت MatShape& shape)
{
    CV_CheckGE(shape.dims, 3, "a block shape is {N, C1, spatial..., C0}");
    החזר shape.toLayout(DATA_LAYOUT_NCHW);
}

// Blocks such a result again. Once N or C changes, elements cross the padded C0 lanes and
// the data needs a repack, which a shape operation cannot express.
ساكن MatShape toBlockOf(MatShape result, ثابت MatShape& src, ثابت حرف* op)
{
    אם (result.dims < 2 || result.p[0] != src.p[0] || result.p[1] != src.C)
        CV_Error_(Error::StsNotImplemented,
                  ("%s: the block layout is kept only when the batch and the channel axes stay "
                   "unchanged; convert the shape with toLayout() first", op));
    CV_CheckLE(result.dims + 1, (صحيح)MatShape::MAX_DIMS, "too many dimensions");
    result.layout = DATA_LAYOUT_NCHW;
    החזר result.toLayout(DATA_LAYOUT_BLOCK, src.p[src.dims-1]);
}

MatShape MatShape::squeeze(ثابت std::vector<صحيح>& axes) ثابت
{
    CV_Assert(dims >= 0);
    אם (layout == DATA_LAYOUT_BLOCK)
        החזר toBlockOf(toSemantic(*هذا).squeeze(axes), *هذا, "squeeze");
    منطقي remove[MAX_DIMS] = {שקר};
    אם (axes.empty())
    {
        עבור (صحيح i = 0; i < dims; i++)
            remove[i] = p[i] == 1;
    }
    עבור (صحيح a : axes)
    {
        صحيح i = normalizeShapeAxis(a, dims);
        CV_Check(a, !remove[i], "squeeze: duplicate axis");
        CV_Check(p[i], p[i] == 1, "squeeze: the size of a squeezed axis must be 1");
        remove[i] = אמת;
    }
    // a new number of axes: the layout of this shape does not apply
    MatShape result;
    result.dims = 0;
    עבור (صحيح i = 0; i < dims; i++)
        אם (!remove[i])
            result.p[result.dims++] = p[i];
    החזר result;
}

MatShape MatShape::unsqueeze(ثابت std::vector<صحيح>& axes) ثابت
{
    CV_Assert(dims >= 0);
    אם (layout == DATA_LAYOUT_BLOCK)
        החזר toBlockOf(toSemantic(*هذا).unsqueeze(axes), *هذا, "unsqueeze");
    ثابت صحيح outDims = dims + (صحيح)axes.size();
    CV_CheckLE(outDims, (صحيح)MAX_DIMS, "unsqueeze: too many dimensions");
    منطقي insert[MAX_DIMS] = {שקר};
    עבור (صحيح a : axes)
    {
        صحيح i = normalizeShapeAxis(a, outDims);
        CV_Check(a, !insert[i], "unsqueeze: duplicate axis");
        insert[i] = אמת;
    }
    // a new number of axes: the layout of this shape does not apply
    MatShape result;
    result.dims = outDims;
    עבור (صحيح i = 0, j = 0; i < outDims; i++)
        result.p[i] = insert[i] ? 1 : p[j++];
    החזר result;
}

MatShape MatShape::flatten(صحيح startAxis, صحيح endAxis) ثابت
{
    CV_Assert(dims >= 0);
    אם (layout == DATA_LAYOUT_BLOCK)
        החזר toBlockOf(toSemantic(*هذا).flatten(startAxis, endAxis), *هذا, "flatten");
    אם (dims == 0)
    {
        CV_Check(startAxis, startAxis == 0 || startAxis == -1, "flatten: axis is out of range");
        CV_Check(endAxis, endAxis == 0 || endAxis == -1, "flatten: axis is out of range");
        החזר MatShape({1});
    }
    صحيح start = normalizeShapeAxis(startAxis, dims), end = normalizeShapeAxis(endAxis, dims);
    CV_CheckLE(start, end, "flatten: startAxis must not follow endAxis");
    int64 merged = 1;
    עבור (صحيح i = start; i <= end; i++)
        merged *= p[i];
    CV_CheckLE((مزدوج)merged, (مزدوج)INT_MAX, "flatten: the result is too big");
    // a new number of axes: the layout of this shape does not apply
    MatShape result;
    result.dims = 0;
    עבור (صحيح i = 0; i < dims; i++)
    {
        אם (i == start)
            result.p[result.dims++] = (صحيح)merged;
        אחרת אם (i < start || i > end)
            result.p[result.dims++] = p[i];
    }
    החזר result;
}

MatShape MatShape::reshape(ثابت MatShape& newShape, منطقي allowZero) ثابت
{
    CV_Assert(dims >= 0 && newShape.dims >= 0);
    אם (layout == DATA_LAYOUT_BLOCK)
        החזר toBlockOf(toSemantic(*هذا).reshape(newShape, allowZero), *هذا, "reshape");
    // a new number of axes: neither the layout of this shape nor the one of the spec applies
    MatShape result;
    result.dims = newShape.dims;
    std::copy(newShape.p, newShape.p + newShape.dims, result.p);
    صحيح inferred = -1;
    int64 known = 1;
    עבור (صحيح i = 0; i < result.dims; i++)
    {
        صحيح& sz = result.p[i];
        אם (sz == -1)
        {
            CV_Check(i, inferred < 0, "reshape: at most one axis may be -1");
            inferred = i;
            استمر;
        }
        CV_Check(sz, sz >= 0, "reshape: sizes must be -1 or non-negative");
        אם (sz == 0 && !allowZero)
        {
            CV_Check(i, i < dims, "reshape: 0 refers to an axis the source does not have");
            sz = p[i];
        }
        known *= sz;
    }
    ثابت int64 total_ = (int64)total();
    אם (inferred >= 0)
    {
        CV_Check((مزدوج)known, known != 0, "reshape: cannot infer the -1 axis next to a zero-size axis");
        CV_Check((مزدوج)total_, total_ % known == 0, "reshape: the number of elements does not match");
        CV_CheckLE((مزدوج)(total_/known), (مزدوج)INT_MAX, "reshape: the result is too big");
        result.p[inferred] = (صحيح)(total_/known);
    }
    אחרת אם (known != total_)
        CV_Error_(Error::StsBadArg, ("reshape: the number of elements does not match: %lld vs %lld",
                                     (long long)known, (long long)total_));
    החזר result;
}

MatShape MatShape::expand(ثابت MatShape& another) ثابت
{
    אם (dims == 0)
        החזר another;
    אם (another.dims == 0)
        החזר *هذا;

    אם ((layout == DATA_LAYOUT_NCHW || layout == DATA_LAYOUT_NHWC) &&
        (another.layout == DATA_LAYOUT_NCHW || another.layout == DATA_LAYOUT_NHWC)) {
        CV_Assert(layout == another.layout);
    }
    // [TODO] support block layout
    CV_Assert(layout != DATA_LAYOUT_BLOCK && another.layout != DATA_LAYOUT_BLOCK);

    MatShape result;

    אם (dims < 0 || another.dims < 0)
        החזר result;

    result = *هذا;
    result.dims = std::max(dims, another.dims);
    result.layout = layout == DATA_LAYOUT_UNKNOWN ? another.layout :
        layout == DATA_LAYOUT_ND && (another.layout == DATA_LAYOUT_NCHW ||
        another.layout == DATA_LAYOUT_NHWC) ? another.layout : layout;
    עבור (صحيح i = result.dims-1; i >= 0; i--) {
        صحيح i1 = i - (result.dims - dims);
        صحيح i2 = i - (result.dims - another.dims);
        صحيح sz1 = i1 < 0 ? 1 : p[i1];
        صحيح sz2 = i2 < 0 ? 1 : another.p[i2];
        CV_Assert(sz1 == sz2 || sz1 == 1 || sz2 == 1);
        // [TODO] handle symbolic shapes
        result.p[i] = sz1 != 1 ? sz1 : sz2;
    }
    החזר result;
}

/////////////////////////// MatAllocator ////////////////////////////

فراغ MatAllocator::map(UMatData*, AccessFlag) ثابت
{
}

فراغ MatAllocator::unmap(UMatData* u) ثابت
{
    אם(u->urefcount == 0 && u->refcount == 0)
    {
        deallocate(u);
    }
}

فراغ MatAllocator::download(UMatData* u, فراغ* dstptr,
         صحيح dims, ثابت size_t sz[],
         ثابت size_t srcofs[], ثابت size_t srcstep[],
         ثابت size_t dststep[]) ثابت
{
    אם(!u)
        החזר;
    صحيح isz[CV_MAX_DIM];
    uchar* srcptr = u->data;
    עבור( صحيح i = 0; i < dims; i++ )
    {
        CV_Assert( sz[i] <= (size_t)INT_MAX );
        אם( sz[i] == 0 )
            החזר;
        אם( srcofs )
            srcptr += srcofs[i]*(i <= dims-2 ? srcstep[i] : 1);
        isz[i] = (صحيح)sz[i];
    }

    Mat src(dims, isz, CV_8U, srcptr, srcstep);
    Mat dst(dims, isz, CV_8U, dstptr, dststep);

    ثابت Mat* arrays[] = { &src, &dst };
    uchar* ptrs[2];
    NAryMatIterator it(arrays, ptrs, 2);
    size_t planesz = it.size;

    עבור( size_t j = 0; j < it.nplanes; j++, ++it )
        memcpy(ptrs[1], ptrs[0], planesz);
}


فراغ MatAllocator::upload(UMatData* u, ثابت فراغ* srcptr, صحيح dims, ثابت size_t sz[],
                    ثابت size_t dstofs[], ثابت size_t dststep[],
                    ثابت size_t srcstep[]) ثابت
{
    אם(!u)
        החזר;
    صحيح isz[CV_MAX_DIM];
    uchar* dstptr = u->data;
    עבור( صحيح i = 0; i < dims; i++ )
    {
        CV_Assert( sz[i] <= (size_t)INT_MAX );
        אם( sz[i] == 0 )
            החזר;
        אם( dstofs )
            dstptr += dstofs[i]*(i <= dims-2 ? dststep[i] : 1);
        isz[i] = (صحيح)sz[i];
    }

    Mat src(dims, isz, CV_8U, (فراغ*)srcptr, srcstep);
    Mat dst(dims, isz, CV_8U, dstptr, dststep);

    ثابت Mat* arrays[] = { &src, &dst };
    uchar* ptrs[2];
    NAryMatIterator it(arrays, ptrs, 2);
    size_t planesz = it.size;

    עבור( size_t j = 0; j < it.nplanes; j++, ++it )
        memcpy(ptrs[1], ptrs[0], planesz);
}

فراغ MatAllocator::copy(UMatData* usrc, UMatData* udst, صحيح dims, ثابت size_t sz[],
                  ثابت size_t srcofs[], ثابت size_t srcstep[],
                  ثابت size_t dstofs[], ثابت size_t dststep[], منطقي /*sync*/) ثابت
{
    CV_INSTRUMENT_REGION();

    אם(!usrc || !udst)
        החזר;
    صحيح isz[CV_MAX_DIM];
    uchar* srcptr = usrc->data;
    uchar* dstptr = udst->data;
    עבור( صحيح i = 0; i < dims; i++ )
    {
        CV_Assert( sz[i] <= (size_t)INT_MAX );
        אם( sz[i] == 0 )
            החזר;
        אם( srcofs )
            srcptr += srcofs[i]*(i <= dims-2 ? srcstep[i] : 1);
        אם( dstofs )
            dstptr += dstofs[i]*(i <= dims-2 ? dststep[i] : 1);
        isz[i] = (صحيح)sz[i];
    }

    Mat src(dims, isz, CV_8U, srcptr, srcstep);
    Mat dst(dims, isz, CV_8U, dstptr, dststep);

    ثابت Mat* arrays[] = { &src, &dst };
    uchar* ptrs[2];
    NAryMatIterator it(arrays, ptrs, 2);
    size_t planesz = it.size;

    עבור( size_t j = 0; j < it.nplanes; j++, ++it )
        memcpy(ptrs[1], ptrs[0], planesz);
}

BufferPoolController* MatAllocator::getBufferPoolController(ثابت حرف* id) ثابت
{
    CV_UNUSED(id);
    ساكن DummyBufferPoolController dummy;
    החזר &dummy;
}

מחלקה StdMatAllocator CV_FINAL : عام MatAllocator
{
عام:
    UMatData* allocate(صحيح dims, ثابت صحيح* sizes, صحيح type,
                       فراغ* data0, size_t* step, AccessFlag /*flags*/, UMatUsageFlags /*usageFlags*/) ثابت CV_OVERRIDE
    {
        size_t total = CV_ELEM_SIZE(type);
        עבור( صحيح i = dims-1; i >= 0; i-- )
        {
            אם( step )
            {
                אם( data0 && step[i] != CV_AUTOSTEP )
                {
                    CV_Assert(total <= step[i]);
                    total = step[i];
                }
                אחרת
                    step[i] = total;
            }
            total *= sizes[i];
        }
        uchar* data = data0 ? (uchar*)data0 : (uchar*)fastMalloc(total);
        UMatData* u = جديد UMatData(هذا);
        u->data = u->origdata = data;
        u->size = total;
        אם(data0)
            u->flags |= UMatData::USER_ALLOCATED;

        החזר u;
    }

    منطقي allocate(UMatData* u, AccessFlag /*accessFlags*/, UMatUsageFlags /*usageFlags*/) ثابت CV_OVERRIDE
    {
        אם(!u) החזר שקר;
        החזר אמת;
    }

    فراغ deallocate(UMatData* u) ثابت CV_OVERRIDE
    {
        אם(!u)
            החזר;

        CV_Assert(u->urefcount == 0);
        CV_Assert(u->refcount == 0);
        אם( !(u->flags & UMatData::USER_ALLOCATED) )
        {
            fastFree(u->origdata);
            u->origdata = 0;
        }
        delete u;
    }
};

ساكن
MatAllocator*& getDefaultAllocatorMatRef()
{
    ساكن MatAllocator* g_matAllocator = Mat::getStdAllocator();
    החזר g_matAllocator;
}

MatAllocator* Mat::getDefaultAllocator()
{
    החזר getDefaultAllocatorMatRef();
}

فراغ Mat::setDefaultAllocator(MatAllocator* allocator)
{
    getDefaultAllocatorMatRef() = allocator;
}

MatAllocator* Mat::getStdAllocator()
{
    CV_SINGLETON_LAZY_INIT(MatAllocator, جديد StdMatAllocator())
}

//==================================================================================================

فراغ setSize( Mat& m, صحيح _dims, ثابت صحيح* _sz, ثابت size_t* _steps, منطقي autoSteps)
{
    CV_Assert( 0 <= _dims && _dims <= CV_MAX_DIM && _dims <= MatShape::MAX_DIMS);

    m.dims = _dims;
    m.size = MatShape(_dims, _sz);
    m.step[std::max(_dims-1, 0)] = CV_ELEM_SIZE(m.flags);
    עבור (صحيح i = _dims-2; i >= 0; i--) {
        size_t autostep = m.size[i+1]*m.step[i+1];
        אם (_steps) {
            m.step[i] = _steps[i];
            //CV_Assert(m.step[i] >= autostep);
        } אחרת אם (autoSteps) {
            m.step[i] = autostep;
        } אחרת {
            m.step[i] = 0;
        }
    }

    אם( _dims <= 2 )
    {
        m.cols = _dims == 0 ? 1 : _sz ? _sz[_dims > 1] : 0;
        m.rows = _dims < 2 ? 1 : _sz ? _sz[0] : 0;
    } אחרת {
        m.cols = m.rows = -1;
    }
}

صحيح updateContinuityFlag(صحيح flags, صحيح dims, ثابت صحيح* size, ثابت size_t* step)
{
    صحيح i, j;
    אם (dims <= 1)
        החזר flags | Mat::CONTINUOUS_FLAG;
    עבור( i = 0; i < dims; i++ )
    {
        אם( size[i] > 1 )
            اكسر;
    }

    uint64 t = (uint64)size[std::min(i, dims-1)]*CV_MAT_CN(flags);
    עבור( j = dims-1; j > i; j-- )
    {
        t *= size[j];
        אם( step[j]*size[j] < step[j-1] )
            اكسر;
    }

    אם( j <= i && t == (uint64)(صحيح)t )
        החזר flags | Mat::CONTINUOUS_FLAG;
    החזר flags & ~Mat::CONTINUOUS_FLAG;
}

فراغ Mat::updateContinuityFlag()
{
    flags = cv::updateContinuityFlag(flags, dims, size.p, step.p);
}

فراغ finalizeHdr(Mat& m)
{
    m.updateContinuityFlag();
    صحيح d = m.dims;
    אם( d > 2 )
        m.rows = m.cols = -1;
    אם(m.u)
        m.datastart = m.data = m.u->data;
    אם( m.data && d > 0 )
    {
        m.datalimit = m.datastart + m.size[0]*m.step[0];
        אם( m.size[0] > 0 )
        {
            صحيح lastdim = d > 0 ? d - 1 : 0;
            m.dataend = m.ptr() + m.size[lastdim]*m.step[lastdim];
            עבור( صحيح i = 0; i < d-1; i++ )
                m.dataend += (m.size[i] - 1)*m.step[i];
        }
        אחרת
            m.dataend = m.datalimit;
    }
    אחרת
        m.dataend = m.datalimit = m.data;
}

//======================================= Mat ======================================================

Mat::Mat() CV_NOEXCEPT
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{}

Mat::Mat(صحيح _rows, صحيح _cols, صحيح _type)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_rows, _cols, _type);
}

Mat::Mat(صحيح _rows, صحيح _cols, صحيح _type, ثابت Scalar& _s)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_rows, _cols, _type);
    *هذا = _s;
}

Mat::Mat(Size _sz, صحيح _type)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create( _sz.height, _sz.width, _type );
}

Mat::Mat(Size _sz, صحيح _type, ثابت Scalar& _s)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_sz.height, _sz.width, _type);
    *هذا = _s;
}

Mat::Mat(صحيح _dims, ثابت صحيح* _sz, صحيح _type)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_dims, _sz, _type);
}

Mat::Mat(صحيح _dims, ثابت صحيح* _sz, صحيح _type, ثابت Scalar& _s)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_dims, _sz, _type);
    *هذا = _s;
}

Mat::Mat(ثابت std::vector<صحيح>& _sz, صحيح _type)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_sz, _type);
}

Mat::Mat(ثابت std::vector<صحيح>& _sz, صحيح _type, ثابت Scalar& _s)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_sz, _type);
    *هذا = _s;
}

Mat::Mat(ثابت MatShape& _shape, صحيح _type)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_shape, _type);
}

Mat::Mat(std::initializer_list<صحيح> _shape, صحيح _type)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_shape, _type);
}

Mat::Mat(ثابت MatShape& _shape, صحيح _type, ثابت Scalar& _s)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_shape, _type);
    *هذا = _s;
}

Mat::Mat(std::initializer_list<صحيح> _shape, صحيح _type, ثابت Scalar& _s)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_shape, _type);
    *هذا = _s;
}

Mat::Mat(ثابت Mat& m)
    : flags(m.flags), dims(m.dims), rows(m.rows), cols(m.cols), data(m.data),
      datastart(m.datastart), dataend(m.dataend), datalimit(m.datalimit), allocator(m.allocator),
      u(m.u), size(m.size), step(m.step)
{
    אם( u )
        CV_XADD(&u->refcount, 1);
}

Mat::Mat(صحيح _rows, صحيح _cols, صحيح _type, فراغ* _data, size_t _step)
    : flags(MAGIC_VAL + (_type & TYPE_MASK)), dims(2), rows(_rows), cols(_cols),
      data((uchar*)_data), datastart((uchar*)_data), dataend(0), datalimit(0),
      allocator(0), u(0), size(2)
{
    CV_Assert(total() == 0 || data != عدم);

    size_t esz = CV_ELEM_SIZE(_type), esz1 = CV_ELEM_SIZE1(_type);
    size_t minstep = cols * esz;
    אם( _step == AUTO_STEP )
    {
        _step = minstep;
    }
    אחרת
    {
        CV_Assert( _step >= minstep );
        אם (_step % esz1 != 0)
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

Mat::Mat(Size _sz, صحيح _type, فراغ* _data, size_t _step)
    : flags(MAGIC_VAL + (_type & TYPE_MASK)), dims(2), rows(_sz.height), cols(_sz.width),
      data((uchar*)_data), datastart((uchar*)_data), dataend(0), datalimit(0),
      allocator(0), u(0), size(2)
{
    CV_Assert(total() == 0 || data != عدم);

    size_t esz = CV_ELEM_SIZE(_type), esz1 = CV_ELEM_SIZE1(_type);
    size_t minstep = cols*esz;
    אם( _step == AUTO_STEP || (_step < minstep && rows == 1))
    {
        _step = minstep;
    }
    אחרת
    {
        CV_CheckGE(_step, minstep, "");

        אם (_step % esz1 != 0)
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

Mat& Mat::operator = (ثابت Mat& m)
{
    אם( هذا != &m )
    {
        אם( m.u )
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
    החזר *هذا;
}

Mat Mat::clone() ثابت
{
    Mat m;
    copyTo(m);
    החזר m;
}

فراغ Mat::assignTo( Mat& m, صحيح _type ) ثابت
{
    אם( _type < 0 )
        m = *هذا;
    אחרת
        convertTo(m, _type);
}

فراغ Mat::create(صحيح _rows, صحيح _cols, صحيح _type)
{
    _type &= TYPE_MASK;
    אם( dims <= 2 && rows == _rows && cols == _cols && type() == _type && data )
        החזר;
    صحيح sz[] = {_rows, _cols};
    create(2, sz, _type);
}

فراغ Mat::create(Size _sz, صحيح _type)
{
    create(_sz.height, _sz.width, _type);
}

فراغ Mat::createSameSize(InputArray m, صحيح type)
{
    MatShape msize = m.shape();
    create(msize, type);
}

فراغ Mat::fit(صحيح _dims, ثابت صحيح* _sizes, صحيح _type)
{
    size_t oldTotalBytes = u ? u->size : 0;
    size_t esz = CV_ELEM_SIZE(_type), newTotal = _dims >= 0;
    עבור (صحيح i = 0; i < _dims; i++)
        newTotal *= _sizes[i];
    size_t newTotalBytes = newTotal*esz;
    אם (newTotalBytes > 0 && (!isContinuous() ||
                              newTotalBytes > oldTotalBytes ||
                              data != datastart)) {
        create(_dims, _sizes, _type);
    } אחרת {
        flags = (flags & ~Mat::TYPE_MASK) | CV_MAT_TYPE(_type);
        صحيح _dummy_size = 0;
        setSize(*هذا, (_dims >= 0 ? _dims : 1), (_dims >= 0 ? _sizes : &_dummy_size), nullptr, אמת);
        finalizeHdr(*هذا);
    }
}

فراغ Mat::fit(ثابت std::vector<صحيح>& _shape, صحيح _type)
{
    fit((صحيح)_shape.size(), _shape.data(), _type);
}

فراغ Mat::fit(ثابت MatShape& _shape, صحيح _type)
{
    fit(_shape.dims, _shape.p, _type);
    size.layout = _shape.layout;
    size.C = _shape.C;
}

فراغ Mat::fit(std::initializer_list<صحيح> _shape, صحيح _type)
{
    صحيح new_shape[MatShape::MAX_DIMS];
    صحيح new_ndims = (صحيح)_shape.size();
    CV_Assert(new_ndims <= MatShape::MAX_DIMS);
    auto it = _shape.begin();
    עבור (صحيح i = 0; i < new_ndims; i++, ++it)
        new_shape[i] = *it;
    fit(new_ndims, new_shape, _type);
}

فراغ Mat::fit(صحيح _rows, صحيح _cols, صحيح _type)
{
    _type &= TYPE_MASK;
    صحيح sz[] = {_rows, _cols};
    fit(2, sz, _type);
}

فراغ Mat::fit(Size _sz, صحيح _type)
{
    fit(_sz.height, _sz.width, _type);
}

فراغ Mat::fitSameSize(InputArray m, صحيح _type)
{
    fit(m.shape(), _type);
}

فراغ Mat::addref()
{
    אם( u )
        CV_XADD(&u->refcount, 1);
}

فراغ Mat::release()
{
    אם( u && CV_XADD(&u->refcount, -1) == 1 )
        deallocate();
    u = عدم;
    datastart = dataend = datalimit = data = 0;
    עבור(صحيح i = 0; i < dims; i++)
        size.p[i] = 0;
    flags = (flags & CV_MAT_TYPE_MASK) | MAGIC_VAL;
    dims = rows = cols = 0;
    size.clear();
}

size_t Mat::step1(صحيح i) ثابت
{
    החזר step.p[i] / elemSize1();
}

منطقي Mat::empty() ثابت
{
    החזר data == 0 || total() == 0;
}

size_t Mat::total() ثابت
{
    אם( dims <= 2 )
        החזר (size_t)rows * cols;
    size_t p = 1;
    עבור( صحيح i = 0; i < dims; i++ )
        p *= size[i];
    החזר p;
}

size_t Mat::total(صحيح startDim, صحيح endDim) ثابت
{
    CV_Assert( 0 <= startDim && startDim <= endDim);
    size_t p = 1;
    صحيح endDim_ = endDim <= dims ? endDim : dims;
    עבור( صحيح i = startDim; i < endDim_; i++ )
        p *= size[i];
    החזר p;
}

Mat::Mat(Mat&& m) CV_NOEXCEPT
    : flags(m.flags), dims(m.dims), rows(m.rows), cols(m.cols), data(m.data),
      datastart(m.datastart), dataend(m.dataend), datalimit(m.datalimit), allocator(m.allocator),
      u(m.u), size(m.size), step(m.step)
{
    m.flags = MAGIC_VAL; m.dims = m.rows = m.cols = 0;
    m.size.clear(); m.step.clear();
    m.data = عدم; m.datastart = عدم; m.dataend = عدم; m.datalimit = عدم;
    m.allocator = عدم;
    m.u = عدم;
}


Mat& Mat::operator = (Mat&& m)
{
    אם (هذا == &m)
      החזר *هذا;

    release();
    flags = m.flags; dims = m.dims; rows = m.rows; cols = m.cols; data = m.data;
    size = m.size; step = m.step;
    datastart = m.datastart; dataend = m.dataend; datalimit = m.datalimit; allocator = m.allocator;
    u = m.u;
    m.flags = MAGIC_VAL; m.dims = m.rows = m.cols = 0;
    m.size.clear(); m.step.clear();
    m.data = عدم; m.datastart = عدم; m.dataend = عدم; m.datalimit = عدم;
    m.allocator = عدم;
    m.u = عدم;
    החזר *هذا;
}


فراغ Mat::create(صحيح d0, ثابت صحيح* _sizes, صحيح _type)
{
    صحيح sz1 = 1, d = d0;
    صحيح i;
    אם (d == 0) {
        d = 1;
        _sizes = (ثابت صحيح*)&sz1;
    }
    CV_Assert(0 <= d && d <= CV_MAX_DIM && _sizes);
    _type = CV_MAT_TYPE(_type);

    אם( data && d == dims && _type == type() )
    {
        עבור( i = 0; i < d; i++ )
            אם( size[i] != _sizes[i] )
                اكسر;
        אם( i == d )
            החזר;
    }

    صحيح _sizes_backup[CV_MAX_DIM]; // #5991
    אם (_sizes == (هذا->size.p))
    {
        עבור(i = 0; i < d; i++ )
            _sizes_backup[i] = _sizes[i];
        _sizes = _sizes_backup;
    }

    release();
    אם( d == 0 )
        החזר;
    flags = (_type & CV_MAT_TYPE_MASK) | MAGIC_VAL;
    setSize(*هذا, d, _sizes, 0, אמת);

    אם( total() > 0 )
    {
        MatAllocator *a = allocator, *a0 = getDefaultAllocator();
        אם(!a)
            a = a0;
        حاول
        {
            u = a->allocate(dims, size.p, _type, 0, step.p, ACCESS_RW /* ignored */, USAGE_DEFAULT);
            CV_Assert(u != 0);
            allocator = a;
        }
        امسك (...)
        {
            אם (a == a0)
                ارم;
            u = a0->allocate(dims, size.p, _type, 0, step.p, ACCESS_RW /* ignored */, USAGE_DEFAULT);
            CV_Assert(u != 0);
            allocator = a0;
        }
        CV_Assert( step[dims-1] == (size_t)CV_ELEM_SIZE(flags) );
    }

    addref();
    finalizeHdr(*هذا);
    size.dims = dims = d0;
}

فراغ Mat::create(ثابت std::vector<صحيح>& _sizes, صحيح _type)
{
    create((صحيح)_sizes.size(), _sizes.data(), _type);
}

فراغ Mat::create(ثابت MatShape& _shape, صحيح _type)
{
    אם (_shape.dims < 0) {
        release();
        החזר;
    }
    create(_shape.dims, _shape.p, _type);
    size.layout = _shape.layout;
    size.C = _shape.C;
}

فراغ Mat::create(std::initializer_list<صحيح> _shape, صحيح _type)
{
    صحيح new_shape[MatShape::MAX_DIMS];
    صحيح new_ndims = (صحيح)_shape.size();
    CV_Assert(new_ndims <= MatShape::MAX_DIMS);
    auto it = _shape.begin();
    עבור (صحيح i = 0; i < new_ndims; i++, ++it)
        new_shape[i] = *it;

    create(new_ndims, new_shape, _type);
}

فراغ Mat::copySize(ثابت Mat& m)
{
    dims = m.dims;
    cols = m.cols;
    rows = m.rows;
    size = m.size;
    step = m.step;
}

فراغ Mat::deallocate()
{
    אם(u)
    {
        UMatData* u_ = u;
        u = عدم;
        (u_->currAllocator ? u_->currAllocator : allocator ? allocator : getDefaultAllocator())->unmap(u_);
    }
}

Mat::Mat(ثابت Mat& m, ثابت Range& _rowRange, ثابت Range& _colRange)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    CV_Assert(m.dims <= MatShape::MAX_DIMS);
    CV_Assert(m.dims >= 2 ||
              (m.dims == 1 && (_rowRange == Range::all() ||
                               _rowRange == Range(0, m.rows))));
    אם( m.dims > 2 )
    {
        Range rs[MatShape::MAX_DIMS];
        rs[0] = _rowRange;
        rs[1] = _colRange;
        עבור( صحيح i = 2; i < m.dims; i++ )
            rs[i] = Range::all();
        *هذا = m(rs);
        החזר;
    }

    *هذا = m;
    حاول
    {
        אם( _rowRange != Range::all() && _rowRange != Range(0,rows) )
        {
            CV_Assert( 0 <= _rowRange.start && _rowRange.start <= _rowRange.end
                       && _rowRange.end <= m.rows );
            size[0] = rows = _rowRange.size();
            data += step*_rowRange.start;
            flags |= SUBMATRIX_FLAG;
        }

        אם( _colRange != Range::all() && _colRange != Range(0,cols) )
        {
            CV_Assert( 0 <= _colRange.start && _colRange.start <= _colRange.end
                       && _colRange.end <= m.cols );
            size[dims > 1] = cols = _colRange.size();
            data += _colRange.start*elemSize();
            flags |= SUBMATRIX_FLAG;
        }
    }
    امسك(...)
    {
        release();
        ارم;
    }

    updateContinuityFlag();

    אם( rows <= 0 || cols <= 0 )
    {
        release();
        rows = cols = 0;
    }
}


Mat::Mat(ثابت Mat& m, ثابت Rect& roi)
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
    אם( roi.width < m.cols || roi.height < m.rows )
        flags |= SUBMATRIX_FLAG;

    size[0] = rows;
    size[1] = cols;
    step[0] = m.step[0];
    step[1] = esz;
    updateContinuityFlag();

    addref();
    אם( rows <= 0 || cols <= 0 )
    {
        rows = cols = 0;
        release();
    }
}


Mat::Mat(صحيح _dims, ثابت صحيح* _sizes, صحيح _type, فراغ* _data, ثابت size_t* _steps)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    flags |= CV_MAT_TYPE(_type);
    datastart = data = (uchar*)_data;
    אם (_dims > 0 || _data != nullptr)
        setSize(*هذا, _dims, _sizes, _steps, אמת);
    finalizeHdr(*هذا);
}


Mat::Mat(ثابت std::vector<صحيح>& _sizes, صحيح _type, فراغ* _data, ثابت size_t* _steps)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    flags |= CV_MAT_TYPE(_type);
    datastart = data = (uchar*)_data;
    صحيح _dims = (صحيح)_sizes.size();
    אם (_dims > 0 || _data != nullptr)
        setSize(*هذا, _dims, _sizes.data(), _steps, אמת);
    finalizeHdr(*هذا);
}

Mat::Mat(ثابت MatShape& _shape, صحيح _type, فراغ* _data, ثابت size_t* _steps)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    flags |= CV_MAT_TYPE(_type);
    datastart = data = (uchar*)_data;
    אם (_shape.dims >= 0) {
        setSize(*هذا, _shape.dims, _shape.p, _steps, אמת);
    }
    אחרת {
        CV_Assert(!data);
    }
    finalizeHdr(*هذا);
}

Mat::Mat(std::initializer_list<صحيح> _shape, صحيح _type, فراغ* _data, ثابت size_t* _steps)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    صحيح new_shape[MatShape::MAX_DIMS];
    صحيح _dims = (صحيح)_shape.size();
    CV_Assert(_dims <= MatShape::MAX_DIMS);
    auto it = _shape.begin();
    עבור (صحيح i = 0; i < _dims; i++, ++it)
        new_shape[i] = *it;

    flags |= CV_MAT_TYPE(_type);
    datastart = data = (uchar*)_data;
    אם (_dims > 0 || _data != nullptr)
        setSize(*هذا, _dims, new_shape, _steps, אמת);
    finalizeHdr(*هذا);
}

Mat::Mat(ثابت Mat& m, ثابت Range* ranges)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    صحيح d = m.dims;

    CV_Assert(ranges);
    עבור( صحيح i = 0; i < d; i++ )
    {
        Range r = ranges[i];
        CV_Assert( r == Range::all() || (0 <= r.start && r.start < r.end && r.end <= m.size[i]) );
    }
    *هذا = m;
    עבור( صحيح i = 0; i < d; i++ )
    {
        Range r = ranges[i];
        אם( r != Range::all() && r != Range(0, size.p[i]))
        {
            size.p[i] = r.end - r.start;
            data += r.start*step.p[i];
            flags |= SUBMATRIX_FLAG;
        }
    }

    אם (d <= 2) {
        rows = d == 2 ? size[0] : 1;
        cols = d <= 0 ? (d >= 0) : size[d > 1];
    }
    updateContinuityFlag();
}

Mat::Mat(ثابت Mat& m, ثابت std::vector<Range>& ranges)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
    datalimit(0), allocator(0), u(0)
{
    صحيح d = m.dims;

    CV_Assert((صحيح)ranges.size() == d);
    עבור (صحيح i = 0; i < d; i++)
    {
        Range r = ranges[i];
        CV_Assert(r == Range::all() || (0 <= r.start && r.start < r.end && r.end <= m.size[i]));
    }
    *هذا = m;
    עבור (صحيح i = 0; i < d; i++)
    {
        Range r = ranges[i];
        אם (r != Range::all() && r != Range(0, size.p[i]))
        {
            size.p[i] = r.end - r.start;
            data += r.start*step.p[i];
            flags |= SUBMATRIX_FLAG;
        }
    }

    אם (d <= 2) {
        rows = d == 2 ? size[0] : 1;
        cols = d <= 0 ? (d >= 0) : size[d > 1];
    }
    updateContinuityFlag();
}


Mat Mat::diag(صحيح d) ثابت
{
    CV_Assert( dims == 2 );
    Mat m = *هذا;
    size_t esz = elemSize();
    صحيح len;

    אם( d >= 0 )
    {
        len = std::min(cols - d, rows);
        m.data += esz*d;
    }
    אחרת
    {
        len = std::min(rows + d, cols);
        m.data -= step[0]*d;
    }
    CV_DbgAssert( len > 0 );

    m.size[0] = m.rows = len;
    m.size[1] = m.cols = 1;
    m.step[0] += (len > 1 ? esz : 0);

    m.updateContinuityFlag();

    אם( size() != Size(1,1) )
        m.flags |= SUBMATRIX_FLAG;

    החזר m;
}


فراغ Mat::pop_back(size_t nelems)
{
    CV_Assert( nelems <= (size_t)size.p[0] );
    CV_Assert( dims >= 1 );

    אם( isSubmatrix() )
        *هذا = rowRange(0, size.p[0] - (صحيح)nelems);
    אחרת
    {
        size.p[0] -= (صحيح)nelems;
        אם (dims == 2)
            rows = size.p[0];
        אחרת אם (dims == 1)
            cols = size.p[0];
        dataend -= nelems*step.p[0];
    }
}


فراغ Mat::push_back_(ثابت فراغ* elem)
{
    size_t r = size.p[0];
    אם( isSubmatrix() || dataend + step.p[0] > datalimit )
        reserve( std::max(r + 1, (r*3+1)/2) );

    size_t esz = elemSize();
    memcpy(data + r*step.p[0], elem, esz);
    size.p[0] = صحيح(r + 1);
    אם (dims == 2)
        rows = size.p[0];
    אחרת אם (dims == 1)
        cols = size.p[0];
    dataend += step.p[0];
    uint64 tsz = size.p[0];
    עבור( صحيح i = 1; i < dims; i++ )
        tsz *= size.p[i];
    אם( esz < step.p[0] || tsz != (uint64)(صحيح)tsz )
        flags &= ~CONTINUOUS_FLAG;
}


فراغ Mat::reserve(size_t nelems)
{
    ثابت size_t MIN_SIZE = 64;

    CV_Assert( (صحيح)nelems >= 0 );
    אם( !isSubmatrix() && step.p[0] != 0 && data + step.p[0]*nelems <= datalimit )
        החזר;

    صحيح r = size.p[0];
    אם( (size_t)r >= nelems)
        החזר;

    MatShape newsize = size;
    size_t esz = elemSize();
    newsize.dims = std::max(newsize.dims, 1);
    אם (dims > 1 && r == 0 && step.p[0] == 0) {
        step.p[0] = size.p[1] * (dims == 2 ? esz : step.p[1]);
    }
    newsize.p[0] = (صحيح)nelems;
    size_t newbytes = newsize.total() * esz;
    אם (newbytes < MIN_SIZE) {
        newsize.p[0] = 1;
        newsize.p[0] = صحيح(MIN_SIZE / newsize.total() / esz);
    }

    Mat m(newsize, type());
    אם( r > 0)
    {
        Mat mpart = m.rowRange(0, r);
        copyTo(mpart);
    }

    *هذا = m;
    size.p[0] = r;

    אם (dims == 2)
        rows = size.p[0];
    אחרת אם (dims == 1)
        cols = size.p[0];
    dataend = data + step.p[0]*r;
}


فراغ Mat::reserveBuffer(size_t nbytes)
{
    size_t esz = 1;
    صحيح mtype = CV_8UC1;
    אם (!empty())
    {
        אם (!isSubmatrix() && data + nbytes <= dataend)//Should it be datalimit?
            החזר;
        esz = elemSize();
        mtype = type();
    }

    size_t nelems = (nbytes - 1) / esz + 1;

#אם SIZE_MAX > UINT_MAX
    CV_Assert(nelems <= size_t(INT_MAX)*size_t(INT_MAX));
    صحيح newrows = nelems > size_t(INT_MAX) ? nelems > 0x400*size_t(INT_MAX) ? nelems > 0x100000 * size_t(INT_MAX) ? nelems > 0x40000000 * size_t(INT_MAX) ?
                  size_t(INT_MAX) : 0x40000000 : 0x100000 : 0x400 : 1;
#אחרת
    صحيح newrows = nelems > size_t(INT_MAX) ? 2 : 1;
#endif
    صحيح newcols = (صحيح)((nelems - 1) / newrows + 1);

    create(newrows, newcols, mtype);
}


فراغ Mat::resize(size_t nelems)
{
    صحيح r = size.p[0];
    אם( r == (صحيح)nelems )
        החזר;
    CV_Assert( (صحيح)nelems >= 0 );

    אם( isSubmatrix() || data + step.p[0]*nelems > datalimit )
        reserve(nelems);

    size.p[0] = (صحيح)nelems;
    אם (dims == 2)
        rows = size.p[0];
    אחרת אם (dims == 1)
        cols = size.p[0];
    dataend += (size.p[0] - r)*(int64_t)step.p[0];

    //updateContinuityFlag(*this);
}


فراغ Mat::resize(size_t nelems, ثابت Scalar& s)
{
    صحيح saveRows = size.p[0];
    resize(nelems);

    אם( size.p[0] > saveRows )
    {
        Mat part = rowRange(saveRows, size.p[0]);
        part = s;
    }
}

فراغ Mat::push_back(ثابت Mat& elems)
{
    size_t r = size.p[0];
    size_t delta = elems.size.p[0];
    אם( delta == 0 )
        החזר;
    אם( هذا == &elems )
    {
        Mat tmp = elems;
        push_back(tmp);
        החזר;
    }
    אם( !data )
    {
        *هذا = elems.clone();
        החזר;
    }
    CV_Assert(dims > 0);

    size.p[0] = elems.size.p[0];
    منطقي eq = size == elems.size;
    size.p[0] = صحيح(r);
    אם( !eq )
        CV_Error(cv::Error::StsUnmatchedSizes, "Pushed vector length is not equal to matrix row length");
    אם( type() != elems.type() )
        CV_Error(cv::Error::StsUnmatchedFormats, "Pushed vector type is not the same as matrix type");
    size_t esz = elemSize();
    size_t minstep = dims <= 1 ? esz : size.p[1] * (dims == 2 ? esz : step.p[1]);
    size_t step0 = step.p[0];
    אם (step0 < minstep) {
        אם (size.p[0] <= 1)
            step.p[0] = step0 = minstep;
    }

    אם( isSubmatrix() || dataend + step0*delta > datalimit )
        reserve( std::max(r + delta, (r*3+1)/2) );

    size.p[0] += صحيح(delta);
    אם (dims == 2)
        rows = size.p[0];
    אחרת אם (dims == 1)
        cols = size.p[0];
    dataend += step0*delta;

    //updateContinuityFlag(*this);

    אם( isContinuous() && elems.isContinuous() )
        memcpy(data + r*step0, elems.data, elems.total()*elems.elemSize());
    אחרת
    {
        Mat part = rowRange(صحيح(r), صحيح(r + delta));
        elems.copyTo(part);
    }
}


فراغ Mat::locateROI( Size& wholeSize, Point& ofs ) ثابت
{
    CV_Assert( dims <= 2 && step[0] > 0 );
    size_t esz = elemSize(), minstep;
    ptrdiff_t delta1 = data - datastart, delta2 = dataend - datastart;

    אם( delta1 == 0 )
        ofs.x = ofs.y = 0;
    אחרת
    {
        ofs.y = (صحيح)(delta1/step[0]);
        ofs.x = (صحيح)((delta1 - step[0]*ofs.y)/esz);
        CV_DbgAssert( data == datastart + ofs.y*step[0] + ofs.x*esz );
    }
    minstep = (ofs.x + cols)*esz;
    wholeSize.height = (صحيح)((delta2 - minstep)/step[0] + 1);
    wholeSize.height = std::max(wholeSize.height, ofs.y + rows);
    wholeSize.width = (صحيح)((delta2 - step*(wholeSize.height-1))/esz);
    wholeSize.width = std::max(wholeSize.width, ofs.x + cols);
}

Mat& Mat::adjustROI( صحيح dtop, صحيح dbottom, صحيح dleft, صحيح dright )
{
    CV_Assert( dims <= 2 && step[0] > 0 );
    Size wholeSize; Point ofs;
    size_t esz = elemSize();
    locateROI( wholeSize, ofs );
    صحيح row1 = std::min(std::max(ofs.y - dtop, 0), wholeSize.height), row2 = std::max(0, std::min(ofs.y + rows + dbottom, wholeSize.height));
    صحيح col1 = std::min(std::max(ofs.x - dleft, 0), wholeSize.width), col2 = std::max(0, std::min(ofs.x + cols + dright, wholeSize.width));
    אם(row1 > row2)
        std::swap(row1, row2);
    אם(col1 > col2)
        std::swap(col1, col2);

    אם (dims == 1) {
        data += (col1 - ofs.x)*(std::ptrdiff_t)esz;
        cols = col2 - col1;
        size.p[0] = cols;
    } אחרת {
        data += (row1 - ofs.y)*(std::ptrdiff_t)step + (col1 - ofs.x)*(std::ptrdiff_t)esz;
        rows = row2 - row1; cols = col2 - col1;
        size.p[0] = rows; size.p[1] = cols;
        updateContinuityFlag();
    }
    החזר *هذا;
}

Mat Mat::reshape(صحيح new_cn, صحيح new_rows) ثابت
{
    صحيح cn = channels();
    Mat hdr = *هذا;

    אם( new_cn == 0 )
        new_cn = cn;

    אם( dims > 2 )
    {
        אם( new_rows == 0 )
        {
            // special case: just change the number of channnels; retain the same shape,
            // except for the last, innermost dimension
            CV_Assert(size[dims-1]*cn % new_cn == 0);
            hdr.flags = (hdr.flags & ~CV_MAT_CN_MASK) | ((new_cn-1) << CV_CN_SHIFT);
            hdr.step[dims-1] = CV_ELEM_SIZE(hdr.flags);
            hdr.size[dims-1] = hdr.size[dims-1]*cn / new_cn;
            החזר hdr;
        }
        CV_Assert( new_rows > 0 );
        صحيح sz[] = { new_rows, (صحيح)(total()*cn/new_rows) };
        החזר reshape(new_cn, 2, sz);
    }

    صحيح total_width = cols * cn;

    אם( (new_cn > total_width || total_width % new_cn != 0) && new_rows == 0 )
        new_rows = rows * total_width / new_cn;

    אם( new_rows != 0 && new_rows != rows )
    {
        صحيح total_size = total_width * rows;
        אם( !isContinuous() )
            CV_Error( cv::Error::BadStep,
            "The matrix is not continuous, thus its number of rows can not be changed" );

        total_width = total_size / new_rows;

        אם( total_width * new_rows != total_size )
            CV_Error( cv::Error::StsBadArg, "The total number of matrix elements "
                                    "is not divisible by the new number of rows" );

        hdr.size[0] = hdr.rows = new_rows;
        hdr.step[0] = total_width * elemSize1();
    } אחרת {
        hdr.size[0] = hdr.rows = rows;
        אם (dims <= 1)
            hdr.step[0] = cols * CV_ELEM_SIZE(flags);
    }

    صحيح new_width = total_width / new_cn;

    אם( new_width * new_cn != total_width )
        CV_Error( cv::Error::BadNumChannels,
        "The total width is not divisible by the new number of channels" );

    hdr.size.dims = hdr.dims = 2;
    hdr.size[1] = hdr.cols = new_width;
    hdr.flags = (hdr.flags & ~CV_MAT_CN_MASK) | ((new_cn-1) << CV_CN_SHIFT);
    hdr.step[1] = CV_ELEM_SIZE(hdr.flags);
    החזר hdr;
}

Mat Mat::reshape(صحيح _cn, صحيح _newndims, ثابت صحيح* _newsz) ثابت
{
    אם(_newndims == dims)
    {
        אם(_newsz == 0)
            החזר reshape(_cn);
        אם(_newndims == 2)
            החזר reshape(_cn, _newsz[0]);
    }

    אם (isContinuous())
    {
        CV_Assert(_cn >= 0 && _newndims >= 0 && _newndims <= CV_MAX_DIM && (_newndims == 0 || _newsz != 0));

        אם (_cn == 0)
            _cn = هذا->channels();
        אחרת
            CV_Assert(_cn <= CV_CN_MAX);

        size_t total_elem1_ref = هذا->total() * هذا->channels();
        size_t total_elem1 = _cn;

        AutoBuffer<صحيح, 4> newsz_buf( (size_t)_newndims );

        صحيح m1_idx = -1;

        עבור (صحيح i = 0; i < _newndims; i++)
        {
            אם (_newsz[i] >= 0) {
                אם (_newsz[i] == 0 && i < dims)
                    newsz_buf[i] = size.p[i];
                אחרת
                    newsz_buf[i] = _newsz[i];
                total_elem1 *= (size_t)newsz_buf[i];
            } אחרת {
                אם (m1_idx >= 0)
                    CV_Error(cv::Error::StsBadSize, "More than one '-1' occured in the new shape");
                m1_idx = i;
            }
        }

        אם (m1_idx >= 0) {
            אם (total_elem1 == 0) {
                CV_Assert(total_elem1_ref == 0);
                total_elem1 = 1;
            }
            CV_Assert(total_elem1_ref % total_elem1 == 0);
            newsz_buf[m1_idx] = (صحيح)(total_elem1_ref / total_elem1);
        } אחרת אם (total_elem1 != total_elem1_ref) {
            CV_Error(cv::Error::StsUnmatchedSizes, "Requested and source matrices have different count of elements");
        }

        Mat hdr = *هذا;
        hdr.flags = (hdr.flags & ~CV_MAT_CN_MASK) | ((_cn-1) << CV_CN_SHIFT);
        setSize(hdr, _newndims, newsz_buf.data(), عدم, אמת);

        החזר hdr;
    }

    CV_Error(cv::Error::StsNotImplemented, "Reshaping of n-dimensional non-continuous matrices is not supported yet");
    // TBD
}

Mat Mat::reshape(صحيح _cn, ثابت std::vector<صحيح>& _newshape) ثابت
{
    صحيح newdims = (صحيح)_newshape.size();
    אם(newdims == 0 && empty())
    {
        החזר *هذا;
    }

    החזר reshape(_cn, newdims, newdims > 0 ? &_newshape[0] : 0);
}

Mat Mat::reshape(صحيح _cn, ثابت MatShape& _newshape) ثابت
{
    אם (_newshape.dims < 0) {
        صحيح newshape[] = {0};
        החזר reshape(_cn, 1, newshape);
    }
    החזר reshape(_cn, _newshape.dims, _newshape.p);
}

Mat Mat::reshape(صحيح _cn, std::initializer_list<صحيح> newshape_) ثابت
{
    صحيح newshape[MatShape::MAX_DIMS];
    size_t i, newshape_dims = newshape_.size();
    CV_Assert(newshape_dims <= (size_t)MatShape::MAX_DIMS);
    auto it = newshape_.begin();
    עבור (i = 0; i < newshape_dims; i++, ++it)
        newshape[i] = *it;
    החזר reshape(_cn, (صحيح)newshape_dims, newshape);
}

Mat Mat::reinterpret(صحيح type) ثابت
{
    type = CV_MAT_TYPE(type);
    CV_Assert(CV_ELEM_SIZE(هذا->type()) == CV_ELEM_SIZE(type));
    Mat m = *هذا;
    m.flags = (m.flags & ~CV_MAT_TYPE_MASK) | type;
    m.updateContinuityFlag();
    החזר m;
}

Mat Mat::diag(ثابت Mat& d)
{
    CV_Assert( d.cols == 1 || d.rows == 1 );
    صحيح len = d.rows + d.cols - 1;
    Mat m(len, len, d.type(), Scalar(0));
    Mat md = m.diag();
    אם( d.cols == 1 )
        d.copyTo(md);
    אחרת
        transpose(d, md);
    החזר m;
}

صحيح Mat::checkVector(صحيح _elemChannels, صحيح _depth, منطقي _requireContinuous) ثابت
{
    החזר data && (depth() == _depth || _depth <= 0) &&
        (isContinuous() || !_requireContinuous) &&
        ((dims <= 2 && (((rows == 1 || cols == 1) && channels() == _elemChannels) ||
                        (cols == _elemChannels && channels() == 1))) ||
        (dims == 3 && channels() == 1 && size.p[2] == _elemChannels && (size.p[0] == 1 || size.p[1] == 1) &&
         (isContinuous() || step.p[1] == step.p[2]*size.p[2])))
    ? (صحيح)(total()*channels()/_elemChannels) : -1;
}


ساكن مضمن Size getContinuousSize_(صحيح flags, صحيح cols, صحيح rows, صحيح widthScale)
{
    int64 sz = (int64)cols * rows * widthScale;
    منطقي has_int_overflow = sz >= INT_MAX;
    منطقي isContiguous = (flags & Mat::CONTINUOUS_FLAG) != 0;
    החזר (isContiguous && !has_int_overflow)
            ? Size((صحيح)sz, 1)
            : Size(cols * widthScale, rows);
}

Size getContinuousSize2D(Mat& m1, صحيح widthScale)
{
    CV_CheckLE(m1.dims, 2, "");
    החזר getContinuousSize_(m1.flags,
                              m1.cols, m1.rows, widthScale);
}
Size getContinuousSize2D(Mat& m1, Mat& m2, صحيح widthScale)
{
    CV_CheckLE(m1.dims, 2, "");
    CV_CheckLE(m2.dims, 2, "");
    ثابت Size sz1 = m1.size();
    אם (sz1 != m2.size())  // reshape all matrixes to the same size (#4159)
    {
        size_t total_sz = m1.total();
        CV_CheckEQ(total_sz, m2.total(), "");
        منطقي is_m1_vector = m1.cols == 1 || m1.rows == 1;
        منطقي is_m2_vector = m2.cols == 1 || m2.rows == 1;
        CV_Assert(is_m1_vector); CV_Assert(is_m2_vector);
        صحيح total = (صحيح)total_sz;  // vector-column
        منطقي isContiguous = ((m1.flags & m2.flags) & Mat::CONTINUOUS_FLAG) != 0;
        منطقي has_int_overflow = ((int64)total_sz * widthScale) >= INT_MAX;
        אם (isContiguous && !has_int_overflow)
            total = 1; // vector-row
        m1 = m1.reshape(0, total);
        m2 = m2.reshape(0, total);
        CV_Assert(m1.cols == m2.cols && m1.rows == m2.rows);
        החזר Size(m1.cols * widthScale, m1.rows);
    }
    החזר getContinuousSize_(m1.flags & m2.flags,
                              m1.cols, m1.rows, widthScale);
}

Size getContinuousSize2D(Mat& m1, Mat& m2, Mat& m3, صحيح widthScale)
{
    CV_CheckLE(m1.dims, 2, "");
    CV_CheckLE(m2.dims, 2, "");
    CV_CheckLE(m3.dims, 2, "");
    ثابت Size sz1 = m1.size();
    אם (sz1 != m2.size() || sz1 != m3.size())  // reshape all matrixes to the same size (#4159)
    {
        size_t total_sz = m1.total();
        CV_CheckEQ(total_sz, m2.total(), "");
        CV_CheckEQ(total_sz, m3.total(), "");
        منطقي is_m1_vector = m1.cols == 1 || m1.rows == 1;
        منطقي is_m2_vector = m2.cols == 1 || m2.rows == 1;
        منطقي is_m3_vector = m3.cols == 1 || m3.rows == 1;
        CV_Assert(is_m1_vector); CV_Assert(is_m2_vector); CV_Assert(is_m3_vector);
        صحيح total = (صحيح)total_sz;  // vector-column
        منطقي isContiguous = ((m1.flags & m2.flags & m3.flags) & Mat::CONTINUOUS_FLAG) != 0;
        منطقي has_int_overflow = ((int64)total_sz * widthScale) >= INT_MAX;
        אם (isContiguous && !has_int_overflow)
            total = 1; // vector-row
        m1 = m1.reshape(0, total);
        m2 = m2.reshape(0, total);
        m3 = m3.reshape(0, total);
        CV_Assert(m1.cols == m2.cols && m1.rows == m2.rows && m1.cols == m3.cols && m1.rows == m3.rows);
        החזר Size(m1.cols * widthScale, m1.rows);
    }
    החזר getContinuousSize_(m1.flags & m2.flags & m3.flags,
                              m1.cols, m1.rows, widthScale);
}

} // cv::
