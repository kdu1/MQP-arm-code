//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// cubic_traj.cpp
//
// Code generation for function 'cubic_traj'
//

// Include files
#include "cubic_traj.h"
#include "rt_nonfinite.h"
#include <cmath>
#include <complex>
#include <valarray>

//typedef std::complex<float> Complex;
//typedef std::valarray<Complex> CArray;



// Function Declarations
//static double rt_powd_snf(double u0, double u1);

// Function Definitions
static double rt_powd_snf(double u0, double u1)
{
  double y;
  if (std::isnan(u0) || std::isnan(u1)) {
    y = rtNaN;
  } else {
    double d;
    double d1;
    d = std::abs(u0);
    d1 = std::abs(u1);
    if (std::isinf(u1)) {
      if (d == 1.0) {
        y = 1.0;
      } else if (d > 1.0) {
        if (u1 > 0.0) {
          y = rtInf;
        } else {
          y = 0.0;
        }
      } else if (u1 > 0.0) {
        y = 0.0;
      } else {
        y = rtInf;
      }
    } else if (d1 == 0.0) {
      y = 1.0;
    } else if (d1 == 1.0) {
      if (u1 > 0.0) {
        y = u0;
      } else {
        y = 1.0 / u0;
      }
    } else if (u1 == 2.0) {
      y = u0 * u0;
    } else if ((u1 == 0.5) && (u0 >= 0.0)) {
      y = std::sqrt(u0);
    } else if ((u0 < 0.0) && (u1 > std::floor(u1))) {
      y = rtNaN;
    } else {
      y = std::pow(u0, u1);
    }
  }
  return y;
}

/**
 * TODO: 
 * output array should be 4X1
 *
*/
CArray cubic_traj(double tf, std::vector<double> vi, std::vector<double> vf,
                                        std::vector<double> pi, std::vector<double> pf)
{
  CArray CTcomplex;
  std::vector<double> CT;
  Complex Acomplex[16];
  Complex smaxcomplex;
  double A[16];
  double smax;
  int i;
  int i1;
  int jA;
  int jBcol;
  int jp1j;
  signed char ipiv[4];
  A[0] = 1.0;
  A[1] = 0.0;
  A[4] = 0.0;
  A[5] = 1.0;
  A[8] = 0.0;
  A[9] = 0.0;
  A[12] = 0.0;
  A[13] = 0.0;
  A[2] = 1.0;
  A[6] = tf;
  smax = tf * tf;
  A[10] = smax;
  A[14] = rt_powd_snf(tf, 3.0);
  A[3] = 0.0;
  A[7] = 1.0;
  A[11] = 2.0 * tf;
  A[15] = 3.0 * smax;
  CT[0] = pi[0];
  CT[1] = vi[0];
  CT[2] = pf[0];
  CT[3] = vf[0];
  CT[4] = pi[1];
  CT[5] = vi[1];
  CT[6] = pf[1];
  CT[7] = vf[1];
  CT[8] = pi[2];
  CT[9] = vi[2];
  CT[10] = pf[2];
  CT[11] = vf[2];
  ipiv[0] = 1;
  ipiv[1] = 2;
  ipiv[2] = 3;
  ipiv[3] = 4;
  for (int j{0}; j < 3; j++) {
    int b_tmp;
    int mmj_tmp;
    signed char i2;
    mmj_tmp = 2 - j;
    b_tmp = j * 5;
    jp1j = b_tmp + 2;
    jA = 4 - j;
    jBcol = 0;
    smax = std::abs(A[b_tmp]);
    for (int k{2}; k <= jA; k++) {
      double s;
      s = std::abs(A[(b_tmp + k) - 1]);
      if (s > smax) {
        jBcol = k - 1;
        smax = s;
      }
    }
    if (A[b_tmp + jBcol] != 0.0) {
      if (jBcol != 0) {
        jA = j + jBcol;
        ipiv[j] = static_cast<signed char>(jA + 1);
        smax = A[j];
        A[j] = A[jA];
        A[jA] = smax;
        smax = A[j + 4];
        A[j + 4] = A[jA + 4];
        A[jA + 4] = smax;
        smax = A[j + 8];
        A[j + 8] = A[jA + 8];
        A[jA + 8] = smax;
        smax = A[j + 12];
        A[j + 12] = A[jA + 12];
        A[jA + 12] = smax;
      }
      i = (b_tmp - j) + 4;
      for (int b_i{jp1j}; b_i <= i; b_i++) {
        A[b_i - 1] /= A[b_tmp];
      }
    }
    jA = b_tmp;
    for (jBcol = 0; jBcol <= mmj_tmp; jBcol++) {
      smax = A[(b_tmp + (jBcol << 2)) + 4];
      if (smax != 0.0) {
        i = jA + 6;
        i1 = (jA - j) + 8;
        for (jp1j = i; jp1j <= i1; jp1j++) {
          A[jp1j - 1] += A[((b_tmp + jp1j) - jA) - 5] * -smax;
        }
      }
      jA += 4;
    }
    i2 = ipiv[j];
    if (i2 != j + 1) {
      smax = CT[j];
      CT[j] = CT[i2 - 1];
      CT[i2 - 1] = smax;
      smax = CT[j + 4];
      CT[j + 4] = CT[i2 + 3];
      CT[i2 + 3] = smax;
      smax = CT[j + 8];
      CT[j + 8] = CT[i2 + 7];
      CT[i2 + 7] = smax;
    }
  }
  for (int j{0}; j < 3; j++) {
    jBcol = j << 2;
    for (int k{0}; k < 4; k++) {
      jA = k << 2;
      i = k + jBcol;
      if (CT[i] != 0.0) {
        i1 = k + 2;
        for (int b_i{i1}; b_i < 5; b_i++) {
          jp1j = (b_i + jBcol) - 1;
          CT[jp1j] -= CT[i] * A[(b_i + jA) - 1];
        }
      }
    }
  }
  for (int j{0}; j < 3; j++) {
    jBcol = j << 2;
    for (int k{3}; k >= 0; k--) {
      jA = k << 2;
      i = k + jBcol;
      smax = CT[i];
      if (smax != 0.0) {
        CT[i] = smax / A[k + jA];
        for (int b_i{0}; b_i < k; b_i++) {
          jp1j = b_i + jBcol;
          CT[jp1j] -= CT[i] * A[b_i + jA];
        }
      }
    }
  }

  //convert to complex
  for(int i = 0; i < CT.size(); i++){
      CTcomplex[i] = Complex(CT[i], 0);
  }
  return CTcomplex;
}

// End of code generation (cubic_traj.cpp)
