#ifndef CYLINDER_H
#define CYLINDER_H

#include "vec3.h"
#include "color.h"

class Cylinder {
public:
    Cylinder() {}

    Cylinder(const point3& center, const double radius, const double height, const double Kd, const color& objColor) : center(center), radius(radius), height(height), Kd(Kd), objectColor(objColor) {}

    const point3& getCenter() const  { return center; }
    const double getRadius() const { return radius; }
    const double getHeight() const { return height; }
    const double getKd() const { return Kd; }
    const color& getObjectColor() const  { return objectColor; }

    double hit_cylinder(const ray& r) {
        vec3 oc = center - r.origin();
        double a = dot(r.direction(), r.direction());
        double b = 2 * dot(r.direction(), oc);
        double c = dot(oc, oc) - radius * radius;
        double discriminant = b * b - 4 * a * c;
        if (discriminant >= 0) {
            double t1 = (-b - sqrt(discriminant)) / (2.0 * a);
            double t2 = (-b + sqrt(discriminant)) / (2.0 * a);
            if (t1 > 0 && t1 < t2) {
                point3 hitPoint = r.at(t1);
                if (hitPoint[1] >= center[1] - height / 2 && hitPoint[1] <= center[1] + height / 2) {
                    return t1;
                }
            }
            if (t2 > 0) {
                point3 hitPoint = r.at(t2);
                if (hitPoint[1] >= center[1] - height / 2 && hitPoint[1] <= center[1] + height / 2) {
                    return t2;
                }
            }
        }
        return -1;
    }

private:
    point3 center;
    double radius;
    double height;
    color objectColor;
    double Kd;
};

#endif