#pragma once
#include <iostream>
#include "Math/Public/Vector.h"

class Input
{
protected:

	struct Vec3 movement = Vec3(0.f);
	float Forward = 0.f;
	float Right = 0.f;
	float Up = 0.f;

	float SpeedMultiplier = 4.f;

	struct Vec2 MouseSensitivity = Vec2(50000.f);
	struct Vec3 MoveSensitivity = Vec3(0.04f);

	struct Vec2 MouseDelta;
	struct Vec2 PreviousMousePosition;

	uint8_t bLeftMouseButtonDown : 1;
	uint8_t bRightMouseButtonDown : 1;
	uint8_t bMouseMoved : 1;
	uint8_t bUsePerspective : 1;

public:

	void keyboardButtontDown(unsigned char Key, int x, int y);
	void keyboardButtonUp(unsigned char Key, int x, int y);
	void mouseButton(int button, int state, int x, int y);
	void mouseMove(int x, int y);

	struct Vec2 GetMouseDelta();
	struct Vec3 GetMovement() { return movement * MoveSensitivity; }
	bool GetUsePerspective() const { return bUsePerspective; }
};