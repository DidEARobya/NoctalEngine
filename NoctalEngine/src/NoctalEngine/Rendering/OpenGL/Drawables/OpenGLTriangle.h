#pragma once
#include "OpenGLBaseObject.h"

class OpenGLTriangle : public OpenGLBaseObject
{
public:
	OpenGLTriangle(glm::vec2 position = glm::vec2(0.0f), glm::vec2 scale = glm::vec2(1.0f));
};