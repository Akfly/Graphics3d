#ifndef VIEW_HEADER
#define VIEW_HEADER

	#include <vector>
	#include "Rasterizer.hpp"
	#include "Mesh.hpp"
	#include "Camera.hpp"
    #include "Pixel_Buffer_Rgba8888.hpp"
	#include "cpp-toolkit\headers\Point.hpp"
	#include "cpp-toolkit\headers\Rotation.hpp"
	#include "cpp-toolkit\headers\Translation.hpp"
	#include "SFML/System/Vector3.hpp"

    namespace aut
    {
        class View
        {
        private:

            typedef Pixel_Buffer_Rgba8888 Pixel_Buffer;

        private:

			static const float MOVEMENT_SPEED;
			static const float ROTATION_SPEED;
			static const float SCALE_SPEED;

			int          activeMesh;	//The mesh that the user controls now
			size_t       width;			//Window size
            size_t       height;

			Pixel_Buffer             pixel_buffer;
			Rasterizer<Pixel_Buffer> rasterizer;
			Camera                   cam;

			//Meshes
			Mesh<Pixel_Buffer>       firstobj;
			Mesh<Pixel_Buffer>       secondobj;

        public:

			//Constructor
            View(size_t width, size_t height);

			void update    ();	//Calls the update of the meshes
            void paint     ();	//Draws the screen
			void wire_paint();	//Draws the screen with the meshes in wireframe

			//Change the active mesh to the given one
			void setActiveMesh(int num);

			//Returns the position, rotation and scale of the active element
			sf::Vector3<float> getActivePosition();
			sf::Vector3<float> getActiveRotation();
			sf::Vector3<float> getActiveScale   ();

			//Changes the position, rotation and scale by an offset of the active mesh
			void MoveActive  (float _x, float _y, float _z);
			void RotateActive(float _x, float _y, float _z);
			void ScaleActive (float intensity);

        };
    }

#endif
