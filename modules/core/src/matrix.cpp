// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#ଅନ୍ତର୍ଭାଵ "precomp.hpp"
#ଅନ୍ତର୍ଭାଵ "bufferpool.impl.hpp"

ନାମାକାଶ cv {

std::string layoutToString(DataLayout layout)
{
    ଫେରନ୍ତୁ
        layout == DATA_LAYOUT_ND ? "ND" :
        layout == DATA_LAYOUT_NCHW ? "NCHW" :
        layout == DATA_LAYOUT_NHWC ? "NHWC" :
        layout == DATA_LAYOUT_BLOCK ? "BLOCK" :
        layout == DATA_LAYOUT_NCDHW ? "NCDHW" :
        layout == DATA_LAYOUT_NDHWC ? "NDHWC" :
        layout == DATA_LAYOUT_PLANAR ? "PLANAR" :
        layout == DATA_LAYOUT_UNKNOWN ? "Unknown" : "???";
}

ତାର୍କିକ operator == (ଅଚଳ MatShape& size1, ଅଚଳ MatShape& size2)
{
    ଯଦି (size1.dims != size2.dims)
        ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ଯଦି (size1.layout != size2.layout &&
        size1.layout != DATA_LAYOUT_UNKNOWN &&
        size2.layout != DATA_LAYOUT_UNKNOWN)
        ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ଯଦି (size1.layout == DATA_LAYOUT_BLOCK &&
        size2.layout == DATA_LAYOUT_BLOCK &&
        size1.C != size2.C)
        ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < size1.dims; i++) {
        ଯଦି (size1.p[i] != size2.p[i])
            ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }
    ଫେରନ୍ତୁ ସତ୍ୟ;
}

ତାର୍କିକ operator != (ଅଚଳ MatShape& size1, ଅଚଳ MatShape& size2)
{
    ଫେରନ୍ତୁ !(size1 == size2);
}

/////////////////////////// MatShape ////////////////////////////////

MatShape MatShape::scalar()
{
    ଫେରନ୍ତୁ MatShape(0);
}

ଶୂନ୍ୟ MatShape::clear()
{
    dims = -1;
    layout = DATA_LAYOUT_UNKNOWN;
    C = 0;
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < MAX_DIMS; i++)
        p[i] = 0;
}

ଶୂନ୍ୟ MatShape::resize(size_t newSize, ପୂର୍ଣ୍ଣସଂଖ୍ୟା value)
{
    CV_Assert(newSize < (size_t)MAX_DIMS);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା old_dims = std::max(dims, 0);
    dims = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)newSize;
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = old_dims; i < dims; i++)
        p[i] = value;
}

ଶୂନ୍ୟ MatShape::reserve(size_t)
{
    // no op; maybe need to add a check for overflow, but we check it anyway in other operations
}

ଶୂନ୍ୟ MatShape::assign(size_t newSize, ପୂର୍ଣ୍ଣସଂଖ୍ୟା value)
{
    CV_Assert(newSize < (size_t)MAX_DIMS);
    dims = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)newSize;
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < dims; i++)
        p[i] = value;
}

ଶୂନ୍ୟ MatShape::assign(ପୂର୍ଣ୍ଣସଂଖ୍ୟା newSize, ପୂର୍ଣ୍ଣସଂଖ୍ୟା value)
{
    assign((size_t)newSize, value);
}

ଶୂନ୍ୟ MatShape::assign(ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା* begin, ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା* end)
{
    assign_(begin, end);
}

ଶୂନ୍ୟ MatShape::assign_(ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା* begin, ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା* end)
{
    ptrdiff_t newSize = end - begin;
    CV_Assert(0 <= newSize && newSize < (ptrdiff_t)MAX_DIMS);
    dims = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)newSize;
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < dims; i++)
        p[i] = begin[i];
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା* MatShape::begin() { ଫେରନ୍ତୁ p; }
ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା* MatShape::begin() ଅଚଳ { ଫେରନ୍ତୁ p; }
ପୂର୍ଣ୍ଣସଂଖ୍ୟା* MatShape::end() { ଫେରନ୍ତୁ p + std::max(dims, 0); }
ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା* MatShape::end() ଅଚଳ { ଫେରନ୍ତୁ p + std::max(dims, 0); }
ପୂର୍ଣ୍ଣସଂଖ୍ୟା& MatShape::back() { ଫେରନ୍ତୁ p[std::max(dims-1, 0)]; }
ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା& MatShape::back() ଅଚଳ { ଫେରନ୍ତୁ p[std::max(dims-1, 0)]; }

ଶୂନ୍ୟ MatShape::push_back(ପୂର୍ଣ୍ଣସଂଖ୍ୟା value)
{
    CV_Assert(dims+1 < MAX_DIMS);
    dims = std::max(dims+1, 1);
    p[dims-1] = value;
}

ଶୂନ୍ୟ MatShape::emplace_back(ପୂର୍ଣ୍ଣସଂଖ୍ୟା value)
{
    push_back(value);
}

ଶୂନ୍ୟ MatShape::insert(ପୂର୍ଣ୍ଣସଂଖ୍ୟା* where, ପୂର୍ଣ୍ଣସଂଖ୍ୟା value)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା old_dims = std::max(dims, 0);
    CV_Assert(old_dims+1 < MAX_DIMS);
    ptrdiff_t ofs = where - p;
    CV_Assert(0 <= ofs && ofs <= old_dims);
    dims = old_dims+1;
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = old_dims-1; i >= (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)ofs; i--)
        p[i+1] = p[i];
    p[ofs] = value;
}

ଶୂନ୍ୟ MatShape::insert(ପୂର୍ଣ୍ଣସଂଖ୍ୟା* where, size_t ଗଣନା_3, ପୂର୍ଣ୍ଣସଂଖ୍ୟା value)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା old_dims = std::max(dims, 0);
    CV_Assert((size_t)(old_dims+ଗଣନା_3) < (size_t)MAX_DIMS);
    ptrdiff_t ofs = where - p;
    CV_Assert(0 <= ofs && ofs <= old_dims);
    dims = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(old_dims+ଗଣନା_3);
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = old_dims-1; i >= (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)ofs; i--)
        p[i+ଗଣନା_3] = p[i];
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)ଗଣନା_3; i++)
        p[i+ofs] = value;
}

ଶୂନ୍ୟ MatShape::insert(ପୂର୍ଣ୍ଣସଂଖ୍ୟା* where, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_3, ପୂର୍ଣ୍ଣସଂଖ୍ୟା value)
{
    insert(where, (size_t)ଗଣନା_3, value);
}

ଶୂନ୍ୟ MatShape::insert(ପୂର୍ଣ୍ଣସଂଖ୍ୟା* where, ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା* begin, ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା* end)
{
    ଭର୍ତ୍ତି(where, begin, end);
}

ଶୂନ୍ୟ MatShape::ଭର୍ତ୍ତି(ପୂର୍ଣ୍ଣସଂଖ୍ୟା* where, ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା* begin, ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା* end)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା old_dims = std::max(dims, 0);
    ptrdiff_t delta = end - begin;
    CV_Assert(0 <= delta && old_dims+delta < MAX_DIMS);
    ptrdiff_t ofs = where - p;
    CV_Assert(0 <= ofs && ofs <= old_dims);
    dims = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(old_dims+delta);
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = old_dims-1; i >= (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)ofs; i--)
        p[i+delta] = p[i];
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)delta; i++)
        p[i+ofs] = begin[i];
}

ଶୂନ୍ୟ MatShape::erase(ପୂର୍ଣ୍ଣସଂଖ୍ୟା* where)
{
    CV_Assert(dims > 0);
    ptrdiff_t ofs = where - p;
    CV_Assert(0 <= ofs && ofs <= dims);
    ଯଦି (ofs == dims)
        ଫେରନ୍ତୁ;
    dims--;
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)ofs+1; i <= dims; i++)
        p[i-1] = p[i];
}

size_t MatShape::total() ଅଚଳ
{
    size_t result = 1;
    ଯଦି (dims < 0)
        ଫେରନ୍ତୁ 0;
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < dims; i++)
        result *= p[i];
    ଫେରନ୍ତୁ result;
}

std::vector<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> MatShape::vec() ଅଚଳ
{
    ଯଦି (dims < 0)
        ଫେରନ୍ତୁ std::vector<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(1, 0);
    ଫେରନ୍ତୁ std::vector<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(p, p + dims);
}

std::string MatShape::str() ଅଚଳ
{
    std::stringstream sstrm;
    ଯଦି (empty()) {
        sstrm << "<empty>";
    } ଅନ୍ୟଥା ଯଦି (dims == 0) {
        sstrm << "<scalar>";
    } ଅନ୍ୟଥା {
        sstrm << "[";
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < dims; i++) {
            sstrm << (i > 0 ? " x " : "") << p[i];
        }
        sstrm << "]";
    }
    ଫେରନ୍ତୁ sstrm.str();
}

ସ୍ଥିର ଶୂନ୍ୟ finalizeBlockLayout(MatShape& size, ପୂର୍ଣ୍ଣସଂଖ୍ୟା C=0)
{
    ଯଦି (size.layout == DATA_LAYOUT_BLOCK) {
        CV_Assert(size.dims >= 4);
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା C0 = size.p[size.dims-1];
        CV_Assert(C0 > 1 && (C0 & (C0-1)) == 0);
        size.C = C > 0 ? C : size.p[1]*size.p[size.dims-1];
    } ଅନ୍ୟଥା {
        size.C = 0;
    }
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = std::max(size.dims, 0); i < MatShape::MAX_DIMS; i++)
        size.p[i] = 0;
    ଯଦି (size.dims == 0)
        size.p[0] = 1;
}

MatShape::MatShape()
{
    clear();
}

MatShape::MatShape(size_t dims_, ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା* size_, DataLayout layout_, ପୂର୍ଣ୍ଣସଂଖ୍ୟା C_)
{
    layout = layout_;
    CV_Assert(dims_ <= (size_t)MAX_DIMS);
    dims = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)dims_;
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < dims; i++) {
        p[i] = size_ ? size_[i] : 0;
    }
    finalizeBlockLayout(*ଏହି, C_);
}

MatShape::MatShape(size_t dims_, ପୂର୍ଣ୍ଣସଂଖ୍ୟା value, DataLayout layout_)
{
    layout = layout_;
    CV_Assert(dims_ <= (size_t)MAX_DIMS);
    dims = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)dims_;
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < dims; i++) {
        p[i] = value;
    }
    finalizeBlockLayout(*ଏହି, 0);
}

