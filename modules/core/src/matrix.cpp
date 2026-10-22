// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#ضمن "precomp.hpp"
#ضمن "bufferpool.impl.hpp"

نطاق cv {

std::string layout_مقصد_String(داده_Layout layout)
{
    أرجع
        layout == داده_LAYOUT_ND ? "ND" :
        layout == داده_LAYOUT_NCHW ? "NCHW" :
        layout == داده_LAYOUT_NHWC ? "NHWC" :
        layout == داده_LAYOUT_بلوک ? "BLOCK" :
        layout == داده_LAYOUT_NCDHW ? "NCDHW" :
        layout == داده_LAYOUT_NDHWC ? "NDHWC" :
        layout == داده_LAYOUT_PLANAR ? "PLANAR" :
        layout == داده_LAYOUT_UNKNOWN ? "Unknown" : "???";
}

منطقي عملگر == (ثابت MatShape& size1, ثابت MatShape& size2)
{
    إذا (size1.dims != size2.dims)
        أرجع خطأ;
    إذا (size1.layout != size2.layout &&
        size1.layout != داده_LAYOUT_UNKNOWN &&
        size2.layout != داده_LAYOUT_UNKNOWN)
        أرجع خطأ;
    إذا (size1.layout == داده_LAYOUT_بلوک &&
        size2.layout == داده_LAYOUT_بلوک &&
        size1.C != size2.C)
        أرجع خطأ;
    لكل (صحيح i = 0; i < size1.dims; i++) {
        إذا (size1.p[i] != size2.p[i])
            أرجع خطأ;
    }
    أرجع درست;
}

منطقي عملگر != (ثابت MatShape& size1, ثابت MatShape& size2)
{
    أرجع !(size1 == size2);
}

/////////////////////////// MatShape ////////////////////////////////

MatShape MatShape::scalar()
{
    أرجع MatShape(0);
}

فراغ MatShape::clear()
{
    dims = -1;
    layout = داده_LAYOUT_UNKNOWN;
    C = 0;
    لكل (صحيح i = 0; i < MAX_DIMS; i++)
        p[i] = 0;
}

فراغ MatShape::resize(size_t new_اندازه, صحيح value)
{
    CV_Assert(new_اندازه < (size_t)MAX_DIMS);
    صحيح old_dims = std::max(dims, 0);
    dims = (صحيح)new_اندازه;
    لكل (صحيح i = old_dims; i < dims; i++)
        p[i] = value;
}

فراغ MatShape::reserve(size_t)
{
    // no op; maybe need to add a check for overflow, but we check it anyway in other operations
}

فراغ MatShape::assign(size_t new_اندازه, صحيح value)
{
    CV_Assert(new_اندازه < (size_t)MAX_DIMS);
    dims = (صحيح)new_اندازه;
    لكل (صحيح i = 0; i < dims; i++)
        p[i] = value;
}

فراغ MatShape::assign(صحيح new_اندازه, صحيح value)
{
    assign((size_t)new_اندازه, value);
}

فراغ MatShape::assign(ثابت صحيح* begin, ثابت صحيح* end)
{
    تخصیص_بده(begin, end);
}

فراغ MatShape::تخصیص_بده(ثابت صحيح* begin, ثابت صحيح* end)
{
    ptrdiff_t new_اندازه = end - begin;
    CV_Assert(0 <= new_اندازه && new_اندازه < (ptrdiff_t)MAX_DIMS);
    dims = (صحيح)new_اندازه;
    لكل (صحيح i = 0; i < dims; i++)
        p[i] = begin[i];
}

صحيح* MatShape::begin() { أرجع p; }
ثابت صحيح* MatShape::begin() ثابت { أرجع p; }
صحيح* MatShape::end() { أرجع p + std::max(dims, 0); }
ثابت صحيح* MatShape::end() ثابت { أرجع p + std::max(dims, 0); }
صحيح& MatShape::back() { أرجع p[std::max(dims-1, 0)]; }
ثابت صحيح& MatShape::back() ثابت { أرجع p[std::max(dims-1, 0)]; }

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
    لكل (صحيح i = old_dims-1; i >= (صحيح)ofs; i--)
        p[i+1] = p[i];
    p[ofs] = value;
}

فراغ MatShape::insert(صحيح* where, size_t شمار_2, صحيح value)
{
    صحيح old_dims = std::max(dims, 0);
    CV_Assert((size_t)(old_dims+شمار_2) < (size_t)MAX_DIMS);
    ptrdiff_t ofs = where - p;
    CV_Assert(0 <= ofs && ofs <= old_dims);
    dims = (صحيح)(old_dims+شمار_2);
    لكل (صحيح i = old_dims-1; i >= (صحيح)ofs; i--)
        p[i+شمار_2] = p[i];
    لكل (صحيح i = 0; i < (صحيح)شمار_2; i++)
        p[i+ofs] = value;
}

فراغ MatShape::insert(صحيح* where, صحيح شمار_2, صحيح value)
{
    insert(where, (size_t)شمار_2, value);
}

فراغ MatShape::insert(صحيح* where, ثابت صحيح* begin, ثابت صحيح* end)
{
    درج(where, begin, end);
}

فراغ MatShape::درج(صحيح* where, ثابت صحيح* begin, ثابت صحيح* end)
{
    صحيح old_dims = std::max(dims, 0);
    ptrdiff_t delta = end - begin;
    CV_Assert(0 <= delta && old_dims+delta < MAX_DIMS);
    ptrdiff_t ofs = where - p;
    CV_Assert(0 <= ofs && ofs <= old_dims);
    dims = (صحيح)(old_dims+delta);
    لكل (صحيح i = old_dims-1; i >= (صحيح)ofs; i--)
        p[i+delta] = p[i];
    لكل (صحيح i = 0; i < (صحيح)delta; i++)
        p[i+ofs] = begin[i];
}

فراغ MatShape::erase(صحيح* where)
{
    CV_Assert(dims > 0);
    ptrdiff_t ofs = where - p;
    CV_Assert(0 <= ofs && ofs <= dims);
    إذا (ofs == dims)
        أرجع;
    dims--;
    لكل (صحيح i = (صحيح)ofs+1; i <= dims; i++)
        p[i-1] = p[i];
}

size_t MatShape::مجموع() ثابت
{
    size_t نتیجه = 1;
    إذا (dims < 0)
        أرجع 0;
    لكل (صحيح i = 0; i < dims; i++)
        نتیجه *= p[i];
    أرجع نتیجه;
}

std::vector<صحيح> MatShape::vec() ثابت
{
    إذا (dims < 0)
        أرجع std::vector<صحيح>(1, 0);
    أرجع std::vector<صحيح>(p, p + dims);
}

std::string MatShape::str() ثابت
{
    std::stringstream sstrm;
    إذا (empty()) {
        sstrm << "<empty>";
    } وإلا إذا (dims == 0) {
        sstrm << "<scalar>";
    } وإلا {
        sstrm << "[";
        لكل (صحيح i = 0; i < dims; i++) {
            sstrm << (i > 0 ? " x " : "") << p[i];
        }
        sstrm << "]";
    }
    أرجع sstrm.str();
}

ساكن فراغ finalize_بلوک_Layout(MatShape& size, صحيح C=0)
{
    إذا (size.layout == داده_LAYOUT_بلوک) {
        CV_Assert(size.dims >= 4);
        صحيح C0 = size.p[size.dims-1];
        CV_Assert(C0 > 1 && (C0 & (C0-1)) == 0);
        size.C = C > 0 ? C : size.p[1]*size.p[size.dims-1];
    } وإلا {
        size.C = 0;
    }
    لكل (صحيح i = std::max(size.dims, 0); i < MatShape::MAX_DIMS; i++)
        size.p[i] = 0;
    إذا (size.dims == 0)
        size.p[0] = 1;
}

MatShape::MatShape()
{
    clear();
}

MatShape::MatShape(size_t dims_, ثابت صحيح* اندازه_3, داده_Layout layout_, صحيح C_)
{
    layout = layout_;
    CV_Assert(dims_ <= (size_t)MAX_DIMS);
    dims = (صحيح)dims_;
    لكل (صحيح i = 0; i < dims; i++) {
        p[i] = اندازه_3 ? اندازه_3[i] : 0;
    }
    finalize_بلوک_Layout(*هذا, C_);
}

MatShape::MatShape(size_t dims_, صحيح value, داده_Layout layout_)
{
    layout = layout_;
    CV_Assert(dims_ <= (size_t)MAX_DIMS);
    dims = (صحيح)dims_;
    لكل (صحيح i = 0; i < dims; i++) {
        p[i] = value;
    }
    finalize_بلوک_Layout(*هذا, 0);
}

MatShape::MatShape(std::initializer_list<صحيح> shape)
{
    layout = داده_LAYOUT_UNKNOWN;
    CV_Assert(shape.size() <= (size_t)MAX_DIMS);
    dims = (صحيح)shape.size();
    خودکار it = shape.begin();
    لكل (صحيح i = 0; i < dims; i++, ++it) {
        p[i] = *it;
    }
    finalize_بلوک_Layout(*هذا, 0);
}

MatShape::MatShape(صحيح dims_, صحيح value, داده_Layout layout_)
{
    layout = layout_;
    CV_Assert(dims_ <= MAX_DIMS);
    dims = dims_;
    لكل (صحيح i = 0; i < dims; i++) {
        p[i] = value;
    }
    finalize_بلوک_Layout(*هذا, 0);
}

MatShape::MatShape(ثابت std::vector<صحيح>& shape_, داده_Layout layout_, صحيح C_)
{
    layout = layout_;
    size_t shape_اندازه = shape_.size();
    CV_Assert(shape_اندازه < (size_t)MAX_DIMS);
    dims = (صحيح)shape_اندازه;
    لكل (صحيح i = 0; i < dims; i++) {
        p[i] = shape_[i];
    }
    finalize_بلوک_Layout(*هذا, C_);
}

