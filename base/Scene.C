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

    // Define grids to store volumes and colors
    std::cout << "Setting up grids. . ." << std::endl;

    VGSP<openvdb::FloatGrid> ajaxGrid;
    ajax(ajaxGrid, 0.01);

    VGSP<openvdb::FloatGrid> bunny1Grid;
    bunny(bunny1Grid, 0.01);

    VGSP<openvdb::FloatGrid> bunny2Grid;
    bunny(bunny2Grid, 0.01);

    std::cout << ajaxGrid->getBBox() << std::endl;

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
    
    VSP<Color> material = constant(Color(1.0f, 1.0f, 1.0f, 0.0f));

    std::cout << "Done!" << std::endl;

    // Stamp volumes into grids
    std::cout << "Stamping . . ." << std::endl;

    colorGrid->stamp(toVec3s(material));

    std::cout << "Done!" << std::endl;

    // Create gridded fields
    std::cout << "Gridding and manipulating fields. . ." << std::endl;
    
    VSP<float> griddedAjax = gridField<openvdb::FloatGrid, float>(ajaxGrid);
    griddedAjax = -griddedAjax;

    VSP<float> griddedBunny1 = gridField<openvdb::FloatGrid, float>(bunny1Grid);
    griddedBunny1 = -griddedBunny1;
    griddedBunny1 = scale(griddedBunny1, 0.38);
    griddedBunny1 = rotate(griddedBunny1, -37.778 * M_PI / 180, Vector(1,0,0));
    griddedBunny1 = rotate(griddedBunny1,  46.937 * M_PI / 180, Vector(0,1,0));
    griddedBunny1 = rotate(griddedBunny1, -46.691 * M_PI / 180, Vector(0,0,1));
    griddedBunny1 = translate(griddedBunny1, Vector(-0.073, 0.689, -0.231));
    
    VSP<float> griddedBunny2 = gridField<openvdb::FloatGrid, float>(bunny2Grid);
    griddedBunny2 = -griddedBunny2;
    griddedBunny2 = scale(griddedBunny2, 0.293);
    griddedBunny2 = rotate(griddedBunny2, -37.778 * M_PI / 180, Vector(1,0,0));
    griddedBunny2 = rotate(griddedBunny2,  46.937 * M_PI / 180, Vector(0,1,0));
    griddedBunny2 = rotate(griddedBunny2, -46.691 * M_PI / 180, Vector(0,0,1));
    griddedBunny2 = translate(griddedBunny2, Vector(-0.068, 0.626, -0.238));

    VSP<openvdb::Vec3s> griddedColor = gridField<openvdb::Vec3SGrid, openvdb::Vec3s>(colorGrid);

    VSP<float> planeA = plane(Vector(0,1,0), Vector(0,0,0));
    planeA = rotate(planeA, -27.436 * M_PI / 180, Vector(1,0,0));
    planeA = rotate(planeA, -27.713 * M_PI / 180, Vector(0,1,0));
    planeA = rotate(planeA, -2.736  * M_PI / 180,  Vector(0,0,1));
    planeA = translate(planeA, Vector(0, 0.407, 0));

    VSP<float> combined = Intersection(griddedAjax, planeA);
    combined = Cutout(combined, griddedBunny1);
    combined = Union(combined, griddedBunny2);
    combined = translate(combined, Vector(0, 1.0, 0));

    std::cout << "Done!" << std::endl;

    // Create Deep Shadow Maps
    std::cout << "Creating Deep Shadow Maps. . ." << std::endl;

    Light key = pointLight(Vector(2,2,2), Color(1.0, 0.5, 0.1, 1.0));
    key->setKappa(1);
    key->setDs(0.01);
    key->createDSM(mask(combined), colorBBox, colorVxSize, 0);

    std::cout << "Key light done!" << std::endl;

    Light fill = pointLight(Vector(-2,-2,2), Color(0.5, 0.1, 1.0, 1.0));
    fill->setKappa(2);
    fill->setDs(0.01);
    fill->createDSM(mask(combined), colorBBox, colorVxSize, 0);

    std::cout << "Fill light done!" << std::endl;

    Light rim = pointLight(Vector(2,0,-2), Color(0.1, 1.0, 1.0, 1.0));
    rim->setKappa(4);
    rim->setDs(0.01);
    rim->createDSM(mask(combined), colorBBox, colorVxSize, 0);

    std::cout << "Rim light done!" << std::endl;

    // Define where color is and is not
    std::cout << "Finalizing scene. . ." << std::endl;
    
    VSP<Color> output = toColor(griddedColor) * mask(combined);

    // Set member variables
    _volumes.push_back(clamp(combined * constant(300.0f), 0.0f, 5.0f));
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