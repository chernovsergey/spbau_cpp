#include <iostream>
#include "mergesort.h"
#include <cassert>
#include <fstream>

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

    using std::fstream;
    fstream input_reader("input.txt", fstream::in);
    int  max = 10;
    int* array = new int[max];
    int  n = 0;
    while(input_reader >> array[n])
    {
        n += 1;
        if(n >= max)
        {
            // Create new array with doubled size;
            max *= 2;
            int* dynamic_arr = new int[max];
            for(int i = 0; i < n; ++i)
            {
                dynamic_arr[i] = array[i];
            }
            delete[] array;
            array = dynamic_arr;
        }
    }
    mergesort(array, n);

    using std::ofstream;
    ofstream output_writer("output.txt", ofstream::out);
    for(int i = 0; i < n; ++i)
    {
        output_writer << array[i] << " ";
    }
    delete[] array;
}
