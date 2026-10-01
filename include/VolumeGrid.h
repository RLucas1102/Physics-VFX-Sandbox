#ifndef VOLUME_GRID_H
#define VOLUME_GRID_H

#include <memory>

#include <openvdb/openvdb.h>
#include <openvdb/tools/Interpolation.h>

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