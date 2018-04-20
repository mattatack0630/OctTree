//
// Created by mathew on 4/19/18.
//
#pragma once

class AABB {
public:
    float x, y, z;
    float extend_x, extend_y, extend_z;

    AABB(float x, float y, float z, float extend_x, float extend_y, float extend_z);
    bool intersects(AABB& other);

    bool encapsulates(AABB aabb);
};

