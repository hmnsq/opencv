// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#ಅನ್ತರ್ಭಾವ "precomp.hpp"
#ಅನ್ತರ್ಭಾವ "bufferpool.impl.hpp"

ನಾಮಾಕಾಶ cv {

std::string layout_ಗಮ್ಯ_String(ದತ್ತಾಂಶ_Layout layout)
{
    ಹಿಂತಿರುಗಿ
        layout == ದತ್ತಾಂಶ_LAYOUT_ND ? "ND" :
        layout == ದತ್ತಾಂಶ_LAYOUT_NCHW ? "NCHW" :
        layout == ದತ್ತಾಂಶ_LAYOUT_NHWC ? "NHWC" :
        layout == ದತ್ತಾಂಶ_LAYOUT_ಖಂಡ ? "BLOCK" :
        layout == ದತ್ತಾಂಶ_LAYOUT_NCDHW ? "NCDHW" :
        layout == ದತ್ತಾಂಶ_LAYOUT_NDHWC ? "NDHWC" :
        layout == ದತ್ತಾಂಶ_LAYOUT_PLANAR ? "PLANAR" :
        layout == ದತ್ತಾಂಶ_LAYOUT_UNKNOWN ? "Unknown" : "???";
}

ತಾರ್ಕಿಕ ಕ್ರಿಯಾಕಾರಕ == (ಸ್ಥಿರಾಂಕ MatShape& size1, ಸ್ಥಿರಾಂಕ MatShape& size2)
{
    ಆದರೆ (size1.dims != size2.dims)
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
    ಆದರೆ (size1.layout != size2.layout &&
        size1.layout != ದತ್ತಾಂಶ_LAYOUT_UNKNOWN &&
        size2.layout != ದತ್ತಾಂಶ_LAYOUT_UNKNOWN)
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
    ಆದರೆ (size1.layout == ದತ್ತಾಂಶ_LAYOUT_ಖಂಡ &&
        size2.layout == ದತ್ತಾಂಶ_LAYOUT_ಖಂಡ &&
        size1.C != size2.C)
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < size1.dims; i++) {
        ಆದರೆ (size1.p[i] != size2.p[i])
            ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
    }
    ಹಿಂತಿರುಗಿ ನಿಜ;
}

ತಾರ್ಕಿಕ ಕ್ರಿಯಾಕಾರಕ != (ಸ್ಥಿರಾಂಕ MatShape& size1, ಸ್ಥಿರಾಂಕ MatShape& size2)
{
    ಹಿಂತಿರುಗಿ !(size1 == size2);
}

/////////////////////////// MatShape ////////////////////////////////

MatShape MatShape::scalar()
{
    ಹಿಂತಿರುಗಿ MatShape(0);
}

ಶೂನ್ಯ MatShape::clear()
{
    dims = -1;
    layout = ದತ್ತಾಂಶ_LAYOUT_UNKNOWN;
    C = 0;
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < MAX_DIMS; i++)
        p[i] = 0;
}

ಶೂನ್ಯ MatShape::resize(size_t new_ಗಾತ್ರ, ಪೂರ್ಣಾಂಕ value)
{
    CV_Assert(new_ಗಾತ್ರ < (size_t)MAX_DIMS);
    ಪೂರ್ಣಾಂಕ old_dims = std::max(dims, 0);
    dims = (ಪೂರ್ಣಾಂಕ)new_ಗಾತ್ರ;
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = old_dims; i < dims; i++)
        p[i] = value;
}

ಶೂನ್ಯ MatShape::reserve(size_t)
{
    // no op; maybe need to add a check for overflow, but we check it anyway in other operations
}

ಶೂನ್ಯ MatShape::assign(size_t new_ಗಾತ್ರ, ಪೂರ್ಣಾಂಕ value)
{
    CV_Assert(new_ಗಾತ್ರ < (size_t)MAX_DIMS);
    dims = (ಪೂರ್ಣಾಂಕ)new_ಗಾತ್ರ;
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < dims; i++)
        p[i] = value;
}

ಶೂನ್ಯ MatShape::assign(ಪೂರ್ಣಾಂಕ new_ಗಾತ್ರ, ಪೂರ್ಣಾಂಕ value)
{
    assign((size_t)new_ಗಾತ್ರ, value);
}

ಶೂನ್ಯ MatShape::assign(ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* begin, ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* end)
{
    ನಿಯೋಜಿಸು(begin, end);
}

ಶೂನ್ಯ MatShape::ನಿಯೋಜಿಸು(ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* begin, ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* end)
{
    ptrdiff_t new_ಗಾತ್ರ = end - begin;
    CV_Assert(0 <= new_ಗಾತ್ರ && new_ಗಾತ್ರ < (ptrdiff_t)MAX_DIMS);
    dims = (ಪೂರ್ಣಾಂಕ)new_ಗಾತ್ರ;
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < dims; i++)
        p[i] = begin[i];
}

ಪೂರ್ಣಾಂಕ* MatShape::begin() { ಹಿಂತಿರುಗಿ p; }
ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* MatShape::begin() ಸ್ಥಿರಾಂಕ { ಹಿಂತಿರುಗಿ p; }
ಪೂರ್ಣಾಂಕ* MatShape::end() { ಹಿಂತಿರುಗಿ p + std::max(dims, 0); }
ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* MatShape::end() ಸ್ಥಿರಾಂಕ { ಹಿಂತಿರುಗಿ p + std::max(dims, 0); }
ಪೂರ್ಣಾಂಕ& MatShape::back() { ಹಿಂತಿರುಗಿ p[std::max(dims-1, 0)]; }
ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ& MatShape::back() ಸ್ಥಿರಾಂಕ { ಹಿಂತಿರುಗಿ p[std::max(dims-1, 0)]; }

ಶೂನ್ಯ MatShape::push_back(ಪೂರ್ಣಾಂಕ value)
{
    CV_Assert(dims+1 < MAX_DIMS);
    dims = std::max(dims+1, 1);
    p[dims-1] = value;
}

ಶೂನ್ಯ MatShape::emplace_back(ಪೂರ್ಣಾಂಕ value)
{
    push_back(value);
}

ಶೂನ್ಯ MatShape::insert(ಪೂರ್ಣಾಂಕ* where, ಪೂರ್ಣಾಂಕ value)
{
    ಪೂರ್ಣಾಂಕ old_dims = std::max(dims, 0);
    CV_Assert(old_dims+1 < MAX_DIMS);
    ptrdiff_t ofs = where - p;
    CV_Assert(0 <= ofs && ofs <= old_dims);
    dims = old_dims+1;
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = old_dims-1; i >= (ಪೂರ್ಣಾಂಕ)ofs; i--)
        p[i+1] = p[i];
    p[ofs] = value;
}

ಶೂನ್ಯ MatShape::insert(ಪೂರ್ಣಾಂಕ* where, size_t ಎಣಿಕೆ_3, ಪೂರ್ಣಾಂಕ value)
{
    ಪೂರ್ಣಾಂಕ old_dims = std::max(dims, 0);
    CV_Assert((size_t)(old_dims+ಎಣಿಕೆ_3) < (size_t)MAX_DIMS);
    ptrdiff_t ofs = where - p;
    CV_Assert(0 <= ofs && ofs <= old_dims);
    dims = (ಪೂರ್ಣಾಂಕ)(old_dims+ಎಣಿಕೆ_3);
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = old_dims-1; i >= (ಪೂರ್ಣಾಂಕ)ofs; i--)
        p[i+ಎಣಿಕೆ_3] = p[i];
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < (ಪೂರ್ಣಾಂಕ)ಎಣಿಕೆ_3; i++)
        p[i+ofs] = value;
}

ಶೂನ್ಯ MatShape::insert(ಪೂರ್ಣಾಂಕ* where, ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_3, ಪೂರ್ಣಾಂಕ value)
{
    insert(where, (size_t)ಎಣಿಕೆ_3, value);
}

ಶೂನ್ಯ MatShape::insert(ಪೂರ್ಣಾಂಕ* where, ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* begin, ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* end)
{
    ಸೇರಿಸು(where, begin, end);
}

ಶೂನ್ಯ MatShape::ಸೇರಿಸು(ಪೂರ್ಣಾಂಕ* where, ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* begin, ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* end)
{
    ಪೂರ್ಣಾಂಕ old_dims = std::max(dims, 0);
    ptrdiff_t delta = end - begin;
    CV_Assert(0 <= delta && old_dims+delta < MAX_DIMS);
    ptrdiff_t ofs = where - p;
    CV_Assert(0 <= ofs && ofs <= old_dims);
    dims = (ಪೂರ್ಣಾಂಕ)(old_dims+delta);
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = old_dims-1; i >= (ಪೂರ್ಣಾಂಕ)ofs; i--)
        p[i+delta] = p[i];
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < (ಪೂರ್ಣಾಂಕ)delta; i++)
        p[i+ofs] = begin[i];
}

ಶೂನ್ಯ MatShape::erase(ಪೂರ್ಣಾಂಕ* where)
{
    CV_Assert(dims > 0);
    ptrdiff_t ofs = where - p;
    CV_Assert(0 <= ofs && ofs <= dims);
    ಆದರೆ (ofs == dims)
        ಹಿಂತಿರುಗಿ;
    dims--;
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = (ಪೂರ್ಣಾಂಕ)ofs+1; i <= dims; i++)
        p[i-1] = p[i];
}

size_t MatShape::ಒಟ್ಟು() ಸ್ಥಿರಾಂಕ
{
    size_t ಫಲಿತಾಂಶ = 1;
    ಆದರೆ (dims < 0)
        ಹಿಂತಿರುಗಿ 0;
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < dims; i++)
        ಫಲಿತಾಂಶ *= p[i];
    ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}

std::vector<ಪೂರ್ಣಾಂಕ> MatShape::vec() ಸ್ಥಿರಾಂಕ
{
    ಆದರೆ (dims < 0)
        ಹಿಂತಿರುಗಿ std::vector<ಪೂರ್ಣಾಂಕ>(1, 0);
    ಹಿಂತಿರುಗಿ std::vector<ಪೂರ್ಣಾಂಕ>(p, p + dims);
}

std::string MatShape::str() ಸ್ಥಿರಾಂಕ
{
    std::stringstream sstrm;
    ಆದರೆ (empty()) {
        sstrm << "<empty>";
    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (dims == 0) {
        sstrm << "<scalar>";
    } ಇಲ್ಲದಿದ್ದರೆ {
        sstrm << "[";
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < dims; i++) {
            sstrm << (i > 0 ? " x " : "") << p[i];
        }
        sstrm << "]";
    }
    ಹಿಂತಿರುಗಿ sstrm.str();
}

ಸ್ಥಿರ ಶೂನ್ಯ finalize_ಖಂಡ_Layout(MatShape& size, ಪೂರ್ಣಾಂಕ C=0)
{
    ಆದರೆ (size.layout == ದತ್ತಾಂಶ_LAYOUT_ಖಂಡ) {
        CV_Assert(size.dims >= 4);
        ಪೂರ್ಣಾಂಕ C0 = size.p[size.dims-1];
        CV_Assert(C0 > 1 && (C0 & (C0-1)) == 0);
        size.C = C > 0 ? C : size.p[1]*size.p[size.dims-1];
    } ಇಲ್ಲದಿದ್ದರೆ {
        size.C = 0;
    }
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = std::max(size.dims, 0); i < MatShape::MAX_DIMS; i++)
        size.p[i] = 0;
    ಆದರೆ (size.dims == 0)
        size.p[0] = 1;
}

MatShape::MatShape()
{
    clear();
}

MatShape::MatShape(size_t dims_, ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* ಗಾತ್ರ_4, ದತ್ತಾಂಶ_Layout layout_, ಪೂರ್ಣಾಂಕ C_)
{
    layout = layout_;
    CV_Assert(dims_ <= (size_t)MAX_DIMS);
    dims = (ಪೂರ್ಣಾಂಕ)dims_;
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < dims; i++) {
        p[i] = ಗಾತ್ರ_4 ? ಗಾತ್ರ_4[i] : 0;
    }
    finalize_ಖಂಡ_Layout(*ಇದು, C_);
}

MatShape::MatShape(size_t dims_, ಪೂರ್ಣಾಂಕ value, ದತ್ತಾಂಶ_Layout layout_)
{
    layout = layout_;
    CV_Assert(dims_ <= (size_t)MAX_DIMS);
    dims = (ಪೂರ್ಣಾಂಕ)dims_;
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < dims; i++) {
        p[i] = value;
    }
    finalize_ಖಂಡ_Layout(*ಇದು, 0);
}

MatShape::MatShape(std::initializer_list<ಪೂರ್ಣಾಂಕ> shape)
{
    layout = ದತ್ತಾಂಶ_LAYOUT_UNKNOWN;
    CV_Assert(shape.size() <= (size_t)MAX_DIMS);
    dims = (ಪೂರ್ಣಾಂಕ)shape.size();
    ಸ್ವಯಂಚಾಲಿತ it = shape.begin();
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < dims; i++, ++it) {
        p[i] = *it;
    }
    finalize_ಖಂಡ_Layout(*ಇದು, 0);
}

MatShape::MatShape(ಪೂರ್ಣಾಂಕ dims_, ಪೂರ್ಣಾಂಕ value, ದತ್ತಾಂಶ_Layout layout_)
{
    layout = layout_;
    CV_Assert(dims_ <= MAX_DIMS);
    dims = dims_;
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < dims; i++) {
        p[i] = value;
    }
    finalize_ಖಂಡ_Layout(*ಇದು, 0);
}

MatShape::MatShape(ಸ್ಥಿರಾಂಕ std::vector<ಪೂರ್ಣಾಂಕ>& shape_, ದತ್ತಾಂಶ_Layout layout_, ಪೂರ್ಣಾಂಕ C_)
{
    layout = layout_;
    size_t shape_ಗಾತ್ರ = shape_.size();
    CV_Assert(shape_ಗಾತ್ರ < (size_t)MAX_DIMS);
    dims = (ಪೂರ್ಣಾಂಕ)shape_ಗಾತ್ರ;
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < dims; i++) {
        p[i] = shape_[i];
    }
    finalize_ಖಂಡ_Layout(*ಇದು, C_);
}

MatShape::MatShape(ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* begin, ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* end, ದತ್ತಾಂಶ_Layout layout_, ಪೂರ್ಣಾಂಕ C_)
{
    layout = layout_;
    ptrdiff_t shape_ಗಾತ್ರ = end - begin;
    CV_Assert(0 <= shape_ಗಾತ್ರ && shape_ಗಾತ್ರ < MAX_DIMS);
    dims = (ಪೂರ್ಣಾಂಕ)shape_ಗಾತ್ರ;
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < dims; i++) {
        p[i] = begin[i];
    }
    finalize_ಖಂಡ_Layout(*ಇದು, C_);
}

MatShape::MatShape(ಸ್ಥಿರಾಂಕ MatShape& shape)
{
    dims = shape.dims;
    layout = shape.layout;
    C = shape.C;
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < MAX_DIMS; i++)
        p[i] = shape.p[i];
}

MatShape& MatShape::ಕ್ರಿಯಾಕಾರಕ = (ಸ್ಥಿರಾಂಕ MatShape& shape)
{
    ಆದರೆ (ಇದು != &shape) {
        dims = shape.dims;
        layout = shape.layout;
        C = shape.C;
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < MAX_DIMS; i++)
            p[i] = shape.p[i];
    }
    ಹಿಂತಿರುಗಿ *ಇದು;
}