MatShape::MatShape(ثابت صحيح* begin, ثابت صحيح* end, داده_Layout layout_, صحيح C_)
{
    layout = layout_;
    ptrdiff_t shape_اندازه = end - begin;
    CV_Assert(0 <= shape_اندازه && shape_اندازه < MAX_DIMS);
    dims = (صحيح)shape_اندازه;
    لكل (صحيح i = 0; i < dims; i++) {
        p[i] = begin[i];
    }
    finalize_بلوک_Layout(*هذا, C_);
}

MatShape::MatShape(ثابت MatShape& shape)
{
    dims = shape.dims;
    layout = shape.layout;
    C = shape.C;
    لكل (صحيح i = 0; i < MAX_DIMS; i++)
        p[i] = shape.p[i];
}

MatShape& MatShape::عملگر = (ثابت MatShape& shape)
{
    إذا (هذا != &shape) {
        dims = shape.dims;
        layout = shape.layout;
        C = shape.C;
        لكل (صحيح i = 0; i < MAX_DIMS; i++)
            p[i] = shape.p[i];
    }
    أرجع *هذا;
}

منطقي MatShape::hasSymbols() ثابت
{
    لكل (صحيح i = 0; i < dims; i++) {
        إذا (p[i] < 0)
            أرجع درست;
    }
    أرجع خطأ;
}

صحيح MatShape::channels() ثابت
{
    CV_Assert(layout == داده_LAYOUT_بلوک || layout == داده_LAYOUT_NCHW || layout == داده_LAYOUT_NHWC);
    أرجع layout == داده_LAYOUT_بلوک ? C : p[layout == داده_LAYOUT_NCHW ? 1 : dims-1];
}

MatShape MatShape::مقصد_Layout(داده_Layout newLayout, صحيح C0) ثابت
{
    CV_Assert(layout == داده_LAYOUT_بلوک || layout == داده_LAYOUT_NCHW || layout == داده_LAYOUT_NHWC);
    CV_Assert(newLayout == داده_LAYOUT_بلوک || newLayout == داده_LAYOUT_NCHW || newLayout == داده_LAYOUT_NHWC);

    MatShape newsize = *هذا;
    newsize.layout = newLayout;

    إذا (newLayout == داده_LAYOUT_بلوک) {
        // any => BLOCK
        CV_Assert_N(C0 > 1, (C0 & (C0-1)) == 0);
        صحيح Corig = channels();
        newsize.C = Corig;

        إذا (layout == داده_LAYOUT_NHWC) {
            لكل (صحيح i = 2; i < dims; i++) {
                newsize.p[i] = p[i-1];
            }
        }

        newsize.dims += layout != داده_LAYOUT_بلوک;
        newsize.p[1] = (Corig + C0 - 1)/C0;
        newsize.p[newsize.dims-1] = C0;
    } وإلا إذا (layout == داده_LAYOUT_بلوک) {
        // BLOCK => any (except for BLOCK, which is handled above)
        newsize.C = 0;
        إذا (newLayout == داده_LAYOUT_NHWC) {
            لكل (صحيح i = 2; i < dims; i++) {
                newsize.p[i-1] = p[i];
            }
        }
        newsize.p[newLayout == داده_LAYOUT_NCHW ? 1 : dims-2] = C;
        newsize.dims--;
    } وإلا {
        CV_Assert_N(C0 <= 1);

        إذا (newLayout == layout)
            أرجع newsize;

        // NHWC => NCHW
        إذا (newLayout == داده_LAYOUT_NCHW) {
            لكل (صحيح i = 2; i < dims; i++)
                newsize.p[i] = p[i-1];
            newsize.p[1] = p[dims-1];
        } وإلا {
            // NCHW => NHWC
            لكل (صحيح i = 2; i < dims; i++)
                newsize.p[i-1] = p[i];
            newsize.p[dims-1] = p[1];
        }
    }

    أرجع newsize;
}

ساكن صحيح normalizeShapeAxis(صحيح axis, صحيح dims)
{
    CV_CheckGE(axis, -dims, "axis is out of range");
    CV_CheckLT(axis, dims, "axis is out of range");
    أرجع axis < 0 ? axis + dims : axis;
}

// The semantic NCHW form {N, C, spatial...} of a block ('NC1HWC0') shape, so that axes and
// element counts are the logical ones rather than the C0-padded ones.
ساكن MatShape مقصد_Semantic(ثابت MatShape& shape)
{
    CV_CheckGE(shape.dims, 3, "a block shape is {N, C1, spatial..., C0}");
    أرجع shape.مقصد_Layout(داده_LAYOUT_NCHW);
}

// Blocks such a result again. Once N or C changes, elements cross the padded C0 lanes and
// the data needs a repack, which a shape operation cannot express.
ساكن MatShape مقصد_بلوک_از(MatShape نتیجه, ثابت MatShape& src, ثابت حرف* op)
{
    إذا (نتیجه.dims < 2 || نتیجه.p[0] != src.p[0] || نتیجه.p[1] != src.C)
        CV_Error_(Error::StsNotImplemented,
                  ("%s: the block layout is kept only when the batch and the channel axes stay "
                   "unchanged; convert the shape with toLayout() first", op));
    CV_CheckLE(نتیجه.dims + 1, (صحيح)MatShape::MAX_DIMS, "too many dimensions");
    نتیجه.layout = داده_LAYOUT_NCHW;
    أرجع نتیجه.مقصد_Layout(داده_LAYOUT_بلوک, src.p[src.dims-1]);
}

MatShape MatShape::squeeze(ثابت std::vector<صحيح>& axes) ثابت
{
    CV_Assert(dims >= 0);
    إذا (layout == داده_LAYOUT_بلوک)
        أرجع مقصد_بلوک_از(مقصد_Semantic(*هذا).squeeze(axes), *هذا, "squeeze");
    منطقي remove[MAX_DIMS] = {خطأ};
    إذا (axes.empty())
    {
        لكل (صحيح i = 0; i < dims; i++)
            remove[i] = p[i] == 1;
    }
    لكل (صحيح a : axes)
    {
        صحيح i = normalizeShapeAxis(a, dims);
        CV_Check(a, !remove[i], "squeeze: duplicate axis");
        CV_Check(p[i], p[i] == 1, "squeeze: the size of a squeezed axis must be 1");
        remove[i] = درست;
    }
    // a new number of axes: the layout of this shape does not apply
    MatShape نتیجه;
    نتیجه.dims = 0;
    لكل (صحيح i = 0; i < dims; i++)
        إذا (!remove[i])
            نتیجه.p[نتیجه.dims++] = p[i];
    أرجع نتیجه;
}

MatShape MatShape::unsqueeze(ثابت std::vector<صحيح>& axes) ثابت
{
    CV_Assert(dims >= 0);
    إذا (layout == داده_LAYOUT_بلوک)
        أرجع مقصد_بلوک_از(مقصد_Semantic(*هذا).unsqueeze(axes), *هذا, "unsqueeze");
    ثابت صحيح بیرون_Dims = dims + (صحيح)axes.size();
    CV_CheckLE(بیرون_Dims, (صحيح)MAX_DIMS, "unsqueeze: too many dimensions");
    منطقي insert[MAX_DIMS] = {خطأ};
    لكل (صحيح a : axes)
    {
        صحيح i = normalizeShapeAxis(a, بیرون_Dims);
        CV_Check(a, !insert[i], "unsqueeze: duplicate axis");
        insert[i] = درست;
    }
    // a new number of axes: the layout of this shape does not apply
    MatShape نتیجه;
    نتیجه.dims = بیرون_Dims;
    لكل (صحيح i = 0, j = 0; i < بیرون_Dims; i++)
        نتیجه.p[i] = insert[i] ? 1 : p[j++];
    أرجع نتیجه;
}

MatShape MatShape::flatten(صحيح شروع_Axis, صحيح انتها_Axis) ثابت
{
    CV_Assert(dims >= 0);
    إذا (layout == داده_LAYOUT_بلوک)
        أرجع مقصد_بلوک_از(مقصد_Semantic(*هذا).flatten(شروع_Axis, انتها_Axis), *هذا, "flatten");
    إذا (dims == 0)
    {
        CV_Check(شروع_Axis, شروع_Axis == 0 || شروع_Axis == -1, "flatten: axis is out of range");
        CV_Check(انتها_Axis, انتها_Axis == 0 || انتها_Axis == -1, "flatten: axis is out of range");
        أرجع MatShape({1});
    }
    صحيح شروع = normalizeShapeAxis(شروع_Axis, dims), end = normalizeShapeAxis(انتها_Axis, dims);
    CV_CheckLE(شروع, end, "flatten: startAxis must not follow endAxis");
    int64 merged = 1;
    لكل (صحيح i = شروع; i <= end; i++)
        merged *= p[i];
    CV_CheckLE((مزدوج)merged, (مزدوج)بیشترین_صحیح, "flatten: the result is too big");
    // a new number of axes: the layout of this shape does not apply
    MatShape نتیجه;
    نتیجه.dims = 0;
    لكل (صحيح i = 0; i < dims; i++)
    {
        إذا (i == شروع)
            نتیجه.p[نتیجه.dims++] = (صحيح)merged;
        وإلا إذا (i < شروع || i > end)
            نتیجه.p[نتیجه.dims++] = p[i];
    }
    أرجع نتیجه;
}

