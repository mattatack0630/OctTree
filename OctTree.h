//
// Created by mathew on 4/19/18.
//
#pragma once

#include <vector>
#include "AABB.h"
#include "Member.h"

template<typename T>
class OctTree {

private:
    std::vector<Member<T> *> members;
    std::vector<OctTree *> subdivs;
    float min_size;
    int subdiv_threshold;
    AABB bounds;

    void subdivide();

    void push_members_down();

public:
    OctTree(AABB bounds, int threshold, float min);

    ~OctTree();

    bool attemptAddToMembers(Member<T> *member);

    bool insert(Member<T> *member);

    bool insert(T &value, float x, float y, float z, float extend_x, float extend_y, float extend_z);

    std::vector<T> query(AABB range);

    std::vector<T> query(float x, float y, float z, float extend_x, float extend_y, float extend_z);

    void clear();

    void print(int level);
};


template<typename T>
OctTree<T>::OctTree(AABB bounds, int threshold, float min) :
        bounds(bounds),
        min_size(min),
        subdiv_threshold(threshold) {
}

template<typename T>
void OctTree<T>::subdivide() {
    if (subdivs.empty()) {

        std::cout << "subdividing Tree@" << this << std::endl;

        // right-top-front
        subdivs.push_back(new OctTree(AABB(
                bounds.x + (bounds.extend_x / 2), bounds.y + (bounds.extend_y / 2), bounds.z + (bounds.extend_z / 2),
                bounds.extend_x / 2, bounds.extend_y / 2, bounds.extend_z / 2),
                                      subdiv_threshold, min_size));
        // right-top-back
        subdivs.push_back(new OctTree(AABB(
                bounds.x + (bounds.extend_x / 2), bounds.y + (bounds.extend_y / 2), bounds.z - (bounds.extend_z / 2),
                bounds.extend_x / 2, bounds.extend_y / 2, bounds.extend_z / 2),
                                      subdiv_threshold, min_size));
        // right-bottom-front
        subdivs.push_back(new OctTree(AABB(
                bounds.x + (bounds.extend_x / 2), bounds.y - (bounds.extend_y / 2), bounds.z + (bounds.extend_z / 2),
                bounds.extend_x / 2, bounds.extend_y / 2, bounds.extend_z / 2),
                                      subdiv_threshold, min_size));
        // right-bottom-back
        subdivs.push_back(new OctTree(AABB(
                bounds.x + (bounds.extend_x / 2), bounds.y - (bounds.extend_y / 2), bounds.z - (bounds.extend_z / 2),
                bounds.extend_x / 2, bounds.extend_y / 2, bounds.extend_z / 2),
                                      subdiv_threshold, min_size));
        // left-top-front
        subdivs.push_back(new OctTree(AABB(
                bounds.x - (bounds.extend_x / 2), bounds.y + (bounds.extend_y / 2), bounds.z + (bounds.extend_z / 2),
                bounds.extend_x / 2, bounds.extend_y / 2, bounds.extend_z / 2),
                                      subdiv_threshold, min_size));
        // left-top-back
        subdivs.push_back(new OctTree(AABB(
                bounds.x - (bounds.extend_x / 2), bounds.y + (bounds.extend_y / 2), bounds.z - (bounds.extend_z / 2),
                bounds.extend_x / 2, bounds.extend_y / 2, bounds.extend_z / 2),
                                      subdiv_threshold, min_size));
        // left-bottom-front
        subdivs.push_back(new OctTree(AABB(
                bounds.x - (bounds.extend_x / 2), bounds.y - (bounds.extend_y / 2), bounds.z + (bounds.extend_z / 2),
                bounds.extend_x / 2, bounds.extend_y / 2, bounds.extend_z / 2),
                                      subdiv_threshold, min_size));
        // left-bottom-back
        subdivs.push_back(new OctTree(AABB(
                bounds.x - (bounds.extend_x / 2), bounds.y - (bounds.extend_y / 2), bounds.z - (bounds.extend_z / 2),
                bounds.extend_x / 2, bounds.extend_y / 2, bounds.extend_z / 2),
                                      subdiv_threshold, min_size));
    }
}


