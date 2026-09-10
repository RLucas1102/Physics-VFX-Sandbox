#ifndef RAYMARCHER_H
#define RAYMARCHER_H

#include "Color.h"
#include "Vector.h"
#include "Volume.h"
#include "ImplicitFields.h"

namespace lux {

    class Raymarcher
    {
        private:
            double _T, _Tmin, _sNear, _sFar, _ds, _kappa;

        public:
            Raymarcher();
            ~Raymarcher() = default;

            // Main algorithm for ray marching a pixel
            Color RayMarchPixel(const Vector& direction, const Vector& eye,
                                const VSP<float>& density, const VSP<Color>& Cm);

            // Mutators
            void SetT(double T) { _T = T; }
            void SetTmin(double Tmin) { _Tmin = Tmin; }
            void SetSnear(double sNear) { _sNear = sNear; }
            void SetSfar(double sFar) { _sFar = sFar; }
            void SetDs(double ds) { _ds = ds; }
            void SetKappa(double kappa) { _kappa = kappa; }

    };

}

#endif