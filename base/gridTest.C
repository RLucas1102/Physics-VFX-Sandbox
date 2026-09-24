#include <openvdb/openvdb.h>

#include <iostream>
#include <memory.h>

#include "VolumeGrid.h"
#include "FieldInterface.h"

using namespace lux;

int main() {

    std::shared_ptr<VolumeGrid> myVGrid = std::make_shared<VolumeGrid>();

    myVGrid->init(openvdb::Coord(-10, -10, -10), openvdb::Coord(10, 10, 10), 1.0, -1000);

    VSP<float> c = constant(4.0f);

    myVGrid->stamp(c);

    // std::cout << myVGrid->triLerp(Vector(0,0,0)) << std::endl; // 4

    VSP<float> gf = std::make_shared<GridField>(myVGrid);

    std::cout << evaluate(gf, Vector(0,0,0)) << std::endl;

}