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
#define PLAYERS_SIZE N_PLAYERS / 5

int main() {
  ifstream playersF("players.csv");
  aria::csv::CsvParser playersParser(playersF);

  ifstream tagsF("tags.csv");
  aria::csv::CsvParser tagsParser(tagsF);

  ifstream ratingsF("rating.csv");
  aria::csv::CsvParser ratingsParser(ratingsF);

  TrieNode* trieHead = new TrieNode();
  
  HashTable<Player> playersList(PLAYERS_SIZE);

  vector<vector<UserReview>> reviewsList(20000);

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

  // Pre-read the first data line of the tags
  auto tagField = tagsParser.next_field();
  string tagUserID = *tagField.data;
  tagField = tagsParser.next_field();
  string tagSofifaID = *tagField.data;
  tagField = tagsParser.next_field();
  string tagName = *tagField.data;
  tagsParser.next_field();

  // Pre-read the first data line of the ratings
  auto ratingField = ratingsParser.next_field();
  int ratingUserID = stoi(*ratingField.data);
  ratingField = ratingsParser.next_field();
  string ratingSofifaID = *ratingField.data;
  ratingField = ratingsParser.next_field();
  float rating = stof(*ratingField.data);
  ratingsParser.next_field();

  int idx = 0;
  for (auto& playersRow : playersParser) {
    // sofifaID, name, positions
    Player player = Player{idx, playersRow[0], playersRow[1], 0, 0};

    // Read tags related to player
    while(playersRow[0].compare(tagSofifaID) == 0) {      
      // Read next row
      tagField = tagsParser.next_field();
      tagUserID = *tagField.data;

      tagField = tagsParser.next_field();
      tagSofifaID = *tagField.data;

      tagField = tagsParser.next_field();
      tagName = *tagField.data;

      tagsParser.next_field();
      
    }

    // Read ratings related to player
    while(playersRow[0].compare(ratingSofifaID) == 0) {      
      // Read next row
      ratingField = ratingsParser.next_field();
      ratingUserID = stoi(*ratingField.data);

      ratingField = ratingsParser.next_field();
      ratingSofifaID = *ratingField.data;

      ratingField = ratingsParser.next_field();
      rating = stof(*ratingField.data);

      UserReview review = UserReview{idx, ratingUserID};

      // Check if user exists
      User* user = reviewsList.get(ratingUserID);

      if (user == nullptr) {
        user = new User();
        reviewsList.insertElement(*user, ratingUserID);
      }

      user->addReview(review);
      

      player.avgRating += rating;
      player.ratingCount++;

      ratingsParser.next_field();
    }

    //cout << idx << endl;

    player.avgRating /= player.ratingCount;

    // Insert into hashtable
    playersList.insertElement(player, idx);
    // Insert into Trie
    trieHead->insertPlayer(playersRow[1], idx);

    idx++;
  }
  
  vector<int> playersFound;
  trieHead->searchPrefix("Fer", &playersFound);

  for (int i = 0; i < playersFound.size(); ++i){
    Player* player = playersList.get(playersFound[i]);
    cout << "ID in hashTable: " << player->id << " | ";
    cout << "Sofifa ID: " << player->sofifaID << " | ";
    cout << "Name: " << player->name << " | ";
    cout << "Avg. Rating: " << player->avgRating << endl;
  }

  User* user = reviewsList.get(4);
  vector<UserReview> reviewsFound;
  user->getReviews(&reviewsFound);

  for (int i = 0; i < reviewsFound.size(); ++i){
    Player* player = playersList.get(reviewsFound[i].playerID);
    cout << "Sofifa ID: " << player->sofifaID << " | ";
    cout << "Name: " << player->name << " | ";
    cout << "Rating: " << reviewsFound[i].rating << " | ";
    cout << "Avg. Rating: " << player->avgRating << endl;
  }

}