
Authors: Oriol Moreno & Joan Pareras
Version: 1.0.1
Last modification: 12 December 2021
Code: A* algorithm
Use: minimum distance between two nodes in a graph.


# A-Star Algorithm

This algorithm computes using a given csv file, the minimum path between nodes
in a graph. In this case, the algorithm minimizes the distance path between two 
locations in the map, given the ids of each node, acording to the csv file.

It generates two files: 

    binaryDataSpain.bin: containing the same information that in the csv file. 
        This procedure permits a higher performance each time we compute the 
        minimum distance path between diferent nodes.
    
    path.csv: containing the latitude and longitude of each node in the map.

---------------------------------------------------------------------------------
---------------------------------------------------------------------------------

# Program structure 

res

|___spain.csv

src

|___csvtobinary.c

|___csvtobinary.exe

|___Astar_algorithm.c

|___Astar_algorithm.exe

|___binarySearch.h

|___distanceFunction.h

|___Filevalues.h

|___node.h

|___readBinary.h

|___strsep.h

|___structsAstar.h

|___writeBinary.h

output

|___binaryDataSpain.h

|___path.csv



---------------------------------------------------------------------------------
---------------------------------------------------------------------------------

# src folder:

---------------------------------------------------------------------------------

# main.c

The main.c algorithm reads the csv file and writes the data in binary following 
the "Nodes structure" in node.h. 

---------------------------------------------------------------------------------

# Astar_algorithm_node.c

The Astar_algorithm_node.c has the code that computes the minimum distance 
between two nodes, that must be given directly to the code. This algorithm returns
the csv file with latitude and longitude of each node that belongs to this path.

main() 
    Reads the binary file, executes the A* algoritm and returns a csv file with
    the latitude and longitude of each node.

AStar_algorithm()
    Explores the graph and returns the path with minimum distance between the
    given start node and end node.

heuristic_function()
    This function computes an heuristic weigth for the adjacent node, computing 
    the 'haversine' distance between the adjacent node and the end-node.

add_with_priority()
    This function adds node to the Priority Queue with priority.

extract_min()
    This function extracts the first element of the priority queue.

requeue()
    This function requeues elements.

---------------------------------------------------------------------------------

# binarySearch.h

Function that finds a node from an ID, binary search is used 
because of the already ordered structure being the fastest
search algorithm for this case.

---------------------------------------------------------------------------------

# distanceFunction.h

This code contains the distance between nodes considering two points in as sphere 
and looking for the optimal geodesic, using the Haversine formula.

---------------------------------------------------------------------------------

# Filevalues.h

Function that look for longest line in file, ad returs its lenght.

---------------------------------------------------------------------------------

# node.h

Struct of the node class and all its related functions.

---------------------------------------------------------------------------------

# readBinary.h

This code reads the binary file.

---------------------------------------------------------------------------------

# strsep.h

Function that slices strings from a concrete delimiter,
returns the first element and modifies the rest of the string.

---------------------------------------------------------------------------------

# structsAstar.h

Contains the structs that the algorithm Astar uses.

---------------------------------------------------------------------------------

# writeBinary.h

This code writes the binary file.

---------------------------------------------------------------------------------
---------------------------------------------------------------------------------

# output folder:

---------------------------------------------------------------------------------

# binaryDataSpain.bin

Has the graph data stored in binary.

---------------------------------------------------------------------------------

# path.csv

Contains the latitude and longitude of every node in the path.

---------------------------------------------------------------------------------
---------------------------------------------------------------------------------

# res

Contains the data of Spain graph in a csv file

---------------------------------------------------------------------------------
---------------------------------------------------------------------------------












    

