// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#شامل "precomp.hpp"
#شامل "bufferpool.impl.hpp"

نامخلا cv {

std::string layout_مقصد_String(ڈیٹا_Layout layout)
{
    واپس
        layout == ڈیٹا_LAYOUT_ND ? "ND" :
        layout == ڈیٹا_LAYOUT_NCHW ? "NCHW" :
        layout == ڈیٹا_LAYOUT_NHWC ? "NHWC" :
        layout == ڈیٹا_LAYOUT_بلاک ? "BLOCK" :
        layout == ڈیٹا_LAYOUT_NCDHW ? "NCDHW" :
        layout == ڈیٹا_LAYOUT_NDHWC ? "NDHWC" :
        layout == ڈیٹا_LAYOUT_PLANAR ? "PLANAR" :
        layout == ڈیٹا_LAYOUT_UNKNOWN ? "Unknown" : "???";
}

منطقی عامل == (ثابت MatShape& size1, ثابت MatShape& size2)
{
    اگر (size1.dims != size2.dims)
        واپس جھوٹ;
    اگر (size1.layout != size2.layout &&
        size1.layout != ڈیٹا_LAYOUT_UNKNOWN &&
        size2.layout != ڈیٹا_LAYOUT_UNKNOWN)
        واپس جھوٹ;
    اگر (size1.layout == ڈیٹا_LAYOUT_بلاک &&
        size2.layout == ڈیٹا_LAYOUT_بلاک &&
        size1.C != size2.C)
        واپس جھوٹ;
    برائے (صحیح i = 0; i < size1.dims; i++) {
        اگر (size1.p[i] != size2.p[i])
            واپس جھوٹ;
    }
    واپس سچ;
}

منطقی عامل != (ثابت MatShape& size1, ثابت MatShape& size2)
{
    واپس !(size1 == size2);
}

/////////////////////////// MatShape ////////////////////////////////

MatShape MatShape::scalar()
{
    واپس MatShape(0);
}

خالی MatShape::clear()
{
    dims = -1;
    layout = ڈیٹا_LAYOUT_UNKNOWN;
    C = 0;
    برائے (صحیح i = 0; i < MAX_DIMS; i++)
        p[i] = 0;
}

خالی MatShape::resize(size_t new_سائز, صحیح value)
{
    CV_Assert(new_سائز < (size_t)MAX_DIMS);
    صحیح old_dims = std::max(dims, 0);
    dims = (صحیح)new_سائز;
    برائے (صحیح i = old_dims; i < dims; i++)
        p[i] = value;
}

خالی MatShape::reserve(size_t)
{
    // no op; maybe need to add a check for overflow, but we check it anyway in other operations
}

خالی MatShape::assign(size_t new_سائز, صحیح value)
{
    CV_Assert(new_سائز < (size_t)MAX_DIMS);
    dims = (صحیح)new_سائز;
    برائے (صحیح i = 0; i < dims; i++)
        p[i] = value;
}

خالی MatShape::assign(صحیح new_سائز, صحیح value)
{
    assign((size_t)new_سائز, value);
}

خالی MatShape::assign(ثابت صحیح* begin, ثابت صحیح* end)
{
    تفویض(begin, end);
}

خالی MatShape::تفویض(ثابت صحیح* begin, ثابت صحیح* end)
{
    ptrdiff_t new_سائز = end - begin;
    CV_Assert(0 <= new_سائز && new_سائز < (ptrdiff_t)MAX_DIMS);
    dims = (صحیح)new_سائز;
    برائے (صحیح i = 0; i < dims; i++)
        p[i] = begin[i];
}

صحیح* MatShape::begin() { واپس p; }
ثابت صحیح* MatShape::begin() ثابت { واپس p; }
صحیح* MatShape::end() { واپس p + std::max(dims, 0); }
ثابت صحیح* MatShape::end() ثابت { واپس p + std::max(dims, 0); }
صحیح& MatShape::back() { واپس p[std::max(dims-1, 0)]; }
ثابت صحیح& MatShape::back() ثابت { واپس p[std::max(dims-1, 0)]; }

خالی MatShape::push_back(صحیح value)
{
    CV_Assert(dims+1 < MAX_DIMS);
    dims = std::max(dims+1, 1);
    p[dims-1] = value;
}

خالی MatShape::emplace_back(صحیح value)
{
    push_back(value);
}

خالی MatShape::insert(صحیح* where, صحیح value)
{
    صحیح old_dims = std::max(dims, 0);
    CV_Assert(old_dims+1 < MAX_DIMS);
    ptrdiff_t ofs = where - p;
    CV_Assert(0 <= ofs && ofs <= old_dims);
    dims = old_dims+1;
    برائے (صحیح i = old_dims-1; i >= (صحیح)ofs; i--)
        p[i+1] = p[i];
    p[ofs] = value;
}

خالی MatShape::insert(صحیح* where, size_t گنتی_2, صحیح value)
{
    صحیح old_dims = std::max(dims, 0);
    CV_Assert((size_t)(old_dims+گنتی_2) < (size_t)MAX_DIMS);
    ptrdiff_t ofs = where - p;
    CV_Assert(0 <= ofs && ofs <= old_dims);
    dims = (صحیح)(old_dims+گنتی_2);
    برائے (صحیح i = old_dims-1; i >= (صحیح)ofs; i--)
        p[i+گنتی_2] = p[i];
    برائے (صحیح i = 0; i < (صحیح)گنتی_2; i++)
        p[i+ofs] = value;
}

خالی MatShape::insert(صحیح* where, صحیح گنتی_2, صحیح value)
{
    insert(where, (size_t)گنتی_2, value);
}

خالی MatShape::insert(صحیح* where, ثابت صحیح* begin, ثابت صحیح* end)
{
    داخل_کرو(where, begin, end);
}

خالی MatShape::داخل_کرو(صحیح* where, ثابت صحیح* begin, ثابت صحیح* end)
{
    صحیح old_dims = std::max(dims, 0);
    ptrdiff_t delta = end - begin;
    CV_Assert(0 <= delta && old_dims+delta < MAX_DIMS);
    ptrdiff_t ofs = where - p;
    CV_Assert(0 <= ofs && ofs <= old_dims);
    dims = (صحیح)(old_dims+delta);
    برائے (صحیح i = old_dims-1; i >= (صحیح)ofs; i--)
        p[i+delta] = p[i];
    برائے (صحیح i = 0; i < (صحیح)delta; i++)
        p[i+ofs] = begin[i];
}

خالی MatShape::erase(صحیح* where)
{
    CV_Assert(dims > 0);
    ptrdiff_t ofs = where - p;
    CV_Assert(0 <= ofs && ofs <= dims);
    اگر (ofs == dims)
        واپس;
    dims--;
    برائے (صحیح i = (صحیح)ofs+1; i <= dims; i++)
        p[i-1] = p[i];
}

size_t MatShape::کل() ثابت
{
    size_t نتیجہ = 1;
    اگر (dims < 0)
        واپس 0;
    برائے (صحیح i = 0; i < dims; i++)
        نتیجہ *= p[i];
    واپس نتیجہ;
}

std::vector<صحیح> MatShape::vec() ثابت
{
    اگر (dims < 0)
        واپس std::vector<صحیح>(1, 0);
    واپس std::vector<صحیح>(p, p + dims);
}

std::string MatShape::str() ثابت
{
    std::stringstream sstrm;
    اگر (empty()) {
        sstrm << "<empty>";
    } ورنہ اگر (dims == 0) {
        sstrm << "<scalar>";
    } ورنہ {
        sstrm << "[";
        برائے (صحیح i = 0; i < dims; i++) {
            sstrm << (i > 0 ? " x " : "") << p[i];
        }
        sstrm << "]";
    }
    واپس sstrm.str();
}

ساکن خالی finalize_بلاک_Layout(MatShape& size, صحیح C=0)
{
    اگر (size.layout == ڈیٹا_LAYOUT_بلاک) {
        CV_Assert(size.dims >= 4);
        صحیح C0 = size.p[size.dims-1];
        CV_Assert(C0 > 1 && (C0 & (C0-1)) == 0);
        size.C = C > 0 ? C : size.p[1]*size.p[size.dims-1];
    } ورنہ {
        size.C = 0;
    }
    برائے (صحیح i = std::max(size.dims, 0); i < MatShape::MAX_DIMS; i++)
        size.p[i] = 0;
    اگر (size.dims == 0)
        size.p[0] = 1;
}

MatShape::MatShape()
{
    clear();
}

MatShape::MatShape(size_t dims_, ثابت صحیح* سائز_3, ڈیٹا_Layout layout_, صحیح C_)
{
    layout = layout_;
    CV_Assert(dims_ <= (size_t)MAX_DIMS);
    dims = (صحیح)dims_;
    برائے (صحیح i = 0; i < dims; i++) {
        p[i] = سائز_3 ? سائز_3[i] : 0;
    }
    finalize_بلاک_Layout(*یہ, C_);
}

MatShape::MatShape(size_t dims_, صحیح value, ڈیٹا_Layout layout_)
{
    layout = layout_;
    CV_Assert(dims_ <= (size_t)MAX_DIMS);
    dims = (صحیح)dims_;
    برائے (صحیح i = 0; i < dims; i++) {
        p[i] = value;
    }
    finalize_بلاک_Layout(*یہ, 0);
}

MatShape::MatShape(std::initializer_list<صحیح> shape)
{
    layout = ڈیٹا_LAYOUT_UNKNOWN;
    CV_Assert(shape.size() <= (size_t)MAX_DIMS);
    dims = (صحیح)shape.size();
    خودکار it = shape.begin();
    برائے (صحیح i = 0; i < dims; i++, ++it) {
        p[i] = *it;
    }
    finalize_بلاک_Layout(*یہ, 0);
}

MatShape::MatShape(صحیح dims_, صحیح value, ڈیٹا_Layout layout_)
{
    layout = layout_;
    CV_Assert(dims_ <= MAX_DIMS);
    dims = dims_;
    برائے (صحیح i = 0; i < dims; i++) {
        p[i] = value;
    }
    finalize_بلاک_Layout(*یہ, 0);
}

MatShape::MatShape(ثابت std::vector<صحیح>& shape_, ڈیٹا_Layout layout_, صحیح C_)
{
    layout = layout_;
    size_t shape_سائز = shape_.size();
    CV_Assert(shape_سائز < (size_t)MAX_DIMS);
    dims = (صحیح)shape_سائز;
    برائے (صحیح i = 0; i < dims; i++) {
        p[i] = shape_[i];
    }
    finalize_بلاک_Layout(*یہ, C_);
}

MatShape::MatShape(ثابت صحیح* begin, ثابت صحیح* end, ڈیٹا_Layout layout_, صحیح C_)
{
    layout = layout_;
    ptrdiff_t shape_سائز = end - begin;
    CV_Assert(0 <= shape_سائز && shape_سائز < MAX_DIMS);
    dims = (صحیح)shape_سائز;
    برائے (صحیح i = 0; i < dims; i++) {
        p[i] = begin[i];
    }
    finalize_بلاک_Layout(*یہ, C_);
}

MatShape::MatShape(ثابت MatShape& shape)
{
    dims = shape.dims;
    layout = shape.layout;
    C = shape.C;
    برائے (صحیح i = 0; i < MAX_DIMS; i++)
        p[i] = shape.p[i];
}

MatShape& MatShape::عامل = (ثابت MatShape& shape)
{
    اگر (یہ != &shape) {
        dims = shape.dims;
        layout = shape.layout;
        C = shape.C;
        برائے (صحیح i = 0; i < MAX_DIMS; i++)
            p[i] = shape.p[i];
    }
    واپس *یہ;
}

منطقی MatShape::hasSymbols() ثابت
{
    برائے (صحیح i = 0; i < dims; i++) {
        اگر (p[i] < 0)
            واپس سچ;
    }
    واپس جھوٹ;
}

صحیح MatShape::channels() ثابت
{
    CV_Assert(layout == ڈیٹا_LAYOUT_بلاک || layout == ڈیٹا_LAYOUT_NCHW || layout == ڈیٹا_LAYOUT_NHWC);
    واپس layout == ڈیٹا_LAYOUT_بلاک ? C : p[layout == ڈیٹا_LAYOUT_NCHW ? 1 : dims-1];
}

MatShape MatShape::مقصد_Layout(ڈیٹا_Layout newLayout, صحیح C0) ثابت
{
    CV_Assert(layout == ڈیٹا_LAYOUT_بلاک || layout == ڈیٹا_LAYOUT_NCHW || layout == ڈیٹا_LAYOUT_NHWC);
    CV_Assert(newLayout == ڈیٹا_LAYOUT_بلاک || newLayout == ڈیٹا_LAYOUT_NCHW || newLayout == ڈیٹا_LAYOUT_NHWC);

    MatShape newsize = *یہ;
    newsize.layout = newLayout;

    اگر (newLayout == ڈیٹا_LAYOUT_بلاک) {
        // any => BLOCK
        CV_Assert_N(C0 > 1, (C0 & (C0-1)) == 0);
        صحیح Corig = channels();
        newsize.C = Corig;

        اگر (layout == ڈیٹا_LAYOUT_NHWC) {
            برائے (صحیح i = 2; i < dims; i++) {
                newsize.p[i] = p[i-1];
            }
        }

        newsize.dims += layout != ڈیٹا_LAYOUT_بلاک;
        newsize.p[1] = (Corig + C0 - 1)/C0;
        newsize.p[newsize.dims-1] = C0;
    } ورنہ اگر (layout == ڈیٹا_LAYOUT_بلاک) {
        // BLOCK => any (except for BLOCK, which is handled above)
        newsize.C = 0;
        اگر (newLayout == ڈیٹا_LAYOUT_NHWC) {
            برائے (صحیح i = 2; i < dims; i++) {
                newsize.p[i-1] = p[i];
            }
        }
        newsize.p[newLayout == ڈیٹا_LAYOUT_NCHW ? 1 : dims-2] = C;
        newsize.dims--;
    } ورنہ {
        CV_Assert_N(C0 <= 1);

        اگر (newLayout == layout)
            واپس newsize;

        // NHWC => NCHW
        اگر (newLayout == ڈیٹا_LAYOUT_NCHW) {
            برائے (صحیح i = 2; i < dims; i++)
                newsize.p[i] = p[i-1];
            newsize.p[1] = p[dims-1];
        } ورنہ {
            // NCHW => NHWC
            برائے (صحیح i = 2; i < dims; i++)
                newsize.p[i-1] = p[i];
            newsize.p[dims-1] = p[1];
        }
    }

    واپس newsize;
}

ساکن صحیح normalizeShapeAxis(صحیح axis, صحیح dims)
{
    CV_CheckGE(axis, -dims, "axis is out of range");
    CV_CheckLT(axis, dims, "axis is out of range");
    واپس axis < 0 ? axis + dims : axis;
}

// The semantic NCHW form {N, C, spatial...} of a block ('NC1HWC0') shape, so that axes and
// element counts are the logical ones rather than the C0-padded ones.
ساکن MatShape مقصد_Semantic(ثابت MatShape& shape)
{
    CV_CheckGE(shape.dims, 3, "a block shape is {N, C1, spatial..., C0}");
    واپس shape.مقصد_Layout(ڈیٹا_LAYOUT_NCHW);
}

// Blocks such a result again. Once N or C changes, elements cross the padded C0 lanes and
// the data needs a repack, which a shape operation cannot express.
ساکن MatShape مقصد_بلاک_کا(MatShape نتیجہ, ثابت MatShape& src, ثابت حرف* op)
{
    اگر (نتیجہ.dims < 2 || نتیجہ.p[0] != src.p[0] || نتیجہ.p[1] != src.C)
        CV_Error_(Error::StsNotImplemented,
                  ("%s: the block layout is kept only when the batch and the channel axes stay "
                   "unchanged; convert the shape with toLayout() first", op));
    CV_CheckLE(نتیجہ.dims + 1, (صحیح)MatShape::MAX_DIMS, "too many dimensions");
    نتیجہ.layout = ڈیٹا_LAYOUT_NCHW;
    واپس نتیجہ.مقصد_Layout(ڈیٹا_LAYOUT_بلاک, src.p[src.dims-1]);
}

MatShape MatShape::squeeze(ثابت std::vector<صحیح>& axes) ثابت
{
    CV_Assert(dims >= 0);
    اگر (layout == ڈیٹا_LAYOUT_بلاک)
        واپس مقصد_بلاک_کا(مقصد_Semantic(*یہ).squeeze(axes), *یہ, "squeeze");
    منطقی remove[MAX_DIMS] = {جھوٹ};
    اگر (axes.empty())
    {
        برائے (صحیح i = 0; i < dims; i++)
            remove[i] = p[i] == 1;
    }
    برائے (صحیح a : axes)
    {
        صحیح i = normalizeShapeAxis(a, dims);
        CV_Check(a, !remove[i], "squeeze: duplicate axis");
        CV_Check(p[i], p[i] == 1, "squeeze: the size of a squeezed axis must be 1");
        remove[i] = سچ;
    }
    // a new number of axes: the layout of this shape does not apply
    MatShape نتیجہ;
    نتیجہ.dims = 0;
    برائے (صحیح i = 0; i < dims; i++)
        اگر (!remove[i])
            نتیجہ.p[نتیجہ.dims++] = p[i];
    واپس نتیجہ;
}

MatShape MatShape::unsqueeze(ثابت std::vector<صحیح>& axes) ثابت
{
    CV_Assert(dims >= 0);
    اگر (layout == ڈیٹا_LAYOUT_بلاک)
        واپس مقصد_بلاک_کا(مقصد_Semantic(*یہ).unsqueeze(axes), *یہ, "unsqueeze");
    ثابت صحیح برآمد_Dims = dims + (صحیح)axes.size();
    CV_CheckLE(برآمد_Dims, (صحیح)MAX_DIMS, "unsqueeze: too many dimensions");
    منطقی insert[MAX_DIMS] = {جھوٹ};
    برائے (صحیح a : axes)
    {
        صحیح i = normalizeShapeAxis(a, برآمد_Dims);
        CV_Check(a, !insert[i], "unsqueeze: duplicate axis");
        insert[i] = سچ;
    }
    // a new number of axes: the layout of this shape does not apply
    MatShape نتیجہ;
    نتیجہ.dims = برآمد_Dims;
    برائے (صحیح i = 0, j = 0; i < برآمد_Dims; i++)
        نتیجہ.p[i] = insert[i] ? 1 : p[j++];
    واپس نتیجہ;
}

MatShape MatShape::flatten(صحیح آغاز_Axis, صحیح انتہا_Axis) ثابت
{
    CV_Assert(dims >= 0);
    اگر (layout == ڈیٹا_LAYOUT_بلاک)
        واپس مقصد_بلاک_کا(مقصد_Semantic(*یہ).flatten(آغاز_Axis, انتہا_Axis), *یہ, "flatten");
    اگر (dims == 0)
    {
        CV_Check(آغاز_Axis, آغاز_Axis == 0 || آغاز_Axis == -1, "flatten: axis is out of range");
        CV_Check(انتہا_Axis, انتہا_Axis == 0 || انتہا_Axis == -1, "flatten: axis is out of range");
        واپس MatShape({1});
    }
    صحیح آغاز = normalizeShapeAxis(آغاز_Axis, dims), end = normalizeShapeAxis(انتہا_Axis, dims);
    CV_CheckLE(آغاز, end, "flatten: startAxis must not follow endAxis");
    int64 merged = 1;
    برائے (صحیح i = آغاز; i <= end; i++)
        merged *= p[i];
    CV_CheckLE((دوہرا)merged, (دوہرا)زیادہ_سے_زیادہ_عدد, "flatten: the result is too big");
    // a new number of axes: the layout of this shape does not apply
    MatShape نتیجہ;
    نتیجہ.dims = 0;
    برائے (صحیح i = 0; i < dims; i++)
    {
        اگر (i == آغاز)
            نتیجہ.p[نتیجہ.dims++] = (صحیح)merged;
        ورنہ اگر (i < آغاز || i > end)
            نتیجہ.p[نتیجہ.dims++] = p[i];
    }
    واپس نتیجہ;
}

MatShape MatShape::reshape(ثابت MatShape& newShape, منطقی allowZero) ثابت
{
    CV_Assert(dims >= 0 && newShape.dims >= 0);
    اگر (layout == ڈیٹا_LAYOUT_بلاک)
        واپس مقصد_بلاک_کا(مقصد_Semantic(*یہ).reshape(newShape, allowZero), *یہ, "reshape");
    // a new number of axes: neither the layout of this shape nor the one of the spec applies
    MatShape نتیجہ;
    نتیجہ.dims = newShape.dims;
    std::copy(newShape.p, newShape.p + newShape.dims, نتیجہ.p);
    صحیح inferred = -1;
    int64 معلوم = 1;
    برائے (صحیح i = 0; i < نتیجہ.dims; i++)
    {
        صحیح& sz = نتیجہ.p[i];
        اگر (sz == -1)
        {
            CV_Check(i, inferred < 0, "reshape: at most one axis may be -1");
            inferred = i;
            جاری;
        }
        CV_Check(sz, sz >= 0, "reshape: sizes must be -1 or non-negative");
        اگر (sz == 0 && !allowZero)
        {
            CV_Check(i, i < dims, "reshape: 0 refers to an axis the source does not have");
            sz = p[i];
        }
        معلوم *= sz;
    }
    ثابت int64 کل_2 = (int64)کل();
    اگر (inferred >= 0)
    {
        CV_Check((دوہرا)معلوم, معلوم != 0, "reshape: cannot infer the -1 axis next to a zero-size axis");
        CV_Check((دوہرا)کل_2, کل_2 % معلوم == 0, "reshape: the number of elements does not match");
        CV_CheckLE((دوہرا)(کل_2/معلوم), (دوہرا)زیادہ_سے_زیادہ_عدد, "reshape: the result is too big");
        نتیجہ.p[inferred] = (صحیح)(کل_2/معلوم);
    }
    ورنہ اگر (معلوم != کل_2)
        CV_Error_(Error::StsBadArg, ("reshape: the number of elements does not match: %lld vs %lld",
                                     (طویل طویل)معلوم, (طویل طویل)کل_2));
    واپس نتیجہ;
}

MatShape MatShape::expand(ثابت MatShape& another) ثابت
{
    اگر (dims == 0)
        واپس another;
    اگر (another.dims == 0)
        واپس *یہ;

    اگر ((layout == ڈیٹا_LAYOUT_NCHW || layout == ڈیٹا_LAYOUT_NHWC) &&
        (another.layout == ڈیٹا_LAYOUT_NCHW || another.layout == ڈیٹا_LAYOUT_NHWC)) {
        CV_Assert(layout == another.layout);
    }
    // [TODO] support block layout
    CV_Assert(layout != ڈیٹا_LAYOUT_بلاک && another.layout != ڈیٹا_LAYOUT_بلاک);

    MatShape نتیجہ;

    اگر (dims < 0 || another.dims < 0)
        واپس نتیجہ;

    نتیجہ = *یہ;
    نتیجہ.dims = std::max(dims, another.dims);
    نتیجہ.layout = layout == ڈیٹا_LAYOUT_UNKNOWN ? another.layout :
        layout == ڈیٹا_LAYOUT_ND && (another.layout == ڈیٹا_LAYOUT_NCHW ||
        another.layout == ڈیٹا_LAYOUT_NHWC) ? another.layout : layout;
    برائے (صحیح i = نتیجہ.dims-1; i >= 0; i--) {
        صحیح i1 = i - (نتیجہ.dims - dims);
        صحیح i2 = i - (نتیجہ.dims - another.dims);
        صحیح sz1 = i1 < 0 ? 1 : p[i1];
        صحیح sz2 = i2 < 0 ? 1 : another.p[i2];
        CV_Assert(sz1 == sz2 || sz1 == 1 || sz2 == 1);
        // [TODO] handle symbolic shapes
        نتیجہ.p[i] = sz1 != 1 ? sz1 : sz2;
    }
    واپس نتیجہ;
}

/////////////////////////// MatAllocator ////////////////////////////

خالی MatAllocator::map(UMat_ڈیٹا*, AccessFlag) ثابت
{
}

خالی MatAllocator::unmap(UMat_ڈیٹا* u) ثابت
{
    اگر(u->urefcount == 0 && u->refcount == 0)
    {
        deallocate(u);
    }
}

خالی MatAllocator::download(UMat_ڈیٹا* u, خالی* dstptr,
         صحیح dims, ثابت size_t sz[],
         ثابت size_t srcofs[], ثابت size_t srcstep[],
         ثابت size_t dststep[]) ثابت
{
    اگر(!u)
        واپس;
    صحیح isz[CV_MAX_DIM];
    uchar* srcptr = u->ڈیٹا;
    برائے( صحیح i = 0; i < dims; i++ )
    {
        CV_Assert( sz[i] <= (size_t)زیادہ_سے_زیادہ_عدد );
        اگر( sz[i] == 0 )
            واپس;
        اگر( srcofs )
            srcptr += srcofs[i]*(i <= dims-2 ? srcstep[i] : 1);
        isz[i] = (صحیح)sz[i];
    }

    Mat src(dims, isz, CV_8U, srcptr, srcstep);
    Mat dst(dims, isz, CV_8U, dstptr, dststep);

    ثابت Mat* arrays[] = { &src, &dst };
    uchar* ptrs[2];
    NAryMatIterator it(arrays, ptrs, 2);
    size_t planesz = it.size;

    برائے( size_t j = 0; j < it.nplanes; j++, ++it )
        حافظہ_نقل(ptrs[1], ptrs[0], planesz);
}


خالی MatAllocator::upload(UMat_ڈیٹا* u, ثابت خالی* srcptr, صحیح dims, ثابت size_t sz[],
                    ثابت size_t dstofs[], ثابت size_t dststep[],
                    ثابت size_t srcstep[]) ثابت
{
    اگر(!u)
        واپس;
    صحیح isz[CV_MAX_DIM];
    uchar* dstptr = u->ڈیٹا;
    برائے( صحیح i = 0; i < dims; i++ )
    {
        CV_Assert( sz[i] <= (size_t)زیادہ_سے_زیادہ_عدد );
        اگر( sz[i] == 0 )
            واپس;
        اگر( dstofs )
            dstptr += dstofs[i]*(i <= dims-2 ? dststep[i] : 1);
        isz[i] = (صحیح)sz[i];
    }

    Mat src(dims, isz, CV_8U, (خالی*)srcptr, srcstep);
    Mat dst(dims, isz, CV_8U, dstptr, dststep);

    ثابت Mat* arrays[] = { &src, &dst };
    uchar* ptrs[2];
    NAryMatIterator it(arrays, ptrs, 2);
    size_t planesz = it.size;

    برائے( size_t j = 0; j < it.nplanes; j++, ++it )
        حافظہ_نقل(ptrs[1], ptrs[0], planesz);
}

خالی MatAllocator::copy(UMat_ڈیٹا* usrc, UMat_ڈیٹا* udst, صحیح dims, ثابت size_t sz[],
                  ثابت size_t srcofs[], ثابت size_t srcstep[],
                  ثابت size_t dstofs[], ثابت size_t dststep[], منطقی /*sync*/) ثابت
{
    CV_INSTRUMENT_REGION();

    اگر(!usrc || !udst)
        واپس;
    صحیح isz[CV_MAX_DIM];
    uchar* srcptr = usrc->ڈیٹا;
    uchar* dstptr = udst->ڈیٹا;
    برائے( صحیح i = 0; i < dims; i++ )
    {
        CV_Assert( sz[i] <= (size_t)زیادہ_سے_زیادہ_عدد );
        اگر( sz[i] == 0 )
            واپس;
        اگر( srcofs )
            srcptr += srcofs[i]*(i <= dims-2 ? srcstep[i] : 1);
        اگر( dstofs )
            dstptr += dstofs[i]*(i <= dims-2 ? dststep[i] : 1);
        isz[i] = (صحیح)sz[i];
    }

    Mat src(dims, isz, CV_8U, srcptr, srcstep);
    Mat dst(dims, isz, CV_8U, dstptr, dststep);

    ثابت Mat* arrays[] = { &src, &dst };
    uchar* ptrs[2];
    NAryMatIterator it(arrays, ptrs, 2);
    size_t planesz = it.size;

    برائے( size_t j = 0; j < it.nplanes; j++, ++it )
        حافظہ_نقل(ptrs[1], ptrs[0], planesz);
}

بفر_Pool_Controller* MatAllocator::get_بفر_Pool_Controller(ثابت حرف* id) ثابت
{
    CV_غیر_مستعمل(id);
    ساکن Dummy_بفر_Pool_Controller dummy;
    واپس &dummy;
}

جماعت StdMatAllocator CV_FINAL : عوامی MatAllocator
{
عوامی:
    UMat_ڈیٹا* allocate(صحیح dims, ثابت صحیح* sizes, صحیح type,
                       خالی* data0, size_t* قدم, AccessFlag /*flags*/, UMatUsageFlags /*usageFlags*/) ثابت CV_OVERRIDE
    {
        size_t کل = CV_ELEM_سائز(type);
        برائے( صحیح i = dims-1; i >= 0; i-- )
        {
            اگر( قدم )
            {
                اگر( data0 && قدم[i] != CV_AUTOSTEP )
                {
                    CV_Assert(کل <= قدم[i]);
                    کل = قدم[i];
                }
                ورنہ
                    قدم[i] = کل;
            }
            کل *= sizes[i];
        }
        uchar* ڈیٹا = data0 ? (uchar*)data0 : (uchar*)fastMalloc(کل);
        UMat_ڈیٹا* u = نیا UMat_ڈیٹا(یہ);
        u->ڈیٹا = u->origdata = ڈیٹا;
        u->size = کل;
        اگر(data0)
            u->flags |= UMat_ڈیٹا::USER_ALLOCATED;

        واپس u;
    }

    منطقی allocate(UMat_ڈیٹا* u, AccessFlag /*accessFlags*/, UMatUsageFlags /*usageFlags*/) ثابت CV_OVERRIDE
    {
        اگر(!u) واپس جھوٹ;
        واپس سچ;
    }

    خالی deallocate(UMat_ڈیٹا* u) ثابت CV_OVERRIDE
    {
        اگر(!u)
            واپس;

        CV_Assert(u->urefcount == 0);
        CV_Assert(u->refcount == 0);
        اگر( !(u->flags & UMat_ڈیٹا::USER_ALLOCATED) )
        {
            fastFree(u->origdata);
            u->origdata = 0;
        }
        حذف u;
    }
};

ساکن
MatAllocator*& getDefaultAllocatorMatRef()
{
    ساکن MatAllocator* g_matAllocator = Mat::getStdAllocator();
    واپس g_matAllocator;
}

MatAllocator* Mat::getDefaultAllocator()
{
    واپس getDefaultAllocatorMatRef();
}

خالی Mat::سیٹ_Default_Allocator(MatAllocator* allocator)
{
    getDefaultAllocatorMatRef() = allocator;
}

MatAllocator* Mat::getStdAllocator()
{
    CV_SINGLETON_LAZY_INIT(MatAllocator, نیا StdMatAllocator())
}

//==================================================================================================

خالی سیٹ_سائز( Mat& m, صحیح _dims, ثابت صحیح* _sz, ثابت size_t* اقدام_2, منطقی auto_اقدام)
{
    CV_Assert( 0 <= _dims && _dims <= CV_MAX_DIM && _dims <= MatShape::MAX_DIMS);

    m.dims = _dims;
    m.size = MatShape(_dims, _sz);
    m.قدم[std::max(_dims-1, 0)] = CV_ELEM_سائز(m.flags);
    برائے (صحیح i = _dims-2; i >= 0; i--) {
        size_t autostep = m.size[i+1]*m.قدم[i+1];
        اگر (اقدام_2) {
            m.قدم[i] = اقدام_2[i];
            //CV_Assert(m.step[i] >= autostep);
        } ورنہ اگر (auto_اقدام) {
            m.قدم[i] = autostep;
        } ورنہ {
            m.قدم[i] = 0;
        }
    }

    اگر( _dims <= 2 )
    {
        m.ستون_تعداد = _dims == 0 ? 1 : _sz ? _sz[_dims > 1] : 0;
        m.صفیں = _dims < 2 ? 1 : _sz ? _sz[0] : 0;
    } ورنہ {
        m.ستون_تعداد = m.صفیں = -1;
    }
}

صحیح updateContinuityFlag(صحیح flags, صحیح dims, ثابت صحیح* size, ثابت size_t* قدم)
{
    صحیح i, j;
    اگر (dims <= 1)
        واپس flags | Mat::CONTINUOUS_FLAG;
    برائے( i = 0; i < dims; i++ )
    {
        اگر( size[i] > 1 )
            توڑو;
    }

    uint64 t = (uint64)size[std::min(i, dims-1)]*CV_MAT_CN(flags);
    برائے( j = dims-1; j > i; j-- )
    {
        t *= size[j];
        اگر( قدم[j]*size[j] < قدم[j-1] )
            توڑو;
    }

    اگر( j <= i && t == (uint64)(صحیح)t )
        واپس flags | Mat::CONTINUOUS_FLAG;
    واپس flags & ~Mat::CONTINUOUS_FLAG;
}

خالی Mat::updateContinuityFlag()
{
    flags = cv::updateContinuityFlag(flags, dims, size.p, قدم.p);
}

خالی finalizeHdr(Mat& m)
{
    m.updateContinuityFlag();
    صحیح d = m.dims;
    اگر( d > 2 )
        m.صفیں = m.ستون_تعداد = -1;
    اگر(m.u)
        m.datastart = m.ڈیٹا = m.u->ڈیٹا;
    اگر( m.ڈیٹا && d > 0 )
    {
        m.datalimit = m.datastart + m.size[0]*m.قدم[0];
        اگر( m.size[0] > 0 )
        {
            صحیح lastdim = d > 0 ? d - 1 : 0;
            m.dataend = m.ptr() + m.size[lastdim]*m.قدم[lastdim];
            برائے( صحیح i = 0; i < d-1; i++ )
                m.dataend += (m.size[i] - 1)*m.قدم[i];
        }
        ورنہ
            m.dataend = m.datalimit;
    }
    ورنہ
        m.dataend = m.datalimit = m.ڈیٹا;
}

//======================================= Mat ======================================================

Mat::Mat() CV_NOEXCEPT
    : flags(MAGIC_VAL), dims(0), صفیں(0), ستون_تعداد(0), ڈیٹا(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{}

Mat::Mat(صحیح صفیں_3, صحیح ستون_تعداد_3, صحیح _type)
    : flags(MAGIC_VAL), dims(0), صفیں(0), ستون_تعداد(0), ڈیٹا(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(صفیں_3, ستون_تعداد_3, _type);
}

Mat::Mat(صحیح صفیں_3, صحیح ستون_تعداد_3, صحیح _type, ثابت Scalar& _s)
    : flags(MAGIC_VAL), dims(0), صفیں(0), ستون_تعداد(0), ڈیٹا(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(صفیں_3, ستون_تعداد_3, _type);
    *یہ = _s;
}

Mat::Mat(سائز _sz, صحیح _type)
    : flags(MAGIC_VAL), dims(0), صفیں(0), ستون_تعداد(0), ڈیٹا(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create( _sz.اونچائی, _sz.چوڑائی, _type );
}

Mat::Mat(سائز _sz, صحیح _type, ثابت Scalar& _s)
    : flags(MAGIC_VAL), dims(0), صفیں(0), ستون_تعداد(0), ڈیٹا(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_sz.اونچائی, _sz.چوڑائی, _type);
    *یہ = _s;
}

Mat::Mat(صحیح _dims, ثابت صحیح* _sz, صحیح _type)
    : flags(MAGIC_VAL), dims(0), صفیں(0), ستون_تعداد(0), ڈیٹا(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_dims, _sz, _type);
}

