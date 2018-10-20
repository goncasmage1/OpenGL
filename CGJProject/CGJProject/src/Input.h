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

	void keyboardButtontDown(unsigned char Key, int x, int y);
	void keyboardButtonUp(unsigned char Key, int x, int y);
	void mouseButton(int button, int state, int x, int y);
	void mouseMove(int x, int y);
};