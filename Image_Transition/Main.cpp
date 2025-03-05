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

	GLuint fBuffer;
	glGenFramebuffers(1, &fBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, fBuffer);
	GLuint fCol;

	ScreenDimensions dims = Screen::Instance()->getDims();

	glGenTextures(1, &fCol);
	glBindTexture(GL_TEXTURE_2D, fCol);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, dims.width, dims.height, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fCol, 0);

	GLuint attachments[1] = {GL_COLOR_ATTACHMENT0};
	glDrawBuffers(1, attachments);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);



	Quad quad;

	//==================

	while (running) {
		Screen::Instance()->Clear();
		glEnable(GL_DEPTH_TEST);
		Input::Instance()->Update();
		if (Input::Instance()->isXClicked()) {
			running = false;
		}

		glBindFramebuffer(GL_FRAMEBUFFER, fBuffer);
		quad.Render();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		Screen::Instance()->Clear();
		glDisable(GL_DEPTH_TEST);

		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, fCol);

		quad.Render();

		Screen::Instance()->SwapBuf();
	}

	Shader::Instance()->DetachShaders();
	Shader::Instance()->DestroyShaders();
	Shader::Instance()->DestroyProgram();
	Screen::Instance()->Close();
	return 0;
}