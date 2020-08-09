#ifndef _CALCULATE_H_
#define _CALCULATE_H_
#include <stdio.h>
#include <stdlib.h>

#include <Eigen>

#include "read.h"
// the header file for calculation
//in CalculCov.cpp
void printMatrix(const Eigen::MatrixXd & corr);
void printAssets(Assets & assets);

Eigen::MatrixXd CalCov(const Eigen::MatrixXd & corr,
                       const Assets & assets);  // get cov matrix

//in Lagrange.cpp
void Lagrange_step2(const Eigen::MatrixXd & cov, double ROR, const Assets & assets);
void RangeLagrange_step2(const Eigen::MatrixXd & cov,
                         const Assets & assets,
                         double Low,
                         double High,
                         double interval);
void Lagrange_step3(const Eigen::MatrixXd & cov, double ROR, const Assets & assets);
void RangeLagrange_step3(const Eigen::MatrixXd & cov,
                         const Assets & assets,
                         double Low,
                         double High,
                         double interval);
#endif