MatShape MatShape::reshape(ثابت MatShape& newShape, منطقي allowZero) ثابت
{
    CV_Assert(dims >= 0 && newShape.dims >= 0);
    إذا (layout == داده_LAYOUT_بلوک)
        أرجع مقصد_بلوک_از(مقصد_Semantic(*هذا).reshape(newShape, allowZero), *هذا, "reshape");
    // a new number of axes: neither the layout of this shape nor the one of the spec applies
    MatShape نتیجه;
    نتیجه.dims = newShape.dims;
    std::copy(newShape.p, newShape.p + newShape.dims, نتیجه.p);
    صحيح inferred = -1;
    int64 معلوم = 1;
    لكل (صحيح i = 0; i < نتیجه.dims; i++)
    {
        صحيح& sz = نتیجه.p[i];
        إذا (sz == -1)
        {
            CV_Check(i, inferred < 0, "reshape: at most one axis may be -1");
            inferred = i;
            استمر;
        }
        CV_Check(sz, sz >= 0, "reshape: sizes must be -1 or non-negative");
        إذا (sz == 0 && !allowZero)
        {
            CV_Check(i, i < dims, "reshape: 0 refers to an axis the source does not have");
            sz = p[i];
        }
        معلوم *= sz;
    }
    ثابت int64 مجموع_2 = (int64)مجموع();
    إذا (inferred >= 0)
    {
        CV_Check((مزدوج)معلوم, معلوم != 0, "reshape: cannot infer the -1 axis next to a zero-size axis");
        CV_Check((مزدوج)مجموع_2, مجموع_2 % معلوم == 0, "reshape: the number of elements does not match");
        CV_CheckLE((مزدوج)(مجموع_2/معلوم), (مزدوج)بیشترین_صحیح, "reshape: the result is too big");
        نتیجه.p[inferred] = (صحيح)(مجموع_2/معلوم);
    }
    وإلا إذا (معلوم != مجموع_2)
        CV_Error_(Error::StsBadArg, ("reshape: the number of elements does not match: %lld vs %lld",
                                     (دراز دراز)معلوم, (دراز دراز)مجموع_2));
    أرجع نتیجه;
}

MatShape MatShape::expand(ثابت MatShape& another) ثابت
{
    إذا (dims == 0)
        أرجع another;
    إذا (another.dims == 0)
        أرجع *هذا;

    إذا ((layout == داده_LAYOUT_NCHW || layout == داده_LAYOUT_NHWC) &&
        (another.layout == داده_LAYOUT_NCHW || another.layout == داده_LAYOUT_NHWC)) {
        CV_Assert(layout == another.layout);
    }
    // [TODO] support block layout
    CV_Assert(layout != داده_LAYOUT_بلوک && another.layout != داده_LAYOUT_بلوک);

    MatShape نتیجه;

    إذا (dims < 0 || another.dims < 0)
        أرجع نتیجه;

    نتیجه = *هذا;
    نتیجه.dims = std::max(dims, another.dims);
    نتیجه.layout = layout == داده_LAYOUT_UNKNOWN ? another.layout :
        layout == داده_LAYOUT_ND && (another.layout == داده_LAYOUT_NCHW ||
        another.layout == داده_LAYOUT_NHWC) ? another.layout : layout;
    لكل (صحيح i = نتیجه.dims-1; i >= 0; i--) {
        صحيح i1 = i - (نتیجه.dims - dims);
        صحيح i2 = i - (نتیجه.dims - another.dims);
        صحيح sz1 = i1 < 0 ? 1 : p[i1];
        صحيح sz2 = i2 < 0 ? 1 : another.p[i2];
        CV_Assert(sz1 == sz2 || sz1 == 1 || sz2 == 1);
        // [TODO] handle symbolic shapes
        نتیجه.p[i] = sz1 != 1 ? sz1 : sz2;
    }
    أرجع نتیجه;
}

/////////////////////////// MatAllocator ////////////////////////////

فراغ MatAllocator::map(UMat_داده*, AccessFlag) ثابت
{
}

فراغ MatAllocator::unmap(UMat_داده* u) ثابت
{
    إذا(u->urefcount == 0 && u->refcount == 0)
    {
        deallocate(u);
    }
}

فراغ MatAllocator::download(UMat_داده* u, فراغ* dstptr,
         صحيح dims, ثابت size_t sz[],
         ثابت size_t srcofs[], ثابت size_t srcstep[],
         ثابت size_t dststep[]) ثابت
{
    إذا(!u)
        أرجع;
    صحيح isz[CV_MAX_DIM];
    uchar* srcptr = u->داده;
    لكل( صحيح i = 0; i < dims; i++ )
    {
        CV_Assert( sz[i] <= (size_t)بیشترین_صحیح );
        إذا( sz[i] == 0 )
            أرجع;
        إذا( srcofs )
            srcptr += srcofs[i]*(i <= dims-2 ? srcstep[i] : 1);
        isz[i] = (صحيح)sz[i];
    }

    Mat src(dims, isz, CV_8U, srcptr, srcstep);
    Mat dst(dims, isz, CV_8U, dstptr, dststep);

    ثابت Mat* arrays[] = { &src, &dst };
    uchar* ptrs[2];
    NAryMatIterator it(arrays, ptrs, 2);
    size_t planesz = it.size;

    لكل( size_t j = 0; j < it.nplanes; j++, ++it )
        کپی_حافظه(ptrs[1], ptrs[0], planesz);
}


فراغ MatAllocator::upload(UMat_داده* u, ثابت فراغ* srcptr, صحيح dims, ثابت size_t sz[],
                    ثابت size_t dstofs[], ثابت size_t dststep[],
                    ثابت size_t srcstep[]) ثابت
{
    إذا(!u)
        أرجع;
    صحيح isz[CV_MAX_DIM];
    uchar* dstptr = u->داده;
    لكل( صحيح i = 0; i < dims; i++ )
    {
        CV_Assert( sz[i] <= (size_t)بیشترین_صحیح );
        إذا( sz[i] == 0 )
            أرجع;
        إذا( dstofs )
            dstptr += dstofs[i]*(i <= dims-2 ? dststep[i] : 1);
        isz[i] = (صحيح)sz[i];
    }

    Mat src(dims, isz, CV_8U, (فراغ*)srcptr, srcstep);
    Mat dst(dims, isz, CV_8U, dstptr, dststep);

    ثابت Mat* arrays[] = { &src, &dst };
    uchar* ptrs[2];
    NAryMatIterator it(arrays, ptrs, 2);
    size_t planesz = it.size;

    لكل( size_t j = 0; j < it.nplanes; j++, ++it )
        کپی_حافظه(ptrs[1], ptrs[0], planesz);
}

فراغ MatAllocator::copy(UMat_داده* usrc, UMat_داده* udst, صحيح dims, ثابت size_t sz[],
                  ثابت size_t srcofs[], ثابت size_t srcstep[],
                  ثابت size_t dstofs[], ثابت size_t dststep[], منطقي /*sync*/) ثابت
{
    CV_INSTRUMENT_REGION();

    إذا(!usrc || !udst)
        أرجع;
    صحيح isz[CV_MAX_DIM];
    uchar* srcptr = usrc->داده;
    uchar* dstptr = udst->داده;
    لكل( صحيح i = 0; i < dims; i++ )
    {
        CV_Assert( sz[i] <= (size_t)بیشترین_صحیح );
        إذا( sz[i] == 0 )
            أرجع;
        إذا( srcofs )
            srcptr += srcofs[i]*(i <= dims-2 ? srcstep[i] : 1);
        إذا( dstofs )
            dstptr += dstofs[i]*(i <= dims-2 ? dststep[i] : 1);
        isz[i] = (صحيح)sz[i];
    }

    Mat src(dims, isz, CV_8U, srcptr, srcstep);
    Mat dst(dims, isz, CV_8U, dstptr, dststep);

    ثابت Mat* arrays[] = { &src, &dst };
    uchar* ptrs[2];
    NAryMatIterator it(arrays, ptrs, 2);
    size_t planesz = it.size;

    لكل( size_t j = 0; j < it.nplanes; j++, ++it )
        کپی_حافظه(ptrs[1], ptrs[0], planesz);
}

میانگیر_Pool_Controller* MatAllocator::get_میانگیر_Pool_Controller(ثابت حرف* id) ثابت
{
    CV_استفادهنشده(id);
    ساكن Dummy_میانگیر_Pool_Controller dummy;
    أرجع &dummy;
}

صنف StdMatAllocator CV_FINAL : عام MatAllocator
{
عام:
    UMat_داده* allocate(صحيح dims, ثابت صحيح* sizes, صحيح type,
                       فراغ* data0, size_t* گام, AccessFlag /*flags*/, UMatUsageFlags /*usageFlags*/) ثابت CV_OVERRIDE
    {
        size_t مجموع = CV_ELEM_اندازه(type);
        لكل( صحيح i = dims-1; i >= 0; i-- )
        {
            إذا( گام )
            {
                إذا( data0 && گام[i] != CV_AUTOSTEP )
                {
                    CV_Assert(مجموع <= گام[i]);
                    مجموع = گام[i];
                }
                وإلا
                    گام[i] = مجموع;
            }
            مجموع *= sizes[i];
        }
        uchar* داده = data0 ? (uchar*)data0 : (uchar*)fastMalloc(مجموع);
        UMat_داده* u = جديد UMat_داده(هذا);
        u->داده = u->origdata = داده;
        u->size = مجموع;
        إذا(data0)
            u->flags |= UMat_داده::USER_ALLOCATED;

        أرجع u;
    }

    منطقي allocate(UMat_داده* u, AccessFlag /*accessFlags*/, UMatUsageFlags /*usageFlags*/) ثابت CV_OVERRIDE
    {
        إذا(!u) أرجع خطأ;
        أرجع درست;
    }

    فراغ deallocate(UMat_داده* u) ثابت CV_OVERRIDE
    {
        إذا(!u)
            أرجع;

        CV_Assert(u->urefcount == 0);
        CV_Assert(u->refcount == 0);
        إذا( !(u->flags & UMat_داده::USER_ALLOCATED) )
        {
            fastFree(u->origdata);
            u->origdata = 0;
        }
        حذف u;
    }
};

ساكن
MatAllocator*& getDefaultAllocatorMatRef()
{
    ساكن MatAllocator* g_matAllocator = Mat::getStdAllocator();
    أرجع g_matAllocator;
}

MatAllocator* Mat::getDefaultAllocator()
{
    أرجع getDefaultAllocatorMatRef();
}

فراغ Mat::مجموعه_Default_Allocator(MatAllocator* allocator)
{
    getDefaultAllocatorMatRef() = allocator;
}

MatAllocator* Mat::getStdAllocator()
{
    CV_SINGLETON_LAZY_INIT(MatAllocator, جديد StdMatAllocator())
}

//==================================================================================================

