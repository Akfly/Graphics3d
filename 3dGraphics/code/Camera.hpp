
#ifndef CAMERA_HEADER
#define CAMERA_HEADER

	#include <vector>
	#include "cpp-toolkit\headers\Point.hpp"
	#include "cpp-toolkit\headers\Rotation.hpp"
	#include "cpp-toolkit\headers\Translation.hpp"
	#include "SFML/System/Vector3.hpp"

    namespace aut
    {
        class Camera
        {
        private:

			//Matrices
			toolkit::Rotation3f    rotation_x;
			toolkit::Rotation3f    rotation_y;
			toolkit::Rotation3f    rotation_z;
			toolkit::Translation3f translation;

			//Position and rotation in the world
			sf::Vector3<float> pos;
			sf::Vector3<float> rot;

        public:

		Camera()
			{
				//Initialize matrices
				translation.set(0, 0, 0);
				rotation_x.set<toolkit::Rotation3f::AROUND_THE_X_AXIS>(0.0f);
				rotation_y.set<toolkit::Rotation3f::AROUND_THE_Y_AXIS>(0.0f);
				rotation_z.set<toolkit::Rotation3f::AROUND_THE_Z_AXIS>(0.0f);

				pos.x = 0;
				pos.y = 0;
				pos.z = 0;

				rot.x = 0;
				rot.y = 0;
				rot.z = 0;
			}

			//Returns the transformation matrix of the camera
			toolkit::Transformation3f getViewMatrix()
			{
				return translation * rotation_x * rotation_y * rotation_z;
			}

			//Sets the rotation of the camera
			inline void setRotation(float _x, float _y, float _z)
			{
				rotation_x.set<toolkit::Rotation3f::AROUND_THE_X_AXIS>(-_x);
				rotation_y.set<toolkit::Rotation3f::AROUND_THE_Y_AXIS>(-_y);
				rotation_z.set<toolkit::Rotation3f::AROUND_THE_Z_AXIS>(-_z);

				rot.x = _x;
				rot.y = _y;
				rot.z = _z;
			}

			//Sets the position of the camera
			inline void setTranslation(float _x, float _y, float _z)
			{
				translation.set(-_x, -_y, -_z);
				pos.x = _x;
				pos.y = _y;
				pos.z = _z;
			}

			//Returns the position of the camera in the world
			sf::Vector3<float> getTranslation()
			{
				return pos;
			}

			//Returns the rotation of the camera in the world
			sf::Vector3<float> getRotation()
			{
				return rot;
			}

			//Moves the camera a given offset
			void Move(float _x, float _y, float _z)
			{
				pos.x += _x;
				pos.y += _y;
				pos.z += _z;

				translation.set(-pos.x, -pos.y, -pos.z);
			}
			 
			//Rotates the camera a given offset
			void Rotate(float _x, float _y, float _z)
			{
				rot.x += _x;
				rot.y += _y;
				rot.z += _z; 

				rotation_x.set<toolkit::Rotation3f::AROUND_THE_X_AXIS>(-rot.x);
				rotation_y.set<toolkit::Rotation3f::AROUND_THE_Y_AXIS>(-rot.y);
				rotation_z.set<toolkit::Rotation3f::AROUND_THE_Z_AXIS>(-rot.z);
			}
        };

    }

#endif
