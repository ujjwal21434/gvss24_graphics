#ifndef CONE_H
#define CONE_H

#include "vec3.h"
#include "color.h"

class Cone {
public:
    Cone() {}

    Cone(const point3& apex, const double height, const double radius, const double Kd, const double Ks, const double Ka, const color& objColor)
        : apex(apex), height(height), radius(radius), Kd(Kd), Ks(Ks), Ka(Ka), objectColor(objColor) {
        slope = radius / height;
        slope_squared = slope * slope;
    }

    const point3& getApex() const { return apex; }
    const double getHeight() const { return height; }
    const double getRadius() const { return radius; }
    const double getKd() const { return Kd; }
    const double getKs() const { return Ks; }
    const double getKa() const { return Ka; }
    const color& getObjectColor() const { return objectColor; }

    double hit_cone(const ray& r) {
        vec3 co = r.origin() - apex;
        vec3 dir = r.direction();

        double a = dot(dir, dir) - (1 + slope_squared) * dir.y() * dir.y();
        double b = 2 * (dot(dir, co) - (1 + slope_squared) * dir.y() * co.y());
        double c = dot(co, co) - (1 + slope_squared) * co.y() * co.y();

        double discriminant = b * b - 4 * a * c;

        if (discriminant >= 0) {
            double t1 = (-b - sqrt(discriminant)) / (2.0 * a);
            double t2 = (-b + sqrt(discriminant)) / (2.0 * a);

            double t = (t1 < t2) ? t1 : t2;
            point3 hitPoint = r.at(t);

            if (hitPoint.y() >= apex.y() && hitPoint.y() <= apex.y() + height) {
                return t;
            }
        }
        return -1;
    }

private:
    point3 apex;
    double height;
    double radius;
    color objectColor;
    double Kd;
    double Ks;
    double Ka;
    double slope;
    double slope_squared;
};

#endif
