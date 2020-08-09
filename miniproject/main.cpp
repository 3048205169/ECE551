#include <stdio.h>
#include <stdlib.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "calculate.h"
#include "read.h"

int main(int argc, char ** argv) {
  //test the input number
  if (argc != 3) {
    std::cerr << "wrong input. Please give me two files." << std::endl;
    return EXIT_FAILURE;
  }

  //read assets from assets.csv
  Assets assets;
  assets = read_assets(argv[1]);
  if (assets.size() == 0) {
    return EXIT_FAILURE;
  }
  std::cout << "We have the assets information:" << std::endl;
  printAssets(assets);
  std::cout << std::endl;

  //read the corr.csv from file
  unsigned int sz = assets.size();
  Eigen::MatrixXd corr = Eigen::MatrixXd::Zero(sz, sz);
  corr = read_cor(argv[2], &assets);
  if (corr.size() == 0) {
    return EXIT_FAILURE;
  }
  std::cout << "We have the Correlation Matrix:" << std::endl << corr << std::endl;
  std::cout << "(omit the % of each number)" << std::endl << std::endl;

  //compute and output the cov matirx
  Eigen::MatrixXd cov = Eigen::MatrixXd::Zero(sz, sz);
  cov = CalCov(corr, assets);
  std::cout << "We have the Covariance Matrix:" << std::endl;
  printMatrix(cov);
  std::cout << std::endl;

  //compute and output the result without nonegative constraint from 1% to 26% with a interval 1%
  std::cout << std::endl << "Without nonegative constraint:" << std::endl;
  RangeLagrange_step2(cov, assets, 1, 26, 1);

  //compute and output the result with constraint from 1% to 26% with a interval 1%
  std::cout << std::endl << "With nonegative constraint:" << std::endl;
  RangeLagrange_step3(cov, assets, 1, 26, 1);

  return 0;
}
