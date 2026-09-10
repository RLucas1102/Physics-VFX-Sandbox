#include <iostream>
#include <memory.h>
#include <vector>

#include "Volume.h"
#include "FieldInterface.h"
#include "Vector.h"
#include "Matrix.h"
#include "Color.h"
#include "Camera.h"
#include "Raymarcher.h"
#include "StarterViewer.h"
#include "ImgProc.h"

using namespace lux;
using namespace image;
using namespace starter;

int main(int argc, char** argv) {

    // Define a camera
    std::shared_ptr<Camera> cam = std::make_shared<Camera>();
    cam->setFov(60);
    cam->setEyeViewUp( Vector(0,0,10), Vector(0,0,-1), Vector(0,1,0) );

    // Define a raymarcher
    double near = 0;
    double far = 50;
    double steps = 20;
    std::shared_ptr<Raymarcher> rm = std::make_shared<Raymarcher>();
    rm->SetT(1);
    rm->SetTmin(0.5);
    rm->SetSnear(near);
    rm->SetSfar(far);
    rm->SetDs((far - near)/steps);
    rm->SetKappa(0.0001);

    // Define an image
    std::shared_ptr<ImgProc> img = std::make_shared<ImgProc>();
    img->clear(1920/2, 1080/2, 4);

    // Sphere
    float radius = 1;
    VSP<float> sphereA  = sphere(radius);
    VSP<Color> sphereC = constant(Color(1,0,0,0));
    VSP<Color> background = constant(Color(0,1,0,0));

    VSP<Color> color = background * mask(-sphereA) + sphereC * mask(sphereA);
    VSP<float> density = mask(sphereA);

    for (int j = 0; j < img->GetNy(); j++)
    {
        for (int i = 0; i < img->GetNx(); i++)
        {
            Vector direction = cam->calculateDirection(i, j, img->GetNx(), img->GetNy());
            Color output = rm->RayMarchPixel(direction, cam->eye(), density, color);
            img->SetValue(i, j, std::vector<float>{output[0], output[1], output[2], output[3]});
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