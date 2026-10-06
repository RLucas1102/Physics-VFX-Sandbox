#include "Scene.h"
#include "Raymarcher.h"
#include "ImgProc.h"
#include "StarterViewer.h"

using namespace lux;
using namespace image;
using namespace starter;

int main(int argc, char** argv)
{

    // Create Scene
    SC scene = CreateScene();
    scene->init();
    
    // Setup camera
    float zdist = 10;
    float xwidth = 4;
    scene->setupCamera(zdist, xwidth);
    
    // Define a raymarcher
    double near = zdist - xwidth / 2.0;
    double far = near + xwidth;
    double steps = 330;
    RM rm = raymarcher();
    float min_ds = (far - near) / 330;
    float max_ds = min_ds * 3.5;
    rm->SetDsMin(min_ds);
    rm->SetDsMax(max_ds);
    rm->SetT(1);
    rm->SetTmin(0.001);
    rm->SetSnear(near);
    rm->SetSfar(far);
    rm->SetKappa(0.1);

    // Define an image
    std::shared_ptr<ImgProc> img = std::make_shared<ImgProc>();
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
                Color output = rm->RayMarchPixel(direction, scene->getCamera()->eye(), scene->getVolumes()[0], scene->getMaterials()[0]);
                img->SetValue(i, j, std::vector<float>{(float)output[0], (float)output[1], (float)output[2], (float)output[3]});
            }
            
        }
    }

   StarterViewer* viewer = CreateViewer();

   std::vector<std::string> args;

   for(int i=0;i<argc;i++)
   {
      std::string s(argv[i]);
      args.push_back(s);
   }

   viewer->Init(args);

   viewer->SetDisplayImage(*img);

   viewer->MainLoop();
    
    return 0;
}