فراغ مجموعه_اندازه( Mat& m, صحيح _dims, ثابت صحيح* _sz, ثابت size_t* گامها_2, منطقي auto_گامها)
{
    CV_Assert( 0 <= _dims && _dims <= CV_MAX_DIM && _dims <= MatShape::MAX_DIMS);

    m.dims = _dims;
    m.size = MatShape(_dims, _sz);
    m.گام[std::max(_dims-1, 0)] = CV_ELEM_اندازه(m.flags);
    لكل (صحيح i = _dims-2; i >= 0; i--) {
        size_t autostep = m.size[i+1]*m.گام[i+1];
        إذا (گامها_2) {
            m.گام[i] = گامها_2[i];
            //CV_Assert(m.step[i] >= autostep);
        } وإلا إذا (auto_گامها) {
            m.گام[i] = autostep;
        } وإلا {
            m.گام[i] = 0;
        }
    }

    إذا( _dims <= 2 )
    {
        m.تعداد_ستون = _dims == 0 ? 1 : _sz ? _sz[_dims > 1] : 0;
        m.ردیفها = _dims < 2 ? 1 : _sz ? _sz[0] : 0;
    } وإلا {
        m.تعداد_ستون = m.ردیفها = -1;
    }
}

صحيح updateContinuityFlag(صحيح flags, صحيح dims, ثابت صحيح* size, ثابت size_t* گام)
{
    صحيح i, j;
    إذا (dims <= 1)
        أرجع flags | Mat::CONTINUOUS_FLAG;
    لكل( i = 0; i < dims; i++ )
    {
        إذا( size[i] > 1 )
            اكسر;
    }

    uint64 t = (uint64)size[std::min(i, dims-1)]*CV_MAT_CN(flags);
    لكل( j = dims-1; j > i; j-- )
    {
        t *= size[j];
        إذا( گام[j]*size[j] < گام[j-1] )
            اكسر;
    }

    إذا( j <= i && t == (uint64)(صحيح)t )
        أرجع flags | Mat::CONTINUOUS_FLAG;
    أرجع flags & ~Mat::CONTINUOUS_FLAG;
}

فراغ Mat::updateContinuityFlag()
{
    flags = cv::updateContinuityFlag(flags, dims, size.p, گام.p);
}

فراغ finalizeHdr(Mat& m)
{
    m.updateContinuityFlag();
    صحيح d = m.dims;
    إذا( d > 2 )
        m.ردیفها = m.تعداد_ستون = -1;
    إذا(m.u)
        m.datastart = m.داده = m.u->داده;
    إذا( m.داده && d > 0 )
    {
        m.datalimit = m.datastart + m.size[0]*m.گام[0];
        إذا( m.size[0] > 0 )
        {
            صحيح lastdim = d > 0 ? d - 1 : 0;
            m.dataend = m.ptr() + m.size[lastdim]*m.گام[lastdim];
            لكل( صحيح i = 0; i < d-1; i++ )
                m.dataend += (m.size[i] - 1)*m.گام[i];
        }
        وإلا
            m.dataend = m.datalimit;
    }
    وإلا
        m.dataend = m.datalimit = m.داده;
}

//======================================= Mat ======================================================

Mat::Mat() CV_NOEXCEPT
    : flags(MAGIC_VAL), dims(0), ردیفها(0), تعداد_ستون(0), داده(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{}

Mat::Mat(صحيح ردیفها_3, صحيح تعداد_ستون_3, صحيح _type)
    : flags(MAGIC_VAL), dims(0), ردیفها(0), تعداد_ستون(0), داده(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(ردیفها_3, تعداد_ستون_3, _type);
}

Mat::Mat(صحيح ردیفها_3, صحيح تعداد_ستون_3, صحيح _type, ثابت Scalar& _s)
    : flags(MAGIC_VAL), dims(0), ردیفها(0), تعداد_ستون(0), داده(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(ردیفها_3, تعداد_ستون_3, _type);
    *هذا = _s;
}

Mat::Mat(اندازه _sz, صحيح _type)
    : flags(MAGIC_VAL), dims(0), ردیفها(0), تعداد_ستون(0), داده(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create( _sz.ارتفاع, _sz.عرض, _type );
}

Mat::Mat(اندازه _sz, صحيح _type, ثابت Scalar& _s)
    : flags(MAGIC_VAL), dims(0), ردیفها(0), تعداد_ستون(0), داده(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_sz.ارتفاع, _sz.عرض, _type);
    *هذا = _s;
}

Mat::Mat(صحيح _dims, ثابت صحيح* _sz, صحيح _type)
    : flags(MAGIC_VAL), dims(0), ردیفها(0), تعداد_ستون(0), داده(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_dims, _sz, _type);
}

Mat::Mat(صحيح _dims, ثابت صحيح* _sz, صحيح _type, ثابت Scalar& _s)
    : flags(MAGIC_VAL), dims(0), ردیفها(0), تعداد_ستون(0), داده(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_dims, _sz, _type);
    *هذا = _s;
}

Mat::Mat(ثابت std::vector<صحيح>& _sz, صحيح _type)
    : flags(MAGIC_VAL), dims(0), ردیفها(0), تعداد_ستون(0), داده(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_sz, _type);
}

Mat::Mat(ثابت std::vector<صحيح>& _sz, صحيح _type, ثابت Scalar& _s)
    : flags(MAGIC_VAL), dims(0), ردیفها(0), تعداد_ستون(0), داده(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_sz, _type);
    *هذا = _s;
}

Mat::Mat(ثابت MatShape& _shape, صحيح _type)
    : flags(MAGIC_VAL), dims(0), ردیفها(0), تعداد_ستون(0), داده(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_shape, _type);
}

Mat::Mat(std::initializer_list<صحيح> _shape, صحيح _type)
    : flags(MAGIC_VAL), dims(0), ردیفها(0), تعداد_ستون(0), داده(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_shape, _type);
}

Mat::Mat(ثابت MatShape& _shape, صحيح _type, ثابت Scalar& _s)
    : flags(MAGIC_VAL), dims(0), ردیفها(0), تعداد_ستون(0), داده(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_shape, _type);
    *هذا = _s;
}

Mat::Mat(std::initializer_list<صحيح> _shape, صحيح _type, ثابت Scalar& _s)
    : flags(MAGIC_VAL), dims(0), ردیفها(0), تعداد_ستون(0), داده(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_shape, _type);
    *هذا = _s;
}

Mat::Mat(ثابت Mat& m)
    : flags(m.flags), dims(m.dims), ردیفها(m.ردیفها), تعداد_ستون(m.تعداد_ستون), داده(m.داده),
      datastart(m.datastart), dataend(m.dataend), datalimit(m.datalimit), allocator(m.allocator),
      u(m.u), size(m.size), گام(m.گام)
{
    إذا( u )
        CV_XADD(&u->refcount, 1);
}

Mat::Mat(صحيح ردیفها_3, صحيح تعداد_ستون_3, صحيح _type, فراغ* داده_3, size_t گام_3)
    : flags(MAGIC_VAL + (_type & TYPE_ماسک)), dims(2), ردیفها(ردیفها_3), تعداد_ستون(تعداد_ستون_3),
      داده((uchar*)داده_3), datastart((uchar*)داده_3), dataend(0), datalimit(0),
      allocator(0), u(0), size(2)
{
    CV_Assert(مجموع() == 0 || داده != عدم);

    size_t esz = CV_ELEM_اندازه(_type), esz1 = CV_ELEM_SIZE1(_type);
    size_t minstep = تعداد_ستون * esz;
    إذا( گام_3 == AUTO_گام )
    {
        گام_3 = minstep;
    }
    وإلا
    {
        CV_Assert( گام_3 >= minstep );
        إذا (گام_3 % esz1 != 0)
        {
            CV_Error(Error::Bad_گام, "Step must be a multiple of esz1");
        }
    }
    size[0] = ردیفها;
    size[1] = تعداد_ستون;
    گام[0] = گام_3;
    گام[1] = esz;
    datalimit = datastart + گام_3 * ردیفها;
    dataend = datalimit - گام_3 + minstep;
    updateContinuityFlag();
}

Mat::Mat(اندازه _sz, صحيح _type, فراغ* داده_3, size_t گام_3)
    : flags(MAGIC_VAL + (_type & TYPE_ماسک)), dims(2), ردیفها(_sz.ارتفاع), تعداد_ستون(_sz.عرض),
      داده((uchar*)داده_3), datastart((uchar*)داده_3), dataend(0), datalimit(0),
      allocator(0), u(0), size(2)
{
    CV_Assert(مجموع() == 0 || داده != عدم);

    size_t esz = CV_ELEM_اندازه(_type), esz1 = CV_ELEM_SIZE1(_type);
    size_t minstep = تعداد_ستون*esz;
    إذا( گام_3 == AUTO_گام || (گام_3 < minstep && ردیفها == 1))
    {
        گام_3 = minstep;
    }
    وإلا
    {
        CV_CheckGE(گام_3, minstep, "");

        إذا (گام_3 % esz1 != 0)
        {
            CV_Error(Error::Bad_گام, "Step must be a multiple of esz1");
        }
    }
    size[0] = ردیفها;
    size[1] = تعداد_ستون;
    گام[0] = گام_3;
    گام[1] = esz;
    datalimit = datastart + گام_3*ردیفها;
    dataend = datalimit - گام_3 + minstep;
    updateContinuityFlag();
}


Mat::~Mat()
{
    release();
}

Mat& Mat::عملگر = (ثابت Mat& m)
{
    إذا( هذا != &m )
    {
        إذا( m.u )
            CV_XADD(&m.u->refcount, 1);
        release();
        flags = m.flags;
        dims = m.dims;
        ردیفها = m.ردیفها;
        تعداد_ستون = m.تعداد_ستون;
        size = m.size;
        گام = m.گام;
        داده = m.داده;
        datastart = m.datastart;
        dataend = m.dataend;
        datalimit = m.datalimit;
        allocator = m.allocator;
        u = m.u;
    }
    أرجع *هذا;
}

Mat Mat::clone() ثابت
{
    Mat m;
    copy_مقصد(m);
    أرجع m;
}

فراغ Mat::تخصیص_بده_مقصد( Mat& m, صحيح _type ) ثابت
{
    إذا( _type < 0 )
        m = *هذا;
    وإلا
        convert_مقصد(m, _type);
}

