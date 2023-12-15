//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// ik3001.cpp
//
// Code generation for function 'ik3001'
//

// Include files
//#include "ik3001.h"
#include "rt_nonfinite.h"
#include "rt_defines.h"
#include <cmath>
#include <vector>
#include <complex>
#include <valarray>

typedef std::complex<float> Complex;
typedef std::valarray<Complex> CArray;

// Function Declarations
static Complex rt_atan2d_snf(Complex u0, Complex u1);

// Function Definitions
static Complex rt_atan2d_snf(Complex u0, Complex u1)
{
  Complex y;
  if (std::isnan(u0) || std::isnan(u1)) {
    y = rtNaN;
  } else if (std::isinf(u0) && std::isinf(u1)) {
    int i;
    int i1;
    if (u0 > 0.0) {
      i = 1;
    } else {
      i = -1;
    }
    if (u1 > 0.0) {
      i1 = 1;
    } else {
      i1 = -1;
    }
    y = std::atan2(static_cast<Complex>(i), static_cast<Complex>(i1));
  } else if (u1 == 0.0) {
    if (u0 > 0.0) {
      y = RT_PI / 2.0;
    } else if (u0 < 0.0) {
      y = -(RT_PI / 2.0);
    } else {
      y = 0.0;
    }
  } else {
    y = std::atan2(u0, u1);
  }
  return y;
}

Complex * ik3001(std::vector<Complex> endPos)
{
  Complex cbeta3;
  Complex r;
  Complex sbeta3;
  Complex * J;
  // end pos is array of x, y, z position of end effector
  // try
  // link lengths
  // changed
  // theta1
  // theta3
  r = std::sqrt(endPos[0] * endPos[0] + endPos[1] * endPos[1]);
  cbeta3 =
      -(20000.0 - (r * r + (endPos[2] - 40.0) * (endPos[2] - 40.0))) / 20000.0;
  sbeta3 = std::sqrt(1.0 - cbeta3 * cbeta3);
  // theta2
  J[0] = 57.295779513082323 * rt_atan2d_snf(endPos[1], endPos[0]);
  J[1] = (90.0 - 57.295779513082323 * rt_atan2d_snf(endPos[2] - 40.0, r)) -
         57.295779513082323 *
             rt_atan2d_snf(100.0 * sbeta3, 100.0 * cbeta3 + 100.0);
  J[2] = 57.295779513082323 * rt_atan2d_snf(sbeta3, cbeta3) - 90.0;
  // returns jacobian
  return J;
  // catches exception for invalid joint positions
  // catch exception
  //     getReport(exception);
  //     error("out of workspace")
  // end
}

// End of code generation (ik3001.cpp)
