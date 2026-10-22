// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#ਅਨ੍ਤਰ੍ਭਾਵ "precomp.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "bufferpool.impl.hpp"

ਨਾਮਾਕਾਸ਼ cv {

std::string layout_ਮੰਜ਼ਿਲ_String(ਅੰਕੜੇ_Layout layout)
{
    ਵਾਪਸ
        layout == ਅੰਕੜੇ_LAYOUT_ND ? "ND" :
        layout == ਅੰਕੜੇ_LAYOUT_NCHW ? "NCHW" :
        layout == ਅੰਕੜੇ_LAYOUT_NHWC ? "NHWC" :
        layout == ਅੰਕੜੇ_LAYOUT_ਖੰਡ ? "BLOCK" :
        layout == ਅੰਕੜੇ_LAYOUT_NCDHW ? "NCDHW" :
        layout == ਅੰਕੜੇ_LAYOUT_NDHWC ? "NDHWC" :
        layout == ਅੰਕੜੇ_LAYOUT_PLANAR ? "PLANAR" :
        layout == ਅੰਕੜੇ_LAYOUT_UNKNOWN ? "Unknown" : "???";
}

ਤਾਰਕਿਕ ਸੰਚਾਲਕ == (ਅਚਲ MatShape& size1, ਅਚਲ MatShape& size2)
{
    ਜੇ (size1.dims != size2.dims)
        ਵਾਪਸ ਝੂਠ;
    ਜੇ (size1.layout != size2.layout &&
        size1.layout != ਅੰਕੜੇ_LAYOUT_UNKNOWN &&
        size2.layout != ਅੰਕੜੇ_LAYOUT_UNKNOWN)
        ਵਾਪਸ ਝੂਠ;
    ਜੇ (size1.layout == ਅੰਕੜੇ_LAYOUT_ਖੰਡ &&
        size2.layout == ਅੰਕੜੇ_LAYOUT_ਖੰਡ &&
        size1.C != size2.C)
        ਵਾਪਸ ਝੂਠ;
    ਲਈ (ਪੂਰਨ i = 0; i < size1.dims; i++) {
        ਜੇ (size1.p[i] != size2.p[i])
            ਵਾਪਸ ਝੂਠ;
    }
    ਵਾਪਸ ਸੱਚ;
}

ਤਾਰਕਿਕ ਸੰਚਾਲਕ != (ਅਚਲ MatShape& size1, ਅਚਲ MatShape& size2)
{
    ਵਾਪਸ !(size1 == size2);
}

/////////////////////////// MatShape ////////////////////////////////

MatShape MatShape::scalar()
{
    ਵਾਪਸ MatShape(0);
}

ਖਾਲੀ MatShape::clear()
{
    dims = -1;
    layout = ਅੰਕੜੇ_LAYOUT_UNKNOWN;
    C = 0;
    ਲਈ (ਪੂਰਨ i = 0; i < MAX_DIMS; i++)
        p[i] = 0;
}

ਖਾਲੀ MatShape::resize(size_t new_ਮਾਪ, ਪੂਰਨ value)
{
    CV_Assert(new_ਮਾਪ < (size_t)MAX_DIMS);
    ਪੂਰਨ old_dims = std::max(dims, 0);
    dims = (ਪੂਰਨ)new_ਮਾਪ;
    ਲਈ (ਪੂਰਨ i = old_dims; i < dims; i++)
        p[i] = value;
}

ਖਾਲੀ MatShape::reserve(size_t)
{
    // no op; maybe need to add a check for overflow, but we check it anyway in other operations
}

ਖਾਲੀ MatShape::assign(size_t new_ਮਾਪ, ਪੂਰਨ value)
{
    CV_Assert(new_ਮਾਪ < (size_t)MAX_DIMS);
    dims = (ਪੂਰਨ)new_ਮਾਪ;
    ਲਈ (ਪੂਰਨ i = 0; i < dims; i++)
        p[i] = value;
}

ਖਾਲੀ MatShape::assign(ਪੂਰਨ new_ਮਾਪ, ਪੂਰਨ value)
{
    assign((size_t)new_ਮਾਪ, value);
}

ਖਾਲੀ MatShape::assign(ਅਚਲ ਪੂਰਨ* begin, ਅਚਲ ਪੂਰਨ* end)
{
    ਸੌਂਪੋ(begin, end);
}

ਖਾਲੀ MatShape::ਸੌਂਪੋ(ਅਚਲ ਪੂਰਨ* begin, ਅਚਲ ਪੂਰਨ* end)
{
    ptrdiff_t new_ਮਾਪ = end - begin;
    CV_Assert(0 <= new_ਮਾਪ && new_ਮਾਪ < (ptrdiff_t)MAX_DIMS);
    dims = (ਪੂਰਨ)new_ਮਾਪ;
    ਲਈ (ਪੂਰਨ i = 0; i < dims; i++)
        p[i] = begin[i];
}

ਪੂਰਨ* MatShape::begin() { ਵਾਪਸ p; }
ਅਚਲ ਪੂਰਨ* MatShape::begin() ਅਚਲ { ਵਾਪਸ p; }
ਪੂਰਨ* MatShape::end() { ਵਾਪਸ p + std::max(dims, 0); }
ਅਚਲ ਪੂਰਨ* MatShape::end() ਅਚਲ { ਵਾਪਸ p + std::max(dims, 0); }
ਪੂਰਨ& MatShape::back() { ਵਾਪਸ p[std::max(dims-1, 0)]; }
ਅਚਲ ਪੂਰਨ& MatShape::back() ਅਚਲ { ਵਾਪਸ p[std::max(dims-1, 0)]; }

ਖਾਲੀ MatShape::push_back(ਪੂਰਨ value)
{
    CV_Assert(dims+1 < MAX_DIMS);
    dims = std::max(dims+1, 1);
    p[dims-1] = value;
}

ਖਾਲੀ MatShape::emplace_back(ਪੂਰਨ value)
{
    push_back(value);
}

ਖਾਲੀ MatShape::insert(ਪੂਰਨ* where, ਪੂਰਨ value)
{
    ਪੂਰਨ old_dims = std::max(dims, 0);
    CV_Assert(old_dims+1 < MAX_DIMS);
    ptrdiff_t ofs = where - p;
    CV_Assert(0 <= ofs && ofs <= old_dims);
    dims = old_dims+1;
    ਲਈ (ਪੂਰਨ i = old_dims-1; i >= (ਪੂਰਨ)ofs; i--)
        p[i+1] = p[i];
    p[ofs] = value;
}

ਖਾਲੀ MatShape::insert(ਪੂਰਨ* where, size_t ਗਿਣਤੀ_3, ਪੂਰਨ value)
{
    ਪੂਰਨ old_dims = std::max(dims, 0);
    CV_Assert((size_t)(old_dims+ਗਿਣਤੀ_3) < (size_t)MAX_DIMS);
    ptrdiff_t ofs = where - p;
    CV_Assert(0 <= ofs && ofs <= old_dims);
    dims = (ਪੂਰਨ)(old_dims+ਗਿਣਤੀ_3);
    ਲਈ (ਪੂਰਨ i = old_dims-1; i >= (ਪੂਰਨ)ofs; i--)
        p[i+ਗਿਣਤੀ_3] = p[i];
    ਲਈ (ਪੂਰਨ i = 0; i < (ਪੂਰਨ)ਗਿਣਤੀ_3; i++)
        p[i+ofs] = value;
}

ਖਾਲੀ MatShape::insert(ਪੂਰਨ* where, ਪੂਰਨ ਗਿਣਤੀ_3, ਪੂਰਨ value)
{
    insert(where, (size_t)ਗਿਣਤੀ_3, value);
}

ਖਾਲੀ MatShape::insert(ਪੂਰਨ* where, ਅਚਲ ਪੂਰਨ* begin, ਅਚਲ ਪੂਰਨ* end)
{
    ਪਾਓ(where, begin, end);
}

ਖਾਲੀ MatShape::ਪਾਓ(ਪੂਰਨ* where, ਅਚਲ ਪੂਰਨ* begin, ਅਚਲ ਪੂਰਨ* end)
{
    ਪੂਰਨ old_dims = std::max(dims, 0);
    ptrdiff_t delta = end - begin;
    CV_Assert(0 <= delta && old_dims+delta < MAX_DIMS);
    ptrdiff_t ofs = where - p;
    CV_Assert(0 <= ofs && ofs <= old_dims);
    dims = (ਪੂਰਨ)(old_dims+delta);
    ਲਈ (ਪੂਰਨ i = old_dims-1; i >= (ਪੂਰਨ)ofs; i--)
        p[i+delta] = p[i];
    ਲਈ (ਪੂਰਨ i = 0; i < (ਪੂਰਨ)delta; i++)
        p[i+ofs] = begin[i];
}

ਖਾਲੀ MatShape::erase(ਪੂਰਨ* where)
{
    CV_Assert(dims > 0);
    ptrdiff_t ofs = where - p;
    CV_Assert(0 <= ofs && ofs <= dims);
    ਜੇ (ofs == dims)
        ਵਾਪਸ;
    dims--;
    ਲਈ (ਪੂਰਨ i = (ਪੂਰਨ)ofs+1; i <= dims; i++)
        p[i-1] = p[i];
}

size_t MatShape::ਕੁੱਲ() ਅਚਲ
{
    size_t ਨਤੀਜਾ = 1;
    ਜੇ (dims < 0)
        ਵਾਪਸ 0;
    ਲਈ (ਪੂਰਨ i = 0; i < dims; i++)
        ਨਤੀਜਾ *= p[i];
    ਵਾਪਸ ਨਤੀਜਾ;
}

std::vector<ਪੂਰਨ> MatShape::vec() ਅਚਲ
{
    ਜੇ (dims < 0)
        ਵਾਪਸ std::vector<ਪੂਰਨ>(1, 0);
    ਵਾਪਸ std::vector<ਪੂਰਨ>(p, p + dims);
}

std::string MatShape::str() ਅਚਲ
{
    std::stringstream sstrm;
    ਜੇ (empty()) {
        sstrm << "<empty>";
    } ਨਹੀਂਤਾਂ ਜੇ (dims == 0) {
        sstrm << "<scalar>";
    } ਨਹੀਂਤਾਂ {
        sstrm << "[";
        ਲਈ (ਪੂਰਨ i = 0; i < dims; i++) {
            sstrm << (i > 0 ? " x " : "") << p[i];
        }
        sstrm << "]";
    }
    ਵਾਪਸ sstrm.str();
}

ਸਥਿਰ ਖਾਲੀ finalize_ਖੰਡ_Layout(MatShape& size, ਪੂਰਨ C=0)
{
    ਜੇ (size.layout == ਅੰਕੜੇ_LAYOUT_ਖੰਡ) {
        CV_Assert(size.dims >= 4);
        ਪੂਰਨ C0 = size.p[size.dims-1];
        CV_Assert(C0 > 1 && (C0 & (C0-1)) == 0);
        size.C = C > 0 ? C : size.p[1]*size.p[size.dims-1];
    } ਨਹੀਂਤਾਂ {
        size.C = 0;
    }
    ਲਈ (ਪੂਰਨ i = std::max(size.dims, 0); i < MatShape::MAX_DIMS; i++)
        size.p[i] = 0;
    ਜੇ (size.dims == 0)
        size.p[0] = 1;
}

MatShape::MatShape()
{
    clear();
}

MatShape::MatShape(size_t dims_, ਅਚਲ ਪੂਰਨ* ਮਾਪ_3, ਅੰਕੜੇ_Layout layout_, ਪੂਰਨ C_)
{
    layout = layout_;
    CV_Assert(dims_ <= (size_t)MAX_DIMS);
    dims = (ਪੂਰਨ)dims_;
    ਲਈ (ਪੂਰਨ i = 0; i < dims; i++) {
        p[i] = ਮਾਪ_3 ? ਮਾਪ_3[i] : 0;
    }
    finalize_ਖੰਡ_Layout(*ਇਹ, C_);
}

MatShape::MatShape(size_t dims_, ਪੂਰਨ value, ਅੰਕੜੇ_Layout layout_)
{
    layout = layout_;
    CV_Assert(dims_ <= (size_t)MAX_DIMS);
    dims = (ਪੂਰਨ)dims_;
    ਲਈ (ਪੂਰਨ i = 0; i < dims; i++) {
        p[i] = value;
    }
    finalize_ਖੰਡ_Layout(*ਇਹ, 0);
}

MatShape::MatShape(std::initializer_list<ਪੂਰਨ> shape)
{
    layout = ਅੰਕੜੇ_LAYOUT_UNKNOWN;
    CV_Assert(shape.size() <= (size_t)MAX_DIMS);
    dims = (ਪੂਰਨ)shape.size();
    ਸਵੈਚਾਲਿਤ it = shape.begin();
    ਲਈ (ਪੂਰਨ i = 0; i < dims; i++, ++it) {
        p[i] = *it;
    }
    finalize_ਖੰਡ_Layout(*ਇਹ, 0);
}

MatShape::MatShape(ਪੂਰਨ dims_, ਪੂਰਨ value, ਅੰਕੜੇ_Layout layout_)
{
    layout = layout_;
    CV_Assert(dims_ <= MAX_DIMS);
    dims = dims_;
    ਲਈ (ਪੂਰਨ i = 0; i < dims; i++) {
        p[i] = value;
    }
    finalize_ਖੰਡ_Layout(*ਇਹ, 0);
}

MatShape::MatShape(ਅਚਲ std::vector<ਪੂਰਨ>& shape_, ਅੰਕੜੇ_Layout layout_, ਪੂਰਨ C_)
{
    layout = layout_;
    size_t shape_ਮਾਪ = shape_.size();
    CV_Assert(shape_ਮਾਪ < (size_t)MAX_DIMS);
    dims = (ਪੂਰਨ)shape_ਮਾਪ;
    ਲਈ (ਪੂਰਨ i = 0; i < dims; i++) {
        p[i] = shape_[i];
    }
    finalize_ਖੰਡ_Layout(*ਇਹ, C_);
}

MatShape::MatShape(ਅਚਲ ਪੂਰਨ* begin, ਅਚਲ ਪੂਰਨ* end, ਅੰਕੜੇ_Layout layout_, ਪੂਰਨ C_)
{
    layout = layout_;
    ptrdiff_t shape_ਮਾਪ = end - begin;
    CV_Assert(0 <= shape_ਮਾਪ && shape_ਮਾਪ < MAX_DIMS);
    dims = (ਪੂਰਨ)shape_ਮਾਪ;
    ਲਈ (ਪੂਰਨ i = 0; i < dims; i++) {
        p[i] = begin[i];
    }
    finalize_ਖੰਡ_Layout(*ਇਹ, C_);
}

MatShape::MatShape(ਅਚਲ MatShape& shape)
{
    dims = shape.dims;
    layout = shape.layout;
    C = shape.C;
    ਲਈ (ਪੂਰਨ i = 0; i < MAX_DIMS; i++)
        p[i] = shape.p[i];
}

MatShape& MatShape::ਸੰਚਾਲਕ = (ਅਚਲ MatShape& shape)
{
    ਜੇ (ਇਹ != &shape) {
        dims = shape.dims;
        layout = shape.layout;
        C = shape.C;
        ਲਈ (ਪੂਰਨ i = 0; i < MAX_DIMS; i++)
            p[i] = shape.p[i];
    }
    ਵਾਪਸ *ਇਹ;
}

ਤਾਰਕਿਕ MatShape::hasSymbols() ਅਚਲ
{
    ਲਈ (ਪੂਰਨ i = 0; i < dims; i++) {
        ਜੇ (p[i] < 0)
            ਵਾਪਸ ਸੱਚ;
    }
    ਵਾਪਸ ਝੂਠ;
}

ਪੂਰਨ MatShape::channels() ਅਚਲ
{
    CV_Assert(layout == ਅੰਕੜੇ_LAYOUT_ਖੰਡ || layout == ਅੰਕੜੇ_LAYOUT_NCHW || layout == ਅੰਕੜੇ_LAYOUT_NHWC);
    ਵਾਪਸ layout == ਅੰਕੜੇ_LAYOUT_ਖੰਡ ? C : p[layout == ਅੰਕੜੇ_LAYOUT_NCHW ? 1 : dims-1];
}

MatShape MatShape::ਮੰਜ਼ਿਲ_Layout(ਅੰਕੜੇ_Layout newLayout, ਪੂਰਨ C0) ਅਚਲ
{
    CV_Assert(layout == ਅੰਕੜੇ_LAYOUT_ਖੰਡ || layout == ਅੰਕੜੇ_LAYOUT_NCHW || layout == ਅੰਕੜੇ_LAYOUT_NHWC);
    CV_Assert(newLayout == ਅੰਕੜੇ_LAYOUT_ਖੰਡ || newLayout == ਅੰਕੜੇ_LAYOUT_NCHW || newLayout == ਅੰਕੜੇ_LAYOUT_NHWC);

    MatShape newsize = *ਇਹ;
    newsize.layout = newLayout;

    ਜੇ (newLayout == ਅੰਕੜੇ_LAYOUT_ਖੰਡ) {
        // any => BLOCK
        CV_Assert_N(C0 > 1, (C0 & (C0-1)) == 0);
        ਪੂਰਨ Corig = channels();
        newsize.C = Corig;

        ਜੇ (layout == ਅੰਕੜੇ_LAYOUT_NHWC) {
            ਲਈ (ਪੂਰਨ i = 2; i < dims; i++) {
                newsize.p[i] = p[i-1];
            }
        }

        newsize.dims += layout != ਅੰਕੜੇ_LAYOUT_ਖੰਡ;
        newsize.p[1] = (Corig + C0 - 1)/C0;
        newsize.p[newsize.dims-1] = C0;
    } ਨਹੀਂਤਾਂ ਜੇ (layout == ਅੰਕੜੇ_LAYOUT_ਖੰਡ) {
        // BLOCK => any (except for BLOCK, which is handled above)
        newsize.C = 0;
        ਜੇ (newLayout == ਅੰਕੜੇ_LAYOUT_NHWC) {
            ਲਈ (ਪੂਰਨ i = 2; i < dims; i++) {
                newsize.p[i-1] = p[i];
            }
        }
        newsize.p[newLayout == ਅੰਕੜੇ_LAYOUT_NCHW ? 1 : dims-2] = C;
        newsize.dims--;
    } ਨਹੀਂਤਾਂ {
        CV_Assert_N(C0 <= 1);

        ਜੇ (newLayout == layout)
            ਵਾਪਸ newsize;

        // NHWC => NCHW
        ਜੇ (newLayout == ਅੰਕੜੇ_LAYOUT_NCHW) {
            ਲਈ (ਪੂਰਨ i = 2; i < dims; i++)
                newsize.p[i] = p[i-1];
            newsize.p[1] = p[dims-1];
        } ਨਹੀਂਤਾਂ {
            // NCHW => NHWC
            ਲਈ (ਪੂਰਨ i = 2; i < dims; i++)
                newsize.p[i-1] = p[i];
            newsize.p[dims-1] = p[1];
        }
    }

    ਵਾਪਸ newsize;
}

ਸਥਿਰ ਪੂਰਨ normalizeShapeAxis(ਪੂਰਨ axis, ਪੂਰਨ dims)
{
    CV_CheckGE(axis, -dims, "axis is out of range");
    CV_CheckLT(axis, dims, "axis is out of range");
    ਵਾਪਸ axis < 0 ? axis + dims : axis;
}

// The semantic NCHW form {N, C, spatial...} of a block ('NC1HWC0') shape, so that axes and
// element counts are the logical ones rather than the C0-padded ones.
ਸਥਿਰ MatShape ਮੰਜ਼ਿਲ_Semantic(ਅਚਲ MatShape& shape)
{
    CV_CheckGE(shape.dims, 3, "a block shape is {N, C1, spatial..., C0}");
    ਵਾਪਸ shape.ਮੰਜ਼ਿਲ_Layout(ਅੰਕੜੇ_LAYOUT_NCHW);
}

// Blocks such a result again. Once N or C changes, elements cross the padded C0 lanes and
// the data needs a repack, which a shape operation cannot express.
ਸਥਿਰ MatShape ਮੰਜ਼ਿਲ_ਖੰਡ_ਦਾ(MatShape ਨਤੀਜਾ, ਅਚਲ MatShape& src, ਅਚਲ ਅੱਖਰ* op)
{
    ਜੇ (ਨਤੀਜਾ.dims < 2 || ਨਤੀਜਾ.p[0] != src.p[0] || ਨਤੀਜਾ.p[1] != src.C)
        CV_Error_(Error::StsNotImplemented,
                  ("%s: the block layout is kept only when the batch and the channel axes stay "
                   "unchanged; convert the shape with toLayout() first", op));
    CV_CheckLE(ਨਤੀਜਾ.dims + 1, (ਪੂਰਨ)MatShape::MAX_DIMS, "too many dimensions");
    ਨਤੀਜਾ.layout = ਅੰਕੜੇ_LAYOUT_NCHW;
    ਵਾਪਸ ਨਤੀਜਾ.ਮੰਜ਼ਿਲ_Layout(ਅੰਕੜੇ_LAYOUT_ਖੰਡ, src.p[src.dims-1]);
}

MatShape MatShape::squeeze(ਅਚਲ std::vector<ਪੂਰਨ>& axes) ਅਚਲ
{
    CV_Assert(dims >= 0);
    ਜੇ (layout == ਅੰਕੜੇ_LAYOUT_ਖੰਡ)
        ਵਾਪਸ ਮੰਜ਼ਿਲ_ਖੰਡ_ਦਾ(ਮੰਜ਼ਿਲ_Semantic(*ਇਹ).squeeze(axes), *ਇਹ, "squeeze");
    ਤਾਰਕਿਕ remove[MAX_DIMS] = {ਝੂਠ};
    ਜੇ (axes.empty())
    {
        ਲਈ (ਪੂਰਨ i = 0; i < dims; i++)
            remove[i] = p[i] == 1;
    }
    ਲਈ (ਪੂਰਨ a : axes)
    {
        ਪੂਰਨ i = normalizeShapeAxis(a, dims);
        CV_Check(a, !remove[i], "squeeze: duplicate axis");
        CV_Check(p[i], p[i] == 1, "squeeze: the size of a squeezed axis must be 1");
        remove[i] = ਸੱਚ;
    }
    // a new number of axes: the layout of this shape does not apply
    MatShape ਨਤੀਜਾ;
    ਨਤੀਜਾ.dims = 0;
    ਲਈ (ਪੂਰਨ i = 0; i < dims; i++)
        ਜੇ (!remove[i])
            ਨਤੀਜਾ.p[ਨਤੀਜਾ.dims++] = p[i];
    ਵਾਪਸ ਨਤੀਜਾ;
}

MatShape MatShape::unsqueeze(ਅਚਲ std::vector<ਪੂਰਨ>& axes) ਅਚਲ
{
    CV_Assert(dims >= 0);
    ਜੇ (layout == ਅੰਕੜੇ_LAYOUT_ਖੰਡ)
        ਵਾਪਸ ਮੰਜ਼ਿਲ_ਖੰਡ_ਦਾ(ਮੰਜ਼ਿਲ_Semantic(*ਇਹ).unsqueeze(axes), *ਇਹ, "unsqueeze");
    ਅਚਲ ਪੂਰਨ ਬਾਹਰਲਾ_Dims = dims + (ਪੂਰਨ)axes.size();
    CV_CheckLE(ਬਾਹਰਲਾ_Dims, (ਪੂਰਨ)MAX_DIMS, "unsqueeze: too many dimensions");
    ਤਾਰਕਿਕ insert[MAX_DIMS] = {ਝੂਠ};
    ਲਈ (ਪੂਰਨ a : axes)
    {
        ਪੂਰਨ i = normalizeShapeAxis(a, ਬਾਹਰਲਾ_Dims);
        CV_Check(a, !insert[i], "unsqueeze: duplicate axis");
        insert[i] = ਸੱਚ;
    }
    // a new number of axes: the layout of this shape does not apply
    MatShape ਨਤੀਜਾ;
    ਨਤੀਜਾ.dims = ਬਾਹਰਲਾ_Dims;
    ਲਈ (ਪੂਰਨ i = 0, j = 0; i < ਬਾਹਰਲਾ_Dims; i++)
        ਨਤੀਜਾ.p[i] = insert[i] ? 1 : p[j++];
    ਵਾਪਸ ਨਤੀਜਾ;
}

MatShape MatShape::flatten(ਪੂਰਨ ਸ਼ੁਰੂ_Axis, ਪੂਰਨ ਸਿਰਾ_Axis) ਅਚਲ
{
    CV_Assert(dims >= 0);
    ਜੇ (layout == ਅੰਕੜੇ_LAYOUT_ਖੰਡ)
        ਵਾਪਸ ਮੰਜ਼ਿਲ_ਖੰਡ_ਦਾ(ਮੰਜ਼ਿਲ_Semantic(*ਇਹ).flatten(ਸ਼ੁਰੂ_Axis, ਸਿਰਾ_Axis), *ਇਹ, "flatten");
    ਜੇ (dims == 0)
    {
        CV_Check(ਸ਼ੁਰੂ_Axis, ਸ਼ੁਰੂ_Axis == 0 || ਸ਼ੁਰੂ_Axis == -1, "flatten: axis is out of range");
        CV_Check(ਸਿਰਾ_Axis, ਸਿਰਾ_Axis == 0 || ਸਿਰਾ_Axis == -1, "flatten: axis is out of range");
        ਵਾਪਸ MatShape({1});
    }
    ਪੂਰਨ ਸ਼ੁਰੂ = normalizeShapeAxis(ਸ਼ੁਰੂ_Axis, dims), end = normalizeShapeAxis(ਸਿਰਾ_Axis, dims);
    CV_CheckLE(ਸ਼ੁਰੂ, end, "flatten: startAxis must not follow endAxis");
    int64 merged = 1;
    ਲਈ (ਪੂਰਨ i = ਸ਼ੁਰੂ; i <= end; i++)
        merged *= p[i];
    CV_CheckLE((ਦੁੱਗਣਾ)merged, (ਦੁੱਗਣਾ)ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ, "flatten: the result is too big");
    // a new number of axes: the layout of this shape does not apply
    MatShape ਨਤੀਜਾ;
    ਨਤੀਜਾ.dims = 0;
    ਲਈ (ਪੂਰਨ i = 0; i < dims; i++)
    {
        ਜੇ (i == ਸ਼ੁਰੂ)
            ਨਤੀਜਾ.p[ਨਤੀਜਾ.dims++] = (ਪੂਰਨ)merged;
        ਨਹੀਂਤਾਂ ਜੇ (i < ਸ਼ੁਰੂ || i > end)
            ਨਤੀਜਾ.p[ਨਤੀਜਾ.dims++] = p[i];
    }
    ਵਾਪਸ ਨਤੀਜਾ;
}

MatShape MatShape::reshape(ਅਚਲ MatShape& newShape, ਤਾਰਕਿਕ allowZero) ਅਚਲ
{
    CV_Assert(dims >= 0 && newShape.dims >= 0);
    ਜੇ (layout == ਅੰਕੜੇ_LAYOUT_ਖੰਡ)
        ਵਾਪਸ ਮੰਜ਼ਿਲ_ਖੰਡ_ਦਾ(ਮੰਜ਼ਿਲ_Semantic(*ਇਹ).reshape(newShape, allowZero), *ਇਹ, "reshape");
    // a new number of axes: neither the layout of this shape nor the one of the spec applies
    MatShape ਨਤੀਜਾ;
    ਨਤੀਜਾ.dims = newShape.dims;
    std::copy(newShape.p, newShape.p + newShape.dims, ਨਤੀਜਾ.p);
    ਪੂਰਨ inferred = -1;
    int64 ਗਿਆਤ = 1;
    ਲਈ (ਪੂਰਨ i = 0; i < ਨਤੀਜਾ.dims; i++)
    {
        ਪੂਰਨ& sz = ਨਤੀਜਾ.p[i];
        ਜੇ (sz == -1)
        {
            CV_Check(i, inferred < 0, "reshape: at most one axis may be -1");
            inferred = i;
            ਜਾਰੀ;
        }
        CV_Check(sz, sz >= 0, "reshape: sizes must be -1 or non-negative");
        ਜੇ (sz == 0 && !allowZero)
        {
            CV_Check(i, i < dims, "reshape: 0 refers to an axis the source does not have");
            sz = p[i];
        }
        ਗਿਆਤ *= sz;
    }
    ਅਚਲ int64 ਕੁੱਲ_2 = (int64)ਕੁੱਲ();
    ਜੇ (inferred >= 0)
    {
        CV_Check((ਦੁੱਗਣਾ)ਗਿਆਤ, ਗਿਆਤ != 0, "reshape: cannot infer the -1 axis next to a zero-size axis");
        CV_Check((ਦੁੱਗਣਾ)ਕੁੱਲ_2, ਕੁੱਲ_2 % ਗਿਆਤ == 0, "reshape: the number of elements does not match");
        CV_CheckLE((ਦੁੱਗਣਾ)(ਕੁੱਲ_2/ਗਿਆਤ), (ਦੁੱਗਣਾ)ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ, "reshape: the result is too big");
        ਨਤੀਜਾ.p[inferred] = (ਪੂਰਨ)(ਕੁੱਲ_2/ਗਿਆਤ);
    }
    ਨਹੀਂਤਾਂ ਜੇ (ਗਿਆਤ != ਕੁੱਲ_2)
        CV_Error_(Error::StsBadArg, ("reshape: the number of elements does not match: %lld vs %lld",
                                     (ਲੰਮਾ ਲੰਮਾ)ਗਿਆਤ, (ਲੰਮਾ ਲੰਮਾ)ਕੁੱਲ_2));
    ਵਾਪਸ ਨਤੀਜਾ;
}

MatShape MatShape::expand(ਅਚਲ MatShape& another) ਅਚਲ
{
    ਜੇ (dims == 0)
        ਵਾਪਸ another;
    ਜੇ (another.dims == 0)
        ਵਾਪਸ *ਇਹ;

    ਜੇ ((layout == ਅੰਕੜੇ_LAYOUT_NCHW || layout == ਅੰਕੜੇ_LAYOUT_NHWC) &&
        (another.layout == ਅੰਕੜੇ_LAYOUT_NCHW || another.layout == ਅੰਕੜੇ_LAYOUT_NHWC)) {
        CV_Assert(layout == another.layout);
    }
    // [TODO] support block layout
    CV_Assert(layout != ਅੰਕੜੇ_LAYOUT_ਖੰਡ && another.layout != ਅੰਕੜੇ_LAYOUT_ਖੰਡ);

    MatShape ਨਤੀਜਾ;

    ਜੇ (dims < 0 || another.dims < 0)
        ਵਾਪਸ ਨਤੀਜਾ;

    ਨਤੀਜਾ = *ਇਹ;
    ਨਤੀਜਾ.dims = std::max(dims, another.dims);
    ਨਤੀਜਾ.layout = layout == ਅੰਕੜੇ_LAYOUT_UNKNOWN ? another.layout :
        layout == ਅੰਕੜੇ_LAYOUT_ND && (another.layout == ਅੰਕੜੇ_LAYOUT_NCHW ||
        another.layout == ਅੰਕੜੇ_LAYOUT_NHWC) ? another.layout : layout;
    ਲਈ (ਪੂਰਨ i = ਨਤੀਜਾ.dims-1; i >= 0; i--) {
        ਪੂਰਨ i1 = i - (ਨਤੀਜਾ.dims - dims);
        ਪੂਰਨ i2 = i - (ਨਤੀਜਾ.dims - another.dims);
        ਪੂਰਨ sz1 = i1 < 0 ? 1 : p[i1];
        ਪੂਰਨ sz2 = i2 < 0 ? 1 : another.p[i2];
        CV_Assert(sz1 == sz2 || sz1 == 1 || sz2 == 1);
        // [TODO] handle symbolic shapes
        ਨਤੀਜਾ.p[i] = sz1 != 1 ? sz1 : sz2;
    }
    ਵਾਪਸ ਨਤੀਜਾ;
}

/////////////////////////// MatAllocator ////////////////////////////

ਖਾਲੀ MatAllocator::map(UMat_ਅੰਕੜੇ*, AccessFlag) ਅਚਲ
{
}

ਖਾਲੀ MatAllocator::unmap(UMat_ਅੰਕੜੇ* u) ਅਚਲ
{
    ਜੇ(u->urefcount == 0 && u->refcount == 0)
    {
        deallocate(u);
    }
}

ਖਾਲੀ MatAllocator::download(UMat_ਅੰਕੜੇ* u, ਖਾਲੀ* dstptr,
         ਪੂਰਨ dims, ਅਚਲ size_t sz[],
         ਅਚਲ size_t srcofs[], ਅਚਲ size_t srcstep[],
         ਅਚਲ size_t dststep[]) ਅਚਲ
{
    ਜੇ(!u)
        ਵਾਪਸ;
    ਪੂਰਨ isz[CV_MAX_DIM];
    uchar* srcptr = u->ਅੰਕੜੇ;
    ਲਈ( ਪੂਰਨ i = 0; i < dims; i++ )
    {
        CV_Assert( sz[i] <= (size_t)ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ );
        ਜੇ( sz[i] == 0 )
            ਵਾਪਸ;
        ਜੇ( srcofs )
            srcptr += srcofs[i]*(i <= dims-2 ? srcstep[i] : 1);
        isz[i] = (ਪੂਰਨ)sz[i];
    }

    Mat src(dims, isz, CV_8U, srcptr, srcstep);
    Mat dst(dims, isz, CV_8U, dstptr, dststep);

    ਅਚਲ Mat* arrays[] = { &src, &dst };
    uchar* ptrs[2];
    NAryMatIterator it(arrays, ptrs, 2);
    size_t planesz = it.size;

    ਲਈ( size_t j = 0; j < it.nplanes; j++, ++it )
        ਸਮ੍ਰਿਤੀ_ਨਕਲ(ptrs[1], ptrs[0], planesz);
}


ਖਾਲੀ MatAllocator::upload(UMat_ਅੰਕੜੇ* u, ਅਚਲ ਖਾਲੀ* srcptr, ਪੂਰਨ dims, ਅਚਲ size_t sz[],
                    ਅਚਲ size_t dstofs[], ਅਚਲ size_t dststep[],
                    ਅਚਲ size_t srcstep[]) ਅਚਲ
{
    ਜੇ(!u)
        ਵਾਪਸ;
    ਪੂਰਨ isz[CV_MAX_DIM];
    uchar* dstptr = u->ਅੰਕੜੇ;
    ਲਈ( ਪੂਰਨ i = 0; i < dims; i++ )
    {
        CV_Assert( sz[i] <= (size_t)ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ );
        ਜੇ( sz[i] == 0 )
            ਵਾਪਸ;
        ਜੇ( dstofs )
            dstptr += dstofs[i]*(i <= dims-2 ? dststep[i] : 1);
        isz[i] = (ਪੂਰਨ)sz[i];
    }

    Mat src(dims, isz, CV_8U, (ਖਾਲੀ*)srcptr, srcstep);
    Mat dst(dims, isz, CV_8U, dstptr, dststep);

    ਅਚਲ Mat* arrays[] = { &src, &dst };
    uchar* ptrs[2];
    NAryMatIterator it(arrays, ptrs, 2);
    size_t planesz = it.size;

    ਲਈ( size_t j = 0; j < it.nplanes; j++, ++it )
        ਸਮ੍ਰਿਤੀ_ਨਕਲ(ptrs[1], ptrs[0], planesz);
}

ਖਾਲੀ MatAllocator::copy(UMat_ਅੰਕੜੇ* usrc, UMat_ਅੰਕੜੇ* udst, ਪੂਰਨ dims, ਅਚਲ size_t sz[],
                  ਅਚਲ size_t srcofs[], ਅਚਲ size_t srcstep[],
                  ਅਚਲ size_t dstofs[], ਅਚਲ size_t dststep[], ਤਾਰਕਿਕ /*sync*/) ਅਚਲ
{
    CV_INSTRUMENT_REGION();

    ਜੇ(!usrc || !udst)
        ਵਾਪਸ;
    ਪੂਰਨ isz[CV_MAX_DIM];
    uchar* srcptr = usrc->ਅੰਕੜੇ;
    uchar* dstptr = udst->ਅੰਕੜੇ;
    ਲਈ( ਪੂਰਨ i = 0; i < dims; i++ )
    {
        CV_Assert( sz[i] <= (size_t)ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ );
        ਜੇ( sz[i] == 0 )
            ਵਾਪਸ;
        ਜੇ( srcofs )
            srcptr += srcofs[i]*(i <= dims-2 ? srcstep[i] : 1);
        ਜੇ( dstofs )
            dstptr += dstofs[i]*(i <= dims-2 ? dststep[i] : 1);
        isz[i] = (ਪੂਰਨ)sz[i];
    }

    Mat src(dims, isz, CV_8U, srcptr, srcstep);
    Mat dst(dims, isz, CV_8U, dstptr, dststep);

    ਅਚਲ Mat* arrays[] = { &src, &dst };
    uchar* ptrs[2];
    NAryMatIterator it(arrays, ptrs, 2);
    size_t planesz = it.size;

    ਲਈ( size_t j = 0; j < it.nplanes; j++, ++it )
        ਸਮ੍ਰਿਤੀ_ਨਕਲ(ptrs[1], ptrs[0], planesz);
}

ਬਫਰ_Pool_Controller* MatAllocator::get_ਬਫਰ_Pool_Controller(ਅਚਲ ਅੱਖਰ* id) ਅਚਲ
{
    CV_ਅਣਵਰਤਿਆ(id);
    ਸਥਿਰ Dummy_ਬਫਰ_Pool_Controller dummy;
    ਵਾਪਸ &dummy;
}

ਵਰਗ StdMatAllocator CV_FINAL : ਜਨਤਕ MatAllocator
{
ਜਨਤਕ:
    UMat_ਅੰਕੜੇ* allocate(ਪੂਰਨ dims, ਅਚਲ ਪੂਰਨ* sizes, ਪੂਰਨ type,
                       ਖਾਲੀ* data0, size_t* ਕਦਮ, AccessFlag /*flags*/, UMatUsageFlags /*usageFlags*/) ਅਚਲ CV_OVERRIDE
    {
        size_t ਕੁੱਲ = CV_ELEM_ਮਾਪ(type);
        ਲਈ( ਪੂਰਨ i = dims-1; i >= 0; i-- )
        {
            ਜੇ( ਕਦਮ )
            {
                ਜੇ( data0 && ਕਦਮ[i] != CV_AUTOSTEP )
                {
                    CV_Assert(ਕੁੱਲ <= ਕਦਮ[i]);
                    ਕੁੱਲ = ਕਦਮ[i];
                }
                ਨਹੀਂਤਾਂ
                    ਕਦਮ[i] = ਕੁੱਲ;
            }
            ਕੁੱਲ *= sizes[i];
        }
        uchar* ਅੰਕੜੇ = data0 ? (uchar*)data0 : (uchar*)fastMalloc(ਕੁੱਲ);
        UMat_ਅੰਕੜੇ* u = ਨਵਾਂ UMat_ਅੰਕੜੇ(ਇਹ);
        u->ਅੰਕੜੇ = u->origdata = ਅੰਕੜੇ;
        u->size = ਕੁੱਲ;
        ਜੇ(data0)
            u->flags |= UMat_ਅੰਕੜੇ::USER_ALLOCATED;

        ਵਾਪਸ u;
    }

    ਤਾਰਕਿਕ allocate(UMat_ਅੰਕੜੇ* u, AccessFlag /*accessFlags*/, UMatUsageFlags /*usageFlags*/) ਅਚਲ CV_OVERRIDE
    {
        ਜੇ(!u) ਵਾਪਸ ਝੂਠ;
        ਵਾਪਸ ਸੱਚ;
    }

    ਖਾਲੀ deallocate(UMat_ਅੰਕੜੇ* u) ਅਚਲ CV_OVERRIDE
    {
        ਜੇ(!u)
            ਵਾਪਸ;

        CV_Assert(u->urefcount == 0);
        CV_Assert(u->refcount == 0);
        ਜੇ( !(u->flags & UMat_ਅੰਕੜੇ::USER_ALLOCATED) )
        {
            fastFree(u->origdata);
            u->origdata = 0;
        }
        ਮਿਟਾਓ u;
    }
};

ਸਥਿਰ
MatAllocator*& getDefaultAllocatorMatRef()
{
    ਸਥਿਰ MatAllocator* g_matAllocator = Mat::getStdAllocator();
    ਵਾਪਸ g_matAllocator;
}

MatAllocator* Mat::getDefaultAllocator()
{
    ਵਾਪਸ getDefaultAllocatorMatRef();
}

ਖਾਲੀ Mat::ਸੈੱਟ_Default_Allocator(MatAllocator* allocator)
{
    getDefaultAllocatorMatRef() = allocator;
}

MatAllocator* Mat::getStdAllocator()
{
    CV_SINGLETON_LAZY_INIT(MatAllocator, ਨਵਾਂ StdMatAllocator())
}

//==================================================================================================

ਖਾਲੀ ਸੈੱਟ_ਮਾਪ( Mat& m, ਪੂਰਨ _dims, ਅਚਲ ਪੂਰਨ* _sz, ਅਚਲ size_t* ਕਦਮਾਂ_2, ਤਾਰਕਿਕ auto_ਕਦਮਾਂ)
{
    CV_Assert( 0 <= _dims && _dims <= CV_MAX_DIM && _dims <= MatShape::MAX_DIMS);

    m.dims = _dims;
    m.size = MatShape(_dims, _sz);
    m.ਕਦਮ[std::max(_dims-1, 0)] = CV_ELEM_ਮਾਪ(m.flags);
    ਲਈ (ਪੂਰਨ i = _dims-2; i >= 0; i--) {
        size_t autostep = m.size[i+1]*m.ਕਦਮ[i+1];
        ਜੇ (ਕਦਮਾਂ_2) {
            m.ਕਦਮ[i] = ਕਦਮਾਂ_2[i];
            //CV_Assert(m.step[i] >= autostep);
        } ਨਹੀਂਤਾਂ ਜੇ (auto_ਕਦਮਾਂ) {
            m.ਕਦਮ[i] = autostep;
        } ਨਹੀਂਤਾਂ {
            m.ਕਦਮ[i] = 0;
        }
    }

    ਜੇ( _dims <= 2 )
    {
        m.ਖਾਨੇ_ਗਿਣਤੀ = _dims == 0 ? 1 : _sz ? _sz[_dims > 1] : 0;
        m.ਪੰਕਤੀਆਂ = _dims < 2 ? 1 : _sz ? _sz[0] : 0;
    } ਨਹੀਂਤਾਂ {
        m.ਖਾਨੇ_ਗਿਣਤੀ = m.ਪੰਕਤੀਆਂ = -1;
    }
}

ਪੂਰਨ updateContinuityFlag(ਪੂਰਨ flags, ਪੂਰਨ dims, ਅਚਲ ਪੂਰਨ* size, ਅਚਲ size_t* ਕਦਮ)
{
    ਪੂਰਨ i, j;
    ਜੇ (dims <= 1)
        ਵਾਪਸ flags | Mat::CONTINUOUS_FLAG;
    ਲਈ( i = 0; i < dims; i++ )
    {
        ਜੇ( size[i] > 1 )
            ਰੋਕੋ;
    }

    uint64 t = (uint64)size[std::min(i, dims-1)]*CV_MAT_CN(flags);
    ਲਈ( j = dims-1; j > i; j-- )
    {
        t *= size[j];
        ਜੇ( ਕਦਮ[j]*size[j] < ਕਦਮ[j-1] )
            ਰੋਕੋ;
    }

    ਜੇ( j <= i && t == (uint64)(ਪੂਰਨ)t )
        ਵਾਪਸ flags | Mat::CONTINUOUS_FLAG;
    ਵਾਪਸ flags & ~Mat::CONTINUOUS_FLAG;
}

ਖਾਲੀ Mat::updateContinuityFlag()
{
    flags = cv::updateContinuityFlag(flags, dims, size.p, ਕਦਮ.p);
}

ਖਾਲੀ finalizeHdr(Mat& m)
{
    m.updateContinuityFlag();
    ਪੂਰਨ d = m.dims;
    ਜੇ( d > 2 )
        m.ਪੰਕਤੀਆਂ = m.ਖਾਨੇ_ਗਿਣਤੀ = -1;
    ਜੇ(m.u)
        m.datastart = m.ਅੰਕੜੇ = m.u->ਅੰਕੜੇ;
    ਜੇ( m.ਅੰਕੜੇ && d > 0 )
    {
        m.datalimit = m.datastart + m.size[0]*m.ਕਦਮ[0];
        ਜੇ( m.size[0] > 0 )
        {
            ਪੂਰਨ lastdim = d > 0 ? d - 1 : 0;
            m.dataend = m.ptr() + m.size[lastdim]*m.ਕਦਮ[lastdim];
            ਲਈ( ਪੂਰਨ i = 0; i < d-1; i++ )
                m.dataend += (m.size[i] - 1)*m.ਕਦਮ[i];
        }
        ਨਹੀਂਤਾਂ
            m.dataend = m.datalimit;
    }
    ਨਹੀਂਤਾਂ
        m.dataend = m.datalimit = m.ਅੰਕੜੇ;
}

//======================================= Mat ======================================================

Mat::Mat() CV_NOEXCEPT
    : flags(MAGIC_VAL), dims(0), ਪੰਕਤੀਆਂ(0), ਖਾਨੇ_ਗਿਣਤੀ(0), ਅੰਕੜੇ(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{}

Mat::Mat(ਪੂਰਨ ਪੰਕਤੀਆਂ_3, ਪੂਰਨ ਖਾਨੇ_ਗਿਣਤੀ_3, ਪੂਰਨ _type)
    : flags(MAGIC_VAL), dims(0), ਪੰਕਤੀਆਂ(0), ਖਾਨੇ_ਗਿਣਤੀ(0), ਅੰਕੜੇ(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(ਪੰਕਤੀਆਂ_3, ਖਾਨੇ_ਗਿਣਤੀ_3, _type);
}

Mat::Mat(ਪੂਰਨ ਪੰਕਤੀਆਂ_3, ਪੂਰਨ ਖਾਨੇ_ਗਿਣਤੀ_3, ਪੂਰਨ _type, ਅਚਲ Scalar& _s)
    : flags(MAGIC_VAL), dims(0), ਪੰਕਤੀਆਂ(0), ਖਾਨੇ_ਗਿਣਤੀ(0), ਅੰਕੜੇ(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(ਪੰਕਤੀਆਂ_3, ਖਾਨੇ_ਗਿਣਤੀ_3, _type);
    *ਇਹ = _s;
}

Mat::Mat(ਮਾਪ _sz, ਪੂਰਨ _type)
    : flags(MAGIC_VAL), dims(0), ਪੰਕਤੀਆਂ(0), ਖਾਨੇ_ਗਿਣਤੀ(0), ਅੰਕੜੇ(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create( _sz.ਉਚਾਈ, _sz.ਚੌੜਾਈ, _type );
}

Mat::Mat(ਮਾਪ _sz, ਪੂਰਨ _type, ਅਚਲ Scalar& _s)
    : flags(MAGIC_VAL), dims(0), ਪੰਕਤੀਆਂ(0), ਖਾਨੇ_ਗਿਣਤੀ(0), ਅੰਕੜੇ(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_sz.ਉਚਾਈ, _sz.ਚੌੜਾਈ, _type);
    *ਇਹ = _s;
}

Mat::Mat(ਪੂਰਨ _dims, ਅਚਲ ਪੂਰਨ* _sz, ਪੂਰਨ _type)
    : flags(MAGIC_VAL), dims(0), ਪੰਕਤੀਆਂ(0), ਖਾਨੇ_ਗਿਣਤੀ(0), ਅੰਕੜੇ(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_dims, _sz, _type);
}

