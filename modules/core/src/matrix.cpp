// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#ಅನ್ತರ್ಭಾವ "precomp.hpp"
#ಅನ್ತರ್ಭಾವ "bufferpool.impl.hpp"

ನಾಮಾಕಾಶ cv {

std::string layoutToString(DataLayout layout)
{
    ಪ್ರತ್ಯಾಗಮ
        layout == DATA_LAYOUT_ND ? "ND" :
        layout == DATA_LAYOUT_NCHW ? "NCHW" :
        layout == DATA_LAYOUT_NHWC ? "NHWC" :
        layout == DATA_LAYOUT_BLOCK ? "BLOCK" :
        layout == DATA_LAYOUT_NCDHW ? "NCDHW" :
        layout == DATA_LAYOUT_NDHWC ? "NDHWC" :
        layout == DATA_LAYOUT_PLANAR ? "PLANAR" :
        layout == DATA_LAYOUT_UNKNOWN ? "Unknown" : "???";
}

ತಾರ್ಕಿಕ operator == (ಧ್ರುವ MatShape& size1, ಧ್ರುವ MatShape& size2)
{
    ಯದಿ (size1.dims != size2.dims)
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    ಯದಿ (size1.layout != size2.layout &&
        size1.layout != DATA_LAYOUT_UNKNOWN &&
        size2.layout != DATA_LAYOUT_UNKNOWN)
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    ಯದಿ (size1.layout == DATA_LAYOUT_BLOCK &&
        size2.layout == DATA_LAYOUT_BLOCK &&
        size1.C != size2.C)
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < size1.dims; i++) {
        ಯದಿ (size1.p[i] != size2.p[i])
            ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
}

ತಾರ್ಕಿಕ operator != (ಧ್ರುವ MatShape& size1, ಧ್ರುವ MatShape& size2)
{
    ಪ್ರತ್ಯಾಗಮ !(size1 == size2);
}

/////////////////////////// MatShape ////////////////////////////////

MatShape MatShape::scalar()
{
    ಪ್ರತ್ಯಾಗಮ MatShape(0);
}

ಶೂನ್ಯ MatShape::clear()
{
    dims = -1;
    layout = DATA_LAYOUT_UNKNOWN;
    C = 0;
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < MAX_DIMS; i++)
        p[i] = 0;
}

ಶೂನ್ಯ MatShape::resize(size_t newSize, ಪೂರ್ಣಾಙ್ಕ value)
{
    CV_Assert(newSize < (size_t)MAX_DIMS);
    ಪೂರ್ಣಾಙ್ಕ old_dims = std::max(dims, 0);
    dims = (ಪೂರ್ಣಾಙ್ಕ)newSize;
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = old_dims; i < dims; i++)
        p[i] = value;
}

ಶೂನ್ಯ MatShape::reserve(size_t)
{
    // no op; maybe need to add a check for overflow, but we check it anyway in other operations
}

ಶೂನ್ಯ MatShape::assign(size_t newSize, ಪೂರ್ಣಾಙ್ಕ value)
{
    CV_Assert(newSize < (size_t)MAX_DIMS);
    dims = (ಪೂರ್ಣಾಙ್ಕ)newSize;
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < dims; i++)
        p[i] = value;
}

ಶೂನ್ಯ MatShape::assign(ಪೂರ್ಣಾಙ್ಕ newSize, ಪೂರ್ಣಾಙ್ಕ value)
{
    assign((size_t)newSize, value);
}

ಶೂನ್ಯ MatShape::assign(ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ* begin, ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ* end)
{
    assign_(begin, end);
}

ಶೂನ್ಯ MatShape::assign_(ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ* begin, ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ* end)
{
    ptrdiff_t newSize = end - begin;
    CV_Assert(0 <= newSize && newSize < (ptrdiff_t)MAX_DIMS);
    dims = (ಪೂರ್ಣಾಙ್ಕ)newSize;
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < dims; i++)
        p[i] = begin[i];
}

ಪೂರ್ಣಾಙ್ಕ* MatShape::begin() { ಪ್ರತ್ಯಾಗಮ p; }
ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ* MatShape::begin() ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ p; }
ಪೂರ್ಣಾಙ್ಕ* MatShape::end() { ಪ್ರತ್ಯಾಗಮ p + std::max(dims, 0); }
ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ* MatShape::end() ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ p + std::max(dims, 0); }
ಪೂರ್ಣಾಙ್ಕ& MatShape::back() { ಪ್ರತ್ಯಾಗಮ p[std::max(dims-1, 0)]; }
ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ& MatShape::back() ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ p[std::max(dims-1, 0)]; }

ಶೂನ್ಯ MatShape::push_back(ಪೂರ್ಣಾಙ್ಕ value)
{
    CV_Assert(dims+1 < MAX_DIMS);
    dims = std::max(dims+1, 1);
    p[dims-1] = value;
}

ಶೂನ್ಯ MatShape::emplace_back(ಪೂರ್ಣಾಙ್ಕ value)
{
    push_back(value);
}

ಶೂನ್ಯ MatShape::insert(ಪೂರ್ಣಾಙ್ಕ* where, ಪೂರ್ಣಾಙ್ಕ value)
{
    ಪೂರ್ಣಾಙ್ಕ old_dims = std::max(dims, 0);
    CV_Assert(old_dims+1 < MAX_DIMS);
    ptrdiff_t ofs = where - p;
    CV_Assert(0 <= ofs && ofs <= old_dims);
    dims = old_dims+1;
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = old_dims-1; i >= (ಪೂರ್ಣಾಙ್ಕ)ofs; i--)
        p[i+1] = p[i];
    p[ofs] = value;
}

ಶೂನ್ಯ MatShape::insert(ಪೂರ್ಣಾಙ್ಕ* where, size_t ಎಣಿಕೆ_2, ಪೂರ್ಣಾಙ್ಕ value)
{
    ಪೂರ್ಣಾಙ್ಕ old_dims = std::max(dims, 0);
    CV_Assert((size_t)(old_dims+ಎಣಿಕೆ_2) < (size_t)MAX_DIMS);
    ptrdiff_t ofs = where - p;
    CV_Assert(0 <= ofs && ofs <= old_dims);
    dims = (ಪೂರ್ಣಾಙ್ಕ)(old_dims+ಎಣಿಕೆ_2);
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = old_dims-1; i >= (ಪೂರ್ಣಾಙ್ಕ)ofs; i--)
        p[i+ಎಣಿಕೆ_2] = p[i];
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < (ಪೂರ್ಣಾಙ್ಕ)ಎಣಿಕೆ_2; i++)
        p[i+ofs] = value;
}

ಶೂನ್ಯ MatShape::insert(ಪೂರ್ಣಾಙ್ಕ* where, ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ_2, ಪೂರ್ಣಾಙ್ಕ value)
{
    insert(where, (size_t)ಎಣಿಕೆ_2, value);
}

ಶೂನ್ಯ MatShape::insert(ಪೂರ್ಣಾಙ್ಕ* where, ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ* begin, ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ* end)
{
    ಸೇರಿಸು(where, begin, end);
}

ಶೂನ್ಯ MatShape::ಸೇರಿಸು(ಪೂರ್ಣಾಙ್ಕ* where, ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ* begin, ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ* end)
{
    ಪೂರ್ಣಾಙ್ಕ old_dims = std::max(dims, 0);
    ptrdiff_t delta = end - begin;
    CV_Assert(0 <= delta && old_dims+delta < MAX_DIMS);
    ptrdiff_t ofs = where - p;
    CV_Assert(0 <= ofs && ofs <= old_dims);
    dims = (ಪೂರ್ಣಾಙ್ಕ)(old_dims+delta);
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = old_dims-1; i >= (ಪೂರ್ಣಾಙ್ಕ)ofs; i--)
        p[i+delta] = p[i];
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < (ಪೂರ್ಣಾಙ್ಕ)delta; i++)
        p[i+ofs] = begin[i];
}

ಶೂನ್ಯ MatShape::erase(ಪೂರ್ಣಾಙ್ಕ* where)
{
    CV_Assert(dims > 0);
    ptrdiff_t ofs = where - p;
    CV_Assert(0 <= ofs && ofs <= dims);
    ಯದಿ (ofs == dims)
        ಪ್ರತ್ಯಾಗಮ;
    dims--;
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = (ಪೂರ್ಣಾಙ್ಕ)ofs+1; i <= dims; i++)
        p[i-1] = p[i];
}

size_t MatShape::total() ಧ್ರುವ
{
    size_t result = 1;
    ಯದಿ (dims < 0)
        ಪ್ರತ್ಯಾಗಮ 0;
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < dims; i++)
        result *= p[i];
    ಪ್ರತ್ಯಾಗಮ result;
}

std::vector<ಪೂರ್ಣಾಙ್ಕ> MatShape::vec() ಧ್ರುವ
{
    ಯದಿ (dims < 0)
        ಪ್ರತ್ಯಾಗಮ std::vector<ಪೂರ್ಣಾಙ್ಕ>(1, 0);
    ಪ್ರತ್ಯಾಗಮ std::vector<ಪೂರ್ಣಾಙ್ಕ>(p, p + dims);
}

std::string MatShape::str() ಧ್ರುವ
{
    std::stringstream sstrm;
    ಯದಿ (empty()) {
        sstrm << "<empty>";
    } ಅನ್ಯಥಾ ಯದಿ (dims == 0) {
        sstrm << "<scalar>";
    } ಅನ್ಯಥಾ {
        sstrm << "[";
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < dims; i++) {
            sstrm << (i > 0 ? " x " : "") << p[i];
        }
        sstrm << "]";
    }
    ಪ್ರತ್ಯಾಗಮ sstrm.str();
}

ಸ್ಥಿರ ಶೂನ್ಯ finalizeBlockLayout(MatShape& size, ಪೂರ್ಣಾಙ್ಕ C=0)
{
    ಯದಿ (size.layout == DATA_LAYOUT_BLOCK) {
        CV_Assert(size.dims >= 4);
        ಪೂರ್ಣಾಙ್ಕ C0 = size.p[size.dims-1];
        CV_Assert(C0 > 1 && (C0 & (C0-1)) == 0);
        size.C = C > 0 ? C : size.p[1]*size.p[size.dims-1];
    } ಅನ್ಯಥಾ {
        size.C = 0;
    }
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = std::max(size.dims, 0); i < MatShape::MAX_DIMS; i++)
        size.p[i] = 0;
    ಯದಿ (size.dims == 0)
        size.p[0] = 1;
}

MatShape::MatShape()
{
    clear();
}

MatShape::MatShape(size_t dims_, ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ* size_, DataLayout layout_, ಪೂರ್ಣಾಙ್ಕ C_)
{
    layout = layout_;
    CV_Assert(dims_ <= (size_t)MAX_DIMS);
    dims = (ಪೂರ್ಣಾಙ್ಕ)dims_;
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < dims; i++) {
        p[i] = size_ ? size_[i] : 0;
    }
    finalizeBlockLayout(*ಏತತ್, C_);
}

MatShape::MatShape(size_t dims_, ಪೂರ್ಣಾಙ್ಕ value, DataLayout layout_)
{
    layout = layout_;
    CV_Assert(dims_ <= (size_t)MAX_DIMS);
    dims = (ಪೂರ್ಣಾಙ್ಕ)dims_;
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < dims; i++) {
        p[i] = value;
    }
    finalizeBlockLayout(*ಏತತ್, 0);
}

