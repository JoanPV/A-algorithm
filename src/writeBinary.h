/*
    This code writes the binary file.
*/

void ExitError(const char *miss, int errcode) {
    fprintf (stderr, "\nERROR: %s.\nStopping...\n\n", miss); exit(errcode);
}

int writeBinary(Node *nodes){
    printf("Writing binary \n");
    FILE *fin;
    unsigned long nnodes = 23895681UL;
    //unsigned long nnodes = 3472620UL;
    char directory[257] = "../output/binaryDataSpain.bin";
    
    /* Computing the total number of successors */
    unsigned long ntotnsucc=0UL;
    int i;
    for(i = 0 ; i < nnodes ; i++) {
        ntotnsucc += nodes[i].nSuccessors;
    }
    
    printf("Number of successors %ld \n", ntotnsucc);

    /* Setting the name of the binary file */    
    printf("Writing in file: %s \n", directory);
    if ((fin = fopen (directory, "wb")) == NULL){
        ExitError("the output binary data file cannot be opened", 31);
    }

    // /* Global data −−− header */
    if( fwrite(&nnodes, sizeof(unsigned long), 1, fin) + fwrite(&ntotnsucc, sizeof(unsigned long), 1, fin) != 2 ){
        ExitError("when initializing the output binary data file", 32);
    }

    // /* Writing all nodes */
    if( fwrite(nodes, sizeof(Node), nnodes, fin) != nnodes ){
       ExitError("when writing nodes to the output binary data file", 32);
    }

    // /* Writing sucessors in blocks */
    for(i = 0 ; i < nnodes ; i++) {
        if(nodes[i].nSuccessors) {
            if (fwrite(nodes[i].successors, sizeof(unsigned long), nodes[i].nSuccessors, fin) != nodes[i].nSuccessors ){
                ExitError("when writing edges to the output binary data file", 32);
            }
        }
    }
    
    fclose(fin);
}