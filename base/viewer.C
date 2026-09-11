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

// Colors
const VSP<Color> BLACK = constant(Color(0,0,0,0));
const VSP<Color> ORANGE = constant(Color(0.715, 0.369, 0, 0));
const VSP<Color> WHITE = constant(Color(1,1,1,0));
const VSP<Color> RED = constant(Color(1,0,0,0));
const VSP<Color> YELLOW = constant(Color(1,1,0,0));
const VSP<Color> PURPLE = constant(Color(1,0,1,0));
const VSP<Color> GREEN = constant(Color(0,1,0,0));

int main(int argc, char** argv) {

    // Define a camera
    std::shared_ptr<Camera> cam = std::make_shared<Camera>();
    
    Vector pos = Vector(0,2,6);
    Vector view = Vector(0,1.25,0) - pos;
    Vector axis = Vector(0,1,0);

    cam->setFov(60);
    cam->setEyeViewUp( pos, view, Vector(0,1,0) );

    // Define a raymarcher
    double near = 4;
    double far = 10;
    double steps = 330;
    std::shared_ptr<Raymarcher> rm = std::make_shared<Raymarcher>();
    rm->SetT(1);
    rm->SetTmin(0.001);
    rm->SetSnear(near);
    rm->SetSfar(far);
    rm->SetDs((far - near)/steps);
    rm->SetKappa(0.1);

    // Define an image
    int n_frames = 20;
    float theta = 360/n_frames * M_PI / 180;
    std::shared_ptr<ImgProc> img = std::make_shared<ImgProc>();
    img->clear(1920/4, 1080/4, 4);

    // Scene
    VSP<float> objects = constant(-100.0f);
    VSP<Color> objects_color = BLACK;

    VSP<float> pupil = sphere(0.050);
    pupil = translate(pupil, Vector(0,2.223,0.282));
    objects = Union(objects, pupil);
    objects_color = objects_color*mask(-pupil) + BLACK*mask(pupil);

    VSP<float> eye = sphere(0.1);
    eye = translate(eye, Vector(0.005, 2.237, 0.212));
    eye = Cutout(eye, pupil);
    objects = Union(objects, eye);
    objects_color = objects_color*mask(-eye) + WHITE*mask(eye);

    VSP<float> head = sphere(0.250);
    head = translate(head, Vector(0, 2.277, 0));
    head = Cutout(head, eye);
    objects = Union(objects, head);
    objects_color = objects_color*mask(-head) + ORANGE*mask(head);

    VSP<float> hat = sphere(0.2);
    hat = translate(hat, Vector(0,2.372,0));
    hat = Cutout(hat, head);
    objects = Union(objects, hat);
    objects_color = objects_color*mask(-hat) + RED*mask(hat);

    VSP<float> hat_stem = cylinder(0.030, Vector(0,1,0));
    hat_stem = Intersection(hat_stem, plane(Vector(0,-1,0), Vector(0, 2.569, 0)));
    hat_stem = Intersection(hat_stem, plane(Vector(0,1,0), Vector(0, 2.724, 0)));
    objects  = Union(objects, hat_stem);
    objects_color = objects_color*mask(-hat_stem) + YELLOW*mask(hat_stem);

    VSP<float> hat_top = torus(0.075, 0.030, Vector(0,0,-1));
    hat_top = translate(hat_top, Vector(0,2.805,0));
    objects = Union(objects, hat_top);
    objects_color = objects_color*mask(-hat_top) + YELLOW*mask(hat_top);

    VSP<float> body_neck = cylinder(0.050, Vector(0,1,0));
    body_neck = Intersection(body_neck, plane(Vector(0,-1,0), Vector(0, 0.608, 0)));
    body_neck = Intersection(body_neck, plane(Vector(0,1,0), Vector(0, 2.108, 0)));
    VSP<float> body_bottom = sphere(0.250);
    VSP<float> body_chest  = ellipse(0.25, 0.15, Vector(0,1,0));
    body_bottom = translate(body_bottom, Vector(0, 0.852, 0));
    body_chest = translate(body_chest, Vector(0, 1.192, 0));
    VSP<float> body = Union(body_bottom, body_chest);
    body = Union(body, body_neck);
    objects = Union(objects, body);
    objects_color = objects_color*mask(-body) + ORANGE*mask(body);

    VSP<float> leg_left_1 = cylinder(0.04, Vector(0,1,0));
    leg_left_1 = Intersection(leg_left_1, plane(Vector(0, 1, 0), Vector(0,1,0)));
    leg_left_1 = rotate(leg_left_1, -61.947, Vector(0,0,1));
    leg_left_1 = translate(leg_left_1, Vector(0,0.728,0));
    leg_left_1 = Intersection(leg_left_1, plane(Vector(-1, 0, 0), Vector(0,0,0)));
    objects  = Union(objects, leg_left_1);
    objects_color = objects_color*mask(-leg_left_1) + ORANGE*mask(leg_left_1);

    VSP<float> leg_left_2 = cylinder(0.04, Vector(0,1,0));
    leg_left_2 = Intersection(leg_left_2, plane(Vector(0, 1, 0), Vector(0,0.45,0)));
    leg_left_2 = Intersection(leg_left_2, plane(Vector(0, -1, 0), Vector(0,-1.2,0)));
    leg_left_2 = rotate(leg_left_2, 24.568, Vector(0,0,1));
    leg_left_2 = translate(leg_left_2, Vector(0.98,1,0));
    objects  = Union(objects, leg_left_2);
    objects_color = objects_color*mask(-leg_left_2) + ORANGE*mask(leg_left_2);

    VSP<float> foot_left = cone(0.3, 0.4, Vector(0,-1,0));
    foot_left = translate(foot_left, Vector(1.635, 0.106, 0));
    objects  = Union(objects, foot_left);
    objects_color = objects_color*mask(-foot_left) + ORANGE*mask(foot_left);

    VSP<float> leg_right_1 = cylinder(0.04, Vector(0,1,0));
    leg_right_1 = Intersection(leg_right_1, plane(Vector(0, 1, 0), Vector(0,1,0)));
    leg_right_1 = rotate(leg_right_1, 61.947, Vector(0,0,1));
    leg_right_1 = translate(leg_right_1, Vector(0,0.728,0));
    leg_right_1 = Intersection(leg_right_1, plane(Vector(1, 0, 0), Vector(0,0,0)));
    objects  = Union(objects, leg_right_1);
    objects_color = objects_color*mask(-leg_right_1) + ORANGE*mask(leg_right_1);

    VSP<float> leg_right_2 = cylinder(0.04, Vector(0,1,0));
    leg_right_2 = Intersection(leg_right_2, plane(Vector(0, 1, 0), Vector(0,0.45,0)));
    leg_right_2 = Intersection(leg_right_2, plane(Vector(0, -1, 0), Vector(0,-1.2,0)));
    leg_right_2 = rotate(leg_right_2, -24.568, Vector(0,0,1));
    leg_right_2 = translate(leg_right_2, Vector(-0.98,1,0));
    objects  = Union(objects, leg_right_2);
    objects_color = objects_color*mask(-leg_right_2) + ORANGE*mask(leg_right_2);

    VSP<float> foot_right = cone(0.3, 0.4, Vector(0,-1,0));
    foot_right = translate(foot_right, Vector(-1.635, 0.106, 0));
    objects  = Union(objects, foot_right);
    objects_color = objects_color*mask(-foot_right) + ORANGE*mask(foot_right);

    VSP<float> arm_left_1 = cylinder(0.04, Vector(0,1,0));
    arm_left_1 = Intersection(arm_left_1, plane(Vector(0, 1, 0), Vector(0,1,0)));
    arm_left_1 = rotate(arm_left_1, -61.947, Vector(0,0,1));
    arm_left_1 = translate(arm_left_1, Vector(0,1.337,0));
    arm_left_1 = Intersection(arm_left_1, plane(Vector(-1, 0, 0), Vector(0,0,0)));
    objects  = Union(objects, arm_left_1);
    objects_color = objects_color*mask(-arm_left_1) + ORANGE*mask(arm_left_1);

    VSP<float> arm_right_1 = cylinder(0.04, Vector(0,1,0));
    arm_right_1 = Intersection(arm_right_1, plane(Vector(0, 1, 0), Vector(0,1,0)));
    arm_right_1 = rotate(arm_right_1, 61.947, Vector(0,0,1));
    arm_right_1 = translate(arm_right_1, Vector(0,1.337,0));
    arm_right_1 = Intersection(arm_right_1, plane(Vector(1, 0, 0), Vector(0,0,0)));
    objects  = Union(objects, arm_right_1);
    objects_color = objects_color*mask(-arm_right_1) + ORANGE*mask(arm_right_1);

    VSP<float> hand_left = box(0.1, 4);
    hand_left = translate(hand_left, Vector(0.85, 2.007, 0));
    objects  = Union(objects, hand_left);
    objects_color = objects_color*mask(-hand_left) + ORANGE*mask(hand_left);

    VSP<float> hand_right = box(0.1, 4);
    hand_right = translate(hand_right, Vector(-0.85, 2.007, 0));
    objects  = Union(objects, hand_right);
    objects_color = objects_color*mask(-hand_right) + ORANGE*mask(hand_right);

    VSP<float> powerball_1 = icosahedron();
    powerball_1 = shell(powerball_1, 1);
    powerball_1 = Blend(powerball_1, powerball_1, 2.0, 2.0);
    powerball_1 = scale(powerball_1, 0.05);
    powerball_1 = translate(powerball_1, Vector(1.1, 2.3, 0));
    objects = Union(objects, powerball_1);
    objects_color = objects_color*mask(-powerball_1) + PURPLE*mask(powerball_1);

    VSP<float> powerball_2 = steiner();
    powerball_2 = dilate(powerball_2, 1);
    powerball_2 = scale(powerball_2, 0.05);
    powerball_2 = translate(powerball_2, Vector(-1.1, 2.3, 0));
    objects = Union(objects, powerball_2);
    objects_color = objects_color*mask(-powerball_2) + GREEN*mask(powerball_2);


    VSP<Color> color = objects_color;
    VSP<float> density = mask(objects);

    for (int k = 0; k < n_frames; k++)
    {
        for (int j = 0; j < img->GetNy(); j++)
        {
            #pragma omp parallel for
            for (int i = 0; i < img->GetNx(); i++)
            {
                Vector direction = cam->calculateDirection(i, j, img->GetNx(), img->GetNy());
                Color output = rm->RayMarchPixel(direction, cam->eye(), density, color);
                img->SetValue(i, j, std::vector<float>{output[0], output[1], output[2], output[3]});
            }
            
        }

        img->Write("images/test." + std::to_string(k) + ".png");

        Vector X = pos;
        float Cos = std::cos(theta);
        float ax = axis * X;
        Vector xa = X^axis;
        pos = X * Cos + axis * ax * (1 - Cos) + xa * std::sin(theta);

        view = Vector(0,1.25,0) - pos;

        cam->setEyeViewUp( pos, view, Vector(0,1,0) );
        
    }

//    StarterViewer* viewer = CreateViewer();

//    std::vector<std::string> args;

//    for(int i=0;i<argc;i++)
//    {
//       std::string s(argv[i]);
//       args.push_back(s);
//    }

//    viewer->Init(args);

//    viewer->SetDisplayImage(*img);

//    viewer->MainLoop();

    return 0;
}