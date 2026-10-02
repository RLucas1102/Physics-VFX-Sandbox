#include <iostream>
#include <vector>

#include "Color.h"
#include "FieldInterface.h"
#include "VolumeGrid.h"
#include "Mesh.h"
#include "StarterViewer.h"
#include "Raymarcher.h"
#include "Camera.h"
#include "Light.h"

using namespace lux;
using namespace starter;

int main(int argc, char** argv) {

    // Animation settings
    int n_frames = strtol(argv[1], NULL, 10);
    int start_frame = strtol(argv[2], NULL, 10);
    int end_frame = strtol(argv[3], NULL, 10);

    // Define a camera
    std::shared_ptr<Camera> cam = std::make_shared<Camera>();
    
    float zdist = 20;
    float xwidth = 8;
    Vector pos = Vector(0,4,zdist);
    Vector lookAt = Vector(0,0,0);
    Vector view = lookAt - pos;
    Vector axis = Vector(0,1,0);

    cam->setFov(60);
    cam->setEyeViewUp( pos, view, Vector(0,1,0) );

    // Define a raymarcher
    double near = zdist - xwidth / 2.0;
    double far = near + xwidth;
    double steps = 330;
    RM rm = raymarcher();
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
    float theta = 360/n_frames * M_PI / 180;
    std::shared_ptr<ImgProc> img = std::make_shared<ImgProc>();
    img->clear(1920/4, 1080/4, 4);

    // Define a scene
    MeshSP object = mesh();
    object->loadObj("models/bunnyFixed/bunny_fixed.obj");
    VGSP<openvdb::FloatGrid> myGrid = grid<openvdb::FloatGrid>();
    myGrid->initLevelSet(createLevelSet(object, 0.1));
    VSP<float> gf = gridField<openvdb::FloatGrid, float>(myGrid);
    gf = mask(-gf);

    // Create color
    VSP<openvdb::Vec3s> object_color = constant(openvdb::Vec3s(1, 1, 1));
    VGSP<openvdb::Vec3SGrid> color_grid = grid<openvdb::Vec3SGrid>();
    color_grid->init(myGrid->getBBox(), 0.1, openvdb::Vec3s(0,0,0));
    object_color = object_color * mask(gf);
    color_grid->stamp(object_color);
    object_color = gridField<openvdb::Vec3SGrid, openvdb::Vec3s>(color_grid);
    VSP<Color> color = toColor(object_color);

    // Create a point light
    std::vector<PLight> PLights(3);

    PLight point1 = pointLight(Vector(15, 15, 15), Color(0.2, 0.1, 1.0, 0));
    point1->createDSM(gf, myGrid->getBBox(), myGrid->getXform().voxelSize().x());
    
    PLight point2 = pointLight(Vector(15, 0, -15), Color(0.1, 0.5, 0.1, 0));
    point2->createDSM(gf, myGrid->getBBox(), myGrid->getXform().voxelSize().x());

    PLight point3 = pointLight(Vector(-15, 0, -15), Color(0.2, 0.1, 0.5, 0));
    point3->createDSM(gf, myGrid->getBBox(), myGrid->getXform().voxelSize().x());

    PLights[0] = point1;
    PLights[1] = point2;
    PLights[2] = point3;


    Vector X = pos;
    float Cos = std::cos(start_frame * theta);
    float ax = axis * X;
    Vector xa = X^axis;
    pos = X * Cos + axis * ax * (1 - Cos) + xa * std::sin(start_frame * theta);

    view = lookAt - pos;

    cam->setEyeViewUp( pos, view, Vector(0,1,0) );

    for (int k = start_frame; k < end_frame; k++)
    {
        for (int j = 0; j < img->GetNy(); j++)
        {
            #pragma omp parallel for
            for (int i = 0; i < img->GetNx(); i++)
            {
                Vector direction = cam->calculateDirection(i, j, img->GetNx(), img->GetNy());
                Color output = rm->RayMarchPixelLight(direction, cam->eye(), gf, color, PLights);
                img->SetValue(i, j, std::vector<float>{output[0], output[1], output[2], output[3]});
            }
            
        }

        std::stringstream ss;
        ss << "images/bunny." << std::setw(4) << std::setfill('0') << k << ".exr";
        std::string filename = ss.str();
        img->Write(filename);

        X = pos;
        Cos = std::cos(theta);
        ax = axis * X;
        xa = X^axis;
        pos = X * Cos + axis * ax * (1 - Cos) + xa * std::sin(theta);

        view = lookAt - pos;

        cam->setEyeViewUp( pos, view, Vector(0,1,0) );
        
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