MatShape::MatShape(std::initializer_list<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> shape)
{
    layout = DATA_LAYOUT_UNKNOWN;
    CV_Assert(shape.size() <= (size_t)MAX_DIMS);
    dims = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)shape.size();
    auto it = shape.begin();
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < dims; i++, ++it) {
        p[i] = *it;
    }
    finalizeBlockLayout(*ଏହି, 0);
}

MatShape::MatShape(ପୂର୍ଣ୍ଣସଂଖ୍ୟା dims_, ପୂର୍ଣ୍ଣସଂଖ୍ୟା value, DataLayout layout_)
{
    layout = layout_;
    CV_Assert(dims_ <= MAX_DIMS);
    dims = dims_;
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < dims; i++) {
        p[i] = value;
    }
    finalizeBlockLayout(*ଏହି, 0);
}

MatShape::MatShape(ଅଚଳ std::vector<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>& shape_, DataLayout layout_, ପୂର୍ଣ୍ଣସଂଖ୍ୟା C_)
{
    layout = layout_;
    size_t shape_size = shape_.size();
    CV_Assert(shape_size < (size_t)MAX_DIMS);
    dims = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)shape_size;
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < dims; i++) {
        p[i] = shape_[i];
    }
    finalizeBlockLayout(*ଏହି, C_);
}

MatShape::MatShape(ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା* begin, ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା* end, DataLayout layout_, ପୂର୍ଣ୍ଣସଂଖ୍ୟା C_)
{
    layout = layout_;
    ptrdiff_t shape_size = end - begin;
    CV_Assert(0 <= shape_size && shape_size < MAX_DIMS);
    dims = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)shape_size;
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < dims; i++) {
        p[i] = begin[i];
    }
    finalizeBlockLayout(*ଏହି, C_);
}

MatShape::MatShape(ଅଚଳ MatShape& shape)
{
    dims = shape.dims;
    layout = shape.layout;
    C = shape.C;
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < MAX_DIMS; i++)
        p[i] = shape.p[i];
}

MatShape& MatShape::operator = (ଅଚଳ MatShape& shape)
{
    ଯଦି (ଏହି != &shape) {
        dims = shape.dims;
        layout = shape.layout;
        C = shape.C;
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < MAX_DIMS; i++)
            p[i] = shape.p[i];
    }
    ଫେରନ୍ତୁ *ଏହି;
}

ତାର୍କିକ MatShape::hasSymbols() ଅଚଳ
{
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < dims; i++) {
        ଯଦି (p[i] < 0)
            ଫେରନ୍ତୁ ସତ୍ୟ;
    }
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା MatShape::channels() ଅଚଳ
{
    CV_Assert(layout == DATA_LAYOUT_BLOCK || layout == DATA_LAYOUT_NCHW || layout == DATA_LAYOUT_NHWC);
    ଫେରନ୍ତୁ layout == DATA_LAYOUT_BLOCK ? C : p[layout == DATA_LAYOUT_NCHW ? 1 : dims-1];
}

MatShape MatShape::toLayout(DataLayout newLayout, ପୂର୍ଣ୍ଣସଂଖ୍ୟା C0) ଅଚଳ
{
    CV_Assert(layout == DATA_LAYOUT_BLOCK || layout == DATA_LAYOUT_NCHW || layout == DATA_LAYOUT_NHWC);
    CV_Assert(newLayout == DATA_LAYOUT_BLOCK || newLayout == DATA_LAYOUT_NCHW || newLayout == DATA_LAYOUT_NHWC);

    MatShape newsize = *ଏହି;
    newsize.layout = newLayout;

    ଯଦି (newLayout == DATA_LAYOUT_BLOCK) {
        // any => BLOCK
        CV_Assert_N(C0 > 1, (C0 & (C0-1)) == 0);
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା Corig = channels();
        newsize.C = Corig;

        ଯଦି (layout == DATA_LAYOUT_NHWC) {
            ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 2; i < dims; i++) {
                newsize.p[i] = p[i-1];
            }
        }

        newsize.dims += layout != DATA_LAYOUT_BLOCK;
        newsize.p[1] = (Corig + C0 - 1)/C0;
        newsize.p[newsize.dims-1] = C0;
    } ଅନ୍ୟଥା ଯଦି (layout == DATA_LAYOUT_BLOCK) {
        // BLOCK => any (except for BLOCK, which is handled above)
        newsize.C = 0;
        ଯଦି (newLayout == DATA_LAYOUT_NHWC) {
            ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 2; i < dims; i++) {
                newsize.p[i-1] = p[i];
            }
        }
        newsize.p[newLayout == DATA_LAYOUT_NCHW ? 1 : dims-2] = C;
        newsize.dims--;
    } ଅନ୍ୟଥା {
        CV_Assert_N(C0 <= 1);

        ଯଦି (newLayout == layout)
            ଫେରନ୍ତୁ newsize;

        // NHWC => NCHW
        ଯଦି (newLayout == DATA_LAYOUT_NCHW) {
            ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 2; i < dims; i++)
                newsize.p[i] = p[i-1];
            newsize.p[1] = p[dims-1];
        } ଅନ୍ୟଥା {
            // NCHW => NHWC
            ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 2; i < dims; i++)
                newsize.p[i-1] = p[i];
            newsize.p[dims-1] = p[1];
        }
    }

    ଫେରନ୍ତୁ newsize;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା normalizeShapeAxis(ପୂର୍ଣ୍ଣସଂଖ୍ୟା axis, ପୂର୍ଣ୍ଣସଂଖ୍ୟା dims)
{
    CV_CheckGE(axis, -dims, "axis is out of range");
    CV_CheckLT(axis, dims, "axis is out of range");
    ଫେରନ୍ତୁ axis < 0 ? axis + dims : axis;
}

// The semantic NCHW form {N, C, spatial...} of a block ('NC1HWC0') shape, so that axes and
// element counts are the logical ones rather than the C0-padded ones.
ସ୍ଥିର MatShape toSemantic(ଅଚଳ MatShape& shape)
{
    CV_CheckGE(shape.dims, 3, "a block shape is {N, C1, spatial..., C0}");
    ଫେରନ୍ତୁ shape.toLayout(DATA_LAYOUT_NCHW);
}

// Blocks such a result again. Once N or C changes, elements cross the padded C0 lanes and
// the data needs a repack, which a shape operation cannot express.
ସ୍ଥିର MatShape toBlockOf(MatShape result, ଅଚଳ MatShape& src, ଅଚଳ ଅକ୍ଷର* op)
{
    ଯଦି (result.dims < 2 || result.p[0] != src.p[0] || result.p[1] != src.C)
        CV_Error_(Error::StsNotImplemented,
                  ("%s: the block layout is kept only when the batch and the channel axes stay "
                   "unchanged; convert the shape with toLayout() first", op));
    CV_CheckLE(result.dims + 1, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)MatShape::MAX_DIMS, "too many dimensions");
    result.layout = DATA_LAYOUT_NCHW;
    ଫେରନ୍ତୁ result.toLayout(DATA_LAYOUT_BLOCK, src.p[src.dims-1]);
}

MatShape MatShape::squeeze(ଅଚଳ std::vector<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>& axes) ଅଚଳ
{
    CV_Assert(dims >= 0);
    ଯଦି (layout == DATA_LAYOUT_BLOCK)
        ଫେରନ୍ତୁ toBlockOf(toSemantic(*ଏହି).squeeze(axes), *ଏହି, "squeeze");
    ତାର୍କିକ remove[MAX_DIMS] = {ମିଥ୍ୟା};
    ଯଦି (axes.empty())
    {
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < dims; i++)
            remove[i] = p[i] == 1;
    }
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା a : axes)
    {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = normalizeShapeAxis(a, dims);
        CV_Check(a, !remove[i], "squeeze: duplicate axis");
        CV_Check(p[i], p[i] == 1, "squeeze: the size of a squeezed axis must be 1");
        remove[i] = ସତ୍ୟ;
    }
    // a new number of axes: the layout of this shape does not apply
    MatShape result;
    result.dims = 0;
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < dims; i++)
        ଯଦି (!remove[i])
            result.p[result.dims++] = p[i];
    ଫେରନ୍ତୁ result;
}

MatShape MatShape::unsqueeze(ଅଚଳ std::vector<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>& axes) ଅଚଳ
{
    CV_Assert(dims >= 0);
    ଯଦି (layout == DATA_LAYOUT_BLOCK)
        ଫେରନ୍ତୁ toBlockOf(toSemantic(*ଏହି).unsqueeze(axes), *ଏହି, "unsqueeze");
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା outDims = dims + (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)axes.size();
    CV_CheckLE(outDims, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)MAX_DIMS, "unsqueeze: too many dimensions");
    ତାର୍କିକ insert[MAX_DIMS] = {ମିଥ୍ୟା};
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା a : axes)
    {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = normalizeShapeAxis(a, outDims);
        CV_Check(a, !insert[i], "unsqueeze: duplicate axis");
        insert[i] = ସତ୍ୟ;
    }
    // a new number of axes: the layout of this shape does not apply
    MatShape result;
    result.dims = outDims;
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0, j = 0; i < outDims; i++)
        result.p[i] = insert[i] ? 1 : p[j++];
    ଫେରନ୍ତୁ result;
}

MatShape MatShape::flatten(ପୂର୍ଣ୍ଣସଂଖ୍ୟା startAxis, ପୂର୍ଣ୍ଣସଂଖ୍ୟା endAxis) ଅଚଳ
{
    CV_Assert(dims >= 0);
    ଯଦି (layout == DATA_LAYOUT_BLOCK)
        ଫେରନ୍ତୁ toBlockOf(toSemantic(*ଏହି).flatten(startAxis, endAxis), *ଏହି, "flatten");
    ଯଦି (dims == 0)
    {
        CV_Check(startAxis, startAxis == 0 || startAxis == -1, "flatten: axis is out of range");
        CV_Check(endAxis, endAxis == 0 || endAxis == -1, "flatten: axis is out of range");
        ଫେରନ୍ତୁ MatShape({1});
    }
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା start = normalizeShapeAxis(startAxis, dims), end = normalizeShapeAxis(endAxis, dims);
    CV_CheckLE(start, end, "flatten: startAxis must not follow endAxis");
    int64 merged = 1;
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = start; i <= end; i++)
        merged *= p[i];
    CV_CheckLE((ଦ୍ୱିଗୁଣ)merged, (ଦ୍ୱିଗୁଣ)INT_MAX, "flatten: the result is too big");
    // a new number of axes: the layout of this shape does not apply
    MatShape result;
    result.dims = 0;
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < dims; i++)
    {
        ଯଦି (i == start)
            result.p[result.dims++] = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)merged;
        ଅନ୍ୟଥା ଯଦି (i < start || i > end)
            result.p[result.dims++] = p[i];
    }
    ଫେରନ୍ତୁ result;
}