MatShape::MatShape(std::initializer_list<ಪೂರ್ಣಾಙ್ಕ> shape)
{
    layout = DATA_LAYOUT_UNKNOWN;
    CV_Assert(shape.size() <= (size_t)MAX_DIMS);
    dims = (ಪೂರ್ಣಾಙ್ಕ)shape.size();
    auto it = shape.begin();
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < dims; i++, ++it) {
        p[i] = *it;
    }
    finalizeBlockLayout(*ಏತತ್, 0);
}

MatShape::MatShape(ಪೂರ್ಣಾಙ್ಕ dims_, ಪೂರ್ಣಾಙ್ಕ value, DataLayout layout_)
{
    layout = layout_;
    CV_Assert(dims_ <= MAX_DIMS);
    dims = dims_;
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < dims; i++) {
        p[i] = value;
    }
    finalizeBlockLayout(*ಏತತ್, 0);
}

MatShape::MatShape(ಧ್ರುವ std::vector<ಪೂರ್ಣಾಙ್ಕ>& shape_, DataLayout layout_, ಪೂರ್ಣಾಙ್ಕ C_)
{
    layout = layout_;
    size_t shape_size = shape_.size();
    CV_Assert(shape_size < (size_t)MAX_DIMS);
    dims = (ಪೂರ್ಣಾಙ್ಕ)shape_size;
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < dims; i++) {
        p[i] = shape_[i];
    }
    finalizeBlockLayout(*ಏತತ್, C_);
}

MatShape::MatShape(ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ* begin, ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ* end, DataLayout layout_, ಪೂರ್ಣಾಙ್ಕ C_)
{
    layout = layout_;
    ptrdiff_t shape_size = end - begin;
    CV_Assert(0 <= shape_size && shape_size < MAX_DIMS);
    dims = (ಪೂರ್ಣಾಙ್ಕ)shape_size;
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < dims; i++) {
        p[i] = begin[i];
    }
    finalizeBlockLayout(*ಏತತ್, C_);
}

MatShape::MatShape(ಧ್ರುವ MatShape& shape)
{
    dims = shape.dims;
    layout = shape.layout;
    C = shape.C;
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < MAX_DIMS; i++)
        p[i] = shape.p[i];
}

MatShape& MatShape::operator = (ಧ್ರುವ MatShape& shape)
{
    ಯದಿ (ಏತತ್ != &shape) {
        dims = shape.dims;
        layout = shape.layout;
        C = shape.C;
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < MAX_DIMS; i++)
            p[i] = shape.p[i];
    }
    ಪ್ರತ್ಯಾಗಮ *ಏತತ್;
}

ತಾರ್ಕಿಕ MatShape::hasSymbols() ಧ್ರುವ
{
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < dims; i++) {
        ಯದಿ (p[i] < 0)
            ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    }
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
}

ಪೂರ್ಣಾಙ್ಕ MatShape::channels() ಧ್ರುವ
{
    CV_Assert(layout == DATA_LAYOUT_BLOCK || layout == DATA_LAYOUT_NCHW || layout == DATA_LAYOUT_NHWC);
    ಪ್ರತ್ಯಾಗಮ layout == DATA_LAYOUT_BLOCK ? C : p[layout == DATA_LAYOUT_NCHW ? 1 : dims-1];
}

MatShape MatShape::toLayout(DataLayout newLayout, ಪೂರ್ಣಾಙ್ಕ C0) ಧ್ರುವ
{
    CV_Assert(layout == DATA_LAYOUT_BLOCK || layout == DATA_LAYOUT_NCHW || layout == DATA_LAYOUT_NHWC);
    CV_Assert(newLayout == DATA_LAYOUT_BLOCK || newLayout == DATA_LAYOUT_NCHW || newLayout == DATA_LAYOUT_NHWC);

    MatShape newsize = *ಏತತ್;
    newsize.layout = newLayout;

    ಯದಿ (newLayout == DATA_LAYOUT_BLOCK) {
        // any => BLOCK
        CV_Assert_N(C0 > 1, (C0 & (C0-1)) == 0);
        ಪೂರ್ಣಾಙ್ಕ Corig = channels();
        newsize.C = Corig;

        ಯದಿ (layout == DATA_LAYOUT_NHWC) {
            ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 2; i < dims; i++) {
                newsize.p[i] = p[i-1];
            }
        }

        newsize.dims += layout != DATA_LAYOUT_BLOCK;
        newsize.p[1] = (Corig + C0 - 1)/C0;
        newsize.p[newsize.dims-1] = C0;
    } ಅನ್ಯಥಾ ಯದಿ (layout == DATA_LAYOUT_BLOCK) {
        // BLOCK => any (except for BLOCK, which is handled above)
        newsize.C = 0;
        ಯದಿ (newLayout == DATA_LAYOUT_NHWC) {
            ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 2; i < dims; i++) {
                newsize.p[i-1] = p[i];
            }
        }
        newsize.p[newLayout == DATA_LAYOUT_NCHW ? 1 : dims-2] = C;
        newsize.dims--;
    } ಅನ್ಯಥಾ {
        CV_Assert_N(C0 <= 1);

        ಯದಿ (newLayout == layout)
            ಪ್ರತ್ಯಾಗಮ newsize;

        // NHWC => NCHW
        ಯದಿ (newLayout == DATA_LAYOUT_NCHW) {
            ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 2; i < dims; i++)
                newsize.p[i] = p[i-1];
            newsize.p[1] = p[dims-1];
        } ಅನ್ಯಥಾ {
            // NCHW => NHWC
            ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 2; i < dims; i++)
                newsize.p[i-1] = p[i];
            newsize.p[dims-1] = p[1];
        }
    }

    ಪ್ರತ್ಯಾಗಮ newsize;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ normalizeShapeAxis(ಪೂರ್ಣಾಙ್ಕ axis, ಪೂರ್ಣಾಙ್ಕ dims)
{
    CV_CheckGE(axis, -dims, "axis is out of range");
    CV_CheckLT(axis, dims, "axis is out of range");
    ಪ್ರತ್ಯಾಗಮ axis < 0 ? axis + dims : axis;
}

// The semantic NCHW form {N, C, spatial...} of a block ('NC1HWC0') shape, so that axes and
// element counts are the logical ones rather than the C0-padded ones.
ಸ್ಥಿರ MatShape toSemantic(ಧ್ರುವ MatShape& shape)
{
    CV_CheckGE(shape.dims, 3, "a block shape is {N, C1, spatial..., C0}");
    ಪ್ರತ್ಯಾಗಮ shape.toLayout(DATA_LAYOUT_NCHW);
}

// Blocks such a result again. Once N or C changes, elements cross the padded C0 lanes and
// the data needs a repack, which a shape operation cannot express.
ಸ್ಥಿರ MatShape toBlockOf(MatShape result, ಧ್ರುವ MatShape& src, ಧ್ರುವ ವರ್ಣ* op)
{
    ಯದಿ (result.dims < 2 || result.p[0] != src.p[0] || result.p[1] != src.C)
        CV_Error_(Error::StsNotImplemented,
                  ("%s: the block layout is kept only when the batch and the channel axes stay "
                   "unchanged; convert the shape with toLayout() first", op));
    CV_CheckLE(result.dims + 1, (ಪೂರ್ಣಾಙ್ಕ)MatShape::MAX_DIMS, "too many dimensions");
    result.layout = DATA_LAYOUT_NCHW;
    ಪ್ರತ್ಯಾಗಮ result.toLayout(DATA_LAYOUT_BLOCK, src.p[src.dims-1]);
}

MatShape MatShape::squeeze(ಧ್ರುವ std::vector<ಪೂರ್ಣಾಙ್ಕ>& axes) ಧ್ರುವ
{
    CV_Assert(dims >= 0);
    ಯದಿ (layout == DATA_LAYOUT_BLOCK)
        ಪ್ರತ್ಯಾಗಮ toBlockOf(toSemantic(*ಏತತ್).squeeze(axes), *ಏತತ್, "squeeze");
    ತಾರ್ಕಿಕ remove[MAX_DIMS] = {ಅಸತ್ಯ};
    ಯದಿ (axes.empty())
    {
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < dims; i++)
            remove[i] = p[i] == 1;
    }
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ a : axes)
    {
        ಪೂರ್ಣಾಙ್ಕ i = normalizeShapeAxis(a, dims);
        CV_Check(a, !remove[i], "squeeze: duplicate axis");
        CV_Check(p[i], p[i] == 1, "squeeze: the size of a squeezed axis must be 1");
        remove[i] = ಸತ್ಯ;
    }
    // a new number of axes: the layout of this shape does not apply
    MatShape result;
    result.dims = 0;
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < dims; i++)
        ಯದಿ (!remove[i])
            result.p[result.dims++] = p[i];
    ಪ್ರತ್ಯಾಗಮ result;
}

MatShape MatShape::unsqueeze(ಧ್ರುವ std::vector<ಪೂರ್ಣಾಙ್ಕ>& axes) ಧ್ರುವ
{
    CV_Assert(dims >= 0);
    ಯದಿ (layout == DATA_LAYOUT_BLOCK)
        ಪ್ರತ್ಯಾಗಮ toBlockOf(toSemantic(*ಏತತ್).unsqueeze(axes), *ಏತತ್, "unsqueeze");
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ outDims = dims + (ಪೂರ್ಣಾಙ್ಕ)axes.size();
    CV_CheckLE(outDims, (ಪೂರ್ಣಾಙ್ಕ)MAX_DIMS, "unsqueeze: too many dimensions");
    ತಾರ್ಕಿಕ insert[MAX_DIMS] = {ಅಸತ್ಯ};
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ a : axes)
    {
        ಪೂರ್ಣಾಙ್ಕ i = normalizeShapeAxis(a, outDims);
        CV_Check(a, !insert[i], "unsqueeze: duplicate axis");
        insert[i] = ಸತ್ಯ;
    }
    // a new number of axes: the layout of this shape does not apply
    MatShape result;
    result.dims = outDims;
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0, j = 0; i < outDims; i++)
        result.p[i] = insert[i] ? 1 : p[j++];
    ಪ್ರತ್ಯಾಗಮ result;
}

MatShape MatShape::flatten(ಪೂರ್ಣಾಙ್ಕ startAxis, ಪೂರ್ಣಾಙ್ಕ endAxis) ಧ್ರುವ
{
    CV_Assert(dims >= 0);
    ಯದಿ (layout == DATA_LAYOUT_BLOCK)
        ಪ್ರತ್ಯಾಗಮ toBlockOf(toSemantic(*ಏತತ್).flatten(startAxis, endAxis), *ಏತತ್, "flatten");
    ಯದಿ (dims == 0)
    {
        CV_Check(startAxis, startAxis == 0 || startAxis == -1, "flatten: axis is out of range");
        CV_Check(endAxis, endAxis == 0 || endAxis == -1, "flatten: axis is out of range");
        ಪ್ರತ್ಯಾಗಮ MatShape({1});
    }
    ಪೂರ್ಣಾಙ್ಕ start = normalizeShapeAxis(startAxis, dims), end = normalizeShapeAxis(endAxis, dims);
    CV_CheckLE(start, end, "flatten: startAxis must not follow endAxis");
    int64 merged = 1;
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = start; i <= end; i++)
        merged *= p[i];
    CV_CheckLE((ದ್ವಿಗುಣ)merged, (ದ್ವಿಗುಣ)INT_MAX, "flatten: the result is too big");
    // a new number of axes: the layout of this shape does not apply
    MatShape result;
    result.dims = 0;
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < dims; i++)
    {
        ಯದಿ (i == start)
            result.p[result.dims++] = (ಪೂರ್ಣಾಙ್ಕ)merged;
        ಅನ್ಯಥಾ ಯದಿ (i < start || i > end)
            result.p[result.dims++] = p[i];
    }
    ಪ್ರತ್ಯಾಗಮ result;
}

