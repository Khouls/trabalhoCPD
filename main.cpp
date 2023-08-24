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
#include "manageOperations.hpp"

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

  TrieNode trieHead;

  HashTable<Player> playersTable(PLAYERS_SIZE);

  HashTable<User> reviewsTable(USERS_SIZE);

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

  for (auto &playersRow : playersParser)
  {
    // sofifaID, name, positions
    int idx = stoi(playersRow[0]);
    Player player = Player{idx, playersRow[0], playersRow[1], posicoesVec(playersRow[2]), 0, 0};

    // Insert into hashtable
    playersTable.insertElement(player, idx);
    // Insert into Trie
    trieHead.insertPlayer(playersRow[1], idx);
  }
  auto endTime = high_resolution_clock::now();

  auto duration = duration_cast<TIMEUNIT>(endTime - startTime);
  totalTime += duration;
  cout << "Finished adding players in " << duration.count() << "ms" << endl;
  startTime = high_resolution_clock::now();

  for (auto &ratingRow : ratingsParser)
  {
    int ratingUserID = stoi(ratingRow[0]);
    int ratingSofifaID = stoi(ratingRow[1]);
    float rating = stof(ratingRow[2]);

    UserReview review = UserReview{ratingSofifaID, rating};

    Player *player = playersTable.get(ratingSofifaID);
    player->totalRating += rating;
    player->ratingCount++;

    User *user = reviewsTable.get(ratingUserID);

    if (user == nullptr)
    {
      user = new User(ratingUserID);
      reviewsTable.insertElement(*user, ratingUserID);
    }

    user->addReview(review);
  }
  endTime = high_resolution_clock::now();

  duration = duration_cast<TIMEUNIT>(endTime - startTime);
  totalTime += duration;
  cout << "Finished adding reviews in " << duration.count() << "ms" << endl;
  startTime = high_resolution_clock::now();


  //set containing all known tags
  set<string> allTags;

  for (auto& tagRow : tagsParser) {
    int tagUserID = stoi(tagRow[0]);
    int tagSofifaID = stoi(tagRow[1]);
    string tag = tagRow[2];
    unsigned int hashedID = hashTag(tag, TAG_SIZE);
    allTags.insert(tag);  

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
  
  string input;

  bool continueOperations = true;

  while (continueOperations) {
    cout << "---------------------------" << endl;
    getline(cin, input);
    Operation op = parseInput(input, allTags);
    switch (op.code) {
    case PREFIX_SEARCH:
      prefixSearch(op.params[0], &trieHead, &playersTable);
      break;

    case USER_SEARCH:
      userSearch(stoi(op.params[0]), &reviewsTable, &playersTable);
      break;

    case TOP_SEARCH:
      topSearch(stoi(op.params[0]), op.params[1], &playersTable);
      break;

    case TAG_SEARCH:
      tagSearch(op.params, &tagTable, &playersTable, allTags);
      break;
    
    case EXIT_CODE:
      continueOperations = false;
      break;
    
    default:
      break;
    }
  }
  return 0;
}
