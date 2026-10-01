#ifndef VOLUME_GRID_H
#define VOLUME_GRID_H

#include <memory>

#include <openvdb/openvdb.h>
#include <openvdb/tools/Interpolation.h>

#include "Volume.h"
#include "Vector.h"

namespace lux {

    // Volume Grid
    // Create a grid with values at each cell of the grid that to represent a volume
    // Uses openvdb grid
    template<typename GridType>
    class VolumeGrid {
        
        public:

            using GridT = typename GridType::Ptr;
            using GridV = typename GridType::ValueType;

            VolumeGrid() {}
            ~VolumeGrid() = default;

            // Initialization functions for grids
            void init(const openvdb::Coord& llc, 
                      const openvdb::Coord& urc, 
                      float vx_size, 
                      const GridV& dg);

        
            // Perform trilinear interpolation on a given world coordinate
            // World coordinate will be converted to index coords
            GridV triLerp(const Vector& P);

            // Evaluated given field at every point in grid and store value
            void stamp(const VSP<GridV>& f);

        private:
            GridT _grid;
            std::shared_ptr<openvdb::CoordBBox> _bbox;

    };

    //-----------------------------------------------------------------------------

    // Defining VGSP as a shared pointer of volume grid
    template<typename GridType>
    using VGSP = std::shared_ptr<VolumeGrid<GridType>>;

    // Helper Functions
    //-----------------------------------------------------------------------------

    // Create grid
    template<typename GridType>
    VGSP<GridType> grid();

}

#endif