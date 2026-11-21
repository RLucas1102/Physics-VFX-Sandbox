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

void Scene::init(const std::vector<std::string>& args)
{
    // Put all objects/volumes/lights/colors in here
    
    openvdb::initialize();
    
    // Setup camera
    std::cout << "Setting up camera. . ." << std::endl;
    
    float zdist = 15;
    this->setupCamera(zdist);
    
    std::cout << "Done!" << std::endl;

    // Define grids to store volumes and colors
    std::cout << "Setting up grids. . ." << std::endl;

    unsigned int seed = 2016;
    float noiseGridDim     = 256;
    float noiseVxSize      = 0.01;
    openvdb::CoordBBox noiseBBox = openvdb::CoordBBox(openvdb::Coord(-noiseGridDim, -noiseGridDim, -noiseGridDim),
                                                      openvdb::Coord( noiseGridDim,  noiseGridDim,  noiseGridDim));
    Noise<openvdb::Vec3s> noiseGrid = std::make_shared<ValueNoise<openvdb::Vec3s>>(seed, noiseBBox, noiseVxSize);
    
    std::cout << "Done!" << std::endl;

    // Define volumes in scene
    std::cout << "Creating volumes . . ." << std::endl;

    float r3 = std::stof(args[1].c_str());
    float g3 = std::stof(args[2].c_str());
    float f3 = std::stof(args[3].c_str());
    float l3 = std::stof(args[4].c_str());
    openvdb::Vec3s xt3(0,0,0);
    int ly3 = std::stof(args[5].c_str());

    NoiseParams N3 = noiseParams(r3, g3, f3, l3, xt3, ly3);

    float gamma1 = 1.0f;
    float gamma2 = 0.7f;
    float amp1 = 1.0;
    float amp2 = 1.0;
    float transition = 0.5;

    TerrainParams T1 = terrainParams(gamma1, gamma2, amp1, amp2, transition);

    openvdb::Vec3s N(0,1,0);
    openvdb::Vec3s X0(0,-1,0);

    VSP<float> terrain1 = terrain(noiseGrid, N, X0, N3, T1);
    VSP<float> plane1 = plane(Vector(-1, 0, 0), Vector(-4, 0, 0));
    VSP<float> plane2 = plane(Vector( 1, 0, 0), Vector( 4, 0, 0));
    VSP<float> plane3 = plane(Vector( 0, 0,-1), Vector( 0, 0,-4));
    VSP<float> plane4 = plane(Vector( 0, 0, 1), Vector( 0, 0, 4));
    VSP<float> plane5 = plane(Vector( 0,-1, 0), Vector( 0,-2, 0));

    terrain1 = terrain1;
    terrain1 = Intersection(terrain1, plane1);
    terrain1 = Intersection(terrain1, plane2);
    terrain1 = Intersection(terrain1, plane3);
    terrain1 = Intersection(terrain1, plane4);
    terrain1 = Intersection(terrain1, plane5);
    
    VSP<openvdb::Vec3s> material = constant(openvdb::Vec3s(1.0f, 1.0f, 1.0f));

    std::cout << "Done!" << std::endl;

    // Stamp volumes into grids
    std::cout << "Stamping . . ." << std::endl;

    std::cout << "Done!" << std::endl;

    // Create gridded fields
    std::cout << "Gridding and manipulating fields. . ." << std::endl;

    VSP<float> combined = terrain1;

    std::cout << "Done!" << std::endl;

    // Create Deep Shadow Maps
    std::cout << "Creating Deep Shadow Maps. . ." << std::endl;
    
    float DSMGridDim     = 50;
    float DSMVxSize      = 0.1;
    float DSMDg          = 0.0;
    openvdb::CoordBBox DSMBBox = openvdb::CoordBBox(openvdb::Coord(-DSMGridDim, -DSMGridDim, -DSMGridDim),
                                                    openvdb::Coord( DSMGridDim,  DSMGridDim,  DSMGridDim));

    Light key = pointLight(Vector(1,5,1), Color(0.7, 0.7, 0.7, 0.0));
    key->setKappa(1.0);
    key->setDs(0.01);
    key->createDSM(mask(combined), DSMBBox, DSMVxSize, DSMDg);
    
    // float DSMGridDim     = 100;
    // float DSMVxSize      = 0.05;
    // float DSMDg          = 0.0;
    // openvdb::CoordBBox DSMBBox = openvdb::CoordBBox(openvdb::Coord(-DSMGridDim + 50, -DSMGridDim + 50, -DSMGridDim + 50),
    //                                                 openvdb::Coord( DSMGridDim + 50,  DSMGridDim + 50,  DSMGridDim + 50));
    
    // Light key = pointLight(Vector(4 + 4, 4 + 4, 4 + 4), Color(1.0, 0.1, 0.1, 0.0));
    // key->setKappa(2);
    // key->setDs(0.01);
    // key->createDSM(mask(combined), DSMBBox, DSMVxSize, DSMDg);

    std::cout << "Key light done!" << std::endl;

    // Light fill = pointLight(Vector(-4 + 4, -4 + 4, 4 + 4), Color(0.1, 0.1, 1.0, 0.0));
    // fill->setKappa(4);
    // fill->setDs(0.01);
    // fill->createDSM(mask(combined), DSMBBox, DSMVxSize, DSMDg);

    // std::cout << "Fill light done!" << std::endl;

    // Light rim = pointLight(Vector(4,0,-4), Color(0.1, 1.0, 0.1, 0.0));
    // rim->setKappa(8);
    // rim->setDs(0.01);
    // rim->createDSM(mask(combined, DSMBBox, DSMVxSize, DSMDg);

    // std::cout << "Rim light done!" << std::endl;

    // Define where color is and is not
    std::cout << "Finalizing scene. . ." << std::endl;
    
    VSP<Color> output = constant(Color(0,0,0,0));
    output = output * mask(-combined) + toColor(material) * mask(combined);

    // Set member variables
    _volumes.push_back(clamp(combined * constant(300.0f), 0.0f, 5.0f));
    _materials.push_back(output);
    _lights.push_back(key);
    // _lights.push_back(fill);
    // _lights.push_back(rim);

    std::cout << "Done!" << std::endl;

    // Scene initialized
    _initialized = true;

}

void Scene::setupCamera(float zdist)
{
    std::shared_ptr<Camera> cam = std::make_shared<Camera>();
    
    // Vector pos = Vector(4,4,zdist);
    // Vector lookAt = Vector(4,4,0);
    Vector pos = Vector(0,8,zdist);
    Vector lookAt = Vector(0,0,0);
    Vector view = lookAt - pos;
    Vector axis = Vector(0,1,0);

    cam->setFov(60);
    cam->setLookAt(lookAt);
    cam->setRotAxis(axis);
    cam->setEyeViewUp( pos, view, Vector(0,1,0) );
    _cam = cam;
}

void Scene::update(int n_frames,
                   int current,
                   int start)
{
    float theta = 360/n_frames * M_PI / 180;
    if (current == start) {
        theta *= start;
    } 

    // Move camera in here for turntable
    Vector pos = _cam->eye();
    Vector axis = _cam->rotAxis();
    Vector lookAt = _cam->lookAt();

    Vector X = pos;
    float Cos = std::cos(theta);
    float ax = axis * X;
    Vector xa = X^axis;
    pos = X * Cos + axis * ax * (1 - Cos) + xa * std::sin(theta);

    Vector view = lookAt - pos;

    _cam->setEyeViewUp( pos, view, Vector(0,1,0) );

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