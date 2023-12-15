//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// ik3001.h
//
// Code generation for function 'ik3001'
//

#ifndef IK3001_H
#define IK3001_H

// Include files
//#include "rtwtypes.h"
#include <cstddef>
#include <cstdlib>
#include <complex>
#include <valarray>

typedef std::complex<float> Complex;
typedef std::valarray<Complex> CArray;
// Function Declarations
std::vector<Complex> ik3001(const Complex endPos[3]);

#endif
// End of code generation (ik3001.h)
