#include "Input.h"
#include <iostream>

Input::Input()
{
    Reset();
}

void Input::Reset()
{
    m_X = false;
    m_keyPressed = false;

    m_keyUp = 0;
    m_keyDown = 0;

    m_isLeftMouseButton = false;
    m_isMiddleMouseButton = false;
    m_isRightMouseButton = false;

    m_mouseX = 0;
    m_mouseY = 0;

    m_mouseDeltaX = 0;
    m_mouseDeltaY = 0;

}

Input* Input::Instance()
{
    static Input* input = new Input();
    return input;
}

void Input::Update()
{
	SDL_Event event;

	Reset();

	while (SDL_PollEvent(&event)) {

		switch (event.type) {
		case SDL_KEYDOWN:
		{
			m_keyPressed = true;
			m_keyDown = event.key.keysym.sym;
			if (m_keyDown == SDLK_ESCAPE) {
				m_X = true;
			}
			break;
		}
		case SDL_KEYUP:
		{
			m_keyPressed = false;
			m_keyUp = event.key.keysym.sym;
			break;
		}
		case SDL_QUIT:
		{
			m_X = true;
			break;
		}
		case SDL_MOUSEBUTTONDOWN:
		{
			switch (event.button.button) {
			case SDL_BUTTON_LEFT:
			{
				m_isLeftMouseButton = true;
				break;
			}
			case SDL_BUTTON_MIDDLE:
			{
				m_isMiddleMouseButton = true;
				break;
			}
			case SDL_BUTTON_RIGHT:
			{
				m_isRightMouseButton = true;
				break;
			}
			}
			break;
		}
		case SDL_MOUSEBUTTONUP:
		{
			switch (event.button.button) {
			case SDL_BUTTON_LEFT:
			{
				m_isLeftMouseButton = false;
				break;
			}
			case SDL_BUTTON_MIDDLE:
			{
				m_isMiddleMouseButton = false;
				break;
			}
			case SDL_BUTTON_RIGHT:
			{
				m_isRightMouseButton = false;
				break;
			}
			}
			break;
		}
		case SDL_MOUSEMOTION:
		{
			m_mouseDeltaX = event.motion.xrel;
			m_mouseDeltaY = event.motion.yrel;

			m_mouseX = event.motion.x;
			m_mouseY = event.motion.y;

		}
		}
	}
}


// Key functions
bool Input::isXClicked()
{
    return m_X;
}

bool Input::isKeyPressed()
{
    return m_keyPressed;
}

char Input::getKeyUp()
{
    return m_keyUp;
}

char Input::getKeyDown()
{
    return m_keyDown;
}

bool Input::isLeftMouseClicked()
{
    return m_isLeftMouseButton;
}

bool Input::isMiddleMouseClicked()
{
    return m_isMiddleMouseButton;
}

bool Input::isRightMouseClicked()
{
    return m_isRightMouseButton;
}

int Input::getMouseX()
{
    return m_mouseX;
}

int Input::getMouseY()
{
    return m_mouseY;
}

int Input::getMouseDeltaX()
{
    return m_mouseDeltaX;
}

int Input::getMouseDeltaY()
{
    return m_mouseDeltaY;
}
