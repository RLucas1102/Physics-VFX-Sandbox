#ifndef RAYMARCHER_H
#define RAYMARCHER_H

#include <memory>
#include <vector>

#include "Color.h"
#include "Vector.h"
#include "Light.h"
#include "Volume.h"
#include "ImplicitFields.h"

namespace lux {

    // Raymarcher
    // Rays are shot from each pixel of the image plane
    // and march into the scene based on a step size.
    // At each step in the scene, density is evaluate
    // to accumulate a color with a certain transmissivity
    class Raymarcher
    {
        public:
            Raymarcher();
            ~Raymarcher() = default;

            // Main algorithm for ray marching a pixel
            Color RayMarchPixel(const Vector& direction, const Vector& eye,
                                const VSP<float>& density, const VSP<Color>& Cm);
            
            // Main algorithm for ray marching a pixel with a light
            Color RayMarchPixelLight(const Vector& direction, const Vector& eye,
                                     const VSP<float>& density, const VSP<Color>& Cm,
                                     const std::vector<PLight>& lights);

            // Mutators
            void SetT(double T) { _T = T; }
            void SetTmin(double Tmin) { _Tmin = Tmin; }
            void SetSnear(double sNear) { _sNear = sNear; }
            void SetSfar(double sFar) { _sFar = sFar; }
            void SetDsMax(double ds) { _dsMax = ds; }
            void SetDsMin(double ds) {_dsMin = ds; }
            void SetKappa(double kappa) { _kappa = kappa; }

        private:
            double _T, _Tmin, _sNear, _sFar, _dsMax, _dsMin, _kappa;


    };

    //-----------------------------------------------------------------------------

    // Defining RM as shared pointer for ray marcher
    using RM = std::shared_ptr<Raymarcher>;

    //-----------------------------------------------------------------------------

    // Helper functions
    // Useful functions to create raymarchers
    RM raymarcher();

    //-----------------------------------------------------------------------------

}

#endif