MatShape MatShape::reshape(ಧ್ರುವ MatShape& newShape, ತಾರ್ಕಿಕ allowZero) ಧ್ರುವ
{
    CV_Assert(dims >= 0 && newShape.dims >= 0);
    ಯದಿ (layout == DATA_LAYOUT_BLOCK)
        ಪ್ರತ್ಯಾಗಮ toBlockOf(toSemantic(*ಏತತ್).reshape(newShape, allowZero), *ಏತತ್, "reshape");
    // a new number of axes: neither the layout of this shape nor the one of the spec applies
    MatShape result;
    result.dims = newShape.dims;
    std::copy(newShape.p, newShape.p + newShape.dims, result.p);
    ಪೂರ್ಣಾಙ್ಕ inferred = -1;
    int64 known = 1;
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < result.dims; i++)
    {
        ಪೂರ್ಣಾಙ್ಕ& sz = result.p[i];
        ಯದಿ (sz == -1)
        {
            CV_Check(i, inferred < 0, "reshape: at most one axis may be -1");
            inferred = i;
            ಅನುವರ್ತ;
        }
        CV_Check(sz, sz >= 0, "reshape: sizes must be -1 or non-negative");
        ಯದಿ (sz == 0 && !allowZero)
        {
            CV_Check(i, i < dims, "reshape: 0 refers to an axis the source does not have");
            sz = p[i];
        }
        known *= sz;
    }
    ಧ್ರುವ int64 total_ = (int64)total();
    ಯದಿ (inferred >= 0)
    {
        CV_Check((ದ್ವಿಗುಣ)known, known != 0, "reshape: cannot infer the -1 axis next to a zero-size axis");
        CV_Check((ದ್ವಿಗುಣ)total_, total_ % known == 0, "reshape: the number of elements does not match");
        CV_CheckLE((ದ್ವಿಗುಣ)(total_/known), (ದ್ವಿಗುಣ)INT_MAX, "reshape: the result is too big");
        result.p[inferred] = (ಪೂರ್ಣಾಙ್ಕ)(total_/known);
    }
    ಅನ್ಯಥಾ ಯದಿ (known != total_)
        CV_Error_(Error::StsBadArg, ("reshape: the number of elements does not match: %lld vs %lld",
                                     (long long)known, (long long)total_));
    ಪ್ರತ್ಯಾಗಮ result;
}

MatShape MatShape::expand(ಧ್ರುವ MatShape& another) ಧ್ರುವ
{
    ಯದಿ (dims == 0)
        ಪ್ರತ್ಯಾಗಮ another;
    ಯದಿ (another.dims == 0)
        ಪ್ರತ್ಯಾಗಮ *ಏತತ್;

    ಯದಿ ((layout == DATA_LAYOUT_NCHW || layout == DATA_LAYOUT_NHWC) &&
        (another.layout == DATA_LAYOUT_NCHW || another.layout == DATA_LAYOUT_NHWC)) {
        CV_Assert(layout == another.layout);
    }
    // [TODO] support block layout
    CV_Assert(layout != DATA_LAYOUT_BLOCK && another.layout != DATA_LAYOUT_BLOCK);

    MatShape result;

    ಯದಿ (dims < 0 || another.dims < 0)
        ಪ್ರತ್ಯಾಗಮ result;

    result = *ಏತತ್;
    result.dims = std::max(dims, another.dims);
    result.layout = layout == DATA_LAYOUT_UNKNOWN ? another.layout :
        layout == DATA_LAYOUT_ND && (another.layout == DATA_LAYOUT_NCHW ||
        another.layout == DATA_LAYOUT_NHWC) ? another.layout : layout;
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = result.dims-1; i >= 0; i--) {
        ಪೂರ್ಣಾಙ್ಕ i1 = i - (result.dims - dims);
        ಪೂರ್ಣಾಙ್ಕ i2 = i - (result.dims - another.dims);
        ಪೂರ್ಣಾಙ್ಕ sz1 = i1 < 0 ? 1 : p[i1];
        ಪೂರ್ಣಾಙ್ಕ sz2 = i2 < 0 ? 1 : another.p[i2];
        CV_Assert(sz1 == sz2 || sz1 == 1 || sz2 == 1);
        // [TODO] handle symbolic shapes
        result.p[i] = sz1 != 1 ? sz1 : sz2;
    }
    ಪ್ರತ್ಯಾಗಮ result;
}

/////////////////////////// MatAllocator ////////////////////////////

ಶೂನ್ಯ MatAllocator::map(UMatData*, AccessFlag) ಧ್ರುವ
{
}

ಶೂನ್ಯ MatAllocator::unmap(UMatData* u) ಧ್ರುವ
{
    ಯದಿ(u->urefcount == 0 && u->refcount == 0)
    {
        deallocate(u);
    }
}

ಶೂನ್ಯ MatAllocator::download(UMatData* u, ಶೂನ್ಯ* dstptr,
         ಪೂರ್ಣಾಙ್ಕ dims, ಧ್ರುವ size_t sz[],
         ಧ್ರುವ size_t srcofs[], ಧ್ರುವ size_t srcstep[],
         ಧ್ರುವ size_t dststep[]) ಧ್ರುವ
{
    ಯದಿ(!u)
        ಪ್ರತ್ಯಾಗಮ;
    ಪೂರ್ಣಾಙ್ಕ isz[CV_MAX_DIM];
    uchar* srcptr = u->data;
    ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ i = 0; i < dims; i++ )
    {
        CV_Assert( sz[i] <= (size_t)INT_MAX );
        ಯದಿ( sz[i] == 0 )
            ಪ್ರತ್ಯಾಗಮ;
        ಯದಿ( srcofs )
            srcptr += srcofs[i]*(i <= dims-2 ? srcstep[i] : 1);
        isz[i] = (ಪೂರ್ಣಾಙ್ಕ)sz[i];
    }

    Mat src(dims, isz, CV_8U, srcptr, srcstep);
    Mat dst(dims, isz, CV_8U, dstptr, dststep);

    ಧ್ರುವ Mat* arrays[] = { &src, &dst };
    uchar* ptrs[2];
    NAryMatIterator it(arrays, ptrs, 2);
    size_t planesz = it.size;

    ಪ್ರತಿ( size_t j = 0; j < it.nplanes; j++, ++it )
        memcpy(ptrs[1], ptrs[0], planesz);
}


ಶೂನ್ಯ MatAllocator::upload(UMatData* u, ಧ್ರುವ ಶೂನ್ಯ* srcptr, ಪೂರ್ಣಾಙ್ಕ dims, ಧ್ರುವ size_t sz[],
                    ಧ್ರುವ size_t dstofs[], ಧ್ರುವ size_t dststep[],
                    ಧ್ರುವ size_t srcstep[]) ಧ್ರುವ
{
    ಯದಿ(!u)
        ಪ್ರತ್ಯಾಗಮ;
    ಪೂರ್ಣಾಙ್ಕ isz[CV_MAX_DIM];
    uchar* dstptr = u->data;
    ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ i = 0; i < dims; i++ )
    {
        CV_Assert( sz[i] <= (size_t)INT_MAX );
        ಯದಿ( sz[i] == 0 )
            ಪ್ರತ್ಯಾಗಮ;
        ಯದಿ( dstofs )
            dstptr += dstofs[i]*(i <= dims-2 ? dststep[i] : 1);
        isz[i] = (ಪೂರ್ಣಾಙ್ಕ)sz[i];
    }

    Mat src(dims, isz, CV_8U, (ಶೂನ್ಯ*)srcptr, srcstep);
    Mat dst(dims, isz, CV_8U, dstptr, dststep);

    ಧ್ರುವ Mat* arrays[] = { &src, &dst };
    uchar* ptrs[2];
    NAryMatIterator it(arrays, ptrs, 2);
    size_t planesz = it.size;

    ಪ್ರತಿ( size_t j = 0; j < it.nplanes; j++, ++it )
        memcpy(ptrs[1], ptrs[0], planesz);
}

ಶೂನ್ಯ MatAllocator::copy(UMatData* usrc, UMatData* udst, ಪೂರ್ಣಾಙ್ಕ dims, ಧ್ರುವ size_t sz[],
                  ಧ್ರುವ size_t srcofs[], ಧ್ರುವ size_t srcstep[],
                  ಧ್ರುವ size_t dstofs[], ಧ್ರುವ size_t dststep[], ತಾರ್ಕಿಕ /*sync*/) ಧ್ರುವ
{
    CV_INSTRUMENT_REGION();

    ಯದಿ(!usrc || !udst)
        ಪ್ರತ್ಯಾಗಮ;
    ಪೂರ್ಣಾಙ್ಕ isz[CV_MAX_DIM];
    uchar* srcptr = usrc->data;
    uchar* dstptr = udst->data;
    ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ i = 0; i < dims; i++ )
    {
        CV_Assert( sz[i] <= (size_t)INT_MAX );
        ಯದಿ( sz[i] == 0 )
            ಪ್ರತ್ಯಾಗಮ;
        ಯದಿ( srcofs )
            srcptr += srcofs[i]*(i <= dims-2 ? srcstep[i] : 1);
        ಯದಿ( dstofs )
            dstptr += dstofs[i]*(i <= dims-2 ? dststep[i] : 1);
        isz[i] = (ಪೂರ್ಣಾಙ್ಕ)sz[i];
    }

    Mat src(dims, isz, CV_8U, srcptr, srcstep);
    Mat dst(dims, isz, CV_8U, dstptr, dststep);

    ಧ್ರುವ Mat* arrays[] = { &src, &dst };
    uchar* ptrs[2];
    NAryMatIterator it(arrays, ptrs, 2);
    size_t planesz = it.size;

    ಪ್ರತಿ( size_t j = 0; j < it.nplanes; j++, ++it )
        memcpy(ptrs[1], ptrs[0], planesz);
}

BufferPoolController* MatAllocator::getBufferPoolController(ಧ್ರುವ ವರ್ಣ* id) ಧ್ರುವ
{
    CV_UNUSED(id);
    ಸ್ಥಿರ DummyBufferPoolController dummy;
    ಪ್ರತ್ಯಾಗಮ &dummy;
}

ವರ್ಗ StdMatAllocator CV_FINAL : ಸಾರ್ವಜನಿಕ MatAllocator
{
ಸಾರ್ವಜನಿಕ:
    UMatData* allocate(ಪೂರ್ಣಾಙ್ಕ dims, ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ* sizes, ಪೂರ್ಣಾಙ್ಕ type,
                       ಶೂನ್ಯ* data0, size_t* step, AccessFlag /*flags*/, UMatUsageFlags /*usageFlags*/) ಧ್ರುವ CV_OVERRIDE
    {
        size_t total = CV_ELEM_SIZE(type);
        ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ i = dims-1; i >= 0; i-- )
        {
            ಯದಿ( step )
            {
                ಯದಿ( data0 && step[i] != CV_AUTOSTEP )
                {
                    CV_Assert(total <= step[i]);
                    total = step[i];
                }
                ಅನ್ಯಥಾ
                    step[i] = total;
            }
            total *= sizes[i];
        }
        uchar* data = data0 ? (uchar*)data0 : (uchar*)fastMalloc(total);
        UMatData* u = ನೂತನ UMatData(ಏತತ್);
        u->data = u->origdata = data;
        u->size = total;
        ಯದಿ(data0)
            u->flags |= UMatData::USER_ALLOCATED;

        ಪ್ರತ್ಯಾಗಮ u;
    }

    ತಾರ್ಕಿಕ allocate(UMatData* u, AccessFlag /*accessFlags*/, UMatUsageFlags /*usageFlags*/) ಧ್ರುವ CV_OVERRIDE
    {
        ಯದಿ(!u) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
        ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    }

    ಶೂನ್ಯ deallocate(UMatData* u) ಧ್ರುವ CV_OVERRIDE
    {
        ಯದಿ(!u)
            ಪ್ರತ್ಯಾಗಮ;

        CV_Assert(u->urefcount == 0);
        CV_Assert(u->refcount == 0);
        ಯದಿ( !(u->flags & UMatData::USER_ALLOCATED) )
        {
            fastFree(u->origdata);
            u->origdata = 0;
        }
        delete u;
    }
};

