/*
    Function that finds a node from an ID, binary search is used 
    because of the already ordered structure being the fastest
    search algorithm for this case.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "node.h"

// Array decays into a pointer when passed, thats why we need the size beforehand
long binarySearch(Node node[], unsigned long search, unsigned long n) {
    long first, last, middle;

    first = 0;
    last = n - 1;
    middle = (first + last) / 2;

    while (first <= last) {
        if (node[middle].id < search){
            first = middle + 1;
        }
        else if (node[middle].id == search) {
            return middle;
        }
        else {
            last = middle - 1;
        }

        middle = (first + last) / 2;
    }
    if (first > last)
        return -1;

    return -1;
}