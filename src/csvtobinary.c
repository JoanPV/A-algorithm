/*
    This code converts the information from the csv file to a binary file.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h>

// #include "node.h"
#include "strsep.h"
#include "binarySearch.h"
#include "Filevalues.h"
#include "writeBinary.h"
#include "distanceFunction.h"

#define MAX_LINE_LENGTH 79859
#define NUM_NODES 23895681UL
// NUM_NODES_CAT 3472620
// NUM_NODES_ESP 23895681

#pragma warning(disable : 4996)

int main() {

    clock_t begin = clock();

    // Defining node array
    Node* nodes = malloc(sizeof(Node) * NUM_NODES);

    // Opening file
    FILE* fp = fopen("../res/spain.csv", "r");
    if (fp == NULL) {
        perror("Unable to open file!");
        exit(1);
    }

    char line[MAX_LINE_LENGTH];
    unsigned int lineCount = 0;

    // Format: relation|@id|@name|@place|@highway|@route|@ref|@oneway|@maxspeed|relation_type|membertype;@id;@role|...
    // Format: way|@id|@name|@place|@highway|@route|@ref|@oneway|@maxspeed|member nodes|...
    // Format: node|@id|@name|@place|@highway|@route|@ref|@oneway|@maxspeed|node_lat|node_lon
    int ind = 0;
    for (ind=0; ind < NUM_NODES; ind++){
        nodes[ind].nSuccessors = 0;
    }
    printf("nSuccesors allocated \n");
            

    // Reading the file
    char rowAux[MAX_LINE_LENGTH];
    while (fgets(line, MAX_LINE_LENGTH, fp)) {
        // Ignoring first 3 lines and possible comments
        if (line[0] == '#') continue;

        // Node
        else if (line[0] == 'n') {
            // We "reset" the row pointer since strsep fucking modifies it.
            char *row = rowAux;
            strcpy(row, line);

            // Ignores 'node' word
            strsep(&row, "|");

            nodes[lineCount].id = strtol(strsep(&row, "|"), NULL, 10);

            // name
            strsep(&row, "|");
            //nodes[lineCount].name = name;

            // Ignores the next fields before latitude, longitude
            for (int i = 0; i < 6; i++) {
                strsep(&row, "|");
            }

            nodes[lineCount].latitude = strtod(strsep(&row, "|"), NULL);
            nodes[lineCount].longitude = strtod(strsep(&row, "\n"), NULL);

            lineCount++;
        }

        // Way
        // Format: way|@id    |@name|@place|@highway    |@route|@ref|@oneway|@maxspeed|member nodes|...
        // way        |4079444|     |      |unclassified|      |    |       |         |1084302604  |1084302239|21638578|1084302590|21638579|1084302629|21638580|1084302358|703840257|21638582|21638712|21638713
        // way        |4079460|     |      |residential |      |    |oneway |         |1084302703  |1084302835|21638796|21638797|21638730|21638807|21638729|1084302219|1084302697

        else if (line[0] == 'w') {

            char *row = rowAux;
            strcpy(row, line);

            // Ignores the next fields before member nodes
            for (int i = 0; i < 7; i++) {
                strsep(&row, "|");
            }
            char *oneWay = strsep(&row, "|");

            strsep(&row, "|");

            long lastNeighbourPos = LONG_MAX;
            while (row != NULL) {
                unsigned long neighbour = strtol(strsep(&row, "|"), NULL, 10);
                long pos = binarySearch(nodes, neighbour, NUM_NODES);
                if (pos >= 0) {
                    if (lastNeighbourPos < NUM_NODES) {
                        if (nodes[lastNeighbourPos].nSuccessors == 0){
                            nodes[lastNeighbourPos].successors = malloc(sizeof(unsigned long) * 9);
                        }
                        nodes[lastNeighbourPos].successors[nodes[lastNeighbourPos].nSuccessors] = pos;
                        nodes[lastNeighbourPos].nSuccessors++;
                        if (strcmp(oneWay, "") == 0) {
                            if (nodes[pos].nSuccessors == 0){
                                nodes[pos].successors = malloc(sizeof(unsigned long) * 9);
                            }
                            nodes[pos].successors[nodes[pos].nSuccessors] = lastNeighbourPos;
                            nodes[pos].nSuccessors++;
                        }
                    }
                    lastNeighbourPos = pos;
                }
            }
            lineCount++;
        }

        // Stop reading if its not a node atm
        else if (line[0] == 'r') {
            break;
        }
    }

    printf("%d \n", writeBinary(nodes));

    fclose(fp);

    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Computation time: %f seconds", time_spent);

    return 0;
}