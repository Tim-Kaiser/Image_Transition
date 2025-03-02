#pragma once

#include <SDL.h>

class Input {

private:

	Input();
	Input(const Input&);
	Input& operator=(const Input&);

	void Reset();

// Member attributes
private:
	bool m_X;
	bool m_keyPressed;

	char m_keyUp;
	char m_keyDown;

	bool m_isLeftMouseButton;
	bool m_isMiddleMouseButton;
	bool m_isRightMouseButton;

	int m_mouseX;
	int m_mouseY;

	int m_mouseDeltaX;
	int m_mouseDeltaY;

public:
	static Input* Instance();
	void Update();

	bool isXClicked();
	bool isKeyPressed();

	char getKeyUp();
	char getKeyDown();

	bool isLeftMouseClicked();
	bool isMiddleMouseClicked();
	bool isRightMouseClicked();

	int getMouseX();
	int getMouseY();

	int getMouseDeltaX();
	int getMouseDeltaY();

};