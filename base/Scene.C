#include "Scene.h"

using namespace lux;

// ------
// Scene
// ------

Scene::Scene()
    : initialized(false), frame(0)
{
}

std::shared_ptr<Scene> Scene::pScene = nullptr;

void Scene::init()
{
    // Put all objects/volumes/lights/colors in here
    
    // Define volumes in scene
    VSP<float> red_sphere = sphere(1.0f);
    VSP<Color> material = constant(Color(1.0f, 0.0f, 0.0f, 0.0f));
    
    VSP<Color> output = material * red_sphere;

    // Set member variables
    _volumes.push_back(red_sphere);
    _materials.push_back(output);

}

void Scene::setupCamera(float zdist, float xwidth)
{
    std::shared_ptr<Camera> cam = std::make_shared<Camera>();
    
    Vector pos = Vector(0,4,zdist);
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