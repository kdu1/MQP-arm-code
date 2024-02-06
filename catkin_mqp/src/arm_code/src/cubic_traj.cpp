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
#include <eigen3/Eigen/Dense>
//#include <Eigen/Dense>
//#include <unsupported/Eigen/FFT>


//typedef std::complex<float> Complex;
//typedef std::valarray<Complex> CArray;



// Function Declarations
//static float rt_powd_snf(float u0, float u1);

// Function Definitions
static float rt_powd_snf(float u0, float u1)
{
  ROS_INFO("rt_powd_snf");
  float y;
  if (std::isnan(u0) || std::isnan(u1)) {
    y = rtNaN;
  } else {
    float d;
    float d1;
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
 * @param tf, vi, vf, pi, pf 
 * traj_time, velocity parameters and position parameters
 * Solves Ax=b equation
 * @return 1X12 Carray aka a flattened 4x3 matrix
 *
*/

CArray cubic_traj(float tf, std::vector<float> vi, std::vector<float> vf, std::vector<float> pi, std::vector<float> pf)
{
  ROS_INFO("cubic_traj");
  ROS_INFO("pf[0]: %f", pf[0]);
  ROS_INFO("pf[1]: %f", pf[1]);
  ROS_INFO("pf[2]: %f", pf[2]);

  //input size check
  if(vi.size() != 3 || vf.size() != 3 || pi.size() != 3 || pf.size() != 3){
      ROS_ERROR("Error: cubic_traj input is wrong size");
      throw std::runtime_error("Error: cubic_traj input is wrong size");
  }

  CArray CTcomplex;
  int CTcomplexsize = 12;
  CTcomplex.resize(CTcomplexsize); //initialize CArray size
  std::vector<float> CT;
  float A[16]; //they just. make a 4x4 array into a 1x16 array
  float smax;
  int i;
  int i1;
  int jA;
  int jBcol;
  int jp1j;
  signed char ipiv[4];
  //A matrix
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
  ROS_INFO("After rt_powd_snf");
  A[3] = 0.0;
  A[7] = 1.0;
  A[11] = 2.0 * tf;
  A[15] = 3.0 * smax;
  ROS_INFO("before CT");
  ROS_INFO("size of pi:%d", pi.size());
  //fill ct with all the values in the correct order
  CT.push_back(pi[0]);
  CT.push_back(vi[0]);
  CT.push_back(pf[0]);
  CT.push_back(vf[0]);
  CT.push_back(pi[1]);
  CT.push_back(vi[1]);
  CT.push_back(pf[1]);
  CT.push_back(vf[1]);
  CT.push_back(pi[2]);
  CT.push_back(vi[2]);
  CT.push_back(pf[2]);
  CT.push_back(vf[2]);
  
  ROS_INFO("before ipiv");
  ipiv[0] = 1;
  ipiv[1] = 2;
  ipiv[2] = 3;
  ipiv[3] = 4;
  ROS_INFO("before for loop");
  for (int j = 0; j < 3; j++) {
    ROS_INFO("j = %d", j);
    int b_tmp;
    int mmj_tmp;
    signed char i2;
    mmj_tmp = 2 - j;
    b_tmp = j * 5;
    jp1j = b_tmp + 2;
    jA = 4 - j;
    jBcol = 0;
    ROS_INFO("A[%d]", b_tmp);
    smax = std::abs(A[b_tmp]);
    for (int k{2}; k <= jA; k++) {
      float s;
      ROS_INFO("A[%d]", (b_tmp + k) - 1);
      s = std::abs(A[(b_tmp + k) - 1]);
      if (s > smax) {
        jBcol = k - 1;
        smax = s;
      }
    }
    ROS_INFO("A[%d]", b_tmp + jBcol);
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
        ROS_INFO("A[%d]", b_i - 1);
        ROS_INFO("A[%d]", b_tmp);
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
          ROS_INFO("A[%d]", jp1j - 1);
          ROS_INFO("A[%d]", ((b_tmp + jp1j) - jA) - 5);
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
  ROS_INFO("before next for loop");
  for (int j = 0; j < 3; j++) {
    jBcol = j << 2;
    for (int k{0}; k < 4; k++) {
      jA = k << 2;
      i = k + jBcol;
      if (CT[i] != 0.0) {
        i1 = k + 2;
        for (int b_i{i1}; b_i < 5; b_i++) {
          jp1j = (b_i + jBcol) - 1;
          ROS_INFO("jp1j CT[%d]", (b_i + jBcol) - 1);
          ROS_INFO("A[%d]", (b_i + jA) - 1);
          CT[jp1j] -= CT[i] * A[(b_i + jA) - 1];
        }
      }
    }
  }
  ROS_INFO("why are there like 10 for loops");
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
          ROS_INFO("jp1j CT[%d]", (b_i + jBcol));
          ROS_INFO("A[%d]", b_i + jA);
          CT[jp1j] -= CT[i] * A[b_i + jA];
        }
      }
    }
  }

 
  ROS_INFO("before convert to complex for loop");
  //convert to complex
  ROS_INFO("CT SIZE: %d", CT.size());
  ROS_INFO("CTcomplex size: %d", CTcomplex.size()); 
  for(int i= 0; i < CT.size(); i++){
      //ROS_INFO("CT at %d: %d", i, CT[i]);
      CTcomplex[i] = Complex(*(CT.begin()), 0);//using get first and erase method instead
      //CT.erase(CT.begin());
  }

  //CArray ret[][3];
   //believe what I need to do is convert a 1x12 array to a 4x3 one.
  /*for(int i = 0, j = 0; j < CTcomplex.size(); j++){
    ret[i][j%3] = CTcomplex[j];
    //end of column, start new row
    if(j%3 = 0){
      i++;
    }
  }*/

  //output size check
  if(CTcomplex.size() != 12){
      ROS_INFO("CTcomplex size: %d", CTcomplex.size());
      ROS_ERROR("Error: cubic_traj output is wrong size");
      throw std::runtime_error("Error: cubic_traj output is wrong size");
  }
  return CTcomplex;
}


