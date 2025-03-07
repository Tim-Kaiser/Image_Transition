#define GLFW_INCLUDE_NONE
#define STB_IMAGE_IMPLEMENTATION

#include "Window.h"
#include "ShaderLoader.h"
#include "ObjectLoader.h"
#include "Model.h"
#include "stb_image.h"

#include <vector>
#include <glm.hpp>

constexpr auto PARTICLE_COUNT = 60000;

float lastMouseX;
float lastMouseY;
bool firstMouseMove = true;

bool genTexture(const std::string& texturePath, GLuint& texID)
{
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	stbi_set_flip_vertically_on_load(true);

	int width, height, channels;
	unsigned char* data = stbi_load(texturePath.c_str(), &width, &height, &channels, 0);
	if (data)
	{
		GLuint format = channels == 3 ? GL_RGB : GL_RGBA;

		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		return false;
	}
	stbi_image_free(data);

	return true;


}


int main(int argc, char* arfv[]) {

	Window window(2540, 1080);
	//===== SHADER INIT =====
	ShaderLoader shaderLoader;

	std::unique_ptr<Shader> renderShader = shaderLoader.CreateShaders();
	shaderLoader.CompileShaders("Shaders/main.vert", renderShader->m_vertexShaderID);
	shaderLoader.CompileShaders("Shaders/main.frag", renderShader->m_fragmentShaderID);

	shaderLoader.AttachShaders(*renderShader);
	shaderLoader.LinkProgram(*renderShader);

	std::unique_ptr<Shader> simulationShader = shaderLoader.CreateShaders();
	shaderLoader.CompileShaders("Shaders/simulation.vert", simulationShader->m_vertexShaderID);
	shaderLoader.CompileShaders("Shaders/simulation.frag", simulationShader->m_fragmentShaderID);

	shaderLoader.AttachShaders(*simulationShader);
	shaderLoader.LinkProgram(*simulationShader);
	//glUseProgram(simulationShader->m_shaderProgramID);

	Object obj;
	loadObject("Objects/quad.obj", obj); 
	Model quadModel(&obj, false);


	GLuint gBuffer;
	glGenFramebuffers(1, &gBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
	GLuint texSimOut;

	int width, height;

	window.getSize(&width, &height);

	glGenTextures(1, &texSimOut);
	glBindTexture(GL_TEXTURE_2D, texSimOut);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texSimOut, 0);

	GLuint attachments[3] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, attachments);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glUseProgram(renderShader->m_shaderProgramID);
	GLint texSimOutID = 0;
	shaderLoader.SendUniformData("texSimOut", texSimOutID);

	// == TEXTURE READ RENDERING ==
	GLuint texSimIn;

	glGenTextures(1, &texSimIn);
	glBindTexture(GL_TEXTURE_2D, texSimIn);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);

	glGenerateMipmap(GL_TEXTURE_2D);

	glUseProgram(simulationShader->m_shaderProgramID);
	GLint texSimInID = 0;
	GLint setup = 1;
	shaderLoader.SendUniformData("texSimIn", texSimInID);
	shaderLoader.SendUniformData("setup", setup);



	// == TEXTURE READ SIM ==
	std::string texPath0 = "Textures/Frame_sticker.png";
	std::string texPath1 = "Textures/hubble-eyes-galactic-refurbishment_17322896925_o.jpg";
	GLuint texID0, texID1;
	genTexture(texPath0, texID0);
	genTexture(texPath1, texID1);


	glUseProgram(simulationShader->m_shaderProgramID);
	GLint ID_1 = 1;
	GLint ID_2 = 2;

	shaderLoader.SendUniformData("startFrame", ID_1);
	shaderLoader.SendUniformData("endFrame", ID_2);


	glClearColor(0.0, 0.0, 0.0, 0.0);
	while (window.Open()) {

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(simulationShader->m_shaderProgramID);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texSimIn);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texID0);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, texID1);


		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, gBuffer);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		quadModel.Render();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		setup = 0;
		shaderLoader.SendUniformData("setup", setup);

		glUseProgram(renderShader->m_shaderProgramID);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texSimOut);

		quadModel.Render();

		// swap tex contents
		glCopyImageSubData(texSimOut, GL_TEXTURE_2D, 0, 0, 0, 0, texSimIn, GL_TEXTURE_2D, 0, 0, 0, 0, width, height, 1);

		window.Update();
	}

	shaderLoader.DetachShaders(*renderShader);

	shaderLoader.DestroyShaders(*renderShader);
	shaderLoader.DestroyProgram(*renderShader);

	shaderLoader.DestroyShaders(*simulationShader);
	shaderLoader.DestroyProgram(*simulationShader);
	return 0;
}