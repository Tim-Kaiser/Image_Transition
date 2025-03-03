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
	// "Textures/milkyway_earth.jpg"
	// "Textures/Frame_sticker.png"
	// "Textures/Frame_snow.png"
	// "Textures/Frame_forest.png"
	// "Textures/Frame_fire.png"
	// "Textures/hubble-eyes-galactic-refurbishment_17322896925_o.jpg"
	if (!Shader::Instance()->CreateTexture1("Textures/milkyway_earth.jpg")) {
		return 0;
	}
	if (!Shader::Instance()->CreateTexture0("Textures/Frame_sticker.png")) {
		return 0;
	}
	if (!Shader::Instance()->CreateTexture2("Textures/Frame_snow.png")) {
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