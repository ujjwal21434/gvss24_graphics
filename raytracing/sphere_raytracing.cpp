#include "../src/color.h"
#include "../src/ray.h"
#include "../src/vec3.h"
#include "../src/utility_functions.hpp"
#include "../src/sphere.hpp"
#include "../src/plane.hpp"

#include <iostream>
#include <stdlib.h>

const int spheres = 6;
const int planes = 5;


Sphere sphereObjects[spheres];
Sphere lightObjects[1];

Plane planeObjects[planes];





struct HitRecord
{
    double t;
    int hitIdx;
    bool isSphere;
};

HitRecord local_hit_record;



// Function to reflect a vector v around normal n
vec3 reflect(const vec3 &v, const vec3 &n)
{
    return v - 2 * dot(v, n) * n;
}



void hit_objects(const ray &r)
{
    double t = 9999.0;
    double temp_t;
    int hitIdx = -1;
    bool hit = false;
    local_hit_record.t = -1.0;
    // Ray Sphere intersection
    for (int i = 0; i < spheres; i++)
    {
        temp_t = sphereObjects[i].hit_sphere(r);
        if (temp_t > 0 && temp_t < t)
        {
            t = temp_t;
            hitIdx = i;
            hit = true;
            local_hit_record.isSphere = true;
        }
    }
    // Ray Plane intersection
    for (int i = 0; i < planes; i++)
    {
        temp_t = planeObjects[i].hit_plane(r);
        if (temp_t > 0 && temp_t < t)
        {
            t = temp_t;
            hitIdx = i;
            hit = true;
            local_hit_record.isSphere = false;
        }
    }

    

    if (hit)
    {
        local_hit_record.t = t;
        local_hit_record.hitIdx = hitIdx;
    }
}


// Function to calculate reflectance using Schlick's approximation
double reflectance(double cosine, double ref_idx)
{
    double r0 = (1 - ref_idx) / (1 + ref_idx);
    r0 = r0 * r0;
    return r0 + (1 - r0) * pow((1 - cosine), 5);
}

color ray_color(const ray &r, int depth) {
    if (depth <= 0)
        return color(0, 0, 0);

    hit_objects(r);

    if (local_hit_record.t > 0.0) {
        vec3 hitPoint = r.at(local_hit_record.t);
        vec3 N, lightDirection, h;
        color local_color, reflected_color, refracted_color(0, 0, 0);
        ray reflected_ray, refracted_ray;

        if (local_hit_record.isSphere) {
            const Sphere &hitSphere = sphereObjects[local_hit_record.hitIdx];
            N = unit_vector(hitPoint - hitSphere.getCenter());
            lightDirection = lightObjects[0].getCenter() - hitPoint;
            h = unit_vector(-r.direction() + lightDirection);

            // Ambient lighting
            color ambient = hitSphere.getObjectColor() * 0.1;

            // Diffuse and specular lighting for sphere
            local_color = hitSphere.getObjectColor() *
                          (hitSphere.getKa() +
                           hitSphere.getKd() * std::max(0.0, dot(N, unit_vector(lightDirection))) +
                           hitSphere.getKs() * std::pow(std::max(0.0, dot(N, h)), 50.0)) +
                           ambient;

            reflected_ray = ray(hitPoint, reflect(unit_vector(r.direction()), N));
            reflected_color = ray_color(reflected_ray, depth - 1);

            //Handling refraction using Snell's Law
            if (hitSphere.getTransparency() > 0.0) {
                double eta = hitSphere.getRefractiveIndex();
                vec3 refract_dir;
                if (dot(r.direction(), N) > 0) { // Ray inside the sphere
                    N = -N;
                    eta = 1.0 / eta;
                }
                double cos_theta = fmin(dot(-unit_vector(r.direction()), N), 1.0);
                double sin_theta = sqrt(1.0 - cos_theta * cos_theta);
                if (eta * sin_theta > 1.0 || reflectance(cos_theta, eta) > drand48()) {
                    refract_dir = reflect(unit_vector(r.direction()), N);
                } else {
                    vec3 r_out_perp = eta * (unit_vector(r.direction()) + cos_theta * N);
                    vec3 r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.length_squared())) * N;
                    refract_dir = r_out_perp + r_out_parallel;
                }
                refracted_ray = ray(hitPoint, refract_dir);
                refracted_color = ray_color(refracted_ray, depth - 1);
            }

            return local_color * (1.0 - hitSphere.getReflectivity() - hitSphere.getTransparency()) +
                   reflected_color * hitSphere.getReflectivity() +
                   refracted_color * hitSphere.getTransparency();
        } else {
            const Plane &hitPlane = planeObjects[local_hit_record.hitIdx];
            N = unit_vector(hitPlane.getNormal());
            lightDirection = lightObjects[0].getCenter() - hitPoint;
            h = unit_vector(-r.direction() + lightDirection);

            // Ambient lighting
            color ambient = hitPlane.getObjectColor() * 0.1;

            // Diffuse and specular lighting for plane
            local_color = hitPlane.getKd() * hitPlane.getObjectColor() * std::max(0.0, dot(N, unit_vector(lightDirection))) +
                          hitPlane.getKs() * std::pow(std::max(0.0, dot(N, h)), 50.0) * color(1.0, 1.0, 1.0) +
                          ambient;

            reflected_ray = ray(hitPoint, reflect(unit_vector(r.direction()), N));
            reflected_color = ray_color(reflected_ray, depth - 1);

            return local_color * (1.0 - hitPlane.getReflectivity()) +
                   reflected_color * hitPlane.getReflectivity();
        }
    }

    return color(0, 0, 0); // Background color
}