Mat::Mat(ਪੂਰਨ _dims, ਅਚਲ ਪੂਰਨ* _sz, ਪੂਰਨ _type, ਅਚਲ Scalar& _s)
    : flags(MAGIC_VAL), dims(0), ਪੰਕਤੀਆਂ(0), ਖਾਨੇ_ਗਿਣਤੀ(0), ਅੰਕੜੇ(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_dims, _sz, _type);
    *ਇਹ = _s;
}

Mat::Mat(ਅਚਲ std::vector<ਪੂਰਨ>& _sz, ਪੂਰਨ _type)
    : flags(MAGIC_VAL), dims(0), ਪੰਕਤੀਆਂ(0), ਖਾਨੇ_ਗਿਣਤੀ(0), ਅੰਕੜੇ(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_sz, _type);
}

Mat::Mat(ਅਚਲ std::vector<ਪੂਰਨ>& _sz, ਪੂਰਨ _type, ਅਚਲ Scalar& _s)
    : flags(MAGIC_VAL), dims(0), ਪੰਕਤੀਆਂ(0), ਖਾਨੇ_ਗਿਣਤੀ(0), ਅੰਕੜੇ(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_sz, _type);
    *ਇਹ = _s;
}

Mat::Mat(ਅਚਲ MatShape& _shape, ਪੂਰਨ _type)
    : flags(MAGIC_VAL), dims(0), ਪੰਕਤੀਆਂ(0), ਖਾਨੇ_ਗਿਣਤੀ(0), ਅੰਕੜੇ(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_shape, _type);
}

