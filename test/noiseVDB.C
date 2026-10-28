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

    unsigned int seed = 2016;
    ValueNoise2D noise(seed, bbox);

    float roughness = 0.25;
    float gain = 0.25;
    float frequency = 5;
    float lacunarity = 2.14159265;
    openvdb::Vec2s xt(0,0);
    int layers = 4;

    for(int j = 0; j < height; j++) {
        for(int i = 0; i < width; i++) {
            openvdb::Vec2s pos((float)i/(float)width,(float)j/(float)height);
            float value = fsn(noise, roughness, gain, frequency, lacunarity, pos, xt, layers);
            std::vector<float> color(img.GetNc(), value);
            img.SetValue(i, j, color);
        }
    }

    img.Write("images/noiseWood.png");

}
