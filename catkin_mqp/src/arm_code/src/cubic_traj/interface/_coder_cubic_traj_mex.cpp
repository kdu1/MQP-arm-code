//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// _coder_cubic_traj_mex.cpp
//
// Code generation for function 'cubic_traj'
//

/*
// Include files
#include "_coder_cubic_traj_mex.h"
#include "_coder_cubic_traj_api.h"

// Function Definitions
void mexFunction(int32_T nlhs, mxArray *plhs[], int32_T nrhs,
                 const mxArray *prhs[])
{
  mexAtExit(&cubic_traj_atexit);
  // Module initialization.
  cubic_traj_initialize();
  // Dispatch the entry-point.
  unsafe_cubic_traj_mexFunction(nlhs, plhs, nrhs, prhs);
  // Module termination.
  cubic_traj_terminate();
}

emlrtCTX mexFunctionCreateRootTLS()
{
  emlrtCreateRootTLSR2022a(&emlrtRootTLSGlobal, &emlrtContextGlobal, nullptr, 1,
                           nullptr, "windows-1252", true);
  return emlrtRootTLSGlobal;
}

void unsafe_cubic_traj_mexFunction(int32_T nlhs, mxArray *plhs[1], int32_T nrhs,
                                   const mxArray *prhs[5])
{
  emlrtStack st{
      nullptr, // site
      nullptr, // tls
      nullptr  // prev
  };
  const mxArray *b_prhs[5];
  const mxArray *outputs;
  st.tls = emlrtRootTLSGlobal;
  // Check for proper number of arguments.
  if (nrhs != 5) {
    emlrtErrMsgIdAndTxt(&st, "EMLRT:runTime:WrongNumberOfInputs", 5, 12, 5, 4,
                        10, "cubic_traj");
  }
  if (nlhs > 1) {
    emlrtErrMsgIdAndTxt(&st, "EMLRT:runTime:TooManyOutputArguments", 3, 4, 10,
                        "cubic_traj");
  }
  // Call the function.
  for (int32_T i{0}; i < 5; i++) {
    b_prhs[i] = prhs[i];
  }
  cubic_traj_api(b_prhs, &outputs);
  // Copy over outputs to the caller.
  emlrtReturnArrays(1, &plhs[0], &outputs);
}
*/
// End of code generation (_coder_cubic_traj_mex.cpp)
