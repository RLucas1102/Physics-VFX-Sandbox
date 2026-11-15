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
                   const PointSampler& sampler)
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
                   const PointSampler& sampler)
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
                   const PointSampler& sampler)
{
    float fx = (float)std::floor(P.x());
    float fy = (float)std::floor(P.y());
    float fz = (float)std::floor(P.z());

    float tx = P.x() - fx;
    float ty = P.y() - fy;
    float tz = P.z() - fz;

    tx = tx * tx * (3 - 2 * tx); // Smoothstep
    ty = ty * ty * (3 - 2 * ty);
    tz = tz * tz * (3 - 2 * tz);

    int xMin = (int)fx;
    int yMin = (int)fy;
    int zMin = (int)fz;

    int xMax = (xMin + 1) & bitMask;
    int yMax = (yMin + 1) & bitMask;
    int zMax = (zMin + 1) & bitMask;
        xMin = xMin & bitMask;
        yMin = yMin & bitMask;
        zMin = zMin & bitMask;

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

//-----------------------------------------------------------------------------

// -----------
// ValueNoise 
// -----------
template <typename T>
ValueNoise<T>::ValueNoise(unsigned int seed, openvdb::CoordBBox bbox, float vxSize)
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

    _grid->setTransform(openvdb::math::Transform::createLinearTransform(vxSize));

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
    float frequency = 0.01;
    float lacunarity = 1;
    openvdb::Vec3s pos(p[0], p[1], p[2]);
    openvdb::Vec3s xt(0,0,0);
    int layers = 7;

    float value = fsn(_noise, roughness, gain, frequency, lacunarity, pos, xt, layers) * 10;
    value = value - int(value);

    return value;

}

//-----------------------------------------------------------------------------

// -----------------
// PyroSphere Field
// -----------------
PyroSphereField::PyroSphereField(const Noise<openvdb::Vec3s>& N, float radius, float gamma) :
    _noise(N),
    _radius(radius),
    _gamma(gamma)
{
}

const PyroSphereField::volumeDataType PyroSphereField::eval(const Vector &p) const
{ 
    float roughness = 0.75;
    float gain = 0.75;
    float frequency = 4;
    float lacunarity = 1;
    openvdb::Vec3s pos(p[0], p[1], p[2]);
    openvdb::Vec3s xt(0,0,0);
    int layers = 7;

    float sphereVal = _radius - p.magnitude();

    openvdb::Vec3s posClose = _radius * (pos / p.magnitude());

    float value = std::pow(std::fabs(fsn(_noise, roughness, gain, frequency, lacunarity, posClose, xt, layers)), _gamma) * 2;
    value = value - (int)value;


    return sphereVal - value;

}

//-----------------------------------------------------------------------------

// -------------------
// Noise Region Field
// -------------------
NoiseRegion::NoiseRegion(const Noise<openvdb::Vec3s>& N, const Vector& cen, float radius, float fade) :
    _noise(N),
    _center(cen),
    _radius(radius),
    _fade(fade)
{
}

const NoiseRegion::volumeDataType NoiseRegion::eval(const Vector &p) const
{ 
    float roughness = 0.75;
    float gain = 0.75;
    float frequency = 2;
    float lacunarity = 1.0;
    openvdb::Vec3s pos(p[0], p[1], p[2]);
    openvdb::Vec3s xt(0,0,0);
    int layers = 7;

    float q;
    if (_fade < 0) {
        q = 1;
    }
    else {
        float term1 = (p - _center).magnitude();
        float term2 = term1 / _radius;
        float term3 = 1.0f - term2;
        q = term3 / _fade;

        if (q > 1) { q = 1; }
        else if (q < 0) { q = 0; }
        else { q = q; }

    }

    float value = 2.0 * fsn(_noise, roughness, gain, frequency, lacunarity, pos, xt, layers) - 1.0;

    return q * value;

}

//-----------------------------------------------------------------------------