ತಾರ್ಕಿಕ MatShape::hasSymbols() ಸ್ಥಿರಾಂಕ
{
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < dims; i++) {
        ಆದರೆ (p[i] < 0)
            ಹಿಂತಿರುಗಿ ನಿಜ;
    }
    ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
}

ಪೂರ್ಣಾಂಕ MatShape::channels() ಸ್ಥಿರಾಂಕ
{
    CV_Assert(layout == ದತ್ತಾಂಶ_LAYOUT_ಖಂಡ || layout == ದತ್ತಾಂಶ_LAYOUT_NCHW || layout == ದತ್ತಾಂಶ_LAYOUT_NHWC);
    ಹಿಂತಿರುಗಿ layout == ದತ್ತಾಂಶ_LAYOUT_ಖಂಡ ? C : p[layout == ದತ್ತಾಂಶ_LAYOUT_NCHW ? 1 : dims-1];
}

MatShape MatShape::ಗಮ್ಯ_Layout(ದತ್ತಾಂಶ_Layout newLayout, ಪೂರ್ಣಾಂಕ C0) ಸ್ಥಿರಾಂಕ
{
    CV_Assert(layout == ದತ್ತಾಂಶ_LAYOUT_ಖಂಡ || layout == ದತ್ತಾಂಶ_LAYOUT_NCHW || layout == ದತ್ತಾಂಶ_LAYOUT_NHWC);
    CV_Assert(newLayout == ದತ್ತಾಂಶ_LAYOUT_ಖಂಡ || newLayout == ದತ್ತಾಂಶ_LAYOUT_NCHW || newLayout == ದತ್ತಾಂಶ_LAYOUT_NHWC);

    MatShape newsize = *ಇದು;
    newsize.layout = newLayout;

    ಆದರೆ (newLayout == ದತ್ತಾಂಶ_LAYOUT_ಖಂಡ) {
        // any => BLOCK
        CV_Assert_N(C0 > 1, (C0 & (C0-1)) == 0);
        ಪೂರ್ಣಾಂಕ Corig = channels();
        newsize.C = Corig;

        ಆದರೆ (layout == ದತ್ತಾಂಶ_LAYOUT_NHWC) {
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 2; i < dims; i++) {
                newsize.p[i] = p[i-1];
            }
        }

        newsize.dims += layout != ದತ್ತಾಂಶ_LAYOUT_ಖಂಡ;
        newsize.p[1] = (Corig + C0 - 1)/C0;
        newsize.p[newsize.dims-1] = C0;
    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (layout == ದತ್ತಾಂಶ_LAYOUT_ಖಂಡ) {
        // BLOCK => any (except for BLOCK, which is handled above)
        newsize.C = 0;
        ಆದರೆ (newLayout == ದತ್ತಾಂಶ_LAYOUT_NHWC) {
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 2; i < dims; i++) {
                newsize.p[i-1] = p[i];
            }
        }
        newsize.p[newLayout == ದತ್ತಾಂಶ_LAYOUT_NCHW ? 1 : dims-2] = C;
        newsize.dims--;
    } ಇಲ್ಲದಿದ್ದರೆ {
        CV_Assert_N(C0 <= 1);

        ಆದರೆ (newLayout == layout)
            ಹಿಂತಿರುಗಿ newsize;

        // NHWC => NCHW
        ಆದರೆ (newLayout == ದತ್ತಾಂಶ_LAYOUT_NCHW) {
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 2; i < dims; i++)
                newsize.p[i] = p[i-1];
            newsize.p[1] = p[dims-1];
        } ಇಲ್ಲದಿದ್ದರೆ {
            // NCHW => NHWC
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 2; i < dims; i++)
                newsize.p[i-1] = p[i];
            newsize.p[dims-1] = p[1];
        }
    }

    ಹಿಂತಿರುಗಿ newsize;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ normalizeShapeAxis(ಪೂರ್ಣಾಂಕ axis, ಪೂರ್ಣಾಂಕ dims)
{
    CV_CheckGE(axis, -dims, "axis is out of range");
    CV_CheckLT(axis, dims, "axis is out of range");
    ಹಿಂತಿರುಗಿ axis < 0 ? axis + dims : axis;
}

// The semantic NCHW form {N, C, spatial...} of a block ('NC1HWC0') shape, so that axes and
// element counts are the logical ones rather than the C0-padded ones.
ಸ್ಥಿರ MatShape ಗಮ್ಯ_Semantic(ಸ್ಥಿರಾಂಕ MatShape& shape)
{
    CV_CheckGE(shape.dims, 3, "a block shape is {N, C1, spatial..., C0}");
    ಹಿಂತಿರುಗಿ shape.ಗಮ್ಯ_Layout(ದತ್ತಾಂಶ_LAYOUT_NCHW);
}

// Blocks such a result again. Once N or C changes, elements cross the padded C0 lanes and
// the data needs a repack, which a shape operation cannot express.
ಸ್ಥಿರ MatShape ಗಮ್ಯ_ಖಂಡ_ದ(MatShape ಫಲಿತಾಂಶ, ಸ್ಥಿರಾಂಕ MatShape& src, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* op)
{
    ಆದರೆ (ಫಲಿತಾಂಶ.dims < 2 || ಫಲಿತಾಂಶ.p[0] != src.p[0] || ಫಲಿತಾಂಶ.p[1] != src.C)
        CV_Error_(Error::StsNotImplemented,
                  ("%s: the block layout is kept only when the batch and the channel axes stay "
                   "unchanged; convert the shape with toLayout() first", op));
    CV_CheckLE(ಫಲಿತಾಂಶ.dims + 1, (ಪೂರ್ಣಾಂಕ)MatShape::MAX_DIMS, "too many dimensions");
    ಫಲಿತಾಂಶ.layout = ದತ್ತಾಂಶ_LAYOUT_NCHW;
    ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ.ಗಮ್ಯ_Layout(ದತ್ತಾಂಶ_LAYOUT_ಖಂಡ, src.p[src.dims-1]);
}

MatShape MatShape::squeeze(ಸ್ಥಿರಾಂಕ std::vector<ಪೂರ್ಣಾಂಕ>& axes) ಸ್ಥಿರಾಂಕ
{
    CV_Assert(dims >= 0);
    ಆದರೆ (layout == ದತ್ತಾಂಶ_LAYOUT_ಖಂಡ)
        ಹಿಂತಿರುಗಿ ಗಮ್ಯ_ಖಂಡ_ದ(ಗಮ್ಯ_Semantic(*ಇದು).squeeze(axes), *ಇದು, "squeeze");
    ತಾರ್ಕಿಕ remove[MAX_DIMS] = {ಸುಳ್ಳು};
    ಆದರೆ (axes.empty())
    {
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < dims; i++)
            remove[i] = p[i] == 1;
    }
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ a : axes)
    {
        ಪೂರ್ಣಾಂಕ i = normalizeShapeAxis(a, dims);
        CV_Check(a, !remove[i], "squeeze: duplicate axis");
        CV_Check(p[i], p[i] == 1, "squeeze: the size of a squeezed axis must be 1");
        remove[i] = ನಿಜ;
    }
    // a new number of axes: the layout of this shape does not apply
    MatShape ಫಲಿತಾಂಶ;
    ಫಲಿತಾಂಶ.dims = 0;
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < dims; i++)
        ಆದರೆ (!remove[i])
            ಫಲಿತಾಂಶ.p[ಫಲಿತಾಂಶ.dims++] = p[i];
    ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}

MatShape MatShape::unsqueeze(ಸ್ಥಿರಾಂಕ std::vector<ಪೂರ್ಣಾಂಕ>& axes) ಸ್ಥಿರಾಂಕ
{
    CV_Assert(dims >= 0);
    ಆದರೆ (layout == ದತ್ತಾಂಶ_LAYOUT_ಖಂಡ)
        ಹಿಂತಿರುಗಿ ಗಮ್ಯ_ಖಂಡ_ದ(ಗಮ್ಯ_Semantic(*ಇದು).unsqueeze(axes), *ಇದು, "unsqueeze");
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ಹೊರಹೋಗು_Dims = dims + (ಪೂರ್ಣಾಂಕ)axes.size();
    CV_CheckLE(ಹೊರಹೋಗು_Dims, (ಪೂರ್ಣಾಂಕ)MAX_DIMS, "unsqueeze: too many dimensions");
    ತಾರ್ಕಿಕ insert[MAX_DIMS] = {ಸುಳ್ಳು};
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ a : axes)
    {
        ಪೂರ್ಣಾಂಕ i = normalizeShapeAxis(a, ಹೊರಹೋಗು_Dims);
        CV_Check(a, !insert[i], "unsqueeze: duplicate axis");
        insert[i] = ನಿಜ;
    }
    // a new number of axes: the layout of this shape does not apply
    MatShape ಫಲಿತಾಂಶ;
    ಫಲಿತಾಂಶ.dims = ಹೊರಹೋಗು_Dims;
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0, j = 0; i < ಹೊರಹೋಗು_Dims; i++)
        ಫಲಿತಾಂಶ.p[i] = insert[i] ? 1 : p[j++];
    ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}

MatShape MatShape::flatten(ಪೂರ್ಣಾಂಕ ಆರಂಭ_Axis, ಪೂರ್ಣಾಂಕ ಕೊನೆ_Axis) ಸ್ಥಿರಾಂಕ
{
    CV_Assert(dims >= 0);
    ಆದರೆ (layout == ದತ್ತಾಂಶ_LAYOUT_ಖಂಡ)
        ಹಿಂತಿರುಗಿ ಗಮ್ಯ_ಖಂಡ_ದ(ಗಮ್ಯ_Semantic(*ಇದು).flatten(ಆರಂಭ_Axis, ಕೊನೆ_Axis), *ಇದು, "flatten");
    ಆದರೆ (dims == 0)
    {
        CV_Check(ಆರಂಭ_Axis, ಆರಂಭ_Axis == 0 || ಆರಂಭ_Axis == -1, "flatten: axis is out of range");
        CV_Check(ಕೊನೆ_Axis, ಕೊನೆ_Axis == 0 || ಕೊನೆ_Axis == -1, "flatten: axis is out of range");
        ಹಿಂತಿರುಗಿ MatShape({1});
    }
    ಪೂರ್ಣಾಂಕ ಆರಂಭ = normalizeShapeAxis(ಆರಂಭ_Axis, dims), end = normalizeShapeAxis(ಕೊನೆ_Axis, dims);
    CV_CheckLE(ಆರಂಭ, end, "flatten: startAxis must not follow endAxis");
    int64 merged = 1;
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = ಆರಂಭ; i <= end; i++)
        merged *= p[i];
    CV_CheckLE((ದ್ವಿಗುಣ)merged, (ದ್ವಿಗುಣ)ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ, "flatten: the result is too big");
    // a new number of axes: the layout of this shape does not apply
    MatShape ಫಲಿತಾಂಶ;
    ಫಲಿತಾಂಶ.dims = 0;
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < dims; i++)
    {
        ಆದರೆ (i == ಆರಂಭ)
            ಫಲಿತಾಂಶ.p[ಫಲಿತಾಂಶ.dims++] = (ಪೂರ್ಣಾಂಕ)merged;
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (i < ಆರಂಭ || i > end)
            ಫಲಿತಾಂಶ.p[ಫಲಿತಾಂಶ.dims++] = p[i];
    }
    ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}

MatShape MatShape::reshape(ಸ್ಥಿರಾಂಕ MatShape& newShape, ತಾರ್ಕಿಕ allowZero) ಸ್ಥಿರಾಂಕ
{
    CV_Assert(dims >= 0 && newShape.dims >= 0);
    ಆದರೆ (layout == ದತ್ತಾಂಶ_LAYOUT_ಖಂಡ)
        ಹಿಂತಿರುಗಿ ಗಮ್ಯ_ಖಂಡ_ದ(ಗಮ್ಯ_Semantic(*ಇದು).reshape(newShape, allowZero), *ಇದು, "reshape");
    // a new number of axes: neither the layout of this shape nor the one of the spec applies
    MatShape ಫಲಿತಾಂಶ;
    ಫಲಿತಾಂಶ.dims = newShape.dims;
    std::copy(newShape.p, newShape.p + newShape.dims, ಫಲಿತಾಂಶ.p);
    ಪೂರ್ಣಾಂಕ inferred = -1;
    int64 ತಿಳಿದ = 1;
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < ಫಲಿತಾಂಶ.dims; i++)
    {
        ಪೂರ್ಣಾಂಕ& sz = ಫಲಿತಾಂಶ.p[i];
        ಆದರೆ (sz == -1)
        {
            CV_Check(i, inferred < 0, "reshape: at most one axis may be -1");
            inferred = i;
            ಮುಂದುವರಿ;
        }
        CV_Check(sz, sz >= 0, "reshape: sizes must be -1 or non-negative");
        ಆದರೆ (sz == 0 && !allowZero)
        {
            CV_Check(i, i < dims, "reshape: 0 refers to an axis the source does not have");
            sz = p[i];
        }
        ತಿಳಿದ *= sz;
    }
    ಸ್ಥಿರಾಂಕ int64 ಒಟ್ಟು_2 = (int64)ಒಟ್ಟು();
    ಆದರೆ (inferred >= 0)
    {
        CV_Check((ದ್ವಿಗುಣ)ತಿಳಿದ, ತಿಳಿದ != 0, "reshape: cannot infer the -1 axis next to a zero-size axis");
        CV_Check((ದ್ವಿಗುಣ)ಒಟ್ಟು_2, ಒಟ್ಟು_2 % ತಿಳಿದ == 0, "reshape: the number of elements does not match");
        CV_CheckLE((ದ್ವಿಗುಣ)(ಒಟ್ಟು_2/ತಿಳಿದ), (ದ್ವಿಗುಣ)ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ, "reshape: the result is too big");
        ಫಲಿತಾಂಶ.p[inferred] = (ಪೂರ್ಣಾಂಕ)(ಒಟ್ಟು_2/ತಿಳಿದ);
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ತಿಳಿದ != ಒಟ್ಟು_2)
        CV_Error_(Error::StsBadArg, ("reshape: the number of elements does not match: %lld vs %lld",
                                     (ದೀರ್ಘ ದೀರ್ಘ)ತಿಳಿದ, (ದೀರ್ಘ ದೀರ್ಘ)ಒಟ್ಟು_2));
    ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}

