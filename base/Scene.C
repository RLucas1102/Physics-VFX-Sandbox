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
    
    float zdist = 10;
    this->setupCamera(zdist);
    
    std::cout << "Done!" << std::endl;

    // Define mesh objects
    MeshSP objMesh = mesh();
    objMesh->loadObj("models/bunnyFixed/bunny_fixed.obj");

    // Define grids to store volumes and colors
    std::cout << "Creating Grids. . ." << std::endl;

    float objVxSize = 0.1;
    VGSP<openvdb::FloatGrid> objGrid = grid<openvdb::FloatGrid>();
    objGrid->initLevelSet(createLevelSet(objMesh, objVxSize));

    std::cout << objGrid->getBBox() << std::endl;

    float colorGridDim     = 30;
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
    
    VSP<openvdb::Vec3s> material = constant(openvdb::Vec3s(1.0f, 1.0f, 1.0f));

    std::cout << "Done!" << std::endl;

    // Stamp volumes into grids
    std::cout << "Stamping . . ." << std::endl;

    colorGrid->stamp(material);

    std::cout << "Done!" << std::endl;

    std::cout << "Finalizing scene. . ." << std::endl;

    // Create gridded fields
    VSP<float> griddedObj = gridField<openvdb::FloatGrid, float>(objGrid);
    griddedObj = -griddedObj;
    VSP<openvdb::Vec3s> griddedColor = gridField<openvdb::Vec3SGrid, openvdb::Vec3s>(colorGrid);

    // Create Deep Shadow Maps
    Light key = pointLight(Vector(2,2,2), Color(1.0,0.1,0.1,1));
    key->setKappa(1);
    key->setDs(0.01);
    key->createDSM(mask(griddedObj), colorBBox, colorVxSize, 0);
    
    Light fill = pointLight(Vector(-2,-2,2), Color(0.1,0.1,1.0,1));
    fill->setKappa(2);
    fill->setDs(0.01);
    fill->createDSM(mask(griddedObj), colorBBox, colorVxSize, 0);

    Light rim = pointLight(Vector(2,0,-2), Color(0.1,1.0,0.1,1));
    rim->setKappa(4);
    rim->setDs(0.01);
    rim->createDSM(mask(griddedObj), colorBBox, colorVxSize, 0);
    
    // Define where color is and is not
    VSP<Color> output = toColor(griddedColor) * mask(griddedObj);

    // Set member variables
    _volumes.push_back(clamp(griddedObj * constant(100.0f), 0.0f, 1.0f));
    _materials.push_back(output);
    _lights.push_back(key);
    _lights.push_back(fill);
    _lights.push_back(rim);

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