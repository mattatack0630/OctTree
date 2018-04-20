//
// Created by mathew on 4/19/18.
//
#pragma once;

#include "AABB.h"

template<typename T> class Member{
public:
    T value;
    AABB bounds;

    Member(float x, float y, float z, float extend_x, float extend_y, float extend_z, T& value);
};

template<typename T>
Member<T>::Member(float x, float y, float z, float extend_x, float extend_y, float extend_z, T& value) :
value(value),
bounds(x, y, z, extend_x, extend_y, extend_z)
{

}