MatShape MatShape::reshape(ଅଚଳ MatShape& newShape, ତାର୍କିକ allowZero) ଅଚଳ
{
    CV_Assert(dims >= 0 && newShape.dims >= 0);
    ଯଦି (layout == DATA_LAYOUT_BLOCK)
        ଫେରନ୍ତୁ toBlockOf(toSemantic(*ଏହି).reshape(newShape, allowZero), *ଏହି, "reshape");
    // a new number of axes: neither the layout of this shape nor the one of the spec applies
    MatShape result;
    result.dims = newShape.dims;
    std::copy(newShape.p, newShape.p + newShape.dims, result.p);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା inferred = -1;
    int64 known = 1;
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < result.dims; i++)
    {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା& sz = result.p[i];
        ଯଦି (sz == -1)
        {
            CV_Check(i, inferred < 0, "reshape: at most one axis may be -1");
            inferred = i;
            ଜାରି;
        }
        CV_Check(sz, sz >= 0, "reshape: sizes must be -1 or non-negative");
        ଯଦି (sz == 0 && !allowZero)
        {
            CV_Check(i, i < dims, "reshape: 0 refers to an axis the source does not have");
            sz = p[i];
        }
        known *= sz;
    }
    ଅଚଳ int64 total_ = (int64)total();
    ଯଦି (inferred >= 0)
    {
        CV_Check((ଦ୍ୱିଗୁଣ)known, known != 0, "reshape: cannot infer the -1 axis next to a zero-size axis");
        CV_Check((ଦ୍ୱିଗୁଣ)total_, total_ % known == 0, "reshape: the number of elements does not match");
        CV_CheckLE((ଦ୍ୱିଗୁଣ)(total_/known), (ଦ୍ୱିଗୁଣ)INT_MAX, "reshape: the result is too big");
        result.p[inferred] = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(total_/known);
    }
    ଅନ୍ୟଥା ଯଦି (known != total_)
        CV_Error_(Error::StsBadArg, ("reshape: the number of elements does not match: %lld vs %lld",
                                     (long long)known, (long long)total_));
    ଫେରନ୍ତୁ result;
}

MatShape MatShape::expand(ଅଚଳ MatShape& another) ଅଚଳ
{
    ଯଦି (dims == 0)
        ଫେରନ୍ତୁ another;
    ଯଦି (another.dims == 0)
        ଫେରନ୍ତୁ *ଏହି;

    ଯଦି ((layout == DATA_LAYOUT_NCHW || layout == DATA_LAYOUT_NHWC) &&
        (another.layout == DATA_LAYOUT_NCHW || another.layout == DATA_LAYOUT_NHWC)) {
        CV_Assert(layout == another.layout);
    }
    // [TODO] support block layout
    CV_Assert(layout != DATA_LAYOUT_BLOCK && another.layout != DATA_LAYOUT_BLOCK);

    MatShape result;

    ଯଦି (dims < 0 || another.dims < 0)
        ଫେରନ୍ତୁ result;

    result = *ଏହି;
    result.dims = std::max(dims, another.dims);
    result.layout = layout == DATA_LAYOUT_UNKNOWN ? another.layout :
        layout == DATA_LAYOUT_ND && (another.layout == DATA_LAYOUT_NCHW ||
        another.layout == DATA_LAYOUT_NHWC) ? another.layout : layout;
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = result.dims-1; i >= 0; i--) {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା i1 = i - (result.dims - dims);
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା i2 = i - (result.dims - another.dims);
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା sz1 = i1 < 0 ? 1 : p[i1];
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା sz2 = i2 < 0 ? 1 : another.p[i2];
        CV_Assert(sz1 == sz2 || sz1 == 1 || sz2 == 1);
        // [TODO] handle symbolic shapes
        result.p[i] = sz1 != 1 ? sz1 : sz2;
    }
    ଫେରନ୍ତୁ result;
}

/////////////////////////// MatAllocator ////////////////////////////

ଶୂନ୍ୟ MatAllocator::map(UMatData*, AccessFlag) ଅଚଳ
{
}

ଶୂନ୍ୟ MatAllocator::unmap(UMatData* u) ଅଚଳ
{
    ଯଦି(u->urefcount == 0 && u->refcount == 0)
    {
        deallocate(u);
    }
}

ଶୂନ୍ୟ MatAllocator::download(UMatData* u, ଶୂନ୍ୟ* dstptr,
         ପୂର୍ଣ୍ଣସଂଖ୍ୟା dims, ଅଚଳ size_t sz[],
         ଅଚଳ size_t srcofs[], ଅଚଳ size_t srcstep[],
         ଅଚଳ size_t dststep[]) ଅଚଳ
{
    ଯଦି(!u)
        ଫେରନ୍ତୁ;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା isz[CV_MAX_DIM];
    uchar* srcptr = u->data;
    ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < dims; i++ )
    {
        CV_Assert( sz[i] <= (size_t)INT_MAX );
        ଯଦି( sz[i] == 0 )
            ଫେରନ୍ତୁ;
        ଯଦି( srcofs )
            srcptr += srcofs[i]*(i <= dims-2 ? srcstep[i] : 1);
        isz[i] = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)sz[i];
    }

    Mat src(dims, isz, CV_8U, srcptr, srcstep);
    Mat dst(dims, isz, CV_8U, dstptr, dststep);

    ଅଚଳ Mat* arrays[] = { &src, &dst };
    uchar* ptrs[2];
    NAryMatIterator it(arrays, ptrs, 2);
    size_t planesz = it.size;

    ପାଇଁ( size_t j = 0; j < it.nplanes; j++, ++it )
        memcpy(ptrs[1], ptrs[0], planesz);
}


ଶୂନ୍ୟ MatAllocator::upload(UMatData* u, ଅଚଳ ଶୂନ୍ୟ* srcptr, ପୂର୍ଣ୍ଣସଂଖ୍ୟା dims, ଅଚଳ size_t sz[],
                    ଅଚଳ size_t dstofs[], ଅଚଳ size_t dststep[],
                    ଅଚଳ size_t srcstep[]) ଅଚଳ
{
    ଯଦି(!u)
        ଫେରନ୍ତୁ;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା isz[CV_MAX_DIM];
    uchar* dstptr = u->data;
    ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < dims; i++ )
    {
        CV_Assert( sz[i] <= (size_t)INT_MAX );
        ଯଦି( sz[i] == 0 )
            ଫେରନ୍ତୁ;
        ଯଦି( dstofs )
            dstptr += dstofs[i]*(i <= dims-2 ? dststep[i] : 1);
        isz[i] = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)sz[i];
    }

    Mat src(dims, isz, CV_8U, (ଶୂନ୍ୟ*)srcptr, srcstep);
    Mat dst(dims, isz, CV_8U, dstptr, dststep);

    ଅଚଳ Mat* arrays[] = { &src, &dst };
    uchar* ptrs[2];
    NAryMatIterator it(arrays, ptrs, 2);
    size_t planesz = it.size;

    ପାଇଁ( size_t j = 0; j < it.nplanes; j++, ++it )
        memcpy(ptrs[1], ptrs[0], planesz);
}

ଶୂନ୍ୟ MatAllocator::copy(UMatData* usrc, UMatData* udst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା dims, ଅଚଳ size_t sz[],
                  ଅଚଳ size_t srcofs[], ଅଚଳ size_t srcstep[],
                  ଅଚଳ size_t dstofs[], ଅଚଳ size_t dststep[], ତାର୍କିକ /*sync*/) ଅଚଳ
{
    CV_INSTRUMENT_REGION();

    ଯଦି(!usrc || !udst)
        ଫେରନ୍ତୁ;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା isz[CV_MAX_DIM];
    uchar* srcptr = usrc->data;
    uchar* dstptr = udst->data;
    ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < dims; i++ )
    {
        CV_Assert( sz[i] <= (size_t)INT_MAX );
        ଯଦି( sz[i] == 0 )
            ଫେରନ୍ତୁ;
        ଯଦି( srcofs )
            srcptr += srcofs[i]*(i <= dims-2 ? srcstep[i] : 1);
        ଯଦି( dstofs )
            dstptr += dstofs[i]*(i <= dims-2 ? dststep[i] : 1);
        isz[i] = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)sz[i];
    }

    Mat src(dims, isz, CV_8U, srcptr, srcstep);
    Mat dst(dims, isz, CV_8U, dstptr, dststep);

    ଅଚଳ Mat* arrays[] = { &src, &dst };
    uchar* ptrs[2];
    NAryMatIterator it(arrays, ptrs, 2);
    size_t planesz = it.size;

    ପାଇଁ( size_t j = 0; j < it.nplanes; j++, ++it )
        memcpy(ptrs[1], ptrs[0], planesz);
}

BufferPoolController* MatAllocator::getBufferPoolController(ଅଚଳ ଅକ୍ଷର* id) ଅଚଳ
{
    CV_UNUSED(id);
    ସ୍ଥିର DummyBufferPoolController dummy;
    ଫେରନ୍ତୁ &dummy;
}

ଶ୍ରେଣୀ StdMatAllocator CV_FINAL : ସର୍ବସାଧାରଣ MatAllocator
{
ସର୍ବସାଧାରଣ:
    UMatData* allocate(ପୂର୍ଣ୍ଣସଂଖ୍ୟା dims, ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା* sizes, ପୂର୍ଣ୍ଣସଂଖ୍ୟା type,
                       ଶୂନ୍ୟ* data0, size_t* step, AccessFlag /*flags*/, UMatUsageFlags /*usageFlags*/) ଅଚଳ CV_OVERRIDE
    {
        size_t total = CV_ELEM_SIZE(type);
        ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = dims-1; i >= 0; i-- )
        {
            ଯଦି( step )
            {
                ଯଦି( data0 && step[i] != CV_AUTOSTEP )
                {
                    CV_Assert(total <= step[i]);
                    total = step[i];
                }
                ଅନ୍ୟଥା
                    step[i] = total;
            }
            total *= sizes[i];
        }
        uchar* data = data0 ? (uchar*)data0 : (uchar*)fastMalloc(total);
        UMatData* u = ନୂତନ UMatData(ଏହି);
        u->data = u->origdata = data;
        u->size = total;
        ଯଦି(data0)
            u->flags |= UMatData::USER_ALLOCATED;

        ଫେରନ୍ତୁ u;
    }

    ତାର୍କିକ allocate(UMatData* u, AccessFlag /*accessFlags*/, UMatUsageFlags /*usageFlags*/) ଅଚଳ CV_OVERRIDE
    {
        ଯଦି(!u) ଫେରନ୍ତୁ ମିଥ୍ୟା;
        ଫେରନ୍ତୁ ସତ୍ୟ;
    }

    ଶୂନ୍ୟ deallocate(UMatData* u) ଅଚଳ CV_OVERRIDE
    {
        ଯଦି(!u)
            ଫେରନ୍ତୁ;

        CV_Assert(u->urefcount == 0);
        CV_Assert(u->refcount == 0);
        ଯଦି( !(u->flags & UMatData::USER_ALLOCATED) )
        {
            fastFree(u->origdata);
            u->origdata = 0;
        }
        delete u;
    }
};

