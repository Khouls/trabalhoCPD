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

#endif