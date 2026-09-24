#ifndef VOLUMEGRID_H
#define VOLUMEGRID_H

#include "Volume.h"
#include "Vector.h"

namespace lux {
    
    class VolumeGrid {

        public:

            VolumeGrid() {};
            ~VolumeGrid() = default;

            void init(const openvdb::Coord& llc, const openvdb::Coord& urc, const float& vx_size, const float& dg);

            auto getGridRaw() const { return _grid; }
            auto getBBox() const { return _bbox; }

            float triLerp(const Vector& P);

            void stamp(const VSP<float>& f);
            
        private:
            openvdb::FloatGrid::Ptr _grid;
            std::shared_ptr<openvdb::CoordBBox> _bbox;

    };

}

#endif