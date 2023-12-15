//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// rt_nonfinite.cpp
//
// Code generation for function 'cubic_traj'
//

// Abstract:
//      MATLAB for code generation function to initialize non-finites,
//      (Inf, NaN and -Inf).
// Include files
#include "rt_nonfinite.h"
#include <cmath>
#include <limits>

double rtNaN{std::numeric_limits<double>::quiet_NaN()};
double rtInf{std::numeric_limits<double>::infinity()};
double rtMinusInf{-std::numeric_limits<double>::infinity()};
double rtNaNF{std::numeric_limits<double>::quiet_NaN()};
double rtInfF{std::numeric_limits<double>::infinity()};
double rtMinusInfF{-std::numeric_limits<double>::infinity()};

// End of code generation (rt_nonfinite.cpp)
