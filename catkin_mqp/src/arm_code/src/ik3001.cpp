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
#include "ik3001.h"
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
  ROS_INFO("rt_atan2d_snf");
  Complex y;
  
  //if NaN
  if (std::isnan(u0.real()) || std::isnan(u1.real())) {
    y = rtNaN;
  } 
  else if (std::isinf(u0.real()) && std::isinf(u1.real())) {
      ROS_INFO("both inf");
      int i;
      int i1;
      if (u0.real() > 0.0) {
        i = 1;
      } 
      else {
        i = -1;
      }
      if (u1.real() > 0.0) {
        i1 = 1;
      } 
      else {
        i1 = -1;
      }
      y = Complex(std::atan2(i, i1));
  } 
  else if (u1.real() == 0.0) {
      ROS_INFO("u1 is 0");
      if (u0.real() > 0.0) {
        y = RT_PI / 2.0;
      } else if (u0.real() < 0.0) {
        y = -(RT_PI / 2.0);
      } else {
        y = 0.0;
      }
  } 
  else {
      ROS_INFO("both noninf");
      y = std::atan2(u0.real(), u1.real());
  }
  ROS_INFO("rt_atan2d_snf return");
  return y;
}


/**
 * calculates inverse kinematics
*/
CArray ik3001(const Complex endPos[3])
{
  ROS_INFO("ik3001");
  Complex cbeta3;
  Complex r;
  Complex sbeta3;
  CArray J(3);
  // end pos is array of x, y, z position of end effector
  // try
  // link lengths
  // changed
  // theta1
  // theta3
  r = std::sqrt(endPos[0] * endPos[0] + endPos[1] * endPos[1]);
  cbeta3 = -(Complex(20000.0) - (r * r + (endPos[2] - Complex(40.0)) * (endPos[2] - Complex(40.0)))) / Complex(20000.0);
  sbeta3 = std::sqrt(Complex(1.0) - cbeta3 * cbeta3);

  ROS_INFO("before theta2");
  // theta2
  
  Complex j0val = rt_atan2d_snf(endPos[1], endPos[0]);
  ROS_INFO("after j0val");
  J[0] = std::complex<float>(57.295779513082323) * j0val;
  ROS_INFO("J[0]: %f", J[0].real());
  ROS_INFO("before J[1]");
  J[1] = (Complex(90.0) - Complex(57.295779513082323) * rt_atan2d_snf(endPos[2] - Complex(40.0), r)) -
         Complex(57.295779513082323) * rt_atan2d_snf(Complex(100.0) * sbeta3, Complex(100.0) * cbeta3 + Complex(100.0));
  J[2] = Complex(57.295779513082323) * rt_atan2d_snf(sbeta3, cbeta3) - Complex(90.0);
  // returns jacobian
  return J;
  // catches exception for invalid joint positions
  // catch exception
  //     getReport(exception);
  //     error("out of workspace")
  // end
}

// End of code generation (ik3001.cpp)
