#include "Noise.h"

using namespace lux;

// ------------------
// Utility Functions
// ------------------
float lux::lerp(float lo, float hi, float t)
{
    return lo * (1 - t) + hi * t;
}

float lux::Value1D(float x,
                   unsigned int bitMask, 
                   PointSampler sampler)
{
    float xp = std::floor(x);
    float t = x - xp;
    t = t * t * (3 - 2 * t); // Smoothstep

    int xMin = std::floor(x);
    int xMax = std::ceil(x);
    
    xMin = xMin & bitMask;
    xMax = xMax & bitMask;

    float fx0 = sampler->isSample(openvdb::Vec3R(xMin, 0.0, 0.0));
    float fx1 = sampler->isSample(openvdb::Vec3R(xMax, 0.0, 0.0));

    return lerp(fx0, fx1, t);

}

float lux::Value2D(const openvdb::Vec2s& P,
                   unsigned int bitMask, 
                   PointSampler sampler)
{
    float x = P.x();
    float y = P.y();
    float tx = x - (float)std::floor(x);
    float ty = y - (float)std::floor(y);
    tx = tx * tx * (3 - 2 * tx); // Smoothstep
    ty = ty * ty * (3 - 2 * ty); // Smoothstep

    int xMin = std::floor(x);
    int xMax = std::ceil(x);
    int yMin = std::floor(y);
    int yMax = std::ceil(y);

    xMin = xMin & bitMask;
    xMax = xMax & bitMask;
    yMin = yMin & bitMask;
    yMax = yMax & bitMask;

    float fx0 = sampler->isSample(openvdb::Vec3R(xMin, yMin, 0.0));
    float fx1 = sampler->isSample(openvdb::Vec3R(xMax, yMin, 0.0));
    float fx2 = sampler->isSample(openvdb::Vec3R(xMin, yMax, 0.0));
    float fx3 = sampler->isSample(openvdb::Vec3R(xMax, yMax, 0.0));

    float a = lerp(fx0, fx1, tx);
    float b = lerp(fx2, fx3, tx);

    return lerp(a, b, ty);

}

float lux::Value3D(const openvdb::Vec3s& P,
                   unsigned int bitMask, 
                   PointSampler sampler)
{
    float x = P.x();
    float y = P.y();
    float z = P.z();
    float tx = x - (float)std::floor(x);
    float ty = y - (float)std::floor(y);
    float tz = z - (float)std::floor(z);
    tx = tx * tx * (3 - 2 * tx); // Smoothstep
    ty = ty * ty * (3 - 2 * ty);
    tz = tz * tz * (3 - 2 * tz);

    int xMin = std::floor(x);
    int xMax = std::ceil(x);
    int yMin = std::floor(y);
    int yMax = std::ceil(y);
    int zMin = std::floor(z);
    int zMax = std::ceil(z);

    xMin = xMin & bitMask;
    xMax = xMax & bitMask;
    yMin = yMin & bitMask;
    yMax = yMax & bitMask;
    zMin = zMin & bitMask;
    zMax = zMax & bitMask;

    float fx0 = sampler->isSample(openvdb::Vec3R(xMin, yMin, zMin));
    float fx1 = sampler->isSample(openvdb::Vec3R(xMin, yMin, zMax));
    float fx2 = sampler->isSample(openvdb::Vec3R(xMin, yMax, zMin));
    float fx3 = sampler->isSample(openvdb::Vec3R(xMin, yMax, zMax));
    float fx4 = sampler->isSample(openvdb::Vec3R(xMax, yMin, zMin));
    float fx5 = sampler->isSample(openvdb::Vec3R(xMax, yMin, zMax));
    float fx6 = sampler->isSample(openvdb::Vec3R(xMax, yMax, zMin));
    float fx7 = sampler->isSample(openvdb::Vec3R(xMax, yMax, zMax));
    
    float a = lerp(fx0, fx4, tx);
    float b = lerp(fx2, fx6, tx);
    float c = lerp(fx1, fx5, tx);
    float d = lerp(fx3, fx7, tx);

    float e = lerp(a, b, ty);
    float f = lerp(c, d, ty);

    return lerp(e, f, tz);

}


float lux::Value3D(const openvdb::Vec3s& P,
              const openvdb::FloatGrid::Ptr& grid)
{
    
    openvdb::Vec3d index = grid->worldToIndex(P);
    
    float v = openvdb::tools::BoxSampler::sample(grid->tree(), index);

    return v;
}


//-----------------------------------------------------------------------------

// -----------
// ValueNoise 
// -----------
template <typename T>
ValueNoise<T>::ValueNoise(unsigned int seed, openvdb::CoordBBox bbox)
{
    
    _grid = openvdb::FloatGrid::create(0.0f);

    std::mt19937 gen(seed); 
    std::uniform_real_distribution<float> distrFloat; 
    auto randFloat = std::bind(distrFloat, gen); 

    typename openvdb::FloatGrid::Accessor accessor = _grid->getAccessor();

    for ( auto iter = bbox.beginXYZ(); iter != bbox.endXYZ(); ++iter)
    {
        accessor.setValue(*iter, randFloat());
    }

    _grid->setTransform(openvdb::math::Transform::createLinearTransform(0.1));

    // Make sure that the input max is a power of 2
    // The bitwise & operator will not work properly
    // later otherwise
    _bitMask = bbox.max().x() - 1; 

    _sampler = std::make_shared<openvdb::tools::GridSampler<openvdb::FloatGrid, openvdb::tools::PointSampler>>(*_grid);

}