int main()
{
    // Init hit_record
    local_hit_record.t = -1.0;

    // Image
    auto aspect_ratio = 16.0 / 9.0;
    int image_width = 1000;

    int image_height = int(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;

    // Camera
    float focal_length = 1.0;
    float viewport_height = 2.0;
    float viewport_width = viewport_height * aspect_ratio;
    point3 camera_center = point3(0.0, 0.0, 0.0);

    vec3 viewport_u = vec3(viewport_width, 0, 0);
    vec3 viewport_v = vec3(0, -viewport_height, 0);

    vec3 pixel_delta_u = viewport_u / image_width;
    vec3 pixel_delta_v = viewport_v / image_height;

    point3 viewport_upper_left = camera_center - vec3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;
    point3 pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

    // Add objects
    sphereObjects[0] = Sphere(point3(-0.5, 0.0, -1), 0.3, 0.8, 0.9, 0.1, 0.5, 0.0, 0.0, color(0.922f, 0.651f, 0.400f));
    sphereObjects[1] = Sphere(point3(0.3, 0.0, -1), 0.2, 0.8, 0.9, 0.1, 0.5, 0.0, 0.0, color(0.8f, 0.1f, 0.1f));
    sphereObjects[2] = Sphere(point3(0.58, -0.3, -1), 0.1, 0.8, 0.9, 0.1, 0.5, 0.0, 1.5, color(0.9f, 0.9f, 1.0f));
    sphereObjects[3] = Sphere(point3(0.1, 0.4, -1), 0.1, 0.8, 0.9, 0.1, 0.5, 1.0, 0.0, color(1.0f, 1.0f, 1.0f));
    sphereObjects[4] = Sphere(point3(-0.05, -0.2, -1), 0.15, 0.8, 0.9, 0.1, 0.5, 0.2, 0.0, color(0.5f, 0.9f, 0.8f));

    //stringObjects[0] = String(sphereObjects[0].getCenter(), point3(-0.3, -0.1, -1), 0.01, color(0.0, 0.0, 0.0), 0.5, 0.5, 0.5, 0.5);

    lightObjects[0] = Sphere(point3(0.5, 0.5, 1), 0.1, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, color(1.0f, 1.0f, 1.0f));

    // Add a plane object
    // Plane plane = Plane(point3 center, vec3 Normal, vec3 Xmin, vec3 Xmax, double Kd, color objectColor);
    // Add a plane object
    // bottom
    planeObjects[0] = Plane(point3(0, -0.5, -1), unit_vector(vec3(0, 1, 0.8)), vec3(-2, -1, -1), vec3(2, 1, 1), 0.5, 0.8, 0.0, 0.0, color(0.9, 0.7, 0.3)); // Wooden floor
    // top
    planeObjects[1] = Plane(point3(0, 0.5, -1), unit_vector(vec3(0, -1, 0.8)), vec3(-2, -1, -1), vec3(2, 1, 1), 2.5, 0.8, 0.0, 0.0, color(0.9, 0.9, 0.6)); // Wooden ceiling
    // back
    planeObjects[2] = Plane(point3(0, 0, -1.0), unit_vector(vec3(0, 0, 1.0)), vec3(-2, -1, -1), vec3(2, 1, 1), 0.2, 0.5, 0.0, 0.0, color(0.9, 0.9, 0.6)); // Soft blue wall
    // right
    planeObjects[3] = Plane(point3(0.3, 0, -1.5), unit_vector(vec3(-1, 0, 1.0)), vec3(-1.5, -1, -1), vec3(1.5, 1, 1), 0.4, 0.5, 0.0, 1.0, color(0.8, 0.6, 0.9)); // Pinkish wall
    // left
    planeObjects[4] = Plane(point3(-0.3, 0, -1.5), unit_vector(vec3(1, 0, 1.0)), vec3(-1.5, -1, -1), vec3(1.5, 1, 1), 0.4, 0.5, 0.0, 1.0, color(0.9, 0.8, 0.6)); // Yellowish wall


    // Render
    std::cout << "P3\n"
              << image_width << " " << image_height << "\n255\n";
    for (int j = 0; j < image_height; j++)
    {
        std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
        for (int i = 0; i < image_width; i++)
        {
            vec3 pixel_sample = pixel00_loc + ((i)*pixel_delta_u) + ((j)*pixel_delta_v);
            vec3 ray_direction = pixel_sample - camera_center;
            ray r(camera_center, ray_direction);
            color pixel_color = ray_color(r, 50);
            write_color(std::cout, pixel_color);
        }
    }
    std::clog << "\rDone.                 \n";
}