ସ୍ଥିର
MatAllocator*& getDefaultAllocatorMatRef()
{
    ସ୍ଥିର MatAllocator* g_matAllocator = Mat::getStdAllocator();
    ଫେରନ୍ତୁ g_matAllocator;
}

MatAllocator* Mat::getDefaultAllocator()
{
    ଫେରନ୍ତୁ getDefaultAllocatorMatRef();
}

ଶୂନ୍ୟ Mat::setDefaultAllocator(MatAllocator* allocator)
{
    getDefaultAllocatorMatRef() = allocator;
}

MatAllocator* Mat::getStdAllocator()
{
    CV_SINGLETON_LAZY_INIT(MatAllocator, ନୂତନ StdMatAllocator())
}

//==================================================================================================

ଶୂନ୍ୟ setSize( Mat& m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା _dims, ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା* _sz, ଅଚଳ size_t* _steps, ତାର୍କିକ autoSteps)
{
    CV_Assert( 0 <= _dims && _dims <= CV_MAX_DIM && _dims <= MatShape::MAX_DIMS);

    m.dims = _dims;
    m.size = MatShape(_dims, _sz);
    m.step[std::max(_dims-1, 0)] = CV_ELEM_SIZE(m.flags);
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = _dims-2; i >= 0; i--) {
        size_t autostep = m.size[i+1]*m.step[i+1];
        ଯଦି (_steps) {
            m.step[i] = _steps[i];
            //CV_Assert(m.step[i] >= autostep);
        } ଅନ୍ୟଥା ଯଦି (autoSteps) {
            m.step[i] = autostep;
        } ଅନ୍ୟଥା {
            m.step[i] = 0;
        }
    }

    ଯଦି( _dims <= 2 )
    {
        m.cols = _dims == 0 ? 1 : _sz ? _sz[_dims > 1] : 0;
        m.rows = _dims < 2 ? 1 : _sz ? _sz[0] : 0;
    } ଅନ୍ୟଥା {
        m.cols = m.rows = -1;
    }
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା updateContinuityFlag(ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags, ପୂର୍ଣ୍ଣସଂଖ୍ୟା dims, ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା* size, ଅଚଳ size_t* step)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, j;
    ଯଦି (dims <= 1)
        ଫେରନ୍ତୁ flags | Mat::CONTINUOUS_FLAG;
    ପାଇଁ( i = 0; i < dims; i++ )
    {
        ଯଦି( size[i] > 1 )
            ଭାଙ୍ଗ;
    }

    uint64 t = (uint64)size[std::min(i, dims-1)]*CV_MAT_CN(flags);
    ପାଇଁ( j = dims-1; j > i; j-- )
    {
        t *= size[j];
        ଯଦି( step[j]*size[j] < step[j-1] )
            ଭାଙ୍ଗ;
    }

    ଯଦି( j <= i && t == (uint64)(ପୂର୍ଣ୍ଣସଂଖ୍ୟା)t )
        ଫେରନ୍ତୁ flags | Mat::CONTINUOUS_FLAG;
    ଫେରନ୍ତୁ flags & ~Mat::CONTINUOUS_FLAG;
}

ଶୂନ୍ୟ Mat::updateContinuityFlag()
{
    flags = cv::updateContinuityFlag(flags, dims, size.p, step.p);
}

ଶୂନ୍ୟ finalizeHdr(Mat& m)
{
    m.updateContinuityFlag();
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା d = m.dims;
    ଯଦି( d > 2 )
        m.rows = m.cols = -1;
    ଯଦି(m.u)
        m.datastart = m.data = m.u->data;
    ଯଦି( m.data && d > 0 )
    {
        m.datalimit = m.datastart + m.size[0]*m.step[0];
        ଯଦି( m.size[0] > 0 )
        {
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା lastdim = d > 0 ? d - 1 : 0;
            m.dataend = m.ptr() + m.size[lastdim]*m.step[lastdim];
            ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < d-1; i++ )
                m.dataend += (m.size[i] - 1)*m.step[i];
        }
        ଅନ୍ୟଥା
            m.dataend = m.datalimit;
    }
    ଅନ୍ୟଥା
        m.dataend = m.datalimit = m.data;
}

//======================================= Mat ======================================================

Mat::Mat() CV_NOEXCEPT
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{}

Mat::Mat(ପୂର୍ଣ୍ଣସଂଖ୍ୟା _rows, ପୂର୍ଣ୍ଣସଂଖ୍ୟା _cols, ପୂର୍ଣ୍ଣସଂଖ୍ୟା _type)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_rows, _cols, _type);
}

Mat::Mat(ପୂର୍ଣ୍ଣସଂଖ୍ୟା _rows, ପୂର୍ଣ୍ଣସଂଖ୍ୟା _cols, ପୂର୍ଣ୍ଣସଂଖ୍ୟା _type, ଅଚଳ Scalar& _s)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_rows, _cols, _type);
    *ଏହି = _s;
}

Mat::Mat(Size _sz, ପୂର୍ଣ୍ଣସଂଖ୍ୟା _type)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create( _sz.height, _sz.width, _type );
}

Mat::Mat(Size _sz, ପୂର୍ଣ୍ଣସଂଖ୍ୟା _type, ଅଚଳ Scalar& _s)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_sz.height, _sz.width, _type);
    *ଏହି = _s;
}

Mat::Mat(ପୂର୍ଣ୍ଣସଂଖ୍ୟା _dims, ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା* _sz, ପୂର୍ଣ୍ଣସଂଖ୍ୟା _type)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_dims, _sz, _type);
}

Mat::Mat(ପୂର୍ଣ୍ଣସଂଖ୍ୟା _dims, ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା* _sz, ପୂର୍ଣ୍ଣସଂଖ୍ୟା _type, ଅଚଳ Scalar& _s)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_dims, _sz, _type);
    *ଏହି = _s;
}

Mat::Mat(ଅଚଳ std::vector<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>& _sz, ପୂର୍ଣ୍ଣସଂଖ୍ୟା _type)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_sz, _type);
}

Mat::Mat(ଅଚଳ std::vector<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>& _sz, ପୂର୍ଣ୍ଣସଂଖ୍ୟା _type, ଅଚଳ Scalar& _s)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_sz, _type);
    *ଏହି = _s;
}

Mat::Mat(ଅଚଳ MatShape& _shape, ପୂର୍ଣ୍ଣସଂଖ୍ୟା _type)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_shape, _type);
}

Mat::Mat(std::initializer_list<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> _shape, ପୂର୍ଣ୍ଣସଂଖ୍ୟା _type)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_shape, _type);
}

Mat::Mat(ଅଚଳ MatShape& _shape, ପୂର୍ଣ୍ଣସଂଖ୍ୟା _type, ଅଚଳ Scalar& _s)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_shape, _type);
    *ଏହି = _s;
}

Mat::Mat(std::initializer_list<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> _shape, ପୂର୍ଣ୍ଣସଂଖ୍ୟା _type, ଅଚଳ Scalar& _s)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    create(_shape, _type);
    *ଏହି = _s;
}

Mat::Mat(ଅଚଳ Mat& m)
    : flags(m.flags), dims(m.dims), rows(m.rows), cols(m.cols), data(m.data),
      datastart(m.datastart), dataend(m.dataend), datalimit(m.datalimit), allocator(m.allocator),
      u(m.u), size(m.size), step(m.step)
{
    ଯଦି( u )
        CV_XADD(&u->refcount, 1);
}

