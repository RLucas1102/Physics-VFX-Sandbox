#include <openvdb/openvdb.h>

#include <iostream>
#include <vector>
#include <memory>

#include "Noise.h"
#include "ImgProc.h"

using namespace lux;
using namespace image;

using CBBox = openvdb::CoordBBox;
using c = openvdb::Coord;

int main()
{

    openvdb::initialize();
    
    CBBox bbox = openvdb::CoordBBox(c( 0,  0, 0.0f),
                                    c( 10, 0, 0.0f));        

    ValueNoise1D noise(2011, bbox);

    for(int i = 0; i < 200; i++) {
        float x = (float)i / (float)(200 - 1) * 10;
        float y = noise.eval(x);
        std::cout << "(" << x << "," << y << ")" << std::endl;
    }

}
