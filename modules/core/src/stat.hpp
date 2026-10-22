// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ SRC_STAT_HPP
#ವ್ಯಾಖ್ಯಾನಿಸು SRC_STAT_HPP

#ಅನ್ತರ್ಭಾವ "opencv2/core/mat.hpp"

ನಾಮಾಕಾಶ cv {

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_OPENCL

ಎಣಿಕೆ { OCL_OP_ಮೊತ್ತ = 0, OCL_OP_ಮೊತ್ತ_ABS =  1, OCL_OP_ಮೊತ್ತ_SQR = 2 };
ತಾರ್ಕಿಕ ocl_ಮೊತ್ತ( ಒಳಹರಿವು_Array _src, Scalar & res, ಪೂರ್ಣಾಂಕ ಮೊತ್ತ_op, ಒಳಹರಿವು_Array ಮುಖವಾಡ_2 = noArray(),
                     ಒಳಹರಿವು_Array _src2 = noArray(), ತಾರ್ಕಿಕ calc2 = ಸುಳ್ಳು, ಸ್ಥಿರಾಂಕ Scalar & res2 = Scalar() );
ತಾರ್ಕಿಕ ocl_minMaxIdx( ಒಳಹರಿವು_Array _src, ದ್ವಿಗುಣ* minVal, ದ್ವಿಗುಣ* maxVal, ಪೂರ್ಣಾಂಕ* minLoc, ಪೂರ್ಣಾಂಕ* maxLoc, ಒಳಹರಿವು_Array ಮುಖವಾಡ_2,
                           ಪೂರ್ಣಾಂಕ ddepth = -1, ತಾರ್ಕಿಕ abs_ಮೌಲ್ಯಗಳು = ಸುಳ್ಳು, ಒಳಹರಿವು_Array _src2 = noArray(), ದ್ವಿಗುಣ * maxVal2 = ಖಾಲಿ);

ಮಾದರಿ <ಬಗೆ_ಹೆಸರು T> Scalar ocl_part_ಮೊತ್ತ(Mat m)
{
    CV_Assert(m.ಅಡ್ಡಸಾಲುಗಳು == 1);

    Scalar s = Scalar::all(0);
    ಪೂರ್ಣಾಂಕ cn = m.channels();
    ಸ್ಥಿರಾಂಕ T * ಸ್ಥಿರಾಂಕ ptr = m.ptr<T>(0);

    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ x = 0, w = m.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ * cn; x < w; )
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ c = 0; c < cn; ++c, ++x)
            s[c] += ptr[x];

    ಹಿಂತಿರುಗಿ s;
}

#ಆದರೆ_ಅಂತ್ಯ

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಪೂರ್ಣಾಂಕ (*ಮೊತ್ತ_Func)(ಸ್ಥಿರಾಂಕ uchar*, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ, uchar*, ಪೂರ್ಣಾಂಕ, ಪೂರ್ಣಾಂಕ);
ಮೊತ್ತ_Func get_ಮೊತ್ತ_Func(ಪೂರ್ಣಾಂಕ ಆಳ);

}

#ಆದರೆ_ಅಂತ್ಯ // SRC_STAT_HPP