// -----------
// Wisp field
// -----------
WispField::WispField(const Noise<openvdb::Vec3s>& N, 
                     const WispParams& wparams, 
                     const NoiseParams& n1params,
                     const NoiseParams& n2params,
                     float vxSize) :
                     _noise(N),
                     _Wparams(wparams),
                     _N1params(n1params),
                     _N2params(n2params)
{
    
    _grid = openvdb::FloatGrid::create(0.0f);
    
    _grid->setTransform(openvdb::math::Transform::createLinearTransform(vxSize));

    std::mt19937 gen(_Wparams.seed); 
    std::uniform_real_distribution<float> distrFloat(-1, 1); 
    auto randFloat = [&distrFloat, &gen](){ return distrFloat(gen); };

    typename openvdb::FloatGrid::Accessor accessor = _grid->getAccessor();

    float r1           = _N1params.roughness;
    float g1           = _N1params.gain;
    float f1           = _N1params.frequency;
    float l1           = _N1params.lacunarity;
    openvdb::Vec3s tx1 = _N1params.translation;
    int o1             = _N1params.layers;
    
    float r2           = _N2params.roughness;
    float g2           = _N2params.gain;
    float f2           = _N2params.frequency;
    float l2           = _N2params.lacunarity;
    openvdb::Vec3s tx2 = _N2params.translation;
    int o2             = _N2params.layers;

    int num           = _Wparams.numChildren;
    float d           = _Wparams.den;
    float c           = _Wparams.clump;
    openvdb::Vec3s dP = _Wparams.dP;
    openvdb::Vec3s Pg = _Wparams.Pguide;
    float Ps          = _Wparams.Pscale;
    openvdb::Vec3s t   = _Wparams.t;
    openvdb::Vec3s n   = _Wparams.n;
    openvdb::Vec3s b   = _Wparams.b;

    for (int count = 0; count < num; ++count) {
        
        openvdb::Vec3s P0(randFloat(), randFloat(), randFloat()); // Random position in [-1,1] cube

        openvdb::Vec3s P1 = P0 / P0.length(); // Project position to unit sphere

        float R = 2.0 * std::pow(std::fabs(fsn(_noise, r1, g1, f1, l1, P0, tx1, o1)), c) - 1.0; // Create radius from FSN

        openvdb::Vec3s P3 = R * P1; // Move position to radius

        openvdb::Vec3s P4 = Pg + Ps * (P3.x() * t + P3.y() * n + P3.z() * b); // Apply guide particle location and pscale

        openvdb::Vec3s D(
            2.0 * fsn(_noise, r2, g2, f2, l2, P3, tx2, o2) - 1.0,    
            2.0 * fsn(_noise, r2, g2, f2, l2, P3 + dP, tx2, o2) - 1.0,    
            2.0 * fsn(_noise, r2, g2, f2, l2, P3 - dP, tx2, o2) - 1.0    
        );

        openvdb::Vec3s P5 = P4 + D; // Move to displaced position

        P5 = _grid->worldToIndex(P5);

        auto makeInt = [&P5](int i){ return (int)std::floor(P5[i]); };

        openvdb::Coord ijk(makeInt(0), makeInt(1), makeInt(2));
        int i = ijk[0], j = ijk[1], k = ijk[2];

        for (int ii = i; ii < i + 2; ++ii) {
            for (int jj = j; jj < j + 2; ++jj) {
                for (int kk = k; kk < k + 2; ++kk) {

                    openvdb::Coord pqr(ii, jj, kk);

                    float term1 = 1 - (t.dot((pqr.asVec3s() - P5)) / vxSize);
                    float term2 = 1 - (n.dot((pqr.asVec3s() - P5)) / vxSize);
                    float term3 = 1 - (b.dot((pqr.asVec3s() - P5)) / vxSize);
                    float weight = term1 * term2 * term3;

                    float g = accessor.getValue(pqr);
                    g += d * weight;
                    accessor.setValue(pqr, g);

                }
            }
        }
    }
}

const WispField::volumeDataType WispField::eval(const Vector& p) const
{
    const openvdb::Vec3d xyz(p.X(), p.Y(), p.Z());

    openvdb::Vec3d index = _grid->worldToIndex(xyz);
    
    float v = openvdb::tools::BoxSampler::sample(_grid->tree(), index);

    return v;
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

VSP<float> lux::pyroSphere(const Noise<openvdb::Vec3s>& N, float radius, float gamma)
{
    return std::make_shared<PyroSphereField>(N, radius, gamma);
}
    
VSP<float> lux::noiseRegion(const Noise<openvdb::Vec3s>& N, Vector cen, float radius, float fade)
{
    return std::make_shared<NoiseRegion>(N, cen, radius, fade);
}

VSP<float> lux::wisp(const Noise<openvdb::Vec3s>& N, 
                     const WispParams& wparams, 
                     const NoiseParams& n1params,
                     const NoiseParams& n2params,
                     float vxSize)
{
    return std::make_shared<WispField>(N, wparams, n1params, n2params, vxSize);
}

NoiseParams lux::noiseParams(float roughness, 
                             float gain, 
                             float frequency, 
                             float lacunarity,
                             const openvdb::Vec3s& translation,
                             int layers)
{
    NoiseParams N;
    N.roughness = roughness;
    N.gain = gain;
    N.frequency = frequency;
    N.lacunarity = lacunarity;
    N.translation = translation;
    N.layers = layers;

    return N;
}


WispParams lux::wispParams(unsigned int numChildren,
                           float den,
                           float clump,
                           const openvdb::Vec3s& dP, // Displacement Vector
                           const openvdb::Vec3s& Pguide, // Guide particle location
                           float Pscale, // Guide particle scale
                           const openvdb::Vec3s& t, // Coordinate system
                           const openvdb::Vec3s& n,
                           const openvdb::Vec3s& b,
                           unsigned int seed)
{
    WispParams W;
    W.numChildren = numChildren;
    W.den = den;
    W.clump = clump;
    W.dP = dP;
    W.Pguide = Pguide;
    W.Pscale = Pscale;
    W.t = t;
    W.n = n;
    W.b = b;
    W.seed = seed;

    return W;
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