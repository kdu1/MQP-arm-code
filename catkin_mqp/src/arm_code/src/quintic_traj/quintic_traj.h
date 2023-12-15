//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// quintic_traj.h
//
// Code generation for function 'quintic_traj'
//

#ifndef QUINTIC_TRAJ_H
#define QUINTIC_TRAJ_H

// Include files
#include "rtwtypes.h"
#include <cstddef>
#include <cstdlib>

// Function Declarations
extern void quintic_traj(double tf, const double vi[3], const double vf[3],
                         const double ai[3], const double af[3],
                         const double pi[3], const double pf[3], double QT[18]);

#endif
// End of code generation (quintic_traj.h)
