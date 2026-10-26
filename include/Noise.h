#ifndef NOISE_H
#define NOISE_H

// STD Library
#include <random>

// Openvdb
#include <openvdb/openvdb.h>

namespace lux {

    // ----------------
    // Noise functions
    // ----------------
    // These functions create openvdb grids of different
    // types of noise that will be stored in VolumeGrids

    openvdb::FloatGrid::Ptr valueNoise(unsigned int seed);

    //-----------------------------------------------------------------------------

} // namespace lux

#endif // Noise.h