ಸ್ಥಿರ
MatAllocator*& getDefaultAllocatorMatRef()
{
    ಸ್ಥಿರ MatAllocator* g_matAllocator = Mat::getStdAllocator();
    ಪ್ರತ್ಯಾಗಮ g_matAllocator;
}

MatAllocator* Mat::getDefaultAllocator()
{
    ಪ್ರತ್ಯಾಗಮ getDefaultAllocatorMatRef();
}

ಶೂನ್ಯ Mat::setDefaultAllocator(MatAllocator* allocator)
{
    getDefaultAllocatorMatRef() = allocator;
}

MatAllocator* Mat::getStdAllocator()
{
    CV_SINGLETON_LAZY_INIT(MatAllocator, ನೂತನ StdMatAllocator())
}

//==================================================================================================

ಶೂನ್ಯ setSize( Mat& m, ಪೂರ್ಣಾಙ್ಕ _dims, ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ* _sz, ಧ್ರುವ size_t* _steps, ತಾರ್ಕಿಕ autoSteps)
{
    CV_Assert( 0 <= _dims && _dims <= CV_MAX_DIM && _dims <= MatShape::MAX_DIMS);

    m.dims = _dims;
    m.size = MatShape(_dims, _sz);
    m.step[std::max(_dims-1, 0)] = CV_ELEM_SIZE(m.flags);
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = _dims-2; i >= 0; i--) {
        size_t autostep = m.size[i+1]*m.step[i+1];
        ಯದಿ (_steps) {
            m.step[i] = _steps[i];
            //CV_Assert(m.step[i] >= autostep);
        } ಅನ್ಯಥಾ ಯದಿ (autoSteps) {
            m.step[i] = autostep;
        } ಅನ್ಯಥಾ {
            m.step[i] = 0;
        }
    }

    ಯದಿ( _dims <= 2 )
    {
        m.cols = _dims == 0 ? 1 : _sz ? _sz[_dims > 1] : 0;
        m.rows = _dims < 2 ? 1 : _sz ? _sz[0] : 0;
    } ಅನ್ಯಥಾ {
        m.cols = m.rows = -1;
    }
}

ಪೂರ್ಣಾಙ್ಕ updateContinuityFlag(ಪೂರ್ಣಾಙ್ಕ flags, ಪೂರ್ಣಾಙ್ಕ dims, ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ* size, ಧ್ರುವ size_t* step)
{
    ಪೂರ್ಣಾಙ್ಕ i, j;
    ಯದಿ (dims <= 1)
        ಪ್ರತ್ಯಾಗಮ flags | Mat::CONTINUOUS_FLAG;
    ಪ್ರತಿ( i = 0; i < dims; i++ )
    {
        ಯದಿ( size[i] > 1 )
            ಭಙ್ಗ;
    }

    uint64 t = (uint64)size[std::min(i, dims-1)]*CV_MAT_CN(flags);
    ಪ್ರತಿ( j = dims-1; j > i; j-- )
    {
        t *= size[j];
        ಯದಿ( step[j]*size[j] < step[j-1] )
            ಭಙ್ಗ;
    }

    ಯದಿ( j <= i && t == (uint64)(ಪೂರ್ಣಾಙ್ಕ)t )
        ಪ್ರತ್ಯಾಗಮ flags | Mat::CONTINUOUS_FLAG;
    ಪ್ರತ್ಯಾಗಮ flags & ~Mat::CONTINUOUS_FLAG;
}

ಶೂನ್ಯ Mat::updateContinuityFlag()
{
    flags = cv::updateContinuityFlag(flags, dims, size.p, step.p);
}

ಶೂನ್ಯ finalizeHdr(Mat& m)
{
    m.updateContinuityFlag();
    ಪೂರ್ಣಾಙ್ಕ d = m.dims;
    ಯದಿ( d > 2 )
        m.rows = m.cols = -1;
    ಯದಿ(m.u)
        m.datastart = m.data = m.u->data;
    ಯದಿ( m.data && d > 0 )
    {
        m.datalimit = m.datastart + m.size[0]*m.step[0];
        ಯದಿ( m.size[0] > 0 )
        {
            ಪೂರ್ಣಾಙ್ಕ lastdim = d > 0 ? d - 1 : 0;
            m.dataend = m.ptr() + m.size[lastdim]*m.step[lastdim];
            ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ i = 0; i < d-1; i++ )
                m.dataend += (m.size[i] - 1)*m.step[i];
        }
        ಅನ್ಯಥಾ
            m.dataend = m.datalimit;
    }
    ಅನ್ಯಥಾ
        m.dataend = m.datalimit = m.data;
}

//======================================= Mat ======================================================

Mat::Mat() CV_NOEXCEPT
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{}

Mat::Mat(ಪೂರ್ಣಾಙ್ಕ _rows, ಪೂರ್ಣಾಙ್ಕ _cols, ಪೂರ್ಣಾಙ್ಕ _type)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_rows, _cols, _type);
}

Mat::Mat(ಪೂರ್ಣಾಙ್ಕ _rows, ಪೂರ್ಣಾಙ್ಕ _cols, ಪೂರ್ಣಾಙ್ಕ _type, ಧ್ರುವ Scalar& _s)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_rows, _cols, _type);
    *ಏತತ್ = _s;
}

Mat::Mat(Size _sz, ಪೂರ್ಣಾಙ್ಕ _type)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create( _sz.height, _sz.width, _type );
}

Mat::Mat(Size _sz, ಪೂರ್ಣಾಙ್ಕ _type, ಧ್ರುವ Scalar& _s)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_sz.height, _sz.width, _type);
    *ಏತತ್ = _s;
}

Mat::Mat(ಪೂರ್ಣಾಙ್ಕ _dims, ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ* _sz, ಪೂರ್ಣಾಙ್ಕ _type)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_dims, _sz, _type);
}

Mat::Mat(ಪೂರ್ಣಾಙ್ಕ _dims, ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ* _sz, ಪೂರ್ಣಾಙ್ಕ _type, ಧ್ರುವ Scalar& _s)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_dims, _sz, _type);
    *ಏತತ್ = _s;
}

Mat::Mat(ಧ್ರುವ std::vector<ಪೂರ್ಣಾಙ್ಕ>& _sz, ಪೂರ್ಣಾಙ್ಕ _type)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_sz, _type);
}

Mat::Mat(ಧ್ರುವ std::vector<ಪೂರ್ಣಾಙ್ಕ>& _sz, ಪೂರ್ಣಾಙ್ಕ _type, ಧ್ರುವ Scalar& _s)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_sz, _type);
    *ಏತತ್ = _s;
}

Mat::Mat(ಧ್ರುವ MatShape& _shape, ಪೂರ್ಣಾಙ್ಕ _type)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_shape, _type);
}

Mat::Mat(std::initializer_list<ಪೂರ್ಣಾಙ್ಕ> _shape, ಪೂರ್ಣಾಙ್ಕ _type)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_shape, _type);
}

Mat::Mat(ಧ್ರುವ MatShape& _shape, ಪೂರ್ಣಾಙ್ಕ _type, ಧ್ರುವ Scalar& _s)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_shape, _type);
    *ಏತತ್ = _s;
}

Mat::Mat(std::initializer_list<ಪೂರ್ಣಾಙ್ಕ> _shape, ಪೂರ್ಣಾಙ್ಕ _type, ಧ್ರುವ Scalar& _s)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_shape, _type);
    *ಏತತ್ = _s;
}

Mat::Mat(ಧ್ರುವ Mat& m)
    : flags(m.flags), dims(m.dims), rows(m.rows), cols(m.cols), data(m.data),
      datastart(m.datastart), dataend(m.dataend), datalimit(m.datalimit), allocator(m.allocator),
      u(m.u), size(m.size), step(m.step)
{
    ಯದಿ( u )
        CV_XADD(&u->refcount, 1);
}

