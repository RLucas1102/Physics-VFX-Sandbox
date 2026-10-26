#include "Noise.h"

// ----------------
// Noise functions
// ----------------
openvdb::FloatGrid::Ptr lux::valueNoise(unsigned int seed)
{
    using GridType = openvdb::FloatGrid;
    using GBBox = openvdb::CoordBBox;
    using c = openvdb::Coord;
    
    GridType::Ptr fGrid = GridType::create(0.0f);

    GBBox bbox = GBBox(c(-10.0f, -10.0f, 0.0f), 
                       c( 10.0f,  10.0f, 0.0f));

    fGrid->fill(bbox, 1.0f, true);    

    std::mt19937 gen(seed); 
    std::uniform_real_distribution<float> distrFloat; 
    auto randFloat = std::bind(distrFloat, gen); 

    for(auto iter = fGrid->beginValueOn(); iter; ++iter)
    {
        iter.setValue(randFloat());
    }

    return fGrid;

}