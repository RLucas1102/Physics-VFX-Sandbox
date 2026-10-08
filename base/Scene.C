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
    float zdist = 5;
    this->setupCamera(zdist);
    
    // Define volumes in scene
    VSP<float> Sphere = sphere(1.0f);
    VSP<Color> material = constant(Color(1.0f, 0.0f, 1.0f, 0.0f));
    
    VSP<Color> output = material * mask(Sphere);

    // Set member variables
    _volumes.push_back(clamp(Sphere * constant(10.0f), 0.0f, 1.0f));
    _materials.push_back(output);

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