MatShape MatShape::expand(ಸ್ಥಿರಾಂಕ MatShape& another) ಸ್ಥಿರಾಂಕ
{
    ಆದರೆ (dims == 0)
        ಹಿಂತಿರುಗಿ another;
    ಆದರೆ (another.dims == 0)
        ಹಿಂತಿರುಗಿ *ಇದು;

    ಆದರೆ ((layout == ದತ್ತಾಂಶ_LAYOUT_NCHW || layout == ದತ್ತಾಂಶ_LAYOUT_NHWC) &&
        (another.layout == ದತ್ತಾಂಶ_LAYOUT_NCHW || another.layout == ದತ್ತಾಂಶ_LAYOUT_NHWC)) {
        CV_Assert(layout == another.layout);
    }
    // [TODO] support block layout
    CV_Assert(layout != ದತ್ತಾಂಶ_LAYOUT_ಖಂಡ && another.layout != ದತ್ತಾಂಶ_LAYOUT_ಖಂಡ);

    MatShape ಫಲಿತಾಂಶ;

    ಆದರೆ (dims < 0 || another.dims < 0)
        ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;

    ಫಲಿತಾಂಶ = *ಇದು;
    ಫಲಿತಾಂಶ.dims = std::max(dims, another.dims);
    ಫಲಿತಾಂಶ.layout = layout == ದತ್ತಾಂಶ_LAYOUT_UNKNOWN ? another.layout :
        layout == ದತ್ತಾಂಶ_LAYOUT_ND && (another.layout == ದತ್ತಾಂಶ_LAYOUT_NCHW ||
        another.layout == ದತ್ತಾಂಶ_LAYOUT_NHWC) ? another.layout : layout;
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = ಫಲಿತಾಂಶ.dims-1; i >= 0; i--) {
        ಪೂರ್ಣಾಂಕ i1 = i - (ಫಲಿತಾಂಶ.dims - dims);
        ಪೂರ್ಣಾಂಕ i2 = i - (ಫಲಿತಾಂಶ.dims - another.dims);
        ಪೂರ್ಣಾಂಕ sz1 = i1 < 0 ? 1 : p[i1];
        ಪೂರ್ಣಾಂಕ sz2 = i2 < 0 ? 1 : another.p[i2];
        CV_Assert(sz1 == sz2 || sz1 == 1 || sz2 == 1);
        // [TODO] handle symbolic shapes
        ಫಲಿತಾಂಶ.p[i] = sz1 != 1 ? sz1 : sz2;
    }
    ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}

/////////////////////////// MatAllocator ////////////////////////////

ಶೂನ್ಯ MatAllocator::map(UMat_ದತ್ತಾಂಶ*, AccessFlag) ಸ್ಥಿರಾಂಕ
{
}

ಶೂನ್ಯ MatAllocator::unmap(UMat_ದತ್ತಾಂಶ* u) ಸ್ಥಿರಾಂಕ
{
    ಆದರೆ(u->urefcount == 0 && u->refcount == 0)
    {
        deallocate(u);
    }
}

ಶೂನ್ಯ MatAllocator::download(UMat_ದತ್ತಾಂಶ* u, ಶೂನ್ಯ* dstptr,
         ಪೂರ್ಣಾಂಕ dims, ಸ್ಥಿರಾಂಕ size_t sz[],
         ಸ್ಥಿರಾಂಕ size_t srcofs[], ಸ್ಥಿರಾಂಕ size_t srcstep[],
         ಸ್ಥಿರಾಂಕ size_t dststep[]) ಸ್ಥಿರಾಂಕ
{
    ಆದರೆ(!u)
        ಹಿಂತಿರುಗಿ;
    ಪೂರ್ಣಾಂಕ isz[CV_MAX_DIM];
    uchar* srcptr = u->ದತ್ತಾಂಶ;
    ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = 0; i < dims; i++ )
    {
        CV_Assert( sz[i] <= (size_t)ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ );
        ಆದರೆ( sz[i] == 0 )
            ಹಿಂತಿರುಗಿ;
        ಆದರೆ( srcofs )
            srcptr += srcofs[i]*(i <= dims-2 ? srcstep[i] : 1);
        isz[i] = (ಪೂರ್ಣಾಂಕ)sz[i];
    }

    Mat src(dims, isz, CV_8U, srcptr, srcstep);
    Mat dst(dims, isz, CV_8U, dstptr, dststep);

    ಸ್ಥಿರಾಂಕ Mat* arrays[] = { &src, &dst };
    uchar* ptrs[2];
    NAryMatIterator it(arrays, ptrs, 2);
    size_t planesz = it.size;

    ಗಾಗಿ( size_t j = 0; j < it.nplanes; j++, ++it )
        ಸ್ಮರಣೆ_ನಕಲು(ptrs[1], ptrs[0], planesz);
}


ಶೂನ್ಯ MatAllocator::upload(UMat_ದತ್ತಾಂಶ* u, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* srcptr, ಪೂರ್ಣಾಂಕ dims, ಸ್ಥಿರಾಂಕ size_t sz[],
                    ಸ್ಥಿರಾಂಕ size_t dstofs[], ಸ್ಥಿರಾಂಕ size_t dststep[],
                    ಸ್ಥಿರಾಂಕ size_t srcstep[]) ಸ್ಥಿರಾಂಕ
{
    ಆದರೆ(!u)
        ಹಿಂತಿರುಗಿ;
    ಪೂರ್ಣಾಂಕ isz[CV_MAX_DIM];
    uchar* dstptr = u->ದತ್ತಾಂಶ;
    ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = 0; i < dims; i++ )
    {
        CV_Assert( sz[i] <= (size_t)ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ );
        ಆದರೆ( sz[i] == 0 )
            ಹಿಂತಿರುಗಿ;
        ಆದರೆ( dstofs )
            dstptr += dstofs[i]*(i <= dims-2 ? dststep[i] : 1);
        isz[i] = (ಪೂರ್ಣಾಂಕ)sz[i];
    }

    Mat src(dims, isz, CV_8U, (ಶೂನ್ಯ*)srcptr, srcstep);
    Mat dst(dims, isz, CV_8U, dstptr, dststep);

    ಸ್ಥಿರಾಂಕ Mat* arrays[] = { &src, &dst };
    uchar* ptrs[2];
    NAryMatIterator it(arrays, ptrs, 2);
    size_t planesz = it.size;

    ಗಾಗಿ( size_t j = 0; j < it.nplanes; j++, ++it )
        ಸ್ಮರಣೆ_ನಕಲು(ptrs[1], ptrs[0], planesz);
}

ಶೂನ್ಯ MatAllocator::copy(UMat_ದತ್ತಾಂಶ* usrc, UMat_ದತ್ತಾಂಶ* udst, ಪೂರ್ಣಾಂಕ dims, ಸ್ಥಿರಾಂಕ size_t sz[],
                  ಸ್ಥಿರಾಂಕ size_t srcofs[], ಸ್ಥಿರಾಂಕ size_t srcstep[],
                  ಸ್ಥಿರಾಂಕ size_t dstofs[], ಸ್ಥಿರಾಂಕ size_t dststep[], ತಾರ್ಕಿಕ /*sync*/) ಸ್ಥಿರಾಂಕ
{
    CV_INSTRUMENT_REGION();

    ಆದರೆ(!usrc || !udst)
        ಹಿಂತಿರುಗಿ;
    ಪೂರ್ಣಾಂಕ isz[CV_MAX_DIM];
    uchar* srcptr = usrc->ದತ್ತಾಂಶ;
    uchar* dstptr = udst->ದತ್ತಾಂಶ;
    ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = 0; i < dims; i++ )
    {
        CV_Assert( sz[i] <= (size_t)ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ );
        ಆದರೆ( sz[i] == 0 )
            ಹಿಂತಿರುಗಿ;
        ಆದರೆ( srcofs )
            srcptr += srcofs[i]*(i <= dims-2 ? srcstep[i] : 1);
        ಆದರೆ( dstofs )
            dstptr += dstofs[i]*(i <= dims-2 ? dststep[i] : 1);
        isz[i] = (ಪೂರ್ಣಾಂಕ)sz[i];
    }

    Mat src(dims, isz, CV_8U, srcptr, srcstep);
    Mat dst(dims, isz, CV_8U, dstptr, dststep);

    ಸ್ಥಿರಾಂಕ Mat* arrays[] = { &src, &dst };
    uchar* ptrs[2];
    NAryMatIterator it(arrays, ptrs, 2);
    size_t planesz = it.size;

    ಗಾಗಿ( size_t j = 0; j < it.nplanes; j++, ++it )
        ಸ್ಮರಣೆ_ನಕಲು(ptrs[1], ptrs[0], planesz);
}

ಬಫರ್_Pool_Controller* MatAllocator::get_ಬಫರ್_Pool_Controller(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* id) ಸ್ಥಿರಾಂಕ
{
    CV_ಬಳಸದ(id);
    ಸ್ಥಿರ Dummy_ಬಫರ್_Pool_Controller dummy;
    ಹಿಂತಿರುಗಿ &dummy;
}

ವರ್ಗ StdMatAllocator CV_FINAL : ಸಾರ್ವಜನಿಕ MatAllocator
{
ಸಾರ್ವಜನಿಕ:
    UMat_ದತ್ತಾಂಶ* allocate(ಪೂರ್ಣಾಂಕ dims, ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* sizes, ಪೂರ್ಣಾಂಕ type,
                       ಶೂನ್ಯ* data0, size_t* ಹೆಜ್ಜೆ, AccessFlag /*flags*/, UMatUsageFlags /*usageFlags*/) ಸ್ಥಿರಾಂಕ CV_OVERRIDE
    {
        size_t ಒಟ್ಟು = CV_ELEM_ಗಾತ್ರ(type);
        ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = dims-1; i >= 0; i-- )
        {
            ಆದರೆ( ಹೆಜ್ಜೆ )
            {
                ಆದರೆ( data0 && ಹೆಜ್ಜೆ[i] != CV_AUTOSTEP )
                {
                    CV_Assert(ಒಟ್ಟು <= ಹೆಜ್ಜೆ[i]);
                    ಒಟ್ಟು = ಹೆಜ್ಜೆ[i];
                }
                ಇಲ್ಲದಿದ್ದರೆ
                    ಹೆಜ್ಜೆ[i] = ಒಟ್ಟು;
            }
            ಒಟ್ಟು *= sizes[i];
        }
        uchar* ದತ್ತಾಂಶ = data0 ? (uchar*)data0 : (uchar*)fastMalloc(ಒಟ್ಟು);
        UMat_ದತ್ತಾಂಶ* u = ಹೊಸ UMat_ದತ್ತಾಂಶ(ಇದು);
        u->ದತ್ತಾಂಶ = u->origdata = ದತ್ತಾಂಶ;
        u->size = ಒಟ್ಟು;
        ಆದರೆ(data0)
            u->flags |= UMat_ದತ್ತಾಂಶ::USER_ALLOCATED;

        ಹಿಂತಿರುಗಿ u;
    }

    ತಾರ್ಕಿಕ allocate(UMat_ದತ್ತಾಂಶ* u, AccessFlag /*accessFlags*/, UMatUsageFlags /*usageFlags*/) ಸ್ಥಿರಾಂಕ CV_OVERRIDE
    {
        ಆದರೆ(!u) ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
        ಹಿಂತಿರುಗಿ ನಿಜ;
    }

    ಶೂನ್ಯ deallocate(UMat_ದತ್ತಾಂಶ* u) ಸ್ಥಿರಾಂಕ CV_OVERRIDE
    {
        ಆದರೆ(!u)
            ಹಿಂತಿರುಗಿ;

        CV_Assert(u->urefcount == 0);
        CV_Assert(u->refcount == 0);
        ಆದರೆ( !(u->flags & UMat_ದತ್ತಾಂಶ::USER_ALLOCATED) )
        {
            fastFree(u->origdata);
            u->origdata = 0;
        }
        ಅಳಿಸು u;
    }
};

ಸ್ಥಿರ
MatAllocator*& getDefaultAllocatorMatRef()
{
    ಸ್ಥಿರ MatAllocator* g_matAllocator = Mat::getStdAllocator();
    ಹಿಂತಿರುಗಿ g_matAllocator;
}

MatAllocator* Mat::getDefaultAllocator()
{
    ಹಿಂತಿರುಗಿ getDefaultAllocatorMatRef();
}

ಶೂನ್ಯ Mat::ಗಣ_Default_Allocator(MatAllocator* allocator)
{
    getDefaultAllocatorMatRef() = allocator;
}

MatAllocator* Mat::getStdAllocator()
{
    CV_SINGLETON_LAZY_INIT(MatAllocator, ಹೊಸ StdMatAllocator())
}

//==================================================================================================

ಶೂನ್ಯ ಗಣ_ಗಾತ್ರ( Mat& m, ಪೂರ್ಣಾಂಕ _dims, ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* _sz, ಸ್ಥಿರಾಂಕ size_t* ಹೆಜ್ಜೆಗಳು_2, ತಾರ್ಕಿಕ auto_ಹೆಜ್ಜೆಗಳು)
{
    CV_Assert( 0 <= _dims && _dims <= CV_MAX_DIM && _dims <= MatShape::MAX_DIMS);

    m.dims = _dims;
    m.size = MatShape(_dims, _sz);
    m.ಹೆಜ್ಜೆ[std::max(_dims-1, 0)] = CV_ELEM_ಗಾತ್ರ(m.flags);
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = _dims-2; i >= 0; i--) {
        size_t autostep = m.size[i+1]*m.ಹೆಜ್ಜೆ[i+1];
        ಆದರೆ (ಹೆಜ್ಜೆಗಳು_2) {
            m.ಹೆಜ್ಜೆ[i] = ಹೆಜ್ಜೆಗಳು_2[i];
            //CV_Assert(m.step[i] >= autostep);
        } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (auto_ಹೆಜ್ಜೆಗಳು) {
            m.ಹೆಜ್ಜೆ[i] = autostep;
        } ಇಲ್ಲದಿದ್ದರೆ {
            m.ಹೆಜ್ಜೆ[i] = 0;
        }
    }

    ಆದರೆ( _dims <= 2 )
    {
        m.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = _dims == 0 ? 1 : _sz ? _sz[_dims > 1] : 0;
        m.ಅಡ್ಡಸಾಲುಗಳು = _dims < 2 ? 1 : _sz ? _sz[0] : 0;
    } ಇಲ್ಲದಿದ್ದರೆ {
        m.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = m.ಅಡ್ಡಸಾಲುಗಳು = -1;
    }
}

ಪೂರ್ಣಾಂಕ updateContinuityFlag(ಪೂರ್ಣಾಂಕ flags, ಪೂರ್ಣಾಂಕ dims, ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* size, ಸ್ಥಿರಾಂಕ size_t* ಹೆಜ್ಜೆ)
{
    ಪೂರ್ಣಾಂಕ i, j;
    ಆದರೆ (dims <= 1)
        ಹಿಂತಿರುಗಿ flags | Mat::CONTINUOUS_FLAG;
    ಗಾಗಿ( i = 0; i < dims; i++ )
    {
        ಆದರೆ( size[i] > 1 )
            ನಿಲ್ಲಿಸು;
    }

    uint64 t = (uint64)size[std::min(i, dims-1)]*CV_MAT_CN(flags);
    ಗಾಗಿ( j = dims-1; j > i; j-- )
    {
        t *= size[j];
        ಆದರೆ( ಹೆಜ್ಜೆ[j]*size[j] < ಹೆಜ್ಜೆ[j-1] )
            ನಿಲ್ಲಿಸು;
    }

    ಆದರೆ( j <= i && t == (uint64)(ಪೂರ್ಣಾಂಕ)t )
        ಹಿಂತಿರುಗಿ flags | Mat::CONTINUOUS_FLAG;
    ಹಿಂತಿರುಗಿ flags & ~Mat::CONTINUOUS_FLAG;
}

ಶೂನ್ಯ Mat::updateContinuityFlag()
{
    flags = cv::updateContinuityFlag(flags, dims, size.p, ಹೆಜ್ಜೆ.p);
}

