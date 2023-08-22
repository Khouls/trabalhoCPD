#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <chrono> 

#include "parser.hpp"
#include "player.hpp"
#include "user.hpp"
#include "trie.hpp"
#include "hashtable.hpp"
#include "tags.hpp"

using namespace std;
using namespace std::chrono;

#define N_PLAYERS 18944
// Nearest prime of N_PLAYERS / 5
#define PLAYERS_SIZE 3793

#define N_USERS 138493
// Nearest prime of N_USERS / 3
#define USERS_SIZE 46171

#define N_TAGS 963
// Closest prime to N_TAGS / 5
#define TAG_SIZE 193

#define TIMEUNIT milliseconds

int main() {
  // Get starting timepoint
  auto startTime = high_resolution_clock::now();
  TIMEUNIT totalTime = TIMEUNIT(0);

  ifstream playersF("players.csv");
  aria::csv::CsvParser playersParser(playersF);

  ifstream tagsF("tags.csv");
  aria::csv::CsvParser tagsParser(tagsF);

  ifstream ratingsF("rating.csv");
  aria::csv::CsvParser ratingsParser(ratingsF);

  TrieNode* trieHead = new TrieNode();
  
  HashTable<Player> playersList(PLAYERS_SIZE);

  HashTable<User> reviewsList(USERS_SIZE);

  HashTable<TagTuple> tagTable(TAG_SIZE);
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
  tagsParser.next_field();
  tagsParser.next_field();
  tagsParser.next_field();
  tagsParser.next_field();

  for (auto& playersRow : playersParser) {
    // sofifaID, name, positions
    int idx = stoi(playersRow[0]);
    Player player = Player{idx, playersRow[0], playersRow[1], 0, 0};

    // Insert into hashtable
    playersList.insertElement(player, idx);
    // Insert into Trie
    trieHead->insertPlayer(playersRow[1], idx);
  }
  auto endTime = high_resolution_clock::now();

  auto duration = duration_cast<TIMEUNIT>(endTime - startTime);
  totalTime += duration;
  cout << "Finished adding players in " << duration.count() << "ms" << endl;
  startTime = high_resolution_clock::now();

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
  endTime = high_resolution_clock::now();

  duration = duration_cast<TIMEUNIT>(endTime - startTime);
  totalTime += duration;
  cout << "Finished adding reviews in " << duration.count() << "ms" << endl;
  startTime = high_resolution_clock::now();

  for (auto& tagRow : tagsParser) {
    int tagUserID = stoi(tagRow[0]);
    int tagSofifaID = stoi(tagRow[1]);
    string tag = tagRow[2];
    unsigned int hashedID = hashTag(tag, TAG_SIZE);  

    TagTuple* tagTableEntry = tagTable.get(tag, hashedID);
    if (tagTableEntry == nullptr) {
      TagTuple tagTuple = TagTuple{tag, new set<int>};
      tagTableEntry = tagTable.insertElement(tagTuple, hashTag(tag, TAG_SIZE));
    }
    tagTableEntry->playerIDs->insert(tagSofifaID);
  }

  endTime = high_resolution_clock::now();

  duration = duration_cast<TIMEUNIT>(endTime - startTime);
  totalTime += duration;
  cout << "Finished adding tags " << duration.count() << "ms" << endl;
  cout << "Finished everything in " << totalTime.count() << "ms" << endl;
  

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

  cout << "---------------------------" << endl;


  set<int> tagIntersection;

  vector<string> taglist = {"Chinese Super League", "Dribbler"};

  TagTuple foundTag = *(tagTable.get(taglist[0], hashTag(taglist[0], TAG_SIZE)));

  for (int i = 1; i < taglist.size(); ++i) {
    
  }

  for (int const& id : *(foundTag.playerIDs)) {
    std::cout << playersList.get(id)->sofifaID << endl;
  }
  return 0;

}