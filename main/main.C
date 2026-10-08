#include "Scene.h"
#include "Raymarcher.h"
#include "ImgProc.h"
#include "StarterViewer.h"
#include "Renderer.h"

using namespace lux;
using namespace image;
using namespace starter;

int main(int argc, char** argv)
{

    std::vector<std::string> args;

    for(int i=0;i<argc;i++)
    {
        std::string s(argv[i]);
        args.push_back(s);
    }
    
    // Create Scene
    SC scene = CreateScene();
    scene->init();
    
    // Create Renderer
    RE tesselator = CreateRenderer();
    tesselator->init();
    ImgProc img = tesselator->render(scene);

    // Create Viewer
    StarterViewer* viewer = CreateViewer();
    viewer->Init(args);
    viewer->SetDisplayImage(img);
    viewer->MainLoop();
    
    return 0;
}