Mat::Mat(ପୂର୍ଣ୍ଣସଂଖ୍ୟା _rows, ପୂର୍ଣ୍ଣସଂଖ୍ୟା _cols, ପୂର୍ଣ୍ଣସଂଖ୍ୟା _type, ଶୂନ୍ୟ* _data, size_t _step)
    : flags(MAGIC_VAL + (_type & TYPE_MASK)), dims(2), rows(_rows), cols(_cols),
      data((uchar*)_data), datastart((uchar*)_data), dataend(0), datalimit(0),
      allocator(0), u(0), size(2)
{
    CV_Assert(total() == 0 || data != ଖାଲି);

    size_t esz = CV_ELEM_SIZE(_type), esz1 = CV_ELEM_SIZE1(_type);
    size_t minstep = cols * esz;
    ଯଦି( _step == AUTO_STEP )
    {
        _step = minstep;
    }
    ଅନ୍ୟଥା
    {
        CV_Assert( _step >= minstep );
        ଯଦି (_step % esz1 != 0)
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

Mat::Mat(Size _sz, ପୂର୍ଣ୍ଣସଂଖ୍ୟା _type, ଶୂନ୍ୟ* _data, size_t _step)
    : flags(MAGIC_VAL + (_type & TYPE_MASK)), dims(2), rows(_sz.height), cols(_sz.width),
      data((uchar*)_data), datastart((uchar*)_data), dataend(0), datalimit(0),
      allocator(0), u(0), size(2)
{
    CV_Assert(total() == 0 || data != ଖାଲି);

    size_t esz = CV_ELEM_SIZE(_type), esz1 = CV_ELEM_SIZE1(_type);
    size_t minstep = cols*esz;
    ଯଦି( _step == AUTO_STEP || (_step < minstep && rows == 1))
    {
        _step = minstep;
    }
    ଅନ୍ୟଥା
    {
        CV_CheckGE(_step, minstep, "");

        ଯଦି (_step % esz1 != 0)
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

Mat& Mat::operator = (ଅଚଳ Mat& m)
{
    ଯଦି( ଏହି != &m )
    {
        ଯଦି( m.u )
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
    ଫେରନ୍ତୁ *ଏହି;
}

Mat Mat::clone() ଅଚଳ
{
    Mat m;
    copyTo(m);
    ଫେରନ୍ତୁ m;
}

ଶୂନ୍ୟ Mat::assignTo( Mat& m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା _type ) ଅଚଳ
{
    ଯଦି( _type < 0 )
        m = *ଏହି;
    ଅନ୍ୟଥା
        convertTo(m, _type);
}

ଶୂନ୍ୟ Mat::create(ପୂର୍ଣ୍ଣସଂଖ୍ୟା _rows, ପୂର୍ଣ୍ଣସଂଖ୍ୟା _cols, ପୂର୍ଣ୍ଣସଂଖ୍ୟା _type)
{
    _type &= TYPE_MASK;
    ଯଦି( dims <= 2 && rows == _rows && cols == _cols && type() == _type && data )
        ଫେରନ୍ତୁ;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା sz[] = {_rows, _cols};
    create(2, sz, _type);
}

ଶୂନ୍ୟ Mat::create(Size _sz, ପୂର୍ଣ୍ଣସଂଖ୍ୟା _type)
{
    create(_sz.height, _sz.width, _type);
}

ଶୂନ୍ୟ Mat::createSameSize(ନିବେଶ_Array m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା type)
{
    MatShape msize = m.shape();
    create(msize, type);
}

ଶୂନ୍ୟ Mat::fit(ପୂର୍ଣ୍ଣସଂଖ୍ୟା _dims, ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା* _sizes, ପୂର୍ଣ୍ଣସଂଖ୍ୟା _type)
{
    size_t oldTotalBytes = u ? u->size : 0;
    size_t esz = CV_ELEM_SIZE(_type), newTotal = _dims >= 0;
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < _dims; i++)
        newTotal *= _sizes[i];
    size_t newTotalBytes = newTotal*esz;
    ଯଦି (newTotalBytes > 0 && (!isContinuous() ||
                              newTotalBytes > oldTotalBytes ||
                              data != datastart)) {
        create(_dims, _sizes, _type);
    } ଅନ୍ୟଥା {
        flags = (flags & ~Mat::TYPE_MASK) | CV_MAT_TYPE(_type);
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା _dummy_size = 0;
        setSize(*ଏହି, (_dims >= 0 ? _dims : 1), (_dims >= 0 ? _sizes : &_dummy_size), nullptr, ସତ୍ୟ);
        finalizeHdr(*ଏହି);
    }
}

ଶୂନ୍ୟ Mat::fit(ଅଚଳ std::vector<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>& _shape, ପୂର୍ଣ୍ଣସଂଖ୍ୟା _type)
{
    fit((ପୂର୍ଣ୍ଣସଂଖ୍ୟା)_shape.size(), _shape.data(), _type);
}

ଶୂନ୍ୟ Mat::fit(ଅଚଳ MatShape& _shape, ପୂର୍ଣ୍ଣସଂଖ୍ୟା _type)
{
    fit(_shape.dims, _shape.p, _type);
    size.layout = _shape.layout;
    size.C = _shape.C;
}

ଶୂନ୍ୟ Mat::fit(std::initializer_list<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> _shape, ପୂର୍ଣ୍ଣସଂଖ୍ୟା _type)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା new_shape[MatShape::MAX_DIMS];
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା new_ndims = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)_shape.size();
    CV_Assert(new_ndims <= MatShape::MAX_DIMS);
    auto it = _shape.begin();
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < new_ndims; i++, ++it)
        new_shape[i] = *it;
    fit(new_ndims, new_shape, _type);
}

ଶୂନ୍ୟ Mat::fit(ପୂର୍ଣ୍ଣସଂଖ୍ୟା _rows, ପୂର୍ଣ୍ଣସଂଖ୍ୟା _cols, ପୂର୍ଣ୍ଣସଂଖ୍ୟା _type)
{
    _type &= TYPE_MASK;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା sz[] = {_rows, _cols};
    fit(2, sz, _type);
}

ଶୂନ୍ୟ Mat::fit(Size _sz, ପୂର୍ଣ୍ଣସଂଖ୍ୟା _type)
{
    fit(_sz.height, _sz.width, _type);
}

ଶୂନ୍ୟ Mat::fitSameSize(ନିବେଶ_Array m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା _type)
{
    fit(m.shape(), _type);
}

ଶୂନ୍ୟ Mat::addref()
{
    ଯଦି( u )
        CV_XADD(&u->refcount, 1);
}

ଶୂନ୍ୟ Mat::release()
{
    ଯଦି( u && CV_XADD(&u->refcount, -1) == 1 )
        deallocate();
    u = ଖାଲି;
    datastart = dataend = datalimit = data = 0;
    ପାଇଁ(ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < dims; i++)
        size.p[i] = 0;
    flags = (flags & CV_MAT_TYPE_MASK) | MAGIC_VAL;
    dims = rows = cols = 0;
    size.clear();
}

size_t Mat::step1(ପୂର୍ଣ୍ଣସଂଖ୍ୟା i) ଅଚଳ
{
    ଫେରନ୍ତୁ step.p[i] / elemSize1();
}

ତାର୍କିକ Mat::empty() ଅଚଳ
{
    ଫେରନ୍ତୁ data == 0 || total() == 0;
}

size_t Mat::total() ଅଚଳ
{
    ଯଦି( dims <= 2 )
        ଫେରନ୍ତୁ (size_t)rows * cols;
    size_t p = 1;
    ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < dims; i++ )
        p *= size[i];
    ଫେରନ୍ତୁ p;
}

size_t Mat::total(ପୂର୍ଣ୍ଣସଂଖ୍ୟା startDim, ପୂର୍ଣ୍ଣସଂଖ୍ୟା endDim) ଅଚଳ
{
    CV_Assert( 0 <= startDim && startDim <= endDim);
    size_t p = 1;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା endDim_ = endDim <= dims ? endDim : dims;
    ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = startDim; i < endDim_; i++ )
        p *= size[i];
    ଫେରନ୍ତୁ p;
}

Mat::Mat(Mat&& m) CV_NOEXCEPT
    : flags(m.flags), dims(m.dims), rows(m.rows), cols(m.cols), data(m.data),
      datastart(m.datastart), dataend(m.dataend), datalimit(m.datalimit), allocator(m.allocator),
      u(m.u), size(m.size), step(m.step)
{
    m.flags = MAGIC_VAL; m.dims = m.rows = m.cols = 0;
    m.size.clear(); m.step.clear();
    m.data = ଖାଲି; m.datastart = ଖାଲି; m.dataend = ଖାଲି; m.datalimit = ଖାଲି;
    m.allocator = ଖାଲି;
    m.u = ଖାଲି;
}


Mat& Mat::operator = (Mat&& m)
{
    ଯଦି (ଏହି == &m)
      ଫେରନ୍ତୁ *ଏହି;

    release();
    flags = m.flags; dims = m.dims; rows = m.rows; cols = m.cols; data = m.data;
    size = m.size; step = m.step;
    datastart = m.datastart; dataend = m.dataend; datalimit = m.datalimit; allocator = m.allocator;
    u = m.u;
    m.flags = MAGIC_VAL; m.dims = m.rows = m.cols = 0;
    m.size.clear(); m.step.clear();
    m.data = ଖାଲି; m.datastart = ଖାଲି; m.dataend = ଖାଲି; m.datalimit = ଖାଲି;
    m.allocator = ଖାଲି;
    m.u = ଖାଲି;
    ଫେରନ୍ତୁ *ଏହି;
}


ଶୂନ୍ୟ Mat::create(ପୂର୍ଣ୍ଣସଂଖ୍ୟା d0, ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା* _sizes, ପୂର୍ଣ୍ଣସଂଖ୍ୟା _type)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା sz1 = 1, d = d0;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ଯଦି (d == 0) {
        d = 1;
        _sizes = (ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା*)&sz1;
    }
    CV_Assert(0 <= d && d <= CV_MAX_DIM && _sizes);
    _type = CV_MAT_TYPE(_type);

    ଯଦି( data && d == dims && _type == type() )
    {
        ପାଇଁ( i = 0; i < d; i++ )
            ଯଦି( size[i] != _sizes[i] )
                ଭାଙ୍ଗ;
        ଯଦି( i == d )
            ଫେରନ୍ତୁ;
    }

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା _sizes_backup[CV_MAX_DIM]; // #5991
    ଯଦି (_sizes == (ଏହି->size.p))
    {
        ପାଇଁ(i = 0; i < d; i++ )
            _sizes_backup[i] = _sizes[i];
        _sizes = _sizes_backup;
    }

    release();
    ଯଦି( d == 0 )
        ଫେରନ୍ତୁ;
    flags = (_type & CV_MAT_TYPE_MASK) | MAGIC_VAL;
    setSize(*ଏହି, d, _sizes, 0, ସତ୍ୟ);

    ଯଦି( total() > 0 )
    {
        MatAllocator *a = allocator, *a0 = getDefaultAllocator();
        ଯଦି(!a)
            a = a0;
        ଚେଷ୍ଟା
        {
            u = a->allocate(dims, size.p, _type, 0, step.p, ACCESS_RW /* ignored */, USAGE_DEFAULT);
            CV_Assert(u != 0);
            allocator = a;
        }
        ଧର (...)
        {
            ଯଦି (a == a0)
                ଫିଙ୍ଗ;
            u = a0->allocate(dims, size.p, _type, 0, step.p, ACCESS_RW /* ignored */, USAGE_DEFAULT);
            CV_Assert(u != 0);
            allocator = a0;
        }
        CV_Assert( step[dims-1] == (size_t)CV_ELEM_SIZE(flags) );
    }

    addref();
    finalizeHdr(*ଏହି);
    size.dims = dims = d0;
}

ଶୂନ୍ୟ Mat::create(ଅଚଳ std::vector<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>& _sizes, ପୂର୍ଣ୍ଣସଂଖ୍ୟା _type)
{
    create((ପୂର୍ଣ୍ଣସଂଖ୍ୟା)_sizes.size(), _sizes.data(), _type);
}

ଶୂନ୍ୟ Mat::create(ଅଚଳ MatShape& _shape, ପୂର୍ଣ୍ଣସଂଖ୍ୟା _type)
{
    ଯଦି (_shape.dims < 0) {
        release();
        ଫେରନ୍ତୁ;
    }
    create(_shape.dims, _shape.p, _type);
    size.layout = _shape.layout;
    size.C = _shape.C;
}

ଶୂନ୍ୟ Mat::create(std::initializer_list<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> _shape, ପୂର୍ଣ୍ଣସଂଖ୍ୟା _type)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା new_shape[MatShape::MAX_DIMS];
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା new_ndims = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)_shape.size();
    CV_Assert(new_ndims <= MatShape::MAX_DIMS);
    auto it = _shape.begin();
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < new_ndims; i++, ++it)
        new_shape[i] = *it;

    create(new_ndims, new_shape, _type);
}

