#pragma once
#include <iostream>
#include "Math/Vector.h"

class Input
{
protected:

	struct Vec3 movement = Vec3(0.f);
	int wheelDirection = 0;

	//Post-Processing control
	int RGBIndex = 0;
	int intensityChange = 0;
	float intensityChangeSpeed = 0.01f;
	int postProcessingMode = 0;
	int numModes = 3;

	struct Vec2 MouseSensitivity = Vec2(1.f);
	struct Vec3 MoveSensitivity = Vec3(0.1f);
	float WheelSensitivity = 0.9f;

	struct Vec2 MouseDelta;
	struct Vec2 PreviousMousePosition;

	uint8_t bLeftMouseButtonDown : 1;
	uint8_t bRightMouseButtonDown : 1;
	uint8_t bMiddleMouseButtonDown : 1;
	uint8_t bMouseMoved : 1;
	uint8_t bWheelMoved : 1;

public:

	void keyboardButtontDown(unsigned char Key);
	void keyboardButtonUp(unsigned char Key);
	void mouseButton(int button, int state);
	void mouseMove(int x, int y);
	void mouseWheel(int direction);

	struct Vec2 GetMouseDelta();
	struct Vec3 GetMovement() { return movement * MoveSensitivity; }
	float GetWheelDelta();
	bool IsMiddleMouseButtonDown() const { return bMiddleMouseButtonDown; }
	int GetRGBIndex() const { return RGBIndex; }
	float GetIntensityChange() const { return intensityChange * intensityChangeSpeed; }
	int GetPostProcessingMode() const { return postProcessingMode; }

};