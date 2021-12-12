/*
    This code compiles the Astar algorithm givin to it a binary file with graph
    information. It returns a csv file with the langitude and latitude of each 
    node for the optimal path between the given starting and ending nodes.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <limits.h>
#include <math.h>
#include <time.h>

#include "node.h"
#include "structsAstar.h"
#include "readBinary.h"
#include "distanceFunction.h"

#define NUM_NODES_ESP 23895681UL
// typedef enum { false, true } bool;

// Initialization of Priority Queue
typedef QueueElement * PriorityQueue;

// Defining prototypes
float heuristic_function(Node *node, unsigned  adjacent_node, unsigned  end_node);
bool add_with_priority(unsigned  node, PriorityQueue *Pq, AStarControlData * Q);
unsigned  extract_min(PriorityQueue *Pq);
void requeue(unsigned  node, PriorityQueue *Pq, AStarControlData * ASCD);
bool AStar_algorithm(Node *node, AStarPath *PathData, unsigned long MAX_NODES, unsigned long start_node, unsigned long end_node);

int main () {

    clock_t begin_time = clock();

    // Read the binary file and stored in graph struct.
    Node *nodes = readBinary();

    AStarPath *PathData = malloc(sizeof(AStarPath) * NUM_NODES_ESP);
    unsigned start = binarySearch(nodes, 240949599UL, NUM_NODES_ESP);
    unsigned end = binarySearch(nodes, 195977239UL, NUM_NODES_ESP);

    unsigned par = binarySearch(nodes, 710632992UL, NUM_NODES_ESP);
    printf("From node %u to node %u \n", start, end);
    int i;

    // Call Astar
    bool r = AStar_algorithm(nodes, PathData, NUM_NODES_ESP, start, end);

    register unsigned v = end, pv = PathData[v].parent, ppv; 
    PathData[end].parent = UINT_MAX;

    printf("Reversing \n");

    while(v != start) {
        ppv = PathData[pv].parent;
        PathData[pv].parent = v;
        v = pv;
        pv = ppv;
    }

    FILE *fip = fopen("../output/path.csv", "w");

    printf("Saving path on file path.csv \n");
    // printf("Optimal path found:\nNode name | Distance\n----------|---------\n");
    // printf(" %ld (%3.3u) | Source\n", nodes[start].id, start);
    for(v = PathData[start].parent ; v != UINT_MAX ; v = PathData[v].parent){
        // printf(" %ld (%3.3u) | %7.3f\n", nodes[v].id, v, PathData[v].g);
        fprintf(fip, "%f,%f\n", nodes[v].latitude, nodes[v].longitude);
    }
    printf("The final path distance is: %7.2f meters\n", PathData[end].g);

    fclose(fip);

    clock_t end_time = clock();
    double time_spent = (double)(end_time - begin_time) / CLOCKS_PER_SEC;
    printf("Computation time: %f seconds\n", time_spent);

    return 0;
}

bool AStar_algorithm(Node *nodes, AStarPath *PathData, unsigned long MAX_NODES, unsigned long start_node, unsigned long end_node){
    // Open empty priority queue
    PriorityQueue PQ = NULL;
    AStarControlData *ASCD;
    ASCD = (AStarControlData*)malloc(MAX_NODES*sizeof(AStarControlData));
    if (ASCD == NULL){
        printf("Error when allocating memory data to control vector.\n");
    }

    // Initialize g to infty
    int i;
    for (i = 0 ; i < MAX_NODES ; i++){
        PathData[i].g = FLT_MAX;
        ASCD[i].IsOpen = false;
    }
    
    // Start at 0 the starting node, parent at start infty
    PathData[start_node].g = 0.0;
    PathData[start_node].parent = UINT_MAX;
    ASCD[start_node].f = heuristic_function(nodes, start_node, end_node);

    // Open add_with_priority
    if (!add_with_priority(start_node, &PQ, ASCD)){
        return -1;
    }

    // while not Open.IsEmpty do 
    while (PQ != NULL) {

        // current ← Open.extract min(g, h)
        unsigned long current_node = extract_min(&PQ);

        // if (current is goal) then return g, parent
        if (current_node == end_node){
            free(ASCD);
            return true;
        }

        // for each adj ∈ current.neighbours do
        for (i = 0 ; i < nodes[current_node].nSuccessors ; i++){

            // adj new try gScore ← g[current] + ω(current, adj)
            unsigned long succesor_node = nodes[current_node].successors[i];
            double distanceSuccessor = distance(nodes[current_node].latitude, nodes[current_node].longitude, nodes[succesor_node].latitude, nodes[succesor_node].longitude);
            float g_succesor = PathData[current_node].g + distanceSuccessor;

            // if adj new try gScore < g[adj] then
            if (g_succesor < PathData[succesor_node].g){

                // parent[adj] ← current
                PathData[succesor_node].parent = current_node;

                // Computes h only if first time visited, else it computes as h = f - g
                if (PathData[succesor_node].g == FLT_MAX){
                    ASCD[succesor_node].f = g_succesor + heuristic_function(nodes, succesor_node, end_node);
                }

                else{
                    ASCD[succesor_node].f = g_succesor + ASCD[succesor_node].f - PathData[succesor_node].g;
                }

                // g[adj] ← adj new try gScore
                PathData[succesor_node].g = g_succesor;

                // if not Open.BelongsTo(adj) then Open.add with priority(adj, g, h)
                if (!ASCD[succesor_node].IsOpen){
                    if(!add_with_priority(succesor_node, &PQ, ASCD)) {
                        return -1; 
                        printf("%i",-1);
                    }
                }

                // else Open.requeue with priority(adj, g, h)
                else {
                    requeue(succesor_node, &PQ, ASCD);
                }
            }
        }
        ASCD[current_node].IsOpen = false;
    }

    return false;
}

float heuristic_function(Node *node, unsigned  adjacent_node, unsigned  end_node) {

    /*
        This function computes an heuristic weigth for the adjacent node, computing the 'haversine'
        distance between the adjacent node and the end-node.

        Input: 
            - Graph structure
            - Adjacent node_id
            - End_node node_id
        
        Output:
            - haversine distance

    */

    double dist = distance(node[adjacent_node].latitude, node[adjacent_node].longitude, node[end_node].latitude, node[end_node].longitude);

    return dist;
}

