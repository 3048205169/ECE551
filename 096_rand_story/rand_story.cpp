#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fstream>
#include <iostream>
#include <map>
#include <vector>

#include "rand.h"
typedef std::map<std::string, class WordLib> Map;
typedef std::vector<std::string> strings;

strings & split(const std::string & str) {
  strings * res = new strings();
  int position = -1;
  for (unsigned long i = 0; i < str.size(); i++) {
    if (str[i] == ':') {
      if (position == -1) {
        position = i;
      }
      else {
        return *res;
      }
    }
  }
  if (position == -1) {
    return *res;
  }
  std::string res1;
  std::string res2;
  res1 = str.substr(0, position);
  res2 = str.substr(position + 1, str.size() - 1);
  res->push_back(res1);
  res->push_back(res2);
  return *res;
}

bool is_digits(const std::string & str) {
  return str.find_first_not_of("0123456789") == std::string::npos;
}

Map * BuildWordLib(const char * filename) {
  std::string str;
  Map * map = new Map();
  std::ifstream f;
  f.open(filename);
  if (!f.is_open()) {
    std::cerr << "Cannot open the file";
    delete map;
    exit(EXIT_FAILURE);
  }
  while (getline(f, str)) {
    if (str.empty()) {
      std::cerr << "Please use the right format on words.txt, ok?";
      f.close();
      delete map;
      exit(EXIT_FAILURE);
    }

    strings & s = split(str);
    //std::cout << "HEllo ";
    if (s.empty()) {
      std::cerr << "Please use the right format on words.txt, o?";
      f.close();
      delete &s;
      delete map;
      exit(EXIT_FAILURE);
    }
    if (s[0].empty() || s[1].empty()) {
      std::cerr << "Please use the right format on words.txt";
      f.close();
      delete &s;
      delete map;
      exit(EXIT_FAILURE);
    }
    Map::iterator search = map->find(s[0]);
    if (search != map->end()) {
      search->second.addWord(s[1]);
    }
    class WordLib wordlib = WordLib(s[0], s[1]);
    map->insert(std::make_pair(s[0], wordlib));
    delete &s;
  }

  f.close();
  return map;
}

std::string GenerateStory(Map * map, const char * filename, unsigned int seed) {
  std::string story;
  std::ifstream f;
  f.open(filename);
  if (!f.is_open()) {
    std::cerr << "Cannot open the file";
    delete map;
    exit(EXIT_FAILURE);
  }
  srand(seed);
  char letter;
  strings types;
  strings record;
  int flag = 0;
  while (!f.eof()) {
    letter = f.get();
    if (letter == -1)
      break;
    if (letter == '_') {
      flag++;
      char letter2;
      std::string word;
      while ((letter2 = f.get()) != '_') {
        word.push_back(letter2);
      }
      if (is_digits(word)) {
        int num = atoi(word.c_str());
        int sz = types.size();
        if (num > sz) {
          std::cerr << "Number refers past beginning of story";
          f.close();
          delete map;
          exit(EXIT_FAILURE);
        }
        word = types[sz - num];
        types.push_back(word);
        story.append(record[sz - num]);
        record.push_back(record[sz - num]);
        continue;
      }
      Map::iterator search = map->find(word);
      if (search != map->end()) {
        class WordLib lib = search->second;
        int index = rand() % lib.getNum();
        record.push_back(lib[index]);
        story.append(lib[index]);
      }
      else {
        std::cerr << "We do not have this type in words";
        f.close();
        delete map;
        exit(EXIT_FAILURE);
      }
      types.push_back(word);
      continue;
    }
    story.push_back(letter);
  }
  if (flag == 0) {
    std::cerr << "there is no type in the story";
    exit(EXIT_FAILURE);
  }
  f.close();
  return story;
}

int main(int argc, char ** argv) {
  if (argc != 4) {
    std::cerr << "We need three arguments.";
    return EXIT_FAILURE;
  }
  unsigned int seed = atoi(argv[3]);
  if (seed < 1) {
    std::cerr << "Seed must be greated than 1";
    return EXIT_FAILURE;
  }
  Map * map = BuildWordLib(argv[2]);
  std::string story = GenerateStory(map, argv[1], seed);
  std::cout << story;
  delete map;
  return EXIT_SUCCESS;
}
