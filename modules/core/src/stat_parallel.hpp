// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
// Copyright (C) 2026, BigVision LLC, all rights reserved.
// Third party copyrights are property of their respective owners.

// Splitting of an array into pieces for the parallel versions of sum, mean, meanStdDev and minMaxIdx.
// Each piece is reduced by the existing serial code (including the HAL), and the results are
// combined in piece order.

#यदि_अपरिभाषितम् OPENCV_CORE_STAT_PARALLEL_HPP
#परिभाषय OPENCV_CORE_STAT_PARALLEL_HPP

नामाकाश cv {

संरचना StatChunk
{
    Mat src, आवरणम्;
    size_t firstElem;   // row-major index of the first element of the piece in the whole array
};

// Returns false when the array is too small to be worth splitting, or cannot be split.
स्थिर अन्तःस्थ तार्किक विभज_For_Parallel_Stat(ध्रुव Mat& src, ध्रुव Mat& आवरणम्, std::vector<StatChunk>& chunks)
{
    ध्रुव size_t योग = src.योग(), work = योग*src.channels();
    यदि (work < ((size_t)1 << 18))
        प्रत्यागम असत्य;
    ध्रुव पूर्णाङ्क nthreads = get_अङ्कः_तन्तवः();
    यदि (nthreads <= 1)
        प्रत्यागम असत्य;
    ध्रुव पूर्णाङ्क n = (पूर्णाङ्क)std::min((size_t)nthreads*2, work >> 16);
    यदि (n < 2)
        प्रत्यागम असत्य;

    chunks.clear();
    यदि (src.dims <= 2 && src.पङ्क्तिकाः >= n)
    {
        प्रति (पूर्णाङ्क i = 0; i < n; i++)
        {
            पूर्णाङ्क r0 = (पूर्णाङ्क)((int64)src.पङ्क्तिकाः*i/n), r1 = (पूर्णाङ्क)((int64)src.पङ्क्तिकाः*(i + 1)/n);
            StatChunk c;
            c.src = src.rowRange(r0, r1);
            यदि (!आवरणम्.empty())
                c.आवरणम् = आवरणम्.rowRange(r0, r1);
            c.firstElem = (size_t)r0*src.स्तम्भसङ्ख्या;
            chunks.push_back(c);
        }
        प्रत्यागम सत्य;
    }
    यदि (src.isContinuous() && (आवरणम्.empty() || आवरणम्.isContinuous()) && योग <= (size_t)अधिकतम_पूर्णाङ्कः)
    {
        प्रति (पूर्णाङ्क i = 0; i < n; i++)
        {
            size_t e0 = योग*i/n, e1 = योग*(i + 1)/n;
            StatChunk c;
            c.src = Mat(1, (पूर्णाङ्क)(e1 - e0), src.type(), (शून्य*)(src.दत्त + e0*src.elem_आकार()));
            यदि (!आवरणम्.empty())
                c.आवरणम् = Mat(1, (पूर्णाङ्क)(e1 - e0), आवरणम्.type(), (शून्य*)(आवरणम्.दत्त + e0*आवरणम्.elem_आकार()));
            c.firstElem = e0;
            chunks.push_back(c);
        }
        प्रत्यागम सत्य;
    }
    प्रत्यागम असत्य;
}

// row-major index of an element given by the per-dimension indices of a 2D piece
स्थिर अन्तःस्थ size_t chunk_सूचकांक_गन्तव्यम्_Elem(ध्रुव StatChunk& c, ध्रुव पूर्णाङ्क* idx)
{
    प्रत्यागम c.firstElem + (size_t)idx[0]*c.src.स्तम्भसङ्ख्या + idx[1];
}

} // namespace cv

#यदि_अन्तः
