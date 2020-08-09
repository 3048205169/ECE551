#include <math.h>

#include <Eigen>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>

#include "calculate.h"
#include "read.h"

//Step2
//compute and build the matrix A in linear equation Ax=B
void getCoefficient_step2(const Eigen::MatrixXd & cov,
                          Eigen::MatrixXd & A,
                          Eigen::VectorXd & AveRes) {
  unsigned int sz = cov.rows();
  for (unsigned int i = 0; i < sz; i++) {
    for (unsigned int j = 0; j < sz; j++) {
      A(i, j) = cov(i, j);
    }
    A(i, sz) = 1;
    A(i, sz + 1) = AveRes(i);
  }
  for (unsigned int i = 0; i < sz; i++) {
    A(sz, i) = 1;
    A(sz + 1, i) = AveRes(i);
  }
}

void Lagrange_step2(const Eigen::MatrixXd & cov, double ROR, const Assets & assets) {
  //Transfer the optimization problem to a linear equation:
  unsigned int sz = cov.rows() + 2;
  Eigen::MatrixXd A = Eigen::MatrixXd::Zero(sz, sz);
  Eigen::VectorXd AveRes(assets.size());
  AveRes = getAveRes(assets);  //in the read.cpp. To get the return vector from assets.
  Eigen::VectorXd B = Eigen::VectorXd::Zero(sz);
  getCoefficient_step2(cov, A, AveRes);
  B(sz - 2) = 1;
  B(sz - 1) = ROR;

  //solver linear equation and maybe can choose different methods depending on different linear equation
  Eigen::VectorXd x = Eigen::VectorXd::Zero(sz);
  //x = A.fullPivHouseholderQr().solve(B);
  //x = A.colPivHouseholderQr().solve(B);
  x = A.lu().solve(B);
  //x = A.fullPivLu().solve(B);

  //X is the wieght + Lagrange Multiplier , so we need to get the fisrt 7 elements to build the weight
  Eigen::VectorXd weight = Eigen::VectorXd::Zero(sz - 2);
  for (unsigned int i = 0; i < sz - 2; i++) {
    //test if there is some weight cannot be solved
    if (isnan(x(i))) {
      std::cout << "The ROR " << ROR << "% have no solution" << std::endl;
      return;
    }
    weight(i) = x(i);
  }
  //compute var and volatility
  double var = weight.transpose() * cov * weight;
  double vol = sqrt(var / 100) * 100;

  std::cout << "The Volatility for ROR " << ROR << "% is "
            << std::setiosflags(std::ios::fixed) << std::setprecision(2) << vol << "%"
            << std::endl;
}

//output the result in the range we want
void RangeLagrange_step2(const Eigen::MatrixXd & cov,
                         const Assets & assets,
                         double Low,
                         double High,
                         double interval) {
  for (double i = Low; i <= High; i = i + interval) {
    Lagrange_step2(cov, i, assets);
  }
}

//step3
//use KKT condition to solve this proble
//check if all of the weight/KKT condition is larger than 0;
int CheckConstraint(Eigen::VectorXd x, unsigned int sz) {
  for (unsigned int i = 0; i < sz - 2; i++) {
    if (x(i) < 0)
      return i;
  }
  return -1;
}

//Get if there are weight is less than zero; than record them and transfer them to zero to reach boundary
std::vector<int> getIndex(Eigen::VectorXd x, unsigned int sz) {
  std::vector<int> indexs_no;
  for (unsigned int i = 0; i < sz - 2; i++) {
    if (x(i) < 0)
      indexs_no.push_back(i);
  }
  return indexs_no;
}

//change the A of the linear equation, to let x_i = 0 and let Lagrange Multiplier of inequaility of x_i not equal to 0;
Eigen::MatrixXd ChangeConstraint(Eigen::MatrixXd & A, std::vector<int> indexs) {
  Eigen::MatrixXd A_temp = A;
  for (unsigned int j = 0; j < indexs.size(); j++) {
    unsigned int index = indexs[j];
    for (unsigned int i = 0; i < A.rows(); i++)
      A_temp(i, index) = 0;
    A_temp(index, index) = -1;
  }
  return A_temp;
}

void Lagrange_step3(const Eigen::MatrixXd & cov, double ROR, const Assets & assets) {
  //Transfer the optimization problem to a linear system:
  unsigned int sz = cov.rows() + 2;
  Eigen::MatrixXd A = Eigen::MatrixXd::Zero(sz, sz);
  Eigen::VectorXd AveRes(assets.size());
  AveRes = getAveRes(assets);  //in the read.cpp.
  Eigen::VectorXd B = Eigen::VectorXd::Zero(sz);
  getCoefficient_step2(cov, A, AveRes);
  B(sz - 2) = 1;
  B(sz - 1) = ROR;

  //solver linear equation Ax=B
  Eigen::VectorXd x = Eigen::VectorXd::Zero(sz);
  //x = A.fullPivHouseholderQr().solve(B);
  //x = A.colPivHouseholderQr().solve(B);
  x = A.lu().solve(B);
  //x = A.fullPivLu().solve(B);
  int KKTflag = -2;

  //compute the weight without nonegitive constraint and get the index of negative weight
  std::vector<int> indexs_neg;
  indexs_neg = getIndex(x, sz);

  //transfer negative x to zero one-by-one
  for (unsigned int i = 0; i < indexs_neg.size(); i++) {
    //get the first index which weight is negative
    std::vector<int> indexs_wg;
    indexs_wg.push_back(indexs_neg[i]);

    //check if there is a situation satify the KKT condition
    while ((KKTflag = CheckConstraint(x, sz)) != -1) {
      Eigen::MatrixXd A_temp = ChangeConstraint(A, indexs_wg);
      Eigen::VectorXd x1 = Eigen::VectorXd::Zero(sz);
      x1 = A_temp.colPivHouseholderQr().solve(B);

      //check if there are coeffient of inequailty is negative, which means the KKT is not satised, so we can jump out.
      int testnegative = 0;
      for (unsigned int i = 0; i < indexs_wg.size(); i++) {
        if (x1(indexs_wg[i]) < 0) {
          testnegative++;
        }
        x1(indexs_wg[i]) = 0;
      }
      if (testnegative != 0)
        break;

      //get the new index where weight is negative
      std::vector<int> index_new = getIndex(x1, sz);
      indexs_wg.insert(indexs_wg.end(), index_new.begin(), index_new.end());
      x = x1;
    }
    if (KKTflag == -1) {
      break;
    }
  }

  //if there is no situation that satify the KKT condition
  if (KKTflag != -1) {
    std::cout << "Cannot Find a solution satified KKT condition when ROR is " << ROR
              << "%" << std::endl;
    return;
  }

  Eigen::VectorXd weight = Eigen::VectorXd::Zero(sz - 2);
  for (unsigned int i = 0; i < sz - 2; i++) {
    //test if there is some weight cannot be solved
    if (isnan(x(i))) {
      std::cout << "The ROR " << ROR << "% have no solution" << std::endl;
      return;
    }
    weight(i) = x(i);
  }
  double var = weight.transpose() * cov * weight;
  double vol = sqrt(var / 100) * 100;
  std::cout << "The Volatility for ROR " << ROR << "% is "
            << std::setiosflags(std::ios::fixed) << std::setprecision(2) << vol << "%"
            << std::endl;
}

void RangeLagrange_step3(const Eigen::MatrixXd & cov,
                         const Assets & assets,
                         double Low,
                         double High,
                         double interval) {
  for (double i = Low; i <= High; i = i + interval) {
    Lagrange_step3(cov, i, assets);
  }
}
