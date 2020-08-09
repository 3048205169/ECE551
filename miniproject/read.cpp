#include "read.h"

#include <stdio.h>
#include <stdlib.h>

#include <Eigen>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

//delete the space of the string.
void deleteSpace(strings & strings) {
  for (unsigned int i = 0; i < strings.size(); i++) {
    strings[i].erase(0, strings[i].find_first_not_of(" "));
    strings[i].erase(strings[i].find_last_not_of(" ") + 1);
  }
}

//seprate the line by the position of ","
bool parseComma(std::string & line, strings & linestr) {
  std::stringstream stringstream(line);
  std::string element;
  while (getline(stringstream, element, ','))
    linestr.push_back(element);
  if (linestr.size() == 0)
    return false;

  //delete the last char of last string: because it has a "\r" in the end for a csv file
  if (linestr[linestr.size() - 1][linestr[linestr.size() - 1].length() - 1] == '\r')
    linestr[linestr.size() - 1].pop_back();
  return true;
}

//check if the string is a number
bool is_digits(const std::string & str) {
  //Cheak if there is some strange char in the number line
  if (!(str.find_first_not_of(".-0123456789 ") == std::string::npos))
    return false;

  //check if  the "-" is in the first place of a number.
  if (!(str.find_first_of("-") == std::string::npos) &&
      str.find_first_of("-") != (str.find_first_of("0123456789.") - 1))
    return false;

  return true;
}

//delete the % and get the double from the data like "19%"
bool deletepercent(std::string & line, double & number) {
  if (line[line.size() - 1] != '%') {
    std::cerr << "Your data do not have %. please check data." << std::endl;
    return false;
  }

  line.pop_back();
  if (is_digits(line)) {
    number = std::stod(line);
    return true;
  }
  else {
    std::cerr << "Your data is not a number" << std::endl;
    return false;
  }
}

//test the first line of Assets
bool AssetsFirstLineTest(strings & info) {
  //test the number of attribuate
  if (info.size() != 3)
    return false;

  //test if the attribuate is not right.
  if (info[0].compare("Asset") || info[1].compare("Standard Deviation") ||
      info[2].compare("Average Return"))
    return false;
  return true;
}

//read from file Assets.csv
Assets read_assets(const char * filename) {
  //open file asset.csv
  std::ifstream fin(filename, std::ios::in);
  if (!fin) {
    std::cerr << "Cannot open file" << std::endl;
    Assets empty;
    return empty;
  }

  //build Assets
  std::string line;
  int lineRowNum = 0;  //tell us if it is the first row so i can check it.
  Assets assets;
  while (getline(fin, line)) {
    //get tne strings which contains every single elements of a line
    strings info;
    if (!parseComma(line, info)) {
      std::cerr << "You have a empty line in the File or file is empty" << std::endl;
      fin.close();
      Assets empty;
      return empty;
    }
    deleteSpace(info);

    //test the first line of assets.csv
    if (lineRowNum == 0) {
      if (!AssetsFirstLineTest(info)) {
        std::cerr << "File Wrong" << std::endl;
        fin.close();
        Assets empty;
        return empty;
      }
      lineRowNum = 1;
      continue;
    }

    //build the vector of assets
    //test the data counts of each line: we have name, standard deviation, and average mean each line
    if (info.size() != 3) {
      std::cerr << "Wrong Format on comma sizes" << std::endl;
      fin.close();
      Assets empty;
      return empty;
    }

    double StDev = 0;
    double AveRe = 0;
    //check the format of number
    if (!deletepercent(info[1], StDev) || !deletepercent(info[2], AveRe)) {
      Assets empty;
      return empty;
    }

    class Asset asset(info[0], StDev, AveRe);
    assets.push_back(asset);
  }

  fin.close();
  if (fin.is_open()) {
    std::cerr << "File close failed." << std::endl;
  }

  return assets;
}

//check if the name of correlation.csv is right
bool CheckCorr(Assets * assets, strings * info) {
  for (unsigned int i = 0; i < assets->size(); i++) {
    if ((*assets)[i].getName().compare((*info)[i])) {
      return true;
    }
  }
  return false;
}

//read correlation.csv
Eigen::MatrixXd read_cor(const char * filename, Assets * assets) {
  unsigned long size = assets->size();

  //test if the open of file is good
  std::ifstream fin(filename, std::ios::in);
  if (!fin) {
    std::cerr << "Cannot open file" << std::endl;
    Eigen::MatrixXd empty;
    return empty;
  }

  std::string line;
  int LineRowNum = 0;       //tell me if it is the first line of csv so i can check it.
  unsigned long index = 0;  //The rows index of correaltion matrix
  Eigen::MatrixXd corr = Eigen::MatrixXd::Zero(size, size);  //correaltion matrix

  while (getline(fin, line)) {
    //test if size of rows is larger than the assets' size. It means we get some useless data.
    if (index >= size) {
      std::cerr << "the rows is more than columns" << std::endl;
      fin.close();
      Eigen::MatrixXd empty;
      return empty;
    }

    //seperate the line and get the elements from line
    strings info;
    if (!parseComma(line, info)) {
      std::cerr << "You have a empty line in the File or file is empty" << std::endl;
      fin.close();
      Eigen::MatrixXd empty;
      return empty;
    }
    deleteSpace(info);

    //test the first line of correlation.csv
    if (LineRowNum == 0) {
      //erase the first empty element
      info.erase(info.begin());

      //check the size of the first line
      if (size != info.size()) {
        std::cerr << "wrong asset numbers on the file" << std::endl;
        fin.close();
        Eigen::MatrixXd empty;
        return empty;
      }

      //test if the assets' order and size in correaltion.csv is right
      if (CheckCorr(assets, &info)) {
        std::cerr << "please give me the right format of the corr.asv--- as the order of "
                     "the asset.csv"
                  << std::endl;
        fin.close();
        Eigen::MatrixXd empty;
        return empty;
      }

      LineRowNum = 1;
      continue;
    }

    //test if the assets's name is right
    if (info[0].compare((*assets)[index].getName())) {
      std::cerr
          << "The rows's name of assets is not right. Maybe order or name is not right"
          << std::endl;
      fin.close();
      Eigen::MatrixXd empty;
      return empty;
    }

    //erase the name and start to handle numers
    info.erase(info.begin());

    //check the size of the data
    if (size != info.size()) {
      std::cerr << "wrong asset numbers on the file" << std::endl;
      fin.close();
      Eigen::MatrixXd empty;
      return empty;
    }

    //input the data into corr
    for (unsigned int i = 0; i < info.size(); i++) {
      double corrnum = 0;
      if (!deletepercent(info[i], corrnum)) {
        Eigen::MatrixXd empty;
        return empty;
      }
      corr(index, i) = corrnum;
    }

    index++;
  }

  if (index != size) {
    std::cerr << "Rows is less than columns" << std::endl;
    Eigen::MatrixXd empty;
    return empty;
  }

  fin.close();
  if (fin.is_open()) {
    std::cerr << "File close failed." << std::endl;
  }

  return corr;
}

//from assets, we get a array of average return and transfor it to a Eigen::vector type
Eigen::VectorXd getAveRes(const Assets & assets) {
  unsigned int sz = assets.size();
  Eigen::VectorXd AveRes(sz);
  for (unsigned int i = 0; i < sz; i++) {
    AveRes(i) = assets[i].getAveRe();
  }
  return AveRes;
}
