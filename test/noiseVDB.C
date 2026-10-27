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
    
    ImgProc img;
    int width = 512;
    int height = 512;
    img.clear(width, height, 3);

    CBBox bbox = openvdb::CoordBBox(c( 0,       0,        0.0f),
                                    c( width/2, height/2, 0.0f));        

    ValueNoise2D noise(2016, bbox);

    float frequency = 0.05;
    for(int j = 0; j < height; j++) {
        for(int i = 0; i < width; i++) {
            float value = noise.eval(i * frequency, j * frequency);
            std::vector<float> color(img.GetNc(), value);
            img.SetValue(i, j, color);
        }
    }

    img.Write("images/noise.png");

}