Mat::Mat(صحیح _dims, ثابت صحیح* _sz, صحیح _type, ثابت Scalar& _s)
    : flags(MAGIC_VAL), dims(0), صفیں(0), ستون_تعداد(0), ڈیٹا(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_dims, _sz, _type);
    *یہ = _s;
}

Mat::Mat(ثابت std::vector<صحیح>& _sz, صحیح _type)
    : flags(MAGIC_VAL), dims(0), صفیں(0), ستون_تعداد(0), ڈیٹا(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_sz, _type);
}

Mat::Mat(ثابت std::vector<صحیح>& _sz, صحیح _type, ثابت Scalar& _s)
    : flags(MAGIC_VAL), dims(0), صفیں(0), ستون_تعداد(0), ڈیٹا(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_sz, _type);
    *یہ = _s;
}

Mat::Mat(ثابت MatShape& _shape, صحیح _type)
    : flags(MAGIC_VAL), dims(0), صفیں(0), ستون_تعداد(0), ڈیٹا(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_shape, _type);
}

Mat::Mat(std::initializer_list<صحیح> _shape, صحیح _type)
    : flags(MAGIC_VAL), dims(0), صفیں(0), ستون_تعداد(0), ڈیٹا(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_shape, _type);
}

Mat::Mat(ثابت MatShape& _shape, صحیح _type, ثابت Scalar& _s)
    : flags(MAGIC_VAL), dims(0), صفیں(0), ستون_تعداد(0), ڈیٹا(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_shape, _type);
    *یہ = _s;
}