Mat::Mat(std::initializer_list<ਪੂਰਨ> _shape, ਪੂਰਨ _type)
    : flags(MAGIC_VAL), dims(0), ਪੰਕਤੀਆਂ(0), ਖਾਨੇ_ਗਿਣਤੀ(0), ਅੰਕੜੇ(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_shape, _type);
}

Mat::Mat(ਅਚਲ MatShape& _shape, ਪੂਰਨ _type, ਅਚਲ Scalar& _s)
    : flags(MAGIC_VAL), dims(0), ਪੰਕਤੀਆਂ(0), ਖਾਨੇ_ਗਿਣਤੀ(0), ਅੰਕੜੇ(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_shape, _type);
    *ਇਹ = _s;
}

Mat::Mat(std::initializer_list<ਪੂਰਨ> _shape, ਪੂਰਨ _type, ਅਚਲ Scalar& _s)
    : flags(MAGIC_VAL), dims(0), ਪੰਕਤੀਆਂ(0), ਖਾਨੇ_ਗਿਣਤੀ(0), ਅੰਕੜੇ(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_shape, _type);
    *ਇਹ = _s;
}

Mat::Mat(ਅਚਲ Mat& m)
    : flags(m.flags), dims(m.dims), ਪੰਕਤੀਆਂ(m.ਪੰਕਤੀਆਂ), ਖਾਨੇ_ਗਿਣਤੀ(m.ਖਾਨੇ_ਗਿਣਤੀ), ਅੰਕੜੇ(m.ਅੰਕੜੇ),
      datastart(m.datastart), dataend(m.dataend), datalimit(m.datalimit), allocator(m.allocator),
      u(m.u), size(m.size), ਕਦਮ(m.ਕਦਮ)
{
    ਜੇ( u )
        CV_XADD(&u->refcount, 1);
}

