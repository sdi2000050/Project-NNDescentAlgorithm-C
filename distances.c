#include <math.h>
#include <stdlib.h>
#include "graph.h"

float euclidean_distance(point x, point y) {
    return x.norm + y.norm - 2*dot_product(x,y);
}

float manhattan_distance(point x, point y) {
    float s = 0.0;
    for(int i = 0; i < x.dim; i++) {
        s = s + fabs(x.coord[i] - y.coord[i]);
    }
    return s;
}

float chebyshev_distance(point x, point y) {
    float max = 0;
    for(int i = 0; i < x.dim; i++) {
        float a = fabs(x.coord[i] - y.coord[i]);
        if(a > max) {
            max = a;
        }
    }
    return max;
}