Mat::Mat(std::initializer_list<صحیح> _shape, صحیح _type, ثابت Scalar& _s)
    : flags(MAGIC_VAL), dims(0), صفیں(0), ستون_تعداد(0), ڈیٹا(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_shape, _type);
    *یہ = _s;
}

Mat::Mat(ثابت Mat& m)
    : flags(m.flags), dims(m.dims), صفیں(m.صفیں), ستون_تعداد(m.ستون_تعداد), ڈیٹا(m.ڈیٹا),
      datastart(m.datastart), dataend(m.dataend), datalimit(m.datalimit), allocator(m.allocator),
      u(m.u), size(m.size), قدم(m.قدم)
{
    اگر( u )
        CV_XADD(&u->refcount, 1);
}

Mat::Mat(صحیح صفیں_3, صحیح ستون_تعداد_3, صحیح _type, خالی* ڈیٹا_3, size_t قدم_3)
    : flags(MAGIC_VAL + (_type & TYPE_نقاب)), dims(2), صفیں(صفیں_3), ستون_تعداد(ستون_تعداد_3),
      ڈیٹا((uchar*)ڈیٹا_3), datastart((uchar*)ڈیٹا_3), dataend(0), datalimit(0),
      allocator(0), u(0), size(2)
{
    CV_Assert(کل() == 0 || ڈیٹا != تہی);

    size_t esz = CV_ELEM_سائز(_type), esz1 = CV_ELEM_SIZE1(_type);
    size_t minstep = ستون_تعداد * esz;
    اگر( قدم_3 == AUTO_قدم )
    {
        قدم_3 = minstep;
    }
    ورنہ
    {
        CV_Assert( قدم_3 >= minstep );
        اگر (قدم_3 % esz1 != 0)
        {
            CV_Error(Error::Bad_قدم, "Step must be a multiple of esz1");
        }
    }
    size[0] = صفیں;
    size[1] = ستون_تعداد;
    قدم[0] = قدم_3;
    قدم[1] = esz;
    datalimit = datastart + قدم_3 * صفیں;
    dataend = datalimit - قدم_3 + minstep;
    updateContinuityFlag();
}