Mat::Mat(ಪೂರ್ಣಾಙ್ಕ _rows, ಪೂರ್ಣಾಙ್ಕ _cols, ಪೂರ್ಣಾಙ್ಕ _type, ಶೂನ್ಯ* _data, size_t _step)
    : flags(MAGIC_VAL + (_type & TYPE_MASK)), dims(2), rows(_rows), cols(_cols),
      data((uchar*)_data), datastart((uchar*)_data), dataend(0), datalimit(0),
      allocator(0), u(0), size(2)
{
    CV_Assert(total() == 0 || data != ರಿಕ್ತ);

    size_t esz = CV_ELEM_SIZE(_type), esz1 = CV_ELEM_SIZE1(_type);
    size_t minstep = cols * esz;
    ಯದಿ( _step == AUTO_STEP )
    {
        _step = minstep;
    }
    ಅನ್ಯಥಾ
    {
        CV_Assert( _step >= minstep );
        ಯದಿ (_step % esz1 != 0)
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

Mat::Mat(Size _sz, ಪೂರ್ಣಾಙ್ಕ _type, ಶೂನ್ಯ* _data, size_t _step)
    : flags(MAGIC_VAL + (_type & TYPE_MASK)), dims(2), rows(_sz.height), cols(_sz.width),
      data((uchar*)_data), datastart((uchar*)_data), dataend(0), datalimit(0),
      allocator(0), u(0), size(2)
{
    CV_Assert(total() == 0 || data != ರಿಕ್ತ);

    size_t esz = CV_ELEM_SIZE(_type), esz1 = CV_ELEM_SIZE1(_type);
    size_t minstep = cols*esz;
    ಯದಿ( _step == AUTO_STEP || (_step < minstep && rows == 1))
    {
        _step = minstep;
    }
    ಅನ್ಯಥಾ
    {
        CV_CheckGE(_step, minstep, "");

        ಯದಿ (_step % esz1 != 0)
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

Mat& Mat::operator = (ಧ್ರುವ Mat& m)
{
    ಯದಿ( ಏತತ್ != &m )
    {
        ಯದಿ( m.u )
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
    ಪ್ರತ್ಯಾಗಮ *ಏತತ್;
}

Mat Mat::clone() ಧ್ರುವ
{
    Mat m;
    copyTo(m);
    ಪ್ರತ್ಯಾಗಮ m;
}

ಶೂನ್ಯ Mat::assignTo( Mat& m, ಪೂರ್ಣಾಙ್ಕ _type ) ಧ್ರುವ
{
    ಯದಿ( _type < 0 )
        m = *ಏತತ್;
    ಅನ್ಯಥಾ
        convertTo(m, _type);
}

ಶೂನ್ಯ Mat::create(ಪೂರ್ಣಾಙ್ಕ _rows, ಪೂರ್ಣಾಙ್ಕ _cols, ಪೂರ್ಣಾಙ್ಕ _type)
{
    _type &= TYPE_MASK;
    ಯದಿ( dims <= 2 && rows == _rows && cols == _cols && type() == _type && data )
        ಪ್ರತ್ಯಾಗಮ;
    ಪೂರ್ಣಾಙ್ಕ sz[] = {_rows, _cols};
    create(2, sz, _type);
}

ಶೂನ್ಯ Mat::create(Size _sz, ಪೂರ್ಣಾಙ್ಕ _type)
{
    create(_sz.height, _sz.width, _type);
}

ಶೂನ್ಯ Mat::createSameSize(ಒಳಹರಿವು_Array m, ಪೂರ್ಣಾಙ್ಕ type)
{
    MatShape msize = m.shape();
    create(msize, type);
}

ಶೂನ್ಯ Mat::fit(ಪೂರ್ಣಾಙ್ಕ _dims, ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ* _sizes, ಪೂರ್ಣಾಙ್ಕ _type)
{
    size_t oldTotalBytes = u ? u->size : 0;
    size_t esz = CV_ELEM_SIZE(_type), newTotal = _dims >= 0;
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < _dims; i++)
        newTotal *= _sizes[i];
    size_t newTotalBytes = newTotal*esz;
    ಯದಿ (newTotalBytes > 0 && (!isContinuous() ||
                              newTotalBytes > oldTotalBytes ||
                              data != datastart)) {
        create(_dims, _sizes, _type);
    } ಅನ್ಯಥಾ {
        flags = (flags & ~Mat::TYPE_MASK) | CV_MAT_TYPE(_type);
        ಪೂರ್ಣಾಙ್ಕ _dummy_size = 0;
        setSize(*ಏತತ್, (_dims >= 0 ? _dims : 1), (_dims >= 0 ? _sizes : &_dummy_size), nullptr, ಸತ್ಯ);
        finalizeHdr(*ಏತತ್);
    }
}

ಶೂನ್ಯ Mat::fit(ಧ್ರುವ std::vector<ಪೂರ್ಣಾಙ್ಕ>& _shape, ಪೂರ್ಣಾಙ್ಕ _type)
{
    fit((ಪೂರ್ಣಾಙ್ಕ)_shape.size(), _shape.data(), _type);
}

ಶೂನ್ಯ Mat::fit(ಧ್ರುವ MatShape& _shape, ಪೂರ್ಣಾಙ್ಕ _type)
{
    fit(_shape.dims, _shape.p, _type);
    size.layout = _shape.layout;
    size.C = _shape.C;
}

ಶೂನ್ಯ Mat::fit(std::initializer_list<ಪೂರ್ಣಾಙ್ಕ> _shape, ಪೂರ್ಣಾಙ್ಕ _type)
{
    ಪೂರ್ಣಾಙ್ಕ new_shape[MatShape::MAX_DIMS];
    ಪೂರ್ಣಾಙ್ಕ new_ndims = (ಪೂರ್ಣಾಙ್ಕ)_shape.size();
    CV_Assert(new_ndims <= MatShape::MAX_DIMS);
    auto it = _shape.begin();
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < new_ndims; i++, ++it)
        new_shape[i] = *it;
    fit(new_ndims, new_shape, _type);
}

ಶೂನ್ಯ Mat::fit(ಪೂರ್ಣಾಙ್ಕ _rows, ಪೂರ್ಣಾಙ್ಕ _cols, ಪೂರ್ಣಾಙ್ಕ _type)
{
    _type &= TYPE_MASK;
    ಪೂರ್ಣಾಙ್ಕ sz[] = {_rows, _cols};
    fit(2, sz, _type);
}

ಶೂನ್ಯ Mat::fit(Size _sz, ಪೂರ್ಣಾಙ್ಕ _type)
{
    fit(_sz.height, _sz.width, _type);
}

ಶೂನ್ಯ Mat::fitSameSize(ಒಳಹರಿವು_Array m, ಪೂರ್ಣಾಙ್ಕ _type)
{
    fit(m.shape(), _type);
}

ಶೂನ್ಯ Mat::addref()
{
    ಯದಿ( u )
        CV_XADD(&u->refcount, 1);
}

ಶೂನ್ಯ Mat::release()
{
    ಯದಿ( u && CV_XADD(&u->refcount, -1) == 1 )
        deallocate();
    u = ರಿಕ್ತ;
    datastart = dataend = datalimit = data = 0;
    ಪ್ರತಿ(ಪೂರ್ಣಾಙ್ಕ i = 0; i < dims; i++)
        size.p[i] = 0;
    flags = (flags & CV_MAT_TYPE_MASK) | MAGIC_VAL;
    dims = rows = cols = 0;
    size.clear();
}

size_t Mat::step1(ಪೂರ್ಣಾಙ್ಕ i) ಧ್ರುವ
{
    ಪ್ರತ್ಯಾಗಮ step.p[i] / elemSize1();
}

ತಾರ್ಕಿಕ Mat::empty() ಧ್ರುವ
{
    ಪ್ರತ್ಯಾಗಮ data == 0 || total() == 0;
}

size_t Mat::total() ಧ್ರುವ
{
    ಯದಿ( dims <= 2 )
        ಪ್ರತ್ಯಾಗಮ (size_t)rows * cols;
    size_t p = 1;
    ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ i = 0; i < dims; i++ )
        p *= size[i];
    ಪ್ರತ್ಯಾಗಮ p;
}

size_t Mat::total(ಪೂರ್ಣಾಙ್ಕ startDim, ಪೂರ್ಣಾಙ್ಕ endDim) ಧ್ರುವ
{
    CV_Assert( 0 <= startDim && startDim <= endDim);
    size_t p = 1;
    ಪೂರ್ಣಾಙ್ಕ endDim_ = endDim <= dims ? endDim : dims;
    ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ i = startDim; i < endDim_; i++ )
        p *= size[i];
    ಪ್ರತ್ಯಾಗಮ p;
}

Mat::Mat(Mat&& m) CV_NOEXCEPT
    : flags(m.flags), dims(m.dims), rows(m.rows), cols(m.cols), data(m.data),
      datastart(m.datastart), dataend(m.dataend), datalimit(m.datalimit), allocator(m.allocator),
      u(m.u), size(m.size), step(m.step)
{
    m.flags = MAGIC_VAL; m.dims = m.rows = m.cols = 0;
    m.size.clear(); m.step.clear();
    m.data = ರಿಕ್ತ; m.datastart = ರಿಕ್ತ; m.dataend = ರಿಕ್ತ; m.datalimit = ರಿಕ್ತ;
    m.allocator = ರಿಕ್ತ;
    m.u = ರಿಕ್ತ;
}


Mat& Mat::operator = (Mat&& m)
{
    ಯದಿ (ಏತತ್ == &m)
      ಪ್ರತ್ಯಾಗಮ *ಏತತ್;

    release();
    flags = m.flags; dims = m.dims; rows = m.rows; cols = m.cols; data = m.data;
    size = m.size; step = m.step;
    datastart = m.datastart; dataend = m.dataend; datalimit = m.datalimit; allocator = m.allocator;
    u = m.u;
    m.flags = MAGIC_VAL; m.dims = m.rows = m.cols = 0;
    m.size.clear(); m.step.clear();
    m.data = ರಿಕ್ತ; m.datastart = ರಿಕ್ತ; m.dataend = ರಿಕ್ತ; m.datalimit = ರಿಕ್ತ;
    m.allocator = ರಿಕ್ತ;
    m.u = ರಿಕ್ತ;
    ಪ್ರತ್ಯಾಗಮ *ಏತತ್;
}


ಶೂನ್ಯ Mat::create(ಪೂರ್ಣಾಙ್ಕ d0, ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ* _sizes, ಪೂರ್ಣಾಙ್ಕ _type)
{
    ಪೂರ್ಣಾಙ್ಕ sz1 = 1, d = d0;
    ಪೂರ್ಣಾಙ್ಕ i;
    ಯದಿ (d == 0) {
        d = 1;
        _sizes = (ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ*)&sz1;
    }
    CV_Assert(0 <= d && d <= CV_MAX_DIM && _sizes);
    _type = CV_MAT_TYPE(_type);

    ಯದಿ( data && d == dims && _type == type() )
    {
        ಪ್ರತಿ( i = 0; i < d; i++ )
            ಯದಿ( size[i] != _sizes[i] )
                ಭಙ್ಗ;
        ಯದಿ( i == d )
            ಪ್ರತ್ಯಾಗಮ;
    }

    ಪೂರ್ಣಾಙ್ಕ _sizes_backup[CV_MAX_DIM]; // #5991
    ಯದಿ (_sizes == (ಏತತ್->size.p))
    {
        ಪ್ರತಿ(i = 0; i < d; i++ )
            _sizes_backup[i] = _sizes[i];
        _sizes = _sizes_backup;
    }

    release();
    ಯದಿ( d == 0 )
        ಪ್ರತ್ಯಾಗಮ;
    flags = (_type & CV_MAT_TYPE_MASK) | MAGIC_VAL;
    setSize(*ಏತತ್, d, _sizes, 0, ಸತ್ಯ);

    ಯದಿ( total() > 0 )
    {
        MatAllocator *a = allocator, *a0 = getDefaultAllocator();
        ಯದಿ(!a)
            a = a0;
        ಯತ್ನ
        {
            u = a->allocate(dims, size.p, _type, 0, step.p, ACCESS_RW /* ignored */, USAGE_DEFAULT);
            CV_Assert(u != 0);
            allocator = a;
        }
        ಗೃಹಾಣ (...)
        {
            ಯದಿ (a == a0)
                ಕ್ಷಿಪ;
            u = a0->allocate(dims, size.p, _type, 0, step.p, ACCESS_RW /* ignored */, USAGE_DEFAULT);
            CV_Assert(u != 0);
            allocator = a0;
        }
        CV_Assert( step[dims-1] == (size_t)CV_ELEM_SIZE(flags) );
    }

    addref();
    finalizeHdr(*ಏತತ್);
    size.dims = dims = d0;
}

ಶೂನ್ಯ Mat::create(ಧ್ರುವ std::vector<ಪೂರ್ಣಾಙ್ಕ>& _sizes, ಪೂರ್ಣಾಙ್ಕ _type)
{
    create((ಪೂರ್ಣಾಙ್ಕ)_sizes.size(), _sizes.data(), _type);
}

ಶೂನ್ಯ Mat::create(ಧ್ರುವ MatShape& _shape, ಪೂರ್ಣಾಙ್ಕ _type)
{
    ಯದಿ (_shape.dims < 0) {
        release();
        ಪ್ರತ್ಯಾಗಮ;
    }
    create(_shape.dims, _shape.p, _type);
    size.layout = _shape.layout;
    size.C = _shape.C;
}

ಶೂನ್ಯ Mat::create(std::initializer_list<ಪೂರ್ಣಾಙ್ಕ> _shape, ಪೂರ್ಣಾಙ್ಕ _type)
{
    ಪೂರ್ಣಾಙ್ಕ new_shape[MatShape::MAX_DIMS];
    ಪೂರ್ಣಾಙ್ಕ new_ndims = (ಪೂರ್ಣಾಙ್ಕ)_shape.size();
    CV_Assert(new_ndims <= MatShape::MAX_DIMS);
    auto it = _shape.begin();
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < new_ndims; i++, ++it)
        new_shape[i] = *it;

    create(new_ndims, new_shape, _type);
}

ಶೂನ್ಯ Mat::copySize(ಧ್ರುವ Mat& m)
{
    dims = m.dims;
    cols = m.cols;
    rows = m.rows;
    size = m.size;
    step = m.step;
}

