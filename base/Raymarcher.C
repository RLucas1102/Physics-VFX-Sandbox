#include <random>
#include "Raymarcher.h"

using namespace lux;

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