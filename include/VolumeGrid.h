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

            void init (const openvdb::CoordBBox& bbox,
                       float vx_size, 
                       const GridV& dg);

            void initLevelSet(const GridT& grid);

            // Accessors
            openvdb::CoordBBox getBBox() {return *_bbox;}

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

    //-----------------------------------------------------------------------------

    // Grid Field
    // Convert grid into a GridField to work with other fields
    template<typename GridT, typename GridV>
    class GridField : public Volume<GridV> {

        public:

            using typename Volume<GridV>::volumeDataType;

            GridField(const VGSP<GridT>& g);
            ~GridField() = default;

            const volumeDataType eval(const Vector& p) const override;

        private:
            VGSP<GridT> _g;
    };

    //-----------------------------------------------------------------------------

    // Helper Functions
    // Functions for VolumeGrid and GridField that are useful for other classes to use

    // Create grid
    template<typename GridType>
    VGSP<GridType> grid();

    // Create grid field
    template<typename GridT, typename GridV>
    VSP<GridV> gridField(const VGSP<GridT>& g);

    //-----------------------------------------------------------------------------

} // namespace lux

#endif