ಶೂನ್ಯ Mat::deallocate()
{
    ಯದಿ(u)
    {
        UMatData* u_ = u;
        u = ರಿಕ್ತ;
        (u_->currAllocator ? u_->currAllocator : allocator ? allocator : getDefaultAllocator())->unmap(u_);
    }
}

Mat::Mat(ಧ್ರುವ Mat& m, ಧ್ರುವ Range& _rowRange, ಧ್ರುವ Range& _colRange)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    CV_Assert(m.dims <= MatShape::MAX_DIMS);
    CV_Assert(m.dims >= 2 ||
              (m.dims == 1 && (_rowRange == Range::all() ||
                               _rowRange == Range(0, m.rows))));
    ಯದಿ( m.dims > 2 )
    {
        Range rs[MatShape::MAX_DIMS];
        rs[0] = _rowRange;
        rs[1] = _colRange;
        ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ i = 2; i < m.dims; i++ )
            rs[i] = Range::all();
        *ಏತತ್ = m(rs);
        ಪ್ರತ್ಯಾಗಮ;
    }

    *ಏತತ್ = m;
    ಯತ್ನ
    {
        ಯದಿ( _rowRange != Range::all() && _rowRange != Range(0,rows) )
        {
            CV_Assert( 0 <= _rowRange.start && _rowRange.start <= _rowRange.end
                       && _rowRange.end <= m.rows );
            size[0] = rows = _rowRange.size();
            data += step*_rowRange.start;
            flags |= SUBMATRIX_FLAG;
        }

        ಯದಿ( _colRange != Range::all() && _colRange != Range(0,cols) )
        {
            CV_Assert( 0 <= _colRange.start && _colRange.start <= _colRange.end
                       && _colRange.end <= m.cols );
            size[dims > 1] = cols = _colRange.size();
            data += _colRange.start*elemSize();
            flags |= SUBMATRIX_FLAG;
        }
    }
    ಗೃಹಾಣ(...)
    {
        release();
        ಕ್ಷಿಪ;
    }

    updateContinuityFlag();

    ಯದಿ( rows <= 0 || cols <= 0 )
    {
        release();
        rows = cols = 0;
    }
}


Mat::Mat(ಧ್ರುವ Mat& m, ಧ್ರುವ Rect& roi)
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
    ಯದಿ( roi.width < m.cols || roi.height < m.rows )
        flags |= SUBMATRIX_FLAG;

    size[0] = rows;
    size[1] = cols;
    step[0] = m.step[0];
    step[1] = esz;
    updateContinuityFlag();

    addref();
    ಯದಿ( rows <= 0 || cols <= 0 )
    {
        rows = cols = 0;
        release();
    }
}


Mat::Mat(ಪೂರ್ಣಾಙ್ಕ _dims, ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ* _sizes, ಪೂರ್ಣಾಙ್ಕ _type, ಶೂನ್ಯ* _data, ಧ್ರುವ size_t* _steps)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    flags |= CV_MAT_TYPE(_type);
    datastart = data = (uchar*)_data;
    ಯದಿ (_dims > 0 || _data != nullptr)
        setSize(*ಏತತ್, _dims, _sizes, _steps, ಸತ್ಯ);
    finalizeHdr(*ಏತತ್);
}


Mat::Mat(ಧ್ರುವ std::vector<ಪೂರ್ಣಾಙ್ಕ>& _sizes, ಪೂರ್ಣಾಙ್ಕ _type, ಶೂನ್ಯ* _data, ಧ್ರುವ size_t* _steps)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    flags |= CV_MAT_TYPE(_type);
    datastart = data = (uchar*)_data;
    ಪೂರ್ಣಾಙ್ಕ _dims = (ಪೂರ್ಣಾಙ್ಕ)_sizes.size();
    ಯದಿ (_dims > 0 || _data != nullptr)
        setSize(*ಏತತ್, _dims, _sizes.data(), _steps, ಸತ್ಯ);
    finalizeHdr(*ಏತತ್);
}

Mat::Mat(ಧ್ರುವ MatShape& _shape, ಪೂರ್ಣಾಙ್ಕ _type, ಶೂನ್ಯ* _data, ಧ್ರುವ size_t* _steps)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    flags |= CV_MAT_TYPE(_type);
    datastart = data = (uchar*)_data;
    ಯದಿ (_shape.dims >= 0) {
        setSize(*ಏತತ್, _shape.dims, _shape.p, _steps, ಸತ್ಯ);
    }
    ಅನ್ಯಥಾ {
        CV_Assert(!data);
    }
    finalizeHdr(*ಏತತ್);
}

Mat::Mat(std::initializer_list<ಪೂರ್ಣಾಙ್ಕ> _shape, ಪೂರ್ಣಾಙ್ಕ _type, ಶೂನ್ಯ* _data, ಧ್ರುವ size_t* _steps)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    ಪೂರ್ಣಾಙ್ಕ new_shape[MatShape::MAX_DIMS];
    ಪೂರ್ಣಾಙ್ಕ _dims = (ಪೂರ್ಣಾಙ್ಕ)_shape.size();
    CV_Assert(_dims <= MatShape::MAX_DIMS);
    auto it = _shape.begin();
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < _dims; i++, ++it)
        new_shape[i] = *it;

    flags |= CV_MAT_TYPE(_type);
    datastart = data = (uchar*)_data;
    ಯದಿ (_dims > 0 || _data != nullptr)
        setSize(*ಏತತ್, _dims, new_shape, _steps, ಸತ್ಯ);
    finalizeHdr(*ಏತತ್);
}

Mat::Mat(ಧ್ರುವ Mat& m, ಧ್ರುವ Range* ranges)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    ಪೂರ್ಣಾಙ್ಕ d = m.dims;

    CV_Assert(ranges);
    ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ i = 0; i < d; i++ )
    {
        Range r = ranges[i];
        CV_Assert( r == Range::all() || (0 <= r.start && r.start < r.end && r.end <= m.size[i]) );
    }
    *ಏತತ್ = m;
    ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ i = 0; i < d; i++ )
    {
        Range r = ranges[i];
        ಯದಿ( r != Range::all() && r != Range(0, size.p[i]))
        {
            size.p[i] = r.end - r.start;
            data += r.start*step.p[i];
            flags |= SUBMATRIX_FLAG;
        }
    }

    ಯದಿ (d <= 2) {
        rows = d == 2 ? size[0] : 1;
        cols = d <= 0 ? (d >= 0) : size[d > 1];
    }
    updateContinuityFlag();
}

Mat::Mat(ಧ್ರುವ Mat& m, ಧ್ರುವ std::vector<Range>& ranges)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
    datalimit(0), allocator(0), u(0)
{
    ಪೂರ್ಣಾಙ್ಕ d = m.dims;

    CV_Assert((ಪೂರ್ಣಾಙ್ಕ)ranges.size() == d);
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < d; i++)
    {
        Range r = ranges[i];
        CV_Assert(r == Range::all() || (0 <= r.start && r.start < r.end && r.end <= m.size[i]));
    }
    *ಏತತ್ = m;
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < d; i++)
    {
        Range r = ranges[i];
        ಯದಿ (r != Range::all() && r != Range(0, size.p[i]))
        {
            size.p[i] = r.end - r.start;
            data += r.start*step.p[i];
            flags |= SUBMATRIX_FLAG;
        }
    }

    ಯದಿ (d <= 2) {
        rows = d == 2 ? size[0] : 1;
        cols = d <= 0 ? (d >= 0) : size[d > 1];
    }
    updateContinuityFlag();
}


Mat Mat::diag(ಪೂರ್ಣಾಙ್ಕ d) ಧ್ರುವ
{
    CV_Assert( dims == 2 );
    Mat m = *ಏತತ್;
    size_t esz = elemSize();
    ಪೂರ್ಣಾಙ್ಕ len;

    ಯದಿ( d >= 0 )
    {
        len = std::min(cols - d, rows);
        m.data += esz*d;
    }
    ಅನ್ಯಥಾ
    {
        len = std::min(rows + d, cols);
        m.data -= step[0]*d;
    }
    CV_DbgAssert( len > 0 );

    m.size[0] = m.rows = len;
    m.size[1] = m.cols = 1;
    m.step[0] += (len > 1 ? esz : 0);

    m.updateContinuityFlag();

    ಯದಿ( size() != Size(1,1) )
        m.flags |= SUBMATRIX_FLAG;

    ಪ್ರತ್ಯಾಗಮ m;
}


ಶೂನ್ಯ Mat::pop_back(size_t nelems)
{
    CV_Assert( nelems <= (size_t)size.p[0] );
    CV_Assert( dims >= 1 );

    ಯದಿ( isSubmatrix() )
        *ಏತತ್ = rowRange(0, size.p[0] - (ಪೂರ್ಣಾಙ್ಕ)nelems);
    ಅನ್ಯಥಾ
    {
        size.p[0] -= (ಪೂರ್ಣಾಙ್ಕ)nelems;
        ಯದಿ (dims == 2)
            rows = size.p[0];
        ಅನ್ಯಥಾ ಯದಿ (dims == 1)
            cols = size.p[0];
        dataend -= nelems*step.p[0];
    }
}


ಶೂನ್ಯ Mat::ತಳ್ಳು_back(ಧ್ರುವ ಶೂನ್ಯ* elem)
{
    size_t r = size.p[0];
    ಯದಿ( isSubmatrix() || dataend + step.p[0] > datalimit )
        reserve( std::max(r + 1, (r*3+1)/2) );

    size_t esz = elemSize();
    memcpy(data + r*step.p[0], elem, esz);
    size.p[0] = ಪೂರ್ಣಾಙ್ಕ(r + 1);
    ಯದಿ (dims == 2)
        rows = size.p[0];
    ಅನ್ಯಥಾ ಯದಿ (dims == 1)
        cols = size.p[0];
    dataend += step.p[0];
    uint64 tsz = size.p[0];
    ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ i = 1; i < dims; i++ )
        tsz *= size.p[i];
    ಯದಿ( esz < step.p[0] || tsz != (uint64)(ಪೂರ್ಣಾಙ್ಕ)tsz )
        flags &= ~CONTINUOUS_FLAG;
}


ಶೂನ್ಯ Mat::reserve(size_t nelems)
{
    ಧ್ರುವ size_t MIN_SIZE = 64;

    CV_Assert( (ಪೂರ್ಣಾಙ್ಕ)nelems >= 0 );
    ಯದಿ( !isSubmatrix() && step.p[0] != 0 && data + step.p[0]*nelems <= datalimit )
        ಪ್ರತ್ಯಾಗಮ;

    ಪೂರ್ಣಾಙ್ಕ r = size.p[0];
    ಯದಿ( (size_t)r >= nelems)
        ಪ್ರತ್ಯಾಗಮ;

    MatShape newsize = size;
    size_t esz = elemSize();
    newsize.dims = std::max(newsize.dims, 1);
    ಯದಿ (dims > 1 && r == 0 && step.p[0] == 0) {
        step.p[0] = size.p[1] * (dims == 2 ? esz : step.p[1]);
    }
    newsize.p[0] = (ಪೂರ್ಣಾಙ್ಕ)nelems;
    size_t newbytes = newsize.total() * esz;
    ಯದಿ (newbytes < MIN_SIZE) {
        newsize.p[0] = 1;
        newsize.p[0] = ಪೂರ್ಣಾಙ್ಕ(MIN_SIZE / newsize.total() / esz);
    }

    Mat m(newsize, type());
    ಯದಿ( r > 0)
    {
        Mat mpart = m.rowRange(0, r);
        copyTo(mpart);
    }

    *ಏತತ್ = m;
    size.p[0] = r;

    ಯದಿ (dims == 2)
        rows = size.p[0];
    ಅನ್ಯಥಾ ಯದಿ (dims == 1)
        cols = size.p[0];
    dataend = data + step.p[0]*r;
}


