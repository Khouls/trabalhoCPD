#ifndef QSort_defined
#define QSort_defined

#include <vector>

template <typename T>
void quicksort(std::vector<T>* C, int i, int f) {
    int p; // indice do pivo
    if(f > i){
        p = partition_hoare(C, i, f);
        quicksort(C, i, p-1);
        quicksort(C, p+1, f);
    }
}

template <typename T>
int partition_hoare(std::vector<T>* C, int left, int right) {
    int i, j;
    auto chave = (*C)[left];
    i = left; j = right+1;


    while (1) {
        while ((*C)[++i] > chave) if (i == right) break;
        while (chave >= (*C)[--j]) if (j == left) break;
        if (i >=j) break;
        std::swap((*C)[i], (*C)[j]);
    }
    std::swap((*C)[left], (*C)[j]);
    return j;
}

#endif