#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <set>

#include "parser.hpp"
#include "player.hpp"
#include "hashtable.hpp"

using namespace std;

#define PRIME 13

unsigned int hashTag(string tag, int tableSize) {
  unsigned int hash = 0;
  unsigned long long int power = 0;

  int strLen = tag.length();

  for (int i = 0; i < strLen; ++i) {
    power = tag[i];
    power *= pow(PRIME, i);
    hash += power;
    hash = hash % tableSize;
  }
  return hash;  
}
