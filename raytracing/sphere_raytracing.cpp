#include "../src/color.h"
#include "../src/ray.h"
#include "../src/vec3.h"
#include "../src/utility_functions.hpp"
#include "../src/sphere.hpp"
#include "../src/plane.hpp"
#include "../src/cylinder.hpp"
#include "../src/box.hpp"


#include <iostream>
#include <stdlib.h>
const int spheres = 2;
const int planes = 5;
const int cylinders = 1;
const int boxes = 1;

Sphere sphereObjects[spheres];
Sphere lightObjects[1];

Cylinder cylinderObjects[cylinders];


Plane planeObjects[planes];

// vec3 random_vec3() {
//         return vec3(random_double(), random_double(), random_double());
//     }

// vec3 random_vec3(double min, double max) {
//     return vec3(random_double(min,max), random_double(min,max), random_double(min,max));
// }

// Checks ray intersection with all the objects in the world
void hit_objects(const ray& r) {
    double t = 9999.0;
    double temp_t;
    int hitIdx = 0;
    bool hit = false;
    hit_record.t = -1.0;
    // Ray Sphere intersection 
    // The sphere object has a function hit_sphere(ray r) function that takes in the ray and returns t
    for(int i=0; i<spheres; i++) {
        temp_t = sphereObjects[i].hit_sphere(r);
        if(temp_t >0 && temp_t < t) {
            t = temp_t;
            hitIdx = i;
            hit = true;
            hit_record.sphere = true;
        }
    }
    // Ray Plane intersection
    // The plane object has a function hit_sphere(ray r) function that takes in the ray and returns t
    for(int i=0; i<planes; i++) {
        temp_t = planeObjects[i].hit_plane(r);
        if(temp_t >0 && temp_t < t) {
            t = temp_t;
            hitIdx = i;
            hit = true;
            hit_record.sphere = false;
        }
    }



    if(hit){
        hit_record.t = t;
        hit_record.hitIdx = hitIdx; 
    }   
}

color ray_color(const ray& r, int depth) {
    if(depth == 0)
        return color(0,0,0);
    vec3  unit_direction = unit_vector(r.direction());
    // test for hit
    hit_objects(r);
    // hit record is a struct that contains three variables t, hitIdx, and a boolean sphere
    // if hit_record.sphere is True, then the nearest object is a sphere. 
    // hitIdx contains the index of the respective object in the sphere array or plane array.
    // If hit_record.sphere is True, the hitIdx is the index in the spheres array 
    // else it is the index in the planes array
    // If the ray does not hit any object, then hit_record.t is -1

    if(hit_record.t > 0.0) {
        if(hit_record.sphere) {
            vec3 N = unit_vector(r.at(hit_record.t) - sphereObjects[hit_record.hitIdx].getCenter()); // also we can use r.at(t)
            if(dot(N, r.direction()) < 0.0) {
            vec3 lightDirection = lightObjects[0].getCenter() - r.at(hit_record.t);//random_on_hemisphere(N);
            ray new_ray(r.at(hit_record.t), unit_vector(lightDirection));
            hit_objects(new_ray);
            vec3 h = unit_vector(-r.direction() + lightDirection);
            if(hit_record.t == -1.0) {// hit light
                return sphereObjects[hit_record.hitIdx].getObjectColor()* (sphereObjects[hit_record.hitIdx].getKa() +sphereObjects[hit_record.hitIdx].getKd() *  std::max(0.0, dot(N, lightDirection)) + sphereObjects[hit_record.hitIdx].getKs() * std::pow(std::max(0.0, dot(N, h)),50.0));

            }
        }
        }
        else{
            vec3 N = unit_vector(planeObjects[hit_record.hitIdx].getNormal());
            if(dot(N, r.direction()) < 0.0) {
                vec3 lightDirection = lightObjects[0].getCenter() - r.at(hit_record.t);//random_on_hemisphere(N);
                ray new_ray(r.at(hit_record.t) + 0.1*unit_vector(lightDirection), unit_vector(lightDirection));
                hit_objects(new_ray);
                if(hit_record.t == -1.0) {// hit light
                    return planeObjects[hit_record.hitIdx].getKd() * planeObjects[hit_record.hitIdx].getObjectColor() * std::max(0.0, dot(N, lightDirection));

                }
            }
            }

    }

        return color(0,0,0);


}