فراغ Mat::create(صحيح ردیفها_3, صحيح تعداد_ستون_3, صحيح _type)
{
    _type &= TYPE_ماسک;
    إذا( dims <= 2 && ردیفها == ردیفها_3 && تعداد_ستون == تعداد_ستون_3 && type() == _type && داده )
        أرجع;
    صحيح sz[] = {ردیفها_3, تعداد_ستون_3};
    create(2, sz, _type);
}

فراغ Mat::create(اندازه _sz, صحيح _type)
{
    create(_sz.ارتفاع, _sz.عرض, _type);
}

فراغ Mat::create_Same_اندازه(درون_داد_Array m, صحيح type)
{
    MatShape msize = m.shape();
    create(msize, type);
}

فراغ Mat::fit(صحيح _dims, ثابت صحيح* _sizes, صحيح _type)
{
    size_t old_مجموع_بایتها = u ? u->size : 0;
    size_t esz = CV_ELEM_اندازه(_type), new_مجموع = _dims >= 0;
    لكل (صحيح i = 0; i < _dims; i++)
        new_مجموع *= _sizes[i];
    size_t new_مجموع_بایتها = new_مجموع*esz;
    إذا (new_مجموع_بایتها > 0 && (!isContinuous() ||
                              new_مجموع_بایتها > old_مجموع_بایتها ||
                              داده != datastart)) {
        create(_dims, _sizes, _type);
    } وإلا {
        flags = (flags & ~Mat::TYPE_ماسک) | CV_MAT_TYPE(_type);
        صحيح dummy_اندازه = 0;
        مجموعه_اندازه(*هذا, (_dims >= 0 ? _dims : 1), (_dims >= 0 ? _sizes : &dummy_اندازه), اشاره_تهی, درست);
        finalizeHdr(*هذا);
    }
}

فراغ Mat::fit(ثابت std::vector<صحيح>& _shape, صحيح _type)
{
    fit((صحيح)_shape.size(), _shape.داده(), _type);
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
    خودکار it = _shape.begin();
    لكل (صحيح i = 0; i < new_ndims; i++, ++it)
        new_shape[i] = *it;
    fit(new_ndims, new_shape, _type);
}

فراغ Mat::fit(صحيح ردیفها_3, صحيح تعداد_ستون_3, صحيح _type)
{
    _type &= TYPE_ماسک;
    صحيح sz[] = {ردیفها_3, تعداد_ستون_3};
    fit(2, sz, _type);
}

فراغ Mat::fit(اندازه _sz, صحيح _type)
{
    fit(_sz.ارتفاع, _sz.عرض, _type);
}

فراغ Mat::fit_Same_اندازه(درون_داد_Array m, صحيح _type)
{
    fit(m.shape(), _type);
}

فراغ Mat::addref()
{
    إذا( u )
        CV_XADD(&u->refcount, 1);
}

فراغ Mat::release()
{
    إذا( u && CV_XADD(&u->refcount, -1) == 1 )
        deallocate();
    u = عدم;
    datastart = dataend = datalimit = داده = 0;
    لكل(صحيح i = 0; i < dims; i++)
        size.p[i] = 0;
    flags = (flags & CV_MAT_TYPE_ماسک) | MAGIC_VAL;
    dims = ردیفها = تعداد_ستون = 0;
    size.clear();
}

size_t Mat::step1(صحيح i) ثابت
{
    أرجع گام.p[i] / elemSize1();
}

منطقي Mat::empty() ثابت
{
    أرجع داده == 0 || مجموع() == 0;
}

size_t Mat::مجموع() ثابت
{
    إذا( dims <= 2 )
        أرجع (size_t)ردیفها * تعداد_ستون;
    size_t p = 1;
    لكل( صحيح i = 0; i < dims; i++ )
        p *= size[i];
    أرجع p;
}

size_t Mat::مجموع(صحيح شروع_Dim, صحيح انتها_Dim) ثابت
{
    CV_Assert( 0 <= شروع_Dim && شروع_Dim <= انتها_Dim);
    size_t p = 1;
    صحيح انتها_Dim_2 = انتها_Dim <= dims ? انتها_Dim : dims;
    لكل( صحيح i = شروع_Dim; i < انتها_Dim_2; i++ )
        p *= size[i];
    أرجع p;
}

Mat::Mat(Mat&& m) CV_NOEXCEPT
    : flags(m.flags), dims(m.dims), ردیفها(m.ردیفها), تعداد_ستون(m.تعداد_ستون), داده(m.داده),
      datastart(m.datastart), dataend(m.dataend), datalimit(m.datalimit), allocator(m.allocator),
      u(m.u), size(m.size), گام(m.گام)
{
    m.flags = MAGIC_VAL; m.dims = m.ردیفها = m.تعداد_ستون = 0;
    m.size.clear(); m.گام.clear();
    m.داده = عدم; m.datastart = عدم; m.dataend = عدم; m.datalimit = عدم;
    m.allocator = عدم;
    m.u = عدم;
}


Mat& Mat::عملگر = (Mat&& m)
{
    إذا (هذا == &m)
      أرجع *هذا;

    release();
    flags = m.flags; dims = m.dims; ردیفها = m.ردیفها; تعداد_ستون = m.تعداد_ستون; داده = m.داده;
    size = m.size; گام = m.گام;
    datastart = m.datastart; dataend = m.dataend; datalimit = m.datalimit; allocator = m.allocator;
    u = m.u;
    m.flags = MAGIC_VAL; m.dims = m.ردیفها = m.تعداد_ستون = 0;
    m.size.clear(); m.گام.clear();
    m.داده = عدم; m.datastart = عدم; m.dataend = عدم; m.datalimit = عدم;
    m.allocator = عدم;
    m.u = عدم;
    أرجع *هذا;
}


فراغ Mat::create(صحيح d0, ثابت صحيح* _sizes, صحيح _type)
{
    صحيح sz1 = 1, d = d0;
    صحيح i;
    إذا (d == 0) {
        d = 1;
        _sizes = (ثابت صحيح*)&sz1;
    }
    CV_Assert(0 <= d && d <= CV_MAX_DIM && _sizes);
    _type = CV_MAT_TYPE(_type);

    إذا( داده && d == dims && _type == type() )
    {
        لكل( i = 0; i < d; i++ )
            إذا( size[i] != _sizes[i] )
                اكسر;
        إذا( i == d )
            أرجع;
    }

    صحيح _sizes_backup[CV_MAX_DIM]; // #5991
    إذا (_sizes == (هذا->size.p))
    {
        لكل(i = 0; i < d; i++ )
            _sizes_backup[i] = _sizes[i];
        _sizes = _sizes_backup;
    }

    release();
    إذا( d == 0 )
        أرجع;
    flags = (_type & CV_MAT_TYPE_ماسک) | MAGIC_VAL;
    مجموعه_اندازه(*هذا, d, _sizes, 0, درست);

    إذا( مجموع() > 0 )
    {
        MatAllocator *a = allocator, *a0 = getDefaultAllocator();
        إذا(!a)
            a = a0;
        حاول
        {
            u = a->allocate(dims, size.p, _type, 0, گام.p, ACCESS_RW /* ignored */, USAGE_DEFAULT);
            CV_Assert(u != 0);
            allocator = a;
        }
        امسك (...)
        {
            إذا (a == a0)
                ارم;
            u = a0->allocate(dims, size.p, _type, 0, گام.p, ACCESS_RW /* ignored */, USAGE_DEFAULT);
            CV_Assert(u != 0);
            allocator = a0;
        }
        CV_Assert( گام[dims-1] == (size_t)CV_ELEM_اندازه(flags) );
    }

    addref();
    finalizeHdr(*هذا);
    size.dims = dims = d0;
}

فراغ Mat::create(ثابت std::vector<صحيح>& _sizes, صحيح _type)
{
    create((صحيح)_sizes.size(), _sizes.داده(), _type);
}