ಶೂನ್ಯ finalizeHdr(Mat& m)
{
    m.updateContinuityFlag();
    ಪೂರ್ಣಾಂಕ d = m.dims;
    ಆದರೆ( d > 2 )
        m.ಅಡ್ಡಸಾಲುಗಳು = m.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = -1;
    ಆದರೆ(m.u)
        m.datastart = m.ದತ್ತಾಂಶ = m.u->ದತ್ತಾಂಶ;
    ಆದರೆ( m.ದತ್ತಾಂಶ && d > 0 )
    {
        m.datalimit = m.datastart + m.size[0]*m.ಹೆಜ್ಜೆ[0];
        ಆದರೆ( m.size[0] > 0 )
        {
            ಪೂರ್ಣಾಂಕ lastdim = d > 0 ? d - 1 : 0;
            m.dataend = m.ptr() + m.size[lastdim]*m.ಹೆಜ್ಜೆ[lastdim];
            ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = 0; i < d-1; i++ )
                m.dataend += (m.size[i] - 1)*m.ಹೆಜ್ಜೆ[i];
        }
        ಇಲ್ಲದಿದ್ದರೆ
            m.dataend = m.datalimit;
    }
    ಇಲ್ಲದಿದ್ದರೆ
        m.dataend = m.datalimit = m.ದತ್ತಾಂಶ;
}

//======================================= Mat ======================================================

Mat::Mat() CV_NOEXCEPT
    : flags(MAGIC_VAL), dims(0), ಅಡ್ಡಸಾಲುಗಳು(0), ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ(0), ದತ್ತಾಂಶ(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{}

Mat::Mat(ಪೂರ್ಣಾಂಕ ಅಡ್ಡಸಾಲುಗಳು_3, ಪೂರ್ಣಾಂಕ ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_3, ಪೂರ್ಣಾಂಕ _type)
    : flags(MAGIC_VAL), dims(0), ಅಡ್ಡಸಾಲುಗಳು(0), ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ(0), ದತ್ತಾಂಶ(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(ಅಡ್ಡಸಾಲುಗಳು_3, ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_3, _type);
}

Mat::Mat(ಪೂರ್ಣಾಂಕ ಅಡ್ಡಸಾಲುಗಳು_3, ಪೂರ್ಣಾಂಕ ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_3, ಪೂರ್ಣಾಂಕ _type, ಸ್ಥಿರಾಂಕ Scalar& _s)
    : flags(MAGIC_VAL), dims(0), ಅಡ್ಡಸಾಲುಗಳು(0), ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ(0), ದತ್ತಾಂಶ(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(ಅಡ್ಡಸಾಲುಗಳು_3, ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_3, _type);
    *ಇದು = _s;
}

Mat::Mat(ಗಾತ್ರ_2 _sz, ಪೂರ್ಣಾಂಕ _type)
    : flags(MAGIC_VAL), dims(0), ಅಡ್ಡಸಾಲುಗಳು(0), ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ(0), ದತ್ತಾಂಶ(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create( _sz.ಎತ್ತರ, _sz.ಅಗಲ, _type );
}

Mat::Mat(ಗಾತ್ರ_2 _sz, ಪೂರ್ಣಾಂಕ _type, ಸ್ಥಿರಾಂಕ Scalar& _s)
    : flags(MAGIC_VAL), dims(0), ಅಡ್ಡಸಾಲುಗಳು(0), ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ(0), ದತ್ತಾಂಶ(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_sz.ಎತ್ತರ, _sz.ಅಗಲ, _type);
    *ಇದು = _s;
}

Mat::Mat(ಪೂರ್ಣಾಂಕ _dims, ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* _sz, ಪೂರ್ಣಾಂಕ _type)
    : flags(MAGIC_VAL), dims(0), ಅಡ್ಡಸಾಲುಗಳು(0), ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ(0), ದತ್ತಾಂಶ(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_dims, _sz, _type);
}

Mat::Mat(ಪೂರ್ಣಾಂಕ _dims, ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* _sz, ಪೂರ್ಣಾಂಕ _type, ಸ್ಥಿರಾಂಕ Scalar& _s)
    : flags(MAGIC_VAL), dims(0), ಅಡ್ಡಸಾಲುಗಳು(0), ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ(0), ದತ್ತಾಂಶ(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_dims, _sz, _type);
    *ಇದು = _s;
}

Mat::Mat(ಸ್ಥಿರಾಂಕ std::vector<ಪೂರ್ಣಾಂಕ>& _sz, ಪೂರ್ಣಾಂಕ _type)
    : flags(MAGIC_VAL), dims(0), ಅಡ್ಡಸಾಲುಗಳು(0), ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ(0), ದತ್ತಾಂಶ(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_sz, _type);
}

Mat::Mat(ಸ್ಥಿರಾಂಕ std::vector<ಪೂರ್ಣಾಂಕ>& _sz, ಪೂರ್ಣಾಂಕ _type, ಸ್ಥಿರಾಂಕ Scalar& _s)
    : flags(MAGIC_VAL), dims(0), ಅಡ್ಡಸಾಲುಗಳು(0), ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ(0), ದತ್ತಾಂಶ(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_sz, _type);
    *ಇದು = _s;
}

Mat::Mat(ಸ್ಥಿರಾಂಕ MatShape& _shape, ಪೂರ್ಣಾಂಕ _type)
    : flags(MAGIC_VAL), dims(0), ಅಡ್ಡಸಾಲುಗಳು(0), ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ(0), ದತ್ತಾಂಶ(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_shape, _type);
}

Mat::Mat(std::initializer_list<ಪೂರ್ಣಾಂಕ> _shape, ಪೂರ್ಣಾಂಕ _type)
    : flags(MAGIC_VAL), dims(0), ಅಡ್ಡಸಾಲುಗಳು(0), ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ(0), ದತ್ತಾಂಶ(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_shape, _type);
}

Mat::Mat(ಸ್ಥಿರಾಂಕ MatShape& _shape, ಪೂರ್ಣಾಂಕ _type, ಸ್ಥಿರಾಂಕ Scalar& _s)
    : flags(MAGIC_VAL), dims(0), ಅಡ್ಡಸಾಲುಗಳು(0), ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ(0), ದತ್ತಾಂಶ(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_shape, _type);
    *ಇದು = _s;
}

Mat::Mat(std::initializer_list<ಪೂರ್ಣಾಂಕ> _shape, ಪೂರ್ಣಾಂಕ _type, ಸ್ಥಿರಾಂಕ Scalar& _s)
    : flags(MAGIC_VAL), dims(0), ಅಡ್ಡಸಾಲುಗಳು(0), ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ(0), ದತ್ತಾಂಶ(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_shape, _type);
    *ಇದು = _s;
}

Mat::Mat(ಸ್ಥಿರಾಂಕ Mat& m)
    : flags(m.flags), dims(m.dims), ಅಡ್ಡಸಾಲುಗಳು(m.ಅಡ್ಡಸಾಲುಗಳು), ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ(m.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ), ದತ್ತಾಂಶ(m.ದತ್ತಾಂಶ),
      datastart(m.datastart), dataend(m.dataend), datalimit(m.datalimit), allocator(m.allocator),
      u(m.u), size(m.size), ಹೆಜ್ಜೆ(m.ಹೆಜ್ಜೆ)
{
    ಆದರೆ( u )
        CV_XADD(&u->refcount, 1);
}

Mat::Mat(ಪೂರ್ಣಾಂಕ ಅಡ್ಡಸಾಲುಗಳು_3, ಪೂರ್ಣಾಂಕ ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_3, ಪೂರ್ಣಾಂಕ _type, ಶೂನ್ಯ* ದತ್ತಾಂಶ_3, size_t ಹೆಜ್ಜೆ_3)
    : flags(MAGIC_VAL + (_type & TYPE_ಮುಖವಾಡ)), dims(2), ಅಡ್ಡಸಾಲುಗಳು(ಅಡ್ಡಸಾಲುಗಳು_3), ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ(ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_3),
      ದತ್ತಾಂಶ((uchar*)ದತ್ತಾಂಶ_3), datastart((uchar*)ದತ್ತಾಂಶ_3), dataend(0), datalimit(0),
      allocator(0), u(0), size(2)
{
    CV_Assert(ಒಟ್ಟು() == 0 || ದತ್ತಾಂಶ != ಖಾಲಿ);

    size_t esz = CV_ELEM_ಗಾತ್ರ(_type), esz1 = CV_ELEM_SIZE1(_type);
    size_t minstep = ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ * esz;
    ಆದರೆ( ಹೆಜ್ಜೆ_3 == AUTO_ಹೆಜ್ಜೆ )
    {
        ಹೆಜ್ಜೆ_3 = minstep;
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        CV_Assert( ಹೆಜ್ಜೆ_3 >= minstep );
        ಆದರೆ (ಹೆಜ್ಜೆ_3 % esz1 != 0)
        {
            CV_Error(Error::Bad_ಹೆಜ್ಜೆ, "Step must be a multiple of esz1");
        }
    }
    size[0] = ಅಡ್ಡಸಾಲುಗಳು;
    size[1] = ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ;
    ಹೆಜ್ಜೆ[0] = ಹೆಜ್ಜೆ_3;
    ಹೆಜ್ಜೆ[1] = esz;
    datalimit = datastart + ಹೆಜ್ಜೆ_3 * ಅಡ್ಡಸಾಲುಗಳು;
    dataend = datalimit - ಹೆಜ್ಜೆ_3 + minstep;
    updateContinuityFlag();
}

Mat::Mat(ಗಾತ್ರ_2 _sz, ಪೂರ್ಣಾಂಕ _type, ಶೂನ್ಯ* ದತ್ತಾಂಶ_3, size_t ಹೆಜ್ಜೆ_3)
    : flags(MAGIC_VAL + (_type & TYPE_ಮುಖವಾಡ)), dims(2), ಅಡ್ಡಸಾಲುಗಳು(_sz.ಎತ್ತರ), ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ(_sz.ಅಗಲ),
      ದತ್ತಾಂಶ((uchar*)ದತ್ತಾಂಶ_3), datastart((uchar*)ದತ್ತಾಂಶ_3), dataend(0), datalimit(0),
      allocator(0), u(0), size(2)
{
    CV_Assert(ಒಟ್ಟು() == 0 || ದತ್ತಾಂಶ != ಖಾಲಿ);

    size_t esz = CV_ELEM_ಗಾತ್ರ(_type), esz1 = CV_ELEM_SIZE1(_type);
    size_t minstep = ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ*esz;
    ಆದರೆ( ಹೆಜ್ಜೆ_3 == AUTO_ಹೆಜ್ಜೆ || (ಹೆಜ್ಜೆ_3 < minstep && ಅಡ್ಡಸಾಲುಗಳು == 1))
    {
        ಹೆಜ್ಜೆ_3 = minstep;
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        CV_CheckGE(ಹೆಜ್ಜೆ_3, minstep, "");

        ಆದರೆ (ಹೆಜ್ಜೆ_3 % esz1 != 0)
        {
            CV_Error(Error::Bad_ಹೆಜ್ಜೆ, "Step must be a multiple of esz1");
        }
    }
    size[0] = ಅಡ್ಡಸಾಲುಗಳು;
    size[1] = ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ;
    ಹೆಜ್ಜೆ[0] = ಹೆಜ್ಜೆ_3;
    ಹೆಜ್ಜೆ[1] = esz;
    datalimit = datastart + ಹೆಜ್ಜೆ_3*ಅಡ್ಡಸಾಲುಗಳು;
    dataend = datalimit - ಹೆಜ್ಜೆ_3 + minstep;
    updateContinuityFlag();
}


Mat::~Mat()
{
    release();
}

Mat& Mat::ಕ್ರಿಯಾಕಾರಕ = (ಸ್ಥಿರಾಂಕ Mat& m)
{
    ಆದರೆ( ಇದು != &m )
    {
        ಆದರೆ( m.u )
            CV_XADD(&m.u->refcount, 1);
        release();
        flags = m.flags;
        dims = m.dims;
        ಅಡ್ಡಸಾಲುಗಳು = m.ಅಡ್ಡಸಾಲುಗಳು;
        ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = m.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ;
        size = m.size;
        ಹೆಜ್ಜೆ = m.ಹೆಜ್ಜೆ;
        ದತ್ತಾಂಶ = m.ದತ್ತಾಂಶ;
        datastart = m.datastart;
        dataend = m.dataend;
        datalimit = m.datalimit;
        allocator = m.allocator;
        u = m.u;
    }
    ಹಿಂತಿರುಗಿ *ಇದು;
}

Mat Mat::clone() ಸ್ಥಿರಾಂಕ
{
    Mat m;
    copy_ಗಮ್ಯ(m);
    ಹಿಂತಿರುಗಿ m;
}

ಶೂನ್ಯ Mat::ನಿಯೋಜಿಸು_ಗಮ್ಯ( Mat& m, ಪೂರ್ಣಾಂಕ _type ) ಸ್ಥಿರಾಂಕ
{
    ಆದರೆ( _type < 0 )
        m = *ಇದು;
    ಇಲ್ಲದಿದ್ದರೆ
        convert_ಗಮ್ಯ(m, _type);
}

ಶೂನ್ಯ Mat::create(ಪೂರ್ಣಾಂಕ ಅಡ್ಡಸಾಲುಗಳು_3, ಪೂರ್ಣಾಂಕ ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_3, ಪೂರ್ಣಾಂಕ _type)
{
    _type &= TYPE_ಮುಖವಾಡ;
    ಆದರೆ( dims <= 2 && ಅಡ್ಡಸಾಲುಗಳು == ಅಡ್ಡಸಾಲುಗಳು_3 && ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ == ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_3 && type() == _type && ದತ್ತಾಂಶ )
        ಹಿಂತಿರುಗಿ;
    ಪೂರ್ಣಾಂಕ sz[] = {ಅಡ್ಡಸಾಲುಗಳು_3, ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_3};
    create(2, sz, _type);
}

ಶೂನ್ಯ Mat::create(ಗಾತ್ರ_2 _sz, ಪೂರ್ಣಾಂಕ _type)
{
    create(_sz.ಎತ್ತರ, _sz.ಅಗಲ, _type);
}

ಶೂನ್ಯ Mat::create_Same_ಗಾತ್ರ(ಒಳಹರಿವು_Array m, ಪೂರ್ಣಾಂಕ type)
{
    MatShape msize = m.shape();
    create(msize, type);
}

ಶೂನ್ಯ Mat::fit(ಪೂರ್ಣಾಂಕ _dims, ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* _sizes, ಪೂರ್ಣಾಂಕ _type)
{
    size_t old_ಒಟ್ಟು_ಬೈಟುಗಳು = u ? u->size : 0;
    size_t esz = CV_ELEM_ಗಾತ್ರ(_type), new_ಒಟ್ಟು = _dims >= 0;
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < _dims; i++)
        new_ಒಟ್ಟು *= _sizes[i];
    size_t new_ಒಟ್ಟು_ಬೈಟುಗಳು = new_ಒಟ್ಟು*esz;
    ಆದರೆ (new_ಒಟ್ಟು_ಬೈಟುಗಳು > 0 && (!isContinuous() ||
                              new_ಒಟ್ಟು_ಬೈಟುಗಳು > old_ಒಟ್ಟು_ಬೈಟುಗಳು ||
                              ದತ್ತಾಂಶ != datastart)) {
        create(_dims, _sizes, _type);
    } ಇಲ್ಲದಿದ್ದರೆ {
        flags = (flags & ~Mat::TYPE_ಮುಖವಾಡ) | CV_MAT_TYPE(_type);
        ಪೂರ್ಣಾಂಕ dummy_ಗಾತ್ರ = 0;
        ಗಣ_ಗಾತ್ರ(*ಇದು, (_dims >= 0 ? _dims : 1), (_dims >= 0 ? _sizes : &dummy_ಗಾತ್ರ), ಶೂನ್ಯಸೂಚಕ, ನಿಜ);
        finalizeHdr(*ಇದು);
    }
}