int main() {
    // Init hit_record
    hit_record.t = -1.0;

    // Image

    auto aspect_ratio = 16.0 / 9.0;
    int image_width = 700;

    // Calculate the image height, and ensure that it's at least 1.
    int image_height = int(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;

    // Camera
    float focal_length = 1.0;
    float viewport_height = 2.0;
    float viewport_width = viewport_height * (double(image_width) / image_height);
    point3 camera_center = point3(0,0,0);

    // Compute Vu and Vv
    vec3 viewport_u = vec3(viewport_width, 0, 0);
    vec3 viewport_v = vec3(0, -viewport_height, 0);
    
    // Compute du and dv
    vec3 pixel_delta_u = viewport_u / image_width;
    vec3 pixel_delta_v = viewport_v / image_height;

    // Calculate the location of upper left pixel
    point3 viewport_upper_left = camera_center - vec3(0,0,focal_length) - viewport_u / 2 - viewport_v /2;
    point3 pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

    // Add objects
    // Add a sphere object
    // Sphere sphere = Sphere(point3 center, double radius, double Kd, double Ks, double Ka, double Ka, color objectColor)
    //sphereObjects[0] = Sphere(point3(0.3,-0.1,-0.6), 0.2, 0.8, 0.9, 0.1, color(0.8f, 0.4f, 0.8f));
    sphereObjects[0] = Sphere(point3(0.4,0,-1), 0.3, 0.8, 0.9, 0.1, color(0.8f, 0.4f, 0.8f));
    
    //sphereObjects[0] = Sphere(point3(0, 0.0, -1), 0.3, 0.8, 0.9, 0.1, color(0.6f, 0.2f, 0.8f));
    sphereObjects[1] = Sphere(point3(0,-100.5,-1),  0.0, 0.4, 0.2, 0.1, color(0.4f,0.6f,0.6f));

    //cylinderObjects[0] = Cylinder(point3(-0.4, 0.0, -1), 0.5, 1.0, 0.6, color(0.3f, 0.7f, 0.9f));
    

    lightObjects[0] = Sphere(point3(0.5,0.5,1), 0.1, 1.0, color(1.0f,1.0f,1.0f));
    // Add a plane object
    // Plane plane = Plane(point3 center, vec3 Normal, vec3 Xmin, vec3 Xmax, double Kd, color objectColor);
    //bottom
    planeObjects[0] = Plane(point3(0,0,-1.5), unit_vector(vec3(0,1,0.8)), vec3(-2,-1,-1), vec3(2,1,1), 0.2, color(0.8,0.8,0.8));
    //top
    planeObjects[1] = Plane(point3(0,0,-1.5), unit_vector(vec3(0,-1,0.8)), vec3(-2,-1,-1), vec3(2,1,1), 0.2, color(0.8,0.8,0.8));
    //back
    planeObjects[2] = Plane(point3(0,0,-1.0), unit_vector(vec3(0,0,1.0)), vec3(-2,-1,-1), vec3(2,1,1), 0.2, color(0.8,0.8,0.8));
    //right
    planeObjects[3] = Plane(point3(0.2,0,-1.5), unit_vector(vec3(-1,0,1.0)), vec3(-1.5,-1,-1), vec3(1.5,1,1), 0.2, color(0.8,0.8,0.8));
    //left
    planeObjects[4] = Plane(point3(-0.2,0,-1.5), unit_vector(vec3(1,0,1.0)), vec3(-1.5,-1,-1), vec3(1.5,1,1), 0.2, color(0.8,0.8,0.8));

    

    // Render

    std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";
        for (int j = 0; j < image_height; j++) {
            std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
            color pixel_color;
            for (int i = 0; i < image_width; i++) {
                vec3 pixel_sample = pixel00_loc
                                + ((i) * pixel_delta_u)
                                + ((j) * pixel_delta_v);
                vec3 ray_direction = pixel_sample - camera_center;
                ray r(camera_center, ray_direction);
                pixel_color = ray_color(r, 50);
                // color pixel_color = ray_color(r);
            write_color(std::cout, pixel_color);
        }
    }

    std::clog << "\rDone.                 \n";
}