Mat::Mat(سائز _sz, صحیح _type, خالی* ڈیٹا_3, size_t قدم_3)
    : flags(MAGIC_VAL + (_type & TYPE_نقاب)), dims(2), صفیں(_sz.اونچائی), ستون_تعداد(_sz.چوڑائی),
      ڈیٹا((uchar*)ڈیٹا_3), datastart((uchar*)ڈیٹا_3), dataend(0), datalimit(0),
      allocator(0), u(0), size(2)
{
    CV_Assert(کل() == 0 || ڈیٹا != تہی);

    size_t esz = CV_ELEM_سائز(_type), esz1 = CV_ELEM_SIZE1(_type);
    size_t minstep = ستون_تعداد*esz;
    اگر( قدم_3 == AUTO_قدم || (قدم_3 < minstep && صفیں == 1))
    {
        قدم_3 = minstep;
    }
    ورنہ
    {
        CV_CheckGE(قدم_3, minstep, "");

        اگر (قدم_3 % esz1 != 0)
        {
            CV_Error(Error::Bad_قدم, "Step must be a multiple of esz1");
        }
    }
    size[0] = صفیں;
    size[1] = ستون_تعداد;
    قدم[0] = قدم_3;
    قدم[1] = esz;
    datalimit = datastart + قدم_3*صفیں;
    dataend = datalimit - قدم_3 + minstep;
    updateContinuityFlag();
}


Mat::~Mat()
{
    release();
}

Mat& Mat::عامل = (ثابت Mat& m)
{
    اگر( یہ != &m )
    {
        اگر( m.u )
            CV_XADD(&m.u->refcount, 1);
        release();
        flags = m.flags;
        dims = m.dims;
        صفیں = m.صفیں;
        ستون_تعداد = m.ستون_تعداد;
        size = m.size;
        قدم = m.قدم;
        ڈیٹا = m.ڈیٹا;
        datastart = m.datastart;
        dataend = m.dataend;
        datalimit = m.datalimit;
        allocator = m.allocator;
        u = m.u;
    }
    واپس *یہ;
}

Mat Mat::clone() ثابت
{
    Mat m;
    copy_مقصد(m);
    واپس m;
}