Mat::Mat(ਪੂਰਨ ਪੰਕਤੀਆਂ_3, ਪੂਰਨ ਖਾਨੇ_ਗਿਣਤੀ_3, ਪੂਰਨ _type, ਖਾਲੀ* ਅੰਕੜੇ_3, size_t ਕਦਮ_3)
    : flags(MAGIC_VAL + (_type & TYPE_ਨਕਾਬ)), dims(2), ਪੰਕਤੀਆਂ(ਪੰਕਤੀਆਂ_3), ਖਾਨੇ_ਗਿਣਤੀ(ਖਾਨੇ_ਗਿਣਤੀ_3),
      ਅੰਕੜੇ((uchar*)ਅੰਕੜੇ_3), datastart((uchar*)ਅੰਕੜੇ_3), dataend(0), datalimit(0),
      allocator(0), u(0), size(2)
{
    CV_Assert(ਕੁੱਲ() == 0 || ਅੰਕੜੇ != ਨਿਰਾ);

    size_t esz = CV_ELEM_ਮਾਪ(_type), esz1 = CV_ELEM_SIZE1(_type);
    size_t minstep = ਖਾਨੇ_ਗਿਣਤੀ * esz;
    ਜੇ( ਕਦਮ_3 == AUTO_ਕਦਮ )
    {
        ਕਦਮ_3 = minstep;
    }
    ਨਹੀਂਤਾਂ
    {
        CV_Assert( ਕਦਮ_3 >= minstep );
        ਜੇ (ਕਦਮ_3 % esz1 != 0)
        {
            CV_Error(Error::Bad_ਕਦਮ, "Step must be a multiple of esz1");
        }
    }
    size[0] = ਪੰਕਤੀਆਂ;
    size[1] = ਖਾਨੇ_ਗਿਣਤੀ;
    ਕਦਮ[0] = ਕਦਮ_3;
    ਕਦਮ[1] = esz;
    datalimit = datastart + ਕਦਮ_3 * ਪੰਕਤੀਆਂ;
    dataend = datalimit - ਕਦਮ_3 + minstep;
    updateContinuityFlag();
}

Mat::Mat(ਮਾਪ _sz, ਪੂਰਨ _type, ਖਾਲੀ* ਅੰਕੜੇ_3, size_t ਕਦਮ_3)
    : flags(MAGIC_VAL + (_type & TYPE_ਨਕਾਬ)), dims(2), ਪੰਕਤੀਆਂ(_sz.ਉਚਾਈ), ਖਾਨੇ_ਗਿਣਤੀ(_sz.ਚੌੜਾਈ),
      ਅੰਕੜੇ((uchar*)ਅੰਕੜੇ_3), datastart((uchar*)ਅੰਕੜੇ_3), dataend(0), datalimit(0),
      allocator(0), u(0), size(2)
{
    CV_Assert(ਕੁੱਲ() == 0 || ਅੰਕੜੇ != ਨਿਰਾ);

    size_t esz = CV_ELEM_ਮਾਪ(_type), esz1 = CV_ELEM_SIZE1(_type);
    size_t minstep = ਖਾਨੇ_ਗਿਣਤੀ*esz;
    ਜੇ( ਕਦਮ_3 == AUTO_ਕਦਮ || (ਕਦਮ_3 < minstep && ਪੰਕਤੀਆਂ == 1))
    {
        ਕਦਮ_3 = minstep;
    }
    ਨਹੀਂਤਾਂ
    {
        CV_CheckGE(ਕਦਮ_3, minstep, "");

        ਜੇ (ਕਦਮ_3 % esz1 != 0)
        {
            CV_Error(Error::Bad_ਕਦਮ, "Step must be a multiple of esz1");
        }
    }
    size[0] = ਪੰਕਤੀਆਂ;
    size[1] = ਖਾਨੇ_ਗਿਣਤੀ;
    ਕਦਮ[0] = ਕਦਮ_3;
    ਕਦਮ[1] = esz;
    datalimit = datastart + ਕਦਮ_3*ਪੰਕਤੀਆਂ;
    dataend = datalimit - ਕਦਮ_3 + minstep;
    updateContinuityFlag();
}


Mat::~Mat()
{
    release();
}

Mat& Mat::ਸੰਚਾਲਕ = (ਅਚਲ Mat& m)
{
    ਜੇ( ਇਹ != &m )
    {
        ਜੇ( m.u )
            CV_XADD(&m.u->refcount, 1);
        release();
        flags = m.flags;
        dims = m.dims;
        ਪੰਕਤੀਆਂ = m.ਪੰਕਤੀਆਂ;
        ਖਾਨੇ_ਗਿਣਤੀ = m.ਖਾਨੇ_ਗਿਣਤੀ;
        size = m.size;
        ਕਦਮ = m.ਕਦਮ;
        ਅੰਕੜੇ = m.ਅੰਕੜੇ;
        datastart = m.datastart;
        dataend = m.dataend;
        datalimit = m.datalimit;
        allocator = m.allocator;
        u = m.u;
    }
    ਵਾਪਸ *ਇਹ;
}

Mat Mat::clone() ਅਚਲ
{
    Mat m;
    copy_ਮੰਜ਼ਿਲ(m);
    ਵਾਪਸ m;
}

ਖਾਲੀ Mat::ਸੌਂਪੋ_ਮੰਜ਼ਿਲ( Mat& m, ਪੂਰਨ _type ) ਅਚਲ
{
    ਜੇ( _type < 0 )
        m = *ਇਹ;
    ਨਹੀਂਤਾਂ
        convert_ਮੰਜ਼ਿਲ(m, _type);
}

ਖਾਲੀ Mat::create(ਪੂਰਨ ਪੰਕਤੀਆਂ_3, ਪੂਰਨ ਖਾਨੇ_ਗਿਣਤੀ_3, ਪੂਰਨ _type)
{
    _type &= TYPE_ਨਕਾਬ;
    ਜੇ( dims <= 2 && ਪੰਕਤੀਆਂ == ਪੰਕਤੀਆਂ_3 && ਖਾਨੇ_ਗਿਣਤੀ == ਖਾਨੇ_ਗਿਣਤੀ_3 && type() == _type && ਅੰਕੜੇ )
        ਵਾਪਸ;
    ਪੂਰਨ sz[] = {ਪੰਕਤੀਆਂ_3, ਖਾਨੇ_ਗਿਣਤੀ_3};
    create(2, sz, _type);
}

ਖਾਲੀ Mat::create(ਮਾਪ _sz, ਪੂਰਨ _type)
{
    create(_sz.ਉਚਾਈ, _sz.ਚੌੜਾਈ, _type);
}

ਖਾਲੀ Mat::create_Same_ਮਾਪ(ਨਿਵੇਸ਼_Array m, ਪੂਰਨ type)
{
    MatShape msize = m.shape();
    create(msize, type);
}