bool add_with_priority(unsigned node, PriorityQueue *Pq, AStarControlData * ASCD){
    /*
        This function adds node to the Priority Queue with priority.
    */

    // Reserving memory for new queue element
    register QueueElement * QElement;
    QueueElement *auxiliarelemet = (QueueElement*) malloc(sizeof(QueueElement));

    if(auxiliarelemet == NULL){
        return false;
    }

    // Saving node_id in auxiliar element.
    auxiliarelemet->node = node;

    // Readig the distance f in the node and saving as visited the node.
    float distance = ASCD[node].f;
    ASCD[node].IsOpen = true;

    // If Priority Queue is null, then save node first(starting node).
    // If distance of node is smaller than any other node must be first.
    if( *Pq == NULL || !(distance > ASCD[(*Pq)->node].f) ) {
        auxiliarelemet->seg = *Pq; 
        *Pq = auxiliarelemet;
        return true;
    }

    // Otherwise
    // Compute the largest QueueElement with QElement >= Pq such that the node distance is smaller than 
    // the distance of the insertion point of auxiliar value.
    for(QElement = *Pq ; (QElement->seg && ASCD[QElement->seg->node].f < distance) ; QElement = QElement->seg);
    auxiliarelemet->seg = QElement->seg; 
    QElement->seg = auxiliarelemet;
    
    return true;
}

unsigned extract_min(PriorityQueue *Pq) {
    /*
        This function extracts the first element of the priority queue.
    */
    unsigned  node;
    PriorityQueue first = *Pq;

    node = first->node;
    *Pq = (*Pq)->seg;
    free(first);

    return node; 
}

void requeue(unsigned  node, PriorityQueue *Pq, AStarControlData * ASCD){
     /*
        This function requeues elements.
    */

    register QueueElement * previousvalue;
    register QueueElement * auxiliar;

    // First element of the queue is already the first node
    if((*Pq)->node == node){ 
        return;
    }

    // Runs through the queue until it finds node. 
    for(previousvalue = *Pq; previousvalue->seg->node != node; previousvalue = previousvalue->seg);

    // Takes the node out of queue.
    auxiliar = previousvalue->seg;
    previousvalue->seg = auxiliar->seg;
    free(auxiliar);

    // Adds new value
    add_with_priority(node, Pq, ASCD); 
}