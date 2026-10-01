#include "Scene.h"

using namespace lux;

std::shared_ptr<Scene> Scene::Instance()
{
    if(pScene==nullptr)
    {
        pScene = std::make_shared<Scene>();
    }
    return pScene;
}

void Scene::init()
{
    // Put all objects/volumes/lights/colors in here
}

void Scene::update()
{
    // Move camera in here for turntable
}

// -------------------------------------------------
// End Scene

std::shared_ptr<Scene> lux::CreateScene()
{
    return std::shared_ptr<Scene>();
}