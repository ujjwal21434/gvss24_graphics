#include "../src/color.h"
#include "../src/vec3.h"
#include "../src/ray.h"
#include <iostream>

double hit_sphere(const ray& r, vec3 center, double radius) {
        vec3 oc = center - r.origin();
        double a = dot(r.direction(), r.direction());
        double b = - 2 * dot(r.direction(),oc);
        double c = dot(oc,oc) - radius * radius;
        double discriminant = b* b - 4 * a * c;
        if(discriminant>=0){
            return (-b-sqrt(discriminant)) / (2.0*a);
        }
        else
            return -1;
    }

color ray_color(const ray& r) {
    point3 center = point3(0,0,-1);
    double radius = 0.5;
    double t = hit_sphere(r, center,radius);
    point3 clight = point3(0.5,0.5,1);

    if(t >0) {
        point3 hitPt = r.at(t);
        vec3 lightDir = unit_vector(clight - hitPt);
        vec3 Normal = unit_vector(hitPt - center);
            return color(0.8,0.4,0.4) * 0.8 * std::max(0.0,dot(Normal, lightDir));
    }


    return color(0,0,0);
}

int main() {

    // Image

    auto aspect_ratio = 16.0 / 9.0;
    int image_width = 400;

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



    // Render

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
    // i controls the columns and j controls the rows
    for(int j=0; j<image_height; j++) {
        std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
        for(int i=0; i< image_width; i++) {
                 vec3 pixel_sample = pixel00_loc
                                + ((i) * pixel_delta_u)
                                + ((j) * pixel_delta_v);
                vec3 ray_direction = unit_vector(pixel_sample - camera_center); 
                ray r(camera_center, ray_direction);

                auto pixel_color = ray_color(r);
            write_color(std::cout, pixel_color);
        }
    }
    std::clog << "\rDone.                                \n";

}