ಶೂನ್ಯ Mat::fit(ಸ್ಥಿರಾಂಕ std::vector<ಪೂರ್ಣಾಂಕ>& _shape, ಪೂರ್ಣಾಂಕ _type)
{
    fit((ಪೂರ್ಣಾಂಕ)_shape.size(), _shape.ದತ್ತಾಂಶ(), _type);
}

ಶೂನ್ಯ Mat::fit(ಸ್ಥಿರಾಂಕ MatShape& _shape, ಪೂರ್ಣಾಂಕ _type)
{
    fit(_shape.dims, _shape.p, _type);
    size.layout = _shape.layout;
    size.C = _shape.C;
}

ಶೂನ್ಯ Mat::fit(std::initializer_list<ಪೂರ್ಣಾಂಕ> _shape, ಪೂರ್ಣಾಂಕ _type)
{
    ಪೂರ್ಣಾಂಕ new_shape[MatShape::MAX_DIMS];
    ಪೂರ್ಣಾಂಕ new_ndims = (ಪೂರ್ಣಾಂಕ)_shape.size();
    CV_Assert(new_ndims <= MatShape::MAX_DIMS);
    ಸ್ವಯಂಚಾಲಿತ it = _shape.begin();
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < new_ndims; i++, ++it)
        new_shape[i] = *it;
    fit(new_ndims, new_shape, _type);
}

ಶೂನ್ಯ Mat::fit(ಪೂರ್ಣಾಂಕ ಅಡ್ಡಸಾಲುಗಳು_3, ಪೂರ್ಣಾಂಕ ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_3, ಪೂರ್ಣಾಂಕ _type)
{
    _type &= TYPE_ಮುಖವಾಡ;
    ಪೂರ್ಣಾಂಕ sz[] = {ಅಡ್ಡಸಾಲುಗಳು_3, ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_3};
    fit(2, sz, _type);
}

ಶೂನ್ಯ Mat::fit(ಗಾತ್ರ_2 _sz, ಪೂರ್ಣಾಂಕ _type)
{
    fit(_sz.ಎತ್ತರ, _sz.ಅಗಲ, _type);
}

ಶೂನ್ಯ Mat::fit_Same_ಗಾತ್ರ(ಒಳಹರಿವು_Array m, ಪೂರ್ಣಾಂಕ _type)
{
    fit(m.shape(), _type);
}

ಶೂನ್ಯ Mat::addref()
{
    ಆದರೆ( u )
        CV_XADD(&u->refcount, 1);
}

ಶೂನ್ಯ Mat::release()
{
    ಆದರೆ( u && CV_XADD(&u->refcount, -1) == 1 )
        deallocate();
    u = ಖಾಲಿ;
    datastart = dataend = datalimit = ದತ್ತಾಂಶ = 0;
    ಗಾಗಿ(ಪೂರ್ಣಾಂಕ i = 0; i < dims; i++)
        size.p[i] = 0;
    flags = (flags & CV_MAT_TYPE_ಮುಖವಾಡ) | MAGIC_VAL;
    dims = ಅಡ್ಡಸಾಲುಗಳು = ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = 0;
    size.clear();
}

size_t Mat::step1(ಪೂರ್ಣಾಂಕ i) ಸ್ಥಿರಾಂಕ
{
    ಹಿಂತಿರುಗಿ ಹೆಜ್ಜೆ.p[i] / elemSize1();
}

ತಾರ್ಕಿಕ Mat::empty() ಸ್ಥಿರಾಂಕ
{
    ಹಿಂತಿರುಗಿ ದತ್ತಾಂಶ == 0 || ಒಟ್ಟು() == 0;
}

size_t Mat::ಒಟ್ಟು() ಸ್ಥಿರಾಂಕ
{
    ಆದರೆ( dims <= 2 )
        ಹಿಂತಿರುಗಿ (size_t)ಅಡ್ಡಸಾಲುಗಳು * ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ;
    size_t p = 1;
    ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = 0; i < dims; i++ )
        p *= size[i];
    ಹಿಂತಿರುಗಿ p;
}

size_t Mat::ಒಟ್ಟು(ಪೂರ್ಣಾಂಕ ಆರಂಭ_Dim, ಪೂರ್ಣಾಂಕ ಕೊನೆ_Dim) ಸ್ಥಿರಾಂಕ
{
    CV_Assert( 0 <= ಆರಂಭ_Dim && ಆರಂಭ_Dim <= ಕೊನೆ_Dim);
    size_t p = 1;
    ಪೂರ್ಣಾಂಕ ಕೊನೆ_Dim_2 = ಕೊನೆ_Dim <= dims ? ಕೊನೆ_Dim : dims;
    ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = ಆರಂಭ_Dim; i < ಕೊನೆ_Dim_2; i++ )
        p *= size[i];
    ಹಿಂತಿರುಗಿ p;
}

Mat::Mat(Mat&& m) CV_NOEXCEPT
    : flags(m.flags), dims(m.dims), ಅಡ್ಡಸಾಲುಗಳು(m.ಅಡ್ಡಸಾಲುಗಳು), ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ(m.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ), ದತ್ತಾಂಶ(m.ದತ್ತಾಂಶ),
      datastart(m.datastart), dataend(m.dataend), datalimit(m.datalimit), allocator(m.allocator),
      u(m.u), size(m.size), ಹೆಜ್ಜೆ(m.ಹೆಜ್ಜೆ)
{
    m.flags = MAGIC_VAL; m.dims = m.ಅಡ್ಡಸಾಲುಗಳು = m.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = 0;
    m.size.clear(); m.ಹೆಜ್ಜೆ.clear();
    m.ದತ್ತಾಂಶ = ಖಾಲಿ; m.datastart = ಖಾಲಿ; m.dataend = ಖಾಲಿ; m.datalimit = ಖಾಲಿ;
    m.allocator = ಖಾಲಿ;
    m.u = ಖಾಲಿ;
}


Mat& Mat::ಕ್ರಿಯಾಕಾರಕ = (Mat&& m)
{
    ಆದರೆ (ಇದು == &m)
      ಹಿಂತಿರುಗಿ *ಇದು;

    release();
    flags = m.flags; dims = m.dims; ಅಡ್ಡಸಾಲುಗಳು = m.ಅಡ್ಡಸಾಲುಗಳು; ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = m.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ; ದತ್ತಾಂಶ = m.ದತ್ತಾಂಶ;
    size = m.size; ಹೆಜ್ಜೆ = m.ಹೆಜ್ಜೆ;
    datastart = m.datastart; dataend = m.dataend; datalimit = m.datalimit; allocator = m.allocator;
    u = m.u;
    m.flags = MAGIC_VAL; m.dims = m.ಅಡ್ಡಸಾಲುಗಳು = m.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = 0;
    m.size.clear(); m.ಹೆಜ್ಜೆ.clear();
    m.ದತ್ತಾಂಶ = ಖಾಲಿ; m.datastart = ಖಾಲಿ; m.dataend = ಖಾಲಿ; m.datalimit = ಖಾಲಿ;
    m.allocator = ಖಾಲಿ;
    m.u = ಖಾಲಿ;
    ಹಿಂತಿರುಗಿ *ಇದು;
}


ಶೂನ್ಯ Mat::create(ಪೂರ್ಣಾಂಕ d0, ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* _sizes, ಪೂರ್ಣಾಂಕ _type)
{
    ಪೂರ್ಣಾಂಕ sz1 = 1, d = d0;
    ಪೂರ್ಣಾಂಕ i;
    ಆದರೆ (d == 0) {
        d = 1;
        _sizes = (ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ*)&sz1;
    }
    CV_Assert(0 <= d && d <= CV_MAX_DIM && _sizes);
    _type = CV_MAT_TYPE(_type);

    ಆದರೆ( ದತ್ತಾಂಶ && d == dims && _type == type() )
    {
        ಗಾಗಿ( i = 0; i < d; i++ )
            ಆದರೆ( size[i] != _sizes[i] )
                ನಿಲ್ಲಿಸು;
        ಆದರೆ( i == d )
            ಹಿಂತಿರುಗಿ;
    }

    ಪೂರ್ಣಾಂಕ _sizes_backup[CV_MAX_DIM]; // #5991
    ಆದರೆ (_sizes == (ಇದು->size.p))
    {
        ಗಾಗಿ(i = 0; i < d; i++ )
            _sizes_backup[i] = _sizes[i];
        _sizes = _sizes_backup;
    }

    release();
    ಆದರೆ( d == 0 )
        ಹಿಂತಿರುಗಿ;
    flags = (_type & CV_MAT_TYPE_ಮುಖವಾಡ) | MAGIC_VAL;
    ಗಣ_ಗಾತ್ರ(*ಇದು, d, _sizes, 0, ನಿಜ);

    ಆದರೆ( ಒಟ್ಟು() > 0 )
    {
        MatAllocator *a = allocator, *a0 = getDefaultAllocator();
        ಆದರೆ(!a)
            a = a0;
        ಪ್ರಯತ್ನಿಸು
        {
            u = a->allocate(dims, size.p, _type, 0, ಹೆಜ್ಜೆ.p, ACCESS_RW /* ignored */, USAGE_DEFAULT);
            CV_Assert(u != 0);
            allocator = a;
        }
        ಹಿಡಿ (...)
        {
            ಆದರೆ (a == a0)
                ಎಸೆ;
            u = a0->allocate(dims, size.p, _type, 0, ಹೆಜ್ಜೆ.p, ACCESS_RW /* ignored */, USAGE_DEFAULT);
            CV_Assert(u != 0);
            allocator = a0;
        }
        CV_Assert( ಹೆಜ್ಜೆ[dims-1] == (size_t)CV_ELEM_ಗಾತ್ರ(flags) );
    }

    addref();
    finalizeHdr(*ಇದು);
    size.dims = dims = d0;
}

ಶೂನ್ಯ Mat::create(ಸ್ಥಿರಾಂಕ std::vector<ಪೂರ್ಣಾಂಕ>& _sizes, ಪೂರ್ಣಾಂಕ _type)
{
    create((ಪೂರ್ಣಾಂಕ)_sizes.size(), _sizes.ದತ್ತಾಂಶ(), _type);
}

ಶೂನ್ಯ Mat::create(ಸ್ಥಿರಾಂಕ MatShape& _shape, ಪೂರ್ಣಾಂಕ _type)
{
    ಆದರೆ (_shape.dims < 0) {
        release();
        ಹಿಂತಿರುಗಿ;
    }
    create(_shape.dims, _shape.p, _type);
    size.layout = _shape.layout;
    size.C = _shape.C;
}

ಶೂನ್ಯ Mat::create(std::initializer_list<ಪೂರ್ಣಾಂಕ> _shape, ಪೂರ್ಣಾಂಕ _type)
{
    ಪೂರ್ಣಾಂಕ new_shape[MatShape::MAX_DIMS];
    ಪೂರ್ಣಾಂಕ new_ndims = (ಪೂರ್ಣಾಂಕ)_shape.size();
    CV_Assert(new_ndims <= MatShape::MAX_DIMS);
    ಸ್ವಯಂಚಾಲಿತ it = _shape.begin();
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < new_ndims; i++, ++it)
        new_shape[i] = *it;

    create(new_ndims, new_shape, _type);
}

ಶೂನ್ಯ Mat::copy_ಗಾತ್ರ(ಸ್ಥಿರಾಂಕ Mat& m)
{
    dims = m.dims;
    ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = m.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ;
    ಅಡ್ಡಸಾಲುಗಳು = m.ಅಡ್ಡಸಾಲುಗಳು;
    size = m.size;
    ಹೆಜ್ಜೆ = m.ಹೆಜ್ಜೆ;
}

ಶೂನ್ಯ Mat::deallocate()
{
    ಆದರೆ(u)
    {
        UMat_ದತ್ತಾಂಶ* u_ = u;
        u = ಖಾಲಿ;
        (u_->currAllocator ? u_->currAllocator : allocator ? allocator : getDefaultAllocator())->unmap(u_);
    }
}

Mat::Mat(ಸ್ಥಿರಾಂಕ Mat& m, ಸ್ಥಿರಾಂಕ Range& ಅಡ್ಡಸಾಲು_Range_3, ಸ್ಥಿರಾಂಕ Range& ಲಂಬಸಾಲು_Range_3)
    : flags(MAGIC_VAL), dims(0), ಅಡ್ಡಸಾಲುಗಳು(0), ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ(0), ದತ್ತಾಂಶ(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    CV_Assert(m.dims <= MatShape::MAX_DIMS);
    CV_Assert(m.dims >= 2 ||
              (m.dims == 1 && (ಅಡ್ಡಸಾಲು_Range_3 == Range::all() ||
                               ಅಡ್ಡಸಾಲು_Range_3 == Range(0, m.ಅಡ್ಡಸಾಲುಗಳು))));
    ಆದರೆ( m.dims > 2 )
    {
        Range rs[MatShape::MAX_DIMS];
        rs[0] = ಅಡ್ಡಸಾಲು_Range_3;
        rs[1] = ಲಂಬಸಾಲು_Range_3;
        ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = 2; i < m.dims; i++ )
            rs[i] = Range::all();
        *ಇದು = m(rs);
        ಹಿಂತಿರುಗಿ;
    }

    *ಇದು = m;
    ಪ್ರಯತ್ನಿಸು
    {
        ಆದರೆ( ಅಡ್ಡಸಾಲು_Range_3 != Range::all() && ಅಡ್ಡಸಾಲು_Range_3 != Range(0,ಅಡ್ಡಸಾಲುಗಳು) )
        {
            CV_Assert( 0 <= ಅಡ್ಡಸಾಲು_Range_3.ಆರಂಭ && ಅಡ್ಡಸಾಲು_Range_3.ಆರಂಭ <= ಅಡ್ಡಸಾಲು_Range_3.end
                       && ಅಡ್ಡಸಾಲು_Range_3.end <= m.ಅಡ್ಡಸಾಲುಗಳು );
            size[0] = ಅಡ್ಡಸಾಲುಗಳು = ಅಡ್ಡಸಾಲು_Range_3.size();
            ದತ್ತಾಂಶ += ಹೆಜ್ಜೆ*ಅಡ್ಡಸಾಲು_Range_3.ಆರಂಭ;
            flags |= SUBMATRIX_FLAG;
        }

        ಆದರೆ( ಲಂಬಸಾಲು_Range_3 != Range::all() && ಲಂಬಸಾಲು_Range_3 != Range(0,ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ) )
        {
            CV_Assert( 0 <= ಲಂಬಸಾಲು_Range_3.ಆರಂಭ && ಲಂಬಸಾಲು_Range_3.ಆರಂಭ <= ಲಂಬಸಾಲು_Range_3.end
                       && ಲಂಬಸಾಲು_Range_3.end <= m.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ );
            size[dims > 1] = ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = ಲಂಬಸಾಲು_Range_3.size();
            ದತ್ತಾಂಶ += ಲಂಬಸಾಲು_Range_3.ಆರಂಭ*elem_ಗಾತ್ರ();
            flags |= SUBMATRIX_FLAG;
        }
    }
    ಹಿಡಿ(...)
    {
        release();
        ಎಸೆ;
    }

    updateContinuityFlag();

    ಆದರೆ( ಅಡ್ಡಸಾಲುಗಳು <= 0 || ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ <= 0 )
    {
        release();
        ಅಡ್ಡಸಾಲುಗಳು = ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = 0;
    }
}


