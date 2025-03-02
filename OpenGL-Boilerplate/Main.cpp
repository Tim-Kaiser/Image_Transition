#include <glm.hpp>
#include <iostream>
#include <SDL.h>
#include <glad/glad.h>


#include "Screen.h"
#include "Input.h"
#include "Shader.h"
#include "Quad.h"

bool running = true;

int main(int argc, char* arfv[]) {
	Screen::Instance()->Init();

	//===== SHADER INIT =====
	if (!Shader::Instance()->CreateProgram()) {
		return 0;
	}

	if (!Shader::Instance()->CreateShaders()) {
		return 0;
	}
	if (!Shader::Instance()->CompileShaders("Shaders/main.vert", Shader::ShaderType::VERTEX_SHADER)) {
		return -1;
	}
	if (!Shader::Instance()->CompileShaders("Shaders/main.frag", Shader::ShaderType::FRAGMENT_SHADER)) {
		return -1;
	}
	Shader::Instance()->AttachShaders();

	if (!Shader::Instance()->LinkProgram()) {
		return 0;
	}



	Quad quad;

	//==================

	while (running) {
		Screen::Instance()->Clear();
		Input::Instance()->Update();
		if (Input::Instance()->isXClicked()) {
			running = false;
		}

		quad.Render();

		Screen::Instance()->SwapBuf();
	}

	Shader::Instance()->DetachShaders();
	Shader::Instance()->DestroyShaders();
	Shader::Instance()->DestroyProgram();
	Screen::Instance()->Close();
	return 0;
}