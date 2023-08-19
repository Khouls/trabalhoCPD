#ifndef Trie_Defined
#define Trie_Defined

#include <iostream>
#include <string>
#include <vector>

using namespace std;

#define TRIESIZE 32

class TrieNode {
  public :
    TrieNode* children[TRIESIZE] = {};
    int playerID;

    TrieNode();

    void insertPlayer(string playerName, int pID);

    void searchPrefix(string playerName, vector<int>* foundPlayers);

    void getAllPlayers(vector<int>* foundPlayers);
};

TrieNode::TrieNode() {
  // Initialize all the pointers as null
  //  " " is 0, "." is 1, "'" is 2, """ is 3, "-" is 4, A-Z is 3-32
  playerID = -1;
}

void TrieNode::insertPlayer(string playerName, int pID) {
  if (playerName.empty()) {
    playerID = pID;
  } else {
      int currentChar = toupper(playerName[0]);
      
      switch (currentChar) {
        case ' ':
          currentChar = 0;
          break;

        case '.':
          currentChar = 1;
          break;

        case 27: // '
          currentChar = 2;
          break;

        case '"':
          currentChar = 3;
          break;

        case '-':
          currentChar = 4;
          break;

        default:
          currentChar -= 'A' - 5;
      }
          
      // Check if there is already a trie in the current character
      if (children[currentChar] == nullptr){
        children[currentChar] = new TrieNode();
      }

      // Insert the rest of the player's name
      children[currentChar]->insertPlayer(playerName.erase(0,1), pID);
  }

}

void TrieNode::searchPrefix(string playerName, vector<int>* foundPlayers) {
  if (playerName.empty()) {
    this->getAllPlayers(foundPlayers);
  } else {
    int currentChar = toupper(playerName[0]);

      switch (currentChar) {
        case ' ':
          currentChar = 0;
          break;

        case '.':
          currentChar = 1;
          break;

        case 27: // '
          currentChar = 2;
          break;

        case '"':
          currentChar = 29;
          break;

        case '-':
          currentChar = 30;
          break;

        default:
          currentChar -= 'A' - 5;
      }

    if (children[currentChar] != nullptr) {
      children[currentChar]->searchPrefix(playerName.erase(0,1), foundPlayers);
    }
  }
}

void TrieNode::getAllPlayers(vector<int>* foundPlayers)  {
  if (playerID != -1) {
    foundPlayers->push_back(playerID);
  }

  for (int i = 0; i < TRIESIZE; ++i) {
    // Check if there is a continuation
    if (children[i] != nullptr) {
      children[i]->getAllPlayers(foundPlayers);
    }
  }
}

#endif