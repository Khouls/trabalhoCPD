#ifndef Manage_Defined
#define Manage_Defined

#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <cstddef>

#include "parser.hpp"
#include "player.hpp"
#include "user.hpp"
#include "trie.hpp"
#include "hashtable.hpp"
#include "tags.hpp"


#define PREFIX_SEARCH 0
#define USER_SEARCH 1
#define TOP_SEARCH 2
#define TAG_SEARCH 3
#define EXIT_CODE 4

using namespace std;
struct Operation {
    int code;
    vector<string> params;
};


void tagSearch(vector<string> taglist, HashTable<TagTuple>* tagTable, HashTable<Player>* playersTable);
void userSearch(int userID, HashTable<User>* reviewsTable, HashTable<Player>* playersTable);
void prefixSearch(string prefix, TrieNode* trieHead, HashTable<Player>* playersTable);
void topSearch(int n, string pos, HashTable<Player>* playersTable);
Operation parseInput(string input);

Operation parseInput(string input) {
    size_t found = input.find(" ");
    string operationString;
    int opCode = -1;
    vector<string> params;

    if (found != string::npos) {
        operationString = input.substr(0,found);

        if (operationString.compare("player") == 0) {
            opCode = PREFIX_SEARCH;
            params.push_back(input.substr(found+1, input.length()));

        } else if (operationString.compare("user") == 0) {
            opCode = USER_SEARCH;
            params.push_back(input.substr(found+1, input.length()));

        } else if (operationString.compare("tags") == 0) {
            opCode = TAG_SEARCH;
            string tag = "";
            int i = found;
            bool opentag = false;
            for (int i = found; i < input.length(); ++i) {
                if (input[i] == '\'') {
                    if (opentag) {
                        opentag = false;
                        params.push_back(tag);
                        
                    } else {
                        tag = "";
                        opentag = true;
                    }
                } else {
                    tag += input[i];
                }
            }

        } else if (operationString.compare(0, 3, "top") == 0) {
            opCode = TOP_SEARCH;
            params.push_back(input.substr(3, found));
            params.push_back(input.substr(found+1, input.length()));
        }
        
    } else if (input.compare("exit") == 0){
        opCode = EXIT_CODE;
    } 

    return Operation{opCode, params};
}


void topSearch(int n, string pos, HashTable<Player>* playersTable) {
  playersTable->topPlayers(n, pos);
}

void prefixSearch(string prefix, TrieNode* trieHead, HashTable<Player>* playersTable) {
  vector<int> playersFound;
  trieHead->searchPrefix(prefix, &playersFound);

  for (int i = 0; i < playersFound.size(); ++i) {
    Player *player = playersTable->get(playersFound[i]);
    cout << player->sofifaID << " | ";
    cout << player->name << " | ";
    for (string pos : player->positions){
        cout << pos << " ";
    } 
    cout << "| Avg. Rating: " << player->totalRating / player->ratingCount << " | ";
    cout << "Count " << player->ratingCount << endl;
  }
}


void userSearch(int userID, HashTable<User>* reviewsTable, HashTable<Player>* playersTable) {
  User *user = reviewsTable->get(userID);
  vector<UserReview> reviewsFound;

  user->getReviews(&reviewsFound);

  for (int i = 0; i < reviewsFound.size(); ++i)
  {
    Player *player = playersTable->get(reviewsFound[i].playerID);
    cout << player->sofifaID << " | ";
    cout << player->name << " | ";
    cout << "Rating: " << reviewsFound[i].rating << " | ";
    cout << "Global. Rating: " << player->totalRating / player->ratingCount << " | ";
    cout << "Count " << player->ratingCount << " | ";
    cout << "Rating: " << reviewsFound[i].rating << endl;
  }

}

void tagSearch(vector<string> taglist, HashTable<TagTuple>* tagTable, HashTable<Player>* playersTable) {
  vector<set<int>> foundTags;
  set<int> foundIDs;

  for (int i = 0; i < taglist.size(); ++i) {
    TagTuple* tag = tagTable->get(taglist[i], hashTag(taglist[i], tagTable->elements.size()));
    if (tag == nullptr) {
        return;
    }
    foundTags.push_back(*tag->playerIDs);
  }

  for (int i = 0; i < foundTags.size(); ++i) {
    for (int const& id : foundTags[i]) {
      int j = 0;

      // Skip same set
      if (j == i) {
        j = foundTags.size();
      }

      while (j < foundTags.size() && foundTags[j].find(id) != foundTags[j].end()) {
        ++j;
      }

      if (j == foundTags.size()) {
        foundIDs.insert(id);
      }
    }
  }

  for (int id : foundIDs) {
    Player *player = playersTable->get(id);
    cout << player->sofifaID << " | ";
    cout << player->name << " | ";
    for (string pos : player->positions){
        cout << pos << " ";
    } 
    cout << "| Avg. Rating: " << player->totalRating / player->ratingCount << " | ";
    cout << "Count " << player->ratingCount << endl;
  }
}

#endif