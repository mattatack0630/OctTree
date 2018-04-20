#include <iostream>
#include <vector>
#include "AABB.h"
#include "OctTree.h"

struct TestMem {
    float x, y, z;
    float d;
    int id;

    TestMem(float x, float y, float z, float d, int id) {
        this->x = x;
        this->y = y;
        this->z = z;
        this->d = d;
        this->id = id;
    }

    ~TestMem(){
        std::cout<<"Destroyed"<<std::endl;
    }
};

std::vector<TestMem> Test;

void subdivide(){
    for(int i = 0; i<10; i++)
        Test.push_back(TestMem(i,i,i,i,i));
}

int main() {

    OctTree<TestMem *> *octTree = new OctTree<TestMem *>(AABB(0, 0, 0, 100, 100, 100), 8, 1);

    std::vector<TestMem *> testArray;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            testArray.push_back(
                    new TestMem(rand() % (100 - (-100)) - 100, rand() % (100 - (-100)) - 100, 10, 1, random()));
        }
    }

    for (TestMem *test : testArray) {
        octTree->insert(test, test->x, test->y, test->z, test->d, test->d, test->d);
    }

    octTree->print(0);


    delete octTree;

    return 0;
}