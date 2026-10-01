#ifndef LIGHT_H
#define LIGHT_H

#include "Color.h"
#include "Vector.h"
#include "FieldInterface.h"
#include "VolumeGrid.h"

namespace lux {

    struct DSMSettings
    {
        double ds;
        double kappa;
    };
    

    class LightBase {

        public:

            Light() {};
            Light(const Vector& inPos, const Color& inCol);
            virtual ~Light() = default;

            // Accessors
            Vector     getPos() const {return _pos;}
            Color      getCol() const {return _col;}
            VSP<float> getDSM() const {return _DSM;}

            // Mutators
            void setPos(const Vector& inPos) {_pos = inPos;}
            void setCol(const Color& inCol)  {_col = inCol;}
            virtual void createDSM(const VSP<float> gridField) = 0;

        protected:
            Vector      _pos;
            Color       _col;
            VSP<float>  _DSM;
            DSMSettings _settings;

    };

    class PointLight : LightBase {

        public:

            PointLight() {}
            PointLight(const Vector& inPos, const Color& inCol);

            void createDSM(const VSP<float> gridField);
        
    }

} // namespace lux


#endif