//attempt at using the eigen vector below
//don't really know what's going on with b being a 4X3 vector, solve will only work with a 4X1 vector I believe. But it also has to return a 4X3 vector in the end.
/**
 * @param tf, vi, vf, pi, pf 
 * traj_time, velocity parameters and position parameters
 * @return 4X3 Eigen Matrix
 * solves matrix equation and returns values to be converted to complex CArray
 *
*/
/*Eigen::Vector4d cubic_traj_eigen(float tf, std::vector<float> vi, std::vector<float> vf, std::vector<float> pi, std::vector<float> pf) {
    Eigen::Matrix4d A;
    A << 1, 0, 0, 0,
         0, 1, 0, 0,
         1, tf, tf*tf, tf*tf*tf,
         0, 1, 2*tf, 3*tf*tf;
   //TODO: literally just filling the extra column with 0 for now, I think this could work?      
    Eigen::Matrix<float, 4, 3>;
    b << pi[0], pi[1], pi[2],  //technically this doesn't make sense, means that there are variables on both sides. Could this be simplified by making it so one side is equal to 0?
         vi[0], vi[1], vi[2],
         pf[0], pf[1], pf[2],
         vf[0], vf[1], vf[2];

   // Eigen::LLT<Matrix4d> lltOfA(A); // compute the Cholesky decomposition of A
    //Eigen::Matrix4d L = lltOfA.matrixL(); // retrieve factor L  in the decomposition
    //Eigen::Matrix<Complex, 4, 3> b(pi, vi, pf, vf);
    Eigen::Matrix<float, 4, 3> CT = A.colPivHouseholderQr().solve(b); //can I even do this if b is a 4x3?
   // Eigen::Matrix<float, 4, 3> CT = L.solve(b);
    return CT;
}*/

/**
 * @param tf, vi, vf, pi, pf 
 * traj_time, velocity parameters and position parameters
 * @return 4X3 CArray 
 *
*/
/*
CArray cubic_traj(float tf, std::vector<float> vi, std::vector<float> vf, std::vector<float> pi, std::vector<float> pf){
    Eigen::Vector4d mat = cubic_traj_eigen(tf, vi, vf, pi, pf);
    std::vector<float> vec(mat.data(), mat.data() + mat.rows() * mat.cols());

    CArray ret;

    for(int i = 0; i < vec.size(); i++){
      ret[i] = Complex(vec[i]);
    }

    return ret;
}*/

// End of code generation (cubic_traj.cpp)
