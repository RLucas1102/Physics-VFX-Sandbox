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

    // Create Scene
    SC scene = CreateScene();
    scene->init();
    
    RE renderer = CreateRenderer();
    renderer->init();
    ImgProc img = renderer->render(scene);

    StarterViewer* viewer = CreateViewer();

    std::vector<std::string> args;

    for(int i=0;i<argc;i++)
    {
        std::string s(argv[i]);
        args.push_back(s);
    }

    viewer->Init(args);

    viewer->SetDisplayImage(img);

    viewer->MainLoop();
    
    return 0;
}