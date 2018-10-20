#include "Input.h"

#include "GL/glew.h"
#include "GL/freeglut.h"

void Input::keyboardButtontDown(unsigned char Key, int x, int y)
{
	switch (Key)
	{
		case 'w': 
			Forward += 1.f;
			break;
		case 's':
			Forward -= 1.f;
			break;
		case 'd':
			Right += 1.f;
			break;
		case 'a':
			Right -= 1.f;
			break;
		case 'e':
			Up += 1.f;
			break;
		case 'q':
			Up -= 1.f;
			break;
		case 27:
			exit(1);
			break;
	};
}

void Input::keyboardButtonUp(unsigned char Key, int x, int y)
{
	switch (Key)
	{
		case 'w':
			Forward -= 1.f;
			break;
		case 's':
			Forward += 1.f;
			break;
		case 'd':
			Right -= 1.f;
			break;
		case 'a':
			Right += 1.f;
			break;
		case 'e':
			Up -= 1.f;
			break;
		case 'q':
			Up += 1.f;
			break;
		case 27:
			exit(1);
			break;
	};
}

void Input::mouseButton(int button, int state, int x, int y)
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

void Input::mouseMove(int x, int y)
{
}
