#ifndef _READ_H_
#define _READ_H_
#include <stdio.h>
#include <stdlib.h>

#include <Eigen>
#include <iostream>
#include <string>
#include <vector>
//it is the header file for reading from file and some basic class

typedef std::vector<std::string> strings;
typedef std::vector<class Asset> Assets;

Eigen::MatrixXd read_cor(const char * filename, Assets * assets);  //read cor.csv
bool parseComma(std::string & line, strings & linestr);            //parse line with ","
bool CheckCorr(Assets * assets, strings * info);  //check the first line of corr.csv

Assets read_assets(const char * filename);  //read assets.csv

Eigen::VectorXd getAveRes(const Assets & assets);  //generate a vector of average return

//the class of Asset
class Asset {
 private:
  std::string Name;  //Asset's name
  double StDev;      //standard deviation
  double AveRe;      //average return

 public:
  Asset(std::string name, double StDev, double AveRe) :
      Name(name),
      StDev(StDev),
      AveRe(AveRe){};
  Asset(const Asset & rhs) : Name(rhs.Name), StDev(rhs.StDev), AveRe(rhs.AveRe) {}
  ~Asset(){};
  std::string getName() const { return Name; }
  double getStDev() const { return StDev; }
  double getAveRe() const { return AveRe; }

  friend std::ostream & operator<<(std::ostream & out, class Asset a) {
    out << "Asset " << a.getName() << " have the standard deviation " << a.getStDev()
        << "% and the average return " << a.getAveRe() << "%" << std::endl;
    return out;
  }
};

#endif
