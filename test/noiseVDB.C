#include <openvdb/openvdb.h>
#include <openvdb/tools/Interpolation.h>
#include <iostream>

#include "Noise.h"

using namespace lux;

using c = openvdb::Coord;

int main()
{
    openvdb::initialize();
    
    auto bbox = openvdb::CoordBBox(c(-10.0f, -10.0f, 0.0f),
                                   c( 10.0f,  10.0f, 0.0f));        

    auto grid = valueNoise(2016, bbox);
    auto accessor = grid->getAccessor();


    // Print rows of stored samples.
    for (int y = bbox.getStart().y(); y < bbox.getEnd().y(); ++y) {
        for (int x = bbox.getStart().x(); x < bbox.getEnd().x(); ++x) {
            std::cout
                << accessor.getValue(openvdb::Coord(x, y, 0))
                << '\t';
        }
        std::cout << '\n';
    }

    // At z = 0, trilinear interpolation reduces to bilinear
    // interpolation between four neighboring XY samples.
    // openvdb::tools::GridSampler<
    //     openvdb::FloatGrid,
    //     openvdb::tools::BoxSampler
    // > sampler(*grid);

    // float value =
    //     sampler.isSample(openvdb::Vec3R(1.5, 2.5, 0.0));

    // std::cout << "Interpolated at (1.5, 2.5): "
    //           << value << '\n';


    // value =
    //     sampler.isSample(openvdb::Vec3R(1, 2, 0.0));

    // std::cout << "Interpolated at (1, 2): "
    //           << value << '\n';
}