خالی Mat::تفویض_مقصد( Mat& m, صحیح _type ) ثابت
{
    اگر( _type < 0 )
        m = *یہ;
    ورنہ
        convert_مقصد(m, _type);
}

خالی Mat::create(صحیح صفیں_3, صحیح ستون_تعداد_3, صحیح _type)
{
    _type &= TYPE_نقاب;
    اگر( dims <= 2 && صفیں == صفیں_3 && ستون_تعداد == ستون_تعداد_3 && type() == _type && ڈیٹا )
        واپس;
    صحیح sz[] = {صفیں_3, ستون_تعداد_3};
    create(2, sz, _type);
}

خالی Mat::create(سائز _sz, صحیح _type)
{
    create(_sz.اونچائی, _sz.چوڑائی, _type);
}

خالی Mat::create_Same_سائز(داخلہ_Array m, صحیح type)
{
    MatShape msize = m.shape();
    create(msize, type);
}

خالی Mat::fit(صحیح _dims, ثابت صحیح* _sizes, صحیح _type)
{
    size_t old_کل_بائٹیں = u ? u->size : 0;
    size_t esz = CV_ELEM_سائز(_type), new_کل = _dims >= 0;
    برائے (صحیح i = 0; i < _dims; i++)
        new_کل *= _sizes[i];
    size_t new_کل_بائٹیں = new_کل*esz;
    اگر (new_کل_بائٹیں > 0 && (!isContinuous() ||
                              new_کل_بائٹیں > old_کل_بائٹیں ||
                              ڈیٹا != datastart)) {
        create(_dims, _sizes, _type);
    } ورنہ {
        flags = (flags & ~Mat::TYPE_نقاب) | CV_MAT_TYPE(_type);
        صحیح dummy_سائز = 0;
        سیٹ_سائز(*یہ, (_dims >= 0 ? _dims : 1), (_dims >= 0 ? _sizes : &dummy_سائز), خالی_اشاریہ, سچ);
        finalizeHdr(*یہ);
    }
}

خالی Mat::fit(ثابت std::vector<صحیح>& _shape, صحیح _type)
{
    fit((صحیح)_shape.size(), _shape.ڈیٹا(), _type);
}

خالی Mat::fit(ثابت MatShape& _shape, صحیح _type)
{
    fit(_shape.dims, _shape.p, _type);
    size.layout = _shape.layout;
    size.C = _shape.C;
}

خالی Mat::fit(std::initializer_list<صحیح> _shape, صحیح _type)
{
    صحیح new_shape[MatShape::MAX_DIMS];
    صحیح new_ndims = (صحیح)_shape.size();
    CV_Assert(new_ndims <= MatShape::MAX_DIMS);
    خودکار it = _shape.begin();
    برائے (صحیح i = 0; i < new_ndims; i++, ++it)
        new_shape[i] = *it;
    fit(new_ndims, new_shape, _type);
}

خالی Mat::fit(صحیح صفیں_3, صحیح ستون_تعداد_3, صحیح _type)
{
    _type &= TYPE_نقاب;
    صحیح sz[] = {صفیں_3, ستون_تعداد_3};
    fit(2, sz, _type);
}

خالی Mat::fit(سائز _sz, صحیح _type)
{
    fit(_sz.اونچائی, _sz.چوڑائی, _type);
}

خالی Mat::fit_Same_سائز(داخلہ_Array m, صحیح _type)
{
    fit(m.shape(), _type);
}

خالی Mat::addref()
{
    اگر( u )
        CV_XADD(&u->refcount, 1);
}

خالی Mat::release()
{
    اگر( u && CV_XADD(&u->refcount, -1) == 1 )
        deallocate();
    u = تہی;
    datastart = dataend = datalimit = ڈیٹا = 0;
    برائے(صحیح i = 0; i < dims; i++)
        size.p[i] = 0;
    flags = (flags & CV_MAT_TYPE_نقاب) | MAGIC_VAL;
    dims = صفیں = ستون_تعداد = 0;
    size.clear();
}

size_t Mat::step1(صحیح i) ثابت
{
    واپس قدم.p[i] / elemSize1();
}

منطقی Mat::empty() ثابت
{
    واپس ڈیٹا == 0 || کل() == 0;
}

size_t Mat::کل() ثابت
{
    اگر( dims <= 2 )
        واپس (size_t)صفیں * ستون_تعداد;
    size_t p = 1;
    برائے( صحیح i = 0; i < dims; i++ )
        p *= size[i];
    واپس p;
}

size_t Mat::کل(صحیح آغاز_Dim, صحیح انتہا_Dim) ثابت
{
    CV_Assert( 0 <= آغاز_Dim && آغاز_Dim <= انتہا_Dim);
    size_t p = 1;
    صحیح انتہا_Dim_2 = انتہا_Dim <= dims ? انتہا_Dim : dims;
    برائے( صحیح i = آغاز_Dim; i < انتہا_Dim_2; i++ )
        p *= size[i];
    واپس p;
}

Mat::Mat(Mat&& m) CV_NOEXCEPT
    : flags(m.flags), dims(m.dims), صفیں(m.صفیں), ستون_تعداد(m.ستون_تعداد), ڈیٹا(m.ڈیٹا),
      datastart(m.datastart), dataend(m.dataend), datalimit(m.datalimit), allocator(m.allocator),
      u(m.u), size(m.size), قدم(m.قدم)
{
    m.flags = MAGIC_VAL; m.dims = m.صفیں = m.ستون_تعداد = 0;
    m.size.clear(); m.قدم.clear();
    m.ڈیٹا = تہی; m.datastart = تہی; m.dataend = تہی; m.datalimit = تہی;
    m.allocator = تہی;
    m.u = تہی;
}


Mat& Mat::عامل = (Mat&& m)
{
    اگر (یہ == &m)
      واپس *یہ;

    release();
    flags = m.flags; dims = m.dims; صفیں = m.صفیں; ستون_تعداد = m.ستون_تعداد; ڈیٹا = m.ڈیٹا;
    size = m.size; قدم = m.قدم;
    datastart = m.datastart; dataend = m.dataend; datalimit = m.datalimit; allocator = m.allocator;
    u = m.u;
    m.flags = MAGIC_VAL; m.dims = m.صفیں = m.ستون_تعداد = 0;
    m.size.clear(); m.قدم.clear();
    m.ڈیٹا = تہی; m.datastart = تہی; m.dataend = تہی; m.datalimit = تہی;
    m.allocator = تہی;
    m.u = تہی;
    واپس *یہ;
}


خالی Mat::create(صحیح d0, ثابت صحیح* _sizes, صحیح _type)
{
    صحیح sz1 = 1, d = d0;
    صحیح i;
    اگر (d == 0) {
        d = 1;
        _sizes = (ثابت صحیح*)&sz1;
    }
    CV_Assert(0 <= d && d <= CV_MAX_DIM && _sizes);
    _type = CV_MAT_TYPE(_type);

    اگر( ڈیٹا && d == dims && _type == type() )
    {
        برائے( i = 0; i < d; i++ )
            اگر( size[i] != _sizes[i] )
                توڑو;
        اگر( i == d )
            واپس;
    }

    صحیح _sizes_backup[CV_MAX_DIM]; // #5991
    اگر (_sizes == (یہ->size.p))
    {
        برائے(i = 0; i < d; i++ )
            _sizes_backup[i] = _sizes[i];
        _sizes = _sizes_backup;
    }

    release();
    اگر( d == 0 )
        واپس;
    flags = (_type & CV_MAT_TYPE_نقاب) | MAGIC_VAL;
    سیٹ_سائز(*یہ, d, _sizes, 0, سچ);

    اگر( کل() > 0 )
    {
        MatAllocator *a = allocator, *a0 = getDefaultAllocator();
        اگر(!a)
            a = a0;
        کوشش
        {
            u = a->allocate(dims, size.p, _type, 0, قدم.p, ACCESS_RW /* ignored */, USAGE_DEFAULT);
            CV_Assert(u != 0);
            allocator = a;
        }
        پکڑو (...)
        {
            اگر (a == a0)
                پھینکو;
            u = a0->allocate(dims, size.p, _type, 0, قدم.p, ACCESS_RW /* ignored */, USAGE_DEFAULT);
            CV_Assert(u != 0);
            allocator = a0;
        }
        CV_Assert( قدم[dims-1] == (size_t)CV_ELEM_سائز(flags) );
    }

    addref();
    finalizeHdr(*یہ);
    size.dims = dims = d0;
}

خالی Mat::create(ثابت std::vector<صحیح>& _sizes, صحیح _type)
{
    create((صحیح)_sizes.size(), _sizes.ڈیٹا(), _type);
}

خالی Mat::create(ثابت MatShape& _shape, صحیح _type)
{
    اگر (_shape.dims < 0) {
        release();
        واپس;
    }
    create(_shape.dims, _shape.p, _type);
    size.layout = _shape.layout;
    size.C = _shape.C;
}

خالی Mat::create(std::initializer_list<صحیح> _shape, صحیح _type)
{
    صحیح new_shape[MatShape::MAX_DIMS];
    صحیح new_ndims = (صحیح)_shape.size();
    CV_Assert(new_ndims <= MatShape::MAX_DIMS);
    خودکار it = _shape.begin();
    برائے (صحیح i = 0; i < new_ndims; i++, ++it)
        new_shape[i] = *it;

    create(new_ndims, new_shape, _type);
}

خالی Mat::copy_سائز(ثابت Mat& m)
{
    dims = m.dims;
    ستون_تعداد = m.ستون_تعداد;
    صفیں = m.صفیں;
    size = m.size;
    قدم = m.قدم;
}

خالی Mat::deallocate()
{
    اگر(u)
    {
        UMat_ڈیٹا* u_ = u;
        u = تہی;
        (u_->currAllocator ? u_->currAllocator : allocator ? allocator : getDefaultAllocator())->unmap(u_);
    }
}

