
#include "Raymarcher.h"

using namespace lux;

// -----------
// Raymarcher
// -----------
Raymarcher::Raymarcher() : _T(0), _Tmin(0), _sNear(0), _sFar(0), _dsMin(0), _dsMax(0), _kappa(0)
{
}

Color Raymarcher::RayMarchPixel(const Vector &direction, const Vector &eye, 
                                const VSP<float> &density, const VSP<Color> &Cm)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<>dis(_dsMin, _dsMax);
    double T = _T;
    Color L(0,0,0,0);
    double s = _sNear;
    Vector X = eye + direction * s;
    while (s < _sFar && T > _Tmin) {
        float den = density->eval(X);

        double ds = dis(gen);

        if (den > 0.0) {
            float dT = std::exp(-ds * _kappa * den);
            L += Cm->eval(X) * (1-dT) * T/_kappa;
            T *= dT;
        }
        X += direction * ds;
        s += ds;
        
    }

    L[3] = 1 - T;
    return L;
}

Color Raymarcher::RayMarchPixelLight(const Vector &direction, const Vector &eye, 
                                     const VSP<float> &density, const VSP<Color> &Cm,
                                     const std::vector<PLight>& lights)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<>dis(_dsMin, _dsMax);

    double T = _T;
    Color L(0,0,0,0);
    double s = _sNear;
    Vector X = eye + direction * s;
    
    while (s < _sFar && T > _Tmin) {
        float den = density->eval(X);

        double ds = dis(gen);

        if (den > 0.0) {
            float dT = std::exp(-ds * _kappa * den);
            Color CLights = Color(0,0,0,0);
            for (size_t i = 0; i < lights.size(); i++)
            {
                CLights += lights[i]->getCol() * lights[i]->getDSM()->eval(X);
            }
            
            L += Cm->eval(X) * CLights * (1-dT) * T/_kappa;
            T *= dT;
        }
        X += direction * ds;
        s += ds;
        
    }

    L[3] = 1 - T;
    return L;
}

Color Raymarcher::RayMarchPixelLightFaster(const Vector &direction, const Vector &eye, 
                                           const VSP<float> &density, const VSP<Color> &Cm,
                                           const std::vector<PLight>& lights, 
                                           const VGSP<openvdb::FloatGrid>& levelSetPtr)
{
    Color result;

    const openvdb::FloatGrid::Ptr levelSet = levelSetPtr->getGridRaw();

    float vs = levelSetPtr->getXform().voxelSize().x();

    openvdb::tools::LevelSetRayIntersector<openvdb::FloatGrid> intersector(*levelSet);

    openvdb::Vec3s originI(eye[0] / vs, eye[1] / vs, eye[2] / vs);

    openvdb::Vec3s directionI(direction[0] / vs, direction[1] / vs, direction[2] / vs);

    openvdb::math::Ray<double> ray(originI, directionI, _sNear, _sFar);
    double hitT;

    // The intersector expects an index-space ray.
    if (intersector.intersectsIS(ray, hitT)) {
        openvdb::Vec3d hitIndex = ray.eye() + hitT * ray.dir();
        openvdb::Vec3d hitWorld = levelSet->transform().indexToWorld(hitIndex);

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<>dis(_dsMin, _dsMax);

        double T = _T;
        Color L(0,0,0,0);
        double s = hitWorld[2];
        Vector X = eye + direction * s;
        
        while (s < _sFar && T > _Tmin) {
            float den = density->eval(X);

            double ds = dis(gen);

            if (den > 0.0) {
                float dT = std::exp(-ds * _kappa * den);
                Color CLights = Color(0,0,0,0);
                for (size_t i = 0; i < lights.size(); i++)
                {
                    CLights += lights[i]->getCol() * lights[i]->getDSM()->eval(X);
                }
                
                L += Cm->eval(X) * CLights * (1-dT) * T/_kappa;
                T *= dT;
            }
            X += direction * ds;
            s += ds;
            
        }

        L[3] = 1 - T;
        result = L;
    }
    else {
        result = Color(0,0,0,0);
    }

    return result;

}

//-----------------------------------------------------------------------------

// -----------------
// Helper Functions
// -----------------

RM lux::raymarcher()
{
    return std::make_shared<Raymarcher>();
}

//-----------------------------------------------------------------------------