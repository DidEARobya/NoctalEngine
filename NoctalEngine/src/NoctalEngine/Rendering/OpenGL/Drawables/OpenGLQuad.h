#pragma once
#include "OpenGLBaseObject.h"

class OpenGLQuad : public OpenGLBaseObject<OpenGLQuad>
{
public:
	OpenGLQuad(glm::vec2 position = glm::vec2(0.0f), glm::vec2 scale = glm::vec2(1.0f));
};