#include "Application.hpp"

int main()
{
	aut::Application app;
	app.initialize();
	app.start();
	
	return (EXIT_SUCCESS);
}

/**
Controles:

MouseLeft:						Move in X and Y axis.
MouseRight:						Move in Z axis.

ControlLeft + MouseLeft.Y:		Rotate in X axis.
ControlLeft + MouseMiddle.Y:	Rotate in Y axis.
ControlLeft + MouseRight.Y:		Rotate in Z axis.

MouseWheel:						Scale. (Camera has no scale)

**/