#include <cmath>
#include <cassert>
#include "View.hpp"
#include "cpp-toolkit\headers\Scaling.hpp"
#include "cpp-toolkit\headers\Rotation.hpp"
#include "cpp-toolkit\headers\Projection.hpp"
#include "cpp-toolkit\headers\Translation.hpp"
#include "SFML/System/Vector3.hpp"
#include "Application.hpp"

namespace aut
{
	const float View::MOVEMENT_SPEED = 0.1f;
	const float View::ROTATION_SPEED = 0.01f;
	const float View::SCALE_SPEED    = 0.1f;

	/**
	*	Constructor
	**/
	View::View(size_t width, size_t height):

		width       (width         ),
		height      (height        ),
		pixel_buffer(width, height ),

		rasterizer  (pixel_buffer               ),
		firstobj    (pixel_buffer, width, height),
		secondobj   (pixel_buffer, width, height)
    {
		//Load 1st obj
		firstobj.load(Application::ASSETS_DIR + "dinosaurio.obj");
		firstobj.setTranslation(0, 0, -1000);
		firstobj.setRotation   (0.3f, 0.0f, 0.0f);
		firstobj.setScale      (0.4f);

		//Load 2nd obj
		secondobj.load(Application::ASSETS_DIR + "doscubos.obj");
		secondobj.setTranslation(0, 0, -1000);
		secondobj.setRotation   (0.3f, 0.0f, 0.0f);
		secondobj.setScale      (2.0f);

		activeMesh=0;
    }

	/**
	*	Calls the update of the meshes
	**/
	void View::update()
	{
		firstobj.update  (cam.getViewMatrix());
		secondobj.update (cam.getViewMatrix());
	}

	/**
	*	Draws the screen
	**/
    void View::paint ()
    {
		rasterizer.clear(127,127,127);

		firstobj.paint (&rasterizer);
		secondobj.paint(&rasterizer);
		
		pixel_buffer.gl_draw_pixels (0, 0);
    }

	/**
	*	Draws the screen with the meshes in wireframe
	**/
	void View::wire_paint ()
    {
		rasterizer.clear(127,127,127);

		firstobj.wire_paint (&rasterizer);
		secondobj.wire_paint(&rasterizer);
		
		pixel_buffer.gl_draw_pixels (0, 0);
    }

	/**
	*	Change the active mesh to the given one
	**/
	void View::setActiveMesh(int num)
	{
		activeMesh = num;
	}

	/**
	*	Returns the position of the active element
	**/
	sf::Vector3<float> View::getActivePosition()
	{
		sf::Vector3<float> pos;

		switch(activeMesh)
		{
		case 1:
			pos = cam.getTranslation();
			break;
		case 2:
			pos = firstobj.getTranslation();
			break;
		case 3:
			pos = secondobj.getTranslation();
			break;
		}

		return pos;
	}

	/**
	*	Returns the rotation of the active element
	**/
	sf::Vector3<float> View::getActiveRotation()
	{
		sf::Vector3<float> rot;

		switch(activeMesh)
		{
		case 1:
			rot = cam.getRotation();
			break;
		case 2:
			rot = firstobj.getRotation();
			break;
		case 3:
			rot = secondobj.getRotation();
			break;
		}

		return rot;
	}

	/**
	*	Returns the scale of the active element
	**/
	sf::Vector3<float> View::getActiveScale()
	{
		sf::Vector3<float> scale;

		switch(activeMesh)
		{
		case 1:
			break;
		case 2:
			scale = firstobj.getScale();
			break;
		case 3:
			scale = secondobj.getScale();
			break;
		}

		return scale;
	}

	/**
	*	Changes the position by an offset of the active mesh
	**/
	void View::MoveActive(float _x, float _y, float _z)
	{
		_x = _x * MOVEMENT_SPEED;
		_y = _y * MOVEMENT_SPEED;
		_z = _z * MOVEMENT_SPEED;

		switch(activeMesh)
		{
		case 1:
			cam.Move(_x, _y, _z);
			break;
		case 2:
			firstobj.Move(_x, _y, _z);
			break;
		case 3:
			secondobj.Move(_x, _y, _z);
			break;
		}
	}

	/**
	*	Changes the rotation by an offset of the active mesh
	**/
	void View::RotateActive(float _x, float _y, float _z)
	{
		_x = _x * ROTATION_SPEED;
		_y = _y * ROTATION_SPEED;
		_z = _z * ROTATION_SPEED;

		switch(activeMesh)
		{
		case 1:
			cam.Rotate(_x, _y, _z);
			break;
		case 2:
			firstobj.Rotate(_x, _y, _z);
			break;
		case 3:
			secondobj.Rotate(_x, _y, _z);
			break;
		}
	}

	/**
	*	Changes the scale by an offset of the active mesh
	**/
	void View::ScaleActive(float intensity)
	{
		intensity = intensity * SCALE_SPEED;
		switch(activeMesh)
		{
		case 1:
			break;
		case 2:
			firstobj.sumScale(intensity);
			break;
		case 3:
			secondobj.sumScale(intensity);
			break;
		}
	}

}