ಶೂನ್ಯ Mat::reserveBuffer(size_t nbytes)
{
    size_t esz = 1;
    ಪೂರ್ಣಾಙ್ಕ mtype = CV_8UC1;
    ಯದಿ (!empty())
    {
        ಯದಿ (!isSubmatrix() && data + nbytes <= dataend)//Should it be datalimit?
            ಪ್ರತ್ಯಾಗಮ;
        esz = elemSize();
        mtype = type();
    }

    size_t nelems = (nbytes - 1) / esz + 1;

#ಯದಿ SIZE_MAX > UINT_MAX
    CV_Assert(nelems <= size_t(INT_MAX)*size_t(INT_MAX));
    ಪೂರ್ಣಾಙ್ಕ newrows = nelems > size_t(INT_MAX) ? nelems > 0x400*size_t(INT_MAX) ? nelems > 0x100000 * size_t(INT_MAX) ? nelems > 0x40000000 * size_t(INT_MAX) ?
                  size_t(INT_MAX) : 0x40000000 : 0x100000 : 0x400 : 1;
#ಅನ್ಯಥಾ
    ಪೂರ್ಣಾಙ್ಕ newrows = nelems > size_t(INT_MAX) ? 2 : 1;
#endif
    ಪೂರ್ಣಾಙ್ಕ newcols = (ಪೂರ್ಣಾಙ್ಕ)((nelems - 1) / newrows + 1);

    create(newrows, newcols, mtype);
}


ಶೂನ್ಯ Mat::resize(size_t nelems)
{
    ಪೂರ್ಣಾಙ್ಕ r = size.p[0];
    ಯದಿ( r == (ಪೂರ್ಣಾಙ್ಕ)nelems )
        ಪ್ರತ್ಯಾಗಮ;
    CV_Assert( (ಪೂರ್ಣಾಙ್ಕ)nelems >= 0 );

    ಯದಿ( isSubmatrix() || data + step.p[0]*nelems > datalimit )
        reserve(nelems);

    size.p[0] = (ಪೂರ್ಣಾಙ್ಕ)nelems;
    ಯದಿ (dims == 2)
        rows = size.p[0];
    ಅನ್ಯಥಾ ಯದಿ (dims == 1)
        cols = size.p[0];
    dataend += (size.p[0] - r)*(int64_t)step.p[0];

    //updateContinuityFlag(*this);
}


ಶೂನ್ಯ Mat::resize(size_t nelems, ಧ್ರುವ Scalar& s)
{
    ಪೂರ್ಣಾಙ್ಕ saveRows = size.p[0];
    resize(nelems);

    ಯದಿ( size.p[0] > saveRows )
    {
        Mat part = rowRange(saveRows, size.p[0]);
        part = s;
    }
}

ಶೂನ್ಯ Mat::push_back(ಧ್ರುವ Mat& elems)
{
    size_t r = size.p[0];
    size_t delta = elems.size.p[0];
    ಯದಿ( delta == 0 )
        ಪ್ರತ್ಯಾಗಮ;
    ಯದಿ( ಏತತ್ == &elems )
    {
        Mat tmp = elems;
        push_back(tmp);
        ಪ್ರತ್ಯಾಗಮ;
    }
    ಯದಿ( !data )
    {
        *ಏತತ್ = elems.clone();
        ಪ್ರತ್ಯಾಗಮ;
    }
    CV_Assert(dims > 0);

    size.p[0] = elems.size.p[0];
    ತಾರ್ಕಿಕ eq = size == elems.size;
    size.p[0] = ಪೂರ್ಣಾಙ್ಕ(r);
    ಯದಿ( !eq )
        CV_Error(cv::Error::StsUnmatchedSizes, "Pushed vector length is not equal to matrix row length");
    ಯದಿ( type() != elems.type() )
        CV_Error(cv::Error::StsUnmatchedFormats, "Pushed vector type is not the same as matrix type");
    size_t esz = elemSize();
    size_t minstep = dims <= 1 ? esz : size.p[1] * (dims == 2 ? esz : step.p[1]);
    size_t step0 = step.p[0];
    ಯದಿ (step0 < minstep) {
        ಯದಿ (size.p[0] <= 1)
            step.p[0] = step0 = minstep;
    }

    ಯದಿ( isSubmatrix() || dataend + step0*delta > datalimit )
        reserve( std::max(r + delta, (r*3+1)/2) );

    size.p[0] += ಪೂರ್ಣಾಙ್ಕ(delta);
    ಯದಿ (dims == 2)
        rows = size.p[0];
    ಅನ್ಯಥಾ ಯದಿ (dims == 1)
        cols = size.p[0];
    dataend += step0*delta;

    //updateContinuityFlag(*this);

    ಯದಿ( isContinuous() && elems.isContinuous() )
        memcpy(data + r*step0, elems.data, elems.total()*elems.elemSize());
    ಅನ್ಯಥಾ
    {
        Mat part = rowRange(ಪೂರ್ಣಾಙ್ಕ(r), ಪೂರ್ಣಾಙ್ಕ(r + delta));
        elems.copyTo(part);
    }
}


ಶೂನ್ಯ Mat::locateROI( Size& wholeSize, Point& ofs ) ಧ್ರುವ
{
    CV_Assert( dims <= 2 && step[0] > 0 );
    size_t esz = elemSize(), minstep;
    ptrdiff_t delta1 = data - datastart, delta2 = dataend - datastart;

    ಯದಿ( delta1 == 0 )
        ofs.x = ofs.y = 0;
    ಅನ್ಯಥಾ
    {
        ofs.y = (ಪೂರ್ಣಾಙ್ಕ)(delta1/step[0]);
        ofs.x = (ಪೂರ್ಣಾಙ್ಕ)((delta1 - step[0]*ofs.y)/esz);
        CV_DbgAssert( data == datastart + ofs.y*step[0] + ofs.x*esz );
    }
    minstep = (ofs.x + cols)*esz;
    wholeSize.height = (ಪೂರ್ಣಾಙ್ಕ)((delta2 - minstep)/step[0] + 1);
    wholeSize.height = std::max(wholeSize.height, ofs.y + rows);
    wholeSize.width = (ಪೂರ್ಣಾಙ್ಕ)((delta2 - step*(wholeSize.height-1))/esz);
    wholeSize.width = std::max(wholeSize.width, ofs.x + cols);
}

Mat& Mat::adjustROI( ಪೂರ್ಣಾಙ್ಕ dtop, ಪೂರ್ಣಾಙ್ಕ dbottom, ಪೂರ್ಣಾಙ್ಕ dleft, ಪೂರ್ಣಾಙ್ಕ dright )
{
    CV_Assert( dims <= 2 && step[0] > 0 );
    Size wholeSize; Point ofs;
    size_t esz = elemSize();
    locateROI( wholeSize, ofs );
    ಪೂರ್ಣಾಙ್ಕ row1 = std::min(std::max(ofs.y - dtop, 0), wholeSize.height), row2 = std::max(0, std::min(ofs.y + rows + dbottom, wholeSize.height));
    ಪೂರ್ಣಾಙ್ಕ col1 = std::min(std::max(ofs.x - dleft, 0), wholeSize.width), col2 = std::max(0, std::min(ofs.x + cols + dright, wholeSize.width));
    ಯದಿ(row1 > row2)
        std::swap(row1, row2);
    ಯದಿ(col1 > col2)
        std::swap(col1, col2);

    ಯದಿ (dims == 1) {
        data += (col1 - ofs.x)*(std::ptrdiff_t)esz;
        cols = col2 - col1;
        size.p[0] = cols;
    } ಅನ್ಯಥಾ {
        data += (row1 - ofs.y)*(std::ptrdiff_t)step + (col1 - ofs.x)*(std::ptrdiff_t)esz;
        rows = row2 - row1; cols = col2 - col1;
        size.p[0] = rows; size.p[1] = cols;
        updateContinuityFlag();
    }
    ಪ್ರತ್ಯಾಗಮ *ಏತತ್;
}

Mat Mat::reshape(ಪೂರ್ಣಾಙ್ಕ new_cn, ಪೂರ್ಣಾಙ್ಕ new_rows) ಧ್ರುವ
{
    ಪೂರ್ಣಾಙ್ಕ cn = channels();
    Mat hdr = *ಏತತ್;

    ಯದಿ( new_cn == 0 )
        new_cn = cn;

    ಯದಿ( dims > 2 )
    {
        ಯದಿ( new_rows == 0 )
        {
            // special case: just change the number of channnels; retain the same shape,
            // except for the last, innermost dimension
            CV_Assert(size[dims-1]*cn % new_cn == 0);
            hdr.flags = (hdr.flags & ~CV_MAT_CN_MASK) | ((new_cn-1) << CV_CN_SHIFT);
            hdr.step[dims-1] = CV_ELEM_SIZE(hdr.flags);
            hdr.size[dims-1] = hdr.size[dims-1]*cn / new_cn;
            ಪ್ರತ್ಯಾಗಮ hdr;
        }
        CV_Assert( new_rows > 0 );
        ಪೂರ್ಣಾಙ್ಕ sz[] = { new_rows, (ಪೂರ್ಣಾಙ್ಕ)(total()*cn/new_rows) };
        ಪ್ರತ್ಯಾಗಮ reshape(new_cn, 2, sz);
    }

    ಪೂರ್ಣಾಙ್ಕ total_width = cols * cn;

    ಯದಿ( (new_cn > total_width || total_width % new_cn != 0) && new_rows == 0 )
        new_rows = rows * total_width / new_cn;

    ಯದಿ( new_rows != 0 && new_rows != rows )
    {
        ಪೂರ್ಣಾಙ್ಕ total_size = total_width * rows;
        ಯದಿ( !isContinuous() )
            CV_Error( cv::Error::BadStep,
            "The matrix is not continuous, thus its number of rows can not be changed" );

        total_width = total_size / new_rows;

        ಯದಿ( total_width * new_rows != total_size )
            CV_Error( cv::Error::StsBadArg, "The total number of matrix elements "
                                    "is not divisible by the new number of rows" );

        hdr.size[0] = hdr.rows = new_rows;
        hdr.step[0] = total_width * elemSize1();
    } ಅನ್ಯಥಾ {
        hdr.size[0] = hdr.rows = rows;
        ಯದಿ (dims <= 1)
            hdr.step[0] = cols * CV_ELEM_SIZE(flags);
    }

    ಪೂರ್ಣಾಙ್ಕ new_width = total_width / new_cn;

    ಯದಿ( new_width * new_cn != total_width )
        CV_Error( cv::Error::Bad_ಅಂಕೆ_Channels,
        "The total width is not divisible by the new number of channels" );

    hdr.size.dims = hdr.dims = 2;
    hdr.size[1] = hdr.cols = new_width;
    hdr.flags = (hdr.flags & ~CV_MAT_CN_MASK) | ((new_cn-1) << CV_CN_SHIFT);
    hdr.step[1] = CV_ELEM_SIZE(hdr.flags);
    ಪ್ರತ್ಯಾಗಮ hdr;
}