فراغ Mat::create(ثابت MatShape& _shape, صحيح _type)
{
    إذا (_shape.dims < 0) {
        release();
        أرجع;
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
    خودکار it = _shape.begin();
    لكل (صحيح i = 0; i < new_ndims; i++, ++it)
        new_shape[i] = *it;

    create(new_ndims, new_shape, _type);
}

فراغ Mat::copy_اندازه(ثابت Mat& m)
{
    dims = m.dims;
    تعداد_ستون = m.تعداد_ستون;
    ردیفها = m.ردیفها;
    size = m.size;
    گام = m.گام;
}

فراغ Mat::deallocate()
{
    إذا(u)
    {
        UMat_داده* u_ = u;
        u = عدم;
        (u_->currAllocator ? u_->currAllocator : allocator ? allocator : getDefaultAllocator())->unmap(u_);
    }
}

Mat::Mat(ثابت Mat& m, ثابت Range& ردیف_Range_3, ثابت Range& ستون_Range_3)
    : flags(MAGIC_VAL), dims(0), ردیفها(0), تعداد_ستون(0), داده(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    CV_Assert(m.dims <= MatShape::MAX_DIMS);
    CV_Assert(m.dims >= 2 ||
              (m.dims == 1 && (ردیف_Range_3 == Range::all() ||
                               ردیف_Range_3 == Range(0, m.ردیفها))));
    إذا( m.dims > 2 )
    {
        Range rs[MatShape::MAX_DIMS];
        rs[0] = ردیف_Range_3;
        rs[1] = ستون_Range_3;
        لكل( صحيح i = 2; i < m.dims; i++ )
            rs[i] = Range::all();
        *هذا = m(rs);
        أرجع;
    }

    *هذا = m;
    حاول
    {
        إذا( ردیف_Range_3 != Range::all() && ردیف_Range_3 != Range(0,ردیفها) )
        {
            CV_Assert( 0 <= ردیف_Range_3.شروع && ردیف_Range_3.شروع <= ردیف_Range_3.end
                       && ردیف_Range_3.end <= m.ردیفها );
            size[0] = ردیفها = ردیف_Range_3.size();
            داده += گام*ردیف_Range_3.شروع;
            flags |= SUBMATRIX_FLAG;
        }

        إذا( ستون_Range_3 != Range::all() && ستون_Range_3 != Range(0,تعداد_ستون) )
        {
            CV_Assert( 0 <= ستون_Range_3.شروع && ستون_Range_3.شروع <= ستون_Range_3.end
                       && ستون_Range_3.end <= m.تعداد_ستون );
            size[dims > 1] = تعداد_ستون = ستون_Range_3.size();
            داده += ستون_Range_3.شروع*elem_اندازه();
            flags |= SUBMATRIX_FLAG;
        }
    }
    امسك(...)
    {
        release();
        ارم;
    }

    updateContinuityFlag();

    إذا( ردیفها <= 0 || تعداد_ستون <= 0 )
    {
        release();
        ردیفها = تعداد_ستون = 0;
    }
}


Mat::Mat(ثابت Mat& m, ثابت Rect& roi)
    : flags(m.flags), dims(2), ردیفها(roi.ارتفاع), تعداد_ستون(roi.عرض),
    داده(m.داده + roi.y*m.گام[0]),
    datastart(m.datastart), dataend(m.dataend), datalimit(m.datalimit),
    allocator(m.allocator), u(m.u), size(2)
{
    CV_Assert( m.dims <= 2 );

    size_t esz = CV_ELEM_اندازه(flags);
    داده += roi.x*esz;
    CV_Assert( 0 <= roi.x && 0 <= roi.عرض && roi.x + roi.عرض <= m.تعداد_ستون &&
              0 <= roi.y && 0 <= roi.ارتفاع && roi.y + roi.ارتفاع <= m.ردیفها );
    إذا( roi.عرض < m.تعداد_ستون || roi.ارتفاع < m.ردیفها )
        flags |= SUBMATRIX_FLAG;

    size[0] = ردیفها;
    size[1] = تعداد_ستون;
    گام[0] = m.گام[0];
    گام[1] = esz;
    updateContinuityFlag();

    addref();
    إذا( ردیفها <= 0 || تعداد_ستون <= 0 )
    {
        ردیفها = تعداد_ستون = 0;
        release();
    }
}


Mat::Mat(صحيح _dims, ثابت صحيح* _sizes, صحيح _type, فراغ* داده_3, ثابت size_t* گامها_2)
    : flags(MAGIC_VAL), dims(0), ردیفها(0), تعداد_ستون(0), داده(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    flags |= CV_MAT_TYPE(_type);
    datastart = داده = (uchar*)داده_3;
    إذا (_dims > 0 || داده_3 != اشاره_تهی)
        مجموعه_اندازه(*هذا, _dims, _sizes, گامها_2, درست);
    finalizeHdr(*هذا);
}


Mat::Mat(ثابت std::vector<صحيح>& _sizes, صحيح _type, فراغ* داده_3, ثابت size_t* گامها_2)
    : flags(MAGIC_VAL), dims(0), ردیفها(0), تعداد_ستون(0), داده(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    flags |= CV_MAT_TYPE(_type);
    datastart = داده = (uchar*)داده_3;
    صحيح _dims = (صحيح)_sizes.size();
    إذا (_dims > 0 || داده_3 != اشاره_تهی)
        مجموعه_اندازه(*هذا, _dims, _sizes.داده(), گامها_2, درست);
    finalizeHdr(*هذا);
}

Mat::Mat(ثابت MatShape& _shape, صحيح _type, فراغ* داده_3, ثابت size_t* گامها_2)
    : flags(MAGIC_VAL), dims(0), ردیفها(0), تعداد_ستون(0), داده(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    flags |= CV_MAT_TYPE(_type);
    datastart = داده = (uchar*)داده_3;
    إذا (_shape.dims >= 0) {
        مجموعه_اندازه(*هذا, _shape.dims, _shape.p, گامها_2, درست);
    }
    وإلا {
        CV_Assert(!داده);
    }
    finalizeHdr(*هذا);
}

Mat::Mat(std::initializer_list<صحيح> _shape, صحيح _type, فراغ* داده_3, ثابت size_t* گامها_2)
    : flags(MAGIC_VAL), dims(0), ردیفها(0), تعداد_ستون(0), داده(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    صحيح new_shape[MatShape::MAX_DIMS];
    صحيح _dims = (صحيح)_shape.size();
    CV_Assert(_dims <= MatShape::MAX_DIMS);
    خودکار it = _shape.begin();
    لكل (صحيح i = 0; i < _dims; i++, ++it)
        new_shape[i] = *it;

    flags |= CV_MAT_TYPE(_type);
    datastart = داده = (uchar*)داده_3;
    إذا (_dims > 0 || داده_3 != اشاره_تهی)
        مجموعه_اندازه(*هذا, _dims, new_shape, گامها_2, درست);
    finalizeHdr(*هذا);
}

Mat::Mat(ثابت Mat& m, ثابت Range* ranges)
    : flags(MAGIC_VAL), dims(0), ردیفها(0), تعداد_ستون(0), داده(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    صحيح d = m.dims;

    CV_Assert(ranges);
    لكل( صحيح i = 0; i < d; i++ )
    {
        Range r = ranges[i];
        CV_Assert( r == Range::all() || (0 <= r.شروع && r.شروع < r.end && r.end <= m.size[i]) );
    }
    *هذا = m;
    لكل( صحيح i = 0; i < d; i++ )
    {
        Range r = ranges[i];
        إذا( r != Range::all() && r != Range(0, size.p[i]))
        {
            size.p[i] = r.end - r.شروع;
            داده += r.شروع*گام.p[i];
            flags |= SUBMATRIX_FLAG;
        }
    }

    إذا (d <= 2) {
        ردیفها = d == 2 ? size[0] : 1;
        تعداد_ستون = d <= 0 ? (d >= 0) : size[d > 1];
    }
    updateContinuityFlag();
}

Mat::Mat(ثابت Mat& m, ثابت std::vector<Range>& ranges)
    : flags(MAGIC_VAL), dims(0), ردیفها(0), تعداد_ستون(0), داده(0), datastart(0), dataend(0),
    datalimit(0), allocator(0), u(0)
{
    صحيح d = m.dims;

    CV_Assert((صحيح)ranges.size() == d);
    لكل (صحيح i = 0; i < d; i++)
    {
        Range r = ranges[i];
        CV_Assert(r == Range::all() || (0 <= r.شروع && r.شروع < r.end && r.end <= m.size[i]));
    }
    *هذا = m;
    لكل (صحيح i = 0; i < d; i++)
    {
        Range r = ranges[i];
        إذا (r != Range::all() && r != Range(0, size.p[i]))
        {
            size.p[i] = r.end - r.شروع;
            داده += r.شروع*گام.p[i];
            flags |= SUBMATRIX_FLAG;
        }
    }

    إذا (d <= 2) {
        ردیفها = d == 2 ? size[0] : 1;
        تعداد_ستون = d <= 0 ? (d >= 0) : size[d > 1];
    }
    updateContinuityFlag();
}


Mat Mat::diag(صحيح d) ثابت
{
    CV_Assert( dims == 2 );
    Mat m = *هذا;
    size_t esz = elem_اندازه();
    صحيح len;

    إذا( d >= 0 )
    {
        len = std::min(تعداد_ستون - d, ردیفها);
        m.داده += esz*d;
    }
    وإلا
    {
        len = std::min(ردیفها + d, تعداد_ستون);
        m.داده -= گام[0]*d;
    }
    CV_DbgAssert( len > 0 );

    m.size[0] = m.ردیفها = len;
    m.size[1] = m.تعداد_ستون = 1;
    m.گام[0] += (len > 1 ? esz : 0);

    m.updateContinuityFlag();

    إذا( size() != اندازه(1,1) )
        m.flags |= SUBMATRIX_FLAG;

    أرجع m;
}


فراغ Mat::pop_back(size_t nelems)
{
    CV_Assert( nelems <= (size_t)size.p[0] );
    CV_Assert( dims >= 1 );

    إذا( isSubmatrix() )
        *هذا = ردیف_Range_2(0, size.p[0] - (صحيح)nelems);
    وإلا
    {
        size.p[0] -= (صحيح)nelems;
        إذا (dims == 2)
            ردیفها = size.p[0];
        وإلا إذا (dims == 1)
            تعداد_ستون = size.p[0];
        dataend -= nelems*گام.p[0];
    }
}


فراغ Mat::افزودن_پشت(ثابت فراغ* elem)
{
    size_t r = size.p[0];
    إذا( isSubmatrix() || dataend + گام.p[0] > datalimit )
        reserve( std::max(r + 1, (r*3+1)/2) );

    size_t esz = elem_اندازه();
    کپی_حافظه(داده + r*گام.p[0], elem, esz);
    size.p[0] = صحيح(r + 1);
    إذا (dims == 2)
        ردیفها = size.p[0];
    وإلا إذا (dims == 1)
        تعداد_ستون = size.p[0];
    dataend += گام.p[0];
    uint64 tsz = size.p[0];
    لكل( صحيح i = 1; i < dims; i++ )
        tsz *= size.p[i];
    إذا( esz < گام.p[0] || tsz != (uint64)(صحيح)tsz )
        flags &= ~CONTINUOUS_FLAG;
}


فراغ Mat::reserve(size_t nelems)
{
    ثابت size_t MIN_اندازه = 64;

    CV_Assert( (صحيح)nelems >= 0 );
    إذا( !isSubmatrix() && گام.p[0] != 0 && داده + گام.p[0]*nelems <= datalimit )
        أرجع;

    صحيح r = size.p[0];
    إذا( (size_t)r >= nelems)
        أرجع;

    MatShape newsize = size;
    size_t esz = elem_اندازه();
    newsize.dims = std::max(newsize.dims, 1);
    إذا (dims > 1 && r == 0 && گام.p[0] == 0) {
        گام.p[0] = size.p[1] * (dims == 2 ? esz : گام.p[1]);
    }
    newsize.p[0] = (صحيح)nelems;
    size_t newbytes = newsize.مجموع() * esz;
    إذا (newbytes < MIN_اندازه) {
        newsize.p[0] = 1;
        newsize.p[0] = صحيح(MIN_اندازه / newsize.مجموع() / esz);
    }

    Mat m(newsize, type());
    إذا( r > 0)
    {
        Mat mpart = m.ردیف_Range_2(0, r);
        copy_مقصد(mpart);
    }

    *هذا = m;
    size.p[0] = r;

    إذا (dims == 2)
        ردیفها = size.p[0];
    وإلا إذا (dims == 1)
        تعداد_ستون = size.p[0];
    dataend = داده + گام.p[0]*r;
}


فراغ Mat::reserve_میانگیر(size_t nbytes)
{
    size_t esz = 1;
    صحيح mtype = CV_8UC1;
    إذا (!empty())
    {
        إذا (!isSubmatrix() && داده + nbytes <= dataend)//Should it be datalimit?
            أرجع;
        esz = elem_اندازه();
        mtype = type();
    }

    size_t nelems = (nbytes - 1) / esz + 1;

#إذا SIZE_MAX > UINT_MAX
    CV_Assert(nelems <= size_t(بیشترین_صحیح)*size_t(بیشترین_صحیح));
    صحيح newrows = nelems > size_t(بیشترین_صحیح) ? nelems > 0x400*size_t(بیشترین_صحیح) ? nelems > 0x100000 * size_t(بیشترین_صحیح) ? nelems > 0x40000000 * size_t(بیشترین_صحیح) ?
                  size_t(بیشترین_صحیح) : 0x40000000 : 0x100000 : 0x400 : 1;
#وإلا
    صحيح newrows = nelems > size_t(بیشترین_صحیح) ? 2 : 1;
#پایان_اگر
    صحيح newcols = (صحيح)((nelems - 1) / newrows + 1);

    create(newrows, newcols, mtype);
}


فراغ Mat::resize(size_t nelems)
{
    صحيح r = size.p[0];
    إذا( r == (صحيح)nelems )
        أرجع;
    CV_Assert( (صحيح)nelems >= 0 );

    إذا( isSubmatrix() || داده + گام.p[0]*nelems > datalimit )
        reserve(nelems);

    size.p[0] = (صحيح)nelems;
    إذا (dims == 2)
        ردیفها = size.p[0];
    وإلا إذا (dims == 1)
        تعداد_ستون = size.p[0];
    dataend += (size.p[0] - r)*(int64_t)گام.p[0];

    //updateContinuityFlag(*this);
}


فراغ Mat::resize(size_t nelems, ثابت Scalar& s)
{
    صحيح save_ردیفها = size.p[0];
    resize(nelems);

    إذا( size.p[0] > save_ردیفها )
    {
        Mat part = ردیف_Range_2(save_ردیفها, size.p[0]);
        part = s;
    }
}

فراغ Mat::push_back(ثابت Mat& elems)
{
    size_t r = size.p[0];
    size_t delta = elems.size.p[0];
    إذا( delta == 0 )
        أرجع;
    إذا( هذا == &elems )
    {
        Mat tmp = elems;
        push_back(tmp);
        أرجع;
    }
    إذا( !داده )
    {
        *هذا = elems.clone();
        أرجع;
    }
    CV_Assert(dims > 0);

    size.p[0] = elems.size.p[0];
    منطقي eq = size == elems.size;
    size.p[0] = صحيح(r);
    إذا( !eq )
        CV_Error(cv::Error::StsUnmatchedSizes, "Pushed vector length is not equal to matrix row length");
    إذا( type() != elems.type() )
        CV_Error(cv::Error::StsUnmatchedFormats, "Pushed vector type is not the same as matrix type");
    size_t esz = elem_اندازه();
    size_t minstep = dims <= 1 ? esz : size.p[1] * (dims == 2 ? esz : گام.p[1]);
    size_t step0 = گام.p[0];
    إذا (step0 < minstep) {
        إذا (size.p[0] <= 1)
            گام.p[0] = step0 = minstep;
    }

    إذا( isSubmatrix() || dataend + step0*delta > datalimit )
        reserve( std::max(r + delta, (r*3+1)/2) );

    size.p[0] += صحيح(delta);
    إذا (dims == 2)
        ردیفها = size.p[0];
    وإلا إذا (dims == 1)
        تعداد_ستون = size.p[0];
    dataend += step0*delta;

    //updateContinuityFlag(*this);

    إذا( isContinuous() && elems.isContinuous() )
        کپی_حافظه(داده + r*step0, elems.داده, elems.مجموع()*elems.elem_اندازه());
    وإلا
    {
        Mat part = ردیف_Range_2(صحيح(r), صحيح(r + delta));
        elems.copy_مقصد(part);
    }
}


فراغ Mat::locateROI( اندازه& کامل_اندازه, نقطه& ofs ) ثابت
{
    CV_Assert( dims <= 2 && گام[0] > 0 );
    size_t esz = elem_اندازه(), minstep;
    ptrdiff_t delta1 = داده - datastart, delta2 = dataend - datastart;

    إذا( delta1 == 0 )
        ofs.x = ofs.y = 0;
    وإلا
    {
        ofs.y = (صحيح)(delta1/گام[0]);
        ofs.x = (صحيح)((delta1 - گام[0]*ofs.y)/esz);
        CV_DbgAssert( داده == datastart + ofs.y*گام[0] + ofs.x*esz );
    }
    minstep = (ofs.x + تعداد_ستون)*esz;
    کامل_اندازه.ارتفاع = (صحيح)((delta2 - minstep)/گام[0] + 1);
    کامل_اندازه.ارتفاع = std::max(کامل_اندازه.ارتفاع, ofs.y + ردیفها);
    کامل_اندازه.عرض = (صحيح)((delta2 - گام*(کامل_اندازه.ارتفاع-1))/esz);
    کامل_اندازه.عرض = std::max(کامل_اندازه.عرض, ofs.x + تعداد_ستون);
}

Mat& Mat::adjustROI( صحيح dtop, صحيح dbottom, صحيح dleft, صحيح dright )
{
    CV_Assert( dims <= 2 && گام[0] > 0 );
    اندازه کامل_اندازه; نقطه ofs;
    size_t esz = elem_اندازه();
    locateROI( کامل_اندازه, ofs );
    صحيح row1 = std::min(std::max(ofs.y - dtop, 0), کامل_اندازه.ارتفاع), row2 = std::max(0, std::min(ofs.y + ردیفها + dbottom, کامل_اندازه.ارتفاع));
    صحيح col1 = std::min(std::max(ofs.x - dleft, 0), کامل_اندازه.عرض), col2 = std::max(0, std::min(ofs.x + تعداد_ستون + dright, کامل_اندازه.عرض));
    إذا(row1 > row2)
        std::swap(row1, row2);
    إذا(col1 > col2)
        std::swap(col1, col2);

    إذا (dims == 1) {
        داده += (col1 - ofs.x)*(std::ptrdiff_t)esz;
        تعداد_ستون = col2 - col1;
        size.p[0] = تعداد_ستون;
    } وإلا {
        داده += (row1 - ofs.y)*(std::ptrdiff_t)گام + (col1 - ofs.x)*(std::ptrdiff_t)esz;
        ردیفها = row2 - row1; تعداد_ستون = col2 - col1;
        size.p[0] = ردیفها; size.p[1] = تعداد_ستون;
        updateContinuityFlag();
    }
    أرجع *هذا;
}

Mat Mat::reshape(صحيح new_cn, صحيح new_ردیفها) ثابت
{
    صحيح cn = channels();
    Mat hdr = *هذا;

    إذا( new_cn == 0 )
        new_cn = cn;

    إذا( dims > 2 )
    {
        إذا( new_ردیفها == 0 )
        {
            // special case: just change the number of channnels; retain the same shape,
            // except for the last, innermost dimension
            CV_Assert(size[dims-1]*cn % new_cn == 0);
            hdr.flags = (hdr.flags & ~CV_MAT_CN_ماسک) | ((new_cn-1) << CV_CN_لغزش);
            hdr.گام[dims-1] = CV_ELEM_اندازه(hdr.flags);
            hdr.size[dims-1] = hdr.size[dims-1]*cn / new_cn;
            أرجع hdr;
        }
        CV_Assert( new_ردیفها > 0 );
        صحيح sz[] = { new_ردیفها, (صحيح)(مجموع()*cn/new_ردیفها) };
        أرجع reshape(new_cn, 2, sz);
    }

    صحيح مجموع_عرض = تعداد_ستون * cn;

    إذا( (new_cn > مجموع_عرض || مجموع_عرض % new_cn != 0) && new_ردیفها == 0 )
        new_ردیفها = ردیفها * مجموع_عرض / new_cn;

    إذا( new_ردیفها != 0 && new_ردیفها != ردیفها )
    {
        صحيح مجموع_اندازه_2 = مجموع_عرض * ردیفها;
        إذا( !isContinuous() )
            CV_Error( cv::Error::Bad_گام,
            "The matrix is not continuous, thus its number of rows can not be changed" );

        مجموع_عرض = مجموع_اندازه_2 / new_ردیفها;

        إذا( مجموع_عرض * new_ردیفها != مجموع_اندازه_2 )
            CV_Error( cv::Error::StsBadArg, "The total number of matrix elements "
                                    "is not divisible by the new number of rows" );

        hdr.size[0] = hdr.ردیفها = new_ردیفها;
        hdr.گام[0] = مجموع_عرض * elemSize1();
    } وإلا {
        hdr.size[0] = hdr.ردیفها = ردیفها;
        إذا (dims <= 1)
            hdr.گام[0] = تعداد_ستون * CV_ELEM_اندازه(flags);
    }

    صحيح new_عرض = مجموع_عرض / new_cn;

    إذا( new_عرض * new_cn != مجموع_عرض )
        CV_Error( cv::Error::Bad_رقم_Channels,
        "The total width is not divisible by the new number of channels" );

    hdr.size.dims = hdr.dims = 2;
    hdr.size[1] = hdr.تعداد_ستون = new_عرض;
    hdr.flags = (hdr.flags & ~CV_MAT_CN_ماسک) | ((new_cn-1) << CV_CN_لغزش);
    hdr.گام[1] = CV_ELEM_اندازه(hdr.flags);
    أرجع hdr;
}

Mat Mat::reshape(صحيح _cn, صحيح _newndims, ثابت صحيح* _newsz) ثابت
{
    إذا(_newndims == dims)
    {
        إذا(_newsz == 0)
            أرجع reshape(_cn);
        إذا(_newndims == 2)
            أرجع reshape(_cn, _newsz[0]);
    }

    إذا (isContinuous())
    {
        CV_Assert(_cn >= 0 && _newndims >= 0 && _newndims <= CV_MAX_DIM && (_newndims == 0 || _newsz != 0));

        إذا (_cn == 0)
            _cn = هذا->channels();
        وإلا
            CV_Assert(_cn <= CV_CN_MAX);

        size_t مجموع_elem1_ref = هذا->مجموع() * هذا->channels();
        size_t مجموع_elem1 = _cn;

        Auto_میانگیر<صحيح, 4> newsz_buf( (size_t)_newndims );

        صحيح m1_idx = -1;

        لكل (صحيح i = 0; i < _newndims; i++)
        {
            إذا (_newsz[i] >= 0) {
                إذا (_newsz[i] == 0 && i < dims)
                    newsz_buf[i] = size.p[i];
                وإلا
                    newsz_buf[i] = _newsz[i];
                مجموع_elem1 *= (size_t)newsz_buf[i];
            } وإلا {
                إذا (m1_idx >= 0)
                    CV_Error(cv::Error::Sts_Bad_اندازه, "More than one '-1' occured in the new shape");
                m1_idx = i;
            }
        }

        إذا (m1_idx >= 0) {
            إذا (مجموع_elem1 == 0) {
                CV_Assert(مجموع_elem1_ref == 0);
                مجموع_elem1 = 1;
            }
            CV_Assert(مجموع_elem1_ref % مجموع_elem1 == 0);
            newsz_buf[m1_idx] = (صحيح)(مجموع_elem1_ref / مجموع_elem1);
        } وإلا إذا (مجموع_elem1 != مجموع_elem1_ref) {
            CV_Error(cv::Error::StsUnmatchedSizes, "Requested and source matrices have different count of elements");
        }

        Mat hdr = *هذا;
        hdr.flags = (hdr.flags & ~CV_MAT_CN_ماسک) | ((_cn-1) << CV_CN_لغزش);
        مجموعه_اندازه(hdr, _newndims, newsz_buf.داده(), عدم, درست);

        أرجع hdr;
    }

    CV_Error(cv::Error::StsNotImplemented, "Reshaping of n-dimensional non-continuous matrices is not supported yet");
    // TBD
}

Mat Mat::reshape(صحيح _cn, ثابت std::vector<صحيح>& _newshape) ثابت
{
    صحيح newdims = (صحيح)_newshape.size();
    إذا(newdims == 0 && empty())
    {
        أرجع *هذا;
    }

    أرجع reshape(_cn, newdims, newdims > 0 ? &_newshape[0] : 0);
}

Mat Mat::reshape(صحيح _cn, ثابت MatShape& _newshape) ثابت
{
    إذا (_newshape.dims < 0) {
        صحيح newshape[] = {0};
        أرجع reshape(_cn, 1, newshape);
    }
    أرجع reshape(_cn, _newshape.dims, _newshape.p);
}

Mat Mat::reshape(صحيح _cn, std::initializer_list<صحيح> newshape_) ثابت
{
    صحيح newshape[MatShape::MAX_DIMS];
    size_t i, newshape_dims = newshape_.size();
    CV_Assert(newshape_dims <= (size_t)MatShape::MAX_DIMS);
    خودکار it = newshape_.begin();
    لكل (i = 0; i < newshape_dims; i++, ++it)
        newshape[i] = *it;
    أرجع reshape(_cn, (صحيح)newshape_dims, newshape);
}

Mat Mat::reinterpret(صحيح type) ثابت
{
    type = CV_MAT_TYPE(type);
    CV_Assert(CV_ELEM_اندازه(هذا->type()) == CV_ELEM_اندازه(type));
    Mat m = *هذا;
    m.flags = (m.flags & ~CV_MAT_TYPE_ماسک) | type;
    m.updateContinuityFlag();
    أرجع m;
}

Mat Mat::diag(ثابت Mat& d)
{
    CV_Assert( d.تعداد_ستون == 1 || d.ردیفها == 1 );
    صحيح len = d.ردیفها + d.تعداد_ستون - 1;
    Mat m(len, len, d.type(), Scalar(0));
    Mat md = m.diag();
    إذا( d.تعداد_ستون == 1 )
        d.copy_مقصد(md);
    وإلا
        transpose(d, md);
    أرجع m;
}

صحيح Mat::check_وکتور(صحيح _elemChannels, صحيح عمق_2, منطقي _requireContinuous) ثابت
{
    أرجع داده && (عمق() == عمق_2 || عمق_2 <= 0) &&
        (isContinuous() || !_requireContinuous) &&
        ((dims <= 2 && (((ردیفها == 1 || تعداد_ستون == 1) && channels() == _elemChannels) ||
                        (تعداد_ستون == _elemChannels && channels() == 1))) ||
        (dims == 3 && channels() == 1 && size.p[2] == _elemChannels && (size.p[0] == 1 || size.p[1] == 1) &&
         (isContinuous() || گام.p[1] == گام.p[2]*size.p[2])))
    ? (صحيح)(مجموع()*channels()/_elemChannels) : -1;
}


ساكن مضمن اندازه get_Continuous_اندازه(صحيح flags, صحيح تعداد_ستون, صحيح ردیفها, صحيح عرض_Scale)
{
    int64 sz = (int64)تعداد_ستون * ردیفها * عرض_Scale;
    منطقي has_int_سرریز = sz >= بیشترین_صحیح;
    منطقي isContiguous = (flags & Mat::CONTINUOUS_FLAG) != 0;
    أرجع (isContiguous && !has_int_سرریز)
            ? اندازه((صحيح)sz, 1)
            : اندازه(تعداد_ستون * عرض_Scale, ردیفها);
}

اندازه getContinuousSize2D(Mat& m1, صحيح عرض_Scale)
{
    CV_CheckLE(m1.dims, 2, "");
    أرجع get_Continuous_اندازه(m1.flags,
                              m1.تعداد_ستون, m1.ردیفها, عرض_Scale);
}
اندازه getContinuousSize2D(Mat& m1, Mat& m2, صحيح عرض_Scale)
{
    CV_CheckLE(m1.dims, 2, "");
    CV_CheckLE(m2.dims, 2, "");
    ثابت اندازه sz1 = m1.size();
    إذا (sz1 != m2.size())  // reshape all matrixes to the same size (#4159)
    {
        size_t مجموع_sz = m1.مجموع();
        CV_CheckEQ(مجموع_sz, m2.مجموع(), "");
        منطقي is_m1_وکتور = m1.تعداد_ستون == 1 || m1.ردیفها == 1;
        منطقي is_m2_وکتور = m2.تعداد_ستون == 1 || m2.ردیفها == 1;
        CV_Assert(is_m1_وکتور); CV_Assert(is_m2_وکتور);
        صحيح مجموع = (صحيح)مجموع_sz;  // vector-column
        منطقي isContiguous = ((m1.flags & m2.flags) & Mat::CONTINUOUS_FLAG) != 0;
        منطقي has_int_سرریز = ((int64)مجموع_sz * عرض_Scale) >= بیشترین_صحیح;
        إذا (isContiguous && !has_int_سرریز)
            مجموع = 1; // vector-row
        m1 = m1.reshape(0, مجموع);
        m2 = m2.reshape(0, مجموع);
        CV_Assert(m1.تعداد_ستون == m2.تعداد_ستون && m1.ردیفها == m2.ردیفها);
        أرجع اندازه(m1.تعداد_ستون * عرض_Scale, m1.ردیفها);
    }
    أرجع get_Continuous_اندازه(m1.flags & m2.flags,
                              m1.تعداد_ستون, m1.ردیفها, عرض_Scale);
}

اندازه getContinuousSize2D(Mat& m1, Mat& m2, Mat& m3, صحيح عرض_Scale)
{
    CV_CheckLE(m1.dims, 2, "");
    CV_CheckLE(m2.dims, 2, "");
    CV_CheckLE(m3.dims, 2, "");
    ثابت اندازه sz1 = m1.size();
    إذا (sz1 != m2.size() || sz1 != m3.size())  // reshape all matrixes to the same size (#4159)
    {
        size_t مجموع_sz = m1.مجموع();
        CV_CheckEQ(مجموع_sz, m2.مجموع(), "");
        CV_CheckEQ(مجموع_sz, m3.مجموع(), "");
        منطقي is_m1_وکتور = m1.تعداد_ستون == 1 || m1.ردیفها == 1;
        منطقي is_m2_وکتور = m2.تعداد_ستون == 1 || m2.ردیفها == 1;
        منطقي is_m3_وکتور = m3.تعداد_ستون == 1 || m3.ردیفها == 1;
        CV_Assert(is_m1_وکتور); CV_Assert(is_m2_وکتور); CV_Assert(is_m3_وکتور);
        صحيح مجموع = (صحيح)مجموع_sz;  // vector-column
        منطقي isContiguous = ((m1.flags & m2.flags & m3.flags) & Mat::CONTINUOUS_FLAG) != 0;
        منطقي has_int_سرریز = ((int64)مجموع_sz * عرض_Scale) >= بیشترین_صحیح;
        إذا (isContiguous && !has_int_سرریز)
            مجموع = 1; // vector-row
        m1 = m1.reshape(0, مجموع);
        m2 = m2.reshape(0, مجموع);
        m3 = m3.reshape(0, مجموع);
        CV_Assert(m1.تعداد_ستون == m2.تعداد_ستون && m1.ردیفها == m2.ردیفها && m1.تعداد_ستون == m3.تعداد_ستون && m1.ردیفها == m3.ردیفها);
        أرجع اندازه(m1.تعداد_ستون * عرض_Scale, m1.ردیفها);
    }
    أرجع get_Continuous_اندازه(m1.flags & m2.flags & m3.flags,
                              m1.تعداد_ستون, m1.ردیفها, عرض_Scale);
}

} // cv::