ଶୂନ୍ୟ Mat::copySize(ଅଚଳ Mat& m)
{
    dims = m.dims;
    cols = m.cols;
    rows = m.rows;
    size = m.size;
    step = m.step;
}

ଶୂନ୍ୟ Mat::deallocate()
{
    ଯଦି(u)
    {
        UMatData* u_ = u;
        u = ଖାଲି;
        (u_->currAllocator ? u_->currAllocator : allocator ? allocator : getDefaultAllocator())->unmap(u_);
    }
}

Mat::Mat(ଅଚଳ Mat& m, ଅଚଳ Range& _rowRange, ଅଚଳ Range& _colRange)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    CV_Assert(m.dims <= MatShape::MAX_DIMS);
    CV_Assert(m.dims >= 2 ||
              (m.dims == 1 && (_rowRange == Range::all() ||
                               _rowRange == Range(0, m.rows))));
    ଯଦି( m.dims > 2 )
    {
        Range rs[MatShape::MAX_DIMS];
        rs[0] = _rowRange;
        rs[1] = _colRange;
        ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 2; i < m.dims; i++ )
            rs[i] = Range::all();
        *ଏହି = m(rs);
        ଫେରନ୍ତୁ;
    }

    *ଏହି = m;
    ଚେଷ୍ଟା
    {
        ଯଦି( _rowRange != Range::all() && _rowRange != Range(0,rows) )
        {
            CV_Assert( 0 <= _rowRange.start && _rowRange.start <= _rowRange.end
                       && _rowRange.end <= m.rows );
            size[0] = rows = _rowRange.size();
            data += step*_rowRange.start;
            flags |= SUBMATRIX_FLAG;
        }

        ଯଦି( _colRange != Range::all() && _colRange != Range(0,cols) )
        {
            CV_Assert( 0 <= _colRange.start && _colRange.start <= _colRange.end
                       && _colRange.end <= m.cols );
            size[dims > 1] = cols = _colRange.size();
            data += _colRange.start*elemSize();
            flags |= SUBMATRIX_FLAG;
        }
    }
    ଧର(...)
    {
        release();
        ଫିଙ୍ଗ;
    }

    updateContinuityFlag();

    ଯଦି( rows <= 0 || cols <= 0 )
    {
        release();
        rows = cols = 0;
    }
}


Mat::Mat(ଅଚଳ Mat& m, ଅଚଳ Rect& roi)
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
    ଯଦି( roi.width < m.cols || roi.height < m.rows )
        flags |= SUBMATRIX_FLAG;

    size[0] = rows;
    size[1] = cols;
    step[0] = m.step[0];
    step[1] = esz;
    updateContinuityFlag();

    addref();
    ଯଦି( rows <= 0 || cols <= 0 )
    {
        rows = cols = 0;
        release();
    }
}


Mat::Mat(ପୂର୍ଣ୍ଣସଂଖ୍ୟା _dims, ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା* _sizes, ପୂର୍ଣ୍ଣସଂଖ୍ୟା _type, ଶୂନ୍ୟ* _data, ଅଚଳ size_t* _steps)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    flags |= CV_MAT_TYPE(_type);
    datastart = data = (uchar*)_data;
    ଯଦି (_dims > 0 || _data != nullptr)
        setSize(*ଏହି, _dims, _sizes, _steps, ସତ୍ୟ);
    finalizeHdr(*ଏହି);
}


Mat::Mat(ଅଚଳ std::vector<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>& _sizes, ପୂର୍ଣ୍ଣସଂଖ୍ୟା _type, ଶୂନ୍ୟ* _data, ଅଚଳ size_t* _steps)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    flags |= CV_MAT_TYPE(_type);
    datastart = data = (uchar*)_data;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା _dims = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)_sizes.size();
    ଯଦି (_dims > 0 || _data != nullptr)
        setSize(*ଏହି, _dims, _sizes.data(), _steps, ସତ୍ୟ);
    finalizeHdr(*ଏହି);
}

Mat::Mat(ଅଚଳ MatShape& _shape, ପୂର୍ଣ୍ଣସଂଖ୍ୟା _type, ଶୂନ୍ୟ* _data, ଅଚଳ size_t* _steps)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    flags |= CV_MAT_TYPE(_type);
    datastart = data = (uchar*)_data;
    ଯଦି (_shape.dims >= 0) {
        setSize(*ଏହି, _shape.dims, _shape.p, _steps, ସତ୍ୟ);
    }
    ଅନ୍ୟଥା {
        CV_Assert(!data);
    }
    finalizeHdr(*ଏହି);
}

Mat::Mat(std::initializer_list<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> _shape, ପୂର୍ଣ୍ଣସଂଖ୍ୟା _type, ଶୂନ୍ୟ* _data, ଅଚଳ size_t* _steps)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା new_shape[MatShape::MAX_DIMS];
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା _dims = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)_shape.size();
    CV_Assert(_dims <= MatShape::MAX_DIMS);
    auto it = _shape.begin();
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < _dims; i++, ++it)
        new_shape[i] = *it;

    flags |= CV_MAT_TYPE(_type);
    datastart = data = (uchar*)_data;
    ଯଦି (_dims > 0 || _data != nullptr)
        setSize(*ଏହି, _dims, new_shape, _steps, ସତ୍ୟ);
    finalizeHdr(*ଏହି);
}

Mat::Mat(ଅଚଳ Mat& m, ଅଚଳ Range* ranges)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା d = m.dims;

    CV_Assert(ranges);
    ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < d; i++ )
    {
        Range r = ranges[i];
        CV_Assert( r == Range::all() || (0 <= r.start && r.start < r.end && r.end <= m.size[i]) );
    }
    *ଏହି = m;
    ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < d; i++ )
    {
        Range r = ranges[i];
        ଯଦି( r != Range::all() && r != Range(0, size.p[i]))
        {
            size.p[i] = r.end - r.start;
            data += r.start*step.p[i];
            flags |= SUBMATRIX_FLAG;
        }
    }

    ଯଦି (d <= 2) {
        rows = d == 2 ? size[0] : 1;
        cols = d <= 0 ? (d >= 0) : size[d > 1];
    }
    updateContinuityFlag();
}

Mat::Mat(ଅଚଳ Mat& m, ଅଚଳ std::vector<Range>& ranges)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
    datalimit(0), allocator(0), u(0)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା d = m.dims;

    CV_Assert((ପୂର୍ଣ୍ଣସଂଖ୍ୟା)ranges.size() == d);
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < d; i++)
    {
        Range r = ranges[i];
        CV_Assert(r == Range::all() || (0 <= r.start && r.start < r.end && r.end <= m.size[i]));
    }
    *ଏହି = m;
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < d; i++)
    {
        Range r = ranges[i];
        ଯଦି (r != Range::all() && r != Range(0, size.p[i]))
        {
            size.p[i] = r.end - r.start;
            data += r.start*step.p[i];
            flags |= SUBMATRIX_FLAG;
        }
    }

    ଯଦି (d <= 2) {
        rows = d == 2 ? size[0] : 1;
        cols = d <= 0 ? (d >= 0) : size[d > 1];
    }
    updateContinuityFlag();
}


Mat Mat::diag(ପୂର୍ଣ୍ଣସଂଖ୍ୟା d) ଅଚଳ
{
    CV_Assert( dims == 2 );
    Mat m = *ଏହି;
    size_t esz = elemSize();
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା len;

    ଯଦି( d >= 0 )
    {
        len = std::min(cols - d, rows);
        m.data += esz*d;
    }
    ଅନ୍ୟଥା
    {
        len = std::min(rows + d, cols);
        m.data -= step[0]*d;
    }
    CV_DbgAssert( len > 0 );

    m.size[0] = m.rows = len;
    m.size[1] = m.cols = 1;
    m.step[0] += (len > 1 ? esz : 0);

    m.updateContinuityFlag();

    ଯଦି( size() != Size(1,1) )
        m.flags |= SUBMATRIX_FLAG;

    ଫେରନ୍ତୁ m;
}


ଶୂନ୍ୟ Mat::pop_back(size_t nelems)
{
    CV_Assert( nelems <= (size_t)size.p[0] );
    CV_Assert( dims >= 1 );

    ଯଦି( isSubmatrix() )
        *ଏହି = rowRange(0, size.p[0] - (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)nelems);
    ଅନ୍ୟଥା
    {
        size.p[0] -= (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)nelems;
        ଯଦି (dims == 2)
            rows = size.p[0];
        ଅନ୍ୟଥା ଯଦି (dims == 1)
            cols = size.p[0];
        dataend -= nelems*step.p[0];
    }
}


ଶୂନ୍ୟ Mat::ଠେଲ_back(ଅଚଳ ଶୂନ୍ୟ* elem)
{
    size_t r = size.p[0];
    ଯଦି( isSubmatrix() || dataend + step.p[0] > datalimit )
        reserve( std::max(r + 1, (r*3+1)/2) );

    size_t esz = elemSize();
    memcpy(data + r*step.p[0], elem, esz);
    size.p[0] = ପୂର୍ଣ୍ଣସଂଖ୍ୟା(r + 1);
    ଯଦି (dims == 2)
        rows = size.p[0];
    ଅନ୍ୟଥା ଯଦି (dims == 1)
        cols = size.p[0];
    dataend += step.p[0];
    uint64 tsz = size.p[0];
    ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 1; i < dims; i++ )
        tsz *= size.p[i];
    ଯଦି( esz < step.p[0] || tsz != (uint64)(ପୂର୍ଣ୍ଣସଂଖ୍ୟା)tsz )
        flags &= ~CONTINUOUS_FLAG;
}


ଶୂନ୍ୟ Mat::reserve(size_t nelems)
{
    ଅଚଳ size_t MIN_SIZE = 64;

    CV_Assert( (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)nelems >= 0 );
    ଯଦି( !isSubmatrix() && step.p[0] != 0 && data + step.p[0]*nelems <= datalimit )
        ଫେରନ୍ତୁ;

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା r = size.p[0];
    ଯଦି( (size_t)r >= nelems)
        ଫେରନ୍ତୁ;

    MatShape newsize = size;
    size_t esz = elemSize();
    newsize.dims = std::max(newsize.dims, 1);
    ଯଦି (dims > 1 && r == 0 && step.p[0] == 0) {
        step.p[0] = size.p[1] * (dims == 2 ? esz : step.p[1]);
    }
    newsize.p[0] = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)nelems;
    size_t newbytes = newsize.total() * esz;
    ଯଦି (newbytes < MIN_SIZE) {
        newsize.p[0] = 1;
        newsize.p[0] = ପୂର୍ଣ୍ଣସଂଖ୍ୟା(MIN_SIZE / newsize.total() / esz);
    }

    Mat m(newsize, type());
    ଯଦି( r > 0)
    {
        Mat mpart = m.rowRange(0, r);
        copyTo(mpart);
    }

    *ଏହି = m;
    size.p[0] = r;

    ଯଦି (dims == 2)
        rows = size.p[0];
    ଅନ୍ୟଥା ଯଦି (dims == 1)
        cols = size.p[0];
    dataend = data + step.p[0]*r;
}


