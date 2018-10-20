#pragma once
#include <iostream>

class Input
{
protected:

	float Forward = 0.f;
	float Right = 0.f;
	float Up = 0.f;

	uint8_t LeftMouseButtonDown : 1;
	uint8_t RightMouseButtonDown : 1;

public:

	void keyboardInputDown(unsigned char Key, int x, int y);
	void keyboardInputUp(unsigned char Key, int x, int y);
	void mouseInput(int button, int state, int x, int y);
};