ਖਾਲੀ Mat::fit(ਪੂਰਨ _dims, ਅਚਲ ਪੂਰਨ* _sizes, ਪੂਰਨ _type)
{
    size_t old_ਕੁੱਲ_ਬਾਈਟਾਂ = u ? u->size : 0;
    size_t esz = CV_ELEM_ਮਾਪ(_type), new_ਕੁੱਲ = _dims >= 0;
    ਲਈ (ਪੂਰਨ i = 0; i < _dims; i++)
        new_ਕੁੱਲ *= _sizes[i];
    size_t new_ਕੁੱਲ_ਬਾਈਟਾਂ = new_ਕੁੱਲ*esz;
    ਜੇ (new_ਕੁੱਲ_ਬਾਈਟਾਂ > 0 && (!isContinuous() ||
                              new_ਕੁੱਲ_ਬਾਈਟਾਂ > old_ਕੁੱਲ_ਬਾਈਟਾਂ ||
                              ਅੰਕੜੇ != datastart)) {
        create(_dims, _sizes, _type);
    } ਨਹੀਂਤਾਂ {
        flags = (flags & ~Mat::TYPE_ਨਕਾਬ) | CV_MAT_TYPE(_type);
        ਪੂਰਨ dummy_ਮਾਪ = 0;
        ਸੈੱਟ_ਮਾਪ(*ਇਹ, (_dims >= 0 ? _dims : 1), (_dims >= 0 ? _sizes : &dummy_ਮਾਪ), ਖਾਲੀ_ਸੂਚਕ, ਸੱਚ);
        finalizeHdr(*ਇਹ);
    }
}

ਖਾਲੀ Mat::fit(ਅਚਲ std::vector<ਪੂਰਨ>& _shape, ਪੂਰਨ _type)
{
    fit((ਪੂਰਨ)_shape.size(), _shape.ਅੰਕੜੇ(), _type);
}

ਖਾਲੀ Mat::fit(ਅਚਲ MatShape& _shape, ਪੂਰਨ _type)
{
    fit(_shape.dims, _shape.p, _type);
    size.layout = _shape.layout;
    size.C = _shape.C;
}

ਖਾਲੀ Mat::fit(std::initializer_list<ਪੂਰਨ> _shape, ਪੂਰਨ _type)
{
    ਪੂਰਨ new_shape[MatShape::MAX_DIMS];
    ਪੂਰਨ new_ndims = (ਪੂਰਨ)_shape.size();
    CV_Assert(new_ndims <= MatShape::MAX_DIMS);
    ਸਵੈਚਾਲਿਤ it = _shape.begin();
    ਲਈ (ਪੂਰਨ i = 0; i < new_ndims; i++, ++it)
        new_shape[i] = *it;
    fit(new_ndims, new_shape, _type);
}

ਖਾਲੀ Mat::fit(ਪੂਰਨ ਪੰਕਤੀਆਂ_3, ਪੂਰਨ ਖਾਨੇ_ਗਿਣਤੀ_3, ਪੂਰਨ _type)
{
    _type &= TYPE_ਨਕਾਬ;
    ਪੂਰਨ sz[] = {ਪੰਕਤੀਆਂ_3, ਖਾਨੇ_ਗਿਣਤੀ_3};
    fit(2, sz, _type);
}

ਖਾਲੀ Mat::fit(ਮਾਪ _sz, ਪੂਰਨ _type)
{
    fit(_sz.ਉਚਾਈ, _sz.ਚੌੜਾਈ, _type);
}

ਖਾਲੀ Mat::fit_Same_ਮਾਪ(ਨਿਵੇਸ਼_Array m, ਪੂਰਨ _type)
{
    fit(m.shape(), _type);
}

ਖਾਲੀ Mat::addref()
{
    ਜੇ( u )
        CV_XADD(&u->refcount, 1);
}

ਖਾਲੀ Mat::release()
{
    ਜੇ( u && CV_XADD(&u->refcount, -1) == 1 )
        deallocate();
    u = ਨਿਰਾ;
    datastart = dataend = datalimit = ਅੰਕੜੇ = 0;
    ਲਈ(ਪੂਰਨ i = 0; i < dims; i++)
        size.p[i] = 0;
    flags = (flags & CV_MAT_TYPE_ਨਕਾਬ) | MAGIC_VAL;
    dims = ਪੰਕਤੀਆਂ = ਖਾਨੇ_ਗਿਣਤੀ = 0;
    size.clear();
}

size_t Mat::step1(ਪੂਰਨ i) ਅਚਲ
{
    ਵਾਪਸ ਕਦਮ.p[i] / elemSize1();
}

ਤਾਰਕਿਕ Mat::empty() ਅਚਲ
{
    ਵਾਪਸ ਅੰਕੜੇ == 0 || ਕੁੱਲ() == 0;
}

size_t Mat::ਕੁੱਲ() ਅਚਲ
{
    ਜੇ( dims <= 2 )
        ਵਾਪਸ (size_t)ਪੰਕਤੀਆਂ * ਖਾਨੇ_ਗਿਣਤੀ;
    size_t p = 1;
    ਲਈ( ਪੂਰਨ i = 0; i < dims; i++ )
        p *= size[i];
    ਵਾਪਸ p;
}

size_t Mat::ਕੁੱਲ(ਪੂਰਨ ਸ਼ੁਰੂ_Dim, ਪੂਰਨ ਸਿਰਾ_Dim) ਅਚਲ
{
    CV_Assert( 0 <= ਸ਼ੁਰੂ_Dim && ਸ਼ੁਰੂ_Dim <= ਸਿਰਾ_Dim);
    size_t p = 1;
    ਪੂਰਨ ਸਿਰਾ_Dim_2 = ਸਿਰਾ_Dim <= dims ? ਸਿਰਾ_Dim : dims;
    ਲਈ( ਪੂਰਨ i = ਸ਼ੁਰੂ_Dim; i < ਸਿਰਾ_Dim_2; i++ )
        p *= size[i];
    ਵਾਪਸ p;
}

Mat::Mat(Mat&& m) CV_NOEXCEPT
    : flags(m.flags), dims(m.dims), ਪੰਕਤੀਆਂ(m.ਪੰਕਤੀਆਂ), ਖਾਨੇ_ਗਿਣਤੀ(m.ਖਾਨੇ_ਗਿਣਤੀ), ਅੰਕੜੇ(m.ਅੰਕੜੇ),
      datastart(m.datastart), dataend(m.dataend), datalimit(m.datalimit), allocator(m.allocator),
      u(m.u), size(m.size), ਕਦਮ(m.ਕਦਮ)
{
    m.flags = MAGIC_VAL; m.dims = m.ਪੰਕਤੀਆਂ = m.ਖਾਨੇ_ਗਿਣਤੀ = 0;
    m.size.clear(); m.ਕਦਮ.clear();
    m.ਅੰਕੜੇ = ਨਿਰਾ; m.datastart = ਨਿਰਾ; m.dataend = ਨਿਰਾ; m.datalimit = ਨਿਰਾ;
    m.allocator = ਨਿਰਾ;
    m.u = ਨਿਰਾ;
}


Mat& Mat::ਸੰਚਾਲਕ = (Mat&& m)
{
    ਜੇ (ਇਹ == &m)
      ਵਾਪਸ *ਇਹ;

    release();
    flags = m.flags; dims = m.dims; ਪੰਕਤੀਆਂ = m.ਪੰਕਤੀਆਂ; ਖਾਨੇ_ਗਿਣਤੀ = m.ਖਾਨੇ_ਗਿਣਤੀ; ਅੰਕੜੇ = m.ਅੰਕੜੇ;
    size = m.size; ਕਦਮ = m.ਕਦਮ;
    datastart = m.datastart; dataend = m.dataend; datalimit = m.datalimit; allocator = m.allocator;
    u = m.u;
    m.flags = MAGIC_VAL; m.dims = m.ਪੰਕਤੀਆਂ = m.ਖਾਨੇ_ਗਿਣਤੀ = 0;
    m.size.clear(); m.ਕਦਮ.clear();
    m.ਅੰਕੜੇ = ਨਿਰਾ; m.datastart = ਨਿਰਾ; m.dataend = ਨਿਰਾ; m.datalimit = ਨਿਰਾ;
    m.allocator = ਨਿਰਾ;
    m.u = ਨਿਰਾ;
    ਵਾਪਸ *ਇਹ;
}


ਖਾਲੀ Mat::create(ਪੂਰਨ d0, ਅਚਲ ਪੂਰਨ* _sizes, ਪੂਰਨ _type)
{
    ਪੂਰਨ sz1 = 1, d = d0;
    ਪੂਰਨ i;
    ਜੇ (d == 0) {
        d = 1;
        _sizes = (ਅਚਲ ਪੂਰਨ*)&sz1;
    }
    CV_Assert(0 <= d && d <= CV_MAX_DIM && _sizes);
    _type = CV_MAT_TYPE(_type);

    ਜੇ( ਅੰਕੜੇ && d == dims && _type == type() )
    {
        ਲਈ( i = 0; i < d; i++ )
            ਜੇ( size[i] != _sizes[i] )
                ਰੋਕੋ;
        ਜੇ( i == d )
            ਵਾਪਸ;
    }

    ਪੂਰਨ _sizes_backup[CV_MAX_DIM]; // #5991
    ਜੇ (_sizes == (ਇਹ->size.p))
    {
        ਲਈ(i = 0; i < d; i++ )
            _sizes_backup[i] = _sizes[i];
        _sizes = _sizes_backup;
    }

    release();
    ਜੇ( d == 0 )
        ਵਾਪਸ;
    flags = (_type & CV_MAT_TYPE_ਨਕਾਬ) | MAGIC_VAL;
    ਸੈੱਟ_ਮਾਪ(*ਇਹ, d, _sizes, 0, ਸੱਚ);

    ਜੇ( ਕੁੱਲ() > 0 )
    {
        MatAllocator *a = allocator, *a0 = getDefaultAllocator();
        ਜੇ(!a)
            a = a0;
        ਕੋਸ਼ਿਸ਼
        {
            u = a->allocate(dims, size.p, _type, 0, ਕਦਮ.p, ACCESS_RW /* ignored */, USAGE_DEFAULT);
            CV_Assert(u != 0);
            allocator = a;
        }
        ਫੜੋ (...)
        {
            ਜੇ (a == a0)
                ਸੁੱਟੋ;
            u = a0->allocate(dims, size.p, _type, 0, ਕਦਮ.p, ACCESS_RW /* ignored */, USAGE_DEFAULT);
            CV_Assert(u != 0);
            allocator = a0;
        }
        CV_Assert( ਕਦਮ[dims-1] == (size_t)CV_ELEM_ਮਾਪ(flags) );
    }

    addref();
    finalizeHdr(*ਇਹ);
    size.dims = dims = d0;
}

ਖਾਲੀ Mat::create(ਅਚਲ std::vector<ਪੂਰਨ>& _sizes, ਪੂਰਨ _type)
{
    create((ਪੂਰਨ)_sizes.size(), _sizes.ਅੰਕੜੇ(), _type);
}

ਖਾਲੀ Mat::create(ਅਚਲ MatShape& _shape, ਪੂਰਨ _type)
{
    ਜੇ (_shape.dims < 0) {
        release();
        ਵਾਪਸ;
    }
    create(_shape.dims, _shape.p, _type);
    size.layout = _shape.layout;
    size.C = _shape.C;
}

ਖਾਲੀ Mat::create(std::initializer_list<ਪੂਰਨ> _shape, ਪੂਰਨ _type)
{
    ਪੂਰਨ new_shape[MatShape::MAX_DIMS];
    ਪੂਰਨ new_ndims = (ਪੂਰਨ)_shape.size();
    CV_Assert(new_ndims <= MatShape::MAX_DIMS);
    ਸਵੈਚਾਲਿਤ it = _shape.begin();
    ਲਈ (ਪੂਰਨ i = 0; i < new_ndims; i++, ++it)
        new_shape[i] = *it;

    create(new_ndims, new_shape, _type);
}

ਖਾਲੀ Mat::copy_ਮਾਪ(ਅਚਲ Mat& m)
{
    dims = m.dims;
    ਖਾਨੇ_ਗਿਣਤੀ = m.ਖਾਨੇ_ਗਿਣਤੀ;
    ਪੰਕਤੀਆਂ = m.ਪੰਕਤੀਆਂ;
    size = m.size;
    ਕਦਮ = m.ਕਦਮ;
}

ਖਾਲੀ Mat::deallocate()
{
    ਜੇ(u)
    {
        UMat_ਅੰਕੜੇ* u_ = u;
        u = ਨਿਰਾ;
        (u_->currAllocator ? u_->currAllocator : allocator ? allocator : getDefaultAllocator())->unmap(u_);
    }
}

Mat::Mat(ਅਚਲ Mat& m, ਅਚਲ Range& ਪੰਕਤੀ_Range_3, ਅਚਲ Range& ਖਾਨਾ_Range_3)
    : flags(MAGIC_VAL), dims(0), ਪੰਕਤੀਆਂ(0), ਖਾਨੇ_ਗਿਣਤੀ(0), ਅੰਕੜੇ(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    CV_Assert(m.dims <= MatShape::MAX_DIMS);
    CV_Assert(m.dims >= 2 ||
              (m.dims == 1 && (ਪੰਕਤੀ_Range_3 == Range::all() ||
                               ਪੰਕਤੀ_Range_3 == Range(0, m.ਪੰਕਤੀਆਂ))));
    ਜੇ( m.dims > 2 )
    {
        Range rs[MatShape::MAX_DIMS];
        rs[0] = ਪੰਕਤੀ_Range_3;
        rs[1] = ਖਾਨਾ_Range_3;
        ਲਈ( ਪੂਰਨ i = 2; i < m.dims; i++ )
            rs[i] = Range::all();
        *ਇਹ = m(rs);
        ਵਾਪਸ;
    }

    *ਇਹ = m;
    ਕੋਸ਼ਿਸ਼
    {
        ਜੇ( ਪੰਕਤੀ_Range_3 != Range::all() && ਪੰਕਤੀ_Range_3 != Range(0,ਪੰਕਤੀਆਂ) )
        {
            CV_Assert( 0 <= ਪੰਕਤੀ_Range_3.ਸ਼ੁਰੂ && ਪੰਕਤੀ_Range_3.ਸ਼ੁਰੂ <= ਪੰਕਤੀ_Range_3.end
                       && ਪੰਕਤੀ_Range_3.end <= m.ਪੰਕਤੀਆਂ );
            size[0] = ਪੰਕਤੀਆਂ = ਪੰਕਤੀ_Range_3.size();
            ਅੰਕੜੇ += ਕਦਮ*ਪੰਕਤੀ_Range_3.ਸ਼ੁਰੂ;
            flags |= SUBMATRIX_FLAG;
        }

        ਜੇ( ਖਾਨਾ_Range_3 != Range::all() && ਖਾਨਾ_Range_3 != Range(0,ਖਾਨੇ_ਗਿਣਤੀ) )
        {
            CV_Assert( 0 <= ਖਾਨਾ_Range_3.ਸ਼ੁਰੂ && ਖਾਨਾ_Range_3.ਸ਼ੁਰੂ <= ਖਾਨਾ_Range_3.end
                       && ਖਾਨਾ_Range_3.end <= m.ਖਾਨੇ_ਗਿਣਤੀ );
            size[dims > 1] = ਖਾਨੇ_ਗਿਣਤੀ = ਖਾਨਾ_Range_3.size();
            ਅੰਕੜੇ += ਖਾਨਾ_Range_3.ਸ਼ੁਰੂ*elem_ਮਾਪ();
            flags |= SUBMATRIX_FLAG;
        }
    }
    ਫੜੋ(...)
    {
        release();
        ਸੁੱਟੋ;
    }

    updateContinuityFlag();

    ਜੇ( ਪੰਕਤੀਆਂ <= 0 || ਖਾਨੇ_ਗਿਣਤੀ <= 0 )
    {
        release();
        ਪੰਕਤੀਆਂ = ਖਾਨੇ_ਗਿਣਤੀ = 0;
    }
}