//-----------------------------------------------------------------------------

// -----------
// Noise Field
// -----------
NoiseField::NoiseField(const Noise<openvdb::Vec3s>& N) :
    _noise(N)
{
}

const NoiseField::volumeDataType NoiseField::eval(const Vector &p) const
{ 
    float roughness = 0.75;
    float gain = 0.75;
    float frequency = 10;
    float lacunarity = 2.14159265;
    openvdb::Vec3s pos(p[0], p[1], p[2]);
    openvdb::Vec3s xt(0,0,0);
    int layers = 1;

    return fsn(_noise, roughness, gain, frequency, lacunarity, pos, xt, layers);

}

//-----------------------------------------------------------------------------

// -------------
// Fractal Sums
// -------------
template<typename T>
float lux::fsn(const Noise<T>& noise, 
               float roughness, 
               float gain, 
               float frequency, 
               float lacunarity, 
               const T& pos,
               const T& translation,
               int layers)
{
    double r1 = 1.0f - roughness;
    double r2 = 1.0f - std::pow(roughness, layers);
    double r3 = r1/r2;
    float r = roughness;
    float f = frequency;
    T x = pos;
    
    double sum = 0;
    for(int n = 0; n < layers; n++) {
        sum += r * noise->eval(x * f);
        r = gain * r;
        f = lacunarity * f;
        x = x - translation;
    }

    sum = r3 * sum;
    return sum;
}

template <typename T>
float lux::fsn(const Noise<T>& noise, 
               float roughness, 
               float gain, 
               float frequency, 
               float lacunarity, 
               const T& pos,
               const T& translation,
               float layers)
{
    double r1 = 1.0f - roughness;
    double r2 = 1.0f - std::pow(roughness, layers);
    double r3 = r1/r2;
    float r = roughness;
    float f = frequency;
    T x = pos;
    
    double sum = 0;
    for(int n = 0; n < (int)std::floor(layers); n++) {
        sum += r * noise->eval(x * f);
        r = gain * r;
        f = lacunarity * f;
        x = x - translation;
    }

    float alpha = layers - std::floor(layers);
    sum += alpha * r * noise->eval(x * f);
    sum = r3 * sum;
    return sum;
}

//-----------------------------------------------------------------------------

// -----------------
// Helper functions
// -----------------
VSP<float> lux::noise(const Noise<openvdb::Vec3s>& N)
{
    return std::make_shared<NoiseField>(N);
}

//-----------------------------------------------------------------------------

// ------------------------
// Explicit Instantiations
// ------------------------

namespace lux 
{

    // NoiseBase
    template class NoiseBase<float>;
    template class NoiseBase<openvdb::Vec2s>;
    template class NoiseBase<openvdb::Vec3s>;

    // ValueNoise
    template class ValueNoise<float>;
    template class ValueNoise<openvdb::Vec2s>;
    template class ValueNoise<openvdb::Vec3s>;

    // FractalSum
    template float lux::fsn(const Noise<float>& noise, 
                            float roughness, 
                            float gain, 
                            float frequency, 
                            float lacunarity, 
                            const float& pos,
                            const float& translation,
                            int layers);

    template float lux::fsn(const Noise<openvdb::Vec2s>& noise, 
                            float roughness, 
                            float gain, 
                            float frequency, 
                            float lacunarity, 
                            const openvdb::Vec2s& pos,
                            const openvdb::Vec2s& translation,
                            int layers);

    template float lux::fsn(const Noise<openvdb::Vec3s>& noise, 
                            float roughness, 
                            float gain, 
                            float frequency, 
                            float lacunarity, 
                            const openvdb::Vec3s& pos,
                            const openvdb::Vec3s& translation,
                            int layers);

    template float lux::fsn(const Noise<float>& noise, 
                            float roughness, 
                            float gain, 
                            float frequency, 
                            float lacunarity, 
                            const float& pos,
                            const float& translation,
                            float layers);

    template float lux::fsn(const Noise<openvdb::Vec2s>& noise, 
                            float roughness, 
                            float gain, 
                            float frequency, 
                            float lacunarity, 
                            const openvdb::Vec2s& pos,
                            const openvdb::Vec2s& translation,
                            float layers);

    template float lux::fsn(const Noise<openvdb::Vec3s>& noise, 
                            float roughness, 
                            float gain, 
                            float frequency, 
                            float lacunarity, 
                            const openvdb::Vec3s& pos,
                            const openvdb::Vec3s& translation,
                            float layers);

}


//-----------------------------------------------------------------------------