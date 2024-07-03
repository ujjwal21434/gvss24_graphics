#include "../src/color.h"
#include "../src/vec3.h"
#include "../src/utility_functions.hpp"
#include <iostream>
#include <stdlib.h>

int main() {
    // vectors/points
    // declaring a vector
    vec3 a, b, c;
    a = vec3(1.0,2.0,3.0);
    b = vec3(0.7,1.0,4.0);
    c = vec3(1.0,0.0,1.0);

    // vector vector arithmetic 
    // sum
    vec3 sum = a + b;
    // difference
    vec3 diff = a - b;
    // dot product
    double dot_product = dot(a,b);

    // computing a unit vector from a vector
    vec3 direction = unit_vector(sum);
    
    // declaring a point
    point3 center;
    center = point3(1,1,1); // vec3(1,1,1) also works

    // vector vector elemenwise product
    vec3 ab = a*b;
    std::cout << ab[0] << " " << ab[1] << " " << ab[2];


    // color

    return 0;
}