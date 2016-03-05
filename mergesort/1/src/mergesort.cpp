#include "mergesort.h"

void merge(int* array, int left, int mid, int right, int size){
    int *result = new int[size];
    int i = 0, j = 0;

    for(i = mid + 1; i > left; i--){
        result[i-1] = array[i-1];
    }

    for(j = mid; j < right; j++){
        result[right + mid - j] = array[j+1];
    }

    for(int k = left; k <= right; k++){
        if(result[j] < result[i])
            array[k] = result[j--];
        else
            array[k] = result[i++];
    }

    delete[] result;
}

void process_merge_sort(int* array, int left, int right){
    if(right <= left){
        return;
    }
    int mid = (left + right)/2;
    process_merge_sort(array, left, mid);
    process_merge_sort(array, mid + 1, right);
    merge(array, left, mid, right, right + 1);
}

void mergesort(int* array, size_t size){
    if(size == 0 || size == 1)
    {
        return;
    }
    process_merge_sort(array, 0, size - 1);
}