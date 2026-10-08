#include "Scene.h"

using namespace lux;

// ------
// Scene
// ------

Scene::Scene()
    : _initialized(false), _frame(0)
{
}

std::shared_ptr<Scene> Scene::pScene = nullptr;

void Scene::init()
{
    // Put all objects/volumes/lights/colors in here
    
    // Setup camera
    std::cout << "Setting up camera. . ." << std::endl;
    
    float zdist = 5;
    this->setupCamera(zdist);
    
    std::cout << "Done!" << std::endl;

    // Define grids to store volumes and colors
    std::cout << "Creating Grids. . ." << std::endl;
    
    float sphereGridDim = 10;
    float sphereVxSize  = 0.1;
    float sphereDg      = -1000;
    openvdb::CoordBBox sphereBBox = openvdb::CoordBBox(openvdb::Coord(-sphereGridDim, -sphereGridDim, -sphereGridDim), 
                                                       openvdb::Coord( sphereGridDim,  sphereGridDim,  sphereGridDim));
    VGSP<openvdb::FloatGrid> sphereGrid = grid<openvdb::FloatGrid>();
    sphereGrid->init(sphereBBox, 
                     sphereVxSize, 
                     sphereDg);
    
    float colorGridDim     = 10;
    float colorVxSize      = 0.1;
    openvdb::Vec3s colorDg = openvdb::Vec3s(0,0,0);
    openvdb::CoordBBox colorBBox = openvdb::CoordBBox(openvdb::Coord(-colorGridDim, -colorGridDim, -colorGridDim),
                                                      openvdb::Coord( colorGridDim,  colorGridDim,  colorGridDim));
    VGSP<openvdb::Vec3SGrid> colorGrid = grid<openvdb::Vec3SGrid>();
    colorGrid->init(colorBBox,
                    colorVxSize,
                    colorDg);

    std::cout << "Done!" << std::endl;

    // Define volumes in scene
    std::cout << "Creating volumes . . ." << std::endl;
    
    VSP<float> Sphere = sphere(1.0f);
    VSP<openvdb::Vec3s> material = constant(openvdb::Vec3s(1.0f, 0.0f, 1.0f));

    std::cout << "Done!" << std::endl;

    // Stamp volumes into grids
    std::cout << "Stamping . . ." << std::endl;

    sphereGrid->stamp(Sphere);
    colorGrid->stamp(material);

    std::cout << "Done!" << std::endl;

    std::cout << "Finalizing scene. . ." << std::endl;

    // Create gridded fields
    VSP<float> griddedSphere         = gridField<openvdb::FloatGrid, float>(sphereGrid);
    VSP<openvdb::Vec3s> griddedColor = gridField<openvdb::Vec3SGrid, openvdb::Vec3s>(colorGrid);

    // Define where color is and is not
    VSP<Color> output = toColor(griddedColor) * mask(griddedSphere);

    // Set member variables
    _volumes.push_back(clamp(griddedSphere * constant(10.0f), 0.0f, 1.0f));
    _materials.push_back(output);

    std::cout << "Done!" << std::endl;

    // Scene initialized
    _initialized = true;

}

void Scene::setupCamera(float zdist)
{
    std::shared_ptr<Camera> cam = std::make_shared<Camera>();
    
    Vector pos = Vector(0,0,zdist);
    Vector lookAt = Vector(0,0,0);
    Vector view = lookAt - pos;
    Vector axis = Vector(0,1,0);

    cam->setFov(60);
    cam->setEyeViewUp( pos, view, Vector(0,1,0) );
    _cam = cam;
}

void Scene::update()
{
    // Move camera in here for turntable
}

//-----------------------------------------------------------------------------

// -----------------
// Helper Functions
// -----------------

SC lux::CreateScene()
{
    return lux::Scene::Instance();
}

//-----------------------------------------------------------------------------