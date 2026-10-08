#ifndef RAYMARCHER_H
#define RAYMARCHER_H

#include <memory>
#include <vector>
#include <random>

#include <openvdb/openvdb.h>
#include <openvdb/tools/RayIntersector.h>

#include "Color.h"
#include "Vector.h"
#include "Light.h"
#include "Volume.h"
#include "ImplicitFields.h"

namespace lux {

    // RaymarcherBase
    // A base class for ray marchers to inherit from that provides common functionality
    class RaymarcherBase
    {
        protected:
            double _T, _Tmin, _sNear, _sFar, _ds, _kappa;

        public:
            RaymarcherBase();
            ~RaymarcherBase() = default;

            // Main algorithm for ray marching a pixel
            virtual Color RayMarchPixel(const Vector& direction, const Vector& eye,
                                        const VSP<float>& density, const VSP<Color>& Cm) 
                                        { return Color(0,0,0,0); }

            // Mutators
            void SetT(double T)         { _T = T; }
            void SetTmin(double Tmin)   { _Tmin = Tmin; }
            void SetSnear(double sNear) { _sNear = sNear; }
            void SetSfar(double sFar)   { _sFar = sFar; }
            void SetDs(double ds)       { _ds = ds; }
            void SetKappa(double kappa) { _kappa = kappa; }

    };
    
    //-----------------------------------------------------------------------------

    // RaymarcherFixed
    // Rays are shot from each pixel of the image plane
    // and march into the scene based on a fixed step size.
    // At each step in the scene, density is evaluate
    // to accumulate a color with a certain transmissivity
    class RaymarcherFixed : public RaymarcherBase
    {
        public:
            RaymarcherFixed();
            ~RaymarcherFixed() = default;

            // Raymarch with a fixed step size
            Color RayMarchPixel(const Vector& direction, const Vector& eye,
                                const VSP<float>& density, const VSP<Color>& Cm) override;
    };

    //-----------------------------------------------------------------------------

    // Defining RM as shared pointer for ray marcher
    using RM = std::shared_ptr<RaymarcherBase>;

    //-----------------------------------------------------------------------------

    // Helper functions
    // Useful functions to create raymarchers
    RM raymarcherFixed();

    //-----------------------------------------------------------------------------

}

#endif