Mat::Mat(ਅਚਲ Mat& m, ਅਚਲ Rect& roi)
    : flags(m.flags), dims(2), ਪੰਕਤੀਆਂ(roi.ਉਚਾਈ), ਖਾਨੇ_ਗਿਣਤੀ(roi.ਚੌੜਾਈ),
    ਅੰਕੜੇ(m.ਅੰਕੜੇ + roi.y*m.ਕਦਮ[0]),
    datastart(m.datastart), dataend(m.dataend), datalimit(m.datalimit),
    allocator(m.allocator), u(m.u), size(2)
{
    CV_Assert( m.dims <= 2 );

    size_t esz = CV_ELEM_ਮਾਪ(flags);
    ਅੰਕੜੇ += roi.x*esz;
    CV_Assert( 0 <= roi.x && 0 <= roi.ਚੌੜਾਈ && roi.x + roi.ਚੌੜਾਈ <= m.ਖਾਨੇ_ਗਿਣਤੀ &&
              0 <= roi.y && 0 <= roi.ਉਚਾਈ && roi.y + roi.ਉਚਾਈ <= m.ਪੰਕਤੀਆਂ );
    ਜੇ( roi.ਚੌੜਾਈ < m.ਖਾਨੇ_ਗਿਣਤੀ || roi.ਉਚਾਈ < m.ਪੰਕਤੀਆਂ )
        flags |= SUBMATRIX_FLAG;

    size[0] = ਪੰਕਤੀਆਂ;
    size[1] = ਖਾਨੇ_ਗਿਣਤੀ;
    ਕਦਮ[0] = m.ਕਦਮ[0];
    ਕਦਮ[1] = esz;
    updateContinuityFlag();

    addref();
    ਜੇ( ਪੰਕਤੀਆਂ <= 0 || ਖਾਨੇ_ਗਿਣਤੀ <= 0 )
    {
        ਪੰਕਤੀਆਂ = ਖਾਨੇ_ਗਿਣਤੀ = 0;
        release();
    }
}


Mat::Mat(ਪੂਰਨ _dims, ਅਚਲ ਪੂਰਨ* _sizes, ਪੂਰਨ _type, ਖਾਲੀ* ਅੰਕੜੇ_3, ਅਚਲ size_t* ਕਦਮਾਂ_2)
    : flags(MAGIC_VAL), dims(0), ਪੰਕਤੀਆਂ(0), ਖਾਨੇ_ਗਿਣਤੀ(0), ਅੰਕੜੇ(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    flags |= CV_MAT_TYPE(_type);
    datastart = ਅੰਕੜੇ = (uchar*)ਅੰਕੜੇ_3;
    ਜੇ (_dims > 0 || ਅੰਕੜੇ_3 != ਖਾਲੀ_ਸੂਚਕ)
        ਸੈੱਟ_ਮਾਪ(*ਇਹ, _dims, _sizes, ਕਦਮਾਂ_2, ਸੱਚ);
    finalizeHdr(*ਇਹ);
}


Mat::Mat(ਅਚਲ std::vector<ਪੂਰਨ>& _sizes, ਪੂਰਨ _type, ਖਾਲੀ* ਅੰਕੜੇ_3, ਅਚਲ size_t* ਕਦਮਾਂ_2)
    : flags(MAGIC_VAL), dims(0), ਪੰਕਤੀਆਂ(0), ਖਾਨੇ_ਗਿਣਤੀ(0), ਅੰਕੜੇ(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    flags |= CV_MAT_TYPE(_type);
    datastart = ਅੰਕੜੇ = (uchar*)ਅੰਕੜੇ_3;
    ਪੂਰਨ _dims = (ਪੂਰਨ)_sizes.size();
    ਜੇ (_dims > 0 || ਅੰਕੜੇ_3 != ਖਾਲੀ_ਸੂਚਕ)
        ਸੈੱਟ_ਮਾਪ(*ਇਹ, _dims, _sizes.ਅੰਕੜੇ(), ਕਦਮਾਂ_2, ਸੱਚ);
    finalizeHdr(*ਇਹ);
}

Mat::Mat(ਅਚਲ MatShape& _shape, ਪੂਰਨ _type, ਖਾਲੀ* ਅੰਕੜੇ_3, ਅਚਲ size_t* ਕਦਮਾਂ_2)
    : flags(MAGIC_VAL), dims(0), ਪੰਕਤੀਆਂ(0), ਖਾਨੇ_ਗਿਣਤੀ(0), ਅੰਕੜੇ(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    flags |= CV_MAT_TYPE(_type);
    datastart = ਅੰਕੜੇ = (uchar*)ਅੰਕੜੇ_3;
    ਜੇ (_shape.dims >= 0) {
        ਸੈੱਟ_ਮਾਪ(*ਇਹ, _shape.dims, _shape.p, ਕਦਮਾਂ_2, ਸੱਚ);
    }
    ਨਹੀਂਤਾਂ {
        CV_Assert(!ਅੰਕੜੇ);
    }
    finalizeHdr(*ਇਹ);
}

Mat::Mat(std::initializer_list<ਪੂਰਨ> _shape, ਪੂਰਨ _type, ਖਾਲੀ* ਅੰਕੜੇ_3, ਅਚਲ size_t* ਕਦਮਾਂ_2)
    : flags(MAGIC_VAL), dims(0), ਪੰਕਤੀਆਂ(0), ਖਾਨੇ_ਗਿਣਤੀ(0), ਅੰਕੜੇ(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    ਪੂਰਨ new_shape[MatShape::MAX_DIMS];
    ਪੂਰਨ _dims = (ਪੂਰਨ)_shape.size();
    CV_Assert(_dims <= MatShape::MAX_DIMS);
    ਸਵੈਚਾਲਿਤ it = _shape.begin();
    ਲਈ (ਪੂਰਨ i = 0; i < _dims; i++, ++it)
        new_shape[i] = *it;

    flags |= CV_MAT_TYPE(_type);
    datastart = ਅੰਕੜੇ = (uchar*)ਅੰਕੜੇ_3;
    ਜੇ (_dims > 0 || ਅੰਕੜੇ_3 != ਖਾਲੀ_ਸੂਚਕ)
        ਸੈੱਟ_ਮਾਪ(*ਇਹ, _dims, new_shape, ਕਦਮਾਂ_2, ਸੱਚ);
    finalizeHdr(*ਇਹ);
}

Mat::Mat(ਅਚਲ Mat& m, ਅਚਲ Range* ranges)
    : flags(MAGIC_VAL), dims(0), ਪੰਕਤੀਆਂ(0), ਖਾਨੇ_ਗਿਣਤੀ(0), ਅੰਕੜੇ(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    ਪੂਰਨ d = m.dims;

    CV_Assert(ranges);
    ਲਈ( ਪੂਰਨ i = 0; i < d; i++ )
    {
        Range r = ranges[i];
        CV_Assert( r == Range::all() || (0 <= r.ਸ਼ੁਰੂ && r.ਸ਼ੁਰੂ < r.end && r.end <= m.size[i]) );
    }
    *ਇਹ = m;
    ਲਈ( ਪੂਰਨ i = 0; i < d; i++ )
    {
        Range r = ranges[i];
        ਜੇ( r != Range::all() && r != Range(0, size.p[i]))
        {
            size.p[i] = r.end - r.ਸ਼ੁਰੂ;
            ਅੰਕੜੇ += r.ਸ਼ੁਰੂ*ਕਦਮ.p[i];
            flags |= SUBMATRIX_FLAG;
        }
    }

    ਜੇ (d <= 2) {
        ਪੰਕਤੀਆਂ = d == 2 ? size[0] : 1;
        ਖਾਨੇ_ਗਿਣਤੀ = d <= 0 ? (d >= 0) : size[d > 1];
    }
    updateContinuityFlag();
}

Mat::Mat(ਅਚਲ Mat& m, ਅਚਲ std::vector<Range>& ranges)
    : flags(MAGIC_VAL), dims(0), ਪੰਕਤੀਆਂ(0), ਖਾਨੇ_ਗਿਣਤੀ(0), ਅੰਕੜੇ(0), datastart(0), dataend(0),
    datalimit(0), allocator(0), u(0)
{
    ਪੂਰਨ d = m.dims;

    CV_Assert((ਪੂਰਨ)ranges.size() == d);
    ਲਈ (ਪੂਰਨ i = 0; i < d; i++)
    {
        Range r = ranges[i];
        CV_Assert(r == Range::all() || (0 <= r.ਸ਼ੁਰੂ && r.ਸ਼ੁਰੂ < r.end && r.end <= m.size[i]));
    }
    *ਇਹ = m;
    ਲਈ (ਪੂਰਨ i = 0; i < d; i++)
    {
        Range r = ranges[i];
        ਜੇ (r != Range::all() && r != Range(0, size.p[i]))
        {
            size.p[i] = r.end - r.ਸ਼ੁਰੂ;
            ਅੰਕੜੇ += r.ਸ਼ੁਰੂ*ਕਦਮ.p[i];
            flags |= SUBMATRIX_FLAG;
        }
    }

    ਜੇ (d <= 2) {
        ਪੰਕਤੀਆਂ = d == 2 ? size[0] : 1;
        ਖਾਨੇ_ਗਿਣਤੀ = d <= 0 ? (d >= 0) : size[d > 1];
    }
    updateContinuityFlag();
}


Mat Mat::diag(ਪੂਰਨ d) ਅਚਲ
{
    CV_Assert( dims == 2 );
    Mat m = *ਇਹ;
    size_t esz = elem_ਮਾਪ();
    ਪੂਰਨ len;

    ਜੇ( d >= 0 )
    {
        len = std::min(ਖਾਨੇ_ਗਿਣਤੀ - d, ਪੰਕਤੀਆਂ);
        m.ਅੰਕੜੇ += esz*d;
    }
    ਨਹੀਂਤਾਂ
    {
        len = std::min(ਪੰਕਤੀਆਂ + d, ਖਾਨੇ_ਗਿਣਤੀ);
        m.ਅੰਕੜੇ -= ਕਦਮ[0]*d;
    }
    CV_DbgAssert( len > 0 );

    m.size[0] = m.ਪੰਕਤੀਆਂ = len;
    m.size[1] = m.ਖਾਨੇ_ਗਿਣਤੀ = 1;
    m.ਕਦਮ[0] += (len > 1 ? esz : 0);

    m.updateContinuityFlag();

    ਜੇ( size() != ਮਾਪ(1,1) )
        m.flags |= SUBMATRIX_FLAG;

    ਵਾਪਸ m;
}


ਖਾਲੀ Mat::pop_back(size_t nelems)
{
    CV_Assert( nelems <= (size_t)size.p[0] );
    CV_Assert( dims >= 1 );

    ਜੇ( isSubmatrix() )
        *ਇਹ = ਪੰਕਤੀ_Range_2(0, size.p[0] - (ਪੂਰਨ)nelems);
    ਨਹੀਂਤਾਂ
    {
        size.p[0] -= (ਪੂਰਨ)nelems;
        ਜੇ (dims == 2)
            ਪੰਕਤੀਆਂ = size.p[0];
        ਨਹੀਂਤਾਂ ਜੇ (dims == 1)
            ਖਾਨੇ_ਗਿਣਤੀ = size.p[0];
        dataend -= nelems*ਕਦਮ.p[0];
    }
}


ਖਾਲੀ Mat::ਧੱਕੋ_ਪਿੱਠ(ਅਚਲ ਖਾਲੀ* elem)
{
    size_t r = size.p[0];
    ਜੇ( isSubmatrix() || dataend + ਕਦਮ.p[0] > datalimit )
        reserve( std::max(r + 1, (r*3+1)/2) );

    size_t esz = elem_ਮਾਪ();
    ਸਮ੍ਰਿਤੀ_ਨਕਲ(ਅੰਕੜੇ + r*ਕਦਮ.p[0], elem, esz);
    size.p[0] = ਪੂਰਨ(r + 1);
    ਜੇ (dims == 2)
        ਪੰਕਤੀਆਂ = size.p[0];
    ਨਹੀਂਤਾਂ ਜੇ (dims == 1)
        ਖਾਨੇ_ਗਿਣਤੀ = size.p[0];
    dataend += ਕਦਮ.p[0];
    uint64 tsz = size.p[0];
    ਲਈ( ਪੂਰਨ i = 1; i < dims; i++ )
        tsz *= size.p[i];
    ਜੇ( esz < ਕਦਮ.p[0] || tsz != (uint64)(ਪੂਰਨ)tsz )
        flags &= ~CONTINUOUS_FLAG;
}


ਖਾਲੀ Mat::reserve(size_t nelems)
{
    ਅਚਲ size_t MIN_ਮਾਪ = 64;

    CV_Assert( (ਪੂਰਨ)nelems >= 0 );
    ਜੇ( !isSubmatrix() && ਕਦਮ.p[0] != 0 && ਅੰਕੜੇ + ਕਦਮ.p[0]*nelems <= datalimit )
        ਵਾਪਸ;

    ਪੂਰਨ r = size.p[0];
    ਜੇ( (size_t)r >= nelems)
        ਵਾਪਸ;

    MatShape newsize = size;
    size_t esz = elem_ਮਾਪ();
    newsize.dims = std::max(newsize.dims, 1);
    ਜੇ (dims > 1 && r == 0 && ਕਦਮ.p[0] == 0) {
        ਕਦਮ.p[0] = size.p[1] * (dims == 2 ? esz : ਕਦਮ.p[1]);
    }
    newsize.p[0] = (ਪੂਰਨ)nelems;
    size_t newbytes = newsize.ਕੁੱਲ() * esz;
    ਜੇ (newbytes < MIN_ਮਾਪ) {
        newsize.p[0] = 1;
        newsize.p[0] = ਪੂਰਨ(MIN_ਮਾਪ / newsize.ਕੁੱਲ() / esz);
    }

    Mat m(newsize, type());
    ਜੇ( r > 0)
    {
        Mat mpart = m.ਪੰਕਤੀ_Range_2(0, r);
        copy_ਮੰਜ਼ਿਲ(mpart);
    }

    *ਇਹ = m;
    size.p[0] = r;

    ਜੇ (dims == 2)
        ਪੰਕਤੀਆਂ = size.p[0];
    ਨਹੀਂਤਾਂ ਜੇ (dims == 1)
        ਖਾਨੇ_ਗਿਣਤੀ = size.p[0];
    dataend = ਅੰਕੜੇ + ਕਦਮ.p[0]*r;
}


