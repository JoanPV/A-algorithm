#pragma once
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

/*
    Struct of the node class and all its related functions.
*/
typedef struct {
    unsigned long id; // Node identification
    double latitude, longitude; // Node position
    unsigned short nSuccessors; // Number of node successors; i. e. length of successors. Is it necessary?
    unsigned long *successors;
} Node;


void printNode(Node node){
    printf("Node n: %lu \n", node.id);
    printf("----------------\n");
    printf("latitude: %f \n", node.latitude);
    printf("longitude: %f \n", node.longitude);
    printf("Number of successors: %d \n", node.nSuccessors);
    int i;
    printf("[ ");
    for (i = 0; i < node.nSuccessors; i++) {
        printf("(%ld) ", node.successors[i]);
    }
    printf("]\n");
    printf("----------------\n");
    printf("\n");
}