Mat::Mat(ثابت Mat& m, ثابت Range& صف_Range_3, ثابت Range& ستون_Range_3)
    : flags(MAGIC_VAL), dims(0), صفیں(0), ستون_تعداد(0), ڈیٹا(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    CV_Assert(m.dims <= MatShape::MAX_DIMS);
    CV_Assert(m.dims >= 2 ||
              (m.dims == 1 && (صف_Range_3 == Range::all() ||
                               صف_Range_3 == Range(0, m.صفیں))));
    اگر( m.dims > 2 )
    {
        Range rs[MatShape::MAX_DIMS];
        rs[0] = صف_Range_3;
        rs[1] = ستون_Range_3;
        برائے( صحیح i = 2; i < m.dims; i++ )
            rs[i] = Range::all();
        *یہ = m(rs);
        واپس;
    }

    *یہ = m;
    کوشش
    {
        اگر( صف_Range_3 != Range::all() && صف_Range_3 != Range(0,صفیں) )
        {
            CV_Assert( 0 <= صف_Range_3.آغاز && صف_Range_3.آغاز <= صف_Range_3.end
                       && صف_Range_3.end <= m.صفیں );
            size[0] = صفیں = صف_Range_3.size();
            ڈیٹا += قدم*صف_Range_3.آغاز;
            flags |= SUBMATRIX_FLAG;
        }

        اگر( ستون_Range_3 != Range::all() && ستون_Range_3 != Range(0,ستون_تعداد) )
        {
            CV_Assert( 0 <= ستون_Range_3.آغاز && ستون_Range_3.آغاز <= ستون_Range_3.end
                       && ستون_Range_3.end <= m.ستون_تعداد );
            size[dims > 1] = ستون_تعداد = ستون_Range_3.size();
            ڈیٹا += ستون_Range_3.آغاز*elem_سائز();
            flags |= SUBMATRIX_FLAG;
        }
    }
    پکڑو(...)
    {
        release();
        پھینکو;
    }

    updateContinuityFlag();

    اگر( صفیں <= 0 || ستون_تعداد <= 0 )
    {
        release();
        صفیں = ستون_تعداد = 0;
    }
}


Mat::Mat(ثابت Mat& m, ثابت Rect& roi)
    : flags(m.flags), dims(2), صفیں(roi.اونچائی), ستون_تعداد(roi.چوڑائی),
    ڈیٹا(m.ڈیٹا + roi.y*m.قدم[0]),
    datastart(m.datastart), dataend(m.dataend), datalimit(m.datalimit),
    allocator(m.allocator), u(m.u), size(2)
{
    CV_Assert( m.dims <= 2 );

    size_t esz = CV_ELEM_سائز(flags);
    ڈیٹا += roi.x*esz;
    CV_Assert( 0 <= roi.x && 0 <= roi.چوڑائی && roi.x + roi.چوڑائی <= m.ستون_تعداد &&
              0 <= roi.y && 0 <= roi.اونچائی && roi.y + roi.اونچائی <= m.صفیں );
    اگر( roi.چوڑائی < m.ستون_تعداد || roi.اونچائی < m.صفیں )
        flags |= SUBMATRIX_FLAG;

    size[0] = صفیں;
    size[1] = ستون_تعداد;
    قدم[0] = m.قدم[0];
    قدم[1] = esz;
    updateContinuityFlag();

    addref();
    اگر( صفیں <= 0 || ستون_تعداد <= 0 )
    {
        صفیں = ستون_تعداد = 0;
        release();
    }
}


Mat::Mat(صحیح _dims, ثابت صحیح* _sizes, صحیح _type, خالی* ڈیٹا_3, ثابت size_t* اقدام_2)
    : flags(MAGIC_VAL), dims(0), صفیں(0), ستون_تعداد(0), ڈیٹا(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    flags |= CV_MAT_TYPE(_type);
    datastart = ڈیٹا = (uchar*)ڈیٹا_3;
    اگر (_dims > 0 || ڈیٹا_3 != خالی_اشاریہ)
        سیٹ_سائز(*یہ, _dims, _sizes, اقدام_2, سچ);
    finalizeHdr(*یہ);
}


Mat::Mat(ثابت std::vector<صحیح>& _sizes, صحیح _type, خالی* ڈیٹا_3, ثابت size_t* اقدام_2)
    : flags(MAGIC_VAL), dims(0), صفیں(0), ستون_تعداد(0), ڈیٹا(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    flags |= CV_MAT_TYPE(_type);
    datastart = ڈیٹا = (uchar*)ڈیٹا_3;
    صحیح _dims = (صحیح)_sizes.size();
    اگر (_dims > 0 || ڈیٹا_3 != خالی_اشاریہ)
        سیٹ_سائز(*یہ, _dims, _sizes.ڈیٹا(), اقدام_2, سچ);
    finalizeHdr(*یہ);
}

Mat::Mat(ثابت MatShape& _shape, صحیح _type, خالی* ڈیٹا_3, ثابت size_t* اقدام_2)
    : flags(MAGIC_VAL), dims(0), صفیں(0), ستون_تعداد(0), ڈیٹا(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    flags |= CV_MAT_TYPE(_type);
    datastart = ڈیٹا = (uchar*)ڈیٹا_3;
    اگر (_shape.dims >= 0) {
        سیٹ_سائز(*یہ, _shape.dims, _shape.p, اقدام_2, سچ);
    }
    ورنہ {
        CV_Assert(!ڈیٹا);
    }
    finalizeHdr(*یہ);
}

Mat::Mat(std::initializer_list<صحیح> _shape, صحیح _type, خالی* ڈیٹا_3, ثابت size_t* اقدام_2)
    : flags(MAGIC_VAL), dims(0), صفیں(0), ستون_تعداد(0), ڈیٹا(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    صحیح new_shape[MatShape::MAX_DIMS];
    صحیح _dims = (صحیح)_shape.size();
    CV_Assert(_dims <= MatShape::MAX_DIMS);
    خودکار it = _shape.begin();
    برائے (صحیح i = 0; i < _dims; i++, ++it)
        new_shape[i] = *it;

    flags |= CV_MAT_TYPE(_type);
    datastart = ڈیٹا = (uchar*)ڈیٹا_3;
    اگر (_dims > 0 || ڈیٹا_3 != خالی_اشاریہ)
        سیٹ_سائز(*یہ, _dims, new_shape, اقدام_2, سچ);
    finalizeHdr(*یہ);
}

Mat::Mat(ثابت Mat& m, ثابت Range* ranges)
    : flags(MAGIC_VAL), dims(0), صفیں(0), ستون_تعداد(0), ڈیٹا(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    صحیح d = m.dims;

    CV_Assert(ranges);
    برائے( صحیح i = 0; i < d; i++ )
    {
        Range r = ranges[i];
        CV_Assert( r == Range::all() || (0 <= r.آغاز && r.آغاز < r.end && r.end <= m.size[i]) );
    }
    *یہ = m;
    برائے( صحیح i = 0; i < d; i++ )
    {
        Range r = ranges[i];
        اگر( r != Range::all() && r != Range(0, size.p[i]))
        {
            size.p[i] = r.end - r.آغاز;
            ڈیٹا += r.آغاز*قدم.p[i];
            flags |= SUBMATRIX_FLAG;
        }
    }

    اگر (d <= 2) {
        صفیں = d == 2 ? size[0] : 1;
        ستون_تعداد = d <= 0 ? (d >= 0) : size[d > 1];
    }
    updateContinuityFlag();
}

Mat::Mat(ثابت Mat& m, ثابت std::vector<Range>& ranges)
    : flags(MAGIC_VAL), dims(0), صفیں(0), ستون_تعداد(0), ڈیٹا(0), datastart(0), dataend(0),
    datalimit(0), allocator(0), u(0)
{
    صحیح d = m.dims;

    CV_Assert((صحیح)ranges.size() == d);
    برائے (صحیح i = 0; i < d; i++)
    {
        Range r = ranges[i];
        CV_Assert(r == Range::all() || (0 <= r.آغاز && r.آغاز < r.end && r.end <= m.size[i]));
    }
    *یہ = m;
    برائے (صحیح i = 0; i < d; i++)
    {
        Range r = ranges[i];
        اگر (r != Range::all() && r != Range(0, size.p[i]))
        {
            size.p[i] = r.end - r.آغاز;
            ڈیٹا += r.آغاز*قدم.p[i];
            flags |= SUBMATRIX_FLAG;
        }
    }

    اگر (d <= 2) {
        صفیں = d == 2 ? size[0] : 1;
        ستون_تعداد = d <= 0 ? (d >= 0) : size[d > 1];
    }
    updateContinuityFlag();
}


Mat Mat::diag(صحیح d) ثابت
{
    CV_Assert( dims == 2 );
    Mat m = *یہ;
    size_t esz = elem_سائز();
    صحیح len;

    اگر( d >= 0 )
    {
        len = std::min(ستون_تعداد - d, صفیں);
        m.ڈیٹا += esz*d;
    }
    ورنہ
    {
        len = std::min(صفیں + d, ستون_تعداد);
        m.ڈیٹا -= قدم[0]*d;
    }
    CV_DbgAssert( len > 0 );

    m.size[0] = m.صفیں = len;
    m.size[1] = m.ستون_تعداد = 1;
    m.قدم[0] += (len > 1 ? esz : 0);

    m.updateContinuityFlag();

    اگر( size() != سائز(1,1) )
        m.flags |= SUBMATRIX_FLAG;

    واپس m;
}


خالی Mat::pop_back(size_t nelems)
{
    CV_Assert( nelems <= (size_t)size.p[0] );
    CV_Assert( dims >= 1 );

    اگر( isSubmatrix() )
        *یہ = صف_Range_2(0, size.p[0] - (صحیح)nelems);
    ورنہ
    {
        size.p[0] -= (صحیح)nelems;
        اگر (dims == 2)
            صفیں = size.p[0];
        ورنہ اگر (dims == 1)
            ستون_تعداد = size.p[0];
        dataend -= nelems*قدم.p[0];
    }
}


خالی Mat::ڈالو_عقب(ثابت خالی* elem)
{
    size_t r = size.p[0];
    اگر( isSubmatrix() || dataend + قدم.p[0] > datalimit )
        reserve( std::max(r + 1, (r*3+1)/2) );

    size_t esz = elem_سائز();
    حافظہ_نقل(ڈیٹا + r*قدم.p[0], elem, esz);
    size.p[0] = صحیح(r + 1);
    اگر (dims == 2)
        صفیں = size.p[0];
    ورنہ اگر (dims == 1)
        ستون_تعداد = size.p[0];
    dataend += قدم.p[0];
    uint64 tsz = size.p[0];
    برائے( صحیح i = 1; i < dims; i++ )
        tsz *= size.p[i];
    اگر( esz < قدم.p[0] || tsz != (uint64)(صحیح)tsz )
        flags &= ~CONTINUOUS_FLAG;
}


