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

    int nFrames = 360;

    ImgProc img;
    int width = 512;
    int height = 512;
    img.clear(width, height, 3);

    CBBox bbox = openvdb::CoordBBox(c( 0,       0,        0.0f),
                                    c( width/2, height/2, 0.0f));        

    unsigned int seed = 2020;
    Noise<openvdb::Vec2s> myNoise = std::make_shared<ValueNoise<openvdb::Vec2s>>(seed, bbox);

    float roughness = 0.75;
    float gain = 0.75;
    float frequency = 3;
    float lacunarity = 2.14159265;
    openvdb::Vec2s xt(0,0);
    float layers = 1;

    for(int k = 0; k <= nFrames; k++) {

        for(int j = 0; j < height; j++) {
            #pragma omp parallel for
            for(int i = 0; i < width; i++) {
                openvdb::Vec2s pos((float)i/(float)width,(float)j/(float)height);
                float value = fsn(myNoise, roughness, gain, frequency, lacunarity, pos, xt, layers) * 10;
                value = value - (int)value;
                std::vector<float> color(img.GetNc(), value);
                img.SetValue(i, j, color);
            }
        }

        std::stringstream ss;
        ss << "images/noise." << std::setw(4) << std::setfill('0') << k << ".exr";
        std::string filename = ss.str();
        img.Write(filename);

        std::cout << "\n" << ss.str() << " complete!" << std::endl; 

        layers += (10.0f - 1.0f) / (float)nFrames;

    }

}
