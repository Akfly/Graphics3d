#ifndef APPLICATION_HEADER
#define APPLICATION_HEADER

#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <sstream>
#include "glfont_cpp\glfont.h"
#include "cpp-toolkit\headers\Point.hpp"
#include "View.hpp"
#include "SFML/System/Vector3.hpp"
#include "SFML/System/Vector2.hpp"

namespace aut
{
	class Application
	{
	public:
		//Asset directory
		static const std::string ASSETS_DIR;

	private:

		//Window size
		static const size_t window_width  = 800;
		static const size_t window_height = 600;

		//Text data
		static const int TEXT_LEFT_MARGIN  = 30;
		static const int TEXT_LINE_SPACING = 16;
		static const int TEXT_HEIGHT       = 100;

		//Vectors of the active object (the one the user is moving, rotating, etc)
		sf::Vector3<float> activePos;
		sf::Vector3<float> activeRot;
		sf::Vector3<float> activeScale;

		//Used to translate and rotate with the mouse. It represents the position where the mouse clicked.
		sf::Vector2<int> translationOrigin;
		sf::Vector2<int> rotationOrigin;

		bool               wireframe;			//Draw in wireframe or not
		bool               running;				//While the app is running
		std::string        activeText;			//The name of the active element
		int                active_element;		//The id of the active element
		PixelPerfectGLFont font;				//The font to draw the text

		sf::Window window;
		View       view;

	public:

		Application():
		  window(sf::VideoMode(window_width, window_height), "Pixel Buffer Window", sf::Style::Titlebar | sf::Style::Close, sf::ContextSettings(32)),
		  view(window_width, window_height)
		{
			wireframe      = false;
			activeText     = "Camera";
			active_element = 1;
			running        = true;
		}

		//Initializes the context
		void initialize();

		//Main loop starts
		void start();

	private:

		//Converts a floating number into a string of characters
		std::string ftstr(float x)
		{
		  std::ostringstream o;
		  o << x;
		  return o.str();
		}

		//Handle different events like inputs or app close
		void handleEvents();

		//draws the text on the screen
		void drawText();
	};
}

#endif