#pragma once
#include <glad/glad.h>
#include "Buffer.h"
#include <glm.hpp>

class Quad {

private:
	Buffer m_buffer;
	glm::vec3 m_position;

public:
	Quad();
	~Quad();

	void Render();

};