#include "Input.h"
#include "Math/Vector.h"

#include "GL/glew.h"
#include "GL/freeglut.h"

void Input::keyboardButtontDown(unsigned char Key)
{
	switch (Key)
	{
		case 'w': 
			movement.z += 1.f;
			break;
		case 's':
			movement.z -= 1.f;
			break;
		case 'd':
			movement.x += 1.f;
			break;
		case 'a':
			movement.x -= 1.f;
			break;
		case 'e':
			movement.y += 1.f;
			break;
		case 'q':
			movement.y -= 1.f;
			break;
		case 'p':
			bPDown = !bPDown;
			break;
		case 'g':
			bGDown = !bGDown;
			break;
		case 'm':
			AnimationDirection += 1;
			break;
		case 'n':
			AnimationDirection -= 1;
			break;
		case 27:
			exit(1);
			break;
	};
}

void Input::keyboardButtonUp(unsigned char Key)
{
	switch (Key)
	{
		case 'w':
			movement.z -= 1.f;
			break;
		case 's':
			movement.z += 1.f;
			break;
		case 'd':
			movement.x -= 1.f;
			break;
		case 'a':
			movement.x += 1.f;
			break;
		case 'e':
			movement.y -= 1.f;
			break;
		case 'q':
			movement.y += 1.f;
			break;
		case 'm':
			AnimationDirection -= 1;
			break;
		case 'n':
			AnimationDirection += 1;
			break;
		case 27:
			exit(1);
			break;
	};
}

void Input::mouseButton(int button, int state)
{
	switch (button)
	{
		case GLUT_LEFT_BUTTON:
			bLeftMouseButtonDown = state == GLUT_DOWN;
			break;
		case GLUT_RIGHT_BUTTON:
			bRightMouseButtonDown = state == GLUT_DOWN;
			break;
		case GLUT_MIDDLE_BUTTON:
			bMiddleMouseButtonDown = state == GLUT_DOWN;
			break;
	};
}

void Input::mouseMove(int x, int y)
{
	bMouseMoved = true;
	MouseDelta.x = x - PreviousMousePosition.x;
	MouseDelta.y = y - PreviousMousePosition.y;

	PreviousMousePosition.x = x;
	PreviousMousePosition.y = y;
}

void Input::mouseWheel(int direction)
{
	bWheelMoved = true;
	wheelDirection = direction;
}

Vec2 Input::GetMouseDelta()
{
	if (bMouseMoved && (bRightMouseButtonDown || bMiddleMouseButtonDown))
	{
		bMouseMoved = false;
		return MouseDelta * MouseSensitivity;
	}
	return Vec2();
}

float Input::GetWheelDelta()
{
	if (bWheelMoved)
	{
		bWheelMoved = false;
		return wheelDirection * WheelSensitivity;
	}
	return 0.f;
}
