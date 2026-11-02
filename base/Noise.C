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

float ValueNoise2D::eval(const openvdb::Vec2s& pos) const
{
    float x = pos.x();
    float y = pos.y();
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

// -------------
// ValueNoise3D 
// -------------
ValueNoise3D::ValueNoise3D(unsigned int seed, openvdb::CoordBBox bbox) :
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

float ValueNoise3D::eval(const openvdb::Vec3s& pos) const
{
    // float x = pos.x();
    // float y = pos.y();
    // float z = pos.z();
    // float tx = x - (float)std::floor(x);
    // float ty = y - (float)std::floor(y);
    // float tz = z - (float)std::floor(z);
    // tx = tx * tx * (3 - 2 * tx); // Smoothstep
    // ty = ty * ty * (3 - 2 * ty);
    // tz = tz * tz * (3 - 2 * tz);

    // int xMin = std::floor(x);
    // int xMax = std::ceil(x);
    // int yMin = std::floor(y);
    // int yMax = std::ceil(y);
    // int zMin = std::floor(z);
    // int zMax = std::ceil(z);

    // xMin = xMin & _bitMask;
    // xMax = xMax & _bitMask;
    // yMin = yMin & _bitMask;
    // yMax = yMax & _bitMask;
    // zMin = zMin & _bitMask;
    // zMax = zMax & _bitMask;

    // float fx0 = _sampler.isSample(openvdb::Vec3R(xMin, yMin, zMin));
    // float fx1 = _sampler.isSample(openvdb::Vec3R(xMin, yMin, zMax));
    // float fx2 = _sampler.isSample(openvdb::Vec3R(xMin, yMax, zMin));
    // float fx3 = _sampler.isSample(openvdb::Vec3R(xMin, yMax, zMax));
    // float fx4 = _sampler.isSample(openvdb::Vec3R(xMax, yMin, zMin));
    // float fx5 = _sampler.isSample(openvdb::Vec3R(xMax, yMin, zMax));
    // float fx6 = _sampler.isSample(openvdb::Vec3R(xMax, yMax, zMin));
    // float fx7 = _sampler.isSample(openvdb::Vec3R(xMax, yMax, zMax));

    // float a = lerp(fx0, fx4, tx);
    // float b = lerp(fx2, fx6, tx);
    // float c = lerp(fx1, fx5, tx);
    // float d = lerp(fx3, fx7, tx);

    // float e = lerp(a, b, ty);
    // float f = lerp(c, d, ty);

    // return lerp(e, f, tz);

    return _sampler.isSample(pos);

}

//-----------------------------------------------------------------------------

// -------------
// Fractal Sums
// -------------
float lux::fsn(const ValueNoise3D& noise, 
               float roughness, 
               float gain, 
               float frequency, 
               float lacunarity, 
               const openvdb::Vec3s& pos,
               const openvdb::Vec3s& translation,
               int layers)
{
    double r1 = 1.0f - roughness;
    double r2 = 1.0f - std::pow(roughness, layers);
    double r3 = r1/r2;
    float r = roughness;
    float f = frequency;
    openvdb::Vec3s x = pos;
    
    double sum = 0;
    for(int n = 0; n < layers; n++) {
        sum += r * noise.eval(x * f);
        r = gain * r;
        f = lacunarity * f;
        x = x - translation;
    }

    sum = r3 * sum;
    return sum;
}

float lux::fsn(const ValueNoise3D& noise, 
               float roughness, 
               float gain, 
               float frequency, 
               float lacunarity, 
               const openvdb::Vec3s& pos,
               const openvdb::Vec3s& translation,
               float layers)
{
    double r1 = 1.0f - roughness;
    double r2 = 1.0f - std::pow(roughness, layers);
    double r3 = r1/r2;
    float r = roughness;
    float f = frequency;
    openvdb::Vec3s x = pos;
    
    double sum = 0;
    for(int n = 0; n < (int)std::floor(layers); n++) {
        sum += r * noise.eval(x * f);
        r = gain * r;
        f = lacunarity * f;
        x = x - translation;
    }

    float alpha = layers - std::floor(layers);
    sum += alpha * r * noise.eval(x * f);
    sum = r3 * sum;
    return sum;
}

//-----------------------------------------------------------------------------

// -----------
// Noise Field
// -----------
NoiseField::NoiseField(const ValueNoise3D& N) :
    _noise(N)
{
}

const NoiseField::volumeDataType NoiseField::eval(const Vector &p) const
{ 
    float roughness = 0.75;
    float gain = 0.75;
    float frequency = 3;
    float lacunarity = 2.14159265;
    openvdb::Vec3s pos(p[0], p[1], p[2]);
    openvdb::Vec3s xt(0,0,0);
    int layers = 4;

    float value = fsn(_noise, roughness, gain, frequency, lacunarity, pos, xt, layers);

    return value;

}

//-----------------------------------------------------------------------------

// -----------------
// Helper functions
// -----------------
VSP<float> lux::noise(const ValueNoise3D& N)
{
    return std::make_shared<NoiseField>(N);
}

//-----------------------------------------------------------------------------

// -----------------
// Utility functions
// -----------------

float lux::lerp(float lo, float hi, float t)
{
    return lo * (1 - t) + hi * t;
}

//-----------------------------------------------------------------------------