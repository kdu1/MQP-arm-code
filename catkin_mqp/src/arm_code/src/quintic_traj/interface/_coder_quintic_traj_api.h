//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// _coder_quintic_traj_api.h
//
// Code generation for function 'quintic_traj'
//

#ifndef _CODER_QUINTIC_TRAJ_API_H
#define _CODER_QUINTIC_TRAJ_API_H

// Include files
#include "emlrt.h"
#include "mex.h"
#include "tmwtypes.h"
#include <algorithm>
#include <cstring>

// Variable Declarations
extern emlrtCTX emlrtRootTLSGlobal;
extern emlrtContext emlrtContextGlobal;

// Function Declarations
void quintic_traj(real_T tf, real_T vi[3], real_T vf[3], real_T ai[3],
                  real_T af[3], real_T pi[3], real_T pf[3], real_T QT[18]);

void quintic_traj_api(const mxArray *const prhs[7], const mxArray **plhs);

void quintic_traj_atexit();

void quintic_traj_initialize();

void quintic_traj_terminate();

void quintic_traj_xil_shutdown();

void quintic_traj_xil_terminate();

#endif
// End of code generation (_coder_quintic_traj_api.h)
