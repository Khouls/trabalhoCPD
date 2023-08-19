#include <iostream>
#include <string>
#include <vector>

#include "parser.hpp"
#include "player.hpp"
#include "hashtable.hpp"

using namespace std;

#define N_PLAYERS 18944
#define PLAYERS_SIZE N_PLAYERS / 5



int main() {
  ifstream f("tags.csv");
  aria::csv::CsvParser parser(f);

  // Skip header
  //parser.next_field();
  //parser.next_field();
  //parser.next_field();
  //parser.next_field();

  int idx = 0;
  for (auto& row : parser) {
    //cout << row[0] << " | " << row[1] << " | " << row[2] << endl;
    idx++;
  }

  cout << "Hi" << endl;
  
}