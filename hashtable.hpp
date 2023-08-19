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
        void insertElement(T elem, int elemID);
        T* get(int elemID);
};

template <typename T>
HashTable<T>::HashTable(int size) {
    elements.resize(size);
}

template <typename T>
void HashTable<T>::insertElement(T elem, int elemID) {
    int insertionIndex = elemID % elements.size();

    elements[insertionIndex].push_back(elem);
}

template <typename T>
T* HashTable<T>::get(int elemID) {
    int searchIndex = elemID % elements.size();

    for (int i = 0; i < elements[searchIndex].size(); ++i) {
        if (elements[searchIndex][i] == elemID){
            return (T*)&elements[searchIndex][i];
        }
    }

    return  nullptr;
}

#endif