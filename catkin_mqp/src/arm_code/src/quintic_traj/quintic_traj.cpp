//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// quintic_traj.cpp
//
// Code generation for function 'quintic_traj'
//

// Include files
#include "quintic_traj.h"
#include "rt_nonfinite.h"
#include <cmath>

// Function Declarations
static double rt_powd_snf(double u0, double u1);

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

void quintic_traj(double tf, const double vi[3], const double vf[3],
                  const double ai[3], const double af[3], const double pi[3],
                  const double pf[3], double QT[18])
{
  static const signed char iv[6]{1, 0, 0, 0, 0, 0};
  static const signed char iv1[6]{0, 1, 0, 0, 0, 0};
  static const signed char iv2[6]{0, 0, 2, 0, 0, 0};
  double A[36];
  double s;
  double smax;
  int i;
  int jA;
  int jBcol;
  int jp1j;
  int kAcol;
  signed char ipiv[6];
  // intakes a trajectory time, starting and ending velocities, starting and
  // ending accelerations, and starting and ending positions and returns a 6x1
  // array of trajectory coefficients for a quintic trajectory
  for (i = 0; i < 6; i++) {
    A[6 * i] = iv[i];
    A[6 * i + 1] = iv1[i];
    A[6 * i + 2] = iv2[i];
  }
  double A_tmp;
  A[3] = 1.0;
  A[9] = tf;
  smax = tf * tf;
  A[15] = smax;
  s = rt_powd_snf(tf, 3.0);
  A[21] = s;
  A_tmp = rt_powd_snf(tf, 4.0);
  A[27] = A_tmp;
  A[33] = rt_powd_snf(tf, 5.0);
  A[4] = 0.0;
  A[10] = 1.0;
  A[16] = 2.0 * tf;
  A[22] = 3.0 * smax;
  A[28] = 4.0 * s;
  A[34] = 5.0 * A_tmp;
  A[5] = 0.0;
  A[11] = 0.0;
  A[17] = 2.0;
  A[23] = 6.0 * tf;
  A[29] = 12.0 * smax;
  A[35] = 20.0 * s;
  QT[0] = pi[0];
  QT[1] = vi[0];
  QT[2] = ai[0];
  QT[3] = pf[0];
  QT[4] = vf[0];
  QT[5] = af[0];
  QT[6] = pi[1];
  QT[7] = vi[1];
  QT[8] = ai[1];
  QT[9] = pf[1];
  QT[10] = vf[1];
  QT[11] = af[1];
  QT[12] = pi[2];
  QT[13] = vi[2];
  QT[14] = ai[2];
  QT[15] = pf[2];
  QT[16] = vf[2];
  QT[17] = af[2];
  for (i = 0; i < 6; i++) {
    ipiv[i] = static_cast<signed char>(i + 1);
  }
  for (int j{0}; j < 5; j++) {
    int b_tmp;
    int mmj_tmp;
    signed char i1;
    mmj_tmp = 4 - j;
    b_tmp = j * 7;
    jp1j = b_tmp + 2;
    jA = 6 - j;
    jBcol = 0;
    smax = std::abs(A[b_tmp]);
    for (int k{2}; k <= jA; k++) {
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
        for (int k{0}; k < 6; k++) {
          jBcol = j + k * 6;
          smax = A[jBcol];
          kAcol = jA + k * 6;
          A[jBcol] = A[kAcol];
          A[kAcol] = smax;
        }
      }
      i = (b_tmp - j) + 6;
      for (int b_i{jp1j}; b_i <= i; b_i++) {
        A[b_i - 1] /= A[b_tmp];
      }
    }
    jA = b_tmp;
    for (jBcol = 0; jBcol <= mmj_tmp; jBcol++) {
      smax = A[(b_tmp + jBcol * 6) + 6];
      if (smax != 0.0) {
        i = jA + 8;
        jp1j = (jA - j) + 12;
        for (kAcol = i; kAcol <= jp1j; kAcol++) {
          A[kAcol - 1] += A[((b_tmp + kAcol) - jA) - 7] * -smax;
        }
      }
      jA += 6;
    }
    i1 = ipiv[j];
    if (i1 != j + 1) {
      smax = QT[j];
      QT[j] = QT[i1 - 1];
      QT[i1 - 1] = smax;
      smax = QT[j + 6];
      QT[j + 6] = QT[i1 + 5];
      QT[i1 + 5] = smax;
      smax = QT[j + 12];
      QT[j + 12] = QT[i1 + 11];
      QT[i1 + 11] = smax;
    }
  }
  for (int j{0}; j < 3; j++) {
    jBcol = 6 * j;
    for (int k{0}; k < 6; k++) {
      kAcol = 6 * k;
      i = k + jBcol;
      if (QT[i] != 0.0) {
        jp1j = k + 2;
        for (int b_i{jp1j}; b_i < 7; b_i++) {
          jA = (b_i + jBcol) - 1;
          QT[jA] -= QT[i] * A[(b_i + kAcol) - 1];
        }
      }
    }
  }
  for (int j{0}; j < 3; j++) {
    jBcol = 6 * j;
    for (int k{5}; k >= 0; k--) {
      kAcol = 6 * k;
      i = k + jBcol;
      smax = QT[i];
      if (smax != 0.0) {
        QT[i] = smax / A[k + kAcol];
        for (int b_i{0}; b_i < k; b_i++) {
          jA = b_i + jBcol;
          QT[jA] -= QT[i] * A[b_i + kAcol];
        }
      }
    }
  }
}

// End of code generation (quintic_traj.cpp)