Mat::Mat(ಸ್ಥಿರಾಂಕ Mat& m, ಸ್ಥಿರಾಂಕ Rect& roi)
    : flags(m.flags), dims(2), ಅಡ್ಡಸಾಲುಗಳು(roi.ಎತ್ತರ), ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ(roi.ಅಗಲ),
    ದತ್ತಾಂಶ(m.ದತ್ತಾಂಶ + roi.y*m.ಹೆಜ್ಜೆ[0]),
    datastart(m.datastart), dataend(m.dataend), datalimit(m.datalimit),
    allocator(m.allocator), u(m.u), size(2)
{
    CV_Assert( m.dims <= 2 );

    size_t esz = CV_ELEM_ಗಾತ್ರ(flags);
    ದತ್ತಾಂಶ += roi.x*esz;
    CV_Assert( 0 <= roi.x && 0 <= roi.ಅಗಲ && roi.x + roi.ಅಗಲ <= m.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ &&
              0 <= roi.y && 0 <= roi.ಎತ್ತರ && roi.y + roi.ಎತ್ತರ <= m.ಅಡ್ಡಸಾಲುಗಳು );
    ಆದರೆ( roi.ಅಗಲ < m.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ || roi.ಎತ್ತರ < m.ಅಡ್ಡಸಾಲುಗಳು )
        flags |= SUBMATRIX_FLAG;

    size[0] = ಅಡ್ಡಸಾಲುಗಳು;
    size[1] = ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ;
    ಹೆಜ್ಜೆ[0] = m.ಹೆಜ್ಜೆ[0];
    ಹೆಜ್ಜೆ[1] = esz;
    updateContinuityFlag();

    addref();
    ಆದರೆ( ಅಡ್ಡಸಾಲುಗಳು <= 0 || ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ <= 0 )
    {
        ಅಡ್ಡಸಾಲುಗಳು = ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = 0;
        release();
    }
}


Mat::Mat(ಪೂರ್ಣಾಂಕ _dims, ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* _sizes, ಪೂರ್ಣಾಂಕ _type, ಶೂನ್ಯ* ದತ್ತಾಂಶ_3, ಸ್ಥಿರಾಂಕ size_t* ಹೆಜ್ಜೆಗಳು_2)
    : flags(MAGIC_VAL), dims(0), ಅಡ್ಡಸಾಲುಗಳು(0), ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ(0), ದತ್ತಾಂಶ(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    flags |= CV_MAT_TYPE(_type);
    datastart = ದತ್ತಾಂಶ = (uchar*)ದತ್ತಾಂಶ_3;
    ಆದರೆ (_dims > 0 || ದತ್ತಾಂಶ_3 != ಶೂನ್ಯಸೂಚಕ)
        ಗಣ_ಗಾತ್ರ(*ಇದು, _dims, _sizes, ಹೆಜ್ಜೆಗಳು_2, ನಿಜ);
    finalizeHdr(*ಇದು);
}


Mat::Mat(ಸ್ಥಿರಾಂಕ std::vector<ಪೂರ್ಣಾಂಕ>& _sizes, ಪೂರ್ಣಾಂಕ _type, ಶೂನ್ಯ* ದತ್ತಾಂಶ_3, ಸ್ಥಿರಾಂಕ size_t* ಹೆಜ್ಜೆಗಳು_2)
    : flags(MAGIC_VAL), dims(0), ಅಡ್ಡಸಾಲುಗಳು(0), ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ(0), ದತ್ತಾಂಶ(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    flags |= CV_MAT_TYPE(_type);
    datastart = ದತ್ತಾಂಶ = (uchar*)ದತ್ತಾಂಶ_3;
    ಪೂರ್ಣಾಂಕ _dims = (ಪೂರ್ಣಾಂಕ)_sizes.size();
    ಆದರೆ (_dims > 0 || ದತ್ತಾಂಶ_3 != ಶೂನ್ಯಸೂಚಕ)
        ಗಣ_ಗಾತ್ರ(*ಇದು, _dims, _sizes.ದತ್ತಾಂಶ(), ಹೆಜ್ಜೆಗಳು_2, ನಿಜ);
    finalizeHdr(*ಇದು);
}

Mat::Mat(ಸ್ಥಿರಾಂಕ MatShape& _shape, ಪೂರ್ಣಾಂಕ _type, ಶೂನ್ಯ* ದತ್ತಾಂಶ_3, ಸ್ಥಿರಾಂಕ size_t* ಹೆಜ್ಜೆಗಳು_2)
    : flags(MAGIC_VAL), dims(0), ಅಡ್ಡಸಾಲುಗಳು(0), ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ(0), ದತ್ತಾಂಶ(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    flags |= CV_MAT_TYPE(_type);
    datastart = ದತ್ತಾಂಶ = (uchar*)ದತ್ತಾಂಶ_3;
    ಆದರೆ (_shape.dims >= 0) {
        ಗಣ_ಗಾತ್ರ(*ಇದು, _shape.dims, _shape.p, ಹೆಜ್ಜೆಗಳು_2, ನಿಜ);
    }
    ಇಲ್ಲದಿದ್ದರೆ {
        CV_Assert(!ದತ್ತಾಂಶ);
    }
    finalizeHdr(*ಇದು);
}

Mat::Mat(std::initializer_list<ಪೂರ್ಣಾಂಕ> _shape, ಪೂರ್ಣಾಂಕ _type, ಶೂನ್ಯ* ದತ್ತಾಂಶ_3, ಸ್ಥಿರಾಂಕ size_t* ಹೆಜ್ಜೆಗಳು_2)
    : flags(MAGIC_VAL), dims(0), ಅಡ್ಡಸಾಲುಗಳು(0), ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ(0), ದತ್ತಾಂಶ(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    ಪೂರ್ಣಾಂಕ new_shape[MatShape::MAX_DIMS];
    ಪೂರ್ಣಾಂಕ _dims = (ಪೂರ್ಣಾಂಕ)_shape.size();
    CV_Assert(_dims <= MatShape::MAX_DIMS);
    ಸ್ವಯಂಚಾಲಿತ it = _shape.begin();
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < _dims; i++, ++it)
        new_shape[i] = *it;

    flags |= CV_MAT_TYPE(_type);
    datastart = ದತ್ತಾಂಶ = (uchar*)ದತ್ತಾಂಶ_3;
    ಆದರೆ (_dims > 0 || ದತ್ತಾಂಶ_3 != ಶೂನ್ಯಸೂಚಕ)
        ಗಣ_ಗಾತ್ರ(*ಇದು, _dims, new_shape, ಹೆಜ್ಜೆಗಳು_2, ನಿಜ);
    finalizeHdr(*ಇದು);
}

Mat::Mat(ಸ್ಥಿರಾಂಕ Mat& m, ಸ್ಥಿರಾಂಕ Range* ranges)
    : flags(MAGIC_VAL), dims(0), ಅಡ್ಡಸಾಲುಗಳು(0), ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ(0), ದತ್ತಾಂಶ(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    ಪೂರ್ಣಾಂಕ d = m.dims;

    CV_Assert(ranges);
    ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = 0; i < d; i++ )
    {
        Range r = ranges[i];
        CV_Assert( r == Range::all() || (0 <= r.ಆರಂಭ && r.ಆರಂಭ < r.end && r.end <= m.size[i]) );
    }
    *ಇದು = m;
    ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = 0; i < d; i++ )
    {
        Range r = ranges[i];
        ಆದರೆ( r != Range::all() && r != Range(0, size.p[i]))
        {
            size.p[i] = r.end - r.ಆರಂಭ;
            ದತ್ತಾಂಶ += r.ಆರಂಭ*ಹೆಜ್ಜೆ.p[i];
            flags |= SUBMATRIX_FLAG;
        }
    }

    ಆದರೆ (d <= 2) {
        ಅಡ್ಡಸಾಲುಗಳು = d == 2 ? size[0] : 1;
        ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = d <= 0 ? (d >= 0) : size[d > 1];
    }
    updateContinuityFlag();
}

Mat::Mat(ಸ್ಥಿರಾಂಕ Mat& m, ಸ್ಥಿರಾಂಕ std::vector<Range>& ranges)
    : flags(MAGIC_VAL), dims(0), ಅಡ್ಡಸಾಲುಗಳು(0), ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ(0), ದತ್ತಾಂಶ(0), datastart(0), dataend(0),
    datalimit(0), allocator(0), u(0)
{
    ಪೂರ್ಣಾಂಕ d = m.dims;

    CV_Assert((ಪೂರ್ಣಾಂಕ)ranges.size() == d);
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < d; i++)
    {
        Range r = ranges[i];
        CV_Assert(r == Range::all() || (0 <= r.ಆರಂಭ && r.ಆರಂಭ < r.end && r.end <= m.size[i]));
    }
    *ಇದು = m;
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < d; i++)
    {
        Range r = ranges[i];
        ಆದರೆ (r != Range::all() && r != Range(0, size.p[i]))
        {
            size.p[i] = r.end - r.ಆರಂಭ;
            ದತ್ತಾಂಶ += r.ಆರಂಭ*ಹೆಜ್ಜೆ.p[i];
            flags |= SUBMATRIX_FLAG;
        }
    }

    ಆದರೆ (d <= 2) {
        ಅಡ್ಡಸಾಲುಗಳು = d == 2 ? size[0] : 1;
        ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = d <= 0 ? (d >= 0) : size[d > 1];
    }
    updateContinuityFlag();
}


Mat Mat::diag(ಪೂರ್ಣಾಂಕ d) ಸ್ಥಿರಾಂಕ
{
    CV_Assert( dims == 2 );
    Mat m = *ಇದು;
    size_t esz = elem_ಗಾತ್ರ();
    ಪೂರ್ಣಾಂಕ len;

    ಆದರೆ( d >= 0 )
    {
        len = std::min(ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ - d, ಅಡ್ಡಸಾಲುಗಳು);
        m.ದತ್ತಾಂಶ += esz*d;
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        len = std::min(ಅಡ್ಡಸಾಲುಗಳು + d, ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ);
        m.ದತ್ತಾಂಶ -= ಹೆಜ್ಜೆ[0]*d;
    }
    CV_DbgAssert( len > 0 );

    m.size[0] = m.ಅಡ್ಡಸಾಲುಗಳು = len;
    m.size[1] = m.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = 1;
    m.ಹೆಜ್ಜೆ[0] += (len > 1 ? esz : 0);

    m.updateContinuityFlag();

    ಆದರೆ( size() != ಗಾತ್ರ_2(1,1) )
        m.flags |= SUBMATRIX_FLAG;

    ಹಿಂತಿರುಗಿ m;
}


ಶೂನ್ಯ Mat::pop_back(size_t nelems)
{
    CV_Assert( nelems <= (size_t)size.p[0] );
    CV_Assert( dims >= 1 );

    ಆದರೆ( isSubmatrix() )
        *ಇದು = ಅಡ್ಡಸಾಲು_Range_2(0, size.p[0] - (ಪೂರ್ಣಾಂಕ)nelems);
    ಇಲ್ಲದಿದ್ದರೆ
    {
        size.p[0] -= (ಪೂರ್ಣಾಂಕ)nelems;
        ಆದರೆ (dims == 2)
            ಅಡ್ಡಸಾಲುಗಳು = size.p[0];
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (dims == 1)
            ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = size.p[0];
        dataend -= nelems*ಹೆಜ್ಜೆ.p[0];
    }
}


ಶೂನ್ಯ Mat::ತಳ್ಳು_ಹಿಂಭಾಗ(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* elem)
{
    size_t r = size.p[0];
    ಆದರೆ( isSubmatrix() || dataend + ಹೆಜ್ಜೆ.p[0] > datalimit )
        reserve( std::max(r + 1, (r*3+1)/2) );

    size_t esz = elem_ಗಾತ್ರ();
    ಸ್ಮರಣೆ_ನಕಲು(ದತ್ತಾಂಶ + r*ಹೆಜ್ಜೆ.p[0], elem, esz);
    size.p[0] = ಪೂರ್ಣಾಂಕ(r + 1);
    ಆದರೆ (dims == 2)
        ಅಡ್ಡಸಾಲುಗಳು = size.p[0];
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (dims == 1)
        ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = size.p[0];
    dataend += ಹೆಜ್ಜೆ.p[0];
    uint64 tsz = size.p[0];
    ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = 1; i < dims; i++ )
        tsz *= size.p[i];
    ಆದರೆ( esz < ಹೆಜ್ಜೆ.p[0] || tsz != (uint64)(ಪೂರ್ಣಾಂಕ)tsz )
        flags &= ~CONTINUOUS_FLAG;
}


ಶೂನ್ಯ Mat::reserve(size_t nelems)
{
    ಸ್ಥಿರಾಂಕ size_t MIN_ಗಾತ್ರ = 64;

    CV_Assert( (ಪೂರ್ಣಾಂಕ)nelems >= 0 );
    ಆದರೆ( !isSubmatrix() && ಹೆಜ್ಜೆ.p[0] != 0 && ದತ್ತಾಂಶ + ಹೆಜ್ಜೆ.p[0]*nelems <= datalimit )
        ಹಿಂತಿರುಗಿ;

    ಪೂರ್ಣಾಂಕ r = size.p[0];
    ಆದರೆ( (size_t)r >= nelems)
        ಹಿಂತಿರುಗಿ;

    MatShape newsize = size;
    size_t esz = elem_ಗಾತ್ರ();
    newsize.dims = std::max(newsize.dims, 1);
    ಆದರೆ (dims > 1 && r == 0 && ಹೆಜ್ಜೆ.p[0] == 0) {
        ಹೆಜ್ಜೆ.p[0] = size.p[1] * (dims == 2 ? esz : ಹೆಜ್ಜೆ.p[1]);
    }
    newsize.p[0] = (ಪೂರ್ಣಾಂಕ)nelems;
    size_t newbytes = newsize.ಒಟ್ಟು() * esz;
    ಆದರೆ (newbytes < MIN_ಗಾತ್ರ) {
        newsize.p[0] = 1;
        newsize.p[0] = ಪೂರ್ಣಾಂಕ(MIN_ಗಾತ್ರ / newsize.ಒಟ್ಟು() / esz);
    }

    Mat m(newsize, type());
    ಆದರೆ( r > 0)
    {
        Mat mpart = m.ಅಡ್ಡಸಾಲು_Range_2(0, r);
        copy_ಗಮ್ಯ(mpart);
    }

    *ಇದು = m;
    size.p[0] = r;

    ಆದರೆ (dims == 2)
        ಅಡ್ಡಸಾಲುಗಳು = size.p[0];
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (dims == 1)
        ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = size.p[0];
    dataend = ದತ್ತಾಂಶ + ಹೆಜ್ಜೆ.p[0]*r;
}