ଶୂନ୍ୟ Mat::reserveBuffer(size_t nbytes)
{
    size_t esz = 1;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା mtype = CV_8UC1;
    ଯଦି (!empty())
    {
        ଯଦି (!isSubmatrix() && data + nbytes <= dataend)//Should it be datalimit?
            ଫେରନ୍ତୁ;
        esz = elemSize();
        mtype = type();
    }

    size_t nelems = (nbytes - 1) / esz + 1;

#ଯଦି SIZE_MAX > UINT_MAX
    CV_Assert(nelems <= size_t(INT_MAX)*size_t(INT_MAX));
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା newrows = nelems > size_t(INT_MAX) ? nelems > 0x400*size_t(INT_MAX) ? nelems > 0x100000 * size_t(INT_MAX) ? nelems > 0x40000000 * size_t(INT_MAX) ?
                  size_t(INT_MAX) : 0x40000000 : 0x100000 : 0x400 : 1;
#ଅନ୍ୟଥା
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା newrows = nelems > size_t(INT_MAX) ? 2 : 1;
#endif
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା newcols = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)((nelems - 1) / newrows + 1);

    create(newrows, newcols, mtype);
}


ଶୂନ୍ୟ Mat::resize(size_t nelems)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା r = size.p[0];
    ଯଦି( r == (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)nelems )
        ଫେରନ୍ତୁ;
    CV_Assert( (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)nelems >= 0 );

    ଯଦି( isSubmatrix() || data + step.p[0]*nelems > datalimit )
        reserve(nelems);

    size.p[0] = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)nelems;
    ଯଦି (dims == 2)
        rows = size.p[0];
    ଅନ୍ୟଥା ଯଦି (dims == 1)
        cols = size.p[0];
    dataend += (size.p[0] - r)*(int64_t)step.p[0];

    //updateContinuityFlag(*this);
}


ଶୂନ୍ୟ Mat::resize(size_t nelems, ଅଚଳ Scalar& s)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା saveRows = size.p[0];
    resize(nelems);

    ଯଦି( size.p[0] > saveRows )
    {
        Mat part = rowRange(saveRows, size.p[0]);
        part = s;
    }
}

ଶୂନ୍ୟ Mat::push_back(ଅଚଳ Mat& elems)
{
    size_t r = size.p[0];
    size_t delta = elems.size.p[0];
    ଯଦି( delta == 0 )
        ଫେରନ୍ତୁ;
    ଯଦି( ଏହି == &elems )
    {
        Mat tmp = elems;
        push_back(tmp);
        ଫେରନ୍ତୁ;
    }
    ଯଦି( !data )
    {
        *ଏହି = elems.clone();
        ଫେରନ୍ତୁ;
    }
    CV_Assert(dims > 0);

    size.p[0] = elems.size.p[0];
    ତାର୍କିକ eq = size == elems.size;
    size.p[0] = ପୂର୍ଣ୍ଣସଂଖ୍ୟା(r);
    ଯଦି( !eq )
        CV_Error(cv::Error::StsUnmatchedSizes, "Pushed vector length is not equal to matrix row length");
    ଯଦି( type() != elems.type() )
        CV_Error(cv::Error::StsUnmatchedFormats, "Pushed vector type is not the same as matrix type");
    size_t esz = elemSize();
    size_t minstep = dims <= 1 ? esz : size.p[1] * (dims == 2 ? esz : step.p[1]);
    size_t step0 = step.p[0];
    ଯଦି (step0 < minstep) {
        ଯଦି (size.p[0] <= 1)
            step.p[0] = step0 = minstep;
    }

    ଯଦି( isSubmatrix() || dataend + step0*delta > datalimit )
        reserve( std::max(r + delta, (r*3+1)/2) );

    size.p[0] += ପୂର୍ଣ୍ଣସଂଖ୍ୟା(delta);
    ଯଦି (dims == 2)
        rows = size.p[0];
    ଅନ୍ୟଥା ଯଦି (dims == 1)
        cols = size.p[0];
    dataend += step0*delta;

    //updateContinuityFlag(*this);

    ଯଦି( isContinuous() && elems.isContinuous() )
        memcpy(data + r*step0, elems.data, elems.total()*elems.elemSize());
    ଅନ୍ୟଥା
    {
        Mat part = rowRange(ପୂର୍ଣ୍ଣସଂଖ୍ୟା(r), ପୂର୍ଣ୍ଣସଂଖ୍ୟା(r + delta));
        elems.copyTo(part);
    }
}


ଶୂନ୍ୟ Mat::locateROI( Size& wholeSize, Point& ofs ) ଅଚଳ
{
    CV_Assert( dims <= 2 && step[0] > 0 );
    size_t esz = elemSize(), minstep;
    ptrdiff_t delta1 = data - datastart, delta2 = dataend - datastart;

    ଯଦି( delta1 == 0 )
        ofs.x = ofs.y = 0;
    ଅନ୍ୟଥା
    {
        ofs.y = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(delta1/step[0]);
        ofs.x = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)((delta1 - step[0]*ofs.y)/esz);
        CV_DbgAssert( data == datastart + ofs.y*step[0] + ofs.x*esz );
    }
    minstep = (ofs.x + cols)*esz;
    wholeSize.height = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)((delta2 - minstep)/step[0] + 1);
    wholeSize.height = std::max(wholeSize.height, ofs.y + rows);
    wholeSize.width = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)((delta2 - step*(wholeSize.height-1))/esz);
    wholeSize.width = std::max(wholeSize.width, ofs.x + cols);
}

Mat& Mat::adjustROI( ପୂର୍ଣ୍ଣସଂଖ୍ୟା dtop, ପୂର୍ଣ୍ଣସଂଖ୍ୟା dbottom, ପୂର୍ଣ୍ଣସଂଖ୍ୟା dleft, ପୂର୍ଣ୍ଣସଂଖ୍ୟା dright )
{
    CV_Assert( dims <= 2 && step[0] > 0 );
    Size wholeSize; Point ofs;
    size_t esz = elemSize();
    locateROI( wholeSize, ofs );
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା row1 = std::min(std::max(ofs.y - dtop, 0), wholeSize.height), row2 = std::max(0, std::min(ofs.y + rows + dbottom, wholeSize.height));
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା col1 = std::min(std::max(ofs.x - dleft, 0), wholeSize.width), col2 = std::max(0, std::min(ofs.x + cols + dright, wholeSize.width));
    ଯଦି(row1 > row2)
        std::swap(row1, row2);
    ଯଦି(col1 > col2)
        std::swap(col1, col2);

    ଯଦି (dims == 1) {
        data += (col1 - ofs.x)*(std::ptrdiff_t)esz;
        cols = col2 - col1;
        size.p[0] = cols;
    } ଅନ୍ୟଥା {
        data += (row1 - ofs.y)*(std::ptrdiff_t)step + (col1 - ofs.x)*(std::ptrdiff_t)esz;
        rows = row2 - row1; cols = col2 - col1;
        size.p[0] = rows; size.p[1] = cols;
        updateContinuityFlag();
    }
    ଫେରନ୍ତୁ *ଏହି;
}

Mat Mat::reshape(ପୂର୍ଣ୍ଣସଂଖ୍ୟା new_cn, ପୂର୍ଣ୍ଣସଂଖ୍ୟା new_rows) ଅଚଳ
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn = channels();
    Mat hdr = *ଏହି;

    ଯଦି( new_cn == 0 )
        new_cn = cn;

    ଯଦି( dims > 2 )
    {
        ଯଦି( new_rows == 0 )
        {
            // special case: just change the number of channnels; retain the same shape,
            // except for the last, innermost dimension
            CV_Assert(size[dims-1]*cn % new_cn == 0);
            hdr.flags = (hdr.flags & ~CV_MAT_CN_MASK) | ((new_cn-1) << CV_CN_SHIFT);
            hdr.step[dims-1] = CV_ELEM_SIZE(hdr.flags);
            hdr.size[dims-1] = hdr.size[dims-1]*cn / new_cn;
            ଫେରନ୍ତୁ hdr;
        }
        CV_Assert( new_rows > 0 );
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା sz[] = { new_rows, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(total()*cn/new_rows) };
        ଫେରନ୍ତୁ reshape(new_cn, 2, sz);
    }

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା total_width = cols * cn;

    ଯଦି( (new_cn > total_width || total_width % new_cn != 0) && new_rows == 0 )
        new_rows = rows * total_width / new_cn;

    ଯଦି( new_rows != 0 && new_rows != rows )
    {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା total_size = total_width * rows;
        ଯଦି( !isContinuous() )
            CV_Error( cv::Error::BadStep,
            "The matrix is not continuous, thus its number of rows can not be changed" );

        total_width = total_size / new_rows;

        ଯଦି( total_width * new_rows != total_size )
            CV_Error( cv::Error::StsBadArg, "The total number of matrix elements "
                                    "is not divisible by the new number of rows" );

        hdr.size[0] = hdr.rows = new_rows;
        hdr.step[0] = total_width * elemSize1();
    } ଅନ୍ୟଥା {
        hdr.size[0] = hdr.rows = rows;
        ଯଦି (dims <= 1)
            hdr.step[0] = cols * CV_ELEM_SIZE(flags);
    }

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା new_width = total_width / new_cn;

    ଯଦି( new_width * new_cn != total_width )
        CV_Error( cv::Error::Bad_ଅଙ୍କ_Channels,
        "The total width is not divisible by the new number of channels" );

    hdr.size.dims = hdr.dims = 2;
    hdr.size[1] = hdr.cols = new_width;
    hdr.flags = (hdr.flags & ~CV_MAT_CN_MASK) | ((new_cn-1) << CV_CN_SHIFT);
    hdr.step[1] = CV_ELEM_SIZE(hdr.flags);
    ଫେରନ୍ତୁ hdr;
}

