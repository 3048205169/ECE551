#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <list>
#include <vector>

#include "task.hpp"

class invalid_input : public std::exception {
 public:
  const char * what() const throw() { return "Wrong Format!\n"; };
};

TaskDep * parseLine(const std::string & line) {
  std::string::size_type position1;
  std::string::size_type position11;
  std::string::size_type position2;
  std::string::size_type position22;
  position1 = line.find(":");
  if (position1 == line.npos) {
    throw invalid_input();
  }
  position11 = line.find_first_not_of(" ");
  TaskDep * it = new TaskDep(line.substr(position11, position1 - position11));
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
    if (position22 != 1) {
      delete it;
      throw invalid_input();
    }
    if (position2 == l.npos && (position22 != l.size() - 1)) {
      std::list<task_t> pre = it->getPredecessors();
      std::list<task_t>::iterator iter =
          find(pre.begin(), pre.end(), l.substr(position22, l.size() - position22));
      if (iter == pre.end())
        it->addPredecessor(l.substr(position22, l.size() - position22));
      break;
    }

    std::list<task_t> pre = it->getPredecessors();
    std::list<task_t>::iterator iter =
        find(pre.begin(), pre.end(), l.substr(position22, position2 - position22));
    if (iter == pre.end())
      it->addPredecessor(l.substr(position22, position2 - position22));
    //std::cout << l.substr(position22, l.size() - position22);
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
  std::vector<TaskDep *> TK;

  while (getline(f, line)) {
    try {
      TK.push_back(parseLine(line));
    }
    catch (std::exception & ex) {
      std::cerr << ex.what();
      for (size_t i = 0; i < TK.size(); i++) {
        delete TK[i];
      }
      return EXIT_FAILURE;
    }
  }
  std::list<task_t> ans;
  std::list<task_t> pre;
  size_t fg = 0;
  while (TK.size() != 0) {
    for (size_t i = 0; i < TK.size(); i++) {
      pre = TK[i]->getPredecessors();
      if (pre.empty()) {
        ans.push_front(TK[i]->getTask());
      }
      else {
        std::list<task_t>::iterator iter;
        std::list<task_t>::iterator pos;
        size_t flag = 0;
        for (iter = ans.begin(); iter != ans.end(); iter++) {
          std::list<task_t>::iterator tem = std::find(pre.begin(), pre.end(), *iter);
          if (tem != pre.end()) {
            pos = iter;
            flag++;
          }
        }
        if (flag != pre.size()) {
          continue;
        }
        pos++;
        ans.insert(pos, TK[i]->getTask());
      }
      delete TK[i];
      TK.erase(TK.begin() + i);
      i--;
      fg++;
    }
    if (fg == 0 && TK.size() != 0) {
      std::cerr << "the dependecies do not allow a todo"
                << "list to be created "
                << "\n";
      for (size_t i = 0; i < TK.size(); i++) {
        delete TK[i];
      }

      return EXIT_FAILURE;
    }
    fg = 0;
  }

  for (std::list<task_t>::iterator iter = ans.begin(); iter != ans.end(); iter++) {
    if (iter == (--ans.end())) {
      std::cout << *iter;
      break;
    }

    std::cout << *iter << ", ";
  }

  f.close();
  return EXIT_SUCCESS;
}
