#define NUM_SUCCESSORS 9
#define NUM_NODES 3472620

typedef struct { 
    float g; 
    unsigned  parent;
} AStarPath;

typedef struct QueueElementstruct { 
    unsigned node; 
    struct QueueElementstruct *seg; 
} QueueElement;

typedef struct { 
    float f; 
    bool IsOpen; 
} AStarControlData;