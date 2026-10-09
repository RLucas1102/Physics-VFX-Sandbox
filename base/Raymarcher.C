
#include "Raymarcher.h"

using namespace lux;

// ---------------
// RaymarcherBase
// ---------------

RaymarcherBase::RaymarcherBase() : _T(0), _Tmin(0), _sNear(0), _sFar(0), _ds(0), _kappa(0)
{
}

//-----------------------------------------------------------------------------

// ----------------
// RaymarcherFixed
// ----------------
RaymarcherFixed::RaymarcherFixed() : RaymarcherBase()
{
}

Color RaymarcherFixed::RayMarchPixel(const Vector &direction, const Vector &eye, 
                                const VSP<float> &density, const VSP<Color> &Cm)
{
    double T = _T;
    Color L(0,0,0,0);
    double s = _sNear;
    Vector X = eye + direction * s;
    while (s < _sFar && T > _Tmin) {
        float den = density->eval(X);

        if (den > 0.0) {
            float dT = std::exp(-_ds * _kappa * den);
            L += Cm->eval(X) * (1-dT) * T/_kappa;
            T *= dT;
        }
        X += direction * _ds;
        s += _ds;
        
    }

    L[3] = 1 - T;
    return L;
}

//-----------------------------------------------------------------------------

// ----------------------
// RaymarcherLightsFixed
// ----------------------
RaymarcherLightsFixed::RaymarcherLightsFixed() : RaymarcherBase()
{
}

Color RaymarcherLightsFixed::RayMarchPixel(const Vector &direction, const Vector &eye, 
                                           const VSP<float> &density, const VSP<Color> &Cm,
                                           const std::vector<Light>& lights)
{

    double T = _T;
    Color L(0,0,0,0);
    double s = _sNear;
    Vector X = eye + direction * s;
    
    while (s < _sFar && T > _Tmin) {
        float den = density->eval(X);

        double ds = _ds;

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

//-----------------------------------------------------------------------------

// -----------------
// Helper Functions
// -----------------

RM lux::raymarcherFixed()
{
    return std::make_shared<RaymarcherFixed>();
}

RM lux::raymarcherLightsFixed()
{
    return std::make_shared<RaymarcherLightsFixed>();
}

//-----------------------------------------------------------------------------