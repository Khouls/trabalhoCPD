#ifndef inputHandling_Defined
#define inputHandling_Defined

#include <iostream>
#include <string>
#include <vector>

#include "string.h"

using namespace std;

// l leverenstein distance
int levDistance(string a, string b)
{
    char va;
    char vb;

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
    
    va = a.begin()[0];
    vb = b.begin()[0];

    string sa = a.substr(1);
    string sb = b.substr(1);

    if(va == vb)
    {
        return levDistance(sa, sb);
    }

    return 1 + min(min(levDistance(sa, b), levDistance(a, sb)), levDistance(sa, sb));
}

#endif