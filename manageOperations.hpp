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
#define HELP_TEXT 5
#define TAGLIST_TEXT 6

vector<string> allCommands{"exit", "help", "player", "user", "tags", "top", "taglist"};

using namespace std;
struct Operation {
    int code;
    vector<string> params;
};

struct strDist {
  int dist;
  string str;
};

int levDistance(string a, string b, int max);
strDist levBest(string input, vector<string> strings);
void tagSearch(vector<string> taglist, HashTable<TagTuple>* tagTable, HashTable<Player>* playersTable, set<string> allTags);
void userSearch(int userID, HashTable<User>* reviewsTable, HashTable<Player>* playersTable);
void prefixSearch(string prefix, TrieNode* trieHead, HashTable<Player>* playersTable);
void topSearch(int n, string pos, HashTable<Player>* playersTable);
Operation parseInput(string input, set<string> allTags);

Operation parseInput(string input, set<string> allTags) {
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
        
    } else {
      operationString = input;

      if (operationString.compare("help") == 0) {
            opCode = HELP_TEXT;
            cout << "Available commands are: " << endl << "          player <name> -> search for player name or prefix" << endl << "          user <ID> -> search for user reviews, by ID" << endl << "          tags <tags> -> find players by their tags " << endl << "          top<n> <position> -> top N players for a position" << endl << "          taglist -> displays all known tags" << endl << "          help -> displays this help menu" << endl;

      } else if (input.compare("exit") == 0){
        opCode = EXIT_CODE;

      } else if (operationString.compare("taglist") == 0) {
            opCode = TAGLIST_TEXT;
            for (auto & tag : allTags) 
            {
              cout << tag << endl;
            }

      }
    }

    if(opCode == -1){
        strDist bestMatch = levBest(operationString, allCommands);
        if(bestMatch.dist == 0){
          if(bestMatch.str == "top"){
              cout << "top<n> <position>. Try 'top20 ST'" << endl;
          } else{
              cout << "Wrong formating. Might need second argument" << endl;
          }
          
        }
        else if(bestMatch.dist < 3){
          cout << "Operation not found. Did you mean " << bestMatch.str << "?"<< endl;
        }
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

void tagSearch(vector<string> taglist, HashTable<TagTuple>* tagTable, HashTable<Player>* playersTable, set<string> allTags) {
  vector<set<int>> foundTags;
  set<int> foundIDs;

  for (int i = 0; i < taglist.size(); ++i){
  
   vector<string> vectorizedTags(allTags.begin(), allTags.end());
   strDist bestMatch = levBest(taglist[i], vectorizedTags);
   
   if(bestMatch.dist != 0 && bestMatch.dist < 3){

    cout << "Tag " << taglist[i] << " not found. Did you mean " << bestMatch.str << "?" << endl;
   }

  }

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


// leverenstein distance
int levDistance(string a, string b, int max)
{
    if(max == 0){
      return 9999;
    }
   
    int lenA = a.length();
    int lenB = b.length();

    //cout << a << endl;
    //cout << b << endl;

    if (lenA == 0)
    {
        return lenB;
    }
    else if (lenB == 0)
    {
        return lenA;
    }
    
    char va = a.begin()[0];
    char vb = b.begin()[0];
    string sa = a.substr(1);
    string sb = b.substr(1);

    if(va == vb)
    {
        return levDistance(sa, sb, max);
    }

    return 1 + min(min(levDistance(sa, b, max - 1), levDistance(a, sb, max - 1)), levDistance(sa, sb, max - 1));
}

strDist levBest(string input, vector<string> strings){

  strDist bestMatch = {9999, "default"};
  int currentDist;

  for(int i = 0; i < strings.size(); i++){
    currentDist = levDistance(input, strings.begin()[i], 3);

    
    if (currentDist < bestMatch.dist){
      bestMatch.dist = currentDist;
      bestMatch.str = strings.begin()[i]; 
    }


  }

  return bestMatch;
}

#endif