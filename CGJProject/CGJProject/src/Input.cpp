#include "Input.h"
#include "Math/Vector.h"

#include "GL/glew.h"
#include "GL/freeglut.h"

void Input::keyboardButtontDown(unsigned char Key)
{
	//Movement
	if (Key == 'w') movement.z += 1.f;
	if (Key == 's') movement.z -= 1.f;
	if (Key == 'd') movement.x += 1.f;
	if (Key == 'a') movement.x -= 1.f;
	if (Key == 'e') movement.y += 1.f;
	if (Key == 'q') movement.y -= 1.f;
	if (Key == 'x') windAxis += 1.f;
	if (Key == 'z') windAxis -= 1.f;

	//Post Processing
	if (Key == 'b') RGBIndex = 0;
	if (Key == 'n') RGBIndex = 1;
	if (Key == 'm') RGBIndex = 2;
	if (Key == '.') intensityChange += 1;
	if (Key == ',') intensityChange -= 1;
	if (Key == 'l') distortionAmountChange += 1;
	if (Key == 'k') distortionAmountChange -= 1;
	if (Key == 'o') distortionSpeedChange += 1;
	if (Key == 'i') distortionSpeedChange -= 1;
	if (Key == '9') distortionFrequencyChange += 1;
	if (Key == '8') distortionFrequencyChange -= 1;
	if (Key == 'p')
	{
		postProcessingMode += 1;
		if (postProcessingMode >= numModes) postProcessingMode = 0;
	}

	if (Key == 'f' || Key == 'F') fog = ! fog;

	if (Key == 27) exit(1);
}

void Input::keyboardButtonUp(unsigned char Key)
{
	if (Key == 'w') movement.z -= 1.f;
	if (Key == 's') movement.z += 1.f;
	if (Key == 'd') movement.x -= 1.f;
	if (Key == 'a') movement.x += 1.f;
	if (Key == 'e') movement.y -= 1.f;
	if (Key == 'q') movement.y += 1.f;
	
	if (Key == 'x') windAxis -= 1.f;
	if (Key == 'z') windAxis += 1.f;
	
	if (Key == '.') intensityChange -= 1;
	if (Key == ',') intensityChange += 1;
	if (Key == 'l') distortionAmountChange -= 1;
	if (Key == 'k') distortionAmountChange += 1;
	if (Key == 'o') distortionSpeedChange -= 1;
	if (Key == 'i') distortionSpeedChange += 1;
	if (Key == '9') distortionFrequencyChange -= 1;
	if (Key == '8') distortionFrequencyChange += 1;
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
