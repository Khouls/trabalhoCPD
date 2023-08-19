#include <iostream>
#include <string>
#include <vector>
#include <set>

#include "parser.hpp"
#include "player.hpp"
#include "user.hpp"
#include "trie.hpp"
#include "hashtable.hpp"

using namespace std;

#define N_PLAYERS 18944
#define PLAYERS_SIZE N_PLAYERS / 3
#define N_USERS 138493
#define USERS_SIZE N_USERS / 3


int main() {
  ifstream playersF("players.csv");
  aria::csv::CsvParser playersParser(playersF);

  ifstream tagsF("tags.csv");
  aria::csv::CsvParser tagsParser(tagsF);

  ifstream ratingsF("rating.csv");
  aria::csv::CsvParser ratingsParser(ratingsF);

  TrieNode* trieHead = new TrieNode();
  
  HashTable<Player> playersList(PLAYERS_SIZE);

  HashTable<User> reviewsList(USERS_SIZE);

  // Skip headers
  playersParser.next_field();
  playersParser.next_field();
  playersParser.next_field();
  playersParser.next_field();
  tagsParser.next_field();
  tagsParser.next_field();
  tagsParser.next_field();
  tagsParser.next_field();
  ratingsParser.next_field();
  ratingsParser.next_field();
  ratingsParser.next_field();
  ratingsParser.next_field();

  for (auto& playersRow : playersParser) {
    // sofifaID, name, positions
    int idx = stoi(playersRow[0]);
    Player player = Player{idx, playersRow[0], playersRow[1], 0, 0};

    // Insert into hashtable
    playersList.insertElement(player, idx);
    // Insert into Trie
    trieHead->insertPlayer(playersRow[1], idx);
  }

  for (auto& ratingRow : ratingsParser) {
    int ratingUserID = stoi(ratingRow[0]);
    int ratingSofifaID = stoi(ratingRow[1]);
    float rating = stof(ratingRow[2]);

    UserReview review = UserReview{ratingSofifaID, rating};

    Player * player = playersList.get(ratingSofifaID);
    player->totalRating += rating;
    player->ratingCount++;

    User* user = reviewsList.get(ratingUserID);

    if (user == nullptr) {
      user = new User(ratingUserID);
      reviewsList.insertElement(*user, ratingUserID);
    }

    user->addReview(review);

  }
  
  vector<int> playersFound;
  trieHead->searchPrefix("Fer", &playersFound);

  for (int i = 0; i < playersFound.size(); ++i){
    Player* player = playersList.get(playersFound[i]);
    cout << "ID in hashTable: " << player->id << " | ";
    cout << "Sofifa ID: " << player->sofifaID << " | ";
    cout << "Name: " << player->name << " | ";
    cout << "Avg. Rating: " << player->totalRating << endl;
  }

}