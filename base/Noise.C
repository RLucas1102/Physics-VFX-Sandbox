#include "Noise.h"

using namespace lux;

// -------------
// ValueNoise1D 
// -------------
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

    // Make sure that the input max is a power of 2
    // The bitwise & operator will not work properly
    // later otherwise
    _bitMask = bbox.max().x() - 1; 

    // We initialize the sampler up top, but that is to make the compiler happy
    // No default constructor for the GridSampler
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

    return lerp(fx0, fx1, t);

}

//-----------------------------------------------------------------------------

// -------------
// ValueNoise2D 
// -------------
ValueNoise2D::ValueNoise2D(unsigned int seed, openvdb::CoordBBox bbox) :
    _grid(GridType::create()),
    _sampler(*_grid)
{
    
    _grid = GridType::create(0.0f);

    _grid->fill(bbox, 1.0f, true);    

    std::mt19937 gen(seed); 
    std::uniform_real_distribution<float> distrFloat; 
    auto randFloat = std::bind(distrFloat, gen); 

    for(auto iter = _grid->beginValueOn(); iter; ++iter)
    {
        iter.setValue(randFloat());
    }

    // Make sure that the input max is a power of 2
    // The bitwise & operator will not work properly
    // later otherwise
    _bitMask = bbox.max().x() - 1; 

    // We initialize the sampler up top, but that is to make the compiler happy
    // No default constructor for the GridSampler
    _sampler = openvdb::tools::GridSampler<GridType, openvdb::tools::BoxSampler>(*_grid);

}

float ValueNoise2D::eval(float x, float y)
{
    float tx = x - (float)std::floor(x);
    float ty = y - (float)std::floor(y);
    tx = tx * tx * (3 - 2 * tx); // Smoothstep
    ty = ty * ty * (3 - 2 * ty); // Smoothstep

    int xMin = std::floor(x);
    int xMax = std::ceil(x);
    int yMin = std::floor(y);
    int yMax = std::ceil(y);

    xMin = xMin & _bitMask;
    xMax = xMax & _bitMask;
    yMin = yMin & _bitMask;
    yMax = yMax & _bitMask;

    float fx0 = _sampler.isSample(openvdb::Vec3R(xMin, yMin, 0.0));
    float fx1 = _sampler.isSample(openvdb::Vec3R(xMax, yMin, 0.0));
    float fx2 = _sampler.isSample(openvdb::Vec3R(xMin, yMax, 0.0));
    float fx3 = _sampler.isSample(openvdb::Vec3R(xMax, yMax, 0.0));

    float a = lerp(fx0, fx1, tx);
    float b = lerp(fx2, fx3, tx);

    return lerp(a, b, ty);

}

//-----------------------------------------------------------------------------

// -----------------
// Helper functions
// -----------------

float lux::lerp(float lo, float hi, float t)
{
    return lo * (1 - t) + hi * t;
}

//-----------------------------------------------------------------------------