#include "Input.h"

#include "GL/glew.h"
#include "GL/freeglut.h"

void Input::keyboardInputDown(unsigned char Key, int x, int y)
{
	switch (Key)
	{
		case 'w': 
			std::cout << "W!" << std::endl;
			break;
		case 27:
			exit(1);
			break;
	};
}

void Input::keyboardInputUp(unsigned char Key, int x, int y)
{
	switch (Key)
	{
		case 'w':
			std::cout << "W!" << std::endl;
			break;
		case 27:
			exit(1);
			break;
	};
}

//Add "glutMouseFunc(MyMouseFunc)" in the opengl init func.

void Input::mouseInput(int button, int state, int x, int y)
{
	switch (button)
	{
		case GLUT_LEFT_BUTTON:
			LeftMouseButtonDown = state == GLUT_DOWN;
			break;
		case GLUT_RIGHT_BUTTON:
			RightMouseButtonDown = state == GLUT_DOWN;
			break;
	};
}