#include <iostream>

#include "Color.h"
#include "FieldInterface.h"
#include "StarterViewer.h"
#include "Raymarcher.h"
#include "Camera.h"
#include "Mesh.h"

using namespace lux;
using namespace starter;

int main(int argc, char** argv) {

    openvdb::initialize();

    // Define a camera
    std::shared_ptr<Camera> cam = std::make_shared<Camera>();
    
    float zdist = 20;
    float xwidth = 8;
    Vector pos = Vector(0,0,zdist);
    Vector lookAt = Vector(0,0,0);
    Vector view = lookAt - pos;
    Vector axis = Vector(0,1,0);

    cam->setFov(60);
    cam->setEyeViewUp( pos, view, Vector(0,1,0) );

    // Define a raymarcher
    double near = zdist - xwidth / 2.0;
    double far = near + xwidth;
    double steps = 330;
    std::shared_ptr<Raymarcher> rm = std::make_shared<Raymarcher>();
    float min_ds = (far - near) / 330;
    float max_ds = min_ds * 1.5;
    rm->SetDsMin(min_ds);
    rm->SetDsMax(max_ds);
    rm->SetT(1);
    rm->SetTmin(0.001);
    rm->SetSnear(near);
    rm->SetSfar(far);
    rm->SetKappa(0.1);

    // Define an image
    // float theta = 360/n_frames * M_PI / 180;
    std::shared_ptr<ImgProc> img = std::make_shared<ImgProc>();
    img->clear(1920/4, 1080/4, 4);

    // Define a scene
    std::shared_ptr<Mesh> teapot = std::make_shared<Mesh>();
    teapot->loadObj("models/bunnyFixed/bunny_fixed.obj");
    VGSP<float> object_grid = std::make_shared<VolumeGrid<float>>();
    VGSP<Color> color_grid = std::make_shared<VolumeGrid<Color>>();

    int val = strtol(argv[1], NULL, 10);
    // object_grid->init(openvdb::Coord(-val, -val, -val), openvdb::Coord(val, val, val), 0.1, -1000);
    object_grid->init(createLevelSet(teapot, 0.1, 3));
    color_grid->init(object_grid->getBBox(), 0.1, Color(0,0,0,0));

    VSP<float> gf = grid(object_grid);
    VSP<Color> col = constant(Color(1,0,0,0));
    col = col * mask(gf) + col * mask(-gf);

    color_grid->stamp(col);

    VSP<Color> c_gf = grid(color_grid);

    VSP<float> c = -gf;
    col = c_gf;
    
    for (int j = 0; j < img->GetNy(); j++)
    {
        #pragma omp parallel for
        for (int i = 0; i < img->GetNx(); i++)
        {
            Vector direction = cam->calculateDirection(i, j, img->GetNx(), img->GetNy());
            Color output = rm->RayMarchPixel(direction, cam->eye(), c, col);
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