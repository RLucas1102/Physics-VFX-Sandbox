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

    VSP<float> noiseRegion1 = noiseRegion(noiseGrid, Vector(0,0,0), 2.0f, 2.0f);

    VSP<float> pyroSphere1 = pyroSphere(noiseGrid, 2.0f, 3.0f);

    int C = 100000000;
    float d = 0.005;
    float cl = 0.8;
    openvdb::Vec3s dP(0.0, 0.0, 0.0);
    openvdb::Vec3s Pguide(0,0,0);
    float Pscale = 2.0f;
    openvdb::Vec3s t(2,0,0);
    openvdb::Vec3s n(0,2,0);
    openvdb::Vec3s b(0,0,2);

    WispParams W = wispParams(C, d, cl, dP, Pguide, Pscale, t, n, b, seed);

    float r1 = 0.75;
    float g1 = 0.75;
    float f1 = 2;
    float l1 = 1;
    openvdb::Vec3s xt1(0,0,0);
    int ly1 = 7;
    
    NoiseParams N1 = noiseParams(r1, g1, f1, l1, xt1, ly1);
    
    float r2 = 0.75;
    float g2 = 0.75;
    float f2 = 2;
    float l2 = 1;
    openvdb::Vec3s xt2(0,0,0);
    int ly2 = 7;

    NoiseParams N2 = noiseParams(r2, g2, f2, l2, xt2, ly2);

    // VSP<float> wisp1 = wisp(noiseGrid, W, N1, N2, 0.005);
    
    float r3 = 0.75;
    float g3 = 0.75;
    float f3 = 2;
    float l3 = 1.1;
    openvdb::Vec3s xt3(0,0,0);
    int ly3 = 7;
    
    NoiseParams N3 = noiseParams(r3, g3, f3, l3, xt3, ly3);

    float gamma1 = 1.5f;
    float gamma2 = 3.0f;
    float amp1 = 4.0;
    float amp2 = 0.7;
    float transition = 0.3;

    TerrainParams T1 = terrainParams(gamma1, gamma2, amp1, amp2, transition);
    
    openvdb::Vec3s N(0,1,0);
    openvdb::Vec3s X0(0,-1,0);

    VSP<float> terrain1 = terrain(noiseGrid, N, X0, N3, T1);
    VSP<float> plane1 = plane(Vector(-1, 0, 0), Vector(-4, 0, 0));
    VSP<float> plane2 = plane(Vector( 1, 0, 0), Vector( 4, 0, 0));
    VSP<float> plane3 = plane(Vector( 0, 0,-1), Vector( 0, 0,-4));
    VSP<float> plane4 = plane(Vector( 0, 0, 1), Vector( 0, 0, 4));
    VSP<float> plane5 = plane(Vector( 0,-1, 0), Vector( 0,-2, 0));
    
    VSP<openvdb::Vec3s> material = constant(openvdb::Vec3s(1.0f, 1.0f, 1.0f));

    std::cout << "Done!" << std::endl;

    // Stamp volumes into grids
    std::cout << "Stamping . . ." << std::endl;

    std::cout << "Done!" << std::endl;

    // Create gridded fields
    std::cout << "Gridding and manipulating fields. . ." << std::endl;

    terrain1 = Intersection(terrain1, plane1);
    terrain1 = Intersection(terrain1, plane2);
    terrain1 = Intersection(terrain1, plane3);
    terrain1 = Intersection(terrain1, plane4);
    terrain1 = Intersection(terrain1, plane5);

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
    
    // Light key = pointLight(Vector(4,4,4), Color(1.0, 0.1, 0.1, 0.0));
    // key->setKappa(2);
    // key->setDs(0.01);
    // key->createDSM(mask(combined), DSMBBox, DSMVxSize, DSMDg);

    // std::cout << "Key light done!" << std::endl;

    // Light fill = pointLight(Vector(-4,-4,4), Color(0.1, 0.1, 1.0, 0.0));
    // fill->setKappa(4);
    // fill->setDs(0.01);
    // fill->createDSM(mask(combined), DSMBBox, DSMVxSize, DSMDg);

    // std::cout << "Fill light done!" << std::endl;

    // Light rim = pointLight(Vector(4,0,-4), Color(0.1, 1.0, 0.1, 0.0));
    // rim->setKappa(8);
    // rim->setDs(0.01);
    // rim->createDSM(mask(combined), DSMBBox, DSMVxSize, DSMDg);

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