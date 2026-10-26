#include <openvdb/openvdb.h>
#include <openvdb/tools/Interpolation.h>
#include <iostream>

#include "Noise.h"

using namespace lux;

int main()
{
    openvdb::initialize();

    auto grid = valueNoise(2016);
    auto accessor = grid->getAccessor();

    // Print rows of stored samples.
    for (int y = -10; y < 10; ++y) {
        for (int x = -10; x < 10; ++x) {
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