template<typename T>
void OctTree<T>::push_members_down() {
    std::vector<Member<T> *> members_copy = members;

    members.clear();

    for (Member<T> *member : members_copy)
        insert(member);
/*
    members_copy.clear();*/
}

template<typename T>
bool OctTree<T>::attemptAddToMembers(Member<T> *member) {
    if (bounds.encapsulates(member->bounds)) {
        members.push_back(member);
        std::cout << "(insertion) Member Bounds [" << member->bounds.x << "," << member->bounds.y << ","
                  << member->bounds.z << "," << member->bounds.extend_x << "," << member->bounds.extend_y << ","
                  << member->bounds.extend_z << "] ";
        std::cout << "are inside of Tree Bounds [" << bounds.x << "," << bounds.y << "," << bounds.z << ","
                  << bounds.extend_x << "," << bounds.extend_y << "," << bounds.extend_z << "]" << std::endl;

        return true;
    } else {
        std::cout << "(passover) Member Bounds [" << member->bounds.x << "," << member->bounds.y << ","
                  << member->bounds.z << "," << member->bounds.extend_x << "," << member->bounds.extend_y << ","
                  << member->bounds.extend_z << "] ";
        std::cout << "are NOT inside of Tree Bounds [" << bounds.x << "," << bounds.y << "," << bounds.z << ","
                  << bounds.extend_x << "," << bounds.extend_y << "," << bounds.extend_z << "]" << std::endl;

        return false;
    }


}

template<typename T>
bool OctTree<T>::insert(Member<T> *member) {
    bool found = false;
    bool is_at_min = bounds.extend_x <= min_size || bounds.extend_z <= min_size || bounds.extend_z <= min_size;

    if (members.size() >= subdiv_threshold && subdivs.empty() && !is_at_min) {
        subdivide();
        push_members_down();
    }

    for (OctTree *sub : subdivs) {
        found |= sub->insert(member);
    }

    if (!found) {
        found |= attemptAddToMembers(member);
    }

    return found;
}

template<typename T>
bool OctTree<T>::insert(T &value, float x, float y, float z, float extend_x, float extend_y, float extend_z) {
    Member<T> *member = new Member<T>(x, y, z, extend_x, extend_y, extend_z, value);
    return insert(member);
}

template<typename T>
std::vector<T> OctTree<T>::query(AABB range) {
    std::vector<T> values_in_range;

    if (bounds.intersects(range)) {
        for (Member<T> *member : members) {
            values_in_range.push_back(member->value);
        }
    }

    // CHECK THIS
    for (OctTree sub : subdivs) {
        std::vector<T> sub_values = (sub.query(range));
        values_in_range.insert(values_in_range.end(), sub_values.begin(), sub_values.end());
    }

    return values_in_range;
}

template<typename T>
std::vector<T> OctTree<T>::query(float x, float y, float z, float extend_x, float extend_y, float extend_z) {
    return query(AABB(x, y, z, extend_x, extend_y, extend_z));
}

template<typename T>
void OctTree<T>::clear() {
    for (int i = members.size() - 1; i >= 0; i--) {
        Member<T> *m = members[i];
        std::cout << "called" << std::endl;
        if (!m) delete m;
    }


    for (int i = subdivs.size() - 1; i >= 0; i--) {
        OctTree *sub = subdivs[i];

        if (!sub) delete sub;
    }

    members.clear();
    subdivs.clear();
}

template<typename T>
void OctTree<T>::print(int level) {

    for (int i = 0; i < level; i++)
        std::cout << "    ";

    std::cout << "Tree@" << this << "(" << bounds.x << "," << bounds.y << "," << bounds.z << "," << bounds.extend_x
              << ")" << std::endl;

    for (Member<T> *member : members) {
        for (int i = 0; i < level + 1; i++)
            std::cout << "    ";

        std::cout << " Member@" << member->value << "(" << member->bounds.x << "," << member->bounds.y << ","
                  << member->bounds.z << ")" << std::endl;
    }

    for (OctTree<T> *sub : subdivs) {
        sub->print(level + 1);
    }
}

template<typename T>
OctTree<T>::~OctTree() {

    clear();
}