Mat Mat::reshape(ಪೂರ್ಣಾಙ್ಕ _cn, ಪೂರ್ಣಾಙ್ಕ _newndims, ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ* _newsz) ಧ್ರುವ
{
    ಯದಿ(_newndims == dims)
    {
        ಯದಿ(_newsz == 0)
            ಪ್ರತ್ಯಾಗಮ reshape(_cn);
        ಯದಿ(_newndims == 2)
            ಪ್ರತ್ಯಾಗಮ reshape(_cn, _newsz[0]);
    }

    ಯದಿ (isContinuous())
    {
        CV_Assert(_cn >= 0 && _newndims >= 0 && _newndims <= CV_MAX_DIM && (_newndims == 0 || _newsz != 0));

        ಯದಿ (_cn == 0)
            _cn = ಏತತ್->channels();
        ಅನ್ಯಥಾ
            CV_Assert(_cn <= CV_CN_MAX);

        size_t total_elem1_ref = ಏತತ್->total() * ಏತತ್->channels();
        size_t total_elem1 = _cn;

        AutoBuffer<ಪೂರ್ಣಾಙ್ಕ, 4> newsz_buf( (size_t)_newndims );

        ಪೂರ್ಣಾಙ್ಕ m1_idx = -1;

        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < _newndims; i++)
        {
            ಯದಿ (_newsz[i] >= 0) {
                ಯದಿ (_newsz[i] == 0 && i < dims)
                    newsz_buf[i] = size.p[i];
                ಅನ್ಯಥಾ
                    newsz_buf[i] = _newsz[i];
                total_elem1 *= (size_t)newsz_buf[i];
            } ಅನ್ಯಥಾ {
                ಯದಿ (m1_idx >= 0)
                    CV_Error(cv::Error::StsBadSize, "More than one '-1' occured in the new shape");
                m1_idx = i;
            }
        }

        ಯದಿ (m1_idx >= 0) {
            ಯದಿ (total_elem1 == 0) {
                CV_Assert(total_elem1_ref == 0);
                total_elem1 = 1;
            }
            CV_Assert(total_elem1_ref % total_elem1 == 0);
            newsz_buf[m1_idx] = (ಪೂರ್ಣಾಙ್ಕ)(total_elem1_ref / total_elem1);
        } ಅನ್ಯಥಾ ಯದಿ (total_elem1 != total_elem1_ref) {
            CV_Error(cv::Error::StsUnmatchedSizes, "Requested and source matrices have different count of elements");
        }

        Mat hdr = *ಏತತ್;
        hdr.flags = (hdr.flags & ~CV_MAT_CN_MASK) | ((_cn-1) << CV_CN_SHIFT);
        setSize(hdr, _newndims, newsz_buf.data(), ರಿಕ್ತ, ಸತ್ಯ);

        ಪ್ರತ್ಯಾಗಮ hdr;
    }

    CV_Error(cv::Error::StsNotImplemented, "Reshaping of n-dimensional non-continuous matrices is not supported yet");
    // TBD
}

Mat Mat::reshape(ಪೂರ್ಣಾಙ್ಕ _cn, ಧ್ರುವ std::vector<ಪೂರ್ಣಾಙ್ಕ>& _newshape) ಧ್ರುವ
{
    ಪೂರ್ಣಾಙ್ಕ newdims = (ಪೂರ್ಣಾಙ್ಕ)_newshape.size();
    ಯದಿ(newdims == 0 && empty())
    {
        ಪ್ರತ್ಯಾಗಮ *ಏತತ್;
    }

    ಪ್ರತ್ಯಾಗಮ reshape(_cn, newdims, newdims > 0 ? &_newshape[0] : 0);
}

Mat Mat::reshape(ಪೂರ್ಣಾಙ್ಕ _cn, ಧ್ರುವ MatShape& _newshape) ಧ್ರುವ
{
    ಯದಿ (_newshape.dims < 0) {
        ಪೂರ್ಣಾಙ್ಕ newshape[] = {0};
        ಪ್ರತ್ಯಾಗಮ reshape(_cn, 1, newshape);
    }
    ಪ್ರತ್ಯಾಗಮ reshape(_cn, _newshape.dims, _newshape.p);
}

Mat Mat::reshape(ಪೂರ್ಣಾಙ್ಕ _cn, std::initializer_list<ಪೂರ್ಣಾಙ್ಕ> newshape_) ಧ್ರುವ
{
    ಪೂರ್ಣಾಙ್ಕ newshape[MatShape::MAX_DIMS];
    size_t i, newshape_dims = newshape_.size();
    CV_Assert(newshape_dims <= (size_t)MatShape::MAX_DIMS);
    auto it = newshape_.begin();
    ಪ್ರತಿ (i = 0; i < newshape_dims; i++, ++it)
        newshape[i] = *it;
    ಪ್ರತ್ಯಾಗಮ reshape(_cn, (ಪೂರ್ಣಾಙ್ಕ)newshape_dims, newshape);
}

Mat Mat::reinterpret(ಪೂರ್ಣಾಙ್ಕ type) ಧ್ರುವ
{
    type = CV_MAT_TYPE(type);
    CV_Assert(CV_ELEM_SIZE(ಏತತ್->type()) == CV_ELEM_SIZE(type));
    Mat m = *ಏತತ್;
    m.flags = (m.flags & ~CV_MAT_TYPE_MASK) | type;
    m.updateContinuityFlag();
    ಪ್ರತ್ಯಾಗಮ m;
}

Mat Mat::diag(ಧ್ರುವ Mat& d)
{
    CV_Assert( d.cols == 1 || d.rows == 1 );
    ಪೂರ್ಣಾಙ್ಕ len = d.rows + d.cols - 1;
    Mat m(len, len, d.type(), Scalar(0));
    Mat md = m.diag();
    ಯದಿ( d.cols == 1 )
        d.copyTo(md);
    ಅನ್ಯಥಾ
        transpose(d, md);
    ಪ್ರತ್ಯಾಗಮ m;
}

ಪೂರ್ಣಾಙ್ಕ Mat::checkVector(ಪೂರ್ಣಾಙ್ಕ _elemChannels, ಪೂರ್ಣಾಙ್ಕ _depth, ತಾರ್ಕಿಕ _requireContinuous) ಧ್ರುವ
{
    ಪ್ರತ್ಯಾಗಮ data && (depth() == _depth || _depth <= 0) &&
        (isContinuous() || !_requireContinuous) &&
        ((dims <= 2 && (((rows == 1 || cols == 1) && channels() == _elemChannels) ||
                        (cols == _elemChannels && channels() == 1))) ||
        (dims == 3 && channels() == 1 && size.p[2] == _elemChannels && (size.p[0] == 1 || size.p[1] == 1) &&
         (isContinuous() || step.p[1] == step.p[2]*size.p[2])))
    ? (ಪೂರ್ಣಾಙ್ಕ)(total()*channels()/_elemChannels) : -1;
}


ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ Size getContinuousSize_(ಪೂರ್ಣಾಙ್ಕ flags, ಪೂರ್ಣಾಙ್ಕ cols, ಪೂರ್ಣಾಙ್ಕ rows, ಪೂರ್ಣಾಙ್ಕ widthScale)
{
    int64 sz = (int64)cols * rows * widthScale;
    ತಾರ್ಕಿಕ has_int_overflow = sz >= INT_MAX;
    ತಾರ್ಕಿಕ isContiguous = (flags & Mat::CONTINUOUS_FLAG) != 0;
    ಪ್ರತ್ಯಾಗಮ (isContiguous && !has_int_overflow)
            ? Size((ಪೂರ್ಣಾಙ್ಕ)sz, 1)
            : Size(cols * widthScale, rows);
}

Size getContinuousSize2D(Mat& m1, ಪೂರ್ಣಾಙ್ಕ widthScale)
{
    CV_CheckLE(m1.dims, 2, "");
    ಪ್ರತ್ಯಾಗಮ getContinuousSize_(m1.flags,
                              m1.cols, m1.rows, widthScale);
}
Size getContinuousSize2D(Mat& m1, Mat& m2, ಪೂರ್ಣಾಙ್ಕ widthScale)
{
    CV_CheckLE(m1.dims, 2, "");
    CV_CheckLE(m2.dims, 2, "");
    ಧ್ರುವ Size sz1 = m1.size();
    ಯದಿ (sz1 != m2.size())  // reshape all matrixes to the same size (#4159)
    {
        size_t total_sz = m1.total();
        CV_CheckEQ(total_sz, m2.total(), "");
        ತಾರ್ಕಿಕ is_m1_vector = m1.cols == 1 || m1.rows == 1;
        ತಾರ್ಕಿಕ is_m2_vector = m2.cols == 1 || m2.rows == 1;
        CV_Assert(is_m1_vector); CV_Assert(is_m2_vector);
        ಪೂರ್ಣಾಙ್ಕ total = (ಪೂರ್ಣಾಙ್ಕ)total_sz;  // vector-column
        ತಾರ್ಕಿಕ isContiguous = ((m1.flags & m2.flags) & Mat::CONTINUOUS_FLAG) != 0;
        ತಾರ್ಕಿಕ has_int_overflow = ((int64)total_sz * widthScale) >= INT_MAX;
        ಯದಿ (isContiguous && !has_int_overflow)
            total = 1; // vector-row
        m1 = m1.reshape(0, total);
        m2 = m2.reshape(0, total);
        CV_Assert(m1.cols == m2.cols && m1.rows == m2.rows);
        ಪ್ರತ್ಯಾಗಮ Size(m1.cols * widthScale, m1.rows);
    }
    ಪ್ರತ್ಯಾಗಮ getContinuousSize_(m1.flags & m2.flags,
                              m1.cols, m1.rows, widthScale);
}

Size getContinuousSize2D(Mat& m1, Mat& m2, Mat& m3, ಪೂರ್ಣಾಙ್ಕ widthScale)
{
    CV_CheckLE(m1.dims, 2, "");
    CV_CheckLE(m2.dims, 2, "");
    CV_CheckLE(m3.dims, 2, "");
    ಧ್ರುವ Size sz1 = m1.size();
    ಯದಿ (sz1 != m2.size() || sz1 != m3.size())  // reshape all matrixes to the same size (#4159)
    {
        size_t total_sz = m1.total();
        CV_CheckEQ(total_sz, m2.total(), "");
        CV_CheckEQ(total_sz, m3.total(), "");
        ತಾರ್ಕಿಕ is_m1_vector = m1.cols == 1 || m1.rows == 1;
        ತಾರ್ಕಿಕ is_m2_vector = m2.cols == 1 || m2.rows == 1;
        ತಾರ್ಕಿಕ is_m3_vector = m3.cols == 1 || m3.rows == 1;
        CV_Assert(is_m1_vector); CV_Assert(is_m2_vector); CV_Assert(is_m3_vector);
        ಪೂರ್ಣಾಙ್ಕ total = (ಪೂರ್ಣಾಙ್ಕ)total_sz;  // vector-column
        ತಾರ್ಕಿಕ isContiguous = ((m1.flags & m2.flags & m3.flags) & Mat::CONTINUOUS_FLAG) != 0;
        ತಾರ್ಕಿಕ has_int_overflow = ((int64)total_sz * widthScale) >= INT_MAX;
        ಯದಿ (isContiguous && !has_int_overflow)
            total = 1; // vector-row
        m1 = m1.reshape(0, total);
        m2 = m2.reshape(0, total);
        m3 = m3.reshape(0, total);
        CV_Assert(m1.cols == m2.cols && m1.rows == m2.rows && m1.cols == m3.cols && m1.rows == m3.rows);
        ಪ್ರತ್ಯಾಗಮ Size(m1.cols * widthScale, m1.rows);
    }
    ಪ್ರತ್ಯಾಗಮ getContinuousSize_(m1.flags & m2.flags & m3.flags,
                              m1.cols, m1.rows, widthScale);
}

} // cv::