ಶೂನ್ಯ Mat::reserve_ಬಫರ್(size_t nbytes)
{
    size_t esz = 1;
    ಪೂರ್ಣಾಂಕ mtype = CV_8UC1;
    ಆದರೆ (!empty())
    {
        ಆದರೆ (!isSubmatrix() && ದತ್ತಾಂಶ + nbytes <= dataend)//Should it be datalimit?
            ಹಿಂತಿರುಗಿ;
        esz = elem_ಗಾತ್ರ();
        mtype = type();
    }

    size_t nelems = (nbytes - 1) / esz + 1;

#ಆದರೆ SIZE_MAX > UINT_MAX
    CV_Assert(nelems <= size_t(ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ)*size_t(ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ));
    ಪೂರ್ಣಾಂಕ newrows = nelems > size_t(ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ) ? nelems > 0x400*size_t(ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ) ? nelems > 0x100000 * size_t(ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ) ? nelems > 0x40000000 * size_t(ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ) ?
                  size_t(ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ) : 0x40000000 : 0x100000 : 0x400 : 1;
#ಇಲ್ಲದಿದ್ದರೆ
    ಪೂರ್ಣಾಂಕ newrows = nelems > size_t(ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ) ? 2 : 1;
#ಆದರೆ_ಅಂತ್ಯ
    ಪೂರ್ಣಾಂಕ newcols = (ಪೂರ್ಣಾಂಕ)((nelems - 1) / newrows + 1);

    create(newrows, newcols, mtype);
}


ಶೂನ್ಯ Mat::resize(size_t nelems)
{
    ಪೂರ್ಣಾಂಕ r = size.p[0];
    ಆದರೆ( r == (ಪೂರ್ಣಾಂಕ)nelems )
        ಹಿಂತಿರುಗಿ;
    CV_Assert( (ಪೂರ್ಣಾಂಕ)nelems >= 0 );

    ಆದರೆ( isSubmatrix() || ದತ್ತಾಂಶ + ಹೆಜ್ಜೆ.p[0]*nelems > datalimit )
        reserve(nelems);

    size.p[0] = (ಪೂರ್ಣಾಂಕ)nelems;
    ಆದರೆ (dims == 2)
        ಅಡ್ಡಸಾಲುಗಳು = size.p[0];
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (dims == 1)
        ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = size.p[0];
    dataend += (size.p[0] - r)*(int64_t)ಹೆಜ್ಜೆ.p[0];

    //updateContinuityFlag(*this);
}


ಶೂನ್ಯ Mat::resize(size_t nelems, ಸ್ಥಿರಾಂಕ Scalar& s)
{
    ಪೂರ್ಣಾಂಕ save_ಅಡ್ಡಸಾಲುಗಳು = size.p[0];
    resize(nelems);

    ಆದರೆ( size.p[0] > save_ಅಡ್ಡಸಾಲುಗಳು )
    {
        Mat part = ಅಡ್ಡಸಾಲು_Range_2(save_ಅಡ್ಡಸಾಲುಗಳು, size.p[0]);
        part = s;
    }
}

ಶೂನ್ಯ Mat::push_back(ಸ್ಥಿರಾಂಕ Mat& elems)
{
    size_t r = size.p[0];
    size_t delta = elems.size.p[0];
    ಆದರೆ( delta == 0 )
        ಹಿಂತಿರುಗಿ;
    ಆದರೆ( ಇದು == &elems )
    {
        Mat tmp = elems;
        push_back(tmp);
        ಹಿಂತಿರುಗಿ;
    }
    ಆದರೆ( !ದತ್ತಾಂಶ )
    {
        *ಇದು = elems.clone();
        ಹಿಂತಿರುಗಿ;
    }
    CV_Assert(dims > 0);

    size.p[0] = elems.size.p[0];
    ತಾರ್ಕಿಕ eq = size == elems.size;
    size.p[0] = ಪೂರ್ಣಾಂಕ(r);
    ಆದರೆ( !eq )
        CV_Error(cv::Error::StsUnmatchedSizes, "Pushed vector length is not equal to matrix row length");
    ಆದರೆ( type() != elems.type() )
        CV_Error(cv::Error::StsUnmatchedFormats, "Pushed vector type is not the same as matrix type");
    size_t esz = elem_ಗಾತ್ರ();
    size_t minstep = dims <= 1 ? esz : size.p[1] * (dims == 2 ? esz : ಹೆಜ್ಜೆ.p[1]);
    size_t step0 = ಹೆಜ್ಜೆ.p[0];
    ಆದರೆ (step0 < minstep) {
        ಆದರೆ (size.p[0] <= 1)
            ಹೆಜ್ಜೆ.p[0] = step0 = minstep;
    }

    ಆದರೆ( isSubmatrix() || dataend + step0*delta > datalimit )
        reserve( std::max(r + delta, (r*3+1)/2) );

    size.p[0] += ಪೂರ್ಣಾಂಕ(delta);
    ಆದರೆ (dims == 2)
        ಅಡ್ಡಸಾಲುಗಳು = size.p[0];
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (dims == 1)
        ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = size.p[0];
    dataend += step0*delta;

    //updateContinuityFlag(*this);

    ಆದರೆ( isContinuous() && elems.isContinuous() )
        ಸ್ಮರಣೆ_ನಕಲು(ದತ್ತಾಂಶ + r*step0, elems.ದತ್ತಾಂಶ, elems.ಒಟ್ಟು()*elems.elem_ಗಾತ್ರ());
    ಇಲ್ಲದಿದ್ದರೆ
    {
        Mat part = ಅಡ್ಡಸಾಲು_Range_2(ಪೂರ್ಣಾಂಕ(r), ಪೂರ್ಣಾಂಕ(r + delta));
        elems.copy_ಗಮ್ಯ(part);
    }
}


ಶೂನ್ಯ Mat::locateROI( ಗಾತ್ರ_2& ಸಂಪೂರ್ಣ_ಗಾತ್ರ, ಬಿಂದು& ofs ) ಸ್ಥಿರಾಂಕ
{
    CV_Assert( dims <= 2 && ಹೆಜ್ಜೆ[0] > 0 );
    size_t esz = elem_ಗಾತ್ರ(), minstep;
    ptrdiff_t delta1 = ದತ್ತಾಂಶ - datastart, delta2 = dataend - datastart;

    ಆದರೆ( delta1 == 0 )
        ofs.x = ofs.y = 0;
    ಇಲ್ಲದಿದ್ದರೆ
    {
        ofs.y = (ಪೂರ್ಣಾಂಕ)(delta1/ಹೆಜ್ಜೆ[0]);
        ofs.x = (ಪೂರ್ಣಾಂಕ)((delta1 - ಹೆಜ್ಜೆ[0]*ofs.y)/esz);
        CV_DbgAssert( ದತ್ತಾಂಶ == datastart + ofs.y*ಹೆಜ್ಜೆ[0] + ofs.x*esz );
    }
    minstep = (ofs.x + ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ)*esz;
    ಸಂಪೂರ್ಣ_ಗಾತ್ರ.ಎತ್ತರ = (ಪೂರ್ಣಾಂಕ)((delta2 - minstep)/ಹೆಜ್ಜೆ[0] + 1);
    ಸಂಪೂರ್ಣ_ಗಾತ್ರ.ಎತ್ತರ = std::max(ಸಂಪೂರ್ಣ_ಗಾತ್ರ.ಎತ್ತರ, ofs.y + ಅಡ್ಡಸಾಲುಗಳು);
    ಸಂಪೂರ್ಣ_ಗಾತ್ರ.ಅಗಲ = (ಪೂರ್ಣಾಂಕ)((delta2 - ಹೆಜ್ಜೆ*(ಸಂಪೂರ್ಣ_ಗಾತ್ರ.ಎತ್ತರ-1))/esz);
    ಸಂಪೂರ್ಣ_ಗಾತ್ರ.ಅಗಲ = std::max(ಸಂಪೂರ್ಣ_ಗಾತ್ರ.ಅಗಲ, ofs.x + ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ);
}

Mat& Mat::adjustROI( ಪೂರ್ಣಾಂಕ dtop, ಪೂರ್ಣಾಂಕ dbottom, ಪೂರ್ಣಾಂಕ dleft, ಪೂರ್ಣಾಂಕ dright )
{
    CV_Assert( dims <= 2 && ಹೆಜ್ಜೆ[0] > 0 );
    ಗಾತ್ರ_2 ಸಂಪೂರ್ಣ_ಗಾತ್ರ; ಬಿಂದು ofs;
    size_t esz = elem_ಗಾತ್ರ();
    locateROI( ಸಂಪೂರ್ಣ_ಗಾತ್ರ, ofs );
    ಪೂರ್ಣಾಂಕ row1 = std::min(std::max(ofs.y - dtop, 0), ಸಂಪೂರ್ಣ_ಗಾತ್ರ.ಎತ್ತರ), row2 = std::max(0, std::min(ofs.y + ಅಡ್ಡಸಾಲುಗಳು + dbottom, ಸಂಪೂರ್ಣ_ಗಾತ್ರ.ಎತ್ತರ));
    ಪೂರ್ಣಾಂಕ col1 = std::min(std::max(ofs.x - dleft, 0), ಸಂಪೂರ್ಣ_ಗಾತ್ರ.ಅಗಲ), col2 = std::max(0, std::min(ofs.x + ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ + dright, ಸಂಪೂರ್ಣ_ಗಾತ್ರ.ಅಗಲ));
    ಆದರೆ(row1 > row2)
        std::swap(row1, row2);
    ಆದರೆ(col1 > col2)
        std::swap(col1, col2);

    ಆದರೆ (dims == 1) {
        ದತ್ತಾಂಶ += (col1 - ofs.x)*(std::ptrdiff_t)esz;
        ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = col2 - col1;
        size.p[0] = ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ;
    } ಇಲ್ಲದಿದ್ದರೆ {
        ದತ್ತಾಂಶ += (row1 - ofs.y)*(std::ptrdiff_t)ಹೆಜ್ಜೆ + (col1 - ofs.x)*(std::ptrdiff_t)esz;
        ಅಡ್ಡಸಾಲುಗಳು = row2 - row1; ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = col2 - col1;
        size.p[0] = ಅಡ್ಡಸಾಲುಗಳು; size.p[1] = ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ;
        updateContinuityFlag();
    }
    ಹಿಂತಿರುಗಿ *ಇದು;
}

Mat Mat::reshape(ಪೂರ್ಣಾಂಕ new_cn, ಪೂರ್ಣಾಂಕ new_ಅಡ್ಡಸಾಲುಗಳು) ಸ್ಥಿರಾಂಕ
{
    ಪೂರ್ಣಾಂಕ cn = channels();
    Mat hdr = *ಇದು;

    ಆದರೆ( new_cn == 0 )
        new_cn = cn;

    ಆದರೆ( dims > 2 )
    {
        ಆದರೆ( new_ಅಡ್ಡಸಾಲುಗಳು == 0 )
        {
            // special case: just change the number of channnels; retain the same shape,
            // except for the last, innermost dimension
            CV_Assert(size[dims-1]*cn % new_cn == 0);
            hdr.flags = (hdr.flags & ~CV_MAT_CN_ಮುಖವಾಡ) | ((new_cn-1) << CV_CN_ಸರಿಸು);
            hdr.ಹೆಜ್ಜೆ[dims-1] = CV_ELEM_ಗಾತ್ರ(hdr.flags);
            hdr.size[dims-1] = hdr.size[dims-1]*cn / new_cn;
            ಹಿಂತಿರುಗಿ hdr;
        }
        CV_Assert( new_ಅಡ್ಡಸಾಲುಗಳು > 0 );
        ಪೂರ್ಣಾಂಕ sz[] = { new_ಅಡ್ಡಸಾಲುಗಳು, (ಪೂರ್ಣಾಂಕ)(ಒಟ್ಟು()*cn/new_ಅಡ್ಡಸಾಲುಗಳು) };
        ಹಿಂತಿರುಗಿ reshape(new_cn, 2, sz);
    }

    ಪೂರ್ಣಾಂಕ ಒಟ್ಟು_ಅಗಲ = ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ * cn;

    ಆದರೆ( (new_cn > ಒಟ್ಟು_ಅಗಲ || ಒಟ್ಟು_ಅಗಲ % new_cn != 0) && new_ಅಡ್ಡಸಾಲುಗಳು == 0 )
        new_ಅಡ್ಡಸಾಲುಗಳು = ಅಡ್ಡಸಾಲುಗಳು * ಒಟ್ಟು_ಅಗಲ / new_cn;

    ಆದರೆ( new_ಅಡ್ಡಸಾಲುಗಳು != 0 && new_ಅಡ್ಡಸಾಲುಗಳು != ಅಡ್ಡಸಾಲುಗಳು )
    {
        ಪೂರ್ಣಾಂಕ ಒಟ್ಟು_ಗಾತ್ರ_2 = ಒಟ್ಟು_ಅಗಲ * ಅಡ್ಡಸಾಲುಗಳು;
        ಆದರೆ( !isContinuous() )
            CV_Error( cv::Error::Bad_ಹೆಜ್ಜೆ,
            "The matrix is not continuous, thus its number of rows can not be changed" );

        ಒಟ್ಟು_ಅಗಲ = ಒಟ್ಟು_ಗಾತ್ರ_2 / new_ಅಡ್ಡಸಾಲುಗಳು;

        ಆದರೆ( ಒಟ್ಟು_ಅಗಲ * new_ಅಡ್ಡಸಾಲುಗಳು != ಒಟ್ಟು_ಗಾತ್ರ_2 )
            CV_Error( cv::Error::StsBadArg, "The total number of matrix elements "
                                    "is not divisible by the new number of rows" );

        hdr.size[0] = hdr.ಅಡ್ಡಸಾಲುಗಳು = new_ಅಡ್ಡಸಾಲುಗಳು;
        hdr.ಹೆಜ್ಜೆ[0] = ಒಟ್ಟು_ಅಗಲ * elemSize1();
    } ಇಲ್ಲದಿದ್ದರೆ {
        hdr.size[0] = hdr.ಅಡ್ಡಸಾಲುಗಳು = ಅಡ್ಡಸಾಲುಗಳು;
        ಆದರೆ (dims <= 1)
            hdr.ಹೆಜ್ಜೆ[0] = ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ * CV_ELEM_ಗಾತ್ರ(flags);
    }

    ಪೂರ್ಣಾಂಕ new_ಅಗಲ = ಒಟ್ಟು_ಅಗಲ / new_cn;

    ಆದರೆ( new_ಅಗಲ * new_cn != ಒಟ್ಟು_ಅಗಲ )
        CV_Error( cv::Error::Bad_ಅಂಕೆ_Channels,
        "The total width is not divisible by the new number of channels" );

    hdr.size.dims = hdr.dims = 2;
    hdr.size[1] = hdr.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = new_ಅಗಲ;
    hdr.flags = (hdr.flags & ~CV_MAT_CN_ಮುಖವಾಡ) | ((new_cn-1) << CV_CN_ಸರಿಸು);
    hdr.ಹೆಜ್ಜೆ[1] = CV_ELEM_ಗಾತ್ರ(hdr.flags);
    ಹಿಂತಿರುಗಿ hdr;
}

