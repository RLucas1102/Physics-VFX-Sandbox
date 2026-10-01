#ifndef VOLUMEGRID_H
#define VOLUMEGRID_H

#include <openvdb/openvdb.h>
#include <openvdb/tools/Interpolation.h>

#include <memory.h>
#include <iostream>

#include "Volume.h"
#include "Vector.h"
#include "Color.h"

namespace lux {
    
    // Volume Grid
    // Create a grid with values at each cell of the grid that to represent a volume
    // Uses openvdb grid
    template<typename GridType>
    class VolumeGrid {

        public:

            using GridPtr = typename GridType::Ptr;
            using GridValue = typename GridType::ValueType;

            VolumeGrid() {};
            ~VolumeGrid() = default;

            // Initialization functions for grids
            void init(const openvdb::Coord& llc, 
                      const openvdb::Coord& urc, 
                      float vx_size, 
                      const GridValue& dg);

            void init(const openvdb::CoordBBox& bbox,
                      float vx_size,
                      const GridValue& dg);

            void init(const GridPtr& grid);
            
            // Accessors
            GridPtr getGridRaw() const { return _grid; }
            openvdb::CoordBBox getBBox() const { return *_bbox; }
            openvdb::math::Transform getGridXform() const { return _grid->transform(); }

            // Perform trilinear interpolation on a given world coordinate
            // World coordinate will be converted to index coords
            GridValue triLerp(const Vector& P);

            // Evaluated given field at every point in grid and store value
            void stamp(const VSP<T>& f);
            
        private:
            GridPtr _grid;
            std::shared_ptr<openvdb::CoordBBox> _bbox;

    };

    template<typename GridType>
    using VGSP = std::shared_ptr<VolumeGrid<GridType>>;

    //-----------------------------------------------------------------------------

}

#endif