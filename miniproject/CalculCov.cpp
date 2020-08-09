#include <Eigen>

#include "calculate.h"
#include "read.h"

void printMatrix(const Eigen::MatrixXd & corr) {
  for (int i = 0; i < corr.rows(); i++) {
    for (int j = 0; j < corr.cols(); j++)
      std::cout << corr(i, j) << "% ";
    std::cout << std::endl;
  }
}

void printAssets(Assets & assets) {
  for (unsigned long i = 0; i < assets.size(); i++)
    std::cout << assets[i];
}

Eigen::MatrixXd CalCov(const Eigen::MatrixXd & corr, const Assets & assets) {
  unsigned long size = corr.rows();
  Eigen::MatrixXd cov = Eigen::MatrixXd::Zero(size, size);
  for (unsigned long i = 0; i < size; i++) {
    for (unsigned long j = 0; j < size; j++) {
      cov(i, j) =
          (double)corr(i, j) / 100 * assets[i].getStDev() / 100 * assets[j].getStDev();
    }
  }
  return cov;
}
