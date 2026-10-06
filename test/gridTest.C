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
    
    float zdist = 10;
    float xwidth = 4;
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
    float max_ds = min_ds * 3.5;
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

    std::cout << "Creating Level Set. . ." << std::endl;

    // Define a scene
    MeshSP object = mesh();
    object->loadObj("models/bunnyFixed/bunny_fixed.obj");
    VGSP<openvdb::FloatGrid> myGrid = grid<openvdb::FloatGrid>();
    myGrid->initLevelSet(createLevelSet(object, 0.01));
    VSP<float> gf = gridField<openvdb::FloatGrid, float>(myGrid);
    gf = clamp(-gf * constant(10.0f), 0, 2);

    std::cout << "Done!" << std::endl;
    std::cout << "Stamping Color Grid. . ." << std::endl;
    
    // Create color
    VSP<openvdb::Vec3s> object_color = constant(openvdb::Vec3s(1, 1, 1));
    VGSP<openvdb::Vec3SGrid> color_grid = grid<openvdb::Vec3SGrid>();

    float val = 30;
    openvdb::CoordBBox cBBox = openvdb::CoordBBox(openvdb::Coord(-val, -val, -val), openvdb::Coord(val, val, val));
    
    color_grid->init(cBBox, 0.5, openvdb::Vec3s(0,0,0));
    object_color = object_color * mask(gf);
    color_grid->stamp(object_color);
    object_color = gridField<openvdb::Vec3SGrid, openvdb::Vec3s>(color_grid);
    VSP<Color> color = toColor(object_color);

    std::cout << "Done!" << std::endl;

    // Create a point light
    std::vector<PLight> PLights(3);

    std::cout << "Creating Deep Shadow Map 1. . ." << std::endl;

    PLight key = pointLight(Vector(3, 3, 3), Color(0.2, 0.1, 1.0, 0));
    key->createDSM(gf, cBBox, 0.1);

    std::cout << "Creating Deep Shadow Map 2. . ." << std::endl;
    
    PLight fill = pointLight(Vector(0, -3, 0), Color(0.1, 1.0, 0.1, 0));
    fill->createDSM(gf, cBBox, 0.1);

    std::cout << "Creating Deep Shadow Map 3. . ." << std::endl;

    PLight rim = pointLight(Vector(0, 0, -3), Color(1.0, 0.1, 0.2, 0));
    rim->createDSM(gf, cBBox, 0.1);

    PLights[0] = key;
    PLights[1] = fill;
    PLights[2] = rim;

    std::cout << "Done!" << std::endl;    

    Vector X = pos;
    float Cos = std::cos(start_frame * theta);
    float ax = axis * X;
    Vector xa = X^axis;
    pos = X * Cos + axis * ax * (1 - Cos) + xa * std::sin(start_frame * theta);

    view = lookAt - pos;

    cam->setEyeViewUp( pos, view, Vector(0,1,0) );

    for (int k = start_frame; k < end_frame; k++)
    {

        std::cout << "Frame: " << k << std::endl;
        std::cout << "Starting Ray marching. . ." << std::endl;

        #pragma omp parallel 
        {
            #pragma omp for schedule(dynamic, 4)
            for (int j = 0; j < img->GetNy(); j++)
            {
                for (int i = 0; i < img->GetNx(); i++)
                {
                    Vector direction = cam->calculateDirection(i, j, img->GetNx(), img->GetNy());
                    Color output = rm->RayMarchPixelLightFaster(direction, cam->eye(), gf, color, PLights, myGrid);
                    img->SetValue(i, j, std::vector<float>{(float)output[0], (float)output[1], (float)output[2], (float)output[3]});
                }
                
            }
        }

        std::stringstream ss;
        ss << "images/bunny." << std::setw(4) << std::setfill('0') << k << ".exr";
        std::string filename = ss.str();
        img->Write(filename);

        std::cout << ss.str() << " complete!" << std::endl;

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