#ifndef NOISE_H
#define NOISE_H

// STD Library
#include <random>

// Openvdb
#include <openvdb/openvdb.h>
#include <openvdb/tools/Interpolation.h>

namespace lux {


    // -------------
    // ValueNoise1D
    // -------------
    // Generate a 1D grid that holds noise values
    // Can be evaluated to return a noise value with a given float
    class ValueNoise1D {
        public:

            using GridType = openvdb::FloatGrid;
        
            ValueNoise1D(unsigned int seed, openvdb::CoordBBox bbox);
            ~ValueNoise1D() = default;

            float eval(float x);

        private:
            GridType::Ptr _grid;
            unsigned int _bitMask;
            openvdb::tools::GridSampler<GridType, openvdb::tools::BoxSampler> _sampler;

    };

    //-----------------------------------------------------------------------------

    // ------------------
    // Utility functions
    // ------------------
    // Functions to small general math tasks
    float lerp1D(float lo, float hi, float t);

    //-----------------------------------------------------------------------------

} // namespace lux

#endif // Noise.h