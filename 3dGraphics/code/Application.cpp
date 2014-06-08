#include "Application.hpp"

namespace aut
{
	const std::string aut::Application::ASSETS_DIR = "../../assets/";

	/**
	*	Initializes the context
	**/
	void aut::Application::initialize()
	{
		view.setActiveMesh(active_element);

		// Initialization (some gl functions needed for text drawing):
		window.setVerticalSyncEnabled (true);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glDisable (GL_BLEND);
		glDisable (GL_DITHER);
		glDisable (GL_CULL_FACE);
		glDisable (GL_DEPTH_TEST);
		glDisable (GL_TEXTURE_2D);

		glViewport     (0, 0, window_width, window_height);
		glMatrixMode   (GL_PROJECTION);
		glLoadIdentity ();
		glOrtho        (0, GLdouble(window_width), 0, GLdouble(window_height), -1, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		//Load the font
		GLuint textureName;
		glGenTextures(1, &textureName);
		try
		{
			font.Create(ASSETS_DIR + "font01.glf", textureName);
		}
		catch(GLFontError::InvalidFile)
		{
			abort();
		}
	}

	/**
	*	Main loop starts
	**/
	void aut::Application::start()
	{
		do
		{
			// Attend the window events:
			handleEvents();

			// Update the view:

			view.update ();

			activePos   = view.getActivePosition();
			activeRot   = view.getActiveRotation();
			activeScale = view.getActiveScale();

			// Repaint the view:

			if(wireframe)
			{
				view.wire_paint ();
			}
			else
			{
				view.paint ();
			}

			//Text
			drawText();

			// Swap the OpenGL buffers:
			window.display ();
		}
		while (running);
	}

	/**
	*	draws the text on the screen
	**/
	void aut::Application::drawText()
	{
		glEnable(GL_TEXTURE_2D);
		glColor3f(1.0f, 0.0f, 0.0f);
		try
		{
			font.Begin();

			glColor3f(1.0f, 0.0f, 0.0f);
			font.TextOut("ACTIVE: " + activeText, TEXT_LEFT_MARGIN, TEXT_HEIGHT, 0);

			glColor3f(1.0f, 1.0f, 1.0f);
			font.TextOut("Position:  (" + ftstr(activePos.x) + ", " + ftstr(activePos.y) + ", " + ftstr(activePos.z) + ")",  TEXT_LEFT_MARGIN, TEXT_HEIGHT - TEXT_LINE_SPACING, 0);
			font.TextOut("Rotation: ("  + ftstr(activeRot.x) + ", " + ftstr(activeRot.y) + ", " + ftstr(activeRot.z) + ")",   TEXT_LEFT_MARGIN, TEXT_HEIGHT - (TEXT_LINE_SPACING*2), 0);
			
			font.TextOut("Scale:     (" + ftstr(activeScale.x) + ", " + ftstr(activeScale.y) + ", " + ftstr(activeScale.z) + ")",  TEXT_LEFT_MARGIN, TEXT_HEIGHT - (TEXT_LINE_SPACING*3), 0);
		}
		catch(GLFontError::InvalidFont)
		{
			abort();
		}
		glDisable(GL_TEXTURE_2D);
	}

	/**
	*	Handle different events like inputs or app close
	**/
	void aut::Application::handleEvents()
	{
		sf::Event event;

		while (window.pollEvent (event))
		{
			if (event.type == sf::Event::Closed)
			{
				running = false;
			}
			else if(event.type == sf::Event::MouseWheelMoved)
			{
				view.ScaleActive(float(event.mouseWheel.delta));
			}
			else if(event.type == sf::Event::MouseButtonPressed)
			{
				//Rotate
				if(sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
				{
					if(
						sf::Mouse::isButtonPressed(sf::Mouse::Left)  || 
						sf::Mouse::isButtonPressed(sf::Mouse::Right) || 
						sf::Mouse::isButtonPressed(sf::Mouse::Middle)
					  )
					{
						rotationOrigin = sf::Mouse::getPosition();
					}
				}
				//Move
				else
				{
					if(sf::Mouse::isButtonPressed(sf::Mouse::Left) || 
						sf::Mouse::isButtonPressed(sf::Mouse::Right))
					{
						translationOrigin = sf::Mouse::getPosition();
					}
				}
			}
		}

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::L))
		{
			wireframe = !wireframe;
			window.setKeyRepeatEnabled(false);
		}

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
		{
			activeText = "Camera";
			active_element = 1;
			view.setActiveMesh(active_element);
		}
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
		{
			activeText = "OBJ1";
			active_element = 2;
			view.setActiveMesh(active_element);
		}
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num3))
		{
			activeText = "OBJ2";
			active_element = 3;
			view.setActiveMesh(active_element);
		}

		//Rotate
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
		{
			if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				sf::Vector2<int> mousepos = sf::Mouse::getPosition();
				view.RotateActive(float(mousepos.y - rotationOrigin.y), 0, 0);	//Rotate in Z
				
				rotationOrigin = mousepos;
			}
			else if(sf::Mouse::isButtonPressed(sf::Mouse::Middle))
			{
				sf::Vector2<int> mousepos = sf::Mouse::getPosition();
				view.RotateActive(0, float(rotationOrigin.y - mousepos.y), 0);	//Rotate in Y
				rotationOrigin = mousepos;
			}
			else if(sf::Mouse::isButtonPressed(sf::Mouse::Right))
			{
				sf::Vector2<int> mousepos = sf::Mouse::getPosition();
				view.RotateActive(0, 0, float(mousepos.y - rotationOrigin.y));	//Rotate in Z
				rotationOrigin = mousepos;
			}
			
		}
		//Move
		else
		{
			if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				sf::Vector2<int> mousepos = sf::Mouse::getPosition();
				view.MoveActive(float(mousepos.x - translationOrigin.x), float(translationOrigin.y - mousepos.y), 0);	//Move in X and Y
				translationOrigin = mousepos;
			}
			else if(sf::Mouse::isButtonPressed(sf::Mouse::Right))
			{
				sf::Vector2<int> mousepos = sf::Mouse::getPosition();
				view.MoveActive(0, 0, float(mousepos.y - translationOrigin.y));	//Move in Z
				translationOrigin = mousepos;
			}
		}
	}

}