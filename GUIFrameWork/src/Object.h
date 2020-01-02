#pragma once
#include "glm\glm.hpp"
class Window;
class Object {
	friend class Window;
public:
	Window& window;
	glm::ivec3 position;
	Object();
	virtual void Draw() {};
	virtual ~Object() {};
};