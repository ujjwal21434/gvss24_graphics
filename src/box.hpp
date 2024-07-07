#ifndef BOX_H
#define BOX_H

#include "vec3.h"
#include "color.h"

class Box {
  public:
    Box() {}

    Box(const point3& minCorner, const point3& maxCorner, const double Kd, const color& objColor) 
        : minCorner(minCorner), maxCorner(maxCorner), Kd(Kd), objectColor(objColor) {}

    const point3& getMinCorner() const { return minCorner; }
    const point3& getMaxCorner() const { return maxCorner; }
    const double getKd() const { return Kd; }
    const color& getObjectColor() const { return objectColor; }
    
    double hit_box(const ray& r) {
        double tmin = (minCorner[0] - r.origin()[0]) / r.direction()[0];
        double tmax = (maxCorner[0] - r.origin()[0]) / r.direction()[0];

        if (tmin > tmax) std::swap(tmin, tmax);

        double tymin = (minCorner[1] - r.origin()[1]) / r.direction()[1];
        double tymax = (maxCorner[1] - r.origin()[1]) / r.direction()[1];

        if (tymin > tymax) std::swap(tymin, tymax);

        if ((tmin > tymax) || (tymin > tmax))
            return -1;

        if (tymin > tmin)
            tmin = tymin;

        if (tymax < tmax)
            tmax = tymax;

        double tzmin = (minCorner[2] - r.origin()[2]) / r.direction()[2];
        double tzmax = (maxCorner[2] - r.origin()[2]) / r.direction()[2];

        if (tzmin > tzmax) std::swap(tzmin, tzmax);

        if ((tmin > tzmax) || (tzmin > tmax))
            return -1;

        if (tzmin > tmin)
            tmin = tzmin;

        if (tzmax < tmax)
            tmax = tzmax;

        if (tmin > 0) return tmin;
        else return -1;
    }

  private:
    point3 minCorner;
    point3 maxCorner;
    color objectColor;
    double Kd;
};

#endif