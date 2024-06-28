#include "../src/color.h"
#include "../src/ray.h"
#include "../src/vec3.h"

#include <iostream>



color ray_color(const ray& r) {
    vec3 unit_direction = unit_vector(r.direction());
    auto a = 0.5*(unit_direction.y() + 1.0);
    return (1.0-a)*color(1.0, 1.0, 1.0) + a*color(0.5, 0.7, 1.0);
}

int main() {

    // Image

    auto aspect_ratio = 16.0 / 9.0;
    int image_width = 400;

    // Calculate the image height, and ensure that it's at least 1.
    int image_height = int(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;

    // Render

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
    // i controls the columns and j controls the rows
    for(int j=0; j<image_height; j++) {
        std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
        for(int i=0; i< image_width; i++) {
            auto pixel_color = color(double(i)/(image_width - 1), double(j)/(image_height-1), 0);
            write_color(std::cout, pixel_color);
        }
    }
    std::clog << "\rDone.                                \n";

}