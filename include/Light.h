#ifndef LIGHT_H
#define LIGHT_H

#include <memory>

#include <openvdb/openvdb.h>

#include "Color.h"
#include "Vector.h"
#include "FieldInterface.h"
#include "VolumeGrid.h"

namespace lux {

    // Deep shadow map settings
    // Define the number of steps and extinction coeffcient for lights
    struct DSMSettings
    {
        double ds = 0.1;
        float kappa = 24;
    };
    
    //-----------------------------------------------------------------------------

    // Base class
    // All lights have a position, color, DSM, and settings
    class LightBase {

        public:

            LightBase() {};
            LightBase(const Vector& inPos, const Color& inCol);
            virtual ~LightBase() = default;

            // Accessors
            Vector     getPos() const {return _pos;}
            Color      getCol() const {return _col;}
            VSP<float> getDSM() const {return _DSM;}

            // Mutators
            void setPos(const Vector& inPos) {_pos = inPos;}
            void setCol(const Color& inCol)  {_col = inCol;}

            // Create a deep shadow map of a given volume from the calling light source
            virtual void createDSM(const VSP<float>& gridField, 
                                   const openvdb::CoordBBox& bbox,
                                   float vx_size,
                                   float dg) = 0;

        protected:
            Vector      _pos;
            Color       _col;
            VSP<float>  _DSM;
            DSMSettings _settings;

    };

    //-----------------------------------------------------------------------------

    // Point Light
    // An omni directional light source
    class PointLight : LightBase {

        public:

            PointLight() {}
            PointLight(const Vector& inPos, const Color& inCol);

            // Accessors
            Vector     getPos() const {return _pos;}
            Color      getCol() const {return _col;}
            VSP<float> getDSM() const {return _DSM;}

            // Mutators
            void setPos(const Vector& inPos) {_pos = inPos;}
            void setCol(const Color& inCol)  {_col = inCol;}

            void createDSM(const VSP<float>& gridField, 
                           const openvdb::CoordBBox& bbox,
                           float vx_size,
                           float dg = 0.0f);
        
    };

    //-----------------------------------------------------------------------------

    // Defining PLight as a shared pointer of a point light
    using PLight = std::shared_ptr<PointLight>;

    //-----------------------------------------------------------------------------

    // Helper Functions
    // Useful functions for other files to create lights with

    PLight pointLight(const Vector& inPos, const Color& inCol);

    //-----------------------------------------------------------------------------

} // namespace lux


#endif