#include "parse.hpp"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include <exception>
#include <fstream>
#include <iostream>
#include <string>
// any other headers you need

class invalid_input : public std::exception {
 public:
  const char * what() const throw() { return "Wrong Format!\n"; };
  // override what() here
};

item_t * parseLine(const std::string & line) {
  std::string::size_type position1;
  std::string::size_type position11;
  std::string::size_type position2;
  std::string::size_type position22;
  position1 = line.find(":");
  if (position1 == line.npos) {
    throw invalid_input();
  }
  position11 = line.find_first_not_of(" ");
  item_t * it = new item_t;
  it->first = line.substr(position11, position1 - position11);
  size_t sz = line.size();
  char line1[sz];
  size_t length = line.copy(line1, sz - position1, position1 + 1);
  line1[length] = '\0';
  std::string l = line1;
  while (1) {
    position2 = l.find(',');
    position22 = l.find_first_not_of(' ');
    if (position2 == l.npos && (position22 == l.size() - 1)) {
      break;
    }
    if (position2 == l.npos && (position22 != l.size() - 1)) {
      it->second.push_back(l.substr(position22, 2));
      break;
    }
    if (((position2 - position22) != 2) || position22 != 1) {
      delete it;
      throw invalid_input();
    }
    it->second.push_back(l.substr(position22, 2));
    l.erase(0, position2 + 1);
  }
  return it;  // write me
}

int main(int argc, char ** argv) {
  if (argc != 2) {
    std::cerr << "wrong";
    return EXIT_FAILURE;
  }
  std::ifstream f(argv[1]);
  if (!f.is_open()) {
    std::cerr << "Cannot open File";
    return EXIT_FAILURE;
  }
  std::string line;
  item_t * a;
  while (getline(f, line)) {
    try {
      a = parseLine(line);
      std::cout << "key = " << a->first << "\n";
      std::cout << "  values = ";
      for (size_t i = 0; i < a->second.size(); i++) {
        if (i == a->second.size() - 1) {
          std::cout << a->second[i];
          break;
        }
        std::cout << a->second[i] << "; ";
      }
      std::cout << "\n";
      delete a;
    }
    catch (std::exception & ex) {
      std::cerr << ex.what();
    }
  }

  f.close();
  return EXIT_SUCCESS;
}