ਖਾਲੀ Mat::reserve_ਬਫਰ(size_t nbytes)
{
    size_t esz = 1;
    ਪੂਰਨ mtype = CV_8UC1;
    ਜੇ (!empty())
    {
        ਜੇ (!isSubmatrix() && ਅੰਕੜੇ + nbytes <= dataend)//Should it be datalimit?
            ਵਾਪਸ;
        esz = elem_ਮਾਪ();
        mtype = type();
    }

    size_t nelems = (nbytes - 1) / esz + 1;

#ਜੇ SIZE_MAX > UINT_MAX
    CV_Assert(nelems <= size_t(ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ)*size_t(ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ));
    ਪੂਰਨ newrows = nelems > size_t(ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ) ? nelems > 0x400*size_t(ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ) ? nelems > 0x100000 * size_t(ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ) ? nelems > 0x40000000 * size_t(ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ) ?
                  size_t(ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ) : 0x40000000 : 0x100000 : 0x400 : 1;
#ਨਹੀਂਤਾਂ
    ਪੂਰਨ newrows = nelems > size_t(ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ) ? 2 : 1;
#ਜੇ_ਅੰਤ
    ਪੂਰਨ newcols = (ਪੂਰਨ)((nelems - 1) / newrows + 1);

    create(newrows, newcols, mtype);
}


ਖਾਲੀ Mat::resize(size_t nelems)
{
    ਪੂਰਨ r = size.p[0];
    ਜੇ( r == (ਪੂਰਨ)nelems )
        ਵਾਪਸ;
    CV_Assert( (ਪੂਰਨ)nelems >= 0 );

    ਜੇ( isSubmatrix() || ਅੰਕੜੇ + ਕਦਮ.p[0]*nelems > datalimit )
        reserve(nelems);

    size.p[0] = (ਪੂਰਨ)nelems;
    ਜੇ (dims == 2)
        ਪੰਕਤੀਆਂ = size.p[0];
    ਨਹੀਂਤਾਂ ਜੇ (dims == 1)
        ਖਾਨੇ_ਗਿਣਤੀ = size.p[0];
    dataend += (size.p[0] - r)*(int64_t)ਕਦਮ.p[0];

    //updateContinuityFlag(*this);
}


ਖਾਲੀ Mat::resize(size_t nelems, ਅਚਲ Scalar& s)
{
    ਪੂਰਨ save_ਪੰਕਤੀਆਂ = size.p[0];
    resize(nelems);

    ਜੇ( size.p[0] > save_ਪੰਕਤੀਆਂ )
    {
        Mat part = ਪੰਕਤੀ_Range_2(save_ਪੰਕਤੀਆਂ, size.p[0]);
        part = s;
    }
}

ਖਾਲੀ Mat::push_back(ਅਚਲ Mat& elems)
{
    size_t r = size.p[0];
    size_t delta = elems.size.p[0];
    ਜੇ( delta == 0 )
        ਵਾਪਸ;
    ਜੇ( ਇਹ == &elems )
    {
        Mat tmp = elems;
        push_back(tmp);
        ਵਾਪਸ;
    }
    ਜੇ( !ਅੰਕੜੇ )
    {
        *ਇਹ = elems.clone();
        ਵਾਪਸ;
    }
    CV_Assert(dims > 0);

    size.p[0] = elems.size.p[0];
    ਤਾਰਕਿਕ eq = size == elems.size;
    size.p[0] = ਪੂਰਨ(r);
    ਜੇ( !eq )
        CV_Error(cv::Error::StsUnmatchedSizes, "Pushed vector length is not equal to matrix row length");
    ਜੇ( type() != elems.type() )
        CV_Error(cv::Error::StsUnmatchedFormats, "Pushed vector type is not the same as matrix type");
    size_t esz = elem_ਮਾਪ();
    size_t minstep = dims <= 1 ? esz : size.p[1] * (dims == 2 ? esz : ਕਦਮ.p[1]);
    size_t step0 = ਕਦਮ.p[0];
    ਜੇ (step0 < minstep) {
        ਜੇ (size.p[0] <= 1)
            ਕਦਮ.p[0] = step0 = minstep;
    }

    ਜੇ( isSubmatrix() || dataend + step0*delta > datalimit )
        reserve( std::max(r + delta, (r*3+1)/2) );

    size.p[0] += ਪੂਰਨ(delta);
    ਜੇ (dims == 2)
        ਪੰਕਤੀਆਂ = size.p[0];
    ਨਹੀਂਤਾਂ ਜੇ (dims == 1)
        ਖਾਨੇ_ਗਿਣਤੀ = size.p[0];
    dataend += step0*delta;

    //updateContinuityFlag(*this);

    ਜੇ( isContinuous() && elems.isContinuous() )
        ਸਮ੍ਰਿਤੀ_ਨਕਲ(ਅੰਕੜੇ + r*step0, elems.ਅੰਕੜੇ, elems.ਕੁੱਲ()*elems.elem_ਮਾਪ());
    ਨਹੀਂਤਾਂ
    {
        Mat part = ਪੰਕਤੀ_Range_2(ਪੂਰਨ(r), ਪੂਰਨ(r + delta));
        elems.copy_ਮੰਜ਼ਿਲ(part);
    }
}


ਖਾਲੀ Mat::locateROI( ਮਾਪ& ਸਾਰਾ_ਮਾਪ, ਬਿੰਦੂ& ofs ) ਅਚਲ
{
    CV_Assert( dims <= 2 && ਕਦਮ[0] > 0 );
    size_t esz = elem_ਮਾਪ(), minstep;
    ptrdiff_t delta1 = ਅੰਕੜੇ - datastart, delta2 = dataend - datastart;

    ਜੇ( delta1 == 0 )
        ofs.x = ofs.y = 0;
    ਨਹੀਂਤਾਂ
    {
        ofs.y = (ਪੂਰਨ)(delta1/ਕਦਮ[0]);
        ofs.x = (ਪੂਰਨ)((delta1 - ਕਦਮ[0]*ofs.y)/esz);
        CV_DbgAssert( ਅੰਕੜੇ == datastart + ofs.y*ਕਦਮ[0] + ofs.x*esz );
    }
    minstep = (ofs.x + ਖਾਨੇ_ਗਿਣਤੀ)*esz;
    ਸਾਰਾ_ਮਾਪ.ਉਚਾਈ = (ਪੂਰਨ)((delta2 - minstep)/ਕਦਮ[0] + 1);
    ਸਾਰਾ_ਮਾਪ.ਉਚਾਈ = std::max(ਸਾਰਾ_ਮਾਪ.ਉਚਾਈ, ofs.y + ਪੰਕਤੀਆਂ);
    ਸਾਰਾ_ਮਾਪ.ਚੌੜਾਈ = (ਪੂਰਨ)((delta2 - ਕਦਮ*(ਸਾਰਾ_ਮਾਪ.ਉਚਾਈ-1))/esz);
    ਸਾਰਾ_ਮਾਪ.ਚੌੜਾਈ = std::max(ਸਾਰਾ_ਮਾਪ.ਚੌੜਾਈ, ofs.x + ਖਾਨੇ_ਗਿਣਤੀ);
}

Mat& Mat::adjustROI( ਪੂਰਨ dtop, ਪੂਰਨ dbottom, ਪੂਰਨ dleft, ਪੂਰਨ dright )
{
    CV_Assert( dims <= 2 && ਕਦਮ[0] > 0 );
    ਮਾਪ ਸਾਰਾ_ਮਾਪ; ਬਿੰਦੂ ofs;
    size_t esz = elem_ਮਾਪ();
    locateROI( ਸਾਰਾ_ਮਾਪ, ofs );
    ਪੂਰਨ row1 = std::min(std::max(ofs.y - dtop, 0), ਸਾਰਾ_ਮਾਪ.ਉਚਾਈ), row2 = std::max(0, std::min(ofs.y + ਪੰਕਤੀਆਂ + dbottom, ਸਾਰਾ_ਮਾਪ.ਉਚਾਈ));
    ਪੂਰਨ col1 = std::min(std::max(ofs.x - dleft, 0), ਸਾਰਾ_ਮਾਪ.ਚੌੜਾਈ), col2 = std::max(0, std::min(ofs.x + ਖਾਨੇ_ਗਿਣਤੀ + dright, ਸਾਰਾ_ਮਾਪ.ਚੌੜਾਈ));
    ਜੇ(row1 > row2)
        std::swap(row1, row2);
    ਜੇ(col1 > col2)
        std::swap(col1, col2);

    ਜੇ (dims == 1) {
        ਅੰਕੜੇ += (col1 - ofs.x)*(std::ptrdiff_t)esz;
        ਖਾਨੇ_ਗਿਣਤੀ = col2 - col1;
        size.p[0] = ਖਾਨੇ_ਗਿਣਤੀ;
    } ਨਹੀਂਤਾਂ {
        ਅੰਕੜੇ += (row1 - ofs.y)*(std::ptrdiff_t)ਕਦਮ + (col1 - ofs.x)*(std::ptrdiff_t)esz;
        ਪੰਕਤੀਆਂ = row2 - row1; ਖਾਨੇ_ਗਿਣਤੀ = col2 - col1;
        size.p[0] = ਪੰਕਤੀਆਂ; size.p[1] = ਖਾਨੇ_ਗਿਣਤੀ;
        updateContinuityFlag();
    }
    ਵਾਪਸ *ਇਹ;
}

Mat Mat::reshape(ਪੂਰਨ new_cn, ਪੂਰਨ new_ਪੰਕਤੀਆਂ) ਅਚਲ
{
    ਪੂਰਨ cn = channels();
    Mat hdr = *ਇਹ;

    ਜੇ( new_cn == 0 )
        new_cn = cn;

    ਜੇ( dims > 2 )
    {
        ਜੇ( new_ਪੰਕਤੀਆਂ == 0 )
        {
            // special case: just change the number of channnels; retain the same shape,
            // except for the last, innermost dimension
            CV_Assert(size[dims-1]*cn % new_cn == 0);
            hdr.flags = (hdr.flags & ~CV_MAT_CN_ਨਕਾਬ) | ((new_cn-1) << CV_CN_ਖਿਸਕਾਓ);
            hdr.ਕਦਮ[dims-1] = CV_ELEM_ਮਾਪ(hdr.flags);
            hdr.size[dims-1] = hdr.size[dims-1]*cn / new_cn;
            ਵਾਪਸ hdr;
        }
        CV_Assert( new_ਪੰਕਤੀਆਂ > 0 );
        ਪੂਰਨ sz[] = { new_ਪੰਕਤੀਆਂ, (ਪੂਰਨ)(ਕੁੱਲ()*cn/new_ਪੰਕਤੀਆਂ) };
        ਵਾਪਸ reshape(new_cn, 2, sz);
    }

    ਪੂਰਨ ਕੁੱਲ_ਚੌੜਾਈ = ਖਾਨੇ_ਗਿਣਤੀ * cn;

    ਜੇ( (new_cn > ਕੁੱਲ_ਚੌੜਾਈ || ਕੁੱਲ_ਚੌੜਾਈ % new_cn != 0) && new_ਪੰਕਤੀਆਂ == 0 )
        new_ਪੰਕਤੀਆਂ = ਪੰਕਤੀਆਂ * ਕੁੱਲ_ਚੌੜਾਈ / new_cn;

    ਜੇ( new_ਪੰਕਤੀਆਂ != 0 && new_ਪੰਕਤੀਆਂ != ਪੰਕਤੀਆਂ )
    {
        ਪੂਰਨ ਕੁੱਲ_ਮਾਪ_2 = ਕੁੱਲ_ਚੌੜਾਈ * ਪੰਕਤੀਆਂ;
        ਜੇ( !isContinuous() )
            CV_Error( cv::Error::Bad_ਕਦਮ,
            "The matrix is not continuous, thus its number of rows can not be changed" );

        ਕੁੱਲ_ਚੌੜਾਈ = ਕੁੱਲ_ਮਾਪ_2 / new_ਪੰਕਤੀਆਂ;

        ਜੇ( ਕੁੱਲ_ਚੌੜਾਈ * new_ਪੰਕਤੀਆਂ != ਕੁੱਲ_ਮਾਪ_2 )
            CV_Error( cv::Error::StsBadArg, "The total number of matrix elements "
                                    "is not divisible by the new number of rows" );

        hdr.size[0] = hdr.ਪੰਕਤੀਆਂ = new_ਪੰਕਤੀਆਂ;
        hdr.ਕਦਮ[0] = ਕੁੱਲ_ਚੌੜਾਈ * elemSize1();
    } ਨਹੀਂਤਾਂ {
        hdr.size[0] = hdr.ਪੰਕਤੀਆਂ = ਪੰਕਤੀਆਂ;
        ਜੇ (dims <= 1)
            hdr.ਕਦਮ[0] = ਖਾਨੇ_ਗਿਣਤੀ * CV_ELEM_ਮਾਪ(flags);
    }

    ਪੂਰਨ new_ਚੌੜਾਈ = ਕੁੱਲ_ਚੌੜਾਈ / new_cn;

    ਜੇ( new_ਚੌੜਾਈ * new_cn != ਕੁੱਲ_ਚੌੜਾਈ )
        CV_Error( cv::Error::Bad_ਅੰਕ_Channels,
        "The total width is not divisible by the new number of channels" );

    hdr.size.dims = hdr.dims = 2;
    hdr.size[1] = hdr.ਖਾਨੇ_ਗਿਣਤੀ = new_ਚੌੜਾਈ;
    hdr.flags = (hdr.flags & ~CV_MAT_CN_ਨਕਾਬ) | ((new_cn-1) << CV_CN_ਖਿਸਕਾਓ);
    hdr.ਕਦਮ[1] = CV_ELEM_ਮਾਪ(hdr.flags);
    ਵਾਪਸ hdr;
}