خالی Mat::reserve(size_t nelems)
{
    ثابت size_t MIN_سائز = 64;

    CV_Assert( (صحیح)nelems >= 0 );
    اگر( !isSubmatrix() && قدم.p[0] != 0 && ڈیٹا + قدم.p[0]*nelems <= datalimit )
        واپس;

    صحیح r = size.p[0];
    اگر( (size_t)r >= nelems)
        واپس;

    MatShape newsize = size;
    size_t esz = elem_سائز();
    newsize.dims = std::max(newsize.dims, 1);
    اگر (dims > 1 && r == 0 && قدم.p[0] == 0) {
        قدم.p[0] = size.p[1] * (dims == 2 ? esz : قدم.p[1]);
    }
    newsize.p[0] = (صحیح)nelems;
    size_t newbytes = newsize.کل() * esz;
    اگر (newbytes < MIN_سائز) {
        newsize.p[0] = 1;
        newsize.p[0] = صحیح(MIN_سائز / newsize.کل() / esz);
    }

    Mat m(newsize, type());
    اگر( r > 0)
    {
        Mat mpart = m.صف_Range_2(0, r);
        copy_مقصد(mpart);
    }

    *یہ = m;
    size.p[0] = r;

    اگر (dims == 2)
        صفیں = size.p[0];
    ورنہ اگر (dims == 1)
        ستون_تعداد = size.p[0];
    dataend = ڈیٹا + قدم.p[0]*r;
}


خالی Mat::reserve_بفر(size_t nbytes)
{
    size_t esz = 1;
    صحیح mtype = CV_8UC1;
    اگر (!empty())
    {
        اگر (!isSubmatrix() && ڈیٹا + nbytes <= dataend)//Should it be datalimit?
            واپس;
        esz = elem_سائز();
        mtype = type();
    }

    size_t nelems = (nbytes - 1) / esz + 1;

#اگر SIZE_MAX > UINT_MAX
    CV_Assert(nelems <= size_t(زیادہ_سے_زیادہ_عدد)*size_t(زیادہ_سے_زیادہ_عدد));
    صحیح newrows = nelems > size_t(زیادہ_سے_زیادہ_عدد) ? nelems > 0x400*size_t(زیادہ_سے_زیادہ_عدد) ? nelems > 0x100000 * size_t(زیادہ_سے_زیادہ_عدد) ? nelems > 0x40000000 * size_t(زیادہ_سے_زیادہ_عدد) ?
                  size_t(زیادہ_سے_زیادہ_عدد) : 0x40000000 : 0x100000 : 0x400 : 1;
#ورنہ
    صحیح newrows = nelems > size_t(زیادہ_سے_زیادہ_عدد) ? 2 : 1;
#اگر_ختم
    صحیح newcols = (صحیح)((nelems - 1) / newrows + 1);

    create(newrows, newcols, mtype);
}


خالی Mat::resize(size_t nelems)
{
    صحیح r = size.p[0];
    اگر( r == (صحیح)nelems )
        واپس;
    CV_Assert( (صحیح)nelems >= 0 );

    اگر( isSubmatrix() || ڈیٹا + قدم.p[0]*nelems > datalimit )
        reserve(nelems);

    size.p[0] = (صحیح)nelems;
    اگر (dims == 2)
        صفیں = size.p[0];
    ورنہ اگر (dims == 1)
        ستون_تعداد = size.p[0];
    dataend += (size.p[0] - r)*(int64_t)قدم.p[0];

    //updateContinuityFlag(*this);
}


خالی Mat::resize(size_t nelems, ثابت Scalar& s)
{
    صحیح save_صفیں = size.p[0];
    resize(nelems);

    اگر( size.p[0] > save_صفیں )
    {
        Mat part = صف_Range_2(save_صفیں, size.p[0]);
        part = s;
    }
}

خالی Mat::push_back(ثابت Mat& elems)
{
    size_t r = size.p[0];
    size_t delta = elems.size.p[0];
    اگر( delta == 0 )
        واپس;
    اگر( یہ == &elems )
    {
        Mat tmp = elems;
        push_back(tmp);
        واپس;
    }
    اگر( !ڈیٹا )
    {
        *یہ = elems.clone();
        واپس;
    }
    CV_Assert(dims > 0);

    size.p[0] = elems.size.p[0];
    منطقی eq = size == elems.size;
    size.p[0] = صحیح(r);
    اگر( !eq )
        CV_Error(cv::Error::StsUnmatchedSizes, "Pushed vector length is not equal to matrix row length");
    اگر( type() != elems.type() )
        CV_Error(cv::Error::StsUnmatchedFormats, "Pushed vector type is not the same as matrix type");
    size_t esz = elem_سائز();
    size_t minstep = dims <= 1 ? esz : size.p[1] * (dims == 2 ? esz : قدم.p[1]);
    size_t step0 = قدم.p[0];
    اگر (step0 < minstep) {
        اگر (size.p[0] <= 1)
            قدم.p[0] = step0 = minstep;
    }

    اگر( isSubmatrix() || dataend + step0*delta > datalimit )
        reserve( std::max(r + delta, (r*3+1)/2) );

    size.p[0] += صحیح(delta);
    اگر (dims == 2)
        صفیں = size.p[0];
    ورنہ اگر (dims == 1)
        ستون_تعداد = size.p[0];
    dataend += step0*delta;

    //updateContinuityFlag(*this);

    اگر( isContinuous() && elems.isContinuous() )
        حافظہ_نقل(ڈیٹا + r*step0, elems.ڈیٹا, elems.کل()*elems.elem_سائز());
    ورنہ
    {
        Mat part = صف_Range_2(صحیح(r), صحیح(r + delta));
        elems.copy_مقصد(part);
    }
}


خالی Mat::locateROI( سائز& پورا_سائز, نقطہ& ofs ) ثابت
{
    CV_Assert( dims <= 2 && قدم[0] > 0 );
    size_t esz = elem_سائز(), minstep;
    ptrdiff_t delta1 = ڈیٹا - datastart, delta2 = dataend - datastart;

    اگر( delta1 == 0 )
        ofs.x = ofs.y = 0;
    ورنہ
    {
        ofs.y = (صحیح)(delta1/قدم[0]);
        ofs.x = (صحیح)((delta1 - قدم[0]*ofs.y)/esz);
        CV_DbgAssert( ڈیٹا == datastart + ofs.y*قدم[0] + ofs.x*esz );
    }
    minstep = (ofs.x + ستون_تعداد)*esz;
    پورا_سائز.اونچائی = (صحیح)((delta2 - minstep)/قدم[0] + 1);
    پورا_سائز.اونچائی = std::max(پورا_سائز.اونچائی, ofs.y + صفیں);
    پورا_سائز.چوڑائی = (صحیح)((delta2 - قدم*(پورا_سائز.اونچائی-1))/esz);
    پورا_سائز.چوڑائی = std::max(پورا_سائز.چوڑائی, ofs.x + ستون_تعداد);
}

Mat& Mat::adjustROI( صحیح dtop, صحیح dbottom, صحیح dleft, صحیح dright )
{
    CV_Assert( dims <= 2 && قدم[0] > 0 );
    سائز پورا_سائز; نقطہ ofs;
    size_t esz = elem_سائز();
    locateROI( پورا_سائز, ofs );
    صحیح row1 = std::min(std::max(ofs.y - dtop, 0), پورا_سائز.اونچائی), row2 = std::max(0, std::min(ofs.y + صفیں + dbottom, پورا_سائز.اونچائی));
    صحیح col1 = std::min(std::max(ofs.x - dleft, 0), پورا_سائز.چوڑائی), col2 = std::max(0, std::min(ofs.x + ستون_تعداد + dright, پورا_سائز.چوڑائی));
    اگر(row1 > row2)
        std::swap(row1, row2);
    اگر(col1 > col2)
        std::swap(col1, col2);

    اگر (dims == 1) {
        ڈیٹا += (col1 - ofs.x)*(std::ptrdiff_t)esz;
        ستون_تعداد = col2 - col1;
        size.p[0] = ستون_تعداد;
    } ورنہ {
        ڈیٹا += (row1 - ofs.y)*(std::ptrdiff_t)قدم + (col1 - ofs.x)*(std::ptrdiff_t)esz;
        صفیں = row2 - row1; ستون_تعداد = col2 - col1;
        size.p[0] = صفیں; size.p[1] = ستون_تعداد;
        updateContinuityFlag();
    }
    واپس *یہ;
}

Mat Mat::reshape(صحیح new_cn, صحیح new_صفیں) ثابت
{
    صحیح cn = channels();
    Mat hdr = *یہ;

    اگر( new_cn == 0 )
        new_cn = cn;

    اگر( dims > 2 )
    {
        اگر( new_صفیں == 0 )
        {
            // special case: just change the number of channnels; retain the same shape,
            // except for the last, innermost dimension
            CV_Assert(size[dims-1]*cn % new_cn == 0);
            hdr.flags = (hdr.flags & ~CV_MAT_CN_نقاب) | ((new_cn-1) << CV_CN_سرکاؤ);
            hdr.قدم[dims-1] = CV_ELEM_سائز(hdr.flags);
            hdr.size[dims-1] = hdr.size[dims-1]*cn / new_cn;
            واپس hdr;
        }
        CV_Assert( new_صفیں > 0 );
        صحیح sz[] = { new_صفیں, (صحیح)(کل()*cn/new_صفیں) };
        واپس reshape(new_cn, 2, sz);
    }

    صحیح کل_چوڑائی = ستون_تعداد * cn;

    اگر( (new_cn > کل_چوڑائی || کل_چوڑائی % new_cn != 0) && new_صفیں == 0 )
        new_صفیں = صفیں * کل_چوڑائی / new_cn;

    اگر( new_صفیں != 0 && new_صفیں != صفیں )
    {
        صحیح کل_سائز_2 = کل_چوڑائی * صفیں;
        اگر( !isContinuous() )
            CV_Error( cv::Error::Bad_قدم,
            "The matrix is not continuous, thus its number of rows can not be changed" );

        کل_چوڑائی = کل_سائز_2 / new_صفیں;

        اگر( کل_چوڑائی * new_صفیں != کل_سائز_2 )
            CV_Error( cv::Error::StsBadArg, "The total number of matrix elements "
                                    "is not divisible by the new number of rows" );

        hdr.size[0] = hdr.صفیں = new_صفیں;
        hdr.قدم[0] = کل_چوڑائی * elemSize1();
    } ورنہ {
        hdr.size[0] = hdr.صفیں = صفیں;
        اگر (dims <= 1)
            hdr.قدم[0] = ستون_تعداد * CV_ELEM_سائز(flags);
    }

    صحیح new_چوڑائی = کل_چوڑائی / new_cn;

    اگر( new_چوڑائی * new_cn != کل_چوڑائی )
        CV_Error( cv::Error::Bad_عدد_Channels,
        "The total width is not divisible by the new number of channels" );

    hdr.size.dims = hdr.dims = 2;
    hdr.size[1] = hdr.ستون_تعداد = new_چوڑائی;
    hdr.flags = (hdr.flags & ~CV_MAT_CN_نقاب) | ((new_cn-1) << CV_CN_سرکاؤ);
    hdr.قدم[1] = CV_ELEM_سائز(hdr.flags);
    واپس hdr;
}

