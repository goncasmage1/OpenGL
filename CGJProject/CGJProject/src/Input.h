#pragma once
#include <iostream>
#include "Math/Public/Vector.h"

class Input
{
protected:

	float Forward = 0.f;
	float Right = 0.f;
	float Up = 0.f;

	struct Vec2 MouseSensitivity = Vec2(50000.f);
	struct Vec3 MoveSensitivity = Vec3(0.01f);

	struct Vec2 MouseDelta;
	struct Vec2 PreviousMousePosition;

	uint8_t LeftMouseButtonDown : 1;
	uint8_t RightMouseButtonDown : 1;
	uint8_t MouseMoved : 1;

public:

	void keyboardButtontDown(unsigned char Key, int x, int y);
	void keyboardButtonUp(unsigned char Key, int x, int y);
	void mouseButton(int button, int state, int x, int y);
	void mouseMove(int x, int y);

	struct Vec2 GetMouseDelta();
	float GetForwardAxis() const { return Forward * MoveSensitivity.x; }
	float GetRightAxis() const { return Right * MoveSensitivity.y; }
	float GetUpAxis() const { return Up * MoveSensitivity.z; }
};