#ifndef NOISE_H
#define NOISE_H

// STD Library
#include <random>
#include <memory>

// Openvdb
#include <openvdb/openvdb.h>
#include <openvdb/tools/Interpolation.h>

// User
#include "Volume.h"

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

    // -------------
    // ValueNoise2D
    // -------------
    // Generate a 2D grid that holds noise values
    // Can be evaluated to return a noise value with a given 2D vector
    class ValueNoise2D {
        public:

            using GridType = openvdb::FloatGrid;
        
            ValueNoise2D(unsigned int seed, openvdb::CoordBBox bbox);
            ~ValueNoise2D() = default;

            float eval(const openvdb::Vec2s& pos) const;

        private:
            GridType::Ptr _grid;
            unsigned int _bitMask;
            openvdb::tools::GridSampler<GridType, openvdb::tools::BoxSampler> _sampler;

    };

    //-----------------------------------------------------------------------------

    // -------------
    // ValueNoise3D
    // -------------
    // Generate a 3D grid that holds noise values
    // Can be evaluated to return a noise value with a given 3D vector
    class ValueNoise3D {
        public:

            using GridType = openvdb::FloatGrid;
        
            ValueNoise3D(unsigned int seed, openvdb::CoordBBox bbox);
            ~ValueNoise3D() = default;

            float eval(const openvdb::Vec3s& pos) const;

        private:
            GridType::Ptr _grid;
            unsigned int _bitMask;
            openvdb::tools::GridSampler<GridType, openvdb::tools::PointSampler> _sampler;

    };

    //-----------------------------------------------------------------------------
    
    // Noise Field
    // Convert noise grid into a NoiseField to work with other fields
    class NoiseField : public Volume<float> {

        public:

            using typename Volume<float>::volumeDataType;

            NoiseField(const ValueNoise3D& N);
            ~NoiseField() = default;

            const volumeDataType eval(const Vector& p) const override;

        private:
            ValueNoise3D _noise;
    };
    
    //-----------------------------------------------------------------------------
    
    // -------------
    // Fractal Sums
    // -------------
    // Discrete Fractal sum with integer layers
    float fsn(const ValueNoise3D& noise, 
              float roughness, 
              float gain, 
              float frequency, 
              float lacunarity,
              const openvdb::Vec3s& pos, 
              const openvdb::Vec3s& translation,
              int layers);
    
    // Continuous Fractal sum with an added fractional portion
    float fsn(const ValueNoise3D& noise, 
              float roughness, 
              float gain, 
              float frequency, 
              float lacunarity,
              const openvdb::Vec3s& pos, 
              const openvdb::Vec3s& translation,
              float layers);

    //-----------------------------------------------------------------------------
    
    // -----------------
    // Helper functions
    // -----------------
    VSP<float> noise(const ValueNoise3D& N);
    
    //-----------------------------------------------------------------------------

    // ------------------
    // Utility functions
    // ------------------
    // Functions to small general math tasks
    float lerp(float lo, float hi, float t);
    
    //-----------------------------------------------------------------------------

} // namespace lux

#endif // Noise.h