Mat Mat::reshape(صحیح _cn, صحیح _newndims, ثابت صحیح* _newsz) ثابت
{
    اگر(_newndims == dims)
    {
        اگر(_newsz == 0)
            واپس reshape(_cn);
        اگر(_newndims == 2)
            واپس reshape(_cn, _newsz[0]);
    }

    اگر (isContinuous())
    {
        CV_Assert(_cn >= 0 && _newndims >= 0 && _newndims <= CV_MAX_DIM && (_newndims == 0 || _newsz != 0));

        اگر (_cn == 0)
            _cn = یہ->channels();
        ورنہ
            CV_Assert(_cn <= CV_CN_MAX);

        size_t کل_elem1_ref = یہ->کل() * یہ->channels();
        size_t کل_elem1 = _cn;

        Auto_بفر<صحیح, 4> newsz_buf( (size_t)_newndims );

        صحیح m1_idx = -1;

        برائے (صحیح i = 0; i < _newndims; i++)
        {
            اگر (_newsz[i] >= 0) {
                اگر (_newsz[i] == 0 && i < dims)
                    newsz_buf[i] = size.p[i];
                ورنہ
                    newsz_buf[i] = _newsz[i];
                کل_elem1 *= (size_t)newsz_buf[i];
            } ورنہ {
                اگر (m1_idx >= 0)
                    CV_Error(cv::Error::Sts_Bad_سائز, "More than one '-1' occured in the new shape");
                m1_idx = i;
            }
        }

        اگر (m1_idx >= 0) {
            اگر (کل_elem1 == 0) {
                CV_Assert(کل_elem1_ref == 0);
                کل_elem1 = 1;
            }
            CV_Assert(کل_elem1_ref % کل_elem1 == 0);
            newsz_buf[m1_idx] = (صحیح)(کل_elem1_ref / کل_elem1);
        } ورنہ اگر (کل_elem1 != کل_elem1_ref) {
            CV_Error(cv::Error::StsUnmatchedSizes, "Requested and source matrices have different count of elements");
        }

        Mat hdr = *یہ;
        hdr.flags = (hdr.flags & ~CV_MAT_CN_نقاب) | ((_cn-1) << CV_CN_سرکاؤ);
        سیٹ_سائز(hdr, _newndims, newsz_buf.ڈیٹا(), تہی, سچ);

        واپس hdr;
    }

    CV_Error(cv::Error::StsNotImplemented, "Reshaping of n-dimensional non-continuous matrices is not supported yet");
    // TBD
}

Mat Mat::reshape(صحیح _cn, ثابت std::vector<صحیح>& _newshape) ثابت
{
    صحیح newdims = (صحیح)_newshape.size();
    اگر(newdims == 0 && empty())
    {
        واپس *یہ;
    }

    واپس reshape(_cn, newdims, newdims > 0 ? &_newshape[0] : 0);
}

Mat Mat::reshape(صحیح _cn, ثابت MatShape& _newshape) ثابت
{
    اگر (_newshape.dims < 0) {
        صحیح newshape[] = {0};
        واپس reshape(_cn, 1, newshape);
    }
    واپس reshape(_cn, _newshape.dims, _newshape.p);
}

Mat Mat::reshape(صحیح _cn, std::initializer_list<صحیح> newshape_) ثابت
{
    صحیح newshape[MatShape::MAX_DIMS];
    size_t i, newshape_dims = newshape_.size();
    CV_Assert(newshape_dims <= (size_t)MatShape::MAX_DIMS);
    خودکار it = newshape_.begin();
    برائے (i = 0; i < newshape_dims; i++, ++it)
        newshape[i] = *it;
    واپس reshape(_cn, (صحیح)newshape_dims, newshape);
}

Mat Mat::reinterpret(صحیح type) ثابت
{
    type = CV_MAT_TYPE(type);
    CV_Assert(CV_ELEM_سائز(یہ->type()) == CV_ELEM_سائز(type));
    Mat m = *یہ;
    m.flags = (m.flags & ~CV_MAT_TYPE_نقاب) | type;
    m.updateContinuityFlag();
    واپس m;
}

Mat Mat::diag(ثابت Mat& d)
{
    CV_Assert( d.ستون_تعداد == 1 || d.صفیں == 1 );
    صحیح len = d.صفیں + d.ستون_تعداد - 1;
    Mat m(len, len, d.type(), Scalar(0));
    Mat md = m.diag();
    اگر( d.ستون_تعداد == 1 )
        d.copy_مقصد(md);
    ورنہ
        transpose(d, md);
    واپس m;
}

صحیح Mat::check_سمتیہ(صحیح _elemChannels, صحیح گہرائی_2, منطقی _requireContinuous) ثابت
{
    واپس ڈیٹا && (گہرائی() == گہرائی_2 || گہرائی_2 <= 0) &&
        (isContinuous() || !_requireContinuous) &&
        ((dims <= 2 && (((صفیں == 1 || ستون_تعداد == 1) && channels() == _elemChannels) ||
                        (ستون_تعداد == _elemChannels && channels() == 1))) ||
        (dims == 3 && channels() == 1 && size.p[2] == _elemChannels && (size.p[0] == 1 || size.p[1] == 1) &&
         (isContinuous() || قدم.p[1] == قدم.p[2]*size.p[2])))
    ? (صحیح)(کل()*channels()/_elemChannels) : -1;
}


ساکن اندرونی سائز get_Continuous_سائز(صحیح flags, صحیح ستون_تعداد, صحیح صفیں, صحیح چوڑائی_Scale)
{
    int64 sz = (int64)ستون_تعداد * صفیں * چوڑائی_Scale;
    منطقی has_int_حد_سے_تجاوز = sz >= زیادہ_سے_زیادہ_عدد;
    منطقی isContiguous = (flags & Mat::CONTINUOUS_FLAG) != 0;
    واپس (isContiguous && !has_int_حد_سے_تجاوز)
            ? سائز((صحیح)sz, 1)
            : سائز(ستون_تعداد * چوڑائی_Scale, صفیں);
}

سائز getContinuousSize2D(Mat& m1, صحیح چوڑائی_Scale)
{
    CV_CheckLE(m1.dims, 2, "");
    واپس get_Continuous_سائز(m1.flags,
                              m1.ستون_تعداد, m1.صفیں, چوڑائی_Scale);
}
سائز getContinuousSize2D(Mat& m1, Mat& m2, صحیح چوڑائی_Scale)
{
    CV_CheckLE(m1.dims, 2, "");
    CV_CheckLE(m2.dims, 2, "");
    ثابت سائز sz1 = m1.size();
    اگر (sz1 != m2.size())  // reshape all matrixes to the same size (#4159)
    {
        size_t کل_sz = m1.کل();
        CV_CheckEQ(کل_sz, m2.کل(), "");
        منطقی is_m1_سمتیہ = m1.ستون_تعداد == 1 || m1.صفیں == 1;
        منطقی is_m2_سمتیہ = m2.ستون_تعداد == 1 || m2.صفیں == 1;
        CV_Assert(is_m1_سمتیہ); CV_Assert(is_m2_سمتیہ);
        صحیح کل = (صحیح)کل_sz;  // vector-column
        منطقی isContiguous = ((m1.flags & m2.flags) & Mat::CONTINUOUS_FLAG) != 0;
        منطقی has_int_حد_سے_تجاوز = ((int64)کل_sz * چوڑائی_Scale) >= زیادہ_سے_زیادہ_عدد;
        اگر (isContiguous && !has_int_حد_سے_تجاوز)
            کل = 1; // vector-row
        m1 = m1.reshape(0, کل);
        m2 = m2.reshape(0, کل);
        CV_Assert(m1.ستون_تعداد == m2.ستون_تعداد && m1.صفیں == m2.صفیں);
        واپس سائز(m1.ستون_تعداد * چوڑائی_Scale, m1.صفیں);
    }
    واپس get_Continuous_سائز(m1.flags & m2.flags,
                              m1.ستون_تعداد, m1.صفیں, چوڑائی_Scale);
}

سائز getContinuousSize2D(Mat& m1, Mat& m2, Mat& m3, صحیح چوڑائی_Scale)
{
    CV_CheckLE(m1.dims, 2, "");
    CV_CheckLE(m2.dims, 2, "");
    CV_CheckLE(m3.dims, 2, "");
    ثابت سائز sz1 = m1.size();
    اگر (sz1 != m2.size() || sz1 != m3.size())  // reshape all matrixes to the same size (#4159)
    {
        size_t کل_sz = m1.کل();
        CV_CheckEQ(کل_sz, m2.کل(), "");
        CV_CheckEQ(کل_sz, m3.کل(), "");
        منطقی is_m1_سمتیہ = m1.ستون_تعداد == 1 || m1.صفیں == 1;
        منطقی is_m2_سمتیہ = m2.ستون_تعداد == 1 || m2.صفیں == 1;
        منطقی is_m3_سمتیہ = m3.ستون_تعداد == 1 || m3.صفیں == 1;
        CV_Assert(is_m1_سمتیہ); CV_Assert(is_m2_سمتیہ); CV_Assert(is_m3_سمتیہ);
        صحیح کل = (صحیح)کل_sz;  // vector-column
        منطقی isContiguous = ((m1.flags & m2.flags & m3.flags) & Mat::CONTINUOUS_FLAG) != 0;
        منطقی has_int_حد_سے_تجاوز = ((int64)کل_sz * چوڑائی_Scale) >= زیادہ_سے_زیادہ_عدد;
        اگر (isContiguous && !has_int_حد_سے_تجاوز)
            کل = 1; // vector-row
        m1 = m1.reshape(0, کل);
        m2 = m2.reshape(0, کل);
        m3 = m3.reshape(0, کل);
        CV_Assert(m1.ستون_تعداد == m2.ستون_تعداد && m1.صفیں == m2.صفیں && m1.ستون_تعداد == m3.ستون_تعداد && m1.صفیں == m3.صفیں);
        واپس سائز(m1.ستون_تعداد * چوڑائی_Scale, m1.صفیں);
    }
    واپس get_Continuous_سائز(m1.flags & m2.flags & m3.flags,
                              m1.ستون_تعداد, m1.صفیں, چوڑائی_Scale);
}

} // cv::
