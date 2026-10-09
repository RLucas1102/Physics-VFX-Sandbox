#include "Renderer.h"

using namespace lux;

// ---------
// Renderer
// ---------

Renderer::Renderer()
    : _initialized(false)
{
}

std::shared_ptr<Renderer> Renderer::pRenderer = nullptr;

void Renderer::init()
{
    // Define a raymarcher
    double near = 5;
    double far = 15;
    double steps = 1000;
    float min_ds = (far - near) / steps;
    
    _raymarcher = raymarcherFixed();
    _raymarcher->SetDs(min_ds);
    _raymarcher->SetT(1);
    _raymarcher->SetTmin(0);
    _raymarcher->SetSnear(near);
    _raymarcher->SetSfar(far);
    _raymarcher->SetKappa(1);
    
    // Initialize the renderer
    _initialized = true;
}

image::ImgProc Renderer::render(const SC& scene)
{
    // Define an image
    std::shared_ptr<image::ImgProc> img = std::make_shared<image::ImgProc>();
    img->clear(1920/4, 1080/4, 4);

    std::cout << "Starting Ray marching. . ." << std::endl;

    #pragma omp parallel 
    {
        #pragma omp for schedule(dynamic, 4)
        for (int j = 0; j < img->GetNy(); j++)
        {
            for (int i = 0; i < img->GetNx(); i++)
            {
                Vector direction = scene->getCamera()->calculateDirection(i, j, img->GetNx(), img->GetNy());
                Color output = _raymarcher->RayMarchPixel(direction, scene->getCamera()->eye(), scene->getVolumes()[0], scene->getMaterials()[0]);
                img->SetValue(i, j, std::vector<float>{(float)output[0], (float)output[1], (float)output[2], (float)output[3]});
            }
            
        }
    }

    return *img;

}
//-----------------------------------------------------------------------------

// -----------------
// Helper Functions
// -----------------

RE lux::CreateRenderer()
{
    return lux::Renderer::Instance();
}

//-----------------------------------------------------------------------------