Mat Mat::reshape(ਪੂਰਨ _cn, ਪੂਰਨ _newndims, ਅਚਲ ਪੂਰਨ* _newsz) ਅਚਲ
{
    ਜੇ(_newndims == dims)
    {
        ਜੇ(_newsz == 0)
            ਵਾਪਸ reshape(_cn);
        ਜੇ(_newndims == 2)
            ਵਾਪਸ reshape(_cn, _newsz[0]);
    }

    ਜੇ (isContinuous())
    {
        CV_Assert(_cn >= 0 && _newndims >= 0 && _newndims <= CV_MAX_DIM && (_newndims == 0 || _newsz != 0));

        ਜੇ (_cn == 0)
            _cn = ਇਹ->channels();
        ਨਹੀਂਤਾਂ
            CV_Assert(_cn <= CV_CN_MAX);

        size_t ਕੁੱਲ_elem1_ref = ਇਹ->ਕੁੱਲ() * ਇਹ->channels();
        size_t ਕੁੱਲ_elem1 = _cn;

        Auto_ਬਫਰ<ਪੂਰਨ, 4> newsz_buf( (size_t)_newndims );

        ਪੂਰਨ m1_idx = -1;

        ਲਈ (ਪੂਰਨ i = 0; i < _newndims; i++)
        {
            ਜੇ (_newsz[i] >= 0) {
                ਜੇ (_newsz[i] == 0 && i < dims)
                    newsz_buf[i] = size.p[i];
                ਨਹੀਂਤਾਂ
                    newsz_buf[i] = _newsz[i];
                ਕੁੱਲ_elem1 *= (size_t)newsz_buf[i];
            } ਨਹੀਂਤਾਂ {
                ਜੇ (m1_idx >= 0)
                    CV_Error(cv::Error::Sts_Bad_ਮਾਪ, "More than one '-1' occured in the new shape");
                m1_idx = i;
            }
        }

        ਜੇ (m1_idx >= 0) {
            ਜੇ (ਕੁੱਲ_elem1 == 0) {
                CV_Assert(ਕੁੱਲ_elem1_ref == 0);
                ਕੁੱਲ_elem1 = 1;
            }
            CV_Assert(ਕੁੱਲ_elem1_ref % ਕੁੱਲ_elem1 == 0);
            newsz_buf[m1_idx] = (ਪੂਰਨ)(ਕੁੱਲ_elem1_ref / ਕੁੱਲ_elem1);
        } ਨਹੀਂਤਾਂ ਜੇ (ਕੁੱਲ_elem1 != ਕੁੱਲ_elem1_ref) {
            CV_Error(cv::Error::StsUnmatchedSizes, "Requested and source matrices have different count of elements");
        }

        Mat hdr = *ਇਹ;
        hdr.flags = (hdr.flags & ~CV_MAT_CN_ਨਕਾਬ) | ((_cn-1) << CV_CN_ਖਿਸਕਾਓ);
        ਸੈੱਟ_ਮਾਪ(hdr, _newndims, newsz_buf.ਅੰਕੜੇ(), ਨਿਰਾ, ਸੱਚ);

        ਵਾਪਸ hdr;
    }

    CV_Error(cv::Error::StsNotImplemented, "Reshaping of n-dimensional non-continuous matrices is not supported yet");
    // TBD
}

Mat Mat::reshape(ਪੂਰਨ _cn, ਅਚਲ std::vector<ਪੂਰਨ>& _newshape) ਅਚਲ
{
    ਪੂਰਨ newdims = (ਪੂਰਨ)_newshape.size();
    ਜੇ(newdims == 0 && empty())
    {
        ਵਾਪਸ *ਇਹ;
    }

    ਵਾਪਸ reshape(_cn, newdims, newdims > 0 ? &_newshape[0] : 0);
}

Mat Mat::reshape(ਪੂਰਨ _cn, ਅਚਲ MatShape& _newshape) ਅਚਲ
{
    ਜੇ (_newshape.dims < 0) {
        ਪੂਰਨ newshape[] = {0};
        ਵਾਪਸ reshape(_cn, 1, newshape);
    }
    ਵਾਪਸ reshape(_cn, _newshape.dims, _newshape.p);
}

Mat Mat::reshape(ਪੂਰਨ _cn, std::initializer_list<ਪੂਰਨ> newshape_) ਅਚਲ
{
    ਪੂਰਨ newshape[MatShape::MAX_DIMS];
    size_t i, newshape_dims = newshape_.size();
    CV_Assert(newshape_dims <= (size_t)MatShape::MAX_DIMS);
    ਸਵੈਚਾਲਿਤ it = newshape_.begin();
    ਲਈ (i = 0; i < newshape_dims; i++, ++it)
        newshape[i] = *it;
    ਵਾਪਸ reshape(_cn, (ਪੂਰਨ)newshape_dims, newshape);
}

Mat Mat::reinterpret(ਪੂਰਨ type) ਅਚਲ
{
    type = CV_MAT_TYPE(type);
    CV_Assert(CV_ELEM_ਮਾਪ(ਇਹ->type()) == CV_ELEM_ਮਾਪ(type));
    Mat m = *ਇਹ;
    m.flags = (m.flags & ~CV_MAT_TYPE_ਨਕਾਬ) | type;
    m.updateContinuityFlag();
    ਵਾਪਸ m;
}

Mat Mat::diag(ਅਚਲ Mat& d)
{
    CV_Assert( d.ਖਾਨੇ_ਗਿਣਤੀ == 1 || d.ਪੰਕਤੀਆਂ == 1 );
    ਪੂਰਨ len = d.ਪੰਕਤੀਆਂ + d.ਖਾਨੇ_ਗਿਣਤੀ - 1;
    Mat m(len, len, d.type(), Scalar(0));
    Mat md = m.diag();
    ਜੇ( d.ਖਾਨੇ_ਗਿਣਤੀ == 1 )
        d.copy_ਮੰਜ਼ਿਲ(md);
    ਨਹੀਂਤਾਂ
        transpose(d, md);
    ਵਾਪਸ m;
}

ਪੂਰਨ Mat::check_ਸਦਿਸ਼(ਪੂਰਨ _elemChannels, ਪੂਰਨ ਡੂੰਘਾਈ_2, ਤਾਰਕਿਕ _requireContinuous) ਅਚਲ
{
    ਵਾਪਸ ਅੰਕੜੇ && (ਡੂੰਘਾਈ() == ਡੂੰਘਾਈ_2 || ਡੂੰਘਾਈ_2 <= 0) &&
        (isContinuous() || !_requireContinuous) &&
        ((dims <= 2 && (((ਪੰਕਤੀਆਂ == 1 || ਖਾਨੇ_ਗਿਣਤੀ == 1) && channels() == _elemChannels) ||
                        (ਖਾਨੇ_ਗਿਣਤੀ == _elemChannels && channels() == 1))) ||
        (dims == 3 && channels() == 1 && size.p[2] == _elemChannels && (size.p[0] == 1 || size.p[1] == 1) &&
         (isContinuous() || ਕਦਮ.p[1] == ਕਦਮ.p[2]*size.p[2])))
    ? (ਪੂਰਨ)(ਕੁੱਲ()*channels()/_elemChannels) : -1;
}


ਸਥਿਰ ਸਤਰਅੰਦਰ ਮਾਪ get_Continuous_ਮਾਪ(ਪੂਰਨ flags, ਪੂਰਨ ਖਾਨੇ_ਗਿਣਤੀ, ਪੂਰਨ ਪੰਕਤੀਆਂ, ਪੂਰਨ ਚੌੜਾਈ_Scale)
{
    int64 sz = (int64)ਖਾਨੇ_ਗਿਣਤੀ * ਪੰਕਤੀਆਂ * ਚੌੜਾਈ_Scale;
    ਤਾਰਕਿਕ has_int_ਉਛਾਲ = sz >= ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ;
    ਤਾਰਕਿਕ isContiguous = (flags & Mat::CONTINUOUS_FLAG) != 0;
    ਵਾਪਸ (isContiguous && !has_int_ਉਛਾਲ)
            ? ਮਾਪ((ਪੂਰਨ)sz, 1)
            : ਮਾਪ(ਖਾਨੇ_ਗਿਣਤੀ * ਚੌੜਾਈ_Scale, ਪੰਕਤੀਆਂ);
}

ਮਾਪ getContinuousSize2D(Mat& m1, ਪੂਰਨ ਚੌੜਾਈ_Scale)
{
    CV_CheckLE(m1.dims, 2, "");
    ਵਾਪਸ get_Continuous_ਮਾਪ(m1.flags,
                              m1.ਖਾਨੇ_ਗਿਣਤੀ, m1.ਪੰਕਤੀਆਂ, ਚੌੜਾਈ_Scale);
}
ਮਾਪ getContinuousSize2D(Mat& m1, Mat& m2, ਪੂਰਨ ਚੌੜਾਈ_Scale)
{
    CV_CheckLE(m1.dims, 2, "");
    CV_CheckLE(m2.dims, 2, "");
    ਅਚਲ ਮਾਪ sz1 = m1.size();
    ਜੇ (sz1 != m2.size())  // reshape all matrixes to the same size (#4159)
    {
        size_t ਕੁੱਲ_sz = m1.ਕੁੱਲ();
        CV_CheckEQ(ਕੁੱਲ_sz, m2.ਕੁੱਲ(), "");
        ਤਾਰਕਿਕ is_m1_ਸਦਿਸ਼ = m1.ਖਾਨੇ_ਗਿਣਤੀ == 1 || m1.ਪੰਕਤੀਆਂ == 1;
        ਤਾਰਕਿਕ is_m2_ਸਦਿਸ਼ = m2.ਖਾਨੇ_ਗਿਣਤੀ == 1 || m2.ਪੰਕਤੀਆਂ == 1;
        CV_Assert(is_m1_ਸਦਿਸ਼); CV_Assert(is_m2_ਸਦਿਸ਼);
        ਪੂਰਨ ਕੁੱਲ = (ਪੂਰਨ)ਕੁੱਲ_sz;  // vector-column
        ਤਾਰਕਿਕ isContiguous = ((m1.flags & m2.flags) & Mat::CONTINUOUS_FLAG) != 0;
        ਤਾਰਕਿਕ has_int_ਉਛਾਲ = ((int64)ਕੁੱਲ_sz * ਚੌੜਾਈ_Scale) >= ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ;
        ਜੇ (isContiguous && !has_int_ਉਛਾਲ)
            ਕੁੱਲ = 1; // vector-row
        m1 = m1.reshape(0, ਕੁੱਲ);
        m2 = m2.reshape(0, ਕੁੱਲ);
        CV_Assert(m1.ਖਾਨੇ_ਗਿਣਤੀ == m2.ਖਾਨੇ_ਗਿਣਤੀ && m1.ਪੰਕਤੀਆਂ == m2.ਪੰਕਤੀਆਂ);
        ਵਾਪਸ ਮਾਪ(m1.ਖਾਨੇ_ਗਿਣਤੀ * ਚੌੜਾਈ_Scale, m1.ਪੰਕਤੀਆਂ);
    }
    ਵਾਪਸ get_Continuous_ਮਾਪ(m1.flags & m2.flags,
                              m1.ਖਾਨੇ_ਗਿਣਤੀ, m1.ਪੰਕਤੀਆਂ, ਚੌੜਾਈ_Scale);
}

ਮਾਪ getContinuousSize2D(Mat& m1, Mat& m2, Mat& m3, ਪੂਰਨ ਚੌੜਾਈ_Scale)
{
    CV_CheckLE(m1.dims, 2, "");
    CV_CheckLE(m2.dims, 2, "");
    CV_CheckLE(m3.dims, 2, "");
    ਅਚਲ ਮਾਪ sz1 = m1.size();
    ਜੇ (sz1 != m2.size() || sz1 != m3.size())  // reshape all matrixes to the same size (#4159)
    {
        size_t ਕੁੱਲ_sz = m1.ਕੁੱਲ();
        CV_CheckEQ(ਕੁੱਲ_sz, m2.ਕੁੱਲ(), "");
        CV_CheckEQ(ਕੁੱਲ_sz, m3.ਕੁੱਲ(), "");
        ਤਾਰਕਿਕ is_m1_ਸਦਿਸ਼ = m1.ਖਾਨੇ_ਗਿਣਤੀ == 1 || m1.ਪੰਕਤੀਆਂ == 1;
        ਤਾਰਕਿਕ is_m2_ਸਦਿਸ਼ = m2.ਖਾਨੇ_ਗਿਣਤੀ == 1 || m2.ਪੰਕਤੀਆਂ == 1;
        ਤਾਰਕਿਕ is_m3_ਸਦਿਸ਼ = m3.ਖਾਨੇ_ਗਿਣਤੀ == 1 || m3.ਪੰਕਤੀਆਂ == 1;
        CV_Assert(is_m1_ਸਦਿਸ਼); CV_Assert(is_m2_ਸਦਿਸ਼); CV_Assert(is_m3_ਸਦਿਸ਼);
        ਪੂਰਨ ਕੁੱਲ = (ਪੂਰਨ)ਕੁੱਲ_sz;  // vector-column
        ਤਾਰਕਿਕ isContiguous = ((m1.flags & m2.flags & m3.flags) & Mat::CONTINUOUS_FLAG) != 0;
        ਤਾਰਕਿਕ has_int_ਉਛਾਲ = ((int64)ਕੁੱਲ_sz * ਚੌੜਾਈ_Scale) >= ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ;
        ਜੇ (isContiguous && !has_int_ਉਛਾਲ)
            ਕੁੱਲ = 1; // vector-row
        m1 = m1.reshape(0, ਕੁੱਲ);
        m2 = m2.reshape(0, ਕੁੱਲ);
        m3 = m3.reshape(0, ਕੁੱਲ);
        CV_Assert(m1.ਖਾਨੇ_ਗਿਣਤੀ == m2.ਖਾਨੇ_ਗਿਣਤੀ && m1.ਪੰਕਤੀਆਂ == m2.ਪੰਕਤੀਆਂ && m1.ਖਾਨੇ_ਗਿਣਤੀ == m3.ਖਾਨੇ_ਗਿਣਤੀ && m1.ਪੰਕਤੀਆਂ == m3.ਪੰਕਤੀਆਂ);
        ਵਾਪਸ ਮਾਪ(m1.ਖਾਨੇ_ਗਿਣਤੀ * ਚੌੜਾਈ_Scale, m1.ਪੰਕਤੀਆਂ);
    }
    ਵਾਪਸ get_Continuous_ਮਾਪ(m1.flags & m2.flags & m3.flags,
                              m1.ਖਾਨੇ_ਗਿਣਤੀ, m1.ਪੰਕਤੀਆਂ, ਚੌੜਾਈ_Scale);
}

} // cv::
