/*
    This code reads the binary file.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

// #include "node.h"
#include "strsep.h"
#include "binarySearch.h"
#include "Filevalues.h"

void ExitError(const char *miss, int errcode) {
    fprintf (stderr, "\nERROR: %s.\nStopping...\n\n", miss); exit(errcode);
}
Node* readBinary (){
    FILE *fin;
    unsigned long nnodes;
    unsigned long *allsuccessors;
    double *alldistancesuccessors;
    unsigned long ntotnsucc=0UL;

    Node *nodes;

    if ((fin = fopen("../output/binaryDataSpain.bin", "rb")) == NULL) {
        ExitError("the data file does not exist or cannot be opened", 11);
    }
        
    /* Global data −−− header */
    if( fread(&nnodes, sizeof(unsigned long), 1, fin) + fread(&ntotnsucc, sizeof(unsigned long), 1, fin) != 2 ){
        ExitError("when reading the header of the binary data file", 12);
    }
    printf("Number of nodes: %ld \n", nnodes);

    /* getting memory for all data */
    if((nodes = (Node *) malloc(nnodes*sizeof(Node))) == NULL){
        ExitError("when allocating memory for the nodes vector", 13);
    }
    printf("Number of total successors: %ld \n", ntotnsucc);

    if( (allsuccessors = (unsigned long *) malloc(ntotnsucc*sizeof(unsigned long))) == NULL) {
        ExitError("when allocating memory for the edges vector", 15);
    }
    printf("Size of all successors: %u \n", sizeof(allsuccessors));

    if( (alldistancesuccessors = (double *) malloc(ntotnsucc*sizeof(double))) == NULL) {
        ExitError("when allocating memory for the edges vector", 15);
    }
    printf("Size of all distance successors: %u \n", sizeof(allsuccessors));

    /* Reading all data from file */
    int nodesRead = fread(nodes, sizeof(Node), nnodes, fin);
    printf("Nodes read %d \n", nodesRead);
    if( nodesRead != nnodes ){
        ExitError("when reading nodes from the binary data file", 17);
    }

    if(fread(allsuccessors, sizeof(unsigned long), ntotnsucc, fin) != ntotnsucc) {
        ExitError("when reading sucessors from the binary data file", 18);
    }

    /* Setting pointers to successors */
    int i;
    for(i=0; i <nnodes; i++) if(nodes[i].nSuccessors) {
        nodes[i].successors = allsuccessors; allsuccessors += nodes[i].nSuccessors;   
    }

    fclose(fin);
    return nodes;
}
