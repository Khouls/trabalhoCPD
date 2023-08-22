#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <set>

#include "parser.hpp"
#include "player.hpp"
#include "hashtable.hpp"

using namespace std;

#define N_TAGS 963
// Closest prime to N_TAGS / 5
#define TAG_SIZE 193
#define PRIME 13

unsigned int hashTag(string tag) {
  unsigned int hash = 0;
  unsigned long long int power = 0;

  int strLen = tag.length();

  for (int i = 0; i < strLen; ++i) {
    power = tag[i];
    power *= pow(PRIME, i);
    hash += power;
    hash = hash % TAG_SIZE;
  }
  return hash;  
}

int main() {
  ifstream f("tags.csv");
  aria::csv::CsvParser parser(f);

  HashTable<TagTuple> tagTable(TAG_SIZE);

  // Skip header
  parser.next_field();
  parser.next_field();
  parser.next_field();
  parser.next_field();

  for (auto& tagRow : parser) {
    int tagUserID = stoi(tagRow[0]);
    int tagSofifaID = stoi(tagRow[1]);
    string tag = tagRow[2];
    unsigned int hashedID = hashTag(tag);  

    TagTuple* tagTableEntry = tagTable.get(tag, hashedID);
    if (tagTableEntry == nullptr) {
      TagTuple tagTuple = TagTuple{tag, new set<PlayerTuple>};
      tagTableEntry = tagTable.insertElement(tagTuple, hashTag(tag));
    }
    tagTableEntry->players->insert(PlayerTuple{tagSofifaID, 0});
  }

  return 0;
}