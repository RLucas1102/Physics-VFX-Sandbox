#ifndef RENDERER_H
#define RENDERER_H

#include <vector>

#include "Color.h"
#include "Vector.h"
#include "Raymarcher.h"
#include "ImgProc.h"
#include "Scene.h"

namespace lux
{

    // Renderer
    // A class to handle the rendering of a 3D scene
    class Renderer {

        public:

            // Renderer is a singleton
            static std::shared_ptr<Renderer> Instance() 
            {
                if(pRenderer==nullptr)
                {
                    pRenderer = std::shared_ptr<Renderer>(new Renderer());
                }
                return pRenderer;
            }

            ~Renderer() = default;

            // Initialize the renderer 
            void init();

            // Render the scene
            image::ImgProc render(const SC& scene);

        private:

            bool _initialized;

            RM _raymarcher;

            // Static renderer exists in class as a whole
            static std::shared_ptr<Renderer> pRenderer;

            // Private constructor
            Renderer();

    };

    //-----------------------------------------------------------------------------

    // Defining RE as a shared pointer of a Renderer object
    using RE = std::shared_ptr<Renderer>;

    //-----------------------------------------------------------------------------

    // Helper functions
    // Useful functions for creating and managing the renderer
    RE CreateRenderer();

    //-----------------------------------------------------------------------------

} // namespace lux


#endif