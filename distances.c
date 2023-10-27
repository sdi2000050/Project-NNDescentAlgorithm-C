#include <math.h>
#include <stdlib.h>
#include "graphtypes.h"

float euclidean_distance(point x, point y) {
    float s = 0.0;
    for(int i = 0; i < x.dim; i++) {
        float a = x.coord[i] - y.coord[i];
        s = s + pow(a, 2);
    }
    return sqrt(s);
}

double manhattan_distance(point x, point y) {
    float s = 0.0;
    for (int i = 0; i < x.dim; i++) {
        s = s + abs(x.coord[i] - y.coord[i]);
    }
    return s;
}