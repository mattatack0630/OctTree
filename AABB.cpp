//
// Created by mathew on 4/19/18.
//

#include "AABB.h"

AABB::AABB(float x, float y, float z, float extend_x, float extend_y, float extend_z) :
    x(x),
    y(y),
    z(z),
    extend_x(extend_x),
    extend_y(extend_y),
    extend_z(extend_z)
{

}

bool AABB::intersects(AABB &b) {

    AABB a = *this;

    return (a.x-a.extend_x <= b.x+b.extend_x && a.x+a.extend_x >= b.x-b.extend_x) &&
           (a.y-a.extend_y <= b.y+b.extend_y && a.y+a.extend_y >= b.y-b.extend_y) &&
           (a.z-a.extend_z <= b.z+b.extend_z && a.z+a.extend_z >= b.z-b.extend_z);
}

bool AABB::encapsulates(AABB b) {
    AABB a = *this;

    return (b.x-b.extend_x>=a.x-a.extend_x &&
            b.y-b.extend_y>=a.y-a.extend_y &&
            b.z-b.extend_z>=a.z-a.extend_z &&
            b.x+b.extend_x<=a.x+a.extend_x &&
            b.y+b.extend_y<=a.y+a.extend_y &&
            b.z+b.extend_z<=a.z+a.extend_z);
}

