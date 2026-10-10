#ifndef SCENE_H
#define SCENE_H

#include <openvdb/openvdb.h>

#include <vector>

#include "Color.h"
#include "Vector.h"
#include "FieldInterface.h"
#include "Mesh.h"
#include "Light.h"
#include "Camera.h"
#include "VolumeGrid.h"

namespace lux
{
    // Scene
    // A class to that contains all the elements of a 3D scene
    // Lights, objects, volumes, camera, etc.
    class Scene {

        public:

            // Scene is a singleton
            static std::shared_ptr<Scene> Instance() 
            {
                if(pScene==nullptr)
                {
                    pScene = std::shared_ptr<Scene>(new Scene());
                }
                return pScene;
            }

            ~Scene() = default;

            // Initialize the scene with objects, light, etc.
            void init();

            void setupCamera(float zdist);
            
            // Accessors
            std::vector<Mesh>           getObjects()    { return _objects; };
            std::vector<Light>          getLights()     { return _lights; };
            std::shared_ptr<Camera>     getCamera()     { return _cam; };
            std::vector<VSP<Color>>     getMaterials()  { return _materials; };
            std::vector<VSP<float>>     getVolumes()    { return _volumes; };

            // Update objects the scene in some way per frame
            void update(int n_frames = 1,
                        int current  = 0,
                        int start = 0);

        private:

            bool _initialized;

            int _frame;

            // Data containers for scene's contents
            std::vector<Mesh>       _objects;
            std::vector<Light>      _lights;
            std::shared_ptr<Camera> _cam;
            std::vector<VSP<Color>> _materials;
            std::vector<VSP<float>> _volumes;

            // Static scene exists in class as a whole
            static std::shared_ptr<Scene> pScene;

            // Private constructor
            Scene();

    };

    //-----------------------------------------------------------------------------

    // Defining SC as a shared pointer of a Scene object
    using SC = std::shared_ptr<Scene>;

    //-----------------------------------------------------------------------------

    // Helper functions
    // Useful functions for creating and managing the scene
    SC CreateScene();

    //-----------------------------------------------------------------------------

} // namespace lux


#endif