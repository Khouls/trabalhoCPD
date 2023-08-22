#ifndef HashTable_Defined
#define HashTable_Defined

#include <iostream>
#include <string>
#include <vector>

#include "player.hpp"

using namespace std;

template <typename T>
class HashTable {
    public :
        vector<vector<T>> elements;
        HashTable(int size);
        T* insertElement(T elem, int elemID);
        T* get(int elemID);
        int topPlayers(int topn, std::string pos);
        TagTuple* get(string tag, int hashedID);
};

template <typename T>
HashTable<T>::HashTable(int size) {
    elements.resize(size);
}

template <typename T>
T* HashTable<T>::insertElement(T elem, int elemID) {
    int insertionIndex = elemID % elements.size();

    elements[insertionIndex].push_back(elem);

    return (T*)&elements[insertionIndex].back();
}

template <typename T>
T* HashTable<T>::get(int elemID) {
    int searchIndex = elemID % elements.size();

    for (int i = 0; i < elements[searchIndex].size(); ++i) {
        if (elements[searchIndex][i] == elemID){
            return (T*)&elements[searchIndex][i];
        }
    }

    return nullptr;
}

template <>
TagTuple* HashTable<TagTuple>::get(string tag, int hashedID) {
    for (int i = 0; i < elements[hashedID].size(); ++i) {
        if (elements[hashedID][i].tag.compare(tag) == 0){
            return (TagTuple*)&elements[hashedID][i];
        }
    }
    return nullptr;
}



// template <typename T>
// int HashTable<T>::topPlayers(int topn) {

//     re
// }


set<std::string> posicoesVec(std::string posicoes) {
    set<std::string> posVec;

    string tempString = "";
    for (auto c : posicoes) 
    {
        if (c == ',')
        {
            
            posVec.insert(tempString);
            tempString = "";
        }
        else {
          if (c != ' '){
            tempString = tempString + c;  
          }
            
        }
    }
    
    posVec.insert(tempString);
    return posVec;
}


template <typename T>
int HashTable<T>::topPlayers(int topn, std::string pos) {
  cout << "Top " << topn << " jogadores da posicao " << pos << endl;


  auto fun = [](Player p1, Player p2)
  {
    return (p1.totalRating / p1.ratingCount) > (p2.totalRating / p2.ratingCount);
  };

  std::set<Player, decltype(fun)> topList(fun);
  float pscore;
  float prevbest = 0;

  for (int pindex = 0; pindex < elements.size(); pindex++)
  {
    for (int sindex = 0; sindex < elements[pindex].size(); sindex++)
    {
      if (elements[pindex][sindex].ratingCount >= 1000 && elements[pindex][sindex].positions.find(pos) != elements[pindex][sindex].positions.end())
      {
         pscore = elements[pindex][sindex].totalRating / elements[pindex][sindex].ratingCount;

         topList.insert(elements[pindex][sindex]);

      }
    }
  }
  
  typename set<Player, decltype(fun)>::iterator iterator;
  int i = 0;
  for(iterator = topList.begin(); i < topn && iterator != topList.end(); iterator++){
     cout << iterator->name << " -> " << iterator->totalRating / iterator->ratingCount << endl;
     i++;
  }


  return 4;
}

#endif