/*
    This code contains the distance between nodes considering two points in as sphere 
    and looking for the optimal geodesic.
*/

#include <math.h>

double distance(double latitude1, double longitude1, double latitude2, double longitude2){
    /*
        This function computes the distance between two nodes using the haversine formula.

        Inputs:
            latiutde1, longitude1: coordinates of one node(location)
            latiutde2, longitude2: coordinates of other node(location)ç
        
        Output:
            distance: distance between both nodes computed by the haversine formula.
    */
   
    double radius = 6371e3; //Earth radius
    double sigma1 = latitude1 * M_PI/180.0;
    double sigma2 = latitude2 * M_PI/180.0;
    double dsigma = (latitude2-latitude1) * M_PI/180.0;
    double dlambda = (longitude2-longitude1) * M_PI/180.0;

    double a = sin(dsigma/2.0)*sin(dsigma/2.0) + (cos(sigma1)*cos(sigma2)*sin(dlambda/2.0)*sin(dlambda/2.0));
    double dist = 2.0 * atan2(sqrt(a),sqrt(1.0-a)) * radius;
    return dist;
}