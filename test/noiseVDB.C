#include <openvdb/openvdb.h>
#include <openvdb/tools/Interpolation.h>

#include <iostream>
#include <vector>

#include "Noise.h"
#include "ImgProc.h"

using namespace lux;
using namespace image;

using c = openvdb::Coord;

int main()
{

    openvdb::initialize();
    
    ImgProc img;
    int width  = 512;
    int height = 512;
    img.clear(width, height, 3);
    
    auto bbox = openvdb::CoordBBox(c(-width, -height, 0.0f),
                                   c( width,  height, 0.0f));        

    auto grid = valueNoise(2016, bbox);
    auto accessor = grid->getAccessor();

    // At z = 0, trilinear interpolation reduces to bilinear
    // interpolation between four neighboring XY samples.
    openvdb::tools::GridSampler<
        openvdb::FloatGrid,
        openvdb::tools::BoxSampler
    > sampler(*grid);

    unsigned bitMask = width - 1;

    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            int ii = i & bitMask;
            int jj = j & bitMask;
            
            float value = sampler.isSample(openvdb::Vec3R(ii, jj, 0.0));
            std::vector<float> color(img.GetNc(), value);

            img.SetValue(i, j, color);
        }
    }

    img.Write("images/noise.png");

}
