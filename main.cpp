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
// Nearest prime of N_PLAYERS / 5
#define PLAYERS_SIZE 3793

#define N_USERS 138493
// Nearest prime of N_USERS / 3
#define USERS_SIZE 46171


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

  cout << "---------------------------" << endl;
  
  vector<int> playersFound;
  trieHead->searchPrefix("Fer", &playersFound);

  for (int i = 0; i < playersFound.size(); ++i){
    Player* player = playersList.get(playersFound[i]);
    cout << "ID in hashTable: " << player->id << " | ";
    cout << "Sofifa ID: " << player->sofifaID << " | ";
    cout << "Name: " << player->name << " | ";
    cout << "Avg. Rating: " << player->totalRating / player->ratingCount << endl;
  }

  cout << "---------------------------" << endl;

  User* user = reviewsList.get(4);
  vector<UserReview> reviewsFound;

  user->getReviews(&reviewsFound);

  for (int i = 0; i < reviewsFound.size(); ++i){
    Player* player = playersList.get(reviewsFound[i].playerID);
    cout << "Sofifa ID: " << player->sofifaID << " | ";
    cout << "Name: " << player->name << " | ";
    cout << "Rating: " << reviewsFound[i].rating << " | ";
    cout << "Avg. Rating: " << player->totalRating / player->ratingCount << endl;
  }

  return 0;

}