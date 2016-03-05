#include <iostream>
#include "mergesort.h"
#include <cassert>

int main()
{
    int* empty = new int[0];
    mergesort(empty, 0);
    delete[] empty;

    int* one = new int[1];
    one[0] = 255;
    mergesort(one, 1);
    assert(one[0] == 255);
    delete[] one;

    int* two = new int[2];
    two[0] = 3;
    two[1] = 2;
    mergesort(two, 2);
    assert(two[0] == 2);
    assert(two[1] == 3);
    delete[] two;

    int* three = new int[3];
    three[0] =  10;
    three[1] = -10;
    three[2] =  0;
    mergesort(two, 3);
    assert(three[0] == -10);
    assert(three[1] ==  0);
    assert(three[2] ==  10);
    delete[] three;
}