Mat Mat::reshape(ପୂର୍ଣ୍ଣସଂଖ୍ୟା _cn, ପୂର୍ଣ୍ଣସଂଖ୍ୟା _newndims, ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା* _newsz) ଅଚଳ
{
    ଯଦି(_newndims == dims)
    {
        ଯଦି(_newsz == 0)
            ଫେରନ୍ତୁ reshape(_cn);
        ଯଦି(_newndims == 2)
            ଫେରନ୍ତୁ reshape(_cn, _newsz[0]);
    }

    ଯଦି (isContinuous())
    {
        CV_Assert(_cn >= 0 && _newndims >= 0 && _newndims <= CV_MAX_DIM && (_newndims == 0 || _newsz != 0));

        ଯଦି (_cn == 0)
            _cn = ଏହି->channels();
        ଅନ୍ୟଥା
            CV_Assert(_cn <= CV_CN_MAX);

        size_t total_elem1_ref = ଏହି->total() * ଏହି->channels();
        size_t total_elem1 = _cn;

        AutoBuffer<ପୂର୍ଣ୍ଣସଂଖ୍ୟା, 4> newsz_buf( (size_t)_newndims );

        ପୂର୍ଣ୍ଣସଂଖ୍ୟା m1_idx = -1;

        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < _newndims; i++)
        {
            ଯଦି (_newsz[i] >= 0) {
                ଯଦି (_newsz[i] == 0 && i < dims)
                    newsz_buf[i] = size.p[i];
                ଅନ୍ୟଥା
                    newsz_buf[i] = _newsz[i];
                total_elem1 *= (size_t)newsz_buf[i];
            } ଅନ୍ୟଥା {
                ଯଦି (m1_idx >= 0)
                    CV_Error(cv::Error::StsBadSize, "More than one '-1' occured in the new shape");
                m1_idx = i;
            }
        }

        ଯଦି (m1_idx >= 0) {
            ଯଦି (total_elem1 == 0) {
                CV_Assert(total_elem1_ref == 0);
                total_elem1 = 1;
            }
            CV_Assert(total_elem1_ref % total_elem1 == 0);
            newsz_buf[m1_idx] = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(total_elem1_ref / total_elem1);
        } ଅନ୍ୟଥା ଯଦି (total_elem1 != total_elem1_ref) {
            CV_Error(cv::Error::StsUnmatchedSizes, "Requested and source matrices have different count of elements");
        }

        Mat hdr = *ଏହି;
        hdr.flags = (hdr.flags & ~CV_MAT_CN_MASK) | ((_cn-1) << CV_CN_SHIFT);
        setSize(hdr, _newndims, newsz_buf.data(), ଖାଲି, ସତ୍ୟ);

        ଫେରନ୍ତୁ hdr;
    }

    CV_Error(cv::Error::StsNotImplemented, "Reshaping of n-dimensional non-continuous matrices is not supported yet");
    // TBD
}

Mat Mat::reshape(ପୂର୍ଣ୍ଣସଂଖ୍ୟା _cn, ଅଚଳ std::vector<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>& _newshape) ଅଚଳ
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା newdims = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)_newshape.size();
    ଯଦି(newdims == 0 && empty())
    {
        ଫେରନ୍ତୁ *ଏହି;
    }

    ଫେରନ୍ତୁ reshape(_cn, newdims, newdims > 0 ? &_newshape[0] : 0);
}

Mat Mat::reshape(ପୂର୍ଣ୍ଣସଂଖ୍ୟା _cn, ଅଚଳ MatShape& _newshape) ଅଚଳ
{
    ଯଦି (_newshape.dims < 0) {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା newshape[] = {0};
        ଫେରନ୍ତୁ reshape(_cn, 1, newshape);
    }
    ଫେରନ୍ତୁ reshape(_cn, _newshape.dims, _newshape.p);
}

Mat Mat::reshape(ପୂର୍ଣ୍ଣସଂଖ୍ୟା _cn, std::initializer_list<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> newshape_) ଅଚଳ
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା newshape[MatShape::MAX_DIMS];
    size_t i, newshape_dims = newshape_.size();
    CV_Assert(newshape_dims <= (size_t)MatShape::MAX_DIMS);
    auto it = newshape_.begin();
    ପାଇଁ (i = 0; i < newshape_dims; i++, ++it)
        newshape[i] = *it;
    ଫେରନ୍ତୁ reshape(_cn, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)newshape_dims, newshape);
}

Mat Mat::reinterpret(ପୂର୍ଣ୍ଣସଂଖ୍ୟା type) ଅଚଳ
{
    type = CV_MAT_TYPE(type);
    CV_Assert(CV_ELEM_SIZE(ଏହି->type()) == CV_ELEM_SIZE(type));
    Mat m = *ଏହି;
    m.flags = (m.flags & ~CV_MAT_TYPE_MASK) | type;
    m.updateContinuityFlag();
    ଫେରନ୍ତୁ m;
}

Mat Mat::diag(ଅଚଳ Mat& d)
{
    CV_Assert( d.cols == 1 || d.rows == 1 );
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା len = d.rows + d.cols - 1;
    Mat m(len, len, d.type(), Scalar(0));
    Mat md = m.diag();
    ଯଦି( d.cols == 1 )
        d.copyTo(md);
    ଅନ୍ୟଥା
        transpose(d, md);
    ଫେରନ୍ତୁ m;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା Mat::checkVector(ପୂର୍ଣ୍ଣସଂଖ୍ୟା _elemChannels, ପୂର୍ଣ୍ଣସଂଖ୍ୟା _depth, ତାର୍କିକ _requireContinuous) ଅଚଳ
{
    ଫେରନ୍ତୁ data && (depth() == _depth || _depth <= 0) &&
        (isContinuous() || !_requireContinuous) &&
        ((dims <= 2 && (((rows == 1 || cols == 1) && channels() == _elemChannels) ||
                        (cols == _elemChannels && channels() == 1))) ||
        (dims == 3 && channels() == 1 && size.p[2] == _elemChannels && (size.p[0] == 1 || size.p[1] == 1) &&
         (isContinuous() || step.p[1] == step.p[2]*size.p[2])))
    ? (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(total()*channels()/_elemChannels) : -1;
}


ସ୍ଥିର ଧାଡ଼ିଭିତରେ Size getContinuousSize_(ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cols, ପୂର୍ଣ୍ଣସଂଖ୍ୟା rows, ପୂର୍ଣ୍ଣସଂଖ୍ୟା widthScale)
{
    int64 sz = (int64)cols * rows * widthScale;
    ତାର୍କିକ has_int_overflow = sz >= INT_MAX;
    ତାର୍କିକ isContiguous = (flags & Mat::CONTINUOUS_FLAG) != 0;
    ଫେରନ୍ତୁ (isContiguous && !has_int_overflow)
            ? Size((ପୂର୍ଣ୍ଣସଂଖ୍ୟା)sz, 1)
            : Size(cols * widthScale, rows);
}

Size getContinuousSize2D(Mat& m1, ପୂର୍ଣ୍ଣସଂଖ୍ୟା widthScale)
{
    CV_CheckLE(m1.dims, 2, "");
    ଫେରନ୍ତୁ getContinuousSize_(m1.flags,
                              m1.cols, m1.rows, widthScale);
}
Size getContinuousSize2D(Mat& m1, Mat& m2, ପୂର୍ଣ୍ଣସଂଖ୍ୟା widthScale)
{
    CV_CheckLE(m1.dims, 2, "");
    CV_CheckLE(m2.dims, 2, "");
    ଅଚଳ Size sz1 = m1.size();
    ଯଦି (sz1 != m2.size())  // reshape all matrixes to the same size (#4159)
    {
        size_t total_sz = m1.total();
        CV_CheckEQ(total_sz, m2.total(), "");
        ତାର୍କିକ is_m1_vector = m1.cols == 1 || m1.rows == 1;
        ତାର୍କିକ is_m2_vector = m2.cols == 1 || m2.rows == 1;
        CV_Assert(is_m1_vector); CV_Assert(is_m2_vector);
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା total = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)total_sz;  // vector-column
        ତାର୍କିକ isContiguous = ((m1.flags & m2.flags) & Mat::CONTINUOUS_FLAG) != 0;
        ତାର୍କିକ has_int_overflow = ((int64)total_sz * widthScale) >= INT_MAX;
        ଯଦି (isContiguous && !has_int_overflow)
            total = 1; // vector-row
        m1 = m1.reshape(0, total);
        m2 = m2.reshape(0, total);
        CV_Assert(m1.cols == m2.cols && m1.rows == m2.rows);
        ଫେରନ୍ତୁ Size(m1.cols * widthScale, m1.rows);
    }
    ଫେରନ୍ତୁ getContinuousSize_(m1.flags & m2.flags,
                              m1.cols, m1.rows, widthScale);
}

Size getContinuousSize2D(Mat& m1, Mat& m2, Mat& m3, ପୂର୍ଣ୍ଣସଂଖ୍ୟା widthScale)
{
    CV_CheckLE(m1.dims, 2, "");
    CV_CheckLE(m2.dims, 2, "");
    CV_CheckLE(m3.dims, 2, "");
    ଅଚଳ Size sz1 = m1.size();
    ଯଦି (sz1 != m2.size() || sz1 != m3.size())  // reshape all matrixes to the same size (#4159)
    {
        size_t total_sz = m1.total();
        CV_CheckEQ(total_sz, m2.total(), "");
        CV_CheckEQ(total_sz, m3.total(), "");
        ତାର୍କିକ is_m1_vector = m1.cols == 1 || m1.rows == 1;
        ତାର୍କିକ is_m2_vector = m2.cols == 1 || m2.rows == 1;
        ତାର୍କିକ is_m3_vector = m3.cols == 1 || m3.rows == 1;
        CV_Assert(is_m1_vector); CV_Assert(is_m2_vector); CV_Assert(is_m3_vector);
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା total = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)total_sz;  // vector-column
        ତାର୍କିକ isContiguous = ((m1.flags & m2.flags & m3.flags) & Mat::CONTINUOUS_FLAG) != 0;
        ତାର୍କିକ has_int_overflow = ((int64)total_sz * widthScale) >= INT_MAX;
        ଯଦି (isContiguous && !has_int_overflow)
            total = 1; // vector-row
        m1 = m1.reshape(0, total);
        m2 = m2.reshape(0, total);
        m3 = m3.reshape(0, total);
        CV_Assert(m1.cols == m2.cols && m1.rows == m2.rows && m1.cols == m3.cols && m1.rows == m3.rows);
        ଫେରନ୍ତୁ Size(m1.cols * widthScale, m1.rows);
    }
    ଫେରନ୍ତୁ getContinuousSize_(m1.flags & m2.flags & m3.flags,
                              m1.cols, m1.rows, widthScale);
}

} // cv::
