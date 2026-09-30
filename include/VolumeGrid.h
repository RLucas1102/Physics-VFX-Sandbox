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

    //-----------------------------------------------------------------------------

    // Setting up logic to be able to determine the data type of the Grid
    // Primary template
    template <typename U>
    struct GridTypes;

    // Specialization for float
    template<>
    struct GridTypes<float>
    {
        using GridType = openvdb::FloatGrid::Ptr;
        using GridAccessor = openvdb::FloatGrid::Accessor;
        using GridValue = float;

        static float fromGrid(const float& val) { return val; }

        static float toGrid(const float& val) { return val; }

        static GridType create(const float& dg) {
            return openvdb::FloatGrid::create(dg);
        }

    };

    // Specialization for Color
    template<>
    struct GridTypes<Color>
    {
        using GridType = openvdb::Vec3SGrid::Ptr;
        using GridAccessor = openvdb::Vec3SGrid::Accessor;
        using GridValue = Color;

        static Color fromGrid(const openvdb::Vec3s& val) {
            return Color(val.x(), val.y(), val.z(), 0);
        }

        static openvdb::Vec3s toGrid(const Color& val ) {
            return openvdb::Vec3s(val.X(), val.Y(), val.Z());
        }

        static GridType create(const Color& dg) {
            openvdb::Vec3s Dg = openvdb::Vec3s(dg.X(), dg.Y(), dg.Z());
            return openvdb::Vec3SGrid::create(Dg);
        }

    };

    //-----------------------------------------------------------------------------
    
    // Volume Grid
    // Create a grid with values at each cell of the grid that to represent a volume
    // Uses openvdb grid
    template<typename T>
    class VolumeGrid {

        public:

            using GridType = typename GridTypes<T>::GridType;
            using GridValue = typename GridTypes<T>::GridValue;

            VolumeGrid() {};
            ~VolumeGrid() = default;

            void init(const openvdb::Coord& llc, 
                      const openvdb::Coord& urc, 
                      const float& vx_size, 
                      const GridValue& dg);

            GridType getGridRaw() const { return _grid; }
            auto getBBox() const { return _bbox; }

            GridValue triLerp(const Vector& P);

            void stamp(const VSP<T>& f);
            
        private:
            GridType _grid;
            std::shared_ptr<openvdb::CoordBBox> _bbox;

    };

    template<typename T>
    using VGSP = std::shared_ptr<VolumeGrid<T>>;

    //-----------------------------------------------------------------------------

    // Grid Field
    // Convert grid into a GridField to work with other fields
    template<typename T>
    class GridField : public Volume<T> {

        public:

            using typename Volume<T>::volumeDataType;

            GridField(const VGSP<T>& g);
            ~GridField() = default;

            const volumeDataType eval(const Vector& p) const override;

        private:
            VGSP<T> _g;
    };

    //-----------------------------------------------------------------------------

}

#endif