Mat Mat::reshape(ಪೂರ್ಣಾಂಕ _cn, ಪೂರ್ಣಾಂಕ _newndims, ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* _newsz) ಸ್ಥಿರಾಂಕ
{
    ಆದರೆ(_newndims == dims)
    {
        ಆದರೆ(_newsz == 0)
            ಹಿಂತಿರುಗಿ reshape(_cn);
        ಆದರೆ(_newndims == 2)
            ಹಿಂತಿರುಗಿ reshape(_cn, _newsz[0]);
    }

    ಆದರೆ (isContinuous())
    {
        CV_Assert(_cn >= 0 && _newndims >= 0 && _newndims <= CV_MAX_DIM && (_newndims == 0 || _newsz != 0));

        ಆದರೆ (_cn == 0)
            _cn = ಇದು->channels();
        ಇಲ್ಲದಿದ್ದರೆ
            CV_Assert(_cn <= CV_CN_MAX);

        size_t ಒಟ್ಟು_elem1_ref = ಇದು->ಒಟ್ಟು() * ಇದು->channels();
        size_t ಒಟ್ಟು_elem1 = _cn;

        Auto_ಬಫರ್<ಪೂರ್ಣಾಂಕ, 4> newsz_buf( (size_t)_newndims );

        ಪೂರ್ಣಾಂಕ m1_idx = -1;

        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < _newndims; i++)
        {
            ಆದರೆ (_newsz[i] >= 0) {
                ಆದರೆ (_newsz[i] == 0 && i < dims)
                    newsz_buf[i] = size.p[i];
                ಇಲ್ಲದಿದ್ದರೆ
                    newsz_buf[i] = _newsz[i];
                ಒಟ್ಟು_elem1 *= (size_t)newsz_buf[i];
            } ಇಲ್ಲದಿದ್ದರೆ {
                ಆದರೆ (m1_idx >= 0)
                    CV_Error(cv::Error::Sts_Bad_ಗಾತ್ರ, "More than one '-1' occured in the new shape");
                m1_idx = i;
            }
        }

        ಆದರೆ (m1_idx >= 0) {
            ಆದರೆ (ಒಟ್ಟು_elem1 == 0) {
                CV_Assert(ಒಟ್ಟು_elem1_ref == 0);
                ಒಟ್ಟು_elem1 = 1;
            }
            CV_Assert(ಒಟ್ಟು_elem1_ref % ಒಟ್ಟು_elem1 == 0);
            newsz_buf[m1_idx] = (ಪೂರ್ಣಾಂಕ)(ಒಟ್ಟು_elem1_ref / ಒಟ್ಟು_elem1);
        } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಒಟ್ಟು_elem1 != ಒಟ್ಟು_elem1_ref) {
            CV_Error(cv::Error::StsUnmatchedSizes, "Requested and source matrices have different count of elements");
        }

        Mat hdr = *ಇದು;
        hdr.flags = (hdr.flags & ~CV_MAT_CN_ಮುಖವಾಡ) | ((_cn-1) << CV_CN_ಸರಿಸು);
        ಗಣ_ಗಾತ್ರ(hdr, _newndims, newsz_buf.ದತ್ತಾಂಶ(), ಖಾಲಿ, ನಿಜ);

        ಹಿಂತಿರುಗಿ hdr;
    }

    CV_Error(cv::Error::StsNotImplemented, "Reshaping of n-dimensional non-continuous matrices is not supported yet");
    // TBD
}

Mat Mat::reshape(ಪೂರ್ಣಾಂಕ _cn, ಸ್ಥಿರಾಂಕ std::vector<ಪೂರ್ಣಾಂಕ>& _newshape) ಸ್ಥಿರಾಂಕ
{
    ಪೂರ್ಣಾಂಕ newdims = (ಪೂರ್ಣಾಂಕ)_newshape.size();
    ಆದರೆ(newdims == 0 && empty())
    {
        ಹಿಂತಿರುಗಿ *ಇದು;
    }

    ಹಿಂತಿರುಗಿ reshape(_cn, newdims, newdims > 0 ? &_newshape[0] : 0);
}

Mat Mat::reshape(ಪೂರ್ಣಾಂಕ _cn, ಸ್ಥಿರಾಂಕ MatShape& _newshape) ಸ್ಥಿರಾಂಕ
{
    ಆದರೆ (_newshape.dims < 0) {
        ಪೂರ್ಣಾಂಕ newshape[] = {0};
        ಹಿಂತಿರುಗಿ reshape(_cn, 1, newshape);
    }
    ಹಿಂತಿರುಗಿ reshape(_cn, _newshape.dims, _newshape.p);
}

Mat Mat::reshape(ಪೂರ್ಣಾಂಕ _cn, std::initializer_list<ಪೂರ್ಣಾಂಕ> newshape_) ಸ್ಥಿರಾಂಕ
{
    ಪೂರ್ಣಾಂಕ newshape[MatShape::MAX_DIMS];
    size_t i, newshape_dims = newshape_.size();
    CV_Assert(newshape_dims <= (size_t)MatShape::MAX_DIMS);
    ಸ್ವಯಂಚಾಲಿತ it = newshape_.begin();
    ಗಾಗಿ (i = 0; i < newshape_dims; i++, ++it)
        newshape[i] = *it;
    ಹಿಂತಿರುಗಿ reshape(_cn, (ಪೂರ್ಣಾಂಕ)newshape_dims, newshape);
}

Mat Mat::reinterpret(ಪೂರ್ಣಾಂಕ type) ಸ್ಥಿರಾಂಕ
{
    type = CV_MAT_TYPE(type);
    CV_Assert(CV_ELEM_ಗಾತ್ರ(ಇದು->type()) == CV_ELEM_ಗಾತ್ರ(type));
    Mat m = *ಇದು;
    m.flags = (m.flags & ~CV_MAT_TYPE_ಮುಖವಾಡ) | type;
    m.updateContinuityFlag();
    ಹಿಂತಿರುಗಿ m;
}

Mat Mat::diag(ಸ್ಥಿರಾಂಕ Mat& d)
{
    CV_Assert( d.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ == 1 || d.ಅಡ್ಡಸಾಲುಗಳು == 1 );
    ಪೂರ್ಣಾಂಕ len = d.ಅಡ್ಡಸಾಲುಗಳು + d.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ - 1;
    Mat m(len, len, d.type(), Scalar(0));
    Mat md = m.diag();
    ಆದರೆ( d.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ == 1 )
        d.copy_ಗಮ್ಯ(md);
    ಇಲ್ಲದಿದ್ದರೆ
        transpose(d, md);
    ಹಿಂತಿರುಗಿ m;
}

ಪೂರ್ಣಾಂಕ Mat::check_ಸದಿಶ(ಪೂರ್ಣಾಂಕ _elemChannels, ಪೂರ್ಣಾಂಕ ಆಳ_2, ತಾರ್ಕಿಕ _requireContinuous) ಸ್ಥಿರಾಂಕ
{
    ಹಿಂತಿರುಗಿ ದತ್ತಾಂಶ && (ಆಳ() == ಆಳ_2 || ಆಳ_2 <= 0) &&
        (isContinuous() || !_requireContinuous) &&
        ((dims <= 2 && (((ಅಡ್ಡಸಾಲುಗಳು == 1 || ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ == 1) && channels() == _elemChannels) ||
                        (ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ == _elemChannels && channels() == 1))) ||
        (dims == 3 && channels() == 1 && size.p[2] == _elemChannels && (size.p[0] == 1 || size.p[1] == 1) &&
         (isContinuous() || ಹೆಜ್ಜೆ.p[1] == ಹೆಜ್ಜೆ.p[2]*size.p[2])))
    ? (ಪೂರ್ಣಾಂಕ)(ಒಟ್ಟು()*channels()/_elemChannels) : -1;
}


ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಗಾತ್ರ_2 get_Continuous_ಗಾತ್ರ(ಪೂರ್ಣಾಂಕ flags, ಪೂರ್ಣಾಂಕ ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, ಪೂರ್ಣಾಂಕ ಅಡ್ಡಸಾಲುಗಳು, ಪೂರ್ಣಾಂಕ ಅಗಲ_Scale)
{
    int64 sz = (int64)ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ * ಅಡ್ಡಸಾಲುಗಳು * ಅಗಲ_Scale;
    ತಾರ್ಕಿಕ has_int_ಅತಿಪ್ರವಾಹ = sz >= ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ;
    ತಾರ್ಕಿಕ isContiguous = (flags & Mat::CONTINUOUS_FLAG) != 0;
    ಹಿಂತಿರುಗಿ (isContiguous && !has_int_ಅತಿಪ್ರವಾಹ)
            ? ಗಾತ್ರ_2((ಪೂರ್ಣಾಂಕ)sz, 1)
            : ಗಾತ್ರ_2(ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ * ಅಗಲ_Scale, ಅಡ್ಡಸಾಲುಗಳು);
}

ಗಾತ್ರ_2 getContinuousSize2D(Mat& m1, ಪೂರ್ಣಾಂಕ ಅಗಲ_Scale)
{
    CV_CheckLE(m1.dims, 2, "");
    ಹಿಂತಿರುಗಿ get_Continuous_ಗಾತ್ರ(m1.flags,
                              m1.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, m1.ಅಡ್ಡಸಾಲುಗಳು, ಅಗಲ_Scale);
}
ಗಾತ್ರ_2 getContinuousSize2D(Mat& m1, Mat& m2, ಪೂರ್ಣಾಂಕ ಅಗಲ_Scale)
{
    CV_CheckLE(m1.dims, 2, "");
    CV_CheckLE(m2.dims, 2, "");
    ಸ್ಥಿರಾಂಕ ಗಾತ್ರ_2 sz1 = m1.size();
    ಆದರೆ (sz1 != m2.size())  // reshape all matrixes to the same size (#4159)
    {
        size_t ಒಟ್ಟು_sz = m1.ಒಟ್ಟು();
        CV_CheckEQ(ಒಟ್ಟು_sz, m2.ಒಟ್ಟು(), "");
        ತಾರ್ಕಿಕ is_m1_ಸದಿಶ = m1.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ == 1 || m1.ಅಡ್ಡಸಾಲುಗಳು == 1;
        ತಾರ್ಕಿಕ is_m2_ಸದಿಶ = m2.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ == 1 || m2.ಅಡ್ಡಸಾಲುಗಳು == 1;
        CV_Assert(is_m1_ಸದಿಶ); CV_Assert(is_m2_ಸದಿಶ);
        ಪೂರ್ಣಾಂಕ ಒಟ್ಟು = (ಪೂರ್ಣಾಂಕ)ಒಟ್ಟು_sz;  // vector-column
        ತಾರ್ಕಿಕ isContiguous = ((m1.flags & m2.flags) & Mat::CONTINUOUS_FLAG) != 0;
        ತಾರ್ಕಿಕ has_int_ಅತಿಪ್ರವಾಹ = ((int64)ಒಟ್ಟು_sz * ಅಗಲ_Scale) >= ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ;
        ಆದರೆ (isContiguous && !has_int_ಅತಿಪ್ರವಾಹ)
            ಒಟ್ಟು = 1; // vector-row
        m1 = m1.reshape(0, ಒಟ್ಟು);
        m2 = m2.reshape(0, ಒಟ್ಟು);
        CV_Assert(m1.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ == m2.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ && m1.ಅಡ್ಡಸಾಲುಗಳು == m2.ಅಡ್ಡಸಾಲುಗಳು);
        ಹಿಂತಿರುಗಿ ಗಾತ್ರ_2(m1.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ * ಅಗಲ_Scale, m1.ಅಡ್ಡಸಾಲುಗಳು);
    }
    ಹಿಂತಿರುಗಿ get_Continuous_ಗಾತ್ರ(m1.flags & m2.flags,
                              m1.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, m1.ಅಡ್ಡಸಾಲುಗಳು, ಅಗಲ_Scale);
}

ಗಾತ್ರ_2 getContinuousSize2D(Mat& m1, Mat& m2, Mat& m3, ಪೂರ್ಣಾಂಕ ಅಗಲ_Scale)
{
    CV_CheckLE(m1.dims, 2, "");
    CV_CheckLE(m2.dims, 2, "");
    CV_CheckLE(m3.dims, 2, "");
    ಸ್ಥಿರಾಂಕ ಗಾತ್ರ_2 sz1 = m1.size();
    ಆದರೆ (sz1 != m2.size() || sz1 != m3.size())  // reshape all matrixes to the same size (#4159)
    {
        size_t ಒಟ್ಟು_sz = m1.ಒಟ್ಟು();
        CV_CheckEQ(ಒಟ್ಟು_sz, m2.ಒಟ್ಟು(), "");
        CV_CheckEQ(ಒಟ್ಟು_sz, m3.ಒಟ್ಟು(), "");
        ತಾರ್ಕಿಕ is_m1_ಸದಿಶ = m1.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ == 1 || m1.ಅಡ್ಡಸಾಲುಗಳು == 1;
        ತಾರ್ಕಿಕ is_m2_ಸದಿಶ = m2.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ == 1 || m2.ಅಡ್ಡಸಾಲುಗಳು == 1;
        ತಾರ್ಕಿಕ is_m3_ಸದಿಶ = m3.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ == 1 || m3.ಅಡ್ಡಸಾಲುಗಳು == 1;
        CV_Assert(is_m1_ಸದಿಶ); CV_Assert(is_m2_ಸದಿಶ); CV_Assert(is_m3_ಸದಿಶ);
        ಪೂರ್ಣಾಂಕ ಒಟ್ಟು = (ಪೂರ್ಣಾಂಕ)ಒಟ್ಟು_sz;  // vector-column
        ತಾರ್ಕಿಕ isContiguous = ((m1.flags & m2.flags & m3.flags) & Mat::CONTINUOUS_FLAG) != 0;
        ತಾರ್ಕಿಕ has_int_ಅತಿಪ್ರವಾಹ = ((int64)ಒಟ್ಟು_sz * ಅಗಲ_Scale) >= ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ;
        ಆದರೆ (isContiguous && !has_int_ಅತಿಪ್ರವಾಹ)
            ಒಟ್ಟು = 1; // vector-row
        m1 = m1.reshape(0, ಒಟ್ಟು);
        m2 = m2.reshape(0, ಒಟ್ಟು);
        m3 = m3.reshape(0, ಒಟ್ಟು);
        CV_Assert(m1.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ == m2.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ && m1.ಅಡ್ಡಸಾಲುಗಳು == m2.ಅಡ್ಡಸಾಲುಗಳು && m1.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ == m3.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ && m1.ಅಡ್ಡಸಾಲುಗಳು == m3.ಅಡ್ಡಸಾಲುಗಳು);
        ಹಿಂತಿರುಗಿ ಗಾತ್ರ_2(m1.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ * ಅಗಲ_Scale, m1.ಅಡ್ಡಸಾಲುಗಳು);
    }
    ಹಿಂತಿರುಗಿ get_Continuous_ಗಾತ್ರ(m1.flags & m2.flags & m3.flags,
                              m1.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, m1.ಅಡ್ಡಸಾಲುಗಳು, ಅಗಲ_Scale);
}

} // cv::
