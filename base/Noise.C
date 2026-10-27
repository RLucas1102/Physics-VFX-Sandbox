#include "Noise.h"

using namespace lux;

// ----------------
// Noise functions
// ----------------
ValueNoise1D::ValueNoise1D(unsigned int seed, openvdb::CoordBBox bbox) :
    _grid(GridType::create()),
    _sampler(*_grid)
{
    
    _grid = GridType::create(0.0f);

    _grid->fill(bbox, 1.0f, true);    

    // std::mt19937 gen(seed); 
    // std::uniform_real_distribution<float> distrFloat; 
    // auto randFloat = std::bind(distrFloat, gen); 

    srand48(seed); 

    for(auto iter = _grid->beginValueOn(); iter; ++iter)
    {
        iter.setValue(drand48());
    }

    _bitMask = bbox.max().x() - 1; 

    _sampler = openvdb::tools::GridSampler<GridType, openvdb::tools::BoxSampler>(*_grid);

}

float ValueNoise1D::eval(float x)
{
    float xp = std::floor(x);
    float t = x - xp;
    t = t * t * (3 - 2 * t); // Smoothstep

    int xMin = std::floor(x);
    int xMax = std::ceil(x);
    
    xMin = xMin & _bitMask;
    xMax = xMax & _bitMask;

    float fx0 = _sampler.isSample(openvdb::Vec3R(xMin, 0.0, 0.0));
    float fx1 = _sampler.isSample(openvdb::Vec3R(xMax, 0.0, 0.0));

    return lerp1D(fx0, fx1, t);

}

//-----------------------------------------------------------------------------

// -----------------
// Helper functions
// -----------------

float lux::lerp1D(float lo, float hi, float t)
{
    return lo * (1 - t) + hi * t;
}

//-----------------------------------------------------------------------------