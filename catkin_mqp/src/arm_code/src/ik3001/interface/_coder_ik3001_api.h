//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// _coder_ik3001_api.h
//
// Code generation for function 'ik3001'
//

#ifndef _CODER_IK3001_API_H
#define _CODER_IK3001_API_H

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
void ik3001(real_T endPos[3], real_T J[3]);

void ik3001_api(const mxArray *prhs, const mxArray **plhs);

void ik3001_atexit();

void ik3001_initialize();

void ik3001_terminate();

void ik3001_xil_shutdown();

void ik3001_xil_terminate();

#endif
// End of code generation (_coder_ik3001_api.h)
